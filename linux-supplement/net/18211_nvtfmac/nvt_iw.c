#include <linux/types.h>
#include <linux/kernel.h>
#include <net/iw_handler.h>
#include <linux/netdevice.h>
#include <linux/workqueue.h>
#include <linux/string.h>
/*AT Support ARP NS offload*/
#include <linux/rcupdate.h>
#include <linux/inetdevice.h>
/*AT Support for IPv6*/
#include <net/ipv6.h>
#include <net/if_inet6.h>
#include <linux/version.h>
#include <linux/if_ether.h>

#include "nvt_wlan_linux.h"
#include "nvt_bus.h"
#include "nvt_icfg.h"
#include "nvt_cfg80211.h"
#include "nvt_fw.h"
#include "nvt_util_dbg.h"
#include "nvt_icfg.h"
#include "nvt_iw.h"
#include "nvt_bus_sdioif.h"

#define NVT_IWPRIV_SETWID_RESP_LEN 16

#define NVT_IWPRIV_GETMIB_CMD_SIZE 32
#define NVT_IWPRIV_GETMIB_RSP_SIZE 512

#define NVT_IWPRIV_SETWID_CMD_SIZE 512

#define NVT_IWPRIV_GETWID_CMD_SIZE 64


/**
* _nvt_iwpriv_mib - MIB attribute entry
* The entry is to describe the related attribute for each "setmib" and
* "getmib" sub commands.
* The usage of "setmib" and "getmib":
*
*      iwpriv <net_interface> setmib <sub_cmd>=<value>
*      iwpriv <net_interface> getmib <sub_cmd>
*
* <sub_cmd> is a string name and <value> could be a number or a string.
*
* Basically, one <sub_cmd> maps to a WID message. If you specify the WID, we
* will send the WID when the related <sub_cmd> was exectued. You can also
* define your own handler, @setmib_handler, or @getmib_handler, for <sub_cmd>.
* If the handler was defined, we will call the handler directly. You can do
* what you need to do in your defined handlers.
*
* @name: iwpriv sub command name, <sub_cmd>, for "setmib" and "getmib"
* @wid: WID to be used
* @len_field_size: 1 for char, short, int, and string WID. 2 for binary WID
* @req_format: define <value> format, please refer to nvt_iwpriv_mib_req enum
* @resp_format: define response format, please refer to nvt_iwpriv_mib_resp
*                         enum
* @str_enum: string table for responsed WID value
* @enum_num: size of str_enum table
* @setmib_handler: user defined handler for the setmib sub command.
*                          This function would get <value> in ascii format.
*                          return 0 for success and a errno for fail
* @getmib_handler: user defined handler for the getmib sub command.
*                          If success, return 0, a string for displaying to
*                          the user, and the length of the string. a errno for
*                          fail
*
*
*/
struct _nvt_iwpriv_mib {
       s8 *name;
       u16 wid;
       u8 len_field_size;
       u8 req_format;
       u8 resp_format;
       s8 **str_enum;
       s8 enum_num;
       s32 (*setmib_handler)(struct _nvt_bus *, void *data, u8 *mib_value_str);
       s32 (*getmib_handler)(struct _nvt_bus *, void *data, u8 *out_str,
               u32 *out_len);

};

/**
* nvt_iwpriv_mib_req - defined the value type of iwpriv sub commands
* @REQ_CHAR_TYPE: char in hex format
* @REQ_SHORT_TYPE: short in hex format
* @REQ_INT_TYPE: int in hex format
* @REQ_STR_TYPE: a string in hex format
* @REQ_ENUM_STR_TYPE: use user defined string. We will compare the value of
*                                        sub command with the enum table
*                                        defined by
*                                        struct _nvt_iwpriv_mib.str_enum, and
*                                        use the matched enum index as WID
*                                        value
* @REQ_RAW_ASCII_TYPE: format example: 1a,1b,1c,1d
*
*/
enum nvt_iwpriv_mib_req {
       REQ_CHAR_TYPE = 0,
       REQ_SHORT_TYPE,
       REQ_INT_TYPE,
       REQ_STR_TYPE,
       REQ_ENUM_STR_TYPE,
       REQ_RAW_ASCII_TYPE,
       REQ_TYPE_NUM
};

/**
* nvt_iwpriv_mib_req - defined the display format
* @RSP_FORMAT_HEX: display result in hex format
* @RSP_FORMAT_ENUM_STRING:
*    use user defined string. We will use returned WID value as enum index of
*    struct _nvt_iwpriv_mib.str_enum to find the corresponding string
*    for display
*
* @RSP_FORMAT_STRING: a string
* @RSP_FORMAT_BINARY: binary format, for example: 1a,1b,1c,1d
*
*/
enum nvt_iwpriv_mib_resp {
       RSP_FORMAT_HEX = 0,
       RSP_FORMAT_ENUM_STRING,
       RSP_FORMAT_STRING,
       RSP_FORMAT_BINARY,
       RSP_FORMAT_NUM
};

static s8 *str_enable_disable[] = {
       "Disable",
       "Enable"
};

static s8 *str_power_save_mode[] = {
       "Disable",
       "Min_FAST_PS",
       "Max_FAST_PS",
       "Min_PSPOOL_PS",
       "Max_PSPOOL_PS",
       "User_Cfg_PS"
};

static s8 *str_short_gi[] = {
       "Long_GI",
       "Short_GI-20",
       "Short_GI-40",
       "Short_GI-20/40"
};

//20160322 nash:
#ifdef BUS_UTILIZATION
static s8 *str_bu_tx_mode[] = {
        "var",
        "allone",
        "allzero",
        "seq",
        "ran"
};
#endif

static s32 nvt_send_wid_set(struct _nvt_bus *nvt_bus, u16 wid, u8 *wid_val_str);
static s32 nvt_byte_to_hex_str(u8 *wid_start_val, s32 wid_value_byte_len,
                u8 data_byte, u8 *out_str, char **headers);
static s8 *str_legacy_mode[] = {
       "B_Only",
       "G_Only",
       "B/G_Mix1",
       "B/G_Mix2"
};

#ifdef CONFIG_WIFI_TUNING_PHASE_II
static s32 nvt_tpm_sta_stats(struct _nvt_bus *nvt_bus, void *data,
                u8 *out_str, u32 *outlen)
{
        struct _nvt_if *nvt_if = netdev_priv((struct net_device *)data);
        struct _nvt_adapter *nvt_adapter = nvt_if->nvt_adapter;
        struct _nvt_priv *nvt_priv = &nvt_adapter->nvt_priv;

        struct _sta_sts_in_ap *sta_sts_in_ap;

        nvt_dbg(ERROR, "%s:\n", __func__);

        list_for_each_entry(sta_sts_in_ap, &nvt_priv->sta_list_in_ap, list)
        {
             if (sta_sts_in_ap) {
                 printk("STA: %02x:%02x:%02x:%02x:%02x:%02x !\n",
                    sta_sts_in_ap->mac_addr[0], sta_sts_in_ap->mac_addr[1],
                    sta_sts_in_ap->mac_addr[2], sta_sts_in_ap->mac_addr[3],
                    sta_sts_in_ap->mac_addr[4], sta_sts_in_ap->mac_addr[5]);

                 printk("Tput:%d Byte/s, Phy_rate:%d\n",
                         sta_sts_in_ap->t_put, sta_sts_in_ap->phyrate);
             }
        }

        *outlen = strlen(out_str);

        return 0;

}
#endif

static s32 nvt_set_fc_borrow(struct _nvt_bus *nvt_bus, void *data,
                u8 *mib_val_str)
{
        ulong flags;
        struct _nvt_if *nvt_if = netdev_priv((struct net_device *)data);
        struct _nvt_adapter *nvt_adapter = nvt_if->nvt_adapter;
        struct _tx_info *tx_info = nvt_adapter->nvt_priv.tx_info;
        s32 ret = 0;
        spin_lock_irqsave(&tx_info->lock, flags);
        if (strcmp(mib_val_str, "1") == 0) {
                tx_info->is_credit_borrow = 1;
        } else {
                tx_info->is_credit_borrow = 0;
        }
        spin_unlock_irqrestore(&tx_info->lock, flags);
        if (nvt_icfg_lock(nvt_bus) < 0) {
                ret = -1;
                goto next_check;
        }

        if (nvt_icfg_reset(nvt_bus) < 0) {
                ret = -1;
                goto next_check;
        }

        if (nvt_icfg_add(nvt_bus, NVT_ICFG_SET, WID_FC_BORROW,
                (u8 *)&(tx_info->is_credit_borrow),
                sizeof(tx_info->is_credit_borrow)) < 0) {
                ret = -1;
                goto next_check;
        }

        if (nvt_icfg_send_and_get_resp(nvt_bus, NULL, 0) < 0) {
                ret = -1;
                goto next_check;
        }
next_check:
        nvt_icfg_unlock(nvt_bus);
        return ret;
}

static s32 nvt_get_fc_borrow_flag(struct _nvt_bus *nvt_bus, void *data,
        u8 *out_str, u32 *outlen) {

        struct _nvt_if *nvt_if = netdev_priv((struct net_device *)data);
        struct _nvt_adapter *nvt_adapter = nvt_if->nvt_adapter;
        struct _tx_info *tx_info = nvt_adapter->nvt_priv.tx_info;
        snprintf(out_str, NVT_IWPRIV_GETMIB_RSP_SIZE, "%x",
                        tx_info->is_credit_borrow);

        *outlen = strlen(out_str);
        return 0;

}

static s32 set_watermark(u32 watermark, bool is_high_watermark,
                void *data) {

        #ifndef CONFIG_WIFI_TUNING_PHASE_I
        struct _nvt_if *nvt_if = netdev_priv((struct net_device *)data);
        struct _nvt_adapter *nvt_adapter = nvt_if->nvt_adapter;
        //struct _nvt_priv nvt_priv = nvt_adapter->nvt_priv;
        struct _tx_info *tx_info = nvt_adapter->nvt_priv.tx_info;
        if (is_high_watermark) {
                tx_info->fc_hi_watermark = watermark;
        } else {
                tx_info->fc_low_watermark = watermark;
        }
        #endif
        return 0;
}

static s32 get_watermark(bool is_high_watermark, void *data)
{
        #ifndef CONFIG_WIFI_TUNING_PHASE_I
        struct _nvt_if *nvt_if = netdev_priv((struct net_device *)data);
        struct _nvt_adapter *nvt_adapter = nvt_if->nvt_adapter;
        //struct _nvt_priv nvt_priv = nvt_adapter->nvt_priv;
        struct _tx_info *tx_info = nvt_adapter->nvt_priv.tx_info;
        if (is_high_watermark) {
                return tx_info->fc_hi_watermark;
        } else {
                return tx_info->fc_low_watermark;
        }
        #endif
        return 0;
}

static s32 nvt_set_hwatermark_handler(struct _nvt_bus *nvt_bus, void *data,
                u8 *mib_value_str)
{

        s32 ret;
        u32 watermark = 0;

        if (mib_value_str == NULL) {
                return -EINVAL;
        }
        ret = kstrtou32(mib_value_str, 16, &watermark);
        if (ret < 0) {
                return -EINVAL;
        }
        ret = set_watermark(watermark, true, data);
        return ret;
}

static s32 nvt_get_hwatermark_handler(struct _nvt_bus *nvt_bus, void *data,
        u8 *out_str, u32 *outlen) {
        u32 watermark = get_watermark(true, data);
        snprintf(out_str, NVT_IWPRIV_GETMIB_RSP_SIZE, "%x", watermark);
        *outlen = strlen(out_str);
        return 0;

}

static s32 nvt_set_lwatermark_handler(struct _nvt_bus *nvt_bus, void *data,
                u8 *mib_value_str)
{

        s32 ret;
        u32 watermark = 0;

        if (mib_value_str == NULL) {
                return -EINVAL;
        }
        ret = kstrtou32(mib_value_str, 16, &watermark);
        if (ret < 0) {
                return -EINVAL;
        }
        ret = set_watermark(watermark, false, data);
        return ret;
}

static s32 nvt_get_lwatermark_handler(struct _nvt_bus *nvt_bus, void *data,
        u8 *out_str, u32 *outlen) {
        u32 watermark = get_watermark(false, data);
        snprintf(out_str, NVT_IWPRIV_GETMIB_RSP_SIZE, "%x", watermark);
        *outlen = strlen(out_str);
        return 0;

}
/*handle the set fix rate[for legacy rate or mcs] WID
* iwpriv wlan0 setmib rate=mcs,legacy_rate
* mcs = 0,1,2,3...15
* legacy = 1, 2, 5, 11, 36, 48, 54
*/
static s32 nvt_set_rate_handler(struct _nvt_bus *nvt_bus, void *data,
                u8 *mib_value_str)
{
        u8 const *delim = ",";
        char *mcs = strsep((char **)&mib_value_str, delim);
        char *legacy = mib_value_str;
        u8 const *autorate = "0";
        u8 config_mcs = 0xFF;

        if (mcs == NULL || legacy == NULL) {
                return -EINVAL;
        }

        if (strcmp(mcs, "FF") == 0 && strcmp(legacy, "FF") != 0) {
                return nvt_send_wid_set(nvt_bus, WID_CURRENT_TX_RATE,
                                (u8 *)legacy);

        } else if (strcmp(mcs, "FF") != 0 && strcmp(legacy, "FF") == 0) {
                nvt_send_wid_by_numeric(nvt_bus, WID_CURRENT_TX_RATE,
                                &config_mcs, 1);
                return nvt_send_wid_set(nvt_bus, WID_11N_CURRENT_TX_MCS,
                                (u8 *)mcs);

        /*Enable the autorate*/
        } else if (strcmp(mcs, "FF") == 0 && strcmp(legacy, "FF") == 0) {
                return nvt_send_wid_set(nvt_bus, WID_CURRENT_TX_RATE,
                                (u8 *)autorate);

        } else {
                return -EINVAL;
        }
        return 0;
}

static s32 nvt_get_rate_handler(struct _nvt_bus *nvt_bus, void *data,
        u8 *out_str, u32 *outlen)
{
        struct _nvt_if *nvt_if = netdev_priv((struct net_device *)data);
        u8 wid_val_pos;
        u8 wid_is_autorate;
        s32 wid_rate_val;
        s32 ret_val;
        u8 resp[512] = {0};
        u32 buff_size = 512;
        wid_val_pos = 0;
        /*send wid to get the rate0 info*/
        ret_val = nvt_get_wid_value(nvt_bus, WID_RATE_0, resp,
                buff_size, &wid_val_pos);
        /* check if wid_val_pos is correctly initialized */
        if (ret_val < 0) {
                return -1;
        }
        /*if b7 of rate_0 is 1 => ht_type*/
        if (resp[wid_val_pos] & 0x0080) {
                nvt_if->ht_type = 1;
        } else {
                nvt_if->ht_type = 0;
        }
        wid_rate_val = nvt_parse_rate_0(resp[wid_val_pos]);
        if (wid_rate_val < 0) {
                return -1;
        }
        /*send wid to get is autorate*/
        ret_val = nvt_get_wid_value(nvt_bus, WID_CURRENT_TX_RATE, resp,
                buff_size, &wid_val_pos);
        if (ret_val < 0) {
                return -1;
        }
        wid_is_autorate = resp[wid_val_pos];

        if (nvt_if->ht_type == 0 && !wid_is_autorate) {
                snprintf(out_str, NVT_IWPRIV_GETMIB_RSP_SIZE,
                        "autorate enable,legacy_rate : %d mbps" ,
                        wid_rate_val / 1000);
        } else if (nvt_if->ht_type == 0 && wid_is_autorate) {
                snprintf(out_str, NVT_IWPRIV_GETMIB_RSP_SIZE,
                        "autorate disable,legacy_rate : %d mbps"
                        , wid_rate_val / 1000);
        } else if (nvt_if->ht_type == 1 && !wid_is_autorate) {
                snprintf(out_str, NVT_IWPRIV_GETMIB_RSP_SIZE,
                        "autorate enable,mcs_rate : %x", wid_rate_val);
        } else if (nvt_if->ht_type == 1 && wid_is_autorate) {
                snprintf(out_str, NVT_IWPRIV_GETMIB_RSP_SIZE,
                        "autorate disable,mcs_rate : %x", wid_rate_val);
        } else {
                return -1;
        }
        *outlen = strlen(out_str);
        return 0;
}

