/*
 * Cryptographic API.
 *
 * Support for Novatek NA51000 Crypto Hardware acceleration.
 *
 * Copyright (c) 2017 Novatek Inc.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as published
 * by the Free Software Foundation.
 *
 */

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/platform_device.h>
#include <linux/scatterlist.h>
#include <linux/dma-mapping.h>
#include <linux/interrupt.h>
#include <linux/of.h>
#include <linux/clk.h>
#include <linux/crypto.h>

#include <crypto/algapi.h>
#include <crypto/aes.h>
#include <crypto/des.h>
#include <crypto/scatterwalk.h>

#include "na51000_crypto.h"

#define DRV_VERSION  "1.00.001"

#define ALIGN_N(x, a)                   ((x)&(~(a-1)))
#define BSWAP(a)                        (((((a) & 0xff000000)>>24) | (((a) & 0x00ff0000)>>8) | (((a) & 0x0000ff00)<<8) | (((a) & 0x000000ff)<<24)))
#define NA5100_CRYPTO_QUEUE_LENGTH      10
#define NA5100_CRYPTO_DEFAULT_TIMEOUT   3000    ///< 3 sec
#define NA5100_CRYPTO_ALG_PRIORITY      1000

struct na51000_crypto_ctx {
	struct na51000_crypto_dev    *dev;
	int                          keylen;
	u32                          key[AES_KEYSIZE_256/sizeof(u32)];
	u32                          block_size;
	struct crypto_ablkcipher     *fallback;
};

struct na51000_crypto_reqctx {
	NA51000_CRYPTO_TYPE_T        type;
	NA51000_CRYPTO_MODE_T        mode;
	NA51000_CRYPTO_OPMODE_T      opmode;
	NA51000_CRYPTO_ACCMODE_T     accmode;
	NA51000_CRYPTO_BYTE_ENDIAN_T byte_endian;
	NA51000_CRYPTO_BIT_ORDER_T   bit_order;
};

struct na51000_crypto_dev {
	struct device                *dev;
	struct clk                   *clk;
	void __iomem                 *iobase;
	int                          irq;
	spinlock_t                   lock;
	bool                         busy;

	struct timer_list            timer;                                         ///< device timeout timer

	struct crypto_queue          queue;

	struct tasklet_struct        queue_tasklet;
	struct tasklet_struct        done_tasklet;

	struct ablkcipher_request    *req;
	struct na51000_crypto_ctx    *ctx;
	struct scatterlist           *sg_src;
	struct scatterlist           *sg_dst;

	u32                          iv[NA51000_CRYPTO_MAX_IV_SIZE/sizeof(u32)];    ///< for store current request IV

	int                          sg_same;                                       ///< source and destination use same buffer

	int                          src_copied;
	int                          dst_copied;
	struct scatterlist           sg_src_cpy;
	struct scatterlist           sg_dst_cpy;

	size_t                       sg_src_len;
	size_t                       sg_src_ofs;
	size_t                       sg_dst_len;
	size_t                       sg_dst_ofs;

	size_t                       src_total;
	size_t                       dst_total;
	size_t                       dma_total;
};

static struct na51000_crypto_dev *na51000_cdev = NULL;

static inline u32 na51000_crypto_read(struct na51000_crypto_dev *dd, u32 offset)
{
	return readl(dd->iobase + offset);
}

static inline void na51000_crypto_write(struct na51000_crypto_dev *dd, u32 offset, u32 value)
{
	writel(value, dd->iobase + offset);
}

static void na51000_crypto_reset(struct na51000_crypto_dev *dd)
{
	na51000_crypto_write(dd, NA51000_CRYPTO_CTRL_REG, 0x01);    ///< hardware will auto clear
}

static int na51000_crypto_handle_req(struct na51000_crypto_dev *dev, struct ablkcipher_request *req)
{
	unsigned long flags;
	int err;

	spin_lock_irqsave(&dev->lock, flags);
	err = ablkcipher_enqueue_request(&dev->queue, req);
	if (dev->busy) {
		spin_unlock_irqrestore(&dev->lock, flags);
		return err;
	}
	dev->busy = true;
	spin_unlock_irqrestore(&dev->lock, flags);

	tasklet_schedule(&dev->queue_tasklet);

	return err;
}

static int na51000_crypto_update_ctr_iv(struct na51000_crypto_dev *dev)
{
	int i, cnt, blocks, update = 0;
	u32 data;
	struct na51000_crypto_ctx *ctx = dev->ctx;

	cnt    = ctx->block_size>>2;
	blocks = dev->sg_src_len/ctx->block_size;
	for (i=cnt-1; i>=0; i--) {
		data = BSWAP(dev->iv[i]);
		if ((data + blocks) < data) {    ///< overflow
			dev->iv[i] = BSWAP(data + blocks);
			blocks = 1;
			update++;
		}
		else {
			dev->iv[i] = BSWAP(data + blocks);
			update++;
			goto exit;
		}
	}

exit:
	return (update ? 1 : 0);
}

static int na51000_crypto_update_ofb_iv(struct na51000_crypto_dev *dev)
{
	int i, cnt;
	u32 tmp, status;
	u32 counter = NA51000_CRYPTO_TIMEOUT;
	int update  = 0;
	struct na51000_crypto_ctx    *ctx    = dev->ctx;
	struct na51000_crypto_reqctx *reqctx = ablkcipher_request_ctx(dev->req);

	/* check hardware status */
	status = na51000_crypto_read(dev, NA51000_CRYPTO_FLAG_REG);
	if(status & NA51000_CRYPTO_FLAG_BUSY_BIT) {
		dev_err(dev->dev, "crypto hardware busy!!\n");
		return 0;
	}

	if (IS_ALIGNED(dev->sg_src_len, NA51000_CRYPTO_CIV_WORK_SIZE)) {
		switch (reqctx->mode) {
		case NA51000_CRYPTO_MODE_DES:
		case NA51000_CRYPTO_MODE_3DES:
			dev->iv[0] = na51000_crypto_read(dev, NA51000_CRYPTO_CURR_IV0_REG);
			dev->iv[1] = na51000_crypto_read(dev, NA51000_CRYPTO_CURR_IV1_REG);
			break;
		case NA51000_CRYPTO_MODE_AES_128:
		case NA51000_CRYPTO_MODE_AES_256:
			dev->iv[0] = na51000_crypto_read(dev, NA51000_CRYPTO_CURR_IV0_REG);
			dev->iv[1] = na51000_crypto_read(dev, NA51000_CRYPTO_CURR_IV1_REG);
			dev->iv[2] = na51000_crypto_read(dev, NA51000_CRYPTO_CURR_IV2_REG);
			dev->iv[3] = na51000_crypto_read(dev, NA51000_CRYPTO_CURR_IV3_REG);
			break;
		default:
			return 0;
		}
		update = 1;
	}
	else {
		/* disable DMA transfer */
		na51000_crypto_write(dev, NA51000_CRYPTO_DMA_CTRL_REG, 0);

		/* disable DMA interrupt mask */
		na51000_crypto_write(dev, NA51000_CRYPTO_DMA_CFG_REG, 0);

		/* clear DMA interrupt status */
		na51000_crypto_write(dev, NA51000_CRYPTO_DMA_STS_REG, 1);

		/* set mode, use pio mode to create data */
		tmp = (reqctx->mode<<1)                   | \
			  (NA51000_CRYPTO_OPMODE_ECB<<4)      | \
			  (NA51000_CRYPTO_TYPE_ENCRYPT<<8)    | \
			  (NA51000_CRYPTO_BYTE_ENDIAN_BIG<<9) | \
			  (NA51000_CRYPTO_BIT_ORDER_0_7<<10)  | \
			  (NA51000_CRYPTO_ACCMODE_PIO<<11);
		na51000_crypto_write(dev, NA51000_CRYPTO_CTRL_REG, tmp);

		cnt = dev->sg_src_len/ctx->block_size;
		for (i=0; i<cnt; i++) {
			/* use IV as input data */
			switch (reqctx->mode) {
			case NA51000_CRYPTO_MODE_DES:
			case NA51000_CRYPTO_MODE_3DES:
				na51000_crypto_write(dev, NA51000_CRYPTO_IN1_REG, dev->iv[1]);
				na51000_crypto_write(dev, NA51000_CRYPTO_IN0_REG, dev->iv[0]);  ///< write this register will trigger hardware
				break;
			case NA51000_CRYPTO_MODE_AES_128:
			case NA51000_CRYPTO_MODE_AES_256:
				na51000_crypto_write(dev, NA51000_CRYPTO_IN3_REG, dev->iv[3]);
				na51000_crypto_write(dev, NA51000_CRYPTO_IN2_REG, dev->iv[2]);
				na51000_crypto_write(dev, NA51000_CRYPTO_IN1_REG, dev->iv[1]);
				na51000_crypto_write(dev, NA51000_CRYPTO_IN0_REG, dev->iv[0]);  ///< write this register will trigger hardware
				break;
			default:
				goto error;
			}

			/* check status */
			do {
				status = na51000_crypto_read(dev, NA51000_CRYPTO_FLAG_REG);
			} while ((status & NA51000_CRYPTO_FLAG_BUSY_BIT) && --counter);

			if(status & NA51000_CRYPTO_FLAG_BUSY_BIT) {
				dev_err(dev->dev, "crypto hardware busy!!\n");
				na51000_crypto_reset(dev);
				goto error;
			}

			/* get output data */
			switch (reqctx->mode) {
			case NA51000_CRYPTO_MODE_DES:
			case NA51000_CRYPTO_MODE_3DES:
				dev->iv[0] = na51000_crypto_read(dev, NA51000_CRYPTO_OUT0_REG);
				dev->iv[1] = na51000_crypto_read(dev, NA51000_CRYPTO_OUT1_REG);
				break;
			case NA51000_CRYPTO_MODE_AES_128:
			case NA51000_CRYPTO_MODE_AES_256:
				dev->iv[0] = na51000_crypto_read(dev, NA51000_CRYPTO_OUT0_REG);
				dev->iv[1] = na51000_crypto_read(dev, NA51000_CRYPTO_OUT1_REG);
				dev->iv[2] = na51000_crypto_read(dev, NA51000_CRYPTO_OUT2_REG);
				dev->iv[3] = na51000_crypto_read(dev, NA51000_CRYPTO_OUT3_REG);
				break;
			default:
				goto error;
			}
		}
		update = 1;

error:
		/* return to previous mode */
		tmp = (reqctx->mode<<1)        | \
			  (reqctx->opmode<<4)      | \
			  (reqctx->type<<8)        | \
			  (reqctx->byte_endian<<9) | \
			  (reqctx->bit_order<<10)  | \
			  (reqctx->accmode<<11);
		na51000_crypto_write(dev, NA51000_CRYPTO_CTRL_REG, tmp);

		/* enable DMA interrupt mask */
		na51000_crypto_write(dev, NA51000_CRYPTO_DMA_CFG_REG, 0x1);
	}

	return update;
}

static void na51000_crypto_sg_copy_buf(void *buf, struct scatterlist *sg, unsigned int start, unsigned int nbytes, int out)
{
	struct scatter_walk walk;

	if (!nbytes)
		return;

	scatterwalk_start(&walk, sg);
	scatterwalk_advance(&walk, start);
	scatterwalk_copychunks(buf, &walk, nbytes, out);    ///< data copy, out means copy direction, 0:walk copy to buf, 1:buf copy to walk
	scatterwalk_done(&walk, out, 0);
}

static void na51000_crypto_complete(struct na51000_crypto_dev *dev, int err)
{
	dev->req->base.complete(&dev->req->base, err);
	dev->busy = false;
	dev->req  = NULL;
}

static bool na51000_crypto_is_sg_aligned(struct scatterlist *sg, int align_size)
{
	while (sg) {
		if (!IS_ALIGNED(sg->length, align_size) || !IS_ALIGNED(sg_phys(sg), 4))	///< DMA address must word alignment
			return false;
		sg = sg_next(sg);
	}

	return true;
}

static void na51000_crypto_timeout_handler(unsigned long data)
{
	unsigned long flags;
	struct na51000_crypto_dev *dev = (struct na51000_crypto_dev *)data;

	spin_lock_irqsave(&dev->lock, flags);

	if (dev->req) {
		dev_err(dev->dev, "crypto hardware dma timeout!\n");
		if (dev->sg_src) {
			dma_unmap_sg(dev->dev, dev->sg_src, 1, DMA_TO_DEVICE);   ///< cache nothing to do
			dev->sg_src = NULL;
		}
		if (dev->sg_dst) {
			dma_unmap_sg(dev->dev, dev->sg_dst, 1, DMA_FROM_DEVICE); ///< cache invalidate
			dev->sg_dst = NULL;
		}
		if (dev->dst_copied) {
			free_pages((unsigned long)sg_virt(&dev->sg_dst_cpy), get_order(dev->req->nbytes));
			dev->dst_copied = 0;
			if (dev->sg_same && dev->src_copied)
				dev->src_copied = 0;
		}
		if (dev->src_copied) {
			free_pages((unsigned long)sg_virt(&dev->sg_src_cpy), get_order(dev->req->nbytes));
			dev->src_copied = 0;
		}
		na51000_crypto_complete(dev, -1);
		dev->busy = true;

		/* reset device */
		na51000_crypto_reset(dev);

		/* trigger to do next crypto request in queue */
		tasklet_schedule(&dev->queue_tasklet);
	}
	else {
		dev_err(dev->dev, "crypto timer timeout!\n");
	}

	spin_unlock_irqrestore(&dev->lock, flags);

	return;
}

