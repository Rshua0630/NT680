/*
 *  driver/mmc/nt96660_mmc.c
 *
 *  Author:	Alvin lin
 *  Created:	April 24, 2013
 *  Copyright:	Novatek Inc.
 *
 */

#include <asm/io.h>
#include "common.h"
#include <errno.h>
#include <mmc.h>
#include <malloc.h>
#include "nt96660_mmc.h"
#include <asm/arch/nt96680_regs.h>
//#define __MMC_DEBUG
//668 emmc HAL APIs
#define FPGA
#define mmc_resp_type (MMC_RSP_PRESENT|MMC_RSP_136|MMC_RSP_CRC|MMC_RSP_BUSY|MMC_RSP_OPCODE)
#ifdef CONFIG_MODEL_NVT_FPGA_S1
#define PLL_SYS_CR_REG_OFS	0xF002003C
#endif

#ifdef CONFIG_MODEL_NVT_FPGA_S3
#define PLL_SYS_CR_REG_OFS	0xF0020040
#endif
#define SDIO_MASK		0xFF

#ifdef CONFIG_MODEL_NVT_FPGA_S1
#ifdef FPGA
#define FPGA_SDIO_SRCCLK	9600000
#else
#define FPGA_SDIO_SRCCLK	96000000
#endif
#endif


#ifdef CONFIG_MODEL_NVT_FPGA_S3
#ifdef FPGA
#define FPGA_SDIO_SRCCLK	48000000
#else
#define FPGA_SDIO_SRCCLK	480000000
#endif
#endif
int mmc_nt96660_start_command(struct mmc_nt96660_host *host);
#define dma	1
//=============================================================================================================

/*
	Get SDIO controller register.

	@param[in] offset       register offset in SDIO controller (word alignment)

	@return register value
*/
static REGVALUE sdiohost_getreg(u32 offset)
{
	u32 value = 0;
	value = readl(IOADDR_SDIO_REG_BASE + offset);
	return value;
}

/*
	Set SDIO controller register.

	@param[in] offset   offset in SDIO controller (word alignment)
	@param[in] value    register value

	@return void
*/
static void sdiohost_setreg(u32 offset, REGVALUE value)
{
	writel(value, IOADDR_SDIO_REG_BASE + offset);
}

/*
	Set SDIO bus width.

	@param[in] Width    SDIO controller bus width
			- @b SDIO_BUS_WIDTH1: 1 bit data bus
			- @b SDIO_BUS_WIDTH4: 4 bits data bus
			- @b SDIO_BUS_WIDTH8: 8 bits data bus

	@return void
*/
void sdiohost_setbuswidth(u32 width)
{
	union SDIO_BUS_WIDTH_REG widthreg;

	widthreg.reg = 0;
	widthreg.bit.BUS_WIDTH = width;
	sdiohost_setreg(SDIO_BUS_WIDTH_REG_OFS, widthreg.reg);
}


/*
	Set SDIO clock enable or disable

	When set to TRUE, SD controller will output SD clock to SD card.
	When set to FALSE, SD controller will not output SD clock to SD card.

	@param[in] enableflag   enable clock output
				- @b TRUE: enable SD clock output
				- @b FALSE: disable SD clock output

	@return void
*/
void sdiohost_enclkout(BOOL enableflag)
{
	union SDIO_CLOCK_CTRL_REG clkctrlreg;

	clkctrlreg.reg = sdiohost_getreg(SDIO_CLOCK_CTRL_REG_OFS);

	if (enableflag == TRUE) {
		/* enabke SDIO CLK */
		clkctrlreg.bit.CLK_DIS = 0;
	} else {
		/* disable SDIO CLK */
		clkctrlreg.bit.CLK_DIS = 1;
	}

	sdiohost_setreg(SDIO_CLOCK_CTRL_REG_OFS, clkctrlreg.reg);
}

/*
	Set SDIO CLK card type.

	@param[in] brisingsample    SDIO controller input sampling timing
			- @b TRUE: sample at rising edge
			- @b FALSE :sample at falling edge

	@return void
*/
void sdiohost_setclktype(BOOL brisingsample)
{
	union SDIO_CLOCK_CTRL_REG clkctrlreg;

	clkctrlreg.reg = sdiohost_getreg(SDIO_CLOCK_CTRL_REG_OFS);

	if (brisingsample)
		clkctrlreg.bit.CLK_SD = 1;
	else
		clkctrlreg.bit.CLK_SD = 0;

	sdiohost_setreg(SDIO_CLOCK_CTRL_REG_OFS, clkctrlreg.reg);
}

/*
	Set SDIO CLK cmd type.

	@param[in] brisingsample    SDIO controller input sampling timing
			- @b TRUE: sample at rising edge
			- @b FALSE :sample at falling edge

	@return void
*/
void sdiohost_setclkcmdtype(BOOL brisingsample)
{
	union SDIO_CLOCK_CTRL_REG clkctrlreg;

	clkctrlreg.reg = sdiohost_getreg(SDIO_CLOCK_CTRL_REG_OFS);

	if (brisingsample)
		clkctrlreg.bit.CLK_SD_CMD = 1;
	else
		clkctrlreg.bit.CLK_SD_CMD = 0;

	sdiohost_setreg(SDIO_CLOCK_CTRL_REG_OFS, clkctrlreg.reg);
}


/**
    Get module clock rate

    Get module clock rate, one module at a time.

    @param[in] num      Module ID(PLL_CLKSEL_*), one module at a time.
                          Please refer to pll.h

    @return Moudle clock rate(PLL_CLKSEL_*_*), please refer to pll.h
*/
u32 pll_get_sdioclock_rate()
{
	REGVALUE    regdata;

	HAL_READ_UINT32(PLL_SYS_CR_REG_OFS, regdata);
	regdata &= SDIO_MASK;

	return (u32)regdata;
}

