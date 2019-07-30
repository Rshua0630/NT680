/*
 * (C) Copyright 2003
 * Wolfgang Denk, DENX Software Engineering, wd@denx.de.
 *
 * See file CREDITS for list of people who contributed to this
 * project.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 * MA 02111-1307 USA
 */

#include <common.h>
#include <spi.h>
#include <malloc.h>
#include <asm/io.h>
#include <asm/arch/spi.h>

#define FALSE	0
#define TRUE	1

#define spi_in8( _register_, _value_ ) \
        ((_value_) = *((volatile unsigned char *)(_register_)))

#define spi_out8( _register_, _value_ ) \
        (*((volatile unsigned char *)(_register_)) = (_value_))

#define spi_in16( _register_, _value_ ) \
        ((_value_) = *((volatile unsigned short *)(_register_)))

#define spi_out16( _register_, _value_ ) \
        (*((volatile unsigned short *)(_register_)) = (_value_))

#define spi_in32( _register_, _value_ ) \
        ((_value_) = *((volatile unsigned int *)(_register_)))

#define spi_out32( _register_, _value_ ) \
        (*((volatile unsigned int *)(_register_)) = (_value_))

#define SPI_CLOCK_96MHZ         	0x1
#define SPI_CLOCK_64MHZ         	0x2
#define SPI_CLOCK_48MHZ         	0x3
#define SPI_CLOCK_32MHZ         	0x5
#define SPI_CLOCK_24MHZ         	0x7
#define SPI_CLOCK_12MHZ         	0xF

//common commands.

#define CMD_SPI_PP					0x02	/* Page Program */
#define CMD_SPI_WRSR				0x01	/* Write Status Register */
#define CMD_SPI_RDSR				0x05	/* Read Status Register */
#define CMD_SPI_READ				0x03	/* Read Data Bytes */
#define CMD_SPI_WRDI				0x04	/* Write Disable */
#define CMD_SPI_WREN				0x06	/* Write Enable */
#define CMD_SPI_FAST_READ			0x0b	/* Read Data Bytes at Higher Speed */
#define CMD_SPI_SE					0x20	/* Sector Erase */
#define CMD_SPI_BE					0xd8	/* Block Erase */
#define CMD_SPI_CE					0x60	/* Chip Erase */
#define CMD_SPI_CE1					0xc7	/* Chip Erase */
#define CMD_SPI_READ_JEDECID		0x9f
#define CMD_SPI_DP					0xb9	/* Deep Power-down */

#ifdef CONFIG_SPI_FLASH_MACRONIX
#define CMD_MX25XX_RES				0xab	/* Release from DP, and Read Signature */
#endif

#ifdef CONFIG_SPI_FLASH_WINBOND
#define CMD_WINBOND_WREN2	        0x50	/* Write Enable */
#define CMD_WINBOND_RDSRHI			0x35	/* Read Status Register 2 */
#define CMD_WINBOND_WRSR2		    0x31	/* Write Status Register */
#define CMD_WINBOND_QPG				0x32	/* Quad page Program */
#define CMD_WINBOND_BE32			0x52	/* Block erase 32K */
#define CMD_WINBOND_ESUSPEND		0x75	/* Erase suspend */
#define CMD_WINBOND_ERESUME			0x7a	/* Erase resume */
#define CMD_WINBOND_FAST_READ_DO	0x3b	/* Fase read Data by dual output */
#define CMD_WINBOND_FAST_READ_DIO	0xbb	/* Fase read Data by dual I/O */
#define CMD_WINBOND_FAST_READ_QO	0x6b	/* Fase read Data by quad output */
#define CMD_WINBOND_FAST_READ_QIO	0xeb	/* Fase read Data by quad I/O */
#endif

#define XFER_DIR_TO_SPI				0
#define XFER_DIR_FROM_SPI			1

#define XFER_DATA_WIDTH_DEFAULT		0
#define XFER_DATA_WIDTH_1_BIT		0
#define XFER_DATA_WIDTH_2_BIT		1
#define XFER_DATA_WIDTH_4_BIT		2

#define SPI_DMA_READ_ALIGN 			16
#define SPI_DMA_WRITE_ALIGN			4
#define SPI_PIO_READ_ALIGN			4

#define SPI_XFER_TIMEOUT			(50 * 1000)

typedef struct spi_regs_s
{
	uint32_t	ctrl;
	uint32_t	cmd;
	uint32_t	access_ctrl;
	uint8_t		access_data[16];
#ifdef CONFIG_SPI_APB_DMA_MODE
	uint32_t	apb_dma_ctrl;
	uint32_t	apb_dma_data;
#endif
	uint32_t	intr;
#ifdef CONFIG_SPI_AHB_DMA_MODE
	uint32_t	ahb_dma_addr;
	uint32_t	ahb_dma_ctrl;
#endif
} spi_regs_t;

typedef struct spi_params_s {
	uint32_t	command;
	uint32_t	address;
	uint32_t	dummy_count;

	u_char		address_en;
	u_char		dummy_en;
	u_char		dir;
} spi_params_t;

static spi_regs_t	spi_regs;
static spi_params_t spi_params;

typedef int (*flash_cmd_parse)(spi_regs_t *spi_reg, const void *dout, uint *bytes, spi_params_t *params);

