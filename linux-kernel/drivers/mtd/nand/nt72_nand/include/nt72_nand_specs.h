#ifdef _NT72_NAND_SPECS_H_
#error Please include nt72_nand_specs.h only once.
#endif

#ifndef _NT72_NAND_SPECS_H_
#define _NT72_NAND_SPECS_H_

#include <linux/init.h>
#include <linux/mtd/nand.h>
#include "nt72_nand_specs_internal.h"

static struct nt72_nand_spec g_nt72_nand_specs[] __initdata = {
	/* K9F2G08U0A */
	{
		.page_per_block = 64,
		.page_size = 2048,
		.oob_size = 64,
		.subpage_size = 512,
		.flash_width = 8,
		.nfc_width = 8,
		.num_blocks = 2048,
		.chip_id = 0x9510DAEC,
		.cfg0_read = 0x82103023,
		.cfg0_prog = 0x10103023,
		.ftune = 0x0000C0C0,
		.blank_0xff_threshold = 16 - 4,
		.blank_non_0xff_threshold = 4,
		.bitflip_threshold = 3,
		.bbt_options = NAND_BBT_SCAN2NDPAGE,
	},
	/* MXIC MX30LF1G08AA */
	{
		.page_per_block = 64,
		.page_size = 2048,
		.oob_size = 64,
		.subpage_size = 512,
		.flash_width = 8,
		.nfc_width = 8,
		.num_blocks = 1024,
		.chip_id = 0x9580F1C2,
		.cfg0_read = 0x72100129,
		.cfg0_prog = 0x1010011B,
		.ftune = 0x0000C0C0,
		.blank_0xff_threshold = 16 - 4,
		.blank_non_0xff_threshold = 4,
		.bitflip_threshold = 3,
		.bbt_options = NAND_BBT_SCAN2NDPAGE,
	},
	/* Winbond W29N04GV */
	{
		.page_per_block = 64,
		.page_size = 2048,
		.oob_size = 64,
		.subpage_size = 512,
		.flash_width = 8,
		.nfc_width = 8,
		.num_blocks = 4096,
		.chip_id = 0x9590DCEF,
		.cfg0_read = 0x82103023,
		.cfg0_prog = 0x10103023,
		.ftune = 0x0000C0C0,
		.blank_0xff_threshold = 16 - 4,
		.blank_non_0xff_threshold = 4,
		.bitflip_threshold = 3,
		.bbt_options = NAND_BBT_SCAN2NDPAGE,
	},
	/* MXIC MX30LF2G18A */
	{
		.page_per_block = 64,
		.page_size = 2048,
		.oob_size = 64,
		.subpage_size = 512,
		.flash_width = 8,
		.nfc_width = 8,
		.num_blocks = 2048,
		.chip_id = 0x9590DAC2,
		.cfg0_read = 0x82103023,
		.cfg0_prog = 0x10103023,
		.ftune = 0x0000C0C0,
		.blank_0xff_threshold = 16 - 4,
		.blank_non_0xff_threshold = 4,
		.bitflip_threshold = 3,
		.bbt_options = NAND_BBT_SCAN2NDPAGE,
	},
	/* MXIC MX30LF4G18A */
	{
		.page_per_block = 64,
		.page_size = 2048,
		.oob_size = 64,
		.subpage_size = 512,
		.flash_width = 8,
		.nfc_width = 8,
		.num_blocks = 4096,
		.chip_id = 0x9590DCC2,
		.cfg0_read = 0x82103023,
		.cfg0_prog = 0x10103023,
		.ftune = 0x0000C0C0,
		.blank_0xff_threshold = 16 - 4,
		.blank_non_0xff_threshold = 4,
		.bitflip_threshold = 3,
		.bbt_options = NAND_BBT_SCAN2NDPAGE,
	},
};

static const unsigned int g_nr_nt72_nand_specs = ARRAY_SIZE(g_nt72_nand_specs);

#endif /* _NT72_NAND_SPECS_H_ */
