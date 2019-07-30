#include "PrjCfg.h"
#include "UIFramework.h"
#include "UIAppWiFiCmd.h"
#include "AppLib.h"
#include "UIFlow.h"
#include "WifiAppCmd.h"
#if !defined(_Gyro_None_)
#include "rsc_common.h"
#endif
#include "UIAppWiFiCmdMovie.h"
#include "UIAppWiFiCmdPhoto.h"
#include "ImageApp_Movie.h"
#include "ImageApp_Photo.h"
#include "ImageApp_MovieMulti.h"

///////////////////////////////////////////////////////////////////////////////
#define __MODULE__          UIAppWiFiCmd
#define __DBGLVL__          2 // 0=FATAL, 1=ERR, 2=WRN, 3=UNIT, 4=FUNC, 5=IND, 6=MSG, 7=VALUE, 8=USER
#define __DBGFLT__          "*" //*=All, [mark]=CustomClass
#include "DebugModule.h"

typedef enum {
	NETMOVIE_CFG_ID_1  =   0,
} NETMOVIE_CFG_ID;

#if (WIFI_AP_FUNC==ENABLE)
UINT32 ImageApp_NetMovie_GetConfig(NETMOVIE_CFG_ID config_id)
{
	DBG_ERR("not support");
	return 0;
}
CHAR *ImageApp_CamFile_GetLastWriteFilePath(void)
{
	DBG_ERR("not support");
	return 0;
}

//----------------------------------------------------------------------------

//#NT#2016/03/23#Isiah Chang -begin
//#NT#add new Wi-Fi UI flow.
static UINT32 uiWifiAppHBCnt  = 0;
static BOOL   bWifiAppHBStart = FALSE;
static UINT32  gUIMenuWndWiFiHeartBeatTimerID = NULL_TIMER;

//#NT#2016/03/23#Isiah Chang -end

INT32 WiFiCmd_OnExeModeChange(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
	UINT32 par = 0;
	INT32 curMode = System_GetState(SYS_STATE_CURRMODE);
//#NT#2016/08/31#Isiah Chang -begin
//#NT#Parameter number must be larger then 0. Fixed mantis bug id:0107729
	if (paramNum > 0) {
		par = paramArray[0];
	} else {
		WifiCmd_Done(WIFIFLAG_MODE_DONE, WIFIAPP_RET_PAR_ERR);
		DBG_ERR("too few parameter!\r\n");
		return NVTEVT_CONSUME;
	}
//#NT#2016/08/31#Isiah Chang -end
	switch (par) {
	case WIFI_APP_MODE_PHOTO:
		//#NT#2015/07/24#KS Hung -begin
		if (curMode != PRIMARY_MODE_PHOTO) {
			if (curMode == PRIMARY_MODE_MOVIE) {
#if !defined(_Gyro_None_)
				MovieExe_RSC_SetSwitch(SEL_RSC_RUNTIME, SEL_RSC_OFF);
#endif
			}
			Ux_PostEvent(NVTEVT_SYSTEM_MODE, 1, PRIMARY_MODE_PHOTO);
		} else {
			WifiCmd_Done(WIFIFLAG_MODE_DONE, WIFIAPP_RET_OK);
		}
		//#NT#2015/07/24#KS Hung -end
		break;
	case WIFI_APP_MODE_MOVIE:
		//#NT#2015/07/24#KS Hung -begin
		if (curMode != PRIMARY_MODE_MOVIE) {
			if (curMode == PRIMARY_MODE_PHOTO) {
#if !defined(_Gyro_None_)
				PhotoExe_RSC_SetSwitch(SEL_RSC_RUNTIME, SEL_RSC_OFF);
#endif
			}
			Ux_PostEvent(NVTEVT_SYSTEM_MODE, 1, PRIMARY_MODE_MOVIE);
		}
		//#NT#2015/07/24#KS Hung -end
		else {
			WifiCmd_Done(WIFIFLAG_MODE_DONE, WIFIAPP_RET_OK);
		}
		break;
	case WIFI_APP_MODE_PLAYBACK:
		if (curMode != PRIMARY_MODE_PLAYBACK) {
			Ux_PostEvent(NVTEVT_SYSTEM_MODE, 1, PRIMARY_MODE_PLAYBACK);
		} else {
			WifiCmd_Done(WIFIFLAG_MODE_DONE, WIFIAPP_RET_OK);
		}
		break;
	default:
		WifiCmd_Done(WIFIFLAG_MODE_DONE, WIFIAPP_RET_PAR_ERR);
		break;
	}

	return NVTEVT_CONSUME;
}

