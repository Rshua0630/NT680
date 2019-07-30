/*
 * drivers/mtd/spiflash/spinand.c
 *
 * Copyright © 2005 Intel Corporation
 * Copyright © 2006 Marvell International Ltd.
 * Copyright © 2016 Novatek Microelectronics Corp.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */
#include <linux/module.h>
#include <linux/interrupt.h>

#include <linux/dma-mapping.h>
#include <linux/clk.h>
#include <linux/mtd/mtd.h>
#include <linux/platform_device.h>
#include <linux/mtd/partitions.h>
#include <linux/io.h>
#include <linux/irq.h>
#include <linux/slab.h>
#include <linux/sizes.h>

#include "../nvt_flash_spi/nvt_flash_spi_reg.h"
#include "../nvt_flash_spi/nvt_flash_spi_int.h"

#ifdef CONFIG_OF
#include <linux/of_device.h>
#endif

#if defined(CONFIG_NVT_IPC)
#include <mach/nvt-ipc.h>
#endif

#define CONFIG_BUF_PRE_FLUSH

#if defined(CONFIG_NVT_IPC)
static inline void spinand_sem_lock(void) {
	nvt_ipc_sem_lock(NVTIPC_SEMID_NAND);
}

static inline void spinand_sem_unlock(void) {
	nvt_ipc_sem_unlock(NVTIPC_SEMID_NAND);
}
#else
static inline void spinand_sem_lock(void) {
}

static inline void spinand_sem_unlock(void) {
}
#endif

/* error code and state */
enum {
	ERR_NONE	= 0,
	ERR_DMABUSERR	= -1,
	ERR_SENDCMD	= -2,
	ERR_DBERR	= -3,
	ERR_BBERR	= -4,
	ERR_ECC_FAIL	= -5,
	ERR_ECC_UNCLEAN = -6,
};

enum {
	STATE_READY = 0,
	STATE_CMD_HANDLE,
	STATE_DMA_READING,
	STATE_DMA_WRITING,
	STATE_DMA_DONE,
	STATE_PIO_READING,
	STATE_PIO_WRITING,
};

static struct nand_flash_dev spinand_flash_ids[] = {
/*
* Some incompatible NAND chips share device ID's and so must be
* listed by full ID. We list them first so that we can easily identify
* the most specific match.
*/
	// Micron
	{"MT29F1G01AAADD 1G 3.3V",
		{ .id = {MFG_ID_MICRON, 0x12} },
		SZ_2K, SZ_128, SZ_128K, 0, 2, 64 },
	// Winbond
	{"W25N01GV 1G 3.3V",
		{ .id = {MFG_ID_WINBOND, 0xAA, 0x21} },
		SZ_2K, SZ_128, SZ_128K, 0, 3, 64 },
	// ESMT
	{"F250L512M 512M 3.3V",
		{ .id = {MFG_ID_ESMT, 0x20} },
		SZ_2K, SZ_64, SZ_128K, 0, 2, 64 },
	// ESMT
	{"F250L512M 1GiB 3.3V",
		{ .id = {MFG_ID_ESMT, 0x01} },
		SZ_2K, SZ_128, SZ_128K, 0, 2, 128 },
	// GigaDevice
	{"GD5F1GQ4UBYIG 1GiB 3.3V",
		{ .id = {MFG_ID_GD, 0xD1} },
		SZ_2K, SZ_128, SZ_128K, 0, 2, 128 },
	// MXIC
	{"MX35LF1GE4AB 1GiB 3.3V",
		{ .id = {MFG_ID_MXIC, 0x12} },
		SZ_2K, SZ_128, SZ_128K, 0, 2, 64 },
	// ETRON
	{"EM73C044 1GiB 3.3V",
		{ .id = {MFG_ID_ETRON, 0x11} },
		SZ_2K, SZ_128, SZ_64K, 0, 2, 64 },
	// ATO
	{"ATO25D1GA 1GiB 3.3V",
		{ .id = {MFG_ID_ATO, 0x12} },
		SZ_2K, SZ_128, SZ_128K, 0, 2, 64 },
	// TOSHIBA
	{"TC58CVG0S3Hx 1GiB 3.3V",
		{ .id = {MFG_ID_TOSHIBA, 0xC2} },
		SZ_2K, SZ_128, SZ_128K, 0, 2, 64 },
};

