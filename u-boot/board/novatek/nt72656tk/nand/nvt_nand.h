/*
 * Copyright (C) 2012 Novatek Microelectronics Corp. All Rights Reserved.
 *
 * NAND flash controller driver for the Novatek DR78326ST(NT78320) board.
 *
 * See file CREDITS for list of people who contributed to this
 * project.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.	 See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 * MA 02111-1307 USA
 */

#ifndef __NVT_NAND_H
#define __NVT_NAND_H


//#include "nvt_nand_flash_list.h"

//#include "common.h"
#include <linux/mtd/bbm.h>
#include <linux/mtd/nand.h>
#include "nvt_nand_flash_list.h"

#define NAND_DEFAULT_OOBSIZE  	  (64*8)
#define NAND_DEFAULT_PAGESIZE	(1024*8)
#define DEFAULT_CACHE_LINE_SZIE       32

typedef struct NVT_NAND_INFO 
{
    struct nand_chip    nand_chip;

    struct platform_device   *pdev;
    NAND_FLASH_PARAMETER *flash_info;

    struct clk      *clk;
    void __iomem        *mmio_base;

    unsigned int        buf_start;
    unsigned int        buf_count;


    unsigned char       *data_buff;
    dma_addr_t      data_buff_phys;
    size_t          data_buff_size;
    dma_addr_t      data_desc_addr;

    /* saved column/page_addr during CMD_SEQIN */
    int         seqin_column;
    int         seqin_page_addr;

    /* relate to the command */
    unsigned int        state;

    int         use_ecc;    /* use HW ECC ? */
    int         use_dma;    /* use DMA ? */

    size_t          data_size;  /* data size in FIFO */
    int             retcode;

    unsigned int        ecc_corrected;  /* ecc corrected bit for regsiter */


    /* calculated from NAND_FLASH_PARAMETER data */
    size_t      oob_size;
    size_t      read_id_bytes;

    unsigned int    col_addr_cycles;
    unsigned int    row_addr_cycles;
}NVT_NAND_INFO, *PNVT_NAND_INFO;

typedef struct NVT_NAND_PLATFORM_DATA
{//nt78320S_nand_platform_data {

    /* the data flash bus is shared between the Static Memory
     * Controller and the Data Flash Controller,  the arbiter
     * controls the ownership of the bus
     */
    int enable_arbiter;

    /* allow platform code to keep OBM/bootloader defined NFC config */
    int keep_config;

    const struct mtd_partition      *parts;
    unsigned int                nr_parts;

    const PNAND_FLASH_PARAMETER   flash;
    size_t                  num_flash;
}NVT_NAND_PLATFORM_DATA, *PNVT_NAND_PLATFORM_DATA;


static int use_dma = 1;

int NVT_nand_wait_func(struct mtd_info *mtd, struct nand_chip *this);
void NVT_nand_select_chip(struct mtd_info *mtd, int chip);
int NVT_nand_dev_ready(struct mtd_info *mtd);
void NVT_nand_cmdfunc(struct mtd_info *mtd, unsigned command, int column, int page_addr);
uint8_t NVT_nand_read_byte(struct mtd_info *mtd);
u16 NVT_nand_read_word(struct mtd_info *mtd);
void NVT_nand_read_buf(struct mtd_info *mtd, uint8_t *buf, int len);
int NVT_nand_verify_buf(struct mtd_info *mtd, const uint8_t *buf, int len);
void NVT_nand_write_buf(struct mtd_info *mtd, const uint8_t *buf, int len);
int NVT_nand_read_page_hwecc(struct mtd_info *mtd, struct nand_chip *chip, uint8_t *buf, int oob_required, int page);
int NVT_nand_write_page_hwecc(struct mtd_info *mtd, struct nand_chip *chip, const uint8_t *buf, int oob_required);
void NVT_nand_ecc_hwctl(struct mtd_info *mtd, int mode);
int NVT_nand_ecc_calculate(struct mtd_info *mtd, const uint8_t *dat, uint8_t *ecc_code);
int NVT_nand_ecc_correct(struct mtd_info *mtd, uint8_t *dat, uint8_t *read_ecc, uint8_t *calc_ecc);





#endif /* __NVT_NAND_H */
