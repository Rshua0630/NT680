/**
    Header file for Interrupt module

    This file is the header file that define the API for Interrupt module.

    @file       Interrupt.h
    @ingroup    mIDrvSys_Interrupt
    @note       Nothing.

    Copyright   Novatek Microelectronics Corp. 2010.  All rights reserved.
*/

#ifndef _INTERRUPT_H
#define _INTERRUPT_H

#if defined(__UITRON)
#include "Type.h"
#include "cc.h"
#else
#include "nvt_type.h"
typedef enum {
	CC_CORE_CA53_CORE1 = 0x0,               ///< 1st Core MIPS1
	CC_CORE_CA53_CORE2,                     ///< 2nd Core MIPS2

	CC_CORE_CA53_CORE_NUM,
	CC_CORE_DSP = 0x2,                      ///< 1st Core DSP XM4_1
	CC_CORE_DSP2,                           ///< 1st Core DSP XM4_2

	CC_CORE_DSP_NUM = 0x2,
} CC_CORE_ID;
#define _ARM_GIC_USAGE_             ENABLE
#endif

#ifdef __ECOS
#define GIC_USE_FIQ                 ENABLE //eCos ise IRQ first
#else
#define GIC_USE_FIQ                 ENABLE
#endif
//#define GIC_USE_FIQ               DISABLE

// Interrupt number <= 32
//typedef UINT32          INT_PTN;
// Interrupt number > 32
typedef UINT64          INT_PTN;

/**
    @addtogroup mIDrvSys_Interrupt
*/
//@{
#define INT_GIC_ID_DST_VER 0x16080216


/**
    Interrupt configuration identifier

    @note For int_setConfig()
*/
typedef enum {
	INT_CONFIG_ID_GIC_DESTINATION = 0,              ///< Configured Interrupt exception destination to CPU/CPU2
	///< @note
	///< Context is :
	///< - @b PINT_ID_GIC_DST : Point of destination table

	INT_CONFIG_ID_INTC_DESTINATION,                 ///< Configured Interrupt exception destination to DSP/DSP2
	///< @note
	///< Context is :
	///< - @b PINT_ID_INTC_DST : Point of destination table

	INT_CONFIG_ID_SDIO_GIC_DESTINATION,             ///< Configured SDIO Interrupt destination to Core1 or Core2exception destination to DSP/DSP2
	///< @note
	///< Context is :
	///< - @b CC_CORE_ID : CC_CORE_CA53_CORE1 or CC_CORE_CA53_CORE2

	INT_CONFIG_ID_USB3_GIC_DESTINATION,             ///< Configured USB3 Interrupt destination to Core1 or Core2exception destination to DSP/DSP2
	///< @note
	///< Context is :
	///< - @b CC_CORE_ID : CC_CORE_CA53_CORE1 or CC_CORE_CA53_CORE2

	INT_CONFIG_ID_USB2_GIC_DESTINATION,             ///< Configured USB2 Interrupt destination to Core1 or Core2exception destination to DSP/DSP2
	///< @note
	///< Context is :
	///< - @b CC_CORE_ID : CC_CORE_CA53_CORE1 or CC_CORE_CA53_CORE2


    INT_CONFIG_ID_GET_CPU_INT_EN,                   ///< Get CPU Interrupt Enable or not
	///< @note
	///< Context is :
	///< - @b INT_ID : Specific Interrupt ID

    INT_CONFIG_ID_GET_CPU_DESTINATION,              ///< Get CPU Interrupt Destination
	///< @note
	///< Context is :
	///< return
	///< - @b 0 : None target configed
	///< - @b 1 : CC_CORE_CA53_CORE1 : Destination to core1
	///< - @b 2 : CC_CORE_CA53_CORE2 : Destination to core2


	ENUM_DUMMY4WORD(INT_CONFIG_ID)
} INT_CONFIG_ID;

