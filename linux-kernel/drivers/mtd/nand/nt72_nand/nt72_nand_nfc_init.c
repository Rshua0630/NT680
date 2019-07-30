#include "include/nt72_nand_regs.h"
#include "include/nt72_nand_specs.h"
#include "include/nt72_nand_nfc.h"

static void __init nt72_nand_nfc_regs_init(struct nt72_nand_info *info)
{
	u32 _reg;
	struct nt72_nand_params *p = &(info->nand_params);

	/* init REG_NFC_XD_CFG */
	_reg = nfc_read32(REG_NFC_XD_CFG);
	nvt_bclear(_reg, NFC_AHB_MASTER_MSK);
	nvt_bset(_reg, NFC_AHB_MASTER_16_8_4BEAT);
	p->regs.xd_cfg = _reg;

	/* init REG_NFC_SYSCTRL */
	_reg = NFC_SYSCTRL_EXTRA_SIZE(p->ecc.extra_size);
	switch (p->page_size) {
	case 2048:
		_reg |= NFC_SYSCTRL_PAGE_2048;
		break;
	case 4096:
		_reg |= NFC_SYSCTRL_PAGE_4096;
		break;
	case 8192:
		_reg |= NFC_SYSCTRL_PAGE_8192;
		break;
	default:
		nfc_err("nfc_err: REG_NFC_SYSCTRL illegal page size\n");
		break;
	}
	switch (p->subpage_size) {
	case 512:
		_reg |= NFC_SYSCTRL_SUBPAGE_512;
		break;
	case 1024:
		_reg |= NFC_SYSCTRL_SUBPAGE_1024;
		break;
	default:
		nfc_err("nfc_err: REG_NFC_SYSCTRL illegal subpage size\n");
		break;
	}
	/* block size */
	switch (p->page_size * p->nr_page_per_block / 1024) {
	case 128:
		_reg |= NFC_SYSCTRL_BLK_128K;
		break;
	case 256:
		_reg |= NFC_SYSCTRL_BLK_256K;
		break;
	case 512:
		_reg |= NFC_SYSCTRL_BLK_512K;
		break;
	default:
		nfc_err("nfc_err: REG_NFC_SYSCTRL illegal block size\n");
		break;
	}
	switch (p->flash_width) {
	case 8:
		break;
	case 16:
		nfc_err("nfc_err: REG_NFC_SYSCTRL width=16 not supported\n");
		break;
	default:
		nfc_err("nfc_err: REG_NFC_SYSCTRL illegal width\n");
		break;
	}
	switch (p->ecc.type) {
	case BCH:
		_reg |= NFC_SYSCTRL_ECC_BCH;
		break;
	case RS:
		break;
	default:
		nfc_err("nfc_err: REG_NFC_SYSCTRL illegal ecc type\n");
		break;
	}
	p->regs.sysctrl = _reg;

	/* init REG_NFC_SYSCTRL1 */
	p->regs.sysctrl1_read = NFC_SYSCTRL1_NEW_VERSION |
				NFC_SYSCTRL1_FRUN_FULL_PAGE_DMA |
				NFC_SYSCTRL1_LATCH_READ_DELAY_1T;
	p->regs.sysctrl1_prog = NFC_SYSCTRL1_NEW_VERSION |
				NFC_SYSCTRL1_FRUN_FULL_PAGE_DMA_WRITE |
				NFC_SYSCTRL1_FRUN_FULL_PAGE_DMA |
				NFC_SYSCTRL1_LATCH_READ_DELAY_1T;
	if (p->ecc.type == BCH) {
		int b1 = p->ecc.nr_bits == 8;
		int b2 = p->ecc.extra_size == 54;

		nvt_bset(p->regs.sysctrl1_read,
			 NFC_SYSCTRL1_DECODE_FROM_BUFFER);
		nvt_bset(p->regs.sysctrl1_prog,
			 NFC_SYSCTRL1_DECODE_FROM_BUFFER);
		if (!(b1 && b2)) {
			nvt_bset(p->regs.sysctrl1_read,
				 NFC_SYSCTRL1_NO_REMOVE_EXTRA_2DUMMY_BYTE);
			nvt_bset(p->regs.sysctrl1_prog,
				 NFC_SYSCTRL1_NO_REMOVE_EXTRA_2DUMMY_BYTE);
		}
	} else if (p->ecc.type == RS) {
		nvt_bset(p->regs.sysctrl1_read,
			 NFC_SYSCTRL1_DECODE_FROM_BUFFER |
			 NFC_SYSCTRL1_NO_REMOVE_EXTRA_2DUMMY_BYTE);
		nvt_bset(p->regs.sysctrl1_prog,
			 NFC_SYSCTRL1_DECODE_FROM_BUFFER |
			 NFC_SYSCTRL1_NO_REMOVE_EXTRA_2DUMMY_BYTE);
	} else
		nfc_err("nfc_err: regs_init SYSCTRL1 error\n");

	/* init REG_NFC_SYSCTRL2 */
	p->regs.sysctrl2_prog = 0;
	p->regs.sysctrl2_read =
		NFC_SYSCTRL2_BLANK_CHECK_THRESHOLD(p->blank_0xff_threshold) |
		NFC_SYSCTRL2_BLANK_CHECK_EN |
		NFC_SYSCTRL2_XTRA_PTR_PARK |
		NFC_SYSCTRL2_DISABLE_FULLPAGE_DMA_WRITE_ECC;

	/* init REG_NFC_CFG1 */
	_reg = NFC_CFG1_BUSY_TO_READY_TIMEOUT(-1) |
	       NFC_CFG1_LITTLE_ENDIAN |
	       NFC_CFG1_LITTLE_ENDIAN_XTRA | NFC_CFG1_READY_TO_BUSY_TIMEOUT(-1);
	if ((p->ecc.type == RS) && (p->ecc.nr_bits == 4)) {
		switch (p->ic_id) {
		case 0x72458:
		case 0x72658:
			_reg |= NFC_CFG1_ECC_RS_NEW;
			break;
		default:
			nfc_err("nfc_err: REG_NFC_CFG1 unknown ic_id (RS)\n");
			break;
		}
	} else if ((p->ecc.type == BCH) && (p->ecc.nr_bits == 8)) {
		if (p->ic_id == 0x72453) {
			_reg |= NFC_CFG1_BCH_EN;
			switch (p->ecc.extra_size) {
			case 16:
				_reg |= NFC_CFG1_ECC_BCH8_16;
				break;
			case 32:
				_reg |= NFC_CFG1_ECC_BCH8_32;
				break;
			case 56:
				_reg |= NFC_CFG1_ECC_BCH8_56;
				break;
			default:
				nfc_err("nfc_err: REG_NFC_CFG1 illegal BCH\n");
			}
		} else
			nfc_err("nfc_err: REG_NFC_CFG1 unknown ic_id (BCH)\n");
	} else if ((p->ecc.type == BCH) && (p->ecc.nr_bits == 24)) {
		nfc_err("nfc_err: REG_NFC_CFG1 not yet implemented (BCH24)\n");
		switch (p->ecc.extra_size) {
		case 54:
			break;
		default:
			break;
		}
	} else
		nfc_err("nfc_err: REG_NFC_CFG1 illegal ECC\n");
	p->regs.cfg1 = _reg;
}

