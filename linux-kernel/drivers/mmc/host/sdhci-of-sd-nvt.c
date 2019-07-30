#include <linux/err.h>
#include <linux/io.h>
#include <linux/module.h>
#include <linux/of.h>
#include <linux/of_address.h>
#include <linux/of_fdt.h>
#include <linux/of_irq.h>
#include <linux/of_platform.h>
#include <linux/delay.h>
#include <linux/debugfs.h>
#include <linux/fs.h>
#include <linux/reboot.h>

#include "sdhci.h"
#include "sdhci-pltfm.h"
#include "linux/dma-mapping.h"
#include "nvt_sdc.h"
#include "nvt_stbc.h"
#include "soc/nvt/clk.h"

static int sd_det_pin;
static bool need_sd_gpio_det;

/* #define NVT_MMC_HS200_TRACE_EACH_BLK
 * //don't enable.... this may cause data-crc error on 658 */
/* #define NVT_MMC_SUPPORT_POWER_OFF_NOTIFICATION */
/* #define NVT_MMC_SUPPORT_PACKED_CMD */
/* #define NVT_SECURE_LOCK_SET_DISABLE */
/* #define CONFIG_NVT_MMC_GATING_EXTERNAL_ONLY
 * //New clk-gating method without changing clk frequency */
#define NVT_MMC_DEBUGFS
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




static void nvt_sd_deinit(struct nvt_mmc_hwplat *hwplat)
{
	iounmap(hwplat->key_ctrl);
#ifdef NVT_SECURE_LOCK_SET_DISABLE
	iounmap(hwplat->secure_lock_ctrl);
#endif
	if (hwplat->id == 1) {
		debug_log("[MMC1] deinit\n");
		iounmap(hwplat->pinmux_ctrl);
		iounmap(hwplat->sd_func_pin_mux);

		iounmap(hwplat->sd_pwr_gpio_mux);
		iounmap(hwplat->sd_pwr_gpio_dir);
		iounmap(hwplat->sd_pwr_gpio_val);
		iounmap(hwplat->sd_tcl_wp_mux);
		iounmap(hwplat->sd_tcl_wp_dir);
		iounmap(hwplat->sd_tcl_wp_val);

		if (need_sd_gpio_det) {
			iounmap(hwplat->sd_cd_pin_mux);
			iounmap(hwplat->sd_cd_pin_dir);
			iounmap(hwplat->sd_cd_pin_val);
			iounmap(hwplat->sd_cd_rising_int_en);
			iounmap(hwplat->sd_cd_falling_int_en);
			iounmap(hwplat->sd_cd_rising_int_sts);
			iounmap(hwplat->sd_cd_falling_int_sts);
			iounmap(hwplat->sd_cd_fcr_ctrl);
		}


	}
}


