/*
 *  driver/mmc/na51000_mmc.c
 *
 *  Author:	Howard Chang
 *  Created:	Oct 21, 2016
 *  Copyright:	Novatek Inc.
 *
 */

#include <asm/io.h>
#include "common.h"
#include <errno.h>
#include <mmc.h>
#include <malloc.h>
#include "na51000_mmc.h"
#include <asm/arch/nt96680_regs.h>
#include <asm/arch/nvt_types.h>
//#define __MMC_DEBUG
//668 emmc HAL APIs
//#define FPGA
#define mmc_resp_type (MMC_RSP_PRESENT|MMC_RSP_136|MMC_RSP_CRC|MMC_RSP_BUSY|MMC_RSP_OPCODE)
#if defined(CONFIG_MODEL_NVT_FPGA_S3)
	#define PLL_SYS_CR_REG_OFS	0xF0020040
#else
	#define PLL_SYS_CR_REG_OFS	0xF0020040
#endif

#define SDIO_MASK		0x7FF
#define SDIO2_MASK		0x7FF000

#ifdef CONFIG_MODEL_NVT_FPGA_S3
	#define FPGA_SDIO_SRCCLK	12000000
#else
	#define FPGA_SDIO_SRCCLK	192000000
#endif
int mmc_nvt_start_command(struct mmc_nvt_host *host);
#define dma	1
//=============================================================================================================

/*
	Get SDIO controller register.

	@param[in] host			host data structure
	@param[in] offset       register offset in SDIO controller (word alignment)

	@return register value
*/
static REGVALUE sdiohost_getreg(struct mmc_nvt_host *host, u32 offset)
{
	if (host->id == SDIO_HOST_ID_1)
		return readl(host->base + offset);
	else if (host->id == SDIO_HOST_ID_2)
		return readl(host->base + offset);
	else
		return readl(host->base + offset);
}

/*
	Set SDIO controller register.

	@param[in] host		host data structure
	@param[in] offset   offset in SDIO controller (word alignment)
	@param[in] value    register value

	@return void
*/
static void sdiohost_setreg(struct mmc_nvt_host *host, u32 offset, REGVALUE value)
{
	if (host->id == SDIO_HOST_ID_1)
		writel(value, host->base + offset);
	else if (host->id == SDIO_HOST_ID_2)
		writel(value, host->base + offset);
	else
		writel(value, host->base + offset);
}

/*
	Set SDIO input delay.

	@param[in] id               SDIO channel ID
	@param[in] uiDlySetting     delay setting

	@return void
*/
static void sdiohost_setinputdelay(struct mmc_nvt_host *host, u32 dly_setting)
{
	union SDIO_DLY0_REG dly_ctrl_reg;
	union SDIO_DLY0_REG input_set;

	input_set.reg = dly_setting;
	dly_ctrl_reg.reg = sdiohost_getreg(host, SDIO_DLY0_REG_OFS);
	dly_ctrl_reg.bit.SAMPLE_CLK_INV = input_set.bit.SAMPLE_CLK_INV;
	dly_ctrl_reg.bit.SRC_CLK_SEL = input_set.bit.SRC_CLK_SEL;
	dly_ctrl_reg.bit.DLY_PHASE_SEL = input_set.bit.DLY_PHASE_SEL;
	dly_ctrl_reg.bit.DLY_SEL = input_set.bit.DLY_SEL;
	dly_ctrl_reg.bit.DATA_DLY_SEL = input_set.bit.DATA_DLY_SEL;
	dly_ctrl_reg.bit.OUT_CLK_DLY_SEL = input_set.bit.OUT_CLK_DLY_SEL;
	dly_ctrl_reg.bit.DET_SEL = input_set.bit.DET_SEL;
	dly_ctrl_reg.bit.DET_AUTO = input_set.bit.DET_AUTO;

	sdiohost_setreg(host, SDIO_DLY0_REG_OFS, dly_ctrl_reg.reg);
}

static void sdiohost_setphyclkindly(struct mmc_nvt_host *host, u32 dly_setting)
{
	union SDIO_CLOCK_CTRL2_REG clk_crtl_reg;

	clk_crtl_reg.reg = sdiohost_getreg(host, SDIO_CLOCK_CTRL2_REG_OFS);

	clk_crtl_reg.bit.INDLY_SEL = dly_setting;

	sdiohost_setreg(host, SDIO_CLOCK_CTRL2_REG_OFS, clk_crtl_reg.reg);
}

static void sdiohost_setphyclkoutdly(struct mmc_nvt_host *host, u32 dly_setting)
{
	union SDIO_CLOCK_CTRL2_REG clk_crtl_reg;

	clk_crtl_reg.reg = sdiohost_getreg(host, SDIO_CLOCK_CTRL2_REG_OFS);

	clk_crtl_reg.bit.OUTDLY_SEL = dly_setting;

	sdiohost_setreg(host, SDIO_CLOCK_CTRL2_REG_OFS, clk_crtl_reg.reg);
}
/*
	Set SDIO bus width.

	@param[in] host		host data structure
	@param[in] Width    SDIO controller bus width
			- @b SDIO_BUS_WIDTH1: 1 bit data bus
			- @b SDIO_BUS_WIDTH4: 4 bits data bus
			- @b SDIO_BUS_WIDTH8: 8 bits data bus

	@return void
*/
void sdiohost_setbuswidth(struct mmc_nvt_host *host, u32 width)
{
	union SDIO_BUS_WIDTH_REG widthreg;

	widthreg.reg = 0;
	widthreg.bit.BUS_WIDTH = width;
	sdiohost_setreg(host, SDIO_BUS_WIDTH_REG_OFS, widthreg.reg);
}


/*
	Set SDIO clock enable or disable

	When set to TRUE, SD controller will output SD clock to SD card.
	When set to FALSE, SD controller will not output SD clock to SD card.

	@param[in] host data structure
	@param[in] enableflag   enable clock output
				- @b TRUE: enable SD clock output
				- @b FALSE: disable SD clock output

	@return void
*/
void sdiohost_enclkout(struct mmc_nvt_host *host, BOOL enableflag)
{
	union SDIO_CLOCK_CTRL_REG clkctrlreg;

	clkctrlreg.reg = sdiohost_getreg(host, SDIO_CLOCK_CTRL_REG_OFS);

	if (enableflag == TRUE) {
		/* enabke SDIO CLK */
		clkctrlreg.bit.CLK_DIS = 0;
	} else {
		/* disable SDIO CLK */
		clkctrlreg.bit.CLK_DIS = 1;
	}

	sdiohost_setreg(host, SDIO_CLOCK_CTRL_REG_OFS, clkctrlreg.reg);
}

/*
	Set SDIO CLK card type.

	@param[in] host data structure
	@param[in] brisingsample    SDIO controller input sampling timing
			- @b TRUE: sample at rising edge
			- @b FALSE :sample at falling edge

	@return void
*/
void sdiohost_setclktype(struct mmc_nvt_host *host, BOOL brisingsample)
{
	union SDIO_CLOCK_CTRL_REG clkctrlreg;

	clkctrlreg.reg = sdiohost_getreg(host, SDIO_CLOCK_CTRL_REG_OFS);

	if (brisingsample)
		clkctrlreg.bit.CLK_SD = 1;
	else
		clkctrlreg.bit.CLK_SD = 0;

	sdiohost_setreg(host, SDIO_CLOCK_CTRL_REG_OFS, clkctrlreg.reg);
}

/*
	Set SDIO CLK cmd type.

	@param[in] host data sturcture
	@param[in] brisingsample    SDIO controller input sampling timing
			- @b TRUE: sample at rising edge
			- @b FALSE :sample at falling edge

	@return void
*/
void sdiohost_setclkcmdtype(struct mmc_nvt_host *host, BOOL brisingsample)
{
	union SDIO_CLOCK_CTRL_REG clkctrlreg;

	clkctrlreg.reg = sdiohost_getreg(host, SDIO_CLOCK_CTRL_REG_OFS);

	if (brisingsample)
		clkctrlreg.bit.CLK_SD_CMD = 1;
	else
		clkctrlreg.bit.CLK_SD_CMD = 0;

	sdiohost_setreg(host, SDIO_CLOCK_CTRL_REG_OFS, clkctrlreg.reg);
}