/**
    Interrupt module ID

    Interrupt module ID for int_getIRQId() and int_getDummyId().
*/
#if(_ARM_GIC_USAGE_ == DISABLE)
#define INT_GIC_SPI_START_ID    0
#else
#define INT_GIC_SPI_START_ID    32
#endif

typedef enum {
	INT_ID_TIMER = INT_GIC_SPI_START_ID,
	INT_ID_SIE,
	INT_ID_SIE2,
	INT_ID_SIE3,

	INT_ID_SIE4,
	INT_ID_IPE,
	INT_ID_IME,
	INT_ID_DCE,

	INT_ID_IFE,
	INT_ID_IFE2,
	INT_ID_DIS,
	INT_ID_FDE,

//  INT_ID_RDE,             //Removed @ NT96680
	INT_ID_ETH_LPI,         //Ethernet_LPI
	INT_ID_RHE,
	INT_ID_DRE,
	INT_ID_DAI,             //15

	INT_ID_H26X,
	INT_ID_JPEG,
	INT_ID_GRAPHIC,
	INT_ID_GRAPHIC2,

	INT_ID_AFFINE,
	INT_ID_ISE,
	INT_ID_TGE,
	INT_ID_TSMUX,

	INT_ID_GPIO,
	INT_ID_REMOTE,
	INT_ID_PWM,
	INT_ID_USB,

	INT_ID_USB3,
	INT_ID_NAND,
	INT_ID_SDIO,
	INT_ID_SDIO2,           //31

	INT_ID_SDIO3,
	INT_ID_DMA,
	INT_ID_ETHERNET,
	INT_ID_SPI,

	INT_ID_SPI2,
	INT_ID_SPI3,
	INT_ID_SPI4,
//  INT_ID_SPI5,            //Removed @ NT96680
	INT_ID_ETH_REV_MII,     //Ethernet_RevMII

	INT_ID_SIF,
	INT_ID_I2C,
	INT_ID_I2C2,
	INT_ID_UART,            //43

	INT_ID_UART2,
	INT_ID_UART3,
	INT_ID_UART4,
	INT_ID_ADC,

	INT_ID_IDE,
	INT_ID_IDE2,
	INT_ID_DSI,
	INT_ID_MI,              //51

	INT_ID_HDMI,
//  INT_ID_VX1,             //Removed @ NT96680
	INT_ID_DMA2,            //New @ NT96680
	INT_ID_LVDS,
	INT_ID_LVDS2,           //55

	INT_ID_RTC,
	INT_ID_WDT,
	INT_ID_CG,
	INT_ID_CC,              //59

	INT_ID_I2C3,            //60
//  New add @ NT96680
	INT_ID_SLVS_EC,
	INT_ID_LVDS3,           //share with HiSPi3/CSI3
	INT_ID_LVDS4,           //share with HiSPi4/CSI4

	INT_ID_LVDS5,           //share with HiSPi5/CSI5
	INT_ID_LVDS6,           //share with HiSPi6/CSI6
	INT_ID_LVDS7,           //share with HiSPi7/CSI7
	INT_ID_LVDS8,           //share with HiSPi8/CSI8

	INT_ID_SIE5,
	INT_ID_SIE6,
	INT_ID_SIE7,
	INT_ID_SIE8,

	INT_ID_IVE,
	INT_ID_SVM,
	INT_ID_SDE,
	INT_ID_CNN,             //75

	INT_ID_DSP,
	INT_ID_DSP2,
	INT_ID_CANBUS,
	INT_ID_CRYPTO,          //79

	INT_ID_COPY,
	INT_ID_ROTATE,
	INT_ID_I2C4,
	INT_ID_I2C5,            //83

//  INT_ID_I2C6,
//  INT_ID_I2C7,
	INT_ID_DRTC,
	INT_ID_ISE2,
	INT_ID_TIMER2,
	INT_ID_GPIO2,           //87

	INT_ID_PWM2,
	INT_ID_CC2,             //89
	INT_ID_SIF2,

	INT_ID_DSP_2,
	INT_ID_DSP_2_2,
	INT_ID_TSDEMUX,         //93

	INT_ID_CNT,
	INT_ID_MAX = INT_ID_CNT - INT_GIC_SPI_START_ID, //94


	INT_ID_WFI = 229,
	INT_ID_GIC_TOTAL = 256,
	ENUM_DUMMY4WORD(INT_ID)
} INT_ID;

