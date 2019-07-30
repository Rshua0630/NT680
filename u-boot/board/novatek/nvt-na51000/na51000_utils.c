/**
    NVT utilities for command customization

    @file       nt96680_utils.c
    @ingroup
    @note
    Copyright   Novatek Microelectronics Corp. 2016.  All rights reserved.

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License version 2 as
    published by the Free Software Foundation.
*/
#include <common.h>
#include <fs.h>
#include <asm/io.h>
#include <nand.h>
#include <mmc.h>
#include <spi_flash.h>
#include <lzma/LzmaTools.h>
#include <modelext/modelext_parser.h>
#include <modelext/emb_partition_info.h>
#include <modelext/dram_partition_info.h>
#include <modelext/bin_info.h>
#include "na51000_utils.h"
#include "nvtpack.h"
#include <asm/arch/nvt_common.h>
#include "gxdsp.h"

#define ER_DSP_ASSERT (-1)
#define ER_DSP_NO_EMB_PARTITION (-2)
#define ER_DSP_NVTPACK_NO_PARTITION (-3)
#define ER_DSP_NO_DSP_PARTITION (-4)
#define ER_DSP_NO_MEM (-5)
#define ER_DSP_INVALID (-6)

#define FW_PART1_SIZE_OFFSET (0x30C) //ref to CodeInfo.S on uitron

int SDIO_DS_MODE_CLK_DRIVING = 0;
int SDIO_DS_MODE_DATA_DRIVING = 0;
int SDIO_DS_MODE_CMD_DRIVING = 0;
int SDIO_HS_MODE_CLK_DRIVING = 0;
int SDIO_HS_MODE_DATA_DRIVING = 0;
int SDIO_HS_MODE_CMD_DRIVING = 0;
int SDIO_SDR50_MODE_CLK_DRIVING = 0;
int SDIO_SDR50_MODE_DATA_DRIVING = 0;
int SDIO_SDR50_MODE_CMD_DRIVING = 0;
int SDIO_SDR104_MODE_CLK_DRIVING = 0;
int SDIO_SDR104_MODE_DATA_DRIVING = 0;
int SDIO_SDR104_MODE_CMD_DRIVING = 0;

typedef int (*DSP_STORAGE_READ_FP)(int dsp_index, unsigned int dst_addr, unsigned int ofs, unsigned int size);

extern HEADINFO gHeadInfo;
#ifdef CONFIG_NVT_LINUX_SPINOR_BOOT
extern struct spi_flash *board_init_spi_flash;
#endif

void parsing_sdio_driving(void)
{
#if defined(_NVT_DS_MODE_DATA_PAD_DRIVING_40MA_)
	SDIO_DS_MODE_DATA_DRIVING = 40;
#elif defined(_NVT_DS_MODE_DATA_PAD_DRIVING_10MA_)
	SDIO_DS_MODE_DATA_DRIVING = 10;
#elif defined(_NVT_DS_MODE_DATA_PAD_DRIVING_15MA_)
	SDIO_DS_MODE_DATA_DRIVING = 15;
#elif defined(_NVT_DS_MODE_DATA_PAD_DRIVING_20MA_)
	SDIO_DS_MODE_DATA_DRIVING = 20;
#elif defined(_NVT_DS_MODE_DATA_PAD_DRIVING_25MA_)
	SDIO_DS_MODE_DATA_DRIVING = 25;
#elif defined(_NVT_DS_MODE_DATA_PAD_DRIVING_30MA_)
	SDIO_DS_MODE_DATA_DRIVING = 30;
#elif defined(_NVT_DS_MODE_DATA_PAD_DRIVING_35MA_)
	SDIO_DS_MODE_DATA_DRIVING = 35;
#else
	SDIO_DS_MODE_DATA_DRIVING = 5;
#endif

#if defined(_NVT_DS_MODE_CMD_PAD_DRIVING_40MA_)
	SDIO_DS_MODE_CMD_DRIVING = 5;
#elif defined(_NVT_DS_MODE_CMD_PAD_DRIVING_10MA_)
	SDIO_DS_MODE_CMD_DRIVING = 10;
#elif defined(_NVT_DS_MODE_CMD_PAD_DRIVING_15MA_)
	SDIO_DS_MODE_CMD_DRIVING = 15;
#elif defined(_NVT_DS_MODE_CMD_PAD_DRIVING_20MA_)
	SDIO_DS_MODE_CMD_DRIVING = 20;
#elif defined(_NVT_DS_MODE_CMD_PAD_DRIVING_25MA_)
	SDIO_DS_MODE_CMD_DRIVING = 25;
#elif defined(_NVT_DS_MODE_CMD_PAD_DRIVING_30MA_)
	SDIO_DS_MODE_CMD_DRIVING = 30;
#elif defined(_NVT_DS_MODE_CMD_PAD_DRIVING_35MA_)
	SDIO_DS_MODE_CMD_DRIVING = 35;
#else
	SDIO_DS_MODE_CMD_DRIVING = 5;
#endif

#if defined(_NVT_DS_MODE_CLK_PAD_DRIVING_40MA_)
	SDIO_DS_MODE_CLK_DRIVING = 40;
#elif defined(_NVT_DS_MODE_CLK_PAD_DRIVING_10MA_)
	SDIO_DS_MODE_CLK_DRIVING = 10;
#elif defined(_NVT_DS_MODE_CLK_PAD_DRIVING_15MA_)
	SDIO_DS_MODE_CLK_DRIVING = 15;
#elif defined(_NVT_DS_MODE_CLK_PAD_DRIVING_20MA_)
	SDIO_DS_MODE_CLK_DRIVING = 20;
#elif defined(_NVT_DS_MODE_CLK_PAD_DRIVING_25MA_)
	SDIO_DS_MODE_CLK_DRIVING = 25;
#elif defined(_NVT_DS_MODE_CLK_PAD_DRIVING_30MA_)
	SDIO_DS_MODE_CLK_DRIVING = 30;
#elif defined(_NVT_DS_MODE_CLK_PAD_DRIVING_35MA_)
	SDIO_DS_MODE_CLK_DRIVING = 35;
#else
	SDIO_DS_MODE_CLK_DRIVING = 5;
#endif

#if defined(_NVT_HS_MODE_DATA_PAD_DRIVING_40MA_)
	SDIO_HS_MODE_DATA_DRIVING = 40;
#elif defined(_NVT_HS_MODE_DATA_PAD_DRIVING_10MA_)
	SDIO_HS_MODE_DATA_DRIVING = 10;
#elif defined(_NVT_HS_MODE_DATA_PAD_DRIVING_15MA_)
	SDIO_HS_MODE_DATA_DRIVING = 15;
#elif defined(_NVT_HS_MODE_DATA_PAD_DRIVING_20MA_)
	SDIO_HS_MODE_DATA_DRIVING = 20;
#elif defined(_NVT_HS_MODE_DATA_PAD_DRIVING_25MA_)
	SDIO_HS_MODE_DATA_DRIVING = 25;
#elif defined(_NVT_HS_MODE_DATA_PAD_DRIVING_30MA_)
	SDIO_HS_MODE_DATA_DRIVING = 30;
#elif defined(_NVT_HS_MODE_DATA_PAD_DRIVING_35MA_)
	SDIO_HS_MODE_DATA_DRIVING = 35;
#else
	SDIO_HS_MODE_DATA_DRIVING = 5;
#endif

#if defined(_NVT_HS_MODE_CMD_PAD_DRIVING_40MA_)
	SDIO_HS_MODE_CMD_DRIVING = 40;
#elif defined(_NVT_HS_MODE_CMD_PAD_DRIVING_10MA_)
	SDIO_HS_MODE_CMD_DRIVING = 10;
#elif defined(_NVT_HS_MODE_CMD_PAD_DRIVING_15MA_)
	SDIO_HS_MODE_CMD_DRIVING = 15;
#elif defined(_NVT_HS_MODE_CMD_PAD_DRIVING_20MA_)
	SDIO_HS_MODE_CMD_DRIVING = 20;
#elif defined(_NVT_HS_MODE_CMD_PAD_DRIVING_25MA_)
	SDIO_HS_MODE_CMD_DRIVING = 25;
#elif defined(_NVT_HS_MODE_CMD_PAD_DRIVING_30MA_)
	SDIO_HS_MODE_CMD_DRIVING = 30;
#elif defined(_NVT_HS_MODE_CMD_PAD_DRIVING_35MA_)
	SDIO_HS_MODE_CMD_DRIVING = 35;
#else
	SDIO_HS_MODE_CMD_DRIVING = 5;
#endif

#if defined(_NVT_HS_MODE_CLK_PAD_DRIVING_40MA_)
	SDIO_HS_MODE_CLK_DRIVING = 40;
#elif defined(_NVT_HS_MODE_CLK_PAD_DRIVING_10MA_)
	SDIO_HS_MODE_CLK_DRIVING = 10;
#elif defined(_NVT_HS_MODE_CLK_PAD_DRIVING_15MA_)
	SDIO_HS_MODE_CLK_DRIVING = 15;
#elif defined(_NVT_HS_MODE_CLK_PAD_DRIVING_20MA_)
	SDIO_HS_MODE_CLK_DRIVING = 20;
#elif defined(_NVT_HS_MODE_CLK_PAD_DRIVING_25MA_)
	SDIO_HS_MODE_CLK_DRIVING = 25;
#elif defined(_NVT_HS_MODE_CLK_PAD_DRIVING_30MA_)
	SDIO_HS_MODE_CLK_DRIVING = 30;
#elif defined(_NVT_HS_MODE_CLK_PAD_DRIVING_35MA_)
	SDIO_HS_MODE_CLK_DRIVING = 35;
#else
	SDIO_HS_MODE_CLK_DRIVING = 5;
#endif

#if defined(_NVT_SDR50_MODE_DATA_PAD_DRIVING_40MA_)
	SDIO_SDR50_MODE_DATA_DRIVING = 40;
#elif defined(_NVT_SDR50_MODE_DATA_PAD_DRIVING_10MA_)
	SDIO_SDR50_MODE_DATA_DRIVING = 10;
#elif defined(_NVT_SDR50_MODE_DATA_PAD_DRIVING_15MA_)
	SDIO_SDR50_MODE_DATA_DRIVING = 15;
#elif defined(_NVT_SDR50_MODE_DATA_PAD_DRIVING_20MA_)
	SDIO_SDR50_MODE_DATA_DRIVING = 20;
#elif defined(_NVT_SDR50_MODE_DATA_PAD_DRIVING_25MA_)
	SDIO_SDR50_MODE_DATA_DRIVING = 25;
#elif defined(_NVT_SDR50_MODE_DATA_PAD_DRIVING_30MA_)
	SDIO_SDR50_MODE_DATA_DRIVING = 30;
#elif defined(_NVT_SDR50_MODE_DATA_PAD_DRIVING_35MA_)
	SDIO_SDR50_MODE_DATA_DRIVING = 35;
#else
	SDIO_SDR50_MODE_DATA_DRIVING = 5;
#endif

#if defined(_NVT_SDR50_MODE_CMD_PAD_DRIVING_40MA_)
	SDIO_SDR50_MODE_CMD_DRIVING = 40;
#elif defined(_NVT_SDR50_MODE_CMD_PAD_DRIVING_10MA_)
	SDIO_SDR50_MODE_CMD_DRIVING = 10;
#elif defined(_NVT_SDR50_MODE_CMD_PAD_DRIVING_15MA_)
	SDIO_SDR50_MODE_CMD_DRIVING = 15;
#elif defined(_NVT_SDR50_MODE_CMD_PAD_DRIVING_20MA_)
	SDIO_SDR50_MODE_CMD_DRIVING = 20;
#elif defined(_NVT_SDR50_MODE_CMD_PAD_DRIVING_25MA_)
	SDIO_SDR50_MODE_CMD_DRIVING = 25;
#elif defined(_NVT_SDR50_MODE_CMD_PAD_DRIVING_30MA_)
	SDIO_SDR50_MODE_CMD_DRIVING = 30;
#elif defined(_NVT_SDR50_MODE_CMD_PAD_DRIVING_35MA_)
	SDIO_SDR50_MODE_CMD_DRIVING = 35;
#else
	SDIO_SDR50_MODE_CMD_DRIVING = 5;
#endif

#if defined(_NVT_SDR50_MODE_CLK_PAD_DRIVING_40MA_)
	SDIO_SDR50_MODE_CLK_DRIVING = 40;
#elif defined(_NVT_SDR50_MODE_CLK_PAD_DRIVING_10MA_)
	SDIO_SDR50_MODE_CLK_DRIVING = 10;
#elif defined(_NVT_SDR50_MODE_CLK_PAD_DRIVING_15MA_)
	SDIO_SDR50_MODE_CLK_DRIVING = 15;
#elif defined(_NVT_SDR50_MODE_CLK_PAD_DRIVING_20MA_)
	SDIO_SDR50_MODE_CLK_DRIVING = 20;
#elif defined(_NVT_SDR50_MODE_CLK_PAD_DRIVING_25MA_)
	SDIO_SDR50_MODE_CLK_DRIVING = 25;
#elif defined(_NVT_SDR50_MODE_CLK_PAD_DRIVING_30MA_)
	SDIO_SDR50_MODE_CLK_DRIVING = 30;
#elif defined(_NVT_SDR50_MODE_CLK_PAD_DRIVING_35MA_)
	SDIO_SDR50_MODE_CLK_DRIVING = 35;
#else
	SDIO_SDR50_MODE_CLK_DRIVING = 5;
#endif

#if defined(_NVT_SDR104_MODE_DATA_PAD_DRIVING_40MA_)
	SDIO_SDR104_MODE_DATA_DRIVING = 40;
#elif defined(_NVT_SDR104_MODE_DATA_PAD_DRIVING_10MA_)
	SDIO_SDR104_MODE_DATA_DRIVING = 10;
#elif defined(_NVT_SDR104_MODE_DATA_PAD_DRIVING_15MA_)
	SDIO_SDR104_MODE_DATA_DRIVING = 15;
#elif defined(_NVT_SDR104_MODE_DATA_PAD_DRIVING_20MA_)
	SDIO_SDR104_MODE_DATA_DRIVING = 20;
#elif defined(_NVT_SDR104_MODE_DATA_PAD_DRIVING_25MA_)
	SDIO_SDR104_MODE_DATA_DRIVING = 25;
#elif defined(_NVT_SDR104_MODE_DATA_PAD_DRIVING_30MA_)
	SDIO_SDR104_MODE_DATA_DRIVING = 30;
#elif defined(_NVT_SDR104_MODE_DATA_PAD_DRIVING_35MA_)
	SDIO_SDR104_MODE_DATA_DRIVING = 35;
#else
	SDIO_SDR104_MODE_DATA_DRIVING = 5;
#endif

#if defined(_NVT_SDR104_MODE_CMD_PAD_DRIVING_40MA_)
	SDIO_SDR104_MODE_CMD_DRIVING = 40;
#elif defined(_NVT_SDR104_MODE_CMD_PAD_DRIVING_10MA_)
	SDIO_SDR104_MODE_CMD_DRIVING = 10;
#elif defined(_NVT_SDR104_MODE_CMD_PAD_DRIVING_15MA_)
	SDIO_SDR104_MODE_CMD_DRIVING = 15;
#elif defined(_NVT_SDR104_MODE_CMD_PAD_DRIVING_20MA_)
	SDIO_SDR104_MODE_CMD_DRIVING = 20;
#elif defined(_NVT_SDR104_MODE_CMD_PAD_DRIVING_25MA_)
	SDIO_SDR104_MODE_CMD_DRIVING = 25;
#elif defined(_NVT_SDR104_MODE_CMD_PAD_DRIVING_30MA_)
	SDIO_SDR104_MODE_CMD_DRIVING = 30;
#elif defined(_NVT_SDR104_MODE_CMD_PAD_DRIVING_35MA_)
	SDIO_SDR104_MODE_CMD_DRIVING = 35;
#else
	SDIO_SDR104_MODE_CMD_DRIVING = 5;
#endif

#if defined(_NVT_SDR104_MODE_CLK_PAD_DRIVING_40MA_)
	SDIO_SDR104_MODE_CLK_DRIVING = 40;
#elif defined(_NVT_SDR104_MODE_CLK_PAD_DRIVING_10MA_)
	SDIO_SDR104_MODE_CLK_DRIVING = 10;
#elif defined(_NVT_SDR104_MODE_CLK_PAD_DRIVING_15MA_)
	SDIO_SDR104_MODE_CLK_DRIVING = 15;
#elif defined(_NVT_SDR104_MODE_CLK_PAD_DRIVING_20MA_)
	SDIO_SDR104_MODE_CLK_DRIVING = 20;
#elif defined(_NVT_SDR104_MODE_CLK_PAD_DRIVING_25MA_)
	SDIO_SDR104_MODE_CLK_DRIVING = 25;
#elif defined(_NVT_SDR104_MODE_CLK_PAD_DRIVING_30MA_)
	SDIO_SDR104_MODE_CLK_DRIVING = 30;
#elif defined(_NVT_SDR104_MODE_CLK_PAD_DRIVING_35MA_)
	SDIO_SDR104_MODE_CLK_DRIVING = 35;
#else
	SDIO_SDR104_MODE_CLK_DRIVING = 5;
#endif
}

