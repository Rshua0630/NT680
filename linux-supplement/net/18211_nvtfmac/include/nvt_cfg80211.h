#ifndef _NVT_CFG80211_H_
#define _NVT_CFG80211_H_

#include <linux/types.h>

#include "nvt_wlan_linux.h"

/* define the vendor command macro when kernel version > 3.14 */
#if LINUX_VERSION_CODE >= KERNEL_VERSION(3, 14, 0)
#define VENDOR_CMD_SUPPORT 1
#endif

#define ETH_P_WAPI 0x88b4

#define WLAN_EID_SSID 0

#define SCAN_BUF_SIZE   0x10000

/*AT add for the Recover buffer len*/
#define RECOVER_BUF_LEN 12

#define MAX_KEY_INDEX   4

#define IEEE80211_NUM_ACS 4

#define MAX_SCAN_COUNT  128

#define IE_MAX_LEN      512

#define MAX_PROBED_SSIDS        5

#define WAPI_REPLAY_CTR_LEN     16

#define DOT_80211_BEACON_HEADER 24
#define DOT_80211_WIRELESS_MGMT_FIX_HEADER 12

#define NVT_MAX_TRIPLET_802_11D 83
#define IE_HDR_LEN              2

/* AT: Add Novatek vendor ID */
#define OUI_NVT 0x38ec11
/* AT: define the vendor subcmd id for ptk offload */
#define NVT_PTK_OFFLOAD 0
/* AT: define BIT7 for the PTK offload capability */
#define WLAN_EXT_PTK_OFFLOAD_CAP (1<<7)

/* mgmt header(24 bytes) */
#define IEEE80211_MGMT_HDR_LEN \
        offsetof(struct ieee80211_mgmt, u.beacon.timestamp)
/* mgmt header(24 bytes) + 12 bytes */
#define IEEE80211_BCN_PRB_FIXED_LEN \
        offsetof(struct ieee80211_mgmt, u.beacon.variable)

/* char WID */
#define WID_BSS_TYPE                            0x0000
#define WID_CURRENT_TX_RATE                     0x0001
#define WID_PRIMARY_CHANNEL                     0x0002
#define WID_PREAMBLE                            0x0003
#define WID_11G_OPERATING_MODE                  0x0004
#define WID_STATUS                              0x0005
#define WID_SCAN_TYPE                           0x0007
#define WID_PRIVACY_INVOKED                     0x0008
#define WID_KEY_ID                              0x0009
#define WID_QOS_ENABLE                          0x000a
#define WID_POWER_MANAGEMENT                    0x000b
#define WID_11I_MODE                            0x000C
#define WID_AUTH_TYPE                           0x000D
#define WID_SITE_SURVEY                         0x000e
#define WID_DTIM_PERIOD                         0x0010
#define WID_START_OP                            0x0012
#define WID_BCAST_SSID                          0x0015
#define WID_DISCONNECT                          0x0016
#define WID_TX_POWER_LEVEL_11A                  0x0018
#define WID_SHORT_SLOT_ALLOWED                  0x001a
#define WID_TX_POWER_LEVEL_11B                  0x001d
#define WID_START_SCAN_REQ                      0x001e
#define WID_JOIN_REQ                            0x0020
#define WID_GET_CURRENT_MAC_STATUS              0x0031
#define WID_SCAN_FILTER                         0x0036
#define WID_ENABLE_INT_SUPP                     0x0047
#define WID_11N_ENABLE                          0x0082
#define WID_11N_SHORT_GI_ENABLE                 0x008d
#define WID_11N_IMMEDIATE_BA_ENABLED            0X00af
#define WID_VSIE_FRAME                          0x00b4
#define WID_2040_COEXISTENCE                    0x00c1
#define WID_2040_ENABLE                         0x00c5
#define WID_11N_CURRENT_TX_BW                   0x00C8
#define WID_ENABLE_STBC                         0x00CD
#define WID_SLEEP_NOW                           0x00CB
#define WID_ENABLE_GREENFIELD                   0x00CE
#define WID_P2P_ENABLE                          0x0201
#define WID_P2P_LISTEN_MODE                     0x020A
#define WID_FC_ENALBE                           0x0401
#define WID_WPS_REGISTRATION_START              0x0403
#define WID_AP_ENABLE                           0x0402
#define WID_SCAN_ABORT                          0x0404
#define WID_11N_CURRENT_TX_MCS                  0x0088
#define WID_RSSI                                0x001F
#define WID_FC_ENABLE                           0x0401
#define WID_RATE_0                              0x020E
#define WID_TP_MONITOR                          0x020F
#define WID_WOW_CONFIG                          0x0407
#define WID_WOW_ENABLE                          0x0408
#define WID_WOW_HOST_SUSPEND                    0x0409
#define WID_WOW_HOST_RESUME                     0x040A
#define WID_EXT_MODE_11I                        0x04FF
#define WID_PMF_ENABLE                          0x0410
#define WID_FC_BORROW                           0x0411
#define WID_MONITOR_ENABLE                      0x0412
#define WID_ACS_RXRDY_THRESH                    0x0413
#define WID_IGNORE_BCN_BITMAP                   0x0414
#define WID_DATA_MODE                           0x0418

