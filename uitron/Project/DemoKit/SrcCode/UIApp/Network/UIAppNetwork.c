#include "PrjCfg.h"
#include "UIInfo.h"
#include "SysCfg.h"
#include "HfsNvtAPI.h"
#include "FsIpcAPI.h"
#include "timer.h"
#if(WIFI_AP_FUNC==ENABLE)
#include "WifiAppCmd.h"
#include "WifiCmdParser.h"
#include "WifiAppXML.h"
#endif
BOOL UINet_FsIpc_Init(void)
{
#if defined(_CPU2_ECOS_)
	FSIPC_OPEN_OBJ OpenObj;

	OpenObj.uiMemPoolAddr = OS_GetMempoolAddr(POOL_ID_FSIPC);
	OpenObj.uiMemPoolSize = OS_GetMempoolSize(POOL_ID_FSIPC);

	if (E_OK != FsIpc_Open(&OpenObj)) {
		DBG_ERR("FsIpc_Open fail\r\n");
		return FALSE;
	}

	if (E_OK != FsIpc_Mount("/sdcard")) {
		DBG_ERR("FsIpc_Mount fail\r\n");
		return FALSE;
	}
#endif

	return TRUE;
}
BOOL UINet_FsIpc_Exit(void)
{
#if defined(_CPU2_ECOS_)
	if (E_OK != FsIpc_Umount()) {
		DBG_ERR("FsIpc_Umount fail\r\n");
		return FALSE;
	}

	if (E_OK != FsIpc_Close()) {
		DBG_ERR("FsIpc_Close fail\r\n");
		return FALSE;
	}
#endif

	return TRUE;
}

#if (IPCAM_FUNC == ENABLE)
#define PORT_NUM             80
#define HTTPS_PORT_NUM       443
#define MOUNT_FS_ROOT        "/var/www"
#define HFS_MAX_CLIENT       20
#define HFS_TIMEOUT_CNT      40
#elif defined(_CPU2_LINUX_)
#define PORT_NUM             80
#define HTTPS_PORT_NUM       443
#define MOUNT_FS_ROOT        "/mnt/sd"
#define HFS_MAX_CLIENT       10
#define HFS_TIMEOUT_CNT      40
#else
#define PORT_NUM             80
//#NT#2016/04/21#Lincy Lin -begin
//#NT#Support curl (http client)
#define HTTPS_PORT_NUM       443
//#NT#2016/04/21#Lincy Lin -end
#define MOUNT_FS_ROOT        "/sdcard"
#define HFS_MAX_CLIENT       10
#define HFS_TIMEOUT_CNT      40
#endif

//#NT#2016/05/18#David Tsai -begin
//#NT# Support http service disable in booting time
BOOL  gHFS_enable = TRUE;
//#NT#2016/05/18#David Tsai -end
INT32 gHFS_http_port = PORT_NUM;
INT32 gHFS_https_port = HTTPS_PORT_NUM;

HFS_U32 xExamHfs_getCurrTime(void)
{
	return timer_getCurrentCount(timer_getSysTimerID());
}

#if (HFS_FUNC == ENABLE)
static void UINet_HFSNotifyStatus(int status)
{
	switch (status) {
	// HTTP client has request coming in
	case CYG_HFS_STATUS_CLIENT_REQUEST:
		//DBG_IND("client request %05d ms\r\n",xExamHfs_getCurrTime()/1000);
		break;
	// HTTP server send data start
	case CYG_HFS_STATUS_SERVER_RESPONSE_START:
		//DBG_IND("send data start, time= %05d ms\r\n",xExamHfs_getCurrTime()/1000);
		break;
	// HTTP server send data start
	case CYG_HFS_STATUS_SERVER_RESPONSE_END:
		//DBG_IND("send data end, time= %05d ms\r\n",xExamHfs_getCurrTime()/1000);
		break;
	// HTTP client disconnect
	case CYG_HFS_STATUS_CLIENT_DISCONNECT:
		//DBG_IND("client disconnect, time= %05d ms\r\n",xExamHfs_getCurrTime()/1000);
		break;

	}
}
#endif

INT32 UINet_HFSUploadResultCb(int result, UINT32 bufAddr, UINT32 *bufSize, CHAR *mimeType)
{
#if _TODO
	XML_DefaultFormat(WIFIAPP_CMD_UPLOAD_FILE, result, bufAddr, (HFS_U32 *)bufSize, (char *)mimeType);
#endif
	return CYG_HFS_CB_GETDATA_RETURN_OK;
}

INT32 UINet_HFSDeleteResultCb(int result, UINT32 bufAddr, UINT32 *bufSize, CHAR *mimeType)
{
#if _TODO
	XML_DefaultFormat(WIFIAPP_CMD_DELETE_ONE, result, bufAddr, (HFS_U32 *)bufSize, (char *)mimeType);
#endif
	return CYG_HFS_CB_GETDATA_RETURN_OK;
}


INT32 UINet_HFSInit(void)
{
	int result = 0 ;
	UINet_FsIpc_Init();

#if (HFS_FUNC == ENABLE)
	if (gHFS_enable == TRUE) {
		HFSNVT_OPEN  hfsObj = {0};
		// register callback function of get XML data.
#if(WIFI_AP_FUNC==ENABLE)
		hfsObj.getCustomData = WifiCmd_GetData;
#endif
		// noify some status of HTTP server
		hfsObj.serverEvent = UINet_HFSNotifyStatus;
		// if need to check user & password
		hfsObj.check_pwd = NULL;//xExamHfs_CheckPasswd;
		// set port number
		hfsObj.portNum = gHFS_http_port;
		// https port number
		hfsObj.httpsPortNum = gHFS_https_port;
		// set thread priority
		hfsObj.threadPriority = 6;//20;
		// set socket buffer
		hfsObj.sockbufSize = 102400;//100K speed up get file speed
		// set query callback
#if(WIFI_AP_FUNC==ENABLE)
		hfsObj.clientQuery = WifiCmd_getQueryData;
#endif
		hfsObj.uploadResultCb = UINet_HFSUploadResultCb;
		hfsObj.deleteResultCb = UINet_HFSDeleteResultCb;
		// set query string
		strncpy(hfsObj.clientQueryStr, "query_devinfo", HFS_USER_QUERY_MAXLEN);
		// set HFS root dir path
		strncpy(hfsObj.rootdir, MOUNT_FS_ROOT, HFS_ROOT_DIR_MAXLEN);
		// max client number
		hfsObj.maxClientNum = HFS_MAX_CLIENT;
		// set share memory
		hfsObj.sharedMemAddr = OS_GetMempoolAddr(POOL_ID_HFSIPC);
		hfsObj.sharedMemSize = OS_GetMempoolSize(POOL_ID_HFSIPC);
		// register put data callback function.
#if(WIFI_AP_FUNC==ENABLE)
		hfsObj.putCustomData = (HFSNVT_PUT_CUSTOM_CB *)WifiCmd_PutData;
#endif
		hfsObj.timeoutCnt = HFS_TIMEOUT_CNT;
		// start HFS
		HfsNvt_Open(&hfsObj);

	}
#endif

	//#NT#2016/04/21#Lincy Lin -begin
	//#NT#Support curl (http client)
#if (CURL_FUNC == ENABLE)
	// start curl
	{
		CURLNVT_OPEN  curl_openInfo = {0};

		curl_openInfo.workbuf.Addr = OS_GetMempoolAddr(POOL_ID_CURLNVT);
		curl_openInfo.workbuf.Size = OS_GetMempoolSize(POOL_ID_CURLNVT);
		CurlNvt_Open(&curl_openInfo);
	}
#endif
	//#NT#2016/04/21#Lincy Lin -end


#if (defined(_CPU2_LINUX_)&&(WIFI_AP_FUNC==ENABLE))
	if (gHFS_enable == TRUE) {
		WifiAppCmd_init();
		WifiCmd_ReceiveCmd(TRUE);
	}
#if(WIFI_AP_FUNC==ENABLE)
	//#NT#2016/06/13#Charlie Chang -begin
	//#NT# enable user socket with linux
	UserSocket_Open();
	//#NT#2016/06/13#Charlie Chang -end
#endif
#endif
	return result;
}

void UINet_HFSPortSet(INT32 http_port, INT32 https_port)
{

	gHFS_https_port = https_port;
	gHFS_http_port = http_port;
	return;
}

void UINet_DisableHFS(BOOL enable)
{
	gHFS_enable = enable;
	return;
}
INT32 UINet_HFSUnInit(void)
{
#if (defined(_CPU2_LINUX_)&&(WIFI_AP_FUNC==ENABLE))
	if (gHFS_enable == TRUE) {
		WifiCmd_ReceiveCmd(FALSE);
        WifiAppCmd_UnInit();
	}
#endif

	//#NT#2016/04/21#Lincy Lin -begin
	//#NT#Support curl (http client)
#if (CURL_FUNC == ENABLE)
	CurlNvt_Close();
#endif
	//#NT#2016/04/21#Lincy Lin -end
#if (HFS_FUNC == ENABLE)
	if (gHFS_enable == TRUE) {
		HfsNvt_Close();
	}
#endif
	UINet_FsIpc_Exit();
	return E_OK;
}

#if (IPCAM_FUNC != ENABLE)
#include "UIInfo.h"
#include "SysCfg.h"
#include "HfsNvtAPI.h"
#include "FsIpcAPI.h"

#include "DhcpNvt.h"
#include "NvtIpcAPI.h"
#include "WiFiIpcAPI.h"
#include "UsockIpcAPI.h"
#if (USOCKET_CLIENT==ENABLE)
#include "UsockCliIpcAPI.h"
#endif
#include "SxCmd.h"
#include "UIAppNetwork.h"
#include "WifiAppCmd.h"
#include "WifiAppXML.h"
#include "timer.h"
#include "NVTToolCommand.h"
#include "UCtrlAppOnvif.h"
#include "UIBackgroundObj.h"
#include "AppCommon.h"
#include "DxWiFi.h"
#include "SysCommon.h"
//global debug level: PRJ_DBG_LVL
#include "PrjCfg.h"
#include "SysMain.h"
//#NT#2016/04/21#Lincy Lin -begin
//#NT#Support curl (http client)
#if (CURL_FUNC == ENABLE)
#include "CurlNvtAPI.h"
#endif
//#NT#2016/04/21#Lincy Lin -end
#if (IOT_P2P_FUNC == ENABLE)
#include "P2pIpcAPI.h"
#endif
//#NT#2016/08/05#Niven Cho -begin
//#NT#MSDC-NET
#include "MsdcNvtCb.h"
//#NT#2016/08/05#Niven Cho -end
//local debug level: THIS_DBGLVL
#define THIS_DBGLVL         6 // 0=FATAL, 1=ERR, 2=WRN, 3=UNIT, 4=FUNC, 5=IND, 6=MSG, 7=VALUE, 8=USER
///////////////////////////////////////////////////////////////////////////////
#define __MODULE__          UINet
#define __DBGLVL__          ((THIS_DBGLVL>=PRJ_DBG_LVL)?THIS_DBGLVL:PRJ_DBG_LVL)
#define __DBGFLT__          "*" //*=All, [mark]=CustomClass
#include "DebugModule.h"
///////////////////////////////////////////////////////////////////////////////

char gNvtDhcpHostNameCli[DHCP_HOST_NAME_MAX_LEN] = "nvtsys01cli";
char gNvtDhcpHostNameSrv[DHCP_HOST_NAME_MAX_LEN] = "nvtsys01srv";
char gMacAddr[6] = {0, 0, 0, 0x81, 0x89, 0xe5};
UINT32 gAuthType = NET_AUTH_WPA2;
UINT32 gMode = NET_AP_MODE;
char gSSID[NVT_WSC_MAX_SSID_LEN] = "NVT_CARDV";
char gSSID_AP_default[NVT_WSC_MAX_SSID_LEN] = "NVT_CARDV";
char gPASSPHRASE[NVT_MAX_WEP_KEY_LEN] = "12345678";
char gPASSPHRASE_AP_default[NVT_MAX_WEP_KEY_LEN] = "12345678";
char gCurrIP[NVT_WIFIIPC_IP_MAX_LEN] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
UINT32 gChannel = 0;//default 0:auto
char gLastMacAddr[NVT_WIFIIPC_IP_MAX_LEN] = {0};
char gRemoveMacAddr[NVT_WIFIIPC_IP_MAX_LEN] = {0};
char currentMacAddr[NVT_WIFIIPC_IP_MAX_LEN] = {0};
UINT32 gSendPeerIP = 0;
DHCP_ASSIGN_IP_INFO CurConnectedIpInfo = {0}; //for DHCP lease is 1,keep Mac of IP
#define EXAM_NET_IP_ETH0            "192.168.0.3"
#define EXAM_NET_NETMASK_ETH0       "255.255.255.0"
#define EXAM_NET_BRAODCAST_ETH0     "192.168.0.255"
#define EXAM_NET_GATEWAY_ETH0       "192.168.0.1"
#define EXAM_NET_SRVIP_ETH0         "192.168.0.1"
#define EXAM_NET_LEASE_START_ETH0   "192.168.0.12"
#define EXAM_NET_LEASE_END_ETH0     "192.168.0.32"
#define EXAM_NET_LEASE_DNS_ETH0     "0.0.0.0"
#define EXAM_NET_LEASE_WINS_ETH0    "0.0.0.0"

