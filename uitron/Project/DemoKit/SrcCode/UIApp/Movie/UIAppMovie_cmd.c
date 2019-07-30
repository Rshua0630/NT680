//global debug level: PRJ_DBG_LVL
#include "PrjCfg.h"

#include "SysCommon.h"
#include "SysCfg.h"
#include "AppLib.h"
#include "UIAppCommon.h"
#include "UIModeMovie.h"
#include "UIAppMovie.h"  // Isiah, implement YUV merge mode of recording func.
#include "UIAppMovie_Param.h"
#include "UIMovieMapping.h"

#include "UIModePhoto.h"
#include "UIAppPhoto.h"
#include "UIPhotoMapping.h"

//local debug level: THIS_DBGLVL
#define THIS_DBGLVL         2 // 0=FATAL, 1=ERR, 2=WRN, 3=UNIT, 4=FUNC, 5=IND, 6=MSG, 7=VALUE, 8=USER
///////////////////////////////////////////////////////////////////////////////
#define __MODULE__          UiAppMovieCmd
#define __DBGLVL__          ((THIS_DBGLVL>=PRJ_DBG_LVL)?THIS_DBGLVL:PRJ_DBG_LVL)
#define __DBGFLT__          "*" //*=All, [mark]=CustomClass
#include "DebugModule.h"
///////////////////////////////////////////////////////////////////////////////

static BOOL Cmd_movie(CHAR *strCmd)
{
	UINT32 menuIdx, value;

	sscanf_s(strCmd, "%d %d", &menuIdx, &value);

	if (menuIdx == 0) {
#if (MOVIE_REC_YUVMERGE == ENABLE)
		BOOL bEnable;
		UINT32 uiInterval;
		DBG_DUMP("Test merge record\r\n");
		sscanf_s(strCmd, "%d %d %d", &menuIdx, &bEnable, &uiInterval);
		FlowMovie_RecSetYUVMergeRecInterval(uiInterval);
		FlowMovie_RecSetYUVMergeRecCounter(0);
		FlowMovie_RecSetYUVMergeMode(bEnable);
		if (bEnable) {
			debug_msg("Enable YUV merge record mode, interval=%d\r\n", uiInterval);
		} else {
			debug_msg("Disable YUV merge record mode\r\n");
		}
#endif
	} else if (menuIdx == 1) {
#if(MOVIE_MODE==ENABLE)
		DBG_DUMP("ADAS(LDWS+FCW) %d\r\n", value);
		Ux_SendEvent(&CustomMovieObjCtrl,   NVTEVT_EXE_MOVIE_LDWS,          1,  value);
		Ux_SendEvent(&CustomMovieObjCtrl,   NVTEVT_EXE_MOVIE_FCW,           1,  value);
#endif
	} else if (menuIdx == 2) {
#if(MOVIE_MODE==ENABLE)
		DBG_DUMP("Urgent Protect Manual %d\r\n", value);
		Ux_SendEvent(&CustomMovieObjCtrl,   NVTEVT_EXE_MOVIE_PROTECT_MANUAL, 1,  value);
#endif
	} else if (menuIdx == 3) {
#if(MOVIE_MODE==ENABLE)
		DBG_DUMP("Image Rotation %d\r\n", value);
		Ux_SendEvent(&CustomMovieObjCtrl,   NVTEVT_EXE_MOVIE_SENSOR_ROTATE, 1,  value);
#endif
	} else if (menuIdx == 5) {
#if(MOVIE_MODE==ENABLE)
		DBG_DUMP("Movie RSC %d\r\n", value);
		Ux_SendEvent(&CustomMovieObjCtrl,   NVTEVT_EXE_MOVIE_RSC,          1,  value);
#endif
	} else if (menuIdx == 7) {
#if(PHOTO_MODE==ENABLE)
		UI_SetData(FL_DUAL_CAM, value);
		DBG_DUMP("Sensor index %d\r\n", value);
		Ux_SendEvent(&CustomMovieObjCtrl,   NVTEVT_EXE_DUALCAM,      1, SysGetFlag(FL_DUAL_CAM));
#endif
	} else if (menuIdx == 11) {
#if(MOVIE_MODE==ENABLE)
		DBG_DUMP("Movie WDR index %d\r\n", value);
		Ux_SendEvent(&CustomMovieObjCtrl, NVTEVT_EXE_MOVIE_WDR, 1, value);
#endif
	} else if (menuIdx == 13) {
#if (PIP_VIEW_LR == ENABLE)
		DBG_DUMP("enter value=%d\r\n", value);
		if (value >= DUALCAM_LR_16_9 && value <= DUALCAM_LR_BEHIND_FULL) {
		} else {
			value = DUALCAM_LR_16_9;
		}
#if(MOVIE_MODE==ENABLE)
		DBG_DUMP("out value=%d\r\n", value);
		UI_SetData(FL_DUAL_CAM, value);

		Ux_SendEvent(&CustomMovieObjCtrl, NVTEVT_EXE_IMAGE_RATIO, 1, GetMovieSizeRatio(UI_GetData(FL_MOVIE_SIZE)));
		PipView_SetStyle(UI_GetData(FL_DUAL_CAM));
#endif
#endif
	}

	return TRUE;
}

SXCMD_BEGIN(appmovie, "movie command")
SXCMD_ITEM("movie %", Cmd_movie, "movie mode test")
SXCMD_END()
