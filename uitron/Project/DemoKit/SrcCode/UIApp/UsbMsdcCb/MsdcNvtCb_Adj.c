#include "SysKer.h"
#include "MsdcNvtApi.h"
#include "MsdcNvtCallback.h"
#include "MsdcNvtCb_Adj.h"
#include <string.h>
#include "PrjCfg.h"

#define THIS_DBGLVL         2       // 0=FATAL, 1=ERR, 2=WRN, 3=UNIT, 4=FUNC, 5=IND, 6=MSG, 7=VALUE, 8=USER
#define __MODULE__          MsdcNvt_Adj
#define __DBGLVL__          ((THIS_DBGLVL>=PRJ_DBG_LVL)?THIS_DBGLVL:PRJ_DBG_LVL)
#define __DBGFLT__          "*" //*=All, [mark]=CustomClass
#include "DebugModule.h"

//for MSDCVendorNVT_AddCallback_Bi
static void MsdcNvtCb_AdjGetNumOfItem(void *pData);
static void MsdcNvtCb_AdjGetItemsDesc(void *pData);
static void MsdcNvtCb_AdjSetItemsData(void *pData);
static void MsdcNvtCb_AdjGetItemsData(void *pData);

MSDCNVT_REG_BI_BEGIN(m_MsdcNvtAdj)
MSDCNVT_REG_BI_ITEM(MsdcNvtCb_AdjGetNumOfItem)
MSDCNVT_REG_BI_ITEM(MsdcNvtCb_AdjGetItemsDesc)
MSDCNVT_REG_BI_ITEM(MsdcNvtCb_AdjSetItemsData)
MSDCNVT_REG_BI_ITEM(MsdcNvtCb_AdjGetItemsData)
MSDCNVT_REG_BI_END()

BOOL MsdcNvtRegBi_Adj(void)
{
	return MsdcNvt_AddCallback_Bi(m_MsdcNvtAdj);
}

//MsdcNvtCb_Adj.c
typedef enum _VAR_TYPE {
	VAR_TYPE_BOOL,
	VAR_TYPE_UINT32,
	VAR_TYPE_INT32,
	VAR_TYPE_UINT16,
	VAR_TYPE_INT16,
	VAR_TYPE_UINT8,
	VAR_TYPE_INT8
} eVAR_TYPE;

typedef struct _ADJ_ITEM {
	void       *pVar;
	eVAR_TYPE   VarType;
	INT32       MaxValue;
	INT32       MinValue;
	CHAR        Tag[64];
} tADJ_ITEM;

//Step 1: Includes
//Step 2: Variables Control
//You can extern these variables at other places for temp adjustment
UINT32 MsdcPar0 = 0;
UINT32 MsdcPar1 = 0;
UINT32 MsdcPar2 = 0;
UINT32 MsdcPar3 = 0;
UINT32 MsdcPar4 = 0;
UINT32 MsdcPar5 = 0;
//Step 3: Setup Up Item
static tADJ_ITEM Items[] = {
	{&MsdcPar0, VAR_TYPE_UINT32, 255, 0, "MsdcPar0"},
	{&MsdcPar1, VAR_TYPE_UINT32, 255, 0, "MsdcPar1"},
	{&MsdcPar2, VAR_TYPE_UINT32, 255, 0, "MsdcPar2"},
	{&MsdcPar3, VAR_TYPE_UINT32, 255, 0, "MsdcPar3"},
	{&MsdcPar4, VAR_TYPE_UINT32, 255, 0, "MsdcPar4"},
	{&MsdcPar5, VAR_TYPE_UINT32, 255, 0, "MsdcPar5"},
};
//Step 4: (Optional) Implement Get Callback, as xxx_get_xxx to refresh your variable controls
static void ItemsGetData(void)
{
}
//Step 5: Implement Set Callback, as xxx_set_xxx to set variables to active
static void ItemsSetData(void)
{
}

static void MsdcNvtCb_AdjGetNumOfItem(void *pData)
{
	tMSDCEXT_UINT32 *pDesc = MSDCNVT_CAST(tMSDCEXT_UINT32, pData);

	if (pDesc == NULL) {
		DBG_ERR("Invalid input data.\r\n");
		return;
	}

	pDesc->uiValue = sizeof(Items) / sizeof(Items[0]);
	pDesc->tParent.bOK = TRUE;
}

