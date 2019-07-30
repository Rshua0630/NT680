/**
    Copyright   Novatek Microelectronics Corp. 2012.  All rights reserved.

    @file       SysMain_Flow_Mode.c
    @ingroup    mSystemFlow

    @brief      Mode Flow

    @note
                1.提供project動態登記Mode Table的介面
                  System_AddMode()

                2.提供change mode的介面
                  System_ChangeMode()
                  (a)System_ChangeMode()執行System_ModeOpen() & System_ModeClose()

                3.提供個別mode的control介面
                  (a)System_ModeOpen()執行中間會透過function pointer通知個別mode,
                     個別mode可以在OnOpen()這邊安插change mode所需的對應動作
                     MODE.pfMode_OnOpen()
                  (b)System_ModeClose()執行中間會透過function pointer通知個別mode,
                     個別mode可以在OnClose()這邊安插change mode所需的對應動作
                     MODE.pfMode_OnClose()
                  (c)System_ModeSleepEnter()執行中間會透過function pointer通知個別mode,
                     個別mode可以在OnSleepEnter()這邊安插change mode所需的對應動作
                     MODE.pfMode_OnSleepEnter()
                  (d)System_ModeSleepLeave()執行中間會透過function pointer通知個別mode,
                     個別mode可以在OnSleepLeave()這邊安插change mode所需的對應動作
                     MODE.pfMode_OnSleepLeave()

    @date       2012/1/1
*/

#include "Type.h"
#include "Debug.h"
#include "SysMain.h"
#include "UIFramework.h"
#include "UICommon.h"
//global debug level: PRJ_DBG_LVL
#include "PrjCfg.h"
//local debug level: THIS_DBGLVL
#define THIS_DBGLVL         2 // 0=FATAL, 1=ERR, 2=WRN, 3=UNIT, 4=FUNC, 5=IND, 6=MSG, 7=VALUE, 8=USER
///////////////////////////////////////////////////////////////////////////////
#define __MODULE__          SysMode
#define __DBGLVL__          ((THIS_DBGLVL>=PRJ_DBG_LVL)?THIS_DBGLVL:PRJ_DBG_LVL)
#define __DBGFLT__          "*" //*=All, [mark]=CustomClass
#include "DebugModule.h"

#define MODE_MAX_COUNT     16
#define SUBMODE_MAX_COUNT   10

UINT32 g_SysModeCnt = 0;
UINT32 g_SysSubModeCnt = 1;  //reserved for noraml mode
UINT32 g_SysModeTable_cnt = 0;
SYS_MODE *g_SysModeTable[MODE_MAX_COUNT] = {0};
SYS_SUBMODE *g_SysSubModeTable[SUBMODE_MAX_COUNT] = {0};

INT32 g_iSysCurMode = SYS_MODE_UNKNOWN;
INT32 g_iSysPrevMode = SYS_MODE_UNKNOWN;
INT32 g_iSysNextMode = SYS_MODE_UNKNOWN;
INT32 g_iSysSubMode = SYS_SUBMODE_NORMAL;
INT32 g_iSysCurSubMode = SYS_SUBMODE_NORMAL;
INT32 g_iSysPrevSubMode = SYS_SUBMODE_NORMAL;
INT32 g_iSysNextSubMode = SYS_SUBMODE_NORMAL;

INT32 System_AddMode(SYS_MODE *pMode)
{
	UINT32 i = g_SysModeCnt;
	if (pMode == 0) {
		DBG_ERR("This mode is invalid!\r\n");
		return SYS_MODE_UNKNOWN;
	}

	g_SysModeTable[i] = pMode;
	g_SysModeCnt++;
	return (INT32)i;
}

