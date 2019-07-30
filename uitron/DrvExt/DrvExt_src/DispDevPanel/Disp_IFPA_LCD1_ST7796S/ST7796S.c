/*
    Display object object panel configuration parameters for ILI8961

    @file       TG240320C104B_ST7789V.c
    @ingroup
    @note       Nothing

    Copyright   Novatek Microelectronics Corp. 2017.  All rights reserved.
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

    {CMDDELAY_MS, 120},
    {0x00, 0x11},  // Sleep Out
    {CMDDELAY_MS, 120},

//    {0x00, 0x36},                          // Memory Data Access Control MY,MX~~
//    {0x01, 0x48},                          // ．1・ =BGR color filter panel ?--LWP
//    {0x01, 0x08},                          // ．1・ =BGR color filter panel ?--LWP

    {0x00, 0x3a},                          // Interface Pixel Format
    {0x01, 0x55},                          // ．110・ = 18bit/pixel

    {0x00, 0xf0},
    {0x01, 0xc3},

    {0x00, 0xf0},
    {0x01, 0x96},

    {0x00, 0xb0},                          // Interface Mode Control
    {0x01, 0x80},                          // VSCP HSCP PKP DEP Polarity

    {0x00, 0xb5},
    {0x01, 0x23},
    {0x01, 0x14},
    {0x01, 0x00},
    {0x01, 0x0a},


    {0x00, 0xb6},                          // Display Function Control
    {0x01, 0xa0},                          // RGB interface
    {0x01, 0x42},
    {0x01, 0x3b},

    {0x00, 0xb7},                          // In Deep Standby mode (DSTB=1)
    {0x01, 0xc6},

    {0x00, 0xe8},                          // Display Output Ctrl Adjust
    {0x01, 0x40},
    {0x01, 0x8a},
    {0x01, 0x00},
    {0x01, 0x00},
    {0x01, 0x29},
    {0x01, 0x19},
    {0x01, 0xa5},
    {0x01, 0x33},

    {0x00, 0xc0},
    {0x01, 0x80},
    {0x01, 0x51},

    {0x00, 0xc1},                          // VAP(GVDD),VAN(GVCL)
    {0x01, 0x19},

    {0x00, 0xc2},                          // SOP[1:0]: Source driving current level,GOP[1:0]: Gamma driving current level
    {0x01, 0xa7},

    {0x00, 0xc5},                          // VCOM Control
    {0x01, 0x08},

    {0x00, 0xe0},                          // Positive Voltage Gamma Control
    {0x01, 0xa0},
    {0x01, 0x0b},
    {0x01, 0x14},
    {0x01, 0x0b},
    {0x01, 0x0b},
    {0x01, 0x27},
    {0x01, 0x3e},
    {0x01, 0x33},
    {0x01, 0x56},
    {0x01, 0x3b},
    {0x01, 0x1a},
    {0x01, 0x19},
    {0x01, 0x31},
    {0x01, 0x33},

    {0x00, 0xe1},                          // Negative Voltage Gamma Control
    {0x01, 0xa0},
    {0x01, 0x0b},
    {0x01, 0x14},
    {0x01, 0x0b},
    {0x01, 0x0b},
    {0x01, 0x27},
    {0x01, 0x3e},
    {0x01, 0x33},
    {0x01, 0x56},
    {0x01, 0x3b},
    {0x01, 0x1a},
    {0x01, 0x19},
    {0x01, 0x31},
    {0x01, 0x33},

    {0x00, 0xf0},
    {0x01, 0x3c},

    {0x00, 0xf0},
    {0x01, 0x69},  // Command Set Control

	{CMDDELAY_MS, 120},
    {0x00, 0x29},
	{CMDDELAY_MS, 120},
};


const tPANEL_CMD tCmdStandby[] =
{
  //  {0x2B, 0x00}

  {0x00, 0x00}
};

const tPANEL_CMD tCmdStandbyParal[] =
{

};

const tLCD_PARAM tModeParal[] =
{
    /***********       RGB Dummy-320 MODE         *************/
    {
        // tPANEL_PARAM
        {
            /* Old prototype */
            PINMUX_LCDMODE_RGB_PARALL565,   //!< LCDMode
            12.6f,                          //!< fDCLK
            560,                            //!< uiHSyncTotalPeriod
            320,                            //!< uiHSyncActivePeriod
            6,                              //!< uiHSyncBackPorch
            699,                            //!< uiVSyncTotalPeriod
            480,                            //!< uiVSyncActivePeriod
            16,                             //!< uiVSyncBackPorchOdd
            16,                             //!< uiVSyncBackPorchEven
            320,                            //!< uiBufferWidth
            480,                            //!< uiBufferHeight
            320,                            //!< uiWindowWidth
            480,                            //!< uiWindowHeight
            FALSE,                          //!< bYCbCrFormat

            /* New added parameters */
            0x01,                           //!< uiHSyncSYNCwidth
            0x01                            //!< uiVSyncSYNCwidth
        },

        // tIDE_PARAM
        {
            /* Old prototype */
            PINMUX_LCD_SEL_PARALLE_RGB565|PINMUX_LCD_SEL_DE_ENABLE,//PINMUX_LCD_SEL_SERIAL_RGB_8BITS,//!< pinmux_select_lcd;
            ICST_CCIR601,                   //!< icst;
            {FALSE,FALSE},                  //!< dithering[2];
            DISPLAY_DEVICE_AU,              //!< **DONT-CARE**
            IDE_PDIR_RGB,                   //!< pdir;
            IDE_LCD_B,                      //!< odd;
            IDE_LCD_G,                      //!< even;
            TRUE,                           //!< hsinv;
            TRUE,                           //!< vsinv;
            FALSE,                          //!< hvldinv;
            FALSE,                          //!< vvldinv;
            FALSE,                           //!< clkinv;
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
            {0x00,0x00},                    //!< **DONT-CARE**

            /* New added parameters */
            FALSE,                          //!< YCex
            FALSE,                          //!< HLPF
            {FALSE,FALSE,FALSE},               //!< subpixOdd[3]
            {FALSE,FALSE,FALSE},            //!< subpixEven[3]
            {IDE_DITHER_6BITS,IDE_DITHER_6BITS,IDE_DITHER_6BITS}//!< DitherBits[3]
        },

        (tPANEL_CMD*)tCmdModeRgbd320,                  //!< pCmdQueue
        sizeof(tCmdModeRgbd320)/sizeof(tPANEL_CMD),    //!< nCmd
    }
};