INT32 WiFiCmd_OnExeSetSSID(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
	DBG_IND("%s %s\r\n", __FUNCTION__, paramArray[0]);
	char *Data = 0;
	char SSID[WIFI_PAR_STR_LEN] = {0};
	if (paramNum) {
		Data = (char *)paramArray[0];
		DBG_IND("%d\r\n", strlen(Data));

		if (1 == sscanf_s(Data, "%s", SSID, sizeof(SSID))) {
			Ux_SendEvent(0, NVTEVT_EXE_WIFI_SET_SSID, 1, SSID);
		}
		WifiCmd_UnlockString(Data);
	}
	return NVTEVT_CONSUME;
}

INT32 WiFiCmd_OnExeSetPassphrase(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
	DBG_IND("%s %s\r\n", __FUNCTION__, paramArray[0]);
	char *Data = 0;
	char Passphrase[WIFI_PAR_STR_LEN] = {0};

	if (paramNum) {
		Data = (char *)paramArray[0];
		DBG_IND("%d\r\n", strlen(Data));

		if (1 == sscanf_s(Data, "%s", Passphrase, sizeof(Passphrase))) {
			if (strlen(Passphrase) >= 8) {
				Ux_SendEvent(0, NVTEVT_EXE_WIFI_SET_PASSPHRASE, 1, Passphrase);
			} else {
				DBG_ERR("Passphrase length illegal!\r\n");
			}
		}
		WifiCmd_UnlockString(Data);
	}

	return NVTEVT_CONSUME;
}

INT32 WiFiCmd_OnExeDate(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
	char *Data = 0;
	UINT32 uiYear, uiMonth, uiDay;
	DBG_IND("%s %s\r\n", __FUNCTION__, paramArray[0]);
	if (paramNum) {
		Data = (char *)paramArray[0];
		DBG_IND("%d\r\n", strlen(Data));

		if (3 == sscanf_s(Data, "%d-%d-%d;", &uiYear, &uiMonth, &uiDay)) {
			WifiCmd_UnlockString(Data);//setDate cost time,unlock string before rtc_setDate
			DBG_IND("Y:%d, M:%d, D:%d\r\n", uiYear, uiMonth, uiDay);
		    Ux_SendEvent(&UISetupObjCtrl,NVTEVT_EXE_DATE,3, uiYear, uiMonth, uiDay);

		} else {
			WifiCmd_UnlockString(Data);
		}
	}

	return NVTEVT_CONSUME;
}

INT32 WiFiCmd_OnExeTime(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
	char *Data = 0;
	UINT32 uiHour, uiMin, uiSec;

	DBG_IND("%s %s\r\n", __FUNCTION__, paramArray[0]);
	if (paramNum) {
		Data = (char *)paramArray[0];
		DBG_IND("%d\r\n", strlen(Data));

		if (3 == sscanf_s(Data, "%d:%d:%d;", &uiHour, &uiMin, &uiSec)) {
			WifiCmd_UnlockString(Data);//setTime cost time,unlock string before rtc_setTime
			DBG_IND("hh:%d, mm:%d, dd:%d\r\n", uiHour, uiMin, uiSec);
		    Ux_SendEvent(&UISetupObjCtrl,NVTEVT_EXE_TIME,3,uiHour, uiMin, uiSec);
		} else {
			WifiCmd_UnlockString(Data);
		}
	}

	return NVTEVT_CONSUME;
}

INT32 WiFiCmd_OnExePoweroff(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
	DBG_IND("%s %d\r\n", __FUNCTION__, paramArray[0]);
	UINT32 Data = 0;
	if (paramNum) {
		Data = paramArray[0];
	}
	Ux_SendEvent(0, NVTEVT_EXE_POWEROFF, 1, Data);
	return NVTEVT_CONSUME;
}

INT32 WiFiCmd_OnExeLanguage(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
	DBG_IND("%s %d\r\n", __FUNCTION__, paramArray[0]);
	UINT32 Data = 0;
	if (paramNum) {
		Data = paramArray[0];
	}
	Ux_SendEvent(0, NVTEVT_EXE_LANGUAGE, 1, Data);
	return NVTEVT_CONSUME;
}

