/*
 *  board/novatek/evb668/nt72668_emmc_fwupdate.c
 *
 *  Author:	Alvin lin
 *  Created:	May 21, 2013
 *  Copyright:	Novatek Inc.
 *
 */

#include <asm-generic/errno.h>
#include "nt72668_emmc_fwupdate.h"
#include "nt72668_utils.h"
#include "nt72668_emmc_partition.h"

#ifdef CONFIG_NT72668_TK_TZONE
#include <asm/arch/nt72668_tzone.h>
#include "crypto_core.h"
#endif

#define ANDROID_BOOT_HEADER 4


#ifndef CONFIG_SYS_DCACHE_OFF
u32 v7_dcache_get_line_len(void);
#endif

int nvt_emmc_image_help(struct _nvt_emmc_image_info *this, NVT_EMMC_IMG_HELP_TYPE help_type)
{
	int ret = 0;

	if (this == NULL) {
		ret = -EINVAL;
		printf("%s : invalid image !\n",__func__);
		goto out;
	}

	switch(help_type) {
		case IMG_HELP_READ:
			printf("Usage: eread buffer_addr %s\n",this->image_name);
			break;

		case IMG_HELP_WRITE:
			printf("Usage: ewrite %s [%s]\n",this->image_name, this->default_name);
			break;

		case IMG_HELP_ERASE:
			printf("Usage: eerase %s\n",this->image_name);
			break;

		default:
			break;
	}
out:
	return ret;
}

int nvt_emmc_read_img(struct _nvt_emmc_image_info *this, unsigned char *buf, unsigned int *rdlen, void* args)
{
	int ret = 0;
	char cmd[64] = {0};
	unsigned int block_cnt = 0;

	if (this == NULL) {
		ret = -EINVAL;
		printf("%s : invalid image !\n",__func__);
		goto out;
	}

	//For some situation, we don't need read all the partition
	if(args != NULL)
	{
		block_cnt = *(int *)args;
	}
	else
	{
		block_cnt = this->size;
	}

	//switch to boot partition, if needed
	switch(this->part_type) {
		case PART_TYPE_BOOT1:
			ret = run_command("mmc dev 0 1", 0);
			if (ret < 0) {
				printf("%s: switch to boot partition 1 fail !\n",__func__);
				goto out;
			}
			break;

		case PART_TYPE_BOOT2:
			ret = run_command("mmc dev 0 2", 0);
			if (ret < 0) {
				printf("%s: switch to boot partition 2 fail !\n", __func__);
				goto out;
			}
			break;

		default:
			break;
	}

	//read image into buffer
	switch(this->part_type) {
		case PART_TYPE_BOOT1:
		case PART_TYPE_BOOT2:
			sprintf(cmd, "mmc read 0x%x 0x%x 0x%x", (unsigned int)buf,
					(unsigned int)this->part_offset, block_cnt);
			break;

		case PART_TYPE_BIN:
		default:
			if(this->user_part_id == 0) {
				//for mbr partition 0, first fs gap start from 2048, so we need seek back
				sprintf(cmd, "mmc read 0x%x 0x%x 0x%x", (unsigned int)buf,
						(unsigned int)(this->pfs_part->start_sector - MBR_FS_GAP + this->part_offset),
						block_cnt);
			} else {
				sprintf(cmd, "mmc read 0x%x 0x%x 0x%x", (unsigned int)buf,
						(unsigned int)(this->pfs_part->start_sector + this->part_offset),
						block_cnt);
			}

			break;
	}

	ret = run_command(cmd, 0);

	if(ret < 0) {
		printf("%s read image fail !\n",__func__);
		run_command("mmc dev 0 0", 0);
		*rdlen = 0;
		goto out;
	}

	*rdlen = block_cnt << BYTE_TO_SECTOR_SFT;

	//switch back to user partition, if needed
	switch(this->part_type) {
		case PART_TYPE_BOOT1:
		case PART_TYPE_BOOT2:
			ret = run_command("mmc dev 0 0", 0);
			if (ret < 0) {
				printf("%s: switch to user fail !\n", __func__);
				goto out;
			}
			break;

		default:
			break;
	}

out:
	return ret;
}


