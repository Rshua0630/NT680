////////////////////////////////////////////////////////////////////////////////
#include "SysCommon.h"
//#include "AppCommon.h"
////////////////////////////////////////////////////////////////////////////////
#include "AppLib.h"
#include "UIMode.h"
#include "UIModePhoto.h"
#include "UIAppPhoto.h"
#include "UISetup.h"
//UIControl
#include "UIFrameworkExt.h"

//UIInfo
#include "UIInfo.h"
#include "UIFlow.h"
#include "UIModeWifi.h"
//#include "FlowPanInit.h"
#include "DeviceCtrl.h"

#if (CALIBRATION_FUNC == ENABLE)
#include "EngineerMode.h"
#endif

extern INT32 PhotoExe_OnOpen(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray);

int PRIMARY_MODE_PHOTO = -1;      ///< Photo
void ModePhoto_Open(void);
void ModePhoto_Close(void);

void ModePhoto_Open(void)
{
	Input_ResetMask();
	Ux_SetActiveApp(&CustomPhotoObjCtrl);
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
	    	Ux_OpenWindow((VControl *)(&UIFlowWndPhotoCtrl), 0);
		}
	} else {
	    SYS_SUBMODE *pCurSubMode = System_GetSubModeCtrl();
        if (pCurSubMode && pCurSubMode->pfSubMode_OnOpen) {
		    pCurSubMode->pfSubMode_OnOpen();
    		FlowPhoto_InitCfgSetting(); // Init Photo mode related parameters.
        }
	}
#endif

}
void ModePhoto_Close(void)
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
			Ux_CloseWindow((VControl *)(&UIFlowWndPhotoCtrl), 0);
		}
	} else {
	    SYS_SUBMODE *pCurSubMode = System_GetSubModeCtrl();
        if (pCurSubMode && pCurSubMode->pfSubMode_OnClose) {
		    pCurSubMode->pfSubMode_OnClose();
        }
	}
#endif

	Ux_SendEvent(0, NVTEVT_EXE_CLOSE, 0);
}

SYS_MODE gModePhoto = {
	"PHOTO",
	ModePhoto_Open,
	ModePhoto_Close,
	NULL,
	NULL,
	NULL,
	NULL
};
