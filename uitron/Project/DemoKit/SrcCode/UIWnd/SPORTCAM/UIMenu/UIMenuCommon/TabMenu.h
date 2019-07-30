
#ifndef _TAB_MENU_H_
#define _TAB_MENU_H_

#include "Type.h"
#include "UIFlow.h"

// forward declare
struct _TM_MENU;

// MenuCallback function for upper layer
typedef int (*pfnCallBack)(UINT32 uiMessage, UINT32 uiParam);

// ----------------------------------------------------------------------------
// Data structure for menu system
// ----------------------------------------------------------------------------

// Option: option for items, includes text and icon
typedef struct _TM_OPTION {
	UINT16          TextId;         // Text
	UINT16          IconId;         // Icon
	UINT16          Status;         // Option Status is Enable/Disable
} TM_OPTION;

// Item: an Item includes several Options, or customized functions
typedef struct _TM_ITEM {
	UINT16          ItemId;         // Item ID, to communicate with upper layer
	UINT16          TextId;         // Item text
	UINT16          IconId;         // Item icon
	UINT16          SysFlag;        // if SysFlags = 0, the sub text and default option will be provided by pfnItemProc,
	// otherwise will be decided by related system flag
	UINT16          Status;         // Item Status is Enable/Disable
	UINT16          Count;          // Option number
	TM_OPTION      *pOptions;       // This will be a custom function pointer if Count = 0,
	// otherwise will be a pointer to a normal Option
} TM_ITEM;

// Page: a Page includes several Items
typedef struct _TM_PAGE {
	UINT16          TextId;         // Tab text
	UINT16          IconId;         // Tab icon (enable)
	UINT16          IconIdX;        // Tab icon (disable)
	UINT16           SelItem;        // current selected Item (start with 0)
	UINT16           FirstItem;      // first Item for display
	UINT16          Count;          // Item number
	TM_ITEM        *pItems;         // point to an Item
} TM_PAGE;

// Menu: includes several Pages
typedef struct _TM_MENU {
	pfnCallBack     pCallbackProc;  // callback function for handling the messages informed by TabMenu
	UINT16          Status;         // current operation status (on TAB, on ITEM, on OPTION, ...)
	UINT16           SelPage;        // current Page (start with 0)
	UINT16           FirstPage;      // 1st Page on current display region
	UINT16          Count;          // Page number
	TM_PAGE        *pPages;         // pointer to a Page
} TM_MENU;

// ----------------------------------------------------------------------------
// macros for defining the menu structure
// ----------------------------------------------------------------------------

// in MenuId.h, define IDM_xxxx for Item ID (which will be used in menu callback function)
// IDI_xxxx for Icon ID, and IDS_xxxx for string ID
// in SysFlags.h, define FL_xxxx as system flag, means the setting value of an Option

// --------------------------
// gTM_OPTIONS_xxxx
// --------------------------
#define TMDEF_BEGIN_OPTIONS(name)       TM_OPTION gTM_OPTIONS_##name[] = {

// normal Option, includes text and icon (text on left hand side, icon on right hand side)
#define TMDEF_OPTION_TEXT_ICON(id,status)      { IDS_##id, IDI_##id,status},

// Option has text only (text will align to left)
#define TMDEF_OPTION_TEXT_S(id,status)           { IDS_##id, 0,status},
#define TMDEF_OPTION_TEXT(id)           { IDS_##id, 0,TM_OPTION_ENABLE},

// Option has icon only (icon will align to right)
#define TMDEF_OPTION_ICON(id,status)           { 0, IDI_##id,status},

#define TMDEF_END_OPTIONS()             };

// --------------------------
// gTM_ITEMS_xxxx
// --------------------------
#define TMDEF_BEGIN_ITEMS(name)         TM_ITEM gTM_ITEMS_##name[] = {

// normal Item
#define TMDEF_ITEM_TEXTID(name)         { IDM_##name, IDS_##name, IDI_##name, IDF_##name,TM_ITEM_ENABLE,\
		sizeof(gTM_OPTIONS_##name)/sizeof(gTM_OPTIONS_##name[0]),\
		gTM_OPTIONS_##name },

// Item with custom function, TabMenu will pass the message to custom function
// custom function needs to provide SubText information
#define TMDEF_ITEM_CUSTOM(name, pfnItemProc)    \
	{ IDM_##name, IDS_##name, IDI_##name, 0,\
		TM_ITEM_ENABLE,0,\
		(TM_OPTION*)pfnItemProc },

#define TMDEF_ITEM_NULL()               { IDM_NULL, 0, 0, 0,\
		TM_ITEM_DISABLE,0,\
		NULL },

#define TMDEF_END_ITEMS()               };
//#define TMDEF_END_ITEMS()               TMDEF_ITEM_NULL()};

// --------------------------
// gTM_PAGES_xxxx
// --------------------------
#define TMDEF_BEGIN_PAGES(name)         TM_PAGE gTM_PAGES_##name[] = {

