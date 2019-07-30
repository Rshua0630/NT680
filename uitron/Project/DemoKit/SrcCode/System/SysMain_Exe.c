/*
    System Main Callback

    System Callback for System Module.

    @file       SysMain_Exe.c
    @ingroup    mIPRJSYS

    @note

    Copyright   Novatek Microelectronics Corp. 2010.  All rights reserved.
*/

////////////////////////////////////////////////////////////////////////////////
#include "SysCommon.h"
#include "AppCommon.h"
////////////////////////////////////////////////////////////////////////////////
#include "UIFrameworkExt.h"
#include "UICommon.h"
#include "AppLib.h"
#include "UIAppCommon.h"

#if(UI_FUNC==ENABLE)
#include "UIBackgroundObj.h"
#include "SoundData.h"
#endif
#if (GPS_FUNCTION == ENABLE)
#include "GPS.h"
#endif
#include "DCF.h"
#include "dma.h"
#include "UsbDevDef.h"
#include "NvtIpcAPI.h"
//global debug level: PRJ_DBG_LVL
#include "PrjCfg.h"
#include "SysCfg.h"
#include "ImageUnit_VdoIn.h"
#include "ImageUnit_VdoOut.h"
//local debug level: THIS_DBGLVL
#if (PIP_VIEW_FUNC == ENABLE)
#include "AppDisp_PipView.h"
#endif
#if (UCTRL_FUNC)
#include "UCtrlMain.h"
#endif
#if(IPCAMPB_MODE == ENABLE)
#include "UIModeIPCamPB.h"
#endif

#include "bin_info.h"
#include "DrvExt.h"
#define THIS_DBGLVL         2 // 0=FATAL, 1=ERR, 2=WRN, 3=UNIT, 4=FUNC, 5=IND, 6=MSG, 7=VALUE, 8=USER
///////////////////////////////////////////////////////////////////////////////
#define __MODULE__          SysMainExe
#define __DBGLVL__          ((THIS_DBGLVL>=PRJ_DBG_LVL)?THIS_DBGLVL:PRJ_DBG_LVL)
#define __DBGFLT__          "*" //*=All, [mark]=CustomClass
#include "DebugModule.h"
///////////////////////////////////////////////////////////////////////////////

extern VControl SystemObjCtrl;
//extern VControl UISetupObjCtrl;
#if(UI_FUNC==ENABLE)
extern VControl *gUxCtrlTypeList[];
#endif
extern VControl *gAppCtrlTypeList[];

#if (DISPLAY_FAST_SWITCH == ENABLE)
extern VControl FlowCommonCtrl;
#endif
#if(WIFI_AP_FUNC==ENABLE)
extern VControl UIWifiCmdObjCtrl;
#endif

void System_InstallAppObj(void)
{
	//Init object list for UIControl event process

#if(UI_FUNC==ENABLE)
	//register UI control type (from UIControl Lib)
	Ux_SetUITypeList(gUxCtrlTypeList, CTRL_EX_TYPE_MAX - CTRL_TYPE_MIN);

#if (DISPLAY_FAST_SWITCH == ENABLE)
	//register FlowCommonCtrl as base ctrl of CTRL_WND
	gUxCtrlTypeList[CTRL_USER - CTRL_TYPE_MIN] = &FlowCommonCtrl;
	FlowCommonCtrl.wType = CTRL_BASE;
	gUxCtrlTypeList[CTRL_WND - CTRL_TYPE_MIN]->wType = CTRL_USER;
#endif

#if(WIFI_AP_FUNC==ENABLE)
	gAppCtrlTypeList[APP_WIFICMD - APP_TYPE_MIN] = &UIWifiCmdObjCtrl; //set APP_WIFICMD = WifiCmdObjCtrl
#endif
#endif
	//register App control type (from AppControl Lib)
	Ux_SetAppTypeList(gAppCtrlTypeList, APP_TYPE_MAX - APP_TYPE_MIN);

	//set default active app = current system object
	Ux_SetActiveApp(&UISetupObjCtrl);
}