/*** This function get the first IPv4 address from net_device ***/
s32 nvt_get_first_ipv4(struct net_device *data, u8 *ret_addr)
{

        struct in_device *in_dev;
        struct in_ifaddr *ifa;
        rcu_read_lock();
        in_dev = rcu_dereference(data->ip_ptr);
        if (in_dev != NULL) {
                ifa = in_dev->ifa_list;
                if (ifa != NULL) {
                        memcpy(ret_addr, &ifa->ifa_address, 4);
                } else {
                        nvt_dbg(CFG80211, "%s IPv4 is not set\n"
                                , __func__);
                        return -1;
                }
        } else {
                nvt_dbg(CFG80211, "%s IPv4 is not set\n", __func__);
                return -1;
        }
        rcu_read_unlock();

        return 0;
}

void nvt_get_mac_from_ip(struct net_device *dev, u8 *ret_addr,
        u8 *server_ip)
{
        struct neighbour *neigh;
        __be32 ip = *((__be32 *) (server_ip));
        neigh = neigh_lookup(&arp_tbl, &ip, dev);
        if (neigh) {
                read_lock_bh(&neigh->lock);
                memcpy(ret_addr, neigh->ha, dev->addr_len);
                read_unlock_bh(&neigh->lock);
                neigh_release(neigh);
        }
}

struct inet6_dev *__nvt_in6_dev_get(const struct net_device *dev)
{
        return rcu_dereference_rtnl(dev->ip6_ptr);
}

//Called by nvt_ipv6_get_lladdr
int __nvt_ipv6_get_lladdr(struct inet6_dev *idev, struct in6_addr *addr,
                unsigned char banned_flags)
{
        struct inet6_ifaddr *ifp;
        int err = -EADDRNOTAVAIL;

        list_for_each_entry(ifp, &idev->addr_list, if_list) {
        if (ifp->scope == IFA_LINK &&
                !(ifp->flags & banned_flags)) {
                        *addr = ifp->addr;
                        err = 0;
                        break;
                }
        }
        return err;
}

/*Get the IPv6 address, since ipv6_get_lladdr does not export
 *Add nvt as prefix for the new name
 * */
int nvt_ipv6_get_lladdr(struct net_device *dev, struct in6_addr *addr,
                                    unsigned char banned_flags)
{
        struct inet6_dev *idev;
        int err = -EADDRNOTAVAIL;

        rcu_read_lock();
        idev = __nvt_in6_dev_get(dev);
        if (idev) {
                read_lock_bh(&idev->lock);
                err = __nvt_ipv6_get_lladdr(idev, addr, banned_flags);
                read_unlock_bh(&idev->lock);
        }
        rcu_read_unlock();
        return err;
}

/*** This function get the first IPv6 address from net_device ***/
s32 nvt_get_first_ipv6(struct net_device *data, u8 *ret_addr)
{
        struct in6_addr *addr;
        addr = kmalloc(sizeof(struct in6_addr), GFP_KERNEL);
        if (!addr) {
                return -1;
        }
        nvt_ipv6_get_lladdr(data, addr, 0);
        if (addr != NULL && addr->in6_u.u6_addr8 != NULL) {
                memcpy(ret_addr, addr->in6_u.u6_addr8, 16);
        } else {
                kfree(addr);
                return -1;
        }
        kfree(addr);
        return 0;
}

static s32 nvt_ka_handler(struct _nvt_bus *nvt_bus, void *data,
                          u8 *mib_value_str)
{
        u8 ip_addr[4];
        char *ip_addr_str[4];
        u16 timeout;
        u8 ip_index = 0;
        u8 const *delim = ",";
        u8 *ka_buf;
        u8 i;
        s32 ret = 0;
        char *ip_index_str = strsep((char **)&mib_value_str, delim);
        char *timeout_str = strsep((char **)&mib_value_str, delim);
        ip_addr_str[0] = strsep((char **)&mib_value_str, delim);
        ip_addr_str[1] = strsep((char **)&mib_value_str, delim);
        ip_addr_str[2] = strsep((char **)&mib_value_str, delim);
        ip_addr_str[3] = mib_value_str;
        for (i = 0; i < 4; i++) {
                kstrtou8(ip_addr_str[i], 16, &ip_addr[i]);
        }
        kstrtou16(timeout_str, 16, &timeout);
        kstrtou8(ip_index_str, 16, &ip_index);
        /* 1B:ip_addr_index, 2B:timeout, 4B: ip_address */
        ka_buf = kzalloc(1 + 2 + 4, GFP_ATOMIC);
        if (ka_buf == 0) {
                goto fail;
        }
        ka_buf[0] = ip_index;
        *((u16 *)(ka_buf + 1)) = timeout;
        memcpy(ka_buf + 3, ip_addr, 4);
        /* 7 is the buffer size */
        ret = nvt_send_wid_by_numeric(nvt_bus, WID_KEEP_ALIVE_FAIL_OFFLOAD,
                                      ka_buf, 7);
fail:
        kfree(ka_buf);
        return ret;
}

static s32 nvt_dhcp_renew_handler(struct _nvt_bus *nvt_bus, void *data,
                u8 *mib_value_str)
{
        /** data buffer sent to FW **/
        u8 dhcp_to_cnts;
        u8 dhcp_pat_int;
        u8 s_ip_addr[4];
        u8 c_ip_addr[4];
        u8 i;
        u8 const *delim = ",";
        u8 *dhcp_buf;
        s32 buf_size;
        u32 lease_time;
        s32 ret = 0;
        char *s_ip_char[4];
        struct _nvt_if *nvt_if = netdev_priv((struct net_device *)data);
        char *dhcp_timeout_cnts = strsep((char **)&mib_value_str, delim);
        char *dhcp_pat_interval = strsep((char **)&mib_value_str, delim);
        char *lease_time_char = strsep((char **)&mib_value_str, delim);

        s_ip_char[0] = strsep((char **)&mib_value_str, delim);
        s_ip_char[1] = strsep((char **)&mib_value_str, delim);
        s_ip_char[2] = strsep((char **)&mib_value_str, delim);
        s_ip_char[3] = mib_value_str;
        for (i = 0; i < 4; i++) {
                kstrtou8(s_ip_char[i], 16, &s_ip_addr[i]);
        }
        kstrtou8(dhcp_timeout_cnts, 16, &dhcp_to_cnts);
        kstrtou8(dhcp_pat_interval, 16, &dhcp_pat_int);
        kstrtou32(lease_time_char, 16, &lease_time);
        nvt_get_first_ipv4((struct net_device *)data, c_ip_addr);
        /*** Sanity check ***/
        if (dhcp_timeout_cnts == NULL || dhcp_pat_interval == NULL) {
                return -EINVAL;
        }
        //size = 1B:to_cnt, 1B:pat_interval, 4B:lease_time, 6B S_HW
        buf_size = 1 + 1 + 4 + 6 + sizeof(struct dhcp_ip) +
                    sizeof(struct dhcp_udp) +
                    sizeof(struct dhcp_packet);
        //fill the wid value buffer
        dhcp_buf = kzalloc(buf_size, GFP_ATOMIC);
        if (dhcp_buf == 0) {
                goto fail;
        }
        dhcp_buf[0] = dhcp_to_cnts;
        dhcp_buf[1] = dhcp_pat_int;
        *((u32 *)(dhcp_buf + 2)) = lease_time;
        dhcp_buf += 6;
        //get the server mac address from the arp table
        nvt_get_mac_from_ip((struct net_device *)data, dhcp_buf,
                s_ip_addr);
        //check if ARP table is empty
        ret = 0;
        for (i = 0; i < 6; i++) {
                ret |= dhcp_buf[i];
        }
        if (ret == 0) {
                nvt_dbg(ERROR, "%s: ARP Table Empty\n",
                        __func__);
                goto arp_empty;
        }
        dhcp_buf += 6;
        //create dhcp pattern
        build_dhcp_renew_pat(dhcp_buf, c_ip_addr, s_ip_addr,
                    nvt_if->nvt_adapter->mac_addr);
        //back to the head of wid value buffer
        dhcp_buf -= 12;
        ret = nvt_send_wid_by_numeric(nvt_bus, WID_DHCP_RENEW_OFFLOAD,
                dhcp_buf, buf_size);
arp_empty:
        kfree(dhcp_buf);
        return ret;
fail:
        return -1;
}

/***** This function set the offload flag and set the IP address *****/
static s32 nvt_set_offload_handler(struct _nvt_bus *nvt_bus, void *data,
                u8 *mib_value_str)
{

        /** data buffer sent to FW **/
        u8 *wid_val_buf;

        u8 const *delim = ",";
        u8 offload_mask = 0;
        u8 offload_tmp_bit = 0;
        char *arp_offload_flag = strsep((char **)&mib_value_str, delim);
        char *ns_offload_flag = mib_value_str;
        s32 ret = 0;

        /*** Sanity check ***/
        if (ns_offload_flag == NULL || arp_offload_flag == NULL) {
                return -EINVAL;
        }
        /*** oring arp flag to offload_mask ***/
        kstrtou8(arp_offload_flag, 16, &offload_tmp_bit);
        offload_mask |= offload_tmp_bit << 7;

        /*** oring ns flag to offload_mask ***/
        kstrtou8(ns_offload_flag, 16, &offload_tmp_bit);
        offload_mask |= offload_tmp_bit << 6;

        /*** turnnig off both flag off ***/
        if (offload_mask == 0) {
                return nvt_send_wid_by_numeric(nvt_bus, WID_ARP_NS_OFFLOAD,
                        &offload_mask, 1);

        /*** set the ns_offload flag on ***/
        } else if (offload_mask == 64) {
                wid_val_buf = kzalloc(17, GFP_KERNEL);
                if (wid_val_buf == NULL) {
                        goto fail;
                }
                *wid_val_buf = offload_mask;
                nvt_get_first_ipv6((struct net_device *)data, wid_val_buf + 1);
                ret = nvt_send_wid_by_numeric(nvt_bus, WID_ARP_NS_OFFLOAD,
                        wid_val_buf, 17);
                if (ret < 0) {
                        nvt_dbg(ERROR, "%s: WID_ARP_NS_OFFLOAD fail\n",
                                __func__);
                }
                kfree(wid_val_buf);


        /*** set the arp_offload flag on ***/
        } else if (offload_mask == 128) {
                wid_val_buf = kzalloc(5, GFP_KERNEL);
                if (wid_val_buf == NULL) {
                        goto fail;
                }
                *wid_val_buf = offload_mask;
                nvt_get_first_ipv4((struct net_device *)data, wid_val_buf + 1);
                ret = nvt_send_wid_by_numeric(nvt_bus, WID_ARP_NS_OFFLOAD,
                        wid_val_buf, 5);
                if (ret < 0) {
                        nvt_dbg(ERROR, "%s: WID_ARP_NS_OFFLOAD fail\n",
                                __func__);
                }
                kfree(wid_val_buf);

        /*** set the arp and ns offload flag on ***/
        } else if (offload_mask == 192) {
                wid_val_buf = kzalloc(21, GFP_KERNEL);
                if (wid_val_buf == NULL) {
                        goto fail;
                }
                *wid_val_buf = offload_mask;
                nvt_get_first_ipv4((struct net_device *)data, wid_val_buf + 1);
                nvt_get_first_ipv6((struct net_device *)data, wid_val_buf + 5);
                ret = nvt_send_wid_by_numeric(nvt_bus, WID_ARP_NS_OFFLOAD,
                        wid_val_buf, 21);
                if (ret < 0) {
                        nvt_dbg(ERROR, "%s: WID_ARP_NS_OFFLOAD fail\n",
                                __func__);
                }
                kfree(wid_val_buf);

        } else {
                nvt_dbg(ERROR, "%s: offload_mask is not right\n", __func__);
                return -1;
        }
        return 0;
fail:
        return -1;

}

/***** This funtion get the offload flag *****/
static s32 nvt_get_offload_handler(struct _nvt_bus *nvt_bus, void *data,
        u8 *out_str, u32 *outlen)
{
        bool arp_flag;
        bool ns_flag;
        s32 ret_val;
        u8 offload_mask;
        u8 wid_val_pos;
        u8 resp[512] = {0};
        u32 buff_size = 512;
        wid_val_pos = 0;
        /*send wid to get the rate0 info*/
        ret_val = nvt_get_wid_value(nvt_bus, WID_ARP_NS_OFFLOAD, resp,
                buff_size, &wid_val_pos);
        if (ret_val < 0) {
                return -1;
        }
        offload_mask = resp[wid_val_pos];
        arp_flag = offload_mask & 1<<7;
        ns_flag = offload_mask & 1<<6;
        snprintf(out_str, NVT_IWPRIV_GETMIB_RSP_SIZE,
                "arp_offload_flag : %d ns_offload_flag : %d\n",
                arp_flag, ns_flag);
        *outlen = strlen(out_str);
        return 0;
}

#ifdef CONFIG_PM
static s32 nvt_set_wow_handler(struct _nvt_bus *nvt_bus, void *data,
                u8 *mib_value_str)
{
#if LINUX_VERSION_CODE > KERNEL_VERSION(3, 10, 0)
        struct wireless_dev *wdev = ((struct net_device *)data)->ieee80211_ptr;
        struct wiphy *wiphy = wdev->wiphy;
        struct cfg80211_wowlan *wow = wiphy->wowlan_config;
        struct _nvt_if *nvt_if = netdev_priv((struct net_device *)data);
        struct _nvt_cfg80211 *nvt_cfg = nvt_if->nvt_adapter->nvt_cfg80211;

        if (strcmp(mib_value_str, "1") == 0) {
                if (wow != NULL) {
                        if (nvt_setup_wowlan(nvt_if, nvt_cfg, wow) == 0) {
                                nvt_dbg(INFO, "enable wow ok\n");
                                nvt_cfg->wow_enable = 1;
                        } else {
                                nvt_dbg(ERROR, "enable wow fail\n");
                                nvt_cfg->wow_enable = 0;
                        }
                }
        } else {
                nvt_dbg(INFO, "disable wow\n");
                nvt_cfg->wow_enable = 0;
        }
#endif
        return 0;
}

