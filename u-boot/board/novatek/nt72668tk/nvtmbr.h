/*
 *  board/novatek/evb668/nvtmbr.h
 *
 *  Author:	Alvin lin
 *  Created:	May 20, 2013
 *  Copyright:	Novatek Inc.
 *
 */

#ifndef _NVT_MBR_H__
#define _NVT_MBR_H__

#include <common.h>
#include <command.h>
#include <mmc.h>

#define PTYPE_EMPTY 		0x00
#define PTYPE_FAT12 		0x01
#define PTYPE_FAT16 		0x04
#define PTYPE_MSEXT_CHS		0x05
#define PTYPE_FAT16L 		0x06
#define PTYPE_NTFS 		0x07
#define PTYPE_FAT32_CHS 	0x0b
#define PTYPE_FAT32_LBA 	0x0c
#define PTYPE_FAT16_LBA 	0x0e
#define PTYPE_MSEXT_LBA 	0x0f
#define PTYPE_HFAT12_CHS 	0x11
#define PTYPE_HFAT16_LCHS 	0x14
#define PTYPE_HFAT16_MCHS 	0x16
#define PTYPE_HFAT32_CHS 	0x1b
#define PTYPE_HFAT32_LBA 	0x1c
#define PTYPE_HFAT16_LBA 	0x1e
#define PTYPE_MSMBR 		0x42
#define PTYPE_LINUX_SWAP 	0x82
#define PTYPE_LINUX_FS 		0x83
#define PTYPE_HIBERNATION1 	0x84
#define PTYPE_LINUX_EXT 	0x85
#define PTYPE_NTFS_VOL1 	0x86
#define PTYPE_NTFS_VOL2 	0x87
#define PTYPE_HIBERNATION2 	0xa0
#define PTYPE_HIBERNATION3 	0xa1
#define PTYPE_FREEBSD 		0xa5
#define PTYPE_OPENBSD 		0xa6
#define PTYPE_MAC_OSX 		0xa8
#define PTYEP_NETBSD 		0xa9
#define PTYPE_MAC_OSX_BOOT 	0xab
#define PTYPE_BSDI 		0xb7
#define PTYPE_BSDI_SWAP 	0xb8
#define PTYPE_EFI_GPT 		0xee
#define PTYPE_EFI_SYSPART 	0xef
#define PTYPE_VM_FS 		0xfb
#define PTYPE_VM_SWAP 		0xfc

#define MAX_PRIMARY_PART_NUM 	4
#define MAX_PART_ENTRY_PER_MBR 	4
#define SECTOR_IN_BYTES 	512
#define BYTE_TO_SECTOR_SFT 	9

#define MAX_ALLOW_PART_NUM 	30

#define BOOT_CODE_SIZE 		446
#define MBR_FS_GAP 		2048//count in sector
#define EXTEND_FS_GAP 		48//count in sector

#define EXT_PART_EXTRA_GAP 	2048//count in sector

#if defined(CONFIG_TCL668)
#define EXT_PART_EXTRA_TCL_NOUSE (300*1024 *2) // left 300M no use
#else
#define EXT_PART_EXTRA_TCL_NOUSE 0
#endif


struct dos_part_entry{
	unsigned char boot_flag;//1:means this partion can use to boot
	unsigned char start_chs[3];//start chs
	unsigned char part_type;//partition type
	unsigned char end_chs[3];//end chs
	unsigned int start_lba;//start LBA address
	unsigned int size_in_sector;//size in sector
}__attribute__((packed));

struct mbr {
	unsigned char boot_code[BOOT_CODE_SIZE];//0-445 bytes are boot code
	struct dos_part_entry part_entrys[MAX_PART_ENTRY_PER_MBR];//partition entrys
	unsigned char signature[2];//0xaa55
} __attribute__((packed));

struct mbr_lite {
	struct dos_part_entry part_entrys[MAX_PART_ENTRY_PER_MBR];//partition entrys
	unsigned char signature[2];//0xaa55
}__attribute__((packed));

struct mbr_dram {
	unsigned int used;
	unsigned int curr_offset;
	struct mbr_lite lmbr;
};

struct nvt_dos_partition {
	char fs_str[16];
	unsigned int fs_type;
	unsigned int ori_start;
	unsigned int start_sector;
	unsigned int end_sector;
	unsigned int length;
};

struct part_parm {
	unsigned int id;
	unsigned int sectors;//partition size in sectors
};

struct nvt_fs_partition {
	unsigned int used;
	unsigned int id;
	unsigned int start_sector;
	unsigned int end_sector;
	unsigned int length;//count in sector
};
int nvt_parse_dos_part_table(struct nvt_dos_partition *ptable);
int nvt_dump_dos_part_table(struct nvt_dos_partition *pdostbl);
unsigned long nvt_get_disk_total_sectors(void);
int nvt_gen_mbr_dram_tbl(struct part_parm *pparm_tbl, struct mbr_dram *pmbr_dram_tbl);
int nvt_test_gen_mbr(struct part_parm *ppart_tbl, struct mbr_dram *dram_tbl);
int nvt_write_mbr_dram_tbl_to_disk(struct mbr_dram *pmbr_dram_tbl);
int nvt_dos_part_conv_fs_part(struct nvt_dos_partition *pdostbl, struct nvt_fs_partition *pfstbl);
int nvt_dram_mbr_conv_fs_part(struct mbr_dram *pmbr_tbl, struct nvt_fs_partition *pfstbl);
int nvt_list_partition(struct nvt_fs_partition *pfstbl);
int nvt_write_fs_image(char *img_buff, unsigned int img_size, unsigned int part_id, struct nvt_fs_partition *pfstbl);
#endif
