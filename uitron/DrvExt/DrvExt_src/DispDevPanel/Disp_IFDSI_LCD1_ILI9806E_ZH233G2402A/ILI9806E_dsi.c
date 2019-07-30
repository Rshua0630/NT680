/*
    Display object for driving DSI device

    @file       NT355510.c
    @ingroup
    @note       This panel MUST select IDE clock to PLL1 ( 480 ). Once change to \n
                another frequence, the _IDE_fDCLK should be re calculated

    Copyright   Novatek Microelectronics Corp. 2011.  All rights reserved.
*/

#include "Debug.h"
#include "display.h"
#include "dispdev_IFdsi.h"
#include "dispdev_panlcomn.h"

#define PANEL_WIDTH     480
#define PANEL_HEIGHT    720

#define HX8369B_CFG_1_LANE  0
#define HX8369B_CFG_2_LANE  1

#define HX8369B_CFG_LANE    HX8369B_CFG_2_LANE


#define HX8369B_IND_MSG(...)       debug_msg(__VA_ARGS__)

#define HX8369B_ERR_MSG(...)       debug_msg(__VA_ARGS__)

#define HX8369B_WRN_MSG(...)       debug_msg(__VA_ARGS__)

#define HX8369B_TE_OFF              0
#define HX8369B_TE_ON               1

#define HX8369B_TE_PACKET           0
#define HX8369B_TE_PIN              1


/*
    RGB888 = 1 pixel = 3bytes packet
    If DSI src = 240MHz, internal clock = 30MHz, data rate = 30MHz x 1bytes = 30MB / sec per lane
    2 lane = 60MB = 20Mpixel ' IDE need > 20MHz
*/


#define DSI_FORMAT_RGB565          0    //N/A in HX8369B
#define DSI_FORMAT_RGB666P         1    //N/A in HX8369B
#define DSI_FORMAT_RGB666L         2    //N/A in HX8369B
#define DSI_FORMAT_RGB888          3    //IDE use 480 & DSI use 480

#define DSI_OP_MODE_CMD_MODE       1
#define DSI_OP_MODE_VDO_MODE       0

#define DSI_PACKET_FORMAT          DSI_FORMAT_RGB888



//#define DSI_TARGET_CLK             240
#define DSI_TARGET_CLK             480
//#define DSI_TARGET_CLK             160
//#define DSI_TARGET_CLK             120
#define DSI_OP_MODE                DSI_OP_MODE_VDO_MODE

#if (DSI_PACKET_FORMAT == DSI_FORMAT_RGB666P) && (DSI_OP_MODE == DSI_OP_MODE_CMD_MODE)
#error "Command mode not support RGB666P"
#endif
#if(DSI_PACKET_FORMAT == DSI_FORMAT_RGB888) || (DSI_PACKET_FORMAT == DSI_FORMAT_RGB666L)
#define _IDE_fDCLK      ((((DSI_TARGET_CLK / 8) * 2)/3)) * 2
#elif(DSI_PACKET_FORMAT == DSI_FORMAT_RGB666P)
#if (DSI_TARGET_CLK == 240)
#define _IDE_fDCLK      53.32f//26.66f * 2
#else
//#define _IDE_fDCLK      ((((((DSI_TARGET_CLK / 8) * 2))*9)/4))
#if(DSI_TARGET_CLK == 120)
#define _IDE_fDCLK      26.66f
#else

#endif
#endif
#elif(DSI_PACKET_FORMAT == DSI_FORMAT_RGB565)
#define _IDE_fDCLK      ((((DSI_TARGET_CLK / 8) * 2) / 2))
#endif


