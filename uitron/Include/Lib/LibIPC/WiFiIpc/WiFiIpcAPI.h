/*
    Copyright (c) 2014  Novatek Microelectronics Corporation

    @file WiFiIpc.h

    @version

    @date
*/

#ifndef _WIFIIPCAPI_H
#define _WIFIIPCAPI_H

#include "Type.h"
#include "nvtwifi.h"

#define NVT_WIFIIPC_IP_MAX_LEN          (16)  //refer to NET_IP_MAX_LEN

#define NULL_FILE               0
#define NULL_STR                ""
#define WIFI_IFNAME             "wlan0"   //interface name

#define WIFIIPC_RET_OK              0
#define WIFIIPC_RET_ERR           (-1)

enum nvt_p2p_role_more {
	P2P_DEVICE = 1,
	P2P_PRE_CLIENT = 2,
	P2P_CLIENT = 3,
	P2P_PRE_GO = 4,  // after GO nego , we are GO and proceed WSC exchange
	P2P_TMP_GO = 5   // after GO nego , we are GO and proceed WSC exchange is done
};

enum {  WSC_AUTH_OPEN = 1,
		WSC_AUTH_WPAPSK = 2,
		WSC_AUTH_SHARED = 4,
		WSC_AUTH_WPA = 8,
		WSC_AUTH_WPA2 = 0x10,
		WSC_AUTH_WPA2PSK = 0x20,
		WSC_AUTH_WPA2PSKMIXED = 0x22
	 };

enum { WSC_ENCRYPT_NONE = 1, WSC_ENCRYPT_WEP = 2, WSC_ENCRYPT_TKIP = 4, WSC_ENCRYPT_AES = 8, WSC_ENCRYPT_TKIPAES = 12 };

enum {  NOT_USED = -1,
		PROTOCOL_START = 0, PROTOCOL_PBC_OVERLAPPING = 1,
		PROTOCOL_TIMEOUT = 2, PROTOCOL_SUCCESS = 3,

		SEND_EAPOL_START = 4, RECV_EAPOL_START = 5, SEND_EAP_IDREQ = 6, RECV_EAP_IDRSP = 7,
		SEND_EAP_START = 8, SEND_M1 = 9, RECV_M1 = 10, SEND_M2 = 11,

		RECV_M2 = 12, RECV_M2D = 13, SEND_M3 = 14, RECV_M3 = 15,
		SEND_M4 = 16, RECV_M4 = 17, SEND_M5 = 18, RECV_M5 = 19,

		SEND_M6 = 20, RECV_M6 = 21, SEND_M7 = 22, RECV_M7 = 23,
		SEND_M8 = 24, RECV_M8 = 25, PROC_EAP_ACK = 26, WSC_EAP_FAIL = 27,

		HASH_FAIL = 28, HMAC_FAIL = 29, PWD_AUTH_FAIL = 30, PROTOCOL_PIN_NUM_ERR = 31,
		PROC_EAP_DONE = 32, PROTOCOL_ERR = 33
	 };

enum {
	CONFIG_METHOD_ETH = 0x2,
	CONFIG_METHOD_PIN = 0x4,
	CONFIG_METHOD_DISPLAY = 0x8,
	CONFIG_METHOD_PBC = 0x80,
	CONFIG_METHOD_KEYPAD = 0x100,
	CONFIG_METHOD_VIRTUAL_PBC = 0x280,
	CONFIG_METHOD_PHYSICAL_PBC = 0x480,
	CONFIG_METHOD_VIRTUAL_PIN = 0x2008,
	CONFIG_METHOD_PHYSICAL_PIN = 0x4008
};

// callback function prototype for link status report
typedef void (wifi_link_status_cb_func_t)(char *pIntfName, int status);

// callback function prototype for station status report
typedef void (wifi_sta_status_cb_func_t)(char *pIntfName, char *pMacAddr, int status);

// callback function prototype for p2p
typedef void (p2p_event_indicate_cb_func_t)(char *pIntfName, int status);

typedef void (wsc_event_cb_func_t)(int evt, int status);
typedef int (wsc_flash_param_cb_func_t)(nvt_wsc_flash_param *);

typedef struct {
	UINT32                  sharedMemAddr;  // shared memory address
	UINT32                  sharedMemSize;  // shared memory size
} WIFIIPC_OPEN;

/**
    Install WiFiIpc flag and semaphore id.

    @return void.
*/
extern void WiFiIpc_InstallID(void) _SECTION(".kercfg_text");

/**
    Open WiFiIpc task.

    This is WiFiIpc initialization function.

    @return
        - @b E_PS_OK if success, others fail
*/
extern ER WiFiIpc_Open(WIFIIPC_OPEN *pOpen);

/**
    Close WiFiIpc task.

    @return
        - @b E_PS_OK if success, others fail
*/
extern ER   WiFiIpc_Close(void);

/**
    WiFiIpc Test API.

    @return
        - @b E_PS_OK if success, others fail
*/
extern ER   WiFiIpc_Test(int param1, int param2, int param3);

/**
    init wlan0 device.

    The API would call RTK wifi_init_wlan0_netdev();

    @param[in]      pri  use to assign wifi thread priority, 0 - default priority

    @return
        - @b 0 success, -1 fail
*/
extern ER WiFiIpc_init_wlan0_netdev(int pri);

/**
    Check interface if up.
    The API would call RTK is_interface_up();

    @param[in]      pIntfName  interface name

    @return
        - @b 1 is up, 0 is down.

*/
extern ER WiFiIpc_is_interface_up(const char *pIntfName);
/**
    Set WiFi interface up.
    The API would call RTK is_interface_up();

    @param[in]      pIntfName  interface name

    @return
        - @b 0 success, -1 fail
*/
extern ER WiFiIpc_interface_up(const char *pIntfName);

