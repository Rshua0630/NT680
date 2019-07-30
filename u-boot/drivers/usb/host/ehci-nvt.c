/*-
 * Copyright (c) 2007-2008, Juniper Networks, Inc.
 * Copyright (c) 2008, Excito Elektronik i Skåne AB
 * Copyright (c) 2008, Michael Trimarchi <trimarchimichael@yahoo.it>
 *
 * All rights reserved.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation version 2 of
 * the License.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 * MA 02111-1307 USA
 */
#include <common.h>
#include <errno.h>
#include <asm/byteorder.h>
#include <asm/unaligned.h>
#include <usb.h>
#include <asm/io.h>
#include <malloc.h>
#include <watchdog.h>
#include <linux/compiler.h>

#include "ehci.h"
#include "ehci-nvt.h"

unsigned int NVT_USB_BASE = 0xFC1F0000;
static void nvt_usb_set_default_port(void)
{
	//because different turnkey customer board has differ usb port layout, we need dynamic chage
	//port for each board, we init all port at first, and dynamic change NVT_USB_BASE for different board
	//in one image
	if(getenv("usb_port") != NULL) {
		int port = simple_strtoul(getenv("usb_port"),NULL,10);
	//	printf("usb port %d\n", port);
		switch(port) {
			case 0:
			default:
				NVT_USB_BASE = 0xFC1F0000;
				break;

			case 1:
				NVT_USB_BASE = 0xFC000000;
				break;

			case 2:
				NVT_USB_BASE = 0xFC140000;
				break;

			case 3:
				NVT_USB_BASE = 0xFC1A0000;
				break;
		}
	}

}

int ehci_hcd_init(int index, enum usb_init_type init, struct ehci_hccr **hccr, struct ehci_hcor **hcor)
{
	uint32_t addr;
	//printf("%s\n", __FUNCTION__);
	nvt_usb_set_default_port();

	addr = NVT_USB_BASE;

	//usb port 0, 1 ACP disable
	*(volatile unsigned long *)(0xfd120084) &= ~(0x3<<28);
	//usb port 2, 3 ACP disable
	*(volatile unsigned long *)(0xfd140084) &= ~(0x3<<29);

	//usb port 0, 1 pclk reset
	*(volatile unsigned long *)(0xfd020094) = (0x5<<9);
	udelay(0x20);
	*(volatile unsigned long *)(0xfd020090) = (0x5<<9);

	//usb port 2,3 pclk reset
	*(volatile unsigned long *)(0xfd0200e8) = (0x3<<23);
	udelay(0x20);
	*(volatile unsigned long *)(0xfd0200e4) = (0x3<<23);

	//# set PONRST to 1
	*(volatile unsigned long *)(NVT_USB0_APB_BASE) &= ~(0x1<<8);
	*(volatile unsigned long *)(NVT_USB2_APB_BASE) &= ~(0x1<<8);
	udelay(0x20);

	// waiting of APB read period
	*(volatile unsigned long *)(NVT_USB0_APB_BASE) &= ~(0x7 << 5);
	*(volatile unsigned long *)(NVT_USB1_APB_BASE) &= ~(0x7 << 5);
	*(volatile unsigned long *)(NVT_USB2_APB_BASE) &= ~(0x7 << 5);
	*(volatile unsigned long *)(NVT_USB3_APB_BASE) &= ~(0x7 << 5);
	udelay(0x20);
	*(volatile unsigned long *)(NVT_USB0_APB_BASE) |= (0x1 << 5);
	*(volatile unsigned long *)(NVT_USB1_APB_BASE) |= (0x1 << 5);
	*(volatile unsigned long *)(NVT_USB2_APB_BASE) |= (0x1 << 5);
	*(volatile unsigned long *)(NVT_USB3_APB_BASE) |= (0x1 << 5);
	udelay(0x20);

	//# select SUSPN and RST mask as 0
	*(volatile unsigned long *)(NVT_USB0_APB_BASE) &= ~(0x3<<14);
	*(volatile unsigned long *)(NVT_USB1_APB_BASE) &= ~(0x3<<14);
	*(volatile unsigned long *)(NVT_USB2_APB_BASE) &= ~(0x3<<14);
	*(volatile unsigned long *)(NVT_USB3_APB_BASE) &= ~(0x3<<14);
	udelay(0x20);

	//# set usb port 0,1 uclk sw reset
	*(volatile unsigned long *)(0xfd020054) = (0x3<<8);
	//# set usb port 2,3 uclk sw reset
	*(volatile unsigned long *)(0xfd0200d8) = (3<<25);
	udelay(0x20);

	//# set usb port 0,1 hclk sw reset
	*(volatile unsigned long *)(0xfd020094) = (0x5<<8);
	//# set usb port 2,3 hclk sw reset
	*(volatile unsigned long *)(0xfd0200e8) = (3<<21);
	udelay(0x20);

	//# set usb port 0,1 aclk sw reset
	*(volatile unsigned long *)(0xfd020074) = (0x3<<6);
	//# set usb port 2,3 aclk sw reset
	*(volatile unsigned long *)(0xfd0200e0) = (3<<16);
	udelay(0x20);

	//# set usb port 0,1 aclk sw clear
	*(volatile unsigned long *)(0xfd020070) = (0x3<<6);
	//# set usb port 2,3 aclk sw clear
	*(volatile unsigned long *)(0xfd0200dc) = (3<<16);
	udelay(0x20);

	//# set usb port 0,1 hclk sw clear
	*(volatile unsigned long *)(0xfd020090) = (0x5<<8);
	//# set usb port 2,3 hclk sw clear
	*(volatile unsigned long *)(0xfd0200e4) = (3<<21);
	udelay(0x20);

	//# set usb port 0,1 uclk sw clear
	*(volatile unsigned long *)(0xfd020050) = (0x3<<8);
	//# set usb port 2,3 uclk sw clear
	*(volatile unsigned long *)(0xfd0200d4) = (3<<25);
	udelay(0x20);

	//#set usb port 0,1 UCLK to PHY 30M
	*(volatile unsigned long *)(0xfd020048) |= (0x1<<27);
	//#set usb port 2,3 UCLK to PHY 30M
	*(volatile unsigned long *)(0xfd020048) |= (0x1<<22);
	udelay(0x20);

	//pw setting
	//*(volatile unsigned long *)(0xfd0d0004) |= (0xF<<20);
	//udelay(0x20);
	//*(volatile unsigned long *)(0xfd0d0008) |= (0xF<<20);

	writel(0x20,(volatile void *)(NVT_USB_BASE+0x100));
	writel(0x0200000e,(volatile void *)(NVT_USB_BASE+0xe0));
	writel(readl((const volatile void *)NVT_USB_BASE+0x84),(volatile void *)NVT_USB_BASE+0x84);

	set((NVT_USB_BASE+0xc4),0x3);

	clear(NVT_USB_BASE+0x88,clearMode);
	set(NVT_USB_BASE+0x88,setMode);

	clear((NVT_USB_BASE+0x80),0x20);
	set((NVT_USB_BASE+0x80),0x10);
	//for full speed device
	//set((NVT_USB_BASE + 0x80),1<<28);

	*hccr = (struct ehci_hccr *)addr;
	*hcor = (struct ehci_hcor *)((uint32_t)*hccr +
			HC_LENGTH(ehci_readl(&(*hccr)->cr_capbase)));

	return 0;
}

