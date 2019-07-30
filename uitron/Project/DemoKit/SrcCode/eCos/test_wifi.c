#if (defined (__ECOS))

#include <network.h>
//#include <pkgconf/devs_eth_rltk_819x_wrapper.h>
//#include <pkgconf/devs_eth_rltk_819x_wlan.h>
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

#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <cyg/infra/mainfunc.h>
//#include <cyg/io/sdio_linux.h>

#include <cyg/io/eth/rltk/819x/wlan/wifi_api.h>
#include <cyg/io/eth/rltk/819x/wlan/ieee802_mib.h>
#include <cyg/io/eth/rltk/819x/wrapper/wireless.h>
#include "net_api.h"
#include "wps_api.h"
#include <net/dhcpelios/dhcpelios.h>
#include <dhcp.h>

#define ECOS_BAND_SEL_5G 0
#define ECOS_SET_CRYSTAL 0

#if 0
#define TESTDEBUG(fmt, args...) printf("[%s %d]"fmt,__func__,__LINE__,## args)
#else
#define TESTDEBUG(fmt, args...) {}
#endif

#define MAX_BSS_DESC    64
#define MAX_STA_NUM 64  // max support sta number

/* wlan driver ioctl id */
#define SIOCGIWRTLSTAINFO           0x8B30  // get station table information
#define SIOCGIWRTLSTANUM        0x8B31  // get the number of stations in table
#define SIOCGIWRTLSCANREQ       0x8B33  // scan request
#define SIOCGIWRTLGETBSSDB      0x8B34  // get bss data base
#define SIOCGIWRTLJOINREQ       0x8B35  // join request
#define SIOCGIWRTLJOINREQSTATUS     0x8B36  // get status of join request
#define SIOCGIWRTLGETBSSINFO        0x8B37  // get currnet bss info
#define SIOCGIWRTLGETWDSINFO        0x8B38
#define SIOCGMISCDATA           0x8B48  // get misc data

#define RTL8192CD_IOCTL_SET_MIB     0x89F1  //(SIOCDEVPRIVATE + 0x1)
#define RTL8192CD_IOCTL_GET_MIB     0x89F2  //(SIOCDEVPRIVATE + 0x2)


//#ifndef CONFIG_RTL_P2P_SUPPORT
//#error "EXCEPTION"
//#endif

//#ifdef CONFIG_RTL_P2P_SUPPORT
//20171016 nash: config P2P support here
//#define P2P_SUPPORT
//#endif



enum PHY_BAND_SELECT {
    _PHY_BAND_2G = 1,
    _PHY_BAND_5G = 2
};

#define ECOS_DHCP      1

static int rtk_testcmd_initialized = 0;

//==============p2p related define==========================
#ifdef P2P_SUPPORT
typedef struct __p2p_state_event{
    unsigned char  p2p_status;
    unsigned char  p2p_event;
    unsigned short p2p_wsc_method;
    unsigned char  p2p_role;
} P2P_SS_STATUS_T, *P2P_SS_STATUS_Tp;

#define SIOCP2PCMD      0x8BD1  // command for p2p
#define SIOCP2PSCANREQ      0x8BD2  // issue p2p discovery request
#define SIOCP2PGETRESULT    0x8BD3  // report p2p discovery result
#define SIOCP2PPROVREQ      0x8BD4  // active issue provision discovery request to target p2p device
#define SIOCP2WSCMETHODCONF 0x8BD5  // report event and state
#define SIOCP2PPGETEVNIND   0x8BD6  // report event and state

#define SIOCP2P_WSC_REPORT_STATE            0x8BD7  // wscd report it's status is success or fail
/*Report 1.p2p client connect state to web server ; for process start udhcpc
               2.p2p pre-GO change to GO (WPS is done and success) indicate web server need start udhcpd*/
#define SIOCP2P_REPORT_CLIENT_STATE         0x8BD8  // report client connect state

//static int rtk_testcmd_initialized = 0;

enum p2p_role_more{
    P2P_DEVICE=1,
    P2P_PRE_CLIENT=2,
    P2P_CLIENT=3,
    P2P_PRE_GO=4,    // after GO nego , we are GO and proceed WSC exchange
    P2P_TMP_GO=5     // after GO nego , we are GO and proceed WSC exchange is done
};
#endif
//==============p2p related define==========================

enum NETWORK_TYPE {
    WIRELESS_11B = 1,
    WIRELESS_11G = 2,
    WIRELESS_11A = 4,
    WIRELESS_11N = 8,
    WIRELESS_11AC = 64
};


enum BANDWIDTH_TYPE {
    _BW20M = 0,
    _BW40M = 1,
    _BW80M = 2
};

enum SECONDARY_CHANNEL_OFFSET {
    HT_2NDCH_OFFSET_DONTCARE = 0,
    HT_2NDCH_OFFSET_BELOW    = 1,   // secondary channel is below primary channel, ex. primary:5   2nd:1   ,  primary:11   2nd:7
    HT_2NDCH_OFFSET_ABOVE    = 2    // secondary channel is above primary channel, ex. primary:5    2nd:9 ,  primary:1   2nd:5
};


typedef enum _Capability {
    cESS        = 0x01,
    cIBSS       = 0x02,
    cPollable       = 0x04,
    cPollReq        = 0x01,
    cPrivacy        = 0x10,
    cShortPreamble  = 0x20,
} Capability;

typedef struct _sitesurvey_status {
    unsigned char number;
    unsigned char pad[3];
    //BssDscr bssdb[MAX_BSS_DESC];
    struct bss_desc bssdb[MAX_BSS_DESC];
} SS_STATUS_T, *SS_STATUS_Tp;

#ifndef _WPS_DEF_H_
typedef enum { BAND_11B=1, BAND_11G=2, BAND_11BG=3, BAND_11A=4, BAND_11N=8, BAND_5G_11AN=12,
    BAND_5G_11AC=64,BAND_5G_11AAC=68,BAND_5G_11NAC=72,BAND_5G_11ANAC=76} BAND_TYPE_T;
#endif

/* flag of sta info */
#define STA_INFO_FLAG_AUTH_OPEN         0x01
#define STA_INFO_FLAG_AUTH_WEP          0x02
#define STA_INFO_FLAG_ASOC              0x04
#define STA_INFO_FLAG_ASLEEP            0x08

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
    WIFI_STATION_STATE  =   0x00000008,
    WIFI_AP_STATE       =   0x00000010,
    WIFI_P2P_SUPPORT    =   0x08000000
};

#define __DISABLE__  0
#define __ENABLE__   1
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

#ifdef P2P_SUPPORT
/* Any changed here MUST sync with 8192cd_p2p.h */
typedef struct _p2p_provision_comm
{
    unsigned char dev_address[6];
    unsigned short wsc_config_method;
    unsigned char channel;
} P2P_PROVISION_COMM_T, *P2P_PROVISION_COMM_Tp;

/* Any changed here MUST sync with 8192cd_p2p.h */
typedef struct __p2p_wsc_confirm
{
    unsigned char dev_address[6];
    unsigned short wsc_config_method;
    unsigned char pincode[9];
}P2P_WSC_CONFIRM_T, *P2P_WSC_CONFIRM_Tp;
#endif

typedef struct
{
    const char*    str_security_mode;
    //unsigned int   support_mode;
    //const char**   cmds;
}WIFI_SECURITY_MODE_T;

static const WIFI_SECURITY_MODE_T wifi_security_tbl[] =
{
    {"none"},
    {"wep64-auto"},
    {"wep128-auto"},
    {"wpa-psk-tkip"},
    {"wpa-psk-aes"},
    {"wpa2-psk-aes"},
    {"wpa-auto-psk-tkipaes"},
    {NULL}
};

SS_STATUS_T ss_status; //site survey result

bool geCosDhcpCliOpened = false;
bool geCosDhcpSrvOpened = false;
char geCosNvtNetHostNameCli[CYGNUM_NET_DHCP_OPTION_HOST_NAME_LEN] = "nvtdsc01cli";
char geCosNvtNetHostNameSrv[CYGNUM_NET_DHCP_OPTION_HOST_NAME_LEN] = "nvtdsc01srv";

char gipaddr[16]="192.168.1.254";
char gpsk[65] = "12345678";
char gssid[33] = "RTK_ap";
char gauth[15] = "wpa2-psk-aes";
int gchan =0;
char wep_key[65];
char wep_key_idx=0;

int chan_2G_index=14;
int chan_5G_index=11;

int chan_2G_list[14]={0,1,2,3,4,5,6,7,8,9,10,11,12,13};
int chan_5G_list[11]={36,40,44,48,149,151,153,157,159,161,165};

#if ECOS_DHCP
static int copy_str(char *pdest, char *psrc)
{
    int i = 0;
    int max = CYGNUM_NET_DHCP_OPTION_HOST_NAME_LEN - 1;
    if (pdest && psrc)
    {
        i = strlen(psrc);
        i = (i < max) ? i : max;
        memcpy((void *)pdest, (void *)psrc, i);
        *(pdest+i) = 0;
    }
    return i;
}
bool eCos_Dhcp_Set_Interface(char *pIntf)
{
    return (dhcpd_set_interface(pIntf));
}
void eCos_Dhcp_Client_Stop(void)
{
    if (geCosDhcpCliOpened)
    {
        dhcp_set_hostname(" ");
        dhcp_release_interfaces(3);
        dhcp_reg_cb(0);
        uninit_all_network_interfaces();
        geCosDhcpCliOpened = false;
    }
    else
    {
        printf("Dhcp Client Not Started;Not End it.\r\n");
    }
}
void eCos_Dhcp_Client_Start(char *pName, dhcp_cb_func pFunc, bool forceStopIfPrvExist)
{
    printf(" dhcpc=%d,dhcps=%d,force=%d, func=0x%x +\r\n",geCosDhcpCliOpened,geCosDhcpSrvOpened,forceStopIfPrvExist,pFunc);

    if (geCosDhcpSrvOpened)
    {
        printf("DhcpSrv Started;Can Not Be Dhcp Client at the same time\r\n");
    }
    else if (forceStopIfPrvExist)
    {
        printf("Dhcp Client Already Started !! Stop First, then Restart is again:%d\r\n",forceStopIfPrvExist);
        eCos_Dhcp_Client_Stop();
    }
    else if (geCosDhcpCliOpened)
    {
        printf("Dhcp Client Already Started !! Not Start it again:%d\r\n",forceStopIfPrvExist);
        return;
    }
    copy_str(geCosNvtNetHostNameCli, pName);
    dhcp_set_hostname(geCosNvtNetHostNameCli);
    geCosDhcpCliOpened = true;
    dhcp_reg_cb((dhcp_cb_func)pFunc);
    init_all_network_interfaces();
}
void eCos_Dhcp_Server_Start(char *pName)
{
    if (geCosDhcpCliOpened)
    {
        printf("Dhcp Client Started;Can Not Be Dhcp Server at the same time\r\n");
    }
    else if (false == geCosDhcpSrvOpened)
    {
        geCosDhcpSrvOpened = true;
        copy_str(geCosNvtNetHostNameSrv, pName);
        dhcp_set_hostname(geCosNvtNetHostNameSrv);
        dhcpd_startup();
    }
    else
    {
        printf("Dhcp Server Already Started !!\r\n");
    }
}
void eCos_Dhcp_Server_Stop(bool uninitIP)
{
    if (true == geCosDhcpSrvOpened)
    {
        dhcp_set_hostname(" ");
        dhcpd_exit();
        geCosDhcpSrvOpened = false;
        if ((true == uninitIP) && (true == geCosDhcpSrvOpened))
        {
            printf("Dhcp Server Running.Please Stop it first.\r\n");
        }
    }
    else
    {
        printf("Dhcp Server Not Started;Not End it.\r\n");
    }
}
#endif
//-----------------------------------------------------------------------------
#ifdef P2P_SUPPORT
int drv_p2p_cmd(char *cmdstr)
{
    int skfd=0;
    struct iwreq wrq;
    char buff[64];

    skfd = socket(AF_INET, SOCK_DGRAM, 0);
    if(skfd==-1)
        return -1;

    strncpy(wrq.ifr_name, "wlan0", IFNAMSIZ);
    sprintf(buff, "%s", cmdstr);
    wrq.u.data.pointer = (caddr_t)buff;
    wrq.u.data.length = strlen(buff);
    if (ioctl(skfd, SIOCP2PCMD, &wrq) < 0){
        close(skfd);
        return -1;
    }
    close(skfd);
    return 0;
}
#endif

int drvmib_set(char *name, char *sval)
{
    int skfd=0;
    struct iwreq wrq;
    char buff[256];

    skfd = socket(AF_INET, SOCK_DGRAM, 0);
    if(skfd==-1)
        return -1;

    strncpy(wrq.ifr_name, "wlan0", IFNAMSIZ);
    sprintf(buff, "%s=%s", name, sval);
    wrq.u.data.pointer = (caddr_t)buff;
    wrq.u.data.length = strlen(buff);
    if (ioctl(skfd, RTL8192CD_IOCTL_SET_MIB, &wrq) < 0){
        close(skfd);
        return -1;
    }
    close(skfd);
    return 0;
}

int drvmib_get(char *name, char *buff, int *len)
{
    int skfd=0;
    struct iwreq wrq;

    skfd = socket(AF_INET, SOCK_DGRAM, 0);
    if(skfd==-1)
        return -1;

    strncpy(wrq.ifr_name, "wlan0", IFNAMSIZ);
    strcpy(buff, name);
    wrq.u.data.pointer = (caddr_t)buff;
    wrq.u.data.length = strlen(buff);
    //wrq.u.data.length = *len;
    if (ioctl(skfd, RTL8192CD_IOCTL_GET_MIB, &wrq) < 0){
        close(skfd);
        return -1;
    }
    close(skfd);
    *len = wrq.u.data.length;
    return 0;
}

MAINFUNC_ENTRY(drvmib,argc,argv)
{
    if (argc < 2)
        return 0;

    if (strcmp(argv[1], "get") == 0) {
        if (argc > 2) {
            char buff[256];
            int i, len = 256;

            if (drvmib_get(argv[2], buff, &len) < 0)
                printf("drvmib_get Fail\n");
            else {
                printf("drvmib_get OK\n");
                printf("HEX:");
                for (i=0; i<len; i++) {
                    printf("%02x", buff[i]);
                }
                printf("\n");
                printf("ASC:");
                for (i=0; i<len; i++) {
                    printf("%c", buff[i]);
                }
                printf("\n");
            }
        }
    }
    else if (strcmp(argv[1], "set") == 0) {
        if (argc > 3) {
            if (drvmib_set(argv[2], argv[3]) < 0)
                printf("drvmib_set Fail\n");
            else
                printf("drvmib_set OK\n");
        }
    }
    return 0;
}
//-----------------------------------------------------------------------------

/*
 * Wrapper to extract some Wireless Parameter out of the driver
 */
static inline int
iw_get_ext(int                  skfd,           /* Socket to the kernel */
           char *               ifname,         /* Device name */
           int                  request,        /* WE ID */
           struct iwreq *       pwrq)           /* Fixed part of the request */
{
  /* Set device name */
  strncpy(pwrq->ifr_name, ifname, IFNAMSIZ);
  /* Do the request */
  return(ioctl(skfd, request, pwrq));
}

