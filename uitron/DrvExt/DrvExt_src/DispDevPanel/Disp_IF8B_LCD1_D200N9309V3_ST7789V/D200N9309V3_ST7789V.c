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
    Panel Parameters for ALT020DSLN
*/
//@{

//#define ILI9432C
//#define ILI9432
#define ILI9431
#ifdef ILI9431  //lim20130413

#define IFCMD_CMD   0x01000000
#define IFCMD_DATA  0x02000000

void dispdev_writeToLcdGpio_Cmd(DISPDEV_IOCTRL pDispDevControl, UINT32 addr, UINT32 value);
void dispdev_writeToLcdGpio_data(DISPDEV_IOCTRL pDispDevControl, UINT32 addr, UINT32 value);

/*Used in RGB 960*/
const tPANEL_CMD tCmdModeRgbThrough320[] =
{
#if 1//ST7789V FOR BOE2.0  RGB v3
{IFCMD_CMD,0x11},
{CMDDELAY_MS,120}, //Delay 120ms

//------------------------------display and color format setting--------------------------------//
{IFCMD_CMD,0x36},
{IFCMD_DATA,0xF7},

{IFCMD_CMD,0x3A},
{IFCMD_DATA,0x06},

{IFCMD_CMD,0xB0},   //影响RGB显示
{IFCMD_DATA,0x11},  //11
{IFCMD_DATA,0xF4},  //CC

{IFCMD_CMD,0xB1},  //影响RGB显示,如果显示方向不对，请调整这里
{IFCMD_DATA,0xEE},
{IFCMD_DATA,0x04}, //04
{IFCMD_DATA,0x14}, //15

//--------------------------------ST7789V Frame rate setting----------------------------------//
{IFCMD_CMD,0xB2},
{IFCMD_DATA,0x0C},
{IFCMD_DATA,0x0C},
{IFCMD_DATA,0x00},
{IFCMD_DATA,0x33},
{IFCMD_DATA,0x33},

{IFCMD_CMD,0xB7},
{IFCMD_DATA,0x35},

//---------------------------------ST7789V Power setting--------------------------------------//
{IFCMD_CMD,0xBB},
{IFCMD_DATA,0x22},//0x1C},

{IFCMD_CMD,0xC0},
{IFCMD_DATA,0x2C},

{IFCMD_CMD,0xC2},
{IFCMD_DATA,0x01},

{IFCMD_CMD,0xC3},
{IFCMD_DATA,0x0a},//0x0B},

{IFCMD_CMD,0xC4},
{IFCMD_DATA,0x1B},//0x20

{IFCMD_CMD,0xC6},
{IFCMD_DATA,0x0F},

{IFCMD_CMD,0xD0},
{IFCMD_DATA,0xA4},
{IFCMD_DATA,0xA1},


//{IFCMD_CMD, 0x26},
//{IFCMD_DATA,0x02},//gammal


//{IFCMD_CMD, 0x55},//color enhance
//{IFCMD_DATA,0x80},


//--------------------------------ST7789V gamma setting---------------------------------------//
{IFCMD_CMD,0xE0},
{IFCMD_DATA,0xd0},
{IFCMD_DATA,0x00},
{IFCMD_DATA,0x03},
{IFCMD_DATA,0x09},
{IFCMD_DATA,0x13},
{IFCMD_DATA,0x1c},
{IFCMD_DATA,0x3a},
{IFCMD_DATA,0x55},
{IFCMD_DATA,0x48},
{IFCMD_DATA,0x18},
{IFCMD_DATA,0x12},
{IFCMD_DATA,0x0e},
{IFCMD_DATA,0x19},
{IFCMD_DATA,0x1e},

{IFCMD_CMD,0xE1},
{IFCMD_DATA,0xd0},
{IFCMD_DATA,0x00},
{IFCMD_DATA,0x03},
{IFCMD_DATA,0x09},
{IFCMD_DATA,0x05},
{IFCMD_DATA,0x25},
{IFCMD_DATA,0x3a},
{IFCMD_DATA,0x55},
{IFCMD_DATA,0x50},
{IFCMD_DATA,0x3d},
{IFCMD_DATA,0x1c},
{IFCMD_DATA,0x1d},
{IFCMD_DATA,0x1d},
{IFCMD_DATA,0x1e},

{IFCMD_CMD,0x29},//Display on

{IFCMD_CMD,0x2C},

#endif //D200FPC9309

#if 0
{CMDDELAY_MS, 120},

{IFCMD_CMD, 0x11},

{CMDDELAY_MS, 120},

{IFCMD_CMD, 0x36},// Memory data access control
{IFCMD_DATA,0x00},//0x00  MY MX MV ML RGB MH XX XX

{IFCMD_CMD, 0x3A},
{IFCMD_DATA,0x66}, //06 ---->262K(RGB666);05---->65K(RGB565)

{IFCMD_CMD, 0xB2},
{IFCMD_DATA,0x0C},//0c
{IFCMD_DATA,0x0C},//0c
{IFCMD_DATA,0x00},//00
{IFCMD_DATA,0x33},
{IFCMD_DATA,0x33},

{IFCMD_CMD, 0xB7},
{IFCMD_DATA,0x35},

{IFCMD_CMD, 0xBB},
{IFCMD_DATA,0x35},//VCOM 0x35

{IFCMD_CMD, 0xC0},
{IFCMD_DATA,0x2C},

{IFCMD_CMD, 0xC2},
{IFCMD_DATA,0x01},

{IFCMD_CMD, 0xC3},// VRH 值越大越暗，反之越白
{IFCMD_DATA,0x11},

{IFCMD_CMD, 0xC4},
{IFCMD_DATA,0x10},//0x20},//提升屏显亮度
/*
{IFCMD_CMD, 0xC6},
{IFCMD_DATA,0x09},//frame

{IFCMD_CMD, 0xD0},
{IFCMD_DATA,0xA4},//内部代码
{IFCMD_DATA,0xA1},
*/
{IFCMD_CMD, 0xC6},
{IFCMD_DATA,0x0f},//frame

{IFCMD_CMD, 0xD0},
{IFCMD_DATA,0xA7},//内部代码
{IFCMD_DATA,0xA1},


{IFCMD_CMD, 0xB0},//影响RGB 显示
{IFCMD_DATA,0x11},// 11
{IFCMD_DATA,0xc4},// CC f4

{IFCMD_CMD, 0xB1},//影响RGB 显示如果显示方向不对，请调整这里
{IFCMD_DATA,0xEE},
{IFCMD_DATA,0x04},// 04
{IFCMD_DATA,0x14},// 14 15


//{IFCMD_CMD, 0x26},
//{IFCMD_DATA,0x02},

{IFCMD_CMD, 0xE0},//GAMMA
{IFCMD_DATA,0xD0},
{IFCMD_DATA,0x00},
{IFCMD_DATA,0x06},
{IFCMD_DATA,0x09},
{IFCMD_DATA,0x0B},
{IFCMD_DATA,0x2A},
{IFCMD_DATA,0x3C},
{IFCMD_DATA,0x55},
{IFCMD_DATA,0x4B},
{IFCMD_DATA,0x08},
{IFCMD_DATA,0x16},
{IFCMD_DATA,0x14},
{IFCMD_DATA,0x19},
{IFCMD_DATA,0x20},

{IFCMD_CMD, 0xE1},//GAMMA
{IFCMD_DATA,0xD0},
{IFCMD_DATA,0x00},
{IFCMD_DATA,0x06},
{IFCMD_DATA,0x09},
{IFCMD_DATA,0x0B},
{IFCMD_DATA,0x29},
{IFCMD_DATA,0x36},
{IFCMD_DATA,0x54},
{IFCMD_DATA,0x4B},
{IFCMD_DATA,0x0D},
{IFCMD_DATA,0x16},
{IFCMD_DATA,0x14},
{IFCMD_DATA,0x21},
{IFCMD_DATA,0x20},

{IFCMD_CMD, 0x29},
{IFCMD_DATA,0x2C},

#endif




};
#endif

