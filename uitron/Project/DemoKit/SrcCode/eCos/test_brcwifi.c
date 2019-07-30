#if (defined (__ECOS))

#include <network.h>
#include <cyg/infra/cyg_type.h>
#include <cyg/infra/diag.h>
#include <cyg/io/devtab.h>
#include <cyg/hal/hal_io.h>
#include <cyg/hal/hal_intr.h>
#include <cyg/hal/plf_io.h>
#include <cyg/kernel/kapi.h>
#include <cyg/io/io.h>
#include <cyg/hal/hal_arch.h>

//#include <cyg/kernel/kapi.h>

#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <cyg/infra/mainfunc.h>
#include <net/dhcpelios/dhcpelios.h>
#include <dhcp.h>
#include <cyg/io/sdio_linux.h>
#include <cyg/io/brcm_wifi.h>
#include <brcm_fop.h>
#include <arpa/inet.h>

#define ECOS_DHCP      1
#define wps            1

// for client mode
#define WIFI_LINK_STATUS_DISCONNECTED   0
#define WIFI_LINK_STATUS_CONNECTED      1
#define WIFI_LINK_STATUS_AUTH_FAIL      2
#define WIFI_LINK_STATUS_DISASSOCIATED  3
#define WIFI_LINK_STATUS_DEAUTHENTICATED    4
#define WIFI_LINK_STATUS_HANDSHAKE_TIMEOUT  5
/*when indicate this event means once scan done and no any result*/
#define WIFI_LINK_STATUS_SACN_ZREO_RESULT   6
#define WIFI_LINK_STATUS_CONNECTED_1      7


// for AP mode
#define WIFI_STA_STATUS_ASSOCIATED  1
#define WIFI_STA_STATUS_REASSOCIATED    2
#define WIFI_STA_STATUS_DISASSOCIATED   3
#define WIFI_STA_STATUS_DEAUTHENTICATED 4
#define WIFI_STA_STATUS_PORT_AUTHORIZED 5
#define WIFI_STA_STATUS_AUTH_FAIL       6
#define WIFI_STA_STATUS_EXPIRED     7
#define WIFI_AP_READY               8


char gapssid[33] = "brcm_ap";
char gappwd[65] = "12345678";

char gstassid[33] = "dlink";
char gstapwd[65] = "12345678";

char gipaddr[16]="192.168.1.254";
char gpsk[65] = "12345678";
char gssid[33] = "brcm_ap";  
char gauth[15] = "wpa2-psk-aes";
char gchan[]="7";	
char wep_key[65] = "1234567890";
char wep_key_idx[] ="0";

int chan_2G_index=14;
int chan_5G_index=11;

int chan_2G_list[14]={0,1,2,3,4,5,6,7,8,9,10,11,12,13};
int chan_5G_list[11]={36,40,44,48,149,151,153,157,159,161,165};

unsigned char gch = 1;
unsigned char ght = 20;
unsigned char gplusminus = 0;
unsigned char gdtim = 1;
int existIP =0;

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

#if ECOS_DHCP
bool geCosDhcpCliOpened = false;
bool geCosDhcpSrvOpened = false;
char geCosNvtNetHostNameCli[CYGNUM_NET_DHCP_OPTION_HOST_NAME_LEN] = "nvtdsc01cli";
char geCosNvtNetHostNameSrv[CYGNUM_NET_DHCP_OPTION_HOST_NAME_LEN] = "nvtdsc01srv";

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

int interface_config(const char *intf, char *addr, char *netmask)
{
    struct sockaddr_in *addrp;
    struct ifreq ifr;

    int s=-1;
    int one = 1;
    //struct in_addr bcaddr, gateway;
    int retcode = -1;

    s = socket(AF_INET, SOCK_DGRAM, 0);
    if (s < 0) {
        perror("socket");
        goto out;
    }

    if (setsockopt(s, SOL_SOCKET, SO_BROADCAST, &one, sizeof(one))) {
        perror("setsockopt");
        goto out;
    }

    addrp = (struct sockaddr_in *) &ifr.ifr_addr;

    if(existIP)
    {
        // get the specific address that was used
        strcpy(ifr.ifr_name, intf);
        addrp->sin_family = AF_INET;
        addrp->sin_len = sizeof(*addrp);
        addrp->sin_port = 0;
        if (ioctl(s, SIOCGIFADDR, &ifr))
        {
                perror("SIOCGIFADDR 1 Srv");
                //goto out;
        }
		
        // Remove any existing address	
        strcpy(ifr.ifr_name, intf);
        addrp->sin_family = AF_INET;
        addrp->sin_len = sizeof(*addrp);
        addrp->sin_port = 0;
        //addrp->sin_addr.s_addr = INADDR_ANY;
        if (ioctl(s, SIOCDIFADDR, &ifr))
        {	
                /* delete IF addr */
                perror("SIOCDIFADDR-Srv");
                //goto out;
        }
    }


    memset(addrp, 0, sizeof(*addrp));

    addrp->sin_family = AF_INET;

    addrp->sin_len = sizeof(*addrp);

    addrp->sin_port = 0;

    //addrp->sin_addr.s_addr = INADDR_ANY;

    if (inet_aton(addr, &(addrp->sin_addr)) == 0) {
        printf("bad value: %s", addr);
        goto out;
    }

    // Must do this temporarily with default route and netmask so that

    // [sub]netmask can be set.

    strcpy(ifr.ifr_name, intf);

    if (ioctl(s, SIOCSIFADDR, &ifr)) { // set ifnet address
        perror("SIOCSIFADDR");
        goto out;
    }

    if (netmask) {
        if (inet_aton(netmask, &(addrp->sin_addr)) == 0) {
            printf("bad value2: %s", netmask);
            goto out;
        }

        if (ioctl(s, SIOCSIFNETMASK, &ifr)) { // set net addr mask
            perror("SIOCSIFNETMASK");
            goto out;
        }

        // Must do this again so that [sub]netmask (and so default route)

        // is taken notice of.

        if (inet_aton(addr, &(addrp->sin_addr)) == 0) {
            printf("bad value3: %s", addr);
            goto out;
        }

        if (ioctl(s, SIOCSIFADDR, &ifr)) {
            perror("SIOCSIFADDR 2");
            goto out;
        }
    }

    ifr.ifr_flags = IFF_UP | IFF_BROADCAST | IFF_RUNNING;
    if (ioctl(s, SIOCSIFFLAGS, &ifr)) { // set ifnet flags
        perror("SIOCSIFFLAGS");
        goto out;
    }

    retcode = 0;
    existIP =1;

 out:
    if (s != -1)
      close(s);
    return retcode;
}

void interface_up(const char *intf)
{
  struct ifreq ifr;
  int s = -1;

  s = socket(AF_INET, SOCK_DGRAM, 0);
  if (s < 0) {
    perror("socket");
    return;
  }

  strcpy(ifr.ifr_name, intf);
  ifr.ifr_flags = IFF_UP | IFF_BROADCAST | IFF_RUNNING;
  if (ioctl(s, SIOCSIFFLAGS, &ifr)) {
    perror("SIOCSIFFLAGS");
  }
  close(s);
}

void interface_down(const char *intf)
{
  struct ifreq ifr;
  int s = -1;

  s = socket(AF_INET, SOCK_DGRAM, 0);
  if (s < 0) {
    perror("socket");
    return;
  }

  strcpy(ifr.ifr_name, intf);
  ifr.ifr_flags &= ~(IFF_UP | IFF_RUNNING);
  if (ioctl(s, SIOCSIFFLAGS, &ifr)) {
    perror("SIOCSIFFLAGS");
  }
  close(s);
}

void wifi_callback_cb_fn(unsigned int status)
{
    if (status == WIFI_LINK_STATUS_CONNECTED) {
        printf("wlan0: Connected with AP\n");
       // cyg_flag_setbits(&sys_flag, SYS_WIFI_LINK_UP_EVENT);
    }
     else if (status == WIFI_LINK_STATUS_CONNECTED_1) {
       printf("wlan0: Connected with AP\n");
    }
    else if (status == WIFI_LINK_STATUS_DISCONNECTED) {
        printf("wlan0: Disconnected with AP\n");
       // cyg_flag_setbits(&sys_flag, SYS_WIFI_LINK_DOWN_EVENT);
    }
    else if (status == WIFI_LINK_STATUS_AUTH_FAIL) {
       printf("wlan0: Fail to authenticate with AP\n");
    }
    else if (status == WIFI_LINK_STATUS_DISASSOCIATED) {
       printf("wlan0: Disassociated by AP\n");
    }
    else if (status == WIFI_LINK_STATUS_DEAUTHENTICATED) {
       printf("wlan0: Deauthenticated by AP\n");
    }
    else if (status == WIFI_LINK_STATUS_HANDSHAKE_TIMEOUT) {
        printf("wlan0: handshake timeout\n");
    }
    else if (status == WIFI_LINK_STATUS_SACN_ZREO_RESULT) {
        printf("wlan0: scan no result\n");
    }
    else
        printf("unknown status\n");
}