/* short WID */
#define WID_RTS_THRESHOLD                       0x1000
#define WID_FRAG_THRESHOLD                      0x1001
#define WID_SHORT_RETRY_LIMIT                   0x1002
#define WID_LONG_RETRY_LIMIT                    0x1003
#define WID_BEACON_INTERVAL                     0x1006
#define WID_ACTIVE_SCAN_TIME                    0x100c
#define WID_PASSIVE_SCAN_TIME                   0x100d
#define WID_SITE_SURVEY_SCAN_TIME               0x100E
#define WID_USER_PREF_CHANNEL                   0x1020
#define WID_CURR_OPER_CHANNEL                   0x1021
#define WID_P2P_LISTEN_EXTEND                   0x1305
#define WID_AP_RESUME_STAY_CH_TIME              0x1306
#define WID_AP_SCAN_STAY_CH_TIME                0x1307
#define WID_11N_SIG_QUAL_VAL                    0x1085

/* string WID */
#define WID_SSID                                0x3000
#define WID_MAC_ADDR                            0x300C
#define WID_SITE_SURVEY_RESULTS                 0x3012
#define WID_ADD_PTK                             0x301B
#define WID_ADD_RX_GTK                          0x301C
#define WID_ADD_TX_GTK                          0x301D
#define WID_ADD_WEP_KEY                         0x3019
#define WID_REMOVE_KEY                          0x301E
#define WID_REMOVE_WEP_KEY                      0x301A
#define WID_ASSOC_REQ_INFO                      0x301f
#define WID_ASSOC_RES_INFO                      0x3020
#define WID_11I_PSK_VALUE                       0x302a
#define WID_11N_P_ACTION_REQ                    0x3080
#define WID_PMKID_INFO                          0x3082
#define WID_FIRMWARE_INFO                       0x3083
#define WID_MIC_FAILURE_INFO                    0x3400
#define WID_EXT_DISCONNECT                      0x3401
#define WID_ADD_RX_IGTK                         0x3402
#define WID_ADD_TX_IGTK                         0x3403
#define WID_ADD_WAPI_PTK                        0x3404
#define WID_ADD_WAPI_GTK                        0x3405
#define WID_WPA_REKEY_OFFLOAD                   0x3406
#define WID_WAPI_REKEY_OFFLOAD                  0x3409
#define WID_AR_PARAMETER                        0x3408

/*int WID*/
#define WID_RECEIVED_FRAGMENT_COUNT             0x2005
#define WID_TX_FRAGMENT_COUNT                   0x200B
#define WID_SUCCESS_FRAME_COUNT                 0x2008
//20150821 nash:
/* control firmware UART mode */
#define WID_UART_MODE                           0x2026
/* control firmware debug level */
#define WID_DBG_LEVEL                           0x2027
#if 1   /* CONFIG_WIFI_TUNING_PHASE_II == 1 */
#define WID_TP_REPORT_INTERVAL                  0x2028
#define WID_TP_MIN_INTERVAL                     0x2029
#endif
#define WID_ACS_MASK                            0x2089
#define WID_SET_DTIM_SCALAR                     0x208a
#define WID_SET_DTIM_TIME                       0x208b

