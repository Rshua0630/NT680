/**
    NVT mmc function
    To handle mmc modules
    @file       na51000_mmchost.c
    @ingroup
    @note
    Copyright   Novatek Microelectronics Corp. 2016.  All rights reserved.

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License version 2 as
    published by the Free Software Foundation.
*/

#include "na51000_mmchost.h"
#include "na51000_mmcreg.h"
#include <linux/io.h>
#include <linux/printk.h>
#include <mach/nvt-io.h>

//#define _FPGA_EMULATION_
#define pad_driver  0
#define debug_err(msg)
#define debug_wrn(msg)
#define debug_ind(msg)
#define loc_cpu()
#define unl_cpu()

#define PAD_DS_REG_OFS              (NVT_PAD_BASE_VIRT+0x40)

#define PAD_DS_CGPIO_BASE      0
#define PAD_DS_GROUP5_40        0x8000
#define PAD_DS_CGPIO16         ((PAD_DS_CGPIO_BASE + 16) | PAD_DS_GROUP5_40)
#define PAD_DS_CGPIO17         ((PAD_DS_CGPIO_BASE + 17) | PAD_DS_GROUP5_40)
#define PAD_DS_CGPIO18         ((PAD_DS_CGPIO_BASE + 18) | PAD_DS_GROUP5_40)
#define PAD_DS_CGPIO19         ((PAD_DS_CGPIO_BASE + 19) | PAD_DS_GROUP5_40)
#define PAD_DS_CGPIO20         ((PAD_DS_CGPIO_BASE + 20) | PAD_DS_GROUP5_40)
#define PAD_DS_CGPIO21         ((PAD_DS_CGPIO_BASE + 21) | PAD_DS_GROUP5_40)
#define PAD_DS_CGPIO22         ((PAD_DS_CGPIO_BASE + 22) | PAD_DS_GROUP5_40)
#define PAD_DS_CGPIO23         ((PAD_DS_CGPIO_BASE + 23) | PAD_DS_GROUP5_40)
#define PAD_DS_CGPIO24         ((PAD_DS_CGPIO_BASE + 24) | PAD_DS_GROUP5_40)
#define PAD_DS_CGPIO25         ((PAD_DS_CGPIO_BASE + 25) | PAD_DS_GROUP5_40)
#define PAD_DS_CGPIO26         ((PAD_DS_CGPIO_BASE + 26) | PAD_DS_GROUP5_40)
#define PAD_DS_CGPIO27         ((PAD_DS_CGPIO_BASE + 27) | PAD_DS_GROUP5_40)

#define PAD_DRIVINGSINK_2P5MA    0x01
#define PAD_DRIVINGSINK_5MA      0x02
#define PAD_DRIVINGSINK_7P5MA    0x04
#define PAD_DRIVINGSINK_10MA     0x08
#define PAD_DRIVINGSINK_12P5MA   0x10
#define PAD_DRIVINGSINK_15MA     0x20
#define PAD_DRIVINGSINK_17P5MA   0x40
#define PAD_DRIVINGSINK_20MA     0x80
#define PAD_DRIVINGSINK_25MA    0x100
#define PAD_DRIVINGSINK_30MA    0x200
#define PAD_DRIVINGSINK_35MA    0x400
#define PAD_DRIVINGSINK_40MA    0x800

#define RESET_TIMEOUT	10000
/************************* cache API ********************************/

#include <asm/cacheflush.h>
#include <linux/dma-mapping.h>

void dma_flushwritecache(struct device *dev, void *pbuf, u32 len)
{
	dma_addr_t dma_handle;
	/*void *ptr;*/
	/*printk("flash write buf 0x%x, len %d\r\n", pbuf, len);*/
	/*for (ptr = pbuf; ptr < (pbuf + len + PAGE_SIZE); ptr += PAGE_SIZE)
		flush_dcache_page(virt_to_page(ptr));*/
	dma_handle = dma_map_single(dev, pbuf, len, DMA_TO_DEVICE);

	dma_sync_single_for_device(dev, dma_handle, len, DMA_TO_DEVICE);

	dma_unmap_single(dev, dma_handle, len, DMA_TO_DEVICE);
}

void dma_flushreadcache(struct device *dev, void *pbuf, u32 len)
{
	dma_addr_t dma_handle;
	/*void *ptr;*/
	/*printk("flash read buf 0x%x, len %d\r\n", pbuf, len);*/
	/*for (ptr = pbuf; ptr < (pbuf + len + PAGE_SIZE); ptr += PAGE_SIZE)
		flush_dcache_page(virt_to_page(ptr));*/
	dma_handle = dma_map_single(dev, pbuf, len, DMA_FROM_DEVICE);

	dma_sync_single_for_device(dev, dma_handle, len, DMA_FROM_DEVICE);

	dma_unmap_single(dev, dma_handle, len, DMA_FROM_DEVICE);
}

void dma_flushbidircache(struct device *dev, void *pbuf, u32 len)
{
	dma_addr_t dma_handle;
	/*void *ptr;*/
	/*printk("flash read buf 0x%x, len %d\r\n", pbuf, len);*/
	/*for (ptr = pbuf; ptr < (pbuf + len + PAGE_SIZE); ptr += PAGE_SIZE)
		flush_dcache_page(virt_to_page(ptr));*/
	dma_handle = dma_map_single(dev, pbuf, len, DMA_BIDIRECTIONAL);

	dma_sync_single_for_device(dev, dma_handle, len, DMA_BIDIRECTIONAL);

	dma_unmap_single(dev, dma_handle, len, DMA_BIDIRECTIONAL);
}

/************************* cache API ********************************/




/*
	Internal reference structure
*/

/*	SDIO host driver structure
*/
struct SDHOSTDRIVER {
	const uint32_t        uihostid;
	/*const FLGPTN        flgCommand;*/
	/*const FLGPTN        flgData;*/
	/*const FLGPTN        flgBoot;*/
	/*const uint32_t        uiIntID;*/
	union SDIO_STATUS_REG   cmdintsts;
	union SDIO_STATUS_REG   dataintsts;
};

/*-------------------------------------------------------*/
/* Internal function prototypes                          */
/*-------------------------------------------------------*/
static REGVALUE sdiohost_getreg(struct mmc_nvt_host *host, uint32_t id,
	uint32_t offset);
static void sdiohost_setreg(struct mmc_nvt_host *host, uint32_t id,
	uint32_t offset, REGVALUE value);

/*-------------------------------------------------------*/
/*  Private driver specific variables                    */
/*-------------------------------------------------------*/
static  struct SDHOSTDRIVER vsdiohosts[SDIO_HOST_ID_COUNT] = {
	{
		SDIO_HOST_ID_1,
		/*FLGPTN_SDIO,*/
		/*FLGPTN_SDIO_DATA,*/
		/*FLGPTN_SDIO_BOOT,*/
		/*DRV_INT_SDIO,*/
		{0},
		{0}
	},
#if (SDIO_HOST_ID_COUNT > 1)
	{
		SDIO_HOST_ID_2,
		/*FLGPTN_SDIO2,*/
		/*FLGPTN_SDIO2_DATA,*/
		/*FLGPTN_SDIO2_BOOT,*/
		/*DRV_INT_SDIO2,*/
		{0},
		{0}
	},
#endif
#if (SDIO_HOST_ID_COUNT > 2)
	{
		SDIO_HOST_ID_3,
		/*FLGPTN_SDIO3,*/
		/*FLGPTN_SDIO3_DATA,*/
		/*FLGPTN_SDIO3_BOOT,*/
		/*DRV_INT_SDIO3,*/
		{0},
		{0}
	}
#endif
};

static uint32_t vsdio_seg_en[SDIO_HOST_ID_COUNT] = {
	FALSE,
#if (SDIO_HOST_ID_COUNT > 1)
	FALSE,
#endif
#if (SDIO_HOST_ID_COUNT > 2)
	FALSE
#endif
};

static uint32_t vsdio_seg_num[SDIO_HOST_ID_COUNT] = {
	1,
#if (SDIO_HOST_ID_COUNT > 1)
	1,
#endif
#if (SDIO_HOST_ID_COUNT > 2)
	1
#endif
};

#if (SDIO_SCATTER_DMA)
#define SDIO_DES_MAX_BUF_SIZE     (64*1024*1024)  /*64MB*/

/*static uint32_t
vuisdio_destab[SDIO_HOST_ID_COUNT][SDIO_DES_TABLE_NUM*SDIO_DES_WORD_SIZE];*/
#endif



#if 0
static bool vuisdio_autopinmux[SDIO_HOST_ID_COUNT] = {
	TRUE,
#if (SDIO_HOST_ID_COUNT > 1)
	TRUE
#endif
};
#endif

/*********************************************************************/
/*  Public(to driver layer) SDIO host controller functions           */
/*********************************************************************/

void sdiohost_setdesen(uint32_t id, uint32_t uien)
{
	if (uien == 0)
		vsdio_seg_en[id] = FALSE;
	else
		vsdio_seg_en[id] = TRUE;
}

