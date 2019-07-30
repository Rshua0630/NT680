#include "include/nt72_nand_io.h"
#include "include/nt72_nand_kapi.h"
#include "include/nt72_nand_selftest.h"

#ifdef SELFTEST_FAKE_BITFLIP
void nt72_nand_selftest_fake_bitflip(struct nt72_nand_info *info)
{
	/*
	 * At here, we know that:
	 * 1. Page is read without errors.
	 * 2. This page is not blank.
	 * so, increase the err_count to make MTD believe we have bitflips
	 */
	struct nt72_nand_params *p = &(info->nand_params);

	if (info->ecc_result.err_count < p->bitflip_threshold)
		info->ecc_result.err_count = p->bitflip_threshold + 1;
}
#endif

#ifdef SELFTEST_FAKE_BITFLIP_BLANK
void nt72_nand_selftest_fake_bitflip_blank(struct nt72_nand_info *info,
					   int subpage_id)
{
	/*
	 * At here, we know this sub-page is blank and full of 0xFF, so just
	 * flip some bits to zeros.
	 */
	struct nt72_nand_params *p = &(info->nand_params);

	u8 *data = (u8 *)(info->data_buff + (p->subpage_size * subpage_id));
	u8 *extra = (u8 *)(info->data_buff + p->page_size + (p->suboob_size *
							     subpage_id));

	if (likely(p->blank_non_0xff_threshold >= p->nr_subpage)) {
		/* flip some bytes of the main data */
		data[nt72_nand_kapi_rand32() % p->subpage_size] = 0xFE;
		data[nt72_nand_kapi_rand32() % p->subpage_size] = 0xFE;
		data[nt72_nand_kapi_rand32() % p->subpage_size] = 0xFE;
		/* flip one byte of the extra data */
		extra[nt72_nand_kapi_rand32() % p->suboob_size] = 0xFE;
	} else
		nfc_err("nfc_err: bitflip_blank failed\n");
}
#endif
