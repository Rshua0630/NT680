/*
    Display object object panel configuration parameters for SII9024A

    @file      SII9024A.c
    @ingroup
    @note       Nothing

    Copyright   Novatek Microelectronics Corp. 2011.  All rights reserved.
*/

#include "display.h"
#include "dispdev_IFParal.h"
#include "dispdev_panlcomn.h"

#define IDE_SIE_CCIR601_8_bit       0
#define IDE_SIE_CCIR601_16_bit      1

#define IDE_SIE_TEST_CCIR   IDE_SIE_CCIR601_16_bit

/*
    Panel Parameters forSII9024A
*/
//@{

/*Used in RGB 16-bit through mode, we must trans delta pattern */
static const tPANEL_CMD tCmdModeCCIR65616bit[] = {
	//{0x05, 0x00},
	//{0x03, 0x63},
	//{0x01, 0x01},
};

/*Used in YUV 16-bit 640 mode */
static const tPANEL_CMD tCmdModeYUV16640[] = {
	//{0x05, 0x00},
	//{0x03, 0x93},
	//{0x01, 0x01},
};


static const tPANEL_CMD tCmdStandbyParal[] = {
	//{0x01, 0x00}
};

/*Used in YUV640 & CCIR601*/
static const tPANEL_CMD tCmdModeYUV640[] = {
	{0x00, 0x0F},
	{0x20, 0x03},
	{0x40, 0x03},
	{0x60, 0x03},
	{0xC0, 0x06},
};

