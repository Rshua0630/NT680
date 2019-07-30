/**
    UI menu control related API.

    UI menu control export variables and function prototypes.

    @file       UICtrlMenuLib.h
    @ingroup    mIUIControl
    @note       Nothing (or anything need to be mentioned).

    Copyright   Novatek Microelectronics Corp. 2011.  All rights reserved.
*/
#ifndef UICTRLMENULIB_H
#define UICTRLMENULIB_H

#include "UIControlExt.h"

/**
    @addtogroup mIUIControl
*/
//@{

/**
     Menu item data structure
*/
typedef struct _CTRL_MENU_ITEM_DATA {
	UINT32    stringID;              ///< Menu item string ID
	UINT32    iconID;                ///< Menu item icon ID
	UINT32    StatusFlag;            ///< Menu item status
	UINT32    pExeEvent;             ///< Menu item event
	VControl *pBindCtrl;             ///< Menu item binding control(submenu control)
	UINT32    value;                 ///< Menu item vale(used as submenu selected value)
} CTRL_MENU_ITEM_DATA;

/**
     Menu data structure
*/
typedef struct _CTRL_MENU_DATA {
	UINT32  currentItem;            ///< Current selected item
	UINT32  totalItem;              ///< Total item
	UINT32  pageItem;               ///< Item number per page
	UINT32  style;                  ///< Menu style (bitwise ,refer to menu style bit mask)
	UINT32  action;                 ///< Menu action
	CTRL_MENU_ITEM_DATA **item;     ///< The pointer of menu item
	UINT32  reserved;               ///< Reserved
} CTRL_MENU_DATA;

/**
    @name Menu style group
*/
//@{
#define MENU_LAYOUT_MASK         0xF        ///< Menu layout mask
#define MENU_LAYOUT_VERTICAL     0x0        ///< Vertical type menu
#define MENU_LAYOUT_HORIZONTAL   0x1        ///< Horizontal type menu

#define MENU_SCROLL_END_MASK     0xF0       ///< Menu scroll end behavior mask
#define MENU_SCROLL_STOP         0x00       ///< Scroll to item end and then stop
#define MENU_SCROLL_CYCLE        0x10       ///< Scroll to item end and start form first item

#define MENU_DISABLE_SCRL_MASK   0xF00      ///< Menu disable item scroll behavior mask
#define MENU_DISABLE_NORMAL      0x000      ///< Scroll on disable menu item
#define MENU_DISABLE_SKIP        0x100      ///< Skip disable menu item

#define MENU_DISABLE_MASK        0xF000     ///< Menu disable item show mask
#define MENU_DISABLE_SHOW        0x0000     ///< Show disable menu item
#define MENU_DISABLE_HIDE        (0x1000|MENU_DISABLE_SKIP) ///< Hide disable menu item(if hide item,must skip item)

#define MENU_SCROLL_MASK         0xF0000    ///< Menu scroll to next item behavior at the end of page
#define MENU_SCROLL_NEXT_PAGE    0x00000    ///< Scroll next item,change to new page
#define MENU_SCROLL_NEXT_ITEM    0x10000    ///< Scroll next item,but only shift up one item(still on the end of page)
#define MENU_SCROLL_CENTER       0x20000    ///< Scrool next item,but item in the page of center(usuall for touch panel menu)

#define MENU_DRAW_MASK           0xF00000   ///< Menu item show table mask
#define MENU_DRAW_IMAGE          0x000000   ///< Every one item has only one icon
#define MENU_DRAW_IMAGE_LIST     0x100000   ///< Every one item has different status icon(Second stautus icon id need to fellow the first icon id.)
#define MENU_DRAW_IMAGE_TABLE    0x200000   ///< Every one item has different status icon by table (icon table would set in control user data)
#define MENU_TEXTCOLOR_TABLE     0x400000   ///< Every one item has different status string color by table (string color table would set in control user data)

#define MENU_REV_TOTITM_MASK     0x0000FFFF  ///< Keep allocated total item mask
#define MENU_REV_KEEP_MASK       0xFFFF0000  ///< Keep inner data mask
//@}

/**
    Menu item attribute index
*/
typedef enum {
	MNUITM_STRID,               ///< Menu item string id
	MNUITM_ICONID,              ///< Menu item icon id
	MNUITM_STATUS,              ///< Menu item status (STATUS_DISABLE,STATUS_ENABLE)
	MNUITM_EVENT,               ///< Menu item event
	MNUITM_CTRL,                ///< Menu item sub control
	MNUITM_VALUE,               ///< Menu item value (used as submenu selected value)
	MNUITM_MAX
} MNUITM_DATA_SET;

/**
    Menu data attribute index
*/
typedef enum {
	MNU_CURITM,                 ///< Menu current item
	MNU_TOTITM,                 ///< Total menu item
	MNU_PAGEITEM,               ///< Menu item per page
	MNU_STYLE,                  ///< Menu style(bitwise ,refer to menu style bit mask)
	MNU_ACTION,                 ///< Menu action(NVTEVT_PREVIOUS_ITEM,NVTEVT_NEXT_ITEM)
	MNU_RESERVED,               ///< Reserved(MENU_SCROLL_NEXT_ITEM,reserved would keep scrollStartIndex)
	MNU_MAX                     ///< Max menu data attribute
} MNU_DATA_SET;

/**
    Set menu item data
    @param[in] pCtrl The pointer of the menu control
    @param[in] index Menu item index (start from 0)
    @param[in] attribute Menu item attribute(MNUITM_DATA_SET)
    @param[in] value the item value would be changed
*/
extern void UxMenu_SetItemData(VControl *pCtrl, UINT32 index, MNUITM_DATA_SET attribute, UINT32 value);

/**
    Get menu item data
    @param[in] pCtrl The pointer of the menu control
    @param[in] index Menu item index (start from 0)
    @param[in] attribute Menu item attribute(MNUITM_DATA_SET)

    @return
    - @b The attribute value
    - @b ERR_TYPE The control type error
    - @b ERR_OUT_RANGE index > total item
    - @b ERR_ATTRIBUTE The control has no this attribute
*/
extern UINT32 UxMenu_GetItemData(VControl *pCtrl, UINT32 index, MNUITM_DATA_SET attribute);

/**
    Set menu data
    @param[in] pCtrl The pointer of the menu control
    @param[in] attribute Menu data attribute(MNU_DATA_SET.)
    @param[in] value The data value would be changed
*/
extern void UxMenu_SetData(VControl *pCtrl, MNU_DATA_SET attribute, UINT32 value);

/**
    Get menu data
    @param[in] pCtrl The pointer of the menu control
    @param[in] attribute Menu data attribute(MNU_DATA_SET)

    @return
        - @b The attribute value
        - @b ERR_TYPE The control type error
        - @b ERR_ATTRIBUTE The control has no this attribute
*/
extern UINT32 UxMenu_GetData(VControl *pCtrl, MNU_DATA_SET attribute);

/**
    Get menu rectangle
    @param[in] pCtrl The pointer of the menu control
    @param[out] pRect The pointer of menu rectangle
*/
extern void UxMenu_GetRange(VControl *pCtrl, Ux_RECT *pRect);

/**
    Get menu item position
    @param[in] pCtrl The pointer of the menu control
    @param[in] index Menu item index (start from 0)
    @param[out] pRect The pointer of menu rectangle
*/
extern void UxMenu_GetItemPos(VControl *pCtrl, UINT32 index, Ux_RECT *pRect);
//@}
#endif
