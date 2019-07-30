/*
 * External Flash table
 *
 * Copyright (C) Novatek Inc. 2017
 *
 */
#include <linux/mtd/mtd.h>

/*
 * {"Flash_name", jedec_id, ext_jedec, sector_size, nr_sectors, rd_cmd, flags},
 *
 * rd_cmd: 
 *       1 bit : 0
 *       2 bits: RD_EXTN
 *       4 bits: RD_FULL
 *
 * flags:
 *       SECT_4K
 *       SECT_32K
 *       E_FSR
 *       WR_QPP
 *  
 *  Read: 2 bits, Write: 1 bit
 * {"MX25L6405D", 0xc22017, 0x0, 64 * 1024,   128, RD_EXTN, 0},
 *
 *  Read: 4 bits, Write: 1 bit
 * {"MX25L12805", 0xc22018, 0x0, 64 * 1024,   256, RD_FULL, 0},
 *
 *  Read: 4 bits, Write: 4 bit
 * {"MX25L12855E",0xc22618, 0x0, 64 * 1024,   256, RD_FULL, WR_QPP},
*/

const struct spi_flash_params ext_spi_flash_params_table[] = {
};

