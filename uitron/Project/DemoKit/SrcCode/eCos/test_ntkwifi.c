#if (defined (__ECOS))

#include <network.h>
#include <cyg/infra/cyg_type.h>
#include <cyg/infra/diag.h>
#include <cyg/io/devtab.h>
#include <cyg/hal/hal_io.h>
#include <cyg/hal/hal_intr.h>
//#include <cyg/hal/var_ints.h>
#include <cyg/hal/plf_io.h>
#include <cyg/kernel/kapi.h>
#include <cyg/io/io.h>
#include <cyg/hal/hal_arch.h>
#include <cyg/infra/cyg_ass.h>

#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <cyg/infra/mainfunc.h>
#include <cyg/infra/maincmd.h>

#include <cyg/io/eth/nvt/18211/wlan_api.h>
#include <cyg/io/eth/nvt/18211/wireless.h>
#include <net/ntkcmd.h>
#include <cyg/net/wpa_supplicant/nvt/18211/wpa_supplicant_api.h>

#include "Utility.h"

//nash:debug
#include <cyg/io/sdio_linux.h>
//nash:debug:end

#define MAX_BSS_DESC    64
#define MAX_STA_NUM 64  // max support sta number


#define MAX_DEVICE_NAME_LEN			32
#define WSC_MAX_SSID_LEN			32
#define MAX_WEP_KEY_LEN				26
#define MAX_NETWORK_KEY_LEN			64
#define PIN_LEN					8
#define ECOS_BAND_SEL_5G 0

enum NETWORK_TYPE {
    WIRELESS_11B = 1,
    WIRELESS_11G = 2,
    WIRELESS_11A = 4,
    WIRELESS_11N = 8,
    WIRELESS_11AC = 64
};

typedef enum _Capability {
    cESS        = 0x01,
    cIBSS       = 0x02,
    cPollable       = 0x04,
    cPollReq        = 0x01,
    cPrivacy        = 0x10,
    cShortPreamble  = 0x20,
} Capability;

char gipaddr[16]="192.168.1.254";
int is_hex_psk=1;
char wep_key[65] = "abcdefghijklm";
char wep_key_idx =0;
char gpsk[65] = "12345678";
char gssid[33] = "NTK_ap";
char gauth[15] = "wpa2-psk-aes";
int gchan =7;

int chan_2G_index=14;
int chan_5G_index=11;

int chan_2G_list[14]={0,1,2,3,4,5,6,7,8,9,10,11,12,13};
int chan_5G_list[11]={36,40,44,48,149,151,153,157,159,161,165};    // temporarily not support
static int ntk_testcmd_initialized = 0;

#ifndef _WPS_DEF_H_
typedef enum { BAND_11B=1, BAND_11G=2, BAND_11BG=3, BAND_11A=4, BAND_11N=8, BAND_5G_11AN=12,
    BAND_5G_11AC=64,BAND_5G_11AAC=68,BAND_5G_11NAC=72,BAND_5G_11ANAC=76} BAND_TYPE_T;
#endif

/* WLAN sta info structure */
typedef struct wlan_sta_info {
    unsigned short  aid;
    unsigned char   addr[6];
    unsigned long   tx_packets;
    unsigned long   rx_packets;
    unsigned long   expired_time;   // 10 msec unit
    unsigned short  flag;
    unsigned char   txOperaRates;
    unsigned char   rssi;
    unsigned long   link_time;      // 1 sec unit
    unsigned long   tx_fail;
    unsigned long tx_bytes;
    unsigned long rx_bytes;
    unsigned char network;
    unsigned char ht_info;  // bit0: 0=20M mode, 1=40M mode; bit1: 0=longGI, 1=shortGI
    unsigned char   resv[6];
} WLAN_STA_INFO_T, *WLAN_STA_INFO_Tp;

typedef struct wlan_rate{
    unsigned int id;
    unsigned char rate[20];
}WLAN_RATE_T, *WLAN_RATE_Tp;

typedef enum {
    MCS0=0x80,
    MCS1=0x81,
    MCS2=0x82,
    MCS3=0x83,
    MCS4=0x84,
    MCS5=0x85,
    MCS6=0x86,
    MCS7=0x87,
    MCS8=0x88,
    MCS9=0x89,
    MCS10=0x8a,
    MCS11=0x8b,
    MCS12=0x8c,
    MCS13=0x8d,
    MCS14=0x8e,
    MCS15=0x8f
} RATE_11N_T;

typedef enum{
  WIFI_LINK_STATUS_DISCONNECTED=0,
  WIFI_LINK_STATUS_CONNECTED,
  WIFI_LINK_STATUS_AUTH_FAIL,
  WIFI_LINK_STATUS_DISASSOCIATED,
  WIFI_LINK_STATUS_DEAUTHENTICATED,
  WIFI_LINK_STATUS_HANDSHAKE_TIMEOUT,
  WIFI_LINK_STATUS_SCAN_ZERO_RESULT
}_link_status;

typedef enum{
  WIFI_STA_STATUS_ASSOCIATED=1,
  WIFI_STA_STATUS_REASSOCIATED,
  WIFI_STA_STATUS_DISASSOCIATED,
  WIFI_STA_STATUS_DEAUTHENTICATED,
  WIFI_STA_STATUS_PORT_AUTHORIZED,
  WIFI_STA_STATUS_AUTH_FAIL,
  WIFI_STA_STATUS_EXPIRED,
  WIFI_AP_READY
}_sta_status;

//changes in following table should be synced to MCS_DATA_RATEStr[] in 8190n_proc.c
WLAN_RATE_T rate_11n_table_20M_LONG[]={
    {MCS0,  "6.5"},
    {MCS1,  "13"},
    {MCS2,  "19.5"},
    {MCS3,  "26"},
    {MCS4,  "39"},
    {MCS5,  "52"},
    {MCS6,  "58.5"},
    {MCS7,  "65"},
    {MCS8,  "13"},
    {MCS9,  "26"},
    {MCS10, "39"},
    {MCS11, "52"},
    {MCS12, "78"},
    {MCS13, "104"},
    {MCS14, "117"},
    {MCS15, "130"},
    {0}
};

WLAN_RATE_T rate_11n_table_20M_SHORT[]={
    {MCS0,  "7.2"},
    {MCS1,  "14.4"},
    {MCS2,  "21.7"},
    {MCS3,  "28.9"},
    {MCS4,  "43.3"},
    {MCS5,  "57.8"},
    {MCS6,  "65"},
    {MCS7,  "72.2"},
    {MCS8,  "14.4"},
    {MCS9,  "28.9"},
    {MCS10, "43.3"},
    {MCS11, "57.8"},
    {MCS12, "86.7"},
    {MCS13, "115.6"},
    {MCS14, "130"},
    {MCS15, "144.5"},
    {0}
};

WLAN_RATE_T rate_11n_table_40M_LONG[]={
    {MCS0,  "13.5"},
    {MCS1,  "27"},
    {MCS2,  "40.5"},
    {MCS3,  "54"},
    {MCS4,  "81"},
    {MCS5,  "108"},
    {MCS6,  "121.5"},
    {MCS7,  "135"},
    {MCS8,  "27"},
    {MCS9,  "54"},
    {MCS10, "81"},
    {MCS11, "108"},
    {MCS12, "162"},
    {MCS13, "216"},
    {MCS14, "243"},
    {MCS15, "270"},
    {0}
};

WLAN_RATE_T rate_11n_table_40M_SHORT[]={
    {MCS0,  "15"},
    {MCS1,  "30"},
    {MCS2,  "45"},
    {MCS3,  "60"},
    {MCS4,  "90"},
    {MCS5,  "120"},
    {MCS6,  "135"},
    {MCS7,  "150"},
    {MCS8,  "30"},
    {MCS9,  "60"},
    {MCS10, "90"},
    {MCS11, "120"},
    {MCS12, "180"},
    {MCS13, "240"},
    {MCS14, "270"},
    {MCS15, "300"},
    {0}
};

enum wifi_state {
	WIFI_STATION_STATE	=	0x00000008,
	WIFI_AP_STATE		=	0x00000010,
	WIFI_P2P_SUPPORT	=	0x08000000
};

typedef enum _wlan_mac_state {
    STATE_DISABLED=0, STATE_IDLE, STATE_SCANNING, STATE_STARTED, STATE_CONNECTED, STATE_WAITFORKEY
} wlan_mac_state;

//bss_info_2_web
typedef struct _bss_info {
    unsigned char state;
    unsigned char channel;
    unsigned char txRate;
    unsigned char bssid[6];
    unsigned char rssi, sq;
    unsigned char ssid[33];
} bss_info;

//20170921 nash: TBD
//SS_STATUS_T ss_status; //site survey result
//20170921 nash:end

static short freq_channel_map[14+1]=
{
    /*auto, CH1,  CH2,  CH3,  CH4,  CH5,  CH6,  CH7, */
    0,      2412, 2417, 2422, 2427, 2432, 2437, 2442,
    /*CH8,  CH9,  CH10, CH11, CH12, CH13, CH14*/
    2447,   2452, 2457, 2462, 2467, 2472, 2484
};

static const char* none_security_cmd[] =
{
    "wpa_cli SET_NETWORK 0 key_mgmt NONE",
    NULL
};

//20171020 nash: currently unused
#if 0
static const char* wep_open_security_cmd[] =
{
    "wpa_cli SET_NETWORK 0 key_mgmt NONE",
    "wpa_cli SET_NETWORK 0 auth_alg OPEN",
    NULL
};

static const char* wep_share_security_cmd[] =
{
    "wpa_cli SET_NETWORK 0 key_mgmt NONE",
    "wpa_cli SET_NETWORK 0 auth_alg SHARED",
    NULL
};
#endif

static const char* wep_auto_security_cmd[] =
{
    "wpa_cli SET_NETWORK 0 key_mgmt NONE",
    "wpa_cli SET_NETWORK 0 auth_alg OPEN SHARED",
    NULL
};

static const char* wpa_psk_tkip_security_cmd[] =
{
    "wpa_cli SET_NETWORK 0 key_mgmt WPA-PSK",
    "wpa_cli SET_NETWORK 0 pairwise TKIP",
//    "wpa_cli SET_NETWORK 0 group TKIP",
    "wpa_cli SET_NETWORK 0 proto WPA",
    NULL
};


static const char* wpa_psk_aes_security_cmd[] =
{
    "wpa_cli SET_NETWORK 0 key_mgmt WPA-PSK",
    "wpa_cli SET_NETWORK 0 pairwise CCMP",
    "wpa_cli SET_NETWORK 0 group CCMP",
    "wpa_cli SET_NETWORK 0 proto WPA",
    NULL
};

static const char* wpa2_psk_aes_security_cmd[] =
{
    "wpa_cli SET_NETWORK 0 key_mgmt WPA-PSK",
    "wpa_cli SET_NETWORK 0 pairwise CCMP",
//    "wpa_cli SET_NETWORK 0 group CCMP",
    "wpa_cli SET_NETWORK 0 proto RSN",
    NULL
};

static const char* wpa_auto_psk_tkipaes_security_cmd[] =
{
    "wpa_cli SET_NETWORK 0 key_mgmt WPA-PSK",
    NULL
};


typedef struct
{
    const char*    str_security_mode;
    unsigned int   support_mode;
    const char**   cmds;
}WIFI_SECURITY_MODE_T;

#define WIFI_MODE_STA       0x01
#define WIFI_MODE_AP        0x02