#if defined(CONFIG_MMC_SDHCI_NVT_SDCARD)
static int nvt_sd_pre_init(struct nvt_mmc_hwplat *hwplat)
{
	unsigned int reg_val;

	hwplat->key_ctrl = ioremap_nocache(NVT_REG_PHY_KEY_CTRL,
		sizeof(hwplat->key_ctrl));

	writel(NVT_CTRL_KEY1, hwplat->key_ctrl);
	writel(NVT_CTRL_KEY2, hwplat->key_ctrl);
	writel(MAGIC_KEY, hwplat->key_ctrl + OFFSET_LONG_TERM_KEY);
	writel(MAGIC_AUTH, hwplat->key_ctrl + OFFSET_AUTH);
	if (!hwplat->key_ctrl) {
		pr_err("key_ctrl(%p)remap failed", hwplat->key_ctrl);
		return -1;
	}

	/*switch sd pinmux to function */
	hwplat->sd_func_pin_mux = ioremap_nocache(NVT_REG_SD_PIN_MUX,
		sizeof(hwplat->sd_func_pin_mux));

	if (!hwplat->sd_func_pin_mux) {
		info_log("sd_func_pin_mux(%p) remap fail !\n",
			hwplat->sd_func_pin_mux);
		return -1;
	}

	reg_val = readl(hwplat->sd_func_pin_mux);
#if (defined(CONFIG_ARCH_NVT72563))
		reg_val &= ~(NVT_SD_D0_FUNC_BIT | NVT_SD_D1_FUNC_BIT |
			NVT_SD_D2_FUNC_BIT | NVT_SD_D3_FUNC_BIT);
#else
	if (need_sd_gpio_det)
		reg_val &= ~(NVT_SD_CLK_FUNC_BIT | NVT_SD_D0_FUNC_BIT    |
		NVT_SD_D1_FUNC_BIT       | NVT_SD_D2_FUNC_BIT    |
		NVT_SD_D3_FUNC_BIT       | NVT_SD_CMD_FUNC_BIT);
	else
		reg_val &= ~(NVT_SD_CLK_FUNC_BIT | NVT_SD_CD_FUNC_BIT    |
			NVT_SD_D0_FUNC_BIT |	NVT_SD_D1_FUNC_BIT       |
			NVT_SD_D2_FUNC_BIT    | NVT_SD_D3_FUNC_BIT       |
			NVT_SD_CMD_FUNC_BIT);
#endif

	writel(reg_val, hwplat->sd_func_pin_mux);

	if (need_sd_gpio_det) {
		hwplat->sd_cd_pin_mux = ioremap_nocache(NVT_REG_SD_CD_PIN_MUX,
			sizeof(hwplat->sd_cd_pin_mux));
		hwplat->sd_cd_pin_dir = ioremap_nocache(NVT_REG_SD_CD_PIN_DIR,
			sizeof(hwplat->sd_cd_pin_dir));
		hwplat->sd_cd_pin_val = ioremap_nocache(NVT_REG_SD_CD_PIN_VAL,
			sizeof(hwplat->sd_cd_pin_val));
		hwplat->sd_cd_rising_int_en =
		    ioremap_nocache(NVT_REG_SD_CD_RISING_INT_EN,
			sizeof(hwplat->sd_cd_rising_int_en));
		hwplat->sd_cd_falling_int_en =
		    ioremap_nocache(NVT_REG_SD_CD_FALLING_INT_EN,
		    sizeof(hwplat->sd_cd_falling_int_en));
		hwplat->sd_cd_rising_int_sts =
		    ioremap_nocache(NVT_REG_SD_CD_RISING_INT_STS,
		    sizeof(hwplat->sd_cd_rising_int_sts));
		hwplat->sd_cd_falling_int_sts =
		    ioremap_nocache(NVT_REG_SD_CD_FALLING_INT_STS,
		    sizeof(hwplat->sd_cd_falling_int_sts));
		hwplat->sd_cd_fcr_ctrl =
		    ioremap_nocache(NVT_REG_SD_FCR_CTRL,
		    sizeof(hwplat->sd_cd_fcr_ctrl));

	if ((!hwplat->sd_cd_pin_mux) || (!hwplat->sd_cd_pin_dir) ||
	   (!hwplat->sd_cd_pin_val) || (!hwplat->sd_cd_rising_int_en) ||
	   (!hwplat->sd_cd_falling_int_en) || (!hwplat->sd_cd_rising_int_sts) ||
	   (!hwplat->sd_cd_falling_int_sts) || (!hwplat->sd_cd_fcr_ctrl)) {
		pr_info("sd_cd_pin_mux(%p), sd_cd_pin_dir(%p), sd_cd_pin_val(%p),sd_cd_rising_int_en(%p), sd_cd_falling_int_en(%p), sd_cd_rising_int_sts(%p), sd_cd_falling_int_sts(%p) sd_cd_fcr_ctrl(%p) remap fail!\n",
		hwplat->sd_cd_pin_mux, hwplat->sd_cd_pin_dir,
		hwplat->sd_cd_pin_val, hwplat->sd_cd_rising_int_en,
		hwplat->sd_cd_falling_int_en, hwplat->sd_cd_rising_int_sts,
		hwplat->sd_cd_falling_int_sts, hwplat->sd_cd_fcr_ctrl);
		return -1;
		}

		/* hwplat->sd_cd_irq = NVT_SD_CD_IRQ; */

		/* switch cd pin as gpio */
		reg_val = readl(hwplat->sd_cd_pin_mux);
		reg_val |= sd_det_pin;
		writel(reg_val, hwplat->sd_cd_pin_mux);

		/* direction input */
		reg_val = readl(hwplat->sd_cd_pin_dir);
		reg_val &= ~sd_det_pin;
		writel(reg_val, hwplat->sd_cd_pin_dir);

		/* enable sw cd function */
		reg_val = readl(hwplat->sd_cd_fcr_ctrl);
		reg_val |= NVT_SD_SW_CD_ENABLE;
		writel(reg_val, hwplat->sd_cd_fcr_ctrl);
	}

	hwplat->sd_pwr_gpio_mux = ioremap_nocache(NVT_REG_SD_PWR_MUX,
		sizeof(hwplat->sd_pwr_gpio_mux));
	hwplat->sd_pwr_gpio_dir = ioremap_nocache(NVT_REG_SD_PWR_DIR,
		sizeof(hwplat->sd_pwr_gpio_dir));
	hwplat->sd_pwr_gpio_val = ioremap_nocache(NVT_REG_SD_PWR_VAL,
		sizeof(hwplat->sd_pwr_gpio_val));
	hwplat->sd_tcl_wp_mux = ioremap_nocache(NVT_TCL_CN_WP_MUX,
		sizeof(hwplat->sd_tcl_wp_mux));
	hwplat->sd_tcl_wp_dir = ioremap_nocache(NVT_TCL_CN_WP_DIR,
		sizeof(hwplat->sd_tcl_wp_dir));
	hwplat->sd_tcl_wp_val = ioremap_nocache(NVT_TCL_CN_WP_REG_VAL,
		sizeof(hwplat->sd_tcl_wp_val));

	if ((!hwplat->sd_pwr_gpio_mux) || (!hwplat->sd_pwr_gpio_dir) ||
		(!hwplat->sd_pwr_gpio_val) || (!hwplat->sd_tcl_wp_mux) ||
		(!hwplat->sd_tcl_wp_dir) || (!hwplat->sd_tcl_wp_val)) {
		pr_info("sd_pwr_gpio_mux(%p), sd_pwr_gpio_dir(%p), sd_pwr_gpio_val(%p), sd_tcl_wp_mux(%p),sd_tcl_wp_dir(%p),sd_tcl_wp_val(%p) remap fail !\n",
		hwplat->sd_pwr_gpio_mux,
		hwplat->sd_pwr_gpio_dir, hwplat->sd_pwr_gpio_val,
		hwplat->sd_tcl_wp_mux, hwplat->sd_tcl_wp_dir,
		hwplat->sd_tcl_wp_val);
		return -1;
	}

	/* enable sd card slot power */
	/* set pin as gpio */
	reg_val = readl(hwplat->sd_pwr_gpio_mux);
	reg_val |= NVT_SD_PWR_ENABLE;
#if defined(CONFIG_MACH_EVB658_NEW_DRV) || \
	defined(CONFIG_MACH_N69001_NEW_DRV) \
	|| defined(CONFIG_MACH_EVB458_NAND)
	/* set bit[10]~[13] each for SD D0~D3 input */
	reg_val &= ~(15<<10);
#endif
#if defined(CONFIG_ARCH_NVT72563)
	reg_val &= ~((1<<12) | (1<<26));
#endif
	writel(reg_val, hwplat->sd_pwr_gpio_mux);

	/* set direction output */
	reg_val = readl(hwplat->sd_pwr_gpio_dir);
	reg_val |= NVT_SD_PWR_ENABLE;
#if defined(CONFIG_ARCH_NVT72563)
	reg_val &= ~((1<<12) | (1<<26));
#endif
	writel(reg_val, hwplat->sd_pwr_gpio_dir);

	/* set output low to enable sd card power */
	reg_val = readl(hwplat->sd_pwr_gpio_val);
	reg_val &= ~NVT_SD_PWR_ENABLE;
	writel(reg_val, hwplat->sd_pwr_gpio_val);

	/* set gph 23 as input for sd_wp */
	reg_val = readl(hwplat->sd_tcl_wp_mux);
#if defined(CONFIG_ARCH_NVT72563)
	reg_val &= ~(1 << NVT_TCL_CN_WP_OFS);
#elif defined(CONFIG_ARCH_NVT72670)

	reg_val |= (1 << NVT_TCL_CN_WP_OFS);
#else
	reg_val &= ~(1 << 17);
#endif
	writel(reg_val, hwplat->sd_tcl_wp_mux);

	reg_val = readl(hwplat->sd_tcl_wp_dir);
	reg_val |= (1 << NVT_TCL_CN_WP_OFS);
	writel(reg_val, hwplat->sd_tcl_wp_dir);

	reg_val = readl(hwplat->sd_tcl_wp_val);
	reg_val &= ~(1 << NVT_TCL_CN_WP_OFS);
	writel(reg_val, hwplat->sd_tcl_wp_val);

	return 0;
}