#if defined(CONFIG_NVT_LINUX_SPINAND_BOOT) || defined(CONFIG_NVT_LINUX_AUTOLOAD) || defined(CONFIG_NVT_LINUX_SPINOR_BOOT) || defined(CONFIG_NVT_LINUX_RAMDISK_BOOT) || defined(CONFIG_NVT_LINUX_EMMC_BOOT)
int nvt_part_config(char *p_cmdline, EMB_PARTITION *partition_ptr)
{
	MODELEXT_HEADER* header;
	EMB_PARTITION *p_partition;
	uint32_t i, j, part_num, pst_part_num, ro_attr;
	int ret = 0;
	char cmd[512];
	char buf[40];

	#if defined(CONFIG_NVT_LINUX_RAMDISK_BOOT) || defined(CONFIG_NVT_LINUX_AUTOLOAD)
		#if defined(CONFIG_NVT_SPI_NAND)
			nand_info_t *nand = &nand_info[0];
		#elif defined(CONFIG_NVT_SPI_NOR)
			struct spi_flash *spi_flash = board_init_spi_flash;
		#elif defined(CONFIG_NVT_IVOT_EMMC)
			struct mmc *mmc = find_mmc_device(CONFIG_NVT_IVOT_EMMC);
		#else
			return 0;
		#endif
	#elif defined(CONFIG_NVT_LINUX_SPINAND_BOOT)
	nand_info_t *nand = &nand_info[0];
	#elif defined(CONFIG_NVT_LINUX_EMMC_BOOT)
	struct mmc *mmc = find_mmc_device(CONFIG_NVT_IVOT_EMMC);
	#else
	struct spi_flash *spi_flash = board_init_spi_flash;
	#endif

	#if defined(CONFIG_NVT_LINUX_EMMC_BOOT)
	/* MMC device doesn't need to init mtdparts parameter */
	if (p_cmdline == NULL) {
		return 0;
	}
	#endif /* CONFIG_NVT_LINUX_EMMC_BOOT */

	if (partition_ptr == NULL) {
		p_partition = (EMB_PARTITION*)modelext_get_cfg((unsigned char*)CONFIG_MODELEXT_SDRAM_BASE, MODELEXT_TYPE_EMB_PARTITION, &header);
		if(!p_partition || !header){
			printf("partition//header is null\n");
			return -1;
		}
	} else {
		/* Receive partition table from argu */
		p_partition = partition_ptr;
	}

	#if defined(CONFIG_NVT_SPI_NAND)
	sprintf(cmd, "nand0=spi_nand.0");
	ret = setenv("mtdids",cmd);
	if (ret) {
		printf("%s: error set\n", __func__);
		return ret;
	}

	sprintf(cmd,"mtdparts=spi_nand.0:");
	if (p_cmdline != NULL) {
			/* To find if mtdparts string existed. If yes, it needs to expand mtdparts environment */
			char *mtdparts_off = NULL;
		        mtdparts_off = strstr ((char *)p_cmdline, "mtdparts=");
			if (mtdparts_off) {
				p_cmdline = mtdparts_off;
				*p_cmdline = '\0';
			}
			strcat(p_cmdline, "mtdparts=spi_nand.0:");
	}
	#elif defined(CONFIG_NVT_SPI_NOR)
	sprintf(cmd, "sf0=spi_nor.0");
	ret = setenv("mtdids",cmd);
	if (ret) {
		printf("%s: error set\n", __func__);
		return ret;
	}

	sprintf(cmd,"mtdparts=spi_nor.0:");
	if (p_cmdline != NULL) {
			/* To find if mtdparts string existed. If yes, it needs to expand mtdparts environment */
			char *mtdparts_off = NULL;
		        mtdparts_off = strstr ((char *)p_cmdline, "mtdparts=");
			if (mtdparts_off) {
				p_cmdline = mtdparts_off;
				*p_cmdline = '\0';
			}
			strcat(p_cmdline, "mtdparts=spi_nor.0:");
	}
	#elif defined(CONFIG_NVT_IVOT_EMMC)
	if (p_cmdline != NULL) {
			/* To find if mtdparts string existed. If yes, it needs to expand mtdparts environment */
			char *nvtemmcpart_off = NULL;
		    nvtemmcpart_off = strstr((char *)p_cmdline, "nvtemmcpart=");
			if (nvtemmcpart_off) {
				p_cmdline = nvtemmcpart_off;
				*p_cmdline = '\0';
			}
			strcat(p_cmdline, "nvtemmcpart=");
	}
	#endif
	j = 0;
	part_num = 0;
	pst_part_num = 0;
	ro_attr = 0;
	/* To parse mtdparts for rootfs partition table */
	for (i = 0 ; i < EMB_PARTITION_INFO_COUNT ; i++) {
		const EMB_PARTITION* p = p_partition + i;
		unsigned int PartitionSize = 0, PartitionOffset = 0;
		#if defined(CONFIG_NVT_SPI_NAND)
		PartitionSize = p->PartitionSize * nand->erasesize;
		PartitionOffset = p->PartitionOffset * nand->erasesize;
		#elif defined(CONFIG_NVT_IVOT_EMMC)
		PartitionSize = p->PartitionSize;
		PartitionOffset = p->PartitionOffset;
		#else
		PartitionSize = p->PartitionSize * spi_flash->erase_size;
		PartitionOffset = p->PartitionOffset * spi_flash->erase_size;
		#endif
		switch(p->EmbType)
		{
		case EMBTYPE_MODELEXT:
			sprintf(buf, "0x%x@0x%x(modelext),", PartitionSize, PartitionOffset);
			strcat(cmd, buf);
			if (p_cmdline != NULL) {
				sprintf(buf, "0x%x@0x%x(modelext),", PartitionSize, PartitionOffset);
				strcat(p_cmdline, buf);
				#if !defined(CONFIG_NVT_IVOT_EMMC)
				j++;
				#endif
			}
			break;
		case EMBTYPE_UITRON:
			sprintf(buf, "0x%x@0x%x(uitron),", PartitionSize, PartitionOffset);
			strcat(cmd, buf);
			if (p_cmdline != NULL) {
				sprintf(buf, "0x%x@0x%x(uitron)ro,", PartitionSize, PartitionOffset);
				strcat(p_cmdline, buf);
				#if !defined(CONFIG_NVT_IVOT_EMMC)
				j++;
				#endif
			}
			break;
		case EMBTYPE_UBOOT:
			sprintf(buf, "0x%x@0x%x(uboot),", PartitionSize, PartitionOffset);
			strcat(cmd, buf);
			if (p_cmdline != NULL) {
				sprintf(buf, "0x%x@0x%x(uboot)ro,", PartitionSize, PartitionOffset);
				strcat(p_cmdline, buf);
				#if !defined(CONFIG_NVT_IVOT_EMMC)
				j++;
				#endif
			}
			break;
		case EMBTYPE_UENV:
			sprintf(buf, "0x%x@0x%x(uenv),", PartitionSize, PartitionOffset);
			strcat(cmd, buf);
			if (p_cmdline != NULL) {
				strcat(p_cmdline, buf);
				#if !defined(CONFIG_NVT_IVOT_EMMC)
				j++;
				#endif
			}
			break;
		case EMBTYPE_LINUX:
			sprintf(buf, "0x%x@0x%x(linux),", PartitionSize, PartitionOffset);
			strcat(cmd, buf);
			if (p_cmdline != NULL) {
				sprintf(buf, "0x%x@0x%x(linux)ro,", PartitionSize, PartitionOffset);
				strcat(p_cmdline, buf);
				#if !defined(CONFIG_NVT_IVOT_EMMC)
				j++;
				#endif
			}
			break;
		case EMBTYPE_RAMFS:
			sprintf(buf, "0x%x@0x%x(ramfs),", PartitionSize, PartitionOffset);
			strcat(cmd, buf);
			if (p_cmdline != NULL) {
				strcat(p_cmdline, buf);
				#if !defined(CONFIG_NVT_IVOT_EMMC)
				j++;
				#endif
			}
			break;
		case EMBTYPE_ROOTFS:
			sprintf(buf, "0x%x@0x%x(rootfs%d),", PartitionSize, PartitionOffset, p->OrderIdx);
			strcat(cmd, buf);
			if (p_cmdline != NULL) {
				strcat(p_cmdline, buf);
				/* Multiple rootfs partitions if OrderIdx > 0 */
				ro_attr = p->OrderIdx;
				if (part_num == 0) {
					#if !defined(CONFIG_NVT_IVOT_EMMC)
					part_num = j;
					#else
					if (p->OrderIdx == 0)
						part_num = ++j;
					#endif
				}
			}
			break;
		case EMBTYPE_FAT:
			sprintf(buf, "0x%x@0x%x(fat),", PartitionSize, PartitionOffset);
			strcat(cmd, buf);
			if (p_cmdline != NULL) {
				strcat(p_cmdline, buf);
				#if !defined(CONFIG_NVT_IVOT_EMMC)
				j++;
				#endif
			}
			break;
		case EMBTYPE_DSP:
			sprintf(buf, "0x%x@0x%x(dsp%d),", PartitionSize, PartitionOffset, p->OrderIdx);
			strcat(cmd, buf);
			if (p_cmdline != NULL) {
				sprintf(buf, "0x%x@0x%x(dsp%d)ro,", PartitionSize, PartitionOffset, p->OrderIdx);
				strcat(p_cmdline, buf);
				#if !defined(CONFIG_NVT_IVOT_EMMC)
				j++;
				#endif
			}
			break;
		case EMBTYPE_PSTORE:
			sprintf(buf, "0x%x@0x%x(pstore),", PartitionSize, PartitionOffset);
			strcat(cmd, buf);
			if (p_cmdline != NULL) {
				sprintf(buf, "0x%x@0x%x(pstore),", PartitionSize, PartitionOffset);
				strcat(p_cmdline, buf);
				#if defined(CONFIG_NVT_IVOT_EMMC)
				pst_part_num = ++j;
				#else
				j++;
				#endif
			}
			break;
		default:
			break;
		}
	}

	if (p_cmdline != NULL) {
		/* To add entire nand mtd device */
		#if defined(CONFIG_NVT_SPI_NAND)
		sprintf(buf, "0x%llx@0(total),", nand->size);
		#elif defined(CONFIG_NVT_SPI_NOR)
		sprintf(buf, "0x%x@0(total),", spi_flash->size);
		#elif defined(CONFIG_NVT_IVOT_EMMC)
		sprintf(buf, "0x%llx@0(total),", mmc->capacity/MMC_MAX_BLOCK_LEN);
		#endif
		strcat(p_cmdline, buf);

		/*
		 * Add bootarg rootfs extension parameter
		 */
		p_cmdline[strlen(p_cmdline) - 1] = ' ';
		#if defined(CONFIG_NVT_UBIFS_SUPPORT)
		if (ro_attr > 0)
			sprintf(buf," ubi.mtd=%d ro ", part_num);
		else
			sprintf(buf," ubi.mtd=%d ", part_num);
		strcat(p_cmdline, buf);
		#elif defined(CONFIG_NVT_EXT4_SUPPORT)
		sprintf(buf," root=/dev/mmcblk2p%d nvt_pst=/dev/mmcblk2p%d ", part_num, pst_part_num);
		strcat(p_cmdline, buf);
		#else
		/*
		 * To handle non-ubifs rootfs type (squashfs/jffs2)
		 */
		if (ro_attr > 0)
			sprintf(buf," root=/dev/mtdblock%d ro ", part_num);
		else
			sprintf(buf," root=/dev/mtdblock%d ", part_num);
		strcat(p_cmdline, buf);
		#endif /* _NVT_ROOTFS_TYPE_ */
	}

	/* To handle uboot mtd env config */

	cmd[strlen(cmd) - 1] = '\0';
	ret = setenv("mtdparts",cmd);


	return ret;
}
#endif /* CONFIG_NVT_LINUX_SPINAND_BOOT || CONFIG_NVT_LINUX_AUTODETECT || CONFIG_NVT_LINUX_SPINOR_BOOT || defined(CONFIG_NVT_LINUX_EMMC_BOOT) */

