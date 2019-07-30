
#include "UIFramework.h"
#include "UIFrameworkExt.h"
#include "UIFlow.h"
#include "JpgEnc.h"
#include "Exif.h"
#include "Utility.h"
#include "Audio.h"



static UINT32  g_uiFreePicNum = 0;
static CHAR    g_cSelftimerCntStr[8]   = {0};

static CHAR *Get_SelftimerCntString(UINT32 uiCount)
{
	snprintf(g_cSelftimerCntStr, sizeof(g_cSelftimerCntStr), "%ld", uiCount);
	return g_cSelftimerCntStr;
}

void FlowPhoto_IconDrawDSCMode(VControl *pCtrl)
{
	UxCtrl_SetShow(pCtrl, TRUE);
}

void FlowPhoto_IconHideDSCMode(VControl *pCtrl)
{
	UxCtrl_SetShow(pCtrl, FALSE);
}

void FlowPhoto_IconDrawSize(VControl *pCtrl)
{
	UxStatic_SetData(&UIFlowWndPhoto_StaticTXT_SizeCtrl, STATIC_VALUE, Txt_Pointer(Get_SizeString(SysGetFlag(FL_PHOTO_SIZE))));
	UxCtrl_SetShow(pCtrl, TRUE);
}

void FlowPhoto_IconHideSize(VControl *pCtrl)
{
	UxCtrl_SetShow(pCtrl, FALSE);
}

void FlowPhoto_IconDrawQuality(VControl *pCtrl)
{
	UxState_SetData(pCtrl, STATE_CURITEM, SysGetFlag(FL_QUALITY));
	UxCtrl_SetShow(pCtrl, TRUE);
}

void FlowPhoto_IconHideQuality(VControl *pCtrl)
{
	UxCtrl_SetShow(pCtrl, FALSE);
}

void FlowPhoto_IconDrawAntishake(VControl *pCtrl)
{
	if (SysGetFlag(FL_ANTISHAKE) == ANTISHAKE_OFF) {
		UxCtrl_SetShow(pCtrl, FALSE);
	} else {
		UxState_SetData(pCtrl, STATE_CURITEM, UIFlowWndPhoto_StatusICN_AntiShaking_ICON_SHAKE_ON);
		UxCtrl_SetShow(pCtrl, TRUE);
	}
}

void FlowPhoto_IconHideAntishake(VControl *pCtrl)
{
	UxCtrl_SetShow(pCtrl, FALSE);
}

void FlowPhoto_IconDrawFreePicNumber(VControl *pCtrl)
{
	UIStorageCheck(STORAGE_CHECK_FULL, &g_uiFreePicNum);
	UxStatic_SetData(pCtrl, STATIC_VALUE, Txt_Pointer(Get_FreePicNumString(g_uiFreePicNum)));
	UxCtrl_SetShow(pCtrl, TRUE);
}

void FlowPhoto_IconHideFreePicNumber(VControl *pCtrl)
{
	UxCtrl_SetShow(pCtrl, FALSE);
}

void FlowPhoto_IconDrawStorage(VControl *pCtrl)
{
	/* Update status item data */
	if (System_GetState(SYS_STATE_CARD)  == CARD_REMOVED) {
		UxState_SetData(pCtrl, STATE_CURITEM, UIFlowWndPhoto_StatusICN_Storage_ICON_INTERNAL_FLASH);
	} else if (System_GetState(SYS_STATE_CARD)  == CARD_LOCKED) {
		UxState_SetData(pCtrl, STATE_CURITEM, UIFlowWndPhoto_StatusICN_Storage_ICON_SD_LOCK);
	} else {
		UxState_SetData(pCtrl, STATE_CURITEM, UIFlowWndPhoto_StatusICN_Storage_ICON_SD_CARD);
	}

	UxCtrl_SetShow(pCtrl, TRUE);
}

void FlowPhoto_IconHideStorage(VControl *pCtrl)
{
	UxCtrl_SetShow(pCtrl, FALSE);
}

void FlowPhoto_IconDrawContinueShot(VControl *pCtrl)
{
	//#NT#2016/04/26#Lincy Lin -begin
	//#NT#Support sidebyside capture mode
	if (SysGetFlag(FL_CONTINUE_SHOT) == CONTINUE_SHOT_OFF || SysGetFlag(FL_CONTINUE_SHOT) == CONTINUE_SHOT_SIDE)
		//#NT#2016/04/26#Lincy Lin -end
	{
		UxCtrl_SetShow(pCtrl, FALSE);
	} else {
		UxState_SetData(pCtrl, STATE_CURITEM, SysGetFlag(FL_CONTINUE_SHOT));
		UxCtrl_SetShow(pCtrl, TRUE);
	}
}

