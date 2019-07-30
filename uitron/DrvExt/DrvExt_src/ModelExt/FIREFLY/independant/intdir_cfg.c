#include "modelext_info.h"
#include "interrupt.h"

INT_GIC_DST  ind_intdir_cfg_data __attribute__((section("modelext_data.intdir_cfg"))) = {
	//MIPS1 interrupt destination
	.int_id_dst[CC_CORE_CA53_CORE1].Bit.bInt_ID_TIMER           = TRUE,
	.int_id_dst[CC_CORE_CA53_CORE1].Bit.bInt_ID_SIE             = TRUE,
	.int_id_dst[CC_CORE_CA53_CORE1].Bit.bInt_ID_SIE2            = TRUE,
	.int_id_dst[CC_CORE_CA53_CORE1].Bit.bInt_ID_SIE3            = TRUE,

	.int_id_dst[CC_CORE_CA53_CORE1].Bit.bInt_ID_SIE4            = TRUE,
	.int_id_dst[CC_CORE_CA53_CORE1].Bit.bInt_ID_IPE             = TRUE,
	.int_id_dst[CC_CORE_CA53_CORE1].Bit.bInt_ID_IME             = TRUE,
	.int_id_dst[CC_CORE_CA53_CORE1].Bit.bInt_ID_DCE             = TRUE,

	.int_id_dst[CC_CORE_CA53_CORE1].Bit.bInt_ID_IFE             = TRUE,
	.int_id_dst[CC_CORE_CA53_CORE1].Bit.bInt_ID_IFE2            = TRUE,
	.int_id_dst[CC_CORE_CA53_CORE1].Bit.bInt_ID_DIS             = TRUE,
	.int_id_dst[CC_CORE_CA53_CORE1].Bit.bInt_ID_FDE             = TRUE,

	.int_id_dst[CC_CORE_CA53_CORE1].Bit.bInt_ID_Ethernet_LPI    = TRUE,
	.int_id_dst[CC_CORE_CA53_CORE1].Bit.bInt_ID_RHE             = TRUE,
	.int_id_dst[CC_CORE_CA53_CORE1].Bit.bInt_ID_DRE             = TRUE,
	.int_id_dst[CC_CORE_CA53_CORE1].Bit.bInt_ID_DAI             = TRUE,

	.int_id_dst[CC_CORE_CA53_CORE1].Bit.bInt_ID_H264            = TRUE,
	.int_id_dst[CC_CORE_CA53_CORE1].Bit.bInt_ID_JPEG            = TRUE,
	.int_id_dst[CC_CORE_CA53_CORE1].Bit.bInt_ID_GRAPHIC         = TRUE,
	.int_id_dst[CC_CORE_CA53_CORE1].Bit.bInt_ID_GRAPHIC2        = TRUE,

	.int_id_dst[CC_CORE_CA53_CORE1].Bit.bInt_ID_AFFINE          = TRUE,
	.int_id_dst[CC_CORE_CA53_CORE1].Bit.bInt_ID_ISE             = TRUE,
	.int_id_dst[CC_CORE_CA53_CORE1].Bit.bInt_ID_TGE             = TRUE,
	.int_id_dst[CC_CORE_CA53_CORE1].Bit.bInt_ID_TSMUX           = TRUE,

	.int_id_dst[CC_CORE_CA53_CORE1].Bit.bInt_ID_GPIO            = TRUE,
	.int_id_dst[CC_CORE_CA53_CORE1].Bit.bInt_ID_REMOTE          = TRUE,
	.int_id_dst[CC_CORE_CA53_CORE1].Bit.bInt_ID_PWM             = TRUE,
#if defined(_CPU2_LINUX_)
	.int_id_dst[CC_CORE_CA53_CORE1].Bit.bInt_ID_USB             = FALSE,
#else
	.int_id_dst[CC_CORE_CA53_CORE1].Bit.bInt_ID_USB             = TRUE,
#endif
#if defined(_USB3_INT_CORE1_)
	.int_id_dst[CC_CORE_CA53_CORE1].Bit.bInt_ID_USB3            = TRUE,
#else
	.int_id_dst[CC_CORE_CA53_CORE1].Bit.bInt_ID_USB3            = FALSE,
#endif
#if defined(_CPU2_LINUX_)
	.int_id_dst[CC_CORE_CA53_CORE1].Bit.bInt_ID_NAND            = FALSE,
	.int_id_dst[CC_CORE_CA53_CORE1].Bit.bInt_ID_SDIO            = FALSE,
	.int_id_dst[CC_CORE_CA53_CORE1].Bit.bInt_ID_SDIO2           = FALSE,
	.int_id_dst[CC_CORE_CA53_CORE1].Bit.bInt_ID_SDIO3           = FALSE,
#else
	.int_id_dst[CC_CORE_CA53_CORE1].Bit.bInt_ID_NAND            = TRUE,
	.int_id_dst[CC_CORE_CA53_CORE1].Bit.bInt_ID_SDIO            = TRUE,
	.int_id_dst[CC_CORE_CA53_CORE1].Bit.bInt_ID_SDIO2           = TRUE,
	.int_id_dst[CC_CORE_CA53_CORE1].Bit.bInt_ID_SDIO3           = TRUE,
#endif
	.int_id_dst[CC_CORE_CA53_CORE1].Bit.bInt_ID_DMA             = TRUE,
#if defined(_CPU2_LINUX_)
	.int_id_dst[CC_CORE_CA53_CORE1].Bit.bInt_ID_ETHERNET        = FALSE,
#else
	.int_id_dst[CC_CORE_CA53_CORE1].Bit.bInt_ID_ETHERNET        = TRUE,
#endif
	.int_id_dst[CC_CORE_CA53_CORE1].Bit.bInt_ID_SPI             = TRUE,

	.int_id_dst[CC_CORE_CA53_CORE1].Bit.bInt_ID_SPI2            = TRUE,
	.int_id_dst[CC_CORE_CA53_CORE1].Bit.bInt_ID_SPI3            = TRUE,
	.int_id_dst[CC_CORE_CA53_CORE1].Bit.bInt_ID_SPI4            = TRUE,
	.int_id_dst[CC_CORE_CA53_CORE1].Bit.bInt_ID_Ehternet_RevMII = TRUE,

	.int_id_dst[CC_CORE_CA53_CORE1].Bit.bInt_ID_SIF             = TRUE,
	.int_id_dst[CC_CORE_CA53_CORE1].Bit.bInt_ID_I2C             = TRUE,
	.int_id_dst[CC_CORE_CA53_CORE1].Bit.bInt_ID_I2C2            = TRUE,
	.int_id_dst[CC_CORE_CA53_CORE1].Bit.bInt_ID_UART            = TRUE,
#if defined(_CPU2_LINUX_)
	.int_id_dst[CC_CORE_CA53_CORE1].Bit.bInt_ID_UART2           = FALSE,
#else
	.int_id_dst[CC_CORE_CA53_CORE1].Bit.bInt_ID_UART2           = TRUE,
#endif
	.int_id_dst[CC_CORE_CA53_CORE1].Bit.bInt_ID_UART3           = TRUE,
	.int_id_dst[CC_CORE_CA53_CORE1].Bit.bInt_ID_UART4           = TRUE,
	.int_id_dst[CC_CORE_CA53_CORE1].Bit.bInt_ID_ADC             = TRUE,


	.int_id_dst[CC_CORE_CA53_CORE1].Bit.bInt_ID_IDE             = TRUE,
	.int_id_dst[CC_CORE_CA53_CORE1].Bit.bInt_ID_IDE2            = TRUE,
	.int_id_dst[CC_CORE_CA53_CORE1].Bit.bInt_ID_DSI             = TRUE,
	.int_id_dst[CC_CORE_CA53_CORE1].Bit.bInt_ID_MI              = TRUE,

	.int_id_dst[CC_CORE_CA53_CORE1].Bit.bInt_ID_HDMI            = TRUE,
	.int_id_dst[CC_CORE_CA53_CORE1].Bit.bInt_ID_DMA2            = TRUE,
	.int_id_dst[CC_CORE_CA53_CORE1].Bit.bInt_ID_LVDS            = TRUE,
	.int_id_dst[CC_CORE_CA53_CORE1].Bit.bInt_ID_LVDS2           = TRUE,

	.int_id_dst[CC_CORE_CA53_CORE1].Bit.bInt_ID_RTC             = TRUE,
	.int_id_dst[CC_CORE_CA53_CORE1].Bit.bInt_ID_WDT             = TRUE,
	.int_id_dst[CC_CORE_CA53_CORE1].Bit.bInt_ID_CG              = TRUE,
	.int_id_dst[CC_CORE_CA53_CORE1].Bit.bInt_ID_CC              = TRUE,

	.int_id_dst[CC_CORE_CA53_CORE1].Bit.bInt_ID_I2C3            = TRUE,
	.int_id_dst[CC_CORE_CA53_CORE1].Bit.bInt_ID_SLVS_EC         = TRUE,
	.int_id_dst[CC_CORE_CA53_CORE1].Bit.bInt_ID_LVDS3           = TRUE,
	.int_id_dst[CC_CORE_CA53_CORE1].Bit.bInt_ID_LVDS4           = TRUE,

	//INT64
	.int_id_dst[CC_CORE_CA53_CORE1].Bit.bInt_ID_LVDS5           = TRUE,
	.int_id_dst[CC_CORE_CA53_CORE1].Bit.bInt_ID_LVDS6           = TRUE,
	.int_id_dst[CC_CORE_CA53_CORE1].Bit.bInt_ID_LVDS7           = TRUE,
	.int_id_dst[CC_CORE_CA53_CORE1].Bit.bInt_ID_LVDS8           = TRUE,

	//INT68
	.int_id_dst[CC_CORE_CA53_CORE1].Bit.bInt_ID_SIE5            = TRUE,
	.int_id_dst[CC_CORE_CA53_CORE1].Bit.bInt_ID_SIE6            = TRUE,
	.int_id_dst[CC_CORE_CA53_CORE1].Bit.bInt_ID_SIE7            = TRUE,
	.int_id_dst[CC_CORE_CA53_CORE1].Bit.bInt_ID_SIE8            = TRUE,

	//INT72
	.int_id_dst[CC_CORE_CA53_CORE1].Bit.bInt_ID_IVE             = TRUE,
	.int_id_dst[CC_CORE_CA53_CORE1].Bit.bInt_ID_SVM             = TRUE,
	.int_id_dst[CC_CORE_CA53_CORE1].Bit.bInt_ID_SDE             = TRUE,
	.int_id_dst[CC_CORE_CA53_CORE1].Bit.bInt_ID_CNN             = TRUE,

	//INT76
	.int_id_dst[CC_CORE_CA53_CORE1].Bit.bInt_ID_DSP             = TRUE,
	.int_id_dst[CC_CORE_CA53_CORE1].Bit.bInt_ID_DSP2            = TRUE,
	.int_id_dst[CC_CORE_CA53_CORE1].Bit.bInt_ID_CANBUS          = TRUE,
	.int_id_dst[CC_CORE_CA53_CORE1].Bit.bInt_ID_CRYPTO          = TRUE,

	//INT80
	.int_id_dst[CC_CORE_CA53_CORE1].Bit.bInt_ID_HWCOPY          = TRUE,
	.int_id_dst[CC_CORE_CA53_CORE1].Bit.bInt_ID_ROTATE          = TRUE,
	.int_id_dst[CC_CORE_CA53_CORE1].Bit.bInt_ID_I2C4            = TRUE,
	.int_id_dst[CC_CORE_CA53_CORE1].Bit.bInt_ID_I2C5            = TRUE,

	//INT84
	.int_id_dst[CC_CORE_CA53_CORE1].Bit.bInt_ID_I2C6            = TRUE,
	.int_id_dst[CC_CORE_CA53_CORE1].Bit.bInt_ID_I2C7            = TRUE,
	.int_id_dst[CC_CORE_CA53_CORE1].Bit.bInt_ID_TIMER2          = FALSE,        //This timer is monitor by CORE2=>always disable @ CORE1
	.int_id_dst[CC_CORE_CA53_CORE1].Bit.bInt_ID_GPIO2           = FALSE,

	//INT88
	.int_id_dst[CC_CORE_CA53_CORE1].Bit.bInt_ID_PWM2            = FALSE,
	.int_id_dst[CC_CORE_CA53_CORE1].Bit.bInt_ID_CC2             = FALSE,
	.int_id_dst[CC_CORE_CA53_CORE1].Bit.bInt_ID_SIF2            = TRUE,
	.int_id_dst[CC_CORE_CA53_CORE1].Bit.bInt_ID_DSP_2           = TRUE,

	//INT92
	.int_id_dst[CC_CORE_CA53_CORE1].Bit.bInt_ID_DSP2_2          = TRUE,
	.int_id_dst[CC_CORE_CA53_CORE1].Bit.bInt_ID_TMDEMUX         = TRUE,



	//MIPS2 interrupt destination
	.int_id_dst[CC_CORE_CA53_CORE2].Bit.bInt_ID_TIMER           = FALSE,        //This timer is monitor by CORE1=>always disable @ CORE2
	.int_id_dst[CC_CORE_CA53_CORE2].Bit.bInt_ID_SIE             = FALSE,
	.int_id_dst[CC_CORE_CA53_CORE2].Bit.bInt_ID_SIE2            = FALSE,
	.int_id_dst[CC_CORE_CA53_CORE2].Bit.bInt_ID_SIE3            = FALSE,

	.int_id_dst[CC_CORE_CA53_CORE2].Bit.bInt_ID_SIE4            = FALSE,
	.int_id_dst[CC_CORE_CA53_CORE2].Bit.bInt_ID_IPE             = FALSE,
	.int_id_dst[CC_CORE_CA53_CORE2].Bit.bInt_ID_IME             = FALSE,
	.int_id_dst[CC_CORE_CA53_CORE2].Bit.bInt_ID_DCE             = FALSE,

	.int_id_dst[CC_CORE_CA53_CORE2].Bit.bInt_ID_IFE             = FALSE,
	.int_id_dst[CC_CORE_CA53_CORE2].Bit.bInt_ID_IFE2            = FALSE,
	.int_id_dst[CC_CORE_CA53_CORE2].Bit.bInt_ID_DIS             = FALSE,
	.int_id_dst[CC_CORE_CA53_CORE2].Bit.bInt_ID_FDE             = FALSE,

	.int_id_dst[CC_CORE_CA53_CORE2].Bit.bInt_ID_Ethernet_LPI    = FALSE,
	.int_id_dst[CC_CORE_CA53_CORE2].Bit.bInt_ID_RHE             = FALSE,
	.int_id_dst[CC_CORE_CA53_CORE2].Bit.bInt_ID_DRE             = FALSE,
	.int_id_dst[CC_CORE_CA53_CORE2].Bit.bInt_ID_DAI             = FALSE,

	.int_id_dst[CC_CORE_CA53_CORE2].Bit.bInt_ID_H264            = FALSE,
	.int_id_dst[CC_CORE_CA53_CORE2].Bit.bInt_ID_JPEG            = FALSE,
	.int_id_dst[CC_CORE_CA53_CORE2].Bit.bInt_ID_GRAPHIC         = FALSE,
	.int_id_dst[CC_CORE_CA53_CORE2].Bit.bInt_ID_GRAPHIC2        = FALSE,

	.int_id_dst[CC_CORE_CA53_CORE2].Bit.bInt_ID_AFFINE          = FALSE,
	.int_id_dst[CC_CORE_CA53_CORE2].Bit.bInt_ID_ISE             = FALSE,
	.int_id_dst[CC_CORE_CA53_CORE2].Bit.bInt_ID_TGE             = FALSE,
	.int_id_dst[CC_CORE_CA53_CORE2].Bit.bInt_ID_TSMUX           = FALSE,

	.int_id_dst[CC_CORE_CA53_CORE2].Bit.bInt_ID_GPIO            = FALSE,
	.int_id_dst[CC_CORE_CA53_CORE2].Bit.bInt_ID_REMOTE          = FALSE,
	.int_id_dst[CC_CORE_CA53_CORE2].Bit.bInt_ID_PWM             = FALSE,
#if defined(_CPU2_LINUX_)
	.int_id_dst[CC_CORE_CA53_CORE2].Bit.bInt_ID_USB             = TRUE,
#else
	.int_id_dst[CC_CORE_CA53_CORE2].Bit.bInt_ID_USB             = FALSE,
#endif
#if defined(_USB3_INT_CORE1_)
	.int_id_dst[CC_CORE_CA53_CORE2].Bit.bInt_ID_USB3            = FALSE,
#else
	.int_id_dst[CC_CORE_CA53_CORE2].Bit.bInt_ID_USB3            = TRUE,
#endif
#if defined(_CPU2_LINUX_)
	.int_id_dst[CC_CORE_CA53_CORE2].Bit.bInt_ID_NAND            = TRUE,
	.int_id_dst[CC_CORE_CA53_CORE2].Bit.bInt_ID_SDIO            = TRUE,
	.int_id_dst[CC_CORE_CA53_CORE2].Bit.bInt_ID_SDIO2           = TRUE,
	.int_id_dst[CC_CORE_CA53_CORE2].Bit.bInt_ID_SDIO3           = TRUE,
#else
	.int_id_dst[CC_CORE_CA53_CORE2].Bit.bInt_ID_NAND            = FALSE,
	.int_id_dst[CC_CORE_CA53_CORE2].Bit.bInt_ID_SDIO            = FALSE,
	.int_id_dst[CC_CORE_CA53_CORE2].Bit.bInt_ID_SDIO2           = FALSE,
	.int_id_dst[CC_CORE_CA53_CORE2].Bit.bInt_ID_SDIO3           = FALSE,
#endif

	.int_id_dst[CC_CORE_CA53_CORE2].Bit.bInt_ID_DMA             = FALSE,
#if defined(_CPU2_LINUX_)
	.int_id_dst[CC_CORE_CA53_CORE2].Bit.bInt_ID_ETHERNET        = TRUE,
#else
	.int_id_dst[CC_CORE_CA53_CORE2].Bit.bInt_ID_ETHERNET        = FALSE,
#endif
	.int_id_dst[CC_CORE_CA53_CORE2].Bit.bInt_ID_SPI             = TRUE,

	.int_id_dst[CC_CORE_CA53_CORE2].Bit.bInt_ID_SPI2            = FALSE,
	.int_id_dst[CC_CORE_CA53_CORE2].Bit.bInt_ID_SPI3            = FALSE,
	.int_id_dst[CC_CORE_CA53_CORE2].Bit.bInt_ID_SPI4            = FALSE,
	.int_id_dst[CC_CORE_CA53_CORE2].Bit.bInt_ID_Ehternet_RevMII = FALSE,

	.int_id_dst[CC_CORE_CA53_CORE2].Bit.bInt_ID_SIF             = FALSE,
	.int_id_dst[CC_CORE_CA53_CORE2].Bit.bInt_ID_I2C             = FALSE,
	.int_id_dst[CC_CORE_CA53_CORE2].Bit.bInt_ID_I2C2            = FALSE,
	.int_id_dst[CC_CORE_CA53_CORE2].Bit.bInt_ID_UART            = FALSE,
	.int_id_dst[CC_CORE_CA53_CORE2].Bit.bInt_ID_UART2           = TRUE,
	.int_id_dst[CC_CORE_CA53_CORE2].Bit.bInt_ID_UART3           = FALSE,
	.int_id_dst[CC_CORE_CA53_CORE2].Bit.bInt_ID_UART4           = FALSE,
	.int_id_dst[CC_CORE_CA53_CORE2].Bit.bInt_ID_ADC             = FALSE,

	.int_id_dst[CC_CORE_CA53_CORE2].Bit.bInt_ID_IDE             = FALSE,
	.int_id_dst[CC_CORE_CA53_CORE2].Bit.bInt_ID_IDE2            = FALSE,
	.int_id_dst[CC_CORE_CA53_CORE2].Bit.bInt_ID_DSI             = FALSE,
	.int_id_dst[CC_CORE_CA53_CORE2].Bit.bInt_ID_MI              = FALSE,

	.int_id_dst[CC_CORE_CA53_CORE2].Bit.bInt_ID_HDMI            = FALSE,
	.int_id_dst[CC_CORE_CA53_CORE2].Bit.bInt_ID_DMA2            = FALSE,
	.int_id_dst[CC_CORE_CA53_CORE2].Bit.bInt_ID_LVDS            = FALSE,
	.int_id_dst[CC_CORE_CA53_CORE2].Bit.bInt_ID_LVDS2           = FALSE,

	.int_id_dst[CC_CORE_CA53_CORE2].Bit.bInt_ID_RTC             = FALSE,
	.int_id_dst[CC_CORE_CA53_CORE2].Bit.bInt_ID_WDT             = FALSE,
	.int_id_dst[CC_CORE_CA53_CORE2].Bit.bInt_ID_CG              = FALSE,
	.int_id_dst[CC_CORE_CA53_CORE2].Bit.bInt_ID_CC              = FALSE,

	.int_id_dst[CC_CORE_CA53_CORE2].Bit.bInt_ID_I2C3            = FALSE,
	.int_id_dst[CC_CORE_CA53_CORE2].Bit.bInt_ID_SLVS_EC         = FALSE,
	.int_id_dst[CC_CORE_CA53_CORE2].Bit.bInt_ID_LVDS3           = FALSE,
	.int_id_dst[CC_CORE_CA53_CORE2].Bit.bInt_ID_LVDS4           = FALSE,

	//INT64
	.int_id_dst[CC_CORE_CA53_CORE2].Bit.bInt_ID_LVDS5           = FALSE,
	.int_id_dst[CC_CORE_CA53_CORE2].Bit.bInt_ID_LVDS6           = FALSE,
	.int_id_dst[CC_CORE_CA53_CORE2].Bit.bInt_ID_LVDS7           = FALSE,
	.int_id_dst[CC_CORE_CA53_CORE2].Bit.bInt_ID_LVDS8           = FALSE,

	//INT68
	.int_id_dst[CC_CORE_CA53_CORE2].Bit.bInt_ID_SIE5            = FALSE,
	.int_id_dst[CC_CORE_CA53_CORE2].Bit.bInt_ID_SIE6            = FALSE,
	.int_id_dst[CC_CORE_CA53_CORE2].Bit.bInt_ID_SIE7            = FALSE,
	.int_id_dst[CC_CORE_CA53_CORE2].Bit.bInt_ID_SIE8            = FALSE,

	//INT72
	.int_id_dst[CC_CORE_CA53_CORE2].Bit.bInt_ID_IVE             = FALSE,
	.int_id_dst[CC_CORE_CA53_CORE2].Bit.bInt_ID_SVM             = FALSE,
	.int_id_dst[CC_CORE_CA53_CORE2].Bit.bInt_ID_SDE             = FALSE,
	.int_id_dst[CC_CORE_CA53_CORE2].Bit.bInt_ID_CNN             = FALSE,

	//INT76
	.int_id_dst[CC_CORE_CA53_CORE2].Bit.bInt_ID_DSP             = FALSE,
	.int_id_dst[CC_CORE_CA53_CORE2].Bit.bInt_ID_DSP2            = FALSE,
	.int_id_dst[CC_CORE_CA53_CORE2].Bit.bInt_ID_CANBUS          = FALSE,
	.int_id_dst[CC_CORE_CA53_CORE2].Bit.bInt_ID_CRYPTO          = FALSE,

	//INT80
	.int_id_dst[CC_CORE_CA53_CORE2].Bit.bInt_ID_HWCOPY          = FALSE,
	.int_id_dst[CC_CORE_CA53_CORE2].Bit.bInt_ID_ROTATE          = FALSE,
	.int_id_dst[CC_CORE_CA53_CORE2].Bit.bInt_ID_I2C4            = FALSE,
	.int_id_dst[CC_CORE_CA53_CORE2].Bit.bInt_ID_I2C5            = FALSE,

	//INT84
	.int_id_dst[CC_CORE_CA53_CORE2].Bit.bInt_ID_I2C6            = FALSE,
	.int_id_dst[CC_CORE_CA53_CORE2].Bit.bInt_ID_I2C7            = FALSE,
#if (_EMULATION_MULTI_CPU_ == ENABLE)
	.int_id_dst[CC_CORE_CA53_CORE2].Bit.bInt_ID_TIMER2          = TRUE,
#else
	.int_id_dst[CC_CORE_CA53_CORE2].Bit.bInt_ID_TIMER2          = FALSE,
#endif
	.int_id_dst[CC_CORE_CA53_CORE2].Bit.bInt_ID_GPIO2           = TRUE,

	//INT88
	.int_id_dst[CC_CORE_CA53_CORE2].Bit.bInt_ID_PWM2            = TRUE,
	.int_id_dst[CC_CORE_CA53_CORE2].Bit.bInt_ID_CC2             = TRUE,
	.int_id_dst[CC_CORE_CA53_CORE2].Bit.bInt_ID_SIF2            = FALSE,
	.int_id_dst[CC_CORE_CA53_CORE2].Bit.bInt_ID_DSP_2           = FALSE,

	//INT92
	.int_id_dst[CC_CORE_CA53_CORE2].Bit.bInt_ID_DSP2_2          = FALSE,
	.int_id_dst[CC_CORE_CA53_CORE2].Bit.bInt_ID_TMDEMUX         = FALSE,
#if 0
	//DSP interrupt destination
	.int_id_dst[CC_CORE_DSP].Bit.bInt_ID_TIMER      = FALSE,
	.int_id_dst[CC_CORE_DSP].Bit.bInt_ID_SIE        = FALSE,
	.int_id_dst[CC_CORE_DSP].Bit.bInt_ID_SIE2       = FALSE,
	.int_id_dst[CC_CORE_DSP].Bit.bInt_ID_SIE3       = FALSE,

	.int_id_dst[CC_CORE_DSP].Bit.bInt_ID_SIE4       = FALSE,
	.int_id_dst[CC_CORE_DSP].Bit.bInt_ID_IPE        = FALSE,
	.int_id_dst[CC_CORE_DSP].Bit.bInt_ID_IME        = FALSE,
	.int_id_dst[CC_CORE_DSP].Bit.bInt_ID_DCE        = FALSE,

	.int_id_dst[CC_CORE_DSP].Bit.bInt_ID_IFE        = FALSE,
	.int_id_dst[CC_CORE_DSP].Bit.bInt_ID_IFE2       = FALSE,
	.int_id_dst[CC_CORE_DSP].Bit.bInt_ID_DIS        = FALSE,
	.int_id_dst[CC_CORE_DSP].Bit.bInt_ID_FDE        = FALSE,

	.int_id_dst[CC_CORE_DSP].Bit.bInt_ID_RDE        = FALSE,
	.int_id_dst[CC_CORE_DSP].Bit.bInt_ID_RHE        = FALSE,
	.int_id_dst[CC_CORE_DSP].Bit.bInt_ID_DRE        = FALSE,
	.int_id_dst[CC_CORE_DSP].Bit.bInt_ID_DAI        = FALSE,

	.int_id_dst[CC_CORE_DSP].Bit.bInt_ID_H264       = FALSE,
	.int_id_dst[CC_CORE_DSP].Bit.bInt_ID_JPEG       = FALSE,
	.int_id_dst[CC_CORE_DSP].Bit.bInt_ID_GRAPHIC    = FALSE,
	.int_id_dst[CC_CORE_DSP].Bit.bInt_ID_GRAPHIC2   = FALSE,

	.int_id_dst[CC_CORE_DSP].Bit.bInt_ID_AFFINE     = FALSE,
	.int_id_dst[CC_CORE_DSP].Bit.bInt_ID_ISE        = FALSE,
	.int_id_dst[CC_CORE_DSP].Bit.bInt_ID_TGE        = FALSE,
	.int_id_dst[CC_CORE_DSP].Bit.bInt_ID_TSMUX      = FALSE,

	.int_id_dst[CC_CORE_DSP].Bit.bInt_ID_GPIO       = FALSE,
	.int_id_dst[CC_CORE_DSP].Bit.bInt_ID_REMOTE     = FALSE,
	.int_id_dst[CC_CORE_DSP].Bit.bInt_ID_PWM        = FALSE,
	.int_id_dst[CC_CORE_DSP].Bit.bInt_ID_USB        = FALSE,

	.int_id_dst[CC_CORE_DSP].Bit.bInt_ID_USB2       = FALSE,
	.int_id_dst[CC_CORE_DSP].Bit.bInt_ID_NAND       = FALSE,
	.int_id_dst[CC_CORE_DSP].Bit.bInt_ID_SDIO       = FALSE,
	.int_id_dst[CC_CORE_DSP].Bit.bInt_ID_SDIO2      = FALSE,

	.int_id_dst[CC_CORE_DSP].Bit.bInt_ID_SDIO3      = FALSE,
	.int_id_dst[CC_CORE_DSP].Bit.bInt_ID_DMA        = FALSE,
	.int_id_dst[CC_CORE_DSP].Bit.bInt_ID_ETHERNET   = FALSE,
	.int_id_dst[CC_CORE_DSP].Bit.bInt_ID_SPI        = FALSE,

	.int_id_dst[CC_CORE_DSP].Bit.bInt_ID_SPI2       = FALSE,
	.int_id_dst[CC_CORE_DSP].Bit.bInt_ID_SPI3       = FALSE,
	.int_id_dst[CC_CORE_DSP].Bit.bInt_ID_SPI4       = FALSE,
	.int_id_dst[CC_CORE_DSP].Bit.bInt_ID_SPI5       = FALSE,

	.int_id_dst[CC_CORE_DSP].Bit.bInt_ID_SIF        = FALSE,
	.int_id_dst[CC_CORE_DSP].Bit.bInt_ID_I2C        = FALSE,
	.int_id_dst[CC_CORE_DSP].Bit.bInt_ID_I2C2       = FALSE,
	.int_id_dst[CC_CORE_DSP].Bit.bInt_ID_UART       = FALSE,

	.int_id_dst[CC_CORE_DSP].Bit.bInt_ID_UART2      = FALSE,
	.int_id_dst[CC_CORE_DSP].Bit.bInt_ID_UART3      = FALSE,
	.int_id_dst[CC_CORE_DSP].Bit.bInt_ID_UART4      = FALSE,
	.int_id_dst[CC_CORE_DSP].Bit.bInt_ID_ADC        = FALSE,

	.int_id_dst[CC_CORE_DSP].Bit.bInt_ID_IDE        = FALSE,
	.int_id_dst[CC_CORE_DSP].Bit.bInt_ID_IDE2       = FALSE,
	.int_id_dst[CC_CORE_DSP].Bit.bInt_ID_DSI        = FALSE,
	.int_id_dst[CC_CORE_DSP].Bit.bInt_ID_MI         = FALSE,

	.int_id_dst[CC_CORE_DSP].Bit.bInt_ID_HDMI       = FALSE,
	.int_id_dst[CC_CORE_DSP].Bit.bInt_ID_VX1        = FALSE,
	.int_id_dst[CC_CORE_DSP].Bit.bInt_ID_LVDS       = FALSE,
	.int_id_dst[CC_CORE_DSP].Bit.bInt_ID_LVDS2      = FALSE,

	.int_id_dst[CC_CORE_DSP].Bit.bInt_ID_RTC        = TRUE,
	.int_id_dst[CC_CORE_DSP].Bit.bInt_ID_WDT        = FALSE,
	.int_id_dst[CC_CORE_DSP].Bit.bInt_ID_CG         = FALSE,
	.int_id_dst[CC_CORE_DSP].Bit.bInt_ID_CC         = FALSE,

	.int_id_dst[CC_CORE_DSP].Bit.bInt_ID_I2C3       = FALSE,


	//DSP interrupt destination
	.int_id_dst[CC_CORE_DSP2].Bit.bInt_ID_TIMER      = FALSE,
	.int_id_dst[CC_CORE_DSP2].Bit.bInt_ID_SIE        = FALSE,
	.int_id_dst[CC_CORE_DSP2].Bit.bInt_ID_SIE2       = FALSE,
	.int_id_dst[CC_CORE_DSP2].Bit.bInt_ID_SIE3       = FALSE,

	.int_id_dst[CC_CORE_DSP2].Bit.bInt_ID_SIE4       = FALSE,
	.int_id_dst[CC_CORE_DSP2].Bit.bInt_ID_IPE        = FALSE,
	.int_id_dst[CC_CORE_DSP2].Bit.bInt_ID_IME        = FALSE,
	.int_id_dst[CC_CORE_DSP2].Bit.bInt_ID_DCE        = FALSE,

	.int_id_dst[CC_CORE_DSP2].Bit.bInt_ID_IFE        = FALSE,
	.int_id_dst[CC_CORE_DSP2].Bit.bInt_ID_IFE2       = FALSE,
	.int_id_dst[CC_CORE_DSP2].Bit.bInt_ID_DIS        = FALSE,
	.int_id_dst[CC_CORE_DSP2].Bit.bInt_ID_FDE        = FALSE,

	.int_id_dst[CC_CORE_DSP2].Bit.bInt_ID_RDE        = FALSE,
	.int_id_dst[CC_CORE_DSP2].Bit.bInt_ID_RHE        = FALSE,
	.int_id_dst[CC_CORE_DSP2].Bit.bInt_ID_DRE        = FALSE,
	.int_id_dst[CC_CORE_DSP2].Bit.bInt_ID_DAI        = FALSE,

	.int_id_dst[CC_CORE_DSP2].Bit.bInt_ID_H264       = FALSE,
	.int_id_dst[CC_CORE_DSP2].Bit.bInt_ID_JPEG       = FALSE,
	.int_id_dst[CC_CORE_DSP2].Bit.bInt_ID_GRAPHIC    = FALSE,
	.int_id_dst[CC_CORE_DSP2].Bit.bInt_ID_GRAPHIC2   = FALSE,

	.int_id_dst[CC_CORE_DSP2].Bit.bInt_ID_AFFINE     = FALSE,
	.int_id_dst[CC_CORE_DSP2].Bit.bInt_ID_ISE        = FALSE,
	.int_id_dst[CC_CORE_DSP2].Bit.bInt_ID_TGE        = FALSE,
	.int_id_dst[CC_CORE_DSP2].Bit.bInt_ID_TSMUX      = FALSE,

	.int_id_dst[CC_CORE_DSP2].Bit.bInt_ID_GPIO       = FALSE,
	.int_id_dst[CC_CORE_DSP2].Bit.bInt_ID_REMOTE     = FALSE,
	.int_id_dst[CC_CORE_DSP2].Bit.bInt_ID_PWM        = FALSE,
	.int_id_dst[CC_CORE_DSP2].Bit.bInt_ID_USB        = FALSE,

	.int_id_dst[CC_CORE_DSP2].Bit.bInt_ID_USB2       = FALSE,
	.int_id_dst[CC_CORE_DSP2].Bit.bInt_ID_NAND       = FALSE,
	.int_id_dst[CC_CORE_DSP2].Bit.bInt_ID_SDIO       = FALSE,
	.int_id_dst[CC_CORE_DSP2].Bit.bInt_ID_SDIO2      = FALSE,

	.int_id_dst[CC_CORE_DSP2].Bit.bInt_ID_SDIO3      = FALSE,
	.int_id_dst[CC_CORE_DSP2].Bit.bInt_ID_DMA        = FALSE,
	.int_id_dst[CC_CORE_DSP2].Bit.bInt_ID_ETHERNET   = FALSE,
	.int_id_dst[CC_CORE_DSP2].Bit.bInt_ID_SPI        = FALSE,

	.int_id_dst[CC_CORE_DSP2].Bit.bInt_ID_SPI2       = FALSE,
	.int_id_dst[CC_CORE_DSP2].Bit.bInt_ID_SPI3       = FALSE,
	.int_id_dst[CC_CORE_DSP2].Bit.bInt_ID_SPI4       = FALSE,
	.int_id_dst[CC_CORE_DSP2].Bit.bInt_ID_SPI5       = FALSE,

	.int_id_dst[CC_CORE_DSP2].Bit.bInt_ID_SIF        = FALSE,
	.int_id_dst[CC_CORE_DSP2].Bit.bInt_ID_I2C        = FALSE,
	.int_id_dst[CC_CORE_DSP2].Bit.bInt_ID_I2C2       = FALSE,
	.int_id_dst[CC_CORE_DSP2].Bit.bInt_ID_UART       = FALSE,

	.int_id_dst[CC_CORE_DSP2].Bit.bInt_ID_UART2      = FALSE,
	.int_id_dst[CC_CORE_DSP2].Bit.bInt_ID_UART3      = FALSE,
	.int_id_dst[CC_CORE_DSP2].Bit.bInt_ID_UART4      = FALSE,
	.int_id_dst[CC_CORE_DSP2].Bit.bInt_ID_ADC        = FALSE,

	.int_id_dst[CC_CORE_DSP2].Bit.bInt_ID_IDE        = FALSE,
	.int_id_dst[CC_CORE_DSP2].Bit.bInt_ID_IDE2       = FALSE,
	.int_id_dst[CC_CORE_DSP2].Bit.bInt_ID_DSI        = FALSE,
	.int_id_dst[CC_CORE_DSP2].Bit.bInt_ID_MI         = FALSE,

	.int_id_dst[CC_CORE_DSP2].Bit.bInt_ID_HDMI       = FALSE,
	.int_id_dst[CC_CORE_DSP2].Bit.bInt_ID_VX1        = FALSE,
	.int_id_dst[CC_CORE_DSP2].Bit.bInt_ID_LVDS       = FALSE,
	.int_id_dst[CC_CORE_DSP2].Bit.bInt_ID_LVDS2      = FALSE,

	.int_id_dst[CC_CORE_DSP2].Bit.bInt_ID_RTC        = TRUE,
	.int_id_dst[CC_CORE_DSP2].Bit.bInt_ID_WDT        = FALSE,
	.int_id_dst[CC_CORE_DSP2].Bit.bInt_ID_CG         = FALSE,
	.int_id_dst[CC_CORE_DSP2].Bit.bInt_ID_CC         = FALSE,

	.int_id_dst[CC_CORE_DSP].Bit.bInt_ID_I2C3        = FALSE,
#endif
};

MODELEXT_HEADER ind_intdir_cfg_header __attribute__((section("modelext_header.intdir_cfg"))) = {
	.size = sizeof(ind_intdir_cfg_data) + sizeof(MODELEXT_HEADER),
	.type = MODELEXT_TYPE_INTDIR_CFG,
	.number = 1,
	.version = INT_GIC_ID_DST_VER,
};
