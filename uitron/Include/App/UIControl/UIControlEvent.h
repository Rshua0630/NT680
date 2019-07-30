/**
    @file       UIControlEvent.h
    @ingroup    mIUIControl

    @brief      Declare events of UI control

    Copyright   Novatek Microelectronics Corp. 2007.  All rights reserved.
*/

#ifndef UICONTROL_EVENT_H
#define UICONTROL_EVENT_H

#include "NVTEvent.h"


/**
    @addtogroup mIUIControl
*/
//@{

/**
     Get event if key event type.

     @param[in] e  Event value.

     @return
        - @b TRUE event is key event type
        - @b FALSE event is not key event type
*/
#define IS_KEY_EVENT(e)                 IS_TYPE_EVENT(e, DEV_KEY_EVENT_BASE)

/**
     Get event if touch event type.

     @param[in] e  Event value.

     @return
        - @b TRUE event is touch event type
        - @b FALSE event is not touch event type
*/
#define IS_TOUCH_EVENT(e)               IS_TYPE_EVENT(e, DEV_TOUCH_EVENT_BASE)

/**
     Get event if touch event type.

     @param[in] e     Event value.
     @param[in] emin  Event minimum value.
     @param[in] emax  Event maximum value.

     @return
        - @b TRUE event is between minimun and maximun
        - @b FALSE event is out of range
*/
#define IN_RANGE_EVENT(e,emin,emax)     (((e) > (emin)) && ((e) < (emax)))

/**
    UI Control event values
*/
enum {
	NVTEVT_WND_EVT_START        = UI_WND_EVENT_BASE, ///< Min value = 0x00008100
	NVTEVT_OPEN_WINDOW          = 0x00008100, ///< Open window command
	NVTEVT_CLOSE_WINDOW         = 0x00008101, ///< Close window command
	NVTEVT_CLOSE_WINDOW_CLEAR   = 0x00008102, ///< Close window command with clear screen
	NVTEVT_CHILD_CLOSE          = 0x00008103, ///< Close child window command
	NVTEVT_REDRAW               = 0x00008104, ///< Redraw screen command
	NVTEVT_BEGIN_CTRL           = 0x00008105, ///< Begin control command
	NVTEVT_END_CTRL             = 0x00008106, ///< End control command
	NVTEVT_BACKGROUND_DONE      = 0x00008107, ///< Background done command
	NVTEVT_OUT_RANGE            = 0x00008108, ///< Current event out of control rectangle,for touch panel event
	/* INSERT NEW EVENT HRER */
	NVTEVT_WND_EVT_END          = UI_WND_EVENT_BASE + 0x100 - 1, ///< Max value = 0x000081ff
};


//@}
#endif
