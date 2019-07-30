/*
    Base IO address for NT96680

    Base IO address for NT96680.

    @file       IOAddress.h
    @ingroup
    @note       Nothing.

    Copyright   Novatek Microelectronics Corp. 2016.  All rights reserved.
*/

#ifndef _IOADDRESS_H
#define _IOADDRESS_H

/*
    @addtogroup
*/
//@{

#define IOADDR_GLOBAL_BASE          (0xF0000000)

// ---------------------------------------------
// System category 0
// ---------------------------------------------

// SDRAM Controller
#define IOADDR_DRAM_REG_BASE        (IOADDR_GLOBAL_BASE+0x00000000)

// Top
#define IOADDR_TOP_REG_BASE         (IOADDR_GLOBAL_BASE+0x00010000)

// CG
#define IOADDR_CG_REG_BASE          (IOADDR_GLOBAL_BASE+0x00020000)

// PAD
#define IOADDR_PAD_REG_BASE         (IOADDR_GLOBAL_BASE+0x00030000)

// Timer
#define IOADDR_TIMER_REG_BASE       (IOADDR_GLOBAL_BASE+0x00040000)

// WDT (Watch Dog Timer)
#define IOADDR_WDT_REG_BASE         (IOADDR_GLOBAL_BASE+0x00050000)

// RTC & Power Button
#define IOADDR_RTC_REG_BASE         (IOADDR_GLOBAL_BASE+0x00060000)

// GPIO
#define IOADDR_GPIO_REG_BASE        (IOADDR_GLOBAL_BASE+0x00070000)

// Interrupt
// **** Note ****
// Must check INT_CTRL_ADDR in \Common\Common_src\Kernel\IntHnder.s
#define IOADDR_INTERRUPT_REG_BASE   (IOADDR_GLOBAL_BASE+0x00080000)

// GIC Interrupt
#define IOADDR_GIC_REG_BASE         (IOADDR_GLOBAL_BASE+0x01500000)

// CC
#define IOADDR_CC_REG_BASE          (IOADDR_GLOBAL_BASE+0x00090000)

// PMC
#define IOADDR_PMC_REG_BASE         (IOADDR_GLOBAL_BASE+0x000A0000)

// ---------------------------------------------
// System category 1
// ---------------------------------------------
// SDRAM Controller 2
#define IOADDR_DRAM2_REG_BASE       (IOADDR_GLOBAL_BASE+0x00100000)

// DRTC
#define IOADDR_DRTC_REG_BASE        (IOADDR_GLOBAL_BASE+0x00110000)

// ---------------------------------------------
// IO category 0
// ---------------------------------------------

// BMC
#define IOADDR_BMC_REG_BASE         (IOADDR_GLOBAL_BASE+0x00200000)

// PWM
#define IOADDR_PWM_REG_BASE         (IOADDR_GLOBAL_BASE+0x00210000)

// I2C
#define IOADDR_I2C_REG_BASE         (IOADDR_GLOBAL_BASE+0x00220000)

// SPI
#define IOADDR_SPI_REG_BASE         (IOADDR_GLOBAL_BASE+0x00230000)

// SIF
#define IOADDR_SIF_REG_BASE         (IOADDR_GLOBAL_BASE+0x00240000)

// Remote
#define IOADDR_REMOTE_REG_BASE      (IOADDR_GLOBAL_BASE+0x00250000)

// ADC
#define IOADDR_ADC_REG_BASE         (IOADDR_GLOBAL_BASE+0x00260000)

// LVDS / HiSPi
#define IOADDR_LVDS_REG_BASE        (IOADDR_GLOBAL_BASE+0x00270000)

// MIPI - CSI
#define IOADDR_CSI_REG_BASE         (IOADDR_GLOBAL_BASE+0x00280000)

// UART
#define IOADDR_UART_REG_BASE        (IOADDR_GLOBAL_BASE+0x00290000)

// Vx1
#define IOADDR_VX1_REG_BASE         (IOADDR_GLOBAL_BASE+0x002A0000)

