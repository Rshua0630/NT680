/**
    NVT clock management module for NA51000 SoC
    @file na51000-clk.c

    Copyright Novatek Microelectronics Corp. 2017. All rights reserved.

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License version 2 as
    published by the Free Software Foundation.
*/

#include <linux/spinlock.h>

#include "mach/ioaddress.h"
#include "mach/cc.h"
#include "mach/cg-reg.h"
#include "nvt-im-clk.h"

#define IOADDR_CG_REG_SIZE		0x12A0

#define DEFAULT_OSC_IN_CLK_FREQ	12000000
#define DEFAULT_PLL_DIV_VALUE	131072
#define DEFAULT_PLL2_RATIO		3244032	/* 0x318000 (297MHz for PLL2) */
#define DEFAULT_PLL3_RATIO		5133653	/* 0x4E5555 (470MHz for DMA) */
#define DEFAULT_PLL4_RATIO		2271914	/* 0x22AAAA (208MHz for SSPLL) */
#define DEFAULT_PLL5_RATIO		4369066	/* 0x42AAAA (400MHz for Sensor) */
#define DEFAULT_PLL6_RATIO		4150613	/* 0x3F5555 (380MHz for IPP) */
#define DEFAULT_PLL7_RATIO		5024426	/* 0x4CAAAA (460MHz for Audio) */
#define DEFAULT_PLL8_RATIO		4587520	/* 0x460000 (420MHz for CPU) */
#define DEFAULT_PLL9_RATIO		5242880	/* 0x500000 (480MHz        ) */
#define DEFAULT_PLL10_RATIO		3276800	/* 0x320000 (300MHz for MCLK) */
#define DEFAULT_PLL11_RATIO		5242880	/* 0x500000 (480MHz for DSI) */
#define DEFAULT_PLL12_RATIO		5461333	/* 0x535555 (500MHz for ETH) */
#define DEFAULT_PLL13_RATIO		4587520	/* 0x460000 (420MHz for H264) */
#define DEFAULT_PLL14_RATIO		2184533	/* 0x215555 (200MHz for DMA2) */

static void __iomem *remap_base = (void __iomem *)IOADDR_CG_REG_BASE;
static spinlock_t cg_lock;

#define NA51000_CG_SETREG(ofs, value)	writel((value), (remap_base+(ofs)))
#define NA51000_CG_GETREG(ofs)		readl(remap_base+(ofs))

#define loc_cpu() {cc_hwlock_resource(CC_CORE_CA53_CORE2, CC_RESOURCE_CKG); }
#define unl_cpu() {cc_hwunlock_resource(CC_CORE_CA53_CORE2, CC_RESOURCE_CKG); }

static struct nvt_fixed_rate_clk na51000_fixed_rate_clk[] __initdata = {
	FIXED_RATE_CONF("osc_in", 12000000),
	FIXED_RATE_CONF("fix480m", 480000000),
	FIXED_RATE_CONF("fix240m", 240000000),
	FIXED_RATE_CONF("fix192m", 192000000),
	FIXED_RATE_CONF("fix160m", 160000000),
	FIXED_RATE_CONF("fix120m", 120000000),
	FIXED_RATE_CONF("fix96m", 96000000),
	FIXED_RATE_CONF("fix80m", 80000000),
	FIXED_RATE_CONF("fix60m", 60000000),
	FIXED_RATE_CONF("fix48m", 48000000),
	FIXED_RATE_CONF("fix12.288m", 12288000),
	FIXED_RATE_CONF("fix11.2896m", 11289600),
	FIXED_RATE_CONF("fix8.192m", 8192000),
	FIXED_RATE_CONF("fix3m", 3000000),
	/* fix11718.75 is for WatchDog Timer */
	FIXED_RATE_CONF("fix11718.75", 12000000 / 1024),
	FIXED_RATE_CONF("fix16M", 16000000),
};

static struct nvt_pll_clk na51000_pll[] __initdata = {
	PLL_CONF("pll2", DEFAULT_PLL2_RATIO, CG_PLL2_CLK_RATE0_REG_OFFSET,
		 CG_PLL_EN_REG_OFFSET, BIT2),
	PLL_CONF("pll4", DEFAULT_PLL4_RATIO, CG_PLL4_CLK_RATE0_REG_OFFSET,
		 CG_PLL_EN_REG_OFFSET, BIT4),
	PLL_CONF("pll12", DEFAULT_PLL12_RATIO, CG_PLL12_CLK_RATE0_REG_OFFSET,
		 CG_PLL_EN_REG_OFFSET, BIT12),
};

