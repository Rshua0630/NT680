/*
    Display object object panel configuration parameters for ILI8961

    @file       ILI8961.c
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
    Panel Parameters for ILI8961
*/
//@{

/*Used in RGB-Serial & RGB Through*/
const tPANEL_CMD tCmdModeThrough[] = {
	{0x05, 0x5F},
	{CMDDELAY_MS, 5}, //delay 5 ms
	{0x05, 0x1F},//reset
	{CMDDELAY_MS, 10}, //delay 10 ms
	{0x05, 0x5F},
	{CMDDELAY_MS, 50}, //delay 50 ms
	{0x2B, 0x01},//exit standby mode
	{0x00, 0x09 | 0x00}, //VCOMAC
	{0x01, 0x9F},//VCOMDC
	{0x04, 0x08 | 0x00}, //8-bit RGB mode + Rotate 0
	{0x16, 0x04} //Default Gamma setting  2.2
};

/*Used in RGB-Dummy 320*/
const tPANEL_CMD tCmdModeRgbd320[] = {
	{0x05, 0x5F},
	{CMDDELAY_MS, 5}, //delay 5 ms
	{0x05, 0x1F},//reset
	{CMDDELAY_MS, 10}, //delay 10 ms
	{0x05, 0x5F},
	{CMDDELAY_MS, 50}, //delay 50 ms
	{0x2B, 0x01},//exit standby mode
	{0x00, 0x09 | 0x00}, //VCOMAC
	{0x01, 0x9F},//VCOMDC
	{0x04, 0x08 | 0x10}, //8-bit RGB-Dummy 320 mode + Rotate 0
	{0x16, 0x04} //Default Gamma setting  2.2
};

/*Used in RGB-Dummy 360*/
const tPANEL_CMD tCmdModeRgbd360[] = {
	{0x05, 0x5F},
	{CMDDELAY_MS, 5}, //delay 5 ms
	{0x05, 0x1F},//reset
	{CMDDELAY_MS, 10}, //delay 10 ms
	{0x05, 0x5F},
	{CMDDELAY_MS, 50}, //delay 50 ms
	{0x2B, 0x01},//exit standby mode
	{0x00, 0x09 | 0x00}, //VCOMAC
	{0x01, 0x9F},//VCOMDC
	{0x04, 0x08 | 0x20}, //8-bit RGB-Dummy 360 mode + Rotate 0
	{0x16, 0x04} //Default Gamma setting  2.2
};

/*Used in CCIR601 */
const tPANEL_CMD tCmdModeCCIR601[] = {
	{0x05, 0x5F},
	{CMDDELAY_MS, 5}, //delay 5 ms
	{0x05, 0x1F},//reset
	{CMDDELAY_MS, 10}, //delay 10 ms
	{0x05, 0x5F},
	{CMDDELAY_MS, 50}, //delay 50 ms
	{0x2B, 0x01},//exit standby mode
	{0x00, 0x09 | 0x40}, //VCOMAC + CCIR601 mode
	{0x01, 0x9F},//VCOMDC
	{0x04, 0x08 | 0x00}, //CCIR601 mode + Rotate 0
	{0x16, 0x04} //Default Gamma setting  2.2
};

/*Used in CCIR656 (Under Test) */
const tPANEL_CMD tCmdModeCCIR656[] = {
	{0x05, 0x5F},
	{CMDDELAY_MS, 5}, //delay 5 ms
	{0x05, 0x1F},//reset
	{CMDDELAY_MS, 10}, //delay 10 ms
	{0x05, 0x5F},
	{CMDDELAY_MS, 50}, //delay 50 ms
	{0x2B, 0x01},//exit standby mode
	{0x00, 0x09 | 0x00}, //VCOMAC + CCIR656 mode
	{0x01, 0x9F},//VCOMDC
	{0x04, 0x08 | 0x40}, //CCIR656 mode + Rotate 0
	//{0x0C, 0x06|0x40},//enable DE
	{0x16, 0x04} //Default Gamma setting  2.2
};