INT32 WiFiCmd_OnExeTV(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
	DBG_IND("%s %d\r\n", __FUNCTION__, paramArray[0]);
#if (TV_FUNC == ENABLE)
	UINT32 Data = 0;
	if (paramNum) {
		Data = paramArray[0];
	}

	Ux_SendEvent(0, NVTEVT_EXE_TVFORMAT, 1, Data);
#else
	debug_err(("TV function not support!\r\n"));
#endif

	return NVTEVT_CONSUME;
}
INT32 WiFiCmd_OnExeFormat(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
	DBG_IND("%s %d\r\n", __FUNCTION__, paramArray[0]);
	UINT32 Data = 0;
	if (paramNum) {
		Data = paramArray[0];
	}
	if (Data == 0) {
		BKG_PostEvent(NVTEVT_BKW_FORMAT_NAND);
	} else if (Data == 1) {
		if (System_GetState(SYS_STATE_CARD) == CARD_REMOVED) {
			DBG_ERR("card removed\r\n");
			WifiCmd_Done(WIFIFLAG_FORMAT_DONE, E_NOEXS);

		} else if (UIStorageCheck(STORAGE_CHECK_LOCKED, NULL) == TRUE) {
			DBG_ERR("card lock\r\n");
			WifiCmd_Done(WIFIFLAG_FORMAT_DONE, E_OBJ);
		} else {
			BKG_PostEvent(NVTEVT_BKW_FORMAT_CARD);
		}
	} else {
		DBG_ERR("err param\r\n");
		WifiCmd_Done(WIFIFLAG_FORMAT_DONE, E_PAR);
	}
	return NVTEVT_CONSUME;
}
INT32 WiFiCmd_OnExeSysReset(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
	DBG_IND("%s \r\n", __FUNCTION__);
	Ux_SendEvent(0, NVTEVT_EXE_SYSRESET, 0);
	return NVTEVT_CONSUME;
}

INT32 WiFiCmd_OnExeFWUpdate(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
	DBG_IND("%s \r\n", __FUNCTION__);
	BKG_PostEvent(NVTEVT_BKW_FW_UPDATE);
	return NVTEVT_CONSUME;
}
INT32 WiFiCmd_OnExeReconnect(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
	DBG_IND("%s \r\n", __FUNCTION__);

	//before stop wifi should close RTSP/HTTP,and change to movie wifi mode after wifi start
	if (System_GetState(SYS_STATE_CURRMODE) == PRIMARY_MODE_MOVIE) {
#if MOVIE_MAPPING_MULTIREC
	ImageApp_MovieMulti_Close();
#else
	ImageApp_Movie_Close();
#endif
	} else if (System_GetState(SYS_STATE_CURRMODE) == PRIMARY_MODE_PHOTO) {
		ImageApp_Photo_Close();
	}

	//#NT#2016/12/22#YongChang Qui -begin
	//#NT#Use new rule to transmit to wifi station mode from ap mode
	UINT32 Data = 0;
	if (paramNum) {
		Data = paramArray[0];
	}
	if (Data & SEND_PEER_IP) {
		UINet_SetSendPeerIP(1);
	} else {
		UINet_SetSendPeerIP(0);
	}
	//#NT#2016/12/22#YongChang Qui -end

	Ux_SendEvent(0, NVTEVT_EXE_WIFI_STOP, 0);
	Ux_SendEvent(0, NVTEVT_EXE_WIFI_START, 0);

	Ux_PostEvent(NVTEVT_SYSTEM_MODE, 2, PRIMARY_MODE_MOVIE, SYS_SUBMODE_WIFI);
	return NVTEVT_CONSUME;
}
INT32 WiFiCmd_OnExeSaveMenu(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
	DBG_IND("%s \r\n", __FUNCTION__);
	Save_MenuInfo();
	return NVTEVT_CONSUME;
}
INT32 WiFiCmd_OnExeRemoveUser(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
	DBG_IND("%s \r\n", __FUNCTION__);
	UINet_RemoveUser();
	return NVTEVT_CONSUME;
}
INT32 WiFiCmd_OnExePipStyle(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
	DBG_IND("%s \r\n", __FUNCTION__);
	UINT32 Data = 0;
	if (paramNum) {
		Data = paramArray[0];
	}

	Ux_SendEvent(0, NVTEVT_EXE_DUALCAM, 1, Data);

	return NVTEVT_CONSUME;
}

