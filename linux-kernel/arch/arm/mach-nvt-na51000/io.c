/**
    NVT IO mapping
    To handle static io mapping
    @file       io.c
    @ingroup
    @note
    Copyright   Novatek Microelectronics Corp. 2016.  All rights reserved.

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License version 2 as
    published by the Free Software Foundation.
*/

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/io.h>
#include <asm/mach/map.h>
#include <mach/hardware.h>

static struct map_desc nvt_na51000_io_desc[] __initdata = {
	{
		.virtual	= (unsigned long) NVT_PERIPHERAL_VIRT_BASE,
		.pfn		= __phys_to_pfn(NVT_PERIPHERAL_PHYS_BASE),
		.length		= NVT_PERIPHERAL_SIZE,
		.type		= MT_DEVICE,
	},
	{
		.virtual	= (unsigned long) NVT_MODELEXT_VIRT_BASE,
		.pfn		= __phys_to_pfn(NVT_MODELEXT_PHYS_BASE),
		.length		= NVT_MODELEXT_SIZE,
		.type		= MT_UNCACHED,
	},
};

/* physical address to virtual address space mapping */
void __init nvt_na51000_map_io(void)
{
	debug_ll_io_init();
	iotable_init(nvt_na51000_io_desc, ARRAY_SIZE(nvt_na51000_io_desc));
}


u32 boot_pad_driving[12]={0};
static int __init get_sdio_pad_driving(char *str)
{
	int i;
	char output_num[10];
        if (!str)
                return 0;

	for (i=0; i<12; i++) {
		strncpy(output_num, str, 2);
		boot_pad_driving[i] = simple_strtol(output_num, NULL, 0);
		str += 3;
	}

	return 0;
}
early_param("sdio_driving", get_sdio_pad_driving);
EXPORT_SYMBOL(boot_pad_driving);