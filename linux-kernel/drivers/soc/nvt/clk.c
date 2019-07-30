#include <linux/module.h>
#include <linux/spinlock.h>

#include <linux/of.h>
#include <linux/of_address.h>
#include <linux/of_fdt.h>
#include <linux/of_irq.h>
#include <linux/of_platform.h>
#include <linux/clk-provider.h>
#include <linux/clkdev.h>

#include <linux/types.h>

#include <linux/atomic.h>

#include <soc/nvt/clk.h>

static unsigned long arm_clk, ahb_clk, mmc_clk, axi_clk, ddr_clk;

void __iomem *clk_reg_base;
EXPORT_SYMBOL(clk_reg_base);
void __iomem *mpll_reg_base;
EXPORT_SYMBOL(mpll_reg_base);
void __iomem *apll_reg_base;

#define NVT_SYS_CLK_INFO(fmt, args...) pr_info(fmt, ## args)
#define NVT_SYS_CLK_ERR(fmt, args...) pr_err("clk " fmt, ## args)

#define CONFIG_NVT_SYS_CLK_DEBUG 0
#undef NVT_SYS_CLK_DBG
#if CONFIG_NVT_SYS_CLK_DEBUG
#define NVT_SYS_CLK_DBG(fmt, args...) pr_debug("clk " fmt, ## args)
#else
#define NVT_SYS_CLK_DBG(fmt, args...)
#endif

#define _AHB_CLK_SEL \
({	\
((readl((unsigned long *)((unsigned long)clk_reg_base + 0x30)) >> 5) & 0x3); \
})
#define _AXI_CLK_SEL \
({	\
((readl((unsigned long *)((unsigned long)clk_reg_base + 0x30)) >> 8) & 0x3); \
})
#define _PERI_CLK_SEL \
({	\
((readl((unsigned long *)((unsigned long)clk_reg_base + 0xf4)) >> 5) & 0x7); \
})

#define _MPLL_GetData(u8Offset) \
({	\
readl((unsigned long *)((unsigned long)mpll_reg_base + ((u8Offset) * 4))); \
})

#define _MPLL_SetData(u8Offset, u8Val) \
({	\
writel(u8Val, (unsigned long *)(	\
	(unsigned long)mpll_reg_base + (u8Offset * 4))); \
})

#define _APLL_GetData(u8Offset) \
({	\
readl((unsigned long *)((unsigned long)apll_reg_base + ((u8Offset) * 4))); \
})

#define _MPLL_EnablePageB() \
({	\
writel(0x1, (unsigned long *)((unsigned long)clk_reg_base + 0xbc)); \
})

#define IS_REAL_CHIP (_MPLL_GetData((EN_MPLL_OFF_ARM + 2)))

static void sys_set_bit(int nr, unsigned long *addr)
{
	unsigned long mask;

	addr += nr >> 5;
	mask = (unsigned long)(1 << (nr & 0x1f));
	*addr |= mask;
};

static void sys_clear_bit(int nr, unsigned long *addr)
{
	unsigned long mask;

	addr += nr >> 5;
	mask = (unsigned long)(1 << (nr & 0x1f));
	*addr &= ~mask;
};

DEFINE_RAW_SPINLOCK(clock_gen_lock);
EXPORT_SYMBOL(clock_gen_lock);

static void compute_mmc_clk(void);
static void compute_ahb_clk(void);
static void compute_axi_clk(void);
static void compute_cpu_clk(void);
static void compute_ddr_clk(void);

struct ST_SYS_CLK_RST {
	unsigned long  RegOff;
	unsigned long  Mask;
};

struct ST_SYS_CLK_SRC_SEL {
	unsigned long RegOff;
	unsigned long FieldSize;
	unsigned long FieldPos;
};

