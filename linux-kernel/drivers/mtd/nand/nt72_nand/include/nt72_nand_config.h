#ifndef _NT72_NAND_CONFIG_H_
#define _NT72_NAND_CONFIG_H_

/* TODO */
#define nfc_dbg pr_err
#define nfc_msg pr_err
#define nfc_warn pr_err
#define nfc_err pr_err
#define nfc_cont pr_cont

#ifdef __NVT_NAND_UBOOT__
#define IS_UBOOT
#else
#define IS_KERNEL
#endif

#ifdef CONFIG_NT72_NAND_FLASH_CONTROLLER_BBT
#define USE_BBT
/* #define USE_BBT_CHECKSUM */
#endif

#ifdef CONFIG_ARM
#define USE_DT
#define USE_IO_REMAP
/* newer kernel has different MTD NAND (like BBT) definitions */
#define USE_NEW_MTD
#define USE_NEW_MAPPING
#endif

#ifdef CONFIG_MTD_OF_PARTS
#define USE_CMDLINE_PARTS
#endif

/* global configurations */
#define USE_INTERRUPT
#define INTERRUPT_TIMEOUT (10 * HZ)     /* 10 seconds */
#define REG_POLLING_TIMEOUT (10 * HZ)   /* 10 seconds */
#define MAX_NR_SUBPAGE 8
#define READ_RETRY_COUNT 10
#define FILL_BLANK_PAGE

/* extra checks */
#define EXTRA_WAIT_NFC_READY
#define EXTRA_CHECK_CACHE_ALIGNMENT
#define EXTRA_CHECK_IN_WAITFUNC
/*
#define EXTRA_PRINT_NON_FF_COUNT
*/

/* dump */
#define DUMP_ECC_WHEN_UNCORRECTABLE
#define DUMP_REG_ON_ERROR
#define DUMP_DATA_ON_ERROR
/* #define DUMP_CONTROL_REGS */

/* debug features */
#define ENABLE_CMD_HISTORY
#define CMD_HISTORY_SIZE 10
/*
#define ENABLE_BUS_SNIFFER
#define BUS_SNIFFER_SIZE 512
*/

/* selftests */
#define SELFTEST_NAND_PARAMS
/*
#define SELFTEST_REG_POLLING_TIMEOUT
#define SELFTEST_FAKE_BITFLIP
#define SELFTEST_FAKE_BITFLIP_BLANK
*/

#endif /* _NT72_NAND_CONFIG_H_ */
