/**
    NVT common api

    @file       nvt_common.c
    @ingroup
    @note
    Copyright   Novatek Microelectronics Corp. 2016.  All rights reserved.

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License version 2 as
    published by the Free Software Foundation.
*/
#include <common.h>
#include <malloc.h>
#include <fs.h>
#include <jffs2/jffs2.h>
#include <mmc.h>
#include <nand.h>
#include <nvt_type.h>
#include <modelext/modelext_parser.h>
#include <modelext/bin_info.h>
#include <modelext/emb_partition_info.h>
#include <modelext/gpio_info.h>
#include <modelext/top.h>
#include <modelext/interrupt.h>
#include <modelext/dram_partition_info.h>
#include <modelext/model_cfg.h>
#include <asm/arch/nvt_common.h>
#include "nvtpack.h"
#include <asm/arch/nvt_ipc_common.h>

unsigned long nvt_tm0_cnt_beg = 0;
unsigned long nvt_tm0_cnt_end = 0;
unsigned long nvt_binfo_comm_uboot_boot_func = 0;
unsigned long nvt_binfo_comm_core1_start = 0;
unsigned long nvt_binfo_comm_core2_start = 0;
unsigned long nvt_binfo_comm_itron_comp_addr = 0;
unsigned long nvt_binfo_comm_itron_comp_len = 0;
unsigned long nvt_binfo_comm_fw_update_addr = 0;
unsigned long nvt_binfo_comm_fw_update_len = 0;
char *nvt_bin_name = NULL;
char *nvt_bin_name_t = NULL;
char *nvt_bin_name_r = NULL;
char *nvt_bin_name_modelext = NULL;

#ifdef CONFIG_NVT_LINUX_AUTOLOAD
static int nvt_get_part_modelext(void)
{
	extern HEADINFO gHeadInfo;
	/* Loader will load bin and filled bin size in HeadInfo.Resv1[0] */
	unsigned int size = gHeadInfo.Resv1[0];
	NVTPACK_GET_PARTITION_INPUT np_get_input;
	NVTPACK_MEM mem_in = {(void*)CONFIG_NVT_RUNFW_SDRAM_BASE, size};
	NVTPACK_MEM mem_out = {0};

	memset(&np_get_input, 0, sizeof(np_get_input));

	/* Get partition 1(modelext) data */
	np_get_input.id = 1; // modelext must always put in partition[1]
	np_get_input.mem = mem_in;
	if (nvtpack_get_partition(&np_get_input,&mem_out) == NVTPACK_ER_SUCCESS) {
		/* Check for modelext is valid */
		if (strncmp((char*)((unsigned char*)mem_out.p_data+sizeof(MODELEXT_HEADER)), "MODELEXT",8)!=0) {
			printf("partition[1] is not modelext.\r\n");
			return -1;
		}
	}
	/* Copy to modelext ram address base */
	memcpy((void *)CONFIG_MODELEXT_SDRAM_BASE, (void *)mem_out.p_data, mem_out.len);

	return 0;
}
#endif /* CONFIG_NVT_LINUX_AUTOLOAD */

static int nvt_modelext_init_bininfo_replace(void)
{
	int ret;

	memset((unsigned char *)CONFIG_MODELEXT_SDRAM_BASE, 0, CONFIG_MODELEXT_SDRAM_SIZE);
#ifdef CONFIG_NVT_LINUX_AUTOLOAD
	char command[128];
	#if defined(CONFIG_NVT_LINUX_SPINAND_BOOT)
	sprintf(command, "nand read %x %x %x", CONFIG_MODELEXT_SDRAM_BASE, CONFIG_MODELEXT_FLASH_BASE, CONFIG_MODELEXT_SDRAM_SIZE);
	ret = run_command(command, 0);
	#elif defined(CONFIG_NVT_LINUX_SPINOR_BOOT)
	sprintf(command, "sf read 0x%x 0x%x 0x%x", CONFIG_MODELEXT_SDRAM_BASE, CONFIG_MODELEXT_FLASH_BASE, CONFIG_MODELEXT_SDRAM_SIZE);
	ret = run_command(command, 0);
	#elif defined(CONFIG_NVT_LINUX_EMMC_BOOT)
	sprintf(command, "mmc read 0x%x 0x%x 0x%x", CONFIG_MODELEXT_SDRAM_BASE, CONFIG_MODELEXT_FLASH_BASE, CONFIG_MODELEXT_SDRAM_SIZE/MMC_MAX_BLOCK_LEN);
	ret = run_command(command, 0);
	#elif defined(CONFIG_NVT_LINUX_SD_BOOT)
	sprintf(command, "fatload mmc 0:1 0x%x %s", CONFIG_MODELEXT_SDRAM_BASE, get_nvt_bin_name(NVT_BIN_NAME_TYPE_MODELEXT));
	ret = run_command(command, 0);
	#else
	/* All-in-one SD boot */
	ret = nvt_get_part_modelext();
	#endif /* CONFIG_NVT_LINUX_SPINAND_BOOT */
#else /* !CONFIG_NVT_LINUX_AUTOLOAD */
	/* FIXME: To do customized boot */

#endif /* CONFIG_NVT_LINUX_AUTOLOAD */

	if (ret) {
		printf("Modelext init fail return %d\n", ret);
		return ret;
	}

	return 0;
}

