/*
 * che-chun Kuo <c_c_kuo@novatek.com.tw>
 *
 * This file is licenced under the GPL.
 */

#include "NT72668.h"
#include <linux/of.h>
#include <linux/of_address.h>
#include <linux/of_irq.h>
#include <linux/spinlock.h>

#ifdef CONFIG_DMA_COHERENT
#define USBH_ENABLE_INIT  (USBH_ENABLE_CE \
			| USB_MCFG_PFEN | USB_MCFG_RDCOMB \
			| USB_MCFG_SSDEN | USB_MCFG_UCAM \
			| USB_MCFG_EBMEN | USB_MCFG_EMEMEN)
#else
#define USBH_ENABLE_INIT  (USBH_ENABLE_CE \
			| USB_MCFG_PFEN | USB_MCFG_RDCOMB \
			| USB_MCFG_SSDEN \
			| USB_MCFG_EBMEN | USB_MCFG_EMEMEN)
#endif

static const struct nt72668_platform_data ehci_pdata[4] = {
	{
		.ahb_usb20      = EN_SYS_CLK_RST_AHB_USB20_U0,
		.ahb_usb20_pclk = EN_SYS_CLK_RST_AHB_USB20_U0_PCLK,
		.axi_usb20      = EN_SYS_CLK_RST_AXI_USB20_U0,
		.core_usb20     = EN_SYS_CLK_RST_CORE_USB20_U0,
		.clk_src        = EN_SYS_CLK_SRC_USB_U0_U1_30M,
	},
	{
		.ahb_usb20      = EN_SYS_CLK_RST_AHB_USB20_U1,
		.ahb_usb20_pclk = EN_SYS_CLK_RST_AHB_USB20_U1_PCLK,
		.axi_usb20      = EN_SYS_CLK_RST_AXI_USB20_U1,
		.core_usb20     = EN_SYS_CLK_RST_CORE_USB20_U1,
		.clk_src        = EN_SYS_CLK_SRC_USB_U0_U1_30M,
	},
	{
		.ahb_usb20      = EN_SYS_CLK_RST_AHB_USB20_U2,
		.ahb_usb20_pclk = EN_SYS_CLK_RST_AHB_USB20_U2_PCLK,
		.axi_usb20      = EN_SYS_CLK_RST_AXI_USB20_U2,
		.core_usb20     = EN_SYS_CLK_RST_CORE_USB20_U2,
		.clk_src        = EN_SYS_CLK_SRC_USB_U2_U3_30M,
	},
	{
		.ahb_usb20      = EN_SYS_CLK_RST_AHB_USB20_U3,
		.ahb_usb20_pclk = EN_SYS_CLK_RST_AHB_USB20_U3_PCLK,
		.axi_usb20      = EN_SYS_CLK_RST_AXI_USB20_U3,
		.core_usb20     = EN_SYS_CLK_RST_CORE_USB20_U3,
		.clk_src        = EN_SYS_CLK_SRC_USB_U2_U3_30M,
	},
};

void NT72668_Plat_Resume(struct usb_hcd *hcd)
{
	struct ehci_hcd *ehci = hcd_to_ehci(hcd);
	int retval, retry = 0;

	ehci_writel(ehci, ehci->periodic_dma, &ehci->regs->frame_list);
	ehci_writel(ehci, (u32)ehci->async->qh_dma, &ehci->regs->async_next);
	retry = 0;
	do {
		ehci_writel(ehci, INTR_MASK,
		&ehci->regs->intr_enable);
		retval = ehci_handshake(ehci, &ehci->regs->intr_enable,
		    INTR_MASK, INTR_MASK, 250);
		retry++;
	} while (retval != 0);

	if (unlikely(retval != 0))
		ehci_err(ehci, "write fail!\n");
}

