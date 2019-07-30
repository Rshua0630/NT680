/*
    Display object object panel configuration parameters for A030VN01

    @file      A030VN01.c
    @ingroup
    @note       Nothing

    Copyright   Novatek Microelectronics Corp. 2011.  All rights reserved.
*/

#include "display.h"
#include "dispdev_IFParal.h"
#include "dispdev_panlcomn.h"

/*
    Panel Parameters for A030VN01
*/
//@{

/*Used in YUV 16-bit 640 mode */
const tPANEL_CMD tCmdModeYUV16640[] = {
	{0x01, 0x39},
	{0x0A, 0x03}
};

const tPANEL_CMD tCmdStandbyParal[] = {
	{0x0A, 0x02}
};

const tLCD_PARAM tModeParal[] = {
	/***********       YUV16 640 MODE         *************/
	{
		// tPANEL_PARAM
		{
			/* Old prototype */
			PINMUX_LCDMODE_CCIR601,         //!< LCDMode
#if (_FPGA_EMULATION_ != ENABLE)
			24.00f,                         //!< fDCLK
#else //for FPGA
			12.00f,                         //!< fDCLK
#endif
			762,                            //!< uiHSyncTotalPeriod
			640,                            //!< uiHSyncActivePeriod
			81,                            //!< uiHSyncBackPorch
			525,                            //!< uiVSyncTotalPeriod
			480,                            //!< uiVSyncActivePeriod
			27,                             //!< uiVSyncBackPorchOdd
			27,                             //!< uiVSyncBackPorchEven
			640,                            //!< uiBufferWidth
			480,                            //!< uiBufferHeight
			640,                            //!< uiWindowWidth
			480,                            //!< uiWindowHeight
			TRUE,                          //!< bYCbCrFormat

			/* New added parameters */
			0x01,                           //!< uiHSyncSYNCwidth
			0x01                            //!< uiVSyncSYNCwidth
		},

		// tIDE_PARAM
		{
			/* Old prototype */
			PINMUX_LCD_SEL_CCIR601_16BITS,  //!< pinmux_select_lcd;
			ICST_CCIR601,                   //!< icst;
			{FALSE, FALSE},                 //!< dithering[2];
			DISPLAY_DEVICE_CCIR601_16BIT,   //!< **DONT-CARE**
			IDE_PDIR_RBG,                   //!< pdir;
			IDE_LCD_R,                      //!< odd;
			IDE_LCD_R,                      //!< even;
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
			FALSE,                          //!< clamp
			TRUE,                           //!< tv_powerdown;
			{0x00, 0x00},                   //!< **DONT-CARE**

			/* New added parameters */
			TRUE,                           //!< YCex
			FALSE,                          //!< HLPF
			{FALSE, FALSE, FALSE},          //!< subpixOdd[3]
			{FALSE, FALSE, FALSE},          //!< subpixEven[3]
			{IDE_DITHER_6BITS, IDE_DITHER_6BITS, IDE_DITHER_6BITS}, //!< DitherBits[3]
			FALSE                           //!< clk1/2
		},

		(tPANEL_CMD *)tCmdModeYUV16640,                 //!< pCmdQueue
		sizeof(tCmdModeYUV16640) / sizeof(tPANEL_CMD),  //!< nCmd
	}
};

const tLCD_ROT *tRotateParal = NULL;

//@}

tLCD_ROT *dispdev_getLcdRotateCmdParal(UINT32 *ModeNumber)
{
//	if (tRotateParal != NULL) {
//		*ModeNumber = sizeof(tRotateParal) / sizeof(tLCD_ROT);
//	} else {
		*ModeNumber = 0;
//	}
	return (tLCD_ROT *)tRotateParal;
}

tLCD_PARAM *dispdev_getConfigModeParal(UINT32 *ModeNumber)
{
	*ModeNumber = sizeof(tModeParal) / sizeof(tLCD_PARAM);
	return (tLCD_PARAM *)tModeParal;
}

tPANEL_CMD *dispdev_getStandbyCmdParal(UINT32 *CmdNumber)
{
	*CmdNumber = sizeof(tCmdStandbyParal) / sizeof(tPANEL_CMD);
	return (tPANEL_CMD *)tCmdStandbyParal;
}

#if 1
void dispdev_writeToLcdSifParal(DISPDEV_IOCTRL pDispDevControl, UINT32 addr, UINT32 value)
{
	UINT32                  uiSIFData;
	DISPDEV_IOCTRL_PARAM    DevIOCtrl;

	uiSIFData = ((((UINT32)addr << 8) | (UINT32)value) << 16);

	pDispDevControl(DISPDEV_IOCTRL_GET_REG_IF, &DevIOCtrl);
	sif_send(DevIOCtrl.SEL.GET_REG_IF.uiSifCh, uiSIFData, 0, 0);

}


void dispdev_writeToLcdGpioParal(DISPDEV_IOCTRL pDispDevControl, UINT32 addr, UINT32 value)
{
	UINT32                  uiSIFData, j;
	UINT32                  SifClk, SifSen, SifData;
	DISPDEV_IOCTRL_PARAM    DevIOCtrl;

	pDispDevControl(DISPDEV_IOCTRL_GET_REG_IF, &DevIOCtrl);
	SifClk  = DevIOCtrl.SEL.GET_REG_IF.uiGpioClk;
	SifSen  = DevIOCtrl.SEL.GET_REG_IF.uiGpioSen;
	SifData = DevIOCtrl.SEL.GET_REG_IF.uiGpioData;

	gpio_setDir(SifSen, GPIO_DIR_OUTPUT);
	gpio_setDir(SifClk, GPIO_DIR_OUTPUT);
	gpio_setDir(SifData, GPIO_DIR_OUTPUT);

	gpio_setPin(SifSen);
	gpio_setPin(SifData);
	gpio_setPin(SifClk);

	uiSIFData = ((UINT32)addr << 8) | (UINT32)value;

	Delay_DelayUs(500);

	gpio_clearPin(SifSen);

	for (j = 16; j > 0; j--) {
		if (((uiSIFData >> (j - 1)) & 0x01)) {
			gpio_setPin(SifData);
		} else {
			gpio_clearPin(SifData);
		}

		Delay_DelayUs(100);
		gpio_clearPin(SifClk);
		Delay_DelayUs(200);
		gpio_setPin(SifClk);
		Delay_DelayUs(100);
	}

	Delay_DelayUs(500);
	gpio_setPin(SifSen);

}



void dispdev_readFromLcdGpioParal(DISPDEV_IOCTRL pDispDevControl, UINT32 addr, UINT32 *p_value)
{
	DBG_ERR("Has not implement dispdev_readFromLcdGpioParal \r\n");
}
#endif