static const WIFI_SECURITY_MODE_T wifi_security_tbl[] =
{
    /*security_mode, wifi_mode,                  cms_str_tbl*/
    {"none",         WIFI_MODE_STA|WIFI_MODE_AP, none_security_cmd},
    {"wep64-auto",   WIFI_MODE_STA|WIFI_MODE_AP, wep_auto_security_cmd},
    {"wep128-auto",  WIFI_MODE_STA|WIFI_MODE_AP, wep_auto_security_cmd},
    {"wpa-psk-tkip", WIFI_MODE_STA|WIFI_MODE_AP, wpa_psk_tkip_security_cmd},
    {"wpa-psk-aes",  WIFI_MODE_STA|WIFI_MODE_AP, wpa_psk_aes_security_cmd},
    {"wpa2-psk-aes", WIFI_MODE_STA|WIFI_MODE_AP, wpa2_psk_aes_security_cmd},
    {"wpa-auto-psk-tkipaes", WIFI_MODE_AP,       wpa_auto_psk_tkipaes_security_cmd},
    {NULL,           0,                          NULL},
};

/*----------------------------------------------------------------------------*/
/* Local Types Definitions                                                    */
/*----------------------------------------------------------------------------*/
typedef struct tag_cmd
{
    char *name;			/* command string */
    int (*cmd_func)(int argc, char *argv[]);    /* command function pointer*/
    char *desc;			/* description */
} CMD_T;

/*----------------------------------------------------------------------------*/
/* Local Function Prototype                                                   */
/*----------------------------------------------------------------------------*/
static int do_wifi_help( int argc, char *argv[] );
//20170828 nash:
#if 1
static int do_wifi_init_cmd( int argc, char *argv[] );
static int do_wifi_setting_cmd( int argc, char *argv[] );
static int do_wifi_wps_cmd( int argc, char *argv[] );
static int do_wifi_sta_cmd( int argc, char *argv[] );
static int do_wifi_ap_cmd( int argc, char *argv[] );
static int do_wifi_set_cmd(int argc, char *argv[]);
static int do_wifi_status_cmd(int argc, char *argv[]);
static int do_wifi_sta_clients_cmd( int argc, char *argv[] );
#endif
static int do_wifi_power_on_cmd( int argc, char *argv[] );
static int do_wifi_power_down_cmd( int argc, char *argv[] );


/*----------------------------------------------------------------------------*/
/* Local Global Variables                                                     */
/*----------------------------------------------------------------------------*/

static CMD_T wifi_cmds[] =
{
//20170828 nash: not support
#if 1
    {"init",    do_wifi_init_cmd,     "wifi(wlan0) init"},
    {"settings",do_wifi_setting_cmd,  "wifi setting"},
    //{"wlan",    do_wifi_wlan_cmd,     "enable/disable wlan"},
    {"wps",     do_wifi_wps_cmd,      "enable/disable wps"},
    {"sta",     do_wifi_sta_cmd,      "sta operation"},
    {"ap",      do_wifi_ap_cmd,       "ap operation"},
    {"set",     do_wifi_set_cmd,      "set parameters"},
    {"status",  do_wifi_status_cmd,   "status"},
    {"associated_clients", do_wifi_sta_clients_cmd,  "show STA clients info"},
#endif
    {"pwron",   do_wifi_power_on_cmd,  "wifi on"},
    {"pwrdown", do_wifi_power_down_cmd,  "wifi off"},
    {NULL,      NULL,               NULL}
};

//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------

//20170828 nash:
#if 1
void translate_control_code(char *buffer)
{
    char tmpBuf[200], *p1 = buffer, *p2 = tmpBuf;


    while (*p1) {
        if (*p1 == '"') {
            memcpy(p2, "&quot;", 6);
            p2 += 6;
        }
        else if (*p1 == '\x27') {
            memcpy(p2, "&#39;", 5);
            p2 += 5;
        }
        else if (*p1 == '\x5c') {
            memcpy(p2, "&#92;", 5);
            p2 += 5;
        }
        else
            *p2++ = *p1;
        p1++;
    }
    *p2 = '\0';

    strcpy(buffer, tmpBuf);
}

//-----------------------------------------------------------------------------
// wifi
//-----------------------------------------------------------------------------
//20171020 nash: currently unused
#if 0
static int IS_MCAST(char *da)
{
    if ((*da) & 0x01)
        return 1;
    else
        return 0;
}
#endif

struct wifi_settings_sample
{
    int wlan_disable;
    int wps_disable;
    char mode[16];
    char security[32];
    int channel;
    char ssid[WSC_MAX_SSID_LEN+1];
    char passphrase[MAX_NETWORK_KEY_LEN+1];
    char wep_key1[MAX_WEP_KEY_LEN+1];
    char wep_key2[MAX_WEP_KEY_LEN+1];
    char wep_key3[MAX_WEP_KEY_LEN+1];
    char wep_key4[MAX_WEP_KEY_LEN+1];
    int wep_default_key;
    char wsc_pin[PIN_LEN+1];
    int is_configured;
    int config_by_ext_reg;
    int p2pmode;
    char device_name[MAX_DEVICE_NAME_LEN+1];
    int is_dhcp; // static ip or dhcp for client mode
};

struct wifi_settings_sample wifi_settings_sample;

void dump_wifi_settings(void)
{
    printf("wlan_disable=%d\n", wifi_settings_sample.wlan_disable);
    printf("wps_disable=%d\n", wifi_settings_sample.wps_disable);
    printf("mode=%s\n", wifi_settings_sample.mode);
    if (strcmp(wifi_settings_sample.mode, "sta") == 0)
    	printf("is_dhcp=%d\n", wifi_settings_sample.is_dhcp);
    printf("security=%s\n", wifi_settings_sample.security);
    printf("channel=%d (%d MHz)\n", wifi_settings_sample.channel, freq_channel_map[wifi_settings_sample.channel]);
    printf("ssid=%s\n", wifi_settings_sample.ssid);
    printf("passphrase=%s\n", wifi_settings_sample.passphrase);
    printf("wep_default_key=%d\n", wifi_settings_sample.wep_default_key);
    printf("wep_key1=%s\n", wifi_settings_sample.wep_key1);
    printf("wep_key2=%s\n", wifi_settings_sample.wep_key2);
    printf("wep_key3=%s\n", wifi_settings_sample.wep_key3);
    printf("wep_key4=%s\n", wifi_settings_sample.wep_key4);
    printf("wsc_pin=%s\n", wifi_settings_sample.wsc_pin);
    printf("is_configured=%d\n", wifi_settings_sample.is_configured);
    printf("config_by_ext_reg=%d\n", wifi_settings_sample.config_by_ext_reg);
    printf("p2pmode=%d\n", wifi_settings_sample.p2pmode);
}

static void init_wifi_settings(char *mode, char *security)
{
    struct wifi_settings_sample *pwifi=&wifi_settings_sample;

    if (strcmp(pwifi->mode, mode) != 0) {
        //if mode is going to be changed, reset wps to unconfigured mode
        pwifi->is_configured = 0;
    }

    //20181004 nash: coverity fix, CID#109432
    //strcpy(pwifi->mode, mode);
    //strcpy(pwifi->security, security);
    strncpy(pwifi->mode, mode, sizeof(pwifi->mode) - 1);
    strncpy(pwifi->security, security, sizeof(pwifi->security) - 1);

    //pwifi->channel = 0; // auto channel
    pwifi->channel = 7;

    if (strcmp(pwifi->mode, "ap") == 0) {
		if(strlen(pwifi->ssid) == 0)
        //strcpy(pwifi->ssid, "NTK_ap");
        //strcpy(pwifi->device_name, "Wireless AP");
        strncpy(pwifi->ssid, "NTK_ap", sizeof(pwifi->ssid) - 1);
        strncpy(pwifi->device_name, "Wireless AP", sizeof(pwifi->device_name) - 1);
    }
    else if (strcmp(pwifi->mode, "sta") == 0) {
        //strcpy(pwifi->ssid, "000ecos");
        if(strlen(pwifi->ssid) == 0)
            //strcpy(pwifi->ssid, "TP-LINK");
            strncpy(pwifi->ssid, "TP-LINK", sizeof(pwifi->ssid));
        //strcpy(pwifi->ssid, "Farquaad");
        //strcpy(pwifi->device_name, "Wireless STA");
        strncpy(pwifi->device_name, "Wireless STA", sizeof(pwifi->device_name) - 1);
    }

    //strcpy(pwifi->passphrase, "12345678");
    strncpy(pwifi->passphrase, "12345678", sizeof(pwifi->passphrase) - 1);

    pwifi->wep_default_key = 0;
    if (strcmp(pwifi->security, "wep64-auto") == 0) {
        //strcpy(pwifi->wep_key1, "1234567890");
        //strcpy(pwifi->wep_key2, "1234567890");
        //strcpy(pwifi->wep_key3, "1234567890");
        //strcpy(pwifi->wep_key4, "1234567890");
        strncpy(pwifi->wep_key1, "1234567890", sizeof(pwifi->wep_key1) - 1);
        strncpy(pwifi->wep_key2, "1234567890", sizeof(pwifi->wep_key2) - 1);
        strncpy(pwifi->wep_key3, "1234567890", sizeof(pwifi->wep_key3) - 1);
        strncpy(pwifi->wep_key4, "1234567890", sizeof(pwifi->wep_key4) - 1);

    }
    else {
        //strcpy(pwifi->wep_key1, "12345678901234567890123456");
        //strcpy(pwifi->wep_key2, "12345678901234567890123456");
        //strcpy(pwifi->wep_key3, "12345678901234567890123456");
        //strcpy(pwifi->wep_key4, "12345678901234567890123456");
        strncpy(pwifi->wep_key1, "12345678901234567890123456", sizeof(pwifi->wep_key1) - 1);
        strncpy(pwifi->wep_key2, "12345678901234567890123456", sizeof(pwifi->wep_key2) - 1);
        strncpy(pwifi->wep_key3, "12345678901234567890123456", sizeof(pwifi->wep_key3) - 1);
        strncpy(pwifi->wep_key4, "12345678901234567890123456", sizeof(pwifi->wep_key4) - 1);
    }
}

static void start_wifi_settings(char *mode, char *security){

   //printf("%s: \n", __func__);

	 struct wifi_settings_sample *pwifi=&wifi_settings_sample;

	 if (strcmp(pwifi->mode, mode) != 0) {
		 //if mode is going to be changed, reset wps to unconfigured mode
		 pwifi->is_configured = 0;
	 }
         //20190119 nash: coverity fix, CID#10944
         //strcpy(pwifi->mode, mode);
         //strcpy(pwifi->security, security);
	 strncpy(pwifi->mode, mode, sizeof(pwifi->mode) - 1);
	 strncpy(pwifi->security, security, sizeof(pwifi->security) - 1);
	 //pwifi->channel = 0; // auto channel
	 pwifi->channel = gchan;
         //strcpy(pwifi->ssid, gssid);
	 strncpy(pwifi->ssid, gssid, sizeof(pwifi->ssid) - 1);

	 if (strcmp(pwifi->mode, "ap") == 0) {
		 //strcpy(pwifi->device_name, "Wireless AP");
                 strncpy(pwifi->device_name, "Wireless AP", sizeof(pwifi->device_name) - 1);
	 }
	 else if (strcmp(pwifi->mode, "sta") == 0) {
		 //strcpy(pwifi->device_name, "Wireless STA");
                 strncpy(pwifi->device_name, "Wireless STA", sizeof(pwifi->device_name) - 1);
	 }

	 //strcpy(pwifi->passphrase, gpsk);
         strncpy(pwifi->passphrase, gpsk, sizeof(pwifi->passphrase) - 1);

         #if 0
	 pwifi->wep_default_key = 0;
	 if (strcmp(pwifi->security, "wep64-auto") == 0) {
		 strcpy(pwifi->wep_key1, gpsk);
		 strcpy(pwifi->wep_key2, "abcdefghijklm");
		 strcpy(pwifi->wep_key3, "abcdefghijklm");
		 strcpy(pwifi->wep_key4, "abcdefghijklm");
	 }
	 else {
		 strcpy(pwifi->wep_key1, gpsk);
		 strcpy(pwifi->wep_key2, "12345678901234567890123456");
		 strcpy(pwifi->wep_key3, "12345678901234567890123456");
		 strcpy(pwifi->wep_key4, "12345678901234567890123456");
	 }
	 #else
	 pwifi->wep_default_key = wep_key_idx;
         //20180104 nash: coverity fix, CID#109433
         //strcpy(pwifi->wep_key1, wep_key);
         strncpy(pwifi->wep_key1, wep_key, sizeof(pwifi->wep_key1) - 1);
	 #endif

}

