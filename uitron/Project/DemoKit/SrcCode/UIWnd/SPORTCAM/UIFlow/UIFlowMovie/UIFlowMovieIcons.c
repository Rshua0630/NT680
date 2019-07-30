
#include "UIFramework.h"
#include "UIFrameworkExt.h"
#include "UIFlow.h"
#include "JpgEnc.h"
#include "Exif.h"
#include "Utility.h"
#include "Audio.h"
#include "HwClock.h"
#include "Perf.h"
#include "SysExam_cmd.h"
//#include "PhotoTsk.h"

static CHAR    g_RecMaxTimeStr[20] = {0};
static UINT32  g_RecMaxTime = 0;
static CHAR    g_RecCurrTimeStr[20] = {0};
static UINT32  g_RecCurrTime = 0;
static CHAR    date_str[20] = {0};
static CHAR    time_str[20] = {0};

//MOVIE_SIZE_TAG
static CHAR    *resolution_Buf[MOVIE_SIZE_ID_MAX] = {
	[MOVIE_SIZE_FRONT_2880x2160P50] = "UHD P50",
	[MOVIE_SIZE_FRONT_3840x2160P30] = "UHD P30",
	[MOVIE_SIZE_FRONT_2880x2160P24] = "UHD P24",
	[MOVIE_SIZE_FRONT_2704x2032P60] = "2.7K P60",
	[MOVIE_SIZE_FRONT_2560x1440P80] = "QHD P80",
	[MOVIE_SIZE_FRONT_2560x1440P60] = "QHD P60",
	[MOVIE_SIZE_FRONT_2560x1440P30] = "QHD P30",
	[MOVIE_SIZE_FRONT_2304x1296P30] = "3MHD P30",
	[MOVIE_SIZE_FRONT_1920x1080P120] = "FHD P120",
	[MOVIE_SIZE_FRONT_1920x1080P96] = "FHD P96",
	[MOVIE_SIZE_FRONT_1920x1080P60] = "FHD P60",
	[MOVIE_SIZE_FRONT_1920x1080P30] = "FHD P30",
	[MOVIE_SIZE_FRONT_1280x720P120] = "HD P120",
	[MOVIE_SIZE_FRONT_1280x720P240] = "HD P240",
	[MOVIE_SIZE_FRONT_1280x720P60] = "HD P60",
	[MOVIE_SIZE_FRONT_1280x720P30] = "HD P30",
	[MOVIE_SIZE_FRONT_848x480P30] = "WVGA P30",
	[MOVIE_SIZE_FRONT_640x480P240] = "VGA P240",
	[MOVIE_SIZE_FRONT_640x480P30] = "VGA P30",
	[MOVIE_SIZE_FRONT_320x240P30] = "QVGA P30",
	[MOVIE_SIZE_DUAL_1920x1080P30_1920x1080P30] = "FHD P30+FHD P30",
	[MOVIE_SIZE_TRI_1920x1080P30] = "FHD P30 x 3",
	[MOVIE_SIZE_QUAD_1920x1080P30] = "FHD P30 x 4",
	[MOVIE_SIZE_CLONE_1920x1080P30_1920x1080P30] = "FHD P30+FHD P30",
	[MOVIE_SIZE_CLONE_1920x1080P30_1280x720P30] = "FHD P30+HD P30",
	[MOVIE_SIZE_CLONE_2560x1440P30_848x480P30] = "QHD P30+WVGA P30",
	[MOVIE_SIZE_CLONE_2304x1296P30_848x480P30] = "3MHD P30+WVGA P30",
	[MOVIE_SIZE_CLONE_1920x1080P60_848x480P30] = "FHD P60+WVGA P30",
	[MOVIE_SIZE_CLONE_1920x1080P60_640x360P30] = "FHD P60+VGA P30",
	[MOVIE_SIZE_CLONE_1920x1080P30_848x480P30] = "FHD P30+WVGA P30",
	[MOVIE_SIZE_CLONE_2048x2048P30_480x480P30] = "2048x2048 P30 + 480x480 P30",
};

void FlowMovie_IconDrawDscMode(VControl *pCtrl)
{
	UxCtrl_SetShow(pCtrl, TRUE);
}

void FlowMovie_IconHideDscMode(VControl *pCtrl)
{
	UxCtrl_SetShow(pCtrl, FALSE);
}

