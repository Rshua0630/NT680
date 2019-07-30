/*
    Display object object panel configuration parameters for AUCN01

    @file       AUCN01.c
    @ingroup
    @note       Nothing

    Copyright   Novatek Microelectronics Corp. 2011.  All rights reserved.
*/

#include "kernel.h"
#include "display.h"
#include "dispdev_IF8bits.h"
#include "dispdev_panlcomn.h"

/*
    Panel Parameters for AUCN01
*/
//@{

/*Used in RGB-Serial & RGB Through*/
const tPANEL_CMD tCmdModeThrough[] = {
	{0x00, 0x07},
	{0x00, 0x0F},
	{0x40, 0x03},
	{0x60, 0x00},
	{0xC0, 0x06},
};

/*Used in RGB-Dummy 320*/
const tPANEL_CMD tCmdModeRgbd320[] = {
	{0x00, 0x0F},
	{0x40, 0x03},
	{0x60, 0x01},
	{0xC0, 0x06},
	{0xE0, 0x13},
};

/*Used in YUV640 & CCIR601*/
const tPANEL_CMD tCmdModeYUV640[] = {
	{0x00, 0x0F},
	{0x20, 0x03},
	{0x40, 0x03},
	{0x60, 0x03},
	{0xC0, 0x06},
};

/*Used in CCIR656*/
const tPANEL_CMD tCmdModeCCIR656[] = {
	{0x00, 0x0F},
	{0x40, 0x03},
	{0x60, 0x07},
	{0xC0, 0x06},
};

const tPANEL_CMD tCmdStandby[] = {
	{0x00, 0x08}
};