/*Used in YUV640 */
const tPANEL_CMD tCmdModeYUV640[] = {
	{0x05, 0x5F},
	{CMDDELAY_MS, 5}, //delay 5 ms
	{0x05, 0x1F},//reset
	{CMDDELAY_MS, 10}, //delay 10 ms
	{0x05, 0x5F},
	{CMDDELAY_MS, 50}, //delay 50 ms
	{0x2B, 0x01},//exit standby mode
	{0x00, 0x09 | 0x40}, //VCOMAC + YUV 640 mode
	{0x01, 0x9F},//VCOMDC
	{0x04, 0x08 | 0x40}, //YUV 640 mode + Rotate 0
	{0x16, 0x04} //Default Gamma setting  2.2
};

/*Used in YUV720 */
const tPANEL_CMD tCmdModeYUV720[] = {
	{0x05, 0x5F},
	{CMDDELAY_MS, 5}, //delay 5 ms
	{0x05, 0x1F},//reset
	{CMDDELAY_MS, 10}, //delay 10 ms
	{0x05, 0x5F},
	{CMDDELAY_MS, 50}, //delay 50 ms
	{0x2B, 0x01},//exit standby mode
	{0x00, 0x09 | 0x40}, //VCOMAC + YUV 720 mode
	{0x01, 0x9F},//VCOMDC
	{0x04, 0x08 | 0x60}, //YUV 720 mode + Rotate 0
	{0x16, 0x04} //Default Gamma setting  2.2
};

const tPANEL_CMD tCmdStandby[] = {
	{0x2B, 0x00} //enter standby mode
};