static void nvt_boot_itron_fw_start(void)
{
	unsigned long val = 0;

	/* Trigger uitron boot ..... */
	if (nvt_readl((ulong)nvt_binfo_comm_core1_start) == 0) {
		nvt_writel((ulong)CONFIG_ITRON_SDRAM_BASE, (ulong)nvt_binfo_comm_core1_start);
		val = (nvt_readl((ulong)nvt_binfo_comm_uboot_boot_func) & ~COMM_UBOOT_BOOT_FUNC_BOOT_DONE_MASK) | COMM_UBOOT_BOOT_FUNC_BOOT_DONE;
		nvt_writel(val, (ulong)nvt_binfo_comm_uboot_boot_func);
	}
}
#if defined(CONFIG_DSP1_FREERTOS) || defined(CONFIG_DSP2_FREERTOS)
static void nvt_boot_dsp_load_done(ulong result)
{
	unsigned long val = 0;

	val = (nvt_readl((ulong)nvt_binfo_comm_uboot_boot_func) & ~COMM_UBOOT_BOOT_FUNC_LOAD_DSP_MASK) | result;
	nvt_writel(val, (ulong)nvt_binfo_comm_uboot_boot_func);
}
#endif

#ifdef	CONFIG_NVT_LINUX_AUTOLOAD

static int nvt_runfw_bin_chk_valid(ulong addr)
{
	unsigned int size = gHeadInfo.Resv1[0];
	NVTPACK_VERIFY_OUTPUT np_verify = {0};
	NVTPACK_GET_PARTITION_INPUT np_get_input;
	NVTPACK_MEM mem_in = {(void*)addr, size};

	memset(&np_get_input, 0, sizeof(np_get_input));

	if(nvtpack_verify(&mem_in, &np_verify) != NVTPACK_ER_SUCCESS) {
		printf("verify failed.\r\n");
		return -1;
	}
	if(np_verify.ver != NVTPACK_VER_16072017) {
		printf("wrong all-in-one bin version\r\n");
		return -1;
	}

	return 0;
}

static int nvt_on_itron_partition_enum_copy_to_dest(unsigned int id, NVTPACK_MEM* p_mem, void* p_user_data)
{
	EMB_PARTITION* pEmb = (EMB_PARTITION*)p_user_data;
	NVTPACK_BFC_HDR *pBFC = (NVTPACK_BFC_HDR *)p_mem->p_data;

	/* Copy to dest address */
	switch (pEmb[id].EmbType) {
#ifndef _NVT_LINUX_SMP_ON_
	case EMBTYPE_UITRON:
		/* Boot compressed uitron or non-compressed uitron */
		if(pBFC->uiFourCC == MAKEFOURCC('B', 'C', 'L', '1')) {
			/* The compressed uitron fw will be decompressed into itron address */
			printf("RUNFW: Boot compressed uitron at 0x%08x with length %d bytes\r\n",
				(unsigned int)p_mem->p_data, p_mem->len);
			nvt_writel((ulong)p_mem->p_data, (ulong)nvt_binfo_comm_itron_comp_addr);
			nvt_writel((ulong)cpu_to_be32(pBFC->uiSizeComp) + sizeof(NVTPACK_BFC_HDR), (ulong)nvt_binfo_comm_itron_comp_len);
		} else {
			printf("RUNFW: Copy uitron from 0x%08x to 0x%08x with length %d bytes\r\n",
				(unsigned int)p_mem->p_data, (unsigned int)CONFIG_ITRON_SDRAM_BASE, p_mem->len);
			memcpy((void *)CONFIG_ITRON_SDRAM_BASE, (void *)p_mem->p_data, p_mem->len);
		}
		break;
#endif /* _NVT_LINUX_SMP_ON_ */
	default:
		break;
	}

	return 0;
}

static int nvt_on_partition_enum_copy_to_dest(unsigned int id, NVTPACK_MEM* p_mem, void* p_user_data)
{
	EMB_PARTITION* pEmb = (EMB_PARTITION*)p_user_data;
	u32 dst_addr = 0, dst_size = 0;
	//NVTPACK_BFC_HDR *pBFC = (NVTPACK_BFC_HDR *)p_mem->p_data;

	/* Copy to dest address */
	switch (pEmb[id].EmbType) {
	case EMBTYPE_LOADER:
		printf("RUNFW: Ignore T.bin loader\n");
		break;
	case EMBTYPE_MODELEXT:
		printf("RUNFW: Ignore T.bin modelext\n");
		break;
	case EMBTYPE_UITRON:
	case EMBTYPE_ECOS:
	case EMBTYPE_UBOOT:
	case EMBTYPE_DSP:
		break;
	case EMBTYPE_LINUX:
		dst_addr = CONFIG_LINUX_SDRAM_START;
		dst_size = p_mem->len;
		printf("RUNFW: Copy Linux from 0x%08x to 0x%08x with length %d bytes\r\n",
			(unsigned int)p_mem->p_data, (unsigned int)dst_addr, dst_size);
		break;
	case EMBTYPE_ROOTFS:
		#ifdef CONFIG_NVT_LINUX_RAMDISK_BOOT
		dst_addr = CONFIG_RAMDISK_SDRAM_BASE;
		dst_size = p_mem->len;
		if ((dst_size) > CONFIG_RAMDISK_SDRAM_SIZE) {
			printf("%sRUNFW: The ramdisk image size %d bytes is larger than %d bytes, can't boot. %s\r\n",
				ANSI_COLOR_RED, dst_size, CONFIG_RAMDISK_SDRAM_SIZE, ANSI_COLOR_RESET);
			printf("%sRUNFW: Please check your modelext dram partition.%s\r\n", ANSI_COLOR_RED, ANSI_COLOR_RESET);
			while(1);
		} else {
			printf("RUNFW: Copy RAMDISK from 0x%08x to 0x%08x with length %d bytes\r\n",
				(unsigned int)p_mem->p_data, (unsigned int)dst_addr, dst_size);
		}
		#endif /* CONFIG_NVT_LINUX_RAMDISK_BOOT */
		break;
	default:
		break;
	}

	if (dst_size != 0) {
		memcpy((void *)dst_addr, (void *)p_mem->p_data, dst_size);
		flush_dcache_range((u32)dst_addr, (u32)(dst_addr + dst_size));
	}
	return 0;
}


static int nvt_runfw_bin_unpack_to_dest(ulong addr, bool itron_only)
{
	u32 i;
	EMB_PARTITION* pEmb = NULL;
	unsigned int size = gHeadInfo.Resv1[0];
	MODELEXT_HEADER* p_resource = NULL;
	NVTPACK_GET_PARTITION_INPUT np_get_input;
	NVTPACK_ENUM_PARTITION_INPUT np_enum_input;
	NVTPACK_MEM mem_in = {(void*)addr, size};
	NVTPACK_MEM mem_out = {0};

	memset(&np_get_input, 0, sizeof(np_get_input));
	memset(&np_enum_input, 0, sizeof(np_enum_input));
	/* Get partition first */
	pEmb = (EMB_PARTITION*)modelext_get_cfg((unsigned char*)CONFIG_MODELEXT_SDRAM_BASE,MODELEXT_TYPE_EMB_PARTITION,&p_resource);
	if(pEmb == NULL) {
		printf("failed to get current partition resource.\r\n");
		return -1;
	}

	/* Check for all partition existed */
	for(i=1; i<EMB_PARTITION_INFO_COUNT; i++) {
		if(pEmb[i].PartitionSize!=0) {
			switch (pEmb[i].EmbType) {
			case EMBTYPE_MODELEXT:
			#ifndef _NVT_LINUX_SMP_ON_
			case EMBTYPE_UITRON:
			#endif
			case EMBTYPE_UBOOT:
			case EMBTYPE_LINUX:
			#ifdef CONFIG_NVT_LINUX_RAMDISK_BOOT
			case EMBTYPE_ROOTFS:
			#endif
				//check this type exist in all-in-one
				np_get_input.id = i;
				np_get_input.mem = mem_in;
				if (nvtpack_get_partition(&np_get_input,&mem_out) != NVTPACK_ER_SUCCESS) {
					printf("RUNFW boot: need partition[%d]\n",i);
					return -1;
				}
				break;
			}
		}
	}

	/* Enum all partition to do necessary handling */
	np_enum_input.mem = mem_in;
	np_enum_input.p_user_data = pEmb;
	if (itron_only)
		/* loading itron only */
		np_enum_input.fp_enum = nvt_on_itron_partition_enum_copy_to_dest;
	else
		/* Loading other images unless itron */
		np_enum_input.fp_enum = nvt_on_partition_enum_copy_to_dest;

	if(nvtpack_enum_partition(&np_enum_input) != NVTPACK_ER_SUCCESS)
	{
		printf("failed sanity.\r\n");
		return -1;
	}

	/* Make sure images had been already copied */
	flush_dcache_all();

	return 0;
}