static int configure_wifi_sample(struct wifi_settings_sample *pwifi)
{
    //char *ifname = "wlan0";
    char tmpbuf[256];
    const WIFI_SECURITY_MODE_T *p_wifi_security_cfg = wifi_security_tbl;

    sprintf(tmpbuf, "nvtwsupp cli DISABLE_NETWORK 0");
    RunNvtCmd(tmpbuf);

    sprintf(tmpbuf, "nvtwsupp cli REMOVE_NETWORK 0");
    RunNvtCmd(tmpbuf);

    sprintf(tmpbuf, "nvtwsupp cli ADD_NETWORK");
    RunNvtCmd(tmpbuf);

    //AP mode
    if (strcmp(pwifi->mode, "ap") == 0) {
        sprintf(tmpbuf, "nvtwsupp cli SET_NETWORK 0 mode 2");
        RunNvtCmd(tmpbuf);

        sprintf(tmpbuf, "nvtwsupp cli SET_NETWORK 0 ssid \"%s\"", pwifi->ssid);
        RunNvtCmd(tmpbuf);

        sprintf(tmpbuf, "nvtwsupp cli SET_NETWORK 0 frequency %d", freq_channel_map[wifi_settings_sample.channel]);
        RunNvtCmd(tmpbuf);

#if 1
        while( p_wifi_security_cfg->str_security_mode != NULL){
            if (strcmp(pwifi->security, p_wifi_security_cfg->str_security_mode) != 0) {
                p_wifi_security_cfg++;
                continue;
            }
            break;
        }

        if(p_wifi_security_cfg->cmds != NULL)
        {
            const char **cmd = p_wifi_security_cfg->cmds;
            while(*cmd != NULL) {
                sprintf(tmpbuf, *cmd);
                RunNvtCmd(tmpbuf);
                cmd++;
            }
        }

        if(strcmp(pwifi->security, "none") == 0) {
            ;//do nothing
        }
        else if ((strcmp(pwifi->security, "wep64-auto") == 0) ||
                 (strcmp(pwifi->security, "wep128-auto") == 0)) {
            //sprintf(tmpbuf, "nvtwsupp cli SET_NETWORK 0 wep_key0 \"%s\"", pwifi->wep_key1);
            //RunNvtCmd(tmpbuf);

            //sprintf(tmpbuf, "nvtwsupp cli SET_NETWORK 0 wep_key1 \"%s\"", pwifi->wep_key2);
            //RunNvtCmd(tmpbuf);

            //sprintf(tmpbuf, "nvtwsupp cli SET_NETWORK 0 wep_key2 \"%s\"", pwifi->wep_key3);
            //RunNvtCmd(tmpbuf);

            //sprintf(tmpbuf, "nvtwsupp cli SET_NETWORK 0 wep_key3 \"%s\"", pwifi->wep_key4);
            //RunNvtCmd(tmpbuf);

            if(is_hex_psk)
                sprintf(tmpbuf, "nvtwsupp cli SET_NETWORK 0 wep_key%d %s", pwifi->wep_default_key, pwifi->wep_key1);
            else
                sprintf(tmpbuf, "nvtwsupp cli SET_NETWORK 0 wep_key%d \"%s\"", pwifi->wep_default_key, pwifi->wep_key1);
            RunNvtCmd(tmpbuf);

            sprintf(tmpbuf, "nvtwsupp cli SET_NETWORK 0 wep_tx_keyidx %d", pwifi->wep_default_key);
            RunNvtCmd(tmpbuf);
        }
        else {//wpa or wpa2
            if(is_hex_psk)
                sprintf(tmpbuf, "nvtwsupp cli SET_NETWORK 0 psk %s", pwifi->passphrase);
            else
                sprintf(tmpbuf, "nvtwsupp cli SET_NETWORK 0 psk \"%s\"", pwifi->passphrase);
            RunNvtCmd(tmpbuf);
        }
#else
        if (strcmp(pwifi->security, "none") == 0) {
            sprintf(tmpbuf, "nvtwsupp cli SET_NETWORK 0 key_mgmt NONE");
            RunNvtCmd(tmpbuf);
        }
        else if (strcmp(pwifi->security, "wep64-auto") == 0) {
            sprintf(tmpbuf, "nvtwsupp cli SET_NETWORK 0 key_mgmt NONE");
            RunNvtCmd(tmpbuf);

            sprintf(tmpbuf, "nvtwsupp cli SET_NETWORK 0 wep_key0 %s", pwifi->wep_key1);
            RunNvtCmd(tmpbuf);

            sprintf(tmpbuf, "nvtwsupp cli SET_NETWORK 0 wep_key1 %s", pwifi->wep_key2);
            RunNvtCmd(tmpbuf);

            sprintf(tmpbuf, "nvtwsupp cli SET_NETWORK 0 wep_key2 %s", pwifi->wep_key3);
            RunNvtCmd(tmpbuf);

            sprintf(tmpbuf, "nvtwsupp cli SET_NETWORK 0 wep_key3 %s", pwifi->wep_key4);
            RunNvtCmd(tmpbuf);

            sprintf(tmpbuf, "nvtwsupp cli SET_NETWORK 0 wep_tx_keyidx %d", pwifi->wep_default_key);
            RunNvtCmd(tmpbuf);
        }
        else if (strcmp(pwifi->security, "wep128-auto") == 0) {
            sprintf(tmpbuf, "nvtwsupp cli SET_NETWORK 0 key_mgmt NONE");
            RunNvtCmd(tmpbuf);

            sprintf(tmpbuf, "nvtwsupp cli SET_NETWORK 0 wep_key0 %s", pwifi->wep_key1);
            RunNvtCmd(tmpbuf);

            sprintf(tmpbuf, "nvtwsupp cli SET_NETWORK 0 wep_key1 %s", pwifi->wep_key2);
            RunNvtCmd(tmpbuf);

            sprintf(tmpbuf, "nvtwsupp cli SET_NETWORK 0 wep_key2 %s", pwifi->wep_key3);
            RunNvtCmd(tmpbuf);

            sprintf(tmpbuf, "nvtwsupp cli SET_NETWORK 0 wep_key3 %s", pwifi->wep_key4);
            RunNvtCmd(tmpbuf);

            sprintf(tmpbuf, "nvtwsupp cli SET_NETWORK 0 wep_tx_keyidx %d", pwifi->wep_default_key);
            RunNvtCmd(tmpbuf);
        }
        else if (strcmp(pwifi->security, "wpa-psk-tkip") == 0) {
            sprintf(tmpbuf, "nvtwsupp cli SET_NETWORK 0 key_mgmt WPA-PSK");
            RunNvtCmd(tmpbuf);

            sprintf(tmpbuf, "nvtwsupp cli SET_NETWORK 0 psk \"%s\"", pwifi->passphrase);
            RunNvtCmd(tmpbuf);

            sprintf(tmpbuf, "nvtwsupp cli SET_NETWORK 0 pairwise TKIP");
            RunNvtCmd(tmpbuf);

            //sprintf(tmpbuf, "nvtwsupp cli SET_NETWORK 0 group TKIP");
            //RunNvtCmd(tmpbuf);

            sprintf(tmpbuf, "nvtwsupp cli SET_NETWORK 0 proto WPA");
            RunNvtCmd(tmpbuf);
        }
        else if (strcmp(pwifi->security, "wpa2-psk-aes") == 0) {
            sprintf(tmpbuf, "nvtwsupp cli SET_NETWORK 0 key_mgmt WPA-PSK");
            RunNvtCmd(tmpbuf);

            sprintf(tmpbuf, "nvtwsupp cli SET_NETWORK 0 psk \"%s\"", pwifi->passphrase);
            RunNvtCmd(tmpbuf);

            sprintf(tmpbuf, "nvtwsupp cli SET_NETWORK 0 pairwise CCMP");
            RunNvtCmd(tmpbuf);

            //sprintf(tmpbuf, "nvtwsupp cli SET_NETWORK 0 group CCMP");
            //RunNvtCmd(tmpbuf);

            sprintf(tmpbuf, "nvtwsupp cli SET_NETWORK 0 proto RSN");
            RunNvtCmd(tmpbuf);
        }
        else if (strcmp(pwifi->security, "wpa-auto-psk-tkipaes") == 0) {
            sprintf(tmpbuf, "nvtwsupp cli SET_NETWORK 0 key_mgmt WPA-PSK");
            RunNvtCmd(tmpbuf);

            sprintf(tmpbuf, "nvtwsupp cli SET_NETWORK 0 psk \"%s\"", pwifi->passphrase);
            RunNvtCmd(tmpbuf);

            //sprintf(tmpbuf, "nvtwsupp cli SET_NETWORK 0 pairwise CCMP");
            //RunNvtCmd(tmpbuf);

            //sprintf(tmpbuf, "nvtwsupp cli SET_NETWORK 0 group CCMP");
            //RunNvtCmd(tmpbuf);

            //sprintf(tmpbuf, "nvtwsupp cli SET_NETWORK 0 proto RSN");
            //RunNvtCmd(tmpbuf);
        }
#endif

        sprintf(tmpbuf, "nvtwsupp cli ENABLE_NETWORK 0");
        RunNvtCmd(tmpbuf);
    }
    //STA mode
    else if (strcmp(pwifi->mode, "sta") == 0) {
        sprintf(tmpbuf, "nvtwsupp cli SET_NETWORK 0 ssid \"%s\"", pwifi->ssid);
        RunNvtCmd(tmpbuf);
#if 1
        while( p_wifi_security_cfg->str_security_mode != NULL){
            if (strcmp(pwifi->security, p_wifi_security_cfg->str_security_mode) != 0) {
                p_wifi_security_cfg++;
                continue;
            }
            break;
        }

        if(p_wifi_security_cfg->cmds != NULL)
        {
            const char **cmd = p_wifi_security_cfg->cmds;
            while(*cmd != NULL) {
                sprintf(tmpbuf, *cmd);
                RunNvtCmd(tmpbuf);
                cmd++;
            }
        }

        if(strcmp(pwifi->security, "none") == 0) {
            ;//do nothing
        }
        else if ((strcmp(pwifi->security, "wep64-auto") == 0) ||
                 (strcmp(pwifi->security, "wep128-auto") == 0)) {
            //sprintf(tmpbuf, "nvtwsupp cli SET_NETWORK 0 wep_key0 \"%s\"", pwifi->wep_key1);
            //RunNvtCmd(tmpbuf);

            //sprintf(tmpbuf, "nvtwsupp cli SET_NETWORK 0 wep_key1 \"%s\"", pwifi->wep_key2);
            //RunNvtCmd(tmpbuf);

            //sprintf(tmpbuf, "nvtwsupp cli SET_NETWORK 0 wep_key2 \"%s\"", pwifi->wep_key3);
            //RunNvtCmd(tmpbuf);

            //sprintf(tmpbuf, "nvtwsupp cli SET_NETWORK 0 wep_key3 \"%s\"", pwifi->wep_key4);
            //RunNvtCmd(tmpbuf);

            if(is_hex_psk)
                sprintf(tmpbuf, "nvtwsupp cli SET_NETWORK 0 wep_key%d %s", pwifi->wep_default_key, pwifi->wep_key1);
            else
                sprintf(tmpbuf, "nvtwsupp cli SET_NETWORK 0 wep_key%d \"%s\"", pwifi->wep_default_key, pwifi->wep_key1);
            RunNvtCmd(tmpbuf);

            sprintf(tmpbuf, "nvtwsupp cli SET_NETWORK 0 wep_tx_keyidx %d", pwifi->wep_default_key);
            RunNvtCmd(tmpbuf);

        }
        else {//wpa or wpa2
            printf("psk_len:%d \n", strlen(pwifi->passphrase));

            if(is_hex_psk)
                sprintf(tmpbuf, "nvtwsupp cli SET_NETWORK 0 psk %s", pwifi->passphrase);
            else
                sprintf(tmpbuf, "nvtwsupp cli SET_NETWORK 0 psk \"%s\"", pwifi->passphrase);
            RunNvtCmd(tmpbuf);
        }
#else
        if (strcmp(pwifi->security, "none") == 0) {
            sprintf(tmpbuf, "nvtwsupp cli SET_NETWORK 0 key_mgmt NONE");
            RunNvtCmd(tmpbuf);
        }
        else if (strcmp(pwifi->security, "wep64-auto") == 0) {
            sprintf(tmpbuf, "nvtwsupp cli SET_NETWORK 0 key_mgmt NONE");
            RunNvtCmd(tmpbuf);

            sprintf(tmpbuf, "nvtwsupp cli SET_NETWORK 0 wep_key0 %s", pwifi->wep_key1);
            RunNvtCmd(tmpbuf);

            sprintf(tmpbuf, "nvtwsupp cli SET_NETWORK 0 wep_key1 %s", pwifi->wep_key2);
            RunNvtCmd(tmpbuf);

            sprintf(tmpbuf, "nvtwsupp cli SET_NETWORK 0 wep_key2 %s", pwifi->wep_key3);
            RunNvtCmd(tmpbuf);

            sprintf(tmpbuf, "nvtwsupp cli SET_NETWORK 0 wep_key3 %s", pwifi->wep_key4);
            RunNvtCmd(tmpbuf);

            sprintf(tmpbuf, "nvtwsupp cli SET_NETWORK 0 wep_tx_keyidx %d", pwifi->wep_default_key);
            RunNvtCmd(tmpbuf);

            //RunNvtCmd("nvtwsupp cli SET_NETWORK 0 auth_alg OPEN");
        }
        else if (strcmp(pwifi->security, "wep128-auto") == 0) {
            sprintf(tmpbuf, "nvtwsupp cli SET_NETWORK 0 key_mgmt NONE");
            RunNvtCmd(tmpbuf);

            sprintf(tmpbuf, "nvtwsupp cli SET_NETWORK 0 wep_key0 %s", pwifi->wep_key1);
            RunNvtCmd(tmpbuf);

            sprintf(tmpbuf, "nvtwsupp cli SET_NETWORK 0 wep_key1 %s", pwifi->wep_key2);
            RunNvtCmd(tmpbuf);

            sprintf(tmpbuf, "nvtwsupp cli SET_NETWORK 0 wep_key2 %s", pwifi->wep_key3);
            RunNvtCmd(tmpbuf);

            sprintf(tmpbuf, "nvtwsupp cli SET_NETWORK 0 wep_key3 %s", pwifi->wep_key4);
            RunNvtCmd(tmpbuf);

            sprintf(tmpbuf, "nvtwsupp cli SET_NETWORK 0 wep_tx_keyidx %d", pwifi->wep_default_key);
            RunNvtCmd(tmpbuf);

            //RunNvtCmd("nvtwsupp cli SET_NETWORK 0 auth_alg OPEN");
        }
        else if (strcmp(pwifi->security, "wpa2-psk-aes") == 0) {
            sprintf(tmpbuf, "nvtwsupp cli SET_NETWORK 0 key_mgmt WPA-PSK");
            RunNvtCmd(tmpbuf);

            sprintf(tmpbuf, "nvtwsupp cli SET_NETWORK 0 psk %s", pwifi->passphrase);
            RunNvtCmd(tmpbuf);

            sprintf(tmpbuf, "nvtwsupp cli SET_NETWORK 0 pairwise CCMP");
            RunNvtCmd(tmpbuf);

            sprintf(tmpbuf, "nvtwsupp cli SET_NETWORK 0 group CCMP");
            RunNvtCmd(tmpbuf);

            sprintf(tmpbuf, "nvtwsupp cli SET_NETWORK 0 proto RSN");
            RunNvtCmd(tmpbuf);
        }
#endif

        sprintf(tmpbuf, "nvtwsupp cli ENABLE_NETWORK 0");

        RunNvtCmd(tmpbuf);
    }
    else {
        return -1;
    }

    //Do not up "wlan0" here. It would be up by interface_config() later.
    //interface_up("wlan0");

    //WPS
///    memset(&wpscfg, 0, sizeof(struct wps_config));
///    pcfg = &wpscfg;
///    //--------------------------------------------------------------------
///    strcpy(pcfg->wlan_interface_name, "wlan0");
///    //strcpy(pcfg->lan_interface_name, "eth0");
///    strcpy(pcfg->lan_interface_name, "wlan0");
///
///    //--------------------------------------------------------------------
///    if (is_client != 1) //AP_MODE=0, CLIENT_MODE=1
///        pcfg->start = 1;
///
///    //WSC_DEBUG("wlan mode=%d\n",is_client);
///    if (is_client == 1) {
///#ifdef __ECOS
///        intVal = MODE_CLIENT_UNCONFIG;
///#else
///
///        if (is_registrar) {
///            if (!wifi_settings_sample.is_configured)
///                intVal = MODE_CLIENT_UNCONFIG_REGISTRAR;
///            else
///                intVal = MODE_CLIENT_CONFIG;
///        }
///        else
///            intVal = MODE_CLIENT_UNCONFIG;
///#endif
///    }
///    else {
///        if (!wifi_settings_sample.is_configured)
///            intVal = MODE_AP_UNCONFIG;
///        else
///            intVal = MODE_AP_PROXY_REGISTRAR;
///    }
///    pcfg->mode = intVal;
///    //WSC_DEBUG("wsc mode=%d\n",pcfg->mode);
///
///    /*
///    if (is_client == 1)
///        intVal = 0;
///    else
///        intVal = 1; // MIB_WLAN_WSC_UPNP_ENABLED
///    pcfg->upnp = intVal;
///    */
///    pcfg->upnp = 0;
///
///#if 1 //def WPS2DOTX
///    //pcfg->config_method = (CONFIG_METHOD_VIRTUAL_PIN |  CONFIG_METHOD_PHYSICAL_PBC | CONFIG_METHOD_VIRTUAL_PBC );
///    pcfg->config_method = 0x188; //(CONFIG_METHOD_KEYPAD | CONFIG_METHOD_DISPLAY | CONFIG_METHOD_PBC);
///#else   // wps 1.0
///    //intVal = 0;
///    // MIB_WLAN_WSC_METHOD
///    // Ethernet(0x2)+Label(0x4)+PushButton(0x80) Bitwise OR
///    //1) Pin+Ethernet
///    //  intVal = (CONFIG_METHOD_ETH | CONFIG_METHOD_PIN);
///    //2) //PBC+Ethernet
///    //  intVal = (CONFIG_METHOD_ETH | CONFIG_METHOD_PBC);
///    //3) //Pin+PBC+Ethernet
///        intVal = (CONFIG_METHOD_ETH | CONFIG_METHOD_PIN | CONFIG_METHOD_PBC);
///    pcfg->config_method = intVal;
///#endif
///
///    /*assigned auth support list 2.0*/
///    pcfg->auth_type_flags = (WSC_AUTH_OPEN |   WSC_AUTH_WPA2PSK);
///
///    /*assigned Encry support list 2.0*/
///    pcfg->encrypt_type_flags =
///        (WSC_ENCRYPT_NONE |  WSC_ENCRYPT_AES );
///
///    //auth_type, MIB_WLAN_WSC_AUTH
///    pcfg->auth_type_flash = auth;
///
///    //encrypt_type, MIB_WLAN_WSC_ENC
///    pcfg->encrypt_type_flash = enc;
///    if (enc == WSC_ENCRYPT_WEP)
///        is_wep = 1;
///
///    pcfg->connect_type = 1; //CONNECT_TYPE_BSS
///    pcfg->manual_config = 0;
///
///    //--------------------------------------------------------------------
///    if (is_wep) { // only allow WEP in none-MANUAL mode (configured by external registrar)
///        pcfg->wep_transmit_key = pwifi->wep_default_key + 1;
///        strcpy(pcfg->network_key, pwifi->wep_key1);
///        strcpy(pcfg->wep_key2, pwifi->wep_key2);
///        strcpy(pcfg->wep_key3, pwifi->wep_key3);
///        strcpy(pcfg->wep_key4, pwifi->wep_key4);
///    }
///    else {
///        strcpy(pcfg->network_key, pwifi->passphrase);
///    }
///    pcfg->network_key_len = strlen(pcfg->network_key);
///
///    strcpy(pcfg->SSID, pwifi->ssid);
///
///    //strcpy(pcfg->pin_code, "12345670");
///    strcpy(pcfg->pin_code, pwifi->wsc_pin);
///
///    if (pwifi->channel > 14)
///        intVal = 2;
///    else
///        intVal = 1;
///    pcfg->rf_band = intVal;
///
///    pcfg->config_by_ext_reg = pwifi->config_by_ext_reg;
///
///    /*for detial mixed mode info*/
///    //  mixed issue
///    intVal=0;
///    if (mixedmode){ // mixed mode (WPA+WPA2)
///        if(wsc_wpa_cipher==1){
///            intVal |= WSC_WPA_TKIP;
///        }else if(wsc_wpa_cipher==2){
///            intVal |= WSC_WPA_AES;
///        }else if(wsc_wpa_cipher==3){
///            intVal |= (WSC_WPA_TKIP | WSC_WPA_AES);
///        }
///        if(wsc_wpa2_cipher==1){
///            intVal |= WSC_WPA2_TKIP;
///        }else if(wsc_wpa2_cipher==2){
///            intVal |= WSC_WPA2_AES;
///        }else if(wsc_wpa2_cipher==3){
///            intVal |= (WSC_WPA2_TKIP | WSC_WPA2_AES);
///        }
///    }
///    pcfg->mixedmode = intVal;
///    /*for detial mixed mode info*/
///
///    //wscd.conf
///    strcpy(pcfg->device_name, pwifi->device_name);
///    strcpy(pcfg->manufacturer, "Realtek Semiconductor Corp.");
///    strcpy(pcfg->manufacturerURL, "http://www.realtek.com/");
///    strcpy(pcfg->model_URL, "http://www.realtek.com/");
///    strcpy(pcfg->model_name, "RTL8xxx");
///    strcpy(pcfg->model_num, "RTK_ECOS");
///    strcpy(pcfg->serial_num, "123456789012347");
///    strcpy(pcfg->modelDescription, "WLAN Access Point");
///    strcpy(pcfg->ssid_prefix, "RTKAP_");
///
///    apply_wps_config_to_wscd(pcfg);
    return 0;
}

