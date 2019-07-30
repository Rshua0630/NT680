/*
 *  board/nvt/evb668/nt72668_spi_fwupdate.c
 *
 *  Author:	Alvin lin
 *  Created:	Dec 16, 2011
 *  Copyright:	Novatek Inc.
 *
 */

#include <common.h>
#include <command.h>
#include "nt72668_spi_partition.h"
#include "nt72668_spi_fwupdate.h"

#define NT72668_SPI_WRITE_DMA_ALIGN		4

static int nt72668_spi_update_bin(SPI_PARTITION partition_name,char*  fname)
{
	char cmd_buf[256] = {0};
	u32 sect_size;
	u32 write_size;
	u32 remain;
	int cmd_ret;
	
	if(spi_calc_partition_size(partition_name) <= 0) {
		printf("Unknown spi partition !\n");
		return -1;
	}

	//load image to ram
	run_command("usb start", 0);
	sprintf(cmd_buf,"fatload usb 0 0x%x %s",CONFIG_FWUP_RAM_START,fname);
	cmd_ret = run_command(cmd_buf, 0);

	if(cmd_ret != 0) {
		printf("load %s from usb fail !\n",fname);
		return -1;
	}

	//get current spi sector size
	sect_size = spi_get_current_sector_size();

	if(sect_size <= 0) {
		printf("Unknown SPI flash sector size !\n");
		return -1;
	}


	//get file size
	write_size = simple_strtoul(getenv("filesize"), NULL, 16);
	
	remain = (write_size % NT72668_SPI_WRITE_DMA_ALIGN);
	
	if(remain) 
		write_size += (NT72668_SPI_WRITE_DMA_ALIGN - remain);
	
	if(write_size > spi_calc_partition_size(partition_name)) {
		printf("Error ! Image size too large !, image size = %d, partition size = %d\n",
			write_size,spi_calc_partition_size(partition_name));
		return -1;
	}


	nt72668_stbc_stay_in_sram();

	//erase spi partition
	sprintf(cmd_buf,"sf erase %x %x",spi_calc_partition_addr(partition_name),spi_calc_partition_size(partition_name));
	cmd_ret = run_command(cmd_buf,0);
	if(cmd_ret == -1) {
		printf("erase spi partition fail !\n");
		goto err_out;
	}
	
	//write image to spi partition
	sprintf(cmd_buf,"sf write %x %x %x",CONFIG_FWUP_RAM_START,
	spi_calc_partition_addr(partition_name),write_size);
	//spi_calc_partition_addr(partition_name),spi_calc_partition_size(partition_name));
	
	cmd_ret = run_command(cmd_buf,0);
	if(cmd_ret == -1) {
		printf("write image to spi partition fail !\n");
		goto err_out;
	}

	//nt72668_stbc_reset();

	return 0;

err_out:	
	//nt72668_stbc_reset();
	return -1;
	
}

int nt72668_spi_burn_bin(SPI_PARTITION partition,u8* buf,u32 len)
{
	char cmd_buf[256] = {0};
	u32 sect_size;
	u32 write_size;
	u32 remain;
	int cmd_ret;
	
	if(spi_calc_partition_size(partition) <= 0) {
		printf("Unknown spi partition !\n");
		goto err_out;
	}

	//get current spi sector size
	sect_size = spi_get_current_sector_size();

	if(sect_size <= 0) {
		printf("Unknown SPI flash sector size !\n");
		goto err_out;
	}

	//get file size
	write_size = len;

	remain = (write_size % NT72668_SPI_WRITE_DMA_ALIGN);
	
	if(remain) 
		write_size += (NT72668_SPI_WRITE_DMA_ALIGN - remain);

	if(write_size > spi_calc_partition_size(partition)) {
		printf("Error ! Image size too large !, image size = %d, partition size = %d\n",
			write_size,spi_calc_partition_size(partition));
		goto err_out;
	}


	//erase spi partition
	sprintf(cmd_buf,"sf erase %x %x",spi_calc_partition_addr(partition),spi_calc_partition_size(partition));
	cmd_ret = run_command(cmd_buf,0);
	if(cmd_ret == -1) {
		printf("erase spi partition fail !\n");
		goto err_out;
	}
	
	//write image to spi partition
	sprintf(cmd_buf,"sf write %x %x %x",buf,
	spi_calc_partition_addr(partition),write_size);
	//spi_calc_partition_addr(partition),spi_calc_partition_size(partition));

	cmd_ret = run_command(cmd_buf,0);
	if(cmd_ret == -1) {
		printf("write image to spi partition fail !\n");
		goto err_out;
	}

	return 0;

err_out:
	return -1;
	
}

#ifndef CFG_USE_SPIFS

