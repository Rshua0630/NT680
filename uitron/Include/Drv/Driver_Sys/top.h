/**
    TOP controller header

    Sets the pinmux of each module.

    @file       top.h
    @ingroup    mIDrvSys_TOP
    @note       Refer NA51000 data sheet for PIN/PAD naming

    Copyright   Novatek Microelectronics Corp. 2016.  All rights reserved.
*/

#ifndef _TOP_H
#define _TOP_H

#if defined(__UITRON)
#include "Type.h"
#else
#include "nvt_type.h"
#endif
//#define TOP_NEW_API                 (0)

/**
    @addtogroup mIDrvSys_TOP
*/
//@{
#define PIN_GROUP_CONFIG_VER 0x17011016

/**
    Debug port select ID

    Debug port select value for pinmux_select_debugport().
*/
typedef enum {
	PINMUX_DEBUGPORT_CKG =          0x0000,     ///< CKGen
	PINMUX_DEBUGPORT_ARB =          0x0001,     ///< Arbiter
	PINMUX_DEBUGPORT_ARB2 =          0x0002,    ///< Arbiter2
	PINMUX_DEBUGPORT_DDR =          0x0003,     ///< DDR
	PINMUX_DEBUGPORT_DDR2 =          0x0004,    ///< DDR2
	PINMUX_DEBUGPORT_IDE =          0x0005,     ///< IDE
	PINMUX_DEBUGPORT_IDE2 =         0x0006,     ///< IDE2
	PINMUX_DEBUGPORT_MI =           0x0007,     ///< MI
	PINMUX_DEBUGMIPI_DSI =          0x0008,     ///< DSI
	PINMUX_DEBUGMIPI_HWCPY =        0x0009,     ///< HwCpy
	PINMUX_DEBUGMIPI_TSDEMUX =      0x000A,     ///< TSDEMUX
	PINMUX_DEBUGMIPI_ISE2 =         0x000B,     ///< ISE2
	PINMUX_DEBUGPORT_TIMER =        0x0010,     ///< Timer
	PINMUX_DEBUGPORT_WDT =          0x0011,     ///< WDT
	PINMUX_DEBUGPORT_GPIO =         0x0012,     ///< GPIO
	PINMUX_DEBUGPORT_SSP =          0x0013,     ///< SSP (DAI)
	PINMUX_DEBUGPORT_AUDIO =        0x0014,     ///< AUDIO
	PINMUX_DEBUGPORT_SDIO1 =        0x0015,     ///< SDIO1
	PINMUX_DEBUGPORT_SDIO2 =        0x0016,     ///< SDIO2
	PINMUX_DEBUGPORT_SDIO3 =        0x0017,     ///< SDIO3
	PINMUX_DEBUGPORT_SMMC =         0x0018,     ///< XD/NAND/SMC
	PINMUX_DEBUGPORT_USB =          0x0019,     ///< USB2
	PINMUX_DEBUGPORT_USB2 =         0x001A,     ///< USB2/3 combo
	PINMUX_DEBUGPORT_EHTERNET =     0x001B,     ///< Ethernet

	PINMUX_DEBUGPORT_I2C =          0x001C,     ///< I2C
	PINMUX_DEBUGPORT_I2C2 =         0x001D,     ///< I2C2
	PINMUX_DEBUGPORT_I2C3 =         0x001E,     ///< I2C3
	PINMUX_DEBUGPORT_I2C4 =         0x001F,     ///< I2C4
	PINMUX_DEBUGPORT_I2C5 =         0x0020,     ///< I2C5
	PINMUX_DEBUGPORT_SIF =          0x0021,     ///< SIF

	PINMUX_DEBUGPORT_SPI =          0x0022,     ///< SPI
	PINMUX_DEBUGPORT_SPI2 =         0x0023,     ///< SPI2
	PINMUX_DEBUGPORT_SPI3 =         0x0024,     ///< SPI3
	PINMUX_DEBUGPORT_SPI4 =         0x0025,     ///< SPI4
	PINMUX_DEBUGPORT_PWM =          0x0026,     ///< PWM
	PINMUX_DEBUGPORT_ADM =          0x0027,     ///< ADC

	PINMUX_DEBUGPORT_REMOTE =       0x0028,     ///< Remote
	PINMUX_DEBUGPORT_CRYPTO =       0x0029,     ///< Crypto
	PINMUX_DEBUGPORT_INTC =         0x002A,     ///< INTC
	PINMUX_DEBUGPORT_CANBUS =       0x002B,     ///< CANBUS
	PINMUX_DEBUGPORT_CC =           0x002C,     ///< CC
	PINMUX_DEBUGPORT_UART2 =        0x002D,     ///< UART2
	PINMUX_DEBUGPORT_UART3 =        0x002E,     ///< UART3
	PINMUX_DEBUGPORT_UART4 =        0x002F,     ///< UART4


	PINMUX_DEBUGPORT_APB_BRG =      0x0030,     ///< APB BRG
	PINMUX_DEBUGPORT_EFUSE =        0x0031,     ///< EFUSE
	PINMUX_DEBUGPORT_DRTC =         0x0032,     ///< DRTC
	PINMUX_DEBUGPORT_CPU_L2 =       0x0033,     ///< CPU_L2
	PINMUX_DEBUGPORT_CPU_CS =       0x0034,     ///< CPU_CS
	PINMUX_DEBUGPORT_CPU_L2_P2 =    0x0035,     ///< CPU_L2_P2
	PINMUX_DEBUGPORT_CPU_CS_P2 =    0x0036,     ///< CPU_CS_P2
	PINMUX_DEBUGPORT_USB3_BRG =     0x0037,     ///< USB3_BRG
	PINMUX_DEBUGPORT_ETH_BRG =      0x0038,     ///< ETH_BRG
	PINMUX_DEBUGPORT_OSPREY =       0x0039,     ///< OSPREY

	PINMUX_DEBUGPORT_DSP =          0x003D,     ///< DSP
	PINMUX_DEBUGPORT_DSP2 =         0x003E,     ///< DSP2
	PINMUX_DEBUGPORT_HEVC =         0x003F,     ///< HEVC


	PINMUX_DEBUGMIPI_CSI =          0x0040,     ///< MIPI CSI
	PINMUX_DEBUGMIPI_CSI2 =         0x0041,     ///< MIPI CSI2
	PINMUX_DEBUGMIPI_CSI3 =         0x0042,     ///< MIPI CSI3
	PINMUX_DEBUGMIPI_CSI4 =         0x0043,     ///< MIPI CSI4
	PINMUX_DEBUGMIPI_CSI5 =         0x0044,     ///< MIPI CSI5
	PINMUX_DEBUGMIPI_CSI6 =         0x0045,     ///< MIPI CSI6
	PINMUX_DEBUGMIPI_CSI7 =         0x0046,     ///< MIPI CSI7
	PINMUX_DEBUGMIPI_CSI8 =         0x0047,     ///< MIPI CSI8

	PINMUX_DEBUGMIPI_SLVSEC =       0x0048,     ///< SLVS_EC


	PINMUX_DEBUGMIPI_VX1 =          0x0049,     ///< Vx1
	PINMUX_DEBUGMIPI_VX12 =         0x004A,     ///< Vx12
	PINMUX_DEBUGMIPI_SIE =          0x004B,     ///< SIE
	PINMUX_DEBUGMIPI_SIE2 =         0x004C,     ///< SIE2
	PINMUX_DEBUGMIPI_SIE3 =         0x004D,     ///< SIE3
	PINMUX_DEBUGMIPI_SIE4 =         0x004E,     ///< SIE4
	PINMUX_DEBUGMIPI_SIE5 =         0x004F,     ///< SIE5
	PINMUX_DEBUGMIPI_SIE6 =         0x0050,     ///< SIE6
	PINMUX_DEBUGMIPI_SIE7 =         0x0051,     ///< SIE7
	PINMUX_DEBUGMIPI_SIE8 =         0x0052,     ///< SIE8
	PINMUX_DEBUGMIPI_TGE =          0x0053,     ///< TGE
	PINMUX_DEBUGMIPI_DIS =          0x0054,     ///< DIS
	PINMUX_DEBUGMIPI_FD =           0x0055,     ///< FD
	PINMUX_DEBUGMIPI_JPEG =         0x0056,     ///< JPEG
	PINMUX_DEBUGMIPI_GRAPHIC =      0x0057,     ///< Graphic
	PINMUX_DEBUGMIPI_GRAPHIC2 =     0x0058,     ///< Graphic2
	PINMUX_DEBUGMIPI_AFFINE =       0x0059,     ///< Affine
	PINMUX_DEBUGMIPI_ISE =          0x005A,     ///< ISE
	PINMUX_DEBUGMIPI_RHE =          0x005B,     ///< RHE

	PINMUX_DEBUGPORT_GROUP_NONE =   0x0000,     ///< No debug port is output
	PINMUX_DEBUGPORT_GROUP1 =       0x0100,     ///< Output debug port to C_GPIO[18..0]
	PINMUX_DEBUGPORT_GROUP2 =       0x0200,     ///< Output debug port to L_GPIO[18..0]

	ENUM_DUMMY4WORD(PINMUX_DEBUGPORT)
} PINMUX_DEBUGPORT;

// for backward compatible
#define PINMUX_DEBUGMIPI_OCP   PINMUX_DEBUGPORT_OCPBRG


/**
    OCP Debug port select ID

    Debug port select value for pinmux_select_ocp_debugport().
*/
typedef enum {
	PINMUX_DEBUGPORT_OCPCMD_DMA_APB =   0x0,    ///< OCP CMD vs. DMA/APB
	PINMUX_DEBUGPORT_OCPCMD_HANDSHAKE = 0x1,    ///< OCP CMD handshaking
	PINMUX_DEBUGPORT_OCP_DMA =          0x2,    ///< OCP vs. DMA
	PINMUX_DEBUGPORT_OCP_DEBUG_3 =      0x3,    ///< OCP debug 3
	PINMUX_DEBUGPORT_OCP_DEBUG_4 =      0x4,    ///< OCP debug 4

	ENUM_DUMMY4WORD(PINMUX_DEBUGPORT_OCP)
} PINMUX_DEBUGPORT_OCP;

/**
    Function group

    @note For pinmux_init()
*/
typedef enum {
	PIN_FUNC_SDIO,      ///< SDIO. Configuration refers to PIN_SDIO_CFG.
	PIN_FUNC_SDIO2,     ///< SDIO2. Configuration refers to PIN_SDIO_CFG.
	PIN_FUNC_SDIO3,     ///< SDIO3. Configuration refers to PIN_SDIO_CFG.
	PIN_FUNC_NAND,      ///< NAND. Configuration refers to PIN_NAND_CFG.
	PIN_FUNC_SENSOR,    ///< sensor interface. Configuration refers to PIN_SENSOR_CFG.
	PIN_FUNC_SENSOR2,   ///< sensor2 interface. Configuration refers to PIN_SENSOR2_CFG.
	PIN_FUNC_SENSOR3,   ///< sensor3 interface. Configuration refers to PIN_SENSOR3_CFG.
	PIN_FUNC_SENSOR4,   ///< sensor4 interface. Configuration refers to PIN_SENSOR4_CFG.
	PIN_FUNC_SENSOR5,   ///< sensor5 interface. Configuration refers to PIN_SENSOR5_CFG.
	PIN_FUNC_SENSOR6,   ///< sensor6 interface. Configuration refers to PIN_SENSOR6_CFG.
	PIN_FUNC_SENSOR7,   ///< sensor7 interface. Configuration refers to PIN_SENSOR7_CFG.
	PIN_FUNC_SENSOR8,   ///< sensor8 interface. Configuration refers to PIN_SENSOR8_CFG.
	PIN_FUNC_MIPI_LVDS, ///< MIPI/LVDS interface configuration. Configuration refers to PIN_MIPI_LVDS_CFG.
	PIN_FUNC_I2C,       ///< I2C. Configuration refers to PIN_I2C_CFG.
	PIN_FUNC_SIF,       ///< SIF. Configuration refers to PIN_SIF_CFG.
	PIN_FUNC_UART,      ///< UART. Configuration refers to PIN_UART_CFG.
	PIN_FUNC_SPI,       ///< SPI. Configuration refers to PIN_SPI_CFG.
	PIN_FUNC_REMOTE,    ///< REMOTE. Configuration refers to PIN_REMOTE_CFG.
	PIN_FUNC_PWM,       ///< PWM. Configuration refers to PIN_PWM_CFG.
	PIN_FUNC_AUDIO,     ///< AUDIO. Configuration refers to PIN_PWM_CFG.
	PIN_FUNC_LCD,       ///< LCD interface. Configuration refers to PINMUX_LCDINIT, PINMUX_PMI_CFG, PINMUX_DISPMUX_SEL.
	PIN_FUNC_LCD2,      ///< LCD2 interface. Configuration refers to PINMUX_LCDINIT, PINMUX_PMI_CFG, PINMUX_DISPMUX_SEL.
	PIN_FUNC_TV,        ///< TV interface. Configuration refers to PINMUX_TV_HDMI_CFG.
	PIN_FUNC_HDMI,      ///< HDMI. Configuration refers to PINMUX_LCDINIT, PINMUX_TV_HDMI_CFG, PINMUX_HDMI_CFG.
	PIN_FUNC_ETH,       ///< ETH. Configuration refers to PINMUX_ETH_CFG.
	PIN_FUNC_CANBUS,    ///< CANBUS. Configutation refers to PINMUX_CANBUS_CFG.

	ENUM_DUMMY4WORD(PIN_FUNC)
} PIN_FUNC;

