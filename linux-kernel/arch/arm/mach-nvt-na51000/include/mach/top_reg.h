/*
	Pinmux module internal header file

	Pinmux module internal header file

	@file		top_reg.h
	@ingroup
	@note		Nothing

	Copyright   Novatek Microelectronics Corp. 2016.  All rights reserved.
*/

#ifndef _PINMUX_INT_H
#define _PINMUX_INT_H

#include "rcw_macro.h"
#define ENUM_DUMMY4WORD(name)   E_##name = 0x10000000

//
//0x00 TOP Control Register 0
//
#define TOP_REG0_OFS                        0x00
union TOP_REG0 {
	uint32_t reg;
	struct {
	unsigned int BOOT_SRC:4;	// Boot Source Selection
	unsigned int reserved0:1;
	unsigned int EJTAG_SEL:1;	// EJTAG select
	unsigned int MPLL_CLKSELL:1;	// PLL1 clock output mux
	unsigned int EMMC_WIDTH:1;	// eMMC bus width
	unsigned int ETH_MODE_SEL:1;
	unsigned int DEBUG_MODE_SEL:1;
	unsigned int reserved1:20;
	} bit;
};

//
//0x04 TOP Control Register 1
//
#define TOP_REG1_OFS                        0x04
union TOP_REG1 {
	uint32_t reg;
	struct {
	unsigned int reserved0:12;
	unsigned int NAND_EXIST:1;       // NAND exist enable control
	unsigned int FSPI_EXIST:1;       // FSPI exist enable control
	unsigned int SDIO_EXIST:1;       // SDIO exist enable control
	unsigned int SDIO2_EXIST:1;      // SDIO2 exist enable control
	unsigned int EXTROM_EXIST:1;     // EXT ROM exist enable control
	unsigned int SDIO3_EXIST:1;      // SDIO3 exist enable control
	unsigned int reserved1:6;
	unsigned int SDIO3_MUX_SEL:1;    // SDIO3 pinmux pad select
	unsigned int reserved2:4;        // SPI pinmux pad select
	unsigned int NAND_CS_NUM:1;      // Select NAND CS Pin number
	unsigned int reserved3:1;
	unsigned int SDIO3_BUS_WIDTH:1;  // Select SDIO3 bus width
	} bit;
};

//
//0x08 TOP Control Register 2
//
#define TOP_REG2_OFS                        0x08
union TOP_REG2 {
	uint32_t reg;
	struct {
	unsigned int LCD_TYPE:4;         // Pinmux of LCD interface
	unsigned int PLCD_DATA_WIDTH:2;  // Parallel LCD data width
	unsigned int PLCD_DE:1;          // Pinmux of DE (for parallel LCD)
	unsigned int CCIR_DATA_WIDTH:1;  // CCIR data width
	unsigned int CCIR_HVLD_VVLD:1;   // CCIR VVLD, HVLD select
	unsigned int CCIR_FIELD:1;       // CCIR FIELD select
	unsigned int reserved0:2;
	unsigned int LCD2_TYPE:4;        // Pinmux of LCD2 interface
	unsigned int reserved1:1;
	unsigned int PLCD2_DE:1;         // Pinmux of LCD2 DE (for parallel LCD)
	unsigned int reserved2:2;
	unsigned int MEMIF_TYPE:2;       // MI display type
	unsigned int PMEM_SEL:1;         // Parallel MI pinmux pad select
	unsigned int SMEMF_DATA_WIDTH:1; // Serial Memory Interface data width
	unsigned int PMEMIF_DATA_WIDTH:2;// Parallel Memory Interface data width
	unsigned int TE_SEL:1;           // Memory Interface TE select
	unsigned int reserved3:3;
	unsigned int TV_TEST_CLK:1;      // TV test clock enable
	unsigned int reserved4:1;
	} bit;
};

