#include "PrjCfg.h"
#include "WifiAppCmd.h"

#if(WIFI_AP_FUNC==ENABLE)
#if defined(_CPU2_ECOS_) || defined(_CPU2_LINUX_)

#include "NVTToolCommand.h"
#include "UIAppWiFiCmd.h"
#include "UIInfo.h"
#include "UsockIpcAPI.h"
#include "nvtmpp.h"
#include "SysCfg.h"
#if (USOCKET_CLIENT==ENABLE)
#include "UsockCliIpcAPI.h"
#endif
#include "WifiAppXML.h"
//#NT#2016/03/23#Isiah Chang -begin
//#NT#add new Wi-Fi UI flow.
#if(WIFI_UI_FLOW_VER == WIFI_UI_VER_2_0)
#include "SysMain.h"
#endif
//#NT#2016/03/23#Isiah Chang -end
//#NT#2016/05/31#Ben Wang -begin
//#NT#Add UVC multimedia function.
#if(UVC_MULTIMEDIA_FUNC == ENABLE)
#include "SysMain.h"
#include "UsbCmdAPI.h"
#endif
//#NT#2016/05/31#Ben Wang -end

#define UDP_SOCKET          0
#define THIS_DBGLVL         2 // 0=FATAL, 1=ERR, 2=WRN, 3=UNIT, 4=FUNC, 5=IND, 6=MSG, 7=VALUE, 8=USER
///////////////////////////////////////////////////////////////////////////////
#define __MODULE__          WifiAppCmd
#define __DBGLVL__          ((THIS_DBGLVL>=PRJ_DBG_LVL)?THIS_DBGLVL:PRJ_DBG_LVL)
#define __DBGFLT__          "*" //*=All, [mark]=CustomClass
#include "DebugModule.h"
///////////////////////////////////////////////////////////////////////////////

extern WIFI_CMD_ENTRY Cmd_app[];

#if (USOCKET_CLIENT==ENABLE)
INT32 gUsockCliHandle = 0;

void UserCliSocket_Notify(int status, int parm)
{
	DBG_IND("status %d %d\r\n", status, parm);
	switch (status) {
	case CYG_USOCKET_UDP_STATUS_CLIENT_REQUEST: {
		}
		break;
	case CYG_USOCKET_STATUS_CLIENT_DISCONNECT: {
			DBG_ERR("UserCliSocket disconnect!!!\r\n");
		}
		break;
	}
}
int UserCliSocket_Recv(char *addr, int size)
{
	if (addr && (size > 0)) {
		addr[size] = '\0';
		DBG_IND("%s %d\r\n", addr, size);
	}
	return TRUE;
}

void UserSocketClient_Open(void)
{
	usocketcli_install_obj  usocketCliObj = {0};

	DBG_IND("open usocket client \r\n");
	usocketCliObj.notify = UserCliSocket_Notify;
	usocketCliObj.recv = UserCliSocket_Recv;
	sprintf(usocketCliObj.svrIP, "%s", "192.168.11.105");
	usocketCliObj.portNum = 8888;
	usocketCliObj.sockbufSize = 40960;
	usocketCliObj.timeout = 10;
	gUsockCliHandle = UsockCliIpc_Connect(&usocketCliObj);
	DBGH(gUsockCliHandle);
	if (gUsockCliHandle) {
		UsockCliIpc_Start();
	}
}

void UserSocketClient_Close(void)
{
	DBGH(gUsockCliHandle);
	UsockCliIpc_Disconnect(&gUsockCliHandle);
	UsockCliIpc_Stop();

}

int UserSocketClient_Send(char *addr, int *size)
{
	return UsockCliIpc_Send(gUsockCliHandle, addr, size);
}
#endif


void UserSocket_Notify(int status, int parm)
{
	//DBG_IND("Notify %d %d\r\n",status,parm);
	switch (status) {
	case CYG_USOCKET_STATUS_CLIENT_CONNECT:
		if (parm) {
			DBG_IND("%d.%d.%d.%d\r\n", (parm & 0xFF), (parm >> 8) & 0xFF, (parm >> 16) & 0xFF, (parm >> 24) & 0xFF);
		}
		break;
	case CYG_USOCKET_STATUS_CLIENT_REQUEST:
		break;
	case CYG_USOCKET_STATUS_CLIENT_DISCONNECT:
		break;
	}
}
#if UDP_SOCKET
void UserUdpSocket_Notify(int status, int parm)
{
	switch (status) {
	case CYG_USOCKET_UDP_STATUS_CLIENT_REQUEST: {
		}
		break;
	}
}
#endif
#define SND_DATA_SIZE   128