INT32 WiFiCmd_OnExeSendSSIDPassphrase(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
	DBG_IND("%s \r\n", __FUNCTION__);
	UINT32 Data = 0;
	if (paramNum) {
		Data = paramArray[0];
	}

	Ux_SendEvent(0, NVTEVT_EXE_WIFI_SEND_SSID_PASSPHRASE, 1, Data);
	return NVTEVT_CONSUME;
}

INT32 WiFiCmd_OnExeSetWiFiConnectMode(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
	DBG_IND("%s \r\n", __FUNCTION__);
	UINT32 Data = 0;
	if (paramNum) {
		Data = paramArray[0];
	}

	Ux_SendEvent(0, NVTEVT_EXE_WIFI_MODE, 1, Data);
	return NVTEVT_CONSUME;
}

INT32 WiFiCmd_OnExeChangeModeDone(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    WifiCmd_Done(WIFIFLAG_MODE_DONE, WIFIAPP_RET_OK);
	return NVTEVT_CONSUME;
}

void WiFiCmd_HBStart(void)
{
	uiWifiAppHBCnt  = 0;
	bWifiAppHBStart = FALSE;

	if (gUIMenuWndWiFiHeartBeatTimerID == NULL_TIMER) {
		gUIMenuWndWiFiHeartBeatTimerID = GxTimer_StartTimer(TIMER_ONE_SEC, NVTEVT_1SEC_TIMER, CONTINUE);
	}
}
void WiFiCmd_HBStop(void)
{
	if (gUIMenuWndWiFiHeartBeatTimerID != NULL_TIMER) {
		GxTimer_StopTimer(&gUIMenuWndWiFiHeartBeatTimerID);
	}

}
void WiFiCmd_HBOneSec(void)
{

	if (bWifiAppHBStart) {
		INT32 iLevel = 0;

		uiWifiAppHBCnt++;
		if (!(uiWifiAppHBCnt % 3)) {

			if (WiFiCmd_BRC_AutoRunStateGet()) {
				iLevel = -3; // Decrease the target bit rate level.
				WiFiCmd_BRCAdjust(iLevel);
			}
		} else if (uiWifiAppHBCnt >= 60) {
			DBG_ERR("HB timeout!!\r\n");
			uiWifiAppHBCnt = 0;
			bWifiAppHBStart = FALSE;

			iLevel = 999; // Restore target bit rate to the best level.
			WiFiCmd_BRCAdjust(iLevel);

			Ux_PostEvent(NVTEVT_WIFI_EXE_APP_SESSION_CLOSE, 0);
		}
	}

}
//#NT#2016/03/23#Isiah Chang -begin
//#NT#add new Wi-Fi UI flow.
INT32 WiFiCmd_OnExeHB(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
	CHAR heartbeat[] = "<?xml version=\"1.0\" encoding=\"UTF-8\" ?>\n<Function>\n<Cmd>3016</Cmd>\n<Status>0</Status>\n</Function>\n";
	int  sendsize = 0;

	if (paramNum) {
		DBG_IND("%s %d\r\n", __FUNCTION__, paramArray[0]);
	}

	sendsize = sizeof(heartbeat);

	UserSocket_Send(heartbeat, &sendsize);

	bWifiAppHBStart = TRUE;
	uiWifiAppHBCnt = 0;      // reset HB count

	return NVTEVT_CONSUME;
}

INT32 WiFiCmd_OnAppSessionClose(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
	debug_msg("Close Wi-Fi window\r\n");
	Ux_PostEvent(NVTEVT_SYSTEM_MODE, 2, System_GetState(SYS_STATE_CURRMODE), SYS_SUBMODE_NORMAL);
	//Ux_PostEvent(NVTEVT_KEY_SELECT, 1, NVTEVT_KEY_PRESS);
	return NVTEVT_CONSUME;
}
//#NT#2016/03/23#Isiah Chang -end

