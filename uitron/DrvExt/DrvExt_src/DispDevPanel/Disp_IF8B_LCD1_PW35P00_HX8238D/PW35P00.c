/*
    Display object object panel configuration parameters for PW35P00

    @file       PW35P00.c
    @ingroup
    @note       Nothing

    Copyright   Novatek Microelectronics Corp. 2014.  All rights reserved.
*/
#include "display.h"
#include "dispdev_IF8bits.h"
#include "dispdev_panlcomn.h"


/*
    Panel Parameters for PW35P00
*/
//@{
const tPANEL_CMD tCmdStandby[] = {
	{0x2B, 0x00}
};

const tPANEL_CMD tCmdModeRgb320[] = {
	{0x70, 0x0001},  //First Transmission (Register 0x01)
	{0x72, 0x6300},  //First Transmission (Data     0x6300)

	{0x70, 0x0002},  //First Transmission (Register 0x02)
	{0x72, 0x0200},  //First Transmission (Data     0x0200)

	{0x70, 0x0003},  //First Transmission (Register 0x03)
	{0x72, 0x7164},  //First Transmission (Data     0x7164)

	{0x70, 0x0004},  //First Transmission (Register 0x04)
	{0x72, 0x044f},  //First Transmission (Data     0x044f, Serial-RGB format)

	{0x70, 0x0005},  //First Transmission (Register 0x05)
	{0x72, 0xb4d4},  //First Transmission (Data     0xb4d4)

	{0x70, 0x000a},  //First Transmission (Register 0x0a)
	{0x72, 0x4008},  //First Transmission (Data     0x4008)

	{0x70, 0x000b},  //First Transmission (Register 0x0b)
	{0x72, 0xd400},  //First Transmission (Data     0xd400)

	{0x70, 0x000d},  //First Transmission (Register 0x0d)
	{0x72, 0x123a},  //First Transmission (Data     0x123a)

	{0x70, 0x000e},  //First Transmission (Register 0x0e)
	{0x72, 0x2c00},  //First Transmission (Data     0x2c00)

	{0x70, 0x000f},  //First Transmission (Register 0x0f)
	{0x72, 0x0000},  //First Transmission (Data     0x0000)

	{0x70, 0x0016},  //First Transmission (Register 0x16)
	{0x72, 0x9f80},  //First Transmission (Data     0x9f80)

	{0x70, 0x0017},  //First Transmission (Register 0x17)
	{0x72, 0x2212},  //First Transmission (Data     0x2212)

	{0x70, 0x001e},  //First Transmission (Register 0x1e)
	{0x72, 0x00cb},  //First Transmission (Data     0x00cb)

	{0x70, 0x0030},  //First Transmission (Register 0x30)
	{0x72, 0x0507},  //First Transmission (Data     0x0507)

	{0x70, 0x0031},  //First Transmission (Register 0x31)
	{0x72, 0x0004},  //First Transmission (Data     0x0004)

	{0x70, 0x0032},  //First Transmission (Register 0x32)
	{0x72, 0x0707},  //First Transmission (Data     0x0707)

	{0x70, 0x0033},  //First Transmission (Register 0x33)
	{0x72, 0x0000},  //First Transmission (Data     0x0000)

	{0x70, 0x0034},  //First Transmission (Register 0x34)
	{0x72, 0x0000},  //First Transmission (Data     0x0000)

	{0x70, 0x0035},  //First Transmission (Register 0x35)
	{0x72, 0x0307},  //First Transmission (Data     0x0307)

	{0x70, 0x0036},  //First Transmission (Register 0x36)
	{0x72, 0x0700},  //First Transmission (Data     0x0700)

	{0x70, 0x0037},  //First Transmission (Register 0x37)
	{0x72, 0x0000},  //First Transmission (Data     0x0000)

	{0x70, 0x003a},  //First Transmission (Register 0x3b)
	{0x72, 0x140b},  //First Transmission (Data     0x140b)

	{0x70, 0x003b},  //First Transmission (Register 0x3b)
	{0x72, 0x140b},  //First Transmission (Data     0x140b)
};

const tLCD_PARAM tMode[] = {
	/***********       RGB 320 MODE         *************/
	{
		// tPANEL_PARAM
		{
			/* Old prototype */
			PINMUX_LCDMODE_RGB_SERIAL,         //!< LCDMode
#if (_EMULATION_ == ENABLE)
#if (_FPGA_EMULATION_ == ENABLE)
			9.75f,                          //!< fDCLK
#else
			19.5f,                          //!< fDCLK
#endif
#else
			19.5f,                         //!< fDCLK
#endif
			1224,                           //!< uiHSyncTotalPeriod
			960,                            //!< uiHSyncActivePeriod
			204,                            //!< uiHSyncBackPorch
			262,                            //!< uiVSyncTotalPeriod
			240,                            //!< uiVSyncActivePeriod
			18,                             //!< uiVSyncBackPorchOdd
			18,                             //!< uiVSyncBackPorchEven
			960,                            //!< uiBufferWidth
			240,                            //!< uiBufferHeight
			960,                            //!< uiWindowWidth
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

		(tPANEL_CMD *)tCmdModeRgb320,                 //!< pCmdQueue
		sizeof(tCmdModeRgb320) / sizeof(tPANEL_CMD),  //!< nCmd
	}

};
const tPANEL_CMD tCmdRotate_None[] = {
	//{0x04,0x18}
};
const tPANEL_CMD tCmdRotate_180[] = {
	//{0x04,0x1b}
};
const tLCD_ROT tRotate[] = {
	{DISPDEV_LCD_ROTATE_NONE, (tPANEL_CMD *)tCmdRotate_None, 1},
	{DISPDEV_LCD_ROTATE_180, (tPANEL_CMD *)tCmdRotate_180, 1}
};

const tLCD_SIF_PARAM tSIFParam[] = {
	// SIF mode 00
	SIF_MODE_00,
	// Bus clock maximun = 20 MHz
	18000000,
	// SENS = 0
	0,
	// SENH = 30
	30,
	// 24 bits per transmission
	24,
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



#if 1
void dispdev_writeToLcdSif(DISPDEV_IOCTRL pDispDevControl, UINT32 addr, UINT32 value)
{
	UINT32                  uiSIFData;
	DISPDEV_IOCTRL_PARAM    DevIOCtrl;

	//uiSIFData = ((((UINT32)addr << 8)|((UINT32)value)) << 16);
	//uiSIFData = ((((UINT32)addr << 8)|((UINT32)value)) << 16);
	uiSIFData = (UINT32)addr << 24 | (UINT32)value << 8;

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

	uiSIFData = (UINT32)addr << 16 | (UINT32)value;
	//debug_err(("Has not implement dispdev_readFromLcdGpio%x %x  %x\r\n",uiSIFData,addr,value));
	Delay_DelayUs(500);

	gpio_clearPin(SifSen);

	for (j = 24; j > 0; j--) {
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

tLCD_SIF_PARAM *dispdev_getLcdSifParam(void)
{
	return (tLCD_SIF_PARAM *)tSIFParam;
}

#endif
