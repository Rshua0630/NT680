/*
    Display object object panel configuration parameters for TXDT270CDR_2V7

    @file       ILI9342C.c
    @ingroup
    @note       Nothing

    Copyright   Novatek Microelectronics Corp. 2011.  All rights reserved.
*/

#include "display.h"
#include "dispdev_IF8bits.h"
#include "dispdev_panlcomn.h"


#define IFCMD_CMD   0x01000000
#define IFCMD_DATA  0x02000000
const tPANEL_CMD tCmdStandby[] =
{

//  {MICMD_CMD,0x28}, //Display off
  {CMDDELAY_US, 1},
//  {MICMD_CMD,0x10}, //enter sleep mode
//  {MICMD_CMD,0x2c}, //memory write
};
const tPANEL_CMD tCmdModeRgbd320[] =
{
        {IFCMD_CMD, 0xc8},
        {IFCMD_DATA,0xFF},
        {IFCMD_DATA,0x93},
        {IFCMD_DATA,0x42},

        {IFCMD_CMD, 0x36},
        {IFCMD_DATA,0x08},//0xc8 for rotate
        //{IFCMD_CMD, 0x36}, 
        //{IFCMD_DATA,0xC8},//0xc8 for rotate

        {IFCMD_CMD, 0x3A},
        {IFCMD_DATA,0x66},

        {IFCMD_CMD, 0xc0},
        {IFCMD_DATA,0x0a},  //  {IFCMD_DATA,0x0f},
        {IFCMD_DATA,0x0a},//  {IFCMD_DATA,0x0f},

        {IFCMD_CMD, 0xC1},
        {IFCMD_DATA,0x01},

        {IFCMD_CMD, 0xC5},
        {IFCMD_DATA,0xb9}, //   {IFCMD_DATA,0xdb},

        {IFCMD_CMD, 0xb4},
        {IFCMD_DATA,0x02},

        {IFCMD_CMD, 0xe0},
        {IFCMD_DATA,0x00},
        {IFCMD_DATA,0x05},
        {IFCMD_DATA,0x08},
        {IFCMD_DATA,0x02},
        {IFCMD_DATA,0x1a},
        {IFCMD_DATA,0x0c},
        {IFCMD_DATA,0x42},
        {IFCMD_DATA,0x7a},
        {IFCMD_DATA,0x54},
        {IFCMD_DATA,0x08},
        {IFCMD_DATA,0x0d},
        {IFCMD_DATA,0x0c},
        {IFCMD_DATA,0x23},
        {IFCMD_DATA,0x25},
        {IFCMD_DATA,0x0f},

        {IFCMD_CMD, 0xE1},
        {IFCMD_DATA,0x00},
        {IFCMD_DATA,0x29},
        {IFCMD_DATA,0x2f},
        {IFCMD_DATA,0x03},
        {IFCMD_DATA,0x0F},
        {IFCMD_DATA,0x05},
        {IFCMD_DATA,0x42},
        {IFCMD_DATA,0x55},
        {IFCMD_DATA,0x53},
        {IFCMD_DATA,0x06},
        {IFCMD_DATA,0x0f},
        {IFCMD_DATA,0x0c},
        {IFCMD_DATA,0x38},
        {IFCMD_DATA,0x3a},
        {IFCMD_DATA,0x0f},

        {IFCMD_CMD, 0xb0},
        {IFCMD_DATA,0xe0},

        {IFCMD_CMD, 0xf6},
        {IFCMD_DATA,0x01},//0X01
        {IFCMD_DATA,0x00},
        {IFCMD_DATA,0x07},

        {IFCMD_CMD, 0x11}, //Exit Sleep
        {CMDDELAY_MS, 120},

        {IFCMD_CMD, 0x29}, //Display ON
        //{IFCMD_CMD, 0x2C}, //Display ON
};

const tLCD_PARAM tMode[] =
{
    /***********       RGB Dummy-320 MODE         *************/
    {
        // tPANEL_PARAM
        {
            /* Old prototype */
            PINMUX_LCDMODE_RGB_SERIAL,      //!< LCDMode
            24.0f,//24.0f,   //27.0f,                           //!< fDCLK
            1750,                            //!< uiHSyncTotalPeriod
            960,                            //!< uiHSyncActivePeriod
            20,                             //!< uiHSyncBackPorch  26 32
            246,                            //!< uiVSyncTotalPeriod
            240,                            //!< uiVSyncActivePeriod
            2,                             //!< uiVSyncBackPorchOdd
            2,                             //!< uiVSyncBackPorchEven
            960,                            //!< uiBufferWidth
            240,                            //!< uiBufferHeight
            960,                            //!< uiWindowWidth
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
            {FALSE,FALSE},                  //!< dithering[2];
            DISPLAY_DEVICE_TOPPOLY,         //!< **DONT-CARE**
            IDE_PDIR_RGB,                   //!< pdir;
            IDE_LCD_R,                      //!< odd;
            IDE_LCD_R,                      //!< even;
            TRUE,                           //!< hsinv;
            TRUE,                           //!< vsinv;
            FALSE,                          //!< hvldinv;
            FALSE,                          //!< vvldinv;
            TRUE,                           //!< clkinv;
            FALSE,                          //!< fieldinv;
            FALSE,                           //!< rgbdummy
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
            FALSE,                          //!< YCex
            FALSE,                          //!< HLPF
            {TRUE,TRUE,TRUE},               //!< subpixOdd[3]
            {FALSE,FALSE,FALSE},            //!< subpixEven[3]
            {IDE_DITHER_6BITS,IDE_DITHER_6BITS,IDE_DITHER_6BITS}//!< DitherBits[3]
        },

        (tPANEL_CMD*)tCmdModeRgbd320,                  //!< pCmdQueue
        sizeof(tCmdModeRgbd320)/sizeof(tPANEL_CMD),    //!< nCmd
    }

};


