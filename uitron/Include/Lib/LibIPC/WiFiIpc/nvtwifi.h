#ifndef CYGONCE_CYG_WIFI_NVT_H
#define CYGONCE_CYG_WIFI_NVT_H

//=============================================================================
//define NVTWIFI_SIM for simulation
//#define NVTWIFI_SIM

//-------------------------------------------------------------------------
//NvtWifiECOS API return value
#define NVTWIFI_RET_OK           0
#define NVTWIFI_RET_OPENED       1
#define NVTWIFI_RET_ERR          (-1)
#define NVTWIFI_RET_NO_FUNC      (-2)

#define NVTWIFI_TOKEN_PATH       "wifiipc"
#define NVTWIFI_MAX_PARAM                6
#define NVTWIFI_PARAM_LEN        64

#define NVTWIFI_INTERFACE_VER    17060900

//-------------------------------------------------------------------------
//IPC command id and parameters
typedef enum {
	NVTWIFI_CMDID_GET_VER_INFO  =   0x00000000,
	NVTWIFI_CMDID_GET_BUILD_DATE,
	NVTWIFI_CMDID_TEST,
	NVTWIFI_CMDID_INIT,
	NVTWIFI_CMDID_INTF_IS_UP,
	NVTWIFI_CMDID_INTF_UP,
	NVTWIFI_CMDID_INTF_DOWN,
	NVTWIFI_CMDID_INTF_CONFIG,
	NVTWIFI_CMDID_INTF_DELETE_ADDR,
	NVTWIFI_CMDID_CONFIG,
	NVTWIFI_CMDID_GET_MAC,
	NVTWIFI_CMDID_SET_MAC,
	NVTWIFI_CMDID_RUN_CMD,
	NVTWIFI_CMDID_REG_STACB,
	NVTWIFI_CMDID_REG_LINKCB,
	NVTWIFI_CMDID_REG_WSCCB,
	NVTWIFI_CMDID_REG_WSC_FLASHCB,
	NVTWIFI_CMDID_REG_P2PCB,
	NVTWIFI_CMDID_STA_CB,
	NVTWIFI_CMDID_LINK_CB,
	NVTWIFI_CMDID_WSCCB,
	NVTWIFI_CMDID_WSC_FLASHCB,
	NVTWIFI_CMDID_P2PCB,
	NVTWIFI_CMDID_SITE_REQ,
	NVTWIFI_CMDID_SITE_RET,
	NVTWIFI_CMDID_P2P_REQ,
	NVTWIFI_CMDID_P2P_RET,
	NVTWIFI_CMDID_P2P_PROVISION,
	NVTWIFI_CMDID_WSCD_CREATE,
	NVTWIFI_CMDID_WSCD_REINIT,
	NVTWIFI_CMDID_WSCD_STOP,
	NVTWIFI_CMDID_WSCD_STATUS,
	NVTWIFI_CMDID_GEN_PIN,
	NVTWIFI_CMDID_THREAD_INFO_BY_NAME,
	NVTWIFI_CMDID_IGNORE_DOWN_UP,
	NVTWIFI_CMDID_SYSREQ_ACK,
	NVTWIFI_CMDID_GET_CONNECT_INFO,
	NVTWIFI_CMDID_UNINIT,
	NVTWIFI_CMDID_INTF_REMOVE,
	NVTWIFI_CMDID_SET_COUNTRY,
	NVTWIFI_CMDID_SET_FREQUENCY,
} NVTWIFI_CMDID;

typedef struct {
	NVTWIFI_CMDID CmdId;
	int Arg;
} NVTWIFI_MSG;

//-------------------------------------------------------------------------
//NVTWIFI definition

typedef struct _NVTWIFI_OPEN_OBJ {
	unsigned int        RcvParamAddr;
	unsigned int        SndParamAddr;
} NVTWIFI_OPEN_OBJ;


#define WIFIIPC_BUF_CHK_SIZE    (4)
#define WIFIIPC_BUF_TAG         MAKEFOURCC('W','I','F','I')

#define NVT_MAX_BSS_DESC            (64)
#define NVT_WIFIIPC_MAC_ADDR_LEN    (6)
#define NVT_MESH_ID_LEN             32

#define NVT_WSC_MAX_SSID_LEN            (32)
#define NVT_MAX_WEP_KEY_LEN             (26)
#define NVT_MAX_NETWORK_KEY_LEN         (64)
#define NVT_PIN_LEN                      (8)
#define NVT_MAX_DEVICE_NAME_LEN         (32)

#define NVT_WPS_REINIT_EVENT            1
#define NVT_WPS_STATUS_CHANGE_EVENT     2

