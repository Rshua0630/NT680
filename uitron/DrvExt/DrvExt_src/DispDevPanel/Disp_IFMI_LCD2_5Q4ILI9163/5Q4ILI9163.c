/*
    Display object object Golden Sample for driving Memory interface panel

    @file       5Q4ILI9163.c
    @ingroup
    @note       Nothing

    Copyright   Novatek Microelectronics Corp. 2011.  All rights reserved.
*/

#include "display.h"
#include "dispdev_IFmi.h"
#include "dispdev_panlcomn.h"


/*
    Panel Parameters for TCON 5Q4ILI9163
*/
//@{
/*Used in MI format*/
const tPANEL_CMD tCmdModeMiFmt3[] = {
	{MICMD_CMD,     0x11},
	{CMDDELAY_MS,   10},

	{MICMD_CMD,     0x35},//TE-ON
	{MICMD_DATA,    0x00},

	{MICMD_CMD,     0x26},
	{MICMD_DATA,    0x04},
	{MICMD_CMD,     0xF2},
	{MICMD_DATA,    0x00},
	{MICMD_CMD,     0xB1},
	{MICMD_DATA,    0x08},
	{MICMD_DATA,    0x14},
	{MICMD_CMD,     0xC0},
	{MICMD_DATA,    0x0C},
	{MICMD_DATA,    0x05},
	{MICMD_CMD,     0xC1},
	{MICMD_DATA,    0x02},
	{MICMD_CMD,     0xC5},
	{MICMD_DATA,    0x44},
	{MICMD_DATA,    0x52},
	{MICMD_CMD,     0xC7},
	{MICMD_DATA,    0xBA},
	{MICMD_CMD,     0x2A},
	{MICMD_DATA,    0x00},
	{MICMD_DATA,    0x00},
	{MICMD_DATA,    0x00},
	{MICMD_DATA,    0x7F},
	{MICMD_CMD,     0x2B},
	{MICMD_DATA,    0x00},
	{MICMD_DATA,    0x00},
	{MICMD_DATA,    0x00},
	{MICMD_DATA,    0x9F},
	{MICMD_CMD,     0x36},
	{MICMD_DATA,    0x08},//0x48
	{MICMD_CMD,     0x3A},
	{MICMD_DATA,    0x55},
	{MICMD_CMD,     0xF2},
	{MICMD_DATA,    0x01},

	{MICMD_CMD,     0xE0},
	{MICMD_DATA,    0x3F},
	{MICMD_DATA,    0x1B},
	{MICMD_DATA,    0x19},
	{MICMD_DATA,    0x27},
	{MICMD_DATA,    0x1F},
	{MICMD_DATA,    0x0D},
	{MICMD_DATA,    0x45},
	{MICMD_DATA,    0xB8},
	{MICMD_DATA,    0x30},
	{MICMD_DATA,    0x17},
	{MICMD_DATA,    0x0F},
	{MICMD_DATA,    0x05},
	{MICMD_DATA,    0x13},
	{MICMD_DATA,    0x02},
	{MICMD_DATA,    0x00},

	{MICMD_CMD,     0xE1},
	{MICMD_DATA,    0x00},
	{MICMD_DATA,    0x24},
	{MICMD_DATA,    0x26},
	{MICMD_DATA,    0x08},
	{MICMD_DATA,    0x10},
	{MICMD_DATA,    0x12},
	{MICMD_DATA,    0x3A},
	{MICMD_DATA,    0x74},
	{MICMD_DATA,    0x4F},
	{MICMD_DATA,    0x08},
	{MICMD_DATA,    0x20},
	{MICMD_DATA,    0x2A},
	{MICMD_DATA,    0x3C},
	{MICMD_DATA,    0x3D},
	{MICMD_DATA,    0x3F},

	{MICMD_CMD,     0x29},
	{CMDDELAY_MS,   10},
	{MICMD_CMD,     0x2C}
};


const tPANEL_CMD tCmdStandbyMI[] = {
	{MICMD_CMD,     0x28},
	{MICMD_CMD,     0x10}
};

