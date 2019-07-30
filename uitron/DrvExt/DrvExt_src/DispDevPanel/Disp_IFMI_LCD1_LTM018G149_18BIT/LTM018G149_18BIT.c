/*
    Display object object Golden Sample for driving Memory interface panel

    @file       LTM018G149.c
    @ingroup
    @note       Nothing

    Copyright   Novatek Microelectronics Corp. 2011.  All rights reserved.
*/

#include "display.h"
#include "dispdev_IFmi.h"
#include "dispdev_panlcomn.h"


/*
    Panel Parameters for LTM018G149
*/
//@{
/*Used in MI format-10 RGB666 18bits bus*/
const tPANEL_CMD tCmdModeMiFmt10[] = {
	{0x0000,        0x0001},
	{CMDDELAY_MS,   10},
	{0x0007,        0x0000},
	{CMDDELAY_MS,   10},
	{0x0003,        0x0410},
	{0x0009,        0x010C},
	{0x000D,        0x0008},
	{CMDDELAY_MS,   10},
	{0x0009,        0x0104},
	{CMDDELAY_MS,   10},
	{0x000D,        0x0018},
	{CMDDELAY_MS,   10},
	{0x0001,        0x0015},
	{CMDDELAY_MS,   10},
	{0x0005,        0x1010},
	{0x0008,        0x0503},
	{0x000B,        0x4400},
	{0x000C,        0x0003},
	{0x000E,        0x2B14},
	{0x000A,        0x0000},

	{0x000A,        0x0000},    // FMT-10

	{0x0007,        0x0005},
	{0x0007,        0x0025},
	{0x0007,        0x0027},
	{0x0007,        0x0037},
};


const tPANEL_CMD *tCmdStandbyMI = NULL;

