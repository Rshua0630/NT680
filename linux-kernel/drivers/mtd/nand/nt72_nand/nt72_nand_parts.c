#include <linux/mtd/partitions.h>
#include "include/nt72_nand_config.h"
#include "include/nt72_nand.h"

#define SZ_16                           0x00000010
#define SZ_256                          0x00000100
#define SZ_512                          0x00000200

#define SZ_1K                           0x00000400
#define SZ_4K                           0x00001000
#define SZ_8K                           0x00002000
#define SZ_16K                          0x00004000
#define SZ_32K                          0x00008000
#define SZ_64K                          0x00010000
#define SZ_128K                         0x00020000
#define SZ_256K                         0x00040000
#define SZ_512K                         0x00080000

#define SZ_1M                           0x00100000
#define SZ_2M                           0x00200000
#define SZ_4M                           0x00400000
#define SZ_8M                           0x00800000
#define SZ_16M                          0x01000000
#define SZ_32M                          0x02000000
#define SZ_48M                          0x03000000
#define SZ_64M                          0x04000000
#define SZ_128M                         0x08000000
#define SZ_256M                         0x10000000
#define SZ_512M                         0x20000000

#define SZ_1G                           0x40000000
#define SZ_2G                           0x80000000

#if defined(CONFIG_BOARD_SKYWORTH)
struct mtd_partition g_nt72_nand_mtd_partitions[] = {
	[0] =  {
		.name	= "uboot",
		.offset = 0,
		.size	= SZ_128K * 5,
	},
	[1] =  {
		.name	= "ubootdat",
		.offset = MTDPART_OFS_APPEND,
		.size	= SZ_128K * 2,
	},
	[2] =  {
		.name	= "stbc",
		.offset = MTDPART_OFS_APPEND,
		.size	= SZ_128K * 3,
	},
	[3] =  {
		.name	= "stbcdat",
		.offset = MTDPART_OFS_APPEND,
		.size	= SZ_128K * 3,
	},
	[4] =  {
		.name	= "kr0",
		.offset = MTDPART_OFS_APPEND,
		.size	= SZ_128K * 40,
	},
	[5] =  {
		.name	= "fs0",
		.offset = MTDPART_OFS_APPEND,
		.size	= SZ_128K * 100,
	},
	[6] =  {
		.name	= "kr1",
		.offset = MTDPART_OFS_APPEND,
		.size	= SZ_128K * 40,
	},
	[7] =  {
		.name	= "fs1",
		.offset = MTDPART_OFS_APPEND,
		.size	= SZ_128K * 100,
	},
	[8] =  {
		.name	= "ap0",
		.offset = MTDPART_OFS_APPEND,
		.size	= SZ_128K * 640,
	},
	[9] =  {
		.name	= "apdat",
		.offset = MTDPART_OFS_APPEND,
		.size	= SZ_128K * 32,
	},
	[10] = {
		.name	= "logo",
		.offset = MTDPART_OFS_APPEND,
		.size	= SZ_128K * 40,
	},
	[11] = {
		.name	= "music",
		.offset = MTDPART_OFS_APPEND,
		.size	= SZ_128K * 12,
	},
	[12] = {
		.name	= "ap",
		.offset = MTDPART_OFS_APPEND,
		.size	= SZ_128K * (1024 + 3),
	},
};
#elif defined(CONFIG_BOARD_TPV_P2800) || defined(CONFIG_BOARD_EVB_NAND128)
struct mtd_partition g_nt72_nand_mtd_partitions[] = {
	[0] =  {
		.name	= "uboot",
		.offset = 0,
		.size	= SZ_128K * 7,
	},
	[1] =  {
		.name	= "ubootdat",
		.offset = MTDPART_OFS_APPEND,
		.size	= SZ_128K * 4,
	},
	[2] =  {
		.name	= "stbc",
		.offset = MTDPART_OFS_APPEND,
		.size	= SZ_128K * 5,
	},
	[3] =  {
		.name	= "stbcdat",
		.offset = MTDPART_OFS_APPEND,
		.size	= SZ_128K * 3,
	},
	[4] =  {
		.name	= "kr0",
		.offset = MTDPART_OFS_APPEND,
		.size	= SZ_128K * 40,
	},
	[5] =  {
		.name	= "fs0",
		.offset = MTDPART_OFS_APPEND,
		.size	= SZ_128K * 80,
	},
	[6] =  {
		.name	= "kr1",
		.offset = MTDPART_OFS_APPEND,
		.size	= SZ_128K * 40,
	},
	[7] =  {
		.name	= "fs1",
		.offset = MTDPART_OFS_APPEND,
		.size	= SZ_128K * 80,
	},
	[8] =  {
		.name	= "ap0",
		.offset = MTDPART_OFS_APPEND,
		.size	= SZ_128K * 320,
	},
	[9] =  {
		.name	= "ap1",
		.offset = MTDPART_OFS_APPEND,
		.size	= SZ_128K * 320,
	},
	[10] = {
		.name	= "apdat",
		.offset = MTDPART_OFS_APPEND,
		.size	= SZ_128K * 72,
	},
	[11] = {
		.name	= "logo",
		.offset = MTDPART_OFS_APPEND,
		.size	= SZ_128K * 10,
	},
	[12] = {
		.name	= "music",
		.offset = MTDPART_OFS_APPEND,
		.size	= SZ_128K * 3,
	},
	[13] = {
		.name	= "pridat",
		.offset = MTDPART_OFS_APPEND,
		.size	= SZ_128K * 40,
	},
};
#elif defined(CONFIG_BOARD_N68001_N22B1_453B_NAND128) || \
defined(CONFIG_BOARD_N68001_N68B1_453B_NAND128)
struct mtd_partition g_nt72_nand_mtd_partitions[] = {
	[0] =  {
		.name	= "uboot",
		.offset = 0,
		.size	= SZ_128K * 7,
	},
	[1] =  {
		.name	= "ubootdat",
		.offset = MTDPART_OFS_APPEND,
		.size	= SZ_128K * 4,
	},
	[2] =  {
		.name	= "stbc",
		.offset = MTDPART_OFS_APPEND,
		.size	= SZ_128K * 5,
	},
	[3] =  {
		.name	= "stbcdat",
		.offset = MTDPART_OFS_APPEND,
		.size	= SZ_128K * 3,
	},
	[4] =  {
		.name	= "kr0",
		.offset = MTDPART_OFS_APPEND,
		.size	= SZ_128K * 20,
	},
	[5] =  {
		.name	= "fs0",
		.offset = MTDPART_OFS_APPEND,
		.size	= SZ_128K * 76,
	},
	[6] =  {
		.name	= "kr1",
		.offset = MTDPART_OFS_APPEND,
		.size	= SZ_128K * 20,
	},
	[7] =  {
		.name	= "fs1",
		.offset = MTDPART_OFS_APPEND,
		.size	= SZ_128K * 76,
	},
	[8] =  {
		.name	= "ap0",
		.offset = MTDPART_OFS_APPEND,
		.size	= SZ_128K * 293,
	},
	[9] =  {
		.name	= "ap1",
		.offset = MTDPART_OFS_APPEND,
		.size	= SZ_128K * 293,
	},
	[10] = {
		.name	= "factorydata",
		.offset = MTDPART_OFS_APPEND,
		.size	= SZ_128K * 32,
	},

