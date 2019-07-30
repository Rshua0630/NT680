/**
    PLL Configuration module header

    PLL Configuration module header file.

    @file       pll.h
    @ingroup    mIDrvSys_CG
    @note       Nothing

    Copyright   Novatek Microelectronics Corp. 2012.  All rights reserved.
*/

#ifndef _PLL_H
#define _PLL_H

#include <nvt_type.h>
/**
    @addtogroup mIHALSysCG
*/
//@{

/*
    System Reset ID

    This is for pll_enableSystemReset() and pll_disableSystemReset().
*/
typedef enum
{
    PLL_RSTN_DRAMC = 1, //< Reset SDRAM controller
    DRAMC2_RSTN = 2,    //< Reset SDRAM2 controller
    SIE_RSTN = 4,       //< Reset SIE controller
    SIE2_RSTN,          //< Reset SIE2 controller
    //5

    TGE_RSTN = 6,       //< Reset PRE controller
    IPE_RSTN,           //< Reset IPE controller
    DIS_RSTN,           //< Reset DIS controller
    IME_RSTN,           //< Reset IME controller
    FDE_RSTN,           //< Reset FDE controller
    //10

    ISE_RSTN,           //< Reset ISE controller
    IFE_RSTN = 13,      //< Reset IFE controller
    DCE_RSTN,           //< Reset DCE controller
    IFE2_RSTN,          //< Reset IFE2 controller
    //15

    IDE_RSTN = 16,      //< Reset IDE controller
    IDE2_RSTN,          //< Reset IDE2 controller
    TV_RSTN,            //< Reset TV controller
    MI_RSTN = 20,       //< Reset MI controller
    //20

    CEC_RSTN = 21,      //< Reset CEC controller
    HDMI_RSTN,          //< Reset HDMI controller
    CRYPTO_RSTN,        //< Reset CRYPTO controller
    H26X_RSTN,          //< Reset H264 controller
    AFFINE_RSTN,        //< Reset AFFINE controller
    //25


    JPG_RSTN = 26,      //< Reset JPG controller
    GRAPH_RSTN,         //< Reset Graphic controller
    GRAPH2_RSTN,        //< Reset Graphic2 controller
    DAI_RSTN,           //< Reset DAI controller
    EAC_RSTN,           //< Reset EAC controller
    //30

    NAND_RSTN = 32,     //< Reset xD/NAND controller
    SDIO_RSTN = 34,     //< Reset SDIO controller
    SDIO2_RSTN,         //< Reset SDIO2 controller
    //35

    I2C_RSTN = 36,      //< Reset I2C controller
    I2C2_RSTN,          //< Reset I2C2 controller
    SPI_RSTN,           //< Reset SPI controller
    SPI2_RSTN,          //< Reset SPI2 controller
    SPI3_RSTN,          //< Reset SPI3 controller
    //40

    SIF_RSTN = 41,      //< Reset SIF controller
    UART_RSTN,          //< Reset UART controller
    UART2_RSTN,         //< Reset UART2 controller
    REMOTE_RSTN,        //< Reset Remote controller
    ADC_RSTN,           //< Reset ADC controller
    //45

    SDIO3_RSTN,         //< Reset SDIO3 controller
    SPI4_RSTN,          //< Reset SPI4 controller

    RTC_RSTN = 48,      //< Reset RTC controller
    WDT_RSTN,           //< Reset WDT controller
    TMR_RSTN,           //< Reset Timer controller
    //50

    USB_RSTN = 51,      //< Reset USB controller

    UART3_RSTN = 54,    //< Reset UART3 controller
    UART4_RSTN,         //< Reset UART4 controller
    //55

    INTC_RSTN = 56,     //< Reset INTC controller
    GPIO_RSTN,          //< Reset GPIO controller
    PAD_RSTN,           //< Reset PAD controller
    TOP_RSTN,           //< Reset TOP controller
    EFUSE_RSTN,         //< Reset EFUSE controller
    ETH_RSTN,           //< Reset ETH controller
    //61

    I2C3_RSTN = 63,     //< Reset I2C3 controller

    MIPI_RSTN = 65,     //< Backward compatible
    MIPI_LVDS_RSTN = 65,//< Reset MIPI_LVDS controller
    MIPI2_RSTN = 66,    //< Backward compatible
    MIPI_LVDS2_RSTN =66,//< Reset MIPI_LVDS2 controller
    MIPI_DSI_RSTN,      //< Reset MIPI DSI controller
    RDE_RSTN = 70,      //< Reset RDE controller


    PWM_RSTN = 72,      //< Reset PWM controller

    DRE_RSTN = 76,      //< Reset DRE controller
    TSMUX_RSTN,         //< Reset TSMUX controller
    Vx1_RSTN,           //< Reset Vx1 controller
    SIE3_RSTN,          //< Reset SIE3 controller
    SIE4_RSTN,          //< Reset SIE4 controller
    USB2_RSTN,          //< Reset USB2 controller
    CC_RSTN,            //< Reset CC controller
    RHE_RSTN,           //< Reset RHE controller
    SL_RSTN,            //< Reset SubLink controller
    SL_REG_RSTN,        //< Reset SubLink controller register
    //85

    SL_2_RSTN,          //< Reset SubLink2 controller
    SL_2_REG_RSTN,      //< Reset SubLink2 controller register
    Vx12_RSTN,          //< Reset Vx12 controller
    CANBUS_RSTN,        //< Reset CANBUS controller
    I2C4_RSTN,          //< Reset I2C4 controller
    I2C5_RSTN,          //< Reset I2C5 controller
    I2C6_RSTN,          //< Reset I2C6 controller
    I2C7_RSTN,          //< Reset I2C7 controller
    HWCPY_RSTN,         //< Reset HWCPY controller
    ROTATE_RSTN,        //< Reset ROTATE controller


    MIPI_LVDS3_RSTN = 116, //< Reset MIPI_LVDS3 controller
    MIPI_LVDS4_RSTN,       //< Reset MIPI_LVDS4 controller
    MIPI_LVDS5_RSTN,       //< Reset MIPI_LVDS5 controller
    MIPI_LVDS6_RSTN,       //< Reset MIPI_LVDS6 controller
    MIPI_LVDS7_RSTN,       //< Reset MIPI_LVDS7 controller
    MIPI_LVDS8_RSTN,       //< Reset MIPI_LVDS8 controller
    SIE5_RSTN,             //< Reset SIE5 controller
    SIE6_RSTN,             //< Reset SIE6 controller
    SIE7_RSTN,             //< Reset SIE7 controller
    SIE8_RSTN,             //< Reset SIE8 controller
    IVE_RSTN,              //< Reset IVE controller
    SDE_RSTN,              //< Reset SDE controller
    //127

    SVM_RSTN = 128,        //< Reset SVM controller
    CNN_RSTN,              //< Reset CNN controller
    ISE2_RSTN = 130,       //< Reset ISE2 controller
    DDRPHY_RSTN = 134,     //< Reset DDRPHY
    DDRPHY2_RSTN,          //< Reset DDRPHY2
    SLVSEC_RSTN = 138,     //< Reset SLVSEC
    DSP_RSTN,              //< Reset DSP
    DSP2_RSTN,             //< Reset DSP2
    //140

    TSDEMUX_RSTN = 148,    //< Reset TSDEMUX
    ETH_REVMII_RSTN = 149, //< Reset ETH_REVMII
    DRTC_RSTN = 150,       //< Reset DRTC

    DSP_NIC_RSTN = 160,    //< Reset DSP NIC
    DSP_APBTX_RSTN,        //< Reset DSP APBTX
    DSP_AXIB_EPP_RSTN,     //< Reset DSP AXIB EPP
    DSP_AXIB_EDP_RSTN,     //< Reset DSP AXIB EDP
    DSP2_NIC_RSTN = 168,   //< Reset DSP2 NIC
    DSP2_APBTX_RSTN,       //< Reset DSP2 APBTX
    DSP2_AXIB_EPP_RSTN,    //< Reset DSP2 AXIB EPP
    DSP2_AXIB_EDP_RSTN,    //< Reset DSP2 AXIB EDP
    DAP_AXIB_RSTN = 176,   //< Reset DAP AXIB
    USB_AXIB_RSTN = 180,   //< Reset USB AXIB
    ETH_AXIB_RSTN = 184,   //< Reset ETH AXIB

    CHIPPAR2_RSTN = 192,   //< Reset CHIP partition2
    CHIPPAR8_RSTN = 193,   //< Reset CHIP partition3

    ENUM_DUMMY4WORD(CG_RSTN)
} CG_RSTN;