/**
    PIN config for SDIO

    @note For pinmux_init() with PIN_FUNC_SDIO or PIN_FUNC_SDIO2 or PIN_FUNC_SDIO3.\n
            For example, you can use {PIN_FUNC_SDIO, PIN_SDIO_CFG_1ST_PINMUX|PIN_SDIO_CFG_4BITS}\n
            to declare SDIO is 4 bits and located in 1st pinmux location.
*/
typedef enum {
	PIN_SDIO_CFG_NONE,
	PIN_SDIO_CFG_4BITS = 0x01,      ///< 4 bits wide.
	PIN_SDIO_CFG_8BITS = 0x02,      ///< 8 bits wide. (only SDIO3 2ND_PINMUX support up to 8 bits data)

	PIN_SDIO_CFG_1ST_PINMUX = 0x00, ///< 1st pinmux location
	///< For SDIO: enable SD_CLK/SD_CMD/SD_D[0..3] on MC[16..21] (C_GPIO[16..21])
	///< For SDIO2: enable SDIO_CLK/SDIO_CMD/SDIO_D[0..3] on MC[22..27] (C_GPIO[22..27])
	///< For SDIO3: enable SDIO_CLK/SDIO_CMD/SDIO_D[0..3] on MC[28..33] (C_GPIO[28..33])
	PIN_SDIO_CFG_2ND_PINMUX = 0x10, ///< 2nd pinmux location
	///< For SDIO: enable SD_CLK/SD_CMD/SD_D[0..3] on MC[16..21] (C_GPIO[16..21]) (SDIO only has one pinmux pad group)
	///< For SDIO2: enable SDIO_CLK/SDIO_CMD/SDIO_D[0..3] on MC[22..27] (C_GPIO[22..27]) (SDIO2 only has one pinmux pad group)
	///< For SDIO3: enable SDIO_CLK/SDIO_CMD/SDIO_D[0..3] on MC[0..3]/MC9/MC11 (C_GPIO[0..3], 9, 11)
	///<			When 8 bits, enable SDIO_D[0..7]/SDIO_CLK/SDIO_CMD on MC[0..7]/MC9/MC11 (C_GPIO[0..7], 9, 11)

	ENUM_DUMMY4WORD(PIN_SDIO_CFG)
} PIN_SDIO_CFG;

/**
    PIN config for NAND

    @note For pinmux_init() with PIN_FUNC_NAND.\n
            For example, you can use {PIN_FUNC_NAND, PIN_NAND_CFG_1CS} to declare NAND with 1 CS.
*/
typedef enum {
	PIN_NAND_CFG_NONE,
	PIN_NAND_CFG_1CS = 0x01,        ///< 1 chip select. Enable NAND_D[0..7]/NAND_CS0/NAND_WE/NAND_RE/NAND_CLE/NAND_ALE/NAND_WP/NAND_RDY on MC[0..8]/MC[10..15] (C_GPIO[0..8], C_GPIO[10..15])
	PIN_NAND_CFG_2CS = 0x02,        ///< 2 chip select. Enable NAND_D[0..7]/NAND_CS[0..1]/NAND_WE/NAND_RE/NAND_CLE/NAND_ALE/NAND_WP/NAND_RDY on MC[0..9]/MC[10..15] (C_GPIO[0..15])
	PIN_NAND_CFG_SPI_NAND = 0x4,    ///< Virtual enum for project layer configuration
	PIN_NAND_CFG_SPI_NOR = 0x8,     ///< Virtual enum for project layer configuration
	ENUM_DUMMY4WORD(PIN_NAND_CFG)
} PIN_NAND_CFG;

/**
    PIN config for Sensor

    @note For pinmux_init() with PIN_FUNC_SENSOR.\n
            For example, you can use {PIN_FUNC_SENSOR, PIN_SENSOR_CFG_10BITS|PIN_SENSOR_CFG_SHUTTER|PIN_SENSOR_CFG_MCLK}\n
            to declare sensor interface is 10 bits sensor, has SHUTTER and MCLK.
*/
typedef enum {
	PIN_SENSOR_CFG_NONE,
	PIN_SENSOR_CFG_8BITS = 0x01,        ///< 8 bits sensor. Enable SN_D[4..11]/SN_PXCLK on HSI_GPI[18..21][24..25][28..29]/S_GPIO[1]
	PIN_SENSOR_CFG_10BITS = 0x02,       ///< 10 bits sensor. Enable SN_D[2..11]/SN_PXCLK on HSI_GPI[16..21][24..25][28..29]/S_GPIO[1]
	PIN_SENSOR_CFG_12BITS = 0x04,       ///< 12 bits sensor. Enable SN_D[0..11]/SN_PXCLK on HSI_GPI[22..23][16..21][24..25][28..29]/S_GPIO[1]
	PIN_SENSOR_CFG_MIPI = 0x20,         ///< MIPI sensor. PIN/PAD is configured by PIN_FUNC_MIPI_LVDS group.
	PIN_SENSOR_CFG_LVDS = 0x40,         ///< LVDS sensor PIN/PAD is configured by PIN_FUNC_MIPI_LVDS group.
	PIN_SENSOR_CFG_LVDS_VDHD = 0x80,    ///< Enable SN_XVS/SN_XHS on S_GPIO[2..3]

	PIN_SENSOR_CFG_SHUTTER = 0x100,     ///< Enable SN_SHUTTER on S_GPIO[11]
	PIN_SENSOR_CFG_MCLK = 0x200,        ///< Enable SN_MCLK on S_GPIO[0]
	PIN_SENSOR_CFG_MCLK2 = 0x400,       ///< Enable SN_MCLK2 on S_GPIO[5]
	PIN_SENSOR_CFG_MES0 = 0x800,        ///< Enable ME_1_SHUT0 on P_GPIO[0]
	PIN_SENSOR_CFG_MES0_2ND = 0x1000,   ///< Enable (2nd) ME_2_SHUT0 on P_GPIO[16]
	PIN_SENSOR_CFG_MES1 = 0x2000,       ///< Enable ME_1_SHUT1 on P_GPIO[1]
	PIN_SENSOR_CFG_MES1_2ND = 0x4000,   ///< Enable (2nd) ME_2_SHUT1 on P_GPIO[17]
	PIN_SENSOR_CFG_FLCTR = 0x8000,      ///< Enable SN_FLCTR on P_GPIO[21]
	PIN_SENSOR_CFG_STROBE = 0x10000,    ///< Enable SN_FLASH on S_GPIO[10]
	PIN_SENSOR_CFG_SPCLK = 0x20000,     ///< Enable SP_CLK on P_GPIO[22]
	PIN_SENSOR_CFG_SPCLK_2ND = 0x40000, ///< Enable SP_CLK on P_GPIO[30]
	PIN_SENSOR_CFG_SP2CLK = 0x80000,    ///< Enable SP2_CLK on P_GPIO[23]
	PIN_SENSOR_CFG_SP2CLK_2ND = 0x100000,///< Enable SP2_CLK on P_GPIO[31]
	PIN_SENSOR_CFG_MES2 = 0x200000,      ///< Enable ME2_1_SHUT0 on P_GPIO[2]
	PIN_SENSOR_CFG_MES2_2ND = 0x400000,  ///< Enable (2nd) ME2_2_SHUT0 P_GPIO[18]
	PIN_SENSOR_CFG_MES3 = 0x800000,      ///< Enable ME2_1_SHUT1 on P_GPIO[3]
	PIN_SENSOR_CFG_MES3_2ND = 0x1000000, ///< Enable (2nd) ME2_2_SHUT1 on P_GPIO[19]
	PIN_SENSOR_CFG_LOCKN = 0x2000000,    ///< Enable LOCKN on P_GPIO[30]
	PIN_SENSOR_CFG_LOCKN2 = 0x4000000,   ///< Enable LOCKN2 on P_GPIO[31]


	ENUM_DUMMY4WORD(PIN_SENSOR_CFG)
} PIN_SENSOR_CFG;

/**
    PIN config for Sensor2

    @note For pinmux_init() with PIN_FUNC_SENSOR2.\n
			For example, you can use {PIN_FUNC_SENSOR2, xxxx}
*/
typedef enum {
	PIN_SENSOR2_CFG_NONE,
	PIN_SENSOR2_CFG_CCIR8BITS = 0x08,   ///< Backward compatible.(not used)
	PIN_SENSOR2_CFG_CCIR16BITS = 0x10,  ///< Backward compatible.(not used)
	PIN_SENSOR2_CFG_MIPI = 0x20,        ///< MIPI sensor. PIN/PAD is configured by PIN_FUNC_MIPI_LVDS group.
	PIN_SENSOR2_CFG_LVDS = 0x40,        ///< LVDS sensor. PIN/PAD is configured by PIN_FUNC_MIPI_LVDS group.
	PIN_SENSOR2_CFG_LVDS_VDHD = 0x80,   ///< Enable SN2_XVS/SN2_XHS on S_GPIO[6..7]
	PIN_SENSOR2_CFG_10BITS = 0x100,     ///< Backward compatible.(not used)

	PIN_SENSOR2_CFG_MCLK2 = 0x200,      ///< Enable SN_MCLK2 on S_GPIO[5]
	PIN_SENSOR2_CFG_MES2 = 0x400,       ///< Enable ME2_1_SHUT0 on P_GPIO[2]
	PIN_SENSOR2_CFG_MES2_2ND = 0x800,   ///< Enable (2nd) ME2_2_SHUT0 P_GPIO[18]
	PIN_SENSOR2_CFG_MES3 = 0x1000,      ///< Enable ME2_1_SHUT1 on P_GPIO[3]
	PIN_SENSOR2_CFG_MES3_2ND = 0x2000,  ///< Enable (2nd) ME2_2_SHUT1 on P_GPIO[19]

	ENUM_DUMMY4WORD(PIN_SENSOR2_CFG)
} PIN_SENSOR2_CFG;

