#include "include/nt72_nand.h"
#include "include/nt72_nand_regs.h"
#include "include/nt72_nand_io.h"
#include "include/nt72_nand_ir.h"
#include "include/nt72_nand_kapi.h"
#include "include/nt72_nand_nfc.h"
#include "include/nt72_nand_selftest.h"
#include "include/nt72_nand_utils.h"

#ifdef DUMP_CONTROL_REGS
void nt72_nand_nfc_dump_ctrl_regs(const char *who)
{
	nfc_err("%s: dump control registers\n", who);
	nfc_err("REG_NFC_XD_CFG=0x%08X\n", nfc_read32(REG_NFC_XD_CFG));
	nfc_err("REG_NFC_SYSCTRL=0x%08X\n", nfc_read32(REG_NFC_SYSCTRL));
	nfc_err("REG_NFC_SYSCTRL1=0x%08X\n", nfc_read32(REG_NFC_SYSCTRL1));
	nfc_err("REG_NFC_SYSCTRL2=0x%08X\n", nfc_read32(REG_NFC_SYSCTRL2));
	nfc_err("REG_NFC_CFG0=0x%08X\n", nfc_read32(REG_NFC_CFG0));
	nfc_err("REG_NFC_CFG1=0x%08X\n", nfc_read32(REG_NFC_CFG1));
	nfc_err("REG_NFC_FTUNE=0x%08X\n", nfc_read32(REG_NFC_FTUNE));
	nfc_err("REG_NFC_XFER=0x%08X\n", nfc_read32(REG_NFC_XFER));
}
#endif

#ifdef DUMP_REG_ON_ERROR
void nt72_nand_nfc_dump_regs(struct nt72_nand_info *info, int page_addr)
{
	nfc_err("block:%d page:%d INTSTS:0x%08X CN0:0x%08X CN1:0x%08X NFC_STS:0x%08X\n",
		page_addr / info->nand_params.nr_page_per_block,
		page_addr % info->nand_params.nr_page_per_block,
		nfc_read32(REG_NFC_INTSTS), nfc_read32(REG_NFC_ERR_CNT0),
		nfc_read32(REG_NFC_ERR_CNT1), nfc_read32(REG_NFC_STS));
}
#endif

#ifdef DUMP_DATA_ON_ERROR
void nt72_nand_nfc_dump_data(struct nt72_nand_info *info, int page_addr)
{
	int i;
	u32 *buf = (u32 *)info->data_buff;
	int size = info->nand_params.dma_buf_size / 4;

	nfc_err("dump_data_start\n");
	for (i = 0; i < size; i += 4) {
		nfc_msg("0x%08X    %08X %08X %08X %08X\n",
			i * 4, buf[i], buf[i + 1], buf[i + 2], buf[i + 3]);
	}
	nfc_err("dump_data_end\n");

	nt72_nand_cmd_history_dump();
}
#endif

#ifdef DUMP_ECC_WHEN_UNCORRECTABLE
void nt72_nand_nfc_dump_ecc(struct nt72_nand_info *info)
{
	int i;

	nfc_err("ECC uncorrectable: ");

	for (i = 0; i < info->nand_params.nr_subpage; i++)
		nfc_cont("%d ", info->ecc_result._err_count[i]);

	nfc_cont("error_count: ");
	for (i = 0; i < info->nand_params.nr_subpage; i++)
		nfc_cont("%d ", info->ecc_result._err_count[i]);

	nfc_cont("uncorrectable: ");
	for (i = 0; i < info->nand_params.nr_subpage; i++)
		nfc_cont("%d ", info->ecc_result._uncorrectable[i]);

	nfc_cont("blank: ");
	for (i = 0; i < info->nand_params.nr_subpage; i++)
		nfc_cont("%d ", info->ecc_result._blank[i]);
}
#endif

#ifdef EXTRA_WAIT_NFC_READY
void nt72_nand_nfc_wait_nfc_ready(const char *who)
{
	unsigned long stop_time = nt72_nand_kapi_timeout_init();

#ifdef SELFTEST_REG_POLLING_TIMEOUT
	int counter = 1;
#endif

	while (nfc_read32(REG_NFC_STS) &
	       (NFC_STS_NAND_BUSY | NFC_STS_ACTIVE)) {
		nfc_err("nfc_err: %s is polling nfc ready\n", who);

#ifdef SELFTEST_REG_POLLING_TIMEOUT
		if ((counter % 1000) == 0)
			ssleep(REG_POLLING_TIMEOUT / HZ * 2);

#endif
		if (nt72_nand_kapi_timeout_check(stop_time)) {
			nfc_err("nfc_err: %s polling nfc ready timeout\n", who);
			break;
		}
	}
}
#endif