#ifdef CONFIG_SPI_FLASH_MACRONIX
static int macronix_flash_cmd_parse(spi_regs_t *spi_reg, const void *dout, uint *bytes, spi_params_t *params);
#endif
#ifdef CONFIG_SPI_FLASH_WINBOND
static int winbond_flash_cmd_parse(spi_regs_t *spi_reg, const void *dout, uint *bytes, spi_params_t *params);
#endif

flash_cmd_parse flash_cmd_tab[] = {
	/* Keep it sorted by define name */
#if CONFIG_SPI_FLASH_ATMEL
	NULL,
#endif
#if CONFIG_SPI_FLASH_EON
	NULL,
#endif
#if CONFIG_SPI_FLASH_MACRONIX
	macronix_flash_cmd_parse,
#endif
#if CONFIG_SPI_FLASH_SPANSION
	NULL,
#endif
#if CONFIG_SPI_FLASH_SST
	NULL,
#endif
#if CONFIG_SPI_FLASH_STMICRO
	NULL,
#endif
#if CONFIG_SPI_FLASH_WINBOND
	winbond_flash_cmd_parse,
#endif
#if CONFIG_SPI_FRAM_RAMTRON
	NULL,
#endif
#if CONFIG_SPI_FLASH_STMICRO
	NULL,
#endif
	/* Keep it sorted by best detection */
#if CONFIG_SPI_FRAM_RAMTRON_NON_JEDEC
	NULL,
#endif
};

#define FLASH_CMD_TAB_SIZE 	(sizeof(flash_cmd_tab)/sizeof(flash_cmd_parse))

#ifdef CONFIG_SPI_APB_DMA_MODE
static void spi_init_apbdma_tzasc(void)
{
	volatile unsigned long val;

	/**
	 * Enable full access of TZASC region 0
	 * bit 1: switching spi apb master to secure master
	 * bit 2: switching spi ahb master to secure master
	 */
	val  = *(volatile unsigned long *)(0xfd650000);
	val |= 0x04;
	val &= ~0x02;

	*(volatile unsigned long *)(0xfd650000) = val;
}
#endif

#ifdef CONFIG_SPI_AHB_DMA_MODE
static void spi_init_ahbdma_tzasc(void)
{
	volatile unsigned long val;

	/**
	 * Enable full access of TZASC region 0
	 * bit 1: switching spi apb master to secure master
	 * bit 2: switching spi ahb master to secure master
	 */
	val  = *(volatile unsigned long *)(0xfd650000);
	val &= ~0x04;
	val |= 0x02;

	*(volatile unsigned long *)(0xfd650000) = val;
}
#endif

void spi_notify_stbc_update(void)
{
	/* STBC password */
	spi_out32(0xfc040204, 0x00072682);
	mdelay(10);
	spi_out32(0xfc040204, 0x00028627);
	mdelay(10);
	spi_out32(0xfc040208, 0x00000001);
	mdelay(10);

	/* STBC move to sram */
	spi_out32(0xfc040000, 0xd1000000);
	mdelay(1000);
	/* STBC release SPI_WP */
	spi_out32(0xfc040000, 0xd4000000);
	mdelay(20);
}

void spi_notify_stbc_reboot(void)
{
	/* STBC reboot system */
	spi_out32(0xfc040000, 0xd5000000);
}

#ifdef CONFIG_DEBUG
static void spi_register_dump(spi_regs_t *spi_reg)
{
	debug("==========================================================\n");
	debug("[SPI SW]:\n");
	debug("  ctrl=%08x, cmd=%08x, access_ctrl=%08x, intr=%08x\n",
			spi_reg->ctrl, spi_reg->cmd, spi_reg->access_ctrl, spi_reg->intr);
#ifdef CONFIG_SPI_AHB_DMA_MODE
	debug("  ahb_dma_addr=%08x, ahb_dma_ctrl=%08x\n",
			spi_reg->ahb_dma_addr, spi_reg->ahb_dma_ctrl);
#endif
	debug("[SPI HW]:\n");
	debug("  ctrl=%08x, cmd=%08x, access_ctrl=%08x, intr=%08x\n",
			readl(SPI_CTRL_REG), readl(SPI_CMD_REG), readl(SPI_ACCESS_CTRL_REG),
			readl(SPI_INTERRUP_REG));
#ifdef CONFIG_SPI_AHB_DMA_MODE
	debug("[AHB HW]:\n");
	debug("  ahb_dma_addr=%08x, ahb_dma_ctrl=%08x\n",
			readl(SPI_AHB_DMA_ADDR_REG), readl(SPI_AHB_DMA_CTRL_REG));
#endif
#ifdef CONFIG_SPI_APB_DMA_MODE
	debug("[APB HW]:\n");
	debug("  dma_src=%08x, dma_dest=%08x, dma_count=%08x, dma_cmd=%08x\n",
			readl(APB_DMA_CHA_SRC_ADDR_REG), readl(APB_DMA_CHA_DEST_ADDR_REG),
			readl(APB_DMA_CHA_COUNT_REG), readl(APB_DMA_CHA_CMD_REG));
#endif
	debug("==========================================================\n");
}
#endif

static void spi_dir_fromflash(spi_regs_t *spi_reg)
{
	spi_reg->ctrl |= SPI_CTRL_DIR;
}