/**
    PIN config for Sensor3

    @note For pinmux_init() with PIN_FUNC_SENSOR3.\n
            For example, you can use {PIN_FUNC_SENSOR3, PIN_SENSOR3_CFG_8BITS}\n
            to declare sensor3 interface connect a 8 bits sensor.
*/
typedef enum {
	PIN_SENSOR3_CFG_NONE,
	PIN_SENSOR3_CFG_8BITS  = 0x01,      ///< 8 bits sensor. Enable SN3_D[4..11]/SN3_PXCLK on HSI_GPI[4..5][8..13]/S_GPIO[4]
	PIN_SENSOR3_CFG_10BITS = 0x02,      ///< 10 bits sensor. Enable SN3_D[2..11]/SN3_PXCLK on HSI_GPI[0..1][4..5][8..13]/S_GPIO[4]
	PIN_SENSOR3_CFG_12BITS = 0x04,      ///< 12 bits sensor. Enable SN3_D[0..11]/SN3_PXCLK on HSI_GPI[2..3][0..1][4..5][8..13]/S_GPIO[4]
	PIN_SENSOR3_CFG_16BITS = 0x08,      ///< CCIR 16 bits sensor. Enable SN3_D[15..0]/SN3_PXCLK on HSI_GPI[4..5][8..13][18..21][24..25][28..29]/S_GPIO[4]
	PIN_SENSOR3_CFG_MIPI   = 0x10,      ///< MIPI sensor. PIN/PAD is configured by PIN_FUNC_MIPI_LVDS group.
	PIN_SENSOR3_CFG_LVDS   = 0x20,      ///< LVDS sensor. PIN/PAD is configured by PIN_FUNC_MIPI_LVDS group.
	PIN_SENSOR3_CFG_LVDS_VDHD = 0x40,       ///< Enable SN3_1_XVS/SN3_1_XHS on S_GPIO[6..7]
	PIN_SENSOR3_CFG_LVDS_VDHD_2ND = 0x80,   ///< Enable SN3_2_XVS/SN3_2_XHS on P_GPIO[2..3]

	PIN_SENSOR3_CFG_MCLK2  = 0x100,     ///< Enable MCLK2 on S_GPIO[5]
	PIN_SENSOR3_CFG_NOFILED = 0x200,    ///< no field pin of PIN_SENSOR3_CFG_8BITS & PIN_SENSOR3_CFG_16BITS

	ENUM_DUMMY4WORD(PIN_SENSOR3_CFG)
} PIN_SENSOR3_CFG;

/**
    PIN config for Sensor4

    @note For pinmux_init() with PIN_FUNC_SENSOR4.\n
            For example, you can use {PIN_FUNC_SENSOR4, xxxx}
*/
typedef enum {
	PIN_SENSOR4_CFG_NONE,
	PIN_SENSOR4_CFG_8BITS = 0x01,       ///< Backward compatible.(not used)
	PIN_SENSOR4_CFG_MIPI  = 0x04,       ///< MIPI sensor. PIN/PAD is configured by PIN_FUNC_MIPI_LVDS group.
	PIN_SENSOR4_CFG_LVDS  = 0x08,       ///< LVDS sensor. PIN/PAD is configured by PIN_FUNC_MIPI_LVDS group.

	PIN_SENSOR4_CFG_MCLK2 = 0x10,       ///< Enable MCLK2 on S_GPIO[5]
	PIN_SENSOR4_CFG_LVDS_VDHD = 0x20,   ///< Enable SN4_XVS/SN4_XHS on P_GPIO[4..5]

	ENUM_DUMMY4WORD(PIN_SENSOR4_CFG)
} PIN_SENSOR4_CFG;

/**
    PIN config for Sensor5

    @note For pinmux_init() with PIN_FUNC_SENSOR5.\n
            For example, you can use {PIN_FUNC_SENSOR5, xxxx}\n
*/
typedef enum {
	PIN_SENSOR5_CFG_NONE,
	PIN_SENSOR5_CFG_MIPI  = 0x04,       ///< MIPI sensor. PIN/PAD is configured by PIN_FUNC_MIPI_LVDS group.
	PIN_SENSOR5_CFG_LVDS  = 0x08,       ///< LVDS sensor. PIN/PAD is configured by PIN_FUNC_MIPI_LVDS group.

	PIN_SENSOR5_CFG_MCLK2 = 0x10,       ///< Enable MCLK2 on S_GPIO[5]
	PIN_SENSOR5_CFG_LVDS_VDHD = 0x20,   ///< Enable SN5_XVS/SN5_XHS on P_GPIO[6..7]

	ENUM_DUMMY4WORD(PIN_SENSOR5_CFG)
} PIN_SENSOR5_CFG;

/**
    PIN config for Sensor6

    @note For pinmux_init() with PIN_FUNC_SENSOR6.\n
            For example, you can use {PIN_FUNC_SENSOR6, xxxx}\n
*/
typedef enum {
	PIN_SENSOR6_CFG_NONE,
	PIN_SENSOR6_CFG_8BITS = 0x01,       ///< 8 bit sensor. Enable SN6_D[0..7]/SN6_PXCLK on P_GPIO[2..9]/P_GPIO[10]
	PIN_SENSOR6_CFG_16BITS = 0x02,      ///< 16 bit sensor. Enable SN6_D[0..15]/SN6_PXCLK on P_GPIO[2..9][14..21]/P_GPIO[10]
	PIN_SENSOR6_CFG_MIPI  = 0x04,       ///< MIPI sensor. PIN/PAD is configured by PIN_FUNC_MIPI_LVDS group.
	PIN_SENSOR6_CFG_LVDS  = 0x08,       ///< LVDS sensor. PIN/PAD is configured by PIN_FUNC_MIPI_LVDS group.

	PIN_SENSOR6_CFG_MCLK2 = 0x10,       ///< Enable MCLK2 on S_GPIO[5]
	PIN_SENSOR6_CFG_LVDS_VDHD = 0x20,   ///< Enable SN6_XVS/SN6_XHS on P_GPIO[0..1]

	ENUM_DUMMY4WORD(PIN_SENSOR6_CFG)
} PIN_SENSOR6_CFG;

/**
    PIN config for Sensor7

    @note For pinmux_init() with PIN_FUNC_SENSOR7.\n
            For example, you can use {PIN_FUNC_SENSOR7, xxxx}\n
*/
typedef enum {
	PIN_SENSOR7_CFG_NONE,
	PIN_SENSOR7_CFG_8BITS = 0x01,       ///< 8 bit sensor. Enable SN7_D[0..7]/SN7_PXCLK on P_GPIO[14..21]/P_GPIO[11]
	PIN_SENSOR7_CFG_MIPI  = 0x04,       ///< MIPI sensor. PIN/PAD is configured by PIN_FUNC_MIPI_LVDS group.
	PIN_SENSOR7_CFG_LVDS  = 0x08,       ///< LVDS sensor. PIN/PAD is configured by PIN_FUNC_MIPI_LVDS group.

	PIN_SENSOR7_CFG_MCLK2 = 0x10,       ///< Enable MCLK2 on S_GPIO[5]
	PIN_SENSOR7_CFG_LVDS_VDHD = 0x20,   ///< Enable SN7_XVS/SN7_XHS on P_GPIO[12..13]

	ENUM_DUMMY4WORD(PIN_SENSOR7_CFG)
} PIN_SENSOR7_CFG;

/**
    PIN config for Sensor8

    @note For pinmux_init() with PIN_FUNC_SENSOR8.\n
            For example, you can use {PIN_FUNC_SENSOR8, xxxx}\n
*/
typedef enum {
	PIN_SENSOR8_CFG_NONE,
	PIN_SENSOR8_CFG_8BITS = 0x01,       ///< 8 bit sensor. Enable SN8_D[0..7]/SN8_PXCLK on L_GPIO[15..22]/L_GPIO[23]
	PIN_SENSOR8_CFG_MIPI  = 0x04,       ///< MIPI sensor. PIN/PAD is configured by PIN_FUNC_MIPI_LVDS group.
	PIN_SENSOR8_CFG_LVDS  = 0x08,       ///< LVDS sensor. PIN/PAD is configured by PIN_FUNC_MIPI_LVDS group.

	PIN_SENSOR8_CFG_MCLK2 = 0x10,       ///< Enable MCLK2 on S_GPIO[5]
	PIN_SENSOR8_CFG_LVDS_VDHD = 0x20,   ///< Enable SN8_XVS/SN8_XHS on P_GPIO[14..15]

	ENUM_DUMMY4WORD(PIN_SENSOR8_CFG)
} PIN_SENSOR8_CFG;


/**
    PIN config for MIPI/LVDS

    @note For pinmux_init() with PIN_FUNC_MIPI_LVDS.\n
            For example, you can use {PIN_FUNC_MIPI_LVDS, PIN_MIPI_LVDS_CFG_CLK0|PIN_MIPI_LVDS_CFG_DAT0|PIN_MIPI_LVDS_CFG_DAT4}\n
            to declare LVDS/MIPI connect sensor with CLK0/D0/D4 lanes.
*/
typedef enum {
	PIN_MIPI_LVDS_CFG_NONE,

	PIN_MIPI_LVDS_CFG_CLK0 = 0x1,       ///< CLK lane 0. Enable HSI_CK0N/HSI_CK0P on HSI_GPI[18..19] for LVDS/CSI.
	PIN_MIPI_LVDS_CFG_CLK1 = 0x2,       ///< CLK lane 1. Enable HSI_CK1N/HSI_CK1P on HSI_GPI[22..23] for LVDS/CSI.
	PIN_MIPI_LVDS_CFG_CLK2 = 0x4,       ///< CLK lane 2. Enable HSI_CK2N/HSI_CK2P on HSI_GPI[26..27] for LVDS/CSI.
	PIN_MIPI_LVDS_CFG_CLK3 = 0x8,       ///< CLK lane 3. Enable HSI_CK3N/HSI_CK3P on HSI_GPI[30..31] for LVDS/CSI.
	PIN_MIPI_LVDS_CFG_CLK4 = 0x10,      ///< CLK lane 4. Enable HSI_CK4N/HSI_CK4P on HSI_GPI[2..3] for LVDS/CSI.
	PIN_MIPI_LVDS_CFG_CLK5 = 0x20,      ///< CLK lane 5. Enable HSI_CK5N/HSI_CK5P on HSI_GPI[6..7] for LVDS/CSI.
	PIN_MIPI_LVDS_CFG_CLK6 = 0x40,      ///< CLK lane 6. Enable HSI_CK6N/HSI_CK6P on HSI_GPI[10..11] for LVDS/CSI.
	PIN_MIPI_LVDS_CFG_CLK7 = 0x80,      ///< CLK lane 7. Enable HSI_CK7N/HSI_CK7P on HSI_GPI[14..15] for LVDS/CSI.

	PIN_MIPI_LVDS_CFG_DAT0 = 0x100,     ///< DATA lane 0. Enable HSI_D0N/HSI_D0P on HSI_GPI[16..17] for LVDS/CSI.
	PIN_MIPI_LVDS_CFG_DAT1 = 0x200,     ///< DATA lane 1. Enable HSI_D1N/HSI_D1P on HSI_GPI[20..21] for LVDS/CSI.
	PIN_MIPI_LVDS_CFG_DAT2 = 0x400,     ///< DATA lane 2. Enable HSI_D2N/HSI_D2P on HSI_GPI[24..25] for LVDS/CSI.
	PIN_MIPI_LVDS_CFG_DAT3 = 0x800,     ///< DATA lane 3. Enable HSI_D3N/HSI_D3P on HSI_GPI[28..29] for LVDS/CSI.
	PIN_MIPI_LVDS_CFG_DAT4 = 0x1000,    ///< DATA lane 4. Enable HSI_D4N/HSI_D4P on HSI_GPI[0..1] for LVDS/CSI.
	PIN_MIPI_LVDS_CFG_DAT5 = 0x2000,    ///< DATA lane 5. Enable HSI_D5N/HSI_D5P on HSI_GPI[4..5] for LVDS/CSI.
	PIN_MIPI_LVDS_CFG_DAT6 = 0x4000,    ///< DATA lane 6. Enable HSI_D6N/HSI_D6P on HSI_GPI[8..9] for LVDS/CSI.
	PIN_MIPI_LVDS_CFG_DAT7 = 0x8000,    ///< DATA lane 7. Enable HSI_D7N/HSI_D7P on HSI_GPI[12..13] for LVDS/CSI.
	PIN_MIPI_LVDS_CFG_DAT8 = 0x10000,   ///< Backward compatible.(not used)
	PIN_MIPI_LVDS_CFG_DAT9 = 0x20000,   ///< Backward compatible.(not used)

	ENUM_DUMMY4WORD(PIN_MIPI_LVDS_CFG)
} PIN_MIPI_LVDS_CFG;