void nt72_nand_nfc_ecc_workaround(struct nt72_nand_info *info)
{
	/* This is workaround to prevent ECC misbehavior during PIO mode */
	nt72_nand_nfc_sw_reset();
	/* And reset ECC just in case */
	nt72_nand_nfc_ecc_reset(info);
}

int nt72_nand_nfc_count_non_ff64(u64 *data)
{
	int i = 0;
	u8 *data8 = (u8 *)data;
	u8 *data8_end = data8 + 8;

	for (; data8 < data8_end; data8++) {
		if (*data8 != 0xFF)
			i++;
	}

	return i;
}

void nt72_nand_nfc_fill_blank_subpage(struct nt72_nand_info *info,
				      int page_addr, int subpage_id,
				      int oob_only)
{
#ifdef EXTRA_PRINT_NON_FF_COUNT
	const int print_cnt = 1;
#else
	const int print_cnt = 0;
#endif
	struct nt72_nand_params *p = &(info->nand_params);

	int non_ff_cnt = 0;
	int non_ff_thr = p->blank_non_0xff_threshold;

	/* pointers to the subpage and the suboob */
	u8 *data = (u8 *)(info->data_buff + (p->subpage_size * subpage_id));
	u8 *extra = (u8 *)(info->data_buff + p->page_size + (p->suboob_size *
							     subpage_id));
	u64 *data64 = (u64 *)data;
	u64 *data64_end = (u64 *)(data + p->subpage_size);

	/* if subpage and suboob are full of 0xFF, it is blank page */
	if (memcmp(extra, info->ff_pattern, p->suboob_size) == 0) {
		if (memcmp(data, info->ff_pattern, p->subpage_size) == 0) {
#ifndef SELFTEST_FAKE_BITFLIP_BLANK
			return;
#else
			nt72_nand_selftest_fake_bitflip_blank(info, subpage_id);
#endif
		}
	}

	/* check the subpage */
	for (; data64 < data64_end; data64++) {
		if (*data64 != 0xFFFFFFFFFFFFFFFFul) {
			non_ff_cnt += nt72_nand_nfc_count_non_ff64(data64);
			if (!print_cnt && (non_ff_cnt > non_ff_thr))
				return;
		}
	}

	/* check the suboob */
	if (p->ecc.extra_size == 16) {
		u64 *extra64 = (u64 *)extra;
		u64 *extra64_end = (u64 *)(extra + p->suboob_size);

		for (; extra64 < extra64_end; extra64++) {
			if (*extra64 != 0xFFFFFFFFFFFFFFFFul) {
				non_ff_cnt +=
					nt72_nand_nfc_count_non_ff64(extra64);
				if (!print_cnt && (non_ff_cnt > non_ff_thr))
					return;
			}
		}
	} else if (p->ecc.extra_size == 54) {
		u8 *extra8 = extra;
		u8 *extra8_end = extra8 + p->suboob_size;

		for (; extra8 < extra8_end; extra8++) {
			if (*extra8 != 0xFF) {
				if ((++non_ff_cnt > non_ff_thr) && !print_cnt)
					return;
			}
		}
	} else
		nfc_err("nfc_err: fill_blank_subpage wrong extra_size\n");

	if (print_cnt) {
		if (non_ff_cnt > non_ff_thr) {
			nfc_msg("nfc_msg: above blank threshold: %d\n",
				non_ff_cnt);
			nt72_nand_nfc_dump_data(info, page_addr);
			return;
		}
	}

	if (!oob_only)
		memset(data, 0xFF, p->subpage_size);
	memset(extra, 0xFF, p->suboob_size);
}

void nt72_nand_nfc_fill_blank_page(struct nt72_nand_info *info, int page_addr,
				   int oob_only)
{
	int i;

	for (i = 0; i < info->nand_params.nr_subpage; i++) {
		if (info->ecc_result._blank[i])
			nt72_nand_nfc_fill_blank_subpage(info, page_addr, i,
							 oob_only);
	}
}