static int nvt_sd_init(struct nvt_mmc_hwplat *hwplat)
{
	/* TODO: use normal card detection */

	void __iomem *fcr = NULL;

	if (!hwplat) {
		pr_err("[MMC1] hwplat is NULL\n");
		return -1;
	}

	if (!hwplat->fcr_vbase) {
		pr_err("[MMC1] hwplat->fcr_vbase is NULL\n");
		return -1;
	}

	fcr = hwplat->fcr_vbase;

	hwplat->pinmux_ctrl = ioremap_nocache(NVT_REG_MMC1_PIN_MUX,
		sizeof(hwplat->pinmux_ctrl));
	if (!hwplat->pinmux_ctrl) {
		pr_err("[MMC1] pinmux_ctlr map failed.\n");
		return -1;
	}

	/* switch I2C function to mmc1 function */
	writel(NVT_CTRL_KEY1, hwplat->key_ctrl);
	writel(NVT_CTRL_KEY2, hwplat->key_ctrl);
	writel(MAGIC_KEY, hwplat->key_ctrl + OFFSET_LONG_TERM_KEY);
	writel(MAGIC_AUTH, hwplat->key_ctrl + OFFSET_AUTH);
	writel((readl(hwplat->pinmux_ctrl)|NVT_MMC1_FUNC_SWITCH),
		hwplat->pinmux_ctrl);

	return 0;
}