static void spi_dir_toflash(spi_regs_t *spi_reg)
{
	spi_reg->ctrl &= ~SPI_CTRL_DIR;
}

#ifdef CONFIG_SPI_AHB_DMA_MODE
static void spi_dir_ahbdma_fromflash(spi_regs_t *spi_reg)
{
	spi_reg->ahb_dma_ctrl |= SPI_AHB_DMA_DIR_FROM_DEV;
}

static void spi_dir_ahbdma_toflash(spi_regs_t *spi_reg)
{
	spi_reg->ahb_dma_ctrl &= ~SPI_AHB_DMA_DIR_FROM_DEV;
}
#endif

static void spi_bus_lock(spi_regs_t *spi_reg)
{
#ifdef CONFIG_SPI_LOCK_CTRL
	spi_reg->ctrl |= SPI_CTRL_LOCK;
#endif
}

static void spi_bus_unlock(spi_regs_t *spi_reg)
{
#ifdef CONFIG_SPI_LOCK_CTRL
	spi_reg->ctrl &= ~SPI_CTRL_LOCK;
#endif
}

static void spi_addr_enable(spi_regs_t *spi_reg)
{
	spi_reg->ctrl |= SPI_CTRL_ADDR_EN;
}

static void spi_addr_disable(spi_regs_t *spi_reg)
{
	spi_reg->ctrl &= ~SPI_CTRL_ADDR_EN;
}

static void spi_dummy_enable(spi_regs_t *spi_reg)
{
	spi_reg->ctrl |= SPI_CTRL_DUMMY_EN;
}

static void spi_dummy_disable(spi_regs_t *spi_reg)
{
	spi_reg->ctrl &= ~SPI_CTRL_DUMMY_EN;
}

static void spi_dummy_count(spi_regs_t *spi_reg, uint32_t count)
{
	spi_reg->ctrl &= ~SPI_CTRL_DUMMY_CNT;
	spi_reg->ctrl |= ((count-1) << 4);
}

static void spi_width_select(spi_regs_t *spi_reg, uint32_t bit)
{
	spi_reg->ctrl &= ~SPI_CTRL_WIDTH_SELECT;
	spi_reg->ctrl |= (bit << 12);
}

static void spi_command(spi_regs_t *spi_reg, uint32_t command)
{
	spi_reg->cmd &= ~SPI_CMD_MASK;
	spi_reg->cmd |= command;
}

static void spi_address(spi_regs_t *spi_reg, uint32_t address)
{
	spi_reg->cmd &= ~SPI_ADDR_MASK;
	spi_reg->cmd |= (address << 8);
}

#ifdef CONFIG_SPI_AHB_DMA_MODE
static void spi_address_ahbdma_buffer(spi_regs_t *spi_reg, uint32_t address)
{
	spi_reg->ahb_dma_addr = address;
}
#endif

static void spi_data_enable(spi_regs_t *spi_reg)
{
	spi_reg->access_ctrl |= SPI_ACCESS_CTRL_DATA_ENABLE;
}

static void spi_data_disable(spi_regs_t *spi_reg)
{
	spi_reg->access_ctrl &= ~SPI_ACCESS_CTRL_DATA_ENABLE;
}

static void spi_data_count(spi_regs_t *spi_reg, uint32_t count)
{
	spi_reg->access_ctrl &= ~SPI_ACCESS_CTRL_DATA_COUNT_MASK;
	spi_reg->access_ctrl |= ((count-1) << 4);
}

#ifdef CONFIG_SPI_APB_DMA_MODE
static void spi_data_apbdma_count(spi_regs_t *spi_reg, uint32_t count)
{
	spi_reg->apb_dma_ctrl &= ~SPI_APB_DMA_DATA_COUNT;
	spi_reg->apb_dma_ctrl |= ((count>>2) - 1);
}
#endif

#ifdef CONFIG_SPI_AHB_DMA_MODE
static void spi_data_ahbdma_count(spi_regs_t *spi_reg, uint32_t len)
{
	unsigned int word_len; // unit: word, but dma always transfer 16-bytes aligned

	if (len < 16) {
		word_len = 3;
	}
	else {
		if (len & 0x0f)
			word_len = (((len >> 4) + 1) << 2);
		else
			word_len = ((len >> 4) << 2);

		word_len--;
	}

	spi_reg->ahb_dma_ctrl &= ~SPI_AHB_DMA_DATA_COUNT;
	spi_reg->ahb_dma_ctrl |= word_len;
}
#endif

static void spi_access_start(spi_regs_t *spi_reg)
{
	volatile uint32_t val;
	uint32_t timebase;

	spi_out32(SPI_CTRL_REG, spi_reg->ctrl);
	spi_out32(SPI_CMD_REG, spi_reg->cmd);
	spi_out32(SPI_ACCESS_CTRL_REG, spi_reg->access_ctrl);
#ifdef CONFIG_DEBUG
	spi_register_dump(spi_reg);
#endif
	spi_reg->access_ctrl |= SPI_ACCESS_CTRL_START;
	spi_out32(SPI_ACCESS_CTRL_REG, spi_reg->access_ctrl);

	timebase = get_timer(0);
	do
	{
		udelay(50);

		spi_in32(SPI_INTERRUP_REG, val);
		if ((val & SPI_INTERRUPT_FINISH) == SPI_INTERRUPT_FINISH)
		{
			spi_reg->ctrl 			= SPI_CTRL_LOCK;	/* keep bus lock state */
			spi_reg->cmd			= 0;
			spi_reg->access_ctrl	= 0;

			spi_out32(SPI_INTERRUP_REG, SPI_INTERRUPT_FINISH);

			spi_out32(SPI_CTRL_REG, spi_reg->ctrl);
			spi_out32(SPI_CMD_REG, spi_reg->cmd);
			spi_out32(SPI_ACCESS_CTRL_REG, spi_reg->access_ctrl);

			break;
		}
	} while (get_timer(timebase) < SPI_XFER_TIMEOUT);
}