void nt72_nand_nfc_hw_reset(void)
{
	u32 _reg = stbc_read32(REG_STBC_1C);

	nvt_bset(_reg, STBC_1C_NFC_RST);
	stbc_write32(REG_STBC_1C, _reg);
	nvt_bclear(_reg, STBC_1C_NFC_RST);
	stbc_write32(REG_STBC_1C, _reg);
}

void nt72_nand_nfc_sw_reset(void)
{
	unsigned long stop_time = nt72_nand_kapi_timeout_init();

#ifdef SELFTEST_REG_POLLING_TIMEOUT
	int counter = 1;
#endif

	nfc_write32(REG_NFC_SFTRST, NFC_SFTRST_CONTROLLER |
		    NFC_SFTRST_DMA | NFC_SFTRST_BUFFER);

	while (nfc_read32(REG_NFC_SFTRST) != 0) {
#ifdef SELFTEST_REG_POLLING_TIMEOUT
		if ((counter % 1000) == 0)
			ssleep(REG_POLLING_TIMEOUT / HZ * 2);
#endif
		if (nt72_nand_kapi_timeout_check(stop_time))
			break;
		nt72_nand_kapi_cpu_relax();
	}
}

void nt72_nand_nfc_ecc_reset(struct nt72_nand_info *info)
{
	u32 cfg1 = info->nand_params.regs.cfg1;

	/*
	 * Workaround:
	 * In order to reset in BCH, you need to be in non-Freerun mode
	 */
	if (info->nand_params.ecc.type == BCH) {
		u32 sysctrl1 = info->nand_params.regs.sysctrl1_read;

		nvt_bclear(sysctrl1, NFC_SYSCTRL1_FRUN_FULL_PAGE_DMA |
			   NFC_SYSCTRL1_FRUN_FULL_PAGE_DMA_WRITE |
			   NFC_SYSCTRL1_DECODE_FROM_BUFFER);
		nfc_write32(REG_NFC_SYSCTRL1, sysctrl1);
	}

	/*
	 * Do the reset, because it is blocking write through APB, no need
	 * waiting, just flip the bit
	 */
	nvt_bset(cfg1, NFC_CFG1_ECC_ENCODE_RESET | NFC_CFG1_ECC_DECODE_RESET);
	nfc_write32(REG_NFC_CFG1, cfg1);

	/* Pull down reset and restore NFC_CFG1_BCH_EN */
	nfc_write32(REG_NFC_CFG1, info->nand_params.regs.cfg1);
}

int nt72_nand_nfc_check_int_status(const char *who, struct nt72_nand_info *info,
				   int page_addr, int read, int oob_only)
{
	int result = ERR_NONE;
	u32 int_status = nfc_read32(REG_NFC_INTSTS);
	/* means we are booting and scanning for bad blocks */
	int boot_scan_bad_block = info->nand_scanning && oob_only;

	if (unlikely(int_status & NFC_INTSTS_ERRORS)) {
		nfc_err("nfc_err: %s error!\n", who);
		result = ERR_STATUS;
		goto has_error;
	}

	if (unlikely(int_status & NFC_INTSTS_TIMEOUTS)) {
		nfc_err("nfc_err: %s timeout!\n", who);
		result = ERR_TIMEOUT;
		goto has_error;
	}

	/* There are no timeouts, so this pin must mean ecc uncorrectable */
	if (unlikely(read && (int_status & NFC_INTSTS_TIMEOUT_UNCORRECTABLE))) {
		/* double check here */
		if (unlikely(!info->ecc_result.uncorrectable)) {
			/* should not be here */
			nfc_err("nfc_err: %s ECC uncorrectable (mismatch)\n",
				who);
		}
	}

	if (unlikely(read && info->ecc_result.uncorrectable)) {
		/* Only report error if we are not scanning bad blocks */
		if (likely(!boot_scan_bad_block)) {
			nfc_err("nfc_err: %s ECC uncorrectable!\n", who);
			result = ERR_ECC_FAIL;
			goto has_error;
		}
		nfc_warn("nfc_warn: %s ECC uncorrectable during bbt scan\n",
			 who);
		return ERR_ECC_FAIL;
	}

	return ERR_NONE;

has_error:
	nt72_nand_nfc_dump_ecc(info);
	nt72_nand_nfc_dump_regs(info, page_addr);
	if (read)
		nt72_nand_nfc_dump_data(info, page_addr);
	return result;
}