// for client mode
#define NVT_WIFI_LINK_STATUS_DISCONNECTED       0
#define NVT_WIFI_LINK_STATUS_CONNECTED          1
#define NVT_WIFI_LINK_STATUS_AUTH_FAIL          2
#define NVT_WIFI_LINK_STATUS_DISASSOCIATED      3
#define NVT_WIFI_LINK_STATUS_DEAUTHENTICATED    4
#define NVT_WIFI_LINK_STATUS_HANDSHAKE_TIMEOUT  5
#define NVT_WIFI_LINK_STATUS_SACN_ZREO_RESULT   6

// for AP mode
#define NVT_WIFI_STA_STATUS_ASSOCIATED      1
#define NVT_WIFI_STA_STATUS_REASSOCIATED    2
#define NVT_WIFI_STA_STATUS_DISASSOCIATED   3
#define NVT_WIFI_STA_STATUS_DEAUTHENTICATED 4
#define NVT_WIFI_STA_STATUS_PORT_AUTHORIZED 5
#define NVT_WIFI_STA_STATUS_AUTH_FAIL       6
#define NVT_WIFI_STA_STATUS_EXPIRED         7
#define NVT_WIFI_AP_READY                   8

// for p2p
#define NVT_WIFI_P2P_EVENT_BACK2DEV            1
#define NVT_WIFI_P2P_EVENT_START_DHCPD      2
#define NVT_WIFI_P2P_EVENT_START_DHCPC      3

typedef struct _nvt_wifi_settings {
	int wlan_disable;
	int wps_disable;
	char mode[16];
	char security[32];
	int channel;
	char ssid[NVT_WSC_MAX_SSID_LEN + 1];
	char passphrase[NVT_MAX_NETWORK_KEY_LEN + 1];
	char wep_key1[NVT_MAX_WEP_KEY_LEN + 1];
	char wep_key2[NVT_MAX_WEP_KEY_LEN + 1];
	char wep_key3[NVT_MAX_WEP_KEY_LEN + 1];
	char wep_key4[NVT_MAX_WEP_KEY_LEN + 1];
	int wep_default_key;
	char wsc_pin[NVT_PIN_LEN + 1];
	int is_configured;
	int config_by_ext_reg;
	int p2pmode;
	char device_name[NVT_MAX_DEVICE_NAME_LEN + 1];
	//YongChang//int is_dhcp; // static ip or dhcp for client mode
} nvt_wifi_settings;

typedef struct _nvt_wsc_flash_param {
	char SSID[NVT_WSC_MAX_SSID_LEN + 1];
	int WSC_AUTH;     /* WSC_AUTH_OPEN=1, WSC_AUTH_WPAPSK=2, WSC_AUTH_SHARED=4, WSC_AUTH_WPA=8, WSC_AUTH_WPA2=0x10, WSC_AUTH_WPA2PSK=0x20, WSC_AUTH_WPA2PSKMIXED=0x22 */
	char WSC_PSK[NVT_MAX_NETWORK_KEY_LEN + 1];
	int WEP; /* WEP_DISABLED=0, WEP64=1, WEP128=2*/
	char WEP_KEY1[NVT_MAX_WEP_KEY_LEN + 1];
	char WEP_KEY2[NVT_MAX_WEP_KEY_LEN + 1];
	char WEP_KEY3[NVT_MAX_WEP_KEY_LEN + 1];
	char WEP_KEY4[NVT_MAX_WEP_KEY_LEN + 1];
	int WEP_DEFAULT_KEY;
	int WSC_ENC; /* WSC_ENCRYPT_NONE=1, WSC_ENCRYPT_WEP=2, WSC_ENCRYPT_TKIP=4, WSC_ENCRYPT_AES=8, WSC_ENCRYPT_TKIPAES=12 */
	int WSC_CONFIGBYEXTREG; /* CONFIG_BY_INTERNAL_REGISTRAR=1, CONFIG_BY_EXTERNAL_REGISTRAR=2, MANUAL_SETTING_TO_ENROLLEE=3 */
	int WLAN0_P2P_TYPE;
} nvt_wsc_flash_param;