typedef enum {
	INT_TARGET_CORE0 = 1,
	INT_TARGET_CORE1,
	ENUM_DUMMY4WORD(INT_CORE_ID)
} INT_CORE_ID;

/*
    Interrupt module ID of GIC

    Indicate which core will configured as destination

    @note For DMA_WRITEPROT_ATTR
*/


typedef union {
	INT_PTN Reg[2];
	struct {
		//INT0
		UINT32  bInt_ID_TIMER: 1;
		UINT32  bInt_ID_SIE: 1;
		UINT32  bInt_ID_SIE2: 1;
		UINT32  bInt_ID_SIE3: 1;

		UINT32  bInt_ID_SIE4: 1;
		UINT32  bInt_ID_IPE: 1;
		UINT32  bInt_ID_IME: 1;
		UINT32  bInt_ID_DCE: 1;

		UINT32  bInt_ID_IFE: 1;
		UINT32  bInt_ID_IFE2: 1;
		UINT32  bInt_ID_DIS: 1;
		UINT32  bInt_ID_FDE: 1;

//      UINT32  bInt_ID_RDE:1;
		UINT32  bInt_ID_Ethernet_LPI: 1;
		UINT32  bInt_ID_RHE: 1;
		UINT32  bInt_ID_DRE: 1;
		UINT32  bInt_ID_DAI: 1;


		//INT16
		UINT32  bInt_ID_H264: 1;
		UINT32  bInt_ID_JPEG: 1;
		UINT32  bInt_ID_GRAPHIC: 1;
		UINT32  bInt_ID_GRAPHIC2: 1;

		UINT32  bInt_ID_AFFINE: 1;
		UINT32  bInt_ID_ISE: 1;
		UINT32  bInt_ID_TGE: 1;
		UINT32  bInt_ID_TSMUX: 1;

		UINT32  bInt_ID_GPIO: 1;
		UINT32  bInt_ID_REMOTE: 1;
		UINT32  bInt_ID_PWM: 1;
		UINT32  bInt_ID_USB: 1;

		UINT32  bInt_ID_USB3: 1;
		UINT32  bInt_ID_NAND: 1;
		UINT32  bInt_ID_SDIO: 1;
		UINT32  bInt_ID_SDIO2: 1;

		//INT32
		UINT32  bInt_ID_SDIO3: 1;
		UINT32  bInt_ID_DMA: 1;
		UINT32  bInt_ID_ETHERNET: 1;
		UINT32  bInt_ID_SPI: 1;

		UINT32  bInt_ID_SPI2: 1;
		UINT32  bInt_ID_SPI3: 1;
		UINT32  bInt_ID_SPI4: 1;
		UINT32  bInt_ID_Ehternet_RevMII: 1;
//      UINT32  bInt_ID_SPI5:1;


		UINT32  bInt_ID_SIF: 1;
		UINT32  bInt_ID_I2C: 1;
		UINT32  bInt_ID_I2C2: 1;
		UINT32  bInt_ID_UART: 1;

		UINT32  bInt_ID_UART2: 1;
		UINT32  bInt_ID_UART3: 1;
		UINT32  bInt_ID_UART4: 1;
		UINT32  bInt_ID_ADC: 1;

		//INT48
		UINT32  bInt_ID_IDE: 1;
		UINT32  bInt_ID_IDE2: 1;
		UINT32  bInt_ID_DSI: 1;
		UINT32  bInt_ID_MI: 1;

		//INT52
		UINT32  bInt_ID_HDMI: 1;
//      UINT32  bInt_ID_VX1:1;
		UINT32  bInt_ID_DMA2: 1;
		UINT32  bInt_ID_LVDS: 1;        // LVDS& HiSPI& CSI
		UINT32  bInt_ID_LVDS2: 1;       //LVDS2&HiSPI2&CSI2

		//INT56
		UINT32  bInt_ID_RTC: 1;
		UINT32  bInt_ID_WDT: 1;
		UINT32  bInt_ID_CG: 1;
		UINT32  bInt_ID_CC: 1;

		//INT60
		UINT32  bInt_ID_I2C3: 1;
		UINT32  bInt_ID_SLVS_EC: 1;
		UINT32  bInt_ID_LVDS3: 1;       //LVDS3&HiSPI3&CSI3
		UINT32  bInt_ID_LVDS4: 1;       //LVDS4&HiSPI4&CSI4

		//INT64
		UINT32  bInt_ID_LVDS5: 1;       //LVDS5&HiSPI5&CSI5
		UINT32  bInt_ID_LVDS6: 1;       //LVDS6&HiSPI6&CSI6
		UINT32  bInt_ID_LVDS7: 1;       //LVDS7&HiSPI7&CSI7
		UINT32  bInt_ID_LVDS8: 1;       //LVDS8&HiSPI8&CSI8

		//INT68
		UINT32  bInt_ID_SIE5: 1;
		UINT32  bInt_ID_SIE6: 1;
		UINT32  bInt_ID_SIE7: 1;
		UINT32  bInt_ID_SIE8: 1;

		//INT72
		UINT32  bInt_ID_IVE: 1;
		UINT32  bInt_ID_SVM: 1;
		UINT32  bInt_ID_SDE: 1;
		UINT32  bInt_ID_CNN: 1;

		//INT76
		UINT32  bInt_ID_DSP: 1;
		UINT32  bInt_ID_DSP2: 1;
		UINT32  bInt_ID_CANBUS: 1;
		UINT32  bInt_ID_CRYPTO: 1;

		//INT80
		UINT32  bInt_ID_HWCOPY: 1;
		UINT32  bInt_ID_ROTATE: 1;
		UINT32  bInt_ID_I2C4: 1;
		UINT32  bInt_ID_I2C5: 1;

		//INT84
		UINT32  bInt_ID_I2C6: 1;
		UINT32  bInt_ID_I2C7: 1;
		UINT32  bInt_ID_TIMER2: 1;      //Timer to CA53 core2
		UINT32  bInt_ID_GPIO2: 1;       //GPIO to CA53 core2

		//INT88
		UINT32  bInt_ID_PWM2: 1;
		UINT32  bInt_ID_CC2: 1;
		UINT32  bInt_ID_SIF2: 1;        //Timer to CA53 core2
		UINT32  bInt_ID_DSP_2: 1;       //??

		//INT92
		UINT32  bInt_ID_DSP2_2: 1;      //??
		UINT32  bInt_ID_TMDEMUX: 1;
		UINT32  bReserved92: 2;
		//INT96 => 4
		UINT32  bReserved96: 4;
		//INT100=> 4
		UINT32  bReserved100: 4;
		//INT104=> 4
		UINT32  bReserved104: 4;
		//INT108=> 4
		UINT32  bReserved108: 4;
		//INT112=> 4
		UINT32  bReserved112: 4;
		//INT116=> 4
		UINT32  bReserved116: 4;
		//INT120=> 4
		UINT32  bReserved120: 4;
		//INT124=> 4
		UINT32  bReserved124: 4;
	} Bit;
} INT_GIC_ID_DST, *PINT_GIC_ID_DST;