const tLCD_PARAM tModeMI[] = {
	/***********       MI Format 10 - 18bits bus RGB666      *************/
	{
		// tPANEL_PARAM
		{
			/* Old prototype */
			PINMUX_LCDMODE_MI_FMT10,         //!< LCDMode
#if (_FPGA_EMULATION_)
			0.9f,                           //!<  fDCLK
#else
			2.7f,                           //!< fDCLK
#endif
			(0x60 + 128),                   //!< uiHSyncTotalPeriod
			128,                            //!< uiHSyncActivePeriod
			0x2D,                           //!< uiHSyncBackPorch
			0x24 + 160,                     //!< uiVSyncTotalPeriod
			160,                            //!< uiVSyncActivePeriod
			0x2,                            //!< uiVSyncBackPorchOdd
			0x2,                            //!< uiVSyncBackPorchEven
			128,                            //!< uiBufferWidth
			160,                            //!< uiBufferHeight
			128,                            //!< uiWindowWidth
			160,                            //!< uiWindowHeight
			FALSE,                          //!< bYCbCrFormat

			/* New added parameters */
			0x01,                           //!< uiHSyncSYNCwidth
			0x01                            //!< uiVSyncSYNCwidth
		},

		// tIDE_PARAM
		{
			/* Old prototype */
			PINMUX_LCD_SEL_PARALLE_MI_18BITS,//!< pinmux_select_lcd;
			ICST_CCIR601,                   //!< icst;
			{FALSE, FALSE},                   //!< dithering[2];
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
			{IDE_DITHER_6BITS, IDE_DITHER_6BITS, IDE_DITHER_6BITS}, //!< DitherBits[3]
			FALSE                           //!< clk1/2
		},

		(tPANEL_CMD *)tCmdModeMiFmt10,                 //!< pCmdQueue
		sizeof(tCmdModeMiFmt10) / sizeof(tPANEL_CMD),  //!< nCmd
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
#if 0
	*CmdNumber = sizeof(tCmdStandbyMI) / sizeof(tPANEL_CMD);
#else
	*CmdNumber = 0;
#endif
	return (tPANEL_CMD *)tCmdStandbyMI;
}

#if 1
void dispdev_writeToLcdMi(DISPDEV_IOCTRL pDispDevControl, UINT32 addr, UINT32 value)
{
	//mi_setCmd1(WrCMD, ((addr&0xff00)>>8)<<1);
	//mi_setCmd2(WrCMD, (addr&0x00ff)<<1);
	//mi_setCmd3(WrDATA,((value&0xff00)>>8)<<1);
	//mi_setCmd4(WrDATA,(value&0x00ff)<<1);
	//mi_setCmdCnt(4);
	mi_setCmd1(WrCMD, (((addr & 0xff00) << 1) + (addr & 0x00ff)) << 1);
	mi_setCmd2(WrDATA, (((value & 0xff00) << 1) + (value & 0x00ff)) << 1);
	mi_setCmdCnt(2);

	mi_waitCmdDone();
}


void dispdev_readFromLcdMi(DISPDEV_IOCTRL pDispDevControl, UINT32 addr, UINT32 *pValue)
{
	UINT32            High = 0, Low = 0;
	UINT32            uireg = 0;

	mi_setCmd1(WrCMD, (((addr & 0xff00) << 1) + ((addr & 0x00ff))) << 1);
	mi_setCmd2(RdDATA, 0x0);
	mi_setCmdCnt(2);
	mi_waitCmdDone();

	// Read ID
	uireg    = mi_getReadBack(2);
	High = ((uireg & 0x3FC00) >> 2);
	Low = ((uireg & 0x001FF) >> 1);

	//DBG_ERR("Addr(0x%4X) got 0x%2X   0x%2X \r\n",addr,High,Low);
	//*pValue = (High<<9)+Low;
	*pValue = High + Low;
}
#endif

void      dispdev_setMiConfig(tMI_CONFIG *pMiConfig)
{
	BOOL    bTeSyncEn   = FALSE;
#if (_FPGA_EMULATION_)
	FLOAT   MiTargetClk = 6.00; // Max is 60MHz
#else
	FLOAT   MiTargetClk = 26.66; // Max is 60MHz
#endif
	//UINT32  Div;

#if 1
	mi_setCtrl(PARALLEL_I80, MI_AUTOFMT3, pMiConfig->DataFmt, FALSE, FALSE, bTeSyncEn, FALSE); // Test I80
#else
	mi_setCtrl(PARALLEL_M68, MI_AUTOFMT3, pMiConfig->DataFmt, FALSE, FALSE, bTeSyncEn, FALSE); //Test M68
#endif
	mi_setSerialAutoRsBitValue(TRUE);

#if 0
	Div = (UINT32)(pMiConfig->fMiSrcClk / MiTargetClk);
	if (Div == 0) {
		pll_setClockRate(PLL_CLKSEL_MI_CLKDIV, PLL_MI_CLKDIV(Div));
	} else {
		pll_setClockRate(PLL_CLKSEL_MI_CLKDIV, PLL_MI_CLKDIV(Div - 1));
	}
#else
	mi_setConfig(MI_CONFIG_ID_FREQ, MiTargetClk * 1000000);
#endif
}

void dispdev_setMiAutoCmd(void)
{
	/* Enable AUTO Mode */
	//mi_setCmd1(WrCMD, 0x00<<1);
	//mi_setCmd2(WrCMD, 0x21<<1);
	//mi_setCmd3(WrDATA,0x9F<<1);
	//mi_setCmd4(WrDATA,0x00<<1);
	//mi_setCmd5(WrCMD, 0x00<<1);
	//mi_setCmd6(WrCMD, 0x22<<1);
	//mi_setCmdCnt(6);
	mi_setCmd1(WrCMD, ((0x00 << 9) + (0x21)) << 1);
	mi_setCmd2(WrDATA, ((0x9F << 9) + (0x00)) << 1);
	mi_setCmd3(WrCMD, ((0x00 << 9) + (0x22)) << 1);
	mi_setCmdCnt(3);
	mi_waitCmdDone();
}