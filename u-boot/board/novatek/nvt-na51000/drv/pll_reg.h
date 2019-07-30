/**
    PLL register header

    PLL register header file

    @file       pll_reg.h
    @ingroup    mIDrvSys_CG
    @note       Nothing

    Copyright   Novatek Microelectronics Corp. 2011.  All rights reserved.
*/

#ifndef _PLL_REG_H
#define _PLL_REG_H

//#include "DrvCommon.h"


/*
    CPU_CLK_ENUM
*/
typedef enum
{
    CPU_CLK_ENUM_80MHZ,             //< 80 MHz
    //CPU_CLK_ENUM_240MHZ,            //< 240 MHz
    //CPU_CLK_ENUM_480MHZ,            //< 480 MHz
    CPU_CLK_ENUM_PLL8,              //< PLL8

    ENUM_DUMMY4WORD(CPU_CLK_ENUM)
} CPU_CLK_ENUM;

/*
    OCP_CLK_ENUM
*/
typedef enum
{
    OCP_CLK_ENUM_CPU,               //< OCP clock = CPU clock
    //OCP_CLK_ENUM_CPUD2,             //< OCP clock = CPU clock / 2

    ENUM_DUMMY4WORD(OCP_CLK_ENUM)
} OCP_CLK_ENUM;

/*
    APB_CLK_ENUM
*/
typedef enum
{
    APB_CLK_ENUM_48,                //< 48 MHz
    APB_CLK_ENUM_60,                //< 60 MHz
    APB_CLK_ENUM_80,                //< 80 MHz
    APB_CLK_ENUM_120,               //< 120 MHz

    ENUM_DUMMY4WORD(APB_CLK_ENUM)
} APB_CLK_ENUM;

/*
    CPU2_CLK_ENUM
*/
typedef enum
{
    CPU2_CLK_ENUM_80MHZ,            //< 80 MHz
    CPU2_CLK_ENUM_PLL9,             //< PLL9

    ENUM_DUMMY4WORD(CPU2_CLK_ENUM)
} CPU2_CLK_ENUM;

/*
    DSP_CLK_ENUM
*/
typedef enum
{
    DSP_CLK_ENUM_80MHZ,             //< 80 MHz
    DSP_CLK_ENUM_PLL10,             //< PLL10

    ENUM_DUMMY4WORD(DSP_CLK_ENUM)
} DSP_CLK_ENUM;

/*
    IPP_CLK_ENUM
*/
typedef enum
{
    //IPP_CLK_ENUM_48,                //< 48 MHz
    //IPP_CLK_ENUM_60,                //< 60 MHz
    //IPP_CLK_ENUM_80,                //< 80 MHz
    //IPP_CLK_ENUM_120,               //< 120 MHz
    //IPP_CLK_ENUM_160,               //< 160 MHz
    IPP_CLK_ENUM_192,               //< 192 MHz
    IPP_CLK_ENUM_240,               //< 240 MHz
    IPP_CLK_ENUM_PLL6,              //< PLL6
    IPP_CLK_ENUM_PLL13,             //< PLL13

    ENUM_DUMMY4WORD(IPP_CLK_ENUM)
} IPP_CLK_ENUM;

/*
    SIE_MCLK_SRC_ENUM
*/
typedef enum
{
    SIE_MCLK_SRC_ENUM_480,          //< 480 MHz
    SIE_MCLK_SRC_ENUM_PLL5,         //< PLL5

    ENUM_DUMMY4WORD(SIE_MCLK_SRC_ENUM)
} SIE_MCLK_SRC_ENUM;

/*
    SIE_PLL_SRC_ENUM
*/
typedef enum
{
    SIE_PLL_SRC_ENUM_480,           //< 480 MHz
    SIE_PLL_SRC_ENUM_PLL6,          //< PLL6
    SIE_PLL_SRC_ENUM_PLL5,          //< PLL5
    SIE_PLL_SRC_ENUM_PLL13,         //< PLL13

    ENUM_DUMMY4WORD(SIE_PLL_SRC_ENUM)
} SIE_PLL_SRC_ENUM;

/*
    SIE_SRC_ENUM
*/
typedef enum
{
    SIE_SRC_ENUM_PAD,               //< PXCLK PAD
    SIE_SRC_ENUM_PLL,               //< SIEx_PLL_CLK

    ENUM_DUMMY4WORD(SIE_SRC_ENUM)
} SIE_SRC_ENUM;

/*
    SIE_MCLK_PHASE_ENUM
*/
typedef enum
{
    SIE_MCLK_PHASE_ENUM_NORMAL,     //< Normal
    SIE_MCLK_PHASE_ENUM_INVERT,     //< Invert

    ENUM_DUMMY4WORD(SIE_MCLK_PHASE_ENUM)
} SIE_MCLK_PHASE_ENUM;

/*
    JPG_CLK_ENUM
*/
typedef enum
{
    //JPG_CLK_ENUM_80,                //< 80 MHz
    //JPG_CLK_ENUM_120,               //< 120 MHz
    //JPG_CLK_ENUM_160,               //< 160 MHz
    JPG_CLK_ENUM_192,               //< 192 MHz
    JPG_CLK_ENUM_240,               //< 240 MHz
    JPG_CLK_ENUM_PLL6,              //< PLL6
    JPG_CLK_ENUM_PLL13,             //< PLL13

    ENUM_DUMMY4WORD(JPG_CLK_ENUM)
} JPG_CLK_ENUM;

/*
    H264_CLK_ENUM
*/
typedef enum
{
    //H264_CLK_ENUM_120,              //< 120 MHz
    //H264_CLK_ENUM_160,              //< 160 MHz
    H264_CLK_ENUM_192,              //< 192 MHz
    H264_CLK_ENUM_240,              //< 240 MHz
    H264_CLK_ENUM_PLL6,             //< PLL6
    H264_CLK_ENUM_PLL13,            //< PLL13 MHz

    ENUM_DUMMY4WORD(H264_CLK_ENUM)
} H264_CLK_ENUM;

/*
    GRPH_CLK_ENUM

    For graphic, graphic2, affine
*/
typedef enum
{
    //GRPH_CLK_ENUM_80,               //< 80 MHz
    GRPH_CLK_ENUM_120,              //< 120 MHz
    GRPH_CLK_ENUM_160,              //< 160 MHz
    GRPH_CLK_ENUM_192,              //< 192 MHz
    GRPH_CLK_ENUM_240,              //< 240 MHz

    ENUM_DUMMY4WORD(GRPH_CLK_ENUM)
} GRPH_CLK_ENUM;

/*
    CRYPTO_CLK_ENUM
*/
typedef enum
{
    CRYPTO_CLK_ENUM_80,             //< 80 MHz
    CRYPTO_CLK_ENUM_160,            //< 160 MHz

    ENUM_DUMMY4WORD(CRYPTO_CLK_ENUM)
} CRYPTO_CLK_ENUM;

/*
    NAND_CLK_ENUM
*/
typedef enum
{
    NAND_CLK_ENUM_48,               //< 48 MHz
    NAND_CLK_ENUM_60,               //< 60 MHz
    NAND_CLK_ENUM_96,               //< 96 MHz
    //NAND_CLK_ENUM_PLL4,             //< PLL4
    //NAND_CLK_ENUM_PLL4D2,           //< PLL4 / 2

    ENUM_DUMMY4WORD(NAND_CLK_ENUM)
} NAND_CLK_ENUM;

/*
    SDIO_CLK_ENUM

    For SDIO, SDIO2, SDIO3
*/
typedef enum
{
    SDIO_CLK_ENUM_60,               //< 60 MHz
    SDIO_CLK_ENUM_80,               //< 80 MHz
    SDIO_CLK_ENUM_96,               //< 96 MHz
    SDIO_CLK_ENUM_PLL4,             //< PLL4

    ENUM_DUMMY4WORD(SDIO_CLK_ENUM)
} SDIO_CLK_ENUM;

/*
    SDIO2_CLK_ENUM
*/
/*typedef enum
{
    SDIO2_CLK_ENUM_40,              //< 40 MHz
    SDIO2_CLK_ENUM_48,              //< 48 MHz
    SDIO2_CLK_ENUM_PLL4,            //< PLL4

    ENUM_DUMMY4WORD(SDIO2_CLK_ENUM)
} SDIO2_CLK_ENUM;*/

/*
    MIPI_CLK_ENUM

    For MIPI, MIPI2
*/
typedef enum
{
    MIPI_CLK_ENUM_60,               //< 60 MHz
    MIPI_CLK_ENUM_120,              //< 120 MHz

    ENUM_DUMMY4WORD(MIPI_CLK_ENUM)
} MIPI_CLK_ENUM;

/*
    LVDS_PHASE_ENUM

    For LVDS/MIPI, LVDS2/MIPI2
*/
typedef enum
{
    LVDS_PHASE_ENUM_NORMAL,         //< Normal
    LVDS_PHASE_ENUM_INVERT,         //< Invert

    ENUM_DUMMY4WORD(LVDS_PHASE_ENUM)
} LVDS_PHASE_ENUM;

/*
    IDE_SRC_ENUM

    For DSI/IDE/IDE2
*/
typedef enum
{
    IDE_SRC_ENUM_480,               //< 480 MHz
    IDE_SRC_ENUM_PLL2,              //< PLL2
    IDE_SRC_ENUM_PLL4,              //< PLL4
    IDE_SRC_ENUM_PLL12,             //< PLL12

    ENUM_DUMMY4WORD(IDE_SRC_ENUM)
} IDE_SRC_ENUM;

/*
    ADO_CLKMUX_ENUM

    For ADO, HDMI_ADO
*/
typedef enum
{
    /*ADO_CLKMUX_ENUM_24P576,         //< 24.576 MHz
    ADO_CLKMUX_ENUM_12P288,         //< 12.288 MHz
    ADO_CLKMUX_ENUM_8P192,          //< 8.192 MHz
    ADO_CLKMUX_ENUM_6P144,          //< 6.144 MHz
    ADO_CLKMUX_ENUM_4P096,          //< 4.096 MHz
    ADO_CLKMUX_ENUM_3P072,          //< 3.072 MHz
    ADO_CLKMUX_ENUM_2P048,          //< 2.048 MHz
    ADO_CLKMUX_ENUM_11P2896,        //< 11.2896 MHz
    ADO_CLKMUX_ENUM_5P6448,         //< 5.6448 MHz
    ADO_CLKMUX_ENUM_2P8224,         //< 2.8224 MHz*/

    ADO_CLKMUX_ENUM_8P192,          //< 8.192 MHz
    ADO_CLKMUX_ENUM_11P2896,        //< 11.2896 MHz
    ADO_CLKMUX_ENUM_12P288,         //< 12.288 MHz

    ENUM_DUMMY4WORD(ADO_CLKMUX_ENUM)
} ADO_CLKMUX_ENUM;

/*
    ADO_CLKSRC_ENUM

    For ADO, HDMI_ADO
*/
typedef enum
{
    ADO_CLKSRC_ENUM_CLKMUX,         //< From CLK MUX
    ADO_CLKSRC_ENUM_PLLDIV,         //< From divided PLL

    ENUM_DUMMY4WORD(ADO_CLKSRC_ENUM)
} ADO_CLKSRC_ENUM;

