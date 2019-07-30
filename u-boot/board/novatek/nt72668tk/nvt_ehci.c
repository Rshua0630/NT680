/*
 * Copyright (C) 2014 Novatek Microelectronics Corp. All rights reserved.
 * Author: SP-KSW <SP_KSW_MailGrp@novatek.com.tw>
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#include <common.h>
#include <usb.h>
#include <errno.h>
#include <asm/io.h>
#include <asm/arch/gpio.h>
#include <asm/arch/ehci.h>

#include "ehci.h"

typedef enum vbus_power_e{
	VBUS_OFF = 0,
	VBUS_ON
} vbus_power_t;

static void nvt_ehci_power(vbus_power_t enable)
{
	u32 mux, dir, level;

	/* GPIO mux select:
	 * 0: select GPIO; 1: select functional
	 * [19:16] 0: GPB_12 / 1:CI_HA10 / 2:Tensilica_TCK / 4:USB0_PWEN / 5:Ambi_SPI_DI
	 */
	mux  = readl((GPIO_GRPB_BASE_ADDR+GPIO_GRPB_MUX_SELECT1_OFFSET));
	mux &= ~(0xf<<16);	/* as a GPB_12 */
	writel(mux, (GPIO_GRPB_BASE_ADDR+GPIO_GRPB_MUX_SELECT1_OFFSET));

	dir  = readl((GPIO_GRPB_BASE_ADDR+GPIO_GRPB_DIR_MODE_OFFSET));
	dir |= (1<<12);		/* output */
	writel(dir, (GPIO_GRPB_BASE_ADDR+GPIO_GRPB_DIR_MODE_OFFSET));

	if (VBUS_ON == enable)
	{
		level  = readl((GPIO_GRPB_BASE_ADDR+GPIO_GRPB_SET_ENABLE_OFFSET));
		level |= (1<<12);		/* level high */
		writel(level, (GPIO_GRPB_BASE_ADDR+GPIO_GRPB_SET_ENABLE_OFFSET));
	}
	else
	{
		level  = readl((GPIO_GRPB_BASE_ADDR+GPIO_GRPB_CLEAR_ENABLE_OFFSET));
		level |= (1<<12);		/* level low */
		writel(level, (GPIO_GRPB_BASE_ADDR+GPIO_GRPB_CLEAR_ENABLE_OFFSET));
	}
}

static void nvt_ehci_clk_reset(void)
{

}

int ehci_hcd_init(int index, enum usb_init_type init,
		struct ehci_hccr **hccr, struct ehci_hcor **hcor)
{
	debug("Init NT72656 EHCI\n");

	nvt_ehci_power(VBUS_ON);

	nvt_ehci_clk_reset();

	*hccr = (struct ehci_hccr *)(EHCI_BASE_ADDR);
	*hcor = (struct ehci_hcor *)(EHCI_BASE_ADDR + 0x10);

	debug("NT72656 EHCI init done\n");
	return 0;
}

int ehci_hcd_stop(int index)
{
	nvt_ehci_power(VBUS_OFF);

	return 0;
}
