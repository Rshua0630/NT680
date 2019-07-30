/*
 * (C) Copyright 2003
 * Wolfgang Denk, DENX Software Engineering, wd@denx.de.
 *
 * See file CREDITS for list of people who contributed to this
 * project.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 * MA 02111-1307 USA
 */

/*
 * This file contains the configuration parameters for qemu-mips target.
 */

#ifndef __CONFIG_H
#define __CONFIG_H

//#define DEVELOPEMENT_MODE     //define this to enable ethernet and command line


//#define DEBUG						1

#define ENV_IN_NAND		// To decide if ENV is stored in nand

//#define CFG_USE_SPIFS

//#define CONFIG_FPGA_EMULATION       1
#define CONFIG_MIPS32				1	/* CPU core */
#define CONFIG_NT96660			    1	/* SoC */
//#define CONFIG_SKIP_LOWLEVEL_INIT	1   // Do not define this, we need to map 0xc0000000 via TLB in lowlevel_init.s

/* What is the oscillator's frequency in MHz? */
#ifdef CONFIG_FPGA_EMULATION
#define CONFIG_SYS_CLKIN  		   24
#else
#define CONFIG_SYS_CLKIN                   240
#endif

//Original
#ifdef CONFIG_FPGA_EMULATION
#define CONFIG_CPU_MHZ			   10 		/* pll setting for CPU core */
#else
#define CONFIG_CPU_MHZ                     420
#endif
#define CONFIG_SYS_MIPS_TIMER_FREQ	((CONFIG_CPU_MHZ * 1000000) / 2)
#define CONFIG_SYS_HZ				1000

/*-----------------------------------------------------------------------
 * Cache Configuration
 */
#define CONFIG_SYS_DCACHE_SIZE		16384
#define CONFIG_SYS_ICACHE_SIZE		16384
#define CONFIG_SYS_CACHELINE_SIZE	32

/*-----------------------------------------------------------------------
 * Serial console configuration
 */
#define CONFIG_SYS_UART				1 // uart 0 or 1
#define CONFIG_SYS_BAUDRATE_TABLE	{ 9600, 19200, 38400, 57600, 115200 }
#define CONFIG_BAUDRATE				115200

/*-----------------------------------------------------------------------
 * IP address configuration
 */
#ifdef DEVELOPEMENT_MODE
#define CONFIG_ETHNET
#endif

#ifdef CONFIG_FIXED_ETH_PARAMETER
#ifdef CONFIG_ETHNET
#define CONFIG_ETHADDR				0a:0b:0c:0d:0e:01
#define CONFIG_IPADDR				192.168.0.1	/* Target IP address */
#define CONFIG_NETMASK				255.255.255.0
#define CONFIG_SERVERIP				192.168.1.254	/* Server IP address */
#define CONFIG_GATEWAYIP			192.168.1.254
#define CONFIG_HOSTNAME				soclnx
#endif

/*-----------------------------------------------------------------------
 * BOOTP options
 */
#ifdef CONFIG_ETHNET
#define CONFIG_BOOTP_SUBNETMASK
#define CONFIG_BOOTP_GATEWAY
#define CONFIG_BOOTP_HOSTNAME
#define CONFIG_BOOTP_BOOTPATH
#define CONFIG_BOOTP_BOOTFILESIZE
#endif
#endif /* CONFIG_FIXED_ETH_PARAMETER */
/*-----------------------------------------------------------------------
 * Miscellaneous configurable options
 */
#ifdef DEVELOPEMENT_MODE
#define CONFIG_SYS_LONGHELP	/* undef to save memory */

#define CONFIG_AUTO_COMPLETE
#define CONFIG_CMDLINE_EDITING
#define CONFIG_SYS_HUSH_PARSER
#define CONFIG_SYS_PROMPT_HUSH_PS2	"> "
#endif

/* Configure Linux kernel compression type support selection */
#define CONFIG_LZO
//#define CONFIG_BZIP2
//#define CONFIG_LZMA

#define CONFIG_SYS_PROMPT			"NT96660> "	/* Monitor Command Prompt */
#define CONFIG_SYS_CBSIZE			512			/* Console I/O Buffer Size */
#define CONFIG_SYS_PBSIZE 			(CONFIG_SYS_CBSIZE + sizeof(CONFIG_SYS_PROMPT) + 16)  /* Print Buffer Size */
#define CONFIG_SYS_MAXARGS			16			/* max number of command args */

#ifdef CONFIG_BZIP2
/* bz2 needs more RAM space to decompress */
#define CONFIG_SYS_MALLOC_LEN		(3* 1024 * 1024)
#else
#define CONFIG_SYS_MALLOC_LEN		(1024 * 1024)
#endif
#define CONFIG_SYS_BOOTPARAMS_LEN	4096

