/* this header contains those registers which will be used as
   control NVT sd controller's exotic behaviors */

#ifndef __NVT_SDC_H__
#define __NVT_SDC_H__

/*
  related physical address & irq nr
*/
#define NVT_MMC_IRQ_NR					(95)

/*
  SoC misc
*/
#define NVT_SDCLK_MAX	(SDCLK_SOURCE_HIGH/4)

#define NVT_REG_PHY_KEY_CTRL				(0xFC040204)
#define NVT_CTRL_KEY1					(0x72682)
#define NVT_CTRL_KEY2					(0x28627)
#define	OFFSET_LONG_TERM_KEY				(0x8-0x4)
#define MAGIC_KEY					(0x1)
#define NVT_REG_PHY_DRIVING_CTRL			(0xFC040000)
#define ACK_RETRY_COUNT				(5000)
#define PMOS_DRIVING					(0x42FA7900)
#define NMOS_DRIVING					(0x42FA7800)

#define NVT_REG_PHY_SRC_CLK_CTRL			(0xFC040C18)
#define NVT_EMMC_FAST_CLK_SRC_ENABLE			(1<<16)

#define NVT_REG_PHY_SECURE_LOCK_CTRL			(0xFD650000)
#define NVT_EMMC_SECURE_LOCK_ENABLE			(1<<4)

#define NVT_REG_PHY_MMC_ARBITOR_CTRL			(0xFC040200)
#define NVT_EMMC_STBC_ENABLE				(1<<4)
#define NVT_EMMC_MAINCHIP_ENABLE			(1<<5)

#define NVT_REG_PHY_HW_RESET_CTRL			(0xFC04021C)
#define NVT_EMMC_HW_RESET_START				(1<<25)

#define NVT_REG_PHY_ACP_CTRL0				(0xFD6B043C)
#define NVT_ACP_ARB_32BIT				(0x009FDFCC)

#define NVT_REG_PHY_ACP_CTRL1				(0XFD150084)
#define NVT_ACP_AHB2AXI_ENABLE				(1<<29)

#define NVT_REG_PHY_ACP_CTRL2				(0xFD150408)
#define NVT_ACP_64BIT_MODE				(1<<21)

#define NVT_REG_PHY_ACP_CTRL3				(0xFD060104)
#define NVT_ACP_ACCESS_WHOLE_DDR_RANGE			(3<<30)

#define NVT_REG_PHY_MPLL				(0xFC040904)
#define NVT_REG_PHY_EMMC_MPLL_ENABLE			(1<<26)

#if defined(CONFIG_MMC_SDHCI_NVT_HS400)
#define NVT_REG_EMMC_IODRV				(0xFC040D60)
#define NVT_REG_EMMC_IONUM				(11)
#else
#define NVT_REG_EMMC_IODRV				(0xFC040824)
#define NVT_REG_EMMC_IONUM				(10)
#endif



#if defined(CONFIG_ARCH_NVT72563)
#define NVT_REG_SD_PIN_MUX				(0xFC040440)
#else
#define NVT_REG_SD_PIN_MUX				(0xFC04043C)
#endif
#define NVT_SD_CLK_FUNC_BIT				(1 << 12)
#define NVT_SD_CD_FUNC_BIT				(1 << 13)
#if (defined(CONFIG_ARCH_NVT72670) || defined(CONFIG_ARCH_NVT72563))
#define NVT_SD_D0_FUNC_BIT				(1 << 10)
#define NVT_SD_D1_FUNC_BIT				(1 << 11)
#define NVT_SD_D2_FUNC_BIT				(1 << 12)
#define NVT_SD_D3_FUNC_BIT				(1 << 13)
#define NVT_SD_CMD_FUNC_BIT				(1 << 26)
#else
#define NVT_SD_D0_FUNC_BIT				(1 << 16)
#define NVT_SD_D1_FUNC_BIT				(1 << 17)
#define NVT_SD_D2_FUNC_BIT				(1 << 18)
#define NVT_SD_D3_FUNC_BIT				(1 << 19)
#define NVT_SD_CMD_FUNC_BIT				(1 << 26)
#endif