/*
    TSMUX_CLK_ENUM
*/
typedef enum
{
    TSMUX_CLK_ENUM_80,              //< 80 MHz
    TSMUX_CLK_ENUM_160,             //< 160 MHz
    TSMUX_CLK_ENUM_240,             //< 240 MHz
    TSMUX_CLK_ENUM_PLL12,           //< PLL12

    ENUM_DUMMY4WORD(TSMUX_CLK_ENUM)
} TSMUX_CLK_ENUM;

/*
    SP_CLK_ENUM
*/
typedef enum
{
    SP_CLK_ENUM_240,                //< 240 MHz
    SP_CLK_ENUM_PLL2,               //< PLL2
    SP_CLK_ENUM_PLL5,               //< PLL5

    ENUM_DUMMY4WORD(SP_CLK_ENUM)
} SP_CLK_ENUM;

//
//0x00 PLL power enable Register
//
#define PLL_PLL_PWREN_REG_OFS           0x00
REGDEF_BEGIN(PLL_PLL_PWREN_REG)
    REGDEF_BIT(reserved1        ,2)
    REGDEF_BIT(PLL2_EN          ,1)
    REGDEF_BIT(PLL3_EN          ,1)
    REGDEF_BIT(PLL4_EN          ,1)
    REGDEF_BIT(PLL5_EN          ,1)
    REGDEF_BIT(PLL6_EN          ,1)
    REGDEF_BIT(PLL7_EN          ,1)
    REGDEF_BIT(PLL8_EN          ,1)
    REGDEF_BIT(PLL9_EN          ,1)
    REGDEF_BIT(PLL10_EN         ,1)
    REGDEF_BIT(PLL11_EN         ,1)
    REGDEF_BIT(PLL12_EN         ,1)
    REGDEF_BIT(PLL13_EN         ,1)
    REGDEF_BIT(PLL14_EN         ,1)
    REGDEF_BIT(reserved2        ,17)
REGDEF_END(PLL_PLL_PWREN_REG)

//
//0x04 PLL Status Register
//
#define PLL_PLL_STATUS_REG_OFS          0x04
REGDEF_BEGIN(PLL_PLL_STATUS_REG)
    REGDEF_BIT(reserved1        ,2)
    REGDEF_BIT(PLL2_READY       ,1)
    REGDEF_BIT(PLL3_READY       ,1)
    REGDEF_BIT(PLL4_READY       ,1)
    REGDEF_BIT(PLL5_READY       ,1)
    REGDEF_BIT(PLL6_READY       ,1)
    REGDEF_BIT(PLL7_READY       ,1)
    REGDEF_BIT(PLL8_READY       ,1)
    REGDEF_BIT(PLL9_READY       ,1)
    REGDEF_BIT(PLL10_READY      ,1)
    REGDEF_BIT(PLL11_READY      ,1)
    REGDEF_BIT(PLL12_READY      ,1)
    REGDEF_BIT(PLL13_READY      ,1)
    REGDEF_BIT(PLL14_READY      ,1)
    REGDEF_BIT(reserved2        ,17)
REGDEF_END(PLL_PLL_STATUS_REG)

//
//0x08~0x0C Reserved Register
//

//
//0x10 System Clock Rate Register
//
#define PLL_SYS_CR_REG_OFS              0x10
REGDEF_BEGIN(PLL_SYS_CR_REG)
    REGDEF_BIT(CPU_CLKSEL       ,2)  // CPU clock select
    REGDEF_BIT(                 ,2)
    REGDEF_BIT(APB_CLKSEL       ,2)  // APB clock select
    REGDEF_BIT(DMA2_SRC_CLKSEL  ,2)  // DMA2 clock select
    REGDEF_BIT(DSP2_CLKSEL      ,2)  // DSP2 clock select
    REGDEF_BIT(DMA_ARBT_CLKSEL  ,2)  // DMA arbiter clock select
    REGDEF_BIT(DSP_IOP_CLKSEL   ,2)  // DSP IOP clock select
    REGDEF_BIT(DSP_EDP_CLKSEL   ,2)  // DSP EDP clock select
    REGDEF_BIT(DSP_EPP_CLKSEL   ,2)  // DSP EPP clock select
    REGDEF_BIT(                 ,4)
    REGDEF_BIT(DSP2_IOP_CLKSEL  ,2)  // DSP2 IOP clock select
    REGDEF_BIT(DSP2_EDP_CLKSEL  ,2)  // DSP2 EDP clock select
    REGDEF_BIT(DSP2_EPP_CLKSEL  ,2)  // DSP2 EPP clock select
    REGDEF_BIT(                 ,4)
REGDEF_END(PLL_SYS_CR_REG)

//
//0x14 IPP Clock Rate Register 0
//
#define PLL_IPP_CR0_REG_OFS             0x14
REGDEF_BEGIN(PLL_IPP_CR0_REG)
    REGDEF_BIT(SDE_CLKSEL       ,2)
    REGDEF_BIT(                 ,2)
    REGDEF_BIT(IPE_CLKSEL       ,2)
    REGDEF_BIT(                 ,2)
    REGDEF_BIT(DIS_CLKSEL       ,2)
    REGDEF_BIT(                 ,2)
    REGDEF_BIT(IME_CLKSEL       ,2)
    REGDEF_BIT(                 ,2)
    REGDEF_BIT(FDE_CLKSEL       ,2)
    REGDEF_BIT(                 ,2)
    REGDEF_BIT(IVE_CLKSEL       ,2)
    REGDEF_BIT(                 ,2)
    REGDEF_BIT(ISE_CLKSEL       ,2)
    REGDEF_BIT(                 ,2)
    REGDEF_BIT(DCE_CLKSEL       ,2)
    REGDEF_BIT(                 ,2)
REGDEF_END(PLL_IPP_CR0_REG)

//
//0x18 IPP Clock Rate Register 1
//
#define PLL_IPP_CR1_REG_OFS             0x18
REGDEF_BEGIN(PLL_IPP_CR1_REG)
    REGDEF_BIT(IFE_CLKSEL       ,2)
    REGDEF_BIT(                 ,2)
    REGDEF_BIT(IFE2_CLKSEL      ,2)
    REGDEF_BIT(                 ,2)
    REGDEF_BIT(SVN_CLKSEL       ,2)
    REGDEF_BIT(                 ,2)
    REGDEF_BIT(RHE_CLKSEL       ,2)
    REGDEF_BIT(                 ,2)
    REGDEF_BIT(CNN_CLKSEL       ,2)
    REGDEF_BIT(                 ,1)
    REGDEF_BIT(SIE_MCLK_INV     ,1)
    REGDEF_BIT(                 ,2)
    REGDEF_BIT(TGE_CLKSRC       ,2)
    REGDEF_BIT(SIE_MCLK2_INV    ,1)
    REGDEF_BIT(                 ,3)
    REGDEF_BIT(DRE_CLKSEL       ,2)
    REGDEF_BIT(TGE2_CLKSRC      ,2)
REGDEF_END(PLL_IPP_CR1_REG)

//
//0x1C CODEC Clock Rate Register
//
#define PLL_CODEC_CR_REG_OFS            0x1C
REGDEF_BEGIN(PLL_CODEC_CR_REG)
    REGDEF_BIT(JPEG_CLKSEL      ,2)
    REGDEF_BIT(                 ,2)
    REGDEF_BIT(H264_CLKSEL      ,2)
    REGDEF_BIT(                 ,2)
    REGDEF_BIT(GRPH_CLKSEL      ,2)
    REGDEF_BIT(                 ,2)
    REGDEF_BIT(GRPH2_CLKSEL     ,2)
    REGDEF_BIT(                 ,2)
    REGDEF_BIT(AFFINE_CLKSEL    ,2)
    REGDEF_BIT(                 ,2)
    REGDEF_BIT(HWCPY_CLKSEL     ,2)
    REGDEF_BIT(                 ,2)
    REGDEF_BIT(ROTATE_CLKSEL    ,2)
    REGDEF_BIT(                 ,2)
    REGDEF_BIT(                 ,2)
    REGDEF_BIT(                 ,1)
    REGDEF_BIT(CRYPTO_CLKSEL    ,1)
REGDEF_END(PLL_CODEC_CR_REG)

//
//0x20 Peripheral Clock Rate Register
//
#define PLL_PERI_CR_REG_OFS             0x20
REGDEF_BEGIN(PLL_PERI_CR_REG)
    REGDEF_BIT(                 ,2)
    REGDEF_BIT(                 ,2)
    REGDEF_BIT(SDIO_CLKEL       ,2)
    REGDEF_BIT(                 ,2)
    REGDEF_BIT(SDIO2_CLKEL      ,2)
    REGDEF_BIT(                 ,6)
    REGDEF_BIT(IDE_CLKSRC       ,2)
    REGDEF_BIT(IDE2_CLKSRC      ,2)
    REGDEF_BIT(DSI_LPSRC        ,2)
    REGDEF_BIT(MI_CLKSRC        ,2)
    REGDEF_BIT(HDMI_ADO_CLKMUX  ,2)
    REGDEF_BIT(                 ,2)
    REGDEF_BIT(ADO_CLKSEL       ,2)
    REGDEF_BIT(HDMI_ADO_CLKSEL  ,2)
REGDEF_END(PLL_PERI_CR_REG)

//
//0x24 Peripheral Clock Rate Register 1
//
#define PLL_PERI_CR1_REG_OFS             0x24
REGDEF_BEGIN(PLL_PERI_CR1_REG)
    REGDEF_BIT(SDIO3_CLKEL      ,2)
    REGDEF_BIT(                 ,2)
    REGDEF_BIT(TSMUX_CLKSEL     ,2)
    REGDEF_BIT(                 ,2)
    REGDEF_BIT(SP_CLKSEL        ,2)
    REGDEF_BIT(SP2_CLKSEL       ,2)
    REGDEF_BIT(CANBUS_CLKSEL    ,2)
    REGDEF_BIT(ETH_REFCLK_INV   ,1)
    REGDEF_BIT(VX1_PIXCLK_INV   ,1)
    REGDEF_BIT(VX1_PIXCLK2_INV  ,1)
    REGDEF_BIT(VX12_PIXCLK_INV  ,1)
    REGDEF_BIT(VX12_PIXCLK2_INV ,1)
    REGDEF_BIT(ETH_REVMII_OPT_INV,  1)
    REGDEF_BIT(ETH_REVMII_OPT2_INV, 1)
    REGDEF_BIT(                 ,11)
REGDEF_END(PLL_PERI_CR1_REG)

//
//0x28 Peripheral Clock Rate Register 2
//
#define PLL_PERI_CR2_REG_OFS             0x28
REGDEF_BEGIN(PLL_PERI_CR2_REG)
    REGDEF_BIT(CSI_CLKSEL      ,1)
    REGDEF_BIT(CSI2_CLKSEL     ,1)
    REGDEF_BIT(CSI3_CLKSEL     ,1)
    REGDEF_BIT(CSI4_CLKSEL     ,1)
    REGDEF_BIT(CSI5_CLKSEL     ,1)
    REGDEF_BIT(CSI6_CLKSEL     ,1)
    REGDEF_BIT(CSI7_CLKSEL     ,1)
    REGDEF_BIT(CSI8_CLKSEL     ,1)
    REGDEF_BIT(CSI_CLK_INV     ,1)
    REGDEF_BIT(CSI2_CLK_INV    ,1)
    REGDEF_BIT(CSI3_CLK_INV    ,1)
    REGDEF_BIT(CSI4_CLK_INV    ,1)
    REGDEF_BIT(CSI5_CLK_INV    ,1)
    REGDEF_BIT(CSI6_CLK_INV    ,1)
    REGDEF_BIT(CSI7_CLK_INV    ,1)
    REGDEF_BIT(CSI8_CLK_INV    ,1)
    REGDEF_BIT(SLVSEC_CLKSEL   ,3)
    REGDEF_BIT(                ,13)