/////////////////////////////////////////////////////////////////////////////
int join_bss_request(char *interface, struct bss_desc *pBss, unsigned char *res)
{
    int skfd=0;
    struct iwreq wrq;

    skfd = socket(AF_INET, SOCK_DGRAM, 0);
    if(skfd==-1)
        return -1;
    /* Get wireless name */
    if ( iw_get_ext(skfd, interface, SIOCGIWNAME, &wrq) < 0)
      /* If no wireless name : no wireless extensions */
        return -1;

    wrq.u.data.pointer = (caddr_t)pBss;
    wrq.u.data.length = sizeof(struct bss_desc);

    if (iw_get_ext(skfd, interface, SIOCGIWRTLJOINREQ, &wrq) < 0)
    return -1;

    close( skfd );

    *res = *(unsigned char *)wrq.u.data.pointer;

    return 0;
}

int getWlSiteSurveyRequest(char *interface, int *pStatus)
{
    int skfd=0;
    struct iwreq wrq;
    unsigned char result;

    skfd = socket(AF_INET, SOCK_DGRAM, 0);
    if(skfd==-1)
        return -1;

    /* Get wireless name */
    if ( iw_get_ext(skfd, interface, SIOCGIWNAME, &wrq) < 0){
      /* If no wireless name : no wireless extensions */
      close( skfd );
        return -1;
    }
    wrq.u.data.pointer = (caddr_t)&result;
    wrq.u.data.length = sizeof(result);

    if (iw_get_ext(skfd, interface, SIOCGIWRTLSCANREQ, &wrq) < 0){
        close( skfd );
    return -1;
    }
    close( skfd );

    if ( result == 0xff )
        *pStatus = -1;
    else
    *pStatus = (int) result;

    return 0;
}

/////////////////////////////////////////////////////////////////////////////
int getWlSiteSurveyResult(char *interface, SS_STATUS_Tp pStatus )
{
    int skfd=0;
    struct iwreq wrq;

    skfd = socket(AF_INET, SOCK_DGRAM, 0);
    if(skfd==-1)
        return -1;
    /* Get wireless name */
    if ( iw_get_ext(skfd, interface, SIOCGIWNAME, &wrq) < 0) {
      /* If no wireless name : no wireless extensions */
      close( skfd );
        return -1;
    }
    wrq.u.data.pointer = (caddr_t)pStatus;

    if ( pStatus->number == 0 )
        wrq.u.data.length = sizeof(SS_STATUS_T);
    else
        wrq.u.data.length = sizeof(pStatus->number);

    if (iw_get_ext(skfd, interface, SIOCGIWRTLGETBSSDB, &wrq) < 0){
        close( skfd );
    return -1;
    }
    close( skfd );

    return 0;
}

void print_bss_desc(struct bss_desc *pBss)
{
    char ssidbuf[40], tmp2Buf[20], tmp3Buf[20];

    //BSSID
    printf("%02x:%02x:%02x:%02x:%02x:%02x",
        pBss->bssid[0], pBss->bssid[1], pBss->bssid[2],
        pBss->bssid[3], pBss->bssid[4], pBss->bssid[5]);

    //channel
    printf("  %3d", pBss->channel);

    //band
    tmp2Buf[0] = 0;
    if (pBss->network & WIRELESS_11N) {
        strcat(tmp2Buf, "N");
    }
    if (pBss->network & WIRELESS_11G) {
        strcat(tmp2Buf, "G");
    }
    if (pBss->network & WIRELESS_11B) {
        strcat(tmp2Buf, "B");
    }
    printf("%5s", tmp2Buf);

    //rssi
    printf(" %3d", pBss->rssi);

    //signal quality
    printf("   %3d ", pBss->sq);

    //Security
    memset(tmp3Buf,0x00,sizeof(tmp3Buf));

    // refer to get_security_info() for details
    if ((pBss->capability & cPrivacy) == 0) {
        strcpy(tmp2Buf, "");
        strcpy(tmp3Buf, "None");
    }
    else {
        if (pBss->t_stamp[0] == 0) {
            strcpy(tmp2Buf, "");
            strcpy(tmp3Buf, "WEP");
        }
        else {
            int wpa_exist = 0, idx = 0;
            if (pBss->t_stamp[0] & 0x0000ffff) {
                idx = sprintf(tmp2Buf,"%s","WPA");
                if (((pBss->t_stamp[0] & 0x0000f000) >> 12) == 0x4)
                    idx += sprintf(tmp2Buf+idx,"%s","-PSK");
                else if(((pBss->t_stamp[0] & 0x0000f000) >> 12) == 0x2)
                    idx += sprintf(tmp2Buf+idx,"%s","-1X");
                wpa_exist = 1;

                if (((pBss->t_stamp[0] & 0x00000f00) >> 8) == 0x5)
                    sprintf(tmp3Buf,"%s","AES/TKIP");
                else if (((pBss->t_stamp[0] & 0x00000f00) >> 8) == 0x4)
                    sprintf(tmp3Buf,"%s","AES");
                else if (((pBss->t_stamp[0] & 0x00000f00) >> 8) == 0x1)
                    sprintf(tmp3Buf,"%s","TKIP");
            }
            if (pBss->t_stamp[0] & 0xffff0000) {
                if (wpa_exist)
                    idx += sprintf(tmp2Buf+idx,"%s","/");
                idx += sprintf(tmp2Buf+idx,"%s","WPA2");
                if (((pBss->t_stamp[0] & 0xf0000000) >> 28) == 0x4)
                    idx += sprintf(tmp2Buf+idx,"%s","-PSK");
                else if (((pBss->t_stamp[0] & 0xf0000000) >> 28) == 0x2)
                    idx += sprintf(tmp2Buf+idx,"%s","-1X");

                if (((pBss->t_stamp[0] & 0x0f000000) >> 24) == 0x5)
                    sprintf(tmp3Buf,"%s","AES/TKIP");
                else if (((pBss->t_stamp[0] & 0x0f000000) >> 24) == 0x4)
                    sprintf(tmp3Buf,"%s","AES");
                else if (((pBss->t_stamp[0] & 0x0f000000) >> 24) == 0x1)
                    sprintf(tmp3Buf,"%s","TKIP");
            }
        }
    }
    printf("%-10s ", tmp3Buf);
    printf("%-16s ", tmp2Buf);

    //SSID
    memcpy(ssidbuf, pBss->ssid, pBss->ssidlen);
    ssidbuf[pBss->ssidlen] = '\0';
    printf("%s\n", ssidbuf);
}

void do_site_survey(char *ifname)
{
    int status;
    unsigned char res;
    int wait_time;
    int i;
    //BssDscr *pBss;
    struct bss_desc *pBss;
    //SS_STATUS_Tp pStatus=NULL;
    SS_STATUS_Tp pStatus = &ss_status;

    //below is copied from formWlSiteSurvey()
    {
        // issue scan request
        wait_time = 0;
        while (1) {
            // ==== modified by GANTOE for site survey 2008/12/26 ====
            switch(getWlSiteSurveyRequest(ifname, &status))
            {
                case -2:
                    printf("Auto scan running!!please wait...\n");
                    goto ss_err;
                    break;
                case -1:
                    printf("Site-survey request failed!\n");
                    goto ss_err;
                    break;
                default:
                    break;
            }

            if (status != 0) {  // not ready
                if (wait_time++ > 5) {
                    printf("scan request timeout!\n");
                    goto ss_err;
                }
                sleep(1);
            }
            else
                break;
        }

        // wait until scan completely
        wait_time = 0;
        while (1) {
            res = 1;    // only request request status
            if ( getWlSiteSurveyResult(ifname, (SS_STATUS_Tp)&res) < 0 ) {
                printf("Read site-survey status failed!");
                goto ss_err;
            }
            if (res == 0xff) {   // in progress
                if (wait_time++ > 10)
                {
                    printf("scan timeout!");
                    goto ss_err;
                }
                sleep(1);
            }
            else
                break;
        }
    }

    //below is copied from wlSiteSurveyTbl()
    /*
    if (pStatus==NULL) {
        pStatus = calloc(1, sizeof(SS_STATUS_T));
        if ( pStatus == NULL ) {
            printf("Allocate buffer failed!\n");
            goto ss_err;
        }
    }
    */

    pStatus->number = 0; // request BSS DB
    if ( getWlSiteSurveyResult(ifname, pStatus) < 0 ) {
        printf("Read site-survey status failed!\n");
        //free(pStatus);
        pStatus = NULL;
        goto ss_err;
    }

    //success
    printf(" #      BSSID          ch band rssi  sq  enc        auth             SSID\n");
    for (i=0; i<pStatus->number && pStatus->number!=0xff; i++) {
        pBss = &pStatus->bssdb[i];
        printf("%2d ", i);
        print_bss_desc(pBss);
    }

    //free(pStatus);
    pStatus = NULL;
    return;

ss_err:
    //failed
    //free(pStatus);
    pStatus = NULL;
}

#ifdef P2P_SUPPORT
int getWlP2PScanResult(char *interface, SS_STATUS_Tp pStatus)
{
    int skfd=0;
    struct iwreq wrq;

    skfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (skfd==-1)
        return -1;
    /* Get wireless name */
    if (iw_get_ext(skfd, interface, SIOCGIWNAME, &wrq) < 0) {
        /* If no wireless name : no wireless extensions */
        close(skfd);
        return -1;
    }

    wrq.u.data.pointer = (caddr_t)pStatus;
    if (pStatus->number == 0)
        wrq.u.data.length = sizeof(SS_STATUS_T);
    else
        wrq.u.data.length = sizeof(pStatus->number);

    if (iw_get_ext(skfd, interface, SIOCP2PGETRESULT, &wrq) < 0) {
        close(skfd);
        return -1;
    }
    close(skfd);
    return 0;
}

int getWlP2PScanRequest(char *interface, int *pStatus)
{
    int skfd=0;
    struct iwreq wrq;
    unsigned char result;

    skfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (skfd==-1)
        return -1;

    /* Get wireless name */
    if (iw_get_ext(skfd, interface, SIOCGIWNAME, &wrq) < 0) {
        /* If no wireless name : no wireless extensions */
        close(skfd);
        return -1;
    }
    wrq.u.data.pointer = (caddr_t)&result;
    wrq.u.data.length = sizeof(result);
    if (iw_get_ext(skfd, interface, SIOCP2PSCANREQ, &wrq) < 0) {
        close(skfd);
        return -1;
    }
    close(skfd);
    if (result == 0xff)
        *pStatus = -1;
    else
        *pStatus = (int)result;
    return 0;
}

void do_p2p_scan(char *ifname)
{
    int status;
    unsigned char res;
    int wait_time;
    int i;
    //BssDscr *pBss;
    struct bss_desc *pBss;
    //SS_STATUS_Tp pStatus=NULL;
    SS_STATUS_Tp pStatus = &ss_status;

    // issue scan request
    wait_time = 0;
    while (1) {
        switch (getWlP2PScanRequest(ifname, &status)) {
        case -2:
            printf("Auto scan running!!please wait...\n");
            goto ss_err;
            break;
        case -1:
            printf("Site-survey request failed!\n");
            goto ss_err;
            break;
        default:
            break;
        }

        if (status != 0) { // not ready
            if (wait_time++ > 5) {
                printf("scan request timeout!\n");
                goto ss_err;
            }
            sleep(1);
        }
        else
            break;
    }

    // wait until scan completely
    wait_time = 0;
    while (1) {
        res = 1;
        if (getWlP2PScanResult(ifname, (SS_STATUS_Tp)&res) < 0) {
            printf("Read site-survey status failed!");
            goto ss_err;
        }

        if (res == 0xff) { // in progress
            if (wait_time++ > 20) {
                printf("scan timeout!\n");
                goto ss_err;
            }
            sleep(1);
        }
        else
            break;
    }

    // get scan result
    pStatus->number = 0; // request BSS DB
    if ( getWlP2PScanResult(ifname, pStatus) < 0 ) {
        printf("Read site-survey status failed!\n");
        //free(pStatus);
        pStatus = NULL;
        return;
    }

    //printf("pStatus->number=%d\n",pStatus->number);
    printf(" # Channel Device address    Role WSCMethod Device Name\n");
    for (i=0; i<pStatus->number && pStatus->number!=0xff; i++) {
        pBss = &pStatus->bssdb[i];
        printf("%2d %3d     ", i, pBss->channel);
        printf("%02x:%02x:%02x:%02x:%02x:%02x ",
            pBss->p2paddress[0], pBss->p2paddress[1], pBss->p2paddress[2],
            pBss->p2paddress[3], pBss->p2paddress[4], pBss->p2paddress[5]);
        if(pBss->p2prole == 2) //2: device
            printf("DEV");
        else if(pBss->p2prole == 1) // 1:GO
            printf("GO ");
        else
            printf("???");
        printf("  0x%04x", pBss->p2pwscconfig);
        printf("     %s\n", pBss->p2pdevname);
    }

ss_err:
    pStatus = NULL;
}

int sendP2PProvisionCommInfo(char *interface, P2P_PROVISION_COMM_Tp pP2PProvisionComm)
{
    int skfd=0;
    struct iwreq wrq;

    skfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (skfd==-1)
        return -1;
    /* Get wireless name */
    if (iw_get_ext(skfd, interface, SIOCGIWNAME, &wrq) < 0) {
        /* If no wireless name : no wireless extensions */
        close(skfd);
        return -1;
    }

    wrq.u.data.pointer = (caddr_t)pP2PProvisionComm;
    wrq.u.data.length = sizeof(P2P_PROVISION_COMM_T);
    if (iw_get_ext(skfd, interface, SIOCP2PPROVREQ, &wrq) < 0) {
        close(skfd);
        return -1;
    }
    close(skfd);
    return 0;
}

int sendP2PWscConfirm(char *interface, P2P_WSC_CONFIRM_Tp pP2PWscConfirm)
{
    int skfd = 0;
    struct iwreq wrq;

    skfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (skfd==-1)
        return -1;
    /* Get wireless name */
    if (iw_get_ext(skfd, interface, SIOCGIWNAME, &wrq) < 0) {
        /* If no wireless name : no wireless extensions */
        close(skfd);
        return -1;
    }

    wrq.u.data.pointer = (caddr_t)pP2PWscConfirm;
    wrq.u.data.length = sizeof(P2P_WSC_CONFIRM_T);
    if (iw_get_ext(skfd, interface, SIOCP2WSCMETHODCONF, &wrq) < 0) {
        close(skfd);
        return -1;
    }
    close(skfd);
    return 0;
}
#endif

int getWlStaNum(char *interface, int *num)
{
    int skfd=0;
    unsigned short staNum;
    struct iwreq wrq;

    skfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (skfd==-1)
        return -1;
    /* Get wireless name */
    if (iw_get_ext(skfd, interface, SIOCGIWNAME, &wrq) < 0) {
        /* If no wireless name : no wireless extensions */
        close(skfd);
        return -1;
    }
    wrq.u.data.pointer = (caddr_t)&staNum;
    wrq.u.data.length = sizeof(staNum);

    if (iw_get_ext(skfd, interface, SIOCGIWRTLSTANUM, &wrq) < 0) {
        close(skfd);
        return -1;
    }
    *num = (int)staNum;

    close(skfd);
    return 0;
}

