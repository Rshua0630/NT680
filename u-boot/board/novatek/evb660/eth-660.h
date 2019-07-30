/** \file
 * Header file for the nework dependent functionality.
 * The function prototype listed here are linux dependent.
 *   
 * \internal
 * ---------------------------REVISION HISTORY-------------------
 * Synopsys 			01/Aug/2007		Created
 */
 
#ifndef NVT_TH_H
#define NVT_TH_H 1

#include <common.h>
#include <command.h>
#include <net.h>
#include <miiphy.h>
#include <malloc.h>
#include "nvt_plat.h"

#define NET_IF_TIMEOUT (10*HZ)
#define CHECK_TIME (HZ)
//#define CHECK_TIME (10*HZ)
#define IPC_OFFLOAD

s32 nt96660_eth_init(struct eth_device *eth_dev, bd_t *bis) ;
void nt96660_eth_halt(struct eth_device *eth_dev);
s32 nt96660_eth_send(struct eth_device *eth_dev, volatile void *packet, int length);
s32 nt96660_eth_recv(struct eth_device *eth_dev) ;
int nt96660_eth_initialize(bd_t *bis) ;

#endif /* End of file */