	[11] = {
		.name	= "apdat0",
		.offset = MTDPART_OFS_APPEND,
		.size	= SZ_128K * 72,
	},
	[12] = {
		.name	= "apdat1",
		.offset = MTDPART_OFS_APPEND,
		.size	= SZ_128K * 72,
	},

	[13] = {
		.name	= "logo",
		.offset = MTDPART_OFS_APPEND,
		.size	= SZ_128K * 11,
	},

	[14] = {
		.name	= "pridat",
		.offset = MTDPART_OFS_APPEND,
		.size	= SZ_128K * 36,
	},
};
#elif defined(CONFIG_BOARD_TPV_N22050_453B_NAND128)
struct mtd_partition g_nt72_nand_mtd_partitions[] = {
	[0] =  {
		.name	= "uboot",
		.offset = 0,
		.size	= SZ_128K * 7,
	},
	[1] =  {
		.name	= "ubootdat",
		.offset = MTDPART_OFS_APPEND,
		.size	= SZ_128K * 4,
	},
	[2] =  {
		.name	= "stbc",
		.offset = MTDPART_OFS_APPEND,
		.size	= SZ_128K * 5,
	},
	[3] =  {
		.name	= "stbcdat",
		.offset = MTDPART_OFS_APPEND,
		.size	= SZ_128K * 3,
	},
	[4] =  {
		.name	= "kr0",
		.offset = MTDPART_OFS_APPEND,
		.size	= SZ_128K * 40,
	},
	[5] =  {
		.name	= "fs0",
		.offset = MTDPART_OFS_APPEND,
		.size	= SZ_128K * 80,
	},
	[6] =  {
		.name	= "kr1",
		.offset = MTDPART_OFS_APPEND,
		.size	= SZ_128K * 40,
	},
	[7] =  {
		.name	= "fs1",
		.offset = MTDPART_OFS_APPEND,
		.size	= SZ_128K * 80,
	},
	[8] =  {
		.name	= "ap0",
		.offset = MTDPART_OFS_APPEND,
		.size	= SZ_128K * 320,
	},
	[9] =  {
		.name	= "ap1",
		.offset = MTDPART_OFS_APPEND,
		.size	= SZ_128K * 320,
	},
	[10] = {
		.name	= "apdat",
		.offset = MTDPART_OFS_APPEND,
		.size	= SZ_128K * 72,
	},
	[11] = {
		.name	= "logo",
		.offset = MTDPART_OFS_APPEND,
		.size	= SZ_128K * 10,
	},
	[12] = {
		.name	= "music",
		.offset = MTDPART_OFS_APPEND,
		.size	= SZ_128K * 3,
	},
	[13] = {
		.name	= "pridat",
		.offset = MTDPART_OFS_APPEND,
		.size	= SZ_128K * 30,
	},
};