///int wsc_flash_param_handler_sample(struct wsc_flash_param *param)
///{
///    struct wifi_settings_sample *pwifi =&wifi_settings_sample;
///
///    //printf("%s %d\n", __FUNCTION__, __LINE__);
///    pwifi->is_configured = 1;
///    strcpy(pwifi->ssid, param->SSID);
///    pwifi->config_by_ext_reg = param->WSC_CONFIGBYEXTREG;
///    if (param->WEP) {
///        //sprintf(temp, "authtype=%d", param->AUTH_TYPE);
///        if (param->WEP == 1) //WEP64
///            strcpy(pwifi->security, "wep64-auto");
///        else
///            strcpy(pwifi->security, "wep128-auto");
///
///        pwifi->wep_default_key = param->WEP_DEFAULT_KEY;
///        strcpy(pwifi->wep_key1, param->WEP_KEY1);
///        strcpy(pwifi->wep_key2, param->WEP_KEY1);
///        strcpy(pwifi->wep_key3, param->WEP_KEY1);
///        strcpy(pwifi->wep_key4, param->WEP_KEY1);
///    }
///    else {
///        if (param->WSC_AUTH==WSC_AUTH_OPEN) {
///            if (param->WSC_ENC==WSC_ENCRYPT_NONE) {
///                strcpy(pwifi->security, "none");
///            }
///        }
///        else if (param->WSC_AUTH==WSC_AUTH_WPAPSK) {
///            if (param->WSC_ENC==WSC_ENCRYPT_TKIP)
///                strcpy(pwifi->security, "wpa-psk-tkip");
///            else if (param->WSC_ENC==WSC_ENCRYPT_AES)
///                strcpy(pwifi->security, "wpa-psk-aes");
///            else if (param->WSC_ENC==WSC_ENCRYPT_TKIPAES)
///                strcpy(pwifi->security, "wpa-psk-tkipaes");
///            strcpy(pwifi->passphrase, param->WSC_PSK);
///        }
///        else if (param->WSC_AUTH==WSC_AUTH_WPA2PSK) {
///            if (param->WSC_ENC==WSC_ENCRYPT_TKIP)
///                strcpy(pwifi->security, "wpa2-psk-tkip");
///            else if (param->WSC_ENC==WSC_ENCRYPT_AES)
///                strcpy(pwifi->security, "wpa2-psk-aes");
///            else if (param->WSC_ENC==WSC_ENCRYPT_TKIPAES)
///                strcpy(pwifi->security, "wpa2-psk-tkipaes");
///            strcpy(pwifi->passphrase, param->WSC_PSK);
///        }
///        else if (param->WSC_AUTH==WSC_AUTH_WPA2PSKMIXED) {
///            if (param->WSC_ENC==WSC_ENCRYPT_TKIP)
///                strcpy(pwifi->security, "wpa-auto-psk-tkip");
///            else if (param->WSC_ENC==WSC_ENCRYPT_AES)
///                strcpy(pwifi->security, "wpa-auto-psk-aes");
///            else if (param->WSC_ENC==WSC_ENCRYPT_TKIPAES)
///                strcpy(pwifi->security, "wpa-auto-psk-tkipaes");
///            strcpy(pwifi->passphrase, param->WSC_PSK);
///        }
///    }
///
///    return 0;
///}
///
/////-----------------------------------------------------------------------------
///// wps
/////-----------------------------------------------------------------------------
///void generate_pin_code(char *pinbuf)
///{
///    struct timeval tod;
///    unsigned long num;
///    char macaddr[6];
///
///    gettimeofday(&tod , NULL);
///    // read mac from efuse
///    if (wifi_get_wlan0_efuse_mac(macaddr) < 0) {
///        macaddr[4] = 0x89;
///        macaddr[5] = 0xe5;
///    }
///    else {
///        unsigned char zero[] = {0, 0, 0, 0, 0, 0};
///        //sanity check
///        if (!memcmp(macaddr, zero, MACADDRLEN) || IS_MCAST(macaddr)) {
///            macaddr[4] = 0x89;
///            macaddr[5] = 0xe5;
///        }
///    }
///    tod.tv_sec += macaddr[4]+macaddr[5];
///    srand(tod.tv_sec+tod.tv_usec);
///    num = rand() % 10000000;
///    num = num*10 + compute_pin_checksum(num);
///    convert_hex_to_ascii((unsigned long)num, pinbuf);
///}

