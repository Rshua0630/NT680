#ifndef _NVT_IW_H_
#define _NVT_IW_H_

#include <net/iw_handler.h>
#include <linux/in6.h>
//include for getting hw addr from ip addr
extern struct neigh_table arp_tbl;
extern s32 nvt_iw_register_handler(struct net_device *ndev);
extern s32 nvt_parse_rate_0(u8 wid_val);
extern int nvt_ipv6_get_lladdr(struct net_device *dev,
                struct in6_addr *addr, unsigned char banned_flags);

extern int __nvt_ipv6_get_lladdr(struct inet6_dev *idev, struct in6_addr *addr,
                unsigned char banned_flags);
extern struct inet6_dev *__nvt_in6_dev_get(const struct net_device *dev);
extern s32 nvt_get_first_ipv6(struct net_device *data, u8 *ret_addr);
extern s32 nvt_get_first_ipv4(struct net_device *data, u8 *ret_addr);
extern s32 nvt_send_wid_by_numeric(struct _nvt_bus *nvt_bus, u16 wid,
                u8 *wid_val_data, s32 num_of_byte);
extern void nvt_init_fw_by_wids(struct _nvt_bus *nvt_bus,
                enum nl80211_iftype type);
#endif /* _NVT_IW_ */
