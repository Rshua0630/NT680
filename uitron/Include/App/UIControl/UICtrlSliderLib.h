/**
    UI slider control related API.

    UI slider control export variables and function prototypes.

    @file       UICtrlSliderLib.h
    @ingroup    mIUIControl
    @note       Nothing (or anything need to be mentioned).

    Copyright   Novatek Microelectronics Corp. 2011.  All rights reserved.
*/
#ifndef UICTRLSLIDERLIB_H
#define UICTRLSLIDERLIB_H

#include "UIControlExt.h"

/**
    @addtogroup mIUIControl
*/
//@{

/**
     Slider bar type
*/
typedef enum {
	SLIDER_VERTICAL = 0x00,         ///< Horizontal slider bar
	SLIDER_HORIZONTAL               ///< Vertical slider bar
} SLIDER_TYPE_SET;

/**
     Slider bar data structure
*/
typedef struct _CTRL_SLIDER_DATA {
	SLIDER_TYPE_SET sliderType;     ///< Slider bar type : vertical / horizontal
	UINT32  currentStep;            ///< Current step
	UINT32  totalStep;              ///< Total step
} CTRL_SLIDER_DATA;


/**
     Slider bar show objct group index
*/
typedef enum {
	SLIDER_BACKGROUND = 0x00,       ///< The button group is background.
	SLIDER_UP,                      ///< Thumb up layer
	SLIDER_DOWN,                    ///< Thumb down layer
	SLIDER_THUMBNAIL,               ///< Thumb layer
	SLIDER_TOTAL_GROUP
} SLIDER_GROUP;

/**
     Slider bar data attribute index
*/
typedef enum {
	SLIDER_TYPE = (CTRL_SLIDER << 8),   ///< Slider bar type,refer to SLIDER_TYPE_SET
	SLIDER_CURSTP,                      ///< Slider bar current step
	SLIDER_TOTSTP,                      ///< Slider bar total step
	SLIDER_MAX
} SLIDER_DATA_SET;

/**
    Set slider bar data
    @param[in] pCtrl The pointer of the slider bar control
    @param[in] attribute Slider bar attribute(SLIDER_DATA_SET)
    @param[in] value The slider bar value would be changed
*/
extern void UxSlider_SetData(VControl *pCtrl, SLIDER_DATA_SET attribute, UINT32 value);
/**
    Get slider bar item data
    @param[in] pCtrl The pointer of the slider bar control
    @param[in] attribute slider bar item attribute(SLIDER_DATA_SET)

    @return
        - @b The attribute value
        - @b ERR_TYPE The control type error
        - @b ERR_ATTRIBUTE The control has no this attribute
*/
extern UINT32 UxSlider_GetData(VControl *pCtrl, SLIDER_DATA_SET attribute);

//@}
#endif
