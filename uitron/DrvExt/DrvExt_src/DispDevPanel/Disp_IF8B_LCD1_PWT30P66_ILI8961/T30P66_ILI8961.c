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
const tPANEL_CMD tCmdStandby[] = {
	{0x2B, 0x00}
};


const tPANEL_CMD tCmdModeRgbd320[] = {
	{0x05, 0x5F},  //set power mode (standby)
	{CMDDELAY_MS, 5},
	{0x05, 0x1F},
	{CMDDELAY_MS, 10},
	{0x05, 0x5F},
	{CMDDELAY_MS, 50},

	{0x2B, 0x01},
	{0x0B, 0x81},
	{0x00, 0x0F},

	//{0x01,0xBA},
	{0x01, 0xea}, //b8//0xe9
	{0x04, 0x1b}, //

	{0x0F, 0x34}, //Red////40
	{0x11, 0x38}, //Blue////30

	{0x2f, 0x69},
	{0x16, 0x04},
	{0x17, 0x77},
	{0x18, 0x77},
	{0x19, 0x33},
	{0x1A, 0x43},

	{0x95, 0x80},
	{0xAF, 0x04},

	{0xA5, 0x22},
	{0xA6, 0x12},

	{0xa7, 0x06},
};


const tLCD_PARAM tMode[] = {
	/***********       RGB Dummy-320 MODE         *************/
	{
		// tPANEL_PARAM
		{
			/* Old prototype */
			PINMUX_LCDMODE_RGBD320,         //!< LCDMode
			24.54f,                         //!< fDCLK
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
			0x01,                           //!< uiHSyncSYNCwidth
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
			IDE_LCD_B,                      //!< even;
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
			0x58,                           //!< cmults;
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
	//{0x04,0x1b}
	{0x04, 0x08},
};
const tPANEL_CMD tCmdRotate_180[] = {
	{0x04, 0x1B}
	//{0x04,0x18},
};
const tLCD_ROT tRotate[] = {
	{DISPDEV_LCD_ROTATE_NONE, (tPANEL_CMD *)tCmdRotate_None, 1},
	{DISPDEV_LCD_ROTATE_180, (tPANEL_CMD *)tCmdRotate_180, 1}
};
//@}


tLCD_ROT *dispdev_getLcdRotateCmd(UINT32 *ModeNumber)
{
#if 0
	if (tRotate != NULL) {
		*ModeNumber = sizeof(tRotate) / sizeof(tLCD_ROT);
	} else 
#endif    
    {
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
	//debug_err(("Has not implement dispdev_readFromLcdGpio%x %x  %x\r\n",uiSIFData,addr,value));
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
	//debug_err(("Has not implement dispdev_readFromLcdGpio \r\n"));
}
#endif
