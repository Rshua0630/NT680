
#include <linux/types.h>
#include <linux/version.h>
#include <net/cfg80211.h>
#include <linux/kernel.h>
#include <linux/etherdevice.h>
#include <linux/if_arp.h>
#include <linux/ctype.h>

#include "nvt_icfg.h"
#include "nvt_wlan_linux.h"
#include "nvt_util_dbg.h"
#include "nvt_iw.h"
#include "nvt_bus_sdioif.h"

#define NVT_NUM_SCAN_MAX 5
#define NVT_SCAN_VSIE_LEN_MAX   512
#define MAX_EP0_SIZE     1024
#define RTS_THRESHOLD_LIMIT 255
#define FRAG_LOW_LIMIT 255
#define FRAG_UP_LIMIT 7936
#define RETRY_LIMIT 255

#define DOT11_MGMT_HDR_LEN              24      /* d11 management header len */
#define DOT11_BCN_PRB_FIXED_LEN         12      /* beacon/probe fixed length */
#define TLV_HDR_LEN 2

//AT: combine reg0 & reg1/ reg2 & reg3
#define WOW_MAXPATTERNS 4
#define WOW_MAXPATTERNLEN 31
#define POLYNOMIAL              0x8005
#define INITIAL_REMAINDER       0xFFFF
#define FINAL_XOR_VALUE         0x0000
#define REVERSE_DATA(X)         ((u8) reverse((X), 8))
#define REVERSE_REMAINDER(X)    (X)
#define WIDTH    (8 * sizeof(u16))
#define TOPBIT   (1 << (WIDTH - 1))

/* wow config parameters */
#define WOW_MAGIC_PKT   0x01
#define WOW_PATTERN     0x02
#define WOW_DISCONNECT  0x04
#define WOW_4_REG       0x08
#define WOW_2_REG       0x10
#define WOW_1_REG       0x20

#ifdef CONFIG_WIFI_TUNING_PHASE_II
#define MAC2STR(a) ((a)[0], (a)[1], (a)[2], (a)[3], (a)[4], (a)[5])
#endif

#define CHAN2G(_channel, _freq, _flags) {   \
    .band           = IEEE80211_BAND_2GHZ,  \
    .hw_value       = (_channel),           \
    .center_freq    = (_freq),              \
    .flags          = (_flags),             \
    .max_antenna_gain   = 0,                \
    .max_power      = 30,                   \
}

#define RATETAB_ENT(_rate, _rateid, _flags) {   \
    .bitrate    = (_rate),                  \
    .flags      = (_flags),                 \
    .hw_value   = (_rateid),                \
}

#if LINUX_VERSION_CODE < KERNEL_VERSION(3, 6, 0)
#define WLAN_OUI_MICROSOFT          0x0050f2
#define WLAN_OUI_TYPE_MICROSOFT_WPA 1
#define WLAN_OUI_TYPE_MICROSOFT_WMM 2
#define WLAN_OUI_TYPE_MICROSOFT_WPS 4
#endif

/* Reset Varialble here */
u8 g_wep_type = 0;
u8 g_wep_is_used = 0;
u8 g_start_op = DONT_RESET;

static struct ieee80211_channel nvt_2ghz_channels[] = {
        CHAN2G(1, 2412, 0),
        CHAN2G(2, 2417, 0),
        CHAN2G(3, 2422, 0),
        CHAN2G(4, 2427, 0),
        CHAN2G(5, 2432, 0),
        CHAN2G(6, 2437, 0),
        CHAN2G(7, 2442, 0),
        CHAN2G(8, 2447, 0),
        CHAN2G(9, 2452, 0),
        CHAN2G(10, 2457, 0),
        CHAN2G(11, 2462, 0),
        CHAN2G(12, 2467, 0),
        CHAN2G(13, 2472, 0),
        CHAN2G(14, 2484, 0),
};

static struct ieee80211_rate nvt_rates[] = {
        RATETAB_ENT(10, 0x1, 0),
        RATETAB_ENT(20, 0x2, 0),
        RATETAB_ENT(55, 0x4, 0),
        RATETAB_ENT(110, 0x8, 0),
        RATETAB_ENT(60, 0x10, 0),
        RATETAB_ENT(90, 0x20, 0),
        RATETAB_ENT(120, 0x40, 0),
        RATETAB_ENT(180, 0x80, 0),
        RATETAB_ENT(240, 0x100, 0),
        RATETAB_ENT(360, 0x200, 0),
        RATETAB_ENT(480, 0x400, 0),
        RATETAB_ENT(540, 0x800, 0),
};

#define nvt_g_rates     (nvt_rates + 0)
#define nvt_g_rates_size    12
#define nvt_g_htcap (IEEE80211_HT_CAP_SUP_WIDTH_20_40 | \
                IEEE80211_HT_CAP_SGI_20         | \
                IEEE80211_HT_CAP_SGI_40         | \
                IEEE80211_HT_CAP_GRN_FLD        | \
                IEEE80211_HT_CAP_DSSSCCK40      | \
                IEEE80211_HT_CAP_TX_STBC        | \
                IEEE80211_HT_CAP_RX_STBC)

static struct ieee80211_supported_band nvt_band_2ghz = {
        .n_channels = ARRAY_SIZE(nvt_2ghz_channels),
        .channels = nvt_2ghz_channels,
        .n_bitrates = nvt_g_rates_size,
        .bitrates = nvt_g_rates,
        .ht_cap.cap = nvt_g_htcap,
        .ht_cap.ht_supported = true,
};

static const struct ieee80211_txrx_stypes
nvt_txrx_stypes[NUM_NL80211_IFTYPES] = {
        [NL80211_IFTYPE_STATION] = {
                .tx = 0xffff,
                .rx = BIT(IEEE80211_STYPE_ACTION >> 4) |
                BIT(IEEE80211_STYPE_PROBE_REQ >> 4)
        },
        [NL80211_IFTYPE_AP] = {
                .tx = 0xffff,
                .rx = BIT(IEEE80211_STYPE_ACTION >> 4) |
                BIT(IEEE80211_STYPE_PROBE_REQ >> 4)
        },
        [NL80211_IFTYPE_P2P_CLIENT] = {
                .tx = 0xffff,
                .rx = BIT(IEEE80211_STYPE_ACTION >> 4) |
                BIT(IEEE80211_STYPE_PROBE_REQ >> 4)
        },
        [NL80211_IFTYPE_P2P_GO] = {
                .tx = 0xffff,
                .rx = BIT(IEEE80211_STYPE_ASSOC_REQ >> 4) |
                      BIT(IEEE80211_STYPE_REASSOC_REQ >> 4) |
                      BIT(IEEE80211_STYPE_PROBE_REQ >> 4) |
                      BIT(IEEE80211_STYPE_DISASSOC >> 4) |
                      BIT(IEEE80211_STYPE_AUTH >> 4) |
                      BIT(IEEE80211_STYPE_DEAUTH >> 4) |
                      BIT(IEEE80211_STYPE_ACTION >> 4)
        }
};

static const struct ieee80211_regdomain nvt_regdom_custom = {
        .n_reg_rules = 2,
        .alpha2 =  "99",
        .reg_rules = {
                REG_RULE(2412-10, 2472+10, 40, 6, 20, 0),
                REG_RULE(2484-10, 2484+10, 20, 6, 20, 0),
        }
};

static const u32 nvt_cipher_suites[] = {
        WLAN_CIPHER_SUITE_WEP40,
        WLAN_CIPHER_SUITE_WEP104,
        WLAN_CIPHER_SUITE_TKIP,
        WLAN_CIPHER_SUITE_CCMP,
        WLAN_CIPHER_SUITE_AES_CMAC,
        WLAN_CIPHER_SUITE_SMS4,
};

static const u16 crctable[256] = {
0x0000, 0x8005, 0x800f, 0x000a, 0x801b, 0x001e, 0x0014, 0x8011, 0x8033, 0x0036,
0x003c, 0x8039, 0x0028, 0x802d, 0x8027, 0x0022, 0x8063, 0x0066, 0x006c, 0x8069,
0x0078, 0x807d, 0x8077, 0x0072, 0x0050, 0x8055, 0x805f, 0x005a, 0x804b, 0x004e,
0x0044, 0x8041, 0x80c3, 0x00c6, 0x00cc, 0x80c9, 0x00d8, 0x80dd, 0x80d7, 0x00d2,
0x00f0, 0x80f5, 0x80ff, 0x00fa, 0x80eb, 0x00ee, 0x00e4, 0x80e1, 0x00a0, 0x80a5,
0x80af, 0x00aa, 0x80bb, 0x00be, 0x00b4, 0x80b1, 0x8093, 0x0096, 0x009c, 0x8099,
0x0088, 0x808d, 0x8087, 0x0082, 0x8183, 0x0186, 0x018c, 0x8189, 0x0198, 0x819d,
0x8197, 0x0192, 0x01b0, 0x81b5, 0x81bf, 0x01ba, 0x81ab, 0x01ae, 0x01a4, 0x81a1,
0x01e0, 0x81e5, 0x81ef, 0x01ea, 0x81fb, 0x01fe, 0x01f4, 0x81f1, 0x81d3, 0x01d6,
0x01dc, 0x81d9, 0x01c8, 0x81cd, 0x81c7, 0x01c2, 0x0140, 0x8145, 0x814f, 0x014a,
0x815b, 0x015e, 0x0154, 0x8151, 0x8173, 0x0176, 0x017c, 0x8179, 0x0168, 0x816d,
0x8167, 0x0162, 0x8123, 0x0126, 0x012c, 0x8129, 0x0138, 0x813d, 0x8137, 0x0132,
0x0110, 0x8115, 0x811f, 0x011a, 0x810b, 0x010e, 0x0104, 0x8101, 0x8303, 0x0306,
0x030c, 0x8309, 0x0318, 0x831d, 0x8317, 0x0312, 0x0330, 0x8335, 0x833f, 0x033a,
0x832b, 0x032e, 0x0324, 0x8321, 0x0360, 0x8365, 0x836f, 0x036a, 0x837b, 0x037e,
0x0374, 0x8371, 0x8353, 0x0356, 0x035c, 0x8359, 0x0348, 0x834d, 0x8347, 0x0342,
0x03c0, 0x83c5, 0x83cf, 0x03ca, 0x83db, 0x03de, 0x03d4, 0x83d1, 0x83f3, 0x03f6,
0x03fc, 0x83f9, 0x03e8, 0x83ed, 0x83e7, 0x03e2, 0x83a3, 0x03a6, 0x03ac, 0x83a9,
0x03b8, 0x83bd, 0x83b7, 0x03b2, 0x0390, 0x8395, 0x839f, 0x039a, 0x838b, 0x038e,
0x0384, 0x8381, 0x0280, 0x8285, 0x828f, 0x028a, 0x829b, 0x029e, 0x0294, 0x8291,
0x82b3, 0x02b6, 0x02bc, 0x82b9, 0x02a8, 0x82ad, 0x82a7, 0x02a2, 0x82e3, 0x02e6,
0x02ec, 0x82e9, 0x02f8, 0x82fd, 0x82f7, 0x02f2, 0x02d0, 0x82d5, 0x82df, 0x02da,
0x82cb, 0x02ce, 0x02c4, 0x82c1, 0x8243, 0x0246, 0x024c, 0x8249, 0x0258, 0x825d,
0x8257, 0x0252, 0x0270, 0x8275, 0x827f, 0x027a, 0x826b, 0x026e, 0x0264, 0x8261,
0x0220, 0x8225, 0x822f, 0x022a, 0x823b, 0x023e, 0x0234, 0x8231, 0x8213, 0x0216,
0x021c, 0x8219, 0x0208, 0x820d, 0x8207, 0x0202};

static const u8 extended_capabilities[] = {WLAN_EXT_PTK_OFFLOAD_CAP};

/*AT add set_pmk handler for catching command from supplicant*/
#ifdef VENDOR_CMD_SUPPORT
static int nvt_set_pmk_cmd(struct wiphy *wiphy, struct wireless_dev *wdev,
                           const void *data, int data_len)
{
        struct _nvt_cfg80211 *cfg = (struct _nvt_cfg80211 *)wiphy_priv(wiphy);
        struct _nvt_adapter *nvt_adapter = cfg->nvt_adapter;
        u8 enable_ptkoffload = 1;
        s32 ret = 0;
        ret = nvt_icfg_lock(nvt_adapter->nvt_bus);
        if (ret < 0) {
                goto err_ret;
        }

        ret = nvt_icfg_reset(nvt_adapter->nvt_bus);
        if (ret < 0) {
                goto err_ret;
        }
        /* enable f/w internal supplicant */
        ret = nvt_icfg_add(nvt_adapter->nvt_bus,
                NVT_ICFG_SET, WID_ENABLE_INT_SUPP, &enable_ptkoffload,
                1);
        if (ret < 0) {
                goto err_ret;
        }
        /* add psk to f/w */
        ret = nvt_icfg_add(nvt_adapter->nvt_bus,
                NVT_ICFG_SET, WID_11I_PSK_VALUE, (u8 *)data,
                32);
        if (ret < 0) {
                goto err_ret;
        }
        ret = nvt_icfg_send_and_get_resp(nvt_adapter->nvt_bus, NULL, 0);
err_ret:
        nvt_icfg_unlock(nvt_adapter->nvt_bus);
        return ret;
}

/*AT add vendor command for supporting PTK offload*/
static struct wiphy_vendor_command nvt_vendor_commands[] = {
        {
                .info = {.vendor_id = OUI_NVT,
                        .subcmd = NVT_PTK_OFFLOAD},
                .flags = WIPHY_VENDOR_CMD_NEED_NETDEV,
                .doit = nvt_set_pmk_cmd
        }
};
#endif

#ifdef CONFIG_PM
#if LINUX_VERSION_CODE > KERNEL_VERSION(3, 8, 0)
static struct wiphy_wowlan_support nvt_wowlan_support = {
        .flags = WIPHY_WOWLAN_MAGIC_PKT | WIPHY_WOWLAN_DISCONNECT,
        .n_patterns = WOW_MAXPATTERNS,
        .pattern_max_len = WOW_MAXPATTERNLEN * 4,
        .pattern_min_len = 1,
#if LINUX_VERSION_CODE >= KERNEL_VERSION(3, 9, 0)
        .max_pkt_offset = 255,
#endif
};
#endif
#endif

#if LINUX_VERSION_CODE >= KERNEL_VERSION(3, 4, 0)
static s32 nvt_cfg80211_start_ap(struct wiphy *, struct net_device *,
                          struct cfg80211_ap_settings *);
static s32 nvt_cfg80211_stop_ap(struct wiphy *, struct net_device *);
static s32 nvt_cfg80211_change_beacon(struct wiphy *,
        struct net_device *, struct cfg80211_beacon_data *);
#else
static int nvt_cfg80211_set_channel(struct wiphy *wiphy,
        struct net_device *dev, struct ieee80211_channel *chan,
        enum nl80211_channel_type channel_type);
static int nvt_cfg80211_add_beacon(struct wiphy *wiphy,
        struct net_device *dev,
        struct beacon_parameters *info);
static int nvt_cfg80211_set_beacon(struct wiphy *wiphy,
        struct net_device *dev,
        struct beacon_parameters *info);
static int nvt_cfg80211_del_beacon(struct wiphy *wiphy,
        struct net_device *dev);
#endif

#if LINUX_VERSION_CODE < KERNEL_VERSION(3, 19, 0)
static s32 nvt_cfg80211_del_station(struct wiphy *,
                          struct net_device *, u8 *);
#else
static s32
nvt_cfg80211_del_station(struct wiphy *wiphy, struct net_device *ndev,
               struct station_del_parameters *params);
#endif
static s32
nvt_cfg80211_change_iface(struct wiphy *, struct net_device *,
        enum nl80211_iftype, u32 *, struct vif_params *);
static void nvt_scan_init(struct _nvt_cfg80211 *cfg);
static s32 nvt_filter_p2p_ie(const u8 **, size_t *);
static s32 nvt_filter_wfd_ie(const u8 **, size_t *);

#ifdef CONFIG_PM
static unsigned long reverse(unsigned long data, unsigned char nbits)
{
        unsigned long  reversed = 0x00000000;
        unsigned char  bit;

        /* Reverse the data about the center bit */
        for (bit = 0; bit < nbits; ++bit) {
                /* If the LSB bit is set, set the reflection of it */
                if (data & 0x01) {
                        reversed |= (1 << ((nbits - 1) - bit));
                }

                data = (data >> 1);
        }
        return reversed;
}

static u16 compute_crc(u32 message[], s32 nbytes)
{
        u16 remainder = INITIAL_REMAINDER;
        u16 crc = 0;
        u8  data;
        s32 byte;

        /* Divide the message by the polynomial, a byte at a time */
        for (byte = 0; byte < nbytes; ++byte) {
                data = REVERSE_DATA(message[byte]) ^ (remainder >> (WIDTH - 8));
                remainder = crctable[data] ^ (remainder << 8);
        }

        /* The final remainder is the CRC */
        crc = REVERSE_REMAINDER(remainder) ^ FINAL_XOR_VALUE;
        return crc;
}
#endif

/*Check if sta is connected*/
static bool is_sta_connected(struct _nvt_if *nvt_if)
{
        if (!test_bit(NVT_IF_CONNECTED, &nvt_if->state_flags)) {
                return false;
        }
        return true;
}

static bool check_if_ready(struct _nvt_if *nvt_if)
{
        if (!test_bit(NVT_IF_ENABLED, &nvt_if->state_flags)) {
                return false;
        }
        return true;
}

static bool nvt_is_wpa_ie(const u8 *pos)
{
        return pos[0] == WLAN_EID_VENDOR_SPECIFIC && pos[1] >= 4 &&
                pos[2] == 0x00 && pos[3] == 0x50 &&
                pos[4] == 0xf2 && pos[5] == 0x01;
}

static bool nvt_is_rsn_ie(const u8 *pos)
{
        return pos[0] == WLAN_EID_RSN;
}

/* 0x44 is the WAPI Element ID */
static bool nvt_is_wapi_ie(const u8 *pos)
{
        return pos[0] == 0x44;
}

/* when set_extra_ie, we want to filter out sec IE */
static bool is_sec_ie(const u8 *pos)
{
        if (nvt_is_wpa_ie(pos) || nvt_is_rsn_ie(pos) ||
                        nvt_is_wapi_ie(pos)) {
                return true;
        } else {
                return false;
        }
}

static bool nvt_is_wps_ie(const u8 *pos)
{
        return (pos[0] == WLAN_EID_VENDOR_SPECIFIC &&
                pos[1] >= 4 &&
                pos[2] == 0x00 && pos[3] == 0x50 && pos[4] == 0xf2 &&
                pos[5] == 0x04);
}

static bool nvt_is_an_alpha2(const char *alpha2)
{
        if (!alpha2)
                return false;
        return isalpha(alpha2[0]) && isalpha(alpha2[1]);
}

/*get_num_of_conn_sta : return how many sta are connected to the station
 *
 *@buff => buff is the buffer get from WID_CONNECTED_STA_LIST
 * */
static s32 get_num_of_conn_sta(u8 *buff, u32 size)
{
        s32 i = 0;
        s32 ret_val = 0;
        while (i < size) {
                ret_val++;
                i += 16;
        }
        return ret_val;
}

static u8 nvt_find_bss(struct _nvt_if *nvt_if)
{
        u8 zeromac[ETH_ALEN] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
        u8 match_bss = 0;
        struct _nvt_cfg80211 *cfg = nvt_if->nvt_adapter->nvt_cfg80211;
        struct _nvt_wlan_conf *wconf = &nvt_if->nvt_wconf;
        struct _nvt_bss_info *bss;

        /* check matching bss */
        list_for_each_entry(bss, &cfg->bss_list, list) {
                 /* compare ssid */
                if ((memcmp(bss->ssid, wconf->ssid, wconf->ssid_len)) ||
                        (bss->ssid_len != wconf->ssid_len)) {
                        continue;
                }

                /* compare channel */
                if (wconf->pref_channel) {
                        if (bss->channel != wconf->pref_channel) {
                                continue;
                        }
                }


                /* compare bssid */
                if (memcmp(wconf->bssid, zeromac, ETH_ALEN)) {
                        if (memcmp(wconf->bssid, bss->bssid, ETH_ALEN)) {
                                continue;
                        }
                }

                /* compare security */
                if (wconf->nvt_sec.mode_11i & WID_11I_SECURITY_ON) {
                        if (((wconf->nvt_sec.mode_11i & WID_11I_WEP40) &&
                                (bss->cap_info & 0x10)) ||
                                ((wconf->nvt_sec.mode_11i & WID_11I_WEP104) &&
                                (bss->cap_info & 0x10)) ||
                                ((wconf->nvt_sec.mode_11i & WID_11I_TKIP) &&
                                (bss->dot11i_info & WID_11I_TKIP)) ||
                                ((wconf->nvt_sec.mode_11i & WID_11I_CCMP) &&
                                (bss->dot11i_info & WID_11I_CCMP))) {
                                match_bss = 1;
                                break;
                        } else if (wconf->nvt_sec.ext_mode_11i &
                                        WID_EXT_11I_WAPI) {
                                match_bss = 1;
                                break;
                        }
                } else {
                        if (wconf->ctrl_flag & WPS_FLAG) {
                                match_bss = 1;
                                break;
                        } else {
                                if (!(bss->dot11i_info & WID_11I_SECURITY_ON)) {
                                        match_bss = 1;
                                        break;
                                }
                        }
                }
        }

        if (match_bss) {
                wconf->conn_bss = bss;
        }

        return match_bss;
}

static void nvt_addkey_wait(struct net_device *ndev)
{
        struct _nvt_if *nvt_if = netdev_priv(ndev);
        int ret = 0;

        nvt_dbg(CFG80211, "%s\n", __func__);

        ret = wait_event_timeout(nvt_if->addkey_wait,
                !atomic_read(&nvt_if->eapol_cnt),
                msecs_to_jiffies(ADDKEY_WAIT_EAPOL_TIME));

        if (ret == 0) {
                nvt_dbg(CFG80211, "addkey wait timeout\n");
        }
}

static void nvt_disconnect_wait(struct net_device *ndev)
{
        struct _nvt_if *nvt_if = netdev_priv(ndev);
        struct _nvt_adapter *nvt_adapter = nvt_if->nvt_adapter;
        int ret = 0;

        ret = wait_event_timeout(nvt_if->disconnect_wait,
                atomic_read(&nvt_if->disconnect_cnt),
                msecs_to_jiffies(DISCONNECT_WAIT__TIME));

        if (ret == 0) {
                nvt_dbg(CFG80211, "disconnect timeout\n");

                clear_bit(NVT_IF_CONNECTED, &nvt_if->state_flags);
                if (nvt_if->filter_disconnect == false) {
                        nvt_scan_init(nvt_adapter->nvt_cfg80211);
                        cfg80211_disconnected(ndev, 0, NULL, 0, GFP_KERNEL);
                }
                /* Reset BA handlers */
                nvt_reset_ba_handle(nvt_adapter, 1);

        }
}

static s32 nvt_update_bss(struct _nvt_cfg80211 *cfg,
                struct _nvt_bss_info *binfo)
{
        struct wiphy *wiphy = cfg->wiphy;
        struct ieee80211_channel *chan;
        struct ieee80211_supported_band *band;
        struct cfg80211_bss *bss = NULL;
        s32 notify_signal = 0;

        if (binfo->ie_length > IE_MAX_LEN) {
                return -EIO;
        }

        if (binfo->channel <= 14) {
                band = wiphy->bands[IEEE80211_BAND_2GHZ];
                chan = ieee80211_get_channel(wiphy,
                        ieee80211_channel_to_frequency(binfo->channel,
                                                band->band));
                notify_signal = (s32)binfo->rssi;
                notify_signal = (s16)(0-notify_signal) * 100;


#if LINUX_VERSION_CODE > KERNEL_VERSION(4, 0, 0)
                bss = cfg80211_inform_bss(wiphy, chan,
                        CFG80211_BSS_FTYPE_UNKNOWN, binfo->bssid, 0,
                        binfo->cap_info, binfo->beacon_period,
                        (u8 *)binfo+binfo->ie_offset, binfo->ie_length,
                        notify_signal, GFP_KERNEL);
#else
                bss = cfg80211_inform_bss(wiphy, chan, binfo->bssid, 0,
                        binfo->cap_info, binfo->beacon_period,
                        (u8 *)binfo+binfo->ie_offset, binfo->ie_length,
                        notify_signal, GFP_KERNEL);
#endif
                if (bss == NULL) {
                        return -ENOMEM;
                }
#if LINUX_VERSION_CODE >= KERNEL_VERSION(3, 10, 0)
                cfg80211_put_bss(wiphy, bss);
#else
                cfg80211_put_bss(bss);
#endif
        } else {
                return -EINVAL;
        }

        return 0;
}

static void nvt_bss_connect(struct _nvt_if *nvt_if, u16 status, u8 *data)
{
        struct _nvt_wlan_conf *wconf = &nvt_if->nvt_wconf;
        struct _nvt_adapter *nvt_adapter = nvt_if->nvt_adapter;
        struct _nvt_cfg80211 *cfg = nvt_adapter->nvt_cfg80211;
        s32 assoc_req_len = 0;
        s32 assoc_rsp_len = 0;

        if (test_bit(NVT_IF_CONNECTING, &nvt_if->state_flags)) {
                clear_bit(NVT_IF_CONNECTING, &nvt_if->state_flags);
        }

        if (status == WLAN_STATUS_SUCCESS) {
                assoc_req_len = data[12] | (data[13] << 8);
                assoc_rsp_len = data[14] | (data[15] << 8);

                if (wconf->conn_bss != NULL) {
                        if (nvt_update_bss(cfg, wconf->conn_bss) < 0) {
                                nvt_dbg(CFG80211, "update bss fail\n");
                        } else {
                                nvt_dbg(CFG80211, "conn bss empty\n");

                                wconf->conn_bss = NULL;
                                set_bit(NVT_IF_CONNECTED, &nvt_if->state_flags);

                                cfg80211_connect_result(nvt_if->ndev, &data[4],
                                        &data[16], assoc_req_len, &data[528],
                                        assoc_rsp_len, WLAN_STATUS_SUCCESS,
                                        GFP_KERNEL);
                        }
                }
        } else {
                cfg80211_connect_result(nvt_if->ndev, NULL, NULL, 0,
                        NULL, 0, WLAN_STATUS_UNSPECIFIED_FAILURE, GFP_KERNEL);
        }
}

static s32 nvt_disconnect_action(struct _nvt_if *nvt_if)
{
        struct _nvt_adapter *nvt_adapter = nvt_if->nvt_adapter;
        u8 val = 0;

        mutex_lock(&nvt_if->disconnect_lock);
        atomic_set(&nvt_if->eapol_cnt, 0);
        atomic_set(&nvt_if->disconnect_cnt, 0);

        if (nvt_icfg_reset(nvt_adapter->nvt_bus) < 0) {
                return -1;
        }

        if (nvt_icfg_add(nvt_adapter->nvt_bus, NVT_ICFG_SET, WID_DISCONNECT,
                (u8 *)&val, sizeof(val)) < 0) {
                return -1;
        }

        if (nvt_icfg_send_and_get_resp(nvt_adapter->nvt_bus, NULL, 0) < 0) {
                return -1;
        }

        nvt_disconnect_wait(nvt_if->wdev.netdev);
        mutex_unlock(&nvt_if->disconnect_lock);

        return 0;
}

static void nvt_scan_complete(struct _nvt_adapter *nvt_adapter,
                                bool drv_abort_scan, u8 fw_stop_scan)
{
        struct _nvt_bus *nvt_bus = nvt_adapter->nvt_bus;
        struct _nvt_cfg80211 *cfg = nvt_adapter->nvt_cfg80211;
        struct cfg80211_scan_request *scan_request = NULL;
        struct _nvt_if *nvt_if = cfg->scan_if;

        mutex_lock(&cfg->scan_lock);
        scan_request = cfg->scan_request;
        cfg->scan_request = NULL;

        if (timer_pending(&cfg->scan_timer)) {
                del_timer_sync(&cfg->scan_timer);
        }

        if (fw_stop_scan) {
                if (nvt_icfg_lock(nvt_bus) < 0) {
                        goto next_check;
                }

                if (nvt_icfg_reset(nvt_bus) < 0) {
                        goto next_check;
                }

                if (nvt_icfg_add(nvt_bus, NVT_ICFG_SET, WID_SCAN_ABORT,
                        (u8 *)&fw_stop_scan, sizeof(fw_stop_scan)) < 0) {
                        goto next_check;
                }

                if (nvt_icfg_send_and_get_resp(nvt_bus, NULL, 0) < 0) {
                        goto next_check;
                }

                msleep(DEFAULT_P2P_ACTIVE_TIME);

next_check:
                nvt_icfg_unlock(nvt_bus);
        }

        if (scan_request) {
                cfg80211_scan_done(scan_request, drv_abort_scan);
        }

        if (test_and_clear_bit(SCAN_PROCESS, &cfg->scan_sts)) {
                /* do nothing */
        }

        if (test_bit(NVT_IF_CONNECTING, &nvt_if->state_flags) &&
                (drv_abort_scan == 1)) {
                nvt_bss_connect(nvt_if, WLAN_STATUS_UNSPECIFIED_FAILURE, NULL);
        }

        mutex_unlock(&cfg->scan_lock);
}

static s32 nvt_cfg80211_add_key(struct wiphy *wiphy,
        struct net_device *ndev, u8 key_idx, bool pairwise, const u8 *mac_addr,
        struct key_params *params)
{
        struct _nvt_if *nvt_if = netdev_priv(ndev);
        struct _nvt_adapter *nvt_adapter = nvt_if->nvt_adapter;
        struct _nvt_priv *nvt_priv = &(nvt_adapter->nvt_priv);
        struct _nvt_sec *sec;
        u8 sta_index;
        s32 ret = 0;
        u8 tmpbuf[WLAN_MAX_KEY_LEN];

        nvt_dbg(CFG80211, "%s\n", __func__);

        sec = &nvt_if->nvt_wconf.nvt_sec;
        if ((params->cipher == WLAN_CIPHER_SUITE_TKIP) &&
                (nvt_if->mode == NVT_FW_STA)) {
                memset(tmpbuf, 0x0, WLAN_MAX_KEY_LEN);
                memcpy(tmpbuf, params->key, params->key_len);
                memcpy((void *)&params->key[24], &tmpbuf[16], 8);
                memcpy((void *)&params->key[16], &tmpbuf[24], 8);
        }
        /*Store the broadcast PN*/
        if (params->seq && params->seq_len &&
                (params->cipher == WLAN_CIPHER_SUITE_TKIP ||
                params->cipher == WLAN_CIPHER_SUITE_CCMP)) {
                if (nvt_priv->pn_bcmc == NULL) {
                        nvt_priv->pn_bcmc = kzalloc(params->seq_len,
                                        GFP_KERNEL);
                }
                if (nvt_priv->pn_bcmc != NULL) {
                        memcpy(nvt_priv->pn_bcmc, params->seq, params->seq_len);
                } else {
                        nvt_dbg(ERROR, "broadcast pn alloc fail\n");
                }

        }
        ret = nvt_icfg_lock(nvt_adapter->nvt_bus);
        if (ret < 0) {
                goto err_ret;
        }

        ret = nvt_icfg_reset(nvt_adapter->nvt_bus);
        if (ret < 0) {
                goto err_ret;
        }

        switch (params->cipher) {
        case WLAN_CIPHER_SUITE_WEP40:
        case WLAN_CIPHER_SUITE_WEP104:
                memset(&sec->wep, 0, sizeof(struct _nvt_wep));
                sec->wep.key_id = key_idx;
                sec->wep.key_len = params->key_len;
                memcpy(sec->wep.key_data, params->key, params->key_len);
                if (params->cipher == WLAN_CIPHER_SUITE_WEP40) {
                        sec->wep_type |= (0 << key_idx);
                        ret = nvt_icfg_add(nvt_adapter->nvt_bus,
                                NVT_ICFG_SET, WID_ADD_WEP_KEY, (u8 *)&sec->wep,
                                sizeof(struct _nvt_wep)-8);
                        if (ret < 0) {
                                goto err_ret;
                        }
                } else {
                        sec->wep_type |= (1 << key_idx);
                        ret = nvt_icfg_add(nvt_adapter->nvt_bus,
                                NVT_ICFG_SET, WID_ADD_WEP_KEY, (u8 *)&sec->wep,
                                sizeof(struct _nvt_wep));
                        if (ret < 0) {
                                goto err_ret;
                        }
                }
                sec->wep_type |= 0x80;
                break;
        case WLAN_CIPHER_SUITE_TKIP:
        case WLAN_CIPHER_SUITE_CCMP:
                for (sta_index = 1; sta_index < 9; sta_index++) {
                        nvt_reset_unicast_pn(nvt_adapter, sta_index);
                }
                if (mac_addr) {
                        memset(&sec->ptk, 0, sizeof(struct _nvt_ptk));
                        memcpy(sec->ptk.addr, mac_addr, ETH_ALEN);
                        sec->ptk.key_len = params->key_len;
                        memcpy(sec->ptk.key_tk, params->key, 16);
                        memcpy(sec->ptk.key_rx_mic, &params->key[16], 8);
                        memcpy(sec->ptk.key_tx_mic, &params->key[24], 8);
                        if (nvt_if->mode == NVT_FW_STA) {
                                nvt_addkey_wait(ndev);
                        }
                        ret = nvt_icfg_add(nvt_adapter->nvt_bus,
                                NVT_ICFG_SET, WID_ADD_PTK, (u8 *)&sec->ptk,
                                sizeof(struct _nvt_ptk));
                        if (ret < 0) {
                                goto err_ret;
                        }
                } else {
                        if (nvt_if->mode == NVT_FW_STA) {
                                memset(&sec->gtk, 0, sizeof(struct _nvt_gtk));
                                memcpy(sec->gtk.addr, nvt_if->nvt_wconf.bssid,
                                        ETH_ALEN);
                                if (params->seq && params->seq_len == 6) {
                                        memcpy(sec->gtk.key_rsc, params->seq,
                                                params->seq_len);
                                }
                                sec->gtk.key_id = key_idx;
                                sec->gtk.key_len = params->key_len;
                                memcpy(sec->gtk.key_tk, params->key, 16);
                                memcpy(sec->gtk.key_mic, &params->key[16], 8);
                                ret = nvt_icfg_add(nvt_adapter->nvt_bus,
                                        NVT_ICFG_SET, WID_ADD_RX_GTK,
                                        (u8 *)&sec->gtk,
                                        sizeof(struct _nvt_gtk) - 8);
                                if (ret < 0) {
                                        goto err_ret;
                                }
                        } else if (nvt_if->mode == NVT_FW_AP) {
                                memset(&sec->gtk, 0, sizeof(struct _nvt_gtk));
                                sec->gtk.key_id = key_idx;
                                sec->gtk.key_len = params->key_len;
                                memcpy(sec->gtk.key_tk, params->key, 16);
                                memcpy(sec->gtk.key_mic, &params->key[16], 16);
                                ret = nvt_icfg_add(nvt_adapter->nvt_bus,
                                        NVT_ICFG_SET, WID_ADD_TX_GTK,
                                        (u8 *)&sec->gtk,
                                        sizeof(struct _nvt_gtk));
                                if (ret < 0) {
                                        goto err_ret;
                                }
                        } else {
                                ret = -EINVAL;
                                goto err_ret;
                        }
                }
                break;
        case WLAN_CIPHER_SUITE_AES_CMAC:
                memset(&sec->igtk, 0, sizeof(struct _nvt_gtk));
                memcpy(sec->igtk.addr, nvt_if->nvt_wconf.bssid,
                                        ETH_ALEN);
                if (params->seq && params->seq_len == 6) {
                        memcpy(sec->igtk.key_rsc, params->seq,
                                                params->seq_len);
                }
                sec->igtk.key_id = key_idx;
                sec->igtk.key_len = params->key_len;
                memcpy(sec->igtk.key_tk, params->key, 16);

                if (nvt_if->mode == NVT_FW_STA) {
                        ret = nvt_icfg_add(nvt_adapter->nvt_bus,
                                        NVT_ICFG_SET, WID_ADD_RX_IGTK,
                                        (u8 *)&sec->igtk,
                                        sizeof(struct _nvt_gtk)-8);
                } else {
                        ret = nvt_icfg_add(nvt_adapter->nvt_bus,
                                        NVT_ICFG_SET, WID_ADD_TX_IGTK,
                                        (u8 *)&sec->igtk,
                                        sizeof(struct _nvt_gtk));
                }
                if (ret < 0) {
                        goto err_ret;
                }
                break;
        case WLAN_CIPHER_SUITE_SMS4:
                nvt_dbg(CFG80211, "%s WLAN_CIPHER_SUITE_SMS4\n", __func__);
                if (params->key_len != 32) {
                        ret = -EINVAL;
                        goto err_ret;
                }
                nvt_dbg(CFG80211, "key_len=%d, seq_len=%d, pairwise=%d\n",
                        params->key_len, params->seq_len, pairwise);
                if (pairwise) {
                        memset(&sec->wapi_ptk, 0, sizeof(struct _nvt_wapi_ptk));
                        memcpy(sec->wapi_ptk.addr, mac_addr, ETH_ALEN);
                        sec->wapi_ptk.key_len = params->key_len;
                        sec->wapi_ptk.key_id = key_idx;
                        memcpy(sec->wapi_ptk.key_tk, params->key, 16);
                        memcpy(sec->wapi_ptk.key_mic, &params->key[16], 16);
                        ret = nvt_icfg_add(nvt_adapter->nvt_bus,
                                NVT_ICFG_SET, WID_ADD_WAPI_PTK,
                                (u8 *)&sec->wapi_ptk,
                                sizeof(struct _nvt_wapi_ptk));
                        if (ret < 0) {
                                goto err_ret;
                        }
                } else {
                        memset(&sec->wapi_gtk, 0, sizeof(struct _nvt_wapi_gtk));
                        memcpy(sec->wapi_gtk.addr,
                                nvt_if->nvt_wconf.bssid, ETH_ALEN);
                        if (params->seq && params->seq_len == 16) {
                                memcpy(sec->wapi_gtk.key_rsc, params->seq,
                                                params->seq_len);
                        }
                        sec->wapi_gtk.key_id = key_idx;
                        sec->wapi_gtk.key_len = params->key_len;
                        memcpy(sec->wapi_gtk.key_tk, params->key, 16);
                        memcpy(sec->wapi_gtk.key_mic, &params->key[16], 16);
                        ret = nvt_icfg_add(nvt_adapter->nvt_bus,
                                NVT_ICFG_SET, WID_ADD_WAPI_GTK,
                                (u8 *)&sec->wapi_gtk,
                                sizeof(struct _nvt_wapi_gtk));
                        if (ret < 0) {
                                goto err_ret;
                        }
                }
                break;
        default:
                ret = -EINVAL;
                goto err_ret;
        }

