/**
    Copyright   Novatek Microelectronics Corp. 2012.  All rights reserved.

    @file       SysMain_Flow_Exit.c
    @ingroup    mSystemFlow

    @brief      PowerOff Flow

    @note
                1.提供power off的介面
                  System_PowerOff()
                  (a)System_PowerOff()執行
                    各mode不同的control condition
                    包含通知不同device exit
                  (b)System_PowerOff()執行
                    各mode不同的control condition
                    包含控制不同device之間相互dependent的order

    @date       2012/1/1
*/

////////////////////////////////////////////////////////////////////////////////
#include "SysCommon.h"
#include "AppCommon.h"
////////////////////////////////////////////////////////////////////////////////
#include "UIFrameworkExt.h"
#include "AppLib.h"
#include "SysMain.h"

//global debug level: PRJ_DBG_LVL
#include "PrjCfg.h"
//local debug level: THIS_DBGLVL
#define THIS_DBGLVL         2 // 0=FATAL, 1=ERR, 2=WRN, 3=UNIT, 4=FUNC, 5=IND, 6=MSG, 7=VALUE, 8=USER
///////////////////////////////////////////////////////////////////////////////
#define __MODULE__          SysExit
#define __DBGLVL__          ((THIS_DBGLVL>=PRJ_DBG_LVL)?THIS_DBGLVL:PRJ_DBG_LVL)
#define __DBGFLT__          "*" //*=All, [mark]=CustomClass
#include "DebugModule.h"

/////////////////////////////////////////////////////////////////////////////
//#include "DevMan.h"
//#include "NvtSystem.h"
#include "Utility.h"
#if(WIFI_FUNC==ENABLE)
#include "WifiAppCmd.h"
#endif

/////////////////////////////////////////////////////////////////////////////

//INT32 g_iSysPowerOffMode = SYS_POWEROFF_NORMAL;
INT32 g_iSysPowerOffMode = SYS_POWEROFF_SAFE;

void System_PowerOff(UINT32 pwrOffType)
{
	//g_iSysPowerOffMode = pwrOffType;
	Ux_SendEvent(0, NVTEVT_SYSTEM_SHUTDOWN, 1, 0); //shutdown start
}

void _System_PowerOff(void)
{
	DBG_FUNC_BEGIN("\r\n");

	DBG_FUNC_END("\r\n");
}

////////////////////////////////////////////////////////////////////////////////
#include "DxSys.h"
#include "DrvExt.h" //Dx_OnCommand, Dx_ConfigIO()
#include "Utility.h"

// ShutDown Start
void System_ShutDownStart(void)
{
	DBG_DUMP("^MSystem Shutdown begin\r\n");
#if(WIFI_FUNC==ENABLE)
#if(WIFI_AP_FUNC==ENABLE && defined(_CPU2_ECOS_))
	if (System_GetState(SYS_STATE_CURRSUBMODE) == SYS_SUBMODE_WIFI) {
		WifiApp_SendCmd(WIFIAPP_CMD_NOTIFY_STATUS, WIFIAPP_RET_POWER_OFF);
	}
#endif
#endif
}

// ShutDown End
void System_ShutDownEnd(void)
{
	DBG_DUMP("^MSystem Shutdown end\r\n");
	DBG_DUMP("\r\n");
}


