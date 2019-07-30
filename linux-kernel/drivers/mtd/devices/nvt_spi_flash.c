/*
 *  drivers/mtd/devices/nvt_spi_flash.c
 *
 *  Author:	Alvin lin
 *  Created:	Sep 10, 2015
 *  Copyright:	Novatek Inc.
 *  Novatek SPI flash driver
 */
#include <linux/init.h>
#include <linux/err.h>
#include <linux/errno.h>
#include <linux/module.h>
#include <linux/device.h>
#include <linux/interrupt.h>
#include <linux/mutex.h>
#include <linux/math64.h>
#include <linux/slab.h>
#include <linux/sched.h>
#include <linux/mod_devicetable.h>

#include <linux/mtd/cfi.h>
#include <linux/mtd/mtd.h>
#include <linux/mtd/partitions.h>
#include <linux/dma-mapping.h>
#include <linux/of.h>
#include <linux/of_irq.h>
#include <linux/of_device.h>
#include <linux/of_platform.h>

#include <linux/spi/spi.h>
#include <linux/spi/flash.h>
#include "nvt_spi_host_reg.h"
#include "../../soc/nvt/include/nvt_stbc.h"
#include "nvt_spi.h"

/****************************************************************************/
#if SPI_PREFORMANCE_TEST
static void __iomem *StbcBase;
static void __iomem *mpllClk;
static void __iomem *ClkSet;
#endif

struct flash_info {
	/* JEDEC id zero means "no ID" (most older chips); otherwise it has
	 * a high byte of zero plus three data bytes: the manufacturer id,
	 * then a two byte device id.
	 */
	u32		jedec_id;
	u16             ext_id;

	/* The size listed here is what works with OPCODE_SE, which isn't
	 * necessarily called a "sector" by the vendor.
	 */
	unsigned	sector_size;
	u16		n_sectors;

	u16		page_size;
	u16		addr_width;

	u16		flags;
#define	SECT_4K		0x01	/* OPCODE_BE_4K works uniformly */
#define	NVT_SPI_FLASH_NO_ERASE	0x02	/* No erase command needed */
#define	SST_WRITE	0x04	/* use SST byte programming */
};


struct nvt_spi_master {
	void __iomem *regs;
	int irq;
	struct completion done;
	dma_addr_t dma_buf_phy;
	u8 *dma_buf_addr;
	int len;
};


struct nvt_spi_flash {
	struct nvt_spi_master	spi;
	struct mutex		lock;
	struct mtd_info		mtd;
	u16			page_size;
	u16			addr_width;
	u8			erase_opcode;
	u8			*command;
	bool			fast_read;
	struct flash_info info;
};

static struct nvt_spi_master *int_ns;

static inline struct nvt_spi_flash *mtd_to_nvt_spi_flash(struct mtd_info *mtd)
{
	return container_of(mtd, struct nvt_spi_flash, mtd);
}

static inline u32 nvt_spi_rd(struct nvt_spi_master *ns, unsigned reg)
{
	return readl(ns->regs + reg);
}

static inline void nvt_spi_wr(struct nvt_spi_master *ns, unsigned reg, u32 val)
{
	writel(val, ns->regs + reg);
}


/****************************************************************************/
#if SPI_PREFORMANCE_TEST
static void spi_initRelatedHW(void)
{
	StbcBase = ioremap_nocache(NVT_REG_STBC_BASE, 0x1000);
	mpllClk = ioremap_nocache(NVT_REG_MPLL_BUS_BASE, 0x1000);
	ClkSet = ioremap_nocache(NVT_REG_CLK_SET_BASE, 0x1000);
}
#endif
/*
 * Internal helper functions
 */
static int nvt_lock_spi(struct nvt_spi_flash *flash)
{
	ssize_t retval;
	u32 reg_val, ontime;
	struct nvt_spi_master *ns = &(flash->spi);

	retval = nvt_stbc_stay_in_sram();
	msleep(20);

	reg_val = (NVT_SPI_TRANS_FROM_DEV | NVT_SPI_LOCK_ON |
					NVT_SPI_DEFAULT_BYTE);
	nvt_spi_wr(ns, NVT_SPI_CTRL_REG, reg_val);

	reg_val = OPCODE_RDSR;
	nvt_spi_wr(ns, NVT_SPI_CMD_ADDR_REG, reg_val);

	reg_val = NVT_SPI_DATA_EN;
	nvt_spi_wr(ns, NVT_SPI_ACC_CTRL_REG, reg_val);

	init_completion(&ns->done);

	reg_val |= NVT_SPI_ACC_START;
	nvt_spi_wr(ns, NVT_SPI_ACC_CTRL_REG, reg_val);

	ontime = wait_for_completion_timeout(&(ns->done),
					msecs_to_jiffies(NVT_SPI_WAIT_TIMEOUT));

	if (!ontime) {
		/* kwinyee debug */
		pr_err("%s timeout !\n", __func__);
		retval = -ETIMEDOUT;
		goto out;
	}

	return retval;
out:
	/* nvt_stbc_reset(); */
	nvt_stbc_exit_idle();
	msleep(20);
	return retval;
}
static int nvt_unlock_spi(struct nvt_spi_flash *flash)
{
	ssize_t retval;
	u32 reg_val, ontime;
	struct nvt_spi_master *ns = &(flash->spi);

	retval = nvt_stbc_stay_in_sram();
	msleep(20);

	reg_val = (NVT_SPI_TRANS_FROM_DEV | NVT_SPI_DEFAULT_BYTE);
	nvt_spi_wr(ns, NVT_SPI_CTRL_REG, reg_val);

	reg_val = OPCODE_RDSR;
	nvt_spi_wr(ns, NVT_SPI_CMD_ADDR_REG, reg_val);

	reg_val = NVT_SPI_DATA_EN;
	nvt_spi_wr(ns, NVT_SPI_ACC_CTRL_REG, reg_val);

	init_completion(&ns->done);

	reg_val |= NVT_SPI_ACC_START;
	nvt_spi_wr(ns, NVT_SPI_ACC_CTRL_REG, reg_val);

	ontime = wait_for_completion_timeout(&(ns->done),
					msecs_to_jiffies(NVT_SPI_WAIT_TIMEOUT));

	if (!ontime) {
		/* kwinyee debug */
		pr_err("%s timeout !\n", __func__);
		retval = -ETIMEDOUT;
		goto out;
	}

	retval = nvt_spi_rd(ns, NVT_SPI_ACC_DWORD0_REG);
	/* kwinyee debug */
	/* printk(KERN_INFO "status = 0x%x\n", retval); */
out:
	/* nvt_stbc_reset(); */
	nvt_stbc_exit_idle();
	msleep(20);
	return retval;
}


/*
 * Read the status register, returning its value in the location
 * Return the status register value.
 * Returns negative if error occurred.
 */
static int read_sr(struct nvt_spi_flash *flash)
{
	ssize_t retval;
	u32 reg_val, ontime;
	struct nvt_spi_master *ns = &(flash->spi);

	reg_val = (NVT_SPI_TRANS_FROM_DEV | NVT_SPI_LOCK_ON |
					NVT_SPI_DEFAULT_BYTE);
	nvt_spi_wr(ns, NVT_SPI_CTRL_REG, reg_val);

	reg_val = OPCODE_RDSR;
	nvt_spi_wr(ns, NVT_SPI_CMD_ADDR_REG, reg_val);

	reg_val = NVT_SPI_DATA_EN;
	nvt_spi_wr(ns, NVT_SPI_ACC_CTRL_REG, reg_val);

	init_completion(&ns->done);

	reg_val |= NVT_SPI_ACC_START;
	nvt_spi_wr(ns, NVT_SPI_ACC_CTRL_REG, reg_val);

	ontime = wait_for_completion_timeout(&(ns->done),
					msecs_to_jiffies(NVT_SPI_WAIT_TIMEOUT));

	if (!ontime) {
		/* kwinyee debug */
		pr_err("%s timeout !\n", __func__);
		return -ETIMEDOUT;
	}

	retval = nvt_spi_rd(ns, NVT_SPI_ACC_DWORD0_REG);
	/* kwinyee debug */
	/* printk(KERN_INFO "status = 0x%x\n", retval); */

	return retval;
}

/*
 * Write status register 1 byte
 * Returns negative if error occurred.
 */
