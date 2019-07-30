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
#include <asm/arch/gpio.h>

#include <linux/sizes.h>
#include <linux/compiler.h>

DECLARE_GLOBAL_DATA_PTR;

void __weak reset_cpu(unsigned long ignored)
{
}

/*
 * Routine: dram_init
 * Description: sets uboots idea of sdram size
 */
int dram_init(void)
{
	gd->ram_size = CONFIG_SYS_SDRAM_SIZE;
	return 0;
}

/*
 * Print board information
 */
int checkboard(void)
{
	return 0;
}

/*
 * Print CPU information
 */
#ifdef CONFIG_DISPLAY_CPUINFO
int print_cpuinfo(void)
{
	printf("CPU:   Novatek NT72656 @ %d MHz\n",
	       get_cpu_clk()/1000);

	return 0;
}
#endif

void s_init(void)
{
	u32 val;

	/* Do we want to combine ARM_UART_B and STBC_UART_1 on EVB board?
	 * NO! So we need set fc040250[bit0] to "0"
	 */
	val = *(volatile unsigned int *)0xfc040250;
	if ((val & 0x01) == 0x01) {
		val &= ~0x01;
		writel(0x0, 0xfc040250);
	}

	gd->cpu_clk = get_cpu_clk() * 1000;
}