#define ROTATE_PARA_COUNT 5
const tPANEL_CMD tCmdRotate_None[] =
{
        {IFCMD_CMD, 0x11}, //Exit Sleep
        {IFCMD_CMD, 0x36},
        {IFCMD_DATA,0x08},//0xc8 for rotate
        {IFCMD_CMD, 0x29}, //Display ON
        {IFCMD_CMD,0x2c}, //memory write
};
const tPANEL_CMD tCmdRotate_180[] =
{
        {IFCMD_CMD, 0x11}, //Exit Sleep
        {IFCMD_CMD, 0x36},
        {IFCMD_DATA,0xC8},//0xc8 for rotate
        {IFCMD_CMD, 0x29}, //Display ON
        {IFCMD_CMD,0x2c}, //memory write
};
const tLCD_ROT tRotate[] =
{
    {DISPDEV_LCD_ROTATE_NONE,(tPANEL_CMD*)tCmdRotate_None,ROTATE_PARA_COUNT},
    {DISPDEV_LCD_ROTATE_180,(tPANEL_CMD*)tCmdRotate_180,ROTATE_PARA_COUNT}
};

tLCD_ROT* dispdev_getLcdRotateCmd(UINT32 *ModeNumber)
{
#if 0
    if(tRotate != NULL)
    {
        *ModeNumber = sizeof(tRotate)/sizeof(tLCD_ROT);
    }
    else
    {
        *ModeNumber = 0;
    }
#else
    *ModeNumber = sizeof(tRotate)/sizeof(tLCD_ROT);
#endif
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

static void dispdev_writeToLcdGpio_Cmd(DISPDEV_IOCTRL pDispDevControl, UINT32 addr, UINT32 value)
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
    gpio_setPin(SifData);
    gpio_clearPin(SifClk);

    uiSIFData = (UINT32)value;

    Delay_DelayUs(500);

    gpio_clearPin(SifSen);
    for(j=9; j>0; j--)
    {
        if(((uiSIFData >> (j - 1)) & 0x01))
        {
            gpio_setPin(SifData);
        }
        else
        {
            gpio_clearPin(SifData);
        }

        Delay_DelayUs(100);
        gpio_setPin(SifClk);
        Delay_DelayUs(200);
        gpio_clearPin(SifClk);
        Delay_DelayUs(100);
    }
    Delay_DelayUs(500);
    gpio_setPin(SifSen);

}


static void dispdev_writeToLcdGpio_data(DISPDEV_IOCTRL pDispDevControl, UINT32 addr, UINT32 value)
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
    gpio_setPin(SifData);
    gpio_clearPin(SifClk);

    uiSIFData = (UINT32)value;
    uiSIFData |= 0x100;

    Delay_DelayUs(500);

    gpio_clearPin(SifSen);

    for(j=9; j>0; j--)
    {
        if(((uiSIFData >> (j - 1)) & 0x01))
        {
            gpio_setPin(SifData);
        }
        else
        {
            gpio_clearPin(SifData);
        }

        Delay_DelayUs(100);
        gpio_setPin(SifClk);
        Delay_DelayUs(200);
        gpio_clearPin(SifClk);
        Delay_DelayUs(100);
    }
    Delay_DelayUs(500);
    gpio_setPin(SifSen);
}
void dispdev_writeToLcdGpio(DISPDEV_IOCTRL pDispDevControl, UINT32 addr, UINT32 value)
{

    if(addr & IFCMD_DATA)
        dispdev_writeToLcdGpio_data(pDispDevControl,addr,value);
    else
        dispdev_writeToLcdGpio_Cmd(pDispDevControl,addr,value);
}

void dispdev_readFromLcdGpio(DISPDEV_IOCTRL pDispDevControl, UINT32 addr, UINT32* p_value)
{
    //debug_err(("Has not implement dispdev_readFromLcdGpio \r\n"));
}

