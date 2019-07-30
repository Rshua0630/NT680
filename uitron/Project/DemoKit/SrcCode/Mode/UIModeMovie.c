////////////////////////////////////////////////////////////////////////////////
#include "SysCommon.h"
//#include "AppCommon.h"
////////////////////////////////////////////////////////////////////////////////
#include "AppLib.h"
#include "UIMode.h"
#include "UIModeMovie.h"
#include "UIAppMovie.h"
#include "UISetup.h"
//UIControl
#include "UIFrameworkExt.h"

//UIWnd
#if(UI_FUNC==ENABLE)
#include "UIFlow.h"
#endif
#if(WIFI_FUNC==ENABLE)
#include "UIModeWifi.h"
#endif

//UIInfo
#include "UIInfo.h"

#if (CALIBRATION_FUNC == ENABLE)
#include "EngineerMode.h"
#endif

////////////////////////////////////////////////////////////////////////////////

#include "DeviceCtrl.h"

extern INT32 MovieExe_OnOpen(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray);

int PRIMARY_MODE_MOVIE = -1;      ///< Movie

void ModeMovie_Open(void);
void ModeMovie_Close(void);

void ModeMovie_Open(void)
{
	Input_ResetMask();
	Ux_SetActiveApp(&CustomMovieObjCtrl);
	Ux_SendEvent(0, NVTEVT_EXE_OPEN, 0);

	UI_SetData(FL_ModeIndex, UI_GetData(FL_NextMode));
	/*
	    //disable video1
	    UI_Show(UI_SHOW_PREVIEW, TRUE);
	    //enable video2
	    UI_Show(UI_SHOW_QUICKVIEW, FALSE);
	*/
#if(UI_FUNC==ENABLE)
    if (System_GetState(SYS_STATE_CURRSUBMODE) == SYS_SUBMODE_NORMAL) {
#if (CALIBRATION_FUNC == ENABLE)
		if (IsEngineerModeOpened()) {
			EngineerMode_WndOpen();
		}
		else
#endif
		{
			gMovData.State = MOV_ST_VIEW;
			Ux_OpenWindow((VControl *)(&UIFlowWndMovieCtrl), 0);
			//#NT#2016/03/07#KCHong -begin
			//#NT#Low power timelapse function
#if (TIMELAPSE_LPR_FUNCTION == ENABLE)
			if (MovieTLLPR_CheckHWRTStatus() == TL_HWRT_BOOT_ALARM) {
				MovieTLLPR_Process(TIMELAPSE_FROM_HWRT);
			}
#endif
			//#NT#2016/03/07#KCHong -end
		}
	} else {
	    SYS_SUBMODE *pCurSubMode = System_GetSubModeCtrl();
        if (pCurSubMode && pCurSubMode->pfSubMode_OnOpen) {
		    pCurSubMode->pfSubMode_OnOpen();
	    }
    }

#endif
}
void ModeMovie_Close(void)
{
#if(UI_FUNC==ENABLE)
    if (System_GetState(SYS_STATE_CURRSUBMODE) == SYS_SUBMODE_NORMAL) {
#if (CALIBRATION_FUNC == ENABLE)
		if (IsEngineerModeOpened()) {
			EngineerMode_WndClose();
		}
		else
#endif
		{
			Ux_CloseWindow((VControl *)(&UIFlowWndMovieCtrl), 0);
		}
	} else {
	    SYS_SUBMODE *pCurSubMode = System_GetSubModeCtrl();
        if (pCurSubMode && pCurSubMode->pfSubMode_OnClose) {
		    pCurSubMode->pfSubMode_OnClose();
        }
	}

#if (MOVIE_DIS_COMP_FUNC == ENABLE)
	Ux_SendEvent(&CustomMovieObjCtrl, NVTEVT_EXE_MOVIEDIS_ENABLE, 1, 0);
#endif
#if 0
	{
    	UINT32 i;

		for (i = 0; i < 500; i++) {
			if (!BKG_GetTskBusy()) {
				break;
			}
			SwTimer_DelayMs(100);
		}
		if (i == 500) {
			debug_err(("^R Wait background task idle timeout \r\n"));
		}
	}
#else
	if (BKG_GetTskBusy()) {
		debug_err(("BKG_GetTskBusy\r\n"));
	}
#endif
#endif
	Ux_SendEvent(0, NVTEVT_EXE_CLOSE, 0);
}

SYS_MODE gModeMovie = {
	"MOVIE",
	ModeMovie_Open,
	ModeMovie_Close,
	NULL,
	NULL,
	NULL,
	NULL
};