static void __init nt72_nand_nfc_parse_ecc_1pin_bs(struct nt72_nand_info *info)
{
	struct nt72_nand_params *p = &info->nand_params;

	/* BCHxxx or RS */
	if (stbc_read32(REG_STBC_BOOTSTRAP) & (1 << 18)) {
		switch (p->ic_id) {
		case 0x72458:
		case 0x72658:
			p->ecc.type = BCH;
			p->ecc.nr_bits = 24;
			p->ecc.data_size = 1024;
			p->ecc.extra_size = 54;
			break;
		default:
			nfc_err(
				"nfc_err: Unknown ic_id to determine ECC type (bs1).\n");
			break;
		}
	} else {
		p->ecc.type = RS;
		p->ecc.nr_bits = 4;
		p->ecc.data_size = 512;
		p->ecc.extra_size = 16;
	}
}

static void __init nt72_nand_nfc_parse_ecc_2pin_bs(struct nt72_nand_info *info)
{
	struct nt72_nand_params *p = &info->nand_params;

	switch ((stbc_read32(REG_STBC_BOOTSTRAP) >> 17) & 0x3) {
	case 0:
		p->ecc.type = RS;
		p->ecc.nr_bits = 4;
		p->ecc.data_size = 512;
		p->ecc.extra_size = 16;
		break;
	/* be care that this is 2 */
	case 2:
		p->ecc.type = BCH;
		p->ecc.nr_bits = 8;
		p->ecc.data_size = 512;
		p->ecc.extra_size = 16;
		break;
	/* be care that this is 1 */
	case 1:
		p->ecc.type = BCH;
		p->ecc.nr_bits = 8;
		p->ecc.data_size = 512;
		p->ecc.extra_size = 32;
		break;
	case 3:
		p->ecc.type = BCH;
		p->ecc.nr_bits = 8;
		p->ecc.data_size = 512;
		p->ecc.extra_size = 56;
		break;
	default:
		nfc_err("nfc_err: Unknown ic_id to determine ECC type (bs2).\n");
		break;
	}
}

