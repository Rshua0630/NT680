/*!
 * @file clk.h
 *
 */
#ifndef __NVT_CLK_H__
#define __NVT_CLK_H__

enum MPLL_OFF_e {
	EN_MPLL_OFF_ARM		= 0x62,	/* ARM */
	EN_MPLL_OFF_AHB		= 0x9d,	/* AHB */
	EN_MPLL_OFF_AXI		= 0x6a,	/* AXI */
	EN_MPLL_OFF_ETH		= 0xaa,	/* Ethernet */
	EN_MPLL_OFF_USB_20	= 0x8a,	/* USB 2.0 */
	EN_MPLL_OFF_USB_30	= 0x15,	/* USB 3.0 */
	EN_MPLL_OFF_EMMC	= 0xc4,	/* EMMC */
	EN_MPLL_OFF_DDR		= 0x82,	/* DDR */
};

/**
 * @ingroup type
 *
 * @defgroup Inv Emueration of clock inverse
 * @{
 *
 * @var EN_SYS_CLK_INV
 *
 * The enumeration of clock inverse
 *
 * @hideinitializer
 */
enum EN_SYS_CLK_INV {
	/* Engine clock select (0x44) */
	/* bit[6]  */
	EN_SYS_CLK_INV_MAC0			= ((0x44 << 3) + 6),
	/* bit[13] */
	EN_SYS_CLK_INV_MAC1			= ((0x44 << 3) + 13),
	/* bit[14] */
	EN_SYS_CLK_INV_MAC_OUT,
};

/**
 * @ingroup type
 *
 * @defgroup Src Emueration of clock source
 * @{
 *
 * @var EN_SYS_CLK_SRC
 *
 * The enumeration of clock inverse
 *
 * @hideinitializer
 */
enum EN_SYS_CLK_SRC {
	/* System clock select (0x30) */
	EN_SYS_CLK_SRC_AHB = 0,				/* bit[6:5] */
	EN_SYS_CLK_SRC_AXI,					/* bit[9:8] */

	/* Engine clock select (0x38) */
	EN_SYS_CLK_SRC_USB_20PHY_12M,		/* bit[5] */

	/* Engine clock select (0x48) */
	EN_SYS_CLK_SRC_USB_U2_U3_30M,		/* bit[22] */
	EN_SYS_CLK_SRC_USB_U0_U1_30M,		/* bit[27] */
};

/**
 * @internal
 * @ingroup type
 *
 * @defgroup AHBPLL Enumeration of AHB clock clock source
 * @{
 *
 * @typedef EN_SYS_AHB_CLK_SRC
 *
 * A enumeration for AHB clock source select
 */
enum EN_SYS_AHB_CLK_SRC {
	/* AHB clock source select */
	/* 2'b00: refserence clock 96M */
	EN_SYS_AHB_CLK_SRC_REF_96M	= 0x0,
	/* 2'b01: ARM PLL div 8 */
	EN_SYS_AHB_CLK_SRC_ARM_D8	= 0x1,
	/* 2'b1x: AHB PLL */
	EN_SYS_AHB_CLK_SRC_AHB		= 0x2,
};

/**
 * @internal
 * @ingroup type
 *
 * @defgroup AXIPLL Enumeration of AXI clock clock source
 * @{
 *
 * @typedef EN_SYS_AXI_CLK_SRC
 *
 * A enumeration for AXI clock source select
 */
enum EN_SYS_AXI_CLK_SRC {
	/* AHB clock source select */
	EN_SYS_AXI_CLK_SRC_ARM_D8	= 0x0,	/* 2'b00: ARM PLL div 8 */
	EN_SYS_AXI_CLK_SRC_DDR_D2	= 0x1,	/* 2'b01: DDR PLL div 2 */
	EN_SYS_AXI_CLK_SRC_AXI_D2	= 0x2,	/* 2'b1x: AXI PLL div 2 */
	EN_SYS_AXI_CLK_SRC_AXI		= 0x3,	/* 2'b1x: AXI PLL */
};

/**
 * @ingroup type
 *
 * @defgroup RST Emueration of clock reset
 * @{
 *
 * @var EN_SYS_CLK_RST
 *
 * The enumeration of clock reset
 *
 * @hideinitializer
 */
enum EN_SYS_CLK_RST {
	/* Parition A, core clock reset disable/enable (0x50/0x54) */
	EN_SYS_CLK_RST_CORE_USB20_U0,			/* bit[8] */
	EN_SYS_CLK_RST_CORE_USB20_U1,			/* bit[9] */

	/* Partition C, core clock reset disable/enable (0x60/0x64) */
	EN_SYS_CLK_RST_CORE_USB20_U2,			/* bit[4] */
	EN_SYS_CLK_RST_CORE_USB20_U3,			/* bit[5] */

	/* Partition A, AXI clock reset disable/enable (0x70/0x74) */
	EN_SYS_CLK_RST_AXI_USB20_U0,			/* bit[6] */
	EN_SYS_CLK_RST_AXI_USB20_U1,			/* bit[7] */

	/* Partition C, core clock reset disable/enable (0x80/0x84) */
	EN_SYS_CLK_RST_AXI_USB20_U2,			/* bit[2] */
	EN_SYS_CLK_RST_AXI_USB20_U3,			/* bit[3] */

	/* Partition A, AHB clock reset disable/enable (0x90/0x94) */
	EN_SYS_CLK_RST_AHB_USB20_U0,			/* bit[8] */
	EN_SYS_CLK_RST_AHB_USB20_U0_PCLK,		/* bit[9] */
	EN_SYS_CLK_RST_AHB_USB20_U1,			/* bit[10] */
	EN_SYS_CLK_RST_AHB_USB20_U1_PCLK,		/* bit[11] */

