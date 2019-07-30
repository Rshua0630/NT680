/*
 * Copyright (C) 2016 Novatek Microelectronics Corp. All rights reserved.
 * Author: iVoT-IM <iVoT_MailGrp@novatek.com.tw>
 *
 * Configuration settings for the Novatek NA51000 SOC.
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#ifndef __CONFIG_NA51000_H
#define __CONFIG_NA51000_H

#include <linux/sizes.h>

/*#define CONFIG_DEBUG	1*/
#define CONFIG_SYS_L2CACHE_OFF

#ifdef CONFIG_DEBUG
#define DEBUG						1
#endif

/*#define CONFIG_NVT_FW_UPDATE_LED*/
/*#define CONFIG_NVT_PWM*/
#if defined(_CLOCK_DRTC_)
#define CONFIG_NVT_DRTC
#endif
/*
 * High Level Configuration Options
 */
#define CONFIG_ARMV7
#define CONFIG_SYS_GENERIC_BOARD
#define CONFIG_BOARD_EARLY_INIT_F
#define CONFIG_NA51000
#define CONFIG_DISPLAY_CPUINFO
#define CONFIG_NT96680					1
#define CONFIG_NVT_BOARD				1
#define CONFIG_NAND_TYPE_ONFI				0
#define CONFIG_NAND_TYPE_SPINAND			1
#define CONFIG_NAND_TYPE_SPINOR				2
#define CONFIG_NOVATEK_MAC_ENET

#if defined(_EMBMEM_SPI_NAND_) || defined(_EMBMEM_NAND_)
#define CONFIG_NVT_SPI_NAND
#elif defined(_EMBMEM_SPI_NOR_)
#define CONFIG_NVT_SPI_NOR
#else
#define CONFIG_NVT_IVOT_EMMC				1		/* To config emmc bus number with 1 (Enable two mmc bus: 0 and 1) */
#endif

#define NVT_FLASH_SOURCE_CLK				480000000
#if defined(CONFIG_NVT_SPI_NAND)
#define CONFIG_NVT_NAND_TYPE				CONFIG_NAND_TYPE_SPINAND
/*
 *	NANDCTRL_SPIFLASH_USE_INTERNAL_RS_ECC
 *	NANDCTRL_SPIFLASH_USE_ONDIE_ECC
*/
#define CONFIG_NVT_NAND_ECC				NANDCTRL_SPIFLASH_USE_ONDIE_ECC
#elif defined(CONFIG_NVT_SPI_NOR)
#define CONFIG_NVT_NAND_TYPE				CONFIG_NAND_TYPE_SPINOR
#define CONFIG_CMD_SF
#define CONFIG_SPI_FLASH
#define CONFIG_MTD_DEVICE
#define CONFIG_SPI_FLASH_MACRONIX
#define CONFIG_SPI_FLASH_WINBOND
#define CONFIG_SPI_FLASH_GIGADEVICE
#define CONFIG_SPI_FLASH_EON
#define CONFIG_SYS_MAX_SF_DEVICE			1
#define CONFIG_SPI_FLASH_MTD
#define CONFIG_SF_DEFAULT_BUS				0
#define CONFIG_SF_DEFAULT_CS				0
#define CONFIG_SF_DEFAULT_MODE				0
#define CONFIG_SF_DEFAULT_SPEED				48000000
#else
#define CONFIG_NVT_NAND_TYPE				CONFIG_NAND_TYPE_ONFI
#endif

#define CONFIG_MACH_TYPE				MACH_TYPE_EVB96680
#define CONFIG_SYS_ARCH_TIMER
#define CONFIG_MISC_INIT_R

#define CONFIG_CPU_FREQ					768	/* default value, MHz */

#define CONFIG_SYS_HZ					1000

#define CONFIG_USE_ARCH_MEMCPY
#define CONFIG_USE_ARCH_MEMSET

/*RTC Default Date*/
#define RTC_YEAR 2000
#define RTC_MONTH 1
#define RTC_DAY 1

/*-----------------------------------------------------------------------
 * IP address configuration
 */
#ifdef CONFIG_NOVATEK_MAC_ENET
#define CONFIG_ETHNET
#endif

#define CONFIG_FIXED_ETH_PARAMETER