void wifi_ap_callback_cb_fn(unsigned int status, unsigned char *pMacAddr)
{
    if (status == WIFI_STA_STATUS_ASSOCIATED)
    {
        printf("wlan0: A client(%02X:%02X:%02X:%02X:%02X:%02X) is associated\n",
                    *pMacAddr, *(pMacAddr+1), *(pMacAddr+2), *(pMacAddr+3), *(pMacAddr+4), *(pMacAddr+5));
    }
    else if (status == WIFI_STA_STATUS_DISASSOCIATED)
    {
        printf("wlan0: A client(%02X:%02X:%02X:%02X:%02X:%02X) is disassociated\n",
                    *pMacAddr, *(pMacAddr+1), *(pMacAddr+2), *(pMacAddr+3), *(pMacAddr+4), *(pMacAddr+5));
    }
    else if (status == WIFI_STA_STATUS_DEAUTHENTICATED)
    {
        printf("wlan0: A client(%02X:%02X:%02X:%02X:%02X:%02X) is deauth\n",
                    *pMacAddr, *(pMacAddr+1), *(pMacAddr+2), *(pMacAddr+3), *(pMacAddr+4), *(pMacAddr+5));
    }
    else if (status == WIFI_AP_READY)
    {
        printf("wlan0: AP is ready (%02X:%02X:%02X:%02X:%02X:%02X)\n",
                    *pMacAddr, *(pMacAddr+1), *(pMacAddr+2), *(pMacAddr+3), *(pMacAddr+4), *(pMacAddr+5));
    }


}

void wifi_wps_sta_callback_cb_fn(char *ssid, char *key, char *encr, char *auth)
{
    char *v[32];
    int c;

    printf("wifi_wps_sta_callback_cb_fn\n");

    if (strcmp(encr, "AES") == 0)
    {
        printf("AES\n");
        encr = "4";
    }
    else if (strcmp(encr, "TKIP") == 0)
    {
        printf("TKIP\n");
        encr = "2";
    }

    if (strcmp(auth, "WPA2-PSK") == 0)
    {
        printf("WPA2-PSK\n");
        auth = "0x80";
    }
    else if (strcmp(auth, "WPA-PSK") == 0)
    {
        printf("WPA-PSK\n");
        auth = "0x04";
    }

    c = 0;
    v[c++] = "wl";
    v[c++] = "wsec";
    v[c++] = encr;   //4: aes, 2: tkip
    v[c] = 0;
    wlu_main(3, v);

    c = 0;
    v[c++] = "wl";
    v[c++] = "wpa_auth";
    v[c++] = auth;   //0x80: wpa2, 0x04: wpa
    v[c] = 0;
    wlu_main(3, v);

    c = 0;
    v[c++] = "wl";
    v[c++] = "sup_wpa";
    v[c++] = "1";
    v[c++] = 0;
    wlu_main(3, v);

    c = 0;
    v[c++] = "wl";
    v[c++] = "set_pmk"; //password
    v[c++] = key;
    v[c++] = 0;
    wlu_main(3, v);

    c = 0;
    v[c++] = "wl";
    v[c++] = "join";
    v[c++] = gstassid;  //SSID
    v[c++] = "imode";
    v[c++] = "bss";
    v[c++] = "amode";
    v[c++] = "wpa2psk";
    v[c++] = 0;
    wlu_main(7, v);

    init_all_network_interfaces();

}

void wifi_pm_callback_cb_fn(unsigned int status)
{
    char *v[32];
    int c;

    c = 0;
    v[c++] = "wl";
    v[c++] = "dump";
    v[c++] = "pm";
    v[c] = 0;
    wlu_main(c-1, v);

    c = 0;
    v[c++] = "wl";
    v[c++] = "pwrstats";
    v[c] = 0;
    wlu_main(c-1, v);
}

void wifi_es_password_callback_cb_fn(unsigned char *pwd)
{
	 strcpy((char *)gstapwd, (const char*)pwd);
    printf("ES get sta pwd %s\n", gstapwd);	
}

void wifi_es_ssid_callback_cb_fn(unsigned char *ssid)
{

	strcpy((char *)gstassid, (const char*)ssid);
   printf("ES get sta ssid %s\n", gstassid);
	
}

void wifi_es_sec_callback_cb_fn(unsigned int sec)
{
   printf("ES get sta sec %d\n", sec);
}


#define GPIO_BASE_ADDR         0xC0070000
#define GPIO_PGPIO_DIR_OFS     0x28
#define GPIO_PGPIO_SET_OFS     0x48

#define GPIO_PGPIO_CLR_OFS     0x68

#define GPIO_OUTPUT_PIN           4   //PWM4

void wifi_dhd_onoff(unsigned char onoff)
{
    volatile unsigned long *reg;
    unsigned long uireg;

    reg = (unsigned long *)(GPIO_BASE_ADDR + GPIO_PGPIO_DIR_OFS);
    uireg = *reg;
    uireg |= (1 << GPIO_OUTPUT_PIN);
    *reg = uireg;

    if (onoff == 1)
    {
        reg = (unsigned long *)(GPIO_BASE_ADDR + GPIO_PGPIO_SET_OFS);
        uireg = (unsigned long)(1 << GPIO_OUTPUT_PIN);
        *reg = uireg;
        cyg_thread_delay(1000);  // must delay here to let chip ready.
    }
    else
    {
        reg = (unsigned long *)(GPIO_BASE_ADDR + GPIO_PGPIO_CLR_OFS);
        uireg = (unsigned long)(1 << GPIO_OUTPUT_PIN);
        *reg = uireg;
    }

}

void wifi_init(void)
{
    brcm_set_dbg_level(0);
    wifi_dhd_onoff(1);
    //brcm_test();
    printf("wifi init\r\n");
}

#define ETH_ALEN        6               /* Octets in one ethernet addr   */

struct ethhdr {
    u8  h_dest[ETH_ALEN];   /* destination eth addr */
    u8  h_source[ETH_ALEN]; /* source ether addr    */
    u16 h_proto;        /* packet type ID field */
} __attribute__((packed));

#define BRCM_EVENT_CODE_OFFSET 28
#define BRCM_REASON_CODE_OFFSET 36 
#define ETH_P_802_1X    0x888e
#define ETH_P_LINK_CTL  0x886c

#define MAX_WAKEUP_PKT_LEN 1000

void wifi_resume(void)
{
    char *v[32];
    int c;
    char wakeup_pkt[MAX_WAKEUP_PKT_LEN];
    int wakeup_pkt_len=MAX_WAKEUP_PKT_LEN;
    int read_len;

    printf("%s: chip resumed, disable pkt filter.\n", __FUNCTION__);
    c = 0;
    v[c++] = "wl";
    v[c++] = "pkt_filter_enable";
    v[c++] = "100";
    v[c++] = "0";
    v[c] = 0;
    wlu_main(5, v);

    /* get the wakeup info */
    read_len = wifi_get_wlan0_wakeup_info( wakeup_pkt, wakeup_pkt_len);
    if (read_len < 0) {
        printf("%s: Cannot get wlan0 wakeup info, err=%d\n", __FUNCTION__, read_len);
    } else {
        if (read_len > (int)(sizeof(struct ethhdr))) {
            struct ethhdr *eth = (struct ethhdr *)wakeup_pkt;
            unsigned short protocol = ntohs(eth->h_proto);
            unsigned int *ev_code;
            unsigned int *reason; 

            switch(protocol){
                case ETH_P_802_1X:
                    printf("%s: woken by eapol pkt, protocol=0x%X\n", __FUNCTION__, protocol);
                    break;
                case ETH_P_LINK_CTL:
                    ev_code = (unsigned int*)(wakeup_pkt + BRCM_EVENT_CODE_OFFSET);
                    *ev_code = ntohl(*ev_code);
                    reason = (unsigned int*)(wakeup_pkt + BRCM_REASON_CODE_OFFSET);
                    *reason = ntohl(*reason);
                    printf("%s: woken by event:%s(%d), reason=%d, protocol=0x%X\n", __FUNCTION__, 
                        wifi_get_wlan0_event_name(*ev_code), *ev_code, *reason, protocol);
                    break;
                default:
                    printf("%s: woken by data pkt, protocol=0x%X\n", __FUNCTION__, protocol);
                    break;
            }
            printf("%s: src mac= %02x:%02x:%02x:%02x:%02x:%02x\n", __FUNCTION__,
                eth->h_source[0], eth->h_source[1], eth->h_source[2],
                eth->h_source[3], eth->h_source[4], eth->h_source[5]);
            printf("%s: dst mac= %02x:%02x:%02x:%02x:%02x:%02x\n", __FUNCTION__,
                eth->h_dest[0], eth->h_dest[1], eth->h_dest[2],
                eth->h_dest[3], eth->h_dest[4], eth->h_dest[5]);
        } else {
            printf("%s: woken by unknown data. data length=%d\n", __FUNCTION__, read_len);
        }
    }
}

