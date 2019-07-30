
#include "UIFramework.h"
#include "UIFrameworkExt.h"
#include "Debug.h"
#include "UIAppMoviePlay.h"
#include "PlaybackTsk.h"
#include "AppLib.h"
#include "GxVideoFile.h"
#include "DCF.h"
#include "SMediaPlayAPI.h"
#include "UIMovieMapping.h"
#include "UIInfo.h"

#define THIS_DBGLVL         2 // 0=FATAL, 1=ERR, 2=WRN, 3=UNIT, 4=FUNC, 5=IND, 6=MSG, 7=VALUE, 8=USER
///////////////////////////////////////////////////////////////////////////////
#define __MODULE__          UiAppMoviePlay
#define __DBGLVL__          ((THIS_DBGLVL>=PRJ_DBG_LVL)?THIS_DBGLVL:PRJ_DBG_LVL)
#define __DBGFLT__          "*" //*=All, [mark]=CustomClass
//#define __DBGFLT__          "[cap]"
//#define __DBGFLT__          "[cb]"
//#define __DBGFLT__          "[set]"
//#define __DBGFLT__          "[zoom]"
//#define __DBGFLT__          "[af]"
#include "DebugModule.h"


#define MoviePlay_Open(x)                        SMediaPlay_Open((PSMEDIAPLAY_USEROBJ)x)
#define MoviePlay_WaitReady(x)                   SMediaPlay_WaitReady(x)
#define MoviePlay_Close(x)                       SMediaPlay_Close(x)
#define MoviePlay_Pause(x)                       SMediaPlay_PausePlay(x)
#define MoviePlay_GetMediaInfo(x)                SMediaPlay_GetMediaInfo(x)
#define MoviePlay_GetCurrFrame(x)                SMediaPlay_GetCurrVideoFrame(x)
#define MoviePlay_GetNowDisplayFrame(x,y,z)      SMediaPlay_GetNowDisplayFrame(x,y,z)
#define MoviePlay_StartPlay(x,y)                 SMediaPlay_StartPlay(x,y)
#define MoviePlay_FWDOneStep(x)                  SMediaPlay_VideoOneStepPlay(SMEDIAPLAY_DIR_FORWARD)
#define MoviePlay_BWDOneStep(x)                  SMediaPlay_VideoOneStepPlay(SMEDIAPLAY_DIR_BACKWARD)
#define MoviePlay_GetVideoFrameRate(x)           SMediaPlay_GetVideoFrameRate(x)
#define MoviePlay_GetTotalVideoFrame(x)          SMediaPlay_GetTotalVideoFrame(x)
#define MoviePlay_GetCreateModifyTime(x,y)       SMediaPlay_GetCreateModifyTime(x,y)
#define MoviePlay_GetThisVideoFrameOffset(x,y)   SMediaPlay_GetThisVideoFrameOffset(x,y)
#define MoviePlay_JumpVideoToSpecificFrame(x)    SMediaPlay_JumpVideoToSpecFramePlay(x)
#define MoviePlay_RestartTo1stFrame(x)           SMediaPlay_Restart1stFrmPlay(x)
#define MoviePlay_SetOneDispFrameEvent(x)
#define MoviePlay_Video_Step(x)
#define MoviePlay_WaitStepFinish(x)
#define MoviePlay_SetAudVolume(x)                SMediaPlay_SetAudVolume(x)
#define MoviePlay_GetCurrPlaySecs(x)             SMediaPlay_GetCurrPlaySecs(x)


GXVIDEO_INFO g_MovieInfo;
UINT32 MoviePlayExe_LibGetTotalSecs(void)
{
	PB_GetParam(PBPRMID_INFO_VDO, (UINT32 *)&g_MovieInfo);

	return (g_MovieInfo.uiToltalSecs);
}
UINT32 MoviePlayExe_LibGetEV(void)
{
	return 0;
}
UINT32 MoviePlayExe_LibGetVideoWidth(void)
{
	PB_GetParam(PBPRMID_INFO_VDO, (UINT32 *)&g_MovieInfo);

	return (g_MovieInfo.uiVidWidth);
}