static void NT72668_init(struct platform_device *dev)
{
	struct device_node *dn = dev->dev.of_node;
	struct usb_hcd *hcd = platform_get_drvdata(dev);
	u32 id;

	if (of_property_read_u32(dn, "id", &id) != 0) {
		dev_err(&dev->dev, "could not get usb id\n");
		return;
	}

	SYS_SetClockReset(ehci_pdata[id].ahb_usb20_pclk, true);
	udelay(0x20);
	SYS_SetClockReset(ehci_pdata[id].ahb_usb20_pclk, false);

	usb_phy_init(hcd->usb_phy);

	/* # set uclk sw reset */
	SYS_SetClockReset(ehci_pdata[id].core_usb20, true);
	udelay(0x20);

	/* # set hclk sw reset */
	SYS_SetClockReset(ehci_pdata[id].ahb_usb20, true);
	udelay(0x20);

	/* # set aclk sw reset */
	SYS_SetClockReset(ehci_pdata[id].axi_usb20, true);
	udelay(0x20);

	/* # set aclk sw clear */
	SYS_SetClockReset(ehci_pdata[id].axi_usb20, false);
	udelay(0x20);

	/* # set hclk sw clear */
	SYS_SetClockReset(ehci_pdata[id].ahb_usb20, false);
	udelay(0x20);

	/* # set uclk sw clear */
	SYS_SetClockReset(ehci_pdata[id].core_usb20, false);
	udelay(0x20);

	/* #set UCLK to PHY 30M */
	SYS_CLK_SetClockSource(ehci_pdata[id].clk_src, 1);
	udelay(0x20);

	writel(0x20, hcd->regs + 0x100);
	writel(0x0200006e, hcd->regs + 0xe0);
	writel(readl(hcd->regs + 0x84), hcd->regs + 0x84);

	set(0xb, hcd->regs + 0xc4);
	clear(OTGC_INT_B_TYPE, hcd->regs + 0x88);
	set(OTGC_INT_A_TYPE, hcd->regs + 0x88);

	clear(0x20, hcd->regs + 0x80);
	set(0x10, hcd->regs + 0x80);

	/* for full speed device */
	set(1 << 28, hcd->regs + 0x80);

	/* for solving full speed BT disconnect issue */
	writel(0x6000, hcd->regs + 0x44);
}

static void NT72668_stop(struct platform_device *dev)
{
	struct device_node *dn = dev->dev.of_node;
	struct usb_hcd *hcd = platform_get_drvdata(dev);
	u32 id;

	if (of_property_read_u32(dn, "id", &id) != 0) {
		dev_err(&dev->dev, "could not get usb id\n");
		return;
	}

	writel(0x2, hcd->regs + 0x10);
	udelay(1000);

	SYS_SetClockReset(ehci_pdata[id].ahb_usb20_pclk, true);
	udelay(0x20);
	SYS_SetClockReset(ehci_pdata[id].ahb_usb20_pclk, false);

	usb_phy_shutdown(hcd->usb_phy);

	/* # set uclk sw reset */
	SYS_SetClockReset(ehci_pdata[id].core_usb20, true);
	udelay(0x20);

	/* # set hclk sw reset */
	SYS_SetClockReset(ehci_pdata[id].ahb_usb20, true);
	udelay(0x20);

	/* # set aclk sw reset */
	SYS_SetClockReset(ehci_pdata[id].axi_usb20, true);
	udelay(0x20);

	/* # set aclk sw clear */
	SYS_SetClockReset(ehci_pdata[id].axi_usb20, false);
	udelay(0x20);

	/* # set hclk sw clear */
	SYS_SetClockReset(ehci_pdata[id].ahb_usb20, false);
	udelay(0x20);

	/* # set uclk sw clear */
	SYS_SetClockReset(ehci_pdata[id].core_usb20, false);
	udelay(0x20);

	/* #set UCLK to PHY 30M */
	SYS_CLK_SetClockSource(ehci_pdata[id].clk_src, 1);
}

/* configure so an HC device and id are always provided */
/* always called with process context; sleeping is OK */

/**
 * usb_ehci_NT72668_probe - initialize NT72668-based HCDs
 * Context: !in_interrupt()
 *
 * Allocates basic resources for this USB host controller, and
 * then invokes the start() method for the HCD associated with it
 * through the hotplug entry's driver_data.
 *
 */
static int usb_ehci_NT72668_probe(const struct hc_driver *driver,
			struct usb_hcd **hcd_out,
			struct platform_device *dev)
{
	struct device_node *dn = dev->dev.of_node;
	struct usb_hcd *hcd;
	struct ehci_hcd *ehci;
	struct resource res;
	int irq;
	int retval;

	retval = of_address_to_resource(dn, 0, &res);
	if (retval) {
		dev_err(&dev->dev, "no address resource provided for index 0");
		return retval;
	}