REGDEF_END(PLL_PERI_CR2_REG)

//
//0x2C Peripheral Clock Rate Register 3
//
#define PLL_PERI_CR3_REG_OFS             0x2C
REGDEF_BEGIN(PLL_PERI_CR3_REG)
    REGDEF_BIT(SIE_CLKSRC      ,3)
    REGDEF_BIT(SIE2_CLKSRC     ,3)
    REGDEF_BIT(SIE3_CLKSRC     ,3)
    REGDEF_BIT(SIE4_CLKSRC     ,3)
    REGDEF_BIT(SIE_MCLKSRC     ,2)
    REGDEF_BIT(SIE_MCLK2SRC    ,2)
    REGDEF_BIT(SIE_PXCLKSRC    ,2)
    REGDEF_BIT(SIE2_PXCLKSRC   ,2)
    REGDEF_BIT(SIE3_PXCLKSRC   ,2)
    REGDEF_BIT(SIE4_PXCLKSRC   ,2)
    REGDEF_BIT(SIE5_PXCLKSRC   ,2)
    REGDEF_BIT(SIE6_PXCLKSRC   ,2)
    REGDEF_BIT(SIE7_PXCLKSRC   ,2)
    REGDEF_BIT(SIE8_PXCLKSRC   ,2)
REGDEF_END(PLL_PERI_CR3_REG)


//
//0x30 IPP Clock Divider Register
//
#define PLL_IPP_CLKDIV_REG_OFS          0x30
REGDEF_BEGIN(PLL_IPP_CLKDIV_REG)
    REGDEF_BIT(SIE_MCLKDIV      ,8)
    REGDEF_BIT(SIE_MCLK2DIV     ,8)
    REGDEF_BIT(SIE_CLKDIV       ,8)
    REGDEF_BIT(SIE2_CLKDIV      ,8)
REGDEF_END(PLL_IPP_CLKDIV_REG)

//
//0x34 IPP Clock Divider Register 1
//
#define PLL_IPP_CLKDIV2_REG_OFS         0x34
REGDEF_BEGIN(PLL_IPP_CLKDIV2_REG)
    REGDEF_BIT(SIE3_CLKDIV      ,8)
    REGDEF_BIT(SIE4_CLKDIV      ,8)
    REGDEF_BIT(SIE5_CLKDIV      ,4)
    REGDEF_BIT(SIE6_CLKDIV      ,4)
    REGDEF_BIT(SIE7_CLKDIV      ,4)
    REGDEF_BIT(SIE8_CLKDIV      ,4)
REGDEF_END(PLL_IPP_CLKDIV2_REG)


//
//0x38 Video Clock Divider Register
//
#define PLL_VDO_CLKDIV_REG_OFS          0x38
REGDEF_BEGIN(PLL_VDO_CLKDIV_REG)
    REGDEF_BIT(                 ,16)
    REGDEF_BIT(MI_CLKDIV        ,8)
    REGDEF_BIT(                 ,8)
REGDEF_END(PLL_VDO_CLKDIV_REG)

//
//0x3C Peripheral Clock Divider Register
//
#define PLL_PERI_CLKDIV_REG_OFS         0x3C
REGDEF_BEGIN(PLL_PERI_CLKDIV_REG)
    REGDEF_BIT(SP_CLKDIV        ,8)
    REGDEF_BIT(CEC_CLKDIV       ,8)
    REGDEF_BIT(ADO_CLKDIV       ,8)
    REGDEF_BIT(HDMI_ADO_CLKDIV  ,8)
REGDEF_END(PLL_PERI_CLKDIV_REG)

//
//0x40 SDIO Clock Divider Register
//
#define PLL_SDIO_CLKDIV_REG_OFS         0x40
REGDEF_BEGIN(PLL_SDIO_CLKDIV_REG)
    REGDEF_BIT(SDIO_CLKDIV      ,11)
    REGDEF_BIT(                 ,1)
    REGDEF_BIT(SDIO2_CLKDIV     ,11)
    REGDEF_BIT(                 ,1)
    REGDEF_BIT(NAND_CLKDIV      ,6)
    REGDEF_BIT(                 ,2)
REGDEF_END(PLL_SDIO_CLKDIV_REG)


//
//0x44 Peripheral Clock Divider Register 1
//
#define PLL_PERI_CLKDIV1_REG_OFS        0x44
REGDEF_BEGIN(PLL_PERI_CLKDIV1_REG)
    REGDEF_BIT(SDIO3_CLKDIV     ,11)
    REGDEF_BIT(                 ,5)
    REGDEF_BIT(EAC_CLASSD_CLKDIV,8)
    REGDEF_BIT(SP2_CLKDIV       ,8)
REGDEF_END(PLL_PERI_CLKDIV1_REG)

//
//0x48 SPI Clock Divider Register
//
#define PLL_SPI_CLKDIV_REG_OFS          0x48
REGDEF_BEGIN(PLL_SPI_CLKDIV_REG)
    REGDEF_BIT(SPI_CLKDIV       ,11)
    REGDEF_BIT(                 ,5)
    REGDEF_BIT(SPI2_CLKDIV      ,11)
    REGDEF_BIT(reserved2        ,5)
REGDEF_END(PLL_SPI_CLKDIV_REG)

//
//0x4C SPI Clock Divider Register 1
//
#define PLL_SPI_CLKDIV1_REG_OFS         0x4C
REGDEF_BEGIN(PLL_SPI_CLKDIV1_REG)
    REGDEF_BIT(SPI3_CLKDIV      ,11)
    REGDEF_BIT(                 ,5)
    REGDEF_BIT(SPI4_CLKDIV      ,11)
    REGDEF_BIT(                 ,5)
REGDEF_END(PLL_SPI_CLKDIV1_REG)


//
//0x50 PWM Clock Rate Register 0
//
#define PLL_PWMCR0_REG_OFS              0x50
REGDEF_BEGIN(PLL_PWMCR0_REG)
    REGDEF_BIT(PWM0_3_CLKDIV    ,14)
    REGDEF_BIT(                 ,2)
    REGDEF_BIT(PWM4_7_CLKDIV    ,14)
    REGDEF_BIT(                 ,2)
REGDEF_END(PLL_PWMCR0_REG)

//
//0x54 PWM Clock Rate Register 1
//
#define PLL_PWMCR1_REG_OFS              0x54
REGDEF_BEGIN(PLL_PWMCR1_REG)
    REGDEF_BIT(PWM8_11_CLKDIV   ,14)
    REGDEF_BIT(                 ,2)
    REGDEF_BIT(PWM12_CLKDIV     ,14)
    REGDEF_BIT(                 ,2)
REGDEF_END(PLL_PWMCR1_REG)

//
//0x58 PWM Clock Rate Register 2
//
#define PLL_PWMCR2_REG_OFS              0x58
REGDEF_BEGIN(PLL_PWMCR2_REG)
    REGDEF_BIT(PWM13_CLKDIV     ,14)
    REGDEF_BIT(                 ,2)
    REGDEF_BIT(PWM14_CLKDIV     ,14)
    REGDEF_BIT(                 ,2)
REGDEF_END(PLL_PWMCR2_REG)

//
//0x5C PWM Clock Rate Register 3
//
#define PLL_PWMCR3_REG_OFS              0x5C
REGDEF_BEGIN(PLL_PWMCR3_REG)
    REGDEF_BIT(PWM15_CLKDIV     ,14)
    REGDEF_BIT(                 ,2)
    REGDEF_BIT(PWM16_CLKDIV     ,14)
    REGDEF_BIT(                 ,2)
REGDEF_END(PLL_PWMCR3_REG)

//
//0x60 PWM Clock Rate Register 4
//
#define PLL_PWMCR4_REG_OFS              0x60
REGDEF_BEGIN(PLL_PWMCR4_REG)
    REGDEF_BIT(PWM17_CLKDIV     ,14)
    REGDEF_BIT(                 ,2)
    REGDEF_BIT(PWM18_CLKDIV     ,14)
    REGDEF_BIT(                 ,2)
REGDEF_END(PLL_PWMCR4_REG)

//
//0x64 PWM Clock Rate Register 5
//
#define PLL_PWMCR5_REG_OFS              0x64
REGDEF_BEGIN(PLL_PWMCR5_REG)
    REGDEF_BIT(PWM19_CLKDIV     ,14)
    REGDEF_BIT(                 ,18)
REGDEF_END(PLL_PWMCR5_REG)

//
//0x68 Reserved Register
//


//
//0x6C IDE Clock Rate Register
//
#define PLL_IDECR_REG_OFS              0x6C
REGDEF_BEGIN(PLL_IDECR_REG)
    REGDEF_BIT(IDE_CLKDIV       ,8)
    REGDEF_BIT(IDE2_CLKDIV      ,8)
    REGDEF_BIT(IDE_OUTIF_CLKDIV ,8)
    REGDEF_BIT(IDE2_OUTIF_CLKDIV,8)
REGDEF_END(PLL_IDECR_REG)

//
//0x80 Clock Enable Register 0
//
#define PLL_CLKEN0_REG_OFS              0x80
REGDEF_BEGIN(PLL_CLKEN0_REG)
    REGDEF_BIT(SDRAM_CLKEN      ,1)
    REGDEF_BIT(SDRAM2_CLKEN     ,1)
    REGDEF_BIT(SIE_MCLKEN       ,1)
    REGDEF_BIT(SIE_MCLK2EN      ,1)
    REGDEF_BIT(SIE_CLKEN        ,1)
    REGDEF_BIT(SIE2_CLKEN       ,1)
    REGDEF_BIT(TGE_CLKEN        ,1)
    REGDEF_BIT(IPE_CLKEN        ,1)
    REGDEF_BIT(DIS_CLKEN        ,1)     //8
    REGDEF_BIT(IME_CLKEN        ,1)
    REGDEF_BIT(FDE_CLKEN        ,1)
    REGDEF_BIT(ISE_CLKEN        ,1)
    REGDEF_BIT(SP_CLKEN         ,1)
    REGDEF_BIT(IFE_CLKEN        ,1)
    REGDEF_BIT(DCE_CLKEN        ,1)
    REGDEF_BIT(IFE2_CLKEN       ,1)
    REGDEF_BIT(IDE_CLKEN        ,1)     //16
    REGDEF_BIT(IDE2_CLKEN       ,1)
    REGDEF_BIT(TV_CLKEN         ,1)
    REGDEF_BIT(                 ,1)
    REGDEF_BIT(MI_CLKEN         ,1)
    REGDEF_BIT(CEC_CLKEN        ,1)
    REGDEF_BIT(HDMI_CLKEN       ,1)
    REGDEF_BIT(CRYPTO           ,1)
    REGDEF_BIT(H264_CLKEN       ,1)     //24
    REGDEF_BIT(AFFINE_CLKEN     ,1)
    REGDEF_BIT(JPEG_CLKEN       ,1)
    REGDEF_BIT(GRAPHIC_CLKEN    ,1)
    REGDEF_BIT(GRAPHIC2_CLKEN   ,1)
    REGDEF_BIT(DAI_CLKEN        ,1)
    REGDEF_BIT(EAC_A_ADC_CLKEN  ,1)
    REGDEF_BIT(EAC_A_DAC_CLKEN  ,1)
