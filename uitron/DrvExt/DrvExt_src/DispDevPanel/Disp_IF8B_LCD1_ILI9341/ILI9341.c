/*
    Display object object panel configuration parameters for ILI9341

    @file       ILI9341.c
    @ingroup
    @note       Nothing

    Copyright   Novatek Microelectronics Corp. 2011.  All rights reserved.
*/

#include "display.h"
#include "dispdev_IF8bits.h"
#include "dispdev_panlcomn.h"
#include "Debug.h"

#define IFCMD_CMD   0x01000000
#define IFCMD_DATA  0x02000000

/*
    Panel Parameters for ILI9341
*/
//@{
const tPANEL_CMD tCmdStandby[] = {
	//{0x06, 0x60}
	{CMDDELAY_US,   0x10}
};

const tPANEL_CMD tCmdModeRgbd320[] = {
	{CMDDELAY_MS, 10},
	{IFCMD_CMD,   0x01},
	{CMDDELAY_MS, 120},

	{IFCMD_CMD,   0xcf},
	{IFCMD_DATA,  0x00},
	{IFCMD_DATA,  0xaa},
	{IFCMD_DATA,  0x30},

	{IFCMD_CMD,   0xed},
	{IFCMD_DATA,  0x64},
	{IFCMD_DATA,  0x03},
	{IFCMD_DATA,  0x12},
	{IFCMD_DATA,  0x81},

	{IFCMD_CMD,   0xe8},
	{IFCMD_DATA,  0x85},
	{IFCMD_DATA,  0x00},
	{IFCMD_DATA,  0x78},

	{IFCMD_CMD,   0xcb},
	{IFCMD_DATA,  0x39},
	{IFCMD_DATA,  0x2c},
	{IFCMD_DATA,  0x00},
	{IFCMD_DATA,  0x34},
	{IFCMD_DATA,  0x02},

	{IFCMD_CMD,   0xf7},
	{IFCMD_DATA,  0x20},

	{IFCMD_CMD,   0xea},
	{IFCMD_DATA,  0x00},
	{IFCMD_DATA,  0x00},

	{IFCMD_CMD,   0xc0}, //Power control
	{IFCMD_DATA,  0x1d},

	{IFCMD_CMD,   0xc1}, //Power control
	{IFCMD_DATA,  0x13},

	{IFCMD_CMD,   0xc5}, //VCM control
	{IFCMD_DATA,  0x30},
	{IFCMD_DATA,  0x3f},

	{IFCMD_CMD,   0xc7}, //VCM control
	{IFCMD_DATA,  0xb5},

	{IFCMD_CMD,   0xb1}, //Power control
	{IFCMD_DATA,  0x00},
	{IFCMD_DATA,  0x17},

	{IFCMD_CMD,   0xb6}, //Display Function Control
	{IFCMD_DATA,  0x32},
	{IFCMD_DATA,  0x02},

	{IFCMD_CMD,   0xf2}, //3Gamma Function Disable
	{IFCMD_DATA,  0x00},

	{IFCMD_CMD,   0x26}, //Gamma curve selected
	{IFCMD_DATA,  0x01},

	//gamma start
	{IFCMD_CMD,   0xe0},
	{IFCMD_DATA,  0x0f}, //P01-VP63
	{IFCMD_DATA,  0x12}, //P02-VP62
	{IFCMD_DATA,  0x19}, //P03-VP61
	{IFCMD_DATA,  0x0b}, //P05-VP57
	{IFCMD_DATA,  0x0a}, //P06-VP50
	{IFCMD_DATA,  0x18}, //P07-VP43
	{IFCMD_DATA,  0x46}, //P08-VP27,36
	{IFCMD_DATA,  0xb8}, //P09-VP20
	{IFCMD_DATA,  0x34}, //P10-VP13
	{IFCMD_DATA,  0x07}, //P11-VP6
	{IFCMD_DATA,  0x0a}, //P12-VP4
	{IFCMD_DATA,  0x00}, //P13-VP2
	{IFCMD_DATA,  0x00}, //P14-VP1
	{IFCMD_DATA,  0x07}, //P15-VP0
	{IFCMD_DATA,  0x00}, //P16-VP0

	{IFCMD_CMD,   0xe1},
	{IFCMD_DATA,  0x00}, //P01
	{IFCMD_DATA,  0x23}, //P02
	{IFCMD_DATA,  0x26}, //P03
	{IFCMD_DATA,  0x04}, //P04
	{IFCMD_DATA,  0x10}, //P05
	{IFCMD_DATA,  0x07}, //P06
	{IFCMD_DATA,  0x39}, //P07
	{IFCMD_DATA,  0x47}, //P08
	{IFCMD_DATA,  0x4b}, //P09
	{IFCMD_DATA,  0x08}, //P10
	{IFCMD_DATA,  0x15}, //P11
	{IFCMD_DATA,  0x0f}, //P12
	{IFCMD_DATA,  0x3f}, //P13
	{IFCMD_DATA,  0x38}, //P14
	{IFCMD_DATA,  0x0F}, //P15
	//gamma end

	{IFCMD_CMD,   0xb0},
	{IFCMD_DATA,  0xe0},

	{IFCMD_CMD,   0x3a}, //pixel format set
	{IFCMD_DATA,  0x66},

	{IFCMD_CMD,   0x36}, //Memory Access Control
	{IFCMD_DATA,  0x48},

	{IFCMD_CMD,   0xf6}, //interface control
	{IFCMD_DATA,  0x01},
	{IFCMD_DATA,  0x00},
	{IFCMD_DATA,  0x07},

	{IFCMD_CMD,   0x21}, //inversion on
	{CMDDELAY_MS, 10},

	{IFCMD_CMD,   0x11}, //Exit Sleep
	{CMDDELAY_MS, 120},

	{IFCMD_CMD,   0x29}, //Display ON
};

