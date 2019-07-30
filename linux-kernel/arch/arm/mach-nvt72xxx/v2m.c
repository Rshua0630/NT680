/*
 * Novatek CA9 Motherboard Support
 */
#include <asm/mach/arch.h>
#include <asm/mach/map.h>
#include "core.h"
#ifdef CONFIG_ARCH_NVT72172
#include <asm/io.h>
#include <mach/motherboard.h>
#endif
#include <linux/pm.h>


static const char * const v2m_dt_match[] __initconst = {
	"nvt,ca9",
	"nvt,ca53",
	"novatek,ca9",
	NULL,
};

static void __init v2m_dt_map_io(void)
{
#if defined(CONFIG_ARCH_NVT72172)
	debug_ll_io_init();
	pm_power_off = nvt_machine_power_off;
#endif
}

DT_MACHINE_START(NVT72668_DT, "Novatek-Cortex A9")
	.dt_compat	= v2m_dt_match,
	.smp		= smp_ops(nvt_ca9_smp_ops),
	.l2c_aux_val	= 0x00400000,
	.l2c_aux_mask	= 0xfe0fffff,
MACHINE_END

DT_MACHINE_START(NVT72172_DT, "Novatek-Cortex CA53")
	.dt_compat	= v2m_dt_match,
	.smp		= smp_ops(nvt_ca53_smp_ops),
	.map_io		= v2m_dt_map_io,
#if defined(CONFIG_ARCH_NVT72172)
	.restart    = nvt_machine_restart,
#endif
MACHINE_END