REGDEF_END(PLL_CLKEN0_REG)

//
//0x84 Clock Enable Register 1
//
#define PLL_CLKEN1_REG_OFS              0x84
REGDEF_BEGIN(PLL_CLKEN1_REG)
    REGDEF_BIT(NAND_CLKEN       ,1)
    REGDEF_BIT(                 ,1)
    REGDEF_BIT(SDIO_CLKEN       ,1)
    REGDEF_BIT(SDIO2_CLKEN      ,1)
    REGDEF_BIT(I2C_CLKEN        ,1)
    REGDEF_BIT(I2C2_CLKEN       ,1)
    REGDEF_BIT(SPI_CLKEN        ,1)
    REGDEF_BIT(SPI2_CLKEN       ,1)
    REGDEF_BIT(SPI3_CLKEN       ,1)     //8
    REGDEF_BIT(SIF_CLKEN        ,1)
    REGDEF_BIT(UART_CLKEN       ,1)
    REGDEF_BIT(UART2_CLKEN      ,1)
    REGDEF_BIT(REMOTE_CLKEN     ,1)
    REGDEF_BIT(ADC_CLKEN        ,1)
    REGDEF_BIT(SDIO3_CLKEN      ,1)
    REGDEF_BIT(SPI4_CLKEN       ,1)
    REGDEF_BIT(                 ,1)     //16
    REGDEF_BIT(WDT_CLKEN        ,1)
    REGDEF_BIT(TMR_CLKEN        ,1)
    REGDEF_BIT(USB_CLKEN        ,1)
    REGDEF_BIT(EAC_D_CLKEN      ,1)
    REGDEF_BIT(                 ,1)
    REGDEF_BIT(UARR3_CLKEN      ,1)
    REGDEF_BIT(UART4_CLKEN      ,1)
    REGDEF_BIT(                 ,4)     //24
    REGDEF_BIT(EFUSE_CLKEN      ,1)
    REGDEF_BIT(ETH_CLKEN        ,1)
    REGDEF_BIT(SP2_CLKEN        ,1)
    REGDEF_BIT(I2C3_CLKEN       ,1)
REGDEF_END(PLL_CLKEN1_REG)

//
//0x88 Clock Enable Register 2
//
#define PLL_CLKEN2_REG_OFS              0x88
REGDEF_BEGIN(PLL_CLKEN2_REG)
    REGDEF_BIT(                 ,1)
    REGDEF_BIT(MIPI_LVDS_CLKEN  ,1)
    REGDEF_BIT(MIPI_LVDS2_CLKEN ,1)
    REGDEF_BIT(MIPI_DSI_CLKEN   ,1)
    REGDEF_BIT(SIE_PX_CLKEN     ,1)
    REGDEF_BIT(SIE2_PX_CLKEN    ,1)
    REGDEF_BIT(                 ,1)
    REGDEF_BIT(                 ,1)
    REGDEF_BIT(PWMCCNT0_CLKEN   ,1)     //8
    REGDEF_BIT(PWMCCNT1_CLKEN   ,1)
    REGDEF_BIT(PWMCCNT2_CLKEN   ,1)
    REGDEF_BIT(PWMCCNT3_CLKEN   ,1)
    REGDEF_BIT(DRE_CLKEN        ,1)
    REGDEF_BIT(TSMUX_CLKEN      ,1)
    REGDEF_BIT(Vx1_CLKEN        ,1)
    REGDEF_BIT(SIE3_CLKEN       ,1)
    REGDEF_BIT(SIE4_CLKEN       ,1)     //16
    REGDEF_BIT(USB2_CLKEN       ,1)
    REGDEF_BIT(CC_CLKEN         ,1)
    REGDEF_BIT(RHE_CLKEN        ,1)
    REGDEF_BIT(VX1_CLKEN        ,1)
    REGDEF_BIT(                 ,1)
    REGDEF_BIT(VX1_SL2_CLKEN    ,1)
    REGDEF_BIT(VX1_ML_CLKEN     ,1)
    REGDEF_BIT(VX1_ML2_CLKEN    ,1)     //24
    REGDEF_BIT(CANBUS_CLKEN     ,1)
    REGDEF_BIT(I2C4_CLKEN       ,1)
    REGDEF_BIT(I2C5_CLKEN       ,1)
    REGDEF_BIT(I2C6_CLKEN       ,1)
    REGDEF_BIT(I2C7_CLKEN       ,1)
    REGDEF_BIT(HWCPY_CLKEN      ,1)
    REGDEF_BIT(ROTATE_CLKEN     ,1)
REGDEF_END(PLL_CLKEN2_REG)

//
//0x8C Clock Enable Register 3
//
#define PLL_CLKEN3_REG_OFS              0x8C
REGDEF_BEGIN(PLL_CLKEN3_REG)
    REGDEF_BIT(PWM0_CLKEN       ,1)
    REGDEF_BIT(PWM1_CLKEN       ,1)
    REGDEF_BIT(PWM2_CLKEN       ,1)
    REGDEF_BIT(PWM3_CLKEN       ,1)
    REGDEF_BIT(PWM4_CLKEN       ,1)
    REGDEF_BIT(PWM5_CLKEN       ,1)
    REGDEF_BIT(PWM6_CLKEN       ,1)
    REGDEF_BIT(PWM7_CLKEN       ,1)
    REGDEF_BIT(PWM8_CLKEN       ,1)     //8
    REGDEF_BIT(PWM9_CLKEN       ,1)
    REGDEF_BIT(PWM10_CLKEN      ,1)
    REGDEF_BIT(PWM11_CLKEN      ,1)
    REGDEF_BIT(PWM12_CLKEN      ,1)
    REGDEF_BIT(PWM13_CLKEN      ,1)
    REGDEF_BIT(PWM14_CLKEN      ,1)
    REGDEF_BIT(PWM15_CLKEN      ,1)
    REGDEF_BIT(PWM16_CLKEN      ,1)     //16
    REGDEF_BIT(PWM17_CLKEN      ,1)
    REGDEF_BIT(PWM18_CLKEN      ,1)
    REGDEF_BIT(PWM19_CLKEN      ,1)
    REGDEF_BIT(MIPI_LVDS3_CLKEN ,1)
    REGDEF_BIT(MIPI_LVDS4_CLKEN ,1)
    REGDEF_BIT(MIPI_LVDS5_CLKEN ,1)
    REGDEF_BIT(MIPI_LVDS6_CLKEN ,1)
    REGDEF_BIT(MIPI_LVDS7_CLKEN ,1)
    REGDEF_BIT(MIPI_LVDS8_CLKEN ,1)
    REGDEF_BIT(SIE5_CLKEN       ,1)
    REGDEF_BIT(SIE6_CLKEN       ,1)
    REGDEF_BIT(SIE7_CLKEN       ,1)
    REGDEF_BIT(SIE8_CLKEN       ,1)
    REGDEF_BIT(IVE_CLKEN        ,1)
    REGDEF_BIT(SDE_CNN_CLKEN    ,1)
REGDEF_END(PLL_CLKEN3_REG)

//
//0x90 Clock Enable Register 4
//
#define PLL_CLKEN4_REG_OFS              0x90
REGDEF_BEGIN(PLL_CLKEN4_REG)
    REGDEF_BIT(SVM_CLKEN       ,1)
    REGDEF_BIT(                ,1)
    REGDEF_BIT(                ,3)
    REGDEF_BIT(                ,1)
    REGDEF_BIT(                ,2)
    REGDEF_BIT(USB3_SUSP_CLKEN ,1)
    REGDEF_BIT(USB3_REF_ITP_CLKEN, 1)
    REGDEF_BIT(                ,1)
    REGDEF_BIT(DSP_CLKEN       ,1)
    REGDEF_BIT(DSP2_CLKEN      ,1)
    REGDEF_BIT(CPU_CORETIMER_CLKEN,1)
    REGDEF_BIT(SIE3_PX_CLKEN   ,1)
    REGDEF_BIT(SIE4_PX_CLKEN   ,1)
    REGDEF_BIT(SIE5_PX_CLKEN   ,1)
    REGDEF_BIT(SIE6_PX_CLKEN   ,1)
    REGDEF_BIT(SIE7_PX_CLKEN   ,1)
    REGDEF_BIT(SIE8_PX_CLKEN   ,1)
    REGDEF_BIT(TSDEMUX_CLKEN   ,1)
    REGDEF_BIT(                ,11)
REGDEF_END(PLL_CLKEN4_REG)


//
//0xA0 System Reset Register 0
//
#define PLL_SYS_RST0_REG_OFS            0xA0
REGDEF_BEGIN(PLL_SYS_RST0_REG)
    REGDEF_BIT(RSTN_SDRAMC      ,1)
    REGDEF_BIT(RSTN_SDRAMC2     ,1)
    REGDEF_BIT(                 ,2)
    REGDEF_BIT(RSTN_SIE         ,1)
    REGDEF_BIT(RSTN_SIE2        ,1)
    REGDEF_BIT(RSTN_TGE         ,1)
    REGDEF_BIT(RSTN_IPE         ,1)
    REGDEF_BIT(RSTN_DIS         ,1)     //8
    REGDEF_BIT(RSTN_IME         ,1)
    REGDEF_BIT(RSTN_FDE         ,1)
    REGDEF_BIT(RSTN_ISE         ,1)
    REGDEF_BIT(                 ,1)
    REGDEF_BIT(RSTN_IFE         ,1)
    REGDEF_BIT(RSTN_DCE         ,1)
    REGDEF_BIT(RSTN_IFE2        ,1)
    REGDEF_BIT(RSTN_IDE         ,1)     //16
    REGDEF_BIT(RSTN_IDE2        ,1)
    REGDEF_BIT(RSTN_TV          ,1)
    REGDEF_BIT(                 ,1)
    REGDEF_BIT(RSTN_MI          ,1)
    REGDEF_BIT(RSTN_CEC         ,1)
    REGDEF_BIT(RSTN_HDMI        ,1)
    REGDEF_BIT(RSTN_CRYPTO      ,1)
    REGDEF_BIT(RSTN_H264        ,1)     //24
    REGDEF_BIT(RSTN_AFFINE      ,1)
    REGDEF_BIT(RSTN_JPG         ,1)
    REGDEF_BIT(RSTN_GRAPH       ,1)
    REGDEF_BIT(RSTN_GRAPH2      ,1)
    REGDEF_BIT(RSTN_DAI         ,1)
    REGDEF_BIT(RSTN_EAC         ,1)
    REGDEF_BIT(                 ,1)
REGDEF_END(PLL_SYS_RST0_REG)