int nvt_emmc_read_android_boot_img(struct _nvt_emmc_image_info *this, unsigned char *buf, unsigned int *rdlen, void* args)
{
	int ret;
	char cmd[64] = {0};

	struct boot_address *addr = (struct boot_address *)args;
	struct boot_img_hdr *boot_img_hdr = (struct boot_img_hdr*)addr->kernel_addr;

	unsigned int kernel_size;
	unsigned int ramdisk_size;
	unsigned int ramdisk_offset;
	unsigned int block_cnt;

	sprintf(cmd, "mmc read 0x%x 0x%x 0x%x", (unsigned int)boot_img_hdr,
				(unsigned int)(this->pfs_part->start_sector + this->part_offset), ANDROID_BOOT_HEADER);
		ret = run_command(cmd, 0);
		if(strncmp ((char*)(boot_img_hdr->magic), "ANDROID!", 8) != 0) {
			printf("%s incorrect android boot.img !\n",__func__);
			ret = -1;
			goto out;
		}

	kernel_size = boot_img_hdr->kernel_size;
	ramdisk_size = boot_img_hdr->ramdisk_size;

	//Move ramdisk to right address
	if(addr->ramdisk_addr != NULL)
	{
		block_cnt = (((kernel_size + boot_img_hdr->page_size - 1) / boot_img_hdr->page_size) + ((ramdisk_size + boot_img_hdr->page_size - 1) / boot_img_hdr->page_size));
		ramdisk_offset = ((kernel_size + boot_img_hdr->page_size - 1) / boot_img_hdr->page_size) * boot_img_hdr->page_size;
	}
	else
	{
		block_cnt = (kernel_size + boot_img_hdr->page_size - 1) / boot_img_hdr->page_size;
	}

	//Normally the page minimum size is 512 bytes, so it is safe now
	block_cnt = block_cnt * (boot_img_hdr->page_size / SECTOR_IN_BYTES);

	sprintf(cmd, "mmc read 0x%x 0x%x 0x%x", (unsigned int)addr->kernel_addr,
		(unsigned int)(this->pfs_part->start_sector + this->part_offset + ANDROID_BOOT_HEADER), block_cnt);

	*rdlen = block_cnt << BYTE_TO_SECTOR_SFT;

	ret = run_command(cmd, 0);

	if(ret < 0) {
		printf("%s read image fail !\n",__func__);
		ret = -EIO;
		*rdlen = 0;

		goto out;
	}

	if(addr->ramdisk_addr != NULL)
	{
		/* Align to 4 bytes */
		ramdisk_size = (ramdisk_size + 3) & ~3;
		memmove_4byte_align(addr->ramdisk_addr, addr->kernel_addr + ramdisk_offset, ramdisk_size);
	}

out:

	return ret;

}

int nvt_emmc_read_secos_img(struct _nvt_emmc_image_info *this, unsigned char *buf, unsigned int *rdlen, void* args)
{
#ifdef CONFIG_NT72668_TK_TZONE	
	int ret = 0;
	char cmd[64] = {0};
	unsigned int Image_blk = 0;
	secos_header_t * header = 0;

	sprintf(cmd, "mmc read 0x%x 0x%x 0x%x", (unsigned int)buf, this->part_offset, 1);
	ret = run_command(cmd, 0);

	if(ret < 0) {
		printf("read secos image fail !\n");
		goto out;
	}

	buf = (unsigned char*)dec_secos_header();

	header = (secos_header_t * )(SECOS_BUFFER_START - sizeof(secos_header_t));

	if(buf == 0) {
		printf("header of secure OS is error !\n");
		ret = -EINVAL;
		goto out;
	} else if (buf == (unsigned char*)SECOS_BUFFER_BLOCK) //encrypted secos.bin
		Image_blk = this->size;
	else //non-encrypted secos.bin
		Image_blk = (header->BinSize) >> BYTE_TO_SECTOR_SFT;

	sprintf(cmd, "mmc read 0x%x 0x%x 0x%x", (unsigned int)buf, this->part_offset, Image_blk + 1);

	ret = run_command(cmd, 0);
	
	if(ret >= 0)
		*rdlen = Image_blk << BYTE_TO_SECTOR_SFT;
	else
		*rdlen = 0;
out:
	return ret;	
#else
	return 0;
#endif
}

int nvt_emmc_write_img(struct _nvt_emmc_image_info *this, unsigned char *buf, unsigned int buf_sz, void *args)
{
	int ret = 0;
	char cmd[64] = {0};
	unsigned int write_sectors = 0;
	unsigned int offset_blks = 0;

	if (this == NULL) {
		ret = -EINVAL;
		printf("%s : invalid image !\n",__func__);
		goto out;
	}
	//kwinyee debug
	//printf("ewrite: name:%s start:%d offset:%d size:%d\n",this->image_name,
	//		this->pfs_part->start_sector, this->part_offset,this->size);

	//check buffer size is less than or equal to partition size
	write_sectors = (buf_sz % SECTOR_IN_BYTES) ? ((buf_sz >> BYTE_TO_SECTOR_SFT) + 1) : (buf_sz >> BYTE_TO_SECTOR_SFT);

	if(write_sectors > this->size) {
		printf("%s error : image size(%d blks) large than partition size(%d blks) !\n",__func__,
				write_sectors, this->size);
		ret = -EIO;
		goto out;
	}

	if(args != NULL)
		offset_blks = *((unsigned int*)args);

	//switch to boot partition, if needed
	switch(this->part_type) {
		case PART_TYPE_BOOT1:
			//for emmc boot mode enable
			run_command("mmc rescan", 0);

			ret = run_command("mmc dev 0 1", 0);
			if (ret < 0) {
				printf("%s: switch to boot partition 1 fail !\n", __func__);
				goto out;
			}
			break;

		case PART_TYPE_BOOT2:
			//for emmc boot mode enable
			run_command("mmc rescan", 0);

			ret = run_command("mmc dev 0 2", 0);
			if (ret < 0) {
				printf("%s: switch to boot partition 2 fail !\n", __func__);
				goto out;
			}
			break;

		default:
			break;
	}

	//write image from buffer
	switch(this->part_type) {
		case PART_TYPE_BOOT1:
		case PART_TYPE_BOOT2:
			sprintf(cmd, "mmc write 0x%x 0x%x 0x%x", (unsigned int)buf,
					this->part_offset + offset_blks, write_sectors);
			break;

		case PART_TYPE_BIN:
		default:
			if(this->part_type == PART_TYPE_BIN && this->user_part_id == 0) {
				//for mbr partition 0, first fs gap start from 2048, so we need seek back
				sprintf(cmd, "mmc write 0x%x 0x%x 0x%x", (unsigned int)buf,
						(this->pfs_part->start_sector - MBR_FS_GAP + this->part_offset + offset_blks),
						write_sectors);
			} else {
				sprintf(cmd, "mmc write 0x%x 0x%x 0x%x", (unsigned int)buf,
						(this->pfs_part->start_sector + this->part_offset + offset_blks),
						write_sectors);
			}

			break;
	}
	//printf("emmc write cmd : %s\n", cmd);
	ret = run_command(cmd, 0);

	if(ret < 0) {
		printf("%s write image fail !\n",__func__);
		run_command("mmc dev 0 0", 0);
		goto out;
	}


	//switch back to user partition, if needed
	switch(this->part_type) {
		case PART_TYPE_BOOT1:
		case PART_TYPE_BOOT2:
			ret = run_command("mmc dev 0 0", 0);
			if (ret < 0) {
				printf("%s: switch to user fail !\n", __func__);
				goto out;
			}
			break;

		default:
			break;
	}


out:
	return ret;
}


