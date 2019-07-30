/**
    UI progress bar control related API.

    UI progress bar control export variables and function prototypes.

    @file       UICtrlProgressBarLib.h
    @ingroup    mIUIControl
    @note       Nothing (or anything need to be mentioned).

    Copyright   Novatek Microelectronics Corp. 2011.  All rights reserved.
*/
#ifndef UICTRLPROGRESSBARLIB_H
#define UICTRLPROGRESSBARLIB_H

#include "UIControlExt.h"

/**
    @addtogroup mIUIControl
*/
//@{

/**
     @name Progress bar type
*/
//@{
#define PROBAR_TYPE_MASK        0x0F    ///< Progress bar type mask
#define PROBAR_VERTICAL         0x00    ///< Vertical progress bar bit
#define PROBAR_HORIZONTAL       0x01    ///< Horizontal progress bar bit
#define PROBAR_DIR_MASK         0xF0    ///< Progress bar direction mask
#define PROBAR_NORMAL           0x00    ///< Normal direction(left to right,down to up)
#define PROBAR_INVERSE          0x10    ///< Inverse direction(right to left,up to down)
//@}

/**
     Progress bar data structure
*/
typedef struct _CTRL_PROBAR_DATA {
	UINT32  progressBarType;        ///< Progress bar type and direction
	UINT32  currentStep;            ///< Current step
	UINT32  totalStep;              ///< Total step
} CTRL_PROBAR_DATA;

/**
     Progress bar data attribute index
*/
typedef enum {
	PROBAR_TYPE = (CTRL_PROGRESSBAR << 8),      ///< Progress bar type (bitwise ,refer to progress bar style bit mask)
	PROBAR_CURSTP,                              ///< Progress bar current step
	PROBAR_TOTSTP,                              ///< Progress bar total step
	PROBAR_MAX
} PROBAR_DATA_SET;

/**
    Set progress bar data
    @param[in] pCtrl The pointer of the progress bar control
    @param[in] attribute progress bar attribute(PROBAR_DATA_SET)
    @param[in] value the progress bar value would be changed
*/
extern void UxProgressBar_SetData(VControl *pCtrl, PROBAR_DATA_SET attribute, UINT32 value);

/**
    Get progress bar data
    @param[in] pCtrl The pointer of the progress bar control
    @param[in] attribute progress bar attribute(PROBAR_DATA_SET)

    @return
        - @b The attribute value
        - @b ERR_TYPE The control type error
        - @b ERR_ATTRIBUTE The control has no this attribute
*/
extern UINT32 UxProgressBar_GetData(VControl *pCtrl, PROBAR_DATA_SET attribute);

//@}
#endif
