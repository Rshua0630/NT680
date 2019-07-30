//This source code is generated by UI Designer Studio.

#include "UIFramework.h"
#include "UIFrameworkExt.h"
#include "NVTToolCommand.h"
#include "UIFlowWndWaitMomentRes.c"
#include "UIFlow.h"
//-----------------------------------------------------------------------------
// UIFlowWndWaitMoment User Variable & Function Declarations
//-----------------------------------------------------------------------------
static UINT32 g_uiWaitMomentMsg     = 0;
static UINT32 g_uiRestart_Rec     = 0;
static char g_StringTmpBuf[64] = {0};
static UINT32  g_UIStopRecTimerID = NULL_TIMER;
static BOOL g_bRecStopFinish     = 0;
static UINT32 g_uiRecStopTimerCnt     = 0;

//---------------------UIFlowWndWaitMomentCtrl Control List---------------------------
CTRL_LIST_BEGIN(UIFlowWndWaitMoment)
CTRL_LIST_ITEM(UIFlowWndWaitMoment_StatusTXT_Msg)
CTRL_LIST_END

//----------------------UIFlowWndWaitMomentCtrl Event---------------------------
INT32 UIFlowWndWaitMoment_OnOpen(VControl *, UINT32, UINT32 *);
INT32 UIFlowWndWaitMoment_OnClose(VControl *, UINT32, UINT32 *);
INT32 UIFlowWndWaitMoment_OnTimer(VControl *, UINT32, UINT32 *);
INT32 UIFlowWndWaitMoment_OnBackgroundDone(VControl *, UINT32, UINT32 *);
INT32 UIFlowWndWaitMoment_OnKeyShutter2(VControl *, UINT32, UINT32 *);
EVENT_BEGIN(UIFlowWndWaitMoment)
EVENT_ITEM(NVTEVT_OPEN_WINDOW, UIFlowWndWaitMoment_OnOpen)
EVENT_ITEM(NVTEVT_CLOSE_WINDOW, UIFlowWndWaitMoment_OnClose)
EVENT_ITEM(NVTEVT_TIMER, UIFlowWndWaitMoment_OnTimer)
EVENT_ITEM(NVTEVT_BACKGROUND_DONE, UIFlowWndWaitMoment_OnBackgroundDone)
EVENT_ITEM(NVTEVT_KEY_SHUTTER2, UIFlowWndWaitMoment_OnKeyShutter2)
EVENT_END