// Ethernet
#define IOADDR_ETH_REG_BASE         (IOADDR_GLOBAL_BASE+0x002B0000)

// SLVS-EC
#define IOADDR_SLVSEC_REG_BASE      (IOADDR_GLOBAL_BASE+0x002C0000)

// CAN-BUS
#define IOADDR_CANBUS_REG_BASE      (IOADDR_GLOBAL_BASE+0x002D0000)

// ---------------------------------------------
// IO category 1
// ---------------------------------------------

// UART2
#define IOADDR_UART2_REG_BASE       (IOADDR_GLOBAL_BASE+0x00300000)

// UART3
#define IOADDR_UART3_REG_BASE       (IOADDR_GLOBAL_BASE+0x00310000)

// SPI2
#define IOADDR_SPI2_REG_BASE        (IOADDR_GLOBAL_BASE+0x00320000)

// MIPI - CSI2
#define IOADDR_CSI2_REG_BASE        (IOADDR_GLOBAL_BASE+0x00330000)

// SPI3
#define IOADDR_SPI3_REG_BASE        (IOADDR_GLOBAL_BASE+0x00340000)

// I2C2
#define IOADDR_I2C2_REG_BASE        (IOADDR_GLOBAL_BASE+0x00350000)

// SPI4
#define IOADDR_SPI4_REG_BASE        (IOADDR_GLOBAL_BASE+0x00360000)

// LVDS2
#define IOADDR_LVDS2_REG_BASE       (IOADDR_GLOBAL_BASE+0x00370000)

// UART4
#define IOADDR_UART4_REG_BASE       (IOADDR_GLOBAL_BASE+0x00380000)

// SPI5
#define IOADDR_SPI5_REG_BASE        (IOADDR_GLOBAL_BASE+0x00390000)

// I2C3
#define IOADDR_I2C3_REG_BASE        (IOADDR_GLOBAL_BASE+0x003A0000)

// I2C4
#define IOADDR_I2C4_REG_BASE        (IOADDR_GLOBAL_BASE+0x003B0000)

// I2C5
#define IOADDR_I2C5_REG_BASE        (IOADDR_GLOBAL_BASE+0x003C0000)

// I2C6
#define IOADDR_I2C6_REG_BASE        (IOADDR_GLOBAL_BASE+0x003D0000)

// I2C7
#define IOADDR_I2C7_REG_BASE        (IOADDR_GLOBAL_BASE+0x003E0000)

// Vx1_2
#define IOADDR_VX12_REG_BASE        (IOADDR_GLOBAL_BASE+0x003F0000)


// ---------------------------------------------
// IO category 2
// ---------------------------------------------

// LVDS3
#define IOADDR_LVDS3_REG_BASE       (IOADDR_GLOBAL_BASE+0x00E00000)

// LVDS4
#define IOADDR_LVDS4_REG_BASE       (IOADDR_GLOBAL_BASE+0x00E10000)

// LVDS5
#define IOADDR_LVDS5_REG_BASE       (IOADDR_GLOBAL_BASE+0x00E20000)

// LVDS6
#define IOADDR_LVDS6_REG_BASE       (IOADDR_GLOBAL_BASE+0x00E30000)

// LVDS7
#define IOADDR_LVDS7_REG_BASE       (IOADDR_GLOBAL_BASE+0x00E40000)

// LVDS8
#define IOADDR_LVDS8_REG_BASE       (IOADDR_GLOBAL_BASE+0x00E50000)

// MIPI - CSI3
#define IOADDR_CSI3_REG_BASE        (IOADDR_GLOBAL_BASE+0x00E60000)

// MIPI - CSI4
#define IOADDR_CSI4_REG_BASE        (IOADDR_GLOBAL_BASE+0x00E70000)

// MIPI - CSI5
#define IOADDR_CSI5_REG_BASE        (IOADDR_GLOBAL_BASE+0x00E80000)