/*
    Panel Parameters for TCON HX8369B
*/
//@{
/*Used in DSI*/
const tPANEL_CMD tCmdModeDSI[] =
{

{DSICMD_CMD,0xFF},
{DSICMD_DATA,0xFF},
{DSICMD_DATA,0x98},
{DSICMD_DATA,0x06},
{DSICMD_DATA,0x04},
{DSICMD_DATA,0x01}, //Change to Page 1

{CMDDELAY_US,10},   
{DSICMD_CMD,0x08},{DSICMD_DATA,0X10}, //output SDA
{DSICMD_CMD,0x20},{DSICMD_DATA,0X01}, //(0x01:VSYNC mode)  (0x00:DE mode)


{DSICMD_CMD,0x21},{DSICMD_DATA,0X01}, 
{DSICMD_CMD,0x23},{DSICMD_DATA,0X02},  //Normally Black
{DSICMD_CMD,0x30},{DSICMD_DATA,0X04},  //02=480 x 800 , 03=480 x 640 , 04=480 x 720
{DSICMD_CMD,0x31},{DSICMD_DATA,0X00}, //00=Column , 02= 2 Dot
{DSICMD_CMD,0x40},{DSICMD_DATA,0X01}, //BT  +2
{CMDDELAY_US,10},   
{DSICMD_CMD,0x41},{DSICMD_DATA,0X33},  //DVDDH DVDDL clamp 33=+-5.2v , 77=+-6V
{CMDDELAY_US,10},   
{DSICMD_CMD,0x42},{DSICMD_DATA,0X00}, //VGH
{CMDDELAY_US,10},   
{DSICMD_CMD,0x43},{DSICMD_DATA,0X89}, //VGH_CLAMP ON ; CLAMP 15V
{CMDDELAY_US,10},   
{DSICMD_CMD,0x44},{DSICMD_DATA,0X89}, //VGL_CLAMP ON ; CLAMP -12V
{CMDDELAY_US,10},   
{DSICMD_CMD,0x46},{DSICMD_DATA,0X34}, //Power Control7
{CMDDELAY_US,10},   
{DSICMD_CMD,0x50},{DSICMD_DATA,0X88}, //VREG1OUT +4.7V
{CMDDELAY_US,10},   
{DSICMD_CMD,0x51},{DSICMD_DATA,0X88}, //VREG2OUT -4.7V
{CMDDELAY_US,10},    
{DSICMD_CMD,0x52},{DSICMD_DATA,0X00}, 
{DSICMD_CMD,0x53},{DSICMD_DATA,0X53}, //Forward Flicker -1.225V
{CMDDELAY_US,10},   
{DSICMD_CMD,0x54},{DSICMD_DATA,0X00}, 
{DSICMD_CMD,0x55},{DSICMD_DATA,0X60}, //Backward Flicker
{CMDDELAY_US,10},   
{DSICMD_CMD,0x60},{DSICMD_DATA,0X07},  //SDTI
{DSICMD_CMD,0x61},{DSICMD_DATA,0X04},  //CRTI
{DSICMD_CMD,0x62},{DSICMD_DATA,0X08}, //EQTI
{DSICMD_CMD,0x63},{DSICMD_DATA,0X04}, //PCTI
{DSICMD_CMD,0xA0},{DSICMD_DATA,0X00}, 
{DSICMD_CMD,0xA1},{DSICMD_DATA,0X0F}, 
{DSICMD_CMD,0xA2},{DSICMD_DATA,0X17}, 
{DSICMD_CMD,0xA3},{DSICMD_DATA,0X11}, 
{DSICMD_CMD,0xA4},{DSICMD_DATA,0X08}, 
{DSICMD_CMD,0xA5},{DSICMD_DATA,0X0B}, 
{DSICMD_CMD,0xA6},{DSICMD_DATA,0X06}, 
{DSICMD_CMD,0xA7},{DSICMD_DATA,0X04}, 
{DSICMD_CMD,0xA8},{DSICMD_DATA,0X08}, 
{DSICMD_CMD,0xA9},{DSICMD_DATA,0X0C}, 
{DSICMD_CMD,0xAA},{DSICMD_DATA,0X14}, 
{DSICMD_CMD,0xAB},{DSICMD_DATA,0X0D}, 
{DSICMD_CMD,0xAC},{DSICMD_DATA,0X16}, 
{DSICMD_CMD,0xAD},{DSICMD_DATA,0X0F}, 
{DSICMD_CMD,0xAE},{DSICMD_DATA,0X0A}, 
{DSICMD_CMD,0xAF},{DSICMD_DATA,0X0A}, 
{CMDDELAY_US,50},   
{DSICMD_CMD,0xC0},{DSICMD_DATA,0X00}, 
{DSICMD_CMD,0xC1},{DSICMD_DATA,0X0F}, 
{DSICMD_CMD,0xC2},{DSICMD_DATA,0X17}, 
{DSICMD_CMD,0xC3},{DSICMD_DATA,0X11}, 
{DSICMD_CMD,0xC4},{DSICMD_DATA,0X08}, 
{DSICMD_CMD,0xC5},{DSICMD_DATA,0X0B}, 
{DSICMD_CMD,0xC6},{DSICMD_DATA,0X06}, 
{DSICMD_CMD,0xC7},{DSICMD_DATA,0X04}, 
{DSICMD_CMD,0xC8},{DSICMD_DATA,0X08}, 
{DSICMD_CMD,0xC9},{DSICMD_DATA,0X0C}, 
{DSICMD_CMD,0xCA},{DSICMD_DATA,0X14}, 
{DSICMD_CMD,0xCB},{DSICMD_DATA,0X0D}, 
{DSICMD_CMD,0xCC},{DSICMD_DATA,0X16}, 
{DSICMD_CMD,0xCD},{DSICMD_DATA,0X0F}, 
{DSICMD_CMD,0xCE},{DSICMD_DATA,0X0A}, 
{DSICMD_CMD,0xCF},{DSICMD_DATA,0X0A}, 
{CMDDELAY_US,50},   
{DSICMD_CMD,0xFF},
{DSICMD_DATA,0XFF0},
{DSICMD_DATA,0X98},
{DSICMD_DATA,0X06},
{DSICMD_DATA,0X04},
{DSICMD_DATA,0X05}, //Change to Page 5
{CMDDELAY_US,10},   
{DSICMD_CMD,0x07},
{DSICMD_DATA,0XB1}, //PWM always high
{CMDDELAY_US,10},   
{DSICMD_CMD,0xFF},
{DSICMD_DATA,0XFF},
{DSICMD_DATA,0X98},
{DSICMD_DATA,0X06},
{DSICMD_DATA,0X04},
{DSICMD_DATA,0X06},	//Change to Page 6 
{CMDDELAY_US,10},   
{DSICMD_CMD,0x00},{DSICMD_DATA,0X21}, 
{DSICMD_CMD,0x01},{DSICMD_DATA,0X09}, 
{DSICMD_CMD,0x02},{DSICMD_DATA,0X00}, 
{DSICMD_CMD,0x03},{DSICMD_DATA,0X00}, 
{DSICMD_CMD,0x04},{DSICMD_DATA,0X01}, 
{DSICMD_CMD,0x05},{DSICMD_DATA,0X01}, 
{DSICMD_CMD,0x06},{DSICMD_DATA,0X98}, 
{DSICMD_CMD,0x07},{DSICMD_DATA,0X05}, 
{DSICMD_CMD,0x08},{DSICMD_DATA,0X02}, 
{DSICMD_CMD,0x09},{DSICMD_DATA,0X00}, 
{DSICMD_CMD,0x0A},{DSICMD_DATA,0X00}, 
{DSICMD_CMD,0x0B},{DSICMD_DATA,0X00}, 
{DSICMD_CMD,0x0C},{DSICMD_DATA,0X01}, 
{DSICMD_CMD,0x0D},{DSICMD_DATA,0X01}, 
{DSICMD_CMD,0x0E},{DSICMD_DATA,0X00}, 
{DSICMD_CMD,0x0F},{DSICMD_DATA,0X00}, 
{DSICMD_CMD,0x10},{DSICMD_DATA,0XE0}, 
{DSICMD_CMD,0x11},{DSICMD_DATA,0XE0}, 
{DSICMD_CMD,0x12},{DSICMD_DATA,0X00}, 
{DSICMD_CMD,0x13},{DSICMD_DATA,0X00}, 
{DSICMD_CMD,0x14},{DSICMD_DATA,0X00}, 
{DSICMD_CMD,0x15},{DSICMD_DATA,0X43}, 
{DSICMD_CMD,0x16},{DSICMD_DATA,0X0B}, 
{DSICMD_CMD,0x17},{DSICMD_DATA,0X00}, 
{DSICMD_CMD,0x18},{DSICMD_DATA,0X00}, 
{DSICMD_CMD,0x19},{DSICMD_DATA,0X00}, 
{DSICMD_CMD,0x1A},{DSICMD_DATA,0X00}, 
{DSICMD_CMD,0x1B},{DSICMD_DATA,0X00}, 
{DSICMD_CMD,0x1C},{DSICMD_DATA,0X00}, 
{DSICMD_CMD,0x1D},{DSICMD_DATA,0X00}, 
{DSICMD_CMD,0x20},{DSICMD_DATA,0X01}, 
{DSICMD_CMD,0x21},{DSICMD_DATA,0X23}, 
{DSICMD_CMD,0x22},{DSICMD_DATA,0X45}, 
{DSICMD_CMD,0x23},{DSICMD_DATA,0X67}, 
{DSICMD_CMD,0x24},{DSICMD_DATA,0X01}, 

{DSICMD_CMD,0x25},{DSICMD_DATA,0X23}, 
{DSICMD_CMD,0x26},{DSICMD_DATA,0X45}, 

{DSICMD_CMD,0x27},{DSICMD_DATA,0X67}, 
{DSICMD_CMD,0x30},{DSICMD_DATA,0X01}, 
{DSICMD_CMD,0x31},{DSICMD_DATA,0X11}, 
{DSICMD_CMD,0x32},{DSICMD_DATA,0X00}, 
{DSICMD_CMD,0x33},{DSICMD_DATA,0X22}, 
{DSICMD_CMD,0x34},{DSICMD_DATA,0X22}, 
{DSICMD_CMD,0x35},{DSICMD_DATA,0XCB}, 
{DSICMD_CMD,0x36},{DSICMD_DATA,0XDA}, 
{DSICMD_CMD,0x37},{DSICMD_DATA,0XAD}, 
{DSICMD_CMD,0x38},{DSICMD_DATA,0XBC}, 
{DSICMD_CMD,0x39},{DSICMD_DATA,0X66}, //START 4  
{DSICMD_CMD,0x3A},{DSICMD_DATA,0X77}, //START 2  
{DSICMD_CMD,0x3B},{DSICMD_DATA,0X22}, 
{DSICMD_CMD,0x3C},{DSICMD_DATA,0X22}, 
{DSICMD_CMD,0x3D},{DSICMD_DATA,0X22}, 
{DSICMD_CMD,0x3E},{DSICMD_DATA,0X22}, 
{DSICMD_CMD,0x3F},{DSICMD_DATA,0X22}, 
{DSICMD_CMD,0x40},{DSICMD_DATA,0X22}, 
{DSICMD_CMD,0x52},{DSICMD_DATA,0X10},  //sleep out 
{DSICMD_CMD,0x53},{DSICMD_DATA,0X10},  //VGLO = VGL
{CMDDELAY_US,100},   
{DSICMD_CMD,0xFF},
{DSICMD_DATA,0XFF},
{DSICMD_DATA,0X98},
{DSICMD_DATA,0X06},
{DSICMD_DATA,0X04},
{DSICMD_DATA,0X07}, //Change to Page 7
{CMDDELAY_US,10},   
{DSICMD_CMD,0x17},
{DSICMD_DATA,0X22},  //VGLO = VGL
{CMDDELAY_US,10},   
{DSICMD_CMD,0x18},
{DSICMD_DATA,0X1D},  //VREG1 VREG2 output
{CMDDELAY_US,10},   
{DSICMD_CMD,0x02},
{DSICMD_DATA,0X77}, 
{DSICMD_CMD,0xE1},
{DSICMD_DATA,0X79},  

{DSICMD_CMD,0xFF},
{DSICMD_DATA,0XFF},
{DSICMD_DATA,0X98},
{DSICMD_DATA,0X06},
{DSICMD_DATA,0X04},
{DSICMD_DATA,0X00},  	//Change to Page 0   

{DSICMD_CMD,0x36},
{DSICMD_DATA,0x02}, ///01   180du 
//00 01 10


{DSICMD_CMD,0x3a},
{DSICMD_DATA,0x55},
{CMDDELAY_US,10},
{DSICMD_CMD,0x11},                 // Sleep-Out
{CMDDELAY_US,120},
{DSICMD_CMD,0x29},                 // Display On
{CMDDELAY_US,10},          
{DSICMD_CMD,0x2c},                 // write gram
{CMDDELAY_US,10},  


};