static void MsdcNvtCb_AdjGetItemsDesc(void *pData)
{
	tMSDCEXT_ADJ_ALL_DESC *pDesc = MSDCNVT_CAST(tMSDCEXT_ADJ_ALL_DESC, pData);
	tMSDCEXT_ADJ_ITEM_DESC *pItemDesc = (tMSDCEXT_ADJ_ITEM_DESC *)(&pDesc[1]);
	UINT32 i, nItem = sizeof(Items) / sizeof(Items[0]);

	if (pDesc == NULL) {
		DBG_ERR("Invalid input data.\r\n");
		return;
	}

	for (i = 0; i < nItem; i++) {
		tADJ_ITEM *pSrc = &Items[i];
		tMSDCEXT_ADJ_ITEM_DESC *pDst = pItemDesc + i;
		memset(pDst, 0, sizeof(tMSDCEXT_ADJ_ITEM_DESC));
		switch (pSrc->VarType) {
		case VAR_TYPE_INT32:
		case VAR_TYPE_INT16:
			pDst->bIsSign = TRUE;
			break;
		default:
			break;

		}
		pDst->MaxValue = pSrc->MaxValue;
		pDst->MinValue = pSrc->MinValue;
		strncpy(pDst->Tag, pSrc->Tag, sizeof(pDst->Tag) - 1);
		pDst->Tag[sizeof(pDst->Tag) - 1] = 0;
	}
	pDesc->tParent.bOK = TRUE;
}

static void MsdcNvtCb_AdjSetItemsData(void *pData)
{
	tMSDCEXT_ADJ_DATA *pDesc = MSDCNVT_CAST(tMSDCEXT_ADJ_DATA, pData);
	UINT32 *pVars = (UINT32 *)(&pDesc[1]);
	UINT32 i, nItem = sizeof(Items) / sizeof(Items[0]);

	if (pDesc == NULL) {
		DBG_ERR("Invalid input data.\r\n");
		return;
	}

	for (i = 0; i < nItem; i++) {
		tADJ_ITEM *pItem = &Items[i];
		void      *pVar =  &pVars[i];

		switch (pItem->VarType) {
		case VAR_TYPE_BOOL:
			*((BOOL *)pItem->pVar) = *((BOOL *)pVar);
			debug_msg("%s=%d,", pItem->Tag, *((BOOL *)pVar));
			break;
		case VAR_TYPE_UINT32:
			*((UINT32 *)pItem->pVar) = *((UINT32 *)pVar);
			debug_msg("%s=%d,", pItem->Tag, *((UINT32 *)pVar));
			break;
		case VAR_TYPE_INT32:
			*((INT32 *)pItem->pVar) = *((INT32 *)pVar);
			debug_msg("%s=%d,", pItem->Tag, *((INT32 *)pVar));
			break;
		case VAR_TYPE_UINT16:
			*((UINT16 *)pItem->pVar) = *((UINT16 *)pVar);
			debug_msg("%s=%d,", pItem->Tag, *((UINT16 *)pVar));
			break;
		case VAR_TYPE_INT16:
			*((INT16 *)pItem->pVar) = *((INT16 *)pVar);
			debug_msg("%s=%d,", pItem->Tag, *((INT16 *)pVar));
			break;
		case VAR_TYPE_UINT8:
			*((UINT8 *)pItem->pVar) = *((UINT8 *)pVar);
			debug_msg("%s=%d,", pItem->Tag, *((UINT8 *)pVar));
			break;
		case VAR_TYPE_INT8:
			*((INT8 *)pItem->pVar) = *((INT8 *)pVar);
			debug_msg("%s=%d,", pItem->Tag, *((INT8 *)pVar));
			break;
		}
	}
	debug_msg("\r\n");

	ItemsSetData();
	pDesc->tParent.bOK = TRUE;
}

static void MsdcNvtCb_AdjGetItemsData(void *pData)
{
	tMSDCEXT_ADJ_DATA *pDesc = MSDCNVT_CAST(tMSDCEXT_ADJ_DATA, pData);
	UINT32 *pVars = (UINT32 *)(&pDesc[1]);
	UINT32 i, nItem = sizeof(Items) / sizeof(Items[0]);

	if (pDesc == NULL) {
		DBG_ERR("Invalid input data.\r\n");
		return;
	}


	ItemsGetData();

	for (i = 0; i < nItem; i++) {
		tADJ_ITEM *pItem = &Items[i];
		void      *pVar = &pVars[i];

		*((UINT32 *)pVar) = 0; //Reset to 0

		switch (pItem->VarType) {
		case VAR_TYPE_BOOL:
			*((BOOL *)pVar) = *((BOOL *)pItem->pVar);
			break;
		case VAR_TYPE_UINT32:
			*((UINT32 *)pVar) = *((UINT32 *)pItem->pVar);
			break;
		case VAR_TYPE_INT32:
			*((INT32 *)pVar) = *((INT32 *)pItem->pVar);
			break;
		case VAR_TYPE_UINT16:
			*((UINT16 *)pVar) = *((UINT16 *)pItem->pVar);
			break;
		case VAR_TYPE_INT16:
			*((INT16 *)pVar) = *((INT16 *)pItem->pVar);
			break;
		case VAR_TYPE_UINT8:
			*((UINT8 *)pVar) = *((UINT8 *)pItem->pVar);
			break;
		case VAR_TYPE_INT8:
			*((INT8 *)pVar) = *((INT8 *)pItem->pVar);
			break;
		}
	}

	pDesc->tParent.bOK = TRUE;
}