#ifdef CONFIG_MTD_SPINAND_HWECC
static struct nand_ecclayout spinand_oob_64 = {
	.eccbytes = 32,
	.eccpos = {
		8, 9, 10, 11, 12, 13, 14, 15,
		24, 25, 26, 27, 28, 29, 30, 31,
		40, 41, 42, 43, 44, 45, 46, 47,
		56, 57, 58, 59, 60, 61, 62, 63},
	.oobavail = 12,
	.oobfree = {
		{.offset = 16,
			.length = 4},
		{.offset = 32,
			.length = 4},
		{.offset = 48,
			.length = 4},
	}
};

static struct nand_ecclayout spinand_oob_etron = {
        .eccbytes = 60,
        .eccpos = {
		4, 5, 6, 7,
                8, 9, 10, 11, 12, 13, 14, 15,
		16, 17, 18, 19, 20, 21, 22, 23,
                24, 25, 26, 27, 28, 29, 30, 31,
		32, 33, 34, 35, 36, 37, 38, 39,
                40, 41, 42, 43, 44, 45, 46, 47,
		48, 49, 50, 51, 52, 53, 54, 55,
                56, 57, 58, 59, 60, 61, 62, 63},
        .oobavail = 2,
        .oobfree = {
                {.offset = 2,
                        .length = 2},
        }
};

static struct nand_ecclayout spinand_oob_gd = {
#ifdef CONFIG_JFFS2_FS
	.eccbytes = 64,
	.eccpos = {
		64, 65, 66, 67, 68, 69, 70, 71,
		72, 73, 74, 75, 76, 77, 78, 79,
		80, 81, 82, 83, 84, 85, 86, 87,
		88, 89, 90, 91, 92, 93, 94, 95,
		96, 97, 98, 99, 100, 101, 102, 103,
		104, 105, 106, 107, 108, 109, 110, 111,
		112, 113, 114, 115, 116, 117, 118, 119,
		120, 121, 122, 123, 124, 125, 126, 127},
	.oobavail = 12,
	.oobfree = {
		{.offset = 16, .length = 4},
		{.offset = 32, .length = 4},
		{.offset = 48, .length = 4},
	}
#else
	.oobavail = 62,
	.oobfree = {
		{.offset = 2, .length = 62},
	}
#endif
};
#endif


static int use_dma = 1;

static int drv_nand_reset(struct drv_nand_dev_info *info, struct platform_device *pdev)
{
	uint32_t reg;

	info->clk = clk_get(&pdev->dev, dev_name(&pdev->dev));
	if (!IS_ERR(info->clk)) {
		clk_prepare(info->clk);
		clk_enable(info->clk);
	} else {
		pr_err("%s: %s not found\n", __func__, dev_name(&pdev->dev));
		return -ENXIO;
	}

	NAND_SETREG(info, NAND_TIME0_REG_OFS, 0x06002222);
	NAND_SETREG(info, NAND_TIME1_REG_OFS, 0x7f0f);

	clk_set_rate(info->clk, info->flash_freq);

	/* Release SRAM */
	reg = INW(0xFD020000 + 0x104);
	reg &= ~(0x1);
	OUTW(0xFD020000 + 0x104, reg);

	nand_host_set_nand_type(info, NANDCTRL_SPI_NAND_TYPE);

	nand_host_settiming2(info, 0x9F51);

	return nand_cmd_reset(info);
}

static int spinand_read_page_hwecc(struct mtd_info *mtd, struct nand_chip *chip,
				uint8_t *buf, int oob_required, int page)

