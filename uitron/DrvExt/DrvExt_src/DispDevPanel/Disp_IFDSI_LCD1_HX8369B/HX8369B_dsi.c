/*
    Display object for driving DSI device

    @file       HX8369B_dsi.c
    @ingroup
    @note       This panel MUST select IDE clock to PLL1 ( 480 ). Once change to \n
                another frequence, the _IDE_fDCLK should be re calculated

    Copyright   Novatek Microelectronics Corp. 2011.  All rights reserved.
*/

#include "display.h"
#include "dispdev_IFdsi.h"
#include "dispdev_panlcomn.h"

// Default debug level
#ifndef __DBGLVL__
#define __DBGLVL__  2       // Output all message by default. __DBGLVL__ will be set to 1 via make parameter when release code.
#endif

// Default debug filter
#ifndef __DBGFLT__
#define __DBGFLT__  "*"     // Display everything when debug level is 2
#endif

#include "DebugModule.h"

#define PANEL_WIDTH     480
#define PANEL_HEIGHT    800



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

#define DSI_PACKET_FORMAT          DSI_FORMAT_RGB888



#define DSI_TARGET_CLK             240


#if(DSI_PACKET_FORMAT == DSI_FORMAT_RGB888)
#define _IDE_fDCLK      24.00f// must > ((((DSI_TARGET_CLK / 8) * 2) / 3)) ->2: lane number
#else
#error "Format not RGB888"
#endif

#define HVALIDST    0x39  //uiHSyncBackPorch(HPB) -> 53
#define VVALIDST    0x15  //uiVSyncBackPorchOdd/Even
#define HSYNCT      0x02
#define VSYNCT      0x02

