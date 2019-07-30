/*
 * drivers/mtd/nand/nt72656_nand.c
 *
 * Copyright 2014 Novatek Corporation
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */
#ifdef __NVT_NAND_KERNEL__
#include "nvt_nand_kernelDrv.h"
#include "nvt_nand.h"
#endif

#ifdef __NVT_NAND_UBOOT__
#include "nvt_nand_uboot.h"
#include "nvt_nand.h"
#endif

enum {
    STATE_READY = 0,
    STATE_CMD_HANDLE,
    STATE_DMA_READING,
    STATE_DMA_WRITING,
    STATE_DMA_DONE,
    STATE_PIO_READING,
    STATE_PIO_WRITING,
};

int NVT_nand_verify_buf(struct mtd_info *mtd,
        const uint8_t *buf, int len)
{
    return 0;
}

void NVT_nand_select_chip(struct mtd_info *mtd, int chip)
{
    return;
}

int NVT_nand_read_page_hwecc(struct mtd_info *mtd, struct nand_chip *chip,
                                uint8_t *buf, int oob_required, int page)
{
        PNVT_NAND_INFO info =  ((struct nand_chip*)mtd->priv)->priv;

        memcpy(buf,info->data_buff,info->flash_info->page_size);
        memcpy(info->nand_chip.oob_poi,info->data_buff + info->flash_info->page_size,
                info->flash_info->oob_size);

        /* printk("%s retcode %d\n", __func__, info->retcode); */
        if (info->retcode == ERR_ECC_FAIL) {
        mtd->ecc_stats.failed++;
                return -EBADMSG;
        } else if (info->retcode == ERR_ECC_UNCLEAN) {
        mtd->ecc_stats.corrected += info->ecc_corrected;
                return -EUCLEAN;
        } else {
                return 0;
        }

}

int NVT_nand_write_page_hwecc(struct mtd_info *mtd, struct nand_chip *chip,
                                  const uint8_t *buf, int oob_required)
{
    /*this function will write page data and oob into nand*/
    PNVT_NAND_INFO info =  ((struct nand_chip*)mtd->priv)->priv;

    memcpy(info->data_buff,buf,info->flash_info->page_size);
	return 0;

}

int NVT_nand_dev_ready(struct mtd_info *mtd)
{
//    struct NVT_nand_info *info = mtd->priv;
    //Do nothing
    NVT_NAND_DBG("DEBUG in %s\n", __func__);
    return 1;
}
int NVT_nand_wait_func(struct mtd_info *mtd, struct nand_chip *this)
{
    PNVT_NAND_INFO pInfo = ((struct nand_chip*)mtd->priv)->priv;
    uint32_t status = 0;

    NVT_nand_nfc_WaitFlashReady(pInfo->flash_info);
    NVT_nand_nfc_ReadStatus(pInfo->flash_info, (pInfo->data_buff + pInfo->buf_start), &status);
    NVT_NAND_DBG("DEBUG in %s status 0x%x\n", __func__, status);

    return status;
}

