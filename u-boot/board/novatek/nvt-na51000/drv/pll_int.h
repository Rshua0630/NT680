/*
    PLL configuration module internal header

    PLL configuration module internal header file

    @file       pll_int.h
    @ingroup    mIDrvSys_CG
    @note       Nothing

    Copyright   Novatek Microelectronics Corp. 2012.  All rights reserved.
*/

#ifndef _PLL_INT_REG_H
#define _PLL_INT_REG_H

#include <asm/arch/IOAddress.h>

//
// Clock Rate bit definition
//
#define PLL_CLKSEL_CPU_MASK             (0x03 << PLL_CLKSEL_CPU)
#define PLL_CLKSEL_APB_MASK             (0x03 << PLL_CLKSEL_APB)
#define PLL_CLKSEL_CPU2_MASK            (0x03 << 0)
#define PLL_CLKSEL_DSP_MASK             (0x03 << PLL_CLKSEL_DSP)
#define PLL_CLKSEL_DSP2_MASK            (0x03 << PLL_CLKSEL_DSP2)
#define PLL_CLKSEL_DSP_IOP_MASK         (0x03 << PLL_CLKSEL_DSP_IOP)
#define PLL_CLKSEL_DSP_EDP_MASK         (0x03 << PLL_CLKSEL_DSP_EDP)
#define PLL_CLKSEL_DSP_EPP_MASK         (0x03 << PLL_CLKSEL_DSP_EPP)
#define PLL_CLKSEL_DSP_EDAP_MASK        (0x03 << PLL_CLKSEL_DSP_EDAP)
#define PLL_CLKSEL_DSP2_IOP_MASK        (0x03 << PLL_CLKSEL_DSP2_IOP)
#define PLL_CLKSEL_DSP2_EDP_MASK        (0x03 << PLL_CLKSEL_DSP2_EDP)
#define PLL_CLKSEL_DSP2_EPP_MASK        (0x03 << PLL_CLKSEL_DSP2_EPP)
#define PLL_CLKSEL_DSP2_EDAP_MASK       (0x03 << PLL_CLKSEL_DSP2_EDAP)


#define PLL_CLKSEL_SDE_MASK             (0x03 << (PLL_CLKSEL_SDE - PLL_CLKSEL_R1_OFFSET))
#define PLL_CLKSEL_CNN_MASK             PLL_CLKSEL_SDE_MASK
#define PLL_CLKSEL_IPE_MASK             (0x03 << (PLL_CLKSEL_IPE - PLL_CLKSEL_R1_OFFSET))
#define PLL_CLKSEL_DIS_MASK             (0x03 << (PLL_CLKSEL_DIS - PLL_CLKSEL_R1_OFFSET))
#define PLL_CLKSEL_IME_MASK             (0x03 << (PLL_CLKSEL_IME - PLL_CLKSEL_R1_OFFSET))
#define PLL_CLKSEL_FDE_MASK             (0x03 << (PLL_CLKSEL_FDE - PLL_CLKSEL_R1_OFFSET))
#define PLL_CLKSEL_IVE_MASK             (0x03 << (PLL_CLKSEL_IVE - PLL_CLKSEL_R1_OFFSET))
#define PLL_CLKSEL_ISE_MASK             (0x03 << (PLL_CLKSEL_ISE - PLL_CLKSEL_R1_OFFSET))
#define PLL_CLKSEL_DCE_MASK             (0x03 << (PLL_CLKSEL_DCE - PLL_CLKSEL_R1_OFFSET))

