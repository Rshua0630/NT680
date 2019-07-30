#include "include/nt72_nand_config.h"
#include "include/nt72_nand.h"
#include "include/nt72_nand_regs.h"
#include "include/nt72_nand_specs.h"
#include "include/nt72_nand_ecc_layouts.h"
#include "include/nt72_nand_nfc.h"
#include "include/nt72_nand_ir.h"
#include "include/nt72_nand_utils.h"
#include "include/nt72_nand_parts.h"

#ifdef USE_BBT
static uint8_t bbt_pattern[] = { 'B', 't', '0' };
static uint8_t mirror_pattern[] = { '1', 't', 'B' };
static struct nand_bbt_descr bbt_main_descr = {
	.options	= NAND_BBT_LASTBLOCK | NAND_BBT_CREATE | NAND_BBT_WRITE
	| NAND_BBT_2BIT | NAND_BBT_VERSION | NAND_BBT_PERCHIP,
	.offs		= 0,
	.len		= 3,
	.veroffs	= 3,
	.maxblocks	= 4,
	.pattern	= bbt_pattern,
#ifdef USE_BBT_CHECKSUM
	.check_sum_offs = 32,
	.check_sum_len  = 3,
#endif
};

static struct nand_bbt_descr bbt_mirror_descr = {
	.options	= NAND_BBT_LASTBLOCK | NAND_BBT_CREATE | NAND_BBT_WRITE
	| NAND_BBT_2BIT | NAND_BBT_VERSION | NAND_BBT_PERCHIP,
	.offs		= 0,
	.len		= 3,
	.veroffs	= 3,
	.maxblocks	= 4,
	.pattern	= mirror_pattern,
#ifdef USE_BBT_CHECKSUM
	.check_sum_offs = 32,
	.check_sum_len  = 3,
#endif
};

#ifdef USE_BBT_CHECKSUM
static uint8_t scan_ff_pattern[] = { 0xff };
static struct nand_bbt_descr nvt_bbt_descr = {
	.options        = 0,
	.offs           = 0,
	.len            = 1,
	.pattern        = scan_ff_pattern,
};
#endif /* USE_BBT_CHECKSUM */
#endif /* USE_BBT */

static int nt72_nand_update_ecc_stats(struct mtd_info *mtd)
{
	struct nt72_nand_info *info = mtd->priv;

	if (unlikely(info->retcode == ERR_ECC_FAIL)) {
		/*
		 * We have no data on uncorrectable error count, let it be 1.
		 * Also we had corrected nothing, return early.
		 * he MTD layer will notice ecc_stats.failed is incremented and
		 * return -EBADMSG.
		 */
		mtd->ecc_stats.failed++;
		return 0;
	}
	/* The MTD layer wants to know how many errors are corrected by ECC */
	mtd->ecc_stats.corrected += info->ecc_result.err_count;
	return info->ecc_result.err_count;
}

static int nt72_nand_dev_ready(struct mtd_info *mtd)
{
	return 1;
}

static void nt72_nand_cmdfunc(struct mtd_info *mtd, unsigned command,
			      int column, int page_addr)
{
	struct nt72_nand_info *info = mtd->priv;

	info->retcode = ERR_NONE;

	switch (command) {
	case NAND_CMD_READOOB:
		check_cache_alignment(info, "cmdfunc: NAND_CMD_READOOB",
				      info->data_buff);
		info->buf_start = mtd->writesize + column;
		info->buf_count = mtd->writesize + info->nand_params.oob_size;
		nt72_nand_nfc_read_page(info, column, page_addr, 1, 1);
		/*
		 * We use default read OOB function from nand_base.c, which
		 * does not update ecc stats.
		 */
		nt72_nand_update_ecc_stats(mtd);
		break;
	case NAND_CMD_READ0:
		check_cache_alignment(info, "cmdfunc: NAND_CMD_READ0",
				      info->data_buff);
		info->buf_start = column;
		info->buf_count = mtd->writesize + info->nand_params.oob_size;
		nt72_nand_nfc_read_page(info, column, page_addr, 1, 0);
		break;
	case NAND_CMD_SEQIN:
		nt72_nand_cmd_history_add("seqin");
		nt72_nand_cmd_history_save_start_timestamp();
		info->buf_start = column;
		info->buf_count = mtd->writesize + info->nand_params.oob_size;
		memset(info->data_buff, 0xff, info->buf_count);

		info->seqin_column = column;
		info->seqin_page_addr = page_addr;
		nt72_nand_cmd_history_save_end_timestamp();
		break;
	case NAND_CMD_PAGEPROG:
		check_cache_alignment(info, "cmdfunc: NAND_CMD_PAGEPROG",
				      info->data_buff);
		nt72_nand_nfc_write_page(info, info->seqin_column,
					 info->seqin_page_addr);
		break;
	case NAND_CMD_ERASE1:
		nt72_nand_nfc_erase_block(info, page_addr);
		break;
	case NAND_CMD_ERASE2:
		break;
	case NAND_CMD_READID:
		nt72_nand_nfc_read_id(info, column, NULL, 0);
		break;
	case NAND_CMD_STATUS:
		nt72_nand_nfc_read_status(info, NULL);
		break;
	case NAND_CMD_RESET:
		/* ignore reset command because we had done it during probe */
		break;
	default:
		nfc_err("nfc_err: unsupported command: 0x%X\n", command);
		break;
	}
}

