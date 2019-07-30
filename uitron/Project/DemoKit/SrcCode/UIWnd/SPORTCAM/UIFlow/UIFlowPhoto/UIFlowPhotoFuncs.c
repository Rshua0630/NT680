#include <stdio.h>
#include "UIFramework.h"
#include "FileSysTsk.h"
#include "UIFlow.h"
#include "UIFlowPhotoParams.h"


#define THIS_DBGLVL         2 // 0=FATAL, 1=ERR, 2=WRN, 3=UNIT, 4=FUNC, 5=IND, 6=MSG, 7=VALUE, 8=USER
///////////////////////////////////////////////////////////////////////////////
#define __MODULE__          UIFlowPhotoFunc
#define __DBGLVL__          ((THIS_DBGLVL>=PRJ_DBG_LVL)?THIS_DBGLVL:PRJ_DBG_LVL)
#define __DBGFLT__          "*" //*=All, [mark]=CustomClass
#include "DebugModule.h"

PHOTO_TASK_DATA gPhotoData = { 0 };

static UINT32 g_uiFreePicNum        = 0;
static CHAR    g_cFreePicNumStr[8]     = {0};
static BOOL    g_SDStatus = FALSE;

CHAR *Get_FreePicNumString(UINT32 uiValue)
{
	snprintf(g_cFreePicNumStr, sizeof(g_cFreePicNumStr), "%05ld", uiValue);
	return g_cFreePicNumStr;
}

void FlowPhoto_DoCapture(void)
{
	switch (SysGetFlag(FL_CONTINUE_SHOT)) {
	case CONTINUE_SHOT_BURST_3:
	case CONTINUE_SHOT_BURST_30:
	case CONTINUE_SHOT_BURST:
		//UI_SetData(FL_ContShotIndex, CONT_SHOT_10);
#if (USE_FILEDB == DISABLE)
		// update DCF Folder/File name
		FlowDCF_UpdateName();
#endif
		Ux_SendEvent(&CustomPhotoObjCtrl, NVTEVT_EXE_CAPTURE_START, 1, ON_CAPTURE_CONT);
		break;
	case CONTINUE_SHOT_OFF:
	//#NT#2016/04/26#Lincy Lin -begin
	//#NT#Support sidebyside capture mode
	case CONTINUE_SHOT_SIDE:
	default:
		//#NT#2016/04/26#Lincy Lin -end
#if (USE_FILEDB == DISABLE)
		// update DCF Folder/File name
		FlowDCF_UpdateName();
#endif
		Ux_SendEvent(&CustomPhotoObjCtrl, NVTEVT_EXE_CAPTURE_START, 1, ON_CAPTURE_SINGLE);
		break;

	}
}

BOOL FlowPhoto_GetSDStatus(void)
{
	return g_SDStatus;
}

void FlowPhoto_SetFdSdProc(BOOL sd_start)
{
	g_SDStatus = sd_start;
	DBG_IND("FL_FD = %d\r\n", SysGetFlag(FL_FD));
	switch (SysGetFlag(FL_FD)) {
	case FD_OFF:

		Ux_SendEvent(&CustomPhotoObjCtrl, NVTEVT_EXE_FD, 1, FD_OFF);
		//Ux_SendEvent(&CustomPhotoObjCtrl, NVTEVT_EXE_CAPTURE_SMILE, 1, SD_STOP);
		break;
	case FD_ON:

		Ux_SendEvent(&CustomPhotoObjCtrl, NVTEVT_EXE_FD, 1, FD_ON);
		//Ux_SendEvent(&CustomPhotoObjCtrl, NVTEVT_EXE_CAPTURE_SMILE, 1, SD_STOP);
		break;
#if 0
	case FD_SMILE:
		Ux_SendEvent(&CustomPhotoObjCtrl, NVTEVT_EXE_FD, 1, FD_ON);
		if (sd_start == TRUE) {
			/* Set smile detection start flag */
			Ux_SendEvent(&CustomPhotoObjCtrl, NVTEVT_EXE_CAPTURE_SMILE, 1, SD_START);
		} else {
			/* Set smile detection start flag */
			Ux_SendEvent(&CustomPhotoObjCtrl, NVTEVT_EXE_CAPTURE_SMILE, 1, SD_CONTINUE);
		}
		// restore FL_FD flag to FD_SMILE
		SysSetFlag(FL_FD, FD_SMILE);

		break;
#endif
	}

}

void FlowPhoto_ForceLockFdSd(void)
{
	//Ux_SendEvent(&CustomPhotoObjCtrl, NVTEVT_EXE_CAPTURE_SMILE, 1, SD_STOP);
}

UINT32 FlowPhoto_GetFreePicNum(void)
{
	return g_uiFreePicNum;
}