static s32 nvt_get_wow_handler(struct _nvt_bus *nvt_bus, void *data,
        u8 *out_str, u32 *outlen)
{
        struct _nvt_if *nvt_if = netdev_priv((struct net_device *)data);
        struct _nvt_cfg80211 *nvt_cfg = nvt_if->nvt_adapter->nvt_cfg80211;

        if (nvt_cfg->wow_enable == 1) {
                snprintf(out_str, NVT_IWPRIV_GETMIB_RSP_SIZE, "wowlan enable");
        } else {
                snprintf(out_str, NVT_IWPRIV_GETMIB_RSP_SIZE, "wowlan disable");
        }
        *outlen = strlen(out_str);
        return 0;
}
#endif

#ifdef BUS_UTILIZATION
static s32 nvt_set_bus_utilization_mode(struct _nvt_bus *nvt_bus, void *data,
        u8 *mib_value_str)
{
        s32 i;
        s32 elem_num;

        if (nvt_bus == NULL) {
                nvt_dbg(ERROR, "%s: nvt_bus is NULL!\n", __func__);
                return -EPERM;
        }

        elem_num = sizeof(str_bu_tx_mode) / sizeof(str_bu_tx_mode[0]);

        for (i = 0; i < elem_num; i++) {
                if (strcmp(mib_value_str, str_bu_tx_mode[i]) == 0) {
                        nvt_bus->nvt_bu->tx_mode = i;
                        break;
                }
        }

        if (i == elem_num) {
                nvt_dbg(ERROR, "%s: Invalid BU TX mode!!\n", __func__);
        }

        return 0;

}

static s32 nvt_get_bus_utilization_mode(struct _nvt_bus *nvt_bus, void *data,
        u8 *out_str, u32 *outlen)
{
        s32 elem_num;

        if (nvt_bus == NULL) {
                nvt_dbg(ERROR, "%s: nvt_bus is NULL!\n", __func__);
                return -EPERM;
        }

        elem_num = sizeof(str_bu_tx_mode) / sizeof(str_bu_tx_mode[0]);
        if (nvt_bus->nvt_bu->tx_mode >= elem_num) {
                nvt_dbg(ERROR, "%s: tx mode error\n", __func__);
                return -EPERM;
        }

        snprintf(out_str, NVT_IWPRIV_GETMIB_RSP_SIZE, "TX mode=%s\n",
                str_bu_tx_mode[nvt_bus->nvt_bu->tx_mode]);

        *outlen = strlen(out_str);

        return 0;
}

static s32 nvt_bu_send_pkt(struct _nvt_bus *nvt_bus)
{
        s32 i;
        s32 ret;
        u32 len;
        u32 pkt_len;
        u32 tx_hdr_lo = 0;
        u32 tx_hdr_hi = 0;
        s32 nvt_txhdr_sz = 8;
        struct sk_buff *skb;

        if (nvt_bus == NULL) {
                nvt_dbg(ERROR, "%s: nvt_bus is NULL!\n", __func__);
                return -EPERM;
        }

        // bit19: ethernet frame
        tx_hdr_lo |= (0x00080000);
        tx_hdr_lo = cpu_to_be32(tx_hdr_lo);

        if (nvt_bus->nvt_bu->tx_mode == NVT_BUS_BU_TX_VARIABLE_LEN) {
                get_random_bytes(&pkt_len, 2);
                pkt_len %= ETH_FRAME_LEN;
                //set min frame size as 16 bytes
                if (pkt_len < 16) {
                        pkt_len = 16;
                }
                len = roundup((pkt_len + nvt_txhdr_sz), NVT_SDIO_F1_BLOCKSIZE);

                skb = dev_alloc_skb(len);
                if (skb == NULL) {
                        nvt_dbg(ERROR, "%s: skb alloc fail!\n", __func__);
                        return -1;
                }

        } else if (nvt_bus->nvt_bu->tx_mode == NVT_BUS_BU_TX_ALL_ONE) {
                pkt_len = roundup((ETH_FRAME_LEN + nvt_txhdr_sz),
                        NVT_SDIO_F1_BLOCKSIZE);

                skb = dev_alloc_skb(pkt_len);
                if (skb == NULL) {
                        nvt_dbg(ERROR, "%s: skb alloc fail!\n", __func__);
                        return -1;
                }
                memset(skb->data, 0xFF, pkt_len);

        } else if (nvt_bus->nvt_bu->tx_mode == NVT_BUS_BU_TX_ALL_ZERO) {
                pkt_len = roundup((ETH_FRAME_LEN + nvt_txhdr_sz),
                        NVT_SDIO_F1_BLOCKSIZE);

                skb = dev_alloc_skb(pkt_len);
                if (skb == NULL) {
                        nvt_dbg(ERROR, "%s: skb alloc fail!\n", __func__);
                        return -1;
                }
                memset(skb->data, 0x00, pkt_len);

        } else if (nvt_bus->nvt_bu->tx_mode == NVT_BUS_BU_TX_SEQ) {
                pkt_len = roundup((ETH_FRAME_LEN + nvt_txhdr_sz),
                        NVT_SDIO_F1_BLOCKSIZE);
                skb = dev_alloc_skb(pkt_len);
                if (skb == NULL) {
                        nvt_dbg(ERROR, "%s: skb alloc fail!\n", __func__);
                        return -1;
                }

                for (i = nvt_txhdr_sz; i < pkt_len; i++) {
                        skb->data[i] = (i - nvt_txhdr_sz) & 0xFF;
                }
        } else if (nvt_bus->nvt_bu->tx_mode == NVT_BUS_BU_TX_RANDOM) {
                pkt_len = roundup((ETH_FRAME_LEN + nvt_txhdr_sz),
                        NVT_SDIO_F1_BLOCKSIZE);
                skb = dev_alloc_skb(pkt_len);
                if (skb == NULL) {
                        nvt_dbg(ERROR, "%s: skb alloc fail!\n", __func__);
                        return -1;
                }

                for (i = nvt_txhdr_sz; i < pkt_len; i++) {
                        skb->data[i] = (i - nvt_txhdr_sz) & 0xFF;
                }
        } else {
                return -1;
        }

        // bit[23:16]: offset
        // bit[15:00]: pkt len (hdr doesn't be included)
        tx_hdr_hi = ((pkt_len) & 0xffff);
        tx_hdr_hi |= (0x00080000);
        tx_hdr_hi = cpu_to_be32(tx_hdr_hi);

        skb_put(skb, pkt_len);
        memcpy(skb->data, (u8 *)&tx_hdr_lo, 4);
        memcpy((skb->data+4), (u8 *)&tx_hdr_hi, 4);

        ret = nvt_bus->nvt_wdev_bus_ops.tx_data(nvt_bus, skb);
        if (ret) {
                nvt_dbg(ERROR, "%s: nvt_bus_txdata fail!\n", __func__);
                dev_kfree_skb_any(skb);
                return -1;
        }

        return 0;
}

static s32 nvt_set_bus_utilization_tx(struct _nvt_bus *nvt_bus, void *data,
        u8 *mib_value_str)
{
        s32 i;
        s32 ret;
        u32 tx_cnt = 0;

        if (nvt_bus == NULL) {
                nvt_dbg(ERROR, "%s: nvt_bus is NULL!\n", __func__);
                return -EPERM;
        }

        ret = kstrtou32((s8 *)mib_value_str, 10, &tx_cnt);
        if (ret) {
                return -EINVAL;
        }

        if (tx_cnt > 10000000) {
                return -EINVAL;
        }


        ret = nvt_bus_bu_tx_on(nvt_bus, true, tx_cnt);

        for (i = 0; i < tx_cnt; i++) {

                ret = nvt_bu_send_pkt(nvt_bus);
                if (ret) {
                        nvt_dbg(ERROR, "%s: nvt_bu_send_pkt() fail!!\n",
                                __func__);
                        return -1;
                }
        }

        return 0;

}

static s32 nvt_set_bus_utilization_rx(struct _nvt_bus *nvt_bus, void *data,
        u8 *mib_value_str)
{
        s32 ret;
        s32 rx_cnt = 0;

        if (nvt_bus == NULL) {
                nvt_dbg(ERROR, "%s: nvt_bus is NULL!\n", __func__);
                return -EPERM;
        }

        ret = kstrtou32((s8 *)mib_value_str, 10, &rx_cnt);
        if (ret) {
                return -EINVAL;
        }

        if (rx_cnt > 10000000) {
                return -EINVAL;
        }

        nvt_bus_bu_rx_on(nvt_bus, true, rx_cnt);

        return 0;
}
#endif /* #ifdef BUS_UTILIZATION */

static s32 nvt_get_mode(struct _nvt_bus *nvt_bus, void *data,
        u8 *out_str, u32 *outlen)
{
        struct _nvt_if *nvt_if = netdev_priv((struct net_device *)data);
        u8 bssMode = nvt_if->mode;

        snprintf(out_str, NVT_IWPRIV_GETMIB_RSP_SIZE, "bss mode : %x", bssMode);
        *outlen = strlen(out_str);
        return 0;
}

static s32 nvt_get_umac_handler(struct _nvt_bus *nvt_bus, void *data,
        u8 *out_str, u32 *outlen)
{
        char *headers[] = {"rxAll", "rxDat", "rxMgm",
                "rxCrt", "rxDp", "amsdu", "ampdu",
                "rxIsr", "rxEmp", "queRx", "queTx"};
        u8 wid_val_pos;
        s32 wid_val_byte_len;
        u8 resp[512] = {0};
        u32 buff_size = 512;
        wid_val_pos = 0;
        wid_val_byte_len = nvt_get_wid_value(nvt_bus, WID_GET_UMAC_STATS,
                        resp, buff_size, &wid_val_pos);
        if (wid_val_byte_len < 0) {
                return -1;
        }
        nvt_byte_to_hex_str(&resp[wid_val_pos],
                        wid_val_byte_len, 4, out_str, headers);
        *outlen = strlen(out_str);
        return 0;
}

static s32 nvt_get_lmac_handler(struct _nvt_bus *nvt_bus, void *data,
        u8 *out_str, u32 *outlen)
{
        u8 val_start_pos;
        u8 i;
        s32 wid_val_byte_len;
        /*data headers*/
        char *rx_headers[] = {"rq1", "rq2"};
        //char *gmax_tx_done[] = {"gmax_tx_done"};
        //char *txqs[] = {"num_tx_q"};
        char *tx_ok_drop[10];
        //char *err[] = {"err"};
        char *error_bits[24];

        u8 resp[512] = {0};
        u32 buff_size = 512;
        val_start_pos = 0;
        /* AT 20150810: Format of the byte data read from fw
         * ======================================================
         * | rx_frame_1 | rx_frame_2 | g_max_tx_done | #Txqs    |
         * ======================================================
         * |     0:3    |    4:7     |     8:8       | 9:10     |
         * ======================================================
         * |  tx_ok_1   |  tx_drop_1 |    tx_ok_2    | tx_drop_2|
         * ======================================================
         * |   11:14    |   15:18    |      19:22    | 23:26    |
         * ======================================================
         * |  tx_ok_3   |  tx_drop_3 |    tx_ok_4    | tx_drop_4|
         * ======================================================
         * |   27:30    |   31:34    |      35:38    | 39:42    |
         * ======================================================
         * |  tx_ok_5   |  tx_drop_5 |    Error      | err_1    |
         * ======================================================
         * |   43:46    |   47:50    |      51:51    | 52:56    |
         * ======================================================
         * |   err_2    |  err_3     |    err_4      | err_5    |
         * ======================================================
         * |   57:60    |   61:64    |      65:68    | 69:72    |
         * ======================================================
         * ..... to err_24
         *
         *  */
        /*Init the data headers*/
        for (i = 0; i < 5; i++) {
                tx_ok_drop[2 * i] = kzalloc(10, GFP_KERNEL);
                snprintf(tx_ok_drop[2 * i], NVT_IWPRIV_GETMIB_RSP_SIZE,
                        "txOk%d", i);
                tx_ok_drop[2 * i + 1] = kzalloc(10, GFP_KERNEL);
                snprintf(tx_ok_drop[2 * i + 1], NVT_IWPRIV_GETMIB_RSP_SIZE,
                        "txDp%d", i);
        }
        /*Init the error headers*/
        for (i = 0; i < 20; i++) {
                error_bits[i] = kzalloc(13, GFP_KERNEL);
                snprintf(error_bits[i], NVT_IWPRIV_GETMIB_RSP_SIZE,
                        "err%d", i);
        }
        wid_val_byte_len = nvt_get_wid_value(nvt_bus, WID_GET_LMAC_STATS,
                        resp, buff_size, &val_start_pos);
        if (wid_val_byte_len < 0) {
                goto fail;
        }
        /*Starting concat strings  */
        /*rx_frame_1 and rx_frame_2*/
        nvt_byte_to_hex_str(&resp[val_start_pos],
                        8, 4, out_str, rx_headers);

        /*nvt_byte_to_hex_str(&resp[val_start_pos + 8],
                        1, 1, out_str + 23 * 2, gmax_tx_done);

        nvt_byte_to_hex_str(&resp[val_start_pos + 9],
                        2, 2, out_str + 23 * 3, txqs);*/

        /*tx_ok and tx_drop*/
        nvt_byte_to_hex_str(&resp[val_start_pos + 11],
                        40, 4, out_str + 15 * 2, tx_ok_drop);

        //nvt_byte_to_hex_str(&resp[val_start_pos + 51],
        //                1, 1, out_str + 23 * 14, err);

        nvt_byte_to_hex_str(&resp[val_start_pos + 52],
                        80, 4, out_str + 15 * 12, error_bits);
        *outlen = strlen(out_str);
fail:
        /*Free the buffers*/
        for (i = 0; i < 10; i++) {
                kfree(tx_ok_drop[i]);
        }
        /*Init the error headers*/
        for (i = 0; i < 20; i++) {
                kfree(error_bits[i]);
        }
        if (wid_val_byte_len < 0)
                return -1;
        else
                return 0;
}
static s32 nvt_iwpriv_getmib_fver(struct _nvt_bus *nvt_bus, void *data,
        u8 *out_str, u32 *out_len)

{
        return nvt_fw_get_ver(nvt_bus, out_str, out_len);
}
static s32 nvt_get_autorate_parameter(struct _nvt_bus *nvt_bus, void *data,
        u8 *out_str, u32 *outlen)
{
        u8 wid_val_pos;
        s32 wid_val_byte_len;
        u8 resp[512] = {0};
        u16 interval, divitor, remainder;
        u32 buff_size = 512;
        wid_val_pos = 0;
        wid_val_byte_len = nvt_get_wid_value(nvt_bus, WID_AR_PARAMETER,
                        resp, buff_size, &wid_val_pos);
        if (wid_val_byte_len < 0) {
                return -1;
        }
        divitor = resp[wid_val_pos+2];
        divitor = divitor << 8;
        remainder = resp[wid_val_pos+3];
        interval = divitor + remainder;
        snprintf(out_str, NVT_IWPRIV_GETMIB_RSP_SIZE,
                "%02u,%1u,%05u,%02u,%02u,%1u\n", resp[wid_val_pos],
                resp[wid_val_pos+1], interval, resp[wid_val_pos+4],
                resp[wid_val_pos+5], resp[wid_val_pos+6]);
        *outlen = strlen(out_str);
        return 0;
}
static s32 nvt_set_autorate_parameter(struct _nvt_bus *nvt_bus, void *data,
        u8 *mib_value_str)
{
        u8 const *delim = ",";
        char *mib_val;
        char set_val[14] = {0};
        int  n = 0, i;
        u8 *str_tmp;
        s32 ret;
        while ((mib_val = strsep((char **)&mib_value_str, delim)) != NULL) {
                n += sprintf(set_val+n, "%s", mib_val);
        }
        str_tmp = kzalloc(strlen(set_val)*3, GFP_KERNEL);
        if (str_tmp == NULL) {
                nvt_dbg(ERROR, "%s: kzalloc fail\n", __func__);
                return -ENOMEM;
        }
        for (i = 0; i < strlen(set_val); i++) {
                if (i == strlen(set_val) - 1) {
                        snprintf((str_tmp + i*3),
                        (strlen(set_val) - i) * 3,
                         "%02x",
                        *(set_val + i));
                } else {
                        snprintf((str_tmp + i*3),
                                (strlen(set_val) - i) * 3,
                                "%02x,",
                                *(set_val + i));
                }
        }
        ret = nvt_send_wid_set(nvt_bus, WID_AR_PARAMETER, str_tmp);
        kfree(str_tmp);
        return ret;
}