void System_ModeOpen(UINT32 mode_id)
{
	SYS_MODE *pCurrMode;

	if (mode_id >= g_SysModeCnt) { //fix for CID 44970
		if ((INT32)mode_id != SYS_MODE_UNKNOWN) { //allow mode_id SYS_MODE_UNKNOWN
			DBG_ERR("Next mode (id: 0x%02x) is not exist! ignored.\r\n", mode_id);
		}
		return;
	}
	//#NT#2016/11/10#Niven Cho -begin
	//#NT#AUTO_TEST
	//next = new
	g_iSysNextMode = mode_id;
	pCurrMode = g_SysModeTable[g_iSysNextMode];
	DBG_DUMP("^MMode {%s} Open begin\r\n", pCurrMode->strName);
	exam_msg("Mode {%s} Open begin\r\n", pCurrMode->strName);
	if (pCurrMode->pfMode_OnOpen) {
		pCurrMode->pfMode_OnOpen();
	}
	DBG_DUMP("^MMode {%s} Open end\r\n", pCurrMode->strName);
	exam_msg("Mode {%s} Open end\r\n", pCurrMode->strName);
	//current = next
	g_iSysCurMode  = g_iSysNextMode;
	//next = 0
	g_iSysNextMode = SYS_MODE_UNKNOWN;
	//#NT#2016/11/10#Niven Cho -end
}

void System_ModeClose(void)
{
	SYS_MODE *pCurrMode;

	if ((g_iSysCurMode >= (INT32)g_SysModeCnt)
		|| (g_iSysCurMode < 0)) {
		if (g_iSysCurMode != SYS_MODE_UNKNOWN) {
			DBG_ERR("Current mode (id: 0x%02x) is not exist! ignored.\r\n", g_iSysCurMode);
		}
		return;
	}
	pCurrMode = g_SysModeTable[g_iSysCurMode];
	DBG_DUMP("^MMode {%s} Close begin\r\n", pCurrMode->strName);
	if (pCurrMode->pfMode_OnClose) {
		pCurrMode->pfMode_OnClose();
	}
	DBG_DUMP("^MMode {%s} Close end\r\n", pCurrMode->strName);
	//prev = current
	g_iSysPrevMode = g_iSysCurMode; // Save mode
	//current = next
	g_iSysCurMode  = SYS_MODE_UNKNOWN;
}

INT32 g_iSysSleep = 0;

void System_ModeSleepEnter(void)
{
	SYS_MODE *pCurrMode;
	if (g_iSysSleep == 1) {
		return;
	}

	if ((g_iSysCurMode >= (INT32)g_SysModeCnt)
		|| (g_iSysCurMode < 0)) {
		if (g_iSysCurMode != SYS_MODE_UNKNOWN) {
			DBG_ERR("Current mode (id: 0x%02x) is not exist! ignored.\r\n", g_iSysCurMode);
		}
		return;
	}
	pCurrMode = g_SysModeTable[g_iSysCurMode];
	DBG_DUMP("^MMode {%s} SleepEnter begin\r\n", pCurrMode->strName);
	if (pCurrMode->pfMode_OnSleepEnter) {
		pCurrMode->pfMode_OnSleepEnter();
	}
	DBG_DUMP("^MMode {%s} SleepEnter end\r\n", pCurrMode->strName);

	g_iSysSleep = 1;
}

void System_ModeSleepLeave(void)
{
	SYS_MODE *pCurrMode;
	if (g_iSysSleep == 0) {
		return;
	}

	if ((g_iSysCurMode >= (INT32)g_SysModeCnt)
		|| (g_iSysCurMode < 0)) {
		if (g_iSysCurMode != SYS_MODE_UNKNOWN) {
			DBG_ERR("Current mode (id: 0x%02x) is not exist! ignored.\r\n", g_iSysCurMode);
		}
		return;
	}
	pCurrMode = g_SysModeTable[g_iSysCurMode];
	DBG_DUMP("^MMode {%s} SleepLeave begin\r\n", pCurrMode->strName);
	if (pCurrMode->pfMode_OnSleepLeave) {
		pCurrMode->pfMode_OnSleepLeave();
	}
	DBG_DUMP("^MMode {%s} SleepLeave end\r\n", pCurrMode->strName);

	g_iSysSleep = 0;
}
/////////////////////////////////////////////////////////////////////////