#define EXAM_NET_IP_ETH1            "192.168.1.3"
#define EXAM_NET_NETMASK_ETH1       "255.255.255.0"
#define EXAM_NET_BRAODCAST_ETH1     "192.168.1.255"
#define EXAM_NET_GATEWAY_ETH1       "192.168.1.1"
#define EXAM_NET_SRVIP_ETH1         "192.168.1.1"

#define EXAM_NET_IP_WLAN0            "192.168.1.254"
#define EXAM_NET_NETMASK_WLAN0       "255.255.255.0"
#define EXAM_NET_BRAODCAST_WLAN0     "192.168.1.255"
#define EXAM_NET_GATEWAY_WLAN0       "192.168.1.254"
#define EXAM_NET_SRVIP_WLAN0         "192.168.1.254"
#define EXAM_NET_LEASE_START_WLAN0   "192.168.1.33"
#define EXAM_NET_LEASE_END_WLAN0     "192.168.1.52"
#define EXAM_NET_LEASE_DNS_WLAN0     "0.0.0.0"
#define EXAM_NET_LEASE_WINS_WLAN0    "0.0.0.0"

#define EXAM_NET_AP_IP               "192.168.1.254"
#define EXAM_NET_STA_IP              "192.168.1.89"
#define EXAM_NET_P2P_IP              "192.168.49.207"


nvt_wifi_settings wifiConfig = {0};
BOOL _g_bFirstWifi = TRUE;

#if !defined(_NVT_SDIO_WIFI_NONE_) || !defined(_NVT_USB_WIFI_NONE_)
static BOOL g_CliConnected = FALSE;
static SWTIMER_ID sta_reconnect_timer;
int SX_TIMER_NET_CONNECT_ID = -1;
#ifndef WIFI_STA_RECONNECT_MAX_COUNT
#define WIFI_STA_RECONNECT_MAX_COUNT 10
#endif
#ifndef WIFI_STA_RECONNECT_INTERVAL
//#NT#2016/12/14#YongChang Qui -begin
//#NT#Extend wifi ap to station timeout to 6s
#define WIFI_STA_RECONNECT_INTERVAL 300
//#NT#2016/12/14#YongChang Qui -end
#endif
static int reconnect_count = 0;
#endif

#if (IPCAM_FUNC==ENABLE)
#define DHCP_LEASE_CNT    20
#else
#define DHCP_LEASE_CNT    1  // Only 1 client is allowed.
#endif

#if (WIFI_FTP_FUNC==ENABLE)
void start_ftpserver(void);
void stop_ftpserver(void);
#endif

char gNetIntf[3][6] = {
	'e', 't', 'h', '0', 0, 0,
	'e', 't', 'h', '1', 0, 0,
	'w', 'l', 'a', 'n', '0', 0,
};
char *UINet_GetConnectedIPMAC(void)
{
	return (char *)CurConnectedIpInfo.macaddr;
}

//////////////////////////////////////////////////////////////////////////
char *UINet_GetConnectedMAC(void)
{
	return currentMacAddr;
}
void UINet_SetMode(NET_MODE_SETTING mode)
{
	gMode = mode;
}
UINT32 UINet_GetMode(void)
{
	return gMode;
}
void UINet_SetAuthType(NET_AUTH_SETTING authtype)
{
	gAuthType = authtype;
}
UINT32 UINet_GetAuthType(void)
{
	return gAuthType;
}
char *UINet_GetSSID(void)
{
	return gSSID;
}
char *UINet_GetDefaultAPSSID(void)
{
	return gSSID_AP_default;
}
char *UINet_GetPASSPHRASE(void)
{
	return gPASSPHRASE;
}
char *UINet_GetDefaultAPPASSPHRASE(void)
{
	return gPASSPHRASE_AP_default;
}
char *UINet_GetIP(void)
{
	return gCurrIP;
}
char *UINet_GetMAC(void)
{
	return gMacAddr;
}
void UINet_SetSSID(char *ssid, UINT32 len)
{
	if (len > NVT_WSC_MAX_SSID_LEN) {
		DBG_ERR("max len %d\r\n", NVT_WSC_MAX_SSID_LEN);
		len = NVT_WSC_MAX_SSID_LEN;
	}
	memset(gSSID, '\0', NVT_WSC_MAX_SSID_LEN);
	snprintf(gSSID, NVT_WSC_MAX_SSID_LEN, ssid, len);
	DBG_IND("%s\r\n", gSSID);
}
void UINet_SetPASSPHRASE(char *pwd, UINT32 len)
{

	if (len > NVT_MAX_WEP_KEY_LEN) {
		DBG_ERR("max len %d\r\n", NVT_MAX_WEP_KEY_LEN);
		len = NVT_MAX_WEP_KEY_LEN;
	}
	memset(gPASSPHRASE, '\0', NVT_MAX_WEP_KEY_LEN);
	strncpy(gPASSPHRASE, pwd, len);
	DBG_IND("%s\r\n", gPASSPHRASE);

}
//0 for auto channel, 1 -14 for 11b/11g,36 -165 for 11a
void UINet_SetChannel(UINT32 channel)
{
	gChannel = channel;
}
UINT32 UINet_GetChannel(void)
{
	return gChannel;
}

void UINet_SetSendPeerIP(UINT32 val)
{
	gSendPeerIP = val;
}

_ALIGNED(4) static UINT8 WorkBuf[8192];

BOOL UINet_NvtIpc_Init(void)
{
	NVTIPC_OPEN_OBJ   openObj = {0};

	openObj.workBuffAddr = (UINT32)&WorkBuf;
	openObj.workBuffSize = sizeof(WorkBuf);
	//#NT#2016/12/15#Lincy Lin -begin
	//#NT#Change cpu2 linux init parameter because ecos also need core semaphore function.
#if defined(_CPU2_LINUX_)
	openObj.bIsSupportCoresSem = TRUE;
#endif
	//#NT#2016/12/15#Lincy Lin -end
	NvtIPC_Open(&openObj);
	return TRUE;
}

UINT32 g_Wifi_time = 0;
UINT32 g_Wifi_time2 = 0;

BOOL UINet_AppIpc_Init(void)
{
	if (_g_bFirstWifi) {
		TM_BOOT_BEGIN("network", "config");
	}
	g_Wifi_time = clock();
	DBG_DUMP("^GAP start\r\n");

	UINT32 result = 0;
#if defined(_CPU2_ECOS_)
	UINT32  uiPoolAddr, uiPoolSize;
#endif

#if (WIFI_FUNC == ENABLE)
#if !defined(_NVT_SDIO_WIFI_NONE_) || !defined(_NVT_USB_WIFI_NONE_)
	WIFIIPC_OPEN WiFiIpcOpen = {0};
	WiFiIpcOpen.sharedMemAddr = OS_GetMempoolAddr(POOL_ID_WIFIIPC);
	WiFiIpcOpen.sharedMemSize = POOL_SIZE_WIFIIPC;
	result = WiFiIpc_Open(&WiFiIpcOpen);
#endif
#endif

	if (E_OK != result) {
		DBG_ERR("WiFiIpc_Open fail\r\n");
		return FALSE;
	}

#if defined(_CPU2_ECOS_)
	uiPoolAddr = OS_GetMempoolAddr(POOL_ID_DHCPIPC);
	uiPoolSize = POOL_SIZE_DHCPIPC;
	if (E_OK != Dhcp_IpcOpen(uiPoolAddr, uiPoolSize)) {
		DBG_ERR("Dhcp_IpcOpen fail\r\n");
		return FALSE;
	}
#endif

#if(WIFI_AP_FUNC==ENABLE)
	USOCKIPC_OPEN UsockIpcOpen = {0};
	UsockIpcOpen.sharedMemAddr = OS_GetMempoolAddr(POOL_ID_USOCKIPC);
	UsockIpcOpen.sharedMemSize = POOL_SIZE_USOCKIPC;
	if (E_OK != UsockIpc_Open(&UsockIpcOpen)) {
		DBG_ERR("UsockIpc_Open fail\r\n");
		return FALSE;
	}
#endif
#if (USOCKET_CLIENT==ENABLE)
	{
		USOCKCLIIPC_OPEN UsockCliIpcOpen = {0};

		UsockCliIpcOpen.sharedMemAddr = OS_GetMempoolAddr(POOL_ID_USOCKIPC) + POOL_SIZE_USOCKIPC;
		UsockCliIpcOpen.sharedMemSize = POOL_SIZE_USOCKIPC;
		if (E_OK != UsockCliIpc_Open(&UsockCliIpcOpen)) {
			DBG_ERR("UsockCliIpc_Open fail\r\n");
			return FALSE;
		}
	}
#endif

	return TRUE;
}

BOOL UINet_AppIpc_UnInit(void)
{
#if !defined(_NVT_SDIO_WIFI_NONE_) || !defined(_NVT_USB_WIFI_NONE_)
	if (E_OK != WiFiIpc_Close()) {
		DBG_ERR("WiFiIpc_Close fail\r\n");
		return FALSE;
	}
#endif

#if defined(_CPU2_ECOS_)
	if (E_OK != Dhcp_IpcClose()) {
		DBG_ERR("Dhcp_IpcClose fail\r\n");
		return FALSE;
	}
#endif

	if (E_OK != UsockIpc_Close()) {
		DBG_ERR("UsockIpc_Close fail\r\n");
		return FALSE;
	}

#if (USOCKET_CLIENT==ENABLE)
	if (E_OK != UsockCliIpc_Close()) {
		DBG_ERR("UsockCliIpc_Close fail\r\n");
		return FALSE;
	}
#endif

	return TRUE;
}

BOOL UINet_CPU2_Init(void)
{
	return TRUE;
}


void Net_DhcpCliCb(NET_DHCP_CB_TYPE cbType, unsigned int p1, unsigned int p2, unsigned int p3)
{
	DBG_IND("::type=%d,P=%d, %d, %d\r\n", cbType, p1, p2, p3);
	if (NET_DHCP_CB_TYPE_CLI_REQ_CNT == cbType) {
		//post an event to UI-task show (cycle,count) that client request an IP
	} else if (NET_DHCP_CB_TYPE_CLI_REQ_RESULT == cbType) {
		if (p1) {
			DBG_DUMP("DHCP Client IP = 0x%x\r\n", p1);
			//post an event to UI-task to call,Dhcp_Client_Start_Ok(p1) for the first time.
			//post an event to UI-task to show Got-IP
			Ux_PostEvent(NVTEVT_WIFI_AUTHORIZED_OK, 0);

		} else {
			DBG_DUMP("DHCP Client IP Fail\r\n");
			//post an event to UI-task to Close DHCP Client,Dhcp_Client_Stop(), and show Get-IP fail
		}
	} else if (NET_DHCP_CB_TYPE_MAX == cbType) {
		OS_DumpKernelResStatus();
	}
}

#if defined(_CPU2_ECOS_)
static void UINet_DhcpSrvCBCliConn(DHCPD_CLI_STS cliSts, DHCP_ASSIGN_IP_INFO *ipInfo)
{
	DBG_IND("::isConn=%d,ip=0x%x,mac=%2x:%2x:%2x:%2x:%2x:%2x\r\n", cliSts, ipInfo->ipaddr, ipInfo->macaddr[0], ipInfo->macaddr[1], ipInfo->macaddr[2], \
			ipInfo->macaddr[3], ipInfo->macaddr[4], ipInfo->macaddr[5]);
	memcpy(&CurConnectedIpInfo, ipInfo, sizeof(DHCP_ASSIGN_IP_INFO));
	Ux_PostEvent(NVTEVT_WIFI_DHCP_REQ, 1, (UINT32)CurConnectedIpInfo.macaddr);
#if 0  // Mode change should be done by Phone APP.
	//make sure, connec to RTSP mode
	if (System_GetState(SYS_STATE_CURRMODE) != PRIMARY_MODE_MOVIE) {
		Ux_PostEvent(NVTEVT_SYSTEM_MODE, 1, PRIMARY_MODE_MOVIE);
	}
#endif
}
#endif

#if !defined(_NVT_SDIO_WIFI_NONE_) || !defined(_NVT_USB_WIFI_NONE_)

static int IS_MCAST(unsigned char *da)
{
	if ((*da) & 0x01) {
		return 1;
	} else {
		return 0;
	}
}