static struct nvt_composite_gate_clk na51000_cgate_clk[] __initdata = {
#ifdef CONFIG_USE_OF
	/* COMP_GATE_CONF("f0230000.0", "fix480m", 96000000,
	   CG_SPI_CLK_DIV_REG0_OFFSET, BIT0, WID13,
	   CG_CLK_EN_REG1_OFFSET, BIT6,
	   CG_SYS_RESET_REG1_OFFSET, BIT6, NOT_RESET), */
	COMP_GATE_CONF("f0320000.1", "fix480m", 48000000,
		       CG_SPI_CLK_DIV_REG0_OFFSET, BIT16, WID13,
		       CG_CLK_EN_REG1_OFFSET, BIT7, DO_ENABLE,
		       CG_SYS_RESET_REG1_OFFSET, BIT7, NOT_RESET,
		       0, 0, 0),
	COMP_GATE_CONF("f0220000.i2c", "fix48m", 48000000, 0, 0, 0,
		       CG_CLK_EN_REG1_OFFSET, BIT4, NOT_ENABLE,
		       CG_SYS_RESET_REG1_OFFSET, BIT4, NOT_RESET,
		       0, 0, 0),
	COMP_GATE_CONF("f0050000.wdt", "fix11718.75", 11718, 0, 0, 0,
		       CG_CLK_EN_REG1_OFFSET, BIT17, NOT_ENABLE,
		       CG_SYS_RESET_REG1_OFFSET, BIT17, NOT_RESET,
		       0, 0, 0),
	COMP_GATE_CONF("f0300000.uart", "fix48m", 48000000, 0, 0, 0,
		       CG_CLK_EN_REG1_OFFSET, BIT11, DO_ENABLE,
		       CG_SYS_RESET_REG1_OFFSET, BIT11, NOT_RESET,
		       0, 0, 0),
	COMP_GATE_CONF("f0310000.uart", "fix48m", 48000000, 0, 0, 0,
		       CG_CLK_EN_REG1_OFFSET, BIT22, DO_ENABLE,
		       CG_SYS_RESET_REG1_OFFSET, BIT22, NOT_RESET,
		       0, 0, 0),
	COMP_GATE_CONF("f0380000.uart", "fix48m", 48000000, 0, 0, 0,
		       CG_CLK_EN_REG1_OFFSET, BIT23, DO_ENABLE,
		       CG_SYS_RESET_REG1_OFFSET, BIT23, NOT_RESET,
		       0, 0, 0),
	COMP_GATE_CONF("f02b0000.eth", "pll12div6", 50000000, 0, 0, 0,
		       CG_CLK_EN_REG1_OFFSET, BIT29, NOT_ENABLE,
		       CG_SYS_RESET_REG1_OFFSET, BIT29, NOT_RESET,
		       0, 0, 0),
	COMP_GATE_CONF("f0600000.nvtim", "APB_CLK", 80000000, 0, 0, 0,
		       CG_CLK_EN_REG2_OFFSET, BIT17, DO_ENABLE,
		       CG_SYS_RESET_REG2_OFFSET, BIT17, NOT_RESET,
		       0, 0, 0),
	COMP_GATE_CONF("f0260000.adc", "fix16M", 16000000, 0, 0, 0,
		       CG_CLK_EN_REG1_OFFSET, BIT13, NOT_ENABLE,
		       CG_SYS_RESET_REG1_OFFSET, BIT13, NOT_RESET,
		       0, 0, 0),
	COMP_GATE_CONF("f0320000.spi", "fix192m", 24000000,
		       CG_SPI_CLK_DIV_REG0_OFFSET, BIT16, WID11,
		       CG_CLK_EN_REG1_OFFSET, BIT7, DO_ENABLE,
		       CG_SYS_RESET_REG1_OFFSET, BIT7, NOT_RESET,
		       0, 0, 0),
#ifdef CONFIG_MTD_SPINAND
	COMP_GATE_CONF("f0400000.nand", "fix480m", 80000000,
		       CG_SDIO_CLK_DIV_REG_OFFSET, BIT24, WID6,
		       CG_CLK_EN_REG1_OFFSET, BIT0, NOT_ENABLE,
		       CG_SYS_RESET_REG1_OFFSET, BIT0, NOT_RESET,
		       CG_CLK_AUTO_GATING_REG0_OFFSET, BIT19, NOT_AUTOGATING),
#else
	COMP_GATE_CONF("f0400000.nor", "fix480m", 48000000,
		       CG_SDIO_CLK_DIV_REG_OFFSET, BIT24, WID6,
		       CG_CLK_EN_REG1_OFFSET, BIT0, NOT_ENABLE,
		       CG_SYS_RESET_REG1_OFFSET, BIT0, NOT_RESET,
		       CG_CLK_AUTO_GATING_REG0_OFFSET, BIT19, NOT_AUTOGATING),
#endif
#else
	COMP_GATE_CONF("spi_na51000.1", "fix480m", 96000000,
		       CG_SPI_CLK_DIV_REG0_OFFSET, BIT0, WID13,
		       CG_CLK_EN_REG1_OFFSET, BIT6,
		       CG_SYS_RESET_REG1_OFFSET, BIT6, NOT_RESET,
		       0, 0, 0),
	COMP_GATE_CONF("spi_nand.0", "fix480m", 96000000,
		       CG_SPI_CLK_DIV_REG0_OFFSET, BIT16, WID13,
		       CG_CLK_EN_REG1_OFFSET, BIT7,
		       CG_SYS_RESET_REG1_OFFSET, BIT7, NOT_RESET,
		       0, 0, 0),
	COMP_GATE_CONF("nvt_im_i2c.0", "fix48m", 48000000, 0, 0, 0,
		       CG_CLK_EN_REG1_OFFSET, BIT4,
		       CG_SYS_RESET_REG1_OFFSET, BIT4, NOT_RESET,
		       0, 0, 0),
	COMP_GATE_CONF("nvt_wdt.0", "fix11718.75", 11718, 0, 0, 0,
		       CG_CLK_EN_REG1_OFFSET, BIT17,
		       CG_SYS_RESET_REG1_OFFSET, BIT17, NOT_RESET,
		       0, 0, 0),
	COMP_GATE_CONF("nvt_uart.2", "fix48m", 48000000, 0, 0, 0,
		       CG_CLK_EN_REG1_OFFSET, BIT22,
		       CG_SYS_RESET_REG1_OFFSET, BIT22, NOT_RESET,
		       0, 0, 0),
	COMP_GATE_CONF("nvt_uart.3", "fix48m", 48000000, 0, 0, 0,
		       CG_CLK_EN_REG1_OFFSET, BIT23,
		       CG_SYS_RESET_REG1_OFFSET, BIT23, NOT_RESET,
		       0, 0, 0),
	COMP_GATE_CONF("nvt_im_eth.0", "pll12div6", 50000000, 0, 0, 0,
		       CG_CLK_EN_REG1_OFFSET, BIT29,
		       CG_SYS_RESET_REG1_OFFSET, BIT29, NOT_RESET,
		       0, 0, 0),
	COMP_GATE_CONF("ehci-nvtim", "APB_CLK", 80000000, 0, 0, 0,
		       CG_CLK_EN_REG2_OFFSET, BIT17,
		       CG_SYS_RESET_REG2_OFFSET, BIT17, NOT_RESET,
		       0, 0, 0),
#endif
	COMP_GATE_CONF("pwm_clk.12", "fix120m", 30000000,
		       CG_PWM_CLK_RATE_REG1_OFFSET, BIT16, WID14,
		       CG_CLK_EN_REG3_OFFSET, BIT12, NOT_ENABLE,
		       CG_SYS_RESET_REG2_OFFSET, BIT8, NOT_RESET,
		       0, 0, 0),
	COMP_GATE_CONF("pwm_clk.13", "fix120m", 30000000,
		       CG_PWM_CLK_RATE_REG2_OFFSET, BIT0, WID14,
		       CG_CLK_EN_REG3_OFFSET, BIT13, NOT_ENABLE,
		       CG_SYS_RESET_REG2_OFFSET, BIT8, NOT_RESET,
		       0, 0, 0),
	COMP_GATE_CONF("pwm_clk.14", "fix120m", 30000000,
		       CG_PWM_CLK_RATE_REG2_OFFSET, BIT16, WID14,
		       CG_CLK_EN_REG3_OFFSET, BIT14, NOT_ENABLE,
		       CG_SYS_RESET_REG2_OFFSET, BIT8, NOT_RESET,
		       0, 0, 0),
	COMP_GATE_CONF("pwm_clk.15", "fix120m", 30000000,
		       CG_PWM_CLK_RATE_REG3_OFFSET, BIT0, WID14,
		       CG_CLK_EN_REG3_OFFSET, BIT15, NOT_ENABLE,
		       CG_SYS_RESET_REG2_OFFSET, BIT8, NOT_RESET,
		       0, 0, 0),
	COMP_GATE_CONF("pwm_clk.16", "fix120m", 30000000,
		       CG_PWM_CLK_RATE_REG3_OFFSET, BIT16, WID14,
		       CG_CLK_EN_REG3_OFFSET, BIT16, NOT_ENABLE,
		       CG_SYS_RESET_REG2_OFFSET, BIT8, NOT_RESET,
		       0, 0, 0),
	COMP_GATE_CONF("pwm_clk.17", "fix120m", 30000000,
		       CG_PWM_CLK_RATE_REG4_OFFSET, BIT0, WID14,
		       CG_CLK_EN_REG3_OFFSET, BIT17, NOT_ENABLE,
		       CG_SYS_RESET_REG2_OFFSET, BIT8, NOT_RESET,
		       0, 0, 0),
	COMP_GATE_CONF("pwm_clk.18", "fix120m", 30000000,
		       CG_PWM_CLK_RATE_REG4_OFFSET, BIT16, WID14,
		       CG_CLK_EN_REG3_OFFSET, BIT18, NOT_ENABLE,
		       CG_SYS_RESET_REG2_OFFSET, BIT8, NOT_RESET,
		       0, 0, 0),
	COMP_GATE_CONF("pwm_clk.19", "fix120m", 30000000,
		       CG_PWM_CLK_RATE_REG5_OFFSET, BIT0, WID14,
		       CG_CLK_EN_REG3_OFFSET, BIT19, NOT_ENABLE,
		       CG_SYS_RESET_REG2_OFFSET, BIT8, NOT_RESET,
		       0, 0, 0),
};