static int write_sr(struct nvt_spi_flash *flash, u8 val)
{
	u32 reg_val, ontime;
	struct nvt_spi_master *ns = &(flash->spi);

	reg_val = NVT_SPI_LOCK_ON;
	nvt_spi_wr(ns, NVT_SPI_CTRL_REG, reg_val);

	reg_val = OPCODE_WRSR;
	nvt_spi_wr(ns, NVT_SPI_CMD_ADDR_REG, reg_val);

	nvt_spi_wr(ns, NVT_SPI_ACC_DWORD0_REG, val);

	reg_val = (NVT_SPI_DATA_EN | NVT_SPI_DATA_CNT(2));
	nvt_spi_wr(ns, NVT_SPI_ACC_CTRL_REG, reg_val);

	init_completion(&ns->done);

	reg_val |= NVT_SPI_ACC_START;
	nvt_spi_wr(ns, NVT_SPI_ACC_CTRL_REG, reg_val);

	ontime = wait_for_completion_timeout(&(ns->done),
					msecs_to_jiffies(NVT_SPI_WAIT_TIMEOUT));

	if (!ontime)
		return -ETIMEDOUT;
	else
		return 0;
}

/*
 * Set write enable latch with Write Enable command.
 * Returns negative if error occurred.
 */
static inline int write_enable(struct nvt_spi_flash *flash)
{
	u32 reg_val, ontime;
	struct nvt_spi_master *ns = &(flash->spi);
	int status = read_sr(flash);

	if (status & 0x1)
		return -EBUSY;

	reg_val = (NVT_SPI_LOCK_ON | NVT_SPI_DEFAULT_BYTE);
	nvt_spi_wr(ns, NVT_SPI_CTRL_REG, reg_val);

	reg_val = OPCODE_WREN;
	nvt_spi_wr(ns, NVT_SPI_CMD_ADDR_REG, reg_val);

	init_completion(&ns->done);

	nvt_spi_wr(ns, NVT_SPI_ACC_CTRL_REG, 0);
	reg_val = NVT_SPI_ACC_START;
	nvt_spi_wr(ns, NVT_SPI_ACC_CTRL_REG, reg_val);

	ontime = wait_for_completion_timeout(&(ns->done),
					msecs_to_jiffies(NVT_SPI_WAIT_TIMEOUT));
	if (!ontime)
		return -ETIMEDOUT;

	return read_sr(flash);
}

/*
 * Send write disble instruction to the chip.
 */
static inline int write_disable(struct nvt_spi_flash *flash)
{
	u32 reg_val, ontime;
	struct nvt_spi_master *ns = &(flash->spi);
	int status = read_sr(flash);

	if (status & 0x1)
		return -EBUSY;

	reg_val = (NVT_SPI_LOCK_ON | NVT_SPI_DEFAULT_BYTE);
	nvt_spi_wr(ns, NVT_SPI_CTRL_REG, reg_val);

	reg_val = OPCODE_WRDI;
	nvt_spi_wr(ns, NVT_SPI_CMD_ADDR_REG, reg_val);

	init_completion(&ns->done);

	nvt_spi_wr(ns, NVT_SPI_ACC_CTRL_REG, 0);
	reg_val = NVT_SPI_ACC_START;
	nvt_spi_wr(ns, NVT_SPI_ACC_CTRL_REG, reg_val);

	ontime = wait_for_completion_timeout(&(ns->done),
					msecs_to_jiffies(NVT_SPI_WAIT_TIMEOUT));
	if (!ontime)
		return -ETIMEDOUT;

	return read_sr(flash);
}

/*
 * Enable/disable 4-byte addressing mode.
 */
static inline int set_4byte(struct nvt_spi_flash *flash,
					u32 jedec_id, int enable)
{
	u32 reg_val, ontime;
	struct nvt_spi_master *ns = &(flash->spi);

	switch (JEDEC_MFR(jedec_id)) {
	case CFI_MFR_MACRONIX:
	case 0xEF /* winbond */:
		reg_val = (NVT_SPI_LOCK_ON | NVT_SPI_DEFAULT_BYTE);
		nvt_spi_wr(ns, NVT_SPI_CTRL_REG, reg_val);

		reg_val = enable ? OPCODE_EN4B : OPCODE_EX4B;
		nvt_spi_wr(ns, NVT_SPI_CMD_ADDR_REG, reg_val);

		init_completion(&ns->done);

		nvt_spi_wr(ns, NVT_SPI_ACC_CTRL_REG, 0);
		reg_val = NVT_SPI_ACC_START;
		nvt_spi_wr(ns, NVT_SPI_ACC_CTRL_REG, reg_val);

		ontime = wait_for_completion_timeout(&(ns->done),
					msecs_to_jiffies(NVT_SPI_WAIT_TIMEOUT));
		if (!ontime)
			return -ETIMEDOUT;
		else
			return 0;
	default:
		/* Spansion style */
		reg_val = (NVT_SPI_LOCK_ON | NVT_SPI_DEFAULT_BYTE);
		nvt_spi_wr(ns, NVT_SPI_CTRL_REG, reg_val);

		reg_val = OPCODE_BRWR;
		nvt_spi_wr(ns, NVT_SPI_CMD_ADDR_REG, reg_val);

		nvt_spi_wr(ns, NVT_SPI_ACC_DWORD0_REG, (enable << 7));

		reg_val = (NVT_SPI_DATA_EN | NVT_SPI_DATA_CNT(1));
		nvt_spi_wr(ns, NVT_SPI_ACC_CTRL_REG, reg_val);

		init_completion(&ns->done);

		reg_val |= NVT_SPI_ACC_START;
		nvt_spi_wr(ns, NVT_SPI_ACC_CTRL_REG, reg_val);

		ontime = wait_for_completion_timeout(&(ns->done),
					msecs_to_jiffies(NVT_SPI_WAIT_TIMEOUT));
		if (!ontime)
			return -ETIMEDOUT;
		else
			return 0;
	}
}

/*
 * Service routine to read status register until ready, or timeout occurs.
 * Returns non-zero if error.
 */
static int wait_till_ready(struct nvt_spi_flash *flash)
{
	unsigned long deadline;
	int sr;

	deadline = jiffies + MAX_READY_WAIT_JIFFIES;

	do {
		sr = read_sr(flash);
		if (sr < 0)
			return 1;
		else if (!(sr & SR_WIP))
			break;

		cond_resched();

	} while (!time_after_eq(jiffies, deadline));

	if ((sr & SR_WIP) == 0)
		return 0;

	return 1;
}

/*
 * modify spi status reg1 to 0 => none protect
 */
static int write_unprotect(struct nvt_spi_flash *flash)
{
	u8 status = 0;

	nvt_lock_spi(flash);
	write_enable(flash);
	status &= ~(SR_BP0 | SR_BP1 | SR_BP2);
	write_sr(flash, 0);
	nvt_unlock_spi(flash);
	return 0;
}

static int write_protect(struct nvt_spi_flash *flash)
{
	u8 status = 0;

	nvt_lock_spi(flash);
	write_enable(flash);
	status |= (SR_BP0 | SR_BP1 | SR_BP2);
	write_sr(flash, status);
	nvt_unlock_spi(flash);
	return 0;
}


/*
 * Erase the whole flash memory
 *
 * Returns 0 if successful, non-zero otherwise.
 */
static int erase_chip(struct nvt_spi_flash *flash)
{
	u32 reg_val, ontime;
	struct nvt_spi_master *ns = &(flash->spi);

	/* Wait until finished previous write command. */
	if (wait_till_ready(flash))
		return 1;

	/* modify status reg1 to none protect*/
	if (write_unprotect(flash))
		return 1;

	/* Send write enable, then erase commands. */
	write_enable(flash);

	/* Set up command buffer. */
	reg_val = (NVT_SPI_LOCK_ON | NVT_SPI_DEFAULT_BYTE);
	nvt_spi_wr(ns, NVT_SPI_CTRL_REG, reg_val);

	reg_val = OPCODE_CHIP_ERASE;
	nvt_spi_wr(ns, NVT_SPI_CMD_ADDR_REG, reg_val);

	init_completion(&ns->done);

	nvt_spi_wr(ns, NVT_SPI_ACC_CTRL_REG, 0);
	reg_val = NVT_SPI_ACC_START;
	nvt_spi_wr(ns, NVT_SPI_ACC_CTRL_REG, reg_val);

	ontime = wait_for_completion_timeout(&(ns->done),
					msecs_to_jiffies(NVT_SPI_WAIT_TIMEOUT));
	if (!ontime)
		return -ETIMEDOUT;

	if (write_protect(flash))
		return 1;

	return 0;
}