void UINet_RemoveUser(void)
{
	UINT32 bNewUser = FALSE;
	//CHKPNT;
	//if 2nd user connect,diconnect 1st user
	if (strncmp(currentMacAddr, gLastMacAddr, strlen(currentMacAddr)) != 0) {
		//notify current connected socket
		//new connect not get IP and socket not create
		//cannot disconnet immediate,suggest after 500 ms
		WifiApp_SendCmd(WIFIAPP_CMD_NOTIFY_STATUS, WIFIAPP_RET_REMOVE_BY_USER);
		bNewUser = TRUE;
	}
	Ux_PostEvent(NVTEVT_WIFI_AUTHORIZED_OK, 1, bNewUser);
}
void UINet_AddACLTable(void)
{
	if (strncmp(currentMacAddr, gLastMacAddr, strlen(currentMacAddr)) != 0) {
		if (strlen(gRemoveMacAddr) != 0) {
			DBG_ERR("%s not remove from ACL !!\r\n", gRemoveMacAddr);
		}

		strncpy(gRemoveMacAddr, gLastMacAddr, strlen(gLastMacAddr));
		WiFiIpc_RunSystemCmd(NULL_FILE, "iwpriv", "wlan0", "add_acl_table", gRemoveMacAddr, NULL_STR);
		strncpy(gLastMacAddr, currentMacAddr, strlen(currentMacAddr));
		DBG_IND("********** gLastMacAddr    %s\r\n", gLastMacAddr);
		DBG_IND("********** gRemoveMacAddr  %s\r\n", gRemoveMacAddr);

		BKG_PostEvent(NVTEVT_BKW_WIFI_CLEARACL);

	}
}
void UINet_ClearACLTable(void)
{
	if (strlen(gRemoveMacAddr)) {
		DBG_IND("**********clr gRemoveMacAddr  %s\r\n", gRemoveMacAddr);
		WiFiIpc_RunSystemCmd(NULL_FILE, "iwpriv", "wlan0", "rm_acl_table", gRemoveMacAddr, NULL_STR);
		memset(gRemoveMacAddr, 0, strlen(gRemoveMacAddr));
	}
}

void UINet_StationStatus_CB(char *pIntfName, char *pMacAddr, int status)
{
	//#NT#2013/12/06#KS Hung -begin
	//#NT#Post AUTHORIZED_OK event
	if (status == NVT_WIFI_STA_STATUS_ASSOCIATED) {

		DBG_IND("%s: A client(%02X:%02X:%02X:%02X:%02X:%02X) is associated\r\n",
				pIntfName, *pMacAddr, *(pMacAddr + 1), *(pMacAddr + 2), *(pMacAddr + 3), *(pMacAddr + 4), *(pMacAddr + 5));
	} else if (status == NVT_WIFI_STA_STATUS_REASSOCIATED) {
		DBG_IND("%s: A client(%02X:%02X:%02X:%02X:%02X:%02X) is reassociated\r\n",
				pIntfName, *pMacAddr, *(pMacAddr + 1), *(pMacAddr + 2), *(pMacAddr + 3), *(pMacAddr + 4), *(pMacAddr + 5));
	} else if (status == NVT_WIFI_STA_STATUS_DISASSOCIATED) {
		DBG_IND("%s: A client(%02X:%02X:%02X:%02X:%02X:%02X) is disassociated\r\n",
				pIntfName, *pMacAddr, *(pMacAddr + 1), *(pMacAddr + 2), *(pMacAddr + 3), *(pMacAddr + 4), *(pMacAddr + 5));
#if(DHCP_LEASE_CNT==1)
		if (strncmp((char *)CurConnectedIpInfo.macaddr, pMacAddr, strlen((char *)CurConnectedIpInfo.macaddr)) == 0)
#endif
		{
			Ux_PostEvent(NVTEVT_WIFI_DEAUTHENTICATED, 0);
		}
	} else if (status == NVT_WIFI_STA_STATUS_DEAUTHENTICATED) {
		DBG_IND("%s: A client(%02X:%02X:%02X:%02X:%02X:%02X) is deauthenticated\r\n",
				pIntfName, *pMacAddr, *(pMacAddr + 1), *(pMacAddr + 2), *(pMacAddr + 3), *(pMacAddr + 4), *(pMacAddr + 5));
#if(DHCP_LEASE_CNT==1)
		if (strncmp((char *)CurConnectedIpInfo.macaddr, pMacAddr, strlen((char *)CurConnectedIpInfo.macaddr)) == 0)
#endif
		{
			Ux_PostEvent(NVTEVT_WIFI_DEAUTHENTICATED, 0);
		}
	} else if (status == NVT_WIFI_STA_STATUS_PORT_AUTHORIZED) {

		DBG_IND("%s: A client(%02X:%02X:%02X:%02X:%02X:%02X) is port authorized\r\n",
				pIntfName, *pMacAddr, *(pMacAddr + 1), *(pMacAddr + 2), *(pMacAddr + 3), *(pMacAddr + 4), *(pMacAddr + 5));

		{
			snprintf(currentMacAddr, NVT_WIFIIPC_IP_MAX_LEN, "%02x%02x%02x%02x%02x%02x",
					 *pMacAddr, *(pMacAddr + 1), *(pMacAddr + 2), *(pMacAddr + 3), *(pMacAddr + 4), *(pMacAddr + 5));
			DBG_IND("********** currentMacAddr  %s\r\n", currentMacAddr);

			if (strlen(gLastMacAddr) == 0) {
				strncpy(gLastMacAddr, currentMacAddr, strlen(currentMacAddr));
				DBG_IND("********** gLastMacAddr    %s\r\n", gLastMacAddr);
			}

			//#NT#2016/03/23#Isiah Chang -begin
			//#NT#add new Wi-Fi UI flow.
#if(WIFI_UI_FLOW_VER == WIFI_UI_VER_2_0)
			WifiCmd_ReceiveCmd(TRUE); // Ready to receive Wi-Fi APP command.
#endif
			//#NT#2016/03/23#Isiah Chang -end
			Ux_PostEvent(NVTEVT_WIFI_AUTHORIZED_OK, 1, 0);
		}

	} else if (status == NVT_WIFI_AP_READY) {
		if (_g_bFirstWifi) {
			TM_BOOT_END("network", "open");
			_g_bFirstWifi = FALSE;
		}
		DBG_DUMP("^GAP ready, total period = %d us\r\n", clock() - g_Wifi_time);
        exam_msg("ap ready\r\n");
	}
//#NT#2016/03/23#Isiah Chang -begin
//#NT#add new Wi-Fi UI flow.
	else if (status == NVT_WIFI_STA_STATUS_AUTH_FAIL) {
		DBG_IND("%s: A client(%02X:%02X:%02X:%02X:%02X:%02X) is disassociated\r\n",
				pIntfName, *pMacAddr, *(pMacAddr + 1), *(pMacAddr + 2), *(pMacAddr + 3), *(pMacAddr + 4), *(pMacAddr + 5));

#if(DHCP_LEASE_CNT==1)
		if (strncmp((char *)CurConnectedIpInfo.macaddr, pMacAddr, strlen((char *)CurConnectedIpInfo.macaddr)) == 0)
#endif
		{
			Ux_PostEvent(NVTEVT_WIFI_DEAUTHENTICATED, 0);
		}
	}
//#NT#2016/03/23#Isiah Chang -end
	else {
		DBG_ERR("unknown status %d\r\n", status);
	}
	//#NT#2013/12/06#KS Hung -end

	Ux_PostEvent(NVTEVT_WIFI_STATE, 2, NET_STATE_WIFI_CB, status);

}


void UINet_Link2APStatus_CB(char *pIntfName, int status)
{
	if (status == NVT_WIFI_LINK_STATUS_CONNECTED) {
		DBG_IND("%s: Connected with AP\r\n", pIntfName);
		if (strcmp(wifiConfig.mode, "sta") == 0) {
			//#NT#2016/12/24#YongChang Qui -begin
			//#NT#Retry timer of station mode connection is separated into two phases
			g_CliConnected = TRUE;
			reconnect_count = 0;
			SxTimer_SetFuncActive(SX_TIMER_NET_CONNECT_ID, FALSE);
			//#NT#2016/12/24#YongChang Qui -end
			Ux_PostEvent(NVTEVT_EXE_WIFI_DHCP_START, 1, 1);
		}
		//#NT#2016/03/23#Isiah Chang -begin
		//#NT#add new Wi-Fi UI flow.
#if(WIFI_UI_FLOW_VER == WIFI_UI_VER_2_0)
		WifiCmd_ReceiveCmd(TRUE); // Ready to receive Wi-Fi APP command.
#endif
		//#NT#2016/03/23#Isiah Chang -end
		Ux_PostEvent(NVTEVT_WIFI_AUTHORIZED_OK, 0);
	} else if (status == NVT_WIFI_LINK_STATUS_DISCONNECTED) {
		DBG_IND("%s: Disconnected with AP\r\n", pIntfName);
		g_CliConnected = FALSE;
		Ux_PostEvent(NVTEVT_WIFI_DEAUTHENTICATED, 0);
	} else if (status == NVT_WIFI_LINK_STATUS_DEAUTHENTICATED) {
		DBG_IND("%s: Deauthenticated\r\n", pIntfName);
		g_CliConnected = FALSE;
		Ux_PostEvent(NVTEVT_WIFI_DEAUTHENTICATED, 0);
	} else if (status == NVT_WIFI_LINK_STATUS_SACN_ZREO_RESULT) {
		DBG_IND("%s: ZREO\r\n", pIntfName);
	} else {
		DBG_IND("unknown status %d \r\n", status);
	}

	Ux_PostEvent(NVTEVT_WIFI_STATE, 2, NET_STATE_WIFI_CB, status);

}

void UINet_P2PEvent_CB(char *pIntfName,  int status)
{
	DBG_IND("status %d \r\n", status);
	if (status == NVT_WIFI_P2P_EVENT_BACK2DEV) {
		Ux_PostEvent(NVTEVT_EXE_WIFI_BACK2DEV, 0);
	} else if (status == NVT_WIFI_P2P_EVENT_START_DHCPD) {
		Ux_PostEvent(NVTEVT_EXE_WIFI_DHCP_START, 1, 0);
	} else if (status == NVT_WIFI_P2P_EVENT_START_DHCPC) {
		Ux_PostEvent(NVTEVT_EXE_WIFI_DHCP_START, 1, 1);
	}
}

