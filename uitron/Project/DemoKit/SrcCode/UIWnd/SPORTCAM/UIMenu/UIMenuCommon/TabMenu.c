#include <stdio.h>
#include "SysCfg.h"
#include "UIFlow.h"

///////////////////////////////////////////////////////////////////////////////
#define __MODULE__          TabMenu
#define __DBGLVL__          2 // 0=FATAL, 1=ERR, 2=WRN, 3=UNIT, 4=FUNC, 5=IND, 6=MSG, 7=VALUE, 8=USER
#define __DBGFLT__          "*" //*=All, [mark]=CustomClass
#include "DebugModule.h"
///////////////////////////////////////////////////////////////////////////////

void TM_SelectTab(TM_MENU *pMenu, INT32 iNewSel)
{

	// page index needs to be within the effective range, otherwise go back to 1st or last page
	if (iNewSel >= pMenu->Count) {
		pMenu->SelPage = 0;
	} else if (iNewSel < 0) {
		pMenu->SelPage = pMenu->Count - 1;
	} else {
		pMenu->SelPage = iNewSel;
	}
}

void TM_ShiftTab(TM_MENU *pMenu, INT32 iOffset)
{
	TM_SelectTab(pMenu, pMenu->SelPage + iOffset);
}

INT32 TM_SetItemStatus(TM_MENU *pMenu, UINT16 itemId, UINT16 status)
{
	TM_ITEM    *pItem = 0;

	pItem = TM_GetItem(pMenu, itemId);

	if (pItem) {
		pItem->Status = (pItem->Status & ~TM_ITEM_STATUS_MASK) | status;
		return E_OK;
	} else {
		DBG_ERR("item %d not found\r\n", itemId);
		return E_SYS;
	}
}

//check input item, if disable find next or preve enable item
//if return item is page->count : find the end
void TM_CheckItemStatus(TM_MENU *pMenu, UINT16 *checkItem, UINT32 bNext)
{
	TM_PAGE    *pPage = 0;
	TM_ITEM    *pItem = 0;
	UINT16      Index;

	pPage = &pMenu->pPages[pMenu->SelPage];

	Index = *checkItem;
	if (Index < pPage->Count) {
		pItem = &pPage->pItems[Index];
	} else {
		//DBG_ERR("*checkItem %d out of range\r\n",*checkItem);
		return ;
	}

	if (bNext) {
		while (((pItem->Status) & TM_ITEM_STATUS_MASK) == TM_ITEM_DISABLE) {
			if (++Index == pPage->Count) {
				break;
			}
			pItem = &pPage->pItems[Index];
		}
	} else {
		while (((pItem->Status) & TM_ITEM_STATUS_MASK) == TM_ITEM_DISABLE) {
			if (Index == 0) {
				Index = pPage->Count;
				break;
			} else {
				Index--;
				pItem = &pPage->pItems[Index];
			}
		}
	}

	*checkItem = Index;
}

void TM_FindStartIndex(TM_MENU *pMenu, UINT16 pageitem, UINT16 *startIndex)
{
	UINT32 enableCount = 0;
	UINT16 index = 0;
	TM_PAGE    *pPage;

	pPage = &pMenu->pPages[pMenu->SelPage];

	if (pPage->Count == 1) {
		*startIndex = pPage->SelItem;
	} else {
		for (index = 0; index <= pPage->SelItem; index++) {
			if (((&pPage->pItems[index])->Status & TM_ITEM_STATUS_MASK) == TM_ITEM_ENABLE) {
				enableCount++;
				if ((enableCount % pageitem) == 1) {
					*startIndex = index;
				}
			}
		}
	}
}


INT32 TM_SetOptionStatus(TM_MENU *pMenu, UINT16 itemId, UINT16 OptionIndex, UINT16 status)
{
	TM_ITEM    *pItem = 0;
	TM_OPTION  *pOption = 0;
	UINT32      result = E_SYS;

	pItem = TM_GetItem(pMenu, itemId);

	if (pItem) {
		if (OptionIndex < pItem ->Count) {
			pOption = &pItem->pOptions[OptionIndex];
			pOption->Status = (pOption->Status & (~TM_OPTION_STATUS_MASK)) | status;
			result = E_OK;
		} else {
			DBG_ERR("OptionIndex too large\r\n", OptionIndex);
			result = E_SYS;
		}
	} else {
		DBG_ERR("item %d not found\r\n", itemId);
		result = E_SYS;
	}
	return result;
}
void TM_CheckOptionStatus(TM_MENU *pMenu, UINT16 *checkOption, UINT32 bNext)
{
	TM_PAGE    *pPage = 0;
	TM_ITEM    *pItem = 0;
	UINT16      Index;
	UINT32      i = 0;


	pPage = &pMenu->pPages[pMenu->SelPage];
	pItem = &pPage->pItems[pPage->SelItem];
	Index = *checkOption;

	DBG_IND("in %d\r\n", *checkOption);
	if (bNext) {
		for (i = 0; i < pItem->Count ; i++) {
			//check if option is disalbe,if disable get next option
			if (((&pItem->pOptions[Index])->Status & TM_OPTION_STATUS_MASK) == TM_OPTION_ENABLE) {
				break;
			}
			if (Index == pItem->Count - 1) {
				Index = 0;
			} else {
				Index++;
			}
		}
	} else {
		DBG_ERR("not support\r\n");
	}

	*checkOption = Index;
	DBG_IND("out %d\r\n", *checkOption);

}


TM_ITEM *TM_GetItem(TM_MENU *pMenu, UINT16 itemId)
{
	TM_PAGE    *pPage = 0;
	TM_ITEM    *pItem = 0;
	UINT16      i, j = 0;
	UINT16      pageNum = pMenu->Count;

	for (j = 0; j < pageNum; j++) {
		pPage = &(pMenu->pPages[j]);
		for (i = 0; i < pPage->Count; i++) {
			pItem = &pPage->pItems[i];
			if (pItem->ItemId == itemId) {
				return pItem;
			}
		}
	}
	DBG_ERR("not found %d\r\n", itemId);
	return NULL;
}
static TM_MENU *g_pTabMenu;

void TM_SetMenu(TM_MENU *pMenu)
{
	g_pTabMenu = pMenu;
}

TM_MENU *TM_GetMenu(void)
{
	return g_pTabMenu;
}