#define PLL_CLKSEL_IFE_MASK             (0x03 << (PLL_CLKSEL_IFE - PLL_CLKSEL_R2_OFFSET))
#define PLL_CLKSEL_IFE2_MASK            (0x03 << (PLL_CLKSEL_IFE2 - PLL_CLKSEL_R2_OFFSET))
#define PLL_CLKSEL_SVM_MASK             (0x03 << (PLL_CLKSEL_SVM - PLL_CLKSEL_R2_OFFSET))
#define PLL_CLKSEL_RHE_MASK             (0x03 << (PLL_CLKSEL_RHE - PLL_CLKSEL_R2_OFFSET))
#define PLL_CLKSEL_ISE2_MASK            (0x03 << (PLL_CLKSEL_ISE2 - PLL_CLKSEL_R2_OFFSET))
#define PLL_CLKSEL_SIE_MCLKINV_MASK     (0x01 << (PLL_CLKSEL_SIE_MCLKINV - PLL_CLKSEL_R2_OFFSET))
#define PLL_CLKSEL_RDE_MASK             (0x03 << (PLL_CLKSEL_RDE - PLL_CLKSEL_R2_OFFSET))
#define PLL_CLKSEL_TGE_MASK             (0x01 << (PLL_CLKSEL_TGE - PLL_CLKSEL_R2_OFFSET))
#define PLL_CLKSEL_SIE2_MCLKINV_MASK    (0x01 << (PLL_CLKSEL_SIE2_MCLKINV - PLL_CLKSEL_R2_OFFSET))
#define PLL_CLKSEL_RHE_CLK2_MASK        (0x03 << (PLL_CLKSEL_RHE_CLK2 - PLL_CLKSEL_R2_OFFSET))
#define PLL_CLKSEL_DRE_MASK             (0x03 << (PLL_CLKSEL_DRE - PLL_CLKSEL_R2_OFFSET))
#define PLL_CLKSEL_TGE2_MASK            (0x03 << (PLL_CLKSEL_TGE2 - PLL_CLKSEL_R2_OFFSET))

#define PLL_CLKSEL_JPEG_MASK            (0x03 << (PLL_CLKSEL_JPEG - PLL_CLKSEL_R3_OFFSET))
#define PLL_CLKSEL_H26X_MASK            (0x03 << (PLL_CLKSEL_H26X - PLL_CLKSEL_R3_OFFSET))
#define PLL_CLKSEL_GRAPHIC_MASK         (0x03 << (PLL_CLKSEL_GRAPHIC - PLL_CLKSEL_R3_OFFSET))
#define PLL_CLKSEL_GRAPHIC2_MASK        (0x03 << (PLL_CLKSEL_GRAPHIC2 - PLL_CLKSEL_R3_OFFSET))
#define PLL_CLKSEL_AFFINE_MASK          (0x03 << (PLL_CLKSEL_AFFINE - PLL_CLKSEL_R3_OFFSET))
#define PLL_CLKSEL_HWCPY_MASK           (0x03 << (PLL_CLKSEL_HWCPY - PLL_CLKSEL_R3_OFFSET))
#define PLL_CLKSEL_ROTATE_MASK          (0x03 << (PLL_CLKSEL_ROTATE - PLL_CLKSEL_R3_OFFSET))
#define PLL_CLKSEL_CRYPTO_MASK          (0x01 << (PLL_CLKSEL_CRYPTO - PLL_CLKSEL_R3_OFFSET))
//Steven Add
#define PLL_CLKSEL_NAND_MASK            (0x3F << (PLL_CLKSEL_NAND_CLKDIV - PLL_CLKSEL_R12_OFFSET))
#define PLL_CLKSEL_SDIO_MASK            (0x03 << (PLL_CLKSEL_SDIO - PLL_CLKSEL_R4_OFFSET))
#define PLL_CLKSEL_SDIO2_MASK           (0x03 << (PLL_CLKSEL_SDIO2 - PLL_CLKSEL_R4_OFFSET))
#define PLL_CLKSEL_IDE_CLKSRC_MASK      (0x03 << (PLL_CLKSEL_IDE_CLKSRC - PLL_CLKSEL_R4_OFFSET))
#define PLL_CLKSEL_IDE2_CLKSRC_MASK     (0x03 << (PLL_CLKSEL_IDE2_CLKSRC - PLL_CLKSEL_R4_OFFSET))
#define PLL_CLKSEL_DSI_LPSRC_MASK       (0x01 << (PLL_CLKSEL_DSI_LPSRC - PLL_CLKSEL_R4_OFFSET))
#define PLL_CLKSEL_MI_CLKSRC_MASK       (0x03 << (PLL_CLKSEL_MI_CLKSRC - PLL_CLKSEL_R4_OFFSET))
#define PLL_CLKSEL_HDMI_ADO_CLKMUX_MASK (0x03 << (PLL_CLKSEL_HDMI_ADO_CLKMUX - PLL_CLKSEL_R4_OFFSET))
#define PLL_CLKSEL_ADO_CLKSEL_MASK      (0x01 << (PLL_CLKSEL_ADO_CLKSEL - PLL_CLKSEL_R4_OFFSET))
#define PLL_CLKSEL_HDMI_ADO_CLKSEL_MASK (0x01 << (PLL_CLKSEL_HDMI_ADO_CLKSEL - PLL_CLKSEL_R4_OFFSET))