        ret = nvt_icfg_send_and_get_resp(nvt_adapter->nvt_bus, NULL, 0);
err_ret:
        nvt_icfg_unlock(nvt_adapter->nvt_bus);
        return ret;
}

static s32 nvt_cfg80211_del_key(struct wiphy *wiphy,
        struct net_device *ndev, u8 key_idx, bool pairwise, const u8 *mac_addr)
{
        /* key_id(4)+key_len(4) */
#define RM_OFFSET       8
        u8 tmp_addr[ETH_ALEN];
        u8 rm_addr[RM_OFFSET+ETH_ALEN];
        s32 ret = 0;
        struct _nvt_if *nvt_if = netdev_priv(ndev);
        struct _nvt_adapter *nvt_adapter = nvt_if->nvt_adapter;

        nvt_dbg(CFG80211, "%s\n", __func__);

        if (key_idx >= MAX_KEY_INDEX) {
                return -EINVAL;
        }

        memset(tmp_addr, 0x0, sizeof(tmp_addr));
        memset(rm_addr, 0x0, sizeof(rm_addr));

        if (mac_addr) {
                memcpy(&rm_addr[RM_OFFSET], mac_addr, ETH_ALEN);
        } else {
                if (nvt_if->mode == NVT_FW_STA) {
                        memcpy(&rm_addr[RM_OFFSET], nvt_if->nvt_wconf.bssid,
                                ETH_ALEN);
                }
        }

        ret = nvt_icfg_lock(nvt_adapter->nvt_bus);
        if (ret < 0) {
                goto err_ret;
        }

        ret = nvt_icfg_reset(nvt_adapter->nvt_bus);
        if (ret < 0) {
                goto err_ret;
        }

        ret = nvt_icfg_add(nvt_adapter->nvt_bus, NVT_ICFG_SET,
                WID_REMOVE_WEP_KEY, (u8 *)&tmp_addr, sizeof(tmp_addr));
        if (ret < 0) {
                goto err_ret;
        }

        ret = nvt_icfg_add(nvt_adapter->nvt_bus, NVT_ICFG_SET,
                WID_REMOVE_KEY, (u8 *)&rm_addr, sizeof(rm_addr));
        if (ret < 0) {
                goto err_ret;
        }

        ret = nvt_icfg_send_and_get_resp(nvt_adapter->nvt_bus, NULL, 0);
        if (ret < 0) {
                goto err_ret;
        }

err_ret:
        nvt_icfg_unlock(nvt_adapter->nvt_bus);
        return ret;
}

static s32 nvt_cfg80211_set_default_key(struct wiphy *wiphy,
                                struct net_device *ndev, u8 key_idx,
                                bool unicast, bool multicast)
{
        struct _nvt_if *nvt_if = netdev_priv(ndev);
        struct _nvt_adapter *nvt_adapter = nvt_if->nvt_adapter;
        struct _nvt_sec *sec;
        s32 ret = 0;

        nvt_dbg(CFG80211, "%s\n", __func__);

        sec = &nvt_if->nvt_wconf.nvt_sec;
        if ((nvt_if->mode == NVT_FW_AP) && (sec->wep_type & 0x80)) {
                sec->wep_type |= (key_idx << 4);

                ret = nvt_icfg_lock(nvt_adapter->nvt_bus);
                if (ret < 0) {
                        goto err_ret;
                }

                ret = nvt_icfg_reset(nvt_adapter->nvt_bus);
                if (ret < 0) {
                        goto err_ret;
                }

                ret = nvt_icfg_add(nvt_adapter->nvt_bus, NVT_ICFG_SET,
                        WID_KEY_ID, (u8 *)&key_idx, sizeof(key_idx));
                if (ret < 0) {
                        goto err_ret;
                }

                ret = nvt_icfg_send_and_get_resp(nvt_adapter->nvt_bus,
                        NULL, 0);
                if (ret < 0) {
                        goto err_ret;
                }
err_ret:
                nvt_icfg_unlock(nvt_adapter->nvt_bus);
        }
        return ret;

}

static u32 is_rts_correct(u32 rts_val)
{
        if (rts_val > RTS_THRESHOLD_LIMIT &&
                        rts_val < 0xFFFFFFFF) {
                return 1;
        }
        return 0;
}

static s32 nvt_cfg80211_set_wiphy_params(struct wiphy *wiphy,
        u32 changed)
{
        struct _nvt_cfg80211 *cfg = (struct _nvt_cfg80211 *)wiphy_priv(wiphy);
        struct _nvt_adapter *nvt_adapter = cfg->nvt_adapter;
        struct _nvt_if *nvt_if = nvt_vif_first(nvt_adapter);
        s8 rts_buf[2];
        s8 frag_buf[2];
        s8 retry_long_buf[2];
        s8 retry_short_buf[2];
        u8 is_wiphy_param_set;

        u32 wid = 0;
        s32 ret = 0;
        is_wiphy_param_set = 0;

        nvt_dbg(CFG80211, "%s\n", __func__);
        if (!nvt_if) {
                nvt_dbg(ERROR, "nvt_if == NULL\n");
                return -EPERM;
        }

        if (changed & WIPHY_PARAM_RTS_THRESHOLD) {
                if (is_rts_correct(wiphy->rts_threshold)) {
                        rts_buf[0] = wiphy->rts_threshold & 0xff;
                        rts_buf[1] = (wiphy->rts_threshold >> 8) & 0xff;
                        nvt_if->rts_val = wiphy->rts_threshold;
                        wid = WID_RTS_THRESHOLD;
                        is_wiphy_param_set |= (1 << 0);
                } else {
                        wiphy->rts_threshold = nvt_if->rts_val;
                        nvt_dbg(CFG80211, "rts is not set\n");
                }
        }

        if (changed & WIPHY_PARAM_FRAG_THRESHOLD) {
                if (wiphy->frag_threshold > FRAG_LOW_LIMIT &&
                        wiphy->frag_threshold <= FRAG_UP_LIMIT) {
                        frag_buf[0] = wiphy->frag_threshold & 0xff;
                        frag_buf[1] = (wiphy->frag_threshold >> 8) & 0xff;
                        nvt_if->frag_val = wiphy->frag_threshold;
                        is_wiphy_param_set |= (1 << 1);
                        wid = WID_FRAG_THRESHOLD;
                } else {
                        wiphy->frag_threshold = nvt_if->frag_val;
                        nvt_dbg(CFG80211, "fragment is not set\n");
                }
        }

        if (changed & WIPHY_PARAM_RETRY_LONG) {
                if (wiphy->retry_long != 0 &&
                        wiphy->retry_long <= RETRY_LIMIT) {
                        retry_long_buf[0] = wiphy->retry_long & 0xff;
                        //retry_long_buf[1] = (wiphy->retry_long >> 8) & 0xff;
                        retry_long_buf[1] = 0;
                        nvt_if->retry_long_val = wiphy->retry_long;
                        is_wiphy_param_set |= (1 << 2);
                        wid = WID_LONG_RETRY_LIMIT;

                } else {
                        wiphy->retry_long = nvt_if->retry_long_val;
                        nvt_dbg(CFG80211, "long retry is not set\n");
                }
        }

        if (changed & WIPHY_PARAM_RETRY_SHORT) {
                if (wiphy->retry_short != 0 &&
                        wiphy->retry_short <= RETRY_LIMIT) {
                        retry_short_buf[0] = wiphy->retry_short & 0xff;
                        //retry_short_buf[1] = (wiphy->retry_short >> 8) & 0xff;
                        retry_short_buf[1] = 0;
                        nvt_if->retry_short_val = wiphy->retry_short;
                        wid = WID_SHORT_RETRY_LIMIT;
                        is_wiphy_param_set |= (1 << 3);
                } else {
                        wiphy->retry_short = nvt_if->retry_short_val;
                        nvt_dbg(CFG80211, "short retry is not set\n");
                }
        }

        ret = nvt_icfg_lock(nvt_adapter->nvt_bus);
        if (ret < 0) {
                goto err_ret;
        }

        ret = nvt_icfg_reset(nvt_adapter->nvt_bus);
        if (ret < 0) {
                goto err_ret;
        }
        //check if rts is set
        if (is_wiphy_param_set & (1 << 0)) {
                ret = nvt_icfg_add(nvt_adapter->nvt_bus, NVT_ICFG_SET,
                        WID_RTS_THRESHOLD, (u8 *)rts_buf, sizeof(rts_buf));
                if (ret < 0) {
                        goto err_ret;
                }
        }
        //check if frag is set
        if (is_wiphy_param_set & (1 << 1)) {
                ret = nvt_icfg_add(nvt_adapter->nvt_bus, NVT_ICFG_SET,
                        WID_FRAG_THRESHOLD, (u8 *)frag_buf, sizeof(frag_buf));
                if (ret < 0) {
                        goto err_ret;
                }
        }
        //check if long retry flag is set
        if (is_wiphy_param_set & (1 << 2)) {
                ret = nvt_icfg_add(nvt_adapter->nvt_bus, NVT_ICFG_SET,
                        WID_LONG_RETRY_LIMIT,
                        (u8 *)retry_long_buf, sizeof(retry_long_buf));
                if (ret < 0) {
                        goto err_ret;
                }
        }
        //check if short retry flag is set
        if (is_wiphy_param_set & (1 << 3)) {
                ret = nvt_icfg_add(nvt_adapter->nvt_bus, NVT_ICFG_SET,
                        WID_SHORT_RETRY_LIMIT,
                        (u8 *)retry_short_buf, sizeof(retry_short_buf));
                if (ret < 0) {
                        goto err_ret;
                }
        }

        ret = nvt_icfg_send_and_get_resp(nvt_adapter->nvt_bus, NULL, 0);
        if (ret < 0) {
                goto err_ret;
        }

err_ret:
        nvt_icfg_unlock(nvt_adapter->nvt_bus);
        return ret;
}

static s32 nvt_cfg80211_set_power_mgmt(struct wiphy *wiphy,
                                struct net_device *ndev, bool enabled,
                                s32 timeout)
{
        struct _nvt_if *nvt_if = netdev_priv(ndev);
        struct _nvt_adapter *nvt_adapter;
        s32 ret = 0;

        nvt_dbg(CFG80211, "%s\n", __func__);

        nvt_adapter = nvt_if->nvt_adapter;
        nvt_if->nvt_wconf.pwr_save = enabled;

        ret = nvt_icfg_lock(nvt_adapter->nvt_bus);
        if (ret < 0) {
                goto err_ret;
        }

        ret = nvt_icfg_reset(nvt_adapter->nvt_bus);
        if (ret < 0) {
                goto err_ret;
        }

        ret = nvt_icfg_add(nvt_adapter->nvt_bus, NVT_ICFG_SET,
                WID_POWER_MANAGEMENT, (u8 *)&nvt_if->nvt_wconf.pwr_save,
                sizeof(nvt_if->nvt_wconf.pwr_save));
        if (ret < 0) {
                goto err_ret;
        }

        ret = nvt_icfg_send_and_get_resp(nvt_adapter->nvt_bus, NULL, 0);
        if (ret < 0) {
                goto err_ret;
        }

err_ret:
        nvt_icfg_unlock(nvt_adapter->nvt_bus);
        return ret;
}

static void nvt_scan_init(struct _nvt_cfg80211 *cfg)
{
        ulong flags = 0;
        struct _nvt_bss_info *binfo;
        struct _nvt_bss_info *tmp_binfo;

        if (timer_pending(&cfg->scan_timer)) {
                del_timer_sync(&cfg->scan_timer);
        }

        spin_lock_irqsave(&cfg->bss_list_lock, flags);
        list_for_each_entry_safe(binfo, tmp_binfo, &cfg->bss_list, list) {
                list_del(&binfo->list);
                kfree(binfo);
                cfg->bss_count--;
        }
        spin_unlock_irqrestore(&cfg->bss_list_lock, flags);
}

static s32 nvt_connect_action(struct _nvt_if *nvt_if)
{
        struct _nvt_wlan_conf *wconf = &nvt_if->nvt_wconf;
        struct _nvt_sec *sec = &wconf->nvt_sec;
        struct _nvt_bss_info *conn_bss = wconf->conn_bss;
        struct _nvt_adapter *nvt_adapter = nvt_if->nvt_adapter;
        s32 bss_len = 0;
        s32 ret = 0;
        u8 wps_enable = 0;
        u8 ba_enable = 1;
        u8 start_op = 0;

        if (conn_bss == NULL) {
                return -ENOMEM;
        }

        bss_len = conn_bss->length - sizeof(struct _nvt_bss_info) -
                        conn_bss->ie_length;
        if (bss_len <= 0) {
                return -ENOMEM;
        }

        wps_enable = (nvt_if->nvt_wconf.ctrl_flag & WPS_FLAG) ? 1 : 0;

        ret = nvt_icfg_lock(nvt_adapter->nvt_bus);
        if (ret < 0) {
                goto err_ret;
        }

        ret = nvt_icfg_reset(nvt_adapter->nvt_bus);
        if (ret < 0) {
                goto err_ret;
        }

        if (sec->ciphers_pairwise == WLAN_CIPHER_SUITE_WEP40) {
                ret = nvt_icfg_add(nvt_adapter->nvt_bus, NVT_ICFG_SET,
                        WID_ADD_WEP_KEY, (u8 *)&wconf->nvt_sec.wep,
                        sizeof(struct _nvt_wep)-8);
                if (ret < 0) {
                        goto err_ret;
                }
        } else if (sec->ciphers_pairwise == WLAN_CIPHER_SUITE_WEP104) {
                ret = nvt_icfg_add(nvt_adapter->nvt_bus, NVT_ICFG_SET,
                        WID_ADD_WEP_KEY, (u8 *)&wconf->nvt_sec.wep,
                        sizeof(struct _nvt_wep));
                if (ret < 0) {
                        goto err_ret;
                }
        }

        ret = nvt_icfg_add(nvt_adapter->nvt_bus, NVT_ICFG_SET,
                WID_WPS_REGISTRATION_START, (u8 *)&wps_enable,
                sizeof(wps_enable));
        if (ret < 0) {
                goto err_ret;
        }

        ret = nvt_icfg_add(nvt_adapter->nvt_bus, NVT_ICFG_SET,
                WID_11I_MODE, (u8 *)&wconf->nvt_sec.mode_11i,
                sizeof(u8));
        if (ret < 0) {
                goto err_ret;
        }

        ret = nvt_icfg_add(nvt_adapter->nvt_bus, NVT_ICFG_SET,
                WID_EXT_MODE_11I, (u8 *)&wconf->nvt_sec.ext_mode_11i,
                sizeof(u8));
        if (ret < 0) {
                goto err_ret;
        }

        ret = nvt_icfg_add(nvt_adapter->nvt_bus, NVT_ICFG_SET,
                WID_AUTH_TYPE, (u8 *)&wconf->nvt_sec.auth_type,
                sizeof(u8));
        if (ret < 0) {
                goto err_ret;
        }

        ret = nvt_icfg_add(nvt_adapter->nvt_bus, NVT_ICFG_SET,
                WID_PMF_ENABLE, (u8 *)&wconf->nvt_sec.pmf_enable,
                sizeof(u8));
        if (ret < 0) {
                goto err_ret;
        }

        ret = nvt_icfg_add(nvt_adapter->nvt_bus, NVT_ICFG_SET,
                WID_11N_IMMEDIATE_BA_ENABLED, (u8 *)&ba_enable,
                sizeof(ba_enable));
        if (ret < 0) {
                goto err_ret;
        }

        ret = nvt_icfg_add(nvt_adapter->nvt_bus, NVT_ICFG_SET,
                WID_JOIN_REQ_BSSTBL, (u8 *)conn_bss +
                sizeof(struct _nvt_bss_info), bss_len);
        if (ret < 0) {
                goto err_ret;
        }

        start_op = DO_RESTART;
        ret = nvt_icfg_add(nvt_adapter->nvt_bus, NVT_ICFG_SET,
                WID_START_OP, (u8 *)&start_op, sizeof(start_op));
        if (ret < 0) {
                goto err_ret;
        }

        ret = nvt_icfg_send_and_get_resp(nvt_adapter->nvt_bus, NULL, 0);
        if (ret < 0) {
                goto err_ret;
        }

err_ret:
        nvt_icfg_unlock(nvt_adapter->nvt_bus);
        return ret;
}

static s32 nvt_handle_reg_channel(struct _nvt_adapter *nvt_adapter,
                struct wiphy *wiphy, struct ieee80211_supported_band *sband)
{
        struct ieee80211_channel *ch;
        unsigned int i;
        u8 *ch_param = NULL;
        u8 ch_index = 0;
        s32 ret = 0;

        nvt_dbg(CFG80211, "%s\n", __func__);

        if (!sband)
                return 0;

        ch_param = kzalloc(sband->n_channels * 9, GFP_KERNEL);
        if (ch_param == NULL) {
                nvt_dbg(CFG80211, "Fail memory allocate!\n");
                ret = -ENOMEM;
                goto err_ret;
        }

        for (i = 0; i < sband->n_channels; i++) {
                ch = &sband->channels[i];
                ch_param[ch_index++] = ch->band;
                ch_param[ch_index++] = ch->hw_value;
                ch_param[ch_index++] = ch->max_power;
                ch_param[ch_index++] =
                        (ch->flags & IEEE80211_CHAN_DISABLED);
#if LINUX_VERSION_CODE >= KERNEL_VERSION(3, 14, 0)
                ch_param[ch_index++] =
                        (ch->flags & IEEE80211_CHAN_NO_IR) >> 1;
#else
                ch_param[ch_index++] =
                        (ch->flags & IEEE80211_CHAN_PASSIVE_SCAN) >> 1;
#endif
                ch_param[ch_index++] =
                        (ch->flags & IEEE80211_CHAN_RADAR) >> 3;
                ch_param[ch_index++] =
                        (ch->flags & IEEE80211_CHAN_NO_HT40PLUS) >> 4;
                ch_param[ch_index++] =
                        (ch->flags & IEEE80211_CHAN_NO_HT40MINUS) >> 5;
            #if LINUX_VERSION_CODE >= KERNEL_VERSION(3, 6, 0)
                ch_param[ch_index++] =
                        (ch->flags & IEEE80211_CHAN_NO_OFDM) >> 6;
            #endif
        }

        ret = nvt_icfg_lock(nvt_adapter->nvt_bus);
        if (ret < 0) {
                goto err_ret;
        }

        ret = nvt_icfg_reset(nvt_adapter->nvt_bus);
        if (ret < 0) {
                goto err_ret;
        }

        ret = nvt_icfg_add(nvt_adapter->nvt_bus, NVT_ICFG_SET,
                WID_SET_REGFLAGS, (u8 *)ch_param, sband->n_channels * 9);
        if (ret < 0) {
                goto err_ret;
        }

        ret = nvt_icfg_send_and_get_resp(nvt_adapter->nvt_bus, NULL, 0);
        if (ret < 0) {
                goto err_ret;
        }
err_ret:
        kfree(ch_param);
        nvt_icfg_unlock(nvt_adapter->nvt_bus);
        return ret;
}

static s32 nvt_scan_action(struct wiphy *wiphy,
                        struct _nvt_adapter *nvt_adapter, u32 n_channels,
                        struct ieee80211_channel *channels[], u8 *ssid_list_ptr,
                        u8 list_len, u8 scan_type, u16 scan_time,
                        u8 scan_filter, u8 no_cck)
{
        struct _nvt_cfg80211 *cfg = nvt_adapter->nvt_cfg80211;
        //use for ap scanning
        struct _nvt_if *nvt_if = nvt_vif_first(nvt_adapter);
        enum ieee80211_band band;
        u8 *ch_param = NULL;
        u8 ch_param_free = 0;
        u8 ch_index = 0;
        u8 scan_ch_num = 0xff;
        u8 i = 0;
        u8 site_survey = 1;
        u8 user_scan = 1;
        //u8 start_op = 0;
        u32 scan_timeout = 0;
        s32 ret = 0;

        if (!nvt_if) {
                nvt_dbg(ERROR, "nvt_if == NULL\n");
                return -EPERM;
        }


        if ((n_channels > 0) && (n_channels < 128)) {
                ch_param = kzalloc(n_channels * 2 + 1, GFP_KERNEL);
                if (ch_param == NULL) {
                        ch_param = &scan_ch_num;
                } else {
                        ch_param_free = 1;
                        ch_param[ch_index++] = n_channels;
                        for (i = 0; i < n_channels; i++) {
                                ch_param[ch_index++] = channels[i]->band;
                                ch_param[ch_index++] = channels[i]->hw_value;
                        }
                }
        } else {
                ch_param = &scan_ch_num;
        }

        for (band = 0; band < IEEE80211_NUM_BANDS; band++)
                        ret = nvt_handle_reg_channel(nvt_adapter,
                                        wiphy, wiphy->bands[band]);
        if (ret < 0) {
                goto err_ret;
        }

        if (nvt_if->mode != NVT_FW_AP) {
                if (ch_param[0] == 0xFF) {
                        scan_timeout = nvt_band_2ghz.n_channels*scan_time+5000;
                } else {
                        scan_timeout = n_channels*scan_time+5000;
                }
        } else {
                if (ch_param[0] == 0xFF) {
                        scan_timeout = nvt_band_2ghz.n_channels*scan_time
                                        + 20000;
                } else {
                        scan_timeout = n_channels*scan_time + 20000;
                }
        }

        mod_timer(&cfg->scan_timer, jiffies + scan_timeout * HZ/1000);

        ret = nvt_icfg_lock(nvt_adapter->nvt_bus);
        if (ret < 0) {
                goto err_ret;
        }

        ret = nvt_icfg_reset(nvt_adapter->nvt_bus);
        if (ret < 0) {
                goto err_ret;
        }

        /* set scan_type */
        ret = nvt_icfg_add(nvt_adapter->nvt_bus, NVT_ICFG_SET,
                WID_SCAN_TYPE, (u8 *)&scan_type, sizeof(scan_type));
        if (ret < 0) {
                goto err_ret;
        }

        /* set scan filter */
        ret = nvt_icfg_add(nvt_adapter->nvt_bus, NVT_ICFG_SET,
                WID_SCAN_FILTER, (u8 *)&scan_filter, sizeof(scan_filter));
        if (ret < 0) {
                goto err_ret;
        }

        /* set scan time */
        if (scan_type == ACTIVE_SCAN) {
                ret = nvt_icfg_add(nvt_adapter->nvt_bus, NVT_ICFG_SET,
                        WID_SITE_SURVEY_SCAN_TIME,
                        (u8 *)&scan_time, sizeof(scan_time));
                if (ret < 0) {
                        goto err_ret;
                }
        } else {
                ret = nvt_icfg_add(nvt_adapter->nvt_bus, NVT_ICFG_SET,
                        WID_PASSIVE_SCAN_TIME, (u8 *)&scan_time,
                        sizeof(scan_time));
                if (ret < 0) {
                        goto err_ret;
                }
        }

        /* set scan channel list */
        if (ch_param[0] == 0xFF) {
                ret = nvt_icfg_add(nvt_adapter->nvt_bus, NVT_ICFG_SET,
                        WID_SCAN_MULTICH, (u8 *)ch_param, sizeof(u8));
                if (ret < 0) {
                        goto err_ret;
                }
        } else {
                ret = nvt_icfg_add(nvt_adapter->nvt_bus, NVT_ICFG_SET,
                        WID_SCAN_MULTICH, (u8 *)ch_param, n_channels*2+1);
                if (ret < 0) {
                        goto err_ret;
                }
        }

        /* set enable site survey */
        ret = nvt_icfg_add(nvt_adapter->nvt_bus, NVT_ICFG_SET,
                WID_SITE_SURVEY, (u8 *)&site_survey, sizeof(u8));
        if (ret < 0) {
                goto err_ret;
        }

        /* set ssid list */
        ret = nvt_icfg_add(nvt_adapter->nvt_bus, NVT_ICFG_SET,
                WID_SCAN_MULTISSID, (u8 *)ssid_list_ptr, list_len);
        if (ret < 0) {
                goto err_ret;
        }

        /* set user scan */
        ret = nvt_icfg_add(nvt_adapter->nvt_bus, NVT_ICFG_SET,
                WID_START_SCAN_REQ, (u8 *)&user_scan, sizeof(user_scan));
        if (ret < 0) {
                goto err_ret;
        }

        /* set op mode */
        if (no_cck == 0) {
                no_cck = 2;
        } else {
                no_cck = 1;
        }

        ret = nvt_icfg_add(nvt_adapter->nvt_bus, NVT_ICFG_SET,
                WID_11G_OPERATING_MODE, (u8 *)&no_cck, sizeof(no_cck));
        if (ret < 0) {
                goto err_ret;
        }

        /* set reset mode */
        //start_op = DO_RESTART;
        //ret = nvt_icfg_add(nvt_adapter->nvt_bus, NVT_ICFG_SET,
        //        WID_START_OP, (u8 *)&start_op, sizeof(start_op));
        //if (ret < 0) {
        //        goto err_ret;
        //}

        ret = nvt_icfg_send_and_get_resp(nvt_adapter->nvt_bus, NULL, 0);
        if (ret < 0) {
                goto err_ret;
        }

err_ret:
        if (ch_param_free == 1) {
                kfree(ch_param);
        }

        nvt_icfg_unlock(nvt_adapter->nvt_bus);
        return ret;
}

#if LINUX_VERSION_CODE >= KERNEL_VERSION(3, 6, 0)
static s32 nvt_cfg80211_scan(struct wiphy *wiphy,
                        struct cfg80211_scan_request *request)
{
        struct _nvt_if *nvt_if =
                container_of(request->wdev, struct _nvt_if, wdev);
#else
static s32 nvt_cfg80211_scan(struct wiphy *wiphy,
                        struct net_device *ndev,
                        struct cfg80211_scan_request *request)
{
        struct _nvt_if *nvt_if = netdev_priv(ndev);
#endif
        struct _nvt_cfg80211 *cfg = wiphy_priv(wiphy);
        struct _nvt_adapter *nvt_adapter = cfg->nvt_adapter;
        struct _nvt_wlan_conf *wconf = &nvt_if->nvt_wconf;
        struct _nvt_probe_ssid ssid_list[MAX_PROBED_SSIDS];
        s32 ret = 0;
        u8 scan_filter = 0;
        u8 scan_type = ACTIVE_SCAN;
        u8 list_len = 0;
        u8 i = 0;
        u8 p2p_enable = 0;
        u8 vsie_enable = 0;
        u16 scan_time = 0;

        nvt_dbg(CFG80211, "%s\n", __func__);

        if (!check_if_ready(nvt_if)) {
                return -EIO;
        }

        /* check scan status */
        if (test_bit(SCAN_PROCESS, &cfg->scan_sts)) {
                nvt_dbg(CFG80211, "scan is processing\n");
                return -EAGAIN;
        }

        /* check scan ssid num */
        if (request->n_ssids > MAX_PROBED_SSIDS) {
                nvt_dbg(CFG80211, "request ssid [%d] over max ssids\n",
                        request->n_ssids);
                return -EINVAL;
        }

        if (nvt_if->start_ap_flag == 0 && nvt_if->mode == NVT_FW_AP) {
                nvt_dbg(CFG80211, "scan abort before start_ap ops\n");
                cfg80211_scan_done(request, 1);
                return 0;
        }

        set_bit(SCAN_PROCESS, &cfg->scan_sts);

        /* clean ctrl flag */
        wconf->ctrl_flag &= ~(PROBE_REQ_EXTRA_IE_FLAG|WPS_FLAG|
                        P2P_WILDCARD_SSID_FLAG|P2P_FLAG);

        nvt_scan_init(cfg);

        cfg->scan_request = request;
        cfg->scan_if = nvt_if;

        scan_filter = SCAN_NO_ADHOC | SCAN_SAVE_WITHOUTRSSI | SCAN_CURRENTCH;

        memset(ssid_list, 0x0, sizeof(struct _nvt_probe_ssid)*MAX_PROBED_SSIDS);


        if (request->n_ssids > 0) {
                for (i = 0; i < request->n_ssids; i++) {
                        ssid_list[i].index = i;
                        ssid_list[i].ssid_len = request->ssids[i].ssid_len;

                        if (ssid_list[i].ssid_len) {
                                memcpy(ssid_list[i].ssid,
                                        request->ssids[i].ssid,
                                        request->ssids[i].ssid_len);
                        }
                        list_len += sizeof(struct _nvt_probe_ssid);
                }
                scan_type = ACTIVE_SCAN;
                if (wconf->ctrl_flag & P2P_WILDCARD_SSID_FLAG) {
                        scan_time = DEFAULT_P2P_ACTIVE_TIME;
                } else {
                        if (nvt_if->mode != NVT_FW_AP)
                                scan_time = DEFAULT_ACTIVE_TIME;
                        else
                                scan_time = DEFAULT_AP_SCAN_TIME;
                }
        } else {
                ssid_list[i].index = 0;
                ssid_list[i].ssid_len = 0;
                scan_type = PASSIVE_SCAN;
                scan_time = DEFAULT_PASSIVE_TIME;
                list_len += sizeof(struct _nvt_probe_ssid);
        }

        ret = nvt_icfg_lock(nvt_adapter->nvt_bus);
        if (ret < 0) {
                goto err_ret;
        }

        ret = nvt_icfg_reset(nvt_adapter->nvt_bus);
        if (ret < 0) {
                goto err_ret;
        }

        if ((request->ie != NULL) && (request->ie_len > 0)) {
                if (cfg80211_find_vendor_ie(WLAN_OUI_WFA, WLAN_OUI_TYPE_WFA_P2P,
                        request->ie, request->ie_len)) {
                        p2p_enable = 1;
                        wconf->ctrl_flag |= P2P_FLAG;
                        ret = nvt_icfg_add(nvt_adapter->nvt_bus,
                                NVT_ICFG_SET, WID_P2P_ENABLE, (u8 *)&p2p_enable,
                                 sizeof(p2p_enable));
                        if (ret < 0) {
                                goto err_ret;
                        }
                } else {
                        p2p_enable = 0;
                        ret = nvt_icfg_add(nvt_adapter->nvt_bus,
                                NVT_ICFG_SET, WID_P2P_ENABLE, (u8 *)&p2p_enable,
                                 sizeof(p2p_enable));
                        if (ret < 0) {
                                goto err_ret;
                        }
                }
                vsie_enable |= 0x02;
                wconf->ctrl_flag |= PROBE_REQ_EXTRA_IE_FLAG;
                ret = nvt_icfg_add(nvt_adapter->nvt_bus, NVT_ICFG_SET,
                        WID_VSIE_TX_PROBEREQ, (u8 *)request->ie,
                        request->ie_len);
                if (ret < 0) {
                        goto err_ret;
                }
        }

        /* if our mode is AP or GO, need to check more vsie flags */
        if (nvt_if->mode == NVT_FW_AP) {
                if (wconf->ctrl_flag & BEACON_EXTRA_IE_FLAG) {
                        vsie_enable |= 0x01;
                }
                if (wconf->ctrl_flag & PROBE_RESP_EXTRA_IE_FLAG) {
                        vsie_enable |= 0x04;
                }
                if (wconf->ctrl_flag & ASSOC_RESP_EXTRA_IE_FLAG) {
                        vsie_enable |= 0x08;
                }
        }

        ret = nvt_icfg_add(nvt_adapter->nvt_bus, NVT_ICFG_SET,
                WID_VSIE_FRAME, (u8 *)&vsie_enable, sizeof(vsie_enable));
        if (ret < 0) {
                goto err_ret;
        }

        ret = nvt_icfg_send_and_get_resp(nvt_adapter->nvt_bus, NULL, 0);
        if (ret < 0) {
                goto err_ret;
        }

        nvt_icfg_unlock(nvt_adapter->nvt_bus);

        ret = nvt_scan_action(wiphy, nvt_adapter, request->n_channels,
                request->channels, (u8 *)ssid_list, list_len, scan_type,
                scan_time, scan_filter, (u8)request->no_cck);
        if (ret < 0) {
                goto err_ret;
        }

        return ret;

err_ret:
        nvt_icfg_unlock(nvt_adapter->nvt_bus);

        cfg->scan_request = NULL;
        if (timer_pending(&cfg->scan_timer)) {
                del_timer_sync(&cfg->scan_timer);
        }
        clear_bit(SCAN_PROCESS, &cfg->scan_sts);
        return ret;
}

#if LINUX_VERSION_CODE >= KERNEL_VERSION(3, 9, 0)
static u8 nvt_pmf_check(u8 *ie, u8 *ielen)
{
#define GROUP_CIPHER_LEN        4
#define PAIRWISE_CIPHER_CNT_LEN 2
#define PAIRWISE_CIPHER_LEN     4
#define AKM_SUITE_CNT           2
#define AKM_SUITE_LEN           4

        u8 AKM_PSK_SHA256[] = {0x00, 0x0f, 0xac, 0x06};
        u8 ret = 0;
        u16 len = (u16)*ielen;
        u16 cnt = 0;
        u16 i = 0;
        u16 rsn_capable = 0;

        if (ie[2] == 1) {
                ie += 4;
                len -= 2;
                if (len >= GROUP_CIPHER_LEN) {
                        ie += GROUP_CIPHER_LEN;
                        len -= GROUP_CIPHER_LEN;
                } else {
                        return ret;
                }

                if (len >= PAIRWISE_CIPHER_CNT_LEN) {
                        cnt = ((ie[1] << 8) | ie[0]);
                        ie += PAIRWISE_CIPHER_CNT_LEN;
                        len -= PAIRWISE_CIPHER_CNT_LEN;
                        if ((cnt == 0) ||
                                (len < (cnt * PAIRWISE_CIPHER_LEN))) {
                                return ret;
                        } else {
                                ie += PAIRWISE_CIPHER_LEN * cnt;
                                len -= PAIRWISE_CIPHER_LEN * cnt;
                        }
                } else {
                        return ret;
                }

                if (len >= AKM_SUITE_CNT) {
                        cnt = ((ie[1] << 8) | ie[0]);
                        ie += AKM_SUITE_CNT;
                        len -= AKM_SUITE_CNT;
                        for (i = 0; i < cnt; i++) {
                                if (memcmp(ie, AKM_PSK_SHA256,
                                                AKM_SUITE_LEN) == 0) {
                                        ret |= PMF_SHA256;
                                }
                                ie += AKM_SUITE_LEN;
                                len -= AKM_SUITE_LEN;
                        }
                } else {
                        return ret;
                }

                if (len >= 2) {
                        rsn_capable = ((ie[1] << 8) | ie[0]);
                        if (rsn_capable & 0x80) {
                                ret |= PMF_CAPABLE;
                                if (rsn_capable & 0x40) {
                                        ret |= PMF_REQUIRED;
                                }
                        }
                } else {
                        return 0;
                }
        }

        return ret;
}
#endif

static s32 nvt_set_extra_ies(struct _nvt_if *nvt_if,
        struct cfg80211_connect_params *sme)
{
        const u8      *ies = sme->ie;
        size_t  ies_len = sme->ie_len;
        struct _nvt_adapter *nvt_adapter = nvt_if->nvt_adapter;
        struct _nvt_wlan_conf *wconf =  &nvt_if->nvt_wconf;
        u8 *buf = NULL;
        const u8 *pos = NULL;
        u8 vsie_enable = 0;
        size_t len = 0;
        s32 ret = 0;
#if LINUX_VERSION_CODE >= KERNEL_VERSION(3, 9, 0)
        struct _nvt_sec *sec = &nvt_if->nvt_wconf.nvt_sec;
        u8 *rsn_ie = NULL;
#endif
        if (ies && ies_len) {
#if LINUX_VERSION_CODE >= KERNEL_VERSION(3, 9, 0)
                // ??
                if (sme->mfp == NL80211_MFP_REQUIRED) {
                        rsn_ie = (u8 *)cfg80211_find_ie(WLAN_EID_RSN, ies,
                                                ies_len);
                        if (rsn_ie != NULL) {
                                sec->pmf_enable = nvt_pmf_check(rsn_ie,
                                                        rsn_ie+1);
                        }
                }
#else
#endif

                buf = kzalloc(ies_len, GFP_KERNEL);
                if (buf == NULL) {
                        return -ENOMEM;
                }
                pos = ies;
                wconf->ctrl_flag &= ~WPS_FLAG;

                while (pos + 1 < ies + ies_len) {
                        if (pos + 2 + pos[1] > ies + ies_len) {
                                break;
                        }
                        if (!is_sec_ie(pos)) {
                                memcpy(buf + len, pos, 2 + pos[1]);
                                len += 2 + pos[1];
                        }

                        if (nvt_is_wps_ie(pos)) {
                                wconf->ctrl_flag |= WPS_FLAG;
                        }

                        pos += 2 + pos[1];
                }
                ret = nvt_icfg_lock(nvt_adapter->nvt_bus);
                if (ret < 0) {
                        goto err_ret;
                }

                ret = nvt_icfg_reset(nvt_adapter->nvt_bus);
                if (ret < 0) {
                        goto err_ret;
                }

                if ((len != 0) &&
                        (wconf->ctrl_flag & PROBE_REQ_EXTRA_IE_FLAG)) {
                        vsie_enable |= 0x82;
                        ret = nvt_icfg_add(nvt_adapter->nvt_bus,
                                NVT_ICFG_SET, WID_VSIE_TX_ASSOCREQ, (u8 *)buf,
                                len);
                        if (ret < 0) {
                                goto err_ret;
                        }

                        ret = nvt_icfg_add(nvt_adapter->nvt_bus,
                                NVT_ICFG_SET, WID_VSIE_FRAME,
                                (u8 *)&vsie_enable, sizeof(vsie_enable));
                        if (ret < 0) {
                                goto err_ret;
                        }
                } else if ((len == 0) &&
                        (wconf->ctrl_flag & PROBE_REQ_EXTRA_IE_FLAG)) {
                        vsie_enable |= 0x02;
                        ret = nvt_icfg_add(nvt_adapter->nvt_bus,
                                NVT_ICFG_SET, WID_VSIE_FRAME,
                                (u8 *)&vsie_enable, sizeof(vsie_enable));
                        if (ret < 0) {
                                goto err_ret;
                        }
                } else if ((len != 0) &&
                        !(wconf->ctrl_flag & PROBE_REQ_EXTRA_IE_FLAG)) {
                        vsie_enable |= 0x80;
                        ret = nvt_icfg_add(nvt_adapter->nvt_bus,
                                NVT_ICFG_SET, WID_VSIE_TX_ASSOCREQ, (u8 *)buf,
                                len);
                        if (ret < 0) {
                                goto err_ret;
                        }

                        ret = nvt_icfg_add(nvt_adapter->nvt_bus,
                                NVT_ICFG_SET, WID_VSIE_FRAME,
                                (u8 *)&vsie_enable, sizeof(vsie_enable));
                        if (ret < 0) {
                                goto err_ret;
                        }
                } else {
                        ret = nvt_icfg_add(nvt_adapter->nvt_bus,
                                NVT_ICFG_SET, WID_VSIE_FRAME,
                                (u8 *)&vsie_enable, sizeof(vsie_enable));
                        if (ret < 0) {
                                goto err_ret;
                        }
                }

                ret = nvt_icfg_send_and_get_resp(nvt_adapter->nvt_bus,
                        NULL, 0);
                if (ret < 0) {
                        goto err_ret;
                }

                nvt_icfg_unlock(nvt_adapter->nvt_bus);

                kfree(buf);
        }
        return ret;

err_ret:
        nvt_icfg_unlock(nvt_adapter->nvt_bus);