/* binary WID */
#define WID_UAPSD_CONFIG                        0x4001
#define WID_WMM_AP_AC_PARAMS                    0x4003
#define WID_WMM_STA_AC_PARAMS                   0x4004
#define WID_STA_JOIN_INFO                       0x4008
#define WID_CONNECTED_STA_LIST                  0x4009
#define WID_VSIE_TX_DATA                        0x4085
#define WID_VSIE_TX_PROBERSP                    0x4087
#define WID_VSIE_TX_ASSOCRSP                    0x4088
#define WID_SCAN_MULTISSID                      0x4089
#define WID_SCAN_MULTICH                        0x408A
#define WID_JOIN_REQ_BSSTBL                     0x408B
#define WID_VSIE_TX_PROBEREQ                    0x4400
#define WID_VSIE_TX_ASSOCREQ                    0x4401
#define WID_MGMT_TX                             0x4402
#define WID_SET_RXMCS                           0x4404
#define WID_GET_UMAC_STATS                      0x4407
#define WID_GET_LMAC_STATS                      0x4408
#define WID_ARP_NS_OFFLOAD                      0x4409
#define WID_WOW_PATTERN                         0x440A
#define WID_SET_REGFLAGS                        0x440C
#define WID_SET_ACS_WEIGHT                      0x440D
#define WID_TX_POWER_ADAPTION                   0x440E
#define WID_AP_RECOVER                          0x440F
#define WID_DHCP_RENEW_OFFLOAD                  0x4410
#define WID_KEEP_ALIVE_FAIL_OFFLOAD             0x4411
/* WID_11I_MODE */
#define WID_11I_SECURITY_ON                     0x01
#define WID_11I_WEP40                           0x02
#define WID_11I_WEP104                          0x06
#define WID_11I_WPA                             0x08
#define WID_11I_RSN                             0x10
#define WID_11I_CCMP                            0x20
#define WID_11I_TKIP                            0x40
#define WID_11I_TSN                             0x80
/* WID_EXT_11I_MODE */
#define WID_EXT_11I_WAPI                        0x80
#define WID_EXT_11I_SMS4                        0x40

/* WID_AUTH */
#define WID_AUTH_OPEN_PSK                       0x01
#define WID_AUTH_SHARED_KEY                     0x02
#define WID_AUTH_802_1X                         0x05

/* scan filter definition */
#define SCAN_ALL                                0
#define SCAN_NO_ADHOC                           1
#define SCAN_NO_INFRA                           2
#define SCAN_SAVE_BYHIGHRSSI                    (0 << 2)
#define SCAN_SAVE_BYLOWRSSI                     (1 << 2)
#define SCAN_SAVE_WITHOUTRSSI                   (2 << 2)
#define SCAN_CURRENTCH                          (1 << 4)
#define SCAN_SSID                               (1 << 5)

/* scan type */
#define ACTIVE_SCAN                             1
#define PASSIVE_SCAN                            0

/* scan time */
#define DEFAULT_ACTIVE_TIME                     200
#define DEFAULT_AP_SCAN_TIME                    50
#define DEFAULT_PASSIVE_TIME                    300
#define DEFAULT_P2P_ACTIVE_TIME                 500

/* event reason code */
#define SUCCESS                                 0
#define INVALID                                 1
#define TIMEOUT                                 2

/* time value */
#define ADDKEY_WAIT_EAPOL_TIME                  500
#define DISCONNECT_WAIT__TIME                   5000

/* mac status */
#define MAC_CONNECTED                           1
#define MAC_DISCONNECTED                        0