/**
    Set module clock rate

    Set module clock rate, one module at a time.

    @param[in] num      Module ID(PLL_CLKSEL_*), one module at a time.
                          Please refer to pll.h
    @param[in] value    Moudle clock rate(PLL_CLKSEL_*_*), please refer to pll.h

    @return void
*/
void pll_set_sdioclock_rate(u32 value)
{
	REGVALUE regdata;

	HAL_READ_UINT32(PLL_SYS_CR_REG_OFS, regdata);
	regdata &= ~SDIO_MASK;
	regdata |= value;
	HAL_WRITE_UINT32(PLL_SYS_CR_REG_OFS, regdata);
}

/*
	Set bus clock.

	@param[in] uiclock  SD bus clock in Hz

	@return void
*/
void nvt_clk_set_rate(u32 uiclock)
{
	u32 divider, src_clk = FPGA_SDIO_SRCCLK;

	divider = (src_clk + uiclock-1)/uiclock;
	if (!divider)
		divider = 1;

	pll_set_sdioclock_rate(divider-1);
}

/*
	Set SDIO bus clock.

	@param[in] uiclock  SD bus clock in Hz

	@return void
*/
void sdiohost_setbusclk(u32 uiclock, u32 *ns)
{
	union SDIO_CLOCK_CTRL_REG clkctrlreg;

	/* Disable SDIO clk */
	sdiohost_enclkout(FALSE);

	if (uiclock == 0) {
		return;
	}

	nvt_clk_set_rate(uiclock);

	if (ns)
		*ns = (1000000) / (uiclock/1000);

	/* Enable SDIO clk */
	sdiohost_enclkout(TRUE);

	clkctrlreg.reg = sdiohost_getreg(SDIO_CLOCK_CTRL_REG_OFS);

	clkctrlreg.bit.DLY_ACT = 1;

	sdiohost_setreg(SDIO_CLOCK_CTRL_REG_OFS, clkctrlreg.reg);

	sdiohost_setclktype(TRUE);
	sdiohost_setclkcmdtype(TRUE);
}

/*
	Get SDIO bus clock.

	@return unit of Hz
*/
static u32 sdiohost_getbusclk(void)
{
	u32 uisourceclock;
	u32 uiclockdivider;

	uisourceclock = FPGA_SDIO_SRCCLK;

	uiclockdivider = pll_get_sdioclock_rate();

	return uisourceclock / (uiclockdivider + 1);
}

/*
	Get SDIO Busy or not

	@return TRUE: ready
		FALSE: busy
*/
BOOL sdiohost_getrdy(void)
{
	union SDIO_BUS_STATUS_REG stsreg;

	stsreg.reg = sdiohost_getreg(SDIO_BUS_STATUS_REG_OFS);

	return stsreg.bit.CARD_READY;
}



/*
	Reset SDIO host controller.

	@return void
*/
void sdiohost_reset(void)
{
	union SDIO_CMD_REG cmdreg;

	cmdreg.reg = sdiohost_getreg(SDIO_CMD_REG_OFS);
	cmdreg.bit.SDC_RST = 1;
	sdiohost_setreg(SDIO_CMD_REG_OFS, cmdreg.reg);

	while (1) {
		cmdreg.reg = sdiohost_getreg(SDIO_CMD_REG_OFS);

		if (cmdreg.bit.SDC_RST == 0)
			break;
	}
}

/*
	Reset SDIO controller data state machine.

	@return void
*/
void sdiohost_resetdata(void)
{
	union SDIO_DATA_CTRL_REG    datactrlreg;
	union SDIO_FIFO_CONTROL_REG fifoctrlreg;

	/* //#NT#Fix SDIO data state machine abnormal when DATA CRC/Timeout
	occurs before FIFO count complete */
	union SDIO_STATUS_REG	stsreg;

	/* SDIO bug: force to clear data end status to exit
	waiting data end state*/
	stsreg.reg          = 0;
	stsreg.bit.DATA_END = 1;
	sdiohost_setreg(SDIO_STATUS_REG_OFS, stsreg.reg);


	fifoctrlreg.reg = 0;
	sdiohost_setreg(SDIO_FIFO_CONTROL_REG_OFS, fifoctrlreg.reg);

	while (1) {
		fifoctrlreg.reg = sdiohost_getreg(SDIO_FIFO_CONTROL_REG_OFS);
		if (fifoctrlreg.bit.FIFO_EN == 0)
			break;
	}

	datactrlreg.reg = sdiohost_getreg(SDIO_DATA_CTRL_REG_OFS);
	datactrlreg.bit.DATA_EN = 0;
	sdiohost_setreg(SDIO_DATA_CTRL_REG_OFS, datactrlreg.reg);

	/* Fix SDIO data state machine abnormal when DATA
	CRC/Timeout occurs before FIFO count complete */
	/* Do software reset to reset SD state machine */
	sdiohost_reset();
}

/*
	Set SDIO controller data timeout.

	@param[in] timeout  time out value between data blocks (unit: SD clock)

	@return void
*/
void sdiohost_setdatatimeout(u32 timeout)
{
	union SDIO_DATA_TIMER_REG timerreg;

	timerreg.bit.Timeout = timeout;
	sdiohost_setreg(SDIO_DATA_TIMER_REG_OFS, timerreg.reg);
}

/*
	Set PAD driving Sink

	@param[in] name driving sink name
	@param[in] paddriving driving current

	@return
		- @b E_OK: sucess
		- @b Else: fail
*/