void nt72_nand_nfc_prepare_pio_regs(struct nt72_nand_info *info)
{
	u32 sysctrl1;
	struct nt72_nand_params *p = &(info->nand_params);

	nfc_write32(REG_NFC_SYSCTRL, p->regs.sysctrl);

	sysctrl1 = p->regs.sysctrl1_read;
	nvt_bclear(sysctrl1, NFC_SYSCTRL1_NEW_VERSION |
		   NFC_SYSCTRL1_FRUN_FULL_PAGE_DMA |
		   NFC_SYSCTRL1_FRUN_FULL_PAGE_DMA_WRITE |
		   NFC_SYSCTRL1_DECODE_FROM_BUFFER);
	nfc_write32(REG_NFC_SYSCTRL1, sysctrl1);

	nfc_write32(REG_NFC_CFG0, p->regs.cfg0_read);

	nfc_write32(REG_NFC_CFG1, p->regs.cfg1);

	nfc_write32(REG_NFC_FTUNE, p->regs.ftune);
}

void nt72_nand_nfc_prepare_rw_regs(struct nt72_nand_info *info, int write)
{
	struct nt72_nand_params *p = &(info->nand_params);

	if (!write)
		nfc_write32(REG_NFC_XD_CFG, p->regs.xd_cfg);

	nfc_write32(REG_NFC_SYSCTRL, p->regs.sysctrl);

	if (write)
		nfc_write32(REG_NFC_SYSCTRL1, p->regs.sysctrl1_prog);
	else
		nfc_write32(REG_NFC_SYSCTRL1, p->regs.sysctrl1_read);

	if (write)
		nfc_write32(REG_NFC_SYSCTRL2, p->regs.sysctrl2_prog);
	else
		nfc_write32(REG_NFC_SYSCTRL2, p->regs.sysctrl2_read);

	if (write)
		nfc_write32(REG_NFC_CFG0, p->regs.cfg0_prog);
	else
		nfc_write32(REG_NFC_CFG0, p->regs.cfg0_read);

	nfc_write32(REG_NFC_CFG1, p->regs.cfg1);

	nfc_write32(REG_NFC_FTUNE, p->regs.ftune);
}

void nt72_nand_nfc_prepare_rw_dma_regs(struct nt72_nand_info *info, int column,
				       int page_addr, int write)
{
	u32 _reg;
	struct nt72_nand_params *p = &(info->nand_params);

	nfc_write32(REG_NFC_ROWADR, page_addr);

	nfc_write32(REG_NFC_COLADR, 0);

	nfc_write32(REG_NFC_SYSADR, info->dma_data_buff_pa);

	nfc_write32(REG_NFC_XTRA_ADDR, info->dma_data_buff_pa + p->page_size);

	if (write) {
		nfc_write32(REG_NFC_RNDACC,
			    NFC_RNDACC_COL_ADDR(p->page_size) |
			    NFC_RNDACC_CODE0(0x85));
	} else {
		nfc_write32(REG_NFC_RNDACC,
			    NFC_RNDACC_COL_ADDR(p->page_size) |
			    NFC_RNDACC_CODE0(0x05) | NFC_RNDACC_CODE1(0xe0));
	}

	_reg = NFC_XFER_DATA_SEL_DMA |
	       NFC_XFER_TRANSFER_BYTE_COUNT(p->page_size) |
	       NFC_XFER_ECC_RESET |
	       NFC_XFER_ECC_ENABLE |
	       /* only after REG_NFC_RNDACC set */
	       NFC_XFER_RAND_DATA_ACC_CMD |
	       NFC_XFER_CHIP_SEL(info->target_chip);
	if (p->ecc.extra_size == 16)
		_reg |= NFC_XFER_XTRA_DATA_COUNT_16;
	else
		nfc_err("nfc_err: NFC_XFER extra size error\n");
	if (write)
		_reg |= NFC_XFER_WRITE | NFC_XFER_RAND_DATA_ACC_CMD_CYCLE_ONE |
			NFC_XFER_KEEP_CE;
	else
		_reg |= NFC_XFER_START_WAIT_RDY |
			NFC_XFER_RAND_DATA_ACC_CMD_CYCLE_TWO;
	nfc_write32(REG_NFC_XFER, _reg);
}

