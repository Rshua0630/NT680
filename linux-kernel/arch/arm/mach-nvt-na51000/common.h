/**
    NVT common header
 
    @file       common.h
    @ingroup
    @note
    Copyright   Novatek Microelectronics Corp. 2016.  All rights reserved.

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License version 2 as
    published by the Free Software Foundation.
*/

#ifndef __ARCH_ARM_MACH_NT96680_COMMON_H
#define __ARCH_ARM_MACH_NT96680_COMMON_H
#include <linux/irq.h>

void __init nvt_na51000_map_io(void);
void nvt_na51000_restart(enum reboot_mode mode, const char *cmd);
#ifdef CONFIG_SMP
void nvt_secondary_startup(void);
extern struct smp_operations nvt_smp_ops;
#endif
#ifdef CONFIG_HOTPLUG_CPU
extern void nvt_cpu_die(unsigned int cpu);
#endif
#endif /* __ARCH_ARM_MACH_NT96680_COMMON_H */