///////////////////////////////////////////////////////////////////////
#include "GxTimer.h"

int SX_TIMER_DET_TIMER_ID = -1;
void Timer_CB(UINT32 event, UINT32 param1, UINT32 param2);

SX_TIMER_ITEM(System_CntTimer, GxTimer_CountTimer, 1, FALSE)

void System_OnTimerInit(void)
{
	//PHASE-1 : Init & Open Drv or DrvExt
	{
		GxTimer_RegCB(Timer_CB);         //Register CB function of GxTimer
		SX_TIMER_DET_TIMER_ID = SxTimer_AddItem(&Timer_System_CntTimer);
		//5.µù¥USxCmdªA°È ---------> Cmd Function
		//HwClock_InstallCmd();
	}
	{
		//PHASE-2 : Init & Open Lib or LibExt
	}
}

void System_OnTimerExit(void)
{
	//PHASE-2 : Close Lib or LibExt
	{
	}
	//PHASE-1 : Close Drv or DrvExt
	{
	}
}

///////////////////////////////////////////////////////////////////////
//System flow event

#if (PWR_FUNC == ENABLE)
#include "GxPower.h"
#endif

extern UINT32 Input_GroupStatus2Event(UINT32 status);
extern BOOL g_bConsumeStatus;
extern BOOL gbIsPlayBackModePowerOn;
extern INT32 System_GetFirstSensorMode(INT32 iCurrMode);

#if 0
INT32 System_GetPoweronModeByFWInfo(void)
{
	MODELEXT_HEADER *header = NULL;
	BININFO *pBinInfo = (BININFO *)Dx_GetModelExtCfg(MODELEXT_TYPE_BIN_INFO, &header);
	if (!pBinInfo || !header) {
		DBG_ERR("bininfo is null\r\n");
		return System_GetFirstSensorMode(PRIMARY_MODE_MAIN);
	}
	if (pBinInfo->fw.Resv[0]== 0)
		return System_GetFirstSensorMode(PRIMARY_MODE_MAIN);
	else
		return System_GetFirstSensorMode(PRIMARY_MODE_IPCAM);

}
#endif

INT32 System_GetBootFirstMode(void)
{
#if (IPCAM_MODE == ENABLE)
	#if 0
	return System_GetPoweronModeByFWInfo();
	#else
	return System_GetFirstSensorMode(PRIMARY_MODE_IPCAM);
	#endif
#elif(IPCAMPB_MODE  == ENABLE)
	return System_GetFirstSensorMode(PRIMARY_MODE_IPCAMPB);
#elif (MOVIE_MODE == ENABLE)
	return System_GetFirstSensorMode(PRIMARY_MODE_MOVIE);
#else
	return System_GetFirstSensorMode(PRIMARY_MODE_MAIN);
	//return System_GetFirstSensorMode(PRIMARY_MODE_PHOTO);
#endif
}