void sdiohost_setdestab(uint32_t id, uint32_t uidesaddr, uint32_t uidesnum,
uint32_t *vuisdio_destab)
{
#if (SDIO_SCATTER_DMA)
	/*union SDIO_DATA_LENGTH_REG datalenreg; */
	/*union SDIO_DMA_DES_START_ADDR_REG dmadesaddrreg; */
	union SDIO_DES_LINE_REG dmadeslinereg;
	union SDIO_DES_LINE1_REG dmadesline1reg;
	union SDIO_DES_LINE2_REG dmadesline2reg;
	uint32_t uitotalsize;
	struct STRG_SEG_DES *pseg_des;
	uint32_t i;

	if (uidesnum > SDIO_DES_TABLE_NUM) {
		debug_err(("Descriptor number must < SDIO_DES_TABLE_NUM\r\n"));
		return;
	}

	memset((void *)&vuisdio_destab[0], 0,
	(SDIO_DES_TABLE_NUM * SDIO_DES_WORD_SIZE)<<2);

	uitotalsize = 0;
	pseg_des = (struct STRG_SEG_DES *)uidesaddr;

	for (i = 0; i < uidesnum; i++) {
		dmadeslinereg.reg = 0;
		dmadesline1reg.reg = 0;
		dmadesline2reg.reg = 0;

		dmadeslinereg.bit.DMA_DES_VALID = 1;

		if (i == (uidesnum - 1))
			dmadeslinereg.bit.DMA_DES_END = 1;
		else
			dmadeslinereg.bit.DMA_DES_END = 0;

		dmadeslinereg.bit.DMA_DES_ACT = 2;

		dmadesline1reg.bit.DMA_DES_DATA_LEN = pseg_des->uisegsize;
		dmadesline2reg.bit.DMA_DES_DATA_ADDR = pseg_des->uisegaddr;

		vuisdio_destab[(i * SDIO_DES_WORD_SIZE) +
		SDIO_DES_LINE_REG_OFS] = (uint32_t)dmadeslinereg.reg;

		vuisdio_destab[(i * SDIO_DES_WORD_SIZE) +
		(SDIO_DES_LINE1_REG_OFS >> 2)] = (uint32_t)dmadesline1reg.reg;

		vuisdio_destab[(i * SDIO_DES_WORD_SIZE) +
		(SDIO_DES_LINE2_REG_OFS >> 2)] = (uint32_t)dmadesline2reg.reg;

		pseg_des++;
		uitotalsize = dmadesline1reg.bit.DMA_DES_DATA_LEN;

	}
	vsdio_seg_num[id] = uidesnum;
#endif
}

/*
	Get SDIO Busy or not

	@param[in] id   SDIO channel ID
			- @b SDIO_HOST_ID_1: SDIO1
			- @b SDIO_HOST_ID_2: SDIO2

	@return TRUE: ready
		FALSE: busy
*/
bool sdiohost_getrdy(struct mmc_nvt_host *host, uint32_t id)
{
	union SDIO_BUS_STATUS_REG stsreg;

	stsreg.reg = sdiohost_getreg(host, id, SDIO_BUS_STATUS_REG_OFS);

	return stsreg.bit.CARD_BUSY;
}

/*
	Enable CKGEN clock for specified SDIO channel.

	@param[in] id   SDIO channel ID
			- @b SDIO_HOST_ID_1: SDIO1
			- @b SDIO_HOST_ID_2: SDIO2

	@return void
*/
void sdiohost_enclock(struct mmc_nvt_host *host, uint32_t id)
{
	clk_enable(host->clk);
}

/*
	Disable CKGEN clock for specified SDIO channel.

	@param[in] id   SDIO channel ID
			- @b SDIO_HOST_ID_1: SDIO1
			- @b SDIO_HOST_ID_2: SDIO2

	@return void
*/
void sdiohost_disclock(struct mmc_nvt_host *host, uint32_t id)
{
	clk_disable(host->clk);
}


/*
	Enable SD clock for specified SDIO channel.

	@param[in] id   SDIO channel ID
			- @b SDIO_HOST_ID_1: SDIO1
			- @b SDIO_HOST_ID_2: SDIO2

	@return void
*/
void sdiohost_enclockout(struct mmc_nvt_host *host, uint32_t id)
{
	union SDIO_CLOCK_CTRL_REG clk_ctrl;

	clk_ctrl.reg = sdiohost_getreg(host, id, SDIO_CLOCK_CTRL_REG_OFS);
	clk_ctrl.bit.CLK_DIS = 0;
	sdiohost_setreg(host, id, SDIO_CLOCK_CTRL_REG_OFS, clk_ctrl.reg);
}

/*
	Disable SD clock for specified SDIO channel.

	@param[in] id   SDIO channel ID
			- @b SDIO_HOST_ID_1: SDIO1
			- @b SDIO_HOST_ID_2: SDIO2

	@return void
*/
void sdiohost_disclockout(struct mmc_nvt_host *host, uint32_t id)
{
	union SDIO_CLOCK_CTRL_REG clk_ctrl;
	clk_ctrl.reg = sdiohost_getreg(host, id, SDIO_CLOCK_CTRL_REG_OFS);
	clk_ctrl.bit.CLK_DIS = 1;
	sdiohost_setreg(host, id, SDIO_CLOCK_CTRL_REG_OFS, clk_ctrl.reg);
}

void sdiohost_delay_chain(struct mmc_nvt_host *host, uint32_t clock_delay,
			uint32_t delay_chain_setting)
{
	union SDIO_DLY0_REG delay_chain;
	union SDIO_CLOCK_CTRL2_REG clock_ctrl;
	union SDIO_PHY_REG phyreg, phyreg_read;
	u8 i;

	clock_ctrl.reg = clock_delay;
	sdiohost_setreg(host, host->id, SDIO_CLOCK_CTRL2_REG_OFS, \
			clock_ctrl.reg);

	delay_chain.reg = delay_chain_setting;
	sdiohost_setreg(host, host->id, SDIO_DLY0_REG_OFS, delay_chain.reg);
	phyreg.reg = sdiohost_getreg(host, host->id, SDIO_PHY_REG_OFS);
	phyreg.bit.PHY_SW_RST = 1;
	sdiohost_setreg(host, host->id, SDIO_PHY_REG_OFS, phyreg.reg);
	while (1)
	{
		phyreg_read.reg = sdiohost_getreg(host, host->id, \
						SDIO_PHY_REG_OFS);
		if ((phyreg_read.bit.PHY_SW_RST == 0) || (i == RESET_TIMEOUT))
			break;

		i++;
	}

	if (i == RESET_TIMEOUT)
		pr_err("reset timeout\n");
}


int pad_setdrivingsink(uint32_t name, uint32_t paddriving)
{
	unsigned long padreg;
	unsigned long dwoffset, bitoffset, bitmask;
	unsigned long driving;

	if ((name >= PAD_DS_CGPIO16) && (name <= PAD_DS_CGPIO23)) {
		dwoffset  = 0x6C - 0x40;
		bitoffset = (name - (PAD_DS_CGPIO16))*4;
	} else {
		dwoffset  = 0x70 - 0x40;
		bitoffset = (name - (PAD_DS_CGPIO24))*4;
	}

	name -= PAD_DS_GROUP5_40;
	bitmask = 0x07;
	if (paddriving & PAD_DRIVINGSINK_5MA)
		driving = 0;
	else if (paddriving & PAD_DRIVINGSINK_10MA)
		driving = 1;
	else if (paddriving & PAD_DRIVINGSINK_15MA)
		driving = 2;
	else if (paddriving & PAD_DRIVINGSINK_20MA)
		driving = 3;
	else if (paddriving & PAD_DRIVINGSINK_25MA)
		driving = 4;
	else if (paddriving & PAD_DRIVINGSINK_30MA)
		driving = 5;
	else if (paddriving & PAD_DRIVINGSINK_35MA)
		driving = 6;
	else if (paddriving & PAD_DRIVINGSINK_40MA)
		driving = 7;
	else
		return E_PAR;


	padreg = nvt_readl(PAD_DS_REG_OFS + dwoffset);

	padreg &= ~(bitmask << bitoffset);
	padreg |=  (driving << bitoffset);

	nvt_writel(padreg, PAD_DS_REG_OFS + dwoffset);

	return E_OK;

}

static u32 driving_xfer(u32 driving)
{
	u32 pad_driving;

	if (driving == 40)
		pad_driving = PAD_DRIVINGSINK_40MA;
	else if (driving == 10)
		pad_driving = PAD_DRIVINGSINK_10MA;
	else if (driving == 15)
		pad_driving = PAD_DRIVINGSINK_15MA;
	else if (driving == 20)
		pad_driving = PAD_DRIVINGSINK_20MA;
	else if (driving == 25)
		pad_driving = PAD_DRIVINGSINK_25MA;
	else if (driving == 30)
		pad_driving = PAD_DRIVINGSINK_30MA;
	else if (driving == 35)
		pad_driving = PAD_DRIVINGSINK_35MA;
	else
		pad_driving = PAD_DRIVINGSINK_5MA;

	return pad_driving;
}

