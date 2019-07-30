/*
 * Copyright (C) 2014 Novatek Microelectronics Corp. All rights reserved.
 * Author: SP-KSW <SP_KSW_MailGrp@novatek.com.tw>
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#include <common.h>
#include <asm/armv7.h>
#include <asm/io.h>
#include <asm/arch/soc.h>
#include <asm/arch/clock.h>

#include <div64.h>

DECLARE_GLOBAL_DATA_PTR;

#define PTIMER_CTRL_TIMER_DISABLE		0
#define PTIMER_CTRL_TIMER_ENABLE		1

#define PTIMER_CTRL_AUTO_RELOAD			0x2

#define PTIMER_CTRL_OVERFLOW_VAL		0xffffffff
#define PTIMER_CTRL_LOAD_VAL			0xffffffff

#define PTIMER_CTRL_PRESCALER_SHIFT		8
#define PTIMER_CTRL_PRESCALE			0

static struct ptimer *timer_base = (struct ptimer *)PTIMER_WATCH_BASE_ADDR;

int timer_init(void)
{
	/* disable timer */
	writel(PTIMER_CTRL_TIMER_DISABLE, &timer_base->ctrl);

	gd->cpu_clk   = get_cpu_clk();
	gd->cpu_clk <<= 10;

	/* PERIPHCLK = (CPU_CLK/4) */
	gd->arch.timer_rate_hz = (gd->cpu_clk >> 2);

	/* Load the timer counter register */
	writel(PTIMER_CTRL_LOAD_VAL, &timer_base->load);

	/* enable timer
	 * start the counter ticking up, reload value
	 *
	 * (PTIMER_CTRL_PRESCALE+1) * (PTIMER_CTRL_AUTO_RELOAD+1)
	 * -------------------------------------------------------
	 * 						PERIPHCLK
	 */
	writel(	PTIMER_CTRL_TIMER_ENABLE								|
			PTIMER_CTRL_AUTO_RELOAD									|
			(PTIMER_CTRL_PRESCALE << PTIMER_CTRL_PRESCALER_SHIFT),
		   &timer_base->ctrl);

	/* reset time */
	gd->arch.lastinc = readl(&timer_base->count) /
			(gd->arch.timer_rate_hz / CONFIG_SYS_HZ);
	gd->arch.tbl = 0;

	return 0;
}

ulong get_timer_masked(void)
{
	ulong now;

	now = readl(&timer_base->count) /
			(gd->arch.timer_rate_hz / CONFIG_SYS_HZ);

	if (gd->arch.lastinc >= now) {
		/* Normal mode */
		gd->arch.tbl += (gd->arch.lastinc - now);
	} else {
		/* We have an overflow ... */
		gd->arch.tbl += ((PTIMER_CTRL_LOAD_VAL / (gd->arch.timer_rate_hz / CONFIG_SYS_HZ))
				- now) + gd->arch.lastinc;
	}
	gd->arch.lastinc = now;
	return gd->arch.tbl;
}

ulong get_timer(ulong base)
{
	return get_timer_masked() - base;
}

unsigned long long get_ticks(void)
{
	return get_timer(0);
}

ulong get_tbclk(void)
{
	return CONFIG_SYS_HZ;
}

void __udelay(unsigned long usec)
{
	long tmo = usec * (gd->arch.timer_rate_hz / 1000) / 1000;
	unsigned long now, last = readl(&timer_base->count);

	while (tmo > 0) {
		now = readl(&timer_base->count);
		if (last > now)
			tmo -= last - now;
		else	/* count up timer overflow */
			tmo -= PTIMER_CTRL_OVERFLOW_VAL - now + last + 1;

		last = now;
	}
}