void FlowMovie_IconDrawMaxRecTime(VControl *pCtrl)
{
	if (GxStrg_GetDevice(0) == NULL) {
		//No Storage, direct to return.
		return;
	}
	TM_BOOT_BEGIN("movie", "GetMaxRecSec");

	memset((void *)g_RecMaxTimeStr, 0, sizeof(g_RecMaxTimeStr));
	g_RecMaxTime = MovieExe_GetMaxRecSec();

	if (g_RecMaxTime == 0) {
		FlowMovie_SetRecMaxTime(0);
	} else if (g_RecMaxTime > 86399) {
		///23:59:59
		FlowMovie_SetRecMaxTime(86399);
	} else {
		FlowMovie_SetRecMaxTime(g_RecMaxTime);
	}

	if (System_GetState(SYS_STATE_CARD)  == CARD_REMOVED) {
		snprintf(g_RecMaxTimeStr, 20, "%02d:%02d:%02d", 0, 0, 0);
	} else {
		snprintf(g_RecMaxTimeStr, 20, "%02d:%02d:%02d", g_RecMaxTime / 3600, (g_RecMaxTime % 3600) / 60, (g_RecMaxTime % 3600) % 60);
	}
	TM_BOOT_END("movie", "GetMaxRecSec");
	SysExam_SetTimeTag(SYSEXAM_TMTAG_RECORD_RDY);

	UxStatic_SetData(pCtrl, STATIC_VALUE, Txt_Pointer(g_RecMaxTimeStr));
	UxCtrl_SetShow(pCtrl, TRUE);
}

void FlowMovie_IconHideMaxRecTime(VControl *pCtrl)
{
	UxCtrl_SetShow(pCtrl, FALSE);
}

void FlowMovie_IconDrawRecTime(VControl *pCtrl)
{
//#NT#2016/03/07#KCHong -begin
//#NT#Low power timelapse function
#if (TIMELAPSE_LPR_FUNCTION == ENABLE)
	UINT32 TimelapseMode = 0;
#endif
//#NT#2016/03/07#KCHong -end

	switch (gMovData.State) {
	case MOV_ST_REC:
	case MOV_ST_REC | MOV_ST_ZOOM:
		//#NT#2016/03/07#KCHong -begin
		//#NT#Low power timelapse function
#if (TIMELAPSE_LPR_FUNCTION == ENABLE)
		if (MovieTLLPR_GetStatus() == (TL_FLOW_LPR | TL_STATE_RECORD)) {
			TimelapseMode = UI_GetData(FL_MOVIE_TIMELAPSE_REC);
			if (TimelapseMode >= TL_LPR_TIME_MIN_PERIOD) {
				if (TimelapseMode == MOVIE_TIMELAPSEREC_OFF) {
					snprintf(g_RecCurrTimeStr, 20, "OFF");
				} else if (TimelapseMode == MOVIE_TIMELAPSEREC_1SEC) {
					snprintf(g_RecCurrTimeStr, 20, "1 SEC");
				} else if (TimelapseMode == MOVIE_TIMELAPSEREC_5SEC) {
					snprintf(g_RecCurrTimeStr, 20, "5 SEC");
				} else if (TimelapseMode == MOVIE_TIMELAPSEREC_10SEC) {
					snprintf(g_RecCurrTimeStr, 20, "10 SEC");
				} else if (TimelapseMode == MOVIE_TIMELAPSEREC_30SEC) {
					snprintf(g_RecCurrTimeStr, 20, "30 SEC");
				} else if (TimelapseMode == MOVIE_TIMELAPSEREC_1MIN) {
					snprintf(g_RecCurrTimeStr, 20, "1 MIN");
				} else if (TimelapseMode == MOVIE_TIMELAPSEREC_5MIN) {
					snprintf(g_RecCurrTimeStr, 20, "5 MIN");
				} else if (TimelapseMode == MOVIE_TIMELAPSEREC_10MIN) {
					snprintf(g_RecCurrTimeStr, 20, "10 MIN");
				} else if (TimelapseMode == MOVIE_TIMELAPSEREC_30MIN) {
					snprintf(g_RecCurrTimeStr, 20, "30 MIN");
				} else if (TimelapseMode == MOVIE_TIMELAPSEREC_1HOUR) {
					snprintf(g_RecCurrTimeStr, 20, "1 HOUR");
				} else if (TimelapseMode == MOVIE_TIMELAPSEREC_2HOUR) {
					snprintf(g_RecCurrTimeStr, 20, "2 HOUR");
				} else if (TimelapseMode == MOVIE_TIMELAPSEREC_3HOUR) {
					snprintf(g_RecCurrTimeStr, 20, "3 HOUR");
				} else if (TimelapseMode == MOVIE_TIMELAPSEREC_1DAY) {
					snprintf(g_RecCurrTimeStr, 20, "1 DAY");
				}
				UxStatic_SetData(pCtrl, STATIC_VALUE, Txt_Pointer(g_RecCurrTimeStr));
				UxCtrl_SetShow(pCtrl, TRUE);
				break;
			}
		}
#endif
		//#NT#2016/03/07#KCHong -end
		g_RecCurrTime = FlowMovie_GetRecCurrTime();
		memset((void *)g_RecCurrTimeStr, 0, sizeof(g_RecCurrTimeStr));
		snprintf(g_RecCurrTimeStr, 20, "%02d:%02d:%02d", g_RecCurrTime / 3600, (g_RecCurrTime % 3600) / 60, (g_RecCurrTime % 3600) % 60);
		UxStatic_SetData(pCtrl, STATIC_VALUE, Txt_Pointer(g_RecCurrTimeStr));
		UxCtrl_SetShow(pCtrl, TRUE);
		break;
	}
}