#elif defined(CONFIG_BOARD_TPV_N22050_453B_NAND128_RELEASE)
struct mtd_partition g_nt72_nand_mtd_partitions[] = {
	[0] =  {
		.name	= "uboot",
		.offset = 0,
		.size	= SZ_128K * 7,
	},
	[1] =  {
		.name	= "ubootdat",
		.offset = MTDPART_OFS_APPEND,
		.size	= SZ_128K * 4,
	},
	[2] =  {
		.name	= "stbc",
		.offset = MTDPART_OFS_APPEND,
		.size	= SZ_128K * 5,
	},
	[3] =  {
		.name	= "stbcdat",
		.offset = MTDPART_OFS_APPEND,
		.size	= SZ_128K * 3,
	},
	[4] =  {
		.name	= "kr0",
		.offset = MTDPART_OFS_APPEND,
		.size	= SZ_128K * 15,
	},
	[5] =  {
		.name	= "fs0",
		.offset = MTDPART_OFS_APPEND,
		.size	= SZ_128K * 80,
	},
	[6] =  {
		.name	= "kr1",
		.offset = MTDPART_OFS_APPEND,
		.size	= SZ_128K * 15,
	},
	[7] =  {
		.name	= "fs1",
		.offset = MTDPART_OFS_APPEND,
		.size	= SZ_128K * 80,
	},
	[8] =  {
		.name	= "ap0",
		.offset = MTDPART_OFS_APPEND,
		.size	= SZ_128K * 320,
	},
	[9] =  {
		.name	= "ap1",
		.offset = MTDPART_OFS_APPEND,
		.size	= SZ_128K * 320,
	},
	[10] = {
		.name	= "apdat",
		.offset = MTDPART_OFS_APPEND,
		.size	= SZ_128K * 72,
	},
	[11] = {
		.name	= "logo",
		.offset = MTDPART_OFS_APPEND,
		.size	= SZ_128K * 10,
	},
	[12] = {
		.name	= "music",
		.offset = MTDPART_OFS_APPEND,
		.size	= SZ_128K * 3,
	},
	[13] = {
		.name	= "pridat",
		.offset = MTDPART_OFS_APPEND,
		.size	= SZ_128K * 30,
	},
};