static void na51000_crypto_start(struct na51000_crypto_dev *dev)
{
	struct na51000_crypto_ctx    *ctx        = dev->ctx;
	struct ablkcipher_request    *req        = dev->req;
	struct na51000_crypto_reqctx *reqctx     = ablkcipher_request_ctx(req);
	u32                          *iv         = (u32 *)req->info;
	struct scatterlist           *src_sg     = req->src;
	struct scatterlist           *dst_sg     = req->dst;
	void                         *src_pages  = NULL;
	void                         *dst_pages  = NULL;
	int                          src_dma_map = 0;
	int                          dst_dma_map = 0;
	u32                          reg_value;
	u32                          dma_len;
	u32                          blocks;
	int                          err;
	unsigned long                flags;

	spin_lock_irqsave(&dev->lock, flags);

	/* source length alignment check */
	if (!na51000_crypto_is_sg_aligned(src_sg, ctx->block_size)) {
		src_pages = (void *)__get_free_pages(GFP_ATOMIC, get_order(req->nbytes));
		if (!src_pages) {
			dev_err(dev->dev, "no free memory to allocte source buffer\n");
			err = -ENOMEM;
			goto error;
		}
		na51000_crypto_sg_copy_buf(src_pages, req->src, 0, req->nbytes, 0);
		sg_init_one(&dev->sg_src_cpy, src_pages, req->nbytes);
		dev->src_copied = 1;
		dev->sg_src     = &dev->sg_src_cpy;
	}
	else {
		dev->src_copied = 0;
		dev->sg_src     = req->src;
	}

	/* destination length alignment check */
	if (!na51000_crypto_is_sg_aligned(dst_sg, ctx->block_size)) {
		if (dev->src_copied) {
			sg_init_one(&dev->sg_dst_cpy, src_pages, req->nbytes);  ///< source and destination use the same buffer to reduce memory usage
		}
		else {
			dst_pages = (void *)__get_free_pages(GFP_ATOMIC, get_order(req->nbytes));
			if (!dst_pages) {
				dev_err(dev->dev, "no free memory to allocte destination buffer\n");
				err = -ENOMEM;
				goto error;
			}
			sg_init_one(&dev->sg_dst_cpy, dst_pages, req->nbytes);
		}
		dev->dst_copied = 1;
		dev->sg_dst     = &dev->sg_dst_cpy;
	}
	else {
		dev->dst_copied = 0;
		dev->sg_dst     = req->dst;
	}

	/* source dma mapping and cache clean */
	err = dma_map_sg(dev->dev, dev->sg_src, 1, DMA_TO_DEVICE);	    ///< direction => memory to device, cache clean, DMA input
	if (!err) {
		dev_err(dev->dev, "source scatterlist dma map error\n");
		err = -ENOMEM;
		goto error;
	}
	src_dma_map = 1;

	/* destination dma mapping and cache invalidate */
	err = dma_map_sg(dev->dev, dev->sg_dst, 1, DMA_FROM_DEVICE);    ///< direction => memory from device, cache invalidate, DMA output
	if (!err) {
		dev_err(dev->dev, "destination scatterlist dma map error\n");
		err = -ENOMEM;
		goto error;
	}
	dst_dma_map = 1;

	/* store IV */
	if (iv) {
		if(reqctx->mode == NA51000_CRYPTO_MODE_DES || reqctx->mode == NA51000_CRYPTO_MODE_3DES) {
			dev->iv[0] = iv[0];
			dev->iv[1] = iv[1];
		}
		else {
			dev->iv[0] = iv[0];
			dev->iv[1] = iv[1];
			dev->iv[2] = iv[2];
			dev->iv[3] = iv[3];
		}
	}

	/* caculate dma transfer length */
	if (reqctx->opmode == NA51000_CRYPTO_OPMODE_CTR) {
		if (reqctx->mode == NA51000_CRYPTO_MODE_DES || reqctx->mode == NA51000_CRYPTO_MODE_3DES)
			blocks = (((~dev->iv[1])>>24)&0xff) + 1;
		else
			blocks = (((~dev->iv[3])>>24)&0xff) + 1;
		dma_len = min(dev->sg_src->length, (blocks*ctx->block_size));
		dma_len = min(dma_len, dev->sg_dst->length);
	}
	else {
		dma_len = min(dev->sg_src->length, dev->sg_dst->length);
	}
	if (reqctx->opmode == NA51000_CRYPTO_OPMODE_OFB) {
		/* The hardware CIV register value only work for every 128 bytes,
		 * the OFB mode trigger hardware dma length must align to 128 bytes,
		 * to get correct next IV from CIV register
		 */
		if ((dma_len > NA51000_CRYPTO_CIV_WORK_SIZE) && !IS_ALIGNED(dma_len, NA51000_CRYPTO_CIV_WORK_SIZE)) {
			if ((dma_len != dev->sg_src->length) || !sg_is_last(dev->sg_src)) {
				dma_len = ALIGN_N(dma_len, NA51000_CRYPTO_CIV_WORK_SIZE);
			}
		}
	}
	if(!dma_len || !IS_ALIGNED(dma_len, ctx->block_size)) {
		dev_err(dev->dev, "request invalid dma length(%u)!\n", dma_len);
		err = -EINVAL;
		goto error;
	}
	if (!sg_dma_address(dev->sg_src)) {
		dev_err(dev->dev, "invalid source dma address!\n");
		err = -EINVAL;
		goto error;
	}
	if (!sg_dma_address(dev->sg_dst)) {
		dev_err(dev->dev, "invalid destination dma address!\n");
		err = -EINVAL;
		goto error;
	}

	dev->sg_same    = (sg_phys(dev->sg_src) == sg_phys(dev->sg_dst)) ? 1 : 0;
	dev->sg_src_ofs = 0;
	dev->sg_src_len = dma_len;
	dev->sg_dst_ofs = 0;
	dev->sg_dst_len = dma_len;
	dev->src_total  = dma_len;
	dev->dst_total  = dma_len;
	dev->dma_total  = dma_len;

	dev_dbg(dev->dev, "Cryp=> mode:%d opmode:%d type:%d block_size:%d keylen:%d\n", reqctx->mode, reqctx->opmode, reqctx->type, ctx->block_size, ctx->keylen);
	dev_dbg(dev->dev, "Sg  => src_nents :%d dst_nents:%d req_nbytes:%d\n", sg_nents(req->src), sg_nents(req->dst), req->nbytes);
	dev_dbg(dev->dev, "Sg  => src_copied:%d dst_copied:%d same:%d\n", dev->src_copied, dev->dst_copied, dev->sg_same);
	dev_dbg(dev->dev, "Src => pa:0x%08x va:0x%08x size:%d\n", (u32)sg_dma_address(dev->sg_src), (u32)sg_virt(dev->sg_src), dev->sg_src->length);
	dev_dbg(dev->dev, "Dst => pa:0x%08x va:0x%08x size:%d\n", (u32)sg_dma_address(dev->sg_dst), (u32)sg_virt(dev->sg_dst), dev->sg_dst->length);

	switch (reqctx->mode) {
	case NA51000_CRYPTO_MODE_DES:		/* key => 64 bits, IV => 64 bits, data => 64 bits */
		/* set IV */
		if (iv) {
			na51000_crypto_write(dev, NA51000_CRYPTO_IV0_REG, dev->iv[0]);
			na51000_crypto_write(dev, NA51000_CRYPTO_IV1_REG, dev->iv[1]);
			dev_dbg(dev->dev, "IV  => %08x %08x\n", iv[0], iv[1]);
		}

		/* set key */
		na51000_crypto_write(dev, NA51000_CRYPTO_KEY0_REG, BSWAP(ctx->key[1]));
		na51000_crypto_write(dev, NA51000_CRYPTO_KEY1_REG, BSWAP(ctx->key[0]));
		dev_dbg(dev->dev, "Key => %08x %08x\n", ctx->key[0], ctx->key[1]);
		break;
	case NA51000_CRYPTO_MODE_3DES:		/* key => 192 bit, IV => 64 bits, data => 64 bits, DES-EDE3 */
		/* set IV */
		if (iv) {
			na51000_crypto_write(dev, NA51000_CRYPTO_IV0_REG, dev->iv[0]);
			na51000_crypto_write(dev, NA51000_CRYPTO_IV1_REG, dev->iv[1]);
			dev_dbg(dev->dev, "IV  => %08x %08x\n", iv[0], iv[1]);
		}

		/* set key */
		na51000_crypto_write(dev, NA51000_CRYPTO_KEY0_REG, BSWAP(ctx->key[1]));	///< K1
		na51000_crypto_write(dev, NA51000_CRYPTO_KEY1_REG, BSWAP(ctx->key[0]));
		na51000_crypto_write(dev, NA51000_CRYPTO_KEY2_REG, BSWAP(ctx->key[3]));	///< K2
		na51000_crypto_write(dev, NA51000_CRYPTO_KEY3_REG, BSWAP(ctx->key[2]));
		na51000_crypto_write(dev, NA51000_CRYPTO_KEY4_REG, BSWAP(ctx->key[5]));	///< K3
		na51000_crypto_write(dev, NA51000_CRYPTO_KEY5_REG, BSWAP(ctx->key[4]));
		dev_dbg(dev->dev, "Key => %08x %08x %08x %08x %08x %08x\n", ctx->key[0], ctx->key[1], ctx->key[2], ctx->key[3], ctx->key[4], ctx->key[5]);
		break;
	case NA51000_CRYPTO_MODE_AES_128:	/* key => 128 bits, IV => 128 bits, data => 128 bits*/
		/* set IV */
		if (iv) {
			na51000_crypto_write(dev, NA51000_CRYPTO_IV0_REG, dev->iv[0]);
			na51000_crypto_write(dev, NA51000_CRYPTO_IV1_REG, dev->iv[1]);
			na51000_crypto_write(dev, NA51000_CRYPTO_IV2_REG, dev->iv[2]);
			na51000_crypto_write(dev, NA51000_CRYPTO_IV3_REG, dev->iv[3]);
			dev_dbg(dev->dev, "IV  => %08x %08x %08x %08x\n", iv[0], iv[1], iv[2], iv[3]);
		}

		/* set key */
		na51000_crypto_write(dev, NA51000_CRYPTO_KEY0_REG, BSWAP(ctx->key[3]));
		na51000_crypto_write(dev, NA51000_CRYPTO_KEY1_REG, BSWAP(ctx->key[2]));
		na51000_crypto_write(dev, NA51000_CRYPTO_KEY2_REG, BSWAP(ctx->key[1]));
		na51000_crypto_write(dev, NA51000_CRYPTO_KEY3_REG, BSWAP(ctx->key[0]));
		dev_dbg(dev->dev, "Key => %08x %08x %08x %08x\n", ctx->key[0], ctx->key[1], ctx->key[2], ctx->key[3]);
		break;
	case NA51000_CRYPTO_MODE_AES_256:	/* key => 256 bits, IV => 128 bits, data => 128 bits*/
		/* set IV */
		if (iv) {
			na51000_crypto_write(dev, NA51000_CRYPTO_IV0_REG, dev->iv[0]);
			na51000_crypto_write(dev, NA51000_CRYPTO_IV1_REG, dev->iv[1]);
			na51000_crypto_write(dev, NA51000_CRYPTO_IV2_REG, dev->iv[2]);
			na51000_crypto_write(dev, NA51000_CRYPTO_IV3_REG, dev->iv[3]);
			dev_dbg(dev->dev, "IV  => %08x %08x %08x %08x\n", iv[0], iv[1], iv[2], iv[3]);
		}

		/* set key */
		na51000_crypto_write(dev, NA51000_CRYPTO_KEY0_REG, BSWAP(ctx->key[7]));
		na51000_crypto_write(dev, NA51000_CRYPTO_KEY1_REG, BSWAP(ctx->key[6]));
		na51000_crypto_write(dev, NA51000_CRYPTO_KEY2_REG, BSWAP(ctx->key[5]));
		na51000_crypto_write(dev, NA51000_CRYPTO_KEY3_REG, BSWAP(ctx->key[4]));
		na51000_crypto_write(dev, NA51000_CRYPTO_KEY4_REG, BSWAP(ctx->key[3]));
		na51000_crypto_write(dev, NA51000_CRYPTO_KEY5_REG, BSWAP(ctx->key[2]));
		na51000_crypto_write(dev, NA51000_CRYPTO_KEY6_REG, BSWAP(ctx->key[1]));
		na51000_crypto_write(dev, NA51000_CRYPTO_KEY7_REG, BSWAP(ctx->key[0]));
		dev_dbg(dev->dev, "Key => %08x %08x %08x %08x %08x %08x %08x %08x\n", ctx->key[0], ctx->key[1], ctx->key[2], ctx->key[3], ctx->key[4], ctx->key[5], ctx->key[6], ctx->key[7]);
		break;
	default:
		dev_err(dev->dev, "unknown crypto mode(%d)!\n", reqctx->mode);
		err = -EINVAL;
		goto error;
	}

	/* get next IV before trigger hardware to decrypt on CBC and CFB mode */
	if ((reqctx->type == NA51000_CRYPTO_TYPE_DECRYPT) &&
		(reqctx->opmode == NA51000_CRYPTO_OPMODE_CBC || reqctx->opmode == NA51000_CRYPTO_OPMODE_CFB)) {
		sg_pcopy_to_buffer(dev->sg_src, 1, dev->iv, ctx->block_size, (dev->sg_src_ofs + dev->sg_src_len - ctx->block_size));
	}

	/* set counter */
	if (reqctx->opmode == NA51000_CRYPTO_OPMODE_CTR) {
		if (reqctx->mode == NA51000_CRYPTO_MODE_DES || reqctx->mode == NA51000_CRYPTO_MODE_3DES) {
			na51000_crypto_write(dev, NA51000_CRYPTO_CNT0_REG, 0x00000000);
			na51000_crypto_write(dev, NA51000_CRYPTO_CNT1_REG, 0x01000000);
			na51000_crypto_write(dev, NA51000_CRYPTO_CNT2_REG, 0x00000000);
			na51000_crypto_write(dev, NA51000_CRYPTO_CNT3_REG, 0x00000000);
		}
		else {
			na51000_crypto_write(dev, NA51000_CRYPTO_CNT0_REG, 0x00000000);
			na51000_crypto_write(dev, NA51000_CRYPTO_CNT1_REG, 0x00000000);
			na51000_crypto_write(dev, NA51000_CRYPTO_CNT2_REG, 0x00000000);
			na51000_crypto_write(dev, NA51000_CRYPTO_CNT3_REG, 0x01000000);
		}
	}

	/* set mode */
	reg_value = (reqctx->mode<<1)        | \
				(reqctx->opmode<<4)      | \
				(reqctx->type<<8)        | \
				(reqctx->byte_endian<<9) | \
				(reqctx->bit_order<<10)  | \
				(reqctx->accmode<<11);
	na51000_crypto_write(dev, NA51000_CRYPTO_CTRL_REG, reg_value);

	/* enable DMA interrupt mask */
	na51000_crypto_write(dev, NA51000_CRYPTO_DMA_CFG_REG, 0x1);

	/* set DMA address and length */
	na51000_crypto_write(dev, NA51000_CRYPTO_DMA_SRC_REG,     ALIGN_N(sg_dma_address(dev->sg_src), 4));
	na51000_crypto_write(dev, NA51000_CRYPTO_DMA_DST_REG,     ALIGN_N(sg_dma_address(dev->sg_dst), 4));
	na51000_crypto_write(dev, NA51000_CRYPTO_DMA_TX_SIZE_REG, dma_len);

	/* clear DMA interrupt status */
	na51000_crypto_write(dev, NA51000_CRYPTO_DMA_STS_REG, 0x1);

	/* start DMA transfer */
	na51000_crypto_write(dev, NA51000_CRYPTO_DMA_CTRL_REG, 0x1);

	/* start timeout timer */
	mod_timer(&dev->timer, jiffies+msecs_to_jiffies(NA5100_CRYPTO_DEFAULT_TIMEOUT));

	spin_unlock_irqrestore(&dev->lock, flags);

	return;

error:
	if (src_dma_map) {
		dma_unmap_sg(dev->dev, dev->sg_src, 1, DMA_TO_DEVICE);   ///< cache nothing to do
		dev->sg_src = NULL;
	}
	if (dst_dma_map) {
		dma_unmap_sg(dev->dev, dev->sg_dst, 1, DMA_FROM_DEVICE); ///< cache invalidate
		dev->sg_dst = NULL;
	}
	if (src_pages)
		free_pages((unsigned long)src_pages, get_order(req->nbytes));
	if (dst_pages)
		free_pages((unsigned long)dst_pages, get_order(req->nbytes));

	na51000_crypto_complete(dev, err);

	spin_unlock_irqrestore(&dev->lock, flags);
}