UINT32 WifiApp_SendCmd(UINT32 cmd, UINT32 status)
{
	static char bufArry[SND_DATA_SIZE];
	static UINT32 len = 0;
	UINT32 result = E_OK;
	char *buf = bufArry;
	UINT32 sendlen = 0;
	UINT32 bufSize = SND_DATA_SIZE;
	memset(bufArry, 0, sizeof(bufArry));

	len = XML_snprintf(&buf, &bufSize, DEF_XML_HEAD);
	len = XML_snprintf(&buf, &bufSize, DEF_XML_RET, cmd, status);

	len = buf - bufArry;

	//DBG_DUMP("%d\r\n",len);
	//DBG_DUMP("%s\r\n",bufArry);

	if (len < SND_DATA_SIZE) {
		sendlen = len;
		DBG_IND(" %s %d\r\n", bufArry, sendlen);
//#NT#2016/05/31#Ben Wang -begin
//#NT#Add UVC multimedia function.
#if(UVC_MULTIMEDIA_FUNC == ENABLE)
		if (System_GetState(SYS_STATE_CURRSUBMODE) == SYS_SUBMODE_UVC) {
			result = UsbCmd_Send(bufArry, (int *)&len);
		} else
#endif
		{
			result = UserSocket_Send(bufArry, (int *)&len);
		}
//#NT#2016/05/31#Ben Wang -end
		if (sendlen != len) {
			result = E_SYS;
			DBG_ERR("sent %d error,should %d\r\n", len, sendlen);
		}
	} else {
		DBG_ERR("len %d err\r\n", len);
		result = E_SYS;
	}

	return result;

}

UINT32 WifiApp_SendAutoTestCmd(UINT32 par1, UINT32 par2)
{
	static char bufArry[SND_DATA_SIZE];
	static UINT32 len = 0;
	UINT32 result = E_OK;
	char *buf = bufArry;
	UINT32 sendlen = 0;
	UINT32 bufSize = SND_DATA_SIZE;
	memset(bufArry, 0, sizeof(bufArry));

	len = XML_snprintf(&buf, &bufSize, DEF_XML_HEAD);
	len = XML_snprintf(&buf, &bufSize, "<Function>\n<Cmd>AutoTest</Cmd>\n<Par1>%d</Par1>\n<Par2>%d</Par2>\n</Function>", par1, par2);

	len = buf - bufArry;

	//DBG_DUMP("%d\r\n",len);
	//DBG_DUMP("%s\r\n",bufArry);

	if (len < SND_DATA_SIZE) {
		if (len & 0xF) {
			//DBG_DUMP("%d\r\n",len);
			len = (len + 0xF) & 0xFFFFFFF0 ;
			//DBG_DUMP("%d\r\n",len);
		}

		sendlen = len;
		DBG_IND(" %s %d\r\n", bufArry, sendlen);
//#NT#2016/05/31#Ben Wang -begin
//#NT#Add UVC multimedia function.
#if(UVC_MULTIMEDIA_FUNC == ENABLE)
		if (System_GetState(SYS_STATE_CURRSUBMODE) == SYS_SUBMODE_UVC) {
			result = UsbCmd_Send(bufArry, (int *)&len);
		} else
#endif
		{
			result = UserSocket_Send(bufArry, (int *)&len);
		}
//#NT#2016/05/31#Ben Wang -end
		if (sendlen != len) {
			result = E_SYS;
			DBG_ERR("sent %d error,should %d\r\n", len, sendlen);
		}
	} else {
		DBG_ERR("len %d err\r\n", len);
		result = E_SYS;
	}

	return result;

}

INT32 WifiApp_Recv(char *addr, INT32 size)
{
	if (addr && (size > 0)) {
		DBG_IND("%s %d\r\n\r\n", addr, size);
	}
	return TRUE;
}