/*
	Set PAD drive/sink of clock pin for specified SDIO channel.

	@param[in] id       SDIO channel ID
			- @b SDIO_HOST_ID_1: SDIO1
			- @b SDIO_HOST_ID_2: SDIO2
	@param[in] driving  desired driving value * 10, unit: mA
				valid value: 50 ~ 200

	@return
		- @b E_OK: sucess
		- @b Else: fail
*/
int sdiohost_setpaddriving(struct mmc_nvt_host *host, SDIO_SPEED_MODE mode)
{
	uint32_t data_uidriving, cmd_uidriving, clk_uidriving;

	if (host->id >= SDIO_HOST_ID_COUNT)
		return E_SYS;

	if (mode == SDIO_MODE_DS) {
		data_uidriving = driving_xfer(host->pad_driving[SDIO_DS_MODE_DATA]);
		cmd_uidriving = driving_xfer(host->pad_driving[SDIO_DS_MODE_CMD]);
		clk_uidriving = driving_xfer(host->pad_driving[SDIO_DS_MODE_CLK]);
	} else if (mode == SDIO_MODE_HS) {
		data_uidriving = driving_xfer(host->pad_driving[SDIO_HS_MODE_DATA]);
		cmd_uidriving = driving_xfer(host->pad_driving[SDIO_HS_MODE_CMD]);
		clk_uidriving = driving_xfer(host->pad_driving[SDIO_HS_MODE_CLK]);
	} else if (mode == SDIO_MODE_SDR50) {
		data_uidriving = driving_xfer(host->pad_driving[SDIO_SDR50_MODE_DATA]);
		cmd_uidriving = driving_xfer(host->pad_driving[SDIO_SDR50_MODE_CMD]);
		clk_uidriving = driving_xfer(host->pad_driving[SDIO_SDR50_MODE_CLK]);
	} else {
		data_uidriving = driving_xfer(host->pad_driving[SDIO_SDR104_MODE_DATA]);
		cmd_uidriving = driving_xfer(host->pad_driving[SDIO_SDR104_MODE_CMD]);
		clk_uidriving = driving_xfer(host->pad_driving[SDIO_SDR104_MODE_CLK]);
	}


	if (host->id == SDIO_HOST_ID_1) {
		pad_setdrivingsink(PAD_DS_CGPIO17, cmd_uidriving);
		pad_setdrivingsink(PAD_DS_CGPIO18, data_uidriving);
		pad_setdrivingsink(PAD_DS_CGPIO19, data_uidriving);
		pad_setdrivingsink(PAD_DS_CGPIO20, data_uidriving);
		pad_setdrivingsink(PAD_DS_CGPIO21, data_uidriving);
		return pad_setdrivingsink(PAD_DS_CGPIO16, clk_uidriving);
	} else {
		pad_setdrivingsink(PAD_DS_CGPIO23, cmd_uidriving);
		pad_setdrivingsink(PAD_DS_CGPIO24, data_uidriving);
		pad_setdrivingsink(PAD_DS_CGPIO25, data_uidriving);
		pad_setdrivingsink(PAD_DS_CGPIO26, data_uidriving);
		pad_setdrivingsink(PAD_DS_CGPIO27, data_uidriving);
		return pad_setdrivingsink(PAD_DS_CGPIO22, clk_uidriving);
	}

}

/*
	Reset SDIO host controller.

	@param[in] id   SDIO channel ID
			- @b SDIO_HOST_ID_1: SDIO1
			- @b SDIO_HOST_ID_2: SDIO2

	@return void
*/
void sdiohost_reset(struct mmc_nvt_host *host, uint32_t id)
{
	union SDIO_CMD_REG cmdreg;
	int i = 0;

	cmdreg.reg = sdiohost_getreg(host, id, SDIO_CMD_REG_OFS);
	cmdreg.bit.SDC_RST = 1;
	sdiohost_setreg(host, id, SDIO_CMD_REG_OFS, cmdreg.reg);

	while (1) {
		cmdreg.reg = sdiohost_getreg(host, id, SDIO_CMD_REG_OFS);

		if ((cmdreg.bit.SDC_RST == 0) || (i == RESET_TIMEOUT))
			break;

		i++;
	}

	if (i == RESET_TIMEOUT)
		pr_err("sdc reset timeout\n");
}

/*
	Send SD command to SD bus.

	@param[in] id       SDIO channel ID
			- @b SDIO_HOST_ID_1: SDIO1
			- @b SDIO_HOST_ID_2: SDIO2
	@param[in] cmd      command value
	@param[in] rsptype  response type
			- @b SDIO_HOST_RSP_NONE: no response is required
			- @b SDIO_HOST_RSP_SHORT: need short (32 bits) response
			- @b SDIO_HOST_RSP_LONG: need long (128 bits) response
	@param[in] beniointdetect enable SDIO INT detect after command end
			- @b TRUE: enable SDIO INT detection
			- @b FALSE: keep SDIO INT detection

	@return command result
	- @b SDIO_HOST_CMD_OK: command execution success
	- @b SDIO_HOST_RSP_TIMEOUT: response timeout. no response got from card.
	- @b SDIO_HOST_RSP_CRCFAIL: response CRC fail.
	- @b SDIO_HOST_CMD_FAIL: other fail.
*/
int sdiohost_sendcmd(struct mmc_nvt_host *host, uint32_t id, uint32_t cmd,
SDIO_HOST_RESPONSE rsptype, bool beniointdetect)
{

	union SDIO_CMD_REG cmdreg;
#if (!SDIO_SCATTER_DMA)
	/* Temp solution for FPGA */
	sdiohost_setreg(host, id, 0x1FC, 1);
#endif
	/*cmdreg.reg = 0;*/
	cmdreg.reg = sdiohost_getreg(host, id, SDIO_CMD_REG_OFS);
	cmdreg.bit.CMD_IDX = 0;
	cmdreg.bit.NEED_RSP = 0;
	cmdreg.bit.LONG_RSP = 0;
	cmdreg.bit.RSP_TIMEOUT_TYPE = 0;
	cmdreg.bit.ENABLE_SDIO_INT_DETECT = beniointdetect;
	if (rsptype != SDIO_HOST_RSP_NONE) {
		/* Need response */
		cmdreg.bit.NEED_RSP = 1;

		switch (rsptype) {

		default:
			break;

		case SDIO_HOST_RSP_LONG:
			cmdreg.bit.LONG_RSP = 1;
			break;

		case SDIO_HOST_RSP_SHORT_TYPE2:
			cmdreg.bit.RSP_TIMEOUT_TYPE = 1;
			break;

		case SDIO_HOST_RSP_LONG_TYPE2:
			cmdreg.bit.RSP_TIMEOUT_TYPE = 1;
			cmdreg.bit.LONG_RSP = 1;
			break;
		}
	}
	cmdreg.bit.CMD_IDX = cmd;
	sdiohost_setreg(host, id, SDIO_CMD_REG_OFS, cmdreg.reg);

	if (id >= SDIO_HOST_ID_COUNT)
		return SDIO_HOST_CMD_FAIL;

	/* clear software status */
	/*clr_flg(FLG_ID_SDIO, vsdiohosts[id].flgCommand);*/
	vsdiohosts[id].cmdintsts.reg = 0;

	/* Start command/data transmits */
	cmdreg.bit.CMD_EN = 1;
	sdiohost_setreg(host, id, SDIO_CMD_REG_OFS, cmdreg.reg);

	return SDIO_HOST_CMD_OK;

}


/*
	Get SDIO command result status.

	@param[in] id   SDIO channel ID
			- @b SDIO_HOST_ID_1: SDIO1
			- @b SDIO_HOST_ID_2: SDIO2

	@return Return interrupt status bits for command phase
*/
REGVALUE sdiohost_getcmdresult(uint32_t id)
{
	return vsdiohosts[id].cmdintsts.reg;
}

/*
	Get SDIO data result status.

	@param[in] id   SDIO channel ID
			- @b SDIO_HOST_ID_1: SDIO1
			- @b SDIO_HOST_ID_2: SDIO2

	@return Return interrupt status bits for data phase
*/
REGVALUE sdiohost_getdataresult(uint32_t id)
{
	return vsdiohosts[id].dataintsts.reg;
}

/*
	Set SDIO command argument.

	@param[in] id   SDIO channel ID
			- @b SDIO_HOST_ID_1: SDIO1
			- @b SDIO_HOST_ID_2: SDIO2
	@param[in] arg command argument
			- possible value: 0x0000_0000 ~ 0xFFFF_FFFF

	@return void
*/
void sdiohost_setarg(struct mmc_nvt_host *host, uint32_t id, uint32_t arg)
{
	sdiohost_setreg(host, id, SDIO_ARGU_REG_OFS, arg);
}

/*
	Get SDIO command response.

	@param[in] id   SDIO channel ID
			- @b SDIO_HOST_ID_1: SDIO1
			- @b SDIO_HOST_ID_2: SDIO2
	@param[out] prsp command response from card

	@return void
*/
void sdiohost_getshortrsp(struct mmc_nvt_host *host, uint32_t id,
uint32_t *prsp)
{
	union SDIO_RSP0_REG rspreg;

	rspreg.reg = sdiohost_getreg(host, id, SDIO_RSP0_REG_OFS);
	*prsp = (uint32_t) rspreg.reg;
}

/*
	Get SDIO command long response.

	@param[in] id   SDIO channel ID
			- @b SDIO_HOST_ID_1: SDIO1
			- @b SDIO_HOST_ID_2: SDIO2
	@param[out] prsp3 command response from card (bit 127..96)
	@param[out] prsp2 command response from card (bit 95..64)
	@param[out] prsp1 command response from card (bit 63..32)
	@param[out] prsp0 command response from card (bit 31..0)

	@return void
*/
void sdiohost_getlongrsp(struct mmc_nvt_host *host, uint32_t id,
uint32_t *prsp3, uint32_t *prsp2, uint32_t *prsp1, uint32_t *prsp0)
{
	union SDIO_RSP0_REG rsp0reg;
	union SDIO_RSP1_REG rsp1reg;
	union SDIO_RSP2_REG rsp2reg;
	union SDIO_RSP3_REG rsp3reg;

	rsp0reg.reg = sdiohost_getreg(host, id, SDIO_RSP0_REG_OFS);
	*prsp0 = (uint32_t) rsp0reg.reg;
	rsp1reg.reg = sdiohost_getreg(host, id, SDIO_RSP1_REG_OFS);
	*prsp1 = (uint32_t) rsp1reg.reg;
	rsp2reg.reg = sdiohost_getreg(host, id, SDIO_RSP2_REG_OFS);
	*prsp2 = (uint32_t) rsp2reg.reg;
	rsp3reg.reg = sdiohost_getreg(host, id, SDIO_RSP3_REG_OFS);
	*prsp3 = (uint32_t) rsp3reg.reg;
}

/*
	Check SDIO voltage switch result.

	@param[in] id SDIO channel ID

	@return
		- @b TRUE: voltage switch success
		- @b FALSE: voltage switch fail
*/
bool sdiohost_chkvoltswitch(uint32_t id)
{
	if (vsdiohosts[id].cmdintsts.bit.RSP_VOL_SWITCH_FAIL)
		return FALSE;
	else
		return TRUE;
}