//
//0x0c TOP Control Register 3
//
#define TOP_REG3_OFS                        0x0C
union TOP_REG3 {
	uint32_t reg;
	struct {
	unsigned int SENSOR:3;           // Pinmux of sensor interface
	unsigned int reserved0:5;
	unsigned int SENSOR3:3;          // Pinmux of sensor3 interface
	unsigned int reserved1:9;
	unsigned int SENSOR6:3;          // Pinmux of sensor6 interface
	unsigned int reserved2:1;
	unsigned int SENSOR7:3;          // Pinmux of sensor7 interface
	unsigned int reserved3:1;
	unsigned int SENSOR8:3;          // Pinmux of sensor8 interface
	unsigned int reserved4:1;
	} bit;
};

//
//0x10 TOP Control Register 4
//
#define TOP_REG4_OFS                        0x10
union TOP_REG4 {
	uint32_t reg;
	struct {
	unsigned int FLCTR:1;            // Pinmux of sensor FLCTR
	unsigned int reserved0:1;
	unsigned int STROBE_SEL:1;       // Pinmux of strobe
	unsigned int reserved1:1;
	unsigned int SHUTTER_SEL:1;      // Pinmux of shutter
	unsigned int reserved2:1;
	unsigned int MES_SH0_SEL:2;      // MES pinmux pad select (MES_SEL)
	unsigned int MES_SH1_SEL:2;      // MES2 pinmux pad select (MES2_SEL)
	unsigned int MES2_SH0_SEL:2;     // MES_2 pinmux pad select (MES_2_SEL)
	unsigned int MES2_SH1_SEL:2;     // MES_2_2 pinmux pad select(MES_2_2_SEL)
	unsigned int reserved3:2;
	unsigned int XVS_XHS_EN:1;       // XVS/XHS enable
	unsigned int XVS_XHS2_EN:1;      // XVS/XHS2 enable
	unsigned int XVS_XHS3_EN:1;      // XVS/XHS3 enable
	unsigned int XVS_XHS4_EN:1;      // XVS/XHS4 enable
	unsigned int XVS_XHS5_EN:1;      // XVS/XHS5 enable
	unsigned int XVS_XHS6_EN:1;      // XVS/XHS6 enable
	unsigned int XVS_XHS7_EN:1;      // XVS/XHS7 enable
	unsigned int XVS_XHS8_EN:1;      // XVS/XHS8 enable
	unsigned int XVS_XHS3_SEL:1;     // XVS/XHS3 pinmux seleciton
	unsigned int reserved4:3;
	unsigned int LOCKN:1;            // Pinmux of LOCKN
	unsigned int LOCKN2:1;           // Pinmux of LOCKN
	unsigned int reserved5:2;
	} bit;
};

//
//0x14 TOP Control Register 5
//
#define TOP_REG5_OFS                        0x14
union TOP_REG5 {
	uint32_t reg;
	struct {
	unsigned int SPI:2;              // SPI  pinmux select
	unsigned int SPI2:2;             // SPI2 pinmux select
	unsigned int SPI3:2;             // SPI3 pinmux select
	unsigned int SPI4:2;             // SPI4 pinmux select
	unsigned int SPI_DAT:1;          // SPI  data select
	unsigned int SPI2_DAT:1;         // SPI2 data select
	unsigned int SPI3_DAT:1;         // SPI3 data select
	unsigned int SPI4_DAT:1;         // SPI4 data select
	unsigned int SPI3_RDY:2;         // SPI3_RDY seletion
	unsigned int reserved0:2;
	unsigned int SIFCH0:2;           // Pinmux of SIF channel 0
	unsigned int SIFCH1:2;           // Pinmux of SIF channel 1
	unsigned int SIFCH2:2;           // Pinmux of SIF channel 2
	unsigned int SIFCH3:2;           // Pinmux of SIF channel 3
	unsigned int SIFCH4:2;           // Pinmux of SIF channel 4
	unsigned int SIFCH5:2;           // Pinmux of SIF channel 5
	unsigned int SIFCH6:2;           // Pinmux of SIF channel 6
	unsigned int SIFCH7:2;           // Pinmux of SIF channel 7
	} bit;
};