static struct nvt_composite_group_pwm_clk na51000_cgpwm_clk[] __initdata = {
	COMP_GPWM_CONF("fix120m", 30000000,
		       CG_PWM_CLK_RATE_REG0_OFFSET, BIT0, WID14,
		       CG_CLK_EN_REG3_OFFSET, BIT0, BIT3, NOT_ENABLE,
		       CG_SYS_RESET_REG2_OFFSET, BIT8),
	COMP_GPWM_CONF("fix120m", 30000000,
		       CG_PWM_CLK_RATE_REG0_OFFSET, BIT16, WID14,
		       CG_CLK_EN_REG3_OFFSET, BIT4, BIT7, NOT_ENABLE,
		       CG_SYS_RESET_REG2_OFFSET, BIT8),
	COMP_GPWM_CONF("fix120m", 30000000,
		       CG_PWM_CLK_RATE_REG1_OFFSET, BIT0, WID14,
		       CG_CLK_EN_REG3_OFFSET, BIT8, BIT11, NOT_ENABLE,
		       CG_SYS_RESET_REG2_OFFSET, BIT8),
};

static const char *sdio_clk_parent[] __initconst = {
	"fix192m", "fix480m", "pll2", "pll4"
};

static const char *apb_clk_parent[] __initconst = {
	"fix80m", "fix120m", "fix160m", "fix240m"
};

