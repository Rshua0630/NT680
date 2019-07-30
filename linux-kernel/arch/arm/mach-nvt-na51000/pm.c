/**
    NVT Power management module
    To handle power turn off, irq wakeup, CPU context store/restore and CPU off
    @file pm.c
    @ingroup
    @note
    Copyright Novatek Microelectronics Corp. 2017. All rights reserved.

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License version 2 as
    published by the Free Software Foundation.
*/

#include <linux/kernel.h>
#include <linux/suspend.h>
#include <linux/io.h>

#include <linux/io.h>
#include <linux/pm.h>
#include <asm/cacheflush.h>
#include <asm/suspend.h>
#include <mach/pm.h>

extern unsigned short nvt_pm_notifier_flag;

static unsigned long nvt_resume_addr = 0;

static void nvt_pm_suspend(void)
{
	cpu_suspend(0, na51000_cpu_suspend);
	pr_info("NVT: PM: NVT is trying to enter deep sleep...\n");
}

static int nvt_pm_begin(suspend_state_t state)
{
	void __iomem *virt = ioremap_cache(nvt_resume_addr, sizeof(u32 *));
	unsigned long linux_resume_address = (unsigned long)virt_to_phys(na51000_cpu_resume);

	/*
	 * Inform the user where to resume from.
	 * Ideally the resume address would have been saved in a scratch
	 * register somewhere from sleep asm code, to allow the bootloader to
	 * automatically resume from this address.
	 */
	pr_info("NVT: PM: Please resume from physical address : 0x%08lx \n", linux_resume_address);

	writel(linux_resume_address, virt);
	smp_wmb();
	__cpuc_flush_dcache_area(virt, sizeof(u32));
	iounmap(virt);

	pr_info("NVT: PM: %s finished\n", __func__);
	return 0;
}

static int nvt_pm_enter(suspend_state_t state)
{
	switch (state) {
	case PM_SUSPEND_MEM:
		/*
		 * Perform the sleep.
		 * After wake, this will return.
		 */
		nvt_pm_suspend();
		pr_info("NVT: PM: %s finished\n", __func__);
		break;

	default:
		return -EINVAL;
	}
	return 0;
}

static void nvt_pm_wake(void)
{
	pr_info("NVT: PM: %s finished\n", __func__);
}

static void nvt_pm_end(void)
{
	pr_info("NVT: PM: %s finished\n", __func__);
}

static const struct platform_suspend_ops nvt_suspend_ops = {
	.valid	= suspend_valid_only_mem,
	.begin	= nvt_pm_begin,
	.enter	= nvt_pm_enter,
	.wake	= nvt_pm_wake,
	.end	= nvt_pm_end,
};

static int __init nvt_pm_init(void)
{
	/* To configure the supported suspend ops */
	suspend_set_ops(&nvt_suspend_ops);

	pr_info("NVT: PM: %s register successfully\n", __func__);
	return 0;
}

late_initcall(nvt_pm_init);

int __init parse_resume_addr(char *arg)
{
        if (!arg)
                return 0;

        return kstrtoul(arg, 0, &nvt_resume_addr);
}

early_param("resume_addr", parse_resume_addr);
