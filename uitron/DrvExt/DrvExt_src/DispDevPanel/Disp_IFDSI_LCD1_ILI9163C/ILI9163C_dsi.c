/*
    Display object object Golden Sample for driving Memory interface panel

    @file       ILI9163C.c
    @ingroup
    @note       Nothing

    Copyright   Novatek Microelectronics Corp. 2011.  All rights reserved.
*/

#include "display.h"
#include "dispdev_IFdsi.h"
#include "dispdev_panlcomn.h"

#define PANEL_WIDTH     480
#define PANEL_HEIGHT    864

/*
    Panel Parameters for TCON ILI9163C
*/
//@{
/*Used in DSI*/
const tPANEL_CMD tCmdModeDSI[] = {
	{DSICMD_CMD,     0x11},     // sleep out
	{CMDDELAY_MS,    10},

	//{DSICMD_CMD,     0x26},
	//{DSICMD_DATA,    0x04},
	//{DSICMD_CMD,     0xF2},
	//{DSICMD_DATA,    0x00},
	//{DSICMD_CMD,     0xB1},
	//{DSICMD_DATA,    0x08},
	//{DSICMD_DATA,    0x14},
	//{DSICMD_CMD,     0xC0},
	//{DSICMD_DATA,    0x0C},
	//{DSICMD_DATA,    0x05},
	//{DSICMD_CMD,     0xC1},
	//{DSICMD_DATA,    0x02},
	//{DSICMD_CMD,     0xC5},
	//{DSICMD_DATA,    0x44},
	//{DSICMD_DATA,    0x52},
	//{DSICMD_CMD,     0xC7},
	//{DSICMD_DATA,    0xBA},
	// 480 x 800
	{DSICMD_CMD,     0x2A},
	{DSICMD_DATA,    0x00},
	{DSICMD_DATA,    0x00},
	{DSICMD_DATA,    0x01},
	{DSICMD_DATA,    0xDF},
	{DSICMD_CMD,     0x2B},     // set page address
	{DSICMD_DATA,    0x00},
	{DSICMD_DATA,    0x00},
	{DSICMD_DATA,    0x03},
	{DSICMD_DATA,    0x60},

	{DSICMD_CMD,     0x36},     // set memory access control
	{DSICMD_DATA,    0x00},     // bit[3] = 0, RGB filter / bit[3] = 1, BGR filter
	{DSICMD_CMD,     0x3A},     // Interface pixel format
	{DSICMD_DATA,    0x55},     // x101, x101 [0x55]: 16 bits / pixel
//  {DSICMD_DATA,    0x66},     // x101, x101 [0x55]: 16 bits / pixel
	// x110, x110 [0x66]: 18 bits / pixel
	// x001, x001 [0x11]:  3 bits / pixel ( 8 color )


	{DSICMD_CMD,     0xB6},     //Page 233
	{DSICMD_DATA,    0x01},
	{DSICMD_DATA,    0x80},
	{DSICMD_DATA,    0xAF},     //864 line
	{DSICMD_DATA,    0x02},     //480x864

	//{DSICMD_CMD,     0xB5},     // VBP
	//{DSICMD_DATA,    0x04},
	//{DSICMD_DATA,    0x04},

	//{DSICMD_CMD,     0xF2},
	//{DSICMD_DATA,    0x01},

	//{DSICMD_CMD,     0xE0},     // Gamma
	//{DSICMD_DATA,    0x3F},
	//{DSICMD_DATA,    0x1B},
	//{DSICMD_DATA,    0x19},
	//{DSICMD_DATA,    0x27},
	//{DSICMD_DATA,    0x1F},
	//{DSICMD_DATA,    0x0D},
	//{DSICMD_DATA,    0x45},
	//{DSICMD_DATA,    0xB8},
	//{DSICMD_DATA,    0x30},
	//{DSICMD_DATA,    0x17},
	//{DSICMD_DATA,    0x0F},
	//{DSICMD_DATA,    0x05},
	//{DSICMD_DATA,    0x13},
	//{DSICMD_DATA,    0x02},
	//{DSICMD_DATA,    0x00},
	/*
	{DSICMD_CMD,     0xE1},
	{DSICMD_DATA,    0x00},
	{DSICMD_DATA,    0x24},
	{DSICMD_DATA,    0x26},
	{DSICMD_DATA,    0x08},
	{DSICMD_DATA,    0x10},
	{DSICMD_DATA,    0x12},
	{DSICMD_DATA,    0x3A},
	{DSICMD_DATA,    0x74},
	{DSICMD_DATA,    0x4F},
	{DSICMD_DATA,    0x08},
	{DSICMD_DATA,    0x20},
	{DSICMD_DATA,    0x2A},
	{DSICMD_DATA,    0x3C},
	{DSICMD_DATA,    0x3D},
	{DSICMD_DATA,    0x3F},
	*/
	//{DSICMD_CMD,     0x13},
	//{DSICMD_CMD,     0x35},     //TE-ON
	//{DSICMD_DATA,    0x00},

	{DSICMD_CMD,     0x29},     // display on
	{CMDDELAY_MS,    10},
	//{DSICMD_CMD,     0x2C}      // Write memory start
};


const tPANEL_CMD tCmdStandbyDSI[] = {
	{DSICMD_CMD,     0x28},     // Display OFF
	{DSICMD_CMD,     0x10}      // Sleep in
};