//
//0x18 TOP Control Register 6
//
#define TOP_REG6_OFS                        0x18
union TOP_REG6 {
	uint32_t reg;
	struct {
	unsigned int AUDIO:2;            // Pinmux of external AUDIO interface
	unsigned int AUDIO_MCLK:2;       // AUDIO MCLK pinmux pad select
	unsigned int reserved0:4; 
	unsigned int SP_CLK:2;           // Pinmux of Special Clock
	unsigned int SP2_CLK:2;          // Pinmux of Special Clock 2
	unsigned int REMOTE:1;           // Pinmux of REMOTE
	unsigned int reserved1:3;
	unsigned int ETH:3;              // Pinmux of ETH
	unsigned int CANBUS:1;           // Pinmux of Canbus
	unsigned int reserved2:12;
	} bit;
};

//
//0x1C TOP Control Register 7
//
#define TOP_REG7_OFS                        0x1C
union TOP_REG7 {
	uint32_t reg;
	struct {
	unsigned int PWM0:1;             // Pinmux of PWM0
	unsigned int PWM1:1;             // Pinmux of PWM1
	unsigned int PWM2:1;             // Pinmux of PWM2
	unsigned int PWM3:1;             // Pinmux of PWM3
	unsigned int PWM4:1;             // Pinmux of PWM4
	unsigned int PWM5:1;             // Pinmux of PWM5
	unsigned int PWM6:1;             // Pinmux of PWM6
	unsigned int PWM7:1;             // Pinmux of PWM7
	unsigned int PWM8:1;             // Pinmux of PWM8
	unsigned int PWM9:1;             // Pinmux of PWM9
	unsigned int PWM10:1;            // Pinmux of PWM10
	unsigned int PWM11:1;            // Pinmux of PWM11
	unsigned int PWM12:1;            // Pinmux of PWM12
	unsigned int PWM13:1;            // Pinmux of PWM13
	unsigned int PWM14:1;            // Pinmux of PWM14
	unsigned int PWM15:1;            // Pinmux of PWM15
	unsigned int PWM16:1;            // Pinmux of PWM16
	unsigned int PWM17:1;            // Pinmux of PWM17
	unsigned int PWM18:1;            // Pinmux of PWM18
	unsigned int PWM19:1;            // Pinmux of PWM19
	unsigned int reserved0:3;
	unsigned int PWM0_SEL:1;         // Pinmux of PWM0 pinmux
	unsigned int PI_CNT:2;           // Pinmux of PI_CNT
	unsigned int PI_CNT2:2;          // Pinmux of PI_CNT2
	unsigned int PI_CNT3:2;          // Pinmux of PI_CNT3
	unsigned int PI_CNT4:2;          // Pinmux of PI_CNT4
	} bit;
};

//
//0x20 TOP Control Register 8
//
#define TOP_REG8_OFS                        0x20
union TOP_REG8 {
	uint32_t reg;
	struct {
	unsigned int USB_VBUSI:1;        // VBUSI control
	unsigned int USB_ID:1;           // USB ID control
	unsigned int USB2_VBUSI:1;       // USB2 VBUSI control
	unsigned int USB2_ID:1;          // USB2 ID control
	unsigned int reserved0:28;
	} bit;
};

//
//0x24 TOP Control Register 9
//
#define TOP_REG9_OFS                        0x24
union TOP_REG9 {
	uint32_t reg;
	struct {
	unsigned int UART:1;             // Pinmux of UART
	unsigned int reserved0:1;
	unsigned int UART2:2;            // Pinmux of UART2
	unsigned int UART3:2;            // Pinmux of UART3
	unsigned int UART4:2;            // Pinmux of UART4
	unsigned int UART2_CTSRTS:1;     // Pinmux of UART2 CTS/RTS
	unsigned int UART3_CTSRTS:1;     // Pinmux of UART3 CTS/RTS
	unsigned int UART4_CTSRTS:1;     // Pinmux of UART4 CTS/RTS
	unsigned int reserved1:21;
	} bit;
};