/*
 * Erase one sector of flash memory at offset ``offset'' which is any
 * address within the sector which should be erased.
 *
 * Returns 0 if successful, non-zero otherwise.
 */
static int erase_sector(struct nvt_spi_flash *flash, u32 offset)
{
	u32 reg_val, ontime;
	struct nvt_spi_master *ns = &(flash->spi);

	pr_debug("%s %dKiB at 0x%08x\n", __func__,
					flash->mtd.erasesize / 1024, offset);

	/* Wait until finished previous write command. */
	if (wait_till_ready(flash))
		return 1;

	/* modify status reg1 to none protect*/
	if (write_unprotect(flash))
		return 1;

	/* Send write enable, then erase commands. */
	write_enable(flash);

	/* Set up command buffer. */
	reg_val = (NVT_SPI_LOCK_ON | NVT_SPI_ADDR_EN | NVT_SPI_DEFAULT_BYTE);
	nvt_spi_wr(ns, NVT_SPI_CTRL_REG, reg_val);

	nvt_spi_wr(ns, NVT_SPI_CMD_ADDR_REG,
			flash->erase_opcode | (offset << NVT_SPI_ADDR_SFT));

	init_completion(&ns->done);

	nvt_spi_wr(ns, NVT_SPI_ACC_CTRL_REG, 0);

	reg_val = NVT_SPI_ACC_START;

	nvt_spi_wr(ns, NVT_SPI_ACC_CTRL_REG, reg_val);

	ontime = wait_for_completion_timeout(&(ns->done),
				msecs_to_jiffies(NVT_SPI_WAIT_TIMEOUT));

	if (!ontime)
		return -ETIMEDOUT;

	if (write_protect(flash))
		return 1;

	return 0;
}

/****************************************************************************/

/*
 * MTD implementation
 */

/*
 * Erase an address range on the flash chip.  The address range may extend
 * one or more erase sectors.  Return an error is there is a problem erasing.
 */
static int nvt_spi_flash_erase(struct mtd_info *mtd, struct erase_info *instr)
{
	struct nvt_spi_flash *flash = mtd_to_nvt_spi_flash(mtd);
	u32 addr, len;
	uint32_t rem;

	pr_debug("%s at 0x%llx, len %lld\n",
			__func__, (long long)instr->addr,
			(long long)instr->len);

	div_u64_rem(instr->len, mtd->erasesize, &rem);
	if (rem)
		return -EINVAL;

	addr = instr->addr;
	len = instr->len;

	mutex_lock(&flash->lock);
	nvt_lock_spi(flash);

	/* whole-chip erase? */
	if (len == flash->mtd.size) {
		if (erase_chip(flash)) {
			instr->state = MTD_ERASE_FAILED;
			mutex_unlock(&flash->lock);
			nvt_unlock_spi(flash);
			return -EIO;
		}

	/* REVISIT in some cases we could speed up erasing large regions
	 * by using OPCODE_SE instead of OPCODE_BE_4K.  We may have set up
	 * to use "small sector erase", but that's not always optimal.
	 */

	/* "sector"-at-a-time erase */
	} else {
		while (len) {
			if (erase_sector(flash, addr)) {
				instr->state = MTD_ERASE_FAILED;
				mutex_unlock(&flash->lock);
				nvt_unlock_spi(flash);
				return -EIO;
			}

			addr += mtd->erasesize;
			len -= mtd->erasesize;
		}
	}

	mutex_unlock(&flash->lock);
	nvt_unlock_spi(flash);

	instr->state = MTD_ERASE_DONE;
	mtd_erase_callback(instr);

	return 0;
}

/*
 * Read an address range from the flash chip.  The address range
 * may be any size provided it is within the physical boundaries.
 */
static int nvt_spi_flash_read(struct mtd_info *mtd, loff_t from, size_t len,
	size_t *retlen, u_char *buf)
{
	struct nvt_spi_flash *flash = mtd_to_nvt_spi_flash(mtd);
	u32 reg_val, dma_val, ontime;
	struct nvt_spi_master *ns = &(flash->spi);
	uint8_t opcode;
	dma_addr_t phy_addr;

	pr_debug("%s from 0x%08x, len %zd\n",
			__func__, (u32)from, len);

	phy_addr = dma_map_single(&mtd->dev, (void *)buf, len, DMA_FROM_DEVICE);

	mutex_lock(&flash->lock);
	nvt_lock_spi(flash);

	/* Wait till previous write/erase is done. */
	if (wait_till_ready(flash)) {
		/* REVISIT status return?? */
		dma_unmap_single(&mtd->dev, phy_addr, len, DMA_FROM_DEVICE);
		nvt_unlock_spi(flash);
		mutex_unlock(&flash->lock);
		return 1;
	}

	/* FIXME switch to OPCODE_FAST_READ.  It's required for higher
	 * clocks; and at this writing, every chip this driver handles
	 * supports that opcode.
	 */

	/* Set up the write data buffer. */
	opcode = flash->fast_read ? OPCODE_FAST_READ : OPCODE_NORM_READ;
	if (opcode == OPCODE_FAST_READ)
		reg_val = (NVT_SPI_TRANS_FROM_DEV | NVT_SPI_LOCK_ON |
					NVT_SPI_ADDR_EN | NVT_SPI_DUMMY_EN |
					NVT_SPI_DUMMY_CLK_CNT(8) |
					NVT_SPI_DEFAULT_BYTE);
	else
		reg_val = (NVT_SPI_TRANS_FROM_DEV | NVT_SPI_LOCK_ON |
				NVT_SPI_ADDR_EN | NVT_SPI_DEFAULT_BYTE);

	nvt_spi_wr(ns, NVT_SPI_CTRL_REG, reg_val);

	reg_val = (opcode | (from << NVT_SPI_ADDR_SFT));

	nvt_spi_wr(ns, NVT_SPI_CMD_ADDR_REG, reg_val);

	nvt_spi_wr(ns, NVT_SPI_NEW_DMA_CTRL_REG, 0);

	nvt_spi_wr(ns, NVT_SPI_NEW_DMA_ADDR_REG, phy_addr);

	dma_val = NVT_SPI_NEW_DMA_TX_LEN(len) | NVT_SPI_NEW_DMA_TO_DDR;

	init_completion(&ns->done);

	dma_val |= NVT_SPI_NEW_DMA_EN;
	nvt_spi_wr(ns, NVT_SPI_NEW_DMA_CTRL_REG, dma_val);

	ontime = wait_for_completion_timeout(&(ns->done),
			msecs_to_jiffies(NVT_SPI_WAIT_TIMEOUT));

	mutex_unlock(&flash->lock);
	nvt_unlock_spi(flash);

	if (!ontime) {
		*retlen = 0;
		dma_unmap_single(&mtd->dev, phy_addr, len, DMA_FROM_DEVICE);
		return -ETIMEDOUT;
	}
	*retlen = len;

	dma_unmap_single(&mtd->dev, phy_addr, len, DMA_FROM_DEVICE);

	return 0;
}

/*
 * Write an address range to the flash chip.  Data must be written in
 * FLASH_PAGESIZE chunks.  The address range may be any size provided
 * it is within the physical boundaries.
 */