int getWlStaInfo(char *interface,  WLAN_STA_INFO_Tp pInfo)
{
    int skfd=0;
    struct iwreq wrq;

    skfd = socket(AF_INET, SOCK_DGRAM, 0);
    if(skfd==-1)
        return -1;
    /* Get wireless name */
    if (iw_get_ext(skfd, interface, SIOCGIWNAME, &wrq) < 0) {
        /* If no wireless name : no wireless extensions */
        close(skfd);
        return -1;
    }
    wrq.u.data.pointer = (caddr_t)pInfo;
    wrq.u.data.length = sizeof(WLAN_STA_INFO_T) * (MAX_STA_NUM+1);
    memset(pInfo, 0, sizeof(WLAN_STA_INFO_T) * (MAX_STA_NUM+1));

    if (iw_get_ext(skfd, interface, SIOCGIWRTLSTAINFO, &wrq) < 0) {
        close(skfd);
        return -1;
    }
    close(skfd);
    return 0;
}

void show_associated_clients(char *ifname)
{
    int i;
    WLAN_STA_INFO_Tp pInfo;
    char *buff;
    char txrate[20];
    int rateid=0;

    buff = calloc(1, sizeof(WLAN_STA_INFO_T) * (MAX_STA_NUM+1));
    if ( buff == 0 ) {
        printf("Allocate buffer failed!\n");
        goto failed_label;
    }

    if ( getWlStaInfo(ifname,  (WLAN_STA_INFO_Tp)buff ) < 0 ) {
        printf("Read wlan sta info failed!\n");
        free(buff);
        goto failed_label;
    }

    //success
    printf("MAC               Mode      TxPkt      RxPkt  TxRate(Mbps)   PowerSaving ExpiredTime(s)\n");
    for (i=1; i<=MAX_STA_NUM; i++) {
        pInfo = (WLAN_STA_INFO_Tp)&buff[i*sizeof(WLAN_STA_INFO_T)];
        if (pInfo->aid && (pInfo->flag & STA_INFO_FLAG_ASOC)) {
            //MAC address
            printf("%02x:%02x:%02x:%02x:%02x:%02x ",
                pInfo->addr[0],pInfo->addr[1],pInfo->addr[2],
                pInfo->addr[3],pInfo->addr[4],pInfo->addr[5]);

            //Mode
            if(pInfo->network & BAND_11N)
                printf("11n");
            else if (pInfo->network & BAND_11G)
                printf("11g");
            else if (pInfo->network & BAND_11B)
                printf("11b");
            else if (pInfo->network& BAND_11A)
                printf("11a");
            else
                printf("---");

            //TxPkt
            printf("  %10d ", pInfo->tx_packets);

            //RxPkt
            printf("%10d ", pInfo->rx_packets);

            //TxRate
            if((pInfo->txOperaRates & 0x80) != 0x80){
                if(pInfo->txOperaRates%2){
                    sprintf(txrate, "%d%s",pInfo->txOperaRates/2, ".5");
                }else{
                    sprintf(txrate, "%d",pInfo->txOperaRates/2);
                }
            }else{
                if((pInfo->ht_info & 0x1)==0){ //20M
                    if((pInfo->ht_info & 0x2)==0){//long
                        for(rateid=0; rateid<16;rateid++){
                            if(rate_11n_table_20M_LONG[rateid].id == pInfo->txOperaRates){
                                sprintf(txrate, "%s", rate_11n_table_20M_LONG[rateid].rate);
                                break;
                            }
                        }
                    }else if((pInfo->ht_info & 0x2)==0x2){//short
                        for(rateid=0; rateid<16;rateid++){
                            if(rate_11n_table_20M_SHORT[rateid].id == pInfo->txOperaRates){
                                sprintf(txrate, "%s", rate_11n_table_20M_SHORT[rateid].rate);
                                break;
                            }
                        }
                    }
                }else if((pInfo->ht_info & 0x1)==0x1){//40M
                    if((pInfo->ht_info & 0x2)==0){//long
                        for(rateid=0; rateid<16;rateid++){
                            if(rate_11n_table_40M_LONG[rateid].id == pInfo->txOperaRates){
                                sprintf(txrate, "%s", rate_11n_table_40M_LONG[rateid].rate);
                                break;
                            }
                        }
                    }else if((pInfo->ht_info & 0x2)==0x2){//short
                        for(rateid=0; rateid<16;rateid++){
                            if(rate_11n_table_40M_SHORT[rateid].id == pInfo->txOperaRates){
                                sprintf(txrate, "%s", rate_11n_table_40M_SHORT[rateid].rate);
                                break;
                            }
                        }
                    }
                }
            }
            printf("%10s ", txrate);

            //Power Saving
            printf("%10s ", ((pInfo->flag & STA_INFO_FLAG_ASLEEP) ? "yes" : "no"));

            //Expired Time
            printf("        %d\n", pInfo->expired_time/100);
        }
    }

    free(buff);
    return;

failed_label:
    //failed
    return;
}


// add only on ecos
#define SIOCTL_GET_PSTA 0x8BCE
struct stat_info_ecos_upper {
    unsigned char       rssi;
    unsigned char       hwaddr[MACADDRLEN];
};

int getPstaInfo(char *interface, unsigned char* sta_mac_addr)
{
    int skfd=0;
    struct iwreq wrq;

    struct stat_info_ecos_upper PstaInfo;

    if(sta_mac_addr!=NULL){
        memcpy(PstaInfo.hwaddr,sta_mac_addr,6);
        printf("query STA[%02X%02X%02X:%02X%02X%02X]\n",sta_mac_addr[0],sta_mac_addr[1]
            ,sta_mac_addr[2],sta_mac_addr[3],sta_mac_addr[4],sta_mac_addr[5]);
    }else{
        printf("error , NULL mac addr\n");
        return -1;
    }

    skfd = socket(AF_INET, SOCK_DGRAM, 0);
    if(skfd==-1)
        return -1;
    /* Get wireless name */
    if ( iw_get_ext(skfd, interface, SIOCGIWNAME, &wrq) < 0)
      /* If no wireless name : no wireless extensions */
      {
         close( skfd );
        return -1;
      }

    wrq.u.data.pointer = (caddr_t)&PstaInfo;
    wrq.u.data.length = sizeof(struct stat_info_ecos_upper);

    if (iw_get_ext(skfd, interface, SIOCTL_GET_PSTA, &wrq) < 0){
         close( skfd );
         return -1;
    }
    close( skfd );

    /*display rssi*/
    printf("rssi[%d]\n",PstaInfo.rssi);

    return 0;
}



/////////////////////////////////////////////////////////////////////////////
int getWlBssInfo(char *interface, bss_info *pInfo)
{
    int skfd=0;
    struct iwreq wrq;

    skfd = socket(AF_INET, SOCK_DGRAM, 0);
    if(skfd==-1)
        return -1;
    /* Get wireless name */
    if ( iw_get_ext(skfd, interface, SIOCGIWNAME, &wrq) < 0)
      /* If no wireless name : no wireless extensions */
      {
         close( skfd );
        return -1;
      }

    wrq.u.data.pointer = (caddr_t)pInfo;
    wrq.u.data.length = sizeof(bss_info);

    if (iw_get_ext(skfd, interface, SIOCGIWRTLGETBSSINFO, &wrq) < 0){
         close( skfd );
    return -1;
    }
    close( skfd );
    return 0;
}

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

void show_bss_status(char *ifname)
{
     bss_info bss;
     //char buff[256];
     int connection=0;
     char ssid[40];

     getWlBssInfo(ifname, &bss);

     if(bss.state==STATE_CONNECTED)
	connection = 1;

     memcpy(ssid, bss.ssid, 33);
     translate_control_code(ssid);

     printf("[ATD] status=%d,%s,%d \n", connection, ssid, bss.channel);
}

void show_bss_info(char *ifname)
{
    bss_info bss;
    char buff[256];

    getWlBssInfo(ifname, &bss);
    //printf("state             bssid          ch  rssi  sq  txrate ssid\n");
    printf("state             bssid          ch  rssi  sq   ssid\n");
    //state
    switch (bss.state) {
        case STATE_DISABLED:
            strcpy(buff, "Disabled");
            break;
        case STATE_IDLE:
            strcpy(buff, "Idle");
            break;
        case STATE_STARTED:
            strcpy(buff, "Started");
            break;
        case STATE_CONNECTED:
            strcpy(buff, "Connected");
            break;
        case STATE_WAITFORKEY:
            strcpy(buff, "Wait for key");
            break;
        case STATE_SCANNING:
            strcpy(buff, "Scanning");
            break;
        default:
            strcpy(buff, "");
    }
    printf("%-12s ", buff);

    //BSSID
    printf("%02x:%02x:%02x:%02x:%02x:%02x",
        bss.bssid[0], bss.bssid[1], bss.bssid[2],
        bss.bssid[3], bss.bssid[4], bss.bssid[5]);

    //channel
    printf("  %3d", bss.channel);

    //rssi
    printf(" %3d", bss.rssi);

    //signal quality
    printf("   %3d ", bss.sq);

    //txRate
    //printf(" %6d", bss.txRate);

    //SSID
    memcpy(buff, bss.ssid, 33);
    translate_control_code(buff);
    printf(" %s\n", buff);
}

//-----------------------------------------------------------------------------
// wifi
//-----------------------------------------------------------------------------
static int IS_MCAST(char *da)
{
    if ((*da) & 0x01)
        return 1;
    else
        return 0;
}

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
    printf("channel=%d\n", wifi_settings_sample.channel);
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
    printf("isdhcp=%d\n", wifi_settings_sample.is_dhcp);
}

static void init_wifi_settings(char *mode, char *security)
{
    struct wifi_settings_sample *pwifi=&wifi_settings_sample;

    if (strcmp(pwifi->mode, mode) != 0) {
        //if mode is going to be changed, reset wps to unconfigured mode
        pwifi->is_configured = 0;
    }

    strcpy(pwifi->mode, mode);
    strcpy(pwifi->security, security);

    pwifi->channel = 0; // auto channel
    //pwifi->channel = 10;
    //pwifi->channel = 11;
    if (strcmp(pwifi->mode, "ap") == 0) {
		if(strlen(gssid))
			strcpy(pwifi->ssid, gssid);
		else
			strcpy(pwifi->ssid, "ecos-8189es-ap");
        strcpy(pwifi->device_name, "Wireless AP");
    }
    else if (strcmp(pwifi->mode, "sta") == 0) {
        //strcpy(pwifi->ssid, "000ecos");
#if (ECOS_BAND_SEL_5G == 1)
        strcpy(pwifi->ssid, "dlink_5g");
#else
        strcpy(pwifi->ssid, "dlink");
#endif
        strcpy(pwifi->device_name, "Wireless STA");
    }
#ifdef P2P_SUPPORT
    else if (strcmp(pwifi->mode, "p2pgo") == 0) {
        pwifi->p2pmode = P2P_TMP_GO;
        /*p2p note ; p2p GO's SSID must in the form "DIRECT-xy-anystring"  xy is random*/
        strcpy(pwifi->ssid, "DIRECT-go-12345678");
        pwifi->is_configured = 1;            // let wscd under configured mode
        strcpy(pwifi->device_name, "p2p-go");
    }
    else if (strcmp(pwifi->mode, "p2pdev") == 0) {
        pwifi->p2pmode = P2P_DEVICE;
        /*p2p note ; p2p GO's SSID must in the form "DIRECT-xy-anystring"  xy is random*/
        strcpy(pwifi->ssid, "DIRECT-de-12345678");
        strcpy(pwifi->device_name, "p2p-dev");
    }
#endif

	if(strlen(gpsk))
		strcpy(pwifi->passphrase, gpsk);
	else
		strcpy(pwifi->passphrase, "12345678");

    pwifi->wep_default_key = 0;
    if (strcmp(pwifi->security, "wep64-auto") == 0) {
        strcpy(pwifi->wep_key1, "1234567890");
        strcpy(pwifi->wep_key2, "1234567890");
        strcpy(pwifi->wep_key3, "1234567890");
        strcpy(pwifi->wep_key4, "1234567890");
    }
    else {
        strcpy(pwifi->wep_key1, "12345678901234567890123456");
        strcpy(pwifi->wep_key2, "12345678901234567890123456");
        strcpy(pwifi->wep_key3, "12345678901234567890123456");
        strcpy(pwifi->wep_key4, "12345678901234567890123456");
    }
}

static void start_wifi_settings(char *mode, char *security)
{
    struct wifi_settings_sample *pwifi=&wifi_settings_sample;

    if (strcmp(pwifi->mode, mode) != 0) {
        //if mode is going to be changed, reset wps to unconfigured mode
        pwifi->is_configured = 0;
    }

    strcpy(pwifi->mode, mode);
    strcpy(pwifi->security, security);

    //pwifi->channel = 0; // auto channel
    pwifi->channel = gchan; // auto channel
	strcpy(pwifi->ssid, gssid);


    if (strcmp(pwifi->mode, "ap") == 0) {
        strcpy(pwifi->device_name, "Wireless AP");
    }
    else if (strcmp(pwifi->mode, "sta") == 0) {
        strcpy(pwifi->device_name, "Wireless STA");
    }

#ifdef P2P_SUPPORT
    else if (strcmp(pwifi->mode, "p2pgo") == 0) {
        pwifi->p2pmode = P2P_TMP_GO;
        /*p2p note ; p2p GO's SSID must in the form "DIRECT-xy-anystring"  xy is random*/
        strcpy(pwifi->ssid, "DIRECT-go-12345678");
        pwifi->is_configured = 1;            // let wscd under configured mode
        strcpy(pwifi->device_name, "p2p-go");
    }
    else if (strcmp(pwifi->mode, "p2pdev") == 0) {
        pwifi->p2pmode = P2P_DEVICE;
        /*p2p note ; p2p GO's SSID must in the form "DIRECT-xy-anystring"  xy is random*/
        strcpy(pwifi->ssid, "DIRECT-de-12345678");
        strcpy(pwifi->device_name, "p2p-dev");
    }
#endif

    strcpy(pwifi->passphrase, gpsk);

    pwifi->wep_default_key = wep_key_idx;
    //pwifi->wep_default_key = 0;
    if (strcmp(pwifi->security, "wep64-auto") == 0) {
        strcpy(pwifi->wep_key1, wep_key);
        strcpy(pwifi->wep_key2, wep_key);
        strcpy(pwifi->wep_key3, wep_key);
        strcpy(pwifi->wep_key4, wep_key);
    }
    else if(strcmp(pwifi->security, "wep128-auto") == 0) {
	if(strlen(wep_key)!=26){
            strcpy(pwifi->wep_key1, "12345678901234567890123456");
            strcpy(pwifi->wep_key2, "12345678901234567890123456");
            strcpy(pwifi->wep_key3, "12345678901234567890123456");
            strcpy(pwifi->wep_key4, "12345678901234567890123456");
        }
	else{
            strcpy(pwifi->wep_key1, wep_key);
            strcpy(pwifi->wep_key2, wep_key);
	    strcpy(pwifi->wep_key3, wep_key);
	    strcpy(pwifi->wep_key4, wep_key);
	}
    }
}