static int nvt_sd_set_bus_width(struct nvt_mmc_hwplat *hwplat, int width)
{
	void __iomem *sdc = hwplat->sdc_vbase;

	debug_log("[MMC1] bus width = %d", width);

	switch (width) {
	case MMC_BUS_WIDTH_1:
		writeb((readb(sdc+REG_SDC_HOST_CTRL) & (~SDC_HOST_CTRL_4BIT)),
			sdc+REG_SDC_HOST_CTRL);
		writeb((readb(sdc+REG_SDC_HOST_CTRL) & (~SDC_HOST_CTRL_8BIT)),
			sdc+REG_SDC_HOST_CTRL);
		break;
	case MMC_BUS_WIDTH_4:
		writeb((readb(sdc+REG_SDC_HOST_CTRL) | SDC_HOST_CTRL_4BIT),
			sdc+REG_SDC_HOST_CTRL);
		writeb((readb(sdc+REG_SDC_HOST_CTRL) & (~SDC_HOST_CTRL_8BIT)),
			sdc+REG_SDC_HOST_CTRL);
		break;
	default:
		debug_log("[MMC1] invalid bus width(%d)", width);
		return 1;
	}

	return 0;
}

static int nvt_mmc_set_bus_timing_mode_ex(struct nvt_mmc_hwplat *hwplat,
enum NVT_MMC_SPEED_MODE speed_mode, enum NVT_MMC_DATA_LATCH latch)
{
	void __iomem *sdc = hwplat->sdc_vbase;
	/* void __iomem *fcr = hwplat->fcr_vbase; */
#define SD_HIGH_SPEED_LATCH_CLK	(50000000)
/* if more than this enable high speed latch mod. */
	debug_log("[MMC1] speed_mode = %d\n", speed_mode);
	debug_log("[MMC1] latch = %d\n", latch);
	debug_log("[MMC1] hwplat->max_bus_clk = %d\n", hwplat->max_bus_clk);

	if (speed_mode == NVT_MMC_HIGH_SPEED && (hwplat->max_bus_clk >=
		SD_HIGH_SPEED_LATCH_CLK)) {
		writeb(readb(sdc+REG_SDC_HOST_CTRL) | SDC_HOST_CTRL_HIGH_SPEED,
			sdc+REG_SDC_HOST_CTRL);
	} else if (speed_mode == NVT_MMC_LEGACY_SPEED || (speed_mode ==
		NVT_MMC_HIGH_SPEED && hwplat->max_bus_clk <
		SD_HIGH_SPEED_LATCH_CLK)) {
		writeb(readb(sdc+REG_SDC_HOST_CTRL) &
		(~SDC_HOST_CTRL_HIGH_SPEED), sdc+REG_SDC_HOST_CTRL);
	} else {
		pr_err("[MMC1] not support host speed mode(%d)", speed_mode);
		return -1;
	}

	if (latch == NVT_MMC_SINGLE_LATCH) {
		/* writel(readl(fcr+REG_FCR_CPBLT) &
		* (~FCR_CPBLT_DUAL_DATA_RATE_ENABLE), fcr+REG_FCR_CPBLT); */
	} else {
		pr_err("[MMC1] latch(%d) is invalid.", latch);
		return -1;
	}

	return 0;
}

