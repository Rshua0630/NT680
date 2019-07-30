////////////////////////////////////////////////////////////////////////////////
#include "SysCommon.h"
//#include "AppCommon.h"
////////////////////////////////////////////////////////////////////////////////
#include "UIAppCommon.h"
#if(PHOTO_MODE==ENABLE)
#include "UIAppPhoto.h"
#endif
#include "UIAppMovie.h"
#include "NVTUserCommand.h"
#include "UIMode.h"
#include "ae_task.h"
#include "awb_task.h"
#include "af_task.h"
#include "af_api.h"
#include "SysCfg.h"
// Audio NR
#include "AudFilterAPI.h"
// Raw Encode
#include "UIAppMovie_AudNR.h"
#include "Lens.h"
#include "iq_api.h"
#if MOVIE_FD_FUNC_
#include "fd_lib.h"
#endif
#if(UI_FUNC==ENABLE)
#include "UIFlowMovieFuncs.h"
#endif
#include "ImageUnit_VdoOut.h"
#include "ImageUnit_VdoEnc.h"
#include "ImageApp_Movie.h"
#include "ImageApp_Photo.h"
#include "HwClock.h"
#include "gpio.h"
//#NT#2016/09/20#Bob Huang -begin
//#NT#Support HDMI Display with 3DNR Out
#if (_3DNROUT_FUNC == ENABLE)
#include "AppDisp_3DNRView.h"
#endif
//#NT#2016/09/20#Bob Huang -end
#include "videosprite.h"

#define __MODULE__          UIMovieCB
//#define __DBGLVL__ 0        //OFF mode, show nothing
#define __DBGLVL__ 1        //ERROR mode, show err, wrn only
//#define __DBGLVL__ 2        //TRACE mode, show err, wrn, ind, msg and func and ind, msg and func can be filtering by __DBGFLT__ settings
#define __DBGFLT__ "*"      //*=All
#include "DebugModule.h"


#define AUDNR_DELAYFRAME  2

extern BOOL _g_bFirstMovie;
extern BOOL _g_bFirstMovieRec;


UINT32 gAudNrIrisDelayFrame = AUDNR_DELAYFRAME;
UINT32 gAudNrAFDelayFrame = AUDNR_DELAYFRAME;
BOOL   gbAudNrZoomOperating = FALSE;
//UINT32 g_uiSeconds = 0;
extern UINT32 System_GetEnableSensor(void);
//#NT#2016/09/20#Bob Huang -begin
//#NT#Support HDMI Display with 3DNR Out
#if (_3DNROUT_FUNC == ENABLE)
static IMG_BUF  g3DNR_DispSrv_Src = {0};
extern BOOL     gb3DNROut;
static BOOL     gbFirstPreview = TRUE;
static UINT32   guiDispCount = 0;
#endif
//#NT#2016/09/20#Bob Huang -end



