//global debug level: PRJ_DBG_LVL
#include "PrjCfg.h"
#include "SxCmd.h"

#if(WIFI_FUNC==ENABLE)
#include "WifiAppCmd.h"
#endif
#if(WIFI_AP_FUNC==ENABLE)
#include "UIAppNetwork.h"
#include "DhcpNvt.h"
#include "DxWiFi.h"
#endif
#if(HFS_FUNC==ENABLE)
#include "UIAppNetwork.h"
#include "HfsNvtAPI.h"
#endif

//local debug level: THIS_DBGLVL
#define THIS_DBGLVL         2 // 0=FATAL, 1=ERR, 2=WRN, 3=UNIT, 4=FUNC, 5=IND, 6=MSG, 7=VALUE, 8=USER
///////////////////////////////////////////////////////////////////////////////
#define __MODULE__          UiAppNetCmd
#define __DBGLVL__          ((THIS_DBGLVL>=PRJ_DBG_LVL)?THIS_DBGLVL:PRJ_DBG_LVL)
#define __DBGFLT__          "*" //*=All, [mark]=CustomClass
#include "DebugModule.h"
///////////////////////////////////////////////////////////////////////////////

#if _TODO
NVT_SS_STATUS_T UINet_ss_status; //site survey result
static BOOL Cmd_wifi_sitesurvey(CHAR *strCmd)
{

	UINet_SiteSurvey("wlan0", &UINet_ss_status);
	UINet_DumpAllSSID(&UINet_ss_status);

	return TRUE;

}
static BOOL Cmd_wifi_p2pscan(CHAR *strCmd)
{
	UINet_P2PScan("wlan0", &UINet_ss_status);
	UINet_P2PDump(&UINet_ss_status);
	return TRUE;

}
static BOOL Cmd_wifi_p2pconnect(CHAR *strCmd)
{
	UINT32 idx = 0;

	sscanf_s(strCmd, "%d", &idx);
	UINet_P2PConnect(&UINet_ss_status, idx);
	return TRUE;

}

static BOOL Cmd_wifi_disconnect(CHAR *strCmd)
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
static BOOL Cmd_wifi_mode(CHAR *strCmd)
{
	UINT32 mode = 0;
	UINT32 security = 0;

	sscanf_s(strCmd, "%d %d", &mode, &security);

	DBG_ERR("mode %d,security %d\r\n", mode, security);

	UI_SetData(FL_NetWorkMode, mode);
	UINet_SetMode(mode);
	UINet_SetAuthType(security);

	return TRUE;

}

static BOOL Cmd_wifi_ssid(CHAR *strCmd)
{
	char szSSID[NVT_WSC_MAX_SSID_LEN] = {0};

	sscanf_s(strCmd, "%s", szSSID, sizeof(szSSID));
	UINet_SetSSID(szSSID, strlen(szSSID));

	return TRUE;
}

static BOOL Cmd_wifi_passphrase(CHAR *strCmd)
{
	char szpassphrase[NVT_MAX_WEP_KEY_LEN] = {0};

	sscanf_s(strCmd, "%s", szpassphrase, sizeof(szpassphrase));
	UINet_SetPASSPHRASE(szpassphrase, strlen(szpassphrase));

	return TRUE;
}


static BOOL Cmd_wifi_ping(CHAR *strCmd)
{
	int packSize = 0;
	char tmpIP[NVT_WIFIIPC_IP_MAX_LEN] = {0};
	unsigned int cnt = 0;

	sscanf_s(strCmd, "%s %d %u", tmpIP, sizeof(tmpIP), &packSize, &cnt);
	DBG_IND(":: %s => ip=%s,pckSize=%d,cnt=%d\r\n", strCmd, tmpIP, packSize, cnt);
#if _TODO
	Dhcp_Net_Ping(tmpIP, packSize, cnt);
#endif
	return TRUE;
}

static BOOL Cmd_wifi_apptest(CHAR *strCmd)
{
	UINT32 cmd = 0;
	UINT32 par = 0;

	sscanf_s(strCmd, "%d %d", &cmd, &par);

	DBG_DUMP("cmd %d,par %d\r\n", cmd, par);
#if _TODO
	WifiApp_SendAutoTestCmd(cmd, par);
#endif
	return TRUE;
}

static BOOL Cmd_wifi_on(CHAR *strCmd)
{
#if _TODO
	DrvWiFi_PowerOn();
#endif
	DBG_DUMP("wifi module is powered on\r\n");

	return TRUE;
}

