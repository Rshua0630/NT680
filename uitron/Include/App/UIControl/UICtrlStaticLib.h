/**
    UI static control related API.

    UI  static control export variables and function prototypes.

    @file       UICtrlStaticLib.h
    @ingroup    mIUIControl
    @note       Nothing (or anything need to be mentioned).

    Copyright   Novatek Microelectronics Corp. 2011.  All rights reserved.
*/
#ifndef UICTRLSTATICLIB_H
#define UICTRLSTATICLIB_H

#include "UIControlExt.h"

/**
    @addtogroup mIUIControl
*/
//@{

/**
     Static control data structure
*/
typedef struct _CTRL_STATIC_DATA {
	UINT32 value;                    ///< Value,string id or icon id
} CTRL_STATIC_DATA;
/**
     Static control data attribute index
*/

typedef enum {
	STATIC_VALUE = (CTRL_STATIC << 8),  ///< String id or Icon id attribute index
	STATIC_MAX
} STATIC_DATA_SET;

/**
    Set static control data
    @param[in] pCtrl The pointer of the static control
    @param[in] attribute Static attribute(STATIC_DATA_SET)
    @param[in] value the static value would be changed
*/
extern void UxStatic_SetData(VControl *pCtrl, STATIC_DATA_SET attribute, UINT32 value);

/**
    Get static control data
    @param[in] pCtrl The pointer of the static control
    @param[in] attribute Static attribute(STATIC_DATA_SET)

    @return
        - @b The attribute value
        - @b ERR_TYPE The control type error
        - @b ERR_ATTRIBUTE The control has no this attribute
*/
extern UINT32 UxStatic_GetData(VControl *pCtrl, STATIC_DATA_SET attribute);
//@}
#endif