const struct ST_SYS_CLK_RST stSYSClkRst[] = {
	/* Parition A, core clock reset disable/enable (0x50/0x54) */
	/* EN_SYS_CLK_RST_CORE_USB20_U0,			bit[8] */
	{0x50,	(1UL << 8)},
	/* EN_SYS_CLK_RST_CORE_USB20_U1,			bit[9] */
	{0x50,	(1UL << 9)},

	/* Partition C, core clock reset disable/enable (0x60/0x64) */
	/* EN_SYS_CLK_RST_CORE_USB20_U2,			bit[4] */
	{0x60,	(1UL << 4)},
	/* EN_SYS_CLK_RST_CORE_USB20_U3,			bit[5] */
	{0x60,	(1UL << 5)},

	/* Partition A, AXI clock reset disable/enable (0x70/0x74) */
	/* EN_KER_CLK_RST_AXI_USB20_U0,			bit[6] */
	{0x70,	(1UL << 6)},
	/* EN_KER_CLK_RST_AXI_USB20_U1,			bit[7] */
	{0x70,	(1UL << 7)},

	/* Partition C, core clock reset disable/enable (0x80/0x84) */
	/* EN_SYS_CLK_RST_AXI_USB20_U2,			bit[2] */
	{0x80, (1UL << 2)},
	/* EN_SYS_CLK_RST_AXI_USB20_U3,			bit[3] */
	{0x80, (1UL << 3)},

	/* Parition A, AHB clock reset disable/enable (0x90/0x94) */
	/* EN_SYS_CLK_RST_AXI_USB20_U0,			bit[8]	*/
	{0x90, (1UL << 8)},
	/* EN_SYS_CLK_RST_AXI_USB20_U0_PCLK,	bit[9]	*/
	{0x90, (1UL << 9)},
	/* EN_SYS_CLK_RST_AXI_USB20_U1,			bit[10]	*/
	{0x90, (1UL << 10)},
	/* EN_SYS_CLK_RST_AXI_USB20_U1_PCLK,	bit[11]	*/
	{0x90, (1UL << 11)},

	/* Partition C, AHB clock reset disable/enable (0xA0/0xA4) */
	/* EN_SYS_CLK_RST_AHB_USB20_U2,			bit[10]	*/
	{0xA0, (1UL << 10)},
	/* EN_SYS_CLK_RST_AHB_USB20_U3,			bit[11] */
	{0xA0, (1UL << 11)},
	/* EN_SYS_CLK_RST_AHB_USB20_U2_PCLK,	bit[12] */
	{0xA0, (1UL << 12)},
	/* EN_SYS_CLK_RST_AHB_USB20_U3_PCLK,	bit[13] */
	{0xA0, (1UL << 13)},
};

const struct ST_SYS_CLK_SRC_SEL stKerClkSrcSel[] = {
	/* RegOff, FieldSize, FieldPos */
	/* Engine clock select (0x30) */
	/* EN_SYS_CLK_SRC_AHB,				bit[6:5] */
	{0x30,	2,	5},
	/* EN_SYS_CLK_SRC_AXI,				bit[9:8] */
	{0x30,	2,	8},

	/* Engine clock select (0x38) */
	/* EN_SYS_CLK_SRC_USB_20PHY_12M,	bit[6:5] */
	{0x38,	2,	5},

	/* Engine clock select (0x48) */
	/* EN_SYS_CLK_SRC_USB_U2_U3_30M,	bit[22]	*/
	{0x48,	1,	22},
	/* EN_SYS_CLK_SRC_USB_U0_U1_30M,	bit[27]	*/
	{0x48,	1,	27},
};

/**
 * @brief Get System PLL by Offset
 *
 * @param [in] off	Offset
 * @return Frequency of PLL (MHz)
 */
unsigned long SYS_CLK_GetMpll(unsigned long off)
{
	unsigned long retVal = 0;
	unsigned long flags;

#if CONFIG_NVT_SYS_CLK_DEBUG
	NVT_SYS_CLK_DBG("func %s\n", __func__);
#endif

	raw_spin_lock_irqsave(&clock_gen_lock, flags);

	/* Select page and set MPLL in crirical section */

	_MPLL_EnablePageB();

	retVal = (_MPLL_GetData(off));
	retVal |= (_MPLL_GetData(off + 1) << 8);
	retVal |= (_MPLL_GetData(off + 2) << 16);

	raw_spin_unlock_irqrestore(&clock_gen_lock, flags);

	retVal *= 12;
	retVal += ((1UL << 17) - 1);
	retVal >>= 17;

#if CONFIG_NVT_SYS_CLK_DEBUG
	NVT_SYS_CLK_DBG("\toffset 0x%02x\n", (int)off);
	NVT_SYS_CLK_DBG("\tVal[0]: 0x%08x\n",  _MPLL_GetData((off + 0)));
	NVT_SYS_CLK_DBG("\tVal[1]: 0x%08x\n",  _MPLL_GetData((off + 1)));
	NVT_SYS_CLK_DBG("\tVal[2]: 0x%08x\n",  _MPLL_GetData((off + 2)));
#endif

	return retVal;
}
EXPORT_SYMBOL(SYS_CLK_GetMpll);