void wifi_suspend(void)
{
    char *v[32];
    int c;

    /* set packet filter here, only allow unicast coming. */
    c = 0;
    v[c++] = "wl";
    v[c++] = "pkt_filter_add";
    v[c++] = "100";
    v[c++] = "0";
    v[c++] = "0";
    v[c++] = "0";
    v[c++] = "0x01";
    v[c++] = "0x00";
    v[c] = 0;
    wlu_main(9, v);

    /* enable pkt filter rule */
    c = 0;
    v[c++] = "wl";
    v[c++] = "pkt_filter_enable";
    v[c++] = "100";
    v[c++] = "1";
    v[c] = 0;
    wlu_main(5, v);

    wifi_suspend_wlan0_netdev();
}

void wifi_down(void)
{
    char *v[32];
    int c;
    int argc = 3;

    c = 0;
    v[c++] = "wl";
    v[c++] = "down";
    v[c] = 0;
    wlu_main(argc, v);
#if (ECOS_DHCP)
    eCos_Dhcp_Client_Stop();
    eCos_Dhcp_Server_Stop(0);
#endif
    interface_down("wlan0");

    //wifi_stop_wlan0_netdev();
    //wifi_dhd_onoff(0);

    printf("wifi down\r\n");
}

void wifi_alive(void)
{
    char *v[32];
    int c;

    c = 0;
    v[c++] = "wl";
    v[c++] = "mkeep_alive";
    v[c++] = "1";
    v[c++] = "30000";
    v[c] = 0;
    wlu_main(5, v);

    printf("wifi keep alive\r\n");
}


#if (wps)
void wifi_ap_wpa2(void)
{
    char *v[32];
    int c;

    dhd_register_wifi_status_callback(wifi_callback_cb_fn);
    dhd_register_wifi_ap_status_callback(wifi_ap_callback_cb_fn);

    set_to_wps(gapssid, gappwd);

    c = 0;
    v[c++] = "wl";
    v[c++] = "down";
    v[c] = 0;
    wlu_main(2, v);

    c = 0;
    v[c++] = "wl";
    v[c++] = "mpc";
    v[c++] = "0";
    v[c] = 0;
    wlu_main(3, v);

    c = 0;
    v[c++] = "wl";
    v[c++] = "ap";
    v[c++] = "1";
    v[c] = 0;
    wlu_main(3, v);


    c = 0;
    v[c++] = "wl";
    v[c++] = "wsec";
    v[c++] = "0x44";   //4 for aes, 2 for tkip, 0x40 for wps
    v[c] = 0;
    wlu_main(3, v);

    c = 0;
    v[c++] = "wl";
    v[c++] = "wpa_auth";
    v[c++] = "0x80";   //0x80: wpa2, 0x04: wpa
    v[c] = 0;
    wlu_main(3, v);

    c = 0;
    v[c++] = "wl";
    v[c++] = "sup_wpa";
    v[c++] = "1";
    v[c++] = 0;
    wlu_main(3, v);

    c = 0;
    v[c++] = "wl";
    v[c++] = "set_pmk"; //password
    v[c++] = gappwd;
    v[c++] = 0;
    wlu_main(3, v);

    c = 0;
    v[c++] = "wl";
    v[c++] = "up";
    v[c] = 0;
    wlu_main(2, v);

    c = 0;
    v[c++] = "wl";
    v[c++] = "ssid";
    v[c++] = gapssid;  //SSID
    v[c++] = 0;
    wlu_main(3, v);

    interface_config("wlan0", gipaddr, "255.255.255.0");
#if (ECOS_DHCP)
    eCos_Dhcp_Set_Interface("wlan0");
    eCos_Dhcp_Server_Start("DhcpSrv01");
#endif
    printf("wifi ap wpa2, %s %s\r\n", gapssid, gappwd);
}
#endif

#if 0
#define WL_CHANSPEC_BW_MASK     0x3800
#define WL_CHANSPEC_BW_SHIFT        11
#define WL_CHANSPEC_BW_5        0x0000
#define WL_CHANSPEC_BW_10       0x0800
#define WL_CHANSPEC_BW_20       0x1000
#define WL_CHANSPEC_BW_40       0x1800
#define WL_CHANSPEC_BW_80       0x2000
#define WL_CHANSPEC_BW_160      0x2800
#define WL_CHANSPEC_BW_8080     0x3000
#define WL_CHANSPEC_BAND_2G     0x0000
#define WL_CHANSPEC_BAND_3G     0x4000
#define WL_CHANSPEC_BAND_4G     0x8000
#define WL_CHANSPEC_BAND_5G     0xc000
#define CH_MAX_2G_CHANNEL       14
typedef uint16 chanspec_t;
#define CH20MHZ_CHSPEC(channel)     (chanspec_t)((chanspec_t)(channel) | WL_CHANSPEC_BW_20 | \
                    (((channel) <= CH_MAX_2G_CHANNEL) ? \
                    WL_CHANSPEC_BAND_2G : WL_CHANSPEC_BAND_5G))
/* uint32 list */
typedef struct wl_uint32_list {
    /* in - # of elements, out - # of entries */
    uint32 count;
    /* variable length uint32 list */
    uint32 element[1];
} wl_uint32_list_t;

void prepare_channel()
{
    int chosen = 0;
    char req_buf[64] = {0};
    wl_uint32_list_t *request = (wl_uint32_list_t *)req_buf;
    int start_channel=5;
    int end_channel =7;

        request->count = end_channel - start_channel + 1;
        //DHD_ERROR(("request channel: %d to %d ,request->count =%d\n",
            //start_channel, end_channel, request->count));
        for (i = 0; i < request->count; i++) {
            request->element[i] = CH20MHZ_CHSPEC((start_channel + i));
            /* request->element[i] = (start_channel + i); */
            printf("request.element[%d]=0x%x\n", i, request->element[i]);
        }

}
#endif

int set_ap_start_cmd(void){

	char *v[32];
	int c;
	int ret=0;

	printf("%s \n", __func__);


	if (strcmp(gauth, "none") == 0)
	{
	
	}
	else if ((strcmp(gauth, "wep64-auto") == 0) || (strcmp(gauth, "wep128-auto") == 0))
	{
		c = 0;
		v[c++] = "wl";
		v[c++] = "wsec";
                v[c++] = "1";       //4: aes, 2: tkip, 1: wep
		v[c] = 0;
		wlu_main(3, v);
	
		c = 0;
		v[c++] = "wl";
		v[c++] = "addwep";
		//v[c++] = "0";//can be set 0,1,2,3 for each index
                v[c++] = wep_key_idx;  
		//v[c++] = "1234567890";
                v[c++] = wep_key;
		v[c++] = 0;
		wlu_main(4, v);
	
	}
	else if (strcmp(gauth, "wpa-psk-tkip") == 0)
	{
		c = 0;
		v[c++] = "wl";
		v[c++] = "wsec";
		v[c++] = "2";	//4: aes, 2: tkip
		v[c] = 0;
		wlu_main(3, v);
	
		c = 0;
		v[c++] = "wl";
		v[c++] = "wpa_auth";
		v[c++] = "0x04";   //0x80: wpa2, 0x04: wpa
		v[c] = 0;
		wlu_main(3, v);
	
		c = 0;
		v[c++] = "wl";
		v[c++] = "sup_wpa";
		v[c++] = "1";
		v[c++] = 0;
		wlu_main(3, v);
	
		c = 0;
		v[c++] = "wl";
		v[c++] = "set_pmk"; //password
		v[c++] = gpsk;
		v[c++] = 0;
		//strcpy(v[2], argv[2]);
		wlu_main(3, v);

	}
	else if (strcmp(gauth, "wpa2-psk-aes") == 0)
	{
		c = 0;
		v[c++] = "wl";
		v[c++] = "wsec";
		v[c++] = "0x44";   //4: aes, 2: tkip, 0x40:wps
		v[c] = 0;
		wlu_main(3, v);
	
		c = 0;
		v[c++] = "wl";
		v[c++] = "wpa_auth";
		v[c++] = "0x80";   //0x80: wpa2, 0x04: wpa
		v[c] = 0;
		wlu_main(3, v);
	
		c = 0;
		v[c++] = "wl";
		v[c++] = "sup_wpa";
		v[c++] = "1";
		v[c++] = 0;
		wlu_main(3, v);
	
		c = 0;
		v[c++] = "wl";
		v[c++] = "set_pmk"; //password
		v[c++] = gpsk;
		v[c++] = 0;
		//strcpy(v[2], argv[2]);
		wlu_main(3, v);

	}
	else 
	{
        printf("Invlaid auth type \n");
		ret = 1;
		return  ret;
	}

	c = 0;
	v[c++] = "wl";
	v[c++] = "up";
	v[c] = 0;
	wlu_main(2, v);
	
	c = 0;
	v[c++] = "wl";
	v[c++] = "ssid";
	v[c++] = gssid;  //SSID
	v[c++] = 0;
	wlu_main(3, v);
	
	if(gchan[0]==0)			 // auto channel select
	{
		c = 0;
		v[c++] = "wl";
		v[c++] = "autochannel";
		v[c++] = "1";
		v[c] = 0;
		wlu_main(3, v);
					
		cyg_thread_delay(1500);
	
		c = 0;
		v[c++] = "wl";
		v[c++] = "autochannel";
		v[c] = 0;
		wlu_main(2, v);
	
		c = 0;
		v[c++] = "wl";
		v[c++] = "autochannel";
		v[c++] = "2";
		v[c] = 0;
		wlu_main(3, v);
	}
	else 
	{
		c = 0;
		v[c++] = "wl";
		v[c++] = "channel";
		v[c++] = gchan;
		v[c] = 0;
		wlu_main(3, v);    
	}
	
	c = 0;
	v[c++] = "wl";
	v[c++] = "down";
	v[c] = 0;
	wlu_main(2, v);
	
	c = 0;
	v[c++] = "wl";
	v[c++] = "up";
	v[c] = 0;
	wlu_main(2, v);

        interface_config("wlan0", gipaddr, "255.255.255.0");
#if (ECOS_DHCP)
        eCos_Dhcp_Set_Interface("wlan0");
        eCos_Dhcp_Server_Start("DhcpSrv01");
#endif

    return ret;

}