void nt72_nand_nfc_send_piocmd(struct nt72_nand_info *info, const int busy_wait,
			       const u32 cmd)
{
	nt72_nand_ir_init(info, busy_wait,
			  NFC_INTCTR_ENABLE |
			  NFC_INTCTR_CMD_COMPLETE |
			  NFC_INTCTR_ERR_TIMEOUT);

	nfc_write32(REG_NFC_CMD, cmd);
}

void nt72_nand_nfc_send_rwcmd(struct nt72_nand_info *info, int column,
			      int page_addr, int write)
{
	nt72_nand_ir_init(info, 0,
			  NFC_INTCTR_ENABLE |
			  NFC_INTCTR_FRUN_COMPLETE |
			  NFC_INTCTR_ERR_TIMEOUT);

	if (write) {
		/*
		 * During write, we write DMACTR after CMD to prevent DMA
		 * misbehaving (lose extra data).
		 * It can be easily observed by running mtd_oobtest.ko.
		 * */
		nfc_write32(REG_NFC_CMD,
			    NFC_CMD_CODE0(0x80) |
			    NFC_CMD_CYCLE_ONE |
			    NFC_CMD_ADDR_CYCLE_COL_OTHER |
			    NFC_CMD_DATA_XFER |
			    NFC_CMD_WP_KEEP |
			    NFC_CMD_WP_NEG |
			    NFC_CMD_CHIP_SEL(info->target_chip));
		nfc_write32(REG_NFC_DMACTR,
			    NFC_DMACTR_TRAN_BYTE(info->nand_params.page_size));
	} else {
		nfc_write32(REG_NFC_DMACTR,
			    NFC_DMACTR_WRITE_TO_DRAM |
			    NFC_DMACTR_TRAN_BYTE(info->nand_params.page_size));
		nfc_write32(REG_NFC_CMD,
			    NFC_CMD_CODE0(0x00) |
			    NFC_CMD_CODE1(0x30) |
			    NFC_CMD_CYCLE_TWO |
			    NFC_CMD_END_WAIT_READY |
			    NFC_CMD_ADDR_CYCLE_COL_ROW |
			    NFC_CMD_DATA_XFER |
			    NFC_CMD_WP_KEEP |
			    NFC_CMD_CHIP_SEL(info->target_chip));
	}
}

void nt72_nand_nfc_get_ecc_result(struct nt72_nand_info *info)
{
	int i;
	u32 cnt0, cnt1;
	struct nt72_nand_ecc_result *ecc_result = NULL;

	ecc_result = &(info->ecc_result);
	memset(ecc_result, 0, sizeof(struct nt72_nand_ecc_result));

	cnt0 = nfc_read32(REG_NFC_ERR_CNT0);
	cnt1 = nfc_read32(REG_NFC_ERR_CNT1);

	for (i = 0; i < (info->nand_params.nr_subpage / 2); i++) {
		/* REG_NFC_ERR_CNT0: subpage: 0/2/4/6 */
		if ((cnt0 & NFC_ERR_CNTX_IS_BLANK(i)) == 0) {
			if (info->nand_params.ecc.type == BCH) {
				ecc_result->_err_count[i * 2] =
					(cnt0 >> (5 * i)) & 0x1F;
				if (((cnt0 >> (24 + i * 2)) & 0x3) == 0x2)
					ecc_result->_uncorrectable[i * 2] = 1;
			} else if (info->nand_params.ecc.type == RS) {
				ecc_result->_err_count[i * 2] =
					(cnt0 >> (5 * i)) & 0x07;
				if (((cnt0 >> (5 * i + 3)) & 0x3) == 0x2)
					ecc_result->_uncorrectable[i * 2] = 1;
			}
			if (ecc_result->_uncorrectable[i * 2])
				ecc_result->uncorrectable = 1;
			else
				ecc_result->err_count +=
					ecc_result->_err_count[i * 2];
		} else {
			ecc_result->_blank[i * 2] = 1;
			ecc_result->blank++;
		}
		/* REG_NFC_ERR_CNT1: subpage: 1/3/5/7 */
		if ((cnt1 & NFC_ERR_CNTX_IS_BLANK(i)) == 0) {
			if (info->nand_params.ecc.type == BCH) {
				ecc_result->_err_count[i * 2 + 1] =
					(cnt1 >> (5 * i)) & 0x1F;
				if (((cnt1 >> (24 + i * 2)) & 0x3) == 0x2)
					ecc_result->_uncorrectable[i * 2 + 1] =
						1;
			} else if (info->nand_params.ecc.type == BCH) {
				ecc_result->_err_count[i * 2 + 1] =
					(cnt1 >> (5 * i)) & 0x07;
				if (((cnt1 >> (5 * i + 3)) & 0x3) == 0x2)
					ecc_result->_uncorrectable[i * 2 + 1] =
						1;
			}
			if (ecc_result->_uncorrectable[i * 2 + 1])
				ecc_result->uncorrectable = 1;
			else
				ecc_result->err_count +=
					ecc_result->_err_count[i * 2 + 1];
		} else {
			ecc_result->_blank[i * 2 + 1] = 1;
			ecc_result->blank++;
		}
	}
}