#ifdef CONFIG_FIXED_ETH_PARAMETER
#ifdef CONFIG_ETHNET
#define CONFIG_ETHADDR				0a:0b:0c:0d:0e:01
#define CONFIG_IPADDR				192.168.1.99	/* Target IP address */
#define CONFIG_NETMASK				255.255.255.0
#define CONFIG_SERVERIP				192.168.1.11	/* Server IP address */
#define CONFIG_GATEWAYIP			192.168.1.254
#define CONFIG_HOSTNAME				soclnx
#endif
#endif

#define ETH_PHY_HW_RESET
#define NVT_PHY_RST_PIN D_GPIO(1)
/*#define ETH_PHY_25MHz_ENABLE*/

/*-----------------------------------------------------------------------
 * BOOTP options
 */
#ifdef CONFIG_ETHNET
#define CONFIG_PHYLIB
#define CONFIG_BOOTP_SUBNETMASK
#define CONFIG_BOOTP_GATEWAY
#define CONFIG_BOOTP_HOSTNAME
#define CONFIG_BOOTP_BOOTPATH
#define CONFIG_BOOTP_BOOTFILESIZE
#endif
/*-----------------------------------------------------------------------*/

#ifdef CONFIG_ETHNET
#define CONFIG_CMD_PING
#define CONFIG_CMD_DHCP
#define CONFIG_CMD_NET
#define CONFIG_NET_MULTI
#define CONFIG_NETCONSOLE
#endif

/*-----------------------------------------------------------------------
 * Serial console configuration
 */

#define CONSOLE_UART1	0
#define CONSOLE_UART2	1
#define CONSOLE_UART3	2
#define CONSOLE_UART4	3

#ifdef _NVT_LINUX_SMP_ON_
#define CONFIG_SYS_UART					CONSOLE_UART1
#else
#define CONFIG_SYS_UART					CONSOLE_UART2
#endif
#define CONFIG_SYS_BAUDRATE_TABLE			{ 9600, 19200, 38400, 57600, 115200 }
#define CONFIG_BAUDRATE					115200

/*
 * UART2_1 UART2_TX/RX
 * UART2_2 C_GPIO29/C_GPIO30
 * UART3_1 P_GPIO30/P_GPIO31
 * UART3_2 P_GPIO8/P_GPIO9
 * UART4_1 P_GPIO4/P_GPIO5
 * UART4_2 C_GPIO12/C_GPIO13
 */
#define PINMUX_CHANNEL_1	0
#define PINMUX_CHANNEL_2	1

#define UART_PINMUX_SEL	PINMUX_CHANNEL_1


/*-----------------------------------------------------------------------
 * NVT LED CONFIG
 *
 * LED GPIO selection
 * C_GPIO(x)
 * P_GPIO(x)
 * S_GPIO(x)
 * L_GPIO(x)
 * D_GPIO(x)
 * Duration Unit: ms
 */
#ifdef CONFIG_NVT_FW_UPDATE_LED
#ifdef CONFIG_NVT_PWM
#define PWM_SIGNAL_NORMAL 0
#define PWM_SIGNAL_INVERT 1
#define NVT_PWMLED (PWMID_0 | PWMID_1)
#define PWM_SIGNAL_TYPE PWM_SIGNAL_INVERT
#define	PWM_LED_ERASE 50
#define	PWM_LED_PROGRAM 5
#else
#define NVT_LED_PIN P_GPIO(12)
#define NVT_LED_ERASE_DURATION 30
#define NVT_LED_PROGRAM_DURATION 10
#endif
#endif

/* Enable GPIO Usage*/
#define CONFIG_NA51000_GPIO

/* Enable ADC Usage*/
#define CONFIG_NVT_ADC

/*
 * DDR information.  If the CONFIG_NR_DRAM_BANKS is not defined,
 * we say (for simplicity) that we have 1 bank, always, even when
 * we have more.  We always start at 0x80000000, and we place the
 * initial stack pointer in our SRAM. Otherwise, we can define
 * CONFIG_NR_DRAM_BANKS before including this file.
 */