{
	struct drv_nand_dev_info *info = mtd->priv;
	int eccsize = chip->ecc.size;
	int eccsteps = chip->ecc.steps;
	u8 status = 0, chip_id = info->flash_info->chip_id & 0xFF;
	int ret = 0;

	spinand_sem_lock();

	spiflash_enable_irq(info);

	chip->read_buf(mtd, buf, eccsize * eccsteps);
	if (oob_required)
		chip->read_buf(mtd, chip->oob_poi, mtd->oobsize);

	if (info->retcode == ERR_ECC_UNCLEAN) {
		mtd->ecc_stats.failed++;
	} else if (info->retcode == ECC_CORRECTED) {
		if (info->use_ecc == NANDCTRL_SPIFLASH_USE_INTERNAL_RS_ECC) {
				mtd->ecc_stats.corrected += \
					nand_cmd_read_ecc_corrected(info);

				ret = status;
		} else {
			if (chip_id == MFG_ID_WINBOND)
				ret = 1;

			if (chip_id == MFG_ID_MXIC) {
				ret = nand_cmd_read_flash_ecc_corrected(info);
				mtd->ecc_stats.corrected += ret;
			} else if (chip_id == MFG_ID_TOSHIBA) {
				status = nand_cmd_read_status(info, \
					NAND_SPI_STS_FEATURE_4);

				mtd->ecc_stats.corrected += status;
				ret = status;
			} else
				mtd->ecc_stats.corrected++;
		}
	}

	spiflash_disable_irq(info);

	spinand_sem_unlock();

	return ret;

}

static int spinand_write_page_hwecc(struct mtd_info *mtd, struct nand_chip *chip,
				  const uint8_t *buf, int oob_required)
{
	struct drv_nand_dev_info *info = mtd->priv;
	int eccsize = chip->ecc.size;
	int eccsteps = chip->ecc.steps;

	spinand_sem_lock();

	spiflash_enable_irq(info);

	chip->write_buf(mtd, buf, eccsize * eccsteps);

	spiflash_disable_irq(info);

	spinand_sem_unlock();

	return 0;
}


static int drv_nand_read_id(struct drv_nand_dev_info *info, uint32_t *id)
{
	uint8_t  card_id[4];
	uint8_t  id_count, i;
	memset(card_id, 0, sizeof(card_id));

	if (nand_cmd_read_id(card_id, info) != E_OK) {
		pr_err("NAND cmd timeout\r\n");
		return -EIO;
	} else {
		pr_debug("id =  0x%02x 0x%02x 0x%02x 0x%02x\n",
			card_id[0], card_id[1], card_id[2], card_id[3]);

		for (i = 0; i < sizeof(spinand_flash_ids); i++) {
			if ((card_id[0] == spinand_flash_ids[i].mfr_id) && \
			(card_id[1] == spinand_flash_ids[i].dev_id)) {
				id_count = spinand_flash_ids[i].id_len;
				memset(card_id + id_count, 0, 4 - id_count);
				break;
			}
		}
		*id = card_id[0] | (card_id[1] << 8) | (card_id[2] << 16) | \
			(card_id[3] << 24);

		return E_OK;
	}
}


static int drv_nand_readpage(struct drv_nand_dev_info *info,
				int column, int page_addr)
{
	return nand_cmd_read_operation(info, info->data_buff,
			page_addr * info->flash_info->page_size, 1);
}

static int drv_nand_write_page(struct drv_nand_dev_info *info,
				int column, int page_addr)
{
	if (column != info->flash_info->page_size)
		nand_cmd_write_spare_sram(info);

	return nand_cmd_write_operation_single(info, \
			page_addr * info->flash_info->page_size, column);
}

static irqreturn_t spinand_irq(int irq, void *devid)
{
	struct drv_nand_dev_info *info = devid;

	info->nand_int_status = NAND_GETREG(info, NAND_CTRL_STS_REG_OFS);

	if (info->nand_int_status) {
		NAND_SETREG(info, NAND_CTRL_STS_REG_OFS, info->nand_int_status);
		complete(&info->cmd_complete);
		return IRQ_HANDLED;
	} else
		return IRQ_NONE;


}

static inline int is_buf_blank(uint8_t *buf, size_t len)
{
	for (; len > 0; len--)
		if (*buf++ != 0xff)
			return 0;
	return 1;
}