//
//0xA4 System Reset Register 1
//
#define PLL_SYS_RST1_REG_OFS            0xA4
REGDEF_BEGIN(PLL_SYS_RST1_REG)
    REGDEF_BIT(RSTN_NAND        ,1)
    REGDEF_BIT(                 ,1)
    REGDEF_BIT(RSTN_SDIO        ,1)
    REGDEF_BIT(RSTN_SDIO2       ,1)
    REGDEF_BIT(RSTN_I2C         ,1)
    REGDEF_BIT(RSTN_I2C2        ,1)
    REGDEF_BIT(RSTN_SPI         ,1)
    REGDEF_BIT(RSTN_SPI2        ,1)
    REGDEF_BIT(RSTN_SPI3        ,1)     //8
    REGDEF_BIT(RSTN_SIF         ,1)
    REGDEF_BIT(RSTN_UART        ,1)
    REGDEF_BIT(RSTN_UART2       ,1)
    REGDEF_BIT(RSTN_RMT         ,1)
    REGDEF_BIT(RSTN_ADC         ,1)
    REGDEF_BIT(RSTN_SDIO3       ,1)
    REGDEF_BIT(RSTN_SPI4        ,1)
    REGDEF_BIT(RSTN_RTC         ,1)     //16
    REGDEF_BIT(RSTN_WDT         ,1)
    REGDEF_BIT(RSTN_TMR         ,1)
    REGDEF_BIT(RSTN_USB         ,1)
    REGDEF_BIT(                 ,1)
    REGDEF_BIT(                 ,1)
    REGDEF_BIT(RSTN_UART3       ,1)
    REGDEF_BIT(RSTN_UART4       ,1)
    REGDEF_BIT(RSTN_INTC        ,1)     //24
    REGDEF_BIT(RSTN_GPIO        ,1)
    REGDEF_BIT(RSTN_PAD         ,1)
    REGDEF_BIT(RSTN_TOP         ,1)
    REGDEF_BIT(RSTN_EFUSE       ,1)
    REGDEF_BIT(RSTN_ETH         ,1)
    REGDEF_BIT(                 ,1)
    REGDEF_BIT(RSTN_I2C3        ,1)
REGDEF_END(PLL_SYS_RST1_REG)

//
//0xA8 System Reset Register 2
//
#define PLL_SYS_RST2_REG_OFS            0xA8
REGDEF_BEGIN(PLL_SYS_RST2_REG)
    REGDEF_BIT(                 ,1)
    REGDEF_BIT(RSTN_MIPI_LVDS   ,1)
    REGDEF_BIT(RSTN_MIPI_LVDS2  ,1)
    REGDEF_BIT(RSTN_MIPI_DSI    ,1)
    REGDEF_BIT(                 ,2)
    REGDEF_BIT(                 ,1)
    REGDEF_BIT(                 ,1)
    REGDEF_BIT(RSTN_PWM         ,1)
    REGDEF_BIT(                 ,3)
    REGDEF_BIT(RSTN_DRE         ,1)
    REGDEF_BIT(RSTN_TSMUX       ,1)
    REGDEF_BIT(RSTN_VX1         ,1)
    REGDEF_BIT(RSTN_SIE3        ,1)
    REGDEF_BIT(RSTN_SIE4        ,1)
    REGDEF_BIT(RSTN_USB2        ,1)
    REGDEF_BIT(RSTN_CC          ,1)
    REGDEF_BIT(RSTN_RHE         ,1)
    REGDEF_BIT(RSTN_SL          ,1)
    REGDEF_BIT(RSTN_SL_REG      ,1)
    REGDEF_BIT(RSTN_SL2         ,1)
    REGDEF_BIT(RSTN_SL2_REG     ,1)
    REGDEF_BIT(RSTN_VX12        ,1)
    REGDEF_BIT(RSTN_CANBUS      ,1)
    REGDEF_BIT(RSTN_I2C4        ,1)
    REGDEF_BIT(RSTN_I2C5        ,1)
    REGDEF_BIT(RSTN_I2C6        ,1)
    REGDEF_BIT(RSTN_I2C7        ,1)
    REGDEF_BIT(RSTN_HWCPY       ,1)
    REGDEF_BIT(RSTN_ROTATE      ,1)
REGDEF_END(PLL_SYS_RST2_REG)

//
//0xAC System Reset Register 3
//
#define PLL_SYS_RST3_REG_OFS            0xAC
REGDEF_BEGIN(PLL_SYS_RST3_REG)
    REGDEF_BIT(                 ,20)
    REGDEF_BIT(RSTN_MIPI_LVDS3  ,1)
    REGDEF_BIT(RSTN_MIPI_LVDS4  ,1)
    REGDEF_BIT(RSTN_MIPI_LVDS5  ,1)
    REGDEF_BIT(RSTN_MIPI_LVDS6  ,1)
    REGDEF_BIT(RSTN_MIPI_LVDS7  ,1)
    REGDEF_BIT(RSTN_MIPI_LVDS8  ,1)
    REGDEF_BIT(RSTN_SIE5        ,1)
    REGDEF_BIT(RSTN_SIE6        ,1)
    REGDEF_BIT(RSTN_SIE7        ,1)
    REGDEF_BIT(RSTN_SIE8        ,1)
    REGDEF_BIT(RSTN_IVE         ,1)
    REGDEF_BIT(RSTN_SDE         ,1)
REGDEF_END(PLL_SYS_RST3_REG)

//
//0xB0 System Reset Register 4
//
#define PLL_SYS_RST4_REG_OFS            0xB0
REGDEF_BEGIN(PLL_SYS_RST4_REG)
    REGDEF_BIT(RSTN_SVM         ,1)
    REGDEF_BIT(RSTN_CNN         ,1)
    REGDEF_BIT(                 ,3)
    REGDEF_BIT(                 ,1)
    REGDEF_BIT(RSTN_DDRPHY      ,1)
    REGDEF_BIT(RSTN_DDRPHY2     ,1)
    REGDEF_BIT(                 ,2)
    REGDEF_BIT(RSTN_SLVSEC      ,1)
    REGDEF_BIT(RSTN_DSP         ,1)
    REGDEF_BIT(RSTN_DSP2        ,1)
    REGDEF_BIT(                 ,19)
REGDEF_END(PLL_SYS_RST4_REG)

//
//0xB4 System Reset Register 5
//
#define PLL_SYS_RST5_REG_OFS            0xB4
REGDEF_BEGIN(PLL_SYS_RST5_REG)
    REGDEF_BIT(RSTN_DSP_NIC      ,1)
    REGDEF_BIT(RSTN_DSP_APBTX    ,1)
    REGDEF_BIT(RSTN_DSP_AXIB_EPP ,1)
    REGDEF_BIT(RSTN_DSP_AXIB_EDP ,1)
    REGDEF_BIT(                  ,4)
    REGDEF_BIT(RSTN_DSP2_NIC     ,1)
    REGDEF_BIT(RSTN_DSP2_APBTX   ,1)
    REGDEF_BIT(RSTN_DSP2_AXIB_EPP,1)
    REGDEF_BIT(RSTN_DSP2_AXIB_EDP,1)
    REGDEF_BIT(                  ,4)
    REGDEF_BIT(RSTN_DAP_AXIB     ,1)
    REGDEF_BIT(                  ,3)
    REGDEF_BIT(RSTN_USB_AXIB     ,1)
    REGDEF_BIT(                  ,3)
    REGDEF_BIT(RSTN_ETH_AXIB     ,1)
    REGDEF_BIT(                  ,3)
    REGDEF_BIT(                  ,4)
REGDEF_END(PLL_SYS_RST5_REG)

//
//0xB8 ~ 0xBC Reserved register
//


//
//0xC0 Sleep Register
//
#define PLL_SLP_REG_OFS                 0xC0
REGDEF_BEGIN(PLL_SLP_REG)
    REGDEF_BIT(SLP_MODE         ,2)
    REGDEF_BIT(SLP_EN           ,1)
    REGDEF_BIT(SLP_INT          ,1)
    REGDEF_BIT(SLP_INTE         ,1)
    REGDEF_BIT(reserved1        ,27)
REGDEF_END(PLL_SLP_REG)

//
//0xC4~0xCC Reserved Registers
//

//
//0xE0 CLK Auto Gating Register 0
//
#define PLL_CLKGATE0_REG_OFS            0xE0
REGDEF_BEGIN(PLL_CLKGATE0_REG)
    REGDEF_BIT(                 ,1)
    REGDEF_BIT(                 ,1)
    REGDEF_BIT(IPE              ,1)
    REGDEF_BIT(IME              ,1)
    REGDEF_BIT(DIS              ,1)
    REGDEF_BIT(FDE              ,1)
    REGDEF_BIT(DCE              ,1)
    REGDEF_BIT(IFE              ,1)
    REGDEF_BIT(GRA              ,1)     //8
    REGDEF_BIT(GRA2             ,1)
    REGDEF_BIT(                 ,1)
    REGDEF_BIT(                 ,1)
    REGDEF_BIT(                 ,1)
    REGDEF_BIT(                 ,1)
    REGDEF_BIT(IFE2             ,1)
    REGDEF_BIT(JPG              ,1)
    REGDEF_BIT(H264             ,1)     //16
    REGDEF_BIT(                 ,1)
    REGDEF_BIT(                 ,1)
    REGDEF_BIT(NAND             ,1)
    REGDEF_BIT(SDIO             ,1)
    REGDEF_BIT(SDIO2            ,1)
    REGDEF_BIT(                 ,1)
    REGDEF_BIT(                 ,1)
    REGDEF_BIT(SPI              ,1)     //24
    REGDEF_BIT(SPI2             ,1)
    REGDEF_BIT(SPI3             ,1)
    REGDEF_BIT(SIF              ,1)
    REGDEF_BIT(                 ,1)
    REGDEF_BIT(                 ,1)
    REGDEF_BIT(                 ,1)
    REGDEF_BIT(                 ,1)
REGDEF_END(PLL_CLKGATE0_REG)

//
//0xE4 CLK Auto Gating Register 1
//
#define PLL_CLKGATE1_REG_OFS            0xE4
REGDEF_BEGIN(PLL_CLKGATE1_REG)
    REGDEF_BIT(                 ,1)
    REGDEF_BIT(                 ,1)
    REGDEF_BIT(                 ,1)
    REGDEF_BIT(                 ,1)
    REGDEF_BIT(                 ,1)
    REGDEF_BIT(                 ,1)
    REGDEF_BIT(                 ,1)
    REGDEF_BIT(                 ,1)
    REGDEF_BIT(ISE              ,1)     //8
    REGDEF_BIT(                 ,1)
    REGDEF_BIT(AFFINE           ,1)
    REGDEF_BIT(PWM              ,1)
    REGDEF_BIT(                 ,1)     //PWMCCNT
    REGDEF_BIT(                 ,1)     //RDE
    REGDEF_BIT(DRE              ,1)
    REGDEF_BIT(SDIO3            ,1)
    REGDEF_BIT(                 ,2)
    REGDEF_BIT(SPI4             ,1)
    REGDEF_BIT(                 ,1)     //SPI5
    REGDEF_BIT(                 ,9)
    REGDEF_BIT(HWCPY            ,1)
    REGDEF_BIT(RORATE           ,1)
    REGDEF_BIT(                 ,1)
REGDEF_END(PLL_CLKGATE1_REG)