//
//0x28 TOP Control Register 10
//
#define TOP_REG10_OFS                        0x28
union TOP_REG10 {
	uint32_t reg;
	struct {
	unsigned int I2C:1;              // Pinmux of I2C interface
	unsigned int I2C_2:1;            // Pinmux of I2C_2 interface
	unsigned int reserved0:2;
	unsigned int I2C2:1;             // Pinmux of I2C2 interface
	unsigned int I2C2_2:1;           // Pinmux of I2C2_2 interface
	unsigned int reserved1:2;
	unsigned int I2C3:1;             // Pinmux of I2C3 interface
	unsigned int I2C3_2:1;           // Pinmux of I2C3_2 interface
	unsigned int I2C3_3:1;           // Pinmux of I2C3_3 interface
	unsigned int reserved2:1;
	unsigned int I2C4:1;             // Pinmux of I2C4 interface
	unsigned int I2C4_2:1;           // Pinmux of I2C4_2 interface
	unsigned int I2C4_3:1;           // Pinmux of I2C4_3 interface
	unsigned int I2C4_4:1;           // Pinmux of I2C4_4 interface
	unsigned int I2C5:1;             // Pinmux of I2C5 interface
	unsigned int I2C5_2:1;           // Pinmux of I2C5_2 interface
	unsigned int reserved3:14;
	} bit;
};

//
//0x2C TOP Control Register 11
//
#define TOP_REG11_OFS                        0x2C
union TOP_REG11 {
	uint32_t reg;
	struct {
	unsigned int CPU_EJTAG_EN:1;     // Pinmux of EJTAG
	unsigned int DSP_EJTAG_EN:1;     // Pinmux of DSP EJTAG
	unsigned int reserved0:2;
	unsigned int DSP_EJTAG_SEL:1;    // Pinmux of DSP EJTAG select
	unsigned int DSP_EJTAG_RTCKSEL:1;// Pinmux of DSP EJTAG RTCK select
	unsigned int DSP_EJTAG_DC:2;     // Daisy-chain selection of DSP EJTAG
	unsigned int TRACE_EN:2;
	unsigned int reserved1:6;
	unsigned int DSP_PERI_EN:1;
	unsigned int DSP_IOP_EN:1;
	unsigned int DSP2_PERI_EN:1;
	unsigned int DSP2_IOP_EN:1;
	unsigned int reserved2:12;
	} bit;
};

//
//0x30 TOP Control Register 12
//
#define TOP_REG12_OFS                        0x30
union TOP_REG12 {
	uint32_t reg;
	struct {
	unsigned int RAM_SRC_SELECT:1;
	unsigned int ROM_SRC_SELECT:1;
	unsigned int reserved0:30;
	} bit;
};

