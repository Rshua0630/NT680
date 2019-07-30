/**
    Copyright   Novatek Microelectronics Corp. 2012.  All rights reserved.

    @file       SysMain_Flow_Sleep.c
    @ingroup    mSystemFlow

    @brief      Sleep Flow

    @note
                1.提供change sleep的介面
                  System_ChangeSleep()
                  (a)System_ChangeSleep()執行
                    各level不同的control condition
                    包含通知不同device sleep, wakeup
                  (b)System_ChangeSleep()執行
                    各level不同的control condition
                    包含通知不同mode sleep, wakeup: System_ModeSleepEnter(), System_ModeSleepLeave()

    @date       2012/1/1
*/

////////////////////////////////////////////////////////////////////////////////
#include "SysCommon.h"
#include "AppCommon.h"
////////////////////////////////////////////////////////////////////////////////
#include "UIFrameworkExt.h"
#include "UICommon.h"
#include "AppLib.h"
#if (INPUT_FUNC == ENABLE)
#include "GxInput.h"
#endif
#include "ImageStream.h"

//global debug level: PRJ_DBG_LVL
#include "PrjCfg.h"
//local debug level: THIS_DBGLVL
#define THIS_DBGLVL         2 // 0=FATAL, 1=ERR, 2=WRN, 3=UNIT, 4=FUNC, 5=IND, 6=MSG, 7=VALUE, 8=USER
///////////////////////////////////////////////////////////////////////////////
#define __MODULE__          SysSleep
#define __DBGLVL__          ((THIS_DBGLVL>=PRJ_DBG_LVL)?THIS_DBGLVL:PRJ_DBG_LVL)
#define __DBGFLT__          "*" //*=All, [mark]=CustomClass
#include "DebugModule.h"

/////////////////////////////////////////////////////////////////////////////

#if (LCD_ROTATE_FUNCTION == ENABLE)
extern void Display_DetLCDDir(void);
#endif

INT32       g_iSysSleepLevel = 0;

UINT32      g_CurKeySoundMask[3] = {0};