//
//0xE8 CLK Auto Gating Register 2
//
#define PLL_CLKGATE2_REG_OFS            0xE8
REGDEF_BEGIN(PLL_CLKGATE2_REG)
    REGDEF_BIT(                 ,1)
    REGDEF_BIT(                 ,1)
    REGDEF_BIT(                 ,1)
    REGDEF_BIT(                 ,1)
    REGDEF_BIT(                 ,1)
    REGDEF_BIT(IVE              ,1)
    REGDEF_BIT(SDE              ,1)
    REGDEF_BIT(SVM              ,1)
    REGDEF_BIT(CNN              ,1)
    REGDEF_BIT(                 ,10)
    REGDEF_BIT(CANBUS           ,1)
    REGDEF_BIT(                 ,3)
    REGDEF_BIT(                 ,1)
    REGDEF_BIT(                 ,8)
REGDEF_END(PLL_CLKGATE2_REG)

//
//0xEC Reserved Register
//

//
//0xF0 PCLK Auto Gating Register 0
//
#define PLL_PCLKGATE0_REG_OFS           0xF0
REGDEF_BEGIN(PLL_PCLKGATE0_REG)
    REGDEF_BIT(SIE              ,1)
    REGDEF_BIT(                 ,1)
    REGDEF_BIT(IPE              ,1)
    REGDEF_BIT(IME              ,1)
    REGDEF_BIT(DIS              ,1)
    REGDEF_BIT(FDE              ,1)
    REGDEF_BIT(DCE              ,1)
    REGDEF_BIT(IFE              ,1)
    REGDEF_BIT(GRA              ,1)     //8
    REGDEF_BIT(GRA2             ,1)
    REGDEF_BIT(IDE              ,1)
    REGDEF_BIT(IDE2             ,1)
    REGDEF_BIT(MI               ,1)
    REGDEF_BIT(HDMI             ,1)
    REGDEF_BIT(IFE2             ,1)
    REGDEF_BIT(JPG              ,1)
    REGDEF_BIT(H264             ,1)     //16
    REGDEF_BIT(DAI              ,1)
    REGDEF_BIT(EAC              ,1)
    REGDEF_BIT(NAND             ,1)
    REGDEF_BIT(SDIO             ,1)
    REGDEF_BIT(SDIO2            ,1)
    REGDEF_BIT(I2C              ,1)
    REGDEF_BIT(I2C2             ,1)
    REGDEF_BIT(SPI              ,1)     //24
    REGDEF_BIT(SPI2             ,1)
    REGDEF_BIT(SPI3             ,1)
    REGDEF_BIT(SIF              ,1)
    REGDEF_BIT(UART             ,1)
    REGDEF_BIT(UART2            ,1)
    REGDEF_BIT(RM               ,1)
    REGDEF_BIT(ADC              ,1)
REGDEF_END(PLL_PCLKGATE0_REG)

//
//0xF4 PCLK Auto Gating Register 1
//
#define PLL_PCLKGATE1_REG_OFS           0xF4
REGDEF_BEGIN(PLL_PCLKGATE1_REG)
    REGDEF_BIT(TMR              ,1)
    REGDEF_BIT(WDT              ,1)
    REGDEF_BIT(                 ,1)
    REGDEF_BIT(MIPI_LVDS        ,1)
    REGDEF_BIT(MIPI_LVDS2       ,1)
    REGDEF_BIT(TV               ,1)
    REGDEF_BIT(MIPI_DSI         ,1)
    REGDEF_BIT(                 ,1)
    REGDEF_BIT(ISE              ,1)     //8
    REGDEF_BIT(SIE2             ,1)
    REGDEF_BIT(AFFINE           ,1)
    REGDEF_BIT(PWM              ,1)
    REGDEF_BIT(                 ,1)
    REGDEF_BIT(                 ,1)     //RDE
    REGDEF_BIT(DRE              ,1)
    REGDEF_BIT(SDIO3            ,1)
    REGDEF_BIT(UART3            ,1)
    REGDEF_BIT(UART4            ,1)
    REGDEF_BIT(SPI4             ,1)
    REGDEF_BIT(                 ,1)     //SPI5
    REGDEF_BIT(TGE              ,1)
    REGDEF_BIT(SIE3             ,1)
    REGDEF_BIT(SIE4             ,1)
    REGDEF_BIT(RHE              ,1)
    REGDEF_BIT(I2C3             ,1)
    REGDEF_BIT(I2C4             ,1)
    REGDEF_BIT(I2C5             ,1)
    REGDEF_BIT(I2C6             ,1)
    REGDEF_BIT(I2C7             ,1)
    REGDEF_BIT(HWCPY            ,1)
    REGDEF_BIT(ROTATE           ,1)
    REGDEF_BIT(                 ,1)
REGDEF_END(PLL_PCLKGATE1_REG)


//
//0xF8 PCLK Auto Gating Register 2
//
#define PLL_PCLKGATE2_REG_OFS           0xF8
REGDEF_BEGIN(PLL_PCLKGATE2_REG)
    REGDEF_BIT(GPIO             ,1)
    REGDEF_BIT(INTC             ,1)
    REGDEF_BIT(                 ,1) //RTC
    REGDEF_BIT(DMA              ,1)
    REGDEF_BIT(DMA2             ,1)
    REGDEF_BIT(IVE              ,1)
    REGDEF_BIT(SDE              ,1)
    REGDEF_BIT(SVM              ,1)
    REGDEF_BIT(CNN              ,1)
    REGDEF_BIT(CSI_LVDS3        ,1)
    REGDEF_BIT(CSI_LVDS4        ,1)
    REGDEF_BIT(CSI_LVDS5        ,1)
    REGDEF_BIT(CSI_LVDS6        ,1)
    REGDEF_BIT(CSI_LVDS7        ,1)
    REGDEF_BIT(CSI_LVDS8        ,1)
    REGDEF_BIT(SIE5             ,1)
    REGDEF_BIT(SIE6             ,1)
    REGDEF_BIT(SIE7             ,1)
    REGDEF_BIT(SIE8             ,1)
    REGDEF_BIT(CANBUS           ,1)
    REGDEF_BIT(                 ,3)
    REGDEF_BIT(                 ,1)
    REGDEF_BIT(reserved1        ,8)
REGDEF_END(PLL_PCLKGATE2_REG)

//
//0x100 SRAM ShutDown Register 0
//
#define PLL_SYS_SRAMSD0_REG_OFS            0x100
REGDEF_BEGIN(PLL_SYS_SRAMSD0_REG)
    REGDEF_BIT(                 ,1)
    REGDEF_BIT(                 ,1)
    REGDEF_BIT(                 ,2)
    REGDEF_BIT(RSTN_SIE         ,1)
    REGDEF_BIT(RSTN_SIE2        ,1)
    REGDEF_BIT(                 ,1)
    REGDEF_BIT(RSTN_IPE         ,1)
    REGDEF_BIT(RSTN_DIS         ,1)     //8
    REGDEF_BIT(RSTN_IME         ,1)
    REGDEF_BIT(RSTN_FDE         ,1)
    REGDEF_BIT(RSTN_ISE         ,1)
    REGDEF_BIT(                 ,1)
    REGDEF_BIT(RSTN_IFE         ,1)
    REGDEF_BIT(RSTN_DCE         ,1)
    REGDEF_BIT(RSTN_IFE2        ,1)
    REGDEF_BIT(RSTN_IDE         ,1)     //16
    REGDEF_BIT(RSTN_IDE2        ,1)
    REGDEF_BIT(                 ,1)
    REGDEF_BIT(                 ,1)
    REGDEF_BIT(                 ,1)
    REGDEF_BIT(                 ,1)
    REGDEF_BIT(                 ,1)
    REGDEF_BIT(                 ,1)
    REGDEF_BIT(RSTN_H264        ,1)     //24
    REGDEF_BIT(                 ,1)
    REGDEF_BIT(RSTN_JPG         ,1)
    REGDEF_BIT(RSTN_GRAPH       ,1)
    REGDEF_BIT(RSTN_GRAPH2      ,1)
    REGDEF_BIT(                 ,1)
    REGDEF_BIT(                 ,1)
    REGDEF_BIT(                 ,1)
REGDEF_END(PLL_SYS_SRAMSD0_REG)


//
//0x104 SRAM ShutDown Register 1
//
#define PLL_SYS_SRAMSD1_REG_OFS            0x104
REGDEF_BEGIN(PLL_SYS_SRAMSD1_REG)
    REGDEF_BIT(RSTN_NAND        ,1)
    REGDEF_BIT(                 ,1)
    REGDEF_BIT(                 ,1)
    REGDEF_BIT(                 ,1)
    REGDEF_BIT(                 ,1)
    REGDEF_BIT(                 ,1)
    REGDEF_BIT(                 ,1)
    REGDEF_BIT(                 ,1)
    REGDEF_BIT(                 ,1)     //8
    REGDEF_BIT(                 ,1)
    REGDEF_BIT(                 ,1)
    REGDEF_BIT(                 ,1)
    REGDEF_BIT(                 ,1)
    REGDEF_BIT(                 ,1)
    REGDEF_BIT(                 ,1)
    REGDEF_BIT(                 ,1)
    REGDEF_BIT(                 ,1)     //16
    REGDEF_BIT(                 ,1)
    REGDEF_BIT(                 ,1)
    REGDEF_BIT(RSTN_USB         ,1)
    REGDEF_BIT(                 ,1)
    REGDEF_BIT(                 ,1)
    REGDEF_BIT(                 ,1)
    REGDEF_BIT(                 ,1)
    REGDEF_BIT(                 ,1)     //24
    REGDEF_BIT(                 ,1)
    REGDEF_BIT(                 ,1)
    REGDEF_BIT(                 ,1)
    REGDEF_BIT(                 ,1)
    REGDEF_BIT(RSTN_ETH         ,1)
    REGDEF_BIT(                 ,2)
REGDEF_END(PLL_SYS_SRAMSD1_REG)

//
//0x108 SRAM ShutDown Register 2
//
#define PLL_SYS_SRAMSD2_REG_OFS            0x108
REGDEF_BEGIN(PLL_SYS_SRAMSD2_REG)
    REGDEF_BIT(                 ,1)
    REGDEF_BIT(                 ,1)
    REGDEF_BIT(                 ,1)
    REGDEF_BIT(RSTN_MIPI_DSI    ,1)
    REGDEF_BIT(                 ,2)
    REGDEF_BIT(RSTN_RDE         ,1)
    REGDEF_BIT(                 ,1)
    REGDEF_BIT(                 ,1)
    REGDEF_BIT(                 ,3)
    REGDEF_BIT(RSTN_DRE         ,1)
    REGDEF_BIT(RSTN_TSMUX       ,1)
    REGDEF_BIT(                 ,1)
    REGDEF_BIT(RSTN_SIE3        ,1)
    REGDEF_BIT(RSTN_SIE4        ,1)
    REGDEF_BIT(RSTN_USB2        ,1)
    REGDEF_BIT(                 ,1)
    REGDEF_BIT(RSTN_RHE         ,1)
    REGDEF_BIT(                 ,12)
REGDEF_END(PLL_SYS_SRAMSD2_REG)


//
//0x10C~0x110 Reserved Register
//