const tLCD_PARAM tModeMI[] = {
	/***********       MI Serial Format 1      *************/
	{
		// tPANEL_PARAM
		{
			/* Old prototype */
			PINMUX_LCDMODE_MI_SERIAL_BI,   //!< LCDMode
			1.164f,                        //!< fDCLK
			(0x20 + 128),                   //!< uiHSyncTotalPeriod
			128,                            //!< uiHSyncActivePeriod
			0x10,                          //!< uiHSyncBackPorch
			0x8 + 160,                     //!< uiVSyncTotalPeriod
			160,                            //!< uiVSyncActivePeriod
			0x02,                          //!< uiVSyncBackPorchOdd
			0x02,                          //!< uiVSyncBackPorchEven
			128,                           //!< uiBufferWidth
			160,                           //!< uiBufferHeight
			128,                           //!< uiWindowWidth
			160,                           //!< uiWindowHeight
			FALSE,                         //!< bYCbCrFormat

			/* New added parameters */
			0x01,                           //!< uiHSyncSYNCwidth
			0x01                            //!< uiVSyncSYNCwidth
		},

		// tIDE_PARAM
		{
			/* Old prototype */
			PINMUX_LCD_SEL_SERIAL_MI_SDIO | PINMUX_LCD_SEL_TE_ENABLE, //!< pinmux_select_lcd;
			ICST_CCIR601,                   //!< icst;
			{TRUE, FALSE},                   //!< dithering[2];
			DISPLAY_DEVICE_MI,              //!< **DONT-CARE**
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

		(tPANEL_CMD *)tCmdModeMiFmt3,                 //!< pCmdQueue
		sizeof(tCmdModeMiFmt3) / sizeof(tPANEL_CMD),  //!< nCmd
	}
};

const tLCD_ROT *tRotMI = NULL;

//@}

tLCD_ROT *dispdev_getLcdRotateMiCmd(UINT32 *ModeNumber)
{
#if 0
	if (tRotMI != NULL) {
		*ModeNumber = sizeof(tRotMI) / sizeof(tLCD_ROT);
	} else
#endif
	{
		*ModeNumber = 0;
	}
	return (tLCD_ROT *)tRotMI;
}

tLCD_PARAM *dispdev_getConfigModeMI(UINT32 *ModeNumber)
{
	*ModeNumber = sizeof(tModeMI) / sizeof(tLCD_PARAM);
	return (tLCD_PARAM *)tModeMI;
}

tPANEL_CMD *dispdev_getStandbyCmdMI(UINT32 *CmdNumber)
{
	*CmdNumber = sizeof(tCmdStandbyMI) / sizeof(tPANEL_CMD);
	return (tPANEL_CMD *)tCmdStandbyMI;
}

#if 1
void dispdev_writeToLcdMi(DISPDEV_IOCTRL pDispDevControl, UINT32 addr, UINT32 value)
{
	if (addr & MICMD_DATA) {
		mi_setCmd1(WrDATA, (value & 0xff));
	} else {
		mi_setCmd1(WrCMD, (value & 0xff));
	}

	mi_setCmdCnt(1);
	mi_waitCmdDone();
}


void dispdev_readFromLcdMi(DISPDEV_IOCTRL pDispDevControl, UINT32 addr, UINT32 *pValue)
{
	*pValue = 0;
	DBG_ERR("No MI Read reg implement!\r\n");
}
#endif

void      dispdev_setMiConfig(tMI_CONFIG *pMiConfig)
{
	BOOL    bTeSyncEn   = FALSE;
	FLOAT   MiTargetClk = 40.0; // Max is 60MHz
	//UINT32  Div;

	mi_setCtrl(SERIAL_3WSPI, MI_AUTOFMT3, pMiConfig->DataFmt, FALSE, FALSE, bTeSyncEn, FALSE);
	mi_setSerialCsCtrl(TRUE);
	mi_setPolCtrl(FALSE, FALSE, FALSE, FALSE, SCLMOD0);
	mi_setSerialAutoRsBitValue(TRUE);

#if 0
	Div = (UINT32)(pMiConfig->fMiSrcClk / MiTargetClk);
	pll_setClockRate(PLL_CLKSEL_MI_CLKDIV, PLL_MI_CLKDIV(Div - 1));
#else
	mi_setConfig(MI_CONFIG_ID_FREQ, MiTargetClk * 1000000);
#endif
}

void dispdev_setMiAutoCmd(void)
{
	return;
}