int UINet_WSCFlashParam_CB(nvt_wsc_flash_param *param)
{
	nvt_wifi_settings *pwifi = &wifiConfig;

	pwifi->is_configured = 1;
	strcpy(pwifi->ssid, param->SSID);
	DBG_IND("new SSID %s \r\n", pwifi->ssid);

	pwifi->config_by_ext_reg = param->WSC_CONFIGBYEXTREG;
	if (param->WEP) {
		if (param->WEP == 1) { //WEP64
			strcpy(pwifi->security, "wep64-auto");
		} else {
			strcpy(pwifi->security, "wep128-auto");
		}

		pwifi->wep_default_key = param->WEP_DEFAULT_KEY;
		strcpy(pwifi->wep_key1, param->WEP_KEY1);
		strcpy(pwifi->wep_key2, param->WEP_KEY1);
		strcpy(pwifi->wep_key3, param->WEP_KEY1);
		strcpy(pwifi->wep_key4, param->WEP_KEY1);
	} else {
		if (param->WSC_AUTH == WSC_AUTH_OPEN) {
			if (param->WSC_ENC == WSC_ENCRYPT_NONE) {
				strcpy(pwifi->security, "none");
			}
		} else if (param->WSC_AUTH == WSC_AUTH_WPAPSK) {
			if (param->WSC_ENC == WSC_ENCRYPT_TKIP) {
				strcpy(pwifi->security, "wpa-psk-tkip");
			} else if (param->WSC_ENC == WSC_ENCRYPT_AES) {
				strcpy(pwifi->security, "wpa-psk-aes");
			} else if (param->WSC_ENC == WSC_ENCRYPT_TKIPAES) {
				strcpy(pwifi->security, "wpa-psk-tkipaes");
			}
			strcpy(pwifi->passphrase, param->WSC_PSK);
		} else if (param->WSC_AUTH == WSC_AUTH_WPA2PSK) {
			if (param->WSC_ENC == WSC_ENCRYPT_TKIP) {
				strcpy(pwifi->security, "wpa2-psk-tkip");
			} else if (param->WSC_ENC == WSC_ENCRYPT_AES) {
				strcpy(pwifi->security, "wpa2-psk-aes");
			} else if (param->WSC_ENC == WSC_ENCRYPT_TKIPAES) {
				strcpy(pwifi->security, "wpa2-psk-tkipaes");
			}
			strcpy(pwifi->passphrase, param->WSC_PSK);
		} else if (param->WSC_AUTH == WSC_AUTH_WPA2PSKMIXED) {
			if (param->WSC_ENC == WSC_ENCRYPT_TKIP) {
				strcpy(pwifi->security, "wpa-auto-psk-tkip");
			} else if (param->WSC_ENC == WSC_ENCRYPT_AES) {
				strcpy(pwifi->security, "wpa-auto-psk-aes");
			} else if (param->WSC_ENC == WSC_ENCRYPT_TKIPAES) {
				strcpy(pwifi->security, "wpa-auto-psk-tkipaes");
			}
			strcpy(pwifi->passphrase, param->WSC_PSK);
		}
	}

	if (strcmp(pwifi->mode, "p2pdev") == 0) {
		DBG_IND("change p2pmode  %d\r\n", param->WLAN0_P2P_TYPE);
		pwifi->p2pmode = param->WLAN0_P2P_TYPE;
	}

	return 0;
}
void print_wscd_status(int status)
{
	switch (status) {
	case NOT_USED:
		DBG_IND("NOT_USED\r\n");
		break;
	case PROTOCOL_START:
		DBG_IND("PROTOCOL_START\r\n");
		break;
	case PROTOCOL_PBC_OVERLAPPING:
		DBG_IND("PROTOCOL_PBC_OVERLAPPING\r\n");
		break;
	case PROTOCOL_TIMEOUT:
		DBG_IND("PROTOCOL_TIMEOUT\r\n");
		break;
	case PROTOCOL_SUCCESS:
		DBG_IND("PROTOCOL_SUCCESS\r\n");
		break;
	case SEND_EAPOL_START:
		DBG_IND("SEND_EAPOL_START\r\n");
		break;
	case RECV_EAPOL_START:
		DBG_IND("RECV_EAPOL_START\r\n");
		break;
	case SEND_EAP_IDREQ:
		DBG_IND("SEND_EAP_IDREQ\r\n");
		break;
	case RECV_EAP_IDRSP:
		DBG_IND("RECV_EAP_IDRSP\r\n");
		break;
	case SEND_EAP_START:
		DBG_IND("SEND_EAP_START\r\n");
		break;
	case SEND_M1:
		DBG_IND("SEND_M1\r\n");
		break;
	case RECV_M1:
		DBG_IND("RECV_M1\r\n");
		break;
	case SEND_M2:
		DBG_IND("SEND_M2\r\n");
		break;
	case RECV_M2:
		DBG_IND("RECV_M2\r\n");
		break;
	case RECV_M2D:
		DBG_IND("RECV_M2D\r\n");
		break;
	case SEND_M3:
		DBG_IND("SEND_M3\r\n");
		break;
	case RECV_M3:
		DBG_IND("RECV_M3\r\n");
		break;
	case SEND_M4:
		DBG_IND("SEND_M4\r\n");
		break;
	case RECV_M4:
		DBG_IND("RECV_M4\r\n");
		break;
	case SEND_M5:
		DBG_IND("SEND_M5\r\n");
		break;
	case RECV_M5:
		DBG_IND("RECV_M5\r\n");
		break;
	case SEND_M6:
		DBG_IND("SEND_M6\r\n");
		break;
	case RECV_M6:
		DBG_IND("RECV_M6\r\n");
		break;
	case SEND_M7:
		DBG_IND("SEND_M7\r\n");
		break;
	case RECV_M7:
		DBG_IND("RECV_M7\r\n");
		break;
	case SEND_M8:
		DBG_IND("SEND_M8\r\n");
		break;
	case RECV_M8:
		DBG_IND("RECV_M8\r\n");
		break;
	case PROC_EAP_ACK:
		DBG_IND("PROC_EAP_ACK\r\n");
		break;
	case WSC_EAP_FAIL:
		DBG_IND("WSC_EAP_FAIL\r\n");
		break;
	case HASH_FAIL:
		DBG_IND("HASH_FAIL\r\n");
		break;
	case HMAC_FAIL:
		DBG_IND("HMAC_FAIL\r\n");
		break;
	case PWD_AUTH_FAIL:
		DBG_IND("PWD_AUTH_FAIL\r\n");
		break;
	case PROTOCOL_PIN_NUM_ERR:
		DBG_IND("PROTOCOL_PIN_NUM_ERR\r\n");
		break;
	case PROC_EAP_DONE:
		DBG_IND("PROC_EAP_DONE\r\n");
		break;
	case PROTOCOL_ERR:
		DBG_IND("PROTOCOL_ERR\r\n");
		break;
	default:
		DBG_IND("Unknown wscd status\r\n");
		break;
	}
}

void UINet_WSCEvent_CB(int evt, int status)
{
	DBG_IND("evt %d status %d\r\n", evt, status);
	if (evt == NVT_WPS_REINIT_EVENT) {
		Ux_PostEvent(NVTEVT_EXE_WIFI_REINIT, 0);
	} else if (evt == NVT_WPS_STATUS_CHANGE_EVENT) {
		print_wscd_status(status);
	} else {
		DBG_IND("unknown event\n");
	}
}

BOOL UINet_SetFixLeaseInfo(UINT32 sec, UINT32 cnt)
{
#if defined(_CPU2_ECOS_)
	NET_DHCP_LEASE_CONF leaseInfo;

	memset((void *)&leaseInfo, 0, sizeof(NET_DHCP_LEASE_CONF));//
	memcpy((void *)leaseInfo.ip_pool_start, EXAM_NET_LEASE_START_WLAN0, strlen(EXAM_NET_LEASE_START_WLAN0));
	memcpy((void *)leaseInfo.ip_pool_end, EXAM_NET_LEASE_END_WLAN0, strlen(EXAM_NET_LEASE_END_WLAN0));
	memcpy((void *)leaseInfo.ip_dns, EXAM_NET_LEASE_DNS_WLAN0, strlen(EXAM_NET_LEASE_DNS_WLAN0));
	memcpy((void *)leaseInfo.ip_wins, EXAM_NET_LEASE_WINS_WLAN0, strlen(EXAM_NET_LEASE_WINS_WLAN0));
	memcpy((void *)leaseInfo.ip_server, EXAM_NET_SRVIP_WLAN0, strlen(EXAM_NET_SRVIP_WLAN0));
	memcpy((void *)leaseInfo.ip_gateway, EXAM_NET_GATEWAY_WLAN0, strlen(EXAM_NET_GATEWAY_WLAN0));
	memcpy((void *)leaseInfo.ip_subnetmask, EXAM_NET_NETMASK_WLAN0, strlen(EXAM_NET_NETMASK_WLAN0));
	memcpy((void *)leaseInfo.ip_broadcast, EXAM_NET_BRAODCAST_WLAN0, strlen(EXAM_NET_BRAODCAST_WLAN0));
	leaseInfo.lease_time = (unsigned int)sec;
	leaseInfo.max_lease_cnt = (unsigned int)cnt;
	Dhcp_Conf_Lease(&leaseInfo);
#endif

	return TRUE;
}

//#NT#2016/10/17#YongChang -begin
//#NT#After switching to station mode, tell novacam about station ip
#if (USOCKET_CLIENT == ENABLE)
static INT32 g_change_mode_response = 0;
static int change_mode_recv(char *addr, int size)
{
	if (addr && (size > 0)) {
		DBG_IND("peer respond with %d bytes\r\n", size);
		g_change_mode_response = 1;
	}
	return TRUE;
}

static int pass_ip_to_peer(char *myIP)
{
#define MAX_RETRY 30
	int                     retry = 0, size, ret = -1;
	usocketcli_install_obj  usocketCliObj = {0};
	char                    msg[64];
	INT32                   handle;
	unsigned int            serverIP = 0;
	char                    server_ip[NVT_WIFIIPC_IP_MAX_LEN] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

	Dhcp_Client_Get_ServerIP(&serverIP);
	snprintf(server_ip, NVT_WIFIIPC_IP_MAX_LEN, "%d.%d.%d.%d", (serverIP & 0xFF), (serverIP >> 8) & 0xFF, (serverIP >> 16) & 0xFF, (serverIP >> 24) & 0xFF);

	strncpy(msg, myIP, sizeof(msg));
	strncat(msg, "\r\n", sizeof(msg));
	g_change_mode_response = 0;

	usocketCliObj.recv = change_mode_recv;
	strcpy(usocketCliObj.svrIP, server_ip);
	usocketCliObj.portNum = 7777;
	usocketCliObj.sockbufSize = 64;
	usocketCliObj.timeout = 10;
	handle = UsockCliIpc_Connect(&usocketCliObj);
	if (handle) {
		UsockCliIpc_Start();
	} else {
		goto out;
	}

	DBG_IND("Passing ip[%s] to peer[%s]\r\n", myIP, server_ip);
	while (!g_change_mode_response && retry < MAX_RETRY) {
		size = strlen(msg);
		UsockCliIpc_Send(handle, msg, &size);
		Delay_DelayMs(1000);
		retry++;
	}

	UsockCliIpc_Disconnect(&handle);
	UsockCliIpc_Stop();

out:
	if (!g_change_mode_response) {
		DBG_ERR("peer doesn't respond. return to ap mode\r\n");
		Ux_SendEvent(0, NVTEVT_EXE_WIFI_STOP, 0);
		Ux_SendEvent(&UISetupObjCtrl, NVTEVT_EXE_WIFI_MODE, 1, NET_AP_MODE);
		BKG_PostEvent(NVTEVT_BKW_WIFI_ON); // Enable WIFI interface
	} else {
		DBG_IND("wifi peer responds\r\n");
		ret = 0;
	}

	return ret;
}
#endif
//#NT#2016/10/17#YongChang -end

UINT32 UINet_DHCP_Start(UINT32 isClient)
{
	UINT32 result = E_OK;
#if defined(_CPU2_ECOS_)

	DBG_IND("isCli %d\r\n", isClient);

	if (isClient) {
		unsigned int gotIP = 0;

		WiFiIpc_ignore_down_up("wlan0", 1);

		//make sure dhcpc has closed
		Dhcp_Client_Stop();
		//make sure dhcpd has closed
		Dhcp_Server_Stop(0);
		//start dhcpc

		//#NT#2016/12/24#YongChang Qui -begin
		//#NT#Retry timer of station mode connection is separated into two phases
		g_CliConnected = FALSE;
		reconnect_count = 0;
		SxTimer_SetFuncActive(SX_TIMER_NET_CONNECT_ID, TRUE);
		//#NT#2016/12/24#YongChang Qui -end

		//set DHCP client
		result = Dhcp_Client_Start(gNvtDhcpHostNameCli, Net_DhcpCliCb, TRUE);

		if (NET_DHCP_RET_OK != result) {
			DBG_ERR("Dhcp cli fail %d\r\n", result);
		} else {
			Dhcp_Client_Get_IP(&gotIP);
			memset(gCurrIP, 0, NVT_WIFIIPC_IP_MAX_LEN);
			snprintf(gCurrIP, NVT_WIFIIPC_IP_MAX_LEN, "%d.%d.%d.%d", (gotIP & 0xFF), (gotIP >> 8) & 0xFF, (gotIP >> 16) & 0xFF, (gotIP >> 24) & 0xFF);
			DBG_IND("ip=0x%x %s\r\n", gotIP, gCurrIP);
			g_CliConnected = TRUE;
			reconnect_count = 0;
			SxTimer_SetFuncActive(SX_TIMER_NET_CONNECT_ID, FALSE);

			//#NT#2016/10/17#YongChang -begin
			//#NT#After switching to station mode, tell novacam about station ip
			if (gSendPeerIP) {
#if (USOCKET_CLIENT == ENABLE)
				gSendPeerIP = 0;
				if (!pass_ip_to_peer(gCurrIP)) {
					Ux_PostEvent(NVTEVT_WIFI_DHCP_REQ, 1, (UINT32)CurConnectedIpInfo.macaddr);
				}
#else
				DBG_ERR("USOCKET_CLIENT must be enabled to send IP to peer\r\n");
#endif
			}
			//#NT#2016/10/17#YongChang -end
		}
		WiFiIpc_ignore_down_up("wlan0", 0);
	} else {
		//set DHCP server
		Dhcp_Set_Interface("wlan0");
		Dhcp_RegSrvCBCliAttach(UINet_DhcpSrvCBCliConn);
		//#NT#2015/10/13#Isiah Chang[0084311]-begin
#if 0 // Multi-connection
		UINet_SetFixLeaseInfo(1200, DHCP_LEASE_CNT);
#else // Single-connection
		UINet_SetFixLeaseInfo(30, DHCP_LEASE_CNT);
#endif
		//#NT#2015/10/13#Isiah Chang[0084311]-end
		result = Dhcp_Server_Start(gNvtDhcpHostNameSrv);
		if (NET_DHCP_RET_OK != result) {
			DBG_ERR("Dhcp svr fail\r\n");
		}
	}

	Ux_PostEvent(NVTEVT_WIFI_STATE, 2, NET_STATE_DHCP_START, result);
#endif

	return result;
}

