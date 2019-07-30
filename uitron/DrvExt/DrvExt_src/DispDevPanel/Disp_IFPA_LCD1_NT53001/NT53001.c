/*
    Display object object panel configuration parameters for NT53001

    @file      NT53001.c
    @ingroup
    @note       Nothing

    Copyright   Novatek Microelectronics Corp. 2011.  All rights reserved.
*/

#include "display.h"
#include "dispdev_IFParal.h"
#include "dispdev_panlcomn.h"

/*
    Panel Parameters for NT53001
*/
//@{

/*Used in RGB 16-bit through mode, we must trans delta pattern */
const tPANEL_CMD tCmdModeRGB16Through[] = {
	{0x05, 0x00},
	{0x03, 0x63},
	{0x01, 0x01},
	//{0x00, 0x0F},
	//{0x40, 0x03},
	//{0x60, 0x00},
	//{0xC0, 0x06},
};

/*Used in RGB 16-bit da mode, we must trans stripe pattern */
const tPANEL_CMD tCmdModeRGB16Da[] = {
	{0x05, 0x00},
	{0x03, 0x73},
	{0x01, 0x01},
	//{0x40, 0x03},
	//{0x60, 0x01},
	//{0xC0, 0x06},
	//{0xE0, 0x13},
};

/*Used in YUV 16-bit 640 mode */
const tPANEL_CMD tCmdModeYUV16640[] = {
	{0x05, 0x00},
	{0x03, 0x93},
	{0x01, 0x01},
	//{0x20, 0x03},
	//{0x40, 0x03},
	//{0x60, 0x03},
	//{0xC0, 0x06},
};

const tPANEL_CMD tCmdStandbyParal[] = {
	{0x01, 0x00}
};