	/*
	* Right now device-tree probed devices don't get dma_mask set.
	* Since shared usb code relies on it, set it here for now.
	* Once we have dma capability bindings this can go away.
	*/
	if (!dev->dev.dma_mask)
		dev->dev.dma_mask = &dev->dev.coherent_dma_mask;
	if (!dev->dev.coherent_dma_mask)
		dev->dev.coherent_dma_mask = DMA_BIT_MASK(32);

	hcd = usb_create_hcd(driver, &dev->dev, "NT72668");
	if (!hcd)
		return -ENOMEM;

	hcd->rsrc_start = res.start;
	hcd->rsrc_len = resource_size(&res);

	pr_info("[USB] " NVT_USB_MODIFICATION_TIME
		NVT_USB_MODIFICATION_VERSION " pdev : %s\n", dev->name);

	hcd->regs = devm_ioremap_resource(&dev->dev, &res);
	if (!hcd->regs) {
		pr_debug("ioremap failed");
		retval = -ENOMEM;
		goto err1;
	}

	ehci = hcd_to_ehci(hcd);
	ehci->caps = hcd->regs;
	ehci->regs = (void __iomem *)ehci->caps +
	HC_LENGTH(ehci, readl(&ehci->caps->hc_capbase));

	ehci->sbrn = HCD_USB2;

	/* cache this readonly data; minimize chip reads */
	ehci->hcs_params = ehci_readl(ehci, &ehci->caps->hcs_params);

	irq = irq_of_parse_and_map(dn, 0);
	if (!irq) {
		dev_err(&dev->dev, "could not get usb irq\n");
		retval = -EBUSY;
		goto err1;
	}

	/* get usb phy by device tree node */
	hcd->usb_phy = devm_usb_get_phy_by_phandle(&dev->dev, "usb2phy", 0);
	if (IS_ERR(hcd->phy)) {
		dev_err(&dev->dev, "could not get usb2phy structure, err %ld\n",
				PTR_ERR(up));
		goto err1;
	}

	NT72668_init(dev);

	retval = usb_add_hcd(hcd, irq, IRQF_SHARED);
	if (retval == 0) {
		platform_set_drvdata(dev, hcd);
		usb_phy_vbus_on(hcd->usb_phy);
		return retval;
	}

	NT72668_stop(dev);

	irq_dispose_mapping(irq);
err1:
	usb_remove_hcd(hcd);
	return retval;
}

/* may be called without controller electrically present */
/* may be called with controller, bus, and devices active */

static void usb_ehci_NT72668_remove(struct usb_hcd *hcd,
			struct platform_device *dev)
{
	usb_remove_hcd(hcd);
	irq_dispose_mapping(hcd->irq);
	pr_debug("calling usb_put_hcd\n");
	usb_put_hcd(hcd);
	NT72668_stop(dev);
}