/*
    Interrupt module ID of INTC

    Indicate which core will configured as destination

    @note For DMA_WRITEPROT_ATTR
*/


typedef union {
	INT_PTN Reg[2];
	struct {
		//INT0
		UINT32  bInt_ID_TIMER: 1;
		UINT32  bInt_ID_SIE: 1;
		UINT32  bInt_ID_SIE2: 1;
		UINT32  bInt_ID_SIE3: 1;

		UINT32  bInt_ID_SIE4: 1;
		UINT32  bInt_ID_IPE: 1;
		UINT32  bInt_ID_IME: 1;
		UINT32  bInt_ID_DCE: 1;

		UINT32  bInt_ID_IFE: 1;
		UINT32  bInt_ID_IFE2: 1;
		UINT32  bInt_ID_DIS: 1;
		UINT32  bInt_ID_FDE: 1;

		//INT12
		UINT32  bReserved12: 1;
		UINT32  bInt_ID_RHE: 1;
		UINT32  bInt_ID_DRE: 1;
		UINT32  bInt_ID_DAI: 1;


		//INT16
		UINT32  bInt_ID_H264: 1;
		UINT32  bInt_ID_JPEG: 1;
		UINT32  bInt_ID_GRAPHIC: 1;
		UINT32  bInt_ID_GRAPHIC2: 1;

		//INT20
		UINT32  bInt_ID_AFFINE: 1;
		UINT32  bInt_ID_ISE: 1;
		UINT32  bInt_ID_TGE: 1;
		UINT32  bInt_ID_TSMUX: 1;

		UINT32  bInt_ID_GPIO: 1;
		UINT32  bInt_ID_REMOTE: 1;
		UINT32  bInt_ID_PWM: 1;
		UINT32  bInt_ID_USB: 1;

		UINT32  bInt_ID_USB3: 1;
		UINT32  bInt_ID_NAND: 1;
		UINT32  bInt_ID_SDIO: 1;
		UINT32  bInt_ID_SDIO2: 1;

		//INT32
		UINT32  bInt_ID_SDIO3: 1;
		UINT32  bInt_ID_DMA: 1;
		UINT32  bInt_ID_ETHERNET: 1;
		UINT32  bInt_ID_SPI: 1;

		//INT36
		UINT32  bInt_ID_SPI2: 1;
		UINT32  bInt_ID_SPI3: 1;
		UINT32  bInt_ID_SPI4: 1;
		UINT32  bReserved39: 1;
//      UINT32  bInt_ID_SPI5:1;

		//INT40
		UINT32  bReserved40: 1;
		UINT32  bInt_ID_I2C: 1;
		UINT32  bInt_ID_I2C2: 1;
		UINT32  bInt_ID_UART: 1;

		UINT32  bInt_ID_UART2: 1;
		UINT32  bInt_ID_UART3: 1;
		UINT32  bInt_ID_UART4: 1;
		UINT32  bInt_ID_ADC: 1;

		//INT48
		UINT32  bInt_ID_IDE: 1;
		UINT32  bInt_ID_IDE2: 1;
		UINT32  bInt_ID_DSI: 1;
		UINT32  bInt_ID_MI: 1;

		//INT52
		UINT32  bInt_ID_HDMI: 1;
//      UINT32  bInt_ID_VX1:1;
		UINT32  bInt_ID_DMA2: 1;
		UINT32  bInt_ID_LVDS: 1;        // LVDS& HiSPI& CSI
		UINT32  bInt_ID_LVDS2: 1;       //LVDS2&HiSPI2&CSI2

		//INT56
		UINT32  bInt_ID_RTC: 1;
		UINT32  bInt_ID_WDT: 1;
		UINT32  bInt_ID_CG: 1;
		UINT32  bInt_ID_CC: 1;

		//INT60
		UINT32  bInt_ID_I2C3: 1;
		UINT32  bInt_ID_SLVS_EC: 1;
		UINT32  bInt_ID_LVDS3: 1;       //LVDS3&HiSPI3&CSI3
		UINT32  bInt_ID_LVDS4: 1;       //LVDS4&HiSPI4&CSI4

		//INT64
		UINT32  bInt_ID_LVDS5: 1;       //LVDS5&HiSPI5&CSI5
		UINT32  bInt_ID_LVDS6: 1;       //LVDS6&HiSPI6&CSI6
		UINT32  bInt_ID_LVDS7: 1;       //LVDS7&HiSPI7&CSI7
		UINT32  bInt_ID_LVDS8: 1;       //LVDS8&HiSPI8&CSI8

		//INT68
		UINT32  bInt_ID_SIE5: 1;
		UINT32  bInt_ID_SIE6: 1;
		UINT32  bInt_ID_SIE7: 1;
		UINT32  bInt_ID_SIE8: 1;

		//INT72
		UINT32  bInt_ID_IVE: 1;
		UINT32  bInt_ID_SVM: 1;
		UINT32  bInt_ID_SDE: 1;
		UINT32  bInt_ID_CNN: 1;

		//INT76
		UINT32  bInt_ID_DSP: 1;
		UINT32  bInt_ID_DSP2: 1;
		UINT32  bInt_ID_CANBUS: 1;
		UINT32  bInt_ID_CRYPTO: 1;

		//INT80
		UINT32  bInt_ID_HWCOPY: 1;
		UINT32  bInt_ID_ROTATE: 1;
		UINT32  bInt_ID_I2C4: 1;
		UINT32  bInt_ID_I2C5: 1;

		//INT84
		UINT32  bInt_ID_I2C6: 1;
		UINT32  bInt_ID_I2C7: 1;
		UINT32  bReserved86: 1;
		UINT32  bReserved87: 1;

		//INT88
		UINT32  bInt_ID_TMDEMUX: 1;
		UINT32  bReserved89: 3;

		//INT92
		UINT32  bReserved92: 4;
		//INT96 => 4
		UINT32  bReserved96: 4;
		//INT100=> 4
		UINT32  bReserved100: 4;
		//INT104=> 4
		UINT32  bReserved104: 4;
		//INT108=> 4
		UINT32  bReserved108: 4;
		//INT112=> 4
		UINT32  bReserved112: 4;
		//INT116=> 4
		UINT32  bReserved116: 4;
		//INT120=> 4
		UINT32  bReserved120: 4;
		//INT124=> 4
		UINT32  bReserved124: 4;
	} Bit;
} INT_INTC_ID_DST, *PINT_INTC_ID_DST;