static int pad_setdrivingsink(u32 name, u32 paddriving)
{
	unsigned long padreg;
	unsigned long dwoffset, bitoffset, bitmask;
	unsigned long driving;

	if (name & PAD_DS_GROUP5_20) {
		name -= PAD_DS_GROUP5_20;
		if (paddriving & PAD_DRIVINGSINK_5MA)
			driving = 0;
		else if (paddriving & PAD_DRIVINGSINK_10MA)
			driving = 1;
		else if (paddriving & PAD_DRIVINGSINK_15MA)
			driving = 2;
		else if (paddriving & PAD_DRIVINGSINK_20MA)
			driving = 3;
		else
			return E_PAR;

		dwoffset  = (name >> 4) << 2;
		bitoffset = (name % 16) << 1;
		bitmask = 0x03;

	} else {
		if (paddriving & PAD_DRIVINGSINK_5MA)
			driving = 0;
		else if (paddriving & PAD_DRIVINGSINK_10MA)
			driving = 1;
		else if (paddriving & PAD_DRIVINGSINK_15MA)
			driving = 2;
		else if (paddriving & PAD_DRIVINGSINK_20MA)
			driving = 3;
		else
			return E_PAR;

		dwoffset  = (name >> 4) << 2;
		bitoffset = (name % 16) << 1;
		bitmask = 0x03;

	}

	HAL_READ_UINT32(PAD_DS_REG_OFS + dwoffset, padreg);

	padreg &= ~(bitmask << bitoffset);
	padreg |=  (driving << bitoffset);

	HAL_WRITE_UINT32(PAD_DS_REG_OFS + dwoffset, padreg);

	return E_OK;

}

/*
	Set PAD drive/sink of clock pin for specified SDIO channel.

	@param[in] driving  desired driving value * 10, unit: mA
				valid value: 50 ~ 200

	@return
		- @b E_OK: sucess
		- @b Else: fail
*/
static int sdiohost_setpaddriving(u32 driving)
{
	UINT32 uidriving;

	if (driving <= 50)
		uidriving = PAD_DRIVINGSINK_5MA;
	else if (driving <= 100)
		uidriving = PAD_DRIVINGSINK_10MA;
	else if (driving <= 150)
		uidriving = PAD_DRIVINGSINK_15MA;
	else
		uidriving = PAD_DRIVINGSINK_20MA;

	pad_setdrivingsink(PAD_DS_CGPIO17, PAD_DRIVINGSINK_7P5MA);
	pad_setdrivingsink(PAD_DS_CGPIO18, PAD_DRIVINGSINK_7P5MA);
	pad_setdrivingsink(PAD_DS_CGPIO19, PAD_DRIVINGSINK_7P5MA);
	pad_setdrivingsink(PAD_DS_CGPIO20, PAD_DRIVINGSINK_7P5MA);
	pad_setdrivingsink(PAD_DS_CGPIO21, PAD_DRIVINGSINK_7P5MA);
	return pad_setdrivingsink(PAD_DS_CGPIO16, uidriving);

}

// void dma_flushwritecache(void *pbuf, u32 len)
// {
	// void *ptr;
	// printf("flash write buf 0x%x, len %d\r\n", pbuf, len);
	// for (ptr = pbuf; ptr < (pbuf + len + PAGE_SIZE); ptr += PAGE_SIZE)
		// flush_dcache_page(virt_to_page(ptr));
// }

// void dma_flushreadcache(void *pbuf, u32 len)
// {
	// void *ptr;
	// printf("flash read buf 0x%x, len %d\r\n", pbuf, len);
	// for (ptr = pbuf; ptr < (pbuf + len + PAGE_SIZE); ptr += PAGE_SIZE)
		// flush_dcache_page(virt_to_page(ptr));
// }

/*
	Delay for SDIO module

	@param[in] uid the count for dummy read

	@return void
*/
void sdiohost_delayd(u32 uid)
{
	u32 i;

	for (i = uid; i; i--)
		sdiohost_getreg(SDIO_CMD_REG_OFS);
}

/*
	Set SDIO controller block size.

	@param[in] size data size per block (unit: byte)
			- possible value: 0x0001 ~ 0xFFFF

	@return void
*/
void sdiohost_setblksize(u32 size)
{
	union SDIO_DATA_CTRL_REG datactrlreg;

	datactrlreg.reg = sdiohost_getreg(SDIO_DATA_CTRL_REG_OFS);
	datactrlreg.bit.BLK_SIZE = size;
	sdiohost_setreg(SDIO_DATA_CTRL_REG_OFS, datactrlreg.reg);
}

/*
	Setup SDIO controller data transfer in DMA mode.

	@param[in] uidmaaddress buffer DRAM address
			- possible value: 0x000_0000 ~ 0xFFF_FFFF
	@param[in] uidatalength total transfer length
			- possible value: 0x000_0001 ~ 0x3FF_FFFF
	@param[in] bisread      read/write mode
			- @b TRUE: indicate data read transfer
			- @b FALSE: indicate data write transfer

	@return void
*/
void sdiohost_setupdatatransferdma(struct mmc_nt96660_host *host ,
u32 uidmaaddress, u32 uidatalength, BOOL bisread)
{
	union SDIO_DATA_CTRL_REG datactrlreg;
	union SDIO_DATA_LENGTH_REG datalenreg;
	union SDIO_FIFO_CONTROL_REG fifoctrlreg;
	union SDIO_DMA_START_ADDR_REG dmaaddrreg;
	u32 uibusclk;


	/* patch for sd fifo bug */
	*(uint32_t*) 0xF0020074 &= ~0x4;

	/* dummy read for patch */
	sdiohost_delayd(2);

	uibusclk = sdiohost_getbusclk();

	if (uibusclk >= 48000000)
		sdiohost_delayd(3);
	else if ((uibusclk >= 24000000) && (uibusclk < 48000000))
		sdiohost_delayd(6);
	else if ((uibusclk >= 12000000) && (uibusclk < 24000000))
		sdiohost_delayd(9);
	else
		sdiohost_delayd(21);

	/* patch for sd fifo bug end */

	datactrlreg.reg = sdiohost_getreg(SDIO_DATA_CTRL_REG_OFS);
	/* multiple read => disable SDIO INT detection after transfer end */
	if (bisread && (uidatalength > datactrlreg.bit.BLK_SIZE))
		datactrlreg.bit.DIS_SDIO_INT_PERIOD = 1;
	else
		datactrlreg.bit.DIS_SDIO_INT_PERIOD = 0;

	/*move data en after fifo en*/
	/*datactrlreg.bit.DATA_EN = 1;*/
	sdiohost_setreg(SDIO_DATA_CTRL_REG_OFS, datactrlreg.reg);

	dmaaddrreg.reg = 0;
	dmaaddrreg.bit.DRAM_ADDR = uidmaaddress;
	sdiohost_setreg(SDIO_DMA_START_ADDR_REG_OFS, dmaaddrreg.reg);

	datalenreg.reg = 0;
	datalenreg.bit.LENGTH = uidatalength;
	sdiohost_setreg(SDIO_DATA_LENGTH_REG_OFS, datalenreg.reg);

	fifoctrlreg.reg = 0;

	/* Flush cache in DMA mode*/
	if (!bisread) {
		fifoctrlreg.bit.FIFO_DIR = 1;
	}

	fifoctrlreg.bit.FIFO_MODE = 1;
	sdiohost_setreg(SDIO_FIFO_CONTROL_REG_OFS, fifoctrlreg.reg);

	/* patch for sd fifo bug */
	*(uint32_t*) 0xF0020074 |= 0x4;
	/* patch for sd fifo bug end */

	fifoctrlreg.bit.FIFO_EN = 1;
	sdiohost_setreg(SDIO_FIFO_CONTROL_REG_OFS, fifoctrlreg.reg);

	datactrlreg.reg = sdiohost_getreg(SDIO_DATA_CTRL_REG_OFS);
	datactrlreg.bit.DATA_EN = 1;
	sdiohost_setreg(SDIO_DATA_CTRL_REG_OFS, datactrlreg.reg);
}