//
// 0xA0 CGPIO Control Register
//
#define TOP_REGCGPIO0_OFS                    0xA0
union TOP_REGCGPIO0 {
	uint32_t reg;
	struct {
	unsigned int CGPIO_0:1;          // pinmux of CGPIO 0
	unsigned int CGPIO_1:1;          // pinmux of CGPIO 1
	unsigned int CGPIO_2:1;          // pinmux of CGPIO 2
	unsigned int CGPIO_3:1;          // pinmux of CGPIO 3
	unsigned int CGPIO_4:1;          // pinmux of CGPIO 4
	unsigned int CGPIO_5:1;          // pinmux of CGPIO 5
	unsigned int CGPIO_6:1;          // pinmux of CGPIO 6
	unsigned int CGPIO_7:1;          // pinmux of CGPIO 7
	unsigned int CGPIO_8:1;          // pinmux of CGPIO 8
	unsigned int CGPIO_9:1;          // pinmux of CGPIO 9
	unsigned int CGPIO_10:1;         // pinmux of CGPIO 10
	unsigned int CGPIO_11:1;         // pinmux of CGPIO 11
	unsigned int CGPIO_12:1;         // pinmux of CGPIO 12
	unsigned int CGPIO_13:1;         // pinmux of CGPIO 13
	unsigned int CGPIO_14:1;         // pinmux of CGPIO 14
	unsigned int CGPIO_15:1;         // pinmux of CGPIO 15
	unsigned int CGPIO_16:1;         // pinmux of CGPIO 16
	unsigned int CGPIO_17:1;         // pinmux of CGPIO 17
	unsigned int CGPIO_18:1;         // pinmux of CGPIO 18
	unsigned int CGPIO_19:1;         // pinmux of CGPIO 19
	unsigned int CGPIO_20:1;         // pinmux of CGPIO 20
	unsigned int CGPIO_21:1;         // pinmux of CGPIO 21
	unsigned int CGPIO_22:1;         // pinmux of CGPIO 22
	unsigned int CGPIO_23:1;         // pinmux of CGPIO 23
	unsigned int CGPIO_24:1;         // pinmux of CGPIO 24
	unsigned int CGPIO_25:1;         // pinmux of CGPIO 25
	unsigned int CGPIO_26:1;         // pinmux of CGPIO 26
	unsigned int CGPIO_27:1;         // pinmux of CGPIO 27
	unsigned int CGPIO_28:1;         // pinmux of CGPIO 28
	unsigned int CGPIO_29:1;         // pinmux of CGPIO 29
	unsigned int CGPIO_30:1;         // pinmux of CGPIO 30
	unsigned int CGPIO_31:1;         // pinmux of CGPIO 31
	} bit;
};

//
// 0xA4 CGPIO Control Register 1
//
#define TOP_REGCGPIO1_OFS                    0xA4
union TOP_REGCGPIO1 {
	uint32_t reg;
	struct {
	unsigned int CGPIO_32:1;         // pinmux of CGPIO 32
	unsigned int CGPIO_33:1;         // pinmux of CGPIO 33
	unsigned int reserved0:30;
	} bit;
};

//
// 0xA8 PGPIO Control Register
//
#define TOP_REGPGPIO0_OFS                    0xA8
union TOP_REGPGPIO0 {
	uint32_t reg;
	struct {
	unsigned int PGPIO_0:1;          // pinmux of PGPIO 0
	unsigned int PGPIO_1:1;          // pinmux of PGPIO 1
	unsigned int PGPIO_2:1;          // pinmux of PGPIO 2
	unsigned int PGPIO_3:1;          // pinmux of PGPIO 3
	unsigned int PGPIO_4:1;          // pinmux of PGPIO 4
	unsigned int PGPIO_5:1;          // pinmux of PGPIO 5
	unsigned int PGPIO_6:1;          // pinmux of PGPIO 6
	unsigned int PGPIO_7:1;          // pinmux of PGPIO 7
	unsigned int PGPIO_8:1;          // pinmux of PGPIO 8
	unsigned int PGPIO_9:1;          // pinmux of PGPIO 9
	unsigned int PGPIO_10:1;         // pinmux of PGPIO 10
	unsigned int PGPIO_11:1;         // pinmux of PGPIO 11
	unsigned int PGPIO_12:1;         // pinmux of PGPIO 12
	unsigned int PGPIO_13:1;         // pinmux of PGPIO 13
	unsigned int PGPIO_14:1;         // pinmux of PGPIO 14
	unsigned int PGPIO_15:1;         // pinmux of PGPIO 15
	unsigned int PGPIO_16:1;         // pinmux of PGPIO 16
	unsigned int PGPIO_17:1;         // pinmux of PGPIO 17
	unsigned int PGPIO_18:1;         // pinmux of PGPIO 18
	unsigned int PGPIO_19:1;         // pinmux of PGPIO 19
	unsigned int PGPIO_20:1;         // pinmux of PGPIO 20
	unsigned int PGPIO_21:1;         // pinmux of PGPIO 21
	unsigned int PGPIO_22:1;         // pinmux of PGPIO 22
	unsigned int PGPIO_23:1;         // pinmux of PGPIO 23
	unsigned int PGPIO_24:1;         // pinmux of PGPIO 24
	unsigned int PGPIO_25:1;         // pinmux of PGPIO 25
	unsigned int PGPIO_26:1;         // pinmux of PGPIO 26
	unsigned int PGPIO_27:1;         // pinmux of PGPIO 27
	unsigned int PGPIO_28:1;         // pinmux of PGPIO 28
	unsigned int PGPIO_29:1;         // pinmux of PGPIO 29
	unsigned int PGPIO_30:1;         // pinmux of PGPIO 30
	unsigned int PGPIO_31:1;         // pinmux of PGPIO 31
	} bit;
};