INT32 System_OnBoot(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
	DBG_DUMP("^MOn Boot begin\r\n");
	if (paramNum == 1 && paramArray[0] == 0) { //power-on start
		//System_BootStart();
	}
	if (paramNum == 1 && paramArray[0] == 1) { //power-on end
		//#NT#2012/10/18#Philex Lin - begin
		UINT32 uiChangeTo;
#if (POWERON_WAIT_FS_READY == ENABLE)
		UINT32 uiFSStatus;
#endif
		//#NT#2012/10/18#Philex Lin - end
#if !defined(_IrRx_NONE_)
		IRRX_DEVICE_OBJ IrRxDevice;
#endif
		TM_BOOT_BEGIN("flow", "boot");

		//ui init state
#if (UI_FUNC == ENABLE)
		//#NT#2016/12/14#Niven Cho -begin
		//#NT#EMMC_AS_PSTORE
#if defined(_CPU2_LINUX_) && defined(_EMBMEM_EMMC_)
		System_OnStrgInit_PS(); //because ps uses filesys in this case, so ps init was moved here.
#if (BOOT_RESET_MENU_INFO == ENABLE)
		Load_MenuInfo();
#endif
#else
#if (BOOT_RESET_MENU_INFO == ENABLE)
		//EMMC_AS_PSTORE
        #if (defined(_CPU2_LINUX_) && defined(_EMBMEM_EMMC_))
		System_OnStrgInit_PS(); //because ps uses filesys in this case, so ps init was moved here.
		Load_MenuInfo();
		#else
        Load_MenuInfo(); //reset value cause the FL_FSStatus as '0', we do so before NVTEVT_STRG_ATTACH finish
        #endif
#endif
#endif
		//#NT#2016/12/14#Niven Cho -end
#if (BOOT_RESET_MENU_INFO == ENABLE)
		Init_MenuInfo();
#endif
#endif

#if (UCTRL_FUNC)
		UctrlMain_Init();
#endif

		//wait fs init finish
#if (POWERON_WAIT_FS_READY == ENABLE)
		if (System_GetState(SYS_STATE_POWERON) == SYSTEM_POWERON_SAFE) {
#if(ONVIF_PROFILE_S!=ENABLE) //No File System
			//#NT#2016/12/22#Niven Cho -begin
			//#NT#MULTI_DRIVE, on linux, fix filedb create fail on drive B.
			UINT32 paramNum;
			UINT32 paramArray[MAX_MESSAGE_PARAM_NUM];
			UINT32 max_dev_id = (FS_MULTI_STRG_FUNC) ? 1 : 0;
			//FilesysWaitFinish(); //wait strg init finish
			//#NT#2016/12/14#Niven Cho -begin
			//#NT#EMMC_AS_PSTORE
			//After Filesys mounted, we can start to access pstron for the condition of (_CPU2_TYPE_==_CPU2_LINUX_ && _INTERSTORAGE_ == _INTERSTORAGE_EMMC_)
			do {
				UserWaitEvent(NVTEVT_STRG_ATTACH, &paramNum, paramArray); //(paramNum == 1 && paramArray[0] == 0) //power-on begin
			} while (paramArray[0] != max_dev_id);
			//#NT#2016/12/22#Niven Cho -end
			//#NT#2016/12/14#Niven Cho -end
#endif
		}

		uiFSStatus = System_GetState(SYS_STATE_FS); //keep card insert status

		if (System_GetState(SYS_STATE_POWERON) == SYSTEM_POWERON_SAFE) {
			if (System_OnStrgInit_EMBMEM_GetGxStrgType() == FST_FS_TYPE_UITRON) {
#if (IPCAM_FUNC != ENABLE)
				UI_SetFileDBFileID();
#endif
				// update card status again
				if (GxStrg_GetDeviceCtrl(0, CARD_READONLY)) {
					System_SetState(SYS_STATE_CARD, CARD_LOCKED);
				} else if (GxStrg_GetDeviceCtrl(0, CARD_INSERT)) {
					System_SetState(SYS_STATE_CARD, CARD_INSERTED);
				} else {
					System_SetState(SYS_STATE_CARD, CARD_REMOVED);
				}
				System_SetState(SYS_STATE_FS, uiFSStatus);
			}
		}
#endif

		//wait disp init finish
		//wait audio init finish
		//wait usb init finish

		SxTimer_SetFuncActive(SX_TIMER_DET_TIMER_ID, TRUE);
#if(UI_FUNC==ENABLE)
		BKG_SetExecFuncTable(gBackgroundExtFuncTable);
#endif

		//get first app
#if !defined(_IrRx_NONE_)
		// Open IrRx driver
		irrx_open((PIRRX_DEVICE_OBJ)&IrRxDevice);
#endif
		//#NT#2016/03/07#KCHong -begin
		//#NT#Low power timelapse function
#if (TIMELAPSE_LPR_FUNCTION == ENABLE)
		extern BOOL g_bBootFromALM;
		if (g_bBootFromALM == FALSE) {
#endif
			//#NT#2016/03/07#KCHong -end
#if (POWERONSOUND_FUNCTION == ENABLE)
			// init Volume to max,100
			GxSound_SetVolume(100);
			if (System_GetState(SYS_STATE_POWERON) == SYSTEM_POWERON_SAFE) {
				// add power up sound
#if(UI_FUNC==ENABLE)
				UISound_Play(DEMOSOUND_SOUND_POWERON_TONE);
#endif
			}
#endif
			//#NT#2016/03/07#KCHong -begin
			//#NT#Low power timelapse function
#if (TIMELAPSE_LPR_FUNCTION == ENABLE)
		}
#endif
		//#NT#2016/03/07#KCHong -end

		//#NT#2013/1/6#Philex Lin-begin
#if (GPS_FUNCTION == ENABLE)
		GPSRec_Open(UART_BAUDRATE_9600);
#endif
		//#NT#2013/1/6#Philex Lin-end

#if (BT_FUNC == ENABLE)
		System_OnBTInit();
#endif
#if (OUTPUT_FUNC == ENABLE)
		// Turn on LED
		GxLED_SetCtrl(KEYSCAN_LED_RED, SET_TOGGLE_LED, TRUE);
#endif

		//#NT#2016/02/26#Niven Cho -begin
		//#NT#Fix, with FAST_BOOT=EN, linux, no compress, no partial, failed to boot.
#if 0 //defined(_CPU2_LINUX_)
		if (System_GetState(SYS_STATE_POWERON) == SYSTEM_POWERON_NORMAL) {
			//not so fast, linux is not ready yet, sir....
			DBG_DUMP("Wait CPU2 ready...\r\n");
			System_CPU2_WaitReady();
			//not so fast, fs is not mount yet, sir....
			DBG_DUMP("Wait FileSys mount finish...\r\n");
			//#NT#2016/03/02#Niven Cho -begin
			//#NT#Fix FAST_BOOT + LINUX + without card, boot failed.
			UserWaitEvent(NVTEVT_STRG_CHANGE, NULL, NULL);
			//#NT#2016/03/02#Niven Cho -end
		}
#endif
		//#NT#2016/02/26#Niven Cho -end
		TM_BOOT_END("flow", "boot");
#if (POWERON_FAST_BOOT == ENABLE)
		INIT_SETFLAG(FLGINIT_MODE);
#endif
		//bind user mem to ImageStream
		ImageStream_ConfigUserDump();
		//open first app mode
		uiChangeTo = System_GetBootFirstMode();
#if ((POWERON_FAST_WIFI == ENABLE) && defined(_CPU2_LINUX_) && defined(_NETWORK_NONE_))
		UI_SetData(FL_WIFI_LINK, WIFI_LINK_OK);
		Ux_SendEvent(0, NVTEVT_SYSTEM_MODE, 2, uiChangeTo, SYS_SUBMODE_WIFI);
#elif ((POWERON_FAST_WIFI == ENABLE) && defined(_CPU2_ECOS_) && defined(_NETWORK_NONE_))
		UI_SetData(FL_WIFI_LINK, WIFI_LINK_OK);
		Ux_SendEvent(0, NVTEVT_SYSTEM_MODE, 2, uiChangeTo, SYS_SUBMODE_WIFI);
#else
		Ux_SendEvent(0, NVTEVT_SYSTEM_MODE, 1, uiChangeTo);
#endif
#if (UI_FUNC == ENABLE)
		UI_SetData(FL_NextMode, uiChangeTo);
#endif

		//!!!!! After boot, MUST reset power-on-state to SAFE for next time NVTEVT_SYSTEM_MODE !!!!!
		System_SetState(SYS_STATE_POWERON, SYSTEM_POWERON_SAFE);

	}
	DBG_DUMP("^MOn Boot end\r\n");
	return NVTEVT_CONSUME;
}

