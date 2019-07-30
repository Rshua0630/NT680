/*
	Register offset and bit definition for I2C module

	Register offset and bit definition for I2C module.

	@file       i2c_reg.h
	@note       Nothing.

	Copyright   Novatek Microelectronics Corp. 2015.  All rights reserved.
*/

#ifndef _I2C_REG_H
#define _I2C_REG_H

#include <linux/bitops.h>

struct nvtim_i2c_platform_data {
	unsigned int	bus_freq;	/* standard bus frequency (Hz) */
	unsigned int	bus_delay;	/* post-transaction delay (usec) */
	unsigned int    sda_pin;        /* GPIO pin ID to use for SDA */
	unsigned int    scl_pin;        /* GPIO pin ID to use for SCL */
};

#define I2C_HANDLE_PINMUX       ENABLE
#define I2C_ALL_BIT		0xFFFFFFFF
#define U32_ALIGNED		I2C_ALL_BIT
#define I2C_SOURCE_CLOCK        48000000	/*48 MHz*/
#define I2C_READ_BIT		0x1

/*Timing & Bus clock register*/
#define I2C_CLKLOW_MIN          0x6
#define I2C_CLKLOW_MAX          0x3FF
#define I2C_CLKHIGH_MIN         0x2
#define I2C_CLKHIGH_MAX         0x3FF
#define I2C_TSR_MIN             0x1
#define I2C_TSR_MAX             0x3FF
#define I2C_GSR_MAX             0xF

/*Timing & Bus clock register default value*/
#define I2C_DEFAULT_GSR         2
#define I2C_DEFAULT_CLKLOW      21	/*(400 KHz) *** not confirm yet ****/
#define I2C_DEFAULT_CLKHIGH     25	/*(400 KHz) *** not confirm yet ****/
#define I2C_DEFAULT_TSR         26	/*(400 KHz)*/
#define I2C_DEFAULT_SCL_TIMEOUT 0x3FF

/*Polling timeout*/
#define I2C_POLLING_TIMEOUT     0x001A0000

#define	I2C_BYTE_CNT_1	1
#define	I2C_BYTE_CNT_2	2
#define	I2C_BYTE_CNT_3	3
#define	I2C_BYTE_CNT_4	4
#define	I2C_BYTE_CNT_5	5
#define	I2C_BYTE_CNT_6	6
#define	I2C_BYTE_CNT_7	7
#define	I2C_BYTE_CNT_8	8

/*I2C Control Register*/
#define I2C_CTRL_INTEN_MASK     0x1FF0
#define I2C_DMA_DIR_READ        0
#define I2C_DMA_DIR_WRITE       1

/*I2C Status Register*/
/*Bit 1 --> NACK status for slave mode*/
#define I2C_STATUS_NACK         0x02

/*I2C SCL timeout value in ns (Not register value)*/
#define I2C_SCL_TIMEOUT_MIN     0xC8000		/*819,200 ns*/
#define I2C_SCL_TIMEOUT_MAX     0x31F38000	/*838,041,600 ns*/

/*I2C bus free time timeout in ns (Not register value)*/
#define I2C_BUSFREE_TIME_MIN    0x14		/*20 ns*/
#define I2C_BUSFREE_TIME_MAX    0x2995		/*10,645 ns*/

#define I2C_BUSFREE_REG_MASK    0x1FF

/*I2C SAR register*/
#define I2C_SAR_REG_MASK        0x3FF
#define I2C_SAR_7BIT_MASK       0x7F

/*I2C Config (PIO)*/
#define I2C_ACCESS_PIO          0
#define I2C_ACCESS_DMA          1
#define I2C_NACK_GEN_SHIFT      4
#define I2C_START_GEN_SHIFT     8
#define I2C_STOP_GEN_SHIFT      12

/*I2C Data*/
#define I2C_DATA_MASK           0xFF
#define I2C_DATA_SHIFT          8

/*I2C DMA*/
#define I2C_DMA_SIZE_MIN        0x4
#define I2C_DMA_SIZE_MAX        0xFFFC

/*Enable specific interrupt event*/
#define I2C_INTR_ALL		0x1473

/*Bus Clock*/
#define I2C_BUS_CLOCK_50KHZ	50000	/*50 KHz*/
#define I2C_BUS_CLOCK_100KHZ	100000	/*100 KHz*/
#define I2C_BUS_CLOCK_200KHZ	200000	/*200 KHz*/
#define I2C_BUS_CLOCK_400KHZ	400000	/*400 KHz*/
#define I2C_BUS_CLOCK_1MHZ	1000000	/*1 MHz*/

