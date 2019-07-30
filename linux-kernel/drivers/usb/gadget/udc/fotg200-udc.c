/*
 * FOTG200 UDC Driver supports Bulk transfer so far
 *
 * Copyright (C) 2017 Novatek Technology Corporation
 *
 * Author : Klins Chen <klins_chen@novatek.com.tw>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; version 2 of the License.
 */

#include <linux/dma-mapping.h>
#include <linux/err.h>
#include <linux/interrupt.h>
#include <linux/io.h>
#include <linux/module.h>
#include <linux/platform_device.h>
#include <linux/usb/ch9.h>
#include <linux/usb/gadget.h>
#include <linux/delay.h>
#include <linux/of.h>

#include "fotg200.h"

#define	DRIVER_DESC	"NOVATEK iVot USB Device FOTG200 Controller Driver"
#define	DRIVER_VERSION	"16-August-2017"

static const char udc_name[] = "nvt,fotg200_udc";
static const char * const fotg200_ep_name[] = {
	"ep0", "ep1", "ep2", "ep3", "ep4"};


#define	NVTUSB_DEBUG	0

#if NVTUSB_DEBUG
#define numsg		printk
#define itfnumsg	printk
#define devnumsg	printk
#define ep0numsg	printk
#else
#define numsg		pr_debug
#define itfnumsg	pr_debug
#define devnumsg	pr_debug
#define ep0numsg	pr_debug
#endif


static void fotg200_disable_fifo_int(struct fotg200_ep *ep)
{
	u32 value;

	value = ioread32(ep->fotg200->reg + FOTG210_DMISGR1);
	if (ep->dir_in)
		value |= DMISGR1_MF_IN_INT(ep->epnum - 1);
	else
		value |= DMISGR1_MF_OUTSPK_INT(ep->epnum - 1);
	iowrite32(value, ep->fotg200->reg + FOTG210_DMISGR1);
}

static void fotg200_enable_fifo_int(struct fotg200_ep *ep)
{
	u32 value = ioread32(ep->fotg200->reg + FOTG210_DMISGR1);

	if (ep->dir_in)
		value &= ~DMISGR1_MF_IN_INT(ep->epnum - 1);
	else
		value &= ~DMISGR1_MF_OUTSPK_INT(ep->epnum - 1);
	iowrite32(value, ep->fotg200->reg + FOTG210_DMISGR1);

	numsg("fotg200_enable_fifo_int epnum=%d reg=0x%08X\n",ep->epnum,value);
}

static void fotg200_set_cxdone(struct fotg200_udc *fotg200)
{
	u32 value = ioread32(fotg200->reg + FOTG210_DCFESR);

	value |= DCFESR_CX_DONE;
	iowrite32(value, fotg200->reg + FOTG210_DCFESR);
}

static void fotg200_done(struct fotg200_ep *ep, struct fotg200_request *req,
			int status)
{
	numsg("fotg200_done 1\n");

	list_del_init(&req->queue);

	/* don't modify queue heads during completion callback */
	if (ep->fotg200->gadget.speed == USB_SPEED_UNKNOWN)
		req->req.status = -ESHUTDOWN;
	else
		req->req.status = status;

	if(req->req.complete != NULL) {
		spin_unlock(&ep->fotg200->lock);
		req->req.complete(&ep->ep, &req->req);
		spin_lock(&ep->fotg200->lock);
	}

	if (ep->epnum) {
		if (list_empty(&ep->queue))
			fotg200_disable_fifo_int(ep);
	} else {
		fotg200_set_cxdone(ep->fotg200);
	}

}

static void fotg200_fifo_ep_mapping(struct fotg200_ep *ep, u32 epnum,
				u32 dir_in)
{
	struct fotg200_udc *fotg200 = ep->fotg200;
	u32 val;

	/* Driver should map an ep to a fifo and then map the fifo
	 * to the ep. What a brain-damaged design!
	 */

	/* map a fifo to an ep */
	val = ioread32(fotg200->reg + FOTG210_EPMAP(epnum-1));
	val &= ~ EPMAP_FIFONOMSK(epnum, dir_in);
	val |= EPMAP_FIFONO(epnum, dir_in);
	if(!dir_in)
		val |= EPMAP_DIROUT;
	iowrite32(val, fotg200->reg + FOTG210_EPMAP(epnum-1));

	/* map the ep to the fifo */
	val = ioread32(fotg200->reg + FOTG210_FIFOMAP(epnum-1));
	//val &= ~ FIFOMAP_EPNOMSK(epnum);
	//val |= FIFOMAP_EPNO(epnum);
	val |= FIFOCF_FIFO_EN(epnum - 1);
	iowrite32(val, fotg200->reg + FOTG210_FIFOMAP(epnum-1));

}

static void fotg200_set_fifo_dir(struct fotg200_ep *ep, u32 epnum, u32 dir_in,u32 type, u32 mps)
{
	struct fotg200_udc *fotg200 = ep->fotg200;
	u32 val;

	val = ioread32(fotg200->reg + FOTG210_FIFOMAP(epnum-1));
	val |= (dir_in ? FIFOMAP_DIRIN(epnum - 1) : FIFOMAP_DIROUT(epnum - 1));
	if((type == USB_ENDPOINT_XFER_BULK))
		val |= FIFOMAP_DOUBLEBLK(epnum - 1);
	else {
		val |= FIFOMAP_SINGLEBLK(epnum - 1);
		if(mps > 512) {
			val |= FIFOCF_BLKSZ_1024(epnum - 1);
		}
	}
	iowrite32(val, fotg200->reg + FOTG210_FIFOMAP(epnum-1));
}

static void fotg200_set_tfrtype(struct fotg200_ep *ep, u32 epnum, u32 type)
{
	struct fotg200_udc *fotg200 = ep->fotg200;
	u32 val;

	val = ioread32(fotg200->reg + FOTG210_FIFOMAP(epnum-1));
	val |= FIFOCF_TYPE(type, epnum - 1);
	iowrite32(val, fotg200->reg + FOTG210_FIFOMAP(epnum-1));
}

static void fotg200_set_mps(struct fotg200_ep *ep, u32 epnum, u32 mps,
				u32 dir_in)
{
	struct fotg200_udc *fotg200 = ep->fotg200;
	u32 val;
	u32 offset = dir_in ? FOTG210_INEPMPSR(epnum) :
				FOTG210_OUTEPMPSR(epnum);

	val = ioread32(fotg200->reg + offset);
	val &= ~INOUTEPMPSR_MPS(0x7FF);
	val |= INOUTEPMPSR_MPS(mps);

	// fixed HBW = 1
	val &= ~(0x3<<13);
	if(ep->type == USB_ENDPOINT_XFER_ISOC) {
		val |= (0x1<<13);
	}
	iowrite32(val, fotg200->reg + offset);

}

static int fotg200_config_ep(struct fotg200_ep *ep,
		     const struct usb_endpoint_descriptor *desc)
{
	struct fotg200_udc *fotg200 = ep->fotg200;

	fotg200_set_fifo_dir(ep, ep->epnum, ep->dir_in, ep->type, ep->ep.maxpacket);
	fotg200_set_tfrtype(ep, ep->epnum, ep->type);
	fotg200_set_mps(ep, ep->epnum, ep->ep.maxpacket, ep->dir_in);
	fotg200_fifo_ep_mapping(ep, ep->epnum, ep->dir_in);

	fotg200->ep[ep->epnum] = ep;

	return 0;
}