INT32 WiFiCmd_OnExeMovieVedioReady(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
//#NT#2016/05/31#Ben Wang -begin
//#NT#Add UVC multimedia function.
#if(UVC_MULTIMEDIA_FUNC == ENABLE)
	if (System_GetState(SYS_STATE_CURRSUBMODE) == SYS_SUBMODE_UVC) {
		if (ImageApp_UsbMovie_GetConfig(NVT_USBMOVIE_CFG_MEDIA_FMT) == NVT_USBMOVIE_MEDIA_H264) {
			WifiCmd_Done(WIFIFLAG_RECORD_DONE, WIFIAPP_RET_OK);
		}
		return NVTEVT_CONSUME;
	}
#endif
//#NT#2016/05/31#Ben Wang -end
#if _TODO
	if (ImageApp_NetMovie_GetConfig(NETMOVIE_CFG_MODE) == NETMOVIE_TYPE_RTSP) {
		if (paramNum && paramArray[0] == 1) {
//#NT#2016/12/02#Isiah Chang -begin
//#NT#Add WiFiCmd Bitrate control.
			UINT32 uiLevel = 0;
//#NT#2016/12/02#Isiah Chang -end

			//#NT#2016/12/30#Isiah Chang -begin
			// Send socket to notify APP movie recording is started.
			if (SysGetFlag(FL_MOVIE_MOTION_DET) == MOVIE_MOTIONDET_ON) {
				if (WiFiCmd_GetStatus() == WIFI_MOV_ST_RECORD) {
					WifiApp_SendCmd(WIFIAPP_CMD_NOTIFY_STATUS, WIFIAPP_RET_RECORD_STARTED);
				} else {
					WifiApp_SendCmd(WIFIAPP_CMD_NOTIFY_STATUS, WIFIAPP_RET_RECORD_STOPPED);
				}
			}
			//#NT#2016/12/30#Isiah Chang -end
			//#NT#2017/01/09#Janice Huang -begin
			//#NT#set done accord to wifi current cmd
			if (WifiCmd_GetCurCmd() == WIFIAPP_CMD_RECORD) {
				WifiCmd_Done(WIFIFLAG_RECORD_DONE, WIFIAPP_RET_OK);
			} else {
				WifiCmd_Done(WIFIFLAG_MODE_DONE, WIFIAPP_RET_OK);
			}
			//#NT#2017/01/09#Janice Huang -end

//#NT#2016/12/02#Isiah Chang -begin
//#NT#Add WiFiCmd Bitrate control.
			return WiFiCmd_BRCAdjust(uiLevel);
//#NT#2016/12/02#Isiah Chang -end
		}
	}
#endif
	return NVTEVT_CONSUME;
}