void UINet_WifiSettings(nvt_wifi_settings *pwifi, UINT32 mode, UINT32 security)
{
	UIMenuStoreInfo *ptMenuStoreInfo = UI_GetMenuInfo();

	if (mode == NET_AP_MODE || mode == NET_WPS_AP_PBC_MODE) {

		if (mode == NET_AP_MODE) {
			strcpy(pwifi->mode, "ap");
		} else {
			strcpy(pwifi->mode, "wps-ap-pbc");
			pwifi->p2pmode = P2P_TMP_GO;
			pwifi->is_configured = 1;
		}

		if (ptMenuStoreInfo->strSSID[0] == 0) {
#if (MAC_APPEN_SSID==ENABLE)
			snprintf(pwifi->ssid, NVT_WSC_MAX_SSID_LEN, "%s%02x%02x%02x%02x%02x%02x", gSSID, gMacAddr[0], gMacAddr[1], gMacAddr[2],
					 gMacAddr[3], gMacAddr[4], gMacAddr[5]);
#else
			snprintf(pwifi->ssid, NVT_WSC_MAX_SSID_LEN, "%s", gSSID);
#endif
		} else {
			snprintf(pwifi->ssid, NVT_WSC_MAX_SSID_LEN, "%s", gSSID);
		}

		strcpy(pwifi->device_name, "Wireless AP");
	}

	else if (mode == NET_STATION_MODE) {
		strcpy(pwifi->mode, "sta");
		snprintf(pwifi->ssid, NVT_WSC_MAX_SSID_LEN, "%s", gSSID);
		strcpy(pwifi->device_name, "Wireless STA");
	} else if (mode == NET_P2P_DEV_MODE) {
		pwifi->p2pmode = P2P_DEVICE;
		strcpy(pwifi->mode, "p2pdev");
		/*p2p note ; p2p GO's SSID must in the form "DIRECT-xy-anystring"  xy is random*/
		strcpy(pwifi->ssid, "DIRECT-de-12345678");
		strcpy(pwifi->device_name, "p2p-dev");
	} else if (mode == NET_P2P_GO_MODE) {
		pwifi->p2pmode = P2P_TMP_GO;
		strcpy(pwifi->mode, "p2pgo");
		/*p2p note ; p2p GO's SSID must in the form "DIRECT-xy-anystring"  xy is random*/
		strcpy(pwifi->ssid, "DIRECT-go-12345678");
		pwifi->is_configured = 1;            // let wscd under configured mode
		strcpy(pwifi->device_name, "p2p-go");
	} else {
		DBG_ERR("unknown mode %d\r\n", mode);
	}

	if (security == NET_AUTH_NONE) {
		strcpy(pwifi->security, "none");
	} else if (security == NET_AUTH_WPA2) {
		strcpy(pwifi->security, "wpa2-psk-aes");
	} else if (security == NET_AUTH_WPA) {
		strcpy(pwifi->security, "wpa-psk-tkip");
	} else {
		DBG_ERR("unknown security %d\r\n", security);
	}

	pwifi->channel = gChannel;

	strcpy(pwifi->passphrase, gPASSPHRASE);

	pwifi->wep_default_key = 0;
	if (strcmp(pwifi->security, "wep64-auto") == 0) {
		strcpy(pwifi->wep_key1, "1234567890");
		strcpy(pwifi->wep_key2, "1234567890");
		strcpy(pwifi->wep_key3, "1234567890");
		strcpy(pwifi->wep_key4, "1234567890");
	} else {
		strcpy(pwifi->wep_key1, "12345678901234567890123456");
		strcpy(pwifi->wep_key2, "12345678901234567890123456");
		strcpy(pwifi->wep_key3, "12345678901234567890123456");
		strcpy(pwifi->wep_key4, "12345678901234567890123456");
	}
}

void UINet_WifiP2PReInit(void)
{
	nvt_wifi_settings *pwifi = &wifiConfig;

	//CHKPNT;
	WiFiIpc_Config(pwifi);
	if (strcmp(pwifi->mode, "ap") == 0) {
		WiFiIpc_interface_config("wlan0", gCurrIP, "255.255.255.0");
	} else if (strcmp(pwifi->mode, "sta") == 0) {
		WiFiIpc_delete_interface_addr("wlan0");
		WiFiIpc_interface_up("wlan0");
	} else if (strcmp(pwifi->mode, "p2pgo") == 0) {
		if (pwifi->p2pmode == P2P_TMP_GO) {
			WiFiIpc_delete_interface_addr("wlan0");
			memset(gCurrIP, 0, NVT_WIFIIPC_IP_MAX_LEN);
			snprintf(gCurrIP, NVT_WIFIIPC_IP_MAX_LEN, "%s", EXAM_NET_AP_IP);
			WiFiIpc_interface_config("wlan0", gCurrIP, "255.255.255.0");
			DBG_IND(" setip=%s -\r\n", gCurrIP);
			UINet_DHCP_Start(FALSE);
		}
	} else if (strcmp(pwifi->mode, "p2pdev") == 0) {
		if (pwifi->p2pmode == P2P_DEVICE) {
			WiFiIpc_delete_interface_addr("wlan0");
			WiFiIpc_interface_up("wlan0");
		} else if (pwifi->p2pmode == P2P_CLIENT) {
			WiFiIpc_delete_interface_addr("wlan0");
			WiFiIpc_interface_up("wlan0");
		}
	}

	if (WiFiIpc_wscd_is_start()) {
		if (!pwifi->wlan_disable && !pwifi->wps_disable) {
			WiFiIpc_wsc_reinit(); //trigger wsc to reinit
			DBG_IND("wsc_reinit\r\n");
		} else {
			WiFiIpc_wsc_stop_service(); //trigger wsc to stop service
			DBG_IND("wsc_stop_service\r\n");
		}
	} else {
		DBG_IND("wsc_stop_service\r\n");
		if (!pwifi->wlan_disable && !pwifi->wps_disable) {
			WiFiIpc_create_wscd();
		}
	}

}

void UINet_WifiBack2Dev(void)
{
	nvt_wifi_settings *pwifi = &wifiConfig;
	//CHKPNT;
	UINet_WifiSettings(pwifi, NET_P2P_DEV_MODE, NET_AUTH_NONE);
	Ux_PostEvent(NVTEVT_EXE_WIFI_REINIT, 0);
}

INT32 UINet_WifiInit(UINT32 mode, UINT32 security)
{
//#NT#2016/04/14#YongChang_Qui -begin
//#NT#Log Support wifi off flow for power on cpu2 but power off wifi module
#if (ECOS_POWER_OFF_FLOW == ENABLE || WIFI_POWER_OFF_FLOW == ENABLE || defined(_CPU2_LINUX_))
//#NT#2016/04/14#YongChang_Qui -end
	int bInit = FALSE;
#else
	static int bInit = FALSE;
#endif

	UINT32 result = 0;
	nvt_wifi_settings *pwifi = &wifiConfig;

	DBG_IND("set wifi %d  %d \r\n", mode, security);

#if (defined(_CPU2_ECOS_) || defined(_MODEL_WiFiSDV_) || defined(_MODEL_FIREFLY_)||defined(_MODEL_B5_) || defined(_MODEL_CarDV_))
	if (_g_bFirstWifi) {
		TM_BOOT_BEGIN("network", "poweron");
	}
	DrvWiFi_PowerOn();
	if (_g_bFirstWifi) {
		TM_BOOT_END("network", "poweron");
	}
#endif

	memset(pwifi, 0, sizeof(nvt_wifi_settings));

	if (!bInit) {
		if (_g_bFirstWifi) {
			TM_BOOT_BEGIN("network", "netdev");
		}
		result = WiFiIpc_init_wlan0_netdev(0);
		if (_g_bFirstWifi) {
			TM_BOOT_END("network", "netdev");
		}
		if (result != 0) {
			DBG_ERR("wifi init fail %d\r\n", result);
			return E_SYS;
		} else {
			DBG_IND("wifi init succesful \r\n");
		}
		#if (ECOS_POWER_OFF_FLOW == ENABLE || WIFI_POWER_OFF_FLOW == ENABLE || defined(_CPU2_LINUX_))
		#else
		bInit = TRUE;
		#endif
	}

	//register sta status cb
	WiFiIpc_register_wifi_sta_status_cb(WIFI_IFNAME, UINet_StationStatus_CB);

	//register link status cb
	WiFiIpc_register_wifi_link_status_cb(WIFI_IFNAME, UINet_Link2APStatus_CB);
	WiFiIpc_register_wsc_event_cb(UINet_WSCEvent_CB);
	WiFiIpc_register_wsc_flash_param_cb(UINet_WSCFlashParam_CB);
	WiFiIpc_register_p2p_event_indicate_cb("wlan0", UINet_P2PEvent_CB);

	if (WiFiIpc_get_wlan0_efuse_mac(gMacAddr) < 0) {
		DBG_IND("wifi_get_wlan0_efuse_mac() fail. Use hardcoded mac.\r\n");
		WiFiIpc_set_mac_address("wlan0", "\x00\x00\x00\x81\x89\xe5");
	} else {
		unsigned char zero[] = {0, 0, 0, 0, 0, 0};
		DBG_IND("wlan0 efuse mac [%02x:%02x:%02x:%02x:%02x:%02x]\r\n",
				gMacAddr[0], gMacAddr[1], gMacAddr[2],
				gMacAddr[3], gMacAddr[4], gMacAddr[5]);
		//sanity check
		if (!memcmp(gMacAddr, zero, NVT_WIFIIPC_MAC_ADDR_LEN) || IS_MCAST((unsigned char *)gMacAddr)) {
			DBG_ERR("efuse mac format is invalid. Use hardcoded mac.\r\n");
			WiFiIpc_set_mac_address("wlan0", "\x00\x00\x00\x81\x89\xe5");
			snprintf(gMacAddr, NVT_WIFIIPC_MAC_ADDR_LEN, "%c%c%c%c%c%c", 0, 0, 0, 0x81, 0x89, 0xe5);
			DBG_ERR("default wlan0 efuse mac [%02x:%02x:%02x:%02x:%02x:%02x]\r\n",
					gMacAddr[0], gMacAddr[1], gMacAddr[2],
					gMacAddr[3], gMacAddr[4], gMacAddr[5]);
		} else {
			WiFiIpc_set_mac_address("wlan0", gMacAddr);
		}
	}

	UINet_WifiSettings(pwifi, mode, security);
	WiFiIpc_generate_pin_code(pwifi->wsc_pin);

#if defined(_CPU2_LINUX_)
	pwifi->channel = 6;//0 for auto-channel scan ; 6 for Linux CarDV_WIFI test
	WiFiIpc_set_frequency(NVT_WIFI_24G);
#endif

#if 0 //5G auto-channel scan in China
	pwifi->channel = 149;
	WiFiIpc_set_country("zh-cn");
	WiFiIpc_set_frequency(NVT_WIFI_5G);
#endif

#if 0 //Only 1 connection is allowed
	WiFiIpc_RunSystemCmd(NULL_FILE, "iwpriv", "wlan0", "set_mib", "stanum=1", NULL_STR);
#endif

	WiFiIpc_Config(pwifi);

//#NT#2016/08/25#YongChang_Qui -begin
//#NT#Default to disable wps function
#if 0
#if 0
	create_wscd();
	DBG_IND("create_wscd \r\n");
#else
	if (WiFiIpc_wscd_is_start()) {
		WiFiIpc_wsc_reinit(); //trigger wsc to reinit
		DBG_IND("wsc_reinit\r\n");
	} else {
		WiFiIpc_create_wscd();
		DBG_IND("create_wscd\r\n");
	}
#endif
#endif
//#NT#2016/08/25#YongChang_Qui -end

	if ((mode == NET_AP_MODE) || (mode == NET_P2P_GO_MODE) || (mode == NET_WPS_AP_PBC_MODE)) {
		memset(gCurrIP, 0, NVT_WIFIIPC_IP_MAX_LEN);
		snprintf(gCurrIP, NVT_WIFIIPC_IP_MAX_LEN, "%s", EXAM_NET_AP_IP);
		if (_g_bFirstWifi) {
			TM_BOOT_BEGIN("network", "up");
		}
#if defined(_CPU2_ECOS_)
		WiFiIpc_interface_config("wlan0", gCurrIP, "255.255.255.0");
#else
		WiFiIpc_interface_up("wlan0");
#endif
		if (_g_bFirstWifi) {
			TM_BOOT_END("network", "up");
		}
		DBG_IND(" setip=%s -\r\n", gCurrIP);
		if (_g_bFirstWifi) {
			TM_BOOT_BEGIN("network", "dhcp");
		}
#if defined(_CPU2_ECOS_)
		result = UINet_DHCP_Start(FALSE);
#endif
		if (_g_bFirstWifi) {
			TM_BOOT_END("network", "dhcp");
		}
	} else if (mode == NET_STATION_MODE) {
		g_CliConnected = FALSE;
		reconnect_count = 0;
		WiFiIpc_delete_interface_addr("wlan0");
#if 1
		WiFiIpc_interface_up("wlan0");
#else //no DHCP
		memcpy((void *)gCurrIP, EXAM_NET_STA_IP, NVT_WIFIIPC_IP_MAX_LEN);
		WiFiIpc_interface_config("wlan0", gCurrIP, "255.255.255.0");
#endif
	} else if (mode == NET_P2P_DEV_MODE) {
		WiFiIpc_delete_interface_addr("wlan0");
		WiFiIpc_interface_up("wlan0");
	} else {
		DBG_ERR("mode %d unknown\r\n", mode);
	}

	DBG_IND("Default IP=%s\r\n", gCurrIP);


	if (_g_bFirstWifi) {
		TM_BOOT_BEGIN("network", "hfs");
	}
	if (result == E_OK) {
		result = UINet_HFSInit();
	}
	//#NT#2018/03/21#Niven Cho -begin
    //#NT#RTSP_PUSH_MODE
    if (result == E_OK) {
		result = UINet_RtspInit();
	}
    //#NT#2018/03/21#Niven Cho -end
	if (_g_bFirstWifi) {
		TM_BOOT_END("network", "hfs");
	}
#if defined(_CPU2_LINUX_)
#if (IOT_P2P_FUNC == ENABLE)
	if (result == E_OK) {
		result = UINet_IotP2PInit();
	}
#endif
#endif

	if (_g_bFirstWifi) {
		TM_BOOT_END("network", "config");
	}
	if (_g_bFirstWifi) {
		TM_BOOT_BEGIN("network", "open");
	}

	//#NT#2016/08/05#Niven Cho -begin
	//#NT#MSDC-NET
#if (MSDCVENDOR_NVT == ENABLE)
	MsdcNvtCb_Net(TRUE);
#endif
	//#NT#2016/08/05#Niven Cho -end

	Ux_PostEvent(NVTEVT_WIFI_STATE, 2, NET_STATE_WIFI_START, result);

	return result;
}