	/* Partition C, AHB clock reset disable/enable (0xA0/0xA4) */
	EN_SYS_CLK_RST_AHB_USB20_U2,			/* bit[10] */
	EN_SYS_CLK_RST_AHB_USB20_U3,			/* bit[11] */
	EN_SYS_CLK_RST_AHB_USB20_U2_PCLK,		/* bit[12] */
	EN_SYS_CLK_RST_AHB_USB20_U3_PCLK,		/* bit[13] */
};

/**
 * @ingroup type
 *
 * @defgroup MASK Emueration of clock mask
 * @{
 *
 * @var EN_SYS_CLK_MASK
 *
 * The enumeration of clock mask
 *
 * @hideinitializer
 */
enum EN_SYS_CLK_MASK {
	/* Partition A, AHB clock mask (0x00) */
	/* bit[7] */
	EN_SYS_CLK_MASK_AHB_USB20_U0		= ((0x00 << 3) + 7),
	/* bit[8] */
	EN_SYS_CLK_MASK_AHB_USB20_U1,

	/* Partition C, AHB clock mask (0x08) */
	/* bit[10] */
	EN_SYS_CLK_MASK_AHB_USB20_U2		= ((0x08 << 3) + 10),
	/* bit[11] */
	EN_SYS_CLK_MASK_AHB_USB20_U3,

	/* Partition A, AXI clock mask (0x10) */
	/* bit[5] */
	EN_SYS_CLK_MASK_AXI_USB20_U0		= ((0x10 << 3) + 5),
	/* bit[6] */
	EN_SYS_CLK_MASK_AXI_USB20_U1,

	/* Partition C, AXI clock mask (0x18) */
	/* bit[8] */
	EN_SYS_CLK_MASK_AXI_USB20_U2		= ((0x18 << 3) + 8),
	/* bit[9] */
	EN_SYS_CLK_MASK_AXI_USB20_U3,

	/* Partition A, engine clock mask (0x20) */
	/* bit[12] */
	EN_SYS_CLK_MASK_CORE_USB20_A_12M	= ((0x20 << 3) + 12),
	/* bit[13] */
	EN_SYS_CLK_MASK_CORE_USB20_U0_30M,
	/* bit[14] */
	EN_SYS_CLK_MASK_CORE_USB20_U1_30M,

	/* Partition C, engine clock mask (0x28) */
	/* bit[3] */
	EN_SYS_CLK_MASK_CORE_USB20_C_12M	= ((0x28 << 3) + 3),
	/* bit[4] */
	EN_SYS_CLK_MASK_CORE_USB20_U2_30M,
	/* bit[5] */
	EN_SYS_CLK_MASK_CORE_USB20_U3_30M,
};

/**
 * @fn void SYS_CLK_SetClockSource(EN_SYS_CLK_SRC enSrc, unsigned long Src)
 *
 * @brief  Set clock source for specific top or engine
 *
 * @param[in]  enSrc	Indicate the clock source which will be changed.
 * @param[in]  u32Src	New clock source value
 *
 */
void SYS_CLK_SetClockSource(enum EN_SYS_CLK_SRC enSrc, unsigned long Src);

/**
 * @fn void SYS_SetClockReset(EN_SYS_CLK_RST enRst, bool b8EnRst)
 *
 * @brief Clock reset enable/disable
 *
 * @param[in] enRst		Clock reset which will be enable or disable
 * @param[in] b8EnRst	TRUE: enable clock reset, FALSE: disable clock reset
 *
 */
void SYS_SetClockReset(enum EN_SYS_CLK_RST enRst, bool b8EnRst);

/**
 * @fn void SYS_CLK_SetClockInv(EN_SYS_CLK_INV enInv, bool b8EnInv)
 *
 * @brief Clock inverse enable/disable
 *
 * @param[in] enInv		Clock inverse which will be enable or disable
 * @param[in] b8EnInv	TRUE: clock inverse enable, FALSE clock inversse disable
 *
 */
void SYS_SetClockInv(enum EN_SYS_CLK_INV enInv, bool b8EnInv);

/**
 * @fn void SYS_SetClockMask(EN_SYS_CLK_MASK enMask, bool b8EnMask)
 *
 * @brief Clock mask enable/disable
 *
 * @param[in] enMask	Clock mask which will be enable or disable
 * @param[in] b8EnMask	TRUE: clock mask enable, FALSE clock mask disable
 *
 */
void SYS_SetClockMask(enum EN_SYS_CLK_MASK enMask, bool b8EnMask);

/**
 * @brief Get System PLL by Offset
 *
 * @param [in] off	Offset
 * @return Frequency of PLL (MHz)
 */
unsigned long SYS_CLK_GetMpll(unsigned long off);

/**
 * @brief Set Value to System PLL by Offset
 *
 * @param [in] off	Offset
 * @param [in] freq	Frequency (MHz)
 */
unsigned long SYS_CLK_SetMpll(unsigned long off, unsigned long freq);

/**
 * Get frequency of AHB clock
 */
unsigned long get_ahb_clk(void);

/**
 * Get frequency of CPU clock
 */
unsigned long get_cpu_clk(void);

/**
 * Get frequency of EMMC clock
 */
unsigned long get_mmc_clk(void);

/**
 * Get frequency of AXI clock
 */
unsigned long get_axi_clk(void);

/**
 * Get frequency of Peripheral clock
 */
unsigned long get_periph_clk(void);

extern void __iomem *clk_reg_base;
#endif /* __NVT_CLK_H__ */

