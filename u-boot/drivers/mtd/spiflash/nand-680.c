/*
 *  board/novatek/evb660/nand-660.c
 *
 *  Author:	Howard Chang
 *  Created:	Jan 26, 2015
 *  Copyright:	Novatek Inc.
 *
 */
#include "config.h"
#include <common.h>
#include <malloc.h>
#include <nand.h>
#include "../nvt_flash_spi/nand_int.h"
#include <asm/addrspace.h>
#include <asm/arch/nvt_ipc_common.h>

#define loc_sem() nvt_ipc_sem_lock(NVTIPC_SEMID_NAND)
#define unl_sem() nvt_ipc_sem_unlock(NVTIPC_SEMID_NAND)

#ifdef CONFIG_CMD_NAND
#define CONFIG_BUF_PRE_FLUSH
/* error code and state */
enum {
	ERR_NONE	= 0,
	ERR_DMABUSERR	= -1,
	ERR_SENDCMD	= -2,
	ERR_DBERR	= -3,
	ERR_BBERR	= -4,
	ERR_ECC_FAIL	= -5,
	ERR_ECC_UNCLEAN = -6,
};

enum {
	STATE_READY = 0,
	STATE_CMD_HANDLE,
	STATE_DMA_READING,
	STATE_DMA_WRITING,
	STATE_DMA_DONE,
	STATE_PIO_READING,
	STATE_PIO_WRITING,
};

static struct nand_ecclayout hw_smallpage_ecclayout = {
	.eccbytes = 12,
	.eccpos = {0, 4, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15 },
	.oobfree = { {1, 3}, {5, 1} }
};
#if (CONFIG_NVT_NAND_TYPE == CONFIG_NAND_TYPE_ONFI)