const tLCD_PARAM tMode[] = {
	/***********       RGB Serial MODE         *************/
	{
		// tPANEL_PARAM
		{
			PINMUX_LCDMODE_RGB_SERIAL,      //!< LCDMode
			27.00f,                         //!< fDCLK
			(0x60 + 720),                   //!< uiHSyncTotalPeriod
			720,                            //!< uiHSyncActivePeriod
			0x10,                           //!< uiHSyncBackPorch
			0x8 + 320,                      //!< uiVSyncTotalPeriod
			320,                            //!< uiVSyncActivePeriod
			0x02,                           //!< uiVSyncBackPorchOdd
			0x02,                           //!< uiVSyncBackPorchEven
			320,                            //!< uiBufferWidth
			720,                            //!< uiBufferHeight
			320,                            //!< uiWindowWidth
			720,                            //!< uiWindowHeight
			FALSE,                          //!< bYCbCrFormat

			/* New added parameters */
			0x01,                           //!< uiHSyncSYNCwidth
			0x01                            //!< uiVSyncSYNCwidth
		},

		// tIDE_PARAM
		{
			/* Old prototype */
			PINMUX_LCD_SEL_SERIAL_RGB_6BITS,//!< pinmux_select_lcd;
			ICST_CCIR601,                   //!< icst;
			{FALSE, FALSE},                 //!< dithering[2];
			DISPLAY_DEVICE_CASIO2G,         //!< **DONT-CARE**
			IDE_PDIR_RBG,                   //!< pdir;
			IDE_LCD_R,                      //!< odd;
			IDE_LCD_R,                      //!< even;
			TRUE,                           //!< hsinv;
			TRUE,                           //!< vsinv;

			FALSE,                          //!< hvldinv;
			FALSE,                          //!< vvldinv;

			TRUE,                           //!< clkinv;
			TRUE,                           //!< fieldinv;

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
			{IDE_DITHER_6BITS, IDE_DITHER_6BITS, IDE_DITHER_6BITS} //!< DitherBits[3]
		},

		(tPANEL_CMD *)tCmdModeRgbd320,                 //!< pCmdQueue
		sizeof(tCmdModeRgbd320) / sizeof(tPANEL_CMD),  //!< nCmd
	}

};

const tPANEL_CMD tCmdRotate_None[] = {
	{IFCMD_CMD, 0x36},  // Memory Access Control
	{IFCMD_DATA, 0xC0}, // 48
	{IFCMD_CMD, 0x2c},  // memory write
};

const tPANEL_CMD tCmdRotate_180[] = {
	{IFCMD_CMD, 0x36},  // Memory Access Control
	{IFCMD_DATA, 0x00}, // 88
	{IFCMD_CMD, 0x2c},  // memory write
};

const tLCD_ROT tRotate[] = {
	{DISPDEV_LCD_ROTATE_NONE, (tPANEL_CMD *)tCmdRotate_None, 3},
	{DISPDEV_LCD_ROTATE_180, (tPANEL_CMD *)tCmdRotate_180, 3}
};
//@}

tLCD_ROT *dispdev_getLcdRotateCmd(UINT32 *ModeNumber)
{
#if 0
	if (tRotate != NULL) {
		*ModeNumber = sizeof(tRotate) / sizeof(tLCD_ROT);
	} else {
		*ModeNumber = 0;
	}
#else
    *ModeNumber = sizeof(tRotate)/sizeof(tLCD_ROT);
#endif	
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

	uiSIFData = ((((UINT32)addr << 8) | ((UINT32)value)) << 16);

	pDispDevControl(DISPDEV_IOCTRL_GET_REG_IF, &DevIOCtrl);
	sif_send(DevIOCtrl.SEL.GET_REG_IF.uiSifCh, uiSIFData, 0, 0);
}

void dispdev_writeToLcdGpio(DISPDEV_IOCTRL pDispDevControl, UINT32 addr, UINT32 value)
{
	//UINT32                  uiSIFData,j;
	UINT32                  j;
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

	//uiSIFData = (UINT32)addr << 8 | (UINT32)value;

	Delay_DelayUs(500);

	gpio_clearPin(SifSen);

	if (addr == IFCMD_CMD) {
		value = (value & 0x00ff);
		for (j = 9; j > 0; j--) {
			if (((value >> (j - 1)) & 0x01)) {
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
	} else if (addr == IFCMD_DATA) {
		value = ((value & 0x00ff) | 0x100);
		for (j = 9; j > 0; j--) {
			if (((value >> (j - 1)) & 0x01)) {
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
	} else if (addr == CMDDELAY_MS) {
		Delay_DelayUs(value * 1000);
	} else if (addr == CMDDELAY_US) {
		Delay_DelayUs(value);
	}

	Delay_DelayUs(500);
	gpio_setPin(SifSen);
}

void dispdev_readFromLcdGpio(DISPDEV_IOCTRL pDispDevControl, UINT32 addr, UINT32 *p_value)
{
}
#endif