// MIPI - CSI6
#define IOADDR_CSI6_REG_BASE        (IOADDR_GLOBAL_BASE+0x00E90000)

// MIPI - CSI7
#define IOADDR_CSI7_REG_BASE        (IOADDR_GLOBAL_BASE+0x00EA0000)

// MIPI - CSI8
#define IOADDR_CSI8_REG_BASE        (IOADDR_GLOBAL_BASE+0x00EB0000)

// ---------------------------------------------
// Storage category 0
// ---------------------------------------------

// NAND
#define IOADDR_NAND_REG_BASE        (IOADDR_GLOBAL_BASE+0x00400000)

// SDIO
#define IOADDR_SDIO_REG_BASE        (IOADDR_GLOBAL_BASE+0x00420000)

// ---------------------------------------------
// Storage category 1
// ---------------------------------------------

// SDIO2
#define IOADDR_SDIO2_REG_BASE       (IOADDR_GLOBAL_BASE+0x00500000)

// SDIO3
#define IOADDR_SDIO3_REG_BASE       (IOADDR_GLOBAL_BASE+0x00510000)


// ---------------------------------------------
// Misc category 0
// ---------------------------------------------

// USB
#define IOADDR_USB_REG_BASE         (IOADDR_GLOBAL_BASE+0x00600000)

// USB3
#define IOADDR_USB3_REG_BASE        (IOADDR_GLOBAL_BASE+0x01700000)

// Crypto
#define IOADDR_CRYPTO_REG_BASE      (IOADDR_GLOBAL_BASE+0x00620000)

// DAI
#define IOADDR_DAI_REG_BASE         (IOADDR_GLOBAL_BASE+0x00630000)

// EAC
#define IOADDR_EAC_REG_BASE         (IOADDR_GLOBAL_BASE+0x00640000)

// TSMux
#define IOADDR_TSMUX_REG_BASE       (IOADDR_GLOBAL_BASE+0x00650000)

// eFUSE
#define IOADDR_EFUSE_REG_BASE       (IOADDR_GLOBAL_BASE+0x00660000)

// USB3-CTRL
#define IOADDR_USB3CTL_REG_BASE     (IOADDR_GLOBAL_BASE+0x00670000)

// TSDEMux
#define IOADDR_TSDEMUX_REG_BASE     (IOADDR_GLOBAL_BASE+0x00680000)

// ---------------------------------------------
// Misc category 1
// ---------------------------------------------


// ---------------------------------------------
// Display category 0
// ---------------------------------------------

// IDE
#define IOADDR_IDE_REG_BASE         (IOADDR_GLOBAL_BASE+0x00800000)

// MI
#define IOADDR_MI_REG_BASE          (IOADDR_GLOBAL_BASE+0x00810000)

// HDMI
#define IOADDR_HDMI_REG_BASE        (IOADDR_GLOBAL_BASE+0x00820000)

// TV
#define IOADDR_TV_REG_BASE          (IOADDR_GLOBAL_BASE+0x00830000)

// MIPI-DSI
#define IOADDR_DSI_REG_BASE         (IOADDR_GLOBAL_BASE+0x00840000)

// ---------------------------------------------
// Display category 1
// ---------------------------------------------

// IDE2
#define IOADDR_IDE2_REG_BASE        (IOADDR_GLOBAL_BASE+0x00900000)

// ---------------------------------------------
// Codec category 0
// ---------------------------------------------

// JPEG
#define IOADDR_JPEG_REG_BASE        (IOADDR_GLOBAL_BASE+0x00A00000)

// H264
#define IOADDR_H264_REG_BASE        (IOADDR_GLOBAL_BASE+0x00A10000)

// ---------------------------------------------
// Codec category 1
// ---------------------------------------------


// ---------------------------------------------
// IPP category 0
// ---------------------------------------------

// SIE
#define IOADDR_SIE_REG_BASE         (IOADDR_GLOBAL_BASE+0x00C00000)

// COPY
#define IOADDR_COPY_REG_BASE        (IOADDR_GLOBAL_BASE+0x00C10000)