int set_sta_start_cmd(void){

	char *v[32];
	int c;
	int ret =0;

	if (strcmp(gauth, "none") == 0)
	{
		c = 0;
		v[c++] = "wl";
		v[c++] = "join";
		v[c++] = gssid;	//SSID
		v[c++] = 0;
		wlu_main(3, v);
	
		//init_all_network_interfaces();
	}
	else if( (strcmp(gauth, "wep64-auto") == 0) || (strcmp(gauth, "wep128-auto") == 0))
	{
		c = 0;
		v[c++] = "wl";
		v[c++] = "addwep";
		v[c++] = wep_key_idx;     
		v[c++] = wep_key;
		v[c] = 0;
		wlu_main(4, v);
	
		c = 0;
		v[c++] = "wl";
		v[c++] = "primary_key";
		//v[c++] = "0";
		v[c++] = wep_key_idx; 
		v[c++] = 0;
		wlu_main(3, v);
	
        c = 0;
        v[c++] = "wl";
        v[c++] = "wsec";
        v[c++] = "1";   //4: aes, 2: tkip, 1: wep
        v[c] = 0;
        wlu_main(3, v);

		c = 0;
		v[c++] = "wl";
		v[c++] = "auth"; //password
		v[c++] = "1";
		v[c++] = 0;
		wlu_main(3, v);
	
		c = 0;
		v[c++] = "wl";
		v[c++] = "join";
		v[c++] = gssid;	//SSID
		v[c++] = "imode";
		v[c++] = "bss";
		v[c++] = 0;
		wlu_main(5, v);
	
	}
	else if (strcmp(gauth, "wpa-psk-tkip") == 0)
	{
		c = 0;
		v[c++] = "wl";
		v[c++] = "wsec";
		v[c++] = "2";	//4: aes, 2: tkip
		v[c] = 0;
		wlu_main(3, v);
	
		c = 0;
		v[c++] = "wl";
		v[c++] = "wpa_auth";
		v[c++] = "0x04";   //0x80: wpa2, 0x04: wpa
		v[c] = 0;
		wlu_main(3, v);
	
		c = 0;
		v[c++] = "wl";
		v[c++] = "sup_wpa";
		v[c++] = "1";
		v[c++] = 0;
		wlu_main(3, v);
	
		c = 0;
		v[c++] = "wl";
		v[c++] = "set_pmk"; //password
		v[c++] = gpsk;
		v[c++] = 0;
		//strcpy(v[2], argv[2]);
		wlu_main(3, v);
	
		c = 0;
		v[c++] = "wl";
		v[c++] = "join";
		v[c++] = gssid;	//SSID
		v[c++] = "imode";
		v[c++] = "bss";
		v[c++] = "amode";
		v[c++] = "wpapsk";
		v[c++] = 0;
		wlu_main(7, v);
	
	}
	else if (strcmp(gauth, "wpa2-psk-aes") == 0)
	{
		c = 0;
		v[c++] = "wl";
		v[c++] = "wsec";
		v[c++] = "4";	//4: aes, 2: tkip
		v[c] = 0;
		wlu_main(3, v);
	
		c = 0;
		v[c++] = "wl";
		v[c++] = "wpa_auth";
		v[c++] = "0x80";   //0x80: wpa2, 0x04: wpa
		v[c] = 0;
		wlu_main(3, v);
	
		c = 0;
		v[c++] = "wl";
		v[c++] = "sup_wpa";
		v[c++] = "1";
		v[c++] = 0;
		wlu_main(3, v);
	
		c = 0;
		v[c++] = "wl";
		v[c++] = "set_pmk"; //password
		v[c++] = gpsk;
		v[c++] = 0;
		//strcpy(v[2], argv[2]);
		wlu_main(3, v);
	
		c = 0;
		v[c++] = "wl";
		v[c++] = "join";
		v[c++] = gssid;	//SSID
		v[c++] = "imode";
		v[c++] = "bss";
		v[c++] = "amode";
		v[c++] = "wpa2psk";
		v[c++] = 0;
		wlu_main(7, v);
	
	
	}
	else
	{
		printf("Invlaid auth type \n");
		ret = 1;
		return	ret;
	}
	return ret;


}


void set_auto_channel(void)
{
    char *v[32];
    int c;

    c = 0;
    v[c++] = "wl";
    v[c++] = "down";
    v[c] = 0;
    wlu_main(2, v);

    c = 0;
    v[c++] = "wl";
    v[c++] = "mpc";
    v[c++] = "0";
    v[c] = 0;
    wlu_main(3, v);

    c = 0;
    v[c++] = "wl";
    v[c++] = "infra";
    v[c++] = "1";
    v[c] = 0;
    wlu_main(3, v);

    c = 0;
    v[c++] = "wl";
    v[c++] = "up";
    v[c] = 0;
    wlu_main(2, v);

    c = 0;
    v[c++] = "wl";
    v[c++] = "ssid";
    v[c++] = "";
    v[c] = 0;
    wlu_main(2, v);

    c = 0;
    v[c++] = "wl";
    v[c++] = "autochannel";
    v[c++] = "1";
    v[c] = 0;
    wlu_main(3, v);

    cyg_thread_delay(1500);//Need time to scan all channels,it might be longer

    c = 0;
    v[c++] = "wl";
    v[c++] = "autochannel";
    v[c] = 0;
    wlu_main(2, v);

    c = 0;
    v[c++] = "wl";
    v[c++] = "autochannel";
    v[c++] = "2";
    v[c] = 0;
    wlu_main(3, v);

    c = 0;
    v[c++] = "wl";
    v[c++] = "channel";
    v[c] = 0;
    wlu_main(2, v);

}

void es_prep(void)
{
		char *v[32];
		int c;
		
		c = 0;
		v[c++] = "wl";
		v[c++] = "down";
		v[c] = 0;
		wlu_main(2, v);
		
		c = 0;
		v[c++] = "wl";
		v[c++] = "ap";
		v[c++] = "0";
		v[c] = 0;
		wlu_main(3, v);
		
		c = 0;
		v[c++] = "wl";
		v[c++] = "auth";
		v[c++] = "0";
		v[c] = 0;
		wlu_main(3, v);
		
		c = 0;
		v[c++] = "wl";
		v[c++] = "wsec";
		v[c++] = "0";
		v[c] = 0;
		wlu_main(3, v);
		
		c = 0;
		v[c++] = "wl";
		v[c++] = "wpa_auth";
		v[c++] = "0";
		v[c] = 0;
		wlu_main(3, v);
		
		c = 0;
		v[c++] = "wl";
		v[c++] = "mpc";
		v[c++] = "1";
		v[c] = 0;
		wlu_main(3, v);
		
		c = 0;
		v[c++] = "wl";
		v[c++] = "PM";
		v[c++] = "2";
		v[c] = 0;
		wlu_main(3, v);
		
		c = 0;
		v[c++] = "wl";
		v[c++] = "roam_off";
		v[c++] = "0";
		v[c] = 0;
		wlu_main(3, v);
		
		c = 0;
		v[c++] = "wl";
		v[c++] = "bcn_timeout";
		v[c++] = "8";
		v[c] = 0;
		wlu_main(3, v);
		
		c = 0;
		v[c++] = "wl";
		v[c++] = "up";
		v[c] = 0;
		wlu_main(2, v);
}
	