static void spinand_cmdfunc(struct mtd_info *mtd, unsigned command,
		int column, int page_addr)
{
	struct drv_nand_dev_info *info = mtd->priv;
	int ret;
	uint8_t *ptr;

	info->use_dma = (use_dma) ? 1 : 0;
	info->data_size = 0;
	info->state = STATE_READY;

	spinand_sem_lock();

	spiflash_enable_irq(info);

	switch (command) {
	case NAND_CMD_READOOB:
		info->buf_count = mtd->writesize + mtd->oobsize;
		info->buf_start = mtd->writesize + column;

		ptr = info->data_buff + info->buf_start;
		if (info->buf_start != info->flash_info->page_size) {
			dev_err(&info->pdev->dev,
			"info->buf_start = %d, != 0\n", info->buf_start);
		}
		nand_cmd_read_page_spare_data(info, ptr,
				info->flash_info->page_size * page_addr);

		/* We only are OOB, so if the data has error, does not matter */
		if (info->retcode == ERR_ECC_FAIL)
			info->retcode = ERR_NONE;
	break;

	case NAND_CMD_READ0:
	case NAND_CMD_READ1:
		if (((unsigned long)(info->data_buff)) % CACHE_LINE_SIZE)
			dev_err(&info->pdev->dev,
				"NAND_CMD_READ0 : is not Cache_Line_Size alignment!\n");

		info->retcode = ERR_NONE;
		info->buf_start = column;
		info->buf_count = mtd->writesize + mtd->oobsize;
		ret = drv_nand_readpage(info, column, page_addr);
		if (ret == E_CTX)
			info->retcode = ERR_ECC_UNCLEAN;
		else if (ret < 0)
			info->retcode = ERR_DBERR;
		else if (ret == ECC_CORRECTED)
			info->retcode = ECC_CORRECTED;
		else
			info->retcode = ERR_NONE;
	break;

	case NAND_CMD_SEQIN:
		info->buf_start = column;
		info->buf_count = mtd->writesize + mtd->oobsize;
		memset(info->data_buff, 0xff, info->buf_count);

		/* save column/page_addr for next CMD_PAGEPROG */
		info->seqin_column = column;
		info->seqin_page_addr = page_addr;
	break;

	case NAND_CMD_PAGEPROG:
		if (((unsigned long)(info->data_buff)) % CACHE_LINE_SIZE)
			dev_err(&info->pdev->dev,
				"not CACHE_LINE_SIZE alignment!\n");

		drv_nand_write_page(info, info->seqin_column, info->seqin_page_addr);
	break;

	case NAND_CMD_ERASE1:
		nand_cmd_erase_block(info, page_addr);
	break;

	case NAND_CMD_ERASE2:
	break;

	case NAND_CMD_READID:
		info->buf_start = 0;
		info->buf_count = 16;
		if(info->flash_info->chip_id)
			memcpy((uint32_t *)info->data_buff, &info->flash_info->chip_id, 16);
		else
			drv_nand_read_id(info, (uint32_t *)info->data_buff);
	break;
	case NAND_CMD_STATUS:
		info->buf_start = 0;
		info->buf_count = 1;
		nand_cmd_read_status(info, NAND_SPI_STS_FEATURE_2);
		if (!(info->data_buff[0] & 0x80))
			info->data_buff[0] = 0x80;
	break;

	case NAND_CMD_RESET:
	break;

	default:
		pr_debug(KERN_ERR "non-supported command.\n");
	break;
	}

	if (info->retcode == ERR_DBERR) {
		pr_debug(KERN_ERR "double bit error @ page %08x\n", page_addr);
		info->retcode = ERR_NONE;
	}

	spiflash_disable_irq(info);

	spinand_sem_unlock();
}

static void spinand_select_chip(struct mtd_info *mtd, int dev)
{

}

static uint8_t spinand_read_byte(struct mtd_info *mtd)
{
	struct drv_nand_dev_info *info = mtd->priv;
	char retval = 0xFF;

	if (info->buf_start < info->buf_count)
		/* Has just send a new command? */
		retval = info->data_buff[info->buf_start++];

	return retval;
}