static int nvt_modelext_init_bininfo_reserve(void)
{
	int ret = 0;
	MODELEXT_HEADER* p_resource = NULL;
	BININFO *p_bininfo = NULL;
	LDINFO *p_ldinfo_reserve = NULL;

	/* Copy bininfo first */
	p_ldinfo_reserve = malloc(sizeof(*p_ldinfo_reserve));
	memset(p_ldinfo_reserve, 0 , sizeof(*p_ldinfo_reserve));
	p_bininfo = (BININFO*)modelext_get_cfg((unsigned char*)CONFIG_MODELEXT_SDRAM_BASE, MODELEXT_TYPE_BIN_INFO, &p_resource);
	memcpy(p_ldinfo_reserve, &p_bininfo->ld, sizeof(LDINFO));

	/* Reload modelext */
	ret = nvt_modelext_init_bininfo_replace();
	if (ret < 0)
		return ret;

	if (strncmp((char*)((unsigned char*)CONFIG_MODELEXT_SDRAM_BASE+sizeof(MODELEXT_HEADER)),"MODELEXT",8)!=0) {
		printf("modelext is empty\n");
		return -1;
	} else {
		/* Restore bininfo */
		p_bininfo = (BININFO*)modelext_get_cfg((unsigned char*)CONFIG_MODELEXT_SDRAM_BASE, MODELEXT_TYPE_BIN_INFO, &p_resource);
		memcpy(&p_bininfo->ld, p_ldinfo_reserve, sizeof(LDINFO));
		free(p_ldinfo_reserve);

		if (p_resource->version != BIN_INFO_VER) {
			printf("Modelext: bininfo version not match. %08X(emb):%08X(code)\n", p_resource->version, BIN_INFO_VER);
			return -1;
		}
	}

	return 0;
}

static bool nvt_chk_bininfo_is_exist(void)
{
	BININFO *p_bininfo;
	MODELEXT_HEADER *header;

	p_bininfo = (BININFO *)modelext_get_cfg((unsigned char*)CONFIG_MODELEXT_SDRAM_BASE, MODELEXT_TYPE_BIN_INFO, &header);
	if (!p_bininfo || !header) {
		return false;
	} else {
		return true;
	}
}

void nvt_modelext_bininfo_init(void)
{
	BININFO *p_bininfo;
	MODELEXT_HEADER *header;

	p_bininfo = (BININFO *)modelext_get_cfg((unsigned char*)CONFIG_MODELEXT_SDRAM_BASE, MODELEXT_TYPE_BIN_INFO, &header);

	if (strncmp(p_bininfo->ld.LdInfo_1, "LD_NVT", 6) != 0) {
		printf("%sAttention!!!! Please update to latest version loader%s", ANSI_COLOR_RED, ANSI_COLOR_RESET);
		while(1);
	}

	nvt_binfo_comm_uboot_boot_func = (unsigned long)&p_bininfo->comm.Resv[0];
	nvt_binfo_comm_core1_start = (unsigned long)&p_bininfo->comm.Resv[1];
	nvt_binfo_comm_core2_start = (unsigned long)&p_bininfo->comm.Resv[2];
	nvt_binfo_comm_itron_comp_addr = (unsigned long)&p_bininfo->comm.Resv[3];
	nvt_binfo_comm_itron_comp_len = (unsigned long)&p_bininfo->comm.Resv[4];
	nvt_binfo_comm_fw_update_addr = (unsigned long)&p_bininfo->comm.Resv[5];
	nvt_binfo_comm_fw_update_len = (unsigned long)&p_bininfo->comm.Resv[6];
}

