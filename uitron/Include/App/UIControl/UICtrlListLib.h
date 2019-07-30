/**
    UI list control related API.

    UI list control export variables and function prototypes.

    @file       UICtrlListLib.h
    @ingroup    mIUIControl
    @note       Nothing (or anything need to be mentioned).

    Copyright   Novatek Microelectronics Corp. 2011.  All rights reserved.
*/

#ifndef UICTRLLISTLIB_H
#define UICTRLLISTLIB_H

#include "UIControlExt.h"

/**
    @addtogroup mIUIControl
*/
//@{

/**
     List item data structure
*/
typedef struct _CTRL_LIST_ITEM_DATA {
	UINT32 stringID;                 ///< List item string ID
	UINT32 iconID;                   ///< List item icon ID
	UINT32 StatusFlag;               ///< List item status
} CTRL_LIST_ITEM_DATA;

/**
     List data structure
*/
typedef struct _CTRL_LIST_DATA {
	UINT32  currentItem;            ///< Current selected item
	UINT32  totalItem;              ///< Total item
	UINT32  pageItem;               ///< Item number per page
	UINT32  style;                  ///< List style (bitwise ,refer to list style bit mask)
	UINT32  action;                 ///< List action
	CTRL_LIST_ITEM_DATA **item;     ///< The pointer of list item
	UINT32 reserved;                ///< Reserved
	UINT32 pExeEvent;               ///< List event
} CTRL_LIST_DATA;


/**
    @name List style group
*/
//@{
#define LIST_LAYOUT_MASK         0xF                    ///< List layout mask
#define LIST_LAYOUT_VERTICAL     0x0                    ///< Vertical type list
#define LIST_LAYOUT_HORIZONTAL   0x1                    ///< Horizontal type list
#define LIST_LAYOUT_ARRAY        0x2                    ///< Array type list

#define LIST_SCROLL_END_MASK     0xF0                   ///< List scroll end behavior mask
#define LIST_SCROLL_STOP         0x00                   ///< Scroll to item end and then stop
#define LIST_SCROLL_CYCLE        0x10                   ///< Scroll to item end and start form first item

#define LIST_DISABLE_SCRL_MASK   0xF00                  ///< List disable item scroll behavior mask
#define LIST_DISABLE_NORMAL      0x000                  ///< Scroll on disable list item
#define LIST_DISABLE_SKIP        0x100                  ///< Skip disable list item

#define LIST_DISABLE_MASK        0xF000                 ///< List disable item show mask
#define LIST_DISABLE_SHOW        0x0000                 ///< Show disable list item
#define LIST_DISABLE_HIDE        (0x1000|LIST_DISABLE_SKIP) ///< Hide disable list item(if hide item,must skip item)

#define LIST_SCROLL_MASK         0xF0000                ///< List scroll to next item behavior at the end of page
#define LIST_SCROLL_NEXT_PAGE    0x00000                ///< Scroll next item,change to new page
#define LIST_SCROLL_NEXT_ITEM    0x10000                ///< Scroll next item,but only shift up one item(still on the end of page)

#define LIST_DRAW_MASK           0xF00000               ///< List item show table mask
#define LIST_DRAW_IMAGE          0x000000               ///< Every one item has only one icon
#define LIST_DRAW_IMAGE_LIST     0x100000               ///< Every one item has different status icon(Second stautus icon id need to fellow the first icon id.)
#define LIST_DRAW_IMAGE_TABLE    0x200000               ///< Every one item has different status icon by table (icon table would set in control user data)
#define LIST_REV_TOTITM_MASK     0xFFFF                 ///< Keep allocated total item
//@}

/**
    List item attribute index
*/
typedef enum {
	LSTITM_STRID = (CTRL_LIST << 8),    ///< List string id
	LSTITM_ICONID,                      ///< List icon id
	LSTITM_STATUS,                      ///< List status (STATUS_DISABLE,STATUS_ENABLE)
	LSTITM_MAX
} LSTITM_DATA_SET;

/**
    List data attribute index
*/
typedef enum {
	LST_CURITM = LSTITM_MAX + 1,        ///< List current item
	LST_TOTITM,                         ///< Total list item
	LST_PAGEITEM,                       ///< List item per page
	LST_STYLE,                          ///< List syle
	LST_ACTION,                         ///< List action
	LST_RESERVED,                       ///< Reserved
	LST_EVENT,                          ///< List event (each list has only event)
	LST_MAX
} LST_DATA_SET;

/**
    Set list item data
    @param[in] pCtrl The pointer of the list control
    @param[in] index List item index (start from 0)
    @param[in] attribute List item attribute(LSTITM_DATA_SET¡K.)
    @param[in] value the item value would be chanaged
*/

extern void UxList_SetItemData(VControl *pCtrl, UINT32 index, LSTITM_DATA_SET attribute, UINT32 value);

/**
    Get list item data
    @param[in] pCtrl The pointer of the list control
    @param[in] index List item index (start from 0)
    @param[in] attribute List item attribute(LSTITM_DATA_SET¡K.)
*/

extern UINT32 UxList_GetItemData(VControl *pCtrl, UINT32 index, LSTITM_DATA_SET attribute);

/**
    Set list data
    @param[in] pCtrl The pointer of the list control
    @param[in] attribute List attribute(LST_DATA_SET)
    @param[in] value the list value would be changed
*/

extern void UxList_SetData(VControl *pCtrl, LST_DATA_SET attribute, UINT32 value);

/**
    Get list data
    @param[in] pCtrl The pointer of the list control
    @param[in] attribute List attribute(LST_DATA_SET)

    @return
        - @b The attribute value
        - @b ERR_TYPE The control type error
        - @b ERR_ATTRIBUTE The control has no this attribute
*/

extern UINT32 UxList_GetData(VControl *pCtrl, LST_DATA_SET attribute);

/**
    Get list rectangle
    @param[in] pCtrl The pointer of the list control
    @param[out] pRect The pointer of list rectangle
*/
extern void UxList_GetRange(VControl *pCtrl, Ux_RECT *pRect);

/**
    Get list item position
    @param[in] pCtrl The pointer of the list control
    @param[in] index list item index (start from 0)
    @param[out] pRect The pointer of list rectangle
*/
extern void UxList_GetItemPos(VControl *pCtrl, UINT32 index, Ux_RECT *pRect);

//@}
#endif