typedef union {
	INT_PTN Reg[4];
	struct {
        UINT32  bInt_ID_PPI:32;
		//INT0
		UINT32  bInt_ID_TIMER: 1;
		UINT32  bInt_ID_SIE: 1;
		UINT32  bInt_ID_SIE2: 1;
		UINT32  bInt_ID_SIE3: 1;

		UINT32  bInt_ID_SIE4: 1;
		UINT32  bInt_ID_IPE: 1;
		UINT32  bInt_ID_IME: 1;
		UINT32  bInt_ID_DCE: 1;

		UINT32  bInt_ID_IFE: 1;
		UINT32  bInt_ID_IFE2: 1;
		UINT32  bInt_ID_DIS: 1;
		UINT32  bInt_ID_FDE: 1;

//      UINT32  bInt_ID_RDE:1;
		UINT32  bInt_ID_Ethernet_LPI: 1;
		UINT32  bInt_ID_RHE: 1;
		UINT32  bInt_ID_DRE: 1;
		UINT32  bInt_ID_DAI: 1;


		//INT16
		UINT32  bInt_ID_H264: 1;
		UINT32  bInt_ID_JPEG: 1;
		UINT32  bInt_ID_GRAPHIC: 1;
		UINT32  bInt_ID_GRAPHIC2: 1;

		UINT32  bInt_ID_AFFINE: 1;
		UINT32  bInt_ID_ISE: 1;
		UINT32  bInt_ID_TGE: 1;
		UINT32  bInt_ID_TSMUX: 1;

		UINT32  bInt_ID_GPIO: 1;
		UINT32  bInt_ID_REMOTE: 1;
		UINT32  bInt_ID_PWM: 1;
		UINT32  bInt_ID_USB: 1;

		UINT32  bInt_ID_USB3: 1;
		UINT32  bInt_ID_NAND: 1;
		UINT32  bInt_ID_SDIO: 1;
		UINT32  bInt_ID_SDIO2: 1;

		//INT32
		UINT32  bInt_ID_SDIO3: 1;
		UINT32  bInt_ID_DMA: 1;
		UINT32  bInt_ID_ETHERNET: 1;
		UINT32  bInt_ID_SPI: 1;

		UINT32  bInt_ID_SPI2: 1;
		UINT32  bInt_ID_SPI3: 1;
		UINT32  bInt_ID_SPI4: 1;
		UINT32  bInt_ID_Ehternet_RevMII: 1;
//      UINT32  bInt_ID_SPI5:1;


		UINT32  bInt_ID_SIF: 1;
		UINT32  bInt_ID_I2C: 1;
		UINT32  bInt_ID_I2C2: 1;
		UINT32  bInt_ID_UART: 1;

		UINT32  bInt_ID_UART2: 1;
		UINT32  bInt_ID_UART3: 1;
		UINT32  bInt_ID_UART4: 1;
		UINT32  bInt_ID_ADC: 1;

		//INT48
		UINT32  bInt_ID_IDE: 1;
		UINT32  bInt_ID_IDE2: 1;
		UINT32  bInt_ID_DSI: 1;
		UINT32  bInt_ID_MI: 1;

		//INT52
		UINT32  bInt_ID_HDMI: 1;
//      UINT32  bInt_ID_VX1:1;
		UINT32  bInt_ID_DMA2: 1;
		UINT32  bInt_ID_LVDS: 1;        // LVDS& HiSPI& CSI
		UINT32  bInt_ID_LVDS2: 1;       //LVDS2&HiSPI2&CSI2

		//INT56
		UINT32  bInt_ID_RTC: 1;
		UINT32  bInt_ID_WDT: 1;
		UINT32  bInt_ID_CG: 1;
		UINT32  bInt_ID_CC: 1;

		//INT60
		UINT32  bInt_ID_I2C3: 1;
		UINT32  bInt_ID_SLVS_EC: 1;
		UINT32  bInt_ID_LVDS3: 1;       //LVDS3&HiSPI3&CSI3
		UINT32  bInt_ID_LVDS4: 1;       //LVDS4&HiSPI4&CSI4

		//INT64
		UINT32  bInt_ID_LVDS5: 1;       //LVDS5&HiSPI5&CSI5
		UINT32  bInt_ID_LVDS6: 1;       //LVDS6&HiSPI6&CSI6
		UINT32  bInt_ID_LVDS7: 1;       //LVDS7&HiSPI7&CSI7
		UINT32  bInt_ID_LVDS8: 1;       //LVDS8&HiSPI8&CSI8

		//INT68
		UINT32  bInt_ID_SIE5: 1;
		UINT32  bInt_ID_SIE6: 1;
		UINT32  bInt_ID_SIE7: 1;
		UINT32  bInt_ID_SIE8: 1;

		//INT72
		UINT32  bInt_ID_IVE: 1;
		UINT32  bInt_ID_SVM: 1;
		UINT32  bInt_ID_SDE: 1;
		UINT32  bInt_ID_CNN: 1;

		//INT76
		UINT32  bInt_ID_DSP: 1;
		UINT32  bInt_ID_DSP2: 1;
		UINT32  bInt_ID_CANBUS: 1;
		UINT32  bInt_ID_CRYPTO: 1;

		//INT80
		UINT32  bInt_ID_HWCOPY: 1;
		UINT32  bInt_ID_ROTATE: 1;
		UINT32  bInt_ID_I2C4: 1;
		UINT32  bInt_ID_I2C5: 1;

		//INT84
		UINT32  bInt_ID_I2C6: 1;
		UINT32  bInt_ID_I2C7: 1;
		UINT32  bInt_ID_TIMER2: 1;      //Timer to CA53 core2
		UINT32  bInt_ID_GPIO2: 1;       //GPIO to CA53 core2

		//INT88
		UINT32  bInt_ID_PWM2: 1;
		UINT32  bInt_ID_CC2: 1;
		UINT32  bInt_ID_SIF2: 1;        //Timer to CA53 core2
		UINT32  bInt_ID_DSP_2: 1;       //??

		//INT92
		UINT32  bInt_ID_DSP2_2: 1;      //??
		UINT32  bInt_ID_TMDEMUX: 1;
		UINT32  bReserved92: 2;
		//INT96 => 4
		UINT32  bReserved96: 4;
		//INT100=> 4
		UINT32  bReserved100: 4;
		//INT104=> 4
		UINT32  bReserved104: 4;
		//INT108=> 4
		UINT32  bReserved108: 4;
		//INT112=> 4
		UINT32  bReserved112: 4;
		//INT116=> 4
		UINT32  bReserved116: 4;
		//INT120=> 4
		UINT32  bReserved120: 4;
		//INT124=> 4
		UINT32  bReserved124: 4;
	} Bit;
} INT_GIC_ID_ENABLE, *PINT_GIC_ID_ENABLE;

