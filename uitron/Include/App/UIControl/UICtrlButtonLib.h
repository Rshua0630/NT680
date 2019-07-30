/**
    UI Button control related API.

    UI Button Control export variables and function prototypes.

    @file       UICtrlButtonLib.h
    @ingroup    mIUIControl
    @note       Nothing (or anything need to be mentioned).

    Copyright   Novatek Microelectronics Corp. 2011.  All rights reserved.
*/


#ifndef UICTRLBUTTONLIB_H
#define UICTRLBUTTONLIB_H

#include "UIControlExt.h"

/**
    @addtogroup mIUIControl
*/
//@{

/**
    button item index
*/
#define BUTTON_ONE      0   ///< Button always has one item,always get first one

/**
    @name Button draw style
*/
//@{
#define BTN_DRAW_MASK           0xF00000    ///< Draw mask bit
#define BTN_DRAW_IMAGE          0x000000    ///< Normal draw image,only one icon in every status
#define BTN_DRAW_IMAGE_LIST     0x100000    ///< Draw image list,every status has one icon.Second stautus icon id need to fellow the first icon id.
//@}

/**
     Button item data structure
*/
typedef struct _CTRL_BUTTON_ITEM_DATA {
	UINT32 stringID;                     ///< Button item string id
	UINT32 iconID;                       ///< Button item icon id
	UINT32 StatusFlag;                   ///< Button item status (STATUS_DISABLE,STATUS_ENABLE)
	UINT32 pExeEvent;                    ///< Button item event
} CTRL_BUTTON_ITEM_DATA;

/**
     Button control data structure
*/
typedef struct _CTRL_BUTTON_DATA {
	UINT32  style;                      ///< Button style ,please refer to button style
	CTRL_BUTTON_ITEM_DATA **item;       ///< Button items pointer
} CTRL_BUTTON_DATA;

/**
    Button item attribute index
*/
typedef enum {
	BTNITM_STRID = (CTRL_BUTTON << 8),  ///< Button item string id index
	BTNITM_ICONID,                      ///< Button item icon id index
	BTNITM_STATUS,                      ///< Button item status index
	BTNITM_EVENT,                       ///< Button item event index
	BTNITM_MAX
} BTNITM_DATA_SET;

/**
    Button data attribute index
*/
typedef enum {
	BTN_STYLE = BTNITM_MAX + 1,         ///< Button style index
	BTN_MAX
} BTN_DATA_SET;

/**
    Set button item data
    @param[in] pCtrl The pointer of the button control
    @param[in] index Button item index (only one,always index 0)
    @param[in] attribute Button item attribute(BTNITM_DATA_SET)
    @param[in] value the item value would be chanaged
*/
extern void UxButton_SetItemData(VControl *pCtrl, UINT32 index, BTNITM_DATA_SET attribute, UINT32 value);

/**
    Get button item data
    @param[in] pCtrl The pointer of the button control
    @param[in] index Button item index (only one,always index 0)
    @param[in] attribute Button item attribute(BTNITM_DATA_SET)
*/
extern UINT32 UxButton_GetItemData(VControl *pCtrl, UINT32 index, BTNITM_DATA_SET attribute);

/**
    Set button data
    @param[in] pCtrl The pointer of the button control
    @param[in] attribute Button attribute(BTN_DATA_SET)
    @param[in] value the button value would be changed
*/
extern void UxButton_SetData(VControl *pCtrl, BTN_DATA_SET attribute, UINT32 value);

/**
    Get button data
    @param[in] pCtrl The pointer of the button control
    @param[in] attribute Button attribute(BTN_DATA_SET)

    @return
        - @b The attribute value
        - @b ERR_TYPE The control type error
        - @b ERR_ATTRIBUTE The control has no this attribute
*/
extern UINT32 UxButton_GetData(VControl *pCtrl, BTN_DATA_SET attribute);
//@}

#endif