static int nvt_sd_set_bus_clk(struct nvt_mmc_hwplat *hwplat, unsigned int clk)
{
	void __iomem *sdc = NULL;
	void __iomem *fcr = NULL;

	sdc = hwplat->sdc_vbase;
	fcr = hwplat->fcr_vbase;

	debug_log("bus clk = %u", clk);

	if (clk != 0) {
		/* stop bus clk first */
		writel(0, sdc+REG_SDC_CLK_CTRL);

		{/* always use flexble_clk because of source is 100M and
		    sd only run at 50M-400K */
			int divisor_org = (hwplat->src_clk)/clk - 2;
			int x = (divisor_org < 0) ? 0 : divisor_org;

			debug_log("divisor = %u, SD clk = %u\n", x,
			hwplat->src_clk/(x+2));

			/* turn on bus internal clk */
			writel((SDC_CLK_CTRL_INCLK_ENABLE |
			SDC_CLK_CTRL_SDCLK_FREQ_SEL_EX(x)),
			sdc+REG_SDC_CLK_CTRL);
			/* use NVT's clk scheme */
			writel((readl(fcr+REG_FCR_SD_2nd_FUNC_CTRL) |
			FCR_FUNC_CTRL_SD_FLEXIBLE_CLK),
			fcr+REG_FCR_SD_2nd_FUNC_CTRL);
		}

		/* wait internal clk stable */
		while (!(readl(sdc+REG_SDC_CLK_CTRL) &
		    SDC_CLK_CTRL_INCLK_STABLE)) {
			debug_log("wait clk stable");
		}

		/* OK, turn on the bus clk */
		writel((readl(sdc+REG_SDC_CLK_CTRL)|SDC_CLK_CTRL_SDCLK_ENABLE),
			sdc+REG_SDC_CLK_CTRL);

		/* wait 1us to do anything further, in order to make sure
		 * bus clk is stable */
		mdelay(1);
	}

	return 0;
}

static int sdhci_nvt_platform_bus_width_ex(struct sdhci_host *host, int width)
{
	struct sdhci_pltfm_host *pltfm_host = sdhci_priv(host);
	struct nvt_mmc_hwplat *hwplat = pltfm_host->priv;
	unsigned char timing = host->mmc->ios.timing;
	int err = 0;

	debug_log("[MMC1] mmc ask bus width = %d", width);
	err = nvt_sd_set_bus_width(hwplat, width);
	if (err)
		return err;

	/* since sdhci layer only set DDR/SDR/HS200 when
	 * host is capable of SD 3.0,we set them here
	 */
	switch (timing) {
	case MMC_TIMING_LEGACY:
		err = nvt_mmc_set_bus_timing_mode_ex(hwplat,
			NVT_MMC_LEGACY_SPEED, NVT_MMC_SINGLE_LATCH);
		break;
	case MMC_TIMING_SD_HS:
		err = nvt_mmc_set_bus_timing_mode_ex(hwplat,
			NVT_MMC_HIGH_SPEED, NVT_MMC_SINGLE_LATCH);
		break;
	default:
		err = 1;
		pr_err("[MMC] timing setting is invalid, %u", timing);
		return err;
	}

	return 0;
}

static void sdhci_nvt_set_clock_ex(struct sdhci_host *host, unsigned int clock)
{
	struct sdhci_pltfm_host *pltfm_host = sdhci_priv(host);
	struct nvt_mmc_hwplat *hwplat = pltfm_host->priv;

	hwplat->cur_clk = clock;
	nvt_sd_set_bus_clk(hwplat, clock);
}

static void sdhci_nvt_platform_reset_enter_ex(struct sdhci_host *host, u8 mask)
{
	struct sdhci_pltfm_host *pltfm_host = NULL;
	struct nvt_mmc_hwplat *hwplat = NULL;

	if (mask & SDHCI_RESET_ALL) {
		debug_log("*** %s:%d\n", __func__, __LINE__);
		pltfm_host = sdhci_priv(host);
		hwplat = pltfm_host->priv;
		/* sdhci layer have ioremap SD base, we need to keep
		 * it in our own for later use */
		hwplat->sdc_vbase = host->ioaddr;

		nvt_sd_pre_init(hwplat);

		if (nvt_sd_init(hwplat) != 0)
			pr_err("*** mmc1 hw reset failed\n");
	}
}

static unsigned int sdhci_nvt_get_ro(struct sdhci_host *host)
{
	struct sdhci_pltfm_host *pltfm_host = sdhci_priv(host);
	struct nvt_mmc_hwplat *hwplat = pltfm_host->priv;

	unsigned int read_only = 0;
	int reg_val;

	reg_val = readl(hwplat->sd_tcl_wp_val);

	if (reg_val & (1 << NVT_TCL_CN_WP_OFS))
		read_only = 1;
	else
		read_only = 0;

	return read_only;
}
#endif

static u64 mmc_dmamask = DMA_BIT_MASK(32);
/* unsigned long get_mmc_clk(void); */
/* unsigned long get_ahb_clk(void); */
static int nvt_mmc_parse_dt(struct nvt_mmc_hwplat *hwplat,
	struct platform_device *plt_dev)
{
	struct device *dev = &plt_dev->dev;
	struct device_node *node = dev->of_node;

