#ifndef _NT72_NAND_NFC_INIT_H_
#define _NT72_NAND_NFC_INIT_H_

#include "nt72_nand_nfc.h"
#include "nt72_nand_specs_internal.h"

void __init nt72_nand_nfc_nand_params_init(struct nt72_nand_info *info,
					   struct nt72_nand_spec *f) __init;
void __init nt72_nand_nfc_nand_params_print(struct nt72_nand_info *info) __init;

#ifndef SELFTEST_NAND_PARAMS
#define nt72_nand_params_selftest(...)
#else
void __init nt72_nand_nfc_nand_params_selftest(struct nt72_nand_info *info)
__init;
#endif

int __init nt72_nand_nfc_detect_flash(struct nt72_nand_info *info,
				      struct nt72_nand_spec **nand_spec) __init;

#endif