/*
	Setup SDIO controller data transfer in PIO mode.

	@param[in] uidmaaddress buffer DRAM address
			- possible value: 0x000_0000 ~ 0xFFF_FFFF
	@param[in] uidatalength total transfer length
			- possible value: 0x000_0001 ~ 0x3FF_FFFF
	@param[in] bisread      read/write mode
			- @b TRUE: indicate data read transfer
			- @b FALSE: indicate data write transfer

	@return void
*/
void sdiohost_setupdatatransferpio(u32 uidmaaddress, u32 uidatalength, BOOL bisread)
{
	union SDIO_DATA_CTRL_REG datactrlreg;
	union SDIO_DATA_LENGTH_REG datalenreg;
	union SDIO_FIFO_CONTROL_REG fifoctrlreg;
	u32 uibusclk;

	/* patch for sd fifo bug */
	*(uint32_t*) 0xF0020074 &= ~0x4;

	/* dummy read for patch */
	sdiohost_delayd(2);

	uibusclk = sdiohost_getbusclk();

	if (uibusclk >= 48000000)
		sdiohost_delayd(3);
	else if ((uibusclk >= 24000000) && (uibusclk < 48000000))
		sdiohost_delayd(6);
	else if ((uibusclk >= 12000000) && (uibusclk < 24000000))
		sdiohost_delayd(9);
	else
		sdiohost_delayd(21);

	/* patch for sd fifo bug end */


	datactrlreg.reg = sdiohost_getreg(SDIO_DATA_CTRL_REG_OFS);
	/* multiple read => disable SDIO INT detection after transfer end */
	if (bisread && (uidatalength > datactrlreg.bit.BLK_SIZE))
		datactrlreg.bit.DIS_SDIO_INT_PERIOD = 1;
	else
		datactrlreg.bit.DIS_SDIO_INT_PERIOD = 0;

	datactrlreg.bit.DATA_EN = 1;
	sdiohost_setreg(SDIO_DATA_CTRL_REG_OFS, datactrlreg.reg);

	datalenreg.reg = 0;
	datalenreg.bit.LENGTH = uidatalength;
	sdiohost_setreg(SDIO_DATA_LENGTH_REG_OFS, datalenreg.reg);

	fifoctrlreg.reg = 0;

	if (!bisread)
		fifoctrlreg.bit.FIFO_DIR = 1;

	sdiohost_setreg(SDIO_FIFO_CONTROL_REG_OFS, fifoctrlreg.reg);


	/* patch for sd fifo bug */
	*(uint32_t*) 0xF0020074 |= 0x4;
	/* patch for sd fifo bug end */

	fifoctrlreg.bit.FIFO_EN = 1;
	sdiohost_setreg(SDIO_FIFO_CONTROL_REG_OFS, fifoctrlreg.reg);

}