#define NVT_REG_MMC1_PIN_MUX			(0xFC040250)
#define NVT_MMC1_FUNC_SWITCH			(1<<7)

#if defined(CONFIG_MMC_SDHCI_NVT_SDCARD)
#if (defined(CONFIG_ARCH_NVT72670) || defined(CONFIG_ARCH_NVT72563))
#define NVT_REG_SD_PWR_MUX				(0xFC04043C)
#define NVT_REG_SD_PWR_DIR				(0xFC040430)
#define NVT_REG_SD_PWR_VAL				(0xFC040424)
#define NVT_SD_PWR_ENABLE				(1 << 23)

#define NVT_TCL_CN_WP_MUX				(0xFC04043C)
#define NVT_TCL_CN_WP_DIR				(0xFC040430)
#define NVT_TCL_CN_WP_REG_VAL				(0xFC040424)
#define NVT_TCL_CN_WP_OFS				22
#else
#define NVT_REG_SD_PWR_MUX				(0xFC040440)
#define NVT_REG_SD_PWR_DIR				(0xFC040434)
#define NVT_REG_SD_PWR_VAL				(0xFC040428)
#define NVT_SD_PWR_ENABLE				(1 << 3)

#define NVT_TCL_CN_WP_MUX				(0xFD100414)
#define NVT_TCL_CN_WP_DIR				(0xFD100408)
#define NVT_TCL_CN_WP_REG_VAL				(0xFD100400)
#define NVT_TCL_CN_WP_OFS				23
#endif

/*#define NVT_SD_CD_IRQ					(106) */
/* #define NVT_SD_CD_OP_BIT				(1 << 3) */
#define NVT_REG_SD_CD_PIN_MUX				(0xFC04043C)
#define NVT_REG_SD_CD_PIN_DIR				(0xFC040430)
#define NVT_REG_SD_CD_PIN_VAL				(0xFC040424)
#define NVT_REG_SD_CD_FALLING_INT_EN			(0xFC040464)
#define NVT_REG_SD_CD_RISING_INT_EN			(0xFC040470)
#define NVT_REG_SD_CD_FALLING_INT_STS			(0xFC04047C)
#define NVT_REG_SD_CD_RISING_INT_STS			(0xFC040488)
#define NVT_REG_SD_FCR_CTRL				(0xFC048208)
#define NVT_SD_SW_CD_ENABLE				(1 << 24)
#define NVT_SD_SW_CARD_REMOVE				(1 << 26)


#endif/* end of CONFIG_MMC_SDHCI_NVT_SDCARD */
#define NVT_IOREMAP_BASE NVT_REG_PHY_MMC_ARBITOR_CTRL
#if defined(CONFIG_MMC_SDHCI_NVT_HS400)
#define NVT_IOREMAP_REGION (NVT_REG_EMMC_IODRV - \
	NVT_REG_PHY_MMC_ARBITOR_CTRL + 4)
#else
#define NVT_IOREMAP_REGION (NVT_REG_PHY_SRC_CLK_CTRL - \
	NVT_REG_PHY_MMC_ARBITOR_CTRL + 4)
#endif
#define NVT_IOREMAP_ACP_BASE NVT_REG_PHY_ACP_CTRL1
#define NVT_IOREMAP_ACP_REGION (NVT_REG_PHY_ACP_CTRL2 - \
	NVT_REG_PHY_ACP_CTRL1 + 4)
/*//#define REG_EMMC_ARBITOR_CTRL*/

/*
 FCR registers & related attributes
*/
#define REG_FCR_FUNC_CTRL                       (0x00)
#define REG_FCR_CPBLT                           (0x04)
#define REG_FCR_SD_2nd_FUNC_CTRL                (0x08)
#define REG_FCR_FUNC_CTRL_1                     (0x0c)
#define REG_FCR_HS200_CTRL                      (0x14)

#define FCR_FUNC_CTRL_SW_MS_CD                  (1 << 27)
#define FCR_FUNC_CTRL_SW_SD_CD                  (1 << 26)
#define FCR_FUNC_CTRL_SW_SD_WP                  (1 << 25)
#define FCR_FUNC_CTRL_SW_CDWP_ENABLE            (1 << 24)
#define FCR_FUNC_CTRL_LITTLE_ENDIAN             (1 << 23)
#define FCR_FUNC_CTRL_SD_FLEXIBLE_CLK           (1 << 22)