struct nvt_bss_desc {
	unsigned char   bssid[NVT_WIFIIPC_MAC_ADDR_LEN];
	unsigned char   ssid[32];
	unsigned char   *ssidptr;           // unused, for backward compatible
	unsigned short  ssidlen;
	unsigned char   meshid[NVT_MESH_ID_LEN];
	unsigned char   *meshidptr;         // unused, for backward compatible
	unsigned short  meshidlen;
	unsigned int    bsstype;
	unsigned short  beacon_prd;
	unsigned char   dtim_prd;
	unsigned int    t_stamp[2];
	unsigned int    ibss_par;
	unsigned short  capability;
	unsigned char   channel;
	unsigned int    basicrate;
	unsigned int    supportrate;
	unsigned char   bdsa[NVT_WIFIIPC_MAC_ADDR_LEN];
	unsigned char   rssi;
	unsigned char   sq;
	unsigned char   network;
	/*add for P2P_SUPPORT ; for sync; it exist no matter p2p enabled or not*/
	unsigned char   p2pdevname[33];
	unsigned char   p2prole;
	unsigned short  p2pwscconfig;
	unsigned char   p2paddress[NVT_WIFIIPC_MAC_ADDR_LEN];
};

typedef struct _nvt_sitesurvey_status {
	unsigned char number;
	unsigned char pad[3];
	//BssDscr bssdb[MAX_BSS_DESC];
	struct nvt_bss_desc bssdb[NVT_MAX_BSS_DESC];
} NVT_SS_STATUS_T, *NVT_SS_STATUS_Tp;

/* Any changed here MUST sync with 8192cd_p2p.h */
typedef struct _nvt_p2p_provision_comm {
	unsigned char dev_address[6];
	unsigned short wsc_config_method;
	unsigned char channel;
} NVT_P2P_PROVISION_COMM_T, *NVT_P2P_PROVISION_COMM_Tp;

enum NVT_NETWORK_TYPE {
	NVT_WIRELESS_11B = 1,
	NVT_WIRELESS_11G = 2,
	NVT_WIRELESS_11A = 4,
	NVT_WIRELESS_11N = 8,
	NVT_WIRELESS_11AC = 64
};

typedef enum _NVT_FREQUENCY_TYPE {
	NVT_WIFI_24G = 0,
	NVT_WIFI_5G  = 1,
	ENUM_DUMMY4WORD (NVT_FREQUENCY_TYPE)
} NVT_FREQUENCY_TYPE;

typedef enum _NVT_Capability {
	nvt_cESS        = 0x01,
	nvt_cIBSS       = 0x02,
	nvt_cPollable       = 0x04,
	nvt_cPollReq        = 0x01,
	nvt_cPrivacy        = 0x10,
	nvt_cShortPreamble  = 0x20,
} NVT_Capability;

typedef enum _NVT_Connect_Info {
	nvt_ci_rssi        = 0x01,
} NVT_Connect_Info;

///////////////////////parameter struct ///////////////
typedef struct {
	int param1;
	int param2;
	int param3;
} NVTWIFI_PARAM_VALUE;

typedef struct {
	char pIntfName[NVTWIFI_PARAM_LEN];
} NVTWIFI_PARAM_INTF;

typedef struct {
	char pIntfName[NVTWIFI_PARAM_LEN];
	int  status;
} NVTWIFI_PARAM_STATUS;

typedef struct {
	char pIntfName[NVTWIFI_PARAM_LEN];
	char string[NVTWIFI_PARAM_LEN];
	int  status;
} NVTWIFI_PARAM_STATUS2;

typedef struct {
	char string1[NVTWIFI_PARAM_LEN];
	char string2[NVTWIFI_PARAM_LEN];
	char string3[NVTWIFI_PARAM_LEN];
} NVTWIFI_PARAM_STRING;

typedef struct {
	char param[NVTWIFI_MAX_PARAM][NVTWIFI_PARAM_LEN];
} NVTWIFI_PARAM_RUN_CMD;

typedef struct {
	char pIntfName[NVTWIFI_PARAM_LEN];
	NVT_SS_STATUS_T  status;
} NVTWIFI_PARAM_SITE_STATUS;

typedef struct {
	char pIntfName[NVTWIFI_PARAM_LEN];
	NVT_P2P_PROVISION_COMM_T  status;
} NVTWIFI_PARAM_PROVISION_STATUS;

typedef struct {
	char pIntfName[NVTWIFI_PARAM_LEN];
	char pStaMac[NVTWIFI_PARAM_LEN];
	NVT_Connect_Info info;
	unsigned char data[NVTWIFI_PARAM_LEN];
} NVTWIFI_PARAM_CONNECT_INFO;

#ifndef __externC
#ifdef __cplusplus
# define __externC extern "C"
#else
# define __externC extern
#endif
#endif

__externC void NvtWifiECOS_CmdLine(char *szCmd);

#endif // CYGONCE_CYG_WIFI_NVT_H
// End of nvtwifi.h