const tPANEL_CMD tCmdStandby[] =
{
    {CMDDELAY_US, 10},//{0x05, 0x5E}
};

const tLCD_PARAM tMode[] =
{
    /***********       RGB-SERIAL MODE         *************/
    {
        // tPANEL_PARAM
        {
#if 0//ST7789
            /* Old prototype */
            PINMUX_LCDMODE_RGB_SERIAL,//PINMUX_LCDMODE_RGB_SERIAL,      //!< LCDMode
            20.0f,//27.0f,                           //!< fDCLK
            980,                            //!< uiHSyncTotalPeriod
            720,                            //!< uiHSyncActivePeriod
            20+48-6,//20+48-6,//20,                             //!< uiHSyncBackPorch
            320+20,//320+20,                            //!< uiVSyncTotalPeriod
            320+20,//320+20,                            //!< uiVSyncActivePeriod
            1,                             //!< uiVSyncBackPorchOdd
            1,                             //!< uiVSyncBackPorchEven
            320,                            //!< uiBufferWidth
            720,                            //!< uiBufferHeight
            320,                            //!< uiWindowWidth
            720,                            //!< uiWindowHeight
            FALSE,                          //!< bYCbCrFormat
#else //ILI9341
            /* Old prototype */
            PINMUX_LCDMODE_RGB_THROUGH,//PINMUX_LCDMODE_RGB_SERIAL,      //!< LCDMode
            6.6f,                           //!< fDCLK
            240+48,                            //!< uiHSyncTotalPeriod
            240,                            //!< uiHSyncActivePeriod
            24,                         //!< uiHSyncBackPorch
            320+32,//                            //!< uiVSyncTotalPeriod
            320,//                            //!< uiVSyncActivePeriod
            6,// 1,                          //!< uiVSyncBackPorchOdd
            6,// 1,                         //!< uiVSyncBackPorchEven

            240,                            //!< uiBufferWidth
            320,                            //!< uiBufferHeight
            240,                            //!< uiWindowWidth
            320,//720,                            //!< uiWindowHeight
            FALSE,                          //!< bYCbCrFormat
#endif


            /* New added parameters */
            0x05,    //                       //!< uiHSyncSYNCwidth
            0x02      //0x05                      //!< uiVSyncSYNCwidth
        },

        // tIDE_PARAM
        {
            /* Old prototype */
            PINMUX_LCD_SEL_SERIAL_RGB_6BITS,//!< pinmux_select_lcd;
            ICST_CCIR601,                   //!< icst;
            {1,1},                  //!< dithering[2];
            DISPLAY_DEVICE_TOPPOLY,         //!< **DONT-CARE**
#if 0//new panel
            IDE_PDIR_RGB,//IDE_PDIR_RGB,//IDE_PDIR_RBG,                   //!< pdir;
            IDE_LCD_B,//IDE_LCD_B,//IDE_LCD_R,                      //!< odd;
            IDE_LCD_B,//IDE_LCD_B,//IDE_LCD_R,                      //!< even;
#else
            IDE_PDIR_RGB,                   //!< pdir;
            IDE_LCD_R,                      //!< odd;
            IDE_LCD_R,                      //!< even;
#endif

            TRUE,                           //!< hsinv;
            TRUE,                           //!< vsinv;
            FALSE,                          //!< hvldinv;
            FALSE,                          //!< vvldinv;
            FALSE,                           //!< clkinv;
            FALSE,                          //!< fieldinv;
            FALSE,                           //!< rgbdummy
            FALSE,                          //!< interlace;
            FALSE,                          //!< **DONT-CARE**
            0x40,//0x40,                           //!< ctrst;
            0x00,                           //!< brt;
            0x40,   //0x40                        //!< cmults;
            FALSE,                          //!< cex;
            FALSE,                          //!< **DONT-CARE**
            TRUE,                           //!< **DONT-CARE**
            TRUE,                           //!< tv_powerdown;
            {0x00,0x00},                    //!< **DONT-CARE**

            /* New added parameters */
            FALSE,                          //!< YCex
            FALSE,                          //!< HLPF
            {TRUE,TRUE,TRUE},            //!< subpixOdd[3]
            {FALSE,FALSE,FALSE},            //!< subpixEven[3]
            {IDE_DITHER_6BITS,IDE_DITHER_6BITS,IDE_DITHER_6BITS}//!< DitherBits[3]
        },

        (tPANEL_CMD*)tCmdModeRgbThrough320,                  //!< pCmdQueue
        sizeof(tCmdModeRgbThrough320)/sizeof(tPANEL_CMD),    //!< nCmd
    },
};