/*
	Enable SDIO interrupt.

	@param[in] id   SDIO channel ID
			- @b SDIO_HOST_ID_1: SDIO1
			- @b SDIO_HOST_ID_2: SDIO2
	@param[in] bits SDIO controller interrupt enable bits

	@return void
*/
void sdiohost_setinten(struct mmc_nvt_host *host, uint32_t id, uint32_t bits)
{
	union SDIO_INT_MASK_REG intenreg;

	intenreg.reg = sdiohost_getreg(host, id, SDIO_INT_MASK_REG_OFS);
	intenreg.reg |= bits;
	sdiohost_setreg(host, id, SDIO_INT_MASK_REG_OFS, intenreg.reg);
}

/*
	Disable SDIO interrupt.

	@param[in] id   SDIO channel ID
			- @b SDIO_HOST_ID_1: SDIO1
			- @b SDIO_HOST_ID_2: SDIO2
	@param[in] bits SDIO controller interrupt enable bits

	@return void
*/
void sdiohost_disinten(struct mmc_nvt_host *host, uint32_t id, uint32_t bits)
{
	union SDIO_INT_MASK_REG intenreg;

	intenreg.reg = sdiohost_getreg(host, id, SDIO_INT_MASK_REG_OFS);
	intenreg.reg &= ~bits;
	sdiohost_setreg(host, id, SDIO_INT_MASK_REG_OFS, intenreg.reg);
}

/*
	Set SDIO bus width.

	@param[in] id       SDIO channel ID
			- @b SDIO_HOST_ID_1: SDIO1
			- @b SDIO_HOST_ID_2: SDIO2
	@param[in] Width    SDIO controller bus width
			- @b SDIO_BUS_WIDTH1: 1 bit data bus
			- @b SDIO_BUS_WIDTH4: 4 bits data bus
			- @b SDIO_BUS_WIDTH8: 8 bits data bus

	@return void
*/
void sdiohost_setbuswidth(struct mmc_nvt_host *host, uint32_t id,
uint32_t width)
{
	union SDIO_BUS_WIDTH_REG widthreg;

	widthreg.reg = 0;
	widthreg.bit.BUS_WIDTH = width;
	sdiohost_setreg(host, id, SDIO_BUS_WIDTH_REG_OFS, widthreg.reg);
}

/*
	Get SDIO bus width.

	@param[in] id   SDIO channel ID
			- @b SDIO_HOST_ID_1: SDIO1
			- @b SDIO_HOST_ID_2: SDIO2

	@return SDIO controller bus Width
			- @b SDIO_BUS_WIDTH1
			- @b SDIO_BUS_WIDTH4
			- @b SDIO_BUS_WIDTH8
*/
uint32_t sdiohost_getbuswidth(struct mmc_nvt_host *host, uint32_t id)
{
	union SDIO_BUS_WIDTH_REG widthreg;

	widthreg.reg = sdiohost_getreg(host, id, SDIO_BUS_WIDTH_REG_OFS);

	return widthreg.bit.BUS_WIDTH;
}

/*
	Set SDIO bus clock.

	@param[in] id       SD host ID
			- @b SDIO_HOST_ID_1: SDIO1
			- @b SDIO_HOST_ID_2: SDIO2
	@param[in] uiclock  SD bus clock in Hz
	@param[out] ns  ns one cycle

	@return void
*/
void sdiohost_setbusclk(struct mmc_nvt_host *host, uint32_t id,
uint32_t uiclock, uint32_t *ns)
{
	clk_set_rate(host->clk, uiclock);

	if (ns)
		*ns = (1000000) / (uiclock/1000);
}

/*
	Get SDIO bus clock.

	@param[in] id       SD host ID
			- @b SDIO_HOST_ID_1: SDIO1
			- @b SDIO_HOST_ID_2: SDIO2

	@return unit of Hz
*/
uint32_t sdiohost_getbusclk(struct mmc_nvt_host *host, uint32_t id)
{
	return clk_get_rate(host->clk);
}

/*
	Get SDIO controller block size.

	@param[in] id   SDIO channel ID
			- @b SDIO_HOST_ID_1: SDIO1
			- @b SDIO_HOST_ID_2: SDIO2

	@return data size per block (unit: byte) (0x0001 ~ 0xFFFF)
*/
uint32_t sdiohost_getblksize(struct mmc_nvt_host *host, uint32_t id)
{
	union SDIO_DATA_CTRL_REG datactrlreg;

	datactrlreg.reg = sdiohost_getreg(host, id, SDIO_DATA_CTRL_REG_OFS);
	return datactrlreg.bit.BLK_SIZE;
}

/*
	Set SDIO controller block size.

	@param[in] id   SDIO channel ID
			- @b SDIO_HOST_ID_1: SDIO1
			- @b SDIO_HOST_ID_2: SDIO2
	@param[in] size data size per block (unit: byte)
			- possible value: 0x0001 ~ 0xFFFF

	@return void
*/
void sdiohost_setblksize(struct mmc_nvt_host *host, uint32_t id, uint32_t size)
{
	union SDIO_DATA_CTRL_REG datactrlreg;

	datactrlreg.reg = sdiohost_getreg(host, id, SDIO_DATA_CTRL_REG_OFS);
	datactrlreg.bit.BLK_SIZE = size;
	sdiohost_setreg(host, id, SDIO_DATA_CTRL_REG_OFS, datactrlreg.reg);
}

/*
	Set SDIO controller data timeout.

	@param[in] id       SDIO channel ID
			- @b SDIO_HOST_ID_1: SDIO1
			- @b SDIO_HOST_ID_2: SDIO2
	@param[in] timeout  time out value between data blocks (unit: SD clock)

	@return void
*/
void sdiohost_setdatatimeout(struct mmc_nvt_host *host, uint32_t id,
uint32_t timeout)
{
	union SDIO_DATA_TIMER_REG timerreg;

	timerreg.bit.TIMEOUT = timeout;
	sdiohost_setreg(host, id, SDIO_DATA_TIMER_REG_OFS, timerreg.reg);
}

/*
	Reset SDIO controller data state machine.

	@param[in] id       SDIO channel ID
			- @b SDIO_HOST_ID_1: SDIO1
			- @b SDIO_HOST_ID_2: SDIO2

	@return void
*/
void sdiohost_resetdata(struct mmc_nvt_host *host, uint32_t id)
{
	union SDIO_DATA_CTRL_REG    datactrlreg;
	union SDIO_FIFO_CONTROL_REG fifoctrlreg;
	union SDIO_DLY1_REG dlyreg;
	union SDIO_PHY_REG  phyreg;
	union SDIO_PHY_REG  phyreg_read;
	union SDIO_FIFO_SWITCH_REG  fifoswitch;
	int i = 0;

	/* //#NT#Fix SDIO data state machine abnormal when DATA CRC/Timeout
	occurs before FIFO count complete */
	union SDIO_STATUS_REG	stsreg;

	/* SDIO bug: force to clear data end status to exit
	waiting data end state*/
	stsreg.reg          = 0;
	stsreg.bit.DATA_END = 1;
	sdiohost_setreg(host, id, SDIO_STATUS_REG_OFS, stsreg.reg);


	fifoctrlreg.reg = 0;
	sdiohost_setreg(host, id, SDIO_FIFO_CONTROL_REG_OFS, fifoctrlreg.reg);

	while (1) {
		fifoctrlreg.reg = sdiohost_getreg(host, id,
			SDIO_FIFO_CONTROL_REG_OFS);
		if (fifoctrlreg.bit.FIFO_EN == 0)
			break;
	}

	datactrlreg.reg = sdiohost_getreg(host, id, SDIO_DATA_CTRL_REG_OFS);
	datactrlreg.bit.DATA_EN = 0;
	sdiohost_setreg(host, id, SDIO_DATA_CTRL_REG_OFS, datactrlreg.reg);

	/* Fix SDIO data state machine abnormal when DATA
	CRC/Timeout occurs before FIFO count complete */
	/* Do software reset to reset SD state machine */
	sdiohost_reset(host, id);

	/* patch begin for sd write hang-up or write byte access error */
	fifoswitch.reg = sdiohost_getreg(host, id, SDIO_FIFO_SWITCH_REG_OFS);
	fifoswitch.bit.FIFO_SWITCH_DLY = 1;
	sdiohost_setreg(host, id, SDIO_FIFO_SWITCH_REG_OFS, fifoswitch.reg);
	/* patch end for sd write hang-up or write byte access error */

	phyreg.reg = sdiohost_getreg(host, id, SDIO_PHY_REG_OFS);
	phyreg.bit.PHY_SW_RST = 1;
	sdiohost_setreg(host, id, SDIO_PHY_REG_OFS, phyreg.reg);
	while (1)
	{
		phyreg_read.reg = sdiohost_getreg(host, id, SDIO_PHY_REG_OFS);
		if ((phyreg_read.bit.PHY_SW_RST == 0) || (i == RESET_TIMEOUT))
			break;

		i++;
	}

	if (i == RESET_TIMEOUT)
		pr_err("reset timeout\n");

	dlyreg.reg = sdiohost_getreg(host, id, SDIO_DLY1_REG_OFS);
	dlyreg.bit.DATA_READ_DLY = 2;
	dlyreg.bit.DET_READ_DLY = 2;
	sdiohost_setreg(host, id, SDIO_DLY1_REG_OFS, dlyreg.reg);

	phyreg.reg = sdiohost_getreg(host, id, SDIO_PHY_REG_OFS);
	phyreg.bit.BLK_FIFO_EN = 1;
	sdiohost_setreg(host, id, SDIO_PHY_REG_OFS, phyreg.reg);

}

void sdiohost_delayd(struct mmc_nvt_host *host, uint32_t uid)
{
	uint32_t i, uidummy;

	for (i = uid; i; i--)
		uidummy = sdiohost_getreg(host, 0, SDIO_CMD_REG_OFS);

}