/**
    Get module clock rate

    Get module clock rate, one module at a time.

    @param[in] num      Module ID(PLL_CLKSEL_*), one module at a time.
                          Please refer to pll.h

    @return Moudle clock rate(PLL_CLKSEL_*_*), please refer to pll.h
*/
u32 pll_get_sdioclock_rate(int id)
{
	REGVALUE    regdata;

	if (id != SDIO_HOST_ID_3)
		HAL_READ_UINT32(PLL_SYS_CR_REG_OFS, regdata);
	else
		HAL_READ_UINT32(PLL_SYS_CR_REG_OFS + 0x4, regdata);

	if (id != SDIO_HOST_ID_2)
		regdata &= SDIO_MASK;
	else
		regdata &= SDIO2_MASK;

	return (u32)regdata;
}

/**
    Set module clock rate

    Set module clock rate, one module at a time.

	@param[in] id		SDIO channel
    @param[in] num      Module ID(PLL_CLKSEL_*), one module at a time.
                          Please refer to pll.h
    @param[in] value    Moudle clock rate(PLL_CLKSEL_*_*), please refer to pll.h

    @return void
*/
void pll_set_sdioclock_rate(int id, u32 value)
{
	REGVALUE regdata;

	if (id != SDIO_HOST_ID_3)
		HAL_READ_UINT32(PLL_SYS_CR_REG_OFS, regdata);
	else
		HAL_READ_UINT32(PLL_SYS_CR_REG_OFS + 0x4, regdata);

	if (id != SDIO_HOST_ID_2) {
		regdata &= ~SDIO_MASK;
		regdata |= value;
	} else {
		regdata &= ~SDIO2_MASK;
		regdata |= value << 12;
	}

	if (id != SDIO_HOST_ID_3)
		HAL_WRITE_UINT32(PLL_SYS_CR_REG_OFS, regdata);
	else
		HAL_WRITE_UINT32(PLL_SYS_CR_REG_OFS + 0x4, regdata);
}

/*
	Set bus clock.

	@param[in] id		SDIO channel
	@param[in] uiclock  SD bus clock in Hz

	@return void
*/
void nvt_clk_set_rate(int id, u32 uiclock)
{
	u32 divider, src_clk = FPGA_SDIO_SRCCLK;

	divider = (src_clk + uiclock-1)/uiclock;
	if (!divider)
		divider = 1;

	pll_set_sdioclock_rate(id, divider-1);
}

/*
	Set SDIO bus clock.

	@param[in] host data structure
	@param[in] uiclock  SD bus clock in Hz

	@return void
*/
void sdiohost_setbusclk(struct mmc_nvt_host *host, u32 uiclock, u32 *ns)
{
	union SDIO_CLOCK_CTRL_REG clkctrlreg;

	/* Disable SDIO clk */
	sdiohost_enclkout(host, FALSE);

	if (uiclock == 0) {
		return;
	}

	nvt_clk_set_rate(host->id, uiclock);

	if (ns)
		*ns = (1000000) / (uiclock/1000);

	/* Enable SDIO clk */
	sdiohost_enclkout(host, TRUE);

	clkctrlreg.reg = sdiohost_getreg(host, SDIO_CLOCK_CTRL_REG_OFS);

	clkctrlreg.bit.DLY_ACT = 1;

	sdiohost_setreg(host, SDIO_CLOCK_CTRL_REG_OFS, clkctrlreg.reg);

	sdiohost_setclktype(host, TRUE);
	sdiohost_setclkcmdtype(host, TRUE);
}

/*
	Get SDIO bus clock.

	@return unit of Hz
*/
static u32 sdiohost_getbusclk(int id)
{
	u32 uisourceclock;
	u32 uiclockdivider;

	uisourceclock = FPGA_SDIO_SRCCLK;

	uiclockdivider = pll_get_sdioclock_rate(id);

	return uisourceclock / (uiclockdivider + 1);
}

/*
	Get SDIO Busy or not

	@return TRUE: ready
		FALSE: busy
*/
BOOL sdiohost_getrdy(struct mmc_nvt_host *host)
{
	union SDIO_BUS_STATUS_REG stsreg;

	stsreg.reg = sdiohost_getreg(host, SDIO_BUS_STATUS_REG_OFS);

	return stsreg.bit.CARD_READY;
}



/*
	Reset SDIO host controller.

	@return void
*/
void sdiohost_reset(struct mmc_nvt_host *host)
{
	union SDIO_CMD_REG cmdreg;

	cmdreg.reg = sdiohost_getreg(host, SDIO_CMD_REG_OFS);
	cmdreg.bit.SDC_RST = 1;
	sdiohost_setreg(host, SDIO_CMD_REG_OFS, cmdreg.reg);

	while (1) {
		cmdreg.reg = sdiohost_getreg(host, SDIO_CMD_REG_OFS);

		if (cmdreg.bit.SDC_RST == 0)
			break;
	}
}

/*
	Reset SDIO controller data state machine.

	@return void
*/
void sdiohost_resetdata(struct mmc_nvt_host *host)
{
	union SDIO_DATA_CTRL_REG    datactrlreg;
	union SDIO_FIFO_CONTROL_REG fifoctrlreg;
	union SDIO_DLY1_REG dlyreg;
	union SDIO_PHY_REG  phyreg;
	union SDIO_PHY_REG  phyreg_read;
	union SDIO_FIFO_SWITCH_REG  fifoswitch;

	/* //#NT#Fix SDIO data state machine abnormal when DATA CRC/Timeout
	occurs before FIFO count complete */
	union SDIO_STATUS_REG	stsreg;

	/* SDIO bug: force to clear data end status to exit
	waiting data end state*/
	stsreg.reg          = 0;
	stsreg.bit.DATA_END = 1;
	sdiohost_setreg(host, SDIO_STATUS_REG_OFS, stsreg.reg);


	fifoctrlreg.reg = 0;
	sdiohost_setreg(host, SDIO_FIFO_CONTROL_REG_OFS, fifoctrlreg.reg);

	while (1) {
		fifoctrlreg.reg = sdiohost_getreg(host, SDIO_FIFO_CONTROL_REG_OFS);
		if (fifoctrlreg.bit.FIFO_EN == 0)
			break;
	}

	datactrlreg.reg = sdiohost_getreg(host, SDIO_DATA_CTRL_REG_OFS);
	datactrlreg.bit.DATA_EN = 0;
	sdiohost_setreg(host, SDIO_DATA_CTRL_REG_OFS, datactrlreg.reg);

	/* Fix SDIO data state machine abnormal when DATA
	CRC/Timeout occurs before FIFO count complete */
	/* Do software reset to reset SD state machine */
	sdiohost_reset(host);

	/* patch begin for sd write hang-up or write byte access error */
	fifoswitch.reg = sdiohost_getreg(host, SDIO_FIFO_SWITCH_REG_OFS);
	fifoswitch.bit.FIFO_SWITCH_DLY = 1;
	sdiohost_setreg(host, SDIO_FIFO_SWITCH_REG_OFS, fifoswitch.reg);
	/* patch end for sd write hang-up or write byte access error */

	phyreg.reg = sdiohost_getreg(host, SDIO_PHY_REG_OFS);
	phyreg.bit.PHY_SW_RST = 1;
	sdiohost_setreg(host, SDIO_PHY_REG_OFS, phyreg.reg);
	while (1)
	{
		phyreg_read.reg = sdiohost_getreg(host, SDIO_PHY_REG_OFS);
		if (phyreg_read.bit.PHY_SW_RST == 0)
			break;
	}

	dlyreg.reg = sdiohost_getreg(host, SDIO_DLY1_REG_OFS);
	dlyreg.bit.DATA_READ_DLY = 2;
	dlyreg.bit.DET_READ_DLY = 2;
	sdiohost_setreg(host, SDIO_DLY1_REG_OFS, dlyreg.reg);
}

