/**
    NVT board init function
    This file will setup initial function
    @file       board-dt.c
    @ingroup
    @note
    Copyright   Novatek Microelectronics Corp. 2016.  All rights reserved.

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License version 2 as
    published by the Free Software Foundation.
*/

#include <linux/console.h>

#include <linux/kernel.h>
#include <linux/types.h>
#include <linux/interrupt.h>
#include <linux/list.h>
#include <linux/timer.h>
#include <linux/init.h>
#include <linux/serial_core.h>
#include <linux/platform_device.h>
#include <linux/io.h>
#include <linux/clocksource.h>
#include <linux/initrd.h>
#include <linux/of_platform.h>
#include <asm/mach-types.h>
#include <asm/mach/arch.h>
#include <asm/mach/map.h>
#include <mach/hardware.h>

#include "common.h"

/* To do nvt board initialization */
static void __init nvt_na51000_machine_init(void)
{
	/* Add  device tree peripheral devices resource */
	of_platform_populate(NULL, of_default_bus_match_table, NULL, NULL);
}

static const char * const nvt_dt_match[] __initconst = {
	"nvt,ca53",
	NULL,
};

DT_MACHINE_START(NVT_NA51000_DT, "Novatek NA51000 platform")
	.dt_compat	= nvt_dt_match,
	.map_io		= nvt_na51000_map_io,
	.init_machine	= nvt_na51000_machine_init,
	.restart	= nvt_na51000_restart,
#ifdef CONFIG_SMP
	.smp		= smp_ops(nvt_smp_ops),
#endif
MACHINE_END