	if (!hwplat || !plt_dev)
		return -1;

	/* node = of_find_compatible_node(NULL, NULL, "nvt,hsmmc"); */
	if (node) {
		struct sdhci_host *host = dev_get_drvdata(dev);
		/* get regs */
		/* of_address_to_resource(node, 0, &mmc_resources0[0]); */
		/* TOCHECK: get sdc_base back? */
		hwplat->nfc_vbase = of_iomap(node, 1);
		hwplat->fcr_vbase = of_iomap(node, 2);
		/* hwplat->emmc_io_driving_vbase= of_iomap(node, 3); */

		/* get irq */
		/* irq_of_parse_and_map(node, 0); */

		/*set irq*/

		/* get specific info of our controller */
		hwplat->src_clk = get_mmc_clk();
#ifdef CONFIG_MMC_SDHCI_NVT_SDCARD
		if (strcmp(node->name, "mmc1") == 0)
			hwplat->src_clk = get_ahb_clk();
#endif
#ifdef CONFIG_MMC_SDHCI_NVT_SDCARD
		if (strcmp(node->name, "mmc1") == 0) {
			struct device_node *node_sd_cd;

			node_sd_cd = of_find_node_by_name(NULL, "sd_cd_gpio");
			if (node_sd_cd) {
				hwplat->sd_cd_irq =
				irq_of_parse_and_map(node_sd_cd, 0);
				of_property_read_u32(node_sd_cd,
				"cd-gpios", &sd_det_pin);
				sd_det_pin = 1<<sd_det_pin;
				need_sd_gpio_det = true;
				pr_info("*** [MMC1] sd_cd_irq= %d sd_det_pin=%d\n",
				hwplat->sd_cd_irq, sd_det_pin);
			} else
				pr_info("*** [MMC1] no define sd_cd_gpio\n");
		}
#endif
		of_property_read_u32(node, "max-bus-frequency",
				&hwplat->max_bus_clk);
		of_node_put(node);

		mmc_of_parse(host->mmc);
		if (of_property_read_u32(node, "clk-shift",
			&hwplat->host_clk_shift) < 0) {
			hwplat->host_clk_shift = 0x2;
		/*(668 default. //designer suggest result 0 */
		}
		debug_log("mmc clk-shift setting %x\n", hwplat->host_clk_shift);
		debug_log("*** [MMC] src_clk = %d, max bus clk = %d, irq= %d\n",
			hwplat->src_clk, hwplat->max_bus_clk, host->irq);
	} else {
		debug_log("*** [MMC] can not find mmc device tree node.\n");
	}

	return 0;

}


#if defined(CONFIG_MMC_SDHCI_NVT_SDCARD)
static struct sdhci_ops sdhci_nvt_ops1 = {
	.write_b = sdhci_nvt_writeb,
	.write_w = sdhci_nvt_writew,
	.write_l = sdhci_nvt_writel,
	.read_w = sdhci_nvt_readw,
	.get_max_clock = sdhci_nvt_get_max_clock,
	.get_min_clock = sdhci_nvt_get_min_clock,
	.get_timeout_clock = sdhci_nvt_get_timeout_clock,
	.set_clock = sdhci_nvt_set_clock_ex,
/*	.platform_bus_width = sdhci_nvt_platform_bus_width_ex,*/
	.set_bus_width = sdhci_nvt_platform_bus_width_ex,
/*	.platform_reset_enter = sdhci_nvt_platform_reset_enter_ex, */
	.reset = sdhci_nvt_platform_reset_enter_ex,
	.get_ro = sdhci_nvt_get_ro,
};
#endif


#if defined(CONFIG_MMC_SDHCI_NVT_SDCARD)
static struct sdhci_pltfm_data sdhci_nvt_pdata1 = {
	.ops    = &sdhci_nvt_ops1,
	/* some register setting must be defined by our own to make
	 * NVT controller happy */
	.quirks = SDHCI_QUIRK_DELAY_AFTER_POWER |
	    /* this will be several millisecond delay after bus pwr on */
		SDHCI_QUIRK_MISSING_CAPS            |
		/* we want to fine tune controller's behaviors, set this */
		SDHCI_QUIRK_FORCE_DMA               |
		/* always use DMA to transfer DATA */
		SDHCI_QUIRK_CAP_CLOCK_BASE_BROKEN   |
		/* we have our own clk scheme, not te same as
		 * standard SD host spec */
		SDHCI_QUIRK_NONSTANDARD_CLOCK       |
		/* we do not use clk setting in spec */
		SDHCI_QUIRK_FORCE_BLK_SZ_2048
		/* use max block size as 2K as default value, can fine fune */
		| SDHCI_QUIRK_MULTIBLOCK_READ_ACMD12
};
#endif

