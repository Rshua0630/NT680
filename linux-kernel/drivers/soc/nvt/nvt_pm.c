/*
 * Novatek Cortex A9 Power Management Functions
 *
 */

#include <linux/suspend.h>
#include <asm/system_misc.h>
#include <asm/suspend.h>
#include <linux/io.h>
#include "mach/pm.h"

static int __nt72668_suspend_enter(unsigned long unused)
{
	soft_restart(virt_to_phys(nt72668_wait_for_die));
	return 0;
}

static int nt72668_pm_enter(suspend_state_t suspend_state)
{
	int ret = 0;

	pr_info("[%s]\n", __func__);
#ifdef CONFIG_ARCH_NVT_V8
	__cpu_suspend(0, __nt72668_suspend_enter);
#else
	cpu_suspend(0, __nt72668_suspend_enter);
#endif
	pr_info("resume successful and back to [%s]\n", __func__);

	return ret;
}

static int nt72668_pm_begin(suspend_state_t state)
{
	pr_info("[%s]\n", __func__);
	return 0;
}

static void nt72668_pm_end(void)
{
	pr_info("[%s]\n", __func__);
}

static void nt72668_pm_finish(void)
{
	pr_info("[%s]\n", __func__);
}

static const struct platform_suspend_ops nt72668_pm_ops = {
	.begin		= nt72668_pm_begin,
	.end		= nt72668_pm_end,
	.enter		= nt72668_pm_enter,
	.finish		= nt72668_pm_finish,
	.valid		= suspend_valid_only_mem,
};

static int __init nt72668_pm_late_init(void)
{
	suspend_set_ops(&nt72668_pm_ops);
	return 0;
}

device_initcall(nt72668_pm_late_init);
