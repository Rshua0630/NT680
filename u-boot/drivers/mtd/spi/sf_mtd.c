/*
*Copyright (C) 2012-2014 Daniel Schwierzeck, daniel.schwierzeck@gmail.com
*
* SPDX-License-Identifier:    GPL-2.0+
*
*/

#include <common.h> 
#include <malloc.h> 
#include <asm/errno.h>
#include <linux/mtd/mtd.h>
#include <spi_flash.h>

#include "sf_internal.h"

static struct mtd_info sf_mtd_info; 
static char sf_mtd_name[8];

static int spi_flash_mtd_erase(struct mtd_info *mtd, struct erase_info *instr)
{
	struct spi_flash *flash = mtd->priv;
	int err;

	instr->state = MTD_ERASING;

	err = spi_flash_erase(flash, instr->addr, instr->len);
	if (err) {
		instr->state = MTD_ERASE_FAILED;
		instr->fail_addr = MTD_FAIL_ADDR_UNKNOWN;
		return -EIO;
	}

	instr->state = MTD_ERASE_DONE;
	mtd_erase_callback(instr);

	return 0;
}

static int spi_flash_mtd_read(struct mtd_info *mtd, loff_t from, size_t len, size_t *retlen, u_char *buf)
{
	struct spi_flash *flash = mtd->priv;
	int err;

	err = spi_flash_read(flash, from, len, buf);
	if (!err)
		*retlen = len;

	return err;
}

static int spi_flash_mtd_write(struct mtd_info *mtd, loff_t to, size_t len,	size_t *retlen, const u_char *buf)
{
	struct spi_flash *flash = mtd->priv;
	int err;

	err = spi_flash_write(flash, to, len, buf);
    if (!err)
        *retlen = len;

    return err;
}

static void spi_flash_mtd_sync(struct mtd_info *mtd)
{
}

static int spi_flash_mtd_number(void) 
{ 
#ifdef CONFIG_SYS_MAX_FLASH_BANKS 
	return CONFIG_SYS_MAX_FLASH_BANKS; 
#else
	return 0; 
#endif 
}

int spi_flash_mtd_register_imp(struct mtd_info *mtd, struct spi_flash *flash) 
{ 

	mtd->type = MTD_NORFLASH;
	mtd->flags = MTD_CAP_NORFLASH;	
	//sf_mtd_info.writesize = 1;

	mtd->_erase = spi_flash_mtd_erase;
	mtd->_read = spi_flash_mtd_read;
	mtd->_write = spi_flash_mtd_write;
	mtd->_sync = spi_flash_mtd_sync;

	mtd->priv = flash;

	/* Only uniform flash devices for now */
	mtd->numeraseregions = 0;

	return add_mtd_device(mtd);
}

void spi_flash_mtd_unregister(void) 
{       
	//del_mtd_device(&sf_mtd_info); 
}

