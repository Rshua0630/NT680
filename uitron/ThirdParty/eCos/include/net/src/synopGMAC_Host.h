#ifndef SYNOP_GMAC_HOST_H
#define SYNOP_GMAC_HOST_H


#include <linux/pci.h>
#include <linux/netdevice.h>
#include <linux/etherdevice.h>

#include "synopGMAC_plat.h"
#include "synopGMAC_Dev.h"

typedef struct synopGMACAdapterStruct{

/*Device Dependent Data structur*/
synopGMACdevice * synopGMACdev;
/*Os/Platform Dependent Data Structures*/
struct net_device *synopGMACnetdev;
struct net_device_stats synopGMACNetStats;

} synopGMACPciNetworkAdapter;


#endif
