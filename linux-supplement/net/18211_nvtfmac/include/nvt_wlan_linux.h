#ifndef _NVT_LINUX_H_
#define _NVT_LINUX_H_

#include <linux/version.h>
#include <linux/list.h>
#include <linux/ieee80211.h>
#include <net/cfg80211.h>

#include "nvt_bus.h"
#include "nvt_fw.h"
#include "nvt_cfg80211.h"
#include "nvt_wlan_priv.h"
#include "nvt_icfg.h"

#define DESTI_MASK      0x03
#define TO_WIFI         0x01
#define RTS_INIT_VAL    2347
#define FRAG_INIT_VAL   2346
#define SHORT_RETRY_INIT_VAL 7
#define LONG_RETRY_INIT_VAL 4

enum nvt_ctrl_flags_bits {
        PROBE_REQ_EXTRA_IE_FLAG = 0x01,
        WPS_FLAG = 0x02,
        P2P_WILDCARD_SSID_FLAG = 0x04,
        P2P_FLAG = 0x08,
        BEACON_EXTRA_IE_FLAG = 0x10,
        PROBE_RESP_EXTRA_IE_FLAG = 0x20,
        ASSOC_RESP_EXTRA_IE_FLAG = 0x40,
};

struct _nvt_fw_capability {
        u8 hostcap;
        u8 phy_rf_cap;
        u8 mac_cap_1;
        u8 mac_cap_2;
        u8 platform_cap;
        u8 extra_cap;
        u8 extra_cap_1;
};

struct _nvt_adapter {
        struct list_head if_list;
        /* Lock to avoid race in if_list entries among add/del/traverse */
        spinlock_t list_lock;
        struct _nvt_bus *nvt_bus;
        /* define the firmware capability */
        struct _nvt_fw_capability nvt_fw_cap;
        struct _nvt_cfg80211 *nvt_cfg80211;
        struct _nvt_priv nvt_priv;
        u8 mac_addr[ETH_ALEN];
};

struct _nvt_wep {
        u8 key_id;
        u8 key_len;
        u8 key_data[13];
};

struct _nvt_ptk {
        u8 addr[6];
        u8 key_len;
        u8 key_tk[16];
        u8 key_rx_mic[8];
        u8 key_tx_mic[8];
};

struct _nvt_gtk {
        u8 addr[6];
        u8 key_rsc[8];
        u8 key_id;
        u8 key_len;
        u8 key_tk[16];
        u8 key_mic[16];
};

struct _nvt_wapi_ptk {
        u8 addr[6];
        u8 key_len;
        u8 key_id;
        u8 key_tk[16];
        u8 key_mic[16];
};

struct _nvt_wapi_gtk {
        u8 addr[6];
        u8 key_rsc[16];
        u8 key_id;
        u8 key_len;
        u8 key_tk[16];
        u8 key_mic[16];

};

struct _nvt_wpa_rekey_offload_data {
        u8 kek[NL80211_KEK_LEN];
        u8 kck[NL80211_KCK_LEN];
        u8 replay_ctr[NL80211_REPLAY_CTR_LEN];
};

struct _nvt_wapi_rekey_offload_data {
        u8 kek[NL80211_KEK_LEN];
        u8 kck[NL80211_KCK_LEN];
        u8 replay_ctr[WAPI_REPLAY_CTR_LEN];
        u8 seq_num[8];
};

struct _nvt_sec {
        struct _nvt_wep wep;
        struct _nvt_ptk ptk;
        struct _nvt_gtk gtk;
        struct _nvt_gtk igtk;
        struct _nvt_wapi_ptk wapi_ptk;
        struct _nvt_wapi_gtk wapi_gtk;
        struct _nvt_wpa_rekey_offload_data wpa_rekey_offload;
        struct _nvt_wapi_rekey_offload_data wapi_rekey_offload;
        u32 ciphers_pairwise;
        u8 wep_type;
        u8 mode_11i;
        /* AT add the extension 11i mode for WAPI support */
        u8 ext_mode_11i;
        u8 auth_type;
        u8 pmf_enable;
};

struct _nvt_wlan_conf {
        u8 ssid[IEEE80211_MAX_SSID_LEN];
        int ssid_len;
        u8 bssid[ETH_ALEN];
        struct _nvt_sec nvt_sec;
        u8 pref_channel;
        u8 ctrl_flag;
        u8 pwr_save;
        struct _nvt_bss_info *conn_bss;
};

struct _nvt_wlan_conf_ap {
        u8 acs_channel;
        u32 acs_mask;
        u8 acs_weight[19];
};

//Control buffer for the SKB
struct _nvt_skb_cb {
        u8 if_id;
        u8 forward_magic_pat;
};

//Store the channel info when set_channel ops
#if LINUX_VERSION_CODE < KERNEL_VERSION(3, 4, 0)
struct _nvt_ch_info {
        struct ieee80211_channel *chan;
        enum nl80211_channel_type channel_type;
};
#endif

struct _nvt_if {
        struct list_head list;
        u8 fw_if_idx;
        struct net_device *ndev;
        struct _nvt_adapter *nvt_adapter;
        /* wireless device */
        struct wireless_dev wdev;
        /* interface op mode */
        u8 mode;
        /* interface wlan sme status */
        ulong state_flags;
        /* interface wlan config */
        struct _nvt_wlan_conf nvt_wconf;
        struct _nvt_wlan_conf_ap nvt_wconf_ap;
        /* set wlan key pending flag */
        wait_queue_head_t addkey_wait;
        atomic_t eapol_cnt;
        /* set wlan disconnect pending flag */
        wait_queue_head_t disconnect_wait;
        atomic_t disconnect_cnt;
        struct mutex disconnect_lock;
        bool filter_disconnect;
        u16 mgmt_rx_reg;
        struct net_device_stats net_stats;
        /* HW headeroom used */
        u32 hdrlen;

