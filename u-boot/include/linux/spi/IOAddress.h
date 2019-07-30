/*
    Base IO address for NT96660

    Base IO address for NT96660.

    @file       IOAddress.h
    @ingroup
    @note       Nothing.

    Copyright   Novatek Microelectronics Corp. 2011.  All rights reserved.
*/

#ifndef _IOADDRESS_H
#define _IOADDRESS_H

/*
    @addtogroup
*/
//@{

// ---------------------------------------------
// System category 0
// ---------------------------------------------

// SDRAM Controller
#define IOADDR_DRAM_REG_BASE        (0xC0000000)

// Top
#define IOADDR_TOP_REG_BASE         (0xC0010000)

// CG
#define IOADDR_CG_REG_BASE          (0xC0020000)

// PAD
#define IOADDR_PAD_REG_BASE         (0xC0030000)

// Timer
#define IOADDR_TIMER_REG_BASE       (0xC0040000)

// WDT (Watch Dog Timer)
#define IOADDR_WDT_REG_BASE         (0xC0050000)

// RTC & Power Button
#define IOADDR_RTC_REG_BASE         (0xC0060000)

// GPIO
#define IOADDR_GPIO_REG_BASE        (0xC0070000)

// Interrupt
// **** Note ****
// Must check INT_CTRL_ADDR in \Common\Common_src\Kernel\IntHnder.s
#define IOADDR_INTERRUPT_REG_BASE   (0xC0080000)

// CC
#define IOADDR_CC_REG_BASE          (0xC0090000)

// PMC
#define IOADDR_PMC_REG_BASE         (0xC00A0000)

// ---------------------------------------------
// System category 1
// ---------------------------------------------


// ---------------------------------------------
// IO category 0
// ---------------------------------------------

// BMC
#define IOADDR_BMC_REG_BASE         (0xC0200000)

// PWM
#define IOADDR_PWM_REG_BASE         (0xC0210000)

// I2C
#define IOADDR_I2C_REG_BASE         (0xC0220000)

// SPI
#define IOADDR_SPI_REG_BASE         (0xC0230000)

// SIF
#define IOADDR_SIF_REG_BASE         (0xC0240000)

// Remote
#define IOADDR_REMOTE_REG_BASE      (0xC0250000)

// ADC
#define IOADDR_ADC_REG_BASE         (0xC0260000)

// LVDS / HiSPi
#define IOADDR_LVDS_REG_BASE        (0xC0270000)

// MIPI - CSI
#define IOADDR_CSI_REG_BASE         (0xC0280000)

// UART
#define IOADDR_UART_REG_BASE        (0xC0290000)

// Vx1
#define IOADDR_VX1_REG_BASE         (0xC02A0000)

// Ethernet
#define IOADDR_ETH_REG_BASE         (0xC02B0000)

// ---------------------------------------------
// IO category 1
// ---------------------------------------------

// UART2
#define IOADDR_UART2_REG_BASE       (0xC0300000)

// UART3
#define IOADDR_UART3_REG_BASE       (0xC0310000)

// SPI2
#define IOADDR_SPI2_REG_BASE        (0xC0320000)

// MIPI - CSI2
#define IOADDR_CSI2_REG_BASE        (0xC0330000)

// SPI3
#define IOADDR_SPI3_REG_BASE        (0xC0340000)

// I2C2
#define IOADDR_I2C2_REG_BASE        (0xC0350000)

// SPI4
#define IOADDR_SPI4_REG_BASE        (0xC0360000)

// LVDS2
#define IOADDR_LVDS2_REG_BASE       (0xC0370000)

// UART4
#define IOADDR_UART4_REG_BASE       (0xC0380000)

// SPI5
#define IOADDR_SPI5_REG_BASE        (0xC0390000)

// ---------------------------------------------
// Storage category 0
// ---------------------------------------------

// NAND
#define IOADDR_NAND_REG_BASE        (0xC0400000)

// SDIO
#define IOADDR_SDIO_REG_BASE        (0xC0420000)

// ---------------------------------------------
// Storage category 1
// ---------------------------------------------

// SDIO2
#define IOADDR_SDIO2_REG_BASE       (0xC0500000)

// SDIO3
#define IOADDR_SDIO3_REG_BASE       (0xC0510000)


// ---------------------------------------------
// Misc category 0
// ---------------------------------------------

// USB
#define IOADDR_USB_REG_BASE         (0xC0600000)

// Crypto
#define IOADDR_CRYPTO_REG_BASE      (0xC0620000)

// DAI
#define IOADDR_DAI_REG_BASE         (0xC0630000)

// EAC
#define IOADDR_EAC_REG_BASE         (0xC0640000)

// TSMux
#define IOADDR_TSMUX_REG_BASE       (0xC0650000)

// eFUSE
#define IOADDR_EFUSE_REG_BASE       (0xC0660000)

// ---------------------------------------------
// Misc category 1
// ---------------------------------------------


// ---------------------------------------------
// Display category 0
// ---------------------------------------------

// IDE
#define IOADDR_IDE_REG_BASE         (0xC0800000)

// MI
#define IOADDR_MI_REG_BASE          (0xC0810000)

// HDMI
#define IOADDR_HDMI_REG_BASE        (0xC0820000)

// TV
#define IOADDR_TV_REG_BASE          (0xC0830000)

// MIPI-DSI
#define IOADDR_DSI_REG_BASE         (0xC0840000)

// ---------------------------------------------
// Display category 1
// ---------------------------------------------

// IDE2
#define IOADDR_IDE2_REG_BASE        (0xC0900000)

// ---------------------------------------------
// Codec category 0
// ---------------------------------------------

// JPEG
#define IOADDR_JPEG_REG_BASE        (0xC0A00000)

// H264
#define IOADDR_H264_REG_BASE        (0xC0A10000)

// ---------------------------------------------
// Codec category 1
// ---------------------------------------------


// ---------------------------------------------
// IPP category 0
// ---------------------------------------------

// SIE
#define IOADDR_SIE_REG_BASE         (0xC0C00000)

// DCE
#define IOADDR_DCE_REG_BASE         (0xC0C20000)

// IPE
#define IOADDR_IPE_REG_BASE         (0xC0C30000)

// IME
#define IOADDR_IME_REG_BASE         (0xC0C40000)

// DIS
#define IOADDR_DIS_REG_BASE         (0xC0C50000)

// FDE
#define IOADDR_FDE_REG_BASE         (0xC0C60000)

// IFE
#define IOADDR_IFE_REG_BASE         (0xC0C70000)

// Graphic
#define IOADDR_GRAPHIC_REG_BASE     (0xC0C80000)

// ISE
#define IOADDR_ISE_REG_BASE         (0xC0C90000)

// Affine
#define IOADDR_AFFINE_REG_BASE      (0xC0CA0000)

// RDE
#define IOADDR_RDE_REG_BASE         (0xC0CB0000)

// RGE
#define IOADDR_TGE_REG_BASE         (0xC0CC0000)

// DRE
#define IOADDR_DRE_REG_BASE         (0xC0CD0000)

// ---------------------------------------------
// IPP category 1
// ---------------------------------------------

// IFE2
#define IOADDR_IFE2_REG_BASE        (0xC0D00000)

// Graphic2
#define IOADDR_GRAPHIC2_REG_BASE    (0xC0D10000)

// SIE2
#define IOADDR_SIE2_REG_BASE        (0xC0D20000)

// SIE3
#define IOADDR_SIE3_REG_BASE        (0xC0D30000)

// SIE4
#define IOADDR_SIE4_REG_BASE        (0xC0D40000)

//@}

#endif