/*
	Write SDIO data blocks.


	@note This function should only be called in PIO mode.

	@param[in] pbuf         buffer DRAM address
	@param[in] uiLength     total length (block alignment)

	@return
			- @b E_OK: success
			- @b E_SYS: data CRC or data timeout error
*/
ER sdiohost_writeblock(struct mmc_nt96660_host *host, u8 *pbuf, u32 uilength)
{
	u32  uiwordcount, i, *pbufword;
	u32  uifullcount, uiremaincount;
	BOOL    bwordalignment;
	union SDIO_DATA_PORT_REG    datareg;
	union SDIO_FIFO_STATUS_REG  fifostsreg;
	union SDIO_STATUS_REG       stsreg;

	uiwordcount     = (uilength + sizeof(u32) - 1) / sizeof(u32);
	uifullcount     = uiwordcount / SDIO_HOST_DATA_FIFO_DEPTH;
	uiremaincount   = uiwordcount % SDIO_HOST_DATA_FIFO_DEPTH;
	pbufword        = (u32 *)pbuf;

	if ((u32)pbuf & 0x3)
		bwordalignment = FALSE;
	else
		bwordalignment = TRUE;

	while (uifullcount) {
		fifostsreg.reg = sdiohost_getreg(SDIO_FIFO_STATUS_REG_OFS);

		if (fifostsreg.bit.FIFO_EMPTY) {
			if (bwordalignment == TRUE) {
				/* Word alignment*/
				for (i = SDIO_HOST_DATA_FIFO_DEPTH; i; i--) {
					sdiohost_setreg(SDIO_DATA_PORT_REG_OFS, \
						*pbufword++);
				}
			} else {
				/* Not word alignment*/
				for (i = SDIO_HOST_DATA_FIFO_DEPTH; i; i--) {
					datareg.reg = *pbuf++;
					datareg.reg |= (*pbuf++) << 8;
					datareg.reg |= (*pbuf++) << 16;
					datareg.reg |= (*pbuf++) << 24;

					sdiohost_setreg(SDIO_DATA_PORT_REG_OFS, \
						datareg.reg);
				}
			}

			uifullcount--;
		}

		stsreg.reg = sdiohost_getreg(SDIO_STATUS_REG_OFS);

		if (stsreg.bit.DATA_CRC_FAIL || stsreg.bit.DATA_TIMEOUT) {
			printf("write block fail\n");
			return E_SYS;
		}
	}

	if (uiremaincount) {
		while (1) {

			fifostsreg.reg = \
				sdiohost_getreg(SDIO_FIFO_STATUS_REG_OFS);

			if (fifostsreg.bit.FIFO_EMPTY)
				break;

			stsreg.reg = sdiohost_getreg(SDIO_STATUS_REG_OFS);

			if (stsreg.bit.DATA_CRC_FAIL || stsreg.bit.DATA_TIMEOUT)
				return E_SYS;
		}

		if (bwordalignment == TRUE) {
			/* Word alignment*/
			for (i = uiremaincount; i; i--) {
				sdiohost_setreg(SDIO_DATA_PORT_REG_OFS, \
					*pbufword++);
			}
		} else {
			/* Not word alignment*/
			for (i = uiremaincount; i; i--) {
				datareg.reg = *pbuf++;
				datareg.reg |= (*pbuf++) << 8;
				datareg.reg |= (*pbuf++) << 16;
				datareg.reg |= (*pbuf++) << 24;

				sdiohost_setreg(SDIO_DATA_PORT_REG_OFS, \
					datareg.reg);
			}
		}
	}

	return E_OK;
}

/*
	Read SDIO data blocks.

	@note This function should only be called in PIO mode.

	@param[out] pbuf        buffer DRAM address
	@param[in] uiLength     total length (block alignment)

	@return
		- @b E_OK: success
		- @b E_SYS: data CRC or data timeout error
*/
ER sdiohost_readblock(struct mmc_nt96660_host *host, u8 *pbuf, u32 uilength)
{
	u32  uiwordcount, i, *pbufword;
	u32  uifullcount, uiremaincount;
	BOOL    bwordalignment;
	union SDIO_DATA_PORT_REG    datareg;
	union SDIO_FIFO_STATUS_REG  fifostsreg;
	union SDIO_STATUS_REG       stsreg;

	uiwordcount     = (uilength + sizeof(u32) - 1) / sizeof(u32);
	uifullcount     = uiwordcount / SDIO_HOST_DATA_FIFO_DEPTH;
	uiremaincount   = uiwordcount % SDIO_HOST_DATA_FIFO_DEPTH;
	pbufword        = (u32 *)pbuf;

	if ((u32)pbuf & 0x3)
		bwordalignment = FALSE;
	else
		bwordalignment = TRUE;

	while (uifullcount) {
		fifostsreg.reg = sdiohost_getreg(SDIO_FIFO_STATUS_REG_OFS);

		if (fifostsreg.bit.FIFO_FULL) {
			if (bwordalignment == TRUE) {
				/* Word alignment*/
				for (i = SDIO_HOST_DATA_FIFO_DEPTH; i; i--) {
					*pbufword++ = \
					sdiohost_getreg(SDIO_DATA_PORT_REG_OFS);
				}
			} else {
				/* Not word alignment*/
				for (i = SDIO_HOST_DATA_FIFO_DEPTH; i; i--) {
					datareg.reg = \
					sdiohost_getreg(SDIO_DATA_PORT_REG_OFS);

					*pbuf++ = datareg.reg & 0xFF;
					*pbuf++ = (datareg.reg>>8) & 0xFF;
					*pbuf++ = (datareg.reg>>16) & 0xFF;
					*pbuf++ = (datareg.reg>>24) & 0xFF;
				}
			}

			uifullcount--;
		}

		stsreg.reg = sdiohost_getreg(SDIO_STATUS_REG_OFS);

		if (stsreg.bit.DATA_CRC_FAIL || stsreg.bit.DATA_TIMEOUT)
			return E_SYS;
	}

	if (uiremaincount) {
		while (1) {
			fifostsreg.reg = \
				sdiohost_getreg(SDIO_FIFO_STATUS_REG_OFS);

			if (fifostsreg.bit.FIFO_CNT == uiremaincount)
				break;

			stsreg.reg = sdiohost_getreg(SDIO_STATUS_REG_OFS);
			if (stsreg.bit.DATA_CRC_FAIL || stsreg.bit.DATA_TIMEOUT)
				return E_SYS;
		}

		if (bwordalignment == TRUE) {
			/* Word alignment*/
			for (i = uiremaincount; i; i--) {
				*pbufword++ = \
					sdiohost_getreg(SDIO_DATA_PORT_REG_OFS);
			}
		} else {
			/* Not word alignment*/
			for (i = uiremaincount; i; i--) {
				datareg.reg = \
					sdiohost_getreg(SDIO_DATA_PORT_REG_OFS);

				*pbuf++ = datareg.reg & 0xFF;
				*pbuf++ = (datareg.reg>>8) & 0xFF;
				*pbuf++ = (datareg.reg>>16) & 0xFF;
				*pbuf++ = (datareg.reg>>24) & 0xFF;
			}
		}
	}

	return E_OK;
}

u32 sdiohost_getstatus(void)
{
	return sdiohost_getreg(SDIO_STATUS_REG_OFS);
}

void sdiohost_setstatus(u32 status)
{
	sdiohost_setreg(SDIO_STATUS_REG_OFS, status);
}

