/*
 *  board/novatek/evb668/nt72668_emmc_sri_img.c
 *
 *  Author:	Alvin lin
 *  Created:	Feb 2, 2014
 *  Copyright:	Novatek Inc.
 *
 */
static struct part_parm nvt_part_parm_tbl[MAX_ALLOW_PART_NUM] = {
	{
		//binary partition
		.id = 0,
		.sectors = (24 * 1024 * 2),//12MB, count in sectors
	},
	{
		//binary partition
		.id = 1,
		.sectors = (24 * 1024 * 2),//12MB, count in sectors
	},
	{
		//ker0
		.id = 2,
		.sectors = (24 * 1024 * 2),//24MB, count in sectors
	},
	{
		//ker1
		.id = 3,
		.sectors = (48 * 1024 * 2),//24MB, count in sectors
	},
	{
		//misc
		.id = 4,
		.sectors = (8 * 1024 * 2),//2MB, count in sectors
	},
	{
		//fs0
		.id = 5,
		.sectors = (500 * 1024 * 2),
	},
	{
		//fs1
		.id = 6,
		.sectors = (500 * 1024 * 2),
	},
	{
		//ap0 , android cache partition
		.id = 7,
		.sectors = (1024 * 1024 * 2),
	},
	{
		//ap1
		.id = 8,
		.sectors = (1024 * 1024 * 2),
	},
	{
		//ro area
		.id = 9,
		.sectors = (1024 * 1024 * 2),
	},
	{
		//rw area
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
		.default_name = "logo.bin",
		.part_type = PART_TYPE_BIN,
		.image_help = nvt_emmc_image_help,
		.read_img = nvt_emmc_read_img,
		.write_img = nvt_emmc_write_img,
		.erase_img = nvt_emmc_erase_img,
		.part_offset = 5521,
		.size = 8807
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
		//stbc
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
		//uboot
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
		//uboot data
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
		//logo
		.user_part_id = 1,
		.image_name = "logo_bk",
		.default_name = "logo.bin",
		.part_type = PART_TYPE_BIN,
		.image_help = nvt_emmc_image_help,
		.read_img = nvt_emmc_read_img,
		.write_img = nvt_emmc_write_img,
		.erase_img = nvt_emmc_erase_img,
		.part_offset = 5393,
		.size = 10240
	},
	{
		//boot music
		.user_part_id = 1,
		.image_name = "music_bk",
		.default_name = "music.bin",
		.part_type = PART_TYPE_BIN,
		.image_help = nvt_emmc_image_help,
		.read_img = nvt_emmc_read_img,
		.write_img = nvt_emmc_write_img,
		.erase_img = nvt_emmc_erase_img,
		.part_offset = 15633,
		.size = 1
	},
	{
		//stbc data
		.user_part_id = 1,
		.image_name = "stbcdat_bk",
		.default_name = "stbcdat.bin",
		.part_type = PART_TYPE_BIN,
		.image_help = nvt_emmc_image_help,
		.read_img = nvt_emmc_read_img,
		.write_img = nvt_emmc_write_img,
		.erase_img = nvt_emmc_erase_img,
		.part_offset = 15634,
		.size = 1
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
		.size = 47104
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
		.size = 47104
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
		.size = 4096
	},
	{
		//fs0
		.user_part_id = 5,
		.image_name = "fs0",
#if defined(CONFIG_NFSK)
		.default_name = "rootfs_nfsb.img",
#else		
		.default_name = "rootfs.img",
#endif		
		.part_type = PART_TYPE_FS,
		.image_help = nvt_emmc_image_help,
		.read_img = nvt_emmc_read_img,
		.write_img = nvt_emmc_write_img,
		.erase_img = nvt_emmc_erase_img,
		.part_offset = 0,
		.size = 1024000
	},
	{
		//fs1
		.user_part_id = 6,
		.image_name = "fs1",
		.default_name = "rootfs_lite.img",
		.part_type = PART_TYPE_FS,
		.image_help = nvt_emmc_image_help,
		.read_img = nvt_emmc_read_img,
		.write_img = nvt_emmc_write_img,
		.erase_img = nvt_emmc_erase_img,
		.part_offset = 0,
		.size = 1024000

	},
	{
		//ap0
		.user_part_id = 7,
		.image_name = "ap0",
#if defined(CONFIG_NFSK)
		.default_name = "apexe_nfsb.img",
#else		
		.default_name = "apexe.img",
#endif		
		.part_type = PART_TYPE_FS,
		.image_help = nvt_emmc_image_help,
		.read_img = nvt_emmc_read_img,
		.write_img = nvt_emmc_write_img,
		.erase_img = nvt_emmc_erase_img,
		.part_offset = 0,
		.size = 2095000
	},
	{
		//ap1
		.user_part_id = 8,
		.image_name = "ap1",
#if defined(CONFIG_NFSK)
		.default_name = "apexe_nfsb.img",
#else		
		.default_name = "apexe.img",
#endif		
		.part_type = PART_TYPE_FS,
		.image_help = nvt_emmc_image_help,
		.read_img = nvt_emmc_read_img,
		.write_img = nvt_emmc_write_img,
		.erase_img = nvt_emmc_erase_img,
		.part_offset = 0,
		.size = 2095000
	},
	{
		//ro
		.user_part_id = 9,
		.image_name = "ro_area",
		.default_name = "ro_area.img",
		.part_type = PART_TYPE_FS,
		.image_help = nvt_emmc_image_help,
		.read_img = nvt_emmc_read_img,
		.write_img = nvt_emmc_write_img,
		.erase_img = nvt_emmc_erase_img,
		.part_offset = 0,
		.size = 2095000
	},
	{
		//user data
		.user_part_id = 10,
		.image_name = "rw_area",
		.default_name = "rw_area.img",
		.part_type = PART_TYPE_FS,
		.image_help = nvt_emmc_image_help,
		.read_img = nvt_emmc_read_img,
		.write_img = nvt_emmc_write_img,
		.erase_img = nvt_emmc_erase_img,
		.part_offset = 0,
		.size = 0
	}
};	