extern void nvt_ipc_init(void)
{
	NVTIPC_SHM_CTRL* pShmCtrl;
	DRAM_PARTITION *p_dram_partition;
	MODELEXT_HEADER *header;

	p_dram_partition = (DRAM_PARTITION *)modelext_get_cfg((unsigned char*)CONFIG_MODELEXT_SDRAM_BASE, MODELEXT_TYPE_DRAM_PARTITION, &header);
	if (!p_dram_partition || !header) {
		printf("Modelext: dram_partition is null\n");
		return;
	}

	pShmCtrl = (NVTIPC_SHM_CTRL*)NVTIPC_SHARED_MEM_ADDR;
	memset((void*)pShmCtrl, 0 , NVTIPC_SHARED_MEM_SIZE);

	pShmCtrl->SemBits = 0xFFFFFFFF;
	pShmCtrl->ddr1_addr = p_dram_partition->dram_addr;
	pShmCtrl->ddr1_size = p_dram_partition->dram_size;
	pShmCtrl->uitron_addr = p_dram_partition->uitron_addr;
	pShmCtrl->uitron_size = p_dram_partition->uitron_size;
	pShmCtrl->ddr2_addr = p_dram_partition->extdram_addr;
	pShmCtrl->ddr2_size = p_dram_partition->extdram_size;

	// should be last
	pShmCtrl->InterfaceVer = NVTIPC_INTERFACE_VER;
	flush_dcache_range(NVTIPC_SHARED_MEM_ADDR,NVTIPC_SHARED_MEM_ADDR+NVTIPC_SHARED_MEM_SIZE);
	#if 0
	// make sure the bits is writen to dram
	__asm__ __volatile__("dsb\n\t");
	while (pShmCtrl->InterfaceVer != NVTIPC_INTERFACE_VER) {
		;
	}
	#endif
}

int nvt_modelext_init(void)
{
	int ret;

	if (nvt_chk_bininfo_is_exist())
		ret = nvt_modelext_init_bininfo_reserve();
	else
		ret = nvt_modelext_init_bininfo_replace();
	if (ret < 0)
		return ret;

	//nvt_modelext_bininfo_init();
	return ret;
}

int nvt_chk_flash_modelext_is_null(void)
{
	int ret;
	char command[128] = {0};

	void *tmp_model_ptr = memalign(ARCH_DMA_MINALIGN, CONFIG_MODELEXT_SDRAM_SIZE);
	#if defined(CONFIG_NVT_LINUX_SPINAND_BOOT)
	sprintf(command, "nand read %x %x %x", (unsigned int)tmp_model_ptr, CONFIG_MODELEXT_FLASH_BASE, CONFIG_MODELEXT_SDRAM_SIZE);
	#elif defined(CONFIG_NVT_LINUX_EMMC_BOOT)
	sprintf(command, "mmc read 0x%x 0x%x 0x%x", (unsigned int)tmp_model_ptr, CONFIG_MODELEXT_FLASH_BASE, CONFIG_MODELEXT_SDRAM_SIZE/MMC_MAX_BLOCK_LEN);
	#else
	sprintf(command, "sf read 0x%x 0x%x 0x%x", (unsigned int)tmp_model_ptr, CONFIG_MODELEXT_FLASH_BASE, CONFIG_MODELEXT_SDRAM_SIZE);
	#endif
	ret = run_command(command, 0);
	if (ret < 0)
		return ret;

	ret = strncmp((char*)((unsigned char*)tmp_model_ptr+sizeof(MODELEXT_HEADER)),"MODELEXT",8);
	free(tmp_model_ptr);
	if(ret != 0) {
		return 1;
	} else {
		return 0;
	}
}

static int nvt_pinmux_init(void)
{
	MODELEXT_HEADER *header;
	PIN_GROUP_CONFIG *p_pinmux;

	p_pinmux = (PIN_GROUP_CONFIG *)modelext_get_cfg((unsigned char*)CONFIG_MODELEXT_SDRAM_BASE, MODELEXT_TYPE_PINMUX_CFG, &header);
	if (!p_pinmux || !header) {
		printf("Modelext: pinmux is null\n");
		return -1;
	}

	if (header->version != PIN_GROUP_CONFIG_VER) {
		printf("Modelext: pinmux version not match. %08X(emb):%08X(code)\n", header->version, PIN_GROUP_CONFIG_VER);
		return -1;
	}

	pinmux_init(p_pinmux);

	return 0;
}