#define PLL_CLKSEL_SDIO3_MASK           (0x03 << (PLL_CLKSEL_SDIO3 - PLL_CLKSEL_R5_OFFSET))
#define PLL_CLKSEL_TSMUX_MASK           (0x03 << (PLL_CLKSEL_TSMUX - PLL_CLKSEL_R5_OFFSET))
#define PLL_CLKSEL_SP_MASK              (0x03 << (PLL_CLKSEL_SP - PLL_CLKSEL_R5_OFFSET))
#define PLL_CLKSEL_SP2_MASK             (0x03 << (PLL_CLKSEL_SP2 - PLL_CLKSEL_R5_OFFSET))
#define PLL_CLKSEL_CANBUS_MASK          (0x03 << (PLL_CLKSEL_CANBUS - PLL_CLKSEL_R5_OFFSET))
#define PLL_CLKSEL_ETH_REFCLK_INV_MASK  (0x01 << (PLL_CLKSEL_ETH_REFCLK_INV - PLL_CLKSEL_R5_OFFSET))
#define PLL_CLKSEL_TSDEMUX_MASK         (0x03 << (PLL_CLKSEL_TSDEMUX - PLL_CLKSEL_R5_OFFSET))
#define PLL_CLKSEL_SLVSEC_D0CK_INV_MASK (0x01 << (PLL_CLKSEL_SLVSEC_D0CK_INV - PLL_CLKSEL_R5_OFFSET))
#define PLL_CLKSEL_SLVSEC_D1CK_INV_MASK (0x01 << (PLL_CLKSEL_SLVSEC_D1CK_INV - PLL_CLKSEL_R5_OFFSET))
#define PLL_CLKSEL_SLVSEC_D2CK_INV_MASK (0x01 << (PLL_CLKSEL_SLVSEC_D2CK_INV - PLL_CLKSEL_R5_OFFSET))
#define PLL_CLKSEL_SLVSEC_D3CK_INV_MASK (0x01 << (PLL_CLKSEL_SLVSEC_D3CK_INV - PLL_CLKSEL_R5_OFFSET))
#define PLL_CLKSEL_SLVSEC_D4CK_INV_MASK (0x01 << (PLL_CLKSEL_SLVSEC_D4CK_INV - PLL_CLKSEL_R5_OFFSET))
#define PLL_CLKSEL_SLVSEC_D5CK_INV_MASK (0x01 << (PLL_CLKSEL_SLVSEC_D5CK_INV - PLL_CLKSEL_R5_OFFSET))
#define PLL_CLKSEL_SLVSEC_D6CK_INV_MASK (0x01 << (PLL_CLKSEL_SLVSEC_D6CK_INV - PLL_CLKSEL_R5_OFFSET))
#define PLL_CLKSEL_SLVSEC_D7CK_INV_MASK (0x01 << (PLL_CLKSEL_SLVSEC_D7CK_INV - PLL_CLKSEL_R5_OFFSET))
//#define PLL_CLKSEL_VX1SL_MASK           (0x03 << (PLL_CLKSEL_VX1SL- PLL_CLKSEL_R3_OFFSET))