static void nt96660_fifo_data_trans(struct mmc_nt96660_host *host,
					unsigned int n)
{
	if (host->data_dir == SDIO_HOST_WRITE_DATA) {
		host->buffer = host->data->src;
		sdiohost_writeblock(host, (u8 *)host->buffer, \
			host->buffer_bytes_left);
		host->bytes_left -= host->buffer_bytes_left;
	} else {
		host->buffer = host->data->dest;
		sdiohost_readblock(host, (u8 *)host->buffer, \
			host->buffer_bytes_left);
		host->bytes_left -= host->buffer_bytes_left;
	}
}

void sdiohost_clrfifoen(void)
{
	union SDIO_FIFO_CONTROL_REG fifoctrlreg;

	fifoctrlreg.reg = sdiohost_getreg(SDIO_FIFO_CONTROL_REG_OFS);
	fifoctrlreg.bit.FIFO_EN = 0;
	sdiohost_setreg(SDIO_FIFO_CONTROL_REG_OFS, fifoctrlreg.reg);
}

u32 sdiohost_getfifodir(void)
{
	union SDIO_FIFO_CONTROL_REG fifoctrlreg;

	fifoctrlreg.reg = sdiohost_getreg(SDIO_FIFO_CONTROL_REG_OFS);

	return fifoctrlreg.bit.FIFO_DIR;
}

void sdiohost_waitfifoempty(void)
{
	union SDIO_FIFO_STATUS_REG fifostsreg;
	u32 read0, read1;

	read0 = 0;
	while (1) {
		fifostsreg.reg = sdiohost_getreg(SDIO_FIFO_STATUS_REG_OFS);
		read1 = fifostsreg.bit.FIFO_EMPTY;
		if (read0 & read1)
			break;
		else
			read0 = read1;
	}
}

void sdiohost_getlongrsp(u32 *prsp3, u32 *prsp2, u32 *prsp1, u32 *prsp0)
{
	union SDIO_RSP0_REG rsp0reg;
	union SDIO_RSP1_REG rsp1reg;
	union SDIO_RSP2_REG rsp2reg;
	union SDIO_RSP3_REG rsp3reg;

	rsp0reg.reg = sdiohost_getreg(SDIO_RSP0_REG_OFS);
	*prsp0 = (u32) rsp0reg.reg;
	rsp1reg.reg = sdiohost_getreg(SDIO_RSP1_REG_OFS);
	*prsp1 = (u32) rsp1reg.reg;
	rsp2reg.reg = sdiohost_getreg(SDIO_RSP2_REG_OFS);
	*prsp2 = (u32) rsp2reg.reg;
	rsp3reg.reg = sdiohost_getreg(SDIO_RSP3_REG_OFS);
	*prsp3 = (u32) rsp3reg.reg;
}

void sdiohost_getshortrsp(u32 *prsp)
{
	union SDIO_RSP0_REG rspreg;

	rspreg.reg = sdiohost_getreg(SDIO_RSP0_REG_OFS);
	*prsp = (u32) rspreg.reg;
}

static void mmc_nt96660_cmd_done(struct mmc_nt96660_host *host)
{
	struct mmc_cmd *cmd = host->cmd;
	host->cmd = NULL;

	if (cmd->resp_type & MMC_RSP_PRESENT) {
		if (cmd->resp_type & MMC_RSP_136) {
			/* response type 2 */
			sdiohost_getlongrsp(
			(u32 *)&cmd->response[0], (u32 *)&cmd->response[1],
			(u32 *)&cmd->response[2], (u32 *)&cmd->response[3]);
		} else {
			/* response types 1, 1b, 3, 4, 5, 6 */
			sdiohost_getshortrsp((u32 *)&cmd->response[0]);
		}
	}
}

