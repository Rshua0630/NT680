#include "UIFramework.h"
#include "UIFrameworkExt.h"
#include "NVTToolCommand.h"
#include "FlowCommon.h"
#include "FlowCommonRes.c"
#include "UIFlow.h"

#define __MODULE__          FlowCommon
#define __DBGLVL__ 1        //ERROR mode, show err, wrn only
#define __DBGFLT__ "*"      //*=All
#include "DebugModule.h"

CTRL_LIST_BEGIN(FlowCommon)
CTRL_LIST_END

//----------------------UIFlowWndMovieCtrl Event---------------------------
//INT32 FlowCommon_OnOpen(VControl *, UINT32, UINT32 *);
//INT32 FlowCommon_OnClose(VControl *, UINT32, UINT32 *);
INT32 FlowCommon_OnVideoChange(VControl *, UINT32, UINT32 *);

//INT32 FlowCommon_OnOpen(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
//{
//    return NVTEVT_PASS;
//}

//INT32 FlowCommon_OnClose(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
//{
//    return NVTEVT_PASS;
//}

INT32 FlowCommon_OnVideoChange(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
	VControl *pFocusedCtrl = 0;
	//only handle focused window
	Ux_GetFocusedWindow(&pFocusedCtrl);
	if (pCtrl != pFocusedCtrl) {
		return NVTEVT_PASS;
	}

	UI_Show(UI_SHOW_WINDOW, TRUE);
	UxCtrl_SetDirty(pCtrl, TRUE);
	//DBGD(Perf_GetCurrent());
	return NVTEVT_CONSUME;
}

EVENT_BEGIN(FlowCommon)
//EVENT_ITEM(NVTEVT_OPEN_WINDOW,FlowCommon_OnOpen)
//EVENT_ITEM(NVTEVT_CLOSE_WINDOW,FlowCommon_OnClose)
EVENT_ITEM(NVTEVT_VIDEO_CHANGE, FlowCommon_OnVideoChange)
EVENT_END