//
//0x114 CPU2 SRAM ShutDown Register
//
#define PLL_CPU2_SRAMSD_REG_OFS            0x114
REGDEF_BEGIN(PLL_CPU2_SRAMSD_REG_REG)
    REGDEF_BIT(                 ,1)
    REGDEF_BIT(                 ,1)
    REGDEF_BIT(                 ,1)
    REGDEF_BIT(                 ,1)
    REGDEF_BIT(                 ,28)
REGDEF_END(PLL_CPU2_SRAMSD_REG_REG)

//
//0x118~0xFFC Reserved Register
//

//
//0x1188 PLL8 (For CPU) Clock Rate Register 0
//
#define PLL_PLL8_CR0_REG_OFS            0x1188
REGDEF_BEGIN(PLL_PLL8_CR0_REG)
    REGDEF_BIT(PLL_RATIO0       ,8)
    REGDEF_BIT(reserved1        ,24)
REGDEF_END(PLL_PLL8_CR0_REG)

//
//0x118C PLL8 (For CPU) Clock Rate Register 1
//
#define PLL_PLL8_CR1_REG_OFS            0x118C
REGDEF_BEGIN(PLL_PLL8_CR1_REG)
    REGDEF_BIT(PLL_RATIO1       ,8)
    REGDEF_BIT(reserved1        ,24)
REGDEF_END(PLL_PLL8_CR1_REG)

//
//0x1190 PLL8 (For CPU) Clock Rate Register 2
//
#define PLL_PLL8_CR2_REG_OFS            0x1190
REGDEF_BEGIN(PLL_PLL8_CR2_REG)
    REGDEF_BIT(PLL_RATIO2       ,8)
    REGDEF_BIT(reserved1        ,24)
REGDEF_END(PLL_PLL8_CR2_REG)

//
//0x1208 PLL3 (For DMA) Clock Rate Register 0
//
#define PLL_PLL3_CR0_REG_OFS            0x1208
REGDEF_BEGIN(PLL_PLL3_CR0_REG)
    REGDEF_BIT(PLL_RATIO0       ,8)
    REGDEF_BIT(reserved1        ,24)
REGDEF_END(PLL_PLL3_CR0_REG)

//
//0x120C PLL3 (For DMA) Clock Rate Register 1
//
#define PLL_PLL3_CR1_REG_OFS            0x120C
REGDEF_BEGIN(PLL_PLL3_CR1_REG)
    REGDEF_BIT(PLL_RATIO1       ,8)
    REGDEF_BIT(reserved1        ,24)
REGDEF_END(PLL_PLL3_CR1_REG)

//
//0x1210 PLL3 (For DMA) Clock Rate Register 2
//
#define PLL_PLL3_CR2_REG_OFS            0x1210
REGDEF_BEGIN(PLL_PLL3_CR2_REG)
    REGDEF_BIT(PLL_RATIO2       ,8)
    REGDEF_BIT(reserved1        ,24)
REGDEF_END(PLL_PLL3_CR2_REG)

//
//0x1228 PLL14 (For DMA2) Clock Rate Register 0
//
#define PLL_PLL14_CR0_REG_OFS           0x1228
REGDEF_BEGIN(PLL_PLL14_CR0_REG)
    REGDEF_BIT(PLL_RATIO0       ,8)
    REGDEF_BIT(reserved1        ,24)
REGDEF_END(PLL_PLL14_CR0_REG)

//
//0x122C PLL14 (For DMA2) Clock Rate Register 1
//
#define PLL_PLL14_CR1_REG_OFS           0x122C
REGDEF_BEGIN(PLL_PLL14_CR1_REG)
    REGDEF_BIT(PLL_RATIO1       ,8)
    REGDEF_BIT(reserved1        ,24)
REGDEF_END(PLL_PLL14_CR1_REG)

//
//0x1230 PLL14 (For DMA2) Clock Rate Register 2
//
#define PLL_PLL14_CR2_REG_OFS           0x1230
REGDEF_BEGIN(PLL_PLL14_CR2_REG)
    REGDEF_BIT(PLL_RATIO2       ,8)
    REGDEF_BIT(reserved1        ,24)
REGDEF_END(PLL_PLL14_CR2_REG)


//
//0x1248 PLL12 (For ETH) Clock Rate Register 0
//
#define PLL_PLL12_CR0_REG_OFS            0x1248
REGDEF_BEGIN(PLL_PLL12_CR0_REG)
    REGDEF_BIT(PLL_RATIO0       ,8)
    REGDEF_BIT(reserved1        ,24)
REGDEF_END(PLL_PLL12_CR0_REG)

//
//0x124C PLL12 (For ETH) Clock Rate Register 1
//
#define PLL_PLL12_CR1_REG_OFS            0x124C
REGDEF_BEGIN(PLL_PLL12_CR1_REG)
    REGDEF_BIT(PLL_RATIO1       ,8)
    REGDEF_BIT(reserved1        ,24)
REGDEF_END(PLL_PLL12_CR1_REG)

//
//0x1250 PLL12 (For ETH) Clock Rate Register 2
//
#define PLL_PLL12_CR2_REG_OFS            0x1250
REGDEF_BEGIN(PLL_PLL12_CR2_REG)
    REGDEF_BIT(PLL_RATIO2       ,8)
    REGDEF_BIT(reserved1        ,24)
REGDEF_END(PLL_PLL12_CR2_REG)

//
//0x1268 PLL11 (For DSI) Clock Rate Register 0
//
#define PLL_PLL11_CR0_REG_OFS            0x1268
REGDEF_BEGIN(PLL_PLL11_CR0_REG)
    REGDEF_BIT(PLL_RATIO0       ,8)
    REGDEF_BIT(reserved1        ,24)
REGDEF_END(PLL_PLL11_CR0_REG)

//
//0x126C PLL11 (For DSI) Clock Rate Register 1
//
#define PLL_PLL11_CR1_REG_OFS            0x126C
REGDEF_BEGIN(PLL_PLL11_CR1_REG)
    REGDEF_BIT(PLL_RATIO1       ,8)
    REGDEF_BIT(reserved1        ,24)
REGDEF_END(PLL_PLL11_CR1_REG)

//
//0x1270 PLL11 (For DSI) Clock Rate Register 2
//
#define PLL_PLL11_CR2_REG_OFS            0x1270
REGDEF_BEGIN(PLL_PLL11_CR2_REG)
    REGDEF_BIT(PLL_RATIO2       ,8)
    REGDEF_BIT(reserved1        ,24)
REGDEF_END(PLL_PLL11_CR2_REG)


//
//0x1288 PLL2 (For SS2) Clock Rate Register 0
//
#define PLL_PLL2_CR0_REG_OFS            0x1288
REGDEF_BEGIN(PLL_PLL2_CR0_REG)
    REGDEF_BIT(PLL_RATIO0       ,8)
    REGDEF_BIT(reserved1        ,24)
REGDEF_END(PLL_PLL2_CR0_REG)

//
//0x128C PLL2 (For SS2) Clock Rate Register 1
//
#define PLL_PLL2_CR1_REG_OFS            0x128C
REGDEF_BEGIN(PLL_PLL2_CR1_REG)
    REGDEF_BIT(PLL_RATIO1       ,8)
    REGDEF_BIT(reserved1        ,24)
REGDEF_END(PLL_PLL2_CR1_REG)

//
//0x1290 PLL2 (For SS2) Clock Rate Register 2
//
#define PLL_PLL2_CR2_REG_OFS            0x1290
REGDEF_BEGIN(PLL_PLL2_CR2_REG)
    REGDEF_BIT(PLL_RATIO2       ,8)
    REGDEF_BIT(reserved1        ,24)
REGDEF_END(PLL_PLL2_CR2_REG)


//
//0x11A8 PLL9 (For DSP1) Clock Rate Register 0
//
#define PLL_PLL9_CR0_REG_OFS            0x12A8
REGDEF_BEGIN(PLL_PLL9_CR0_REG)
    REGDEF_BIT(PLL_RATIO0       ,8)
    REGDEF_BIT(reserved1        ,24)
REGDEF_END(PLL_PLL9_CR0_REG)

//
//0x11AC PLL9 (For DSP1) Clock Rate Register 1
//
#define PLL_PLL9_CR1_REG_OFS            0x12AC
REGDEF_BEGIN(PLL_PLL9_CR1_REG)
    REGDEF_BIT(PLL_RATIO1       ,8)
    REGDEF_BIT(reserved1        ,24)
REGDEF_END(PLL_PLL9_CR1_REG)

//
//0x11B0 PLL9 (For DSP1) Clock Rate Register 2
//
#define PLL_PLL9_CR2_REG_OFS            0x12B0
REGDEF_BEGIN(PLL_PLL9_CR2_REG)
    REGDEF_BIT(PLL_RATIO2       ,8)
    REGDEF_BIT(reserved1        ,24)
REGDEF_END(PLL_PLL9_CR2_REG)


//
//0x12C8 PLL6 (For IPP) Clock Rate Register 0
//
#define PLL_PLL6_CR0_REG_OFS            0x12C8
REGDEF_BEGIN(PLL_PLL6_CR0_REG)
    REGDEF_BIT(PLL_RATIO0       ,8)
    REGDEF_BIT(reserved1        ,24)
REGDEF_END(PLL_PLL6_CR0_REG)

//
//0x12CC PLL6 (For IPP) Clock Rate Register 1
//
#define PLL_PLL6_CR1_REG_OFS            0x12CC
REGDEF_BEGIN(PLL_PLL6_CR1_REG)
    REGDEF_BIT(PLL_RATIO1       ,8)
    REGDEF_BIT(reserved1        ,24)
REGDEF_END(PLL_PLL6_CR1_REG)

//
//0x12D0 PLL6 (For IPP) Clock Rate Register 2
//
#define PLL_PLL6_CR2_REG_OFS            0x12D0
REGDEF_BEGIN(PLL_PLL6_CR2_REG)
    REGDEF_BIT(PLL_RATIO2       ,8)
    REGDEF_BIT(reserved1        ,24)
REGDEF_END(PLL_PLL6_CR2_REG)

//
//0x12E8 PLL7 (For audio) Clock Rate Register 0
//
#define PLL_PLL7_CR0_REG_OFS            0x12E8
REGDEF_BEGIN(PLL_PLL7_CR0_REG)
    REGDEF_BIT(PLL_RATIO0       ,8)
    REGDEF_BIT(reserved1        ,24)
REGDEF_END(PLL_PLL7_CR0_REG)

//
//0x12EC PLL7 (For audio) Clock Rate Register 1
//
#define PLL_PLL7_CR1_REG_OFS            0x12EC
REGDEF_BEGIN(PLL_PLL7_CR1_REG)
    REGDEF_BIT(PLL_RATIO1       ,8)
    REGDEF_BIT(reserved1        ,24)
REGDEF_END(PLL_PLL7_CR1_REG)

//
//0x12F0 PLL7 (For audio) Clock Rate Register 2
//
#define PLL_PLL7_CR2_REG_OFS            0x12F0
REGDEF_BEGIN(PLL_PLL7_CR2_REG)
    REGDEF_BIT(PLL_RATIO2       ,8)
    REGDEF_BIT(reserved1        ,24)
REGDEF_END(PLL_PLL7_CR2_REG)