static uint8_t nt72_nand_read8(struct mtd_info *mtd)
{
	struct nt72_nand_info *info = mtd->priv;

	if (likely((info->buf_start < info->buf_count)))
		return info->data_buff[info->buf_start++];
	else
		return 0xFF;
}

static u16 nt72_nand_read16(struct mtd_info *mtd)
{
	struct nt72_nand_info *info = mtd->priv;
	/* algined to 16-bit */
	int b1 = !(info->buf_start & 0x01);
	int b2 = info->buf_start < info->buf_count;

	if (likely(b1 && b2)) {
		info->buf_start += 2;
		return *((u16 *)(info->data_buff + info->buf_start));
	}

	return 0xFFFF;
}

static void nt72_nand_read_buf(struct mtd_info *mtd, uint8_t *buf, int len)
{
	struct nt72_nand_info *info = mtd->priv;
	int real_len = min_t(int, len, info->buf_count - info->buf_start);

	memcpy(buf, info->data_buff + info->buf_start, real_len);
	info->buf_start += real_len;
}

static void nt72_nand_write_buf(struct mtd_info *mtd, const uint8_t *buf,
				int len)
{
	struct nt72_nand_info *info = mtd->priv;
	int real_len = min_t(int, len, info->buf_count - info->buf_start);

	memcpy(info->data_buff + info->buf_start, buf, real_len);
	info->buf_start += real_len;
}

#ifndef USE_NEW_MTD
static int nt72_nand_verify_buf(struct mtd_info *mtd,
				const uint8_t *buf, int len)
{
	return 0;
}
#endif

static void nt72_nand_select_chip(struct mtd_info *mtd, int chip)
{
}

static int nt72_nand_waitfunc(struct mtd_info *mtd, struct nand_chip *chip)
{
	struct nt72_nand_info *info = mtd->priv;
	int b1, b2, b3;

	b1 = info->retcode == ERR_WERROR;
	b2 = info->retcode == ERR_EERROR;
	b3 = info->retcode == ERR_ECC_FAIL;

	info->retcode = ERR_NONE;

	/*
	 * the upper layer only detect this bit: NAND_STATUS_FAIL, so we skipped
	 * reading whole status register
	 */
	if (unlikely(b1 || b2 || b3))
		return NAND_STATUS_FAIL;

#ifdef EXTRA_CHECK_IN_WAITFUNC
	{
		uint32_t status = 0;

		nt72_nand_nfc_read_status(info, &status);
		if (unlikely(status & NAND_STATUS_FAIL)) {
			nfc_err("nfc_err: Un-detected NAND_STATUS_FAIL in waitfunc\n");
			return status;
		}
	}
#endif

	return 0;
}

#ifdef USE_NEW_MTD
static int nt72_nand_read_page_hwecc(struct mtd_info *mtd,
				     struct nand_chip *chip, uint8_t *buf,
				     int oob_required, int page)
#else
static int nt72_nand_read_page_hwecc(struct mtd_info *mtd,
				     struct nand_chip *chip, uint8_t *buf,
				     int page)
#endif
{
#ifndef USE_NEW_MTD
	const int oob_required = 1;
#endif
	struct nt72_nand_info *info = mtd->priv;

	memcpy(buf, info->data_buff, info->nand_params.page_size);
	if (likely(oob_required)) {
		memcpy(info->chip.oob_poi,
		       info->data_buff + info->nand_params.page_size,
		       info->nand_params.oob_size);
	}

	return nt72_nand_update_ecc_stats(mtd);
}

#ifdef USE_NEW_MTD
static int nt72_nand_write_page_hwecc(struct mtd_info *mtd,
				      struct nand_chip *chip,
				      const uint8_t *buf, int oob_required)
#else
static void nt72_nand_write_page_hwecc(struct mtd_info	*mtd,
				       struct nand_chip *chip,
				       const uint8_t	*buf)