void nt72_nand_nfc_read_id(struct nt72_nand_info *info, int column, u32 *id,
			   int probe)
{
	u32 _id = 0;
	u32 _id_ext = 0;
	int read_onfi = column == 0x20;

	nt72_nand_kapi_lock_nfc(info);
	nt72_nand_cmd_history_add("read_id");
	nt72_nand_nfc_wait_nfc_ready("read_id");

	if (read_onfi) {
		/*
		 * Linux may read ONFI identifier but we don't support it,
		 * so skip it to boot faster.
		 * */
		goto skip_read_onfi;
	}

	if (!probe) {
		/* During probe, we have no init values for registers */
		nt72_nand_nfc_prepare_pio_regs(info);
	}

	if (read_onfi) {
		nfc_write32(REG_NFC_XFER,
			    NFC_XFER_TRANSFER_BYTE_COUNT(4) |
			    NFC_XFER_CHIP_SEL(info->target_chip));
	} else {
		nfc_write32(REG_NFC_XFER,
			    NFC_XFER_TRANSFER_BYTE_COUNT(5) |
			    NFC_XFER_CHIP_SEL(info->target_chip));
	}

	nt72_nand_nfc_dump_ctrl_regs("read_id");

	nt72_nand_cmd_history_save_start_timestamp();
	nt72_nand_nfc_send_piocmd(info, 1, NFC_CMD_CODE0(0x90) |
				  NFC_CMD_CYCLE_ONE |
				  NFC_CMD_ADDR_CYCLE_DUMMY |
				  NFC_CMD_DATA_XFER |
				  NFC_CMD_CHIP_SEL(info->target_chip));
	nt72_nand_ir_force_busy_wait(info, "read_id",
				     NFC_INTSTS_DATA_XFER_TO_SRAM_COMPLETE |
				     NFC_INTSTS_ERRORS | NFC_INTSTS_TIMEOUTS);
	nt72_nand_cmd_history_save_end_timestamp();
	nt72_nand_nfc_wait_nfc_ready("read_id_after");

	info->retcode =
		nt72_nand_nfc_check_int_status("read_id", info, 0, 0, 0);
	if (unlikely(info->retcode != ERR_NONE))
		goto has_error;

	_id = nfc_read32(REG_NFC_DATA);
	if (!read_onfi)
		_id_ext = nfc_read32(REG_NFC_DATA);

skip_read_onfi:

	if (id)
		*id = _id;
	else {
		u8 id_buff[5];

		id_buff[0] = _id & 0xff;
		id_buff[1] = (_id >> 8) & 0xff;
		id_buff[2] = (_id >> 16) & 0xff;
		id_buff[3] = (_id >> 24) & 0xff;
		id_buff[4] = read_onfi ? 0 : _id_ext & 0xff;
		info->buf_start = 0;
		info->buf_count = read_onfi ? 4 : 5;
		memcpy(info->data_buff + info->buf_start, id_buff,
		       info->buf_count);
	}

	if (!probe)
		nt72_nand_nfc_ecc_workaround(info);

	nt72_nand_kapi_unlock_nfc(info);
	return;
has_error:
	nt72_nand_kapi_unlock_nfc(info);
	nfc_err("nfc_err: read_id failed: %d\n", info->retcode);
}

