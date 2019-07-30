/*
 *  board/novatek/evb668/nt72668_emmc_nvt_img.c
 *
 *  Author:	Alvin lin
 *  Created:	Jun 5, 2015
 *  Copyright:	Novatek Inc.
 *
 */
static struct part_parm nvt_part_parm_tbl[MAX_ALLOW_PART_NUM] = {
	{
		//binary partition  -> /dev/mmcblk0p1
		.id = 0,
		.sectors = (24 * 1024 * 2),//24MB, count in sectors
	},
	{
		//binary partition backup for BSP -> /dev/mmcblk0p2
		.id = 1,
		.sectors = (24 * 1024 * 2),//24MB, count in sectors
	},
	{
		//ker0 -> /dev/mmcblk0p3
		.id = 2,
		.sectors = (24 * 1024 * 2),//24MB, count in sectors
	},
	{
		//ker1 -> /dev/mmcblk0p5
		.id = 3,
		.sectors = (50 * 1024 * 2),//50MB, count in sectors
	},
	{
		//misc -> /dev/mmcblk0p6
		.id = 4,
		.sectors = (2 * 1024 * 2),//2MB, count in sectors
	},
	{
		//fs0 -> /dev/mmcblk0p7  system_ext4.img
		.id = 5,
		.sectors = (1000 * 1024 * 2),
	},
	{
		//fs1 -> /dev/mmcblk0p8
		.id = 6,
		.sectors = (200 * 1024 * 2),
	},
	{
		//ap0 , android cache partition -> /dev/mmcblk0p9
		.id = 7,
		.sectors = (800 * 1024 * 2),
	},
	{
		//ap1 -> /dev/mmcblk0p10
		.id = 8,
		.sectors = (1 * 1024 * 2),
	},
	{
		//apdat -> /dev/mmcblk0p11
		.id = 9,
		.sectors = (50 * 1024 * 2),
	},
	{
		//user -> /dev/mmcblk0p12
		.id = 10,
		.sectors = 0,//0 means use rest all
	},
};