static const struct of_device_id sdhci_nvt_dt_match[] = {
	{ .compatible = "nvt,hsmmc1", .data = &sdhci_nvt_pdata1 },
	{},
};

MODULE_DEVICE_TABLE(of, sdhci_nvt_dt_match);

#if defined(CONFIG_MMC_SDHCI_NVT_SDCARD)

static int nvt_sd_card_exist(struct nvt_mmc_hwplat *hwplat)
{
	int ret = 0;
	unsigned int reg_val;

	reg_val = readl(hwplat->sd_cd_pin_val);

	if (reg_val & sd_det_pin)
		ret = 0;
	else
		ret = 1;

	return ret;
}

static void nvt_sd_switch_cd_int(struct nvt_mmc_hwplat *hwplat, int enable)
{
	unsigned int reg_val;

	if (enable) {
		/* enable rising interrupt */
		reg_val = readl(hwplat->sd_cd_rising_int_en);
		reg_val |= sd_det_pin;
		writel(reg_val, hwplat->sd_cd_rising_int_en);

		/* enable faling interrupt */
		reg_val = readl(hwplat->sd_cd_falling_int_en);
		reg_val |= sd_det_pin;
		writel(reg_val, hwplat->sd_cd_falling_int_en);
	} else {
		/* enable rising interrupt */
		reg_val = readl(hwplat->sd_cd_rising_int_en);
		reg_val &= ~sd_det_pin;
		writel(reg_val, hwplat->sd_cd_rising_int_en);

		/* enable faling interrupt */
		reg_val = readl(hwplat->sd_cd_falling_int_en);
		reg_val &= ~sd_det_pin;
		writel(reg_val, hwplat->sd_cd_falling_int_en);
	}
}

static void nvt_sd_clear_cd_int_sts(struct nvt_mmc_hwplat *hwplat)
{
	unsigned int reg_val;

	/* clear rising edge interrupt */
	reg_val = readl(hwplat->sd_cd_rising_int_sts);
	reg_val |= sd_det_pin;
	writel(reg_val, hwplat->sd_cd_rising_int_sts);

	/* clear falling edge interrupt */
	reg_val = readl(hwplat->sd_cd_falling_int_sts);
	reg_val |= sd_det_pin;
	writel(reg_val, hwplat->sd_cd_falling_int_sts);
}

static void nvt_sd_sw_card_detect(struct nvt_mmc_hwplat *hwplat,
	int card_insert)
{
	unsigned int reg_val;

	reg_val = readl(hwplat->sd_cd_fcr_ctrl);
	if (card_insert)
		reg_val &= ~NVT_SD_SW_CARD_REMOVE;
	else
		reg_val |= NVT_SD_SW_CARD_REMOVE;
	writel(reg_val, hwplat->sd_cd_fcr_ctrl);
}

static void nvt_sd_sw_cd_notify(struct platform_device *pdev)
{
	struct sdhci_host *host = platform_get_drvdata(pdev);
	struct nvt_mmc_hwplat *hwplat = pdev->dev.platform_data;
	unsigned long flags;

	if (host) {
		spin_lock_irqsave(&host->lock, flags);
		if (nvt_sd_card_exist(hwplat)) {
			host->flags &= ~SDHCI_DEVICE_DEAD;
			host->quirks |= SDHCI_QUIRK_BROKEN_CARD_DETECTION;
			nvt_sd_sw_card_detect(hwplat, 1);
		} else {
			host->flags |= SDHCI_DEVICE_DEAD;
			host->quirks &= ~SDHCI_QUIRK_BROKEN_CARD_DETECTION;
			nvt_sd_sw_card_detect(hwplat, 0);
		}
		tasklet_schedule(&host->card_tasklet);
		spin_unlock_irqrestore(&host->lock, flags);
	}


}

static irqreturn_t sdhci_sd_cd_irq_thread(int irq, void *dev_id)
{
	struct platform_device *pdev = dev_id;
	struct nvt_mmc_hwplat *hwplat = pdev->dev.platform_data;

	if (pdev->id != 1)
		return IRQ_NONE;

	nvt_sd_clear_cd_int_sts(hwplat);

	nvt_sd_sw_cd_notify(pdev);

	return IRQ_HANDLED;
}
#endif