#define CONFIG_NR_DRAM_BANKS				1		/* we have 1 bank of DRAM */
#define PHYS_SDRAM_1					0x00000000	/* DDR Start */
#define PHYS_SDRAM_1_SIZE				CONFIG_MEM_SIZE	/* DDR size 512MB */

/*
 * To include nvt memory layout
 */
#include "novatek/na51000_ca53_evb.h"
#define CONFIG_SYS_SDRAM_BASE				CONFIG_LINUX_SDRAM_BASE
#define CONFIG_SYS_SDRAM_SIZE				CONFIG_LINUX_SDRAM_SIZE + CONFIG_UBOOT_SDRAM_SIZE

#define NVT_LINUX_BOOT_PARAM_ADDR			(CONFIG_LINUX_SDRAM_BASE + 0x100)

#define CONFIG_SYS_INIT_SP_ADDR				(CONFIG_UBOOT_SDRAM_BASE + CONFIG_UBOOT_SDRAM_SIZE - 0x1000)

/*
 * Our DDR memory always starts at 0x00000000 and U-Boot shall have
 * relocated itself to higher in memory by the time this value is used.
 * However, set this to a 32MB offset to allow for easier Linux kernel
 * booting as the default is often used as the kernel load address.
 */
#define CONFIG_SYS_LOAD_ADDR				CONFIG_LINUX_SDRAM_START

#define CONFIG_SYS_UBOOT_START				CONFIG_SYS_TEXT_BASE

#define CONFIG_CMD_MEMORY				1

/*
 * We typically do not contain NOR flash.  In the cases where we do, we
 * undefine this later.
 */
#define CONFIG_SYS_NO_FLASH

/* ENV related config options */
#if defined(_NVT_UBOOT_ENV_IN_STORG_SUPPORT_NAND_)
#define CONFIG_CMD_SAVEENV
#define CONFIG_ENV_IS_IN_NAND
#define CONFIG_ENV_OFFSET				0x00B00000				/* Defined by iTron emb_partition_info.c */
#define CONFIG_ENV_SIZE					(128 << 10)				/* Unit: Block size: 128 KiB */
#define CONFIG_ENV_RANGE				2 * CONFIG_ENV_SIZE			/* Defined by iTron emb_partition_info.c */
#elif defined(_NVT_UBOOT_ENV_IN_STORG_SUPPORT_NOR_)
#define CONFIG_CMD_SAVEENV
#define CONFIG_ENV_IS_IN_SPI_FLASH
#define CONFIG_ENV_OFFSET				0x004E0000				/* It must be aligned to an erase secrote boundary */
#define CONFIG_ENV_SIZE					0x00010000				/* Sync to emb_partition_info.c */
#define CONFIG_ENV_SECT_SIZE				(64 << 10)				/* Define the SPI flash's sector size */
#elif defined(_NVT_UBOOT_ENV_IN_STORG_SUPPORT_MMC_)
#define CONFIG_CMD_SAVEENV
#define CONFIG_ENV_IS_IN_MMC
#define CONFIG_ENV_OFFSET				(6 * 64 * 1024)
#define CONFIG_ENV_SIZE					0x00040000
#else
#define CONFIG_ENV_IS_NOWHERE
#define CONFIG_ENV_SIZE					(8 << 10)
#endif

/*
 * The following are general good-enough settings for U-Boot.  We set a
 * large malloc pool as we generally have a lot of DDR, and we opt for
 * function over binary size in the main portion of U-Boot as this is
 * generally easily constrained later if needed.  We enable the config
 * options that give us information in the environment about what board
 * we are on so we do not need to rely on the command prompt.  We set a
 * console baudrate of 115200 and use the default baud rate table.
 */
#define CONFIG_SYS_MALLOC_LEN				(3*1024 * 1024)	/* 3MB */
#define CONFIG_SYS_HUSH_PARSER
#define CONFIG_SYS_PROMPT				"uboot("_CHIP_NAME_") $ "
#define CONFIG_SYS_CONSOLE_INFO_QUIET
#define CONFIG_BAUDRATE					115200
#define CONFIG_ENV_VARS_UBOOT_CONFIG			/* Strongly encouraged */
#define CONFIG_ENV_OVERWRITE				/* Overwrite ethaddr / serial# */

