/*
 *  drivers/misc/nvt_spi_host_reg.h
 *
 *  Author:	Alvin lin
 *  Created:	Apr 15, 2015
 *  Copyright:	Novatek Inc.
 *  Novatek NT72668 SPI host register set
 */
#ifndef __NVT_72668_SPI_HOST_REG_H__
#define __NVT_72668_SPI_HOST_REG_H__

#define NVT_SPI_CTRL_REG			0x00
#define NVT_SPI_TRANS_FROM_DEV		(1 << 0)
#define NVT_SPI_LOCK_ON				(1 << 1)
#define NVT_SPI_ADDR_EN				(1 << 2)
#define NVT_SPI_DUMMY_EN			(1 << 3)
#define NVT_SPI_DUMMY_CLK_SFT		4
#define NVT_SPI_DUMMY_CLK_MSK		0x1F
#define NVT_SPI_DUMMY_CLK_CNT(x)	((x-1) << NVT_SPI_DUMMY_CLK_SFT)
#define NVT_SPI_DBIT_WIDTH_SFT		12
#define NVT_SPI_DBIT_WIDTH_MSK		0x3
#define NVT_SPI_DBIT_1BIT			(0 << NVT_SPI_DBIT_WIDTH_SFT)
#define NVT_SPI_DBIT_2BIT			(1 << NVT_SPI_DBIT_WIDTH_SFT)
#define NVT_SPI_DBIT_4BIT			(2 << NVT_SPI_DBIT_WIDTH_SFT)
#define NVT_SPI_ADDR_WIDTH_SFT      14
#define NVT_SPI_DEFAULT_BYTE		(2 << NVT_SPI_ADDR_WIDTH_SFT)

#define NVT_SPI_CMD_ADDR_REG		0x04
#define NVT_SPI_CMD_SFT				0
#define NVT_SPI_CMD_MSK				0xFF
#define NVT_SPI_ADDR_SFT			0x8
#define NVT_SPI_ADDR_MSK			0xFFFFFF

#define NVT_SPI_ACC_CTRL_REG		0x08
#define NVT_SPI_ACC_START			(1 << 0)
#define NVT_SPI_DATA_EN				(1 << 3)
#define NVT_SPI_DATA_SFT			4
#define NVT_SPI_DATA_MSK			0xF
#define NVT_SPI_DATA_CNT(x)			((x - 1) << NVT_SPI_DATA_SFT)

#define NVT_SPI_ACC_DWORD0_REG		0x10
#define NVT_SPI_ACC_DWORD1_REG		0x14
#define NVT_SPI_ACC_DWORD2_REG		0x18
#define NVT_SPI_ACC_DWORD3_REG		0x1C

#define NVT_SPI_DMA_CTRL_REG		0x20
#define NVT_SPI_DMA_WCNT_SFT		0
#define NVT_SPI_DMA_WCNT_MSK		0x3FFFF
#define NVT_SPI_DMA_START			(1 << 24)

#define NVT_SPI_DMA_DPORT_REG		0x24

#define NVT_SPI_INT_CTRL_REG		0x30
#define NVT_SPI_INT_OLD_DONE_EN		(1 << 0)
#define NVT_SPI_INT_NEW_DONE_EN		(1 << 1)
#define NVT_SPI_INT_OLD_DONE_STS	(1 << 8)
#define NVT_SPI_INT_NEW_DONE_STS	(1 << 9)

#define NVT_SPI_NEW_DMA_ADDR_REG	0x40

#define NVT_SPI_NEW_DMA_CTRL_REG		0x44
#define NVT_SPI_NEW_DMA_TX_CYCLE_SFT	0
#define NVT_SPI_NEW_DMA_TX_CYCLE_MSK	0x3FFFF
#define NVT_SPI_NEW_DMA_TX_LEN(x)		(((x >> 2) - 1) &\
					NVT_SPI_NEW_DMA_TX_CYCLE_MSK)
#define NVT_SPI_NEW_DMA_TO_DDR			(1 << 20)
#define NVT_SPI_NEW_DMA_EN				(1 << 21)

#define NVT_SPI_USE_DMA_LEN_DECISION	16

#define NVT_SPI_DMA_BUF_MAX_SZ			(128*1024)

#define NVT_SPI_WAIT_TIMEOUT			3000


#endif