void FlowMovie_IconDrawDateTime(void)
{
    struct tm Curr_DateTime;
    Curr_DateTime = HwClock_GetTime(TIME_ID_CURRENT);

	// display Date/Time string in movie mode
	snprintf(date_str, 20, "%04d/%02d/%02d", Curr_DateTime.tm_year, Curr_DateTime.tm_mon, Curr_DateTime.tm_mday);
	snprintf(time_str, 20, "%02d:%02d:%02d", Curr_DateTime.tm_hour, Curr_DateTime.tm_min, Curr_DateTime.tm_sec);
	UxStatic_SetData(&UIFlowWndMovie_YMD_StaticCtrl, STATIC_VALUE, Txt_Pointer(date_str));
	UxStatic_SetData(&UIFlowWndMovie_HMS_StaticCtrl, STATIC_VALUE, Txt_Pointer(time_str));
	UxCtrl_SetShow(&UIFlowWndMovie_YMD_StaticCtrl, TRUE);
	UxCtrl_SetShow(&UIFlowWndMovie_HMS_StaticCtrl, TRUE);
}

void FlowMovie_IconHideDateTime(void)
{
	UxCtrl_SetShow(&UIFlowWndMovie_YMD_StaticCtrl, FALSE);
	UxCtrl_SetShow(&UIFlowWndMovie_HMS_StaticCtrl, FALSE);
}

void FlowMovie_IconDrawRec(VControl *pCtrl)
{
	UxState_SetData(pCtrl, STATE_CURITEM, UIFlowWndMovie_Status_REC_ICON_REC_TRANSPAENT);
}

void FlowMovie_IconHideRec(VControl *pCtrl)
{
	UxState_SetData(pCtrl, STATE_CURITEM, UIFlowWndMovie_Status_REC_ICON_REC_TRANSPAENT);
}

void FlowMovie_IconDrawSize(VControl *pCtrl)
{
	UxStatic_SetData(pCtrl, STATIC_VALUE, Txt_Pointer(resolution_Buf[SysGetFlag(FL_MOVIE_SIZE)]));
	UxCtrl_SetShow(pCtrl, TRUE);
}

void FlowMovie_IconHideSize(VControl *pCtrl)
{
	UxCtrl_SetShow(pCtrl, FALSE);
}

void FlowMovie_IconDrawEV(VControl *pCtrl)
{
	UxState_SetData(pCtrl, STATE_CURITEM, SysGetFlag(FL_EV));
	UxCtrl_SetShow(pCtrl, TRUE);
}

void FlowMovie_IconHideEV(VControl *pCtrl)
{
	UxCtrl_SetShow(pCtrl, FALSE);
}

void FlowMovie_IconDrawStorage(VControl *pCtrl)
{
	/* Update status item data */
	if (System_GetState(SYS_STATE_CARD)  == CARD_REMOVED) {
		UxState_SetData(pCtrl, STATE_CURITEM, UIFlowWndMovie_Status_Storage_ICON_INTERNAL_FLASH);
	} else if (System_GetState(SYS_STATE_CARD)  == CARD_LOCKED) {
		UxState_SetData(pCtrl, STATE_CURITEM, UIFlowWndMovie_Status_Storage_ICON_SD_LOCK);
	} else {
		UxState_SetData(pCtrl, STATE_CURITEM, UIFlowWndMovie_Status_Storage_ICON_SD_CARD);
	}

	UxCtrl_SetShow(pCtrl, TRUE);
}

void FlowMovie_IconHideStorage(VControl *pCtrl)
{
	UxCtrl_SetShow(pCtrl, FALSE);
}

void FlowMovie_IconDrawCyclicRec(VControl *pCtrl)
{
	UxState_SetData(pCtrl, STATE_CURITEM, SysGetFlag(FL_MOVIE_CYCLIC_REC));
	UxCtrl_SetShow(pCtrl, TRUE);
}

void FlowMovie_IconHideCyclicRec(VControl *pCtrl)
{
	UxCtrl_SetShow(pCtrl, FALSE);
}

