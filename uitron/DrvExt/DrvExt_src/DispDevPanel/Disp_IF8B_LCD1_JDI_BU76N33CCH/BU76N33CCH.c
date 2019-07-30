/*
    Display object object panel configuration parameters for TXDT270CDR_2V7

    @file       BU76N33CCH.c
    @ingroup
    @note       Nothing

    Copyright   Novatek Microelectronics Corp. 2011.  All rights reserved.
*/
//#include "debug.h"
#include "display.h"
#include "dispdev_IF8bits.h"
#include "dispdev_panlcomn.h"


const tPANEL_CMD tCmdStandby[] =
{
    //{0x2B, 0x00}
	{CMDDELAY_MS, 150},
};

const tPANEL_CMD tCmdModeRgbd320[] =
{
{0x00,0x48},
{CMDDELAY_MS, 150},
{0x01,0x00},
{0x02,0x52},
{0x03,0x26},
{0x04,0xF4},
//{0x05,0x08},
{0x05,0x10},
{0x06,0x00},
{0x07,0x10},
//{0x07,0x14},

{0x08,0x7B},  //YUV 720

{0x09,0x0F},
{0x0A,0x17},
{0x0B,0x20},
{0x0C,0x20},
{0x0D,0x17},
{0x0E,0x48},
{0x0F,0x48},

{0x10,0x28},
{0x11,0x48},
{0x12,0x40},
{0x13,0x40},
{0x14,0x40},
{0x15,0x40},
//{0x16,0x00},
{0x16,0xC8},
{0x17,0xC6},

{0x18,0xB4},
{0x19,0x00},
{0x1A,0x88},
{0x1B,0xFF},
{0x1C,0x00},
{0x1D,0x03},
{0x1E,0x1D},
{0x1F,0xA1},

{0x20,0xF0},
{0x21,0x80},
{0x22,0x80},
{0x23,0x80},
{0x24,0x00},
{0x25,0x00},
{0x26,0x97},
{0x27,0x8D},

{0x28,0xB4},
{0x29,0x74},
{0x2A,0xD8},
{0x2B,0x17},
{0x2C,0x47},
{0x2D,0x97},
{0x2E,0x9E},
{0x2F,0xA5},

{0x30,0x72},
{0x31,0xC9},
{0x32,0x2A},
{0x33,0x4B},
{0x34,0x88},
{0x35,0xAC},
{0x36,0xAF},
{0x37,0xA7},

{0x38,0x9A},
{0x39,0xA1},
{0x3A,0xB4},
{0x3B,0x8B},
{0x3C,0xAA},
{0x3D,0xAF},
{0x3E,0xA3},
{0x3F,0x94},

{0x40,0x99},
{0x41,0xB3},
{0x42,0x8A},
{0x43,0xB0},
{0x44,0xBB},
{0x45,0xB9},
{0x46,0xA8},
{0x47,0xA8},

{0x48,0xB1},
{0x49,0x8A},
{0x4A,0xAA},
{0x4B,0xAA},
{0x4C,0xA6},
{0x4D,0x92},
{0x4E,0x9F},
{0x4F,0xB6},

{0x50,0x80},
{0x51,0x80},
{0x52,0xFF},
{0x53,0xFF},
{0x54,0x00},
{0x55,0x13},
{0x56,0x00},
{0x57,0xE0},

{0x58,0x20},
{0x59,0x40},
{0x5A,0x0A},
{0x5B,0x00},
{0x5C,0xDD},
{0x5D,0x01},
{0x5E,0x07},
{0x5F,0xE0},


{0x60,0x80},
{0x61,0x7B},
{0x62,0x80},
{0x63,0x06},
{0x64,0x00},
{0x65,0x00},
{0x66,0x00},
{0x67,0x30},

{0x68,0x56},
{0x69,0x72},
{0x6A,0x80},
{0x6B,0x00},
{0x6C,0x0A},
{0x6D,0x69},
{0x6E,0x00},
{0x6F,0x11},

{0x00,0x49},

{CMDDELAY_MS, 150},
};


