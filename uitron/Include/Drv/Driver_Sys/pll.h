/**
    PLL Configuration module header

    PLL Configuration module header file.

    @file       pll.h
    @ingroup    mIDrvSys_CG
    @note       Nothing

    Copyright   Novatek Microelectronics Corp. 2016.  All rights reserved.
*/

#ifndef _PLL_H
#define _PLL_H

#include "Type.h"

/**
    @addtogroup mIHALSysCG
*/
//@{

/**
    Clock Enable ID

    This is for pll_enableClock() and pll_disableClock().
*/
typedef enum {
	DMA_CLK = 0,        ///< DMA(SDRAM) clock
	DMA2_CLK,           ///< DMS2(SDRAM2) clock
	SIE_MCLK,           ///< SIE MCLK
	SIE_MCLK2,          ///< SIE MCLK2
	SIE_CLK,            ///< SIE clock
	SIE2_CLK,           ///< SIE2 Clock
	TGE_CLK,            ///< TGE clock
	IPE_CLK,            ///< IPE clock
	DIS_CLK = 8,        ///< DIS clock
	IME_CLK,            ///< IME clock
	FDE_CLK,            ///< FDE clock
	ISE_CLK,            ///< ISE clock
	SP_CLK,             ///< special clock
	IFE_CLK,            ///< IFE clock
	DCE_CLK,            ///< DCE clock
	IFE2_CLK,           ///< IFE2 clock
	IDE1_CLK = 16,      ///< IDE clock
	IDE2_CLK,           ///< IDE2 clock
	TV_CLK,             ///< TV clock
	MI_CLK = 20,        ///< MI clock
	CEC_CLK,            ///< CEC clock
	HDMI_CLK,           ///< HDMI clock
	CRYPTO_CLK,         ///< Crypto clock
	H26X_CLK = 24,      ///< H.264 clock
	AFFINE_CLK,         ///< Affine clock
	JPG_CLK,            ///< JPEG clock
	GRAPH_CLK,          ///< Graphic clock
	GRAPH2_CLK,         ///< Graphic2 clock
	DAI_CLK,            ///< DAI clock
	EAC_A_ADC_CLK,      ///< EAC analog AD clock
	EAC_A_DAC_CLK,      ///< EAC analog DA clock

	NAND_CLK = 32,      ///< NAND clock
	SDIO_CLK = 34,      ///< SDIO clock
	SDIO2_CLK,          ///< SDIO2 clock
	I2C_CLK,            ///< I2C clock
	I2C2_CLK,           ///< I2C2 clock
	SPI_CLK,            ///< SPI clock
	SPI2_CLK,           ///< SPI2 clock
	SPI3_CLK = 40,      ///< SPI3 clock
	SIF_CLK,            ///< SIF clock
	UART_CLK,           ///< UART clock
	UART2_CLK,          ///< UART2 clock
	REMOTE_CLK,         ///< Remote clock
	ADC_CLK,            ///< ADC clock
	SDIO3_CLK,          ///< SDIO3 clock
	SPI4_CLK,           ///< SPI4 clock
	RTC_CLK = 48,       ///< RTC clock
	WDT_CLK,            ///< WDT clock
	TMR_CLK,            ///< Timer clock
	USB_CLK,            ///< USB clock
	EAC_D_CLKEN,        ///< EAC digital clock
	SPI5_CLK,           ///< SPI5 PX clock
	UART3_CLK,          ///< UART3 clock
	UART4_CLK,          ///< UART4 clock
	EFUSE_CLK = 60,     ///< EFUSE clock
	ETH_CLK,            ///< ETH clock
	SP2_CLK,            ///< SP2 clock
	I2C3_CLK,           ///< I2C3 clock

	LVDS_CLK = 65,      ///< LVDS clock
	MIPI_CLK = 65,      ///< MIPI clock
	MIPI_LVDS_CLK = 65, ///< MIPI/LVDS clock
	MIPI2_CLK = 66,     ///< MIPI2 clock
	MIPI_LVDS2_CLK = 66,///< MIPI/LVDS2 clock
	MIPI_DSI_CLK,       ///< MIPI DSI clock
	SIE_PXCLK,          ///< SIE PX clock
	SIE2_PXCLK,         ///< SIE2 PX clock
	RDE_CLK,            ///< RDE clock
	PWM_CCNT_CLK = 72,  ///< PWM CCNT clock
	PWM_CCNT0_CLK = 72, ///< PWM CCNT0 clock
	PWM_CCNT1_CLK,      ///< PWM CCNT1 clock
	PWM_CCNT2_CLK,      ///< PWM CCNT2 clock
	PWM_CCNT3_CLK,      ///< PWM CCNT3 clock
	DRE_CLK,            ///< DRE clock
	TSMUX_CLK,          ///< TSMUX clock
	Vx1_CLK,            ///< Vx1 clock
	SIE3_CLK,           ///< SIE3 clock
	SIE4_CLK,           ///< SIE4 clock
	USB2_CLK,           ///< USB2 clock
	CC_CLK,             ///< CC clock
	RHE_CLK,            ///< RHE clock
	Vx1_SLCLK,          ///< Vx1 sub-link clock
	Vx1_SL2CLK = 86,    ///< Vx1 sub-link2 clock
	Vx1_MLCLK,          ///< Vx1 main-link clock
	Vx1_ML2CLK,         ///< Vx1 main-link2 clock
	CANBUS_CLK,         ///< CANBUX clock
	I2C4_CLK,           ///< I2C4 clock
	I2C5_CLK,           ///< I2C5 clock
	I2C6_CLK,           ///< I2C6 clock
	I2C7_CLK,           ///< I2C7 clock
	HWCPY_CLK,          ///< HWCPY clock
	RORATE_CLK,         ///< ROTATE clock

	PWM0_CLK = 96,      ///< PWM0 clock
	PWM1_CLK,           ///< PWM1 clock
	PWM2_CLK,           ///< PWM2 clock
	PWM3_CLK,           ///< PWM3 clock
	PWM4_CLK,           ///< PWM4 clock
	PWM5_CLK,           ///< PWM5 clock
	PWM6_CLK,           ///< PWM6 clock
	PWM7_CLK,           ///< PWM7 clock
	PWM8_CLK,           ///< PWM8 clock
	PWM9_CLK,           ///< PWM9 clock
	PWM10_CLK,          ///< PWM10 clock
	PWM11_CLK,          ///< PWM11 clock
	PWM12_CLK,          ///< PWM12 clock
	PWM13_CLK,          ///< PWM13 clock
	PWM14_CLK,          ///< PWM14 clock
	PWM15_CLK,          ///< PWM15 clock
	PWM16_CLK,          ///< PWM16 clock
	PWM17_CLK,          ///< PWM17 clock
	PWM18_CLK,          ///< PWM18 clock
	PWM19_CLK,          ///< PWM19 clock
	MIPI_LVDS3_CLK,     ///< MIPI/LVDS3 clock
	MIPI_LVDS4_CLK,     ///< MIPI/LVDS4 clock
	MIPI_LVDS5_CLK,     ///< MIPI/LVDS5 clock
	MIPI_LVDS6_CLK,     ///< MIPI/LVDS6 clock
	MIPI_LVDS7_CLK,     ///< MIPI/LVDS7 clock
	MIPI_LVDS8_CLK,     ///< MIPI/LVDS8 clock
	SIE5_CLK,           ///< SIE5 clock
	SIE6_CLK,           ///< SIE6 clock
	SIE7_CLK,           ///< SIE7 clock
	SIE8_CLK,           ///< SIE8 clock
	IVE_CLK,            ///< IVE clock
	SDE_CLK = 127,      ///< SDE/CNN clock
	SDE_CNN_CLK = 127,  ///< SDE/CNN clock
	CNN_CLK = 127,      ///< SDE/CNN clock

	SVM_CLK = 128,      ///< SVM clock
	ISE2_CLK = 130,     ///< ISE2 clock
	USB3_USP_CLK = 136, ///< USB3 suspend clock (12MHz)
	USB3_REF_ITP_CLK,   ///< USB3 ref ipt clock (24MHz)
	SLVSEC_CLK,			///< SLVSEC clock
	DSP_CLK = 139,      ///< DSP clock
	DSP2_CLK,           ///< DSP2 clock
	CPU_CORETIMER_CLK,  ///< CPU core timer clock
	SIE3_PXCLK,         ///< SIE3 PX clock
	SIE4_PXCLK,         ///< SIE4 PX clock
	SIE5_PXCLK,         ///< SIE5 PX clock
	SIE6_PXCLK,         ///< SIE6 PX clock
	SIE7_PXCLK,         ///< SIE7 PX clock
	SIE8_PXCLK,         ///< SIE8 PX clock
	TSDEMUX_CLK,        ///< TSDEMUX clock
	DRTC_CLK = 150,     ///< DRTC clock

	ENUM_DUMMY4WORD(CG_EN)
} CG_EN;