/**
    Clock Enable ID

    This is for pll_enableClock() and pll_disableClock().
*/
typedef enum
{
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
    Vx1_SL2CLK=86,      ///< Vx1 sub-link2 clock
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
    SIE5_CLK,           ///< SIE5
    SIE6_CLK,           ///< SIE6
    SIE7_CLK,           ///< SIE7
    SIE8_CLK,           ///< SIE8
    IVE_CLK,            ///< IVE
    SDE_CLK = 127,      ///< SDE/CNN
    SDE_CNN_CLK = 127,  ///  SDE/CNN
    CNN_CLK = 127,      ///  SDE/CNN

    SVM_CLK = 128,      ///< SVM
    ISE2_CLK = 130,     ///< ISE2
    USB3_USP_CLK = 136, ///< USB3 suspend clock (12MHz)
    USB3_REF_ITP_CLK,   ///< USB3 ref ipt clock (24MHz)
    SLVSEC_CLK,
    DSP_CLK = 139,      ///< DSP clock
    DSP2_CLK,           ///< DSP2 clock
    CPU_CORETIMER_CLK,  ///< CPU core timer clock
    SIE3_PXCLK,         ///< SIE3 PX clock
    SIE4_PXCLK,         ///< SIE3 PX clock
    SIE5_PXCLK,         ///< SIE3 PX clock
    SIE6_PXCLK,         ///< SIE3 PX clock
    SIE7_PXCLK,         ///< SIE3 PX clock
    SIE8_PXCLK,         ///< SIE3 PX clock
    TSDEMUX_CLK,        ///< TSDEMUX clock
    DRTC_CLK = 150,     ///< DRTC

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
typedef enum
{
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
}GATECLK;

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
typedef enum
{
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
typedef enum
{
    PLL_CLK_SIEMCLK,        ///< SIE MCLK
    PLL_CLK_SIEMCLK2,       ///< SIE MCLK2
    PLL_CLK_SPCLK,          ///< SP CLK
    PLL_CLK_SPCLK2,         ///< SP2 CLK

    ENUM_DUMMY4WORD(PLL_CLK)
} PLL_CLK;

/**
    PLL Clock Source

    @note This is for pll_selectClkSrc(), pll_getClkSrc()
*/
typedef enum
{
    PLL_CLKSRC_240,         ///< CLK source is 240MHz
    PLL_CLKSRC_480,         ///< CLK source is 480MHz
    PLL_CLKSRC_PLL2,        ///< CLK source is PLL2
    PLL_CLKSRC_PLL5,        ///< CLK source is PLL5


    PLL_CLKSRC_UNKNOWN,     ///< unkown clock source

    ENUM_DUMMY4WORD(PLL_CLKSRC)
} PLL_CLKSRC;

extern void pll_disableSystemReset(CG_RSTN Num);
extern void pll_enableClock(CG_EN Num);

extern ER       pll_setPLL(PLL_ID id, UINT32 uiSetting);
extern BOOL     pll_getPLLEn(PLL_ID id);
extern ER       pll_setPLLEn(PLL_ID id, BOOL bEnable);
extern UINT32   pll_getPLLFreq(PLL_ID id);
extern ER       pll_setPLLSpreadSpectrum(PLL_ID id, UINT32 uiLowerFreq, UINT32 uiUpperFreq);
extern ER       pll_getPLLSpreadSpectrum(PLL_ID id, UINT32* puiLowerFreq, UINT32* puiUpperFreq);

extern void     pll_setPWMClockRate(UINT32 uiPWMNumber, UINT32 uiDiv);
extern BOOL     pll_isClockEnabled(CG_EN Num);

extern void     pll_setClkAutoGating(GATECLK ClkSel);
extern void     pll_clearClkAutoGating(GATECLK ClkSel);
extern BOOL     pll_getClkAutoGating(GATECLK ClkSel);

extern void     pll_setPclkAutoGating(GATECLK ClkSel);
extern void     pll_clearPclkAutoGating(GATECLK ClkSel);
extern BOOL     pll_getPclkAutoGating(GATECLK ClkSel);

extern ER       pll_setClkFreq(PLL_CLK clkID, UINT32 uiFreq);
extern ER       pll_getClkFreq(PLL_CLK clkID, UINT32* puiFreq);

extern ER       pll_selectClkSrc(PLL_CLK clkID, PLL_CLKSRC clkSrc);
extern ER       pll_getClkSrc(PLL_CLK clkID, PLL_CLKSRC* pClkSrc);

extern ER       pll_setClkEn(PLL_CLK clkID, BOOL bEnable);
extern ER       pll_getClkEn(PLL_CLK clkID, BOOL* pEnable);
//@}

#endif