#ifdef CONFIG_SPI_APB_DMA_MODE
static void spi_apbdma_start(spi_regs_t *spi_reg)
{
	volatile uint32_t val;
	uint32_t timebase;

	spi_out32(SPI_CTRL_REG, spi_reg->ctrl);
	spi_out32(SPI_CMD_REG, spi_reg->cmd);
	spi_out32(SPI_ACCESS_CTRL_REG, spi_reg->access_ctrl);
	spi_out32(SPI_APB_DMA_CTRL_REG, spi_reg->apb_dma_ctrl);
#ifdef CONFIG_DEBUG
	spi_register_dump(spi_reg);
#endif
	spi_reg->apb_dma_ctrl |= SPI_APB_DMA_START;
	spi_out32(SPI_APB_DMA_CTRL_REG, spi_reg->apb_dma_ctrl);

	timebase = get_timer(0);
	do
	{
		udelay(50);

		spi_in32(SPI_APB_DMA_CTRL_REG, val);
		if (!(val & SPI_APB_DMA_START))
		{
			spi_reg->ctrl 			= SPI_CTRL_LOCK;	/* keep bus lock state */
			spi_reg->cmd			= 0;
			spi_reg->access_ctrl	= 0;
			spi_reg->apb_dma_ctrl	= 0;

			spi_out32(SPI_CTRL_REG, spi_reg->ctrl);
			spi_out32(SPI_CMD_REG, spi_reg->cmd);
			spi_out32(SPI_ACCESS_CTRL_REG, spi_reg->access_ctrl);
			spi_out32(SPI_APB_DMA_CTRL_REG, spi_reg->apb_dma_ctrl);

			break;
		}
	} while (get_timer(timebase) < SPI_XFER_TIMEOUT);
}
#endif

#ifdef CONFIG_SPI_AHB_DMA_MODE
static void spi_ahbdma_start(spi_regs_t *spi_reg)
{
	volatile uint32_t val;
	uint32_t timebase;

	spi_out32(SPI_CTRL_REG, spi_reg->ctrl);
	spi_out32(SPI_CMD_REG, spi_reg->cmd);
	spi_out32(SPI_ACCESS_CTRL_REG, spi_reg->access_ctrl);
	spi_out32(SPI_AHB_DMA_ADDR_REG, spi_reg->ahb_dma_addr);
	spi_out32(SPI_AHB_DMA_CTRL_REG, spi_reg->ahb_dma_ctrl);
#ifdef CONFIG_DEBUG
	spi_register_dump(spi_reg);
#endif
	spi_reg->ahb_dma_ctrl |= SPI_AHB_DMA_ENABLE;
	spi_out32(SPI_AHB_DMA_CTRL_REG, spi_reg->ahb_dma_ctrl);

	timebase = get_timer(0);
	do
	{
		udelay(50);

		spi_in32(SPI_INTERRUP_REG, val);
		if ((val & SPI_AHB_DMA_FINISH) == SPI_AHB_DMA_FINISH)
		{
			spi_reg->ctrl 			= SPI_CTRL_LOCK;	/* keep bus lock state */
			spi_reg->cmd			= 0;
			spi_reg->access_ctrl	= 0;
			spi_reg->ahb_dma_addr	= 0;
			spi_reg->ahb_dma_ctrl	= 0;

			spi_out32(SPI_INTERRUP_REG, SPI_AHB_DMA_FINISH); // clear finish bit

			spi_out32(SPI_CTRL_REG, spi_reg->ctrl);
			spi_out32(SPI_CMD_REG, spi_reg->cmd);
			spi_out32(SPI_ACCESS_CTRL_REG, spi_reg->access_ctrl);
			spi_out32(SPI_AHB_DMA_ADDR_REG, spi_reg->ahb_dma_addr);
			spi_out32(SPI_AHB_DMA_CTRL_REG, spi_reg->ahb_dma_ctrl);

			break;
		}
	} while (get_timer(timebase) < SPI_XFER_TIMEOUT);
}
#endif

static int spi_data_read(spi_regs_t *spi_reg, uint32_t *buf, uint32_t len)
{
	uint32_t word_len, i;
	uint32_t tmp_buf[4];

	if (len > 16)
		return -1;

	word_len = (len / 4) + ((len % 4) ? 1 : 0);

	spi_access_start(spi_reg);

	for (i=0; i<word_len; i++)
		spi_in32((SPI_DATA0_REG + i*4), tmp_buf[i]);

	memcpy((char*)buf, (char*)tmp_buf, len);
	return 0;
}