#define PRE_CLK     0               ///< Backward compatible
#define CPU2_CLK    159             ///< Backward compatible

/**
    Clock Gating Select ID

    This is for pll_setClkAutoGating() / pll_clearClkAutoGating() /
    pll_getClkAutoGating() / pll_setPclkAutoGating() /
    pll_clearPclkAutoGating() / pll_getPclkAutoGating().
*/
typedef enum {
	SIE_GCLK,           ///< Gating SIE clock (Gating PCLK only)
	PRE_GCLK,           ///< Backward compatible
	IPE_GCLK,           ///< Gating IPE clock
	IME_GCLK,           ///< Gating IME clock
	DIS_GCLK,           ///< Gating DIS clock
	FDE_GCLK,           ///< Gating FDE clock
	DCE_GCLK,           ///< Gating DCE clock
	IFE_GCLK,           ///< Gating IFE clock
	GRA_GCLK = 8,       ///< Gating Graphic clock
	GRA2_GCLK,          ///< Gating Graphic2 clock
	IDE_GCLK,           ///< Gating IDE clock  (Gating PCLK only)
	IDE2_GCLK,          ///< Gating IDE2 clock  (Gating PCLK only)
	MI_GCLK,            ///< Gating MI clock  (Gating PCLK only)
	HDMI_GCLK = 13,     ///< Gating HDMI PCLK (Gating PCLK only)
	IFE2_GCLK = 14,     ///< Gating IFE2 clock
	JPG_GCLK,           ///< Gating JPEG clock
	H264_GCLK = 16,     ///< Gating H264 clock
	DAI_GCLK,           ///< Gating DAI clock  (Gating PCLK only)
	EAC_GCLK,           ///< Gating EAC clock  (Gating PCLK only)
	NAND_GCLK,          ///< Gating xD/Nand clock
	SDIO_GCLK,          ///< Gating SDIO clock
	SDIO2_GCLK,         ///< Gating SDIO2 clock
	I2C_GCLK,           ///< Gating I2C clock  (Gating PCLK only)
	I2C2_GCLK,          ///< Gating I2C2 clock  (Gating PCLK only)
	SPI_GCLK = 24,      ///< Gating SPI clock
	SPI2_GCLK,          ///< Gating SPI2 clock
	SPI3_GCLK,          ///< Gating SPI3 clock
	SIF_GCLK,           ///< Gating SIF clock
	UART_GCLK,          ///< Gating UART clock  (Gating PCLK only)
	UART2_GCLK,         ///< Gating UART2 clock  (Gating PCLK only)
	RM_GCLK,            ///< Gating Remote clock  (Gating PCLK only)
	ADC_GCLK,           ///< Gating ADC clock  (Gating PCLK only)

	TMR_GCLK = 32,      ///< Gating TMR clock  (Gating PCLK only)
	WDT_GCLK,           ///< Gating Watchdog clock  (Gating PCLK only)
	LVDS_GCLK = 35,     ///< Gating LVDS clock  (Gating PCLK only)
	MIPI_GCLK = 35,     ///< Gating MIPI clock  (Gating PCLK only)
	MIPI_LVDS_GCLK = 35,///< Gating MIPI/LVDS clock  (Gating PCLK only)
	MIPI2_GCLK = 36,    ///< Gating MIPI2 clock  (Gating PCLK only)
	MIPI_LVDS2_GCLK = 36,///< Gating MIPI/LVDS2 clock  (Gating PCLK only)
	TV_GCLK,            ///< Gating TV clock  (Gating PCLK only)
	MIPI_DSI_GCLK,      ///< Gating MIPI DSI clock  (Gating PCLK only)
	ISE_GCLK = 40,      ///< Gating ISE clock
	SIE2_GCLK,          ///< Gating SIE2 clock  (Gating PCLK only)
	AFFINE_GCLK,        ///< Gating AFFINE clock
	PWM_GCLK,           ///< Gating PWM clock
	PWMCCNT_GCLK,       ///<  Backward compatible
	RDE_GCLK = 45,      ///< Gating RDE clock
	DRE_GCLK,           ///< Gating DRE clock
	SDIO3_GCLK,         ///< Gating SDIO3 clock
	UART3_GCLK,         ///< Gating UART3 clock (Gating PCLK only)
	UART4_GCLK,         ///< Gating UART4 clock (Gating PCLK only)
	SPI4_GCLK,          ///< Gating SPI4 clock
	SPI5_GCLK,          ///< Gating SPI5 clock
	TGE_GCLK,           ///< Gating TGE clock (Gating PCLK only)
	SIE3_GCLK,          ///< Gating SIE3 clock (Gating PCLK only)
	SIE4_GCLK,          ///< Gating SIE4 clock (Gating PCLK only)
	RHE_GCLK,           ///< Gating RHE clock (Gating PCLK only)
	I2C3_GCLK,          ///< Gating I2C3 clock (Gating PCLK only)
	I2C4_GCLK,          ///< Gating I2C4 clock (Gating PCLK only)
	I2C5_GCLK,          ///< Gating I2C5 clock (Gating PCLK only)
	I2C6_GCLK,          ///< Gating I2C6 clock (Gating PCLK only)
	I2C7_GCLK,          ///< Gating I2C7 clock (Gating PCLK only)
	HWCPY_GCLK,         ///< Gating HWCPY clock
	RORATE_GCLK,        ///< Gating RORATE clock

	GPIO_GCLK = 64,     ///< Gating GPIO PCLK (Gating PCLK only)
	INTC_GCLK,          ///< Gating INTC PCLK (Gating PCLK only)
	DMA_GCLK = 67,      ///< Gating DMAC PCLK (Gating PCLK only)
	DMA2_GCLK,          ///< Gating DMAC2 PCLK
	IVE_GCLK,           ///< Gating IVE clock
	SDE_GCLK,           ///< Gating SDE clock
	SVM_GCLK,           ///< Gating SVM clock
	CNN_GCLK,           ///< Gating CNN clock
	MIPI_LVDS3_GCLK,    ///< Gating MIPI/LVDS3 clock  (Gating PCLK only)
	MIPI_LVDS4_GCLK,    ///< Gating MIPI/LVDS4 clock  (Gating PCLK only)
	MIPI_LVDS5_GCLK,    ///< Gating MIPI/LVDS5 clock  (Gating PCLK only)
	MIPI_LVDS6_GCLK,    ///< Gating MIPI/LVDS6 clock  (Gating PCLK only)
	MIPI_LVDS7_GCLK,    ///< Gating MIPI/LVDS7 clock  (Gating PCLK only)
	MIPI_LVDS8_GCLK,    ///< Gating MIPI/LVDS8 clock  (Gating PCLK only)
	SIE5_GCLK,          ///< Gating SIE5 clock (Gating PCLK only)
	SIE6_GCLK,          ///< Gating SIE6 clock (Gating PCLK only)
	SIE7_GCLK,          ///< Gating SIE7 clock (Gating PCLK only)
	SIE8_GCLK,          ///< Gating SIE8 clock (Gating PCLK only)
	CANBUS_GCLK,        ///< Gating CANBUS clock (Note: Canbus clock and pclk can't be gated)
	ETH_GCLK,           ///< Gating ETH clock
	TSDEMUX_GCLK,       ///< Gating TSDEMUX clock
	ISE2_G0CLK = 86,    ///< Gating ISE2 clock

	PCLKGAT_MAXNUM,

	ENUM_DUMMY4WORD(GATECLK)
} GATECLK;