extern void UI_Quit(void);
extern UINT32 User_GetTempBuffer(UINT32 uiSize);

INT32 System_OnShutdown(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
	DBG_DUMP("^MOn Shutdown begin\r\n");
	if (paramNum == 1 && paramArray[0] == 0) { //power-off begin
#if (POWEROFFLOGO_FUNCTION != ENABLE)
		if ((DX_HANDLE)GxVideo_GetDevice(DOUT1)) {
			GxVideo_SetDeviceCtrl(DOUT1, DISPLAY_DEVCTRL_BACKLIGHT, FALSE);
		}
#if (DISPLAY2_FUNC == ENABLE)
		if ((DX_HANDLE)GxVideo_GetDevice(DOUT2)) {
			GxVideo_SetDeviceCtrl(DOUT1, DISPLAY_DEVCTRL_BACKLIGHT, FALSE);
		}
#endif
#endif

#if (UI_FUNC == ENABLE)
		// save RTC date data and file ID
		UI_SaveRTCDate();
#endif

		Display_SetEnable(LAYER_VDO1, FALSE); //turn off VDO1 to avoid garbage frame.

		//close last app mode
		Ux_SendEvent(0, NVTEVT_SYSTEM_MODE, 1, SYS_MODE_UNKNOWN);

		//register Project level Memory Provide API for Command Parser Interface
		SxCmd_RegTempMemFunc(User_GetTempBuffer);

#if _TODO //(IPCAM_FUNC == ENABLE)
		if (UI_GetData(FL_WIFI_LINK) == WIFI_LINK_OK) {
			//should close network application,then stop wifi
			Ux_SendEvent(&UISetupObjCtrl, NVTEVT_EXE_WIFI_STOP, 0);
		}
#else
		debug_msg("_TODO -> %s:%d, task(%x,%s) caller(%pF)\r\n", __FUNCTION__, __LINE__,
				  OS_GetTaskID(), OS_GetTaskName(), __builtin_return_address(0));
#endif

		// low battery, lens error power off tone and screen
		if (System_GetState(SYS_STATE_POWEROFF) == SYS_POWEROFF_BATTEMPTY) {
			DBG_ERR("low battery **********\r\n");
		} else if (System_GetState(SYS_STATE_POWEROFF) == SYS_POWEROFF_LENSERROR) {
			DBG_ERR("lens error **********\r\n");
		} else {
#if (POWEROFFLOGO_FUNCTION == ENABLE)
			GxDisplay_Set(LAYER_OSD1, LAYER_STATE_ENABLE, 0); //turn off OSD1
			GxDisplay_Set(LAYER_OSD2, LAYER_STATE_ENABLE, 0); //turn off OSD2
			GxDisplay_Flush(LAYER_OSD1);
			GxDisplay_Flush(LAYER_OSD2);
			GxDisplay_Set(LAYER_VDO1, LAYER_STATE_ENABLE, 0); //turn off VDO1
			GxDisplay_Flush(LAYER_VDO1);
			Display_ShowSplash(SPLASH_POWEROFF);
			SwTimer_DelayMs(500);
#endif
		}

#if (UI_FUNC == ENABLE)
		Save_MenuInfo();
#endif
#if (BT_FUNC == ENABLE)
		System_OnBTExit();
#endif

		//#NT#2016/03/07#KCHong -begin
		//#NT#Low power timelapse function
#if (TIMELAPSE_LPR_FUNCTION == ENABLE)
		// Do not enable key and play power on tone
		extern BOOL g_bBootFromALM;
		if (g_bBootFromALM == FALSE) {
#endif
			//#NT#2016/03/07#KCHong -end
#if (POWEROFFSOUND_FUNCTION == ENABLE)
			//power off sound
#if(UI_FUNC==ENABLE)
			UISound_EnableKey(TRUE);
			UISound_Play(DEMOSOUND_SOUND_POWERON_TONE);
#endif
#endif
			//#NT#2016/03/07#KCHong -begin
			//#NT#Low power timelapse function
#if (TIMELAPSE_LPR_FUNCTION == ENABLE)
		}
#endif
		//#NT#2016/03/07#KCHong -end
		UI_LockEvent();

		UI_Quit(); //quit from event loop
	}
	if (paramNum == 1 && paramArray[0] == 1) { //power-off end
		//System_ShutDownEnd();
		//bQuit = TRUE;
	}
	DBG_DUMP("^MOn Shutdown end\r\n");
	return NVTEVT_CONSUME;
}