/**
    PIN config for I2C

    @note For pinmux_init() with PIN_FUNC_I2C.\n
            For example, you can use {PIN_FUNC_I2C, PIN_I2C_CFG_CH1|PIN_I2C_CFG_CH1_2ND_PINMUX}\n
            to declare I2C channel1 both output to 1st and 2nd pinmux location.
*/
typedef enum {
	PIN_I2C_CFG_NONE,
	PIN_I2C_CFG_CH1            = 0x1,     ///< Enable channel 1.               Enable I2C    SCL/SDA  (P_GPIO[28..29])
	PIN_I2C_CFG_CH1_2ND_PINMUX = 0x2,     ///< Enable 2nd pinmux of channel 1. Enable I2C_2  SCL/SDA  (C_GPIO[12..13])
	PIN_I2C_CFG_CH2            = 0x10,    ///< Enable channel 2.               Enable I2C2   SCL/SDA  (S_GPIO[8..9])
	PIN_I2C_CFG_CH2_2ND_PINMUX = 0x20,    ///< Enable 2nd pinmux of channel 2. Enable I2C2_2 SCL/SDA  (P_GPIO[8..9])
	PIN_I2C_CFG_CH3            = 0x100,   ///< Enable channel 3.               Enable I2C3   SCL/SDA  (S_GPIO[10..11])
	PIN_I2C_CFG_CH3_2ND_PINMUX = 0x200,   ///< Enable 2nd pinmux of channel 3. Enable I2C3_2 SCL/SDA  (P_GPIO[10..11])
	PIN_I2C_CFG_CH3_3RD_PINMUX = 0x400,   ///< Enable 3nd pinmux of channel 3. Enable I2C3_3 SCL/SDA  (P_GPIO[26..27])
	PIN_I2C_CFG_CH4            = 0x1000,  ///< Enable channel 4.               Enable I2C4   SCL/SDA  (P_GPIO[16..17])
	PIN_I2C_CFG_CH4_2ND_PINMUX = 0x2000,  ///< Enable 2nd pinmux of channel 4. Enable I2C4_2 SCL/SDA  (P_GPIO[24..25])
	PIN_I2C_CFG_CH4_3RD_PINMUX = 0x4000,  ///< Enable 3nd pinmux of channel 4. Enable I2C4_3 SCL/SDA  (P_GPIO[18..19])
	PIN_I2C_CFG_CH4_4TH_PINMUX = 0x8000,  ///< Enable 4th pinmux of channel 4. Enable I2C4_4 SCL/SDA  (L_GPIO[28..29])
	PIN_I2C_CFG_CH5            = 0x10000, ///< Enable channel 5.               Enable I2C5   SCL/SDA  (P_GPIO[45..46])
	PIN_I2C_CFG_CH5_2ND_PINMUX = 0x20000, ///< Enable 2nd pinmux of channel 5. Enable I2C5_2 SCL/SDA  (C_GPIO[14..15])

	ENUM_DUMMY4WORD(PIN_I2C_CFG)
} PIN_I2C_CFG;

/**
    PIN config for SIF

    *note For pinmux_init() with PIN_FUNC_SIF.\n
            For example, you can use {PIN_FUNC_SIF, PIN_SIF_CFG_CH0|PIN_SIF_CFG_CH2}\n
            to declare SIF channel0 and channel2 pinmux are enabled.
*/
typedef enum {
	PIN_SIF_CFG_NONE,

	PIN_SIF_CFG_CH0 = 0x01,             ///< Enable SIF channel 0. Enable SB_CS0/SB_SCK0/SB_DAT0 (L_GPIO 27/28/29)
	PIN_SIF_CFG_CH0_2ND_PINMUX = 0x02,  ///< Enable SIF channel 0 2'nd pinmux. Enable SB_CS0/SB_SCK0/SB_SAT0 (P_GPIO 16/18/19)
	PIN_SIF_CFG_CH0_3RD_PINMUX = 0x04,  ///< Enable SIF channel 0 3'rd pinmux. Enable SB_CS0/SB_SCK0/SB_SAT0 (P_GPIO 17/18/19)

	PIN_SIF_CFG_CH1 = 0x08,             ///< Enable SIF channel 1. Enable SB_CS1/SB_SCK1/SB_DAT1 (L_GPIO 30/31/32)
	PIN_SIF_CFG_CH1_2ND_PINMUX = 0x10,  ///< Backward compatible. (not used)

	PIN_SIF_CFG_CH2 = 0x20,             ///< Enable SIF channel 2. Enable SB_CS2/SB_CK23/SB_DAT23 (P_GPIO 30/32/33)
	PIN_SIF_CFG_CH2_2ND_PINMUX = 0x40,  ///< Enable SIF channel 2 2'nd pinmux. Enable SB_CS2/SB_CK23/SB_DAT23 (P_GPIO 8/10/11)

	PIN_SIF_CFG_CH3 = 0x80,             ///< Enable SIF channel 3. Enable SB_CS3/SB_CK23/SB_DAT23 (P_GPIO 31/32/33)
	PIN_SIF_CFG_CH3_2ND_PINMUX = 0x100, ///< Enable SIF channel 3 2'nd pinmux. Enable SB_CS2/SB_CK23/SB_DAT23 (P_GPIO 9/10/11)

	PIN_SIF_CFG_CH4 = 0x200,            ///< Enable SIF channel 4. Enable SB_CS4/SB_CK45/SB_DAT45 (P_GPIO 24/26/27)

	PIN_SIF_CFG_CH5 = 0x400,            ///< Enable SIF channel 5. Enable SB_CS5/SB_CK45/SB_DAT45 (P_GPIO 25/26/27)

	PIN_SIF_CFG_CH6 = 0x800,            ///< Enable SIF channel 6. Enable SB_CS6/SB_CK67/SB_DAT67 (S_GPIO 10/8/9)

	PIN_SIF_CFG_CH7 = 0x1000,           ///< Enable SIF channel 7. Enable SB_CS7/SB_CK67/SB_DAT67 (S_GPIO 11/8/9)

	ENUM_DUMMY4WORD(PIN_SIF_CFG)
} PIN_SIF_CFG;

/**
    PIN config for UART

    @note For pinmux_init() with PIN_FUNC_UART.\n
            For example, you can use {PIN_FUNC_UART, PIN_UART_CFG_CH1}\n
            to declare UART1 pinmux is enabled.
*/
typedef enum {
	PIN_UART_CFG_NONE,

	PIN_UART_CFG_CH1 = 0x01,            ///< Enable channel 1. Enable UART TX and RX. Enable TX/RX (dedicate UART_TX/UART_RX on P_GPIO[42..43])
	PIN_UART_CFG_CH1_TX = 0x02,         ///< Enalbe channel 1. Enable only UART_TX. Enable TX (dedicate UART_TX on P_GPIO[42])

	PIN_UART_CFG_CH2 = 0x04,            ///< Enable channel 2. Enable UART2 TX and RX. Enable TX/RX (P_GPIO[34..35]), Enable RTS/CTS (P_GPIO[36..37])
	PIN_UART_CFG_CH2_CTSRTS = 0x08,     ///< Enable channel 2 HW handshake. Enable UART2 CTS and RTS
	PIN_UART_CFG_CH2_2ND = 0x10,        ///< Enable channel 2 to 2ND pinmux (UART2_2). Enable TX/RX (C_GPIO[29..30]), Enable RTS/CTS (C_GPIO[31..32])

	PIN_UART_CFG_CH3 = 0x20,            ///< Enable channel 3. Enable UART3 TX and RX. Enable TX/RX (P_GPIO[30..31]), Enable RTS/CTS (P_GPIO[32..33])
	PIN_UART_CFG_CH3_CTSRTS = 0x40,     ///< Enable channel 3 HW handshake. Enable UART3 CTS and RTS
	PIN_UART_CFG_CH3_2ND = 0x80,        ///< Enable channel 3 to 2ND pinmux (UART3_2). Enable TX/RX (P_GPIO[8..9]), Enable RTS/CTS (P_GPIO[10..11])

	PIN_UART_CFG_CH4 = 0x100,           ///< Enable channel 4. Enable UART4 TX and RX. Enable TX/RX (P_GPIO[4..5]), Enable RTS/CTS (P_GPIO[6..7])
	PIN_UART_CFG_CH4_CTSRTS = 0x200,    ///< Enable channel 4 HW handshake. Enable UART4 CTS and RTS.
	PIN_UART_CFG_CH4_2ND = 0x400,       ///< Enable channel 4 to 2ND pinmux (UART4_2). Enable TX/RX (C_GPIO[12..13]), Enable RTS/CTS (C_GPIO[14..15])

	ENUM_DUMMY4WORD(PIN_UART_CFG)
} PIN_UART_CFG;

/**
    PIN config for SPI

    @note For pinmux_init() with PIN_FUNC_SPI.\n
            For example, you can use {PIN_FUNC_SPI, PIN_SPI_CFG_CH1|PIN_SPI_CFG_CH3}\n
            to declare SPI channel1 and channel3 are enabled.
*/
typedef enum {
	PIN_SPI_CFG_NONE,

//    PIN_SPI_CFG_CH1                = 0x1,      ///< Enable channel 1. Enable SPI_DO/SPI_CLK/SPI_CS on MC[0]/MC[2]/MC8 (C_GPIO[0]/C_GPIO[2]/C_GPIO[8])
//    PIN_SPI_CFG_CH1_2BITS          = 0x2,      ///< Enable channel 1 with 2 bits mode. Enable SPI_DO/SPI_DI/SPI_CLK/SPI_CS on MC[0..2]/MC8 (C_GPIO[0..2]/C_GPIO[8])
//    PIN_SPI_CFG_CH1_4BITS          = 0x4,      ///< Backward compatible
//    PIN_SPI_CFG_CH1_2ND_PINMUX     = 0x8,      ///< Enable 2nd pinmux of channel 1.

	PIN_SPI_CFG_CH1                = 0x10,      ///< Enable channel 1. Enable SPI_CS/SPI_CLK/SPI_DO(/SPI_DI) on P_GPIO[38..40](/P_GPIO41)
	PIN_SPI_CFG_CH1_2BITS          = 0x20,      ///< Enable channel 1 with 2 data pins (SPI_DO and SPI_DI).
	PIN_SPI_CFG_CH1_2ND_PINMUX     = 0x40,      ///< Enable 2nd pinmux of channel 1. Enable SPI_CS/SPI_CLK/SPI_DO(/SPI_DI) on P_GPIO[16..18](/P_GPIO[19])
//    PIN_SPI_CFG_CH2_3RD_PINMUX     = 0x80,


	PIN_SPI_CFG_CH2                = 0x100,     ///< Enable channel 2. Eanble SPI2_CS/SPI2_CLK/SPI2_DO(/SPI2_DI) on P_GPIO[30]/P_GPIO[32..33](/P_GPIO[31])
	PIN_SPI_CFG_CH2_2BITS          = 0x200,     ///< Enable channel 2 with 2 data pins (SPI2_DO and SPI2_DI).
	PIN_SPI_CFG_CH2_2ND_PINMUX     = 0x400,     ///< Enable 2nd pinmux of channel 2. Enable SPI2_CS/SPI2_CLK/SPI2_DO(/SPI2_DI) on P_GPIO[12..14](/P_GPIO[15)
	PIN_SPI_CFG_CH2_3RD_PINMUX     = 0x800,     ///< Enable 3rd pinmux of channel 2. Enable SPI2_CS/SPI2_CLK/SPI2_DO(/SPI2_DI) on C_GPIO[23..22]/C_GPIO[25](/C_GPIO[24])
//    PIN_SPI_CFG_CH2_RDY            = 0x1000,
//    PIN_SPI_CFG_CH2_RDY_2ND_PINMUX = 0x2000,

	PIN_SPI_CFG_CH3                = 0x10000,   ///< Enable channel 3. Eanble SPI3_CS/SPI3_CLK/SPI3_DO(/SPI3_DI) on C_GPIO[29..28]/C_GPIO[31](/C_GPIO[30])
	PIN_SPI_CFG_CH3_2BITS          = 0x20000,   ///< Enable channel 3. with 2 data pins (SPI3_DO and SPI3_DI).
	PIN_SPI_CFG_CH3_2ND_PINMUX     = 0x40000,   ///< Enable 2nd pinmux of channel 3. Enable SPI3_CS/SPI3_CLK/SPI3_DO(/SPI3_DI) on P_GPIO[8]/P_GPIO[10]/P_GPIO[11](P_GPIO[9])
	PIN_SPI_CFG_CH3_RDY            = 0x80000,   ///< Enable channel 3 RDY. Eanble SPI3_RDY on C_GPIO[32]
	PIN_SPI_CFG_CH3_RDY_2ND_PINMUX = 0x01000,   ///< Enable 2nd pinmux of channel 3 RDY. Eanble SPI3_RDY on P_GPIO[3]
//    PIN_SPI_CFG_CH4_3RD_PINMUX     = 0x100000,

	PIN_SPI_CFG_CH4                = 0x100000,   ///< Enable channel 4. Eanble SPI4_CS/SPI4_CLK/SPI4_DO(/SPI4_DI) on S_GPIO[11]/S_GPIO[8..9](/S_GPIO[10])
	PIN_SPI_CFG_CH4_2BITS          = 0x200000,   ///< Enable channel 4. with 2 data pins (SPI4_DO and SPI4_DI).
	PIN_SPI_CFG_CH4_2ND_PINMUX     = 0x400000,   ///< Enable 2nd pinmux of channel 4. Eanble SPI4_CS/SPI4_CLK/SPI4_DO(/SPI4_DI) on P_GPIO[34..36](/P_GPIO[37])
	PIN_SPI_CFG_CH4_3RD_PINMUX     = 0x800000,   ///< Enable 2nd pinmux of channel 4. Eanble SPI4_CS/SPI4_CLK/SPI4_DO(/SPI4_DI) on L_GPIO[27..29](/P_GPIO[26])

	ENUM_DUMMY4WORD(PIN_SPI_CFG)
} PIN_SPI_CFG;