#endif
{
	struct nt72_nand_info *info = mtd->priv;

	memcpy(info->data_buff, buf, info->nand_params.page_size);
	if (oob_required)
		memcpy(info->data_buff + info->nand_params.page_size,
		       chip->oob_poi, info->nand_params.oob_size);

#ifdef USE_NEW_MTD
	return 0;
#endif
}

static void nt72_nand_ecc_hwctl(struct mtd_info *mtd, int mode)
{
}

static int nt72_nand_ecc_calculate(struct mtd_info *mtd,
				   const uint8_t *dat, uint8_t *ecc_code)
{
	return 0;
}

static int nt72_nand_ecc_correct(struct mtd_info *mtd,
				 uint8_t *dat, uint8_t *read_ecc,
				 uint8_t *calc_ecc)
{
	return 0;
}

#ifdef USE_BBT
static void __init nt72_nand_bbt_init(struct nt72_nand_info *info)
{
	struct nand_chip *this = &info->chip;

#ifdef USE_NEW_MTD
	this->bbt_options |=
		(NAND_BBT_USE_FLASH | info->nand_params.bbt_options);
#else
	this->options |= (NAND_USE_FLASH_BBT | info->nand_params.bbt_options);
#endif

	bbt_main_descr.veroffs = bbt_mirror_descr.veroffs = 1;
	bbt_main_descr.offs = bbt_mirror_descr.offs =
				      info->nand_params.oob_size /
				      info->nand_params.nr_subpage;

#ifdef USE_BBT_CHECKSUM
	bbt_main_descr.check_sum_offs = info->nand_params.suboob_size * 2;
	bbt_main_descr.check_sum_len = 3;

	bbt_mirror_descr.check_sum_offs = info->nand_params.suboob_size * 2;
	bbt_mirror_descr.check_sum_len = 3;

	this->badblock_pattern  = &nvt_bbt_descr;
#endif

	this->bbt_td = &bbt_main_descr;
	this->bbt_md = &bbt_mirror_descr;

	nfc_msg("nfc: main_bbt: offs=%d len=%d veroffs=%d maxblocks=%d\n",
		bbt_main_descr.offs, bbt_main_descr.len,
		bbt_main_descr.veroffs, bbt_main_descr.maxblocks);
	nfc_msg("nfc: mirror_bbt: offs=%d len=%d veroffs=%d maxblocks=%d\n",
		bbt_mirror_descr.offs, bbt_mirror_descr.len,
		bbt_mirror_descr.veroffs, bbt_mirror_descr.maxblocks);
}
#endif /* USE_BBT */

void __init nt72_nand_init_nand_chip(struct nt72_nand_info *info)
{
	struct nand_chip *this = &info->chip;
	struct nt72_nand_params *p = &info->nand_params;

	this->options = (p->flash_width == 16) ? NAND_BUSWIDTH_16 : 0;

	this->read_byte = nt72_nand_read8;
	this->read_word = nt72_nand_read16;
	this->write_buf = nt72_nand_write_buf;
	this->read_buf = nt72_nand_read_buf;
#ifndef USE_NEW_MTD
	this->verify_buf = nt72_nand_verify_buf;
#endif
	this->select_chip = nt72_nand_select_chip;

	this->dev_ready = nt72_nand_dev_ready;
	this->cmdfunc = nt72_nand_cmdfunc;
	this->waitfunc = nt72_nand_waitfunc;

	this->ecc.hwctl = nt72_nand_ecc_hwctl;
	this->ecc.calculate = nt72_nand_ecc_calculate;
	this->ecc.correct = nt72_nand_ecc_correct;
	this->ecc.read_page = nt72_nand_read_page_hwecc;
	this->ecc.write_page = nt72_nand_write_page_hwecc;

	this->ecc.mode = NAND_ECC_HW;
	this->ecc.size = p->page_size;

#ifdef USE_NEW_MTD
	this->ecc.strength = p->ecc.nr_bits;
	info->mtd->bitflip_threshold = p->bitflip_threshold;
#endif

	if ((p->ecc.type == BCH) && (p->ecc.nr_bits == 8) &&
	    (p->ecc.data_size == 512) && (p->ecc.extra_size == 16))
		this->ecc.layout = &g_hw_2KB_page_bch8_16_ecclayout;
	else if ((p->ecc.type == RS) && (p->ecc.nr_bits == 4) &&
		 (p->ecc.data_size == 512) && (p->ecc.extra_size == 16))
		this->ecc.layout = &g_hw_2KB_page_rs4_16_ecclayout;
	else
		nfc_err("nfc_err: error select ecc layout\n");

	this->chip_delay = 25;

#ifdef USE_BBT
	nt72_nand_bbt_init(info);
#endif
}