BOOL FlowPhoto_IsStorageErr(void)
{
	BOOL    retV = FALSE;

	if (UIStorageCheck(STORAGE_CHECK_ERROR, NULL) == TRUE) {
		Ux_OpenWindow(&UIFlowWndWrnMsgCtrl, 2, FLOWWRNMSG_ISSUE_MEM_ERR, FLOWWRNMSG_TIMER_KEEP);
		return TRUE;
	}
	if (UIStorageCheck(STORAGE_CHECK_FULL, &(g_uiFreePicNum)) == TRUE) {
		if (System_GetState(SYS_STATE_CARD)  == CARD_REMOVED) {
			Ux_OpenWindow(&UIFlowWndWrnMsgCtrl, 2, UIFlowWndWrnMsg_StatusTXT_Msg_STRID_PLEASE_INSERT_SD, FLOWWRNMSG_TIMER_2SEC);
		} else {
			Ux_OpenWindow(&UIFlowWndWrnMsgCtrl, 2, UIFlowWndWrnMsg_StatusTXT_Msg_STRID_CARD_FULL, FLOWWRNMSG_TIMER_2SEC);
		}
		return TRUE;
	}
	if (UIStorageCheck(STORAGE_CHECK_LOCKED, NULL) == TRUE) {
		Ux_OpenWindow(&UIFlowWndWrnMsgCtrl, 2, UIFlowWndWrnMsg_StatusTXT_Msg_STRID_CARD_LOCKED, FLOWWRNMSG_TIMER_2SEC);
		return TRUE;
	}

	return retV;
}

void FlowPhoto_OnTimer01SecIndex(void)
{

	if ((gPhotoData.SelfTimerCount % 10 == 0) && (gPhotoData.SelfTimerCount != 0)) {
		FlowPhoto_IconDrawSelftimerTime(&UIFlowWndPhoto_StaticTXT_SelftimerCntCtrl, gPhotoData.SelfTimerCount / 10);
		UISound_Play(DEMOSOUND_SOUND_KEY_TONE);
	}

	if (gPhotoData.SelfTimerCount == 0) {
		UISound_Play(DEMOSOUND_SOUND_KEY_TONE);
		FlowPhoto_IconHideSelftimer(&UIFlowWndPhoto_StatusICN_SelftimerCtrl);
		FlowPhoto_IconHideSelftimerTime(&UIFlowWndPhoto_StaticTXT_SelftimerCntCtrl);

		/* Clear the whole OSD screen */
		UxCtrl_SetAllChildShow(&UIFlowWndPhotoCtrl, FALSE);

		gPhotoData.State = PHOTO_ST_CAPTURE;
		FlowPhoto_DoCapture();
	}
}

void FlowPhoto_OnTimer1SecIndex(void)
{
	switch (gPhotoData.State) {
	case PHOTO_ST_VIEW:
	case PHOTO_ST_VIEW | PHOTO_ST_ZOOM:
	case PHOTO_ST_SELFTIMER:
		gPhotoData.SysTimeCount++;
		break;
	}
}