//20171020 nash: currently unused
#if 0
static int get_thread_info_by_name(char *name, cyg_thread_info *pinfo)
{
    cyg_handle_t thandle = 0;
    cyg_uint16 id = 0;

    while (cyg_thread_get_next(&thandle, &id)) {
        cyg_thread_get_info(thandle, id, pinfo);
        //printf("ID: %04x name: %10s pri: %d\n",
                //  pinfo->id, pinfo->name ? pinfo->name:"----", pinfo->set_pri);
        if (strcmp(name, pinfo->name)==0)
            return 1;
    }
    return 0;
}
#endif

unsigned char sample_sys_stack[36*1024];
cyg_handle_t sample_sys_thread_handle;
cyg_thread sample_sys_thread_obj;

#define BIT(x)                          (1UL<<(x))
#define SYS_RESET_EVENT                 BIT(0)
#define SYS_REINIT_EVENT                BIT(4)
#define SYS_WPS_STATUS_CHANGE_EVENT     BIT(5)
#define SYS_WIFI_P2P_BACK2DEV           BIT(18)
#define SYS_WIFI_P2P_START_DHCPD        BIT(19)
//#define SYS_WIFI_P2P_START_DHCPC        BIT(20)
#define SYS_WIFI_LINK_UP_EVENT          BIT(21)
#define SYS_WIFI_LINK_DOWN_EVENT        BIT(22)

cyg_flag_t sys_flag;

///void wsc_event_cb_func_sample(int evt)
///{
///    if (evt == WPS_REINIT_EVENT) {
///        cyg_flag_setbits(&sys_flag, SYS_REINIT_EVENT);
///    }
///    else if (evt == WPS_STATUS_CHANGE_EVENT) {
///        cyg_flag_setbits(&sys_flag, SYS_WPS_STATUS_CHANGE_EVENT);
///    }
///    else
///        printf("unknown event\n");
///}

///void print_wscd_status(int status)
///{
///    switch(status) {
///    case NOT_USED:
///        printf("NOT_USED\n");
///        break;
///    case PROTOCOL_START:
///        printf("PROTOCOL_START\n");
///        break;
///    case PROTOCOL_PBC_OVERLAPPING:
///        printf("PROTOCOL_PBC_OVERLAPPING\n");
///        break;
///    case PROTOCOL_TIMEOUT:
///        printf("PROTOCOL_TIMEOUT\n");
///        break;
///    case PROTOCOL_SUCCESS:
///        printf("PROTOCOL_SUCCESS\n");
///        break;
///    case SEND_EAPOL_START:
///        printf("SEND_EAPOL_START\n");
///        break;
///    case RECV_EAPOL_START:
///        printf("RECV_EAPOL_START\n");
///        break;
///    case SEND_EAP_IDREQ:
///        printf("SEND_EAP_IDREQ\n");
///        break;
///    case RECV_EAP_IDRSP:
///        printf("RECV_EAP_IDRSP\n");
///        break;
///    case SEND_EAP_START:
///        printf("SEND_EAP_START\n");
///        break;
///    case SEND_M1:
///        printf("SEND_M1\n");
///        break;
///    case RECV_M1:
///        printf("RECV_M1\n");
///        break;
///    case SEND_M2:
///        printf("SEND_M2\n");
///        break;
///    case RECV_M2:
///        printf("RECV_M2\n");
///        break;
///    case RECV_M2D:
///        printf("RECV_M2D\n");
///        break;
///    case SEND_M3:
///        printf("SEND_M3\n");
///        break;
///    case RECV_M3:
///        printf("RECV_M3\n");
///        break;
///    case SEND_M4:
///        printf("SEND_M4\n");
///        break;
///    case RECV_M4:
///        printf("RECV_M4\n");
///        break;
///    case SEND_M5:
///        printf("SEND_M5\n");
///        break;
///    case RECV_M5:
///        printf("RECV_M5\n");
///        break;
///    case SEND_M6:
///        printf("SEND_M6\n");
///        break;
///    case RECV_M6:
///        printf("RECV_M6\n");
///        break;
///    case SEND_M7:
///        printf("SEND_M7\n");
///        break;
///    case RECV_M7:
///        printf("RECV_M7\n");
///        break;
///    case SEND_M8:
///        printf("SEND_M8\n");
///        break;
///    case RECV_M8:
///        printf("RECV_M8\n");
///        break;
///    case PROC_EAP_ACK:
///        printf("PROC_EAP_ACK\n");
///        break;
///    case WSC_EAP_FAIL:
///        printf("WSC_EAP_FAIL\n");
///        break;
///    case HASH_FAIL:
///        printf("HASH_FAIL\n");
///        break;
///    case HMAC_FAIL:
///        printf("HMAC_FAIL\n");
///        break;
///    case PWD_AUTH_FAIL:
///        printf("PWD_AUTH_FAIL\n");
///        break;
///    case PROTOCOL_PIN_NUM_ERR:
///        printf("PROTOCOL_PIN_NUM_ERR\n");
///        break;
///    case PROC_EAP_DONE:
///        printf("PROC_EAP_DONE\n");
///        break;
///    case PROTOCOL_ERR:
///        printf("PROTOCOL_ERR\n");
///        break;
///    default:
///        printf("Unknown wscd status\n");
///        break;
///    }
///}

void wifi_link_status_cb_func_sample(char *pIntfName, int status)
{
    if (status == WIFI_LINK_STATUS_CONNECTED) {
        printf("wlan0: Connected with AP\n");
        cyg_flag_setbits(&sys_flag, SYS_WIFI_LINK_UP_EVENT);
    }
    else if (status == WIFI_LINK_STATUS_DISCONNECTED) {
        printf("wlan0: Disconnected with AP \n");
        cyg_flag_setbits(&sys_flag, SYS_WIFI_LINK_DOWN_EVENT);
    }
    else if (status == WIFI_LINK_STATUS_AUTH_FAIL) {
        printf("wlan0: Fail to authenticate with AP \n");
    }
    else if (status == WIFI_LINK_STATUS_DISASSOCIATED) {
        printf("wlan0: Disassociated by AP \n");
    }
    else if (status == WIFI_LINK_STATUS_DEAUTHENTICATED) {
        printf("wlan0: Deauthenticated by AP\n");
    }
    else if (status == WIFI_LINK_STATUS_HANDSHAKE_TIMEOUT) {
        printf("wlan0: handshake timeout\n");
    }
    else
        printf("%s: unknown status : %d \n", __func__, status);
}