static int configure_wifi_sample(struct wifi_settings_sample *pwifi)
{
    //char *ifname = "wlan0";
    int intVal, is_client=0, is_wep=0;
    char tmpbuf[256];
    struct wps_config wpscfg, *pcfg;
    int auth, enc, mixedmode=0, wsc_wpa_cipher=0, wsc_wpa2_cipher=0;
    int tmpval=0;
    //int channel_band_width=0;
    auth = WSC_AUTH_OPEN;
    enc = WSC_ENCRYPT_NONE;

    //int channel_band_width=0;
    int network_type=0;
    unsigned char phyBandSelect=0;

    if (is_interface_up("wlan0"))
        interface_down("wlan0");

    //RunSystemCmd(NULL_FILE, "iwpriv", "wlan0", "set_mib", "band=3", NULL_STR); //bg
    RunSystemCmd(NULL_FILE, "iwpriv", "wlan0", "set_mib", "band=11", NULL_STR); //bgn
    tmpval = (WIRELESS_11B|WIRELESS_11G|WIRELESS_11N);
    RunSystemCmd(NULL_FILE, "iwpriv", "wlan0", "set_mib", "ampdu=1", NULL_STR);
//    RunSystemCmd(NULL_FILE, "iwpriv", "wlan0", "set_mib", "use40M=1", NULL_STR);
    RunSystemCmd(NULL_FILE, "iwpriv", "wlan0", "set_mib", "use40M=0", NULL_STR);
    RunSystemCmd(NULL_FILE, "iwpriv", "wlan0", "set_mib", "shortGI20M=1", NULL_STR);
    RunSystemCmd(NULL_FILE, "iwpriv", "wlan0", "set_mib", "shortGI40M=1", NULL_STR);
    RunSystemCmd(NULL_FILE, "iwpriv", "wlan0", "set_mib", "rtsthres=2347", NULL_STR);
    RunSystemCmd(NULL_FILE, "iwpriv", "wlan0", "set_mib", "fragthres=2346", NULL_STR);
    //RunSystemCmd(NULL_FILE, "iwpriv", "wlan0", "set_mib", "disable_protection=1", NULL_STR);
    RunSystemCmd(NULL_FILE, "iwpriv", "wlan0", "set_mib", "sdio_power_cycle=1", NULL_STR);

    //AP mode
    if (strcmp(pwifi->mode, "ap") == 0) {
        is_client = 0;

#if (ECOS_BAND_SEL_5G == 1)

        RunSystemCmd(NULL_FILE, "iwpriv", "wlan0", "set_mib", "rtsthres=100", NULL_STR);

        /*BAND (network type setting) */
        network_type = (WIRELESS_11AC|WIRELESS_11A|WIRELESS_11N);
        //network_type = (WIRELESS_11B|WIRELESS_11G|WIRELESS_11N);
        //network_type = (WIRELESS_11AC|WIRELESS_11A|WIRELESS_11N);
        //network_type = (WIRELESS_11B|WIRELESS_11G|WIRELESS_11N);
        sprintf(tmpbuf, "band=%d", network_type);
        RunSystemCmd(NULL_FILE, "iwpriv", "wlan0", "set_mib", tmpbuf, NULL_STR);

        phyBandSelect = _PHY_BAND_5G;
        sprintf(tmpbuf, "phyBandSelect=%d", phyBandSelect);
        RunSystemCmd(NULL_FILE, "iwpriv", "wlan0", "set_mib", tmpbuf, NULL_STR);
#else
        /*BAND (network type setting) */
        network_type = (WIRELESS_11B|WIRELESS_11G|WIRELESS_11N);
        sprintf(tmpbuf, "band=%d", network_type);
        RunSystemCmd(NULL_FILE, "iwpriv", "wlan0", "set_mib", tmpbuf, NULL_STR);

        phyBandSelect = _PHY_BAND_2G;
        sprintf(tmpbuf, "phyBandSelect=%d", phyBandSelect);
        RunSystemCmd(NULL_FILE, "iwpriv", "wlan0", "set_mib", tmpbuf, NULL_STR);
#endif

        /*OPMODE*/
        RunSystemCmd(NULL_FILE, "iwpriv", "wlan0", "set_mib", "opmode=16", NULL_STR);

        /*SSID*/
        sprintf(tmpbuf, "ssid=%s", pwifi->ssid);
        RunSystemCmd(NULL_FILE, "iwpriv", "wlan0", "set_mib", tmpbuf, NULL_STR);
        sprintf(tmpbuf, "channel=%d", pwifi->channel);
        RunSystemCmd(NULL_FILE, "iwpriv", "wlan0", "set_mib", tmpbuf, NULL_STR);

        /*when use auto channel don't care 2ndchofset wlan driver will auto fill it's value*/
        //RunSystemCmd(NULL_FILE, "iwpriv", "wlan0", "set_mib", "2ndchoffset=1", NULL_STR); // 1:below, 2:above
        //RunSystemCmd(NULL_FILE, "iwpriv", "wlan0", "set_mib", "disable_protection=1", NULL_STR);

#if (ECOS_BAND_SEL_5G == 1)

        /*bandWidth , 20M / 40M /80M*/
        channel_band_width = _BW20M;  //_BW40M = _BW40M;
        sprintf(tmpbuf, "use40M=%d", channel_band_width);
        RunSystemCmd(NULL_FILE, "iwpriv", "wlan0", "set_mib", tmpbuf, NULL_STR);

        if(channel_band_width  == _BW20M){
            tmpval = HT_2NDCH_OFFSET_DONTCARE;
            sprintf(tmpbuf, "2ndchoffset=%d", tmpval);
            RunSystemCmd(NULL_FILE, "iwpriv", "wlan0", "set_mib", tmpbuf, NULL_STR);
        }else if((channel_band_width  == _BW40M)){

            if(pwifi->channel<=11 && pwifi->channel>=5){
                tmpval = HT_2NDCH_OFFSET_BELOW;    //  1
                sprintf(tmpbuf, "2ndchoffset=%d", tmpval);
            }else if(pwifi->channel>=1 && pwifi->channel<=4){
                tmpval = HT_2NDCH_OFFSET_ABOVE;
                sprintf(tmpbuf, "2ndchoffset=%d", tmpval);  // 2
            }
            RunSystemCmd(NULL_FILE, "iwpriv", "wlan0", "set_mib", tmpbuf, NULL_STR);
        }

#endif

        if (strcmp(pwifi->security, "none") == 0) {
            // No security
            auth = WSC_AUTH_OPEN;
            enc = WSC_ENCRYPT_NONE;
            RunSystemCmd(NULL_FILE, "iwpriv", "wlan0", "set_mib", "authtype=0", NULL_STR);
            RunSystemCmd(NULL_FILE, "iwpriv", "wlan0", "set_mib", "encmode=0", NULL_STR);
            RunSystemCmd(NULL_FILE, "iwpriv", "wlan0", "set_mib", "psk_enable=0", NULL_STR);
            RunSystemCmd(NULL_FILE, "iwpriv", "wlan0", "set_mib", "802_1x=0", NULL_STR);
        }
        else if (strcmp(pwifi->security, "wep64-auto") == 0) {
            // WEP64
            auth = WSC_AUTH_OPEN;
            enc = WSC_ENCRYPT_WEP;
            //authtype: 0-open, 1-shared, 2-auto
            RunSystemCmd(NULL_FILE, "iwpriv", "wlan0", "set_mib", "authtype=2", NULL_STR);
            RunSystemCmd(NULL_FILE, "iwpriv", "wlan0", "set_mib", "encmode=1", NULL_STR);
            RunSystemCmd(NULL_FILE, "iwpriv", "wlan0", "set_mib", "psk_enable=0", NULL_STR);
            RunSystemCmd(NULL_FILE, "iwpriv", "wlan0", "set_mib", "802_1x=0", NULL_STR);
            sprintf(tmpbuf, "wepdkeyid=%d", pwifi->wep_default_key);
            RunSystemCmd(NULL_FILE, "iwpriv", "wlan0", "set_mib", tmpbuf, NULL_STR);
            sprintf(tmpbuf, "wepkey1=%s", pwifi->wep_key1);
            RunSystemCmd(NULL_FILE, "iwpriv", "wlan0", "set_mib", tmpbuf, NULL_STR);
            sprintf(tmpbuf, "wepkey2=%s", pwifi->wep_key2);
            RunSystemCmd(NULL_FILE, "iwpriv", "wlan0", "set_mib", tmpbuf, NULL_STR);
            sprintf(tmpbuf, "wepkey3=%s", pwifi->wep_key3);
            RunSystemCmd(NULL_FILE, "iwpriv", "wlan0", "set_mib", tmpbuf, NULL_STR);
            sprintf(tmpbuf, "wepkey4=%s", pwifi->wep_key4);
            RunSystemCmd(NULL_FILE, "iwpriv", "wlan0", "set_mib", tmpbuf, NULL_STR);
        }
        else if (strcmp(pwifi->security, "wep128-auto") == 0) {
            // WEP128
            auth = WSC_AUTH_OPEN;
            enc = WSC_ENCRYPT_WEP;
            //authtype: 0-open, 1-shared, 2-auto
            RunSystemCmd(NULL_FILE, "iwpriv", "wlan0", "set_mib", "authtype=2", NULL_STR);
            RunSystemCmd(NULL_FILE, "iwpriv", "wlan0", "set_mib", "encmode=5", NULL_STR);
            RunSystemCmd(NULL_FILE, "iwpriv", "wlan0", "set_mib", "psk_enable=0", NULL_STR);
            RunSystemCmd(NULL_FILE, "iwpriv", "wlan0", "set_mib", "802_1x=0", NULL_STR);
            sprintf(tmpbuf, "wepdkeyid=%d", pwifi->wep_default_key);
            RunSystemCmd(NULL_FILE, "iwpriv", "wlan0", "set_mib", tmpbuf, NULL_STR);
            sprintf(tmpbuf, "wepkey1=%s", pwifi->wep_key1);
            RunSystemCmd(NULL_FILE, "iwpriv", "wlan0", "set_mib", tmpbuf, NULL_STR);
            sprintf(tmpbuf, "wepkey2=%s", pwifi->wep_key2);
            RunSystemCmd(NULL_FILE, "iwpriv", "wlan0", "set_mib", tmpbuf, NULL_STR);
            sprintf(tmpbuf, "wepkey3=%s", pwifi->wep_key3);
            RunSystemCmd(NULL_FILE, "iwpriv", "wlan0", "set_mib", tmpbuf, NULL_STR);
            sprintf(tmpbuf, "wepkey4=%s", pwifi->wep_key4);
            RunSystemCmd(NULL_FILE, "iwpriv", "wlan0", "set_mib", tmpbuf, NULL_STR);
        }
        else if (strcmp(pwifi->security, "wpa-psk-tkip") == 0) {
            // WPA-PSK, TKIP
            auth = WSC_AUTH_WPAPSK;
            enc = WSC_ENCRYPT_TKIP;
            mixedmode = 0;
            wsc_wpa_cipher = 1;
            RunSystemCmd(NULL_FILE, "iwpriv", "wlan0", "set_mib", "authtype=2", NULL_STR);
            RunSystemCmd(NULL_FILE, "iwpriv", "wlan0", "set_mib", "encmode=2", NULL_STR);
            RunSystemCmd(NULL_FILE, "iwpriv", "wlan0", "set_mib", "psk_enable=1", NULL_STR);
            RunSystemCmd(NULL_FILE, "iwpriv", "wlan0", "set_mib", "wpa_cipher=2", NULL_STR);
            sprintf(tmpbuf, "passphrase=%s", pwifi->passphrase);
            RunSystemCmd(NULL_FILE, "iwpriv", "wlan0", "set_mib", tmpbuf, NULL_STR);
        }
        else if (strcmp(pwifi->security, "wpa2-psk-aes") == 0) {
            // WPA2-PSK, AES
            auth = WSC_AUTH_WPA2PSK;
            enc = WSC_ENCRYPT_AES;
            mixedmode = 0;
            wsc_wpa2_cipher = 2;
            RunSystemCmd(NULL_FILE, "iwpriv", "wlan0", "set_mib", "authtype=2", NULL_STR);
            RunSystemCmd(NULL_FILE, "iwpriv", "wlan0", "set_mib", "encmode=2", NULL_STR);
            RunSystemCmd(NULL_FILE, "iwpriv", "wlan0", "set_mib", "psk_enable=2", NULL_STR);
            RunSystemCmd(NULL_FILE, "iwpriv", "wlan0", "set_mib", "wpa2_cipher=8", NULL_STR);
            sprintf(tmpbuf, "passphrase=%s", pwifi->passphrase);
            RunSystemCmd(NULL_FILE, "iwpriv", "wlan0", "set_mib", tmpbuf, NULL_STR);
        }
        else if (strcmp(pwifi->security, "wpa-auto-psk-tkipaes") == 0) {
            // WPA/WPA2 mixed PSK, TKIP/AES
            auth = WSC_AUTH_WPA2PSKMIXED;
            enc = WSC_ENCRYPT_TKIPAES;
            mixedmode = 1;
            wsc_wpa_cipher = 3;
            wsc_wpa2_cipher = 3;
            RunSystemCmd(NULL_FILE, "iwpriv", "wlan0", "set_mib", "authtype=2", NULL_STR);
            RunSystemCmd(NULL_FILE, "iwpriv", "wlan0", "set_mib", "encmode=2", NULL_STR);
            RunSystemCmd(NULL_FILE, "iwpriv", "wlan0", "set_mib", "psk_enable=3", NULL_STR);
            RunSystemCmd(NULL_FILE, "iwpriv", "wlan0", "set_mib", "wpa_cipher=10", NULL_STR);
            RunSystemCmd(NULL_FILE, "iwpriv", "wlan0", "set_mib", "wpa2_cipher=10", NULL_STR);
            sprintf(tmpbuf, "passphrase=%s", pwifi->passphrase);
            RunSystemCmd(NULL_FILE, "iwpriv", "wlan0", "set_mib", tmpbuf, NULL_STR);
        }
    }
    //STA mode
    else if ((strcmp(pwifi->mode, "sta") == 0)
#ifdef P2P_SUPPORT
             || ((strcmp(pwifi->mode, "p2pdev") == 0) && (pwifi->p2pmode==P2P_CLIENT))
#endif
             ) {

#if (ECOS_BAND_SEL_5G == 1)
        /*BAND (network type setting) */
        network_type = (WIRELESS_11AC|WIRELESS_11A|WIRELESS_11N);
        //network_type = (WIRELESS_11B|WIRELESS_11G|WIRELESS_11N);
        //network_type = (WIRELESS_11AC|WIRELESS_11A|WIRELESS_11N);
        //network_type = (WIRELESS_11B|WIRELESS_11G|WIRELESS_11N);
        sprintf(tmpbuf, "band=%d", network_type);
        RunSystemCmd(NULL_FILE, "iwpriv", "wlan0", "set_mib", tmpbuf, NULL_STR);

	phyBandSelect = _PHY_BAND_5G;
        sprintf(tmpbuf, "phyBandSelect=%d", phyBandSelect);
        RunSystemCmd(NULL_FILE, "iwpriv", "wlan0", "set_mib", tmpbuf, NULL_STR);

        /*bandWidth , 20M / 40M /80M*/
        tmpval = _BW40M;
        sprintf(tmpbuf, "use40M=%d", tmpval);
        RunSystemCmd(NULL_FILE, "iwpriv", "wlan0", "set_mib", tmpbuf, NULL_STR);

        /*2ND channel offset  , cowork with use40M,channel*/
        //tmpval = HT_2NDCH_OFFSET_ABOVE;
        /*tmpval = HT_2NDCH_OFFSET_DONTCARE;
        sprintf(tmpbuf, "2ndchoffset=%d", tmpval);
        RunSystemCmd(NULL_FILE, "iwpriv", "wlan0", "set_mib", tmpbuf, NULL_STR);*/

#else
        /*BAND (network type setting) */
        //network_type = (WIRELESS_11AC|WIRELESS_11A|WIRELESS_11N);
	network_type = (WIRELESS_11B|WIRELESS_11G|WIRELESS_11N);
        sprintf(tmpbuf, "band=%d", network_type);
        RunSystemCmd(NULL_FILE, "iwpriv", "wlan0", "set_mib", tmpbuf, NULL_STR);

        phyBandSelect = _PHY_BAND_2G;
		sprintf(tmpbuf, "phyBandSelect=%d", phyBandSelect);
        RunSystemCmd(NULL_FILE, "iwpriv", "wlan0", "set_mib", tmpbuf, NULL_STR);
#endif

        is_client = 1;
#ifdef P2P_SUPPORT
    if ((strcmp(pwifi->mode, "p2pdev") == 0) && (pwifi->p2pmode==P2P_CLIENT)) {
        sprintf(tmpbuf, "opmode=0x%X", (WIFI_P2P_SUPPORT|WIFI_STATION_STATE));
        RunSystemCmd(NULL_FILE, "iwpriv", "wlan0", "set_mib", tmpbuf, NULL_STR);
        sprintf(tmpbuf, "setp2pmode=%d", P2P_CLIENT);
        RunSystemCmd(NULL_FILE, "iwpriv", "wlan0", "p2pcmd", tmpbuf, NULL_STR);
    }
    else
#endif
    {
        RunSystemCmd(NULL_FILE, "iwpriv", "wlan0", "set_mib", "opmode=8", NULL_STR);
    }
        sprintf(tmpbuf, "ssid=%s", pwifi->ssid);
        RunSystemCmd(NULL_FILE, "iwpriv", "wlan0", "set_mib", tmpbuf, NULL_STR);
        if (strcmp(pwifi->security, "none") == 0) {
            // No security
            auth = WSC_AUTH_OPEN;
            enc = WSC_ENCRYPT_NONE;
            RunSystemCmd(NULL_FILE, "iwpriv", "wlan0", "set_mib", "authtype=0", NULL_STR);
            RunSystemCmd(NULL_FILE, "iwpriv", "wlan0", "set_mib", "encmode=0", NULL_STR);
            RunSystemCmd(NULL_FILE, "iwpriv", "wlan0", "set_mib", "psk_enable=0", NULL_STR);
            RunSystemCmd(NULL_FILE, "iwpriv", "wlan0", "set_mib", "802_1x=0", NULL_STR);
        }
        else if (strcmp(pwifi->security, "wep64-auto") == 0) {
            // WEP64
            auth = WSC_AUTH_OPEN;
            enc = WSC_ENCRYPT_WEP;
            //authtype: 0-open, 1-shared, 2-auto
            RunSystemCmd(NULL_FILE, "iwpriv", "wlan0", "set_mib", "authtype=2", NULL_STR);
            RunSystemCmd(NULL_FILE, "iwpriv", "wlan0", "set_mib", "encmode=1", NULL_STR);
            RunSystemCmd(NULL_FILE, "iwpriv", "wlan0", "set_mib", "psk_enable=0", NULL_STR);
            RunSystemCmd(NULL_FILE, "iwpriv", "wlan0", "set_mib", "802_1x=0", NULL_STR);
            sprintf(tmpbuf, "wepdkeyid=%d", pwifi->wep_default_key);
            RunSystemCmd(NULL_FILE, "iwpriv", "wlan0", "set_mib", tmpbuf, NULL_STR);
            sprintf(tmpbuf, "wepkey1=%s", pwifi->wep_key1);
            RunSystemCmd(NULL_FILE, "iwpriv", "wlan0", "set_mib", tmpbuf, NULL_STR);
            sprintf(tmpbuf, "wepkey2=%s", pwifi->wep_key2);
            RunSystemCmd(NULL_FILE, "iwpriv", "wlan0", "set_mib", tmpbuf, NULL_STR);
            sprintf(tmpbuf, "wepkey3=%s", pwifi->wep_key3);
            RunSystemCmd(NULL_FILE, "iwpriv", "wlan0", "set_mib", tmpbuf, NULL_STR);
            sprintf(tmpbuf, "wepkey4=%s", pwifi->wep_key4);
            RunSystemCmd(NULL_FILE, "iwpriv", "wlan0", "set_mib", tmpbuf, NULL_STR);
        }
        else if (strcmp(pwifi->security, "wep128-auto") == 0) {
            // WEP128
            auth = WSC_AUTH_OPEN;
            enc = WSC_ENCRYPT_WEP;
            //authtype: 0-open, 1-shared, 2-auto
            RunSystemCmd(NULL_FILE, "iwpriv", "wlan0", "set_mib", "authtype=2", NULL_STR);
            RunSystemCmd(NULL_FILE, "iwpriv", "wlan0", "set_mib", "encmode=5", NULL_STR);
            RunSystemCmd(NULL_FILE, "iwpriv", "wlan0", "set_mib", "psk_enable=0", NULL_STR);
            RunSystemCmd(NULL_FILE, "iwpriv", "wlan0", "set_mib", "802_1x=0", NULL_STR);
            sprintf(tmpbuf, "wepdkeyid=%d", pwifi->wep_default_key);
            RunSystemCmd(NULL_FILE, "iwpriv", "wlan0", "set_mib", tmpbuf, NULL_STR);
            sprintf(tmpbuf, "wepkey1=%s", pwifi->wep_key1);
            RunSystemCmd(NULL_FILE, "iwpriv", "wlan0", "set_mib", tmpbuf, NULL_STR);
            sprintf(tmpbuf, "wepkey2=%s", pwifi->wep_key2);
            RunSystemCmd(NULL_FILE, "iwpriv", "wlan0", "set_mib", tmpbuf, NULL_STR);
            sprintf(tmpbuf, "wepkey3=%s", pwifi->wep_key3);
            RunSystemCmd(NULL_FILE, "iwpriv", "wlan0", "set_mib", tmpbuf, NULL_STR);
            sprintf(tmpbuf, "wepkey4=%s", pwifi->wep_key4);
            RunSystemCmd(NULL_FILE, "iwpriv", "wlan0", "set_mib", tmpbuf, NULL_STR);
        }
        else if (strcmp(pwifi->security, "wpa2-psk-aes") == 0) {
            // WPA2-PSK, AES
            auth = WSC_AUTH_WPA2PSK;
            enc = WSC_ENCRYPT_AES;
            mixedmode = 0;
            wsc_wpa2_cipher = 2;
            RunSystemCmd(NULL_FILE, "iwpriv", "wlan0", "set_mib", "authtype=2", NULL_STR);
            RunSystemCmd(NULL_FILE, "iwpriv", "wlan0", "set_mib", "encmode=4", NULL_STR);       // 20150624 bug fixed
            RunSystemCmd(NULL_FILE, "iwpriv", "wlan0", "set_mib", "psk_enable=2", NULL_STR);
            RunSystemCmd(NULL_FILE, "iwpriv", "wlan0", "set_mib", "wpa2_cipher=8", NULL_STR);
            sprintf(tmpbuf, "passphrase=%s", pwifi->passphrase);
            RunSystemCmd(NULL_FILE, "iwpriv", "wlan0", "set_mib", tmpbuf, NULL_STR);
        }
    }
    //P2P
#ifdef P2P_SUPPORT
    else if (strcmp(pwifi->mode, "p2pdev") == 0) {
        if (pwifi->p2pmode==P2P_DEVICE) {
            is_client = 1;

            sprintf(tmpbuf, "opmode=0x%X", (WIFI_P2P_SUPPORT|WIFI_STATION_STATE));
            RunSystemCmd(NULL_FILE, "iwpriv", "wlan0", "set_mib", tmpbuf, NULL_STR);

            sprintf(tmpbuf, "setp2pmode=%d", P2P_DEVICE);
            RunSystemCmd(NULL_FILE, "iwpriv", "wlan0", "p2pcmd", tmpbuf, NULL_STR);

            RunSystemCmd(NULL_FILE, "iwpriv", "wlan0", "p2pcmd", "dwellch=1",NULL_STR);   /*listen channel, should be 1 or 6 or11 */
            RunSystemCmd(NULL_FILE, "iwpriv", "wlan0", "p2pcmd", "intent=6",NULL_STR);   /*intent value (0~15) ,the intent of as GO */
            //RunSystemCmd(NULL_FILE, "iwpriv", "wlan0", "p2pcmd", "intent=14",NULL_STR);   /*intent value (0~15) ,the intent of as GO */
            RunSystemCmd(NULL_FILE, "iwpriv", "wlan0", "p2pcmd", "opch=1", NULL_STR);   /*operation channel of GO, if use auto channel don't care this parameter*/
            sprintf(tmpbuf, "devname=%s", pwifi->device_name);
            RunSystemCmd(NULL_FILE, "iwpriv", "wlan0", "p2pcmd", tmpbuf, NULL_STR);/*device name; must same with wps's device name parameter*/
            //RunSystemCmd(NULL_FILE, "iwpriv", "wlan0", "p2pcmd", "setwscmethod=2688", NULL_STR);
            RunSystemCmd(NULL_FILE, "iwpriv", "wlan0", "p2pcmd", "setwscmethod=188", NULL_STR); //0x188 Display,PushButton,Keypad

            /*p2p note ; p2p GO's SSID must in the form "DIRECT-xy-anystring"  xy is random*/
            sprintf(tmpbuf, "ssid=%s", pwifi->ssid);
            RunSystemCmd(NULL_FILE, "iwpriv", "wlan0", "set_mib", tmpbuf, NULL_STR);
            RunSystemCmd(NULL_FILE, "iwpriv", "wlan0", "set_mib", "channel=1", NULL_STR);

            // No security
            auth = WSC_AUTH_OPEN;
            enc = WSC_ENCRYPT_NONE;
            RunSystemCmd(NULL_FILE, "iwpriv", "wlan0", "set_mib", "authtype=0", NULL_STR);
            RunSystemCmd(NULL_FILE, "iwpriv", "wlan0", "set_mib", "encmode=0", NULL_STR);
            RunSystemCmd(NULL_FILE, "iwpriv", "wlan0", "set_mib", "psk_enable=0", NULL_STR);
            RunSystemCmd(NULL_FILE, "iwpriv", "wlan0", "set_mib", "802_1x=0", NULL_STR);
            RunSystemCmd(NULL_FILE, "iwpriv", "wlan0", "set_mib", "use40M=0", NULL_STR);
        }
    }
    else if (strcmp(pwifi->mode, "p2pgo") == 0) {
            is_client = 0;

            /*parameter for wscd*/
            auth = WSC_AUTH_WPA2PSK;
            enc = WSC_ENCRYPT_AES;
            mixedmode = 0;
            wsc_wpa2_cipher = 2;
            /*parameter for wscd*/

            sprintf(tmpbuf, "opmode=0x%X", (WIFI_P2P_SUPPORT|WIFI_AP_STATE));
            RunSystemCmd(NULL_FILE, "iwpriv", "wlan0", "set_mib", tmpbuf, NULL_STR);

            /*p2p note ; take care!! All p2pcmd is valid after interface's opmode include WIFI_P2P_SUPPORT */
            sprintf(tmpbuf, "setp2pmode=%d", P2P_TMP_GO);
            RunSystemCmd(NULL_FILE, "iwpriv", "wlan0", "p2pcmd", tmpbuf, NULL_STR);
            RunSystemCmd(NULL_FILE, "iwpriv", "wlan0", "p2pcmd", "dwellch=1",NULL_STR);   /*listen channel, should be 1 or 6 or11 */
            RunSystemCmd(NULL_FILE, "iwpriv", "wlan0", "p2pcmd", "intent=6",NULL_STR);   /*intent value (0~15) ,the intent of as GO */
            RunSystemCmd(NULL_FILE, "iwpriv", "wlan0", "p2pcmd", "opch=1", NULL_STR);   /*operation channel of GO, if use auto channel don't care this parameter*/
            sprintf(tmpbuf, "devname=%s", pwifi->device_name);
            RunSystemCmd(NULL_FILE, "iwpriv", "wlan0", "p2pcmd", tmpbuf, NULL_STR);/*device name; must same with wps's device name parameter*/
            //RunSystemCmd(NULL_FILE, "iwpriv", "wlan0", "p2pcmd", "setwscmethod=2688", NULL_STR);
            RunSystemCmd(NULL_FILE, "iwpriv", "wlan0", "p2pcmd", "setwscmethod=188", NULL_STR); //0x188 Display,PushButton,Keypad

            /*p2p note ; p2p GO's SSID must in the form "DIRECT-xy-anystring"  xy is random*/
            sprintf(tmpbuf, "ssid=%s", pwifi->ssid);
            RunSystemCmd(NULL_FILE, "iwpriv", "wlan0", "set_mib", tmpbuf, NULL_STR);
            RunSystemCmd(NULL_FILE, "iwpriv", "wlan0", "set_mib", "channel=1", NULL_STR);

            /*p2p note ; p2p GO/GC  must use WPA2AES security*/
            RunSystemCmd(NULL_FILE, "iwpriv", "wlan0", "set_mib", "authtype=2", NULL_STR);
            RunSystemCmd(NULL_FILE, "iwpriv", "wlan0", "set_mib", "encmode=2", NULL_STR);
            RunSystemCmd(NULL_FILE, "iwpriv", "wlan0", "set_mib", "psk_enable=2", NULL_STR);
            RunSystemCmd(NULL_FILE, "iwpriv", "wlan0", "set_mib", "wpa2_cipher=8", NULL_STR);
            sprintf(tmpbuf, "passphrase=%s", pwifi->passphrase);
            RunSystemCmd(NULL_FILE, "iwpriv", "wlan0", "set_mib", tmpbuf, NULL_STR);
    }
#endif
    else {
        return -1;
    }

    //Do not up "wlan0" here. It would be up by interface_config() later.
    //interface_up("wlan0");

    //WPS
    memset(&wpscfg, 0, sizeof(struct wps_config));
    pcfg = &wpscfg;
    //--------------------------------------------------------------------
    strcpy(pcfg->wlan_interface_name, "wlan0");
    //strcpy(pcfg->lan_interface_name, "eth0");
    strcpy(pcfg->lan_interface_name, "wlan0");

    //--------------------------------------------------------------------
    if (is_client != 1) //AP_MODE=0, CLIENT_MODE=1
        pcfg->start = 1;

    //WSC_DEBUG("wlan mode=%d\n",is_client);
    if (is_client == 1) {
#ifdef __ECOS
        intVal = MODE_CLIENT_UNCONFIG;
#else

        if (is_registrar) {
            if (!wifi_settings_sample.is_configured)
                intVal = MODE_CLIENT_UNCONFIG_REGISTRAR;
            else
                intVal = MODE_CLIENT_CONFIG;
        }
        else
            intVal = MODE_CLIENT_UNCONFIG;
#endif
    }
    else {
        if (!wifi_settings_sample.is_configured)
            intVal = MODE_AP_UNCONFIG;
        else
            intVal = MODE_AP_PROXY_REGISTRAR;
    }
    pcfg->mode = intVal;
    //WSC_DEBUG("wsc mode=%d\n",pcfg->mode);

    /*
    if (is_client == 1)
        intVal = 0;
    else
        intVal = 1; // MIB_WLAN_WSC_UPNP_ENABLED
    pcfg->upnp = intVal;
    */
    pcfg->upnp = 0;

#if 1 //def WPS2DOTX
    //pcfg->config_method = (CONFIG_METHOD_VIRTUAL_PIN |  CONFIG_METHOD_PHYSICAL_PBC | CONFIG_METHOD_VIRTUAL_PBC );
    pcfg->config_method = 0x188; //(CONFIG_METHOD_KEYPAD | CONFIG_METHOD_DISPLAY | CONFIG_METHOD_PBC);
#else   // wps 1.0
    //intVal = 0;
    // MIB_WLAN_WSC_METHOD
    // Ethernet(0x2)+Label(0x4)+PushButton(0x80) Bitwise OR
    //1) Pin+Ethernet
    //  intVal = (CONFIG_METHOD_ETH | CONFIG_METHOD_PIN);
    //2) //PBC+Ethernet
    //  intVal = (CONFIG_METHOD_ETH | CONFIG_METHOD_PBC);
    //3) //Pin+PBC+Ethernet
        intVal = (CONFIG_METHOD_ETH | CONFIG_METHOD_PIN | CONFIG_METHOD_PBC);
    pcfg->config_method = intVal;
#endif

    /*assigned auth support list 2.0*/
    pcfg->auth_type_flags = (WSC_AUTH_OPEN |   WSC_AUTH_WPA2PSK);

    /*assigned Encry support list 2.0*/
    pcfg->encrypt_type_flags =
        (WSC_ENCRYPT_NONE |  WSC_ENCRYPT_AES );

    //auth_type, MIB_WLAN_WSC_AUTH
    pcfg->auth_type_flash = auth;

    //encrypt_type, MIB_WLAN_WSC_ENC
    pcfg->encrypt_type_flash = enc;
    if (enc == WSC_ENCRYPT_WEP)
        is_wep = 1;

    pcfg->connect_type = 1; //CONNECT_TYPE_BSS
    pcfg->manual_config = 0;

    //--------------------------------------------------------------------
    if (is_wep) { // only allow WEP in none-MANUAL mode (configured by external registrar)
        pcfg->wep_transmit_key = pwifi->wep_default_key + 1;
        strcpy(pcfg->network_key, pwifi->wep_key1);
        strcpy(pcfg->wep_key2, pwifi->wep_key2);
        strcpy(pcfg->wep_key3, pwifi->wep_key3);
        strcpy(pcfg->wep_key4, pwifi->wep_key4);
    }
    else {
        strcpy(pcfg->network_key, pwifi->passphrase);
    }
    pcfg->network_key_len = strlen(pcfg->network_key);

    strcpy(pcfg->SSID, pwifi->ssid);

    //strcpy(pcfg->pin_code, "12345670");
    strcpy(pcfg->pin_code, pwifi->wsc_pin);

    if (pwifi->channel > 14)
        intVal = 2;
    else
        intVal = 1;
    pcfg->rf_band = intVal;

    pcfg->config_by_ext_reg = pwifi->config_by_ext_reg;

    /*for detial mixed mode info*/
    //  mixed issue
    intVal=0;
    if (mixedmode){ // mixed mode (WPA+WPA2)
        if(wsc_wpa_cipher==1){
            intVal |= WSC_WPA_TKIP;
        }else if(wsc_wpa_cipher==2){
            intVal |= WSC_WPA_AES;
        }else if(wsc_wpa_cipher==3){
            intVal |= (WSC_WPA_TKIP | WSC_WPA_AES);
        }
        if(wsc_wpa2_cipher==1){
            intVal |= WSC_WPA2_TKIP;
        }else if(wsc_wpa2_cipher==2){
            intVal |= WSC_WPA2_AES;
        }else if(wsc_wpa2_cipher==3){
            intVal |= (WSC_WPA2_TKIP | WSC_WPA2_AES);
        }
    }
    pcfg->mixedmode = intVal;
    /*for detial mixed mode info*/

    //wscd.conf
    strcpy(pcfg->device_name, pwifi->device_name);
    strcpy(pcfg->manufacturer, "Realtek Semiconductor Corp.");
    strcpy(pcfg->manufacturerURL, "http://www.realtek.com/");
    strcpy(pcfg->model_URL, "http://www.realtek.com/");
    strcpy(pcfg->model_name, "RTL8xxx");
    strcpy(pcfg->model_num, "RTK_ECOS");
    strcpy(pcfg->serial_num, "123456789012347");
    strcpy(pcfg->modelDescription, "WLAN Access Point");
    strcpy(pcfg->ssid_prefix, "RTKAP_");

    apply_wps_config_to_wscd(pcfg);
    return 0;
}

