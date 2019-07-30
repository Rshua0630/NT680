/** \file
 * Header file for the nework dependent functionality.
 * The function prototype listed here are linux dependent.
 *
 * \internal
 * ---------------------------REVISION HISTORY-------------------
 * Synopsys	01/Aug/2007		Created
 */

#ifndef SYNOP_GMAC_NETWORK_INTERFACE_H
#define SYNOP_GMAC_NETWORK_INTERFACE_H 1

#define NET_IF_TIMEOUT (10 * HZ)
#define IPC_OFFLOAD

extern u32 NVT_Phy_Init_Done;
extern struct timer_list NVT_Phy_InitTimer;
/*These are the global pointers for their respecive structures*/
extern struct synop_adapter_struct *synopGMACadapter;
extern struct synop_device	          *synopGMACdev;
extern struct semaphore	sem_phy_rw;

s32   synopGMAC_init_network_interface(void);
void  synopGMAC_exit_network_interface(void);

s32 synopGMAC_linux_open(struct net_device *);
s32 synopGMAC_linux_close(struct net_device *);
s32 synopGMAC_linux_xmit_frames(struct sk_buff *, struct net_device *);
struct net_device_stats *synopGMAC_linux_get_stats(struct net_device *);
void synopGMAC_linux_set_multicast_list(struct net_device *);
s32 synopGMAC_linux_set_mac_address(struct net_device *, void *);
s32 synopGMAC_linux_change_mtu(struct net_device *, s32);
s32 synopGMAC_linux_do_ioctl(struct net_device *, struct ifreq *, s32);
void synopGMAC_linux_tx_timeout(struct net_device *);
void synopGMAC_linux_change_rx_flags(struct net_device *, int flags);

/*System Platform dependent functions*/
/*int SysPlat_AHB_Get(void);*/
void SysPlat_Init_Mac_Addr(void);

#endif /* End of file */