/**
    Set WiFi interface down.
    The API would call RTK is_interface_down();

    @param[in]      pIntfName  interface name

    @return
        - @b 0 success, -1 fail
*/
extern ER WiFiIpc_interface_down(const char *pIntfName);

/**
    Config WiFi IP and netmask.
    The API would call RTK interface_config();

    @param[in]      pIntfName  interface name
    @param[in]      IPaddr     IP address
    @param[in]      netmask    network net mask

    @return
        - @b 0 success, -1 fail
*/
extern ER WiFiIpc_interface_config(const char *pIntfName, char *IPaddr, char *netmask);

extern ER WiFiIpc_delete_interface_addr(const char *pIntfName);

/**
    Get WiFi wlan0 mac address.
    The API would call RTK get_wlan0_efuse_mac();

    @param[out]      pMacAddr    pointer of mac address

    @return
        - @b 0 success, -1 fail
*/
extern ER WiFiIpc_get_wlan0_efuse_mac(char *pMacAddr);

/**
    Set WiFi mac address.
    The API would call RTK set_mac_address();

    @param[in]     pIntfName      interface name
    @param[in]     pMacAddr    pointer of mac address

    @return
        - @b 0 success, -1 fail
*/
extern ER WiFiIpc_set_mac_address(const char *pIntfName, char *pMacAddr);

/**
    Set WiFi system command.
    The API would call RTK RunSystemCmd();

    @param[in]     filepath     variable number of parameter,start from NULL_FILE,end to NULL_STR
    Example:       WiFiIpc_RunSystemCmd(NULL_FILE, "iwpriv", WIFI_IFNAME, "set_mib", "band=11", NULL_STR);

    @return
        - @b 0 success, -1 fail
*/extern ER WiFiIpc_RunSystemCmd(char *filepath, ...);

/**
    Register callback function of station status

    Register callback function for link status report,when WiFi is AP mode.

    @param[in]      pIntfName  interface name
    @param[in]      pFunc      pointer of callback function

    @return
        - @b 1: register success, 0: register fail.
*/
extern ER WiFiIpc_register_wifi_sta_status_cb(const char *pIntfName, wifi_sta_status_cb_func_t *pFunc);

/**
    Register callback function of link to AP

    Register callback function for link status report,when WiFi is station mode.

    @param[in]      pIntfName  interface name
    @param[in]      pFunc      pointer of callback function

    @return
        - @b 1: register success, 0: register fail.
*/
extern ER WiFiIpc_register_wifi_link_status_cb(const char *pIntfName, wifi_link_status_cb_func_t *pFunc);


extern ER WiFiIpc_register_p2p_event_indicate_cb(const char *pIntfName, p2p_event_indicate_cb_func_t *pFunc);
extern ER WiFiIpc_register_wsc_event_cb(wsc_event_cb_func_t *pFunc);
extern ER WiFiIpc_register_wsc_flash_param_cb(wsc_flash_param_cb_func_t *pFunc);



/**
    Get wfii site survey request

    @param[in]      pwifi  WiFi setting,refer to struct nvt_wifi_settings

    @return
        - @b 0 success, -1 fail

*/
extern INT32 WiFiIpc_Config(nvt_wifi_settings *pwifi);

/**
    Get WiFi site survey request

    @param[in]      pIntfName  interface name
    @param[out]     pStatus    pointer of request status

    @return
        - @b 0 success, -1 fail
*/
extern INT32 WiFiIpc_getWlSiteSurveyRequest(const char *pIntfName, int *pStatus);

/**
    Get WiFi site survey result

    @param[in]      pIntfName  interface name
    @param[out]     pStatus    pointer of all site information
    @param[in]      size       size of pStatus

    @return
        - @b 0 success, -1 fail
*/
extern INT32 WiFiIpc_getWlSiteSurveyResult(const char *pIntfName, NVT_SS_STATUS_Tp pStatus, UINT32 size);
extern INT32 WiFiIpc_getWlP2PScanRequest(const char *pIntfName, int *pStatus);
extern INT32 WiFiIpc_getWlP2PScanResult(const char *pIntfName, NVT_SS_STATUS_Tp pStatus, UINT32 size);
extern INT32 WiFiIpc_sendP2PProvisionCommInfo(const char *pIntfName, NVT_P2P_PROVISION_COMM_Tp pP2PProvisionComm);
extern INT32 WiFiIpc_create_wscd(void);
extern INT32 WiFiIpc_wsc_reinit(void);
extern INT32 WiFiIpc_wsc_stop_service(void);
extern INT32 WiFiIpc_generate_pin_code(char *pinbuf);
extern INT32 WiFiIpc_wscd_is_start(void);
extern INT32 WiFiIpc_ignore_down_up(const char *pIntfName, int val);
extern INT32 WiFiIpc_get_connect_info(const char *pIntfName, const char *pStaMac, NVT_Connect_Info info, unsigned char *buf, unsigned int size);
extern INT32 WiFiIpc_set_country(const char *country);
extern INT32 WiFiIpc_set_frequency(NVT_FREQUENCY_TYPE f);

/**
    Remove WiFi interface.
    The API would remove wifi device from ecos sdio framework;

    @param[in]      pIntfName  interface name

    @return
        - @b 0 success, -1 fail
*/
extern ER WiFiIpc_interface_remove(const char *pIntfName);
#endif /* _WIFIIPCAPI_H  */