static void spinand_read_buf(struct mtd_info *mtd, uint8_t *buf, int len)
{
	struct drv_nand_dev_info *info = mtd->priv;

	memcpy(buf, info->data_buff + info->buf_start, len);

#ifdef CONFIG_BUF_PRE_FLUSH
	__cpuc_flush_dcache_area((void *)buf, (size_t)len);
#endif
	info->buf_start += len;
}

static void spinand_write_buf(struct mtd_info *mtd,
	const uint8_t *buf, int len)
{
	struct drv_nand_dev_info *info = mtd->priv;
	memcpy(info->data_buff + info->buf_start, buf, len);
#ifdef CONFIG_BUF_PRE_FLUSH
	__cpuc_flush_dcache_area((void *)info->data_buff + info->buf_start, (size_t)len);
#endif

	info->buf_start += len;
}


static int spinand_wait(struct mtd_info *mtd, struct nand_chip *this)
{
	return E_OK;
}

#ifdef CONFIG_MTD_SPINAND_HWECC
static void drv_nand_ecc_hwctl(struct mtd_info *mtd, int mode)
{
	struct drv_nand_dev_info *info = mtd->priv;
	dev_info(&info->pdev->dev, "%s retcode\n", __func__);
	return;
}

static int drv_nand_ecc_calculate(struct mtd_info *mtd,
	const uint8_t *dat, uint8_t *ecc_code)
{
	struct drv_nand_dev_info *info = mtd->priv;
	dev_info(&info->pdev->dev, "%s retcode\n", __func__);
	return 0;
}

static int drv_nand_ecc_correct(struct mtd_info *mtd,
	uint8_t *dat, uint8_t *read_ecc, uint8_t *calc_ecc)
{
	struct drv_nand_dev_info *info = mtd->priv;
	dev_info(&info->pdev->dev, "%s retcode\n", __func__);
	return 0;
}
#endif

static int drv_nand_identify(struct drv_nand_dev_info *info)
{
	int maf_id, dev_id;
	uint32_t id;
	uint32_t reg;
	u8 status;

	if(drv_nand_read_id(info, &id) != E_OK)
		return -EIO;

	maf_id = id & 0xFF;
	dev_id = (id >> 8) & 0xFF;

	info->flash_info->page_per_block = 64;
	info->flash_info->page_size = 2048;
	info->flash_info->oob_size = 64;
	info->flash_info->subpage_size = 512;
	info->flash_info->flash_width = 8;
	info->flash_info->nfc_width = 8;
	info->flash_info->num_blocks = 2048;
	info->flash_info->chip_id = id;
	info->flash_info->phy_page_ratio= 4;
	info->flash_info->device_size = 128;
	info->flash_info->chip_sel = 0;
	info->flash_info->block_size = 0x20000;
	info->flash_info->module_config = NAND_PAGE2K | NAND_2COL_ADDR;

	status = nand_cmd_read_status(info, NAND_SPI_STS_FEATURE_2);

	if (status & SPINAND_CONFIG_ECCEN) {
		info->use_ecc = NANDCTRL_SPIFLASH_USE_ONDIE_ECC;
		info->flash_info->module_config |= NAND_PRI_ECC_SPI_ON_DIE;
		NAND_SETREG(info, NAND_INTMASK_REG_OFS, NAND_COMP_INTREN | \
			NAND_STS_FAIL_INTREN | NAND_TOUT_INTREN);
	} else {
		info->use_ecc = NANDCTRL_SPIFLASH_USE_INTERNAL_RS_ECC;
		info->flash_info->module_config |= NAND_PRI_ECC_RS;
		NAND_SETREG(info, NAND_INTMASK_REG_OFS, NAND_COMP_INTREN | \
			NAND_PRI_ECC_INTREN | NAND_STS_FAIL_INTREN | NAND_TOUT_INTREN);
	}

	nand_host_setup_page_size(info, NAND_PAGE_SIZE_2048);

	reg = NAND_GETREG(info, NAND_MODULE0_REG_OFS);
	reg &= ~(0x7FFFF);
	NAND_SETREG(info, NAND_MODULE0_REG_OFS, reg | info->flash_info->module_config);
	return 0;
}