        bool is_p2p;
        /*0=>non-ht, 1=>ht*/
        u8 ht_type;
        struct work_struct setfwdownload_work;
        s32 fw_download_type;
        /*record the current rts threshold to protect user set unused value*/
        s32 rts_val;
        /*record the current fragment threshold to protect
         * user set invalid value
         */

        s32 frag_val;
        s32 retry_short_val;
        s32 retry_long_val;

        u8 sleep_mode;
#define NVT_SLEEP_MODE_DISABLE  (0)     /* no sleep mode */
#define NVT_SLEEP_MODE_NORMAL   (1)     /* normal suspend/resume */
#define NVT_SLEEP_MODE_WOW      (2)     /* wow suspend/resume */

        u32 skip_dtim_scalar;
        u32 skip_dtim_time;
        u8 ignore_bcn_bitmap;

        /* save wow recover parameters */
        u8 recover_buf[RECOVER_BUF_LEN];

        /*1:wait enable ap event, 0:no wait ap event*/
        u8 wait_ap_start_event;
        u8 start_ap_flag;
#if LINUX_VERSION_CODE < KERNEL_VERSION(3, 4, 0)
        struct _nvt_ch_info ch_info;
#endif
};

#ifdef CONFIG_WIFI_TUNING_PHASE_II
struct _sta_sts_in_ap {
    u8 mac_addr[6];
    u8 incr_counter;
    unsigned int t_put;                          /* Unit : Byte/s */
    // Unit : real phy rate * 1000  (e.g., 5500 in this field means 5.5Mb/s)
    unsigned int phyrate;
    unsigned int phyrate_history[8];
    unsigned int outq_vo_be_history[8];          /* Driver-level */
    unsigned int drop_be_history[8];             /* Driver-level */
    unsigned int mac_votxsz_history[8];          /* from MAC firmware */
    unsigned int mac_betxsz_history[8];          /* from MAC firmware */
    unsigned int mac_vodropframe_history[8];     /* from MAC firmware */
    unsigned int mac_bedropframe_history[8];     /* from MAC firmware */
    unsigned int enq_in_vo_be;
    unsigned int drop_in_be;
    unsigned int mac_votxsz;
    unsigned int mac_votxframe;
    unsigned int mac_vodropsz;
    unsigned int mac_vodropframe;
    unsigned int mac_betxsz;
    unsigned int mac_betxframe;
    unsigned int mac_bedropsz;
    unsigned int mac_bedropframe;
    struct list_head list;
};

typedef enum _WIFI_TUNING_CMD_TYPE {
    WIFI_TUNING_SET = 0,
    WIFI_TUNING_GET,
    WIFI_TUNING_MAX
} WIFI_TUNING_CMD_TYPE;

typedef enum _WIFI_TUNING_SET_CMD {
    WIFI_TUNING_SET_INTERVAL = 0,
    WIFI_TUNING_SET_START,
    WIFI_TUNING_SET_CMD_MAX
} WIFI_TUNING_SET_CMD;

typedef enum _WIFI_TUNING_GET_CMD {
    WIFI_TUNING_GET_STATUS = 0,
    WIFI_TUNING_GET_CMD_MAX
} WIFI_TUNING_GET_CMD;

struct _wifi_tuning_cmd {
    WIFI_TUNING_CMD_TYPE type;
    union {
        WIFI_TUNING_SET_CMD set;
        WIFI_TUNING_GET_CMD get;
    } cmd_code;
    unsigned int buf_len;
    void *buf;
};
#endif

extern u32 nvt_data_mode;

extern s32 nvt_ptk_offload_enable;
extern void nvt_shedule_tx_dequeue(struct _tx_info *tx_info);
extern s32 nvt_get_credit_lender(struct _tx_info *tx_info,
                u8 fifo_borrower);
extern void nvt_tx_dequeue_by_borrow_credit(struct _tx_info *tx_info,
                u8 fifo_type);
extern void nvt_tx_dequeue_worker(struct work_struct *work);
extern void nvt_deaggr_amsdu(struct sk_buff *pkt,
        struct _nvt_hwinfo_rx *info, struct _nvt_if *nvt_if);
extern void nvt_process_rx_list(struct _nvt_bus *nvt_bus,
        struct sk_buff_head *skb_list);
extern void nvt_is_trigger_pkt(struct _nvt_bus *, struct sk_buff *);
extern void nvt_netif_rx(struct _nvt_if *nvt_if, struct sk_buff *skb);
extern struct _nvt_if *
        nvt_get_if_by_index(struct _nvt_adapter *nvt_adapter, u8 ifidx);
extern s32 nvt_del_all_if(struct _nvt_adapter *nvt_adapter);
extern s32 nvt_register_to_system(struct device *dev);
extern s32 nvt_set_macaddress_to_fw(struct _nvt_adapter *nvt_adapter);
extern struct _nvt_if *nvt_vif_first(struct _nvt_adapter *nvt_adapter);
extern void nvt_handle_bcmc_mpdu_skb(struct _nvt_hwinfo_rx *info,
        struct sk_buff *skb, struct _nvt_if *nvt_if,
        u8 ba_idx);
#endif