/**
    PIN config for REMOTE

    @note For pinmux_init() with PIN_FUNC_REMOTE.\n
            For example, you can use {PIN_FUNC_REMOTE, PIN_REMOTE_CFG_CH1}\n
            to declare remote pinmux is enabled.
*/
typedef enum {
	PIN_REMOTE_CFG_NONE,
	PIN_REMOTE_CFG_CH1,                 ///< Enable remote rx channel 1. Enable REMOTE_RX (on P_GPIO[20])

	ENUM_DUMMY4WORD(PIN_REMOTE_CFG)
} PIN_REMOTE_CFG;

/**
    PIN config for PWM

    @note For pinmux_init() with PIN_FUNC_PWM.\n
            For example, you can use {PIN_FUNC_PWM, PIN_PWM_CFG_PWM0|PIN_PWM_CFG_PWM1|PIN_PWM_CFG_PWM2|PIN_PWM_CFG_PWM3}\n
            to declare your system need PWM channel0/1/2/3.
*/
typedef enum {
	PIN_PWM_CFG_NONE,
	PIN_PWM_CFG_PWM0 = 0x01,            ///< Enable PWM0 (on P_GPIO[0])
	PIN_PWM_CFG_PWM1 = 0x02,            ///< Enable PWM1 (on P_GPIO[1])
	PIN_PWM_CFG_PWM2 = 0x04,            ///< Enable PWM2 (on P_GPIO[2])
	PIN_PWM_CFG_PWM3 = 0x08,            ///< Enable PWM3 (on P_GPIO[3])
	PIN_PWM_CFG_PWM4 = 0x10,            ///< Enable PWM4 (on P_GPIO[4])
	PIN_PWM_CFG_PWM5 = 0x20,            ///< Enable PWM5 (on P_GPIO[5])
	PIN_PWM_CFG_PWM6 = 0x40,            ///< Enable PWM6 (on P_GPIO[6])
	PIN_PWM_CFG_PWM7 = 0x80,            ///< Enable PWM7 (on P_GPIO[7])
	PIN_PWM_CFG_PWM8 = 0x100,           ///< Enable PWM8 (on P_GPIO[8])
	PIN_PWM_CFG_PWM9 = 0x200,           ///< Enable PWM9 (on P_GPIO[9])
	PIN_PWM_CFG_PWM10 = 0x400,          ///< Enable PWM10 (on P_GPIO[10])
	PIN_PWM_CFG_PWM11 = 0x800,          ///< Enable PWM11 (on P_GPIO[11])
	PIN_PWM_CFG_PWM12 = 0x1000,         ///< Enable PWM12 (on P_GPIO[12])
	PIN_PWM_CFG_PWM13 = 0x2000,         ///< Enable PWM13 (on P_GPIO[13])
	PIN_PWM_CFG_PWM14 = 0x4000,         ///< Enable PWM14 (on P_GPIO[14])
	PIN_PWM_CFG_PWM15 = 0x8000,         ///< Enable PWM15 (on P_GPIO[15])
	PIN_PWM_CFG_PWM16 = 0x10000,        ///< Enable PWM16 (on P_GPIO[16])
	PIN_PWM_CFG_PWM17 = 0x20000,        ///< Enable PWM17 (on P_GPIO[17])
	PIN_PWM_CFG_PWM18 = 0x40000,        ///< Enable PWM18 (on P_GPIO[18])
	PIN_PWM_CFG_PWM19 = 0x80000,        ///< Enable PWM19 (on P_GPIO[19])

	PIN_PWM_CFG_CCNT      = 0x1000000,  ///< Enable PICNT_1 (on P_GPIO[36])
	PIN_PWM_CFG_CCNT_2ND  = 0x2000000,  ///< Enable PICNT_2 (on L_GPIO[20])
	PIN_PWM_CFG_CCNT2     = 0x4000000,  ///< Enable PICNT2_1 (on P_GPIO[37])
	PIN_PWM_CFG_CCNT2_2ND = 0x8000000,  ///< Enable PICNT2_2 (on L_GPIO[21])
	PIN_PWM_CFG_CCNT3     = 0x10000000, ///< Enable PICNT3_1 (on P_GPIO[20])
	PIN_PWM_CFG_CCNT3_2ND = 0x20000000, ///< Enable PICNT3_2 (on L_GPIO[22])
	PIN_PWM_CFG_CCNT4     = 0x40000000, ///< Enable PICNT4_1 (on P_GPIO[22])
	PIN_PWM_CFG_CCNT4_2ND = 0x80000000, ///< Enable PICNT4_2 (on L_GPIO[27])

	ENUM_DUMMY4WORD(PIN_PWM_CFG)
} PIN_PWM_CFG;

/**
    PIN config for AUDIO

    @note For pinmux_init() with PIN_FUNC_AUDIO.\n
            For example, you can use {PIN_FUNC_AUDIO, PIN_AUDIO_CFG_I2S|PIN_AUDIO_CFG_MCLK}\n
            to declare pinmux of I2S and I2S MCLK are enabled.
*/
typedef enum {
	PIN_AUDIO_CFG_NONE,
	PIN_AUDIO_CFG_I2S = 0x01,           ///< Enable I2S channel. Enable I2S_BCLK/I2S_SYNC/I2S_DO/I2S_DI (on P_GPIO[34..37])
	PIN_AUDIO_CFG_I2S_2ND_PINMUX = 0x02,///< Backward compatible.(not used)

	PIN_AUDIO_CFG_MCLK = 0x10,          ///< Enable audio MCLK. Enable I2S_MCLK (on P_GPIO[30])
	PIN_AUDIO_CFG_MCLK_2ND_PINMUX = 0x20,///< Backward compatible.(not used)

	ENUM_DUMMY4WORD(PIN_AUDIO_CFG)
} PIN_AUDIO_CFG;