#define FCR_FUNC_CTRL_AHB_MAX_BURST_SIZE(n)     (((n) & 3) << 20)
#define FCR_FUNC_CTRL_AHB_MAX_BURST_SIZE_MASK	\
		FCR_FUNC_CTRL_AHB_MAX_BURST_SIZE(-1)
#define FCR_FUNC_CTRL_AHB_MAX_BURST_SIZE_512	\
		FCR_FUNC_CTRL_AHB_MAX_BURST_SIZE(3)
#define FCR_FUNC_CTRL_AHB_MAX_BURST_SIZE_256	\
		FCR_FUNC_CTRL_AHB_MAX_BURST_SIZE(2)
#define FCR_FUNC_CTRL_AHB_MAX_BURST_SIZE_128	\
		FCR_FUNC_CTRL_AHB_MAX_BURST_SIZE(1)
#define FCR_FUNC_CTRL_AHB_MAX_BURST_SIZE_64	\
		FCR_FUNC_CTRL_AHB_MAX_BURST_SIZE(0)

#define FCR_FUNC_CTRL_INT_OPT_STAT              (1 << 19)
#define FCR_FUNC_CTRL_INT_OPT_DMA               (1 << 18)
#define FCR_FUNC_CTRL_SD_PULLUP_RESISTOR        (1 << 17)
#define FCR_FUNC_CTRL_MMC_8BIT                  (1 << 16)
#define FCR_FUNC_CTRL_CD_DEBOUNCE_TIME(n)       (((n) & 0xf) << 12)
#define FCR_FUNC_CTRL_CD_DEBOUNCE_TIME_MASK	\
		FCR_FUNC_CTRL_CD_DEBOUNCE_TIME(-1)
#define FCR_FUNC_CTRL_PW_UP_TIME(n)             (((n) & 0xf) << 8)
#define FCR_FUNC_CTRL_PW_UP_TIME_MASK           FCR_FUNC_CTRL_PW_UP_TIME(-1)
#define FCR_FUNC_CTRL_SD_SIG_PULLUP_TIME(n)	(((n) & 0xf) << 4)
#define FCR_FUNC_CTRL_SD_SIG_PULLUP_TIME_MASK   \
		FCR_FUNC_CTRL_SD_SIG_PULLUP_TIME(-1)
#define FCR_FUNC_CTRL_MS_SIG_DELAY(n)           (((n) & 3) << 2)
#define FCR_FUNC_CTRL_MS_SIG_DELAY_MASK         \
		FCR_FUNC_CTRL_MS_SIG_DELAY(-1)
#define FCR_FUNC_CTRL_SD_SIG_DELAY(n)           ((n) & 3)
#define FCR_FUNC_CTRL_SD_SIG_DELAY_MASK         \
		FCR_FUNC_CTRL_SD_SIG_DELAY(-1)
#define FCR_FUNC_CTRL_READ_CLK_DELAY(n)         (((n) & 3) << 28)
#define FCR_FUNC_CTRL_READ_CLK_DELAY_MASK       \
		FCR_FUNC_CTRL_READ_CLK_DELAY(-1)
#define FCR_FUNC_CTRL_DEFAULT			(0xf3020)

#define FCR_CPBLT_SD_CLK_BYPASS                 (1 << 20)
#define FCR_CPBLT_DUAL_DATA_RATE_ENABLE         (1 << 19)
#define FCR_CPBLT_VOL_18V                       (1 << 18)
#define FCR_CPBLT_VOL_30V                       (1 << 17)
#define FCR_CPBLT_VOL_33V                       (1 << 16)
#define FCR_CPBLT_SD_BASE_CLK_FREQ(n)           (((n) & 0x3f) << 8)
#define FCR_CPBLT_SD_BASE_CLK_FREQ_MASK         FCR_CPBLT_SD_BASE_CLK_FREQ(-1)
#define FCR_CPBLT_SD_MAX_CURR_CPBLT(n)          ((n) & 0xff)
#define FCR_CPBLT_SD_MAX_CURR_CPBLT_MASK        FCR_CPBLT_SD_MAX_CURR_CPBLT(-1)
#define FCR_CPBLT_DEFAULT			(0xf8134ff)

