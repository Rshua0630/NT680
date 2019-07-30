/*
 *  linux/arch/arm/mach-nvt72668/platsmp.c
 *
 *  Copyright (C) Novatek Ltd.
 *  All Rights Reserved
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */
#include <linux/init.h>
#include <linux/smp.h>
#include <linux/of_address.h>
#include <asm/smp_scu.h>
#include "plat/platsmp.h"
#include "core.h"
#include <linux/io.h>
#include <asm/cacheflush.h>
#include <linux/io.h>
#include <asm/cacheflush.h>

phys_addr_t cpu_release_addr[NR_CPUS];

/*
 * Initialise the CPU possible map early - this describes the CPUs
 * which may be present or become present in the system.
 */
static const struct of_device_id nvt_smp_dt_scu_match[] __initconst = {
	{ .compatible = "arm,cortex-a9-scu", },
	{}
};

void write_release_addr(u32 release_phys)
{
	u32 *virt = ioremap_cache(release_phys, sizeof(u32 *));

	writel_relaxed(virt_to_phys(nvt_secondary_startup), virt);
	/* Make sure this store is visible to other CPUs */
	smp_wmb();
	__cpuc_flush_dcache_area(virt, sizeof(u32));
	iounmap(virt);
}

static void __init nvt_smp_prepare_cpus(unsigned int max_cpus)
{
	struct device_node *scu = of_find_matching_node(NULL,
			nvt_smp_dt_scu_match);

	if (scu)
		scu_enable(of_iomap(scu, 0));
}

static void __init nvt_smp_prepare_cpus_ca53(unsigned int max_cpus)
{
	int cpu_count = 0;
	int cpu;

	/*
	 * Initialise the present map, which describes the set of CPUs actually
	 * populated at the present time.
	 */
	for_each_possible_cpu(cpu) {
			struct device_node *np;
			u32 release_phys;

			np = of_get_cpu_node(cpu, NULL);
			if (!np)
				continue;
			if (of_property_read_u32(np, "cpu-release-addr"
							, &release_phys))
				continue;

			if (cpu_count < max_cpus) {
					set_cpu_present(cpu, true);
					cpu_count++;
			}

			if (release_phys != 0) {
					cpu_release_addr[cpu] = release_phys;
					write_release_addr(release_phys);
			}
	}
	sev();
}

struct smp_operations __initdata nvt_ca9_smp_ops = {
	.smp_prepare_cpus	= nvt_smp_prepare_cpus,
	.smp_secondary_init	= nvt_secondary_init,
	.smp_boot_secondary	= nvt_boot_secondary,
#ifdef CONFIG_HOTPLUG_CPU
	.cpu_die		= nvt_ca9_cpu_die,
#endif
};

struct smp_operations __initdata nvt_ca53_smp_ops = {
	.smp_prepare_cpus	= nvt_smp_prepare_cpus_ca53,
	.smp_secondary_init	= nvt_secondary_init,
	.smp_boot_secondary	= nvt_boot_secondary_ca53,
#ifdef CONFIG_HOTPLUG_CPU
	.cpu_die		= nvt_ca9_cpu_die,
#endif
};