void wifi_sta_status_cb_func_sample(char *pIntfName, char *pMacAddr, int status)
{
    printf("%s: status:%d \n", __func__, status);

    if (status == WIFI_STA_STATUS_ASSOCIATED) {
        printf("wlan0: A client(%02X:%02X:%02X:%02X:%02X:%02X) is associated\n",
            *pMacAddr, *(pMacAddr+1), *(pMacAddr+2), *(pMacAddr+3), *(pMacAddr+4), *(pMacAddr+5));
    }
    else if (status == WIFI_STA_STATUS_REASSOCIATED) {
        printf("wlan0: A client(%02X:%02X:%02X:%02X:%02X:%02X) is reassociated\n",
            *pMacAddr, *(pMacAddr+1), *(pMacAddr+2), *(pMacAddr+3), *(pMacAddr+4), *(pMacAddr+5));
    }
    else if (status == WIFI_STA_STATUS_DISASSOCIATED) {
        printf("wlan0: A client(%02X:%02X:%02X:%02X:%02X:%02X) is disassociated\n",
            *pMacAddr, *(pMacAddr+1), *(pMacAddr+2), *(pMacAddr+3), *(pMacAddr+4), *(pMacAddr+5));
    }
    else if (status == WIFI_STA_STATUS_DEAUTHENTICATED) {
        printf("wlan0: A client(%02X:%02X:%02X:%02X:%02X:%02X) is deauthenticated\n",
            *pMacAddr, *(pMacAddr+1), *(pMacAddr+2), *(pMacAddr+3), *(pMacAddr+4), *(pMacAddr+5));
    }
    else if (status == WIFI_STA_STATUS_PORT_AUTHORIZED) {
        printf("wlan0: Port for a client(%02X:%02X:%02X:%02X:%02X:%02X) is in an authorized state and the client can access network now\n",
            *pMacAddr, *(pMacAddr+1), *(pMacAddr+2), *(pMacAddr+3), *(pMacAddr+4), *(pMacAddr+5));
    }
    else if (status == WIFI_STA_STATUS_AUTH_FAIL) {
        printf("wlan0: A client(%02X:%02X:%02X:%02X:%02X:%02X) authentication fail\n",
            *pMacAddr, *(pMacAddr+1), *(pMacAddr+2), *(pMacAddr+3), *(pMacAddr+4), *(pMacAddr+5));
    }
    else if (status == WIFI_STA_STATUS_EXPIRED) {
        printf("wlan0: A client(%02X:%02X:%02X:%02X:%02X:%02X) is expired\n",
            *pMacAddr, *(pMacAddr+1), *(pMacAddr+2), *(pMacAddr+3), *(pMacAddr+4), *(pMacAddr+5));
    }
	else if (status == WIFI_AP_READY) {
		printf("wlan0: AP is ready \n", pIntfName);
	}
    else
        printf("%s: unknown status: %d \n", __func__, status);
}

int delete_interface_addr(char *intf)
{
    struct ifreq ifr;
    //20180104 nash: coverity fix, CID#109503
    //int s=-1;
    int s;
    int retcode = -1;

    s = socket(AF_INET, SOCK_DGRAM, 0);
    if (s < 0) {
        perror("socket");
        goto out;
    }

    //delete previous address first
    //20180104 nash: coverity fix, CID#109428
    //strcpy(ifr.ifr_name, intf);
    strncpy(ifr.ifr_name, intf, sizeof(ifr.ifr_name) - 1);
    if (ioctl(s, SIOCDIFADDR, &ifr) < 0) {
        //20180119 nash:
        //if (errno == EADDRNOTAVAIL) {
            /* means no previous address for interface */
        //} else {
            perror("SIOCDIFADDR");
            goto out;
        //}
    }
    retcode = 0;

 out:
    if (s != -1)
      close(s);
    return retcode;
}

void aaa_cb(void)
{
    printf("assert cb!!\r\n");
}

void sample_sys_main(cyg_addrword_t data)
{
    cyg_flag_value_t flag_val;
    //20171020 nash: unused
    //char macaddr[6];

    cyg_flag_init(&sys_flag);

    memset(&wifi_settings_sample, 0, sizeof(struct wifi_settings_sample));
    //init_wifi_settings("sta", "none"); //for test purpose
    init_wifi_settings("ap", "none"); //for test purpose
    //init_wifi_settings("ap", "none");
    //wifi_settings_sample.wlan_disable = 1;
    //wifi_settings_sample.wps_disable = 1;
    //wifi_settings_sample.is_configured = 1;

///    // Generate WPS PIN number
///    generate_pin_code(wifi_settings_sample.wsc_pin);
///    //strcpy(wifi_settings_sample.wsc_pin,"11223344");
///    //strcpy(wifi_settings_sample.wsc_pin,"12345670");
///    //printf("wifi_settings_sample.wsc_pin=%s\n", wifi_settings_sample.wsc_pin);

    //configure_wifi_sample(&wifi_settings_sample);

///
///    register_wsc_flash_param_cb(wsc_flash_param_handler_sample);
///    register_wsc_event_cb(wsc_event_cb_func_sample);

	printf("%s: Register WPA CB \n", __func__);

    //register link status cb (STA mode)
	if(register_wifi_wpa_link_status_cb("wlan0", wifi_link_status_cb_func_sample))
		 printf("%s: register wpa_link_status_cb fail \n",__func__);


    //register sta status cb (AP mode)
    //register_wifi_sta_status_cb("wlan0", wifi_sta_status_cb_func_sample);
    if(register_wifi_wpa_sta_status_cb("wlan0", wifi_sta_status_cb_func_sample))
         printf("%s: register wpa_sta_status_cb fail \n",__func__);


    //register assert cb
///    cyg_assert_install_callback(aaa_cb);


    // mac address setting
///    if (wifi_get_wlan0_efuse_mac(macaddr) < 0) {
///        printf("wifi_get_wlan0_efuse_mac() fail. Use hardcoded mac.\n");
///        set_mac_address("wlan0", "\x00\x00\x00\x81\x89\xe5");
///    }
///    else {
///        unsigned char zero[] = {0, 0, 0, 0, 0, 0};
///
///        printf("wlan0 efuse mac [%02x:%02x:%02x:%02x:%02x:%02x]\n",
///            macaddr[0], macaddr[1], macaddr[2],
///            macaddr[3], macaddr[4], macaddr[5]);
///        //sanity check
///        if (!memcmp(macaddr, zero, MACADDRLEN) || IS_MCAST(macaddr)) {
///            printf("efuse mac format is invalid. Use hardcoded mac.\n");
///            set_mac_address("wlan0", "\x00\x00\x00\x81\x89\xe5");
///        }
///        else {
///            set_mac_address("wlan0", macaddr);
///        }
///    }
///
///    // ip address setting
///    if (strcmp(wifi_settings_sample.mode, "ap") == 0) {
///        interface_config("wlan0", "192.168.1.254", "255.255.255.0");
///        //interface_config2("wlan0", "192.168.1.254", "255.255.255.0");
///    }
///    else if (strcmp(wifi_settings_sample.mode, "sta") == 0) {
///        if (wifi_settings_sample.is_dhcp) {
///		delete_interface_addr("wlan0");
///		interface_up("wlan0");
///        }
///        else {
///        	interface_config("wlan0", "192.168.1.189", "255.255.255.0");
///        	//interface_config2("wlan0", "192.168.1.189", "255.255.255.0");
///        }
///    }

    //wps
///    if (!wifi_settings_sample.wlan_disable) {
///        if (!wifi_settings_sample.wps_disable) {
///            create_wscd();
///        }
///    }

    while (1) {
        flag_val = cyg_flag_wait(&sys_flag, 0xffffffff, CYG_FLAG_WAITMODE_OR | CYG_FLAG_WAITMODE_CLR);
        //printf("sys_flag=%x\n", flag_val);
        if (flag_val & SYS_RESET_EVENT) {
            //reboot
            HAL_PLATFORM_RESET();
        }

        if (flag_val & SYS_REINIT_EVENT) {
///            cyg_thread_info tinfo;

            //diag_printf("%s %d\n", __FUNCTION__, __LINE__);
            //receive event from wsc
            //printf("system reinit\n");
            //reinit wlan
            configure_wifi_sample(&wifi_settings_sample);

            // ip address setting
            if (strcmp(wifi_settings_sample.mode, "ap") == 0) {
                //RunNvtCmd("ifconfig wlan0 192.168.1.254 netmask 255.255.255.0");
///                interface_config("wlan0", "192.168.1.254", "255.255.255.0");
                //interface_config2("wlan0", "192.168.1.254", "255.255.255.0");
            }
///            else if (strcmp(wifi_settings_sample.mode, "sta") == 0) {
///            	if (wifi_settings_sample.is_dhcp) {
///			delete_interface_addr("wlan0");
///			interface_up("wlan0");
///        	}
///        	else {
///                	interface_config("wlan0", "192.168.1.189", "255.255.255.0");
///                	//interface_config2("wlan0", "192.168.1.189", "255.255.255.0");
///                }
///            }

//#ifdef HAVE_WPS
///            if (get_thread_info_by_name("wscd", &tinfo)) {
///                if (!wifi_settings_sample.wlan_disable && !wifi_settings_sample.wps_disable)
///                    wsc_reinit(); //trigger wsc to reinit
///                else
///                    wsc_stop_service(); //trigger wsc to stop service
///            }
///            else {
///                if (!wifi_settings_sample.wlan_disable && !wifi_settings_sample.wps_disable)
///                    create_wscd();
///            }
//#endif
        }
///        if (flag_val & SYS_WPS_STATUS_CHANGE_EVENT) {
///            int status;
///            status = get_wscd_status();
///            printf("[%d] ", status);
///            print_wscd_status(status);
///        }
        if (flag_val & SYS_WIFI_LINK_UP_EVENT) {
///            show_bss_info("wlan0");
            if (((strcmp(wifi_settings_sample.mode, "sta") == 0) && wifi_settings_sample.is_dhcp)
            	) {
                printf("Starting DHCPC...\n");
            }
        }

        if (flag_val & SYS_WIFI_LINK_DOWN_EVENT) {
                printf(" SYS_WIFI_LINK_DOWN_EVENT\n");
///            show_bss_info("wlan0");
        }
///        else {
///            printf("invalid sys_flag value: 0x%x\n", flag_val);
///        }
    }
}

void create_sample_sys_thread(void)
{

    if(ntk_testcmd_initialized != 0)
         return;

    ntk_testcmd_initialized = 1;

    /* Create the thread */
    cyg_thread_create(8,
              sample_sys_main,
              0,
              "sample_sys",
              &sample_sys_stack,
              sizeof(sample_sys_stack),
              &sample_sys_thread_handle,
              &sample_sys_thread_obj);
    /* Let the thread run when the scheduler starts */
    cyg_thread_resume(sample_sys_thread_handle);
}