static const char *tsmux_clk_parent[] __initconst = {
	"reserved", "fix240m", "fix480m", "reserved"
};

static const char *crypto_clk_parent[] __initconst = {
	"fix240m", "fix480m"
};

static struct nvt_composite_mux_clk na51000_cmux_clk[] __initdata = {
#ifdef CONFIG_USE_OF
	COMP_MUX_CONF("f0420000.mmc", sdio_clk_parent, 800000, 0,
		      CG_PERI_CLK_RATE_REG0_OFFSET, BIT4, WID2,
		      CG_SDIO_CLK_DIV_REG_OFFSET, BIT0, WID11,
		      CG_CLK_EN_REG1_OFFSET, BIT2, NOT_ENABLE,
		      CG_SYS_RESET_REG1_OFFSET, BIT2, DO_RESET,
		      CG_CLK_AUTO_GATING_REG0_OFFSET, BIT20, DO_AUTOGATING),
	COMP_MUX_CONF("f0500000.mmc", sdio_clk_parent, 800000, 0,
		      CG_PERI_CLK_RATE_REG0_OFFSET, BIT8, WID2,
		      CG_SDIO_CLK_DIV_REG_OFFSET, BIT12, WID11,
		      CG_CLK_EN_REG1_OFFSET, BIT3, NOT_ENABLE,
		      CG_SYS_RESET_REG1_OFFSET, BIT3, DO_RESET,
		      CG_CLK_AUTO_GATING_REG0_OFFSET, BIT21, NOT_AUTOGATING),
	COMP_MUX_CONF("f0510000.mmc", sdio_clk_parent, 800000, 0,
		      CG_PERI_CLK_RATE_REG1_OFFSET, BIT0, WID2,
		      CG_PERI_CLK_DIV_REG1_OFFSET, BIT0, WID11,
		      CG_CLK_EN_REG1_OFFSET, BIT14, NOT_ENABLE,
		      CG_SYS_RESET_REG1_OFFSET, BIT14, DO_RESET,
		      CG_CLK_AUTO_GATING_REG1_OFFSET, BIT15, NOT_AUTOGATING),
	COMP_MUX_CONF("f0620000.crypto", crypto_clk_parent, 480000000, 1,
		      CG_CODEC_CLK_RATE_REG_OFFSET, BIT31, WID1, 0, 0, 0,
		      CG_CLK_EN_REG0_OFFSET, BIT23, NOT_ENABLE,
		      CG_SYS_RESET_REG0_OFFSET, BIT23, NOT_RESET,
		      0, 0, 0),
	COMP_MUX_CONF("f0650000.tsmux", tsmux_clk_parent, 240000000, 1,
		      CG_PERI_CLK_RATE_REG1_OFFSET, BIT4, WID2, 0, 0, 0,
		      CG_CLK_EN_REG2_OFFSET, BIT13, DO_ENABLE,
		      CG_SYS_RESET_REG2_OFFSET, BIT13, NOT_RESET,
		      0, 0, 0),
#else
	COMP_MUX_CONF("nvt_mmc.0", sdio_clk_parent, 800000, 0,
		      CG_PERI_CLK_RATE_REG0_OFFSET, BIT4, WID2,
		      CG_SDIO_CLK_DIV_REG_OFFSET, BIT0, WID11,
		      CG_CLK_EN_REG1_OFFSET, BIT2,
		      CG_SYS_RESET_REG1_OFFSET, BIT2, DO_RESET,
		      CG_CLK_AUTO_GATING_REG0_OFFSET, BIT20, NOT_AUTOGATING),
	COMP_MUX_CONF("nvt_mmc.1", sdio_clk_parent, 800000, 0,
		      CG_PERI_CLK_RATE_REG0_OFFSET, BIT4, WID2,
		      CG_SDIO_CLK_DIV_REG_OFFSET, BIT12, WID11,
		      CG_CLK_EN_REG1_OFFSET, BIT3,
		      CG_SYS_RESET_REG1_OFFSET, BIT3, DO_RESET,
		      CG_CLK_AUTO_GATING_REG0_OFFSET, BIT21, NOT_AUTOGATING),
	COMP_MUX_CONF("nvt_mmc.2", sdio_clk_parent, 800000, 0,
		      CG_PERI_CLK_RATE_REG1_OFFSET, BIT0, WID2,
		      CG_PERI_CLK_DIV_REG1_OFFSET, BIT0, WID11,
		      CG_CLK_EN_REG1_OFFSET, BIT14,
		      CG_SYS_RESET_REG1_OFFSET, BIT14, DO_RESET,
		      CG_CLK_AUTO_GATING_REG1_OFFSET, BIT15, NOT_AUTOGATING),
	COMP_MUX_CONF("nvt_crypto.0", crypto_clk_parent, 480000000, 1,
		      CG_CODEC_CLK_RATE_REG_OFFSET, BIT31, WID1, 0, 0, 0,
		      CG_CLK_EN_REG0_OFFSET, BIT23,
		      CG_SYS_RESET_REG0_OFFSET, BIT23, NOT_RESET,
		      0, 0, 0),
	COMP_MUX_CONF("nvt_tsmux.0", tsmux_clk_parent, 240000000, 1,
		      CG_PERI_CLK_RATE_REG1_OFFSET, BIT4, WID2, 0, 0, 0,
		      CG_CLK_EN_REG2_OFFSET, BIT13,
		      CG_SYS_RESET_REG2_OFFSET, BIT13, NOT_RESET,
		      0, 0, 0),
#endif
};

