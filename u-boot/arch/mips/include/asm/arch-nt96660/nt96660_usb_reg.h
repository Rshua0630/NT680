/*
 *  include/asm/arch-nt96660/nt96660_usb_reg.h
 *
 *  Author:	Alvin lin
 *  Created:	Nov 10, 2011
 *  Copyright:	Novatek Inc.
 *
 */
#ifndef __NT96660_USB_REGS_H__
#define __NT96660_USB_REGS_H__

#include "hardware.h"

//CORE CLOCK REG
#define REG_CORE_CLK0_RESET_DIS		__REG(0xBD020060)
#define REG_CORE_CLK0_RESET_EN		__REG(0XBD020064)

#define USB0_SW_CORE_CLK_RST			(1 << 5)
#define USB1_SW_CORE_CLK_RST			(1 << 6)

//AHB CLOCK REG
#define REG_AHB_CLK0_RESET_DIS		__REG(0xBD0200A0)
#define REG_AHB_CLK0_RESET_EN			__REG(0xBD0200A4)

#define USB0_AHB_CLK_RST				(1 << 1)
#define USB1_AHB_CLK_RST				(1 << 2)

//AXI CLOCK REG
#define REG_AXI_CLK0_RESET_DIS			__REG(0xBD020080)
#define REG_AXI_CLK0_RESET_EN			__REG(0xBD020084)

#define USB0_AXI_CLK_RST				(1 << 2)
#define USB1_AXI_CLK_RST				(1 << 3)

//USB0 register
#define REG_USB0_CFG					__REG(0xBD110804)
#define REG_USB1_CFG					__REG(0xBD110808)

#define USB_CFG_PWR_MASK				7
#define USB_CFG_PWR_27_5_MV			0x0
#define USB_CFG_PWR_137_5_MV			0x4
#define USB_CFG_PWR_220_MV			0x7

#define USB_CFG_BIST_T2R				(1 << 3)
#define USB_CFG_PD_VDT					(1 << 4)
#define USB_CFG_S1						(1 << 5)
#define USB_CFG_MAHB_SEL				(1 << 7)
#define USB_CFG_HD_MASK				7
#define USB_CFG_HD_SHIFT				8
#define USB_CFG_HD_LEVEL0				0
#define USB_CFG_HD_LEVEL1				1
#define USB_CFG_HD_LEVEL2				2
#define USB_CFG_HD_LEVEL3				3
#define USB_CFG_HD_LEVEL4				4
#define USB_CFG_HD_LEVEL5				5
#define USB_CFG_HD_LEVEL6				6
#define USB_CFG_HD_LEVEL7				7
#define USB_CFG_PONRST					(1 << 11)
#define USB_CFG_MASK_SUSPEND			(1 << 12)
#define USB_CFG_MASK_RESET			(1 << 13)

#define USB0_CFG_HOST_DISTCONN_LVL	4

//ENGINE CLOCK CFG REG
#define REG_ENG_CLK_CFG				__REG(0xBD020048)
#define USB_ENG_CLK_30M				(1 << 27)

//USB EHCI REGs
#define USB0_EHCI_BASE					0xB0600000
#define USB1_EHCI_BASE					0xB0600000

#define REG_USB_HC_CAP(BASE)				__REG((BASE + 0x000))
#define REG_USB_HC_STRUCT_PARAM(BASE)	__REG((BASE + 0x004))
#define REG_USB_HC_CAP_PARAM(BASE)		__REG((BASE + 0x008))
#define REG_USB_HC_CMD(BASE)				__REG((BASE + 0x010))
#define REG_USB_HC_INT_STS(BASE)			__REG((BASE + 0x014))
#define REG_USB_HC_INT_EN(BASE)			__REG((BASE + 0x018))
#define REG_USB_HC_FRAME_IDX(BASE)		__REG((BASE + 0x01C))
#define REG_USB_HC_PERI_FRAME_BASE(BASE)	__REG((BASE + 0x024))
#define REG_USB_HC_ASYNC_ADDR(BASE)		__REG((BASE + 0x028))
#define REG_USB_HC_PORTSTS_CTRL(BASE)		__REG((BASE + 0x030))
#define REG_USB_HC_MISC(BASE)				__REG((BASE + 0x040))
#define REG_USB_OTG_CTRL_STS(BASE)		__REG((BASE + 0x080))
#define REG_USB_OTG_INT_STS(BASE)			__REG((BASE + 0x084))
#define REG_USB_OTG_INT_EN(BASE)			__REG((BASE + 0x088))
#define REG_USB_GLOBAL_INT_STS0(BASE)		__REG((BASE + 0x0C0))
#define REG_USB_GLOBAL_INT_STS1(BASE)		__REG((BASE + 0x0C4))
#define REG_USB_OTG_SYS_CFG(BASE)			__REG((BASE + 0x0E0))
#define REG_USB_PHY_SOF(BASE)				__REG((BASE + 0x0E4))
#define REG_USB_DEV_MAIN_CTRL(BASE)		__REG((BASE + 0x100))
#define REG_USB_DEV_ADDR(BASE)			__REG((BASE + 0x104))
#define REG_USB_DEV_TST(BASE)				__REG((BASE + 0x108))
#define REG_USB_SOF_NUM(BASE)				__REG((BASE + 0x10C))
#define REG_USB_SOF_TMR(BASE)				__REG((BASE + 0x110))
#define REG_USB_PHY_TST(BASE)				__REG((BASE + 0x114))
#define REG_USB_VEND_IO0(BASE)				__REG((BASE + 0x118))
#define REG_USB_VEND_IO1(BASE)				__REG((BASE + 0x11C))
#define REG_USB_CXCFG_FIFO_STS(BASE)		__REG((BASE + 0x120))
#define REG_USB_IDLE_CNT(BASE)				__REG((BASE + 0x124))