#elif defined(CONFIG_BOARD_EVB_NAND32)
struct mtd_partition g_nt72_nand_mtd_partitions[] = {
	[0] =  {
		.name	= "uboot",
		.offset = 0,
		.size	= SZ_128K * 5,
	},
	[1] =  {
		.name	= "ubootdat",
		.offset = MTDPART_OFS_APPEND,
		.size	= SZ_128K * 2,
	},
	[2] =  {
		.name	= "stbc",
		.offset = MTDPART_OFS_APPEND,
		.size	= SZ_128K * 3,
	},
	[3] =  {
		.name	= "stbcdat",
		.offset = MTDPART_OFS_APPEND,
		.size	= SZ_128K * 3,
	},
	[4] =  {
		.name	= "kr0",
		.offset = MTDPART_OFS_APPEND,
		.size	= SZ_128K * 16,
	},
	[5] =  {
		.name	= "fs0",
		.offset = MTDPART_OFS_APPEND,
		.size	= SZ_128K * 20,
	},
	[6] =  {
		.name	= "kr1",
		.offset = MTDPART_OFS_APPEND,
		.size	= SZ_128K * 16,
	},
	[7] =  {
		.name	= "fs1",
		.offset = MTDPART_OFS_APPEND,
		.size	= SZ_128K * 20,
	},
	[8] =  {
		.name	= "ap0",
		.offset = MTDPART_OFS_APPEND,
		.size	= SZ_128K * 72,
	},
	[9] =  {
		.name	= "ap1",
		.offset = MTDPART_OFS_APPEND,
		.size	= SZ_128K * 72,
	},
	[10] = {
		.name	= "apdat",
		.offset = MTDPART_OFS_APPEND,
		.size	= SZ_128K * 8,
	},
	[11] = {
		.name	= "logo",
		.offset = MTDPART_OFS_APPEND,
		.size	= SZ_128K * 16,
	},
};
#elif defined(CONFIG_BOARD_TK_NAND256)
struct mtd_partition g_nt72_nand_mtd_partitions[] = {
	[0] =  {
		.name	= "uboot",
		.offset = 0,
		.size	= SZ_128K * 9,
	},
	[1] =  {
		.name	= "ubootdat",
		.offset = MTDPART_OFS_APPEND,
		.size	= SZ_128K * 2,
	},
	[2] =  {
		.name	= "stbc",
		.offset = MTDPART_OFS_APPEND,
		.size	= SZ_128K * 3,
	},
	[3] =  {
		.name	= "stbcdat",
		.offset = MTDPART_OFS_APPEND,
		.size	= SZ_128K * 3,
	},
	[4] =  {
		.name	= "kr0",
		.offset = MTDPART_OFS_APPEND,
		.size	= SZ_128K * 40,
	},
	[5] =  {
		.name	= "fs0",
		.offset = MTDPART_OFS_APPEND,
		.size	= SZ_128K * 100,
	},
	[6] =  {
		.name	= "kr1",
		.offset = MTDPART_OFS_APPEND,
		.size	= SZ_128K * 40,
	},
	[7] =  {
		.name	= "fs1",
		.offset = MTDPART_OFS_APPEND,
		.size	= SZ_128K * 100,
	},
	[8] =  {
		.name	= "ap0",
		.offset = MTDPART_OFS_APPEND,
		.size	= SZ_128K * (320 + 500),
	},
	[9] =  {
		.name	= "ap1",
		.offset = MTDPART_OFS_APPEND,
		.size	= SZ_128K * (320 + 500),
	},
	[10] = {
		.name	= "apdat",
		.offset = MTDPART_OFS_APPEND,
		.size	= SZ_128K * 72,
	},
	[11] = {
		.name	= "logo",
		.offset = MTDPART_OFS_APPEND,
		.size	= SZ_128K * 20,
	},
	[12] = {
		.name	= "music",
		.offset = MTDPART_OFS_APPEND,
		.size	= SZ_128K * 16,
	},
};
#elif defined(CONFIG_BOARD_CVT_EU)
struct mtd_partition g_nt72_nand_mtd_partitions[] = {
	[0] =  {
		.name	= "uboot",
		.offset = 0,
		.size	= SZ_128K * 9,
	},
	[1] =  {
		.name	= "ubootdat",
		.offset = MTDPART_OFS_APPEND,
		.size	= SZ_128K * 2,
	},
	[2] =  {
		.name	= "stbc",
		.offset = MTDPART_OFS_APPEND,
		.size	= SZ_128K * 3,
	},
	[3] =  {
		.name	= "stbcdat",
		.offset = MTDPART_OFS_APPEND,
		.size	= SZ_128K * 3,
	},
	[4] =  {
		.name	= "kr0",
		.offset = MTDPART_OFS_APPEND,
		.size	= SZ_128K * 40,
	},
	[5] =  {
		.name	= "fs0",
		.offset = MTDPART_OFS_APPEND,
		.size	= SZ_128K * 100,
	},
	[6] =  {
		.name	= "kr1",
		.offset = MTDPART_OFS_APPEND,
		.size	= SZ_128K * 40,
	},
	[7] =  {
		.name	= "fs1",
		.offset = MTDPART_OFS_APPEND,
		.size	= SZ_128K * 100,
	},
	[8] =  {
		.name	= "ap0",
		.offset = MTDPART_OFS_APPEND,
		.size	= SZ_128K * (320 + 500),
	},
	[9] =  {
		.name	= "ap1",
		.offset = MTDPART_OFS_APPEND,
		.size	= SZ_128K * (320 + 500),
	},
	[10] = {
		.name	= "apdat",
		.offset = MTDPART_OFS_APPEND,
		.size	= SZ_128K * 72,
	},
	[11] = {
		.name	= "logo",
		.offset = MTDPART_OFS_APPEND,
		.size	= SZ_128K * 20,
	},
	[12] = {
		.name	= "music",
		.offset = MTDPART_OFS_APPEND,
		.size	= SZ_128K * 16,
	},
};
#elif defined(CONFIG_MACH_N68002_NAND_NEW_DRV)
struct mtd_partition g_nt72_nand_mtd_partitions[] = {
	[0] =  {
		.name	= "xboot",
		.offset = 0,
		.size	= SZ_128K * 4,
	},
	[1] =  {
		.name	= "ddrcfg",
		.offset = MTDPART_OFS_APPEND,
		.size	= SZ_128K * 4,
	},
	[2] =  {
		.name	= "ddrbud",
		.offset = MTDPART_OFS_APPEND,
		.size	= SZ_128K * 3,
	},
	[3] =  {
		.name	= "xbootdat",
		.offset = MTDPART_OFS_APPEND,
		.size	= SZ_128K * 5,
	},
	[4] =  {
		.name	= "misc",
		.offset = MTDPART_OFS_APPEND,
		.size	= SZ_128K * 3,
	},
	[5] =  {
		.name	= "stbc",
		.offset = MTDPART_OFS_APPEND,
		.size	= SZ_128K * 5,
	},