/*
    @name Default Gating Clock Select definition

    This is for pll_configClkAutoGating() & pll_configPclkAutoGating().
*/
//@{
#define PLL_CLKSEL_DEFAULT_CLKGATE1     0x00000000
#define PLL_CLKSEL_DEFAULT_CLKGATE2     0x00000000
#define PLL_CLKSEL_DEFAULT_PCLKGATE1    0x00000000
#define PLL_CLKSEL_DEFAULT_PCLKGATE2    0x00000000
#define PLL_CLKSEL_DEFAULT_PCLKGATE3    0x00000000
//@}


/**
    PLL ID
*/
typedef enum {
	PLL_ID_1,               ///< PLL1 (internal 480 MHz)
	PLL_ID_2,               ///< PLL2 (for video/HDMI)
	PLL_ID_3,               ///< PLL3 (for DMA)
	PLL_ID_4,               ///< PLL4 (for SSPLL)
	PLL_ID_5,               ///< PLL5 (for sensor)
	PLL_ID_6,               ///< PLL6 (for IPP)
	PLL_ID_7,               ///< PLL7 (for audio)
	PLL_ID_8,               ///< PLL8 (for CPU)
	PLL_ID_9,               ///< PLL9 (for DSP)
	PLL_ID_10,              ///< PLL10 (for SNMCLK)
	PLL_ID_11,              ///< PLL11 (for DSI)
	PLL_ID_12,              ///< PLL12 (for ETH)
	PLL_ID_13,              ///< PLL13 (for H264)
	PLL_ID_14,              ///< PLL14 (for DMA2)

	ENUM_DUMMY4WORD(PLL_ID)
} PLL_ID;