#if defined(CONFIG_DSP1_FREERTOS) || defined(CONFIG_DSP2_FREERTOS)
static int nvt_boot_dsp_get_nvtpack_partition(int dsp_index, ulong *p_addr, ulong *p_size)
{
	u32 i;
	EMB_PARTITION* pEmb = NULL;
	unsigned int size = gHeadInfo.Resv1[0];
	MODELEXT_HEADER* p_resource = NULL;
	NVTPACK_GET_PARTITION_INPUT np_get_input;
	NVTPACK_ENUM_PARTITION_INPUT np_enum_input;
	NVTPACK_MEM mem_in = {(void*)(void*)CONFIG_NVT_RUNFW_SDRAM_BASE, size};
	NVTPACK_MEM mem_out = {0};

	memset(&np_get_input, 0, sizeof(np_get_input));
	memset(&np_enum_input, 0, sizeof(np_enum_input));

	/* Get partition first */
	pEmb = (EMB_PARTITION*)modelext_get_cfg((unsigned char*)CONFIG_MODELEXT_SDRAM_BASE,MODELEXT_TYPE_EMB_PARTITION,&p_resource);
	if(pEmb == NULL) {
		printf("failed to get current partition resource.\r\n");
		return ER_DSP_NO_EMB_PARTITION;
	}

	/* Check for all partition existed */
	for (i=1; i<EMB_PARTITION_INFO_COUNT; i++) {
		switch (pEmb[i].EmbType) {
		case EMBTYPE_DSP:
			if (pEmb[i].OrderIdx != dsp_index) {
				break;
			} else {
				//check this type exist in all-in-one
				np_get_input.id = i;
				np_get_input.mem = mem_in;
				if (nvtpack_get_partition(&np_get_input,&mem_out) == NVTPACK_ER_SUCCESS) {
					*p_addr = (ulong)mem_out.p_data;
					*p_size = (ulong)mem_out.len;
					return 0;
				} else {
					return ER_DSP_NVTPACK_NO_PARTITION; //cannot found
				}
			}
			break;
		}
	}
	return ER_DSP_NO_DSP_PARTITION;
}

static int nvt_boot_dsp_get_dram_partition(int dsp_index, ulong *p_addr, ulong *p_size)
{
	MODELEXT_HEADER *header;
	DRAM_PARTITION *p_dram_partition = (DRAM_PARTITION *)modelext_get_cfg((unsigned char*)CONFIG_MODELEXT_SDRAM_BASE, MODELEXT_TYPE_DRAM_PARTITION, &header);

	switch (dsp_index) {
	case 0:
		*p_addr = p_dram_partition->dsp1_addr;
		*p_size = p_dram_partition->dsp1_size;
		break;
	case 1:
		*p_addr = p_dram_partition->dsp2_addr;
		*p_size = p_dram_partition->dsp2_size;
		break;
	default:
		printf("cannot handle dsp[%d]\n", dsp_index);
		return ER_DSP_ASSERT;
	}

	if (*p_addr == 0 || *p_size == 0) {
		printf("failed to run dsp, because dsp[%d] addr=%08X, size=%08X\n", dsp_index, (int)*p_addr, (int)*p_size);
		return ER_DSP_ASSERT;
	}
	return 0;
}

static EMB_PARTITION* nvt_boot_dsp_get_emb_partition(int dsp_index)
{
	int i;
	MODELEXT_HEADER *header;
	EMB_PARTITION *pEmb = (EMB_PARTITION*)modelext_get_cfg((unsigned char*)CONFIG_MODELEXT_SDRAM_BASE,MODELEXT_TYPE_EMB_PARTITION,&header);
	for (i=1; i<EMB_PARTITION_INFO_COUNT; i++) {
		if (pEmb[i].EmbType == EMBTYPE_DSP && pEmb[i].OrderIdx == dsp_index) {
			return &pEmb[i];
		}
	}
	printf("cannot find dsp[%d] partition in emb_partition.\n",dsp_index);
	return NULL;
}

static int nvt_boot_dsp_check_uncompressed_dsp_sanity(int dsp_index, DSP_FW_HEADER* p_hdr)
{
	int i;
	NVTPACK_MEM mem = {0};
	DSP_BIN_INFO* pInfo[] = {
	            		&p_hdr->BLoader,
				&p_hdr->ProgInt,
				&p_hdr->DataInt,
				&p_hdr->ProgExt,
				&p_hdr->DataExt};
	for (i=0; i<sizeof(pInfo)/sizeof(DSP_BIN_INFO*);i++) {
		mem.p_data = (void *)(pInfo[i]->TargetAddr);
		mem.len = pInfo[i]->Size;
		unsigned short check_sum = (unsigned short)nvtpack_calc_nvt_sum(&mem);
		unsigned short  complement = (unsigned short)pInfo[i]->CheckSum;
		if ((unsigned short)(check_sum+complement)!=0) {
			printf("dsp[%d], at section %d (addr:%08X) check sum failed.\n",dsp_index, i, pInfo[i]->TargetAddr);
			return ER_DSP_ASSERT;
		}
	}

	return 0;
}

static int nvt_boot_dsp_start(int dsp_index)
{
	MODELEXT_HEADER* header;
	DRAM_PARTITION *p_dram_partition = (DRAM_PARTITION *)modelext_get_cfg((unsigned char*)CONFIG_MODELEXT_SDRAM_BASE, MODELEXT_TYPE_DRAM_PARTITION, &header);
	GXDSP_OPEN Open={0};

	switch (dsp_index) {
	case 0:
		Open.uiAddr = p_dram_partition->dsp1_addr;
		Open.uiSize = p_dram_partition->dsp1_size;
        	break;
	case 1:
		Open.uiAddr = p_dram_partition->dsp2_addr;
		Open.uiSize = p_dram_partition->dsp2_size;
		break;
	default:
		return ER_DSP_ASSERT;
	}

	if (!GxDSP_Open(&Open)) {
		return ER_DSP_ASSERT;
	}

	return 0;
}

#if defined(CONFIG_NVT_LINUX_SPINAND_BOOT) || defined(CONFIG_NVT_LINUX_AUTODETECT) || defined(CONFIG_NVT_LINUX_SPINOR_BOOT) || defined(CONFIG_NVT_LINUX_EMMC_BOOT)
static int nvt_boot_dsp_nand_read(int dsp_index, unsigned int dst_addr, unsigned int ofs, unsigned int size)
{
	char cmd[50] = {0};
#if defined(CONFIG_NVT_LINUX_SPINOR_BOOT) || defined(CONFIG_NVT_LINUX_EMMC_BOOT)
	loff_t part_off=0, part_size=0;
#endif

	if (ofs == 0) {
		#if defined(CONFIG_NVT_LINUX_SPINAND_BOOT)
		sprintf(cmd, "nand read 0x%x dsp%d 0x%x", (unsigned int)dst_addr, dsp_index, size);
		#elif defined(CONFIG_NVT_LINUX_EMMC_BOOT)
		sprintf(cmd, "dsp%d", dsp_index);
		get_part(cmd, &part_off, &part_size);
		u32 align_size = ALIGN_CEIL(size, MMC_MAX_BLOCK_LEN)/MMC_MAX_BLOCK_LEN;
		sprintf(cmd, "mmc read 0x%x 0x%llx 0x%x", (unsigned int)dst_addr, part_off, align_size);
		#else
		sprintf(cmd, "dsp%d", dsp_index);
		get_part(cmd, &part_off, &part_size);
		sprintf(cmd, "sf read 0x%x 0x%llx 0x%x", (unsigned int)dst_addr, part_off, size);
		#endif /* !CONFIG_NVT_LINUX_SPINAND_BOOT */
	} else {
		EMB_PARTITION *pEmb = nvt_boot_dsp_get_emb_partition(dsp_index);
		#if !defined(CONFIG_NVT_LINUX_EMMC_BOOT)
		unsigned int base_ofs = pEmb->PartitionOffset*_EMBMEM_BLK_SIZE_;
		#else
		unsigned int base_ofs = pEmb->PartitionOffset;
		#endif
		#if defined(CONFIG_NVT_LINUX_SPINAND_BOOT)
		sprintf(cmd, "nand read 0x%x 0x%x 0x%x", (unsigned int)dst_addr, base_ofs+ofs, size);
		#elif defined(CONFIG_NVT_LINUX_EMMC_BOOT)
		u32 align_size = ALIGN_CEIL(size, MMC_MAX_BLOCK_LEN)/MMC_MAX_BLOCK_LEN;
		if(ofs%MMC_MAX_BLOCK_LEN) {
			printf("^R offset(0x%X) error\n",ofs);
		}
		ofs /= MMC_MAX_BLOCK_LEN;
		sprintf(cmd, "mmc read 0x%x 0x%x 0x%x", (unsigned int)dst_addr, base_ofs+ofs, align_size);
		#else
		u32 align_size = ALIGN_CEIL(size, ARCH_DMA_MINALIGN);
		sprintf(cmd, "sf read 0x%x 0x%x 0x%x", (unsigned int)dst_addr, base_ofs+ofs, align_size);
		#endif
	}
	printf("cmd=%s\n",cmd);
	run_command(cmd, 0);
	return 0;
}
#endif /* CONFIG_NVT_LINUX_SPINAND_BOOT || CONFIG_NVT_LINUX_AUTODETECT || CONFIG_NVT_LINUX_SPINOR_BOOT */

#if defined(CONFIG_NVT_LINUX_SD_BOOT)
static int nvt_boot_dsp_sd_read(int dsp_index, unsigned int dst_addr, unsigned int ofs, unsigned int size)
{
	char cmd[50] = {0};
	sprintf(cmd, "fatload mmc 0:1 0x%x dsp%d.bin 0x%x 0x%x", (unsigned int)dst_addr, dsp_index, size, ofs);
	run_command(cmd, 0);
	return 0;
}
#endif