static int nvt_iocfg_init(void)
{
	MODELEXT_HEADER *header;
	GPIO_INIT_OBJ *p_iocfg;

	p_iocfg = (GPIO_INIT_OBJ *)modelext_get_cfg((unsigned char*)CONFIG_MODELEXT_SDRAM_BASE, MODELEXT_TYPE_GPIO_INFO, &header);

	if (!p_iocfg || !header) {
		printf("Modelext: iocfg is null\n");
		return -1;
	}

	if (header->version != GPIO_INFO_VER) {
		printf("Modelext: iocfg version not match. %08X(emb):%08X(code)\n", header->version, GPIO_INFO_VER);
		return -1;
	}

	iocfg_init(p_iocfg, header->number);

	return 0;
}

#ifndef _NVT_LINUX_SMP_ON_
static int nvt_intdir_init(void)
{
	MODELEXT_HEADER *header;
	INT_GIC_DST *p_intdir;

	p_intdir = (INT_GIC_DST *)modelext_get_cfg((unsigned char*)CONFIG_MODELEXT_SDRAM_BASE, MODELEXT_TYPE_INTDIR_CFG, &header);
	if (!p_intdir || !header) {
		printf("Modelext: Interrupt direction setting is null\n");
		return -1;
	}

	if (header->version != INT_GIC_ID_DST_VER) {
		printf("Modelext: intdir version not match. %08X(emb):%08X(code)\n", header->version, INT_GIC_ID_DST_VER);
		return -1;
	}

	/* IntDir init. */
	int_set_config(0, (UINT32)p_intdir);

	return 0;
}
#endif /* !_NVT_LINUX_SMP_ON_ */

static int nvt_emb_partition_init(void)
{
	//here only check valid. The real partition action is at nvt_fw_update.c
	MODELEXT_HEADER *header;
	EMB_PARTITION *p_emb_partition;

	p_emb_partition = (EMB_PARTITION *)modelext_get_cfg((unsigned char*)CONFIG_MODELEXT_SDRAM_BASE, MODELEXT_TYPE_EMB_PARTITION, &header);
	if (!p_emb_partition || !header) {
		printf("Modelext: emb_partition setting is null\n");
		return -1;
	}

	//TODO: wait SA1 implement
	if (header->version != EMB_PARTITION_INFO_VER) {
		printf("Modelext: intdir version not match. %08X(emb):%08X(code)\n", header->version, EMB_PARTITION_INFO_VER);
		return -1;
	}

	return 0;
}

static int nvt_dram_partition_init(void)
{
	//here only check valid. The real partition action is at nvt_fw_update.c
	MODELEXT_HEADER *header;
	DRAM_PARTITION *p_dram_partition;

	p_dram_partition = (DRAM_PARTITION *)modelext_get_cfg((unsigned char*)CONFIG_MODELEXT_SDRAM_BASE, MODELEXT_TYPE_DRAM_PARTITION, &header);
	if (!p_dram_partition || !header) {
		printf("Modelext: dram_partition is null\n");
		return -1;
	}

	//TODO: wait SA1 implement
	if (header->version != DRAM_PARTITION_INFO_VER) {
		printf("Modelext: dram_partition version not match. %08X(emb):%08X(code)\n", header->version, DRAM_PARTITION_INFO_VER);
		return -1;
	}

	return 0;
}

static int nvt_model_cfg_init(void)
{
	//here only check valid. The real partition action is at nvt_fw_update.c
	MODELEXT_HEADER *header;
	char *p_model_cfg;

	p_model_cfg = (char *)modelext_get_cfg((unsigned char*)CONFIG_MODELEXT_SDRAM_BASE, MODELEXT_TYPE_MODEL_CFG, &header);
	if (!p_model_cfg || !header) {
		printf("Modelext: model_cfg is null\n");
		return -1;
	}

	//TODO: wait SA1 implement
	if (header->version != MODEL_CFG_VER) {
		printf("Modelext: model_cfg version not match. %08X(emb):%08X(code)\n", header->version, MODEL_CFG_VER);
		return -1;
	}

	return 0;
}