#define FCR_HS200_CTRL_ENABLE                           (1<<0)
#define FCR_HS200_CTRL_SW_OVERSAMPLE_ENABLE             (1<<1)
#define FCR_HS200_CTRL_SW_OVERSAMPLE_CMD_MASK           (3<<2)
#define FCR_HS200_CTRL_HW_TRACK_EACH_BLK                (1<<6)
#define FCR_HS200_CRTL_FASTEST_CLK                      (1<<7)
#define FCR_HS200_CTRL_DISABLE_CMD_CONFLICT             (1<<8)
#define FCR_HS200_OUTPUT_SELECT_MASK                    (3<<26)
#define FCR_HS200_OUTPUT_SELECT_PHASE(phase)             ((phase&3)<<26)
#define FCR_HS200_CTRL_RESET_DLL                (1<<29)
#define FCR_HS200_CTRL_EXTEND_STROBE            (1<<30)
#define FCR_HS200_CTRL_EXTSTRB_LATCH            (1<<31)

/*
 NFC registers & related attributes
*/
#define REG_NFC_SYS_CTRL                        (0x5c)
#define NFC_EMMC_SEL                            (1<<7)

/*
 HS400 EMMC ADLL Configure Register
*/
#define REG_FCR_ADLL_CFG_REG                    (0x24)
#define REG_FCR_ADLL_STATUS_REG             (0x28)
#define ADLL_CFG_ADS_RESET                      (1 << 13)
#define ADLL_STATUS_FDLOCK                      (1 << 0)
#define ADLL_CFG_SET_RGL_LDOA(adllreg_val, n)	\
		((adllreg_val&0xCFFFFFFF)|(n<<28))
#define ADLL_CFG_SET_REG_NM(adllreg_val, n)	\
		((adllreg_val&0xFFFF3FFF)|(n<<14))
#define ADLL_CFG_SET_REG_SW(adllreg_val, n)	\
		((adllreg_val&0xFFFFE7FF)|(n<<11))
#define ADLL_CFG_SET_BAND_SEL(adllreg_val, n)	\
		((adllreg_val&0x3FFFFFFF)|(n<<30))
#define ADLL_CFG_SET_ADLL_WRITE_CLK_PHASE(adllreg_val, n)	\
		((adllreg_val&0xFFFFFFE0)|(n<<0))
#define ADLL_CFG_SET_ADLL_READ_CLK_PHASE(adllreg_val, n)	\
		((adllreg_val&0xFC3FFFFF)|(n<<22))


/*
  SDC registers & related attributes
*/
#define REG_SDC_HOST_CTRL                       (0x28)
#define SDC_HOST_CTRL_8BIT                      (1 << 5)
#define SDC_HOST_CTRL_HIGH_SPEED                (1 << 2)
#define SDC_HOST_CTRL_4BIT                      (1 << 1)
#define SDC_HOST_CTRL_LED_ON                    (1 << 0)

#define REG_SDC_CLK_CTRL                        (0x2c)
#define SDC_CLK_CTRL_SDCLK_FREQ_SEL(n)	((((WORD)(n)) & 0xff) << 8)
#define SDC_CLK_CTRL_SDCLK_FREQ_SEL_EX(n)	\
		((((n) & 0xff) << 8) | ((n&0x300)>>2))
#define SDC_CLK_CTRL_SDCLK_ENABLE               (1 << 2)
#define SDC_CLK_CTRL_INCLK_STABLE               (1 << 1)
#define SDC_CLK_CTRL_INCLK_ENABLE               (1 << 0)
#define SDC_MAX_CLK_DIV                         (1023)

#if defined(CONFIG_ARCH_NVT_FPGA_670)
#define CONFIG_MMC_SDHCI_NVT_HS400
#endif

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

enum NVT_MMC_DATA_LATCH {
	NVT_MMC_SINGLE_LATCH,
	NVT_MMC_DUAL_LATCH
};