static s32 nvt_set_channel_handler(struct _nvt_bus *nvt_bus, void *data,
                u8 *mib_value_str)
{
        struct _nvt_if *nvt_if = netdev_priv((struct net_device *)data);
        struct _nvt_wlan_conf_ap *wconf_ap = &nvt_if->nvt_wconf_ap;
        s32 ret = 0;
        u8 acs_channel = 0;

        if (mib_value_str == NULL) {
                return -EINVAL;
        }
        ret = kstrtou8(mib_value_str, 10, &acs_channel);
        if (ret < 0) {
                return -EINVAL;
        }

        if (acs_channel <= 14) {
                nvt_dbg(CFG80211, "%s:acs_channel=%d\n", __func__, acs_channel);
                wconf_ap->acs_channel = acs_channel;
        } else if ((36 <= acs_channel) && (acs_channel <= 165)) {
                nvt_dbg(ERROR, "%s:5G not implement yet\n", __func__);
                //wconf->acs_channel = acs_channel;
                return -EINVAL;
        } else {
                nvt_dbg(ERROR, "%s:invalid ch=%d\n", __func__, acs_channel);
                return -EINVAL;
        }
        return ret;
}

static s32 nvt_get_channel_handler(struct _nvt_bus *nvt_bus, void *data,
        u8 *out_str, u32 *outlen) {
        struct _nvt_if *nvt_if = netdev_priv((struct net_device *)data);
        struct _nvt_wlan_conf_ap *wconf_ap = &nvt_if->nvt_wconf_ap;

        u8 acs_channel = wconf_ap->acs_channel;
        snprintf(out_str, NVT_IWPRIV_GETMIB_RSP_SIZE, "%d", acs_channel);
        *outlen = strlen(out_str);
        return 0;

}

static s32 nvt_set_acs_mask_handler(struct _nvt_bus *nvt_bus, void *data,
                u8 *mib_value_str)
{
        struct _nvt_if *nvt_if = netdev_priv((struct net_device *)data);
        struct _nvt_wlan_conf_ap *wconf_ap = &nvt_if->nvt_wconf_ap;
        s32 ret = 0;
        u32 acs_mask = 0;

        if (mib_value_str == NULL) {
                return -EINVAL;
        }
        ret = kstrtou32(mib_value_str, 10, &acs_mask);
        if (ret < 0) {
                return -EINVAL;
        }
        if (acs_mask < (1 << 15)) {
                nvt_dbg(CFG80211, "%s:ch_mask=0x%x\n", __func__, acs_mask);
                wconf_ap->acs_mask = acs_mask;
        } else {
                nvt_dbg(ERROR, "%s:invalid mask=0x%x\n", __func__, acs_mask);
                return -EINVAL;
        }
        return ret;
#if 0
        while ((val = strsep((char **)&mib_value_str, delim)) != NULL) {
                ret = kstrtou32(val, 10, &int_val);
                if (ret < 0) {
                        return -EINVAL;
                }
                nvt_dbg(ERROR, "%s:val=%d, int_val=%d\n", __func__, *val,
                        int_val);
                if ((1 <= int_val) && (int_val <= 14)) {
                        temp |= (1 << (int_val - 1));
                } else if ((36 <= int_val) && (int_val <= 165)) {
                        nvt_dbg(ERROR, "%s:5G not implement yet\n", __func__);
                        return -EINVAL;
                } else {
                        nvt_dbg(ERROR, "%s:invalid ch=%d\n", __func__, int_val);
                        return -EINVAL;
                }
        }
        wconf_ap->acs_mask = temp;
        return ret;
#endif
}

static s32 nvt_get_acs_mask_handler(struct _nvt_bus *nvt_bus, void *data,
        u8 *out_str, u32 *outlen) {
        struct _nvt_if *nvt_if = netdev_priv((struct net_device *)data);
        struct _nvt_wlan_conf_ap *wconf_ap = &nvt_if->nvt_wconf_ap;

        u32 acs_mask = wconf_ap->acs_mask;
        snprintf(out_str, NVT_IWPRIV_GETMIB_RSP_SIZE, "%d", acs_mask);
        *outlen = strlen(out_str);
        return 0;
}

static s32 nvt_set_wow_scanfrq(struct _nvt_bus *nvt_bus, void *data,
                        u8 *mib_value_str)
{
        struct _nvt_if *nvt_if = netdev_priv((struct net_device *)data);
        u8 const *delim = ",";
        s32 ret = 0;
        u8 *mib_val = NULL;
        u8 i = 0;
        u8 index = 0;
        u32 int_val = 0;
        u8 *recover_buf = nvt_if->recover_buf;

        if (mib_value_str == NULL) {
                return -EINVAL;
        }
        while ((mib_val = strsep((char **)&mib_value_str, delim)) != NULL) {
                ret = kstrtou32(mib_val, 10, &int_val);
                if (ret < 0) {
                        goto fail;
                }

                //sanity check
                switch (i) {
                case 0: /*enable */
                        if (int_val > 1) {
                                nvt_dbg(ERROR, "%s:invalid enable val=%d\n",
                                                       __func__, int_val);
                                goto fail;
                        }
                        recover_buf[index++] = (u8)int_val;
                        break;
                case 1: /* scan duty */
                case 6: /* per channel scan timeout */
                        if (int_val > 65535) {
                                nvt_dbg(ERROR, "%s:invalid enable val=%d\n",
                                                __func__, int_val);
                                goto fail;
                        }
                        recover_buf[index++] = (u8)int_val & 0xff;
                        recover_buf[index++] = (u8)(int_val >> 8) & 0xff;
                        break;
                case 4: /* recover timeout MAX timeout value is one day*/
                        if (int_val > 86400) {
                                nvt_dbg(ERROR, "%s:invalid enable val=%d\n",
                                                __func__, int_val);
                                goto fail;
                        }
                        recover_buf[index++] = (u8)int_val & 0xff;
                        recover_buf[index++] = (u8)(int_val >> 8) & 0xff;
                        recover_buf[index++] = (u8)(int_val >> 16) & 0xff;
                        recover_buf[index++] = (u8)(int_val >> 24) & 0xff;
                        break;
                case 2: /* scan repeat times */
                case 3: /* scan exp */
                case 5: /* scan type */
                        if (int_val > 255) {
                                nvt_dbg(ERROR, "%s:invalid scan repeat=%d\n",
                                                __func__, int_val);
                                goto fail;
                        }
                        recover_buf[index++] = (u8)int_val;
                        break;
                default:
                        nvt_dbg(ERROR, "%s:invalid index\n", __func__);
                        goto fail;
                }
                i++;
        }

        if (i == 7) {
                return ret;
        } else {
                nvt_dbg(ERROR, "%s:invalid input parameter number\n",
                                __func__);
        }
fail:
        return -EINVAL;
}
static s32 nvt_set_acs_weight_handler(struct _nvt_bus *nvt_bus, void *data,
                u8 *mib_value_str)
{
        struct _nvt_if *nvt_if = netdev_priv((struct net_device *)data);
        struct _nvt_wlan_conf_ap *wconf_ap = &nvt_if->nvt_wconf_ap;
        u8 const *delim = ",";
        s32 ret = 0;
        u32 int_val = 0;
        u8 *mib_val = NULL;
        u8 temp[sizeof(wconf_ap->acs_weight)] = {0};
        u8 i = 0;

        if (mib_value_str == NULL) {
                return -EINVAL;
        }
        while ((mib_val = strsep((char **)&mib_value_str, delim)) != NULL) {
                ret = kstrtou32(mib_val, 10, &int_val);
                if (ret < 0) {
                        goto fail;
                }
                if (int_val > 127) {
                        nvt_dbg(ERROR, "%s:invalid val=%d\n",
                                        __func__, int_val);
                        goto fail;
                }
                if (i == sizeof(wconf_ap->acs_weight)) {
                        nvt_dbg(ERROR, "%s:over length, need 19\n", __func__);
                        goto fail;
                }
                temp[i] = (u8)int_val;
                i++;
        }

        if (i != sizeof(wconf_ap->acs_weight)) {
                nvt_dbg(ERROR, "%s:under length, need 19\n", __func__);
                        goto fail;
        }
        memcpy(wconf_ap->acs_weight, temp, sizeof(wconf_ap->acs_weight));

        return ret;
fail:
        wconf_ap->acs_weight[0] = 0xFF;
        return -EINVAL;

}

static s32 nvt_get_acs_weight_handler(struct _nvt_bus *nvt_bus, void *data,
        u8 *out_str, u32 *outlen) {
        char *headers[] = {" RSSI_1", " RSSI_2", " RSSI_3",
                        " Coch_1", " Coch_2", " Coch_3", " Coch_4",
                        " Adj1_1", " Adj1_2", " Adj1_3", " Adj1_4",
                        " Adj2_1", " Adj2_2", " Adj2_3", " Adj2_4",
                        " Adj3_1", " Adj3_2", " Adj3_3", " Adj3_4",};
        struct _nvt_if *nvt_if = netdev_priv((struct net_device *)data);
        struct _nvt_wlan_conf_ap *wconf_ap = &nvt_if->nvt_wconf_ap;
        u8 i;
        char *cur = out_str;
        char * const end = out_str + NVT_IWPRIV_GETMIB_RSP_SIZE;
        for (i = 0; i < sizeof(wconf_ap->acs_weight); i++) {
                if (cur < end) {
                        cur += snprintf(cur, end-cur, "%s:%d",
                                        headers[i], wconf_ap->acs_weight[i]);
                }
        }
        *outlen = strlen(out_str);
        return 0;
}

//The command line arguments are put as follows:
//ARG1:1BYTE:ht or non-ht
//ARG2:1BYTE:rate code
//ARG3:1BYTE:disable
//ARG4:1BYTE:min power level
//ARG5:1BYTE:max power level
//ARG6:1BYTE:power level step
//ARG7:1BYTE:num of no rate changes [MAX-RATE use this param]
//ARG8:1BYTE:back-off-counter-threshold
//ARG9:2BYTE:num_of_pkts_to_select
//ARG10:2BYTE:retry low water mark
//ARG11:2BYTE:retry high water mark

s32 nvt_set_tx_power_adaption_params(struct _nvt_bus *nvt_bus, void *data,
        u8 *mib_value_str)
{
        //start parsing the cmd line arguments
        u8 const *delim = ",";
        u8 *wid_val_buf;
        s32 ret = 0;
        u8 ar_pwr_tbl;
        u8 r_code;
        u8 disable;
        u8 min_p_l, max_p_l, p_l_step;
        u8 num_of_no_r_changes;
        u8 b_off_cnt;
        u16 num_of_p_2_sel;
        u16 r_l_wmark;
        u16 r_h_wmark;

        char *ar_tbl_type = strsep((char **)&mib_value_str, delim);
        char *rate_code = strsep((char **)&mib_value_str, delim);
        char *disable_flag = strsep((char **)&mib_value_str, delim);
        char *min_power_level = strsep((char **)&mib_value_str, delim);
        char *max_power_level = strsep((char **)&mib_value_str, delim);
        char *power_level_step = strsep((char **)&mib_value_str, delim);
        char *num_of_no_rate_changes = strsep((char **)&mib_value_str,
                delim);
        char *back_off_counter_th = strsep((char **)&mib_value_str,
                delim);
        char *num_of_pkts_to_sel = strsep((char **)&mib_value_str, delim);
        char *num_of_retry_low_watermark = strsep((char **)&mib_value_str,
                delim);
        char *num_of_retry_high_watermark = mib_value_str;

        //sanity check
        if (!(ar_tbl_type && rate_code && disable_flag && min_power_level &&
                max_power_level && power_level_step && num_of_no_rate_changes &&
                back_off_counter_th && num_of_pkts_to_sel &&
                num_of_retry_low_watermark && num_of_retry_high_watermark)) {
                return -EINVAL;
        }
        //transform the str to u8 or u16
        kstrtou8(ar_tbl_type, 16, &ar_pwr_tbl);
        kstrtou8(rate_code, 16, &r_code);
        kstrtou8(disable_flag, 16, &disable);
        kstrtou8(min_power_level, 16, &min_p_l);
        kstrtou8(max_power_level, 16, &max_p_l);
        kstrtou8(power_level_step, 16, &p_l_step);
        kstrtou8(num_of_no_rate_changes, 16, &num_of_no_r_changes);
        kstrtou8(back_off_counter_th, 16, &b_off_cnt);
        kstrtou16(num_of_pkts_to_sel, 16, &num_of_p_2_sel);
        kstrtou16(num_of_retry_low_watermark, 16, &r_l_wmark);
        kstrtou16(num_of_retry_high_watermark, 16, &r_h_wmark);

        //layout the buffer in big endian fashion if the data size
        //larger than one byte
        wid_val_buf = kzalloc(14, GFP_KERNEL);
        if (wid_val_buf == NULL) {
                ret = -1;
                goto fail;
        }
        wid_val_buf[0] = ar_pwr_tbl;
        wid_val_buf[1] = r_code;
        wid_val_buf[2] = disable;
        wid_val_buf[3] = min_p_l;
        wid_val_buf[4] = max_p_l;
        wid_val_buf[5] = p_l_step;
        wid_val_buf[6] = num_of_no_r_changes;
        wid_val_buf[7] = b_off_cnt;
        wid_val_buf[8] = (num_of_p_2_sel >> 8) & 0xFF;
        wid_val_buf[9] = num_of_p_2_sel & 0xFF;
        wid_val_buf[10] = (r_l_wmark >> 8) & 0xFF;
        wid_val_buf[11] = r_l_wmark & 0xFF;
        wid_val_buf[12] = (r_h_wmark >> 8) & 0xFF;
        wid_val_buf[13] = r_h_wmark & 0xFF;

        ret = nvt_send_wid_by_numeric(nvt_bus, WID_TX_POWER_ADAPTION,
                wid_val_buf, 14);
        if (ret < 0) {
                ret = -1;
                goto fail;
        }

fail:
        kfree(wid_val_buf);
        return ret;
}