void FlowPhoto_InitCfgSetting(void)
{
	UIPhotoSettingParamsInfo *pParamsInfo = NULL;
	UINT32 uhCurrScene = SysGetFlag(FL_SCENE);
	UINT32 i;

	for (i = 0; i < TOTAL_PHOTO_PARAMS_SETS; i++) {
		if (uhCurrScene == UIPhotoParams[i].uiSceneType) {
			pParamsInfo = (UIPhotoSettingParamsInfo *)&UIPhotoParams[i];
			DBG_IND("SCENE i=%d, %s\r\n", i, pParamsInfo->pNameString);
			break;
		}
	}
	//localInfo->pParams = pParamsInfo;
	if (!pParamsInfo) {
		DBG_ERR("No setting parameters info!!!\r\n");
		return;
	}


	/* Update current photo mode setting object */
	DBG_IND("### Photo Setting: %s\r\n", (CHAR *)pParamsInfo->pNameString);
	SetPhotoParamsInfo(pParamsInfo);
	//localInfo->Releasekeymask = pParamsInfo->uiInitMaskKeyRelease;

	/* Init setting functions, get init setting flag */
	UI_SetData(FL_IsInitSettingFlag, TRUE);

	Ux_SendEvent(&CustomPhotoObjCtrl, NVTEVT_EXE_SCENEMODE, 2, uhCurrScene, 0);

	Ux_SendEvent(&CustomPhotoObjCtrl, NVTEVT_EXE_FLASH, 1, ((pParamsInfo->uhIdxFlash != PHOTOPARAMS_USER_DEFINED) ?
				 pParamsInfo->uhIdxFlash : Photo_GetUserIndex(PHOTO_USR_FLASH)));

	Ux_SendEvent(&CustomPhotoObjCtrl, NVTEVT_EXE_EV,        1, ((pParamsInfo->uhIdxEV != PHOTOPARAMS_USER_DEFINED) ?
				 pParamsInfo->uhIdxEV : Photo_GetUserIndex(PHOTO_USR_EV)));

	Ux_SendEvent(&CustomPhotoObjCtrl, NVTEVT_EXE_MACRO,     1, ((pParamsInfo->uhIdxMacro != PHOTOPARAMS_USER_DEFINED) ?
				 pParamsInfo->uhIdxMacro : UI_GetData(FL_MACRO)));

	Ux_SendEvent(&CustomPhotoObjCtrl, NVTEVT_EXE_SELFTIMER,     1, ((pParamsInfo->uhIdxSelftimer != PHOTOPARAMS_USER_DEFINED) ?
				 pParamsInfo->uhIdxSelftimer : UI_GetData(FL_SELFTIMER)));

	Ux_SendEvent(&CustomPhotoObjCtrl, NVTEVT_EXE_AFWINDOW, 1, ((pParamsInfo->uhIdxAFWindow != PHOTOPARAMS_USER_DEFINED) ?
				 pParamsInfo->uhIdxAFWindow : Photo_GetUserIndex(PHOTO_USR_AFWINDOW)));

	Ux_SendEvent(&CustomPhotoObjCtrl, NVTEVT_EXE_AFBEAM, 1, ((pParamsInfo->uhIdxAFBeam != PHOTOPARAMS_USER_DEFINED) ?
				 pParamsInfo->uhIdxAFBeam : Photo_GetUserIndex(PHOTO_USR_AFBEAM)));

	Ux_SendEvent(&CustomPhotoObjCtrl, NVTEVT_EXE_METERING, 1, (pParamsInfo->uhIdxMetering != PHOTOPARAMS_USER_DEFINED) ?
				 pParamsInfo->uhIdxMetering : Photo_GetUserIndex(PHOTO_USR_METERING));

	Ux_SendEvent(&CustomPhotoObjCtrl, NVTEVT_EXE_SHARPNESS, 1, (pParamsInfo->uhIdxSharpness != PHOTOPARAMS_USER_DEFINED) ?
				 pParamsInfo->uhIdxSharpness : Photo_GetUserIndex(PHOTO_USR_SHARPNESS));

	Ux_SendEvent(&CustomPhotoObjCtrl, NVTEVT_EXE_SATURATION, 1, pParamsInfo->uhIdxSaturation);

	Ux_SendEvent(&CustomPhotoObjCtrl, NVTEVT_EXE_COLOR, 1, (pParamsInfo->uhIdxColor != PHOTOPARAMS_USER_DEFINED) ?
				 pParamsInfo->uhIdxColor : Photo_GetUserIndex(PHOTO_USR_COLOR));

	Ux_SendEvent(&CustomPhotoObjCtrl, NVTEVT_EXE_CAPTURE_SIZE, 1, (pParamsInfo->uhIdxSize != PHOTOPARAMS_USER_DEFINED) ?
				 pParamsInfo->uhIdxSize : Photo_GetUserIndex(PHOTO_USR_SIZE));


	Ux_SendEvent(&CustomPhotoObjCtrl, NVTEVT_EXE_QUALITY, 1, (pParamsInfo->uhIdxQuality != PHOTOPARAMS_USER_DEFINED) ?
				 pParamsInfo->uhIdxQuality : Photo_GetUserIndex(PHOTO_USR_QUALITY));

	Ux_SendEvent(&CustomPhotoObjCtrl, NVTEVT_EXE_ISO, 1, (pParamsInfo->uhIdxISO != PHOTOPARAMS_USER_DEFINED) ?
				 pParamsInfo->uhIdxISO : Photo_GetUserIndex(PHOTO_USR_ISO));

	Ux_SendEvent(&CustomPhotoObjCtrl, NVTEVT_EXE_WB, 1, (pParamsInfo->uhIdxWB != PHOTOPARAMS_USER_DEFINED) ?
				 pParamsInfo->uhIdxWB : Photo_GetUserIndex(PHOTO_USR_WB));

	Ux_SendEvent(&CustomPhotoObjCtrl, NVTEVT_EXE_RSC,       1, SysGetFlag(FL_RSC));
	Ux_SendEvent(&CustomPhotoObjCtrl, NVTEVT_EXE_EDGE,      1, SysGetFlag(FL_EDGE));
	Ux_SendEvent(&CustomPhotoObjCtrl, NVTEVT_EXE_NR,        1, SysGetFlag(FL_NR));
	Ux_SendEvent(&CustomPhotoObjCtrl, NVTEVT_EXE_DATE_PRINT,  1,  SysGetFlag(FL_DATE_STAMP));

	Ux_SendEvent(&CustomPhotoObjCtrl, NVTEVT_EXE_IMAGE_RATIO, 1, GetPhotoSizeRatio(UI_GetData(FL_PHOTO_SIZE)));

	Ux_SendEvent(&CustomPhotoObjCtrl, NVTEVT_EXE_WDR,       1, SysGetFlag(FL_WDR));

	Ux_SendEvent(&CustomPhotoObjCtrl, NVTEVT_EXE_DEFOG,       1, SysGetFlag(FL_DEFOG));

	SysSetFlag(FL_Dzoom, DZOOM_ON);
	// set FL_CONTINUE_SHOT to off
	//SysSetFlag(FL_CONTINUE_SHOT,CONTINUE_SHOT_OFF);

	/* Setting functions init completed, free init setting flag */
	UI_SetData(FL_IsInitSettingFlag, FALSE);
}