const tLCD_PARAM tMode[] = {
	/***********       RGB-SERIAL MODE         *************/
	{
		// tPANEL_PARAM
		{
			/* Old prototype */
			PINMUX_LCDMODE_RGB_SERIAL,      //!< LCDMode
			27.0f,                          //!< fDCLK
			1716,                           //!< uiHSyncTotalPeriod
			960,                            //!< uiHSyncActivePeriod
			70,                             //!< uiHSyncBackPorch
			262,                            //!< uiVSyncTotalPeriod
			240,                            //!< uiVSyncActivePeriod
			21,                             //!< uiVSyncBackPorchOdd
			21,                             //!< uiVSyncBackPorchEven
			960,                            //!< uiBufferWidth
			240,                            //!< uiBufferHeight
			960,                            //!< uiWindowWidth
			240,                            //!< uiWindowHeight
			FALSE,                          //!< bYCbCrFormat

			/* New added parameters */
			0x00,                           //!< uiHSyncSYNCwidth
			0x00                            //!< uiVSyncSYNCwidth
		},

		// tIDE_PARAM
		{
			/* Old prototype */
			PINMUX_LCD_SEL_SERIAL_RGB_8BITS,//!< pinmux_select_lcd;
			ICST_CCIR601,                   //!< icst;
			{FALSE, FALSE},                 //!< dithering[2];
			DISPLAY_DEVICE_AU,              //!< **DONT-CARE**
			IDE_PDIR_RBG,                   //!< pdir;
			IDE_LCD_R,                      //!< odd;
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
			9.7f,                           //!< fDCLK
			1716,                           //!< uiHSyncTotalPeriod
			960,                            //!< uiHSyncActivePeriod
			70,                             //!< uiHSyncBackPorch
			262,                            //!< uiVSyncTotalPeriod
			240,                            //!< uiVSyncActivePeriod
			21,                             //!< uiVSyncBackPorchOdd
			21,                             //!< uiVSyncBackPorchEven
			320,                            //!< uiBufferWidth
			240,                            //!< uiBufferHeight
			320,                            //!< uiWindowWidth
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
			IDE_PDIR_RBG,                   //!< pdir;
			IDE_LCD_R,                      //!< odd;
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

		(tPANEL_CMD *)tCmdModeThrough,                 //!< pCmdQueue
		sizeof(tCmdModeThrough) / sizeof(tPANEL_CMD),  //!< nCmd
	},
	/***********       RGB Dummy-320 MODE         *************/
	{
		// tPANEL_PARAM
		{
			/* Old prototype */
			PINMUX_LCDMODE_RGBD320,         //!< LCDMode
			24.535f,                        //!< fDCLK
			1560,                           //!< uiHSyncTotalPeriod
			1280,                           //!< uiHSyncActivePeriod
			241,                            //!< uiHSyncBackPorch
			262,                            //!< uiVSyncTotalPeriod
			240,                            //!< uiVSyncActivePeriod
			21,                             //!< uiVSyncBackPorchOdd
			21,                             //!< uiVSyncBackPorchEven
			320,                            //!< uiBufferWidth
			240,                            //!< uiBufferHeight
			320,                            //!< uiWindowWidth
			240,                            //!< uiWindowHeight
			FALSE,                          //!< bYCbCrFormat

			/* New added parameters */
			0x00,                           //!< uiHSyncSYNCwidth
			0x00                            //!< uiVSyncSYNCwidth
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
	/***********       RGB Dummy-360 MODE         *************/
	{
		// tPANEL_PARAM
		{
			/* Old prototype */
			PINMUX_LCDMODE_RGBD360,         //!< LCDMode
			27.0f,                          //!< fDCLK
			1716,                           //!< uiHSyncTotalPeriod
			1440,                           //!< uiHSyncActivePeriod
			241,                            //!< uiHSyncBackPorch
			262,                            //!< uiVSyncTotalPeriod
			240,                            //!< uiVSyncActivePeriod
			21,                             //!< uiVSyncBackPorchOdd
			21,                             //!< uiVSyncBackPorchEven
			360,                            //!< uiBufferWidth
			240,                            //!< uiBufferHeight
			360,                            //!< uiWindowWidth
			240,                            //!< uiWindowHeight
			FALSE,                          //!< bYCbCrFormat

			/* New added parameters */
			0x00,                           //!< uiHSyncSYNCwidth
			0x00                            //!< uiVSyncSYNCwidth
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

		(tPANEL_CMD *)tCmdModeRgbd360,                 //!< pCmdQueue
		sizeof(tCmdModeRgbd360) / sizeof(tPANEL_CMD),  //!< nCmd
	},
	/***********       CCIR601 MODE         *************/
	{
		// tPANEL_PARAM
		{
			/* Old prototype */
			PINMUX_LCDMODE_CCIR601,         //!< LCDMode
			24.535f,                        //!< fDCLK
			780,                           //!< uiHSyncTotalPeriod
			640,                           //!< uiHSyncActivePeriod
			120,                            //!< uiHSyncBackPorch
			262,                            //!< uiVSyncTotalPeriod
			240,                            //!< uiVSyncActivePeriod
			21,                             //!< uiVSyncBackPorchOdd
			21,                             //!< uiVSyncBackPorchEven
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

		(tPANEL_CMD *)tCmdModeCCIR601,                 //!< pCmdQueue
		sizeof(tCmdModeCCIR601) / sizeof(tPANEL_CMD),  //!< nCmd
	},
	/***********       CCIR656 MODE (Under Test)*************/
	{
		// tPANEL_PARAM
		{
			/* Old prototype */
			PINMUX_LCDMODE_CCIR656,         //!< LCDMode
			6.2f,                           //!< fDCLK
			390,                            //!< uiHSyncTotalPeriod
			320,                            //!< uiHSyncActivePeriod
			61,                             //!< uiHSyncBackPorch
			262,                            //!< uiVSyncTotalPeriod
			240,                            //!< uiVSyncActivePeriod
			21,                             //!< uiVSyncBackPorchOdd
			21,                             //!< uiVSyncBackPorchEven
			320,                            //!< uiBufferWidth
			240,                            //!< uiBufferHeight
			320,                            //!< uiWindowWidth
			240,                            //!< uiWindowHeight
			TRUE,                           //!< bYCbCrFormat

			/* New added parameters */
			0x00,                           //!< uiHSyncSYNCwidth
			0x00                            //!< uiVSyncSYNCwidth
		},

		// tIDE_PARAM
		{
			/* Old prototype */
			//PINMUX_LCD_SEL_CCIR656|PINMUX_LCD_SEL_DE_ENABLE,//!< pinmux_select_lcd;
			PINMUX_LCD_SEL_CCIR656,         //!< pinmux_select_lcd;
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
			FALSE,                          //!< clkinv;
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
			24.535f,                        //!< fDCLK
			780,                           //!< uiHSyncTotalPeriod
			640,                           //!< uiHSyncActivePeriod
			120,                            //!< uiHSyncBackPorch
			262,                            //!< uiVSyncTotalPeriod
			240,                            //!< uiVSyncActivePeriod
			21,                             //!< uiVSyncBackPorchOdd
			21,                             //!< uiVSyncBackPorchEven
			640,                            //!< uiBufferWidth
			240,                            //!< uiBufferHeight
			640,                            //!< uiWindowWidth
			240,                            //!< uiWindowHeight
			TRUE,                           //!< bYCbCrFormat

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

		(tPANEL_CMD *)tCmdModeYUV640,                 //!< pCmdQueue
		sizeof(tCmdModeYUV640) / sizeof(tPANEL_CMD),  //!< nCmd
	},
	/***********       YUV720 MODE         *************/
	{
		// tPANEL_PARAM
		{
			/* Old prototype */
			PINMUX_LCDMODE_YUV720,          //!< LCDMode
			27.0f,                          //!< fDCLK
			858,                           //!< uiHSyncTotalPeriod
			720,                           //!< uiHSyncActivePeriod
			120,                            //!< uiHSyncBackPorch
			262,                            //!< uiVSyncTotalPeriod
			240,                            //!< uiVSyncActivePeriod
			21,                             //!< uiVSyncBackPorchOdd
			21,                             //!< uiVSyncBackPorchEven
			720,                            //!< uiBufferWidth
			240,                            //!< uiBufferHeight
			720,                            //!< uiWindowWidth
			240,                            //!< uiWindowHeight
			TRUE,                           //!< bYCbCrFormat

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

		(tPANEL_CMD *)tCmdModeYUV720,                 //!< pCmdQueue
		sizeof(tCmdModeYUV720) / sizeof(tPANEL_CMD),  //!< nCmd
	},
};

const tPANEL_CMD tCmdRotate_None[] = {
	{0x04, 0x08}, //8-bit RGB mode + Rotate 0

};
const tPANEL_CMD tCmdRotate_180[] = {
	{0x04, 0x1B}, //RGB320 mode + Rotate 180,

};
const tLCD_ROT tRotate[] = {
	{DISPDEV_LCD_ROTATE_NONE, (tPANEL_CMD *)tCmdRotate_None, 1},
	{DISPDEV_LCD_ROTATE_180, (tPANEL_CMD *)tCmdRotate_180, 1}
};

/*
50ns + 50ns = 10MHz
SIF Source Clock 是 96MHz, 所以可以設 9.6MHz
*/
const tLCD_SIF_PARAM tSIFParam[] = {
	// SIF mode 00
	SIF_MODE_00,
	// Bus clock maximun = 20 MHz(configure as 18MHZ)
	18000000,
	// SENS = 50x2
	30,
	// SENH = 50
	50,
	// 16 bits per transmission
	16,
	// MSb shift out first
	SIF_DIR_MSB,
};
//@}

tLCD_ROT *dispdev_getLcdRotateCmd(UINT32 *ModeNumber)
{
	if (tRotate != NULL) {
		*ModeNumber = sizeof(tRotate) / sizeof(tLCD_ROT);
	} else {
		*ModeNumber = 0;
	}
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

///////////////////////////////////////////////////////////////////////////////////////////////

#define OPTION_SIF_0000  0x0   ///< SIF bus mode 0000
#define OPTION_SIF_0001  0x1   ///< SIF bus mode 0001
#define OPTION_SIF_0010  0x2   ///< SIF bus mode 0010
#define OPTION_SIF_0011  0x3   ///< SIF bus mode 0011
#define OPTION_SIF_0100  0x4   ///< SIF bus mode 0100
#define OPTION_SIF_0101  0x5   ///< SIF bus mode 0101
#define OPTION_SIF_0110  0x6   ///< SIF bus mode 0110
#define OPTION_SIF_0111  0x7   ///< SIF bus mode 0111
#define OPTION_SIF_1000  0x8   ///< SIF bus mode 1000
#define OPTION_SIF_1001  0x9   ///< SIF bus mode 1001
#define OPTION_SIF_1010  0xA   ///< SIF bus mode 1010
#define OPTION_SIF_1011  0xB   ///< SIF bus mode 1011
#define OPTION_SIF_1100  0xC   ///< SIF bus mode 1100
#define OPTION_SIF_1101  0xD   ///< SIF bus mode 1101
#define OPTION_SIF_1110  0xE   ///< SIF bus mode 1110
#define OPTION_SIF_1111  0xF   ///< SIF bus mode 1111

#define SETTING_SIF_MODE                    OPTION_SIF_0000 //SIF MODE: SIF_MODE_0000 ~ SIF_MODE_1111
#define SETTING_PANEL_REG_BIT_SHIFT_ADDRESS 8               //D[?] is Address LSB
#define SETTING_PANEL_REG_BIT_SHIFT_VALUE   0               //D[?] is Content LSB
#define SETTING_PANEL_REG_BIT_WRITE         14              //D[?] is Bit of Write Command
#define SETTING_PANEL_REG_MODE_WRITE        0               //BIT High(1) or LOW(0) is Write Mode
#define SETTING_PANEL_REG_MODE_READ         1               //BIT High(1) or LOW(0) is Read  Mode

#if (SETTING_SIF_MODE==OPTION_SIF_0000)     //Confirmed
#define USED_SIF_MODE     SIF_MODE_00
#define SIF_CLK_INIT()    gpio_setPin(SifClk)
#define SIF_CLK_BEGIN()   gpio_clearPin(SifClk)
#define SIF_CLK_END()     gpio_setPin(SifClk)
#define SIF_SEN_INIT()    gpio_setPin(SifSen)
#define SIF_SEN_BEGIN()   gpio_clearPin(SifSen)
#define SIF_SEN_END()     gpio_setPin(SifSen)
#elif (SETTING_SIF_MODE==OPTION_SIF_0001)   //Not Confirmed
#define USED_SIF_MODE     SIF_MODE_01
#define SIF_CLK_INIT()    gpio_clearPin(SifClk)
#define SIF_CLK_BEGIN()   gpio_setPin(SifClk)
#define SIF_CLK_END()     gpio_clearPin(SifClk)
#define SIF_SEN_INIT()    gpio_setPin(SifSen)
#define SIF_SEN_BEGIN()   gpio_clearPin(SifSen)
#define SIF_SEN_END()     gpio_setPin(SifSen)
#elif (SETTING_SIF_MODE==OPTION_SIF_0010)   //Not Confirmed
#define USED_SIF_MODE     SIF_MODE_02
#define SIF_CLK_INIT()    gpio_setPin(SifClk)
#define SIF_CLK_BEGIN()   gpio_clearPin(SifClk)
#define SIF_CLK_END()     gpio_setPin(SifClk)
#define SIF_SEN_INIT()    gpio_clearPin(SifSen)
#define SIF_SEN_BEGIN()   gpio_setPin(SifSen)
#define SIF_SEN_END()     gpio_clearPin(SifSen)
#elif (SETTING_SIF_MODE==OPTION_SIF_0011)   //Not Confirmed
#define USED_SIF_MODE     SIF_MODE_03
#define SIF_CLK_INIT()    gpio_clearPin(SifClk)
#define SIF_CLK_BEGIN()   gpio_setPin(SifClk)
#define SIF_CLK_END()     gpio_clearPin(SifClk)
#define SIF_SEN_INIT()    gpio_clearPin(SifSen)
#define SIF_SEN_BEGIN()   gpio_setPin(SifSen)
#define SIF_SEN_END()     gpio_clearPin(SifSen)
#elif (SETTING_SIF_MODE==OPTION_SIF_0100)   //Not Confirmed
#define USED_SIF_MODE     SIF_MODE_04
#define SIF_CLK_INIT()    gpio_clearPin(SifClk)
#define SIF_CLK_BEGIN()   gpio_setPin(SifClk)
#define SIF_CLK_END()     gpio_clearPin(SifClk)
#define SIF_SEN_INIT()    gpio_setPin(SifSen)
#define SIF_SEN_BEGIN()   gpio_clearPin(SifSen)
#define SIF_SEN_END()     gpio_setPin(SifSen)
#elif (SETTING_SIF_MODE==OPTION_SIF_0101)   //Not Confirmed
#define USED_SIF_MODE     SIF_MODE_05
#define SIF_CLK_INIT()    gpio_setPin(SifClk)
#define SIF_CLK_BEGIN()   gpio_clearPin(SifClk)
#define SIF_CLK_END()     gpio_setPin(SifClk)
#define SIF_SEN_INIT()    gpio_setPin(SifSen)
#define SIF_SEN_BEGIN()   gpio_clearPin(SifSen)
#define SIF_SEN_END()     gpio_setPin(SifSen)
#elif (SETTING_SIF_MODE==OPTION_SIF_0110)   //Not Confirmed
#define USED_SIF_MODE     SIF_MODE_06
#define SIF_CLK_INIT()    gpio_clearPin(SifClk)
#define SIF_CLK_BEGIN()   gpio_setPin(SifClk)
#define SIF_CLK_END()     gpio_clearPin(SifClk)
#define SIF_SEN_INIT()    gpio_clearPin(SifSen)
#define SIF_SEN_BEGIN()   gpio_setPin(SifSen)
#define SIF_SEN_END()     gpio_clearPin(SifSen)
#elif (SETTING_SIF_MODE==OPTION_SIF_0111)   //Not Confirmed
#define USED_SIF_MODE     SIF_MODE_07
#define SIF_CLK_INIT()    gpio_setPin(SifClk)
#define SIF_CLK_BEGIN()   gpio_clearPin(SifClk)
#define SIF_CLK_END()     gpio_setPin(SifClk)
#define SIF_SEN_INIT()    gpio_clearPin(SifSen)
#define SIF_SEN_BEGIN()   gpio_setPin(SifSen)
#define SIF_SEN_END()     gpio_clearPin(SifSen)
#elif (SETTING_SIF_MODE==OPTION_SIF_1000)   //Confirmed
#define USED_SIF_MODE     SIF_MODE_08
#define SIF_CLK_INIT()    gpio_clearPin(SifClk)
#define SIF_CLK_BEGIN()   gpio_setPin(SifClk)
#define SIF_CLK_END()     gpio_clearPin(SifClk)
#define SIF_SEN_INIT()    gpio_setPin(SifSen)
#define SIF_SEN_BEGIN()   gpio_clearPin(SifSen)
#define SIF_SEN_END()     gpio_setPin(SifSen)
#else
#error("This SIF Mode has not implement!");
#endif


void dispdev_writeToLcdSif(DISPDEV_IOCTRL pDispDevControl, UINT32 addr, UINT32 value)
{
	UINT32                  uiSIFData;
	DISPDEV_IOCTRL_PARAM    DevIOCtrl;

	//Pack Data
	uiSIFData = ((UINT32)addr << SETTING_PANEL_REG_BIT_SHIFT_ADDRESS)
				| ((UINT32)value << SETTING_PANEL_REG_BIT_SHIFT_VALUE);

	//Arrange Address
	uiSIFData = ((uiSIFData & (1 << SETTING_PANEL_REG_BIT_WRITE)) << 1)
				| (uiSIFData & (~(1 << SETTING_PANEL_REG_BIT_WRITE)));

	uiSIFData = (uiSIFData << 16);
	//RW Mode
	//or 0 => skip
	//uiSIFData |= (SETTING_PANEL_REG_MODE_WRITE<<SETTING_PANEL_REG_BIT_WRITE);


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

	SIF_SEN_INIT();
	SIF_CLK_INIT();
	gpio_setPin(SifData);

	//Pack Data
	uiSIFData = ((UINT32)addr << SETTING_PANEL_REG_BIT_SHIFT_ADDRESS)
				| ((UINT32)value << SETTING_PANEL_REG_BIT_SHIFT_VALUE);

	//Arrange Address
	uiSIFData = ((uiSIFData & (1 << SETTING_PANEL_REG_BIT_WRITE)) << 1)
				| (uiSIFData & (~(1 << SETTING_PANEL_REG_BIT_WRITE)));

	//RW Mode
	uiSIFData |= (SETTING_PANEL_REG_MODE_WRITE << SETTING_PANEL_REG_BIT_WRITE);

	Delay_DelayUs(500);

	SIF_SEN_BEGIN();

	for (j = 16; j > 0; j--) {
		if (((uiSIFData >> (j - 1)) & 0x01)) {
			gpio_setPin(SifData);
		} else {
			gpio_clearPin(SifData);
		}

		Delay_DelayUs(100);
		SIF_CLK_BEGIN();
		Delay_DelayUs(200);
		SIF_CLK_END();
		Delay_DelayUs(100);
	}

	Delay_DelayUs(500);
	SIF_SEN_END();
}



void dispdev_readFromLcdGpio(DISPDEV_IOCTRL pDispDevControl, UINT32 addr, UINT32 *p_value)
{
	UINT32                  SifClk, SifSen, SifData;
	DISPDEV_IOCTRL_PARAM    DevIOCtrl;
	UINT32                  uiSIFData2, j2;

	if (!p_value) {
		return;
	}

	pDispDevControl(DISPDEV_IOCTRL_GET_REG_IF, &DevIOCtrl);
	SifClk  = DevIOCtrl.SEL.GET_REG_IF.uiGpioClk;
	SifSen  = DevIOCtrl.SEL.GET_REG_IF.uiGpioSen;
	SifData = DevIOCtrl.SEL.GET_REG_IF.uiGpioData;

	gpio_setDir(SifSen, GPIO_DIR_OUTPUT);
	gpio_setDir(SifClk, GPIO_DIR_OUTPUT);
	gpio_setDir(SifData, GPIO_DIR_OUTPUT);

	SIF_SEN_INIT();
	SIF_CLK_INIT();
	gpio_setPin(SifData);

	//Pack Data
	uiSIFData2 = ((UINT32)addr << SETTING_PANEL_REG_BIT_SHIFT_ADDRESS);

	//Arrange Address
	uiSIFData2 = ((uiSIFData2 & (1 << SETTING_PANEL_REG_BIT_WRITE)) << 1)
				 | (uiSIFData2 & (~(1 << SETTING_PANEL_REG_BIT_WRITE)));

	//RW Mode
	uiSIFData2 |= (SETTING_PANEL_REG_MODE_READ << SETTING_PANEL_REG_BIT_WRITE);


	Delay_DelayUs(500);

	SIF_SEN_BEGIN();

	for (j2 = 16; j2 > 8; j2--) {
		if (((uiSIFData2 >> (j2 - 1)) & 0x01)) {
			gpio_setPin(SifData);
		} else {
			gpio_clearPin(SifData);
		}

		Delay_DelayUs(100);
		SIF_CLK_BEGIN();
		Delay_DelayUs(200);
		SIF_CLK_END();
		Delay_DelayUs(100);
	}
	(*p_value) = 0;
	gpio_setDir(SifData, GPIO_DIR_INPUT);
	for (; j2 > 0; j2--) {
		int v;
		SIF_CLK_BEGIN();
		Delay_DelayUs(100);
		SIF_CLK_END();
		v = (gpio_getPin(SifData) << (j2 - 1));
		(*p_value) |= v;

		Delay_DelayUs(200);
	}

	Delay_DelayUs(500);
	SIF_SEN_END();
}

tLCD_SIF_PARAM *dispdev_getLcdSifParam(void)
{
	return (tLCD_SIF_PARAM *)tSIFParam;
}