/* the maximum possible buffer size for large page with OOB data
 * is: 2048 + 64 = 2112 bytes, allocate a page here for both the
 * data buffer and the DMA descriptor
 */
#define MAX_BUFF_SIZE	PAGE_SIZE

static int drv_nand_init_buff(struct drv_nand_dev_info *info)
{
	struct platform_device *pdev = info->pdev;

	if (MAX_BUFF_SIZE >= (2048 + 64)) {
		info->data_buff = kzalloc(MAX_BUFF_SIZE, GFP_KERNEL);

		if (info->data_buff == NULL) {
			dev_err(&pdev->dev, "failed to allocate dma buffer\n");
			kfree(info->data_buff);
			return -ENOMEM;
		}
		info->data_buff_size = MAX_BUFF_SIZE;
	} else {
		info->data_buff = kzalloc((2048+64), GFP_KERNEL);

		if (info->data_buff == NULL) {
			dev_err(&pdev->dev, "failed to allocate dma buffer\n");
			kfree(info->data_buff);
			return -ENOMEM;
		}
		info->data_buff_size = (2048+64);
	}

	if ((unsigned long)info->data_buff % CACHE_LINE_SIZE)
		info->data_buff = (uint8_t *)((((unsigned long)info->data_buff + CACHE_LINE_SIZE - 1)) & 0xFFFFFFC0);

	return E_OK;
}

static void spinand_init_mtd(struct mtd_info *mtd)
{
	struct drv_nand_dev_info *info = mtd->priv;
	struct nand_chip *chip = &info->nand_chip;
#ifdef CONFIG_MTD_SPINAND_HWECC
	const struct nvt_nand_flash *f = info->flash_info;
	u8 chip_id = f->chip_id & 0xFF;
	chip->ecc.mode	= NAND_ECC_HW;
	chip->ecc.size	= 0x200;
	chip->ecc.bytes	= 0x8;
	chip->ecc.steps	= 0x4;

	chip->ecc.strength = 1;
	chip->ecc.total	= chip->ecc.steps * chip->ecc.bytes;
	if (chip_id == MFG_ID_ETRON)
		chip->ecc.layout = &spinand_oob_etron;
	else if (chip_id == MFG_ID_GD)
		chip->ecc.layout = &spinand_oob_gd;
	else
		chip->ecc.layout = &spinand_oob_64;
	chip->ecc.read_page = spinand_read_page_hwecc;
	chip->ecc.write_page = spinand_write_page_hwecc;

	chip->ecc.hwctl = drv_nand_ecc_hwctl;
	chip->ecc.calculate = drv_nand_ecc_calculate;
	chip->ecc.correct = drv_nand_ecc_correct;
	chip->options |= NAND_NO_SUBPAGE_WRITE;
#else
	chip->ecc.mode	= NAND_ECC_SOFT;
	if (spi_nand_unlock_bp(info) < 0)
		pr_err("%s: disable ecc failed!\n", __func__);
#endif

	chip->priv	= info;
	chip->read_buf	= spinand_read_buf;
	chip->write_buf	= spinand_write_buf;
	chip->read_byte	= spinand_read_byte;
	chip->cmdfunc	= spinand_cmdfunc;
	chip->waitfunc	= spinand_wait;
	chip->select_chip = spinand_select_chip;
	info->flash_info->spi_nand_status.block_unlocked = FALSE;
	info->flash_info->spi_nand_status.quad_en = FALSE;
	info->flash_info->spi_nand_status.quad_program = FALSE;
	info->flash_info->spi_nand_status.timer_record = FALSE;
}