//#NT#2016/03/23#Isiah Chang -begin
//#NT#add new Wi-Fi UI flow.
INT32 WifiApp_CheckIfWifiMode(void)
{
#if(WIFI_UI_FLOW_VER == WIFI_UI_VER_2_0)
	if (System_GetState(SYS_STATE_CURRSUBMODE) == SYS_SUBMODE_WIFI) {
		return TRUE;
	}

	return FALSE;
#elif(WIFI_UI_FLOW_VER == WIFI_UI_VER_1_0)
	return TRUE;
#endif
}
//#NT#2016/03/23#Isiah Chang -end

void UserSocket_Open(void)
{

	usocket_install_obj  usocketObj = {0};

	DBG_IND("open usocket\r\n");
	usocketObj.notify = UserSocket_Notify;
	usocketObj.recv = UserSocket_Recv;
	usocketObj.portNum = 3333;
	usocketObj.threadPriority = 6;
	usocketObj.sockbufSize = 40960;
	UsockIpc_Init(&usocketObj);

#if UDP_SOCKET
	usocket_install_obj  usocketUdpObj = {0};
	DBG_IND("open udp usocket\r\n");
	usocketUdpObj.notify = UserUdpSocket_Notify;
	usocketUdpObj.recv = UserUdpSocket_Recv;
	usocketUdpObj.portNum = 2222;
	usocketUdpObj.threadPriority = 8;
	usocketUdpObj.sockbufSize = 0;
	UsockIpc_Udp_Init(&usocketUdpObj);
#endif
}

void UserSocket_Close(void)
{
	UsockIpc_Uninit();
#if UDP_SOCKET
	UsockIpc_Udp_Uninit();
#endif
}

int UserSocket_Send(char *addr, int *size)
{
	return UsockIpc_Send(addr, size);
}

int UserSocket_Recv(char *addr, int size)
{
	if (addr && (size > 0)) {
		addr[size] = '\0';
		DBG_IND("%s %d\r\n", addr, size);
//#NT#2016/03/23#Isiah Chang -begin
//#NT#add new Wi-Fi UI flow.
#if(WIFI_UI_FLOW_VER == WIFI_UI_VER_2_0)
		Ux_PostEvent(NVTEVT_WIFI_EXE_HEARTBEAT, 0);
#endif
//#NT#2016/03/23#Isiah Chang -end
	}
	return TRUE;
}
#if UDP_SOCKET
int UserUdpSocket_Recv(char *addr, int size)
{
	if (addr && (size > 0)) {
		addr[size] = '\0';
		DBG_IND("%s %d\r\n", addr, size);
	}
	return TRUE;
}
#endif
static UINT32 XML_InitBuf(void)
{
    #if 0  //avoid memory fragment,always use fix poll,
	//xmlPool = AppMem_Alloc("XML", POOL_SIZE_TEMP);
	//XML_SetTempMem(xmlPool.Addr,xmlPool.Size);
	NVTMPP_VB_POOL pool;
	UINT32         blk_size = POOL_SIZE_TEMP;
	pool = nvtmpp_vb_create_pool("XML", blk_size, 1, NVTMPP_DDR_1);
	if (NVTMPP_VB_INVALID_POOL == pool)
	{
		DBG_ERR("create private pool err\r\n");
        return FALSE;
	}
	blk_xml = nvtmpp_vb_get_block(0, pool, blk_size, NVTMPP_DDR_1);
	if (NVTMPP_VB_INVALID_BLK == blk_xml)
	{
		DBG_ERR("get vb block err\r\n");
        return FALSE;

	}
	XML_SetTempMem(nvtmpp_vb_block2addr(blk_xml), blk_size);
    return TRUE;
    #else

	XML_SetTempMem(OS_GetMempoolAddr(POOL_ID_TEMP),OS_GetMempoolSize(POOL_ID_TEMP));
    return TRUE;
    #endif
}

static UINT32 XML_UnInitBuf(void)
{
    #if 0  //avoid memory fragment,always use fix poll,

	NVTMPP_VB_POOL pool;

    pool = nvtmpp_vb_addr2pool(nvtmpp_vb_block2addr(blk_xml));
	if (pool == NVTMPP_VB_INVALID_POOL){
		DBG_ERR("invalid block 0x%08X\r\n",blk_xml);
        return FALSE;
	} else {
		return nvtmpp_vb_destroy_pool(pool);
	}
    #else
    return TRUE;
    #endif
}