/*
	Set SDIO controller data timeout.

	@param[in] timeout  time out value between data blocks (unit: SD clock)

	@return void
*/
void sdiohost_setdatatimeout(struct mmc_nvt_host *host, u32 timeout)
{
	union SDIO_DATA_TIMER_REG timerreg;

	timerreg.bit.Timeout = timeout;
	sdiohost_setreg(host, SDIO_DATA_TIMER_REG_OFS, timerreg.reg);
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


	HAL_READ_UINT32(PAD_DS_REG_OFS + dwoffset, padreg);

	padreg &= ~(bitmask << bitoffset);
	padreg |=  (driving << bitoffset);

	HAL_WRITE_UINT32(PAD_DS_REG_OFS + dwoffset, padreg);

	return E_OK;

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
static int sdiohost_setpaddriving(int id, SDIO_SPEED_MODE mode)
{
	UINT32 data_uidriving, cmd_uidriving, clk_uidriving;

	if (mode == SDIO_MODE_DS) {
#if defined(_NVT_DS_MODE_DATA_PAD_DRIVING_40MA_)
		data_uidriving = PAD_DRIVINGSINK_40MA;
#elif defined(_NVT_DS_MODE_DATA_PAD_DRIVING_10MA_)
		data_uidriving = PAD_DRIVINGSINK_10MA;
#elif defined(_NVT_DS_MODE_DATA_PAD_DRIVING_15MA_)
		data_uidriving = PAD_DRIVINGSINK_15MA;
#elif defined(_NVT_DS_MODE_DATA_PAD_DRIVING_20MA_)
		data_uidriving = PAD_DRIVINGSINK_20MA;
#elif defined(_NVT_DS_MODE_DATA_PAD_DRIVING_25MA_)
		data_uidriving = PAD_DRIVINGSINK_25MA;
#elif defined(_NVT_DS_MODE_DATA_PAD_DRIVING_30MA_)
		data_uidriving = PAD_DRIVINGSINK_30MA;
#elif defined(_NVT_DS_MODE_DATA_PAD_DRIVING_35MA_)
		data_uidriving = PAD_DRIVINGSINK_35MA;
#else
		data_uidriving = PAD_DRIVINGSINK_5MA;
#endif

#if defined(_NVT_DS_MODE_CMD_PAD_DRIVING_40MA_)
		cmd_uidriving = PAD_DRIVINGSINK_40MA;
#elif defined(_NVT_DS_MODE_CMD_PAD_DRIVING_10MA_)
		cmd_uidriving = PAD_DRIVINGSINK_10MA;
#elif defined(_NVT_DS_MODE_CMD_PAD_DRIVING_15MA_)
		cmd_uidriving = PAD_DRIVINGSINK_15MA;
#elif defined(_NVT_DS_MODE_CMD_PAD_DRIVING_20MA_)
		cmd_uidriving = PAD_DRIVINGSINK_20MA;
#elif defined(_NVT_DS_MODE_CMD_PAD_DRIVING_25MA_)
		cmd_uidriving = PAD_DRIVINGSINK_25MA;
#elif defined(_NVT_DS_MODE_CMD_PAD_DRIVING_30MA_)
		cmd_uidriving = PAD_DRIVINGSINK_30MA;
#elif defined(_NVT_DS_MODE_CMD_PAD_DRIVING_35MA_)
		cmd_uidriving = PAD_DRIVINGSINK_35MA;
#else
		cmd_uidriving = PAD_DRIVINGSINK_5MA;
#endif

#if defined(_NVT_DS_MODE_CLK_PAD_DRIVING_40MA_)
		clk_uidriving = PAD_DRIVINGSINK_40MA;
#elif defined(_NVT_DS_MODE_CLK_PAD_DRIVING_10MA_)
		clk_uidriving = PAD_DRIVINGSINK_10MA;
#elif defined(_NVT_DS_MODE_CLK_PAD_DRIVING_15MA_)
		clk_uidriving = PAD_DRIVINGSINK_15MA;
#elif defined(_NVT_DS_MODE_CLK_PAD_DRIVING_20MA_)
		clk_uidriving = PAD_DRIVINGSINK_20MA;
#elif defined(_NVT_DS_MODE_CLK_PAD_DRIVING_25MA_)
		clk_uidriving = PAD_DRIVINGSINK_25MA;
#elif defined(_NVT_DS_MODE_CLK_PAD_DRIVING_30MA_)
		clk_uidriving = PAD_DRIVINGSINK_30MA;
#elif defined(_NVT_DS_MODE_CLK_PAD_DRIVING_35MA_)
		clk_uidriving = PAD_DRIVINGSINK_35MA;
#else
		clk_uidriving = PAD_DRIVINGSINK_5MA;
#endif
	} else if (mode == SDIO_MODE_HS) {
#if defined(_NVT_HS_MODE_DATA_PAD_DRIVING_40MA_)
		data_uidriving = PAD_DRIVINGSINK_40MA;
#elif defined(_NVT_HS_MODE_DATA_PAD_DRIVING_10MA_)
		data_uidriving = PAD_DRIVINGSINK_10MA;
#elif defined(_NVT_HS_MODE_DATA_PAD_DRIVING_15MA_)
		data_uidriving = PAD_DRIVINGSINK_15MA;
#elif defined(_NVT_HS_MODE_DATA_PAD_DRIVING_20MA_)
		data_uidriving = PAD_DRIVINGSINK_20MA;
#elif defined(_NVT_HS_MODE_DATA_PAD_DRIVING_25MA_)
		data_uidriving = PAD_DRIVINGSINK_25MA;
#elif defined(_NVT_HS_MODE_DATA_PAD_DRIVING_30MA_)
		data_uidriving = PAD_DRIVINGSINK_30MA;
#elif defined(_NVT_HS_MODE_DATA_PAD_DRIVING_35MA_)
		data_uidriving = PAD_DRIVINGSINK_35MA;
#else
		data_uidriving = PAD_DRIVINGSINK_5MA;
#endif

#if defined(_NVT_HS_MODE_CMD_PAD_DRIVING_40MA_)
		cmd_uidriving = PAD_DRIVINGSINK_40MA;
#elif defined(_NVT_HS_MODE_CMD_PAD_DRIVING_10MA_)
		cmd_uidriving = PAD_DRIVINGSINK_10MA;
#elif defined(_NVT_HS_MODE_CMD_PAD_DRIVING_15MA_)
		cmd_uidriving = PAD_DRIVINGSINK_15MA;
#elif defined(_NVT_HS_MODE_CMD_PAD_DRIVING_20MA_)
		cmd_uidriving = PAD_DRIVINGSINK_20MA;
#elif defined(_NVT_HS_MODE_CMD_PAD_DRIVING_25MA_)
		cmd_uidriving = PAD_DRIVINGSINK_25MA;
#elif defined(_NVT_HS_MODE_CMD_PAD_DRIVING_30MA_)
		cmd_uidriving = PAD_DRIVINGSINK_30MA;
#elif defined(_NVT_HS_MODE_CMD_PAD_DRIVING_35MA_)
		cmd_uidriving = PAD_DRIVINGSINK_35MA;
#else
		cmd_uidriving = PAD_DRIVINGSINK_5MA;
#endif

#if defined(_NVT_HS_MODE_CLK_PAD_DRIVING_40MA_)
		clk_uidriving = PAD_DRIVINGSINK_40MA;
#elif defined(_NVT_HS_MODE_CLK_PAD_DRIVING_10MA_)
		clk_uidriving = PAD_DRIVINGSINK_10MA;
#elif defined(_NVT_HS_MODE_CLK_PAD_DRIVING_15MA_)
		clk_uidriving = PAD_DRIVINGSINK_15MA;
#elif defined(_NVT_HS_MODE_CLK_PAD_DRIVING_20MA_)
		clk_uidriving = PAD_DRIVINGSINK_20MA;
#elif defined(_NVT_HS_MODE_CLK_PAD_DRIVING_25MA_)
		clk_uidriving = PAD_DRIVINGSINK_25MA;
#elif defined(_NVT_HS_MODE_CLK_PAD_DRIVING_30MA_)
		clk_uidriving = PAD_DRIVINGSINK_30MA;
#elif defined(_NVT_HS_MODE_CLK_PAD_DRIVING_35MA_)
		clk_uidriving = PAD_DRIVINGSINK_35MA;
#else
		clk_uidriving = PAD_DRIVINGSINK_5MA;
#endif
	} else if (mode == SDIO_MODE_SDR50) {
#if defined(_NVT_SDR50_MODE_DATA_PAD_DRIVING_40MA_)
		data_uidriving = PAD_DRIVINGSINK_40MA;
#elif defined(_NVT_SDR50_MODE_DATA_PAD_DRIVING_10MA_)
		data_uidriving = PAD_DRIVINGSINK_10MA;
#elif defined(_NVT_SDR50_MODE_DATA_PAD_DRIVING_15MA_)
		data_uidriving = PAD_DRIVINGSINK_15MA;
#elif defined(_NVT_SDR50_MODE_DATA_PAD_DRIVING_20MA_)
		data_uidriving = PAD_DRIVINGSINK_20MA;
#elif defined(_NVT_SDR50_MODE_DATA_PAD_DRIVING_25MA_)
		data_uidriving = PAD_DRIVINGSINK_25MA;
#elif defined(_NVT_SDR50_MODE_DATA_PAD_DRIVING_30MA_)
		data_uidriving = PAD_DRIVINGSINK_30MA;
#elif defined(_NVT_SDR50_MODE_DATA_PAD_DRIVING_35MA_)
		data_uidriving = PAD_DRIVINGSINK_35MA;
#else
		data_uidriving = PAD_DRIVINGSINK_5MA;
#endif

#if defined(_NVT_SDR50_MODE_CMD_PAD_DRIVING_40MA_)
		cmd_uidriving = PAD_DRIVINGSINK_40MA;
#elif defined(_NVT_SDR50_MODE_CMD_PAD_DRIVING_10MA_)
		cmd_uidriving = PAD_DRIVINGSINK_10MA;
#elif defined(_NVT_SDR50_MODE_CMD_PAD_DRIVING_15MA_)
		cmd_uidriving = PAD_DRIVINGSINK_15MA;
#elif defined(_NVT_SDR50_MODE_CMD_PAD_DRIVING_20MA_)
		cmd_uidriving = PAD_DRIVINGSINK_20MA;
#elif defined(_NVT_SDR50_MODE_CMD_PAD_DRIVING_25MA_)
		cmd_uidriving = PAD_DRIVINGSINK_25MA;
#elif defined(_NVT_SDR50_MODE_CMD_PAD_DRIVING_30MA_)
		cmd_uidriving = PAD_DRIVINGSINK_30MA;
#elif defined(_NVT_SDR50_MODE_CMD_PAD_DRIVING_35MA_)
		cmd_uidriving = PAD_DRIVINGSINK_35MA;
#else
		cmd_uidriving = PAD_DRIVINGSINK_5MA;
#endif

#if defined(_NVT_SDR50_MODE_CLK_PAD_DRIVING_40MA_)
		clk_uidriving = PAD_DRIVINGSINK_40MA;
#elif defined(_NVT_SDR50_MODE_CLK_PAD_DRIVING_10MA_)
		clk_uidriving = PAD_DRIVINGSINK_10MA;
#elif defined(_NVT_SDR50_MODE_CLK_PAD_DRIVING_15MA_)
		clk_uidriving = PAD_DRIVINGSINK_15MA;
#elif defined(_NVT_SDR50_MODE_CLK_PAD_DRIVING_20MA_)
		clk_uidriving = PAD_DRIVINGSINK_20MA;
#elif defined(_NVT_SDR50_MODE_CLK_PAD_DRIVING_25MA_)
		clk_uidriving = PAD_DRIVINGSINK_25MA;
#elif defined(_NVT_SDR50_MODE_CLK_PAD_DRIVING_30MA_)
		clk_uidriving = PAD_DRIVINGSINK_30MA;
#elif defined(_NVT_SDR50_MODE_CLK_PAD_DRIVING_35MA_)
		clk_uidriving = PAD_DRIVINGSINK_35MA;
#else
		clk_uidriving = PAD_DRIVINGSINK_5MA;
#endif
	} else {
#if defined(_NVT_SDR104_MODE_DATA_PAD_DRIVING_40MA_)
		data_uidriving = PAD_DRIVINGSINK_40MA;
#elif defined(_NVT_SDR104_MODE_DATA_PAD_DRIVING_10MA_)
		data_uidriving = PAD_DRIVINGSINK_10MA;
#elif defined(_NVT_SDR104_MODE_DATA_PAD_DRIVING_15MA_)
		data_uidriving = PAD_DRIVINGSINK_15MA;
#elif defined(_NVT_SDR104_MODE_DATA_PAD_DRIVING_20MA_)
		data_uidriving = PAD_DRIVINGSINK_20MA;
#elif defined(_NVT_SDR104_MODE_DATA_PAD_DRIVING_25MA_)
		data_uidriving = PAD_DRIVINGSINK_25MA;
#elif defined(_NVT_SDR104_MODE_DATA_PAD_DRIVING_30MA_)
		data_uidriving = PAD_DRIVINGSINK_30MA;
#elif defined(_NVT_SDR104_MODE_DATA_PAD_DRIVING_35MA_)
		data_uidriving = PAD_DRIVINGSINK_35MA;
#else
		data_uidriving = PAD_DRIVINGSINK_5MA;
#endif

#if defined(_NVT_SDR104_MODE_CMD_PAD_DRIVING_40MA_)
		cmd_uidriving = PAD_DRIVINGSINK_40MA;
#elif defined(_NVT_SDR104_MODE_CMD_PAD_DRIVING_10MA_)
		cmd_uidriving = PAD_DRIVINGSINK_10MA;
#elif defined(_NVT_SDR104_MODE_CMD_PAD_DRIVING_15MA_)
		cmd_uidriving = PAD_DRIVINGSINK_15MA;
#elif defined(_NVT_SDR104_MODE_CMD_PAD_DRIVING_20MA_)
		cmd_uidriving = PAD_DRIVINGSINK_20MA;
#elif defined(_NVT_SDR104_MODE_CMD_PAD_DRIVING_25MA_)
		cmd_uidriving = PAD_DRIVINGSINK_25MA;
#elif defined(_NVT_SDR104_MODE_CMD_PAD_DRIVING_30MA_)
		cmd_uidriving = PAD_DRIVINGSINK_30MA;
#elif defined(_NVT_SDR104_MODE_CMD_PAD_DRIVING_35MA_)
		cmd_uidriving = PAD_DRIVINGSINK_35MA;
#else
		cmd_uidriving = PAD_DRIVINGSINK_5MA;
#endif

#if defined(_NVT_SDR104_MODE_CLK_PAD_DRIVING_40MA_)
		clk_uidriving = PAD_DRIVINGSINK_40MA;
#elif defined(_NVT_SDR104_MODE_CLK_PAD_DRIVING_10MA_)
		clk_uidriving = PAD_DRIVINGSINK_10MA;
#elif defined(_NVT_SDR104_MODE_CLK_PAD_DRIVING_15MA_)
		clk_uidriving = PAD_DRIVINGSINK_15MA;
#elif defined(_NVT_SDR104_MODE_CLK_PAD_DRIVING_20MA_)
		clk_uidriving = PAD_DRIVINGSINK_20MA;
#elif defined(_NVT_SDR104_MODE_CLK_PAD_DRIVING_25MA_)
		clk_uidriving = PAD_DRIVINGSINK_25MA;
#elif defined(_NVT_SDR104_MODE_CLK_PAD_DRIVING_30MA_)
		clk_uidriving = PAD_DRIVINGSINK_30MA;
#elif defined(_NVT_SDR104_MODE_CLK_PAD_DRIVING_35MA_)
		clk_uidriving = PAD_DRIVINGSINK_35MA;
#else
		clk_uidriving = PAD_DRIVINGSINK_5MA;
#endif
	}



	if (id == SDIO_HOST_ID_1) {
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
void sdiohost_delayd(struct mmc_nvt_host *host, u32 uid)
{
	u32 i;

	for (i = uid; i; i--)
		sdiohost_getreg(host, SDIO_CMD_REG_OFS);
}

/*
	Set SDIO controller block size.

	@param[in] host data structure

	@return void
*/
void sdiohost_setblksize(struct mmc_nvt_host *host)
{
	union SDIO_DATA_CTRL_REG datactrlreg;

	datactrlreg.reg = sdiohost_getreg(host, SDIO_DATA_CTRL_REG_OFS);
	datactrlreg.bit.BLK_SIZE = host->data->blocksize;
	sdiohost_setreg(host, SDIO_DATA_CTRL_REG_OFS, datactrlreg.reg);
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
void sdiohost_setupdatatransferdma(struct mmc_nvt_host *host ,
u32 uidmaaddress, u32 uidatalength, BOOL bisread)
{
	union SDIO_DATA_CTRL_REG datactrlreg;
	union SDIO_DATA_LENGTH_REG datalenreg;
	union SDIO_FIFO_CONTROL_REG fifoctrlreg;
	union SDIO_DMA_START_ADDR_REG dmaaddrreg;
	u32 uibusclk;

	/* dummy read for patch */
	sdiohost_delayd(host, 2);

	uibusclk = sdiohost_getbusclk(host->id);

	if (uibusclk >= 48000000)
		sdiohost_delayd(host, 3);
	else if ((uibusclk >= 24000000) && (uibusclk < 48000000))
		sdiohost_delayd(host, 6);
	else if ((uibusclk >= 12000000) && (uibusclk < 24000000))
		sdiohost_delayd(host, 9);
	else
		sdiohost_delayd(host, 21);

	/* patch for sd fifo bug end */

	datactrlreg.reg = sdiohost_getreg(host, SDIO_DATA_CTRL_REG_OFS);
	/* multiple read => disable SDIO INT detection after transfer end */
	if (bisread && (uidatalength > datactrlreg.bit.BLK_SIZE))
		datactrlreg.bit.DIS_SDIO_INT_PERIOD = 1;
	else
		datactrlreg.bit.DIS_SDIO_INT_PERIOD = 0;

	/*move data en after fifo en*/
	/*datactrlreg.bit.DATA_EN = 1;*/
	sdiohost_setreg(host, SDIO_DATA_CTRL_REG_OFS, datactrlreg.reg);

	dmaaddrreg.reg = 0;
	dmaaddrreg.bit.DRAM_ADDR = uidmaaddress;
	sdiohost_setreg(host, SDIO_DMA_START_ADDR_REG_OFS, dmaaddrreg.reg);

	datalenreg.reg = 0;
	datalenreg.bit.LENGTH = uidatalength;
	sdiohost_setreg(host, SDIO_DATA_LENGTH_REG_OFS, datalenreg.reg);

	fifoctrlreg.reg = 0;

	/* Flush cache in DMA mode*/
	if (!bisread)
		fifoctrlreg.bit.FIFO_DIR = 1;
	else
		fifoctrlreg.bit.FIFO_DIR = 0;

	fifoctrlreg.bit.FIFO_MODE = 1;
	sdiohost_setreg(host, SDIO_FIFO_CONTROL_REG_OFS, fifoctrlreg.reg);

	datactrlreg.reg = sdiohost_getreg(host, SDIO_DATA_CTRL_REG_OFS);
	datactrlreg.bit.DATA_EN = 1;
	sdiohost_setreg(host, SDIO_DATA_CTRL_REG_OFS, datactrlreg.reg);

	fifoctrlreg.bit.FIFO_EN = 1;
	sdiohost_setreg(host, SDIO_FIFO_CONTROL_REG_OFS, fifoctrlreg.reg);
}

/*
	Setup SDIO controller data transfer in PIO mode.

	@param[in] uidatalength total transfer length
			- possible value: 0x000_0001 ~ 0x3FF_FFFF
	@param[in] bisread      read/write mode
			- @b TRUE: indicate data read transfer
			- @b FALSE: indicate data write transfer

	@return void
*/
void sdiohost_setupdatatransferpio(struct mmc_nvt_host *host,  u32 uidatalength, BOOL bisread)
{
	union SDIO_DATA_CTRL_REG datactrlreg;
	union SDIO_DATA_LENGTH_REG datalenreg;
	union SDIO_FIFO_CONTROL_REG fifoctrlreg;
	u32 uibusclk;

	/* dummy read for patch */
	sdiohost_delayd(host, 2);

	uibusclk = sdiohost_getbusclk(host->id);

	if (uibusclk >= 48000000)
		sdiohost_delayd(host, 3);
	else if ((uibusclk >= 24000000) && (uibusclk < 48000000))
		sdiohost_delayd(host, 6);
	else if ((uibusclk >= 12000000) && (uibusclk < 24000000))
		sdiohost_delayd(host, 9);
	else
		sdiohost_delayd(host, 21);

	/* patch for sd fifo bug end */


	datactrlreg.reg = sdiohost_getreg(host, SDIO_DATA_CTRL_REG_OFS);
	/* multiple read => disable SDIO INT detection after transfer end */
	if (bisread && (uidatalength > datactrlreg.bit.BLK_SIZE))
		datactrlreg.bit.DIS_SDIO_INT_PERIOD = 1;
	else
		datactrlreg.bit.DIS_SDIO_INT_PERIOD = 0;

	datactrlreg.bit.DATA_EN = 1;
	sdiohost_setreg(host, SDIO_DATA_CTRL_REG_OFS, datactrlreg.reg);

	datalenreg.reg = 0;
	datalenreg.bit.LENGTH = uidatalength;
	sdiohost_setreg(host, SDIO_DATA_LENGTH_REG_OFS, datalenreg.reg);

	fifoctrlreg.reg = 0;

	if (!bisread)
		fifoctrlreg.bit.FIFO_DIR = 1;
	else
		fifoctrlreg.bit.FIFO_DIR = 0;

	sdiohost_setreg(host, SDIO_FIFO_CONTROL_REG_OFS, fifoctrlreg.reg);

	fifoctrlreg.bit.FIFO_EN = 1;
	sdiohost_setreg(host, SDIO_FIFO_CONTROL_REG_OFS, fifoctrlreg.reg);

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
ER sdiohost_writeblock(struct mmc_nvt_host *host, u8 *pbuf, u32 uilength)
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
		fifostsreg.reg = sdiohost_getreg(host, SDIO_FIFO_STATUS_REG_OFS);

		if (fifostsreg.bit.FIFO_EMPTY) {
			if (bwordalignment == TRUE) {
				/* Word alignment*/
				for (i = SDIO_HOST_DATA_FIFO_DEPTH; i; i--) {
					sdiohost_setreg(host, SDIO_DATA_PORT_REG_OFS, \
						*pbufword++);
				}
			} else {
				/* Not word alignment*/
				for (i = SDIO_HOST_DATA_FIFO_DEPTH; i; i--) {
					datareg.reg = *pbuf++;
					datareg.reg |= (*pbuf++) << 8;
					datareg.reg |= (*pbuf++) << 16;
					datareg.reg |= (*pbuf++) << 24;

					sdiohost_setreg(host, SDIO_DATA_PORT_REG_OFS, \
						datareg.reg);
				}
			}

			uifullcount--;
		}

		stsreg.reg = sdiohost_getreg(host, SDIO_STATUS_REG_OFS);

		if (stsreg.bit.DATA_CRC_FAIL || stsreg.bit.DATA_TIMEOUT) {
			printf("write block fail\n");
			return E_SYS;
		}
	}

	if (uiremaincount) {
		while (1) {

			fifostsreg.reg = \
				sdiohost_getreg(host, SDIO_FIFO_STATUS_REG_OFS);

			if (fifostsreg.bit.FIFO_EMPTY)
				break;

			stsreg.reg = sdiohost_getreg(host, SDIO_STATUS_REG_OFS);

			if (stsreg.bit.DATA_CRC_FAIL || stsreg.bit.DATA_TIMEOUT)
				return E_SYS;
		}

		if (bwordalignment == TRUE) {
			/* Word alignment*/
			for (i = uiremaincount; i; i--) {
				sdiohost_setreg(host, SDIO_DATA_PORT_REG_OFS, \
					*pbufword++);
			}
		} else {
			/* Not word alignment*/
			for (i = uiremaincount; i; i--) {
				datareg.reg = *pbuf++;
				datareg.reg |= (*pbuf++) << 8;
				datareg.reg |= (*pbuf++) << 16;
				datareg.reg |= (*pbuf++) << 24;

				sdiohost_setreg(host, SDIO_DATA_PORT_REG_OFS, \
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
ER sdiohost_readblock(struct mmc_nvt_host *host, u8 *pbuf, u32 uilength)
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
		fifostsreg.reg = sdiohost_getreg(host, SDIO_FIFO_STATUS_REG_OFS);

		if (fifostsreg.bit.FIFO_FULL) {
			if (bwordalignment == TRUE) {
				/* Word alignment*/
				for (i = SDIO_HOST_DATA_FIFO_DEPTH; i; i--) {
					*pbufword++ = \
					sdiohost_getreg(host, SDIO_DATA_PORT_REG_OFS);
				}
			} else {
				/* Not word alignment*/
				for (i = SDIO_HOST_DATA_FIFO_DEPTH; i; i--) {
					datareg.reg = \
					sdiohost_getreg(host, SDIO_DATA_PORT_REG_OFS);

					*pbuf++ = datareg.reg & 0xFF;
					*pbuf++ = (datareg.reg>>8) & 0xFF;
					*pbuf++ = (datareg.reg>>16) & 0xFF;
					*pbuf++ = (datareg.reg>>24) & 0xFF;
				}
			}

			uifullcount--;
		}

		stsreg.reg = sdiohost_getreg(host, SDIO_STATUS_REG_OFS);

		if (stsreg.bit.DATA_CRC_FAIL || stsreg.bit.DATA_TIMEOUT)
			return E_SYS;
	}

	if (uiremaincount) {
		while (1) {
			fifostsreg.reg = \
				sdiohost_getreg(host, SDIO_FIFO_STATUS_REG_OFS);

			if (fifostsreg.bit.FIFO_CNT == uiremaincount)
				break;

			stsreg.reg = sdiohost_getreg(host, SDIO_STATUS_REG_OFS);
			if (stsreg.bit.DATA_CRC_FAIL || stsreg.bit.DATA_TIMEOUT)
				return E_SYS;
		}

		if (bwordalignment == TRUE) {
			/* Word alignment*/
			for (i = uiremaincount; i; i--) {
				*pbufword++ = \
					sdiohost_getreg(host, SDIO_DATA_PORT_REG_OFS);
			}
		} else {
			/* Not word alignment*/
			for (i = uiremaincount; i; i--) {
				datareg.reg = \
					sdiohost_getreg(host, SDIO_DATA_PORT_REG_OFS);

				*pbuf++ = datareg.reg & 0xFF;
				*pbuf++ = (datareg.reg>>8) & 0xFF;
				*pbuf++ = (datareg.reg>>16) & 0xFF;
				*pbuf++ = (datareg.reg>>24) & 0xFF;
			}
		}
	}

	return E_OK;
}

u32 sdiohost_getstatus(struct mmc_nvt_host *host)
{
	return sdiohost_getreg(host, SDIO_STATUS_REG_OFS);
}

void sdiohost_setstatus(struct mmc_nvt_host *host, u32 status)
{
	sdiohost_setreg(host, SDIO_STATUS_REG_OFS, status);
}

static void sdiohost_fifo_data_trans(struct mmc_nvt_host *host,
					unsigned int n)
{
	if (host->data_dir == SDIO_HOST_WRITE_DATA) {
		host->buffer = (u8*) host->data->src;
		sdiohost_writeblock(host, (u8 *)host->buffer, \
			host->buffer_bytes_left);
		host->bytes_left -= host->buffer_bytes_left;
	} else {
		host->buffer = (u8*) host->data->dest;
		sdiohost_readblock(host, (u8 *)host->buffer, \
			host->buffer_bytes_left);
		host->bytes_left -= host->buffer_bytes_left;
	}
}

void sdiohost_clrfifoen(struct mmc_nvt_host *host)
{
	union SDIO_FIFO_CONTROL_REG fifoctrlreg;

	fifoctrlreg.reg = sdiohost_getreg(host, SDIO_FIFO_CONTROL_REG_OFS);
	fifoctrlreg.bit.FIFO_EN = 0;
	sdiohost_setreg(host, SDIO_FIFO_CONTROL_REG_OFS, fifoctrlreg.reg);
}

u32 sdiohost_getfifodir(struct mmc_nvt_host *host)
{
	union SDIO_FIFO_CONTROL_REG fifoctrlreg;

	fifoctrlreg.reg = sdiohost_getreg(host, SDIO_FIFO_CONTROL_REG_OFS);

	return fifoctrlreg.bit.FIFO_DIR;
}

void sdiohost_waitfifoempty(struct mmc_nvt_host *host)
{
	union SDIO_FIFO_STATUS_REG fifostsreg;
	u32 read0, read1;

	read0 = 0;
	while (1) {
		fifostsreg.reg = sdiohost_getreg(host, SDIO_FIFO_STATUS_REG_OFS);
		read1 = fifostsreg.bit.FIFO_EMPTY;
		if (read0 & read1)
			break;
		else
			read0 = read1;
	}
}

void sdiohost_getlongrsp(struct mmc_nvt_host *host, u32 *prsp3, u32 *prsp2, \
					u32 *prsp1, u32 *prsp0)
{
	union SDIO_RSP0_REG rsp0reg;
	union SDIO_RSP1_REG rsp1reg;
	union SDIO_RSP2_REG rsp2reg;
	union SDIO_RSP3_REG rsp3reg;

	rsp0reg.reg = sdiohost_getreg(host, SDIO_RSP0_REG_OFS);
	*prsp0 = (u32) rsp0reg.reg;
	rsp1reg.reg = sdiohost_getreg(host, SDIO_RSP1_REG_OFS);
	*prsp1 = (u32) rsp1reg.reg;
	rsp2reg.reg = sdiohost_getreg(host, SDIO_RSP2_REG_OFS);
	*prsp2 = (u32) rsp2reg.reg;
	rsp3reg.reg = sdiohost_getreg(host, SDIO_RSP3_REG_OFS);
	*prsp3 = (u32) rsp3reg.reg;
}

void sdiohost_getshortrsp(struct mmc_nvt_host *host, u32 *prsp)
{
	union SDIO_RSP0_REG rspreg;

	rspreg.reg = sdiohost_getreg(host, SDIO_RSP0_REG_OFS);
	*prsp = (u32) rspreg.reg;
}

static void mmc_nvt_cmd_done(struct mmc_nvt_host *host)
{
	struct mmc_cmd *cmd = host->cmd;
	host->cmd = NULL;

	if (cmd->resp_type & MMC_RSP_PRESENT) {
		if (cmd->resp_type & MMC_RSP_136) {
			/* response type 2 */
			sdiohost_getlongrsp(host, \
			(u32 *)&cmd->response[0], (u32 *)&cmd->response[1],
			(u32 *)&cmd->response[2], (u32 *)&cmd->response[3]);
			debug("lrsp 0x%x 0x%x 0x%x 0x%x\n", cmd->response[0], cmd->response[1], cmd->response[2], cmd->response[3]);
		} else {
			/* response types 1, 1b, 3, 4, 5, 6 */
			sdiohost_getshortrsp(host, (u32 *)&cmd->response[0]);
			debug("rsp 0x%x\n", cmd->response[0]);
		}
	}
}

static ER sdiohost_transfer(struct mmc_nvt_host *host)
{
	u32 status, qstatus = 0;
	int end_command = 0;
	int end_transfer = 0;
	int err_sts = 1, timeout = 0;
	struct mmc_data *data = host->data;


	while(1) {
		status = sdiohost_getstatus(host);
		//printf("cmd status is 0x%x\n", status);
		if (status & SDIO_STATUS_REG_CMD_SEND) {
			qstatus = status;
			sdiohost_setstatus(host, SDIO_STATUS_REG_CMD_SEND);
			break;
		}
	}

	if (data) {
		while(1) {
			status = sdiohost_getstatus(host);
			//printf("data status is 0x%x\n", status);
			if ((status & SDIO_STATUS_REG_DATA_END) || \
				(status & SDIO_STATUS_REG_DATA_TIMEOUT)) {
				qstatus = status;
				sdiohost_setstatus(host, SDIO_STATUS_REG_DATA_END);
				break;
			}
		}
	}


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
			debug("CMD%d timeout, status %x\n",host->cmd->cmdidx, qstatus);
			if (host->data) {
				sdiohost_resetdata(host);
				sdiohost_reset(host);
			}
		}
		sdiohost_setstatus(host, MMCST_RSP_TIMEOUT);
		timeout = 1;
	}

	if (qstatus & MMCST_DATA_TIMEOUT) {
		debug("data timeout (CMD%d), status 0x%x\n", host->cmd->cmdidx, qstatus);
		if (host->data) {
			sdiohost_resetdata(host);
			sdiohost_reset(host);
		}
		/*printk("MMCST_DATA_TIMEOUT\r\n");*/
		sdiohost_setstatus(host, MMCST_DATA_TIMEOUT);
	}

	if (qstatus & MMCST_RSP_CRC_FAIL) {
		/* Command CRC error */
		printf("Command CRC error\n");
		/*printk("MMCST_RSP_CRC_FAIL\r\n");*/
		sdiohost_setstatus(host, MMCST_RSP_CRC_FAIL);
	}

	if (qstatus & MMCST_DATA_CRC_FAIL) {
		/* Data CRC error */
		printf("data %s error\n", \
			(host->data->flags & MMC_DATA_WRITE) ? "write" : "read");
		printf("(CMD%d), status 0x%x\n", host->cmd->cmdidx, qstatus);
		sdiohost_resetdata(host);
		sdiohost_reset(host);
		/*printk("MMCST_DATA_CRC_FAIL\r\n");*/
		sdiohost_setstatus(host, MMCST_DATA_CRC_FAIL);
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
				sdiohost_fifo_data_trans(host, host->bytes_left);
			}
		}
		if (qstatus & MMCST_RSP_CRC_OK)
			sdiohost_setstatus(host, MMCST_RSP_CRC_OK);

		if (qstatus & MMCST_CMD_SENT)
			sdiohost_setstatus(host, MMCST_CMD_SENT);
	}

	if ((qstatus & MMCST_RSP_CRC_OK))
		end_command = (int) host->cmd;

	if ((qstatus & MMCST_DATA_END) || (qstatus & MMCST_DATA_CRC_OK)) {
		end_transfer = 1;
		data->dest += data->blocksize;
		if (!sdiohost_getfifodir(host)) {
			if (qstatus & MMCST_DATA_END) {
				if ((qstatus & MMCST_DMA_ERROR)
					!= MMCST_DMA_ERROR) {
					sdiohost_waitfifoempty(host);
				}
				sdiohost_clrfifoen(host);
			}
		}
		if (qstatus & MMCST_DATA_END)
			sdiohost_setstatus(host, MMCST_DATA_END);

		if (qstatus & MMCST_DATA_CRC_OK)
			sdiohost_setstatus(host, MMCST_DATA_CRC_OK);
	}

	if (end_command)
		mmc_nvt_cmd_done(host);

	if (end_transfer && (!host->cmd)) {
		host->data = NULL;
		host->cmd = NULL;
	}

	if (err_sts) {
		if (timeout)
			return TIMEOUT;

		status = sdiohost_getstatus(host);
		if(status)
			printf("end status is 0x%x\n", status);
		return COMM_ERR;
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
ER sdiohost_sendcmd(struct mmc_nvt_host *host, \
	u32 cmd, SDIO_HOST_RESPONSE rsptype, BOOL beniointdetect)
{
	union SDIO_CMD_REG cmdreg;
	u32 status;

	/*cmdreg.reg = 0;*/
	cmdreg.reg = sdiohost_getreg(host, SDIO_CMD_REG_OFS);
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
	sdiohost_setreg(host, SDIO_CMD_REG_OFS, cmdreg.reg);

	/*Clear all status*/
	status = sdiohost_getstatus(host);
	sdiohost_setstatus(host, status);

	/* Start command/data transmits */
	cmdreg.bit.CMD_EN = 1;
	sdiohost_setreg(host, SDIO_CMD_REG_OFS, cmdreg.reg);

	return sdiohost_transfer(host);

}

ER sdiohost_sendsdcmd(struct mmc_nvt_host *host, u32 cmdpart)
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

	sdiohost_setreg(host, SDIO_ARGU_REG_OFS, param);
	//printf("bEnIntDetect %d\r\n",benintdetect);

	return sdiohost_sendcmd(host, cmdpart & SDIO_CMD_REG_INDEX, \
			rsptype, benintdetect);
}

static void mmc_nvt_prepare_data(struct mmc_nvt_host *host)
{
	u32 size;

	if (host->data == NULL) {
		return;
	}

	sdiohost_setblksize(host);

	host->buffer = (u8*)(host->data->dest);
	host->bytes_left = host->data->blocks * host->data->blocksize;
	host->data_dir = ((host->data->flags & MMC_DATA_WRITE) ?
		SDIO_HOST_WRITE_DATA : SDIO_HOST_READ_DATA);

	if (dma) {
		if (host->bytes_left % ARCH_DMA_MINALIGN)
			size = (u32)(host->buffer) + roundup(host->bytes_left, ARCH_DMA_MINALIGN);
		else
			size = (u32)(host->buffer) + host->bytes_left;


		if (host->data_dir == SDIO_HOST_WRITE_DATA)
			flush_dcache_range((u32)(host->buffer), size);
		else
			invalidate_dcache_range(ALIGN_FLOOR((u32)host->buffer, ARCH_DMA_MINALIGN), (u32)roundup(size,ARCH_DMA_MINALIGN));

		sdiohost_setupdatatransferdma(host, (u32)host->buffer, \
			host->bytes_left, host->data_dir);
		/* zero this to ensure we take no PIO paths */
		host->bytes_left = 0;
	} else {
		sdiohost_setupdatatransferpio(host, \
			host->data->blocks * host->data->blocksize, \
			host->data_dir);
	}


}

int mmc_nvt_start_command(struct mmc_nvt_host *host)
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

	debug("CMD%d, arg 0x%08x %s\n", host->cmd->cmdidx, host->cmd->cmdarg, s);

	/* Set command index */
	cmd_reg |= host->cmd->cmdidx;

	return sdiohost_sendsdcmd(host, cmd_reg);
}

static int host_request(struct mmc *dev,
			struct mmc_cmd *cmd,
			struct mmc_data *data)
{
	int result;
	struct mmc_nvt_host *host = dev->priv;
	unsigned int time_start, time_now, mmcst = 0;
	host->data = data;
	host->cmd = cmd;

#ifdef CONFIG_NVT_IVOT_EMMC
	if (data || (host->id == SDIO_HOST_ID_3)) {
#else
	if (data) {
#endif
		time_start = get_timer (0);
		while(1) {
			mmcst = sdiohost_getrdy(host);
			if (mmcst == true)
				break;
#ifdef CONFIG_NVT_IVOT_EMMC
			if (host->id != SDIO_HOST_ID_3) {
				time_now = get_timer (0);
				if ((time_now - time_start) > 1000)
					break;
			}
#else
			time_now = get_timer (0);
			if ((time_now - time_start) > 1000)
				break;
#endif
		}

		if (mmcst == false) {
			printf("still busy\n");
			return -ETIMEDOUT;
		}
	}


	mmc_nvt_prepare_data(host);
	result = mmc_nvt_start_command(host);

	return result;
}

/* MMC uses open drain drivers in the enumeration phase */
static int mmc_host_reset(struct mmc *dev)
{
	return 0;
}

static int nvt_emmc_arch_host_preinit(struct mmc_nvt_host *host)
{
	if (host->id == SDIO_HOST_ID_1) {
		*(uint32_t*) 0xF00200A4 &= ~(0x4);
		udelay(10);
		*(uint32_t*) 0xF00200A4 |= 0x4;
		udelay(10);
		*(uint32_t*) 0xF0020020 &= ~0x30;
		udelay(10);
		*(uint32_t*) 0xF0020040 &= ~0x7FF;
		udelay(10);
		*(uint32_t*) 0xF0020040 |= 0x1F0;
		udelay(10);
		*(uint32_t*) 0xF0020084 |= 0x4;
	} else if (host->id == SDIO_HOST_ID_2) {
		*(uint32_t*) 0xF00200A4 &= ~(0x8);
		udelay(10);
		*(uint32_t*) 0xF00200A4 |= 0x8;
		udelay(10);
		*(uint32_t*) 0xF0020020 &= ~0x300;
		udelay(10);
		*(uint32_t*) 0xF0020040 &= ~0x7FF000;
		udelay(10);
		*(uint32_t*) 0xF0020040 |= 0x1F0000;
		udelay(10);
		*(uint32_t*) 0xF0020084 |= 0x8;
	} else {
		*(uint32_t*) 0xF00200A4 &= ~(0x4000);
		udelay(10);
		*(uint32_t*) 0xF00200A4 |= 0x4000;
		udelay(10);
		*(uint32_t*) 0xF0020084 &= ~0x1;
		udelay(10);
		*(uint32_t*) 0xF0020024 &= ~0x3;
		udelay(10);
		*(uint32_t*) 0xF0020044 &= ~0x7FF;
		udelay(10);
		*(uint32_t*) 0xF0020044 |= 0x1F0;
		udelay(10);
		*(uint32_t*) 0xF0020084 |= 0x4000;
	}

	sdiohost_enclkout(host, TRUE);

	sdiohost_setpaddriving(host->id, SDIO_MODE_DS);

	sdiohost_setinputdelay(host, SDIO_DLY_DS_DEFAULT);

	sdiohost_setphyclkindly(host, 0x20);

	sdiohost_resetdata(host);

	/* Delay 1 ms (SD spec) after clock is outputted. */
	/* (Delay 1024 us to reduce code size) */
	udelay(1024);

	sdiohost_setreg(host, SDIO_INT_MASK_REG_OFS, 0xFF);
	sdiohost_setdatatimeout(host, 0x10000000);

	sdiohost_setreg(host, 0x1FC, 0x1);

	return E_OK;
}

static void host_set_ios(struct mmc *dev)
{
	struct mmc_nvt_host *host = dev->priv;
	//kwinyee debug
	//printf("%s called !,clk = %d, bwidth = %d\n",__func__, dev->clock, dev->bus_width);

	if (dev->bus_width) {
		switch (dev->bus_width) {
		case 8:
			sdiohost_setbuswidth(host, SDIO_BUS_WIDTH8);
			break;
		case 4:
			sdiohost_setbuswidth(host, SDIO_BUS_WIDTH4);
			break;
		case 1:
			sdiohost_setbuswidth(host, SDIO_BUS_WIDTH1);
			break;
		}
	}

	if (dev->clock) {
		static int cur_clk = 0;
		if(dev->clock != cur_clk) {
			sdiohost_setbusclk(host, dev->clock, (u32*)&host->ns_in_one_cycle);

			/*Set driving with corresponding frequency*/
			if (dev->clock <= SDIO_MODE_DS) {
				sdiohost_setpaddriving(host->id, SDIO_MODE_DS);
				sdiohost_setinputdelay(host, SDIO_DLY_DS_DEFAULT);
			} else if (dev->clock <= SDIO_MODE_HS) {
				sdiohost_setpaddriving(host->id, SDIO_MODE_HS);
				sdiohost_setinputdelay(host, SDIO_DLY_HS_DEFAULT);
			} else if (dev->clock <= SDIO_MODE_SDR50) {
				sdiohost_setpaddriving(host->id, SDIO_MODE_SDR50);
				sdiohost_setinputdelay(host, SDIO_DLY_SDR50_DEFAULT);
				sdiohost_setphyclkoutdly(host, 0x10);
			} else {
				sdiohost_setpaddriving(host->id, SDIO_MODE_SDR104);
				sdiohost_setinputdelay(host, SDIO_DLY_SDR104_DEFAULT);
				sdiohost_setphyclkoutdly(host, 0x10);
			}
			sdiohost_setphyclkindly(host, 0x20);
			sdiohost_reset(host);
		}

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
static char dev_name_0[] = "NVT_MMC0";
static char dev_name_1[] = "NVT_MMC1";
static char dev_name_2[] = "NVT_MMC2";

/*
 * mmc_host_init - initialize the mmc controller.
 * Set initial clock and power for mmc slot.
 * Initialize mmc struct and register with mmc framework.
 */
int nvt_mmc_init(int id)
{
	struct mmc *dev;
	struct mmc_nvt_host *host;
	struct mmc_config *cfg;
	uint32_t reg;
	int enable_8bits = 0;

	host = malloc(sizeof(struct mmc_nvt_host));
	if (!host)
		return -ENOMEM;

	memset(host, 0, sizeof(struct mmc_nvt_host));

	cfg = malloc(sizeof(struct mmc_config));
	if (!cfg)
		return -ENOMEM;

	memset(cfg, 0, sizeof(struct mmc_config));


	//init host controler
	host->id = id;
	if (id == SDIO_HOST_ID_1) {
		host->base = (u32*)IOADDR_SDIO_REG_BASE;
		cfg->name = dev_name_0;
	} else if (id == SDIO_HOST_ID_2) {
		host->base = (u32*)IOADDR_SDIO2_REG_BASE;
		cfg->name = dev_name_1;
	} else {
		reg = *(uint32_t*) 0xF0010004;
		if (!(reg & 0x20000))
			return E_NOSPT;

		if ((reg & 0x81000000) == 0x81000000)
			enable_8bits = 1;

		host->base = (u32*)IOADDR_SDIO3_REG_BASE;
		cfg->name = dev_name_2;
	}

	nvt_emmc_arch_host_preinit(host);

	cfg->voltages = MMC_VDD_27_28 | MMC_VDD_28_29 | MMC_VDD_29_30 \
			| MMC_VDD_30_31 | MMC_VDD_31_32 | MMC_VDD_32_33 \
			| MMC_VDD_33_34 | MMC_VDD_34_35 | MMC_VDD_35_36;
	cfg->f_min = 312500;
#ifdef FPGA
	cfg->f_max = 6000000;
#else
	cfg->f_max = 50000000;
#endif
	cfg->b_max = (32*1024);
	cfg->host_caps = MMC_MODE_4BIT | MMC_MODE_HS_52MHz | MMC_MODE_HC \
			| MMC_MODE_HS;

	if(enable_8bits)
		cfg->host_caps |= MMC_MODE_8BIT;

	cfg->ops = &nvt_hsmmc_ops;

	dev = mmc_create(cfg, NULL);
	if (dev == NULL) {
		free(cfg);
		return -1;
	}

	dev->priv = host;

	debug("registered mmc interface number is:%d\n", dev->block_dev.dev);
	debug("MMC DEBUG : %s done \n", __FUNCTION__);

	return 0;
}