static int fotg200_ep_enable(struct usb_ep *_ep,
			  const struct usb_endpoint_descriptor *desc)
{
	struct fotg200_ep *ep;


	ep = container_of(_ep, struct fotg200_ep, ep);

	ep->desc = desc;
	ep->epnum = usb_endpoint_num(desc);
	ep->type = usb_endpoint_type(desc);
	ep->dir_in = usb_endpoint_dir_in(desc);
	ep->ep.maxpacket = usb_endpoint_maxp(desc);

	itfnumsg("fotg200_ep_enable\n");
	itfnumsg("ep-desc len=0x%X type=0x%X epaddr=0x%X attr=0x%X MaxPkt=0x%X intval=0x%X\n",desc->bLength,desc->bDescriptorType
		,desc->bEndpointAddress,desc->bmAttributes,desc->wMaxPacketSize,desc->bInterval);

	return fotg200_config_ep(ep, desc);
}

static void fotg200_reset_tseq(struct fotg200_udc *fotg200, u8 epnum)
{
	struct fotg200_ep *ep = fotg200->ep[epnum];
	u32 value;
	void __iomem *reg;

	reg = (ep->dir_in) ?
		fotg200->reg + FOTG210_INEPMPSR(epnum) :
		fotg200->reg + FOTG210_OUTEPMPSR(epnum);

	/* Note: Driver needs to set and clear INOUTEPMPSR_RESET_TSEQ
	 *	 bit. Controller wouldn't clear this bit. WTF!!!
	 */

	value = ioread32(reg);
	value |= INOUTEPMPSR_RESET_TSEQ;
	iowrite32(value, reg);

	value = ioread32(reg);
	value &= ~INOUTEPMPSR_RESET_TSEQ;
	iowrite32(value, reg);
}

static int fotg200_ep_release(struct fotg200_ep *ep)
{
	if (!ep->epnum)
		return 0;
	ep->epnum = 0;
	ep->stall = 0;
	ep->wedged = 0;

	fotg200_reset_tseq(ep->fotg200, ep->epnum);

	return 0;
}

static int fotg200_ep_disable(struct usb_ep *_ep)
{
	struct fotg200_ep *ep;
	struct fotg200_request *req;
	unsigned long flags;

	BUG_ON(!_ep);

	itfnumsg("fotg200_ep_disable\n");

	ep = container_of(_ep, struct fotg200_ep, ep);

	while (!list_empty(&ep->queue)) {
		req = list_entry(ep->queue.next,
			struct fotg200_request, queue);
		spin_lock_irqsave(&ep->fotg200->lock, flags);
		fotg200_done(ep, req, -ECONNRESET);
		spin_unlock_irqrestore(&ep->fotg200->lock, flags);
	}

	return fotg200_ep_release(ep);
}

static struct usb_request *fotg200_ep_alloc_request(struct usb_ep *_ep,
						gfp_t gfp_flags)
{
	struct fotg200_request *req;

	itfnumsg("fotg200_ep_alloc_request\n");

	req = kzalloc(sizeof(struct fotg200_request), gfp_flags);
	if (!req)
		return NULL;

	INIT_LIST_HEAD(&req->queue);

	return &req->req;
}

static void fotg200_ep_free_request(struct usb_ep *_ep,
					struct usb_request *_req)
{
	struct fotg200_request *req;

	itfnumsg("fotg200_ep_free_request\n");

	req = container_of(_req, struct fotg200_request, req);
	kfree(req);
}

static void fotg200_enable_dma(struct fotg200_ep *ep,
			      dma_addr_t d, u32 len)
{
	u32 value;
	struct fotg200_udc *fotg200 = ep->fotg200;

	if(!ep->epnum)
		pr_err("ep0 has no dma. shall not enter fotg200_enable_dma!!\n");

	do {
		value = ioread32(fotg200->reg + FOTG210_DMACHOFS(ep->epnum));
	}while(value & DMACPSR1_DMA_START);

	/* set transfer length and direction */
	value = ioread32(fotg200->reg + FOTG210_DMACHOFS(ep->epnum));
	value &= ~(DMACPSR1_DMA_LEN(0x7FFFFF) | DMACPSR1_DMA_TYPE(1));
	value |= DMACPSR1_DMA_LEN(len) | DMACPSR1_DMA_TYPE(ep->dir_in);
	iowrite32(value, fotg200->reg + FOTG210_DMACHOFS(ep->epnum));

	/* set device DMA target FIFO number */
	//iowrite32(((ep->epnum - 1)<<1), fotg210->reg + FOTG210_DMATFNR);

	/* set DMA memory address */
	iowrite32(d, fotg200->reg + FOTG210_DMAADROFS(ep->epnum));

	/* enable MDMA_EROR and MDMA_CMPLT interrupt */
	value = ioread32(fotg200->reg + FOTG210_DMISGR2);
	value |= DMISGR2_MDMA_CMPLTALL;
	iowrite32(value, fotg200->reg + FOTG210_DMISGR2);

	/* start DMA */
	value = ioread32(fotg200->reg + FOTG210_DMACHOFS(ep->epnum));
	value |= DMACPSR1_DMA_START;
	iowrite32(value, fotg200->reg + FOTG210_DMACHOFS(ep->epnum));
}

static void fotg200_disable_dma(struct fotg200_ep *ep)
{
	//iowrite32(DMATFNR_DISDMA, ep->fotg210->reg + FOTG210_DMATFNR);
}

static void fotg200_wait_dma_done(struct fotg200_ep *ep)
{
	u32 value;
	unsigned long timeout = 0;

	timeout = jiffies + msecs_to_jiffies(500);
	do {
		value = ioread32(ep->fotg200->reg + FOTG210_DISGR2);
		if ((value & DISGR2_USBRST_INT))
			goto dma_reset;

        if (time_after(jiffies, timeout)) {
               //pr_err("%s timeout\n", __func__);
               goto dma_reset;
        }
        cpu_relax();

		//udelay(5);
	} while (!(value & (DISGR2_DMA_CMPLT<<((ep->epnum-1)<<1))));

	value |= (DISGR2_DMA_CMPLT<<((ep->epnum-1)<<1));
	iowrite32(value, ep->fotg200->reg + FOTG210_DISGR2);

	if ((ep->epnum)&&(ep->dir_in)) {
		do {
			value = ioread32(ep->fotg200->reg + FOTG210_DCFESR);
		} while (!(value & DCFESR_FIFO_EMPTY(ep->epnum - 1)));
	}

	return;

dma_reset:

	value = ioread32(ep->fotg200->reg + FOTG210_DMACHOFS(ep->epnum));
	value |= DMACPSR1_DMA_ABORT;
	iowrite32(value, ep->fotg200->reg + FOTG210_DMACHOFS(ep->epnum));

	pr_err("fotg210_wait_dma_done dma_abort\n");

	/* reset fifo */
	if (ep->epnum) {
		value = ioread32(ep->fotg200->reg +
				FOTG210_FIBCR(ep->epnum - 1));
		value = ((ep->epnum - 1)&0x1) ? value|(FIBCR_FFRST<<16) : value|FIBCR_FFRST;
		iowrite32(value, ep->fotg200->reg +
				FOTG210_FIBCR(ep->epnum - 1));
	} else {
		value = ioread32(ep->fotg200->reg + FOTG210_DCFESR);
		value |= DCFESR_CX_CLR;
		iowrite32(value, ep->fotg200->reg + FOTG210_DCFESR);
	}

	value = ioread32(ep->fotg200->reg + FOTG210_DMACHOFS(ep->epnum));
	value &= ~DMACPSR1_DMA_ABORT;
	iowrite32(value, ep->fotg200->reg + FOTG210_DMACHOFS(ep->epnum));

}

