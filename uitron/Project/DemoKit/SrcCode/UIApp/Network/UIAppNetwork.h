#ifndef _UIAPPNET_H_
#define _UIAPPNET_H_

#include "WiFiIpcAPI.h"

typedef enum {
	NET_AP_MODE = 0,
	NET_STATION_MODE,
	NET_P2P_DEV_MODE,
	NET_P2P_GO_MODE,
	NET_WPS_AP_PBC_MODE,
	NET_MODE_SETTING_MAX
} NET_MODE_SETTING;


typedef enum {
	NET_AUTH_NONE = 0,
	NET_AUTH_WPA2,
	NET_AUTH_WPA,       //
	NET_AUTH_SETTING_MAX
} NET_AUTH_SETTING;

typedef enum {
	NET_STATE_WIFI_START,
	NET_STATE_WIFI_CONNECTING,
	NET_STATE_WIFI_CB,
	NET_STATE_DHCP_START,
	NET_STATE_MAX,
} NET_STATE;
#define NET_CONNECTING_TIMEOUT    (-1)
#define SEND_PEER_IP               1
extern void UINet_WifiBack2Dev(void);
extern void UINet_WifiP2PReInit(void);
extern BOOL UINet_CPU2_Init(void);
extern BOOL UINet_AppIpc_Init(void);
extern BOOL UINet_AppIpc_UnInit(void);
extern INT32 UINet_WifiInit(UINT32 mode, UINT32 security);
extern INT32 UINet_LanInit(UINT32 mode);
extern INT32 UINet_HFSInit(void);
extern INT32 UINet_OnvifInit(void);
extern INT32 UINet_WifiUnInit(UINT32 mode);
extern INT32 UINet_LanUnInit(UINT32 mode);
extern INT32 UINet_HFSUnInit(void);
extern INT32 UINet_OnvifUnInit(void);
extern char *UINet_GetSSID(void);
extern char *UINet_GetDefaultAPSSID(void);
extern char *UINet_GetPASSPHRASE(void);
extern char *UINet_GetDefaultAPPASSPHRASE(void);
extern char *UINet_GetIP(void);
extern char *UINet_GetMAC(void);
extern char *UINet_GetConnectedIPMAC(void);
extern char *UINet_GetConnectedMAC(void);
extern void UINet_SetMode(NET_MODE_SETTING mode);
extern UINT32 UINet_GetMode(void);
extern void UINet_SetAuthType(NET_AUTH_SETTING authtype);
extern UINT32 UINet_GetAuthType(void);
extern void UINet_SetSSID(char *ssid, UINT32 len);
extern void UINet_SetPASSPHRASE(char *pwd, UINT32 len);
extern void UINet_SetChannel(UINT32 channel);
extern UINT32 UINet_GetChannel(void);
extern void UINet_RemoveUser(void);
extern void UINet_AddACLTable(void);
extern void UINet_ClearACLTable(void);
extern void UINet_SiteSurvey(char *ifname, NVT_SS_STATUS_Tp pStatus);
extern void UINet_DumpAllSSID(NVT_SS_STATUS_Tp pStatus);
extern UINT32 UINet_DHCP_Start(UINT32 isClient);
extern void UINet_P2PDump(NVT_SS_STATUS_Tp pStatus);
extern void UINet_P2PScan(char *ifname, NVT_SS_STATUS_Tp pStatus);
extern UINT32 UINet_P2PConnect(NVT_SS_STATUS_Tp pStatus, UINT32 idx);
extern BOOL UINet_CliReConnect(UINT32 bStart);
extern void UINet_Reg_Timer(void);
extern void UINet_HFSPortSet(INT32 http_port, INT32 https_port);
extern void UINet_DisableHFS(BOOL enable);
extern INT32 UINet_IotP2PInit(void);
extern INT32 UINet_IotP2PUnInit(void);
extern void UINet_SetSendPeerIP(UINT32 val);
//#NT#2018/03/21#Niven Cho -begin
//#NT#RTSP_PUSH_MODE
extern INT32 UINet_RtspInit(void);
extern INT32 UINet_RtspUnInit(void);
//#NT#2018/03/21#Niven Cho -end

#endif //_UIAPPNET_H_