#if (wps)
void del_my_ie(void)
{
#if 0
    char *v[32];
    int c;

    //wps probrsp IE
    c = 0;
    v[c++] = "wl";
    v[c++] = "del_ie";
    v[c++] = "2";
    v[c++] = "135";
    v[c++] = "00:50:f2";
    v[c++] = "04104a00011010440001021041000101101200020004105300020284103b000103104700102221020304050607080900904cc512381021000842726f6164636f6d10230006536f66744150102400013010420001301054000800060050f20400011011000a736f667461705f777073100800020284103c0001011049000600372a000120";
    v[c++] = 0;
    wlu_main(6, v);

    //wps beacon IE
    c = 0;
    v[c++] = "wl";
    v[c++] = "del_ie";
    v[c++] = "1";
    v[c++] = "67";
    v[c++] = "00:50:f2";
    v[c++] = "04104a000110104400010210410001011012000200041053000202841049000600372a0001201054000800060050f20400011011000a736f667461705f777073";
    v[c++] = 0;
    wlu_main(6, v);

    //wps assocRsp IE
    c = 0;
    v[c++] = "wl";
    v[c++] = "del_ie";
    v[c++] = "4";
    v[c++] = "24";
    v[c++] = "00:50:f2";
    v[c++] = "04104a000110103b0001031049000600372a000120";
    v[c++] = 0;
    wlu_main(6, v);
#endif


}

void pre_add_wps_ie(void)
{

    char *v[32];
    int c;

    //wps beacon IE
    c = 0;
    v[c++] = "wl";
    v[c++] = "add_ie";
    v[c++] = "1";
    v[c++] = "67";
    v[c++] = "00:50:f2";
    v[c++] = "04104a000110104400010210410001011012000200041053000202841049000600372a0001201054000800060050f20400011011000a736f667461705f777073";
    v[c++] = 0;
    wlu_main(6, v);

    //wps probrsp IE
    c = 0;
    v[c++] = "wl";
    v[c++] = "add_ie";
    v[c++] = "2";
    v[c++] = "135";
    v[c++] = "00:50:f2";
    v[c++] = "04104a00011010440001021041000101101200020004105300020284103b000103104700102221020304050607080900904cc512381021000842726f6164636f6d10230006536f66744150102400013010420001301054000800060050f20400011011000a736f667461705f777073100800020284103c0001011049000600372a000120";
    v[c++] = 0;
    wlu_main(6, v);

}

void set_infra(void)
{
    char *v[32];
    int c = 0;

    v[c++] = "wl";
    v[c++] = "infra";
    v[c++] = "1";
    v[c] = 0;
    wlu_main(3, v);
}
#endif

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
	u8 *opos = (u8 *)buf;

	for (i = 0; i < len; i++) {
		a = hex2byte(ipos);
		if (a < 0)
			return -1;
		*opos++ = a;
		ipos += 2;
	}
	return 0;
}

