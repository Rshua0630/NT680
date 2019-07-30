/**
    DevMan, Service command function implementation

    @file       DeviceUsbMan.c
    @ingroup    mDEVMAN

    Copyright   Novatek Microelectronics Corp. 2012.  All rights reserved.
*/

//global debug level: PRJ_DBG_LVL
#include "PrjCfg.h"
//local debug level: THIS_DBGLVL
#define THIS_DBGLVL         2 // 0=FATAL, 1=ERR, 2=WRN, 3=UNIT, 4=FUNC, 5=IND, 6=MSG, 7=VALUE, 8=USER
///////////////////////////////////////////////////////////////////////////////
#define __MODULE__          SysPwrExe
#define __DBGLVL__          ((THIS_DBGLVL>=PRJ_DBG_LVL)?THIS_DBGLVL:PRJ_DBG_LVL)
#define __DBGFLT__          "*" //*=All, [mark]=CustomClass
#include "DebugModule.h"


#include "SysKer.h"
#include "Debug.h"
#include "NvtUser.h"

//#include "NvtSystem.h"
#include "GxSystem.h"
#include "AppControl.h"


/////////////////////////////////////////////////////////////////////////////
// POWER

#include "GxPower.h"
#include "AppControl.h"

void Power_CB(UINT32 event, UINT32 param1, UINT32 param2);

void Power_CB(UINT32 event, UINT32 param1, UINT32 param2)
{
	switch (event) {
	//power-battery-status
	case POWER_CB_BATT_EMPTY:
		DBG_IND("Low battery \r\n");
		Ux_PostEvent(NVTEVT_PWR_BATTEMPTY, 0);
		break;

	case POWER_CB_BATT_CHG:
		DBG_IND("Batt Change %d\r\n", GxPower_GetControl(GXPWR_CTRL_BATTERY_LEVEL));
		Ux_PostEvent(NVTEVT_PWR_BATTCHANGE, 0);
		break;

	//power-save
	//Relationship between TV and sleep:
	//    begin Power sleeping flow -> stop detect TV
	//    end Power sleeping flow -> start detect TV
	case POWER_CB_SLEEP_ENTER_L:
		DBG_IND("Auto Sleep -1\r\n");
		Ux_PostEvent(NVTEVT_SYSTEM_SLEEP, 1, 1);
		break;
	case POWER_CB_SLEEP_ENTER:
		DBG_IND("Auto Sleep -2\r\n");
		Ux_PostEvent(NVTEVT_SYSTEM_SLEEP, 1, 2);
		break;
	case POWER_CB_SLEEP_ENTER_D:
		DBG_IND("Auto Sleep -3\r\n");
		Ux_PostEvent(NVTEVT_SYSTEM_SLEEP, 1, 3);
		break;
	case POWER_CB_SLEEP_RESET:
		DBG_IND("Auto Sleep 0\r\n");
		Ux_PostEvent(NVTEVT_SYSTEM_SLEEP, 1, 0);
		break;
	case POWER_CB_POWEROFF:
		DBG_IND("Auto Power-off \r\n");
		Ux_PostEvent(NVTEVT_SYSTEM_SHUTDOWN, 1, 0); //shutdown start
		break;

	case POWER_CB_CHARGE_OK:
		DBG_IND("Charge OK \r\n");
		Ux_PostEvent(NVTEVT_PWR_CHARGE_OK, 0);
		break;

	case POWER_CB_CHARGE_SUSPEND:
		DBG_IND("Charge SUSPEND \r\n");
		Ux_PostEvent(NVTEVT_PWR_CHARGE_SUSPEND, 0);
		break;
	case POWER_CB_CHARGE_RESUME:
		DBG_IND("Charge RESUME \r\n");
		Ux_PostEvent(NVTEVT_PWR_CHARGE_RESUME, 0);
		break;
	default:
		DBG_ERR("event(%d)\r\n", event);

	}
}