/**
    PIN config of LCD modes

    @note For pinmux_init() with PIN_FUNC_LCD or PIN_FUNC_LCD2.\n
            For example, you can use {PIN_FUNC_LCD, PINMUX_DISPMUX_SEL_LCD | PINMUX_LCDMODE_RGB_SERIAL}\n
            to tell display object that PIN_FUNC_LCD is located on primary LCD pinmux,\n
            and it's LCD mode is RGB serial.
*/
typedef enum {
	PINMUX_LCDMODE_RGB_SERIAL           =    0,     ///< LCD MODE is RGB Serial or UPS051
	PINMUX_LCDMODE_RGB_PARALL           =    1,     ///< LCD MODE is RGB Parallel (888)
	PINMUX_LCDMODE_YUV640               =    2,     ///< LCD MODE is YUV640
	PINMUX_LCDMODE_YUV720               =    3,     ///< LCD MODE is YUV720
	PINMUX_LCDMODE_RGBD360              =    4,     ///< LCD MODE is RGB Dummy 360
	PINMUX_LCDMODE_RGBD320              =    5,     ///< LCD MODE is RGB Dummy 320
	PINMUX_LCDMODE_RGB_THROUGH          =    6,     ///< LCD MODE is RGB through mode
	PINMUX_LCDMODE_CCIR601              =    7,     ///< LCD MODE is CCIR601
	PINMUX_LCDMODE_CCIR656              =    8,     ///< LCD MODE is CCIR656
	PINMUX_LCDMODE_RGB_PARALL666        =    9,     ///< LCD MODE is RGB Parallel 666
	PINMUX_LCDMODE_RGB_PARALL565        =   10,     ///< LCD MODE is RGB Parallel 565
	PINMUX_LCDMODE_RGB_PARALL_DELTA     =   11,     ///< LCD MODE is RGB Parallel Delta
	PINMUX_LCDMODE_MIPI                 =   12,     ///< LCD MODE is MIPI Display

	PINMUX_LCDMODE_MI_OFFSET            = 32,       ///< Memory LCD MODE offset
	PINMUX_LCDMODE_MI_FMT0              = 32 + 0,   ///< LCD MODE is Memory(Parallel Interface) 8bits
	PINMUX_LCDMODE_MI_FMT1              = 32 + 1,   ///< LCD MODE is Memory(Parallel Interface) 8bits
	PINMUX_LCDMODE_MI_FMT2              = 32 + 2,   ///< LCD MODE is Memory(Parallel Interface) 8bits
	PINMUX_LCDMODE_MI_FMT3              = 32 + 3,   ///< LCD MODE is Memory(Parallel Interface) 8bits
	PINMUX_LCDMODE_MI_FMT4              = 32 + 4,   ///< LCD MODE is Memory(Parallel Interface) 8bits
	PINMUX_LCDMODE_MI_FMT5              = 32 + 5,   ///< LCD MODE is Memory(Parallel Interface) 8bits
	PINMUX_LCDMODE_MI_FMT6              = 32 + 6,   ///< LCD MODE is Memory(Parallel Interface) 8bits
	PINMUX_LCDMODE_MI_FMT7              = 32 + 7,   ///< LCD MODE is Memory(Parallel Interface) 9bits
	PINMUX_LCDMODE_MI_FMT8              = 32 + 8,   ///< LCD MODE is Memory(Parallel Interface) 16bits
	PINMUX_LCDMODE_MI_FMT9              = 32 + 9,   ///< LCD MODE is Memory(Parallel Interface) 16bits
	PINMUX_LCDMODE_MI_FMT10             = 32 + 10,  ///< LCD MODE is Memory(Parallel Interface) 18bits
	PINMUX_LCDMODE_MI_FMT11             = 32 + 11,  ///< LCD MODE is Memory(Parallel Interface)  8bits
	PINMUX_LCDMODE_MI_FMT12             = 32 + 12,  ///< LCD MODE is Memory(Parallel Interface) 16bits
	PINMUX_LCDMODE_MI_FMT13             = 32 + 13,  ///< LCD MODE is Memory(Parallel Interface) 16bits
	PINMUX_LCDMODE_MI_FMT14             = 32 + 14,  ///< LCD MODE is Memory(Parallel Interface) 16bits
	PINMUX_LCDMODE_MI_SERIAL_BI         = 32 + 20,  ///< LCD MODE is Serial Interface bi-direction
	PINMUX_LCDMODE_MI_SERIAL_SEP        = 32 + 21,  ///< LCD MODE is Serial Interface separation

	PINMUX_LCDMODE_AUTO_PINMUX          = 0x01 << 22, ///< Set display device to GPIO mode when display device is closed. Select this filed will inform display object to switch to GPIO when display device is closed.


	PINMUX_HDMIMODE_OFFSET              = 64,       ///< HDMI MODE offset
	PINMUX_HDMIMODE_640X480P60          = 64 + 1,   ///< HDMI Video format is 640x480 & Progressive 60fps
	PINMUX_HDMIMODE_720X480P60          = 64 + 2,   ///< HDMI Video format is 720x480 & Progressive 60fps & 4:3
	PINMUX_HDMIMODE_720X480P60_16X9     = 64 + 3,   ///< HDMI Video format is 720x480 & Progressive 60fps & 16:9
	PINMUX_HDMIMODE_1280X720P60         = 64 + 4,   ///< HDMI Video format is 1280x720 & Progressive 60fps
	PINMUX_HDMIMODE_1920X1080I60        = 64 + 5,   ///< HDMI Video format is 1920x1080 & Interlaced 60fps
	PINMUX_HDMIMODE_720X480I60          = 64 + 6,   ///< HDMI Video format is 720x480 & Interlaced 60fps
	PINMUX_HDMIMODE_720X480I60_16X9     = 64 + 7,   ///< HDMI Video format is 720x480 & Interlaced 60fps & 16:9
	PINMUX_HDMIMODE_720X240P60          = 64 + 8,   ///< HDMI Video format is 720x240 & Progressive 60fps
	PINMUX_HDMIMODE_720X240P60_16X9     = 64 + 9,   ///< HDMI Video format is 720x240 & Progressive 60fps & 16:9
	PINMUX_HDMIMODE_1440X480I60         = 64 + 10,  ///< HDMI Video format is 1440x480 & Interlaced 60fps
	PINMUX_HDMIMODE_1440X480I60_16X9    = 64 + 11,  ///< HDMI Video format is 1440x480 & Interlaced 60fps & 16:9
	PINMUX_HDMIMODE_1440X240P60         = 64 + 12,  ///< HDMI Video format is 1440x240 & Progressive 60fps
	PINMUX_HDMIMODE_1440X240P60_16X9    = 64 + 13,  ///< HDMI Video format is 1440x240 & Progressive 60fps & 16:9
	PINMUX_HDMIMODE_1440X480P60         = 64 + 14,  ///< HDMI Video format is 1440x480 & Progressive 60fps
	PINMUX_HDMIMODE_1440X480P60_16X9    = 64 + 15,  ///< HDMI Video format is 1440x480 & Progressive 60fps & 16:9
	PINMUX_HDMIMODE_720X576P50          = 64 + 17,  ///< HDMI Video format is 720x576 & Progressive 50fps
	PINMUX_HDMIMODE_720X576P50_16X9     = 64 + 18,  ///< HDMI Video format is 720x576 & Progressive 50fps & 16:9
	PINMUX_HDMIMODE_1280X720P50         = 64 + 19,  ///< HDMI Video format is 1280x720 & Progressive 50fps
	PINMUX_HDMIMODE_1920X1080I50        = 64 + 20,  ///< HDMI Video format is 1920x1080 & Interlaced 50fps
	PINMUX_HDMIMODE_720X576I50          = 64 + 21,  ///< HDMI Video format is 720x576 & Interlaced 50fps
	PINMUX_HDMIMODE_720X576I50_16X9     = 64 + 22,  ///< HDMI Video format is 720x576 & Interlaced 50fps & 16:9
	PINMUX_HDMIMODE_720X288P50          = 64 + 23,  ///< HDMI Video format is 720x288 & Progressive 50fps
	PINMUX_HDMIMODE_720X288P50_16X9     = 64 + 24,  ///< HDMI Video format is 720x288 & Progressive 50fps & 16:9
	PINMUX_HDMIMODE_1440X576I50         = 64 + 25,  ///< HDMI Video format is 1440x576 & Interlaced 50fps
	PINMUX_HDMIMODE_1440X576I50_16X9    = 64 + 26,  ///< HDMI Video format is 1440x576 & Interlaced 50fps & 16:9
	PINMUX_HDMIMODE_1440X288P50         = 64 + 27,  ///< HDMI Video format is 1440x288 & Progressive 50fps
	PINMUX_HDMIMODE_1440X288P50_16X9    = 64 + 28,  ///< HDMI Video format is 1440x288 & Progressive 50fps & 16:9
	PINMUX_HDMIMODE_1440X576P50         = 64 + 29,  ///< HDMI Video format is 1440x576 & Progressive 50fps
	PINMUX_HDMIMODE_1440X576P50_16X9    = 64 + 30,  ///< HDMI Video format is 1440x576 & Progressive 50fps & 16:9
	PINMUX_HDMIMODE_1920X1080P50        = 64 + 31,  ///< HDMI Video format is 1920x1080 & Progressive 50fps
	PINMUX_HDMIMODE_1920X1080P24        = 64 + 32,  ///< HDMI Video format is 1920x1080 & Progressive 24fps
	PINMUX_HDMIMODE_1920X1080P25        = 64 + 33,  ///< HDMI Video format is 1920x1080 & Progressive 25fps
	PINMUX_HDMIMODE_1920X1080P30        = 64 + 34,  ///< HDMI Video format is 1920x1080 & Progressive 30fps
	PINMUX_HDMIMODE_1920X1080I50_VT1250 = 64 + 39,  ///< HDMI Video format is 1920x1080 & Interlaced  50fps & V-total is 1250 lines
	PINMUX_HDMIMODE_1920X1080I100       = 64 + 40,  ///< HDMI Video format is 1920x1080 & Interlaced  100fps
	PINMUX_HDMIMODE_1280X720P100        = 64 + 41,  ///< HDMI Video format is 1280X720  & Progressive 100fps
	PINMUX_HDMIMODE_720X576P100         = 64 + 42,  ///< HDMI Video format is 720X576   & Progressive 100fps
	PINMUX_HDMIMODE_720X576P100_16X9    = 64 + 43,  ///< HDMI Video format is 720X576   & Progressive 100fps & 16:9
	PINMUX_HDMIMODE_720X576I100         = 64 + 44,  ///< HDMI Video format is 720X576  & Interlaced 100fps
	PINMUX_HDMIMODE_720X576I100_16X9    = 64 + 45,  ///< HDMI Video format is 720X576  & Interlaced 100fps & 16:9
	PINMUX_HDMIMODE_1920X1080I120       = 64 + 46,  ///< HDMI Video format is 1920X1080 & Interlaced 120fps
	PINMUX_HDMIMODE_1280X720P120        = 64 + 47,  ///< HDMI Video format is 1280X720  & Progressive 120fps
	PINMUX_HDMIMODE_720X480P120         = 64 + 48,  ///< HDMI Video format is 720X480   & Progressive 120fps
	PINMUX_HDMIMODE_720X480P120_16X9    = 64 + 49,  ///< HDMI Video format is 720X480   & Progressive 120fps & 16:9
	PINMUX_HDMIMODE_720X480I120         = 64 + 50,  ///< HDMI Video format is 720X480  & Interlaced 120fps
	PINMUX_HDMIMODE_720X480I120_16X9    = 64 + 51,  ///< HDMI Video format is 720X480  & Interlaced 120fps & 16:9
	PINMUX_HDMIMODE_720X576P200         = 64 + 52,  ///< HDMI Video format is 720X576  & Progressive 200fps
	PINMUX_HDMIMODE_720X576P200_16X9    = 64 + 53,  ///< HDMI Video format is 720X576  & Progressive 200fps & 16:9
	PINMUX_HDMIMODE_720X576I200         = 64 + 54,  ///< HDMI Video format is 720X576  & Interlaced 200fps
	PINMUX_HDMIMODE_720X576I200_16X9    = 64 + 55,  ///< HDMI Video format is 720X576  & Interlaced 200fps & 16:9
	PINMUX_HDMIMODE_720X480P240         = 64 + 56,  ///< HDMI Video format is 720X480  & Progressive 240fps
	PINMUX_HDMIMODE_720X480P240_16X9    = 64 + 57,  ///< HDMI Video format is 720X480  & Progressive 240fps & 16:9
	PINMUX_HDMIMODE_720X480I240         = 64 + 58,  ///< HDMI Video format is 720X480  & Interlaced 240fps
	PINMUX_HDMIMODE_720X480I240_16X9    = 64 + 59,  ///< HDMI Video format is 720X480  & Interlaced 240fps & 16:9

	PINMUX_DSI_1_LANE_CMD_MODE_RGB565           = 128 + 0, ///< DSI command mode with RGB565 format
	PINMUX_DSI_1_LANE_CMD_MODE_RGB666P          = 128 + 1, ///< DSI command mode with RGB666 packed
	PINMUX_DSI_1_LANE_CMD_MODE_RGB666L          = 128 + 2, ///< DSI command mode with RGB666 loosely
	PINMUX_DSI_1_LANE_CMD_MODE_RGB888           = 128 + 3, ///< DSI command mode with RGB888

	PINMUX_DSI_1_LANE_VDO_SYNC_PULSE_RGB565     = 128 + 4, ///< DSI video sync pulse mode with RGB565 format
	PINMUX_DSI_1_LANE_VDO_SYNC_PULSE_RGB666P    = 128 + 5, ///< DSI video sync pulse mode with RGB666 packed
	PINMUX_DSI_1_LANE_VDO_SYNC_PULSE_RGB666L    = 128 + 6, ///< DSI video sync pulse mode with RGB666 loosely
	PINMUX_DSI_1_LANE_VDO_SYNC_PULSE_RGB888     = 128 + 7, ///< DSI video sync pulse mode with RGB888

	PINMUX_DSI_1_LANE_VDO_SYNC_EVENT_RGB565     = 128 + 8, ///< DSI video sync event burst mode with RGB565 format
	PINMUX_DSI_1_LANE_VDO_SYNC_EVENT_RGB666P    = 128 + 9, ///< DSI video sync event burst mode with RGB666 packed
	PINMUX_DSI_1_LANE_VDO_SYNC_EVENT_RGB666L    = 128 + 10, ///< DSI video sync event burst mode with RGB666 loosely
	PINMUX_DSI_1_LANE_VDO_SYNC_EVENT_RGB888     = 128 + 11, ///< DSI video sync event burst mode with RGB888

	PINMUX_DSI_2_LANE_CMD_MODE_RGB565           = 128 + 12, ///< DSI command mode with RGB565 format
	PINMUX_DSI_2_LANE_CMD_MODE_RGB666P          = 128 + 13, ///< DSI command mode with RGB666 packed
	PINMUX_DSI_2_LANE_CMD_MODE_RGB666L          = 128 + 14, ///< DSI command mode with RGB666 loosely
	PINMUX_DSI_2_LANE_CMD_MODE_RGB888           = 128 + 15, ///< DSI command mode with RGB888

	PINMUX_DSI_2_LANE_VDO_SYNC_PULSE_RGB565     = 128 + 16, ///< DSI video sync pulse mode with RGB565 format
	PINMUX_DSI_2_LANE_VDO_SYNC_PULSE_RGB666P    = 128 + 17, ///< DSI video sync pulse mode with RGB666 packed
	PINMUX_DSI_2_LANE_VDO_SYNC_PULSE_RGB666L    = 128 + 18, ///< DSI video sync pulse mode with RGB666 loosely
	PINMUX_DSI_2_LANE_VDO_SYNC_PULSE_RGB888     = 128 + 19, ///< DSI video sync pulse mode with RGB888

	PINMUX_DSI_2_LANE_VDO_SYNC_EVENT_RGB565     = 128 + 20, ///< DSI video sync event burst mode with RGB565 format
	PINMUX_DSI_2_LANE_VDO_SYNC_EVENT_RGB666P    = 128 + 21, ///< DSI video sync event burst mode with RGB666 packed
	PINMUX_DSI_2_LANE_VDO_SYNC_EVENT_RGB666L    = 128 + 22, ///< DSI video sync event burst mode with RGB666 loosely
	PINMUX_DSI_2_LANE_VDO_SYNC_EVENT_RGB888     = 128 + 23, ///< DSI video sync event burst mode with RGB888

	PINMUX_DSI_4_LANE_CMD_MODE_RGB565           = 128 + 24, ///< DSI command mode with RGB565 format
	PINMUX_DSI_4_LANE_CMD_MODE_RGB666P          = 128 + 25, ///< DSI command mode with RGB666 packed
	PINMUX_DSI_4_LANE_CMD_MODE_RGB666L          = 128 + 26, ///< DSI command mode with RGB666 loosely
	PINMUX_DSI_4_LANE_CMD_MODE_RGB888           = 128 + 27, ///< DSI command mode with RGB888

	PINMUX_DSI_4_LANE_VDO_SYNC_PULSE_RGB565     = 128 + 28, ///< DSI video sync pulse mode with RGB565 format
	PINMUX_DSI_4_LANE_VDO_SYNC_PULSE_RGB666P    = 128 + 29, ///< DSI video sync pulse mode with RGB666 packed
	PINMUX_DSI_4_LANE_VDO_SYNC_PULSE_RGB666L    = 128 + 30, ///< DSI video sync pulse mode with RGB666 loosely
	PINMUX_DSI_4_LANE_VDO_SYNC_PULSE_RGB888     = 128 + 31, ///< DSI video sync pulse mode with RGB888

	PINMUX_DSI_4_LANE_VDO_SYNC_EVENT_RGB565     = 128 + 32, ///< DSI video sync event burst mode with RGB565 format
	PINMUX_DSI_4_LANE_VDO_SYNC_EVENT_RGB666P    = 128 + 33, ///< DSI video sync event burst mode with RGB666 packed
	PINMUX_DSI_4_LANE_VDO_SYNC_EVENT_RGB666L    = 128 + 34, ///< DSI video sync event burst mode with RGB666 loosely
	PINMUX_DSI_4_LANE_VDO_SYNC_EVENT_RGB888     = 128 + 35, ///< DSI video sync event burst mode with RGB888



	ENUM_DUMMY4WORD(PINMUX_LCDINIT)
} PINMUX_LCDINIT;