INT32 UINet_WifiUnInit(UINT32 mode)
{
	UINT32 ret = 0;

#if defined(_CPU2_LINUX_)
#if (IOT_P2P_FUNC == ENABLE)
	ret = UINet_IotP2PUnInit();
#endif
#endif

	//#NT#2016/08/05#Niven Cho -begin
	//#NT#MSDC-NET
#if (MSDCVENDOR_NVT == ENABLE)
	MsdcNvtCb_Net(FALSE);
#endif
	//#NT#2016/08/05#Niven Cho -end

	UINet_HFSUnInit();
	//#NT#2018/03/21#Niven Cho -begin
    //#NT#RTSP_PUSH_MODE
	UINet_RtspUnInit();
    //#NT#2018/03/21#Niven Cho -end
	WiFiIpc_delete_interface_addr("wlan0");

#if defined(_CPU2_ECOS_)

	Dhcp_Server_Stop(FALSE);

	Dhcp_Client_Stop();

#endif

	//#NT#2016/08/25#YongChang_Qui -begin
	//#NT#Default to disable wps function
	//WiFiIpc_wsc_stop_service(); //trigger wsc to stop service
	//#NT#2016/08/25#YongChang_Qui -end
	ret = WiFiIpc_interface_down("wlan0");

//#NT#2016/04/14#YongChang_Qui -begin
//#NT#Log Support wifi off flow for power on cpu2 but power off wifi module
#if (ECOS_POWER_OFF_FLOW == DISABLE && WIFI_POWER_OFF_FLOW == ENABLE)
	ret = WiFiIpc_interface_remove("wlan0");
#endif
//#NT#2016/04/14#YongChang_Qui -end

#if (defined(_CPU2_ECOS_) || defined(_MODEL_WiFiSDV_) || defined(_MODEL_FIREFLY_) || defined(_MODEL_B5_) || defined(_MODEL_CarDV_))
	DrvWiFi_PowerOff();
#endif

	if (mode == NET_STATION_MODE) {
		g_CliConnected = FALSE;
		reconnect_count = 0;
	}

	return ret;
}

void UINet_print_bss_desc(struct nvt_bss_desc *pBss)
{
	char ssidbuf[40], tmp2Buf[20], tmp3Buf[20];

	//BSSID
	DBG_DUMP("%02x:%02x:%02x:%02x:%02x:%02x",
			 pBss->bssid[0], pBss->bssid[1], pBss->bssid[2],
			 pBss->bssid[3], pBss->bssid[4], pBss->bssid[5]);

	//channel
	DBG_DUMP("  %3d", pBss->channel);

	//band
	tmp2Buf[0] = 0;
	if (pBss->network & NVT_WIRELESS_11N) {
		strcat(tmp2Buf, "N");
	}
	if (pBss->network & NVT_WIRELESS_11G) {
		strcat(tmp2Buf, "G");
	}
	if (pBss->network & NVT_WIRELESS_11B) {
		strcat(tmp2Buf, "B");
	}
	DBG_DUMP("%5s", tmp2Buf);

	//rssi
	DBG_DUMP(" %3d", pBss->rssi);

	//signal quality
	DBG_DUMP("   %3d ", pBss->sq);

	//Security
	memset(tmp3Buf, 0x00, sizeof(tmp3Buf));

	// refer to get_security_info() for details
	if ((pBss->capability & nvt_cPrivacy) == 0) {
		strcpy(tmp2Buf, "");
		strcpy(tmp3Buf, "None");
	} else {
		if (pBss->t_stamp[0] == 0) {
			strcpy(tmp2Buf, "");
			strcpy(tmp3Buf, "WEP");
		} else {
			int wpa_exist = 0, idx = 0;
			if (pBss->t_stamp[0] & 0x0000ffff) {
				idx = sprintf(tmp2Buf, "%s", "WPA");
				if (((pBss->t_stamp[0] & 0x0000f000) >> 12) == 0x4) {
					idx += sprintf(tmp2Buf + idx, "%s", "-PSK");
				} else if (((pBss->t_stamp[0] & 0x0000f000) >> 12) == 0x2) {
					idx += sprintf(tmp2Buf + idx, "%s", "-1X");
				}
				wpa_exist = 1;

				if (((pBss->t_stamp[0] & 0x00000f00) >> 8) == 0x5) {
					sprintf(tmp3Buf, "%s", "AES/TKIP");
				} else if (((pBss->t_stamp[0] & 0x00000f00) >> 8) == 0x4) {
					sprintf(tmp3Buf, "%s", "AES");
				} else if (((pBss->t_stamp[0] & 0x00000f00) >> 8) == 0x1) {
					sprintf(tmp3Buf, "%s", "TKIP");
				}
			}
			if (pBss->t_stamp[0] & 0xffff0000) {
				if (wpa_exist) {
					idx += sprintf(tmp2Buf + idx, "%s", "/");
				}
				idx += sprintf(tmp2Buf + idx, "%s", "WPA2");
				if (((pBss->t_stamp[0] & 0xf0000000) >> 28) == 0x4) {
					idx += sprintf(tmp2Buf + idx, "%s", "-PSK");
				} else if (((pBss->t_stamp[0] & 0xf0000000) >> 28) == 0x2) {
					idx += sprintf(tmp2Buf + idx, "%s", "-1X");
				}

				if (((pBss->t_stamp[0] & 0x0f000000) >> 24) == 0x5) {
					sprintf(tmp3Buf, "%s", "AES/TKIP");
				} else if (((pBss->t_stamp[0] & 0x0f000000) >> 24) == 0x4) {
					sprintf(tmp3Buf, "%s", "AES");
				} else if (((pBss->t_stamp[0] & 0x0f000000) >> 24) == 0x1) {
					sprintf(tmp3Buf, "%s", "TKIP");
				}
			}
		}
	}
	DBG_DUMP("%-10s ", tmp3Buf);
	DBG_DUMP("%-16s ", tmp2Buf);

	//SSID
	memcpy(ssidbuf, pBss->ssid, pBss->ssidlen);
	ssidbuf[pBss->ssidlen] = '\0';
	DBG_DUMP("%s\r\n", ssidbuf);
}


void UINet_DumpAllSSID(NVT_SS_STATUS_Tp pStatus)
{
	int i;
	struct nvt_bss_desc *pBss;

	if (!pStatus) {
		DBG_ERR("no pStatus \r\n");
		return ;
	}

	DBG_DUMP("pStatus->number %d\r\n", pStatus->number);

	//success
	DBG_DUMP(" #      BSSID          ch band rssi  sq  enc        auth             SSID\r\n");
	for (i = 0; i < pStatus->number && pStatus->number != 0xff; i++) {
		pBss = &pStatus->bssdb[i];
		DBG_DUMP("%2d ", i);
		UINet_print_bss_desc(pBss);
	}
	//free(pStatus);
	//pStatus = NULL;

}

void UINet_SiteSurvey(char *ifname, NVT_SS_STATUS_Tp pStatus)
{
	int status = 0;
	unsigned char res;
	int wait_time;
	//int i;
	//BssDscr *pBss;
	//struct nvt_bss_desc *pBss;
	//NVT_SS_STATUS_Tp pStatus=NULL;
	//NVT_SS_STATUS_Tp pStatus = &UINet_ss_status;

	//below is copied from formWlSiteSurvey()
	{
		// issue scan request
		wait_time = 0;
		while (1) {
			// ==== modified by GANTOE for site survey 2008/12/26 ====
			switch (WiFiIpc_getWlSiteSurveyRequest(ifname, &status)) {
			case -2:
				DBG_ERR("Auto scan running!!please wait...\n");
				goto ss_err;
				break;
			case -1:
				DBG_ERR("Site-survey request failed!\n");
				goto ss_err;
				break;
			default:
				break;
			}

			if (status != 0) {  // not ready
				if (wait_time++ > 5) {
					DBG_ERR("scan request timeout!\n");
					goto ss_err;
				}
				//sleep(1);
				Delay_DelayMs(1000);
			} else {
				break;
			}
		}

		// wait until scan completely
		wait_time = 0;
		while (1) {
			res = 1;    // only request request status
			if (WiFiIpc_getWlSiteSurveyResult(ifname, (NVT_SS_STATUS_Tp)&res, sizeof(res)) < 0) {
				DBG_ERR("Read site-survey status failed!");
				goto ss_err;
			}
			if (res == 0xff) {   // in progress
				if (wait_time++ > 10) {
					DBG_ERR("scan timeout!");
					goto ss_err;
				}
				//sleep(1);
				Delay_DelayMs(1000);
			} else {
				break;
			}
		}
	}

	//below is copied from wlSiteSurveyTbl()
	/*
	if (pStatus==NULL) {
	    pStatus = calloc(1, sizeof(NVT_SS_STATUS_T));
	    if ( pStatus == NULL ) {
	        DBG_IND("Allocate buffer failed!\n");
	        goto ss_err;
	    }
	}
	*/

	pStatus->number = 0; // request BSS DB
	if (WiFiIpc_getWlSiteSurveyResult(ifname, pStatus, sizeof(NVT_SS_STATUS_T)) < 0) {
		DBG_ERR("Read site-survey status failed!\n");
		//free(pStatus);
		goto ss_err;
	}

ss_err:
	//failed
	//free(pStatus);
	DBG_ERR("fail\r\n");
}

void UINet_P2PDump(NVT_SS_STATUS_Tp pStatus)
{
	struct nvt_bss_desc *pBss;
	int i;

	DBG_DUMP("pStatus->number=%d\r\n", pStatus->number);
	DBG_DUMP(" # Channel Device address    Role WSCMethod Device Name\r\n");
	for (i = 0; i < pStatus->number && pStatus->number != 0xff; i++) {
		pBss = &pStatus->bssdb[i];
		DBG_DUMP("%2d %3d     ", i, pBss->channel);
		DBG_DUMP("%02x:%02x:%02x:%02x:%02x:%02x ",
				 pBss->p2paddress[0], pBss->p2paddress[1], pBss->p2paddress[2],
				 pBss->p2paddress[3], pBss->p2paddress[4], pBss->p2paddress[5]);
		if (pBss->p2prole == 2) { //2: device
			DBG_DUMP("DEV");
		} else if (pBss->p2prole == 1) { // 1:GO
			DBG_DUMP("GO ");
		} else {
			DBG_DUMP("???");
		}
		DBG_DUMP("  0x%04x", pBss->p2pwscconfig);
		DBG_DUMP("     %s\r\n", pBss->p2pdevname);
	}
}
void UINet_P2PScan(char *ifname, NVT_SS_STATUS_Tp pStatus)
{
	int status = 0;
	unsigned char res;
	int wait_time;

	// issue scan request
	wait_time = 0;
	while (1) {
		switch (WiFiIpc_getWlP2PScanRequest(ifname, &status)) {
		case -2:
			DBG_ERR("Auto scan running!!please wait...\r\n");
			goto ss_err;
			break;
		case -1:
			DBG_ERR("Site-survey request failed!\r\n");
			goto ss_err;
			break;
		default:
			break;
		}

		if (status != 0) { // not ready
			if (wait_time++ > 5) {
				DBG_ERR("scan request timeout!\r\n");
				goto ss_err;
			}
			//sleep(1);
			Delay_DelayMs(1000);
		} else {
			break;
		}
	}

	// wait until scan completely
	wait_time = 0;
	while (1) {
		res = 1;
		if (WiFiIpc_getWlP2PScanResult(ifname, (NVT_SS_STATUS_Tp)&res, sizeof(res)) < 0) {
			DBG_ERR("Read site-survey status failed!\r\n");
			goto ss_err;
		}

		if (res == 0xff) { // in progress
			if (wait_time++ > 20) {
				DBG_ERR("scan timeout!\r\n");
				goto ss_err;
			}
			//sleep(1);
			Delay_DelayMs(1000);
		} else {
			break;
		}
	}

	// get scan result
	pStatus->number = 0; // request BSS DB
	if (WiFiIpc_getWlP2PScanResult(ifname, pStatus, sizeof(NVT_SS_STATUS_T)) < 0) {
		DBG_ERR("Read site-survey status failed!\r\n");
		//free(pStatus);
		return;
	}

ss_err:
	DBG_ERR("fail\r\n");

}

