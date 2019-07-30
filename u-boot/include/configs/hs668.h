/*
 * Copyright (C) 2014 Novatek Microelectronics Corp. All rights reserved.
 * Author: SP-ISW
 *
 * Configuration settings for the Novatek NT72668 Turnkey board.
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#ifndef __CONFIG_EVB668_H__
#define __CONFIG_EVB668_H__

#ifdef CONFIG_DEBUG
#define DEBUG						1
#endif

/*
 * High Level Configuration Options
 */
#define CONFIG_ARMV7
#define CONFIG_SYS_GENERIC_BOARD
#define CONFIG_NT72xxx				1	/* which is a DTV SOC */
#define CONFIG_NT72668				1	/* which is in a NT72668 */
/*#define CONFIG_EVB_668 			1*/
#define CONFIG_N62001				1
#define CONFIG_BOARD_N62001A		1   /* Disable this line, uboot run in EVB board */

#define CONFIG_FLASH_P3

#define CONFIG_DISPLAY_CPUINFO		1
#define CONFIG_DISPLAY_BOARDINFO	1

#define CONFIG_CPU_FREQ				1000	/* default value, MHz */

#define CONFIG_SYS_HZ				1000

#ifndef CONFIG_SYS_L2CACHE_OFF
#define CONFIG_SYS_L2_PL310			1
#define CONFIG_SYS_PL310_BASE		0xffe00000
#endif

#define CONFIG_BOARD_LATE_INIT

#define CPU_NUM_MAX     4
/*
 * DDR information.  If the CONFIG_NR_DRAM_BANKS is not defined,
 * we say (for simplicity) that we have 1 bank, always, even when
 * we have more.  We always start at 0x80000000, and we place the
 * initial stack pointer in our SRAM. Otherwise, we can define
 * CONFIG_NR_DRAM_BANKS before including this file.
 */
#ifndef CONFIG_NR_DRAM_BANKS
#define CONFIG_NR_DRAM_BANKS		1
#endif
#define CONFIG_SYS_SDRAM_BASE		0x00000000
#define CONFIG_SYS_SDRAM_SIZE		0x40000000

#define IMVQ_SECURE_SRAM_START		0xfc130000
#define IMVQ_SECURE_SRAM_END		0xfc13c000

#define CONFIG_SYS_INIT_SP_ADDR		(IMVQ_SECURE_SRAM_END - GENERATED_GBL_DATA_SIZE)

/*
 * Our DDR memory always starts at 0x00000000 and U-Boot shall have
 * relocated itself to higher in memory by the time this value is used.
 * However, set this to a 32MB offset to allow for easier Linux kernel
 * booting as the default is often used as the kernel load address.
 */
#define CONFIG_SYS_LOAD_ADDR		0x20000000

#ifndef CONFIG_SYS_TEXT_BASE
#define CONFIG_SYS_TEXT_BASE		0x02000000
#endif

#define CONFIG_SYS_UBOOT_START		CONFIG_SYS_TEXT_BASE

#define CONFIG_CMD_MEMORY			1

/*
 * We typically do not contain NOR flash.  In the cases where we do, we
 * undefine this later.
 */
#define CONFIG_SYS_NO_FLASH

/*
 * The following are general good-enough settings for U-Boot.  We set a
 * large malloc pool as we generally have a lot of DDR, and we opt for
 * function over binary size in the main portion of U-Boot as this is
 * generally easily constrained later if needed.  We enable the config
 * options that give us information in the environment about what board
 * we are on so we do not need to rely on the command prompt.  We set a
 * console baudrate of 115200 and use the default baud rate table.
 */
#define CONFIG_SYS_MALLOC_LEN   (CONFIG_ENV_SIZE + 256*1024)
#define CONFIG_SYS_HUSH_PARSER
#define CONFIG_SYS_PROMPT			"NT72668>"
#define CONFIG_SYS_CONSOLE_INFO_QUIET
#define CONFIG_BAUDRATE				115200
#define CONFIG_ENV_VARS_UBOOT_CONFIG	/* Strongly encouraged */
#define CONFIG_ENV_OVERWRITE			/* Overwrite ethaddr / serial# */
#define CONFIG_CMDLINE_TAG

/* As stated above, the following choices are optional. */
#define CONFIG_SYS_LONGHELP
#define CONFIG_AUTO_COMPLETE
#define CONFIG_CMDLINE_EDITING

/* We set the max number of command args high to avoid HUSH bugs. */
#define CONFIG_SYS_MAXARGS			64

/* Console I/O Buffer Size */
#define CONFIG_SYS_CBSIZE			512
/* Print Buffer Size */
#define CONFIG_SYS_PBSIZE			(CONFIG_SYS_CBSIZE + sizeof(CONFIG_SYS_PROMPT) + 16)
/* Boot Argument Buffer Size */
#define CONFIG_SYS_BARGSIZE			CONFIG_SYS_CBSIZE

#define CONFIG_BOOTDELAY			0		/* autoboot after 3 seconds */
#define CONFIG_ZERO_BOOTDELAY_CHECK			/* allow stopping of boot process */
#define CONFIG_SILENT_CONSOLE   1
#define CONFIG_SYS_DEVICE_NULLDEV               1
#define CONFIG_SILENT_CONSOLE_UPDATE_ON_RELOC   1
#define CONFIG_AUTOBOOT_KEYED		1
#define CONFIG_AUTOBOOT_STOP_STR	"~"