/* P2P Related Parameters */
#define P2P_NEGO_DWELL_TIME                     500

/* P2P Public Action Frame/P2P Action Frame/P2P GAS Frame */
#define P2P_PUBLIC_ACTION_CATEGORY              0x04
#define P2P_ACTION_CATEGORY                     0x7f
#define P2P_PUBLIC_ACTION_FIELD                 0x09
#define P2P_OUI                                 "\x50\x6F\x9A"
#define P2P_OUI_LEN                             3
#define P2P_OUI_TYPE                            9

/* P2P Public Action Frame OUI Subtypes */
#define P2P_GO_NEGO_REQ                         0x00
#define P2P_GO_NEGO_RSP                         0x01
#define P2P_GO_NEGO_CONF                        0x02
#define P2P_INVITE_REQ                          0x03
#define P2P_INVITE_RSP                          0x04
#define P2P_DEVICE_DISC_REQ                     0x05
#define P2P_DEVICE_DISC_RSP                     0x06
#define P2P_PROV_DISC_REQ                       0x07
#define P2P_PROV_DISC_RSP                       0x08

/* P2P Action Frame OUI Subtypes */
#define P2P_NOA                                 0x00
#define P2P_PRESENCE_REQ                        0x01
#define P2P_PRESENCE_RSP                        0x02
#define P2P_GO_DISC_REQ                         0x03

/* P2P GAS Frame */
#define P2P_GAS_INITIAL_REQ                     0x0a
#define P2P_GAS_INITIAL_RSP                     0x0b
#define P2P_GAS_COMEBACK_REQ                    0x0c
#define P2P_GAS_COMEBACK_RSP                    0x0d

/* WID_11I_MODE */
#define WID_11I_MODE_Security_ON                0x01
#define WID_11I_MODE_WEP40                      0x02
#define WID_11I_MODE_WEP104                     0x06
#define WID_11I_MODE_WPA                        0x08
#define WID_11I_MODE_RSN                        0x10
#define WID_11I_MODE_CCMP                       0x20
#define WID_11I_MODE_TKIP                       0x40
#define WID_11I_MODE_TSN                        0x80

//BSS type definition
#define BSS_STA                                 0
#define IBSS_STA                                1
#define AP                                      2

//STBC enable definition
#define TX_STBC                                 1
#define RX_STBC_1                               1
#define RX_STBC_2                               2
#define RX_STBC_3                               3
#define NVT_RX_STBC1                            0x100
#define NVT_RX_STBC12                           0x200
#define NVT_RX_STBC123                          (NVT_RX_STBC1 | NVT_RX_STBC12)

//SGI enable definition
#define SGI_40                                  2
#define SGI_20                                  1
#define LGI                                     0

//channel definition
#define SCN                                     (0 << 8)
#define SCA                                     (1 << 8)
#define SCB                                     (3 << 8)
#define BAND_2G                                 (0 << 15)
#define BAND_5G                                 (1 << 15)

//preamble definition
#define PREAMBLE_SHORT                          0
#define PREAMBLE_LONG                           1
#define PREAMBLE_AUTO                           2

//11g operating mode definition
#define OP_11B_ONLY_MODE                        0
#define OP_11G_HP_MODE                          1
#define OP_11G_COMP_MODE_1                      2
#define OP_11G_COMP_MODE_2                      3

//security definition
#define PROTO_WPA1                              1
#define PROTO_WPA2                              2
#define CIPHER_WEP40                            1
#define CIPHER_WEP104                           2
#define CIPHER_TKIP                             3
#define CIPHER_CCMP                             4
#define CIPHER_SMS4                             5

//pmf parameters
#define PMF_CAPABLE                             0x01
#define PMF_REQUIRED                            0x02
#define PMF_SHA256                              0x04

extern u32 nvt_txmq;
/* IE TLV processing */
#define TLV_LEN_OFF                             1       /* length offset */
#define TLV_HDR_LEN                             2       /* header length */
#define TLV_BODY_OFF                            2       /* body offset */
#define TLV_OUI_LEN                             3       /* oui id length */