static int na51000_misc_clk_register(void)
{
	struct clk *clk;
	struct clk_hw *clk_hw;
	unsigned int reg_val;
	int ret = 0;

	clk =
	    clk_register_mux(NULL, "APB_CLK", apb_clk_parent,
			     ARRAY_SIZE(apb_clk_parent),
			     CLK_SET_RATE_PARENT | CLK_SET_RATE_NO_REPARENT |
			     CLK_GET_RATE_NOCACHE,
			     remap_base + CG_SYS_CLK_RATE_REG_OFFSET, 4, 2, 0,
			     &cg_lock);
	if (IS_ERR(clk)) {
		pr_err("%s: failed to register clock hardware \"APB_CLK\"\n",
		       __func__);
		ret = -EPERM;
	}
	if (clk_register_clkdev(clk, "APB_CLK", NULL)) {
		pr_err("%s: failed to register clock device \"APB_CLK\"\n",
		       __func__);
		ret = -EPERM;
	} else {
		clk_hw = __clk_get_hw(clk);
		clk_mux_ops.set_parent(clk_hw, 2);
		ret = clk_prepare_enable(clk);
		if (ret < 0)
			pr_err("APB_CLK prepare & enable failed!\n");
	}

	clk = clk_register_fixed_factor(NULL, "pll12div6", "pll12", 0, 1, 6);
	if (IS_ERR(clk)) {
		pr_err("%s: failed to register clock hardware \"pll12div6\"\n",
		       __func__);
		ret = -EPERM;
	} else {
		ret = clk_prepare_enable(clk);
		if (ret < 0)
			pr_err("pll12div6 prepare & enable failed!\n");
	}

	return ret;
}