/* As stated above, the following choices are optional. */
#define CONFIG_SYS_LONGHELP
#define CONFIG_AUTO_COMPLETE
#define CONFIG_CMDLINE_EDITING
#define CONFIG_VERSION_VARIABLE

/* We set the max number of command args high to avoid HUSH bugs. */
#define CONFIG_SYS_MAXARGS				64

/* Console I/O Buffer Size */
#define CONFIG_SYS_CBSIZE				1024
/* Print Buffer Size */
#define CONFIG_SYS_PBSIZE				(CONFIG_SYS_CBSIZE + sizeof(CONFIG_SYS_PROMPT) + 16)
/* Boot Argument Buffer Size */
#define CONFIG_SYS_BARGSIZE				CONFIG_SYS_CBSIZE

#define CONFIG_BOOTDELAY				0			/* autoboot after 3 seconds */
#define CONFIG_ZERO_BOOTDELAY_CHECK						/* allow stopping of boot process */
										/* even with bootdelay=0 */
/*#define CONFIG_AUTOBOOT_KEYED				1*/
#define CONFIG_AUTOBOOT_STOP_STR			"~"

/* MMC */
#define CONFIG_MMC
#define CONFIG_CMD_MMC
#define CONFIG_CMD_FAT
#define CONFIG_FS_FAT
#define CONFIG_CMD_EXFAT
#define CONFIG_FS_EXFAT
#define CONFIG_FS_EXT4
#define CONFIG_DOS_PARTITION
#define CONFIG_GENERIC_MMC
#define CONFIG_SDHCI
#define CONFIG_SUPPORT_EMMC_BOOT					/* Support emmc boot partition */
#ifdef CONFIG_NVT_IVOT_EMMC
#define CONFIG_NVT_MMC_CHANNEL				5		/* This is for emmc boot flow, the sdio init should be bus 1 and 3 */
#else
#define CONFIG_NVT_MMC_CHANNEL				1
#endif
#define CONFIG_NVT_MMC_MAX_NUM				3
/* NVT MMC Channel Matrix
SDIO   BIT 2  1  0
1          0  0  1
2          0  1  0
3          1  0  0

example:
if enabling SDIO 1 and 3, CONFIG_NVT_MMC_CHANNEL would be BIT[1, 0, 1], which is 5 */

/* FAT */
#if defined(CONFIG_FS_FAT)
#define CONFIG_FAT_WRITE
#endif

/* CRYPTO */
#define CONFIG_MD5

/* MTD */
#define CONFIG_CMD_MTDPARTS
#define CONFIG_RBTREE							/* required by CONFIG_CMD_UBI */
#define CONFIG_LZO							/* required by CONFIG_CMD_UBIFS */
#define CONFIG_LZMA							/* required by uitron decompress */
#define CONFIG_MTD_PARTITIONS						/* required for UBI partition support */
#define CONFIG_MTD_DEVICE

#define CONFIG_CMDLINE_TAG				1		/* enable passing of ATAGs */
#define CONFIG_SETUP_MEMORY_TAGS			1
#define CONFIG_INITRD_TAG				1
#define CONFIG_REVISION_TAG				1

#define CONFIG_BOOTARGS_COMMON				"earlyprintk console=ttyS0,115200 rootwait "