void FlowPhoto_IconHideContinueShot(VControl *pCtrl)
{
	UxCtrl_SetShow(pCtrl, FALSE);
}

void FlowPhoto_IconDrawSelftimer(VControl *pCtrl)
{
	BOOL bShow;

	UxState_SetData(pCtrl, STATE_CURITEM, SysGetFlag(FL_SELFTIMER));
	bShow = (SysGetFlag(FL_SELFTIMER) == 0) ? FALSE : TRUE;
	UxCtrl_SetShow(pCtrl, bShow);
}

void FlowPhoto_IconHideSelftimer(VControl *pCtrl)
{
	UxCtrl_SetShow(pCtrl, FALSE);
}

void FlowPhoto_IconDrawSelftimerTime(VControl *pCtrl, UINT32 time)
{
	UxStatic_SetData(&UIFlowWndPhoto_StaticTXT_SelftimerCntCtrl, STATIC_VALUE,
					 Txt_Pointer(Get_SelftimerCntString(time)));

	UxCtrl_SetShow(pCtrl, TRUE);
}

void FlowPhoto_IconHideSelftimerTime(VControl *pCtrl)
{
	UxCtrl_SetShow(pCtrl, FALSE);
}

void FlowPhoto_IconDrawWB(VControl *pCtrl)
{
	UxState_SetData(pCtrl, STATE_CURITEM, SysGetFlag(FL_WB));
	UxCtrl_SetShow(pCtrl, TRUE);
}

void FlowPhoto_IconHideWB(VControl *pCtrl)
{
	UxCtrl_SetShow(pCtrl, FALSE);
}

void FlowPhoto_IconDrawEV(VControl *pCtrl)
{
	UxState_SetData(pCtrl, STATE_CURITEM, SysGetFlag(FL_EV));
	UxCtrl_SetShow(pCtrl, TRUE);
}

void FlowPhoto_IconHideEV(VControl *pCtrl)
{
	UxCtrl_SetShow(pCtrl, FALSE);
}

void FlowPhoto_IconDrawISO(VControl *pCtrl)
{
	UxState_SetData(&UIFlowWndPhoto_StatusICN_ISOCtrl, STATE_CURITEM, SysGetFlag(FL_ISO));
	UxCtrl_SetShow(pCtrl, TRUE);
}

void FlowPhoto_IconHideISO(VControl *pCtrl)
{
	UxCtrl_SetShow(pCtrl, FALSE);
}

void FlowPhoto_IconDrawFD(VControl *pCtrl)
{
	if (SysGetFlag(FL_FD) == FD_OFF) {
		UxCtrl_SetShow(pCtrl, FALSE);
	} else {
		UxState_SetData(pCtrl, STATE_CURITEM, UIFlowWndPhoto_StatusICN_FD_ICON_FACE_ON);
		UxCtrl_SetShow(pCtrl, TRUE);
	}
}

void FlowPhoto_IconHideFD(VControl *pCtrl)
{
	UxCtrl_SetShow(pCtrl, FALSE);
}

void FlowPhoto_IconDrawBattery(VControl *pCtrl)
{
	UxState_SetData(pCtrl, STATE_CURITEM, GetBatteryLevel());
	UxCtrl_SetShow(pCtrl, TRUE);
}

void FlowPhoto_IconHideBattery(VControl *pCtrl)
{
	UxCtrl_SetShow(pCtrl, FALSE);
}

void FlowPhoto_IconDrawDZoom(VControl *pCtrl)
{
	BOOL   bShow;

	UxStatic_SetData(pCtrl, STATIC_VALUE, Txt_Pointer(Get_DZoomRatioString()));
	bShow = (DZOOM_IDX_GET() > DZOOM_IDX_MIN()) ? TRUE : FALSE;
	UxCtrl_SetShow(pCtrl, bShow);
}

void FlowPhoto_IconHideDZoom(VControl *pCtrl)
{
	UxCtrl_SetShow(pCtrl, FALSE);
}