static void na51000_crypto_queue_tasklet(unsigned long data)
{
	struct na51000_crypto_dev   *dev = (struct na51000_crypto_dev *)data;
	struct crypto_async_request *async_req, *backlog;
	unsigned long flags;

	spin_lock_irqsave(&dev->lock, flags);
	backlog   = crypto_get_backlog(&dev->queue);
	async_req = crypto_dequeue_request(&dev->queue);

	if (!async_req) {
		dev->busy = false;
		spin_unlock_irqrestore(&dev->lock, flags);
		return;
	}

	if (backlog) {
		backlog->complete(backlog, -EINPROGRESS);
	}

	/* assign new request to device */
	dev->req = ablkcipher_request_cast(async_req);
	dev->ctx = crypto_ablkcipher_ctx(crypto_ablkcipher_reqtfm(dev->req));

	spin_unlock_irqrestore(&dev->lock, flags);

	na51000_crypto_start(dev);
}

static void na51000_crypto_done_tasklet(unsigned long data)
{
	struct na51000_crypto_dev    *dev = (struct na51000_crypto_dev *)data;
	struct na51000_crypto_ctx    *ctx;
	struct ablkcipher_request    *req;
	struct na51000_crypto_reqctx *reqctx;
	unsigned long flags;
	u32 blocks;
	u32 dma_len;
	int ret, update_iv = 0;

	spin_lock_irqsave(&dev->lock, flags);

	/* check request */
	if (!dev->req) {
		dev_err(dev->dev, "crypto request is empty!!\n");
		goto exit;
	}
	ctx    = dev->ctx;
	req    = dev->req;
	reqctx = ablkcipher_request_ctx(req);

	dev_dbg(dev->dev, "CIV => %08x %08x %08x %08x\n",
			na51000_crypto_read(dev, NA51000_CRYPTO_CURR_IV0_REG),
			na51000_crypto_read(dev, NA51000_CRYPTO_CURR_IV1_REG),
			na51000_crypto_read(dev, NA51000_CRYPTO_CURR_IV2_REG),
			na51000_crypto_read(dev, NA51000_CRYPTO_CURR_IV3_REG));

	dev_dbg(dev->dev, "TX  => size:%d count:%d\n",
			na51000_crypto_read(dev, NA51000_CRYPTO_DMA_TX_SIZE_REG),
			na51000_crypto_read(dev, NA51000_CRYPTO_DMA_TX_CNT_REG));

	/* check source sg */
	if (dev->sg_src) {
		if (dev->src_total >= req->nbytes) {
			dma_unmap_sg(dev->dev, dev->sg_src, 1,  DMA_TO_DEVICE);  ///< cache nothing to do
			dev->sg_src     = NULL;
			dev->sg_src_ofs = 0;
			dev->sg_src_len = 0;
		}
		else if ((dev->sg_src_ofs + dev->sg_src_len) < dev->sg_src->length) {
			if (reqctx->opmode == NA51000_CRYPTO_OPMODE_CTR)
				update_iv = na51000_crypto_update_ctr_iv(dev);
			else if (reqctx->opmode == NA51000_CRYPTO_OPMODE_OFB)
				update_iv = na51000_crypto_update_ofb_iv(dev);
			else if ((reqctx->opmode == NA51000_CRYPTO_OPMODE_CBC || reqctx->opmode == NA51000_CRYPTO_OPMODE_CFB) &&
					 (reqctx->type == NA51000_CRYPTO_TYPE_DECRYPT)) {
				update_iv = 1;  ///< IV have updated at previous hardware trigger
			}
			dev->sg_src_ofs = dev->sg_src_ofs + dev->sg_src_len;
			dev->sg_src_len = dev->sg_src->length - dev->sg_src_ofs;
			if ((dev->src_total + dev->sg_src_len) > req->nbytes)
				dev->sg_src_len = req->nbytes - dev->src_total;
			if (reqctx->opmode == NA51000_CRYPTO_OPMODE_CTR) {
				if (reqctx->mode == NA51000_CRYPTO_MODE_DES || reqctx->mode == NA51000_CRYPTO_MODE_3DES)
					blocks = (((~dev->iv[1])>>24)&0xff) + 1;
				else
					blocks = (((~dev->iv[3])>>24)&0xff) + 1;
				dev->sg_src_len = min(dev->sg_src_len, blocks*ctx->block_size);
			}
		}
		else {
			dma_unmap_sg(dev->dev, dev->sg_src, 1, DMA_TO_DEVICE);  ///< cache nothing to do
			if (sg_is_last(dev->sg_src)) {
				dev->sg_src     = NULL;
				dev->sg_src_ofs = 0;
				dev->sg_src_len = 0;
			}
			else {
				ret = dma_map_sg(dev->dev, sg_next(dev->sg_src), 1, DMA_TO_DEVICE);  ///< cache clean
				if (!ret) {
					dev->sg_src     = NULL;
					dev->sg_src_ofs = 0;
					dev->sg_src_len = 0;
				}
				else {
					if (reqctx->opmode == NA51000_CRYPTO_OPMODE_CTR)
						update_iv = na51000_crypto_update_ctr_iv(dev);
					else if (reqctx->opmode == NA51000_CRYPTO_OPMODE_OFB)
						update_iv = na51000_crypto_update_ofb_iv(dev);
					else if ((reqctx->opmode == NA51000_CRYPTO_OPMODE_CBC || reqctx->opmode == NA51000_CRYPTO_OPMODE_CFB) &&
							 (reqctx->type == NA51000_CRYPTO_TYPE_DECRYPT)) {
						update_iv = 1;  ///< IV have updated at previous hardware trigger
					}
					dev->sg_src     = sg_next(dev->sg_src);
					dev->sg_src_ofs = 0;
					dev->sg_src_len = dev->sg_src->length;
					if ((dev->src_total + dev->sg_src_len) > req->nbytes)
						dev->sg_src_len = req->nbytes - dev->src_total;
					if (reqctx->opmode == NA51000_CRYPTO_OPMODE_CTR) {
						if (reqctx->mode == NA51000_CRYPTO_MODE_DES || reqctx->mode == NA51000_CRYPTO_MODE_3DES)
							blocks = (((~dev->iv[1])>>24)&0xff) + 1;
						else
							blocks = (((~dev->iv[3])>>24)&0xff) + 1;
						dev->sg_src_len = min(dev->sg_src_len, blocks*ctx->block_size);
					}
				}
			}
		}
	}

	/* check destination sg */
	if (dev->sg_dst) {
		if (dev->dst_total >= req->nbytes) {
			dma_unmap_sg(dev->dev, dev->sg_dst, 1, DMA_FROM_DEVICE);  ///< do cache invalidate
			dev->sg_dst     = NULL;
			dev->sg_dst_ofs = 0;
			dev->sg_dst_len = 0;
		}
		else if ((dev->sg_dst_ofs + dev->sg_dst_len) < dev->sg_dst->length) {
			if ((reqctx->opmode == NA51000_CRYPTO_OPMODE_CBC || reqctx->opmode == NA51000_CRYPTO_OPMODE_CFB) &&
				(reqctx->type == NA51000_CRYPTO_TYPE_ENCRYPT)) {
				sg_pcopy_to_buffer(dev->sg_dst, 1, dev->iv, ctx->block_size, (dev->sg_dst_ofs + dev->sg_dst_len - ctx->block_size));
				update_iv = 1;
			}
			dev->sg_dst_ofs = dev->sg_dst_ofs + dev->sg_dst_len;
			dev->sg_dst_len = dev->sg_dst->length - dev->sg_dst_ofs;
			if ((dev->dst_total + dev->sg_dst_len) > req->nbytes)
				dev->sg_dst_len = req->nbytes - dev->dst_total;
		}
		else {
			dma_unmap_sg(dev->dev, dev->sg_dst, 1, DMA_FROM_DEVICE);  ///< cache invalidate
			if (sg_is_last(dev->sg_dst)) {
				dev->sg_dst     = NULL;
				dev->sg_dst_ofs = 0;
				dev->sg_dst_len = 0;
			}
			else {
				ret = dma_map_sg(dev->dev, sg_next(dev->sg_dst), 1, DMA_FROM_DEVICE);  ///< cache invalidate
				if(!ret) {
					dev->sg_dst     = NULL;
					dev->sg_dst_ofs = 0;
					dev->sg_dst_len = 0;
				}
				else {
					if ((reqctx->opmode == NA51000_CRYPTO_OPMODE_CBC || reqctx->opmode == NA51000_CRYPTO_OPMODE_CFB) &&
						(reqctx->type == NA51000_CRYPTO_TYPE_ENCRYPT)) {
						sg_pcopy_to_buffer(dev->sg_dst, 1, dev->iv, ctx->block_size, (dev->sg_dst_ofs + dev->sg_dst_len - ctx->block_size));
						update_iv = 1;
					}
					dev->sg_dst     = sg_next(dev->sg_dst);
					dev->sg_dst_ofs = 0;
					dev->sg_dst_len = dev->sg_dst->length;
					if ((dev->dst_total + dev->sg_dst_len) > req->nbytes)
						dev->sg_dst_len = req->nbytes - dev->dst_total;
				}
			}
		}
	}

	/* check request complete */
	if (dev->sg_src && dev->sg_dst) {
		dma_len = min(dev->sg_src_len, dev->sg_dst_len);
		if (reqctx->opmode == NA51000_CRYPTO_OPMODE_OFB) {
			if ((dma_len > NA51000_CRYPTO_CIV_WORK_SIZE) && !IS_ALIGNED(dma_len, NA51000_CRYPTO_CIV_WORK_SIZE)) {
				if (((dev->sg_src_ofs+dma_len) != dev->sg_src->length) || !sg_is_last(dev->sg_src)) {
					dma_len = ALIGN_N(dma_len, NA51000_CRYPTO_CIV_WORK_SIZE);
				}
			}
		}
		if (!dma_len || !IS_ALIGNED(dma_len, ctx->block_size)) {
			dev_err(dev->dev, "issue invalid dma length(%u)!\n", dma_len);
			goto completed;
		}
		if (!sg_dma_address(dev->sg_src)) {
			dev_err(dev->dev, "issue invalid source dma address!\n");
			goto completed;
		}
		if (!sg_dma_address(dev->sg_dst)) {
			dev_err(dev->dev, "issue invalid destination dma address!\n");
			goto completed;
		}

		dev->sg_src_len = dma_len;
		dev->sg_dst_len = dma_len;
		dev->src_total += dma_len;
		dev->dst_total += dma_len;
		dev->dma_total += dma_len;

		/* Update IV */
		if (update_iv) {
			switch (reqctx->mode) {
			case NA51000_CRYPTO_MODE_DES:
			case NA51000_CRYPTO_MODE_3DES:
				na51000_crypto_write(dev, NA51000_CRYPTO_IV0_REG, dev->iv[0]);
				na51000_crypto_write(dev, NA51000_CRYPTO_IV1_REG, dev->iv[1]);
				dev_dbg(dev->dev, "NIV => %08x %08x\n", dev->iv[0], dev->iv[1]);
				break;
			case NA51000_CRYPTO_MODE_AES_128:
			case NA51000_CRYPTO_MODE_AES_256:
				na51000_crypto_write(dev, NA51000_CRYPTO_IV0_REG, dev->iv[0]);
				na51000_crypto_write(dev, NA51000_CRYPTO_IV1_REG, dev->iv[1]);
				na51000_crypto_write(dev, NA51000_CRYPTO_IV2_REG, dev->iv[2]);
				na51000_crypto_write(dev, NA51000_CRYPTO_IV3_REG, dev->iv[3]);
				dev_dbg(dev->dev, "NIV => %08x %08x %08x %08x\n", dev->iv[0], dev->iv[1], dev->iv[2], dev->iv[3]);
				break;
			default:
				break;
			}
		}

		/* get next IV before trigger hardware to decrypt on CBC and CFB mode */
		if ((reqctx->type == NA51000_CRYPTO_TYPE_DECRYPT) &&
			(reqctx->opmode == NA51000_CRYPTO_OPMODE_CBC || reqctx->opmode == NA51000_CRYPTO_OPMODE_CFB)) {
			sg_pcopy_to_buffer(dev->sg_src, 1, dev->iv, ctx->block_size, (dev->sg_src_ofs + dev->sg_src_len - ctx->block_size));
		}

		/* update next buffer DMA address and length */
		na51000_crypto_write(dev, NA51000_CRYPTO_DMA_SRC_REG,     ALIGN_N((sg_dma_address(dev->sg_src)+dev->sg_src_ofs), 4));
		na51000_crypto_write(dev, NA51000_CRYPTO_DMA_DST_REG,     ALIGN_N((sg_dma_address(dev->sg_dst)+dev->sg_dst_ofs), 4));
		na51000_crypto_write(dev, NA51000_CRYPTO_DMA_TX_SIZE_REG, dma_len);

		/* clear DMA interrupt status */
		na51000_crypto_write(dev, NA51000_CRYPTO_DMA_STS_REG, 0x1);

		/* trigger to do next buffer */
		na51000_crypto_write(dev, NA51000_CRYPTO_DMA_CTRL_REG, 0x1);

		/* start timeout timer */
		mod_timer(&dev->timer, jiffies+msecs_to_jiffies(NA5100_CRYPTO_DEFAULT_TIMEOUT));

		dev_dbg(dev->dev, "trigger next DMA transfer!\n");
	}
	else {
completed:
		if (dev->sg_src) {
			dma_unmap_sg(dev->dev, dev->sg_src, 1, DMA_TO_DEVICE);    ///< cache nothing to do
			dev->sg_src = NULL;
		}
		if (dev->sg_dst) {
			dma_unmap_sg(dev->dev, dev->sg_dst, 1, DMA_FROM_DEVICE);  ///< cache invalidate
			dev->sg_dst = NULL;
		}
		if (dev->dst_copied) {
			dev_dbg(dev->dev, "copy %d bytes back to original buffer\n", dev->dst_total);
			na51000_crypto_sg_copy_buf(sg_virt(&dev->sg_dst_cpy), req->dst, 0, dev->dst_total, 1);
			free_pages((unsigned long)sg_virt(&dev->sg_dst_cpy), get_order(req->nbytes));
			dev->dst_copied = 0;
			if (dev->sg_same && dev->src_copied)
				dev->src_copied = 0;
		}
		if (dev->src_copied) {
			free_pages((unsigned long)sg_virt(&dev->sg_src_cpy), get_order(req->nbytes));
			dev->src_copied = 0;
		}
		na51000_crypto_complete(dev, ((dev->dst_total == req->nbytes) ? 0 : -1));
		dev->busy = true;
		tasklet_schedule(&dev->queue_tasklet);  ///< trigger to do next crypto request in queue
	}

exit:
	spin_unlock_irqrestore(&dev->lock, flags);
}