const tLCD_PARAM tMode[] =
{
    /***********       YUV720 MODE         *************/
	{
        // tPANEL_PARAM
        {
            /* Old prototype */
            PINMUX_LCDMODE_YUV720,          //!< LCDMode
            27.0f,                          //!< fDCLK
//            36.0f,                          //!< fDCLK
            1716,                           //!< uiHSyncTotalPeriod
//            1724,                           //!< uiHSyncTotalPeriod
            1440,                           //!< uiHSyncActivePeriod
//            1440,                           //!< uiHSyncActivePeriod
//            8,                            //!< uiHSyncBackPorch
//            294,                            //!< uiHSyncBackPorch
            124,                            //!< uiHSyncBackPorch
//            245,                            //!< uiHSyncBackPorch
            525,                            //!< uiVSyncTotalPeriod
//            400,                            //!< uiVSyncActivePeriod
            480,                            //!< uiVSyncActivePeriod
            38,                             //!< uiVSyncBackPorchOdd
            38,                             //!< uiVSyncBackPorchEven
            720,                            //!< uiBufferWidth
            480,                            //!< uiBufferHeight
            720,                            //!< uiWindowWidth
            480,                            //!< uiWindowHeight
//            FALSE,                           //!< bYCbCrFormat
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
            {FALSE,FALSE},                  //!< dithering[2];
            DISPLAY_DEVICE_AU,              //!< **DONT-CARE**
            IDE_PDIR_RGB,                   //!< pdir;
            IDE_LCD_R,                      //!< odd;
            IDE_LCD_G,                      //!< even;
//            IDE_LCD_G,                      //!< even;
            FALSE,                           //!< hsinv;
            FALSE,                           //!< vsinv;
            FALSE,                          //!< hvldinv;
            FALSE,                          //!< vvldinv;
            TRUE,                           //!< clkinv;
//            TRUE,                          //!< fieldinv;
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
            {0x00,0x00},                    //!< **DONT-CARE**

            /* New added parameters */
            TRUE,                          //!< YCex
            FALSE,                          //!< HLPF
            {FALSE,FALSE,FALSE},            //!< subpixOdd[3]
            {FALSE,FALSE,FALSE},            //!< subpixEven[3]
            {IDE_DITHER_6BITS,IDE_DITHER_6BITS,IDE_DITHER_6BITS},//!< DitherBits[3]
            FALSE                           //!< clk1/2
        },

        (tPANEL_CMD*)tCmdModeRgbd320,                  //!< pCmdQueue
        sizeof(tCmdModeRgbd320)/sizeof(tPANEL_CMD),    //!< nCmd
    }
};


const tPANEL_CMD tCmdRotate_None[] =
{
	{CMDDELAY_MS, 150},
    //{0x04,0x18}
};
const tPANEL_CMD tCmdRotate_180[] =
{
	{CMDDELAY_MS, 150},
    //{0x04,0x1b}
};

const tLCD_ROT tRotate[] =
{
    {DISPDEV_LCD_ROTATE_NONE,(tPANEL_CMD*)tCmdRotate_None,1},
    {DISPDEV_LCD_ROTATE_180,(tPANEL_CMD*)tCmdRotate_180,1}
};

tLCD_ROT* dispdev_getLcdRotateCmd(UINT32 *ModeNumber)
{
    if(tRotate != NULL)
    {
        *ModeNumber = sizeof(tRotate)/sizeof(tLCD_ROT);
    }
    else
    {
        *ModeNumber = 0;
    }
    return (tLCD_ROT*)tRotate;
}

tLCD_PARAM* dispdev_getConfigMode(UINT32 *ModeNumber)
{
    *ModeNumber = sizeof(tMode)/sizeof(tLCD_PARAM);
    return (tLCD_PARAM*)tMode;
}

tPANEL_CMD* dispdev_getStandbyCmd(UINT32 *CmdNumber)
{
    *CmdNumber = sizeof(tCmdStandby)/sizeof(tPANEL_CMD);
    return (tPANEL_CMD*)tCmdStandby;
}