void System_ChangeSleep(UINT32 sleep_lv)
{
	UI_LockEvent();

	DBG_FUNC_BEGIN("\r\n");
	DBG_DUMP("^MSLEEP Begin (Lvl: 0x%x=>0x%x) ...\r\n", g_iSysSleepLevel, sleep_lv);

	//enter sleep
	if (g_iSysSleepLevel < (INT32)sleep_lv) {
		if (g_iSysSleepLevel < (INT32)sleep_lv) {
			if (g_iSysSleepLevel == 0) {
				//enter LIGHT level
				DBG_MSG("enter level 1 - begin\r\n");
				
				//LCD device
				GxVideo_SetDeviceCtrl(DOUT1, DISPLAY_DEVCTRL_BRIGHTLVL, 0);
#if (INPUT_FUNC == ENABLE)
				//save key sound mask
				g_CurKeySoundMask[0] = Input_GetKeySoundMask(KEY_PRESS);
				g_CurKeySoundMask[1] = Input_GetKeySoundMask(KEY_CONTINUE);
				g_CurKeySoundMask[2] = Input_GetKeySoundMask(KEY_RELEASE);
				//mute key sound
				Input_SetKeySoundMask(KEY_PRESS, FLGKEY_KEY_MASK_NULL);
				Input_SetKeySoundMask(KEY_CONTINUE, FLGKEY_KEY_MASK_NULL);
				Input_SetKeySoundMask(KEY_RELEASE, FLGKEY_KEY_MASK_NULL);
#endif
				g_iSysSleepLevel = 1;
				DBG_MSG("enter level 1 - end\r\n");
			}
		}
		if (g_iSysSleepLevel < (INT32)sleep_lv) {
			if (g_iSysSleepLevel == 1) {
				//enter NORMAL level
				DBG_MSG("enter level 2 - begin\r\n");
				System_ModeSleepEnter();
				Ux_SendEvent(0, NVTEVT_EXE_SLEEP, 0);
				/*
				//under sleep, cannot do flash charge
				Flash_OnSystem(SYSTEM_CMD_SLEEPENTER);
				//under sleep, FW need to stop AE,AWB, sensor also need to enter power-save mode
				Sensor_OnSystem(SYSTEM_CMD_SLEEPENTER);
				//under sleep, FW need to stop AF,CAF, lens also need to enter power-save mode
				Lens_OnSystem(SYSTEM_CMD_SLEEPENTER);
				*/
#if (OUTPUT_FUNC == ENABLE)
				//GxLED_SetCtrl(KEYSCAN_LED_RED,SETLED_SPEED,GXLED_1SEC_LED_TOGGLE_CNT);
				//GxLED_SetCtrl(KEYSCAN_LED_GREEN,TURNON_LED,FALSE);
				//GxLED_SetCtrl(KEYSCAN_LED_RED,SET_TOGGLE_LED,TRUE);
				GxLED_SetCtrl(KEYSCAN_LED_GREEN, SETLED_SPEED, GXLED_1SEC_LED_TOGGLE_CNT);
				GxLED_SetCtrl(KEYSCAN_LED_GREEN, TURNON_LED, FALSE);
				GxLED_SetCtrl(KEYSCAN_LED_GREEN, SET_TOGGLE_LED, TRUE);
#endif
				SxTimer_SetFuncActive(SX_TIMER_DET_SYSTEM_BUSY_ID, FALSE);
				g_iSysSleepLevel = 2;
				DBG_MSG("enter level 2 - end\r\n");
			}
		}
		if (g_iSysSleepLevel < (INT32)sleep_lv) {
			if (g_iSysSleepLevel == 2) {
				//enter DEEP level
				DBG_MSG("enter level 3 - begin\r\n");
				g_iSysSleepLevel = 3;
				DBG_MSG("enter level 3 - end\r\n");
			}
		}
	}
	//leave sleep
	else if (g_iSysSleepLevel > (INT32)sleep_lv) {
		if (g_iSysSleepLevel > (INT32)sleep_lv) {
			if (g_iSysSleepLevel == 3) {
				//leave DEEP level
				DBG_MSG("leave level 3 - begin\r\n");
				
				g_iSysSleepLevel = 2;
				DBG_MSG("leave level 3 - end\r\n");
			}
		}
		if (g_iSysSleepLevel > (INT32)sleep_lv) {
			if (g_iSysSleepLevel == 2) {
				//leave NORMAL level
				DBG_MSG("leave level 2 - begin\r\n");
				
				Ux_SendEvent(0, NVTEVT_EXE_WAKEUP, 0);
				System_ModeSleepLeave();
				/*
				Lens_OnSystem(SYSTEM_CMD_SLEEPLEAVE);
				Sensor_OnSystem(SYSTEM_CMD_SLEEPLEAVE);
				Flash_OnSystem(SYSTEM_CMD_SLEEPLEAVE);
				*/
#if (OUTPUT_FUNC == ENABLE)
				//GxLED_SetCtrl(KEYSCAN_LED_GREEN,TURNON_LED,TRUE);
				//GxLED_SetCtrl(KEYSCAN_LED_RED,SET_TOGGLE_LED,FALSE);
				GxLED_SetCtrl(KEYSCAN_LED_GREEN, SET_TOGGLE_LED, FALSE);
				GxLED_SetCtrl(KEYSCAN_LED_GREEN, TURNON_LED, TRUE);
#endif
				SxTimer_SetFuncActive(SX_TIMER_DET_SYSTEM_BUSY_ID, TRUE);
				g_iSysSleepLevel = 1;
				DBG_MSG("leave level 2 - end\r\n");
			}
		}
		if (g_iSysSleepLevel > (INT32)sleep_lv) {
			if (g_iSysSleepLevel == 1) {
				//leave LIGHT level
				DBG_MSG("leave level 1 - begin\r\n");
				
#if (LCD_ROTATE_FUNCTION == ENABLE)
				GxVideo_SetDeviceCtrl(DOUT1, DISPLAY_DEVCTRL_FORCEDETDIR, 1);
				//config video Direct
				Display_DetLCDDir();
#endif
				//LCD device
#if (UI_FUNC == ENABLE)
				GxVideo_SetDeviceCtrl(DOUT1, DISPLAY_DEVCTRL_BRIGHTLVL, UI_GetData(FL_LCD_BRIGHTNESS)); //get last user config level
#else
				GxVideo_SetDeviceCtrl(DOUT1, DISPLAY_DEVCTRL_BRIGHTLVL, 0); //off
#endif
				//restore key sound mask
#if (OUTPUT_FUNC == ENABLE)
				Input_SetKeySoundMask(KEY_PRESS, g_CurKeySoundMask[0]);
				Input_SetKeySoundMask(KEY_CONTINUE, g_CurKeySoundMask[1]);
				Input_SetKeySoundMask(KEY_RELEASE, g_CurKeySoundMask[2]);
#endif
				g_iSysSleepLevel = 0;
				DBG_MSG("leave level 1 - end\r\n");
			}
		}
	}

	DBG_DUMP("^MSLEEP End (Lvl: 0x%x)\r\n", g_iSysSleepLevel);
	DBG_FUNC_END("\r\n");

	UI_UnlockEvent();
}