static int na51000_crypto_cra_init(struct crypto_tfm *tfm)
{
	struct na51000_crypto_ctx *ctx = crypto_tfm_ctx(tfm);
	struct crypto_alg         *alg = tfm->__crt_alg;

	if (alg->cra_flags & CRYPTO_ALG_NEED_FALLBACK) {
		ctx->fallback = crypto_alloc_ablkcipher(crypto_tfm_alg_name(tfm), CRYPTO_ALG_TYPE_ABLKCIPHER, (CRYPTO_ALG_ASYNC | CRYPTO_ALG_NEED_FALLBACK));
		if (IS_ERR(ctx->fallback)) {
			pr_err( "failed to allocate fallback for %s\n", alg->cra_name);
			ctx->fallback = NULL;
		}
	}
	tfm->crt_ablkcipher.reqsize = sizeof(struct na51000_crypto_reqctx);

	return 0;
}

static void na51000_crypto_cra_exit(struct crypto_tfm *tfm)
{
	struct na51000_crypto_ctx *ctx = crypto_tfm_ctx(tfm);

	if (ctx->fallback) {
		crypto_free_ablkcipher(ctx->fallback);
		ctx->fallback = NULL;
	}
	return;
}

static int na51000_crypto_aes_do_fallback(struct ablkcipher_request *req, bool is_encrypt)
{
	struct crypto_tfm         *tfm = crypto_ablkcipher_tfm(crypto_ablkcipher_reqtfm(req));
	struct na51000_crypto_ctx *ctx = crypto_tfm_ctx(tfm);
	int err;

	if (!ctx->fallback)
		return -EINVAL;

	/*
	 * Change the request to use the software fallback transform, and once
	 * the ciphering has completed, put the old transform back into the
	 * request.
	 */
	ablkcipher_request_set_tfm(req, ctx->fallback);
	err = is_encrypt ? crypto_ablkcipher_encrypt(req) : crypto_ablkcipher_decrypt(req);
	ablkcipher_request_set_tfm(req, __crypto_ablkcipher_cast(tfm));

	return err;
}

static int na51000_crypto_aes_setkey(struct crypto_ablkcipher *tfm, const u8 *key, unsigned int keylen)
{
	int err;
	struct na51000_crypto_ctx *ctx = crypto_ablkcipher_ctx(tfm);

	if (unlikely(keylen != AES_KEYSIZE_128 && keylen != AES_KEYSIZE_256)) {
		/* The requested key size is not supported by HW, do a fallback */
		if (ctx->fallback) {
			crypto_ablkcipher_clear_flags(ctx->fallback, CRYPTO_TFM_REQ_MASK);
			crypto_ablkcipher_set_flags(ctx->fallback, crypto_ablkcipher_get_flags(tfm)&CRYPTO_TFM_REQ_MASK);
			err = crypto_ablkcipher_setkey(ctx->fallback, key, keylen);
			if (err) {
				crypto_ablkcipher_clear_flags(tfm, CRYPTO_TFM_REQ_MASK);
				crypto_ablkcipher_set_flags(tfm, crypto_ablkcipher_get_flags(ctx->fallback)&CRYPTO_TFM_REQ_MASK);
				return err;
			}
		}
		else {
			crypto_ablkcipher_set_flags(tfm, CRYPTO_TFM_RES_BAD_KEY_LEN);
			return -EINVAL;
		}
	}

	memcpy(ctx->key, key, keylen);
	ctx->keylen = keylen;

	return 0;
}

static int na51000_crypto_aes_ecb_encrypt(struct ablkcipher_request *req)
{
	struct crypto_ablkcipher     *tfm    = crypto_ablkcipher_reqtfm(req);
	struct na51000_crypto_ctx    *ctx    = crypto_ablkcipher_ctx(tfm);
	struct na51000_crypto_reqctx *reqctx = ablkcipher_request_ctx(req);

	if (!IS_ALIGNED(req->nbytes, AES_BLOCK_SIZE)) {
		pr_err("request size is not exact amount of AES blocks(nbytes:%d)\n", req->nbytes);
		return -EINVAL;
	}

	if (unlikely(ctx->keylen != AES_KEYSIZE_128 && ctx->keylen != AES_KEYSIZE_256)) {
		return na51000_crypto_aes_do_fallback(req, 1);
	}

	/* setup device */
	ctx->dev        = na51000_cdev;
	ctx->block_size = AES_BLOCK_SIZE;

	/* setup operation */
	reqctx->type        = NA51000_CRYPTO_TYPE_ENCRYPT;
	reqctx->mode        = (ctx->keylen == AES_KEYSIZE_256) ? NA51000_CRYPTO_MODE_AES_256 : NA51000_CRYPTO_MODE_AES_128;
	reqctx->opmode      = NA51000_CRYPTO_OPMODE_ECB;
	reqctx->accmode     = NA51000_CRYPTO_ACCMODE_DMA;
	reqctx->byte_endian = NA51000_CRYPTO_BYTE_ENDIAN_BIG;
	reqctx->bit_order   = NA51000_CRYPTO_BIT_ORDER_0_7;

	return na51000_crypto_handle_req(ctx->dev, req);
}

static int na51000_crypto_aes_ecb_decrypt(struct ablkcipher_request *req)
{
	struct crypto_ablkcipher     *tfm    = crypto_ablkcipher_reqtfm(req);
	struct na51000_crypto_ctx    *ctx    = crypto_ablkcipher_ctx(tfm);
	struct na51000_crypto_reqctx *reqctx = ablkcipher_request_ctx(req);

	if (!IS_ALIGNED(req->nbytes, AES_BLOCK_SIZE)) {
		pr_err("request size is not exact amount of AES blocks(nbytes:%d)\n", req->nbytes);
		return -EINVAL;
	}

	if (unlikely(ctx->keylen != AES_KEYSIZE_128 && ctx->keylen != AES_KEYSIZE_256)) {
		return na51000_crypto_aes_do_fallback(req, 0);
	}

	/* setup device */
	ctx->dev        = na51000_cdev;
	ctx->block_size = AES_BLOCK_SIZE;

	/* setup operation */
	reqctx->type        = NA51000_CRYPTO_TYPE_DECRYPT;
	reqctx->mode        = (ctx->keylen == AES_KEYSIZE_256) ? NA51000_CRYPTO_MODE_AES_256 : NA51000_CRYPTO_MODE_AES_128;
	reqctx->opmode      = NA51000_CRYPTO_OPMODE_ECB;
	reqctx->accmode     = NA51000_CRYPTO_ACCMODE_DMA;
	reqctx->byte_endian = NA51000_CRYPTO_BYTE_ENDIAN_BIG;
	reqctx->bit_order   = NA51000_CRYPTO_BIT_ORDER_0_7;

	return na51000_crypto_handle_req(ctx->dev, req);
}

static int na51000_crypto_aes_cbc_encrypt(struct ablkcipher_request *req)
{
	struct crypto_ablkcipher     *tfm    = crypto_ablkcipher_reqtfm(req);
	struct na51000_crypto_ctx    *ctx    = crypto_ablkcipher_ctx(tfm);
	struct na51000_crypto_reqctx *reqctx = ablkcipher_request_ctx(req);

	if (!IS_ALIGNED(req->nbytes, AES_BLOCK_SIZE)) {
		pr_err("request size is not exact amount of AES blocks(nbytes:%d)\n", req->nbytes);
		return -EINVAL;
	}

	if (unlikely(ctx->keylen != AES_KEYSIZE_128 && ctx->keylen != AES_KEYSIZE_256)) {
		return na51000_crypto_aes_do_fallback(req, 1);
	}

	/* setup device */
	ctx->dev        = na51000_cdev;
	ctx->block_size = AES_BLOCK_SIZE;

	/* setup operation */
	reqctx->type        = NA51000_CRYPTO_TYPE_ENCRYPT;
	reqctx->mode        = (ctx->keylen == AES_KEYSIZE_256) ? NA51000_CRYPTO_MODE_AES_256 : NA51000_CRYPTO_MODE_AES_128;
	reqctx->opmode      = NA51000_CRYPTO_OPMODE_CBC;
	reqctx->accmode     = NA51000_CRYPTO_ACCMODE_DMA;
	reqctx->byte_endian = NA51000_CRYPTO_BYTE_ENDIAN_BIG;
	reqctx->bit_order   = NA51000_CRYPTO_BIT_ORDER_0_7;

	return na51000_crypto_handle_req(ctx->dev, req);
}