const tLCD_PARAM tMode[] = {
	/***********       CCIR601 MODE         *************/
	{
		// tPANEL_PARAM
		{
			/* Old prototype */
			PINMUX_LCDMODE_CCIR601,         //!< LCDMode
#if (_FPGA_EMULATION_ == ENABLE)
			(_FPGA_PLL_OSC_ / 4000000),     //!< fDCLK
#else
			24.55f,                         //!< fDCLK
#endif
			780,                           //!< uiHSyncTotalPeriod
			640,                           //!< uiHSyncActivePeriod
			126,                            //!< uiHSyncBackPorch
			262,                            //!< uiVSyncTotalPeriod
			240,                            //!< uiVSyncActivePeriod
			13,                             //!< uiVSyncBackPorchOdd
			13,                             //!< uiVSyncBackPorchEven
			640,                            //!< uiBufferWidth
			240,                            //!< uiBufferHeight
			640,                            //!< uiWindowWidth
			240,                            //!< uiWindowHeight
			TRUE,                           //!< bYCbCrFormat(YCC 8bit)

			/* New added parameters */
			0x00,                           //!< uiHSyncSYNCwidth
			0x00                            //!< uiVSyncSYNCwidth
		},

		// tIDE_PARAM
		{
			/* Old prototype */
			PINMUX_LCD_SEL_CCIR601,         //!< pinmux_select_lcd;
			ICST_CCIR601,                   //!< icst;
			{FALSE, FALSE},                 //!< dithering[2];
			DISPLAY_DEVICE_AU,              //!< **DONT-CARE**
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

		(tPANEL_CMD *)tCmdModeYUV640,                 //!< pCmdQueue
		sizeof(tCmdModeYUV640) / sizeof(tPANEL_CMD),  //!< nCmd
	},

	/***********       RGB Dummy-320 MODE         *************/
	{
		// tPANEL_PARAM
		{
			/* Old prototype */
			PINMUX_LCDMODE_RGBD320,         //!< LCDMode
#if (_FPGA_EMULATION_ == ENABLE)
			12.0f,                          //!< fDCLK
#else
			9.0f,
#endif
			448,                            //!< uiHSyncTotalPeriod
			320,                            //!< uiHSyncActivePeriod
			63,                             //!< uiHSyncBackPorch
			262,                            //!< uiVSyncTotalPeriod
			240,                            //!< uiVSyncActivePeriod
			12,                             //!< uiVSyncBackPorchOdd
			12,                             //!< uiVSyncBackPorchEven
			320,                            //!< uiBufferWidth
			240,                            //!< uiBufferHeight
			320,                            //!< uiWindowWidth
			240,                            //!< uiWindowHeight
			FALSE,                          //!< bYCbCrFormat

			/* New added parameters */
			0x00,                           //!< uiHSyncSYNCwidth
			0x01                            //!< uiVSyncSYNCwidth
		},

		// tIDE_PARAM
		{
			/* Old prototype */
			PINMUX_LCD_SEL_SERIAL_RGB_8BITS,//!< pinmux_select_lcd;
			ICST_CCIR601,                   //!< icst;
			{FALSE, FALSE},                 //!< dithering[2];
			DISPLAY_DEVICE_AU,              //!< **DONT-CARE**
			IDE_PDIR_RGB,                   //!< pdir;
			IDE_LCD_B,                      //!< odd;
			IDE_LCD_B,                      //!< even;
			TRUE,                           //!< hsinv;
			TRUE,                           //!< vsinv;
			FALSE,                          //!< hvldinv;
			FALSE,                          //!< vvldinv;
			FALSE,                          //!< clkinv;
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

		(tPANEL_CMD *)tCmdModeRgbd320,                 //!< pCmdQueue
		sizeof(tCmdModeRgbd320) / sizeof(tPANEL_CMD),  //!< nCmd
	},

	/***********       RGB-SERIAL MODE         *************/
	{
		// tPANEL_PARAM
		{
			/* Old prototype */
			PINMUX_LCDMODE_RGB_SERIAL,      //!< LCDMode
#if (_FPGA_EMULATION_ == ENABLE)
			8.0f,                           //!< fDCLK
#else
			//9.7f,                           //!< fDCLK
			15,
#endif
			616,                            //!< uiHSyncTotalPeriod
			480,                            //!< uiHSyncActivePeriod
			100,                            //!< uiHSyncBackPorch
			262,                            //!< uiVSyncTotalPeriod
			240,                            //!< uiVSyncActivePeriod
			18,                             //!< uiVSyncBackPorchOdd
			18,                             //!< uiVSyncBackPorchEven
			480,                            //!< uiBufferWidth
			240,                            //!< uiBufferHeight
			480,                            //!< uiWindowWidth
			240,                            //!< uiWindowHeight
			FALSE,                          //!< bYCbCrFormat

			/* New added parameters */
			0x00,                           //!< uiHSyncSYNCwidth
			0x05                            //!< uiVSyncSYNCwidth
		},

		// tIDE_PARAM
		{
			/* Old prototype */
			PINMUX_LCD_SEL_SERIAL_RGB_8BITS,//!< pinmux_select_lcd;
			ICST_CCIR601,                   //!< icst;
			{FALSE, FALSE},                 //!< dithering[2];
			DISPLAY_DEVICE_AU,              //!< **DONT-CARE**
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
			{TRUE, TRUE, TRUE},             //!< subpixOdd[3]
			{FALSE, FALSE, FALSE},          //!< subpixEven[3]
			{IDE_DITHER_6BITS, IDE_DITHER_6BITS, IDE_DITHER_6BITS}, //!< DitherBits[3]
			FALSE                           //!< clk1/2
		},

		(tPANEL_CMD *)tCmdModeThrough,                 //!< pCmdQueue
		sizeof(tCmdModeThrough) / sizeof(tPANEL_CMD),  //!< nCmd
	},

	/***********       THROUGH MODE         *************/
	{
		// tPANEL_PARAM
		{
			/* Old prototype */
			PINMUX_LCDMODE_RGB_THROUGH,     //!< LCDMode
#if (_FPGA_EMULATION_ == ENABLE)
			8.0f,                           //!< fDCLK
#else
			6.6f,                          	//!< fDCLK: IDE clock = 1/3X out I/F clock
#endif
			228,                            //!< uiHSyncTotalPeriod
			160,                            //!< uiHSyncActivePeriod
			33,                             //!< uiHSyncBackPorch
			254,                            //!< uiVSyncTotalPeriod
			240,                            //!< uiVSyncActivePeriod
			12,                             //!< uiVSyncBackPorchOdd
			12,                             //!< uiVSyncBackPorchEven
			160,                            //!< uiBufferWidth
			240,                            //!< uiBufferHeight
			160,                            //!< uiWindowWidth
			240,                            //!< uiWindowHeight
			FALSE,                          //!< bYCbCrFormat

			/* New added parameters */
			0x00,                           //!< uiHSyncSYNCwidth
			0x05                            //!< uiVSyncSYNCwidth
		},

		// tIDE_PARAM
		{
			/* Old prototype */
			PINMUX_LCD_SEL_SERIAL_RGB_8BITS,//!< pinmux_select_lcd;
			ICST_CCIR601,                   //!< icst;
			{FALSE, FALSE},                 //!< dithering[2];
			DISPLAY_DEVICE_AU,              //!< **DONT-CARE**
			IDE_PDIR_RGB,                   //!< pdir;
			IDE_LCD_B,                      //!< odd;
			IDE_LCD_R,                      //!< even;
			TRUE,                           //!< hsinv;
			TRUE,                           //!< vsinv;
			FALSE,                          //!< hvldinv;
			FALSE,                          //!< vvldinv;
			TRUE,                          //!< clkinv;
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

		(tPANEL_CMD *)tCmdModeThrough,                 //!< pCmdQueue
		sizeof(tCmdModeThrough) / sizeof(tPANEL_CMD),  //!< nCmd
	},

	/***********       CCIR656 MODE         *************/
	{
		// tPANEL_PARAM
		{
			/* Old prototype */
			PINMUX_LCDMODE_CCIR656,         //!< LCDMode
#if _FPGA_EMULATION_ == ENABLE
			12.0f,                           //!< fDCLK
#else
			24.0f,                          //!< fDCLK
#endif
			858,                           //!< uiHSyncTotalPeriod
			720,                           //!< uiHSyncActivePeriod
			136,                            //!< uiHSyncBackPorch
			525,                            //!< uiVSyncTotalPeriod
			480,                            //!< uiVSyncActivePeriod
			36,                             //!< uiVSyncBackPorchOdd
			37,                             //!< uiVSyncBackPorchEven
			720,                            //!< uiBufferWidth
			480,                            //!< uiBufferHeight
			720,                            //!< uiWindowWidth
			480,                            //!< uiWindowHeight
			TRUE,                           //!< bYCbCrFormat

			/* New added parameters */
			0x00,                           //!< uiHSyncSYNCwidth
			0x05                            //!< uiVSyncSYNCwidth
		},

		// tIDE_PARAM
		{
			/* Old prototype */
			PINMUX_LCD_SEL_CCIR656 | PINMUX_LCD_SEL_DE_ENABLE, //!< pinmux_select_lcd;
			ICST_CCIR601,                   //!< icst;
			{FALSE, FALSE},                 //!< dithering[2];
			DISPLAY_DEVICE_AU,              //!< **DONT-CARE**
			IDE_PDIR_RGB,                   //!< pdir;
			IDE_LCD_R,                      //!< odd;
			IDE_LCD_G,                      //!< even;
			FALSE,                          //!< hsinv;
			FALSE,                          //!< vsinv;
			FALSE,                          //!< hvldinv;
			FALSE,                          //!< vvldinv;
			TRUE,                          //!< clkinv;
			FALSE,                          //!< fieldinv;
			FALSE,                          //!< **DONT-CARE**
			TRUE,                           //!< interlace;
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

		(tPANEL_CMD *)tCmdModeCCIR656,                 //!< pCmdQueue
		sizeof(tCmdModeCCIR656) / sizeof(tPANEL_CMD),  //!< nCmd
	},


	/***********       YUV640 MODE         *************/
	{
		// tPANEL_PARAM
		{
			/* Old prototype */
			PINMUX_LCDMODE_YUV640,          //!< LCDMode
#if (_EMULATION_ == ENABLE)
#if (_FPGA_EMULATION_ == DISABLE)
			// RealChip emulation (Including MT)
			//27.0f,                          //!< fDCLK
			//24.55,
			12.6,
#else
			// FPGA (Recude DMA bandwidth)
			//(_FPGA_PLL_OSC_ / 4000000),     //!< fDCLK,
			10,
#endif
#else
			24.55f,                         //!< fDCLK
#endif
			780,                           //!< uiHSyncTotalPeriod
			640,                           //!< uiHSyncActivePeriod
			126,                            //!< uiHSyncBackPorch
			262,                            //!< uiVSyncTotalPeriod
			240,                            //!< uiVSyncActivePeriod
			13,                             //!< uiVSyncBackPorchOdd
			13,                             //!< uiVSyncBackPorchEven
			640,                            //!< uiBufferWidth
			240,                            //!< uiBufferHeight
			640,                            //!< uiWindowWidth
			240,                            //!< uiWindowHeight
			TRUE,                          //!< bYCbCrFormat

			/* New added parameters */
			0x00,                           //!< uiHSyncSYNCwidth
			0x00                            //!< uiVSyncSYNCwidth
		},

		// tIDE_PARAM
		{
			/* Old prototype */
			PINMUX_LCD_SEL_SERIAL_YCbCr_8BITS,//!< pinmux_select_lcd;
			ICST_CCIR601,                   //!< icst;
			{FALSE, FALSE},                 //!< dithering[2];
			DISPLAY_DEVICE_AU,              //!< **DONT-CARE**
			IDE_PDIR_RGB,                   //!< pdir;
			IDE_LCD_R,                      //!< odd;
			IDE_LCD_G,                      //!< even;
			TRUE,                           //!< hsinv;
			TRUE,                           //!< vsinv;
			FALSE,                          //!< hvldinv;
			FALSE,                          //!< vvldinv;
			TRUE,                          //!< clkinv;
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

		(tPANEL_CMD *)tCmdModeYUV640,                 //!< pCmdQueue
		sizeof(tCmdModeYUV640) / sizeof(tPANEL_CMD),  //!< nCmd
	}
};

const tLCD_ROT *tRotate = NULL;

//@}

tLCD_ROT *dispdev_getLcdRotateCmd(UINT32 *ModeNumber)
{
//	if (tRotate != NULL) {
//		*ModeNumber = sizeof(tRotate) / sizeof(tLCD_ROT);
//	} else {
		*ModeNumber = 0;
//	}
	return (tLCD_ROT *)tRotate;
}

tLCD_PARAM *dispdev_getConfigMode(UINT32 *ModeNumber)
{
	*ModeNumber = sizeof(tMode) / sizeof(tLCD_PARAM);
	return (tLCD_PARAM *)tMode;
}

tPANEL_CMD *dispdev_getStandbyCmd(UINT32 *CmdNumber)
{
	*CmdNumber = sizeof(tCmdStandby) / sizeof(tPANEL_CMD);
	return (tPANEL_CMD *)tCmdStandby;
}

#if 1
void dispdev_writeToLcdSif(DISPDEV_IOCTRL pDispDevControl, UINT32 addr, UINT32 value)
{
	UINT32                  uiSIFData;
	DISPDEV_IOCTRL_PARAM    DevIOCtrl;

	uiSIFData = ((((UINT32)addr << 8) | (UINT32)value) << 16);

	pDispDevControl(DISPDEV_IOCTRL_GET_REG_IF, &DevIOCtrl);
	sif_send(DevIOCtrl.SEL.GET_REG_IF.uiSifCh, uiSIFData, 0, 0);

}


void dispdev_writeToLcdGpio(DISPDEV_IOCTRL pDispDevControl, UINT32 addr, UINT32 value)
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

	uiSIFData = (UINT32)addr << 8 | (UINT32)value;

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



void dispdev_readFromLcdGpio(DISPDEV_IOCTRL pDispDevControl, UINT32 addr, UINT32 *p_value)
{
	DBG_ERR("Has not implement dispdev_readFromLcdGpio \r\n");
}
#endif