enum NVT_MMC_SPEED_MODE {
	NVT_MMC_LEGACY_SPEED,
	NVT_MMC_HIGH_SPEED,
	NVT_MMC_HS200
};
enum NVT_MMC_DRIVING_MODE {
	NVT_MMC_NMOS,
	NVT_MMC_PMOS,
};
enum NVT_MMC_RDQS_TUNE_STATE {
	MMC_RDQS_TUNE_DISABLED = 0,
	MMC_RDQS_TUNE_ENABLING,
	MMC_RDQS_TUNE_DONE,
	MMC_RDQS_TUNE_ERROR,
};
struct emmc_io_driving {

/* just keep setting for driving field only */
#define DEFAULT_DRIVING	(-1)
#define DRIVING_BIT		5
#define DRIVING_MASK	((1<<(DRIVING_BIT))-1)
#define BYTE_MASK	((1<<8)-1)

#if defined(CONFIG_MMC_SDHCI_NVT_HS400)
#define RDQS_BIT_SHIFT	(22)
#define RDQS_MASK	(((1<<4)-1)<<22)
#define MMC_HS400_IO_DRIVING
#endif
	u32	data[8];
	u32	cmd;
	u8	ds;
	u8	clk;
};
struct nvt_mmc_hwplat {
#if 0
	int (*init)(struct nvt_mmc_hwplat *hwplat);
	void (*deinit)(struct nvt_mmc_hwplat *hwplat);
	int (*set_bus_clk)(struct nvt_mmc_hwplat *hwplat, unsigned int clk);
	int (*set_bus_width)(struct nvt_mmc_hwplat *hwplat, int width);
	int (*set_bus_timing_mode)(struct nvt_mmc_hwplat *hwplat,
		enum NVT_MMC_SPEED_MODE speed_mode,
		enum NVT_MMC_DATA_LATCH latch);
	void (*host_clk_gating)(struct nvt_mmc_hwplat *hwplat);
	void (*host_clk_ungating)(struct nvt_mmc_hwplat *hwplat);
#endif

	int sd_cd_irq;

	unsigned int src_clk;
	unsigned int max_bus_clk;
	unsigned int min_bus_clk;
	unsigned int cur_clk;
	int id;
	void __iomem *nfc_vbase;
	void __iomem *fcr_vbase;
	void __iomem *sdc_vbase;
	void __iomem *driving_ctrl;
	void __iomem *key_ctrl;
	void __iomem *clk_src_ctrl;
	void __iomem *secure_lock_ctrl;
	void __iomem *arbitor_ctrl;
	void __iomem *hw_reset_ctrl;
	void __iomem *acp_ctrl0;
	void __iomem *acp_ctrl1;
	void __iomem *acp_ctrl2;
	void __iomem *acp_ctrl3;
	void __iomem *mpll_ctrl;
	void __iomem *pinmux_ctrl;
	void __iomem *emmc_io_driving_vbase;
	void __iomem *sd_pwr_gpio_mux;
	void __iomem *sd_pwr_gpio_dir;
	void __iomem *sd_pwr_gpio_val;
	void __iomem *sd_tcl_wp_mux;
	void __iomem *sd_tcl_wp_dir;
	void __iomem *sd_tcl_wp_val;
	void __iomem *sd_func_pin_mux;
	void __iomem *ioremap_base;
	void __iomem *ioremap_acp_base;

#if defined(CONFIG_MMC_SDHCI_NVT_SDCARD)
	void __iomem *sd_cd_pin_mux;
	void __iomem *sd_cd_pin_dir;
	void __iomem *sd_cd_pin_val;
	void __iomem *sd_cd_rising_int_en;
	void __iomem *sd_cd_falling_int_en;
	void __iomem *sd_cd_rising_int_sts;
	void __iomem *sd_cd_falling_int_sts;
	void __iomem *sd_cd_fcr_ctrl;
#endif
	u8 gating_status;
	u8 rdqs_tune;
	u32 host_clk_shift;
	u32 host_adll_read_phase;
	u32 host_adll_write_phase;
	struct emmc_io_driving emmc_iodrv;
};

#endif
/*end of __NVT_SDC_H__ */