static int spinand_probe(struct platform_device *pdev)
{
	struct drv_nand_dev_info *info;
	struct mtd_part_parser_data ppdata;
	struct platform_nand_data *pplatdata;
	struct nand_chip *this;
	struct mtd_info *mtd;
	struct resource *res;
	int ret = 0;
	u8 chip_id;

	mtd = kmalloc(sizeof(struct mtd_info) +
		sizeof(struct drv_nand_dev_info), GFP_KERNEL | __GFP_ZERO);
	if (!mtd) {
		dev_err(&pdev->dev, "failed to allocate memory\n");
		return -ENOMEM;
	}
	info = (struct drv_nand_dev_info *)(&mtd[1]);
	info->pdev = pdev;

	info->flash_info = kmalloc(sizeof(struct nvt_nand_flash), GFP_KERNEL);
	if (!info->flash_info) {
		dev_err(&pdev->dev, "failed to allocate nvt_nand_flash\n");
		return -ENOMEM;
	}

	pplatdata = NULL;
	if (pdev->dev.platform_data != NULL)
	{
		pplatdata = pdev->dev.platform_data;
	}

	this = &info->nand_chip;
	mtd->priv = info;

	/* Get mtd device resource of base address */
	res = platform_get_resource(pdev, IORESOURCE_MEM, 0);

	if (!res) {
		dev_err(&pdev->dev, "failed to get resource\n");
		goto fail_free_mtd;
	}

	info->mmio_base = devm_ioremap_resource(&pdev->dev, res);
	if (IS_ERR(info->mmio_base)) {
		PTR_ERR(info->mmio_base);
		goto fail_free_mtd;
	}

	info->irq = platform_get_irq(pdev, 0);
	if (info->irq < 0) {
		dev_err(&pdev->dev, "no IRQ resource defined\n");
		ret = -ENXIO;
		goto fail_free_mtd;
	}

	ret = drv_nand_init_buff(info);
	if (ret) {
		dev_err(&pdev->dev, "failed to allocate buffer\n");
		goto fail_free_buf;
	}

	ret = request_irq(info->irq, spinand_irq, IRQF_SHARED, pdev->name, info);
	if (ret < 0) {
		dev_err(&pdev->dev, "failed to request IRQ\n");
		goto fail_free_buf;
	}

	of_property_read_u32(pdev->dev.of_node, "clock-frequency",
				&info->flash_freq);

	spin_lock_init(&info->intc_lock);

	init_completion(&info->cmd_complete);

	spinand_sem_lock();

	spiflash_enable_irq(info);

	if (!spiflash_boot_detect(info)) {
		drv_nand_reset(info, pdev);
		nand_dll_reset(info);
	}

	/*Delay 1ms for spinand characteristic*/
	mdelay(1);

	ret = drv_nand_identify(info);

	spiflash_disable_irq(info);

	spinand_sem_unlock();

	if (ret) {
		dev_err(&pdev->dev, "failed to identify flash\n");
		ret = -ENODEV;
		goto fail_free_irq;
	}

	spinand_init_mtd(mtd);
	mtd->priv = this;
#ifdef CONFIG_OF
	of_property_read_string_index(pdev->dev.of_node, "nvt-devname", 0, &mtd->name);
#else
	mtd->name = dev_name(&pdev->dev);
#endif
	mtd->owner = THIS_MODULE;
	mtd->oobsize = 64;

	chip_id = info->flash_info->chip_id & 0xFF;
	if (info->use_ecc == NANDCTRL_SPIFLASH_USE_INTERNAL_RS_ECC) {
			info->nand_chip.ecc.strength = 4;
			mtd->bitflip_threshold = 2;
	} else {
		if (chip_id == MFG_ID_WINBOND)
			mtd->bitflip_threshold = 1;
		else if (chip_id == MFG_ID_MXIC) {
			info->nand_chip.ecc.strength = 4;
			mtd->bitflip_threshold = 3;
		} else if (chip_id == MFG_ID_TOSHIBA) {
			info->nand_chip.ecc.strength = 8;
			mtd->bitflip_threshold = 6;
		} else
			mtd->bitflip_threshold = 8;
	}

	platform_set_drvdata(pdev, mtd);
	/* Scan for card properties */

	if (info->flash_info->chip_id != TOSHIBA_TC58CVG)
		info->flash_info->spi_nand_status.quad_program = TRUE;

	ret = nand_scan_ident(mtd, 1, spinand_flash_ids);
	if (ret) {
		dev_err(&pdev->dev, "Identify nand fail\n");
		goto fail_free_irq;
	}

	ret = nand_scan_tail(mtd);
	if (ret) {
		dev_err(&pdev->dev, "nand scan tail fail\n");
		goto fail_free_irq;
	}

	ppdata.of_node = pdev->dev.of_node;
	if (pplatdata == NULL)
		return mtd_device_parse_register(mtd, NULL, &ppdata, NULL, 0);
	else
		return mtd_device_parse_register(mtd, NULL, &ppdata,
				pplatdata->chip.partitions,
				pplatdata->chip.nr_partitions);

fail_free_irq:
	free_irq(info->irq, info);

fail_free_buf:
	kfree(info->data_buff);

fail_free_mtd:
	kfree(mtd);
	return ret;
}