//#NT#2016/03/07#KCHong -begin
//#NT#Low power timelapse function
//#if (TIMELAPSE_LPR_FUNCTION == ENABLE)
//-------------------------------------------------------------------------------------------------
//
// Callback function for custom file naming rule
//
//-------------------------------------------------------------------------------------------------
static void UIMovie_SetCustomFileName(UINT32 uiPathId, char *pFileName)
{
//#NT#2016/08/12#Hideo Lin -begin
//#NT#For small size clone movie
#if (SMALL_CLONE_MOVIE == DISABLE)
	//#NT#2016/08/10#Brain Yen -begin
	//#NT#add for clone multi-record
	BOOL bCustomNameByDate = FALSE;
#if((MOVIE_MULTI_RECORD_FUNC == ENABLE) && defined(_UI_STYLE_SPORTCAM_))
	if (UI_GetData(FL_IsCloneRec) == CLONE_REC_ON) {
		bCustomNameByDate = TRUE;
	}
#endif
	//#NT#2016/08/10#Brain Yen -end
	if (System_GetState(SYS_STATE_CURRSUBMODE) == SYS_SUBMODE_WIFI || bCustomNameByDate == TRUE) {
		static struct tm   CurDateTime = {0};
		//let both files' name be the same
		if (uiPathId == 0) {
			CurDateTime = HwClock_GetTime(TIME_ID_CURRENT);
		}
		snprintf(pFileName, NMC_TOTALFILEPATH_MAX_LEN, "%04d%02d%02d_%02d%02d%02d", CurDateTime.tm_year, CurDateTime.tm_mon, CurDateTime.tm_mday, CurDateTime.tm_hour, CurDateTime.tm_min, CurDateTime.tm_sec);
	} else {
		if (uiPathId == 0) {
			sprintf(pFileName, "Timelapse0_Tmp");
		} else {
			sprintf(pFileName, "Timelapse1_Tmp");
		}
	}
#else
	static struct tm   CurDateTime = {0};
	static UINT32      uSetPathCnt = 0;
	static UINT32      uiPrevPathId = 0;

	if (((uSetPathCnt % 2) == 0) || (uiPrevPathId == uiPathId)) { //2016/12/30
		CurDateTime = HwClock_GetTime(TIME_ID_CURRENT);
		if (uiPrevPathId == uiPathId) {
			uSetPathCnt = 0;
		}
	}

	uSetPathCnt++;
	uiPrevPathId = uiPathId;

	if (uiPathId == 0) { // main movie
		snprintf(pFileName, NMC_TOTALFILEPATH_MAX_LEN, "%04d%02d%02d_%02d%02d%02d",
				 CurDateTime.tm_year, CurDateTime.tm_mon, CurDateTime.tm_mday,
				 CurDateTime.tm_hour, CurDateTime.tm_min, CurDateTime.tm_sec);
	} else { // clone movie
		snprintf(pFileName, NMC_TOTALFILEPATH_MAX_LEN, "%04d%02d%02d_%02d%02d%02d_S",
				 CurDateTime.tm_year, CurDateTime.tm_mon, CurDateTime.tm_mday,
				 CurDateTime.tm_hour, CurDateTime.tm_min, CurDateTime.tm_sec);
	}
#endif
//#NT#2016/08/12#Hideo Lin -end
}
//#NT#2016/03/07#KCHong -end

void UIMovie_MovieFileNamingCB(UINT32 uiPathId, char *pFileName)
{
	UIMovie_SetCustomFileName(uiPathId, pFileName);
}

void UIMovie_EMRMovFileNamingCB(UINT32 uiPathId, char *pFileName)
{
	UIMovie_SetCustomFileName(uiPathId, pFileName);
}

void UIMovie_PhotoFileNamingCB(UINT32 uiPathId, char *pFileName)
{
	UIMovie_SetCustomFileName(uiPathId, pFileName);
}
//#endif

//-------------------------------------------------------------------------------------------------
//
// Callback function for AF events
//
//-------------------------------------------------------------------------------------------------
void UIMovie_AFCB(AF_CBMSG uiMsgID, void *Data)
{
	switch (uiMsgID) {
	case AF_CBMSG_AF_START:
		DBG_IND("===AF Start! ===\r\n");
		break;
	case AF_CBMSG_AF_DONE:
		DBG_IND("===AF Done! ===\r\n");
		//AE_LOCK(FALSE);
		//AWB_LOCK(FALSE);
		//Ux_PostEvent(NVTEVT_ALGMSG_FOCUSEND,1, NVTEVT_ALGMSG_FOCUSEND);
		break;
	default:
		break;
	}
}

//-------------------------------------------------------------------------------------------------
//
// Callback function for IPL events
//
//-------------------------------------------------------------------------------------------------
extern BOOL bEnableAE;
extern BOOL bStableAE;
extern BOOL bEnableVdo;
extern BOOL bShowPreview;