void WifiAppCmd_init(void)
{
	WifiCmd_SetExecTable(Cmd_app);
	WifiCmd_SetDefautCB((UINT32)XML_DefaultFormat);
	WifiCmd_SetEventHandle((UINT32)Ux_PostEvent);
//#NT#2016/03/23#Isiah Chang -begin
//#NT#add new Wi-Fi UI flow.
	WifiCmd_SetAppStartupChecker((UINT32)WifiApp_CheckIfWifiMode);
	WifiCmd_SetAppStartupCmdCode(WIFIAPP_CMD_APP_STARTUP);
//#NT#2016/03/23#Isiah Chang -end
    XML_InitBuf();

}
void WifiAppCmd_UnInit(void)
{
    XML_UnInitBuf();
}

WIFI_CMD_BEGIN(app)
//#NT#2016/03/04#YongChang Qui -begin
//#NT#Add wifi app command support for linux cardv wifi.
#if (defined(_CPU2_ECOS_) || (defined(_CPU2_LINUX_) && WIFI_AP_FUNC==ENABLE))
//#NT#2016/03/04#YongChang Qui -end
WIFI_CMD_ITEM(WIFIAPP_CMD_CAPTURE, NVTEVT_WIFI_EXE_PHOTO_CAPTURE, (UINT32)XML_GetPictureEnd, WIFIFLAG_CAPTURE_DONE, FL_NULL)
WIFI_CMD_ITEM(WIFIAPP_CMD_CAPTURESIZE, NVTEVT_WIFI_EXE_PHOTO_SIZE, 0, 0, FL_PHOTO_SIZE)
WIFI_CMD_ITEM(WIFIAPP_CMD_FREE_PIC_NUM, 0, (UINT32)XML_GetFreePictureNum, 0, FL_NULL)
WIFI_CMD_ITEM(WIFIAPP_CMD_MOVIE_RECORDING_TIME, 0, (UINT32)XML_GetMovieRecStatus, 0, FL_MOVIE_REC)
WIFI_CMD_ITEM(WIFIAPP_CMD_MOVIE_REC_TRIGGER_RAWENC, NVTEVT_WIFI_EXE_MOVIE_TRIGGER_RAWENC, 0, WIFIFLAG_MOVIE_REC_RAWENC_DONE, FL_NULL)
WIFI_CMD_ITEM(WIFIAPP_CMD_MOVIE_GET_RAWENC_JPG, 0, (UINT32)XML_GetRawEncJpg, 0, FL_NULL)
WIFI_CMD_ITEM(WIFIAPP_CMD_MOVIE_GET_LIVEVIEW_FMT, 0, (UINT32)XML_GetLiveViewFmt, 0, FL_NULL)
//#NT#2016/12/02#Isiah Chang -begin
//#NT#Add WiFiCmd Bitrate control.
WIFI_CMD_ITEM(WIFIAPP_CMD_MOVIE_BRC_ADJUST, NVTEVT_WIFI_EXE_MOVIE_BRC_ADJUST, 0, 0, FL_NULL)
//#NT#2016/12/02#Isiah Chang -bend