INT32 System_OnMode(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
	static BOOL bFirst = TRUE;
	int mode_id;
	if (paramNum < 1) {
		return NVTEVT_CONSUME;
	}

	mode_id = paramArray[0];
	if (paramNum == 2) {
		System_SetState(SYS_STATE_NEXTSUBMODE, paramArray[1]);
	}
	if (bFirst) {
		TM_BOOT_BEGIN("flow", "mode");
	}
	System_ChangeMode(mode_id);
	if (bFirst) {
		TM_BOOT_END("flow", "mode");
	}
	bFirst = FALSE;

	return NVTEVT_CONSUME;
}

INT32 System_OnSleepLevel(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
	int sleep_lv;
	if (paramNum < 1) {
		return NVTEVT_CONSUME;
	}

	sleep_lv = paramArray[0];
	System_ChangeSleep(sleep_lv);

	return NVTEVT_CONSUME;
}

extern UINT32 System_GetConnectDisplay(void);
extern UINT32 System_GetConnectSensor(void);

INT32 System_OnAppOpen(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
	//UINT32 nDisp, uDispMask;
	UINT32 nSensor, uSensorMask, iSensorDisp;
	DBG_IND("\r\n");

	//System_ChangeMode(mode); //open phase
	//
	//  1. UIModeXXX.c::ModeXXX_Open()
	//      Ux_SetActiveApp(&AppObj);
	//      Ux_SendEvent(0, NVTEVT_EXE_OPEN, 0);
	//  2. UIAppXXX_Exe.c::XXXExe_OnOpen()
	//      Ux_DefaultEvent(pCtrl,NVTEVT_EXE_OPEN,paramNum,paramArray);
	//  3. SysMain_Exe.c::System_OnAppOpen()
	//      Ux_SendEvent(0, NVTEVT_SENSOR_ATTACH, 0);
	//      Ux_SendEvent(0, NVTEVT_LENS_ATTACH, 0);
	//             wait device control finish
	//  4. UIModeXXX.c::ModeXXX_Open()
	//      Ux_OpenWindow();
	DBG_IND("^M -display attach\r\n");
	Ux_SendEvent(0, NVTEVT_VIDEO_ATTACH, 0);
	//nDisp = System_GetEnableDispCount(); //return 0/1/2
	//uDispMask = System_GetEnableDisp(); //return 0x00/0x01/0x02/0x03
	//DBG_DUMP(" -display count=%d, insert_mask=%02x, attach_mask=%02x\r\n", nDisp, System_GetConnectDisplay(), uDispMask);
	DBG_IND("^M -sensor attach\r\n");
	Ux_SendEvent(0, NVTEVT_SENSOR_ATTACH, 0);
	nSensor = ImageUnit_GetParam(ISF_VIN(0), ISF_CTRL, VDOIN_PARAM_SENSORCOUNT); //return 0/1/2/4
	uSensorMask = ImageUnit_GetParam(ISF_VIN(0), ISF_CTRL, VDOIN_PARAM_SENSORMASK);
	iSensorDisp = ImageUnit_GetParam(ISF_VIN(0), ISF_CTRL, VDOIN_PARAM_SENSORDISPLAY);
	DBG_DUMP(" -sensor count=%d, insert_mask=%04x, attach_mask=%04x, disp_mask=%04x\r\n", nSensor, System_GetConnectSensor(), uSensorMask, iSensorDisp);
	DBG_IND("^M -lens attach\r\n");
	Ux_SendEvent(0, NVTEVT_LENS_ATTACH, 0);
	//DBG_DUMP(" -lens count=%d\r\n", 0);

	return NVTEVT_CONSUME;
}

