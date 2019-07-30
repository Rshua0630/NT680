/*
 *  board/novatek/evb668/nt72668_emmc_fwupdate.h
 *
 *  Author:	Alvin lin
 *  Created:	May 21, 2013
 *  Copyright:	Novatek Inc.
 *
 */
#ifndef __NT72668_EMMC_FWUPDATE_H__
#define __NT72668_EMMC_FWUPDATE_H__
#include "nt72668_emmc_partition.h"

#define BOOT_MAGIC_SIZE 8
#define BOOT_NAME_SIZE 16
#define BOOT_ARGS_SIZE 512

struct boot_img_hdr
{
    unsigned char magic[BOOT_MAGIC_SIZE];

    unsigned kernel_size;  /* size in bytes */
    unsigned kernel_addr;  /* physical load addr */

    unsigned ramdisk_size; /* size in bytes */
    unsigned ramdisk_addr; /* physical load addr */

    unsigned second_size;  /* size in bytes */
    unsigned second_addr;  /* physical load addr */

    unsigned tags_addr;    /* physical addr for kernel tags */
    unsigned page_size;    /* flash page size we assume */
    unsigned unused[2];    /* future expansion: should be 0 */

    unsigned char name[BOOT_NAME_SIZE]; /* asciiz product name */

    unsigned char cmdline[BOOT_ARGS_SIZE];

    unsigned id[8]; /* timestamp / checksum / sha1 / etc */
};


int nvt_emmc_image_help(struct _nvt_emmc_image_info *this, NVT_EMMC_IMG_HELP_TYPE help_type);
int nvt_emmc_read_img(struct _nvt_emmc_image_info *this, unsigned char *buf, unsigned int *rdlen, void* args);
int nvt_emmc_write_img(struct _nvt_emmc_image_info *this, unsigned char *buf, unsigned int buf_sz, void *args);
int nvt_emmc_erase_img(struct _nvt_emmc_image_info *this, void* args);
int nvt_emmc_read_android_boot_img(struct _nvt_emmc_image_info *this, unsigned char *buf, unsigned int *rdlen, void* args);
int nvt_emmc_read_secos_img(struct _nvt_emmc_image_info *this, unsigned char *buf, unsigned int *rdlen, void* args);

#define NVTFW_BOOT_FLAG_SIGN 		"BTFG"
#define NVTFW_UPDATE_FLAG_SIGN 		"UPFG"

typedef struct _boot_flag {
	unsigned char sign[4];//BTFG
	unsigned int uboot_idx;
	unsigned int kernel_idx;
	unsigned int fs_idx;
	unsigned char reserved[512 - 16];
} boot_flag;

#define FW_UPDATE_FLG_PATTERN 		0x5a5a55aa
#define FW_UPDATE_PTBL 			0x8001

typedef struct _update_flag {
	unsigned char sign[4];//UPFG
	unsigned int update_pattern;
	unsigned int update_ptable_flag;
	unsigned char reserved[512 - 12];
}update_flag;

#if defined(CONFIG_TCL_CN_668) || defined(CONFIG_TCL_AU_668)
#define TCLCN_FWUPDATE_FLAG_BLK_ID(pimg)  (pimg->pfs_part->start_sector + pimg->part_offset + pimg->size + MBR_FS_GAP - 1)
#define TCLCN_FWUPDATE_FLAG         0x8001
typedef struct _tclcn_fwupdate_flag {
	unsigned char sign[4];//UPFG
	unsigned int fwupdate_flag;
	unsigned int fwupdate_num;
	unsigned char reserved[512 - 12];
}tclcn_fwupdate_flag;
int tclcn668_read_update_flag(tclcn_fwupdate_flag *pstUpdate);
#endif

#if defined(CONFIG_TCL_EU_668)
#define NVTFW_UPDATE_FLAG_BLK_ID(pimg)  (pimg->pfs_part->start_sector + pimg->part_offset + pimg->size + MBR_FS_GAP - 2)
#define NVTFW_BOOT_FLAG_BLK_ID(pimg) (pimg->pfs_part->start_sector + pimg->part_offset + pimg->size + MBR_FS_GAP - 1)

int nt72668_read_update_flag(update_flag *pupdate);
int nt72668_read_boot_flag(boot_flag *pboot);
#endif

#endif

