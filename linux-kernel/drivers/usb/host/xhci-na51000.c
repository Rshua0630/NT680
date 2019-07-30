/*
 * Copyright (C) 2017 Novatek
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * version 2 as published by the Free Software Foundation.
 */

#include <linux/delay.h>
#include <linux/platform_device.h>
#include <linux/gpio.h>
#include <mach/hardware.h>
#include <mach/nvt-gpio.h>
#include <asm/io.h>

#include "xhci.h"


#define PLL_SETREG(ofs,value)	writel(value, (NVT_CG_BASE_VIRT+(ofs)))
#define PLL_GETREG(ofs)		readl(NVT_CG_BASE_VIRT+(ofs))

#define USB3_SETREG(ofs,value)	writel((value), (NVT_USB3_BASE_VIRT+(ofs)))
#define USB3_GETREG(ofs)	readl(NVT_USB3_BASE_VIRT+(ofs))

#define USB2APB_SETREG(ofs,value)	writel((value), (volatile void __iomem *)(0xFD600000+(ofs)))
#define USB3APB_SETREG(ofs,value)	writel((value), (volatile void __iomem *)(0xFD670000+(ofs)))
#define USB3APB_GETREG(ofs)	readl((volatile void __iomem *)(0xFD670000+(ofs)))

#define USB3PHY_SETREG(ofs,value)	writel((value), (volatile void __iomem *)(0xFD672000+((ofs)<<2)))
#define USB3PHY_GETREG(ofs)	readl((volatile void __iomem *)(0xFD672000+((ofs)<<2)))
#define REG_SETREG(ofs,value)	writel((value), (volatile void __iomem *)(ofs))
#define REG_GETREG(ofs)	readl((volatile void __iomem *)(ofs))

#define USB3_SIMULATE_POWEROFF		0

#define PLL_OFS 					0x0
#define PLL12_MASK 					0x1000
#define AXI_BRDE_OFS 				0xB4
#define AXI_BRDE_MASK 				0x100000
#define USB3_REF_SUSP_CLK_OFS 		0x90
#define USB3_REF_SUSP_MASK 			0x300
#define USB3_GCTL_OFS 				0xC110
#define USB3_GCTL_CONFIG 			0x17711000
#define USB3_GCTL_MASK 				0xFFF93030
#define USB3_GSBUSCFG0_OFS 			0xC100
#define USB3_GSBUSCFG1_OFS 			0xC104
#define USB3_GSBUSCFG0_DEFAULT 		0x00000001
#define USB3_GSBUSCFG1_DEFAULT 		0x00000300
#define USB3_GUSB2PHYCFG_OFS 		0xC200
#define USB3_GUSB3PIPECTL_OFS 		0xC2C0
#define USB3_GUSB2PHYCFG_DEFAULT    0x40102408
#define USB3_GUSB3PIPECTL_DEFAULT   0x010C0003