INT32 System_OnAppClose(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
	DBG_IND("\r\n");

	//System_ChangeMode(mode); //close phase
	//
	//  4. UIModeXXX.c::ModeXXX_Close()
	//      Ux_CloseWindow();
	//  3. UIModeXXX.c::ModeXXX_Close()
	//      Ux_SendEvent(0, NVTEVT_EXE_CLOSE, 0);
	//  2. UIAppXXX_Exe.c::XXXExe_OnClose()
	//      Ux_DefaultEvent(pCtrl,NVTEVT_EXE_CLOSE,paramNum,paramArray);
	//  1. SysMain_Exe.c::System_OnAppClose()
	//      Ux_SendEvent(0, NVTEVT_SENSOR_DETACH, 0);
	//      Ux_SendEvent(0, NVTEVT_LENS_DETACH, 0);
	Ux_SendEvent(0, NVTEVT_VIDEO_DETACH, 0);
	Ux_SendEvent(0, NVTEVT_SENSOR_DETACH, 0);
	Ux_SendEvent(0, NVTEVT_LENS_DETACH, 0);

	return NVTEVT_CONSUME;
}

void Timer_CB(UINT32 event, UINT32 param1, UINT32 param2)
{
	Ux_PostEvent(NVTEVT_TIMER, 1, event);
}