typedef struct {
	INT_GIC_ID_DST  int_id_dst[CC_CORE_CA53_CORE_NUM];
} INT_GIC_DST, *PINT_GIC_DST;

typedef struct {
	INT_INTC_ID_DST  int_id_dst[CC_CORE_DSP_NUM];
} INT_INTC_DST, *PINT_INTC_DST;

typedef struct {
	INT_GIC_ID_ENABLE  int_id_enable;
} INT_INTC_ENABLE, *PINT_INTC_ENABLE;


// MIPI CSI share the interrupt ID with LVDS / HiSPi
#define INT_ID_CSI          INT_ID_LVDS
#define INT_ID_CSI2         INT_ID_LVDS2

#define INT_ID_CSI3         INT_ID_LVDS3
#define INT_ID_CSI4         INT_ID_LVDS4

#define INT_ID_CSI5         INT_ID_LVDS5
#define INT_ID_CSI6         INT_ID_LVDS6
#define INT_ID_CSI7         INT_ID_LVDS7
#define INT_ID_CSI8         INT_ID_LVDS8

#define INT_ID_VX1          INT_ID_LVDS5
#define INT_ID_VX1_2        INT_ID_LVDS6

// Macro to generate bit value from ID
#define INT_ID_TO_BIT(Id)   ((INT_PTN)(1) << (Id))

// check type and ID
STATIC_ASSERT((INT_ID_MAX - INT_GIC_SPI_START_ID) <= (sizeof(INT_PTN) << 3));

#define INT_ALL_MODULES     ((sizeof(INT_PTN) == 4) ? 0xFFFFFFFFul : 0xFFFFFFFFFFFFFFFFull)

extern INT_PTN  int_getEnable(void);
extern INT_PTN  int_getFlag(void);
extern INT_ID   int_getIRQId(void);
extern INT_ID   int_getDummyId(void);
extern UINT32   int_getLatency(void);
extern ER       int_setConfig(INT_CONFIG_ID ConfigID, UINT32 uiConfig);
extern INT32    int_getConfig(INT_CONFIG_ID ConfigID, UINT32 uiConfig);
extern void     int_get_gic_enable(PINT_INTC_ENABLE gic_int_en);


//@}

#endif
