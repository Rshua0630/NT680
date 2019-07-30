#ifndef _DXDISPLAY_H_
#define _DXDISPLAY_H_

#include "Dx.h"

typedef struct _DISPLAYMODE {
	UINT32  uiFormat;
	UINT32  uiWidth;
	UINT32  uiHeight;
}
DISPLAYMODE;

typedef struct _DISPLAYMODELIST {
	int     iMaxMode;
}
DISPLAYMODELIST;

//ver
#define DISPLAY_VER                 0x00010000

//caps id
#define DISPLAY_CAPS_BASE           0x00000001
#define DISPLAY_CAPS_PLUG           0x00000002
#define DISPLAY_CAPS_DEFMODE        0x00000003
#define DISPLAY_CAPS_MODEINFO       0x00000004 // current mode size
#define DISPLAY_CAPS_DIRECT         0x00000005
#define DISPLAY_CAPS_VIDMODE        0x00000006 // supported video modes (for HDMI)
#define DISPLAY_CAPS_ASPECT         0x00000007 // display aspect ratio (for human eyes)
#define DISPLAY_CAPS_MAXSIZE        0x00000008 // max mode size

/*
Display Aspect Ratio (DAR) = the aspect ratio of the image as displayed = display_aspect_w : display_aspect_h
Storage Aspect Ratio (SAR) = the aspect ratio of the image buffer = image_buffer_w : image_buffer_h
Pixel Aspect Ratio (PAR) = the aspect ratio of the image buffer scaled to display = scaling_ratio_w : scaling_ratio_h

SAR * PAR = DAR
PAR = DAR / SAR

For example:

A 640x480 VGA image has a SAR of 640/480 = 4:3,
=> if displayed on a 4:3 display (DAR = 4:3) => has square pixels, hence a PAR of 1:1

A 720x576 D-1 PAL image has a SAR of 720/576 = 5:4,
=> if displayed on a 4:3 display (DAR = 4:3) => not has square pixels, hence a PAR of 16:15
*/


//caps base flag
#define DISPLAY_BF_DETPLUG          0x00000001
#define DISPLAY_BF_BACKLIGHT        0x00000002
#define DISPLAY_BF_DETDIR           0x00000004

#define DISPLAY_CFG_MODE            0x00000001
#define DISPLAY_CFG_DOUT            0x00000002
#define DISPLAY_CFG_DUAL            0x00000003
#define DISPLAY_CFG_INFOBUF         0x00000004
#define DISPLAY_CFG_AUDIOFMT        0x00000005

//state id
#define DRVDISP_STATE_ENBALE        0   //enable (R/O)
#define DRVDISP_STATE_MODE          1   //mode (R/O)
#define DRVDISP_STATE_SLEEP         2   //sleep (R/O)
#define DRVDISP_STATE_BACKLIGHT     3   //backlight
#define DRVDISP_STATE_BRIGHTLVL     4   //backlight brightness
#define DRVDISP_STATE_INSERT        5   //TV insert
#define DRVDISP_STATE_LOCK          6   //lock mode (hiding screen to avoid see flikcer)
#define DRVDISP_STATE_LASTMODE      7   //mode (R/O)
#define DRVDISP_STATE_DIRECT        8   //control screen dir by LCD (W/O)


//control id
#define DRVDISP_CTRL_MODE           1   //mode control (for change mode)
#define DRVDISP_CTRL_SLEEP          2   //sleep control (for enter/leave sleep mode)
#define DRVDISP_CTRL_WAITFINISH     5   //wait enable finish (for change device)

//value for DRVDISP_CTRL_ENABLE
//TRUE or FALSE

//value for DRVDISP_CTRL_SLEEP
//TRUE or FALSE

//value for DRVDISP_CTRL_BACKLIGHT
#define DRVDISP_BACKLIGHT_OFF       0
#define DRVDISP_BACKLIGHT_ON        0xff

void DxDisplay_UpdateModeInfo(UINT32 DevID, UINT32 uiLCDMode, USIZE *pWinSize);

#endif //_DXDISPLAY_H_

