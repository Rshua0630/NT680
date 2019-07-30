/*
 * Copyright (C) 2014 Novatek Microelectronics Corp. All rights reserved.
 * Author: SP-ISW
 *
 * Configuration settings for the Novatek NT72668 Turnkey board.
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#ifndef __CONFIG_TCLCN668_H__
#define __CONFIG_TCLCN668_H__

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
#define CONFIG_TCL_CN_668 			1

#define CONFIG_DISPLAY_CPUINFO		1
#define CONFIG_DISPLAY_BOARDINFO	1

#define CONFIG_CPU_FREQ				1000	/* default value, MHz */

#define CONFIG_SYS_HZ				1000

#ifndef CONFIG_SYS_L2CACHE_OFF
#define CONFIG_SYS_L2_PL310			1
#define CONFIG_SYS_PL310_BASE		0xffe00000
#endif

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
#define CONFIG_SYS_SDRAM_SIZE		0x08000000

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

/* ENV related config options */
#if defined(CONFIG_NAND)
#define CONFIG_ENV_IS_IN_NAND		1
#elif defined(CONFIG_MMC)
#define CONFIG_ENV_IS_IN_MMC		1
#elif defined(CONFIG_SPIFLASH)
#define CONFIG_ENV_IS_IN_SPI_FLASH	1
#else
#define CONFIG_ENV_IS_NOWHERE
#define CONFIG_ENV_VARS_UBOOT_RUNTIME_CONFIG
#endif

/*
 * Total Size Environment - 8k
 */
#define CONFIG_ENV_SIZE				(8 << 10)

/*
 * The following are general good-enough settings for U-Boot.  We set a
 * large malloc pool as we generally have a lot of DDR, and we opt for
 * function over binary size in the main portion of U-Boot as this is
 * generally easily constrained later if needed.  We enable the config
 * options that give us information in the environment about what board
 * we are on so we do not need to rely on the command prompt.  We set a
 * console baudrate of 115200 and use the default baud rate table.
 */
#define CONFIG_SYS_MALLOC_LEN		(16 << 20)
#define CONFIG_SYS_HUSH_PARSER
#define CONFIG_SYS_PROMPT			"U-Boot# "
#define CONFIG_SYS_CONSOLE_INFO_QUIET
#define CONFIG_BAUDRATE				115200
#define CONFIG_ENV_VARS_UBOOT_CONFIG	/* Strongly encouraged */
#define CONFIG_ENV_OVERWRITE			/* Overwrite ethaddr / serial# */

/* As stated above, the following choices are optional. */
#define CONFIG_SYS_LONGHELP
#define CONFIG_AUTO_COMPLETE
#define CONFIG_CMDLINE_EDITING
#define CONFIG_VERSION_VARIABLE

/* We set the max number of command args high to avoid HUSH bugs. */
#define CONFIG_SYS_MAXARGS			64

/* Console I/O Buffer Size */
#define CONFIG_SYS_CBSIZE			512
/* Print Buffer Size */
#define CONFIG_SYS_PBSIZE			(CONFIG_SYS_CBSIZE + sizeof(CONFIG_SYS_PROMPT) + 16)
/* Boot Argument Buffer Size */
#define CONFIG_SYS_BARGSIZE			CONFIG_SYS_CBSIZE

#define CONFIG_BOOTDELAY			3		/* autoboot after 3 seconds */
#define CONFIG_ZERO_BOOTDELAY_CHECK			/* allow stopping of boot process */
											/* even with bootdelay=0 */
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

/* FLASH	FileSystem */
#define CONFIG_CMD_UBI				/* UBI-formated MTD partition support */
#define CONFIG_CMD_UBIFS			/* Read-only UBI volume operations */
#define CONFIG_CMD_MTDPARTS
#define CONFIG_RBTREE				/* required by CONFIG_CMD_UBI */
#define CONFIG_LZO					/* required by CONFIG_CMD_UBIFS */
#define CONFIG_MTD_PARTITIONS		/* required for UBI partition support */
#define CONFIG_MTD_DEVICE

/*#define CONFIG_NT72656_SPI*/

#define CONFIG_NT72668_HSMMC
/* FLASH	eMMC */
#ifdef CONFIG_NT72668_HSMMC
#define CONFIG_MMC
#define CONFIG_CMD_MMC
#define CONFIG_GENERIC_MMC
/* eMMC port from 668 tk*/
#define CONFIG_NVT_EMMC
#define CONFIG_DOS_PARTITION
#define CONFIG_NT72668_EMMC_MIN_OPCLK 	400000
#define CONFIG_NT72668_EMMC_MAX_OPCLK   52000000//1000000
#define CONFIG_SYS_MMC_SYS_DEV 		0
#define CONFIG_SYS_FWUPDATE_BUF 	0x3200000//set 50MB as firmware update buffer
/*#define CONFIG_MMC_TRACE*/
#define CONFIG_SUPPORT_EMMC_BOOT
#endif

/* FLASH	NAND */
/*#define CONFIG_NT72656_NAND*/
#ifdef CONFIG_NT72656_NAND
#define CONFIG_NAND
#define CONFIG_CMD_NAND
#define CONFIG_SYS_MAX_NAND_DEVICE	1
#define CONFIG_SYS_NAND_BASE		0x8000000
#endif

/* FLASH	SPI */
#ifdef CONFIG_NT72656_SPI
#define CONFIG_CMD_SF
#define CONFIG_SPI_FLASH
#define CONFIG_SYS_MAX_SF_DEVICE	1
#define CONFIG_SPI_FLASH_WINBOND	1

#define CONFIG_SF_DEFAULT_BUS		0
#define CONFIG_SF_DEFAULT_CS		0
#define CONFIG_SF_DEFAULT_MODE		3
#define CONFIG_SF_DEFAULT_SPEED		12000000

#define CONFIG_SPI_LOCK_CTRL
#define CONFIG_SPI_AHB_DMA_MODE
#endif

/*fwupdate*/
#define CONFIG_CMD_FAT
#define CONFIG_NVT_INGNORE_FSREAD_SIZE_CHECK
#define CONFIG_FWUP_RAM_START		0x80A00000
#define CONFIG_FWUP_RAM_SIZE			0x4000000
#define DDRA_BASE_ADDR 0x40000000

#define CONFIG_SETUP_MEMORY_TAGS
#define CONFIG_OF_LIBFDT
#define CONFIG_IMAGE_FORMAT_LEGACY
#define CONFIG_ANDROID_BOOT_IMAGE
#endif /* __CONFIG_NT72668TK_H */