static s32 nvt_set_dtim_scalar_handler(struct _nvt_bus *nvt_bus, void *data,
                u8 *mib_value_str)
{
        struct _nvt_if *nvt_if = netdev_priv((struct net_device *)data);
        s32 ret = 0;
        u32 temp_val = 0;

        if (mib_value_str == NULL) {
                return -EINVAL;
        }
        ret = kstrtou32(mib_value_str, 10, &temp_val);
        if (ret < 0) {
                return -EINVAL;
        }
        if (temp_val == 0) {
                return -EINVAL;
        }
        nvt_if->skip_dtim_scalar = temp_val;
        nvt_dbg(CFG80211, "%s, dtim_scalar=%d\n",
                __func__, nvt_if->skip_dtim_scalar);
        return ret;
}

static s32 nvt_get_dtim_scalar_handler(struct _nvt_bus *nvt_bus, void *data,
        u8 *out_str, u32 *outlen)
{
        struct _nvt_if *nvt_if = netdev_priv((struct net_device *)data);

        u32 temp_val = nvt_if->skip_dtim_scalar;
        snprintf(out_str, NVT_IWPRIV_GETMIB_RSP_SIZE, "%d", temp_val);
        *outlen = strlen(out_str);
        return 0;
}

static s32 nvt_set_dtim_time_handler(struct _nvt_bus *nvt_bus, void *data,
                u8 *mib_value_str)
{
        struct _nvt_if *nvt_if = netdev_priv((struct net_device *)data);
        s32 ret = 0;
        u32 temp_val = 0;

        if (mib_value_str == NULL) {
                return -EINVAL;
        }
        ret = kstrtou32(mib_value_str, 10, &temp_val);
        if (ret < 0) {
                return -EINVAL;
        }
        if (temp_val == 0) {
                return -EINVAL;
        }
        nvt_if->skip_dtim_time = temp_val;
        nvt_dbg(CFG80211, "%s, dtim_time=%d\n",
                __func__, nvt_if->skip_dtim_time);
        return ret;
}

static s32 nvt_get_dtim_time_handler(struct _nvt_bus *nvt_bus, void *data,
        u8 *out_str, u32 *outlen)
{
        struct _nvt_if *nvt_if = netdev_priv((struct net_device *)data);

        u32 temp_val = nvt_if->skip_dtim_time;
        snprintf(out_str, NVT_IWPRIV_GETMIB_RSP_SIZE, "%d", temp_val);
        *outlen = strlen(out_str);
        return 0;
}

static s32 nvt_set_ignore_bcn_bitmap_handler(struct _nvt_bus *nvt_bus,
        void *data, u8 *mib_value_str)
{
        struct _nvt_if *nvt_if = netdev_priv((struct net_device *)data);
        s32 ret = 0;
        u32 temp_val = 0;

        if (mib_value_str == NULL) {
                return -EINVAL;
        }
        ret = kstrtou32(mib_value_str, 10, &temp_val);
        if (ret < 0) {
                return -EINVAL;
        }
        nvt_if->ignore_bcn_bitmap = temp_val;
        nvt_dbg(CFG80211, "%s:ignore_bcn_bitmap=%d\n",
                __func__, nvt_if->ignore_bcn_bitmap);
        return ret;
}

static s32 nvt_get_ignore_bcn_bitmap_handler(struct _nvt_bus *nvt_bus,
        void *data, u8 *out_str, u32 *outlen)
{
        struct _nvt_if *nvt_if = netdev_priv((struct net_device *)data);

        u32 temp_val = nvt_if->ignore_bcn_bitmap;
        snprintf(out_str, NVT_IWPRIV_GETMIB_RSP_SIZE, "%d", temp_val);
        *outlen = strlen(out_str);
        return 0;
}

static s32 nvt_set_queue_cnt_handler(struct _nvt_bus *nvt_bus,
        void *data, u8 *mib_value_str)
{
        struct _nvt_if *nvt_if = netdev_priv((struct net_device *)data);
        struct _nvt_adapter *nvt_adapter = nvt_if->nvt_adapter;
        struct _tx_info *tx_info = nvt_adapter->nvt_priv.tx_info;
        s32 ret;
        u32 max_qcnt = 0;

        if (mib_value_str == NULL) {
                return -EINVAL;
        }
        ret = kstrtou32(mib_value_str, 10, &max_qcnt);
        if (ret < 0) {
                return -EINVAL;
        }
        tx_info->max_qcnt = max_qcnt;
        return ret;
}

static s32 nvt_get_queue_cnt_handler(struct _nvt_bus *nvt_bus,
        void *data, u8 *out_str, u32 *outlen)
{
        struct _nvt_if *nvt_if = netdev_priv((struct net_device *)data);
        struct _nvt_adapter *nvt_adapter = nvt_if->nvt_adapter;
        struct _tx_info *tx_info = nvt_adapter->nvt_priv.tx_info;

        u32 temp_val = tx_info->max_qcnt;
        snprintf(out_str, NVT_IWPRIV_GETMIB_RSP_SIZE, "%d", temp_val);
        *outlen = strlen(out_str);
        return 0;
}

static s32 nvt_set_testmode_handler(struct _nvt_bus *nvt_bus, void *data,
                u8 *mib_value_str)
{
        struct _nvt_if *nvt_if = netdev_priv((struct net_device *)data);
        struct _nvt_adapter *nvt_adapter = nvt_if->nvt_adapter;
        struct _tx_info *tx_info = nvt_adapter->nvt_priv.tx_info;
        s32 ret;
        u32 testmode = 0;

        if (mib_value_str == NULL) {
                return -EINVAL;
        }
        ret = kstrtou32(mib_value_str, 10, &testmode);
        if (ret < 0) {
                return -EINVAL;
        }
        if (testmode < 0) {
                return -EINVAL;
        }
        memset(&tx_info->nvt_tx_test, 0, sizeof(struct _nvt_tx_test));
        tx_info->nvt_tx_test.tx_test_mode = testmode;
        return ret;
}

static s32 nvt_get_testmode_handler(struct _nvt_bus *nvt_bus, void *data,
        u8 *out_str, u32 *outlen)
{
        struct _nvt_if *nvt_if = netdev_priv((struct net_device *)data);
        struct _nvt_adapter *nvt_adapter = nvt_if->nvt_adapter;
        struct _tx_info *tx_info = nvt_adapter->nvt_priv.tx_info;
        struct _nvt_tx_test nvt_tx_test = tx_info->nvt_tx_test;
        char *headers[] = {" TESTMODE", " TOTAL_CNT", " ENQUE_CNT",
                        " ENQUE_DROP_CNT", " DEQUE_CNT", " DEQUE_DROP_CNT",
                        " START_TIME", " END_TIME",
                        " ENQ_VO", " ENQ_VI", " ENQ_BE", " ENQ_BK",
                        " CRED_VO", " CRED_VI", " CRED_BE", " CRED_BK",
                        " CRED_PUB"};
        char *cur = out_str;
        char * const end = out_str + NVT_IWPRIV_GETMIB_RSP_SIZE;
        cur += snprintf(cur, end-cur, "%s:%d",
                        headers[0], nvt_tx_test.tx_test_mode);
        cur += snprintf(cur, end-cur, "%s:%d",
                        headers[1], nvt_tx_test.tx_total_cnt);
        cur += snprintf(cur, end-cur, "%s:%d",
                        headers[2], nvt_tx_test.tx_enque_cnt);
        cur += snprintf(cur, end-cur, "%s:%d",
                        headers[3], nvt_tx_test.rx_enque_drop_cnt);
        cur += snprintf(cur, end-cur, "%s:%d",
                        headers[4], nvt_tx_test.tx_deque_cnt);
        cur += snprintf(cur, end-cur, "%s:%d",
                        headers[5], nvt_tx_test.tx_deque_drop_cnt);
        cur += snprintf(cur, end-cur, "%s:%lu",
                        headers[6], nvt_tx_test.start_time);
        cur += snprintf(cur, end-cur, "%s:%lu",
                        headers[7], nvt_tx_test.end_time);
        cur += snprintf(cur, end-cur, "%s:%d",
                        headers[8], tx_info->fifo_enqpkt[NV_TX_FIFO_AC_VO]);
        cur += snprintf(cur, end-cur, "%s:%d",
                        headers[9], tx_info->fifo_enqpkt[NV_TX_FIFO_AC_VI]);
        cur += snprintf(cur, end-cur, "%s:%d",
                        headers[10], tx_info->fifo_enqpkt[NV_TX_FIFO_AC_BE]);
        cur += snprintf(cur, end-cur, "%s:%d",
                        headers[11], tx_info->fifo_enqpkt[NV_TX_FIFO_AC_BK]);
        cur += snprintf(cur, end-cur, "%s:%d",
                        headers[12], tx_info->fifo_credit[NV_TX_FIFO_AC_VO]);
        cur += snprintf(cur, end-cur, "%s:%d",
                        headers[13], tx_info->fifo_credit[NV_TX_FIFO_AC_VI]);
        cur += snprintf(cur, end-cur, "%s:%d",
                        headers[14], tx_info->fifo_credit[NV_TX_FIFO_AC_BE]);
        cur += snprintf(cur, end-cur, "%s:%d",
                        headers[15], tx_info->fifo_credit[NV_TX_FIFO_AC_BK]);
        cur += snprintf(cur, end-cur, "%s:%d",
                        headers[16], tx_info->fifo_credit[NV_TX_FIFO_PUBLIC]);

        *outlen = strlen(out_str);
        return 0;
}

static struct _nvt_iwpriv_mib mib_cmd_tbl[] = {
        {"pwm", WID_POWER_MANAGEMENT,   /* name, wid */
               1,                      /* len_field_size */
               REQ_CHAR_TYPE,          /* req_format */
               RSP_FORMAT_ENUM_STRING, /* resp_format */
               str_power_save_mode,    /* str_enum */
               6,                      /* enum_num */
               NULL,                   /* setmib_handler */
               NULL},                  /* getmib_handler */

        {"fc", WID_FC_ENABLE,
               1,
               REQ_CHAR_TYPE,
               RSP_FORMAT_HEX,
               NULL,
               0,
               NULL,
               NULL},

        #ifdef CONFIG_WIFI_TUNING_PHASE_II
        {"tpm_start", WID_TP_MONITOR,
               1,
               REQ_CHAR_TYPE,
               RSP_FORMAT_HEX,
               NULL,
               0,
               NULL,
               NULL},

        {"tpm_interval", WID_TP_MIN_INTERVAL,
                1,
                REQ_INT_TYPE,
                RSP_FORMAT_HEX,
                NULL,
                0,
                NULL,
                NULL},

        {"tpm_sta_stats", 0xFFFF,
                1,
                REQ_CHAR_TYPE,
                RSP_FORMAT_ENUM_STRING,
                NULL,
                0,
                NULL,
                nvt_tpm_sta_stats},

        #endif

        {"fcb", 0xFFFF,
               1,
               REQ_CHAR_TYPE,
               RSP_FORMAT_HEX,
               NULL,
               0,
               nvt_set_fc_borrow,
               nvt_get_fc_borrow_flag},

        {"sgi", WID_11N_SHORT_GI_ENABLE,
               1,
               REQ_CHAR_TYPE,
               RSP_FORMAT_ENUM_STRING,
               str_short_gi,
               4,
               NULL,
               NULL},

        {"rxrdy", WID_ACS_RXRDY_THRESH,
               1,
               REQ_CHAR_TYPE,
               RSP_FORMAT_HEX,
               NULL,
               0,
               NULL,
               NULL},

        {"rts", WID_RTS_THRESHOLD,
               1,
               REQ_SHORT_TYPE,
               RSP_FORMAT_HEX,
               NULL,
               0,
               NULL,
               NULL},

        {"scan_ch_time", WID_AP_SCAN_STAY_CH_TIME,
               1,
               REQ_SHORT_TYPE,
               RSP_FORMAT_HEX,
               NULL,
               0,
               NULL,
               NULL},

        {"scan_resume_time", WID_AP_RESUME_STAY_CH_TIME,
               1,
               REQ_SHORT_TYPE,
               RSP_FORMAT_HEX,
               NULL,
               0,
               NULL,
               NULL},

        {"ochan", WID_CURR_OPER_CHANNEL,
               1,
               REQ_SHORT_TYPE,
               RSP_FORMAT_HEX,
               NULL,
               0,
               NULL,
               NULL},

        {"ht40", WID_2040_ENABLE,
               1,
               REQ_CHAR_TYPE,
               RSP_FORMAT_ENUM_STRING,
               str_enable_disable,
               2,
               NULL,
               NULL},

        {"20/40", WID_2040_COEXISTENCE,
               1,
               REQ_CHAR_TYPE,
               RSP_FORMAT_ENUM_STRING,
               str_enable_disable,
               2,
               NULL,
               NULL},

        {"qos", WID_QOS_ENABLE,
               1,
               REQ_CHAR_TYPE,
               RSP_FORMAT_ENUM_STRING,
               str_enable_disable,
               2,
               NULL,
               NULL},

        {"gmode", WID_11G_OPERATING_MODE,
               1,
               REQ_CHAR_TYPE,
               RSP_FORMAT_ENUM_STRING,
               str_legacy_mode,
               4,
               NULL,
               NULL},

        {"nmode", WID_11N_ENABLE,
               1,
               REQ_CHAR_TYPE,
               RSP_FORMAT_ENUM_STRING,
               str_enable_disable,
               2,
               NULL,
               NULL},

        {"tx_rate", WID_CURRENT_TX_RATE,
               1,
               REQ_CHAR_TYPE,
               RSP_FORMAT_HEX,
               NULL,
               0,
               NULL,
               NULL},

        {"mcs", WID_11N_CURRENT_TX_MCS,
               1,
               REQ_CHAR_TYPE,
               RSP_FORMAT_HEX,
               NULL,
               0,
               NULL,
               NULL},

        {"ctxbw", WID_11N_CURRENT_TX_BW,
               1,
               REQ_CHAR_TYPE,
               RSP_FORMAT_ENUM_STRING,
               str_enable_disable,
               2,
               NULL,
               NULL},

        {"pream", WID_PREAMBLE,
               1,
               REQ_CHAR_TYPE,
               RSP_FORMAT_HEX,
               NULL,
               0,
               NULL,
               NULL},

        {"rfrag", WID_RECEIVED_FRAGMENT_COUNT,
               1,
               REQ_INT_TYPE,
               RSP_FORMAT_HEX,
               NULL,
               0,
               NULL,
               NULL},

        {"pact", WID_11N_P_ACTION_REQ,
               1,
               REQ_STR_TYPE,
               RSP_FORMAT_STRING,
               NULL,
               0,
               NULL,
               NULL},

        {"umac", 0xFFFF,
               0,
               0,
               0,
               NULL,
               0,
               NULL,
               nvt_get_umac_handler},

        {"lmac", 0xFFFF,
               0,
               0,
               0,
               NULL,
               0,
               NULL,
               nvt_get_lmac_handler},

        {"mode", 0xFFFF,
               0,
               0,
               0,
               NULL,
               0,
               NULL,
               nvt_get_mode},

        {"rate", 0xFFFF,
               0,
               0,
               0,
               NULL,
               0,
               nvt_set_rate_handler,
               nvt_get_rate_handler},

        {"hwater", 0xFFFF,
               0,
               0,
               0,
               NULL,
               0,
               nvt_set_hwatermark_handler,
               nvt_get_hwatermark_handler},

        {"lwater", 0xFFFF,
               0,
               0,
               0,
               NULL,
               0,
               nvt_set_lwatermark_handler,
               nvt_get_lwatermark_handler},

        {"fver", 0xFFFF,
               0,
               0,
               0,
               NULL,
               0,
               NULL,
               nvt_iwpriv_getmib_fver},