static int nvt_spi_flash_write(struct mtd_info *mtd, loff_t to, size_t len,
		size_t *retlen, const u_char *buf)
{
	struct nvt_spi_flash *flash = mtd_to_nvt_spi_flash(mtd);
	u32 reg_val, dma_val, ontime;
	struct nvt_spi_master *ns = &(flash->spi);
	u32 page_offset, page_size;
	dma_addr_t phy_addr;

	pr_debug("%s to 0x%08x, len %zd\n",
			__func__, (u32)to, len);

	phy_addr = dma_map_single(&mtd->dev, (void *)buf, len, DMA_TO_DEVICE);
	if (dma_mapping_error(&mtd->dev, phy_addr)) {
		pr_err("%s mapping fail !\n", __func__);
		return 1;
	}

	mutex_lock(&flash->lock);
	nvt_lock_spi(flash);

	/* Wait until finished previous write command. */
	if (wait_till_ready(flash)) {
		mutex_unlock(&flash->lock);
		nvt_unlock_spi(flash);
		return 1;
	}

	/* modify status reg1 to none protect*/
	if (write_unprotect(flash))
		return 1;

	write_enable(flash);

	/* Set up the opcode in the write buffer. */

	page_offset = to & (flash->page_size - 1);

	/* do all the bytes fit onto one page? */
	if (page_offset + len <= flash->page_size) {

		reg_val = (NVT_SPI_LOCK_ON | NVT_SPI_ADDR_EN |
					NVT_SPI_DEFAULT_BYTE);
		nvt_spi_wr(ns, NVT_SPI_CTRL_REG, reg_val);

		reg_val = OPCODE_PP | (to << NVT_SPI_ADDR_SFT);
		nvt_spi_wr(ns, NVT_SPI_CMD_ADDR_REG, reg_val);

		nvt_spi_wr(ns, NVT_SPI_NEW_DMA_CTRL_REG, 0);
		nvt_spi_wr(ns, NVT_SPI_NEW_DMA_ADDR_REG, phy_addr);

		dma_val = NVT_SPI_NEW_DMA_TX_LEN(len);
		nvt_spi_wr(ns, NVT_SPI_NEW_DMA_CTRL_REG, dma_val);

		init_completion(&ns->done);

		dma_val |= NVT_SPI_NEW_DMA_EN;
		nvt_spi_wr(ns, NVT_SPI_NEW_DMA_CTRL_REG, dma_val);

		ontime = wait_for_completion_timeout(&(ns->done),
					msecs_to_jiffies(NVT_SPI_WAIT_TIMEOUT));

		if (!ontime) {
			*retlen = 0;
			pr_err("[SPI]: wait for completion timeout!\n");
			goto time_out;
		}
		else
			*retlen = len;

	} else {
		u32 i;

		/* the size of data remaining on the first page */
		page_size = flash->page_size - page_offset;

		reg_val = (NVT_SPI_LOCK_ON | NVT_SPI_ADDR_EN |
					NVT_SPI_DEFAULT_BYTE);
		nvt_spi_wr(ns, NVT_SPI_CTRL_REG, reg_val);

		reg_val = OPCODE_PP | (to << NVT_SPI_ADDR_SFT);
		nvt_spi_wr(ns, NVT_SPI_CMD_ADDR_REG, reg_val);

		nvt_spi_wr(ns, NVT_SPI_NEW_DMA_CTRL_REG, 0);
		nvt_spi_wr(ns, NVT_SPI_NEW_DMA_ADDR_REG, phy_addr);

		dma_val = NVT_SPI_NEW_DMA_TX_LEN(page_size);

		init_completion(&ns->done);

		dma_val |= NVT_SPI_NEW_DMA_EN;
		nvt_spi_wr(ns, NVT_SPI_NEW_DMA_CTRL_REG, dma_val);

		ontime = wait_for_completion_timeout(&(ns->done),
				msecs_to_jiffies(NVT_SPI_WAIT_TIMEOUT));

		if (!ontime) {
			*retlen = 0;
			pr_err("[SPI]:wait for completion timeout!\n");
			goto time_out;
		}
		else
			*retlen = page_size;

		if (wait_till_ready(flash)) {
			pr_err("[SPI]:wait busy timeout!\n");
			goto time_out;
		}
		write_enable(flash);

		/* write everything in flash->page_size chunks */
		for (i = page_size; i < len; i += page_size) {
			page_size = len - i;
			if (page_size > flash->page_size)
				page_size = flash->page_size;

			reg_val = (NVT_SPI_LOCK_ON | NVT_SPI_ADDR_EN |
					NVT_SPI_DEFAULT_BYTE);
			nvt_spi_wr(ns, NVT_SPI_CTRL_REG, reg_val);

			reg_val = OPCODE_PP | ((to + i) << NVT_SPI_ADDR_SFT);
			nvt_spi_wr(ns, NVT_SPI_CMD_ADDR_REG, reg_val);

			nvt_spi_wr(ns, NVT_SPI_NEW_DMA_CTRL_REG, 0);

			nvt_spi_wr(ns, NVT_SPI_NEW_DMA_ADDR_REG,
					(phy_addr + i));

			dma_val = NVT_SPI_NEW_DMA_TX_LEN(page_size);

			init_completion(&ns->done);

			dma_val |= NVT_SPI_NEW_DMA_EN;
			nvt_spi_wr(ns, NVT_SPI_NEW_DMA_CTRL_REG, dma_val);

			ontime = wait_for_completion_timeout(&(ns->done),
					msecs_to_jiffies(NVT_SPI_WAIT_TIMEOUT));

			if (!ontime) {
				*retlen = 0;
				pr_err("[SPI]:wait for completion timeout!\n");
				goto time_out;
			}
			else
				*retlen += page_size;

			if (wait_till_ready(flash)) {
				pr_err("[SPI]:wait busy timeout!\n");
				goto time_out;
			}

			write_enable(flash);
		}
	}

time_out:
	dma_unmap_single(&mtd->dev, phy_addr, len, DMA_TO_DEVICE);

	if (write_protect(flash))
		return 1;

	mutex_unlock(&flash->lock);
	nvt_unlock_spi(flash);

	return 0;
}

static int sst_write(struct mtd_info *mtd, loff_t to, size_t len,
		size_t *retlen, const u_char *buf)
{
	struct nvt_spi_flash *flash = mtd_to_nvt_spi_flash(mtd);
	u32 reg_val, ontime;
	struct nvt_spi_master *ns = &(flash->spi);
	size_t actual;
	int ret;

	pr_debug("%s to 0x%08x, len %zd\n",
			__func__, (u32)to, len);

	mutex_lock(&flash->lock);
	nvt_lock_spi(flash);

	/* Wait until finished previous write command. */
	ret = wait_till_ready(flash);
	if (ret)
		goto time_out;

	/* modify status reg1 to none protect*/
	if (write_unprotect(flash))
		return 1;

	write_enable(flash);

	actual = to % 2;
	/* Start write from odd address. */
	if (actual) {
		reg_val = (NVT_SPI_LOCK_ON | NVT_SPI_ADDR_EN |
					NVT_SPI_DEFAULT_BYTE);
		nvt_spi_wr(ns, NVT_SPI_CTRL_REG, reg_val);

		reg_val = (OPCODE_BP | (to << NVT_SPI_ADDR_SFT));
		nvt_spi_wr(ns, NVT_SPI_CMD_ADDR_REG, reg_val);

		nvt_spi_wr(ns, NVT_SPI_ACC_DWORD0_REG, *buf);

		reg_val = NVT_SPI_DATA_EN | NVT_SPI_DATA_CNT(1);
		nvt_spi_wr(ns, NVT_SPI_ACC_CTRL_REG, reg_val);

		init_completion(&ns->done);

		reg_val |= NVT_SPI_ACC_START;
		nvt_spi_wr(ns, NVT_SPI_ACC_CTRL_REG, reg_val);

		ontime = wait_for_completion_timeout(&(ns->done),
					msecs_to_jiffies(NVT_SPI_WAIT_TIMEOUT));
		if (!ontime)
			goto time_out;
		/* write one byte. */
		ret = wait_till_ready(flash);
		if (ret)
			goto time_out;
		*retlen += 1;
	}
	to += actual;


	/* Write out most of the data here. */
	for (; actual < len - 1; actual += 2) {
		reg_val = (NVT_SPI_LOCK_ON | NVT_SPI_ADDR_EN |
					NVT_SPI_DEFAULT_BYTE);
		nvt_spi_wr(ns, NVT_SPI_CTRL_REG, reg_val);

		reg_val = (OPCODE_AAI_WP | to << NVT_SPI_ADDR_SFT);
		nvt_spi_wr(ns, NVT_SPI_CMD_ADDR_REG, reg_val);

		nvt_spi_wr(ns, NVT_SPI_ACC_DWORD0_REG, *(buf + actual));

		reg_val = NVT_SPI_DATA_EN | NVT_SPI_DATA_CNT(2);
		nvt_spi_wr(ns, NVT_SPI_ACC_CTRL_REG, reg_val);

		init_completion(&ns->done);

		reg_val |= NVT_SPI_ACC_START;
		nvt_spi_wr(ns, NVT_SPI_ACC_CTRL_REG, reg_val);

		ontime = wait_for_completion_timeout(&(ns->done),
					msecs_to_jiffies(NVT_SPI_WAIT_TIMEOUT));
		if (!ontime)
			goto time_out;

		ret = wait_till_ready(flash);
		if (ret)
			goto time_out;
		*retlen += 2;
		to += 2;
	}
	write_disable(flash);
	ret = wait_till_ready(flash);
	if (ret)
		goto time_out;

	/* Write out trailing byte if it exists. */
	if (actual != len) {
		write_enable(flash);

		reg_val = (NVT_SPI_LOCK_ON | NVT_SPI_ADDR_EN |
					NVT_SPI_DEFAULT_BYTE);
		nvt_spi_wr(ns, NVT_SPI_CTRL_REG, reg_val);

		reg_val = (OPCODE_BP | (to << NVT_SPI_ADDR_SFT));
		nvt_spi_wr(ns, NVT_SPI_CMD_ADDR_REG, reg_val);

		nvt_spi_wr(ns, NVT_SPI_ACC_DWORD0_REG, *(buf + actual));

		reg_val = NVT_SPI_DATA_EN | NVT_SPI_DATA_CNT(1);
		nvt_spi_wr(ns, NVT_SPI_ACC_CTRL_REG, reg_val);

		init_completion(&ns->done);

		reg_val |= NVT_SPI_ACC_START;
		nvt_spi_wr(ns, NVT_SPI_ACC_CTRL_REG, reg_val);

		ontime = wait_for_completion_timeout(&(ns->done),
					msecs_to_jiffies(NVT_SPI_WAIT_TIMEOUT));
		if (!ontime)
			goto time_out;

		ret = wait_till_ready(flash);
		if (ret)
			goto time_out;

		*retlen += 1;
		write_disable(flash);

	}

time_out:
	mutex_unlock(&flash->lock);
	nvt_unlock_spi(flash);
	if (write_protect(flash))
		return 1;
	return ret;
}