void xhci_nvt_host_init(void)
{
	u32 reg;
	int err = 0;

	#ifdef CONFIG_PM
	err = gpio_request(P_GPIO(31), "usb3_pwr");
	if (err < 0) {
		pr_info("%s: Faiiled to request gpio %d for USB 3.0 POWER\n", __func__, P_GPIO(31));
		return;
	}

	err = gpio_direction_output(P_GPIO(31), 0);
	if (err) {
		pr_info("%s: USB 3.0 POWER didn't output high\n", __FUNCTION__);
		return;
	}

	gpio_set_value(P_GPIO(31), 1);

	gpio_free(P_GPIO(31));
	#endif

	#if USB3_SIMULATE_POWEROFF
	printk("xhci_nvt_host_init\n");
	{
		u32 temp;
		printk("xhci_nvt_host_init assert reset\n");
		temp = readl(NVT_CG_BASE_VIRT+(0xA8));
		temp &= ~(0x1<<17);
		writel(temp, (NVT_CG_BASE_VIRT+(0xA8)));
		udelay(1000);
		temp |= (0x1<<17);
		writel(temp, (NVT_CG_BASE_VIRT+(0xA8)));
		udelay(1000);
		printk("R114=0x%02X\n",USB3PHY_GETREG(0x114));
	}
	#endif

	/*Enable PLL12*/
	reg = PLL_GETREG(PLL_OFS);
	if (!(reg & PLL12_MASK))
		PLL_SETREG(PLL_OFS, reg | PLL12_MASK);

	do{
		reg = PLL_GETREG(0x4);
	}while(!(reg & PLL12_MASK));

	udelay(2);

	/*Disable AXI-Bridge Reset*/
	reg = PLL_GETREG(AXI_BRDE_OFS);
	PLL_SETREG(AXI_BRDE_OFS, reg | AXI_BRDE_MASK);

	udelay(2);

	/*Enable USB3 ref and suspend clk*/
	reg = PLL_GETREG(USB3_REF_SUSP_CLK_OFS);
	PLL_SETREG(USB3_REF_SUSP_CLK_OFS, reg | USB3_REF_SUSP_MASK);

	udelay(2);

	/*Config Core Control Register GCTL*/
	reg = USB3_GETREG(USB3_GCTL_OFS);
	reg &= ~USB3_GCTL_MASK;
	USB3_SETREG(USB3_GCTL_OFS, reg | USB3_GCTL_CONFIG);

	/*Config GSBUSCFG0/GSBUSCFG1*/
	USB3_SETREG(USB3_GSBUSCFG0_OFS, USB3_GSBUSCFG0_DEFAULT);
	USB3_SETREG(USB3_GSBUSCFG1_OFS, USB3_GSBUSCFG1_DEFAULT);

	/*Setting PHY Control Default value*/
	USB3_SETREG(USB3_GUSB2PHYCFG_OFS,  USB3_GUSB2PHYCFG_DEFAULT);
	USB3_SETREG(USB3_GUSB3PIPECTL_OFS, USB3_GUSB3PIPECTL_DEFAULT);

	/* APB Read delay. This makes APB PHY REG read functioning */
	USB3APB_SETREG(0x34, 0x33);

	/* Set VBUS */
	USB3APB_SETREG(0x10, 0x0);

	/* asd_mode=1 (toggle rate) */
	USB3PHY_SETREG(0x198, 0x04);
	/* RX_ICTRL's offset = 0 */
    USB3PHY_SETREG(0x1BF, 0x40);
	/* TX_AMP_CTL=3, TX_DEC_EM_CTL=f */
    USB3PHY_SETREG(0x014, 0xfb);
	/* TX_LFPS_AMP_CTL = 1 */
    USB3PHY_SETREG(0x034, 0xfc);
	/* PHY Power Mode Change ready reponse time. (3 is 1ms.)(4 is 1.3ms.) */
    USB3PHY_SETREG(0x114, 0x0B);
    USB3PHY_SETREG(0x152, 0x2E);
    USB3PHY_SETREG(0x153, 0x01);
    USB3PHY_SETREG(0x1B0, 0xC0);
    USB3PHY_SETREG(0x1B1, 0x91);
    USB3PHY_SETREG(0x1B2, 0x00);
    USB3PHY_SETREG(0x135, 0x88);
    USB3PHY_SETREG(0x12A, 0x50);
    USB3PHY_SETREG(0x1F0, 0x80);
    USB3PHY_SETREG(0x1F5, 0xB1);
    USB3PHY_SETREG(0x105, 0x01);
    udelay(2);
    USB3PHY_SETREG(0x102, 0x20);
    udelay(10);
    USB3PHY_SETREG(0x102, 0x00);
    udelay(300);

	/* disable_power_saving */
	USB3APB_SETREG((0x1000+(0xE<<2)), 	0x04);
	USB2APB_SETREG((0x1000+(0xE<<2)), 	0x04);

	udelay(50);

	/* Force termination turn on */
	USB3PHY_SETREG(0x029, 0x1);

	#if USB3_SIMULATE_POWEROFF
	printk("R114=0x%02X\n",USB3PHY_GETREG(0x114));
	#endif
}

void xhci_nvt_host_uninit(void)
{
	u32 reg;

	/* Restore PRTCAPDIR to Device, Or the Charger detect would be failed during OTG usage */
	reg = USB3_GETREG(USB3_GCTL_OFS);
	reg &= ~0x3000;
	USB3_SETREG(USB3_GCTL_OFS, reg | 0x2000);//Device mode

	/* enable_power_saving */
	USB3APB_SETREG((0x1000+(0xE<<2)), 	0x00);
	USB2APB_SETREG((0x1000+(0xE<<2)), 	0x00);

	/* Set PHY Suspend */
	USB3_SETREG(USB3_GUSB2PHYCFG_OFS,  USB3_GETREG(USB3_GUSB2PHYCFG_OFS) |(0x1<<6));
	USB3_SETREG(USB3_GUSB3PIPECTL_OFS, USB3_GETREG(USB3_GUSB3PIPECTL_OFS)|(0x1<<17));
}

void xhci_nvt_fuse_log(struct xhci_hcd *xhci)
{
	u32 fusedata=0;

	fusedata += ((USB3PHY_GETREG(0x1A0) & 0xFF) << 0);
	fusedata += ((USB3APB_GETREG(0x1000+(0x6<<2)) & 0xFF) << 8);
	fusedata += ((USB3APB_GETREG(0x1000+(0x5<<2)) & 0xFF) << 16);
	xhci->fuseData = fusedata;
}

void xhci_nvt_fuse_restore(struct xhci_hcd *xhci)
{
	USB3PHY_SETREG(0x1A0, 				(xhci->fuseData >> 0)&0xFF);
	USB3APB_SETREG((0x1000+(0x6<<2)), 	(xhci->fuseData >> 8)&0xFF);
	USB3APB_SETREG((0x1000+(0x5<<2)), 	(xhci->fuseData >> 16)&0xFF);
}

