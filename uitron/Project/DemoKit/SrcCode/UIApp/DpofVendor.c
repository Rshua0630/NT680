/**
    DPOF sample code
    Just sample code to show how to use DPOF lib.
    @file       ExifVendor.c
    @ingroup    Predefined_group_name
    @note       Nothing (or anything need to be mentioned).

    Copyright   Novatek Microelectronics Corp. 2011.  All rights reserved.
*/
#include <string.h>
#include "SysKer.h"
#include "PrjCfg.h"
#include "DpofVendor.h"
//#include "DPOF.h"
#include "DCF.h"

static UINT32 g_DpofBufAddr = 0;
static BOOL   g_bDpofOpened = FALSE;

void DpofVendor_Open(UINT32 buffAddr)
{
	if (g_bDpofOpened == FALSE) {
		g_DpofBufAddr = buffAddr;
		DPOF_Open(buffAddr);
		g_bDpofOpened = TRUE;
	} else {
		debug_err(("^R DPOF has been opened!\r\n"));
	}
}
void DpofVendor_Close(void)
{
	if (g_bDpofOpened) {
		DPOF_Close(DPOF_NO_UPDATE);
		g_bDpofOpened = FALSE;
	} else {
		debug_err(("^R DpofVendor not open yet!\r\n"));
	}
}
void DpofVendor_Update(void)
{
	if (g_bDpofOpened == FALSE) {
		debug_err(("^R DpofVendor not open yet!\r\n"));
		return;
	}
	DPOF_Close(DPOF_UPDATE);
	DPOF_Open(g_DpofBufAddr);
}
void DpofVendor_SetDPOFQty(DPOF_OP_MODE DpofOPMode, UINT32 DpofPrtNum, BOOL DpofDateOn)
{
	UINT32 DirID, FilenameID;
	DirID = DCF_GetDBInfo(DCF_INFO_CUR_DIR_ID);
	FilenameID = DCF_GetDBInfo(DCF_INFO_CUR_FILE_ID);
	if (g_bDpofOpened == FALSE) {
		debug_err(("^R DpofVendor not open yet!\r\n"));
		return;
	}
	switch (DpofOPMode) {
	case DPOF_OP_SET_CURR:
		if ((DPOF_SetQty(FilenameID, DpofPrtNum, DirID) == DPOF_OK) && DpofPrtNum) {
			if (DpofDateOn) {
				DPOF_SetOneDateStr(FilenameID, TRUE, DirID, 0);
			} else {
				DPOF_SetOneDateStr(FilenameID, FALSE, DirID, 0);
			}
		}
		break;

	case DPOF_OP_SET_ALL:
		if (DpofPrtNum) {
			DPOF_SetAll(TRUE, DpofPrtNum, DpofDateOn);
		} else {
			DPOF_DeleteAll_and_DeleteMISC();
		}
		break;
	default:
		debug_err(("^R DpofVendor_SetCurrDPOFQty Parameter error!\r\n"));
		return;
	}
	DpofVendor_Update();

}
UINT32 DpofVendor_GetDPOFInfo(DPOF_INFO DpofInfo)
{
	UINT32 Value;
	DPOF_INFO_OBJ DPOFInfo;
	UINT32 DirID, FilenameID;
	DirID = DCF_GetDBInfo(DCF_INFO_CUR_DIR_ID);
	FilenameID = DCF_GetDBInfo(DCF_INFO_CUR_FILE_ID);
	switch (DpofInfo) {
	case DPOF_INFO_CURR_QTY:
		DPOF_GetDPOFInfo(FilenameID, DirID, &DPOFInfo);
		Value = DPOFInfo.uiQty;
		break;
	case DPOF_INFO_CURR_DATE_STATUS:
		DPOF_GetDPOFInfo(FilenameID, DirID, &DPOFInfo);
		Value = DPOFInfo.uiDSC;
		break;
	case DPOF_INFO_IF_ALL_IMG_SET_TO_DPOF:
		Value = DPOF_GetAll();
		break;
	default:
		debug_err(("^R DpofVendor_GetDPOFInfo Parameter error!\r\n"));
		Value = 0;
		break;
	}
	return Value;
}

void DpofVendor_DPOFSetCreatorName(char *pRetVal, UINT32 length, BOOL bUpdateNow)
{
	UINT8 retV;
	if (g_bDpofOpened == FALSE) {
		debug_err(("^R DpofVendor not open yet!\r\n"));
		return;
	}
	retV = DPOF_AccessHeaderItem(DPOF_HDR_CRT, DPOF_SET, pRetVal, length);
	if ((retV == DPOF_OK) && bUpdateNow) {
		DpofVendor_Update();
	}
	return;
}