#define PLL_CLKSEL_MIPI_LVDS_MASK       (0x01 << (PLL_CLKSEL_MIPI_LVDS - PLL_CLKSEL_R6_OFFSET))
#define PLL_CLKSEL_MIPI_LVDS2_MASK      (0x01 << (PLL_CLKSEL_MIPI_LVDS2 - PLL_CLKSEL_R6_OFFSET))
#define PLL_CLKSEL_MIPI_LVDS3_MASK      (0x01 << (PLL_CLKSEL_MIPI_LVDS3 - PLL_CLKSEL_R6_OFFSET))
#define PLL_CLKSEL_MIPI_LVDS4_MASK      (0x01 << (PLL_CLKSEL_MIPI_LVDS4 - PLL_CLKSEL_R6_OFFSET))
#define PLL_CLKSEL_MIPI_LVDS5_MASK      (0x01 << (PLL_CLKSEL_MIPI_LVDS5 - PLL_CLKSEL_R6_OFFSET))
#define PLL_CLKSEL_MIPI_LVDS6_MASK      (0x01 << (PLL_CLKSEL_MIPI_LVDS6 - PLL_CLKSEL_R6_OFFSET))
#define PLL_CLKSEL_MIPI_LVDS7_MASK      (0x01 << (PLL_CLKSEL_MIPI_LVDS7 - PLL_CLKSEL_R6_OFFSET))
#define PLL_CLKSEL_MIPI_LVDS8_MASK      (0x01 << (PLL_CLKSEL_MIPI_LVDS8 - PLL_CLKSEL_R6_OFFSET))
#define PLL_CLKSEL_LVDS_CLKPHASE_MASK   (0x01 << (PLL_CLKSEL_LVDS_CLKPHASE - PLL_CLKSEL_R6_OFFSET))
#define PLL_CLKSEL_LVDS2_CLKPHASE_MASK  (0x01 << (PLL_CLKSEL_LVDS2_CLKPHASE - PLL_CLKSEL_R6_OFFSET))
#define PLL_CLKSEL_LVDS3_CLKPHASE_MASK  (0x01 << (PLL_CLKSEL_LVDS3_CLKPHASE - PLL_CLKSEL_R6_OFFSET))
#define PLL_CLKSEL_LVDS4_CLKPHASE_MASK  (0x01 << (PLL_CLKSEL_LVDS4_CLKPHASE - PLL_CLKSEL_R6_OFFSET))
#define PLL_CLKSEL_LVDS5_CLKPHASE_MASK  (0x01 << (PLL_CLKSEL_LVDS5_CLKPHASE - PLL_CLKSEL_R6_OFFSET))
#define PLL_CLKSEL_LVDS6_CLKPHASE_MASK  (0x01 << (PLL_CLKSEL_LVDS6_CLKPHASE - PLL_CLKSEL_R6_OFFSET))
#define PLL_CLKSEL_LVDS7_CLKPHASE_MASK  (0x01 << (PLL_CLKSEL_LVDS7_CLKPHASE - PLL_CLKSEL_R6_OFFSET))
#define PLL_CLKSEL_LVDS8_CLKPHASE_MASK  (0x01 << (PLL_CLKSEL_LVDS8_CLKPHASE - PLL_CLKSEL_R6_OFFSET))
#define PLL_CLKSEL_MIPI_SLVSEC_MASK     (0x07 << (PLL_CLKSEL_SLVSEC - PLL_CLKSEL_R6_OFFSET))
#define PLL_CLKSEL_U3PCLK_INV_MASK      (0x01 << (PLL_CLKSEL_U3PCLK_INV - PLL_CLKSEL_R6_OFFSET))
#define PLL_CLKSEL_U3UCLK_INV_MASK      (0x01 << (PLL_CLKSEL_U3UCLK_INV - PLL_CLKSEL_R6_OFFSET))
#define PLL_CLKSEL_U2UCLK_INV_MASK      (0x01 << (PLL_CLKSEL_U2UCLK_INV - PLL_CLKSEL_R6_OFFSET))
#define PLL_CLKSEL_U3REFCLK_SEL_MASK    (0x01 << (PLL_CLKSEL_U3REFCLK_SEL - PLL_CLKSEL_R6_OFFSET))
#define PLL_CLKSEL_SIE_IO_PXCLK_MASK    (0x01 << (PLL_CLKSEL_SIE_IO_PXCLK - PLL_CLKSEL_R6_OFFSET))
#define PLL_CLKSEL_SIE2_IO_PXCLK_MASK   (0x01 << (PLL_CLKSEL_SIE2_IO_PXCLK - PLL_CLKSEL_R6_OFFSET))
#define PLL_CLKSEL_SIE4_IO_PXCLK_MASK   (0x01 << (PLL_CLKSEL_SIE4_IO_PXCLK - PLL_CLKSEL_R6_OFFSET))
#define PLL_CLKSEL_SIE5_IO_PXCLK_MASK   (0x01 << (PLL_CLKSEL_SIE5_IO_PXCLK - PLL_CLKSEL_R6_OFFSET))
#define PLL_CLKSEL_SIE7_IO_PXCLK_MASK   (0x01 << (PLL_CLKSEL_SIE7_IO_PXCLK - PLL_CLKSEL_R6_OFFSET))
#define PLL_CLKSEL_SIE8_IO_PXCLK_MASK   (0x01 << (PLL_CLKSEL_SIE8_IO_PXCLK - PLL_CLKSEL_R6_OFFSET))


