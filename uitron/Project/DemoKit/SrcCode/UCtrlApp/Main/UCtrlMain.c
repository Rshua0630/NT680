#include "timer.h"
#include "SysCommon.h"
#include "UIFramework.h"
#include "PrjCfg.h"
#include "UIInfo.h"
#if(PHOTO_MODE==ENABLE)
#include "UIAppPhoto.h"
#endif
#include "UIControl.h"
#include "UIControlWnd.h"
#include "UCtrlMain.h"
#include "UCtrlMainID.h"



#define THIS_DBGLVL         2 // 0=FATAL, 1=ERR, 2=WRN, 3=UNIT, 4=FUNC, 5=IND, 6=MSG, 7=VALUE, 8=USER
///////////////////////////////////////////////////////////////////////////////
#define __MODULE__          UCtrlMain
#define __DBGLVL__          ((THIS_DBGLVL>=PRJ_DBG_LVL)?THIS_DBGLVL:PRJ_DBG_LVL)
#define __DBGFLT__          "*" //*=All, [mark]=CustomClass
#include "DebugModule.h"

// flag define
#define FLG_UCTRL_MAIN_IDLE           0x00000001


void UctrlMain_Init(void)
{
	set_flg(UCTRL_MAIN_FLG_ID, FLG_UCTRL_MAIN_IDLE);
}

static void UctrlMain_TriggerEvent(void)
{
	FLGPTN     uiFlag;
	//clr_flg(UCTRL_MAIN_FLG_ID, FLG_UCTRL_MAIN_IDLE);
	wai_flg(&uiFlag, UCTRL_MAIN_FLG_ID, FLG_UCTRL_MAIN_IDLE, TWF_ORW | TWF_CLR);
}

static void UctrlMain_WaitEventFinish(void)
{
	FLGPTN     uiFlag;

	DBG_IND("WaitB = %ld us\r\n", timer_getCurrentCount(timer_getSysTimerID()));
	wai_flg(&uiFlag, UCTRL_MAIN_FLG_ID, FLG_UCTRL_MAIN_IDLE, TWF_ORW);
	DBG_IND("WaitE = %ld us\r\n", timer_getCurrentCount(timer_getSysTimerID()));
}

void UctrlMain_SendEvent(NVTEVT evt, UINT32 paramNum, ...)
{
	UINT32 i, wParam[MAX_MESSAGE_PARAM_NUM] = {0};
	va_list ap;

	if (paramNum) {
		va_start(ap, paramNum);
		for (i = 0; i < paramNum; i++) {
			wParam[i] = va_arg(ap, UINT32);
		}
		va_end(ap);
	}
	UctrlMain_TriggerEvent();
	Ux_PostEvent(UCTRL_EVENT_MASK | evt, paramNum, wParam[0], wParam[1], wParam[2]);
	UctrlMain_WaitEventFinish();

}

void UctrlMain_EventEnd(int ret, int Event)
{
	DBG_IND("Event = 0x%x\r\n", Event);
	set_flg(UCTRL_MAIN_FLG_ID, FLG_UCTRL_MAIN_IDLE);
}

void UctrlMain_Lock(void)
{
	wai_sem(UCTRL_MAIN_SEM_ID);
}

void UctrlMain_UnLock(void)
{
	sig_sem(UCTRL_MAIN_SEM_ID);
}