int wsc_flash_param_handler_sample(struct wsc_flash_param *param)
{
    struct wifi_settings_sample *pwifi =&wifi_settings_sample;

    //printf("%s %d\n", __FUNCTION__, __LINE__);
    pwifi->is_configured = 1;
    strcpy(pwifi->ssid, param->SSID);
    pwifi->config_by_ext_reg = param->WSC_CONFIGBYEXTREG;
    if (param->WEP) {
        //sprintf(temp, "authtype=%d", param->AUTH_TYPE);
        if (param->WEP == 1) //WEP64
            strcpy(pwifi->security, "wep64-auto");
        else
            strcpy(pwifi->security, "wep128-auto");

        pwifi->wep_default_key = param->WEP_DEFAULT_KEY;
        strcpy(pwifi->wep_key1, param->WEP_KEY1);
        strcpy(pwifi->wep_key2, param->WEP_KEY1);
        strcpy(pwifi->wep_key3, param->WEP_KEY1);
        strcpy(pwifi->wep_key4, param->WEP_KEY1);
    }
    else {
        if (param->WSC_AUTH==WSC_AUTH_OPEN) {
            if (param->WSC_ENC==WSC_ENCRYPT_NONE) {
                strcpy(pwifi->security, "none");
            }
        }
        else if (param->WSC_AUTH==WSC_AUTH_WPAPSK) {
            if (param->WSC_ENC==WSC_ENCRYPT_TKIP)
                strcpy(pwifi->security, "wpa-psk-tkip");
            else if (param->WSC_ENC==WSC_ENCRYPT_AES)
                strcpy(pwifi->security, "wpa-psk-aes");
            else if (param->WSC_ENC==WSC_ENCRYPT_TKIPAES)
                strcpy(pwifi->security, "wpa-psk-tkipaes");
            strcpy(pwifi->passphrase, param->WSC_PSK);
        }
        else if (param->WSC_AUTH==WSC_AUTH_WPA2PSK) {
            if (param->WSC_ENC==WSC_ENCRYPT_TKIP)
                strcpy(pwifi->security, "wpa2-psk-tkip");
            else if (param->WSC_ENC==WSC_ENCRYPT_AES)
                strcpy(pwifi->security, "wpa2-psk-aes");
            else if (param->WSC_ENC==WSC_ENCRYPT_TKIPAES)
                strcpy(pwifi->security, "wpa2-psk-tkipaes");
            strcpy(pwifi->passphrase, param->WSC_PSK);
        }
        else if (param->WSC_AUTH==WSC_AUTH_WPA2PSKMIXED) {
            if (param->WSC_ENC==WSC_ENCRYPT_TKIP)
                strcpy(pwifi->security, "wpa-auto-psk-tkip");
            else if (param->WSC_ENC==WSC_ENCRYPT_AES)
                strcpy(pwifi->security, "wpa-auto-psk-aes");
            else if (param->WSC_ENC==WSC_ENCRYPT_TKIPAES)
                strcpy(pwifi->security, "wpa-auto-psk-tkipaes");
            strcpy(pwifi->passphrase, param->WSC_PSK);
        }
    }

#ifdef P2P_SUPPORT
    if (strcmp(pwifi->mode, "p2pdev") == 0) {
    pwifi->p2pmode = param->WLAN0_P2P_TYPE;
    }
#endif
    return 0;
}