#ifdef CONFIG_SPI_APB_DMA_MODE
static int spi_data_apbdma_read(spi_regs_t *spi_reg, uint32_t *buf, uint32_t len)
{
	volatile uint32_t src, dest;
	volatile uint32_t cmd;
	uint32_t qword_len = len / 16;

	if (!qword_len)
		return -1;

	src  = (uint32_t)SPI_APB_DMA_DATAPORT_REG;
	dest = (uint32_t)buf;

	spi_out32(APB_DMA_CHA_SRC_ADDR_REG, src);

	spi_out32(APB_DMA_CHA_DEST_ADDR_REG, dest);

	spi_out32(APB_DMA_CHA_COUNT_REG, qword_len);

	cmd =	APB_DMA_BURST_MODE       |
			APB_DMA_SRC_ADDR_APB_BUS | APB_DMA_DEST_ADDR_AHB_BUS |
			APB_DMA_SRC_BURST_NO_INC | APB_DMA_DST_BURST_INC_4   | APB_DMA_DATA_WIDTH_WORD |
			APB_DMA_REQ_SIGNAL_SPI;
	spi_out32(APB_DMA_CHA_CMD_REG, cmd);
	cmd |= APB_DMA_ENABLE;
	spi_out32(APB_DMA_CHA_CMD_REG, cmd);

	spi_apbdma_start(spi_reg);

	invalidate_dcache_range((ulong)buf, (ulong)(buf+len-1));

	return 0;
}
#endif

#ifdef CONFIG_SPI_AHB_DMA_MODE
static int spi_data_ahbdma_read(spi_regs_t *spi_reg, unsigned int *buf, unsigned int len)
{
	spi_address_ahbdma_buffer(spi_reg, (uint32_t)buf);

	spi_data_ahbdma_count(spi_reg, len);
	spi_dir_ahbdma_fromflash(spi_reg);

	invalidate_dcache_range((ulong)buf, (ulong)(buf+len-1));

	spi_ahbdma_start(spi_reg);

	return 0;
}
#endif

#ifdef CONFIG_SPI_PIO_MODE
static int spi_data_write(spi_regs_t *spi_reg, uint32_t *buf, uint32_t len)
{
	uint32_t word_len, i;

	if (len > 16)
		return -1;

	word_len = (len / 4) + ((len % 4) ? 1 : 0);

	for (i=0; i<word_len; i++)
		spi_out32((SPI_DATA0_REG + i*4), buf[i]);

	spi_access_start(spi_reg);

	return 0;
}
#endif

#ifdef CONFIG_SPI_APB_DMA_MODE
static int spi_data_apbdma_write(spi_regs_t *spi_reg, uint32_t *buf, uint32_t len)
{
	volatile uint32_t src, dest;
	volatile uint32_t cmd;
	uint32_t word_len = len / 4;

	if (!word_len)
		return -1;

	src  = (uint32_t)buf;
	dest = (uint32_t)SPI_APB_DMA_DATAPORT_REG;

	spi_out32(APB_DMA_CHA_SRC_ADDR_REG, src);

	spi_out32(APB_DMA_CHA_DEST_ADDR_REG, dest);

	spi_out32(APB_DMA_CHA_COUNT_REG, word_len);

	cmd =	APB_DMA_SRC_ADDR_AHB_BUS | APB_DMA_DEST_ADDR_APB_BUS |
			APB_DMA_SRC_BURST_INC_4  | APB_DMA_DST_BURST_NO_INC  | APB_DMA_DATA_WIDTH_WORD |
			APB_DMA_REQ_SIGNAL_SPI;
	spi_out32(APB_DMA_CHA_CMD_REG, cmd);
	cmd |= APB_DMA_ENABLE;
	spi_out32(APB_DMA_CHA_CMD_REG, cmd);

	/* note: it needs flush the cache when a dma operation */
	flush_dcache_range((ulong)buf, (ulong)(buf+len-1));

	spi_apbdma_start(spi_reg);

	return 0;
}
#endif

#ifdef CONFIG_SPI_AHB_DMA_MODE
static int spi_data_ahbdma_write(spi_regs_t *spi_reg, unsigned int *buf, unsigned int len)
{
	spi_address_ahbdma_buffer(spi_reg, (uint32_t)buf);

	spi_data_ahbdma_count(spi_reg, len);
	spi_dir_ahbdma_toflash(spi_reg);

	/* note: it needs flush the cache when a dma operation */
	flush_dcache_range((ulong)buf, (ulong)(buf+len-1));

	spi_ahbdma_start(spi_reg);

	return 0;
}
#endif