//
// 0xAC PGPIO Control Register
//
#define TOP_REGPGPIO1_OFS                    0xAC
union TOP_REGPGPIO1 {
	uint32_t reg;
	struct {
	unsigned int PGPIO_32:1;         // pinmux of PGPIO 32
	unsigned int PGPIO_33:1;         // pinmux of PGPIO 33
	unsigned int PGPIO_34:1;         // pinmux of PGPIO 34
	unsigned int PGPIO_35:1;         // pinmux of PGPIO 35
	unsigned int PGPIO_36:1;         // pinmux of PGPIO 36
	unsigned int PGPIO_37:1;         // pinmux of PGPIO 37
	unsigned int PGPIO_38:1;         // pinmux of PGPIO 38
	unsigned int PGPIO_39:1;         // pinmux of PGPIO 39
	unsigned int PGPIO_40:1;         // pinmux of PGPIO 40
	unsigned int PGPIO_41:1;         // pinmux of PGPIO 41
	unsigned int PGPIO_42:1;         // pinmux of PGPIO 42
	unsigned int PGPIO_43:1;         // pinmux of PGPIO 43
	unsigned int PGPIO_44:1;         // pinmux of PGPIO 44
	unsigned int PGPIO_45:1;         // pinmux of PGPIO 45
	unsigned int PGPIO_46:1;         // pinmux of PGPIO 46
	unsigned int PGPIO_47:1;         // pinmux of PGPIO 47
	unsigned int reserved0:16;
	} bit;
};

//
// 0xB0 SGPIO Control Register
//
#define TOP_REGSGPIO0_OFS                    0xB0
union TOP_REGSGPIO0 {
	uint32_t reg;
	struct {
	unsigned int SGPIO_0:1;          // pinmux of SGPIO 0
	unsigned int SGPIO_1:1;          // pinmux of SGPIO 1
	unsigned int SGPIO_2:1;          // pinmux of SGPIO 2
	unsigned int SGPIO_3:1;          // pinmux of SGPIO 3
	unsigned int SGPIO_4:1;          // pinmux of SGPIO 4
	unsigned int SGPIO_5:1;          // pinmux of SGPIO 5
	unsigned int SGPIO_6:1;          // pinmux of SGPIO 6
	unsigned int SGPIO_7:1;          // pinmux of SGPIO 7
	unsigned int SGPIO_8:1;          // pinmux of SGPIO 8
	unsigned int SGPIO_9:1;          // pinmux of SGPIO 9
	unsigned int SGPIO_10:1;         // pinmux of SGPIO 10
	unsigned int SGPIO_11:1;         // pinmux of SGPIO 11
	unsigned int reserved0:20;
	} bit;
};