void na51000_cg_lock(void)
{
	loc_cpu();
}

void na51000_cg_unlock(void)
{
	unl_cpu();
}

int na51000_clock_init(void)
{
	int ret;

	pr_info("na51000_clock_init\n");

	spin_lock_init(&cg_lock);

	remap_base = ioremap_nocache(IOADDR_CG_REG_BASE, IOADDR_CG_REG_SIZE);
	if (IS_ERR((void *)remap_base)) {
		ret = -EPERM;
		pr_err("Failed to remap CKG registers!\n");
		goto exit;
	} else
		nvt_cg_base_remap(remap_base);

	nvt_cg_lock_register(&na51000_cg_lock);
	nvt_cg_unlock_register(&na51000_cg_unlock);

	ret =
	    nvt_fixed_rate_clk_register(na51000_fixed_rate_clk,
					ARRAY_SIZE(na51000_fixed_rate_clk));
	if (ret < 0)
		goto exit;

	ret =
	    nvt_pll_clk_register(na51000_pll, ARRAY_SIZE(na51000_pll),
				 DEFAULT_PLL_DIV_VALUE, &cg_lock);
	if (ret < 0)
		goto exit;

	ret = na51000_misc_clk_register();
	if (ret < 0)
		goto exit;

	ret =
	    nvt_composite_gate_clk_register(na51000_cgate_clk,
					    ARRAY_SIZE
					    (na51000_cgate_clk), &cg_lock);
	if (ret < 0)
		goto exit;

	ret =
	    nvt_composite_group_pwm_clk_register(na51000_cgpwm_clk,
						 ARRAY_SIZE
						 (na51000_cgpwm_clk), &cg_lock);
	if (ret < 0)
		goto exit;

	ret =
	    nvt_composite_mux_clk_register(na51000_cmux_clk,
					   ARRAY_SIZE
					   (na51000_cmux_clk), &cg_lock);
	if (ret < 0)
		goto exit;

exit:
	return ret;
}

arch_initcall(na51000_clock_init);