UINT32 UINet_P2PConnect(NVT_SS_STATUS_Tp pStatus, UINT32 idx)
{
	ER result = 0;
	if (idx >= pStatus->number) {
		DBG_ERR("wifi p2pconnect <0~%d>\r\n", (pStatus->number - 1));
	} else {
		NVT_P2P_PROVISION_COMM_T p2pProvisionComm;

		//p2pProvisionComm.wsc_config_method = CONFIG_METHOD_DISPLAY;
		//p2pProvisionComm.wsc_config_method = CONFIG_METHOD_KEYPAD;
		p2pProvisionComm.wsc_config_method = CONFIG_METHOD_PBC;
		p2pProvisionComm.channel = pStatus->bssdb[idx].channel;
		memcpy(p2pProvisionComm.dev_address, pStatus->bssdb[idx].p2paddress, 6);

		result = WiFiIpc_sendP2PProvisionCommInfo("wlan0", &p2pProvisionComm);
		if (result < 0) {
			DBG_ERR("SendP2PProvisionCommInfo fail\r\n");
		}
	}

	return result;
}

void WIFI_CliReconnect(UINT32 uiEvent)
{
	nvt_wifi_settings *pwifi = &wifiConfig;

	if (strcmp(pwifi->mode, "sta")) {
		return;
	}

	if (WIFI_STA_RECONNECT_MAX_COUNT && reconnect_count >= WIFI_STA_RECONNECT_MAX_COUNT) {
		DBG_ERR("wifi station aborts retry\r\n");
		UINet_CliReConnect(0);
		Ux_PostEvent(NVTEVT_WIFI_STATE, 2, NET_STATE_WIFI_CONNECTING, NET_CONNECTING_TIMEOUT);
		return;
	} else {
		reconnect_count++;
	}

	if (g_CliConnected == TRUE) {
		UINet_CliReConnect(0);
		return;
	}
	DBG_WRN("Wifi Reconnect %d \r\n", reconnect_count);
	WiFiIpc_interface_down("wlan0");
	WiFiIpc_Config(pwifi);
	WiFiIpc_interface_up("wlan0");
	return;
}

BOOL UINet_CliReConnect(UINT32 bStart)
{
	static int bInit = 0;
	nvt_wifi_settings *pwifi = &wifiConfig;

	if (strcmp(pwifi->mode, "sta")) {
		return FALSE;
	}

	if (!WIFI_STA_RECONNECT_MAX_COUNT) {
		return TRUE;
	}

	if (!bInit) {
		if (SwTimer_Open(&sta_reconnect_timer, WIFI_CliReconnect) != E_OK) {
			DBG_ERR("open timer fail\r\n");
			return FALSE;
		}
		SwTimer_Cfg(sta_reconnect_timer, WIFI_STA_RECONNECT_INTERVAL * 20, SWTIMER_MODE_FREE_RUN);
		bInit = 1;
	}

	if (bStart) {
		DBG_IND("Start Station Reconnect Timer\r\n");
	} else {
		DBG_IND("Stop Station Reconnect Timer\r\n");
	}

	if (bStart) {
		SwTimer_Start(sta_reconnect_timer);
	} else {
		SwTimer_Stop(sta_reconnect_timer);
	}

	return TRUE;
}


#else //#if (_NVT_SDIO_WIFI_ != _NVT_SDIO_WIFI_NONE_ || _NVT_USB_WIFI_ != _NVT_USB_WIFI_NONE_)

#if defined(_CPU2_ECOS_)
static NET_IP_PAIR gExamNetIP[NET_IP_PAIR_MAX_CNT] = {
	//eth0
	{EXAM_NET_IP_ETH0, EXAM_NET_NETMASK_ETH0, EXAM_NET_BRAODCAST_ETH0, EXAM_NET_GATEWAY_ETH0, EXAM_NET_SRVIP_ETH0, 0},
	//eth1
	{EXAM_NET_IP_ETH1, EXAM_NET_NETMASK_ETH1, EXAM_NET_BRAODCAST_ETH1, EXAM_NET_GATEWAY_ETH1, EXAM_NET_SRVIP_ETH1, 1},
	//wlan0
	{EXAM_NET_IP_WLAN0, EXAM_NET_NETMASK_WLAN0, EXAM_NET_BRAODCAST_WLAN0, EXAM_NET_GATEWAY_WLAN0, EXAM_NET_SRVIP_WLAN0, 2}
};
#endif

INT32 UINet_WifiInit(UINT32 mode, UINT32 security)
{
	NET_DHCP_RET ret = 0;

#if defined(_CPU2_ECOS_)
	DBG_IND("set Lan %d\r\n", mode);

	if (mode == NET_AP_MODE) { //ap mode
		ret = Dhcp_Init_Network_With_IP(gExamNetIP, NET_IP_PAIR_MAX_CNT);
		if (NET_DHCP_RET_OK == ret) {
			Dhcp_RegSrvCBCliAttach(UINet_DhcpSrvCBCliConn);
			ret = Dhcp_Server_Start(gNvtDhcpHostNameSrv);
			if (NET_DHCP_RET_OK != ret) {
				DBG_ERR("AP fail %d\r\n", ret);
			} else {
				memset(gCurrIP, 0, NVT_WIFIIPC_IP_MAX_LEN);
				snprintf(gCurrIP, NVT_WIFIIPC_IP_MAX_LEN, "%s", EXAM_NET_IP_ETH0);
			}
		} else {
			DBG_ERR("eth0 fail %d\r\n", ret);
		}
	} else if (mode == NET_STATION_MODE) { //station mode
		//start DHCP client
		unsigned int gotIP = 0;
		ret = Dhcp_Client_Start(gNvtDhcpHostNameCli, Net_DhcpCliCb, TRUE);

		if (NET_DHCP_RET_OK != ret) {
			DBG_ERR("dhcp cli fail\r\n");
			return E_SYS;
		} else {
			Dhcp_Client_Get_IP(&gotIP);
			memset(gCurrIP, 0, NVT_WIFIIPC_IP_MAX_LEN);
			snprintf(gCurrIP, NVT_WIFIIPC_IP_MAX_LEN, "%d.%d.%d.%d", (gotIP & 0xFF), (gotIP >> 8) & 0xFF, (gotIP >> 16) & 0xFF, (gotIP >> 24) & 0xFF);
			DBG_IND("^R:::: ip=0x%x %s\r\n", gotIP, gCurrIP);

#if 0
			debug_err(("%x \r\n", gotIP));
			debug_err(("%x \r\n", (gotIP & 0xFF)));
			debug_err(("%x \r\n", (gotIP >> 8) & 0xFF));
			debug_err(("%x \r\n", (gotIP >> 16) & 0xFF));
			debug_err(("%x \r\n", (gotIP >> 24) & 0xFF));
#endif

		}
		//CHKPNT;

	} else {
		ret = E_SYS;
		DBG_ERR("not support mode %d\r\n", mode);
	}

	if (ret == NET_DHCP_RET_OK) {
		ret = UINet_HFSInit();
	}
	//#NT#2018/03/21#Niven Cho -begin
    //#NT#RTSP_PUSH_MODE
    if (ret == E_OK) {
		ret = UINet_RtspInit();
	}
    //#NT#2018/03/21#Niven Cho -end
#endif

	//#NT#2016/08/05#Niven Cho -begin
	//#NT#MSDC-NET
	MsdcNvtCb_Net(TRUE);
	//#NT#2016/08/05#Niven Cho -end

	return ret;
}

INT32 UINet_WifiUnInit(UINT32 mode)
{
	UINT32 ret = 0;

	//#NT#2016/08/05#Niven Cho -begin
	//#NT#MSDC-NET
#if (MSDCVENDOR_NVT == ENABLE)
	MsdcNvtCb_Net(FALSE);
#endif
	//#NT#2016/08/05#Niven Cho -end

#if defined(_CPU2_ECOS_) && defined(_NETWORK_ON_CPU2_)

	UINet_HFSUnInit();
	//#NT#2018/03/21#Niven Cho -begin
	//#NT#RTSP_PUSH_MODE
	UINet_RtspUnInit();
    //#NT#2018/03/21#Niven Cho -end

	if (mode == NET_AP_MODE) { //ap mode
		ret = Dhcp_Server_Stop(TRUE);
	} else {
		ret = Dhcp_Client_Stop();
	}

#endif

	return ret;
}
void UINet_RemoveUser(void)
{
}
void UINet_AddACLTable(void)
{
}
void UINet_ClearACLTable(void)
{
}
void UINet_SiteSurvey(char *ifname, NVT_SS_STATUS_Tp pStatus)
{
}
void UINet_DumpAllSSID(NVT_SS_STATUS_Tp pStatus)
{
}
UINT32 UINet_DHCP_Start(UINT32 isClient)
{
	return 0;
}
void UINet_P2PDump(NVT_SS_STATUS_Tp pStatus)
{
}
void UINet_P2PScan(char *ifname, NVT_SS_STATUS_Tp pStatus)
{
}
UINT32 UINet_P2PConnect(NVT_SS_STATUS_Tp pStatus, UINT32 idx)
{
	return 0;
}

void UINet_WifiBack2Dev(void)
{
}
void UINet_WifiP2PReInit(void)
{
}
#endif //#if (_NVT_SDIO_WIFI_ != _NVT_SDIO_WIFI_NONE_ || _NVT_USB_WIFI_ != _NVT_USB_WIFI_NONE_)
#if (IOT_P2P_FUNC == ENABLE)

ER UINet_IotP2P_InitWifiCmdInterface(void)
{

	WifiAppCmd_init();
	WifiCmd_ReceiveCmd(1);
	return E_OK;
}
#define P2PIPC_LAST_DATA '1'
#define P2PIPC_CONTINUE_DATA '2'
#define P2PIPC_WAIT_DATA_GET '0'
ER UINet_IotP2P_SendWifiCmd(void)
{
	////this function is used for sending request to wificmd
//    UINT check_timeout=0;


	char *recv_buf = NULL;
	////get recv share memory with linux
	recv_buf = (char *)P2pIpc_getRcvAddr();
	if (recv_buf == NULL) {
		DBG_ERR("P2pIpc_getRcvAddrerror buf=NULL\r\n");
		return E_PAR;
	}

	if (strlen(recv_buf) <= 0 || strlen(recv_buf) > P2pIpc_RecvMemSize()) {
		DBG_ERR("buf len error len=%d\r\n", strlen(recv_buf));
	}

	char *send_buf = NULL;
	////get send share memory with linux
	send_buf = (char *)P2pIpc_getSndAddr();
	if (send_buf == NULL) {
		DBG_ERR("P2pIpc_getSndAddr error send_buf==NULL\r\n");
		return E_PAR;
	}
	send_buf[0] = '0';
	send_buf[1] = '\0';
	UINT32 len = 0;

	DBG_DUMP("recvbuf=%p  sendbuf=%p  recvlen=%d sendlen=%d\r\n", recv_buf, send_buf, P2pIpc_RecvMemSize(), P2pIpc_SendMemSize());

	char mimeType[32] = "text/xml";
	static int index = 0;
	int ret = 0;
	char path[256] = {0};
	do {

		len = P2pIpc_SendMemSize() - 1;// the first byte is continue flag
		memset(send_buf, '\0', len + 1);
		ret = WifiCmd_GetData(path, recv_buf, (UINT32)send_buf + 1, (UINT32 *)&len, mimeType, index);
		len =  len + 1;//add first byte
		if (ret == 0) { ///not continue data
			index = 0;
			memset(send_buf, P2PIPC_LAST_DATA, 1); ////set continue flag: 1 means last data
		} else if (ret == 1) { /// continue data
			index ++;
			len = P2pIpc_SendMemSize() - 1;
			memset(send_buf, P2PIPC_CONTINUE_DATA, 1); ////set continue flag:2 means continue data
		} else {
			index = 0;
			DBG_ERR("WifiCmd_GetDataerror ret=%d\r\n", ret);
		}

		P2pIpc_IOCTL();


	} while (index != 0);

	return E_OK;
}