// Page has text and icon
#if 0
#define TMDEF_PAGE_TEXT_ICON(name)      { IDS_##name, IDI_##name, IDIX_##name,\
		0, 0,\
		sizeof(gTM_ITEMS_##name)/sizeof(gTM_ITEMS_##name[0]),\
		gTM_ITEMS_##name },

// Page has text only
#define TMDEF_PAGE_TEXT(name)           { IDS_##name, 0, 0,\
		0, 0,\
		sizeof(gTM_ITEMS_##name)/sizeof(gTM_ITEMS_##name[0]),\
		gTM_ITEMS_##name },

// Page has no text and icon
#define TMDEF_PAGE_NONE(name)           { 0, 0, 0,\
		0, 0,\
		sizeof(gTM_ITEMS_##name)/sizeof(gTM_ITEMS_##name[0]),\
		gTM_ITEMS_##name },
#else
#define TMDEF_PAGE_TEXT_ICON(name)      { IDS_##name, IDI_##name, IDIX_##name,\
		0, 0,\
		sizeof(gTM_ITEMS_##name)/sizeof(gTM_ITEMS_##name[0]),\
		gTM_ITEMS_##name },

// Page has text only
#define TMDEF_PAGE_TEXT(name)           { IDS_##name, 0, 0,\
		0, 0,\
		sizeof(gTM_ITEMS_##name)/sizeof(gTM_ITEMS_##name[0]),\
		gTM_ITEMS_##name },

// Page has no text and icon
#define TMDEF_PAGE_NONE(name)           { 0, 0, 0,\
		0, 0,\
		0,\
		gTM_ITEMS_##name },
#endif

#define TMDEF_END_PAGES()               };

// --------------------------
// MENU
// --------------------------
// tab menu definition
#define TMDEF_EMNU(Menu, pages, proc)   \
	int proc(UINT32 uiMessage, UINT32 uiParam);\
	TM_MENU Menu = {\
					proc,\
					0,\
					0,\
					0,\
					sizeof(gTM_PAGES_##pages)/sizeof(gTM_PAGES_##pages[0]),\
					gTM_PAGES_##pages};

// pass message to menu callback function
#define TM_MENU_CALLBACK(pMenu, uiMessage, uiParam)     (pMenu->pCallbackProc((UINT32)(uiMessage), (UINT32)(uiParam)))

// pass message to custom callback function
#define TM_ITEM_CALLBACK(pItem, uiMessage, uiParam)     (*(pfnCallBack)(pItem->pOptions))((UINT32)(uiMessage), (UINT32)(uiParam))

// ----------------------------------------------------------------------------
// Operation status of menu system
// ----------------------------------------------------------------------------

// Menu Status
enum _TM_MENU_STATUS {
	TMS_CLOSED = 0,             // closed
	TMS_ON_TAB,                 // focus on TAB
	TMS_ON_CONFIRMED,           // Option confirmed, to do animation
	TMS_ON_ITEM,                // focus on Item
	TMS_ON_OPTION,              // focus on Option
	TMS_ON_CUSTOM               // processed by custom function
};

// Message for Callback Function
enum _TM_MENU_MESSAGE {
	TMM_CONFIRM_OPTION = 0,
	TMM_ENTER_OPTION
};

// Menu Flag
enum _TM_MENU_FLAG {
	TMF_PASS_MESSAGE = 0,       // pass message
	TMF_PROCESSED               // processed
};

#define TM_ITEM_STATUS_MASK   0x01   ///< Enable/disable mask bit
#define TM_ITEM_ENABLE        0x00   ///< Enable bit
#define TM_ITEM_DISABLE       0x01   ///< Disable bit

#define TM_OPTION_STATUS_MASK   0x03   ///< Enable/disable mask bit
#define TM_OPTION_ENABLE        0x00   ///< Enable bit
#define TM_OPTION_DISABLE       0x01   ///< Disable bit
#define TM_OPTION_NOT_SUPPORT   0x02 ///< Disabled by Novatek original, do not enable it.
// ----------------------------------------------------------------------------
// Function Declaration
// ----------------------------------------------------------------------------

extern void     TM_ShiftTab(TM_MENU *pMenu, INT32 iOffset);
extern INT32    TM_SetItemStatus(TM_MENU *pMenu, UINT16 itemId, UINT16 status);
extern void     TM_CheckItemStatus(TM_MENU *pMenu, UINT16 *checkItem, UINT32 bNext);
extern void     TM_FindStartIndex(TM_MENU *pMenu, UINT16 pageitem, UINT16 *startIndex);

extern INT32    TM_SetOptionStatus(TM_MENU *pMenu, UINT16 itemId, UINT16 OptionIndex, UINT16 status);
extern void     TM_CheckOptionStatus(TM_MENU *pMenu, UINT16 *checkOption, UINT32 bNext);
extern TM_ITEM *TM_GetItem(TM_MENU *pMenu, UINT16 itemId);
extern void     TM_SetMenu(TM_MENU *pMenu);
extern TM_MENU *TM_GetMenu(void);

#endif //_TAB_MENU_H_