/* NVT boot related setting */
#define CONFIG_NVT_LINUX_AUTOLOAD						/* Support for uboot autoboot (loading and boot FW96680T.bin[for RAMDISK] or FW96680A.bin[for mtd device]) */
#ifdef CONFIG_NVT_LINUX_AUTOLOAD
	#define CONFIG_NVT_LINUX_AUTODETECT					/* Support for detect FW96680A.bin/FW96680T.bin automatically. (Only working on mtd device boot method) */
	#define CONFIG_NVT_BIN_CHKSUM_SUPPORT					/* This option will check rootfs/uboot checksum info. during update image flow */
	#if defined(_NVT_ROOTFS_TYPE_RAMDISK_)
		/* the ramdisk dram base/size will be defined in itron modelext info. */
		#define CONFIG_NVT_LINUX_RAMDISK_BOOT 				/* Loading ramdisk image rootfs.bin from SD card */
		#define CONFIG_NVT_SPI_NAND
		#define CONFIG_NVT_NAND_TYPE		CONFIG_NAND_TYPE_SPINAND
		#define CONFIG_BOOTARGS 		CONFIG_BOOTARGS_COMMON "root=/dev/ram0 rootfstype=ramfs rdinit=/linuxrc "
	#elif defined(_NVT_ROOTFS_TYPE_NAND_UBI_)				/* UBIFS rootfs boot */
		#define CONFIG_NVT_LINUX_SPINAND_BOOT
		#define CONFIG_NVT_UBIFS_SUPPORT
		#define CONFIG_CMD_NAND_TRIMFFS
		#define CONFIG_BOOTARGS			CONFIG_BOOTARGS_COMMON "root=ubi0:rootfs rootfstype=ubifs rw ubi.fm_autoconvert=1 init=/linuxrc "
		#define CONFIG_CMD_UBI						/* UBI-formated MTD partition support */
		#define CONFIG_CMD_UBIFS					/* Read-only UBI volume operations */
	#elif defined(_NVT_ROOTFS_TYPE_NAND_SQUASH_)				/* SquashFs rootfs boot */
		#define CONFIG_NVT_LINUX_SPINAND_BOOT
		#define CONFIG_NVT_SQUASH_SUPPORT
		#define CONFIG_BOOTARGS			CONFIG_BOOTARGS_COMMON "rootfstype=squashfs ro "
	#elif defined(_NVT_ROOTFS_TYPE_NAND_JFFS2_)				/* JFFS2 rootfs boot */
		#define CONFIG_NVT_LINUX_SPINAND_BOOT				/* Boot from spinand or spinor (Support FW96680A.bin update all-in-one) */
		#define CONFIG_NVT_JFFS2_SUPPORT
		#define CONFIG_CMD_NAND_TRIMFFS
		#define CONFIG_BOOTARGS			CONFIG_BOOTARGS_COMMON "rootfstype=jffs2 rw "
	#elif defined(_NVT_ROOTFS_TYPE_NOR_SQUASH_)				/* Squashfs rootfs boot */
		#define CONFIG_NVT_LINUX_SPINOR_BOOT				/* Boot from spinand or spinor (Support FW96680A.bin update all-in-one) */
		#define CONFIG_NVT_SQUASH_SUPPORT
		#define CONFIG_BOOTARGS			CONFIG_BOOTARGS_COMMON "rootfstype=squashfs ro "
	#elif defined(_NVT_ROOTFS_TYPE_NOR_JFFS2_)				/* JFFS2 rootfs boot */
		#define CONFIG_NVT_LINUX_SPINOR_BOOT				/* Boot from spinand or spinor (Support FW96680A.bin update all-in-one) */
		#define CONFIG_NVT_JFFS2_SUPPORT
		#define CONFIG_BOOTARGS			CONFIG_BOOTARGS_COMMON "rootfstype=jffs2 rw "
	#elif defined(_NVT_ROOTFS_TYPE_EMMC_)
		#define CONFIG_NVT_LINUX_EMMC_BOOT				/* Boot from emmc (Support FW96680A.bin update all-in-one) */
		#define CONFIG_NVT_EXT4_SUPPORT
		#define CONFIG_BOOTARGS			CONFIG_BOOTARGS_COMMON "rootfstype=ext4 rw "
	#else
		#define CONFIG_NVT_LINUX_SD_BOOT				/* To handle RAW SD boot (e.g. itron.bin, uImage.bin, uboot.bin...) itron.bin u-boot.bin dsp.bin dsp2.bin must be not compressed.*/
		#define CONFIG_BOOTARGS 		CONFIG_BOOTARGS_COMMON "root=/dev/mmcblk0p2 noinitrd rootfstype=ext3 init=/linuxrc "
	#endif /* _NVT_ROOTFS_TYPE_ */
#endif /* CONFIG_NVT_LINUX_AUTOLOAD */

#if defined(_DSP1_FREERTOS_)
#define CONFIG_DSP1_FREERTOS
#endif

#if defined(_DSP2_FREERTOS_)
#define CONFIG_DSP2_FREERTOS
#endif

#define CONFIG_IMAGE_FORMAT_LEGACY
#define CONFIG_BOOTCOMMAND				"nvt_boot"

#endif /* __CONFIG_NA51000_H */