int nvt_board_init(void)
{
	int ret = 0;

	if (strncmp((char*)((unsigned char*)CONFIG_MODELEXT_SDRAM_BASE+sizeof(MODELEXT_HEADER)),"MODELEXT",8)!=0) {
		printf("modelext is empty\n");
		return -1;
	}

	if(nvt_check_is_fw_update_fw() || nvt_check_is_fomat_rootfs()){
		printf("no need to reset pinmux/gpio/rtc\r\n");
		return 0;
	}

	ret = nvt_pinmux_init();
	if (ret < 0)
		return ret;

	ret = nvt_iocfg_init();
	if (ret < 0)
		return ret;

#ifndef _NVT_LINUX_SMP_ON_
	ret = nvt_intdir_init();
	if (ret < 0)
		return ret;
#endif /* _NVT_LINUX_SMP_ON_ */

	ret = nvt_emb_partition_init();
	if (ret < 0)
		return ret;

	ret = nvt_dram_partition_init();
	if (ret < 0)
		return ret;

	nvt_rtc_init();

	return 0;
}

int nvt_fw_load_tbin(void)
{
	int size = 0;

	if (nvt_fs_set_blk_dev()) 
		return ERR_NVT_UPDATE_OPENFAILED;
	else {
		if (!fs_exists(get_nvt_bin_name(NVT_BIN_NAME_TYPE_RUNFW))) {
			return ERR_NVT_UPDATE_NO_NEED;
		}
	}

	if (nvt_fs_set_blk_dev())
		return ERR_NVT_UPDATE_OPENFAILED;
	else {
		size = fs_read(get_nvt_bin_name(NVT_BIN_NAME_TYPE_RUNFW), (ulong)CONFIG_NVT_RUNFW_SDRAM_BASE, 0, 0);
		if (size <= 0) {
			printf("Read %s at 0x%x failed ret=%d\n", get_nvt_bin_name(NVT_BIN_NAME_TYPE_RUNFW), CONFIG_NVT_RUNFW_SDRAM_BASE, size);
			return ERR_NVT_UPDATE_READ_FAILED;
		} else {
			printf("Read %s at 0x%x successfully, size=%d\n", get_nvt_bin_name(NVT_BIN_NAME_TYPE_RUNFW), CONFIG_NVT_RUNFW_SDRAM_BASE, size);
		}
	}

	return 0;
}

int nvt_detect_fw_tbin(void)
{
	MODELEXT_HEADER* p_resource = NULL;
	BININFO *p_bininfo = NULL;
	LDINFO *p_ldinfo = NULL;

	p_bininfo = (BININFO*)modelext_get_cfg((unsigned char*)CONFIG_MODELEXT_SDRAM_BASE, MODELEXT_TYPE_BIN_INFO, &p_resource);
	p_ldinfo = (LDINFO*)&p_bininfo->ld;

	if (!p_bininfo)
		return -1;

	if (p_ldinfo->LdCtrl2 & LDCF_BOOT_CARD) {
		/* T.bin */
		printf("%s Boot from SD card\n", __func__);
		return 1;
	} else {
		/* A.bin */
		printf("%s Boot from flash or emmc\n", __func__);
		return 0;
	}
}

/*************************************************************************
* _LZ_ReadVarSize() - Read unsigned integer with variable number of
* bytes depending on value.
*************************************************************************/

static unsigned int _lz_read_var_size(unsigned int *x, unsigned char *buf)
{
	unsigned int y, b, num_bytes;

	/* Read complete value (stop when byte contains zero in 8:th bit) */
	y = 0;
	num_bytes = 0;
	do
	{
		b = (unsigned int) (*buf ++);
		y = (y << 7) | (b & 0x0000007f);
		++ num_bytes;
	}
	while( b & 0x00000080 );

	/* Store value in x */
	*x = y;

	/* Return number of bytes read */
	return num_bytes;
}