int ehci_hcd_stop(int index)
{
    return 0;
}

#define __REG(x)        (*((volatile u32 *) (x)))

void usb_power_on(void)
{
#if defined(CONFIG_NT72668)
	//USB0 PWEN
	__REG(0xFD100420) &= ~(1 << 14);
	//USB1 PWEN
	__REG(0xFD100420) &= ~(1 << 15);
	//USB2 PWEN
	__REG(0xFD100420) &= ~(1 << 16);
	//USB3 PWEN
	__REG(0xFD100420) &= ~(1 << 17);

	__REG(0xFD100408) |= ((1 << 20) | (1 << 21) | (1 << 22) | (1 << 23));
	__REG(0xFD100404) |= ((1 << 20) | (1 << 21) | (1 << 22) | (1 << 23));
#endif
#if defined(CONFIG_NT72656)
	__REG(0xFD0F0224) &= 0xFFF0FFFF;
	__REG(0xFD0F0224) &= 0xFF0FFFFF;
	__REG(0xFD0F0220) &= 0xFFF0FFFF;
	__REG(0xFD0F0220) &= 0xFF0FFFFF;

	__REG(0xFD0F0208) |= (1 << 12) | (1 << 13) | (1 << 4) | (1 << 5);
	__REG(0xFD0F0204) |= (1 << 12) | (1 << 13) | (1 << 4) | (1 << 5);
#endif
}

void usb_power_off(void)
{
#if defined(CONFIG_NT72668)
	//USB0 PWEN
	__REG(0xFD100420) |= (1 << 14);
	//USB1 PWEN
	__REG(0xFD100420) |= (1 << 15);
	//USB2 PWEN
	__REG(0xFD100420) |= (1 << 16);
	//USB3 PWEN
	__REG(0xFD100420) |= (1 << 17);

	__REG(0xFD100408) &= ~((1 << 20) | (1 << 21) | (1 << 22) | (1 << 23));
	__REG(0xFD100404) &= ~((1 << 20) | (1 << 21) | (1 << 22) | (1 << 23));
#endif
#if defined(CONFIG_NT72656)
	__REG(0xFD0F0224) &= 0xFFF0FFFF;
	__REG(0xFD0F0224) &= 0xFF0FFFFF;
	__REG(0xFD0F0220) &= 0xFFF0FFFF;
	__REG(0xFD0F0220) &= 0xFF0FFFFF;

	__REG(0xFD0F0208) |= (1 << 12) | (1 << 13) | (1 << 4) | (1 << 5);
	__REG(0xFD0F0200) |= (1 << 12) | (1 << 13) | (1 << 4) | (1 << 5);
#endif
}