/**
 * @brief Set Value to System PLL by Offset
 *
 * @param [in] off	Offset
 * @param [in] freq	Frequency (MHz)
 * @return Value of PLL
 */
unsigned long SYS_CLK_SetMpll(unsigned long off, unsigned long freq)
{
	unsigned long flags;

#if CONFIG_NVT_SYS_CLK_DEBUG
	NVT_SYS_CLK_DBG("Func %s\n", __func__);
#endif

	freq <<= 17;
	freq += (12 - 1);
	freq /= 12;

	raw_spin_lock_irqsave(&clock_gen_lock, flags);

	/* Select page and set MPLL in crirical section */
	_MPLL_EnablePageB();

	_MPLL_SetData(off + 0, ((freq >>  0) & 0xff));
	_MPLL_SetData(off + 1, ((freq >>  8) & 0xff));
	_MPLL_SetData(off + 2, ((freq >> 16) & 0xff));

	raw_spin_unlock_irqrestore(&clock_gen_lock, flags);
#if CONFIG_NVT_SYS_CLK_DEBUG
	NVT_SYS_CLK_DBG("\toffset 0x%02x, value 0x%08x\n", (int)off, (int)freq);
	NVT_SYS_CLK_DBG("\tVal[0]: 0x%08x\n", _MPLL_GetData((off + 0));
	NVT_SYS_CLK_DBG("\tVal[1]: 0x%08x\n", _MPLL_GetData((off + 1));
	NVT_SYS_CLK_DBG("\tVal[2]: 0x%08x\n", _MPLL_GetData((off + 2));
#endif

	return freq;
}
EXPORT_SYMBOL(SYS_CLK_SetMpll);

/**
 * Initialize clock
 *
 * Get base address of clkgen, MPLL and APLL
 * Get frequency of ARM, AHB and EMMC
 */
static void __init nvt_clk_init(struct device_node *node)
{
	arm_clk = 0;
	ahb_clk = 0;
	mmc_clk = 0;
	axi_clk = 0;

#if CONFIG_NVT_SYS_CLK_DEBUG
	NVT_SYS_CLK_DBG("Func %s\n", __func__);
	NVT_SYS_CLK_DBG("nvt clkgen initialization\n");
#endif

	if (node) {
		clk_reg_base = of_iomap(node, 0);
		mpll_reg_base = of_iomap(node, 1);
		apll_reg_base = of_iomap(node, 2);

		of_node_put(node);
	} else {
#if CONFIG_NVT_SYS_CLK_DEBUG
		NVT_SYS_CLK_DBG("can't find node\n");
#endif

		return;
	}

	_MPLL_EnablePageB();

	compute_cpu_clk();
	compute_ahb_clk();
	compute_axi_clk();
	compute_mmc_clk();
	compute_ddr_clk();

	pr_info("cpu clk = %4dMHz\n", (int)(arm_clk / 1000000));
	pr_info("ahb clk = %4dMHz\n", (int)(ahb_clk / 1000000));
	pr_info("axi clk = %4dMHz\n", (int)(axi_clk / 1000000));
	pr_info("mmc clk = %4dMHz\n", (int)(mmc_clk / 1000000));
	pr_info("ddr clk = %4dMHz\n", (int)(ddr_clk << 2));
}

CLK_OF_DECLARE(nvt72668_clkgen, "nvt,clkgen", nvt_clk_init);

static void __init nvt_periph_clk_init(struct device_node *node)
{
	struct clk *periph_clk;

	periph_clk = clk_register_fixed_rate(NULL,
			"periph_clk",
			NULL,
			CLK_IS_ROOT,
			get_periph_clk());
	clk_register_clkdev(periph_clk, NULL, "periph_clk_dev");
	of_clk_add_provider(node, of_clk_src_simple_get, periph_clk);
}

CLK_OF_DECLARE(nvt_periph_clk, "nvt,periph_clk", nvt_periph_clk_init);

static void __init nvt_ahb_clk_init(struct device_node *node)
{
	struct clk *ahb_clk;

	ahb_clk = clk_register_fixed_rate(NULL, "ahb_clk", NULL, CLK_IS_ROOT,
			get_ahb_clk());
	clk_register_clkdev(ahb_clk, NULL, "ahb_clk_dev");
	of_clk_add_provider(node, of_clk_src_simple_get, ahb_clk);
}

CLK_OF_DECLARE(nvt_ahb_clk, "nvt,ahb_clk", nvt_ahb_clk_init);

/**
 * Get frequency of peripheral clock
 */
unsigned long get_periph_clk(void)
{
	return (arm_clk/(_PERI_CLK_SEL + 1));
}

/**
 * Get frequency of AHB clock
 */
unsigned long get_ahb_clk(void)
{
	return ahb_clk;
}
EXPORT_SYMBOL(get_ahb_clk);

/**
 * Get frequency of AXI clock
 */
unsigned long get_axi_clk(void)
{
	return axi_clk;
}
EXPORT_SYMBOL(get_axi_clk);

/**
 * Get frequency of CPU clock
 */
unsigned long get_cpu_clk(void)
{
	return arm_clk;
}
EXPORT_SYMBOL(get_cpu_clk);

/**
 * Get frequency of MMC clock
 */
unsigned long get_mmc_clk(void)
{
	return mmc_clk;
}
EXPORT_SYMBOL(get_mmc_clk);

/**
 * @fn void SYS_CLK_SetClockSource(EN_SYS_CLK_SRC enSrc, unsigned long Src)
 *
 * @brief  Set clock source for specific top or engine
 *
 * @param[in]  enSrc	Indicate the clock source which will be changed.
 * @param[in]  (unsigned long) Src	New clock source value
 *
 */
void SYS_CLK_SetClockSource(enum EN_SYS_CLK_SRC enSrc, unsigned long Src)
{
	unsigned long flags;
	unsigned long RegVal;
	unsigned long RegOff = stKerClkSrcSel[(int)enSrc].RegOff;
	unsigned long FieldSize = stKerClkSrcSel[(int)enSrc].FieldSize;
	unsigned long FieldPos = stKerClkSrcSel[(int)enSrc].FieldPos;
	unsigned long *off = (unsigned long *)(
		(unsigned long)clk_reg_base + (unsigned long) RegOff);

	if (Src >= (1UL << FieldSize))
		return;

	/**
	 * Critical section start
	 */
	raw_spin_lock_irqsave(&clock_gen_lock, flags);

	RegVal = readl(off);
	RegVal &= ~(((1UL << FieldSize) - 1) << FieldPos);
	RegVal |= (Src << FieldPos);
	writel((unsigned long) RegVal, off);

	raw_spin_unlock_irqrestore(&clock_gen_lock, flags);
	/**
	 * Critical section end
	 */
}
EXPORT_SYMBOL(SYS_CLK_SetClockSource);

/**
 * @fn void SYS_SetClockReset(EN_SYS_CLK_RST enRst, bool b8EnRst)
 *
 * @brief Clock reset enable/disable
 *
 * @param[in] enRst		Clock reset which will be enable or disable
 * @param[in] b8EnRst	TRUE: enable clock reset, FALSE: disable clock reset
 *
 */
void SYS_SetClockReset(enum EN_SYS_CLK_RST enRst, bool b8EnRst)
{
	unsigned long flags;
	unsigned long RegOff = stSYSClkRst[(int)enRst].RegOff;
	unsigned long Mask = stSYSClkRst[(int)enRst].Mask;
	unsigned long *off = (unsigned long *)(
		(unsigned long)clk_reg_base + (unsigned long) RegOff);

	/**
	 * Critical section start
	 */
	raw_spin_lock_irqsave(&clock_gen_lock, flags);

	if (b8EnRst) /* Reset enable */
		writel((unsigned long) Mask, off++);
	else /* Reset disable */
		writel((unsigned long) Mask, off);

	raw_spin_unlock_irqrestore(&clock_gen_lock, flags);
	/**
	 * Critical section end
	 */
}
EXPORT_SYMBOL(SYS_SetClockReset);

/**
 * @fn void SYS_CLK_SetClockInv(EN_SYS_CLK_INV enInv, bool b8EnInv)
 *
 * @brief Clock inverse enable/disable
 *
 * @param[in] enInv		Clock inverse which will be enable or disable
 * @param[in] b8EnInv	TRUE: clock inverse enable, FALSE clock inversse disable
 *
 */
void SYS_SetClockInv(enum EN_SYS_CLK_INV enInv, bool b8EnInv)
{
	if (b8EnInv)
		sys_set_bit(enInv, (void *)(clk_reg_base));
	else
		sys_clear_bit(enInv, (void *)(clk_reg_base));
}
EXPORT_SYMBOL(SYS_SetClockInv);

/**
 * @fn void SYS_SetClockMask(EN_SYS_CLK_MASK enMask, bool b8EnMask)
 *
 * @brief Clock mask enable/disable
 *
 * @param[in] enMask	Clock mask which will be enable or disable
 * @param[in] b8EnMask	TRUE: clock mask enable, FALSE clock mask disable
 *
 */
void SYS_SetClockMask(enum EN_SYS_CLK_MASK enMask, bool b8EnMask)
{
	if (b8EnMask)
		sys_set_bit(enMask, (void *)(clk_reg_base));
	else
		sys_clear_bit(enMask, (void *)(clk_reg_base));
}
EXPORT_SYMBOL(SYS_SetClockMask);

static void compute_ahb_clk(void)
{
#if CONFIG_NVT_SYS_CLK_DEBUG
	NVT_SYS_CLK_DBG("Func %s\n", __func__);
#endif
	if (_MPLL_GetData(0x64)) {
		/**
		 * Real Chip
		 */
		switch (_AHB_CLK_SEL) {
		case EN_SYS_AHB_CLK_SRC_REF_96M:	/* OSC16X/2 */
			ahb_clk = 96000000;

			break;

		case EN_SYS_AHB_CLK_SRC_ARM_D8:		/* ARM_D8CK */
			ahb_clk = (arm_clk / 8);

			break;

		case EN_SYS_AHB_CLK_SRC_AHB:		/* AHB_CK */
		default:
			ahb_clk = SYS_CLK_GetMpll(EN_MPLL_OFF_AHB);
			ahb_clk *= 1000000;

			break;
		}
	} else {
		ahb_clk = 12000000;
	}
}

static void compute_axi_clk(void)
{
#if CONFIG_NVT_SYS_CLK_DEBUG
	NVT_SYS_CLK_DBG("Func %s\n", __func__);
#endif
	if (_MPLL_GetData(0x64)) {
		/**
		 * Real Chip
		 */
		switch (_AXI_CLK_SEL) {
		case EN_SYS_AXI_CLK_SRC_ARM_D8:	/* ARM_D8CK */
			axi_clk = (arm_clk / 8);
		break;

		case EN_SYS_AXI_CLK_SRC_DDR_D2:	/* DDR_D2CK */
			axi_clk = (SYS_CLK_GetMpll(EN_MPLL_OFF_DDR) / 2);
			axi_clk *= 1000000;
			break;

		case EN_SYS_AXI_CLK_SRC_AXI_D2:	/* AXI_CLK/2 */
		case EN_SYS_AXI_CLK_SRC_AXI:	/* AXI_CLK */
			axi_clk =
				(SYS_CLK_GetMpll(EN_MPLL_OFF_AXI) /
				 (4 - _AXI_CLK_SEL));
			axi_clk *= 1000000;
			break;

		default:
			NVT_SYS_CLK_ERR("Invalid AXI clock selection\n");
			break;
		}
	} else {
		axi_clk = 27000000;
	}
}

static void compute_mmc_clk(void)
{
#if CONFIG_NVT_SYS_CLK_DEBUG
	NVT_SYS_CLK_DBG("Func %s\n", __func__);
#endif

	/* Real Chip */
	if (_MPLL_GetData(0x64)) {
		mmc_clk = SYS_CLK_GetMpll(EN_MPLL_OFF_EMMC);
		mmc_clk *= 1000000;
		mmc_clk *= 4;
	} else {
		mmc_clk = 12000000;
	}
}

static void compute_cpu_clk(void)
{
#if CONFIG_NVT_SYS_CLK_DEBUG
	NVT_SYS_CLK_DBG("Func %s\n", __func__);
#endif

	/* Real Chip */
	if (_MPLL_GetData(0x64)) {
		/**
		 * Get ratio of ARM PLL
		 */
		arm_clk = SYS_CLK_GetMpll(EN_MPLL_OFF_ARM);

		/**
		 * Check MUX
		 */
		if ((_APLL_GetData(0x00) & 0x1)) {	/* Select local PLL */
			NVT_SYS_CLK_INFO("\tSelect local PLL\n");
			arm_clk *= 8;
		} else {
			NVT_SYS_CLK_INFO("\tSelect MPLL\n");
		}

		arm_clk *= 1000000;
	} else {
		arm_clk = 55000000;
	}
}

static void compute_ddr_clk(void)
{
	if (_MPLL_GetData(0x64))
		ddr_clk = SYS_CLK_GetMpll(EN_MPLL_OFF_DDR);
	else
		ddr_clk = 1800;

}