const tPANEL_CMD tCmdStandbyDSI[] =
{
    {DSICMD_CMD,     0x28},     // Display OFF
    {DSICMD_CMD,     0x10}      // Sleep in
};

const tLCD_PARAM tModeDSI[] =
{
    /***********       MI Serial Format 1      *************/
    {
        // tPANEL_PARAM
        {
            /* Old prototype */
            //PINMUX_DSI_2_LANE_VDO_SYNC_PULSE_RGB888,
            //PINMUX_DSI_2_LANE_VDO_SYNC_EVENT_RGB888, //OK
            //PINMUX_DSI_2_LANE_VDO_SYNC_PULSE_RGB565,
            //PINMUX_DSI_2_LANE_VDO_SYNC_EVENT_RGB565, //OK
            //PINMUX_DSI_2_LANE_VDO_SYNC_EVENT_RGB666L,
            //PINMUX_DSI_2_LANE_VDO_SYNC_PULSE_RGB666L,
            //PINMUX_DSI_2_LANE_VDO_SYNC_EVENT_RGB666P,
            //PINMUX_DSI_2_LANE_VDO_SYNC_PULSE_RGB666P,
            PINMUX_DSI_2_LANE_VDO_SYNC_EVENT_RGB888,
            _IDE_fDCLK,                             //!< fDCLK
            (0x75+PANEL_WIDTH),                     //!< uiHSyncTotalPeriod
            PANEL_WIDTH,                            //!< uiHSyncActivePeriod
            //0x2F,                                   //!< uiHSyncBackPorch
            0x40,                                    //!< uiHSyncBackPorch
            0x40+PANEL_HEIGHT,                      //!< uiVSyncTotalPeriod
            PANEL_HEIGHT,                           //!< uiVSyncActivePeriod
            17,                                    //!< uiVSyncBackPorchOdd
            17,                                    //!< uiVSyncBackPorchEven
            PANEL_WIDTH,                           //!< uiBufferWidth
            PANEL_HEIGHT,                            //!< uiBufferHeight
            PANEL_WIDTH,                            //!< uiWindowWidth
            PANEL_HEIGHT,                           //!< uiWindowHeight
            FALSE,                                  //!< bYCbCrFormat

            /* New added parameters */
            //0x01,                                   //!< uiHSyncSYNCwidth
            0x04,                                   //!< uiHSyncSYNCwidth
            //0x01                                    //!< uiVSyncSYNCwidth
            0x02                                    //!< uiVSyncSYNCwidth
        },

        // tIDE_PARAM
        {
            /* Old prototype */
            PINMUX_LCD_SEL_GPIO,            //!< pinmux_select_lcd;
            ICST_CCIR601,                   //!< icst;
            {TRUE, FALSE},                  //!< dithering[2];
            DISPLAY_DEVICE_MIPIDSI,         //!< **DONT-CARE**
            IDE_PDIR_RGB,                   //!< pdir;
            IDE_LCD_R,                      //!< odd;
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
            0x40,                           //!< cmults;
            FALSE,                          //!< cex;
            FALSE,                          //!< **DONT-CARE**
            TRUE,                           //!< **DONT-CARE**
            TRUE,                           //!< tv_powerdown;
            {0x00,0x00},                    //!< **DONT-CARE**

            /* New added parameters */
            FALSE,                          //!< YCex
            FALSE,                          //!< HLPF
            {FALSE,FALSE,FALSE},            //!< subpixOdd[3]
            {FALSE,FALSE,FALSE},            //!< subpixEven[3]
            {IDE_DITHER_5BITS,IDE_DITHER_6BITS,IDE_DITHER_5BITS},//!< DitherBits[3]
            FALSE                           //!< clk1/2
        },

        (tPANEL_CMD*)tCmdModeDSI,                  //!< pCmdQueue
        sizeof(tCmdModeDSI)/sizeof(tPANEL_CMD),    //!< nCmd
    }
};