const tLCD_PARAM tModeParal[] = {
#if 0  //no 1/2, no through
	/***********       RGB16 Through MODE         *************/
	{
		// tPANEL_PARAM
		{
			/* Old prototype */
			PINMUX_LCDMODE_RGB_PARALL_DELTA,//!< LCDMode
			24.00f,                         //!< fDCLK
			762,                            //!< uiHSyncTotalPeriod
			480,                            //!< uiHSyncActivePeriod
			32,                             //!< uiHSyncBackPorch
			525,                            //!< uiVSyncTotalPeriod
			480,                            //!< uiVSyncActivePeriod
			42,                             //!< uiVSyncBackPorchOdd
			42,                             //!< uiVSyncBackPorchEven
			480,                            //!< uiBufferWidth
			480,                            //!< uiBufferHeight
			480,                            //!< uiWindowWidth
			480,                            //!< uiWindowHeight
			FALSE,                          //!< bYCbCrFormat

			/* New added parameters */
			0x01,                           //!< uiHSyncSYNCwidth
			0x01                            //!< uiVSyncSYNCwidth
		},

		// tIDE_PARAM
		{
			/* Old prototype */
			PINMUX_LCD_SEL_RGB_16BITS,      //!< pinmux_select_lcd;
			ICST_CCIR601,                   //!< icst;
			{FALSE, FALSE},                 //!< dithering[2];
			DISPLAY_DEVICE_RGB_16BIT,       //!< **DONT-CARE**
			IDE_PDIR_RBG,                   //!< pdir;
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
			FALSE,                          //!< clamp
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

		(tPANEL_CMD *)tCmdModeRGB16Through,                 //!< pCmdQueue
		sizeof(tCmdModeRGB16Through) / sizeof(tPANEL_CMD),  //!< nCmd
	},
#endif
#if 1 //have 1/2, no through
	/***********       RGB16 Through MODE         *************/
	{
		// tPANEL_PARAM
		{
			/* Old prototype */
			PINMUX_LCDMODE_RGB_PARALL_DELTA,//!< LCDMode
			24.00f,                         //!< fDCLK
			762 * 2,                        //!< uiHSyncTotalPeriod
			480 * 2,                        //!< uiHSyncActivePeriod
			32 * 2,                         //!< uiHSyncBackPorch
			525,                            //!< uiVSyncTotalPeriod
			480,                            //!< uiVSyncActivePeriod
			42,                             //!< uiVSyncBackPorchOdd
			42,                             //!< uiVSyncBackPorchEven
			960,                            //!< uiBufferWidth
			480,                            //!< uiBufferHeight
			960,                            //!< uiWindowWidth
			480,                            //!< uiWindowHeight
			FALSE,                          //!< bYCbCrFormat

			/* New added parameters */
			0x01,                           //!< uiHSyncSYNCwidth
			0x01                            //!< uiVSyncSYNCwidth
		},

		// tIDE_PARAM
		{
			/* Old prototype */
			PINMUX_LCD_SEL_RGB_16BITS,      //!< pinmux_select_lcd;
			ICST_CCIR601,                   //!< icst;
			{FALSE, FALSE},                 //!< dithering[2];
			DISPLAY_DEVICE_RGB_16BIT,       //!< **DONT-CARE**
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
			FALSE,                          //!< clamp
			TRUE,                           //!< tv_powerdown;
			{0x00, 0x00},                   //!< **DONT-CARE**

			/* New added parameters */
			FALSE,                          //!< YCex
			FALSE,                          //!< HLPF
			{FALSE, FALSE, FALSE},          //!< subpixOdd[3]
			{FALSE, FALSE, FALSE},          //!< subpixEven[3]
			{IDE_DITHER_6BITS, IDE_DITHER_6BITS, IDE_DITHER_6BITS}, //!< DitherBits[3]
			TRUE                            //!< clk1/2
		},

		(tPANEL_CMD *)tCmdModeRGB16Through,                 //!< pCmdQueue
		sizeof(tCmdModeRGB16Through) / sizeof(tPANEL_CMD),  //!< nCmd
	},
#endif

#if 0
	/***********       RGB16 DA MODE         *************/
	{
		// tPANEL_PARAM
		{
			/* Old prototype */
			PINMUX_LCDMODE_RGB_PARALL_DELTA,//!< LCDMode
			24.0f,                          //!< fDCLK
			762,                           //!< uiHSyncTotalPeriod
			480,                           //!< uiHSyncActivePeriod
			32,                            //!< uiHSyncBackPorch
			525,                            //!< uiVSyncTotalPeriod
			480,                            //!< uiVSyncActivePeriod
			42,                             //!< uiVSyncBackPorchOdd
			42,                             //!< uiVSyncBackPorchEven
			480,                            //!< uiBufferWidth
			480,                            //!< uiBufferHeight
			480,                            //!< uiWindowWidth
			480,                            //!< uiWindowHeight
			FALSE,                          //!< bYCbCrFormat

			/* New added parameters */
			0x00,                           //!< uiHSyncSYNCwidth
			0x01                            //!< uiVSyncSYNCwidth
		},

		// tIDE_PARAM
		{
			/* Old prototype */
			PINMUX_LCD_SEL_RGB_16BITS,//!< pinmux_select_lcd;
			ICST_CCIR601,                   //!< icst;
			{FALSE, FALSE},                 //!< dithering[2];
			DISPLAY_DEVICE_RGB_16BIT,       //!< **DONT-CARE**
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
			FALSE,                          //!< YCex
			FALSE,                          //!< HLPF
			{FALSE, FALSE, FALSE},             //!< subpixOdd[3]
			{FALSE, FALSE, FALSE},          //!< subpixEven[3]
			{IDE_DITHER_6BITS, IDE_DITHER_6BITS, IDE_DITHER_6BITS}, //!< DitherBits[3]
			FALSE                           //!< clk1/2
		},

		(tPANEL_CMD *)tCmdModeRGB16Da,                 //!< pCmdQueue
		sizeof(tCmdModeRGB16Da) / sizeof(tPANEL_CMD),  //!< nCmd
	},
#endif
	/***********       YUV16 640 MODE         *************/
	{
		// tPANEL_PARAM
		{
			/* Old prototype */
			PINMUX_LCDMODE_CCIR601,         //!< LCDMode
			25.00f,                         //!< fDCLK
			778,                            //!< uiHSyncTotalPeriod
			640,                            //!< uiHSyncActivePeriod
			120,                            //!< uiHSyncBackPorch
			525,                            //!< uiVSyncTotalPeriod
			480,                            //!< uiVSyncActivePeriod
			42,                             //!< uiVSyncBackPorchOdd
			42,                             //!< uiVSyncBackPorchEven
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

	uiSIFData = ((((UINT32)addr << 9) | (UINT32)value) << 16);

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

	uiSIFData = (UINT32)addr << 9 | (UINT32)value;

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
