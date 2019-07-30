/**
    UI scroll bar control related API.

    UI scroll bar control export variables and function prototypes.

    @file       UICtrlScrollBarLib.h
    @ingroup    mIUIControl
    @note       Nothing (or anything need to be mentioned).

    Copyright   Novatek Microelectronics Corp. 2011.  All rights reserved.
*/

#ifndef UICTRLSCROLLBARLIB_H
#define UICTRLSCROLLBARLIB_H

#include "UIControlExt.h"

/**
    @addtogroup mIUIControl
*/
//@{

/**
     Scroll bar type
*/
typedef enum {
	SCRBAR_VERTICAL = 0x00,         ///< Vertical scroll bar
	SCRBAR_HORIZONTAL               ///< Horizontal scroll bar
} SCRLBAR_TYPE;

/**
     The method of scroll bar thumb size
*/
typedef enum {
	SCRBAR_FIXSIZE = 0x00,          ///< Scroll bar rectangle define by user
	SCRBAR_STEP_AVG                 ///< Scroll bar rectangle calculate by total step
} SCRLBAR_SHAPE;

/**
     Scroll bar data structure
*/
typedef struct _CTRL_SCRBAR_DATA {
	SCRLBAR_TYPE    scrlBarType;    ///< Scroll bar type:horizontal or vertical
	SCRLBAR_SHAPE   scrlBarShape;   ///< Thumb lenght.Fix size or calculate by average step rectangle
	UINT32          currentStep;    ///< Current value index
	UINT32          totalStep;      ///< Max step
	UINT32          pageStep;       ///< Step number per page
} CTRL_SCRBAR_DATA;

/**
     Scroll bar show object group index
*/
typedef enum {
	SCRBAR_BACKGROUND = 0x00,   ///< The button group is background.
	SCRBAR_UP,                  ///< Thumb up layer
	SCRBAR_DOWN,                ///< Thumb down layer
	SCRBAR_THUMBNAIL,           ///< Thumb layer
	ENUM_DUMMY4WORD(SCRBAR_GROUP)
} SCRBAR_GROUP;

/**
     Scroll bar data attribute index
*/
typedef enum {
	SCRBAR_TYPE = (CTRL_SCROLLBAR << 8),    ///< Scroll bar type,refer to SCRLBAR_TYPE
	SCRBAR_SHAPE,                           ///< Scroll bar type,please refer to SCRLBAR_SHAPE
	SCRBAR_CURSTP,                          ///< Scroll bar current step
	SCRBAR_TOTSTP,                          ///< Scroll bar total step
	SCRBAR_PAGESTP,                         ///< Scroll bar step per page
	SCRBAR_RECT,                            ///< Scroll bar legth
	SCRBAR_THUMB,                           ///< Scroll bar thumb legth
	ENUM_DUMMY4WORD(SCRBAR_DATA_SET)
} SCRBAR_DATA_SET;

/**
    Set scroll bar data
    @param[in] pCtrl The pointer of the scroll bar control
    @param[in] attribute Scroll bar attribute(SCRBAR_DATA_SET)
    @param[in] value the scroll bar value would be changed
*/
extern void UxScrollBar_SetData(VControl *pCtrl, SCRBAR_DATA_SET attribute, UINT32 value);

/**
    Get scroll bar item data
    @param[in] pCtrl The pointer of the scroll bar control
    @param[in] attribute scroll bar item attribute(SCRBAR_DATA_SET)

    @return
        - @b The attribute value
        - @b ERR_TYPE The control type error
        - @b ERR_ATTRIBUTE The control has no this attribute

*/
extern UINT32 UxScrollBar_GetData(VControl *pCtrl, SCRBAR_DATA_SET attribute);

/**
    Get scroll bar rectangle
    @param[in] pCtrl The pointer of the scroll bar control
    @param[out] pRange The pointer of scroll bar rectangle
*/
extern void UxScrollBar_GetRange(VControl *pCtrl, Ux_RECT *pRange);
//@}
#endif
