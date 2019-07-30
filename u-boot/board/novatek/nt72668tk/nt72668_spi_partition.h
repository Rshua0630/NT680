/*
 *  board/nvt/evb668/nt72668_spi_partition.h
 *
 *  Author:	Alvin lin
 *  Created:	Dec 16, 2011
 *  Copyright:	Novatek Inc.
 *
 */
#ifndef __NT72668_SPI_PARTITION_H__
#define __NT72668_SPI_PARTITION_H__

#ifndef UBOOT_FNAME 
#define UBOOT_FNAME 	"nvt-uboot.bin"
#endif
#ifndef STBC_FNAME 
#define STBC_FNAME      "stbc.bin"
#endif
#ifndef STBCDAT_FNAME 
#define STBCDAT_FNAME   "stbcdat.bin"
#endif
#ifndef KERNEL_FNAME 
#define KERNEL_FNAME   "uImage"
#endif
#ifndef FS_FNAME 
#define FS_FNAME       "rootfs.img"
#endif
#ifndef APDAT_FNAME 
#define APDAT_FNAME 	"apdat.img"
#endif
#ifndef BOARD_FNAME 
#define BOARD_FNAME 	"board.sqfs"
#endif

typedef enum {
	SPI_BLKSZ_32K,
	SPI_BLKSZ_64K,
	SPI_BLKSZ_MAX
}SPI_SECT_SZ;

#if defined(TK_SPI_8M)
typedef enum {
	SPI_BLK_SIZE,
	SPI_STBC,
	SPI_FS,
	SPI_UBOOT,
	SPI_KERNEL,
	SPI_APDAT,
	SPI_BOARD,
	SPI_STBCDAT,
	SPI_LOGO,
	SPI_PARTI_MAX
}SPI_PARTITION;
#else
typedef enum {
	SPI_BLK_SIZE,
	SPI_STBC,
	SPI_STBCDAT,
	SPI_UBOOT,
	SPI_KERNEL,
	SPI_FS,
	SPI_APDAT,
	SPI_LOGO,
	SPI_PARTI_MAX
}SPI_PARTITION;
#endif

unsigned int spi_calc_partition_addr(SPI_PARTITION partition);
unsigned int spi_calc_partition_size(SPI_PARTITION partition);
unsigned int spi_get_current_sector_size(void);
unsigned int spi_get_flash_size(void);

#endif

