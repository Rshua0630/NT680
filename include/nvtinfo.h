/**
    @file nvtinfo.h
    @ingroup
    @note Nothing (or anything need to be mentioned).
    Copyright Novatek Microelectronics Corp. 2017. All rights reserved.

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License version 2 as
    published by the Free Software Foundation.
*/

#ifndef _NVTINFO_H
#define _NVTINFO_H
#include <stdbool.h>
#ifdef __cplusplus
extern "C" {
#endif

extern int nvtbootts_set(char *s, bool boot_only);
extern int nvt_timer_tm0_get(void);
extern int nvtmem_hotplug_align_size(void);
extern int nvtmem_hotplug_set(unsigned long mem_size, unsigned long mem_addr);

#ifdef __cplusplus
}
#endif
/* ----------------------------------------------------------------- */
#endif /* nvtinfo.h  */
