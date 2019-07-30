
#ifdef CONFIG_USB_NVT_EHCI_HCD
struct nvt_u2_phy {
	void __iomem *apb_regs;
	void __iomem *phy_regs;
	struct usb_phy u_phy;
	struct device *dev;
	int usb3_controller;
	u32 (*get_phy_reg)(struct nvt_u2_phy *pphy, u32 offset);
	u32 (*get_apb_reg)(struct nvt_u2_phy *pphy, u32 offset);
	void (*set_phy_reg)(struct nvt_u2_phy *pphy, u32 offset, u32 value);
	void (*set_apb_reg)(struct nvt_u2_phy *pphy, u32 offset, u32 value);
};

struct nvt_u3_phy {
	void __iomem *regs;
#ifdef CONFIG_NVT_72670_USBPHY
	void __iomem *apb_regs;
#endif
	void __iomem *hcd_regs;
	struct usb_phy u_phy;
	struct device *dev;
	u32	status;
	struct timer_list	phy_timer;
	u32 (*get_phy_page_reg)(struct nvt_u3_phy *pphy, int page, u32 offset);
	u32 (*get_apb_reg)(struct nvt_u3_phy *pphy, u32 offset);
	void (*set_phy_page_reg)(struct nvt_u3_phy *pphy,
		int page, u32 offset, u32 value);
	void (*set_apb_reg)(struct nvt_u3_phy *pphy, u32 offset, u32 value);
};
#endif

