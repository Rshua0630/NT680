/*
 *  board/novatek/evb668/nt72668_utils.h
 *
 *  Author:	Alvin lin
 *  Created:	June 21, 2013
 *  Copyright:	Novatek Inc.
 *
 */
#ifndef __NT72668_UTILS_H__
#define __NT72668_UTILS_H__

struct bootloader_message {
    char command[32];
    char status[32];
    char recovery[1024];
};

struct boot_address {
	unsigned char *kernel_addr;
	unsigned char *ramdisk_addr;
};

#endif