void sdiohost_waitfifoempty(struct mmc_nvt_host *host, uint32_t id)
{
	union SDIO_FIFO_STATUS_REG fifostsreg;
	uint32_t read0, read1;

	read0 = 0;
	while (1) {
		fifostsreg.reg = sdiohost_getreg(host, id,
			SDIO_FIFO_STATUS_REG_OFS);
		read1 = fifostsreg.bit.FIFO_EMPTY;
		if (read0 & read1)
			break;
		else
			read0 = read1;
	}
}
void sdiohost_clrfifoen(struct mmc_nvt_host *host, uint32_t id)
{
	union SDIO_FIFO_CONTROL_REG fifoctrlreg;

	fifoctrlreg.reg = sdiohost_getreg(host, id, SDIO_FIFO_CONTROL_REG_OFS);
	fifoctrlreg.bit.FIFO_EN = 0;
	sdiohost_setreg(host, id, SDIO_FIFO_CONTROL_REG_OFS, fifoctrlreg.reg);
}
uint32_t sdiohost_getfifodir(struct mmc_nvt_host *host, uint32_t id)
{
	union SDIO_FIFO_CONTROL_REG fifoctrlreg;

	fifoctrlreg.reg = sdiohost_getreg(host, id, SDIO_FIFO_CONTROL_REG_OFS);

	return fifoctrlreg.bit.FIFO_DIR;
}

/*
	Setup SDIO controller data transfer in DMA mode.

	@param[in] id           SDIO channel ID
			- @b SDIO_HOST_ID_1: SDIO1
			- @b SDIO_HOST_ID_2: SDIO2
	@param[in] uidmaaddress buffer DRAM address
			- possible value: 0x000_0000 ~ 0xFFF_FFFF
	@param[in] uidatalength total transfer length
			- possible value: 0x000_0001 ~ 0x3FF_FFFF
	@param[in] bisread      read/write mode
			- @b TRUE: indicate data read transfer
			- @b FALSE: indicate data write transfer

	@return void
*/
void sdiohost_setupdatatransferdma(struct mmc_nvt_host *host, uint32_t id,
uint32_t uidmaaddress, uint32_t uidatalength, bool bisread, uint32_t *vuisdio_destab)
{
	union SDIO_DATA_CTRL_REG datactrlreg;
	union SDIO_DATA_LENGTH_REG datalenreg;
	union SDIO_FIFO_CONTROL_REG fifoctrlreg;
#if (SDIO_SCATTER_DMA)
	union SDIO_DMA_DES_START_ADDR_REG dmadesaddrreg;
	union SDIO_DES_LINE_REG dmadeslinereg;
	union SDIO_DES_LINE1_REG dmadesline1reg;
	union SDIO_DES_LINE2_REG dmadesline2reg;
	/*uint32_t i;*/
#else
	union SDIO_DMA_START_ADDR_REG dmaaddrreg;
#endif
	uint32_t uibusclk;


	/* for debug */
	/*fifoctrlreg.reg = sdiohost_getreg(id, SDIO_FIFO_CONTROL_REG_OFS);
	if (fifoctrlreg.bit.FIFO_EN == 1) {
		fifoctrlreg.reg = 0;
		sdiohost_setreg(id, SDIO_FIFO_CONTROL_REG_OFS, fifoctrlreg.reg);
	}*/
	/* for debug end */

	/* dummy read for patch */
	sdiohost_delayd(host, 2);

	uibusclk = sdiohost_getbusclk(host, id);

	if (uibusclk >= 48000000)
		sdiohost_delayd(host, 3);
	else if ((uibusclk >= 24000000) && (uibusclk < 48000000))
		sdiohost_delayd(host, 6);
	else if ((uibusclk >= 12000000) && (uibusclk < 24000000))
		sdiohost_delayd(host, 9);
	else
		sdiohost_delayd(host, 21);

	/* patch for sd fifo bug end */

	datactrlreg.reg = sdiohost_getreg(host, id, SDIO_DATA_CTRL_REG_OFS);
	/* multiple read => disable SDIO INT detection after transfer end */
	if (bisread && (uidatalength > datactrlreg.bit.BLK_SIZE))
		datactrlreg.bit.DIS_SDIO_INT_PERIOD = 1;
	else
		datactrlreg.bit.DIS_SDIO_INT_PERIOD = 0;

	/*move data en after fifo en*/
	/*datactrlreg.bit.DATA_EN = 1;*/
	sdiohost_setreg(host, id, SDIO_DATA_CTRL_REG_OFS, datactrlreg.reg);

#if (SDIO_SCATTER_DMA)
	if (vsdio_seg_en[id] == FALSE) {
		dmadeslinereg.reg = 0;
		dmadesline1reg.reg = 0;
		dmadesline2reg.reg = 0;

		dmadeslinereg.bit.DMA_DES_VALID = 1;
		dmadeslinereg.bit.DMA_DES_END = 1;
		dmadeslinereg.bit.DMA_DES_ACT = 2;

		dmadesline1reg.bit.DMA_DES_DATA_LEN = uidatalength;
		dmadesline2reg.bit.DMA_DES_DATA_ADDR = uidmaaddress;

		vuisdio_destab[SDIO_DES_LINE_REG_OFS] =
			(uint32_t)dmadeslinereg.reg;
		vuisdio_destab[SDIO_DES_LINE1_REG_OFS>>2] =
			(uint32_t)dmadesline1reg.reg;
		vuisdio_destab[SDIO_DES_LINE2_REG_OFS>>2] =
			(uint32_t)dmadesline2reg.reg;
	}

	dmadesaddrreg.reg = 0;
	dmadesaddrreg.bit.DES_ADDR = virt_to_phys((uint32_t*)&vuisdio_destab[0]);
	sdiohost_setreg(host, id, SDIO_DMA_DES_START_ADDR_REG_OFS,
		dmadesaddrreg.reg);

	datalenreg.reg = 0;
	datalenreg.bit.LENGTH = uidatalength;
	sdiohost_setreg(host, id, SDIO_DATA_LENGTH_REG_OFS, datalenreg.reg);

	fifoctrlreg.reg = 0;

	if (vsdio_seg_en[id] == FALSE) {
		/* Flush cache in data DMA*/
		if (!bisread) {
			dma_flushwritecache(mmc_dev(host->mmc), (void *)uidmaaddress, uidatalength);
			fifoctrlreg.bit.FIFO_DIR = 1;
		} else {
			dma_flushreadcache(mmc_dev(host->mmc), (void *)uidmaaddress, uidatalength);
		}

		/* Flush cache in Des*/
		dma_flushbidircache(mmc_dev(host->mmc), (void *)&vuisdio_destab[0],
		SDIO_DES_WORD_SIZE<<2);
	} else {
		#if 0
		for (i = 0; i < vsdio_seg_num[id]; i++) {
			/* Flush cache in data DMA*/
			if (!bisread) {
				dma_flushwritecache((void *)(mmc_dev(host->mmc), (vuisdio_destab
				[(i * SDIO_DES_WORD_SIZE) +
				(SDIO_DES_LINE2_REG_OFS>>2)])),
				vuisdio_destab[(i * SDIO_DES_WORD_SIZE +
				SDIO_DES_LINE1_REG_OFS)>>2]);
				fifoctrlreg.bit.FIFO_DIR = 1;
			} else {
				dma_flushreadcache((void *)(mmc_dev(host->mmc), (vuisdio_destab
				[(i * SDIO_DES_WORD_SIZE) +
				(SDIO_DES_LINE2_REG_OFS>>2)])),
				vuisdio_destab[(i * SDIO_DES_WORD_SIZE +
				SDIO_DES_LINE1_REG_OFS)>>2]);
			}

		}
		#else
		if (!bisread)
			fifoctrlreg.bit.FIFO_DIR = 1;
		else
			fifoctrlreg.bit.FIFO_DIR = 0;
		#endif

		/* Flush cache in Des*/
		dma_flushbidircache(mmc_dev(host->mmc), (void *)(&vuisdio_destab[0]),
		(SDIO_DES_WORD_SIZE<<2)*SDIO_DES_TABLE_NUM);
	}
#else
	dmaaddrreg.reg = 0;
	dmaaddrreg.bit.DRAM_ADDR = virt_to_phys((uint32_t *)uidmaaddress);
	sdiohost_setreg(host, id, SDIO_DMA_START_ADDR_REG_OFS, dmaaddrreg.reg);

	datalenreg.reg = 0;
	datalenreg.bit.LENGTH = uidatalength;
	sdiohost_setreg(host, id, SDIO_DATA_LENGTH_REG_OFS, datalenreg.reg);

	fifoctrlreg.reg = 0;

	/* Flush cache in DMA mode*/
	if (!bisread) {
		dma_flushwritecache((void *)uidmaaddress, uidatalength);
		fifoctrlreg.bit.FIFO_DIR = 1;
	} else {
		dma_flushreadcache((void *)uidmaaddress, uidatalength);
	}
#endif

	fifoctrlreg.bit.FIFO_MODE = 1;
	sdiohost_setreg(host, id, SDIO_FIFO_CONTROL_REG_OFS, fifoctrlreg.reg);

	datactrlreg.reg = sdiohost_getreg(host, id, SDIO_DATA_CTRL_REG_OFS);
	datactrlreg.bit.DATA_EN = 1;
	sdiohost_setreg(host, id, SDIO_DATA_CTRL_REG_OFS, datactrlreg.reg);

	fifoctrlreg.bit.FIFO_EN = 1;
	sdiohost_setreg(host, id, SDIO_FIFO_CONTROL_REG_OFS, fifoctrlreg.reg);
}

