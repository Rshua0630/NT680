/*
 *  board/nvt/evb668/nt72668_spi_partition.c
 *
 *  Author:	Alvin lin
 *  Created:	Dec 16, 2011
 *  Copyright:	Novatek Inc.
 *
 */
#include <common.h> 
#include <spi_flash.h>
#include "nt72668_spi_partition.h"

#ifndef CONFIG_SF_DEFAULT_SPEED
#define CONFIG_SF_DEFAULT_SPEED	1200000
#endif
#ifndef CONFIG_SF_DEFAULT_MODE
#define CONFIG_SF_DEFAULT_MODE		SPI_MODE_2
#endif


static struct spi_flash *board_flash;
static unsigned int current_blksz = SPI_BLKSZ_MAX;

#if defined(EVB_SPI8)
static unsigned int spi_partiton[SPI_BLKSZ_MAX][SPI_PARTI_MAX] = {
    ///*sect size       stbc        stbcdata              uboot     kr      fs      apdat*/
    //4096,         16,         48,         192,            512,        2816,   512
    /*sect size     stbc        stbcdata              uboot     kr      fs      apdat*/
    //65536,            1,          3,          12,         32,     176,        32
    /* Debug setting */
    //65536,            1,          3,          5,         32,     84,        3
    {32768,            4,          7,          10,         36,     196,        6},
    {65536,            1,          3,          5,         18,     98,        3},
};

#elif defined(TK_SPI_8M)
static unsigned int spi_partiton[SPI_BLKSZ_MAX][SPI_PARTI_MAX] = {
	/*sect size		stbc		fs, 		uboot			kr		apdat 		board*/
	{32768,			1,			10, 		10,			38,		16,			180,		1},
	{32768,			1,			10, 		10,			38,		16,			180,		1},
};

#else
static unsigned int spi_partiton[SPI_BLKSZ_MAX][SPI_PARTI_MAX] = {
	///*sect size		stbc		stbcdata		      uboot		kr		fs		apdat*/
	//4096,			16,			48,			192,			512,		2816,	512
	/*sect size		stbc		stbcdata		      uboot		kr		fs		apdat*/
	{32768,			4,			7,			24,			64,		352,		64},
	{65536,			1,			3,			12,			32,		176,		32}
};
#endif

static void nt72668_spi_flash_init(void)
{
	if(board_flash == NULL) {
		board_flash = spi_flash_probe(0, 0, CONFIG_SF_DEFAULT_SPEED, CONFIG_SF_DEFAULT_MODE);
		if(board_flash) {
			current_blksz = SPI_BLKSZ_32K;
		}
	}
}

unsigned int spi_calc_partition_addr(SPI_PARTITION partition)
{
	unsigned int addr = 0,idx;

	nt72668_spi_flash_init();

	if (board_flash == NULL || partition >= SPI_PARTI_MAX)
		return 0;

	for(idx = SPI_STBC;idx < partition;idx++) 
		addr += spi_partiton[current_blksz][idx];

	addr *= spi_partiton[current_blksz][SPI_BLK_SIZE];

	return addr;
}

unsigned int spi_calc_partition_size(SPI_PARTITION partition)
{
	nt72668_spi_flash_init();

	if (board_flash == NULL || partition >= SPI_PARTI_MAX)
		return 0;

	return 	(spi_partiton[current_blksz][SPI_BLK_SIZE] * spi_partiton[current_blksz][partition]);
}

unsigned int spi_get_current_sector_size(void)
{
	nt72668_spi_flash_init();

	if(board_flash == NULL)
		return 0;
	else
		return board_flash->sector_size;
}

unsigned int spi_get_flash_size(void)
{
	nt72668_spi_flash_init();

	if(board_flash == NULL)
		return 0;
	else
		return board_flash->size;
}


