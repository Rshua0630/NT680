////////////////////////////////////////////////////////////////////////////////
#include "SysCommon.h"
//#include "AppCommon.h"
////////////////////////////////////////////////////////////////////////////////
#include "AppLib.h"
#include "UIMode.h"
#include "UIModeUpdFw.h"
#if(WIFI_FUNC==ENABLE)
#include "UIModeWifi.h"
#endif
#include "UISetup.h"
#include "UIFrameworkExt.h"
#include "UIInfo.h"

#if (CALIBRATION_FUNC == ENABLE)
#include "EngineerMode.h"
#endif

int PRIMARY_MODE_UPDFW = -1;      ///< UpdFw
int SYS_SUBMODE_UPDFW = -1;
void ModeUpdFw_Open(void);
void ModeUpdFw_Close(void);

void ModeUpdFw_Open(void)
{
	Ux_SetActiveApp(&UISetupObjCtrl);
	Ux_SendEvent(0, NVTEVT_EXE_OPEN, 0);
#if(UI_FUNC==ENABLE)
	if (System_GetState(SYS_STATE_CURRSUBMODE) == SYS_SUBMODE_NORMAL) {
		//Ux_OpenWindow((VControl *)(&UIUpdFwWndCtrl), 0);
	} else {
		SYS_SUBMODE *pCurSubMode = System_GetSubModeCtrl();
		if (pCurSubMode && pCurSubMode->pfSubMode_OnOpen) {
			pCurSubMode->pfSubMode_OnOpen();
		}
	}
#endif


}
void ModeUpdFw_Close(void)
{

#if(UI_FUNC==ENABLE)
	if (System_GetState(SYS_STATE_CURRSUBMODE) == SYS_SUBMODE_NORMAL) {
		//Ux_CloseWindow((VControl *)(&UIUpdFwWndCtrl), 0);
	} else {
		SYS_SUBMODE *pCurSubMode = System_GetSubModeCtrl();
		if (pCurSubMode && pCurSubMode->pfSubMode_OnClose) {
			pCurSubMode->pfSubMode_OnClose();
		}
	}
#endif

	Ux_SendEvent(0, NVTEVT_EXE_CLOSE, 0);

}

SYS_MODE gModeUpdFw = {
	"UPDFW",
	ModeUpdFw_Open,
	ModeUpdFw_Close,
	NULL,
	NULL,
	NULL,
	NULL
};

SYS_SUBMODE gSubModeUpdFw = {
	"SUBUPDFW",
	NULL,
	NULL,
};