//
// 0xB8 LGPIO Control Register
//
#define TOP_REGLGPIO0_OFS                    0xB8
union TOP_REGLGPIO0 {
	uint32_t reg;
	struct {
	unsigned int LGPIO_0:1;          // pinmux of LGPIO 0
	unsigned int LGPIO_1:1;          // pinmux of LGPIO 1
	unsigned int LGPIO_2:1;          // pinmux of LGPIO 2
	unsigned int LGPIO_3:1;          // pinmux of LGPIO 3
	unsigned int LGPIO_4:1;          // pinmux of LGPIO 4
	unsigned int LGPIO_5:1;          // pinmux of LGPIO 5
	unsigned int LGPIO_6:1;          // pinmux of LGPIO 6
	unsigned int LGPIO_7:1;          // pinmux of LGPIO 7
	unsigned int LGPIO_8:1;          // pinmux of LGPIO 8
	unsigned int LGPIO_9:1;          // pinmux of LGPIO 9
	unsigned int LGPIO_10:1;         // pinmux of LGPIO 10
	unsigned int LGPIO_11:1;         // pinmux of LGPIO 11
	unsigned int LGPIO_12:1;         // pinmux of LGPIO 12
	unsigned int LGPIO_13:1;         // pinmux of LGPIO 13
	unsigned int LGPIO_14:1;         // pinmux of LGPIO 14
	unsigned int LGPIO_15:1;         // pinmux of LGPIO 15
	unsigned int LGPIO_16:1;         // pinmux of LGPIO 16
	unsigned int LGPIO_17:1;         // pinmux of LGPIO 17
	unsigned int LGPIO_18:1;         // pinmux of LGPIO 18
	unsigned int LGPIO_19:1;         // pinmux of LGPIO 19
	unsigned int LGPIO_20:1;         // pinmux of LGPIO 20
	unsigned int LGPIO_21:1;         // pinmux of LGPIO 21
	unsigned int LGPIO_22:1;         // pinmux of LGPIO 22
	unsigned int LGPIO_23:1;         // pinmux of LGPIO 23
	unsigned int LGPIO_24:1;         // pinmux of LGPIO 24
	unsigned int LGPIO_25:1;         // pinmux of LGPIO 25
	unsigned int LGPIO_26:1;         // pinmux of LGPIO 26
	unsigned int LGPIO_27:1;         // pinmux of LGPIO 27
	unsigned int LGPIO_28:1;         // pinmux of LGPIO 28
	unsigned int LGPIO_29:1;         // pinmux of LGPIO 29
	unsigned int LGPIO_30:1;         // pinmux of LGPIO 30
	unsigned int LGPIO_31:1;         // pinmux of LGPIO 31
	} bit;
};

//
// 0xBC LGPIO Control Register
//
#define TOP_REGLGPIO1_OFS                    0xBC
union TOP_REGLGPIO1 {
	uint32_t reg;
	struct {
	unsigned int LGPIO_32:1;         // pinmux of SGPIO 0
	unsigned int reserved0:31;
	} bit;
};

//
// 0xD0 DGPIO Control Register
//
#define TOP_REGDGPIO0_OFS                    0xD0
union TOP_REGDGPIO0 {
	uint32_t reg;
	struct {
	unsigned int DGPIO_0:1;         // pinmux of DGPIO 0
	unsigned int DGPIO_1:1;         // pinmux of DGPIO 1
	unsigned int DGPIO_2:1;         // pinmux of DGPIO 2
	unsigned int DGPIO_3:1;         // pinmux of DGPIO 3
	unsigned int DGPIO_4:1;         // pinmux of DGPIO 4
	unsigned int DGPIO_5:1;         // pinmux of DGPIO 5
	unsigned int DGPIO_6:1;         // pinmux of DGPIO 6
	unsigned int DGPIO_7:1;         // pinmux of DGPIO 7
	unsigned int DGPIO_8:1;         // pinmux of DGPIO 8
	unsigned int DGPIO_9:1;         // pinmux of DGPIO 9
	unsigned int DGPIO_10:1;        // pinmux of DGPIO 10
	unsigned int DGPIO_11:1;        // pinmux of DGPIO 11
	unsigned int DGPIO_12:1;        // pinmux of DGPIO 11
	unsigned int reserved0:19;
	} bit;
};