// DCE
#define IOADDR_DCE_REG_BASE         (IOADDR_GLOBAL_BASE+0x00C20000)

// IPE
#define IOADDR_IPE_REG_BASE         (IOADDR_GLOBAL_BASE+0x00C30000)

// IME
#define IOADDR_IME_REG_BASE         (IOADDR_GLOBAL_BASE+0x00C40000)

// DIS
#define IOADDR_DIS_REG_BASE         (IOADDR_GLOBAL_BASE+0x00C50000)

// FDE
#define IOADDR_FDE_REG_BASE         (IOADDR_GLOBAL_BASE+0x00C60000)

// IFE
#define IOADDR_IFE_REG_BASE         (IOADDR_GLOBAL_BASE+0x00C70000)

// Graphic
#define IOADDR_GRAPHIC_REG_BASE     (IOADDR_GLOBAL_BASE+0x00C80000)

// ISE
#define IOADDR_ISE_REG_BASE         (IOADDR_GLOBAL_BASE+0x00C90000)

// Affine
#define IOADDR_AFFINE_REG_BASE      (IOADDR_GLOBAL_BASE+0x00CA0000)

// CNN
#define IOADDR_CNN_REG_BASE         (IOADDR_GLOBAL_BASE+0x00CB0000)

// TGE
#define IOADDR_TGE_REG_BASE         (IOADDR_GLOBAL_BASE+0x00CC0000)

// DRE
#define IOADDR_DRE_REG_BASE         (IOADDR_GLOBAL_BASE+0x00CD0000)

// RHE
#define IOADDR_RHE_REG_BASE         (IOADDR_GLOBAL_BASE+0x00CE0000)

// Rotation
#define IOADDR_ROT_REG_BASE         (IOADDR_GLOBAL_BASE+0x00CF0000)

// ---------------------------------------------
// IPP category 1
// ---------------------------------------------

// IFE2
#define IOADDR_IFE2_REG_BASE        (IOADDR_GLOBAL_BASE+0x00D00000)

// Graphic2
#define IOADDR_GRAPHIC2_REG_BASE    (IOADDR_GLOBAL_BASE+0x00D10000)

// SIE2
#define IOADDR_SIE2_REG_BASE        (IOADDR_GLOBAL_BASE+0x00D20000)

// SIE3
#define IOADDR_SIE3_REG_BASE        (IOADDR_GLOBAL_BASE+0x00D30000)

// SIE4
#define IOADDR_SIE4_REG_BASE        (IOADDR_GLOBAL_BASE+0x00D40000)

// SIE5
#define IOADDR_SIE5_REG_BASE        (IOADDR_GLOBAL_BASE+0x00D50000)

// SIE6
#define IOADDR_SIE6_REG_BASE        (IOADDR_GLOBAL_BASE+0x00D60000)

// SIE7
#define IOADDR_SIE7_REG_BASE        (IOADDR_GLOBAL_BASE+0x00D70000)

// SIE8
#define IOADDR_SIE8_REG_BASE        (IOADDR_GLOBAL_BASE+0x00D80000)

// IVE
#define IOADDR_IVE_REG_BASE         (IOADDR_GLOBAL_BASE+0x00D90000)

// SVM
#define IOADDR_SVM_REG_BASE         (IOADDR_GLOBAL_BASE+0x00DA0000)

// SDE
#define IOADDR_SDE_REG_BASE         (IOADDR_GLOBAL_BASE+0x00DB0000)

// ---------------------------------------------
// DSP category 0
// ---------------------------------------------
// DSP
#define IOADDR_DSP_REG_BASE         (IOADDR_GLOBAL_BASE+0x01430000)

// DSP2
#define IOADDR_DSP2_REG_BASE        (IOADDR_GLOBAL_BASE+0x01440000)


//@}


// RDE - Obselete
#define IOADDR_RDE_REG_BASE         (IOADDR_GLOBAL_BASE+0x00FFFFFF)

#endif