WIFI_CMD_ITEM(WIFIAPP_CMD_RECORD, NVTEVT_WIFI_EXE_MOVIE_REC, 0, WIFIFLAG_RECORD_DONE, FL_MOVIE_REC)
WIFI_CMD_ITEM(WIFIAPP_CMD_MOVIE_REC_SIZE, NVTEVT_WIFI_EXE_MOVIE_REC_SIZE, 0, 0, FL_MOVIE_SIZE)
WIFI_CMD_ITEM(WIFIAPP_CMD_CYCLIC_REC, NVTEVT_WIFI_EXE_CYCLIC_REC, 0, 0, FL_MOVIE_CYCLIC_REC)
WIFI_CMD_ITEM(WIFIAPP_CMD_MOVIE_WDR, NVTEVT_WIFI_EXE_MOVIE_WDR, 0, 0, FL_MOVIE_WDR)
WIFI_CMD_ITEM(WIFIAPP_CMD_MOVIE_EV, NVTEVT_WIFI_EXE_MOVIE_EV, 0, 0, FL_EV)
//#NT#2016/06/03#Charlie Chang -begin
//#NT# support contrast, twoway audio, flip_mirror, movie quality set
WIFI_CMD_ITEM(WIFIAPP_CMD_MOVIE_CONTRAST, NVTEVT_WIFI_EXE_MOVIE_CONTRAST, 0, 0, FL_MOVIE_CONTRAST)
WIFI_CMD_ITEM(WIFIAPP_CMD_TWOWAY_AUDIO, NVTEVT_WIFI_EXE_MOVIE_AUDIOIN, 0, 0, FL_MOVIE_AUDIOIN)
WIFI_CMD_ITEM(WIFIAPP_CMD_TWOWAY_AUDIO_SAMPLERATE, NVTEVT_WIFI_EXE_MOVIE_AUDIOIN_SR, 0, 0, FL_MOVIE_AUDIOIN_SR)
WIFI_CMD_ITEM(WIFIAPP_CMD_FLIP_MIRROR, NVTEVT_WIFI_EXE_MOVIE_FLIP_MIRROR, 0, 0, FL_MOVIE_FLIP_MIRROR)
WIFI_CMD_ITEM(WIFIAPP_CMD_QUALITYSET, NVTEVT_WIFI_EXE_MOVIE_QUALITY_SET, 0, 0, FL_MOVIE_QUALITY_SET)
//#NT#2016/06/03#Charlie Chang -end
WIFI_CMD_ITEM(WIFIAPP_CMD_MOTION_DET, NVTEVT_WIFI_EXE_MOTION_DET, 0, 0, FL_MOVIE_MOTION_DET)
WIFI_CMD_ITEM(WIFIAPP_CMD_MOVIE_AUDIO, NVTEVT_WIFI_EXE_MOVIE_AUDIO, 0, 0, FL_MOVIE_AUDIO)
WIFI_CMD_ITEM(WIFIAPP_CMD_DATEIMPRINT, NVTEVT_WIFI_EXE_DATEIMPRINT, 0, 0, FL_MOVIE_DATEIMPRINT)
WIFI_CMD_ITEM(WIFIAPP_CMD_MAX_RECORD_TIME, 0, (UINT32)XML_GetMaxRecordTime, 0, FL_NULL)
#if (WIFI_FINALCAM_APP_STYLE == ENABLE)
WIFI_CMD_ITEM(WIFIAPP_CMD_MOVIE_LIVEVIEW_SIZE, NVTEVT_WIFI_EXE_MOVIE_LIVEVIEW_SIZE, 0, 0, FL_WIFI_MOVIE_APP_PREVIEW_SIZE)
#endif
WIFI_CMD_ITEM(WIFIAPP_CMD_MOVIE_GSENSOR_SENS, NVTEVT_WIFI_EXE_MOVIE_GSENSOR_SENS, 0, 0, FL_GSENSOR)
WIFI_CMD_ITEM(WIFIAPP_CMD_SET_AUTO_RECORDING, NVTEVT_WIFI_EXE_SET_AUTO_RECORDING, 0, 0, FL_WIFI_AUTO_RECORDING)
WIFI_CMD_ITEM(WIFIAPP_CMD_MOVIE_REC_BITRATE, NVTEVT_WIFI_EXE_MOVIE_REC_BITRATE, 0, 0, FL_NULL)
WIFI_CMD_ITEM(WIFIAPP_CMD_MOVIE_LIVEVIEW_BITRATE, NVTEVT_WIFI_EXE_MOVIE_LIVEVIEW_BITRATE, 0, 0, FL_NULL)
WIFI_CMD_ITEM(WIFIAPP_CMD_MOVIE_LIVEVIEW_START, NVTEVT_WIFI_EXE_MOVIE_LIVEVIEW_START, 0, WIFIFLAG_PREVIEW_DONE, FL_NULL)

