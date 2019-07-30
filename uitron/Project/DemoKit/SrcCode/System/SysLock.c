/*
    System Lock

    System Lock for SxInput.

    @file       SysLock.c
    @ingroup    mIPRJSYS

    @note       這個檔案負責三件事

                1.管理AutoSleep detect機制的介面
                UI_LockAutoSleep()
                   暫停
                UI_UnlockAutoSleep()
                   恢復
                UI_IsForceLockSleep(void)
                   詢問狀態

                2.管理AutoPowerOff detect機制的介面
                UI_LockAutoPWROff()
                   暫停
                UI_UnlockAutoPWROff()
                   恢復
                UI_IsForceLockAutoPWROff()
                   詢問狀態

                3.管理所有與UI Event有關的detect機制的介面 (除了Timer之外)
                UI_LockEvent()
                   暫停
                UI_UnlockEvent()
                   恢復
                UI_IsForceLock()
                   詢問狀態



    Copyright   Novatek Microelectronics Corp. 2010.  All rights reserved.
*/

#include "UIFramework.h"
#include "UICommon.h"

//---------------------SysLockCB Debug Definition -----------------------------
//---------------------SysLockCB Global Variables -----------------------------
//---------------------SysLockCB Prototype Declaration  -----------------------
//---------------------SysLockCB Public API  ----------------------------------
//---------------------SysLockCB Private API ----------------------------------

/*
/// Force Lock of Input Dectection /////////////////////////

注意!!! SxJob執行的工作使用時, 需要暫停某些SxTimer的detection

原因:
由於某些SxJob執行的工作使用時,
另有某些SxTimer的detect function同時會發出 Ux_PostEvent()
如此一來, SxJob/SxTimer兩個task之間就會發生循環lock造成當機
(A等B的SxJob finish, B等A的UI framework window samphore)
==> 範例: TV插拔的同時, 按下Playback Key造成當機

執行:
power_on flow (第一次change mode完成前)
power_off flow
sleep_enter flow
sleep_leave flow
change_dispdev flow
change_dispmode flow
change_mode flow

暫停: (凡是會PostEvent給上層的, 都要暫停)
SX_TIMER_DET_PWR_ID   //Key輸入
SX_TIMER_DET_KEY_ID   //Key輸入
SX_TIMER_DET_MODE_ID   //Key輸入
SX_TIMER_DET_USB_ID   //USB連接
SX_TIMER_DET_TV_ID   //TV連接
SX_TIMER_DET_BATT_ID   //Batt偵測
SX_TIMER_DET_SLEEP_ID   //Sleep偵測
SX_TIMER_DET_AUTOPOWEROFF_ID   //PowerOff偵測


不會用SxTimer_SetFuncActive()來做,
而是直接在detect function內加入條件, 不符合條件即bypass detect
此法可避免其他UI flow的SxTimer_SetFuncActive()設定被破壞

層次上:
UI Flow code -> 請使用設定SxTimer_SetFuncActive()來暫停
SxJob Flow code -> 設定force lock來暫停 (算是在更低階的地方攔截)
*/
UINT32 bDetect_ForceLockStatus = FALSE; //defalt can not lock
UINT32 bDetect_ForceLock = TRUE;
BOOL    g_bPostponeTVChange = FALSE;  //#NT#TV change action should be postponed from movie play start to first video frame decoded
static UINT32 bDetect_LockSleep = FALSE;
static UINT32 bDetect_LockAutoPWROff = FALSE;

UINT32 UI_IsForceLockAutoSleep(void)
{
	return bDetect_LockSleep;
}
UINT32 UI_IsForceLockAutoPWROff(void)
{
	return bDetect_LockAutoPWROff;
}
void UI_LockAutoSleep(void)
{
	bDetect_LockSleep = TRUE;
}
void UI_LockAutoPWROff(void)
{
	bDetect_LockAutoPWROff = TRUE;
}
void UI_UnlockAutoSleep(void)
{
	bDetect_LockSleep = FALSE;
}
void UI_UnlockAutoPWROff(void)
{
	bDetect_LockAutoPWROff = FALSE;
}

UINT32 UI_IsForceLockStatus(void)
{
	return bDetect_ForceLockStatus;
}
UINT32 UI_IsForceLock(void)
{
	return bDetect_ForceLock;
}

//lock mode key,before Sx job start to change mode
void UI_LockStatusEvent()
{
	bDetect_ForceLockStatus = TRUE;
}
void UI_LockEvent(void)
{
	bDetect_ForceLock = TRUE;

	//PwrKey,NormalKey,StatusKey: 避免event queue裡面的key event還是會被執行
	//注意!!! 不能flush release key, 否則很可能會造成UI異常
	// ==> 範例: TV插拔的同時, 按下S1再放開S1, UI的對焦框不消失(機率性)
	// ==> 不過UI也要做到確保Press/Release成對處理的程序(如果沒有收到Press,就不要管Release)
	//Ux_FlushEventByRange(NVTEVT_KEY_PRESS_START,NVTEVT_KEY_PRESS_END); //不能flush release key
	//TV
	//USB
	//Battery
	//Sleep
	//AutoPoweroff
}
//unlock mode key,after Sx job end change mode
void UI_UnlockEvent(void)
{
	bDetect_ForceLock = FALSE;
	bDetect_ForceLockStatus = FALSE;
}