int nvt_emmc_erase_img(struct _nvt_emmc_image_info *this, void* args)
{
	int ret = 0;
	char cmd[64] = {0};

	if (this == NULL) {
		ret = -EINVAL;
		printf("%s : invalid image !\n",__func__);
		goto out;
	}

	//switch to boot partition, if needed
	switch(this->part_type) {
		case PART_TYPE_BOOT1:
			ret = run_command("mmc dev 0 1", 0);
			if (ret < 0) {
				printf("%s: switch to boot partition 1 fail !\n", __func__);
				goto out;
			}
			break;

		case PART_TYPE_BOOT2:
			ret = run_command("mmc dev 0 2", 0);
			if (ret < 0) {
				printf("%s: switch to boot partition 2 fail !\n", __func__);
				goto out;
			}
			break;

		default:
			break;
	}

	//erase partition
	switch(this->part_type) {
		case PART_TYPE_BOOT1:
		case PART_TYPE_BOOT2:
			memset(CONFIG_SYS_FWUPDATE_BUF, 0x0, (this->size) << BYTE_TO_SECTOR_SFT);
			sprintf(cmd, "mmc write 0x%x 0x%x 0x%x", CONFIG_SYS_FWUPDATE_BUF, this->part_offset,
					this->size);
			break;
		case PART_TYPE_BIN:
			memset(CONFIG_SYS_FWUPDATE_BUF, 0x0, (this->size) << BYTE_TO_SECTOR_SFT);

			if(this->user_part_id == 0) {
				//for mbr partition 0, first fs gap start from 2048, so we need seek back
				sprintf(cmd, "mmc write 0x%x 0x%x 0x%x", CONFIG_SYS_FWUPDATE_BUF,
						(this->pfs_part->start_sector - MBR_FS_GAP + this->part_offset),
						this->size);
			} else {
				sprintf(cmd, "mmc write 0x%x 0x%x 0x%x", CONFIG_SYS_FWUPDATE_BUF,
						(this->pfs_part->start_sector + this->part_offset),
						this->size);
			}

			break;

		default:
			memset(CONFIG_SYS_FWUPDATE_BUF, 0x0, (FS_ERASE_BLK_CNT << BYTE_TO_SECTOR_SFT));
			sprintf(cmd, "mmc write 0x%x 0x%x 0x%x", CONFIG_SYS_FWUPDATE_BUF,
					(this->pfs_part->start_sector + this->part_offset),
					(FS_ERASE_BLK_CNT));
	}

	ret = run_command(cmd, 0);

	if(ret < 0) {
		printf("%s erase image fail !\n",__func__);
		run_command("mmc dev 0 0", 0);
		goto out;
	}

	//switch back to user partition, if needed
	switch(this->part_type) {
		case PART_TYPE_BOOT1:
		case PART_TYPE_BOOT2:
			ret = run_command("mmc dev 0 0", 0);
			if (ret < 0) {
				printf("%s: switch to user fail !\n", __func__);
				goto out;
			}
			break;

		default:
			break;
	}


out:
	return ret;
}

#define NVT_FWUPDATE_MAX_WRITE_BYTES 		(450 * 1024 *1024)
#define NVT_FWUPDATE_MAX_WRITE_BLKS 		(NVT_FWUPDATE_MAX_WRITE_BYTES >> BYTE_TO_SECTOR_SFT)