int do_nvt_swrite_cmd(cmd_tbl_t * cmdtp, int flag, int argc, char *argv[])
{
	int ret = 1;
	int fwup_ret = -1;
	int i;
	
	if(argc != 2 && argc != 3) {
		printf("Usage:\n%s\n", cmdtp->usage);
		ret = 1;
		goto err_out;
	}

	//kwinyee debug
#if 0	
	printf("STBC : start 0x%x len 0x%x\n",spi_calc_partition_addr(SPI_STBC),spi_calc_partition_size(SPI_STBC));
	printf("STBCDAT : start 0x%x len 0x%x\n",spi_calc_partition_addr(SPI_STBCDAT),spi_calc_partition_size(SPI_STBCDAT));
	printf("UBOOT : start 0x%x len 0x%x\n",spi_calc_partition_addr(SPI_UBOOT),spi_calc_partition_size(SPI_UBOOT));
	printf("KERNEL : start 0x%x len 0x%x\n",spi_calc_partition_addr(SPI_KERNEL),spi_calc_partition_size(SPI_KERNEL));
	printf("FS : start 0x%x len 0x%x\n",spi_calc_partition_addr(SPI_FS),spi_calc_partition_size(SPI_FS));
#endif	


	if(strcmp(argv[1],"all") == 0) {
		fwup_ret = nt72668_spi_update_bin(SPI_STBC, STBC_FNAME);
		
		if(fwup_ret < 0)
			goto err_out;
		
		fwup_ret = nt72668_spi_update_bin(SPI_STBCDAT, STBCDAT_FNAME);
		
		if(fwup_ret < 0)
			goto err_out;
		
		fwup_ret = nt72668_spi_update_bin(SPI_UBOOT, UBOOT_FNAME);
		
		if(fwup_ret < 0)
			goto err_out;
		
		fwup_ret = nt72668_spi_update_bin(SPI_KERNEL, KERNEL_FNAME);
		
		if(fwup_ret < 0)
			goto err_out;
		
		fwup_ret = nt72668_spi_update_bin(SPI_FS, FS_FNAME);
		
		if(fwup_ret < 0)
			goto err_out;
		
	} else if (strcmp(argv[1],"stbc")== 0) {
		if(argc == 3)
			fwup_ret = nt72668_spi_update_bin(SPI_STBC, argv[2]);
		else
			fwup_ret = nt72668_spi_update_bin(SPI_STBC, STBC_FNAME);
	} else if(strcmp(argv[1],"stbcdat") == 0) {
		if(argc == 3)
			fwup_ret = nt72668_spi_update_bin(SPI_STBCDAT, argv[2]);
		else
			fwup_ret = nt72668_spi_update_bin(SPI_STBCDAT, STBCDAT_FNAME);
	} else if(strcmp(argv[1],"uboot") == 0) {
		if(argc == 3)
			fwup_ret = nt72668_spi_update_bin(SPI_UBOOT, argv[2]);
		else
			fwup_ret = nt72668_spi_update_bin(SPI_UBOOT, UBOOT_FNAME);
	} else if(strcmp(argv[1],"ker")== 0) {
		if(argc == 3)
			fwup_ret = nt72668_spi_update_bin(SPI_KERNEL, argv[2]);
		else
			fwup_ret = nt72668_spi_update_bin(SPI_KERNEL, KERNEL_FNAME);
	} else if(strcmp(argv[1],"fs") == 0) {
		if(argc == 3)
			fwup_ret = nt72668_spi_update_bin(SPI_FS, argv[2]);
		else
			fwup_ret = nt72668_spi_update_bin(SPI_FS, FS_FNAME);
	}
#if defined(TK_SPI_8M)
	else if(strcmp(argv[1],"board") == 0) {
		if(argc == 3)
			fwup_ret = nt72668_spi_update_bin(SPI_BOARD, argv[2]);
		else
			fwup_ret = nt72668_spi_update_bin(SPI_BOARD, BOARD_FNAME);
	}
#endif
	if(fwup_ret < 0)
		goto err_out;
	
	return 0;
	
err_out:
	return ret;
	
}

U_BOOT_CMD(
	swrite,	3,	0,	do_nvt_swrite_cmd,
	"swrite    - write image to spi flash\n",
	"all - write all images into spi flash\n"
	"uboot - write uboot image into spi flash [default file name:nvt-uboot.bin]\n"
	"stbc - write stbc image into spi flash [default file name:stbc.bin]\n"
	"stbcdat - write stbc data image into spi flash [default file name:stbcdat.bin]\n"
	"ker - write kernel image into spi flash [default file name:rootfs.img]\n"
	"fs - write root file system image into spi flash [default file name:rootfs.img]\n"
);

#endif

