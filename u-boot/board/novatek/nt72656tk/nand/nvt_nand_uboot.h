#ifndef __NVT_NAND_UBOOT_H__
#define __NVT_NAND_UBOOT_H__

#include <linux/compat.h>
#include <linux/types.h>
#include <asm/cache.h>
#include <asm/dma-mapping.h>
#include <asm/errno.h>
#include <common.h>
#include <linux/mtd/nand.h>
#include "nvt_nand_ecc_layout.h"
#include "nvt_nand_nfc.h"
#include "nvt_nand.h"

//#define NVT_NAND_DBG printf
#define NVT_NAND_DBG

int NVT_nand_init_buff(PNVT_NAND_INFO nand_info);
int board_nand_init(struct nand_chip *nand);
void NVT_nand_invalidata_buf(PNVT_NAND_INFO info);
void NVT_nand_flush_data_buf(PNVT_NAND_INFO info);

#endif