static nvt_emmc_image_info image_info[] = {
	{
		//mloader = xboot0(16KB) + sbin(2KB) + stbc(2KB) + xboot1(32KB)
		.user_part_id = INVAILD_USER_PART_ID,
		.image_name = "xboot",
		.default_name = "xboot.bin",
		.part_type = PART_TYPE_BOOT1,
		.image_help = nvt_emmc_image_help,
		.read_img = nvt_emmc_read_img,
		.write_img = nvt_emmc_write_img,
		.erase_img = nvt_emmc_erase_img,
		.part_offset = 0,
		.size = 256
	},
	{
		//ddr cfg 48KB + 1 block for sign
		.user_part_id = INVAILD_USER_PART_ID,
		.image_name = "ddrcfg",
		.default_name = "668_ddr_cfg.ddr",
		.part_type = PART_TYPE_BOOT1,
		.image_help = nvt_emmc_image_help,
		.read_img = nvt_emmc_read_img,
		.write_img = nvt_emmc_write_img,
		.erase_img = nvt_emmc_erase_img,
		.part_offset = 256,
		.size = 97
	},
	{
		//mloader = xboot0(16KB) + sbin(2KB) + stbc(2KB) + xboot1(32KB)
		.user_part_id = INVAILD_USER_PART_ID,
		.image_name = "xboot_bk",
		.default_name = "xboot.bin",
		.part_type = PART_TYPE_BOOT2,
		.image_help = nvt_emmc_image_help,
		.read_img = nvt_emmc_read_img,
		.write_img = nvt_emmc_write_img,
		.erase_img = nvt_emmc_erase_img,
		.part_offset = 0,
		.size = 256
	},
	{
		//ddr cfg 48KB + 1 block for sign
		.user_part_id = INVAILD_USER_PART_ID,
		.image_name = "ddrcfg_bk",
		.default_name = "668_ddr_cfg.ddr",
		.part_type = PART_TYPE_BOOT2,
		.image_help = nvt_emmc_image_help,
		.read_img = nvt_emmc_read_img,
		.write_img = nvt_emmc_write_img,
		.erase_img = nvt_emmc_erase_img,
		.part_offset = 256,
		.size = 97
	},
	{
		//stbc
		.user_part_id = 0,
		.image_name = "stbc",
		.default_name = "stbc_emmc.bin",
		.part_type = PART_TYPE_BIN,
		.image_help = nvt_emmc_image_help,
		.read_img = nvt_emmc_read_img,
		.write_img = nvt_emmc_write_img,
		.erase_img = nvt_emmc_erase_img,
		.part_offset = 1,
		.size = 4096
	},
	{
		//uboot
		.user_part_id = 0,
		.image_name = "uboot",
		.default_name = "u-boot.bin",
		.part_type = PART_TYPE_BIN,
		.image_help = nvt_emmc_image_help,
		.read_img = nvt_emmc_read_img,
		.write_img = nvt_emmc_write_img,
		.erase_img = nvt_emmc_erase_img,
		.part_offset = 4097,
		.size = 1280
	},
	{
		//uboot data
		.user_part_id = 0,
		.image_name = "ubootdat",
		.default_name = "ubootdat.bin",
		.part_type = PART_TYPE_BIN,
		.image_help = nvt_emmc_image_help,
		.read_img = nvt_emmc_read_img,
		.write_img = nvt_emmc_write_img,
		.erase_img = nvt_emmc_erase_img,
		.part_offset = 5377 ,
		.size = 16
	},
	{
		//xboot data
		.user_part_id = 0,
		.image_name = "xbootdat",
		.default_name = "xbootdat.bin",
		.part_type = PART_TYPE_BIN,
		.image_help = nvt_emmc_image_help,
		.read_img = nvt_emmc_read_img,
		.write_img = nvt_emmc_write_img,
		.erase_img = nvt_emmc_erase_img,
		.part_offset = 5393 ,
		.size = 128
	},
	{
		//logo
		.user_part_id = 0,
		.image_name = "logo",
		.default_name = "logo.jpg",
		.part_type = PART_TYPE_BIN,
		.image_help = nvt_emmc_image_help,
		.read_img = nvt_emmc_read_img,
		.write_img = nvt_emmc_write_img,
		.erase_img = nvt_emmc_erase_img,
		.part_offset = 5521,
		.size = 2048
	},
	{
		//music
		.user_part_id = 0,
		.image_name = "music",
		.default_name = "music.bin",
		.part_type = PART_TYPE_BIN,
		.image_help = nvt_emmc_image_help,
		.read_img = nvt_emmc_read_img,
		.write_img = nvt_emmc_write_img,
		.erase_img = nvt_emmc_erase_img,
		.part_offset = 7569,
		.size = 2048
	},
	{
		//stbc data
		.user_part_id = 0,
		.image_name = "stbcdat",
		.default_name = "stbcdat.bin",
		.part_type = PART_TYPE_BIN,
		.image_help = nvt_emmc_image_help,
		.read_img = nvt_emmc_read_img,
		.write_img = nvt_emmc_write_img,
		.erase_img = nvt_emmc_erase_img,
		.part_offset = 9617,
		.size = 1
	},
	{
		//secos
		.user_part_id = 0,
		.image_name = "secos",
		.default_name = "secos.bin",
		.part_type = PART_TYPE_BIN,
		.image_help = nvt_emmc_image_help,
		.read_img = nvt_emmc_read_secos_img,
		.write_img = nvt_emmc_write_img,
		.erase_img = nvt_emmc_erase_img,
		.part_offset = 14328,
		.size = 2048
	},
	{
		//stbc backup
		.user_part_id = 1,
		.image_name = "stbc_bk",
		.default_name = "stbc_emmc.bin",
		.part_type = PART_TYPE_BIN,
		.image_help = nvt_emmc_image_help,
		.read_img = nvt_emmc_read_img,
		.write_img = nvt_emmc_write_img,
		.erase_img = nvt_emmc_erase_img,
		.part_offset = 0,
		.size = 4096
	},
	{
		//uboot backup
		.user_part_id = 1,
		.image_name = "uboot_bk",
		.default_name = "uboot.bin",
		.part_type = PART_TYPE_BIN,
		.image_help = nvt_emmc_image_help,
		.read_img = nvt_emmc_read_img,
		.write_img = nvt_emmc_write_img,
		.erase_img = nvt_emmc_erase_img,
		.part_offset = 4097,
		.size = 1280
	},
	{
		//uboot data backup
		.user_part_id = 1,
		.image_name = "ubootdat_bk",
		.default_name = "ubootdat.bin",
		.part_type = PART_TYPE_BIN,
		.image_help = nvt_emmc_image_help,
		.read_img = nvt_emmc_read_img,
		.write_img = nvt_emmc_write_img,
		.erase_img = nvt_emmc_erase_img,
		.part_offset = 5377 ,
		.size = 16
	},
	{
		//xboot data backup
		.user_part_id = 1,
		.image_name = "xbootdat",
		.default_name = "xbootdat.bin",
		.part_type = PART_TYPE_BIN,
		.image_help = nvt_emmc_image_help,
		.read_img = nvt_emmc_read_img,
		.write_img = nvt_emmc_write_img,
		.erase_img = nvt_emmc_erase_img,
		.part_offset = 5393 ,
		.size = 128
	},
	{
		//logo backup
		.user_part_id = 1,
		.image_name = "logo",
		.default_name = "logo.jpg",
		.part_type = PART_TYPE_BIN,
		.image_help = nvt_emmc_image_help,
		.read_img = nvt_emmc_read_img,
		.write_img = nvt_emmc_write_img,
		.erase_img = nvt_emmc_erase_img,
		.part_offset = 5521,
		.size = 2048
	},
	{
		//music backup
		.user_part_id = 1,
		.image_name = "music",
		.default_name = "music.bin",
		.part_type = PART_TYPE_BIN,
		.image_help = nvt_emmc_image_help,
		.read_img = nvt_emmc_read_img,
		.write_img = nvt_emmc_write_img,
		.erase_img = nvt_emmc_erase_img,
		.part_offset = 7569,
		.size = 2048
	},
	{
		//stbc data backup
		.user_part_id = 1,
		.image_name = "stbcdat_bk",
		.default_name = "stbcdat.bin",
		.part_type = PART_TYPE_BIN,
		.image_help = nvt_emmc_image_help,
		.read_img = nvt_emmc_read_img,
		.write_img = nvt_emmc_write_img,
		.erase_img = nvt_emmc_erase_img,
		.part_offset = 9617,
		.size = 1
	},
	{
		//secos backup
		.user_part_id = 0,
		.image_name = "secos_bk",
		.default_name = "secos.bin",
		.part_type = PART_TYPE_BIN,
		.image_help = nvt_emmc_image_help,
		.read_img = nvt_emmc_read_secos_img,
		.write_img = nvt_emmc_write_img,
		.erase_img = nvt_emmc_erase_img,
		.part_offset = 14328,
		.size = 2048
	},

	{
		//kernel 0
		.user_part_id = 2,
		.image_name = "ker0",
		.default_name = "boot.img",
		.part_type = PART_TYPE_BIN,
		.image_help = nvt_emmc_image_help,
		.read_img = nvt_emmc_read_android_boot_img,
		.write_img = nvt_emmc_write_img,
		.erase_img = nvt_emmc_erase_img,
		.part_offset = 0,
		.size = 0
	},
	{
		//kernel 1
		.user_part_id = 3,
		.image_name = "ker1",
		.default_name = "recovery.img",
		.part_type = PART_TYPE_BIN,
		.image_help = nvt_emmc_image_help,
		.read_img = nvt_emmc_read_android_boot_img,
		.write_img = nvt_emmc_write_img,
		.erase_img = nvt_emmc_erase_img,
		.part_offset = 0,
		.size = 0
	},
	{
		//MISC
		.user_part_id = 4,
		.image_name = "misc",
		.default_name = "misc.bin",
		.part_type = PART_TYPE_BIN,
		.image_help = nvt_emmc_image_help,
		.read_img = nvt_emmc_read_img,
		.write_img = nvt_emmc_write_img,
		.erase_img = nvt_emmc_erase_img,
		.part_offset = 0,
		.size = 0
	},
	{
		//system
		.user_part_id = 5,
		.image_name = "system",
		.default_name = "system_ext4.img",
		.part_type = PART_TYPE_FS,
		.image_help = nvt_emmc_image_help,
		.read_img = nvt_emmc_read_img,
		.write_img = nvt_emmc_write_img,
		.erase_img = nvt_emmc_erase_img,
		.part_offset = 0,
		.size = 0
	},
	{
		//fs1
		.user_part_id = 6,
		.image_name = "fs1",
		.default_name = "hdal_rootfs.img",
		.part_type = PART_TYPE_FS,
		.image_help = nvt_emmc_image_help,
		.read_img = nvt_emmc_read_img,
		.write_img = nvt_emmc_write_img,
		.erase_img = nvt_emmc_erase_img,
		.part_offset = 0,
		.size = 0

	},
	{
		//ap0
		.user_part_id = 7,
		.image_name = "cache",
		.default_name = "cache.img",
		.part_type = PART_TYPE_FS,
		.image_help = nvt_emmc_image_help,
		.read_img = nvt_emmc_read_img,
		.write_img = nvt_emmc_write_img,
		.erase_img = nvt_emmc_erase_img,
		.part_offset = 0,
		.size = 0
	},
	{
		//ap1
		.user_part_id = 8,
		.image_name = "ap1",
		.default_name = "ap1.img",
		.part_type = PART_TYPE_BIN,
		.image_help = nvt_emmc_image_help,
		.read_img = nvt_emmc_read_img,
		.write_img = nvt_emmc_write_img,
		.erase_img = nvt_emmc_erase_img,
		.part_offset = 0,
		.size = 0
	},
	{
		//apdat
		.user_part_id = 9,
		.image_name = "apdat",
		.default_name = "apdat.img",
		.part_type = PART_TYPE_FS,
		.image_help = nvt_emmc_image_help,
		.read_img = nvt_emmc_read_img,
		.write_img = nvt_emmc_write_img,
		.erase_img = nvt_emmc_erase_img,
		.part_offset = 0,
		.size = 0
	},
	{
		//user data
		.user_part_id = 10,
		.image_name = "user",
		.default_name = "user.img",
		.part_type = PART_TYPE_FS,
		.image_help = nvt_emmc_image_help,
		.read_img = nvt_emmc_read_img,
		.write_img = nvt_emmc_write_img,
		.erase_img = nvt_emmc_erase_img,
		.part_offset = 0,
		.size = 0
	}
};	