static int common_cmd_parse(spi_regs_t *spi_reg, const void *dout, uint *bytes, spi_params_t *params)
{
	const u8 *txp = dout;

	params->command = 0;
	params->address = 0;
	params->dummy_count = 0;
	params->address_en = 0;
	params->dummy_en = 0;
	params->dir = 0;

	spi_bus_lock(spi_reg);

	switch (txp[0])
	{
		case CMD_SPI_WREN:			/* Write Enable */
		case CMD_SPI_WRDI:			/* Write Disable */
		case CMD_SPI_CE:			/* Chip Erase */
		case CMD_SPI_CE1:			/* Chip Erase */
		case CMD_SPI_DP:			/* Deep Power-down */
			spi_data_disable(spi_reg);
			params->command = txp[0];
			params->address = 0;
			params->dummy_count = 0;
			params->address_en = 0;
			params->dummy_en = 0;
			params->dir = XFER_DIR_TO_SPI;
			*bytes -= 1;
			break;
		case CMD_SPI_WRSR:			/* Write Status Register */
			params->command = txp[0];
			params->address = 0;
			params->dummy_count = 0;
			params->address_en = 0;
			params->dummy_en = 0;
			params->dir = XFER_DIR_TO_SPI;
			*bytes -= 1;
			break;
		case CMD_SPI_READ_JEDECID:
		case CMD_SPI_RDSR:			/* Read Status Register */
			params->command = txp[0];
			params->address = 0;
			params->dummy_count = 0;
			params->address_en = 0;
			params->dummy_en = 0;
			params->dir = XFER_DIR_FROM_SPI;
			*bytes -= 1;
			break;
		case CMD_SPI_READ:			/* Read Data Bytes */
			params->command = txp[0];
			params->address = ((uint)txp[1]<<16) | ((uint)txp[2]<<8) | ((uint)txp[3]);
			params->dummy_count = 0;
			params->address_en = 1;
			params->dummy_en = 0;
			params->dir = XFER_DIR_FROM_SPI;
			*bytes -= 4;
			break;
		case CMD_SPI_FAST_READ:		/* Read Data Bytes at Higher Speed */
			params->command = txp[0];
			params->address = ((uint)txp[1]<<16) | ((uint)txp[2]<<8) | ((uint)txp[3]);
			params->dummy_count = 8;
			params->address_en = 1;
			params->dummy_en = 1;
			params->dir = XFER_DIR_FROM_SPI;
			*bytes -= (4 + (params->dummy_count / 8));
			break;
		case CMD_SPI_PP:				/* Page Program */
		case CMD_SPI_SE:				/* Sector Erase */
		case CMD_SPI_BE:				/* Block Erase */
			params->command = txp[0];
			params->address = ((uint)txp[1]<<16) | ((uint)txp[2]<<8) | ((uint)txp[3]);
			params->dummy_count = 0;
			params->address_en = 1;
			params->dummy_en = 0;
			params->dir = XFER_DIR_TO_SPI;
			*bytes -= 4;
			break;
		default:
			return -1;
	}

	return 0;
}

#ifdef CONFIG_SPI_FLASH_MACRONIX
static int macronix_flash_cmd_parse(spi_regs_t *spi_reg, const void *dout, uint *bytes, spi_params_t *params)
{
	const u8 *txp = dout;

	params->command = 0;
	params->address = 0;
	params->dummy_count = 0;
	params->address_en = 0;
	params->dummy_en = 0;
	params->dir = 0;

	switch (txp[0])
	{
		case CMD_MX25XX_RES:			/* Release from DP, and Read Signature */
			params->command = txp[0];
			params->address = 0;
			params->dummy_count = 24;
			params->address_en = 0;
			params->dummy_en = 1;
			params->dir = XFER_DIR_FROM_SPI;
			*bytes -= (4 + (params->dummy_count / 8));
			break;
		default:
			debug("Unknown command (0x%x) type(Macronix)\r\n", txp[0]);
			return -1;
	}

	return 0;
}
#endif

#ifdef CONFIG_SPI_FLASH_WINBOND
static int winbond_flash_cmd_parse(spi_regs_t *spi_reg, const void *dout, uint *bytes, spi_params_t *params)
{
	const u8 *txp = dout;

	params->command = 0;
	params->address = 0;
	params->dummy_count = 0;
	params->address_en = 0;
	params->dummy_en = 0;
	params->dir = 0;

	switch (txp[0])
	{
		case CMD_WINBOND_WREN2:
			spi_data_disable(spi_reg);
			params->command = txp[0];
			params->address = 0;
			params->dummy_count = 0;
			params->address_en = 0;
			params->dummy_en = 0;
			params->dir = XFER_DIR_TO_SPI;
			*bytes -= 1;
			break;
		case CMD_WINBOND_WRSR2:	/* Write Status Register */
			params->command = txp[0];
			params->address = 0;
			params->dummy_count = 0;
			params->address_en = 0;
			params->dummy_en = 0;
			params->dir = XFER_DIR_TO_SPI;
			*bytes -= 1;
			break;
		case CMD_WINBOND_RDSRHI: 	/* Read Status Register 2 */
		case CMD_WINBOND_ESUSPEND:	/* Erase suspend */
		case CMD_WINBOND_ERESUME:	/* Erase resume */
			params->command = txp[0];
			params->address = 0;
			params->dummy_count = 0;
			params->address_en = 0;
			params->dummy_en = 0;
			params->dir = XFER_DIR_FROM_SPI;
			*bytes -= 1;
			break;
		case CMD_WINBOND_QPG:	/* Quad page Program */
		case CMD_WINBOND_BE32:	/* Block erase 32K */
			params->command = txp[0];
			params->address = ((uint)txp[1]<<16) | ((uint)txp[2]<<8) | ((uint)txp[3]);
			params->dummy_count = 0;
			params->address_en = 1;
			params->dummy_en = 0;
			params->dir = XFER_DIR_TO_SPI;
			*bytes -= 4;
			break;
		case CMD_WINBOND_FAST_READ_DO:	/* Fase read Data by dual output */
		case CMD_WINBOND_FAST_READ_QO:	/* Fase read Data by quad output */
			params->command = txp[0];
			params->address = ((uint)txp[1]<<16) | ((uint)txp[2]<<8) | ((uint)txp[3]);
			params->dummy_count = 8;
			params->address_en = 1;
			params->dummy_en = 1;
			params->dir = XFER_DIR_FROM_SPI;
			*bytes -= (4 + (params->dummy_count / 8));
			break;

		case CMD_WINBOND_FAST_READ_QIO:
			params->command = txp[0];
			params->address = ((uint)txp[1]<<16) | ((uint)txp[2]<<8) | ((uint)txp[3]);
			params->dummy_count = 4;
			params->address_en = 1;
			params->dummy_en = 1;
			params->dir = XFER_DIR_FROM_SPI;
			*bytes -= (4 + ((params->dummy_count * 4) / 8));
			break;
		default:
			debug("Unknown command (0x%x) type(Winbond)\r\n", txp[0]);
			return -1;
	}

	return 0;
}
#endif