void System_ChangeMode(UINT32 mode_id)
{
	//avoid event in queue,and execute at the same time
	UI_LockEvent();

	DBG_FUNC_BEGIN("\r\n");
	DBG_MSG("Mode Begin (id: 0x%02x) ...\r\n", mode_id);

	//close current
	System_ModeClose();
	/*
	//if(g_bIsInitSystemFinish)
	{
	    GX_MODE* pCurrMode = GxSystem_GetCurrentMode();

	    if(pCurrMode && pCurrMode->pfMode_OnPauseDMA)
	        pCurrMode->pfMode_OnPauseDMA(); //call mode process
	    GxSystem_ActivePause(); //pause DMA access of all standard modules
	}

	DxSys_ChangeCPUSpeed(speed_id);
	DxSys_ChangeDMASpeed(speed_id); //Must "pause DMA" before this line, "resume DMA" after this line

	//if(g_bIsInitSystemFinish)
	{
	    GX_MODE* pCurrMode = GxSystem_GetCurrentMode();

	    GxSystem_ActiveResume(); //resume DMA access of all standard modules
	    if(pCurrMode && pCurrMode->pfMode_OnResumeDMA)
	        pCurrMode->pfMode_OnResumeDMA(); //call mode process
	}
	*/

	g_iSysPrevSubMode = g_iSysCurSubMode;
	g_iSysCurSubMode = g_iSysNextSubMode;
	//open new
	System_ModeOpen(mode_id);

	DBG_MSG("Mode End (id: 0x%02x)\r\n", System_GetState(SYS_STATE_CURRMODE));
	DBG_FUNC_END("\r\n");

	//resume queue,event can be executed
	UI_UnlockEvent();
}
void System_ChangeSubMode(INT32 mode_id)
{
	g_iSysPrevSubMode = g_iSysCurSubMode;
	g_iSysCurSubMode = mode_id;
	g_iSysNextSubMode = mode_id;
}
INT32 System_AddSubMode(SYS_SUBMODE *pMode)
{
	UINT32 i = g_SysSubModeCnt;
	if (pMode == 0) {
		DBG_ERR("This mode is invalid!\r\n");
		return SYS_MODE_UNKNOWN;
	}

	g_SysSubModeTable[i] = pMode;
	g_SysSubModeCnt++;
	return (INT32)i;
}
SYS_SUBMODE *System_GetSubModeCtrl(void)
{
	return g_SysSubModeTable[g_iSysCurSubMode];
}
#if 0
/**
    The system control operation
*/
typedef enum {
	SYS_CTRL_SLEEP = 0x10,  ///< Enter Sleep mode
	SYS_CTRL_WAKEUP = 0x11,  ///< Leave Sleep mode
	SYS_CTRL_PAUSEDMA = 0x12, ///< Pause DMA
	SYS_CTRL_RESUMEDMA = 0x13, ///< Resume DMA
} SYS_CTRL;

/**
    Support system control for related flow

    @param[in] uiSysCtrl     The system control operation.
    @param[in] bWaitFinish   Wait until finish or not.

    @return Status returned.
         - @b 1:       success
         - @b 0:       fail
*/
UINT32 PB_SysControl(UINT32 uiSysCtrl, BOOL bWaitFinish);

//[XXX_SysCtrl.c]
UINT32 PB_SysControl(UINT32 uiSysCtrl, BOOL bWaitFinish)
{
	switch (uiSysCtrl) {
	case SYS_CTRL_SLEEP:
		break;
	case SYS_CTRL_WAKEUP:
		break;
	case SYS_CTRL_PAUSEDMA:
		break;
	case SYS_CTRL_RESUMEDMA:
		break;
	}
}
#endif