//const tLCD_ROT *tRotate = NULL;

const tPANEL_CMD tCmdRotate_None[] =
{
    {IFCMD_CMD,0x36},   // Memory Access Control
    {IFCMD_DATA,0x00},//{IFCMD_DATA,0xC0},  // 48
    {IFCMD_CMD,0x2c},   // memory write

};
const tPANEL_CMD tCmdRotate_180[] =
{
    {IFCMD_CMD,0x36},   // Memory Access Control
    {IFCMD_DATA,0xf7},//{IFCMD_DATA,0x00},  // 88
    {IFCMD_CMD,0x2c},   // memory write
};


const tLCD_ROT tRotate[] =
{
    {DISPDEV_LCD_ROTATE_NONE,(tPANEL_CMD*)tCmdRotate_None,3},
    {DISPDEV_LCD_ROTATE_180,(tPANEL_CMD*)tCmdRotate_180,3}
};

//@}

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
    *ModeNumber = 0;
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

#define      TimerDelayUs                Delay_DelayUs
#if 1
void dispdev_writeToLcdSif(DISPDEV_IOCTRL pDispDevControl, UINT32 addr, UINT32 value)
{
   // UINT32                  uiSIFData;
   // DISPDEV_IOCTRL_PARAM    DevIOCtrl;

   // uiSIFData = ((((UINT32)addr << 8)|(UINT32)value) << 16);

   // pDispDevControl(DISPDEV_IOCTRL_GET_REG_IF, &DevIOCtrl);
    //sif_sendMSB(DevIOCtrl.SEL.GET_REG_IF.uiSifCh, uiSIFData);

}