#define BIG_MAP 0
const tLCD_PARAM tModeParal[] = {

	/***********       CCIR656 16-bit MODE         *************/
	{
		// tPANEL_PARAM
		{
#if 1
			/* Old prototype */
			PINMUX_LCDMODE_CCIR656,         //!< LCDMode
#if _FPGA_EMULATION_ == ENABLE
			//!< For IDE -> SIE usage
			//!< Also need to configure @ Pinmux table
			/*
			{PIN_FUNC_SENSOR,           PIN_SENSOR_CFG_NONE},
			{PIN_FUNC_SENSOR2,          PIN_SENSOR2_CFG_CCIR16BITS},
			{PIN_FUNC_SENSOR3,          PIN_SENSOR3_CFG_NONE},
			{PIN_FUNC_SENSOR4,          PIN_SENSOR4_CFG_NONE},
			{PIN_FUNC_MIPI_LVDS,        PIN_MIPI_LVDS_CFG_NONE},
			*/
			1.0f,                           //!< fDCLK
#else

			27.00f,                         //!< fDCLK
#endif

#if !BIG_MAP
			1716 / 2,                       //!< uiHSyncTotalPeriod
			1440 / 2,                       //!< uiHSyncActivePeriod
#else
			2058,
			1920,
#endif
			(124 + 114) / 2,                //!< uiHSyncBackPorch
#if !BIG_MAP
			525,                            //!< uiVSyncTotalPeriod
			480,                            //!< uiVSyncActivePeriod
#else
			1125,
			1080,
#endif
			36,                             //!< uiVSyncBackPorchOdd => 0xD8 bit[10:0]@dispdev_panlcomn.c line396
			37,                             //!< uiVSyncBackPorchEven
#if !BIG_MAP
			720,                            //!< uiBufferWidth
			480,                            //!< uiBufferHeight
			720,                            //!< uiWindowWidth
			480,                            //!< uiWindowHeight
#else
			1920,                           //!< uiBufferWidth
			1080,                           //!< uiBufferHeight
			1920,                           //!< uiWindowWidth
			1080,                           //!< uiWindowHeight
#endif
			TRUE,                           //!< bYCbCrFormat

			/* New added parameters */
			124 / 2,                        //!< uiHSyncSYNCwidth
			0x6                             //!< uiVSyncSYNCwidth
#else
			/* Old prototype */
			PINMUX_LCDMODE_CCIR656,         //!< LCDMode
			1.00f,                         //!< fDCLK
			2200,                           //!< uiHSyncTotalPeriod
			1920,                           //!< uiHSyncActivePeriod
			192,                            //!< uiHSyncBackPorch
			1125,                          //!< uiVSyncTotalPeriod
			1080,                            //!< uiVSyncActivePeriod
			40,                             //!< uiVSyncBackPorchOdd
			41,                             //!< uiVSyncBackPorchEven
			1920,                            //!< uiBufferWidth
			1080,                            //!< uiBufferHeight
			1920,                            //!< uiWindowWidth
			1080,                            //!< uiWindowHeight
			TRUE,                          //!< bYCbCrFormat

			/* New added parameters */
			0x2B,                           //!< uiHSyncSYNCwidth
			0x9                            //!< uiVSyncSYNCwidth
#endif
		},

		// tIDE_PARAM
		{
			/* Old prototype */
			PINMUX_LCD_SEL_CCIR656_16BITS/*|PINMUX_LCD_SEL_DE_ENABLE*/,  //!< pinmux_select_lcd;
			ICST_CCIR601,                   //!< icst;
			{FALSE, FALSE},                 //!< dithering[2];
			DISPLAY_DEVICE_CCIR656_16BIT,   //!< **DONT-CARE**
			IDE_PDIR_RGB,                   //!< pdir;
			IDE_LCD_R,                      //!< odd;
			IDE_LCD_G,                      //!< even;
			FALSE,                           //!< hsinv;
			FALSE,                           //!< vsinv;
			FALSE,                          //!< hvldinv;
			FALSE,                          //!< vvldinv;
			TRUE,                           //!< clkinv;
			FALSE,                          //!< fieldinv;
			FALSE,                          //!< **DONT-CARE**
			TRUE,                           //!< interlace;
			FALSE,                          //!< **DONT-CARE**
			0x40,                           //!< ctrst;
			0x00,                           //!< brt;
			0x40,                           //!< cmults;
			FALSE,                          //!< cex;
			FALSE,                          //!< **DONT-CARE**
			TRUE,                           //!< clamp
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

		(tPANEL_CMD *)tCmdModeCCIR65616bit,                 //!< pCmdQueue
		sizeof(tCmdModeCCIR65616bit) / sizeof(tPANEL_CMD),  //!< nCmd
	},

	/***********       RGB16 bits MODE         *************/
	{
		// tPANEL_PARAM
		{
			/* Old prototype */
			PINMUX_LCDMODE_RGB_PARALL_DELTA,//!< LCDMode
#if (_FPGA_EMULATION_ == ENABLE)
			1.0f,                           //!< fDCLK
#else
			24.55f,                         //!< fDCLK
#endif
			880,//778,                      //!< uiHSyncTotalPeriod
			640,                            //!< uiHSyncActivePeriod
			120,                            //!< uiHSyncBackPorch
			540,                            //!< uiVSyncTotalPeriod
			480,                            //!< uiVSyncActivePeriod
			42,                             //!< uiVSyncBackPorchOdd
			42,                             //!< uiVSyncBackPorchEven
			640,                            //!< uiBufferWidth
			480,                            //!< uiBufferHeight
			640,                            //!< uiWindowWidth
			480,                            //!< uiWindowHeight
			FALSE,                          //!< bYCbCrFormat

			/* New added parameters */
			0x01,                           //!< uiHSyncSYNCwidth
			0x01                            //!< uiVSyncSYNCwidth
		},

		// tIDE_PARAM
		{
			/* Old prototype */
			PINMUX_LCD_SEL_RGB_16BITS,      //!< pinmux_select_lcd : Note: during IDE to SIE path, DE can not enable
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
			TRUE,                           //!< YCex
			FALSE,                          //!< HLPF
			{FALSE, FALSE, FALSE},          //!< subpixOdd[3]
			{FALSE, FALSE, FALSE},          //!< subpixEven[3]
			{IDE_DITHER_6BITS, IDE_DITHER_6BITS, IDE_DITHER_6BITS}, //!< DitherBits[3]
			FALSE                           //!< clk1/2
		},

		(tPANEL_CMD *)tCmdModeYUV640,                 //!< pCmdQueue
		sizeof(tCmdModeYUV640) / sizeof(tPANEL_CMD),  //!< nCmd
	},

#if (IDE_SIE_TEST_CCIR == IDE_SIE_CCIR601_16_bit)

	/***********       CCIR601 Progressive 640 MODE         *************/
	{
		// tPANEL_PARAM
		{
			/* Old prototype */
			PINMUX_LCDMODE_CCIR601,         //!< LCDMode
#if _FPGA_EMULATION_ == ENABLE
			//!< For IDE -> SIE usage
			//!< Also need to configure @ Pinmux table
			/*
			{PIN_FUNC_SENSOR,           PIN_SENSOR_CFG_NONE},
			{PIN_FUNC_SENSOR2,          PIN_SENSOR2_CFG_CCIR16BITS},
			{PIN_FUNC_SENSOR3,          PIN_SENSOR3_CFG_NONE},
			{PIN_FUNC_SENSOR4,          PIN_SENSOR4_CFG_NONE},
			{PIN_FUNC_MIPI_LVDS,        PIN_MIPI_LVDS_CFG_NONE},
			*/
			1.0f,                           //!< fDCLK
#else

			25.00f,                         //!< fDCLK
#endif
			880,//778,                      //!< uiHSyncTotalPeriod
			640,                            //!< uiHSyncActivePeriod
			120,                            //!< uiHSyncBackPorch
			540,                            //!< uiVSyncTotalPeriod
			480,                            //!< uiVSyncActivePeriod
			42,                             //!< uiVSyncBackPorchOdd
			42,                             //!< uiVSyncBackPorchEven
			640,                            //!< uiBufferWidth
			480,                            //!< uiBufferHeight
			640,                            //!< uiWindowWidth
			480,                            //!< uiWindowHeight
			TRUE,                           //!< bYCbCrFormat

			/* New added parameters */
			0x01,                           //!< uiHSyncSYNCwidth
			0x01                            //!< uiVSyncSYNCwidth
		},

		// tIDE_PARAM
		{
			/* Old prototype */
			PINMUX_LCD_SEL_CCIR601_16BITS,  //!< pinmux_select_lcd : Note: during IDE to SIE path, DE can not enable
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
#else
	/***********       CCIR601 MODE         *************/
	{
		// tPANEL_PARAM
		{
			/* Old prototype */
			PINMUX_LCDMODE_CCIR601,         //!< LCDMode
#if (_FPGA_EMULATION_ == ENABLE)
			(_FPGA_PLL_OSC_ / 1000000),     //!< fDCLK
#else
			24.55f,                         //!< fDCLK
#endif
			1560,                           //!< uiHSyncTotalPeriod
			1280,                           //!< uiHSyncActivePeriod
			252,                            //!< uiHSyncBackPorch
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
	}
};
#endif


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
#if 0
	UINT32                  uiSIFData;
	DISPDEV_IOCTRL_PARAM    DevIOCtrl;

	uiSIFData = ((((UINT32)addr << 9) | (UINT32)value) << 16);

	pDispDevControl(DISPDEV_IOCTRL_GET_REG_IF, &DevIOCtrl);
	sif_send(DevIOCtrl.SEL.GET_REG_IF.uiSifCh, uiSIFData, 0, 0);
#endif
}


void dispdev_writeToLcdGpioParal(DISPDEV_IOCTRL pDispDevControl, UINT32 addr, UINT32 value)
{
#if (IDE_SIE_TEST_CCIR == IDE_SIE_CCIR601_8_bit)

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
#endif
}



void dispdev_readFromLcdGpioParal(DISPDEV_IOCTRL pDispDevControl, UINT32 addr, UINT32 *p_value)
{
	DBG_ERR("Has not implement dispdev_readFromLcdGpioParal \r\n");
}
#endif