//REG_USB0_DEV_MAIN_CTRL
#define USB_CTRL_CAP_RMWAKEP				(1 << 0)
#define USB_CTRL_HALF_SPEED				(1 << 1)
#define USB_CTRL_GLINT_EN					(1 << 2)
#define USB_CTRL_GOSUSP					(1 << 3)
#define USB_CTRL_SFRST						(1 << 4)
#define USB_CTRL_CHIP_EN					(1 << 5)
#define USB_CTRL_HS_EN						(1 << 6)
#define USB_CTRL_RISC51_INF				(1 << 7)

//REG_USB0_OTG_SYS_CFG
#define USB_OTG_SYS_BDEV					(1 << 0)
#define USB_OTG_SYS_VBUS_VLD				(1 << 1)
#define USB_OTG_SYS_A_SESS_VLD			(1 << 2)
#define USB_OTG_SYS_B_SESS_VLD			(1 << 3)
#define USB_OTG_SYS_B_SESS_END			(1 << 4)
#define USB_OTG_SYS_AXI_REQ_MSK			3
#define USB_OTG_SYS_AXI_REQ_SFT			5
#define USB_OTG_SYS_AXI_REQ_8WRD			0
#define USB_OTG_SYS_AXI_REQ_4WRD			1
#define USB_OTG_SYS_RMOTE_WAKEUP		(1 << 7)
#define USB_OTG_SYS_CDELAY_MASK			0x3F
#define USB_OTG_SYS_CDELAY_SFT			16
#define USB_OTG_SYS_CINV					(1 << 23)
#define USB_OTG_SYS_AXI_BUSID_MSK		0xFF
#define USB_OTG_SYS_AXI_BUSID_SFT			24

#define USB0_OTG_AXI_BUS_ID				0x2
#define USB1_OTG_AXI_BUS_ID				0x13

//REG_USB0_GLOBAL_INT_STS1
#define USB_GLB1_MDEV_INT					(1 << 0)
#define USB_GLB1_MOTG_INT					(1 << 1)
#define USB_GLB1_MHC_INT					(1 << 2)
#define USB_GLB1_INT_POLAR					(1<< 3)

//REG_USB0_OTG_INT_EN
#define USB_OTG_INT_B_SRP_DN				(1 << 0)
#define USB_OTG_INT_A_SRP_DET				(1 << 4)
#define USB_OTG_INT_A_VBUS_ERR			(1 << 5)
#define USB_OTG_INT_B_SESS_END			(1 << 6)
#define USB_OTG_INT_RLCHG					(1 << 8)
#define USB_OTG_INT_IDCHG					(1 << 9)
#define USB_OTG_INT_OVC					(1 << 10)
#define USB_OTG_INT_BPLGRMV				(1 << 11)
#define USB_OTG_INT_APLGRMV				(1 << 12)

//REG_USB0_OTG_CTRL_STS
#define USB_OTG_CTRL_STS_B_BUS_REG		(1 << 0)
#define USB_OTG_CTRL_STS_B_HNP			(1 << 1)
#define USB_OTG_CTRL_STS_B_DSC_BUS		(1 << 2)
#define USB_OTG_CTRL_STS_A_BUS_REG		(1 << 4)
#define USB_OTG_CTRL_STS_A_BUS_DROP		(1 << 5)
#define USB_OTG_CTRL_STS_A_SET_BHNP		(1 << 6)
#define USB_OTG_CTRL_STS_A_SRP_DET		(1 << 7)
#define USB_OTG_CTRL_STS_ASRP_RESP		(1 << 8)
#define USB_OTG_CTRL_STS_ID_FLT			(1 << 9)
#define USB_OTG_CTRL_STS_VBUS_FLT			(1 << 10)
#define USB_OTG_CTRL_STS_HDISC_FLT		(1 << 11)
#define USB_OTG_CTRL_STS_CROLE_DEV		(1 << 20)
#define USB_OTG_CTRL_HST_SPD_MASK		3
#define USB_OTG_CTRL_STS_SPD_SFT			22
#define USB_OTG_CTRL_STS_FS				0x0
#define USB_OTG_CTRL_STS_LS				0x1
#define USB_OTG_CTRL_STS_HS				0x2

//REG_USB_HC_CMD
#define USB_HC_CMD_RUN						(1 << 0)
#define USB_HC_CMD_RESET					(1 << 1)
#define USB_HC_CMD_FRLSZ_MASK				0x3
#define USB_HC_CMS_FRLSZ_SFT				2
#define USB_HC_CMD_FRLSZ_1024				0
#define USB_HC_CMD_FRLSZ_512				1
#define USB_HC_CMD_FRLSZ_256				2
#define USB_HC_CMD_PSCH_EN				(1 << 4)
#define USB_HC_CMD_ASCH_EN				(1 << 5)
#define USB_HC_CMD_INT_OAAD				(1 << 6)
#define USB_HC_CMD_ASYN_PK_CNT_MASK		0x3
#define USB_HC_CMD_ASYN_PK_CNT_SFT		8
#define USB_HC_CMD_ASYN_PK_EN			(1 << 11)
#define USB_HC_CMD_INT_THRC_MASK			0xFF
#define USB_HC_CMD_INT_THRC_SFT			16
#define USB_HC_CMD_INT_THRC_1FR			0x1
#define USB_HC_CMD_INT_THRC_2FR			0x2
#define USB_HC_CMD_INT_THRC_4FR			0x4
#define USB_HC_CMD_INT_THRC_8FR			0x8
#define USB_HC_CMD_INT_THRC_16FR			0x10
#define USB_HC_CMD_INT_THRC_32FR			0x20
#define USB_HC_CMD_INT_THRC_64FR			0x40


#endif