/*
	Setup SDIO controller data transfer in PIO mode.

	@param[in] id           SDIO channel ID
			- @b SDIO_HOST_ID_1: SDIO1
			- @b SDIO_HOST_ID_2: SDIO2
	@param[in] uidmaaddress buffer DRAM address
			- possible value: 0x000_0000 ~ 0xFFF_FFFF
	@param[in] uidatalength total transfer length
			- possible value: 0x000_0001 ~ 0x3FF_FFFF
	@param[in] bisread      read/write mode
			- @b TRUE: indicate data read transfer
			- @b FALSE: indicate data write transfer

	@return void
*/
void sdiohost_setupdatatransferpio(struct mmc_nvt_host *host, uint32_t id,
uint32_t uidmaaddress, uint32_t uidatalength, bool bisread)
{
	union SDIO_DATA_CTRL_REG datactrlreg;
	union SDIO_DATA_LENGTH_REG datalenreg;
	union SDIO_FIFO_CONTROL_REG fifoctrlreg;
#if (!SDIO_SCATTER_DMA)
	union SDIO_DMA_START_ADDR_REG dmaaddrreg;
#endif
	uint32_t uibusclk;

	/* dummy read for patch */
	sdiohost_delayd(host, 2);

	uibusclk = sdiohost_getbusclk(host, id);

	if (uibusclk >= 48000000)
		sdiohost_delayd(host, 3);
	else if ((uibusclk >= 24000000) && (uibusclk < 48000000))
		sdiohost_delayd(host, 6);
	else if ((uibusclk >= 12000000) && (uibusclk < 24000000))
		sdiohost_delayd(host, 9);
	else
		sdiohost_delayd(host, 21);

	/* patch for sd fifo bug end */


	datactrlreg.reg = sdiohost_getreg(host, id, SDIO_DATA_CTRL_REG_OFS);
	/* multiple read => disable SDIO INT detection after transfer end */
	if (bisread && (uidatalength > datactrlreg.bit.BLK_SIZE))
		datactrlreg.bit.DIS_SDIO_INT_PERIOD = 1;
	else
		datactrlreg.bit.DIS_SDIO_INT_PERIOD = 0;

	datactrlreg.bit.DATA_EN = 1;
	sdiohost_setreg(host, id, SDIO_DATA_CTRL_REG_OFS, datactrlreg.reg);


#if (!SDIO_SCATTER_DMA)
	dmaaddrreg.reg = 0;
	sdiohost_setreg(host, id, SDIO_DMA_START_ADDR_REG_OFS, dmaaddrreg.reg);
#endif

	datalenreg.reg = 0;
	datalenreg.bit.LENGTH = uidatalength;
	sdiohost_setreg(host, id, SDIO_DATA_LENGTH_REG_OFS, datalenreg.reg);

	fifoctrlreg.reg = 0;

	if (!bisread)
		fifoctrlreg.bit.FIFO_DIR = 1;

	sdiohost_setreg(host, id, SDIO_FIFO_CONTROL_REG_OFS, fifoctrlreg.reg);

	fifoctrlreg.bit.FIFO_EN = 1;
	sdiohost_setreg(host, id, SDIO_FIFO_CONTROL_REG_OFS, fifoctrlreg.reg);
}

/*
	Write SDIO data blocks.


	@note This function should only be called in PIO mode.

	@param[in] id           SDIO channel ID
			- @b SDIO_HOST_ID_1: SDIO1
			- @b SDIO_HOST_ID_2: SDIO2
	@param[in] pbuf         buffer DRAM address
	@param[in] uiLength     total length (block alignment)

	@return
			- @b E_OK: success
			- @b E_SYS: data CRC or data timeout error
*/
int sdiohost_writeblock(struct mmc_nvt_host *host, uint32_t id, uint8_t *pbuf,
uint32_t uilength)
{
	uint32_t  uiwordcount, i, *pbufword;
	uint32_t  uifullcount, uiremaincount;
	bool    bwordalignment;
	union SDIO_DATA_PORT_REG    datareg;
	union SDIO_FIFO_STATUS_REG  fifostsreg;
	union SDIO_STATUS_REG       stsreg;

	uiwordcount     = (uilength + sizeof(uint32_t) - 1) / sizeof(uint32_t);
	uifullcount     = uiwordcount / SDIO_HOST_DATA_FIFO_DEPTH;
	uiremaincount   = uiwordcount % SDIO_HOST_DATA_FIFO_DEPTH;
	pbufword        = (uint32_t *)pbuf;

	if ((uint32_t)pbuf & 0x3)
		bwordalignment = FALSE;
	else
		bwordalignment = TRUE;

	while (uifullcount) {
		fifostsreg.reg = sdiohost_getreg(host, id,
			SDIO_FIFO_STATUS_REG_OFS);

		if (fifostsreg.bit.FIFO_EMPTY) {
			if (bwordalignment == TRUE) {
				/* Word alignment*/
				for (i = SDIO_HOST_DATA_FIFO_DEPTH; i; i--) {
					sdiohost_setreg(host, id,
					SDIO_DATA_PORT_REG_OFS, *pbufword++);
				}
			} else {
				/* Not word alignment*/
				for (i = SDIO_HOST_DATA_FIFO_DEPTH; i; i--) {
					datareg.reg = *pbuf++;
					datareg.reg |= (*pbuf++) << 8;
					datareg.reg |= (*pbuf++) << 16;
					datareg.reg |= (*pbuf++) << 24;

					sdiohost_setreg(host, id,
					SDIO_DATA_PORT_REG_OFS, datareg.reg);
				}
			}

			uifullcount--;
		}

		stsreg = vsdiohosts[id].dataintsts;

		if (stsreg.bit.DATA_CRC_FAIL || stsreg.bit.DATA_TIMEOUT)
			return E_SYS;

	}

	if (uiremaincount) {
		while (1) {

			fifostsreg.reg = sdiohost_getreg(host, id,
			SDIO_FIFO_STATUS_REG_OFS);

			if (fifostsreg.bit.FIFO_EMPTY)
				break;

			stsreg = vsdiohosts[id].dataintsts;

			if (stsreg.bit.DATA_CRC_FAIL || stsreg.bit.DATA_TIMEOUT)
				return E_SYS;
		}

		if (bwordalignment == TRUE) {
			/* Word alignment*/
			for (i = uiremaincount; i; i--) {
				sdiohost_setreg(host, id,
				SDIO_DATA_PORT_REG_OFS, *pbufword++);
			}
		} else {
			/* Not word alignment*/
			for (i = uiremaincount; i; i--) {
				datareg.reg = *pbuf++;
				datareg.reg |= (*pbuf++) << 8;
				datareg.reg |= (*pbuf++) << 16;
				datareg.reg |= (*pbuf++) << 24;

				sdiohost_setreg(host, id,
				SDIO_DATA_PORT_REG_OFS, datareg.reg);
			}
		}
	}

	return E_OK;
}

/*
	Read SDIO data blocks.

	@note This function should only be called in PIO mode.

	@param[in] id           SDIO channel ID
			- @b SDIO_HOST_ID_1: SDIO1
			- @b SDIO_HOST_ID_2: SDIO2
	@param[out] pbuf        buffer DRAM address
	@param[in] uiLength     total length (block alignment)

	@return
		- @b E_OK: success
		- @b E_SYS: data CRC or data timeout error
*/
int sdiohost_readblock(struct mmc_nvt_host *host, uint32_t id, uint8_t *pbuf,
uint32_t uilength)
{
	uint32_t  uiwordcount, i, *pbufword;
	uint32_t  uifullcount, uiremaincount;
	bool    bwordalignment;
	union SDIO_DATA_PORT_REG    datareg;
	union SDIO_FIFO_STATUS_REG  fifostsreg;
	union SDIO_STATUS_REG       stsreg;

	uiwordcount     = (uilength + sizeof(uint32_t) - 1) / sizeof(uint32_t);
	uifullcount     = uiwordcount / SDIO_HOST_DATA_FIFO_DEPTH;
	uiremaincount   = uiwordcount % SDIO_HOST_DATA_FIFO_DEPTH;
	pbufword        = (uint32_t *)pbuf;

	if ((uint32_t)pbuf & 0x3)
		bwordalignment = FALSE;
	else
		bwordalignment = TRUE;

	while (uifullcount) {
		fifostsreg.reg = sdiohost_getreg(host, id,
			SDIO_FIFO_STATUS_REG_OFS);

		if (fifostsreg.bit.FIFO_FULL) {
			if (bwordalignment == TRUE) {
				/* Word alignment*/
				for (i = SDIO_HOST_DATA_FIFO_DEPTH; i; i--) {
					*pbufword++ = sdiohost_getreg(host, id,
					SDIO_DATA_PORT_REG_OFS);
				}
			} else {
				/* Not word alignment*/
				for (i = SDIO_HOST_DATA_FIFO_DEPTH; i; i--) {
					datareg.reg = sdiohost_getreg(host, id,
					SDIO_DATA_PORT_REG_OFS);

					*pbuf++ = datareg.reg & 0xFF;
					*pbuf++ = (datareg.reg>>8) & 0xFF;
					*pbuf++ = (datareg.reg>>16) & 0xFF;
					*pbuf++ = (datareg.reg>>24) & 0xFF;
				}
			}

			uifullcount--;
		}

		/*stsreg = vsdiohosts[id].dataintsts;*/
		stsreg.reg = sdiohost_getreg(host, id,
		SDIO_STATUS_REG_OFS);

		if (stsreg.bit.DATA_CRC_FAIL || stsreg.bit.DATA_TIMEOUT)
			return E_SYS;
	}

	if (uiremaincount) {
		while (1) {
			fifostsreg.reg = sdiohost_getreg(host, id,
			SDIO_FIFO_STATUS_REG_OFS);

			if (fifostsreg.bit.FIFO_CNT == uiremaincount)
				break;

			stsreg = vsdiohosts[id].dataintsts;
			if (stsreg.bit.DATA_CRC_FAIL || stsreg.bit.DATA_TIMEOUT)
				return E_SYS;
		}

		if (bwordalignment == TRUE) {
			/* Word alignment*/
			for (i = uiremaincount; i; i--) {
				*pbufword++ = sdiohost_getreg(host, id,
				SDIO_DATA_PORT_REG_OFS);
			}
		} else {
			/* Not word alignment*/
			for (i = uiremaincount; i; i--) {
				datareg.reg = sdiohost_getreg(host, id,
				SDIO_DATA_PORT_REG_OFS);

				*pbuf++ = datareg.reg & 0xFF;
				*pbuf++ = (datareg.reg>>8) & 0xFF;
				*pbuf++ = (datareg.reg>>16) & 0xFF;
				*pbuf++ = (datareg.reg>>24) & 0xFF;
			}
		}
	}

	return E_OK;
}


