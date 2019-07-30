#ifndef _NT72_NAND_SPECS_INTERNAL_H_
#define _NT72_NAND_SPECS_INTERNAL_H_

#include <linux/types.h>

struct nt72_nand_spec {
	uint32_t			page_per_block;
	uint32_t			page_size;
	uint32_t			oob_size;
	uint32_t			subpage_size;
	uint32_t			flash_width;
	uint32_t			nfc_width;
	uint32_t			num_blocks;
	uint32_t			chip_id;

	uint32_t			cfg0_read;
	uint32_t			cfg0_prog;
	uint32_t			ftune;
	uint32_t			blank_0xff_threshold;
	uint32_t			blank_non_0xff_threshold;
	uint32_t			bitflip_threshold;
	uint32_t			bbt_options;
};

#endif  /* _NT72_NAND_SPECS_INTERNAL_H_ */