enum nvt_scan_sts {
        SCAN_PROCESS,
};

enum WID_EVENT_ID {
        WID_EVENT_FW_READY = 0x5000,
        WID_EVENT_BA_INFO = 0x5001,
        WID_EVENT_FC_CREDIT_INIT,
        WID_EVENT_FC_INFO,
        WID_EVENT_BSS_INFO,
        WID_EVENT_SCAN_DONE,
        WID_EVENT_RX_PROBEREQ_FRAME,
        WID_EVENT_RX_ACTION_FRAME,
        WID_EVENT_LISTEN_DONE,
        WID_EVENT_DEBUG_MESSAGE_INFO,
        WID_EVENT_ADDBA_REQ,
        WID_EVENT_DELBA_REQ,
        WID_EVENT_BAR,
        WID_EVENT_RESUME,
        WID_EVENT_REKEY_OFFLOAD,
        WID_EVENT_TP_MONITOR,
        WID_EVENT_SETKEY_INFO,
        WID_EVENT_ASSOC_REQ = 0x5080,
        WID_EVENT_ASSOC_RSP,
        WID_EVENT_CONNECT_INFO,
        WID_EVENT_DHCP_RENEW_FIRST_TIME,
        WID_EVENT_NUM
};

enum nvt_if_status {
        NVT_IF_ENABLED,
        NVT_IF_CONNECTING,
        NVT_IF_CONNECTED,
        NVT_IF_REDOWNLOAD
};

enum nvt_p2p_status {
        NVT_P2P_ROC_MODE,
};

enum nvt_p2p_action {
        NVT_P2P_START_ROC,
        NVT_P2P_CANCEL_ROC,
};

enum nvt_reset_req_flags {
        DONT_RESET = 0,
        DO_RESET   = 1,
        DO_START   = 2,
        DO_RESTART = 3,
        NO_REQUEST = 4,
        NUM_RESET_REQ
};


enum nvt_if_mode {
        NVT_STA_MODE = 0,
        NVT_AP_MODE = 1,
};

/* Element ID  of various Information Elements */
enum nvt_element_id {
        ICOUNTRY            = 7,   /* Country element                */
};

struct _nvt_802_11d_reg {
        u8 country_code[IEEE80211_COUNTRY_STRING_LEN];
        u8 no_of_triplet;
        struct ieee80211_country_ie_triplet
                triplet[NVT_MAX_TRIPLET_802_11D];
};

struct _nvt_bss_dscr {
        u32 bss_type;
        u8  num_failures;
        u8  channel;
        u8  dtim_period;
        s8  rssi;
        u16 beacon_period;
        u16 cap_info;
        u8  time[4];
        u8  bssid[ETH_ALEN];
        u8  sa[ETH_ALEN];
        u8  ssid[IEEE80211_MAX_SSID_LEN+1];
        u8  dot11i_info;
};

struct _nvt_bss_info {
        struct list_head list;
        u32 length;
        u8  bssid[ETH_ALEN];
        u8  ssid_len;
        u8  ssid[IEEE80211_MAX_SSID_LEN];
        u8  channel;
        u8  dot11i_info;
        s8  rssi;
        u16 beacon_period;
        u16 cap_info;
        u32 ie_offset;
        u32 ie_length;
};

struct _nvt_p2p_public_action {
        u8 category;
        u8 action;
        u8 oui[3];
        u8 oui_type;
        u8 subtype;
        u8 dialog_token;
        u8 data[1];
};

struct _nvt_p2p_action {
        u8 category;
        u8 oui[3];
        u8 oui_type;
        u8 subtype;
        u8 dialog_token;
        u8 data[1];
};

struct _nvt_p2p_gas_public_action {
        u8 category;
        u8 action;
        u8 dialog_token;
        u8 data[1];
};

struct _nvt_p2p {
        ulong status;
        u8 listen_channel;
        struct ieee80211_channel remain_on_channel;
        u32 roc_cookie;
};