INT32 MoviePlayExe_OpenPlay(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
	INT32       retV = 0;
	static SMEDIAPLAY_USEROBJ MediaPlayObj;

	DBG_IND("+pN=0x%x\r\n", paramNum);
	if (paramNum == 0) {
		DBG_ERR("paramenter wrong\r\n");
		return E_PAR;
	}

	// Close
	MoviePlay_Close();

	memcpy((void *)&MediaPlayObj, (void *)*paramArray, sizeof(SMEDIAPLAY_USEROBJ));

	// Open
	MoviePlay_Open(&MediaPlayObj);
	retV = MoviePlay_WaitReady();

	if (retV != E_OK) {
		DBG_ERR("Failed!!\r\n");
		MoviePlay_Close();
		return retV;
	}

	return retV;
}

INT32 MoviePlayExe_ClosePlay(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
	MoviePlay_Pause();
	MoviePlay_Close();

	return NVTEVT_CONSUME;
}

INT32 MoviePlayExe_PausePlay(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
	MoviePlay_Pause();
	return NVTEVT_CONSUME;
}

INT32 MoviePlayExe_StartPlay(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
	UINT32 uiSpeedLevel, direction;

	if (paramNum != 2) {
		return NVTEVT_CONSUME;
	}

	uiSpeedLevel = paramArray[0];
	direction    = paramArray[1];

	MoviePlay_StartPlay(uiSpeedLevel, direction);

	return NVTEVT_CONSUME;
}

UINT32 MoviePlayExe_GetDataPlay(PLAYMOVIE_GET_DATATYPE dataType)
{
	UINT32 retV = 0;
	switch (dataType) {
#if (USE_DCF == ENABLE)
	case PLAYMOVIE_DIRID:
		retV = DCF_GetDBInfo(DCF_INFO_CUR_DIR_ID);
		break;
	case PLAYMOVIE_FILEID:
		retV = DCF_GetDBInfo(DCF_INFO_CUR_FILE_ID);
		break;
#endif
	case PLAYMOVIE_TOTAL_SECOND:
		retV = MoviePlayExe_LibGetTotalSecs();
		break;
	case PLAYMOVIE_EV:
		retV = MoviePlayExe_LibGetEV();
		break;
	case PLAYMOVIE_WIDTH:
		retV = MoviePlayExe_LibGetVideoWidth();
		break;
#if (USE_DCF == ENABLE)
	case PLAYMOVIE_LOCKSTATUS: {
			UINT32 CurIndex = 0, DirectoryID = 0, FilenameID = 0, uiThisFileFormat = 0;
			CHAR   filePath[DCF_FULL_FILE_PATH_LEN];
			BOOL   ReturnValue = FALSE;
			UINT8  uAttrib = 0;

			CurIndex = DCF_GetDBInfo(DCF_INFO_CUR_INDEX);
			DCF_GetObjInfo(CurIndex, &DirectoryID, &FilenameID, &uiThisFileFormat);
			DCF_GetObjPath(CurIndex, uiThisFileFormat, filePath);
			FileSys_GetAttrib(filePath, &uAttrib);
			if (M_IsReadOnly(uAttrib)) {
				ReturnValue = TRUE;
			} else {
				ReturnValue = FALSE;
			}
			retV = ReturnValue;
		}
		break;
	case PLAYMOVIE_FILENUM:
		retV = DCF_GetDBInfo(DCF_INFO_TOL_FILE_COUNT);
		break;
#endif
	case PLAYMOVIE_OPENED:
		retV = TRUE; // Always return true.
		break;
	case PLAYMOVIE_CURR_SECOND:
		retV = MoviePlay_GetCurrPlaySecs();
		break;
	case PLAYMOVIE_CURR_FRAME:
		retV = MoviePlay_GetCurrFrame();
		break;
	case PLAYMOVIE_TOTAL_FRAME:
		retV = MoviePlay_GetTotalVideoFrame();
		break;

	default:
		DBG_IND("Unknown type=%d\r\n", dataType);
		break;
	}
	DBG_IND("-type=0x%x,v=0x%x\r\n", dataType, retV);
	return retV;
}