void FlowPhoto_IconDrawHDR(VControl *pCtrl)
{
	UxState_SetData(pCtrl, STATE_CURITEM, SysGetFlag(FL_SHDR));
	UxCtrl_SetShow(pCtrl, TRUE);

	if (SysGetFlag(FL_SHDR) == SHDR_OFF) {
		UxCtrl_SetShow(pCtrl, FALSE);
	} else {
		UxState_SetData(pCtrl, STATE_CURITEM, UIFlowWndPhoto_StatusICN_HDR_ICON_HDR);
		UxCtrl_SetShow(pCtrl, TRUE);
	}
}

void FlowPhoto_IconHideHDR(VControl *pCtrl)
{
	UxCtrl_SetShow(pCtrl, FALSE);
}

void FlowPhoto_UpdateIcons(BOOL bShow)
{

	if (bShow == FALSE || SysGetFlag(FL_LCD_DISPLAY) == DISPOUT_OFF) {
		FlowPhoto_IconHideDSCMode(&UIFlowWndPhoto_StaticICN_DSCModeCtrl);
		FlowPhoto_IconHideSize(&UIFlowWndPhoto_StaticTXT_SizeCtrl);
		FlowPhoto_IconHideQuality(&UIFlowWndPhoto_StatusICN_QualityCtrl);
		FlowPhoto_IconHideAntishake(&UIFlowWndPhoto_StatusICN_AntiShakingCtrl);
		FlowPhoto_IconHideFreePicNumber(&UIFlowWndPhoto_StaticTXT_FreePicCtrl);
		FlowPhoto_IconHideStorage(&UIFlowWndPhoto_StatusICN_StorageCtrl);
		FlowPhoto_IconHideContinueShot(&UIFlowWndPhoto_StatusICN_ContinueShotCtrl);
		FlowPhoto_IconHideSelftimer(&UIFlowWndPhoto_StatusICN_SelftimerCtrl);
		FlowPhoto_IconHideWB(&UIFlowWndPhoto_StatusICN_WBCtrl);
		FlowPhoto_IconHideEV(&UIFlowWndPhoto_StatusICN_EVCtrl);
		FlowPhoto_IconHideISO(&UIFlowWndPhoto_StatusICN_ISOCtrl);
		FlowPhoto_IconHideFD(&UIFlowWndPhoto_StatusICN_FDCtrl);
		FlowPhoto_IconHideBattery(&UIFlowWndPhoto_StatusICN_BatteryCtrl);
		FlowPhoto_IconHideDZoom(&UIFlowWndPhoto_StaticTXT_DZoomCtrl);
		FlowPhoto_IconHideHDR(&UIFlowWndPhoto_StatusICN_HDRCtrl);
	} else {
		FlowPhoto_IconDrawDSCMode(&UIFlowWndPhoto_StaticICN_DSCModeCtrl);
		FlowPhoto_IconDrawSize(&UIFlowWndPhoto_StaticTXT_SizeCtrl);
		FlowPhoto_IconDrawQuality(&UIFlowWndPhoto_StatusICN_QualityCtrl);
		FlowPhoto_IconDrawAntishake(&UIFlowWndPhoto_StatusICN_AntiShakingCtrl);
		FlowPhoto_IconDrawFD(&UIFlowWndPhoto_StatusICN_FDCtrl);
		FlowPhoto_IconDrawFreePicNumber(&UIFlowWndPhoto_StaticTXT_FreePicCtrl);
		FlowPhoto_IconDrawStorage(&UIFlowWndPhoto_StatusICN_StorageCtrl);
		FlowPhoto_IconDrawContinueShot(&UIFlowWndPhoto_StatusICN_ContinueShotCtrl);
		FlowPhoto_IconDrawSelftimer(&UIFlowWndPhoto_StatusICN_SelftimerCtrl);
		FlowPhoto_IconHideSelftimerTime(&UIFlowWndPhoto_StaticTXT_SelftimerCntCtrl);
		FlowPhoto_IconDrawWB(&UIFlowWndPhoto_StatusICN_WBCtrl);
		FlowPhoto_IconDrawEV(&UIFlowWndPhoto_StatusICN_EVCtrl);
		FlowPhoto_IconDrawISO(&UIFlowWndPhoto_StatusICN_ISOCtrl);
		FlowPhoto_IconDrawBattery(&UIFlowWndPhoto_StatusICN_BatteryCtrl);
		FlowPhoto_IconDrawDZoom(&UIFlowWndPhoto_StaticTXT_DZoomCtrl);
		FlowPhoto_IconDrawHDR(&UIFlowWndPhoto_StatusICN_HDRCtrl);

	}

}