///////////////////////////////////////////////////////////////////////////////
//Device flow event
INT32 System_OnVideoInsert(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray);
INT32 System_OnVideoRemove(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray);
INT32 System_OnVideoAttach(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray);
INT32 System_OnVideoDetach(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray);
INT32 System_OnAudioInsert(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray);
INT32 System_OnAudioRemove(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray);
INT32 System_OnAudioAttach(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray);
INT32 System_OnAudioDetach(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray);
INT32 System_OnAudioMode(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray);
INT32 System_OnAudioInput(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray);
INT32 System_OnAudioVol(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray);
INT32 System_OnStrgInsert(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray);
INT32 System_OnStrgRemove(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray);
INT32 System_OnStrgAttach(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray);
INT32 System_OnStrgDetach(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray);
INT32 System_OnUsbInsert(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray);
INT32 System_OnUsbRemove(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray);
INT32 System_OnUsbChargeCurrent(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray);
INT32 System_OnBattEmpty(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray);
INT32 System_OnBattChange(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray);
INT32 System_OnPwrChargeOK(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray);
INT32 System_OnPwrChargeSuspend(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray);
INT32 System_OnPwrChargeResume(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray);

INT32 System_OnVideoDir(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray);
INT32 System_OnVideoMode(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray);

INT32 System_OnSensorInsert(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray);
INT32 System_OnSensorRemove(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray);
INT32 System_OnSensorAttach(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray);
INT32 System_OnSensorDetach(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray);
INT32 System_OnSensorDisplay(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray);
INT32 System_OnLensAttach(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray);
INT32 System_OnLensDetach(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray);

/*
INT32 System_OnDefAttach(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    //  PWR-KEY:
    //  NORMAL-KEY:
    //  STATUS-KEY:
    //  TOUCH:
    //  LED:
    //  BATTERY:
    return NVTEVT_CONSUME;
}

INT32 System_OnSensorAttach(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    //  SENSOR
    return NVTEVT_CONSUME;
}

INT32 System_OnLensAttach(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    //  LENS
    return NVTEVT_CONSUME;
}
*/