/**
    PIN config for Parallel MI

    @note For pinmux_init() with PIN_FUNC_LCD or PIN_FUNC_LCD2.\n
            For example, you can use {PIN_FUNC_LCD, PINMUX_DISPMUX_SEL_LCD|PINMUX_PMI_CFG_NORMAL|PINMUX_LCDMODE_MI_FMT0}\n
            to tell display object that PIN_FUNC_LCD is bound to MI and format is FMT0,\n
            and MI is located at primary location.
*/
typedef enum {
	PINMUX_PMI_CFG_NORMAL = 0x00,                   ///< Normal Parallel MI location (at L_GPIO[0..22] depend on PINMUX_LCDMODE_MI_FMTx)
	PINMUX_PMI_CFG_2ND_PINMUX = 0x01 << 26,         ///< Secondary Parallel MI location (at L_GPIO[12..25] depend on PINMUX_LCDMODE_MI_FMTx)

	PINMUX_PMI_CFG_MASK = 0x03 << 26,
	ENUM_DUMMY4WORD(PINMUX_PMI_CFG)
} PINMUX_PMI_CFG;

/**
    PIN config for TV/HDMI

    @note For pinmux_init() for PIN_FUNC_TV or PIN_FUNC_HDMI.\n
            For example, you can use {PIN_FUNC_HDMI, PINMUX_TV_HDMI_CFG_NORMAL|PINMUX_HDMIMODE_1280X720P60}\n
            to tell display object that HDMI activation will disable PANEL,\n
            and HDMI mode is 1280x720 P60.
*/
typedef enum {
	PINMUX_TV_HDMI_CFG_GPIO = 0x00,                 ///< TV activation will disable PINMUX to GPIO
	PINMUX_TV_HDMI_CFG_NORMAL = 0x00,               ///< TV activation will disable PANEL which shared the same IDE
	PINMUX_TV_HDMI_CFG_PINMUX_ON = 0x01 << 28,      ///< TV activation will keep PINMUX setting

	PINMUX_TV_HDMI_CFG_MASK = 0x03 << 28,
	ENUM_DUMMY4WORD(PINMUX_TV_HDMI_CFG)
} PINMUX_TV_HDMI_CFG;

/**
    PIN config for HDMI

    @note For pinmux_init() for PIN_FUNC_HDMI.\n
            For example, you can use {PIN_FUNC_HDMI, PINMUX_HDMI_CFG_CEC|PINMUX_TV_HDMI_CFG_NORMAL}\n
            to declare HDMI CEC pinmux is enabled.
*/
typedef enum {
	PINMUX_HDMI_CFG_GPIO = 0x00,                    ///< HDMI specific PIN to GPIO
	PINMUX_HDMI_CFG_HOTPLUG = 0x01 << 26,           ///< HDMI HOTPLUG. Enable HDMI_PLUG (on P_GPIO[47])
	PINMUX_HDMI_CFG_CEC = 0x02 << 26,               ///< HDMI CEC. Enable HDMI_CEC (on P_GPIO[44])

	PINMUX_HDMI_CFG_MASK = 0x03 << 26,
	ENUM_DUMMY4WORD(PINMUX_HDMI_CFG)
} PINMUX_HDMI_CFG;

/**
    PIN location of LCD

    @note For pinmux_init() with PIN_FUNC_LCD or PIN_FUNC_LCD2.\n
            For example, you can use {PIN_FUNC_LCD, PINMUX_DISPMUX_SEL_LCD2|PINMUX_LCDMODE_XXX}\n
            to tell display object that PIN_FUNC_LCD is located on secondary LCD pinmux.
*/
typedef enum {
	PINMUX_DISPMUX_SEL_NONE = 0x00 << 28,           ///< PINMUX none
	PINMUX_DISPMUX_SEL_LCD = 0x01 << 28,            ///< PINMUX at LCD interface
	PINMUX_DISPMUX_SEL_LCD2 = 0x02 << 28,           ///< PINMUX at LCD2 interface

	PINMUX_DISPMUX_SEL_MASK = 0x03 << 28,
	ENUM_DUMMY4WORD(PINMUX_DISPMUX_SEL)
} PINMUX_DISPMUX_SEL;

/**
    PIN config for ETH

    @note For pinmux_init() for PIN_FUNC_ETH.\n
            For example, you can use {PIN_FUNC_ETH, PINMUX_ETH_CFG_MII}\n
            to select USB as device.
*/
typedef enum {
	PIN_ETH_CFG_NONE  = 0x00,                        ///< PINMUX none
	PIN_ETH_CFG_MII   = 0x01,                        ///< Enable ETH MII on L_GPIO[11..26]/L_GPIO[28..29]
	PIN_ETH_CFG_RMII  = 0x02,                        ///< Enable ETH RMII on L_GPIO[12..14]/L_GPIO[17]/L_GPIO[19]/L_GPIO[22..23]/L_GPIO[27]/L_GPIO[28..29]
	PIN_ETH_CFG_GMII  = 0x04,                        ///< Enable ETH GMII on L_GPIO[0..3]/L_GPIO[7..27]/L_GPIO[28..29] 
	PIN_ETH_CFG_RGMII = 0x08,                        ///< Enable ETH RGMII on L_GPIO[12..16]/L_GPIO[22..26]/L_GPIO[27]/L_GPIO[28..29]
	PIN_ETH_CFG_REVMII_10_100  = 0x10,               ///< Enable ETH REVMII_10_100 on L_GPIO[11..26]/L_GPIO[28..29]
	PIN_ETH_CFG_REVMII_10_1000 = 0x20,               ///< Enable ETH REVMII_10_1000 on L_GPIO[0..3]/L_GPIO[7..27]/L_GPIO[28..29] 

	ENUM_DUMMY4WORD(PINMUX_ETH_CFG)
} PINMUX_ETH_CFG;

/**
    PIN config for CANBUS

    @note For pinmux_init() for PIN_FUNC_CANBUS.\n
            For example, you can use {PIN_FUNC_CANBUS,  PIN_CANBUS_CFG_CANBUS}\n
            to select CANBUS as device.
*/
typedef enum {
	PIN_CANBUS_CFG_NONE     = 0x00,                        ///< PINMUX none
	PIN_CANBUS_CFG_CANBUS   = 0x01,                        ///< Enable CANBUS CAN_TXD/CAN_RXD/CAN_STBY on P_GPIO[31..33] 

	ENUM_DUMMY4WORD(PINMUX_CANBUS_CFG)
} PINMUX_CANBUS_CFG;


/**
    Pinmux Function identifier

    @note For pinmux_getDispMode(), pinmux_setPinmux().
*/
typedef enum {
	PINMUX_FUNC_ID_LCD,                             ///< 1st Panel (LCD), pinmux can be:
	///< - @b PINMUX_LCD_SEL_GPIO
	///< - @b PINMUX_LCD_SEL_CCIR656
	///< - @b PINMUX_LCD_SEL_CCIR656_16BITS
	///< - @b PINMUX_LCD_SEL_CCIR601
	///< - @b PINMUX_LCD_SEL_CCIR601_16BITS
	///< - @b PINMUX_LCD_SEL_SERIAL_RGB_6BITS
	///< - @b PINMUX_LCD_SEL_SERIAL_RGB_8BITS
	///< - @b PINMUX_LCD_SEL_SERIAL_YCbCr_8BITS
	///< - @b PINMUX_LCD_SEL_PARALLE_RGB565
	///< - @b PINMUX_LCD_SEL_PARALLE_RGB666
	///< - @b PINMUX_LCD_SEL_PARALLE_RGB888
	///< - @b PINMUX_LCD_SEL_RGB_16BITS
	///< - @b PINMUX_LCD_SEL_PARALLE_MI_8BITS
	///< - @b PINMUX_LCD_SEL_PARALLE_MI_9BITS
	///< - @b PINMUX_LCD_SEL_PARALLE_MI_16BITS
	///< - @b PINMUX_LCD_SEL_PARALLE_MI_18BITS
	///< - @b PINMUX_LCD_SEL_SERIAL_MI_SDIO
	///< - @b PINMUX_LCD_SEL_SERIAL_MI_SDI_SDO
	///< ORed with
	///< - @b PINMUX_LCD_SEL_DE_ENABLE
	///< - @b PINMUX_LCD_SEL_TE_ENABLE
	PINMUX_FUNC_ID_LCD2,                            ///< 2nd Panel (LCD), pinmux can be:
	///< - @b PINMUX_LCD_SEL_GPIO
	///< - @b PINMUX_LCD_SEL_CCIR656
	///< - @b PINMUX_LCD_SEL_CCIR601
	///< - @b PINMUX_LCD_SEL_SERIAL_RGB_6BITS
	///< - @b PINMUX_LCD_SEL_SERIAL_RGB_8BITS
	///< - @b PINMUX_LCD_SEL_SERIAL_YCbCr_8BITS
	///< - @b PINMUX_LCD_SEL_PARALLE_MI_8BITS
	///< - @b PINMUX_LCD_SEL_PARALLE_MI_9BITS
	///< - @b PINMUX_LCD_SEL_SERIAL_MI_SDIO
	///< - @b PINMUX_LCD_SEL_SERIAL_MI_SDI_SDO
	///< ORed with
	///< - @b PINMUX_LCD_SEL_DE_ENABLE
	PINMUX_FUNC_ID_TV,                              ///< TV, pinmux can be:
	///< - @b PINMUX_LCD_SEL_GPIO
	PINMUX_FUNC_ID_HDMI,                            ///< HDMI, pinmux can be:
	///< - @b PINMUX_LCD_SEL_GPIO
	PINMUX_FUNC_ID_SN_MES0,                         ///< Sensor MES0, pinmux can be:
	///< - @b PINMUX_SENSOR_SEL_INACTIVE
	///< - @b PINMUX_SENSOR_SEL_ACTIVE
	PINMUX_FUNC_ID_SN_MES1,                         ///< Sensor MES1, pinmux can be:
	///< - @b PINMUX_SENSOR_SEL_INACTIVE
	///< - @b PINMUX_SENSOR_SEL_ACTIVE
	PINMUX_FUNC_ID_SN_MES2,                         ///< Sensor MES2, pinmux can be:
	///< - @b PINMUX_SENSOR_SEL_INACTIVE
	///< - @b PINMUX_SENSOR_SEL_ACTIVE
	PINMUX_FUNC_ID_SN_MES3,                         ///< Sensor MES3, pinmux can be:
	///< - @b PINMUX_SENSOR_SEL_INACTIVE
	///< - @b PINMUX_SENSOR_SEL_ACTIVE
	PINMUX_FUNC_ID_SN_FLCTR,                        ///< Sensor Flash Control, pinmux can be:
	///< - @b PINMUX_SENSOR_SEL_INACTIVE
	///< - @b PINMUX_SENSOR_SEL_ACTIVE
	PINMUX_FUNC_ID_SN_MCLK,                         ///< Sensor MCLK, pinmux can be:
	///< - @b PINMUX_SENSOR_SEL_INACTIVE
	///< - @b PINMUX_SENSOR_SEL_ACTIVE
	PINMUX_FUNC_ID_SPI2,                            ///< SPI channel2 pinmux switch, pinmux can be:
	///< - @b PINMUX_SPI_SEL_INACTIVE
	///< - @b PINMUX_SPI_SEL_ACTIVE
	PINMUX_FUNC_ID_SPI3,                            ///< SPI channel3 pinmux switch, pinmux can be:
	///< - @b PINMUX_SPI_SEL_INACTIVE
	///< - @b PINMUX_SPI_SEL_ACTIVE
	PINMUX_FUNC_ID_SPI4,                            ///< SPI channel4 pinmux switch, pinmux can be:
	///< - @b PINMUX_SPI_SEL_INACTIVE
	///< - @b PINMUX_SPI_SEL_ACTIVE
	PINMUX_FUNC_ID_SPI5,                            ///< SPI channel5 pinmux switch, pinmux can be:
	///< - @b PINMUX_SPI_SEL_INACTIVE
	///< - @b PINMUX_SPI_SEL_ACTIVE
	PINMUX_FUNC_ID_COUNT,                           //< Total function count

	ENUM_DUMMY4WORD(PINMUX_FUNC_ID)
} PINMUX_FUNC_ID;