WIFI_CMD_ITEM(WIFIAPP_CMD_MODECHANGE, NVTEVT_WIFI_EXE_MODE, 0, WIFIFLAG_MODE_DONE, FL_NULL)
WIFI_CMD_ITEM(WIFIAPP_CMD_QUERY, 0, (UINT32)XML_QueryCmd, 0, FL_NULL)
WIFI_CMD_ITEM(WIFIAPP_CMD_SET_SSID, NVTEVT_WIFI_EXE_SET_SSID, 0, 0, FL_NULL)
WIFI_CMD_ITEM(WIFIAPP_CMD_SET_PASSPHRASE, NVTEVT_WIFI_EXE_SET_PASSPHRASE, 0, 0, FL_NULL)
WIFI_CMD_ITEM(WIFIAPP_CMD_SET_DATE, NVTEVT_WIFI_EXE_SET_DATE, 0, 0, FL_NULL)
WIFI_CMD_ITEM(WIFIAPP_CMD_SET_TIME, NVTEVT_WIFI_EXE_SET_TIME, 0, 0, FL_NULL)
WIFI_CMD_ITEM(WIFIAPP_CMD_POWEROFF, NVTEVT_WIFI_EXE_POWEROFF, 0, 0, FL_AUTO_POWER_OFF)
WIFI_CMD_ITEM(WIFIAPP_CMD_LANGUAGE, NVTEVT_WIFI_EXE_LANGUAGE, 0, 0, FL_LANGUAGE)
WIFI_CMD_ITEM(WIFIAPP_CMD_TVFORMAT, NVTEVT_WIFI_EXE_TVFORMAT, 0, 0, FL_TV_MODE)
WIFI_CMD_ITEM(WIFIAPP_CMD_FORMAT, NVTEVT_WIFI_EXE_FORMAT, 0, WIFIFLAG_FORMAT_DONE, FL_NULL)
WIFI_CMD_ITEM(WIFIAPP_CMD_SYSRESET, NVTEVT_WIFI_EXE_SYSRESET, 0, 0, FL_NULL)
WIFI_CMD_ITEM(WIFIAPP_CMD_VERSION, 0, (UINT32)XML_GetVersion, 0, FL_NULL)
WIFI_CMD_ITEM(WIFIAPP_CMD_FWUPDATE, NVTEVT_WIFI_EXE_FWUPDATE, 0, WIFIFLAG_UPDATE_DONE, FL_NULL)
WIFI_CMD_ITEM(WIFIAPP_CMD_QUERY_CUR_STATUS, 0, (UINT32)XML_QueryCmd_CurSts, 0, FL_NULL)
WIFI_CMD_ITEM(WIFIAPP_CMD_FILELIST, 0, (UINT32)XML_FileList, 0, FL_NULL)
WIFI_CMD_ITEM(WIFIAPP_CMD_HEARTBEAT, 0, (UINT32)XML_GetHeartBeat, 0, FL_NULL)
WIFI_CMD_ITEM(WIFIAPP_CMD_DISK_FREE_SPACE, 0, (UINT32)XML_GetDiskFreeSpace, 0, FL_NULL)
WIFI_CMD_ITEM(WIFIAPP_CMD_RECONNECT_WIFI, NVTEVT_WIFI_EXE_RECONNECT, 0, 0, FL_NULL)
WIFI_CMD_ITEM(WIFIAPP_CMD_GET_BATTERY, 0, (UINT32)XML_GetBattery, 0, FL_NULL)
WIFI_CMD_ITEM(WIFIAPP_CMD_SAVE_MENUINFO, NVTEVT_WIFI_EXE_SAVEMENU, 0, 0, FL_NULL)
WIFI_CMD_ITEM(WIFIAPP_CMD_GET_HW_CAP, 0, (UINT32)XML_HWCapability, 0, FL_NULL)
WIFI_CMD_ITEM(WIFIAPP_CMD_REMOVE_USER, NVTEVT_WIFI_EXE_REMOVE_USER, 0, 0, FL_NULL)
WIFI_CMD_ITEM(WIFIAPP_CMD_GET_CARD_STATUS, 0, (UINT32)XML_GetCardStatus, 0, FL_NULL)
WIFI_CMD_ITEM(WIFIAPP_CMD_GET_DOWNLOAD_URL, 0, (UINT32)XML_GetDownloadURL, 0, FL_NULL)
WIFI_CMD_ITEM(WIFIAPP_CMD_GET_UPDATEFW_PATH, 0, (UINT32)XML_GetUpdateFWPath, 0, FL_NULL)
WIFI_CMD_ITEM(WIFIAPP_CMD_SET_PIP_STYLE, NVTEVT_WIFI_EXE_PIP_STYLE, 0, 0, FL_DUAL_CAM)
WIFI_CMD_ITEM(WIFIAPP_CMD_GET_SSID_PASSPHRASE, 0, (UINT32)XML_GetSSID_passphrase, 0, FL_NULL)
WIFI_CMD_ITEM(WIFIAPP_CMD_QUERY_MOVIE_SIZE, 0, (UINT32)XML_GetMovieSizeCapability, 0, FL_NULL)
WIFI_CMD_ITEM(WIFIAPP_CMD_QUERY_MENUITEM, 0, (UINT32)XML_GetMenuItem, 0, FL_NULL)
WIFI_CMD_ITEM(WIFIAPP_CMD_SEND_SSID_PASSPHRASE, NVTEVT_WIFI_EXE_SEND_SSID_PASSPHRASE, 0, 0, FL_NULL)
WIFI_CMD_ITEM(WIFIAPP_CMD_SET_WIFI_CONNECT_MODE, NVTEVT_WIFI_EXE_SET_WIFI_CONNECT_MODE, 0, 0, FL_NetWorkMode)
WIFI_CMD_ITEM(WIFIAPP_CMD_AUTO_TEST_CMD_DONE, 0, (UINT32)XML_AutoTestCmdDone, 0, FL_NULL)
//#NT#2016/03/23#Isiah Chang -begin
//#NT#add new Wi-Fi UI flow.
#if(WIFI_UI_FLOW_VER == WIFI_UI_VER_2_0)
WIFI_CMD_ITEM(WIFIAPP_CMD_APP_STARTUP, 0, (UINT32)XML_APP_STARTUP, 0, FL_NULL)
WIFI_CMD_ITEM(WIFIAPP_CMD_APP_SESSION_CLOSE, NVTEVT_WIFI_EXE_APP_SESSION_CLOSE, 0, 0, FL_NULL)
#endif
//#NT#2016/03/23#Isiah Chang -end