//-----------------------------------------------------------------------------
// wifi test command
//-----------------------------------------------------------------------------
#if 0
MAINFUNC_ENTRY(wifi,argc,argv)
{
    if (argc < 2)
        goto show_help;

    if (argc == 2) {
        if (strcmp(argv[1], "init") == 0) {
            //nash:debug
            sdio_host_set_config(SDIODEV_CONFIG_ID_BUS_SPEED_MODE, SDIODEV_BUS_SPEED_MODE_DS);
            //nash:debug:end
            if (wifi_init_wlan0_netdev(0) < 0)
                printf("wifi_init_wlan0_netdev() fail\n");
            else {
                printf("wifi_init_wlan0_netdev() success\n");
                create_sample_sys_thread();
            }
            return 0;
        }
        else if (strcmp(argv[1], "associated_clients") == 0) {
            show_associated_clients("wlan0");
            return 0;
        }
        else if (strcmp(argv[1], "site_survey") == 0) {
            do_site_survey("wlan0");
            return 0;
        }
///        else if (strcmp(argv[1], "bss_info") == 0) {
///            show_bss_info("wlan0");
///            return 0;
///        }
        else if (strcmp(argv[1], "settings") == 0) {
            dump_wifi_settings();
            return 0;
        }
        else if (strcmp(argv[1], "wlan_enable") == 0) {
            wifi_settings_sample.wlan_disable = 0;
            return 0;
        }
        else if (strcmp(argv[1], "wlan_disable") == 0) {
            wifi_settings_sample.wlan_disable = 1;
            return 0;
        }
        else if (strcmp(argv[1], "wps_enable") == 0) {
            wifi_settings_sample.wps_disable = 0;
            return 0;
        }
        else if (strcmp(argv[1], "wps_disable") == 0) {
            wifi_settings_sample.wps_disable = 1;
            return 0;
        }
        else {
            goto show_help;
        }
    }
    else if (argc == 3) {
        //20170921 nash: TBD
        #if 0
        if (strcmp(argv[1], "join") == 0) {
            unsigned char res;
            int idx = atoi(argv[2]);
            if ((idx < 0) || (idx >= ss_status.number)) {
                printf("wifi join <0~%d>\n", (ss_status.number-1));
            }
            else {
                // TBD, configure wifi security before sending out "join request"

                if (join_bss_request("wlan0", &ss_status.bssdb[idx], &res) < 0)
                    printf("Join request failed!\n");
                else {
                    if (res == 0) {
                        printf("On going to connect with AP!\n");
                        print_bss_desc(&ss_status.bssdb[idx]);
                    }
                    else if (res == 1) {
                        printf("Site survey is still on-going!\n");
                    }
                    else if (res == 2) {
                        printf("Fail to connect with AP!\n");
                    }
                }
            }
            return 0;
        }
        #endif
        //20170921 nash:end
    }

    //AP mode
    if ((strcmp(argv[1], "ap") == 0) || (strcmp(argv[1], "sta") == 0)) {
        if ((strcmp(argv[2], "none") == 0) ||
            (strcmp(argv[2], "wep64-auto") == 0) ||
            (strcmp(argv[2], "wep128-auto") == 0) ||
            (strcmp(argv[2], "wpa-psk-tkip") == 0) ||
            (strcmp(argv[2], "wpa2-psk-aes") == 0) ||
            (strcmp(argv[2], "wpa-auto-psk-tkipaes") == 0)) {
            if (strcmp(argv[1], "sta") == 0) {
                if (argc==4) {
                    if (strcmp(argv[3], "dhcp") == 0)
                        wifi_settings_sample.is_dhcp = 1; //dhcp client
                    else
                        wifi_settings_sample.is_dhcp = 0; //static ip
                }
                else
                    wifi_settings_sample.is_dhcp = 0; //static ip
            }
            init_wifi_settings(argv[1], argv[2]);
            cyg_flag_setbits(&sys_flag, SYS_REINIT_EVENT);
            return 0;
        }
    }

show_help:
    printf("wifi init\n");
    printf("wifi settings\n");
    printf("wifi wlan_enable\n");
    printf("wifi wlan_disable\n");
    printf("wifi wps_enable\n");
    printf("wifi wps_disable\n");
    printf("wifi ap none\n");
    printf("wifi ap wep64-auto\n");
    printf("wifi ap wep128-auto\n");
    printf("wifi ap wpa-psk-tkip\n");
    printf("wifi ap wpa2-psk-aes\n");
    printf("wifi ap wpa-auto-psk-tkipaes\n");
    printf("wifi sta none\n");
    printf("wifi sta wep64-auto\n");
    printf("wifi sta wep128-auto\n");
    printf("wifi sta wpa2-psk-aes\n");
    printf("wifi sta none dhcp\n");
    printf("wifi sta wep64-auto dhcp\n");
    printf("wifi sta wep128-auto dhcp\n");
    printf("wifi sta wpa2-psk-aes dhcp\n");
    printf("wifi associated_clients\n");
    printf("wifi site_survey\n");
    //20170921 nash: TBD
    //printf("wifi join <0~%d>\n", (MAX_BSS_DESC-1));
    //20170921 nash:end
    printf("wifi bss_info\n");

    return 0;
}
#endif

#endif//#if 0

/**
 * \brief wlan cli command handler \n
 *
 * \param [in] argc: number of args
 * \param [in] args: arg pos32s
 *
 * \return 0: success
 * \return 1: fail
 */
MAINFUNC_ENTRY(wifi,argc,argv)
{
    int i;
    int unknow_cmd=1;

    if (argc < 2)
    {
        do_wifi_help((int)argc, (char**)argv);
        return 0;
    }

    for(i=0; wifi_cmds[i].cmd_func!=NULL; i++)
    {
        if(strcmp(argv[1], wifi_cmds[i].name) == 0)
        {
            unknow_cmd = 0;
            wifi_cmds[i].cmd_func(argc-1, &argv[1]);
            break;
        }
    }

    if(unknow_cmd == 1)
    {
        printf( "wlan0: unknown sub-command (%s)\n", argv[1]);
        do_wifi_help(argc, argv);
    }

    return 0;
}

static int do_wifi_help( int argc, char *argv[] )
{
    unsigned int i;

    for(i=0; wifi_cmds[i].cmd_func!=NULL; i++)
    {
        printf ("wifi %s\n", wifi_cmds[i].name, wifi_cmds[i].desc );
    }
    printf ("\n");
    return 0;
}

//20170828 nash:
#if 1
static int do_wifi_init_cmd( int argc, char *argv[] )
{
    int result;

    result = wlan_init_netdev(0, false, true, false, true);
    if(result < 0) {
         printf("wlan_init_netdev() fail\n");
         return 0;
    }

    //enable wlan0 here,wpa_supplicant need if to init wifi
    //RunNvtCmd("ifconfig wlan0 up");

    nvt_wpa_supplicant_init(false);

    //create wifi configuration thread
    create_sample_sys_thread();
	printf("[ATD] init process finished \n");
    return 0;
}

static int do_wifi_setting_cmd( int argc, char *argv[] )
{
    if(argc >= 3)
    {
        if(strcmp (argv[1], "ssid") == 0) {
            memset(wifi_settings_sample.ssid, 0, sizeof(wifi_settings_sample.ssid));
            strncpy(wifi_settings_sample.ssid, argv[2], sizeof(wifi_settings_sample.ssid)-1 );
        }
        else if (strcmp (argv[1], "passphrase") == 0) {
            memset(wifi_settings_sample.passphrase, 0, sizeof(wifi_settings_sample.passphrase));
            strncpy(wifi_settings_sample.passphrase, argv[2], sizeof(wifi_settings_sample.passphrase)-1 );
        }
        else
            ;
    }
    dump_wifi_settings();
    return 0;
}

static int do_wifi_wps_cmd( int argc, char *argv[] )
{
    return 0;
}

static int do_wifi_sta_cmd( int argc, char *argv[] )
{
    int new_wifi_cfg = 0;
    const WIFI_SECURITY_MODE_T *p_security_entry;

    p_security_entry = wifi_security_tbl;

    if(argc < 2) {
        printf(" wifi STA sub-cmd:\n");
        while(p_security_entry->str_security_mode != NULL) {
            if(p_security_entry->support_mode & WIFI_MODE_STA)
                printf("  %s\n", p_security_entry->str_security_mode);
            p_security_entry++;
        }
        printf("  site_survey\n");
        return 0;
    }

    while(p_security_entry->str_security_mode != NULL) {
        if (strcmp(argv[1], p_security_entry->str_security_mode) != 0) {
            p_security_entry++;
            continue;
        }
        if(p_security_entry->support_mode & WIFI_MODE_STA) {//valid security mode
            new_wifi_cfg = 1;
            break;
        }
        p_security_entry++;
    }

    if(new_wifi_cfg){
        printf("change to wifi mode:%s-%s\n", argv[0], argv[1]);
        init_wifi_settings(argv[0], argv[1]);
        cyg_flag_setbits(&sys_flag, SYS_REINIT_EVENT);
        return 0;
    }

    if(strcmp(argv[1],"site_survey") == 0){
        RunNvtCmd("wpa_cli scan_results");
    }
 	  else if (strcmp(argv[1], "start")==0){
		    start_wifi_settings(argv[0], gauth);
		    cyg_flag_setbits(&sys_flag, SYS_REINIT_EVENT);
		    return 0;
	  }
    else {
       printf ("unsupported sub-smd\n");
    }
    return 0;
}

static int do_wifi_ap_cmd( int argc, char *argv[] )
{

    int new_wifi_cfg = 0;
    const WIFI_SECURITY_MODE_T *p_security_entry;

    p_security_entry = wifi_security_tbl;

    if(argc < 2) {
        printf(" wifi AP sub-cmd:\n");
        while(p_security_entry->str_security_mode != NULL) {
            if(p_security_entry->support_mode & WIFI_MODE_AP)
                printf("  %s\n", p_security_entry->str_security_mode);
            p_security_entry++;
        }
        return 0;
    }

    while(p_security_entry->str_security_mode != NULL) {
        if (strcmp(argv[1], p_security_entry->str_security_mode) != 0) {
            p_security_entry++;
            continue;
        }
        if(p_security_entry->support_mode & WIFI_MODE_AP) {//valid security mode
            new_wifi_cfg = 1;
            break;
        }
        p_security_entry++;
    }

    if(new_wifi_cfg){
        printf("change to wifi mode:%s-%s\n", argv[0], argv[1]);
        init_wifi_settings(argv[0], argv[1]);
        cyg_flag_setbits(&sys_flag, SYS_REINIT_EVENT);
    }
 	  else if (strcmp(argv[1], "start")==0){
		    start_wifi_settings(argv[0], gauth);
		    cyg_flag_setbits(&sys_flag, SYS_REINIT_EVENT);
		    return 0;
	  }
    else {
        printf ("unsupported sub-smd\n");
    }

    return 0;
}

static int do_wifi_status_cmd(int argc, char *argv[]){

    char tmpbuf[256];

    sprintf(tmpbuf, "nvtwsupp cli nvt_conn_status");
    RunNvtCmd(tmpbuf);

	return 0;

}