void __init nt72_nand_nfc_nand_params_init(struct nt72_nand_info *info,
					   struct nt72_nand_spec *f)
{
	u32 low_id, ext_id;
	struct nt72_nand_params *p = &info->nand_params;

	/* nand parameters */
	p->chip_id = f->chip_id;
	p->flash_width = f->flash_width;

	p->page_size = f->page_size;
	p->subpage_size = f->subpage_size;
	p->oob_size = f->oob_size;
	p->suboob_size = (f->oob_size) / (f->page_size / f->subpage_size);

	p->nr_blocks = f->num_blocks;
	p->nr_page_per_block = f->page_per_block;
	p->nr_subpage = p->page_size / p->subpage_size;

	p->size = p->page_size * p->nr_page_per_block * p->nr_blocks;
	p->dma_buf_size = p->page_size + p->oob_size;

	/* set ic_id */
	low_id = clkgen_read32(REG_CLK_GEN_IC_ID) & 0xFFFF;
	ext_id = (clkgen_read32(REG_CLK_GEN_IC_ID) >> 24) & 0xF;
	p->ic_id = low_id | (ext_id << 16);

	/* set cache_line_size and BCH type */
	switch (p->ic_id) {
	case 0x72453:
		p->cache_line_size = 64;
		nt72_nand_nfc_parse_ecc_2pin_bs(info);
		break;
	case 0x72458:
	case 0x72658:
		p->cache_line_size = 64;
		nt72_nand_nfc_parse_ecc_1pin_bs(info);
		break;
	default:
		nfc_err("nfc_err: error ic_id\n");
	}

	/* load timing */
	p->regs.cfg0_read = f->cfg0_read;
	p->regs.cfg0_prog = f->cfg0_prog;
	p->regs.ftune = f->ftune;

	/* misc */
	p->blank_0xff_threshold = f->blank_0xff_threshold;
	p->blank_non_0xff_threshold = f->blank_non_0xff_threshold;

	p->bbt_options = f->bbt_options;

	/* init other regs */
	nt72_nand_nfc_regs_init(info);
}

void __init nt72_nand_nfc_nand_params_print(struct nt72_nand_info *info)
{
	struct nt72_nand_params *p = &info->nand_params;

	nfc_msg("nfc: chip_id=0x%X flash_width=%d page_size=%d\n",
		p->chip_id, p->flash_width, p->page_size);
	nfc_msg("nfc: subpage_size=%d oob_size=%d sub_oobsize=%d\n",
		p->subpage_size, p->oob_size, p->suboob_size);
	nfc_msg("nfc: nr_blocks=%d\n", p->nr_blocks);
	nfc_msg("nfc: nr_page_per_block=%d nr_subpage=%d size=%lluMB\n",
		p->nr_page_per_block, p->nr_subpage, p->size / 1024 / 1024);
	nfc_msg("nfc: dma_buf_size=%d ic_id=0x%X cache_line_size=%d\n",
		p->dma_buf_size, p->ic_id, p->cache_line_size);
	nfc_msg("nfc: blank_threshold=%d-%d bbt_options=0x%X ",
		p->blank_0xff_threshold, p->blank_non_0xff_threshold,
		p->bbt_options);

	switch (p->ecc.type) {
	case RS:
		nfc_cont("ECC=RS%d (%d+%d)\n", p->ecc.nr_bits,
			 p->ecc.data_size, p->ecc.extra_size);
		break;
	case BCH:
		nfc_cont("ECC=BCH%d (%d+%d)\n", p->ecc.nr_bits,
			 p->ecc.data_size, p->ecc.extra_size);
		break;
	default:
		nfc_err("nfc_err: ECC type unknown!\n");
		break;
	}

	nfc_msg("nfc: cfg0_read=0x%08X cfg0_prog=0x%08X ftune=0x%08X\n",
		p->regs.cfg0_read, p->regs.cfg0_prog, p->regs.ftune);
#ifdef ENABLE_CMD_HISTORY
	nfc_msg("nfc: cmd history on\n");
#endif
#ifdef SELFTEST_FAKE_BITFLIP
	nfc_msg("nfc: fake bitflip on\n");
#endif
#ifdef SELFTEST_FAKE_BITFLIP_BLANK
	nfc_msg("nfc: blank page fake bitflip on\n");
#endif
}

