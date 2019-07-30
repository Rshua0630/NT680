/**
    UI tab control related API.

    UI tab control export variables and function prototypes.

    @file       UICtrlTabLib.h
    @ingroup    mIUIControl
    @note       Nothing (or anything need to be mentioned).

    Copyright   Novatek Microelectronics Corp. 2011.  All rights reserved.
*/

#ifndef UICTRLTABLIB_H
#define UICTRLTABLIB_H

/**
    @addtogroup mIUIControl
*/
//@{

/**
     Tab control data structure
*/
typedef struct _CTRL_TAB_DATA {
	UINT32 focus;               ///< Focus control in the tab
	UINT32 total;               ///< Total control number in the tab
} CTRL_TAB_DATA;

/**
     Scroll bar data attribute index
*/
typedef enum {
	TAB_FOCUS = (CTRL_TAB << 8),        ///< Focus attribute index
	TAB_TOTAL,                          ///< Total control index
	ENUM_DUMMY4WORD(TAB_DATA_SET)
} TAB_DATA_SET;

/**
    Set tab data
    @param[in] pCtrl The pointer of the tab control
    @param[in] attribute Tab attribute(TAB_DATA_SET)
    @param[in] value The static value would be changed
*/
extern void UxTab_SetData(VControl *pCtrl, TAB_DATA_SET attribute, UINT32 value);

/**
    Get tab data
    @param[in] pCtrl The pointer of the tab control
    @param[in] attribute Tab attribute(TAB_DATA_SET)

    @return
        - @b The attribute value
        - @b ERR_TYPE The control type error
        - @b ERR_ATTRIBUTE The control has no this attribute
*/
extern UINT32 UxTab_GetData(VControl *pCtrl, TAB_DATA_SET attribute);


//@}
#endif
