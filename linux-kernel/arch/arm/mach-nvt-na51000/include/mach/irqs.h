/**
    NVT IRQ define
    To define IRQ related setting
    @file      irqs.h 
    @ingroup
    @note
    Copyright   Novatek Microelectronics Corp. 2016.  All rights reserved.

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License version 2 as
    published by the Free Software Foundation.
*/

#ifndef __ASM_ARCH_NA51000_IRQS_H
#define __ASM_ARCH_NA51000_IRQS_H

#ifndef CONFIG_SPARSE_IRQ
#define NR_IRQS (256 + NVT_GPIO_INT_EXT_TOTAL)
#endif

#define IRQ_SPI_START			32
#define IRQ_SPI_END			256

#define	NVT_IRQ_Timer   		(IRQ_SPI_START + 0 )
#define	NVT_IRQ_SIE     		(IRQ_SPI_START + 1 )
#define	NVT_IRQ_SIE2    		(IRQ_SPI_START + 2 )
#define	NVT_IRQ_SIE3    		(IRQ_SPI_START + 3 )
#define	NVT_IRQ_SIE4    		(IRQ_SPI_START + 4 )
#define	NVT_IRQ_IPE     		(IRQ_SPI_START + 5 )
#define	NVT_IRQ_IME     		(IRQ_SPI_START + 6 )
#define	NVT_IRQ_DCE     		(IRQ_SPI_START + 7 )
#define	NVT_IRQ_IFE     		(IRQ_SPI_START + 8 )
#define	NVT_IRQ_IFE2    		(IRQ_SPI_START + 9 )
#define	NVT_IRQ_DIS     		(IRQ_SPI_START + 10)
#define	NVT_IRQ_FDE     		(IRQ_SPI_START + 11)
#define	NVT_IRQ_ETH_LPI			(IRQ_SPI_START + 12)
#define	NVT_IRQ_RHE     		(IRQ_SPI_START + 13)
#define	NVT_IRQ_DRE     		(IRQ_SPI_START + 14)
#define	NVT_IRQ_DAI     		(IRQ_SPI_START + 15)
#define	NVT_IRQ_H264	   		(IRQ_SPI_START + 16)
#define	NVT_IRQ_JPEG    		(IRQ_SPI_START + 17)
#define	NVT_IRQ_Graphic 		(IRQ_SPI_START + 18)
#define	NVT_IRQ_Graphic2		(IRQ_SPI_START + 19)
#define	NVT_IRQ_Affine  		(IRQ_SPI_START + 20)
#define	NVT_IRQ_ISE     		(IRQ_SPI_START + 21)
#define	NVT_IRQ_TGE     		(IRQ_SPI_START + 22)
#define	NVT_IRQ_TSMux   		(IRQ_SPI_START + 23)
#define	NVT_IRQ_GPIO    		(IRQ_SPI_START + 24)
#define	NVT_IRQ_Remote  		(IRQ_SPI_START + 25)
#define	NVT_IRQ_PWM     		(IRQ_SPI_START + 26)
#define	NVT_IRQ_USB     		(IRQ_SPI_START + 27)
#define	NVT_IRQ_USB3    		(IRQ_SPI_START + 28)
#define	NVT_IRQ_NAND    		(IRQ_SPI_START + 29)
#define	NVT_IRQ_SDIO    		(IRQ_SPI_START + 30)
#define	NVT_IRQ_SDIO2   		(IRQ_SPI_START + 31)
#define	NVT_IRQ_SDIO3   		(IRQ_SPI_START + 32)
#define	NVT_IRQ_DRAM			(IRQ_SPI_START + 33)
#define	NVT_IRQ_ETH			(IRQ_SPI_START + 34)
#define	NVT_IRQ_SPI  			(IRQ_SPI_START + 35)
#define	NVT_IRQ_SPI2 			(IRQ_SPI_START + 36)
#define	NVT_IRQ_SPI3 			(IRQ_SPI_START + 37)
#define	NVT_IRQ_SPI4 			(IRQ_SPI_START + 38)
#define	NVT_IRQ_ETH_REVMII 		(IRQ_SPI_START + 39)
#define	NVT_IRQ_SIF  			(IRQ_SPI_START + 40)
#define	NVT_IRQ_I2C  			(IRQ_SPI_START + 41)
#define	NVT_IRQ_I2C2 			(IRQ_SPI_START + 42)
#define	NVT_IRQ_UART0 			(IRQ_SPI_START + 43)
#define	NVT_IRQ_UART1			(IRQ_SPI_START + 44)
#define	NVT_IRQ_UART2			(IRQ_SPI_START + 45)
#define	NVT_IRQ_UART3			(IRQ_SPI_START + 46)
#define	NVT_IRQ_ADC  			(IRQ_SPI_START + 47)
#define	NVT_IRQ_IDE  			(IRQ_SPI_START + 48)
#define	NVT_IRQ_IDE2 			(IRQ_SPI_START + 49)
#define	NVT_IRQ_MIPI_DSI		(IRQ_SPI_START + 50)
#define	NVT_IRQ_MI  			(IRQ_SPI_START + 51)
#define	NVT_IRQ_HDMI			(IRQ_SPI_START + 52)
#define	NVT_IRQ_DDR_CTR2 		(IRQ_SPI_START + 53)
#define	NVT_IRQ_LVDS_CSI		(IRQ_SPI_START + 54)
#define	NVT_IRQ_LVDS_CSI2		(IRQ_SPI_START + 55)
#define	NVT_IRQ_RTC 			(IRQ_SPI_START + 56)
#define	NVT_IRQ_WDT 			(IRQ_SPI_START + 57)
#define	NVT_IRQ_CG  			(IRQ_SPI_START + 58)
#define	NVT_IRQ_CC  			(IRQ_SPI_START + 59)
#define	NVT_IRQ_I2C3			(IRQ_SPI_START + 60)
#define	NVT_IRQ_SLVS_EC			(IRQ_SPI_START + 61)
#define	NVT_IRQ_LVDS3			(IRQ_SPI_START + 62)
#define	NVT_IRQ_LVDS4			(IRQ_SPI_START + 63)
#define NVT_IRQ_LVDS5			(IRQ_SPI_START + 64)
#define NVT_IRQ_LVDS6			(IRQ_SPI_START + 65)
#define NVT_IRQ_LVDS7			(IRQ_SPI_START + 66)
#define NVT_IRQ_LVDS8			(IRQ_SPI_START + 67)
#define NVT_IRQ_SIE5			(IRQ_SPI_START + 68)
#define NVT_IRQ_SIE6			(IRQ_SPI_START + 69)
#define NVT_IRQ_SIE7			(IRQ_SPI_START + 70)
#define NVT_IRQ_SIE8			(IRQ_SPI_START + 71)
#define NVT_IRQ_IVE			(IRQ_SPI_START + 72)
#define NVT_IRQ_SVM			(IRQ_SPI_START + 73)
#define NVT_IRQ_SDE			(IRQ_SPI_START + 74)
#define NVT_IRQ_CNN			(IRQ_SPI_START + 75)
#define NVT_IRQ_RDE_2DSP		(IRQ_SPI_START + 76)
#define NVT_IRQ_RDE_3DSP2		(IRQ_SPI_START + 77)
#define NVT_IRQ_RDE_4CANBUS		(IRQ_SPI_START + 78)
#define NVT_IRQ_CRYPTO			(IRQ_SPI_START + 79)
#define NVT_IRQ_COPY			(IRQ_SPI_START + 80)
#define NVT_IRQ_ROTATE			(IRQ_SPI_START + 81)
#define NVT_IRQ_I2C4			(IRQ_SPI_START + 82)
#define NVT_IRQ_I2C5			(IRQ_SPI_START + 83)
#define NVT_IRQ_I2C6			(IRQ_SPI_START + 84)
#define NVT_IRQ_I2C7			(IRQ_SPI_START + 85)

#define	NVT_IRQ_CC2  			(IRQ_SPI_START + 89)
#define NVT_IRQ_PMU0			212
#define NVT_IRQ_PMU1			213

#define NVT_GPIO_INT_EXT_TOTAL		45
#endif /* __ASM_ARCH_NA51000_IRQS_H */