void nt72_nand_nfc_read_status(struct nt72_nand_info *info, uint32_t *status)
{
	uint8_t st_buff[2];

	nt72_nand_kapi_lock_nfc(info);
	nt72_nand_cmd_history_add("readsts");
	nt72_nand_nfc_wait_nfc_ready("read_status");

	nt72_nand_nfc_prepare_pio_regs(info);

	nfc_write32(REG_NFC_XFER, NFC_XFER_CHIP_SEL(info->target_chip) |
		    NFC_XFER_TRANSFER_BYTE_COUNT(1));

	nt72_nand_nfc_dump_ctrl_regs("read_status");

	nt72_nand_cmd_history_save_start_timestamp();
	nt72_nand_nfc_send_piocmd(info, 1, NFC_CMD_CHIP_SEL(info->target_chip) |
				  NFC_CMD_DATA_XFER | NFC_CMD_CYCLE_ONE |
				  NFC_CMD_WP_NEG | NFC_CMD_CODE0(0x70));
	nt72_nand_ir_force_busy_wait(info, "read_status",
				     NFC_INTSTS_CMD_COMPLETE |
				     NFC_INTSTS_ERRORS | NFC_INTSTS_TIMEOUTS);
	nt72_nand_cmd_history_save_end_timestamp();
	nt72_nand_nfc_wait_nfc_ready("read_status_after");

	info->retcode =
		nt72_nand_nfc_check_int_status("read_status", info, 0, 0, 0);
	if (unlikely(info->retcode != ERR_NONE))
		goto has_error;

	st_buff[0] = nfc_read32(REG_NFC_DATA) & 0xFF;

	if (status)
		*status = st_buff[0];
	else {
		info->buf_start = 0;
		info->buf_count = 1;
		memcpy(info->data_buff + info->buf_start, st_buff,
		       info->buf_count);
	}

	nt72_nand_nfc_ecc_workaround(info);

	nt72_nand_kapi_unlock_nfc(info);
	return;
has_error:
	nfc_err("nfc_err: read_status failed: %d\n", info->retcode);
	nt72_nand_kapi_unlock_nfc(info);
}

void nt72_nand_nfc_read_page(struct nt72_nand_info *info, int column,
			     int page_addr, int use_ecc, int oob_only)
{
	int retry_count = READ_RETRY_COUNT;
	/* means we are booting and scanning for bad blocks */
	int boot_scan_bad_block = info->nand_scanning && oob_only;

	nt72_nand_kapi_lock_nfc(info);

do_retry:
	if (likely(retry_count == READ_RETRY_COUNT)) {
		if (unlikely(oob_only))
			nt72_nand_cmd_history_add("roob");
		else
			nt72_nand_cmd_history_add("read");
	} else {
		if (unlikely(oob_only))
			nt72_nand_cmd_history_add("roob_r");
		else
			nt72_nand_cmd_history_add("read_r");
	}
	nt72_nand_nfc_ecc_workaround(info);
	nt72_nand_nfc_wait_nfc_ready("read_page");

	nt72_nand_nfc_prepare_rw_regs(info, 0);
	nt72_nand_nfc_prepare_rw_dma_regs(info, column, page_addr, 0);

	nt72_nand_nfc_dump_ctrl_regs("read_page");

	nt72_nand_cmd_history_save_start_timestamp();
	nt72_nand_nfc_send_rwcmd(info, column, page_addr, 0);
	nt72_nand_ir_wait(info, "read_page",
			  NFC_INTSTS_FULL_PAGE_DMA_TERMINATED |
			  NFC_INTSTS_ERRORS | NFC_INTSTS_TIMEOUTS);
	nt72_nand_cmd_history_save_end_timestamp();
	nt72_nand_nfc_wait_nfc_ready("read_page_after");

	nt72_nand_nfc_get_ecc_result(info);

	info->retcode =
		nt72_nand_nfc_check_int_status("read_page", info, page_addr, 1,
					       oob_only);

	/*
	 * If we are scanning bad blocks during boot, DON'T:
	 *	1. Fill blank page
	 *	2. Retry when failed
	 */
	if (likely(!boot_scan_bad_block)) {
		if (unlikely(info->retcode != ERR_NONE))
			goto has_error;
		if (unlikely(info->ecc_result.blank)) {
#ifdef FILL_BLANK_PAGE
			nt72_nand_nfc_fill_blank_page(info, page_addr,
						      oob_only);
			if (unlikely(info->ecc_result.blank < 4))
				nfc_msg("nfc_msg: blank subpage(s): %d\n",
					info->ecc_result.blank);
#endif
		} else
			nt72_nand_selftest_fake_bitflip(info);
	}

	if (unlikely(retry_count < READ_RETRY_COUNT)) {
		nfc_msg("nfc: retry success, dump good page\n");
		nt72_nand_nfc_dump_ecc(info);
		nt72_nand_nfc_dump_regs(info, page_addr);
		nt72_nand_nfc_dump_data(info, page_addr);
		nt72_nand_bus_sniffer_dump();
	}

	nt72_nand_kapi_unlock_nfc(info);
	return;

has_error:
	if (retry_count > 0) {
		nfc_msg("nfc: read_page retry, count = %d\n",
			retry_count);
		retry_count--;
		goto do_retry;
	}
	nfc_err("nfc_err: read_page failed: %d\n", info->retcode);
	nt72_nand_kapi_unlock_nfc(info);
}