#define PLL_CLKSEL_SIE_CLKSRC_MASK      (0x07 << (PLL_CLKSEL_SIE_CLKSRC - PLL_CLKSEL_R7_OFFSET))
#define PLL_CLKSEL_SIE2_CLKSRC_MASK     (0x07 << (PLL_CLKSEL_SIE2_CLKSRC - PLL_CLKSEL_R7_OFFSET))
#define PLL_CLKSEL_SIE3_CLKSRC_MASK     (0x07 << (PLL_CLKSEL_SIE3_CLKSRC - PLL_CLKSEL_R7_OFFSET))
#define PLL_CLKSEL_SIE4_CLKSRC_MASK     (0x07 << (PLL_CLKSEL_SIE4_CLKSRC - PLL_CLKSEL_R7_OFFSET))
#define PLL_CLKSEL_SIE_MCLKSRC_MASK     (0x01 << (PLL_CLKSEL_SIE_MCLKSRC - PLL_CLKSEL_R7_OFFSET))
#define PLL_CLKSEL_SIE_MCLK2SRC_MASK    (0x01 << (PLL_CLKSEL_SIE_MCLK2SRC - PLL_CLKSEL_R7_OFFSET))
#define PLL_CLKSEL_SIE_PXCLKSRC_MASK    (0x01 << (PLL_CLKSEL_SIE_PXCLKSRC - PLL_CLKSEL_R7_OFFSET))
#define PLL_CLKSEL_SIE2_PXCLKSRC_MASK   (0x01 << (PLL_CLKSEL_SIE2_PXCLKSRC - PLL_CLKSEL_R7_OFFSET))
#define PLL_CLKSEL_SIE3_PXCLKSRC_MASK   (0x03 << (PLL_CLKSEL_SIE3_PXCLKSRC - PLL_CLKSEL_R7_OFFSET))
#define PLL_CLKSEL_SIE4_PXCLKSRC_MASK   (0x03 << (PLL_CLKSEL_SIE4_PXCLKSRC - PLL_CLKSEL_R7_OFFSET))
#define PLL_CLKSEL_SIE5_PXCLKSRC_MASK   (0x03 << (PLL_CLKSEL_SIE5_PXCLKSRC - PLL_CLKSEL_R7_OFFSET))
#define PLL_CLKSEL_SIE6_PXCLKSRC_MASK   (0x03 << (PLL_CLKSEL_SIE6_PXCLKSRC - PLL_CLKSEL_R7_OFFSET))
#define PLL_CLKSEL_SIE7_PXCLKSRC_MASK   (0x01 << (PLL_CLKSEL_SIE7_PXCLKSRC - PLL_CLKSEL_R7_OFFSET))
#define PLL_CLKSEL_SIE8_PXCLKSRC_MASK   (0x01 << (PLL_CLKSEL_SIE8_PXCLKSRC - PLL_CLKSEL_R7_OFFSET))


#define PLL_CLKSEL_SIE_MCLKDIV_MASK     (0xFF << (PLL_CLKSEL_SIE_MCLKDIV - PLL_CLKSEL_R8_OFFSET))
#define PLL_CLKSEL_SIE_MCLK2DIV_MASK    (0xFF << (PLL_CLKSEL_SIE_MCLK2DIV - PLL_CLKSEL_R8_OFFSET))
#define PLL_CLKSEL_SIE_CLKDIV_MASK      (0xFF << (PLL_CLKSEL_SIE_CLKDIV - PLL_CLKSEL_R8_OFFSET))
#define PLL_CLKSEL_SIE2_CLKDIV_MASK     (0xFF << (PLL_CLKSEL_SIE2_CLKDIV - PLL_CLKSEL_R8_OFFSET))