static ER sdiohost_transfer(struct mmc_nt96660_host *host)
{
	u32 size, length, status, qstatus, mmcst = 0;
	int end_command = 0;
	int end_transfer = 0;
	int err_sts = 1;
	unsigned int time_start, time_now;
	struct mmc_data *data = host->data;

	while(1) {
		status = sdiohost_getstatus();
		//printf("cmd status is 0x%x\n", status);
		if (status & SDIO_STATUS_REG_CMD_SEND) {
			qstatus = status;
			sdiohost_setstatus(SDIO_STATUS_REG_CMD_SEND);
			break;
		}
	}

	if (data) {
		while(1) {
			status = sdiohost_getstatus();
			//printf("data status is 0x%x\n", status);
			if (status & SDIO_STATUS_REG_DATA_END) {
				qstatus = status;
				sdiohost_setstatus(SDIO_STATUS_REG_DATA_END);
				break;
			}
		}
	}

	length = host->data->blocks * host->data->blocksize;
	if (length % ARCH_DMA_MINALIGN)
		size = (u32)(host->buffer) + roundup(length, ARCH_DMA_MINALIGN);
	else
		size = (u32)(host->buffer) + length;

	invalidate_dcache_range((u32)(host->buffer), size);

	if ((qstatus & SDIO_STATUS_REG_RSP_CRC_FAIL) ||
		(qstatus & SDIO_STATUS_REG_DATA_CRC_FAIL) ||
		(qstatus & SDIO_STATUS_REG_RSP_TIMEOUT) ||
		(qstatus & SDIO_STATUS_REG_DATA_TIMEOUT)) {
		err_sts = 1;
	}
	else {
		err_sts = 0;
	}

	if (qstatus & MMCST_RSP_TIMEOUT) {
		/* Command timeout */
		if (host->cmd) {
			printf("CMD%d timeout, status %x\n",host->cmd->cmdidx, qstatus);
			if (host->data) {
				sdiohost_resetdata();
				sdiohost_reset();
			}
		}
		sdiohost_setstatus(MMCST_RSP_TIMEOUT);
	}

	if (qstatus & MMCST_DATA_TIMEOUT) {
		printf("data timeout (CMD%d), status 0x%x\n", host->cmd->cmdidx, qstatus);
		if (host->data) {
			sdiohost_resetdata();
			sdiohost_reset();
		}
		/*printk("MMCST_DATA_TIMEOUT\r\n");*/
		sdiohost_setstatus(MMCST_DATA_TIMEOUT);
	}

	if (qstatus & MMCST_RSP_CRC_FAIL) {
		/* Command CRC error */
		printf("Command CRC error\n");
		/*printk("MMCST_RSP_CRC_FAIL\r\n");*/
		sdiohost_setstatus(MMCST_RSP_CRC_FAIL);
	}

	if (qstatus & MMCST_DATA_CRC_FAIL) {
		/* Data CRC error */
		printf("data %s error\n", \
			(host->data->flags & MMC_DATA_WRITE) ? "write" : "read");
		printf("(CMD%d), status 0x%x\n", host->cmd->cmdidx, qstatus);
		sdiohost_resetdata();
		sdiohost_reset();
		/*printk("MMCST_DATA_CRC_FAIL\r\n");*/
		sdiohost_setstatus(MMCST_DATA_CRC_FAIL);
	}

	if ((qstatus & MMCST_RSP_CRC_OK) || (qstatus & MMCST_CMD_SENT)) {
		/* End of command phase */
		if (data == NULL)
			end_command = (int) host->cmd;
		else {
			if ((host->bytes_left > 0) && (dma == 0)) {
				/* if datasize < rw_threshold
				 * no RX ints are generated
				 */
				nt96660_fifo_data_trans(host, host->bytes_left);
			}
		}
		if (qstatus & MMCST_RSP_CRC_OK)
			sdiohost_setstatus(MMCST_RSP_CRC_OK);

		if (qstatus & MMCST_CMD_SENT)
			sdiohost_setstatus(MMCST_CMD_SENT);
	}

	if ((qstatus & MMCST_RSP_CRC_OK))
		end_command = (int) host->cmd;

	if ((qstatus & MMCST_DATA_END) || (qstatus & MMCST_DATA_CRC_OK)) {
		end_transfer = 1;
		data->dest += data->blocksize;
		if (!sdiohost_getfifodir()) {
			if (qstatus & MMCST_DATA_END) {
				if ((qstatus & MMCST_DMA_ERROR)
					!= MMCST_DMA_ERROR) {
					sdiohost_waitfifoempty();
				}
				sdiohost_clrfifoen();
			}
		}
		if (qstatus & MMCST_DATA_END)
			sdiohost_setstatus(MMCST_DATA_END);

		if (qstatus & MMCST_DATA_CRC_OK)
			sdiohost_setstatus(MMCST_DATA_CRC_OK);
	}

	if (end_command)
		mmc_nt96660_cmd_done(host);

	if (end_transfer && (!host->cmd)) {
		host->data = NULL;
		host->cmd = NULL;
	}

	if (err_sts) {
		status = sdiohost_getstatus();
		if(status)
			printf("end status is 0x%x\n", status);
		return COMM_ERR;
	}

	time_start = get_timer (0);
	while(1) {
		mmcst = sdiohost_getrdy();
		if (mmcst == true)
			break;

		time_now = get_timer (0);
		if ((time_now - time_start) > 1000)
			break;
	}

	if (mmcst == false) {
		printf("still busy\n");
		return -ETIMEDOUT;
	}



	return SDIO_HOST_CMD_OK;
}

/*
	Send SD command to SD bus.

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
ER sdiohost_sendcmd(struct mmc_nt96660_host *host, \
	u32 cmd, SDIO_HOST_RESPONSE rsptype, BOOL beniointdetect)
{
	union SDIO_CMD_REG cmdreg;
	u32 status;

	/*cmdreg.reg = 0;*/
	cmdreg.reg = sdiohost_getreg(SDIO_CMD_REG_OFS);
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
	sdiohost_setreg(SDIO_CMD_REG_OFS, cmdreg.reg);

	/*Clear all status*/
	status = sdiohost_getstatus();
	sdiohost_setstatus(status);

	/* Start command/data transmits */
	cmdreg.bit.CMD_EN = 1;
	sdiohost_setreg(SDIO_CMD_REG_OFS, cmdreg.reg);

	return sdiohost_transfer(host);

}

ER sdiohost_sendsdcmd(struct mmc_nt96660_host *host, u32 cmdpart)
{
	BOOL benintdetect = FALSE;
	SDIO_HOST_RESPONSE rsptype = SDIO_HOST_RSP_NONE;
	u32 param = host->cmd->cmdarg;

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

	sdiohost_setreg(SDIO_ARGU_REG_OFS, param);
	//printf("bEnIntDetect %d\r\n",benintdetect);

	return sdiohost_sendcmd(host, cmdpart & SDIO_CMD_REG_INDEX, \
			rsptype, benintdetect);
}

static void mmc_nt96660_prepare_data(struct mmc_nt96660_host *host)
{

	if (host->data == NULL) {
		return;
	}

	sdiohost_setblksize(host->data->blocksize);

	host->buffer = (u32)(host->data->dest);
	host->bytes_left = host->data->blocks * host->data->blocksize;
	host->data_dir = ((host->data->flags & MMC_DATA_WRITE) ?
		SDIO_HOST_WRITE_DATA : SDIO_HOST_READ_DATA);

	if (dma) {
		if(host->data_dir) {
			flush_dcache_range((u32)(host->buffer), \
				(u32)(host->buffer) + \
				host->data->blocks * host->data->blocksize);
		}

		sdiohost_setupdatatransferdma(host, (u32)host->buffer, \
			host->data->blocks * host->data->blocksize, \
			host->data_dir);
		/* zero this to ensure we take no PIO paths */
		host->bytes_left = 0;
	} else {
		sdiohost_setupdatatransferpio((u32)host->buffer, \
			host->data->blocks * host->data->blocksize, \
			host->data_dir);
	}


}