#define CONFIG_SYS_LOAD_ADDR		0 //not used currently	/* default linux kernel load address (uImage)*/

#define CONFIG_SYS_MEMTEST_START	0 //not used currently
#define CONFIG_SYS_MEMTEST_END		0 //not used currently

#undef CONFIG_MEMSIZE_IN_BYTES

#define CONFIG_MISC_INIT_R


/*-----------------------------------------------------------------------
 * NVT related setting
 *
 */
#define CONFIG_NVT_SPINAND_HWECC       //Support SPI Nand HWECC
//#define CONFIG_NVT_SQUASHFS_SUPPORT	//Support squashfs boot
//#define CONFIG_NVT_JFFS2_SUPPORT	//Support jffs2 boot

/*-----------------------------------------------------------------------
 * Physical memory map
 *
 */
#define CFG_MEM_SIZE				4  // we allow u-boot use 4MB DRAM
#define CONFIG_SYS_FLASH_BASE		0x0 // not used currently
#define CONFIG_RAMOOPS_LOG

/*-----------------------------------------------------------------------
 * Stack sizes
 * The stack sizes are set up in start.S using the settings below
 */
#define CONFIG_STACKSIZE			(256 * 1024)	/* regular stack */
//#define CONFIG_SYS_INIT_SP_OFFSET	(CONFIG_SYS_TEXT_BASE + 0x00200000 - CONFIG_SYS_SDRAM_BASE) //here must avoid overlaping uItron memory area. we use address of u-boot area + 0x200000

/*-----------------------------------------------------------------------
 * FLASH organization
 */
#define CONFIG_SYS_NO_FLASH		1

/*-----------------------------------------------------------------------
 * environment setting
 */
#define CONFIG_CMD_NAND
#define CONFIG_SYS_MAX_NAND_DEVICE	1
#define CONFIG_SYS_NAND_BASE		0xC0400000

#define CONFIG_BOOTDELAY		0	/* autoboot after 0 seconds */
//#define CONFIG_AUTOBOOT_KEYED      1
//#define CONFIG_AUTOBOOT_STOP_STR    " "
//#define CONFIG_AUTOBOOT_PROMPT  "Press SPACE to abort autoboot in %d seconds\n", bootdelay
#ifdef DEVELOPEMENT_MODE
#define CONFIG_ZERO_BOOTDELAY_CHECK
#endif

#define CONFIG_CMD_UBI
#define CONFIG_CMD_UBIFS
#ifdef CONFIG_CMD_UBIFS
#define CONFIG_LZO
#endif
#define CONFIG_RBTREE
#define CONFIG_MTD_DEVICE
#define CONFIG_MTD_PARTITIONS
#define CONFIG_CMD_MTDPARTS
#define CONFIG_CMD_NAND_TRIMFFS

#define CONFIG_TIMESTAMP	/* Print image info with timestamp */

#define CONFIG_SYS_STBC_LEN			(64 << 10)
#define CONFIG_SYS_ROM_LEN			(32 << 10)

#ifdef ENV_IN_NAND
#define CONFIG_ENV_IS_IN_NAND		1
#define CONFIG_ENV_SIZE			(128 << 10)	/* Unit: Block size: 128 KiB */
#define CONFIG_ENV_OFFSET		0x00B00000	/* Defined by iTron PartitionInfo.c */
#define CONFIG_ENV_RANGE		2 * CONFIG_ENV_SIZE /* Defined by iTron PartitionInfo.c */
#else
#define CONFIG_ENV_IS_NOWHERE 1
#define CONFIG_ENV_SIZE			8192
#endif

//#define CONFIG_SYS_MONITOR_BASE		CONFIG_SYS_TEXT_BASE
#define CONFIG_SYS_MONITOR_LEN		(512 << 10)

#define CONFIG_BOOTARGS				"" /* Kernel cmdline expend */

#define CONFIG_BOOTCOMMAND 			"nvt_boot; bootm ${kernel_loadaddr}"

/*-----------------------------------------------------------------------
 * Command line configuration.
 */
#include <config_cmd_default.h>

#ifndef CONFIG_ETHNET
#undef CONFIG_CMD_NET
#undef CONFIG_CMD_NFS
#endif

#ifndef CFG_USE_SPIFS
#define CONFIG_CMD_ELF
#endif

#ifdef CONFIG_ETHNET
#define CONFIG_CMD_PING
#define CONFIG_CMD_DHCP

#define CONFIG_CMD_NET
#define CONFIG_NET_MULTI
#define CONFIG_NETCONSOLE
#endif


#endif /* __CONFIG_H */