EVENT_ENTRY SystemObjCmdMap[] = {
////////////////////////////////////////////////////////////

	//System flow event
	{NVTEVT_SYSTEM_BOOT,             System_OnBoot              },
	{NVTEVT_SYSTEM_SHUTDOWN,         System_OnShutdown          },
	{NVTEVT_SYSTEM_MODE,             System_OnMode              },
	{NVTEVT_SYSTEM_SLEEP,            System_OnSleepLevel        },
	//App common event
	{NVTEVT_EXE_OPEN,                System_OnAppOpen           },
	{NVTEVT_EXE_CLOSE,               System_OnAppClose          },

	//Video device event
	{NVTEVT_VIDEO_INSERT,            System_OnVideoInsert       },
	{NVTEVT_VIDEO_REMOVE,            System_OnVideoRemove       },
	{NVTEVT_VIDEO_ATTACH,            System_OnVideoAttach       },
	{NVTEVT_VIDEO_DETACH,            System_OnVideoDetach       },
	{NVTEVT_VIDEO_MODE,              System_OnVideoMode         },
	{NVTEVT_VIDEO_DIR,               System_OnVideoDir          },
	//Audio device event
	{NVTEVT_AUDIO_INSERT,            System_OnAudioInsert       },
	{NVTEVT_AUDIO_REMOVE,            System_OnAudioRemove       },
	{NVTEVT_AUDIO_ATTACH,            System_OnAudioAttach       },
	{NVTEVT_AUDIO_DETACH,            System_OnAudioDetach       },
	{NVTEVT_AUDIO_INPUT,             System_OnAudioInput        },
	{NVTEVT_AUDIO_PLAY_VOL,          System_OnAudioVol          },
	{NVTEVT_AUDIO_MODE,              System_OnAudioMode         },
	//Storage device event
	{NVTEVT_STRG_INSERT,             System_OnStrgInsert        },
	{NVTEVT_STRG_REMOVE,             System_OnStrgRemove        },
	{NVTEVT_STRG_ATTACH,             System_OnStrgAttach        },
	{NVTEVT_STRG_DETACH,             System_OnStrgDetach        },
#if(USB_MODE==ENABLE)
	//Usb device event
	{NVTEVT_USB_INSERT,              System_OnUsbInsert         },
	{NVTEVT_USB_REMOVE,              System_OnUsbRemove         },
	{NVTEVT_USB_CHARGE_CURRENT,      System_OnUsbChargeCurrent  },
#endif
#if (PWR_FUNC == ENABLE)
	//power device event
	{NVTEVT_PWR_BATTEMPTY,           System_OnBattEmpty         },
	{NVTEVT_PWR_BATTCHANGE,          System_OnBattChange        },
	{NVTEVT_PWR_CHARGE_OK,           System_OnPwrChargeOK       },
	{NVTEVT_PWR_CHARGE_SUSPEND,      System_OnPwrChargeSuspend  },
	{NVTEVT_PWR_CHARGE_RESUME,       System_OnPwrChargeResume   },
#endif
	//sensor device event
	{NVTEVT_SENSOR_INSERT,           System_OnSensorInsert      },
	{NVTEVT_SENSOR_REMOVE,           System_OnSensorRemove      },
	{NVTEVT_SENSOR_ATTACH,           System_OnSensorAttach      },
	{NVTEVT_SENSOR_DETACH,           System_OnSensorDetach      },
	{NVTEVT_SENSOR_DISPLAY,          System_OnSensorDisplay     },
#if (LENS_FUNCTION == ENABLE)
	{NVTEVT_LENS_ATTACH,             System_OnLensAttach        },
	{NVTEVT_LENS_DETACH,             System_OnLensDetach        },
#endif

	//Other device event
	//{NVTEVT_DEFAULT_ATTACH,        System_OnDefAttach         },

	{NVTEVT_NULL,                    0},  //End of Command Map
};

CREATE_APP(SystemObj, 0)