static int na51000_crypto_aes_cbc_decrypt(struct ablkcipher_request *req)
{
	struct crypto_ablkcipher     *tfm    = crypto_ablkcipher_reqtfm(req);
	struct na51000_crypto_ctx    *ctx    = crypto_ablkcipher_ctx(tfm);
	struct na51000_crypto_reqctx *reqctx = ablkcipher_request_ctx(req);

	if (!IS_ALIGNED(req->nbytes, AES_BLOCK_SIZE)) {
		pr_err("request size is not exact amount of AES blocks(nbytes:%d)\n", req->nbytes);
		return -EINVAL;
	}

	if (unlikely(ctx->keylen != AES_KEYSIZE_128 && ctx->keylen != AES_KEYSIZE_256)) {
		return na51000_crypto_aes_do_fallback(req, 0);
	}

	/* setup device */
	ctx->dev        = na51000_cdev;
	ctx->block_size = AES_BLOCK_SIZE;

	/* setup operation */
	reqctx->type        = NA51000_CRYPTO_TYPE_DECRYPT;
	reqctx->mode        = (ctx->keylen == AES_KEYSIZE_256) ? NA51000_CRYPTO_MODE_AES_256 : NA51000_CRYPTO_MODE_AES_128;
	reqctx->opmode      = NA51000_CRYPTO_OPMODE_CBC;
	reqctx->accmode     = NA51000_CRYPTO_ACCMODE_DMA;
	reqctx->byte_endian = NA51000_CRYPTO_BYTE_ENDIAN_BIG;
	reqctx->bit_order   = NA51000_CRYPTO_BIT_ORDER_0_7;

	return na51000_crypto_handle_req(ctx->dev, req);
}

static int na51000_crypto_aes_cfb_encrypt(struct ablkcipher_request *req)
{
	struct crypto_ablkcipher     *tfm    = crypto_ablkcipher_reqtfm(req);
	struct na51000_crypto_ctx    *ctx    = crypto_ablkcipher_ctx(tfm);
	struct na51000_crypto_reqctx *reqctx = ablkcipher_request_ctx(req);

	if (!IS_ALIGNED(req->nbytes, AES_BLOCK_SIZE)) {
		pr_err("request size is not exact amount of AES blocks(nbytes:%d)\n", req->nbytes);
		return -EINVAL;
	}

	if (unlikely(ctx->keylen != AES_KEYSIZE_128 && ctx->keylen != AES_KEYSIZE_256)) {
		return na51000_crypto_aes_do_fallback(req, 1);
	}

	/* setup device */
	ctx->dev        = na51000_cdev;
	ctx->block_size = AES_BLOCK_SIZE;

	/* setup operation */
	reqctx->type        = NA51000_CRYPTO_TYPE_ENCRYPT;
	reqctx->mode        = (ctx->keylen == AES_KEYSIZE_256) ? NA51000_CRYPTO_MODE_AES_256 : NA51000_CRYPTO_MODE_AES_128;
	reqctx->opmode      = NA51000_CRYPTO_OPMODE_CFB;
	reqctx->accmode     = NA51000_CRYPTO_ACCMODE_DMA;
	reqctx->byte_endian = NA51000_CRYPTO_BYTE_ENDIAN_BIG;
	reqctx->bit_order   = NA51000_CRYPTO_BIT_ORDER_0_7;

	return na51000_crypto_handle_req(ctx->dev, req);
}

static int na51000_crypto_aes_cfb_decrypt(struct ablkcipher_request *req)
{
	struct crypto_ablkcipher     *tfm    = crypto_ablkcipher_reqtfm(req);
	struct na51000_crypto_ctx    *ctx    = crypto_ablkcipher_ctx(tfm);
	struct na51000_crypto_reqctx *reqctx = ablkcipher_request_ctx(req);

	if (!IS_ALIGNED(req->nbytes, AES_BLOCK_SIZE)) {
		pr_err("request size is not exact amount of AES blocks(nbytes:%d)\n", req->nbytes);
		return -EINVAL;
	}

	if (unlikely(ctx->keylen != AES_KEYSIZE_128 && ctx->keylen != AES_KEYSIZE_256)) {
		return na51000_crypto_aes_do_fallback(req, 0);
	}

	/* setup device */
	ctx->dev        = na51000_cdev;
	ctx->block_size = AES_BLOCK_SIZE;

	/* setup operation */
	reqctx->type        = NA51000_CRYPTO_TYPE_DECRYPT;
	reqctx->mode        = (ctx->keylen == AES_KEYSIZE_256) ? NA51000_CRYPTO_MODE_AES_256 : NA51000_CRYPTO_MODE_AES_128;
	reqctx->opmode      = NA51000_CRYPTO_OPMODE_CFB;
	reqctx->accmode     = NA51000_CRYPTO_ACCMODE_DMA;
	reqctx->byte_endian = NA51000_CRYPTO_BYTE_ENDIAN_BIG;
	reqctx->bit_order   = NA51000_CRYPTO_BIT_ORDER_0_7;

	return na51000_crypto_handle_req(ctx->dev, req);
}

static int na51000_crypto_aes_ofb_encrypt(struct ablkcipher_request *req)
{
	struct crypto_ablkcipher     *tfm    = crypto_ablkcipher_reqtfm(req);
	struct na51000_crypto_ctx    *ctx    = crypto_ablkcipher_ctx(tfm);
	struct na51000_crypto_reqctx *reqctx = ablkcipher_request_ctx(req);

	if (!IS_ALIGNED(req->nbytes, AES_BLOCK_SIZE)) {
		pr_err("request size is not exact amount of AES blocks(nbytes:%d)\n", req->nbytes);
		return -EINVAL;
	}

	if (unlikely(ctx->keylen != AES_KEYSIZE_128 && ctx->keylen != AES_KEYSIZE_256)) {
		return na51000_crypto_aes_do_fallback(req, 1);
	}

	/* setup device */
	ctx->dev        = na51000_cdev;
	ctx->block_size = AES_BLOCK_SIZE;

	/* setup operation */
	reqctx->type        = NA51000_CRYPTO_TYPE_ENCRYPT;
	reqctx->mode        = (ctx->keylen == AES_KEYSIZE_256) ? NA51000_CRYPTO_MODE_AES_256 : NA51000_CRYPTO_MODE_AES_128;
	reqctx->opmode      = NA51000_CRYPTO_OPMODE_OFB;
	reqctx->accmode     = NA51000_CRYPTO_ACCMODE_DMA;
	reqctx->byte_endian = NA51000_CRYPTO_BYTE_ENDIAN_BIG;
	reqctx->bit_order   = NA51000_CRYPTO_BIT_ORDER_0_7;

	return na51000_crypto_handle_req(ctx->dev, req);
}

static int na51000_crypto_aes_ofb_decrypt(struct ablkcipher_request *req)
{
	struct crypto_ablkcipher     *tfm    = crypto_ablkcipher_reqtfm(req);
	struct na51000_crypto_ctx    *ctx    = crypto_ablkcipher_ctx(tfm);
	struct na51000_crypto_reqctx *reqctx = ablkcipher_request_ctx(req);

	if (!IS_ALIGNED(req->nbytes, AES_BLOCK_SIZE)) {
		pr_err("request size is not exact amount of AES blocks(nbytes:%d)\n", req->nbytes);
		return -EINVAL;
	}

	if (unlikely(ctx->keylen != AES_KEYSIZE_128 && ctx->keylen != AES_KEYSIZE_256)) {
		return na51000_crypto_aes_do_fallback(req, 0);
	}

	/* setup device */
	ctx->dev        = na51000_cdev;
	ctx->block_size = AES_BLOCK_SIZE;

	/* setup operation */
	reqctx->type        = NA51000_CRYPTO_TYPE_DECRYPT;
	reqctx->mode        = (ctx->keylen == AES_KEYSIZE_256) ? NA51000_CRYPTO_MODE_AES_256 : NA51000_CRYPTO_MODE_AES_128;
	reqctx->opmode      = NA51000_CRYPTO_OPMODE_OFB;
	reqctx->accmode     = NA51000_CRYPTO_ACCMODE_DMA;
	reqctx->byte_endian = NA51000_CRYPTO_BYTE_ENDIAN_BIG;
	reqctx->bit_order   = NA51000_CRYPTO_BIT_ORDER_0_7;

	return na51000_crypto_handle_req(ctx->dev, req);
}

static int na51000_crypto_aes_ctr_encrypt(struct ablkcipher_request *req)
{
	struct crypto_ablkcipher     *tfm    = crypto_ablkcipher_reqtfm(req);
	struct na51000_crypto_ctx    *ctx    = crypto_ablkcipher_ctx(tfm);
	struct na51000_crypto_reqctx *reqctx = ablkcipher_request_ctx(req);

	if (!IS_ALIGNED(req->nbytes, AES_BLOCK_SIZE)) {
		pr_err("request size is not exact amount of AES blocks(nbytes:%d)\n", req->nbytes);
		return -EINVAL;
	}

	if (unlikely(ctx->keylen != AES_KEYSIZE_128 && ctx->keylen != AES_KEYSIZE_256)) {
		return na51000_crypto_aes_do_fallback(req, 1);
	}

	/* setup device */
	ctx->dev        = na51000_cdev;
	ctx->block_size = AES_BLOCK_SIZE;

	/* setup operation */
	reqctx->type        = NA51000_CRYPTO_TYPE_ENCRYPT;
	reqctx->mode        = (ctx->keylen == AES_KEYSIZE_256) ? NA51000_CRYPTO_MODE_AES_256 : NA51000_CRYPTO_MODE_AES_128;
	reqctx->opmode      = NA51000_CRYPTO_OPMODE_CTR;
	reqctx->accmode     = NA51000_CRYPTO_ACCMODE_DMA;
	reqctx->byte_endian = NA51000_CRYPTO_BYTE_ENDIAN_BIG;
	reqctx->bit_order   = NA51000_CRYPTO_BIT_ORDER_0_7;

	return na51000_crypto_handle_req(ctx->dev, req);
}

static int na51000_crypto_aes_ctr_decrypt(struct ablkcipher_request *req)
{
	struct crypto_ablkcipher     *tfm    = crypto_ablkcipher_reqtfm(req);
	struct na51000_crypto_ctx    *ctx    = crypto_ablkcipher_ctx(tfm);
	struct na51000_crypto_reqctx *reqctx = ablkcipher_request_ctx(req);

	if (!IS_ALIGNED(req->nbytes, AES_BLOCK_SIZE)) {
		pr_err("request size is not exact amount of AES blocks(nbytes:%d)\n", req->nbytes);
		return -EINVAL;
	}

	if (unlikely(ctx->keylen != AES_KEYSIZE_128 && ctx->keylen != AES_KEYSIZE_256)) {
		return na51000_crypto_aes_do_fallback(req, 0);
	}

	/* setup device */
	ctx->dev        = na51000_cdev;
	ctx->block_size = AES_BLOCK_SIZE;

	/* setup operation */
	reqctx->type        = NA51000_CRYPTO_TYPE_DECRYPT;
	reqctx->mode        = (ctx->keylen == AES_KEYSIZE_256) ? NA51000_CRYPTO_MODE_AES_256 : NA51000_CRYPTO_MODE_AES_128;
	reqctx->opmode      = NA51000_CRYPTO_OPMODE_CTR;
	reqctx->accmode     = NA51000_CRYPTO_ACCMODE_DMA;
	reqctx->byte_endian = NA51000_CRYPTO_BYTE_ENDIAN_BIG;
	reqctx->bit_order   = NA51000_CRYPTO_BIT_ORDER_0_7;

	return na51000_crypto_handle_req(ctx->dev, req);
}

static int na51000_crypto_des_setkey(struct crypto_ablkcipher *tfm, const u8 *key, unsigned int keylen)
{
	struct na51000_crypto_ctx *ctx = crypto_ablkcipher_ctx(tfm);

	if (unlikely(keylen != DES_KEY_SIZE && keylen != DES3_EDE_KEY_SIZE)) {
		crypto_ablkcipher_set_flags(tfm, CRYPTO_TFM_RES_BAD_KEY_LEN);
		return -EINVAL;
	}

	/* check for weak keys */
	if (crypto_ablkcipher_get_flags(tfm) & CRYPTO_TFM_REQ_WEAK_KEY)
		return -EINVAL;

	memcpy(ctx->key, key, keylen);
	ctx->keylen = keylen;

	return 0;
}