int do_nvt_emmc_write(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
	int ret = 0;
	char cmd[64] = {0};
	char img_name[64] = {0};
	unsigned int flen;
	unsigned int read_blks;
	unsigned int read_len;
	int cnt,i;
	nvt_emmc_image_info *pimg;

	if(argc != 2 && argc != 3)
		return CMD_RET_USAGE;

	cnt = nvt_emmc_get_image_cnt();

	if(strcmp(argv[1],"help") == 0) {
		for(i=0;i < cnt;i++) {
			pimg = nvt_emmc_get_img_by_index(i);
			if(pimg->image_help != NULL)
				pimg->image_help(pimg, IMG_HELP_WRITE);
		}

		goto out;
	}

	pimg = nvt_emmc_get_img_by_name(argv[1]);

	if(pimg == NULL) {
		printf("%s: unknown image name %s\n",__func__,argv[1]);
		ret = -EINVAL;
		goto out;
	}

	if(argc == 3)
		strcpy(img_name, argv[2]);
	else
		strcpy(img_name, pimg->default_name);

	ret = run_command("usb start", 0);
	if(ret != 0) {
		printf("%s init usb fail !\n",__func__);
		ret = -EIO;
		goto out;
	}

	read_blks = 0;

	//initial partition table first be write emmc to get correct offset	
	nvt_emmc_part_init();

	do {
		sprintf(cmd, "fatload usb 0 0x%x %s 0x%x 0x%x", CONFIG_SYS_FWUPDATE_BUF,
				 img_name, NVT_FWUPDATE_MAX_WRITE_BYTES, (read_blks << BYTE_TO_SECTOR_SFT));
		//printf("ewrite fatcmd: %s\n", cmd);
		ret = run_command(cmd, 0);
		if(ret != 0) {
			printf("read image file %s error !\n", img_name);
			ret = -EIO;
			goto out;
		}

		flen = simple_strtoul(getenv("filesize"), NULL, 16);

		if(pimg->size < (read_blks + (flen >> BYTE_TO_SECTOR_SFT))) {
			printf("%s image size large than partition size, update fail !\n", img_name);
			ret = -EINVAL;
			goto out;
		}

		if(pimg->write_img != NULL)
			ret = pimg->write_img(pimg, (unsigned char*)CONFIG_SYS_FWUPDATE_BUF, flen, &read_blks);

		if(ret != 0) {
			printf("write image file %s error !\n", img_name);
			ret = -EIO;
			goto out;
		}

		if(flen < NVT_FWUPDATE_MAX_WRITE_BYTES)
			break;
		else
			read_blks += NVT_FWUPDATE_MAX_WRITE_BLKS;
	}while(1);

out:
	return ret;
}

U_BOOT_CMD(
	ewrite, 3, 1, do_nvt_emmc_write,
	"ewrite - write image to emmc partition",
	"       use \"ewrite help\" to get more information\n"
);