        /* control firmware UART mode */
        {"fuart", WID_UART_MODE,
               1,
               REQ_INT_TYPE,
               RSP_FORMAT_HEX,
               NULL,
               0,
               NULL,
               NULL},

        /* control firmware debug level */
        {"fdbg", WID_DBG_LEVEL,
               1,
               REQ_INT_TYPE,
               RSP_FORMAT_HEX,
               NULL,
               0,
               NULL,
               NULL},

        {"offload", 0xFFFF,
               0,
               0,
               0,
               NULL,
               0,
               nvt_set_offload_handler,
               nvt_get_offload_handler},

        {"dhcp_renew", 0xFFFF,
               0,
               0,
               0,
               NULL,
               0,
               nvt_dhcp_renew_handler,
               0},

        {"keep_alive_offload", 0xFFFF,
               0,
               0,
               0,
               NULL,
               0,
               nvt_ka_handler,
               0},
#ifdef CONFIG_PM
        {"wow", 0xFFFF,
               0,
               0,
               0,
               NULL,
               0,
               nvt_set_wow_handler,
               nvt_get_wow_handler},
#endif
        //20160406 Wade: autorate parameter
        {"autorate", 0xFFFF,
               0,
               0,
               0,
               NULL,
               0,
               nvt_set_autorate_parameter,
               nvt_get_autorate_parameter},

        {"tx_power_adapt", 0xFFFF,
               0,
               0,
               0,
               NULL,
               0,
               nvt_set_tx_power_adaption_params,
               0},

        {"scanfrq", 0xFFFF,
               0,
               0,
               0,
               NULL,
               0,
               nvt_set_wow_scanfrq,
               NULL},

#ifdef BUS_UTILIZATION
        //20160322 nash:
        {"butxmode", 0xFFFF,
               0,
               0,
               0,
               NULL,
               0,
               nvt_set_bus_utilization_mode,
               nvt_get_bus_utilization_mode},

        //20160122 nash: bus utilization
        {"butx", 0xFFFF,
               0,
               0,
               0,
               NULL,
               0,
               nvt_set_bus_utilization_tx,
               NULL},

        {"burx", 0xFFFF,
               0,
               0,
               0,
               NULL,
               0,
               nvt_set_bus_utilization_rx,
               NULL},
#endif

        {"channel", 0xFFFF,
                0,
                0,
                0,
                NULL,
                0,
                nvt_set_channel_handler,
                nvt_get_channel_handler},

        {"autoch_maskmap_2g", 0xFFFF,
                0,
                0,
                0,
                NULL,
                0,
                nvt_set_acs_mask_handler,
                nvt_get_acs_mask_handler},

        {"acs_weight", 0xFFFF,
                0,
                0,
                0,
                NULL,
                0,
                nvt_set_acs_weight_handler,
                nvt_get_acs_weight_handler},

        {"dtim_scalar", 0xFFFF,
                0,
                0,
                0,
                NULL,
                0,
                nvt_set_dtim_scalar_handler,
                nvt_get_dtim_scalar_handler},

        {"dtim_time", 0xFFFF,
                0,
                0,
                0,
                NULL,
                0,
                nvt_set_dtim_time_handler,
                nvt_get_dtim_time_handler},

        {"ignore_bcn_bitmap", 0xFFFF,
                0,
                0,
                0,
                NULL,
                0,
                nvt_set_ignore_bcn_bitmap_handler,
                nvt_get_ignore_bcn_bitmap_handler},

        {"gfield", WID_ENABLE_GREENFIELD,
               1,
               REQ_CHAR_TYPE,
               RSP_FORMAT_HEX,
               NULL,
               0,
               NULL,
               NULL},

        {"qcnt", 0xFFFF,
                0,
                0,
                0,
                NULL,
                0,
                nvt_set_queue_cnt_handler,
                nvt_get_queue_cnt_handler},

        {"testmode", 0xFFFF,
                0,
                0,
                0,
                NULL,
                0,
                nvt_set_testmode_handler,
                nvt_get_testmode_handler},

        {NULL, 0xFFFF,
               0,
               0,
               0,
               NULL,
               0,
               NULL,
               NULL}
};
/**
 * nvt_byte_to_hex_str - transform the byte array to the human
 * readable hex
 * @wid_start_val: this is the pointer pointed to the started
 * byte address of the started wid value
 * @byte_len: how many bytes are in the byte array
 * [CAUTION:]Only counts for the real value!
 *
 * @data_byte: for short this value is 2, for int this value
 * is 4. In the nutshell, how many bytes are bundle for one
 * hex value.
 * @header_str: is the header name for each of the data
 * [CAUTION]I assume each header len will not exceed 5byte!
 */
static s32 nvt_byte_to_hex_str(u8 *wid_start_val, s32 byte_len,
                u8 data_byte, u8 *out_str, char **header_str)
{
        s32 i;
        s32 j;
        s32 remain_byte;
        u32 wid_val;
        for (i = 0; data_byte * i < byte_len; i++) {
                wid_val = 0;
                remain_byte = byte_len - data_byte * i;
                if (remain_byte > data_byte) {
                        remain_byte = data_byte;
                }
                for (j = 0; j < remain_byte; j++) {
                        wid_val += (*(wid_start_val + data_byte * i
                                        + remain_byte - j - 1)) << (8 * j);
                }
                snprintf(out_str + 15 * i, NVT_IWPRIV_GETMIB_RSP_SIZE,
                        "%5s:%08x\n", header_str[i], wid_val);
        }
        return 0;
}

s32 nvt_parse_rate_0(u8 wid_val)

{
        s32 retval;
        /*test b7 => HT or non-HT*/
        if (wid_val & 0x0080) {
                retval = (0x0001 & wid_val) + (0x0002 & wid_val) +
                        (0x0004 & wid_val);
        /*non-HT*/
        } else {
                wid_val = (0x0001 & wid_val) + (0x0002 & wid_val) +
                        (0x0004 & wid_val) + (0x0008 & wid_val);
                /*b rate*/
                if (wid_val == 4) {
                        retval = 1000;
                } else if (wid_val == 1 || wid_val == 5) {
                        retval = 2000;
                } else if (wid_val == 2 || wid_val == 6) {
                        retval = 5500;
                } else if (wid_val == 3 || wid_val == 7) {
                        retval = 11000;
                /*g rate*/
                } else if (wid_val == 11) {
                        retval = 6000;
                } else if (wid_val == 15) {
                        retval = 9000;
                } else if (wid_val == 10) {
                        retval = 12000;
                } else if (wid_val == 14) {
                        retval = 18000;
                } else if (wid_val == 9) {
                        retval = 24000;
                } else if (wid_val == 13) {
                        retval = 36000;
                } else if (wid_val == 8) {
                        retval = 48000;
                } else if (wid_val == 12) {
                        retval = 54000;
                } else {
                        nvt_dbg(ERROR, "%s: input rate is wrong!\n", __func__);
                        retval = -1;
                }
        }
        return retval;
}

/**
* nvt_parse_strdata_to_array -parse ascii string to byte buffer
* @str: input ascii string
* @wid: WID
* @ret_data: returned byte buffer
*
*
* Return: parsed data length or a negative value for fail
*/
static s32 nvt_parse_strdata_to_array(u8 *str, u16 wid, u8 *ret_data)
{
        s32 ret;
        s32 index;
        u8 const *delim = ",";
        u8 *token;
        u8 val;

        if (str == NULL || ret_data == NULL) {
                return -EPERM;
        }

        index = 0;
        while (1) {
                token = strsep((char **)&str, delim);
                if (token == NULL) {
                        if (wid < NVT_ICFG_SHORT_WID_START) {
                                if (index != 1) {
                                        return -EINVAL;
                                }

                        } else if (wid < NVT_ICFG_INT_WID_START) {
                                if (index != 2) {
                                        return -EINVAL;
                                }
                        } else if (wid < NVT_ICFG_STR_WID_START) {
                                if (index != 4) {
                                        return -EINVAL;
                                }
                        }
                        return index;
                }

                ret = kstrtou8(token, 16, &val);
                if (ret < 0) {
                        nvt_dbg(ERROR, "%s: input string error\n", __func__);
                        return -EINVAL;
                }
                *(ret_data + index) = val;
                index++;
        }

 //20150803 nash: original design (supprot much data handling for display)
#if 0
        s32 ret;
        u8 val_8;
        u16 val_16;
        u32 val_32;
        u8 *token;
        u8 const *delim = ",";
        u32 index;
        s8 *pch;

        if (str == NULL || ret_data == NULL) {
                return -EPERM;
        }


        if (wid < NVT_ICFG_SHORT_WID_START) {
                ret = kstrtou8(str, 16, &val_8);
                if (ret < 0) {
                        return -EINVAL;
                }
                *ret_data = val_8;
                return 1;

        } else if (wid < NVT_ICFG_INT_WID_START) {
                ret = kstrtou16(str, 16, &val_16);
                if (ret < 0) {
                        return -EINVAL;
                }
                val_16 = cpu_to_le16(val_16);
                memcpy(ret_data, &val_16, 2);
                return 2;

        } else if (wid < NVT_ICFG_STR_WID_START) {
                ret = kstrtou32(str, 16, &val_32);
                if (ret < 0) {
                        return -EINVAL;
                }
                val_32 = cpu_to_le32(val_32);
                memcpy(ret_data, &val_32, 4);
                return 4;

        } else if (wid < NVT_ICFG_BIN_WID_START) {
                pch = strchr(str, ',');
                /* Support input argument, "01,02,03", for example
                 * for string WID
                 */
                if (pch != NULL) {
                        if ((u8 *)pch - str == 0) {
                                nvt_dbg(INFO, "%s: string start with ,\n",
                                        __func__);
                                return -EINVAL;
                        }
                        index = 0;
                        while (1) {
                                token = strsep((char **)&str, delim);
                                if (token == NULL) {
                                        break;
                                }
                                ret = kstrtou8(token, 16, &val_8);
                                if (ret < 0) {
                                        return -EINVAL;
                                }

                                *(ret_data + index) = val_8;
                                index++;
                        }
                        return index;

                } else {
                        memcpy(ret_data, str, strlen(str));
                        return strlen(str);
                }

        } else if (wid < NVT_ICFG_MAX_WID_START) {
                index = 0;
                while (1) {
                        token = strsep((char **)&str, delim);
                        if (token == NULL) {
                                break;
                        }
                        ret = kstrtou8(token, 16, &val_8);
                        if (ret < 0) {
                                return -EINVAL;
                        }

                        *(ret_data + index) = val_8;
                        index++;
                }
                return index;

        } else {
                return -EPERM;

        }
#endif
}

void nvt_init_fw_by_wids(struct _nvt_bus *nvt_bus, enum nl80211_iftype type)
{
        s32 ret;
        u8 fc_enabled = 1;
        u8 monitor = 0;
        u8 start_op = DO_RESET;
        u8 streaming_mode = 1;

        if ((type == NL80211_IFTYPE_STATION) ||
                (type == NL80211_IFTYPE_P2P_CLIENT) ||
                (type == NL80211_IFTYPE_AP) ||
                (type == NL80211_IFTYPE_AP_VLAN) ||
                (type == NL80211_IFTYPE_P2P_GO)) {

                /* set monitor disabled */
                ret = nvt_send_wid_by_numeric(nvt_bus, WID_MONITOR_ENABLE,
                                (u8 *)&monitor, 1);
                if (ret < 0) {
                        nvt_dbg(ERROR, "%s: icfg reset fail\n", __func__);
                }

                ret = nvt_send_wid_by_numeric(nvt_bus, WID_START_OP,
                                (u8 *)&start_op, 1);
                if (ret < 0) {
                        nvt_dbg(ERROR, "%s: icfg reset fail\n", __func__);
                }

                /* set flow control enabled */
                ret = nvt_send_wid_by_numeric(nvt_bus, WID_FC_ENABLE,
                                &fc_enabled, 1);
                if (ret < 0) {
                        nvt_dbg(ERROR, "%s: icfg reset fail\n", __func__);
                }

                /* set streaming mode */
                if (nvt_data_mode == stream_mode) {
                      streaming_mode = 1;
                      if ((nvt_bus->nvt_adapter->nvt_fw_cap.extra_cap_1 &
                           FW_CAPABILITY_STREAMING_MODE)) {
                          nvt_dbg(ERROR, "%s: streaming mode\n", __func__);
                          ret = nvt_send_wid_by_numeric(nvt_bus, WID_DATA_MODE,
                                            &streaming_mode, 1);
                          if (ret < 0) {
                                 nvt_dbg(ERROR, "%s: icfg reset fail\n",
                                            __func__);
                          }
                      } else {
                          nvt_dbg(ERROR, "%s: Streaming Mode is Unsupported\n",
                                     __func__);
                      }
                } else {
                      streaming_mode = 0;
                      nvt_dbg(ERROR, "%s: data mode\n", __func__);
                      ret = nvt_send_wid_by_numeric(nvt_bus, WID_DATA_MODE,
                                        &streaming_mode, 1);
                      if (ret < 0) {
                              nvt_dbg(ERROR, "%s: icfg reset fail\n", __func__);
                      }
                }
        } else if (type == NL80211_IFTYPE_MONITOR) {
                monitor = 1;
                fc_enabled = 0;

                /* set monitor enabled */
                ret = nvt_send_wid_by_numeric(nvt_bus, WID_MONITOR_ENABLE,
                                (u8 *)&monitor, 1);
                if (ret < 0) {
                        nvt_dbg(ERROR, "%s: icfg reset fail\n", __func__);
                }

                ret = nvt_send_wid_by_numeric(nvt_bus, WID_START_OP,
                                (u8 *)&start_op, 1);
                if (ret < 0) {
                        nvt_dbg(ERROR, "%s: icfg reset fail\n", __func__);
                }

                /* set flow control disabled */
                ret = nvt_send_wid_by_numeric(nvt_bus, WID_FC_ENABLE,
                                &fc_enabled, 0);
                if (ret < 0) {
                        nvt_dbg(ERROR, "%s: icfg reset fail\n", __func__);
                }
        }

        return;
}

/*AT add for sending the numerical data*/

s32 nvt_send_wid_by_numeric(struct _nvt_bus *nvt_bus, u16 wid,
       u8 *wid_val_data, s32 num_of_byte)
{
        s32 ret;

        if (nvt_bus == NULL || wid_val_data == NULL) {
                return -EINVAL;
        }

        nvt_icfg_lock(nvt_bus);

        ret = nvt_icfg_reset(nvt_bus);
        if (ret < 0) {
                nvt_dbg(ERROR, "%s: icfg reset fail\n", __func__);
                goto fail;
        }

        ret = nvt_icfg_add(nvt_bus, NVT_ICFG_SET, wid, wid_val_data,
                        num_of_byte);
        if (ret < 0) {
                goto fail;
        }

        ret = nvt_icfg_send_and_get_resp(nvt_bus, NULL, 0);
        if (ret < 0) {
                nvt_dbg(ERROR, "%s: send WID fail\n", __func__);
                ret = -EPERM;
                goto fail;
        }


fail:
        nvt_icfg_unlock(nvt_bus);
        return ret;
}