static int na51000_crypto_des_ecb_encrypt(struct ablkcipher_request *req)
{
	struct crypto_ablkcipher     *tfm    = crypto_ablkcipher_reqtfm(req);
	struct na51000_crypto_ctx    *ctx    = crypto_ablkcipher_ctx(tfm);
	struct na51000_crypto_reqctx *reqctx = ablkcipher_request_ctx(req);

	if (!IS_ALIGNED(req->nbytes, DES_BLOCK_SIZE)) {
		pr_err("request size is not exact amount of DES blocks(nbytes:%d)\n", req->nbytes);
		return -EINVAL;
	}

	if (unlikely(ctx->keylen != DES_KEY_SIZE && ctx->keylen != DES3_EDE_KEY_SIZE)) {
		return -EINVAL;
	}

	/* setup device */
	ctx->dev        = na51000_cdev;
	ctx->block_size = DES_BLOCK_SIZE;

	/* setup operation */
	reqctx->type        = NA51000_CRYPTO_TYPE_ENCRYPT;
	reqctx->mode        = (ctx->keylen == DES_KEY_SIZE) ? NA51000_CRYPTO_MODE_DES : NA51000_CRYPTO_MODE_3DES;
	reqctx->opmode      = NA51000_CRYPTO_OPMODE_ECB;
	reqctx->accmode     = NA51000_CRYPTO_ACCMODE_DMA;
	reqctx->byte_endian = NA51000_CRYPTO_BYTE_ENDIAN_BIG;
	reqctx->bit_order   = NA51000_CRYPTO_BIT_ORDER_0_7;

	return na51000_crypto_handle_req(ctx->dev, req);
}

static int na51000_crypto_des_ecb_decrypt(struct ablkcipher_request *req)
{
	struct crypto_ablkcipher     *tfm    = crypto_ablkcipher_reqtfm(req);
	struct na51000_crypto_ctx    *ctx    = crypto_ablkcipher_ctx(tfm);
	struct na51000_crypto_reqctx *reqctx = ablkcipher_request_ctx(req);

	if (!IS_ALIGNED(req->nbytes, DES_BLOCK_SIZE)) {
		pr_err("request size is not exact amount of DES blocks(nbytes:%d)\n", req->nbytes);
		return -EINVAL;
	}

	if (unlikely(ctx->keylen != DES_KEY_SIZE && ctx->keylen != DES3_EDE_KEY_SIZE)) {
		return -EINVAL;
	}

	/* setup device */
	ctx->dev        = na51000_cdev;
	ctx->block_size = DES_BLOCK_SIZE;

	/* setup operation */
	reqctx->type        = NA51000_CRYPTO_TYPE_DECRYPT;
	reqctx->mode        = (ctx->keylen == DES_KEY_SIZE) ? NA51000_CRYPTO_MODE_DES : NA51000_CRYPTO_MODE_3DES;
	reqctx->opmode      = NA51000_CRYPTO_OPMODE_ECB;
	reqctx->accmode     = NA51000_CRYPTO_ACCMODE_DMA;
	reqctx->byte_endian = NA51000_CRYPTO_BYTE_ENDIAN_BIG;
	reqctx->bit_order   = NA51000_CRYPTO_BIT_ORDER_0_7;

	return na51000_crypto_handle_req(ctx->dev, req);
}

static int na51000_crypto_des_cbc_encrypt(struct ablkcipher_request *req)
{
	struct crypto_ablkcipher     *tfm    = crypto_ablkcipher_reqtfm(req);
	struct na51000_crypto_ctx    *ctx    = crypto_ablkcipher_ctx(tfm);
	struct na51000_crypto_reqctx *reqctx = ablkcipher_request_ctx(req);

	if (!IS_ALIGNED(req->nbytes, DES_BLOCK_SIZE)) {
		pr_err("request size is not exact amount of DES blocks(nbytes:%d)\n", req->nbytes);
		return -EINVAL;
	}

	if (unlikely(ctx->keylen != DES_KEY_SIZE && ctx->keylen != DES3_EDE_KEY_SIZE)) {
		return -EINVAL;
	}

	/* setup device */
	ctx->dev        = na51000_cdev;
	ctx->block_size = DES_BLOCK_SIZE;

	/* setup operation */
	reqctx->type        = NA51000_CRYPTO_TYPE_ENCRYPT;
	reqctx->mode        = (ctx->keylen == DES_KEY_SIZE) ? NA51000_CRYPTO_MODE_DES : NA51000_CRYPTO_MODE_3DES;
	reqctx->opmode      = NA51000_CRYPTO_OPMODE_CBC;
	reqctx->accmode     = NA51000_CRYPTO_ACCMODE_DMA;
	reqctx->byte_endian = NA51000_CRYPTO_BYTE_ENDIAN_BIG;
	reqctx->bit_order   = NA51000_CRYPTO_BIT_ORDER_0_7;

	return na51000_crypto_handle_req(ctx->dev, req);
}

static int na51000_crypto_des_cbc_decrypt(struct ablkcipher_request *req)
{
	struct crypto_ablkcipher     *tfm    = crypto_ablkcipher_reqtfm(req);
	struct na51000_crypto_ctx    *ctx    = crypto_ablkcipher_ctx(tfm);
	struct na51000_crypto_reqctx *reqctx = ablkcipher_request_ctx(req);

	if (!IS_ALIGNED(req->nbytes, DES_BLOCK_SIZE)) {
		pr_err("request size is not exact amount of DES blocks(nbytes:%d)\n", req->nbytes);
		return -EINVAL;
	}

	if (unlikely(ctx->keylen != DES_KEY_SIZE && ctx->keylen != DES3_EDE_KEY_SIZE)) {
		return -EINVAL;
	}

	/* setup device */
	ctx->dev        = na51000_cdev;
	ctx->block_size = DES_BLOCK_SIZE;

	/* setup operation */
	reqctx->type        = NA51000_CRYPTO_TYPE_DECRYPT;
	reqctx->mode        = (ctx->keylen == DES_KEY_SIZE) ? NA51000_CRYPTO_MODE_DES : NA51000_CRYPTO_MODE_3DES;
	reqctx->opmode      = NA51000_CRYPTO_OPMODE_CBC;
	reqctx->accmode     = NA51000_CRYPTO_ACCMODE_DMA;
	reqctx->byte_endian = NA51000_CRYPTO_BYTE_ENDIAN_BIG;
	reqctx->bit_order   = NA51000_CRYPTO_BIT_ORDER_0_7;

	return na51000_crypto_handle_req(ctx->dev, req);
}

static int na51000_crypto_des_cfb_encrypt(struct ablkcipher_request *req)
{
	struct crypto_ablkcipher     *tfm    = crypto_ablkcipher_reqtfm(req);
	struct na51000_crypto_ctx    *ctx    = crypto_ablkcipher_ctx(tfm);
	struct na51000_crypto_reqctx *reqctx = ablkcipher_request_ctx(req);

	if (!IS_ALIGNED(req->nbytes, DES_BLOCK_SIZE)) {
		pr_err("request size is not exact amount of DES blocks(nbytes:%d)\n", req->nbytes);
		return -EINVAL;
	}

	if (unlikely(ctx->keylen != DES_KEY_SIZE && ctx->keylen != DES3_EDE_KEY_SIZE)) {
		return -EINVAL;
	}

	/* setup device */
	ctx->dev        = na51000_cdev;
	ctx->block_size = DES_BLOCK_SIZE;

	/* setup operation */
	reqctx->type        = NA51000_CRYPTO_TYPE_ENCRYPT;
	reqctx->mode        = (ctx->keylen == DES_KEY_SIZE) ? NA51000_CRYPTO_MODE_DES : NA51000_CRYPTO_MODE_3DES;
	reqctx->opmode      = NA51000_CRYPTO_OPMODE_CFB;
	reqctx->accmode     = NA51000_CRYPTO_ACCMODE_DMA;
	reqctx->byte_endian = NA51000_CRYPTO_BYTE_ENDIAN_BIG;
	reqctx->bit_order   = NA51000_CRYPTO_BIT_ORDER_0_7;

	return na51000_crypto_handle_req(ctx->dev, req);
}

static int na51000_crypto_des_cfb_decrypt(struct ablkcipher_request *req)
{
	struct crypto_ablkcipher     *tfm    = crypto_ablkcipher_reqtfm(req);
	struct na51000_crypto_ctx    *ctx    = crypto_ablkcipher_ctx(tfm);
	struct na51000_crypto_reqctx *reqctx = ablkcipher_request_ctx(req);

	if (!IS_ALIGNED(req->nbytes, DES_BLOCK_SIZE)) {
		pr_err("request size is not exact amount of DES blocks(nbytes:%d)\n", req->nbytes);
		return -EINVAL;
	}

	if (unlikely(ctx->keylen != DES_KEY_SIZE && ctx->keylen != DES3_EDE_KEY_SIZE)) {
		return -EINVAL;
	}

	/* setup device */
	ctx->dev        = na51000_cdev;
	ctx->block_size = DES_BLOCK_SIZE;

	/* setup operation */
	reqctx->type        = NA51000_CRYPTO_TYPE_DECRYPT;
	reqctx->mode        = (ctx->keylen == DES_KEY_SIZE) ? NA51000_CRYPTO_MODE_DES : NA51000_CRYPTO_MODE_3DES;
	reqctx->opmode      = NA51000_CRYPTO_OPMODE_CFB;
	reqctx->accmode     = NA51000_CRYPTO_ACCMODE_DMA;
	reqctx->byte_endian = NA51000_CRYPTO_BYTE_ENDIAN_BIG;
	reqctx->bit_order   = NA51000_CRYPTO_BIT_ORDER_0_7;

	return na51000_crypto_handle_req(ctx->dev, req);
}

static int na51000_crypto_des_ofb_encrypt(struct ablkcipher_request *req)
{
	struct crypto_ablkcipher     *tfm    = crypto_ablkcipher_reqtfm(req);
	struct na51000_crypto_ctx    *ctx    = crypto_ablkcipher_ctx(tfm);
	struct na51000_crypto_reqctx *reqctx = ablkcipher_request_ctx(req);

	if (!IS_ALIGNED(req->nbytes, DES_BLOCK_SIZE)) {
		pr_err("request size is not exact amount of DES blocks(nbytes:%d)\n", req->nbytes);
		return -EINVAL;
	}

	if (unlikely(ctx->keylen != DES_KEY_SIZE && ctx->keylen != DES3_EDE_KEY_SIZE)) {
		return -EINVAL;
	}

	/* setup device */
	ctx->dev        = na51000_cdev;
	ctx->block_size = DES_BLOCK_SIZE;

	/* setup operation */
	reqctx->type        = NA51000_CRYPTO_TYPE_ENCRYPT;
	reqctx->mode        = (ctx->keylen == DES_KEY_SIZE) ? NA51000_CRYPTO_MODE_DES : NA51000_CRYPTO_MODE_3DES;
	reqctx->opmode      = NA51000_CRYPTO_OPMODE_OFB;
	reqctx->accmode     = NA51000_CRYPTO_ACCMODE_DMA;
	reqctx->byte_endian = NA51000_CRYPTO_BYTE_ENDIAN_BIG;
	reqctx->bit_order   = NA51000_CRYPTO_BIT_ORDER_0_7;

	return na51000_crypto_handle_req(ctx->dev, req);
}

static int na51000_crypto_des_ofb_decrypt(struct ablkcipher_request *req)
{
	struct crypto_ablkcipher     *tfm    = crypto_ablkcipher_reqtfm(req);
	struct na51000_crypto_ctx    *ctx    = crypto_ablkcipher_ctx(tfm);
	struct na51000_crypto_reqctx *reqctx = ablkcipher_request_ctx(req);

	if (!IS_ALIGNED(req->nbytes, DES_BLOCK_SIZE)) {
		pr_err("request size is not exact amount of DES blocks(nbytes:%d)\n", req->nbytes);
		return -EINVAL;
	}

	if (unlikely(ctx->keylen != DES_KEY_SIZE && ctx->keylen != DES3_EDE_KEY_SIZE)) {
		return -EINVAL;
	}

	/* setup device */
	ctx->dev        = na51000_cdev;
	ctx->block_size = DES_BLOCK_SIZE;

	/* setup operation */
	reqctx->type        = NA51000_CRYPTO_TYPE_DECRYPT;
	reqctx->mode        = (ctx->keylen == DES_KEY_SIZE) ? NA51000_CRYPTO_MODE_DES : NA51000_CRYPTO_MODE_3DES;
	reqctx->opmode      = NA51000_CRYPTO_OPMODE_OFB;
	reqctx->accmode     = NA51000_CRYPTO_ACCMODE_DMA;
	reqctx->byte_endian = NA51000_CRYPTO_BYTE_ENDIAN_BIG;
	reqctx->bit_order   = NA51000_CRYPTO_BIT_ORDER_0_7;

	return na51000_crypto_handle_req(ctx->dev, req);
}

static int na51000_crypto_des_ctr_encrypt(struct ablkcipher_request *req)
{
	struct crypto_ablkcipher     *tfm    = crypto_ablkcipher_reqtfm(req);
	struct na51000_crypto_ctx    *ctx    = crypto_ablkcipher_ctx(tfm);
	struct na51000_crypto_reqctx *reqctx = ablkcipher_request_ctx(req);

	if (!IS_ALIGNED(req->nbytes, DES_BLOCK_SIZE)) {
		pr_err("request size is not exact amount of DES blocks(nbytes:%d)\n", req->nbytes);
		return -EINVAL;
	}

	if (unlikely(ctx->keylen != DES_KEY_SIZE && ctx->keylen != DES3_EDE_KEY_SIZE)) {
		return -EINVAL;
	}

	/* setup device */
	ctx->dev        = na51000_cdev;
	ctx->block_size = DES_BLOCK_SIZE;

	/* setup operation */
	reqctx->type        = NA51000_CRYPTO_TYPE_ENCRYPT;
	reqctx->mode        = (ctx->keylen == DES_KEY_SIZE) ? NA51000_CRYPTO_MODE_DES : NA51000_CRYPTO_MODE_3DES;
	reqctx->opmode      = NA51000_CRYPTO_OPMODE_CTR;
	reqctx->accmode     = NA51000_CRYPTO_ACCMODE_DMA;
	reqctx->byte_endian = NA51000_CRYPTO_BYTE_ENDIAN_BIG;
	reqctx->bit_order   = NA51000_CRYPTO_BIT_ORDER_0_7;

	return na51000_crypto_handle_req(ctx->dev, req);
}