SXCMD_BEGIN(wifi, "wifi command")
SXCMD_ITEM("site", Cmd_wifi_sitesurvey, "wifi site survey")
SXCMD_ITEM("p2pscan", Cmd_wifi_p2pscan, "p2p scan")
SXCMD_ITEM("p2pcon %d", Cmd_wifi_p2pconnect, "p2p connect")
SXCMD_ITEM("discon %d", Cmd_wifi_disconnect, "1:disconnect 0:connect")
SXCMD_ITEM("mode %d", Cmd_wifi_mode, "wifi mode 0:ap 1:station 2:p2pdev")
SXCMD_ITEM("ssid %", Cmd_wifi_ssid, "wifi set SSID")
SXCMD_ITEM("pass %", Cmd_wifi_passphrase, "wifi set PASSPHRASE")
SXCMD_ITEM("ping %", Cmd_wifi_ping, "wifi ping")
SXCMD_ITEM("apptest %", Cmd_wifi_apptest, "wifi app auto test")
SXCMD_ITEM("on", Cmd_wifi_on, "wifi power on")
SXCMD_END()
#endif


#if(HFS_FUNC==ENABLE)
static BOOL cmd_hfs_open(CHAR *strCmd)
{
	UINet_HFSInit();
	exam_msg("hfs opened\r\n");
	return TRUE;
}

static BOOL cmd_hfs_close(CHAR *strCmd)
{
	UINet_HFSUnInit();
	exam_msg("hfs closed\r\n");
	return TRUE;
}

SXCMD_BEGIN(hfs, "hfs command")
SXCMD_ITEM("open %",  cmd_hfs_open,  "hfs open")
SXCMD_ITEM("close %", cmd_hfs_close, "hfs close")
SXCMD_END()
#endif




#if _TODO
//#NT#2016/05/20#Lincy Lin -begin
//#NT#Support NTP client
#include "rtc.h"
#include "NvtIpcAPI.h"
//#NT#2016/05/20#Lincy Lin -end

//#NT#2016/05/20#Lincy Lin -begin
//#NT#Support NTP client
static BOOL Cmd_sntp_open(CHAR *strCmd)
{
	INT32            ret;
	NVTIPC_SYS_MSG   sysMsg;
	char             cmd[128];
	char             ntp_server[64] = "192.168.0.4";

	sscanf_s(strCmd, "%s", &ntp_server, sizeof(ntp_server));
	snprintf(cmd, sizeof(cmd), "sntp -open %s", ntp_server);
	sysMsg.sysCmdID = NVTIPC_SYSCMD_SYSCALL_REQ;
	sysMsg.DataAddr = (UINT32)&cmd[0];
	sysMsg.DataSize = strlen(cmd) + 1;
	if ((ret = NvtIPC_MsgSnd(NVTIPC_SYS_QUEUE_ID, NVTIPC_SENDTO_CORE2, &sysMsg, sizeof(sysMsg))) < 0) {
		DBG_ERR("NvtIPC_MsgSnd %d\r\n", ret);
		return -1;
	}
	return TRUE;
}

static BOOL Cmd_sntp_close(CHAR *strCmd)
{
	INT32            ret;
	NVTIPC_SYS_MSG   sysMsg;
	char             *cmd = "sntp -close";

	sysMsg.sysCmdID = NVTIPC_SYSCMD_SYSCALL_REQ;
	sysMsg.DataAddr = (UINT32)&cmd[0];
	sysMsg.DataSize = strlen(cmd) + 1;
	if ((ret = NvtIPC_MsgSnd(NVTIPC_SYS_QUEUE_ID, NVTIPC_SENDTO_CORE2, &sysMsg, sizeof(sysMsg))) < 0) {
		DBG_ERR("NvtIPC_MsgSnd %d\r\n", ret);
		return -1;
	}
	return TRUE;
}

static BOOL Cmd_sntp_settime(CHAR *strCmd)
{
	UINT32     ntptime = 0;

	DBG_DUMP("\r\nstrCmd =%s\r\n", strCmd);

	sscanf(strCmd, "%d", &ntptime);
	DBG_DUMP("ntptime =%d\r\n", ntptime);
	return TRUE;
}


SXCMD_BEGIN(sntp, "sntp command")
SXCMD_ITEM("open %", Cmd_sntp_open, "sntp open")
SXCMD_ITEM("close %", Cmd_sntp_close, "sntp close")
SXCMD_ITEM("settime %", Cmd_sntp_settime, "sntp settime callback")
SXCMD_END()
//#NT#2016/05/20#Lincy Lin -end

#endif