static int nvt_boot_dsp_load_from_storage(int dsp_index, DSP_STORAGE_READ_FP fp_read)
{
	int er;
	NVTPACK_BFC_HDR *pBFC = NULL;
	DSP_FW_HEADER *pDSP = NULL;
	ulong dst_addr = 0, dst_size = 0;

	// get dram dsp memory range
	if ((er = nvt_boot_dsp_get_dram_partition(dsp_index, &dst_addr, &dst_size)) !=0 ) {
		return er;
	}

	// read first block to check if is compressed
	fp_read(dsp_index, (unsigned int)dst_addr, 0, 0x200);
	// read remain blocks
	pBFC = (NVTPACK_BFC_HDR *)dst_addr;
	pDSP = (DSP_FW_HEADER *)dst_addr;
	if (pBFC->uiFourCC == MAKEFOURCC('B', 'C', 'L', '1')) {
		printf("dsp[%d] is comp\n", dsp_index);
		// load to dsp memory and decompress to linux temp memory, and then copy back each section to dsp memory
		fp_read(dsp_index, (unsigned int)dst_addr, 0, cpu_to_be32(pBFC->uiSizeComp));
		// check if temp buffer size is enough
		if (CONFIG_LINUX_SDRAM_SIZE < cpu_to_be32(pBFC->uiSizeUnComp)) {
			printf("compressed size dsp[%d]:%08X too large than temp size (%08X)\n", dsp_index, (int)cpu_to_be32(pBFC->uiSizeUnComp), (int)CONFIG_LINUX_SDRAM_SIZE);
			return ER_DSP_NO_MEM;
		}
		// decompress to linux temp memory
		lz_uncompress((unsigned char *)(dst_addr + sizeof(NVTPACK_BFC_HDR)), (unsigned char *)CONFIG_LINUX_SDRAM_START, cpu_to_be32(pBFC->uiSizeComp));
		// copy back each section to dsp memory
		pDSP = (DSP_FW_HEADER*)CONFIG_LINUX_SDRAM_START;
		// check fourcc
		if((dsp_index==0 && pDSP->uiFourCC != MAKEFOURCC('D','S','P','1')) ||
			(dsp_index==1 && pDSP->uiFourCC != MAKEFOURCC('D','S','P','2'))) {
			printf("dsp[%d] invalid fourcc 0x%08X\n", dsp_index, (unsigned int)pDSP->uiFourCC);
			return ER_DSP_INVALID;
		}
		//copy header to dst_addr of dsp memory end for gxdsp
		memcpy((void*)(dst_addr + dst_size - pDSP->uiSize), pDSP, pDSP->uiSize);
		//load bin files
		memcpy((void*)(pDSP->BLoader.TargetAddr), (void*)(CONFIG_LINUX_SDRAM_START+pDSP->BLoader.Offset), pDSP->BLoader.Size);
		memcpy((void*)(pDSP->ProgInt.TargetAddr), (void*)(CONFIG_LINUX_SDRAM_START+pDSP->ProgInt.Offset), pDSP->ProgInt.Size);
		memcpy((void*)(pDSP->DataInt.TargetAddr), (void*)(CONFIG_LINUX_SDRAM_START+pDSP->DataInt.Offset), pDSP->DataInt.Size);
		memcpy((void*)(pDSP->ProgExt.TargetAddr), (void*)(CONFIG_LINUX_SDRAM_START+pDSP->ProgExt.Offset), pDSP->ProgExt.Size);
		memcpy((void*)(pDSP->DataExt.TargetAddr), (void*)(CONFIG_LINUX_SDRAM_START+pDSP->DataExt.Offset), pDSP->DataExt.Size);
		// check sanity
		if ((er = nvt_boot_dsp_check_uncompressed_dsp_sanity(dsp_index, pDSP)) != 0) {
			return ER_DSP_ASSERT;
		}
	} else if ((dsp_index==0 && pBFC->uiFourCC == MAKEFOURCC('D','S','P','1')) ||
		(dsp_index==1 && pBFC->uiFourCC == MAKEFOURCC('D','S','P','2'))) {
		printf("dsp[%d] is uncomp\n", dsp_index);
		// load mixed-3-part to temp memory
		u32 size_3part = sizeof(DSP_FW_HEADER) + pDSP->BLoader.Size + pDSP->ProgInt.Size + pDSP->DataInt.Size;
		fp_read(dsp_index, (unsigned int)CONFIG_LINUX_SDRAM_START, 0, size_3part);
		//pDSP must move on to temp buffer, because original header address will be overwrite later by ProgExt or DataExt
		pDSP = (DSP_FW_HEADER *)CONFIG_LINUX_SDRAM_START;
		//copy header to dst_addr of dsp memory end for gxdsp
		memcpy((void*)(dst_addr + dst_size - pDSP->uiSize), pDSP, pDSP->uiSize);
		// move mixed-3-part to their memory space
		memcpy((void*)(pDSP->BLoader.TargetAddr), (void*)(CONFIG_LINUX_SDRAM_START+pDSP->BLoader.Offset), pDSP->BLoader.Size);
		memcpy((void*)(pDSP->ProgInt.TargetAddr), (void*)(CONFIG_LINUX_SDRAM_START+pDSP->ProgInt.Offset), pDSP->ProgInt.Size);
		memcpy((void*)(pDSP->DataInt.TargetAddr), (void*)(CONFIG_LINUX_SDRAM_START+pDSP->DataInt.Offset), pDSP->DataInt.Size);
		// load remain 2 parts (ProgExt and DataExt)
		fp_read(dsp_index, (unsigned int)pDSP->ProgExt.TargetAddr, (unsigned int)pDSP->ProgExt.Offset, (unsigned int)pDSP->ProgExt.Size);
		fp_read(dsp_index, (unsigned int)pDSP->DataExt.TargetAddr, (unsigned int)pDSP->DataExt.Offset, (unsigned int)pDSP->DataExt.Size);
		// check sanity
		if ((er = nvt_boot_dsp_check_uncompressed_dsp_sanity(dsp_index, pDSP)) != 0) {
			return ER_DSP_ASSERT;
		}
	} else {
		printf("dsp[%d] invalid fourcc 0x%08X\n", dsp_index, (unsigned int)pDSP->uiFourCC);
		return ER_DSP_INVALID;
	}

	//flush, write to back for dsp engine
	flush_dcache_range((u32)dst_addr, (u32)(dst_addr + dst_size));

	return 0;
}

static int nvt_boot_dsp_load_from_nvtpack(int dsp_index)
{
	int er;
	NVTPACK_MEM mem = {0};
	NVTPACK_BFC_HDR *pBFC = NULL;
	DSP_FW_HEADER *pDSP = NULL;
	ulong src_addr = 0, src_size = 0;
	ulong dst_addr = 0, dst_size = 0;

	// get dram dsp memory range
	if ((er = nvt_boot_dsp_get_dram_partition(dsp_index, &dst_addr, &dst_size)) !=0 ) {
		return er;
	}

	// get starting address in nvtpack
	if ((er = nvt_boot_dsp_get_nvtpack_partition(dsp_index, &src_addr, &src_size) )!= 0) {
		return er;
	}

	// decompress or memcpy from src to dst
	pBFC = (NVTPACK_BFC_HDR *)src_addr;
	if(pBFC->uiFourCC == MAKEFOURCC('B','C','L','1')) {
		printf("dsp[%d] is comp\n", dsp_index);
		// check sum
		mem.p_data = (void *)src_addr;
		mem.len = (unsigned int)src_size;
		if(nvtpack_calc_nvt_sum(&mem)!=0) {
			printf("nvtpack dsp[%d] check sum failed.\n",dsp_index);
			return ER_DSP_INVALID;
		}
		// decompress to linux memory as temp memory
		// check if temp buffer size is enough
		if (CONFIG_LINUX_SDRAM_SIZE < cpu_to_be32(pBFC->uiSizeUnComp)) {
			printf("dsp[%d]:%08X too large to decode on temp linux memory (%08X)\n", dsp_index, (int)cpu_to_be32(pBFC->uiSizeUnComp), (int)CONFIG_LINUX_SDRAM_SIZE);
			return ER_DSP_NO_MEM;
		}
		lz_uncompress((unsigned char *)(src_addr + sizeof(NVTPACK_BFC_HDR)), (unsigned char *)CONFIG_LINUX_SDRAM_START, cpu_to_be32(pBFC->uiSizeComp));
		src_addr = CONFIG_LINUX_SDRAM_START; //uncompressed source address
		// check fourcc
		pDSP = (DSP_FW_HEADER*)src_addr;
		if((dsp_index==0 && pDSP->uiFourCC != MAKEFOURCC('D','S','P','1'))
			|| (dsp_index==1 && pDSP->uiFourCC != MAKEFOURCC('D','S','P','2'))) {
			printf("dsp[%d] invalid fourcc 0x%08X\n", dsp_index, (unsigned int)pDSP->uiFourCC);
			return ER_DSP_ASSERT;
		}
	} else if((dsp_index==0 && pBFC->uiFourCC == MAKEFOURCC('D','S','P','1'))
		|| (dsp_index==1 && pBFC->uiFourCC == MAKEFOURCC('D','S','P','2'))) {
		printf("dsp[%d] is uncomp\n", dsp_index);
		// do nothing
	} else {
		printf("dsp[%d]: invalid fourcc 0x%08X\n", dsp_index, pBFC->uiFourCC);
		return ER_DSP_INVALID;
	}

	pDSP = (DSP_FW_HEADER*)src_addr;
	//copy header to dst_addr of dsp memory end for gxdsp
	memcpy((void*)(dst_addr + dst_size - pDSP->uiSize), pDSP, pDSP->uiSize);
	//load bin files
	memcpy((void*)(pDSP->BLoader.TargetAddr), (void*)(src_addr+pDSP->BLoader.Offset), pDSP->BLoader.Size);
	memcpy((void*)(pDSP->ProgInt.TargetAddr), (void*)(src_addr+pDSP->ProgInt.Offset), pDSP->ProgInt.Size);
	memcpy((void*)(pDSP->DataInt.TargetAddr), (void*)(src_addr+pDSP->DataInt.Offset), pDSP->DataInt.Size);
	memcpy((void*)(pDSP->ProgExt.TargetAddr), (void*)(src_addr+pDSP->ProgExt.Offset), pDSP->ProgExt.Size);
	memcpy((void*)(pDSP->DataExt.TargetAddr), (void*)(src_addr+pDSP->DataExt.Offset), pDSP->DataExt.Size);

	// check sanity
	if ((er = nvt_boot_dsp_check_uncompressed_dsp_sanity(dsp_index, pDSP)) != 0) {
		return ER_DSP_ASSERT;
	}

	//flush, write to back for dsp engine
	flush_dcache_range((u32)dst_addr, (u32)(dst_addr + dst_size));
	return 0;
}
#endif

