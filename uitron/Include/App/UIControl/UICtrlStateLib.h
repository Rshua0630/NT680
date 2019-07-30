/**
    UI state control related API.

    UI state control export variables and function prototypes.

    @file       UICtrlStateLib.h
    @ingroup    mIUIControl
    @note       Nothing (or anything need to be mentioned).

    Copyright   Novatek Microelectronics Corp. 2011.  All rights reserved.
*/
#ifndef UICTRLSTATELIB_H
#define UICTRLSTATELIB_H

#include "UIControlExt.h"

/**
    @addtogroup mIUIControl
*/
//@{

/**
     State item data structure
*/
typedef struct _CTRL_STATE_ITEM_DATA {
	UINT32 stringID;                 ///< State item string ID
	UINT32 iconID;                   ///< State item icon ID
	UINT32 pExeEvent;                ///< State item event
} CTRL_STATE_ITEM_DATA;


/**
     State control data structure
*/
typedef struct _CTRL_STATE_DATA {
	UINT32  currentItem;            ///< Current selected data
	UINT32  totalItem;              ///< Total data count
	CTRL_STATE_ITEM_DATA **item;    ///< The pointer of state item
} CTRL_STATE_DATA;


/**
     State control item attribute index
*/
typedef enum {
	STATE_ITEM_STRID = (CTRL_STATE << 8),   ///< String ID attribute index
	STATE_ITEM_ICONID,                      ///< Icon ID attribute index
	STATE_ITEM_EVENT,                       ///< Event attribute index
	STATE_ITEM_MAX
} STATE_ITEM_DATA_SET;

/**
     State control data attribute index
*/
typedef enum {
	STATE_CURITEM = STATE_ITEM_MAX + 1,     ///< Current item attribute index
	STATE_TOTITEM,                          ///< Total item attribute index
	STATE_MAX
} STATE_DATA_SET;

/**
    Set state item data
    @param[in] pCtrl The pointer of the state control
    @param[in] index State item index (start from 0)
    @param[in] attribute State item attribute(STATE_ITEM_DATA_SET)
    @param[in] value the item value would be chanaged
*/
extern void UxState_SetItemData(VControl *pCtrl, UINT32 index, STATE_ITEM_DATA_SET attribute, UINT32 value);

/**
    Get state item data
    @param[in] pCtrl The pointer of the state control
    @param[in] index State item index (start from 0)
    @param[in] attribute State item attribute(STATE_ITEM_DATA_SET)

    @return
    - @b The attribute value
    - @b ERR_TYPE The control type error
    - @b ERR_OUT_RANGE index > total item
    - @b ERR_ATTRIBUTE The control has no this attribute

*/
extern UINT32 UxState_GetItemData(VControl *pCtrl, UINT32 index, STATE_ITEM_DATA_SET attribute);

/**
    Set state data
    @param[in] pCtrl The pointer of the state control
    @param[in] attribute State attribute(STATE_DATA_SET)
    @param[in] value the state value would be changed
*/
extern void UxState_SetData(VControl *pCtrl, STATE_DATA_SET attribute, UINT32 value);
/**
    Get state data
    @param[in] pCtrl The pointer of the state control
    @param[in] attribute State item attribute(STATE_DATA_SET)

    @return
        - @b The attribute value
        - @b ERR_TYPE The control type error
        - @b ERR_ATTRIBUTE The control has no this attribute
*/
extern UINT32 UxState_GetData(VControl *pCtrl, STATE_DATA_SET attribute);
//@}
#endif