void NVT_nand_cmdfunc(struct mtd_info *mtd, unsigned command, int column, int page_addr)
{
    PNVT_NAND_INFO pInfo =  ((struct nand_chip*)mtd->priv)->priv;
//    const struct NVT_nand_flash *flash_info = info->flash_info;
//    const struct NVT_nand_cmdset *cmdset = flash_info->cmdset;
//    int ret;
    unsigned char *ptr;

    pInfo->use_dma = (use_dma) ? 1 : 0;
    pInfo->use_ecc = 0;
    pInfo->data_size = 0;
    pInfo->state = STATE_READY;

    NVT_NAND_DBG("DEBUG in %s command 0x%x\n", __func__, command);
    switch (command) {
    case NAND_CMD_READOOB:

        if (((unsigned long)(pInfo->data_buff)) % DEFAULT_CACHE_LINE_SZIE)
        {
            printk(KERN_ERR "NVT_nand_cmdfunc : case NAND_CMD_READOOB : is not DEFAULT_CACHE_LINE_SZIE alignment!\n");
        }


        /* Read a page and set buff forom writesize */
        pInfo->buf_count = mtd->writesize + mtd->oobsize;
        pInfo->buf_start = mtd->writesize + column;
        memset(pInfo->data_buff, 0xFF, pInfo->buf_count);

        NVT_NAND_DBG("pInfo->buf_start %ud\n", pInfo->buf_start);
        ptr = pInfo->data_buff + pInfo->buf_start;

        if(NVT_nand_nfc_ReadOOB(pInfo->flash_info, pInfo->data_buff_phys, page_addr, NULL) != NAND_ERROR_OK)
            pInfo->retcode = ERR_NONE;
		
        NVT_NAND_DBG("DEBUG A SPARE 0x%02x 0x%02x 0x%02x 0x%02x 0x%02x 0x%02x 0x%02x\n", ptr[0], ptr[1], ptr[2], ptr[3], ptr[4], ptr[5], ptr[6]);

        break;

    case NAND_CMD_READ0:
        if (((unsigned long)(pInfo->data_buff)) % DEFAULT_CACHE_LINE_SZIE)
        {
            printk(KERN_ERR "NVT_nand_cmdfunc : case NAND_CMD_READ0 : is not DEFAULT_CACHE_LINE_SZIE alignment!\n");
        }

        pInfo->use_ecc = 1;
        pInfo->retcode = ERR_NONE;
        pInfo->buf_start = column;
        pInfo->buf_count = mtd->writesize + mtd->oobsize;
        //memset(pInfo->data_buff, 0xFF, pInfo->buf_count);
		NVT_nand_invalidata_buf(pInfo);
        if(NVT_nand_nfc_ReadPage(pInfo->flash_info, pInfo->data_buff_phys, page_addr, &pInfo->ecc_corrected) == NAND_ERROR_UNCORRECTABLE_ECC)
			pInfo->retcode = ERR_ECC_FAIL;

        break;
    case NAND_CMD_SEQIN:
        pInfo->buf_start = column;
        pInfo->buf_count = mtd->writesize + mtd->oobsize;
        memset(pInfo->data_buff, 0xff, pInfo->buf_count);

        /* save column/page_addr for next CMD_PAGEPROG */
        pInfo->seqin_column = column;
        pInfo->seqin_page_addr = page_addr;
        break;
    case NAND_CMD_PAGEPROG:
        if (((unsigned long)(pInfo->data_buff)) % DEFAULT_CACHE_LINE_SZIE)
        {
            printk(KERN_ERR "NVT_nand_cmdfunc : case NAND_CMD_PAGEPROG : is not DEFAULT_CACHE_LINE_SZIE alignment!\n");
        }

		NVT_nand_flush_data_buf(pInfo);
        NVT_nand_nfc_WritePage(pInfo->flash_info, pInfo->data_buff_phys, pInfo->seqin_page_addr);
        break;
    case NAND_CMD_ERASE1:
        NVT_nand_nfc_EraseBlock(pInfo->flash_info, page_addr);
        break;
    case NAND_CMD_ERASE2:
        break;
    case NAND_CMD_READID:
        pInfo->buf_start = 0;
        pInfo->buf_count = 8;
        NVT_nand_nfc_ReadId(pInfo->flash_info, (pInfo->data_buff + pInfo->buf_start), NULL, NULL);
        break;
    case NAND_CMD_STATUS:
        pInfo->buf_start = 0;
        pInfo->buf_count = 1;
        NVT_nand_nfc_ReadStatus(pInfo->flash_info, pInfo->data_buff + pInfo->buf_start, NULL);
        break;
    case NAND_CMD_RESET:
        break;
    default:
        NVT_NAND_DBG(KERN_ERR "non-supported command.\n");
        break;
    }

    if (pInfo->retcode == ERR_DBERR) {
        NVT_NAND_DBG(KERN_ERR "double bit error @ page %08x\n", page_addr);
        pInfo->retcode = ERR_NONE;
    }


}

uint8_t NVT_nand_read_byte(struct mtd_info *mtd)
{
    PNVT_NAND_INFO info =  ((struct nand_chip*)mtd->priv)->priv;
    char retval = 0xFF;

    if (info->buf_start < info->buf_count)
        /* Has just send a new command? */
        retval = info->data_buff[info->buf_start++];

    return retval;
}

u16 NVT_nand_read_word(struct mtd_info *mtd)
{
    PNVT_NAND_INFO info =  ((struct nand_chip*)mtd->priv)->priv;
    u16 retval = 0xFFFF;

    if (!(info->buf_start & 0x01) && info->buf_start < info->buf_count) {
        retval = *((u16 *)(info->data_buff+info->buf_start));
        info->buf_start += 2;
    }
    return retval;
}

void NVT_nand_read_buf(struct mtd_info *mtd, uint8_t *buf, int len)
{
    PNVT_NAND_INFO info =  ((struct nand_chip*)mtd->priv)->priv;
    int real_len = min_t(size_t, len, info->buf_count - info->buf_start);


    /*
    if (STOP_NAND_WRITE_FLAG == 1)
    {
        printk(KERN_ERR "DEBUG in %s\n", __func__);
        printk(KERN_ERR "DEBUG buf : 0x%08X, info->data_buff + info->buf_start : 0x%08X, real_len : 0x%08X\n", buf, info->data_buff + info->buf_start, real_len);
    }
    */

    memcpy(buf, info->data_buff + info->buf_start, real_len);
    info->buf_start += real_len;
}

void NVT_nand_write_buf(struct mtd_info *mtd,
        const uint8_t *buf, int len)
{
    PNVT_NAND_INFO info =  ((struct nand_chip*)mtd->priv)->priv;
    int real_len = min_t(size_t, len, info->buf_count - info->buf_start);

    /*
    if (STOP_NAND_WRITE_FLAG == 1)
    {
        printk(KERN_ERR "DEBUG in %s\n", __func__);
        printk(KERN_ERR "DEBUG info->data_buff + info->buf_start : 0x%08X, buf : 0x%08X, real_len : 0x%08X\n", info->data_buff + info->buf_start, buf, real_len);
    }
    */

    memcpy(info->data_buff + info->buf_start, buf, real_len);
    info->buf_start += real_len;
}

void NVT_nand_ecc_hwctl(struct mtd_info *mtd, int mode)
{
    return;
}

int NVT_nand_ecc_calculate(struct mtd_info *mtd,
        const uint8_t *dat, uint8_t *ecc_code)
{
    return 0;
}

int NVT_nand_ecc_correct(struct mtd_info *mtd,
        uint8_t *dat, uint8_t *read_ecc, uint8_t *calc_ecc)
{
//    struct NVT_nand_info *info = mtd->priv;

    return 0;
}