static void fotg200_start_ep0_data(struct fotg200_ep *ep,
			struct fotg200_request *req)
{
	u32 *buffer;
	u32 value,length,i=0;
	s32	opsize;

	buffer = (u32 *)(req->req.buf + req->req.actual);

	if (req->req.length - req->req.actual > 64)  {
		length = 64;

		if (ep->dir_in) {
			value = ioread32(ep->fotg200->reg +
						FOTG210_DMISGR0);
			value &= ~DMISGR0_MCX_IN_INT;
			iowrite32(value, ep->fotg200->reg + FOTG210_DMISGR0);
		}

	} else {
		length = req->req.length - req->req.actual;

		if (ep->dir_in) {
			value = ioread32(ep->fotg200->reg +
						FOTG210_DMISGR0);
			value |= DMISGR0_MCX_IN_INT;
			iowrite32(value, ep->fotg200->reg + FOTG210_DMISGR0);
		}
	}

	value = ioread32(ep->fotg200->reg + FOTG210_DCFESR);
	if(value & DCFESR_CX_DATAPORT_EN)
		pr_err("DATAPORT EN ERROR!!!\n");

	if (ep->dir_in) {
		ep0numsg("fotg200_start_ep0_data IN 0x%X  act=0x%X len=%d\n",req->req.length,req->req.actual,length);

		value = ioread32(ep->fotg200->reg + FOTG210_DCFESR);
		value &= ~DCFESR_CX_FNT_IN;
		value |= (length<<16);
		value |= DCFESR_CX_DATAPORT_EN;
		iowrite32(value, ep->fotg200->reg + FOTG210_DCFESR);


		opsize = length;
		while(opsize>0)
		{
			iowrite32(buffer[i++], ep->fotg200->reg + FOTG210_CXDATAPORT);
			opsize-=4;
		}
	} else {
		ep0numsg("fotg200_start_ep0_data OUT 0x%X  act=0x%X\n",req->req.length,req->req.actual);

		value = ioread32(ep->fotg200->reg + FOTG210_DCFESR);
		value |= DCFESR_CX_DATAPORT_EN;
		iowrite32(value, ep->fotg200->reg + FOTG210_DCFESR);

		opsize = (ioread32(ep->fotg200->reg + FOTG210_DCFESR) & DCFESR_CX_FNT_OUT)>>24;
		while(!opsize) {
			msleep(1);
			opsize = (ioread32(ep->fotg200->reg + FOTG210_DCFESR) & DCFESR_CX_FNT_OUT)>>24;
		}

		while(opsize>0)
		{
			value = ioread32(ep->fotg200->reg + FOTG210_CXDATAPORT);

			if(opsize>=4) {
				buffer[i++] = value;
			} else if (opsize==3) {
				buffer[i] &= ~0xFFFFFF;
				value &= 0xFFFFFF;
				buffer[i] += value;
			} else if (opsize==2) {
				buffer[i] &= ~0xFFFF;
				value &= 0xFFFF;
				buffer[i] += value;
			} else if (opsize==1) {
				buffer[i] &= ~0xFF;
				value &= 0xFF;
				buffer[i] += value;
			}
			opsize-=4;
		}


	}

	value = ioread32(ep->fotg200->reg + FOTG210_DCFESR);
	value &= ~DCFESR_CX_DATAPORT_EN;
	iowrite32(value, ep->fotg200->reg + FOTG210_DCFESR);

	/* update actual transfer length */
	req->req.actual += length;

}


static void fotg200_start_dma(struct fotg200_ep *ep,
			struct fotg200_request *req)
{
	dma_addr_t d;
	u8 *buffer;
	u32 length,slice,szOp;

	if (ep->epnum) {
		if (ep->dir_in) {
			buffer = req->req.buf + req->req.actual;
			length = req->req.length - req->req.actual;

			//if(length>4096)
			//	length=4096;

		} else {
			buffer = req->req.buf + req->req.actual;
			length = ioread32(ep->fotg200->reg +
					FOTG210_FIBCR(ep->epnum - 1));
			//length = ((ep->epnum - 1)&0x1) ? length>>16 : length;
			length &= FIBCR_BCFX;

			if(length < 512)
				length = length;//short packet
			else
				length = req->req.length - req->req.actual;

			//if(length>1536)
			//	length=1536;
		}
	} else {
		pr_err("ep0 has no dma. shall not enter enable_dma!!\n");
		buffer = 0;
		length = 0;
	}

	numsg("fotg200_start_dma %d ",length);

	szOp = 0;

	while(szOp<length) {

		//if((length - szOp) > 4096)
		//	slice = 4096;
		//else
			slice = (length - szOp);

		d = dma_map_single(NULL, (u8 *)(buffer+szOp), slice,
				ep->dir_in ? DMA_TO_DEVICE : DMA_FROM_DEVICE);

		if (dma_mapping_error(NULL, d)) {
			pr_err("dma_mapping_error\n");
			return;
		}

		dma_sync_single_for_device(NULL, d, slice,
					   ep->dir_in ? DMA_TO_DEVICE :
						DMA_FROM_DEVICE);

		fotg200_enable_dma(ep, d, slice);

		/* check if dma is done */
		fotg200_wait_dma_done(ep);

		fotg200_disable_dma(ep);

		szOp+=slice;
		dma_unmap_single(NULL, d, slice, DMA_TO_DEVICE);
	}

	/* update actual transfer length */
	req->req.actual += length;

}

static void fotg200_ep0_queue(struct fotg200_ep *ep,
				struct fotg200_request *req)
{
	ep0numsg("fotg200_ep0_queue\n");

	if (!req->req.length) {
		fotg200_done(ep, req, 0);
		return;
	}
	if (ep->dir_in) { /* if IN */
		if (req->req.length) {
			fotg200_start_ep0_data(ep, req);
		} else {
			pr_err("%s : req->req.length = 0x%x\n",
			       __func__, req->req.length);
		}
		if ((req->req.length == req->req.actual) ||
		    (req->req.actual < ep->ep.maxpacket))
			fotg200_done(ep, req, 0);

	} else { /* OUT */
		u32 value = ioread32(ep->fotg200->reg +
					FOTG210_DMISGR0);

		value &= ~DMISGR0_MCX_OUT_INT;
		iowrite32(value, ep->fotg200->reg + FOTG210_DMISGR0);
	}
}