/**
    PLL Clock ID

    @note This is for pll_setClkFreq(), pll_getClkFreq(), pll_selectClkSrc(), pll_getClkSrc()
*/
typedef enum {
	PLL_CLK_SIEMCLK,        ///< SIE MCLK
	PLL_CLK_SIEMCLK2,       ///< SIE MCLK2
	PLL_CLK_SPCLK,          ///< SP CLK
	PLL_CLK_SPCLK2,         ///< SP2 CLK

	ENUM_DUMMY4WORD(PLL_CLK)
} PLL_CLK;

/**
    PLL Clock Source

    @note This is for pll_selectClkSrc(), pll_getClkSrc() (setting clock source of PLL_CLK_SIEMCLK, PLL_CLK_SIEMCLK2, PLL_CLK_SPCLK, PLL_CLK_SPCLK2) 
    		
*/
typedef enum {
	PLL_CLKSRC_240,         ///< Backward compatible.(not used)
	PLL_CLKSRC_480,         ///< CLK source is 480MHz. (valid when setting PLL_CLK_SIEMCLK, PLL_CLK_SIEMCLK2, PLL_CLK_SPCLK, PLL_CLK_SPCLK2)
	PLL_CLKSRC_PLL2,        ///< Backward compatible.(not used)
	PLL_CLKSRC_PLL4,        ///< CLK source is PLL4. (valid when setting PLL_CLK_SIEMCLK, PLL_CLK_SIEMCLK2, PLL_CLK_SPCLK, PLL_CLK_SPCLK2)
	PLL_CLKSRC_PLL5,        ///< CLK source is PLL5. (valid when setting PLL_CLK_SIEMCLK, PLL_CLK_SIEMCLK2)
	PLL_CLKSRC_PLL10,       ///< CLK source is PLL10. (valid when setting PLL_CLK_SIEMCLK, PLL_CLK_SIEMCLK2, PLL_CLK_SPCLK, PLL_CLK_SPCLK2)
	PLL_CLKSRC_PLL14,       ///< CLK source is PLL14. (valid when setting PLL_CLK_SPCLK, PLL_CLK_SPCLK2)

	PLL_CLKSRC_UNKNOWN,     ///< unkown clock source

	ENUM_DUMMY4WORD(PLL_CLKSRC)
} PLL_CLKSRC;


