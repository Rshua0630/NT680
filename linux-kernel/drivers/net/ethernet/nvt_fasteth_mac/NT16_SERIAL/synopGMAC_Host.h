#ifndef SYNOP_GMAC_HOST_H
#define SYNOP_GMAC_HOST_H

#include <linux/pci.h>
#include <linux/netdevice.h>
#include <linux/etherdevice.h>

#include "synopGMAC_plat.h"
#include "synopGMAC_Dev.h"

struct synop_adapter_struct {
	/*Device Dependent Data structur*/
	struct synop_device *synopGMACdev;
	/*Os/Platform Dependent Data Structures*/
	struct net_device *synopGMACnetdev;
	struct net_device_stats synop_stats;
};

int __init SynopGMAC_Host_Interface_init(void);
void __exit SynopGMAC_Host_Interface_exit(void);
int proc_read_synopGMAC_info(char *page,
			     char **start, off_t off, int count,
	int *eof, void *data);
int proc_write_synopGMAC_info(struct file *file,
			      const char *buffer, unsigned long count,
	void *data);

#endif