struct _nvt_cfg80211 {
        struct wiphy *wiphy;
        struct _nvt_adapter *nvt_adapter;
        /* scan parameters */
        struct timer_list scan_timer;
        struct cfg80211_scan_request *scan_request;
        struct _nvt_if *scan_if;
        struct work_struct scan_timeout_work;
        struct mutex scan_lock;
        unsigned long scan_sts;
        /*add for cfg_ops get_station*/
        struct semaphore sem;
        /*add for cfg_ops set txpower*/
        u32 tx_pwr;

        u8 bss_count;
        struct list_head bss_list;
        spinlock_t bss_list_lock;
        struct _nvt_p2p nvt_p2p;

        u8 wow_enable;
        struct _nvt_802_11d_reg domain_reg;
};

struct _nvt_probe_ssid {
        u8 index;
        u8 ssid_len;
        u8 ssid[32];
};

struct _ssid_le {
    u8 SSID_len;
    u8 SSID[32];
};

struct ieee_types_wmm_ac_parameters {
        u8 aci_aifsn_bitmap;
        u8 ecw_bitmap;
        __le16 tx_op_limit;
} __packed;

struct wmm_info {
        u8 oui[4];
        u8 subtype;
        u8 version;
        u8 qos_info;
        u8 reserved;
        struct ieee_types_wmm_ac_parameters ac_params[IEEE80211_NUM_ACS];
} __packed;

struct _BSS_CONFIG_INFO {
        u16             channel;
        u8              bss_mode;
        u8              op_11g_mode;
        u8              channel_type;
        u8              bw40_enable;
        u8              preamble;
        u8              slot;
        u8              sgi;
        u8              stbc;
        u8              qos_enable;
        struct  wmm_info wmm_info;
        u8              op_11n_enable;
        u8              hidden_ssid;
        struct _ssid_le ssid_le;
        u8              auth_mode;
        u8              security_mode;
        u8              ext_security_mode;
        u8              vsie_enable;
        u8              wpa_proto;
        u8              pairwise_cipher;
        u8              group_cipher;
        u8              wps_enable;
        u8              p2p_enable;
        u8              greenfield;
        u8              pmf_enable;
};

struct ieee_types_header {
        u8 element_id;
        u8 len;
} __packed;

struct Nova_AC_XX_PARA {
        u16 TXOP;
        u8  CWmax;
        u8  CWmin;
        u8  AIFSN;
        u8  ACM;
};
struct Nova_WID_WMM_STA_AC_PARAMS {
        struct Nova_AC_XX_PARA AC_VO;
        struct Nova_AC_XX_PARA AC_VI;
        struct Nova_AC_XX_PARA AC_BE;
        struct Nova_AC_XX_PARA AC_BK;
};

struct wow_pattern {
        u32 mask;
        u16 crc16;
        u8 offset;
        u8 filter_enable;
} __packed;

struct nvt_tlv {
        u8 id;
        u8 len;
        u8 data[1];
};

struct nvt_vs_tlv {
        u8 id;
        u8 len;
        u8 oui[3];
        u8 oui_type;
};

struct dhcp_ip {
        //fixed 0x45
        u8 ip_v_and_hl;
        u8 ip_tos;
        u8 ip_len[2];
        u8 ip_id[2];
        u8 ip_off[2];
        u8 ip_ttl;
        u8 ip_p;
        u8 ip_checksum[2];
        u8 ip_src[4];
        u8 ip_dst[4];
};

struct dhcp_udp {
        //0x00 0x44
        u8 src_port[2];
        //0x00 0x43
        u8 dst_port[2];
        //0x01 0x02
        u8 length[2];
        u8 checksum[2];
};