uint32_t sdiohost_setiointen(struct mmc_nvt_host *host, uint32_t id, bool ben)
{
	union SDIO_INT_MASK_REG intenreg;

	intenreg.reg = sdiohost_getreg(host, id, SDIO_INT_MASK_REG_OFS);
	intenreg.bit.SDIO_INT_INT_EN = ben;
	sdiohost_setreg(host, id, SDIO_INT_MASK_REG_OFS, intenreg.reg);
	return TRUE;
}

bool sdiohost_getiointen(struct mmc_nvt_host *host, uint32_t id)
{
	union SDIO_INT_MASK_REG intenreg;

	intenreg.reg = sdiohost_getreg(host, id, SDIO_INT_MASK_REG_OFS);
	if (intenreg.bit.SDIO_INT_INT_EN)
		return TRUE;
	else
		return FALSE;
}
/** ********************************************************************


  Private(in driver layer) SDIO host controller functions


***********************************************************************/

#if (SDIO_HOST_ID_COUNT > 1)
/*
    Get SDIO controller register.

    @param[in] id           SDIO channel ID
					- @b SDIO_HOST_ID_1: SDIO
					- @b SDIO_HOST_ID_2: SDIO2
    @param[in] offset       register offset in SDIO controller (word alignment)

    @return register value
*/
static REGVALUE sdiohost_getreg(struct mmc_nvt_host *host, uint32_t id,
uint32_t offset)
{
	if (id == SDIO_HOST_ID_1)
		return nvt_readl(host->base + offset);
	else if (id == SDIO_HOST_ID_2)
		return nvt_readl(host->base + offset);
	else
		return nvt_readl(host->base + offset);
}
#endif

#if (SDIO_HOST_ID_COUNT > 1)
/*
	Set SDIO controller register.

	@param[in] id	DIO channel ID
			- @b SDIO_HOST_ID_1: SDIO1
			- @b SDIO_HOST_ID_2: SDIO2
	@param[in] offset   offset in SDIO controller (word alignment)
	@param[in] value    register value

	@return void
*/
static void sdiohost_setreg(struct mmc_nvt_host *host, uint32_t id,
uint32_t offset, REGVALUE value)
{
	if (id == SDIO_HOST_ID_1)
		nvt_writel(value, host->base + offset);
	else if (id == SDIO_HOST_ID_2)
		nvt_writel(value, host->base + offset);
	else
		nvt_writel(value, host->base + offset);
}
#endif

/*uint32_t Flag = 0;*/
#include <linux/delay.h>

void sdiohost_delay(uint32_t uius)
{
	udelay(uius);
}

int sdiohost_sendsdcmd(struct mmc_nvt_host *host, uint32_t id, uint32_t cmdpart,
uint32_t param)
{
	int status;
	bool benintdetect = FALSE;
	SDIO_HOST_RESPONSE rsptype = SDIO_HOST_RSP_NONE;
#if 0
	pr_info("CMD%d arg %x %x\r\n",
	cmdpart&(~(SDIO_CMD_REG_APP_CMD | SDIO_CMD_REG_VOLTAGE_SWITCH_DETECT
	| SDIO_CMD_REG_ABORT | SDIO_CMD_REG_NEED_RSP | SDIO_CMD_REG_LONG_RSP
	| SDIO_CMD_REG_RSP_TYPE2)),
	param, cmdpart);
#endif
	if ((cmdpart & SDIO_CMD_REG_LONG_RSP) == SDIO_CMD_REG_LONG_RSP) {
		if (cmdpart & SDIO_CMD_REG_RSP_TYPE2)
			rsptype = SDIO_HOST_RSP_LONG_TYPE2;
		else
			rsptype = SDIO_HOST_RSP_LONG;
	} else if (cmdpart & SDIO_CMD_REG_VOLTAGE_SWITCH_DETECT) {
		rsptype = SDIO_HOST_RSP_VOLT_DETECT;
	} else if (cmdpart & SDIO_CMD_REG_NEED_RSP) {

		if (cmdpart & SDIO_CMD_REG_RSP_TYPE2)
			rsptype = SDIO_HOST_RSP_SHORT_TYPE2;
		else
			rsptype = SDIO_HOST_RSP_SHORT;
	}

	if (cmdpart & SDIO_CMD_REG_ABORT)
		benintdetect = TRUE;

	sdiohost_setarg(host, id, param);
	/*pr_info("bEnIntDetect %d\r\n",bEnIntDetect);*/

	status = sdiohost_sendcmd(host, id, cmdpart&SDIO_CMD_REG_INDEX,
	rsptype, benintdetect);

	return E_OK;
}

int sdiohost_open(struct mmc_nvt_host *host, uint32_t id, int voltage_switch)
{
	sdiohost_setpaddriving(host, SDIO_MODE_DS);

	sdiohost_delay_chain(host, 0x200000, SDIO_DLY_DS_DEFAULT);

	sdiohost_resetdata(host, id);

	/* Pass TG1.1-15: init freq should <=400Khz. */
	/* Set to 399KHz to guarantee pass TG1.1-15 */
	/*sdiohost_setbusclk(id, 399000, &ns);*/

	/* Delay 1 ms (SD spec) after clock is outputted. */
	/* (Delay 1024 us to reduce code size) */
	sdiohost_delay(1024);

	/*sdioHost_setClkGating(id, bClkGating);*/
	/* restore clock gating*/

	/* Interrupt Enable */
	sdiohost_setinten(host, id,
		SDIO_STATUS_REG_DATA_TIMEOUT |
		SDIO_STATUS_REG_DATA_CRC_FAIL|
		SDIO_STATUS_REG_DATA_CRC_OK |
		SDIO_STATUS_REG_DATA_END |
		SDIO_STATUS_REG_RSP_CRC_FAIL |
		SDIO_STATUS_REG_RSP_TIMEOUT |
		SDIO_STATUS_REG_RSP_CRC_OK |
		SDIO_STATUS_REG_CMD_SEND);

	sdiohost_setdatatimeout(host, id, 0x10000000);

	return E_OK;
}


void sdiohost_setstatus(struct mmc_nvt_host *host, uint32_t id,
uint32_t status)
{
	sdiohost_setreg(host, id, SDIO_STATUS_REG_OFS, status);
}

uint32_t sdiohost_getstatus(struct mmc_nvt_host *host, uint32_t id)
{
	return sdiohost_getreg(host, id, SDIO_STATUS_REG_OFS);
}

void sdiohost_setpower(struct mmc_nvt_host *host, SDIO_HOST_SETPOWER_VOL vol)
{
	if (host->id == SDIO_HOST_ID_2) {
		if (vol == SDIO_HOST_SETPOWER_VOL_3P3)
			*(uint32_t*) 0xFD030080 = 0x10;
		else if (vol == SDIO_HOST_SETPOWER_VOL_1P8)
			*(uint32_t*) 0xFD030080 = 0x11;
		else
			*(uint32_t*) 0xFD030080 = 0x0;
	} else if (host->id == SDIO_HOST_ID_1) {
		if (vol == SDIO_HOST_SETPOWER_VOL_3P3)
			*(uint32_t*) 0xFD030084 = 0x10;
		else if (vol == SDIO_HOST_SETPOWER_VOL_1P8)
			*(uint32_t*) 0xFD030084 = 0x11;
		else
			*(uint32_t*) 0xFD030084 = 0x0;
	}
}

int sdiohost_getpower(struct mmc_nvt_host *host)
{
	if (host->id == SDIO_HOST_ID_2)
		return (*(uint32_t*) 0xFD030080 & 0x1);
	else if (host->id == SDIO_HOST_ID_1)
		return (*(uint32_t*) 0xFD030084 & 0x1);
	else
		return 1;
}

static int sdiohost_getpower_enable(struct mmc_nvt_host *host)
{
	if (host->id == SDIO_HOST_ID_2)
		return (*(uint32_t*) 0xFD030080 & 0x10);
	else if (host->id == SDIO_HOST_ID_1)
		return (*(uint32_t*) 0xFD030084 & 0x10);
	else
		return 1;
}

void sdiohost_power_switch(struct mmc_nvt_host *host, bool enable)
{
	if (enable) {
		if (!sdiohost_getpower_enable(host))
			sdiohost_setpower(host, SDIO_HOST_SETPOWER_VOL_3P3);
	} else
		sdiohost_setpower(host, SDIO_HOST_SETPOWER_VOL_0);
}

