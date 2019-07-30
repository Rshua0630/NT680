/**
    NVT common header file
    Define common functions
    @file       nvt_common.h
    @ingroup
    @note
    Copyright   Novatek Microelectronics Corp. 2016.  All rights reserved.

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License version 2 as
    published by the Free Software Foundation.
*/

#ifndef __NVT_COMMON_H__
#define __NVT_COMMON_H__

#include <asm/io.h>
#include <modelext/emb_partition_info.h>
#include <modelext/interrupt.h>
#include <modelext/gpio_info.h>
#include <asm/arch/nvt_types.h>
extern unsigned long nvt_tm0_cnt_beg;
extern unsigned long nvt_tm0_cnt_end;
extern unsigned long nvt_binfo_comm_uboot_boot_func;
extern unsigned long nvt_binfo_comm_core1_start;
extern unsigned long nvt_binfo_comm_core2_start;
extern unsigned long nvt_binfo_comm_itron_comp_addr;
extern unsigned long nvt_binfo_comm_itron_comp_len;
extern unsigned long nvt_binfo_comm_fw_update_addr;
extern unsigned long nvt_binfo_comm_fw_update_len;
extern void int_set_config(INT_CONFIG_ID config_id, UINT32 config);
extern void iocfg_init(GPIO_INIT_OBJ *gpio_map_initable, u32 gpio_count);
extern void nvt_rtc_init(void);
extern char *nvt_bin_name;
extern char *nvt_bin_name_t;
extern char *nvt_bin_name_r;
extern char *nvt_bin_name_modelext;
extern u32 nvt_flash_ecc_err_pages[100];

/* print color setting */
#define ANSI_COLOR_RED			"\x1b[31m"
#define ANSI_COLOR_GREEN		"\x1b[32m"
#define ANSI_COLOR_YELLOW		"\x1b[33m"
#define ANSI_COLOR_BLUE			"\x1b[34m"
#define ANSI_COLOR_MAGENTA		"\x1b[35m"
#define ANSI_COLOR_CYAN			"\x1b[36m"
#define ANSI_COLOR_WHITE		"\x1b[37m"
#define ANSI_COLOR_RESET		"\x1b[0m"

/* Error code for update all-in-one */
#define ERR_NVT_UPDATE_FAILED		-1
#define ERR_NVT_UPDATE_OPENFAILED	-2
#define ERR_NVT_UPDATE_READ_FAILED	-3
#define ERR_NVT_UPDATE_NO_NEED		-4


/*Bininfo boot info. */
#define COMM_UBOOT_BOOT_FUNC_BOOT_UPDFIRM	1
#define COMM_UBOOT_BOOT_FUNC_BOOT_UPD_FRM_USB	2
#define COMM_UBOOT_BOOT_FUNC_BOOT_UPD_FRM_ETH	3
#define COMM_UBOOT_BOOT_FUNC_BOOT_FORMAT_ROOTFS	4
#define COMM_UBOOT_BOOT_FUNC_BOOT_RECOVERY_SYS	5
#define COMM_UBOOT_BOOT_FUNC_BOOT_REASON_MASK	0xff << 0
#define COMM_UBOOT_BOOT_FUNC_BOOT_DONE		1 << 8
#define COMM_UBOOT_BOOT_FUNC_BOOT_NG		2 << 8
#define COMM_UBOOT_BOOT_FUNC_BOOT_DONE_MASK	0xff << 8
#define COMM_UBOOT_BOOT_FUNC_LOAD_DSP_MASK	 0xff << 16
#define COMM_UBOOT_BOOT_FUNC_LOAD_DSP_DONE		1 << 16
#define COMM_UBOOT_BOOT_FUNC_LOAD_DSP_NG		2 << 16

#define NVT_TIMER0_CNT			0xF0040108

#define nvt_readb(offset)		readb(offset)
#define nvt_readw(offset)		readw(offset)
#define nvt_readl(offset)		readl(offset)

#define nvt_writeb(val, offset)		writeb(val, offset)
#define nvt_writew(val, offset)		writew(val, offset)
#define nvt_writel(val, offset)		writel(val, offset)

#define C_GPIO(pin)	(pin)
#define P_GPIO(pin)	(pin + 0x40)
#define S_GPIO(pin)	(pin + 0x80)
#define L_GPIO(pin)	(pin + 0xA0)
#define H_GPIO(pin)	(pin + 0xC0)
#define D_GPIO(pin)	(pin + 0xE0)

typedef enum
{
	NVT_BIN_NAME_TYPE_FW = 1,
	NVT_BIN_NAME_TYPE_RUNFW,
	NVT_BIN_NAME_TYPE_RECOVERY_FW,
	NVT_BIN_NAME_TYPE_MODELEXT,
} NVT_BIN_NAME_TYPE;

typedef struct _FASTBOOTPSTORE
{
	UINT32 uiDataSize;	///<section size (without header)
	UINT32 uiDataAddr;	///<section addr
	UINT32 uiCheckSum1;	///<make sure this header is intact. value = (uiDataSize | uiDataAddr)
	UINT32 uiCheckSum2;	///<make sure section buffer is intact
} FASTBOOTPSTORE, *PFASTBOOTPSTORE;

int nvt_fw_update(bool firm_upd_firm);
int nvt_chk_all_in_one_valid(unsigned short EmbType, unsigned int addr, unsigned int size, unsigned int id);
int nvt_fw_load_tbin(void);
int nvt_process_all_in_one(ulong addr, unsigned int size, int firm_upd_firm);
int nvt_modelext_init(void);
int nvt_chk_flash_modelext_is_null(void);
int nvt_board_init(void);
int nvt_detect_fw_tbin(void);
void nvt_modelext_bininfo_init(void);
void nvt_ipc_init(void);
unsigned long get_nvt_timer0_cnt(void);
void lz_uncompress(unsigned char *in, unsigned char *out, unsigned int insize);
int get_part(const char *partname, loff_t *off, loff_t *maxsize);
void nvt_disable_mem_protect(void);
#if defined(CONFIG_NVT_LINUX_SPINAND_BOOT) || defined(CONFIG_NVT_LINUX_AUTODETECT) || defined(CONFIG_NVT_LINUX_SPINOR_BOOT) || defined(CONFIG_NVT_LINUX_EMMC_BOOT)
int nvt_part_config(char *p_cmdline, EMB_PARTITION *partition_ptr);
#endif
char *get_nvt_bin_name(NVT_BIN_NAME_TYPE type);
int nvt_check_should_rename_updfw(void);
int nvt_rename_updfw(int reverse);
int nvt_create_delfw(void);
int nvt_delete_delfw(void);
int nvt_process_rootfs_format(void);
#endif
int nvt_check_is_fw_update_fw(void);
int nvt_check_is_fomat_rootfs(void);
int nvt_fs_set_blk_dev(void);
int nvt_process_sys_recovery(void);
int nvt_flash_mark_bad(void);