/*
    Display object for driving DSI device

    @file       H245QBN02_dsi.c
    @ingroup
    @note       This panel MUST select IDE clock to PLL1 ( 480 ). Once change to \n
                another frequence, the _IDE_fDCLK should be re calculated

    Copyright   Novatek Microelectronics Corp. 2011.  All rights reserved.
*/

#include "display.h"
#include "dispdev_IFdsi.h"
#include "dispdev_panlcomn.h"

#define PANEL_WIDTH     240
#define PANEL_HEIGHT    432

#define H245QBN02_IND_MSG(...)       debug_msg(__VA_ARGS__)

#define H245QBN02_ERR_MSG(...)       debug_msg(__VA_ARGS__)

#define H245QBN02_WRN_MSG(...)       debug_msg(__VA_ARGS__)

#define H245QBN02_TE_OFF              0
#define H245QBN02_TE_ON               1

#define H245QBN02_TE_PACKET           0
#define H245QBN02_TE_PIN              1

/*
    RGB888 = 1 pixel = 3bytes packet
    If DSI src = 240MHz, internal clock = 30MHz, data rate = 30MHz x 1bytes = 30MB / sec per lane
    2 lane = 60MB = 20Mpixel ' IDE need > 20MHz
*/


#define DSI_FORMAT_RGB565          0    //N/A in H245QBN02
#define DSI_FORMAT_RGB666P         1    //N/A in H245QBN02
#define DSI_FORMAT_RGB666L         2    //N/A in H245QBN02
#define DSI_FORMAT_RGB888          3    //IDE use 480 & DSI use 480

#define DSI_OP_MODE_CMD_MODE       1
#define DSI_OP_MODE_VDO_MODE       0

#define DSI_PACKET_FORMAT          DSI_FORMAT_RGB888

//#define DSI_TARGET_CLK             240
#define DSI_TARGET_CLK             480
//#define DSI_TARGET_CLK             160
//#define DSI_TARGET_CLK             120
#define DSI_OP_MODE                DSI_OP_MODE_CMD_MODE

#if (DSI_PACKET_FORMAT == DSI_FORMAT_RGB666P) && (DSI_OP_MODE == DSI_OP_MODE_CMD_MODE)
#error "Command mode not support RGB666P"
#endif


#if(DSI_PACKET_FORMAT == DSI_FORMAT_RGB888) || (DSI_PACKET_FORMAT == DSI_FORMAT_RGB666L)
#if(DSI_TARGET_CLK == 160)
#define _IDE_fDCLK      6.66f
#else
#define _IDE_fDCLK      (((DSI_TARGET_CLK / 8)/3)*2)
#endif
#elif(DSI_PACKET_FORMAT == DSI_FORMAT_RGB666P)
#if(DSI_TARGET_CLK == 160)
#define _IDE_fDCLK      8.88f
#elif(DSI_TARGET_CLK == 120)
#define _IDE_fDCLK      6.66f
#else
#define _IDE_fDCLK      ((((DSI_TARGET_CLK / 8) * 4)/9))
#endif
#elif(DSI_PACKET_FORMAT == DSI_FORMAT_RGB565)
#if(DSI_TARGET_CLK == 120)
#define _IDE_fDCLK      7.5f
#else
#define _IDE_fDCLK      (((DSI_TARGET_CLK / 8)/2))
#endif
#endif