INT32 UINet_IotP2P_CB(NVTP2P_CBID msg)
{
	int ret = 0;
	switch (msg) {
	case NVT_P2P_CB_SERVER_CONNECTED :
		DBG_DUMP("IotP2P_CB_SERVER_CONNECTED\r\n");
		break;
	case NVT_P2P_CB_SERVER_DISCONNECTED:
		DBG_DUMP("IotP2P_CB_SERVER_DISCONNECTED\r\n");
		break;
	case NVT_P2P_CB_SERVER_ERROR:
		DBG_DUMP("IotP2P_CB_SERVER_ERROR\r\n");
		break;
	case NVT_P2P_CB_CLIENT_CONNECTED:
		DBG_DUMP("IotP2P_CB_CLIENT_CONNECTED\r\n");
		break;
	case NVT_P2P_CB_CLIENT_DISCONNECTED:
		DBG_DUMP("IotP2P_CB_CLIENT_DISCONNECTED\r\n");
		break;
	case NVT_P2P_CB_CLIENT_ERROR:
		DBG_DUMP("IotP2P_CB_CLIENT_ERROR\r\n");
		break;
	case NVT_P2P_CB_DATA_ERROR:
		DBG_DUMP("IotP2P_CB_DATA_ERROR\r\n");
		break;
	case NVT_P2P_CB_NOTIFY_ERROR:
		DBG_DUMP("IotP2P_CB_NOTIFY_ERROR\r\n");
		break;
	case NVT_P2P_CB_VENDOR_SPECIFIC:
		DBG_DUMP("IotP2P_CB_VENDOR_SPECIFIC\r\n");
		ret = UINet_IotP2P_SendWifiCmd();
		break;
	default :
		DBG_ERR("IotP2P Unknown event(%d)", msg);
		break;
	};
	return ret;
}

INT32 UINet_IotP2PInit(void)
{
	ER r = P2pIpc_Open(OS_GetMempoolAddr(POOL_ID_P2PIPC), POOL_SIZE_P2PIPC, UINet_IotP2P_CB);
	if (r != E_OK) {
		DBG_ERR("IotP2P Open fail! (%d)", r);
		return r;
	}
	r = P2pIpc_Server_Start();
	if (r != E_OK) {
		DBG_ERR("IotP2P Server Start fail! (%d)", r);
		return r;
	}
	//#NT#2016/06/3#Charlie Chang -begin
	//#NT# set wificmd interface
	r = UINet_IotP2P_InitWifiCmdInterface();
	if (r != E_OK) {
		DBG_ERR("UINet_ItP2P_InitCmdInterface fail! (%d)", r);
		return r;
	}

	//#NT#2016/06/3#Charlie Chang -end
	return E_OK;
}
#endif //IOT_P2P_FUNC

INT32 UINet_OnvifInit(void)
{
	// open uctrl app onvif
	UCtrlAppOnvif_Open();
	return E_OK;
}
INT32 UINet_OnvifUnInit(void)
{
	UCtrlAppOnvif_Close();
	return E_OK;
}
#if (IOT_P2P_FUNC == ENABLE)
INT32 UINet_IotP2PUnInit(void)
{
	ER r = P2pIpc_Server_Stop();
	if (r != E_OK) {
		DBG_ERR("IotP2P Server Stop fail! (%d)", r);
		return r;
	}
	r = P2pIpc_Close();
	if (r != E_OK) {
		DBG_ERR("IotP2P Close fail! (%d)", r);
		return r;
	}
	return E_OK;
}
#endif

//#NT#2018/03/21#Niven Cho -begin
//#NT#RTSP_PUSH_MODE
INT32 UINet_RtspInit(void)
{
#if defined(_NETWORK_ON_CPU1_)
	char cmd[] = "nvtrtspd_ecos open";
	runmaincmd(cmd);
#else
	char cmd[] = "nvtrtspd &";
	NVTIPC_SYS_MSG sysMsg;
	sysMsg.sysCmdID = NVTIPC_SYSCMD_SYSCALL_REQ;
	sysMsg.DataAddr = (UINT32)cmd;
	sysMsg.DataSize = strlen(cmd) + 1;
	if (NvtIPC_MsgSnd(NVTIPC_SYS_QUEUE_ID, NVTIPC_SENDTO_CORE2, &sysMsg, sizeof(sysMsg)) < 0) {
		DBG_ERR("NvtIPC_MsgSnd\r\n");
	}
#endif
	return E_OK;
}

INT32 UINet_RtspUnInit(void)
{
#if defined(_NETWORK_ON_CPU1_)
	char cmd[] = "nvtrtspd_ecos close";
	runmaincmd(cmd);
#else
	char cmd[] = "nvtrtspd quit &";
	NVTIPC_SYS_MSG sysMsg;
	sysMsg.sysCmdID = NVTIPC_SYSCMD_SYSCALL_REQ;
	sysMsg.DataAddr = (UINT32)cmd;
	sysMsg.DataSize = strlen(cmd) + 1;
	if (NvtIPC_MsgSnd(NVTIPC_SYS_QUEUE_ID, NVTIPC_SENDTO_CORE2, &sysMsg, sizeof(sysMsg)) < 0) {
		DBG_ERR("NvtIPC_MsgSnd\r\n");
	}
#endif
	return E_OK;
}
//#NT#2018/03/21#Niven Cho -end

#if (WIFI_FTP_FUNC==ENABLE)
////////////////////////////////////FTP server ///////////////////////////////////

#include <network.h>
#include <net/ftpd.h>

#define STACK_SIZE (32*1024 + 0x1000)
static char stackFtpd[STACK_SIZE];
static cyg_thread thread_dataFtpd;
static cyg_handle_t thread_handleFtpd;

static ftp_server_t gFtpServer;

static int ftpCheckPasswd(const char *username, const char *passwd)
{
	if (strncmp(username, "ftpuser", strlen("ftpuser")) == 0) {
		if (strncmp(passwd, "12345", strlen("12345")) == 0) {
			// return 0 indicates user accepted
			return 0;
		}
	} else if (strncmp(username, "nvtuser", strlen("nvtuser")) == 0) {
		if (strncmp(passwd, "nvt123", strlen("nvt123")) == 0) {
			// return 0 indicates user accepted
			return 0;
		}
	}

	// return 1 indicates user rejected
	return 1;
}


static void ftpd_thread(cyg_addrword_t p)
{
	int sts;

	memset(&gFtpServer, 0, sizeof(gFtpServer));
	gFtpServer.allow_guest = 0;     // allow anonymous login
	gFtpServer.check_pwd = ftpCheckPasswd;
	gFtpServer.max_nr_of_clients = 4;
	strcpy(gFtpServer.chrootdir, MOUNT_FS_ROOT);
	sts = ftpd_server(&gFtpServer);
	if (sts != 0) {
		DBG_IND("%s: result is %d\r\n", __func__, sts);
	}
	DBG_IND("ftpd ended\r\n");

	cyg_thread_exit();
}

void start_ftpserver(void)
{
	// assume a file system is already mounted on /sdcard/ftp_home

	/* Create a main thread, so we can run the scheduler and have time 'pass' */
	cyg_thread_create(10,                // Priority - just a number
					  ftpd_thread,          // entry
					  0,                 // entry parameter
					  "FTP test",        // Name
					  &stackFtpd[0],         // Stack
					  STACK_SIZE,        // Size
					  &thread_handleFtpd,    // Handle
					  &thread_dataFtpd       // Thread data structure
					 );
	cyg_thread_resume(thread_handleFtpd);  /* Start it */
}


static ftp_server_t gFtpServer;
static cyg_handle_t thread_handleFtpd;

void stop_ftpserver(void)
{
	// assume gFtpServer and thread_handleFtpd are those used to start ftp server

	ftpd_server_stop(&gFtpServer);
	cyg_thread_delete(thread_handleFtpd);
}
#endif //WIFI_FTP_FUNC

#endif

#if !defined(_NVT_SDIO_WIFI_NONE_) || !defined(_NVT_USB_WIFI_NONE_)
#if (IPCAM_FUNC != ENABLE)
BOOL Cmd_wifi_on(CHAR *strCmd)
{
	DrvWiFi_PowerOn();
	DBG_DUMP("wifi module is powered on\r\n");

	return TRUE;
}

BOOL Cmd_wifi_open(CHAR *strCmd)
{
	UINet_AppIpc_Init();
	UINet_WifiInit(NET_AP_MODE, NET_AUTH_WPA2);
	return TRUE;
}

BOOL Cmd_wifi_close(CHAR *strCmd)
{
	UINet_WifiUnInit(NET_AP_MODE);
	UINet_AppIpc_UnInit();
	return TRUE;
}

NVT_SS_STATUS_T UINet_ss_status; //site survey result
BOOL Cmd_wifi_sitesurvey(CHAR *strCmd)
{

	UINet_SiteSurvey("wlan0", &UINet_ss_status);
	UINet_DumpAllSSID(&UINet_ss_status);

	return TRUE;

}

BOOL Cmd_wifi_p2pscan(CHAR *strCmd)
{
	UINet_P2PScan("wlan0", &UINet_ss_status);
	UINet_P2PDump(&UINet_ss_status);
	return TRUE;

}
BOOL Cmd_wifi_p2pconnect(CHAR *strCmd)
{
	UINT32 idx = 0;

	sscanf_s(strCmd, "%d", &idx);
	UINet_P2PConnect(&UINet_ss_status, idx);
	return TRUE;

}

BOOL Cmd_wifi_disconnect(CHAR *strCmd)
{
	UINT32 discon = 0;

	sscanf_s(strCmd, "%d", &discon);

	DBGD(discon);

	if (discon) {
		Ux_SendEvent(0, NVTEVT_EXE_WIFI_STOP, 0);
	} else {
		Ux_SendEvent(0, NVTEVT_EXE_WIFI_START, 0);
	}
	return TRUE;

}
//need modify SetupExe_OnWifiStart
//UI_SetData(FL_NetWorkMode,NET_AP_MODE);
//UINet_SetAuthType(NET_AUTH_NONE);
BOOL Cmd_wifi_mode(CHAR *strCmd)
{
	UINT32 mode = 0;
	UINT32 security = 0;

	sscanf_s(strCmd, "%d %d", &mode, &security);

	DBG_ERR("mode %d,security %d\r\n", mode, security);

	//UI_SetData(FL_NetWorkMode,mode);
	UINet_SetMode(mode);
	UINet_SetAuthType(security);

	return TRUE;

}

BOOL Cmd_wifi_ssid(CHAR *strCmd)
{
	char szSSID[NVT_WSC_MAX_SSID_LEN] = {0};

	sscanf_s(strCmd, "%s", szSSID, sizeof(szSSID));
	UINet_SetSSID(szSSID, strlen(szSSID));

	return TRUE;
}

BOOL Cmd_wifi_passphrase(CHAR *strCmd)
{
	char szpassphrase[NVT_MAX_WEP_KEY_LEN] = {0};

	sscanf_s(strCmd, "%s", szpassphrase, sizeof(szpassphrase));
	UINet_SetPASSPHRASE(szpassphrase, strlen(szpassphrase));

	return TRUE;
}

#if _TODO
BOOL Cmd_wifi_apptest(CHAR *strCmd)
{
	UINT32 cmd = 0;
	UINT32 par = 0;

	sscanf_s(strCmd, "%d %d", &cmd, &par);

	DBG_DUMP("cmd %d,par %d\r\n", cmd, par);
	WifiApp_SendAutoTestCmd(cmd, par);
	return TRUE;
}
#endif
#endif

SXCMD_BEGIN(wifi, "wifi command")
#if (IPCAM_FUNC != ENABLE)
SXCMD_ITEM("on", Cmd_wifi_on, "wifi power on")
SXCMD_ITEM("open", Cmd_wifi_open, "run wifi as AP")
SXCMD_ITEM("close", Cmd_wifi_close, "shutdonw wifi AP")
SXCMD_ITEM("site", Cmd_wifi_sitesurvey, "wifi site survey")
SXCMD_ITEM("p2pscan", Cmd_wifi_p2pscan, "p2p scan")
SXCMD_ITEM("p2pcon %d", Cmd_wifi_p2pconnect, "p2p connect")
SXCMD_ITEM("discon %d", Cmd_wifi_disconnect, "1:disconnect 0:connect")
SXCMD_ITEM("mode %d", Cmd_wifi_mode, "wifi mode 0:ap 1:station 2:p2pdev")
SXCMD_ITEM("ssid %", Cmd_wifi_ssid, "wifi set SSID")
SXCMD_ITEM("pass %", Cmd_wifi_passphrase, "wifi set PASSPHRASE")
#if _TODO
SXCMD_ITEM("apptest %", Cmd_wifi_apptest, "wifi app auto tset")
#endif
#endif
SXCMD_END()
#endif