static int spinand_remove(struct platform_device *pdev)
{
	struct mtd_info *mtd = platform_get_drvdata(pdev);
	struct drv_nand_dev_info *info = mtd->priv;

	platform_set_drvdata(pdev, NULL);

	nand_release(mtd);

	free_irq(info->irq, info);

	kfree(info->data_buff);

	kfree(mtd);
	return E_OK;
}

#ifdef CONFIG_PM
static int spinand_suspend(struct device *dev)
{
	struct platform_device *pdev = to_platform_device(dev);
	struct mtd_info *mtd = platform_get_drvdata(pdev);
	struct drv_nand_dev_info *info = mtd->priv;

	spinand_sem_lock();

	spiflash_disable_irq(info);

	if (!IS_ERR(info->clk))
		clk_disable(info->clk);

	spinand_sem_unlock();

	return 0;
}
static int spinand_resume(struct device *dev)
{
	struct platform_device *pdev = to_platform_device(dev);
	struct mtd_info *mtd = platform_get_drvdata(pdev);
	struct drv_nand_dev_info *info = mtd->priv;

	clk_enable(info->clk);

	spinand_sem_lock();

	spiflash_enable_irq(info);

	mdelay(10);
	if (!spiflash_boot_detect(info)) {
		drv_nand_reset(info, pdev);
		nand_dll_reset(info);
	} else {
		NAND_SETREG(info, NAND_TIME1_REG_OFS, 0x7f0f);
		nand_host_set_nand_type(info, NANDCTRL_SPI_NAND_TYPE);
	}

	mdelay(10);

	spiflash_disable_irq(info);

	spinand_sem_unlock();

	return 0;
}
static const struct dev_pm_ops spinand_pmops = {
	.suspend	= spinand_suspend,
	.resume		= spinand_resume,
};
#define SPINAND_PMOPS &spinand_pmops
#else
#define SPINAND_PMOPS NULL
#endif /* CONFIG_PM */

#ifdef CONFIG_OF
static const struct of_device_id nvt_spinand_of_dt_ids[] = {
    { .compatible = "nvt,nvt_spinand" },
    {},
};

MODULE_DEVICE_TABLE(of, nvt_spinand_of_dt_ids);
#endif

static struct platform_driver nvt_nand_driver = {
	.driver = {
		.name = "spi_nand",
		.owner = THIS_MODULE,
		.pm = SPINAND_PMOPS,
#ifdef CONFIG_OF
		.of_match_table = nvt_spinand_of_dt_ids,
#endif
	},
	.probe	    = spinand_probe,
	.remove     = spinand_remove,
};

static int __init spinand_init(void)
{
	return platform_driver_register(&nvt_nand_driver);
}

static void __exit spinand_exit(void)
{
	platform_driver_unregister(&nvt_nand_driver);
}

module_init(spinand_init);
module_exit(spinand_exit);

MODULE_DESCRIPTION("SPI NAND driver");
MODULE_AUTHOR("Howard Chang");
MODULE_LICENSE("GPL v2");
MODULE_VERSION("1.03.600");