/*Register offsets*/
#define I2C_CTRL_REG	0x00
#define I2C_STS_REG	0x04
#define I2C_CONFIG_REG	0x08
#define I2C_BUSCLK_REG	0x0C
#define I2C_DATA_REG	0x10
#define I2C_SAR_REG	0x14
#define I2C_TIMING_REG	0x18
#define I2C_BUS_REG	0x1C
#define I2C_DMASIZE_REG	0x20
#define I2C_DMAADDR_REG	0x24
#define I2C_BUSFREE_REG	0x28

/*Register default value*/
#define I2C_CTRL_REG_DEFAULT        0x00000002
#define I2C_STS_REG_DEFAULT         0x10000000
#define I2C_CONFIG_REG_DEFAULT      0x00000000
#define I2C_BUSCLK_REG_DEFAULT      0x00000000
#define I2C_DATA_REG_DEFAULT        0x00000000
#define I2C_SAR_REG_DEFAULT         0x00000000
#define I2C_TIMING_REG_DEFAULT      0x03FF1001
#define I2C_BUS_REG_DEFAULT         0x00000003
#define I2C_DMASIZE_REG_DEFAULT     0x00000000
#define I2C_DMAADDR_REG_DEFAULT     0x00000000
#define I2C_BUSFREE_REG_DEFAULT     0x000000E2


#define I2C_CTRL_I2C_EN	0
#define I2C_CTRL_SCL_EN	1
#define I2C_CTRL_GC_EN	2
#define I2C_CTRL_TB_EN	3
#define I2C_CTRL_DT_INT_EN	4
#define I2C_CTRL_DR_INT_EN	5
#define I2C_CTRL_BERR_INT_EN	6
#define I2C_CTRL_STOP_INT_EN	7
#define I2C_CTRL_SAM_INT_EN	8
#define I2C_CTRL_GC_INT_EN	9
#define I2C_CTRL_AL_INT_EN	10
#define I2C_CTRL_DMAED_INT_EN	11
#define I2C_CTRL_SCLTIMEOUT_INT_EN	12
#define I2C_CTRL_DMA_DIR	30
#define I2C_CTRL_DMA_EN	31

#define I2C_STS_RW	0
#define I2C_STS_NACK	1
#define I2C_STS_BUSY	2
#define I2C_STS_BUSBUSY	3
#define I2C_STS_DT	4
#define I2C_STS_DR	5
#define I2C_STS_BERR	6
#define I2C_STS_STOP	7
#define I2C_STS_SAM	8
#define I2C_STS_GC	9
#define I2C_STS_AL	10
#define I2C_STS_DMAED	11
#define I2C_STS_SCLTIMEOUT	12
#define I2C_STS_BUSFREE	28

#define I2C_CONFIG_ACCESSMODE	0
#define I2C_CONFIG_NACK_GEN0	4
#define I2C_CONFIG_NACK_GEN1	5
#define I2C_CONFIG_NACK_GEN2	6
#define I2C_CONFIG_NACK_GEN3	7
#define I2C_CONFIG_START_GEN0	8
#define I2C_CONFIG_START_GEN1	9
#define I2C_CONFIG_START_GEN2	10
#define I2C_CONFIG_START_GEN3	11
#define I2C_CONFIG_STOP_GEN0	12
#define I2C_CONFIG_STOP_GEN1	13
#define I2C_CONFIG_STOP_GEN2	14
#define I2C_CONFIG_STOP_GEN3	15
#define I2C_CONFIG_PIO_DATASIZE	16	/*3 bits*/

#define I2C_BUSCLK_LOW_COUNTER	0	/*10 bits*/
#define I2C_BUSCLK_HIGH_COUNTER	16	/*10 bits*/

#define I2C_DATA_DR0	0	/*8 bits*/
#define I2C_DATA_DR1	8	/*8 bits*/
#define I2C_DATA_DR2	16	/*8 bits*/
#define I2C_DATA_DR3	24	/*8 bits*/

#define I2C_SAR_SAR	0	/*10 bits*/
#define I2C_SAR_TENBITS	31

#define I2C_TIMING_TSR	0	/*10 bits*/
#define I2C_TIMING_GSR	12	/*4 bits*/
#define I2C_TIMING_SCLTIMEOUT	16	/*10 bits*/

#define I2C_BUS_SDA	0
#define I2C_BUS_SCL	1

#define I2C_DMASIZE_DMA_BUFSIZE	0	/*16 bits*/
#define I2C_DMASIZE_DMA_TRANCNT	16	/*16 bits*/

#define I2C_DMAADDR_DMA_STARTADDR	0	/*29 bits*/

#define I2C_BUSFREE_BUSFREE	0	/*9 bits*/

#endif
