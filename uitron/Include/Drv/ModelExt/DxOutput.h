#ifndef _DXOUTPUT_H_
#define _DXOUTPUT_H_

#include "Type.h"

///////////////////////////////////////////////////////////////////////////////
// LED
///////////////////////////////////////////////////////////////////////////////
// Definitions for GPIOMap_EnableLED() and GPIOMap_IsLEDOn

//#NT#2010/02/02#Lincy Lin -begin
//#NT#set FCS LED brightness level
typedef enum {
	LCSBRT_LVL_00 = 0,
	LCSBRT_LVL_01,
	LCSBRT_LVL_02,
	LCSBRT_LVL_03,
	LCSBRT_LVL_04,
	LCSBRT_LVL_05,
	LCSBRT_LVL_06,
	LCSBRT_LVL_SETTING_MAX
} LCSBRT_LVL_SETTING;
//#NT#2010/02/02#Lincy Lin -end


#define GPIOMAP_LED_GREEN           0
#define GPIOMAP_LED_RED             1
#define GPIOMAP_LED_FCS             2
#define GPIOMAP_LED_IR              3
extern void     LED_TurnOnLED(UINT32 uiLED);
extern void     LED_TurnOffLED(UINT32 uiLED);
extern BOOL     LED_IsLEDOn(UINT32 uiLED);
//#NT#2010/02/02#Lincy Lin -begin
//#NT#set FCS LED brightness level
extern void     LED_SetFcsLevel(UINT32 uiLevel);
//#NT#2010/02/02#Lincy Lin -end
BOOL LED_OnCommand(CHAR *pcCmdStr);



#include "Dx.h"

//ver
#define OUTPUT_VER                 0x00010000

//caps id
#define OUTPUT_CAPS_BASE           0x00000001
#define OUTPUT_CAPS_TIMER          0x00000002

#define OUTPUT_CFG_MODE            0x00000001

#define OUTPUT_CTRL_MODE           1   //mode control (for change mode)

#endif //_DXOUTPUT_H_

