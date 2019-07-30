/*
    Display object object panel configuration parameters for AUCN01

    @file       AUCN01.c
    @ingroup
    @note       Nothing

    Copyright   Novatek Microelectronics Corp. 2011.  All rights reserved.
*/

#include "kernel.h"
#include "Debug.h"
#include "display.h"
#include "dispdev_IF8bits.h"
#include "dispdev_panlcomn.h"

/*
    Panel Parameters for AUCN01
*/
//@{

/*Used in RGB-Serial & RGB Through*/
const tPANEL_CMD tCmdModeThrough[] = {
	{0x05, 0x01},
	{0x70, 0x06},
	{CMDDELAY_MS, 1},
	{0x05, 0x81},
	{CMDDELAY_MS, 60},
	{0x70, 0x07},
	{0x01, 85},
	{0x03, 0x10},
	{0x04, 0x10},
	{0x08, 0x20},
	{0x0A, 0x84},
	{0x0E, 0x80},
	{0x5A, 0x06},
	{0x6B, 0x00},
	{0x5F, 0x1F},
	{0x7F, 0x26},
	{0x65, 0x66},
	{0x19, 0x05},
	{0x1A, 0x24},//1B
	{0x1B, 0x2A},//25
	{0x1C, 0x0E},//0F
	{0x1D, 0x10},
	{0x1E, 0x17},//18
	{0x1F, 0xD5},//C5
	{0x20, 0xB0},//A0
	{0x21, 0x4F},
	{0x22, 0x6F},
	{0x23, 0x06},
	{0x24, 0x0E},
	{0x25, 0x0F},
	{0x26, 0x28},//27
	{0x27, 0x22},//71
	{0x28, 0x08},
	{0x29, 0x04},
	{0x2A, 0x1F},//1E
	{0x2B, 0x22},//20
	{0x2C, 0x0F},//0E
	{0x2D, 0x11},
	{0x2E, 0x06},//05
	{0x2F, 0x14},//15
	{0x30, 0x10},//11
	{0x31, 0x0D},//0E
	{0x32, 0x28},//23
	{0x33, 0x23},//1A
	{0x34, 0x06},
	{0x10, 0x38},//38
	{0x11, 0x40},
	{0x12, 0x48},//48
	{0x16, 0xF0},//F0
	{0x18, 0xC0},//C0
	{0x36, 0x0F},//*A
	{0x3E, 0x0F},
	{0x46, 0x0F},
#if 0
	{0x3C, 0xF0},//*G
	{0x44, 0xF0},
	{0x4C, 0xF0},
	{0x3B, 0xD2},//*F
	{0x3C, 0xE5},//*G
	{0x3D, 0xF2},//*H
	{0x43, 0xD2},
	{0x44, 0xE5},
	{0x45, 0xF2},
	{0x4B, 0xD2},
	{0x4C, 0xE5},
	{0x4D, 0xF2},
#endif

	{0x37, 0x30},//*B 40-8-8
	{0x3F, 0x30},
	{0x47, 0x30},
	{0x38, 0x70},//*C 80-8-8
	{0x40, 0x70},
	{0x48, 0x70},
	{0x39, 0x90},//*D A0-8-8
	{0x41, 0x90},
	{0x49, 0x90},
	{0x3A, 0xB8},//*E C0    -8
	{0x42, 0xB8},
	{0x4A, 0xB8},
	{0x3B, 0xCA},//*F D2-8
	{0x43, 0xCA},
	{0x4B, 0xCA},
	{0x3C, 0xD9},//*G E5-8    -4
	{0x44, 0xD9},
	{0x4C, 0xD9},
	{0x3D, 0xEA},//*H F2-8
	{0x45, 0xEA},
	{0x4D, 0xEA},

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
	{0x05, 0x01},
	{0x70, 0x06},
	{CMDDELAY_MS, 1},
	{0x05, 0x81},
	{CMDDELAY_MS, 60},
	{0x70, 0x07},
	{0x01, 85},
	{0x03, 0x10},
	{0x04, 0x10},
	{0x08, 0x20},
	{0x0A, 0x84},
	{0x0E, 0x80},
	{0x5A, 0x06},
	{0x6B, 0x00},
	{0x5F, 0x1F},
	{0x7F, 0x26},
	{0x65, 0x66},
	{0x19, 0x05},
	{0x1A, 0x24},
	{0x1B, 0x2A},
	{0x1C, 0x0E},
	{0x1D, 0x10},
	{0x1E, 0x17},
	{0x1F, 0xD5},
	{0x20, 0xB0},
	{0x21, 0x4F},
	{0x22, 0x6F},
	{0x23, 0x06},
	{0x24, 0x0E},
	{0x25, 0x0F},
	{0x26, 0x28},
	{0x27, 0x22},
	{0x28, 0x08},
	{0x29, 0x04},
	{0x2A, 0x1F},
	{0x2B, 0x22},
	{0x2C, 0x0F},
	{0x2D, 0x11},
	{0x2E, 0x06},
	{0x2F, 0x14},
	{0x30, 0x10},
	{0x31, 0x0D},
	{0x32, 0x28},
	{0x33, 0x23},
	{0x34, 0x06},
	{0x10, 0x38},
	{0x11, 0x40},
	{0x12, 0x48},
	{0x16, 0xF0},
	{0x18, 0xC0},
	{0x36, 0x0F},
	{0x3E, 0x0F},
	{0x46, 0x0F},
	{0x3C, 0xF0},
	{0x44, 0xF0},
	{0x4C, 0xF0},
	{0x3B, 0xD2},
	{0x3C, 0xE5},
	{0x3D, 0xF2},
	{0x43, 0xD2},
	{0x44, 0xE5},
	{0x45, 0xF2},
	{0x4B, 0xD2},
	{0x4C, 0xE5},
	{0x4D, 0xF2},
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
			24.55f,                          //!< fDCLK
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
			27.0f,                          //!< fDCLK
			1560,                           //!< uiHSyncTotalPeriod
			1280,                           //!< uiHSyncActivePeriod
			253,                            //!< uiHSyncBackPorch
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
			{FALSE, FALSE, FALSE},             //!< subpixOdd[3]
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
			33.075f,                           //!< fDCLK
			1050,                            //!< uiHSyncTotalPeriod
			960,                            //!< uiHSyncActivePeriod
			40,                           //!< uiHSyncBackPorch
			525,                           //!< uiVSyncTotalPeriod
			480,                            //!< uiVSyncActivePeriod
			42,                           //!< uiVSyncBackPorchOdd
			42,                           //!< uiVSyncBackPorchEven
			960,                            //!< uiBufferWidth
			480,                            //!< uiBufferHeight
			960,                            //!< uiWindowWidth
			480,                            //!< uiWindowHeight
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
			{TRUE, TRUE, TRUE},          //!< subpixOdd[3]
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
			9.7f,                           //!< fDCLK
			616,                            //!< uiHSyncTotalPeriod
			480,                            //!< uiHSyncActivePeriod
			100,                            //!< uiHSyncBackPorch
			262,                            //!< uiVSyncTotalPeriod
			240,                            //!< uiVSyncActivePeriod
			18,                             //!< uiVSyncBackPorchOdd
			18,                             //!< uiVSyncBackPorchEven
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

		(tPANEL_CMD *)tCmdModeThrough,                 //!< pCmdQueue
		sizeof(tCmdModeThrough) / sizeof(tPANEL_CMD),  //!< nCmd
	},

	/***********       CCIR656 MODE         *************/
	{
		// tPANEL_PARAM
		{
			/* Old prototype */
			PINMUX_LCDMODE_CCIR656,         //!< LCDMode
			27.0f,                         //!< fDCLK
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
			TRUE,                          //!< bYCbCrFormat

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
#if (_FPGA_EMULATION_ != ENABLE)
			44.1f,                         //!< fDCLK
#else //for FPGA
			18.0f,                          //!< fDCLK //reduce dma bandwidth
#endif
			700,                           //!< uiHSyncTotalPeriod
			640,                           //!< uiHSyncActivePeriod
			20,                            //!< uiHSyncBackPorch
			525,                            //!< uiVSyncTotalPeriod
			480,                            //!< uiVSyncActivePeriod
			42,                             //!< uiVSyncBackPorchOdd
			42,                             //!< uiVSyncBackPorchEven
			1280,                            //!< uiBufferWidth
			480,                            //!< uiBufferHeight
			1280,                            //!< uiWindowWidth
			480,                            //!< uiWindowHeight
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