//
//0x1304 PLL13 (For H264) Clock Rate Register 0
//
#define PLL_PLL13_CR0_REG_OFS           0x1304
REGDEF_BEGIN(PLL_PLL13_CR0_REG)
    REGDEF_BIT(PLL_RATIO0       ,8)
    REGDEF_BIT(reserved1        ,24)
REGDEF_END(PLL_PLL13_CR0_REG)

//
//0x1308 PLL13 (For H264) Clock Rate Register 1
//
#define PLL_PLL13_CR1_REG_OFS           0x1308
REGDEF_BEGIN(PLL_PLL13_CR1_REG)
    REGDEF_BIT(PLL_RATIO1       ,8)
    REGDEF_BIT(reserved1        ,24)
REGDEF_END(PLL_PLL13_CR1_REG)

//
//0x130C PLL13 (For H264) Clock Rate Register 2
//
#define PLL_PLL13_CR2_REG_OFS           0x130C
REGDEF_BEGIN(PLL_PLL13_CR2_REG)
    REGDEF_BIT(PLL_RATIO2       ,8)
    REGDEF_BIT(reserved1        ,24)
REGDEF_END(PLL_PLL13_CR2_REG)

//
//0x1318 PLL4 (For SSPLL) Clock Rate Register 0
//
#define PLL_PLL4_CR0_REG_OFS            0x1318
REGDEF_BEGIN(PLL_PLL4_CR0_REG)
    REGDEF_BIT(PLL_RATIO0       ,8)
    REGDEF_BIT(reserved1        ,24)
REGDEF_END(PLL_PLL4_CR0_REG)

//
//0x131C PLL4 (For SSPLL) Clock Rate Register 1
//
#define PLL_PLL4_CR1_REG_OFS            0x131C
REGDEF_BEGIN(PLL_PLL4_CR1_REG)
    REGDEF_BIT(PLL_RATIO1       ,8)
    REGDEF_BIT(reserved1        ,24)
REGDEF_END(PLL_PLL4_CR1_REG)

//
//0x1320 PLL4 (For SSPLL) Clock Rate Register 2
//
#define PLL_PLL4_CR2_REG_OFS            0x1320
REGDEF_BEGIN(PLL_PLL4_CR2_REG)
    REGDEF_BIT(PLL_RATIO2       ,8)
    REGDEF_BIT(reserved1        ,24)
REGDEF_END(PLL_PLL4_CR2_REG)

//
//0x13D0 PLL5 (For sensor) Clock Rate Register 0
//
#define PLL_PLL5_CR0_REG_OFS            0x13D0
REGDEF_BEGIN(PLL_PLL5_CR0_REG)
    REGDEF_BIT(PLL_RATIO0       ,8)
    REGDEF_BIT(reserved1        ,24)
REGDEF_END(PLL_PLL5_CR0_REG)

//
//0x13D4 PLL5 (For sensor) Clock Rate Register 1
//
#define PLL_PLL5_CR1_REG_OFS            0x13D4
REGDEF_BEGIN(PLL_PLL5_CR1_REG)
    REGDEF_BIT(PLL_RATIO1       ,8)
    REGDEF_BIT(reserved1        ,24)
REGDEF_END(PLL_PLL5_CR1_REG)

//
//0x13D8 PLL5 (For sensor) Clock Rate Register 2
//
#define PLL_PLL5_CR2_REG_OFS            0x13D8
REGDEF_BEGIN(PLL_PLL5_CR2_REG)
    REGDEF_BIT(PLL_RATIO2       ,8)
    REGDEF_BIT(reserved1        ,24)
REGDEF_END(PLL_PLL5_CR2_REG)

//
//0x13DC PLL10 (For sn mclk) Clock Rate Register 0
//
#define PLL_PLL10_CR0_REG_OFS            0x13DC
REGDEF_BEGIN(PLL_PLL10_CR0_REG)
    REGDEF_BIT(PLL_RATIO0       ,8)
    REGDEF_BIT(reserved1        ,24)
REGDEF_END(PLL_PLL10_CR0_REG)

//
//0x13E0 PLL10 (For sn mclk) Clock Rate Register 1
//
#define PLL_PLL10_CR1_REG_OFS            0x13E0
REGDEF_BEGIN(PLL_PLL10_CR1_REG)
    REGDEF_BIT(PLL_RATIO1       ,8)
    REGDEF_BIT(reserved1        ,24)
REGDEF_END(PLL_PLL10_CR1_REG)

//
//0x13DC PLL10 (For sn mclk) Clock Rate Register 2
//
#define PLL_PLL10_CR2_REG_OFS            0x13E4
REGDEF_BEGIN(PLL_PLL10_CR2_REG)
    REGDEF_BIT(PLL_RATIO2       ,8)
    REGDEF_BIT(reserved1        ,24)
REGDEF_END(PLL_PLL10_CR2_REG)


//
//0x11C8 PLL4 (For SSPLL) Spread Spectrum Register 0
//
#define PLL_PLL4_SSPLL0_REG_OFS         0x11C8
REGDEF_BEGIN(PLL_PLL4_SSPLL0_REG)
    REGDEF_BIT(SSC_RST          ,1)
    REGDEF_BIT(SSC_NEW_MODE     ,1)                 // 0: legacy mode, 1: NT96650 new mode
    REGDEF_BIT(SSC_STEP_SEL     ,2)
    REGDEF_BIT(                 ,2)
    REGDEF_BIT(DSSC             ,1)                 // 0: center spread, 1: down spread
    REGDEF_BIT(reserved1        ,25)
REGDEF_END(PLL_PLL4_SSPLL0_REG)

//
//0x11CC PLL4 (For SSPLL) Spread Spectrum Register 1
//
#define PLL_PLL4_SSPLL1_REG_OFS         0x11CC
REGDEF_BEGIN(PLL_PLL4_SSPLL1_REG)
    REGDEF_BIT(SSC_EN           ,1)                 // 0: disable, 1: enable
    REGDEF_BIT(                 ,4)
    REGDEF_BIT(                 ,3)
    REGDEF_BIT(reserved1        ,24)
REGDEF_END(PLL_PLL4_SSPLL1_REG)

//
//0x11D0 PLL4 (For SSPLL) Spread Spectrum Register 2
//
#define PLL_PLL4_SSPLL2_REG_OFS         0x11D0
REGDEF_BEGIN(PLL_PLL4_SSPLL2_REG)
    REGDEF_BIT(SSC_MULFAC       ,8)
    REGDEF_BIT(reserved1        ,24)
REGDEF_END(PLL_PLL4_SSPLL2_REG)

//
//0x11D4 PLL4 (For SSPLL) Spread Spectrum Register 3
//
#define PLL_PLL4_SSPLL3_REG_OFS         0x11D4
REGDEF_BEGIN(PLL_PLL4_SSPLL3_REG)
    REGDEF_BIT(SSC_PERIOD_VALUE ,8)
    REGDEF_BIT(reserved1        ,24)
REGDEF_END(PLL_PLL4_SSPLL3_REG)


//
//0x12A0 PLL2 (For SSPLL2) Spread Spectrum Register 0
//
#define PLL_PLL2_SSPLL0_REG_OFS         0x12A0
REGDEF_BEGIN(PLL_PLL2_SSPLL0_REG)
    REGDEF_BIT(SSC_RST          ,1)
    REGDEF_BIT(SSC_NEW_MODE     ,1)                 // 0: legacy mode, 1: NT96650 new mode
    REGDEF_BIT(SSC_STEP_SEL     ,2)
    REGDEF_BIT(                 ,2)
    REGDEF_BIT(DSSC             ,1)                 // 0: center spread, 1: down spread
    REGDEF_BIT(reserved1        ,25)
REGDEF_END(PLL_PLL2_SSPLL0_REG)

//
//0x1294 PLL2 (For SSPLL2) Spread Spectrum Register 1
//
#define PLL_PLL2_SSPLL1_REG_OFS         0x1294
REGDEF_BEGIN(PLL_PLL2_SSPLL1_REG)
    REGDEF_BIT(SSC_EN           ,1)                 // 0: disable, 1: enable
    REGDEF_BIT(                 ,4)
    REGDEF_BIT(                 ,3)
    REGDEF_BIT(reserved1        ,24)
REGDEF_END(PLL_PLL2_SSPLL1_REG)

//
//0x1298 PLL2 (For SSPLL2) Spread Spectrum Register 2
//
#define PLL_PLL2_SSPLL2_REG_OFS         0x1298
REGDEF_BEGIN(PLL_PLL2_SSPLL2_REG)
    REGDEF_BIT(SSC_MULFAC       ,8)
    REGDEF_BIT(reserved1        ,24)
REGDEF_END(PLL_PLL2_SSPLL2_REG)

//
//0x129C PLL2 (For SSPLL2) Spread Spectrum Register 3
//
#define PLL_PLL2_SSPLL3_REG_OFS         0x129C
REGDEF_BEGIN(PLL_PLL2_SSPLL3_REG)
    REGDEF_BIT(SSC_PERIOD_VALUE ,8)
    REGDEF_BIT(reserved1        ,24)
REGDEF_END(PLL_PLL2_SSPLL3_REG)


//
//0x1204 PLL12 (For ETH) Spread Spectrum Register 0
//
#define PLL_PLL12_SSPLL0_REG_OFS        0x1204
REGDEF_BEGIN(PLL_PLL12_SSPLL0_REG)
    REGDEF_BIT(SSC_RST          ,1)
    REGDEF_BIT(SSC_NEW_MODE     ,1)                 // 0: legacy mode, 1: NT96650 new mode
    REGDEF_BIT(SSC_STEP_SEL     ,2)
    REGDEF_BIT(                 ,2)
    REGDEF_BIT(DSSC             ,1)                 // 0: center spread, 1: down spread
    REGDEF_BIT(reserved1        ,25)
REGDEF_END(PLL_PLL12_SSPLL0_REG)

//
//0x1214 PLL12 (For ETH)  Spread Spectrum Register 1
//
#define PLL_PLL12_SSPLL1_REG_OFS        0x1214
REGDEF_BEGIN(PLL_PLL12_SSPLL1_REG)
    REGDEF_BIT(SSC_EN           ,1)                 // 0: disable, 1: enable
    REGDEF_BIT(                 ,4)
    REGDEF_BIT(                 ,3)
    REGDEF_BIT(reserved1        ,24)
REGDEF_END(PLL_PLL12_SSPLL1_REG)

//
//0x1218 PLL12 (For ETH)  Spread Spectrum Register 2
//
#define PLL_PLL12_SSPLL2_REG_OFS        0x1218
REGDEF_BEGIN(PLL_PLL12_SSPLL2_REG)
    REGDEF_BIT(SSC_MULFAC       ,8)
    REGDEF_BIT(reserved1        ,24)
REGDEF_END(PLL_PLL12_SSPLL2_REG)

//
//0x121C PLL12 (For ETH)  Spread Spectrum Register 3
//
#define PLL_PLL12_SSPLL3_REG_OFS         0x121C
REGDEF_BEGIN(PLL_PLL12_SSPLL3_REG)
    REGDEF_BIT(SSC_PERIOD_VALUE ,8)
    REGDEF_BIT(reserved1        ,24)
REGDEF_END(PLL_PLL12_SSPLL3_REG)



#endif