static int na51000_crypto_des_ctr_decrypt(struct ablkcipher_request *req)
{
	struct crypto_ablkcipher     *tfm    = crypto_ablkcipher_reqtfm(req);
	struct na51000_crypto_ctx    *ctx    = crypto_ablkcipher_ctx(tfm);
	struct na51000_crypto_reqctx *reqctx = ablkcipher_request_ctx(req);

	if (!IS_ALIGNED(req->nbytes, DES_BLOCK_SIZE)) {
		pr_err("request size is not exact amount of DES blocks(nbytes:%d)\n", req->nbytes);
		return -EINVAL;
	}

	if (unlikely(ctx->keylen != DES_KEY_SIZE && ctx->keylen != DES3_EDE_KEY_SIZE)) {
		return -EINVAL;
	}

	/* setup device */
	ctx->dev        = na51000_cdev;
	ctx->block_size = DES_BLOCK_SIZE;

	/* setup operation */
	reqctx->type        = NA51000_CRYPTO_TYPE_DECRYPT;
	reqctx->mode        = (ctx->keylen == DES_KEY_SIZE) ? NA51000_CRYPTO_MODE_DES : NA51000_CRYPTO_MODE_3DES;
	reqctx->opmode      = NA51000_CRYPTO_OPMODE_CTR;
	reqctx->accmode     = NA51000_CRYPTO_ACCMODE_DMA;
	reqctx->byte_endian = NA51000_CRYPTO_BYTE_ENDIAN_BIG;
	reqctx->bit_order   = NA51000_CRYPTO_BIT_ORDER_0_7;

	return na51000_crypto_handle_req(ctx->dev, req);
}

static irqreturn_t na51000_crypto_irq(int irq, void *dev_id)
{
	struct na51000_crypto_dev *dev = (struct na51000_crypto_dev *)dev_id;
	u32                       status;

	spin_lock(&dev->lock);

	/* read DMA status */
	status = na51000_crypto_read(dev, NA51000_CRYPTO_DMA_STS_REG);

	/* clear DMA status */
	na51000_crypto_write(dev, NA51000_CRYPTO_DMA_STS_REG, status);

	/* DMA transfer done */
	if (status & 0x1) {
		del_timer(&dev->timer);
		tasklet_schedule(&dev->done_tasklet);
	}

	spin_unlock(&dev->lock);

	return IRQ_HANDLED;
}

static struct crypto_alg na51000_crypto_algs[] = {
	{
		.cra_name			= "ecb(aes)",
		.cra_driver_name	= "na51000-ecb-aes",
		.cra_priority		= NA5100_CRYPTO_ALG_PRIORITY,
		.cra_flags			= CRYPTO_ALG_TYPE_ABLKCIPHER | CRYPTO_ALG_ASYNC | CRYPTO_ALG_KERN_DRIVER_ONLY | CRYPTO_ALG_NEED_FALLBACK,
		.cra_blocksize		= AES_BLOCK_SIZE,
		.cra_ctxsize		= sizeof(struct na51000_crypto_ctx),
		.cra_alignmask		= 0x3,
		.cra_type			= &crypto_ablkcipher_type,
		.cra_module			= THIS_MODULE,
		.cra_init			= na51000_crypto_cra_init,
		.cra_exit			= na51000_crypto_cra_exit,
		.cra_u.ablkcipher = {
			.min_keysize	= AES_MIN_KEY_SIZE,
			.max_keysize	= AES_MAX_KEY_SIZE,
			.setkey			= na51000_crypto_aes_setkey,
			.encrypt		= na51000_crypto_aes_ecb_encrypt,
			.decrypt		= na51000_crypto_aes_ecb_decrypt,
		}
	},
	{
		.cra_name			= "cbc(aes)",
		.cra_driver_name	= "na51000-cbc-aes",
		.cra_priority		= NA5100_CRYPTO_ALG_PRIORITY,
		.cra_flags			= CRYPTO_ALG_TYPE_ABLKCIPHER | CRYPTO_ALG_ASYNC | CRYPTO_ALG_KERN_DRIVER_ONLY | CRYPTO_ALG_NEED_FALLBACK,
		.cra_blocksize		= AES_BLOCK_SIZE,
		.cra_ctxsize		= sizeof(struct na51000_crypto_ctx),
		.cra_alignmask		= 0x3,
		.cra_type			= &crypto_ablkcipher_type,
		.cra_module			= THIS_MODULE,
		.cra_init			= na51000_crypto_cra_init,
		.cra_exit			= na51000_crypto_cra_exit,
		.cra_u.ablkcipher = {
			.min_keysize	= AES_MIN_KEY_SIZE,
			.max_keysize	= AES_MAX_KEY_SIZE,
			.ivsize			= AES_BLOCK_SIZE,
			.setkey			= na51000_crypto_aes_setkey,
			.encrypt		= na51000_crypto_aes_cbc_encrypt,
			.decrypt		= na51000_crypto_aes_cbc_decrypt,
		}
	},
	{
		.cra_name			= "cfb(aes)",
		.cra_driver_name	= "na51000-cfb-aes",
		.cra_priority		= NA5100_CRYPTO_ALG_PRIORITY,
		.cra_flags			= CRYPTO_ALG_TYPE_ABLKCIPHER | CRYPTO_ALG_ASYNC | CRYPTO_ALG_KERN_DRIVER_ONLY | CRYPTO_ALG_NEED_FALLBACK,
		.cra_blocksize		= AES_BLOCK_SIZE,
		.cra_ctxsize		= sizeof(struct na51000_crypto_ctx),
		.cra_alignmask		= 0x3,
		.cra_type			= &crypto_ablkcipher_type,
		.cra_module			= THIS_MODULE,
		.cra_init			= na51000_crypto_cra_init,
		.cra_exit			= na51000_crypto_cra_exit,
		.cra_u.ablkcipher = {
			.min_keysize	= AES_MIN_KEY_SIZE,
			.max_keysize	= AES_MAX_KEY_SIZE,
			.ivsize			= AES_BLOCK_SIZE,
			.setkey			= na51000_crypto_aes_setkey,
			.encrypt		= na51000_crypto_aes_cfb_encrypt,
			.decrypt		= na51000_crypto_aes_cfb_decrypt,
		}
	},
	{
		.cra_name			= "ofb(aes)",
		.cra_driver_name	= "na51000-ofb-aes",
		.cra_priority		= NA5100_CRYPTO_ALG_PRIORITY,
		.cra_flags			= CRYPTO_ALG_TYPE_ABLKCIPHER | CRYPTO_ALG_ASYNC | CRYPTO_ALG_KERN_DRIVER_ONLY | CRYPTO_ALG_NEED_FALLBACK,
		.cra_blocksize		= AES_BLOCK_SIZE,
		.cra_ctxsize		= sizeof(struct na51000_crypto_ctx),
		.cra_alignmask		= 0x3,
		.cra_type			= &crypto_ablkcipher_type,
		.cra_module			= THIS_MODULE,
		.cra_init			= na51000_crypto_cra_init,
		.cra_exit			= na51000_crypto_cra_exit,
		.cra_u.ablkcipher = {
			.min_keysize	= AES_MIN_KEY_SIZE,
			.max_keysize	= AES_MAX_KEY_SIZE,
			.ivsize			= AES_BLOCK_SIZE,
			.setkey			= na51000_crypto_aes_setkey,
			.encrypt		= na51000_crypto_aes_ofb_encrypt,
			.decrypt		= na51000_crypto_aes_ofb_decrypt,
		}
	},
	{
		.cra_name			= "ctr(aes)",
		.cra_driver_name	= "na51000-ctr-aes",
		.cra_priority		= NA5100_CRYPTO_ALG_PRIORITY,
		.cra_flags			= CRYPTO_ALG_TYPE_ABLKCIPHER | CRYPTO_ALG_ASYNC | CRYPTO_ALG_KERN_DRIVER_ONLY | CRYPTO_ALG_NEED_FALLBACK,
		.cra_blocksize		= AES_BLOCK_SIZE,
		.cra_ctxsize		= sizeof(struct na51000_crypto_ctx),
		.cra_alignmask		= 0x3,
		.cra_type			= &crypto_ablkcipher_type,
		.cra_module			= THIS_MODULE,
		.cra_init			= na51000_crypto_cra_init,
		.cra_exit			= na51000_crypto_cra_exit,
		.cra_u.ablkcipher = {
			.min_keysize	= AES_MIN_KEY_SIZE,
			.max_keysize	= AES_MAX_KEY_SIZE,
			.ivsize			= AES_BLOCK_SIZE,
			.setkey			= na51000_crypto_aes_setkey,
			.encrypt		= na51000_crypto_aes_ctr_encrypt,
			.decrypt		= na51000_crypto_aes_ctr_decrypt,
		}
	},
	{
		.cra_name			= "ecb(des)",
		.cra_driver_name	= "na51000-ecb-des",
		.cra_priority		= NA5100_CRYPTO_ALG_PRIORITY,
		.cra_flags			= CRYPTO_ALG_TYPE_ABLKCIPHER | CRYPTO_ALG_ASYNC | CRYPTO_ALG_KERN_DRIVER_ONLY,
		.cra_blocksize		= DES_BLOCK_SIZE,
		.cra_ctxsize		= sizeof(struct na51000_crypto_ctx),
		.cra_alignmask		= 0x3,
		.cra_type			= &crypto_ablkcipher_type,
		.cra_module			= THIS_MODULE,
		.cra_init			= na51000_crypto_cra_init,
		.cra_exit			= na51000_crypto_cra_exit,
		.cra_u.ablkcipher = {
			.min_keysize	= DES_KEY_SIZE,
			.max_keysize	= DES_KEY_SIZE,
			.setkey			= na51000_crypto_des_setkey,
			.encrypt		= na51000_crypto_des_ecb_encrypt,
			.decrypt		= na51000_crypto_des_ecb_decrypt,
		}
	},
	{
		.cra_name			= "cbc(des)",
		.cra_driver_name	= "na51000-cbc-des",
		.cra_priority		= NA5100_CRYPTO_ALG_PRIORITY,
		.cra_flags			= CRYPTO_ALG_TYPE_ABLKCIPHER | CRYPTO_ALG_ASYNC | CRYPTO_ALG_KERN_DRIVER_ONLY,
		.cra_blocksize		= DES_BLOCK_SIZE,
		.cra_ctxsize		= sizeof(struct na51000_crypto_ctx),
		.cra_alignmask		= 0x3,
		.cra_type			= &crypto_ablkcipher_type,
		.cra_module			= THIS_MODULE,
		.cra_init			= na51000_crypto_cra_init,
		.cra_exit			= na51000_crypto_cra_exit,
		.cra_u.ablkcipher = {
			.min_keysize	= DES_KEY_SIZE,
			.max_keysize	= DES_KEY_SIZE,
			.ivsize		    = DES_BLOCK_SIZE,
			.setkey			= na51000_crypto_des_setkey,
			.encrypt		= na51000_crypto_des_cbc_encrypt,
			.decrypt		= na51000_crypto_des_cbc_decrypt,
		}
	},
	{
		.cra_name			= "cfb(des)",
		.cra_driver_name	= "na51000-cfb-des",
		.cra_priority		= NA5100_CRYPTO_ALG_PRIORITY,
		.cra_flags			= CRYPTO_ALG_TYPE_ABLKCIPHER | CRYPTO_ALG_ASYNC | CRYPTO_ALG_KERN_DRIVER_ONLY,
		.cra_blocksize		= DES_BLOCK_SIZE,
		.cra_ctxsize		= sizeof(struct na51000_crypto_ctx),
		.cra_alignmask		= 0x3,
		.cra_type			= &crypto_ablkcipher_type,
		.cra_module			= THIS_MODULE,
		.cra_init			= na51000_crypto_cra_init,
		.cra_exit			= na51000_crypto_cra_exit,
		.cra_u.ablkcipher = {
			.min_keysize	= DES_KEY_SIZE,
			.max_keysize	= DES_KEY_SIZE,
			.ivsize		    = DES_BLOCK_SIZE,
			.setkey			= na51000_crypto_des_setkey,
			.encrypt		= na51000_crypto_des_cfb_encrypt,
			.decrypt		= na51000_crypto_des_cfb_decrypt,
		}
	},
	{
		.cra_name			= "ofb(des)",
		.cra_driver_name	= "na51000-ofb-des",
		.cra_priority		= NA5100_CRYPTO_ALG_PRIORITY,
		.cra_flags			= CRYPTO_ALG_TYPE_ABLKCIPHER | CRYPTO_ALG_ASYNC | CRYPTO_ALG_KERN_DRIVER_ONLY,
		.cra_blocksize		= DES_BLOCK_SIZE,
		.cra_ctxsize		= sizeof(struct na51000_crypto_ctx),
		.cra_alignmask		= 0x3,
		.cra_type			= &crypto_ablkcipher_type,
		.cra_module			= THIS_MODULE,
		.cra_init			= na51000_crypto_cra_init,
		.cra_exit			= na51000_crypto_cra_exit,
		.cra_u.ablkcipher = {
			.min_keysize	= DES_KEY_SIZE,
			.max_keysize	= DES_KEY_SIZE,
			.ivsize		    = DES_BLOCK_SIZE,
			.setkey			= na51000_crypto_des_setkey,
			.encrypt		= na51000_crypto_des_ofb_encrypt,
			.decrypt		= na51000_crypto_des_ofb_decrypt,
		}
	},
	{
		.cra_name			= "ctr(des)",
		.cra_driver_name	= "na51000-ctr-des",
		.cra_priority		= NA5100_CRYPTO_ALG_PRIORITY,
		.cra_flags			= CRYPTO_ALG_TYPE_ABLKCIPHER | CRYPTO_ALG_ASYNC | CRYPTO_ALG_KERN_DRIVER_ONLY,
		.cra_blocksize		= DES_BLOCK_SIZE,
		.cra_ctxsize		= sizeof(struct na51000_crypto_ctx),
		.cra_alignmask		= 0x3,
		.cra_type			= &crypto_ablkcipher_type,
		.cra_module			= THIS_MODULE,
		.cra_init			= na51000_crypto_cra_init,
		.cra_exit			= na51000_crypto_cra_exit,
		.cra_u.ablkcipher = {
			.min_keysize	= DES_KEY_SIZE,
			.max_keysize	= DES_KEY_SIZE,
			.ivsize		    = DES_BLOCK_SIZE,
			.setkey			= na51000_crypto_des_setkey,
			.encrypt		= na51000_crypto_des_ctr_encrypt,
			.decrypt		= na51000_crypto_des_ctr_decrypt,
		}
	},
	{
		.cra_name			= "ecb(des3_ede)",
		.cra_driver_name	= "na51000-ecb-des3",
		.cra_priority		= NA5100_CRYPTO_ALG_PRIORITY,
		.cra_flags			= CRYPTO_ALG_TYPE_ABLKCIPHER | CRYPTO_ALG_ASYNC | CRYPTO_ALG_KERN_DRIVER_ONLY,
		.cra_blocksize		= DES_BLOCK_SIZE,
		.cra_ctxsize		= sizeof(struct na51000_crypto_ctx),
		.cra_alignmask		= 0x3,
		.cra_type			= &crypto_ablkcipher_type,
		.cra_module			= THIS_MODULE,
		.cra_init			= na51000_crypto_cra_init,
		.cra_exit			= na51000_crypto_cra_exit,
		.cra_u.ablkcipher = {
			.min_keysize	= DES3_EDE_KEY_SIZE,
			.max_keysize	= DES3_EDE_KEY_SIZE,
			.setkey			= na51000_crypto_des_setkey,
			.encrypt		= na51000_crypto_des_ecb_encrypt,
			.decrypt		= na51000_crypto_des_ecb_decrypt,
		}
	},
	{
		.cra_name			= "cbc(des3_ede)",
		.cra_driver_name	= "na51000-cbc-des3",
		.cra_priority		= NA5100_CRYPTO_ALG_PRIORITY,
		.cra_flags			= CRYPTO_ALG_TYPE_ABLKCIPHER | CRYPTO_ALG_ASYNC | CRYPTO_ALG_KERN_DRIVER_ONLY,
		.cra_blocksize		= DES_BLOCK_SIZE,
		.cra_ctxsize		= sizeof(struct na51000_crypto_ctx),
		.cra_alignmask		= 0x3,
		.cra_type			= &crypto_ablkcipher_type,
		.cra_module			= THIS_MODULE,
		.cra_init			= na51000_crypto_cra_init,
		.cra_exit			= na51000_crypto_cra_exit,
		.cra_u.ablkcipher = {
			.min_keysize	= DES3_EDE_KEY_SIZE,
			.max_keysize	= DES3_EDE_KEY_SIZE,
			.ivsize		    = DES_BLOCK_SIZE,
			.setkey			= na51000_crypto_des_setkey,
			.encrypt		= na51000_crypto_des_cbc_encrypt,
			.decrypt		= na51000_crypto_des_cbc_decrypt,
		}
	},
	{
		.cra_name			= "cfb(des3_ede)",
		.cra_driver_name	= "na51000-cfb-des3",
		.cra_priority		= NA5100_CRYPTO_ALG_PRIORITY,
		.cra_flags			= CRYPTO_ALG_TYPE_ABLKCIPHER | CRYPTO_ALG_ASYNC | CRYPTO_ALG_KERN_DRIVER_ONLY,
		.cra_blocksize		= DES_BLOCK_SIZE,
		.cra_ctxsize		= sizeof(struct na51000_crypto_ctx),
		.cra_alignmask		= 0x3,
		.cra_type			= &crypto_ablkcipher_type,
		.cra_module			= THIS_MODULE,
		.cra_init			= na51000_crypto_cra_init,
		.cra_exit			= na51000_crypto_cra_exit,
		.cra_u.ablkcipher = {
			.min_keysize	= DES3_EDE_KEY_SIZE,
			.max_keysize	= DES3_EDE_KEY_SIZE,
			.ivsize		    = DES_BLOCK_SIZE,
			.setkey			= na51000_crypto_des_setkey,
			.encrypt		= na51000_crypto_des_cfb_encrypt,
			.decrypt		= na51000_crypto_des_cfb_decrypt,
		}
	},
	{
		.cra_name			= "ofb(des3_ede)",
		.cra_driver_name	= "na51000-ofb-des3",
		.cra_priority		= NA5100_CRYPTO_ALG_PRIORITY,
		.cra_flags			= CRYPTO_ALG_TYPE_ABLKCIPHER | CRYPTO_ALG_ASYNC | CRYPTO_ALG_KERN_DRIVER_ONLY,
		.cra_blocksize		= DES_BLOCK_SIZE,
		.cra_ctxsize		= sizeof(struct na51000_crypto_ctx),
		.cra_alignmask		= 0x3,
		.cra_type			= &crypto_ablkcipher_type,
		.cra_module			= THIS_MODULE,
		.cra_init			= na51000_crypto_cra_init,
		.cra_exit			= na51000_crypto_cra_exit,
		.cra_u.ablkcipher = {
			.min_keysize	= DES3_EDE_KEY_SIZE,
			.max_keysize	= DES3_EDE_KEY_SIZE,
			.ivsize		    = DES_BLOCK_SIZE,
			.setkey			= na51000_crypto_des_setkey,
			.encrypt		= na51000_crypto_des_ofb_encrypt,
			.decrypt		= na51000_crypto_des_ofb_decrypt,
		}
	},
	{
		.cra_name			= "ctr(des3_ede)",
		.cra_driver_name	= "na51000-ctr-des3",
		.cra_priority		= NA5100_CRYPTO_ALG_PRIORITY,
		.cra_flags			= CRYPTO_ALG_TYPE_ABLKCIPHER | CRYPTO_ALG_ASYNC | CRYPTO_ALG_KERN_DRIVER_ONLY,
		.cra_blocksize		= DES_BLOCK_SIZE,
		.cra_ctxsize		= sizeof(struct na51000_crypto_ctx),
		.cra_alignmask		= 0x3,
		.cra_type			= &crypto_ablkcipher_type,
		.cra_module			= THIS_MODULE,
		.cra_init			= na51000_crypto_cra_init,
		.cra_exit			= na51000_crypto_cra_exit,
		.cra_u.ablkcipher = {
			.min_keysize	= DES3_EDE_KEY_SIZE,
			.max_keysize	= DES3_EDE_KEY_SIZE,
			.ivsize		    = DES_BLOCK_SIZE,
			.setkey			= na51000_crypto_des_setkey,
			.encrypt		= na51000_crypto_des_ctr_encrypt,
			.decrypt		= na51000_crypto_des_ctr_decrypt,
		}
	}
};