static int nvt_spi_flash_lock(struct mtd_info *mtd, loff_t ofs, uint64_t len)
{
	struct nvt_spi_flash *flash = mtd_to_nvt_spi_flash(mtd);
	uint32_t offset = ofs;
	uint8_t status_old, status_new;
	int res = 0;

	mutex_lock(&flash->lock);
	nvt_lock_spi(flash);
	/* Wait until finished previous command */
	if (wait_till_ready(flash)) {
		res = 1;
		goto err;
	}

	status_old = read_sr(flash);

	if (offset < flash->mtd.size-(flash->mtd.size/2))
		status_new = status_old | SR_BP2 | SR_BP1 | SR_BP0;
	else if (offset < flash->mtd.size-(flash->mtd.size/4))
		status_new = (status_old & ~SR_BP0) | SR_BP2 | SR_BP1;
	else if (offset < flash->mtd.size-(flash->mtd.size/8))
		status_new = (status_old & ~SR_BP1) | SR_BP2 | SR_BP0;
	else if (offset < flash->mtd.size-(flash->mtd.size/16))
		status_new = (status_old & ~(SR_BP0|SR_BP1)) | SR_BP2;
	else if (offset < flash->mtd.size-(flash->mtd.size/32))
		status_new = (status_old & ~SR_BP2) | SR_BP1 | SR_BP0;
	else if (offset < flash->mtd.size-(flash->mtd.size/64))
		status_new = (status_old & ~(SR_BP2|SR_BP0)) | SR_BP1;
	else
		status_new = (status_old & ~(SR_BP2|SR_BP1)) | SR_BP0;

	/* Only modify protection if it will not unlock other areas */
	if ((status_new&(SR_BP2|SR_BP1|SR_BP0)) >
					(status_old&(SR_BP2|SR_BP1|SR_BP0))) {
		write_enable(flash);
		if (write_sr(flash, status_new) < 0) {
			res = 1;
			goto err;
		}
	}

err:	mutex_unlock(&flash->lock);
	nvt_unlock_spi(flash);
	return res;
}

static int nvt_spi_flash_unlock(struct mtd_info *mtd, loff_t ofs, uint64_t len)
{
	struct nvt_spi_flash *flash = mtd_to_nvt_spi_flash(mtd);
	uint32_t offset = ofs;
	uint8_t status_old, status_new;
	int res = 0;

	mutex_lock(&flash->lock);
	nvt_lock_spi(flash);
	/* Wait until finished previous command */
	if (wait_till_ready(flash)) {
		res = 1;
		goto err;
	}

	status_old = read_sr(flash);

	if (offset+len > flash->mtd.size-(flash->mtd.size/64))
		status_new = status_old & ~(SR_BP2|SR_BP1|SR_BP0);
	else if (offset+len > flash->mtd.size-(flash->mtd.size/32))
		status_new = (status_old & ~(SR_BP2|SR_BP1)) | SR_BP0;
	else if (offset+len > flash->mtd.size-(flash->mtd.size/16))
		status_new = (status_old & ~(SR_BP2|SR_BP0)) | SR_BP1;
	else if (offset+len > flash->mtd.size-(flash->mtd.size/8))
		status_new = (status_old & ~SR_BP2) | SR_BP1 | SR_BP0;
	else if (offset+len > flash->mtd.size-(flash->mtd.size/4))
		status_new = (status_old & ~(SR_BP0|SR_BP1)) | SR_BP2;
	else if (offset+len > flash->mtd.size-(flash->mtd.size/2))
		status_new = (status_old & ~SR_BP1) | SR_BP2 | SR_BP0;
	else
		status_new = (status_old & ~SR_BP0) | SR_BP2 | SR_BP1;

	/* Only modify protection if it will not lock other areas */
	if ((status_new&(SR_BP2|SR_BP1|SR_BP0)) <
					(status_old&(SR_BP2|SR_BP1|SR_BP0))) {
		write_enable(flash);
		if (write_sr(flash, status_new) < 0) {
			res = 1;
			goto err;
		}
	}

err:	mutex_unlock(&flash->lock);
	nvt_unlock_spi(flash);
	return res;
}

/****************************************************************************/

/*
 * SPI device driver setup and teardown
 */

#define INFO(_jedec_id, _ext_id, _sector_size, _n_sectors, _flags)	\
	((kernel_ulong_t)&(struct flash_info) {		\
		.jedec_id = (_jedec_id),			\
		.ext_id = (_ext_id),			\
		.sector_size = (_sector_size),			\
		.n_sectors = (_n_sectors),			\
		.page_size = 256,			\
		.flags = (_flags),			\
	})

#define CAT25_INFO(_sector_size, _n_sectors, _page_size, _addr_width)	\
	((kernel_ulong_t)&(struct flash_info) {			\
		.sector_size = (_sector_size),			\
		.n_sectors = (_n_sectors),			\
		.page_size = (_page_size),			\
		.addr_width = (_addr_width),			\
		.flags = NVT_SPI_FLASH_NO_ERASE,			\
	})

/* NOTE: double check command sets and memory organization when you add
 * more flash chips.  This current list focusses on newer chips, which
 * have been converging on command sets which including JEDEC ID.
 */