INT32 MoviePlayExe_SwitchFirstFrame(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
	MoviePlay_RestartTo1stFrame();
	return NVTEVT_CONSUME;
}

INT32 MoviePlayExe_PlayStepFwd(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
	MoviePlay_FWDOneStep();
	return NVTEVT_CONSUME;
}
INT32 MoviePlayExe_PlayStepBwd(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
	MoviePlay_BWDOneStep();
	return NVTEVT_CONSUME;
}

INT32 MoviePlayExe_PlayFwd(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
	return NVTEVT_CONSUME;
}
INT32 MoviePlayExe_PlayBwd(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
	return NVTEVT_CONSUME;
}
INT32 MoviePlayExe_SwitchLastFrame(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
	return NVTEVT_CONSUME;
}
void MoviePlayExe_SetDataPlay(PLAYMOVIE_SET_DATATYPE dataType, UINT32 data)
{
	return;
}

INT32 MoviePlayExe_ChangeSize(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
	return NVTEVT_CONSUME;
}

INT32 MoviePlayExe_OnAudPlayVolume(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
	UINT32 uhSelect = 0;
	BOOL  bChangHWSetting = FALSE;

	if (paramNum == 1) { //save index only
		uhSelect = paramArray[0];
	} else if (paramNum == 2 && paramArray[1] == 1) { //save index and change HW setting
		uhSelect = paramArray[0];
		bChangHWSetting = TRUE;
	} else {
		DBG_ERR("paramNum error!\r\n");
		return NVTEVT_CONSUME;
	}
	DBG_IND("+-idx=%d,val=%d\r\n", uhSelect, GetMovieAudioVolumeValue(uhSelect));
	//if (UI_GetData(FL_MovieAudioPlayIndex) != uhSelect)
	{
		UI_SetData(FL_MovieAudioPlayIndex, uhSelect);
		if (bChangHWSetting) {
			MoviePlay_SetAudVolume(GetMovieAudioVolumeValue(uhSelect));
		}
	}
	return NVTEVT_CONSUME;
}


////////////////////////////////////////////////////////////

EVENT_ENTRY CustomMoviePlayObjCmdMap[] = {
	{NVTEVT_EXE_OPENPLAY,                   MoviePlayExe_OpenPlay},
	{NVTEVT_EXE_CLOSEPLAY,                  MoviePlayExe_ClosePlay},
	{NVTEVT_EXE_PAUSEPLAY,                  MoviePlayExe_PausePlay},
	{NVTEVT_EXE_STARTPLAY,                  MoviePlayExe_StartPlay},
	{NVTEVT_EXE_FWDPLAY,                    MoviePlayExe_PlayFwd},
	{NVTEVT_EXE_BWDPLAY,                    MoviePlayExe_PlayBwd},
	{NVTEVT_EXE_FWDSTEPPLAY,                MoviePlayExe_PlayStepFwd},
	{NVTEVT_EXE_BWDSTEPPLAY,                MoviePlayExe_PlayStepBwd},
	{NVTEVT_EXE_SWITCH_FIRSTFRAME,          MoviePlayExe_SwitchFirstFrame},
	{NVTEVT_EXE_SWITCH_LASTFRAME,           MoviePlayExe_SwitchLastFrame},
	{NVTEVT_EXE_CHANGESIZE,                 MoviePlayExe_ChangeSize},
	{NVTEVT_EXE_MOVIEAUDPLAYVOLUME,         MoviePlayExe_OnAudPlayVolume},
	{NVTEVT_NULL,                           0},  //End of Command Map
};

CREATE_APP(CustomMoviePlayObj, APP_SETUP)