#ifdef SELFTEST_NAND_PARAMS
void __init nt72_nand_nfc_nand_params_selftest(struct nt72_nand_info *info)
{
	int b1, b2, b3, b4;
	struct nt72_nand_params *p = &info->nand_params;
	struct mtd_info *m = info->mtd;

	/* make sure nand_params is reasonable */
	b1 = p->page_size == 2048;
	b2 = p->page_size == 4096;
	b3 = p->page_size == 8192;
	if (!(b1 || b2 || b3))
		nfc_err("nfc_err: illegal nand page size\n");

	b1 = p->subpage_size == 512;
	b2 = p->subpage_size == 1024;
	if (!(b1 || b2))
		nfc_err("nfc_err: illegal nand subpage size\n");

	if (!((p->flash_width == 16) || (p->flash_width == 8)))
		nfc_err("nfc_err: illegal nand flash width\n");

	if (p->dma_buf_size != (p->page_size + p->oob_size))
		nfc_err("nfc_err: illegal dma_buf_size\n");

	if (p->page_size != (p->subpage_size * p->nr_subpage))
		nfc_err("nfc_err: illegal subpage size\n");

	if (p->nr_subpage > MAX_NR_SUBPAGE)
		nfc_err("nfc_err: nr_subpage too large\n");
	/* make sure mtd_info is reasonable */
	if (m->size != p->size)
		nfc_err("nfc_err: nand flash size mismatch\n");

	if (m->oobsize != p->oob_size)
		nfc_err("nfc_err: nand flash OOB size mismatch\n");

	if (m->erasesize != (p->page_size * p->nr_page_per_block))
		nfc_err("nfc_err: nand flash size mismatch\n");

	b1 = (m->erasesize / 1024 / 1024) != 128;
	b2 = (m->erasesize / 1024 / 1024) != 256;
	b3 = (m->erasesize / 1024 / 1024) != 512;
	b4 = (m->erasesize / 1024 / 1024) != 1024;
	if (!(b1 || b2 || b3 || b4))
		nfc_err("nfc_err: illegal erasesize\n");
	/* refer to comment of struct mtd_info */
	b1 = m->writesize == p->page_size;
	b2 = m->writesize == (p->page_size / 2);
	b3 = m->writesize == (p->page_size / 4);
	if (!(b1 || b2 || b3))
		nfc_err("nfc_err: illegal writesize\n");
	/* check ECC params */
	if (p->ecc.data_size != p->subpage_size)
		nfc_err("nfc_err: invalid ecc data size\n");
	if (p->ecc.type == RS) {
		b1 = (p->ecc.nr_bits == 4) &&
		     (p->ecc.data_size == 512) && (p->ecc.extra_size == 16);
		if (!b1)
			nfc_err("nfc_err: illegal RS params\n");
	} else if (p->ecc.type == BCH) {
		b1 = (p->ecc.nr_bits == 8) &&
		     (p->ecc.data_size == 512) && (p->ecc.extra_size == 16);
		b2 = (p->ecc.nr_bits == 8) &&
		     (p->ecc.data_size == 512) && (p->ecc.extra_size == 32);
		b3 = (p->ecc.nr_bits == 8) &&
		     (p->ecc.data_size == 512) && (p->ecc.extra_size == 56);
		b4 = (p->ecc.nr_bits == 24) &&
		     (p->ecc.data_size == 1024) && (p->ecc.extra_size == 54);
		if (!(b1 || b2 || b3 || b4))
			nfc_err("nfc_err: illegal BCH params\n");
	}
	/* noted that it is BOOTSTRAP[17:18], not BOOTSTRAP[18:17] */
	switch ((stbc_read32(REG_STBC_BOOTSTRAP) >> 17) & 0x03) {
	case 0:
		if (p->ecc.type != RS)
			nfc_err("nfc_err: bootstrap ecc type is not RS\n");
		break;
	case 1:
	case 2:
	case 3:
		if (p->ecc.type != BCH)
			nfc_err("nfc_err: bootstrap ecc type is not BCH\n");
		break;
	default:
		nfc_err("nfc_err: bootstrap ecc type unknown\n");
		break;
	}
}
#endif

int __init nt72_nand_nfc_detect_flash(struct nt72_nand_info *info,
				      struct nt72_nand_spec **nand_spec)
{
	u32 chip_id = -1;
	int i = 0;

	for (i = 0; i < g_nr_nt72_nand_specs; i++) {
		/*
		 * Here we do early setup for read_id because NFC registers are
		 * not initialized yet
		 */
		nt72_nand_nfc_sw_reset();
		info->nand_params.regs.cfg0_read =
			g_nt72_nand_specs[i].cfg0_read;
		info->nand_params.regs.ftune = g_nt72_nand_specs[i].ftune;
		nt72_nand_nfc_read_id(info, 0, &chip_id, 1);

		if (info->retcode != ERR_NONE)
			continue;

		if ((chip_id & 0xFFFF) ==
		    (g_nt72_nand_specs[i].chip_id & 0xFFFF)) {
			*nand_spec =
				(struct nt72_nand_spec *)&g_nt72_nand_specs[i];
			return 0;
		}
	}
	*nand_spec = NULL;
	nfc_err("nfc_err: unknown flash chip: 0x%X\n", chip_id);
	return -ENODEV;
}
