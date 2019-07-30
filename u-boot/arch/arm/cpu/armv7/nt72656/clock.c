/*
 * Copyright (C) 2014 Novatek Microelectronics Corp. All rights reserved.
 * Author: SP-KSW <SP_KSW_MailGrp@novatek.com.tw>
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#include <common.h>
#include <asm/armv7.h>
#include <asm/io.h>
#include <asm/arch/clock.h>

inline void apll_set_data(u8 offset, u8 value)
{
	writel(value, (APLL_BASE_ADDR + (offset * 4)));
}

inline u32 apll_get_data(u8 offset)
{
	return readl((APLL_BASE_ADDR + (offset * 4)));
}

inline void apll_enable(pll_page_t page)
{
	if (PLL_PAGE_0 == page) {
		writel(APLL_PAGE_0_EN, (APLL_PAGE_EN_ADDR));
	}
	else if (PLL_PAGE_B == page) {
		writel(APLL_PAGE_B_EN, (APLL_PAGE_EN_ADDR));
	}
	else {
		/* ignore */
	}
}

inline void mpll_set_data(u8 offset, u8 value)
{
	writel(value, (MPLL_BASE_ADDR + (offset * 4)));
}

inline u32 mpll_get_data(u8 offset)
{
	return readl((MPLL_BASE_ADDR + (offset * 4)));
}

inline void mpll_enable(pll_page_t page)
{
	if (PLL_PAGE_0 == page) {
		writel(MPLL_PAGE_0_EN, (MPLL_PAGE_EN_ADDR));
	}
	else if (PLL_PAGE_B == page) {
		writel(MPLL_PAGE_B_EN, (MPLL_PAGE_EN_ADDR));
	}
	else {
		/* ignore */
	}
}

void set_sys_mpll(unsigned long off, unsigned long val)
{
	val <<= 17; val  /= 12;

	mpll_enable(PLL_PAGE_B);

	mpll_set_data((off + 0), ((val >> 0) & 0xff));
	mpll_set_data((off + 1), ((val >> 8) & 0xff));
	mpll_set_data((off + 2), ((val >> 16) & 0xff));
}

unsigned long get_sys_mpll(unsigned long off)
{
	unsigned long val;

	mpll_enable(PLL_PAGE_B);

	val  = mpll_get_data(off);
	val |= (mpll_get_data((off + 1)) << 8);
	val |= (mpll_get_data((off + 2)) << 16);

	val *= 12; val >>= 17;

	return val;
}

void set_cpu_clk(unsigned long freq)
{
	/* no implement */
}

unsigned long get_cpu_clk(void)
{
	unsigned long aclk = 0;	/* ARM frequency (KHz) */

	aclk = get_sys_mpll(CPU_PLL_OFFSET);

	apll_enable(PLL_PAGE_0);
	if (apll_get_data(0x00) & 0x01) {
		aclk <<= 3;
	}

	/* the unit of value is KHz */
	return ((aclk) ? (aclk * 1000) : (CONFIG_CPU_FREQ * 1000));
}
