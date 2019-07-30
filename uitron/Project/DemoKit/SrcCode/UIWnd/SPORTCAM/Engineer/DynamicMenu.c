#include "DynamicMenu.h"
#include "UIFrameworkExt.h"
#include "PrjCfg.h"

void DMenu_SetItem(VControl *pMenuCtrl)
{
    UIDMenuInfo *pDMenuInfo = pMenuCtrl->usrData;
    UINT32 curPage = 0;
    UINT32 curItem = 0;
    UINT32 firstItemInPage = 0, lastItemInPage = 0;
    UINT32 i = 0, j = 0;

    curPage = pDMenuInfo->uiSelectNum / pDMenuInfo->uiPageItem;
    curItem = pDMenuInfo->uiSelectNum % pDMenuInfo->uiPageItem;

    firstItemInPage = curPage*pDMenuInfo->uiPageItem;
    lastItemInPage = firstItemInPage+pDMenuInfo->uiPageItem;

    if (lastItemInPage > (pDMenuInfo->uiMaxItem+1))
    {
        lastItemInPage = (pDMenuInfo->uiMaxItem+1);
    }
    debug_ind(("cur = %d,curPage=%d,curItem=%d firstItemInPage=%d,lastItemInPage=%d\r\n",pDMenuInfo->uiSelectNum,curPage,curItem,firstItemInPage,lastItemInPage));

    UxMenu_SetData(pMenuCtrl,MNU_CURITM,curItem);

    for (i= 0 ; i< pDMenuInfo->uiPageItem;i++)
    {
        UxMenu_SetItemData(pMenuCtrl, i, MNUITM_STRID, STRID_NULL );
    }
    for (i = firstItemInPage; i < lastItemInPage; i++)
    {
        UxMenu_SetItemData(pMenuCtrl, j, MNUITM_STRID, Txt_Pointer(pDMenuInfo->menuItem[i].pItemName) );
        j++;
    }
}


void DMenu_InitItem(VControl *pMenuCtrl,UIDMenuInfo *pDMenuInfo)
{
    pMenuCtrl->usrData = pDMenuInfo;
    DMenu_SetItem(pMenuCtrl);
}
INT32 DMenu_PreItem(VControl *pMenuCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    UIDMenuInfo *dMenuInfo = pMenuCtrl->usrData;
    UINT32 prePage = dMenuInfo->uiSelectNum / dMenuInfo->uiPageItem;
    UINT32 curPage = 0;

    if (dMenuInfo->uiSelectNum == 0)
    {
        dMenuInfo->uiSelectNum = dMenuInfo->uiMaxItem;
    }
    else
    {
        dMenuInfo->uiSelectNum--;
    }
    curPage = dMenuInfo->uiSelectNum / dMenuInfo->uiPageItem;

    if (curPage != prePage)
    {
        DMenu_SetItem(pMenuCtrl);
    }
    else
    {
        UxMenu_SetData(pMenuCtrl,MNU_CURITM, dMenuInfo->uiSelectNum %dMenuInfo->uiPageItem);
    }
    return NVTEVT_CONSUME;
}
INT32 DMenu_NextItem(VControl *pMenuCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    UIDMenuInfo *dMenuInfo = pMenuCtrl->usrData;
    UINT32 prePage = dMenuInfo->uiSelectNum / dMenuInfo->uiPageItem;
    UINT32 curPage = 0;

    if (dMenuInfo->uiSelectNum == dMenuInfo->uiMaxItem)
    {
        dMenuInfo->uiSelectNum = 0;
    }
    else
    {
        dMenuInfo->uiSelectNum++;
    }

    curPage = dMenuInfo->uiSelectNum / dMenuInfo->uiPageItem;

    if (curPage != prePage)
    {
        DMenu_SetItem(pMenuCtrl);
    }
    else
    {
        UxMenu_SetData(pMenuCtrl,MNU_CURITM, dMenuInfo->uiSelectNum %dMenuInfo->uiPageItem);
    }
    return NVTEVT_CONSUME;
}