        kfree(buf);

        return ret;
}

static void nvt_set_auth_by_ext_mode_11i(u8 ext_mode_11i, u8 mode_11i,
                struct cfg80211_connect_params *sme,
                struct _nvt_sec *sec)
{
        /* reset the auth_type */
        sec->auth_type = 0;

        /* May check other condition for WAPI CERT in the future */
        if (ext_mode_11i & WID_EXT_11I_WAPI) {
                sec->auth_type |= WID_EXT_11I_WAPI;
        }

        /* Check for the original auth_type if mode_11i is being set*/
        if (sme->auth_type == NL80211_AUTHTYPE_OPEN_SYSTEM) {
                sec->auth_type |= WID_AUTH_OPEN_PSK;
        } else if (sme->auth_type == NL80211_AUTHTYPE_SHARED_KEY) {
                sec->auth_type |= WID_AUTH_SHARED_KEY;
        } else if (sme->auth_type == NL80211_AUTHTYPE_AUTOMATIC) {
                sec->auth_type |= (WID_AUTH_OPEN_PSK | WID_AUTH_SHARED_KEY);
        } else if (sme->auth_type == NL80211_AUTHTYPE_NETWORK_EAP) {
                sec->auth_type |= WID_AUTH_802_1X;
        } else {
                sec->auth_type |= WID_AUTH_OPEN_PSK;
        }
}

static s32 nvt_cfg80211_connect(struct wiphy *wiphy,
        struct net_device *ndev, struct cfg80211_connect_params *sme)
{
        struct _nvt_if *nvt_if = netdev_priv(ndev);
        struct _nvt_sec *sec = &nvt_if->nvt_wconf.nvt_sec;
        struct _nvt_cfg80211 *cfg = nvt_if->nvt_adapter->nvt_cfg80211;
        struct _nvt_probe_ssid ssid_list[MAX_PROBED_SSIDS];
        struct ieee80211_channel *ch = sme->channel;
        s32 ret = 0;
        u8 scan_filter = 0;
        u8 list_len = 0;
        u8 p2p_ie = 0;

        nvt_dbg(CFG80211, "%s\n", __func__);

        if (!check_if_ready(nvt_if)) {
                return -EIO;
        }

        if (sme->ssid == NULL) {
                return -EINVAL;
        }

        set_bit(NVT_IF_CONNECTING, &nvt_if->state_flags);

        /* clean wlan config */
        memset(&nvt_if->nvt_wconf, 0x0, sizeof(struct _nvt_wlan_conf));

        /* save ssid */
        nvt_if->nvt_wconf.ssid_len = sme->ssid_len;
        memcpy(nvt_if->nvt_wconf.ssid, sme->ssid, nvt_if->nvt_wconf.ssid_len);

        /* save channel */
        if (ch) {
                nvt_if->nvt_wconf.pref_channel =
                        ieee80211_frequency_to_channel(ch->center_freq);
        }

        /* save bssid */
        if (sme->bssid && !is_broadcast_ether_addr(sme->bssid)) {
                memcpy(nvt_if->nvt_wconf.bssid, sme->bssid, ETH_ALEN);
        }

        /* save 11i mode */
        if (sme->crypto.wpa_versions & NL80211_WPA_VERSION_1) {
                sec->mode_11i |= WID_11I_WPA;
        } else if (sme->crypto.wpa_versions & NL80211_WPA_VERSION_2) {
                sec->mode_11i |= WID_11I_RSN;
        }
        /* reset ext mode before using it*/
        sec->ext_mode_11i = 0;
        /* save cipher */
        if (sme->crypto.n_ciphers_pairwise) {
                switch (sme->crypto.ciphers_pairwise[0]) {
                case WLAN_CIPHER_SUITE_WEP40:
                        sec->mode_11i |=  (WID_11I_SECURITY_ON | WID_11I_WEP40);
                        break;
                case WLAN_CIPHER_SUITE_WEP104:
                        sec->mode_11i |= (WID_11I_SECURITY_ON | WID_11I_WEP104);
                        break;
                case WLAN_CIPHER_SUITE_TKIP:
                        sec->mode_11i |= (WID_11I_SECURITY_ON | WID_11I_TKIP);
                        break;
                case WLAN_CIPHER_SUITE_CCMP:
                case WLAN_CIPHER_SUITE_AES_CMAC:
                        sec->mode_11i |= (WID_11I_SECURITY_ON | WID_11I_CCMP);
                        break;
                case WLAN_CIPHER_SUITE_SMS4:
                        sec->mode_11i |= WID_11I_SECURITY_ON;
                        sec->ext_mode_11i |= (WID_EXT_11I_WAPI |
                                        WID_EXT_11I_SMS4);
                        break;
                default:
                        ret = -EINVAL;
                        goto err_ret;
                }
        }
        sec->ciphers_pairwise = sme->crypto.ciphers_pairwise[0];
        if (sme->crypto.cipher_group) {
                switch (sme->crypto.cipher_group) {
                case WLAN_CIPHER_SUITE_WEP40:
                        sec->mode_11i |= (WID_11I_SECURITY_ON | WID_11I_WEP40);
                        break;
                case WLAN_CIPHER_SUITE_WEP104:
                        sec->mode_11i |= (WID_11I_SECURITY_ON | WID_11I_WEP104);
                        break;
                case WLAN_CIPHER_SUITE_TKIP:
                        sec->mode_11i |= (WID_11I_SECURITY_ON | WID_11I_TKIP);
                        break;
                case WLAN_CIPHER_SUITE_CCMP:
                case WLAN_CIPHER_SUITE_AES_CMAC:
                        sec->mode_11i |= (WID_11I_SECURITY_ON | WID_11I_CCMP);
                        break;
                case WLAN_CIPHER_SUITE_SMS4:
                        sec->mode_11i |= WID_11I_SECURITY_ON;
                        sec->ext_mode_11i |= (WID_EXT_11I_WAPI |
                                        WID_EXT_11I_SMS4);
                        break;
                default:
                        ret = -EINVAL;
                        goto err_ret;
                }
        }

        //set the auth-type
        nvt_set_auth_by_ext_mode_11i(sec->ext_mode_11i, sec->mode_11i,
                        sme, sec);

        /* save wep key */
        if ((sme->key_len != 0) && (sme->crypto.ciphers_pairwise[0] &
                (WLAN_CIPHER_SUITE_WEP40 | WLAN_CIPHER_SUITE_WEP104))) {
                sec->wep.key_id = sme->key_idx;
                sec->wep.key_len = sme->key_len;
                memcpy(sec->wep.key_data, sme->key, sme->key_len);
        }

        /* check if connected state. if connected, disconnect and connect new */
        if (test_bit(NVT_IF_CONNECTED, &nvt_if->state_flags)) {
                nvt_if->filter_disconnect = true;
                nvt_disconnect_action(nvt_if);
                nvt_if->filter_disconnect = false;
        }

        /* set probe req or assoc req extra ie to fw */
        ret = nvt_set_extra_ies(nvt_if, sme);
        if (ret < 0) {
                nvt_dbg(CFG80211, "set extra ie fail\n");
                goto err_ret;
        }

        /* check bss list */
        if (nvt_find_bss(nvt_if)) {
                /* find matching bss, do connect */
                ret = nvt_connect_action(nvt_if);
                if (ret < 0) {
                        goto err_ret;
                }
        } else {
                /* no find matching bss, scan again */
                set_bit(SCAN_PROCESS, &cfg->scan_sts);
                nvt_scan_init(cfg);
                ssid_list[0].index = 0;
                ssid_list[0].ssid_len = nvt_if->nvt_wconf.ssid_len;
                if (ssid_list[0].ssid_len) {
                        memcpy(ssid_list[0].ssid, nvt_if->nvt_wconf.ssid,
                                nvt_if->nvt_wconf.ssid_len);
                }
                list_len += sizeof(struct _nvt_probe_ssid);
                cfg->scan_if = nvt_if;
                scan_filter = SCAN_NO_ADHOC |
                        SCAN_SAVE_WITHOUTRSSI | SCAN_CURRENTCH;
                if (cfg80211_find_vendor_ie(WLAN_OUI_WFA, WLAN_OUI_TYPE_WFA_P2P,
                        sme->ie, sme->ie_len)) {
                        p2p_ie = 1;
                }

                if (nvt_if->nvt_wconf.pref_channel) {
                        ret = nvt_scan_action(wiphy, nvt_if->nvt_adapter, 1,
                                &sme->channel, (u8 *)ssid_list, list_len,
                                ACTIVE_SCAN, DEFAULT_ACTIVE_TIME, scan_filter,
                                p2p_ie);
                        if (ret < 0) {
                                goto err_ret;
                        }
                } else {
                        ret = nvt_scan_action(wiphy, nvt_if->nvt_adapter, 0,
                                NULL, (u8 *)ssid_list, list_len,
                                ACTIVE_SCAN, DEFAULT_ACTIVE_TIME, scan_filter,
                                p2p_ie);
                        if (ret < 0) {
                                goto err_ret;
                        }
                }
        }

        return ret;

err_ret:
        clear_bit(NVT_IF_CONNECTING, &nvt_if->state_flags);
        return ret;
}

static s32 nvt_cfg80211_disconnect(struct wiphy *wiphy,
        struct net_device *ndev, u16 reason_code)
{
        struct _nvt_if *nvt_if = netdev_priv(ndev);

        if (!check_if_ready(nvt_if)) {
                return -EIO;
        }

        nvt_disconnect_action(nvt_if);

        return 0;
}
#if LINUX_VERSION_CODE >= KERNEL_VERSION(3, 6, 0)
static void nvt_cfg80211_mgmt_frame_register(struct wiphy *wiphy,
        struct wireless_dev *wdev, u16 frame_type, bool reg)
{
        struct _nvt_if *nvt_if = container_of(wdev, struct _nvt_if, wdev);
#else
static void nvt_cfg80211_mgmt_frame_register(struct wiphy *wiphy,
        struct net_device *ndev, u16 frame_type, bool reg)
{
        struct _nvt_if *nvt_if = netdev_priv(ndev);
#endif
        u16 mgmt_type;

        nvt_dbg(CFG80211, "%s\n", __func__);
        nvt_dbg(CFG80211, "frame_type = %04x, reg = %d\n", frame_type, reg);

        mgmt_type = (frame_type & IEEE80211_FCTL_STYPE) >> 4;

        if (reg) {
                nvt_if->mgmt_rx_reg |= BIT(mgmt_type);
        } else {
                nvt_if->mgmt_rx_reg &= ~BIT(mgmt_type);
        }
}

static void nvt_send_probe_resp_frame(struct _nvt_cfg80211 *nvt_cfg,
        struct _nvt_if *nvt_if, u16 channel_nr, unsigned int wait,
        const u8 *buf, u8 *wid_frame_buf, size_t len)
{
        struct _nvt_p2p *nvt_p2p = &nvt_cfg->nvt_p2p;
        struct _nvt_adapter *nvt_adapter = nvt_if->nvt_adapter;
        const u8 *ie_ptr = NULL;
        s32 ie_offset;
        s32 ie_len;
        s32 ret = 0;

        nvt_dbg(P2P, "%s\n", __func__);

        /* get DSSS Parameter Set value */
        ie_offset = IEEE80211_BCN_PRB_FIXED_LEN;
        ie_len = cpu_to_le16(len - ie_offset);
        ie_ptr = cfg80211_find_ie(WLAN_EID_DS_PARAMS,
                (u8 *)&buf[ie_offset], ie_len);
        if (ie_ptr == NULL) {
                nvt_dbg(P2P, "ie_ptr == NULL\n");
                return;
        }
        nvt_dbg(P2P, "DSSS Parameter IE = 0x%x\n", ie_ptr[2]);

        if (test_bit(NVT_P2P_ROC_MODE, &nvt_p2p->status)) {
                nvt_dbg(P2P, "FW is still Remain-on-channel right now\n");
                if (nvt_p2p->listen_channel == ie_ptr[2]) {
                        nvt_dbg(P2P,
                                "DSSS value match ROC channel, send it!\n");
                        channel_nr = nvt_p2p->listen_channel;
                } else {
                        nvt_dbg(P2P,
                                "DSSS value mismatch ROC channel, drop it!\n");
                        return;
                }
        } else {
                nvt_dbg(P2P,
                        "Not ROC now, send probe resp. on specified channel\n");
                channel_nr = ie_ptr[2];
        }

        wid_frame_buf[0] = channel_nr;
        memcpy(&wid_frame_buf[1], buf, len);

        ret = nvt_icfg_lock(nvt_adapter->nvt_bus);
        if (ret < 0) {
                goto err_ret;
        }
        ret = nvt_icfg_reset(nvt_adapter->nvt_bus);
        if (ret < 0) {
                goto err_ret;
        }
        ret = nvt_icfg_add(nvt_adapter->nvt_bus, NVT_ICFG_SET,
                WID_MGMT_TX, (u8 *)wid_frame_buf, len + 1);
        if (ret < 0) {
                goto err_ret;
        }
        ret = nvt_icfg_send_and_get_resp(nvt_adapter->nvt_bus, NULL, 0);

err_ret:
        nvt_icfg_unlock(nvt_adapter->nvt_bus);
        if (ret < 0) {
                nvt_dbg(P2P, "WID_MGMT_TX(probe resp.) has problems\n");
        }
        return;
}

static bool nvt_is_p2p_public_action(const void *data, u32 msg_len)
{
        struct _nvt_p2p_public_action *p2p_pub_act;

        if (data == NULL) {
                return false;
        }
        p2p_pub_act = (struct _nvt_p2p_public_action *)data;
        if (msg_len < sizeof(struct _nvt_p2p_public_action) - 1) {
                return false;
        }
        if (p2p_pub_act->category == P2P_PUBLIC_ACTION_CATEGORY &&
            p2p_pub_act->action == P2P_PUBLIC_ACTION_FIELD &&
            memcmp(p2p_pub_act->oui, P2P_OUI, P2P_OUI_LEN) == 0 &&
            p2p_pub_act->oui_type == P2P_OUI_TYPE) {
                return true;
        }
        return false;
}

static bool nvt_is_p2p_action(const void *data, u32 msg_len)
{
        struct _nvt_p2p_action *p2p_act;

        if (data == NULL) {
                return false;
        }
        p2p_act = (struct _nvt_p2p_action *)data;
        if (msg_len < sizeof(struct _nvt_p2p_action) - 1) {
                return false;
        }
        if (p2p_act->category == P2P_ACTION_CATEGORY &&
            memcmp(p2p_act->oui, P2P_OUI, P2P_OUI_LEN) == 0 &&
            p2p_act->oui_type == P2P_OUI_TYPE) {
                return true;
        }
        return false;
}

static bool nvt_is_p2p_gas_public_action(const void *data, u32 msg_len)
{
        struct _nvt_p2p_gas_public_action *p2p_gas_pub_act;

        if (data == NULL) {
                return false;
        }
        p2p_gas_pub_act = (struct _nvt_p2p_gas_public_action *)data;
        if (msg_len < sizeof(struct _nvt_p2p_gas_public_action) - 1) {
                return false;
        }
        if (p2p_gas_pub_act->category != P2P_PUBLIC_ACTION_CATEGORY) {
                return false;
        }
        if (p2p_gas_pub_act->action == P2P_GAS_INITIAL_REQ ||
            p2p_gas_pub_act->action == P2P_GAS_INITIAL_RSP ||
            p2p_gas_pub_act->action == P2P_GAS_COMEBACK_REQ ||
            p2p_gas_pub_act->action == P2P_GAS_COMEBACK_RSP) {
                return true;
        }
        return false;
}

static void nvt_dbg_p2p(bool tx, const void *data, u32 msg_len)
{
        struct _nvt_p2p_public_action *p2p_pub_act;
        struct _nvt_p2p_action *p2p_act;
        struct _nvt_p2p_gas_public_action *p2p_gas_pub_act;

        if (!data || msg_len <= 2) {
                return;
        }

        nvt_dbg(P2P, "%s", (tx) ? "TX " : "RX ");
        if (nvt_is_p2p_public_action(data, msg_len)) {
                p2p_pub_act = (struct _nvt_p2p_public_action *)data;
                switch (p2p_pub_act->subtype) {
                case P2P_GO_NEGO_REQ:
                        nvt_dbg(P2P, "P2P GO NEGO REQ Frame\n");
                        break;
                case P2P_GO_NEGO_RSP:
                        nvt_dbg(P2P, "P2P GO NEGO RSP Frame\n");
                        break;
                case P2P_GO_NEGO_CONF:
                        nvt_dbg(P2P, "P2P GO NEGO CONF Frame\n");
                        break;
                case P2P_INVITE_REQ:
                        nvt_dbg(P2P, "P2P INVITE REQ Frame\n");
                        break;
                case P2P_INVITE_RSP:
                        nvt_dbg(P2P, "P2P INVITE RSP Frame\n");
                        break;
                case P2P_DEVICE_DISC_REQ:
                        nvt_dbg(P2P, "P2P DEVICE DISC REQ Frame\n");
                        break;
                case P2P_DEVICE_DISC_RSP:
                        nvt_dbg(P2P, "P2P DEVICE DISC RSP Frame\n");
                        break;
                case P2P_PROV_DISC_REQ:
                        nvt_dbg(P2P, "P2P PRIV DISC REQ Frame\n");
                        break;
                case P2P_PROV_DISC_RSP:
                        nvt_dbg(P2P, "P2P PRIV DISC RSP Frame\n");
                        break;
                default:
                        nvt_dbg(P2P, "Unknown P2P Public Action Frame\n");
                        break;
                }
        } else if (nvt_is_p2p_gas_public_action(data, msg_len)) {
                p2p_gas_pub_act = (struct _nvt_p2p_gas_public_action *)data;
                switch (p2p_gas_pub_act->action) {
                case P2P_GAS_INITIAL_REQ:
                        nvt_dbg(P2P, "P2P GAS INITIAL REQ\n");
                        break;
                case P2P_GAS_INITIAL_RSP:
                        nvt_dbg(P2P, "P2P GAS INITIAL RSP\n");
                        break;
                case P2P_GAS_COMEBACK_REQ:
                        nvt_dbg(P2P, "P2P GAS COMEBACK REQ\n");
                        break;
                case P2P_GAS_COMEBACK_RSP:
                        nvt_dbg(P2P, "P2P GAS COMEBACK RSP\n");
                        break;
                default:
                        nvt_dbg(P2P, "Unknown P2P GAS Frame\n");
                        break;
                }
        } else if (nvt_is_p2p_action(data, msg_len)) {
                p2p_act = (struct _nvt_p2p_action *)data;
                switch (p2p_act->subtype) {
                case P2P_NOA:
                        nvt_dbg(P2P, "P2P NOA Frame\n");
                        break;
                case P2P_PRESENCE_REQ:
                        nvt_dbg(P2P, "P2P PRESENCE REQ Frame\n");
                        break;
                case P2P_PRESENCE_RSP:
                        nvt_dbg(P2P, "P2P PRESENCE RSP Frame\n");
                        break;
                case P2P_GO_DISC_REQ:
                        nvt_dbg(P2P, "P2P GO DISC REQ Frame\n");
                        break;
                default:
                        nvt_dbg(P2P, "Unknown P2P Action Frame\n");
                }
        }
}

static s32 nvt_p2p_public_action_handle(struct _nvt_adapter *nvt_adapter,
        const void *data)
{
        struct _nvt_p2p_public_action *p2p_pub_act;
        struct _nvt_bus *nvt_bus = nvt_adapter->nvt_bus;
        s32 ret = 0;
        u16 wait;

        nvt_dbg(P2P, "%s\n", __func__);

        p2p_pub_act = (struct _nvt_p2p_public_action *)(data);

        switch (p2p_pub_act->subtype) {
        case P2P_GO_NEGO_REQ:
        case P2P_GO_NEGO_RSP:
                nvt_dbg(P2P, "handle GO NEGO REQ/RSP frame\n");
                wait = P2P_NEGO_DWELL_TIME;
                ret = nvt_icfg_lock(nvt_bus);
                if (ret < 0) {
                        goto err_ret;
                }
                ret = nvt_icfg_reset(nvt_bus);
                if (ret < 0) {
                        goto err_ret;
                }
                ret = nvt_icfg_add(nvt_bus, NVT_ICFG_SET,
                        WID_P2P_LISTEN_EXTEND, (u8 *)&wait, 1);
                if (ret < 0) {
                        goto err_ret;
                }
                ret = nvt_icfg_send_and_get_resp(nvt_bus, NULL, 0);
                if (ret < 0) {
                        goto err_ret;
                }
                nvt_icfg_unlock(nvt_bus);
                break;
        default:
                nvt_dbg(P2P, "p2p pub act frame subtype: %d, do nothing\n",
                        p2p_pub_act->subtype);
        }
        return 0;
err_ret:
        nvt_icfg_unlock(nvt_bus);
        nvt_dbg(P2P, "WID_P2P_LISTEN_EXTEND has problems\n");
        return ret;
}

static bool nvt_send_action_frame(struct _nvt_cfg80211 *nvt_cfg,
        struct _nvt_if *nvt_if, u16 channel_nr, unsigned int wait,
        const u8 *buf, u8 *wid_frame_buf, size_t len)
{
        struct _nvt_p2p *nvt_p2p = &nvt_cfg->nvt_p2p;
        struct _nvt_adapter *nvt_adapter = nvt_if->nvt_adapter;
        struct _nvt_bus *nvt_bus = nvt_adapter->nvt_bus;
        s32 ie_offset;
        s32 ie_len;
        u8 category;
        u8 action;
        s32 ret = 0;

        nvt_dbg(P2P, "%s\n", __func__);

        /* Add the length exepted for 802.11 header  */
        ie_offset = IEEE80211_MGMT_HDR_LEN;
        ie_len = cpu_to_le16(len - ie_offset);
        category = buf[ie_offset];
        action = buf[ie_offset + 1];

        nvt_dbg_p2p(true, &buf[ie_offset], ie_len);

        if (nvt_is_p2p_public_action(&buf[ie_offset], ie_len)) {
                /* Process p2p public action frame */
                if (nvt_p2p_public_action_handle(nvt_adapter,
                       &buf[ie_offset])) {
                        nvt_dbg(P2P, "handle p2p public action error!!\n");
                }
        } else if (nvt_is_p2p_gas_public_action(&buf[ie_offset], ie_len)) {
                /* Process p2p GAS frame */
                /* Do nothing */
        } else if (nvt_is_p2p_action(&buf[ie_offset], ie_len)) {
                /* Process p2p action frame */
                /* Do nothing */
        } else {
                nvt_dbg(P2P, "Unknown Frame: category 0x%x, action 0x%x\n",
                        category, action);
                return false;
        }

        if (test_bit(NVT_P2P_ROC_MODE, &nvt_p2p->status)) {
                nvt_dbg(P2P, "FW is ROC right now\n");
                nvt_dbg(P2P, "Not support off-channel TX\n");
                if (nvt_p2p->listen_channel == (u8)channel_nr) {
                        nvt_dbg(P2P, "Channel match ROC channel\n");
                        if (wait > 0) {
                                nvt_dbg(P2P, "wait > 0 is weird!!\n");
                                ret = nvt_icfg_lock(nvt_bus);
                                if (ret < 0) {
                                        goto err_ret;
                                }
                                ret = nvt_icfg_reset(nvt_bus);
                                if (ret < 0) {
                                        goto err_ret;
                                }
                                ret = nvt_icfg_add(nvt_bus, NVT_ICFG_SET,
                                        WID_P2P_LISTEN_EXTEND, (u8 *)&wait, 1);
                                if (ret < 0) {
                                        goto err_ret;
                                }
                                ret = nvt_icfg_send_and_get_resp(nvt_bus,
                                        NULL, 0);
                                if (ret < 0) {
                                        goto err_ret;
                                }
                                nvt_icfg_unlock(nvt_bus);
                        } else {
                                nvt_dbg(P2P, "wait == 0 is correct\n");
                        }
                } else {
                        nvt_dbg(P2P, "Channel mismatch ROC channel\n");
                        nvt_dbg(P2P, "Send it on specified channel\n");
                }
        } else {
                nvt_dbg(P2P, "Not ROC now,send it on specified channel\n");
        }

        wid_frame_buf[0] = channel_nr;
        memcpy(&wid_frame_buf[1], buf, len);
        ret = nvt_icfg_lock(nvt_bus);
        if (ret < 0) {
                goto err_ret;
        }
        ret = nvt_icfg_reset(nvt_bus);
        if (ret < 0) {
                goto err_ret;
        }
        ret = nvt_icfg_add(nvt_bus, NVT_ICFG_SET,
                WID_MGMT_TX, (u8 *)wid_frame_buf, len + 1);
        if (ret < 0) {
                goto err_ret;
        }
        ret = nvt_icfg_send_and_get_resp(nvt_bus, NULL, 0);
        if (ret < 0) {
                goto err_ret;
        }
        nvt_icfg_unlock(nvt_bus);
        return true;
err_ret:
        nvt_icfg_unlock(nvt_bus);
        nvt_dbg(P2P, "WID_MGMT_TX(action frame) has problems\n");
        return false;
}

static s32 nvt_cfg80211_mgmt_tx(struct wiphy *wiphy,
#if LINUX_VERSION_CODE >= KERNEL_VERSION(3, 6, 0)
        struct wireless_dev *wdev,
#else
        struct net_device *ndev,
#endif

#if LINUX_VERSION_CODE < KERNEL_VERSION(3, 14, 0)
        struct ieee80211_channel *chan,
        bool offchan,
#if LINUX_VERSION_CODE <= KERNEL_VERSION(3, 7, 0)
        enum nl80211_channel_type channel_type,
        bool channel_type_valid,
#endif
        unsigned int wait, const u8 *buf, size_t len, bool no_cck,
        bool dont_wait_for_ack,
#else
        struct cfg80211_mgmt_tx_params *params,
#endif /* KERNEL_vERSION (3, 14, 0) */
        u64 *cookie)
{
        struct _nvt_if *nvt_if;
        const struct ieee80211_mgmt *mgmt;
        struct _nvt_cfg80211 *nvt_cfg = wiphy_priv(wiphy);
        struct _nvt_p2p *nvt_p2p = &nvt_cfg->nvt_p2p;
        u8 *wid_frame_buf = NULL;
        u16 channel_nr = 0;
        u32 freq = 0;
        bool ack;
#if LINUX_VERSION_CODE >= KERNEL_VERSION(3, 14, 0)
        struct ieee80211_channel *chan = params->chan;
        bool offchan = params->offchan;
        unsigned int wait = params->wait;
        size_t len = params->len;
        bool no_cck = params->no_cck;
        bool dont_wait_for_ack = params->dont_wait_for_ack;
        const u8 *buf = params->buf;
#endif

        nvt_dbg(P2P, "%s\n", __func__);
        nvt_dbg(P2P, "offchan = %d, wait = %d, no_cck = %d\n",
                offchan, wait, no_cck);
        nvt_dbg(P2P, "len = %zu, dont_wait_for_ack = %d\n",
                len, dont_wait_for_ack);

        if (chan) {
                freq = chan->center_freq;
                channel_nr = ieee80211_frequency_to_channel(chan->center_freq);
                /* never send freq zero to the firmware */
                if (WARN_ON(freq == 0)) {
                        return -EINVAL;
                }
        } else {
                nvt_dbg(P2P, "Chan is NULL !!!!, return error\n");
                return -EINVAL;
        }

        nvt_dbg(P2P, "freq = %d, channel_nr = %d\n", freq, channel_nr);
        nvt_dbg(P2P, "p2p->status = %lx, p2p->listen_channel = %d\n",
                nvt_p2p->status, nvt_p2p->listen_channel);

        *cookie = 0;

        mgmt = (const struct ieee80211_mgmt *)buf;
        if (!ieee80211_is_mgmt(mgmt->frame_control)) {
                nvt_dbg(CFG80211, "Driver only allows mgmt packet\n");
                return -EPERM;
        }
#if LINUX_VERSION_CODE >= KERNEL_VERSION(3, 6, 0)
        nvt_if = container_of(wdev, struct _nvt_if, wdev);
#else
        nvt_if = netdev_priv(ndev);
#endif

        wid_frame_buf = kmalloc(len + 1, GFP_KERNEL);
        if (!wid_frame_buf) {
                return -ENOMEM;
        }

        if (ieee80211_is_probe_resp(mgmt->frame_control)) {
                nvt_dbg(P2P, "Send Probe Response\n");
                nvt_send_probe_resp_frame(nvt_cfg, nvt_if, channel_nr,
                        wait, buf, wid_frame_buf, len);
#if LINUX_VERSION_CODE >= KERNEL_VERSION(3, 6, 0)
                cfg80211_mgmt_tx_status(wdev, *cookie, buf,
                        len, true, GFP_KERNEL);
#else
                cfg80211_mgmt_tx_status(ndev, *cookie, buf,
                        len, true, GFP_KERNEL);
#endif
        } else if (ieee80211_is_action(mgmt->frame_control)) {
                ack = nvt_send_action_frame(nvt_cfg, nvt_if, channel_nr,
                        wait, buf, wid_frame_buf, len);
#if LINUX_VERSION_CODE >= KERNEL_VERSION(3, 6, 0)
                cfg80211_mgmt_tx_status(wdev, *cookie, buf,
                        len, ack, GFP_KERNEL);
#else
                cfg80211_mgmt_tx_status(ndev, *cookie, buf,
                        len, ack, GFP_KERNEL);
#endif
        } else {
                nvt_dbg(P2P, "Unhandled, fc=%04x!\n", mgmt->frame_control);
        }
        kfree(wid_frame_buf);
        return 0;
}

static s32 nvt_do_roc(struct _nvt_if *nvt_if, u16 channel, u16 duration)
{
        struct _nvt_adapter *nvt_adapter = nvt_if->nvt_adapter;
        struct _nvt_probe_ssid ssid_list[MAX_PROBED_SSIDS];
        struct _nvt_wlan_conf *wconf = &nvt_if->nvt_wconf;
        u8 list_len = 0;
        u8 scan_type = PASSIVE_SCAN;
        u8 scan_filter = SCAN_NO_ADHOC | SCAN_SAVE_WITHOUTRSSI | SCAN_CURRENTCH;
        u8 *channels_param = NULL;
        u8 site_survey = 1;
        u8 user_scan = 1;
        u8 p2p_enable = 1;
        u8 p2p_listen_mode = 1;
        s32 ret = 0;

        nvt_dbg(P2P, "%s\n", __func__);
        memset(ssid_list, 0x0,
                sizeof(struct _nvt_probe_ssid) * MAX_PROBED_SSIDS);

        nvt_dbg(P2P, "wconf->ctrl_flag=0x%x, channel=%d\n",
                wconf->ctrl_flag, channel);
        nvt_dbg(P2P, "listen_duration=%d ms\n", duration);
        ret = nvt_icfg_lock(nvt_adapter->nvt_bus);
        if (ret < 0) {
                goto err_ret;
        }
        ret = nvt_icfg_reset(nvt_adapter->nvt_bus);
        if (ret < 0) {
                goto err_ret;
        }
        /* Enable P2P Listen Mode */
        ret = nvt_icfg_add(nvt_adapter->nvt_bus, NVT_ICFG_SET,
                WID_P2P_ENABLE, (u8 *)&p2p_enable, 1);
        if (ret < 0) {
                goto err_ret;
        }
        ret = nvt_icfg_add(nvt_adapter->nvt_bus, NVT_ICFG_SET,
                WID_P2P_LISTEN_MODE, (u8 *)&p2p_listen_mode, 1);
        if (ret < 0) {
                goto err_ret;
        }
        /* Enable Passive Scan to Firmware */
        ret = nvt_icfg_add(nvt_adapter->nvt_bus, NVT_ICFG_SET,
                WID_SCAN_TYPE, (u8 *)&scan_type, 1);
        if (ret < 0) {
                goto err_ret;
        }
        /* Set Scan Filter value */
        ret = nvt_icfg_add(nvt_adapter->nvt_bus, NVT_ICFG_SET,
                WID_SCAN_FILTER, (u8 *)&scan_filter, 1);
        if (ret < 0) {
                goto err_ret;
        }
        /* Set Scan Time value */
        ret = nvt_icfg_add(nvt_adapter->nvt_bus, NVT_ICFG_SET,
                WID_PASSIVE_SCAN_TIME, (u8 *)&duration, 2);
        if (ret < 0) {
                goto err_ret;
        }
        /* Set Scan Channel*/
        channels_param = kzalloc(3, GFP_KERNEL);
        if (channels_param == NULL) {
                nvt_dbg(P2P, "Fail memory allocate!\n");
                ret = -ENOMEM;
                goto err_ret;
        } else {
                channels_param[0] = 1;
                channels_param[1] = IEEE80211_BAND_2GHZ; /* 2G Band */
                channels_param[2] = channel; /* Channel number */
                ret = nvt_icfg_add(nvt_adapter->nvt_bus, NVT_ICFG_SET,
                        WID_SCAN_MULTICH, (u8 *)channels_param, 3);
                if (ret < 0) {
                        kfree(channels_param);
                        goto err_ret;
                }
                kfree(channels_param);
        }
        ret = nvt_icfg_add(nvt_adapter->nvt_bus, NVT_ICFG_SET,
                WID_SITE_SURVEY, (u8 *)&site_survey, 1);
        if (ret < 0) {
                goto err_ret;
        }
        /* Set SSID List */
        ssid_list[0].index = 0;
        ssid_list[0].ssid_len = 0;
        list_len += sizeof(struct _nvt_probe_ssid);
        ret = nvt_icfg_add(nvt_adapter->nvt_bus, NVT_ICFG_SET,
                WID_SCAN_MULTISSID, (u8 *)ssid_list, list_len);
        if (ret < 0) {
                goto err_ret;
        }
        /* Set User Scan */
        ret = nvt_icfg_add(nvt_adapter->nvt_bus, NVT_ICFG_SET,
                WID_START_SCAN_REQ, (u8 *)&user_scan, 1);
        if (ret < 0) {
                goto err_ret;
        }
        ret = nvt_icfg_send_and_get_resp(nvt_adapter->nvt_bus, NULL, 0);
        if (ret < 0) {
                goto err_ret;
        }
        nvt_icfg_unlock(nvt_adapter->nvt_bus);

        return ret;
err_ret:
        nvt_icfg_unlock(nvt_adapter->nvt_bus);
        nvt_dbg(P2P, "WID(START ROC) has problems\n");
        return ret;
}

static s32 nvt_do_cancel_roc(struct _nvt_if *nvt_if)
{
        struct _nvt_adapter *nvt_adapter = nvt_if->nvt_adapter;
        struct _nvt_wlan_conf *wconf = &nvt_if->nvt_wconf;
        u8 p2p_enable = 1;
        u8 p2p_listen_mode = 0;
        s32 ret = 0;

        nvt_dbg(P2P, "%s\n", __func__);
        nvt_dbg(P2P, "wconf->ctrl_flag=0x%x\n", wconf->ctrl_flag);
        ret = nvt_icfg_lock(nvt_adapter->nvt_bus);
        if (ret < 0) {
                goto err_ret;
        }
        ret = nvt_icfg_reset(nvt_adapter->nvt_bus);
        if (ret < 0) {
                goto err_ret;
        }
        /* Disable P2P Listen Mode */
        ret = nvt_icfg_add(nvt_adapter->nvt_bus, NVT_ICFG_SET,
                WID_P2P_ENABLE, (u8 *)&p2p_enable, 1);
        if (ret < 0) {
                goto err_ret;
        }
        ret = nvt_icfg_add(nvt_adapter->nvt_bus, NVT_ICFG_SET,
                WID_P2P_LISTEN_MODE, (u8 *)&p2p_listen_mode, 1);
        if (ret < 0) {
                goto err_ret;
        }
        ret = nvt_icfg_send_and_get_resp(nvt_adapter->nvt_bus, NULL, 0);
        if (ret < 0) {
                goto err_ret;
        }
        nvt_icfg_unlock(nvt_adapter->nvt_bus);
        return ret;
err_ret:
        nvt_icfg_unlock(nvt_adapter->nvt_bus);
        nvt_dbg(CFG80211, "WID(CANCEL ROC) has problems\n");
        return ret;
}

/**
 * nvt_p2p_roc_done - handle ROC done(listen done)
 *
 * @nvt_if: nvt network interface
 * @msg_len: event message length
 * @data: event data
 * @direction: true - Firmware send ROC done to Driver
 *             false - WPA_Supplicant send ROC done to Driver
 *
 * Return: zero - success.
 *         negative - failure.
 */
s32 nvt_p2p_roc_done(struct _nvt_if *nvt_if,
        u16 msg_len, u8 *data, bool direction)
{
        struct _nvt_cfg80211 *nvt_cfg = nvt_if->nvt_adapter->nvt_cfg80211;
        struct _nvt_p2p *nvt_p2p = &nvt_cfg->nvt_p2p;

        nvt_dbg(P2P, "%s is recieved from %s\n",
                __func__, (direction) ? "WPA_SUPPLICANT" : "FW");

        nvt_dbg(P2P, "p2p->status=%lx\n", nvt_p2p->status);

        if (test_and_clear_bit(NVT_P2P_ROC_MODE, &nvt_p2p->status)) {
#if LINUX_VERSION_CODE >= KERNEL_VERSION(3, 6, 0)
                cfg80211_remain_on_channel_expired(&nvt_if->wdev,
                        nvt_p2p->roc_cookie, &nvt_p2p->remain_on_channel,
                        GFP_KERNEL);
#else
                cfg80211_remain_on_channel_expired(nvt_if->ndev,
                        nvt_p2p->roc_cookie, &nvt_p2p->remain_on_channel,
                        NL80211_CHAN_NO_HT, GFP_KERNEL);
#endif
        }
        return 0;
}

static s32 nvt_p2p_roc_handle(struct _nvt_if *nvt_if,
        struct _nvt_p2p *nvt_p2p, u16 channel, u32 duration, u8 action)
{
        s32 ret = 0;

