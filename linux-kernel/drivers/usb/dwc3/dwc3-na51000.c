/* Copyright (c) 2013-2014, The Linux Foundation. All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 and
 * only version 2 as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */

#include <linux/delay.h>
#include <linux/platform_device.h>
#include <mach/hardware.h>
#include <asm/io.h>

#include "core.h"

#define PLL_SETREG(ofs,value)	writel(value, (NVT_CG_BASE_VIRT+(ofs)))
#define PLL_GETREG(ofs)		readl(NVT_CG_BASE_VIRT+(ofs))

#define USB3_SETREG(ofs,value)	writel((value), (NVT_USB3_BASE_VIRT+(ofs)))
#define USB3_GETREG(ofs)	readl(NVT_USB3_BASE_VIRT+(ofs))

#define USB2APB_SETREG(ofs,value)	writel((value), (volatile void __iomem *)(0xFD600000+(ofs)))
#define USB3APB_SETREG(ofs,value)	writel((value), (volatile void __iomem *)(0xFD670000+(ofs)))
#define USB3APB_GETREG(ofs)	readl((volatile void __iomem *)(0xFD670000+(ofs)))

#define USB3PHY_SETREG(ofs,value)	writel((value), (volatile void __iomem *)(0xFD672000+((ofs)<<2)))
#define USB3PHY_GETREG(ofs)	readl((volatile void __iomem *)(0xFD672000+((ofs)<<2)))

#define USB3_SIMULATE_POWEROFF		0

#define PLL_OFS 					0x0
#define PLL12_MASK 					0x1000
#define AXI_BRDE_OFS 				0xB4
#define AXI_BRDE_MASK 				0x100000
#define USB3_REF_SUSP_CLK_OFS 		0x90
#define USB3_REF_SUSP_MASK 			0x300
#define USB3_RST_OFS 				0xA8
#define USB3_RST_BIT 				(0x01<<17)
#define USB3_GCTL_OFS 				0xC110
#define USB3_GCTL_CONFIG 			0x17712000
#define USB3_GCTL_MASK 				0xFFF93030
#define USB3_GSBUSCFG0_OFS 			0xC100
#define USB3_GSBUSCFG1_OFS 			0xC104
#define USB3_GSBUSCFG0_DEFAULT 		0x00000001
#define USB3_GSBUSCFG1_DEFAULT 		0x00000300
#define USB3_GUSB2PHYCFG_OFS 		0xC200
#define USB3_GUSB3PIPECTL_OFS 		0xC2C0
#define USB3_GUSB2PHYCFG_DEFAULT	0x40102408
#define USB3_GUSB3PIPECTL_DEFAULT	0x010C0003