/**
	LZ Uncompress

	Uncompress/decompress by LZ77 decoder

	@param[in] in       Input buffer of compressed data
	@param[out] out     Output buffer to store decompressed data (Caller should maintain large enough buffer size to hold decompressed data)
	@param[in] insize   Length of input buffer in

	@return void
*/
void lz_uncompress(unsigned char *in, unsigned char *out, unsigned int insize)
{
	unsigned char marker, symbol;
	unsigned int  i, inpos, outpos, length, offset;

	/* Do we have anything to uncompress? */
	if( insize < 1 )
	{
		return;
	}

	/* Get marker symbol from input stream */
	marker = in[ 0 ];
	inpos = 1;

	/* Main decompression loop */
	outpos = 0;
	do
	{
		symbol = in[ inpos ++ ];
		if( symbol == marker )
		{
			/* We had a marker byte */
			if( in[ inpos ] == 0 )
			{
				/* It was a single occurrence of the marker byte */
				out[ outpos ++ ] = marker;
				++ inpos;
			}
			else
			{
				/* Extract true length and offset */
				inpos += _lz_read_var_size( &length, &in[ inpos ] );
				inpos += _lz_read_var_size( &offset, &in[ inpos ] );

				/* Copy corresponding data from history window */
				for( i = 0; i < length; ++ i )
				{
					out[ outpos ] = out[ outpos - offset ];
					++ outpos;
				}
			}
		}
		else
		{
			/* No marker, plain copy */
			out[ outpos ++ ] = symbol;
		}
	}
	while( inpos < insize );
}

int get_part(const char *partname, loff_t *off, loff_t *maxsize)
{
#if defined(CONFIG_NVT_LINUX_EMMC_BOOT)
	char cmdline[1024] = {0}, nvtemmcpart[32] = {0};
	char *nvtemmcpart_off = NULL, *nvtemmcpart_off_next = NULL, *nvtemmcpart_off_end = NULL, *tmp = NULL;
	u32 nvtemmcpart_sz = 0;

	sprintf(cmdline,"%s ",getenv("bootargs"));
	nvtemmcpart_off = strstr((char *)cmdline, "nvtemmcpart=") + strlen("nvtemmcpart=") - 1;
	nvtemmcpart_off_end = strstr((char *)nvtemmcpart_off, " ");
	nvtemmcpart_off_next = strstr((char *)nvtemmcpart_off, ",");
	*maxsize = 0;
	*off = 0;

	if (nvtemmcpart_off == NULL || ((u32)nvtemmcpart_off_end - (u32)nvtemmcpart_off) < 20)
		return -1;
	do
	{
		memset(nvtemmcpart, 0, sizeof(nvtemmcpart));
		nvtemmcpart_sz = (u32)(nvtemmcpart_off_next - nvtemmcpart_off - 1);
		strncpy(nvtemmcpart, nvtemmcpart_off+1, nvtemmcpart_sz);
		nvtemmcpart_off = nvtemmcpart_off_next;
		nvtemmcpart_off_next = strstr((char *)nvtemmcpart_off_next+1, ",");

		if (strstr(nvtemmcpart, partname) != NULL) {
			*maxsize = simple_strtoul(nvtemmcpart, &tmp, 0);
			*off = simple_strtoul((tmp + 1), NULL, 0);
			break;
		}
	} while((u32)nvtemmcpart_off_next < (u32)nvtemmcpart_off_end && (u32)nvtemmcpart_off_next != 0);

	return 0;
#elif defined(CONFIG_CMD_MTDPARTS)
	struct mtd_device *dev;
	struct part_info *part;
	u8 pnum;
	int ret;

	ret = mtdparts_init();
	if (ret)
		return ret;

	ret = find_dev_and_part(partname, &dev, &pnum, &part);
	if (ret)
		return ret;

	*off = part->offset;
	*maxsize = part->size;

	return 0;
#else
	puts("offset is not a number\n");
	return -1;
#endif
}

void nvt_disable_mem_protect(void)
{
	nvt_writel(0,0xF0000060);
}

unsigned long get_nvt_timer0_cnt(void)
{
	return nvt_readl((ulong)NVT_TIMER0_CNT);
}

/*
 * get from bin info.
 * select:
 *		0: FW_NAME
 *		1: RUNFW_NAME
 *		2: MODELEXT_NAME
 */