        nvt_dbg(P2P, "%s, p2p->status=%lx\n", __func__, nvt_p2p->status);

        switch (action) {
        case NVT_P2P_START_ROC:
                if (test_bit(NVT_P2P_ROC_MODE, &nvt_p2p->status)) {
                        nvt_dbg(P2P, "Previous ROC is not finish yet\n");
                        goto exit;
                }
                nvt_p2p->listen_channel = (u8)channel;
                nvt_dbg(P2P, "listen_channel=%d\n",
                        nvt_p2p->listen_channel);
                ret = nvt_do_roc(nvt_if, channel, (u16)duration);
                if (!ret) {
                        set_bit(NVT_P2P_ROC_MODE, &nvt_p2p->status);
                        nvt_p2p->roc_cookie++;
                } else {
                        goto exit;
                }
                nvt_dbg(P2P, "ROC_cookie=%d\n", nvt_p2p->roc_cookie);
                break;
        case NVT_P2P_CANCEL_ROC:
                ret = nvt_do_cancel_roc(nvt_if);
                if (!ret) {
                        nvt_p2p_roc_done(nvt_if, 0, NULL, true);
                } else {
                        goto exit;
                }
                break;
        default:
                break;
        }
exit:
        return ret;
}
#if LINUX_VERSION_CODE >= KERNEL_VERSION(3, 6, 0)
static s32 nvt_cfg80211_remain_on_channel(struct wiphy *wiphy,
        struct wireless_dev *wdev, struct ieee80211_channel *channel,
        unsigned int duration, u64 *cookie)
{
        struct _nvt_if *nvt_if = container_of(wdev, struct _nvt_if, wdev);
#else
static s32 nvt_cfg80211_remain_on_channel(struct wiphy *wiphy,
        struct net_device *ndev,
        struct ieee80211_channel *channel,
        enum nl80211_channel_type channel_type,
        unsigned int duration, u64 *cookie)
{
        struct _nvt_if *nvt_if = netdev_priv(ndev);
#endif
        struct _nvt_cfg80211 *nvt_cfg = wiphy_priv(wiphy);
        struct _nvt_p2p *nvt_p2p = &nvt_cfg->nvt_p2p;
        s32 ret = 0;
        u16 channel_nr;

        nvt_dbg(P2P, "%s\n", __func__);

        channel_nr = ieee80211_frequency_to_channel(channel->center_freq);

        nvt_dbg(P2P, "channel->center_freq= %d, channel_nr = %d\n",
                channel->center_freq, channel_nr);
        nvt_dbg(P2P, "duration=%d\n", duration);

        ret = nvt_p2p_roc_handle(nvt_if, nvt_p2p,
                channel_nr, duration, NVT_P2P_START_ROC);
        if (ret) {
                goto exit;
        }
        memcpy(&nvt_p2p->remain_on_channel, channel, sizeof(*channel));
        *cookie = nvt_p2p->roc_cookie;
#if LINUX_VERSION_CODE >= KERNEL_VERSION(3, 6, 0)
        cfg80211_ready_on_channel(wdev, *cookie, channel, duration, GFP_KERNEL);
#else
        cfg80211_ready_on_channel(ndev, *cookie, channel, channel_type,
            duration, GFP_KERNEL);
#endif

exit:
        return ret;
}

#if LINUX_VERSION_CODE >= KERNEL_VERSION(3, 6, 0)
static s32
nvt_cfg80211_cancel_remain_on_channel(struct wiphy *wiphy,
        struct wireless_dev *wdev, u64 cookie)
{
        struct _nvt_if *nvt_if = container_of(wdev, struct _nvt_if, wdev);
#else
static s32
nvt_cfg80211_cancel_remain_on_channel(struct wiphy *wiphy,
        struct net_device *ndev, u64 cookie)
{
        struct _nvt_if *nvt_if = netdev_priv(ndev);
#endif
        struct _nvt_cfg80211 *nvt_cfg = wiphy_priv(wiphy);
        struct _nvt_p2p *nvt_p2p = &nvt_cfg->nvt_p2p;
        s32 ret = 0;

        nvt_dbg(P2P, "%s\n", __func__);

        ret = nvt_p2p_roc_handle(nvt_if, nvt_p2p, 0, 0, NVT_P2P_CANCEL_ROC);

        return ret;
}
static int nvt_cfg80211_set_txpower(struct wiphy *wiphy,
#if LINUX_VERSION_CODE >= KERNEL_VERSION(3, 6, 0)
        struct wireless_dev *wdev,
#endif
        enum nl80211_tx_power_setting type,
        int mbm)
{
        struct _nvt_cfg80211 *nvt_cfg = wiphy_priv(wiphy);
        struct _nvt_cfg80211 **cfg_2ptr = &nvt_cfg;
        struct _nvt_adapter *nvt_adapter = container_of(cfg_2ptr,
                        struct _nvt_adapter, nvt_cfg80211);
        struct _nvt_adapter **adapter_2ptr = &nvt_adapter;
        struct _nvt_if *nvt_if = container_of(adapter_2ptr,
                        struct _nvt_if, nvt_adapter);
        int dbm = MBM_TO_DBM(mbm);

        if (!check_if_ready(nvt_if))
                return -EIO;

        switch (type) {
        case NL80211_TX_POWER_AUTOMATIC:
                return 0;
        case NL80211_TX_POWER_LIMITED:
                nvt_cfg->tx_pwr = dbm;
                return 0;
        case NL80211_TX_POWER_FIXED:
                nvt_cfg->tx_pwr = dbm;
                return 0;
        default:
                return -EOPNOTSUPP;
        }
        //TODO
        //need to know what WID needed to send to the firmware
        return 0;
}


static int nvt_cfg80211_set_bitrate(struct wiphy *wiphy,
                        struct net_device *dev,
                        const u8 *addr,
                        const struct cfg80211_bitrate_mask *mask)
{
        struct _nvt_if *nvt_if = netdev_priv(dev);
        //TODO replace return 0 with the WID commands to the firmware
        struct _nvt_bus *nvt_bus = nvt_if->nvt_adapter->nvt_bus;
        u8 wid_msg[4] = {0};
        s32 ret = 0;

        //MCS32
        wid_msg[0] = 00;
#if LINUX_VERSION_CODE >= KERNEL_VERSION(3, 14, 0)
        //MCS support
        wid_msg[1] = mask->control[0].ht_mcs[4];
        //fill the mcs
        wid_msg[2] = mask->control[0].ht_mcs[0];
        wid_msg[3] = mask->control[0].ht_mcs[1];
#elif LINUX_VERSION_CODE >= KERNEL_VERSION(3, 4, 0)
        //MCS support
        wid_msg[1] = mask->control[0].mcs[4];
        //fill the mcs
        wid_msg[2] = mask->control[0].mcs[0];
        wid_msg[3] = mask->control[0].mcs[1];
#endif
        ret = nvt_icfg_lock(nvt_bus);
        if (ret < 0) {
                goto err_ret;
        }
        ret = nvt_icfg_reset(nvt_bus);
        if (ret < 0) {
                goto err_ret;
        }
        ret = nvt_icfg_add(nvt_bus, NVT_ICFG_SET, WID_SET_RXMCS, wid_msg, 4);
        if (ret < 0) {
                goto err_ret;
        }
        ret = nvt_icfg_send_and_get_resp(nvt_bus, NULL, 0);
        if (ret < 0) {
                goto err_ret;
        }
err_ret:
        nvt_icfg_unlock(nvt_bus);
        return 0;
}

static int nvt_get_stat_cmd(struct wiphy *wiphy, struct _nvt_if *nvt_if,
                         struct station_info *sinfo, u8 mode)
{

        struct _nvt_bus *nvt_bus = nvt_if->nvt_adapter->nvt_bus;
        s32 retval = 0;
        u8 wid_val_pos = 0;
        u32 buff_size = 512;
        s32 wid_rate_val = 0;
        u8 sgi;
        u8 *resp = kzalloc(sizeof(u8)*512, GFP_KERNEL);
        if (resp == NULL) {
                nvt_dbg(ERROR, "%s[0] kzalloc for diag_req is failed\n",
                        __func__);
                retval = -1;
                goto alloc_failed;
        }

        //the statistic we need for the iw dev link/station dump
        nvt_get_wid_value(nvt_bus, WID_DTIM_PERIOD, resp,
                        buff_size, &wid_val_pos);
#if LINUX_VERSION_CODE > KERNEL_VERSION(4, 0, 0)
        sinfo->filled |= BIT(NL80211_STA_INFO_BSS_PARAM);
#else
        sinfo->filled |= STATION_INFO_BSS_PARAM;
#endif
        sinfo->bss_param.dtim_period = resp[wid_val_pos];
        nvt_get_wid_value(nvt_bus, WID_BEACON_INTERVAL,
                        resp, buff_size, &wid_val_pos);
        sinfo->bss_param.beacon_interval = resp[wid_val_pos];

        if (mode == NVT_STA_MODE) {
#if LINUX_VERSION_CODE > KERNEL_VERSION(4, 0, 0)
                sinfo->filled |= BIT(NL80211_STA_INFO_RX_PACKETS);
                sinfo->filled |= BIT(NL80211_STA_INFO_RX_BYTES);
                sinfo->filled |= BIT(NL80211_STA_INFO_TX_PACKETS);
                sinfo->filled |= BIT(NL80211_STA_INFO_TX_BYTES);
                sinfo->filled |= BIT(NL80211_STA_INFO_SIGNAL);
                //tx-rate
                sinfo->filled |= BIT(NL80211_STA_INFO_TX_BITRATE);
#else
                sinfo->filled |= STATION_INFO_RX_PACKETS;
                sinfo->filled |= STATION_INFO_RX_BYTES;
                sinfo->filled |= STATION_INFO_TX_PACKETS;
                sinfo->filled |= STATION_INFO_TX_BYTES;
                sinfo->filled |= STATION_INFO_SIGNAL;
                //tx-rate
                sinfo->filled |= STATION_INFO_TX_BITRATE;
#endif
                sinfo->rx_packets = nvt_if->net_stats.rx_packets;
                sinfo->rx_bytes = nvt_if->net_stats.rx_bytes;

                sinfo->tx_packets = nvt_if->net_stats.tx_packets;
                sinfo->tx_bytes = nvt_if->net_stats.tx_bytes;

                nvt_get_wid_value(nvt_bus, WID_RSSI, resp,
                                buff_size, &wid_val_pos);
                sinfo->signal = -1*resp[wid_val_pos];

                nvt_get_wid_value(nvt_bus, WID_RATE_0, resp,
                                buff_size, &wid_val_pos);
                /*check if the b7 of rate 0 */
                if (resp[wid_val_pos] & 0x0080) {
                        nvt_if->ht_type = 1;
                } else {
                        nvt_if->ht_type = 0;
                }
                wid_rate_val = nvt_parse_rate_0(resp[wid_val_pos]);
                nvt_get_wid_value(nvt_bus, WID_11N_SHORT_GI_ENABLE, resp,
                        buff_size, &wid_val_pos);
                sgi = resp[wid_val_pos];

                if (nvt_if->ht_type == 0) {
                        sinfo->txrate.legacy = wid_rate_val / 100;
                } else if (!sgi && nvt_if->ht_type == 1) {
                        sinfo->txrate.mcs = wid_rate_val;
                        sinfo->txrate.flags |= RATE_INFO_FLAGS_MCS;
                } else if (sgi && nvt_if->ht_type == 1) {
                        sinfo->txrate.mcs = wid_rate_val;
                        sinfo->txrate.flags |= RATE_INFO_FLAGS_SHORT_GI;
                        sinfo->txrate.flags |= RATE_INFO_FLAGS_MCS;
                } else {
                        nvt_dbg(ERROR, "%s[0] get rate info is error!\n",
                        __func__);
                }
                //bss_flags
                sinfo->bss_param.flags = 0;
                nvt_get_wid_value(nvt_bus, WID_SHORT_SLOT_ALLOWED, resp,
                                buff_size, &wid_val_pos);

                if (resp[wid_val_pos])
                        sinfo->bss_param.flags |=
                                BSS_PARAM_FLAGS_SHORT_SLOT_TIME;
        }
alloc_failed:
        kfree(resp);
        return retval;
}

#if LINUX_VERSION_CODE >= KERNEL_VERSION(3, 16, 0)
static int nvt_get_station(struct wiphy *wiphy, struct net_device *dev,
                        const u8 *mac, struct station_info *sinfo)
#else
static int nvt_get_station(struct wiphy *wiphy, struct net_device *dev,
                           u8 *mac, struct station_info *sinfo)
#endif
{

        struct _nvt_if *nvt_if = netdev_priv(dev);
        memcpy((void *)mac, nvt_if->nvt_wconf.bssid, ETH_ALEN);
        if (!is_sta_connected(nvt_if)) {
                return -ENOENT;
        }
        return nvt_get_stat_cmd(wiphy, nvt_if, sinfo, nvt_if->mode);
}

/*NOTE: NL80211 counts idx from zero*/
static int nvt_dump_station(struct wiphy *wiphy, struct net_device *dev,
                  int idx, u8 *mac, struct station_info *sinfo)
{
        struct _nvt_if *nvt_if = netdev_priv(dev);
        struct _nvt_bus *nvt_bus = nvt_if->nvt_adapter->nvt_bus;
        u8 retval = 0;
        u8 wid_val_pos = 0;
        s32 num_of_sta;
        u8 *buff = kzalloc(sizeof(u8)*512, GFP_KERNEL);

        if (buff == NULL) {
                nvt_dbg(ERROR, "%s[0] kzalloc for diag_req is failed\n",
                        __func__);
                retval = -1;
                goto alloc_failed;
        }

        /**station mode**/
        if (nvt_if->mode == NVT_STA_MODE) {
                if (idx || !is_sta_connected(nvt_if)) {
                        retval = -ENOENT;
                        goto alloc_failed;
                }
                memcpy(mac, nvt_if->nvt_wconf.bssid, ETH_ALEN);
                kfree(buff);
                return nvt_get_stat_cmd(wiphy, nvt_if, sinfo, nvt_if->mode);
        /**In AP mode**/
        } else if (nvt_if->mode == NVT_AP_MODE) {
                nvt_get_wid_value(nvt_bus, WID_CONNECTED_STA_LIST,
                                buff, 512, &wid_val_pos);
                /*wid_val_pos-2 is the length position*/
                num_of_sta = get_num_of_conn_sta(&buff[wid_val_pos]
                                , buff[wid_val_pos-2]);
                /*If no sta connected to ap => quit*/
                if (num_of_sta == 0) {
                        retval = -ENOENT;
                        goto alloc_failed;
                }
                /*Loop all the sta connecting to the AP*/
                if (idx != num_of_sta) {
                        memcpy(mac, &buff[idx * 16 + wid_val_pos + 1], 6);
                        kfree(buff);
                        return nvt_get_stat_cmd(wiphy,
                                        nvt_if, sinfo, nvt_if->mode);
                } else {
                        retval = -ENOENT;
                        goto alloc_failed;
                }
        } else {
                retval = -ENOENT;
                goto alloc_failed;
        }

alloc_failed:
        kfree(buff);
        return retval;
}

#ifdef CONFIG_PM
//Each pattern register can only support 31 bytes
static u8 nvt_get_num_of_pat_regs(struct cfg80211_wowlan *wow,
            u8 pat_cnt)
{
        u32 size_of_pattern = wow->patterns[pat_cnt].pattern_len;
        u8 num_of_regs = 1;
        while (size_of_pattern > WOW_MAXPATTERNLEN) {
                size_of_pattern -= 31;
                num_of_regs++;
        }
        return num_of_regs;
}

//AT sanity check for the patterns
static s32 nvt_is_pats_valid(struct cfg80211_wowlan *wow)
{

        u8 cnt;
        s32 ret = 0;

        if (wow->n_patterns > 4) {
                ret = -1;
        } else if (wow->n_patterns > 2) {
                //AT if pattern# equals 3 or 4 then each must smaller than
                //WOW_MAXPATTERNLEN
                for (cnt = 0; cnt < wow->n_patterns; cnt++) {
                        if (wow->patterns[cnt].pattern_len >
                            WOW_MAXPATTERNLEN) {
                                ret = -1;
                        }
                }
        } else if (wow->n_patterns == 2) {
                //if only 2 patterns check each smaller than
                //WOW_MAXPATTERNLEN * 2
                for (cnt = 0; cnt < wow->n_patterns; cnt++) {
                        if (wow->patterns[cnt].pattern_len >
                                WOW_MAXPATTERNLEN * 2) {
                                ret = -1;
                        }
                }
        } else {
                //only one pattern
                if (wow->patterns[0].pattern_len >
                        4 * WOW_MAXPATTERNLEN) {
                        ret = -1;
                }
        }

        return ret;
}

static void nvt_clean_unused_wow_mask(u32 *reg_mask,
            u8 mask_len)
{
        u8 i;
        for (i = mask_len; i < WOW_MAXPATTERNLEN; i++) {
                (*reg_mask) &= ~(1 << i);
        }
}

//AT get the mask from register index
static void nvt_set_mask_by_reg_index(struct cfg80211_wowlan *wow,
            u8 *reg_idx, struct wow_pattern *buf, u8 pat_cnt)
{
        u32 *start_mask = (u32 *)(wow->patterns[pat_cnt].mask);
        u8 pattern_len = wow->patterns[pat_cnt].pattern_len;
        u8 mask_len = (pattern_len + 7) / 8;
        u8 reg_mask_len = 0;
        u32 reg_mask = 0;

        if (wow->n_patterns > 2) {
                nvt_clean_unused_wow_mask(start_mask, pattern_len);
                memcpy((u32 *)&buf[*reg_idx].mask, start_mask,
                            mask_len);
        } else {
                if (wow->n_patterns == 2) {
                        if (*reg_idx == 0 || *reg_idx == 2) {
                                if (pattern_len > WOW_MAXPATTERNLEN) {
                                        reg_mask_len = WOW_MAXPATTERNLEN;
                                } else {
                                        reg_mask_len = pattern_len;
                                }
                                nvt_clean_unused_wow_mask(start_mask,
                                        reg_mask_len);

                                //set to mask len per byte
                                reg_mask_len = (reg_mask_len + 7) / 8;
                                memcpy((u32 *)&buf[*reg_idx].mask,
                                    start_mask, reg_mask_len);
                        } else {
                                reg_mask = *(start_mask + 1);
                                reg_mask <<= 1;
                                reg_mask &= ~(1 << 0);
                                //reg_mask now: BIT31:BIT62
                                reg_mask |= (((*start_mask) & (1 << 31))
                                                >> 31);
                                //get the reg_mask_len
                                reg_mask_len = pattern_len -
                                    WOW_MAXPATTERNLEN;
                                nvt_clean_unused_wow_mask(&reg_mask,
                                        reg_mask_len);
                                reg_mask_len = (reg_mask_len + 7) / 8;

                                memcpy((u32 *)&buf[*reg_idx].mask,
                                                &reg_mask,
                                                reg_mask_len);
                        }
                //only single long pattern
                } else {
                        if (*reg_idx == 0) {
                                if (pattern_len > WOW_MAXPATTERNLEN) {
                                        reg_mask_len = WOW_MAXPATTERNLEN;
                                } else {
                                        reg_mask_len = pattern_len;
                                }

                                nvt_clean_unused_wow_mask(start_mask,
                                        reg_mask_len);
                                reg_mask_len = (reg_mask_len + 7) / 8;

                                memcpy((u32 *)&buf[*reg_idx].mask,
                                            start_mask,
                                            reg_mask_len);
                        } else if (*reg_idx == 1) {
                                //reg_mask now : BIT32:BIT63
                                reg_mask = *(start_mask + 1);
                                reg_mask <<= 1;
                                reg_mask &= ~(1 << 0);
                                //reg_mask now: BIT31:BIT62
                                reg_mask |= (((*start_mask) & (1 << 31))
                                                >> 31);
                                //get reg_mask_len
                                if (pattern_len >
                                        2 * WOW_MAXPATTERNLEN) {
                                        reg_mask_len = WOW_MAXPATTERNLEN;
                                } else {
                                        reg_mask_len = pattern_len -
                                            WOW_MAXPATTERNLEN;
                                }
                                nvt_clean_unused_wow_mask(&reg_mask,
                                        reg_mask_len);
                                reg_mask_len = (reg_mask_len + 7) / 8;
                                memcpy((u32 *)&buf[*reg_idx].mask,
                                                &reg_mask, reg_mask_len);
                        } else if (*reg_idx == 2) {
                                reg_mask = *(start_mask + 2);
                                reg_mask <<= 2;
                                //clear BIT62 and BIT63
                                reg_mask &= ~(1 << 0);
                                reg_mask &= ~(1 << 1);
                                //OR BIT62
                                reg_mask |= (((*(start_mask + 1)) & (1 << 30))
                                            >> 30);
                                //OR BIT63
                                reg_mask |= ((((*(start_mask + 1)) & (1 << 31))
                                                >> 31) << 1);
                                //get reg_mask_len
                                if (pattern_len >
                                        3 * WOW_MAXPATTERNLEN) {
                                        reg_mask_len = WOW_MAXPATTERNLEN;
                                } else {
                                        reg_mask_len = pattern_len -
                                            2 *  WOW_MAXPATTERNLEN;
                                }
                                nvt_clean_unused_wow_mask(&reg_mask,
                                        reg_mask_len);
                                reg_mask_len = (reg_mask_len + 7) / 8;

                                memcpy((u32 *)&buf[*reg_idx].mask,
                                                &reg_mask, reg_mask_len);
                        } else {
                                reg_mask = *(start_mask + 3);
                                reg_mask <<= 3;
                                //clear BIT93 - BIT95
                                reg_mask &= ~(1 << 0);
                                reg_mask &= ~(1 << 1);
                                reg_mask &= ~(1 << 2);
                                //OR BIT93 to BIT95
                                reg_mask |= (((*(start_mask + 2)) & (1 << 29))
                                                >> 29);
                                reg_mask |= ((((*(start_mask + 2)) & (1 << 30))
                                                >> 30)
                                                << 1);
                                reg_mask |= ((((*(start_mask + 2)) & (1 << 31))
                                                >> 31)
                                                << 2);
                                reg_mask_len = pattern_len -
                                                3 * WOW_MAXPATTERNLEN;
                                nvt_clean_unused_wow_mask(&reg_mask,
                                        reg_mask_len);
                                reg_mask_len = (reg_mask_len + 7) / 8;
                                memcpy((u32 *)&buf[*reg_idx].mask,
                                                &reg_mask, reg_mask_len);
                        }

                }
        }
}

//AT config HW registers to one WoWlan Pattern
static void nvt_set_pat_buffer(struct cfg80211_wowlan *wow, u8 pat_cnt,
            struct wow_pattern *buf, u8 *reg_idx)
{
        u8 num_of_regs = nvt_get_num_of_pat_regs(wow, pat_cnt);
        u8 i, j;
        u8 pos = 0;
        //u32 size = 0;
        u16 crc16 = 0;
        u32 wid_pat_buf[WOW_MAXPATTERNLEN];
        u8 remain_pat_len = wow->patterns[pat_cnt].pattern_len;
        u8 end_of_size = 0;
        //u8 *start_mask = wow->patterns[pat_cnt].mask;
        //Looping each FW register
        for (j = 0; j < num_of_regs; j++) {
                //Looping the pattern bytes

                if (remain_pat_len < WOW_MAXPATTERNLEN) {
                        end_of_size = remain_pat_len;
                } else {
                        end_of_size = WOW_MAXPATTERNLEN;
                }

                for (i = j * WOW_MAXPATTERNLEN;
                            i < j * WOW_MAXPATTERNLEN + end_of_size; i++) {
                        if (wow->patterns[pat_cnt].mask[i/8] &
                                (0x01 << (i % 8))) {
                                wid_pat_buf[pos++] =
                                    wow->patterns[pat_cnt].pattern[i];
                        }
                }

                if (remain_pat_len >= WOW_MAXPATTERNLEN) {
                        remain_pat_len -= WOW_MAXPATTERNLEN;
                }

                crc16 = compute_crc(wid_pat_buf, pos);
                nvt_set_mask_by_reg_index(wow, reg_idx, buf, pat_cnt);
                buf[*reg_idx].crc16 = crc16;
                /*enable this filter register*/
                buf[*reg_idx].filter_enable = 1;
                //set offset only at first register
#if LINUX_VERSION_CODE >= KERNEL_VERSION(3, 9, 0)
                buf[*reg_idx].offset =
                    wow->patterns[pat_cnt].pkt_offset +
                    j * WOW_MAXPATTERNLEN;
#endif

                nvt_dbg(CFG80211, "%d[msk:%x][of:%d][cr:%x]\n",
                            *reg_idx, buf[*reg_idx].mask,
                            buf[*reg_idx].offset, buf[*reg_idx].crc16);
                if (num_of_regs == 1 && wow->n_patterns == 2 && *reg_idx == 0)
                        *reg_idx += 2;
                else
                        *reg_idx += 1;
                pos = 0;
        }
}

s32 nvt_setup_wowlan(struct _nvt_if *nvt_if, struct _nvt_cfg80211 *nvt_cfg,
                                struct cfg80211_wowlan *wow)
{
        struct _nvt_adapter *nvt_adapter = nvt_cfg->nvt_adapter;
        s32 ret = 0;
        u8 wow_mode = 0;
        u8 wow_enable = 0;
        u8 cnt = 0;
        u8 idx = 0;
        //u8 i = 0;
        //u32 pattern[WOW_MAXPATTERNLEN];
        //u8 pos = 0;
        //u16 crc16 = 0;
        //u32 size = 0;
        struct wow_pattern buf[WOW_MAXPATTERNS];

        nvt_dbg(CFG80211, "%s\n", __func__);
        /* todo: setup arp/ns/rekey offload */
        /* setup wow mode */
        if (wow == 0) {
                nvt_dbg(CFG80211, "mode=0x%x\n", wow_mode);
                goto err_ret;
        }
        if (wow->magic_pkt) {
                wow_mode |= WOW_MAGIC_PKT;
        }
        if (wow->disconnect) {
                wow_mode |= WOW_DISCONNECT;
        }

        if ((wow->patterns) && (wow->n_patterns)) {
                nvt_dbg(CFG80211, "pattern num=%d\n", wow->n_patterns);
                if (nvt_is_pats_valid(wow) < 0) {
                        nvt_dbg(ERROR, "pattern len check fail\n");

                } else {
                        memset(buf, 0x0,
                            sizeof(struct wow_pattern) * WOW_MAXPATTERNS);
                        wow_mode |= WOW_PATTERN;

                        //configuraion of HW register concatenation
                        if (wow->n_patterns > 2) {
                                wow_mode |= WOW_4_REG;
                        } else if (wow->n_patterns == 2) {
                                wow_mode |= WOW_2_REG;
                        } else {
                                wow_mode |= WOW_1_REG;
                        }

                        for (cnt = 0; cnt < wow->n_patterns; cnt++) {
                                nvt_set_pat_buffer(wow, cnt, buf, &idx);
                        }
                }
        }

        nvt_dbg(CFG80211, "mode=0x%x\n", wow_mode);
        /* setup wow */
        ret = nvt_icfg_lock(nvt_adapter->nvt_bus);
        if (ret < 0) {
                goto err_ret;
        }
        ret = nvt_icfg_reset(nvt_adapter->nvt_bus);
        if (ret < 0) {
                goto err_ret;
        }
        ret = nvt_icfg_add(nvt_adapter->nvt_bus, NVT_ICFG_SET,
                        WID_WOW_CONFIG, (u8 *)&wow_mode, sizeof(wow_mode));
        if (ret < 0) {
                goto err_ret;
        }
        if (idx != 0) {
                ret = nvt_icfg_add(nvt_adapter->nvt_bus, NVT_ICFG_SET,
                        WID_WOW_PATTERN, (u8 *)buf,
                        sizeof(struct wow_pattern)*idx);
                if (ret < 0) {
                        goto err_ret;
                }
        }
        /* set skip_dtim */
        if (nvt_if->skip_dtim_scalar != 0) {
                ret = nvt_icfg_add(nvt_adapter->nvt_bus, NVT_ICFG_SET,
                        WID_SET_DTIM_SCALAR, (u8 *)&nvt_if->skip_dtim_scalar,
                        sizeof(nvt_if->skip_dtim_scalar));
                if (ret < 0) {
                        goto err_ret;
                }
        }
        if (nvt_if->skip_dtim_time != 0) {
                ret = nvt_icfg_add(nvt_adapter->nvt_bus, NVT_ICFG_SET,
                        WID_SET_DTIM_TIME, (u8 *)&nvt_if->skip_dtim_time,
                        sizeof(nvt_if->skip_dtim_time));
                if (ret < 0) {
                        goto err_ret;
                }
        }
        /* set ignore_bcn_bitmap */
        ret = nvt_icfg_add(nvt_adapter->nvt_bus, NVT_ICFG_SET,
                WID_IGNORE_BCN_BITMAP, (u8 *)&nvt_if->ignore_bcn_bitmap,
                sizeof(nvt_if->ignore_bcn_bitmap));
        if (ret < 0) {
                goto err_ret;
        }

        if (wow_mode != 0) {
                wow_enable = 1;
        }
        ret = nvt_icfg_add(nvt_adapter->nvt_bus, NVT_ICFG_SET,
                        WID_WOW_ENABLE, (u8 *)&wow_enable, sizeof(wow_enable));
        if (ret < 0) {
                goto err_ret;
        }

        ret = nvt_icfg_send_and_get_resp(nvt_adapter->nvt_bus,
                             NULL, 0);
        if (ret < 0) {
                goto err_ret;
        }
err_ret:
        nvt_icfg_unlock(nvt_adapter->nvt_bus);
        return ret;
}
#endif

static s32 nvt_cfg80211_set_default_mgmt_key(struct wiphy *wiphy,
                        struct net_device *ndev, u8 key_id)
{
        nvt_dbg(CFG80211, "set default mgmt key,id=%d\n", key_id);
        return 0;
}

#ifdef CONFIG_PM
static s32 nvt_cfg80211_resume(struct wiphy *wiphy)
{
        struct _nvt_cfg80211 *cfg = (struct _nvt_cfg80211 *)wiphy_priv(wiphy);
        struct _nvt_adapter *nvt_adapter = cfg->nvt_adapter;
        struct _nvt_if *nvt_if = nvt_vif_first(nvt_adapter);
        s32 ret = 0;
        u8 fc_enable = 1;
        u8 i = 0;
        u8 buf[64] = {0};
        nvt_dbg(CFG80211, "%s\n", __func__);
        if (!nvt_if) {
                nvt_dbg(ERROR, "nvt_if == NULL\n");
                return -EPERM;
        }

        //nash:work around for nt96660
        ret |= nvt_sdio_wa_resume(nvt_adapter->nvt_bus);

        /* Disable flow control before suspend */
        ret = nvt_send_wid_by_numeric(nvt_adapter->nvt_bus,
                        WID_FC_ENABLE, &fc_enable, 1);
        if (ret < 0) {
                nvt_dbg(ERROR, "%s: FC enable fail\n",
                        __func__);
        }

        /* Prepare DELBA for all TID Command to FW before resume */
        for (i = 0; i < 8; i++) {
                buf[0] = 12;
                /* Block Ack Category */
                buf[1] = 3;
                /* MLME_DELBA_REQ_TYPE */
                buf[2] = 2;
                memcpy(&buf[3], nvt_if->nvt_wconf.bssid, ETH_ALEN);
                /* TID */
                buf[9] = i;
                /* Recipient */
                buf[10] = 0;
                /* QSTA_TIMEOUT */
                buf[11] = 39;
                ret = nvt_send_wid_by_numeric(nvt_adapter->nvt_bus,
                        WID_11N_P_ACTION_REQ, (u8 *)&buf, 12);
                if (ret < 0) {
                        nvt_dbg(ERROR, "%s: delba on tid(%d) fail\n",
                        __func__, i);
                }
        }

        if (nvt_if->sleep_mode != NVT_SLEEP_MODE_DISABLE) {
                ret = nvt_icfg_lock(nvt_adapter->nvt_bus);
                if (ret < 0) {
                        goto err_ret;
                }

                ret = nvt_icfg_reset(nvt_adapter->nvt_bus);
                if (ret < 0) {
                        goto err_ret;
                }

                ret = nvt_icfg_add(nvt_adapter->nvt_bus, NVT_ICFG_SET,
                        WID_WOW_HOST_RESUME, (u8 *)&(nvt_if->sleep_mode),
                        sizeof(nvt_if->sleep_mode));
                if (ret < 0) {
                        goto err_ret;
                }

                ret = nvt_icfg_send_and_get_resp(nvt_adapter->nvt_bus, NULL, 0);
                if (ret < 0) {
                        goto err_ret;
                }

err_ret:
                nvt_icfg_unlock(nvt_adapter->nvt_bus);
        }

        return ret;
}
#endif

s32 nvt_set_arp_ns_offload(struct _nvt_if *nvt_if)
{
        struct net_device *ndev = nvt_if->ndev;
        u8 *ipv4_addr;
#if IS_ENABLED(CONFIG_IPV6)
        u8 *ipv6_addr;
#endif
        u8 *wid_val_buf;
        s32 ret;
        u8 buf_size;
        ret = 0;
        buf_size = 1;

        /* allocate buffer for IPv4 */
        ipv4_addr = kzalloc(4, GFP_KERNEL);
        if (!ipv4_addr) {
                nvt_dbg(ERROR, "%s: IPv4 buffer created fail\n",
                                __func__);
                ret = -1;
                goto err_ret;

        }

        /* check if IPv4 address is NULL */
        ret = nvt_get_first_ipv4(ndev, ipv4_addr);
        if (ret == 0)
                buf_size += 4;
        else
                ret = 0;

#if IS_ENABLED(CONFIG_IPV6)
        /* allocate buffer for IPv6 */
        ipv6_addr = kzalloc(16, GFP_KERNEL);
        if (!ipv6_addr) {
                nvt_dbg(ERROR, "%s: IPv6 buffer created fail\n",
                                __func__);
                ret = -1;
                kfree(ipv4_addr);
                goto err_ret;
        }
        /* check if IPv6 address is being set by user */
        ret = nvt_get_first_ipv6(ndev, ipv6_addr);
        if (ret == 0)
                buf_size += 16;
        else
                ret = 0;
#else
        nvt_dbg(CFG80211, "%s: KERNEL doesnot support IPv6\n", __func__);
#endif

        /* create the buffer for sending to FW */
        wid_val_buf = kzalloc(buf_size, GFP_KERNEL);
        if (!wid_val_buf) {
                nvt_dbg(ERROR, "%s: buffer send to FW created fail\n",
                                __func__);
                ret = -1;
                kfree(ipv4_addr);
#if IS_ENABLED(CONFIG_IPV6)
                kfree(ipv6_addr);
#endif
                goto err_ret;
        }

        if (buf_size == 1) {
                nvt_dbg(CFG80211, "%s: NO ARP-NS OFFLOAD\n", __func__);
                *wid_val_buf = 0;
        /* Only set IPv4 address */
        } else if (buf_size == 5) {
                *wid_val_buf = 128;
                nvt_dbg(CFG80211, "%s: ARP OFFLOAD only\n", __func__);
                memcpy(wid_val_buf + 1, ipv4_addr, 4);
        /* Only set IPv6 address */
        } else {
#if IS_ENABLED(CONFIG_IPV6)
                if (buf_size == 17) {
                        *wid_val_buf = 64;
                        nvt_dbg(CFG80211, "%s: NS OFFLOAD only\n",
                                        __func__);
                        memcpy(wid_val_buf + 1, ipv6_addr, 16);
                } else {
                        /* case: buf_size == 21 */
                        *wid_val_buf = 192;
                        nvt_dbg(CFG80211, "%s: ARP-NS OFFLOAD\n",
                                        __func__);
                        memcpy(wid_val_buf + 1, ipv4_addr, 4);
                        memcpy(wid_val_buf + 5, ipv6_addr, 16);
                }
#endif
        }
        if (ret == 0)
                ret = nvt_send_wid_by_numeric(nvt_if->nvt_adapter->nvt_bus,
                        WID_ARP_NS_OFFLOAD, wid_val_buf, buf_size);

        kfree(ipv4_addr);
#if IS_ENABLED(CONFIG_IPV6)
        kfree(ipv6_addr);
#endif
        kfree(wid_val_buf);
err_ret:
        return ret;
}

#ifdef CONFIG_PM
s32 nvt_set_wowlan_on(struct cfg80211_wowlan *wow, struct _nvt_if *nvt_if)
{
        struct _nvt_cfg80211 *nvt_cfg = nvt_if->nvt_adapter->nvt_cfg80211;
        s32 ret = -1;
        if (wow != NULL) {
                if (nvt_setup_wowlan(nvt_if, nvt_cfg, wow) == 0) {
                        nvt_cfg->wow_enable = 1;
                        ret = 0;
                } else {
                        nvt_cfg->wow_enable = 0;
                }
        }
        return ret;
}

static s32 nvt_cfg80211_suspend(struct wiphy *wiphy,
                                   struct cfg80211_wowlan *wow)
{
        struct _nvt_cfg80211 *cfg = (struct _nvt_cfg80211 *)wiphy_priv(wiphy);
        struct _nvt_adapter *nvt_adapter = cfg->nvt_adapter;
        struct _nvt_if *nvt_if = nvt_vif_first(nvt_adapter);
        s32 ret = 0;
        u8 fc_enabled = 0;
        u8 pn_byte_cnt = 0;
        u8 sta_index;
        nvt_dbg(CFG80211, "%s\n", __func__);
        if (!nvt_if) {
                nvt_dbg(ERROR, "nvt_if == NULL\n");
                return -EPERM;
        }
        if (!check_if_ready(nvt_if)) {
                return 0;
        }

        if (nvt_txmq == 0) {
                netif_stop_queue(nvt_if->ndev);
        } else {
                netif_tx_stop_all_queues(nvt_if->ndev);
        }

        nvt_abort_scanning(cfg);
        /* Disable flow control before suspend */
        ret = nvt_send_wid_by_numeric(nvt_adapter->nvt_bus,
                        WID_FC_ENABLE, &fc_enabled, 1);
        if (ret < 0) {
                nvt_dbg(ERROR, "%s: FC disable fail\n",
                        __func__);
        }
        /* Reset BA handlers */
        nvt_reset_ba_handle(nvt_adapter, 1);
        //AT: reset broadcast pn to 0
        if (nvt_adapter->nvt_priv.pn_bcmc != NULL) {
                for (pn_byte_cnt = 0; pn_byte_cnt < 6; pn_byte_cnt++) {
                        nvt_adapter->nvt_priv.pn_bcmc[pn_byte_cnt] = 0;
                }
        }
        /*AT reset the unicast when f/w offload 4-way handshake */
        if (nvt_ptk_offload_enable &&
            (nvt_adapter->nvt_fw_cap.mac_cap_1 &
             FW_MAC_CAP_1_INTERNAL_SUPP_CAP)) {
                for (sta_index = 1; sta_index < 9; sta_index++) {
                        nvt_reset_unicast_pn(nvt_adapter, sta_index);
                }
        }

        if ((wow == NULL) ||
                (!test_bit(NVT_IF_CONNECTED, &nvt_if->state_flags))) {
                nvt_dbg(CFG80211, "%s without wow\n", __func__);
                if (test_bit(NVT_IF_CONNECTED, &nvt_if->state_flags)) {
                        nvt_disconnect_action(nvt_if);
                }
                nvt_if->sleep_mode = NVT_SLEEP_MODE_NORMAL;
        } else {
                nvt_dbg(CFG80211, "%s with wow\n", __func__);

                nvt_if->sleep_mode = NVT_SLEEP_MODE_WOW;

                /* set ap recover */
                ret = nvt_send_wid_by_numeric(nvt_adapter->nvt_bus,
                        WID_AP_RECOVER, nvt_if->recover_buf,
                        RECOVER_BUF_LEN);
                if (ret < 0) {
                        nvt_dbg(ERROR, "%s: WID_AP_RECOVER fail\n", __func__);
                }

                /* set arp_ns_offload */
                ret = nvt_set_arp_ns_offload(nvt_if);
                if (ret < 0) {
                        nvt_dbg(ERROR, "%s: arp_ns_offload send fail to FW\n",
                                __func__);
                }
                /* set pmt module and wowlan main flag on */
                ret = nvt_set_wowlan_on(wow, nvt_if);
                if (ret < 0) {
                        nvt_dbg(ERROR, "%s: arp_ns_offload send fail to FW\n",
                                __func__);
                }
        }

        ret = nvt_icfg_lock(nvt_adapter->nvt_bus);
        if (ret < 0) {
                goto err_ret;
        }

        ret = nvt_icfg_reset(nvt_adapter->nvt_bus);
        if (ret < 0) {
                goto err_ret;
        }

        ret = nvt_icfg_add(nvt_adapter->nvt_bus, NVT_ICFG_SET,
                WID_WOW_HOST_SUSPEND, (u8 *)&(nvt_if->sleep_mode),
                sizeof(nvt_if->sleep_mode));
        if (ret < 0) {
                goto err_ret;
        }

        ret = nvt_icfg_send_and_get_resp(nvt_adapter->nvt_bus, NULL, 0);
        if (ret < 0) {
                goto err_ret;
        }

err_ret:
        nvt_icfg_unlock(nvt_adapter->nvt_bus);

        //netif_stop_queue(nvt_if->ndev);

        //nash:work around for nt96660
        ret |= nvt_sdio_wa_suspend(nvt_adapter->nvt_bus);

        return ret;
}
#endif /*CONFIG_PM*/

static s32 nvt_set_gtk_rekey_data(struct wiphy *wiphy,
        struct net_device *ndev, struct cfg80211_gtk_rekey_data *data)
{
        struct _nvt_if *nvt_if = netdev_priv(ndev);
        struct _nvt_adapter *nvt_adapter = nvt_if->nvt_adapter;
        struct _nvt_sec *sec = &nvt_if->nvt_wconf.nvt_sec;
        s32 ret = 0;
        static s8 count;

        nvt_dbg(CFG80211, "%s\n", __func__);

        ret = nvt_icfg_lock(nvt_adapter->nvt_bus);
        if (ret < 0) {
                goto err_ret;
        }

        ret = nvt_icfg_reset(nvt_adapter->nvt_bus);
        if (ret < 0) {
                goto err_ret;
        }

        count++;
        if (sec->ext_mode_11i & WID_EXT_11I_WAPI) {
                nvt_dbg(CFG80211, "WAPI REKEY OFFLOAD\n");
                if (count == 1 &&
                        memcmp(data->replay_ctr,
                        (u8 *) "\xff\xff\xff\xff\xff\xff\xff\xff",
                        NL80211_REPLAY_CTR_LEN) == 0) {
                        nvt_dbg(CFG80211, "Bufffer first half\n");
                        memset(&sec->wapi_rekey_offload, 0,
                                sizeof(struct _nvt_wapi_rekey_offload_data));
                        memcpy(sec->wapi_rekey_offload.kek,
                                data->kek, NL80211_KEK_LEN);
                        memcpy(sec->wapi_rekey_offload.kck,
                                data->kck, NL80211_KCK_LEN);
                        ret = 0;
                        goto err_ret;
                } else if (count == 2 &&
                        memcmp(data->kek, data->kck, NL80211_KEK_LEN) == 0) {
                        nvt_dbg(CFG80211, "Bufffer second half\n");
                        count = 0;
                        /* copy reply_counter */
                        memcpy(sec->wapi_rekey_offload.replay_ctr,
                                data->kek, WAPI_REPLAY_CTR_LEN);
                        /* copy seq_num */
                        memcpy(sec->wapi_rekey_offload.seq_num,
                                data->replay_ctr, 8);
                        ret = nvt_icfg_add(nvt_adapter->nvt_bus,
                                NVT_ICFG_SET, WID_WAPI_REKEY_OFFLOAD,
                                (u8 *)&sec->wapi_rekey_offload,
                                sizeof(struct _nvt_wapi_rekey_offload_data));
                        if (ret < 0)
                                goto err_ret;
                } else {
                        nvt_dbg(CFG80211, "count=%d,Something Wrong!\n", count);
                        count = 0;
                        memset(&sec->wapi_rekey_offload, 0,
                                sizeof(struct _nvt_wapi_rekey_offload_data));
                        ret = -EINVAL;
                        goto err_ret;
                }
        } else if ((sec->mode_11i & WID_11I_TKIP) ||
                (sec->mode_11i & WID_11I_CCMP)) {
                nvt_dbg(CFG80211, "WPA REKEY OFFLOAD\n");
                count = 0;
                memset(&sec->wpa_rekey_offload, 0,
                        sizeof(struct _nvt_wpa_rekey_offload_data));
                memcpy(sec->wpa_rekey_offload.kek, data->kek, NL80211_KEK_LEN);
                memcpy(sec->wpa_rekey_offload.kck, data->kck, NL80211_KCK_LEN);
                memcpy(sec->wpa_rekey_offload.replay_ctr,
                        data->replay_ctr, NL80211_REPLAY_CTR_LEN);
                ret = nvt_icfg_add(nvt_adapter->nvt_bus,
                        NVT_ICFG_SET, WID_WPA_REKEY_OFFLOAD,
                        (u8 *)&sec->wpa_rekey_offload,
                        sizeof(struct _nvt_wpa_rekey_offload_data));
                if (ret < 0) {
                        goto err_ret;
                }
        } else {
                nvt_dbg(CFG80211, "Rekey but not in WPA/WPA2/WAPI mode\n");
                count = 0;
                ret = -EINVAL;
                goto err_ret;
        }
        ret = nvt_icfg_send_and_get_resp(nvt_adapter->nvt_bus, NULL, 0);
err_ret:
        nvt_icfg_unlock(nvt_adapter->nvt_bus);
        return ret;

}

#if LINUX_VERSION_CODE >= KERNEL_VERSION(3, 6, 0)
static int nvt_set_monitor_channel(struct wiphy *wiphy,
                                struct cfg80211_chan_def *chandef)
{
        struct _nvt_cfg80211 *cfg = (struct _nvt_cfg80211 *)wiphy_priv(wiphy);
        struct _nvt_adapter *nvt_adapter = cfg->nvt_adapter;
        int channel;
        u8 *ch_param = NULL;
        u8 ch_index = 0;
        u8 scan_type = PASSIVE_SCAN;
        u8 scan_time = 10;
        u8 list_len = 0;
        s32 ret = 0;
        u8 site_survey = 1;
        u8 user_scan = 1;
        u8 start_op = 0;
        u8 scan_filter = SCAN_NO_ADHOC | SCAN_SAVE_WITHOUTRSSI | SCAN_CURRENTCH;
        struct _nvt_probe_ssid ssid_list[MAX_PROBED_SSIDS];


        nvt_dbg(CFG80211, "%s\n", __func__);

        if (chandef->chan == NULL) {
                return -EINVAL;
        }

        if ((cfg80211_get_chandef_type(chandef) == NL80211_CHAN_HT40MINUS) ||
                (cfg80211_get_chandef_type(chandef) == NL80211_CHAN_HT40PLUS)) {
                return -EINVAL;
        }

        if (chandef->chan->band != IEEE80211_BAND_2GHZ) {
                return -EINVAL;
        }


#if LINUX_VERSION_CODE >= KERNEL_VERSION(3, 15, 0)
        channel = ieee80211_frequency_to_channel(chandef->chan->center_freq);
#else
        channel = ieee80211_freq_to_dsss_chan(chandef->chan->center_freq);
#endif

        memset(ssid_list, 0x0, sizeof(struct _nvt_probe_ssid)*MAX_PROBED_SSIDS);

        if ((channel > 0) && (channel <= 14)) {
                ch_param = kzalloc(1*2+1, GFP_KERNEL);
                if (ch_param == NULL) {
                        return -EINVAL;
                }
                ch_param[ch_index++] = 1;
                ch_param[ch_index++] = IEEE80211_BAND_2GHZ;
                ch_param[ch_index++] = channel;

                ret = nvt_icfg_lock(nvt_adapter->nvt_bus);
                if (ret < 0) {
                        goto err_ret;
                }

                ret = nvt_icfg_reset(nvt_adapter->nvt_bus);
                if (ret < 0) {
                        goto err_ret;
                }

                ret = nvt_icfg_add(nvt_adapter->nvt_bus, NVT_ICFG_SET,
                        WID_USER_PREF_CHANNEL, (u8 *)&channel, 2);
                if (ret < 0) {
                        goto err_ret;
                }

                /* set scan_type */
                ret = nvt_icfg_add(nvt_adapter->nvt_bus, NVT_ICFG_SET,
                        WID_SCAN_TYPE, (u8 *)&scan_type, sizeof(scan_type));
                if (ret < 0) {
                        goto err_ret;
                }

                /* set scan filter */
                ret = nvt_icfg_add(nvt_adapter->nvt_bus, NVT_ICFG_SET,
                        WID_SCAN_FILTER, (u8 *)&scan_filter,
                        sizeof(scan_filter));
                if (ret < 0) {
                        goto err_ret;
                }

                ret = nvt_icfg_add(nvt_adapter->nvt_bus, NVT_ICFG_SET,
                        WID_PASSIVE_SCAN_TIME, (u8 *)&scan_time,
                        sizeof(scan_time));
                if (ret < 0) {
                        goto err_ret;
                }

                ret = nvt_icfg_add(nvt_adapter->nvt_bus, NVT_ICFG_SET,
                        WID_SCAN_MULTICH, (u8 *)ch_param, 1*2+1);
                if (ret < 0) {
                        goto err_ret;
                }

                /* set enable site survey */
                ret = nvt_icfg_add(nvt_adapter->nvt_bus, NVT_ICFG_SET,
                        WID_SITE_SURVEY, (u8 *)&site_survey, sizeof(u8));
                if (ret < 0) {
                        goto err_ret;
                }

                /* set ssid list */
                ret = nvt_icfg_add(nvt_adapter->nvt_bus, NVT_ICFG_SET,
                        WID_SCAN_MULTISSID, (u8 *)ssid_list, list_len);
                if (ret < 0) {
                        goto err_ret;
                }

                /* set user scan */
                ret = nvt_icfg_add(nvt_adapter->nvt_bus, NVT_ICFG_SET,
                        WID_START_SCAN_REQ, (u8 *)&user_scan,
                        sizeof(user_scan));
                if (ret < 0) {
                        goto err_ret;
                }

                /* set reset mode */
                start_op = DO_RESTART;
                ret = nvt_icfg_add(nvt_adapter->nvt_bus, NVT_ICFG_SET,
                        WID_START_OP, (u8 *)&start_op, sizeof(start_op));
                if (ret < 0) {
                        goto err_ret;
                }

                ret = nvt_icfg_send_and_get_resp(nvt_adapter->nvt_bus, NULL, 0);
                if (ret < 0) {
                        goto err_ret;
                }
err_ret:
                nvt_icfg_unlock(nvt_adapter->nvt_bus);

                kfree(ch_param);
        }
        return 0;
}
#endif

#if LINUX_VERSION_CODE >= KERNEL_VERSION(3, 9, 0)
static void nvt_reg_notifier(struct wiphy *wiphy,
                                struct regulatory_request *request)
#else
static int nvt_reg_notifier(struct wiphy *wiphy,
                                struct regulatory_request *request)
#endif
{
        struct _nvt_cfg80211 *cfg = (struct _nvt_cfg80211 *)wiphy_priv(wiphy);
        //struct _nvt_adapter *nvt_adapter = cfg->nvt_adapter;
        u8 no_of_triplet = 0;
        struct ieee80211_country_ie_triplet *t;
        u8 no_of_parsed_chan = 0;
        u8 first_chan = 0, next_chan = 0, max_pwr = 0;
        u8 i, flag = 0;
        enum ieee80211_band band;
        struct ieee80211_supported_band *sband;
        struct ieee80211_channel *ch;
        struct _nvt_802_11d_reg *domain_info = &cfg->domain_reg;

        /* ignore non-ISO3166 country codes */
        for (i = 0; i < sizeof(request->alpha2); i++) {
                if (request->alpha2[i] < 'A' || request->alpha2[i] > 'Z') {
                        nvt_dbg(ERROR, "%s\n", __func__);
                        nvt_dbg(ERROR, "not a ISO3166 code (0x%02x 0x%02x)\n",
                                request->alpha2[0], request->alpha2[1]);
                        nvt_dbg(ERROR, "regulatory request pointer=%p)\n",
                                request);
#if LINUX_VERSION_CODE >= KERNEL_VERSION(3, 9, 0)
                        return;
#else
                        return -1;
#endif
                }
        }
#if LINUX_VERSION_CODE >= KERNEL_VERSION(3, 6, 0)
        nvt_dbg(CFG80211,
                "%s, cfg reg_notify %c%c%s%s initiator %d hint_type %d\n",
                __func__,
                request->alpha2[0], request->alpha2[1],
                request->intersect ? " intersect" : "",
                request->processed ? " processed" : "",
                request->initiator, request->user_reg_hint_type);
#else
        nvt_dbg(CFG80211,
                "%s, cfg reg_notify %c%c%s%s initiator %d\n",
                __func__,
                request->alpha2[0], request->alpha2[1],
                request->intersect ? " intersect" : "",
                request->processed ? " processed" : "",
                request->initiator);
#endif

        switch (request->initiator) {
        case NL80211_REGDOM_SET_BY_DRIVER:
        case NL80211_REGDOM_SET_BY_CORE:
        case NL80211_REGDOM_SET_BY_USER:
                break;
                /* Todo: apply driver specific changes in channel flags based
                on the request initiator if necessary. */
        case NL80211_REGDOM_SET_BY_COUNTRY_IE:
                break;
        }

        /* Set country code */
        domain_info->country_code[0] = request->alpha2[0];
        domain_info->country_code[1] = request->alpha2[1];
        domain_info->country_code[2] = ' ';

        band = IEEE80211_BAND_2GHZ;
        sband = wiphy->bands[band];

        for (i = 0; i < sband->n_channels; i++) {
                ch = &sband->channels[i];
                nvt_dbg(CFG80211, "i=%d, n_channels=%d, flag=%d\n",
                        i, sband->n_channels, flag);
                nvt_dbg(CFG80211, "beacon_found=%d, flags=%d, orig_flag=%d\n",
                        ch->beacon_found, ch->flags, ch->orig_flags);
                nvt_dbg(CFG80211, "max_pwr=%d, orig_pwr=%d\n",
                        ch->max_power, ch->orig_mpwr);
                if (ch->flags & IEEE80211_CHAN_DISABLED)
                        continue;

                if (!flag) {
                        flag = 1;
                        first_chan = (u32) ch->hw_value;
                        next_chan = first_chan;
                        max_pwr = ch->max_power;
                        no_of_parsed_chan = 1;
                        continue;
                }

                if (ch->hw_value == next_chan + 1 && ch->max_power == max_pwr) {
                        next_chan++;
                        no_of_parsed_chan++;
                } else {
                        t = &domain_info->triplet[no_of_triplet];
                        t->chans.first_channel = first_chan;
                        t->chans.num_channels = no_of_parsed_chan;
                        t->chans.max_power = max_pwr;
                        no_of_triplet++;
                        first_chan = (u32) ch->hw_value;
                        next_chan = first_chan;
                        max_pwr = ch->max_power;
                        no_of_parsed_chan = 1;
                }
        }

        if (flag) {
                t = &domain_info->triplet[no_of_triplet];
                t->chans.first_channel = first_chan;
                t->chans.num_channels = no_of_parsed_chan;
                t->chans.max_power = max_pwr;
                no_of_triplet++;
        }

        domain_info->no_of_triplet = no_of_triplet;
#if LINUX_VERSION_CODE >= KERNEL_VERSION(3, 9, 0)
        return;
#else
        return 0;
#endif
}


static struct cfg80211_ops nvt_cfg80211_ops = {
        .add_key = nvt_cfg80211_add_key,
        .del_key = nvt_cfg80211_del_key,
        .set_default_key = nvt_cfg80211_set_default_key,
        .set_wiphy_params = nvt_cfg80211_set_wiphy_params,
        .get_station = nvt_get_station,
        .dump_station = nvt_dump_station,
        .set_tx_power = nvt_cfg80211_set_txpower,
        .set_bitrate_mask = nvt_cfg80211_set_bitrate,
        .set_power_mgmt = nvt_cfg80211_set_power_mgmt,
        .scan = nvt_cfg80211_scan,
        .connect = nvt_cfg80211_connect,
        .disconnect = nvt_cfg80211_disconnect,
        .mgmt_frame_register = nvt_cfg80211_mgmt_frame_register,
        .mgmt_tx = nvt_cfg80211_mgmt_tx,
        .remain_on_channel = nvt_cfg80211_remain_on_channel,
        .cancel_remain_on_channel = nvt_cfg80211_cancel_remain_on_channel,
        .del_station = nvt_cfg80211_del_station,
        .change_virtual_intf = nvt_cfg80211_change_iface,
        .set_default_mgmt_key = nvt_cfg80211_set_default_mgmt_key,
#ifdef CONFIG_PM
        .suspend = nvt_cfg80211_suspend,
        .resume = nvt_cfg80211_resume,
#endif
        .set_rekey_data = nvt_set_gtk_rekey_data,
#if LINUX_VERSION_CODE >= KERNEL_VERSION(3, 4, 0)
        .change_beacon = nvt_cfg80211_change_beacon,
        .start_ap = nvt_cfg80211_start_ap,
        .stop_ap = nvt_cfg80211_stop_ap,
#else
        .set_channel = nvt_cfg80211_set_channel,
        .add_beacon = nvt_cfg80211_add_beacon,
        .set_beacon = nvt_cfg80211_set_beacon,
        .del_beacon = nvt_cfg80211_del_beacon,
#endif
#if LINUX_VERSION_CODE >= KERNEL_VERSION(3, 6, 0)
        .set_monitor_channel = nvt_set_monitor_channel,
#endif
};

static void nvt_scan_timeout(unsigned long data)
{
        struct _nvt_cfg80211 *cfg = (struct _nvt_cfg80211 *)data;

        nvt_dbg(CFG80211, "%s\n", __func__);

        if (test_bit(SCAN_PROCESS, &cfg->scan_sts)) {
                schedule_work(&cfg->scan_timeout_work);
        }
}

static void nvt_scan_timeout_handle(struct work_struct *work)
{
        struct _nvt_cfg80211 *nvt_cfg =
                container_of(work, struct _nvt_cfg80211, scan_timeout_work);

        nvt_dbg(CFG80211, "%s\n", __func__);

        nvt_scan_complete(nvt_cfg->nvt_adapter, 1, 0);
}

static void nvt_cfg_priv_init(struct _nvt_cfg80211 *nvt_cfg)
{
        init_timer(&nvt_cfg->scan_timer);
        nvt_cfg->scan_timer.data = (unsigned long)nvt_cfg;
        nvt_cfg->scan_timer.function = nvt_scan_timeout;
        mutex_init(&nvt_cfg->scan_lock);
        INIT_WORK(&nvt_cfg->scan_timeout_work, nvt_scan_timeout_handle);
        spin_lock_init(&nvt_cfg->bss_list_lock);
        INIT_LIST_HEAD(&nvt_cfg->bss_list);
}

void nvt_abort_scanning(struct _nvt_cfg80211 *cfg)
{
        if (test_bit(SCAN_PROCESS, &cfg->scan_sts)) {
                nvt_scan_complete(cfg->nvt_adapter, 1, 1);
        }
}

static void nvt_cfg_priv_deinit(struct _nvt_cfg80211 *nvt_cfg)
{
        nvt_abort_scanning(nvt_cfg);
}

/**
 * nvt_cfg80211_init - register cfg80211 and init relative parameters
 *
 * @nvt_adapter: struct _nvt_adapter
 *
 * Return: return 0 success, return < 0 error
 */
s32 nvt_cfg80211_init(struct _nvt_adapter *nvt_adapter)
{
        struct wiphy *wiphy;
        struct _nvt_cfg80211 *nvt_cfg;

        nvt_dbg(CFG80211, "%s\n", __func__);

        wiphy = wiphy_new(&nvt_cfg80211_ops, sizeof(struct _nvt_cfg80211));

        if (!wiphy) {
                nvt_dbg(CFG80211, "allocate wiphy fail\n");
                return -1;
        }

        wiphy->max_scan_ssids = NVT_NUM_SCAN_MAX;
        wiphy->max_scan_ie_len = NVT_SCAN_VSIE_LEN_MAX;
        wiphy->interface_modes = BIT(NL80211_IFTYPE_STATION) |
                                BIT(NL80211_IFTYPE_AP) |
                                BIT(NL80211_IFTYPE_P2P_CLIENT) |
                                BIT(NL80211_IFTYPE_P2P_GO) |
                                BIT(NL80211_IFTYPE_MONITOR);

        //add for suppoting the iw set bitrate command
        nvt_band_2ghz.ht_cap.mcs.rx_mask[0] = 0xff;
        nvt_band_2ghz.ht_cap.mcs.rx_mask[1] = 0xff;
        nvt_band_2ghz.ht_cap.mcs.rx_mask[4] = 0x01;

        wiphy->bands[IEEE80211_BAND_2GHZ] = &nvt_band_2ghz;
        wiphy->signal_type = CFG80211_SIGNAL_TYPE_MBM;
        wiphy->cipher_suites = nvt_cipher_suites;
        wiphy->n_cipher_suites = ARRAY_SIZE(nvt_cipher_suites);
        wiphy->flags &= ~WIPHY_FLAG_PS_ON_BY_DEFAULT;
        wiphy->flags |= WIPHY_FLAG_HAVE_AP_SME |
                        WIPHY_FLAG_HAS_REMAIN_ON_CHANNEL;
#if LINUX_VERSION_CODE < KERNEL_VERSION(3, 14, 0)
        wiphy->flags |= WIPHY_FLAG_CUSTOM_REGULATORY;
        //                WIPHY_FLAG_STRICT_REGULATORY |
#else
        wiphy->regulatory_flags |= REGULATORY_CUSTOM_REG;
#endif

#if LINUX_VERSION_CODE >= KERNEL_VERSION(3, 4, 0)
        wiphy->features |= NL80211_FEATURE_INACTIVITY_TIMER;
#endif
        wiphy_apply_custom_regulatory(wiphy, &nvt_regdom_custom);
        wiphy->mgmt_stypes = nvt_txrx_stypes;
        wiphy->max_remain_on_channel_duration = 5000;
#ifdef CONFIG_PM
#if LINUX_VERSION_CODE > KERNEL_VERSION(3, 10, 0)
        wiphy->wowlan = &nvt_wowlan_support;
#elif LINUX_VERSION_CODE > KERNEL_VERSION(3, 8, 0)
        wiphy->wowlan = nvt_wowlan_support;
#endif
#endif/*CONFIG_PM*/
        /* get the f/w capability */
        nvt_set_firmware_capability(nvt_adapter);
        /* AT vendor command for PTK offload */
        if (nvt_ptk_offload_enable &&
            (nvt_adapter->nvt_fw_cap.mac_cap_1 &
                FW_MAC_CAP_1_INTERNAL_SUPP_CAP)) {
                nvt_dbg(CFG80211, "%s PTK offload enable\n", __func__);
#ifdef VENDOR_CMD_SUPPORT
                wiphy->vendor_commands = nvt_vendor_commands;
                wiphy->n_vendor_commands = ARRAY_SIZE(nvt_vendor_commands);
                wiphy->extended_capabilities = extended_capabilities;
                wiphy->extended_capabilities_mask = extended_capabilities;
                wiphy->extended_capabilities_len =
                        ARRAY_SIZE(extended_capabilities);
#else
                nvt_dbg(ERROR, "%s VENDOR CMD Compile Option is off!\n",
                        __func__);
#endif
        }

        wiphy->reg_notifier = nvt_reg_notifier;

        set_wiphy_dev(wiphy, nvt_adapter->nvt_bus->dev);

        nvt_cfg = wiphy_priv(wiphy);
        nvt_cfg->wiphy = wiphy;
        nvt_cfg->nvt_adapter = nvt_adapter;

        if (wiphy_register(wiphy) < 0) {
                wiphy_free(wiphy);
                return -1;
        }

        nvt_cfg_priv_init(nvt_cfg);

        nvt_adapter->nvt_cfg80211 = nvt_cfg;

        //regulatory_hint(wiphy, "TW ");
        return 0;
}

/**
 * nvt_cfg80211_deinit - unregister cfg80211 and free relative parameters
 *
 * @nvt_adapter: struct _nvt_adapter
 *
 * Return: return 0 success
 */
s32 nvt_cfg80211_deinit(struct _nvt_adapter *nvt_adapter)
{
        struct _nvt_cfg80211 *nvt_cfg = nvt_adapter->nvt_cfg80211;

        nvt_dbg(CFG80211, "%s\n", __func__);

        nvt_cfg_priv_deinit(nvt_cfg);
        wiphy_unregister(nvt_cfg->wiphy);
        wiphy_free(nvt_cfg->wiphy);

        return 0;
}

/**
 * nvt_scan_result - scan bss list and scan done evnet handle
 *
 * @nvt_if: nvt network interface
 * @msg_len: event message length
 * @data: event data
 *
 * Return: return 0 success, return < 0 error
 */
s32 nvt_scan_result(struct _nvt_if *nvt_if, u16 msg_len, u8 *data)
{
        struct _nvt_adapter *nvt_adapter = nvt_if->nvt_adapter;
        struct _nvt_cfg80211 *cfg = nvt_adapter->nvt_cfg80211;
        struct _nvt_bss_info *bss = NULL;
        s32 ret = 0;
        u32 size = 0;
        u32 ie_len = 0;
        u32 ie_offset = 0;
        u16 wid = data[0]+(data[1] << 8);
        u16 reason = WLAN_STATUS_UNSPECIFIED_FAILURE;
        u8 *buf = NULL;
        ulong flags = 0;

        nvt_dbg(CFG80211, "%s, wid=0x%x\n", __func__, wid);

        if (!test_bit(SCAN_PROCESS, &cfg->scan_sts)) {
                nvt_dbg(CFG80211, "no scan processing\n");
                return -EPERM;
        }

        if (wid == WID_EVENT_BSS_INFO) {
                if ((!cfg->scan_request) &&
                        (!test_bit(NVT_IF_CONNECTING, &nvt_if->state_flags))) {
                        nvt_dbg(CFG80211, "no scan request\n");
                        return 0;
                }

                if (cfg->bss_count <= MAX_SCAN_COUNT) {
                        size = msg_len-2+sizeof(struct _nvt_bss_info);
                        ie_len = data[msg_len-4] | (data[msg_len-3] << 8);
                        ie_offset = msg_len-2-ie_len-4;

                        buf = kzalloc(size, GFP_KERNEL);
                        if (buf == NULL) {
                                nvt_dbg(CFG80211, "allocate bss buffer fail\n");
                                return 0;
                        }

                        /* fill bss info */
                        bss = (struct _nvt_bss_info *)buf;
                        bss->length = size;
                        memcpy(bss->bssid, &data[18], ETH_ALEN);
                        bss->ssid_len = strlen(&data[30]);
                        memcpy(bss->ssid, &data[30], bss->ssid_len);
                        bss->channel = data[7];
                        bss->dot11i_info = data[63];
                        bss->rssi = data[9];
                        bss->beacon_period = data[11] | (data[10] << 8);
                        bss->cap_info = data[13] | (data[12] << 8);

                        bss->ie_offset = ie_offset+sizeof(struct _nvt_bss_info);
                        bss->ie_length = ie_len;
                        memcpy(&buf[sizeof(struct _nvt_bss_info)], &data[2],
                                msg_len-2);

                        spin_lock_irqsave(&cfg->bss_list_lock, flags);
                        list_add_tail(&bss->list, &cfg->bss_list);
                        cfg->bss_count++;
                        spin_unlock_irqrestore(&cfg->bss_list_lock, flags);
                } else {
                        nvt_dbg(CFG80211, "bss count[%d] reach max limit\n",
                                cfg->bss_count);

                        return 0;
                }
        } else if (wid == WID_EVENT_SCAN_DONE) {
                if (data[2] == SUCCESS) {
                        if (test_bit(NVT_IF_CONNECTING, &nvt_if->state_flags)) {
                                nvt_scan_complete(cfg->nvt_adapter, 0, 0);

                                if (nvt_find_bss(nvt_if)) {
                                        if (nvt_connect_action(nvt_if) < 0) {
                                                nvt_dbg(CFG80211,
                                                        "connect action err\n");
                                                nvt_bss_connect(nvt_if, reason,
                                                        data);
                                        }
                                } else {
                                        nvt_dbg(CFG80211,
                                                "connect fail, no found bss\n");
                                        nvt_bss_connect(nvt_if, reason, data);
                                }
                        } else if (test_bit(SCAN_PROCESS, &cfg->scan_sts)) {
                                list_for_each_entry(bss, &cfg->bss_list, list) {
                                        ret = nvt_update_bss(cfg, bss);
                                        if (ret < 0) {
                                                nvt_dbg(CFG80211,
                                                        "update fail, ret=%d\n",
                                                         ret);
                                                break;
                                        }
                                }
                                nvt_scan_complete(cfg->nvt_adapter, 0, 0);
                        }
                } else {
                        nvt_scan_complete(cfg->nvt_adapter, 1, 0);
                }
        } else {
                nvt_dbg(CFG80211, "unknown wid=0x%x\n", wid);
        }

        return 0;
}

/**
 * nvt_connect_result - station mode connect result event handle
 *
 * @nvt_if: nvt network interface
 * @msg_len: event message length
 * @data: event data
 *
 * Return: return 0 success
 */
s32 nvt_connect_result(struct _nvt_if *nvt_if, u16 msg_len, u8 *data)
{
        struct _nvt_adapter *nvt_adapter = nvt_if->nvt_adapter;
        struct _nvt_cfg80211 *cfg = nvt_adapter->nvt_cfg80211;
        u16 wid = data[0]+(data[1] << 8);
        u16 reason = WLAN_STATUS_UNSPECIFIED_FAILURE;

        nvt_dbg(CFG80211, "%s, wid=0x%x\n", __func__, wid);

        if (wid == WID_EVENT_CONNECT_INFO) {
                if (test_bit(NVT_IF_CONNECTING, &nvt_if->state_flags)) {
                        if (test_bit(SCAN_PROCESS, &cfg->scan_sts)) {
                                nvt_dbg(CFG80211, "scanning, no connecting\n");
                                return 0;
                        }

                        if (data[2] == SUCCESS) {
                                reason = WLAN_STATUS_SUCCESS;
                                nvt_bss_connect(nvt_if, reason, data);
                                /* Reset BA handlers */
                                nvt_reset_ba_handle(nvt_adapter, 1);
                        } else {
                                nvt_dbg(CFG80211,
                                        "connect result fail, status=%d\n",
                                        data[2]);

                                nvt_bss_connect(nvt_if, reason, data);
                        }
                } else {
                        nvt_dbg(CFG80211,
                                "sme not connecting state, sme=%lu\n",
                                nvt_if->state_flags);
                }
        } else {
                nvt_dbg(CFG80211, "unknown wid=0x%x\n", wid);
        }

        return 0;
}

/**
 * nvt_connect_status_station - station mode disconnect event handle
 *
 * @nvt_if: nvt network interface
 * @msg_len: event message length
 * @data: event data
 *
 * Return: return 0 success
 */
s32 nvt_connect_status_station(struct _nvt_if *nvt_if,
        u16 msg_len, u8 *data)
{
        struct _nvt_adapter *nvt_adapter = nvt_if->nvt_adapter;
        u16 wid = data[0] + (data[1] << 8);

        if (wid == WID_STATUS) {
                if (test_bit(NVT_IF_CONNECTED, &nvt_if->state_flags) &&
                        (data[2] == MAC_DISCONNECTED)) {
                        clear_bit(NVT_IF_CONNECTED, &nvt_if->state_flags);
                        if (nvt_if->filter_disconnect == false) {
                                nvt_scan_init(nvt_adapter->nvt_cfg80211);
                                cfg80211_disconnected(nvt_if->ndev, 0,
                                        NULL, 0, GFP_KERNEL);
                        }
                        atomic_inc(&nvt_if->disconnect_cnt);
                        if (waitqueue_active(&nvt_if->disconnect_wait)) {
                                wake_up(&nvt_if->disconnect_wait);
                        }
                        /* Reset BA handlers */
                        nvt_reset_ba_handle(nvt_adapter, 1);
                }
        }

        return 0;
}

/**
 * nvt_enable_status_ap - ap mode enable/disable event handle
 *
 * @nvt_if: nvt network interface
 * @msg_len: event message length
 * @data: event data
 *
 * Return: return 0 success
 */
s32 nvt_enable_status_ap(struct _nvt_if *nvt_if,
        u16 msg_len, u8 *data)
{
        u16 wid = data[0] + (data[1] << 8);

        if (wid == WID_STATUS) {
                if (data[2] == MAC_CONNECTED) {
                        set_bit(NVT_IF_CONNECTED, &nvt_if->state_flags);
                        nvt_dbg(CFG80211, "Notify AP Enabled\n");
                } else {
                        clear_bit(NVT_IF_CONNECTED, &nvt_if->state_flags);
                        nvt_dbg(CFG80211, "Notify AP Disabled\n");
                }
        }
        return 0;
}

/**
 * nvt_connect_status_ap - ap mode connect and disconnect event handle
 *
 * @nvt_if: nvt network interface
 * @msg_len: event message length
 * @data: event data
 *
 * Return: return 0 success
 */
s32 nvt_connect_status_ap(struct _nvt_if *nvt_if, u16 msg_len, u8 *data)
{
        u16 wid = data[0] + (data[1] << 8);
        struct station_info sinfo;
        struct _nvt_adapter *nvt_adapter = nvt_if->nvt_adapter;
        #ifdef CONFIG_WIFI_TUNING_PHASE_II
        u8 mac_addr[6];
        struct _sta_sts_in_ap *sta_sts_in_ap;
        struct _nvt_priv *nvt_priv = &(nvt_if->nvt_adapter->nvt_priv);
        #endif
        struct _ba_struct_t *ba_ctxt = NULL;
        static s32 generation;
        u8 i = 0;
        u8 idx;

        if (wid == WID_EVENT_ASSOC_REQ) {
                memset(&sinfo, 0, sizeof(sinfo));
#if LINUX_VERSION_CODE < KERNEL_VERSION(4, 0, 0)
                sinfo.filled = STATION_INFO_ASSOC_REQ_IES;
#endif
                sinfo.assoc_req_ies = data + 8;
                sinfo.assoc_req_ies_len = msg_len - 8;
                generation++;
                sinfo.generation = generation;
                cfg80211_new_sta(nvt_if->ndev, data + 2, &sinfo, GFP_KERNEL);
                nvt_dbg(CFG80211, "Notify connect STA Info\n");

                #ifdef CONFIG_WIFI_TUNING_PHASE_II
                memcpy(mac_addr, data + 2, ETH_ALEN);
                sta_sts_in_ap = kzalloc(sizeof(struct _sta_sts_in_ap),
                                        GFP_KERNEL);

                INIT_LIST_HEAD(&sta_sts_in_ap->list);

                if (sta_sts_in_ap != NULL) {
                    nvt_dbg(CLEARVIEW, "%s: Append into sta("
                            "%02x:%02x:%02x:%02x:%02x:%02x) list,%p size:%d\n",
                             __func__, MAC2STR(mac_addr),
                            sta_sts_in_ap, sizeof(struct _sta_sts_in_ap));

                    memset(sta_sts_in_ap, 0, sizeof(struct _sta_sts_in_ap));
                    memcpy(sta_sts_in_ap->mac_addr, mac_addr, ETH_ALEN);

                    list_add_tail(&sta_sts_in_ap->list,
                                   &nvt_priv->sta_list_in_ap);

                    // debug
                   list_for_each_entry(sta_sts_in_ap, &nvt_priv->sta_list_in_ap,
                                       list) {
                       nvt_dbg(CLEARVIEW, "element mac:"
                               "%02x:%02x:%02x:%02x:%02x:%02x\n",
                               MAC2STR(sta_sts_in_ap->mac_addr));
                    }

                } else {
                    nvt_dbg(CLEARVIEW, "%s: sta_sts_in_ap can't be created\n",
                             __func__);
                }
                #endif

                /* Reset BA handlers */
                for (i = 0; i < ASSOC_NUM; i++) {
                        idx = i * NUM_TIDS;
                        ba_ctxt = &(nvt_adapter->nvt_priv.ba_rx[idx]);
                        if (memcmp(ba_ctxt->dst_addr,
                                data + 2, ETH_ALEN) == 0) {
                                nvt_reset_ba_handle(nvt_adapter, i + 1);
                        }
                }
        } else if (wid == WID_STA_JOIN_INFO) {
                cfg80211_del_sta(nvt_if->ndev, data + 3, GFP_KERNEL);
                nvt_dbg(CFG80211, "Notify Disconnect STA Info\n");
                /* Reset BA handlers */
                for (i = 0; i < ASSOC_NUM; i++) {
                        idx = i * NUM_TIDS;
                        ba_ctxt = &(nvt_adapter->nvt_priv.ba_rx[idx]);
                        if (memcmp(ba_ctxt->dst_addr,
                                data + 3, ETH_ALEN) == 0) {
                                nvt_reset_ba_handle(nvt_adapter, i + 1);
                        }
                }
        } else {
                nvt_dbg(CFG80211, "unknown wid=0x%x\n", wid);
        }
        return 0;
}

/**
 * nvt_micfail_status - mic fail event handle
 *
 * @nvt_if: nvt network interface
 * @msg_len: event message length
 * @data: event data
 *
 * Return: return 0 success
 */
s32 nvt_micfail_status(struct _nvt_if *nvt_if, u16 msg_len, u8 *data)
{
        enum nl80211_key_type key_type;
        nvt_dbg(CFG80211, "Notify_MIC_Failure\n");

        if (data[8] == 0) {
                key_type = NL80211_KEYTYPE_GROUP;
        } else {
                key_type = NL80211_KEYTYPE_PAIRWISE;
        }

        cfg80211_michael_mic_failure(nvt_if->ndev,
                &data[2], key_type, -1, NULL, GFP_KERNEL);
        return 0;

}

/**
 * nvt_rx_mgmt_probereq - receive probe request frame from Firmware
 *
 * @nvt_if: nvt network interface
 * @msg_len: event message length
 * @data: event data
 *
 * Return: zero - success.
 *         negative - failure.
 */
s32 nvt_rx_mgmt_probereq(struct _nvt_if *nvt_if, u16 msg_len, u8 *data)
{
        u8 *mgmt_frame;
        u32 mgmt_frame_len;
        s32 freq;
        u16 mgmt_type;

        nvt_dbg(P2P, "%s", __func__);

        /* Check if wpa_supplicant has registered for this frame */
        nvt_dbg(P2P, "mgmt_rx_reg=%04x\n", nvt_if->mgmt_rx_reg);
        mgmt_type = (IEEE80211_STYPE_PROBE_REQ & IEEE80211_FCTL_STYPE) >> 4;
        if ((nvt_if->mgmt_rx_reg & BIT(mgmt_type)) == 0) {
                return 0;
        }

        mgmt_frame = (u8 *)(data + 2);
        /* WID_Len + Freq_Len = 2 + 1 = 3 */
        mgmt_frame_len = msg_len - 3;
        freq = ieee80211_channel_to_frequency(data[msg_len - 1],
                (data[msg_len - 1] <= 14) ?
                IEEE80211_BAND_2GHZ : IEEE80211_BAND_5GHZ);
        nvt_dbg(P2P, "freq = %d\n", freq);

#if LINUX_VERSION_CODE > KERNEL_VERSION(4, 0, 0)
        cfg80211_rx_mgmt(&nvt_if->wdev, freq, 0, mgmt_frame, mgmt_frame_len, 0);
#elif LINUX_VERSION_CODE > KERNEL_VERSION(3, 10, 0)
        cfg80211_rx_mgmt(&nvt_if->wdev, freq, 0, mgmt_frame, mgmt_frame_len, 0,
                GFP_ATOMIC);
#elif LINUX_VERSION_CODE > KERNEL_VERSION(3, 7, 0)
        cfg80211_rx_mgmt(&nvt_if->wdev, freq, 0, mgmt_frame, mgmt_frame_len,
                 GFP_ATOMIC);
#else
        cfg80211_rx_mgmt(nvt_if->ndev, freq, mgmt_frame, mgmt_frame_len,
                 GFP_ATOMIC);
#endif

        return 0;
}

/**
 * nvt_rx_action_frame - receive action frame from Firmware
 *
 * @nvt_if: nvt network interface
 * @msg_len: event message length
 * @data: event data
 *
 * Return: zero - success.
 *         negative - failure.
 */
s32 nvt_rx_action_frame(struct _nvt_if *nvt_if, u16 msg_len, u8 *data)
{
        u8 *mgmt_frame;
        u32 mgmt_frame_len;
        s32 freq;
        u16 mgmt_type;
        s32 ie_offset;
        s32 ie_len;

        nvt_dbg(P2P, "%s", __func__);

        /* Check if wpa_supplicant has registered for this frame */
        nvt_dbg(P2P, "mgmt_rx_reg=%04x\n", nvt_if->mgmt_rx_reg);
        mgmt_type = (IEEE80211_STYPE_ACTION & IEEE80211_FCTL_STYPE) >> 4;
        if ((nvt_if->mgmt_rx_reg & BIT(mgmt_type)) == 0) {
                return 0;
        }

        mgmt_frame = (u8 *)(data + 2);
        /* WID_Len + Freq_Len = 2 + 1 = 3 */
        mgmt_frame_len = msg_len - 3;
        freq = ieee80211_channel_to_frequency(data[msg_len - 1],
                         (data[msg_len - 1] <= 14) ?
                         IEEE80211_BAND_2GHZ : IEEE80211_BAND_5GHZ);
        nvt_dbg(P2P, "freq = %d\n", freq);

        ie_offset = IEEE80211_MGMT_HDR_LEN;
        ie_len = cpu_to_le16(mgmt_frame_len - ie_offset);
        nvt_dbg_p2p(false, &mgmt_frame[ie_offset], ie_len);

//JERRY
#if LINUX_VERSION_CODE > KERNEL_VERSION(4, 0, 0)
        cfg80211_rx_mgmt(&nvt_if->wdev, freq, 0, mgmt_frame, mgmt_frame_len, 0);
#elif LINUX_VERSION_CODE > KERNEL_VERSION(3, 10, 0)
        cfg80211_rx_mgmt(&nvt_if->wdev, freq, 0, mgmt_frame, mgmt_frame_len, 0,
                GFP_ATOMIC);
#elif LINUX_VERSION_CODE > KERNEL_VERSION(3, 7, 0)
        cfg80211_rx_mgmt(&nvt_if->wdev, freq, 0, mgmt_frame, mgmt_frame_len,
                 GFP_ATOMIC);
#else
        cfg80211_rx_mgmt(nvt_if->ndev, freq, mgmt_frame, mgmt_frame_len,
                 GFP_ATOMIC);
#endif

        return 0;
}

static u8 nvt_set_country_info_elem(u8 *data,
                                struct _nvt_802_11d_reg *domain_info)
{
        struct ieee80211_country_ie_triplet *t;
        u8 ch_idx = 0;
        u8 index_len = 0;
        u8 ie_len = 0;
        u8 index = 0;

        /* ignore non-ISO3166 country codes */
        if (domain_info->country_code[0] < 'A' ||
                domain_info->country_code[0] > 'Z' ||
                domain_info->country_code[1] < 'A' ||
                domain_info->country_code[1] > 'Z') {
                nvt_dbg(ERROR, "%s, not a ISO3166 code (0x%02x 0x%02x)\n",
                        __func__, domain_info->country_code[0],
                                domain_info->country_code[1]);
                return 0;
        }

        /* Country Element ID */
        data[index++] = ICOUNTRY;

        /* Save index for length field */
        index_len = index++;

        data[index++] = domain_info->country_code[0];
        data[index++] = domain_info->country_code[1];
        data[index++] = domain_info->country_code[2];

        for (ch_idx = 0; ch_idx < domain_info->no_of_triplet; ch_idx++) {
                t = &domain_info->triplet[ch_idx];
                data[index++] = t->chans.first_channel;
                data[index++] = t->chans.num_channels;
                data[index++] = t->chans.max_power;
        }

        /* Update the length field */
        ie_len = (index - 1 - index_len);

        /* If IE length is not divisible by 2 then add 1byte zero Padding */
        if ((ie_len % 2) == 1) {
                data[index] = 0;
                ie_len += 1;
        }

        data[index_len] = ie_len;
        return IE_HDR_LEN + ie_len;
}

#if LINUX_VERSION_CODE >= KERNEL_VERSION(3, 4, 0)
s32 nvt_cfg80211_bcn_params(struct _nvt_if *nvt_if,
      struct _BSS_CONFIG_INFO *bss_config,
      struct cfg80211_ap_settings *settings)
#else
s32 nvt_cfg80211_bcn_params(struct _nvt_if *nvt_if,
      struct _BSS_CONFIG_INFO *bss_config,
      struct beacon_parameters *settings)
#endif
{
        s32 err = 0;
        struct _nvt_bus *nvt_bus = nvt_if->nvt_adapter->nvt_bus;
        struct _nvt_wlan_conf *wconf = &nvt_if->nvt_wconf;
        struct _nvt_cfg80211 *cfg = nvt_if->nvt_adapter->nvt_cfg80211;
        struct _nvt_802_11d_reg *domain_info = &cfg->domain_reg;
#if LINUX_VERSION_CODE >= KERNEL_VERSION(3, 4, 0)
        int beacon_interval = settings->beacon_interval;
        struct cfg80211_beacon_data *bcon = &settings->beacon;
        const u8 *proberesp_ies = bcon->proberesp_ies;
        size_t proberesp_ies_len = bcon->proberesp_ies_len;
        const u8 *beacon_ies = bcon->beacon_ies;
        size_t beacon_ies_len = bcon->beacon_ies_len;
        const u8 *assocresp_ies = bcon->assocresp_ies;
        size_t assocresp_ies_len = bcon->assocresp_ies_len;
#else
        int beacon_interval = settings->interval;
        const u8 *proberesp_ies = settings->proberesp_ies;
        size_t proberesp_ies_len = settings->proberesp_ies_len;
        const u8 *beacon_ies = settings->beacon_ies;
        size_t beacon_ies_len = settings->beacon_ies_len;
        const u8 *assocresp_ies = settings->assocresp_ies;
        size_t assocresp_ies_len = settings->assocresp_ies_len;
#endif
        u8 *bcn_buf = NULL;
        u8 *probe_rsp_buf = NULL;
        u8 reg_buf[NVT_MAX_TRIPLET_802_11D * 3 + 3] = {0};
        u8 reg_len = 0;

        nvt_dbg(CFG80211, "%s\n", __func__);
        if ((settings->ssid != NULL) && (settings->ssid_len > 0) &&
                (settings->ssid_len <= 32)) {
                memset(bss_config->ssid_le.SSID, 0, 32);
                memcpy(bss_config->ssid_le.SSID, settings->ssid,
                        settings->ssid_len);
                bss_config->ssid_le.SSID_len =
                        cpu_to_le32((u32)settings->ssid_len);
        } else {
                nvt_dbg(ERROR, "[ssid]=%s,[len]=%zu\n", settings->ssid,
                        settings->ssid_len);
                err = -EINVAL;
        }

        switch (settings->hidden_ssid) {
        case NL80211_HIDDEN_SSID_NOT_IN_USE:
                bss_config->hidden_ssid = 0;
                break;
        case NL80211_HIDDEN_SSID_ZERO_LEN:
                bss_config->hidden_ssid = 1;
                break;
        case NL80211_HIDDEN_SSID_ZERO_CONTENTS:
                nvt_dbg(ERROR,
                        "not support hiddle_ssid type [%d]\n",
                        settings->hidden_ssid);
                err = -EINVAL;
                break;
        default:
                bss_config->hidden_ssid = 1;
                break;
        }

        if (err < 0)
                goto exit;

        if (nvt_icfg_lock(nvt_bus) < 0 || nvt_icfg_reset(nvt_bus) < 0) {
                nvt_icfg_unlock(nvt_bus);
                return -EINVAL;
        }

        /*set beacon interval*/
        if (nvt_icfg_add(nvt_bus, NVT_ICFG_SET, WID_BEACON_INTERVAL,
           (u8 *)&beacon_interval, 2) < 0) {
                nvt_icfg_unlock(nvt_bus);
                return -EINVAL;
        }

        /*set dtim period*/
        if (nvt_icfg_add(nvt_bus, NVT_ICFG_SET, WID_DTIM_PERIOD,
           (u8 *)&settings->dtim_period, 1) < 0) {
                nvt_icfg_unlock(nvt_bus);
                return -EINVAL;
        }

        /*set ssid*/
        if (nvt_icfg_add(nvt_bus, NVT_ICFG_SET, WID_SSID,
           (u8 *)&bss_config->ssid_le.SSID, 0) < 0) {
                nvt_icfg_unlock(nvt_bus);
                return -EINVAL;
        }

        /*set hidden ssid*/
        if (nvt_icfg_add(nvt_bus, NVT_ICFG_SET, WID_BCAST_SSID,
           (u8 *)&bss_config->hidden_ssid, 1) < 0) {
                nvt_icfg_unlock(nvt_bus);
                return -EINVAL;
        }
        //probe rsp ies
        // filter p2p IE
        err = nvt_filter_p2p_ie(&proberesp_ies, &proberesp_ies_len);
        if (err < 0)
                goto exit;
        // filter wfd IE
        err = nvt_filter_wfd_ie(&proberesp_ies, &proberesp_ies_len);
        if (err < 0)
                goto exit;

        nvt_dbg(CFG80211, "%s, country_code: %c%c\n", __func__,
                domain_info->country_code[0], domain_info->country_code[1]);

        if (nvt_is_an_alpha2(domain_info->country_code)) {
                nvt_dbg(CFG80211, "construct country element\n");
                reg_len = nvt_set_country_info_elem(reg_buf, domain_info);
        }

        if (((beacon_ies_len > 0) && (beacon_ies != NULL)) &&
                (reg_len != 0)) {
                bss_config->vsie_enable |= 0x01;
                wconf->ctrl_flag |= BEACON_EXTRA_IE_FLAG;
                nvt_dbg(CFG80211, "add country info. in extra beacon ies\n");
                bcn_buf = kzalloc(reg_len + beacon_ies_len, GFP_KERNEL);
                if (bcn_buf == NULL) {
                        nvt_dbg(CFG80211, "alloc bcn buf fail\n");
                        return -ENOMEM;
                }
                memcpy(&bcn_buf[0], &reg_buf[0], reg_len);
                memcpy(&bcn_buf[reg_len], beacon_ies,
                        beacon_ies_len);
                if (nvt_icfg_add(nvt_bus, NVT_ICFG_SET, WID_VSIE_TX_DATA,
                        (u8 *)bcn_buf, reg_len + beacon_ies_len) < 0) {
                        nvt_icfg_unlock(nvt_bus);
                        kfree(bcn_buf);
                        return -EINVAL;
                }
                kfree(bcn_buf);
        } else if ((beacon_ies_len > 0) && (beacon_ies != NULL)) {
                bss_config->vsie_enable |= 0x01;
                wconf->ctrl_flag |= BEACON_EXTRA_IE_FLAG;
                if (nvt_icfg_add(nvt_bus, NVT_ICFG_SET, WID_VSIE_TX_DATA,
                        (u8 *)beacon_ies, beacon_ies_len) < 0) {
                        nvt_icfg_unlock(nvt_bus);
                        return -EINVAL;
                }
        } else if (reg_len != 0) {
                nvt_dbg(CFG80211, "only country info. in extra beacon ies\n");
                bss_config->vsie_enable |= 0x01;
                wconf->ctrl_flag |= BEACON_EXTRA_IE_FLAG;
                if (nvt_icfg_add(nvt_bus, NVT_ICFG_SET, WID_VSIE_TX_DATA,
                        (u8 *)reg_buf, reg_len) < 0) {
                        nvt_icfg_unlock(nvt_bus);
                        return -EINVAL;
                }
        } else {
                nvt_dbg(CFG80211, "no ie in extra beacon ies\n");
        }

        if (((proberesp_ies_len > 0) && (proberesp_ies != NULL)) &&
                (reg_len != 0)) {
                bss_config->vsie_enable |= 0x04;
                wconf->ctrl_flag |= PROBE_RESP_EXTRA_IE_FLAG;
                nvt_dbg(CFG80211, "add country info. in extra prob_rsp ies\n");
                probe_rsp_buf = kzalloc(reg_len + proberesp_ies_len,
                        GFP_KERNEL);
                if (probe_rsp_buf == NULL) {
                        nvt_dbg(CFG80211, "alloc prob_rsp buf fail\n");
                        return -ENOMEM;
                }
                memcpy(&probe_rsp_buf[0], &reg_buf[0], reg_len);
                memcpy(&probe_rsp_buf[reg_len], proberesp_ies,
                                proberesp_ies_len);
                if (nvt_icfg_add(nvt_bus, NVT_ICFG_SET, WID_VSIE_TX_PROBERSP,
                        (u8 *)probe_rsp_buf,
                        reg_len + proberesp_ies_len) < 0) {
                        nvt_icfg_unlock(nvt_bus);
                        kfree(probe_rsp_buf);
                        return -EINVAL;
                }
                kfree(probe_rsp_buf);
        } else if ((proberesp_ies_len > 0) && (proberesp_ies != NULL)) {
                bss_config->vsie_enable |= 0x04;
                wconf->ctrl_flag |= PROBE_RESP_EXTRA_IE_FLAG;
                if (nvt_icfg_add(nvt_bus, NVT_ICFG_SET, WID_VSIE_TX_PROBERSP,
                        (u8 *)proberesp_ies, proberesp_ies_len) < 0) {
                        nvt_icfg_unlock(nvt_bus);
                        return -EINVAL;
                }
        } else if (reg_len != 0) {
                bss_config->vsie_enable |= 0x04;
                wconf->ctrl_flag |= PROBE_RESP_EXTRA_IE_FLAG;
                nvt_dbg(CFG80211, "only country info. in extra prob_rsp ies\n");
                if (nvt_icfg_add(nvt_bus, NVT_ICFG_SET, WID_VSIE_TX_PROBERSP,
                        (u8 *)reg_buf, reg_len) < 0) {
                        nvt_icfg_unlock(nvt_bus);
                        return -EINVAL;
                }
        } else {
                nvt_dbg(CFG80211, "no ie in extra prob_rsp ies\n");
        }

        if ((assocresp_ies_len > 0) && (assocresp_ies != NULL)) {
                bss_config->vsie_enable |= 0x08;
                wconf->ctrl_flag |= ASSOC_RESP_EXTRA_IE_FLAG;
                if (nvt_icfg_add(nvt_bus, NVT_ICFG_SET, WID_VSIE_TX_ASSOCRSP,
                    (u8 *)assocresp_ies, assocresp_ies_len) < 0) {
                        nvt_icfg_unlock(nvt_bus);
                        return -EINVAL;
                }
        }
        /*set hidden ssid*/
        if (nvt_icfg_add(nvt_bus, NVT_ICFG_SET, WID_VSIE_FRAME,
           (u8 *)&bss_config->vsie_enable, 1) < 0) {
                nvt_icfg_unlock(nvt_bus);
                return -EINVAL;
        }
exit:
        if (nvt_icfg_send_and_get_resp(nvt_bus, NULL, 0) < 0) {
                nvt_icfg_unlock(nvt_bus);
                return -EINVAL;
        } else {
                nvt_icfg_unlock(nvt_bus);
        }

        return err;
}

#if LINUX_VERSION_CODE >= KERNEL_VERSION(3, 4, 0)
s32 nvt_cfg80211_ap_ies(struct _nvt_if *nvt_if,
        struct _BSS_CONFIG_INFO *bss_config, struct cfg80211_beacon_data *info,
        struct cfg80211_ap_settings *settings)
#else
s32 nvt_cfg80211_ap_ies(struct _nvt_if *nvt_if,
        struct _BSS_CONFIG_INFO *bss_config,
        struct beacon_parameters *settings)
#endif
{
        u8 *wps_ie, *p2p_ie, *rsn_ie, *wpa_ie;
        u32 i;
        struct _nvt_bus *nvt_bus = nvt_if->nvt_adapter->nvt_bus;
#if LINUX_VERSION_CODE < KERNEL_VERSION(3, 4, 0)
        u8 *tail = settings->tail;
        int tail_len = settings->tail_len;
#else
        const u8 *tail = info->tail;
        int tail_len = info->tail_len;
#endif
        nvt_dbg(CFG80211, "%s\n", __func__);
        if (!settings->privacy) {
                bss_config->security_mode &= ~WID_11I_MODE_Security_ON;
        } else {
                switch (settings->auth_type) {
                case NL80211_AUTHTYPE_OPEN_SYSTEM:
                        bss_config->auth_mode = WID_AUTH_OPEN_PSK;
                        break;
                case NL80211_AUTHTYPE_SHARED_KEY:
                        bss_config->auth_mode = WID_AUTH_SHARED_KEY;
                        break;
                case NL80211_AUTHTYPE_NETWORK_EAP:
                        bss_config->auth_mode = WID_AUTH_802_1X;
                        break;
                case NL80211_AUTHTYPE_AUTOMATIC:
                        nvt_dbg(CFG80211, "NL80211_AUTHTYPE_AUTOMATIC\n");
                        bss_config->auth_mode = WID_AUTH_OPEN_PSK
                                | WID_AUTH_SHARED_KEY;
                        break;
                default:
                        bss_config->auth_mode = WID_AUTH_OPEN_PSK;
                        break;
                }
                bss_config->security_mode |= WID_11I_MODE_Security_ON;

                if (settings->crypto.n_ciphers_pairwise) {
                        switch (settings->crypto.ciphers_pairwise[0]) {
                        case WLAN_CIPHER_SUITE_USE_GROUP:
                                bss_config->security_mode
                                        &= ~WID_11I_MODE_Security_ON;
                                break;
                        case WLAN_CIPHER_SUITE_WEP40:
                                bss_config->security_mode |= WID_11I_MODE_WEP40;
                                break;
                        case WLAN_CIPHER_SUITE_WEP104:
                                bss_config->security_mode
                                        |= WID_11I_MODE_WEP104;
                                break;
                        case WLAN_CIPHER_SUITE_TKIP:
                                 if ((settings->crypto.wpa_versions &
                                      NL80211_WPA_VERSION_1) ||
                                      (settings->crypto.wpa_versions &
                                      NL80211_WPA_VERSION_2))
                                        bss_config->security_mode
                                                |= WID_11I_MODE_TKIP;
                                 break;
                        case WLAN_CIPHER_SUITE_CCMP:
                                 if ((settings->crypto.wpa_versions &
                                      NL80211_WPA_VERSION_1) ||
                                      (settings->crypto.wpa_versions &
                                      NL80211_WPA_VERSION_2))
                                        bss_config->security_mode
                                                |= WID_11I_MODE_CCMP;
                                 break;
                        case WLAN_CIPHER_SUITE_SMS4:
                                bss_config->security_mode
                                        |= WID_11I_SECURITY_ON;
                                bss_config->ext_security_mode
                                        |= (WID_EXT_11I_WAPI |
                                                WID_EXT_11I_SMS4);
                                break;
                        default:
                                 break;
                         }
                }
                switch (settings->crypto.cipher_group) {
                case WLAN_CIPHER_SUITE_USE_GROUP:
                        bss_config->security_mode &= ~WID_11I_MODE_Security_ON;
                        break;
                case WLAN_CIPHER_SUITE_WEP40:
                        bss_config->security_mode |= WID_11I_MODE_WEP40;
                        break;
                case WLAN_CIPHER_SUITE_WEP104:
                        bss_config->security_mode |= WID_11I_MODE_WEP104;
                        break;
                case WLAN_CIPHER_SUITE_TKIP:
                        bss_config->security_mode |= WID_11I_MODE_TKIP;
                        break;
                case WLAN_CIPHER_SUITE_CCMP:
                        bss_config->security_mode |= WID_11I_MODE_CCMP;
                        break;
                case WLAN_CIPHER_SUITE_SMS4:
                        bss_config->security_mode |= WID_11I_SECURITY_ON;
                        bss_config->ext_security_mode |= (WID_EXT_11I_WAPI |
                                                        WID_EXT_11I_SMS4);
                        break;
                default:
                        break;
                }

                for (i = 0; i < settings->crypto.n_akm_suites; i++) {
                        switch (settings->crypto.akm_suites[i]) {
                        case WLAN_AKM_SUITE_8021X:
                                return -EINVAL;
                                break;
                        case WLAN_AKM_SUITE_PSK:
                                if (settings->crypto.wpa_versions &
                                        NL80211_WPA_VERSION_1)
                                        bss_config->wpa_proto = PROTO_WPA1;
                                if (settings->crypto.wpa_versions &
                                        NL80211_WPA_VERSION_2)
                                        bss_config->wpa_proto = PROTO_WPA2;
                                        break;
                        default:
                                break;
                        }
                }

                for (i = 0; i < settings->crypto.n_ciphers_pairwise; i++) {
                        switch (settings->crypto.ciphers_pairwise[i]) {
                        case WLAN_CIPHER_SUITE_TKIP:
                                bss_config->pairwise_cipher = CIPHER_TKIP;
                                break;
                        case WLAN_CIPHER_SUITE_CCMP:
                                bss_config->pairwise_cipher = CIPHER_CCMP;
                                break;
                        case WLAN_CIPHER_SUITE_SMS4:
                                bss_config->pairwise_cipher = CIPHER_SMS4;
                                break;
                        default:
                                break;
                        }
                }
                if (bss_config->ext_security_mode & WID_EXT_11I_WAPI) {
                        bss_config->auth_mode |= WID_EXT_11I_WAPI;
                }
        }

        if (tail && tail_len) {

                /* find the RSN_IE */
                rsn_ie = (u8 *)cfg80211_find_ie(WLAN_EID_RSN, tail, tail_len);

                /* find the WPA_IE */
                wpa_ie = (u8 *)cfg80211_find_vendor_ie(WLAN_OUI_MICROSOFT,
                    WLAN_OUI_TYPE_MICROSOFT_WPA, tail, tail_len);

                if ((wpa_ie != NULL || rsn_ie != NULL)) {
                        if (wpa_ie != NULL) {
                                /* WPA IE */
                                bss_config->security_mode |= WID_11I_MODE_WPA;
                        }
                        if (rsn_ie != NULL) {
                                /* RSN IE */
                                bss_config->security_mode |= WID_11I_MODE_RSN;
                        }
                }
#if LINUX_VERSION_CODE >= KERNEL_VERSION(3, 9, 0)
                if (rsn_ie != NULL) {
                        bss_config->pmf_enable = nvt_pmf_check(rsn_ie,
                                                        rsn_ie+1);
                }
#endif
                /* find the WPS IE */
                wps_ie = (u8 *)cfg80211_find_vendor_ie(WLAN_OUI_MICROSOFT,
                    WLAN_OUI_TYPE_MICROSOFT_WPS, tail, tail_len);

                if (wps_ie != NULL)
                        bss_config->wps_enable = 1;
                else
                        bss_config->wps_enable = 0;

                /* find the P2P IE */
                p2p_ie = (u8 *)cfg80211_find_vendor_ie(WLAN_OUI_WFA,
                    WLAN_OUI_TYPE_WFA_P2P, tail, tail_len);

                if (p2p_ie != NULL) {
                        bss_config->p2p_enable = 1;
                        bss_config->op_11g_mode = OP_11G_HP_MODE;
                } else {
                        bss_config->p2p_enable = 0;
                        if (cfg80211_find_ie(WLAN_EID_EXT_SUPP_RATES, tail
                                , tail_len) != NULL)
                                bss_config->op_11g_mode = OP_11G_COMP_MODE_1;
                        else
                                bss_config->op_11g_mode = OP_11B_ONLY_MODE;
                }
        }

        if (nvt_icfg_lock(nvt_bus) < 0 || nvt_icfg_reset(nvt_bus) < 0) {
                nvt_icfg_unlock(nvt_bus);
                return -EINVAL;
        }

        /*set op mode*/
        if (nvt_icfg_add(nvt_bus, NVT_ICFG_SET, WID_11G_OPERATING_MODE,
                (u8 *)&bss_config->op_11g_mode, 1) < 0) {
                nvt_icfg_unlock(nvt_bus);
                return -EINVAL;
        }

        /*set security*/
        if (nvt_icfg_add(nvt_bus, NVT_ICFG_SET, WID_11I_MODE,
                (u8 *)&bss_config->security_mode, 1) < 0) {
                nvt_icfg_unlock(nvt_bus);
                return -EINVAL;
        }

        if (nvt_icfg_add(nvt_bus, NVT_ICFG_SET, WID_EXT_MODE_11I,
                   (u8 *)&bss_config->ext_security_mode, 1) < 0) {
                        nvt_icfg_unlock(nvt_bus);
                        return -EINVAL;
        }

        if (nvt_icfg_add(nvt_bus, NVT_ICFG_SET, WID_AUTH_TYPE,
                (u8 *)&bss_config->auth_mode, 1) < 0) {
                nvt_icfg_unlock(nvt_bus);
                return -EINVAL;
        }

        /*set pmf capable*/
        if (nvt_icfg_add(nvt_bus, NVT_ICFG_SET, WID_PMF_ENABLE,
                (u8 *)&bss_config->pmf_enable, 1) < 0) {
                nvt_icfg_unlock(nvt_bus);
                return -EINVAL;
        }

        /*set WPS_enable*/
        if (nvt_icfg_add(nvt_bus, NVT_ICFG_SET, WID_WPS_REGISTRATION_START,
                (u8 *)&bss_config->wps_enable, 1) < 0) {
                nvt_icfg_unlock(nvt_bus);
                return -EINVAL;
        }
        /*set P2P_enable*/
        if (nvt_icfg_add(nvt_bus, NVT_ICFG_SET, WID_P2P_ENABLE,
           (u8 *)&bss_config->p2p_enable, 1) < 0) {
                nvt_icfg_unlock(nvt_bus);
                return -EINVAL;
        }

        if (nvt_icfg_send_and_get_resp(nvt_bus, NULL, 0) < 0) {
                nvt_icfg_unlock(nvt_bus);
                return -EINVAL;
        } else {
                nvt_icfg_unlock(nvt_bus);
        }

        return 0;
}

#if LINUX_VERSION_CODE >= KERNEL_VERSION(3, 4, 0)
s32 nvt_cfg80211_set_chan_ie(struct wiphy *wiphy,
        struct _nvt_if *nvt_if ,
        struct _BSS_CONFIG_INFO *bss_config,
        struct cfg80211_ap_settings *settings)
#else
s32 nvt_cfg80211_set_chan_ie(struct wiphy *wiphy,
        struct _nvt_if *nvt_if ,
        struct _BSS_CONFIG_INFO *bss_config,
        struct beacon_parameters *beacon_data)
#endif
{
        s32     err = 0;
        u8      *ht_ie;
        u8      *ssid_ie = 0;
        u8      *capability_info = 0;
        u8      ba_enable = 1;
        struct ieee80211_ht_cap ht_cap;
        const u8 *vendor_ie;
        struct ieee_types_header *wmm_ie = NULL;
        struct Nova_WID_WMM_STA_AC_PARAMS Nova_WMM_AC_PARAMS;
        struct _nvt_bus *nvt_bus = nvt_if->nvt_adapter->nvt_bus;
        struct _nvt_wlan_conf_ap *wconf_ap = &nvt_if->nvt_wconf_ap;
        struct _nvt_adapter *nvt_adapter = nvt_if->nvt_adapter;
        enum ieee80211_band band;

        nvt_dbg(CFG80211, "%s\n", __func__);

#if LINUX_VERSION_CODE >= KERNEL_VERSION(3, 4, 0)
        bss_config->channel =
            ieee80211_frequency_to_channel(settings->chandef.chan->center_freq);
#else
        bss_config->channel =
                    ieee80211_frequency_to_channel(nvt_if->
                    ch_info.chan->center_freq);
#endif
        for (band = 0; band < IEEE80211_NUM_BANDS; band++)
                err = nvt_handle_reg_channel(nvt_adapter,
                                wiphy, wiphy->bands[band]);
        if (err < 0) {
                return err;
        }

        if (nvt_icfg_lock(nvt_bus) < 0 || nvt_icfg_reset(nvt_bus) < 0) {
                nvt_icfg_unlock(nvt_bus);
                return -EINVAL;
        }

#if LINUX_VERSION_CODE >= KERNEL_VERSION(3, 4, 0)
        bss_config->channel_type =
                cfg80211_get_chandef_type(&settings->chandef);
#else
        bss_config->channel_type = nvt_if->ch_info.channel_type;
#endif
        if (bss_config->channel_type == NL80211_CHAN_NO_HT) {
                if (bss_config->channel > 14)
                        bss_config->channel |= BAND_5G;
                else
                        bss_config->channel |= BAND_2G;

                bss_config->bw40_enable = 0;
                bss_config->op_11n_enable = 0;
                /*if (bss_config->op_11g_mode == 0) {
                        bss_config->preamble = PREAMBLE_LONG;
                        bss_config->slot = 0;
                } else {
                        bss_config->preamble = PREAMBLE_AUTO;
                        bss_config->slot = 1;
                }*/
        } else {
                if (bss_config->channel > 14)
                        bss_config->channel |= BAND_5G;
                else
                        bss_config->channel |= BAND_2G;

                if (bss_config->channel_type == NL80211_CHAN_HT20) {
                        bss_config->bw40_enable = 0;
                } else if (bss_config->channel_type == NL80211_CHAN_HT40PLUS) {
                        bss_config->bw40_enable = 1;
                        bss_config->channel |= SCA;
                } else if (bss_config->channel_type == NL80211_CHAN_HT40MINUS) {
                        bss_config->bw40_enable = 1;
                        bss_config->channel |= SCB;
                } else {
                        nvt_dbg(ERROR,
                                "chane type error [%x], channel [%d]\n",
                                bss_config->channel_type, bss_config->channel);
                        err = -EINVAL;
                }

#if LINUX_VERSION_CODE >= KERNEL_VERSION(3, 4, 0)
                ht_ie = (u8 *)cfg80211_find_ie(WLAN_EID_HT_CAPABILITY,
                    settings->beacon.tail, settings->beacon.tail_len);
#else
                ht_ie = (u8 *)cfg80211_find_ie(WLAN_EID_HT_CAPABILITY,
                    beacon_data->tail, beacon_data->tail_len);
#endif
                if (ht_ie) {
                        bss_config->sgi = LGI;

                        memcpy(&ht_cap, ht_ie+2,
                                sizeof(struct ieee80211_ht_cap));
                        if (ht_cap.cap_info & IEEE80211_HT_CAP_SGI_20) {
                                bss_config->sgi = SGI_20;
                        }
                        if (ht_cap.cap_info & IEEE80211_HT_CAP_SGI_40) {
                                bss_config->sgi |= SGI_40;
                        }
                        switch (ht_cap.cap_info & IEEE80211_HT_CAP_RX_STBC) {
                        case NVT_RX_STBC1:
                                bss_config->stbc |= (RX_STBC_1 << 1);
                                break;
                        case NVT_RX_STBC12:
                                bss_config->stbc |= (RX_STBC_2 << 1);
                                break;
                        case NVT_RX_STBC123:
                                bss_config->stbc |= (RX_STBC_3 << 1);
                                break;
                        default:
                                break;
                        }
                        if (ht_cap.cap_info & IEEE80211_HT_CAP_TX_STBC) {
                                bss_config->stbc |= TX_STBC;
                        }
                        if (ht_cap.cap_info & IEEE80211_HT_CAP_GRN_FLD) {
                                bss_config->greenfield = 1;
                        }
                } else {
                        bss_config->sgi = LGI;
                }

                bss_config->op_11n_enable = 1;
                bss_config->preamble = PREAMBLE_AUTO;
                bss_config->slot = 1;
        }
        /* Check Auto Channel Selection */
        if (wconf_ap->acs_channel != 0xFF) {
                /* apply iwpriv acs_channel value instead of hostapd conf. */
                bss_config->channel =
                        (bss_config->channel & 0xFF00) | wconf_ap->acs_channel;
                nvt_dbg(CFG80211, "%s:apply iwpriv ch=%d\n",
                                __func__, bss_config->channel);
                nvt_dbg(CFG80211, "%s:cs_mask=0x%x\n", __func__,
                        wconf_ap->acs_mask);
        } else {
                /* apply hostapd conf.'s channel */
                nvt_dbg(CFG80211, "%s:apply hostapd conf.'s ch=%d\n",
                                __func__, bss_config->channel);
        }
        // get wmm parameter
#if LINUX_VERSION_CODE >= KERNEL_VERSION(3, 4, 0)
        vendor_ie = cfg80211_find_vendor_ie(WLAN_OUI_MICROSOFT,
                    WLAN_OUI_TYPE_MICROSOFT_WMM, settings->beacon.tail,
                    settings->beacon.tail_len);
#else
        vendor_ie = cfg80211_find_vendor_ie(WLAN_OUI_MICROSOFT,
                        WLAN_OUI_TYPE_MICROSOFT_WMM, beacon_data->tail,
                        beacon_data->tail_len);
#endif
        if (vendor_ie) {
                bss_config->qos_enable = 1;
                wmm_ie = (struct ieee_types_header *)vendor_ie;

                memcpy(&bss_config->wmm_info, wmm_ie + 1,
                        sizeof(struct wmm_info));

                Nova_WMM_AC_PARAMS.AC_BE.TXOP =
                bss_config->wmm_info.ac_params[0].tx_op_limit;

                Nova_WMM_AC_PARAMS.AC_BE.TXOP =
                Nova_WMM_AC_PARAMS.AC_BE.TXOP << 5;

                Nova_WMM_AC_PARAMS.AC_BE.CWmax =
                (bss_config->wmm_info.ac_params[0].ecw_bitmap & 0xF0) >> 4;

                Nova_WMM_AC_PARAMS.AC_BE.CWmin =
                bss_config->wmm_info.ac_params[0].ecw_bitmap & 0x0F;

                Nova_WMM_AC_PARAMS.AC_BE.AIFSN =
                bss_config->wmm_info.ac_params[0].aci_aifsn_bitmap & 0x0F;

                Nova_WMM_AC_PARAMS.AC_BE.ACM =
                (bss_config->wmm_info.ac_params[0].aci_aifsn_bitmap &
                0x10) >> 4;

                Nova_WMM_AC_PARAMS.AC_BK.TXOP =
                bss_config->wmm_info.ac_params[1].tx_op_limit;

                Nova_WMM_AC_PARAMS.AC_BK.TXOP =
                Nova_WMM_AC_PARAMS.AC_BK.TXOP << 5;

                Nova_WMM_AC_PARAMS.AC_BK.CWmax =
                (bss_config->wmm_info.ac_params[1].ecw_bitmap & 0xF0) >> 4;

                Nova_WMM_AC_PARAMS.AC_BK.CWmin =
                bss_config->wmm_info.ac_params[1].ecw_bitmap & 0x0F;

                Nova_WMM_AC_PARAMS.AC_BK.AIFSN =
                bss_config->wmm_info.ac_params[1].aci_aifsn_bitmap & 0x0F;

                Nova_WMM_AC_PARAMS.AC_BK.ACM =
                (bss_config->wmm_info.ac_params[1].aci_aifsn_bitmap &
                0x10) >> 4;

                Nova_WMM_AC_PARAMS.AC_VI.TXOP =
                bss_config->wmm_info.ac_params[2].tx_op_limit;

                Nova_WMM_AC_PARAMS.AC_VI.TXOP =
                Nova_WMM_AC_PARAMS.AC_VI.TXOP << 5;

                Nova_WMM_AC_PARAMS.AC_VI.CWmax =
                (bss_config->wmm_info.ac_params[2].ecw_bitmap & 0xF0) >> 4;

                Nova_WMM_AC_PARAMS.AC_VI.CWmin =
                bss_config->wmm_info.ac_params[2].ecw_bitmap & 0x0F;

                Nova_WMM_AC_PARAMS.AC_VI.AIFSN =
                bss_config->wmm_info.ac_params[2].aci_aifsn_bitmap & 0x0F;

                Nova_WMM_AC_PARAMS.AC_VI.ACM =
                (bss_config->wmm_info.ac_params[2].aci_aifsn_bitmap &
                0x10) >> 4;

                Nova_WMM_AC_PARAMS.AC_VO.TXOP =
                bss_config->wmm_info.ac_params[3].tx_op_limit;

                Nova_WMM_AC_PARAMS.AC_VO.TXOP =
                Nova_WMM_AC_PARAMS.AC_VO.TXOP << 5;

                Nova_WMM_AC_PARAMS.AC_VO.CWmax =
                (bss_config->wmm_info.ac_params[3].ecw_bitmap & 0xF0) >> 4;

                Nova_WMM_AC_PARAMS.AC_VO.CWmin =
                bss_config->wmm_info.ac_params[3].ecw_bitmap & 0x0F;

                Nova_WMM_AC_PARAMS.AC_VO.AIFSN =
                bss_config->wmm_info.ac_params[3].aci_aifsn_bitmap & 0x0F;

                Nova_WMM_AC_PARAMS.AC_VO.ACM =
                (bss_config->wmm_info.ac_params[3].aci_aifsn_bitmap &
                0x10) >> 4;

        } else {
                bss_config->qos_enable = 0;
        }

        /* find the SSID_IE */
#if LINUX_VERSION_CODE >= KERNEL_VERSION(3, 4, 0)
        ssid_ie = (u8 *)cfg80211_find_ie(WLAN_EID_SSID, settings->beacon.head +
                    DOT_80211_BEACON_HEADER +
                    DOT_80211_WIRELESS_MGMT_FIX_HEADER,
                    settings->beacon.head_len);
#else
        ssid_ie = (u8 *)cfg80211_find_ie(WLAN_EID_SSID, beacon_data->head +
                    DOT_80211_BEACON_HEADER +
                    DOT_80211_WIRELESS_MGMT_FIX_HEADER, beacon_data->head_len);
#endif
        capability_info = ssid_ie - 2;

        if (capability_info[0] & 0x20) {
                // short preamble bit
                nvt_dbg(CFG80211, "short preamble bit is enable\n");
                bss_config->preamble = PREAMBLE_AUTO;
        } else {
                nvt_dbg(CFG80211, "short preamble bit is disable\n");
                bss_config->preamble = PREAMBLE_LONG;
        }
        if (capability_info[1] & 0x04) {
                // short slot time bit
                nvt_dbg(CFG80211, "short slot time bit is enable\n");
                bss_config->slot = 1;
        } else {
                nvt_dbg(CFG80211, "short slot time bit is disable\n");
                bss_config->slot = 0;
        }

#if 0
        if (ba_enable > 3) {
                // user input to enable/disable immediate block ack bit
                nvt_dbg(CFG80211, "User disable immediate block ack\n");
                ba_enable = 0;
        } else {
                nvt_dbg(CFG80211, "immediate block ack is enable\n");
                ba_enable = 1;
        }
#endif

        /*set ba*/
        if (nvt_icfg_add(nvt_bus, NVT_ICFG_SET, WID_11N_IMMEDIATE_BA_ENABLED,
           (u8 *)&ba_enable, 1) < 0) {
                nvt_icfg_unlock(nvt_bus);
                return -EINVAL;
        }
        /*set preamble*/
        if (nvt_icfg_add(nvt_bus, NVT_ICFG_SET, WID_PREAMBLE,
           (u8 *)&bss_config->preamble, 1) < 0) {
                nvt_icfg_unlock(nvt_bus);
                return -EINVAL;
        }
        /*set shortslot*/
        if (nvt_icfg_add(nvt_bus, NVT_ICFG_SET, WID_SHORT_SLOT_ALLOWED,
           (u8 *)&bss_config->slot, 1) < 0) {
                nvt_icfg_unlock(nvt_bus);
                return -EINVAL;
        }
        /*set wmm*/
        if (nvt_icfg_add(nvt_bus, NVT_ICFG_SET, WID_QOS_ENABLE,
           (u8 *)&bss_config->qos_enable, 1) < 0) {
                nvt_icfg_unlock(nvt_bus);
                return -EINVAL;
        }

        if (wmm_ie)
                if (nvt_icfg_add(nvt_bus, NVT_ICFG_SET, WID_WMM_STA_AC_PARAMS,
                    (u8 *)&Nova_WMM_AC_PARAMS,
                    sizeof(Nova_WMM_AC_PARAMS)) < 0) {
                        nvt_icfg_unlock(nvt_bus);
                        return -EINVAL;
                }
        //channel and 11n
        if (nvt_icfg_add(nvt_bus, NVT_ICFG_SET, WID_2040_ENABLE,
           (u8 *)&bss_config->bw40_enable, 1) < 0) {
                nvt_icfg_unlock(nvt_bus);
                return -EINVAL;
        }

        if (nvt_icfg_add(nvt_bus, NVT_ICFG_SET, WID_11N_ENABLE,
           (u8 *)&bss_config->op_11n_enable, 1) < 0) {
                nvt_icfg_unlock(nvt_bus);
                return -EINVAL;
        }
        //channel
        if (nvt_icfg_add(nvt_bus, NVT_ICFG_SET, WID_USER_PREF_CHANNEL,
                (u8 *)&bss_config->channel, 2) < 0) {
                nvt_icfg_unlock(nvt_bus);
                return -EINVAL;
        }
        //acs_mask
        if (wconf_ap->acs_channel == 0x00) {
                if (nvt_icfg_add(nvt_bus, NVT_ICFG_SET, WID_ACS_MASK,
                        (u8 *)&wconf_ap->acs_mask,
                        sizeof(wconf_ap->acs_mask)) < 0) {
                        nvt_icfg_unlock(nvt_bus);
                        return -EINVAL;
                }
        }
        //acs_weight
        if (wconf_ap->acs_weight[0] != 0xFF) {
                if (nvt_icfg_add(nvt_bus, NVT_ICFG_SET, WID_SET_ACS_WEIGHT,
                        (u8 *)wconf_ap->acs_weight,
                        sizeof(wconf_ap->acs_weight)) < 0) {
                        nvt_icfg_unlock(nvt_bus);
                        return -EINVAL;
                }
        }
        //Greenfield
        if (nvt_icfg_add(nvt_bus, NVT_ICFG_SET, WID_ENABLE_GREENFIELD,
           (u8 *)&bss_config->greenfield, 1) < 0) {
                nvt_icfg_unlock(nvt_bus);
                return -EINVAL;
        }
        //SGI
        if (nvt_icfg_add(nvt_bus, NVT_ICFG_SET, WID_11N_SHORT_GI_ENABLE,
           (u8 *)&bss_config->sgi, 1) < 0) {
                nvt_icfg_unlock(nvt_bus);
                return -EINVAL;
        }
        //STBC
        if (nvt_icfg_add(nvt_bus, NVT_ICFG_SET, WID_ENABLE_STBC,
           (u8 *)&bss_config->stbc, 1) < 0) {
                nvt_icfg_unlock(nvt_bus);
                return -EINVAL;
        }
        if (nvt_icfg_send_and_get_resp(nvt_bus, NULL, 0) < 0) {
                nvt_icfg_unlock(nvt_bus);
                return -EINVAL;
        } else {
                nvt_icfg_unlock(nvt_bus);
        }

        return err;
}

#if LINUX_VERSION_CODE >= KERNEL_VERSION(3, 4, 0)
static s32 nvt_cfg80211_start_ap(struct wiphy *wiphy, struct net_device *ndev,
                          struct cfg80211_ap_settings *settings)
{
        struct _nvt_if *nvt_if = netdev_priv(ndev);
        struct _BSS_CONFIG_INFO bss_config;
        struct cfg80211_beacon_data *bcon = &settings->beacon;
        struct _nvt_bus *nvt_bus = nvt_if->nvt_adapter->nvt_bus;
        #ifdef CONFIG_WIFI_TUNING_PHASE_II
        struct _nvt_priv *nvt_priv = &(nvt_if->nvt_adapter->nvt_priv);
        #endif
        s32 err = 0;
        u32 dev_role = 0;
        u8 enable = 1;
        dev_role = nvt_if->wdev.iftype;
        nvt_dbg(CFG80211, "cfg80211_start_ap\n");
        memset(&bss_config, 0, sizeof(struct _BSS_CONFIG_INFO));
        g_start_op = DO_RESTART;
        if (dev_role == NL80211_IFTYPE_AP ||
                dev_role == NL80211_IFTYPE_P2P_GO) {
                bss_config.bss_mode = AP;
        } else {
                nvt_dbg(CFG80211, "not support dev role [%x]\n",
                        dev_role);
                        return -EINVAL;
        }
        if (nvt_cfg80211_bcn_params(nvt_if, &bss_config, settings) < 0)
                nvt_dbg(ERROR, "nvt_cfg80211_bcn_params error!!!\n");
        else if (nvt_cfg80211_ap_ies(
                        nvt_if, &bss_config, bcon, settings) < 0)
                nvt_dbg(ERROR, "nvt_cfg80211_ap_ies error!!!\n");
        else if (nvt_cfg80211_set_chan_ie(
                        wiphy, nvt_if, &bss_config, settings) < 0)
                nvt_dbg(ERROR, "nvt_cfg80211_set_chan_ie error!!!\n");
        else if (nvt_icfg_lock(nvt_bus) < 0 ||
                 nvt_icfg_reset(nvt_bus) < 0 ||
                 nvt_icfg_add(nvt_bus, NVT_ICFG_SET,
                             WID_AP_ENABLE, &enable, 1) < 0 ||
                 nvt_icfg_add(nvt_bus, NVT_ICFG_SET,
                             WID_START_OP, &g_start_op, 1) < 0 ||
                 nvt_icfg_send_and_get_resp(nvt_bus, NULL, 0) < 0) {
                        nvt_dbg(ERROR, "AP ENABLE Failed!!!\n");
                        nvt_icfg_unlock(nvt_bus);
        } else {
                nvt_dbg(CFG80211, "AP ENABLE Sucess!!!,ap_start_evt=%d\n",
                                nvt_if->wait_ap_start_event);
                nvt_icfg_unlock(nvt_bus);
                if (nvt_if->wait_ap_start_event == 0) {
                        set_bit(NVT_IF_CONNECTED, &nvt_if->state_flags);
                }
                nvt_if->start_ap_flag = 1;

                #ifdef CONFIG_WIFI_TUNING_PHASE_II
                INIT_LIST_HEAD(&nvt_priv->sta_list_in_ap);
                #endif
        }

        return err;
}

static s32 nvt_cfg80211_stop_ap(struct wiphy *wiphy, struct net_device *ndev)
{
        u8 enable = 0;
        struct _nvt_if *nvt_if = netdev_priv(ndev);
        struct _nvt_bus *nvt_bus = nvt_if->nvt_adapter->nvt_bus;
        struct _nvt_adapter *nvt_adapter = nvt_if->nvt_adapter;
        #ifdef CONFIG_WIFI_TUNING_PHASE_II
        struct _nvt_priv *nvt_priv = &(nvt_adapter->nvt_priv);
        struct _sta_sts_in_ap *sta_sts_in_ap;
        #endif
        u8 i = 0;
        g_start_op = DO_RESET;

        nvt_if->wait_ap_start_event = 0;
        nvt_if->start_ap_flag = 0;
        nvt_if->nvt_wconf_ap.acs_channel = 0xFF;
        nvt_if->nvt_wconf_ap.acs_mask = 0;
        nvt_if->nvt_wconf_ap.acs_weight[0] = 0xFF;
        clear_bit(NVT_IF_CONNECTED, &nvt_if->state_flags);

        if (nvt_icfg_lock(nvt_bus) < 0 ||
           nvt_icfg_reset(nvt_bus) < 0 ||
           nvt_icfg_add(nvt_bus, NVT_ICFG_SET,
                        WID_AP_ENABLE, &enable, 1) < 0 ||
           nvt_icfg_add(nvt_bus, NVT_ICFG_SET,
                        WID_START_OP, &g_start_op, 1) < 0 ||
           nvt_icfg_send_and_get_resp(nvt_bus, NULL, 0) < 0)
                nvt_icfg_unlock(nvt_bus);
        else {
                nvt_dbg(CFG80211, "%s\n", __func__);
                nvt_icfg_unlock(nvt_bus);
        }

        #ifdef CONFIG_WIFI_TUNING_PHASE_II
        // Recycle the all sta info stored in sta_list
        list_for_each_entry(sta_sts_in_ap, &nvt_priv->sta_list_in_ap, list) {
            nvt_dbg(CLEARVIEW, "%s: remove element:%p\n", __func__,
                      sta_sts_in_ap);
            list_del(&sta_sts_in_ap->list);
            kfree(sta_sts_in_ap);
        }
        #endif

        /* Reset BA handlers */
        for (i = 0; i < ASSOC_NUM; i++) {
                nvt_reset_ba_handle(nvt_adapter, i + 1);
        }
        return 0;
}
//AT Define AP related CFG80211 ops for GM Linux Kenel V3.3
#else
static int nvt_cfg80211_set_channel(struct wiphy *wiphy,
        struct net_device *ndev, struct ieee80211_channel *chan,
        enum nl80211_channel_type channel_type) {

        struct _nvt_if *nvt_if = netdev_priv(ndev);
        if (nvt_if == 0)
                return -EIO;
        nvt_if->ch_info.chan = chan;
        nvt_if->ch_info.channel_type = channel_type;
        return 0;
}

static int nvt_cfg80211_add_beacon(struct wiphy *wiphy,
        struct net_device *ndev,
        struct beacon_parameters *info)
{
        struct _nvt_if *nvt_if = netdev_priv(ndev);
        struct _BSS_CONFIG_INFO bss_config;
        struct _nvt_bus *nvt_bus = nvt_if->nvt_adapter->nvt_bus;
        #ifdef CONFIG_WIFI_TUNING_PHASE_II
        struct _nvt_priv *nvt_priv = &(nvt_if->nvt_adapter->nvt_priv);
        #endif
        s32 err = 0;
        u32 dev_role = 0;
        u8 enable = 1;
        dev_role = nvt_if->wdev.iftype;
        nvt_dbg(CFG80211, "cfg80211_start_ap\n");
        memset(&bss_config, 0, sizeof(struct _BSS_CONFIG_INFO));
        g_start_op = DO_RESTART;
        if (dev_role == NL80211_IFTYPE_AP ||
                dev_role == NL80211_IFTYPE_P2P_GO) {
                bss_config.bss_mode = AP;
        } else {
                nvt_dbg(CFG80211, "not support dev role [%x]\n",
                        dev_role);
                        return -EINVAL;
        }
        if (nvt_cfg80211_bcn_params(nvt_if, &bss_config, info) < 0)
                nvt_dbg(ERROR, "nvt_cfg80211_bcn_params error!!!\n");
        else if (nvt_cfg80211_ap_ies(nvt_if, &bss_config, info) < 0)
                nvt_dbg(ERROR, "nvt_cfg80211_ap_ies error!!!\n");
        else if (nvt_cfg80211_set_chan_ie(wiphy, nvt_if,
                    &bss_config, info) < 0)
                nvt_dbg(ERROR, "nvt_cfg80211_set_chan_ie error!!!\n");
        else if (nvt_icfg_lock(nvt_bus) < 0 ||
                 nvt_icfg_reset(nvt_bus) < 0 ||
                 nvt_icfg_add(nvt_bus, NVT_ICFG_SET,
                             WID_AP_ENABLE, &enable, 1) < 0 ||
                 nvt_icfg_add(nvt_bus, NVT_ICFG_SET,
                             WID_START_OP, &g_start_op, 1) < 0 ||
                 nvt_icfg_send_and_get_resp(nvt_bus, NULL, 0) < 0) {
                        nvt_dbg(ERROR, "AP ENABLE Failed!!!\n");
                        nvt_icfg_unlock(nvt_bus);
        } else {
                nvt_dbg(CFG80211, "AP ENABLE Sucess!!!,ap_start_evt=%d\n",
                                nvt_if->wait_ap_start_event);
                nvt_icfg_unlock(nvt_bus);
                if (nvt_if->wait_ap_start_event == 0) {
                        set_bit(NVT_IF_CONNECTED, &nvt_if->state_flags);
                }
                nvt_if->start_ap_flag = 1;

                #ifdef CONFIG_WIFI_TUNING_PHASE_II
                nvt_dbg(CLEARVIEW, "init sta_list_in_ap\n");
                INIT_LIST_HEAD(&nvt_priv->sta_list_in_ap);
                #endif
        }

        return err;
}

static int nvt_cfg80211_set_beacon(struct wiphy *wiphy,
        struct net_device *ndev,
        struct beacon_parameters *bcon)
{
        u8 *wps_ie, *p2p_ie, *rsn_ie, *wpa_ie;
        struct _nvt_if *nvt_if = netdev_priv(ndev);
        struct _BSS_CONFIG_INFO bss_config;
        struct _nvt_wlan_conf *wconf = &nvt_if->nvt_wconf;
        struct _nvt_bus *nvt_bus = nvt_if->nvt_adapter->nvt_bus;
        struct nvt_tlv *ssid_ie;
        u8 vsie_enable = 0;
        u8 *capability_info = 0;
        u8 ie_offset;
        s32 err = 0;
        struct _nvt_cfg80211 *cfg = nvt_if->nvt_adapter->nvt_cfg80211;
        struct _nvt_802_11d_reg *domain_info = &cfg->domain_reg;
        u8 *bcn_buf = NULL;
        u8 *probe_rsp_buf = NULL;
        u8 reg_buf[NVT_MAX_TRIPLET_802_11D * 3 + 3] = {0};
        u8 reg_len = 0;

        nvt_dbg(CFG80211, "%s\n", __func__);

        //clean ap relative flags
        wconf->ctrl_flag &= ~(BEACON_EXTRA_IE_FLAG|PROBE_RESP_EXTRA_IE_FLAG|
        ASSOC_RESP_EXTRA_IE_FLAG);

        memset(&bss_config, 0, sizeof(struct  _BSS_CONFIG_INFO));

        /* find the SSID_IE */
        ie_offset = DOT11_MGMT_HDR_LEN + DOT11_BCN_PRB_FIXED_LEN;
        ssid_ie = nvt_parse_tlvs((u8 *)&bcon->head[ie_offset],
                bcon->head_len - ie_offset, WLAN_EID_SSID);

        if (!ssid_ie)
                return -EINVAL;

        bss_config.ssid_le.SSID_len = ssid_ie->len;
        memcpy(bss_config.ssid_le.SSID, ssid_ie->data, ssid_ie->len);

        // get capability info
        capability_info = (u8 *)ssid_ie;
        capability_info = capability_info - 2;

        /* find the WPS IE */
        wps_ie = (u8 *)cfg80211_find_vendor_ie(WLAN_OUI_MICROSOFT,
                                                    WLAN_OUI_TYPE_MICROSOFT_WPS,
                                                    bcon->tail,
                                                    bcon->tail_len);
        if (wps_ie != NULL)
                bss_config.wps_enable = 1;
        else
                bss_config.wps_enable = 0;
        if (wps_ie != NULL)
                nvt_dbg(CFG80211, "[%s]WPS_OK\n", __func__);
        /* find the P2P IE */
        p2p_ie = (u8 *)cfg80211_find_vendor_ie(WLAN_OUI_WFA,
                                                    WLAN_OUI_TYPE_WFA_P2P,
                                                    bcon->tail,
                                                    bcon->tail_len);
        if (p2p_ie != NULL)
                bss_config.p2p_enable = 1;
        else
                bss_config.p2p_enable = 0;

        /* find the RSN_IE */
        rsn_ie = (u8 *)cfg80211_find_ie(WLAN_EID_RSN,
                       bcon->tail, bcon->tail_len);

        /* find the WPA_IE */
        wpa_ie = (u8 *)cfg80211_find_vendor_ie(WLAN_OUI_MICROSOFT,
                                         WLAN_OUI_TYPE_MICROSOFT_WPA,
                                         bcon->tail, bcon->tail_len);

        if ((wpa_ie != NULL || rsn_ie != NULL)) {
                if (wpa_ie != NULL) {
                        err = nvt_configure_wpaie((struct nvt_vs_tlv *)wpa_ie,
                                false, &bss_config.security_mode,
                                &bss_config.auth_mode);
                        /* WPA IE */
                        bss_config.security_mode |= WID_11I_MODE_WPA;
                        nvt_dbg(CFG80211, "WPA_IE OK\n");
                        if (err < 0)
                                goto exit;
                }
                if (rsn_ie != NULL) {
                        err = nvt_configure_wpaie((struct nvt_vs_tlv *)rsn_ie,
                                true, &bss_config.security_mode,
                                &bss_config.auth_mode);
                        /* RSN IE */
                        bss_config.security_mode |= WID_11I_MODE_RSN;
                        if (err < 0)
                                goto exit;
                }
         } else {
                if (capability_info[0] & 0x10) {
                        // privacy flag
                        err = -EINVAL;
                        goto exit;
                } else {
                        bss_config.auth_mode = WID_AUTH_OPEN_PSK;
                        bss_config.security_mode = 0;
                }
        }
        if (nvt_icfg_lock(nvt_bus) < 0 || nvt_icfg_reset(nvt_bus) < 0) {
                nvt_icfg_unlock(nvt_bus);
                return -EINVAL;
        }

        //ssid
        if (nvt_icfg_add(nvt_bus, NVT_ICFG_SET, WID_SSID,
           (u8 *)&bss_config.ssid_le.SSID, 0) < 0) {
                nvt_icfg_unlock(nvt_bus);
                return -EINVAL;
        }

        //security
        if (nvt_icfg_add(nvt_bus, NVT_ICFG_SET, WID_11I_MODE,
           (u8 *)&bss_config.security_mode, 1) < 0) {
                nvt_icfg_unlock(nvt_bus);
                return -EINVAL;
        }

        if (nvt_icfg_add(nvt_bus, NVT_ICFG_SET, WID_AUTH_TYPE,
            (u8 *)&bss_config.auth_mode, 1) < 0) {
                nvt_icfg_unlock(nvt_bus);
                return -EINVAL;
        }

        // WPS_enable
        if (nvt_icfg_add(nvt_bus, NVT_ICFG_SET, WID_WPS_REGISTRATION_START,
           (u8 *)&bss_config.wps_enable, 1) < 0) {
                nvt_icfg_unlock(nvt_bus);
                return -EINVAL;
        }

        // P2P_enable
        if (nvt_icfg_add(nvt_bus, NVT_ICFG_SET, WID_P2P_ENABLE,
           (u8 *)&bss_config.p2p_enable, 1) < 0) {
                nvt_icfg_unlock(nvt_bus);
                return -EINVAL;
        }

        if (nvt_icfg_send_and_get_resp(nvt_bus, NULL, 0) < 0) {
                nvt_icfg_unlock(nvt_bus);
                return -EINVAL;
        } else {
                nvt_icfg_unlock(nvt_bus);
        }

        if (nvt_icfg_lock(nvt_bus) < 0 || nvt_icfg_reset(nvt_bus) < 0) {
                nvt_icfg_unlock(nvt_bus);
                return -EINVAL;
        }

        nvt_dbg(CFG80211, "%s, country_code: %c%c\n", __func__,
                        domain_info->country_code[0],
                        domain_info->country_code[1]);

        if (nvt_is_an_alpha2(domain_info->country_code)) {
                nvt_dbg(CFG80211, "construct country element\n");
                reg_len = nvt_set_country_info_elem(reg_buf, domain_info);
        }

        //beacon ies
        if (((bcon->beacon_ies_len > 0) && (bcon->beacon_ies != NULL)) &&
                (reg_len != 0)) {
                vsie_enable |= 0x01;
                wconf->ctrl_flag |= BEACON_EXTRA_IE_FLAG;
                nvt_dbg(CFG80211, "add country info. in extra beacon ies\n");
                bcn_buf = kzalloc(reg_len + bcon->beacon_ies_len, GFP_KERNEL);
                if (bcn_buf == NULL) {
                        nvt_dbg(CFG80211, "alloc bcn buf fail\n");
                        return -ENOMEM;
                }
                memcpy(&bcn_buf[0], &reg_buf[0], reg_len);
                memcpy(&bcn_buf[reg_len], bcon->beacon_ies,
                                bcon->beacon_ies_len);
                if (nvt_icfg_add(nvt_bus, NVT_ICFG_SET, WID_VSIE_TX_DATA,
                        (u8 *)bcn_buf, reg_len + bcon->beacon_ies_len) < 0) {
                        nvt_icfg_unlock(nvt_bus);
                        kfree(bcn_buf);
                        return -EINVAL;
                }
                kfree(bcn_buf);
        } else if ((bcon->beacon_ies_len > 0) && (bcon->beacon_ies != NULL)) {
                vsie_enable |= 0x01;
                wconf->ctrl_flag |= BEACON_EXTRA_IE_FLAG;
                if (nvt_icfg_add(nvt_bus, NVT_ICFG_SET, WID_VSIE_TX_DATA,
                        (u8 *)bcon->beacon_ies, bcon->beacon_ies_len) < 0) {
                        nvt_icfg_unlock(nvt_bus);
                        return -EINVAL;
                }
        } else if (reg_len != 0) {
                nvt_dbg(CFG80211, "only country info. in extra beacon ies\n");
                vsie_enable |= 0x01;
                wconf->ctrl_flag |= BEACON_EXTRA_IE_FLAG;
                if (nvt_icfg_add(nvt_bus, NVT_ICFG_SET, WID_VSIE_TX_DATA,
                                (u8 *)reg_buf, reg_len) < 0) {
                        nvt_icfg_unlock(nvt_bus);
                        return -EINVAL;
                }
        } else {
                nvt_dbg(CFG80211, "no ie in extra beacon ies\n");
        }

        //probe rsp ies
        // filter p2p IE
        err = nvt_filter_p2p_ie(&bcon->proberesp_ies, &bcon->proberesp_ies_len);
        if (err < 0)
                goto exit;
        // filter wfd IE
        err = nvt_filter_wfd_ie(&bcon->proberesp_ies, &bcon->proberesp_ies_len);
        if (err < 0)
                goto exit;

        if (((bcon->proberesp_ies_len > 0) && (bcon->proberesp_ies != NULL)) &&
                               (reg_len != 0)) {
                vsie_enable |= 0x04;
                wconf->ctrl_flag |= PROBE_RESP_EXTRA_IE_FLAG;
                nvt_dbg(CFG80211, "add country info. in extra prob_rsp ies\n");
                probe_rsp_buf = kzalloc(reg_len + bcon->proberesp_ies_len,
                                       GFP_KERNEL);
                if (probe_rsp_buf == NULL) {
                        nvt_dbg(CFG80211, "alloc prob_rsp buf fail\n");
                        return -ENOMEM;
                }
                memcpy(&probe_rsp_buf[0], &reg_buf[0], reg_len);
                memcpy(&probe_rsp_buf[reg_len], bcon->proberesp_ies,
                                               bcon->proberesp_ies_len);
                if (nvt_icfg_add(nvt_bus, NVT_ICFG_SET, WID_VSIE_TX_PROBERSP,
                        (u8 *)probe_rsp_buf,
                        reg_len + bcon->proberesp_ies_len) < 0) {
                        nvt_icfg_unlock(nvt_bus);
                        kfree(probe_rsp_buf);
                        return -EINVAL;
                }
                kfree(probe_rsp_buf);
        } else if ((bcon->proberesp_ies_len > 0) &&
                               (bcon->proberesp_ies != NULL)) {
                vsie_enable |= 0x04;
                wconf->ctrl_flag |= PROBE_RESP_EXTRA_IE_FLAG;
                if (nvt_icfg_add(nvt_bus, NVT_ICFG_SET, WID_VSIE_TX_PROBERSP,
                        (u8 *)bcon->proberesp_ies,
                        bcon->proberesp_ies_len) < 0) {
                        nvt_icfg_unlock(nvt_bus);
                        return -EINVAL;
                }
        } else if (reg_len != 0) {
                vsie_enable |= 0x04;
                wconf->ctrl_flag |= PROBE_RESP_EXTRA_IE_FLAG;
                nvt_dbg(CFG80211, "only country info. in extra prob_rsp ies\n");
                if (nvt_icfg_add(nvt_bus, NVT_ICFG_SET, WID_VSIE_TX_PROBERSP,
                                       (u8 *)reg_buf, reg_len) < 0) {
                        nvt_icfg_unlock(nvt_bus);
                        return -EINVAL;
                }
        } else {
                nvt_dbg(CFG80211, "no ie in extra prob_rsp ies\n");
        }

        //assoc rsp ies
        if ((bcon->assocresp_ies_len > 0) && (bcon->assocresp_ies != NULL)) {
                vsie_enable |= 0x08;
                wconf->ctrl_flag |= ASSOC_RESP_EXTRA_IE_FLAG;

                if (nvt_icfg_add(nvt_bus, NVT_ICFG_SET, WID_VSIE_TX_ASSOCRSP,
                   (u8 *)bcon->assocresp_ies, bcon->assocresp_ies_len) < 0) {
                        nvt_icfg_unlock(nvt_bus);
                        return -EINVAL;
                }
        }

        if (nvt_icfg_add(nvt_bus, NVT_ICFG_SET, WID_VSIE_FRAME,
           (u8 *)&vsie_enable, 1) < 0) {
                nvt_icfg_unlock(nvt_bus);
                return -EINVAL;
        }
exit:
        if (nvt_icfg_send_and_get_resp(nvt_bus, NULL, 0) < 0) {
                nvt_icfg_unlock(nvt_bus);
                return -EINVAL;
        } else {
                nvt_icfg_unlock(nvt_bus);
        }

        return err;
}

static int nvt_cfg80211_del_beacon(struct wiphy *wiphy,
        struct net_device *ndev)
{
        u8 enable = 0;
        struct _nvt_if *nvt_if = netdev_priv(ndev);
        struct _nvt_bus *nvt_bus = nvt_if->nvt_adapter->nvt_bus;
        struct _nvt_adapter *nvt_adapter = nvt_if->nvt_adapter;
        #ifdef CONFIG_WIFI_TUNING_PHASE_II
        struct _nvt_priv *nvt_priv = &(nvt_adapter->nvt_priv);
        struct _sta_sts_in_ap *sta_sts_in_ap;
        #endif
        u8 i = 0;
        g_start_op = DO_RESET;

        nvt_if->wait_ap_start_event = 0;
        nvt_if->start_ap_flag = 0;
        nvt_if->nvt_wconf_ap.acs_channel = 0xFF;
        nvt_if->nvt_wconf_ap.acs_mask = 0;
        nvt_if->nvt_wconf_ap.acs_weight[0] = 0xFF;
        clear_bit(NVT_IF_CONNECTED, &nvt_if->state_flags);

        if (nvt_icfg_lock(nvt_bus) < 0 ||
           nvt_icfg_reset(nvt_bus) < 0 ||
           nvt_icfg_add(nvt_bus, NVT_ICFG_SET,
                        WID_AP_ENABLE, &enable, 1) < 0 ||
           nvt_icfg_add(nvt_bus, NVT_ICFG_SET,
                        WID_START_OP, &g_start_op, 1) < 0 ||
           nvt_icfg_send_and_get_resp(nvt_bus, NULL, 0) < 0)
                nvt_icfg_unlock(nvt_bus);
        else {
                nvt_dbg(CFG80211, "%s\n", __func__);
                nvt_icfg_unlock(nvt_bus);
        }

        #ifdef CONFIG_WIFI_TUNING_PHASE_II
        // Recycle the all sta info stored in sta_list
        list_for_each_entry(sta_sts_in_ap, &nvt_priv->sta_list_in_ap, list) {
            nvt_dbg(CLEARVIEW, "%s: remove element:%p\n", __func__,
                      sta_sts_in_ap);
            list_del(&sta_sts_in_ap->list);
            kfree(sta_sts_in_ap);
        }
        #endif

        /* Reset BA handlers */
        for (i = 0; i < ASSOC_NUM; i++) {
                nvt_reset_ba_handle(nvt_adapter, i + 1);
        }
        return 0;
}
#endif

static s32 nvt_filter_p2p_ie(const u8 **ies, size_t *ies_len)
{
        const u8 *pos;
        u8 *buf = NULL;
        size_t len = 0;

        if (*ies && *ies_len) {
                buf = kmalloc(*ies_len, GFP_KERNEL);
                if (buf == NULL)
                        return -ENOMEM;
                pos = *ies;

                while (pos + 1 < *ies + *ies_len) {
                        if (pos + 2 + pos[1] > *ies + *ies_len)
                                break;
                        if (!(pos[0] == WLAN_EID_VENDOR_SPECIFIC &&
                                pos[1] >= 4 && pos[2] == 0x50 &&
                                pos[3] == 0x6f && pos[4] == 0x9a &&
                                pos[5] == 0x09)) {
                                memcpy(buf + len, pos, 2 + pos[1]);
                                len += 2 + pos[1];
                        }
                        pos += 2 + pos[1];
                }
        }
        *ies = buf;
        *ies_len = len;

        kfree(buf);
        return 0;
}

static s32 nvt_filter_wfd_ie(const u8 **ies, size_t *ies_len)
{
        const u8 *pos;
        u8 *buf = NULL;
        size_t len = 0;

        if (*ies && *ies_len) {
                buf = kmalloc(*ies_len, GFP_KERNEL);
                if (buf == NULL)
                        return -ENOMEM;
                pos = *ies;

                while (pos + 1 < *ies + *ies_len) {
                        if (pos + 2 + pos[1] > *ies + *ies_len)
                                break;
                        if (!(pos[0] == WLAN_EID_VENDOR_SPECIFIC &&
                              pos[1] >= 4 && pos[2] == 0x50 &&
                              pos[3] == 0x6f && pos[4] == 0x9a &&
                              pos[5] == 0x0A)) {
                                memcpy(buf + len, pos, 2 + pos[1]);
                                len += 2 + pos[1];
                        }
                        pos += 2 + pos[1];
                 }
        }
        *ies = buf;
        *ies_len = len;

        kfree(buf);
        return 0;
}

struct nvt_tlv *nvt_parse_tlvs(void *buf, int buflen, uint key)
{
        struct nvt_tlv *elt;
        int totlen;

        elt = (struct nvt_tlv *)buf;
        totlen = buflen;

        /* find tagged parameter */
        while (totlen >= TLV_HDR_LEN) {
                int len = elt->len;

                /* validate remaining totlen */
                if ((elt->id == key) && (totlen >= (len + TLV_HDR_LEN)))
                        return elt;

                elt = (struct nvt_tlv *)((u8 *)elt + (len + TLV_HDR_LEN));
                totlen -= (len + TLV_HDR_LEN);
        }

        return NULL;
}

#define WPA_OUI                         "\x00\x50\xF2"  /* WPA OUI */
#define WPA_OUI_TYPE                    1
#define RSN_OUI                         "\x00\x0F\xAC"  /* RSN OUI */
#define WME_OUI_TYPE                    2
#define WPS_OUI_TYPE                    4

#define VS_IE_FIXED_HDR_LEN             6
#define WPA_IE_VERSION_LEN              2
#define WPA_IE_MIN_OUI_LEN              4
#define WPA_IE_SUITE_COUNT_LEN          2

#define WPA_CIPHER_NONE                 0       /* None */
#define WPA_CIPHER_WEP_40               1       /* WEP (40-bit) */
#define WPA_CIPHER_TKIP                 2       /* TKIP: default for WPA */
#define WPA_CIPHER_AES_CCM              4       /* AES (CCM) */
#define WPA_CIPHER_WEP_104              5       /* WEP (104-bit) */

#define RSN_AKM_NONE                    0       /* None (IBSS) */
#define RSN_AKM_UNSPECIFIED             1       /* Over 802.1x */
#define RSN_AKM_PSK                     2       /* Pre-shared Key */
#define RSN_CAP_LEN                     2       /* Length of RSN capabilities */
#define RSN_CAP_PTK_REPLAY_CNTR_MASK    0x000C
#define RSN_CAP_MASK                    0x000C

static bool nvt_valid_wpa_oui(u8 *oui, bool is_rsn_ie)
{
        if (is_rsn_ie)
                return (memcmp(oui, RSN_OUI, TLV_OUI_LEN) == 0);

        return (memcmp(oui, WPA_OUI, TLV_OUI_LEN) == 0);
}

s32 nvt_configure_wpaie(struct nvt_vs_tlv *wpa_ie, bool is_rsn_ie,
        u8 *Nova_11I, u8 *Nova_AUTH)
{
        u16 count;
        s32 err = 0;
        s32 len = 0;
        u32 i;
        u32 pval = 0;
        u32 gval = 0;
        u32 wpa_auth = 0;
        u32 offset;
        u8 *data;
        u16 rsn_cap;

        if (wpa_ie == NULL)
                goto exit;

        len = wpa_ie->len + TLV_HDR_LEN;
        data = (u8 *)wpa_ie;
        offset = TLV_HDR_LEN;
        if (!is_rsn_ie)
                offset += VS_IE_FIXED_HDR_LEN;
        else
                offset += WPA_IE_VERSION_LEN;

        /* check for multicast cipher suite */
        if (offset + WPA_IE_MIN_OUI_LEN > len) {
                err = -EINVAL;
                goto exit;
        }

        if (!nvt_valid_wpa_oui(&data[offset], is_rsn_ie)) {
                err = -EINVAL;
                goto exit;
        }
        offset += TLV_OUI_LEN;

        /* pick up multicast cipher */
        switch (data[offset]) {
        case WPA_CIPHER_NONE:
                gval = 0;
                break;
        case WPA_CIPHER_WEP_40:
                *Nova_11I |= (WID_11I_MODE_Security_ON | WID_11I_MODE_WEP40);
                gval = 0x1;
                break;
        case WPA_CIPHER_WEP_104:
                *Nova_11I |= (WID_11I_MODE_Security_ON | WID_11I_MODE_WEP104);
                gval = 0x1;
                break;
        case WPA_CIPHER_TKIP:
                *Nova_11I |= (WID_11I_MODE_Security_ON | WID_11I_MODE_TKIP);
                gval = 0x2;
                break;
        case WPA_CIPHER_AES_CCM:
                *Nova_11I |= (WID_11I_MODE_Security_ON | WID_11I_MODE_CCMP);
                gval = 0x4;
                break;
        default:
                err = -EINVAL;
                goto exit;
        }

        offset++;
        /* walk thru unicast cipher list and pick up what we recognize */
        count = data[offset] + (data[offset + 1] << 8);
        offset += WPA_IE_SUITE_COUNT_LEN;
        /* Check for unicast suite(s) */
        if (offset + (WPA_IE_MIN_OUI_LEN * count) > len) {
                err = -EINVAL;
                goto exit;
        }
        for (i = 0; i < count; i++) {
                if (!nvt_valid_wpa_oui(&data[offset], is_rsn_ie)) {
                        err = -EINVAL;
                        goto exit;
                }
                offset += TLV_OUI_LEN;
                switch (data[offset]) {
                case WPA_CIPHER_NONE:
                        break;
                case WPA_CIPHER_WEP_40:
                        *Nova_11I |= (WID_11I_MODE_Security_ON |
                                      WID_11I_MODE_WEP40);
                        pval |= 0x1;
                        break;
                case WPA_CIPHER_WEP_104:
                        *Nova_11I |= (WID_11I_MODE_Security_ON |
                                      WID_11I_MODE_WEP104);
                        pval |= 0x1;
                        break;
                case WPA_CIPHER_TKIP:
                        *Nova_11I |= (WID_11I_MODE_Security_ON |
                                      WID_11I_MODE_TKIP);
                        pval |= 0x2;
                        break;
                case WPA_CIPHER_AES_CCM:
                        *Nova_11I |= (WID_11I_MODE_Security_ON |
                                      WID_11I_MODE_CCMP);
                        pval |= 0x4;
                        break;
                default:
                        nvt_dbg(CFG80211, "Ivalid unicast security info\n");
                }
                offset++;
        }
        /* walk thru auth management suite list and pick up what we recognize */
        count = data[offset] + (data[offset + 1] << 8);
        offset += WPA_IE_SUITE_COUNT_LEN;
        /* Check for auth key management suite(s) */
        if (offset + (WPA_IE_MIN_OUI_LEN * count) > len) {
                err = -EINVAL;
                goto exit;
        }
        for (i = 0; i < count; i++) {
                if (!nvt_valid_wpa_oui(&data[offset], is_rsn_ie)) {
                        err = -EINVAL;
                        goto exit;
                }
                offset += TLV_OUI_LEN;
                switch (data[offset]) {
                case RSN_AKM_NONE:
                        *Nova_AUTH |= WID_AUTH_OPEN_PSK;
                        wpa_auth |= 0x1;
                        break;
                case RSN_AKM_UNSPECIFIED:
                        *Nova_AUTH |= WID_AUTH_802_1X;
                        is_rsn_ie ? (wpa_auth |= 0x40) :
                                    (wpa_auth |= 0x2);
                        break;
                case RSN_AKM_PSK:
                        *Nova_AUTH |= WID_AUTH_OPEN_PSK;
                        is_rsn_ie ? (wpa_auth |= 0x80) :
                                    (wpa_auth |= 0x4);
                        break;
                default:
                        nvt_dbg(CFG80211, "Ivalid key mgmt info\n");
                }
                offset++;
        }

    /* Sanity Check with FW capability */
    if (is_rsn_ie) {
                if ((offset + RSN_CAP_LEN) <= len) {
                        rsn_cap = data[offset] + (data[offset + 1] << 8);
                        if ((rsn_cap & ~RSN_CAP_MASK) != 0)
                                err = -EINVAL;
                }
                if (err < 0) {
                        nvt_dbg(CFG80211, "RSN capability mismatch\n");
                        goto exit;
                }
        }
exit:
        return err;
}
#if LINUX_VERSION_CODE >= KERNEL_VERSION(3, 4, 0)
s32 nvt_cfg80211_change_beacon(struct wiphy *wiphy, struct net_device *ndev,
                              struct cfg80211_beacon_data *bcon)
{
        u8 *wps_ie, *p2p_ie, *rsn_ie, *wpa_ie;
        struct _nvt_if *nvt_if = netdev_priv(ndev);
        struct _BSS_CONFIG_INFO bss_config;
        struct _nvt_wlan_conf *wconf = &nvt_if->nvt_wconf;
        struct _nvt_bus *nvt_bus = nvt_if->nvt_adapter->nvt_bus;
        struct nvt_tlv *ssid_ie;
        u8 vsie_enable = 0;
        u8 *capability_info = 0;
        u8 ie_offset;
        s32 err = 0;
        struct _nvt_cfg80211 *cfg = nvt_if->nvt_adapter->nvt_cfg80211;
        struct _nvt_802_11d_reg *domain_info = &cfg->domain_reg;
        u8 *bcn_buf = NULL;
        u8 *probe_rsp_buf = NULL;
        u8 reg_buf[NVT_MAX_TRIPLET_802_11D * 3 + 3] = {0};
        u8 reg_len = 0;

        nvt_dbg(CFG80211, "%s\n", __func__);

        //clean ap relative flags
        wconf->ctrl_flag &= ~(BEACON_EXTRA_IE_FLAG|PROBE_RESP_EXTRA_IE_FLAG|
        ASSOC_RESP_EXTRA_IE_FLAG);

        memset(&bss_config, 0, sizeof(struct  _BSS_CONFIG_INFO));

        /* find the SSID_IE */
        ie_offset = DOT11_MGMT_HDR_LEN + DOT11_BCN_PRB_FIXED_LEN;
        ssid_ie = nvt_parse_tlvs((u8 *)&bcon->head[ie_offset],
                bcon->head_len - ie_offset, WLAN_EID_SSID);

        if (!ssid_ie)
                return -EINVAL;

        bss_config.ssid_le.SSID_len = ssid_ie->len;
        memcpy(bss_config.ssid_le.SSID, ssid_ie->data, ssid_ie->len);

        // get capability info
        capability_info = (u8 *)ssid_ie;
        capability_info = capability_info - 2;

        /* find the WPS IE */
        wps_ie = (u8 *)cfg80211_find_vendor_ie(WLAN_OUI_MICROSOFT,
                                                    WLAN_OUI_TYPE_MICROSOFT_WPS,
                                                    bcon->tail,
                                                    bcon->tail_len);
        if (wps_ie != NULL)
                bss_config.wps_enable = 1;
        else
                bss_config.wps_enable = 0;
        if (wps_ie != NULL)
                nvt_dbg(CFG80211, "[%s]WPS_OK\n", __func__);
        /* find the P2P IE */
        p2p_ie = (u8 *)cfg80211_find_vendor_ie(WLAN_OUI_WFA,
                                                    WLAN_OUI_TYPE_WFA_P2P,
                                                    bcon->tail,
                                                    bcon->tail_len);
        if (p2p_ie != NULL)
                bss_config.p2p_enable = 1;
        else
                bss_config.p2p_enable = 0;

        /* find the RSN_IE */
        rsn_ie = (u8 *)cfg80211_find_ie(WLAN_EID_RSN,
                       bcon->tail, bcon->tail_len);

        /* find the WPA_IE */
        wpa_ie = (u8 *)cfg80211_find_vendor_ie(WLAN_OUI_MICROSOFT,
                                         WLAN_OUI_TYPE_MICROSOFT_WPA,
                                         bcon->tail, bcon->tail_len);

        if ((wpa_ie != NULL || rsn_ie != NULL)) {
                if (wpa_ie != NULL) {
                        err = nvt_configure_wpaie((struct nvt_vs_tlv *)wpa_ie,
                                false, &bss_config.security_mode,
                                &bss_config.auth_mode);
                        /* WPA IE */
                        bss_config.security_mode |= WID_11I_MODE_WPA;
                        nvt_dbg(CFG80211, "WPA_IE OK\n");
                        if (err < 0)
                                goto exit;
                }
                if (rsn_ie != NULL) {
                        err = nvt_configure_wpaie((struct nvt_vs_tlv *)rsn_ie,
                                true, &bss_config.security_mode,
                                &bss_config.auth_mode);
                        /* RSN IE */
                        bss_config.security_mode |= WID_11I_MODE_RSN;
                        if (err < 0)
                                goto exit;
                }
         } else {
                if (capability_info[0] & 0x10) {
                        // privacy flag
                        err = -EINVAL;
                        goto exit;
                } else {
                        bss_config.auth_mode = WID_AUTH_OPEN_PSK;
                        bss_config.security_mode = 0;
                }
        }
        if (nvt_icfg_lock(nvt_bus) < 0 || nvt_icfg_reset(nvt_bus) < 0) {
                nvt_icfg_unlock(nvt_bus);
                return -EINVAL;
        }

        //ssid
        if (nvt_icfg_add(nvt_bus, NVT_ICFG_SET, WID_SSID,
           (u8 *)&bss_config.ssid_le.SSID, 0) < 0) {
                nvt_icfg_unlock(nvt_bus);
                return -EINVAL;
        }

        //security
        if (nvt_icfg_add(nvt_bus, NVT_ICFG_SET, WID_11I_MODE,
           (u8 *)&bss_config.security_mode, 1) < 0) {
                nvt_icfg_unlock(nvt_bus);
                return -EINVAL;
        }

        if (nvt_icfg_add(nvt_bus, NVT_ICFG_SET, WID_AUTH_TYPE,
            (u8 *)&bss_config.auth_mode, 1) < 0) {
                nvt_icfg_unlock(nvt_bus);
                return -EINVAL;
        }

        // WPS_enable
        if (nvt_icfg_add(nvt_bus, NVT_ICFG_SET, WID_WPS_REGISTRATION_START,
           (u8 *)&bss_config.wps_enable, 1) < 0) {
                nvt_icfg_unlock(nvt_bus);
                return -EINVAL;
        }

        // P2P_enable
        if (nvt_icfg_add(nvt_bus, NVT_ICFG_SET, WID_P2P_ENABLE,
           (u8 *)&bss_config.p2p_enable, 1) < 0) {
                nvt_icfg_unlock(nvt_bus);
                return -EINVAL;
        }

        if (nvt_icfg_send_and_get_resp(nvt_bus, NULL, 0) < 0) {
                nvt_icfg_unlock(nvt_bus);
                return -EINVAL;
        } else {
                nvt_icfg_unlock(nvt_bus);
        }

        if (nvt_icfg_lock(nvt_bus) < 0 || nvt_icfg_reset(nvt_bus) < 0) {
                nvt_icfg_unlock(nvt_bus);
                return -EINVAL;
        }

        nvt_dbg(CFG80211, "%s, country_code: %c%c\n", __func__,
                        domain_info->country_code[0],
                        domain_info->country_code[1]);

        if (nvt_is_an_alpha2(domain_info->country_code)) {
                nvt_dbg(CFG80211, "construct country element\n");
                reg_len = nvt_set_country_info_elem(reg_buf, domain_info);
        }

        //beacon ies
        if (((bcon->beacon_ies_len > 0) && (bcon->beacon_ies != NULL)) &&
                (reg_len != 0)) {
                vsie_enable |= 0x01;
                wconf->ctrl_flag |= BEACON_EXTRA_IE_FLAG;
                nvt_dbg(CFG80211, "add country info. in extra beacon ies\n");
                bcn_buf = kzalloc(reg_len + bcon->beacon_ies_len, GFP_KERNEL);
                if (bcn_buf == NULL) {
                        nvt_dbg(CFG80211, "alloc bcn buf fail\n");
                        return -ENOMEM;
                }
                memcpy(&bcn_buf[0], &reg_buf[0], reg_len);
                memcpy(&bcn_buf[reg_len], bcon->beacon_ies,
                                bcon->beacon_ies_len);
                if (nvt_icfg_add(nvt_bus, NVT_ICFG_SET, WID_VSIE_TX_DATA,
                        (u8 *)bcn_buf, reg_len + bcon->beacon_ies_len) < 0) {
                        nvt_icfg_unlock(nvt_bus);
                        kfree(bcn_buf);
                        return -EINVAL;
                }
                kfree(bcn_buf);
        } else if ((bcon->beacon_ies_len > 0) && (bcon->beacon_ies != NULL)) {
                vsie_enable |= 0x01;
                wconf->ctrl_flag |= BEACON_EXTRA_IE_FLAG;
                if (nvt_icfg_add(nvt_bus, NVT_ICFG_SET, WID_VSIE_TX_DATA,
                        (u8 *)bcon->beacon_ies, bcon->beacon_ies_len) < 0) {
                        nvt_icfg_unlock(nvt_bus);
                        return -EINVAL;
                }
        } else if (reg_len != 0) {
                nvt_dbg(CFG80211, "only country info. in extra beacon ies\n");
                vsie_enable |= 0x01;
                wconf->ctrl_flag |= BEACON_EXTRA_IE_FLAG;
                if (nvt_icfg_add(nvt_bus, NVT_ICFG_SET, WID_VSIE_TX_DATA,
                                (u8 *)reg_buf, reg_len) < 0) {
                        nvt_icfg_unlock(nvt_bus);
                        return -EINVAL;
                }
        } else {
                nvt_dbg(CFG80211, "no ie in extra beacon ies\n");
        }

        //probe rsp ies
        // filter p2p IE
        err = nvt_filter_p2p_ie(&bcon->proberesp_ies, &bcon->proberesp_ies_len);
        if (err < 0)
                goto exit;
        // filter wfd IE
        err = nvt_filter_wfd_ie(&bcon->proberesp_ies, &bcon->proberesp_ies_len);
        if (err < 0)
                goto exit;

        if (((bcon->proberesp_ies_len > 0) && (bcon->proberesp_ies != NULL)) &&
                               (reg_len != 0)) {
                vsie_enable |= 0x04;
                wconf->ctrl_flag |= PROBE_RESP_EXTRA_IE_FLAG;
                nvt_dbg(CFG80211, "add country info. in extra prob_rsp ies\n");
                probe_rsp_buf = kzalloc(reg_len + bcon->proberesp_ies_len,
                                       GFP_KERNEL);
                if (probe_rsp_buf == NULL) {
                        nvt_dbg(CFG80211, "alloc prob_rsp buf fail\n");
                        return -ENOMEM;
                }
                memcpy(&probe_rsp_buf[0], &reg_buf[0], reg_len);
                memcpy(&probe_rsp_buf[reg_len], bcon->proberesp_ies,
                                               bcon->proberesp_ies_len);
                if (nvt_icfg_add(nvt_bus, NVT_ICFG_SET, WID_VSIE_TX_PROBERSP,
                        (u8 *)probe_rsp_buf,
                        reg_len + bcon->proberesp_ies_len) < 0) {
                        nvt_icfg_unlock(nvt_bus);
                        kfree(probe_rsp_buf);
                        return -EINVAL;
                }
                kfree(probe_rsp_buf);
        } else if ((bcon->proberesp_ies_len > 0) &&
                               (bcon->proberesp_ies != NULL)) {
                vsie_enable |= 0x04;
                wconf->ctrl_flag |= PROBE_RESP_EXTRA_IE_FLAG;
                if (nvt_icfg_add(nvt_bus, NVT_ICFG_SET, WID_VSIE_TX_PROBERSP,
                        (u8 *)bcon->proberesp_ies,
                        bcon->proberesp_ies_len) < 0) {
                        nvt_icfg_unlock(nvt_bus);
                        return -EINVAL;
                }
        } else if (reg_len != 0) {
                vsie_enable |= 0x04;
                wconf->ctrl_flag |= PROBE_RESP_EXTRA_IE_FLAG;
                nvt_dbg(CFG80211, "only country info. in extra prob_rsp ies\n");
                if (nvt_icfg_add(nvt_bus, NVT_ICFG_SET, WID_VSIE_TX_PROBERSP,
                                       (u8 *)reg_buf, reg_len) < 0) {
                        nvt_icfg_unlock(nvt_bus);
                        return -EINVAL;
                }
        } else {
                nvt_dbg(CFG80211, "no ie in extra prob_rsp ies\n");
        }

        //assoc rsp ies
        if ((bcon->assocresp_ies_len > 0) && (bcon->assocresp_ies != NULL)) {
                vsie_enable |= 0x08;
                wconf->ctrl_flag |= ASSOC_RESP_EXTRA_IE_FLAG;

                if (nvt_icfg_add(nvt_bus, NVT_ICFG_SET, WID_VSIE_TX_ASSOCRSP,
                   (u8 *)bcon->assocresp_ies, bcon->assocresp_ies_len) < 0) {
                        nvt_icfg_unlock(nvt_bus);
                        return -EINVAL;
                }
        }

        if (nvt_icfg_add(nvt_bus, NVT_ICFG_SET, WID_VSIE_FRAME,
           (u8 *)&vsie_enable, 1) < 0) {
                nvt_icfg_unlock(nvt_bus);
                return -EINVAL;
        }
exit:
        if (nvt_icfg_send_and_get_resp(nvt_bus, NULL, 0) < 0) {
                nvt_icfg_unlock(nvt_bus);
                return -EINVAL;
        } else {
                nvt_icfg_unlock(nvt_bus);
        }

        return err;
}
#endif

#if LINUX_VERSION_CODE < KERNEL_VERSION(3, 19, 0)
static s32
nvt_cfg80211_del_station(struct wiphy *wiphy, struct net_device *ndev,
               u8 *mac)
#else
static s32
nvt_cfg80211_del_station(struct wiphy *wiphy, struct net_device *ndev,
               struct station_del_parameters *params)
#endif
{
#if LINUX_VERSION_CODE >= KERNEL_VERSION(3, 19, 0)
        u8 *mac = (u8 *)params->mac;
#endif
        struct _nvt_if *nvt_if = netdev_priv(ndev);
        struct _nvt_bus *nvt_bus = nvt_if->nvt_adapter->nvt_bus;
        struct _nvt_adapter *nvt_adapter = nvt_if->nvt_adapter;
        #ifdef CONFIG_WIFI_TUNING_PHASE_II
        struct _nvt_priv *nvt_priv = &(nvt_adapter->nvt_priv);
        struct _sta_sts_in_ap *sta_sts_in_ap;
        #endif
        struct _ba_struct_t *ba_ctxt = NULL;
        u8 i = 0;
        u8 idx;
        s32 err = 0;

        nvt_dbg(CFG80211, "%s\n", __func__);

        if ((!mac) || is_broadcast_ether_addr(mac))
                return -EFAULT;

        if (nvt_icfg_lock(nvt_bus) > -1 &&
           nvt_icfg_reset(nvt_bus) > -1 &&
           nvt_icfg_add(nvt_bus,
           NVT_ICFG_SET, WID_EXT_DISCONNECT, mac, 6) > -1 &&
           nvt_icfg_send_and_get_resp(nvt_bus, NULL, 0) > -1)
                nvt_icfg_unlock(nvt_bus);
        else {
                nvt_icfg_unlock(nvt_bus);
                err = -EINVAL;
        }

        #ifdef CONFIG_WIFI_TUNING_PHASE_II
        list_for_each_entry(sta_sts_in_ap, &nvt_priv->sta_list_in_ap, list) {
            if (!memcmp(sta_sts_in_ap->mac_addr, mac, ETH_ALEN)) {
                nvt_dbg(CLEARVIEW, "%s: del sta sts from list, %p\n", __func__,
                          sta_sts_in_ap);
                list_del(&sta_sts_in_ap->list);
                kfree(sta_sts_in_ap);
                break;
            }
        }
        #endif

        /* Reset BA handlers */
        for (i = 0; i < ASSOC_NUM; i++) {
                idx = i * NUM_TIDS;
                ba_ctxt = &(nvt_adapter->nvt_priv.ba_rx[idx]);
                if (memcmp(ba_ctxt->dst_addr, mac, ETH_ALEN) == 0) {
                        nvt_reset_ba_handle(nvt_adapter, i + 1);
                }
        }
    return err;
}

static s32
nvt_cfg80211_change_iface(struct wiphy *wiphy, struct net_device *ndev,
        enum nl80211_iftype type, u32 *flags, struct vif_params *params)
{
        s32 ret = 0;
        struct _nvt_if *nvt_if = netdev_priv(ndev);

        nvt_dbg(CFG80211, "%s, iftype = %d\n", __func__, type);

        ndev->type = ARPHRD_ETHER;
        switch (type) {
        case NL80211_IFTYPE_MONITOR:
                nvt_reset_fcmode(nvt_if, type);
                if (nvt_if->mode != NVT_FW_STA) {
                        nvt_if->mode = NVT_FW_STA;
                        ret = nvt_fw_change_opmode(nvt_if, NVT_FW_STA);
                        if (ret < 0) {
                                nvt_dbg(ERROR, "%s:fw download STA fail\r\n",
                                        __func__);
                        }
                }
                ndev->type = ARPHRD_IEEE80211_RADIOTAP;
                break;
        case NL80211_IFTYPE_WDS:
        case NL80211_IFTYPE_MESH_POINT:
                nvt_dbg(ERROR,
                "type (%d) : currently we do not support this type\n", type);
                break;
        case NL80211_IFTYPE_ADHOC:
                break;
        case NL80211_IFTYPE_STATION:
        case NL80211_IFTYPE_P2P_CLIENT:
                nvt_reset_fcmode(nvt_if, type);
                if (nvt_if->mode != NVT_FW_STA) {
                        nvt_if->mode = NVT_FW_STA;
                        ret = nvt_fw_change_opmode(nvt_if, NVT_FW_STA);
                        //nvt_init_fw_by_wids(nvt_if->nvt_adapter->nvt_bus);
                        if (ret < 0) {
                                nvt_dbg(ERROR, "%s:fw download STA fail\r\n",
                                        __func__);
                        }
                }
                break;
        case NL80211_IFTYPE_AP:
        case NL80211_IFTYPE_AP_VLAN:
        case NL80211_IFTYPE_P2P_GO:
                nvt_reset_fcmode(nvt_if, type);
                nvt_if->mode = NVT_FW_AP;
                ret = nvt_fw_change_opmode(nvt_if, NVT_FW_AP);
                if (ret < 0) {
                        nvt_dbg(ERROR, "%s:fw download AP fail\r\n", __func__);
                }
                /*get fw info*/
                {
                        s32 val_len = 0;
                        u8 resp[512] = {0};
                        u32 buff_size = 512;
                        u8 wid_val_pos = 0;
                        struct _nvt_bus *nvt_bus = nvt_if->nvt_adapter->nvt_bus;

                        val_len = nvt_get_wid_value(nvt_bus, WID_FIRMWARE_INFO,
                                         resp, buff_size, &wid_val_pos);

                        if (resp[wid_val_pos+6] & 0x8) {
                                nvt_if->wait_ap_start_event = 1;
                        } else {
                                nvt_if->wait_ap_start_event = 0;
                        }
                }

                /* Initialize FW defaut configurations */
                //nvt_init_fw_by_wids(nvt_if->nvt_adapter->nvt_bus);
                break;
        default:
                return -EOPNOTSUPP;
        }

        nvt_if->wdev.iftype = type;

        /* get firmware capability */
        nvt_set_firmware_capability(nvt_if->nvt_adapter);

        /* Initialize FW defaut configurations */
        nvt_init_fw_by_wids(nvt_if->nvt_adapter->nvt_bus, type);

        return ret;
}

/**
 * nvt_resume_result - station mode resume event handle
 *
 * @nvt_if: nvt network interface
 * @msg_len: event message length
 * @data: event data
 *
 * Return: return 0 success
 */
s32 nvt_resume_result(struct _nvt_if *nvt_if, u16 msg_len, u8 *data)
{
        struct _nvt_adapter *nvt_adapter = nvt_if->nvt_adapter;
        u16 wid = data[0]+(data[1] << 8);

        nvt_dbg(CFG80211, "%s, wid=0x%x\n", __func__, wid);
        nvt_if->sleep_mode = NVT_SLEEP_MODE_DISABLE;

        if (data[2] != 0) {
                nvt_scan_init(nvt_adapter->nvt_cfg80211);
                cfg80211_disconnected(nvt_if->ndev, 0, NULL, 0, GFP_KERNEL);
        }

        if (nvt_txmq == 0) {
                netif_start_queue(nvt_if->ndev);
        } else {
                netif_tx_start_all_queues(nvt_if->ndev);
        }
        return 0;
}

/**
 * nvt_rekeyoffload_info - station mode rekeyoffload info handle
 *
 * @nvt_if: nvt network interface
 * @msg_len: event message length
 * @data: event data
 *
 * Return: return 0 success
 */
s32 nvt_rekeyoffload_info(struct _nvt_if *nvt_if, u16 msg_len, u8 *data)
{
        struct _nvt_wlan_conf *wconf = &nvt_if->nvt_wconf;
        struct _nvt_sec *sec = &nvt_if->nvt_wconf.nvt_sec;
        u16 wid = data[0]+(data[1] << 8);

        nvt_dbg(CFG80211, "%s, wid=0x%x\n", __func__, wid);

        if (sec->ext_mode_11i & WID_EXT_11I_WAPI) {
                nvt_dbg(CFG80211, "WAPI REKEY OFFLOAD INFO\n");
                cfg80211_gtk_rekey_notify(nvt_if->ndev,
                        wconf->bssid, &data[2], GFP_KERNEL);
        } else if ((sec->mode_11i & WID_11I_TKIP) ||
                (sec->mode_11i & WID_11I_CCMP)) {
                nvt_dbg(CFG80211, "WPA REKEY OFFLOAD INFO\n");
                cfg80211_gtk_rekey_notify(nvt_if->ndev,
                        wconf->bssid, &data[2], GFP_KERNEL);
        } else {
                nvt_dbg(CFG80211, "Rekey but not in WPA/WPA2/WAPI mode\n");
        }
        return 0;
}

/**
 * nvt_setkey_info - station mode setkey info handle
 *
 * @nvt_if: nvt network interface
 * @msg_len: event message length
 * @data: event data
 *
 * Return: return 0 success
 */
s32 nvt_setkey_info(struct _nvt_if *nvt_if, u16 msg_len, u8 *data)
{
        struct _nvt_adapter *nvt_adapter = nvt_if->nvt_adapter;
        u8 sta_index;
        u16 wid = data[0]+(data[1] << 8);

        nvt_dbg(ERROR, "%s, wid=0x%x\n", __func__, wid);

        /* handle set ptk key info */
        if (data[2] == 1) {
                for (sta_index = 1; sta_index < 9; sta_index++) {
                        nvt_dbg(ERROR, "reset unicast PN\n");
                        nvt_reset_unicast_pn(nvt_adapter, sta_index);
                }
        }
        /* handle set gtk key info */
        if (data[3] == 1) {
                if (nvt_adapter->nvt_priv.pn_bcmc == NULL) {
                        nvt_dbg(ERROR, "BCMC PN is NULL, allocate it\n");
                        nvt_adapter->nvt_priv.pn_bcmc = kzalloc(6, GFP_KERNEL);
                        if (nvt_adapter->nvt_priv.pn_bcmc == NULL) {
                                nvt_dbg(ERROR, "BC PN allocate fail!!\n");
                                return -ENOMEM;
                        }
                }
                memcpy(nvt_adapter->nvt_priv.pn_bcmc, &data[4], 6);
        }
        return 0;
}

/*compute UDP and IP header's checksum*/
u16 dhcp_checksum(u8 *buf, u32 nbytes, u32 sum)
{
        u32 counter;
        counter = 0;
        while (nbytes > 1) {
                sum += htons(*((u16 *)(buf + counter)));
                nbytes -= 2;
                counter += 2;
        }

        if (nbytes) {
                sum += (buf[counter] << 8);
        }
        while (sum >> 16) {
                sum = (sum & 0xffff) + (sum >> 16);
        }
        return ~sum;
}

void build_dhcp_renew_pat(u8 *buf, u8 *src_addr, u8 *dst_addr,
            u8 *mac_addr)
{
        u16 buf_idx = 0;
        u8 checksum[2] = {0, 0};
        u16 sum = 0;
        u16 udp_data_size = sizeof(struct dhcp_udp) +
                sizeof(struct dhcp_packet);
        buf_idx += sizeof(struct dhcp_ip);
        build_udp_header(buf + buf_idx, checksum);
        buf_idx += sizeof(struct dhcp_udp);
        build_dhcp_request(buf + buf_idx, mac_addr, src_addr, dst_addr);
        //back 12 byte to fill udp pseudo header
        //since pseudo header for UDP require 12bytes
        buf_idx = sizeof(struct dhcp_ip) - 12;
        //fill udp pseudo header
        memcpy(buf + buf_idx, src_addr, 4);
        buf_idx += 4;
        memcpy(buf + buf_idx, dst_addr, 4);
        buf_idx += 4;
        *(buf + buf_idx) = 0x00;
        *(buf + buf_idx + 1) = 0x11;
        buf_idx += 2;
        *(buf + buf_idx) = (udp_data_size >> 8) & 0xff;
        *(buf + buf_idx + 1) = udp_data_size & 0xff;
        buf_idx = sizeof(struct dhcp_ip) - 12;
        //calculate udp checksum
        sum = dhcp_checksum(buf + buf_idx,
                20 + sizeof(struct dhcp_packet), 0);
        //fill checksum to udp header
        ((struct dhcp_udp *)(buf + sizeof(struct dhcp_ip)))->checksum[1]
                    = sum & 0xFF;
        ((struct dhcp_udp *)(buf + sizeof(struct dhcp_ip)))->checksum[0]
                    = (sum >> 8) & 0xFF;
        //build IP header
        build_ip_header(buf, checksum, src_addr, dst_addr);
        //calculate ip checksum
        sum = dhcp_checksum(buf, 20, 0);
        ((struct dhcp_ip *)(buf))->ip_checksum[1] = sum & 0xFF;
        ((struct dhcp_ip *)(buf))->ip_checksum[0] = (sum >> 8) & 0xFF;
}

void build_ip_header(u8 *buf, u8 *checksum, u8 *src_addr,
            u8 *dst_addr)
{
        u16 ip_data_size = sizeof(struct dhcp_ip) +
                sizeof(struct dhcp_udp) +
                sizeof(struct dhcp_packet);
        struct dhcp_ip *dhcp_ip_header = (struct dhcp_ip *) buf;
        memset(dhcp_ip_header, 0, sizeof(struct dhcp_ip));
        //fixed version 4 and header len 20byte
        dhcp_ip_header->ip_v_and_hl = 0x45;
        dhcp_ip_header->ip_tos = 0x00;
        //id = 0x01
        dhcp_ip_header->ip_id[0] = 0x00;
        dhcp_ip_header->ip_id[1] = 0x01;
        //fixed ip_len : 278byte
        dhcp_ip_header->ip_len[0] =
                (ip_data_size >> 8) & 0xff;
        dhcp_ip_header->ip_len[1] = ip_data_size & 0xff;
        //offset
        dhcp_ip_header->ip_off[0] = 0x00;
        dhcp_ip_header->ip_off[0] = 0x00;
        //ip time to live
        dhcp_ip_header->ip_ttl = 0x40;
        //fixed udp protocol : 0x11
        dhcp_ip_header->ip_p = 0x11;
        dhcp_ip_header->ip_checksum[0] = checksum[0];
        dhcp_ip_header->ip_checksum[1] = checksum[1];
        memcpy(dhcp_ip_header->ip_src, src_addr, 4);
        memcpy(dhcp_ip_header->ip_dst, dst_addr, 4);
}

void build_udp_header(u8 *buf, u8 *checksum)
{
        u16 udp_data_size = sizeof(struct dhcp_udp) +
                sizeof(struct dhcp_packet);
        struct dhcp_udp *dhcp_udp_header = (struct dhcp_udp *) buf;
        memset(dhcp_udp_header, 0, sizeof(struct dhcp_udp));
        dhcp_udp_header->src_port[0] = 0x00;
        dhcp_udp_header->src_port[1] = 0x44;
        dhcp_udp_header->dst_port[0] = 0x00;
        dhcp_udp_header->dst_port[1] = 0x43;
        //dhcp request is being hard code : 258byte
        dhcp_udp_header->length[0] =
                (udp_data_size >> 8) & 0xff;
        dhcp_udp_header->length[1] = udp_data_size & 0xff;
        dhcp_udp_header->checksum[0] = checksum[0];
        dhcp_udp_header->checksum[1] = checksum[1];
}

void build_dhcp_request(u8 *buf, u8 *chaddr, u8 *ciaddr, u8 *s_addr)
{
        struct dhcp_packet *dhcp_pkt_ptr = (struct dhcp_packet *) buf;
        memset(dhcp_pkt_ptr, 0, sizeof(struct dhcp_packet));
        dhcp_pkt_ptr->op = 0x01;
        dhcp_pkt_ptr->htype = 0x01;
        dhcp_pkt_ptr->hlen = 0x06;
        dhcp_pkt_ptr->hops = 0x0;
        dhcp_pkt_ptr->xid[0] = 0x00;
        dhcp_pkt_ptr->xid[1] = 0x00;
        dhcp_pkt_ptr->xid[2] = 0x00;
        dhcp_pkt_ptr->xid[3] = 0x05;
        dhcp_pkt_ptr->secs[0] = 0x0;
        dhcp_pkt_ptr->secs[1] = 0x0;
        //solicit own client IP addr
        memcpy(dhcp_pkt_ptr->ciaddr, ciaddr, 4);
        memcpy(dhcp_pkt_ptr->chaddr, chaddr, 6);
        //fill cookie
        dhcp_pkt_ptr->dhcp_cookie[0] = 0x63;
        dhcp_pkt_ptr->dhcp_cookie[1] = 0x82;
        dhcp_pkt_ptr->dhcp_cookie[2] = 0x53;
        dhcp_pkt_ptr->dhcp_cookie[3] = 0x63;
        //request option
        dhcp_pkt_ptr->dhcp_req_type = 0x35;
        dhcp_pkt_ptr->dhcp_req_len = 0x01;
        dhcp_pkt_ptr->request_code = 0x03;
        //client ID option
        dhcp_pkt_ptr->dhcp_ci_type = 61;
        dhcp_pkt_ptr->dhcp_ci_len = 7;
        dhcp_pkt_ptr->dhcp_ci_hw_type = 0x01;
        memcpy(dhcp_pkt_ptr->dhcp_ci_hw_mac, chaddr, 6);
        //request IP option
        dhcp_pkt_ptr->dhcp_rIP_type = 50;
        dhcp_pkt_ptr->dhcp_rIP_len = 4;
        memcpy(dhcp_pkt_ptr->dhcp_rIP, ciaddr, 4);
        //server ID option
        //dhcp_pkt_ptr->dhcp_server_type = 0x36;
        //dhcp_pkt_ptr->dhcp_s_len = 0x04;
        //memcpy(dhcp_pkt_ptr->server_ip, s_addr, 4);
        dhcp_pkt_ptr->end = 0xff;
}