/* UART */
#define CONFIG_SYS_NS16550
#define CONFIG_SYS_NS16550_SERIAL
#define CONFIG_SYS_NS16550_REG_SIZE	(-4)			/* (-) means CPU little endian */
#define CONFIG_SYS_NS16550_MEM32
#define CONFIG_SYS_NS16550_CLK		100000000
#define CONFIG_CONS_INDEX			2				/* Serial Port B */
#define CONFIG_SYS_NS16550_COM2		0xfd091000

/* USB */
#define CONFIG_CMD_USB
#define CONFIG_USB_HOST
#define CONFIG_USB_EHCI
#define CONFIG_USB_STORAGE
#define CONFIG_USB_EHCI_NVT
/*#define CONFIG_NT72656_USB*/


#define CONFIG_NT72668_HSMMC
/* FLASH	eMMC */
#ifdef CONFIG_NT72668_HSMMC
#define CONFIG_MMC
#define CONFIG_CMD_MMC
#define CONFIG_GENERIC_MMC
/* eMMC port from 668 tk*/
#define CONFIG_NVT_EMMC
#define CONFIG_NVT_EMMC_HS200
#define CONFIG_NVT_EMMC_ONLY_HOST	/*host controller ignore sd card detection flow*/
#define CONFIG_DOS_PARTITION
#define CONFIG_NT72668_EMMC_MIN_OPCLK 	400000
/*#define CONFIG_NT72668_EMMC_MAX_OPCLK   52000000//1000000 */
#define CONFIG_NT72668_EMMC_MAX_OPCLK   180000000//1000000
#define CONFIG_SYS_MMC_SYS_DEV 		0
#define CONFIG_SYS_FWUPDATE_BUF 	0x3200000//set 50MB as firmware update buffer
/*#define CONFIG_MMC_TRACE*/
#define CONFIG_SUPPORT_EMMC_BOOT
#endif

/*fwupdate*/
#define CONFIG_CMD_FAT
#define CONFIG_NVT_INGNORE_FSREAD_SIZE_CHECK
#define CONFIG_FWUP_RAM_START		0x80A00000
#define CONFIG_FWUP_RAM_SIZE			0x4000000
#define DDRA_BASE_ADDR 0x40000000

#define CONFIG_SETUP_MEMORY_TAGS
#define CONFIG_OF_LIBFDT
/*#define CONFIG_IMAGE_FORMAT_LEGACY*/
/*#define CONFIG_ANDROID_BOOT_IMAGE*/
#define CONFIG_USB_UPDATE


/* ENV related config options */
#if defined(CONFIG_NAND)

#define CONFIG_ENV_IS_IN_NAND		1

#elif defined(CONFIG_MMC)

#define CONFIG_ENV_IS_IN_MMC		1
#define CONFIG_ENV_SIZE				(16 * 512)
#define CONFIG_ENV_MIN_ENTRIES 			128 
#define CONFIG_ENV_OFFSET       		(5377*512)
#define CONFIG_SYS_MMC_ENV_DEV  		0
#define CONFIG_CMD_SAVEENV
#define CONFIG_ENABLE_EMMC_BOOT_MODE
#define CONFIG_CMD_BOOTD

#endif


#define CONFIG_NT72656_SPI
#ifdef CONFIG_NT72656_SPI
#define CONFIG_CMD_SF
#define CONFIG_SPI_FLASH
#define CONFIG_SYS_MAX_SF_DEVICE	3

#define CONFIG_SPI_FLASH_WINBOND	1
#define CONFIG_SPI_FLASH_MACRONIX	1
#define CONFIG_SPI_FLASH_EON	    1

#define CONFIG_SF_DEFAULT_BUS		0
#define CONFIG_SF_DEFAULT_CS		0
#define CONFIG_SF_DEFAULT_MODE		3
#define CONFIG_SF_DEFAULT_SPEED		12000000
#define CONFIG_SPI_FLASH_MTD
/* #define CONFIG_SPI_LOCK_CTRL */  /* Enable  SPI lock control */
#define CONFIG_SPI_AHB_DMA_MODE
#endif

#define CONFIG_USB_PORT				"0"
#define CONFIG_EXTRA_ENV_SETTINGS       \
	"kernel_loadaddr=0x04407FC0\0" \
	"ramdisk_loadaddr=0x4E00000\0" \
	"memstr=mem=756M@68M mem=112M@2448M\0" \
	"mali_core=4\0" \
	"mali_start=0\0" \
	"mali_size=0\0" \
	"mali_share=0x18000000\0" \
	"fb_size=0x02000000\0" \
	"fb_start=0x96900000\0" \
	"ump_backend=1\0" \
	"ump_start=0\0" \
	"ump_size=0x8000000\0" \
	"cpus=4\0" \
	"usb_port=0\0" \
	"verify=y\0" \
	"quiet=n\0" \
    "selinux=n\0" \
    "serialno=nt72668\0" \
	"kparam_addon=no_console_suspend earlyprintk"
	
#define CONFIG_BOOTCOMMAND                      "n62001_boot;bootm ${kernel_loadaddr} ${ramdisk_loadaddr}"
#define CONFIG_BOOTARGS 			""

/*
 * Auto Script Configuration
 */
#define CONFIG_CMD_NVTUPDATE
#define CONFIG_AUTO_SCRIPT          "auto_update.txt"
#define CONFIG_AUTO_SCRIPT_SIZE     8192
#define CONFIG_AUTO_SCRIPT_BUF      0x90000000

#define CONFIG_NVT_CMD_NOREPEAT

#endif /* __CONFIG_NT72668TK_H */