#ifdef CONFIG_NVT_LINUX_AUTODETECT
static int nvt_boot_itron_bin_auto(void)
{
	char cmd[50] = {0};
	int ret = 0;
#if defined(CONFIG_NVT_LINUX_SPINOR_BOOT) || defined(CONFIG_NVT_LINUX_EMMC_BOOT)
	u32 align_size = 0;
	loff_t part_off=0, part_size=0;
	get_part("uitron", &part_off, &part_size);
#endif

	/* Get bininfo first */
	MODELEXT_HEADER* p_resource = NULL;
	BININFO	*p_modelext_bininfo = (BININFO *)modelext_get_cfg((unsigned char*)CONFIG_MODELEXT_SDRAM_BASE, MODELEXT_TYPE_BIN_INFO,&p_resource);
	if(p_modelext_bininfo == NULL) {
		printf("failed to get current partition resource.\r\n");
		return -1;
	}

	/* Check if it's SMP mode */
	if (gHeadInfo.BinCtrl & 0x2) {
		printf(">>> SMP boot\n");
	} else {
		printf(">>> Dual OS boot\n");
		/* Check if uitron is compressed */
		/* Check for fw is A.bin? */
		if (nvt_detect_fw_tbin()) {
			/* Check fw is valid */
			nvt_runfw_bin_chk_valid((ulong)CONFIG_NVT_RUNFW_SDRAM_BASE);
			/* Copy itron binary to destination address */
			ret = nvt_runfw_bin_unpack_to_dest((ulong)CONFIG_NVT_RUNFW_SDRAM_BASE, true);
			if (ret < 0)
				return ret;
		} else {
			#if defined(CONFIG_NVT_LINUX_SPINAND_BOOT)
			sprintf(cmd, "nand read 0x%x uitron 0x%x ", CONFIG_NVT_RUNFW_SDRAM_BASE, 0x200);
			#elif defined(CONFIG_NVT_LINUX_SPINOR_BOOT)
			sprintf(cmd, "sf read 0x%x 0x%llx 0x%llx", CONFIG_NVT_RUNFW_SDRAM_BASE, part_off, part_size);
			#elif defined(CONFIG_NVT_LINUX_EMMC_BOOT)
			sprintf(cmd, "mmc read 0x%x 0x%llx 1", CONFIG_NVT_RUNFW_SDRAM_BASE, part_off);
			#endif /* CONFIG_NVT_LINUX_SPINAND_BOOT */
			run_command(cmd, 0);
			NVTPACK_BFC_HDR *pbfc = (NVTPACK_BFC_HDR *)CONFIG_NVT_RUNFW_SDRAM_BASE;

			/* Boot compressed uitron or non-compressed uitron */
			if(pbfc->uiFourCC == MAKEFOURCC('B', 'C', 'L', '1')) {
				printf("Compressed uItron\n");
				/*
				 * Trigger compressed uitron boot.
				 * Comp. uitron => CONFIG_NVT_RUNFW_SDRAM_BASE,
				 * UnComp. uitron => CONFIG_ITRON_SDRAM_BASE.
				 */
				size_t firmware_size = cpu_to_be32(pbfc->uiSizeComp) + sizeof(NVTPACK_BFC_HDR);
				if ((CONFIG_ITRON_SDRAM_BASE + cpu_to_be32(pbfc->uiSizeUnComp)) > CONFIG_NVT_RUNFW_SDRAM_BASE) {
					printf("%s Attention!!! iTron starting address at 0x%08x with size 0x%08x (uncompressed) is larger than temp buffer address!! 0x%08x %s\r\n",
						ANSI_COLOR_RED, CONFIG_ITRON_SDRAM_BASE, cpu_to_be32(pbfc->uiSizeUnComp), CONFIG_NVT_RUNFW_SDRAM_BASE, ANSI_COLOR_RESET);
					printf("%s It can't decompress %s\r\n", ANSI_COLOR_RED, ANSI_COLOR_RESET);
					while(1);
				}
				#if defined(CONFIG_NVT_LINUX_SPINAND_BOOT)
				sprintf(cmd, "nand read 0x%x uitron 0x%x ", CONFIG_NVT_RUNFW_SDRAM_BASE, firmware_size);
				#elif defined(CONFIG_NVT_LINUX_SPINOR_BOOT)
				align_size = ALIGN_CEIL(firmware_size, ARCH_DMA_MINALIGN);
				sprintf(cmd, "sf read 0x%x 0x%llx 0x%x", CONFIG_NVT_RUNFW_SDRAM_BASE, part_off, align_size);
				#elif defined(CONFIG_NVT_LINUX_EMMC_BOOT)
				align_size = ALIGN_CEIL(firmware_size, MMC_MAX_BLOCK_LEN)/MMC_MAX_BLOCK_LEN;
				sprintf(cmd, "mmc read 0x%x 0x%llx 0x%x", CONFIG_NVT_RUNFW_SDRAM_BASE, part_off, align_size);
				#endif /* CONFIG_NVT_LINUX_SPINAND_BOOT */
				run_command(cmd, 0);

				debug("auto boot bfc UnCompsz:%d Compsz:%d Algorithm: %x \n", cpu_to_be32(pbfc->uiSizeUnComp), cpu_to_be32(pbfc->uiSizeComp), cpu_to_be32(pbfc->uiAlgorithm));
				if((cpu_to_be32(pbfc->uiAlgorithm) & 0xFF ) == 11)
				{
					size_t Compsz = cpu_to_be32(pbfc->uiSizeComp);
					size_t Uncompsz = cpu_to_be32(pbfc->uiSizeUnComp);
					printf("Decompressed lzma uItron\n");
					flush_dcache_range((u32)CONFIG_NVT_RUNFW_SDRAM_BASE,
							(u32)(CONFIG_NVT_RUNFW_SDRAM_BASE + firmware_size));
					ret = lzmaBuffToBuffDecompress((unsigned char *)CONFIG_ITRON_SDRAM_BASE, &Uncompsz, (unsigned char *)(CONFIG_NVT_RUNFW_SDRAM_BASE + sizeof(NVTPACK_BFC_HDR)), Compsz);
					if(ret != 0)
						printf("Decompressed lzma fail\n");
					flush_dcache_range((u32)CONFIG_ITRON_SDRAM_BASE, (u32)(CONFIG_ITRON_SDRAM_BASE+Uncompsz));
				}
				else
				{
					nvt_writel((ulong)CONFIG_NVT_RUNFW_SDRAM_BASE, (ulong)nvt_binfo_comm_itron_comp_addr);
					nvt_writel((ulong)cpu_to_be32(pbfc->uiSizeComp), (ulong)nvt_binfo_comm_itron_comp_len);
					flush_dcache_range((u32)CONFIG_NVT_RUNFW_SDRAM_BASE,
							(u32)(CONFIG_NVT_RUNFW_SDRAM_BASE + firmware_size));
				}
			} else {
				NVTPACK_BININFO_HDR *pbininfo =  (NVTPACK_BININFO_HDR *)(CONFIG_NVT_RUNFW_SDRAM_BASE + BIN_INFO_OFFSET_UITRON);
				printf("Non-compressed uItron\n");
				if (p_modelext_bininfo->ld.LdCtrl & LDCF_PARTLOAD_EN) {
					// preload some to get part-1 size for partial load and partial compressed load
					u32 preload_size = ALIGN_CEIL(FW_PART1_SIZE_OFFSET, _EMBMEM_BLK_SIZE_);
					#if defined(CONFIG_NVT_LINUX_SPINAND_BOOT)
					sprintf(cmd, "nand read 0x%x uitron 0x%x ", CONFIG_ITRON_SDRAM_BASE, preload_size);
					#elif defined(CONFIG_NVT_LINUX_SPINOR_BOOT)
					align_size = ALIGN_CEIL(preload_size, ARCH_DMA_MINALIGN);
					sprintf(cmd, "sf read 0x%x 0x%llx 0x%x", CONFIG_ITRON_SDRAM_BASE, part_off, align_size);
					#elif defined(CONFIG_NVT_LINUX_EMMC_BOOT)
					align_size = ALIGN_CEIL(preload_size, MMC_MAX_BLOCK_LEN)/MMC_MAX_BLOCK_LEN;
					sprintf(cmd, "mmc read 0x%x 0x%llx 0x%x", CONFIG_NVT_RUNFW_SDRAM_BASE, part_off, align_size);
					#endif
					run_command(cmd, 0);
					//partial-load or partial-compressed-load
					u32 part1_size = *(u32 *)(CONFIG_ITRON_SDRAM_BASE + FW_PART1_SIZE_OFFSET);
					printf("part1_size=%08X,preload_size=%08X\r\n",part1_size,preload_size);
					//align to block size
					part1_size = ALIGN_CEIL(part1_size, _EMBMEM_BLK_SIZE_);
					printf("part1_size_aligned=%08X\r\n",part1_size);
					//sprintf(cmd, "nand read 0x%x 0x%x 0x%x", (unsigned int)dst_addr, base_ofs+ofs, size);
					#if defined(CONFIG_NVT_LINUX_SPINAND_BOOT)
					sprintf(cmd, "nand read 0x%x uitron 0x%x ", CONFIG_ITRON_SDRAM_BASE, part1_size);
					#elif defined(CONFIG_NVT_LINUX_SPINOR_BOOT)
					align_size = ALIGN_CEIL(part1_size, ARCH_DMA_MINALIGN);
					sprintf(cmd, "sf read 0x%x 0x%llx 0x%x", CONFIG_ITRON_SDRAM_BASE, part_off, align_size);
					#elif defined(CONFIG_NVT_LINUX_EMMC_BOOT)
					align_size = ALIGN_CEIL(part1_size, MMC_MAX_BLOCK_LEN)/MMC_MAX_BLOCK_LEN;
					sprintf(cmd, "mmc read 0x%x 0x%llx 0x%x", CONFIG_ITRON_SDRAM_BASE, part_off, align_size);
					#endif
					//update LdLoadSize
					p_modelext_bininfo->ld.LdLoadSize = part1_size;
				} else {
					//full load
					#if defined(CONFIG_NVT_LINUX_SPINAND_BOOT)
					sprintf(cmd, "nand read 0x%x uitron 0x%x ", CONFIG_ITRON_SDRAM_BASE, pbininfo->BinLength);
					#elif defined(CONFIG_NVT_LINUX_SPINOR_BOOT)
					align_size = ALIGN_CEIL(pbininfo->BinLength, ARCH_DMA_MINALIGN);
					sprintf(cmd, "sf read 0x%x 0x%llx 0x%x", CONFIG_ITRON_SDRAM_BASE, part_off, align_size);
					#elif defined(CONFIG_NVT_LINUX_EMMC_BOOT)
					align_size = ALIGN_CEIL(pbininfo->BinLength, MMC_MAX_BLOCK_LEN)/MMC_MAX_BLOCK_LEN;
					sprintf(cmd, "mmc read 0x%x 0x%llx 0x%x", CONFIG_ITRON_SDRAM_BASE, part_off, align_size);
					#endif
					//update LdLoadSize
					p_modelext_bininfo->ld.LdLoadSize = pbininfo->BinLength;
				}
				run_command(cmd, 0);
			}
		}
		nvt_boot_itron_fw_start();
	}

	return 0;
}

static int nvt_boot_linux_bin_auto(void)
{
	char cmd[256] = {0};
	int ret = 0;
	image_header_t *hdr;
	uint size;
#if defined(CONFIG_NVT_LINUX_SPINOR_BOOT) || defined(CONFIG_NVT_LINUX_EMMC_BOOT)
	loff_t part_off=0, part_size=0;
	u32 align_size = 0;
	get_part("linux", &part_off, &part_size);
#endif

	if (nvt_detect_fw_tbin()) {
		/* Check fw is valid */
		nvt_runfw_bin_chk_valid((ulong)CONFIG_NVT_RUNFW_SDRAM_BASE);
		/* Copy linux binary to destination address */
		ret = nvt_runfw_bin_unpack_to_dest((ulong)CONFIG_NVT_RUNFW_SDRAM_BASE, false);
		if (ret < 0) {
			return ret;
		}
	} else {
		// get uImage size firstly
		#if defined(CONFIG_NVT_LINUX_SPINAND_BOOT)
		sprintf(cmd, "nand read 0x%x linux 0x%x", CONFIG_LINUX_SDRAM_START, sizeof(image_header_t));
		#elif defined(CONFIG_NVT_LINUX_SPINOR_BOOT)
		align_size = ALIGN_CEIL(sizeof(image_header_t), ARCH_DMA_MINALIGN);
		sprintf(cmd, "sf read 0x%x 0x%llx 0x%x", CONFIG_LINUX_SDRAM_START, part_off, align_size);
		#elif defined(CONFIG_NVT_LINUX_EMMC_BOOT)
		sprintf(cmd, "mmc read 0x%x 0x%llx 0x%x", CONFIG_LINUX_SDRAM_START, part_off, 2);
		#endif
		run_command(cmd, 0);
		hdr = (image_header_t *)CONFIG_LINUX_SDRAM_START;
		size = image_get_data_size(hdr) + sizeof(image_header_t);
		// Reading Linux kernel image
		#if defined(CONFIG_NVT_LINUX_SPINAND_BOOT)
		sprintf(cmd, "nand read 0x%x linux 0x%x", CONFIG_LINUX_SDRAM_START, size);
		#elif defined(CONFIG_NVT_LINUX_SPINOR_BOOT)
		align_size = ALIGN_CEIL(size, ARCH_DMA_MINALIGN);
		sprintf(cmd, "sf read 0x%x 0x%llx 0x%x", CONFIG_LINUX_SDRAM_START, part_off, align_size);
		#elif defined(CONFIG_NVT_LINUX_EMMC_BOOT)
		align_size = ALIGN_CEIL(size, MMC_MAX_BLOCK_LEN)/MMC_MAX_BLOCK_LEN;
		sprintf(cmd, "mmc read 0x%x 0x%llx 0x%x", CONFIG_LINUX_SDRAM_START, part_off, align_size);
		#endif
		run_command(cmd, 0);
	}

	return 0;
}

static int nvt_boot_dsp_bin_auto(void)
{
#if defined(CONFIG_DSP1_FREERTOS) || defined(CONFIG_DSP2_FREERTOS)
	int er;
	MODELEXT_HEADER* header;
	BININFO *p_bininfo = (BININFO *)modelext_get_cfg((unsigned char*)CONFIG_MODELEXT_SDRAM_BASE, MODELEXT_TYPE_BIN_INFO, &header);
	ulong result = COMM_UBOOT_BOOT_FUNC_LOAD_DSP_DONE;
#endif

#if defined(CONFIG_DSP1_FREERTOS)
	printf("NVT DSP boot.....\n");
	if (p_bininfo->ld.LdCtrl2 & LDCF_BOOT_CARD) {
		er = nvt_boot_dsp_load_from_nvtpack(0);
		if (er == 0) {
			printf("DSP1_FREERTOS from tbin\n");
		} else if (er == ER_DSP_NVTPACK_NO_PARTITION) {
			printf("DSP1_FREERTOS from flash\n");
			er = nvt_boot_dsp_load_from_storage(0, nvt_boot_dsp_nand_read);
		}
	} else {
		printf("DSP1_FREERTOS from flash\n");
		er = nvt_boot_dsp_load_from_storage(0, nvt_boot_dsp_nand_read);
	}
	if (er == 0) {
		nvt_boot_dsp_start(0);
	} else {
		result = COMM_UBOOT_BOOT_FUNC_LOAD_DSP_NG;
		printf("DSP1 load failed.\r\n");
	}
#else
	printf("DSP1_NONE\n");
#endif

#if defined(CONFIG_DSP2_FREERTOS)
	if (p_bininfo->ld.LdCtrl2 & LDCF_BOOT_CARD) {
		er = nvt_boot_dsp_load_from_nvtpack(1);
		if (er == 0) {
			printf("DSP2_FREERTOS from tbin\n");
		} else if (er == ER_DSP_NVTPACK_NO_PARTITION) {
			printf("DSP2_FREERTOS from flash\n");
			er = nvt_boot_dsp_load_from_storage(1, nvt_boot_dsp_nand_read);
		}
	} else {
		printf("DSP2_FREERTOS from flash\n");
		er = nvt_boot_dsp_load_from_storage(1, nvt_boot_dsp_nand_read);
	}
	if (er == 0) {
		nvt_boot_dsp_start(1);
	} else {
		result = COMM_UBOOT_BOOT_FUNC_LOAD_DSP_NG;
		printf("DSP2 load failed.\r\n");
	}
#else
	printf("DSP2_NONE\n");
#endif
#if defined(CONFIG_DSP1_FREERTOS) || defined(CONFIG_DSP2_FREERTOS)
	nvt_boot_dsp_load_done(result);
#endif
	return 0;
}

#if defined(CONFIG_NVT_LINUX_EMMC_BOOT)
static int nvt_load_pstore_1st_section(void)
{
	int		free_bytes, read_bytes;
	NVTPACK_MEM	mem_sum;

	PFASTBOOTPSTORE header = (PFASTBOOTPSTORE)(CONFIG_NVTIPC_SHARED_MEM_ADDR - 8192);
	header->uiDataAddr = ((ulong)header + sizeof(FASTBOOTPSTORE));
	if(header->uiDataAddr % 512)
		header->uiDataAddr += (512 - (header->uiDataAddr % 512));
	free_bytes = (8192 - (header->uiDataAddr - (UINT32)header));

	if (fs_set_blk_dev("mmc", "1:1", FS_TYPE_EXT)){
		printf("MMC interface configure failed\n");
		return -1;
	}
	read_bytes = fs_read("/SYSP", (ulong)header->uiDataAddr, 0, 0);
	if(read_bytes <= 0){
		printf("fail to read pstore section\r\n");
		return -1;
	}
	if(read_bytes > free_bytes){
		printf("pstore section size(%d) > temp buffer size(%d)\r\n", read_bytes, free_bytes);
		return -1;
	}
	header->uiDataSize = (UINT32)read_bytes;

	memset(&mem_sum, 0, sizeof(mem_sum));
	mem_sum.p_data = (void*)header->uiDataAddr;
	mem_sum.len = header->uiDataSize;
	header->uiCheckSum1 = (header->uiDataSize | header->uiDataAddr);
	header->uiCheckSum2 = nvtpack_calc_nvt_sum(&mem_sum);

	flush_dcache_range((u32)header, header->uiDataAddr + header->uiDataSize);

	return 0;
}
#endif

