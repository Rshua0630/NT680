/**
    Extention UI Control export variables and function prototypes.
    Include menu,list,button,static,state, slide and scroll bar control.

    @file       UIControlExt.h
    @ingroup    mIUIControl
    @note       Nothing (or anything need to be mentioned).

    Copyright   Novatek Microelectronics Corp. 2011.  All rights reserved.
*/

#ifndef UICONTROLEXT_H
#define UICONTROLEXT_H
#include "UIControl.h"
#include "UIControlWnd.h"
#include "GxGfx.h"
/**
     @addtogroup mIUIControl
*/
//@{

/**
    Extention control type.
*/
typedef enum {
	CTRL_PANEL = CTRL_TYPE_MAX,     ///< Panel control,it can contain other type control
	CTRL_STATIC,                    ///< Static control
	CTRL_STATE,                     ///< State control
	CTRL_STATEGRAPH,                ///< State graph control
	CTRL_TAB,                       ///< Tab control
	CTRL_BUTTON,                    ///< Button control
	CTRL_MENU,                      ///< Menu control
	CTRL_LIST,                      ///< List control
	CTRL_SLIDER,                    ///< Slide control
	CTRL_SCROLLBAR,                 ///< Scroll bar control
	CTRL_PROGRESSBAR,               ///< Progress Bar control
	CTRL_ZOOM,                      ///< Zoom control
	CTRL_USER,                      ///< User control
	CTRL_EX_TYPE_MAX                ///< Max extention control type
} CONTROL_EX_TYPE;

/**
    @name Extention control error code.
*/
//@{
#define ERR_TYPE           ((UINT32)-1) ///< Type error,API and parameter in differen control type
#define ERR_ATTRIBUTE      ((UINT32)-2) ///< Attribute error,API not support this attribute
#define ERR_OUT_RANGE      ((UINT32)-3) ///< Touch event not in current control rectangle
//@}

/**
    Extention control default event.
    Each event for different has different default behavior.
*/
typedef enum {
	NVTEVT_CTRL_EVT_START       = UI_CTRL_EVENT_BASE, ///< Min value = 0x00008200
	NVTEVT_UNFOCUS              = 0x00008200, ///< Control loss focus
	NVTEVT_FOCUS                = 0x00008201, ///< Control is focus
	NVTEVT_CHANGE_STATE         = 0x00008202, ///< Control change state
	NVTEVT_PREVIOUS_ITEM        = 0x00008203, ///< Move to previous item.Ex:List or menu control which has many item
	NVTEVT_NEXT_ITEM            = 0x00008204, ///< Move to next item.Ex:List or menu control which has many item
	NVTEVT_FIRST_ITEM           = 0x00008205, ///< Move to first item.Ex:List or menu control which has many item
	NVTEVT_LAST_ITEM            = 0x00008206, ///< Move to last item.Ex:List or menu control which has many item
	NVTEVT_PRESS_ITEM           = 0x00008207, ///< Press item.Ex:List or menu control which has many item
	NVTEVT_NEXT_STEP            = 0x00008208, ///< Move to next step.Ex:Progress bar or scroll bar control which has many step
	NVTEVT_PREVIOUS_STEP        = 0x00008209, ///< Move to previous step.Ex:Progress bar or scroll bar control which has many step
	/* INSERT NEW EVENT HRER */
	NVTEVT_CTRL_EVT_END         = UI_CTRL_EVENT_BASE + 0xe00 - 1, ///< Max value = 0x00008fff
} CONTROL_EVENT;


/**
    Extention control touch event.
    Each event for different has different default behavior.
*/
typedef enum {
	NVTEVT_TOUCH_EVT_START      = DEV_TOUCH_EVENT_BASE,   ///< Min value = 0x11070000
	NVTEVT_PRESS                = 0x11070000, ///< Press control
	NVTEVT_RELEASE              = 0x11070001, ///< Release control
	NVTEVT_MOVE                 = 0x11070002, ///< Move control
	NVTEVT_CLICK                = 0x11070003, ///< Click control
	NVTEVT_DOUBLECLICK          = 0x11070004, ///< Double click control
	NVTEVT_SLIDE_LEFT           = 0x11070005, ///< Slide left for horizontalslide control
	NVTEVT_SLIDE_RIGHT          = 0x11070006, ///< Slide right for horizontal slide control
	NVTEVT_SLIDE_UP             = 0x11070007, ///< Slide up for vertical slide control
	NVTEVT_SLIDE_DOWN           = 0x11070008, ///< Slide down for vertical slide control
	/* INSERT NEW EVENT HRER */
	NVTEVT_HOLD                 = 0x11070009, ///< Hold control        //#NT#2015/09/14#KCHong
	NVTEVT_TOUCH_EVT_END        = DEV_TOUCH_EVENT_BASE + 0x10000 - 1, ///< Max value = 0x1107ffff
} CONTROL_TP_VENT;

/**
    Extention control basic status.
    Each event for different has different appearance.
    User can define in UI tool.
*/
typedef enum {
	STATUS_NORMAL = 0x00,   ///< Control or control item is normal status,
	STATUS_FOCUS,           ///< Control or control item is focuse status
	STATUS_NORMAL_DISABLE,  ///< Control or control item is normal disablestatus
	STATUS_FOCUS_DISABLE,   ///< Control or control item is focus disable status
	STATUS_FOCUS_PRESS,     ///< Control or control item is focus press status
	STATUS_SETTIMG_MAX      ///< Max status value
} STATUS_SETTIMG;


/**
    @name Extention focus/normal status data bit.
    It combines focus/normal bit to become control status
*/
//@{
#define STATUS_FOCUS_MASK    0x0F   ///< Focus/normal mask bit
#define STATUS_NORMAL_BIT    0x00   ///< Normal bit
#define STATUS_FOCUS_BIT     0x01   ///< Focus bit
//@}

/**
    @name Extention enable/disable status data bit.
    It combines focus/normal bit to become control status
*/
//@{
#define STATUS_ENABLE_MASK   0xF0   ///< Enable/disable mask bit
#define STATUS_ENABLE        0x00   ///< Enable bit
#define STATUS_DISABLE       0x10   ///< Disable bit
//@}

/**
     Current item index for extention control API.
     If user want to get some current index attribute;use this special index
     would not have to get current index.
*/
#define CURITEM_INDEX  0xFFFFFFFF


#include "UICtrlButtonLib.h"
#include "UICtrlListLib.h"
#include "UICtrlMenuLib.h"
#include "UICtrlPanelLib.h"
#include "UICtrlProgressBarLib.h"
#include "UICtrlScrollBarLib.h"
#include "UICtrlSliderLib.h"
#include "UICtrlStateGraphLib.h"
#include "UICtrlStateLib.h"
#include "UICtrlStaticLib.h"
#include "UICtrlTabLib.h"
#include "UICtrlWndLib.h"
#include "UICtrlZoomLib.h"
#include "UIDrawShowObj.h"

//@}
#endif //UICONTROLEXT_H