//-----------------------------------------------------------------------------
// wps
//-----------------------------------------------------------------------------
void generate_pin_code(char *pinbuf)
{
    struct timeval tod;
    unsigned long num;
    char macaddr[6];

    gettimeofday(&tod , NULL);
    // read mac from efuse
    if (wifi_get_wlan0_efuse_mac(macaddr) < 0) {
        macaddr[4] = 0x89;
        macaddr[5] = 0xe5;
    }
    else {
        unsigned char zero[] = {0, 0, 0, 0, 0, 0};
        //sanity check
        if (!memcmp(macaddr, zero, MACADDRLEN) || IS_MCAST(macaddr)) {
            macaddr[4] = 0x89;
            macaddr[5] = 0xe5;
        }
    }
    tod.tv_sec += macaddr[4]+macaddr[5];
    srand(tod.tv_sec+tod.tv_usec);
    num = rand() % 10000000;
    num = num*10 + compute_pin_checksum(num);
    convert_hex_to_ascii((unsigned long)num, pinbuf);
}

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
#define SYS_WIFI_P2P_INDICATE_PBC_REQ       BIT(23)   /*wlan drv indicate has peer request to do P2P nego by PBC method*/

cyg_flag_t sys_flag;

void wsc_event_cb_func_sample(int evt)
{
    if (evt == WPS_REINIT_EVENT) {
        cyg_flag_setbits(&sys_flag, SYS_REINIT_EVENT);
    }
    else if (evt == WPS_STATUS_CHANGE_EVENT) {
        cyg_flag_setbits(&sys_flag, SYS_WPS_STATUS_CHANGE_EVENT);
    }
    else
        printf("unknown event\n");
}

void print_wscd_status(int status)
{
    switch(status) {
    case NOT_USED:
        printf("NOT_USED\n");
        break;
    case PROTOCOL_START:
        printf("PROTOCOL_START\n");
        break;
    case PROTOCOL_PBC_OVERLAPPING:
        printf("PROTOCOL_PBC_OVERLAPPING\n");
        break;
    case PROTOCOL_TIMEOUT:
        printf("PROTOCOL_TIMEOUT\n");
        break;
    case PROTOCOL_SUCCESS:
        printf("PROTOCOL_SUCCESS\n");
        break;
    case SEND_EAPOL_START:
        printf("SEND_EAPOL_START\n");
        break;
    case RECV_EAPOL_START:
        printf("RECV_EAPOL_START\n");
        break;
    case SEND_EAP_IDREQ:
        printf("SEND_EAP_IDREQ\n");
        break;
    case RECV_EAP_IDRSP:
        printf("RECV_EAP_IDRSP\n");
        break;
    case SEND_EAP_START:
        printf("SEND_EAP_START\n");
        break;
    case SEND_M1:
        printf("SEND_M1\n");
        break;
    case RECV_M1:
        printf("RECV_M1\n");
        break;
    case SEND_M2:
        printf("SEND_M2\n");
        break;
    case RECV_M2:
        printf("RECV_M2\n");
        break;
    case RECV_M2D:
        printf("RECV_M2D\n");
        break;
    case SEND_M3:
        printf("SEND_M3\n");
        break;
    case RECV_M3:
        printf("RECV_M3\n");
        break;
    case SEND_M4:
        printf("SEND_M4\n");
        break;
    case RECV_M4:
        printf("RECV_M4\n");
        break;
    case SEND_M5:
        printf("SEND_M5\n");
        break;
    case RECV_M5:
        printf("RECV_M5\n");
        break;
    case SEND_M6:
        printf("SEND_M6\n");
        break;
    case RECV_M6:
        printf("RECV_M6\n");
        break;
    case SEND_M7:
        printf("SEND_M7\n");
        break;
    case RECV_M7:
        printf("RECV_M7\n");
        break;
    case SEND_M8:
        printf("SEND_M8\n");
        break;
    case RECV_M8:
        printf("RECV_M8\n");
        break;
    case PROC_EAP_ACK:
        printf("PROC_EAP_ACK\n");
        break;
    case WSC_EAP_FAIL:
        printf("WSC_EAP_FAIL\n");
        break;
    case HASH_FAIL:
        printf("HASH_FAIL\n");
        break;
    case HMAC_FAIL:
        printf("HMAC_FAIL\n");
        break;
    case PWD_AUTH_FAIL:
        printf("PWD_AUTH_FAIL\n");
        break;
    case PROTOCOL_PIN_NUM_ERR:
        printf("PROTOCOL_PIN_NUM_ERR\n");
        break;
    case PROC_EAP_DONE:
        printf("PROC_EAP_DONE\n");
        break;
    case PROTOCOL_ERR:
        printf("PROTOCOL_ERR\n");
        break;
    default:
        printf("Unknown wscd status\n");
        break;
    }
}

void wifi_link_status_cb_func_sample(char *pIntfName, int status)
{
    if (status == WIFI_LINK_STATUS_CONNECTED) {
        printf("%s: Connected with AP\n", pIntfName);
        cyg_flag_setbits(&sys_flag, SYS_WIFI_LINK_UP_EVENT);
    }
    else if (status == WIFI_LINK_STATUS_DISCONNECTED) {
        printf("%s: Disconnected with AP\n", pIntfName);
        cyg_flag_setbits(&sys_flag, SYS_WIFI_LINK_DOWN_EVENT);
    }
    else if (status == WIFI_LINK_STATUS_AUTH_FAIL) {
        printf("%s: Fail to authenticate with AP\n", pIntfName);
    }
    else if (status == WIFI_LINK_STATUS_DISASSOCIATED) {
        printf("%s: Disassociated by AP\n", pIntfName);
    }
    else if (status == WIFI_LINK_STATUS_DEAUTHENTICATED) {
        printf("%s: Deauthenticated by AP\n", pIntfName);
    }
    else if (status == WIFI_LINK_STATUS_HANDSHAKE_TIMEOUT) {
        printf("%s: handshake timeout\n", pIntfName);
    }
    else if (status == WIFI_LINK_STATUS_SACN_ZREO_RESULT) {
        //printf("%s: scan no result\n", pIntfName);
    }
    else
        printf("unknown status\n");
}