static int sdhci_nvt_probe(struct platform_device *pdev)
{
	const struct of_device_id *match;
	struct sdhci_host *host = NULL;
	struct sdhci_pltfm_host *pltfm_host = NULL;
	struct nvt_mmc_hwplat *hwplat = NULL;
	int ret = 0;

	match = of_match_device(sdhci_nvt_dt_match, &pdev->dev);
	if (!match) {
		pr_err("[MMC] OF not found\n");
		return -EINVAL;
	}

	debug_log("%s %d: match->compatible = %s\n", __func__,
		__LINE__,  match->compatible);

	if (strcmp(match->compatible, "nvt,hsmmc1") == 0)
		host = sdhci_pltfm_init(pdev, &sdhci_nvt_pdata1, 0);

	if (IS_ERR(host)) {
		pr_err("[MMC] sdhci plat init fail\n");
		ret = PTR_ERR(host);
		goto err_sdhci_pltfm_init;
	}

	if (pdev->dev.platform_data) {
		pr_err("[MMC] nvt mmc platform data should be NULL when initial\n");
		goto err_sdhci_pltfm_init;
	}

	if (strcmp(match->compatible, "nvt,hsmmc1") == 0) {
		host->quirks2 |= SDHCI_QUIRK2_HOST_NO_CMD23;

		host->caps |= MMC_CAP_4_BIT_DATA |
		    SDHCI_CAN_DO_SDMA | SDHCI_CAN_VDD_330 | SDHCI_CAN_DO_HISPD;
	} else {
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

#ifdef NVT_MMC_SUPPORT_POWER_OFF_NOTIFICATION
	host->mmc->caps2 |= MMC_CAP2_POWEROFF_NOTIFY;
	debug_log("NVT_MMC_SUPPORT_POWER_OFF_NOTIFICATION\n");
#endif

#ifdef NVT_MMC_SUPPORT_PACKED_CMD
	host->mmc->caps2 |= MMC_CAP2_PACKED_CMD;
	debug_log("NVT_MMC_SUPPORT_PACKED_CMD\n");
#endif
	}

	pltfm_host = sdhci_priv(host);

	hwplat  = pdev->dev.platform_data =  devm_kzalloc(&pdev->dev,
		sizeof(struct nvt_mmc_hwplat), GFP_KERNEL);
	if (!hwplat) {
		ret = -ENOMEM;
		goto err_sdhci_add;
	}
	nvt_mmc_parse_dt(hwplat, pdev);
	pltfm_host->priv = hwplat;

	/* set platform  device information*/
	pdev->dev.dma_mask = &mmc_dmamask;
	pdev->dev.coherent_dma_mask = DMA_BIT_MASK(32);
	ret = sdhci_add_host(host);
	if (ret)
		goto err_sdhci_add;

#ifdef CONFIG_NVT_MMC_CLKGATE
	debug_log("novatek mmc clk-gating\n");
#endif
#if defined(CONFIG_MMC_SDHCI_NVT_SDCARD)
if (need_sd_gpio_det) {
	if (strcmp(match->compatible, "nvt,hsmmc1") == 0) {
		pdev->id = 1;
		hwplat->id = 1;
		nvt_sd_clear_cd_int_sts(hwplat);
		ret = request_irq(hwplat->sd_cd_irq, sdhci_sd_cd_irq_thread,
				IRQF_SHARED, mmc_hostname(host->mmc), pdev);
		if (ret >= 0)
			nvt_sd_switch_cd_int(hwplat, 1);
		if (ret < 0) {
			pr_err("%s : request irq fail !\n", __func__);
			goto err_sdhci_add;
		}
		if (nvt_sd_card_exist(hwplat))
			nvt_sd_sw_cd_notify(pdev);
		}
	}
#endif
	debug_log("[MMC] add NVT host\n");

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
#if defined(CONFIG_MMC_SDHCI_NVT_SDCARD)
	if (need_sd_gpio_det)
		free_irq(hwplat->sd_cd_irq, pdev);
#endif
	nvt_sd_deinit(hwplat);
	devm_kfree(&pdev->dev, hwplat);
	pdev->dev.platform_data = NULL;
	sdhci_pltfm_unregister(pdev);

	return 0;
}

static struct platform_driver sdhci_sd_nvt_driver = {
	.driver         = {
		.name   = "sdhci_sd_nvt",
		.owner  = THIS_MODULE,
		.pm     = SDHCI_PLTFM_PMOPS,
		.of_match_table = sdhci_nvt_dt_match,
	},
	.probe          = sdhci_nvt_probe,
	.remove         = sdhci_nvt_remove,
};


module_platform_driver(sdhci_sd_nvt_driver);

MODULE_DESCRIPTION("SDHCI SD driver for NVT");
MODULE_AUTHOR("Novatek Corp.");
MODULE_LICENSE("GPL v2");