static struct nand_ecclayout hw_largepage_ecclayout = {
	.eccbytes = 48,
	.eccpos = {
		 4, 5, 6,  7,  8,  9, 10, 11, 12, 13, 14, 15,
		20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31,
		36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47,
		52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63},
	.oobfree = { {0, 4},  {16, 4}, {32, 4}, {48, 4} }
#elif(CONFIG_NVT_NAND_TYPE == CONFIG_NAND_TYPE_SPINAND)
static struct nand_ecclayout spinand_oob_64 = {
	.eccbytes = 32,
	.eccpos = {
		8, 9, 10, 11, 12, 13, 14, 15,
		24, 25, 26, 27, 28, 29, 30, 31,
		40, 41, 42, 43, 44, 45, 46, 47,
		56, 57, 58, 59, 60, 61, 62, 63},
	.oobavail = 12,
	.oobfree = {
		{.offset = 16,
			.length = 4},
		{.offset = 32,
			.length = 4},
		{.offset = 48,
			.length = 4},
	}
#endif
};

static void drv_nand_hwcontrol(struct mtd_info *mtd, int cmd, unsigned int ctrl)
{

}


static void nvt_scan_nand_type(struct drv_nand_dev_info *info, int32_t* id)
{
	int maf_id, dev_id;
	uint32_t reg;

	maf_id = *id & 0xFF;
	dev_id = (*id >> 8) & 0xFF;

	debug("the maf_id is 0x%x, the dev_id is 0x%x\n", maf_id, dev_id);

	info->flash_info->page_per_block = 64;
	info->flash_info->page_size = 2048;
	info->flash_info->oob_size = 64;
	info->flash_info->subpage_size = 512;
	info->flash_info->flash_width = 8;
	info->flash_info->nfc_width = 8;
	info->flash_info->num_blocks = 2048;
	info->flash_info->chip_id = *id;
	info->flash_info->phy_page_ratio = 4;
	info->flash_info->device_size = 128;
	info->flash_info->chip_sel = 0;
	info->flash_info->block_size = 0x20000;

	info->flash_info->module_config = NAND_PAGE2K | NAND_2COL_ADDR;
	if (info->use_ecc == NANDCTRL_SPIFLASH_USE_INTERNAL_RS_ECC)
		info->flash_info->module_config |= NAND_PRI_ECC_RS;
	else
		info->flash_info->module_config |= NAND_PRI_ECC_SPI_ON_DIE;

	nand_hostSetupPageSize(0, NAND_PAGE_SIZE_2048);

	reg = NAND_GETREG(info, NAND_MODULE0_REG_OFS);
	reg &= ~(0x7FFFF);

	NAND_SETREG(info, NAND_MODULE0_REG_OFS, reg | info->flash_info->module_config);
}

static int drv_nand_readpage(struct drv_nand_dev_info *info,
				int column, int page_addr)
{
	return nand_cmd_read_operation(info, (int8_t *)info->data_buff,
			page_addr * info->flash_info->page_size, 1);
}

static int drv_nand_write_page(struct drv_nand_dev_info *info,
				int column, int page_addr)
{
	if (column != info->flash_info->page_size)
		nand_cmd_write_spare_sram(info);

	return nand_cmd_write_operation_single(info, (int8_t *)info->data_buff,
				page_addr * info->flash_info->page_size, column);
}

static int drv_nand_dev_ready(struct mtd_info *mtd)
{
	struct nand_chip *this = mtd->priv;
	struct drv_nand_dev_info *info = this->priv;
	return nand_host_check_ready(info);
}


static int drv_nand_read_page_ecc(struct mtd_info *mtd, struct nand_chip *chip,
				uint8_t *buf, int oob_required, int page)

{
	struct nand_chip *this = mtd->priv;
	struct drv_nand_dev_info *info = this->priv;
	int eccsize = chip->ecc.size;
	int eccsteps = chip->ecc.steps;
	u8 status = 0, chip_id = info->flash_info->chip_id & 0xFF;
	int ret = 0;

	chip->read_buf(mtd, buf, eccsize * eccsteps);

	if (info->nand_chip.oob_poi)
		chip->read_buf(mtd, chip->oob_poi, mtd->oobsize);

	if (info->retcode == ERR_ECC_UNCLEAN) {
		mtd->ecc_stats.failed++;
	} else if (info->retcode == ECC_CORRECTED) {
		if (info->use_ecc == NANDCTRL_SPIFLASH_USE_INTERNAL_RS_ECC) {
				mtd->ecc_stats.corrected += \
					nand_cmd_read_ecc_corrected(info);

				ret = status;
		} else {
			if (chip_id == NAND_WINBOND)
				ret = 1;

			if (chip_id == NAND_MXIC) {
				ret = nand_cmd_read_flash_ecc_corrected(info);
				mtd->ecc_stats.corrected += ret;
			} else if (chip_id == NAND_TOSHIBA) {
				status = nand_cmd_read_status(info, \
					NAND_SPI_STS_FEATURE_4);

				mtd->ecc_stats.corrected += status;
				ret = status;
			} else
				mtd->ecc_stats.corrected++;
		}
	}

	return ret;

}

static int drv_nand_write_page_ecc(struct mtd_info *mtd, struct nand_chip *chip,
				  const uint8_t *buf, int oob_required)
{
	/* this function will write page data and oob into nand */
	int eccsize = chip->ecc.size;
	int eccsteps = chip->ecc.steps;

	chip->write_buf(mtd, buf, eccsize * eccsteps);

	return 0;
}

static int drv_nand_waitfunc(struct mtd_info *mtd, struct nand_chip *this)
{
	struct nand_chip *nand_chip = mtd->priv;
	struct drv_nand_dev_info *info = nand_chip->priv;

	while (nand_host_check_ready(info) != E_OK)
		;
	return 0;
}

static uint8_t drv_nand_read_byte(struct mtd_info *mtd)
{
	struct nand_chip *this = mtd->priv;
	struct drv_nand_dev_info *info = this->priv;

	char retval = 0xFF;

	if (info->buf_start < info->buf_count)
		retval = info->data_buff[info->buf_start++];

	return retval;
}

static u16 drv_nand_read_word(struct mtd_info *mtd)
{
	struct nand_chip *this = mtd->priv;
	struct drv_nand_dev_info *info = this->priv;

	u16 retval = 0xFFFF;

	if (!(info->buf_start & 0x01) && info->buf_start < info->buf_count) {
		retval = *((u16 *)(info->data_buff+info->buf_start));
		info->buf_start += 2;
	}
	return retval;
}

static void drv_nand_read_buf(struct mtd_info *mtd, uint8_t *buf, int len)
{
	struct nand_chip *this = mtd->priv;
	struct drv_nand_dev_info *info = this->priv;

	int real_len = min_t(size_t, len, info->buf_count - info->buf_start);
	loc_sem();
	memcpy(buf, info->data_buff + info->buf_start, real_len);
#ifdef CONFIG_BUF_PRE_FLUSH
	flush_dcache_range((unsigned long)buf, (unsigned long)((unsigned long)buf + real_len));
#endif
	info->buf_start += real_len;
	unl_sem();
}

static void drv_nand_write_buf(struct mtd_info *mtd,
	const uint8_t *buf, int len)
{
	struct nand_chip *this = mtd->priv;
	struct drv_nand_dev_info *info = this->priv;

	int real_len = min_t(size_t, len, info->buf_count - info->buf_start);
	loc_sem();
	memcpy(info->data_buff + info->buf_start, buf, real_len);
#ifdef CONFIG_BUF_PRE_FLUSH
	flush_dcache_range((unsigned long)info->data_buff, (unsigned long)((unsigned long)info->data_buff + real_len));
#endif
	info->buf_start += real_len;
	unl_sem();
}

static void drv_nand_cmdfunc(struct mtd_info *mtd, unsigned command,
		int column, int page_addr)
{
	struct nand_chip *this = mtd->priv;
	struct drv_nand_dev_info *info = this->priv;

	int ret;
	uint8_t *ptr;

	info->data_size = 0;
	info->state = STATE_READY;

	loc_sem();

	switch (command) {
	case NAND_CMD_READOOB:
		info->buf_count = mtd->writesize + mtd->oobsize;
		info->buf_start = mtd->writesize + column;

		ptr = (uint8_t *)info->data_buff;
		ptr = info->data_buff + info->buf_start;
		if (info->buf_start != info->flash_info->page_size) {
			dev_err(&info->pdev->dev,
			"info->buf_start = %d, != 0\n", info->buf_start);
		}

		nand_cmd_read_page_spare_data(info, (int8_t *)ptr,
				info->flash_info->page_size * page_addr);

		/* We only are OOB, so if the data has error, does not matter */
		if (info->retcode == ERR_ECC_FAIL)
			info->retcode = ERR_NONE;
	break;

	case NAND_CMD_READ0:
		if (((unsigned long)(info->data_buff)) % CACHE_LINE_SIZE)
			printf("NAND_CMD_READ0 : is not Cache_Line_Size alignment!\n");

		info->retcode = ERR_NONE;
		info->buf_start = column;
		info->buf_count = mtd->writesize + mtd->oobsize;

		ret = drv_nand_readpage(info, column, page_addr);
		if (ret == E_CTX)
			info->retcode = ERR_ECC_UNCLEAN;
		else if (ret < 0)
			info->retcode = ERR_DBERR;
		else if (ret == ECC_CORRECTED)
			info->retcode = ECC_CORRECTED;
		else
			info->retcode = ERR_NONE;
	break;

	case NAND_CMD_SEQIN:
		info->buf_start = column;
		info->buf_count = mtd->writesize + mtd->oobsize;
		memset(info->data_buff, 0xff, info->buf_count);

		/* save column/page_addr for next CMD_PAGEPROG */
		info->seqin_column = column;
		info->seqin_page_addr = page_addr;
	break;

	case NAND_CMD_PAGEPROG:
		if (((unsigned long)(info->data_buff)) % CACHE_LINE_SIZE)
			printf("not MIPS_Cache_Line_Size alignment!\n");

		drv_nand_write_page(info, info->seqin_column, info->seqin_page_addr);
	break;

	case NAND_CMD_ERASE1:
		nand_cmd_erase_block(info, page_addr);
	break;

	case NAND_CMD_ERASE2:
	break;

	case NAND_CMD_READID:
		info->buf_start = 0;
		info->buf_count = 4;
		if(info->flash_info->chip_id)
			memcpy((uint32_t *)info->data_buff, &info->flash_info->chip_id, 4);
		else
			nvt_nand_read_id(info, (uint32_t *)info->data_buff);
	break;
	case NAND_CMD_STATUS:
		info->buf_start = 0;
		info->buf_count = 1;
		nand_cmd_read_status(info, NAND_SPI_STS_FEATURE_2);
		if (!(info->data_buff[0] & 0x80))
			info->data_buff[0] = 0x80;
	break;

	case NAND_CMD_RESET:
	break;

	default:
		printf("non-supported command.\n");
	break;
	}

	if (info->retcode == ERR_DBERR) {
		printf("double bit error @ page %08x\n", page_addr);
		info->retcode = ERR_NONE;
	}

	unl_sem();
}

static int drv_nand_reset(struct drv_nand_dev_info *info)
{
	uint32_t reg;
#if (CONFIG_NVT_NAND_TYPE == CONFIG_NAND_TYPE_SPINAND)
	/* Config NAND module clock */
	reg = INW(0xF0020000 + 0x84);
	reg |= (0x1);
	OUTW(0xF0020000 + 0x84, reg);

	NAND_SETREG(info, NAND_TIME0_REG_OFS, 0x06002222);
	NAND_SETREG(info, NAND_TIME1_REG_OFS, 0x7f0f);

	/* Need use PinCtrl framework */
	reg = INW(0xF0010000 + 0x4);
	reg &= ~(0x00001000);
	reg |= 0x00002000;
	OUTW(0xF0010000 + 0x4, reg);

	reg = INW(0xF0010000 + 0xA0);
	reg &= ~(0x0000030F);
	OUTW(0xF0010000 + 0xA0, reg);
	/* reset NAND Config NAND module configuration */

	/* Config clock div */
	reg = INW(0xF0020000 + 0x40);
	reg &= ~(0x3F000000);
	reg |= (0x5 << 24);
	OUTW(0xF0020000 + 0x40, reg);

	/* Release SRAM */
	reg = INW(0xF0020000 + 0x104);
	reg &= ~(0x1);
	OUTW(0xF0020000 + 0x104, reg);

	/* Config driving */
	reg = INW(0xF0030000 + 0x40);
	reg &= ~(0x3F00FF);
	reg |= 0x150055;
	OUTW(0xF0030000 + 0x40, reg);

	nand_hostSetNandType(info, NANDCTRL_SPI_NAND_TYPE);

        nand_host_settiming2(info, 0x9F51);
#endif
	return nand_cmd_reset(info);

}


int nvt_nand_read_id(struct drv_nand_dev_info *info, uint32_t *id)
{
	uint8_t  card_id[8];

	if (nand_cmd_read_id(card_id, info) != 0) {
		printf("NAND cmd timeout\r\n");
		return -1;
	} else {
		printf("id =  0x%02x 0x%02x 0x%02x 0x%02x\n",
			card_id[0], card_id[1], card_id[2], card_id[3]);

		*id = card_id[0] | (card_id[1] << 8);
		return 0;
	}

}

int nvt_nand_board_nand_init(struct nand_chip *nand)
{
	int32_t id;
	struct drv_nand_dev_info *info;
	u32 status;

	info = malloc(sizeof(struct drv_nand_dev_info));
	if (!info) {
		printf("alloc drv_nand_dev_info failed!\n");
		return -ENOMEM;
	}

	info->flash_info = malloc(sizeof(struct nvt_nand_flash));
	if (!info) {
		printf("alloc nvt_nand_flash failed!\n");
		return -ENOMEM;
	}
	info->flash_info->spi_nand_status.bBlockUnlocked    = FALSE;
	info->flash_info->spi_nand_status.bQuadEnable       = FALSE;
	info->flash_info->spi_nand_status.bQuadProgram      = FALSE;
	info->flash_info->spi_nand_status.uiTimerRecord     = FALSE;

	nand->priv = info;
	info->mmio_base = (void *)(CONFIG_SYS_NAND_BASE);
	info->use_ecc = CONFIG_NVT_NAND_ECC;

	loc_sem();
	if (!spiflash_boot_detect(info)) {
		drv_nand_reset(info);
		nand_dll_reset();
	}

	/*Delay 1 ms for spinand characteristic*/
	mdelay(1);

	nvt_nand_read_id(info, (uint32_t *)&id);

	if (id != TOSHIBA_TC58CVG)
		info->flash_info->spi_nand_status.bQuadProgram = TRUE;

	nvt_scan_nand_type(info, &id);
	unl_sem();

	info->data_buff = malloc(info->flash_info->page_size+info->flash_info->oob_size + CACHE_LINE_SIZE);

	if (((unsigned long)(info->data_buff)) % CACHE_LINE_SIZE)
		info->data_buff = (uint8_t *)((((unsigned long)info->data_buff + CACHE_LINE_SIZE - 1)) & 0xFFFFFFC0);

	if(info->data_buff == NULL) {
		printf("allocate nand buffer fail !\n");
		return 1;
	}

	loc_sem();
	status = nand_cmd_read_status(info, NAND_SPI_STS_FEATURE_2);

	if (info->use_ecc == NANDCTRL_SPIFLASH_USE_INTERNAL_RS_ECC)
		status &= ~SPINAND_CONFIG_ECCEN;
	else
		status |= SPINAND_CONFIG_ECCEN;

	if (((id & 0xFF) == NAND_MXIC) || ((id & 0xFF) == SPI_NAND_GD))
		status |= SPINAND_CONFIG_QUADEN;

	nand_cmd_write_status(info, NAND_SPI_STS_FEATURE_2, status);
	unl_sem();

	info->use_dma = 1;
	info->data_size = 0;
	info->state = STATE_READY;

	if (info->flash_info->page_size == 512)
		nand->ecc.layout = &hw_smallpage_ecclayout;
	else
		nand->ecc.layout = &spinand_oob_64;
	nand->ecc.size = 0x200;
	nand->ecc.bytes = 0x8;
	nand->ecc.steps = 0x4;
	nand->ecc.total	= nand->ecc.steps * nand->ecc.bytes;
	nand->ecc.strength = 1;

	nand->cmd_ctrl = drv_nand_hwcontrol;
	nand->dev_ready = drv_nand_dev_ready;
	nand->ecc.mode = NAND_ECC_HW;
	nand->ecc.read_page = drv_nand_read_page_ecc;
	nand->ecc.write_page = drv_nand_write_page_ecc;
	nand->options = NAND_NO_READRDY | NAND_NO_SUBPAGE_WRITE;
	nand->waitfunc = drv_nand_waitfunc;
	nand->read_byte = drv_nand_read_byte;
	nand->read_word = drv_nand_read_word;
	nand->read_buf = drv_nand_read_buf;
	nand->write_buf = drv_nand_write_buf;
	nand->cmdfunc = drv_nand_cmdfunc;
	nand->chip_delay = 0;

	if (info->use_ecc == NANDCTRL_SPIFLASH_USE_INTERNAL_RS_ECC)
			nand->ecc.strength = 4;
	else {
		if ((id & 0xFF) == NAND_TOSHIBA)
			nand->ecc.strength = 8;
		else if ((id & 0xFF) == NAND_MXIC)
			nand->ecc.strength = 4;
	}


	return 0;
}

#endif

