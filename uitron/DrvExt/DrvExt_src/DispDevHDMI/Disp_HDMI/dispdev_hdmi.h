/*
    TV HDMI TX module internal header file

    TV HDMI TX module internal header file

    @file       dispdev_hdmi.h
    @ingroup    mIDISPHDMITx
    @note       Nothing

    Copyright   Novatek Microelectronics Corp. 2012.  All rights reserved.
*/
#ifndef _DISPDEV_HDMI_H
#define _DISPDEV_HDMI_H

#include "hdmitx.h"
#include "display.h"
#include "Type.h"

// Default debug level
#ifndef __DBGLVL__
#define __DBGLVL__  2       // Output all message by default. __DBGLVL__ will be set to 1 via make parameter when release code.
#endif

// Default debug filter
#ifndef __DBGFLT__
#define __DBGFLT__  "*"     // Display everything when debug level is 2
#endif

#include "DebugModule.h"


#define DISPHDMI_DEBUG              DISABLE
#if DISPHDMI_DEBUG
#define disphdmi_debug(msg)         DBG_WRN msg
#else
#define disphdmi_debug(msg)
#endif

#define ACT_HIGH                    0   //Signal change level at PIXCLK positive edge
#define ACT_LOW                     1   //Signal change level at PIXCLK negative edge


typedef struct {
	HDMI_VIDEOID    VID;                ///< Video ID

	BOOL            HS;                 ///< H-Sync Active edge
	BOOL            VS;                 ///< V-Sync Active edge
	BOOL            HVLD;               ///< H-Valid Active edge
	BOOL            VVLD;               ///< V-Valid Active edge
	BOOL            CLK;                ///< Pixel-Clock Active edge
	BOOL            DE;                 ///< Data Enable Active edge
	BOOL            PROGRESSIVE;        ///< Progressive/Interlaced Selection

	UINT32          H_SYNCTIME;         ///< H-Sync Time
	UINT32          H_TOTAL;            ///< H-Total Time
	UINT32          H_STARTTIME;        ///< H-Start Time
	UINT32          H_ENDTIME;          ///< H-End Time

	UINT32          V_SYNCTIME;         ///< V-Sync Time
	UINT32          V_TOTAL;            ///< V-Total Time
	UINT32          V_ODD_STARTTIME;    ///< V-Odd field Start Time
	UINT32          V_ODD_ENDTIME;      ///< V-Odd field End Time
	UINT32          V_EVEN_STARTTIME;   ///< V-Even field Start Time
	UINT32          V_EVEN_ENDTIME;     ///< V-Even field End Time

	UINT32          WINX_OFS;           ///< IDE Window Offset X when TV overscan
	UINT32          WINY_OFS;           ///< IDE Window Offset Y when TV overscan
} DISPDEV_HDMI_TIMING, *PDISPDEV_HDMI_TIMING;


typedef enum {
	HDMI_MODE_RGB444 = 0, ///<    HDMI Output format is RGB-444
	HDMI_MODE_YCBCR422,  ///<    HDMI Output format is YCbCr-422
	HDMI_MODE_YCBCR444,  ///<    HDMI Output format is YCbCr-444

	ENUM_DUMMY4WORD(HDMI_MODE)
} HDMI_MODE;



#define HDMI_PLL216     0x240000 // 216*131072/12 For 13.5/27/54/108MHz
#define HDMI_PLL252     0x2A0000 // 252*131072/12 For 25.2MHz
#define HDMI_PLL297     0x318000 // 297*131072/12 For 74.25MHz


#if _FPGA_EMULATION_
// APIs for FPGA Verification Usage Only.
extern void hdmitx_setPgEnable(BOOL bEn);
extern void hdmitx_setPgPattern(UINT32 uiPatID);
extern void hdmitx_setPg1(UINT32 Gray, UINT32 HTOT);
extern void hdmitx_setPg2(UINT32 HSW, UINT32 HAB);
extern void hdmitx_setPg3(UINT32 HAW, UINT32 VTOT);
extern void hdmitx_setPg4(UINT32 VSW, UINT32 VAB);
extern void hdmitx_setPg5(UINT32 VAW);
extern void hdmitx_resetSil9002(void);
#endif


#endif
