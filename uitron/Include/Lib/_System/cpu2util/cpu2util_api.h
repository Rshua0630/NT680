/**
    uboot, APIs declare.

    @file       uboot_api.h
    @ingroup    mVDODISP
    @note       --

    Copyright   Novatek Microelectronics Corp. 2012.  All rights reserved.
*/

#ifndef _UBOOTAPI_H
#define _UBOOTAPI_H

/**
     Structure for uboot to update all-in-one-bin
*/
typedef struct _CPU2UTIL_UPDFW {
	UINT32 uboot_addr;
	UINT32 uboot_size;
	UINT32 fw_addr;
	UINT32 fw_size;
	CHAR   *ota_name;
} CPU2UTIL_UPDFW, *PCPU2UTIL_UPDFW;

typedef struct _CPU2UTIL_FORMAT_ROOTFS {
	UINT32 uboot_addr;
	UINT32 uboot_size;
	UINT32 timeout_min;
} CPU2UTIL_FORMAT_ROOTFS, *PCPU2UTIL_FORMAT_ROOTFS;

extern INT32 cpu2util_updfw(CPU2UTIL_UPDFW *p_info);
extern INT32 cpu2util_wait_updfw_done(UINT32 timeout_min);
extern INT32 cpu2util_format_rootfs(CPU2UTIL_FORMAT_ROOTFS *p_info);
extern INT32 cpu2util_terminate(void);
extern INT32 cpu2util_suspend(void);
extern INT32 cpu2util_resume(void);
#endif