void dispdev_writeToLcdSif(DISPDEV_IOCTRL pDispDevControl, UINT32 addr, UINT32 value)
{
    UINT32                  uiSIFData;
    DISPDEV_IOCTRL_PARAM    DevIOCtrl;

    uiSIFData = ((((UINT32)addr << 8)|((UINT32)value)) << 16);

    pDispDevControl(DISPDEV_IOCTRL_GET_REG_IF, &DevIOCtrl);
    sif_send(DevIOCtrl.SEL.GET_REG_IF.uiSifCh, uiSIFData, 0, 0);
}

void dispdev_writeToLcdGpio(DISPDEV_IOCTRL pDispDevControl, UINT32 addr, UINT32 value)
{
    UINT32                  uiSIFData,j;
    UINT32                  SifClk,SifSen,SifData;
    DISPDEV_IOCTRL_PARAM    DevIOCtrl;

    pDispDevControl(DISPDEV_IOCTRL_GET_REG_IF, &DevIOCtrl);
    SifClk  = DevIOCtrl.SEL.GET_REG_IF.uiGpioClk;
    SifSen  = DevIOCtrl.SEL.GET_REG_IF.uiGpioSen;
    SifData = DevIOCtrl.SEL.GET_REG_IF.uiGpioData;

    gpio_setDir(SifSen, GPIO_DIR_OUTPUT);
    gpio_setDir(SifClk, GPIO_DIR_OUTPUT);
    gpio_setDir(SifData, GPIO_DIR_OUTPUT);

    gpio_setPin(SifSen);

	gpio_clearPin(SifData);
	gpio_clearPin(SifClk);
	Delay_DelayUs(10);

    uiSIFData = (UINT32)addr << 8 | (UINT32)value;
    //debug_err(("Has not implement dispdev_readFromLcdGpio%x %x  %x\r\n",uiSIFData,addr,value));
    Delay_DelayUs(500);

    gpio_clearPin(SifSen);
    for(j = 16; j > 0; j--)
    {
        if(((uiSIFData >> (j - 1)) & 0x01))
            gpio_setPin(SifData);
        else
            gpio_clearPin(SifData);

        Delay_DelayUs(100);
//        DBGD(gpio_getPin(SifData));
        gpio_clearPin(SifClk);
        Delay_DelayUs(200);
//        DBGD(gpio_getPin(SifClk));
        gpio_setPin(SifClk);
        Delay_DelayUs(100);
//        DBGD(gpio_getPin(SifClk));
    }

    Delay_DelayUs(500);
    gpio_setPin(SifSen);

}


void dispdev_readFromLcdGpio(DISPDEV_IOCTRL pDispDevControl, UINT32 addr, UINT32* p_value)
{
    UINT32                  uiSIFData = 0,j;
    UINT32                  SifClk,SifSen,SifData;
    DISPDEV_IOCTRL_PARAM    DevIOCtrl;

    *p_value = 0;

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

    uiSIFData = ((UINT32)addr << 8) | (0x00004000);

    Delay_DelayUs(500);

    gpio_clearPin(SifSen);

    for(j = 16; j > 0; j--)
    {
        if (j >= 9)
        {
            if(((uiSIFData >> (j - 1)) & 0x01))
                gpio_setPin(SifData);
            else
                gpio_clearPin(SifData);

            Delay_DelayUs(100);
            gpio_clearPin(SifClk);
            Delay_DelayUs(200);
            gpio_setPin(SifClk);
        }
        else
        {
            gpio_setDir(SifData, GPIO_DIR_INPUT);

            Delay_DelayUs(100);
            gpio_clearPin(SifClk);
            Delay_DelayUs(200);
            gpio_setPin(SifClk);
            Delay_DelayUs(100);
            *p_value = (*p_value << 1) | gpio_getPin(SifData);
        }
    }

    Delay_DelayUs(500);
    gpio_setPin(SifSen);
}