char *get_nvt_bin_name(NVT_BIN_NAME_TYPE type)
{
	BININFO *p_bininfo = NULL;
	MODELEXT_HEADER* p_resource = NULL;

	p_bininfo = (BININFO*)modelext_get_cfg((unsigned char*)CONFIG_MODELEXT_SDRAM_BASE, MODELEXT_TYPE_BIN_INFO, &p_resource);
	if (nvt_bin_name == NULL) {
		nvt_bin_name = calloc(sizeof(char),15);
		if (!nvt_bin_name) {
			printf( "%s: allocation failure \n", __FUNCTION__);
			return NULL;
		}
		strcpy(nvt_bin_name, p_bininfo->fw.FwInfo_1);
		strcat(nvt_bin_name, ".bin");
	}

	if ( nvt_bin_name_t == NULL) {
		nvt_bin_name_t = calloc(sizeof(char),15);
		if (!nvt_bin_name_t) {
			printf( "%s: allocation failure \n", __FUNCTION__);
			free(nvt_bin_name);
			return NULL;
		}
		strcpy(nvt_bin_name_t, p_bininfo->fw.FwInfo_1);
		nvt_bin_name_t[strlen(nvt_bin_name_t) - 1] = '\0';
		strcat(nvt_bin_name_t, "T.bin");
	}

	if ( nvt_bin_name_r == NULL) {
		nvt_bin_name_r = calloc(sizeof(char),15);
		if (!nvt_bin_name_r) {
			printf( "%s: allocation failure \n", __FUNCTION__);
			free(nvt_bin_name);
			free(nvt_bin_name_t);
			return NULL;
		}
		strcpy(nvt_bin_name_r, p_bininfo->fw.FwInfo_1);
		nvt_bin_name_r[strlen(nvt_bin_name_r) - 1] = '\0';
		strcat(nvt_bin_name_r, "R.bin");
	}

	if ( nvt_bin_name_modelext == NULL) {
		nvt_bin_name_modelext = calloc(sizeof(char),15);
		if (!nvt_bin_name_modelext) {
			printf( "%s: allocation failure \n", __FUNCTION__);
			free(nvt_bin_name);
			free(nvt_bin_name_t);
			free(nvt_bin_name_r);
			return NULL;
		}
		strcpy(nvt_bin_name_modelext, p_bininfo->fw.FwInfo_1);
		strcat(nvt_bin_name_modelext, ".ext.bin");
	}

	if (type == NVT_BIN_NAME_TYPE_FW) {
		return nvt_bin_name;
	} else if (type == NVT_BIN_NAME_TYPE_RUNFW) {
		return nvt_bin_name_t;
	} else if (type == NVT_BIN_NAME_TYPE_RECOVERY_FW) {
		return nvt_bin_name_r;
	} else {
		return nvt_bin_name_modelext;
	}
}

int nvt_check_is_fw_update_fw(void)
{
	unsigned long      boot_reason;

	boot_reason = nvt_readl((ulong)nvt_binfo_comm_uboot_boot_func) & COMM_UBOOT_BOOT_FUNC_BOOT_REASON_MASK;

	return (boot_reason == COMM_UBOOT_BOOT_FUNC_BOOT_UPDFIRM);
}

int nvt_check_is_fomat_rootfs(void)
{
	unsigned long      boot_reason;
	static int         is_format = -1;

	if(is_format != -1)
		return is_format;

	boot_reason = nvt_readl((ulong)nvt_binfo_comm_uboot_boot_func) & COMM_UBOOT_BOOT_FUNC_BOOT_REASON_MASK;
	is_format = ((boot_reason == COMM_UBOOT_BOOT_FUNC_BOOT_FORMAT_ROOTFS) ? 1 : 0);

	return is_format;
}

int nvt_fs_set_blk_dev(void)
{
	if (fs_set_blk_dev("mmc", "0:1", FS_TYPE_FAT))
		if (fs_set_blk_dev("mmc", "0:1", FS_TYPE_EXFAT))
			if (fs_set_blk_dev("mmc", "0:0", FS_TYPE_FAT))
				if (fs_set_blk_dev("mmc", "0:0", FS_TYPE_EXFAT)){
			                printf("MMC interface configure failed\n");
			                return -1;
				}

	return 0;
}

int nvt_flash_mark_bad(void)
{
	#if defined(CONFIG_NVT_LINUX_SPINAND_BOOT)
	int i = 0, ret = 0;
	nand_info_t *nand = &nand_info[0];

	for (i = 0; i < 100; i++) {
		if (nvt_flash_ecc_err_pages[i] != 0) {
			ret = mtd_block_markbad(nand, (loff_t)nvt_flash_ecc_err_pages[i]);
			if (ret) {
				printf("cannot mark nand flash block %d as bad, error %d", nvt_flash_ecc_err_pages[i], ret);
				return ret;
			}
		}
	}
	#endif
	return 0;
}