static int fotg200_ep_queue(struct usb_ep *_ep, struct usb_request *_req,
				gfp_t gfp_flags)
{
	struct fotg200_ep *ep;
	struct fotg200_request *req;
	unsigned long flags;
	int request = 0;

	itfnumsg("fotg200_ep_queue\n");

	ep = container_of(_ep, struct fotg200_ep, ep);
	req = container_of(_req, struct fotg200_request, req);

	if (ep->fotg200->gadget.speed == USB_SPEED_UNKNOWN)
		return -ESHUTDOWN;

	spin_lock_irqsave(&ep->fotg200->lock, flags);

	if (list_empty(&ep->queue))
		request = 1;

	list_add_tail(&req->queue, &ep->queue);

	req->req.actual = 0;
	req->req.status = -EINPROGRESS;

	if (!ep->epnum) /* ep0 */
		fotg200_ep0_queue(ep, req);
	else if (request && !ep->stall)
		fotg200_enable_fifo_int(ep);

	spin_unlock_irqrestore(&ep->fotg200->lock, flags);

	return 0;
}

static int fotg200_ep_dequeue(struct usb_ep *_ep, struct usb_request *_req)
{
	struct fotg200_ep *ep;
	struct fotg200_request *req;
	unsigned long flags;

	itfnumsg("fotg200_ep_dequeue\n");

	ep = container_of(_ep, struct fotg200_ep, ep);
	req = container_of(_req, struct fotg200_request, req);

	spin_lock_irqsave(&ep->fotg200->lock, flags);
	if (!list_empty(&ep->queue))
		fotg200_done(ep, req, -ECONNRESET);
	spin_unlock_irqrestore(&ep->fotg200->lock, flags);

	return 0;
}

static void fotg200_set_epnstall(struct fotg200_ep *ep)
{
	struct fotg200_udc *fotg200 = ep->fotg200;
	u32 value;
	void __iomem *reg;

	/* check if IN FIFO is empty before stall */
	if (ep->dir_in) {
		do {
			value = ioread32(fotg200->reg + FOTG210_DCFESR);
		} while (!(value & DCFESR_FIFO_EMPTY(ep->epnum - 1)));
	}

	reg = (ep->dir_in) ?
		fotg200->reg + FOTG210_INEPMPSR(ep->epnum) :
		fotg200->reg + FOTG210_OUTEPMPSR(ep->epnum);
	value = ioread32(reg);
	value |= INOUTEPMPSR_STL_EP;
	iowrite32(value, reg);
}

static void fotg200_clear_epnstall(struct fotg200_ep *ep)
{
	struct fotg200_udc *fotg200 = ep->fotg200;
	u32 value;
	void __iomem *reg;

	reg = (ep->dir_in) ?
		fotg200->reg + FOTG210_INEPMPSR(ep->epnum) :
		fotg200->reg + FOTG210_OUTEPMPSR(ep->epnum);
	value = ioread32(reg);
	value &= ~INOUTEPMPSR_STL_EP;
	iowrite32(value, reg);
}

static int fotg200_set_halt_and_wedge(struct usb_ep *_ep, int value, int wedge)
{
	struct fotg200_ep *ep;
	struct fotg200_udc *fotg200;
	unsigned long flags;
	int ret = 0;

	ep = container_of(_ep, struct fotg200_ep, ep);

	fotg200 = ep->fotg200;

	spin_lock_irqsave(&ep->fotg200->lock, flags);

	if (value) {
		fotg200_set_epnstall(ep);
		ep->stall = 1;
		if (wedge)
			ep->wedged = 1;
	} else {
		fotg200_reset_tseq(fotg200, ep->epnum);
		fotg200_clear_epnstall(ep);
		ep->stall = 0;
		ep->wedged = 0;
		if (!list_empty(&ep->queue))
			fotg200_enable_fifo_int(ep);
	}

	spin_unlock_irqrestore(&ep->fotg200->lock, flags);
	return ret;
}

static int fotg200_ep_set_halt(struct usb_ep *_ep, int value)
{
	itfnumsg("fotg200_ep_set_halt\n");

	return fotg200_set_halt_and_wedge(_ep, value, 0);
}

static int fotg200_ep_set_wedge(struct usb_ep *_ep)
{
	itfnumsg("fotg200_ep_set_wedge\n");
	return fotg200_set_halt_and_wedge(_ep, 1, 1);
}

static void fotg200_ep_fifo_flush(struct usb_ep *_ep)
{
	itfnumsg("fotg200_ep_fifo_flush\n");
}

static int fotg200_fifo_status(struct usb_ep *ep)
{
	itfnumsg("fotg200_fifo_status\n");
	return 0;
}

static struct usb_ep_ops fotg200_ep_ops = {
	.enable		= fotg200_ep_enable,
	.disable	= fotg200_ep_disable,

	.alloc_request	= fotg200_ep_alloc_request,
	.free_request	= fotg200_ep_free_request,

	.queue		= fotg200_ep_queue,
	.dequeue	= fotg200_ep_dequeue,

	.set_halt	= fotg200_ep_set_halt,
	.fifo_flush	= fotg200_ep_fifo_flush,
	.set_wedge	= fotg200_ep_set_wedge,
	.fifo_status= fotg200_fifo_status,
};

static void fotg200_set_tx0byte(struct fotg200_ep *ep)
{
	struct fotg200_udc *fotg200 = ep->fotg200;
	u32 val;
	u32 offset = (ep->dir_in) ? FOTG210_INEPMPSR(ep->epnum) :
				FOTG210_OUTEPMPSR(ep->epnum);

	val = ioread32(fotg200->reg + offset);
	val |= INOUTEPMPSR_TX0BYTE_IEP;
	iowrite32(val, fotg200->reg + offset);

	do{
		udelay(5);
		val = ioread32(fotg200->reg + offset);
	}while(val & INOUTEPMPSR_TX0BYTE_IEP);
}


static void fotg200_clear_tx0byte(struct fotg200_udc *fotg200)
{
	u32 value = ioread32(fotg200->reg + FOTG210_TX0BYTE);

	value &= ~(TX0BYTE_EP1 | TX0BYTE_EP2 | TX0BYTE_EP3
		   | TX0BYTE_EP4);
	iowrite32(value, fotg200->reg + FOTG210_TX0BYTE);
}

static void fotg200_clear_rx0byte(struct fotg200_udc *fotg200)
{
	u32 value = ioread32(fotg200->reg + FOTG210_RX0BYTE);

	value &= ~(RX0BYTE_EP1 | RX0BYTE_EP2 | RX0BYTE_EP3
		   | RX0BYTE_EP4);
	iowrite32(value, fotg200->reg + FOTG210_RX0BYTE);
}

/* read 8-byte setup packet only */
static void fotg200_rdsetupp(struct fotg200_udc *fotg200,
		   u8 *buffer)
{
	u32 *tmp32;
	tmp32 = (u32 *)buffer;
	*tmp32++ = ioread32(fotg200->reg + FOTG210_CXPORT);
	*tmp32   = ioread32(fotg200->reg + FOTG210_CXPORT);

	ep0numsg("SETUP 0x%02X 0x%02X 0x%02X 0x%02X 0x%02X 0x%02X 0x%02X 0x%02X\n"
		,buffer[0],buffer[1],buffer[2],buffer[3],buffer[4],buffer[5],buffer[6],buffer[7]);
}