	[6] =  {
		.name	= "secos",
		.offset = MTDPART_OFS_APPEND,
		.size	= SZ_128K * 15,
	},

	[7] =  {
		.name	= "uboot",
		.offset = MTDPART_OFS_APPEND,
		.size	= SZ_128K * 7,
	},
	[8] =  {
		.name	= "fdt0",
		.offset = MTDPART_OFS_APPEND,
		.size	= SZ_128K * 3,
	},
	[9] =  {
		.name	= "ker0",
		.offset = MTDPART_OFS_APPEND,
		.size	= SZ_128K * 85,
	},
	[10] = {
		.name	= "fs0",
		.offset = MTDPART_OFS_APPEND,
		.size	= SZ_128K * 130,
	},
	[11] = {
		.name	= "ap0",
		.offset = MTDPART_OFS_APPEND,
		.size	= SZ_128K * 700,
	},
	[12] = {
		.name	= "apdat0",
		.offset = MTDPART_OFS_APPEND,
		.size	= SZ_128K * 72,
	},
	[13] = {
		.name	= "fdt1",
		.offset = MTDPART_OFS_APPEND,
		.size	= SZ_128K * 3,
	},
	[14] = {
		.name	= "ker1",
		.offset = MTDPART_OFS_APPEND,
		.size	= SZ_128K * 85,
	},
	[15] = {
		.name	= "fs1",
		.offset = MTDPART_OFS_APPEND,
		.size	= SZ_128K * 130,
	},
	[16] = {
		.name	= "ap1",
		.offset = MTDPART_OFS_APPEND,
		.size	= SZ_128K * 700,
	},
	[17] = {
		.name	= "apdat1",
		.offset = MTDPART_OFS_APPEND,
		.size	= SZ_128K * 72,
	},
	[18] = {
		.name	= "logo",
		.offset = MTDPART_OFS_APPEND,
		.size	= SZ_128K * 10,
	},
	[19] = {
		.name	= "music",
		.offset = MTDPART_OFS_APPEND,
		.size	= SZ_128K * 3,
	},
};
#elif defined(CONFIG_MACH_EVB658_NAND_NEW_DRV)
struct mtd_partition g_nt72_nand_mtd_partitions[] = {
	[0] =  {
		.name	= "xboot",
		.offset = 0,
		.size	= SZ_128K * 4,
	},
	[1] =  {
		.name	= "ddrcfg",
		.offset = MTDPART_OFS_APPEND,
		.size	= SZ_128K * 4,
	},
	[2] =  {
		.name	= "ddrbud",
		.offset = MTDPART_OFS_APPEND,
		.size	= SZ_128K * 3,
	},
	[3] =  {
		.name	= "xbootdat",
		.offset = MTDPART_OFS_APPEND,
		.size	= SZ_128K * 5,
	},
	[4] =  {
		.name	= "misc",
		.offset = MTDPART_OFS_APPEND,
		.size	= SZ_128K * 3,
	},
	[5] =  {
		.name	= "stbc",
		.offset = MTDPART_OFS_APPEND,
		.size	= SZ_128K * 5,
	},
	[6] =  {
		.name	= "secos",
		.offset = MTDPART_OFS_APPEND,
		.size	= SZ_128K * 15,
	},
	[7] =  {
		.name	= "uboot",
		.offset = MTDPART_OFS_APPEND,
		.size	= SZ_128K * 7,
	},
	[8] =  {
		.name	= "fdt0",
		.offset = MTDPART_OFS_APPEND,
		.size	= SZ_128K * 3,
	},
	[9] =  {
		.name	= "fdt1",
		.offset = MTDPART_OFS_APPEND,
		.size	= SZ_128K * 3,
	},
	[10] = {
		.name	= "ker0",
		.offset = MTDPART_OFS_APPEND,
		.size	= SZ_128K * 85,
	},
	[11] = {
		.name	= "ker1",
		.offset = MTDPART_OFS_APPEND,
		.size	= SZ_128K * 85,
	},
	[12] = {
		.name	= "fs0",
		.offset = MTDPART_OFS_APPEND,
		.size	= SZ_128K * 80,
	},
	[13] = {
		.name	= "fs1",
		.offset = MTDPART_OFS_APPEND,
		.size	= SZ_128K * 80,
	},
	[14] = {
		.name	= "ap0",
		.offset = MTDPART_OFS_APPEND,
		.size	= SZ_128K * 620,
	},
	[15] = {
		.name	= "ap1",
		.offset = MTDPART_OFS_APPEND,
		.size	= SZ_128K * 620,
	},
	[16] = {
		.name	= "apdat0",
		.offset = MTDPART_OFS_APPEND,
		.size	= SZ_128K * 80,
	},
	[17] = {
		.name	= "apdat1",
		.offset = MTDPART_OFS_APPEND,
		.size	= SZ_128K * 80,
	},
};
#else
struct mtd_partition g_nt72_nand_mtd_partitions[] = {
	{
		.name = "",
		.offset = 0,
		.size = SZ_128K * 0,
	},
};
#endif

const unsigned int g_nr_nt72_nand_mtd_partitions =
	ARRAY_SIZE(g_nt72_nand_mtd_partitions);

const char *g_nt72_nand_part_probes[] = { "cmdlinepart", NULL };
const char g_nt72_nand_partition_table_name[] = "nvt_nand";