/**
    Pinmux selection for LCD

    @note For PINMUX_FUNC_ID_LCD, PINMUX_FUNC_ID_LCD2
*/
typedef enum {
	PINMUX_LCD_SEL_GPIO,                            ///< GPIO
	PINMUX_LCD_SEL_CCIR656,                         ///< CCIR-656 8 bits. 
	///< When PINMUX_FUNC_ID_LCD, Enable CCIR_YC[0..7]/CCIR_CLK on L_GPIO[0..8]
	///< When PINMUX_FUNC_ID_LCD2, Enable CCIR2_YC[0..7]/CCIR2_CLK on L_GPIO[12..20]
	PINMUX_LCD_SEL_CCIR656_16BITS,                  ///< CCIR-656 16 bits. Enable CCIR_Y[0..7]/CCIR_CLK/CCIR_C[0..7] on L_GPIO[0..8]/L_GPIO[12..19]
	PINMUX_LCD_SEL_CCIR601,                         ///< CCIR-601 8 bits. 
	///< When PINMUX_FUNC_ID_LCD, Enable CCIR_YC[0..7]/CCIR_CLK/CCIR_VD/CCIR_HD on L_GPIO[0..10]
	///< When PINMUX_FUNC_ID_LCD2, Enable CCIR2_YC[0..7]/CCIR2_CLK/CCIR2_VD/CCIR2_HD on L_GPIO[12..22] 
	PINMUX_LCD_SEL_CCIR601_16BITS,                  ///< CCIR-601 16 bits. Enable CCIR_Y[0..7]/CCIR_CLK/CCIR_VD/CCIR_HD/CCIR_C[0..7] on L_GPIO[0..10]/L_GPIO[12..19]
	PINMUX_LCD_SEL_SERIAL_RGB_6BITS,                ///< Serial RGB 6 bits. 
	///< When PINMUX_FUNC_ID_LCD, Enable RGB_D[2..7]/RGB_CLK/RGB_VD/RGB_HD on L_GPIO[2..10]
	///< When PINMUX_FUNC_ID_LCD2, Enable RGB2_D[2..7]/RGB2_CLK/RGB2_VD/RGB2_HD on L_GPIO[14..22]
	PINMUX_LCD_SEL_SERIAL_RGB_8BITS,                ///< Serial RGB 8 bits.
	///< When PINMUX_FUNC_ID_LCD, Enable RGB_D[0..7]/RGB_CLK/RGB_VD/RGB_HD on L_GPIO[0..10]
	///< When PINMUX_FUNC_ID_LCD2, Enable RGB2_D[0..7]/RGB2_CLK/RGB2_VD/RGB2_HD on L_GPIO[12..22]
	PINMUX_LCD_SEL_SERIAL_YCbCr_8BITS,              ///< Serial YCbCr 8 bits.
	///< When PINMUX_FUNC_ID_LCD, Enable CCIR_YC[0..7]/CCIR_CLK/CCIR_VD/CCIR_HD on L_GPIO[0..10]
	///< When PINMUX_FUNC_ID_LCD2, Enable CCIR2_YC[0..7]/CCIR2_CLK/CCIR2_VD/CCIR2_HD on L_GPIO[12..22]
	PINMUX_LCD_SEL_PARALLE_RGB565,                  ///< Parallel RGB565. Enable RGB_C0_[0..4]/RGB_DCLK/RGB_VS/RGB_HS/RGB_C1_[0..5]/RGB_C2_[0..4] on L_GPIO[3..10]/L_GPIO[14..19]/L_GPIO[23..27]
	PINMUX_LCD_SEL_PARALLE_RGB666,                  ///< Parallel RGB666. Enable RGB_C0_[0..5]/RGB_DCLK/RGB_VS/RGB_HS/RGB_C1_[0..5]/RGB_C2_[0..5] on L_GPIO[2..10]/L_GPIO[14..19]/L_GPIO[22..27]
	PINMUX_LCD_SEL_PARALLE_RGB888,                  ///< Parallel RGB888. Enable RGB_C0_[0..7]/RGB_DCLK/RGB_VS/RGB_HS/RGB_C1_[0..7]/RGB_C2_[0..7] on L_GPIO[0..10]/L_GPIO[12..27]
	PINMUX_LCD_SEL_RGB_16BITS,                      ///< RGB 16 bits. Enable CCIR_Y[0..7]/CCIR_CLK/CCIR_VD/CCIR_HD/CCIR_C[0..7] on L_GPIO[0..10]/L_GPIO[12..19]
	PINMUX_LCD_SEL_PARALLE_MI_8BITS,                ///< Parallel MI 8 bits.
	///< When PINMUX_PMI_CFG_NORMAL is set, Enable MPU_D[0..7]/MPU_CS/MPU_RS/MPU_WR/MPU_RD on L_GPIO[0..7]/L_GPIO[9..12]
	///< When PINMUX_PMI_CFG_2ND_PINMUX is set, Enable MPU2_RS/MPU2_CS/MPU2_D[0..7]/MPU2_WR/MPU2_RD on L_GPIO[12..22]/L_GPIO[24]
	PINMUX_LCD_SEL_PARALLE_MI_9BITS,                ///< Parallel MI 9 bits.
	///< When PINMUX_PMI_CFG_NORMAL is set, Enable MPU_D[0..8]/MPU_CS/MPU_RS/MPU_WR/MPU_RD on L_GPIO[0..7]/L_GPIO[9..13]
	///< When PINMUX_PMI_CFG_2ND_PINMUX is set, Enable MPU2_RS/MPU2_CS/MPU2_D[0..8]/MPU2_WR/MPU2_RD on L_GPIO[12..24]
	PINMUX_LCD_SEL_PARALLE_MI_16BITS,               ///< Parallel MI 16 bits. Enable MPU_D[0..15]/MPU_CS/MPU_RS/MPU_WR/MPU_RD on L_GPIO[0..7]/L_GPIO[9..20]
	PINMUX_LCD_SEL_PARALLE_MI_18BITS,               ///< Parallel MI 18 bits. Enable MPU_D[0..17]/MPU_CS/MPU_RS/MPU_WR/MPU_RD on L_GPIO[0..7]/L_GPIO[9..22]
	PINMUX_LCD_SEL_SERIAL_MI_SDIO,                  ///< Serial MI SDIO bi-direction. Enable MPU_CS/MPU_RS/MPU_CLK/MPU_SDIO L_GPIO[15..18]
	PINMUX_LCD_SEL_SERIAL_MI_SDI_SDO,               ///< Serial MI SDI/SDO seperate. Enable MPU_SDO/MPU_SDI/MPU_CS/MPU_RS/MPU_CLK on L_GPIO[13..17]
	PINMUX_LCD_SEL_MIPI,                            ///< MIPI DSI

	PINMUX_LCD_SEL_TE_ENABLE = 0x01 << 23,          ///< TE Enable (For Parallel/Serial MI)
	///< When normal (PINMUX_PMI_CFG_NORMAL) parallel MI, enable MPU_TE on L_GPIO[8].
	///< When 2nd (PINMUX_PMI_CFG_2ND_PINMUX) parallel MI, enable MPU_TE on L_GPIO[25].
	///< When serial MI, enable MI_TE on L_GPIO[19].
	PINMUX_LCD_SEL_DE_ENABLE = 0x01 << 24,          ///< DE Enable (For CCIR656 8/16bits, CCIR601 8/16bits, Serial RGB 6/8bits, Serial YCbCr 8 bits, RGB565/RGB666/RGB888, RGB 16 bits)
	///< When corresponding item in pinmux_init() is set with PINMUX_DISPMUX_SEL_LCD, enable CCIR_DE L_GPIO[11]
	///< When corresponding item in pinmux_init() is set with PINMUX_DISPMUX_SEL_LCD2, enable CCIR2_DE L_GPIO[23]
	PINMUX_LCD_SEL_HVLD_VVLD = 0x01 << 25,          ///< HVLD/VVLD Enable (For CCIR601 8 bits). Enable CCIR_HVLD/CCIR_VVLD on L_GPIO[12..13]
	PINMUX_LCD_SEL_FIELD = 0x01 << 26,              ///< FIELD Enable (For CCIR601 8 bits). Enable CCIR_FIELD on L_GPIO[14]

	PINMUX_LCD_SEL_FEATURE_MSK = 0x0F << 23,

	ENUM_DUMMY4WORD(PINMUX_LCD_SEL)
} PINMUX_LCD_SEL;

/**
    Pinmux selection for Storage

    @note For PINMUX_FUNC_ID_SDIO, PINMUX_FUNC_ID_SPI, PINMUX_FUNC_ID_NAND
*/
typedef enum {
	PINMUX_STORAGE_SEL_INACTIVE,                    ///< Inactive storage
	PINMUX_STORAGE_SEL_ACTIVE,                      ///< Active storage
	PINMUX_STORAGE_SEL_INEXIST,                     ///< Inexist storage
	PINMUX_STORAGE_SEL_EXIST,                       ///< Exist storage
	ENUM_DUMMY4WORD(PINMUX_STORAGE_SEL)
} PINMUX_STORAGE_SEL;

/**
    Pinmux selection for sensor

    @note For PINMUX_FUNC_ID_SN_MES0, PINMUX_FUNC_ID_SN_MES1, PINMUX_FUNC_ID_SN_FLCTR
*/
typedef enum {
	PINMUX_SENSOR_SEL_INACTIVE,                     ///< Inactive
	PINMUX_SENSOR_SEL_ACTIVE,                       ///< Active
	ENUM_DUMMY4WORD(PINMUX_SENSOR_SEL)
} PINMUX_SENSOR_SEL;

/**
    Pinmux selection for sensor

    @note For PINMUX_FUNC_ID_USB_VBUSI
*/
typedef enum {
	PINMUX_USB_SEL_INACTIVE,                        ///< Inactive
	PINMUX_USB_SEL_ACTIVE,                          ///< Active
	ENUM_DUMMY4WORD(PINMUX_USB_SEL)
} PINMUX_USB_SEL;

/**
    Pinmux selection for SPI

    @note For PINMUX_FUNC_ID_SPI2, PINMUX_FUNC_ID_SPI3
*/
typedef enum {
	PINMUX_SPI_SEL_INACTIVE,                        ///< Inactive
	PINMUX_SPI_SEL_ACTIVE,                          ///< Active
	ENUM_DUMMY4WORD(PINMUX_SPI_SEL)
} PINMUX_SPI_SEL;

/**
    Pinmux group

    @note For pinmux_init()
*/
typedef struct {
	PIN_FUNC    pinFunction;            ///< PIN Function group
	UINT32      config;                 ///< Configuration for pinFunction
} PIN_GROUP_CONFIG;

//
// API for Top controller
//
extern void     pinmux_select_debugport(PINMUX_DEBUGPORT uiDebug);
extern void     pinmux_select_ocp_debugport(PINMUX_DEBUGPORT_OCP uiDebug);

extern ER       pinmux_init(PIN_GROUP_CONFIG *pConfig);
extern ER       pinmux_setDispMode(PINMUX_FUNC_ID id, PINMUX_LCDINIT dispMode);
extern PINMUX_LCDINIT pinmux_getDispMode(PINMUX_FUNC_ID id);
extern ER       pinmux_setPinmux(PINMUX_FUNC_ID id, UINT32 pinmux);
extern BOOL     pinmux_chkPinmux(PINMUX_FUNC_ID id, UINT32 pinmux);
extern void     pinmux_init_for_dualcore(PIN_GROUP_CONFIG *pConfig, BOOL enable);

//@}    //addtogroup mIHALSysCfg

#endif