//
// 0xD8 HSIGPI Control Register
//
#define TOP_REGHGPI0_OFS                    0xD8
union TOP_REGHGPI0 {
	uint32_t reg;
	struct {
	unsigned int HSIGPI_0:1;          // pinmux of HSIGPI 0
	unsigned int HSIGPI_1:1;          // pinmux of HSIGPI 1
	unsigned int HSIGPI_2:1;          // pinmux of HSIGPI 2
	unsigned int HSIGPI_3:1;          // pinmux of HSIGPI 3
	unsigned int HSIGPI_4:1;          // pinmux of HSIGPI 4
	unsigned int HSIGPI_5:1;          // pinmux of HSIGPI 5
	unsigned int HSIGPI_6:1;          // pinmux of HSIGPI 6
	unsigned int HSIGPI_7:1;          // pinmux of HSIGPI 7
	unsigned int HSIGPI_8:1;          // pinmux of HSIGPI 8
	unsigned int HSIGPI_9:1;          // pinmux of HSIGPI 9
	unsigned int HSIGPI_10:1;         // pinmux of HSIGPI 10
	unsigned int HSIGPI_11:1;         // pinmux of HSIGPI 11
	unsigned int HSIGPI_12:1;         // pinmux of HSIGPI 12
	unsigned int HSIGPI_13:1;         // pinmux of HSIGPI 13
	unsigned int HSIGPI_14:1;         // pinmux of HSIGPI 14
	unsigned int HSIGPI_15:1;         // pinmux of HSIGPI 15
	unsigned int HSIGPI_16:1;         // pinmux of HSIGPI 16
	unsigned int HSIGPI_17:1;         // pinmux of HSIGPI 17
	unsigned int HSIGPI_18:1;         // pinmux of HSIGPI 18
	unsigned int HSIGPI_19:1;         // pinmux of HSIGPI 19
	unsigned int HSIGPI_20:1;         // pinmux of HSIGPI 20
	unsigned int HSIGPI_21:1;         // pinmux of HSIGPI 21
	unsigned int HSIGPI_22:1;         // pinmux of HSIGPI 22
	unsigned int HSIGPI_23:1;         // pinmux of HSIGPI 23
	unsigned int HSIGPI_24:1;         // pinmux of HSIGPI 24
	unsigned int HSIGPI_25:1;         // pinmux of HSIGPI 25
	unsigned int HSIGPI_26:1;         // pinmux of HSIGPI 26
	unsigned int HSIGPI_27:1;         // pinmux of HSIGPI 27
	unsigned int HSIGPI_28:1;         // pinmux of HSIGPI 28
	unsigned int HSIGPI_29:1;         // pinmux of HSIGPI 29
	unsigned int HSIGPI_30:1;         // pinmux of HSIGPI 30
	unsigned int HSIGPI_31:1;         // pinmux of HSIGPI 31
	} bit;
};

//
//0xF0 Version Code Register
//
#define TOP_VERSION_REG_OFS                 0xF0
union TOP_VERSION_REG {
	uint32_t reg;
	struct {
	unsigned int reserved0:8;
	unsigned int ROM_CODE_VERSION:4; // ROM Code Version
	unsigned int ECO_VERSION:4;      // ECO Version
	unsigned int CHIP_ID:16;         // Chip ID
	} bit;
};

//
//0xF8 OCP bus debug port sel
//
#define TOP_REG_REG_OCP_DEBUG_OFS           0xF8
union TOP_REG_REG_OCP_DEBUG {
	uint32_t reg;
	struct {
	unsigned int OCP_DEBUG_SEL:4;  // ROM Code Version
	unsigned int reserved0:28;
	} bit;
};

//
//0xFC Debug Port Register
//
#define TOP_REG_DEBUG_OFS                   0xFC
union TOP_REG_DEBUG {
	uint32_t reg;
	struct {
	unsigned int DEBUG_SEL:7;  // Debug Port Selection
	unsigned int reserved0:1;
	unsigned int DEBUG_OUTSEL:2;  // Debug Port output select
	unsigned int reserved1:22;
	} bit;
};
#endif