////////////////////////////////////////////////////////////////////////////////
#if 1
void SystemExit(void)
{
	DBG_MSG("^GExit Start\r\n");

	///////////////////////////////////////////
#if (POWERON_FAST_BOOT == ENABLE)
	if (System_GetState(SYS_STATE_POWEROFF) == SYSTEM_POWEROFF_NORMAL) {
		//"normal power-off sequence"
		DBG_DUMP("Power Off Sequence = Normal\r\n");

		///////////////////////////////////////////
		//Start Multi-Task
		EXIT_SETFLAG(FLGEXIT_BEGIN2);
		EXIT_SETFLAG(FLGEXIT_BEGIN3);
		EXIT_SETFLAG(FLGEXIT_BEGIN4);
		EXIT_SETFLAG(FLGEXIT_BEGIN5);
		EXIT_SETFLAG(FLGEXIT_BEGIN6);
		EXIT_SETFLAG(FLGEXIT_BEGIN7);
		EXIT_SETFLAG(FLGEXIT_BEGIN8);

#if (INPUT_FUNC == ENABLE)
		System_OnInputExit();
#endif

		EXIT_WAITFLAG(FLGEXIT_BEGIN2);
		EXIT_WAITFLAG(FLGEXIT_BEGIN3);
		EXIT_WAITFLAG(FLGEXIT_BEGIN4);
		EXIT_WAITFLAG(FLGEXIT_BEGIN5);
		EXIT_WAITFLAG(FLGEXIT_BEGIN6);
		EXIT_WAITFLAG(FLGEXIT_BEGIN7);
		EXIT_WAITFLAG(FLGEXIT_BEGIN8);
	} else
#endif
		if (System_GetState(SYS_STATE_POWEROFF) == SYSTEM_POWEROFF_SAFE) {
			//"safe power-off sequence"
			DBG_DUMP("Power Off Sequence = Safe\r\n");

			//#NT#2016/03/08#Niven Cho -begin
			//#NT#because it is long time closing the linux, we don't close video ahead.
#if defined(_CPU2_LINUX_)
			System_OnStrgExit_FS(); //it triggers linux close.
			System_OnStrgExit_PS();
			System_OnStrgExit_FWS();
			System_OnStrgExit_EXMEM();
			System_OnStrgExit_EMBMEM();
			System_OnVideoExit();
			System_OnAudioExit();
#else
			//Exit Lib & Device
			System_OnVideoExit();
			System_OnAudioExit();
			System_OnStrgExit_FS();
			System_OnStrgExit_PS();
			System_OnStrgExit_FWS();
			System_OnStrgExit_EXMEM();
			System_OnStrgExit_EMBMEM();
#endif
			//#NT#2016/03/08#Niven Cho -end
#if(USB_MODE==ENABLE)
			System_OnUsbExit();
#endif
#if (LENS_FUNCTION == ENABLE)
			System_OnLensExit();
#endif
			System_OnSensorExit();
#if (INPUT_FUNC == ENABLE)
			System_OnInputExit();
#endif
		} else if (System_GetState(SYS_STATE_POWEROFF) == SYSTEM_POWEROFF_LOWPOWER) {
			//"lowpower power-off sequence"
			DBG_DUMP("Power Off Sequence = LowPower\r\n");
		} else if (System_GetState(SYS_STATE_POWEROFF) == SYSTEM_POWEROFF_CHARGE) {
			//"charge power-off sequence"
			DBG_DUMP("Power Off Sequence = Charge\r\n");
		}

#if (PWR_FUNC == ENABLE)
	System_OnPowerExit();
#endif
#if (OUTPUT_FUNC == ENABLE)
	System_OnOutputExit();
#endif
	System_OnTimerExit();

	DBG_MSG("^GExit End\r\n");
}

#if (POWERON_FAST_BOOT == ENABLE)

void SystemExit2(void)
{
	if (System_GetState(SYS_STATE_POWEROFF) == SYSTEM_POWEROFF_NORMAL) {
		DBG_MSG("^GExit 2 Start\r\n");
		System_OnStrgExit_PS();
		System_OnStrgExit_FWS();
		System_OnStrgExit_EXMEM();
		System_OnStrgExit_EMBMEM();
		DBG_MSG("^GExit 2 End\r\n");
	}
}
void SystemExit3(void)
{
	if (System_GetState(SYS_STATE_POWEROFF) == SYSTEM_POWEROFF_NORMAL) {
		DBG_MSG("^GExit 3 Start\r\n");
		System_OnVideoExit();
		DBG_MSG("^GExit 3 End\r\n");
	}
}
void SystemExit4(void)
{
	if (System_GetState(SYS_STATE_POWEROFF) == SYSTEM_POWEROFF_NORMAL) {
		DBG_MSG("^GExit 4 Start\r\n");
		System_OnAudioExit();
		DBG_MSG("^GExit 4 End\r\n");
	}
}
void SystemExit5(void)
{
	if (System_GetState(SYS_STATE_POWEROFF) == SYSTEM_POWEROFF_NORMAL) {
		DBG_MSG("^GExit 5 Start\r\n");
		System_OnStrgExit_FS();
		DBG_MSG("^GExit 5 End\r\n");
	}
}
void SystemExit6(void)
{
	if (System_GetState(SYS_STATE_POWEROFF) == SYSTEM_POWEROFF_NORMAL) {
		DBG_MSG("^GExit 6 Start\r\n");
#if(USB_MODE==ENABLE)
		System_OnUsbExit();
#endif
		DBG_MSG("^GExit 6 End\r\n");
	}
}
void SystemExit7(void)
{
	if (System_GetState(SYS_STATE_POWEROFF) == SYSTEM_POWEROFF_NORMAL) {
		DBG_MSG("^GExit 7 Start\r\n");
		System_OnSensorExit();
		DBG_MSG("^GExit 7 End\r\n");
	}
}
void SystemExit8(void)
{
	if (System_GetState(SYS_STATE_POWEROFF) == SYSTEM_POWEROFF_NORMAL) {
		DBG_MSG("^GExit 8 Start\r\n");
		DBG_MSG("^GExit 8 End\r\n");
	}
}
#endif

#endif
