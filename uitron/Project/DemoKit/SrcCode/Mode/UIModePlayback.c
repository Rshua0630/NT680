////////////////////////////////////////////////////////////////////////////////
#include "SysCommon.h"
//#include "AppCommon.h"
////////////////////////////////////////////////////////////////////////////////
#include "AppLib.h"
#include "UIMode.h"
#include "UIAppPlay.h"
#include "UISetup.h"
//UIControl
#include "UIFrameworkExt.h"

//UIWnd
//#include "FlowPlay.h"
#include "UIFlow.h"
#include "UIModeWifi.h"
//UIInfo
#include "UIPlayInfo.h"
#include "UIInfo.h"

#include "DeviceCtrl.h"
#include "SMediaPlayAPI.h"

#define  USE_ROT_TASK 0


int PRIMARY_MODE_PLAYBACK = -1;      ///< Playback

void ModePlay_Open(void);
void ModePlay_Close(void);

void ModePlay_Open(void)
{
	Input_ResetMask();
	Ux_SetActiveApp(&CustomPlayObjCtrl);
	Ux_SendEvent(0, NVTEVT_EXE_OPEN, 0);

	UI_SetData(FL_ModeIndex, UI_GetData(FL_NextMode));
#if(UI_FUNC==ENABLE)
    if (System_GetState(SYS_STATE_CURRSUBMODE) == SYS_SUBMODE_NORMAL) {
	    Ux_OpenWindow((VControl *)(&UIFlowWndPlayCtrl), 0);
	} else {
	    SYS_SUBMODE *pCurSubMode = System_GetSubModeCtrl();
        if (pCurSubMode && pCurSubMode->pfSubMode_OnOpen) {
		    pCurSubMode->pfSubMode_OnOpen();
        }
	}
#endif

	GxVideo_SetDeviceCtrl(DOUT1, DISPLAY_DEVCTRL_BACKLIGHT, TRUE);
}
void ModePlay_Close(void)
{

	//#NT#2010/11/29#Ben Wang -begin
	//#NT#Close root win by function instead of by win name because FlowPlayCtrl may not always be the root win.
	//Ux_CloseWindowClear((VControl *)(&FlowPlayCtrl), 0);
	//Ux_CloseWindow((VControl *)(&FlowPlayCtrl), 0);
#if(UI_FUNC==ENABLE)
    if (System_GetState(SYS_STATE_CURRSUBMODE) == SYS_SUBMODE_NORMAL) {
	    Ux_CloseWindow((VControl *)(&UIFlowWndPlayCtrl), 0);
	} else {
	    SYS_SUBMODE *pCurSubMode = System_GetSubModeCtrl();
        if (pCurSubMode && pCurSubMode->pfSubMode_OnClose) {
		    pCurSubMode->pfSubMode_OnClose();
        }
	}
#endif
	//set default play by name
	//if(PRIMARY_MODE_PLAYBACK != System_GetState(SYS_STATE_NEXTMODE))
	//    UI_SetData(FL_PlayIKeyModeIndex, PLAY_BY_NAME);
	//#NT#2010/11/29#Ben Wang -end
	Ux_SendEvent(0, NVTEVT_EXE_CLOSE, 0);

}

SYS_MODE gModePlay = {
	"PLAYBACK",
	ModePlay_Open,
	ModePlay_Close,
	NULL,
	NULL,
	NULL,
	NULL
};