static void fotg200_set_configuration(struct fotg200_udc *fotg200)
{
	u32 value = ioread32(fotg200->reg + FOTG210_DAR);

	value |= DAR_AFT_CONF;
	iowrite32(value, fotg200->reg + FOTG210_DAR);

	// reset fifo/ep mapping
	iowrite32(0x88000200, fotg200->reg + 0x160);
	iowrite32(0x88000200, fotg200->reg + 0x164);
	iowrite32(0x88000200, fotg200->reg + 0x168);
	iowrite32(0x88000200, fotg200->reg + 0x16C);
	iowrite32(0x88000200, fotg200->reg + 0x170);
	iowrite32(0x88000200, fotg200->reg + 0x174);
	iowrite32(0x88000200, fotg200->reg + 0x178);
	iowrite32(0x88000200, fotg200->reg + 0x17C);
	iowrite32(0, fotg200->reg + 0x1A8);
	iowrite32(0, fotg200->reg + 0x1AC);

	if(ioread32(fotg200->reg + FOTG210_DMCR) & DMCR_HS_EN)
		iowrite32(0x44C, fotg200->reg + FOTG210_SOFTM);
	else
		iowrite32(0x2710, fotg200->reg + FOTG210_SOFTM);

}

static void fotg200_set_dev_addr(struct fotg200_udc *fotg200, u32 addr)
{
	u32 value = ioread32(fotg200->reg + FOTG210_DAR);

	value |= (addr & 0x7F);
	iowrite32(value, fotg200->reg + FOTG210_DAR);
}

static void fotg200_set_cxstall(struct fotg200_udc *fotg200)
{
	u32 value = ioread32(fotg200->reg + FOTG210_DCFESR);

	value |= DCFESR_CX_STL;
	iowrite32(value, fotg200->reg + FOTG210_DCFESR);
	numsg("fotg200_set_cxstall\n");
}

static void fotg200_request_error(struct fotg200_udc *fotg200)
{
	fotg200_set_cxstall(fotg200);
	pr_err("request error!!\n");
}

static void fotg200_set_address(struct fotg200_udc *fotg200,
				struct usb_ctrlrequest *ctrl)
{
	if (ctrl->wValue >= 0x0100) {
		fotg200_request_error(fotg200);
	} else {
		fotg200_set_dev_addr(fotg200, ctrl->wValue);
		fotg200_set_cxdone(fotg200);
	}
}

static void fotg200_set_feature(struct fotg200_udc *fotg200,
				struct usb_ctrlrequest *ctrl)
{
	switch (ctrl->bRequestType & USB_RECIP_MASK) {
	case USB_RECIP_DEVICE:
		fotg200_set_cxdone(fotg200);
		break;
	case USB_RECIP_INTERFACE:
		fotg200_set_cxdone(fotg200);
		break;
	case USB_RECIP_ENDPOINT: {
		u8 epnum;
		epnum = le16_to_cpu(ctrl->wIndex) & USB_ENDPOINT_NUMBER_MASK;
		if (epnum)
			fotg200_set_epnstall(fotg200->ep[epnum]);
		else
			fotg200_set_cxstall(fotg200);
		fotg200_set_cxdone(fotg200);
		}
		break;
	default:
		fotg200_request_error(fotg200);
		break;
	}
}

static void fotg200_clear_feature(struct fotg200_udc *fotg200,
				struct usb_ctrlrequest *ctrl)
{
	struct fotg200_ep *ep =
		fotg200->ep[ctrl->wIndex & USB_ENDPOINT_NUMBER_MASK];

	switch (ctrl->bRequestType & USB_RECIP_MASK) {
	case USB_RECIP_DEVICE:
		fotg200_set_cxdone(fotg200);
		break;
	case USB_RECIP_INTERFACE:
		fotg200_set_cxdone(fotg200);
		break;
	case USB_RECIP_ENDPOINT:
		if (ctrl->wIndex & USB_ENDPOINT_NUMBER_MASK) {
			if (ep->wedged) {
				fotg200_set_cxdone(fotg200);
				break;
			}

			fotg200_clear_epnstall(ep);

			if (ep->stall)
				fotg200_set_halt_and_wedge(&ep->ep, 0, 0);
		}
		fotg200_set_cxdone(fotg200);
		break;
	default:
		fotg200_request_error(fotg200);
		break;
	}
}

static int fotg200_is_epnstall(struct fotg200_ep *ep)
{
	struct fotg200_udc *fotg200 = ep->fotg200;
	u32 value;
	void __iomem *reg;

	reg = (ep->dir_in) ?
		fotg200->reg + FOTG210_INEPMPSR(ep->epnum) :
		fotg200->reg + FOTG210_OUTEPMPSR(ep->epnum);
	value = ioread32(reg);
	return value & INOUTEPMPSR_STL_EP ? 1 : 0;
}

static void fotg200_get_status(struct fotg200_udc *fotg200,
				struct usb_ctrlrequest *ctrl)
{
	u8 epnum;

	switch (ctrl->bRequestType & USB_RECIP_MASK) {
	case USB_RECIP_DEVICE:
		fotg200->ep0_data = 1 << USB_DEVICE_SELF_POWERED;
		break;
	case USB_RECIP_INTERFACE:
		fotg200->ep0_data = 0;
		break;
	case USB_RECIP_ENDPOINT:
		epnum = ctrl->wIndex & USB_ENDPOINT_NUMBER_MASK;
		if (epnum)
			fotg200->ep0_data =
				fotg200_is_epnstall(fotg200->ep[epnum])
				<< USB_ENDPOINT_HALT;
		else
			fotg200_request_error(fotg200);
		break;

	default:
		fotg200_request_error(fotg200);
		return;		/* exit */
	}

	fotg200->ep0_req->buf = &fotg200->ep0_data;
	fotg200->ep0_req->length = 2;

	spin_unlock(&fotg200->lock);
	fotg200_ep_queue(fotg200->gadget.ep0, fotg200->ep0_req, GFP_KERNEL);
	spin_lock(&fotg200->lock);
}

static int fotg200_setup_packet(struct fotg200_udc *fotg200,
				struct usb_ctrlrequest *ctrl)
{
	u8 *p = (u8 *)ctrl;
	u8 ret = 0;

	fotg200_rdsetupp(fotg200, p);

	fotg200->ep[0]->dir_in = ctrl->bRequestType & USB_DIR_IN;

	if (fotg200->gadget.speed == USB_SPEED_UNKNOWN) {
		u32 value = ioread32(fotg200->reg + FOTG210_DMCR);
		fotg200->gadget.speed = value & DMCR_HS_EN ?
				USB_SPEED_HIGH : USB_SPEED_FULL;
	}

	/* check request */
	if ((ctrl->bRequestType & USB_TYPE_MASK) == USB_TYPE_STANDARD) {
		switch (ctrl->bRequest) {
		case USB_REQ_GET_STATUS:
			fotg200_get_status(fotg200, ctrl);
			numsg("USB_REQ_GET_STATUS\n");
			break;
		case USB_REQ_CLEAR_FEATURE:
			fotg200_clear_feature(fotg200, ctrl);
			numsg("USB_REQ_CLEAR_FEATURE\n");
			break;
		case USB_REQ_SET_FEATURE:
			fotg200_set_feature(fotg200, ctrl);
			numsg("USB_REQ_SET_FEATURE\n");
			break;
		case USB_REQ_SET_ADDRESS:
			fotg200_set_address(fotg200, ctrl);
			numsg("USB_REQ_SET_ADDRESS\n");
			break;
		case USB_REQ_SET_CONFIGURATION:
			fotg200_set_configuration(fotg200);
			ret = 1;
			numsg("USB_REQ_SET_CONFIGURATION\n");
			break;
		default:
			ret = 1;
			break;
		}
	} else {
		ret = 1;
	}

