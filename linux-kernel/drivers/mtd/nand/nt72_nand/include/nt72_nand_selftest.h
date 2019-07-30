#include "nt72_nand_nfc.h"

#ifndef SELFTEST_FAKE_BITFLIP
#define nt72_nand_selftest_fake_bitflip(...)
#else
void nt72_nand_selftest_fake_bitflip(struct nt72_nand_info *info);
#endif

#ifndef SELFTEST_FAKE_BITFLIP_BLANK
#define nt72_nand_selftest_fake_bitflip_blank(...)
#else
void nt72_nand_selftest_fake_bitflip_blank(struct nt72_nand_info *info, int
					   subpage_id);
#endif