static const struct spi_device_id nvt_spi_flash_ids[] = {
	/* Atmel -- some are (confusingly) marketed as "DataFlash" */
	{ "at25fs010",  INFO(0x1f6601, 0, 32 * 1024,   4, SECT_4K) },
	{ "at25fs040",  INFO(0x1f6604, 0, 64 * 1024,   8, SECT_4K) },

	{ "at25df041a", INFO(0x1f4401, 0, 64 * 1024,   8, SECT_4K) },
	{ "at25df321a", INFO(0x1f4701, 0, 64 * 1024,  64, SECT_4K) },
	{ "at25df641",  INFO(0x1f4800, 0, 64 * 1024, 128, SECT_4K) },

	{ "at26f004",   INFO(0x1f0400, 0, 64 * 1024,  8, SECT_4K) },
	{ "at26df081a", INFO(0x1f4501, 0, 64 * 1024, 16, SECT_4K) },
	{ "at26df161a", INFO(0x1f4601, 0, 64 * 1024, 32, SECT_4K) },
	{ "at26df321",  INFO(0x1f4700, 0, 64 * 1024, 64, SECT_4K) },

	{ "at45db081d", INFO(0x1f2500, 0, 64 * 1024, 16, SECT_4K) },

	/* EON -- en25xxx */
	{ "en25f16", INFO(0x1c3115, 0, 64 * 1024,  32, SECT_4K) },
	{ "en25f32", INFO(0x1c3116, 0, 64 * 1024,  64, SECT_4K) },
	{ "en25p32", INFO(0x1c2016, 0, 64 * 1024,  64, 0) },
	{ "en25q32b", INFO(0x1c3016, 0, 64 * 1024,  64, 0) },
	{ "en25p64", INFO(0x1c2017, 0, 64 * 1024, 128, 0) },
	{ "en25q64", INFO(0x1c3017, 0, 64 * 1024, 128, SECT_4K) },
	{ "en25qh256", INFO(0x1c7019, 0, 64 * 1024, 512, 0) },

	/* Everspin */
	{ "mr25h256", CAT25_INFO(32 * 1024, 1, 256, 2) },

	/* GigaDevice */
	{ "gd25q32", INFO(0xc84016, 0, 64 * 1024,  64, SECT_4K) },
	{ "gd25q64", INFO(0xc84017, 0, 64 * 1024, 128, SECT_4K) },

	/* Intel/Numonyx -- xxxs33b */
	{ "160s33b",  INFO(0x898911, 0, 64 * 1024,  32, 0) },
	{ "320s33b",  INFO(0x898912, 0, 64 * 1024,  64, 0) },
	{ "640s33b",  INFO(0x898913, 0, 64 * 1024, 128, 0) },

	/* Macronix */
	{ "mx25l2005a",  INFO(0xc22012, 0, 64 * 1024,   4, SECT_4K) },
	{ "mx25l4005a",  INFO(0xc22013, 0, 64 * 1024,   8, SECT_4K) },
	{ "mx25l8005",   INFO(0xc22014, 0, 64 * 1024,  16, 0) },
	{ "mx25l1606e",  INFO(0xc22015, 0, 64 * 1024,  32, SECT_4K) },
	{ "mx25l3205d",  INFO(0xc22016, 0, 64 * 1024,  64, 0) },
	{ "mx25l6405d",  INFO(0xc22017, 0, 64 * 1024, 128, 0) },
	{ "mx25l12805d", INFO(0xc22018, 0, 64 * 1024, 256, 0) },
	{ "mx25l12855e", INFO(0xc22618, 0, 64 * 1024, 256, 0) },
	{ "mx25l25635e", INFO(0xc22019, 0, 64 * 1024, 512, 0) },
	{ "mx25l25655e", INFO(0xc22619, 0, 64 * 1024, 512, 0) },
	{ "mx66l51235l", INFO(0xc2201a, 0, 64 * 1024, 1024, 0) },

	/* Micron */
	{ "n25q064",  INFO(0x20ba17, 0, 64 * 1024, 128, 0) },
	{ "n25q128a11",  INFO(0x20bb18, 0, 64 * 1024, 256, 0) },
	{ "n25q128a13",  INFO(0x20ba18, 0, 64 * 1024, 256, 0) },
	{ "n25q256a", INFO(0x20ba19, 0, 64 * 1024, 512, SECT_4K) },

	/* Spansion -- single (large) sector size only, at least
	 * for the chips listed here (without boot sectors).
	 */
	{ "s25sl032p",  INFO(0x010215, 0x4d00,  64 * 1024,  64, 0) },
	{ "s25sl064p",  INFO(0x010216, 0x4d00,  64 * 1024, 128, 0) },
	{ "s25fl256s0", INFO(0x010219, 0x4d00, 256 * 1024, 128, 0) },
	{ "s25fl256s1", INFO(0x010219, 0x4d01,  64 * 1024, 512, 0) },
	{ "s25fl512s",  INFO(0x010220, 0x4d00, 256 * 1024, 256, 0) },
	{ "s70fl01gs",  INFO(0x010221, 0x4d00, 256 * 1024, 256, 0) },
	{ "s25sl12800", INFO(0x012018, 0x0300, 256 * 1024,  64, 0) },
	{ "s25sl12801", INFO(0x012018, 0x0301,  64 * 1024, 256, 0) },
	{ "s25fl129p0", INFO(0x012018, 0x4d00, 256 * 1024,  64, 0) },
	{ "s25fl129p1", INFO(0x012018, 0x4d01,  64 * 1024, 256, 0) },
	{ "s25sl004a",  INFO(0x010212,      0,  64 * 1024,   8, 0) },
	{ "s25sl008a",  INFO(0x010213,      0,  64 * 1024,  16, 0) },
	{ "s25sl016a",  INFO(0x010214,      0,  64 * 1024,  32, 0) },
	{ "s25sl032a",  INFO(0x010215,      0,  64 * 1024,  64, 0) },
	{ "s25sl064a",  INFO(0x010216,      0,  64 * 1024, 128, 0) },
	{ "s25fl016k",  INFO(0xef4015,      0,  64 * 1024,  32, SECT_4K) },
	/*{ "s25fl064k",  INFO(0xef4017,      0,  64 * 1024, 128, SECT_4K) },*/

	/* SST -- large erase sizes are "overlays", "sectors" are 4K */
	{ "sst25vf040b", INFO(0xbf258d, 0,
				64 * 1024, 8, SECT_4K | SST_WRITE) },
	{ "sst25vf080b", INFO(0xbf258e, 0,
				64 * 1024, 16, SECT_4K | SST_WRITE) },
	{ "sst25vf016b", INFO(0xbf2541, 0,
				64 * 1024, 32, SECT_4K | SST_WRITE) },
	{ "sst25vf032b", INFO(0xbf254a, 0,
				64 * 1024, 64, SECT_4K | SST_WRITE) },
	{ "sst25vf064c", INFO(0xbf254b, 0,
				64 * 1024, 128, SECT_4K) },
	{ "sst25wf512",  INFO(0xbf2501, 0,
				64 * 1024, 1, SECT_4K | SST_WRITE) },
	{ "sst25wf010",  INFO(0xbf2502, 0,
				64 * 1024, 2, SECT_4K | SST_WRITE) },
	{ "sst25wf020",  INFO(0xbf2503, 0,
				64 * 1024, 4, SECT_4K | SST_WRITE) },
	{ "sst25wf040",  INFO(0xbf2504, 0,
				64 * 1024, 8, SECT_4K | SST_WRITE) },

	/* ST Microelectronics -- newer production may have feature updates */
	{ "m25p05",  INFO(0x202010,  0,  32 * 1024,   2, 0) },
	{ "m25p10",  INFO(0x202011,  0,  32 * 1024,   4, 0) },
	{ "m25p20",  INFO(0x202012,  0,  64 * 1024,   4, 0) },
	{ "m25p40",  INFO(0x202013,  0,  64 * 1024,   8, 0) },
	{ "m25p80",  INFO(0x202014,  0,  64 * 1024,  16, 0) },
	{ "m25p16",  INFO(0x202015,  0,  64 * 1024,  32, 0) },
	{ "m25p32",  INFO(0x202016,  0,  64 * 1024,  64, 0) },
	{ "m25p64",  INFO(0x202017,  0,  64 * 1024, 128, 0) },
	{ "m25p128", INFO(0x202018,  0, 256 * 1024,  64, 0) },
	{ "n25q032", INFO(0x20ba16,  0,  64 * 1024,  64, 0) },

	{ "m25p05-nonjedec",  INFO(0, 0,  32 * 1024,   2, 0) },
	{ "m25p10-nonjedec",  INFO(0, 0,  32 * 1024,   4, 0) },
	{ "m25p20-nonjedec",  INFO(0, 0,  64 * 1024,   4, 0) },
	{ "m25p40-nonjedec",  INFO(0, 0,  64 * 1024,   8, 0) },
	{ "m25p80-nonjedec",  INFO(0, 0,  64 * 1024,  16, 0) },
	{ "m25p16-nonjedec",  INFO(0, 0,  64 * 1024,  32, 0) },
	{ "m25p32-nonjedec",  INFO(0, 0,  64 * 1024,  64, 0) },
	{ "m25p64-nonjedec",  INFO(0, 0,  64 * 1024, 128, 0) },
	{ "m25p128-nonjedec", INFO(0, 0, 256 * 1024,  64, 0) },

	{ "m45pe10", INFO(0x204011,  0, 64 * 1024,    2, 0) },
	{ "m45pe80", INFO(0x204014,  0, 64 * 1024,   16, 0) },
	{ "m45pe16", INFO(0x204015,  0, 64 * 1024,   32, 0) },

	{ "m25pe20", INFO(0x208012,  0, 64 * 1024,  4,       0) },
	{ "m25pe80", INFO(0x208014,  0, 64 * 1024, 16,       0) },
	{ "m25pe16", INFO(0x208015,  0, 64 * 1024, 32, SECT_4K) },

	{ "m25px32",    INFO(0x207116,  0, 64 * 1024, 64, SECT_4K) },
	{ "m25px32-s0", INFO(0x207316,  0, 64 * 1024, 64, SECT_4K) },
	{ "m25px32-s1", INFO(0x206316,  0, 64 * 1024, 64, SECT_4K) },
	{ "m25px64",    INFO(0x207117,  0, 64 * 1024, 128, 0) },

	/* Winbond -- w25x "blocks" are 64K, "sectors" are 4KiB */
	{ "w25x10", INFO(0xef3011, 0, 64 * 1024,  2,  SECT_4K) },
	{ "w25x20", INFO(0xef3012, 0, 64 * 1024,  4,  SECT_4K) },
	{ "w25x40", INFO(0xef3013, 0, 64 * 1024,  8,  SECT_4K) },
	{ "w25x80", INFO(0xef3014, 0, 64 * 1024,  16, SECT_4K) },
	{ "w25x16", INFO(0xef3015, 0, 64 * 1024,  32, SECT_4K) },
	{ "w25x32", INFO(0xef3016, 0, 64 * 1024,  64, SECT_4K) },
	{ "w25q32", INFO(0xef4016, 0, 64 * 1024,  64, SECT_4K) },
	{ "w25q32dw", INFO(0xef6016, 0, 64 * 1024,  64, SECT_4K) },
	{ "w25x64", INFO(0xef3017, 0, 64 * 1024, 128, SECT_4K) },
	{ "w25q64", INFO(0xef4017, 0, 64 * 1024, 128, SECT_4K) },
	{ "w25q64fv", INFO(0xef6017, 0, 64 * 1024, 128, SECT_4K) },
	{ "w25q80", INFO(0xef5014, 0, 64 * 1024,  16, SECT_4K) },
	{ "w25q80bl", INFO(0xef4014, 0, 64 * 1024,  16, SECT_4K) },
	{ "w25q128", INFO(0xef4018, 0, 64 * 1024, 256, SECT_4K) },
	{ "w25q256", INFO(0xef4019, 0, 64 * 1024, 512, SECT_4K) },
	{ "w25q512jv", INFO(0xef7119, 0, 64 * 1024, 1024, SECT_4K) },

	/* Catalyst / On Semiconductor -- non-JEDEC */
	{ "cat25c11", CAT25_INFO(16, 8, 16, 1) },
	{ "cat25c03", CAT25_INFO(32, 8, 16, 2) },
	{ "cat25c09", CAT25_INFO(128, 8, 32, 2) },
	{ "cat25c17", CAT25_INFO(256, 8, 32, 2) },
	{ "cat25128", CAT25_INFO(2048, 8, 64, 2) },
	{ },
};
MODULE_DEVICE_TABLE(spi, nvt_spi_flash_ids);
#if SPI_PREFORMANCE_TEST
static void spi_changeCLK(unsigned int uiClock)
{
#if 0
	/* 458/658/656 IC */
	unsigned int ulRegValue;
	unsigned int ratio = (uiClock/12000000) * (1<<17);

	/* STBC Keypass */
	HAL_WRITE_UINT32((ClkSet + CLK_SET_OFFSET), SPI_CLOCK_SOURCE_PAGEB);

	HAL_WRITE_UINT32(mpllClk + (NVT_MPLL_ADDR1<<2), (ratio>>0) & 0xff);
	HAL_WRITE_UINT32(mpllClk + (NVT_MPLL_ADDR2<<2), (ratio>>8) & 0xff);
	HAL_WRITE_UINT32(mpllClk + (NVT_MPLL_ADDR3<<2), (ratio>>16) & 0xff);

	nvt_soc_stbc_unlock_reg();

	HAL_READ_UINT32(StbcBase + SPI_CLOCK_SOURCE, ulRegValue);
	ulRegValue = (ulRegValue & ~(SPI_CLOCK_SOURCE_PAGEB)) |
					SPI_CLOCK_SOURCE_OSC16X;
	HAL_WRITE_UINT32(StbcBase + SPI_CLOCK_SOURCE, ulRegValue);
#endif

	/* for 668/670/563/171/172 and after IC */

	unsigned int ulRegValue;

	HAL_WRITE_UINT32((ClkSet + CLK_SET_OFFSET), SPI_CLOCK_SOURCE_PAGE0);

	HAL_WRITE_UINT32(mpllClk + (NVT_MPLL_INDEX<<2), uiClock);

	nvt_soc_stbc_unlock_reg();

	HAL_READ_UINT32(StbcBase + SPI_CLOCK_SOURCE, ulRegValue);
	ulRegValue = (ulRegValue & ~(SPI_CLOCK_SOURCE_PAGEB)) |
					SPI_CLOCK_SOURCE_OSC16X;
	HAL_WRITE_UINT32(StbcBase + SPI_CLOCK_SOURCE, ulRegValue);

}
#endif
static const struct spi_device_id *jedec_probe(struct nvt_spi_master *spi)
{
	int			tmp;
	u8			code = OPCODE_RDID;
	u32			jedec;
	u16                     ext_jedec;
	struct flash_info	*info;
	u32 reg_val, ontime;
	u32 ret_val[2] = {0};
	u8 *ptr = NULL;

	/* JEDEC also defines an optional "extended device information"
	 * string for after vendor-specific data, after the three bytes
	 * we use here.  Supporting some chips might require using it.
	 */
	reg_val = (NVT_SPI_TRANS_FROM_DEV | NVT_SPI_LOCK_ON |
					NVT_SPI_DEFAULT_BYTE);
	nvt_spi_wr(spi, NVT_SPI_CTRL_REG, reg_val);

	reg_val = code;
	nvt_spi_wr(spi, NVT_SPI_CMD_ADDR_REG, reg_val);

	reg_val = (NVT_SPI_DATA_EN | NVT_SPI_DATA_CNT(3));
	nvt_spi_wr(spi, NVT_SPI_ACC_CTRL_REG, reg_val);

	init_completion(&spi->done);

	reg_val |= NVT_SPI_ACC_START;
	nvt_spi_wr(spi, NVT_SPI_ACC_CTRL_REG, reg_val);

	ontime = wait_for_completion_timeout(&spi->done,
				msecs_to_jiffies(NVT_SPI_WAIT_TIMEOUT));

	if (!ontime) {
		pr_err("read JEDEC id fail !\n");
		return ERR_PTR(-ENODEV);
	}

	ret_val[0] = nvt_spi_rd(spi, NVT_SPI_ACC_DWORD0_REG);
	ret_val[1] = nvt_spi_rd(spi, NVT_SPI_ACC_DWORD1_REG);
	ptr = (u8 *)ret_val;

	jedec = ptr[0];
	jedec <<= 8;
	jedec |= ptr[1];
	jedec <<= 8;
	jedec |= ptr[2];

	ext_jedec = ptr[3] << 8 | ptr[4];

	for (tmp = 0; tmp < ARRAY_SIZE(nvt_spi_flash_ids) - 1; tmp++) {
		info = (void *)nvt_spi_flash_ids[tmp].driver_data;
		if (info->jedec_id == jedec) {
			if (info->ext_id != 0 && info->ext_id != ext_jedec)
				continue;
			return &nvt_spi_flash_ids[tmp];
		}
	}
	pr_err("unrecognized JEDEC id %06x\n", jedec);
	return ERR_PTR(-ENODEV);
}

