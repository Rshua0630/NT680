#ifndef NVTUSB_REGS
#define NVTUSB_REGS

#include <linux/module.h>
#include <linux/platform_device.h>
#include <linux/writeback.h>
/*#include <mach/clk.h>*/
#include <soc/nvt/clk.h>
/*#include "mach/motherboard.h"*/

#define NVT_USB_MODIFICATION_TIME "2016-02-25"
#define NVT_USB_MODIFICATION_VERSION "v00003"
#define OTGC_INT_BSRPDN							1
#define OTGC_INT_ASRPDET						(1<<4)
#define OTGC_INT_AVBUSERR						(1<<5)
#define OTGC_INT_RLCHG							(1<<8)
#define OTGC_INT_IDCHG							(1<<9)
#define OTGC_INT_OVC							(1<<10)
#define OTGC_INT_BPLGRMV						(1<<11)
#define OTGC_INT_APLGRMV						(1<<12)

#define OTGC_INT_A_TYPE	(OTGC_INT_ASRPDET|OTGC_INT_AVBUSERR|OTGC_INT_OVC \
					|OTGC_INT_RLCHG|OTGC_INT_IDCHG \
					|OTGC_INT_BPLGRMV|OTGC_INT_APLGRMV)
#define OTGC_INT_B_TYPE	(OTGC_INT_BSRPDN|OTGC_INT_AVBUSERR|OTGC_INT_OVC \
					|OTGC_INT_RLCHG|OTGC_INT_IDCHG)

#define clear(val, regs)        writel(~((u32)val) & readl(regs), regs)
#define set(val, regs)          writel(val | readl(regs), regs)

int xhci_NT72668_probe(struct platform_device *pdev);

struct nt72668_platform_data {
	enum EN_SYS_CLK_RST  ahb_usb20;
	enum EN_SYS_CLK_RST  ahb_usb20_pclk;
	enum EN_SYS_CLK_RST  axi_usb20;
	enum EN_SYS_CLK_RST  core_usb20;
	enum EN_SYS_CLK_SRC  clk_src;
};

#endif