void wifi_sta_status_cb_func_sample(char *pIntfName, char *pMacAddr, int status)
{
    if (status == WIFI_STA_STATUS_ASSOCIATED) {
        printf("%s: A client(%02X:%02X:%02X:%02X:%02X:%02X) is associated\n",
            pIntfName, *pMacAddr, *(pMacAddr+1), *(pMacAddr+2), *(pMacAddr+3), *(pMacAddr+4), *(pMacAddr+5));
    }
    else if (status == WIFI_STA_STATUS_REASSOCIATED) {
        printf("%s: A client(%02X:%02X:%02X:%02X:%02X:%02X) is reassociated\n",
            pIntfName, *pMacAddr, *(pMacAddr+1), *(pMacAddr+2), *(pMacAddr+3), *(pMacAddr+4), *(pMacAddr+5));
    }
    else if (status == WIFI_STA_STATUS_DISASSOCIATED) {
        printf("%s: A client(%02X:%02X:%02X:%02X:%02X:%02X) is disassociated\n",
            pIntfName, *pMacAddr, *(pMacAddr+1), *(pMacAddr+2), *(pMacAddr+3), *(pMacAddr+4), *(pMacAddr+5));
    }
    else if (status == WIFI_STA_STATUS_DEAUTHENTICATED) {
        printf("%s: A client(%02X:%02X:%02X:%02X:%02X:%02X) is deauthenticated\n",
            pIntfName, *pMacAddr, *(pMacAddr+1), *(pMacAddr+2), *(pMacAddr+3), *(pMacAddr+4), *(pMacAddr+5));
    }
    else if (status == WIFI_STA_STATUS_PORT_AUTHORIZED) {
        printf("%s: Port for a client(%02X:%02X:%02X:%02X:%02X:%02X) is in an authorized state and the client can access network now\n",
            pIntfName, *pMacAddr, *(pMacAddr+1), *(pMacAddr+2), *(pMacAddr+3), *(pMacAddr+4), *(pMacAddr+5));
    }
    else if (status == WIFI_STA_STATUS_AUTH_FAIL) {
        printf("%s: A client(%02X:%02X:%02X:%02X:%02X:%02X) authentication fail\n",
            pIntfName, *pMacAddr, *(pMacAddr+1), *(pMacAddr+2), *(pMacAddr+3), *(pMacAddr+4), *(pMacAddr+5));
    }
    else if (status == WIFI_STA_STATUS_EXPIRED) {
        printf("%s: A client(%02X:%02X:%02X:%02X:%02X:%02X) is expired\n",
            pIntfName, *pMacAddr, *(pMacAddr+1), *(pMacAddr+2), *(pMacAddr+3), *(pMacAddr+4), *(pMacAddr+5));
    }
    else if (status == WIFI_AP_READY) {
        printf("%s: AP is ready \n", pIntfName);
    }
    else
        printf("unknown status\n");
}

#ifdef P2P_SUPPORT
void wifi_p2p_event_indicate_cb_func_sample(char *pIntfName,  int status)
{
    if (status==WIFI_P2P_EVENT_BACK2DEV) {
        cyg_flag_setbits(&sys_flag, SYS_WIFI_P2P_BACK2DEV);
    }
    else if (status==WIFI_P2P_EVENT_START_DHCPD) {
        cyg_flag_setbits(&sys_flag, SYS_WIFI_P2P_START_DHCPD);
    }
    //20171002 nash: prevent build error
    #if 0
    else if (status==WIFI_P2P_EVENT_INDICATE_PBC_REQ) {
        cyg_flag_setbits(&sys_flag, SYS_WIFI_P2P_INDICATE_PBC_REQ);
        TESTDEBUG("wlan drv indicate has peer request to do P2P nego by PBC method\n");

        /*1 issue : iwpriv wlan0 p2pcmd pbcconf*/
        //drv_p2p_cmd("pbcconf");
        /*2 call function drvmib_set("wlan0","");*/
    }
    #endif

    #if 0
    else if (status==WIFI_P2P_EVENT_SWITCH_toPreGO) {
        cyg_flag_setbits(&sys_flag, SYS_WIFI_P2P_START_DHCPD);
    }
    else if (status==WIFI_P2P_EVENT_SWITCH_topreClient) {
        cyg_flag_setbits(&sys_flag, SYS_WIFI_P2P_START_DHCPD);
    }
    #endif
    //else if(status==WIFI_P2P_EVENT_START_DHCPC) {
    //    cyg_flag_setbits(&sys_flag, SYS_WIFI_P2P_START_DHCPC);
    //}
}
#endif

int delete_interface_addr(char *intf)
{
    struct ifreq ifr;
    int s=-1;
    int retcode = -1;

    s = socket(AF_INET, SOCK_DGRAM, 0);
    if (s < 0) {
        perror("socket");
        goto out;
    }

    //delete previous address first
    strcpy(ifr.ifr_name, intf);
    if (ioctl(s, SIOCDIFADDR, &ifr) < 0) {
        if (errno == EADDRNOTAVAIL) {
            /* means no previous address for interface */
        } else {
            perror("SIOCDIFADDR");
            goto out;
        }
    }
    retcode = 0;

 out:
    if (s != -1)
      close(s);
    return retcode;
}

void sample_sys_main(cyg_addrword_t data)
{
    cyg_flag_value_t flag_val;
    char macaddr[6];

    cyg_flag_init(&sys_flag);

    memset(&wifi_settings_sample, 0, sizeof(struct wifi_settings_sample));
    init_wifi_settings("sta", "none"); //for test purpose
    //init_wifi_settings("ap", "none");
    //wifi_settings_sample.wlan_disable = 1;
    //wifi_settings_sample.wps_disable = 1;
    //wifi_settings_sample.is_configured = 1;

    // Generate WPS PIN number
    generate_pin_code(wifi_settings_sample.wsc_pin);
    //strcpy(wifi_settings_sample.wsc_pin,"11223344");
    //strcpy(wifi_settings_sample.wsc_pin,"12345670");
    //printf("wifi_settings_sample.wsc_pin=%s\n", wifi_settings_sample.wsc_pin);

    configure_wifi_sample(&wifi_settings_sample);
    register_wsc_flash_param_cb(wsc_flash_param_handler_sample);
    register_wsc_event_cb(wsc_event_cb_func_sample);

    //register link status cb
    register_wifi_link_status_cb("wlan0", wifi_link_status_cb_func_sample);

    //register sta status cb
    register_wifi_sta_status_cb("wlan0", wifi_sta_status_cb_func_sample);

    //register p2p event indicate cb function
#ifdef P2P_SUPPORT
    register_p2p_event_indicate_cb("wlan0", wifi_p2p_event_indicate_cb_func_sample);
#endif

    // mac address setting
    if (wifi_get_wlan0_efuse_mac(macaddr) < 0) {
        printf("wifi_get_wlan0_efuse_mac() fail. Use hardcoded mac.\n");
        set_mac_address("wlan0", "\x00\x00\x00\x81\x89\xe5");
    }
    else {
        unsigned char zero[] = {0, 0, 0, 0, 0, 0};

        printf("wlan0 efuse mac [%02x:%02x:%02x:%02x:%02x:%02x]\n",
            macaddr[0], macaddr[1], macaddr[2],
            macaddr[3], macaddr[4], macaddr[5]);
        //sanity check
        if (!memcmp(macaddr, zero, MACADDRLEN) || IS_MCAST(macaddr)) {
            printf("efuse mac format is invalid. Use hardcoded mac.\n");
            set_mac_address("wlan0", "\x00\x00\x00\x81\x89\xe5");
        }
        else {
            set_mac_address("wlan0", macaddr);
        }
    }
    #if 0   /*test API for set value to efuse */
    {
        if(wifi_set_HT40_1s_A_to_efuse("2b2a2a2a2a2a2a2a292929292930")<0){
            printf("[%s %d]fail\n",__func__,__LINE__);
        }
        if(wifi_set_wlan0_efuse_mac("00e04c112233")<0){
            printf("[%s %d]fail\n",__func__,__LINE__);
        }
        if(wifi_set_cck_pwr_to_efuse("2525252525252626252525252524")<0){
            printf("[%s %d]fail\n",__func__,__LINE__);
        }
        if(wifi_set_xcap_to_efuse("1f")<0){
            printf("[%s %d]fail\n",__func__,__LINE__);
        }
        if(wifi_set_pwrDiff_ht20ofdm_to_efuse("02")<0){
            printf("[%s %d]fail\n",__func__,__LINE__);
        }
        if(wifi_set_ther_to_efuse("19")<0){
            printf("[%s %d]fail\n",__func__,__LINE__);
        }

        if(wifi_set_xcap_to_efuse("2e")<0){
            printf("[%s %d]fail\n",__func__,__LINE__);
        }

    }
    #endif


    // ip address setting
    if (strcmp(wifi_settings_sample.mode, "ap") == 0) {
        interface_config("wlan0", "192.168.1.254", "255.255.255.0");
        //init_all_network_interfaces_statically();
        //interface_config2("wlan0", "192.168.1.254", "255.255.255.0");
    }
    else if (strcmp(wifi_settings_sample.mode, "sta") == 0) {
        if (wifi_settings_sample.is_dhcp) {
        delete_interface_addr("wlan0");
        interface_up("wlan0");
        }
        else {
            interface_config("wlan0", "192.168.1.189", "255.255.255.0");
            //interface_config2("wlan0", "192.168.1.189", "255.255.255.0");
        }
    }

    //wps
    if (!wifi_settings_sample.wlan_disable) {
        if (!wifi_settings_sample.wps_disable) {
            create_wscd();
        }
    }

    printf("[ATD] init process finished \n");

    while (1) {
        flag_val = cyg_flag_wait(&sys_flag, 0xffffffff, CYG_FLAG_WAITMODE_OR | CYG_FLAG_WAITMODE_CLR);
        //printf("sys_flag=%x\n", flag_val);
        if (flag_val & SYS_RESET_EVENT) {
            //reboot
            HAL_PLATFORM_RESET();
        }
        else if (flag_val & SYS_REINIT_EVENT) {
            cyg_thread_info tinfo;

            //diag_printf("%s %d\n", __FUNCTION__, __LINE__);
            //receive event from wsc
            //printf("system reinit\n");
            //reinit wlan
            configure_wifi_sample(&wifi_settings_sample);

            // ip address setting
            if (strcmp(wifi_settings_sample.mode, "ap") == 0) {
                interface_up("wlan0");
                //interface_config("wlan0", "192.168.1.254", "255.255.255.0");
                //init_all_network_interfaces_statically();
                //interface_config2("wlan0", "192.168.1.254", "255.255.255.0");
            }
            else if (strcmp(wifi_settings_sample.mode, "sta") == 0) {
                if (wifi_settings_sample.is_dhcp) {
            delete_interface_addr("wlan0");
            interface_up("wlan0");
            }
            else {
                    interface_up("wlan0");
                    //interface_config("wlan0", "192.168.1.189", "255.255.255.0");
                    //interface_config2("wlan0", "192.168.1.189", "255.255.255.0");
                }
            }
#ifdef P2P_SUPPORT
            else if (strcmp(wifi_settings_sample.mode, "p2pgo") == 0) {
                if (wifi_settings_sample.p2pmode==P2P_TMP_GO) {
                    delete_interface_addr("wlan0");
                    interface_config("wlan0", "192.168.1.254", "255.255.255.0");
                    printf("Starting DHCPD(reinit)...\n");
                    //make sure dhcpc has closed
                    eCos_Dhcp_Client_Stop();
                    //make sure dhcpd has closed
                    eCos_Dhcp_Server_Stop(0);
                    //start dhcpd
                    eCos_Dhcp_Set_Interface("wlan0");
                    eCos_Dhcp_Server_Start("DhcpSrv01");
                }
            }
            else if (strcmp(wifi_settings_sample.mode, "p2pdev") == 0) {
                if (wifi_settings_sample.p2pmode==P2P_DEVICE) {
                    delete_interface_addr("wlan0");
                    interface_up("wlan0");
                }
                else if (wifi_settings_sample.p2pmode==P2P_CLIENT) {
                    delete_interface_addr("wlan0");
                    interface_up("wlan0");
#if 0
                    printf("Starting DHCPC...\n");
                    //make sure dhcpc has closed
                    eCos_Dhcp_Client_Stop();
                    //make sure dhcpd has closed
                    eCos_Dhcp_Server_Stop(0);
                    //start dhcpc
                    wifi_ignore_down_up("wlan0", 1);
                    eCos_Dhcp_Client_Start("DhcpCli01", 0, 1);
                    wifi_ignore_down_up("wlan0", 0);
#endif
                }
            }
#endif

//#ifdef HAVE_WPS
            if (get_thread_info_by_name("wscd", &tinfo)) {
                if (!wifi_settings_sample.wlan_disable && !wifi_settings_sample.wps_disable)
                    wsc_reinit(); //trigger wsc to reinit
                else
                    wsc_stop_service(); //trigger wsc to stop service
            }
            else {
                if (!wifi_settings_sample.wlan_disable && !wifi_settings_sample.wps_disable)
                    create_wscd();
            }
//#endif
        }
        else if (flag_val & SYS_WPS_STATUS_CHANGE_EVENT) {
            int status;
            status = get_wscd_status();
            printf("[%d] ", status);
            print_wscd_status(status);
        }
#ifdef P2P_SUPPORT
        else if (flag_val & SYS_WIFI_P2P_BACK2DEV) {
            if (strcmp(wifi_settings_sample.mode, "p2pdev") == 0) {
                init_wifi_settings("p2pdev", "");
                cyg_flag_setbits(&sys_flag, SYS_REINIT_EVENT);
            }
        }
        else if (flag_val & SYS_WIFI_P2P_START_DHCPD) {
            delete_interface_addr("wlan0");
            interface_config("wlan0", "192.168.1.254", "255.255.255.0");
            printf("Starting DHCPD...\n");
            //make sure dhcpc has closed
            eCos_Dhcp_Client_Stop();
            // close dhcp server
            eCos_Dhcp_Server_Stop(0);
            //start dhcp server
            eCos_Dhcp_Set_Interface("wlan0");
            //start dhcpd
            eCos_Dhcp_Server_Start("DhcpSrv01");
        }
#endif
        else if (flag_val & SYS_WIFI_LINK_UP_EVENT) {
            show_bss_info("wlan0");
            if (((strcmp(wifi_settings_sample.mode, "sta") == 0) && wifi_settings_sample.is_dhcp)
#ifdef P2P_SUPPORT
                || ((strcmp(wifi_settings_sample.mode, "p2pdev") == 0) &&
                (wifi_settings_sample.p2pmode==P2P_CLIENT))
#endif
                ) {
                printf("Starting DHCPC...\n");
#ifdef P2P_SUPPORT
                //make sure dhcpc has closed
                eCos_Dhcp_Client_Stop();
                //make sure dhcpd has closed
                eCos_Dhcp_Server_Stop(0);
                //start dhcpc
                wifi_ignore_down_up("wlan0", 1);
                eCos_Dhcp_Client_Start("DhcpCli01", 0, 1);
                wifi_ignore_down_up("wlan0", 0);
#endif
            }
        }
        else if (flag_val & SYS_WIFI_LINK_DOWN_EVENT) {
            show_bss_info("wlan0");
        }
        else {
            printf("invalid sys_flag value: 0x%x\n", flag_val);
        }
    }
}