INT32 UIFlowWndWaitMoment_OnOpen(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    	g_uiRestart_Rec=0;
    	g_bRecStopFinish=0;
	if (paramNum > 0) {
		g_uiWaitMomentMsg = paramArray[0];
		debug_ind(("UIFlowWndWaitMoment: Msg = 0x%x\r\n", g_uiWaitMomentMsg));
	}
        if(UIFlowWndWaitMoment_StatusTXT_Msg_STRID_RESTART_REC==g_uiWaitMomentMsg){
            	//UxState_SetItemData(&UIFlowWndWaitMoment_StatusTXT_MsgCtrl, 0, STATE_ITEM_STRID,  Txt_Pointer(UIRes_GetUserString(STRID_LIVESTREAM)));
        	memset(g_StringTmpBuf, 0, 64);
            	snprintf(g_StringTmpBuf, sizeof(g_StringTmpBuf), "Slow card!  Restart rec...");
            	UxState_SetItemData(&UIFlowWndWaitMoment_StatusTXT_MsgCtrl, 0, STATE_ITEM_STRID,  Txt_Pointer(g_StringTmpBuf));
        }else if(UIFlowWndWaitMoment_StatusTXT_Msg_STRID_STOPREC_WAIT==g_uiWaitMomentMsg){
        	memset(g_StringTmpBuf, 0, 64);
            	snprintf(g_StringTmpBuf, sizeof(g_StringTmpBuf), "Stop rec, Please Wait...");
            	UxState_SetItemData(&UIFlowWndWaitMoment_StatusTXT_MsgCtrl, 0, STATE_ITEM_STRID,  Txt_Pointer(g_StringTmpBuf));

        	if (g_UIStopRecTimerID == NULL_TIMER) {
        		g_UIStopRecTimerID = GxTimer_StartTimer(100, NVTEVT_01SEC_TIMER, CONTINUE);
        		g_uiRecStopTimerCnt=3;
        	}

        }else{
        	UxState_SetData(&UIFlowWndWaitMoment_StatusTXT_MsgCtrl, STATE_CURITEM, g_uiWaitMomentMsg);
        }

	UxCtrl_SetShow(&UIFlowWndWaitMoment_StatusTXT_MsgCtrl, TRUE);

	Ux_DefaultEvent(pCtrl, NVTEVT_OPEN_WINDOW, paramNum, paramArray);
	return NVTEVT_CONSUME;
}
INT32 UIFlowWndWaitMoment_OnClose(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
	g_uiRestart_Rec=0;
	if (g_UIStopRecTimerID != NULL_TIMER) {
		GxTimer_StopTimer(&g_UIStopRecTimerID);
	}

	Ux_DefaultEvent(pCtrl, NVTEVT_CLOSE_WINDOW, paramNum, paramArray);
	return NVTEVT_CONSUME;
}
INT32 UIFlowWndWaitMoment_OnTimer(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
	UINT32  uiEvent;
	uiEvent = paramNum ? paramArray[0] : 0;
	switch (uiEvent) {
	case NVTEVT_01SEC_TIMER:
        	if(g_uiRecStopTimerCnt){
        	    g_uiRecStopTimerCnt--;
        	    if(g_bRecStopFinish && g_uiRecStopTimerCnt==0){
        	        debug_ind(("WaitMoment Timer Close Wnd\r\n"));
        	        Ux_CloseWindow(pCtrl, 2, NVTRET_WAITMOMENT, 0);
        	    }
        	}
		break;
	}
	return NVTEVT_CONSUME;
}
INT32 UIFlowWndWaitMoment_OnBackgroundDone(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
	NVTEVT event;
	UINT32 uiReturn = 0;
	UINT32 status;

	event = paramArray[ONDONE_PARAM_INDEX_CMD];
	status = paramArray[ONDONE_PARAM_INDEX_RET];
	debug_ind(("UIFlowWndWaitMoment OnBackgroundDone event = 0x%x, status=%d\r\n", event, status));

	switch (event) {
	case NVTEVT_BKW_COPY2CARD: {
			switch (status) {
			case 1: // TRUE
				uiReturn = NVTRET_COPY2CARD;
				break;
			case 0:
				uiReturn = NVTRET_CANCEL;
			}
			//Rsvd
			break;
		}

	case NVTEVT_BKW_DELALL: {
			uiReturn = NVTRET_DELETEALL;
			//Rsvd
			break;
		}


	case NVTEVT_BKW_FORMAT_CARD: {
			uiReturn = NVTRET_FORMAT;
			//Rsvd
			break;
		}

	case NVTEVT_BKW_FORMAT_NAND: {
			uiReturn = NVTRET_FORMAT;
			//Rsvd
			break;
		}

	case NVTEVT_BKW_FORMAT: {
			uiReturn = NVTRET_FORMAT;
			//Rsvd
			break;
		}

	case NVTEVT_BKW_SETPROTECT: {
			uiReturn = NVTRET_PROTECT;
			//Rsvd
			break;
		}
	case NVTEVT_BKW_WIFI_ON: {
			Ux_PostEvent(NVTEVT_SYSTEM_MODE, 2, PRIMARY_MODE_MOVIE, SYS_SUBMODE_WIFI);
			return NVTEVT_CONSUME;
		}
	case NVTEVT_BKW_FW_UPDATE: {
			if (status) {
				debug_err(("%s:update FW fail %d\r\n", __FUNCTION__, status));
			} else {
				Delay_DelayMs(2000);
				// Should power off immediatelly
				System_PowerOff(SYS_POWEROFF_NORMAL);
			}
			break;
		}
	case NVTEVT_BKW_STOPREC_PROCESS: {
			//debug_err(("NVTEVT_BKW_STOPREC_PROCESS, Restart_Rec=%d\r\n",g_uiRestart_Rec));
			//Delay_DelayMs(300);
			if(g_uiRestart_Rec){
        			uiReturn = NVTRET_RESTART_REC;
			}else{
        			uiReturn = 0;
        			g_bRecStopFinish=1;
        			if(g_uiRecStopTimerCnt){
        			    return NVTEVT_CONSUME;
        			}
			}
		}
			break;
	default:
		uiReturn = NVTRET_ERROR;
		debug_err(("UIFlowWndWaitMoment OnBackgroundDone: Unknown event 0x%x\r\n", event));
		break;
	}

	Ux_CloseWindow(pCtrl, 2, NVTRET_WAITMOMENT, uiReturn);
	return NVTEVT_CONSUME;
}
INT32 UIFlowWndWaitMoment_OnKeyShutter2(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    	UINT32 uiKeyAct;
	UINT32 uiState;

	//debug_err(("UIFlowWndWaitMoment_OnKeyShutter2 \r\n"));
	uiKeyAct = paramNum ? paramArray[0] : 0;

	if (paramNum >= 3) {
		uiState = paramArray[2];
	} else {
		uiState = 0;
	}

	//debug_err(("uiKeyAct=%d, uiState=0x%x\r\n",uiKeyAct,uiState));
	switch (uiKeyAct) {
	case NVTEVT_KEY_PRESS:
	    //debug_err(("NVTEVT_KEY_PRESS\r\n"));
	    if (uiState == UIFlowWndMovie_Restart_Rec) {
	        g_uiRestart_Rec=1;
	    }else{
	        g_uiRestart_Rec=0;
	    }
	break;
    	}

	return NVTEVT_CONSUME;
}

//----------------------UIFlowWndWaitMoment_StatusTXT_MsgCtrl Event---------------------------
EVENT_BEGIN(UIFlowWndWaitMoment_StatusTXT_Msg)
EVENT_END

