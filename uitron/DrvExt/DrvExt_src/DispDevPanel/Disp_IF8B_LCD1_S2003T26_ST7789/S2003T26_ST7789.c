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


#define IFCMD_CMD   0x01000000
#define IFCMD_DATA  0x02000000

void dispdev_writeToLcdGpio_Cmd(DISPDEV_IOCTRL pDispDevControl, UINT32 addr, UINT32 value);
void dispdev_writeToLcdGpio_data(DISPDEV_IOCTRL pDispDevControl, UINT32 addr, UINT32 value);

/*Used in RGB 960*/
const tPANEL_CMD tCmdModeRgbd320[] =


 {


{IFCMD_CMD,0x11},

{CMDDELAY_MS, 120},
//-------------Display and color format setting---------------//

{IFCMD_CMD,0x36},
{IFCMD_DATA,0xFF},

{IFCMD_CMD,0x3a},  //262k,參數設定為0X66, 65K，參數設定為0X55
{IFCMD_DATA,0x66},
//----------ST7789S Frame rate setting----------//

{IFCMD_CMD,0xb2},
{IFCMD_DATA,0x08},
{IFCMD_DATA,0x08},
{IFCMD_DATA,0x00},
{IFCMD_DATA,0x22},
{IFCMD_DATA,0x22},

{IFCMD_CMD,0xb7},
{IFCMD_DATA,0x35},

{IFCMD_CMD,0x2A},
{IFCMD_DATA,0x00},
{IFCMD_DATA,0x00},
{IFCMD_DATA,0x01},
{IFCMD_DATA,0x3F},


{IFCMD_CMD,0x2B},
{IFCMD_DATA,0x00},
{IFCMD_DATA,0x00},
{IFCMD_DATA,0x00},
{IFCMD_DATA,0xEF},

//----------ST7789S Power setting-----------------//

{IFCMD_CMD,0xbb},
{IFCMD_DATA,0x27},

{IFCMD_CMD,0xc0},
{IFCMD_DATA,0x2c},

{IFCMD_CMD,0xc2},
{IFCMD_DATA,0x01},

{IFCMD_CMD,0xc3},
{IFCMD_DATA,0x0b}, //VRH--->值越大越暗，反之越白

{IFCMD_CMD,0xc4},
{IFCMD_DATA,0x20},

{IFCMD_CMD,0xc6},
{IFCMD_DATA,0x0f},

{IFCMD_CMD,0xd0},
{IFCMD_DATA,0xa4},//有变更，请注意要用A7,根据老化情况更改
{IFCMD_DATA,0xa1},


{IFCMD_CMD,0xB0},     //影响RGB显示
{IFCMD_DATA,0x11},  //11
{IFCMD_DATA,0xF4},  //CC

{IFCMD_CMD,0xB1},  //影响RGB显示,如果显示方向不对，请调整这里
{IFCMD_DATA,0xEE},
{IFCMD_DATA,0x04}, //04
{IFCMD_DATA,0x12}, //15

//-----------ST7789S gamma setting-----------------//

{IFCMD_CMD,0xe0},
{IFCMD_DATA,0xd0},
{IFCMD_DATA,0x00},
{IFCMD_DATA,0x02},
{IFCMD_DATA,0x07},
{IFCMD_DATA,0x07},
{IFCMD_DATA,0x19},
{IFCMD_DATA,0x2e},
{IFCMD_DATA,0x54},
{IFCMD_DATA,0x41},
{IFCMD_DATA,0x2d},
{IFCMD_DATA,0x17},
{IFCMD_DATA,0x18},
{IFCMD_DATA,0x14},
{IFCMD_DATA,0x18},

{IFCMD_CMD,0xe1},
{IFCMD_DATA,0xd0},
{IFCMD_DATA,0x00},
{IFCMD_DATA,0x02},
{IFCMD_DATA,0x07},
{IFCMD_DATA,0x04},
{IFCMD_DATA,0x24},
{IFCMD_DATA,0x2c},
{IFCMD_DATA,0x44},
{IFCMD_DATA,0x42},
{IFCMD_DATA,0x1c},
{IFCMD_DATA,0x1a},
{IFCMD_DATA,0x17},
{IFCMD_DATA,0x15},
{IFCMD_DATA,0x18},

{IFCMD_CMD,0x29},
{IFCMD_CMD,0x2C},



 }
 ;



const tPANEL_CMD tCmdStandby[] =
{
    {0x05, 0x5E}
};

const tLCD_PARAM tMode[] =
{
    /***********       RGB-SERIAL MODE         *************/
    {
        // tPANEL_PARAM
        {
            /* Old prototype */
            PINMUX_LCDMODE_RGB_THROUGH,      //!< LCDMode

		   6.6f,							 //!< fDCLK
		   240+48, 						   //!< uiHSyncTotalPeriod
		   240, 						   //!< uiHSyncActivePeriod
		   24, 						       //!< uiHSyncBackPorch
		   320+32,							 //!< uiVSyncTotalPeriod
		   320, 						   //!< uiVSyncActivePeriod
		   6,							  //!< uiVSyncBackPorchOdd
		   6,							  //!< uiVSyncBackPorchEven


            240,                            //!< uiBufferWidth
            320,                            //!< uiBufferHeight
            240,                            //!< uiWindowWidth
            320,                            //!< uiWindowHeight

            FALSE,                          //!< bYCbCrFormat

            /* New added parameters */
            0x05,                           //!< uiHSyncSYNCwidth
            0x02                            //!< uiVSyncSYNCwidth
        },

        // tIDE_PARAM
        {
            /* Old prototype */
            PINMUX_LCD_SEL_SERIAL_RGB_6BITS,//!< pinmux_select_lcd;
            ICST_CCIR601,                   //!< icst;
            {1,1},                  //!< dithering[2];
            DISPLAY_DEVICE_TOPPOLY,         //!< **DONT-CARE**
            //
			IDE_PDIR_RGB,					//!< pdir;
	        IDE_LCD_R,					  //!< odd;
	        IDE_LCD_R,

            TRUE,                           //!< hsinv;
            TRUE,                           //!< vsinv;
            FALSE,                          //!< hvldinv;
            FALSE,                          //!< vvldinv;
            FALSE,                           //!< clkinv;
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
    },
};


//const tLCD_ROT *tRotate = NULL;

const tPANEL_CMD tCmdRotate_None[] =
{
 {IFCMD_CMD, 0x36},
	 {IFCMD_DATA,0x08},

};
const tPANEL_CMD tCmdRotate_180[] =
{
 {IFCMD_CMD, 0x36},
	 {IFCMD_DATA,0xFF},

};
const tLCD_ROT tRotate[] =
{
    {DISPDEV_LCD_ROTATE_NONE,(tPANEL_CMD*)tCmdRotate_None,3},
    {DISPDEV_LCD_ROTATE_180,(tPANEL_CMD*)tCmdRotate_180,3}
};




//@}

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

#define      TimerDelayUs                Delay_DelayUs
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
  //  debug_err(("Has not implement dispdev_readFromLcdGpio \r\n"));


}




