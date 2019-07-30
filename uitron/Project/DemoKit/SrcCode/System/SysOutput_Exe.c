/*
    System Output Callback

    System Callback for Output Module.

    @file       SysOutput_Exe.c
    @ingroup    mIPRJSYS

    @note

    Copyright   Novatek Microelectronics Corp. 2010.  All rights reserved.
*/

////////////////////////////////////////////////////////////////////////////////
#include "SysCommon.h"
#include "AppCommon.h"
////////////////////////////////////////////////////////////////////////////////
#include "UIFrameworkExt.h"
#include "AppLib.h"
#include "GxOutput.h"
#if !defined(_Gyro_None_)
#include "gyro_drv_ext.h"
#include "gyro_drv.h"
#endif

//global debug level: PRJ_DBG_LVL
#include "PrjCfg.h"
//local debug level: THIS_DBGLVL
#define THIS_DBGLVL         2 // 0=FATAL, 1=ERR, 2=WRN, 3=UNIT, 4=FUNC, 5=IND, 6=MSG, 7=VALUE, 8=USER
///////////////////////////////////////////////////////////////////////////////
#define __MODULE__          SysOutputExe
#define __DBGLVL__          ((THIS_DBGLVL>=PRJ_DBG_LVL)?THIS_DBGLVL:PRJ_DBG_LVL)
#define __DBGFLT__          "*" //*=All, [mark]=CustomClass
#include "DebugModule.h"
///////////////////////////////////////////////////////////////////////////////
#if (LED_FUNCTION == ENABLE)
int SX_TIMER_DET_LED_ID = -1;
SX_TIMER_ITEM(Output_LED, GxLED_ToggleLED, 1, TRUE)
#endif

void System_OnOutputInit(void)
{
	//PHASE-1 : Init & Open Drv or DrvExt
	{
		//1.設定init值
		//2.設定CB值,
		//3.註冊SxJob服務 ---------> System Job
		//4.註冊SxTimer服務 ---------> Detect Job
#if (LED_FUNCTION == ENABLE)
		SX_TIMER_DET_LED_ID = SxTimer_AddItem(&Timer_Output_LED);
#endif
	}
	//PHASE-2 : Init & Open Lib or LibExt
	{
		// Init LED_
		GxLED_SetCtrl(KEYSCAN_LED_GREEN, SETLED_SPEED, GXLED_1SEC_LED_TOGGLE_CNT / 2);
		GxLED_SetCtrl(KEYSCAN_LED_RED, SETLED_SPEED, GXLED_1SEC_LED_TOGGLE_CNT / 5);
		GxLED_SetCtrl(KEYSCAN_LED_FCS, SETLED_SPEED, GXLED_1SEC_LED_TOGGLE_CNT / 10);
#if (LED_FUNCTION == ENABLE)
		SxTimer_SetFuncActive(SX_TIMER_DET_LED_ID, TRUE);
#endif
	}
	//start LED toggle
	{
		GxLED_SetCtrl(KEYSCAN_LED_GREEN, SETLED_SPEED, KEYSCAN_LED_TSPEED_FAST);
		GxLED_SetCtrl(KEYSCAN_LED_RED, SETLED_SPEED, KEYSCAN_LED_TSPEED_FAST);
		GxLED_SetCtrl(KEYSCAN_LED_FCS, SETLED_SPEED, KEYSCAN_LED_TSPEED_NORMAL);

		GxLED_SetCtrl(KEYSCAN_LED_FCS, TURNON_LED, FALSE);
		GxLED_SetCtrl(KEYSCAN_LED_GREEN, TURNON_LED, TRUE);
		GxLED_SetCtrl(KEYSCAN_LED_RED, TURNON_LED, FALSE);
	}
}

void System_OnOutputExit(void)
{
	//stop LED toggle
	{
		GxLED_SetCtrl(KEYSCAN_LED_FCS, TURNON_LED, FALSE);
		GxLED_SetCtrl(KEYSCAN_LED_GREEN, TURNON_LED, FALSE);
		GxLED_SetCtrl(KEYSCAN_LED_RED, TURNON_LED, FALSE);
	}
	//PHASE-2 : Close Lib or LibExt
	{
	}
	//PHASE-1 : Close Drv or DrvExt
	{
#if !defined(_Gyro_None_)
		gyro_shutdown();
#endif
	}
}
