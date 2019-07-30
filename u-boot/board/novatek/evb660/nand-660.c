/*
 *  board/novatek/evb660/nand-660.c
 *
 *  Author:	Howard Chang
 *  Created:	Jan 26, 2015
 *  Copyright:	Novatek Inc.
 *
 */
#include <common.h>
#include <malloc.h>
#include <nand.h>
#include "nand_int.h"
#include <asm/addrspace.h>
#if defined(CONFIG_CMD_NAND)


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

static struct nand_ecclayout hw_largepage_ecclayout = {
	.eccbytes = 48,
	.eccpos = {
		 4, 5, 6,  7,  8,  9, 10, 11, 12, 13, 14, 15,
		20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31,
		36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47,
		52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63},
	.oobfree = { {0, 4},  {16, 4}, {32, 4}, {48, 4} }
};

static int use_dma = 1;

/* YMK Add for USE_FLASH_BBT */
static uint8_t bbt_pattern[] = {'B', 'b', 't', '0' };
static uint8_t mirror_pattern[] = {'1', 't', 'b', 'B' };

static struct nand_bbt_descr bbt_main_descr = {
	.options = NAND_BBT_CREATE | NAND_BBT_WRITE | NAND_BBT_2BIT
		| NAND_BBT_VERSION | NAND_BBT_PERCHIP|NAND_BBT_SCANEMPTY,
	.offs = 1,
	.len = 4,
	.veroffs = 5,
	.maxblocks = 4,
	.pattern = bbt_pattern
};

static struct nand_bbt_descr bbt_mirror_descr = {
	.options = NAND_BBT_CREATE | NAND_BBT_WRITE | NAND_BBT_2BIT
		| NAND_BBT_VERSION | NAND_BBT_PERCHIP | NAND_BBT_SCANEMPTY,
	.offs = 1,
	.len = 4,
	.veroffs = 5,
	.maxblocks = 4,
	.pattern = mirror_pattern
};

static void drv_nand_hwcontrol(struct mtd_info *mtd, int cmd, unsigned int ctrl)
{
	//Howard debug
	//printf("%s\n",__func__);
	//return;
}


static void nvt_scan_nand_type(struct drv_nand_dev_info *info, int32_t* id)
{
	int maf_id;
	int dev_id;

	maf_id = *id & 0xFF;
	dev_id = (*id >> 8) & 0xFF;

	printf(" the maf_id is 0x%x, the dev_id is 0x%x\n", maf_id, dev_id);

	switch(dev_id) {
		case NAND_256M_2K:
			info->flash_info->page_per_block	= 64;
			info->flash_info->page_size		= 2048;
			info->flash_info->oob_size		= 64;
			info->flash_info->subpage_size		= 512;
			info->flash_info->flash_width		= 8;
			info->flash_info->nfc_width		= 8;
			info->flash_info->num_blocks		= 2048;
			info->flash_info->chip_id		= *id;
			info->flash_info->phy_page_ratio		= 4;
			info->flash_info->device_size		= 256;
			info->flash_info->chip_sel		= 0;
			info->flash_info->block_size		= 0x20000;
			break;
		default:
			printf("Cannot find the parameter of dev_id 0x%x\n", dev_id);

	}
}

static int drv_nand_readpage(struct drv_nand_dev_info *info,
				int column, int page_addr)
{
	return nand_cmd_read_operation(info, info->data_buff,
			page_addr * info->flash_info->page_size, 1);
}

static int drv_nand_write_page(struct drv_nand_dev_info *info,
				int column, int page_addr)
{
	return nand_cmd_write_operation_single(info, info->data_buff,
				page_addr * info->flash_info->page_size, 1);
}

static int drv_nand_dev_ready(struct mtd_info *mtd)
{
	struct nand_chip *this = mtd->priv;
	struct drv_nand_dev_info *info = this->priv;
	return nand_host_check_ready(info);
}


static int drv_nand_read_page_ecc(struct mtd_info *mtd, struct nand_chip *chip,
				uint8_t *buf, int page)

{
	struct nand_chip *this = mtd->priv;
	struct drv_nand_dev_info *info = this->priv;

	memcpy(buf, info->data_buff, info->flash_info->page_size);

//	if (info->nand_chip.oob_poi && oob_required)
	if (info->nand_chip.oob_poi)
		memcpy(info->nand_chip.oob_poi, info->data_buff +
			info->flash_info->page_size,
			info->flash_info->oob_size);
	if (info->retcode == ERR_ECC_FAIL) {
		mtd->ecc_stats.failed++;
		return -1;
	} else if (info->retcode == ERR_ECC_UNCLEAN) {
		mtd->ecc_stats.corrected += info->ecc_corrected;
		return -1;
	} else {
		return 0;
	}

}