/**
* nvt_send_wid_set -send SET WID message
* @nvt_bus: nvt bus structure
* @wid: WID
* @wid_val_str: WID value in ascii string
*
* Return: 0:success, a negative value for fail
*/
static s32 nvt_send_wid_set(struct _nvt_bus *nvt_bus, u16 wid,
       u8 *wid_val_str)
{
        s32 ret;
        u8 *wid_val_buff;

        wid_val_buff = kzalloc(NVT_IWPRIV_SETWID_CMD_SIZE, GFP_KERNEL);

        ret = nvt_parse_strdata_to_array(wid_val_str, wid, wid_val_buff);
        /*ret is the number of bytes for the data*/
        if (ret < 0) {
                goto fail;
        }

        ret = nvt_send_wid_by_numeric(nvt_bus, wid, wid_val_buff, ret);
        if (ret < 0) {
                nvt_dbg(ERROR, "%s: nvt_send_wid_set fail\n",
                        __func__);
        }
fail:
        kfree(wid_val_buff);
        return ret;
}

static s32 nvt_parse_setmib(struct _nvt_bus *nvt_bus,
       struct net_device *dev, s8 *mib_cmd, s8 *mib_value)
{
        s32 i;
        s32 ret;
        u8 *buff;
        u8 char_val;
        u16 short_val;
        //20151221 nash: coverity#48993
        u32 int_val = 0;
        u8 *str_tmp;
        u32 str_tmp_len = 32;

        struct _nvt_iwpriv_mib *mib_entry = NULL;

        for (i = 0; mib_cmd_tbl[i].name != NULL; i++) {
                if (strcmp(mib_cmd_tbl[i].name, mib_cmd) == 0) {
                        mib_entry = &mib_cmd_tbl[i];
                        break;
                }
        }

        if (mib_entry == NULL) {
                nvt_dbg(ERROR, "%s: no mib cmd matched!\n", __func__);
                return -EINVAL;
        }


        /* Run user specify handler */
        if (mib_entry->wid == 0xFFFF) {
                if (mib_entry->setmib_handler != NULL) {
                        return mib_entry->setmib_handler(nvt_bus, dev,
                                mib_value);
                } else {
                        return -EPERM;
                }
        }
        switch (mib_entry->req_format) {
        case REQ_CHAR_TYPE:
                ret = kstrtou8(mib_value, 16, &char_val);
                if (ret < 0) {
                        return -EINVAL;
                }
                str_tmp = kzalloc(str_tmp_len, GFP_KERNEL);
                //20151221 nash: ocverity#48957
                if (str_tmp == NULL) {
                        nvt_dbg(ERROR, "%s: kzalloc fail\n", __func__);
                        return -ENOMEM;
                }
                //20151221 nash: coverity#48977
                snprintf(str_tmp, str_tmp_len, "%x", char_val);
                ret = nvt_send_wid_set(nvt_bus, mib_entry->wid, str_tmp);
                kfree(str_tmp);
                return ret;

        case REQ_SHORT_TYPE:
                ret = kstrtou16(mib_value, 16, &short_val);
                if (ret < 0) {
                        return -EINVAL;
                }
                str_tmp = kzalloc(str_tmp_len, GFP_KERNEL);
                if (str_tmp == NULL) {
                        nvt_dbg(ERROR, "%s: kzalloc fail\n", __func__);
                        return -ENOMEM;
                }
                //20151221 nash: coverity#48977
                snprintf(str_tmp, str_tmp_len, "%x,%x",
                        short_val & 0x00FF, (short_val >> 8));
                ret = nvt_send_wid_set(nvt_bus, mib_entry->wid, str_tmp);
                kfree(str_tmp);
                return ret;

        case REQ_INT_TYPE:
                ret = kstrtou32(mib_value, 16, &int_val);
                if (ret < 0) {
                        return -EINVAL;
                }
                str_tmp = kzalloc(str_tmp_len, GFP_KERNEL);
                //20151221 nash: coverity#48977
                if (str_tmp == NULL) {
                        nvt_dbg(ERROR, "%s: kzalloc fail\n", __func__);
                        return -ENOMEM;
                }
                snprintf(str_tmp, str_tmp_len,
                        "%x,%x,%x,%x",
                        int_val & 0xFF,
                        (int_val >> 8) & 0xFF,
                        (int_val >> 16) & 0xFF,
                        (int_val >> 24) & 0xFF);
                ret = nvt_send_wid_set(nvt_bus, mib_entry->wid, str_tmp);
                kfree(str_tmp);
                return ret;

        case REQ_STR_TYPE:
                str_tmp = kzalloc(strlen(mib_value)*3, GFP_KERNEL);
                if (str_tmp == NULL) {
                        nvt_dbg(ERROR, "%s: kzalloc fail\n", __func__);
                        return -ENOMEM;
                }
                for (i = 0; i < strlen(mib_value); i++) {
                        if (i == strlen(mib_value) - 1) {
                                //20151221 nash: coverity#48977
                                snprintf((str_tmp + i*3),
                                        (strlen(mib_value) - i) * 3,
                                        "%02x",
                                        *(mib_value + i));
                        } else {
                                //20151221 nash: coverity#48977
                                snprintf((str_tmp + i*3),
                                        (strlen(mib_value) - i) * 3,
                                        "%02x,",
                                        *(mib_value + i));
                        }
                }
                ret = nvt_send_wid_set(nvt_bus, mib_entry->wid, str_tmp);
                kfree(str_tmp);
                return ret;

        case REQ_RAW_ASCII_TYPE:
                return nvt_send_wid_set(nvt_bus, mib_entry->wid, mib_value);

        case REQ_ENUM_STR_TYPE:
                /* get wid value */
                for (i = 0; i < mib_entry->enum_num; i++) {
                        if (strcmp(mib_entry->str_enum[i], mib_value) == 0) {
                                break;
                        }
                }
                if (i >= mib_entry->enum_num) {
                        return -EINVAL;
                }

                buff = kzalloc(str_tmp_len, GFP_KERNEL);
                if (buff == NULL) {
                        return -EPERM;
                }
                //20151221 nash: coverity#48977
                snprintf(buff, str_tmp_len, "%x", i);

                ret = nvt_send_wid_set(nvt_bus, mib_entry->wid, buff);
                kfree(buff);
                return ret;

        default:
                return -EPERM;
        }

}

static s32 nvt_iwpriv_setmib(struct net_device *dev,
       struct iw_request_info *info, struct iw_point *wrq, char *extra)
{
        s8 *mib_cmd;
        s8 *mib_value;
        u8 const *delim = "=";

        struct _nvt_if *nvt_if = netdev_priv(dev);
        struct _nvt_bus *nvt_bus = nvt_if->nvt_adapter->nvt_bus;

        mib_cmd = strsep(&extra, delim);
        if (mib_cmd == NULL) {
                return -EINVAL;
        }

        mib_value = strsep(&extra, delim);
        if (mib_value == NULL) {
                return -EINVAL;
        }

        return nvt_parse_setmib(nvt_bus, dev, mib_cmd, mib_value);

}

static s32 nvt_iwpriv_getmib(struct net_device *dev,
       struct iw_request_info *info, struct iw_point *wrq, char *extra)
{
        s32 i;
        s32 ret;
        u8 *cmd;
        u8 *resp_buff;
        u32 wid_len;
        u32 wid_val;

        struct _nvt_iwpriv_mib *mib_entry;
        struct _nvt_if *nvt_if = netdev_priv(dev);
        struct _nvt_bus *nvt_bus = nvt_if->nvt_adapter->nvt_bus;

        cmd = kzalloc(NVT_IWPRIV_GETMIB_CMD_SIZE, GFP_KERNEL);
        if (cmd == NULL) {
                return -EPERM;
        }

        copy_from_user(cmd, wrq->pointer, wrq->length);

        mib_entry = NULL;
        for (i = 0; mib_cmd_tbl[i].name != NULL; i++) {
                if (strcmp(mib_cmd_tbl[i].name, cmd) == 0) {
                        mib_entry = &mib_cmd_tbl[i];
                }
        }
        kfree(cmd);

        if (mib_entry == NULL) {
                return -EINVAL;
        }

        /* Run by handler if specified */
        if (mib_entry->wid == 0xFFFF) {
                if (mib_entry->getmib_handler != NULL) {
                        return mib_entry->getmib_handler(nvt_bus, dev, extra,
                                (u32 *)&wrq->length);
                } else {
                        return -EPERM;
                }
        }

        /* Send a WID message */
        //20151221 nash: coverity#48924
        ret = nvt_icfg_lock(nvt_bus);
        //20170109 nash: coverity#24344
        if (ret < 0) {
                nvt_dbg(ERROR, "%s: icfg lock fail\n", __func__);
                return -1;
        }

        ret = nvt_icfg_reset(nvt_bus);
        if (ret < 0) {
                nvt_dbg(ERROR, "%s: icfg reset fail\n", __func__);
                ret = -EPERM;
                goto fail_and_unlock;
        }

        ret = nvt_icfg_add(nvt_bus, NVT_ICFG_GET, mib_entry->wid, NULL, 0);
        if (ret < 0) {
                nvt_dbg(ERROR, "%s: icfg add fail\n", __func__);
                ret = -EPERM;
                goto fail_and_unlock;
        }

        resp_buff = kzalloc(NVT_IWPRIV_GETMIB_RSP_SIZE, GFP_KERNEL);
        if (resp_buff == NULL) {
                nvt_dbg(ERROR, "%s: resp buff alloc fail\n", __func__);
                ret = -EPERM;
                goto fail_and_unlock;
        }

        ret = nvt_icfg_send_and_get_resp(nvt_bus, resp_buff,
                NVT_IWPRIV_GETMIB_RSP_SIZE);
        if (ret < 0) {
                nvt_dbg(ERROR, "%s: get resp fail\n", __func__);
                ret = -EPERM;
                kfree(resp_buff);
                goto fail_and_unlock;
        }

        /* Parse result */
        wid_len = 0;
        for (i = 0; i < mib_entry->len_field_size; i++) {
                wid_len += ((resp_buff[NVT_ICFG_MSG_HDR_LEN +
                        NVT_ICFG_WID_SIZE + i]) << (8 * i));
        }

        ret = 0;

        switch (mib_entry->resp_format) {
        case RSP_FORMAT_HEX:
                wid_val = 0;
                for (i = 0; i < wid_len; i++) {
                        wid_val += ((resp_buff[NVT_ICFG_MSG_HDR_LEN +
                                NVT_ICFG_WID_SIZE +
                                mib_entry->len_field_size + i]) << (8 * i));
                }
                //20151221 nash: coverity#48975
                snprintf(extra, NVT_IWPRIV_GETMIB_RSP_SIZE, "%x", wid_val);
                wrq->length = strlen(extra);
                break;

        case RSP_FORMAT_ENUM_STRING:
                wid_val = 0;
                for (i = 0; i < wid_len; i++) {
                        wid_val <<= 8;
                        wid_val += ((resp_buff[NVT_ICFG_MSG_HDR_LEN +
                                NVT_ICFG_WID_SIZE +
                                mib_entry->len_field_size + i]) << (8 * i));
                }

                if (wid_val >= mib_entry->enum_num) {
                        ret = -EPERM;
                        break;
                }
                //20151221 nash: coverity#48975
                snprintf(extra, NVT_IWPRIV_GETMIB_RSP_SIZE, "%s(%x)",
                        mib_entry->str_enum[wid_val], wid_val);
                wrq->length = strlen(extra);
                break;

        case RSP_FORMAT_STRING:
        case RSP_FORMAT_BINARY:
                for (i = 0; i < wid_len; i++) {
                        //20151221 nash: coverity#48975
                        snprintf((extra + i * 3), NVT_IWPRIV_GETMIB_RSP_SIZE,
                                "%02x ", *(resp_buff +
                                NVT_ICFG_MSG_HDR_LEN + NVT_ICFG_WID_SIZE +
                                mib_entry->len_field_size + i));
                }
                wrq->length = strlen(extra);
                break;

        default:
                ret = -EPERM;
                break;
        }

        kfree(resp_buff);
fail_and_unlock:
       nvt_icfg_unlock(nvt_bus);
       return ret;
}

static s32 nvt_iwpriv_setwid(struct net_device *dev,
       struct iw_request_info *info, struct iw_point *wrq, char *extra)
{
        s32 ret;
        u8 const *delim = ",";
        u8 *token;
        u16 wid;

        struct _nvt_if *nvt_if = netdev_priv(dev);
        struct _nvt_bus *nvt_bus = nvt_if->nvt_adapter->nvt_bus;

        nvt_dbg(INFO, "%s: %s\n", __func__, extra);

        token = strsep(&extra, delim);
        ret = kstrtou16(token, 16, &wid);
        if (ret < 0) {
                return -EINVAL;
        }

        token = strsep(&extra, "");
        if (token == NULL) {
                nvt_dbg(ERROR, "%s: no wid value\n", __func__);
                return -EINVAL;
        }

        return nvt_send_wid_set(nvt_bus, wid, token);

}

static s32 nvt_iwpriv_getwid(struct net_device *dev,
       struct iw_request_info *info, struct iw_point *wrq, char *extra)
{
        s32 i;
        s32 ret;
        u32 wid = 0;
        s32 total_len;
        u8 *resp_buff;
        u8 *wid_value = NULL;
        u32 wid_len = 0;

        struct _nvt_if *nvt_if = netdev_priv(dev);
        struct _nvt_bus *nvt_bus = nvt_if->nvt_adapter->nvt_bus;

        copy_from_user((u8 *)&wid, wrq->pointer, wrq->length);

        ret = kstrtoul((char *)&wid, 16, (unsigned long *)&wid);
        if (ret < 0) {
                return -EINVAL;
        }

        if (wid >= NVT_ICFG_MAX_WID_START) {
                nvt_dbg(ERROR, "%s: unknown WID\n", __func__);
                return -EINVAL;
        }

        /* Send and get response */
        //20151221 nash: coverity#48925
        ret = nvt_icfg_lock(nvt_bus);
        //20170109 nash: coverity#24345
        if (ret < 0) {
                nvt_dbg(ERROR, "%s: icfg lock fail\n", __func__);
                return -1;
        }

        ret = nvt_icfg_reset(nvt_bus);
        if (ret < 0) {
                nvt_dbg(ERROR, "%s: reset wid msg buffer fail\n", __func__);
                ret = -EPERM;
                goto fail_and_unlock;
        }

        ret = nvt_icfg_add(nvt_bus, NVT_ICFG_GET, wid, NULL, 0);
        if (ret < 0) {
                nvt_dbg(ERROR, "%s: add wid fail\n", __func__);
                ret = -EPERM;
                goto fail_and_unlock;
        }

        resp_buff = kzalloc(NVT_IWPRIV_GETWID_CMD_SIZE*2, GFP_KERNEL);
        if (resp_buff == NULL) {
                ret = -EPERM;
                goto fail_and_unlock;
        }

        total_len = nvt_icfg_send_and_get_resp(nvt_bus, resp_buff,
                NVT_IWPRIV_GETWID_CMD_SIZE*2);
        if (total_len < 0) {
                nvt_dbg(ERROR, "%s: send wid fail\n", __func__);
                ret = -EPERM;
                goto fail_and_free;
        }

        nvt_icfg_unlock(nvt_bus);