static int NT72668_ehci_init(struct usb_hcd *hcd)
{
	struct ehci_hcd *ehci = hcd_to_ehci(hcd);
	u32 temp;
	int retval;
	u32 hcc_params;
	struct ehci_qh_hw   *hw;

	spin_lock_init(&ehci->lock);

	ehci->need_io_watchdog = 1;

	hrtimer_init(&ehci->hrtimer, CLOCK_MONOTONIC, HRTIMER_MODE_ABS);
	ehci->hrtimer.function = ehci_hrtimer_func;
	ehci->next_hrtimer_event = EHCI_HRTIMER_NO_EVENT;

	hcc_params = ehci_readl(ehci, &ehci->caps->hcc_params);

	ehci->uframe_periodic_max = 100;

	/*
	* hw default: 1K periodic list heads, one per frame.
	* periodic_size can shrink by USBCMD update if hcc_params allows.
	*/
	ehci->periodic_size = DEFAULT_I_TDPS;

	INIT_LIST_HEAD(&ehci->async_unlink);
	INIT_LIST_HEAD(&ehci->async_idle);
	INIT_LIST_HEAD(&ehci->intr_unlink_wait);
	INIT_LIST_HEAD(&ehci->intr_unlink);
	INIT_LIST_HEAD(&ehci->intr_qh_list);
	INIT_LIST_HEAD(&ehci->cached_itd_list);
	INIT_LIST_HEAD(&ehci->cached_sitd_list);
	INIT_LIST_HEAD(&ehci->tt_list);

	retval = ehci_mem_init(ehci, GFP_KERNEL);
	if (retval < 0)
		return retval;

	/* controllers may cache some of the periodic schedule ... */
	hcc_params = ehci_readl(ehci, &ehci->caps->hcc_params);
	if (HCC_ISOC_CACHE(hcc_params))     /*  full frame cache */
		ehci->i_thresh = 8;
	else                    /*  N microframes cached */
		ehci->i_thresh = 2 + HCC_ISOC_THRES(hcc_params);

	/*
	* dedicate a qh for the async ring head, since we couldn't unlink
	* a 'real' qh without stopping the async schedule [4.8].  use it
	* as the 'reclamation list head' too.
	* its dummy is used in hw_alt_next of many tds, to prevent the qh
	* from automatically advancing to the next td after short reads.
	*/
	ehci->async->qh_next.qh = NULL;
	hw = ehci->async->hw;
	hw->hw_next = QH_NEXT(ehci, ehci->async->qh_dma);
	hw->hw_info1 = cpu_to_hc32(ehci, QH_HEAD);
	hw->hw_token = cpu_to_hc32(ehci, QTD_STS_HALT);
	hw->hw_qtd_next = EHCI_LIST_END(ehci);
	ehci->async->qh_state = QH_STATE_LINKED;
	hw->hw_alt_next = QTD_NEXT(ehci, ehci->async->dummy->qtd_dma);

	/* clear interrupt enables, set irq latency */
	if (log2_irq_thresh < 0 || log2_irq_thresh > 6)
		log2_irq_thresh = 0;
	temp = 1 << (16 + log2_irq_thresh);
	ehci->has_ppcd = 0;
	if (HCC_CANPARK(hcc_params)) {
		/* HW default park == 3, on hardware that supports it (like
		* NVidia and ALI silicon), maximizes throughput on the async
		* schedule by avoiding QH fetches between transfers.
		*
		* With fast usb storage devices and NForce2, "park" seems to
		* make problems:  throughput reduction (!), data errors...
		*/
		if (park) {
			park = min_t(unsigned, park, (unsigned) 3);
			temp |= CMD_PARK;
			temp |= park << 8;
		}
	}
	if (HCC_PGM_FRAMELISTLEN(hcc_params)) {
		/* periodic schedule size can be smaller than default */
		temp &= ~(3 << 2);
		temp |= (EHCI_TUNE_FLS << 2);
		switch (EHCI_TUNE_FLS) {
		case 0:
			ehci->periodic_size = 1024; break;
		case 1:
			ehci->periodic_size = 512; break;
		case 2:
			ehci->periodic_size = 256; break;
		default:
			BUG();
		}
	}
	ehci->command = temp;
	hcd->has_tt = 1;
	hcd->self.sg_tablesize = 0;
	return 0;
}


static void NT72668_patch(struct usb_hcd *hcd)
{
	struct ehci_hcd *ehci = hcd_to_ehci(hcd);
	unsigned int    command = ehci_readl(ehci, &ehci->regs->command);
	int retval, retry = 0;

	command |= CMD_RESET;
	ehci_writel(ehci, command, &ehci->regs->command);
	do {
		retval = ehci_handshake(ehci, &ehci->regs->command,
		    CMD_RESET, 0, 250 * 1000);
		retry++;
	} while (retval && (retry < 3));

	if (unlikely(retval != 0 && retry >= 3))
		ehci_err(ehci, "reset fail!\n");

	command = ehci->command;

	ehci_writel(ehci, (command &
			~((unsigned int)(CMD_RUN|CMD_PSE|CMD_ASE))),
			&ehci->regs->command);
	ehci_writel(ehci, ehci->periodic_dma, &ehci->regs->frame_list);
	ehci_writel(ehci, (u32)ehci->async->qh_dma, &ehci->regs->async_next);
	retry = 0;
	do {
		ehci_writel(ehci, INTR_MASK,
		&ehci->regs->intr_enable);
		retval = ehci_handshake(ehci, &ehci->regs->intr_enable,
		    INTR_MASK, INTR_MASK, 250);
		retry++;
	} while (retval != 0 && (retry < 3));
	if (unlikely(retval != 0))
		ehci_err(ehci, "write fail!\n");
	ehci->command &= ~((unsigned int)(CMD_PSE|CMD_ASE));
	set_bit(1, &hcd->porcd);
}