static int spi_cmd_parse(spi_regs_t *spi_reg, const void *dout, uint *bytes, spi_params_t *params)
{
	int id, ret;

	if(common_cmd_parse(spi_reg, dout, bytes, params) == -1)
	{
		/* special command ? */
		for (id=0; id<FLASH_CMD_TAB_SIZE; id++)
		{
			if (flash_cmd_tab[id] == NULL)
				break;

			ret = (flash_cmd_tab[id])(spi_reg, dout, bytes, params);
			if (!ret)
				break;
		}
	}

	return 0;
}

void spi_cs_activate(struct spi_slave *slave)
{
}

void spi_cs_deactivate(struct spi_slave *slave)
{
}

static void spi_set_clock(unsigned long ulMaxClock)
{
}

static void spi_set_iomode(unsigned char ucMode)
{
}

void spi_init(void)
{
}

struct spi_slave *spi_setup_slave(unsigned int bus, unsigned int cs,
			unsigned int max_hz, unsigned int mode)
{
	struct spi_slave *slave = NULL;
	spi_regs_t *spi_reg;

	debug("spi_setup_slave: bus:%i cs:%i max_hz:%i mode:%i\n",
			bus, cs, max_hz, mode);

	slave = malloc(sizeof(struct spi_slave));
	if (!slave)
		return NULL;

	slave->bus = bus;
	slave->cs = cs;

	/* When setting this lock bit, it needs to send an additional
	 * command(dummy command) to signal controller. SPI controller will
	 * deal this lock bit for activate or deactivate bus.
	 */
	spi_reg = &spi_regs;
	memset(spi_reg, '\0', sizeof(spi_regs_t));

	spi_bus_lock(spi_reg);
	spi_dir_fromflash(spi_reg);
	spi_addr_disable(spi_reg);
	spi_dummy_disable(spi_reg);
	spi_command(spi_reg, CMD_SPI_RDSR);
	spi_data_enable(spi_reg);
	spi_data_count(spi_reg, 1);
	spi_access_start(spi_reg);

	return slave;
}

void spi_free_slave(struct spi_slave *slave)
{
	spi_regs_t *spi_reg;

	debug("spi_free_slave: bus:%i cs:%i\n", slave->bus, slave->cs);

	/* When remove this lock bit, you need to send an additional
	 * command(dummy command) to signal controller. SPI controller will
	 * deal this lock bit for activate or deactivate bus.
	 */
	spi_reg = &spi_regs;
	memset(spi_reg, '\0', sizeof(spi_regs_t));

	spi_bus_unlock(spi_reg);
	spi_dir_fromflash(spi_reg);
	spi_addr_disable(spi_reg);
	spi_dummy_disable(spi_reg);
	spi_command(spi_reg, CMD_SPI_RDSR);
	spi_data_enable(spi_reg);
	spi_data_count(spi_reg, 1);
	spi_access_start(spi_reg);

	free(slave);
}

int spi_claim_bus(struct spi_slave *slave)
{
	debug("spi_claim_bus: bus:%i cs:%i\n", slave->bus, slave->cs);
	return 0;
}

void spi_release_bus(struct spi_slave *slave)
{
	debug("spi_release_bus: bus:%i cs:%i\n", slave->bus, slave->cs);
}