EVENT_ENTRY UIWifiCmdObjCmdMap[] = {
	{NVTEVT_WIFI_EXE_PHOTO_CAPTURE, WiFiCmd_OnExePhotoCapture},
	{NVTEVT_WIFI_EXE_PHOTO_SIZE, WiFiCmd_OnExeSetPhotoSize},

	{NVTEVT_WIFI_EXE_MOVIE_REC, WiFiCmd_OnExeMovieRec},
	{NVTEVT_WIFI_EXE_MOVIE_REC_SIZE, WiFiCmd_OnExeSetMovieRecSize},
	{NVTEVT_WIFI_EXE_CYCLIC_REC, WiFiCmd_OnExeCyclicRec},
	{NVTEVT_WIFI_EXE_MOVIE_WDR, WiFiCmd_OnExeMovieHDR},
	{NVTEVT_WIFI_EXE_MOVIE_EV, WiFiCmd_OnExeSetMovieEV},

	{NVTEVT_WIFI_EXE_MOVIE_CONTRAST, WiFiCmd_OnExeSetMovieContrast},
	{NVTEVT_WIFI_EXE_MOVIE_AUDIOIN, WiFiCmd_OnExeSetMovieAudioIn},
	{NVTEVT_WIFI_EXE_MOVIE_AUDIOIN_SR, WiFiCmd_OnExeSetMovieAudioInSR},
	{NVTEVT_WIFI_EXE_MOVIE_FLIP_MIRROR, WiFiCmd_OnExeSetMovieFlipMirror},
	{NVTEVT_WIFI_EXE_MOVIE_QUALITY_SET, WiFiCmd_OnExeSetMovieQualitySet},

	{NVTEVT_WIFI_EXE_MOTION_DET, WiFiCmd_OnExeSetMotionDet},
	{NVTEVT_WIFI_EXE_MOVIE_AUDIO, WiFiCmd_OnExeSetMovieAudio},
	{NVTEVT_WIFI_EXE_DATEIMPRINT, WiFiCmd_OnExeSetMovieDateImprint},
	{NVTEVT_WIFI_EXE_MOVIE_LIVEVIEW_SIZE, WiFiCmd_OnExeSetMovieLiveviewSize},
	{NVTEVT_WIFI_EXE_MOVIE_GSENSOR_SENS, WiFiCmd_OnExeSetMovieGSesnorSensitivity},
	{NVTEVT_WIFI_EXE_SET_AUTO_RECORDING, WiFiCmd_OnExeSetAutoRecording},
	{NVTEVT_WIFI_EXE_MOVIE_REC_BITRATE, WiFiCmd_OnExeSetMovieRecBitRate},
	{NVTEVT_WIFI_EXE_MOVIE_LIVEVIEW_BITRATE, WiFiCmd_OnExeSetMovieLiveviewBitRate},
	{NVTEVT_WIFI_EXE_MOVIE_LIVEVIEW_START, WiFiCmd_OnExeMovieLiveviewStart},
	{NVTEVT_WIFI_EXE_MOVIE_TRIGGER_RAWENC, WiFiCmd_OnExeTriggerMovieRawEnc},
	{NVTEVT_WIFI_EXE_MOVIE_BRC_ADJUST, WiFiCmd_OnExeMovieBRCAdjust},

	{NVTEVT_WIFI_EXE_MODE, WiFiCmd_OnExeModeChange},
	{NVTEVT_WIFI_EXE_SET_SSID, WiFiCmd_OnExeSetSSID},
	{NVTEVT_WIFI_EXE_SET_PASSPHRASE, WiFiCmd_OnExeSetPassphrase},
	{NVTEVT_WIFI_EXE_SET_DATE, WiFiCmd_OnExeDate},
	{NVTEVT_WIFI_EXE_SET_TIME, WiFiCmd_OnExeTime},
	{NVTEVT_WIFI_EXE_POWEROFF, WiFiCmd_OnExePoweroff},
	{NVTEVT_WIFI_EXE_LANGUAGE, WiFiCmd_OnExeLanguage},
	{NVTEVT_WIFI_EXE_TVFORMAT, WiFiCmd_OnExeTV},
	{NVTEVT_WIFI_EXE_FORMAT, WiFiCmd_OnExeFormat},
	{NVTEVT_WIFI_EXE_SYSRESET, WiFiCmd_OnExeSysReset},
	{NVTEVT_WIFI_EXE_FWUPDATE, WiFiCmd_OnExeFWUpdate},
	{NVTEVT_WIFI_EXE_RECONNECT, WiFiCmd_OnExeReconnect},
	{NVTEVT_WIFI_EXE_SAVEMENU, WiFiCmd_OnExeSaveMenu},
	{NVTEVT_WIFI_EXE_REMOVE_USER, WiFiCmd_OnExeRemoveUser},
	{NVTEVT_WIFI_EXE_PIP_STYLE, WiFiCmd_OnExePipStyle},
	{NVTEVT_WIFI_EXE_SEND_SSID_PASSPHRASE, WiFiCmd_OnExeSendSSIDPassphrase},
	{NVTEVT_WIFI_EXE_SET_WIFI_CONNECT_MODE, WiFiCmd_OnExeSetWiFiConnectMode},
	{NVTEVT_WIFI_EXE_MODE_DONE, WiFiCmd_OnExeChangeModeDone},

//#NT#2016/03/23#Isiah Chang -begin
//#NT#add new Wi-Fi UI flow.
	{NVTEVT_WIFI_EXE_HEARTBEAT, WiFiCmd_OnExeHB},
	{NVTEVT_WIFI_EXE_APP_SESSION_CLOSE, WiFiCmd_OnAppSessionClose},

	{NVTEVT_NULL,                        0},  //End of Command Map
};

//#NT#2016/06/30#JeahYen - begin
//#NT#add WifiCmdObj to "chain of responsibility" of AppObj
CREATE_APP(UIWifiCmdObj, APP_BASE)
//#NT#2016/06/30#JeahYen - end
#endif