#define PLL_CLKSEL_SIE3_CLKDIV_MASK     (0xFF << (PLL_CLKSEL_SIE3_CLKDIV - PLL_CLKSEL_R9_OFFSET))
#define PLL_CLKSEL_SIE4_CLKDIV_MASK     (0xFF << (PLL_CLKSEL_SIE4_CLKDIV - PLL_CLKSEL_R9_OFFSET))
#define PLL_CLKSEL_SIE5_CLKDIV_MASK     (0x0F << (PLL_CLKSEL_SIE5_CLKDIV - PLL_CLKSEL_R9_OFFSET))
#define PLL_CLKSEL_SIE6_CLKDIV_MASK     (0x0F << (PLL_CLKSEL_SIE6_CLKDIV - PLL_CLKSEL_R9_OFFSET))
#define PLL_CLKSEL_SIE7_CLKDIV_MASK     (0x0F << (PLL_CLKSEL_SIE7_CLKDIV - PLL_CLKSEL_R9_OFFSET))
#define PLL_CLKSEL_SIE8_CLKDIV_MASK     (0x0F << (PLL_CLKSEL_SIE8_CLKDIV - PLL_CLKSEL_R9_OFFSET))


//#define PLL_CLKSEL_DSI_CLKDIV_MASK      (0x03 << (PLL_CLKSEL_DSI_CLKDIV - PLL_C LKSEL_R9_OFFSET))
#define PLL_CLKSEL_MI_CLKDIV_MASK       (0xFF << (PLL_CLKSEL_MI_CLKDIV - PLL_CLKSEL_R10_OFFSET))

#define PLL_CLKSEL_SP_CLKDIV_MASK       (0xFF << (PLL_CLKSEL_SP_CLKDIV - PLL_CLKSEL_R11_OFFSET))
#define PLL_CLKSEL_CEC_CLKDIV_MASK      (0xFF << (PLL_CLKSEL_CEC_CLKDIV - PLL_CLKSEL_R11_OFFSET))
#define PLL_CLKSEL_ADO_CLKDIV_MASK      (0xFF << (PLL_CLKSEL_ADO_CLKDIV - PLL_CLKSEL_R11_OFFSET))
#define PLL_CLKSEL_HDMI_ADO_CLKDIV_MASK (0xFF << (PLL_CLKSEL_HDMI_ADO_CLKDIV - PLL_CLKSEL_R11_OFFSET))

#define PLL_CLKSEL_SDIO_CLKDIV_MASK     (0x7FF<< (PLL_CLKSEL_SDIO_CLKDIV - PLL_CLKSEL_R12_OFFSET))
#define PLL_CLKSEL_SDIO2_CLKDIV_MASK    (0x7FF<< (PLL_CLKSEL_SDIO2_CLKDIV - PLL_CLKSEL_R12_OFFSET))
#define PLL_CLKSEL_NAND_CLKDIV_MASK     (0x3F << (PLL_CLKSEL_NAND_CLKDIV - PLL_CLKSEL_R12_OFFSET))

#define PLL_CLKSEL_SDIO3_CLKDIV_MASK    (0x7FF<< (PLL_CLKSEL_SDIO3_CLKDIV - PLL_CLKSEL_R13_OFFSET))
#define PLL_CLKSEL_CLASSD_CLKDIV_MASK   (0xFF << (PLL_CLKSEL_CLASSD_CLKDIV - PLL_CLKSEL_R13_OFFSET))
#define PLL_CLKSEL_SP2_CLKDIV_MASK      (0xFF << (PLL_CLKSEL_SP2_CLKDIV - PLL_CLKSEL_R13_OFFSET))

#define PLL_CLKSEL_SPI_CLKDIV_MASK      (0x7FF << (PLL_CLKSEL_SPI_CLKDIV - PLL_CLKSEL_R14_OFFSET))
#define PLL_CLKSEL_SPI2_CLKDIV_MASK     (0x7FF << (PLL_CLKSEL_SPI2_CLKDIV - PLL_CLKSEL_R14_OFFSET))

#define PLL_CLKSEL_SPI3_CLKDIV_MASK     (0x7FF << (PLL_CLKSEL_SPI3_CLKDIV - PLL_CLKSEL_R15_OFFSET))
#define PLL_CLKSEL_SPI4_CLKDIV_MASK     (0x7FF << (PLL_CLKSEL_SPI4_CLKDIV - PLL_CLKSEL_R15_OFFSET))