static int nvt_boot_fw_init_detect(void)
{
	printf("NVT firmware boot.....\n");

#ifndef _NVT_LINUX_SMP_ON_

#if defined(CONFIG_NVT_LINUX_EMMC_BOOT)
	nvt_load_pstore_1st_section();
#endif

	nvt_boot_itron_bin_auto();
#endif /* _NVT_LINUX_SMP_ON_ */

#ifdef _CPU2_NONE_
	/* core1 will turn off core2 if core2 is not linux */
	while(1);
#endif /* _CPU2_NONE_ */

#ifdef CONFIG_NVT_LINUX_EMMC_BOOT
        nvt_boot_dsp_bin_auto();
#endif

	nvt_boot_linux_bin_auto();

	return 0;
}
#else /* !CONFIG_NVT_LINUX_AUTODETECT */
static int nvt_boot_itron_bin(void)
{
	int ret = 0;
	/* Check if it's SMP mode */
	if (gHeadInfo.BinCtrl & 0x2) {
		printf(">>> SMP boot\n");
	} else {
		printf(">>> Dual OS boot\n");
		/* Check if uitron is compressed */
		#if defined(CONFIG_NVT_LINUX_SPINAND_BOOT)
		char cmd[50] = {0};
		sprintf(cmd, "nand read 0x%x uitron 0x%x ", CONFIG_NVT_RUNFW_SDRAM_BASE, 0x200);
		run_command(cmd, 0);
		NVTPACK_BFC_HDR *pbfc = (NVTPACK_BFC_HDR *)CONFIG_NVT_RUNFW_SDRAM_BASE;

		/* Boot compressed uitron or non-compressed uitron */
		if(pbfc->uiFourCC == MAKEFOURCC('B', 'C', 'L', '1')) {
			printf("Compressed uItron\n");
			/*
			 * Trigger compressed uitron boot.
			 * Comp. uitron => CONFIG_NVT_RUNFW_SDRAM_BASE,
			 * UnComp. uitron => CONFIG_ITRON_SDRAM_BASE.
			 */
			size_t firmware_size = cpu_to_be32(pbfc->uiSizeComp) + sizeof(NVTPACK_BFC_HDR);
			sprintf(cmd, "nand read 0x%x uitron 0x%x ", CONFIG_NVT_RUNFW_SDRAM_BASE, firmware_size);
			run_command(cmd, 0);

			debug("nand boot bfc UnCompsz:%d Compsz:%d Algorithm: %x \n", cpu_to_be32(pbfc->uiSizeUnComp), cpu_to_be32(pbfc->uiSizeComp), cpu_to_be32(pbfc->uiAlgorithm));
			if((cpu_to_be32(pbfc->uiAlgorithm) & 0xFF ) == 11)
			{
				size_t Compsz = cpu_to_be32(pbfc->uiSizeComp);
				size_t Uncompsz = cpu_to_be32(pbfc->uiSizeUnComp);
				printf("Decompressed lzma uItron\n");
				flush_dcache_range((u32)CONFIG_NVT_RUNFW_SDRAM_BASE,
						(u32)(CONFIG_NVT_RUNFW_SDRAM_BASE + firmware_size));
				ret = lzmaBuffToBuffDecompress((unsigned char *)CONFIG_ITRON_SDRAM_BASE, &Uncompsz, (unsigned char *)(CONFIG_NVT_RUNFW_SDRAM_BASE + sizeof(NVTPACK_BFC_HDR)), Compsz);
				if(ret != 0)
					printf("Decompressed lzma fail\n");
				flush_dcache_range((u32)CONFIG_ITRON_SDRAM_BASE, (u32)(CONFIG_ITRON_SDRAM_BASE+Uncompsz));
			}
			else
			{
				nvt_writel((ulong)CONFIG_NVT_RUNFW_SDRAM_BASE, (ulong)nvt_binfo_comm_itron_comp_addr);
				nvt_writel((ulong)cpu_to_be32(pbfc->uiSizeComp), (ulong)nvt_binfo_comm_itron_comp_len);
				flush_dcache_range((u32)CONFIG_NVT_RUNFW_SDRAM_BASE,
						(u32)(CONFIG_NVT_RUNFW_SDRAM_BASE + firmware_size));
			}
		} else {
			printf("Non-compressed uItron\n");
			sprintf(cmd, "nand read 0x%x uitron ", CONFIG_ITRON_SDRAM_BASE);
			run_command(cmd, 0);
		}
		#elif defined(CONFIG_NVT_LINUX_SPINOR_BOOT)
		char cmd[50] = {0};
		loff_t part_off=0, part_size=0;
		get_part("uitron", &part_off, &part_size);
		sprintf(cmd, "sf read 0x%x 0x%llx 0x%x ", CONFIG_NVT_RUNFW_SDRAM_BASE, part_off, 0x200);
		run_command(cmd, 0);
		NVTPACK_BFC_HDR *pbfc = (NVTPACK_BFC_HDR *)CONFIG_NVT_RUNFW_SDRAM_BASE;

		/* Boot compressed uitron or non-compressed uitron */
		if(pbfc->uiFourCC == MAKEFOURCC('B', 'C', 'L', '1')) {
			printf("Compressed uItron\n");
			/*
			 * Trigger compressed uitron boot.
			 * Comp. uitron => CONFIG_NVT_RUNFW_SDRAM_BASE,
			 * UnComp. uitron => CONFIG_ITRON_SDRAM_BASE.
			 */
			size_t firmware_size = cpu_to_be32(pbfc->uiSizeComp) + sizeof(NVTPACK_BFC_HDR);
			u32 align_size = ALIGN_CEIL(firmware_size, ARCH_DMA_MINALIGN);
			sprintf(cmd, "sf read 0x%x 0x%llx 0x%x ", CONFIG_NVT_RUNFW_SDRAM_BASE, part_off, align_size);
			run_command(cmd, 0);

			debug("nor boot bfc UnCompsz:%d Compsz:%d Algorithm: %x \n", cpu_to_be32(pbfc->uiSizeUnComp), cpu_to_be32(pbfc->uiSizeComp), cpu_to_be32(pbfc->uiAlgorithm));
			if((cpu_to_be32(pbfc->uiAlgorithm) & 0xFF ) == 11)
			{
				size_t Compsz = cpu_to_be32(pbfc->uiSizeComp);
				size_t Uncompsz = cpu_to_be32(pbfc->uiSizeUnComp);
				printf("Decompressed lzma uItron\n");
				flush_dcache_range((u32)CONFIG_NVT_RUNFW_SDRAM_BASE,
						(u32)(CONFIG_NVT_RUNFW_SDRAM_BASE + firmware_size));
				ret = lzmaBuffToBuffDecompress((unsigned char *)CONFIG_ITRON_SDRAM_BASE, &Uncompsz, (unsigned char *)(CONFIG_NVT_RUNFW_SDRAM_BASE + sizeof(NVTPACK_BFC_HDR)), Compsz);
				if(ret != 0)
					printf("Decompressed lzma fail\n");
				flush_dcache_range((u32)CONFIG_ITRON_SDRAM_BASE, (u32)(CONFIG_ITRON_SDRAM_BASE+Uncompsz));
			}
			else
			{
				nvt_writel((ulong)CONFIG_NVT_RUNFW_SDRAM_BASE, (ulong)nvt_binfo_comm_itron_comp_addr);
				nvt_writel((ulong)cpu_to_be32(pbfc->uiSizeComp), (ulong)nvt_binfo_comm_itron_comp_len);
				flush_dcache_range((u32)CONFIG_NVT_RUNFW_SDRAM_BASE,
						(u32)(CONFIG_NVT_RUNFW_SDRAM_BASE + firmware_size));
			}
		} else {
			printf("Non-compressed uItron\n");
			sprintf(cmd, "sf read 0x%x 0x%llx 0x%x ", CONFIG_ITRON_SDRAM_BASE, part_off, part_size);
			run_command(cmd, 0);
		}
		#elif defined(CONFIG_NVT_LINUX_SD_BOOT)
		char cmd[30] = {0};
		sprintf(cmd, "fatload mmc 0:1 0x%x uItron.bin 0x%x ", CONFIG_NVT_RUNFW_SDRAM_BASE, 0x200);
		run_command(cmd, 0);
		NVTPACK_BFC_HDR *pbfc = (NVTPACK_BFC_HDR *)CONFIG_NVT_RUNFW_SDRAM_BASE;

		/* Boot compressed uitron or non-compressed uitron */
		if(pBFC->uiFourCC == MAKEFOURCC('B', 'C', 'L', '1')) {
			/* Trigger compressed uitron boot */
			sprintf(cmd, "fatload mmc 0:1 0x%x uItron.bin 0x%x ", CONFIG_NVT_RUNFW_SDRAM_BASE, pbfc->uiSizeComp);
			run_command(cmd, 0);

			debug("sd boot bfc UnCompsz:%d Compsz:%d Algorithm: %x \n", cpu_to_be32(pbfc->uiSizeUnComp), cpu_to_be32(pbfc->uiSizeComp), cpu_to_be32(pbfc->uiAlgorithm));
			if((cpu_to_be32(pbfc->uiAlgorithm) & 0xFF ) == 11)
			{
				size_t Compsz = cpu_to_be32(pbfc->uiSizeComp);
				size_t Uncompsz = cpu_to_be32(pbfc->uiSizeUnComp);
				printf("Decompressed lzma uItron\n");
				ret = lzmaBuffToBuffDecompress((unsigned char *)CONFIG_ITRON_SDRAM_BASE, &Uncompsz, (unsigned char *)(CONFIG_NVT_RUNFW_SDRAM_BASE + sizeof(NVTPACK_BFC_HDR)), Compsz);
				if(ret != 0)
					printf("Decompressed lzma fail\n");
				flush_dcache_range((u32)CONFIG_ITRON_SDRAM_BASE, (u32)(CONFIG_ITRON_SDRAM_BASE+Uncompsz));
			}
			else
			{
				nvt_writel((ulong)CONFIG_NVT_RUNFW_SDRAM_BASE, (ulong)NVT_CC_ITRON_COMP_ADDR);
				nvt_writel((ulong)cpu_to_be32(pbfc->uiSizeComp), (ulong)nvt_binfo_comm_itron_comp_len);
			}
		} else {
			sprintf(cmd, "fatload mmc 0:1 0x%x uItron.bin ", CONFIG_ITRON_SDRAM_BASE);
			run_command(cmd, 0);
		}
		#else
		int ret = 0;
		/* Check fw is valid */
		nvt_runfw_bin_chk_valid((ulong)CONFIG_NVT_RUNFW_SDRAM_BASE);
		/* Copy itron binary to destination address */
		ret = nvt_runfw_bin_unpack_to_dest((ulong)CONFIG_NVT_RUNFW_SDRAM_BASE, true);
		if (ret < 0)
			return ret;
		#endif /* CONFIG_NVT_LINUX_SPINAND_BOOT */
		nvt_boot_itron_fw_start();
	}

	return 0;
}