struct dhcp_packet {
        u8 op;
        u8 htype;
        u8 hlen;
        u8 hops;
        u8 xid[4];
        u8 secs[2];
        u8 flags[2];
        u8 ciaddr[4];
        u8 yiaddr[4];
        u8 siaddr[4];
        u8 giaddr[4];
        u8 chaddr[16];
        u8 sname[64];
        u8 file[128];
        /*DHCP COOKIE*/
        //0x63 0x82 0x53 0x63
        u8 dhcp_cookie[4];
        /*DHCP REQUSET OPTION*/
        //0x35 --> dhcp request
        u8 dhcp_req_type;
        //0x01 --> dhcp req len
        u8 dhcp_req_len;
        //0x03 --> request code
        u8 request_code;
        /*DHCP Client ID*/
        u8 dhcp_ci_type;
        u8 dhcp_ci_len;
        u8 dhcp_ci_hw_type;
        u8 dhcp_ci_hw_mac[6];
        /*DHCP request IP*/
        u8 dhcp_rIP_type;
        u8 dhcp_rIP_len;
        u8 dhcp_rIP[4];
        /*DHCP SERVER IDENTIFIER OPTION*/
        //0x36 --> server identifier option
        //u8 dhcp_server_type;
        //0x04 --> dhcp server len
        //u8 dhcp_s_len;
        //server ip addr
        //u8 server_ip[4];

        //end --> 0xff
        u8 end;
};

extern int nvt_cfg80211_init(struct _nvt_adapter *);
extern int nvt_cfg80211_deinit(struct _nvt_adapter *);
extern s32 nvt_p2p_roc_done(struct _nvt_if *nvt_if,
        u16 msg_len, u8 *data, bool direction);
extern s32 nvt_scan_result(struct _nvt_if *nvt_if, u16 msg_len, u8 *data);
extern s32 nvt_connect_status_station(struct _nvt_if *nvt_if, u16 msg_len,
                                        u8 *data);
extern s32 nvt_enable_status_ap(struct _nvt_if *nvt_if, u16 msg_len,
                                        u8 *data);
extern s32 nvt_connect_status_ap(struct _nvt_if *nvt_if,
        u16 msg_len, u8 *data);
extern s32 nvt_micfail_status(struct _nvt_if *nvt_if, u16 msg_len, u8 *data);
extern s32 nvt_connect_result(struct _nvt_if *nvt_if, u16 msg_len, u8 *data);
extern s32 nvt_rx_mgmt_probereq(struct _nvt_if *nvt_if,
        u16 msg_len, u8 *data);
extern s32 nvt_rx_action_frame(struct _nvt_if *nvt_if,
        u16 msg_len, u8 *data);
extern void nvt_abort_scanning(struct _nvt_cfg80211 *cfg);
#ifdef CONFIG_PM
extern s32 nvt_setup_wowlan(struct _nvt_if *nvt_if,
        struct _nvt_cfg80211 *nvt_cfg, struct cfg80211_wowlan *wow);
extern s32 nvt_set_wowlan_on(struct cfg80211_wowlan *, struct _nvt_if *);
#endif
extern s32 nvt_resume_result(struct _nvt_if *nvt_if, u16 msg_len, u8 *data);
extern s32 nvt_set_arp_ns_offload(struct _nvt_if *nvt_if);
extern s32 nvt_rekeyoffload_info(struct _nvt_if *nvt_if,
        u16 msg_len, u8 *data);
extern s32 nvt_setkey_info(struct _nvt_if *nvt_if, u16 msg_len, u8 *data);
extern u16 dhcp_checksum(u8 *buf, u32 nbytes, u32 sum);
extern void build_dhcp_renew_pat(u8 *buf, u8 *src_addr, u8 *dst,
                u8 *mac_addr);
extern void build_ip_header(u8 *buf, u8 *checksum, u8 *src_addr,
                u8 *dst_addr);
extern void build_udp_header(u8 *buf, u8 *checksum);
extern void build_dhcp_request(u8 *buf, u8 *chaddr, u8 *ciaddr, u8 *s_addr);
extern struct nvt_tlv *nvt_parse_tlvs(void *buf, int buflen, uint key);
extern s32 nvt_configure_wpaie(struct nvt_vs_tlv *wpa_ie, bool is_rsn_ie,
        u8 *Nova_11I, u8 *Nova_AUTH);
#endif
