/*
 *  board/novatek/evb668/nt72668_emmc_nvt_img.c
 *
 *  Author:	Alvin lin
 *  Created:	Feb 2, 2014
 *  Copyright:	Novatek Inc.
 *
 */


/*
	temp table for 656
*/
static struct part_parm nvt_part_parm_tbl[MAX_ALLOW_PART_NUM] = {
	// sector for MBR and reseve // 1MB	MBR gap	=>2048 sector
	{
		//binary partition			//just use as reserve 
		.id = 0,
		//.sectors = (9 * 1024 * 2),//9MB, count in sectors	//
		.sectors = (2096),//9MB, count in sectors	//
	},
	{						//for xboot default start value is 4144
		//ker0
		.id = 1,
		.sectors = (24 * 1024 * 2),//24MB, count in sectors
	},
	{
		//ker1
		.id = 2,
		.sectors = (6 * 1024 * 2),//24MB, count in sectors
	},
	{
		//misc
		.id = 3,
		.sectors = (2 * 1024 * 2),//2MB, count in sectors
	},
	{
		//fs0
		.id = 4,
		.sectors = (2 * 1024 * 2),
	},
	{
		//fs1
		.id = 5,
		.sectors = (100 * 1024 * 2),
	},
	{
		//ap0 , android cache partition
		.id = 6,
		.sectors = (500 * 1024 * 2),
	},
	{
		//ap1
		.id = 7,
		.sectors = (500 * 1024 * 2),
	},
	{
		//apdat
		.id = 8,
		.sectors = (100 * 1024 * 2),
	},
	{
		//user
		.id = 9,
		.sectors = 0,//0 means use rest all
	},

};

static nvt_emmc_image_info image_info[] = {
	{
		//mloader = mloader0(16KB) + sbin(2KB) + stbc(2KB) + mloader1(32KB)
		.user_part_id = INVAILD_USER_PART_ID,
		.image_name = "mloader",
		.default_name = "mloader.bin",
		.part_type = PART_TYPE_BOOT1,
		.image_help = nvt_emmc_image_help,
		.read_img = nvt_emmc_read_img,
		.write_img = nvt_emmc_write_img,
		.erase_img = nvt_emmc_erase_img,
		.part_offset = 0,
		.size = 104
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
		.part_offset = 104,
		.size = 97
	},
	{
		//stbc
		.user_part_id = 0,
		.image_name = "stbc",
		.default_name = "stbc.bin",
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
		//logo
		.user_part_id = 0,
		.image_name = "logo",
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
		.user_part_id = 0,
		.image_name = "music",
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
		.user_part_id = 0,
		.image_name = "stbcdat",
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
		//secos
		.user_part_id = 0,
		.image_name = "secos",
		.default_name = "secos.bin",
		.part_type = PART_TYPE_BIN,
		.image_help = nvt_emmc_image_help,
		.read_img = nvt_emmc_read_secos_img,
		.write_img = nvt_emmc_write_img,
		.erase_img = nvt_emmc_erase_img,
		.part_offset = 16384,
		.size = 2047
	},
	{
		//kernel 0
		.user_part_id = 1,
		.image_name = "ker0",
		.default_name = "boot.img",
		.part_type = PART_TYPE_BIN,
		.image_help = nvt_emmc_image_help,
		.read_img = nvt_emmc_read_android_boot_img,
		.write_img = nvt_emmc_write_img,
		.erase_img = nvt_emmc_erase_img,
		.part_offset = 0,
		.size = 43008
	},
	{
		//kernel 1
		.user_part_id = 2,
		.image_name = "ker1",
		.default_name = "recovery.img",
		.part_type = PART_TYPE_BIN,
		.image_help = nvt_emmc_image_help,
		.read_img = nvt_emmc_read_android_boot_img,
		.write_img = nvt_emmc_write_img,
		.erase_img = nvt_emmc_erase_img,
		.part_offset = 0,
		.size = 43008
	},
	{
		//MISC
		.user_part_id = 3,
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
		.user_part_id = 4,
		.image_name = "fs0",
		.default_name = "system_ext4.img",
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
		.user_part_id = 5,
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
		.user_part_id = 6,
		.image_name = "ap0",
		.default_name = "cache.img",
		.part_type = PART_TYPE_FS,
		.image_help = nvt_emmc_image_help,
		.read_img = nvt_emmc_read_img,
		.write_img = nvt_emmc_write_img,
		.erase_img = nvt_emmc_erase_img,
		.part_offset = 0,
		.size = 1024000
	},
	{
		//ap1
		.user_part_id = 7,
		.image_name = "ap1",
		.default_name = "ap1.img",
		.part_type = PART_TYPE_FS,
		.image_help = nvt_emmc_image_help,
		.read_img = nvt_emmc_read_img,
		.write_img = nvt_emmc_write_img,
		.erase_img = nvt_emmc_erase_img,
		.part_offset = 0,
		.size = 1024000
	},
	{
		//apdat
		.user_part_id = 8,
		.image_name = "apdat",
		.default_name = "apdat.img",
		.part_type = PART_TYPE_FS,
		.image_help = nvt_emmc_image_help,
		.read_img = nvt_emmc_read_img,
		.write_img = nvt_emmc_write_img,
		.erase_img = nvt_emmc_erase_img,
		.part_offset = 0,
		.size = 20480
	},
	{
		//user data
		.user_part_id = 9,
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