         if (wid < NVT_ICFG_BIN_WID_START) {
                wid_len = resp_buff[NVT_ICFG_MSG_HDR_LEN + NVT_ICFG_WID_SIZE];
                wid_value = (resp_buff + NVT_ICFG_MSG_HDR_LEN +
                        NVT_ICFG_WID_SIZE + 1);

        } else if (wid < NVT_ICFG_MAX_WID_START) {
                wid_len = resp_buff[NVT_ICFG_MSG_HDR_LEN + NVT_ICFG_WID_SIZE];
                wid_len += resp_buff[NVT_ICFG_MSG_HDR_LEN +
                        NVT_ICFG_WID_SIZE + 1] << 8;
                wid_value = (resp_buff + NVT_ICFG_MSG_HDR_LEN +
                        NVT_ICFG_WID_SIZE + 2);
        }

        for (i = 0; i < wid_len; i++) {
                //20151221 nash: coverity#48976
                snprintf((extra + i * 3), NVT_IWPRIV_GETMIB_RSP_SIZE,
                        "%02x ", *(wid_value + i));
        }
        wrq->length = strlen(extra);

        kfree(resp_buff);

        return ret;

//20150803 nash: original design (supprot much data handling for display)
#if 0
        /* Dispaly result */
        /* char, short, and int WID */
        if (wid < NVT_ICFG_STR_WID_START) {
                wid_len = resp_buff[NVT_ICFG_MSG_HDR_LEN + NVT_ICFG_WID_SIZE];
                wid_value = (resp_buff + NVT_ICFG_MSG_HDR_LEN +
                        NVT_ICFG_WID_SIZE + 1);
                for (i = 0; i < wid_len; i++) {
                        sprintf((extra + i * 2), "%02x",
                                *(wid_value + wid_len - 1 - i));
                }
                wrq->length = wid_len * 2;

        /* Special case for get MAC address */
        } else if (wid == WID_MAC_ADDR) {
                wid_len = resp_buff[NVT_ICFG_MSG_HDR_LEN + NVT_ICFG_WID_SIZE];
                wid_value = (resp_buff + NVT_ICFG_MSG_HDR_LEN +
                        NVT_ICFG_WID_SIZE + 1);
                for (i = 0; i < wid_len; i++) {
                        sprintf((extra + i * 3), "%02x:", *(wid_value + i));
                }
                wrq->length = wid_len * 3;

        /* String WID */
        } else if (wid < NVT_ICFG_BIN_WID_START) {
                wid_len = resp_buff[NVT_ICFG_MSG_HDR_LEN + NVT_ICFG_WID_SIZE];
                wid_value = (resp_buff + NVT_ICFG_MSG_HDR_LEN +
                        NVT_ICFG_WID_SIZE + 1);
                memcpy(extra, wid_value, wid_len);
                wrq->length = wid_len;

        /* Binary WID */
        } else if (wid < NVT_ICFG_MAX_WID_START) {
                wid_len = resp_buff[NVT_ICFG_MSG_HDR_LEN + NVT_ICFG_WID_SIZE];
                wid_len += resp_buff[NVT_ICFG_MSG_HDR_LEN +
                        NVT_ICFG_WID_SIZE + 1] << 8;
                wid_value = (resp_buff + NVT_ICFG_MSG_HDR_LEN +
                        NVT_ICFG_WID_SIZE + 2);

                for (i = 0; i < wid_len; i++) {
                        sprintf((extra + i * 3), "%02x,", *(wid_value + i));
                }
                wrq->length = wid_len * 3;
        }

        ret = 0;
#endif

fail_and_free:
       kfree(resp_buff);

fail_and_unlock:
       nvt_icfg_unlock(nvt_bus);
       return ret;
}

static s32 nvt_iwpriv_set_fw_download(struct net_device *dev,
       struct iw_request_info *info, struct iw_param *rrq, char *extra)
{
        struct _nvt_if *nvt_if = netdev_priv(dev);

        nvt_if->fw_download_type = rrq->value;
        nvt_dbg(INFO, "%s: mode=%d\n", __func__, nvt_if->fw_download_type);

        schedule_work(&nvt_if->setfwdownload_work);

        return 0;
}

#if 1  /* tx statistic */
static s32 nvt_iwpriv_shwmm(struct net_device *dev,
       struct iw_request_info *info, struct iw_point *wrq, char *extra)
{
        struct _nvt_if *nvt_if = netdev_priv(dev);

        struct _nvt_adapter *nvt_adapter = nvt_if->nvt_adapter;
        //struct _nvt_priv *nvt_priv = &nvt_adapter->nvt_priv;

        struct _tx_info *tx_info = nvt_adapter->nvt_priv.tx_info;

        struct _nvt_bus *nvt_bus = nvt_adapter->nvt_bus;

        EDCA_STATS_T stats[4];

        s8 fifo;

        printk("%s:\n", __func__);
        printk("fc:%d      BK          BE          VI          VO\n",
                tx_info->fcmode);
        printk("=========================================================\n");

        printk("    enq:   ");
        for (fifo = NV_TX_FIFO_AC_BK; fifo >= 0; fifo--) {
          stats[fifo].en = tx_info->stats[fifo].en;
          printk("%-9d   ", tx_info->stats[fifo].en);
        }
        printk("\n");

        printk("    deq:   ");
        for (fifo = NV_TX_FIFO_AC_BK; fifo >= 0; fifo--) {
          stats[fifo].out = tx_info->stats[fifo].out;
          printk("%-9d   ", tx_info->stats[fifo].out);
        }
        printk("\n");

        printk(" remain:   ");
        for (fifo = NV_TX_FIFO_AC_BK; fifo >= 0; fifo--) {
          printk("%-9d   ", tx_info->fifo_enqpkt[fifo]);
        }
        printk("\n");

        printk(" deq_sz:   ");
        for (fifo = NV_TX_FIFO_AC_BK; fifo >= 0; fifo--) {
          stats[fifo].out_sz = tx_info->stats[fifo].out_sz;
          printk("%-9x   ", tx_info->stats[fifo].out_sz);
        }
        printk("\n");

        printk("   drop:   ");
        for (fifo = NV_TX_FIFO_AC_BK; fifo >= 0; fifo--) {
          stats[fifo].drop = tx_info->stats[fifo].drop;
          printk("%-9d   ", tx_info->stats[fifo].drop);
        }
        printk("\n");

        printk("\n");
        printk(" Bus       ok          fail\n");
        printk("=========================================================\n");
        printk(" pkt tx:   %-9llu   %-9llu\n",
                 nvt_bus->statistic.pkt_tx_cnt,
                 nvt_bus->statistic.pkt_tx_err_cnt);
        printk(" pkt rx:   %-9llu   %-9llu\n",
                 nvt_bus->statistic.pkt_rx_cnt,
                 nvt_bus->statistic.pkt_rx_err_cnt);
        printk("ctrl tx:   %-9llu   %-9llu\n",
                 nvt_bus->statistic.ctl_tx_cnt,
                 nvt_bus->statistic.ctl_tx_err_cnt);
        printk("ctrl rx:   %-9llu   %-9llu\n",
                 nvt_bus->statistic.ctl_rx_cnt,
                 nvt_bus->statistic.ctl_rx_err_cnt);

        return 0;
}
#endif


//#ifdef CONFIG_WIFI_TUNING_PHASE_II
#if 0
static s32 nvt_iwpriv_wifi_tuning(struct net_device *dev,
       struct iw_request_info *info, struct iw_param *rrq, char *extra)
{

        s32 ret = 0;
        s8 *mib_cmd;
        s8 *mib_value;
        u8 const *delim = "=";
        unsigned int interval, start;
        u32  buff_size = 256;
        //int skfd = 0;

        //struct _nvt_if *nvt_if = netdev_priv(dev);
        //struct _nvt_bus *nvt_bus = nvt_if->nvt_adapter->nvt_bus;

        struct iwreq wrq;
        struct _wifi_tuning_cmd wifi_tuning_cmd;

        mib_cmd = strsep(&extra, delim);
        if (mib_cmd == NULL) {
                return -EINVAL;
        }
        nvt_dbg(ERROR, "%s: mib_cmd:%s\n", __func__, mib_cmd);

        mib_value = strsep(&extra, delim);
        if (mib_value == NULL) {
                return -EINVAL;
        }
        nvt_dbg(ERROR, "%s: mib_value:%s\n", __func__, mib_value);


        wifi_tuning_cmd.buf = kzalloc(buff_size, GFP_KERNEL);

        if (wifi_tuning_cmd.buf == NULL) {
            nvt_dbg(ERROR, "ERROR: malloc fail\n");
            ret = -1;
            goto buf_create_failed;
        }

        if (strcmp("start", mib_cmd) == 0) {
            sscanf((const char *)mib_value, "%d", &start);
            nvt_dbg(ERROR, "start:%d\n", start);
            wifi_tuning_cmd.type = WIFI_TUNING_SET;
            wifi_tuning_cmd.cmd_code.set = WIFI_TUNING_SET_START;
            wifi_tuning_cmd.buf_len = 1;
            *((u8 *)wifi_tuning_cmd.buf) = (u8)start;

        } else if (strcmp("interval", mib_cmd) == 0) {
            sscanf((const char *)mib_value, "%d", &interval);
            nvt_dbg(ERROR, "interval:%d\n", interval);
            wifi_tuning_cmd.type = WIFI_TUNING_SET;
            wifi_tuning_cmd.cmd_code.set = WIFI_TUNING_SET_INTERVAL;
            wifi_tuning_cmd.buf_len = 4;
            *((unsigned int *)wifi_tuning_cmd.buf) = interval;

        } else {
            ret = -1;
            goto cmd_code_failed;
        }

cmd_code_failed:
        kfree(wifi_tuning_cmd.buf);
buf_create_failed:
        return ret;

}
#endif

static const iw_handler nvt_iw_handler[] = {
        (iw_handler) NULL,      /* SIOCSIWCOMMIT */
        (iw_handler) NULL,      /* SIOCGIWNAME */
        (iw_handler) NULL,                      /* SIOCSIWN`WID */
        (iw_handler) NULL,                      /* SIOCGIWNWID */
        (iw_handler) NULL,              /* SIOCSIWFREQ */
        (iw_handler) NULL,              /* SIOCGIWFREQ */
        (iw_handler) NULL,              /* SIOCSIWMODE */
        (iw_handler) NULL,              /* SIOCGIWMODE */
        (iw_handler) NULL,                      /* SIOCSIWSENS */
        (iw_handler) NULL,                      /* SIOCGIWSENS */
        (iw_handler) NULL,                      /* SIOCSIWRANGE */
        (iw_handler) NULL,           /* SIOCGIWRANGE */
        (iw_handler) NULL,                      /* SIOCSIWPRIV */
        (iw_handler) NULL,                      /* SIOCGIWPRIV */
        (iw_handler) NULL,                      /* SIOCSIWSTATS */
        (iw_handler) NULL,                      /* SIOCGIWSTATS */
        (iw_handler) NULL,                      /* SIOCSIWSPY */
        (iw_handler) NULL,                      /* SIOCGIWSPY */
        (iw_handler) NULL,                      /* -- hole -- */
        (iw_handler) NULL,                      /* -- hole -- */
        (iw_handler) NULL,              /* SIOCSIWAP */
        (iw_handler) NULL,              /* SIOCGIWAP */
        (iw_handler) NULL,                      /* -- hole -- */
        (iw_handler) NULL,                      /* SIOCGIWAPLIST */
        (iw_handler) NULL,           /* SIOCSIWSCAN */
        (iw_handler) NULL,           /* SIOCGIWSCAN */
        (iw_handler) NULL,          /* SIOCSIWESSID */
        (iw_handler) NULL,          /* SIOCGIWESSID */
        (iw_handler) NULL,                      /* SIOCSIWNICKN */
        (iw_handler) NULL,           /* SIOCGIWNICKN */
        (iw_handler) NULL,                      /* -- hole -- */
        (iw_handler) NULL,                      /* -- hole -- */
        (iw_handler) NULL,           /* SIOCSIWRATE */
        (iw_handler) NULL,           /* SIOCGIWRATE */
        (iw_handler) NULL,            /* SIOCSIWRTS */
        (iw_handler) NULL,            /* SIOCGIWRTS */
        (iw_handler) NULL,           /* SIOCSIWFRAG */
        (iw_handler) NULL,           /* SIOCGIWFRAG */
        (iw_handler) NULL,                      /* SIOCSIWTXPOW */
        (iw_handler) NULL,                      /* SIOCGIWTXPOW */
        (iw_handler) NULL,          /* SIOCSIWRETRY */
        (iw_handler) NULL,          /* SIOCGIWRETRY */
        (iw_handler) NULL,         /* SIOCSIWENCODE */
        (iw_handler) NULL,         /* SIOCGIWENCODE */
        (iw_handler) NULL,          /* SIOCSIWPOWER */
        (iw_handler) NULL,          /* SIOCGIWPOWER */
};

static const iw_handler nvt_priv_handler[] = {
       (iw_handler) nvt_iwpriv_setmib,
       (iw_handler) nvt_iwpriv_getmib,
       (iw_handler) nvt_iwpriv_setwid,
       (iw_handler) nvt_iwpriv_getwid,
       (iw_handler) nvt_iwpriv_set_fw_download,
       #if 1  /* tx statistic */
       (iw_handler) nvt_iwpriv_shwmm,
       #endif
};

static const struct iw_priv_args nvt_priv_args[] = {
       {SIOCIWFIRSTPRIV,
       IW_PRIV_TYPE_CHAR | 1024,
       0,
       "setmib"},

       {SIOCIWFIRSTPRIV + 1,
       IW_PRIV_TYPE_CHAR | NVT_IWPRIV_GETMIB_CMD_SIZE,
       IW_PRIV_TYPE_CHAR | NVT_IWPRIV_GETMIB_RSP_SIZE,
       "getmib" },

       {SIOCIWFIRSTPRIV + 2,
       IW_PRIV_TYPE_CHAR | NVT_IWPRIV_SETWID_CMD_SIZE,
       0,
       "setwid" },

       {SIOCIWFIRSTPRIV + 3,
       IW_PRIV_TYPE_CHAR | NVT_IWPRIV_GETWID_CMD_SIZE,
       IW_PRIV_TYPE_CHAR | NVT_IWPRIV_GETWID_CMD_SIZE,
       "getwid" },

       {SIOCIWFIRSTPRIV + 4,
       IW_PRIV_TYPE_BYTE | IW_PRIV_SIZE_FIXED | 1,
       IW_PRIV_TYPE_NONE,
       "set_fw_download" },

       #if 1   /* tx statistic */
       {SIOCIWFIRSTPRIV + 5,
       IW_PRIV_TYPE_NONE,
       IW_PRIV_TYPE_NONE,
       "shwmm" },
       #endif
};

static const struct iw_handler_def nvt_iw_handlers = {
       .num_standard           = ARRAY_SIZE(nvt_iw_handler),
       .num_private            = ARRAY_SIZE(nvt_priv_handler),
       .num_private_args       = ARRAY_SIZE(nvt_priv_args),
       .standard               = (iw_handler *)nvt_iw_handler,
       .private                = (iw_handler *)nvt_priv_handler,
       .private_args           = (struct iw_priv_args *)nvt_priv_args,
};

/**
* nvt_iw_register_handler - iw_handler registration for net_device
* @handler: iw_handler_def structure
*
*
* Return: 0:success, -1:fail
*/
s32 nvt_iw_register_handler(struct net_device *ndev)
{
       ndev->wireless_handlers = &nvt_iw_handlers;
       return 0;
}