/*
    Panel Parameters for TCON H245QBN02
*/
//@{
/*Used in DSI*/
const tPANEL_CMD tCmdModeDSI[] = {
	{DSICMD_CMD,     0xF2},     // DISPCTL
	{DSICMD_DATA,    0x00},     // [01]Resolution is determine by RSEL
	{DSICMD_DATA,    0x77},     // [02]Default value
	{DSICMD_DATA,    0x03},     // [03]Default value
	{DSICMD_DATA,    0x0A},     // [04]Default value
	{DSICMD_DATA,    0x76},     // [05]Default value
	{DSICMD_DATA,    0x03},     // [06]Resolution selection
	//  [00]:240 x 240
	//  [01]:240 x 320
	//  [02]:240 x 376
	//  [03]:240 x 432

	{DSICMD_DATA,    0x00},     //  [07] TE ON/OFF
	//  [00]:TE_OFF
	//  [01]:TE_ON

	{DSICMD_DATA,    0x01},     // [08]:Default value
	{DSICMD_DATA,    0x82},     // [09]:Default value
	{DSICMD_DATA,    0x01},     // [10]:Default value
	{DSICMD_DATA,    0xBA},     // [11]:Default value
	{DSICMD_DATA,    0x57},     // [12]:Default value
	{DSICMD_DATA,    0x00},     // [13]:Default value
	{DSICMD_DATA,    0x00},     // [14]:Default value
	{DSICMD_DATA,    0x77},     // [15]:Default value
	{DSICMD_DATA,    0x0A},     // [16]:Default value
	{DSICMD_DATA,    0x76},     // [17]:Default value


	{DSICMD_CMD,     0xF4},     // PWRCTL
	{DSICMD_DATA,    0x0B},     // [01]Resolution is determine by RSEL
	{DSICMD_DATA,    0x00},     // [02]Default value
	{DSICMD_DATA,    0x00},     // [03]Default value
	{DSICMD_DATA,    0x00},     // [04]Default value
	{DSICMD_DATA,    0x21},     // [05]Default value
	{DSICMD_DATA,    0x4F},     // [06]Resolution selection
	//  [00]:240 x 240
	//  [01]:240 x 320
	//  [02]:240 x 376
	//  [03]:240 x 432

	{DSICMD_DATA,    0x01},     //  [07] TE ON/OFF
	//  [00]:TE_OFF
	//  [01]:TE_ON

	{DSICMD_DATA,    0x02},     // [08]:Default value
	{DSICMD_DATA,    0x2A},     // [09]:Default value
	{DSICMD_DATA,    0x7F},     // [10]:Default value
	{DSICMD_DATA,    0x03},     // [11]:Default value
	{DSICMD_DATA,    0x2A},     // [12]:Default value
	{DSICMD_DATA,    0x00},     // [13]:Default value
	{DSICMD_DATA,    0x03},     // [14]:Default value

	{DSICMD_CMD,     0xF5},     // VCOMCTRL
	{DSICMD_DATA,    0x00},
	{DSICMD_DATA,    0x4D},
	{DSICMD_DATA,    0x66},
	{DSICMD_DATA,    0x00},
	{DSICMD_DATA,    0x00},
	{DSICMD_DATA,    0x11},
	{DSICMD_DATA,    0x00},
	{DSICMD_DATA,    0x00},
	{DSICMD_DATA,    0x04},
	{DSICMD_DATA,    0x04},

	{DSICMD_CMD,     0xF6},     // VCOMCTRL
	{DSICMD_DATA,    0x02},
	{DSICMD_DATA,    0x01},
	{DSICMD_DATA,    0x06},
	{DSICMD_DATA,    0x00},
	{DSICMD_DATA,    0x06},
	{DSICMD_DATA,    0x04},
	{DSICMD_DATA,    0x06},
	{DSICMD_DATA,    0x74},
	{DSICMD_DATA,    0x06},

	{DSICMD_CMD,     0xF7},     // VCOMCTRL
	{DSICMD_DATA,    0x02},

	{DSICMD_CMD,     0xF8},     // VCOMCTRL
	{DSICMD_DATA,    0x33},
	{DSICMD_DATA,    0x00},

	{DSICMD_CMD,     0xF9},     // VCOMCTRL
	{DSICMD_DATA,    0x00},





	{DSICMD_CMD,     0x2A},     // DISPCTL
	{DSICMD_DATA,    0x00},     // Column address set to 240
	{DSICMD_DATA,    0x00},
	{DSICMD_DATA,    0x00},
	{DSICMD_DATA,    0xEF},

	{DSICMD_CMD,     0x2B},     // Page address set to 0x432
	{DSICMD_DATA,    0x00},     // Column address set to 240
	{DSICMD_DATA,    0x00},
	{DSICMD_DATA,    0x01},
	{DSICMD_DATA,    0xAF},

	{DSICMD_CMD,     0x11},     // sleep out
	{CMDDELAY_MS,    160},

#if (DSI_OP_MODE == DSI_OP_MODE_CMD_MODE)
	{DSICMD_CMD,     0x36},     // set memory access control
	{DSICMD_DATA,    0x00},     // bit[3] = 0, RGB filter / bit[3] = 1, BGR filter
#if (DSI_PACKET_FORMAT == DSI_FORMAT_RGB565)
#error "Not support RGB565"
#elif(DSI_PACKET_FORMAT == DSI_FORMAT_RGB666L)
#error "Not support RGB666L"
#elif(DSI_PACKET_FORMAT == DSI_FORMAT_RGB888)
	{DSICMD_CMD,     0x3A},     // Interface pixel format
	{DSICMD_DATA,    0x07},     // x101, x101 [0x55]: 16 bits / pixel

#endif


#if 1
//    {CMDTEON,        H245QBN02_TE_ON},
//  {CMDTEEXTPIN,    H245QBN02_TE_PACKET},
//    {CMDTEEXTPIN,    H245QBN02_TE_PIN},



	//{CMDTEONEACHFRAME,0x1},
	//{CMDBTA,         0x0},
	//{DSICMD_CMD,     0x35},     //TE-ON
	//{CMDBTA,         0x0},
#endif
	{DSICMD_CMD,     0x29},     // display on
	{CMDDELAY_MS,    10},
	{CMDDELAY_MS,    10},
	{CMDDELAY_MS,    10},
	{CMDDELAY_MS,    10},
#endif
};