static int na51000_crypto_probe(struct platform_device *pdev)
{
	int ret;
	struct resource           *res;
	struct na51000_crypto_dev *dd;

	dd = na51000_cdev = devm_kzalloc(&pdev->dev, sizeof(struct na51000_crypto_dev), GFP_KERNEL);
	if (!dd) {
		dev_err(&pdev->dev, "unable to alloc device data struct\n");
		return -ENOMEM;
	}

	dd->dev   = &pdev->dev;
	dd->irq   = -1;
	dd->busy  = false;

	platform_set_drvdata(pdev, dd);

	spin_lock_init(&dd->lock);

	crypto_init_queue(&dd->queue, NA5100_CRYPTO_QUEUE_LENGTH);

	res = platform_get_resource(pdev, IORESOURCE_MEM, 0);
	if (!res) {
		dev_err(&pdev->dev, "no MEM resource info\n");
		return -ENODEV;
	}

	dd->irq = platform_get_irq(pdev,  0);
	if (dd->irq < 0) {
		dev_err(&pdev->dev, "no IRQ resource info\n");
		return dd->irq;
	}

	dd->iobase = devm_ioremap_resource(&pdev->dev, res);
	if (IS_ERR(dd->iobase)) {
		dev_err(&pdev->dev, "can't ioremap\n");
		return PTR_ERR(dd->iobase);
	}

	ret = devm_request_irq(&pdev->dev, dd->irq, na51000_crypto_irq, 0, dev_name(&pdev->dev), dd);
	if (ret) {
		dev_err(&pdev->dev, "unable to request IRQ\n");
		return ret;
	}

	dd->clk = devm_clk_get(&pdev->dev, dev_name(&pdev->dev));
	if (IS_ERR(dd->clk)) {
		dev_err(&pdev->dev, "failed to find crypto hardware clock\n");
		return PTR_ERR(dd->clk);
	}

	/* init tasklet for device request */
	tasklet_init(&dd->queue_tasklet, na51000_crypto_queue_tasklet, (unsigned long)dd);
	tasklet_init(&dd->done_tasklet,  na51000_crypto_done_tasklet,  (unsigned long)dd);

	/* init timer for device timeout */
	init_timer(&dd->timer);
	dd->timer.function = na51000_crypto_timeout_handler;
	dd->timer.data     = (unsigned long)dd;

	/* enable crypto hardware clock */
	clk_prepare_enable(dd->clk);

	/* software reset crypto hardware */
	na51000_crypto_reset(dd);

	/* register crypto algorithm */
	ret = crypto_register_algs(na51000_crypto_algs, ARRAY_SIZE(na51000_crypto_algs));
	if (ret) {
		dev_err(&pdev->dev, "failed to register crypto algorithm\n");
		goto err;
	}

	pr_info("nvt-crypto driver registered (CLK: %luHz)\n", clk_get_rate(dd->clk));

	return 0;

err:
	del_timer(&dd->timer);
	tasklet_kill(&dd->queue_tasklet);
	tasklet_kill(&dd->done_tasklet);
	clk_disable_unprepare(dd->clk);
	return ret;
}

static int na51000_crypto_remove(struct platform_device *pdev)
{
	struct na51000_crypto_dev *dev = platform_get_drvdata(pdev);

	if (!dev)
		return -ENODEV;

	del_timer(&dev->timer);

	crypto_unregister_algs(na51000_crypto_algs, ARRAY_SIZE(na51000_crypto_algs));

	tasklet_kill(&dev->queue_tasklet);
	tasklet_kill(&dev->done_tasklet);

	clk_disable_unprepare(dev->clk);

	na51000_cdev = NULL;

	return 0;
}

#ifdef CONFIG_PM
static int na51000_crypto_suspend(struct device *dev)
{
	struct platform_device *pdev = to_platform_device(dev);
	struct na51000_crypto_dev *dd = platform_get_drvdata(pdev);

	/* disable crypto hardware clock */
	clk_disable(dd->clk);
	
	return 0;
}

static int na51000_crypto_resume(struct device *dev)
{
	struct platform_device *pdev = to_platform_device(dev);
	struct na51000_crypto_dev *dd = platform_get_drvdata(pdev);
	
	/* enable crypto hardware clock */
	return clk_enable(dd->clk);
}

static const struct dev_pm_ops na51000_crypto_pm = {
	.suspend        = na51000_crypto_suspend,
	.resume         = na51000_crypto_resume,
};

#define na51000_crypto_pm_ops (&na51000_crypto_pm)
#else
#define na51000_crypto_pm_ops NULL
#endif

#ifdef CONFIG_OF
static const struct of_device_id na51000_crypto_of_match[] = {
	{ .compatible = "nvt,nvt_crypto" },
	{},
};
MODULE_DEVICE_TABLE(of, na51000_crypto_of_match);
#else
#define na51000_crypto_of_match     NULL
#endif

static struct platform_driver na51000_crypto_driver = {
	.probe	= na51000_crypto_probe,
	.remove	= na51000_crypto_remove,
	.driver	= {
		.name	= "nvt_crypto",
		.of_match_table	= na51000_crypto_of_match,
		.pm	= na51000_crypto_pm_ops,
	},
};

module_platform_driver(na51000_crypto_driver);

MODULE_DESCRIPTION("Novatek crypto hardware acceleration support.");
MODULE_AUTHOR("Novatek");
MODULE_LICENSE("GPL");
MODULE_VERSION(DRV_VERSION);