const tLCD_PARAM tModeDSI[] = {
	/***********       MI Serial Format 1      *************/
	{
		// tPANEL_PARAM
		{
			/* Old prototype */
			PINMUX_DSI_2_LANE_CMD_MODE_RGB565,  //!< LCDMode, don't care under DSI mode
			//6.5f,                             //!< fDCLK
			8.5f,                               //!< fDCLK
			(0x68 + PANEL_WIDTH),               //!< uiHSyncTotalPeriod
			PANEL_WIDTH,                        //!< uiHSyncActivePeriod
			0x34,                               //!< uiHSyncBackPorch
			0x8 + PANEL_HEIGHT,                 //!< uiVSyncTotalPeriod
			PANEL_HEIGHT,                       //!< uiVSyncActivePeriod
			0x04,                               //!< uiVSyncBackPorchOdd
			0x04,                               //!< uiVSyncBackPorchEven
			PANEL_WIDTH,                        //!< uiBufferWidth
			PANEL_HEIGHT,                       //!< uiBufferHeight
			PANEL_WIDTH,                        //!< uiWindowWidth
			PANEL_HEIGHT,                       //!< uiWindowHeight
			FALSE,                              //!< bYCbCrFormat

			/* New added parameters */
			0x01,                               //!< uiHSyncSYNCwidth
			0x01                                //!< uiVSyncSYNCwidth
		},

		// tIDE_PARAM
		{
			/* Old prototype */
			PINMUX_LCD_SEL_GPIO,            //!< pinmux_select_lcd;
			ICST_CCIR601,                   //!< icst;
			{TRUE, FALSE},                  //!< dithering[2];
			DISPLAY_DEVICE_MIPIDSI,         //!< **DONT-CARE**
			IDE_PDIR_RGB,                   //!< pdir;
			IDE_LCD_R,                      //!< odd;
			IDE_LCD_G,                      //!< even;
			TRUE,                           //!< hsinv;
			TRUE,                           //!< vsinv;
			FALSE,                          //!< hvldinv;
			FALSE,                          //!< vvldinv;
			TRUE,                           //!< clkinv;
			FALSE,                          //!< fieldinv;
			FALSE,                          //!< **DONT-CARE**
			FALSE,                          //!< interlace;
			FALSE,                          //!< **DONT-CARE**
			0x40,                           //!< ctrst;
			0x00,                           //!< brt;
			0x40,                           //!< cmults;
			FALSE,                          //!< cex;
			FALSE,                          //!< **DONT-CARE**
			TRUE,                           //!< **DONT-CARE**
			TRUE,                           //!< tv_powerdown;
			{0x00, 0x00},                   //!< **DONT-CARE**

			/* New added parameters */
			FALSE,                          //!< YCex
			FALSE,                          //!< HLPF
			{FALSE, FALSE, FALSE},          //!< subpixOdd[3]
			{FALSE, FALSE, FALSE},          //!< subpixEven[3]
			{IDE_DITHER_5BITS, IDE_DITHER_6BITS, IDE_DITHER_5BITS}, //!< DitherBits[3]
			FALSE                           //!< clk1/2
		},

		(tPANEL_CMD *)tCmdModeDSI,                 //!< pCmdQueue
		sizeof(tCmdModeDSI) / sizeof(tPANEL_CMD),  //!< nCmd
	}
};

const tLCD_ROT *tRotDSI = NULL;

//@}

tLCD_ROT *dispdev_getLcdRotateDSICmd(UINT32 *ModeNumber)
{
#if 0
	if (tRotDSI != NULL) {
		*ModeNumber = sizeof(tRotDSI) / sizeof(tLCD_ROT);
	} else
#endif
	{
		*ModeNumber = 0;
	}
	return (tLCD_ROT *)tRotDSI;
}

tLCD_PARAM *dispdev_getConfigModeDSI(UINT32 *ModeNumber)
{
	*ModeNumber = sizeof(tModeDSI) / sizeof(tLCD_PARAM);
	return (tLCD_PARAM *)tModeDSI;
}

tPANEL_CMD *dispdev_getStandbyCmdDSI(UINT32 *CmdNumber)
{
	*CmdNumber = sizeof(tCmdStandbyDSI) / sizeof(tPANEL_CMD);
	return (tPANEL_CMD *)tCmdStandbyDSI;
}

#if 0
void dispdev_writeToLcdDsi(DISPDEV_IOCTRL pDispDevControl, UINT32 addr, UINT32 value)
{
	//if(addr & MICMD_DATA)
	//    mi_setCmd1(WrDATA, (value&0xff));
	//else
	//    mi_setCmd1(WrCMD, (value&0xff));

	//mi_setCmdCnt(1);
	//mi_waitCmdDone();
}


void dispdev_readFromLcdDsi(DISPDEV_IOCTRL pDispDevControl, UINT32 addr, UINT32 *pValue)
{
	*pValue = 0;
	DBG_ERR("No MI Read reg implement!\r\n");
}
#endif

void      dispdev_setDSIConfig(tDSI_CONFIG *pDsiConfig)
{
#if 0
	//BOOL    bTeSyncEn   = FALSE;
	FLOAT   DsiTargetClk = 40.0; // Max is 60MHz
	UINT32  Div;


	Div = (UINT32)(pDsiConfig->fDsiSrcClk / DsiTargetClk);
	if (Div == 0) {
		Div++;
	}
	pll_setClockRate(PLL_CLKSEL_DSI_CLKDIV, PLL_DSI_CLKDIV(Div - 1));
#else
	dsi_setConfig(DSI_CONFIG_ID_FREQ, 40 * 1000000);
#endif
}