const tLCD_ROT *tRotDSI = NULL;

//@}

tLCD_ROT* dispdev_getLcdRotateDSICmd(UINT32 *ModeNumber)
{
    #if 0  
    if(tRotDSI != NULL)
    {
        *ModeNumber = sizeof(tRotDSI)/sizeof(tLCD_ROT);
    }
    else
    {
        *ModeNumber = 0;
    }
    #else
    *ModeNumber = 0;
    #endif
        
    return (tLCD_ROT*)tRotDSI;
}

tLCD_PARAM* dispdev_getConfigModeDSI(UINT32 *ModeNumber)
{

	
	    *ModeNumber = sizeof(tModeDSI)/sizeof(tLCD_PARAM);
	    return (tLCD_PARAM*)tModeDSI;
	
}

tPANEL_CMD* dispdev_getStandbyCmdDSI(UINT32 *CmdNumber)
{
	
	    *CmdNumber = sizeof(tCmdStandbyDSI)/sizeof(tPANEL_CMD);
	    return (tPANEL_CMD*)tCmdStandbyDSI;
	
}


void dispdev_setDSIConfig(tDSI_CONFIG *pDsiConfig)
{
    dsi_setConfig(DSI_CONFIG_ID_FREQ, DSI_TARGET_CLK * 1000000);
#if(DSI_TARGET_CLK == 160)
    dsi_setConfig(DSI_CONFIG_ID_TLPX, 1);
    dsi_setConfig(DSI_CONFIG_ID_BTA_TA_GO, 7);

    dsi_setConfig(DSI_CONFIG_ID_THS_PREPARE, 1);
    dsi_setConfig(DSI_CONFIG_ID_THS_ZERO, 4);
    dsi_setConfig(DSI_CONFIG_ID_THS_TRAIL, 2);
    dsi_setConfig(DSI_CONFIG_ID_THS_EXIT, 3);

    dsi_setConfig(DSI_CONFIG_ID_TCLK_PREPARE, 1);
    dsi_setConfig(DSI_CONFIG_ID_TCLK_ZERO, 7);
    dsi_setConfig(DSI_CONFIG_ID_TCLK_POST, 8);
    dsi_setConfig(DSI_CONFIG_ID_TCLK_PRE, 1);
    dsi_setConfig(DSI_CONFIG_ID_TCLK_TRAIL, 1);
#elif(DSI_TARGET_CLK == 480)
    dsi_setConfig(DSI_CONFIG_ID_TLPX, 3);
    dsi_setConfig(DSI_CONFIG_ID_BTA_TA_GO, 15);

    dsi_setConfig(DSI_CONFIG_ID_THS_PREPARE, 4);
    dsi_setConfig(DSI_CONFIG_ID_THS_ZERO, 5);
    dsi_setConfig(DSI_CONFIG_ID_THS_TRAIL, 4);
    dsi_setConfig(DSI_CONFIG_ID_THS_EXIT, 7);

    dsi_setConfig(DSI_CONFIG_ID_TCLK_PREPARE, 3);
    dsi_setConfig(DSI_CONFIG_ID_TCLK_ZERO, 14);
    dsi_setConfig(DSI_CONFIG_ID_TCLK_POST, 10);
    dsi_setConfig(DSI_CONFIG_ID_TCLK_PRE, 1);
    dsi_setConfig(DSI_CONFIG_ID_TCLK_TRAIL, 3);

#elif(DSI_TARGET_CLK == 240)
    dsi_setConfig(DSI_CONFIG_ID_TLPX, 1);
    dsi_setConfig(DSI_CONFIG_ID_BTA_TA_GO, 7);

    dsi_setConfig(DSI_CONFIG_ID_THS_PREPARE, 1);
    dsi_setConfig(DSI_CONFIG_ID_THS_ZERO, 4);
    dsi_setConfig(DSI_CONFIG_ID_THS_TRAIL, 3);
    dsi_setConfig(DSI_CONFIG_ID_THS_EXIT, 4);

    dsi_setConfig(DSI_CONFIG_ID_TCLK_PREPARE, 2);
    dsi_setConfig(DSI_CONFIG_ID_TCLK_ZERO, 9);
    dsi_setConfig(DSI_CONFIG_ID_TCLK_POST, 9);
    dsi_setConfig(DSI_CONFIG_ID_TCLK_PRE, 1);
    dsi_setConfig(DSI_CONFIG_ID_TCLK_TRAIL, 2);
#elif(DSI_TARGET_CLK == 120)
    dsi_setConfig(DSI_CONFIG_ID_TLPX, 0);
    dsi_setConfig(DSI_CONFIG_ID_BTA_TA_GO, 3);

    dsi_setConfig(DSI_CONFIG_ID_THS_PREPARE, 1);
    dsi_setConfig(DSI_CONFIG_ID_THS_ZERO, 2);
    dsi_setConfig(DSI_CONFIG_ID_THS_TRAIL, 1);
    dsi_setConfig(DSI_CONFIG_ID_THS_EXIT, 1);

    dsi_setConfig(DSI_CONFIG_ID_TCLK_PREPARE, 0);
    dsi_setConfig(DSI_CONFIG_ID_TCLK_ZERO, 5);
    dsi_setConfig(DSI_CONFIG_ID_TCLK_POST, 8);
    dsi_setConfig(DSI_CONFIG_ID_TCLK_PRE, 1);
    dsi_setConfig(DSI_CONFIG_ID_TCLK_TRAIL, 1);
#endif
#if (HX8369B_CFG_LANE == HX8369B_CFG_1_LANE)
    dsi_setConfig(DSI_CONFIG_ID_DATALANE_NO, DSI_DATA_LANE_0);
#else
    dsi_setConfig(DSI_CONFIG_ID_DATALANE_NO, DSI_DATA_LANE_1);
#endif
    dsi_setConfig(DSI_CONFIG_ID_TE_BTA_INTERVAL, 0x1F);
    //dsi_setConfig(DSI_CONFIG_ID_CLK_PHASE_OFS, 0x3);
    //dsi_setConfig(DSI_CONFIG_ID_PHASE_DELAY_ENABLE_OFS, 0x1);

    dsi_setConfig(DSI_CONFIG_ID_CLK_LP_CTRL, 0x1);
    dsi_setConfig(DSI_CONFIG_ID_SYNC_DLY_CNT, 0xF);

    dsi_setConfig(DSI_CONFIG_ID_EOT_PKT_EN, 1);
}