static int drv_nand_write_page_ecc(struct mtd_info *mtd, struct nand_chip *chip,
				  const uint8_t *buf)
{
	/* this function will write page data and oob into nand */
	struct nand_chip *this = mtd->priv;
	struct drv_nand_dev_info *info = this->priv;

	memcpy(info->data_buff, buf, info->flash_info->page_size);

	// if (oob_required)
		// memcpy(info->data_buff + info->flash_info->page_size,
			// chip->oob_poi, info->flash_info->oob_size);
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
	memcpy(buf, info->data_buff + info->buf_start, real_len);
	info->buf_start += real_len;
}

static void drv_nand_write_buf(struct mtd_info *mtd,
	const uint8_t *buf, int len)
{
	struct nand_chip *this = mtd->priv;
	struct drv_nand_dev_info *info = this->priv;

	int real_len = min_t(size_t, len, info->buf_count - info->buf_start);
	memcpy(info->data_buff + info->buf_start, buf, real_len);
	info->buf_start += real_len;
}

static void drv_nand_cmdfunc(struct mtd_info *mtd, unsigned command,
		int column, int page_addr)
{
	struct nand_chip *this = mtd->priv;
	struct drv_nand_dev_info *info = this->priv;

	int ret;
	uint8_t *ptr;

	info->use_dma = (use_dma) ? 1 : 0;
	info->use_ecc = 0;
	info->data_size = 0;
	info->state = STATE_READY;
/*
	pr_info("%s cmd[%02x]\n", __func__, command);
*/
	switch (command) {
	case NAND_CMD_READOOB:
		info->buf_count = mtd->writesize + mtd->oobsize;
		info->buf_start = mtd->writesize + column;
/*
		memset(info->data_buff, 0xFF, info->buf_count);
*/
		ptr = (uint8_t *)info->data_buff;
		ptr = info->data_buff + info->buf_start;
		if (info->buf_start != info->flash_info->page_size) {
			// dev_err(&info->pdev->dev,
			// "info->buf_start = %d, != 0\n", info->buf_start);
		}
		nand_cmd_read_page_spare_data(info, ptr,
				info->flash_info->page_size * page_addr);
/*
		drv_nand_readpage(info, column, page_addr);
		pr_info("%02x %02x %02x %02x\n",
			ptr[0], ptr[1], ptr[2], ptr[3]);

*/

		/* We only are OOB, so if the data has error, does not matter */
		if (info->retcode == ERR_ECC_FAIL)
			info->retcode = ERR_NONE;
	break;

	case NAND_CMD_READ0:
	if (((unsigned long)(info->data_buff)) % MIPS_CACHE_LINE_SIZE)
		printf("NAND_CMD_READ0 : is not Cache_Line_Size alignment!\n");

	info->use_ecc = 1;
	info->retcode = ERR_NONE;
	info->buf_start = column;
	info->buf_count = mtd->writesize + mtd->oobsize;
/*
	memset(info->data_buff, 0xFF, info->buf_count);
*/
	ret = drv_nand_readpage(info, column, page_addr);
	if (ret == E_CTX)
		info->retcode = ERR_ECC_UNCLEAN;
	else if (ret < 0)
		info->retcode = ERR_DBERR;
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
/*
		pr_info("col[%d], cnt[%d]\n", info->buf_start, info->buf_count);
*/
	break;

	case NAND_CMD_PAGEPROG:
	if (((unsigned long)(info->data_buff)) % MIPS_CACHE_LINE_SIZE)
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
		info->buf_count = 8;
		nvt_nand_read_id(info, (uint32_t *)info->data_buff);
	break;
	case NAND_CMD_STATUS:
		info->buf_start = 0;
		info->buf_count = 1;
		nand_cmd_read_status(info);
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

}

static int drv_nand_reset(struct drv_nand_dev_info *info)
{
	uint32_t reg;
	int32_t chip_sel = 0;

	/* Config NAND module clock */
	reg = INW(0xC0020000 + 0x74);
	reg |= (0x1);
	OUTW(0xC0020000 + 0x74, reg);

	reg = INW(0xC0020000 + 0x74);
	reg |= (0x1);
	OUTW(0xC0020000 + 0x74, reg);

	NAND_SETREG(info, NAND_TIME0_REG_OFS, 0x06002222);
	NAND_SETREG(info, NAND_TIME1_REG_OFS, 0x7f0f);

	/* Need use PinCtrl framework */
	reg = INW(0xC0010000 + 0x4);
	OUTW(0xC0010000 + 0x4, reg | 0x1000);

	reg = INW(0xC0010000 + 0xA0);
	reg &= ~(0x0000FFFF);

	OUTW(0xC0010000 + 0xA0, reg);
	/* reset NAND Config NAND module configuration */
	return nand_cmd_reset(info, chip_sel);
}


int nvt_nand_read_id(struct drv_nand_dev_info *info, uint32_t *id)
{
	uint8_t  card_id[8];

	if (nand_cmd_read_id(card_id, info) != 0) {
		printf("NAND cmd timeout\r\n");
		return -1;
	} else {
		printf("id =  0x%02x 0x%02x 0x%02x 0x%02x 0x%02x\n",
			card_id[0], card_id[1], card_id[2], card_id[3],
			card_id[4]);

		*id = card_id[0] | (card_id[1] << 8);
		return 0;
	}

}
/*
static void emu_dspDumpData(uint32_t startAddress, uint32_t len)
{
    u32 index;
    u32 reg;
    printf("\r\n");
    printf("[0x%08x] ", startAddress);
    for(index = 0; index < len; index+=4)
    {
        if((index % 16) == 0 && index > 0)
        {
            printf("\r\n");
            printf("[0x%08x] ", startAddress + index);
        }
        reg = INW((startAddress + index));
        printf("0x%08x ", reg);
    }
    printf("\n");
}
*/
int nvt_nand_board_nand_init(struct nand_chip *nand)
{
	int32_t id;
	struct drv_nand_dev_info *info;
	struct nvt_nand_flash *flash_info;

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

	nand->priv = info;
	info->mmio_base = CONFIG_SYS_NAND_BASE;
	drv_nand_reset(info);


	nvt_nand_read_id(info, &id);
	printf("read chip_id 0x%08x\n",id);

	nvt_scan_nand_type(info, &id);

	info->data_buff = malloc(info->flash_info->page_size+info->flash_info->oob_size);

	if(info->data_buff == NULL) {
		printf("allocate nand buffer fail !\n");
		return 1;
	}
/*
	//info->data_buff = CKSEG0ADDR(info->data_buff);
	int test;
	for(test = 0; test < 0x840; test++)
	{
		//info->data_buff[test] = 0x5a + test;
		info->data_buff[test] = 0x00;
	}

printf("###########erase start##################\n");
	nand_cmd_erase_block(info, 0);
printf("###########erase end##################\n");
	// info->use_dma = 1;
	// info->use_ecc = 0;
	// info->data_size = 0;
	// info->state = STATE_READY;
	// drv_nand_write_page(info, 0, 0);
// printf("###########write end##################\n");
//while(nand_test);
	// for(test = 0; test < 0x840; test++)
	// {
		// info->data_buff[test] = 0xff;
	// }

	info->use_ecc = 1;
	info->retcode = ERR_NONE;
	info->buf_start = 0;
	info->buf_count = 0x840;
	drv_nand_readpage(info, 0, 0);

printf("###########read page end##################\n");
	emu_dspDumpData((uint32_t)info->data_buff, 256);
*/
	if (info->flash_info->page_size == 512)
		nand->ecc.layout = &hw_smallpage_ecclayout;
	else
		nand->ecc.layout = &hw_largepage_ecclayout;
	nand->ecc.size = 0x200;
	nand->ecc.bytes = 0xC;
	nand->cmd_ctrl = drv_nand_hwcontrol;
	nand->dev_ready = drv_nand_dev_ready;
	nand->ecc.mode = NAND_ECC_HW;
	nand->ecc.read_page = drv_nand_read_page_ecc;
	nand->ecc.write_page = drv_nand_write_page_ecc;
	nand->options = NAND_NO_READRDY | NAND_NO_SUBPAGE_WRITE;
//#ifdef CONFIG_SYS_NAND_USE_FLASH_BBT
	//nand->bbt_options = NAND_USE_FLASH_BBT;
	nand->bbt_td = &bbt_main_descr;
	nand->bbt_md = &bbt_mirror_descr;
//#endif
	nand->waitfunc = drv_nand_waitfunc;
	nand->read_byte = drv_nand_read_byte;
	nand->read_word = drv_nand_read_word;
	nand->read_buf = drv_nand_read_buf;
	nand->write_buf = drv_nand_write_buf;
	nand->cmdfunc = drv_nand_cmdfunc;
	nand->chip_delay = 0;

	return 0;
}


#endif