static int nvt_boot_dsp_bin(void)
{
	MODELEXT_HEADER* header;
	BININFO *p_bininfo = (BININFO *)modelext_get_cfg((unsigned char*)CONFIG_MODELEXT_SDRAM_BASE, MODELEXT_TYPE_BIN_INFO, &header);
#if defined(CONFIG_DSP1_FREERTOS) || defined(CONFIG_DSP2_FREERTOS)
	ulong result = COMM_UBOOT_BOOT_FUNC_LOAD_DSP_DONE;
#endif
	printf("NVT DSP boot.....\n");

#if defined(CONFIG_NVT_LINUX_SPINAND_BOOT)
#if defined(CONFIG_DSP1_FREERTOS)
	printf("DSP1_FREERTOS from flash\n");
	if (nvt_boot_dsp_load_from_storage(0, nvt_boot_dsp_nand_read) == 0) {
		nvt_boot_dsp_start(0);
	} else {
		result = COMM_UBOOT_BOOT_FUNC_LOAD_DSP_NG;
		printf("DSP1 load failed.\r\n");
	}
#else
	printf("DSP1_NONE\n");
#endif
#if defined(CONFIG_DSP2_FREERTOS)
	printf("DSP2_FREERTOS from flash\n");
	if (nvt_boot_dsp_load_from_storage(1, nvt_boot_dsp_nand_read) == 0) {
		nvt_boot_dsp_start(1);
	} else {
		result = COMM_UBOOT_BOOT_FUNC_LOAD_DSP_NG;
		printf("DSP2 load failed.\r\n");
	}
#else
	printf("DSP2_NONE\n");
#endif
#endif

#if defined(CONFIG_NVT_LINUX_SD_BOOT)
#if defined(CONFIG_DSP1_FREERTOS)
	printf("DSP1_FREERTOS from sd\n");
	if (nvt_boot_dsp_load_from_storage(0, nvt_boot_dsp_sd_read) == 0) {
		nvt_boot_dsp_start(0);
	} else {
		result = COMM_UBOOT_BOOT_FUNC_LOAD_DSP_NG;
		printf("DSP1 load failed.\r\n");
	}
#else
	printf("DSP1_NONE\n");
#endif
#if defined(CONFIG_DSP2_FREERTOS)
	printf("DSP2_FREERTOS from sd\n");
	if (nvt_boot_dsp_load_from_storage(1, nvt_boot_dsp_sd_read)  == 0) {
		nvt_boot_dsp_start(1);
	} else {
		result = COMM_UBOOT_BOOT_FUNC_LOAD_DSP_NG;
		printf("DSP2 load failed.\r\n");
	}
#else
	printf("DSP2_NONE\n");
#endif
#endif

#if defined(CONFIG_DSP1_FREERTOS) || defined(CONFIG_DSP2_FREERTOS)
	nvt_boot_dsp_load_done(result);
#endif


	return 0;
}

static int nvt_boot_linux_bin(void)
{
	char cmd[256] = {0};

#if defined(CONFIG_NVT_LINUX_SPINAND_BOOT)
	sprintf(cmd, "nand read 0x%x linux ", CONFIG_LINUX_SDRAM_START);
	run_command(cmd, 0);
#elif defined(CONFIG_NVT_LINUX_SPINOR_BOOT)
	loff_t part_off=0, part_size=0;
	/* Get linux partition offset and size */
	get_part("linux", &part_off, &part_size);
	sprintf(cmd,"sf read 0x%x 0x%llx 0x%llx", CONFIG_LINUX_SDRAM_START, part_off, part_size);
	ret = run_command(cmd, 0);
#elif defined(CONFIG_NVT_LINUX_SD_BOOT) /* CONFIG_NVT_LINUX_SD_BOOT */
	sprintf(cmd, "fatload mmc 0:1 0x%x uImage.bin ", CONFIG_LINUX_SDRAM_START);
	run_command(cmd, 0);
#elif defined(CONFIG_NVT_LINUX_EMMC_BOOT) /* CONFIG_NVT_LINUX_EMMC_BOOT */
	loff_t part_off=0, part_size=0;
	/* Get linux partition offset and size */
	get_part("linux", &part_off, &part_size);
	sprintf(cmd, "mmc read 0x%x 0x%llx 0x%llx ", CONFIG_LINUX_SDRAM_START, part_off, part_size);
	run_command(cmd, 0);
#else /* !defined(CONFIG_NVT_LINUX_SPINAND_BOOT) && !defined(CONFIG_NVT_LINUX_SD_BOOT) */
	int ret = 0;
	/* Check fw is valid */
	nvt_runfw_bin_chk_valid((ulong)CONFIG_NVT_RUNFW_SDRAM_BASE);
	/* Copy linux binary to destination address */
	ret = nvt_runfw_bin_unpack_to_dest((ulong)CONFIG_NVT_RUNFW_SDRAM_BASE, false);
	if (ret < 0)
		return ret;
#endif /* CONFIG_NVT_LINUX_SPINAND_BOOT */
}

static int nvt_boot_fw_init(void)
{
	char cmd[256] = {0};

	printf("NVT firmware boot.....\n");

#ifndef _NVT_LINUX_SMP_ON_

#if defined(CONFIG_NVT_LINUX_EMMC_BOOT)
	nvt_load_pstore_1st_section();
#endif

	nvt_boot_itron_bin();
#endif /* !_NVT_LINUX_SMP_ON_ */
#ifdef _CPU2_NONE_
	/* Core1 will turn off core2 if core2 is not linux */
	while(1);
#endif /* _CPU2_NONE_ */
	nvt_boot_dsp_bin();
	nvt_boot_linux_bin();

	return 0;
}
#endif /* CONFIG_NVT_LINUX_AUTODETECT */
#endif /* CONFIG_NVT_LINUX_AUTOLOAD */

static int nvt_boot_itron_fw_init(void)
{
	printf("NVT uItron boot.....\n");

#ifdef CONFIG_NVT_LINUX_AUTODETECT
	nvt_boot_itron_bin_auto();
#else /* !CONFIG_NVT_LINUX_AUTODETECT */
	nvt_boot_itron_bin();
#endif /* CONFIG_NVT_LINUX_AUTODETECT */

#ifdef _CPU2_NONE_
	/* Core1 will turn off core2 if core2 is not linux */
	while(1);
#endif /* _CPU2_NONE_ */
	return 0;
}

static int nvt_boot_dsp_fw_init(void)
{
	printf("NVT DSP boot.....\n");

#ifdef CONFIG_NVT_LINUX_AUTODETECT
	nvt_boot_dsp_bin_auto();
#else /* !CONFIG_NVT_LINUX_AUTODETECT */
	nvt_boot_dsp_bin();
#endif /* CONFIG_NVT_LINUX_AUTODETECT */
	return 0;
}

int do_nvt_boot_cmd(cmd_tbl_t * cmdtp, int flag, int argc, char * const argv[])
{
	int ret = 0;
	char cmdline[512] = {0};
	char cmd[256] = {0};
	char buf[255] = {0};

	/*
	 * To handle bootargs expanding for the kernel /proc/cmdline
	 */
	sprintf(buf,"%s ",getenv("bootargs"));
	strcat(cmdline, buf);

	/*
	 * Loading linux kernel
	 */
#ifdef	CONFIG_NVT_LINUX_AUTOLOAD
	#ifdef CONFIG_NVT_LINUX_AUTODETECT
	ret = nvt_boot_fw_init_detect();
	if (ret < 0) {
		printf("boot firmware init failed\n");
		return ret;
	}
	#else /* !CONFIG_NVT_LINUX_AUTODETECT */
	ret = nvt_boot_fw_init();
	if (ret < 0) {
		printf("boot firmware init failed\n");
		return ret;
	}
	#endif /* CONFIG_NVT_LINUX_AUTODETECT */
#else /* !CONFIG_NVT_LINUX_AUTOLOAD */
	/* FIXME: To do customized boot */

#endif /* CONFIG_NVT_LINUX_AUTOLOAD */

	/*Set up ramoops buffer*/
#define RAMOOPS_BUF_SIZE (32 * 1024)
#define RAMOOPS_REC_SIZE (16 * 1024)
	sprintf(buf,"ramoops.record_size=%d ", RAMOOPS_REC_SIZE);
	strcat(cmdline,buf);
	sprintf(buf,"ramoops.mem_address=0x00018000 ");
	strcat(cmdline,buf);
	sprintf(buf,"ramoops.mem_size=%d ", RAMOOPS_BUF_SIZE);
	strcat(cmdline,buf);

	/*
	 * Add bootarg memory extension parameter
	 */
#ifdef	CONFIG_NVT_LINUX_RAMDISK_BOOT
	/* RAMDISK boot */
	sprintf(buf,"mem=0x%x@0x%x mem=0x%x@0x%x ", CONFIG_UBOOT_SDRAM_SIZE + CONFIG_LINUX_SDRAM_SIZE, CONFIG_LINUX_SDRAM_BASE, CONFIG_RAMDISK_SDRAM_SIZE, CONFIG_RAMDISK_SDRAM_BASE);
#else
	/* SD boot */
	sprintf(buf,"mem=0x%x@0x%x ", CONFIG_UBOOT_SDRAM_SIZE + CONFIG_LINUX_SDRAM_SIZE, CONFIG_LINUX_SDRAM_BASE);
#endif
	strcat(cmdline, buf);

#if	CONFIG_LINUX_SDRAM_MEMRESERVED_SIZE != 0
	sprintf(buf,"mem=0x%x@0x%x ", CONFIG_LINUX_SDRAM_MEMRESERVED_SIZE, CONFIG_LINUX_SDRAM_MEMRESERVED_BASE);
	strcat(cmdline, buf);
#endif

	parsing_sdio_driving();
	sprintf(buf,"sdio_driving=%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d ", SDIO_DS_MODE_CLK_DRIVING, SDIO_DS_MODE_CMD_DRIVING, SDIO_DS_MODE_DATA_DRIVING, \
									SDIO_HS_MODE_CLK_DRIVING, SDIO_HS_MODE_CMD_DRIVING, SDIO_HS_MODE_DATA_DRIVING, \
									SDIO_SDR50_MODE_CLK_DRIVING, SDIO_SDR50_MODE_CMD_DRIVING, SDIO_SDR50_MODE_DATA_DRIVING, \
									SDIO_SDR104_MODE_CLK_DRIVING, SDIO_SDR104_MODE_CMD_DRIVING, SDIO_SDR104_MODE_DATA_DRIVING);
	strcat(cmdline, buf);

	nvt_tm0_cnt_end = get_nvt_timer0_cnt();
	/* boot time recording */
	sprintf(buf,"bootts=%lu,%lu resume_addr=0x%08lx ", nvt_tm0_cnt_beg, nvt_tm0_cnt_end, nvt_binfo_comm_core2_start);
	strcat(cmdline, buf);

	setenv("bootargs",cmdline);

	/*
	 * Issue boot command
	 */
#ifdef  CONFIG_NVT_LINUX_RAMDISK_BOOT
	sprintf(cmd, "bootm %x %x ", CONFIG_LINUX_SDRAM_START, CONFIG_RAMDISK_SDRAM_BASE);
#else
	sprintf(cmd, "bootm %x ", CONFIG_LINUX_SDRAM_START);
#endif
	printf("%s Uboot boot time: \n\tstart:\t%lu us\n\tending:\t%lu us %s\r\n", ANSI_COLOR_GREEN, nvt_tm0_cnt_beg, nvt_tm0_cnt_end, ANSI_COLOR_RESET);
	run_command(cmd, 0);
	return 0;
}

U_BOOT_CMD(
	nvt_boot,	1,	0,	do_nvt_boot_cmd,
	"To do nvt platform boot init.","\n"
);

int do_nvt_booti_cmd(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
	int ret = 0;

	ret = nvt_boot_itron_fw_init();
	if (ret < 0) {
		printf("boot uitron firmware init failed\n");
		return ret;
	}

	return 0;
}

U_BOOT_CMD(
	nvt_booti,	1,	1,	do_nvt_booti_cmd,
	"nvt_booti     - Loading itron and boot itron (Without Linux). \n",
	"\n"
);

int do_nvt_booti_tftp_cmd(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
	nvt_boot_itron_fw_start();

	return 0;
}

U_BOOT_CMD(
	nvt_booti_tftp,	1,	1,	do_nvt_booti_tftp_cmd,
	"nvt_booti_tftp     - Boot itron without loading. \n",
	"\n"
);

int do_nvt_bootd_cmd(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
	int ret = 0;

	ret = nvt_boot_dsp_fw_init();
	if (ret < 0) {
		printf("boot dsp firmware init failed\n");
		return ret;
	}

	return 0;
}

U_BOOT_CMD(
	nvt_bootd,	1,	1,	do_nvt_bootd_cmd,
	"nvt_bootd     - Loading dsp and boot dsp (Without Linux). \n",
	"\n"
);

int do_nvt_update_all_cmd(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
	ulong addr = 0, size = 0;
	int ret = 0;

	if (argc != 3)
		return CMD_RET_USAGE;

	size = cmd_get_data_size(argv[0], 4);
	if (size < 0)
		return 1;

	addr = simple_strtoul(argv[1], NULL, 16);
	size = simple_strtoul(argv[2], NULL, 10);

	printf("%s addr: 0x%08lx, size: 0x%08lx(%lu) bytes %s\r\n", ANSI_COLOR_YELLOW, addr, size, size, ANSI_COLOR_RESET);

	ret = nvt_process_all_in_one(addr, size, 0);
	if (ret < 0) {
		printf("Update nvt all-in-one image failed.\n");
		return ret;
	}

	return 0;
}

U_BOOT_CMD(
	nvt_update_all,	3,	0,	do_nvt_update_all_cmd,
	"To Update all-in-one image from memory address and size \n",
	"[address(hex)][size(dec)] \n"
);