int mmc_nt96660_start_command(struct mmc_nt96660_host *host)
{
	u32 cmd_reg = 0;
	char *s;

	switch (host->cmd->resp_type & mmc_resp_type) {
	case MMC_RSP_R1: /* 48 bits, CRC */
		s = ", R1";
		cmd_reg |= SDIO_CMD_REG_NEED_RSP;
		break;
	case MMC_RSP_R1b:
		s = ", R1b";
		/* There's some spec confusion about when R1B is
		 * allowed, but if the card doesn't issue a BUSY
		 * then it's harmless for us to allow it.
		 */
		/*need to check card busy CARD_BUSY2READY bit or
		 *send _SDIO_SD_SEND_STATUS to check
		*/
		cmd_reg |= SDIO_CMD_REG_NEED_RSP;
		/* FALLTHROUGH */
		break;
	case MMC_RSP_R2: /* 136 bits, CRC */
		s = ", R2";
		cmd_reg |= SDIO_CMD_REG_LONG_RSP;
		break;
	case MMC_RSP_R3: /* 48 bits, no CRC */
		s = ", R3/R4";
		cmd_reg |= SDIO_CMD_REG_NEED_RSP;
		break;
	default:
		s = ", Rx";
		cmd_reg |= 0;
		break;
	};

	//printf("CMD%d, arg 0x%08x %s\n", host->cmd->cmdidx, host->cmd->cmdarg, s);

	/* Set command index */
	cmd_reg |= host->cmd->cmdidx;

	return sdiohost_sendsdcmd(host, cmd_reg);
}

static int host_request(struct mmc *dev,
			struct mmc_cmd *cmd,
			struct mmc_data *data)
{
	int result;
	struct mmc_nt96660_host *host = dev->priv;
	host->data = data;
	host->cmd = cmd;

	sdiohost_setreg(0x1FC, 0x1);

	mmc_nt96660_prepare_data(host);
	result = mmc_nt96660_start_command(host);

	return result;
}

/* MMC uses open drain drivers in the enumeration phase */
static int mmc_host_reset(struct mmc *dev)
{
	return 0;
}

static int nt96660_emmc_arch_host_preinit(void)
{
#ifdef CONFIG_MODEL_NVT_FPGA_S1
	*(uint32_t*) 0xF0020084 &= ~(0x4);
	udelay(10);
	*(uint32_t*) 0xF0020084 |= 0x4;
	udelay(10);
	*(uint32_t*) 0xF0020074 |= 0x4;
#endif

#ifdef CONFIG_MODEL_NVT_FPGA_S3
	*(uint32_t*) 0xF00200A4 &= ~(0x4);
	udelay(10);
	*(uint32_t*) 0xF00200A4 |= 0x4;
	udelay(10);
	*(uint32_t*) 0xF0020084 |= 0x4;
#endif

	sdiohost_enclkout(TRUE);

	sdiohost_setpaddriving(100);

	sdiohost_resetdata();

	/* Delay 1 ms (SD spec) after clock is outputted. */
	/* (Delay 1024 us to reduce code size) */
	udelay(1024);

	sdiohost_setreg(SDIO_INT_MASK_REG_OFS, 0xFF);
	sdiohost_setdatatimeout(0x10000000);

	return E_OK;
}

static void host_set_ios(struct mmc *dev)
{
	struct mmc_nt96660_host *host = dev->priv;
	//kwinyee debug
	//printf("%s called !,clk = %d, bwidth = %d\n",__func__, dev->clock, dev->bus_width);

	if (dev->bus_width) {
		switch (dev->bus_width) {
		case 8:
			sdiohost_setbuswidth(SDIO_BUS_WIDTH8);
			break;
		case 4:
			sdiohost_setbuswidth(SDIO_BUS_WIDTH4);
			break;
		case 1:
			sdiohost_setbuswidth(SDIO_BUS_WIDTH1);
			break;
		}
	}

	if (dev->clock) {
		static int cur_clk = 0;
		if(dev->clock != cur_clk)
			sdiohost_setbusclk(dev->clock, (u32*)&host->ns_in_one_cycle);

		cur_clk = dev->clock;
	}

}

static int mmc_host_getcd(struct mmc *mmc)
{
	return 1;
}


static const struct mmc_ops nvt_hsmmc_ops = {
	.send_cmd = host_request,
	.set_ios = host_set_ios,
	.init = mmc_host_reset,
	.getcd = mmc_host_getcd,
};
static char dev_name[] = "NT96660_MMC";

/*
 * mmc_host_init - initialize the mmc controller.
 * Set initial clock and power for mmc slot.
 * Initialize mmc struct and register with mmc framework.
 */
int nt96660_mmc_init(void)
{
	struct mmc *dev;
	struct mmc_nt96660_host *host = malloc(sizeof(struct mmc_nt96660_host));
	struct mmc_config *cfg = malloc(sizeof(struct mmc_config));

	//init host controler
	nt96660_emmc_arch_host_preinit();

	cfg->name = dev_name;

	cfg->voltages = MMC_VDD_27_28 | MMC_VDD_28_29 | MMC_VDD_29_30 \
			| MMC_VDD_30_31 | MMC_VDD_31_32 | MMC_VDD_32_33 \
			| MMC_VDD_33_34 | MMC_VDD_34_35 | MMC_VDD_35_36;
	cfg->f_min = 312500;
#ifdef FPGA
	cfg->f_max = 5000000;
#else
	cfg->f_max = 50000000;
#endif
	cfg->b_max = (32*1024);
	cfg->host_caps = MMC_MODE_4BIT | MMC_MODE_HS_52MHz | MMC_MODE_HC \
			| MMC_MODE_HS;

	cfg->ops = &nvt_hsmmc_ops;
	dev = mmc_create(cfg, NULL);
	dev->priv = host;

	// printf("registered mmc interface number is:%d\n", dev->block_dev.dev);
	// printf("MMC DEBUG : %s done \n", __FUNCTION__);

	return 0;
}