static int hex2num(char c)
{
        if (c >= '0' && c <= '9')
            return c - '0';
        if (c >= 'a' && c <= 'f')
            return c - 'a' + 10;
        if (c >= 'A' && c <= 'F')
            return c - 'A' + 10;
        return -1;
}

int hex2byte(const char *hex)
{
        int a, b;
        a = hex2num(*hex++);
        if (a < 0)
            return -1;
        b = hex2num(*hex++);
        if (b < 0)
            return -1;
        return (a << 4) | b;
}

int hexstr2bin(char *hex, char *buf, size_t len)
{
        size_t i;
        int a;
        char *ipos = hex;
        char *opos = buf;

        for (i = 0; i < len; i++) {
            a = hex2byte(ipos);
            if (a < 0)
                return -1;
            *opos++ = a;
            ipos += 2;
        }
        return 0;
}

void create_sample_sys_thread(void)
{

    if(rtk_testcmd_initialized != 0)
    {
       printf("[ATD] init process finished \n");
        return;
    }

    rtk_testcmd_initialized = 1;

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
// wps test command
//-----------------------------------------------------------------------------
MAINFUNC_ENTRY(wps,argc,argv)
{
    if ((argc != 2) && (argc != 3)) {
        goto show_help;
    }
    if (argc == 3) {
        if (strcmp(argv[1], "client-pin") == 0) {
            if (strlen(argv[2]) == 8) {
                //char temp[16];
                //sprintf(temp, "pin=%s", argv[2]);
                //RunSystemCmd(NULL_FILE, "iwpriv", "wlan0", "set_mib", temp, NULL_STR);
                if (drvmib_set("pin", argv[2]) < 0)
                    printf("Fail to set client pin\n");
            }
            else {
                printf("invaild pin len\n");
            }
        }
        else if (strcmp(argv[1], "ap-mac") == 0) {
            if (strlen(argv[2]) == 12) {
                //char temp[32];
                //sprintf(temp, "wsc_specmac=%s", argv[2]);
                //RunSystemCmd(NULL_FILE, "iwpriv", "wlan0", "set_mib", temp, NULL_STR);
                if (drvmib_set("wsc_specmac", argv[2]) < 0)
                    printf("Fail to set specmac\n");
            }
            else {
                printf("invaild mac len\n");
            }
        }
        else if (strcmp(argv[1], "ap-ssid") == 0) {
            if (strlen(argv[2]) <= 32) {
                if (drvmib_set("wsc_specssid", argv[2]) < 0)
                    printf("Fail to set specssid\n");
            }
            else {
                printf("invaild ssid len\n");
            }
        }
    }
    else if (argc == 2) {
        if (strcmp(argv[1], "ctx") == 0) {
            dump_wsc_context(NULL);
        }
        else if (strcmp(argv[1], "flash_param") == 0) {
            dump_wsc_flash_param();
        }
        else if (strcmp(argv[1], "unlock") == 0) {
            if (get_wscd_lock_stat()) {
                wsc_unlock(); //unlock "auto lock down"
            }
        }
        else if (strcmp(argv[1], "start-pbc") == 0) {
            wsc_sig_pbc();
        }
        else if (strcmp(argv[1], "start-pin") == 0) {
            wsc_sig_pin();
        }
        else if (strcmp(argv[1], "stop-wsc") == 0) {
            wsc_sig_stop();
        }
        /*else if (strcmp(argv[1], "reinit") == 0) {
            wsc_reinit();
        }
        else if (strcmp(argv[1], "gen-pin") == 0) {
            char tmpbuf[16];
            generate_pin_code(tmpbuf);
            printf("PIN: %s\n", tmpbuf);
        }*/
        else {
            goto show_help;
        }
    }
    return 0;

show_help:
    printf("\n");
    if (strcmp(wifi_settings_sample.mode, "ap") == 0)
        printf("WPS Status: %s\n", (wifi_settings_sample.is_configured ? "Configured" : "UnConfigured"));
    printf("Auto-lock-down state: %s\n", (get_wscd_lock_stat() ? "Locked" : "Unlocked"));
    printf("Self-PIN Number: %s\n", wifi_settings_sample.wsc_pin);
    printf("wscd_state=%d\n", get_wscd_state());
    printf("wscd_status=%d\n", get_wscd_status());

    printf("\n");
    printf("wps start-pbc\n");
    printf("wps start-pin\n");        // use in client mode
    printf("wps client-pin 12345670\n");  // use in AP mode
    printf("wps ap-mac  00ee4c819689\n"); // use in client mode. only take effect once
    printf("wps ap-ssid 000ecos\n");      // use in client mode. only take effect once
    printf("wps stop-wsc\n");
    printf("wps unlock\n");               // unlock "auto lock down"

    // below is for debug
    //printf("wps reinit\n");
    //printf("wps gen-pin\n");
    printf("wps ctx\n");
    printf("wps flash_param\n");

    return 0;
}

//-----------------------------------------------------------------------------
// wifi test command
//-----------------------------------------------------------------------------
MAINFUNC_ENTRY(wifi,argc,argv)
{
    int i;
    if (argc < 2)
        goto show_help;

    if (argc == 2) {
        if (strcmp(argv[1], "init") == 0) {
            //20170925 nash: adjust driving strength
            cyg_uint32 reg;
            HAL_READ_UINT32(0xC0030068, reg);
            reg &= (0x00FFFFFF);
            reg |= (0x12000000);
            HAL_WRITE_UINT32(0xC0030068, reg);

            HAL_READ_UINT32(0xC003006C, reg);
            reg &= (0xFFFF0000);
            reg |= (0x00001111);
            HAL_WRITE_UINT32(0xC003006C, reg);

            if (wifi_init_wlan0_netdev(0) < 0)
                printf("wifi_init_wlan0_netdev() fail\n");
            else {
                printf("wifi_init_wlan0_netdev() success\n");
                create_sample_sys_thread();
            }
            return 0;
        }
        if (strcmp(argv[1], "stop") == 0) {
            delete_interface_addr("wlan0");
            wsc_stop_service();
            interface_down("wlan0");
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
        else if (strcmp(argv[1], "bss_info") == 0) {
            show_bss_info("wlan0");
            return 0;
        }
        else if (strcmp(argv[1], "status") == 0) {
            show_bss_status("wlan0");
            return 0;
        }
        else if (strcmp(argv[1], "dhcpd") == 0) {
            eCos_Dhcp_Set_Interface("wlan0");
            eCos_Dhcp_Server_Start("DhcpSrv01");
            return 0;
        }
        else if (strcmp(argv[1], "dhcpc") == 0) {
            //make sure dhcpc has closed
            eCos_Dhcp_Client_Stop();
            //make sure dhcpd has closed
            eCos_Dhcp_Server_Stop(0);
            //start dhcpc
            wifi_ignore_down_up("wlan0", 1);
            eCos_Dhcp_Client_Start("DhcpCli01", 0, 1);
            wifi_ignore_down_up("wlan0", 0);
            return 0;
        }
#if (ECOS_SET_CRYSTAL)
        else if (strcmp(argv[1], "crystal") == 0) {
            unsigned char tmp001 = 0x22;

            tmp001 = wifi_get_cystalclk_to_efuse();

            printf("crystalclk=[%02x]\n",tmp001);

            wifi_set_cystalclk_to_efuse("50"); //  hex  string

            tmp001 = wifi_get_cystalclk_to_efuse();

            printf("crystalclk=[%02x]\n",tmp001);

            return 0;
        }
#endif
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
#ifdef P2P_SUPPORT
        else if ((strcmp(argv[1], "p2pdev") == 0) || (strcmp(argv[1], "p2pgo") == 0)) {
            init_wifi_settings(argv[1], "");
            cyg_flag_setbits(&sys_flag, SYS_REINIT_EVENT);
            return 0;
        }
        else if (strcmp(argv[1], "p2pscan") == 0) {
            do_p2p_scan("wlan0");
            return 0;
        }
#endif
        else {
            goto show_help;
        }
    }
    else if (argc == 3) {
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
#ifdef P2P_SUPPORT
        else if (strcmp(argv[1], "p2pconnect") == 0) {
            //unsigned char res;
            int idx = atoi(argv[2]);
            if ((idx < 0) || (idx >= ss_status.number)) {
                printf("wifi p2pconnect <0~%d>\n", (ss_status.number-1));
                printf("input argc[2]=[%d]\n", idx);
            }
            else {
                P2P_PROVISION_COMM_T p2pProvisionComm;
                printf("p2pconnect to [%X:%X:%X:%X:%X:%X] \n",ss_status.bssdb[idx].p2paddress[0],ss_status.bssdb[idx].p2paddress[1]
                    ,ss_status.bssdb[idx].p2paddress[2],ss_status.bssdb[idx].p2paddress[3],
                    ss_status.bssdb[idx].p2paddress[4],ss_status.bssdb[idx].p2paddress[5]);

                //p2pProvisionComm.wsc_config_method = CONFIG_METHOD_DISPLAY;
                //p2pProvisionComm.wsc_config_method = CONFIG_METHOD_KEYPAD;
                p2pProvisionComm.wsc_config_method = CONFIG_METHOD_PBC;
                p2pProvisionComm.channel = ss_status.bssdb[idx].channel;
                memcpy(p2pProvisionComm.dev_address, ss_status.bssdb[idx].p2paddress, 6);
                if (sendP2PProvisionCommInfo("wlan0", &p2pProvisionComm) < 0) {
                    printf("SendP2PProvisionCommInfo fail\n");
                }
            }
            return 0;
        }
#endif
    }
    else if (argc >= 4) {
        if (strcmp(argv[1], "set") == 0) {

            if(strcmp(argv[2],"ssid")==0 ){
                 strcpy((char *)gssid, argv[3]);
                 printf("[ATD] ssid=%s\n", gssid);
                 return 0;
            }
            else if(strcmp(argv[2],"auth")==0){
                 const WIFI_SECURITY_MODE_T *p_wifi_security_cfg = wifi_security_tbl;
                 int valid=0;

                 while( p_wifi_security_cfg->str_security_mode != NULL){
                    if (strcmp(argv[3], p_wifi_security_cfg->str_security_mode) != 0) {
                        p_wifi_security_cfg++;
                        continue;
                    }
                    valid = 1;
                    break;
                 }
		 if (valid == 1){
                    strcpy((char *)gauth, argv[3]);
                    printf("[ATD] auth=%s\n", gauth);
                 }
                 else
                    printf("[ATD] unsupport auth:%s\n", argv[3]);

                 return 0;
            }
            else if(strcmp(argv[2],"psk")==0){
                 char * buf = argv[3];
                 int len;
                 if ( *buf== '"') {
                    const char *pos;
                    buf++;
                    pos = strrchr(buf, '"');
                    if (pos)
                       len = pos - buf;
                    else
                       len = strlen(buf);

                    if (len < 8 || len > 64) {
                       printf("Invalid passphrase length %lu (expected: 8..64) '%s'. \n",
                                (unsigned long) len, buf);
                       return -1;
                    }
                    strncpy((char *)gpsk, buf, len);
                    gpsk[len]=0;
                 }
                 else{
                    len = strlen(buf);

                    if (len < 8 || len > 64){
                        printf("Invalid passphrase length %lu (expected: 8..64) '%s'. \n",
                                   (unsigned long) len, buf);
                        return -1;
                    }
                    strcpy((char *)gpsk, argv[3]);
                 }
                 printf("[ATD] psk=%s\n", gpsk);
                 return 0;
             }
             else if(strcmp(argv[2],"wepkey")==0){
                 char * buf = argv[3];
                 int len;
                 if ( *buf== '"') {

                     printf("[ATD] not support ! \n");
                     return 0;

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
                     strncpy((char *)wep_key, buf, len);
                     wep_key[len]=0;
                 }
                 else{
                     len = strlen(buf);

                     if (len && len != 10 && len != 26 && len != 32) {
                         printf("Invalid WEP hex key len %d (expected: 10 || 26 || 32) \n", len);
                         return -1;
                     }

                     if(hexstr2bin(buf, (char *)wep_key, len/2)){
                         printf("Invalid PSK '%s' (hex len:%d) \n", buf, len);
                         return -1;
                     }
                     strcpy((char *)wep_key, buf);
                 }
                 printf("[ATD] wepkey=%s\n", wep_key);
                 return 0;
            }
            else if(strcmp(argv[2],"wepkey_idx")==0){
                 int wep_idx;

                 wep_idx = atoi(argv[3]);
                 if(wep_idx>3 || wep_idx<0 ){
                     printf("Invalid wep key index (expected: 0~3) \n");
                     return -1;
                 }

                 wep_key_idx = wep_idx;
                 printf("[ATD] wepkey_idx=%d\n", wep_key_idx);
                 return 0;
            }
            else if(strcmp(argv[2],"chan")==0){
                 int ch;
                 int supported_ch=0;
                 ch = atoi(argv[3]);

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
            else
                printf("Please set parameter: ssid / auth / psk / wepkey / wepkey_idx / chan \n");
        }
		else if(strcmp(argv[1], "ap") == 0){
            if(strcmp(argv[2],"setssid")==0 ){
                 strcpy((char *)gssid, argv[3]);
                 printf("[ATD] ssid=%s\n", gssid);
                 return 0;
            }
            else if(strcmp(argv[2],"setpwd")==0){
                 strcpy((char *)gpsk, argv[3]);
                 printf("[ATD] psk=%s\n", gpsk);
                 return 0;
            }
		}
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
        else if (strcmp(argv[2], "start")==0)
        {
            if (strcmp(argv[1], "sta") == 0)
            {
                  if (argc==4) {
                     if (strcmp(argv[3], "dhcp") == 0)
                         wifi_settings_sample.is_dhcp = 1; //dhcp client
                     else
                         wifi_settings_sample.is_dhcp = 0; //static ip
                  }
                  else
                     wifi_settings_sample.is_dhcp = 0; //static ip
            }
	    start_wifi_settings(argv[1], gauth);
	    cyg_flag_setbits(&sys_flag, SYS_REINIT_EVENT);
	    printf("[ATD] wifi cmd result=1 \n");
            return 0;
	}
    }

show_help:
    printf("wifi init\n");
    printf("wifi stop\n");
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
    printf("wifi join <0~%d>\n", (MAX_BSS_DESC-1));
    printf("wifi bss_info\n");
    printf("wifi dhcpd\n");
    printf("wifi dhcpc\n");
#if (ECOS_SET_CRYSTAL)
    printf("wifi crystal\n");
#endif
    printf("wifi ap setssid x\n");
    printf("wifi ap setpwd x\n");

#ifdef P2P_SUPPORT
    printf("wifi p2pgo\n");
    printf("wifi p2pdev\n");
    printf("wifi p2pscan\n");
    printf("wifi p2pconnect <0~%d>\n", (MAX_BSS_DESC-1));
#endif
    return 0;
}

#endif //#if (defined (__ECOS))