	return ret;
}

static void fotg200_ep0out(struct fotg200_udc *fotg200)
{
	struct fotg200_ep *ep = fotg200->ep[0];

	numsg("fotg210_ep0in\n");

	if (!list_empty(&ep->queue) && !ep->dir_in) {
		struct fotg200_request *req;

		req = list_first_entry(&ep->queue,
			struct fotg200_request, queue);

		if (req->req.length)
			fotg200_start_ep0_data(ep, req);
			//fotg200_start_dma(ep, req);

		if ((req->req.length == req->req.actual) ||
		    (req->req.actual < ep->ep.maxpacket))
			fotg200_done(ep, req, 0);
	} else {
		pr_err("%s : empty queue\n", __func__);
	}
}

static void fotg200_ep0in(struct fotg200_udc *fotg200)
{
	struct fotg200_ep *ep = fotg200->ep[0];

	numsg("fotg200_ep0in\n");

	if ((!list_empty(&ep->queue)) && (ep->dir_in)) {
		struct fotg200_request *req;

		req = list_entry(ep->queue.next,
				struct fotg200_request, queue);

		if (req->req.length)
			fotg200_start_ep0_data(ep, req);
			//fotg200_start_dma(ep, req);

		if ((req->req.length == req->req.actual) ||
		    (req->req.actual < ep->ep.maxpacket))
			fotg200_done(ep, req, 0);
	} else {
		fotg200_set_cxdone(fotg200);
	}
}

static void fotg200_clear_comabt_int(struct fotg200_udc *fotg200)
{
	u32 value = ioread32(fotg200->reg + FOTG210_DISGR0);

	value &= ~DISGR0_CX_COMABT_INT;
	iowrite32(value, fotg200->reg + FOTG210_DISGR0);
}

static void fotg200_in_fifo_handler(struct fotg200_ep *ep)
{
	struct fotg200_request *req = list_entry(ep->queue.next,
					struct fotg200_request, queue);

	numsg("fotg200_in_fifo_handler %d\n",req->req.length);

	if (req->req.length)
		fotg200_start_dma(ep, req);
	else
		fotg200_set_tx0byte(ep);

	if (req->req.length == req->req.actual)
		fotg200_done(ep, req, 0);
}

static void fotg200_out_fifo_handler(struct fotg200_ep *ep)
{
	struct fotg200_request *req = list_entry(ep->queue.next,
						 struct fotg200_request, queue);

	numsg("fotg200_out_fifo_handler\n");

	fotg200_start_dma(ep, req);

	/* finish out transfer */
	if (req->req.length == req->req.actual ||
	    req->req.actual < ep->ep.maxpacket)
		fotg200_done(ep, req, 0);
}

static irqreturn_t fotg200_irq(int irq, void *_fotg200)
{
	struct fotg200_udc *fotg200 = _fotg200;
	u32 int_grp = ioread32(fotg200->reg + FOTG210_DIGR);
	u32 int_msk = ioread32(fotg200->reg + FOTG210_DMIGR);

	int_grp &= ~int_msk;

	spin_lock(&fotg200->lock);

	if (int_grp & DIGR_INT_G2) {
		void __iomem *reg = fotg200->reg + FOTG210_DISGR2;
		u32 int_grp2 = ioread32(reg);
		u32 int_msk2 = ioread32(fotg200->reg + FOTG210_DMISGR2);
		//u32 value;

		int_grp2 &= ~int_msk2;
		//iowrite32(int_grp2, reg);
		//pr_err("2");

		//printk("G2 0x%08X\n",int_grp2);
		if (int_grp2 & DISGR2_USBRST_INT) {
			iowrite32(DISGR2_USBRST_INT, reg);

			// reset releated settings when bus reset got
			iowrite32(0x0, fotg200->reg + FOTG210_DAR);

			iowrite32(0x88000200, fotg200->reg + 0x160);
			iowrite32(0x88000200, fotg200->reg + 0x164);
			iowrite32(0x88000200, fotg200->reg + 0x168);
			iowrite32(0x88000200, fotg200->reg + 0x16C);
			iowrite32(0x88000200, fotg200->reg + 0x170);
			iowrite32(0x88000200, fotg200->reg + 0x174);
			iowrite32(0x88000200, fotg200->reg + 0x178);
			iowrite32(0x88000200, fotg200->reg + 0x17C);
			iowrite32(0, fotg200->reg + 0x1A8);
			iowrite32(0, fotg200->reg + 0x1AC);

			devnumsg("fotg200 udc reset\n");
		}
		if (int_grp2 & DISGR2_SUSP_INT) {
			iowrite32(DISGR2_SUSP_INT, reg);
			devnumsg("fotg200 udc suspend\n");
		}
		if (int_grp2 & DISGR2_RESM_INT) {
			iowrite32(DISGR2_RESM_INT, reg);
			devnumsg("fotg200 udc resume\n");
		}
		if (int_grp2 & DISGR2_ISO_SEQ_ERR_INT) {
			iowrite32(DISGR2_ISO_SEQ_ERR_INT, reg);
			devnumsg("fotg200 iso sequence error\n");
		}
		if (int_grp2 & DISGR2_ISO_SEQ_ABORT_INT) {
			iowrite32(DISGR2_ISO_SEQ_ABORT_INT, reg);
			devnumsg("fotg200 iso sequence abort\n");
		}
		if (int_grp2 & DISGR2_DMA_CMPLT) {
			pr_err("fotg200 DMA_CMPLT\n");
		}
		if (int_grp2 & DISGR2_DMA_CMPLT2) {
			pr_err("fotg200 DMA_CMPLT2\n");
		}
		if (int_grp2 & DISGR2_DMA_CMPLT3) {
			pr_err("fotg200 DMA_CMPLT3\n");
		}
		if (int_grp2 & DISGR2_DMA_CMPLT4) {
			pr_err("fotg200 DMA_CMPLT4\n");
		}
		if (int_grp2 & DISGR2_DMA_CMPLT5) {
			pr_err("fotg200 DMA_CMPLT5\n");
		}

		if (int_grp2 & DISGR2_TX0BYTE_INT) {
			iowrite32(DISGR2_TX0BYTE_INT, reg);
			fotg200_clear_tx0byte(fotg200);
			numsg("fotg200 transferred 0 byte\n");
		}
		if (int_grp2 & DISGR2_RX0BYTE_INT) {
			iowrite32(DISGR2_RX0BYTE_INT, reg);
			fotg200_clear_rx0byte(fotg200);
			numsg("fotg200 received 0 byte\n");
		}

	}





	if (int_grp & DIGR_INT_G0) {
		void __iomem *reg = fotg200->reg + FOTG210_DISGR0;
		u32 int_grp0 = ioread32(reg);
		u32 int_msk0 = ioread32(fotg200->reg + FOTG210_DMISGR0);
		struct usb_ctrlrequest ctrl;

		int_grp0 &= ~int_msk0;

		if(int_grp0 & ~0x1)
			numsg("G0 0x%08X\n",int_grp0);

		//pr_err("0");

		/* the highest priority in this source register */
		if (int_grp0 & DISGR0_CX_COMABT_INT) {
			fotg200_clear_comabt_int(fotg200);
			numsg("fotg200 CX command abort\n");
		}

		if (int_grp0 & DISGR0_CX_SETUP_INT) {
			ep0numsg("fotg210 SETUP\n");
			if (fotg200_setup_packet(fotg200, &ctrl)) {
				spin_unlock(&fotg200->lock);
				if (fotg200->driver->setup(&fotg200->gadget,
							   &ctrl) < 0)
					fotg200_set_cxstall(fotg200);
				spin_lock(&fotg200->lock);
			}
		}
		if (int_grp0 & DISGR0_CX_COMEND_INT)
			numsg("fotg200 cmd end\n");

		if (int_grp0 & DISGR0_CX_IN_INT) {
			fotg200_ep0in(fotg200);
			numsg("fotg200 CX_IN_INT\n");
		}

		if (int_grp0 & DISGR0_CX_OUT_INT) {
			fotg200_ep0out(fotg200);
			numsg("fotg200 CX_OUT_INT\n");
		}

		if (int_grp0 & DISGR0_CX_COMFAIL_INT) {
			fotg200_set_cxstall(fotg200);
			numsg("fotg200 ep0 fail\n");
		}
	}

	if (int_grp & DIGR_INT_G1) {
		void __iomem *reg = fotg200->reg + FOTG210_DISGR1;
		u32 int_grp1 = ioread32(reg);
		u32 int_msk1 = ioread32(fotg200->reg + FOTG210_DMISGR1);
		int fifo;

		int_grp1 &= ~int_msk1;

		numsg("G1 0x%08X\n",int_grp1);
		//pr_err("1");

		for (fifo = 0; fifo < FOTG210_MAX_FIFO_NUM; fifo+=2) {
			if (int_grp1 & DISGR1_IN_INT(fifo)) {
				fotg200_in_fifo_handler(fotg200->ep[(fifo>>1) + 1]);
			}

			if ((int_grp1 & DISGR1_OUT_INT(fifo)) ||
			    (int_grp1 & DISGR1_SPK_INT(fifo))) {
				fotg200_out_fifo_handler(fotg200->ep[(fifo>>1) + 1]);
			}
		}
	}

	spin_unlock(&fotg200->lock);

	return IRQ_HANDLED;
}


