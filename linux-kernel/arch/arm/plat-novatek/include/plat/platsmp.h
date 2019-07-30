/*
 *  linux/arch/arm/plat-novatek/include/plat/platsmp.h
 *
 *  Copyright (C) Novatek Ltd.
 *  All Rights Reserved
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

extern void nvt_secondary_startup(void);
extern void nvt_secondary_init(unsigned int cpu);
extern int  nvt_boot_secondary(unsigned int cpu, struct task_struct *idle);

extern void nvt_secondary_init_ca53(unsigned int cpu);
extern int  nvt_boot_secondary_ca53(unsigned int cpu, struct task_struct *idle);

extern void write_release_addr(u32 release_phys);
extern phys_addr_t cpu_release_addr[NR_CPUS];