void sdiohost_power_cycle(struct mmc_nvt_host *host)
{
	gpio_set_value(host->cp_gpio, !host->cp_gpio_value);
	sdiohost_setpower(host, SDIO_HOST_SETPOWER_VOL_0);
	*(u32*) 0xFD0100A0 |= 0x3F0000;
	*(u32*) 0xFD030004 &= ~0xFFF;
	*(u32*) 0xFD030004 |= 0x555;
	gpio_direction_output(C_GPIO(16), 0);
	gpio_direction_output(C_GPIO(17), 0);
	gpio_direction_output(C_GPIO(18), 0);
	gpio_direction_output(C_GPIO(19), 0);
	gpio_direction_output(C_GPIO(20), 0);
	gpio_direction_output(C_GPIO(21), 0);
	mdelay(1000);
	gpio_direction_input(C_GPIO(16));
	gpio_direction_input(C_GPIO(17));
	gpio_direction_input(C_GPIO(18));
	gpio_direction_input(C_GPIO(19));
	gpio_direction_input(C_GPIO(20));
	gpio_direction_input(C_GPIO(21));
	*(u32*) 0xFD0100A0 &= ~0x3F0000;
	*(u32*) 0xFD030004 &= ~0xFFF;
	*(u32*) 0xFD030004 |= 0xAA9;
	sdiohost_setpower(host, SDIO_HOST_SETPOWER_VOL_3P3);
	gpio_set_value(host->cp_gpio, host->cp_gpio_value);
}

/*
	Get SDIO Data status

	@param[in] id   SDIO channel ID
			- @b SDIO_HOST_ID_1: SDIO1
			- @b SDIO_HOST_ID_2: SDIO2

	@return TRUE: ready
		FALSE: busy
*/
uint32_t sdiohost_getdata_status(struct mmc_nvt_host *host, uint32_t id)
{
	union SDIO_BUS_STATUS_REG stsreg;

	stsreg.reg = sdiohost_getreg(host, id, SDIO_BUS_STATUS_REG_OFS);

	return stsreg.reg;
}

u32 sdiohost_set_voltage_switch(struct mmc_nvt_host *host)
{
	union SDIO_DATA_CTRL_REG datactrl_reg;
	union SDIO_VOL_SWITCH_TIMER_REG timer_reg;
	union SDIO_INT_MASK_REG int_mask_reg;

	host->voltage_switch_timeout = 0;

	timer_reg.reg = clk_get_rate(host->clk)/1000;
	sdiohost_setreg(host, host->id, SDIO_VOL_SWITCH_TIMER_REG_OFS, timer_reg.reg);

	int_mask_reg.reg = sdiohost_getreg(host, host->id, SDIO_INT_MASK_REG_OFS);
	int_mask_reg.bit.VOL_SWITCH_END_INT_EN = 1;
	int_mask_reg.bit.VOL_SWITCH_TIMEOUT_INT_EN = 1;
	sdiohost_setreg(host, host->id, SDIO_INT_MASK_REG_OFS, int_mask_reg.reg);

	datactrl_reg.reg = sdiohost_getreg(host, host->id, SDIO_DATA_CTRL_REG_OFS);
	datactrl_reg.bit.WAIT_VOL_SWITCH_EN = 1;
	sdiohost_setreg(host, host->id, SDIO_DATA_CTRL_REG_OFS, datactrl_reg.reg);

	wait_for_completion(&host->voltage_switch_complete);

	int_mask_reg.reg = sdiohost_getreg(host, host->id, SDIO_INT_MASK_REG_OFS);
	int_mask_reg.bit.VOL_SWITCH_END_INT_EN = 0;
	int_mask_reg.bit.VOL_SWITCH_TIMEOUT_INT_EN = 0;
	sdiohost_setreg(host, host->id, SDIO_INT_MASK_REG_OFS, int_mask_reg.reg);

	if (host->voltage_switch_timeout) {
		pr_err("voltage switch timeout\n");
		if ((host->cp_gpio) && (host->id == SDIO_HOST_ID_1))
			sdiohost_power_cycle(host);

		return -EIO;
	} else
		return 0;

}

void sdiohost_setphyphase_cmpen(struct mmc_nvt_host *host, u32 sel)
{
	union SDIO_DLY4_REG dly4_reg;

	dly4_reg.reg = sdiohost_getreg(host, host->id, SDIO_DLY4_REG_OFS);
	dly4_reg.bit.PHASE_COMP_EN = sel;

	sdiohost_setreg(host, host->id, SDIO_DLY4_REG_OFS, dly4_reg.reg);
}

u32 sdiohost_getphydetout(struct mmc_nvt_host *host)
{
	union SDIO_DLY2_REG dly2_reg;

	dly2_reg.reg = sdiohost_getreg(host, host->id, SDIO_DLY2_REG_OFS);

	return dly2_reg.bit.DET_DATA_OUT;
}

int sdiohost_tuning_cmd(struct mmc_nvt_host *host, u32 opcode)
{
	struct STRG_SEG_DES tuning_destable[1];
	u32 i, blocksize, timeout, status;
	u8 tuningbuf[128] = {0};
	u8 golden_buf[64] = {
		0xFF, 0x0F, 0xFF, 0x00, 0xFF, 0xCC, 0xC3, 0xCC,
		0xC3, 0x3C, 0xCC, 0xFF, 0xFE, 0xFF, 0xFE, 0xEF,
		0xFF, 0xDF, 0xFF, 0xDD, 0xFF, 0xFB, 0xFF, 0xFB,
		0xBF, 0xFF, 0x7F, 0xFF, 0x77, 0xF7, 0xBD, 0xEF,
		0xFF, 0xF0, 0xFF, 0xF0, 0x0F, 0xFC, 0xCC, 0x3C,
		0xCC, 0x33, 0xCC, 0xCF, 0xFF, 0xEF, 0xFF, 0xEE,
		0xFF, 0xFD, 0xFF, 0xFD, 0xDF, 0xFF, 0xBF, 0xFF,
		0xBB, 0xFF, 0xF7, 0xFF, 0xF7, 0x7F, 0x7B, 0xDE
	};

	/*Set timeout clock count and blk sz*/
	blocksize = sdiohost_getblksize(host, host->id);

	sdiohost_setblksize(host, host->id, 64);

	timeout = (clk_get_rate(host->clk) / 1000) * 1000;

	sdiohost_setdatatimeout(host, host->id, timeout);

	tuning_destable[0].uisegaddr = virt_to_phys(tuningbuf);
	tuning_destable[0].uisegsize = 64;

	__cpuc_flush_dcache_area(tuningbuf, 64);

	sdiohost_setdestab(host->id, (uint32_t)tuning_destable, 1,
		(uint32_t *)host->vuisdio_destab);

	sdiohost_setupdatatransferdma(host, host->id, (u32)tuningbuf, 64,
		SDIO_HOST_READ_DATA, host->vuisdio_destab);

	if (sdiohost_sendsdcmd(host, host->id, opcode | SDIO_CMD_REG_NEED_RSP, 0)) {

		sdiohost_resetdata(host, host->id);
		pr_err("SDIO%d RCard: read tuning pattern error!!\r\n", host->id);
		goto read_error;
	}

	wait_for_completion(&host->tuning_data_end);

	// Compare the data with expected tuning pattern
	for (i = 0; i < 64; i++) {
		if (tuningbuf[i] != golden_buf[i]) {
			goto pattern_error;
		}
	}

	status = sdiohost_getdataresult(host->id);
	if ((status & SDIO_STATUS_REG_DATA_END) &&
		(status & SDIO_STATUS_REG_DATA_CRC_OK))
		pr_debug("Tuning pattern Data end or CRC ok\r\n");

	sdiohost_setblksize(host, host->id, blocksize);

	return E_OK;

pattern_error:
	pr_err("Tuning pattern shoule be:\r\n");
	for (i = 0; i < 64; i++) {
		if (i != 0 && (i % 16 == 0)) {
			pr_err("\n");
		}
		pr_err("%02x ", golden_buf[i]);
	}
	pr_err("nBut, pattern from card is:\n");
	for (i = 0; i < 64; i++) {
		if (i != 0 && (i % 16 == 0)) {
			pr_err("\n");
		}
		pr_err("%02x ", tuningbuf[i]);
	}
	pr_err("\r\n");

read_error:
	pr_err("SDIO%d Tuning error\r\n", host->id);

	sdiohost_setblksize(host, host->id, blocksize);

	return E_SYS;
}

void sdiohost_setphyclrdetval(struct mmc_nvt_host *host)
{
	union SDIO_DLY3_REG dly3_reg;

	if ((clk_get_phase(host->clk)) && (host->id == SDIO_HOST_ID_1))
			clk_set_phase(host->clk, 0);

	dly3_reg.reg = sdiohost_getreg(host, host->id, SDIO_DLY3_REG_OFS);

	dly3_reg.bit.DET_CLR = 1;
	sdiohost_setreg(host, host->id, SDIO_DLY3_REG_OFS, dly3_reg.reg);

	dly3_reg.bit.DET_CLR = 0;
	sdiohost_setreg(host, host->id, SDIO_DLY3_REG_OFS, dly3_reg.reg);

	if ((!clk_get_phase(host->clk)) && (host->id == SDIO_HOST_ID_1))
			clk_set_phase(host->clk, 1);
}

void sdiohost_setphydetch(struct mmc_nvt_host *host, u32 ch)
{
	union SDIO_DLY0_REG dly0_reg;

	dly0_reg.reg = sdiohost_getreg(host, host->id, SDIO_DLY0_REG_OFS);

	dly0_reg.bit.DET_SEL = ch;

	sdiohost_setreg(host, host->id, SDIO_DLY0_REG_OFS, dly0_reg.reg);
}

void sdiohost_setdlyphase_sel(struct mmc_nvt_host *host, u32 sel)
{
	union SDIO_DLY0_REG dly0_reg;

	dly0_reg.reg = sdiohost_getreg(host, host->id, SDIO_DLY0_REG_OFS);

	dly0_reg.bit.DLY_PHASE_SEL = sel;

	sdiohost_setreg(host, host->id, SDIO_DLY0_REG_OFS, dly0_reg.reg);
}

int sdiohost_getcmd(struct mmc_nvt_host *host)
{
	return sdiohost_getreg(host, host->id, \
		SDIO_CMD_REG_OFS) & SDIO_CMD_REG_INDEX;
}
