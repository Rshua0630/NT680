/**
    Copyright   Novatek Microelectronics Corp. 2012.  All rights reserved.

    @file       SysMain_State.c
    @ingroup    mSystemFlow

    @brief      State

    @note
                1.提供system state的介面
                  System_GetState()
                  (a)System_GetState()執行 get state
                  包含POWERON mode, POWEROFF mode,
                  ui CURRMODE, ui PREVMODE, ui NEXTMODE
                  pwr SLEEPLEVEL
                  (b)System_SetState()執行 set state
                  包含POWERON mode, POWEROFF mode,

    @date       2012/1/1
*/

////////////////////////////////////////////////////////////////////////////////
#include "Type.h"
#include "Debug.h"
#include "SysMain.h"
#include "Utility.h"

#include "DxSys.h"

//--------------------- Debug Definition -----------------------------
///////////////////////////////////////////////////////////////////////////////
#define __MODULE__          SysState
#define __DBGLVL__          2 // 0=FATAL, 1=ERR, 2=WRN, 3=UNIT, 4=FUNC, 5=IND, 6=MSG, 7=VALUE, 8=USER
#define __DBGFLT__          "*" //*=All, [mark]=CustomClass
#include "DebugModule.h"
///////////////////////////////////////////////////////////////////////////////


extern INT32 g_iSysPowerOnMode;
extern INT32 g_iSysPowerOffMode;
extern INT32 g_iSysCurMode;
extern INT32 g_iSysPrevMode;
extern INT32 g_iSysNextMode;
extern INT32 g_iSysSleepLevel;
extern INT32 g_iSysCurSubMode;
extern INT32 g_iSysPrevSubMode;
extern INT32 g_iSysNextSubMode;
INT32 g_iSysFs = 0;
INT32 g_iSysCard = 0;

INT32 System_GetState(INT32 state)
{
	INT32 rvalue = 0;

	switch (state) {
	case SYS_STATE_POWERON:
		rvalue = g_iSysPowerOnMode;
		break;
	case SYS_STATE_POWEROFF:
		rvalue = g_iSysPowerOffMode;
		break;
	case SYS_STATE_CURRMODE:
		rvalue = g_iSysCurMode;
		break;
	case SYS_STATE_PREVMODE:
		rvalue = g_iSysPrevMode;
		break;
	case SYS_STATE_NEXTMODE:
		rvalue = g_iSysNextMode;
		break;
	case SYS_STATE_SLEEPLEVEL:
		rvalue = g_iSysSleepLevel;
		break;
	case SYS_STATE_CURRSUBMODE:
		rvalue = g_iSysCurSubMode;
		break;
	case SYS_STATE_PREVSUBMODE:
		rvalue = g_iSysPrevSubMode;
		break;
	case SYS_STATE_NEXTSUBMODE:
		rvalue = g_iSysNextSubMode;
		break;
	case SYS_STATE_FS:
		rvalue = g_iSysFs;
		break;
	case SYS_STATE_CARD:
		rvalue = g_iSysCard;
		break;
	}
	return rvalue;
}

void System_SetState(INT32 state, INT32 value)
{
	switch (state) {
	case SYS_STATE_POWERON:
		g_iSysPowerOnMode = value;
		break;
	case SYS_STATE_POWEROFF:
		g_iSysPowerOffMode = value;
		break;
	case SYS_STATE_CURRMODE:
	case SYS_STATE_PREVMODE:
	case SYS_STATE_NEXTMODE:
	case SYS_STATE_SLEEPLEVEL:
		break;
	case SYS_STATE_CURRSUBMODE:
		g_iSysCurSubMode = value;
		break;
	case SYS_STATE_NEXTSUBMODE:
		g_iSysNextSubMode = value;
		break;
	case SYS_STATE_FS:
		g_iSysFs = value;
		break;
	case SYS_STATE_CARD:
		g_iSysCard = value;
		break;
	}
}

void System_ChangeCPUSpeed(int speed_id)
{
	DxSys_ChangeCPUSpeed(speed_id);
}
void System_ChangeDMASpeed(int speed_id)
{
	DxSys_ChangeDMASpeed(speed_id);
}