extern ER       pll_setPLL(PLL_ID id, UINT32 uiSetting);
extern BOOL     pll_getPLLEn(PLL_ID id);
extern ER       pll_setPLLEn(PLL_ID id, BOOL bEnable);
extern UINT32   pll_getPLLFreq(PLL_ID id);
extern ER       pll_setPLLSpreadSpectrum(PLL_ID id, UINT32 uiLowerFreq, UINT32 uiUpperFreq);
extern ER       pll_getPLLSpreadSpectrum(PLL_ID id, UINT32 *puiLowerFreq, UINT32 *puiUpperFreq);

extern void     pll_setPWMClockRate(UINT32 uiPWMNumber, UINT32 uiDiv);
extern BOOL     pll_isClockEnabled(CG_EN Num);

extern void     pll_setClkAutoGating(GATECLK ClkSel);
extern void     pll_clearClkAutoGating(GATECLK ClkSel);
extern BOOL     pll_getClkAutoGating(GATECLK ClkSel);

extern void     pll_setPclkAutoGating(GATECLK ClkSel);
extern void     pll_clearPclkAutoGating(GATECLK ClkSel);
extern BOOL     pll_getPclkAutoGating(GATECLK ClkSel);

extern ER       pll_setClkFreq(PLL_CLK clkID, UINT32 uiFreq);
extern ER       pll_getClkFreq(PLL_CLK clkID, UINT32 *puiFreq);

extern ER       pll_selectClkSrc(PLL_CLK clkID, PLL_CLKSRC clkSrc);
extern ER       pll_getClkSrc(PLL_CLK clkID, PLL_CLKSRC *pClkSrc);

extern ER       pll_setClkEn(PLL_CLK clkID, BOOL bEnable);
extern ER       pll_getClkEn(PLL_CLK clkID, BOOL *pEnable);
//@}

#endif