WIFI_CMD_ITEM(WIFIAPP_CMD_THUMB, 0, (UINT32)XML_GetThumbnail, 0, FL_NULL)
WIFI_CMD_ITEM(WIFIAPP_CMD_SCREEN, 0, (UINT32)XML_GetThumbnail, 0, FL_NULL)
WIFI_CMD_ITEM(WIFIAPP_CMD_MOVIE_FILE_INFO, 0, (UINT32)XML_GetMovieFileInfo, 0, FL_NULL)
WIFI_CMD_ITEM(WIFIAPP_CMD_DELETE_ONE, 0, (UINT32)XML_DeleteOnePicture, 0, FL_NULL)
WIFI_CMD_ITEM(WIFIAPP_CMD_DELETE_ALL, 0, (UINT32)XML_DeleteAllPicture, 0, FL_NULL)

WIFI_CMD_ITEM(WIFIAPP_CMD_UPLOAD, 0, (UINT32)XML_UploadFile, 0, FL_NULL)
//#NT#2016/06/01#Isiah Chang -begin
//#NT#add a Wi-Fi command to get current mode status.
WIFI_CMD_ITEM(WIFIAPP_CMD_GET_MODE_STAUTS, 0, (UINT32)XML_GetCntModeStatus, 0, FL_NULL)
//#NT#2016/06/01#Isiah Chang -end
//#NT#2016/06/06#Charlie Chang -begin
//#NT# support wifi AP search
WIFI_CMD_ITEM(WIFIAPP_CMD_WIFIAP_SEARCH, 0, (UINT32)XML_GetWifiAP, 0, FL_NULL)
//#NT#2016/06/06#Charlie Chang -end


#endif
#if defined(_CPU2_LINUX_)
WIFI_CMD_ITEM(WIFIAPP_CMD_UPLOAD_AUDIO, 0, (UINT32)XML_UploadAudio, 0, FL_NULL)
#endif
WIFI_CMD_END()

#else
UINT32 WifiApp_SendCmd(UINT32 cmd, UINT32 status)
{
	return 0;
}
int UserSocket_Send(char *addr, int *size)
{
	return 0;
}

#endif
#endif