const tLCD_ROT *tRotate = NULL;

//@}

tLCD_ROT* dispdev_getLcdRotateCmdParal(UINT32 *ModeNumber)
{
//    if(tRotate != NULL)
//    {
    	  //Already determine if(tRotMI != NULL)
    	  //coverity[bad_sizeof]
//        *ModeNumber = sizeof(tRotate)/sizeof(tLCD_ROT);
//    }
//    else
//    {
        *ModeNumber = 0;
//    }
    return (tLCD_ROT*)tRotate;
}

tLCD_PARAM* dispdev_getConfigModeParal(UINT32 *ModeNumber)
{
    *ModeNumber = sizeof(tModeParal)/sizeof(tLCD_PARAM);
    return (tLCD_PARAM*)tModeParal;
}

tPANEL_CMD* dispdev_getStandbyCmdParal(UINT32 *CmdNumber)
{
    *CmdNumber = sizeof(tCmdStandbyParal)/sizeof(tPANEL_CMD);
    return (tPANEL_CMD*)tCmdStandbyParal;
}


#if 1
void dispdev_writeToLcdSifParal(DISPDEV_IOCTRL pDispDevControl, UINT32 addr, UINT32 value)
{



}


void dispdev_writeToLcdGpioParal(DISPDEV_IOCTRL pDispDevControl, UINT32 addr, UINT32 value)
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
    gpio_setPin(SifClk);

    uiSIFData = value << 23;
    if (addr)
         uiSIFData |= 0x80000000;
    //debug_msg("&&&&&uiSIFData   =0x%08x\r\n",uiSIFData);
    Delay_DelayUs(500);
    gpio_clearPin(SifSen);
    for(j = 9; j > 0; j--)
    {
        gpio_clearPin(SifClk);

        if(uiSIFData & 0x80000000)
            gpio_setPin(SifData);
        else
            gpio_clearPin(SifData);

        Delay_DelayUs(100);
        gpio_setPin(SifClk);
        Delay_DelayUs(100);
        uiSIFData <<= 1;
    }

    Delay_DelayUs(500);
    gpio_setPin(SifSen);
}



void dispdev_readFromLcdGpioParal(DISPDEV_IOCTRL pDispDevControl, UINT32 addr, UINT32* p_value)
{
  //  debug_err(("Has not implement dispdev_readFromLcdGpio \r\n"));
}
#endif
