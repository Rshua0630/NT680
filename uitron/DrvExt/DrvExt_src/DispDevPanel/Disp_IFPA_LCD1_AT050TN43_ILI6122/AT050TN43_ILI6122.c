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
#include "dispdev_IFParal.h"
#include "dispdev_panlcomn.h"


/*
    Panel Parameters for TXDT270M
*/
//@{


/*Used in RGB-Dummy 320*/
const tPANEL_CMD tCmdModeRgbd320[] =

{


	{0x00, 0x0D},
//  RESET_H;
//  Delayms(10);
//  RESET_L;
//  Delayms(50);
//  RESET_H;
//  Delayms(50);
	{CMDDELAY_MS, 10},
	{0x00, 0x0f},
	{0x00, 0x05},
	{CMDDELAY_MS, 10},
	{0x00, 0x0f},
	{0x00, 0x05},
	{CMDDELAY_MS, 10},
	{0x00, 0x0f},
	{0x05, 0x00},
	{CMDDELAY_MS, 10},
#ifdef DoubleSDCardEN  //lim20140310
	{0x01, 0xE4},
	{0x02, 0x03},
	{0x03, 0x05},
	{0x04, 0x03},
	{0x07, 0x35},
	{0x0C, 0x05},
	{0x0E, 0x13},
	{0x06, 0x01},
#else



#if 1
#ifdef DoubleSDCardEN  //lim20140410
////////  hei
	{0x01, 0xE4},
	{0x02, 0x03},
	{0x03, 0x0a},
	{0x04, 0x03},
	{0x07, 0x40},
	{0x0C, 0x07},
	{0x0E, 0x13},
	{0x06, 0x01},
	{0x05, 0x3f},
#else

	{0x01, 0xE4},
	{0x02, 0x03},
	{0x03, 0x08},
	{0x04, 0x03},
	{0x07, 0x40},
	{0x0C, 0x07},
	{0x0E, 0x13},
	{0x06, 0x01},
	{0x05, 0x3f},

#endif
#else

	{0x01, 0xE4},
	{0x05, 0x60},
	{0x06, 0x00},
	{0x07, 0x40},
	{0x0a, 0x00},
	{0x0C, 0x07},
	{0x0E, 0x03},
//  {0x06,0x01},
//{0x04,0x00},


#endif

#endif



};


const tPANEL_CMD tCmdStandby[] = {
	//  {0x2B, 0x00}

	{0x00, 0x00}
};

const tPANEL_CMD tCmdStandbyParal[] = {

};

const tLCD_PARAM tModeParal[] = {
	/***********       RGB Dummy-320 MODE         *************/
	{
		// tPANEL_PARAM
		{
			/* Old prototype */
			PINMUX_LCDMODE_RGB_PARALL,//PINMUX_LCDMODE_RGBD320,         //!< LCDMode
			40.0f,                         //!< fDCLK
			1056,//1559,                           //!< uiHSyncTotalPeriod
			800,//1280,                           //!< uiHSyncActivePeriod
			46,//251,                            //!< uiHSyncBackPorch
			525,//262,                            //!< uiVSyncTotalPeriod
			480,//240,                            //!< uiVSyncActivePeriod
			23,//12,                             //!< uiVSyncBackPorchOdd
			23,//12,                             //!< uiVSyncBackPorchEven
			800,//320,                            //!< uiBufferWidth
			480,//240,                            //!< uiBufferHeight
			800,//320,                            //!< uiWindowWidth
			480,//240,                            //!< uiWindowHeight
			FALSE,                          //!< bYCbCrFormat

			/* New added parameters */
			0x01,                           //!< uiHSyncSYNCwidth
			0x01                            //!< uiVSyncSYNCwidth
		},

		// tIDE_PARAM
		{
			/* Old prototype */
			PINMUX_LCD_SEL_PARALLE_RGB888 | PINMUX_LCD_SEL_DE_ENABLE, //PINMUX_LCD_SEL_SERIAL_RGB_8BITS,//!< pinmux_select_lcd;
			ICST_CCIR601,                   //!< icst;
			{FALSE, FALSE},                 //!< dithering[2];
			DISPLAY_DEVICE_AU,         //!< **DONT-CARE**
			IDE_PDIR_RGB,                   //!< pdir;
			IDE_LCD_B,                      //!< odd;
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
			0x45,                           //!< cmults;
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
			{IDE_DITHER_6BITS, IDE_DITHER_6BITS, IDE_DITHER_6BITS} //!< DitherBits[3]
		},

		(tPANEL_CMD *)tCmdModeRgbd320,                 //!< pCmdQueue
		sizeof(tCmdModeRgbd320) / sizeof(tPANEL_CMD),  //!< nCmd
	}
};

const tLCD_ROT *tRotate = NULL;

//@}

tLCD_ROT *dispdev_getLcdRotateCmdParal(UINT32 *ModeNumber)
{
	if (tRotate != NULL) {
		*ModeNumber = sizeof(tRotate) / sizeof(tLCD_ROT);
	} else {
		*ModeNumber = 0;
	}
	return (tLCD_ROT *)tRotate;
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

	uiSIFData = (UINT32)addr << 12 | (UINT32)value;
	//debug_msg("uiSIFData   =%5d \r\n",uiSIFData);

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
	//  debug_err(("Has not implement dispdev_readFromLcdGpio \r\n"));
}
#endif
