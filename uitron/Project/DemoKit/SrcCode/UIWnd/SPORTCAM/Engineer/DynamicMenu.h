#ifndef DYNAMIC_MENU_H
#define DYNAMIC_MENU_H
#include "UIFramework.h"

// Calibration requirement: Show different text color in menu. OK: Green text; NG: Red text.
typedef UINT32 (*CALCHKSTSFUNC)(void);    ///< Point to the function which check cal item status.

// Calibration requirement: Show different text color in menu. OK: Green text; NG: Red text.
typedef struct
{
    CHAR    *pItemName;
    FP      ItemFunction;
    VControl *pNextControl;
    CALCHKSTSFUNC ChkStatusFunc;
} DMENU_ITEM;

typedef struct _UIDMenuInfo
{
    UINT32          uiSelectNum;
    DMENU_ITEM      *menuItem;
    UINT32          uiMaxItem;
    UINT32          uiPageItem;
} UIDMenuInfo;


extern  void DMenu_InitItem(VControl *pMenuCtrl,UIDMenuInfo *pDMenuInfo);
extern  INT32 DMenu_PreItem(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray);
extern  INT32 DMenu_NextItem(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray);


#endif