//#define PLL_CLKSEL_SPI5_CLKDIV_MASK     (0x1FFF << (PLL_CLKSEL_SPI5_CLKDIV - PLL_CLKSEL_R16_OFFSET))


#define PLL_CLKSEL_IDE_CLKDIV_MASK            (0xFF << (PLL_CLKSEL_IDE_CLKDIV - PLL_CLKSEL_R23_OFFSET))
#define PLL_CLKSEL_IDE2_CLKDIV_MASK           (0xFF << (PLL_CLKSEL_IDE2_CLKDIV - PLL_CLKSEL_R23_OFFSET))
#define PLL_CLKSEL_IDE_OUTIF_CLKDIV_MASK      (0xFF << (PLL_CLKSEL_IDE_OUTIF_CLKDIV - PLL_CLKSEL_R23_OFFSET))
#define PLL_CLKSEL_IDE2_OUTIF_CLKDIV_MASK     (0xFF << (PLL_CLKSEL_IDE2_OUTIF_CLKDIV - PLL_CLKSEL_R23_OFFSET))

#define PLL_CLKSEL_CSILPD0_MASK         (0x3 << (PLL_CLKSEL_LPD0_LPCLK - PLL_CLKSEL_R24_OFFSET))
#define PLL_CLKSEL_CSILPD1_MASK         (0x3 << (PLL_CLKSEL_LPD1_LPCLK - PLL_CLKSEL_R24_OFFSET))
#define PLL_CLKSEL_CSILPD2_MASK         (0x3 << (PLL_CLKSEL_LPD2_LPCLK - PLL_CLKSEL_R24_OFFSET))
#define PLL_CLKSEL_CSILPD3_MASK         (0x3 << (PLL_CLKSEL_LPD3_LPCLK - PLL_CLKSEL_R24_OFFSET))
#define PLL_CLKSEL_CSILPD4_MASK         (0x3 << (PLL_CLKSEL_LPD4_LPCLK - PLL_CLKSEL_R24_OFFSET))
#define PLL_CLKSEL_CSILPD5_MASK         (0x3 << (PLL_CLKSEL_LPD5_LPCLK - PLL_CLKSEL_R24_OFFSET))
#define PLL_CLKSEL_CSILPD6_MASK         (0x3 << (PLL_CLKSEL_LPD6_LPCLK - PLL_CLKSEL_R24_OFFSET))
#define PLL_CLKSEL_CSILPD7_MASK         (0x3 << (PLL_CLKSEL_LPD7_LPCLK - PLL_CLKSEL_R24_OFFSET))


//
//  PLL Register access definition
//
#define     PLL_SETREG(ofs,value)       OUTW((IOADDR_CG_REG_BASE+(ofs)),(value))
#define     PLL_GETREG(ofs)             INW(IOADDR_CG_REG_BASE+(ofs))
/*
FPGA     |  Real chip
pll2*2   |   CLK480
pll1/2   |   CLK192     => PLL_FPGA_192SRC
pll1*2   |   PLL2
pll1*2   |   PLL4
pll1*2   |   PLL5
pll1     |   PLL6
pll3     |   PLL7
pll2*2   |   PLL8
pll1*2/5 |   PLL9
pll1*2/4 |   PLL10
pll1*2   |   PLL11
pll3*5   |   PLL12
pll1*2/3 |   PLL13
*/
#ifndef _FPGA_PLL_OSC_
#define _FPGA_PLL_OSC_  270000000
#endif
#define PLL_FPGA_PLL1SRC                (_FPGA_PLL_OSC_)  //where @ FPGA Y1
#define PLL_FPGA_192SRC                 (PLL_FPGA_PLL1SRC / 2)
#define PLL_FPGA_480SRC                 (24000000)
#define PLL_FPGA_PLL2RC                 (24000000)  //where @ FPGA(dual) Y12 or FPGA(single) Y23
#define PLL_FPGA_PLL3SRC                (20000000)
#define PLL_FPGA_PLL7SRC                (12288000)
#define PLL_FPGA_PLL12SRC               (25000000)


typedef struct
{
    UINT32          uiSrcEnabled;
    UINT32          uiSrcModule;
    UINT32          uiCheckItem;
    CHAR*           ModuleName;
}PLLCLKCHECK,*PPLLCLKCHECK;

#endif