MAINFUNC_ENTRY(wifi,argc,argv)
{
    char *v[32];
    int c;
    int ra;
    char t[3000];
    int num,i;
    simple_scan_result *list;
    simple_status *status;      

    dhd_register_wifi_status_callback(wifi_callback_cb_fn);
    dhd_register_wifi_ap_status_callback(wifi_ap_callback_cb_fn);
    dhd_register_wifi_es_password_callback(wifi_es_password_callback_cb_fn);
    dhd_register_wifi_es_ssid_callback(wifi_es_ssid_callback_cb_fn);
    dhd_register_wifi_es_sec_callback(wifi_es_sec_callback_cb_fn);
    //dhd_register_wifi_pm_status_callback(wifi_pm_callback_cb_fn);

    if (argc < 2)
        goto show_help;
#if (wps)
    if (strcmp(argv[1], "wps-sta-pin") == 0)
    {
        dhd_register_wps_sta_callback(wifi_wps_sta_callback_cb_fn);
        set_role_flag(1);
        set_pwd_flag(1);
        set_infra();
        init_wps(argc, argv);
        wps_start();
        return 0;
    }
    if (strcmp(argv[1], "wps-sta-pbc") == 0)
    {
        dhd_register_wps_sta_callback(wifi_wps_sta_callback_cb_fn);
        set_role_flag(1);
        set_pwd_flag(0);
        set_infra();
        init_wps(argc, argv);
        wps_start();
        return 0;
    }
    if (strcmp(argv[1], "wps-ap-pin") == 0)
    {
        if (argv[2] == NULL)
        {
            printf("You need to input STA PIN number\n");
            goto show_help;
        }
        wifi_ap_wpa2();
        pre_add_wps_ie();
        cyg_thread_delay(2000);
        //del_my_ie();
        set_wps_pin(argv[2]);
        set_role_flag(0);
        set_pwd_flag(1);
        init_wps(argc, argv);
        wps_start();

        interface_config("wlan0", gipaddr, "255.255.255.0");
#if (ECOS_DHCP)
        eCos_Dhcp_Set_Interface("wlan0");
        eCos_Dhcp_Server_Start("DhcpSrv01");
#endif
        return 0;
    }
    if (strcmp(argv[1], "wps-ap-pbc") == 0)
    {
        wifi_ap_wpa2();
        pre_add_wps_ie();
        cyg_thread_delay(2000);
        //del_my_ie();
        set_role_flag(0);
        set_pwd_flag(0);
        init_wps(argc, argv);
        wps_start();

        interface_config("wlan0", gipaddr, "255.255.255.0");
#if (ECOS_DHCP)
        eCos_Dhcp_Set_Interface("wlan0");
        eCos_Dhcp_Server_Start("DhcpSrv01");
#endif
        return 0;
    }
#endif
    if (argc == 2)
    {
        if (strcmp(argv[1], "init") == 0)
        {
            //20170925 nash: adjust driving strength
            cyg_uint32 reg;
            HAL_READ_UINT32(0xC0030068, reg);
            reg &= (0x00FFFFFF);
            reg |= (0x23000000);
            HAL_WRITE_UINT32(0xC0030068, reg);

            HAL_READ_UINT32(0xC003006C, reg);
            reg &= (0xFFFF0000);
            reg |= (0x00002222);
            HAL_WRITE_UINT32(0xC003006C, reg);
            
            brcm_set_dbg_level(0);
            wifi_start_wlan0_netdev();
            printf("wifi init\r\n");
            if (wifi_check_wlan0_resumed()) {
                wifi_resume();
            }
            printf("[ATD] init process finished \n");  
            return 0;
        }
        else if (strcmp(argv[1], "suspend") == 0)
        {
            wifi_suspend();
            printf("wifi suspend\r\n");
            return 0;
        }
        else if (strcmp(argv[1], "rand") == 0)
        {
            ra = cyg_arc4random();
            printf("ra = %d\r\n", ra);
            return 0;
        }
        else if (strcmp(argv[1], "get_ethaddr") == 0)
        {
            char ea[6];
            c = 0;
            v[c++] = "wl";
            v[c++] = "cur_etheraddr";
            v[c] = 0;
            wlu_main(2, v);

            get_eth_addr(ea);

            printf("eth addr = %x:%x:%x:%x:%x:%x\n",ea[0],ea[1],ea[2],ea[3],ea[4],ea[5]);
            return 0;
        }

         else if (strcmp(argv[1], "get_bssid") == 0)
        {
            char ea[6];
            c = 0;
            v[c++] = "wl";
            v[c++] = "bssid";
            v[c] = 0;
            wlu_main(2, v);
            get_ap_bssid(ea);
            printf("bssid = %x:%x:%x:%x:%x:%x\n",ea[0],ea[1],ea[2],ea[3],ea[4],ea[5]);
            return 0;
        }
        else if (strcmp(argv[1], "site_survey") == 0)
        {
            c = 0;
            v[c++] = "wl";
            v[c++] = "ap";
            v[c] = 0;
            wlu_main(2, v);

            c = 0;
            v[c++] = "wl";
            v[c++] = "scan";
            v[c] = 0;
            wlu_main(2, v);

            cyg_thread_delay(1500);   
            c = 0;
            v[c++] = "wl";
            v[c++] = "scanresults";
            v[c] = 0;
            wlu_main(2, v);

            get_simple_scan_result(t, &num);
            list = (simple_scan_result *)t;

            for (i = 0;i<num;i++){    
                char ssidbuf[129];
                wl_format_ssid(ssidbuf, (unsigned char *)list[i].ssid, strlen(list[i].ssid));
                printf("t[%d].ssid = %s, auth = %d, sec = %d, channel = %d, band = %s, SNR = %d, rssi = %d\n",
                     i,ssidbuf,list[i].auth,list[i].sec,list[i].channel,list[i].band, list[i].SNR, list[i].rssi);
            }
            return 0;
        }
        else if (strcmp(argv[1], "dhcpd") == 0)
        {
            printf("dhcpd\r\n");
            interface_config("wlan0", gipaddr, "255.255.255.0");
#if (ECOS_DHCP)
            eCos_Dhcp_Set_Interface("wlan0");
            eCos_Dhcp_Server_Start("DhcpSrv01");
#endif
            return 0;
        }
        else if (strcmp(argv[1], "dhcpc") == 0)
        {
            printf("dhcpcli\r\n");
            init_all_network_interfaces();
            return 0;
        }
        else if (strcmp(argv[1], "down") == 0)
        {
            wifi_down();
            return 0;
        }
        else if (strcmp(argv[1], "alive") == 0)
        {
            wifi_alive();
            return 0;
        }
        else if (strcmp(argv[1], "cmd0x") == 0)
        {
            sdio_host_set_config(SDIODEV_CONFIG_ID_CMD0_RST_EN, 0);
            return 0;
        }
        else if (strcmp(argv[1], "associated") == 0)
        {
            bool is_associated = wifi_check_wlan0_associated();
            printf("wlan0 is %s.\n", (is_associated)?"associated":"non-associated");
            return 0;
        }
        else if (strcmp(argv[1], "fw") == 0) {
            // dump current firmware type
            dhd_ecos_fop_get_firmware_type();
            return 0;
        }
        else if (strcmp(argv[1], "es") == 0) {
     			//es_prep();
     			//cyg_thread_delay(1000);
     			//es_main(1,30);
          return 0;
        }
        else if (strcmp(argv[1], "status") == 0) {
            char ssidbuf[129];
            c = 0;
            v[c++] = "wl";
            v[c++] = "status";
            v[c] = 0;
            wlu_main(2, v);
            get_simple_status(t);
            status = (simple_status *)t;
            wl_format_ssid(ssidbuf, (unsigned char *)status->ssid, strlen(status->ssid)); 
            //printf("ssid = %s, channel = %d, associated = %d\n",
                     //ssidbuf, status->channel, status->associated);
            printf("[ATD] status=%d,%s,%d \n",
				status->associated,ssidbuf, status->channel);
			return 0;
	}
        else if (strcmp(argv[1], "dhcpc_ip_addr") == 0) {
            char ip[4];
            int err = 0;
            err = wifi_get_dhcpc_ip_addr(ip);
            printf("ip = %d.%d.%d.%d, err = %d\n",
                        ip[0], ip[1], ip[2], ip[3], err);
            return 0;
        }
        else if (strcmp(argv[1], "bcn_li_dtim") == 0)
        {
            int i = 0;
            i = wifi_get_bcn_li_dtim();
            printf("bcn_li_dtim %d\n", i);
            return 0;
        }
        else if (strcmp(argv[1], "dtim_skip") == 0)
        {
            int i = 0;
            i = wifi_get_dtim_skip();
            printf("dtim_skip %d\n", i);
            return 0;
        }
        else
        {
            goto show_help;
        }

    }
    else if (argc == 3)
    {
        if (strcmp(argv[1], "fw") == 0) {
            if (strcmp(argv[2], "mfg") == 0) {
                // choose mfg firmware
                dhd_ecos_fop_set_firmware_type(0);
            }
            else if (strcmp(argv[2], "normal") == 0) {
                // choose normal firmware
                dhd_ecos_fop_set_firmware_type(1);
            } else {
                printf("wifi fw mfg|normal\n");
            }
            return 0;
        }
        else if (strcmp(argv[1], "ap") == 0)
        {
            wifi_set_wlan0_ap_mode(true);

            //If last time you are sta and connected with one AP, you should kick off this AP first
            //c = 0;
            //v[c++] = "wl";
            //v[c++] = "disassoc";
            //v[c] = 0;
            //wlu_main(2, v);
             c = 0;
             v[c++] = "wl";
             v[c++] = "deauthenticate";
             v[c] = 0;
             wlu_main(2, v);

            c = 0;
            v[c++] = "wl";
            v[c++] = "down";
            v[c] = 0;
            wlu_main(2, v);

            c = 0;
            v[c++] = "wl";
            v[c++] = "ap";
            v[c++] = "1";
            v[c] = 0;
            wlu_main(3, v);

            c = 0;
            v[c++] = "wl";
            v[c++] = "auth";
            v[c++] = "0";
            v[c] = 0;
            wlu_main(3, v);

            c = 0;
            v[c++] = "wl";
            v[c++] = "wsec";
            v[c++] = "0";
            v[c] = 0;
            wlu_main(3, v);

            c = 0;
            v[c++] = "wl";
            v[c++] = "wpa_auth";
            v[c++] = "0";
            v[c] = 0;
            wlu_main(3, v);

            c = 0;
            v[c++] = "wl";
            v[c++] = "sup_wpa";
            v[c++] = "0";
            v[c++] = 0;
            wlu_main(3, v);

            c = 0;
            v[c++] = "wl";
            v[c++] = "mpc";
            v[c++] = "0";
            v[c] = 0;
            wlu_main(3, v);

            /*c = 0;
            v[c++] = "wl";
            v[c++] = "dtim";
            v[c++] = "1";
            v[c] = 0;
            wlu_main(3, v);*/

            c = 0;
            v[c++] = "wl";
            v[c++] = "ampdu_rx_factor";
            v[c++] = "0";
            v[c] = 0;
            wlu_main(3, v);

            c = 0;
            v[c++] = "wl";
            v[c++] = "ampdu_ba_wsize";
            v[c++] = "8";
            v[c] = 0;
            wlu_main(3, v);

            if (strcmp(argv[2], "start") == 0)
            {
                if(set_ap_start_cmd()){
                   return -1;
                }
                return 0;

            }
            else if (strcmp(argv[2], "none") == 0)
            {

            }
            else if (strcmp(argv[2], "autoch") == 0)
            {
                set_auto_channel();
            }
            else if (strcmp(argv[2], "wep") == 0)
            {
                c = 0;
                v[c++] = "wl";
                v[c++] = "wsec";
                v[c++] = "1";
                v[c] = 0;
                wlu_main(3, v);

                c = 0;
                v[c++] = "wl";
                v[c++] = "addwep";
                v[c++] = "0";//can be set 0,1,2,3 for each index
                v[c++] = "1234567890";
                v[c++] = 0;
                wlu_main(4, v);

            }
            else if (strcmp(argv[2], "wpa-psk-tkip") == 0)
            {
                c = 0;
                v[c++] = "wl";
                v[c++] = "wsec";
                v[c++] = "2";   //4: aes, 2: tkip
                v[c] = 0;
                wlu_main(3, v);

                c = 0;
                v[c++] = "wl";
                v[c++] = "wpa_auth";
                v[c++] = "0x04";   //0x80: wpa2, 0x04: wpa
                v[c] = 0;
                wlu_main(3, v);

                c = 0;
                v[c++] = "wl";
                v[c++] = "sup_wpa";
                v[c++] = "1";
                v[c++] = 0;
                wlu_main(3, v);

                c = 0;
                v[c++] = "wl";
                v[c++] = "set_pmk"; //password
                v[c++] = gappwd;
                v[c++] = 0;
                //strcpy(v[2], argv[2]);
                wlu_main(3, v);

            }
            else if (strcmp(argv[2], "wpa2-psk-aes") == 0)
            {
                c = 0;
                v[c++] = "wl";
                v[c++] = "wsec";
                v[c++] = "0x44";   //4: aes, 2: tkip, 0x40:wps
                v[c] = 0;
                wlu_main(3, v);

                c = 0;
                v[c++] = "wl";
                v[c++] = "wpa_auth";
                v[c++] = "0x80";   //0x80: wpa2, 0x04: wpa
                v[c] = 0;
                wlu_main(3, v);

                c = 0;
                v[c++] = "wl";
                v[c++] = "sup_wpa";
                v[c++] = "1";
                v[c++] = 0;
                wlu_main(3, v);

                c = 0;
                v[c++] = "wl";
                v[c++] = "set_pmk"; //password
                v[c++] = gappwd;
                v[c++] = 0;
                //strcpy(v[2], argv[2]);
                wlu_main(3, v);

            }

            c = 0;
            v[c++] = "wl";
            v[c++] = "up";
            v[c] = 0;
            wlu_main(2, v);

            c = 0;
            v[c++] = "wl";
            v[c++] = "ssid";
            v[c++] = gapssid;  //SSID
            v[c++] = 0;
            wlu_main(3, v);

            interface_config("wlan0", gipaddr, "255.255.255.0");
#if (ECOS_DHCP)
            eCos_Dhcp_Set_Interface("wlan0");
            eCos_Dhcp_Server_Start("DhcpSrv01");
#endif
            return 0;
        }
        else if (strcmp(argv[1], "sta") == 0)
        {
            wifi_set_wlan0_ap_mode(false);

            //eCos_Dhcp_Client_Stop();
            //eCos_Dhcp_Server_Stop(0);
            c = 0;
            v[c++] = "wl";
            v[c++] = "down";
            v[c] = 0;
            wlu_main(2, v);

            c = 0;
            v[c++] = "wl";
            v[c++] = "ap";
            v[c++] = "0";
            v[c] = 0;
            wlu_main(3, v);

            c = 0;
            v[c++] = "wl";
            v[c++] = "auth";
            v[c++] = "0";
            v[c] = 0;
            wlu_main(3, v);

            c = 0;
            v[c++] = "wl";
            v[c++] = "wsec";
            v[c++] = "0";
            v[c] = 0;
            wlu_main(3, v);

            c = 0;
            v[c++] = "wl";
            v[c++] = "wpa_auth";
            v[c++] = "0";
            v[c] = 0;
            wlu_main(3, v);

            c = 0;
            v[c++] = "wl";
            v[c++] = "mpc";
            v[c++] = "1";
            v[c] = 0;
            wlu_main(3, v);

            c = 0;
            v[c++] = "wl";
            v[c++] = "PM";
            v[c++] = "2";
            v[c] = 0;
            wlu_main(3, v);

            c = 0;
            v[c++] = "wl";
            v[c++] = "roam_off";
            v[c++] = "0";
            v[c] = 0;
            wlu_main(3, v);

            c = 0;
            v[c++] = "wl";
            v[c++] = "bcn_timeout";
            v[c++] = "8";
            v[c] = 0;
            wlu_main(3, v);

            c = 0;
            v[c++] = "wl";
            v[c++] = "up";
            v[c] = 0;
            wlu_main(2, v);

            if (strcmp(argv[2], "start") == 0)
            {
                if(set_sta_start_cmd())
                {
  		     return -1;
                }
            }
            else if (strcmp(argv[2], "none") == 0)
            {
                c = 0;
                v[c++] = "wl";
                v[c++] = "join";
                v[c++] = gstassid;  //SSID
                v[c++] = 0;
                wlu_main(3, v);

                //init_all_network_interfaces();
            }
            else if (strcmp(argv[2], "wep") == 0)
            {
                c = 0;
                v[c++] = "wl";
                v[c++] = "addwep";
                v[c++] = "0";
                v[c++] = "1234567890";
                v[c] = 0;
                wlu_main(4, v);

                c = 0;
                v[c++] = "wl";
                v[c++] = "primary_key";
                v[c++] = "0";
                v[c++] = 0;
                wlu_main(3, v);

                c = 0;
                v[c++] = "wl";
                v[c++] = "wsec";
                v[c++] = "1";	//4: aes, 2: tkip, 1: wep
                v[c] = 0;
                wlu_main(3, v);

                c = 0;
                v[c++] = "wl";
                v[c++] = "auth"; //password
                v[c++] = "1";
                v[c++] = 0;
                wlu_main(3, v);

                c = 0;
                v[c++] = "wl";
                v[c++] = "join";
                v[c++] = gstassid;  //SSID
                v[c++] = "imode";
                v[c++] = "bss";
                v[c++] = 0;
                wlu_main(5, v);

            }
            else if (strcmp(argv[2], "wpa-psk-tkip") == 0)
            {
                c = 0;
                v[c++] = "wl";
                v[c++] = "wsec";
                v[c++] = "2";   //4: aes, 2: tkip
                v[c] = 0;
                wlu_main(3, v);

                c = 0;
                v[c++] = "wl";
                v[c++] = "wpa_auth";
                v[c++] = "0x04";   //0x80: wpa2, 0x04: wpa
                v[c] = 0;
                wlu_main(3, v);

                c = 0;
                v[c++] = "wl";
                v[c++] = "sup_wpa";
                v[c++] = "1";
                v[c++] = 0;
                wlu_main(3, v);

                c = 0;
                v[c++] = "wl";
                v[c++] = "set_pmk"; //password
                v[c++] = gstapwd;
                v[c++] = 0;
                //strcpy(v[2], argv[2]);
                wlu_main(3, v);

                c = 0;
                v[c++] = "wl";
                v[c++] = "join";
                v[c++] = gstassid;  //SSID
                v[c++] = "imode";
                v[c++] = "bss";
                v[c++] = "amode";
                v[c++] = "wpapsk";
                v[c++] = 0;
                wlu_main(7, v);

            }
			else if (strcmp(argv[2], "wpa-psk-aes") == 0)
            {
                c = 0;
                v[c++] = "wl";
                v[c++] = "wsec";
                v[c++] = "4";   //4: aes, 2: tkip
                v[c] = 0;
                wlu_main(3, v);

                c = 0;
                v[c++] = "wl";
                v[c++] = "wpa_auth";
                v[c++] = "0x04";   //0x80: wpa2, 0x04: wpa
                v[c] = 0;
                wlu_main(3, v);

                c = 0;
                v[c++] = "wl";
                v[c++] = "sup_wpa";
                v[c++] = "1";
                v[c++] = 0;
                wlu_main(3, v);

                c = 0;
                v[c++] = "wl";
                v[c++] = "set_pmk"; //password
                v[c++] = gstapwd;
                v[c++] = 0;
                //strcpy(v[2], argv[2]);
                wlu_main(3, v);

                c = 0;
                v[c++] = "wl";
                v[c++] = "join";
                v[c++] = gstassid;  //SSID
                v[c++] = "imode";
                v[c++] = "bss";
                v[c++] = "amode";
                v[c++] = "wpapsk";
                v[c++] = 0;
                wlu_main(7, v);

            }
            else if (strcmp(argv[2], "wpa2-psk-aes") == 0)
            {
                c = 0;
                v[c++] = "wl";
                v[c++] = "wsec";
                v[c++] = "4";   //4: aes, 2: tkip
                v[c] = 0;
                wlu_main(3, v);

                c = 0;
                v[c++] = "wl";
                v[c++] = "wpa_auth";
                v[c++] = "0x80";   //0x80: wpa2, 0x04: wpa
                v[c] = 0;
                wlu_main(3, v);

                c = 0;
                v[c++] = "wl";
                v[c++] = "sup_wpa";
                v[c++] = "1";
                v[c++] = 0;
                wlu_main(3, v);

                c = 0;
                v[c++] = "wl";
                v[c++] = "set_pmk"; //password
                v[c++] = gstapwd;
                v[c++] = 0;
                //strcpy(v[2], argv[2]);
                wlu_main(3, v);

                c = 0;
                v[c++] = "wl";
                v[c++] = "join";
                v[c++] = gstassid;  //SSID
                v[c++] = "imode";
                v[c++] = "bss";
                v[c++] = "amode";
                v[c++] = "wpa2psk";
                v[c++] = 0;
                wlu_main(7, v);


            }

            init_all_network_interfaces();
            return 0;
        }
		else if (strcmp(argv[1], "es") == 0){
             //init_es();
             //es_start();
             i = atoi(argv[2]);
             if (i <= 0)
                i = 30;
             else if (i > 300)
                i = 300;
             //es_prep();
             //cyg_thread_delay(1000);
             //es_main(1, i);
             return 0;
        }
        else if (strcmp(argv[1], "bcn_li_dtim") == 0)
        {
            int err = 0;
            unsigned int i = 0;
            if ((i = atoi(argv[2])) == 0 &&
                !strcmp(argv[2], "0")) {
                printf("bad value: %s\n", argv[2]);
            }
            err = wifi_set_bcn_li_dtim(i);
            printf("bcn_li_dtim %d, err = %d\n", i, err);
            return 0;
        }
        else if (strcmp(argv[1], "dtim_skip") == 0)
        {
            int err = 0;
            unsigned int i = 0;
            if ((i = atoi(argv[2])) == 0 &&
                !strcmp(argv[2], "0")) {
                printf("bad value: %s\n", argv[2]);
            }
            err = wifi_set_dtim_skip(i);
            printf("dtim_skip %d, err = %d\n", i, err);
            return 0;
        }	
    }
    else if (argc >= 4)
    {
        //unsigned char ch;

        if (strcmp(argv[1], "ap") == 0)
        {
            if (strcmp(argv[2], "setssid") == 0)
            {
                strcpy((char *)gapssid, argv[3]);
                printf("ap ssid %s\n", gapssid);
                return 0;
            }
            else if (strcmp(argv[2], "setpwd") == 0)
            {
                strcpy((char *)gappwd, argv[3]);
                printf("ap pwd %s\n", gappwd);
                return 0;
            }
/*            else if (strcmp(argv[2], "setch") == 0)
            {
                ch = atoi(argv[3]);
                if ((ch >=1) && (ch <= 14))
                {
                  gch = ch;
                  printf("ap channel %s\n", gch);
                }
            }
*/
        }
        else if (strcmp(argv[1], "sta") == 0)
        {
            if (strcmp(argv[2], "setssid") == 0)
            {
                strcpy((char *)gstassid, argv[3]);
                printf("sta ssid %s\n", gstassid);
                return 0;
            }
            else if (strcmp(argv[2], "setpwd") == 0)
            {
                strcpy((char *)gstapwd, argv[3]);
                printf("sta pwd %s\n", gstapwd);
                return 0;
            }
        }
        else if (strcmp(argv[1], "dhcpc") == 0)
        {
            int err = 0;
            dhcpc_param param;
            if (argc == 5) {
                printf("argc %d \"0 %s 1 %s 2 %s 3 %s 4 %s\"\n", argc, argv[0], argv[1], argv[2], argv[3], argv[4]);
                if (inet_aton(argv[2], (struct in_addr *)&(param.own_ip)) == 0) {
                    printf("bad value: %s", argv[2]);
                }
                if (inet_aton(argv[3], (struct in_addr *)&(param.server_ip)) == 0) {
                    printf("bad value: %s", argv[3]);
                }
                if ((param.lease_time = atoi(argv[4])) == 0) {
                    printf("bad value: %s", argv[4]);
                }

                err = wifi_set_dhcpc_param(&param);
                printf("own ip = 0x%08x, server ip = 0x%08x, lease time = %d, err = %d\n",
                    param.own_ip, param.server_ip, param.lease_time, err);
            }
            return 0;
        }
        if (strcmp(argv[1], "set") == 0)
        {
            if(strcmp(argv[2],"ssid")==0 )
            {
                 strcpy((char *)gssid, argv[3]);
                 printf("[ATD] ssid=%s\n", gssid);
                 return 0;
            }
            else if(strcmp(argv[2],"auth")==0)
            {
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
            else if(strcmp(argv[2],"psk")==0)
            {
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
            else if(strcmp(argv[2],"wepkey")==0)
            {
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
            else if(strcmp(argv[2],"wepkey_idx")==0)
            {
                 int wep_idx;

                 wep_idx = atoi(argv[3]);  
                 if(wep_idx>3 || wep_idx<0 ){ 
                     printf("Invalid wep key index (expected: 0~3) \n");
                     return -1;
                 }

                 strcpy((char *)wep_key_idx, argv[3]);
                 printf("[ATD] wepkey_idx=%s\n", wep_key_idx);
                 return 0;
            }
            else if(strcmp(argv[2],"chan")==0)
            {
                 int ch;
                 int supported_ch=0;
                 ch = atoi(argv[3]);

                 if(ch<14) 
                 {
                      for(i=0; i<chan_2G_index; i++)
                      {
                          if(ch==chan_2G_list[i]){
                              strcpy((char *)gchan, argv[3]);
                              supported_ch=1;
                              break;
                          }
                      }  
                }
                else 
                {
                      for(i=0; i<chan_5G_index; i++)
                      {
                          if(ch==chan_5G_list[i]){
                              strcpy((char *)gchan, argv[3]);
	                          supported_ch=1;
		                      break;
                          }
                      }	
                }
				 
                if(supported_ch)
                {
                      printf("[ATD] channel=%s\n", gchan); 
                }
                else
                {
                      printf("[ATD] invalid channel: %d\n", ch);
		              printf("please enter below channel: \n");
                      printf("2G: ");
                      for(i=0; i<chan_2G_index; i++)
                      {
                           printf("%d ", chan_2G_list[i]);    
                      }
                      printf("\n");
                      printf("5G: ");
                      for(i=0; i<chan_5G_index; i++)
                      {
                           printf("%d ", chan_5G_list[i]);    
                      }
                      printf("\n");
                }
                return 0;
            }  
            else 
                printf("Please set parameter: ssid / auth / psk / wepkey / wepkey_idx / chan \n");
        }
		
    }
show_help:
    printf("wifi fw mfg\n");
    printf("wifi fw normal\n");

    printf("wifi init\n");

    printf("wifi ap autoch\n");
    printf("wifi ap none\n");
    printf("wifi ap wep\n");
    printf("wifi ap wpa-psk-tkip\n");
    printf("wifi ap wpa2-psk-aes\n");
    printf("wifi ap setssid x\n");
    printf("wifi ap setpwd x\n");
    //printf("wifi ap setmod x\n");
    //printf("wifi ap setch x\n");
    //printf("wifi ap setht x\n");

    printf("wifi sta none\n");
    printf("wifi sta wep\n");
    printf("wifi sta wpa-psk-tkip\n");
    printf("wifi sta wpa2-psk-aes\n");
    printf("wifi sta setssid x\n");
    printf("wifi sta setpwd x\n");

    printf("wifi site_survey\n");
    printf("wifi get_ethaddr\n");
    printf("wifi get_bssid\n");  
    printf("wifi rand\n");
    printf("wifi dhcpd\n");
    printf("wifi dhcpc\n");
    printf("wifi suspend\n");
    printf("wifi down\n");
    printf("wifi alive\n");
    printf("wifi cmd0x\n");
    printf("wifi associated\n");

    printf("wifi wps-sta-pin\n");
    printf("wifi wps-sta-pbc\n");
    printf("wifi wps-ap-pin\n");
    printf("wifi wps-ap-pbc\n");
	printf("wifi es\n");
    printf("wifi status\n");

    return 0;
}

MAINFUNC_ENTRY(wl, argc, argv)
{
    //int ret;
    //int s;

    if (argc < 2)
    return 0;

    wlu_main(argc, argv);
    return 0;
}

MAINFUNC_ENTRY(aaa,argc,argv)
{
    brcm_set_dbg_level(0);
    init_all_network_interfaces();
    return 0;
}

MAINFUNC_ENTRY(brcmdbg,argc,argv)
{
    dhd_ecos_dbg(argc, argv);
    return 0;
}

MAINFUNC_ENTRY(brcmcons,argc,argv)
{
    char *endptr = NULL;
    int interval = 0;

    if (argc>1) {
        argv++;
        interval = strtol(*argv, &endptr, 0);
    }
    diag_printf("interval=%x\n", interval);
    brcm_set_console_interval((unsigned int)interval);
    return 0;
}

MAINFUNC_ENTRY(brcmhwoob,argc,argv)
{
    char *endptr = NULL;
    int val = 0;

    if (argc>1) {
        argv++;
        val = strtol(*argv, &endptr, 0);
    }
    diag_printf("val=%x\n", val);

    brcm_set_hwoob_enable(!!val);

    return 0;
}

MAINFUNC_ENTRY(ifconfig,argc,argv)
{
    if(argc==3){
       if(strcmp(argv[1], "wlan0") == 0)
       {
            if(strcmp(argv[2], "up") == 0)
                 interface_up("wlan0");
			
	    else if(strcmp(argv[2], "down") == 0)
	         interface_down("wlan0");

	    else
	    {
                strcpy((char *)gipaddr, argv[2]);
                interface_config("wlan0", gipaddr , "255.255.255.0");   
                //eCos_Dhcp_Set_Interface("wlan0");
                //eCos_Dhcp_Server_Start("DhcpSrv01");			
	     }
       }
       else{
             printf("invalid wlan interface ! \n");
       }
    }
    if (argc < 3) {
       printf("wlan0 ip addr: %s\n", gipaddr);
    }

    return 0;
}


#if 0
MAINFUNC_ENTRY(ap, argc, argv)
{
    char *v[32];
    int c;

    if (argc < 2)
    return 0;

    printf("argv[0] = %s\n",argv[0]);
    printf("argv[1] = %s\n",argv[1]);
    printf("argv[2] = %s\n",argv[2]);

        c = 0;
        v[c++] = "wl";
        v[c++] = "down";
        v[c] = 0;
        wlu_main(2, v);

        c = 0;
        v[c++] = "wl";
        v[c++] = "mpc";
        v[c++] = "0";
        v[c] = 0;
        wlu_main(3, v);

        c = 0;
        v[c++] = "wl";
        v[c++] = "ap";
        v[c++] = "1";
        v[c] = 0;
        wlu_main(3, v);

#if 0
        c = 0;
        v[c++] = "wl";
        v[c++] = "wsec";
        v[c++] = "4";
        v[c] = 0;
        wlu_main(3, v);

        c = 0;
        v[c++] = "wl";
        v[c++] = "wpa_auth";
        v[c++] = "0x80";
        v[c] = 0;
        wlu_main(3, v);
#endif

        c = 0;
        v[c++] = "wl";
        v[c++] = "up";
        v[c++] = 0;
        wlu_main(2, v);

        c = 0;
        v[c++] = "wl";
        v[c++] = "ssid";
        v[c++] = "brcm_ap";
        v[c++] = 0;
        //strcpy(v[2], argv[1]);
        wlu_main(3, v);
#if 0
        c = 0;
        v[c++] = "wl";
        v[c++] = "sup_wpa";
        v[c++] = "1";
        v[c++] = 0;
        wlu_main(3, v);

        c = 0;
        v[c++] = "wl";
        v[c++] = "set_pmk";
        v[c++] = "12345678";
        v[c++] = 0;
        //strcpy(v[2], argv[2]);
        wlu_main(3, v);
#endif

    return 0;
}

MAINFUNC_ENTRY(dhcpd,argc,argv)
{
    interface_config("wlan0", "192.168.1.1", "255.255.255.0");
    eCos_Dhcp_Set_Interface("wlan0");
    eCos_Dhcp_Server_Start("DhcpSrv01");

    return 0;
}
#endif

#endif //#if (defined (__ECOS))