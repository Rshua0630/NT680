/*
 * Copyright (C) 2014 Novatek Microelectronics Corp. All rights reserved.
 * Author: SP-KSW <SP_KSW_MailGrp@novatek.com.tw>
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#ifndef __ARCH_CLOCK_H_
#define __ARCH_CLOCK_H_

#define CPU_PLL_OFFSET			0x62
#define AXI_PLL_OFFSET			0x6a
#define DDR_PLL_OFFSET			0x82
#define MPG_PLL_OFFSET			0x9a
#define AHB_PLL_OFFSET			0x9d
#define ETH_PLL_OFFSET			0xaa
#define MMC_PLL_OFFSET			0xc4
#define MEMC_NR_PLL_OFFSET		0x1d

#define CLK_BASE_ADDR			(0xfd020000)

#define APLL_BASE_ADDR			(0xfd720000)
#define MPLL_BASE_ADDR			(0xfd670000)

#define APLL_PAGE_B_EN			(0x00000002)
#define APLL_PAGE_0_EN			(0x00000001)
#define APLL_PAGE_EN_ADDR		(0xfd0f0c2c)

#define MPLL_PAGE_B_EN			(0x00000001)
#define MPLL_PAGE_0_EN			(0x00000002)
#define MPLL_PAGE_EN_ADDR		(CLK_BASE_ADDR + 0xbc)

typedef enum pll_page_e {
	PLL_PAGE_0,
	PLL_PAGE_B
} pll_page_t;

void 			set_sys_mpll(unsigned long off, unsigned long val);
unsigned long	get_sys_mpll(unsigned long off);
void			set_cpu_clk(unsigned long freq);
unsigned long	get_cpu_clk(void);

#endif /* __ARCH_CLOCK_H_ */