void Movie_IPLCB(IPL_PROC_ID Id, IPL_CBMSG uiMsgID, void *Data)
{
	switch (uiMsgID) {
	case IPL_CBMSG_PREVIEW:
		DBG_IND("===IPL_CBMSG_PREVIEW===\r\n");
		if(!bEnableAE)
			bEnableAE = ae_isopen(AE_ID_1);
		if (_g_bFirstMovie) {
			TM_BOOT_BEGIN("sensor", "2a_stable");
		}
#if(UI_FUNC==ENABLE)
		Ux_PostEvent(NVTEVT_CALLBACK, 1, NVTEVT_ALGMSG_PREVIEW);
#endif
		break;
	case IPL_CBMSG_DZOOM:
		DBG_IND("===IPL_CBMSG_DZOOM===\r\n");
		// for Dzoom keep on moving
		//Ux_PostEvent(NVTEVT_CALLBACK,1,NVTEVT_ALGMSG_DZOOMSTEP);
		// post event for update UI
#if(UI_FUNC==ENABLE)
		Ux_PostEvent(NVTEVT_CB_ZOOM, 0);
#endif
		break;

	case IPL_CBMSG_SHUTTER_SOUND:
		//Photo_ShutterSoundCB();
		break;
	default:
		break;
	}
}

#include "ae_cb_msg.h"
static void Movie_AE_CB(AE_CBMSG uiMsgID, void *Data)
{
	switch (uiMsgID) {

	case AE_CBMSG_PREVIEWSTABLE:
		DBG_IND("===AE_CBMSG_PREVIEWSTABLE===\r\n");
		if (_g_bFirstMovie) {
			TM_BOOT_END("sensor", "2a_stable");
		}
		if (_g_bFirstMovie) {
			if (!_g_bFirstMovieRec) {
				_g_bFirstMovie = FALSE;
			}
			bStableAE = TRUE;
			if(bEnableVdo) {
				if(!bShowPreview) {
					bShowPreview = TRUE;
					Display_ShowPreview(); //show preview immediately (AE should be last one)
					DBG_DUMP("^YShow Preview! (IPL > Video VSync > AE stable)\r\n");
				}
			}
/*
			//#NT#2016/09/20#Bob Huang -begin
			//#NT#Support HDMI Display with 3DNR Out
#if (_3DNROUT_FUNC == ENABLE)
			if (!gb3DNROut)
#endif
				//#NT#2016/09/20#Bob Huang -end
			{
				Display_ShowPreview(); //show preview immediately
			}
*/
		}
#if(UI_FUNC==ENABLE)
		Ux_PostEvent(NVTEVT_CALLBACK, 1, NVTEVT_ALGMSG_PREVIEW_STABLE);
#endif
	default:
		break;
	}
}


#define STREAM_GET_QP     0 //fix coverity 30795 (uninit_use: Using uninitialized value uiParam[0])

#if STREAM_GET_QP
typedef struct {
	UINT32  P0_FrameType;// 0 : P slice, 1 : B slice , 2,3 : I or  IDR slice
	UINT32  P0_FrameQP;
	UINT32  P1_FrameType;// 0 : P slice, 1 : B slice , 2,3 : I or  IDR slice
	UINT32  P1_FrameQP;
	UINT32  P2_FrameType;// 0 : P slice, 1 : B slice , 2,3 : I or  IDR slice
	UINT32  P2_FrameQP;
	UINT32  P3_FrameType;// 0 : P slice, 1 : B slice , 2,3 : I or  IDR slice
	UINT32  P3_FrameQP;
	UINT32  P4_FrameType;// 0 : P slice, 1 : B slice , 2,3 : I or  IDR slice
	UINT32  P4_FrameQP;
} STREAM_QP_INFO;

STREAM_QP_INFO  g_Stream_QP_info;

void GetStreamQPInfo(UINT32 ID, UINT32 puiParam[2])
{
	if (ID == 0) {
		puiParam[0] = g_Stream_QP_info.P0_FrameType;
		puiParam[1] = g_Stream_QP_info.P0_FrameQP;

	} else if (ID == 1) {
		puiParam[0] = g_Stream_QP_info.P1_FrameType;
		puiParam[1] = g_Stream_QP_info.P1_FrameQP;

	} else if (ID == 2) {
		puiParam[0] = g_Stream_QP_info.P2_FrameType;
		puiParam[1] = g_Stream_QP_info.P2_FrameQP;

	} else if (ID == 3) {
		puiParam[0] = g_Stream_QP_info.P3_FrameType;
		puiParam[1] = g_Stream_QP_info.P3_FrameQP;

	}

}
#endif


