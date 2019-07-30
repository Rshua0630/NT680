#include <linux/err.h>
#include <linux/io.h>
#include <linux/module.h>
#include <linux/of.h>
#include <linux/of_address.h>
#include <linux/of_fdt.h>
#include <linux/of_irq.h>
#include <linux/of_platform.h>
#include <linux/delay.h>
#include <linux/reboot.h>
#include <linux/slab.h>

#include "sdhci.h"
#include "sdhci-pltfm.h"
#include "linux/dma-mapping.h"
#include "nvt_sdc.h"
#include "soc/nvt/clk.h"
#include "../core/mmc_ops.h"
#include <linux/mmc/mmc.h>
#define EN_MPLL_OF_EMMC	 0xc4
#if 0
#ifdef DEBUG
#define debug_log(msg, args...) \
	pr_err("mmc%s:%s:%d: " #msg "\n",	\
		 __FILE__, __func__, __LINE__, ## args)
#else
#define debug_log(msg, args...)
#endif
#define info_log(msg, args...)  \
	pr_err("[ERR_BSP_MMC]%s:%s:%d: " #msg "\n",\
		 __FILE__, __func__, __LINE__, ##args)
#endif
#define EN_MPLL_OFF_EMMC	 0xc4	/*EMMC*/

#ifdef CONFIG_NVT_MMC_IO
void set_emmc_io_driving(struct nvt_mmc_hwplat *hwplat, u32 value,
		void __iomem *reg, unsigned int emmc_item, unsigned int mode);
#define NVT_MMC_DEBUGFS
#endif

#ifdef CONFIG_MMC_SDHCI_NVT_HS400_RDQS_TUNING
static int nvt_mmc_rdqs_tuning_set(void *data, u64 val);
static int nvt_mmc_rdqs_tuning_get(void *data, u64 *val);
#endif

static void __iomem *g_sdc_vbase;
static struct nvt_mmc_hwplat *_hwplat;

static  inline void writel_and_bits(u32 value, /*volatile*/ void __iomem *p)
{
	writel(readl(p) & value, p);
}

static  inline void writel_or_bits(u32 value, /*volatile*/ void __iomem *p)
{
	writel(readl(p) | value, p);

}

static  inline void writeb_and_bits(u8 value, /*volatile*/ void __iomem *p)
{
	writeb(readb(p) & value, p);

}

static  inline void writeb_or_bits(u8 value, /*volatile*/ void __iomem *p)
{
	writeb(readb(p) | value, p);

}

static void sdhci_nvt_writeb(struct sdhci_host *host, u8 val, int reg)
{
	/*debug_log("[MMC] reg = 0x%x, val = 0x%x", reg, val);*/

	switch (reg) {
	case SDHCI_POWER_CONTROL:
		/* clear pwr setting */
		val &= (u8)(~(7<<1));
		/* even 1.8V bus voltage on the board,
		   we need to set SDHCI_POWER_330 still
		   since we switch 1.8V/3.3V via other way
		 */
		val |= SDHCI_POWER_330;
		writeb(val, host->ioaddr + reg);
		return;
	case SDHCI_TIMEOUT_CONTROL:
		/* always use the longest data timeout
			to prevent poor devices failed */
		writeb(0xf, host->ioaddr + reg);
		return;
	case SDHCI_HOST_CONTROL:
		writeb(val&((u8)(~SDHCI_CTRL_LED)), host->ioaddr + reg);
		return;
	default:
		break;
	}

	writeb(val, host->ioaddr + reg);
}

/* sdhci_nvt_writew(), sdhci_nvt_writel() and sdhci_nvt_readw()
	callback functions do nothing than setting/reading register
	  we attach these MMIO callback functions for debug
	use when we need to trace the control flow of mmc layer
	*/

static void sdhci_nvt_writew(struct sdhci_host *host, u16 val, int reg)
{
	/*debug_log("[MMC] reg = 0x%x, val = %x", reg, val);*/
	writew(val, host->ioaddr + reg);
}

static void sdhci_nvt_writel(struct sdhci_host *host, u32 val, int reg)
{
	/*debug_log("[MMC] reg = 0x%x, val = %x", reg, val);*/
	writel(val, host->ioaddr + reg);
}

static u16 sdhci_nvt_readw(struct sdhci_host *host, int reg)
{
	/*debug_log("[MMC] reg = 0x%x, val = %x", reg, val);*/
	return readw(host->ioaddr + reg);
}

static unsigned int sdhci_nvt_get_max_clock(struct sdhci_host *host)
{
	struct sdhci_pltfm_host *pltfm_host = sdhci_priv(host);
	struct nvt_mmc_hwplat *hwplat = pltfm_host->priv;

	return hwplat->max_bus_clk;
}

static unsigned int sdhci_nvt_get_min_clock(struct sdhci_host *host)
{
	struct sdhci_pltfm_host *pltfm_host = sdhci_priv(host);
	struct nvt_mmc_hwplat *hwplat = pltfm_host->priv;

	return hwplat->min_bus_clk;
}

static unsigned int sdhci_nvt_get_timeout_clock(struct sdhci_host *host)
{
	return 1;
}

static u32 nvt_mmc_clk2div(struct nvt_mmc_hwplat *hwplat, u32 clk)
{
	u32 q = 0;
	u32 r = 0;
	/* TODO: make sure we have select faster clk source */
	u32 clk_src = hwplat->src_clk;

	/* the max clk using this approach is clk_src divied by 8 */
	if (clk_src/8 <= clk)
		return 0;

	q = (clk_src/(clk*4));
	r = (clk_src%(clk*4));
	q -= 2;

	if (r)
		q += 1;

	return q;
}

static int nvt_mmc_init(struct nvt_mmc_hwplat *hwplat)
{
	void __iomem *fcr = hwplat->fcr_vbase;
	void __iomem *nfc = hwplat->nfc_vbase;
	static int ioremap_done;

	g_sdc_vbase = hwplat->sdc_vbase;
	_hwplat = hwplat;

	if (!ioremap_done) {
		hwplat->ioremap_base = ioremap_nocache(NVT_IOREMAP_BASE,
			NVT_IOREMAP_REGION);
		hwplat->driving_ctrl = hwplat->ioremap_base +
			(NVT_REG_PHY_DRIVING_CTRL - NVT_IOREMAP_BASE);
		hwplat->key_ctrl = hwplat->ioremap_base +
		    (NVT_REG_PHY_KEY_CTRL - NVT_IOREMAP_BASE);
		hwplat->clk_src_ctrl = hwplat->ioremap_base +
		    (NVT_REG_PHY_SRC_CLK_CTRL - NVT_IOREMAP_BASE);
		hwplat->arbitor_ctrl = hwplat->ioremap_base +
		    (NVT_REG_PHY_MMC_ARBITOR_CTRL - NVT_IOREMAP_BASE);
		hwplat->hw_reset_ctrl = hwplat->ioremap_base +
		    (NVT_REG_PHY_HW_RESET_CTRL - NVT_IOREMAP_BASE);
		hwplat->mpll_ctrl = hwplat->ioremap_base +
		    (NVT_REG_PHY_MPLL - NVT_IOREMAP_BASE);
#ifdef CONFIG_NVT_MMC_IO
		hwplat->emmc_io_driving_vbase = hwplat->ioremap_base +
		    (NVT_REG_EMMC_IODRV - NVT_IOREMAP_BASE);
#endif
		if ((!hwplat->key_ctrl) || (!hwplat->clk_src_ctrl) ||
			(!hwplat->arbitor_ctrl) || (!hwplat->hw_reset_ctrl)) {
			pr_err("key_ctrl(%p), clk_src_ctrl(%p), ",
				hwplat->key_ctrl,
				hwplat->clk_src_ctrl);
			pr_err("arbitor_ctrl(%p)", hwplat->arbitor_ctrl);
			pr_err("or hw_reset_ctrl(%p) remap failed",
					 hwplat->hw_reset_ctrl);
			return -1;
		}

		hwplat->ioremap_acp_base = ioremap_nocache(NVT_IOREMAP_ACP_BASE,
			NVT_IOREMAP_ACP_REGION);
		hwplat->acp_ctrl0 = ioremap_nocache(NVT_REG_PHY_ACP_CTRL0, 4);
		hwplat->acp_ctrl1 = hwplat->ioremap_acp_base +
		    (NVT_REG_PHY_ACP_CTRL1 - NVT_IOREMAP_ACP_BASE);
		hwplat->acp_ctrl2 = hwplat->ioremap_acp_base +
		    (NVT_REG_PHY_ACP_CTRL2 - NVT_IOREMAP_ACP_BASE);
		hwplat->acp_ctrl3 = ioremap_nocache(NVT_REG_PHY_ACP_CTRL3, 4);

		if ((!hwplat->acp_ctrl0) || (!hwplat->acp_ctrl1)
			|| (!hwplat->acp_ctrl2) || (!hwplat->acp_ctrl3)) {

			pr_err("acp_ctrl0(%p), acp_ctrl1(%p), acp_ctrl2(%p)",
				hwplat->acp_ctrl0,
				hwplat->acp_ctrl1, hwplat->acp_ctrl2);
			pr_err(" or acp_ctrl3(%p) remap failed",
				hwplat->acp_ctrl3);
			return -1;
		}

		ioremap_done = 1;
	}

	/* set mmc controller arbitor to allow
		only mainchip to control mmc registers */
	writel(NVT_CTRL_KEY1, hwplat->key_ctrl);
	writel(NVT_CTRL_KEY2, hwplat->key_ctrl);

	writel_and_bits((u32)(~NVT_EMMC_STBC_ENABLE), hwplat->arbitor_ctrl);

	writel(NVT_CTRL_KEY1, hwplat->key_ctrl);
	writel(NVT_CTRL_KEY2, hwplat->key_ctrl);
	writel_or_bits(NVT_EMMC_MAINCHIP_ENABLE, hwplat->arbitor_ctrl);

	/**
	 Be Careful,
	 1. low MPLL
	 2. HW reset
	 3. use user MPLL (much higher clk)
	 */

	/* use low MPLL first before HW reset */
	SYS_CLK_SetMpll(EN_MPLL_OF_EMMC, 208>>2);
	udelay(1);

	/* host HW reset */
	writel(NVT_CTRL_KEY1, hwplat->key_ctrl);
	writel(NVT_CTRL_KEY2, hwplat->key_ctrl);
	writel_or_bits(NVT_EMMC_HW_RESET_START, hwplat->hw_reset_ctrl);

	udelay(1);

	writel(NVT_CTRL_KEY1, hwplat->key_ctrl);
	writel(NVT_CTRL_KEY2, hwplat->key_ctrl);
	writel_and_bits((u32)(~NVT_EMMC_HW_RESET_START), hwplat->hw_reset_ctrl);

	udelay(1);

	/* OK, we can use user-defined MPLL now */
	SYS_CLK_SetMpll(EN_MPLL_OF_EMMC, (hwplat->src_clk/1000000)>>2);
	udelay(1);

	/* TODO: make sure the src clk value is correct */

	/* change to high emmc clk src */
	writel(NVT_CTRL_KEY1, hwplat->key_ctrl);
	writel(NVT_CTRL_KEY2, hwplat->key_ctrl);
	writel_or_bits(NVT_EMMC_FAST_CLK_SRC_ENABLE, hwplat->clk_src_ctrl);

	/* this register might be modified by STBC,
		need to set its default value again, or cmd might error */
	writel(FCR_CPBLT_DEFAULT, fcr+REG_FCR_CPBLT);

	/* set DMA beat mode, 0x3:16_8_4, 0x2: 8_4, 0x1:4, 0x0:1*/
	writel(((0x3<<20) | FCR_FUNC_CTRL_DEFAULT),
		fcr+REG_FCR_FUNC_CTRL);

	/* enable SW card detect function */
	writel_or_bits(FCR_FUNC_CTRL_SW_CDWP_ENABLE, fcr+REG_FCR_FUNC_CTRL);

	/* force card always exists */
	writel_and_bits((u32)(~FCR_FUNC_CTRL_SW_SD_CD), fcr+REG_FCR_FUNC_CTRL);

	/* data bus is little endian */
	writel_or_bits(FCR_FUNC_CTRL_LITTLE_ENDIAN, fcr+REG_FCR_FUNC_CTRL);

	/* do not bypass sd clk */
	writel_and_bits((u32)(~FCR_CPBLT_SD_CLK_BYPASS), fcr+REG_FCR_CPBLT);

	/* since NFC has nand and eMMC, select eMMC */
	writel_or_bits(NFC_EMMC_SEL, nfc+REG_NFC_SYS_CTRL);

	/* disable HS200 for now */
	writel(0, fcr+REG_FCR_HS200_CTRL);

	/* disable AHB to AXI ACP */
	writel_and_bits((u32)(~NVT_ACP_AHB2AXI_ENABLE), hwplat->acp_ctrl1);
	hwplat->gating_status = 1;
	udelay(1);

	return 0;
}

static void nvt_mmc_deinit(struct nvt_mmc_hwplat *hwplat)
{
	iounmap(hwplat->ioremap_base);
	iounmap(hwplat->acp_ctrl0);
	iounmap(hwplat->acp_ctrl3);
	iounmap(hwplat->ioremap_acp_base);
}

static int nvt_mmc_set_bus_width(struct nvt_mmc_hwplat *hwplat, int bus_width)
{
	void __iomem *sdc = hwplat->sdc_vbase;
	void __iomem *fcr = hwplat->fcr_vbase;

	debug_log("[MMC] bus width = %d", bus_width);

	switch (bus_width) {
	case MMC_BUS_WIDTH_1:
		writeb_and_bits((u8)(~SDC_HOST_CTRL_4BIT),
			sdc+REG_SDC_HOST_CTRL);
		writel_and_bits((u32)(~FCR_FUNC_CTRL_MMC_8BIT),
			fcr+REG_FCR_FUNC_CTRL);
		writeb_and_bits((u8)(~SDC_HOST_CTRL_8BIT),
			sdc+REG_SDC_HOST_CTRL);
		break;
	case MMC_BUS_WIDTH_4:
		writeb_or_bits((u8)SDC_HOST_CTRL_4BIT, sdc+REG_SDC_HOST_CTRL);
		writel_and_bits((u32)(~FCR_FUNC_CTRL_MMC_8BIT),
			fcr+REG_FCR_FUNC_CTRL);
		writeb_and_bits((u8)(~SDC_HOST_CTRL_8BIT),
			sdc+REG_SDC_HOST_CTRL);
		break;
	case MMC_BUS_WIDTH_8:
		/* note: following setting sequence is very important,
		 __NEVER__ try to change the sequence!! */
		writeb_or_bits((u8)SDC_HOST_CTRL_4BIT, sdc+REG_SDC_HOST_CTRL);
		writel_or_bits((u32)FCR_FUNC_CTRL_MMC_8BIT,
			fcr+REG_FCR_FUNC_CTRL);
		writeb_or_bits((u8)SDC_HOST_CTRL_8BIT, sdc+REG_SDC_HOST_CTRL);
		break;
	default:
		info_log("invalid bus width(%d)", bus_width);
		return -1;
	}

	udelay(1);

	return 0;
}

static int _nvt_mmc_set_bus_timing_mode(struct nvt_mmc_hwplat *hwplat,
					enum NVT_MMC_SPEED_MODE speed_mode,
					enum NVT_MMC_DATA_LATCH latch,
					int enhanced_strobe
					)
{
	void __iomem *sdc = hwplat->sdc_vbase;
	void __iomem *fcr = hwplat->fcr_vbase;

	debug_log("[MMC] speed_mode = %d", speed_mode);
	debug_log("[MMC] latch = %d", latch);

	if ((speed_mode == NVT_MMC_HIGH_SPEED)
		|| (speed_mode == NVT_MMC_HS200)) {
		writeb_or_bits(SDC_HOST_CTRL_HIGH_SPEED, sdc+REG_SDC_HOST_CTRL);
		mdelay(1);

		if (speed_mode == NVT_MMC_HS200) {
			writel_or_bits(FCR_HS200_CTRL_DISABLE_CMD_CONFLICT,
				fcr+REG_FCR_HS200_CTRL);
			writel_and_bits((u32)(~(FCR_HS200_OUTPUT_SELECT_MASK)),
				fcr+REG_FCR_HS200_CTRL);
			writel_or_bits(
			FCR_HS200_OUTPUT_SELECT_PHASE(hwplat->host_clk_shift),
				fcr+REG_FCR_HS200_CTRL);
		#ifdef NVT_MMC_HS200_TRACE_EACH_BLK
			writel_or_bits(FCR_HS200_CTRL_HW_TRACK_EACH_BLK,
				fcr+REG_FCR_HS200_CTRL);
		#endif
			writel_or_bits(FCR_HS200_CTRL_ENABLE,
				fcr+REG_FCR_HS200_CTRL);
		} else {
			writel_and_bits((u32)(~FCR_HS200_CTRL_ENABLE),
				fcr+REG_FCR_HS200_CTRL);
			writel_and_bits(
				(u32)(~FCR_HS200_CTRL_DISABLE_CMD_CONFLICT),
				fcr+REG_FCR_HS200_CTRL);
		}

	} else if (speed_mode == NVT_MMC_LEGACY_SPEED) {
		writel_and_bits((u32)(~FCR_HS200_CTRL_ENABLE),
			fcr+REG_FCR_HS200_CTRL);
		writeb_and_bits((u8)(~SDC_HOST_CTRL_HIGH_SPEED),
			sdc+REG_SDC_HOST_CTRL);
	} else {
		info_log("[MMC] not support host speed mode(%d)", speed_mode);
		return -1;
	}

	if (latch == NVT_MMC_SINGLE_LATCH) {
		writel_and_bits((u32)(~FCR_CPBLT_DUAL_DATA_RATE_ENABLE),
			fcr+REG_FCR_CPBLT);
	} else if (latch == NVT_MMC_DUAL_LATCH) {
		writel_or_bits(FCR_CPBLT_DUAL_DATA_RATE_ENABLE,
			fcr+REG_FCR_CPBLT);
	} else {
		info_log("[MMC] latch(%d) is invalid.", latch);
		return -1;
	}

	if (speed_mode == NVT_MMC_HS200 && latch == NVT_MMC_DUAL_LATCH) {
		writel_or_bits((u32)(FCR_HS200_CTRL_RESET_DLL),
			fcr+REG_FCR_HS200_CTRL);
	}

	if (enhanced_strobe == 1) {
		writel_or_bits((u32)
			(FCR_HS200_CTRL_EXTEND_STROBE |
			FCR_HS200_CTRL_EXTSTRB_LATCH)
			, fcr+REG_FCR_HS200_CTRL);
	} else {
	}

	udelay(1);

	return 0;
}
static int nvt_mmc_set_bus_timing_mode(struct nvt_mmc_hwplat *hwplat,
					enum NVT_MMC_SPEED_MODE speed_mode,
					enum NVT_MMC_DATA_LATCH latch)
{
	return  _nvt_mmc_set_bus_timing_mode(hwplat, speed_mode, latch, 0);
}

static void nvt_mmc_set_adll_init(void __iomem *fcr)
{
	unsigned int reg_val = readl(fcr+REG_FCR_ADLL_CFG_REG);

	/*printk(" before adll init cfg = 0x%x\n",reg_val);*/
	reg_val = ADLL_CFG_SET_RGL_LDOA(reg_val, 0x01);
	reg_val = ADLL_CFG_SET_REG_NM(reg_val, 0x1);
	       reg_val = ADLL_CFG_SET_REG_SW(reg_val, 0x2);
	       reg_val = ADLL_CFG_SET_BAND_SEL(reg_val, 0x1);
	writel(reg_val, fcr+REG_FCR_ADLL_CFG_REG);
	       reg_val = readl(fcr+REG_FCR_ADLL_CFG_REG);
	       /*printk(" aflter adll init cfg = 0x%x\n",reg_val);*/
}
static int nvt_mmc_set_adll_top(struct nvt_mmc_hwplat *hwplat, unsigned int clk)
{
	u32 divisor = 0;
	void __iomem *sdc = NULL;
	void __iomem *fcr = NULL;
	int retry = 0;
	u32 clk_src = 0;
	unsigned int reg_val = 0;

	if (unlikely(!hwplat)) {
		info_log("hwplat is null");
		return 0;
	}

	sdc = hwplat->sdc_vbase;
	fcr = hwplat->fcr_vbase;
	clk_src = hwplat->src_clk;

	debug_log("bus clk = %u", clk);
	/*printk("emmc bus clk = %u\n", clk);*/

	if (clk != 0) {
		/* stop bus clk first */
		writel(0, sdc+REG_SDC_CLK_CTRL);


		/* if clk is higher than 1/4 of eMMC clk source,
				use 1/4 clk source, which is the max rate

		for mt case, onboot set emmc run 200 Mhz(emmc pll 800MHz) ,
			but kernel config keep 180Mhz...
		if we don't enable fast clk.. it will be just 100Mhz
		there are other solution like... 1. dtb set emmc 200Mhz
			2. emmc set pll on kernel...
		but add the mt case will make the smallest side-effect...
		*/
		debug_log("[mmc]clk %d clk source %d\n", clk, clk_src);
#define HS200_FASTEST_CLK_THRESHOLD	(100000000)
		if ((clk >= (clk_src/4))
			|| (clk > HS200_FASTEST_CLK_THRESHOLD)) {
			debug_log("mmc fast clk enable !!!!!!!!!!!\n");
#define FASTEST_CLK_ENABLE_THRESHOLD (180000000)
#define MHZ (1000000)
			if (clk_src > (FASTEST_CLK_ENABLE_THRESHOLD*4)) {
			/*desgner talk us don't seting fastest_clk_enable
				at high frequency.
			because the design may not run normally
				at high frequency.
			this circuit is run x4 clk that others*/
				SYS_CLK_SetMpll(EN_MPLL_OF_EMMC,
					(FASTEST_CLK_ENABLE_THRESHOLD/MHZ));
				udelay(1);
			}
			writel_or_bits(FCR_HS200_CRTL_FASTEST_CLK,
				fcr+REG_FCR_HS200_CTRL);
			if (clk_src > (FASTEST_CLK_ENABLE_THRESHOLD*4)) {
				SYS_CLK_SetMpll(EN_MPLL_OF_EMMC,
					(hwplat->src_clk/MHZ)>>2);
				udelay(1);
			}
		} else {
			writel_and_bits((u32)(~FCR_HS200_CRTL_FASTEST_CLK),
				fcr+REG_FCR_HS200_CTRL);

			divisor = nvt_mmc_clk2div(hwplat, clk);

			/* sanity check divisor */
			WARN_ON((divisor > SDC_MAX_CLK_DIV));

			/* use NVT's clk scheme */
			writel_or_bits(FCR_FUNC_CTRL_SD_FLEXIBLE_CLK,
				fcr+REG_FCR_FUNC_CTRL);
		}

		/* turn on bus internal clk */
		writel(
			(SDC_CLK_CTRL_INCLK_ENABLE|
				SDC_CLK_CTRL_SDCLK_FREQ_SEL_EX(divisor)),
			sdc+REG_SDC_CLK_CTRL);

		/* wait internal clk stable */
		while (!(readl(sdc+REG_SDC_CLK_CTRL) &
				SDC_CLK_CTRL_INCLK_STABLE))
			debug_log("wait clk stable");

		pr_info("MMC:  adll  init!!\n");
		nvt_mmc_set_adll_init(fcr);
		/*printk("adll start!!\n");*/
		/*adll setting start*/
		udelay(50);
		writel_and_bits(~ADLL_CFG_ADS_RESET, fcr+REG_FCR_ADLL_CFG_REG);
		udelay(20);

		do {
			if (readl(fcr+REG_FCR_ADLL_STATUS_REG) &
					ADLL_STATUS_FDLOCK) {
				reg_val = readl(fcr+REG_FCR_ADLL_CFG_REG);
				pr_info("MMC: adll Ready!!\n");
				pr_info("adll init cfg = 0x%x\n", reg_val);
				break;
			}

			retry++;
			udelay(1);

		} while (1);
		/*adll setting end*/

		reg_val = readl(fcr+REG_FCR_ADLL_CFG_REG);
		reg_val = ADLL_CFG_SET_ADLL_WRITE_CLK_PHASE(reg_val,
				hwplat->host_adll_write_phase);
		reg_val = ADLL_CFG_SET_ADLL_READ_CLK_PHASE(reg_val,
				hwplat->host_adll_read_phase);
		writel(reg_val, fcr+REG_FCR_ADLL_CFG_REG);

		/* OK, turn on the bus clk */
		writel_or_bits(SDC_CLK_CTRL_SDCLK_ENABLE, sdc+REG_SDC_CLK_CTRL);

		/* wait 10us to do anything further,
			in order to make sure bus clk is stable */
		udelay(10);/*mdelay(1);*/
	}
	return 0;
}

static int nvt_mmc_set_bus_clk(struct nvt_mmc_hwplat *hwplat, unsigned int clk)
{
	u32 divisor = 0;
	void __iomem *sdc = NULL;
	void __iomem *fcr = NULL;
	u32 clk_src = 0;

	if (unlikely(!hwplat)) {
		info_log("hwplat is null");
		return 0;
	}

	sdc = hwplat->sdc_vbase;
	fcr = hwplat->fcr_vbase;
	clk_src = hwplat->src_clk;

	debug_log("bus clk = %u", clk);

	if (clk != 0) {
		/* stop bus clk first */
		writel(0, sdc+REG_SDC_CLK_CTRL);

		/* if clk is higher than 1/4 of eMMC clk source,
			use 1/4 clk source, which is the max rate/

		for mt case, onboot set emmc run 200 Mhz(emmc pll 800MHz) ,
			but kernel config keep 180Mhz...
		if we don't enable fast clk.. it will be just 100Mhz
		there are other solution like... 1. dtb set emmc 200Mhz
		2. emmc set pll on kernel...
		 but add the mt case will make the smallest side-effect..
		*/
		debug_log("[mmc]clk %d clk source %d\n", clk, clk_src);
		#define HS200_FASTEST_CLK_THRESHOLD	(100000000)
		if ((clk >= (clk_src/4)) ||
			(clk > HS200_FASTEST_CLK_THRESHOLD)) {
			debug_log("mmc fast clk enable !!!!!!!!!!!\n");
			#define FASTEST_CLK_ENABLE_THRESHOLD (180000000)
			if (clk_src > (FASTEST_CLK_ENABLE_THRESHOLD*4)) {
			/*desgner talk us don't seting fastest_clk_enable
				at high frequency.
				because the design may not run normally
				at high frequency.
				this circuit is run x4 clk that others
			*/
				SYS_CLK_SetMpll(EN_MPLL_OF_EMMC,
					(FASTEST_CLK_ENABLE_THRESHOLD/MHZ));
				udelay(1);
			}
			writel_or_bits(FCR_HS200_CRTL_FASTEST_CLK,
				fcr+REG_FCR_HS200_CTRL);
			if (clk_src > (FASTEST_CLK_ENABLE_THRESHOLD*4)) {
				SYS_CLK_SetMpll(EN_MPLL_OF_EMMC,
					(hwplat->src_clk/MHZ)>>2);
				udelay(1);
			}
		} else {
			writel_and_bits((u32)(~FCR_HS200_CRTL_FASTEST_CLK),
				fcr+REG_FCR_HS200_CTRL);

			divisor = nvt_mmc_clk2div(hwplat, clk);

			/* sanity check divisor */
			WARN_ON((divisor > SDC_MAX_CLK_DIV));

			/* use NVT's clk scheme */
			writel_or_bits(FCR_FUNC_CTRL_SD_FLEXIBLE_CLK,
				fcr+REG_FCR_FUNC_CTRL);
		}

		/* turn on bus internal clk */
		writel((SDC_CLK_CTRL_INCLK_ENABLE|
			SDC_CLK_CTRL_SDCLK_FREQ_SEL_EX(divisor)),
			sdc+REG_SDC_CLK_CTRL);

		/* wait internal clk stable */
		while (!(readl(sdc+REG_SDC_CLK_CTRL) &
				 SDC_CLK_CTRL_INCLK_STABLE))
			debug_log("wait clk stable");

		/* OK, turn on the bus clk */
		writel_or_bits(SDC_CLK_CTRL_SDCLK_ENABLE, sdc+REG_SDC_CLK_CTRL);

		/* wait 10us to do anything further,
			in order to make sure bus clk is stable */
		udelay(10);
	}
#if defined(CONFIG_MMC_CLKGATE) || defined(CONFIG_NVT_MMC_CLKGATE)
	else{
		/*this is clock gated status.*/
		writel(0, sdc+REG_SDC_CLK_CTRL);
	}
#endif

	return 0;
}

void nvt_mmc_host_reset(struct sdhci_host *host)
{
	int timeout_count = 1000;

	writeb(SDHCI_RESET_CMD|SDHCI_RESET_DATA,
		host->ioaddr + SDHCI_SOFTWARE_RESET);
	while (sdhci_readb(host, SDHCI_SOFTWARE_RESET)
		& (SDHCI_RESET_CMD|SDHCI_RESET_DATA)) {
		udelay(1);
		timeout_count--;
		if (timeout_count == 0) {
			info_log("EMMC : nvt_mmc_host_reset timeout\n");
			break;
		}
	}

}

static void sdhci_nvt_platform_bus_width(struct sdhci_host *host, int width)
{
	struct sdhci_pltfm_host *pltfm_host = sdhci_priv(host);
	struct nvt_mmc_hwplat *hwplat = pltfm_host->priv;
	unsigned char timing = host->mmc->ios.timing;
	int err = 0;

	debug_log("[MMC] mmc ask bus width = %d", width);
	err = nvt_mmc_set_bus_width(hwplat, width);

	if (err)
		return;

	/* since sdhci layer only set DDR/SDR/HS200
		when host is capable of SD 3.0,
		we set them here
	 */
	switch (timing) {
	case MMC_TIMING_LEGACY:
	case MMC_TIMING_MMC_HS:
	case MMC_TIMING_SD_HS:
	case MMC_TIMING_UHS_SDR12:
	case MMC_TIMING_UHS_SDR25:
	case MMC_TIMING_UHS_SDR50:
		nvt_mmc_set_bus_timing_mode(hwplat, NVT_MMC_HIGH_SPEED,
				NVT_MMC_SINGLE_LATCH);
		break;
	case MMC_TIMING_UHS_DDR50:
	case MMC_TIMING_MMC_DDR52:
		nvt_mmc_set_bus_timing_mode(hwplat, NVT_MMC_HIGH_SPEED,
				NVT_MMC_DUAL_LATCH);
		break;
	case MMC_TIMING_MMC_HS200:
		nvt_mmc_set_bus_timing_mode(hwplat, NVT_MMC_HS200,
				NVT_MMC_SINGLE_LATCH);
#if defined(CONFIG_NVT_HS200_LOW_FREQ_RESET)
		/* SW workaround for NVT controller when switch to HS200 */
		nvt_mmc_set_bus_clk(hwplat, 52000000);
#endif
		nvt_mmc_host_reset(host);
		nvt_mmc_set_bus_clk(hwplat, hwplat->cur_clk);
		break;
	case MMC_TIMING_MMC_HS400:
		pr_info("MMC TIMING: HS400\n");
		if (hwplat->cur_clk < hwplat->max_bus_clk) {
			hwplat->cur_clk = hwplat->max_bus_clk;
			nvt_mmc_set_bus_clk(hwplat, hwplat->cur_clk);
		/*	printk("MMC TIMING: HS400 force set 180\n");*/
		}
		/*mdelay(1);
		nvt_mmc_host_reset(host);*/
		nvt_mmc_set_bus_timing_mode(hwplat,
				NVT_MMC_HS200, NVT_MMC_DUAL_LATCH);

		nvt_mmc_host_reset(host);
		/*nvt_mmc_set_bus_clk(hwplat, hwplat->cur_clk);
		nvt_mmc_set_adll_top(hwplat);*/
		nvt_mmc_set_adll_top(hwplat, hwplat->cur_clk);
		nvt_mmc_host_reset(host);
		mdelay(1);

		break;

	default:
		info_log("invalid bus (%d)", (unsigned int)timing);
		return;
	}

}

static void sdhci_nvt_set_clock(struct sdhci_host *host, unsigned int clock)
{
	struct sdhci_pltfm_host *pltfm_host = sdhci_priv(host);
	struct nvt_mmc_hwplat *hwplat = pltfm_host->priv;

	hwplat->cur_clk = clock;
	nvt_mmc_set_bus_clk(hwplat, clock);
}

void nvt_mmc_extclk_gating_control(struct nvt_mmc_hwplat *hwplat, int gating)
{
	void __iomem *sdc = hwplat->sdc_vbase;

	if (gating) {
		writel((readl(sdc+REG_SDC_CLK_CTRL)&~SDC_CLK_CTRL_SDCLK_ENABLE),
			sdc+REG_SDC_CLK_CTRL);
	} else{
		writel((readl(sdc+REG_SDC_CLK_CTRL)|SDC_CLK_CTRL_SDCLK_ENABLE),
			sdc+REG_SDC_CLK_CTRL);
		udelay(1);
	}
}
#ifdef CONFIG_NVT_MMC_CLKGATE

void nvt_mmc_clk_gating(struct sdhci_host *host, struct mmc_command *cmd)
{
	struct sdhci_pltfm_host *pltfm_host = sdhci_priv(host);
	struct nvt_mmc_hwplat *hwplat = pltfm_host->priv;

	if (hwplat->gating_status == 0) {
		nvt_mmc_set_bus_clk(hwplat, 0);
	} else {
		if (cmd)
			info_log("%s check host command : %x\n",
				 __func__, cmd->opcode);
		else
			info_log("%s check host command : NULL\n",
				 __func__);
	}
	hwplat->gating_status = 1;
}
EXPORT_SYMBOL(nvt_mmc_clk_gating);

void nvt_mmc_clk_ungating(struct sdhci_host *host, struct mmc_command *cmd)
{
	struct sdhci_pltfm_host *pltfm_host = sdhci_priv(host);
	struct nvt_mmc_hwplat *hwplat = pltfm_host->priv;

	if (hwplat->gating_status == 1) {
		nvt_mmc_set_bus_clk(hwplat, hwplat->cur_clk);
	} else {
		if (cmd)
			info_log("%s check host command : %x\n",
				__func__, cmd->opcode);
		else
			info_log("%s check host command : NULL\n", __func__);
	}
	hwplat->gating_status = 0;
}
EXPORT_SYMBOL(nvt_mmc_clk_ungating);
#endif

#ifdef CONFIG_MMC_SDHCI_NVT_HS400_RDQS_TUNING
#define rdqs_err(msg, args...)   pr_info("%s:%s:%d: " #msg "\n", \
				__FILE__, __func__, __LINE__, ##args)
#define rdqs_dbg(msg, args...)   pr_debug("%s:%s:%d: " #msg "\n", \
				__FILE__, __func__, __LINE__, ##args)
#define MAX_RDQS_COUNT 16

static inline int bscnl_emit(char *buf, int buflen, int rbot, int rtop, int len)
{
	if (len > 0)
		len += scnprintf(buf + len, buflen - len, ",");
	if (rbot == rtop)
		len += scnprintf(buf + len, buflen - len, "%d", rbot);
	else
		len += scnprintf(buf + len, buflen - len, "%d-%d", rbot, rtop);
	return len;
}

static int bitmap_scnlistprintf(char *buf, unsigned int buflen,
				const unsigned long *maskp, int nmaskbits)
{
		int len = 0;
		int cur, rbot, rtop;

		if (buflen == 0)
			return 0;

		buf[0] = 0;

		rbot = cur = find_first_bit(maskp, nmaskbits);
		while (cur < nmaskbits) {
			rtop = cur;
			cur = find_next_bit(maskp, nmaskbits, cur+1);
			if (cur >= nmaskbits || cur > rtop + 1) {
				len = bscnl_emit(buf, buflen, rbot, rtop, len);
				rbot = cur;
			}
		}
		return len;
}

static int mmc_find_rdqs_median(const unsigned long *bitmap, int bits,
				int *window_size)
{
	int max_start = 0, max_len = 0, now_bit = 0, pre_bit = 0;
	int start = 0, len = 0, i;

	if (bitmap_empty(bitmap, bits))
		return -EINVAL;

	for (i = 0; i < bits * 1; i++) {
		now_bit = test_bit(i % bits, bitmap);

		if (!pre_bit && now_bit) {
			start = i;
			len = 1;
		} else if (pre_bit && now_bit) {
			len++;
		} else if (pre_bit && !now_bit) {
			if (len > max_len) {
				max_start = start;
				max_len = len;
			}
			start = 0;
			len = 0;
		}

		pre_bit = now_bit;
	}

	/* check last */
	if (len > max_len) {
		max_start = start;
		max_len = len;
	}


	if (window_size)
		*window_size = max_len;

	return (max_start+((max_len-1)/2)) % bits;
}

static int mmc_execute_hs400_do_rdqs_tuning(struct mmc_card *card)
{
	struct mmc_host *host = card->host;
	int cur_rdqs;
	int ret;
	int median = 0, window_size = 0;
	static DECLARE_BITMAP(bitmap_rdqs, MAX_RDQS_COUNT);
	static char str_rdqs[MAX_RDQS_COUNT];
	u8 *extcsd = NULL;
	u64 rdqs_save = 0;

	nvt_mmc_rdqs_tuning_get(host, &rdqs_save);

	rdqs_dbg("RDQS Tuning start! range %d to %d, inc %d, current rdqs %lld\n",
				0, MAX_RDQS_COUNT-1, 1, rdqs_save);

	bitmap_zero(bitmap_rdqs, MAX_RDQS_COUNT);
	extcsd = kzalloc(512, GFP_KERNEL);
	if (!extcsd) {
		rdqs_err("kzalloc return NULL.\n");
		return -ENOMEM;
	}

	for (cur_rdqs = 0; cur_rdqs <= MAX_RDQS_COUNT-1; cur_rdqs += 1) {

		/* set cur rdqs value */
		ret = nvt_mmc_rdqs_tuning_set((void *)host, cur_rdqs);

		/* check data error */
		ret = mmc_get_ext_csd(card, &extcsd);
		if (!ret)
			bitmap_set(bitmap_rdqs, cur_rdqs, 1);
	}

	kfree(extcsd);

	median = mmc_find_rdqs_median(bitmap_rdqs, MAX_RDQS_COUNT,
				&window_size);
	if (median < 0) {
		nvt_mmc_rdqs_tuning_set(host, rdqs_save);

		rdqs_dbg("RDQS Tuning fail! error %d\n", median);
		ret = median;
	} else {
		nvt_mmc_rdqs_tuning_set((void *)host, median);

		bitmap_scnlistprintf(str_rdqs, ARRAY_SIZE(str_rdqs),
				bitmap_rdqs, MAX_RDQS_COUNT);
		pr_info("RDQS Tuning done! pass range %s, new rdqs %d, window size %d\n",
				str_rdqs, median, window_size);
		ret = median;
	}
	return ret;

}

static int nvt_mmc_rdqs_tuning_get(void *data, u64 *val)
{
	struct mmc_host *host = data;
	struct sdhci_host *nvt_host = mmc_priv(host);
	struct sdhci_pltfm_host *pltfm_host = sdhci_priv(nvt_host);
	struct nvt_mmc_hwplat *hwplat = pltfm_host->priv;
	void __iomem *fcr = hwplat->fcr_vbase;

	*val =  (readl(fcr+REG_FCR_ADLL_CFG_REG) & RDQS_MASK) >> RDQS_BIT_SHIFT;
	return 0;
}

static int nvt_mmc_rdqs_tuning_set(void *data, u64 val)
{
	struct mmc_host *host = data;
	struct sdhci_host *nvt_host = mmc_priv(host);
	struct sdhci_pltfm_host *pltfm_host = sdhci_priv(nvt_host);
	struct nvt_mmc_hwplat *hwplat = pltfm_host->priv;
	void __iomem *fcr = hwplat->fcr_vbase;
	u32 reg_value =  readl(fcr+REG_FCR_ADLL_CFG_REG);
	unsigned long flags;

	if (val > (RDQS_MASK >> RDQS_BIT_SHIFT) || val < 0) {
		pr_info("Input RDQS is wrong\n");
		return -EINVAL;
	}
	reg_value &= (~RDQS_MASK);
	reg_value |= (val << RDQS_BIT_SHIFT);

	spin_lock_irqsave(&nvt_host->lock, flags);
	if (hwplat->gating_status == 0)
		nvt_mmc_extclk_gating_control(hwplat, 1);
	writel(reg_value, fcr+REG_FCR_ADLL_CFG_REG);

	if (hwplat->gating_status == 0)
		nvt_mmc_extclk_gating_control(hwplat, 0);
	spin_unlock_irqrestore(&nvt_host->lock, flags);

	return 0;
}

int nvt_mmc_execute_hs400_rdqs_tuning(struct mmc_host *host,
		struct mmc_card *card)
{
	struct sdhci_host *nvt_host = mmc_priv(host);
	struct sdhci_pltfm_host *pltfm_host = sdhci_priv(nvt_host);
	struct nvt_mmc_hwplat *hwplat = pltfm_host->priv;
	int ret = 0;

	if (hwplat->rdqs_tune == MMC_RDQS_TUNE_DISABLED) {
		pr_info("first boot. skip hs400 rdqs tuning.\n");
		hwplat->rdqs_tune = MMC_RDQS_TUNE_ENABLING;
		return 0;
	}

	ret = mmc_execute_hs400_do_rdqs_tuning(card);

	if (ret < 0)
		hwplat->rdqs_tune = MMC_RDQS_TUNE_ERROR;
	else
		hwplat->rdqs_tune = MMC_RDQS_TUNE_DONE;

	return ret;
}
EXPORT_SYMBOL(nvt_mmc_execute_hs400_rdqs_tuning);
#endif

#ifdef CONFIG_DEBUG_FS
#include <linux/debugfs.h>
#ifdef NVT_MMC_DEBUGFS
unsigned int emmc_ds_pos = 9; /*CLK strength*/
unsigned int emmc_ds_vol = 2;

struct EMMC_DS_SET {
	char name[5];
	u32 offset;
	unsigned int ds_shift;
};

#define GEN_DS_ITEM(NAME, REG_OFFSET, DS_SHIFT) \
		{.name = #NAME, .offset = REG_OFFSET, .ds_shift = DS_SHIFT},
struct EMMC_DS_SET emmc_ds_set[] = {
	GEN_DS_ITEM(D0, 0x0, 0x0)
	GEN_DS_ITEM(D1, 0x4, 0x0)
	GEN_DS_ITEM(D2, 0x8, 0x0)
	GEN_DS_ITEM(D3, 0xc, 0x0)
	GEN_DS_ITEM(D4, 0x10, 0x0)
	GEN_DS_ITEM(D5, 0x14, 0x0)
	GEN_DS_ITEM(D6, 0x18, 0x0)
	GEN_DS_ITEM(D7, 0x1c, 0x0)
	GEN_DS_ITEM(CMD, 0x24, 0x0)
	GEN_DS_ITEM(CLK, 0x20, 0x0)
	GEN_DS_ITEM(DS, 0x28, 0x0)
};

void set_emmc_io_driving(struct nvt_mmc_hwplat *hwplat, u32 value,
		void __iomem *reg, unsigned int emmc_item, unsigned int mode)
{
	/*i is position , cmd 8 clk 9 ds 10*/
	u32 reg_value = 0;
	u32 tmp_value = 0;
	int i;

	if (value != DEFAULT_DRIVING) {
		u32 reg_long_term_key = 0;

		writel(NVT_CTRL_KEY1, hwplat->key_ctrl);
		writel(NVT_CTRL_KEY2, hwplat->key_ctrl);
		reg_long_term_key = readl(hwplat->key_ctrl +
				OFFSET_LONG_TERM_KEY);
		writel(MAGIC_KEY, hwplat->key_ctrl + OFFSET_LONG_TERM_KEY);

		reg_value = readl(reg + emmc_ds_set[emmc_item].offset);
		value &= DRIVING_MASK;

		if (mode == NVT_MMC_PMOS) {
			reg_value &= ~(DRIVING_MASK << DRIVING_BIT);
			reg_value |= (value << DRIVING_BIT);
			tmp_value = reg_value & BYTE_MASK;
			tmp_value |= NMOS_DRIVING;
			writel(tmp_value, hwplat->driving_ctrl);

			/* check write done */
			for (i = 0 ; i < ACK_RETRY_COUNT ; i++) {
				if ((readl(reg + emmc_ds_set[emmc_item].offset)
				& BYTE_MASK) == (reg_value & BYTE_MASK)) {
					break;
				}
				udelay(10);
			}

			reg_value &= (BYTE_MASK << 8);
			reg_value = PMOS_DRIVING | (reg_value >> 8);
			writel(reg_value, hwplat->driving_ctrl);
		} else {
			reg_value &= ~(DRIVING_MASK);
			reg_value |= value;
			reg_value &= BYTE_MASK;
			reg_value |= NMOS_DRIVING;
			writel(reg_value, hwplat->driving_ctrl);
		}
		pr_info("EMMC set driving 0x%u @%p\n", value, reg);
		writel(reg_long_term_key, hwplat->key_ctrl +
				OFFSET_LONG_TERM_KEY);
	}
}
static u32 mmc_get_ds(int pos, unsigned int mode)
{
	u32 reg_val = 0;

	reg_val = readl(_hwplat->emmc_io_driving_vbase +
			emmc_ds_set[pos].offset);

	if (mode == NVT_MMC_PMOS) {
		reg_val &= ((DRIVING_MASK << DRIVING_BIT));
		reg_val = ((reg_val >> (DRIVING_BIT)));
	} else {
		reg_val &= ((DRIVING_MASK << emmc_ds_set[pos].ds_shift));
		reg_val = ((reg_val >> emmc_ds_set[pos].ds_shift));
	}
	return reg_val;
}

static int mmc_set_ds(unsigned int pos, u32 val, unsigned int mode)
{
	set_emmc_io_driving(_hwplat, val, _hwplat->emmc_io_driving_vbase,
				pos, mode);
	return 0;
}

static int mmc_driver_strength_pmos_get(void *data, u64 *val)
{
	u32 reg_val = 0;

	reg_val = mmc_get_ds(emmc_ds_pos, NVT_MMC_PMOS);
	pr_info("Get PMOS clk_strength :  %s => [%x]\n",
	emmc_ds_set[emmc_ds_pos].name, reg_val);

	*val = reg_val;
	return 0;
}

static int mmc_driver_strength_pmos_set(void *data, u64 val)
{

	if (val > DRIVING_MASK || val < 0) {
		pr_info("Input driver_strength is wrong: range 0~%x\n",
				DRIVING_MASK);
		return -EINVAL;
	}
	emmc_ds_vol = (u32) val;

	mmc_set_ds(emmc_ds_pos, emmc_ds_vol, NVT_MMC_PMOS);
	pr_info("Set PMOS clk_strength : %s [%x]\n",
				emmc_ds_set[emmc_ds_pos].name, emmc_ds_vol);

	return 0;
}
DEFINE_SIMPLE_ATTRIBUTE(mmc_driver_strength_pmos_fops,
	mmc_driver_strength_pmos_get, mmc_driver_strength_pmos_set, "%llu\n");

static int mmc_driver_strength_nmos_get(void *data, u64 *val)
{
	u32 reg_val = 0;

	reg_val = mmc_get_ds(emmc_ds_pos, NVT_MMC_NMOS);
	pr_info("Get NMOS clk_strength :  %s => [%x]\n",
	emmc_ds_set[emmc_ds_pos].name, reg_val);

	*val = reg_val;
	return 0;
}

static int mmc_driver_strength_nmos_set(void *data, u64 val)
{

	if (val > DRIVING_MASK || val < 0) {
		pr_info("Input driver_strength is wrong: range 0~%x\n",
				DRIVING_MASK);
		return -EINVAL;
	}
	emmc_ds_vol = (u32) val;

	mmc_set_ds(emmc_ds_pos, emmc_ds_vol, NVT_MMC_NMOS);
	pr_info("Set NMOS clk_strength : %s [%x]\n",
				emmc_ds_set[emmc_ds_pos].name, emmc_ds_vol);

	return 0;
}
DEFINE_SIMPLE_ATTRIBUTE(mmc_driver_strength_nmos_fops,
	mmc_driver_strength_nmos_get, mmc_driver_strength_nmos_set, "%llu\n");

#ifdef MMC_HS400_IO_DRIVING
static int mmc_rdqs_get(void *data, u64 *val)
{
	struct mmc_host *host = data;
	struct sdhci_host *nvt_host = mmc_priv(host);
	struct sdhci_pltfm_host *pltfm_host = sdhci_priv(nvt_host);
	struct nvt_mmc_hwplat *hwplat = pltfm_host->priv;
	void __iomem *fcr = hwplat->fcr_vbase;

	*val =  (readl(fcr+REG_FCR_ADLL_CFG_REG) & RDQS_MASK) >> RDQS_BIT_SHIFT;
	pr_info("read DQS phase selection :  0~ 0x%x (0x%x)\n",
		RDQS_MASK>>RDQS_BIT_SHIFT, (u32)*val);
	return 0;
}

static int mmc_rdqs_set(void *data, u64 val)
{
	struct mmc_host *host = data;
	struct sdhci_host *nvt_host = mmc_priv(host);
	struct sdhci_pltfm_host *pltfm_host = sdhci_priv(nvt_host);
	struct nvt_mmc_hwplat *hwplat = pltfm_host->priv;
	void __iomem *fcr = hwplat->fcr_vbase;
	u32 reg_value =  readl(fcr+REG_FCR_ADLL_CFG_REG);
	unsigned long flags;

	if (val > (RDQS_MASK >> RDQS_BIT_SHIFT) || val < 0) {
		pr_info("Input RDQS is wrong\n");
		return -EINVAL;
	}
	pr_info("RDQS reg_val %x regq %x\n", reg_value,
		(reg_value & RDQS_MASK) >> RDQS_BIT_SHIFT);
	reg_value &= (~RDQS_MASK);
	reg_value |= (val << RDQS_BIT_SHIFT);
	pr_info("RDQS choice is 0x%x\n", (unsigned int)val);

	spin_lock_irqsave(&nvt_host->lock, flags);
	if (hwplat->gating_status == 0)
		nvt_mmc_extclk_gating_control(hwplat, 1);
	writel(reg_value, fcr+REG_FCR_ADLL_CFG_REG);
	if (hwplat->gating_status == 0)
		nvt_mmc_extclk_gating_control(hwplat, 0);
	spin_unlock_irqrestore(&nvt_host->lock, flags);
	pr_info("RDQS reg_val %x regq %x\n", readl(fcr+REG_FCR_ADLL_CFG_REG),
		(readl(fcr+REG_FCR_ADLL_CFG_REG) & RDQS_MASK)
		>> RDQS_BIT_SHIFT);

	return 0;
}

DEFINE_SIMPLE_ATTRIBUTE(mmc_rdqs_fops, mmc_rdqs_get, mmc_rdqs_set,
	"%llu\n");

#ifdef CONFIG_MMC_SDHCI_NVT_HS400_RDQS_TUNING
static int mmc_rdqs_show(void *data, u64 *val)
{
	struct mmc_host *host = data;
	struct mmc_card *card = host->card;

	mmc_claim_host(host);

	mmc_execute_hs400_do_rdqs_tuning(card);

	mmc_release_host(host);
	return 0;
}
DEFINE_SIMPLE_ATTRIBUTE(mmc_rdqs_tune_fops,
	mmc_rdqs_show, NULL, "%llx\n");
#endif

#define WRITE_PHASE_MASK	((1<<5)-1)
static int mmc_write_phase_get(void *data, u64 *val)
{
	struct mmc_host *host = data;
	struct sdhci_host *nvt_host = mmc_priv(host);
	struct sdhci_pltfm_host *pltfm_host = sdhci_priv(nvt_host);
	struct nvt_mmc_hwplat *hwplat = pltfm_host->priv;
	void __iomem *fcr = hwplat->fcr_vbase;
	*val =  readl(fcr+REG_FCR_ADLL_CFG_REG) & WRITE_PHASE_MASK;
	pr_info("WRITE  phase selection :  0 ~ %x: %x\n",
		WRITE_PHASE_MASK, (u32)*val);
	return 0;
}

static int mmc_write_phase_set(void *data, u64 val)
{
	struct mmc_host *host = data;
	struct sdhci_host *nvt_host = mmc_priv(host);
	struct sdhci_pltfm_host *pltfm_host = sdhci_priv(nvt_host);
	struct nvt_mmc_hwplat *hwplat = pltfm_host->priv;
	void __iomem *fcr = hwplat->fcr_vbase;

	u32 reg_value =  readl(fcr+REG_FCR_ADLL_CFG_REG);
	unsigned long flags;

	if (val > WRITE_PHASE_MASK || val < 0) {
		pr_info("Input write phase is wrong\n");
		return -EINVAL;
	}
	pr_info("write phase reg_val %x write_phase %x\n", reg_value,
		(reg_value & WRITE_PHASE_MASK));
	reg_value &= ~WRITE_PHASE_MASK;
	reg_value |= val;
	pr_info("write choice is 0x%x\n", (unsigned int)val);

	spin_lock_irqsave(&nvt_host->lock, flags);
	if (hwplat->gating_status == 0)
		nvt_mmc_extclk_gating_control(hwplat, 1);
	writel(reg_value, fcr+REG_FCR_ADLL_CFG_REG);
	if (hwplat->gating_status == 0)
		nvt_mmc_extclk_gating_control(hwplat, 0);
	spin_unlock_irqrestore(&nvt_host->lock, flags);
	pr_info("write_phase reg_val %x write_phase %x\n",
		readl(fcr+REG_FCR_ADLL_CFG_REG),
		(readl(fcr+REG_FCR_ADLL_CFG_REG) & WRITE_PHASE_MASK));
	return 0;
}

DEFINE_SIMPLE_ATTRIBUTE(mmc_write_phse_fops, mmc_write_phase_get,
	mmc_write_phase_set, "%llu\n");
#endif

static int mmc_write_pattern_set(void *data, u64 val)
{

	struct mmc_host *host = data;
	struct sdhci_host *nvt_host = mmc_priv(host);
	struct mmc_card *card = host->card;
	u8 part_config, save_part;
	int ret;
	int i;

	static u8 __aligned(128) buf[512] = {0};
	unsigned int error_flag = 0;
	u32 status = 0;

	pr_info("info write pattern 0x%llx\n", val);

	for (i = 0; i < (int)ARRAY_SIZE(buf); i += 2) {
		buf[i] = val&0xFF;
		buf[i+1] = (val>>8)&0xFF;
	}

	mmc_claim_host(host);

	/*switch to partition2*/
	part_config = card->ext_csd.part_config;
	save_part = part_config & EXT_CSD_PART_CONFIG_ACC_MASK;
	part_config &= ~EXT_CSD_PART_CONFIG_ACC_MASK;
	part_config |= 2;
	ret = mmc_switch(card, EXT_CSD_CMD_SET_NORMAL,
					EXT_CSD_PART_CONFIG, part_config,
					card->ext_csd.part_time);
	if (ret) {
		pr_err("%s boot part switch fail!\n", __func__);
		return -1;
	}
	card->ext_csd.part_config = part_config;

	for (i = 0; i < 1; i++) {
		struct scatterlist sg;
		struct mmc_request mrq = {0};
		struct mmc_command cmd = {0};
		struct mmc_command stop = {0};
		struct mmc_data mmcdata = {0};

		nvt_mmc_host_reset(nvt_host);

		cmd.opcode = MMC_WRITE_BLOCK;
		cmd.arg = 0;
		cmd.flags = MMC_RSP_R1 | MMC_CMD_ADTC;

		stop.opcode = MMC_STOP_TRANSMISSION;
		stop.arg = 0;
		stop.flags = MMC_RSP_R1B | MMC_CMD_AC;

		sg_init_one(&sg, buf, ARRAY_SIZE(buf));

		mmcdata.blksz = ARRAY_SIZE(buf);
		mmcdata.blocks = 1;
		mmcdata.flags = MMC_DATA_WRITE;
		mmcdata.sg = &sg;
		mmcdata.sg_len = 1;

		mrq.cmd = &cmd;
		mrq.data = &mmcdata;
		mmc_wait_for_req(host, &mrq);

		mrq.cmd = &stop;
		mrq.data = NULL;
		mrq.stop = NULL;

		if (!cmd.error) {
			do {
				ret = mmc_send_status(card, &status);
				if (ret) {
					pr_err("tuning %2d send status fail!(%d)",
						i, ret);
					break;
				}
				udelay(10);
			} while (R1_CURRENT_STATE(status) == R1_STATE_PRG);
		}

		if (mmcdata.error) {
			pr_err("%s mmcdata.error : %d\n", __func__,
						mmcdata.error);
			error_flag = mmcdata.error;
		}
	}

	/*restore*/
	part_config = card->ext_csd.part_config;
	part_config &= ~EXT_CSD_PART_CONFIG_ACC_MASK;
	part_config |= save_part;

	ret = mmc_switch(card, EXT_CSD_CMD_SET_NORMAL,
					EXT_CSD_PART_CONFIG, part_config,
					card->ext_csd.part_time);

	if (ret || error_flag) {
		pr_err("%s boot part switch fail!\n", __func__);
		return -1;
	}
	mmc_release_host(host);

	card->ext_csd.part_config = part_config;

	return 0;
}


static int mmc_write_pattern_get(void *data, u64 *val)
{
	return 0;
}


DEFINE_SIMPLE_ATTRIBUTE(mmc_write_pattern_fops, mmc_write_pattern_get,
			mmc_write_pattern_set, "0x%08llx\n");

static int mmc_read_pattern_get(void *data, u64 *val)
{
	struct mmc_host *host = data;
	struct sdhci_host *nvt_host = mmc_priv(host);
	struct mmc_card *card = host->card;

	static struct scatterlist sg;
	struct mmc_request mrq = {0};
	struct mmc_command cmd = {0};
	struct mmc_command stop = {0};
	struct mmc_data mmcdata = {0};
	int i, ret;

	static u8 __aligned(128) buf[512] = {0};

	u8 part_config;
	u8 save_part;

	mmc_claim_host(host);

	part_config = card->ext_csd.part_config;
	save_part = part_config & EXT_CSD_PART_CONFIG_ACC_MASK;
	part_config &= ~EXT_CSD_PART_CONFIG_ACC_MASK;
	part_config |= 2;

	ret = mmc_switch(card, EXT_CSD_CMD_SET_NORMAL,
					EXT_CSD_PART_CONFIG, part_config,
					card->ext_csd.part_time);
	if (ret) {
		pr_err("(%s) boot part switch fail! to boot mode\n", __func__);
		return -1;
	}
	pr_info("before read pattern, 0byte : %x, 1byte : %x\n",
			buf[0], buf[1]);

	sg_init_one(&sg, buf, ARRAY_SIZE(buf));

	for (i = 0; i < 1; i++) {

		nvt_mmc_host_reset(nvt_host);

		cmd.opcode = MMC_READ_SINGLE_BLOCK;
		cmd.arg = 0;
		cmd.flags = MMC_RSP_R1 | MMC_CMD_ADTC;/*MMC_CMD_AC*/

		stop.opcode = MMC_STOP_TRANSMISSION;
		stop.arg = 0;
		stop.flags = MMC_RSP_R1B | MMC_CMD_AC;

		sg_init_one(&sg, buf, ARRAY_SIZE(buf));

		mmcdata.blksz = ARRAY_SIZE(buf);
		mmcdata.blocks = 1;
		mmcdata.flags = MMC_DATA_READ;
		mmcdata.sg = &sg;
		mmcdata.sg_len = 1;

		mrq.cmd = &cmd;
		mrq.data = &mmcdata;

		mmc_wait_for_req(host, &mrq);

		mrq.cmd = &stop;
		mrq.data = NULL;
		mrq.stop = NULL;

	}

	pr_info("after read pattern, 0byte : %x, 1byte : %x\n", buf[0], buf[1]);

	/*restore*/
	part_config = card->ext_csd.part_config;
	part_config &= ~EXT_CSD_PART_CONFIG_ACC_MASK;
	part_config |= save_part;

	ret = mmc_switch(card, EXT_CSD_CMD_SET_NORMAL,
					EXT_CSD_PART_CONFIG, part_config,
					card->ext_csd.part_time);

	if (ret) {
		pr_err("%s boot part switch fail! to user\n", __func__);
		pr_debug("printk read pattern val_mem : 0x%llx\n", *val);
		pr_debug("printk read pattern save_part : 0x%x, part_config : 0x%x",
				save_part, part_config);
		return -1;
	}

	mmc_release_host(host);

	card->ext_csd.part_config = part_config;

	return 0;
}
DEFINE_SIMPLE_ATTRIBUTE(mmc_read_pattern_fops, mmc_read_pattern_get, NULL,
	"0x%08llx\n");

static int mmc_dbg_interrupt_status_get(void *data, u64 *val)
{

#define SDHCI_INT_STATUS	0x30
	/* we may need save interrupt first*/
	*val = readl(g_sdc_vbase+SDHCI_INT_STATUS) & ~(0xefff);
	/* 0xefff to clear correct interrupt status*/
	pr_info("nvt mmc interrupt status :  %llu\n",  (u64)*val);
	return 0;
}
DEFINE_SIMPLE_ATTRIBUTE(mmc_dbg_interrupt_status,
	mmc_dbg_interrupt_status_get, NULL, "%llx\n");

static int error_reboot;
static int mmc_error_reboot_get(void *data, u64 *val)
{
	*val =  error_reboot;
	return 0;
}

static int mmc_error_reboot_set(void *data, u64 val)
{
	error_reboot = val;
	pr_info("nvt mmc error_reboot %d\n", error_reboot);
	return 0;
}

DEFINE_SIMPLE_ATTRIBUTE(mmc_error_reboot_fops, mmc_error_reboot_get,
	mmc_error_reboot_set, "%llu\n");

void nvt_handle_irq_error(void)
{
	if (error_reboot)
		emergency_restart();
}

struct dentry *nvt_debugfs_root;
void mmc_add_nvt_host_debugfs(struct mmc_host *host)
{
	if (!host->debugfs_root)
		return;

	nvt_debugfs_root = debugfs_create_dir("nvt_mmc", host->debugfs_root);
	if (IS_ERR(nvt_debugfs_root))
		return;
	if (!nvt_debugfs_root)
		goto err;

#ifdef MMC_HS400_IO_DRIVING
	if (!debugfs_create_file("rdqs", S_IRUSR | S_IWUSR, nvt_debugfs_root,
			host, &mmc_rdqs_fops))
		goto err;

#ifdef CONFIG_MMC_SDHCI_NVT_HS400_RDQS_TUNING
	if (!debugfs_create_file("rdqs_tune", S_IRUSR, nvt_debugfs_root, host,
			&mmc_rdqs_tune_fops))
		goto err;
#endif

	if (!debugfs_create_file("clk_phase", S_IRUSR | S_IWUSR,
			nvt_debugfs_root, host, &mmc_write_phse_fops))
		goto err;
#endif

	if (!debugfs_create_file("strength_pmos", S_IRUSR | S_IWUSR,
			nvt_debugfs_root, host, &mmc_driver_strength_pmos_fops))
		goto err;

	if (!debugfs_create_file("strength_nmos", S_IRUSR | S_IWUSR,
			nvt_debugfs_root, host, &mmc_driver_strength_nmos_fops))
		goto err;

	if (!debugfs_create_file("readpattern", S_IRUSR, nvt_debugfs_root,
			host, &mmc_read_pattern_fops))
		goto err;

	if (!debugfs_create_file("writepattern", S_IRUSR | S_IWUSR,
			nvt_debugfs_root, host, &mmc_write_pattern_fops))
		goto err;

	if (!debugfs_create_file("error", S_IRUSR, nvt_debugfs_root, host,
			&mmc_dbg_interrupt_status))
		goto err;

	if (!debugfs_create_file("error_reboot", S_IRUSR | S_IWUSR,
			nvt_debugfs_root, host,	&mmc_error_reboot_fops))
		goto err;

	return;

err:
	debugfs_remove_recursive(nvt_debugfs_root);
	dev_err(&host->class_dev, "failed to initialize nvt_debugfs\n");

}
#endif
#endif

static void sdhci_nvt_platform_reset_enter(struct sdhci_host *host, u8 mask)
{
	struct sdhci_pltfm_host *pltfm_host = NULL;
	struct nvt_mmc_hwplat *hwplat = NULL;

	if (mask & SDHCI_RESET_ALL) {
		pr_info("*** %s:%d\n", __func__, __LINE__);
		pltfm_host = sdhci_priv(host);
		hwplat = pltfm_host->priv;
		/* sdhci layer have ioremap SD base,
			we need to keep it in our own for later use */
		hwplat->sdc_vbase = host->ioaddr;
		if (nvt_mmc_init(hwplat) != 0)
			pr_err("*** mmc hw reset failed\n");
	}
	sdhci_reset(host, mask);
}

static u64 mmc_dmamask = DMA_BIT_MASK(32);
static int nvt_mmc_parse_dt(struct nvt_mmc_hwplat *hwplat,
			struct platform_device *plt_dev)
{
	struct device *dev = &plt_dev->dev;
	struct device_node *node = dev->of_node;

	if (!hwplat || !plt_dev)
		return -1;

	/*node = of_find_compatible_node(NULL, NULL, "nvt,hsmmc");*/
	if (node) {
		struct sdhci_host *host = dev_get_drvdata(dev);

		/* get regs */
		/*of_address_to_resource(node, 0, &mmc_resources0[0]);*/
		hwplat->nfc_vbase = of_iomap(node, 1);
		hwplat->fcr_vbase = of_iomap(node, 2);

		/* get specific info of our controller */
		hwplat->src_clk = get_mmc_clk();
		of_property_read_u32(node, "max-bus-frequency",
			&hwplat->max_bus_clk);
		of_node_put(node);
		mmc_of_parse(host->mmc);
		if (of_property_read_u32(node, "clk-shift",
			&hwplat->host_clk_shift) < 0) {
			/*designer suggest result 0*/
			hwplat->host_clk_shift = 0x0;
		}
		pr_info("mmc clk-shift setting %x\n", hwplat->host_clk_shift);

#ifdef CONFIG_MMC_SDHCI_NVT_HS400
		if (of_property_read_u32(node, "adll-read-phase",
				&hwplat->host_adll_read_phase) < 0)
			hwplat->host_adll_read_phase = 0x5;

		if (of_property_read_u32(node, "adll-write-phase",
				&hwplat->host_adll_write_phase) < 0)
			hwplat->host_adll_write_phase = 0x1C;
#endif
		if (of_property_read_u32(node, "drv_clk",
				&hwplat->host_adll_write_phase) < 0)
			hwplat->emmc_iodrv.clk = DEFAULT_DRIVING;


		debug_log("*** [MMC] src_clk = %d, max bus clk = %d, irq= %d\n",
			hwplat->src_clk, hwplat->max_bus_clk, host->irq);
	} else {
		debug_log("*** [MMC] can not find mmc device tree node.\n");
	}

	return 0;
}

void sdhci_nvt_hs400_enhanced_strobe(struct mmc_host *host, struct mmc_ios *ios)
{
	void __iomem *fcr = _hwplat->fcr_vbase;

	if (ios->enhanced_strobe) {
		pr_info("mmc->enhanced_strobe enable\n");
		writel_or_bits((u32)(FCR_HS200_CTRL_EXTEND_STROBE |
		FCR_HS200_CTRL_EXTSTRB_LATCH), fcr + REG_FCR_HS200_CTRL);
	}
}

static struct sdhci_ops sdhci_nvt_ops = {
	.write_b = sdhci_nvt_writeb,
	.write_w = sdhci_nvt_writew,
	.write_l = sdhci_nvt_writel,
	.read_w = sdhci_nvt_readw,
	.get_max_clock = sdhci_nvt_get_max_clock,
	.get_min_clock = sdhci_nvt_get_min_clock,
	.get_timeout_clock = sdhci_nvt_get_timeout_clock,
	.set_clock = sdhci_nvt_set_clock,
	.set_bus_width = sdhci_nvt_platform_bus_width,
	.reset = sdhci_nvt_platform_reset_enter,
};

static struct sdhci_pltfm_data sdhci_nvt_pdata = {
/* some register setting must be defined to make NVT controller happy */
	       .ops    = &sdhci_nvt_ops,
	       .quirks =
#ifndef CONFIG_NVT_MMC_REDUCE_DELAY
		/* this will be several millisecond delay after bus pwr on */
		SDHCI_QUIRK_DELAY_AFTER_POWER |
#endif
	/* we want to fine tune controller's behaviors, set this */
		SDHCI_QUIRK_MISSING_CAPS |
		SDHCI_QUIRK_FORCE_DMA | /* always use DMA to transfer DATA */
	/* we have our own clk scheme, not te same as standard SD host spec */
		SDHCI_QUIRK_CAP_CLOCK_BASE_BROKEN |
	/* we do not support zero length descriptor in ADMA table */
		SDHCI_QUIRK_BROKEN_ADMA_ZEROLEN_DESC |
	/* use max block size as 2K as default value, can fine fune */
		SDHCI_QUIRK_FORCE_BLK_SZ_2048 |
	/* we ask the very last descriptor must be valid in ADMA table */
		SDHCI_QUIRK_NO_ENDATTR_IN_NOPDESC
#ifdef CONFIG_MMC_SDHCI_NVT_OPEN_ENDED_RW
	/* enable AUTO CMD12,
		might save a little time for issuing separate CMD12 */
		| SDHCI_QUIRK_MULTIBLOCK_READ_ACMD12
#endif
};

static const struct of_device_id sdhci_nvt_dt_match[] = {
	{ .compatible = "nvt,hsmmc", .data = &sdhci_nvt_pdata },
	{},
};

MODULE_DEVICE_TABLE(of, sdhci_nvt_dt_match);
static int sdhci_nvt_probe(struct platform_device *pdev)
{
	const struct of_device_id *match;
	struct sdhci_host *host = NULL;
	struct sdhci_pltfm_host *pltfm_host = NULL;
	struct nvt_mmc_hwplat *hwplat = NULL;
	int ret = 0;

	match = of_match_device(sdhci_nvt_dt_match, &pdev->dev);
	if (!match) {
		dev_err(&pdev->dev, "[MMC] OF not found\n");
		return -EINVAL;
	}

	host = sdhci_pltfm_init(pdev, &sdhci_nvt_pdata, 0);
	if (IS_ERR(host)) {
		dev_err(&pdev->dev, "[MMC] sdhci plat init fail\n");
		ret = PTR_ERR(host);
		goto err_sdhci_pltfm_init;
	}

	if (pdev->dev.platform_data) {
		dev_err(&pdev->dev,
			"nvt mmc platform data should be NULL when initial\n");
		goto err_sdhci_pltfm_init;
	}

#ifdef CONFIG_MMC_SDHCI_NVT_OPEN_ENDED_RW
	/* disable CMD23, always use CMD12 to stop multiple block read/write */
	host->quirks2 |= SDHCI_QUIRK2_HOST_NO_CMD23;
#endif
	host->caps |= SDHCI_CAN_DO_ADMA2 |
		      SDHCI_CAN_DO_HISPD |
		      SDHCI_CAN_DO_8BIT  |
		      SDHCI_CAN_DO_SDMA  |
		      SDHCI_CAN_VDD_330  |
		      SDHCI_CAN_VDD_180
		     ;

#ifdef CONFIG_MMC_SDHCI_NVT_PIO_MODE
	host->caps &= (u32)(~(SDHCI_CAN_DO_ADMA2 | SDHCI_CAN_DO_SDMA));
	host->quirks &= (u32)(~(SDHCI_QUIRK_FORCE_DMA));
#endif

	host->mmc->caps |= MMC_CAP_NONREMOVABLE  |
			   MMC_CAP_8_BIT_DATA    |
			   MMC_CAP_MMC_HIGHSPEED |
			   MMC_CAP_UHS_DDR50	 |
			   MMC_CAP_1_8V_DDR
			;
#ifdef CONFIG_MMC_SDHCI_NVT_ENABLE_DEV_CACHE
	host->mmc->caps2 |= MMC_CAP2_CACHE_CTRL;
#endif
#ifdef CONFIG_MMC_SDHCI_NVT_HS200
	host->mmc->caps2 |= MMC_CAP2_HS200_1_8V_SDR |
			    MMC_CAP2_HS200
			;
#endif
#ifdef CONFIG_MMC_SDHCI_NVT_HS400
	host->mmc->caps2 |= MMC_CAP2_HS400;
#endif
	pltfm_host = sdhci_priv(host);

	hwplat  = pdev->dev.platform_data =
	devm_kzalloc(&pdev->dev, sizeof(struct nvt_mmc_hwplat), GFP_KERNEL);

	if (!hwplat) {
		ret = -ENOMEM;
		goto err_sdhci_add;
	}
	nvt_mmc_parse_dt(hwplat, pdev);
	pltfm_host->priv = hwplat;
	hwplat->rdqs_tune = MMC_RDQS_TUNE_DISABLED;

	/* set platform  device information*/
	pdev->dev.dma_mask = &mmc_dmamask;
	pdev->dev.coherent_dma_mask = DMA_BIT_MASK(32);

	ret = sdhci_add_host(host);
	if (ret)
		goto err_sdhci_add;

	debug_log("[MMC] add NVT host\n");

#ifdef CONFIG_DEBUG_FS
#ifdef NVT_MMC_DEBUGFS
	mmc_add_nvt_host_debugfs(host->mmc);
#endif
#endif

	return 0;

err_sdhci_add:
	sdhci_pltfm_free(pdev);
err_sdhci_pltfm_init:
	return ret;
}

static int sdhci_nvt_remove(struct platform_device *pdev)
{
	       struct sdhci_host *host = platform_get_drvdata(pdev);
	       struct sdhci_pltfm_host *pltfm_host = sdhci_priv(host);
	       struct nvt_mmc_hwplat *hwplat = pltfm_host->priv;

	nvt_mmc_deinit(hwplat);
	devm_kfree(&pdev->dev, hwplat);
	pdev->dev.platform_data = NULL;
	       sdhci_pltfm_unregister(pdev);

	       return 0;
}

static struct platform_driver sdhci_nvt_driver = {
	.driver         = {
		.name   = "sdhci_nvt",
		.owner  = THIS_MODULE,
		.pm     = SDHCI_PLTFM_PMOPS,
		.of_match_table = sdhci_nvt_dt_match,
	},
	.probe          = sdhci_nvt_probe,
	.remove         = sdhci_nvt_remove,
};

#ifdef CONFIG_EMRG_SAVE_KLOG
#include <linux/vmalloc.h>
#include <linux/slab.h>
#include <linux/scatterlist.h>
#include <linux/highmem.h>
#include <linux/dma-mapping.h>
#include <linux/mmc/mmc.h>

static void nvt_sdhci_reset(struct sdhci_host *host, u8 mask)
{
	unsigned long timeout;

	sdhci_writeb(host, mask, SDHCI_SOFTWARE_RESET);

	/* Wait max 100 ms */
	timeout = 100;

	/* hw clears the bit when it's done */
	while (sdhci_readb(host, SDHCI_SOFTWARE_RESET) & mask) {
		if (timeout == 0) {
			pr_err("%s: Reset 0x%x never completed.\n",
				mmc_hostname(host->mmc), (int)mask);
			/*sdhci_dumpregs(host);*/
			return;
		}
		timeout--;
		mdelay(1);
	}
}

static void nvt_sdhci_tasklet_finish(unsigned long param)
{
	struct sdhci_host *host;
	unsigned long flags;
	struct mmc_request *mrq;

	host = (struct sdhci_host *)param;
	spin_lock_irqsave(&host->lock, flags);
	if (!host->mrq) {
		spin_unlock_irqrestore(&host->lock, flags);
		return;
	}

	del_timer(&host->timer);

	mrq = host->mrq;

	if (!(host->flags & SDHCI_DEVICE_DEAD) &&
	    ((mrq->cmd && mrq->cmd->error) ||
		 (mrq->data && (mrq->data->error ||
		  (mrq->data->stop && mrq->data->stop->error))) ||
		   (host->quirks & SDHCI_QUIRK_RESET_AFTER_REQUEST))) {
		/* Spec says we should do both at the same time, but Ricoh
		   controllers do not like that. */
		nvt_sdhci_reset(host, SDHCI_RESET_CMD);
		nvt_sdhci_reset(host, SDHCI_RESET_DATA);
	}

	host->mrq = NULL;
	host->cmd = NULL;
	host->data = NULL;

	mmiowb();
	spin_unlock_irqrestore(&host->lock, flags);

	mmc_request_done(host->mmc, mrq);
}

static void nvt_mmc_prepare_mrq(struct mmc_card	*card,
	struct mmc_request *mrq, struct scatterlist *sg, unsigned sg_len,
	unsigned dev_addr, unsigned blocks, unsigned blksz, int write)
{
	BUG_ON(!mrq || !mrq->cmd || !mrq->data || !mrq->stop);

	if (blocks > 1) {
		mrq->cmd->opcode = write ?
			MMC_WRITE_MULTIPLE_BLOCK : MMC_READ_MULTIPLE_BLOCK;
	} else {
		mrq->cmd->opcode = write ?
			MMC_WRITE_BLOCK : MMC_READ_SINGLE_BLOCK;
	}
	mrq->cmd->arg = dev_addr;
	mrq->cmd->flags = MMC_RSP_R1 | MMC_CMD_ADTC;
	if (!mmc_card_blockaddr(card))
		mrq->cmd->arg <<= 9;

	if (blocks == 1)
		mrq->stop = NULL;
	else {
		mrq->stop->opcode = MMC_STOP_TRANSMISSION;
		mrq->stop->arg = 0;
		if (write)
			mrq->stop->flags = MMC_RSP_R1B | MMC_CMD_AC;
		else
			mrq->stop->flags =  MMC_RSP_R1 | MMC_CMD_AC;
	}

	mrq->data->blksz = blksz;
	mrq->data->blocks = blocks;
	mrq->data->flags = write ? MMC_DATA_WRITE : MMC_DATA_READ;
	mrq->data->sg = sg;
	mrq->data->sg_len = sg_len;

	mmc_set_data_timeout(mrq->data, card);
}

void nvt_mmc_start_request(struct mmc_host *host, struct mmc_request *mrq)
{
	/*void sdhci_send_command(struct sdhci_host *host,
		struct mmc_command *cmd);*/
	struct sdhci_host *nvt_host = mmc_priv(host);

	WARN_ON(!host->claimed);
	mrq->cmd->error = 0;
	mrq->cmd->mrq = mrq;
	if (mrq->data) {
		BUG_ON(mrq->data->blksz > host->max_blk_size);
		BUG_ON(mrq->data->blocks > host->max_blk_count);
		BUG_ON(mrq->data->blocks * mrq->data->blksz >
			host->max_req_size);

		mrq->cmd->data = mrq->data;
		mrq->data->error = 0;
		mrq->data->mrq = mrq;
		if (mrq->stop) {
			mrq->data->stop = mrq->stop;
			mrq->stop->error = 0;
			mrq->stop->mrq = mrq;
		}
	}
	/*sdhci_request*/
	WARN_ON(nvt_host->mrq != NULL);

	if (!mrq->sbc && (nvt_host->flags & SDHCI_AUTO_CMD12)) {
		if (mrq->stop) {
			mrq->data->stop = NULL;
			mrq->stop = NULL;
		}
	   }
	nvt_host->mrq = mrq;
	sdhci_send_command(nvt_host, mrq->cmd);
}

#define CMD_ERRORS							\
	(R1_OUT_OF_RANGE |	/* Command argument out of range */	\
	 R1_ADDRESS_ERROR |	/* Misaligned address */		\
	 R1_BLOCK_LEN_ERROR |	/* Transferred block length incorrect */\
	 R1_WP_VIOLATION |	/* Tried to write to protected block */	\
	 R1_CC_ERROR |		/* Card controller error */		\
	 R1_ERROR)		/* General/unknown error */
static void nvt_sdhci_cmd_irq(struct sdhci_host *host)
{
	debug_log("%s Enter\n", __func__);
	if (!host->cmd) {
		pr_err("no command operation was in progress.\n");
		return;
	}

	while (!(SDHCI_INT_RESPONSE & (sdhci_readl(host, SDHCI_INT_STATUS)))) {
		debug_log("mmc intr status 1: %x\n",
			sdhci_readl(host, SDHCI_INT_STATUS));
		if (SDHCI_INT_ERROR & (sdhci_readl(host, SDHCI_INT_STATUS))) {
			host->cmd->error = -EILSEQ;
			return;
		}
	}

	sdhci_writel(host, SDHCI_INT_RESPONSE, SDHCI_INT_STATUS);

	/*sdhci_finish_command(host);*/
	if (host->cmd->flags & MMC_RSP_PRESENT) {
		if (host->cmd->flags & MMC_RSP_136) {
			int i = 0;
			/* CRC is stripped so we need to do some shifting. */
			for (i = 0; i < 4; i++) {
				host->cmd->resp[i] = sdhci_readl(host,
					SDHCI_RESPONSE + (3-i)*4) << 8;
				if (i != 3)
					host->cmd->resp[i] |=
						sdhci_readb(host,
						SDHCI_RESPONSE + (3-i)*4-1);
			}
		} else {
			host->cmd->resp[0] = sdhci_readl(host, SDHCI_RESPONSE);
		}
	}

	if (host->cmd->resp[0] & CMD_ERRORS) {/*CMD_ERRORS 0XFDF90008*/
		host->cmd->error = -EILSEQ;
		return;
	}

	host->cmd->error = 0;
	host->cmd = NULL;
	debug_log("%s Exit\n", __func__);
}

static void nvt_sdhci_adma_table_post(struct sdhci_host *host,
	struct mmc_data *data)
{
#ifdef CONFIG_NVT_MMC_IF
#define ADMA_MAX_SEGS		(32)
#else
#define ADMA_MAX_SEGS		(128)
#endif
	int direction;

	struct scatterlist *sg;
	int i, size;
	u8 *align;
	char *buffer;
	unsigned long flags;

	if (data->flags & MMC_DATA_READ)
		direction = DMA_FROM_DEVICE;
	else
		direction = DMA_TO_DEVICE;

	dma_unmap_single(mmc_dev(host->mmc), host->adma_addr,
		(128 * 2 + 1) * 4, DMA_TO_DEVICE);

	dma_unmap_single(mmc_dev(host->mmc), host->align_addr,
		ADMA_MAX_SEGS * 4, direction);

	if (data->flags & MMC_DATA_READ) {
		dma_sync_sg_for_cpu(mmc_dev(host->mmc), data->sg,
			data->sg_len, direction);

		align = host->align_buffer;

		for_each_sg(data->sg, sg, host->sg_count, i) {
			if (sg_dma_address(sg) & 0x3) {
				size = 4 - (sg_dma_address(sg) & 0x3);

				/*buffer = sdhci_kmap_atomic(sg, &flags);*/
				local_irq_save(flags);
				buffer = (char *)(kmap_atomic(
						sg_page(sg)) + sg->offset);

				/*WARN_ON(((long)buffer & PAGE_MASK)
					> (PAGE_SIZE - 3));*/
				memcpy(buffer, align, size);

				/*sdhci_kunmap_atomic(buffer, &flags);*/
				kunmap_atomic(buffer);
				local_irq_restore(flags);

				align += 4;
			}
		}
	}

	dma_unmap_sg(mmc_dev(host->mmc), data->sg,
		data->sg_len, direction);
}
static void nvt_sdhci_data_irq(struct sdhci_host *host)
{
	struct mmc_data *data;

	debug_log("%s Enter\n", __func__);
	if (!host->data) {
		pr_info("no data operation was in progress.\n");
		return;
	}

	while (!(SDHCI_INT_DATA_END & (sdhci_readl(host, SDHCI_INT_STATUS)))) {
		debug_log("mmc  intr status 2: %x\n",
			sdhci_readl(host, SDHCI_INT_STATUS));
		if (SDHCI_INT_ERROR & (sdhci_readl(host, SDHCI_INT_STATUS)))
			host->data->error = -EILSEQ;
	}
	sdhci_writel(host, SDHCI_INT_DATA_END, SDHCI_INT_STATUS);

	/*sdhci_finish_data(host);*/
	data = host->data;
	host->data = NULL;

	if (host->flags & SDHCI_REQ_USE_DMA) {
		if (host->flags & SDHCI_USE_ADMA)
			nvt_sdhci_adma_table_post(host, data);
		else {
			dma_unmap_sg(mmc_dev(host->mmc), data->sg,
				data->sg_len, (data->flags & MMC_DATA_READ) ?
					DMA_FROM_DEVICE : DMA_TO_DEVICE);
		}
	}
	if (data->error)
		data->bytes_xfered = 0;
	else
		data->bytes_xfered = data->blksz * data->blocks;

	/*
	 * Need to send CMD12 if -
	 * a) open-ended multiblock transfer (no CMD23)
	 * b) error in multiblock transfer
	 */
	if (data->stop &&
	    (data->error ||
	     !host->mrq->sbc)) {

		/*
		 * The controller needs a reset of internal state machines
		 * upon error conditions.
		 */
		if (data->error) {
			nvt_sdhci_reset(host, SDHCI_RESET_CMD);
			nvt_sdhci_reset(host, SDHCI_RESET_DATA);
		}
		sdhci_send_command(host, data->stop);
		nvt_sdhci_cmd_irq(host);
	}
	debug_log("%s Exit\n", __func__);

}

void nvt_mmc_wait_for_req_rw(struct mmc_host *_host, struct mmc_request *mrq)
{
	struct sdhci_host *host = mmc_priv(_host);

	nvt_mmc_start_request(_host, mrq);

	nvt_sdhci_cmd_irq(host);
	nvt_sdhci_data_irq(host);

	nvt_sdhci_tasklet_finish((unsigned long)host);
}

static int nvt_mmc_wait_busy(struct mmc_card *card)
{
	int ret;
	struct sdhci_host *nvt_host = mmc_priv(card->host);
	struct mmc_request mrq = {NULL};
	struct mmc_command cmd = {0};

	debug_log("%s Enter\n", __func__);

	do {
		cmd.opcode = MMC_SEND_STATUS;
		cmd.arg = card->rca << 16;
		cmd.flags = MMC_RSP_R1 | MMC_CMD_AC;

		memset(cmd.resp, 0, sizeof(cmd.resp));
		cmd.retries = 0;
		mrq.cmd = &cmd;
		cmd.data = NULL;

		nvt_mmc_start_request(card->host, &mrq);
		nvt_sdhci_cmd_irq(nvt_host);
		ret = cmd.error;
		nvt_sdhci_tasklet_finish((unsigned long)nvt_host);
		if (ret)
			break;
	} while (!(cmd.resp[0] & R1_READY_FOR_DATA));
	debug_log("%s Exit\n", __func__);
	return ret;
}

#define NR_SG 0x400
#define ALIGH_MASK	(32/8-1)
int nvt_mmc_rw_request(struct mmc_host *host, u32 blk_addr,
		u32 blk_count, u8 *buf, bool is_write)
{
	static struct scatterlist sg[NR_SG];
	const unsigned int sgnum = (blk_count/(host->max_seg_size/512))
		+(blk_count%(host->max_seg_size/512)?1:0);

	int i;
	struct mmc_card *card = host->card;
	struct mmc_request mrq = {0};
	struct mmc_command cmd = {0};
	struct mmc_command stop = {0};
	struct mmc_data data = {0};
	struct sdhci_host *nvt_mmch = mmc_priv(host);

	unsigned int seg_size = host->max_seg_size & ~(ALIGH_MASK);

	pr_info("mmc_read_write_block Enter (");
	pr_info("buf: %p start=%x blcokcount=%d.is_write=%d)\n",
		buf, blk_addr, blk_count, is_write);
	pr_info("nvt_mmc_: in_irq(%lx), in_softirq(%lx),",
			in_irq(), in_softirq());
	pr_info("in_interrupt(%lx), in_serving_softirq(%lx)\n",
		in_interrupt(), in_serving_softirq());

	mrq.cmd = &cmd;
	mrq.data = &data;
	mrq.stop = &stop;

	sg_init_table(sg, sgnum);
	for (i = 0; i < sgnum-1; i++)
		sg_set_buf(sg+i, buf+(seg_size * i), seg_size);

	sg_set_buf(sg+i, buf+(seg_size * (i)), (blk_count*512)-(seg_size*(i)));

	nvt_mmc_prepare_mrq(card, &mrq, sg, sgnum, blk_addr,
			blk_count, 512, is_write);

	if (host->claimed) {
		pr_info("%s: forced release host->claimed\n",
			mmc_hostname(nvt_mmch->mmc));
		host->claimed = 0;
	}
	if (spin_is_locked(&nvt_mmch->lock)) {
		pr_info("%s: forced unlock mmc->lock\n",
			mmc_hostname(nvt_mmch->mmc));
		spin_unlock(&nvt_mmch->lock);
	}
	if (nvt_mmch->mrq != NULL) {
		pr_info("%s : host mrq is not finish force clear\n", __func__);
		nvt_sdhci_tasklet_finish((unsigned long)nvt_mmch);
	}

	 /*nvt_sdhci_reset(nvt_mmch, SDHCI_RESET_ALL);*/
	/*we don't dependo on interrupt*/
	disable_irq(nvt_mmch->irq);
	tasklet_disable(&nvt_mmch->finish_tasklet);
	del_timer(&nvt_mmch->timer);

	host->claimed = 1;

	nvt_mmc_wait_for_req_rw(host, &mrq);

	if (cmd.error) {
		pr_err("%s cmd.error %x\n", __func__, cmd.error);
		return cmd.error;
	}

	if (data.error) {
		pr_err("%s data.error %x\n", __func__, data.error);
		return data.error;
	}

	if (stop.error) {
		pr_err("%s stop.error %x\n", __func__, stop.error);
		return stop.error;
	}
	nvt_mmc_wait_busy(card);

	enable_irq(nvt_mmch->irq);
	tasklet_enable(&nvt_mmch->finish_tasklet);
	host->claimed = 0;

	return 0;
}
EXPORT_SYMBOL(nvt_mmc_rw_request);

int nvt_mmc_read_write_log(struct mmc_host *host, u32 blk_addr,
		u32 blk_count, u8 *buf, bool is_write)
{
	int ret;

	char *content_buf = kmalloc(blk_count*512, GFP_KERNEL);
	int i = 0;

	memset(content_buf, 0x0, blk_count*512);
	/*
	for(i = 0; i <blk_count*512; i++){
		pr_info("[%x]", (unsigned int)buf[i]);
		if ((i%32) == 0 && (i!=0))
			printk("\n");
	}
	*/
	ret = nvt_mmc_rw_request(host, blk_addr, blk_count, buf, true);
	ret = nvt_mmc_rw_request(host, blk_addr, blk_count, content_buf, false);

	for (i = 0; i < blk_count*512; i++) {
		if (buf[i] != content_buf[i]) {
			pr_err("the %d data is not correct %x %x\n",
				i, buf[i], content_buf[i]);
			break;
		}
		/*
		else{
			printk("[%x]",(unsigned int)content_buf[i]);
		}
		*/
	}

	if (i == blk_count*512)
		pr_info("====>w/r emerg_log test correct\n");
	else
		pr_err("====>w/r emerg_log test wrong at %d\n", i);

	kfree(content_buf);
	return ret;
}
EXPORT_SYMBOL(nvt_mmc_read_write_log);
#endif

module_platform_driver(sdhci_nvt_driver);

MODULE_DESCRIPTION("SDHCI driver for NVT");
MODULE_AUTHOR("Novatek Corp.");
MODULE_LICENSE("GPL v2");