void nt72_nand_nfc_erase_block(struct nt72_nand_info *info, int page_addr)
{
	nt72_nand_kapi_lock_nfc(info);

	nt72_nand_cmd_history_add("erase");
	nt72_nand_nfc_wait_nfc_ready("erase_block");

	nt72_nand_nfc_prepare_pio_regs(info);

	nfc_write32(REG_NFC_ROWADR, page_addr);
	nfc_write32(REG_NFC_COLADR, 0x00);

	nt72_nand_nfc_dump_ctrl_regs("eraseblock");

	nt72_nand_cmd_history_save_start_timestamp();
	nt72_nand_nfc_send_piocmd(info, 0, NFC_CMD_CHIP_SEL(info->target_chip) |
				  NFC_CMD_WP_NEG |
				  NFC_CMD_ADDR_CYCLE_ROW |
				  NFC_CMD_END_WAIT_READY |
				  NFC_CMD_CYCLE_TWO |
				  NFC_CMD_CODE1(0xd0) |
				  NFC_CMD_CODE0(0x60));
	nt72_nand_ir_wait(info, "erase_block",
			  NFC_INTSTS_ERRORS |
			  NFC_INTSTS_TIMEOUTS |
			  NFC_INTSTS_CMD_COMPLETE);
	nt72_nand_cmd_history_save_end_timestamp();
	nt72_nand_nfc_wait_nfc_ready("erase_block_after");

	info->retcode =
		nt72_nand_nfc_check_int_status("erase_block", info, 0, 0, 0);
	if (unlikely(info->retcode != ERR_NONE))
		goto has_error;

	nt72_nand_nfc_ecc_workaround(info);

	nt72_nand_kapi_unlock_nfc(info);
	return;
has_error:
	nfc_err("nfc_err: erase_block failed: %d\n", info->retcode);
	info->retcode = ERR_EERROR;
	nt72_nand_kapi_unlock_nfc(info);
}

void nt72_nand_nfc_write_page(struct nt72_nand_info *info,
			      int column, int page_addr)
{
	nt72_nand_kapi_lock_nfc(info);
	nt72_nand_cmd_history_add("prog");

	nt72_nand_nfc_ecc_workaround(info);
	nt72_nand_nfc_wait_nfc_ready("write_page");

	nt72_nand_nfc_prepare_rw_regs(info, 1);
	nt72_nand_nfc_prepare_rw_dma_regs(info, column, page_addr, 1);

	nt72_nand_nfc_dump_ctrl_regs("write_page");

	nt72_nand_cmd_history_save_start_timestamp();
	nt72_nand_nfc_send_rwcmd(info, column, page_addr, 1);
	nt72_nand_ir_wait(info, "write_page",
			  NFC_INTSTS_FULL_PAGE_DMA_TERMINATED |
			  NFC_INTSTS_ERRORS |
			  NFC_INTSTS_TIMEOUTS);
	nt72_nand_cmd_history_save_end_timestamp();
	nt72_nand_nfc_wait_nfc_ready("write_page_after");

	info->retcode =
		nt72_nand_nfc_check_int_status("write_page", info, page_addr,
					       0, 0);
	if (unlikely(info->retcode != ERR_NONE))
		goto has_error;

	nt72_nand_kapi_unlock_nfc(info);
	return;
has_error:
	nfc_err("nfc_err: write_page failed: %d\n", info->retcode);
	info->retcode = ERR_WERROR;
	nt72_nand_kapi_unlock_nfc(info);
}