//-------------------------------------------------------------------------------------------------
//
// Callback function for Capture events
//
//-------------------------------------------------------------------------------------------------
void Movie_CaptureCB(IMG_CAP_CBMSG Msg, void *Data)
{
	switch (Msg) {
	case IMG_CAP_CBMSG_GET_ONERAWEND:
		Ux_PostEvent(NVTEVT_CB_MOVIE_PIM_READY, 0);
		break;

	default:
		break;
	}
}

#if MOVIE_FD_FUNC_
void Movie_FdCB(FD_CBMSG uiMsgID, void *Data)
{
	switch (uiMsgID) {
	case FD_CBMSG_DETEND:
		DBG_IND("===FDEND===\r\n");
		//#NT#2016/06/08#Lincy Lin -begin
		//#NT#Implement generic OSD and video drawing mechanism for ALG function
		Ux_PostEvent(NVTEVT_EXE_MOVIE_ALGEND, 1, NVTEVT_EXE_MOVIE_ALGEND);
		//#NT#2016/06/08#Lincy Lin -end
		break;


	default:
		break;
	}
}
#endif

void UIMovie_IRCutCtrl(BOOL isON)
{
	// day mode
	if (isON == TRUE) {

	}
	// night mode
	else {

	}
}


void UIMovie_AeIRCutCB(UINT32 Id, IQ_IRCUT Value)
{
	if (Value == IQ_IRCUT_DAY) {
		UIMovie_IRCutCtrl(TRUE);
	} else if (Value == IQ_IRCUT_NIGHT) {
		UIMovie_IRCutCtrl(FALSE);
	} else {
		DBG_ERR("Value = %d\r\n", Value);
	}
}

void Movie_RegCB(void)
{
#if !defined(_IPL1_IPL_FAKE_)
	// Register AF callback function
	AF_RegisterCB(UIMovie_AFCB);
#if _TODO
	AE_SetIRCutFP(UIMovie_AeIRCutCB);
#endif
#endif
#if MOVIE_FD_FUNC_
	FD_RegisterCB(Movie_FdCB);
#endif

	if (System_GetEnableSensor() == (SENSOR_2)) {
		ImageUnit_Begin(ISF_IPL(1), 0);
		ImageUnit_SetParam(ISF_CTRL, IMAGEPIPE_PARAM_IPL_CB, (UINT32)Movie_IPLCB);
		ImageUnit_SetParam(ISF_CTRL, IMAGEPIPE_PARAM_VIDEOOSD_CB,  (UINT32)videosprite_draw_ps_image);
		ImageUnit_SetParam(ISF_CTRL, IMAGEPIPE_PARAM_VIDEOMASK_CB, (UINT32)videosprite_draw_ps_mask);
		ImageUnit_End();
		AE_CBMsgInit(IPL_PATH(1), Movie_AE_CB);
	} else {
		ImageUnit_Begin(ISF_IPL(0), 0);
		ImageUnit_SetParam(ISF_CTRL, IMAGEPIPE_PARAM_IPL_CB, (UINT32)Movie_IPLCB);
		ImageUnit_SetParam(ISF_CTRL, IMAGEPIPE_PARAM_VIDEOOSD_CB,  (UINT32)videosprite_draw_ps_image);
		ImageUnit_SetParam(ISF_CTRL, IMAGEPIPE_PARAM_VIDEOMASK_CB, (UINT32)videosprite_draw_ps_mask);
		ImageUnit_End();
		AE_CBMsgInit(IPL_PATH(0), Movie_AE_CB);
	}

#if (MOVIE_PIM_CAPTURE == ENABLE)
	ImageUnit_Begin(&ISF_Cap, 0);
	ImageUnit_SetParam(ISF_CTRL, CAP_PARAM_CAP_CB, (UINT32)Movie_CaptureCB);
	ImageUnit_SetParam(ISF_CTRL, CAP_PARAM_DETS2_CB, (UINT32)NULL);
	ImageUnit_End();
#endif

#if _TODO
	MovRec_Register3DNRCb(0, iq_get_tnr_config);
#endif
}