static irqreturn_t fotg200_isr(int irq, void *_fotg200)
{
	u32 value;
	struct fotg200_udc *fotg200 = _fotg200;

	spin_lock(&fotg200->lock);

	value = ioread32(fotg200->reg + FOTG210_DMCR);
	value &= ~DMCR_GLINT_EN;
	iowrite32(value, fotg200->reg + FOTG210_DMCR);

	tasklet_schedule(fotg200->t1);

	spin_unlock(&fotg200->lock);

	return IRQ_HANDLED;
}

static void fotg200_ist(unsigned long _fotg200)
{
	u32 value;
	struct fotg200_udc *fotg200 = (struct fotg200_udc *)_fotg200;

	fotg200_irq(0,(void *)_fotg200);

	value = ioread32(fotg200->reg + FOTG210_DMCR);
	value |= DMCR_GLINT_EN;
	iowrite32(value, fotg200->reg + FOTG210_DMCR);
}



static void fotg200_disable_unplug(struct fotg200_udc *fotg200)
{
	u32 reg = ioread32(fotg200->reg + FOTG210_PHYTMSR);

	reg &= ~PHYTMSR_UNPLUG;
	iowrite32(reg, fotg200->reg + FOTG210_PHYTMSR);
}

static void fotg200_enable_unplugsuspend(struct fotg200_udc *fotg200)
{
	u32 reg = ioread32(fotg200->reg + FOTG210_PHYTMSR);
	reg |= PHYTMSR_UNPLUG;
	iowrite32(reg, fotg200->reg + FOTG210_PHYTMSR);

	reg = ioread32(fotg200->reg + FOTG210_DMACPSR1);
	reg |= DMACPSR1_DEVSUSPEND;
	iowrite32(reg, fotg200->reg + FOTG210_DMACPSR1);


}

static int fotg200_udc_start(struct usb_gadget *g,
		struct usb_gadget_driver *driver)
{
	struct fotg200_udc *fotg200 = gadget_to_fotg200(g);
	u32 value;

	itfnumsg("fotg200_udc_start\n");

	/* make device exit suspend */
	value = ioread32(fotg200->reg + FOTG210_DMACPSR1);
	value &= ~DMACPSR1_DEVSUSPEND;
	iowrite32(value, fotg200->reg + FOTG210_DMACPSR1);

	/* hook up the driver */
	driver->driver.bus = NULL;
	fotg200->driver = driver;

	tasklet_init(fotg200->t1, fotg200_ist, (unsigned long)fotg200);

	fotg200_disable_unplug(fotg200);

	/* enable device global interrupt */
	value = ioread32(fotg200->reg + FOTG210_DMCR);
	value &= ~(0xFF<<16);
	value |= (0x5<<16);
	value |= DMCR_GLINT_EN;
	iowrite32(value|DMCR_SFRST, fotg200->reg + FOTG210_DMCR);
	iowrite32(value, fotg200->reg + FOTG210_DMCR);

	iowrite32(DTR_TST_CLRFF|ioread32(fotg200->reg + FOTG210_DTR), fotg200->reg + FOTG210_DTR);

	return 0;
}

static void fotg200_init(struct fotg200_udc *fotg200)
{
	u32 value;

	iowrite32(0x3, fotg200->reg + 0x310);

	/* make device exit suspend */
	value = ioread32(fotg200->reg + FOTG210_DMACPSR1);
	value &= ~DMACPSR1_DEVSUSPEND;
	iowrite32(value, fotg200->reg + FOTG210_DMACPSR1);

	udelay(200);

	/* VBUS debounce time */
	iowrite32(ioread32(fotg200->reg + FOTG210_OTGCTRLSTS)|0x400, fotg200->reg + FOTG210_OTGCTRLSTS);

	/* disable device global interrupt */
	value = ioread32(fotg200->reg + FOTG210_DMCR);
	value &= ~DMCR_GLINT_EN;
	//value |= DMCR_HALF_SPEED;
	iowrite32(value, fotg200->reg + FOTG210_DMCR);

	/* disable global interrupt and set int polarity to active high */
	iowrite32(GMIR_MHC_INT | GMIR_MOTG_INT, fotg200->reg + FOTG210_GMIR);

	/* disable all fifo interrupt */
	iowrite32(~(u32)0, fotg200->reg + FOTG210_DMISGR1);

	/* disable cmd end */
	value = ioread32(fotg200->reg + FOTG210_DMISGR0);
	value |= (DMISGR0_MCX_COMEND|DMISGR0_MCX_OUT_INT|DMISGR0_MCX_IN_INT);
	iowrite32(value, fotg200->reg + FOTG210_DMISGR0);


	iowrite32(DTR_TST_CLRFF|ioread32(fotg200->reg + FOTG210_DTR), fotg200->reg + FOTG210_DTR);
	iowrite32(0x7, fotg200->reg + FOTG210_DICR);

	/* Mask idle int */
	value = ioread32(fotg200->reg + FOTG210_DMISGR2);
	value |= (DMISGR2_MDMA_IDLE|DMISGR2_MDMA_WAKEUPVBUS|DMISGR2_MDMA_CMPLT|DMISGR2_MDMA_CMPLT2
		|DMISGR2_MDMA_CMPLT3|DMISGR2_MDMA_CMPLT4|DMISGR2_MDMA_CMPLT5|DMISGR2_MDMA_CMPLT6
		|DMISGR2_MDMA_CMPLT7|DMISGR2_MDMA_CMPLT8);
	iowrite32(value, fotg200->reg + FOTG210_DMISGR2);
}