int spi_xfer(struct spi_slave *slave, unsigned int bitlen,
		const void *dout, void *din, unsigned long flags)
{
	void *rxd = din;
	void *txd = (void *)dout;
	unsigned int bytes = bitlen / 8;
	unsigned int xfer_len;
	int ret = 0;

	spi_regs_t *spi_reg;

	debug("spi_xfer: slave %u:%u dout %p din %p bitlen %u flags %lu\n",
	      slave->bus, slave->cs, dout, din, bitlen, flags);

	if (bitlen == 0) {
		return -1;
	}

	/* we can only do 8 bit transfers */
	if (bitlen % 8)
	{
		flags |= SPI_XFER_END;
		return -1;
	}

	#if defined(CONFIG_SPI_APB_DMA_MODE) && (defined(CONFIG_NT72656) || defined(CONFIG_NT72668))
	spi_init_apbdma_tzasc();
	#endif
	#if defined(CONFIG_SPI_AHB_DMA_MODE) && (defined(CONFIG_NT72656) || defined(CONFIG_NT72668))
	spi_init_ahbdma_tzasc();
	#endif

	spi_reg = &spi_regs;

	if (flags & SPI_XFER_BEGIN)
	{
		memset(&spi_regs, '\0', sizeof(spi_regs_t));
		memset(&spi_params, '\0', sizeof(spi_params_t));

		if(spi_cmd_parse(spi_reg, dout, &bytes, &spi_params) == -1)
		{
			debug("spi_xfer: flash command parse failed\n");
			return -1;
		}

		spi_cs_activate(slave);

		spi_dir_toflash(spi_reg);
		if (spi_params.dir) {
			spi_dir_fromflash(spi_reg);
		}

		spi_addr_disable(spi_reg);
		if (spi_params.address_en) {
			spi_addr_enable(spi_reg);
			spi_address(spi_reg, spi_params.address);
		}

		spi_dummy_disable(spi_reg);
		if (spi_params.dummy_en) {
			spi_dummy_enable(spi_reg);
			spi_dummy_count(spi_reg, spi_params.dummy_count);
		}

		spi_command(spi_reg, spi_params.command);

		if ((flags & SPI_XFER_END) && (!bytes)) {
			spi_data_disable(spi_reg);
			spi_access_start(spi_reg);
			spi_cs_deactivate(slave);
			return 0;
		}
	}

	if (flags & SPI_XFER_END) {
		goto done;
	}

	return 0;

done:
	if (spi_params.dir == XFER_DIR_FROM_SPI)
	{
		/* spi_xfer will also be used by spi_flash_cmd
		 * and it will read short bytes data as reponse
		 * in such case, DMA is not suitable
		 */
		if(bytes < 16)
		{
			//u8 u8i;
			// keep the code as it was
			while(bytes) {
				xfer_len = ((bytes/16) ? 16 : bytes);
				spi_data_enable(spi_reg);
				spi_data_count(spi_reg, xfer_len);

				//access start and read data.
				ret = spi_data_read(spi_reg, rxd, xfer_len);
				if (ret)
					goto exit;

				bytes              -= xfer_len;
				rxd                += xfer_len;
				spi_params.address += xfer_len;
				spi_address(spi_reg, spi_params.address);
			}
		}
		else
		{
			// for larger data size, use DMA
			xfer_len = bytes;

			#ifdef CONFIG_SPI_FLASH_WINBOND
			if (spi_params.command == CMD_WINBOND_FAST_READ_DO || spi_params.command == CMD_WINBOND_FAST_READ_DIO)
			{
				spi_width_select(spi_reg, XFER_DATA_WIDTH_2_BIT);
			}
			else if (spi_params.command == CMD_WINBOND_FAST_READ_QO)
			{
				spi_width_select(spi_reg, XFER_DATA_WIDTH_4_BIT);
			}
			else
			#endif
			{
				spi_width_select(spi_reg, XFER_DATA_WIDTH_1_BIT);
			}

			#ifdef CONFIG_SPI_PIO_MODE
			unsigned int actual_len = 0;
			while(xfer_len > 0)
			{
				actual_len = xfer_len > 16 ? 16 : xfer_len;
				spi_data_read(spi_reg, rxd, actual_len);

				rxd      += actual_len;
				xfer_len -= actual_len;
			}
			#endif

			#ifdef CONFIG_SPI_APB_DMA_MODE
			spi_data_apbdma_count(spi_reg, xfer_len);
			ret = spi_data_apbdma_read(spi_reg, rxd, xfer_len);
			#endif

			#ifdef CONFIG_SPI_AHB_DMA_MODE
			ret = spi_data_ahbdma_read(spi_reg, rxd, xfer_len);
			#endif

			spi_width_select(spi_reg, XFER_DATA_WIDTH_DEFAULT);
		}
	}
	else if (spi_params.dir == XFER_DIR_TO_SPI)
	{
		if (spi_params.command == CMD_SPI_WRSR)
		{
			xfer_len = bytes;
			spi_data_enable(spi_reg);
			spi_data_count(spi_reg, xfer_len);
			if(xfer_len == 2)
			{
				//for winbond only
				spi_out8((SPI_DATA0_REG + 0), ((unsigned char *)txd)[1]);
				spi_out8((SPI_DATA0_REG + 1), ((unsigned char *)txd)[2]);
			}
			else
			{
				//for general spi flash.
				spi_out8((SPI_DATA0_REG + 0), ((unsigned char *)txd)[1]);
			}
			spi_access_start(spi_reg);
		}
		else
		{
			xfer_len = bytes;

			#ifdef CONFIG_SPI_PIO_MODE
			unsigned int actual_len = 0;
			while(xfer_len > 0)
			{
				actual_len = xfer_len > 16 ? 16 : xfer_len;
				spi_data_write(spi_reg, txd, actual_len);

				txd      += actual_len;
				xfer_len -= actual_len;
			}
			#endif

			#ifdef CONFIG_SPI_APB_DMA_MODE
			spi_data_apbdma_count(spi_reg, xfer_len);
			ret = spi_data_apbdma_write(spi_reg, txd, xfer_len);
			#endif

			#ifdef CONFIG_SPI_AHB_DMA_MODE
			ret = spi_data_ahbdma_write(spi_reg, txd, xfer_len);
			#endif
		}
	}

	spi_data_disable(spi_reg);

exit:
	spi_cs_deactivate(slave);
	return ret;
}