static int do_wifi_set_cmd(int argc, char *argv[]){
    int i=0;
    int ch;
    int wep_idx;
    int supported_ch=0;

    printf("%s: argc:%d argv[0]:%s \n", __func__, argc, argv[0] );

	  if (argc < 3) {
        printf("Please set parameter: ssid / auth / psk / wepkey / wepkey_idx / chan \n");
        return 0;
	  }

    if (strcmp(argv[0], "set") == 0) {
       if(strcmp(argv[1],"ssid")==0 ){
           strcpy((char *)gssid, argv[2]);
           printf("[ATD] ssid=%s\n", gssid);
           return 0;
       }
       else if(strcmp(argv[1],"auth")==0){
           const WIFI_SECURITY_MODE_T *p_wifi_security_cfg = wifi_security_tbl;
           int valid=0;

           while( p_wifi_security_cfg->str_security_mode != NULL){
                if (strcmp(argv[2], p_wifi_security_cfg->str_security_mode) != 0) {
                     p_wifi_security_cfg++;
                     continue;
                }
                valid = 1;
                break;
           }
           if (valid == 1){
                strcpy((char *)gauth, argv[2]);
                printf("[ATD] auth=%s\n", gauth);
           }
           else
                printf("[ATD] unsupport auth:%s\n", argv[2]);

           return 0;
       }
       else if(strcmp(argv[1],"psk")==0){
	         char * buf = argv[2];
           int len;
           if ( *buf== '"') {
              const char *pos;
              buf++;
              pos = strrchr(buf, '"');
              if (pos)
                  len = pos - buf;
              else
                  len = strlen(buf);

              if (len < 8 || len > 63) {
                  printf("Invalid passphrase length %lu (expected: 8..63) '%s'. \n",
                               (unsigned long) len, buf);
                  return -1;
              }
              is_hex_psk =0;
              strncpy((char *)gpsk, buf, len);
              gpsk[len]=0;
           }
           else{
              len = strlen(buf);
              if(len!=64){
                 printf("Invalid PSK '%s' (hex len:%d) \n", buf, len);
                 return -1;
              }
              is_hex_psk =1;
              strcpy((char *)gpsk, buf);
           }
           printf("[ATD] psk=%s\n", gpsk);
           return 0;
       }
       else if(strcmp(argv[1],"wepkey")==0){
           char * buf = argv[2];
           int len;
           if ( *buf== '"') {
              const char *pos;
              buf++;
              pos = strrchr(buf, '"');
              if (pos)
                  len = pos - buf;
              else
                  len = strlen(buf);

              if (len && len != 5 && len != 13 && len != 16) {
                  printf("Invalid WEP ASCII key length %d. (expected: 5 || 13 || 16) \n", len);
                  return -1;
              }
              is_hex_psk =0;
              strncpy((char *)wep_key, buf, len);
              wep_key[len]=0;
           }
           else{
              len = strlen(buf);
              if (len && len != 10 && len != 26 && len != 32) {
                  printf("Invalid WEP hex key len %d (expected: 10 || 26 || 32) \n", len);
                  return -1;
              }
              is_hex_psk =1;
              strcpy((char *)wep_key, buf);
           }
           printf("[ATD] wepkey=%s\n", wep_key);
           return 0;
       }
       else if(strcmp(argv[1],"wepkey_idx")==0){
           wep_idx = atoi(argv[2]);
  	       if(wep_idx>3 || wep_idx<0 ){
               printf("Invalid wep key index (expected: 0~3) \n");
               return -1;
           }
           wep_key_idx = wep_idx;
           printf("[ATD] wepkey_idx=%d\n", wep_key_idx);
           return 0;
       }
       else if(strcmp(argv[1],"chan")==0){

           ch = atoi(argv[2]);

           #if (ECOS_BAND_SEL_5G == 0)
           if (0 < ch && ch < 14)
           {
               gchan = ch;
               supported_ch=1;
           }
           #else
           {
               for(i=0; i<chan_5G_index; i++)
               {
                   if(ch==chan_5G_list[i]){
                      gchan = ch;
                      supported_ch=1;
                      break;
                  }
               }
           }
           #endif

           if(supported_ch)
           {
       	       printf("[ATD] channel=%d \n", ch);
           }
           else
           {
               printf("[ATD] invalid channel: %d \n", ch);
               printf("please enter below channel: \n");
               #if (ECOS_BAND_SEL_5G == 0)
               printf("2G: ");
               for(i=0; i<chan_2G_index; i++)
               {
                  printf("%d ", chan_2G_list[i]);
               }
               printf("\n");
               #else
               printf("5G: ");
               for(i=0; i<chan_5G_index; i++)
               {
                   printf("%d ", chan_5G_list[i]);
               }
               printf("\n");
               #endif
           }
           return 0;
       }
	}

    return 0;
}

static int do_wifi_sta_clients_cmd(int argc, char *argv[]){
	char tmpbuf[256];
    //20171020 nash: unused
	//WIFI_SECURITY_MODE_T *p_wifi_security_cfg = wifi_security_tbl;

	sprintf(tmpbuf, "nvtwsupp cli STA_LIST");
	RunNvtCmd(tmpbuf);

    return 0;
}
#endif

extern int NT18211_GPIO_config(void)
{
    cyg_uint32 reg;

    //set pinmux
    HAL_READ_UINT32(0xB0010014, reg);
    reg &= ~(0x000C0003);
    HAL_WRITE_UINT32(0xB0010014, reg);

    HAL_READ_UINT32(0xB001001C, reg);
    reg &= ~(0x00000003);
    HAL_WRITE_UINT32(0xB001001C, reg);


    //set pin output direction
    HAL_READ_UINT32(0xB0070028, reg);
    reg |= (0x001E0003);
    HAL_WRITE_UINT32(0xB0070028, reg);

    HAL_READ_UINT32(0xB007002C, reg);
    reg |= (0x00000030);
    HAL_WRITE_UINT32(0xB007002C, reg);


    //set pins output voltage
    //set GPIO pin output high
    HAL_READ_UINT32(0xB0070068, reg);
    reg &= ~(0x00160000);
    HAL_WRITE_UINT32(0xB0070068, reg);

    HAL_READ_UINT32(0xB0070048, reg);
    reg |= (0x00160000);
    HAL_WRITE_UINT32(0xB0070048, reg);


    //set GPIO pin output low
    HAL_READ_UINT32(0xB0070048, reg);
    reg &= ~(0x00080003);
    HAL_WRITE_UINT32(0xB0070048, reg);

    HAL_READ_UINT32(0xB0070068, reg);
    reg |= (0x00080003);
    HAL_WRITE_UINT32(0xB0070068, reg);

    //set WiFi_RST to low
    HAL_READ_UINT32(0xB007004C, reg);
    reg &= ~(0x00000020);
    HAL_WRITE_UINT32(0xB007004C, reg);

    HAL_READ_UINT32(0xB007006C, reg);
    reg |= (0x00000020);
    HAL_WRITE_UINT32(0xB007006C, reg);

    //power on
    //WiFi_ON to high
    HAL_READ_UINT32(0xB007006C, reg);
    reg &= ~(0x00000010);
    HAL_WRITE_UINT32(0xB007006C, reg);

    HAL_READ_UINT32(0xB007004C, reg);
    reg |= (0x00000010);
    HAL_WRITE_UINT32(0xB007004C, reg);

    Delay_DelayMs(20);

    //set WiFi_RST to high
    HAL_READ_UINT32(0xB007006C, reg);
    reg &= ~(0x00000020);
    HAL_WRITE_UINT32(0xB007006C, reg);

    HAL_READ_UINT32(0xB007004C, reg);
    reg |= (0x00000020);
    HAL_WRITE_UINT32(0xB007004C, reg);

    return 0;
}

//20170828 nash: for nt18211 wifi on/off
static int do_wifi_power_on_cmd( int argc, char *argv[] )
{
    cyg_uint32 reg;

    //set pinmux
    HAL_READ_UINT32(0xB0010014, reg);
    reg &= ~(0x000C0003);
    HAL_WRITE_UINT32(0xB0010014, reg);

    HAL_READ_UINT32(0xB001001C, reg);
    reg &= ~(0x00000003);
    HAL_WRITE_UINT32(0xB001001C, reg);


    //set pin output direction
    HAL_READ_UINT32(0xB0070028, reg);
    reg |= (0x001E0003);
    HAL_WRITE_UINT32(0xB0070028, reg);

    HAL_READ_UINT32(0xB007002C, reg);
    reg |= (0x00000030);
    HAL_WRITE_UINT32(0xB007002C, reg);


    //set pins output voltage
    //set GPIO pin output high
    HAL_READ_UINT32(0xB0070068, reg);
    reg &= ~(0x00160000);
    HAL_WRITE_UINT32(0xB0070068, reg);

    HAL_READ_UINT32(0xB0070048, reg);
    reg |= (0x00160000);
    HAL_WRITE_UINT32(0xB0070048, reg);


    //set GPIO pin output low
    HAL_READ_UINT32(0xB0070048, reg);
    reg &= ~(0x00080003);
    HAL_WRITE_UINT32(0xB0070048, reg);

    HAL_READ_UINT32(0xB0070068, reg);
    reg |= (0x00080003);
    HAL_WRITE_UINT32(0xB0070068, reg);


    //power on SDIO power
    HAL_READ_UINT32(0xB0010004, reg);
    reg |= (0x00008002);
    HAL_WRITE_UINT32(0xB0010004, reg);

    HAL_READ_UINT32(0xB0030004, reg);
    reg &= 0xFF000FFF;
    reg |= 0x00AA9000;
    HAL_WRITE_UINT32(0xB0030004, reg);


    //power on
    //WiFi_ON to high
    HAL_READ_UINT32(0xB007006C, reg);
    reg &= ~(0x00000010);
    HAL_WRITE_UINT32(0xB007006C, reg);

    HAL_READ_UINT32(0xB007004C, reg);
    reg |= (0x00000010);
    HAL_WRITE_UINT32(0xB007004C, reg);

    Delay_DelayMs(20);

    //set WiFi_RST to high
    HAL_READ_UINT32(0xB007006C, reg);
    reg &= ~(0x00000020);
    HAL_WRITE_UINT32(0xB007006C, reg);

    HAL_READ_UINT32(0xB007004C, reg);
    reg |= (0x00000020);
    HAL_WRITE_UINT32(0xB007004C, reg);

    printf("power on ok\n");

    return 0;
}

static int do_wifi_power_down_cmd( int argc, char *argv[] )
{
    cyg_uint32 reg;

    //RunNvtCmd("nvt fwboot 0");
    //Delay_DelayMs(100);

    //set pinmux
    HAL_READ_UINT32(0xB0010014, reg);
    reg &= ~(0x000C0003);
    HAL_WRITE_UINT32(0xB0010014, reg);

    HAL_READ_UINT32(0xB001001C, reg);
    reg &= ~(0x00000003);
    HAL_WRITE_UINT32(0xB001001C, reg);


    //set pin output direction
    HAL_READ_UINT32(0xB0070028, reg);
    reg |= (0x001E0003);
    HAL_WRITE_UINT32(0xB0070028, reg);

    HAL_READ_UINT32(0xB007002C, reg);
    reg |= (0x00000030);
    HAL_WRITE_UINT32(0xB007002C, reg);


    //set pins output voltage
    //set CLR to 1
    HAL_READ_UINT32(0xB0070048, reg);
    reg &= ~(0x001E0003);
    HAL_WRITE_UINT32(0xB0070048, reg);

    HAL_READ_UINT32(0xB0070068, reg);
    reg |= (0x001E0003);
    HAL_WRITE_UINT32(0xB0070068, reg);

    //WiFi_RST to be low
    HAL_READ_UINT32(0xB007004C, reg);
    reg &= ~(0x00000020);
    HAL_WRITE_UINT32(0xB007004C, reg);

    HAL_READ_UINT32(0xB007006C, reg);
    reg |= (0x00000020);
    HAL_WRITE_UINT32(0xB007006C, reg);


    //power off SDIO power
    HAL_READ_UINT32(0xB0010004, reg);
    reg &= ~(0x00008004);
    HAL_WRITE_UINT32(0xB0010004, reg);

    HAL_READ_UINT32(0xB0030004, reg);
    reg &= 0xFF000FFF;
    reg |= 0x00555000;
    HAL_WRITE_UINT32(0xB0030004, reg);

    //WiFi_ON to be low
    HAL_READ_UINT32(0xB007004C, reg);
    reg &= ~(0x00000010);
    HAL_WRITE_UINT32(0xB007004C, reg);

    HAL_READ_UINT32(0xB007006C, reg);
    reg |= (0x00000010);
    HAL_WRITE_UINT32(0xB007006C, reg);

    printf("power down ok\n");

    return 0;
}

#endif //#if (defined (__ECOS))