static int fotg200_udc_stop(struct usb_gadget *g)
{
	struct fotg200_udc *fotg200 = gadget_to_fotg200(g);
	unsigned long	flags;
	u32 value;

	itfnumsg("fotg200_udc_stop\n");

	spin_lock_irqsave(&fotg200->lock, flags);

	//fotg210_init(fotg210);
	fotg200->driver = NULL;
	fotg200_enable_unplugsuspend(fotg200);

	/* enable device global interrupt */
	value = ioread32(fotg200->reg + FOTG210_DMCR);
	value &= ~DMCR_GLINT_EN;
	iowrite32(value, fotg200->reg + FOTG210_DMCR);

	tasklet_kill(fotg200->t1);

	spin_unlock_irqrestore(&fotg200->lock, flags);

	return 0;
}

static struct usb_gadget_ops fotg200_gadget_ops = {
	.udc_start		= fotg200_udc_start,
	.udc_stop		= fotg200_udc_stop,
};

static int fotg200_udc_remove(struct platform_device *pdev)
{
	struct fotg200_udc *fotg200 = platform_get_drvdata(pdev);

	itfnumsg("fotg200_udc_remove\n");

	usb_del_gadget_udc(&fotg200->gadget);
	free_irq(platform_get_irq(pdev, 0), fotg200);

	fotg200_ep_free_request(&fotg200->ep[0]->ep, fotg200->ep0_req);
	iounmap(fotg200->reg);
	kfree(fotg200->t1);
	kfree(fotg200);

	return 0;
}

static int fotg200_udc_probe(struct platform_device *pdev)
{
	struct resource *res, *ires;
	struct fotg200_udc *fotg200 = NULL;
	struct fotg200_ep *_ep[FOTG210_MAX_NUM_EP];
	int ret = 0;
	int i;

	numsg("fotg200_udc_probe\n");

	res = platform_get_resource(pdev, IORESOURCE_MEM, 0);
	if (!res) {
		pr_err("platform_get_resource error.\n");
		return -ENODEV;
	}

	ires = platform_get_resource(pdev, IORESOURCE_IRQ, 0);
	if (!ires) {
		pr_err("platform_get_resource IORESOURCE_IRQ error.\n");
		return -ENODEV;
	}

	ret = -ENOMEM;

	/* initialize udc */
	fotg200 = kzalloc(sizeof(struct fotg200_udc), GFP_KERNEL);
	if (fotg200 == NULL) {
		pr_err("kzalloc error\n");
		goto err_alloc;
	}

	for (i = 0; i < FOTG210_MAX_NUM_EP; i++) {
		_ep[i] = kzalloc(sizeof(struct fotg200_ep), GFP_KERNEL);
		if (_ep[i] == NULL) {
			pr_err("_ep kzalloc error\n");
			goto err_alloc;
		}
		fotg200->ep[i] = _ep[i];
	}

	fotg200->t1 = kzalloc(sizeof(struct tasklet_struct), GFP_KERNEL);
	if(fotg200->t1 == NULL) {
		pr_err("tasklet alloc error.\n");
		goto err_alloc;
	}

	fotg200->reg = ioremap(res->start, resource_size(res));
	if (fotg200->reg == NULL) {
		pr_err("ioremap error.\n");
		goto err_map;
	}

	spin_lock_init(&fotg200->lock);

	platform_set_drvdata(pdev, fotg200);

	fotg200->gadget.ops = &fotg200_gadget_ops;

	fotg200->gadget.max_speed = USB_SPEED_HIGH;
	fotg200->gadget.dev.parent = &pdev->dev;
	fotg200->gadget.dev.dma_mask = pdev->dev.dma_mask;
	fotg200->gadget.name = udc_name;

	INIT_LIST_HEAD(&fotg200->gadget.ep_list);

	for (i = 0; i < FOTG210_MAX_NUM_EP; i++) {
		struct fotg200_ep *ep = fotg200->ep[i];

		if (i) {
			INIT_LIST_HEAD(&fotg200->ep[i]->ep.ep_list);
			list_add_tail(&fotg200->ep[i]->ep.ep_list,
				      &fotg200->gadget.ep_list);
		}
		ep->fotg200 = fotg200;
		INIT_LIST_HEAD(&ep->queue);
		ep->ep.name = fotg200_ep_name[i];
		ep->ep.ops = &fotg200_ep_ops;
		usb_ep_set_maxpacket_limit(&ep->ep, (unsigned short) ~0);
	}
	usb_ep_set_maxpacket_limit(&fotg200->ep[0]->ep, 0x40);
	fotg200->gadget.ep0 = &fotg200->ep[0]->ep;
	INIT_LIST_HEAD(&fotg200->gadget.ep0->ep_list);

	fotg200->ep0_req = fotg200_ep_alloc_request(&fotg200->ep[0]->ep,
				GFP_KERNEL);
	if (fotg200->ep0_req == NULL)
		goto err_req;

	fotg200_init(fotg200);

	//fotg210_disable_unplug(fotg210);


#if 1
	/* Using tasklet for command processing */
	ret = request_irq(ires->start, fotg200_isr, IRQF_SHARED,
			  udc_name, fotg200);
#else
	ret = request_irq(ires->start, fotg200_irq, IRQF_SHARED,
			  udc_name, fotg200);
#endif
	if (ret < 0) {
		pr_err("request_irq error (%d)\n", ret);
		goto err_irq;
	}

	ret = usb_add_gadget_udc(&pdev->dev, &fotg200->gadget);
	if (ret)
		goto err_add_udc;

	dev_info(&pdev->dev, "version %s\n", DRIVER_VERSION);

	return 0;

err_add_udc:
err_irq:
	free_irq(ires->start, fotg200);

err_req:
	fotg200_ep_free_request(&fotg200->ep[0]->ep, fotg200->ep0_req);

err_map:
	if (fotg200->reg)
		iounmap(fotg200->reg);

err_alloc:
	kfree(fotg200);

	return ret;
}


#ifdef CONFIG_OF
static const struct of_device_id of_fotg200_match[] = {
	{
		.compatible = "nvt,fotg200_udc"
	},

	{ },
};
MODULE_DEVICE_TABLE(of, of_fotg200_match);
#endif


static struct platform_driver fotg200_driver = {
	.driver		= {
		.name =	(char *)udc_name,
#ifdef CONFIG_OF
		.of_match_table = of_match_ptr(of_fotg200_match),
#endif
	},
	.probe		= fotg200_udc_probe,
	.remove		= fotg200_udc_remove,

};

module_platform_driver(fotg200_driver);

MODULE_AUTHOR("Klins Chen <klins_chen@novatek.com.tw>");
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION(DRIVER_DESC);