void dispdev_writeToLcdGpio_Cmd(DISPDEV_IOCTRL pDispDevControl, UINT32 addr, UINT32 value)
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

//debug_err(("===dispdev_writeToLcdGpio_Cmd =0x%x\r\n",uiSIFData));

    TimerDelayUs(500);

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

        TimerDelayUs(100);
        gpio_setPin(SifClk);
        TimerDelayUs(200);
        gpio_clearPin(SifClk);
        TimerDelayUs(100);
    }
    TimerDelayUs(500);
    gpio_setPin(SifSen);

}

void dispdev_writeToLcdGpio_data(DISPDEV_IOCTRL pDispDevControl, UINT32 addr, UINT32 value)
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

//debug_err(("===dispdev_writeToLcdGpio_data =0x%x\r\n",uiSIFData));

    TimerDelayUs(500);

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

        TimerDelayUs(100);
        gpio_setPin(SifClk);
        TimerDelayUs(200);
        gpio_clearPin(SifClk);
        TimerDelayUs(100);
    }
    TimerDelayUs(500);
    gpio_setPin(SifSen);

}

void dispdev_writeToLcdGpio(DISPDEV_IOCTRL pDispDevControl, UINT32 addr, UINT32 value)
{
    //UINT32                  uiSIFData,j;
    //UINT32                  SifClk,SifSen,SifData;
    //DISPDEV_IOCTRL_PARAM    DevIOCtrl;
    if(addr & IFCMD_DATA)
        dispdev_writeToLcdGpio_data(pDispDevControl,addr,value);
    else
        dispdev_writeToLcdGpio_Cmd(pDispDevControl,addr,value);
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


    uiSIFData = (UINT32)addr;//uiSIFData = ((UINT32)addr << 8)/* | (0x00004000)*/;

//debug_err(("===dispdev_readFromLcdGpio =0x%x\r\n",uiSIFData));

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

        TimerDelayUs(100);
        gpio_setPin(SifClk);
        TimerDelayUs(200);
        gpio_clearPin(SifClk);
        TimerDelayUs(100);
    }
    TimerDelayUs(500);


    for(j = 16; j > 0; j--)//for(j = 16; j > 0; j--)
    {
            gpio_setDir(SifData, GPIO_DIR_INPUT);

            Delay_DelayUs(100);
            gpio_clearPin(SifClk);
            Delay_DelayUs(200);
            gpio_setPin(SifClk);
            Delay_DelayUs(100);
            *p_value = (*p_value << 1) | gpio_getPin(SifData);
    }

    DBG_ERR("addr  = 0x%x\r\n", addr);
    DBG_ERR("value = 0x%x\r\n", *p_value);

    Delay_DelayUs(500);
    gpio_setPin(SifSen);
}
#endif