int do_nvt_emmc_erase(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{

	int ret = 0;
	int cnt,i;
	nvt_emmc_image_info *pimg;

	if(argc != 2)
		return CMD_RET_USAGE;

	cnt = nvt_emmc_get_image_cnt();

	if(strcmp(argv[1],"help") == 0) {
		for(i=0;i < cnt;i++) {
			pimg = nvt_emmc_get_img_by_index(i);
			if(pimg->image_help != NULL)
				pimg->image_help(pimg, IMG_HELP_ERASE);
		}

		goto out;
	}

	pimg = nvt_emmc_get_img_by_name(argv[1]);

	if(pimg == NULL) {
		printf("%s: unknown image name %s\n",__func__,argv[1]);
		ret = -EINVAL;
		goto out;
	}

	if(pimg->erase_img != NULL)
		ret = pimg->erase_img(pimg, NULL);

out:
	return ret;
}

U_BOOT_CMD(
	eerase, 2, 1, do_nvt_emmc_erase,
	"eerase - erase image from emmc partition",
	"       use \"eerase help\" to get more information\n"
);

int do_nvt_emmc_get_ddrinfo(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
	int ret = 0;
	char cmd[128] = {0};
	char *out_fname;
	unsigned int ddr_len;
	nvt_emmc_image_info *pimg;


	pimg = nvt_emmc_get_img_by_name("ddrcfg");

	if(pimg == NULL || pimg->read_img == NULL) {
		ret = -EINVAL;
		printf("ddr config partition not exist !\n");
		goto out;
	}

	ret = pimg->read_img(pimg, (unsigned char*)CONFIG_SYS_FWUPDATE_BUF, &ddr_len, NULL);

	if(ret < 0) {
		ret = -EIO;
		printf("read ddr config from emmc error !\n");
		goto out;
	}


	if(argc >= 2)
		out_fname = argv[1];
	else
		out_fname = "668_ddr.ddr";

	ret = run_command("usb reset\0",0);

	if(ret != 0) {
		printf("usb storage not ready !\n");
		goto out;
	}

	sprintf(cmd,"fatwrite usb 0:1 0x%x %s 0x%x",CONFIG_SYS_FWUPDATE_BUF, out_fname, ddr_len);

	ret = run_command(cmd, 0);

	if(ret < 0) {
		printf("write ddr config to usb fail !\n");
		goto out;
	}

out:
	return ret;
}

U_BOOT_CMD(
	egetddr, 2, 1, do_nvt_emmc_get_ddrinfo,
	"egetddr - get ddr setting from board",
	"      fname - add fname parameter to specify saved file name, or else default name is 668_ddr.ddr \n"
	);

int do_nvt_emmc_read(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{

	int ret = 0;
	int i,cnt;
	nvt_emmc_image_info *pimg;
	unsigned char *addr;
	unsigned int rdlen;

	if(argc != 2 && argc !=  3)
		return CMD_RET_USAGE;

	cnt = nvt_emmc_get_image_cnt();

	if(strcmp(argv[1],"help") == 0) {
		for(i=0;i < cnt;i++) {
			pimg = nvt_emmc_get_img_by_index(i);
			if(pimg->image_help != NULL)
				pimg->image_help(pimg, IMG_HELP_READ);
		}

		goto out;
	}

	addr = simple_strtoul(argv[1],NULL,16);

	pimg = nvt_emmc_get_img_by_name(argv[2]);

	if(pimg == NULL) {
		printf("%s invalid image name : %s\n",__func__,argv[2]);
		ret = -EINVAL;
		goto out;
	}

	if(pimg->read_img != NULL)
		ret = pimg->read_img(pimg, addr, &rdlen, NULL);

	if (ret != 0) {
		printf("eread image error !\n");
		goto out;
	} else
		printf("image read to 0x%x, len = 0x%x\n",addr,rdlen);

out:
	return ret;
}

U_BOOT_CMD(
	eread, 3, 1, do_nvt_emmc_read,
	"eread - read image from emmc partition to dram",
	"       use \"eread help\" to get more information\n"
);

int do_nvt_emmc_miscmd(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
	int ret = 0;
	unsigned int len;
	char cmd[128] = {0};
	unsigned char *ker_load_addr = NULL;
	unsigned char *ramdisk_load_addr = NULL;
	nvt_emmc_image_info *pimg;

	/* eMMC write buffer must aligned in 64 bytes */
	struct bootloader_message __attribute__((__aligned__(64)))  misc_info ;

	if(argc != 2)
		return CMD_RET_USAGE;

	memset(&misc_info, 0, sizeof(struct bootloader_message));
	strncpy(misc_info.command, argv[1], strlen(argv[1]));

	pimg = nvt_emmc_get_img_by_name("misc");

	if(pimg == NULL) {
		printf("%s get misc partition fail !\n",__func__);
		return -EINVAL;
	}

	ret = pimg->write_img(pimg, (unsigned char*)(&misc_info), sizeof(struct bootloader_message), NULL);


	return ret;
}

U_BOOT_CMD(
	emiscmd, 3, 1, do_nvt_emmc_miscmd,
	"emiscmd - write misc command to misc partition",
	""
);


int do_nvt_emmc_dump(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
	int ret = 0;
	unsigned int len;
	char cmd[128] = {0};
	unsigned char *ker_load_addr = NULL;
	unsigned char *ramdisk_load_addr = NULL;
	nvt_emmc_image_info *pimg;

	/* eMMC write buffer must aligned in 64 bytes */
	struct bootloader_message __attribute__((__aligned__(64)))  misc_info ;

	memset(&misc_info, 0, sizeof(struct bootloader_message));
	strncpy(misc_info.command, "emmc-dump", 9);

	pimg = nvt_emmc_get_img_by_name("misc");

	if(pimg == NULL) {
		printf("%s get misc partition fail !\n",__func__);
		goto loop;
	}

	/* Shell script behavior depends on command in MISC partition */
	pimg->write_img(pimg, (unsigned char*)(&misc_info), sizeof(struct bootloader_message), NULL);

	setenv("dump_restore", "edump");

	sprintf(cmd, "nvt_boot;bootm ${kernel_loadaddr} ${ramdisk_loadaddr}");

	ret = run_command(cmd, 0);

	if(ret < 0) {
		printf("Run into recovery fail !\n");
		goto loop;
	}

	return ret;

loop:
	while(1);

}


U_BOOT_CMD(
	edump, 2, 1, do_nvt_emmc_dump,
	"edump - dump emmc partition to U-disk",
	"edump "
);

int do_nvt_emmc_eupm(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
	int ret = 0;
	unsigned int len;
	char cmd[128] = {0};
	unsigned char *ker_load_addr = NULL;
	unsigned char *ramdisk_load_addr = NULL;
	nvt_emmc_image_info *pimg;

	/* eMMC write buffer must aligned in 64 bytes */
	struct bootloader_message __attribute__((__aligned__(64)))  misc_info ;

	memset(&misc_info, 0, sizeof(struct bootloader_message));
	strncpy(misc_info.command, "emmc-eupm", 9);

	pimg = nvt_emmc_get_img_by_name("misc");

	if(pimg == NULL) {
		printf("%s get misc partition fail !\n",__func__);
		goto loop;
	}

	/* Shell script behavior depends on command in MISC partition */
	pimg->write_img(pimg, (unsigned char*)(&misc_info), sizeof(struct bootloader_message), NULL);

	setenv("dump_restore", "eupm");

	sprintf(cmd, "nvt_boot;bootm ${kernel_loadaddr} ${ramdisk_loadaddr}");

	ret = run_command(cmd, 0);

	if(ret < 0) {
		printf("Run into recovery fail !\n");
		goto loop;
	}

	return ret;

loop:
	while(1);

}


U_BOOT_CMD(
	eupm, 2, 1, do_nvt_emmc_eupm,
	"eupm - dump emmc image to U-disk",
	"eupm "
);


int do_nvt_emmc_restore(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
	int ret = 0;
	unsigned int len;
	char cmd[128] = {0};
	unsigned char *ker_load_addr = NULL;
	unsigned char *ramdisk_load_addr = NULL;
	nvt_emmc_image_info *pimg;

	/* eMMC write buffer must aligned in 64 bytes */
	struct bootloader_message __attribute__((__aligned__(64)))  misc_info ;

	memset(&misc_info, 0, sizeof(struct bootloader_message));
	strncpy(misc_info.command, "emmc-restore", 12);

	pimg = nvt_emmc_get_img_by_name("misc");

	if(pimg == NULL) {
		printf("%s get misc partition fail !\n",__func__);
		goto loop;
	}

	/* Shell script behavior depends on command in MISC partition */
	pimg->write_img(pimg, (unsigned char*)(&misc_info), sizeof(struct bootloader_message), NULL);

	setenv("dump_restore", "erestore");

	sprintf(cmd, "nvt_boot;bootm ${kernel_loadaddr} ${ramdisk_loadaddr}");

	ret = run_command(cmd, 0);

	if(ret < 0) {
		printf("Run into recovery fail !\n");
		goto loop;
	}

	return ret;
loop:
	while(1);
}


U_BOOT_CMD(
	erestore, 2, 1, do_nvt_emmc_restore,
	"erestore - restore emmc partition from U-disk",
	"erestore"
);

#if defined(CONFIG_USB_UPDATE)	/* Logic add for usb update */
// ---
//2015/3/3 - disable by PSW2 / Waynce Chen.
// note:
// don't use the old flow to write command to misc partition.
// Use emiscmd command to write recovery command.
// ---
#if 0
int do_nvt_emmc_check(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
	int ret = 0;
	char cmd[64] = {0};
	char img_name[64] = {0};

	if(argc != 2)
		return CMD_RET_USAGE;

	if(argc == 2)
	{
		strcpy(img_name, argv[1]);
		sprintf(cmd,"nvtload 0x%x %s",CONFIG_SYS_FWUPDATE_BUF, img_name);
		
		ret = run_command(cmd, 0);
		if(ret != 0)
		{
			printf("read update file %s  error !\n",img_name);
			ret = -EIO;
			goto out;
		}
	}

out:
	return ret;
}

U_BOOT_CMD(
	echeck, 2, 1, do_nvt_emmc_check,
	"echeck - check update file existed",
	" "
);

int do_nvt_emmc_update(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
	int ret = 0;
	unsigned int len;
	char cmd[128] = {0};
	unsigned char *ker_load_addr = NULL;
	unsigned char *ramdisk_load_addr = NULL;
	nvt_emmc_image_info *pimg;

	/* eMMC write buffer must aligned in 64 bytes */
	struct bootloader_message __attribute__((__aligned__(64)))  misc_info ;

	if(argc != 5)
		return CMD_RET_USAGE;

	memset(&misc_info, 0, sizeof(struct bootloader_message));
	strncpy(misc_info.command, argv[1], strlen(argv[1]));
	strncpy(misc_info.recovery, argv[2], strlen(argv[2]));
	strncat(misc_info.recovery, "\n", 1);
	strncat(misc_info.recovery, argv[3], strlen(argv[3]));
	strncat(misc_info.recovery, "\n", 1);
	strncat(misc_info.recovery, argv[4], strlen(argv[4]));

	pimg = nvt_emmc_get_img_by_name("misc");

	if(pimg == NULL) {
		printf("%s get misc partition fail !\n",__func__);
		return -EINVAL;
	}

	ret = pimg->write_img(pimg, (unsigned char*)(&misc_info), sizeof(struct bootloader_message), NULL);


	return ret;
}

U_BOOT_CMD(
	eupdate, 5, 1, do_nvt_emmc_update,
	"eupdate - android recovery update",
	""
);
#endif

int do_nvt_emmc_cu(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
	int ret = 0;

	if(argc != 1)
		return CMD_RET_USAGE;

	//2015/3/3 - disable by PSW2 / Waynce Chen.
	//skip the file check to run the recovery upgrade on each USB port.
	/*
	if(run_command("echeck /TargetHis/His72667Upgrage.zip", 0) < 0)
	{
		printf("The update file is not exist...!\n");
		ret = -ENOENT;
	}
	else
	*/	
	{
		//if(run_command("eupdate boot-recovery recovery --update_package=/usb/TargetHis/His72667Upgrade.zip --reboot", 0) < 0)
		if(run_command("emiscmd boot-recovery recovery --update_package=/usb/TargetHis/His72667Upgrade.zip --reboot", 0) < 0)
		{
			printf("write recovery flag fail ...!\n");
			ret = -ENOENT;
		}
		else
		{
			printf("write recovery flag done ...\n");
			run_command("boot", 0);
		}
	}

	return ret;
}

U_BOOT_CMD(
	cu, 1, 1, do_nvt_emmc_cu,
	"boot from recovery mode",
	""
);
#endif

#if defined(CONFIG_TCL_CN_668) || defined(CONFIG_TCL_AU_668)
int tclcn668_read_update_flag(tclcn_fwupdate_flag *pstUpdate)
{

	int ret = 0;
	char cmd[128] = {};
	nvt_emmc_image_info *pimg;

	if(pstUpdate == NULL) {
		ret = -1;
		goto out;
	}

	pimg = nvt_emmc_get_img_by_name("misc");
	if(pimg == NULL) {
		ret = -1;
		goto out;
	}


	sprintf(cmd,"mmc read 0x%x 0x%x 1", pstUpdate, TCLCN_FWUPDATE_FLAG_BLK_ID(pimg));
	ret = run_command(cmd, 0);

	if(ret < 0)
		goto out;


    /*printf("dd debug message pstUpdate->fwupdate_flag = %d \n",pstUpdate->fwupdate_flag);*/

	if(pstUpdate->fwupdate_flag != TCLCN_FWUPDATE_FLAG) {
		ret = -1;
		goto out;
	}

out:
	return ret;
}
#endif

#if defined(CONFIG_TCL_EU_668)
int nt72668_read_update_flag(update_flag *pupdate)
{
	int ret = 0;
	char cmd[128] = {};
	nvt_emmc_image_info *pimg;

	if(pupdate == NULL) {
		ret = -1;
		goto out;
	}

	pimg = nvt_emmc_get_img_by_name("misc");
	if(pimg == NULL) {
		ret = -1;
		goto out;
	}


	sprintf(cmd,"mmc read 0x%x 0x%x 1", pupdate, NVTFW_UPDATE_FLAG_BLK_ID(pimg));
	ret = run_command(cmd, 0);

	if(ret < 0)
		goto out;

	if(memcmp(pupdate->sign, NVTFW_UPDATE_FLAG_SIGN, 4)) {
		ret = -1;
		goto out;
	}

	if(pupdate->update_pattern != FW_UPDATE_FLG_PATTERN){
		ret = -1;
		goto out;
	}

	if(pupdate->update_ptable_flag != 0 && pupdate->update_ptable_flag != FW_UPDATE_PTBL) {
		ret = -1;
		goto out;
	}

out:
	return ret;
}

int nt72668_read_boot_flag(boot_flag *pboot)
{
	int ret = 0;
	char cmd[128] = {};
	nvt_emmc_image_info *pimg;

	if(pboot == NULL) {
		ret = -1;
		goto out;
	}

	pimg = nvt_emmc_get_img_by_name("misc");
	if(pimg == NULL) {
		ret = -1;
		goto out;
	}

	sprintf(cmd, "mmc read 0x%x 0x%x 1", pboot, NVTFW_BOOT_FLAG_BLK_ID(pimg));
	ret = run_command(cmd, 0);

	if(ret < 0)
		goto out;

	if(memcmp(pboot->sign, NVTFW_BOOT_FLAG_SIGN, 4)) {
		ret = -1;
		goto out;
	}

	if(pboot->uboot_idx != 0 && pboot->uboot_idx != 1) {
		ret = -1;
		goto out;
	}

	if(pboot->kernel_idx != 0 && pboot->kernel_idx != 1) {
		ret = -1;
		goto out;
	}

	if(pboot->fs_idx != 0 && pboot->fs_idx != 1) {
		ret = -1;
		goto out;
	}
out:
	return ret;
}

int do_nvt_write_update_flag(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
	update_flag __attribute__((__aligned__(64)))  uflag = {};
	char cmd[64] = {};
	int ret = 0;
	unsigned int update_ptbl = 0;
	nvt_emmc_image_info *pimg;

	pimg = nvt_emmc_get_img_by_name("misc");
	if(pimg == NULL) {
		ret = -1;
		goto out;
	}

	if(argc != 1 && argc != 2)
		return CMD_RET_USAGE;

	if(argc == 1) {
		sprintf(cmd,"mmc write 0x%x 0x%x 1", &uflag, NVTFW_UPDATE_FLAG_BLK_ID(pimg));
		ret = run_command(cmd, 0);
	} else {
		update_ptbl = simple_strtoul(argv[1], NULL, 10);
		memcpy(uflag.sign, NVTFW_UPDATE_FLAG_SIGN, strlen(NVTFW_UPDATE_FLAG_SIGN));
		uflag.update_pattern = FW_UPDATE_FLG_PATTERN;
		if (update_ptbl == 0)
			uflag.update_ptable_flag = 0;
		else
			uflag.update_ptable_flag = FW_UPDATE_PTBL;

		sprintf(cmd,"mmc write 0x%x 0x%x 1", &uflag, NVTFW_UPDATE_FLAG_BLK_ID(pimg));
		ret = run_command(cmd, 0);
	}
out:
	return ret;
}
U_BOOT_CMD(
	eupdateflag, 2, 1, do_nvt_write_update_flag,
	"eupdateflag - write update flag to misc partition",
	"eupdateflag update_ptbl(0/1)"
);

int do_nvt_write_boot_flag(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
	boot_flag __attribute__((__aligned__(64)))  bflag = {};
	char cmd[64] = {};
	int ret = 0;
	int uboot_idx,kernel_idx,fs_idx;
	nvt_emmc_image_info *pimg;

	pimg = nvt_emmc_get_img_by_name("misc");
	if(pimg == NULL) {
		ret = -1;
		goto out;
	}

	if(argc != 1 && argc != 4)
		return CMD_RET_USAGE;

	if(argc == 1) {
		sprintf(cmd,"mmc write 0x%x 0x%x 1", &bflag, NVTFW_BOOT_FLAG_BLK_ID(pimg));
		ret = run_command(cmd, 0);
	} else {
		uboot_idx = simple_strtoul(argv[1], NULL, 10);
		kernel_idx = simple_strtoul(argv[2], NULL, 10);
		fs_idx = simple_strtoul(argv[3], NULL, 10);
		memcpy(bflag.sign, NVTFW_BOOT_FLAG_SIGN, strlen(NVTFW_BOOT_FLAG_SIGN));

		if (uboot_idx == 0)
			bflag.uboot_idx = 0;
		else
			bflag.uboot_idx = 1;

		if (kernel_idx == 0)
			bflag.kernel_idx = 0;
		else
			bflag.kernel_idx = 1;

		if (fs_idx == 0)
			bflag.fs_idx = 0;
		else
			bflag.fs_idx = 1;

		sprintf(cmd,"mmc write 0x%x 0x%x 1", &bflag, NVTFW_BOOT_FLAG_BLK_ID(pimg));
		ret = run_command(cmd, 0);
	}
out:
	return ret;
}

U_BOOT_CMD(
	ebootflag, 4, 1, do_nvt_write_boot_flag,
	"ebootflag - write boot flag to misc partition",
	"ebootflag uboot_idx kernel_idx fs_idx"
);


int do_nvt_read_flag(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
	int boot_flag_valid = 0;
	int update_flag_valid = 0;
	boot_flag __attribute__((__aligned__(64)))  bflag = {};
	update_flag __attribute__((__aligned__(64)))  uflag = {};

	boot_flag_valid = nt72668_read_boot_flag(&bflag);
	update_flag_valid = nt72668_read_update_flag(&uflag);

	if(boot_flag_valid == 0) {
		printf("boot flag valid !\n");
		printf("uidx : %d, kidx : %d, fidx : %d\n",bflag.uboot_idx,bflag.kernel_idx,bflag.fs_idx);
	}

	if(update_flag_valid == 0) {
		printf("update flag valid !\n");
		printf("update ptable = 0X%X\n",uflag.update_ptable_flag);
	}

	return 0;
}

U_BOOT_CMD(
	ereadflag, 1, 1, do_nvt_read_flag,
	"ereadflag - read current boot flag and update flag",
	""
);
#endif

int nvt_mmc_erase_part(NVT_EMMC_PART_TYPE part_type)
{
	char cmd[128] = {};
	int ret = 0;
	struct mmc *mmc = find_mmc_device(CONFIG_SYS_MMC_SYS_DEV);

	switch(part_type) {
		case PHY_PART_TYPE_BOOT1:
			ret = run_command("mmc dev 0 1", 0);
			if(ret < 0) {
				printf("%s switch to boot1 partition fail !\n",__func__);
				goto out;
			}
			sprintf(cmd,"mmc erase 0 0x%x",(mmc->boot_capacity) >> BYTE_TO_SECTOR_SFT);
			ret = run_command(cmd, 0);
			if(ret < 0) {
				printf("%s erase boot1 partition fail !\n");
				goto out;
			}
			break;

		case PHY_PART_TYPE_BOOT2:
			ret = run_command("mmc dev 0 2", 0);
			if(ret < 0) {
				printf("%s switch to boot2 partition fail !\n",__func__);
				goto out;
			}
			sprintf(cmd,"mmc erase 0 0x%x",(mmc->boot_capacity) >> BYTE_TO_SECTOR_SFT);
			ret = run_command(cmd, 0);
			if(ret < 0) {
				printf("%s erase boot2 partition fail !\n");
				goto out;
			}

			break;

		case PHY_PART_TYPE_USER:
			ret = run_command("mmc dev 0 0", 0);
			if(ret < 0) {
				printf("%s switch to user partition fail !\n",__func__);
				goto out;
			}

			sprintf(cmd,"mmc erase 0 0x%x",(mmc->capacity) >> BYTE_TO_SECTOR_SFT);
			ret = run_command(cmd, 0);
			if(ret < 0) {
				printf("%s erase user partition fail !\n");
				goto out;
			}
			break;

		case PHY_PART_TYPE_RPMB:
		default:
			break;
	}

out:
	if(part_type == PHY_PART_TYPE_BOOT1 || part_type == PHY_PART_TYPE_BOOT2)
		run_command("mmc dev 0 0", 0);
	return ret;
}

int do_nvt_emmc_eraseall(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{

	if(argc != 2 || (strcmp(argv[1],"fast") != 0 && strcmp(argv[1],"full") != 0))
		return CMD_RET_USAGE;

	if(strcmp(argv[1],"fast") == 0) {
		int i,cnt;
		nvt_emmc_image_info *pimg;
		cnt = nvt_emmc_get_image_cnt();
		for(i=0; i<cnt;i++) {
			pimg = nvt_emmc_get_img_by_index(i);
			if(pimg->erase_img != NULL)
				pimg->erase_img(pimg, NULL);
		}
	} else {
		nvt_mmc_erase_part(PHY_PART_TYPE_BOOT1);
		nvt_mmc_erase_part(PHY_PART_TYPE_BOOT2);
		nvt_mmc_erase_part(PHY_PART_TYPE_USER);
	}
	return 0;
}

U_BOOT_CMD(
	eeraseall, 2, 1, do_nvt_emmc_eraseall,
	"eeraseall - erase all images on emmc",
	"eeraseall fast || full"
);