static irqreturn_t nvt_spi_interrupt(int irq, void *dev_id)
{
	struct nvt_spi_master *ns = (struct nvt_spi_master *)dev_id;
	u32 reg_val;

	if (ns != int_ns)
		return IRQ_NONE;

	do {
		reg_val = nvt_spi_rd(ns, NVT_SPI_INT_CTRL_REG);
	} while (!(reg_val & NVT_SPI_INT_OLD_DONE_STS));

	/* reg_val |= (NVT_SPI_INT_OLD_DONE_STS | NVT_SPI_INT_NEW_DONE_STS); */
	nvt_spi_wr(ns, NVT_SPI_INT_CTRL_REG, reg_val);

	complete(&ns->done);

	return IRQ_HANDLED;
}

#ifdef CONFIG_MTD_NVT_SPI_USE_STBC_ALIAS_NAME
static char *alias_stbc_name = "spi_stbc";
#endif

/*
 * board specific setup should have ensured the SPI clock used here
 * matches what the READ command supports, at least until this driver
 * understands FAST_READ (for clocks over 25 MHz).
 */
static int nvt_spi_flash_probe(struct platform_device *pdev)
{
	struct flash_platform_data	*data = NULL;
	struct nvt_spi_flash		*flash;
	struct flash_info		*info;
	unsigned			i;
	struct resource *res;
	struct nvt_spi_master *ns = NULL;
	int err;
	const struct spi_device_id *jid;

	flash = kzalloc(sizeof(*flash), GFP_KERNEL);
	if (!flash)
		return -ENOMEM;

	ns = &(flash->spi);

	platform_set_drvdata(pdev, flash);


	init_completion(&ns->done);

	res = platform_get_resource(pdev, IORESOURCE_MEM, 0);
	if (!res) {
		dev_err(&pdev->dev, "could not get memory resource\n");
		err = -ENODEV;
		goto out;
	}

	ns->regs = devm_ioremap_resource(&pdev->dev, res);
	if (!ns->regs) {
		dev_err(&pdev->dev, "could not request/map memory region\n");
		err = -ENODEV;
		goto out;
	}

	/* config interrupt bits */
	nvt_spi_wr(ns, NVT_SPI_INT_CTRL_REG,
			(NVT_SPI_INT_OLD_DONE_STS | NVT_SPI_INT_NEW_DONE_STS));
	nvt_spi_wr(ns, NVT_SPI_INT_CTRL_REG,
			(NVT_SPI_INT_NEW_DONE_EN | NVT_SPI_INT_OLD_DONE_EN));

	ns->irq = irq_of_parse_and_map(pdev->dev.of_node, 0);
	if (ns->irq <= 0) {
		dev_err(&pdev->dev, "could not get IRQ: %d\n", ns->irq);
		err = ns->irq ? ns->irq : -ENODEV;
		goto out;
	}

	int_ns = ns;

	err = request_irq(ns->irq, nvt_spi_interrupt, IRQF_SHARED,
			dev_name(&pdev->dev), ns);
	if (err) {
		dev_err(&pdev->dev, "could not request IRQ: %d\n", err);
		goto out_free_irq;
	}


	/* Platform data helps sort out which chip type we have, as
	 * well as how this board partitions it.  If we don't have
	 * a chip ID, try the JEDEC id commands; they'll work for most
	 * newer chips, even if we don't recognize the particular chip.
	 */
	info = (void *)&(flash->info);


	nvt_lock_spi(flash);
	jid = jedec_probe(ns);
	nvt_unlock_spi(flash);
	if (IS_ERR(jid)) {
		err =  PTR_ERR(jid);
		goto out_free_irq;
	} else {
		/*
		 * JEDEC knows better, so overwrite platform ID. We
		 * can't trust partitions any longer, but we'll let
		 * mtd apply them anyway, since some partitions may be
		 * marked read-only, and we don't want to lose that
		 * information, even if it's not 100% accurate.
		 */
		pr_err("found %s, expected %s\n",
			 jid->name, jid->name);
		info = (void *)jid->driver_data;
	}

	flash->command = kmalloc(MAX_CMD_SIZE + (flash->fast_read ? 1 : 0),
					GFP_KERNEL);
	if (!flash->command) {
		kfree(flash);
		err = -ENOMEM;
		goto out_free_irq;
	}

	mutex_init(&flash->lock);

	/*
	 * Atmel, SST and Intel/Numonyx serial flash tend to power
	 * up with the software protection bits set
	 */
	if (JEDEC_MFR(info->jedec_id) == CFI_MFR_ATMEL ||
	    JEDEC_MFR(info->jedec_id) == CFI_MFR_INTEL ||
	    JEDEC_MFR(info->jedec_id) == CFI_MFR_SST) {
		nvt_lock_spi(flash);
		write_enable(flash);
		write_sr(flash, 0);
		nvt_unlock_spi(flash);
	}

	if (data && data->name)
		flash->mtd.name = data->name;
	else
		flash->mtd.name = jid->name;

#ifdef CONFIG_MTD_NVT_SPI_USE_STBC_ALIAS_NAME
	/*
	 * We replace mtd.name to STBC alias name.
	 */
	flash->mtd.name = alias_stbc_name;
#endif

	flash->mtd.type = MTD_NORFLASH;
	flash->mtd.writesize = 1;
	flash->mtd.flags = MTD_CAP_NORFLASH;
	flash->mtd.size = info->sector_size * info->n_sectors;
	flash->mtd._erase = nvt_spi_flash_erase;
	flash->mtd._read = nvt_spi_flash_read;

	/* flash protection support for STmicro chips */
	if (JEDEC_MFR(info->jedec_id) == CFI_MFR_ST) {
		flash->mtd._lock = nvt_spi_flash_lock;
		flash->mtd._unlock = nvt_spi_flash_unlock;
	}

	/* sst flash chips use AAI word program */
	if (info->flags & SST_WRITE)
		flash->mtd._write = sst_write;
	else
		flash->mtd._write = nvt_spi_flash_write;

	/* prefer "small sector" erase if possible */
	if (info->flags & SECT_4K) {
		flash->erase_opcode = OPCODE_BE_4K;
		flash->mtd.erasesize = 4096;
	} else {
		flash->erase_opcode = OPCODE_SE;
		flash->mtd.erasesize = info->sector_size;
	}

	if (info->flags & NVT_SPI_FLASH_NO_ERASE)
		flash->mtd.flags |= MTD_NO_ERASE;

	/* ppdata.of_node = spi->dev.of_node; */
	/* flash->mtd.dev.parent = &spi->dev; */
	flash->page_size = info->page_size;
	flash->mtd.writebufsize = flash->page_size;

	flash->fast_read = true;

#if SPI_PREFORMANCE_TEST
	/* set CLK */
	spi_initRelatedHW();

	spi_changeCLK(20);
	pr_debug("Change SPI clock to 48MHZ\r\n");
#endif

	if (info->addr_width)
		flash->addr_width = info->addr_width;
	else {
		/* enable 4-byte addressing if the device exceeds 16MiB */
		if (flash->mtd.size > 0x1000000) {
			flash->addr_width = 4;
			nvt_lock_spi(flash);
			set_4byte(flash, info->jedec_id, 1);
			nvt_unlock_spi(flash);
		} else
			flash->addr_width = 3;
	}

	pr_debug("mtd .name = %s, .size = 0x%llx (%lldMiB)",
		flash->mtd.name,
		(long long)flash->mtd.size, (long long)(flash->mtd.size >> 20));
	pr_debug(".erasesize = 0x%.8x (%uKiB) .numeraseregions = %d\n",
		flash->mtd.erasesize, flash->mtd.erasesize / 1024,
		flash->mtd.numeraseregions);

	if (flash->mtd.numeraseregions) {
		for (i = 0; i < flash->mtd.numeraseregions; i++) {
			pr_debug("mtd.eraseregions[%d] = {.offset = 0x%llx,",
			i, (long long)flash->mtd.eraseregions[i].offset);
			pr_debug(".erasesize = 0x%.8x (%uKiB), .numblocks = %d }\n",
				flash->mtd.eraseregions[i].erasesize,
				flash->mtd.eraseregions[i].erasesize / 1024,
				flash->mtd.eraseregions[i].numblocks);
		}
	}
	/* partitions should match sector boundaries; and it may be good to
	 * use readonly partitions for writeprotected sectors (BP2..BP0).
	 */

	return mtd_device_parse_register(&flash->mtd, NULL, NULL,
			data ? data->parts : NULL,
			data ? data->nr_parts : 0);

out_free_irq:
	free_irq(ns->irq, ns);

out:
	if (ns != NULL)
		kfree(ns);
	return err;
}


static int nvt_spi_flash_remove(struct platform_device *pdev)
{
	struct nvt_spi_flash	*flash = platform_get_drvdata(pdev);
	struct nvt_spi_master *ns = &(flash->spi);
	int		status;

	free_irq(ns->irq, ns);

	/* Clean up MTD stuff. */
	status = mtd_device_unregister(&flash->mtd);
	if (status == 0) {
		kfree(flash->command);
		kfree(flash);
	}
	return 0;
}

static const struct of_device_id nt72668_spi_match[] = {
	{ .compatible = "nvt,NT72668-spi", },
	{}
};
MODULE_DEVICE_TABLE(of, nt72668_spi_match);

static struct platform_driver nt72668_spi_driver = {
	.driver		= {
		.name		= "spi-nt72668",
		.owner		= THIS_MODULE,
		.of_match_table	= nt72668_spi_match,
	},
	.probe		= nvt_spi_flash_probe,
	.remove		= nvt_spi_flash_remove,
};
module_platform_driver(nt72668_spi_driver);


MODULE_LICENSE("GPL");
MODULE_AUTHOR("Novatek");
MODULE_DESCRIPTION("MTD SPI driver for Nor flash chips");