const tPANEL_CMD tCmdStandbyDSI[] = {
	{DSICMD_CMD,     0x28},     // Display OFF
	{DSICMD_CMD,     0x10}      // Sleep in
};

const tLCD_PARAM tModeDSI[] = {
	/***********       MI Serial Format 1      *************/
	{
		// tPANEL_PARAM
		{
			/* Old prototype */
			//PINMUX_DSI_1_LANE_VDO_SYNC_EVENT_RGB666P,   //!< LCDMode
			//PINMUX_DSI_1_LANE_VDO_SYNC_PULSE_RGB666P,   //!< LCDMode
			//PINMUX_DSI_1_LANE_VDO_SYNC_EVENT_RGB666L,
			//PINMUX_DSI_1_LANE_VDO_SYNC_PULSE_RGB666L,   //!< LCDMode
			//PINMUX_DSI_1_LANE_VDO_SYNC_PULSE_RGB565,
#if (DSI_OP_MODE == DSI_OP_MODE_VDO_MODE)
#if (DSI_PACKET_FORMAT == DSI_FORMAT_RGB565)
			//PINMUX_DSI_1_LANE_VDO_SYNC_PULSE_RGB565,
			PINMUX_DSI_1_LANE_VDO_SYNC_EVENT_RGB565,
#elif (DSI_PACKET_FORMAT == DSI_FORMAT_RGB666P)
			//PINMUX_DSI_1_LANE_VDO_SYNC_EVENT_RGB666P,
			PINMUX_DSI_1_LANE_VDO_SYNC_PULSE_RGB666P,
#elif (DSI_PACKET_FORMAT == DSI_FORMAT_RGB666L)
			PINMUX_DSI_1_LANE_VDO_SYNC_EVENT_RGB666L,
			//PINMUX_DSI_1_LANE_VDO_SYNC_PULSE_RGB666L,
#elif (DSI_PACKET_FORMAT == DSI_FORMAT_RGB888)
			PINMUX_DSI_1_LANE_VDO_SYNC_EVENT_RGB888,
			//PINMUX_DSI_1_LANE_VDO_SYNC_PULSE_RGB888,
#endif

#else
#if (DSI_PACKET_FORMAT == DSI_FORMAT_RGB565)
			PINMUX_DSI_1_LANE_CMD_MODE_RGB565,
#elif (DSI_PACKET_FORMAT == DSI_FORMAT_RGB666L)
			PINMUX_DSI_1_LANE_CMD_MODE_RGB666L,
#elif (DSI_PACKET_FORMAT == DSI_FORMAT_RGB888)
			PINMUX_DSI_1_LANE_CMD_MODE_RGB888,
#endif
#endif
			_IDE_fDCLK,                                 //!< fDCLK
			(0x68 + PANEL_WIDTH),                       //!< uiHSyncTotalPeriod
			PANEL_WIDTH,                                //!< uiHSyncActivePeriod
			0x34,                                       //!< uiHSyncBackPorch
			0x20 + PANEL_HEIGHT,                         //!< uiVSyncTotalPeriod
			PANEL_HEIGHT,                               //!< uiVSyncActivePeriod
			0x10,                                       //!< uiVSyncBackPorchOdd
			0x10,                                       //!< uiVSyncBackPorchEven
			PANEL_WIDTH,                                //!< uiBufferWidth
			PANEL_HEIGHT,                               //!< uiBufferHeight
			PANEL_WIDTH,                                //!< uiWindowWidth
			PANEL_HEIGHT,                               //!< uiWindowHeight
			FALSE,                                      //!< bYCbCrFormat

			/* New added parameters */
			0x01,                                       //!< uiHSyncSYNCwidth
			0x01                                        //!< uiVSyncSYNCwidth
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
			{0x00, 0x00},                   //!< **DONT-CARE**

			/* New added parameters */
			FALSE,                          //!< YCex
			FALSE,                          //!< HLPF
			{FALSE, FALSE, FALSE},          //!< subpixOdd[3]
			{FALSE, FALSE, FALSE},          //!< subpixEven[3]
			{IDE_DITHER_5BITS, IDE_DITHER_6BITS, IDE_DITHER_5BITS}, //!< DitherBits[3]
			FALSE                           //!< clk1/2
		},

		(tPANEL_CMD *)tCmdModeDSI,                 //!< pCmdQueue
		sizeof(tCmdModeDSI) / sizeof(tPANEL_CMD),  //!< nCmd
	}
};