#define ROTATE_180DEGREE      DISABLE//ENABLE//
/*
    Panel Parameters for TCON HX8369B
*/
//@{
/*Used in DSI*/
const tPANEL_CMD tCmdModeDSI[] = {
#if 0
	{DSICMD_CMD,    0x01},   // SW reset
	{CMDDELAY_MS,   50},

	{DSICMD_CMD,    0xB9},     //Set EXTC
	{DSICMD_DATA,   0xFF},
	{DSICMD_DATA,   0x83},
	{DSICMD_DATA,   0x69},


	{DSICMD_CMD,    0x3A},     // Interface pixel format
	{DSICMD_DATA,   0x77},     // x101, [0x07]: 24 bits / pixel RGB888

	{DSICMD_CMD,    0xD5},     // Set GIP timing control
	{DSICMD_DATA,   0x00},
	{DSICMD_DATA,   0x00},
	{DSICMD_DATA,   0x13},
	{DSICMD_DATA,   0x03},
	{DSICMD_DATA,   0x35},
	{DSICMD_DATA,   0x00},
	{DSICMD_DATA,   0x01},
	{DSICMD_DATA,   0x10},
	{DSICMD_DATA,   0x01},
	{DSICMD_DATA,   0x00},
	{DSICMD_DATA,   0x00},
	{DSICMD_DATA,   0x00},
	{DSICMD_DATA,   0x01},
	{DSICMD_DATA,   0x7A},
	{DSICMD_DATA,   0x16},
	{DSICMD_DATA,   0x04},
	{DSICMD_DATA,   0x04},
	{DSICMD_DATA,   0x13},
	{DSICMD_DATA,   0x07},
	{DSICMD_DATA,   0x40},
	{DSICMD_DATA,   0x13},
	{DSICMD_DATA,   0x00},
	{DSICMD_DATA,   0x00},
	{DSICMD_DATA,   0x00},
	{DSICMD_DATA,   0x20},
	{DSICMD_DATA,   0x10},
	{DSICMD_DATA,   0x00},
	{DSICMD_DATA,   0x00},
	{DSICMD_DATA,   0x00},
	{DSICMD_DATA,   0x00},
	{DSICMD_DATA,   0x00},
	{DSICMD_DATA,   0x00},
	{DSICMD_DATA,   0x00},
	{DSICMD_DATA,   0x00},
	{DSICMD_DATA,   0x00},
	{DSICMD_DATA,   0x36},
	{DSICMD_DATA,   0x00},
	{DSICMD_DATA,   0x00},
	{DSICMD_DATA,   0x48},
	{DSICMD_DATA,   0x88},
	{DSICMD_DATA,   0x85},
	{DSICMD_DATA,   0x42},
	{DSICMD_DATA,   0x00},
	{DSICMD_DATA,   0x88},
	{DSICMD_DATA,   0x88},
	{DSICMD_DATA,   0x00},
	{DSICMD_DATA,   0x00},
	{DSICMD_DATA,   0x18},
	{DSICMD_DATA,   0x88},
	{DSICMD_DATA,   0x86},
	{DSICMD_DATA,   0x71},
	{DSICMD_DATA,   0x35},
	{DSICMD_DATA,   0x88},
	{DSICMD_DATA,   0x88},
	{DSICMD_DATA,   0x00},
	{DSICMD_DATA,   0x00},
	{DSICMD_DATA,   0x58},
	{DSICMD_DATA,   0x88},
	{DSICMD_DATA,   0x87},
	{DSICMD_DATA,   0x63},
	{DSICMD_DATA,   0x11},
	{DSICMD_DATA,   0x88},
	{DSICMD_DATA,   0x88},
	{DSICMD_DATA,   0x00},
	{DSICMD_DATA,   0x00},
	{DSICMD_DATA,   0x08},
	{DSICMD_DATA,   0x88},
	{DSICMD_DATA,   0x84},
	{DSICMD_DATA,   0x50},
	{DSICMD_DATA,   0x24},
	{DSICMD_DATA,   0x88},
	{DSICMD_DATA,   0x88},
	{DSICMD_DATA,   0x00},
	{DSICMD_DATA,   0x00},
	{DSICMD_DATA,   0x00},
	{DSICMD_DATA,   0x51},
	{DSICMD_DATA,   0x00},
	{DSICMD_DATA,   0x00},
	{DSICMD_DATA,   0x00},
	{DSICMD_DATA,   0x00},
	{DSICMD_DATA,   0x00},
	{DSICMD_DATA,   0x0F},
	{DSICMD_DATA,   0x00},
	{DSICMD_DATA,   0x0F},
	{DSICMD_DATA,   0x00},
	{DSICMD_DATA,   0x00},
	{DSICMD_DATA,   0x0F},
	{DSICMD_DATA,   0x00},
	{DSICMD_DATA,   0x0F},
	{DSICMD_DATA,   0x00},
	{DSICMD_DATA,   0x01},
	{DSICMD_DATA,   0x5A},


	{DSICMD_CMD,    0xBA},     // Set MIPI Characteristic

//#if (HX8369B_CFG_LANE == HX8369B_CFG_1_LANE)
//  {DSICMD_DATA,   0x30},     // 0x30:1 lane, 0x31: 2 lane
//#else
	{DSICMD_DATA,   0x31},     // 0x30:1 lane, 0x31: 2 lane
//#endif

	{DSICMD_DATA,   0x00},
	{DSICMD_DATA,   0x16},
	{DSICMD_DATA,   0xCA},
//  {DSICMD_DATA,   0x91},     //bit[5]=0, disable EoPT detect
	{DSICMD_DATA,   0xB1},     //bit[5]=1,  enable EoPT detect
	{DSICMD_DATA,   0x0A},
	{DSICMD_DATA,   0x00},
	{DSICMD_DATA,   0x10},
	{DSICMD_DATA,   0x28},
	{DSICMD_DATA,   0x02},
	{DSICMD_DATA,   0x21},
	{DSICMD_DATA,   0x21},
	{DSICMD_DATA,   0x9A},
	{DSICMD_DATA,   0x1A},
	{DSICMD_DATA,   0x8F},

	{DSICMD_CMD,    0xB1},     // set Power
	{DSICMD_DATA,   0x09},
	{DSICMD_DATA,   0x83},
	{DSICMD_DATA,   0x67},
	{DSICMD_DATA,   0x00},
	{DSICMD_DATA,   0x92},
	{DSICMD_DATA,   0x12},
	{DSICMD_DATA,   0x16},
	{DSICMD_DATA,   0x16},
	{DSICMD_DATA,   0x0C},
	{DSICMD_DATA,   0x02},


	{DSICMD_CMD,    0xB2},
	{DSICMD_DATA,   0x00},
	{DSICMD_DATA,   0x70},

	{DSICMD_CMD,    0xB3},     // set RGB display
	{DSICMD_DATA,   0x03},     // RES_SEL[4..0]
	//      [0x00] = 480 x 1024
	//      [0x01] = 480 x 864
	//      [0x02] = 480 x 854
	//      [0x03] = 480 x 800
	//      [0x04] = 480 x 720
	{DSICMD_DATA,   0x00},
	{DSICMD_DATA,   0x31},
	{DSICMD_DATA,   0x03},
	{DSICMD_DATA,   0x01},
	{DSICMD_DATA,   0x15},
	{DSICMD_DATA,   0x14},     //  RCM = 0, VS+HS+DE ,Mode



	{DSICMD_CMD,    0xB4},     //  Set Display Inversion
	{DSICMD_DATA,   0x02},     //  N_NW[2:0]=・010・: Normal mode 2-dot inversion
	//  I_NW[2:0]=・000・: Idle mode column inversion

	{DSICMD_CMD,    0xB5},     // set Vref
	{DSICMD_DATA,   0x0B},
	{DSICMD_DATA,   0x0B},
	{DSICMD_DATA,   0x24},

	{DSICMD_CMD,    0xB6},     // set VCOM
	{DSICMD_DATA,   0x7C},
	{DSICMD_DATA,   0x7C},
	{DSICMD_DATA,   0x00},

	{DSICMD_CMD,    0xC0},     // Set Source Driving Ability
	{DSICMD_DATA,   0x73},
	{DSICMD_DATA,   0x50},
	{DSICMD_DATA,   0x00},
	{DSICMD_DATA,   0x34},
	{DSICMD_DATA,   0xC4},
	{DSICMD_DATA,   0x02},

	{DSICMD_CMD,    0xC1},     // Set DGC
	{DSICMD_DATA,   0x03},
	{DSICMD_DATA,   0x00},
	{DSICMD_DATA,   0x09},
	{DSICMD_DATA,   0x11},
	{DSICMD_DATA,   0x1A},
	{DSICMD_DATA,   0x21},
	{DSICMD_DATA,   0x29},
	{DSICMD_DATA,   0x31},
	{DSICMD_DATA,   0x38},
	{DSICMD_DATA,   0x40},
	{DSICMD_DATA,   0x48},
	{DSICMD_DATA,   0x50},
	{DSICMD_DATA,   0x58},
	{DSICMD_DATA,   0x61},
	{DSICMD_DATA,   0x69},
	{DSICMD_DATA,   0x70},
	{DSICMD_DATA,   0x78},
	{DSICMD_DATA,   0x80},
	{DSICMD_DATA,   0x88},
	{DSICMD_DATA,   0x90},
	{DSICMD_DATA,   0x8B},
	{DSICMD_DATA,   0x9A},
	{DSICMD_DATA,   0xA1},
	{DSICMD_DATA,   0xAA},
	{DSICMD_DATA,   0xB3},
	{DSICMD_DATA,   0xBA},
	{DSICMD_DATA,   0xC2},
	{DSICMD_DATA,   0xCB},
	{DSICMD_DATA,   0xD2},
	{DSICMD_DATA,   0xDA},
	{DSICMD_DATA,   0xE3},
	{DSICMD_DATA,   0xE9},
	{DSICMD_DATA,   0xF1},
	{DSICMD_DATA,   0xF8},
	{DSICMD_DATA,   0xF7},
	{DSICMD_DATA,   0xFF},
	{DSICMD_DATA,   0x00},
	{DSICMD_DATA,   0x00},
	{DSICMD_DATA,   0x00},
	{DSICMD_DATA,   0x00},
	{DSICMD_DATA,   0x00},
	{DSICMD_DATA,   0x00},
	{DSICMD_DATA,   0x00},
	{DSICMD_DATA,   0x00},
	{DSICMD_DATA,   0x00},
	{DSICMD_DATA,   0x00},
	{DSICMD_DATA,   0x09},
	{DSICMD_DATA,   0x11},
	{DSICMD_DATA,   0x1A},
	{DSICMD_DATA,   0x21},
	{DSICMD_DATA,   0x29},
	{DSICMD_DATA,   0x31},
	{DSICMD_DATA,   0x38},
	{DSICMD_DATA,   0x40},
	{DSICMD_DATA,   0x48},
	{DSICMD_DATA,   0x50},
	{DSICMD_DATA,   0x58},
	{DSICMD_DATA,   0x61},
	{DSICMD_DATA,   0x69},
	{DSICMD_DATA,   0x70},
	{DSICMD_DATA,   0x78},
	{DSICMD_DATA,   0x80},
	{DSICMD_DATA,   0x88},
	{DSICMD_DATA,   0x90},
	{DSICMD_DATA,   0x9A},
	{DSICMD_DATA,   0xA1},
	{DSICMD_DATA,   0xAA},
	{DSICMD_DATA,   0xB3},
	{DSICMD_DATA,   0xBA},
	{DSICMD_DATA,   0xC2},
	{DSICMD_DATA,   0xCB},
	{DSICMD_DATA,   0xD2},
	{DSICMD_DATA,   0xDA},
	{DSICMD_DATA,   0xE3},
	{DSICMD_DATA,   0xE9},
	{DSICMD_DATA,   0xF1},
	{DSICMD_DATA,   0xF8},
	{DSICMD_DATA,   0xFF},
	{DSICMD_DATA,   0x00},
	{DSICMD_DATA,   0x00},
	{DSICMD_DATA,   0x00},
	{DSICMD_DATA,   0x00},
	{DSICMD_DATA,   0x00},
	{DSICMD_DATA,   0x00},
	{DSICMD_DATA,   0x00},
	{DSICMD_DATA,   0x00},
	{DSICMD_DATA,   0x00},
	{DSICMD_DATA,   0x00},
	{DSICMD_DATA,   0x09},
	{DSICMD_DATA,   0x11},
	{DSICMD_DATA,   0x1A},
	{DSICMD_DATA,   0x21},
	{DSICMD_DATA,   0x29},
	{DSICMD_DATA,   0x31},
	{DSICMD_DATA,   0x38},
	{DSICMD_DATA,   0x40},
	{DSICMD_DATA,   0x48},
	{DSICMD_DATA,   0x50},
	{DSICMD_DATA,   0x58},
	{DSICMD_DATA,   0x61},
	{DSICMD_DATA,   0x69},
	{DSICMD_DATA,   0x70},
	{DSICMD_DATA,   0x78},
	{DSICMD_DATA,   0x80},
	{DSICMD_DATA,   0x88},
	{DSICMD_DATA,   0x90},
	{DSICMD_DATA,   0x9A},
	{DSICMD_DATA,   0xA1},
	{DSICMD_DATA,   0xAA},
	{DSICMD_DATA,   0xB3},
	{DSICMD_DATA,   0xBA},
	{DSICMD_DATA,   0xC2},
	{DSICMD_DATA,   0xCB},
	{DSICMD_DATA,   0xD2},
	{DSICMD_DATA,   0xDA},
	{DSICMD_DATA,   0xE3},
	{DSICMD_DATA,   0xE9},
	{DSICMD_DATA,   0xF1},
	{DSICMD_DATA,   0xF8},
	{DSICMD_DATA,   0xFF},
	{DSICMD_DATA,   0x00},
	{DSICMD_DATA,   0x00},
	{DSICMD_DATA,   0x00},
	{DSICMD_DATA,   0x00},
	{DSICMD_DATA,   0x00},
	{DSICMD_DATA,   0x00},
	{DSICMD_DATA,   0x00},
	{DSICMD_DATA,   0x00},
	{DSICMD_DATA,   0x00},

	{DSICMD_CMD,    0xC6},     // Set ECO
	{DSICMD_DATA,   0x41},
	{DSICMD_DATA,   0xFF},
	{DSICMD_DATA,   0x7A},
	//bit[0]BGR_PANEL : The color filter order direction selected.
	//  0 : S1:S2:S3=R:G:B
	//  1 : S1:S2:S3=B:G:R
	//bit[1]REV_PANEL :  The source output data polarity selected.
	//  0: normally white panel.
	//  1: normally black panel.
	//bit[2]GS_PANEL : The gate driver output shift direction selected.
	//  0: G1G1024
	//  1: G1024G1
	//bit[3]SS_PANEL :The source driver output shift direction selected.
	//  0: S1S1440
	//  1: S1440S1
	{DSICMD_CMD,    0xCC},     //  Set Panel
	{DSICMD_DATA,   0x02},     //  REV_PANEL = 1                   => GIP Forward Scan
//  {DSICMD_DATA,   0x00},     //  GS_PANEL = 1, REV_PANEL = 1    => GIP backward Scan


	{DSICMD_CMD,    0xE3},     // set  EQ
	{DSICMD_DATA,   0x00},
	{DSICMD_DATA,   0x00},
	{DSICMD_DATA,   0x13},
	{DSICMD_DATA,   0x1B},

	{DSICMD_CMD,    0xEA},     //  Set MeSSI
	{DSICMD_DATA,   0x62},     //  CABC_PWM1_OE = 0,   CABC_PWM1_OUT output enable
	//  CABC_PWM2_OE = 0,   CABC_PWM2_OUT output enable
	//  VCOM_PWR =  1, VCOM power source comes from VSN

	{DSICMD_CMD,    0xE0},    //  Set Gamma
	{DSICMD_DATA,   0x00},
	{DSICMD_DATA,   0x05},
	{DSICMD_DATA,   0x0B},
	{DSICMD_DATA,   0x2F},
	{DSICMD_DATA,   0x2F},
	{DSICMD_DATA,   0x30},
	{DSICMD_DATA,   0x1B},
	{DSICMD_DATA,   0x3D},
	{DSICMD_DATA,   0x07},
	{DSICMD_DATA,   0x0D},
	{DSICMD_DATA,   0x0E},
	{DSICMD_DATA,   0x12},
	{DSICMD_DATA,   0x13},
	{DSICMD_DATA,   0x12},
	{DSICMD_DATA,   0x13},
	{DSICMD_DATA,   0x11},
	{DSICMD_DATA,   0x1A},
	{DSICMD_DATA,   0x00},
	{DSICMD_DATA,   0x05},
	{DSICMD_DATA,   0x0B},
	{DSICMD_DATA,   0x2F},
	{DSICMD_DATA,   0x2F},
	{DSICMD_DATA,   0x30},
	{DSICMD_DATA,   0x1B},
	{DSICMD_DATA,   0x3D},
	{DSICMD_DATA,   0x07},
	{DSICMD_DATA,   0x0D},
	{DSICMD_DATA,   0x0E},
	{DSICMD_DATA,   0x12},
	{DSICMD_DATA,   0x13},
	{DSICMD_DATA,   0x12},
	{DSICMD_DATA,   0x13},
	{DSICMD_DATA,   0x11},
	{DSICMD_DATA,   0x1A},
	{DSICMD_DATA,   0x01},

	{DSICMD_CMD,    0x11},     // sleep out
	{CMDDELAY_MS,   120},


	{DSICMD_CMD,    0x29},     // display on
	{CMDDELAY_MS,   10},
	{CMDDELAY_MS,   10},
	{CMDDELAY_MS,   10},
	{CMDDELAY_MS,   10},
#else

	{DSICMD_CMD,    0x01},   // SW reset
	{CMDDELAY_MS,   50},
	//
	{DSICMD_CMD,    0xB9},//B9 set extension command
	{DSICMD_DATA,   0xFF},
	{DSICMD_DATA,   0x83},
	{DSICMD_DATA,   0x69},
#if(ROTATE_180DEGREE==ENABLE)
	{DSICMD_CMD,    0x0B},
	{DSICMD_DATA,   0x00},
	{DSICMD_DATA,   0x80},  //0x80

	{DSICMD_CMD,    0x36},//
	{DSICMD_DATA,   0x80},//0x60},  //0x40 rotate
#else
	{DSICMD_CMD,    0x36},//
	{DSICMD_DATA,   0x40},//0x60},
#endif
	//
	{DSICMD_CMD,    0xBA},
	{DSICMD_DATA,   0x31},
	{DSICMD_DATA,   0x00},
	{DSICMD_DATA,   0x16},
	{DSICMD_DATA,   0xCA},
	{DSICMD_DATA,   0xB1},
	{DSICMD_DATA,   0x0A},
	{DSICMD_DATA,   0x00},
	{DSICMD_DATA,   0x10},
	{DSICMD_DATA,   0x28},
	{DSICMD_DATA,   0x02},
	{DSICMD_DATA,   0x21},
	{DSICMD_DATA,   0x21},
	{DSICMD_DATA,   0x9A},
	{DSICMD_DATA,   0x1A},
	{DSICMD_DATA,   0x8F},

	{DSICMD_CMD,    0x3A},      //
	{DSICMD_DATA,   0x70},      //  70

	//
	{DSICMD_CMD,    0xD5},      //GIP
	{DSICMD_DATA,   0x00},      //1   PANSEL
	{DSICMD_DATA,   0x00},      //2   SHR_0[11:8]
	{DSICMD_DATA,   0x08},      //3   SHR_0[7:0]
	{DSICMD_DATA,   0x03},      //4   SHR_1[11:8]
	{DSICMD_DATA,   0x30},      //5   SHR_1[7:0]
	{DSICMD_DATA,   0x00},      //6   SHR_2[11:8]
	{DSICMD_DATA,   0x00},      //7   SHR_2[7:0]
	{DSICMD_DATA,   0x10},      //8   SHR0_1[3:0], SHR0_2[3:0]
	{DSICMD_DATA,   0x01},      //9   SHR0_3[3:0], SHR1_1[3:0]
	{DSICMD_DATA,   0x00},      //10  SHR1_2[3:0], SHR1_3[3:0]

	{DSICMD_DATA,   0x00},      //11  SHR2_1[3:0], SHR2_2[3:0]
	{DSICMD_DATA,   0x00},      //12  SHR2_3[3:0]
	{DSICMD_DATA,   0x01},      //13  SPON[7:0] //D
	{DSICMD_DATA,   0x39},      //14  SPOFF[7:0] //60
	{DSICMD_DATA,   0x45},      //15  SHP[3:0], SCP[3:0]
	{DSICMD_DATA,   0x00},      //16  GTO[5:0]
	{DSICMD_DATA,   0x00},      //17  GNO[7:0]
	{DSICMD_DATA,   0x0C},      //18  CHR[7:0]
	{DSICMD_DATA,   0x44},      //19  CON[7:0]  //50
	{DSICMD_DATA,   0x39},      //20  COFF[7:0] //45

	{DSICMD_DATA,   0x47},      //21  CHP[3:0], CCP[3:0]
	{DSICMD_DATA,   0x05},      //22  EQ_DELAY[7:0]
	{DSICMD_DATA,   0x00},      //23  EQ_DELAY_HSYNC[7:0]
	{DSICMD_DATA,   0x02},      //24  CGTS[18:16]
	{DSICMD_DATA,   0x04},      //25  CGTS[15:8]
	{DSICMD_DATA,   0x00},      //26  CGTS[7:0]
	{DSICMD_DATA,   0x00},      //27  CGTS_INV[18:16]
	{DSICMD_DATA,   0x00},      //28  CGTS_INV[15:8]
	{DSICMD_DATA,   0x00},      //29  CGTS_INV[7:0]
	{DSICMD_DATA,   0x00},      //30  CGTS_ZERO_1ST_FRAME[18:16]
	{DSICMD_DATA,   0x00},      //31  CGTS_ZERO_1ST_FRAME[15:8]
	{DSICMD_DATA,   0x00},      //32  CGTS_ZERO_1ST_FRAME[7:0]
	{DSICMD_DATA,   0x03},      //33
	{DSICMD_DATA,   0x00},      //34
	{DSICMD_DATA,   0x00},      //35
	{DSICMD_DATA,   0x00},      //36  USER_GIP_GATE[7:0]
	{DSICMD_DATA,   0x00},      //37  COS1_L_GS[3:0], COS2_L_GS[3:0]
	{DSICMD_DATA,   0x00},      //38  COS3_L_GS[3:0], COS4_L_GS[3:0]
	{DSICMD_DATA,   0x37},      //39  COS5_L_GS[3:0], COS6_L_GS[3:0]
	{DSICMD_DATA,   0x59},      //40  COS7_L_GS[3:0], COS8_L_GS[3:0]

	{DSICMD_DATA,   0x18},      //41  COS9_L_GS[3:0], COS10_L_GS[3:0]
	{DSICMD_DATA,   0x10},      //42  COS11_L_GS[3:0], COS12_L_GS[3:0]
	{DSICMD_DATA,   0x00},      //43  COS13_L_GS[3:0], COS14_L_GS[3:0]
	{DSICMD_DATA,   0x00},      //44  COS15_L_GS[3:0], COS16_L_GS[3:0]
	{DSICMD_DATA,   0x05},      //45  COS17_L_GS[3:0], COS18_L_GS[3:0]
	{DSICMD_DATA,   0x00},      //46  COS1_L[3:0], COS2_L[3:0]
	{DSICMD_DATA,   0x00},      //47  COS3_L[3:0], COS4_L[3:0]
	{DSICMD_DATA,   0x40},      //48  COS5_L[3:0], COS6_L[3:0]
	{DSICMD_DATA,   0x28},      //49  COS7_L[3:0], COS8_L[3:0]
	{DSICMD_DATA,   0x69},      //50  COS9_L[3:0], COS10_L[3:0]
	{DSICMD_DATA,   0x40},      //51  COS11_L[3:0], COS12_L[3:0]
	{DSICMD_DATA,   0x00},      //52  COS13_L[3:0], COS14_L[3:0]
	{DSICMD_DATA,   0x00},      //53  COS15_L[3:0], COS16_L[3:0]
	{DSICMD_DATA,   0x00},      //54  COS17_L[3:0], COS18_L[3:0]
	{DSICMD_DATA,   0x00},      //55  COS1_R_GS[3:0], COS2_R_GS[3:0]
	{DSICMD_DATA,   0x00},      //56  COS3_R_GS[3:0], COS4_R_GS[3:0]
	{DSICMD_DATA,   0x26},      //57  COS5_R_GS[3:0], COS6_R_GS[3:0]
#if(ROTATE_180DEGREE==ENABLE)
	{DSICMD_DATA,   0x48},      //58  COS7_R_GS[3:0], COS8_R_GS[3:0]    49
	{DSICMD_DATA,   0x09},      //59  COS9_R_GS[3:0], COS10_R_GS[3:0]   08
#else
	{DSICMD_DATA,   0x49},      //58  COS7_R_GS[3:0], COS8_R_GS[3:0]
	{DSICMD_DATA,   0x08},      //59  COS9_R_GS[3:0], COS10_R_GS[3:0]
#endif
	{DSICMD_DATA,   0x00},      //60  COS11_R_GS[3:0], COS12_R_GS[3:0]

	{DSICMD_DATA,   0x00},      //61  COS13_R_GS[3:0], COS14_R_GS[3:0]
	{DSICMD_DATA,   0x00},      //62  COS15_R_GS[3:0], COS16_R_GS[3:0]
	{DSICMD_DATA,   0x04},      //63  COS17_R_GS[3:0], COS18_R_GS[3:0]
	{DSICMD_DATA,   0x00},      //64  COS1_R[3:0], COS2_R[3:0]
	{DSICMD_DATA,   0x00},      //65  COS3_R[3:0], COS4_R[3:0]
	{DSICMD_DATA,   0x51},      //66  COS5_R[3:0], COS6_R[3:0]
#if(ROTATE_180DEGREE==ENABLE)
	{DSICMD_DATA,   0x38},      //67  COS7_R[3:0], COS8_R[3:0] 39
	{DSICMD_DATA,   0x79},      //68  COS9_R[3:0], COS10_R[3:0] 78
#else
	{DSICMD_DATA,   0x39},      //67  COS7_R[3:0], COS8_R[3:0]
	{DSICMD_DATA,   0x78},      //68  COS9_R[3:0], COS10_R[3:0]
#endif
	{DSICMD_DATA,   0x50},      //69  COS11_R[3:0], COS12_R[3:0]
	{DSICMD_DATA,   0x00},      //70  COS13_R[3:0], COS14_R[3:0]
	{DSICMD_DATA,   0x00},      //71  COS15_R[3:0], COS16_R[3:0]
	{DSICMD_DATA,   0x01},      //72  COS17_R[3:0], COS18_R[3:0]
	{DSICMD_DATA,   0x00},      //73  TCON_OPT[7:0]
	{DSICMD_DATA,   0x00},      //74  GIP_OPT[22:16]
	{DSICMD_DATA,   0x00},      //75  GIP_OPT[15:8]
	{DSICMD_DATA,   0x01},      //76  GIP_OPT[7:0]
	{DSICMD_DATA,   0x00},      //77
	{DSICMD_DATA,   0x00},      //78
	{DSICMD_DATA,   0x00},      //79
	{DSICMD_DATA,   0x07},      //80  HZ_L[17:16], HZ_UL[17:16]

	{DSICMD_DATA,   0xF8},      //81  HZ_L[15:8]
	{DSICMD_DATA,   0x0F},      //82  HZ_L[7:0]
	{DSICMD_DATA,   0xFF},      //83  HZ_UL[15:8]
	{DSICMD_DATA,   0xFF},      //84  HZ_UL[7:0]
	{DSICMD_DATA,   0x07},      //85  HZ_R[17:16], HZ_UR[17:16]
	{DSICMD_DATA,   0xF8},      //86  HZ_R[15:8]
	{DSICMD_DATA,   0x0F},      //87  HZ_R[7:0]
	{DSICMD_DATA,   0xFF},      //88  HZ_UR[15:8]
	{DSICMD_DATA,   0xFF},      //89  HZ_UR[7:0]
	{DSICMD_DATA,   0x00},      //90  EQ_ENB_B,EQ_ENB_F, HSYNC_TO_CL1_CNT9[8]
	{DSICMD_DATA,   0x20},      //91  HSYNC_TO_CL1_CNT9[7:0]
	{DSICMD_DATA,   0x5A},      //92  COFF_TCON[7:0]

	{DSICMD_CMD,    0xB1},//B1 set power
#if 0
	{DSICMD_DATA,   0x12},
	{DSICMD_DATA,   0x83},
	{DSICMD_DATA,   0x77},
	{DSICMD_DATA,   0x00},
	{DSICMD_DATA,   0x10},
	{DSICMD_DATA,   0x10},
	{DSICMD_DATA,   0x1E},
	{DSICMD_DATA,   0x1E},
	{DSICMD_DATA,   0x0C},
	{DSICMD_DATA,   0x1A},
#else
	{DSICMD_DATA,   0x13},// 0x13   //0x14
	{DSICMD_DATA,   0x83},
	{DSICMD_DATA,   0x77},
	{DSICMD_DATA,   0x00},
	{DSICMD_DATA,   0x10},
	{DSICMD_DATA,   0x10},
	{DSICMD_DATA,   0x18},  //0x1E
	{DSICMD_DATA,   0x18},
	{DSICMD_DATA,   0x0C},
	{DSICMD_DATA,   0x2c},//0x2c},  // 0x1A
#endif
	{DSICMD_DATA,   0x20},
	{DSICMD_DATA,   0xD6},

	//
	{DSICMD_CMD,    0xB3},
	{DSICMD_DATA,   0x83},
	{DSICMD_DATA,   0x00},
	{DSICMD_DATA,   0x3A},
	{DSICMD_DATA,   0x17},
	//
	{DSICMD_CMD,    0xB4},//B4 set display waveform cycle
	{DSICMD_DATA,   0x02},// 2-dot inversion

	//
	{DSICMD_CMD,    0xB6},//B6 set vcom voltage
	{DSICMD_DATA,   0xA4},//42
	{DSICMD_DATA,   0xA4},//42

	//
	{DSICMD_CMD,    0xCC},  //Set panel
	{DSICMD_DATA,   0x06},  // NB panel     //0x06

	//
	{DSICMD_CMD,    0xC6}, // Set ECO
	{DSICMD_DATA,   0x41},
	{DSICMD_DATA,   0xFF},
	{DSICMD_DATA,   0x7D},

	//
	{DSICMD_CMD,    0xEA}, //Set MESSI
	{DSICMD_DATA,   0x7A},
//************************************************


	//
	{DSICMD_CMD,    0xE3},
	{DSICMD_DATA,   0x0F},
	{DSICMD_DATA,   0x0F},
	{DSICMD_DATA,   0x11},
	{DSICMD_DATA,   0x11},
	//
	{DSICMD_CMD,    0xC0},
	{DSICMD_DATA,   0x73},
	{DSICMD_DATA,   0x50},
	{DSICMD_DATA,   0x00},
	{DSICMD_DATA,   0x20},
	{DSICMD_DATA,   0xC4},
	{DSICMD_DATA,   0x00},
	//
	{DSICMD_CMD,    0xE0},
	{DSICMD_DATA,   0x00},  //1
	{DSICMD_DATA,   0x18},
	{DSICMD_DATA,   0x1E},
	{DSICMD_DATA,   0x36},
	{DSICMD_DATA,   0x3E},  //5
	{DSICMD_DATA,   0x3F},
	{DSICMD_DATA,   0x2C},
	{DSICMD_DATA,   0x42},
	{DSICMD_DATA,   0x09},
	{DSICMD_DATA,   0x0E},  //10
	{DSICMD_DATA,   0x0F},
	{DSICMD_DATA,   0x14},
	{DSICMD_DATA,   0x16},
	{DSICMD_DATA,   0x14},
	{DSICMD_DATA,   0x15},  //15
	{DSICMD_DATA,   0x12},
	{DSICMD_DATA,   0x17},

	{DSICMD_DATA,   0x00},  //1
	{DSICMD_DATA,   0x18},
	{DSICMD_DATA,   0x1E},
	{DSICMD_DATA,   0x36},
	{DSICMD_DATA,   0x3E},  //5
	{DSICMD_DATA,   0x3F},
	{DSICMD_DATA,   0x2C},
	{DSICMD_DATA,   0x42},
	{DSICMD_DATA,   0x09},
	{DSICMD_DATA,   0x0E},  //10
	{DSICMD_DATA,   0x0F},
	{DSICMD_DATA,   0x14},
	{DSICMD_DATA,   0x16},
	{DSICMD_DATA,   0x14},
	{DSICMD_DATA,   0x15},  //15
	{DSICMD_DATA,   0x12},
	{DSICMD_DATA,   0x17},
	{DSICMD_DATA,   0x01},

	//
	{DSICMD_CMD,    0x11}, //Sleep Out
	{CMDDELAY_MS,   150},
	//
	{DSICMD_CMD,    0x29}, //Display On
	{CMDDELAY_MS,   150},

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
			PINMUX_DSI_2_LANE_VDO_SYNC_EVENT_RGB888,
			_IDE_fDCLK,                     //!< fDCLK
			(0xC0 + PANEL_WIDTH),           //!< uiHSyncTotalPeriod(HTOTAL)
			PANEL_WIDTH,                    //!< uiHSyncActivePeriod(HACT)
			0x60,                           //!< uiHSyncBackPorch(HBP)
			0x52 + PANEL_HEIGHT,            //!< uiVSyncTotalPeriod(VTOTAL)
			PANEL_HEIGHT,                   //!< uiVSyncActivePeriod
			VVALIDST,                       //!< uiVSyncBackPorchOdd
			VVALIDST,                       //!< uiVSyncBackPorchEven
			PANEL_HEIGHT,                   //!< uiBufferWidth
			PANEL_WIDTH,                    //!< uiBufferHeight
			PANEL_WIDTH,                    //!< uiWindowWidth
			PANEL_HEIGHT,                   //!< uiWindowHeight
			FALSE,                          //!< bYCbCrFormat
			/* New added parameters */
			HSYNCT,                         //!< uiHSyncSYNCwidth
			HSYNCT                          //!< uiVSyncSYNCwidth
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
	dsi_setConfig(DSI_CONFIG_ID_FREQ, DSI_TARGET_CLK * 1000000);

	/*#if(DSI_TARGET_CLK == 480)
	    dsi_setConfig(DSI_CONFIG_ID_TLPX, 3);
	    dsi_setConfig(DSI_CONFIG_ID_BTA_TA_GO, 0x15);
	    dsi_setConfig(DSI_CONFIG_ID_BTA_TA_SURE, 0);
	    dsi_setConfig(DSI_CONFIG_ID_BTA_TA_GET, 0x14);

	    dsi_setConfig(DSI_CONFIG_ID_THS_PREPARE, 4);
	    dsi_setConfig(DSI_CONFIG_ID_THS_ZERO, 6);
	    dsi_setConfig(DSI_CONFIG_ID_THS_TRAIL, 7);
	    dsi_setConfig(DSI_CONFIG_ID_THS_EXIT, 6);

	    dsi_setConfig(DSI_CONFIG_ID_TCLK_PREPARE, 2);
	    dsi_setConfig(DSI_CONFIG_ID_TCLK_ZERO, 16);
	    dsi_setConfig(DSI_CONFIG_ID_TCLK_POST, 16);
	    dsi_setConfig(DSI_CONFIG_ID_TCLK_PRE, 2);
	    dsi_setConfig(DSI_CONFIG_ID_TCLK_TRAIL, 3);

	    dsi_setConfig(DSI_CONFIG_ID_CLK_PHASE_OFS, 0x4);
	    dsi_setConfig(DSI_CONFIG_ID_PHASE_DELAY_ENABLE_OFS, 0x1);
	#elif(DSI_TARGET_CLK == 240)
	*/
	dsi_setConfig(DSI_CONFIG_ID_TLPX, 3);
	dsi_setConfig(DSI_CONFIG_ID_BTA_TA_GO, 0x15);
	dsi_setConfig(DSI_CONFIG_ID_BTA_TA_SURE, 0);
	dsi_setConfig(DSI_CONFIG_ID_BTA_TA_GET, 0x14);

	dsi_setConfig(DSI_CONFIG_ID_THS_PREPARE, 4);//4
	dsi_setConfig(DSI_CONFIG_ID_THS_ZERO, 6);
	dsi_setConfig(DSI_CONFIG_ID_THS_TRAIL, 7);
	dsi_setConfig(DSI_CONFIG_ID_THS_EXIT, 6);

	dsi_setConfig(DSI_CONFIG_ID_TCLK_PREPARE, 3);
	dsi_setConfig(DSI_CONFIG_ID_TCLK_ZERO, 16);
	dsi_setConfig(DSI_CONFIG_ID_TCLK_POST, 16);
	dsi_setConfig(DSI_CONFIG_ID_TCLK_PRE, 2);
	dsi_setConfig(DSI_CONFIG_ID_TCLK_TRAIL, 3);

	dsi_setConfig(DSI_CONFIG_ID_CLK_PHASE_OFS, 0x7);
	dsi_setConfig(DSI_CONFIG_ID_PHASE_DELAY_ENABLE_OFS, 0x1);
	/*#endif*/

//#if (HX8369B_CFG_LANE == HX8369B_CFG_1_LANE)
//    dsi_setConfig(DSI_CONFIG_ID_DATALANE_NO, DSI_DATA_LANE_0);
//#else
	dsi_setConfig(DSI_CONFIG_ID_DATALANE_NO, DSI_DATA_LANE_1);
//#endif
	//dsi_setConfig(DSI_CONFIG_ID_TE_BTA_INTERVAL, 0x1F);

	dsi_setConfig(DSI_CONFIG_ID_CLK_LP_CTRL, 0x1);
	//dsi_setConfig(DSI_CONFIG_ID_SYNC_DLY_CNT, 0xF);

	dsi_setConfig(DSI_CONFIG_ID_EOT_PKT_EN, 0);
}