void dwc3_nvtivot_device_init(void)
{
	u32 reg;

	#if USB3_SIMULATE_POWEROFF
	printk("dwc3_nvtivot_device_init\n");
	{
		u32 temp;
		printk("dwc3_nvtivot_device_init assert reset\n");
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

	/* Enable PLL12*/
	reg = PLL_GETREG(PLL_OFS);
	if (!(reg & PLL12_MASK))
		PLL_SETREG(PLL_OFS, reg | PLL12_MASK);

	/* todo: log efuse settings and restore */

	/* Assert async reset for 680A bug patch */
	reg = PLL_GETREG(USB3_RST_OFS);
	PLL_SETREG(USB3_RST_OFS, reg & ~USB3_RST_BIT);
	PLL_SETREG(USB3_RST_OFS, reg | USB3_RST_BIT);

	udelay(10);

	/*Disable AXI-Bridge Reset*/
	reg = PLL_GETREG(AXI_BRDE_OFS);
	PLL_SETREG(AXI_BRDE_OFS, reg | AXI_BRDE_MASK);

	udelay(2);

	/* Enable USB3 ref and suspend clk*/
	reg = PLL_GETREG(USB3_REF_SUSP_CLK_OFS);
	PLL_SETREG(USB3_REF_SUSP_CLK_OFS, reg | USB3_REF_SUSP_MASK);

	udelay(2);

	/* Config Core Control Register GCTL*/
	reg = USB3_GETREG(USB3_GCTL_OFS);
	reg &= ~USB3_GCTL_MASK;
	USB3_SETREG(USB3_GCTL_OFS, reg | USB3_GCTL_CONFIG);

	/* Config GSBUSCFG0/GSBUSCFG1*/
	USB3_SETREG(USB3_GSBUSCFG0_OFS, USB3_GSBUSCFG0_DEFAULT);
	USB3_SETREG(USB3_GSBUSCFG1_OFS, USB3_GSBUSCFG1_DEFAULT);

	/* Setting PHY Control Default value*/
	USB3_SETREG(USB3_GUSB2PHYCFG_OFS,  USB3_GUSB2PHYCFG_DEFAULT);
	USB3_SETREG(USB3_GUSB3PIPECTL_OFS, USB3_GUSB3PIPECTL_DEFAULT);

	/* APB Read delay. This makes APB PHY REG read functioning */
	USB3APB_SETREG(0x34, 0x33);

	/* Set VBUS */
	USB3APB_SETREG(0x10, 0x3);

	/* Force termination turn on */
	USB3PHY_SETREG(0x029, 0x1);

	/* BIAS_ICDR_SEL_ini=4'hf */
	USB3PHY_SETREG(0x1F5, 0xF1);
	/* BIAS_ICDR_SEL load_en=1 */
	USB3PHY_SETREG(0x1F0, 0x80);
	/* BIAS_ICDR_SEL load_en=0 */
	USB3PHY_SETREG(0x1F0, 0x00);
	/* asd_mode=1 (toggle rate) */
	USB3PHY_SETREG(0x198, 0x04);
	/* TX_AMP_CTL=3, TX_DEC_EM_CTL=f */
	USB3PHY_SETREG(0x014, 0xfb);
	/* Enable software control */
	USB3PHY_SETREG(0x034, 0xff);
	/* PHY Power Mode Change ready reponse time. (3 is 1ms.)(4 is 1.3ms.) */
	USB3PHY_SETREG(0x114, 0x03);

	/* disable_power_saving */
	USB3APB_SETREG((0x1000+(0xE<<2)), 	0x04);
	USB2APB_SETREG((0x1000+(0xE<<2)), 	0x04);

	#if USB3_SIMULATE_POWEROFF
	printk("R114=0x%02X\n",USB3PHY_GETREG(0x114));
	#endif

}


void dwc3_nvtivot_device_uninit(void)
{
	/* enable_power_saving */
	USB3APB_SETREG((0x1000+(0xE<<2)), 	0x00);
	USB2APB_SETREG((0x1000+(0xE<<2)), 	0x00);

	/* Set PHY Suspend */
	USB3_SETREG(USB3_GUSB2PHYCFG_OFS,  USB3_GETREG(USB3_GUSB2PHYCFG_OFS) |(0x1<<6));
	USB3_SETREG(USB3_GUSB3PIPECTL_OFS, USB3_GETREG(USB3_GUSB3PIPECTL_OFS)|(0x1<<17));
}

void dwc3_nvtivot_fuse_log(struct dwc3 *dwc)
{
	u32 fusedata=0;

	fusedata += ((USB3PHY_GETREG(0x1A0) & 0xFF) << 0);
	fusedata += ((USB3APB_GETREG(0x1000+(0x6<<2)) & 0xFF) << 8);
	fusedata += ((USB3APB_GETREG(0x1000+(0x5<<2)) & 0xFF) << 16);
	dwc->fuseData = fusedata;
}

void dwc3_nvtivot_fuse_restore(struct dwc3 *dwc)
{
	USB3PHY_SETREG(0x1A0, 				(dwc->fuseData >> 0)&0xFF);
	USB3APB_SETREG((0x1000+(0x6<<2)), 	(dwc->fuseData >> 8)&0xFF);
	USB3APB_SETREG((0x1000+(0x5<<2)), 	(dwc->fuseData >> 16)&0xFF);
}

void dwc3_nvtivot_enable_power_saving(void)
{
	USB3APB_SETREG((0x1000+(0xE<<2)), 	0x00);
	USB2APB_SETREG((0x1000+(0xE<<2)), 	0x00);
}

void dwc3_nvtivot_disable_power_saving(void)
{
	USB3APB_SETREG((0x1000+(0xE<<2)), 	0x04);
	USB2APB_SETREG((0x1000+(0xE<<2)), 	0x04);
}