void FlowMovie_IconDrawHDR(VControl *pCtrl)
{
	UxState_SetData(pCtrl, STATE_CURITEM, SysGetFlag(FL_MOVIE_HDR));
	UxCtrl_SetShow(pCtrl, TRUE);
}

void FlowMovie_IconHideHDR(VControl *pCtrl)
{
	UxCtrl_SetShow(pCtrl, FALSE);
}


void FlowMovie_IconDrawMotionDet(VControl *pCtrl)
{
	UxState_SetData(pCtrl, STATE_CURITEM, SysGetFlag(FL_MOVIE_MOTION_DET));
	UxCtrl_SetShow(pCtrl, TRUE);
}

void FlowMovie_IconHideMotionDet(VControl *pCtrl)
{
	UxCtrl_SetShow(pCtrl, FALSE);
}

void FlowMovie_IconDrawBattery(VControl *pCtrl)
{
	UxState_SetData(pCtrl, STATE_CURITEM, GetBatteryLevel());
	UxCtrl_SetShow(pCtrl, TRUE);
}

void FlowMovie_IconHideBattery(VControl *pCtrl)
{
	UxCtrl_SetShow(pCtrl, FALSE);
}

void FlowMovie_IconDrawDZoom(VControl *pCtrl)
{
	BOOL   bShow;

	UxStatic_SetData(pCtrl, STATIC_VALUE, Txt_Pointer(Get_DZoomRatioString()));
	bShow = (DZOOM_IDX_GET() > DZOOM_IDX_MIN()) ? TRUE : FALSE;
	UxCtrl_SetShow(pCtrl, bShow);
}

void FlowMovie_IconHideDZoom(VControl *pCtrl)
{
	UxCtrl_SetShow(pCtrl, FALSE);
}

void FlowMovie_DrawPIM(BOOL bDraw)
{
	UxCtrl_SetShow(&UIFlowWndMovie_StaticIcon_PIMCCtrl, bDraw);
}


void FlowMovie_UpdateIcons(BOOL bShow)
{
	if (bShow == FALSE || SysGetFlag(FL_LCD_DISPLAY) == DISPOUT_OFF) {
		FlowMovie_IconHideDscMode(&UIFlowWndMovie_Static_cameraCtrl);
		FlowMovie_IconHideSize(&UIFlowWndMovie_Static_resolutionCtrl);
		FlowMovie_IconHideMaxRecTime(&UIFlowWndMovie_Static_maxtimeCtrl);
		FlowMovie_IconHideRec(&UIFlowWndMovie_Status_RECCtrl);
		FlowMovie_IconHideStorage(&UIFlowWndMovie_Status_StorageCtrl);
		FlowMovie_IconHideCyclicRec(&UIFlowWndMovie_Status_CyclicRecCtrl);
		FlowMovie_IconHideMotionDet(&UIFlowWndMovie_Status_MotionDetCtrl);
		FlowMovie_IconHideDZoom(&UIFlowWndMovie_Zoom_StaticCtrl);
		FlowMovie_IconHideBattery(&UIFlowWndMovie_Status_batteryCtrl);
		FlowMovie_IconHideEV(&UIFlowWndMovie_StatusICN_EVCtrl);
		FlowMovie_IconHideDateTime();
		FlowMovie_DrawPIM(FALSE);
	} else {
	    FlowMovie_IconDrawDscMode(&UIFlowWndMovie_Static_cameraCtrl);
		FlowMovie_IconDrawSize(&UIFlowWndMovie_Static_resolutionCtrl);
		FlowMovie_IconDrawMaxRecTime(&UIFlowWndMovie_Static_maxtimeCtrl);
		FlowMovie_IconHideRec(&UIFlowWndMovie_Status_RECCtrl);
		FlowMovie_IconDrawStorage(&UIFlowWndMovie_Status_StorageCtrl);
		FlowMovie_IconDrawCyclicRec(&UIFlowWndMovie_Status_CyclicRecCtrl);
		FlowMovie_IconDrawHDR(&UIFlowWndMovie_Status_HDRCtrl);
		FlowMovie_IconDrawMotionDet(&UIFlowWndMovie_Status_MotionDetCtrl);
		FlowMovie_IconDrawDZoom(&UIFlowWndMovie_Zoom_StaticCtrl);
		FlowMovie_IconDrawBattery(&UIFlowWndMovie_Status_batteryCtrl);
		FlowMovie_IconDrawEV(&UIFlowWndMovie_StatusICN_EVCtrl);
		FlowMovie_IconDrawDateTime();
		FlowMovie_DrawPIM(FALSE);
	}
}