static const struct hc_driver ehci_NT72668_hc_driver = {
	.description =      hcd_name,
	.product_desc =     "Novatek EHCI",
	.hcd_priv_size =    sizeof(struct ehci_hcd),

/*
* generic hardware linkage
*/
	.irq =              ehci_irq,
	.flags =            HCD_MEMORY | HCD_USB2 | HCD_BH,

/*
* basic lifecycle operations
*/
	.reset =            NT72668_ehci_init,
	.start =            ehci_run,
	.stop =             ehci_stop,
	.shutdown =         ehci_shutdown,

/*
* managing i/o requests and associated device resources
*/
	.urb_enqueue =      ehci_urb_enqueue,
	.urb_dequeue =      ehci_urb_dequeue,
	.endpoint_disable = ehci_endpoint_disable,
	.endpoint_reset =   ehci_endpoint_reset,

/*
* scheduling support
*/
	.get_frame_number = ehci_get_frame,

/*
* root hub support
*/
	.hub_status_data =  ehci_hub_status_data,
	.hub_control =      ehci_hub_control,
	.bus_suspend =      ehci_bus_suspend,
	.bus_resume =       ehci_bus_resume,
	.relinquish_port =  ehci_relinquish_port,
	.port_handed_over = ehci_port_handed_over,

	.clear_tt_buffer_complete = ehci_clear_tt_buffer_complete,
	.port_nc = NT72668_patch,
};

static int ehci_hcd_NT72668_drv_probe(struct platform_device *pdev)
{
	struct usb_hcd *hcd = NULL;
	int ret;

	pr_debug("In ehci_hcd_NT72668_drv_probe\n");

	if (usb_disabled())
		return -ENODEV;

	ret = usb_ehci_NT72668_probe(&ehci_NT72668_hc_driver, &hcd, pdev);
	return ret;
}

static int ehci_hcd_NT72668_drv_remove(struct platform_device *pdev)
{
	struct usb_hcd *hcd = platform_get_drvdata(pdev);

	pr_debug("In ehci_hcd_NT72668_drv_remove\n");
	usb_ehci_NT72668_remove(hcd, pdev);
	platform_set_drvdata(pdev, NULL);

	return 0;
}

#ifdef CONFIG_PM
static int NT72668_ehci_suspend(struct device *dev)
{
	struct usb_hcd *hcd = dev_get_drvdata(dev);
	bool do_wakeup = device_may_wakeup(dev);
	struct platform_device *pdev = to_platform_device(dev);
	int rc;

	rc = ehci_suspend(hcd, do_wakeup);
	disable_irq(hcd->irq);
	NT72668_stop(pdev);

	return rc;
}

static int NT72668_ehci_resume(struct device *dev)
{
	struct usb_hcd *hcd = dev_get_drvdata(dev);
	struct platform_device *pdev = to_platform_device(dev);

	usb_phy_vbus_on(hcd->usb_phy);
	NT72668_init(pdev);
	NT72668_Plat_Resume(hcd);
	ehci_resume(hcd, false);
	enable_irq(hcd->irq);
	return 0;
}
#else
#define NT72668_ehci_suspend    NULL
#define NT72668_ehci_resume     NULL
#endif

static const struct dev_pm_ops NT72668_ehci_pm_ops = {
	.suspend    = NT72668_ehci_suspend,
	.resume     = NT72668_ehci_resume,
};

#ifdef CONFIG_OF
static const struct of_device_id NT72668_ehci_match[] = {
	{ .compatible = "nvt,NT72668-ehci" },
	{},
};

MODULE_DEVICE_TABLE(of, NT72668_ehci_match);
#endif

MODULE_ALIAS("NT72668-ehci");
static struct platform_driver ehci_hcd_NT72668_driver = {
	.probe = ehci_hcd_NT72668_drv_probe,
	.remove = ehci_hcd_NT72668_drv_remove,
	.driver = {
		.name = "NT72668-ehci",
		.pm = &NT72668_ehci_pm_ops,
		.of_match_table = of_match_ptr(NT72668_ehci_match),
	}
};