const tLCD_ROT *tRotDSI = NULL;

//@}

tLCD_ROT *dispdev_getLcdRotateDSICmd(UINT32 *ModeNumber)
{
#if 0
	if (tRotDSI != NULL) {
		*ModeNumber = sizeof(tRotDSI) / sizeof(tLCD_ROT);
	} else
#endif
	{
		*ModeNumber = 0;
	}
	return (tLCD_ROT *)tRotDSI;
}

tLCD_PARAM *dispdev_getConfigModeDSI(UINT32 *ModeNumber)
{
	*ModeNumber = sizeof(tModeDSI) / sizeof(tLCD_PARAM);
	return (tLCD_PARAM *)tModeDSI;
}

tPANEL_CMD *dispdev_getStandbyCmdDSI(UINT32 *CmdNumber)
{
	*CmdNumber = sizeof(tCmdStandbyDSI) / sizeof(tPANEL_CMD);
	return (tPANEL_CMD *)tCmdStandbyDSI;
}


void dispdev_setDSIConfig(tDSI_CONFIG *pDsiConfig)
{
	// DSI input source clock = 480
	// Target can be 480 / 240 / 160 / 120
#if 0
	FLOAT   DsiTargetClk = DSI_TARGET_CLK;

	UINT32  Div;



	Div = (UINT32)(pDsiConfig->fDsiSrcClk / DsiTargetClk);

	if (Div == 0) {
		H245QBN02_WRN_MSG("Div = 0 force ++\r\n");
		Div++;
	}
	pll_setClockRate(PLL_CLKSEL_DSI_CLKDIV, PLL_DSI_CLKDIV(Div - 1));
#else
	dsi_setConfig(DSI_CONFIG_ID_FREQ, DSI_TARGET_CLK * 1000000);
#endif

#if(DSI_TARGET_CLK == 160)
	dsi_setConfig(DSI_CONFIG_ID_TLPX, 3);
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
#elif(DSI_TARGET_CLK == 240)
	dsi_setConfig(DSI_CONFIG_ID_TLPX, 2);
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
	dsi_setConfig(DSI_CONFIG_ID_TLPX, 1);
	dsi_setConfig(DSI_CONFIG_ID_BTA_TA_GO, 4);
	dsi_setConfig(DSI_CONFIG_ID_THS_PREPARE, 1);
	dsi_setConfig(DSI_CONFIG_ID_THS_ZERO, 1);
	dsi_setConfig(DSI_CONFIG_ID_THS_TRAIL, 1);
	dsi_setConfig(DSI_CONFIG_ID_THS_EXIT, 1);

	dsi_setConfig(DSI_CONFIG_ID_TCLK_PREPARE, 0);
	dsi_setConfig(DSI_CONFIG_ID_TCLK_ZERO, 4);
	dsi_setConfig(DSI_CONFIG_ID_TCLK_POST, 7);
	dsi_setConfig(DSI_CONFIG_ID_TCLK_PRE, 1);
	dsi_setConfig(DSI_CONFIG_ID_TCLK_TRAIL, 1);

#endif
	dsi_setConfig(DSI_CONFIG_ID_DATALANE_NO, DSI_DATA_LANE_0);
	dsi_setConfig(DSI_CONFIG_ID_TE_BTA_INTERVAL, 0x1F);
	dsi_setConfig(DSI_CONFIG_ID_CLK_LP_CTRL, 0x1);
	dsi_setConfig(DSI_CONFIG_ID_SYNC_DLY_CNT, 0xF);
}
