/**
    NVT firmware update
    To do all-in-one firmware check and update
    @file       nvt_fw_update.c
    @ingroup
    @note
    Copyright   Novatek Microelectronics Corp. 2016.  All rights reserved.

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License version 2 as
    published by the Free Software Foundation.
*/

#include <common.h>
#include <fs.h>
#include <u-boot/md5.h>
#include <malloc.h>
#include <nvt_type.h>
#include <nand.h>
#include <mmc.h>
#include <spi_flash.h>
#include <image-sparse.h>
#include <modelext/modelext_info.h>
#include <modelext/modelext_parser.h>
#include <modelext/bin_info.h>
#include <modelext/top.h>
#include <modelext/interrupt.h>
#include <modelext/emb_partition_info.h>
#include <modelext/gpio_info.h>
#include <modelext/dram_partition_info.h>
#include <modelext/model_cfg.h>
#include <asm/arch/nvt_common.h>
#include "nvtpack.h"
#include "na51000_utils.h"
#include "gxdsp.h"

#if defined(CONFIG_NVT_FW_UPDATE_LED)
#include <asm/arch/pwm.h>
#endif

#define UPDATE_ALL_IN_ONE_RETRY_TIMES	3
#define CFG_MODEL_EXT_PARTITION 1

#define UINT32_SWAP(data)           (((((UINT32)(data)) & 0x000000FF) << 24) | \
                                     ((((UINT32)(data)) & 0x0000FF00) << 8) | \
                                     ((((UINT32)(data)) & 0x00FF0000) >> 8) | \
                                     ((((UINT32)(data)) & 0xFF000000) >> 24))   ///< Swap [31:24] with [7:0] and [23:16] with [15:8].

// {8827BE90-36CD-4FC2-A987-73A8484E84B1}
static const GUID GUID_FW =
{ 0x8827be90, 0x36cd, 0x4fc2, { 0xa9, 0x87, 0x73, 0xa8, 0x48, 0x4e, 0x84, 0xb1 } };

#ifdef CONFIG_NVT_LINUX_SPINOR_BOOT
extern struct spi_flash *board_init_spi_flash;
#endif

static void nvt_update_partitions(unsigned int addr, unsigned int size, unsigned int part_off, unsigned int PartitionSize)
{
	char command[128];
	#if defined(CONFIG_NVT_LINUX_SPINAND_BOOT)
	u32 align_size = ALIGN_CEIL(size, nand_get_block_size());
	#elif defined(CONFIG_NVT_LINUX_SPINOR_BOOT)
	u32 align_size = ALIGN_CEIL(size, board_init_spi_flash->erase_size);
	#elif defined(CONFIG_NVT_LINUX_EMMC_BOOT)
	u32 align_size = ALIGN_CEIL(size, MMC_MAX_BLOCK_LEN);
	/* Using block unit */
	align_size /= MMC_MAX_BLOCK_LEN;
	#endif

	memset(command, 0, sizeof(command));
	#if defined(CONFIG_NVT_LINUX_SPINAND_BOOT)
	#if defined(CONFIG_NVT_FW_UPDATE_LED) && defined(CONFIG_NVT_PWM)
	pwm_setup_start(NVT_PWMLED, PWM_LED_ERASE, PWM_SIGNAL_TYPE);
	#endif
	sprintf(command, "nand erase 0x%x 0x%x", part_off, PartitionSize);
	run_command(command, 0);
	#if defined(CONFIG_NVT_FW_UPDATE_LED) && defined(CONFIG_NVT_PWM)
	pwm_reload(NVT_PWMLED, PWM_LED_PROGRAM, PWM_SIGNAL_TYPE);
	#endif
	sprintf(command, "nand write 0x%x 0x%x 0x%x", addr, part_off, align_size);
	run_command(command, 0);
	#elif defined(CONFIG_NVT_LINUX_SPINOR_BOOT)
	#if defined(CONFIG_NVT_FW_UPDATE_LED) && defined(CONFIG_NVT_PWM)
	pwm_setup_start(NVT_PWMLED, PWM_LED_ERASE, PWM_SIGNAL_TYPE);
	#endif
	sprintf(command, "sf erase 0x%x +0x%x", part_off, PartitionSize);
	run_command(command, 0);
	#if defined(CONFIG_NVT_FW_UPDATE_LED) && defined(CONFIG_NVT_PWM)
	pwm_reload(NVT_PWMLED, PWM_LED_PROGRAM, PWM_SIGNAL_TYPE);
	#endif
	sprintf(command, "sf write 0x%x 0x%x 0x%x", addr, part_off, align_size);
	run_command(command, 0);
	#elif defined(CONFIG_NVT_LINUX_EMMC_BOOT)
	#if defined(CONFIG_NVT_FW_UPDATE_LED) && defined(CONFIG_NVT_PWM)
	pwm_setup_start(NVT_PWMLED, PWM_LED_ERASE, PWM_SIGNAL_TYPE);
	#endif
	//sprintf(command, "mmc erase 0x%x 0x%x", part_off, align_size);
	//run_command(command, 0);
	#if defined(CONFIG_NVT_FW_UPDATE_LED) && defined(CONFIG_NVT_PWM)
	pwm_reload(NVT_PWMLED, PWM_LED_PROGRAM, PWM_SIGNAL_TYPE);
	#endif
	sprintf(command, "mmc write 0x%x 0x%x 0x%x", addr, part_off, align_size);
	run_command(command, 0);
	#endif /* CONFIG_NVT_LINUX_EMMC_BOOT */
	#if defined(CONFIG_NVT_FW_UPDATE_LED) && defined(CONFIG_NVT_PWM)
	pwm_close(NVT_PWMLED, 0);
	#endif
}

static int nvt_read_partitions(unsigned int addr, unsigned int size, unsigned int part_off, unsigned int pat_id, unsigned short embtype)
{
	char command[128];

	memset(command, 0, sizeof(command));
	#if defined(CONFIG_NVT_LINUX_SPINAND_BOOT)
	sprintf(command, "nand read 0x%x 0x%x 0x%x", addr, part_off, ALIGN_CEIL(size, nand_get_block_size()));
	run_command(command, 0);
	#elif defined(CONFIG_NVT_LINUX_SPINOR_BOOT)
	sprintf(command, "sf read 0x%x 0x%x 0x%x", addr, part_off, ALIGN_CEIL(size, board_init_spi_flash->erase_size));
	run_command(command, 0);
	#elif defined(CONFIG_NVT_LINUX_EMMC_BOOT)
	u32 align_size = ALIGN_CEIL(size, MMC_MAX_BLOCK_LEN);
	/* Using block unit */
	align_size /= MMC_MAX_BLOCK_LEN;
	sprintf(command, "mmc read 0x%x 0x%x 0x%x", addr, part_off, align_size);
	run_command(command, 0);
	#endif /* !CONFIG_NVT_LINUX_SPINAND_BOOT */

	return 0;
}

#ifdef CONFIG_NVT_LINUX_EMMC_BOOT
static lbaint_t nvt_mmc_sparse_write(struct sparse_storage *info, lbaint_t blk, lbaint_t blkcnt, const void *buffer)
{
	char command[128];

	sprintf(command, "mmc write 0x%lx 0x%lx 0x%lx", (unsigned long)buffer, blk, blkcnt);
	run_command(command, 0);
	return blkcnt;
}

static lbaint_t nvt_mmc_sparse_read(struct sparse_storage *info, lbaint_t blk, lbaint_t blkcnt, const void *buffer)
{
	char command[128];

	sprintf(command, "mmc read 0x%lx 0x%lx 0x%lx", (unsigned long)buffer, blk, blkcnt);
	run_command(command, 0);
	return blkcnt;
}

static lbaint_t nvt_mmc_sparse_reserve(struct sparse_storage *info, lbaint_t blk, lbaint_t blkcnt)
{
	return blkcnt;
}
#endif

static void nvt_update_rootfs_partition(unsigned int addr, unsigned int size, unsigned int part_off, unsigned int part_size, EMB_PARTITION *pEmb)
{
	char command[128];

	memset(command, 0, sizeof(command));
#if defined(CONFIG_NVT_LINUX_SPINAND_BOOT)
	#if defined(CONFIG_NVT_FW_UPDATE_LED) && defined(CONFIG_NVT_PWM)
	pwm_setup_start(NVT_PWMLED, PWM_LED_ERASE, PWM_SIGNAL_TYPE);
	#endif
	sprintf(command, "nand erase 0x%x 0x%x", part_off, part_size);
	run_command(command, 0);
	sprintf(command, "nand info");
	run_command(command, 0);
#elif defined(CONFIG_NVT_LINUX_EMMC_BOOT)
	#if defined(CONFIG_NVT_FW_UPDATE_LED) && defined(CONFIG_NVT_PWM)
	pwm_setup_start(NVT_PWMLED, PWM_LED_ERASE, PWM_SIGNAL_TYPE);
	#endif
	//sprintf(command, "mmc erase 0x%x 0x%x", part_off, part_size);
	//run_command(command, 0);
#else
	#if defined(CONFIG_NVT_FW_UPDATE_LED) && defined(CONFIG_NVT_PWM)
	pwm_setup_start(NVT_PWMLED, PWM_LED_ERASE, PWM_SIGNAL_TYPE);
	#endif
	sprintf(command, "sf erase 0x%x +0x%x", part_off, part_size);
	run_command(command, 0);
#endif

#ifdef CONFIG_NVT_BIN_CHKSUM_SUPPORT
	// Skip nvt head info
	addr += 64;
	size -= 64;
#endif

#if defined(CONFIG_NVT_FW_UPDATE_LED) && defined(CONFIG_NVT_PWM)
	pwm_reload(NVT_PWMLED, PWM_LED_PROGRAM, PWM_SIGNAL_TYPE);
#endif

#if defined(CONFIG_NVT_JFFS2_SUPPORT)
	printf("Update: %s %s\n", __func__, "JFFS2");
	#if defined(CONFIG_NVT_LINUX_SPINAND_BOOT)
	sprintf(command, "nand write.trimffs 0x%x rootfs%u 0x%x", addr, pEmb->OrderIdx, size);
	run_command(command, 0);
	#else
	sprintf(command, "sf write 0x%x 0x%x 0x%x", addr, part_off, size);
	run_command(command, 0);
	#endif /* !CONFIG_NVT_LINUX_SPINAND_BOOT */

#elif defined(CONFIG_NVT_SQUASH_SUPPORT)
	printf("Update: %s %s\n", __func__, "SquashFS");
	#if defined(CONFIG_NVT_LINUX_SPINAND_BOOT)
	sprintf(command, "nand write 0x%x rootfs%u 0x%x", addr, pEmb->OrderIdx, size);
	run_command(command, 0);
	#else
	sprintf(command, "sf write 0x%x 0x%x 0x%x", addr, part_off, size);
	run_command(command, 0);
	#endif /* !CONFIG_NVT_LINUX_SPINAND_BOOT */

	#if defined(CONFIG_NVT_FW_UPDATE_LED) && defined(CONFIG_NVT_PWM)
	pwm_close(NVT_PWMLED, 0);
	#endif
#elif defined(CONFIG_NVT_EXT4_SUPPORT)
	printf("Update: %s %s\n", __func__, "EXT4");
	u32 align_size = ALIGN_CEIL(size, MMC_MAX_BLOCK_LEN);
	/* Using block unit */
	align_size /= MMC_MAX_BLOCK_LEN;
	if (is_sparse_image((void*)addr)) {
		struct sparse_storage sparse;
		sparse.blksz = MMC_MAX_BLOCK_LEN;
		sparse.start = (lbaint_t)part_off;
		sparse.size = (lbaint_t)part_size;
		sparse.read = NULL;
		sparse.write = nvt_mmc_sparse_write;
		sparse.reserve = nvt_mmc_sparse_reserve;
		printf("Flashing sparse ext4 image at offset 0x%lx\n", sparse.start);
		write_sparse_image(&sparse, "Rootfs.ext4", (void *)addr, align_size);
	} else {
		printf("Flashing raw ext4 or fat image at offset 0x%x\n", part_off);
		sprintf(command, "mmc write 0x%x 0x%x 0x%x", addr, part_off, align_size);
		run_command(command, 0);
	}
	#if defined(CONFIG_NVT_FW_UPDATE_LED) && defined(CONFIG_NVT_PWM)
	pwm_close(NVT_PWMLED, 0);
	#endif
#else
	printf("Update: %s %s\n", __func__, "UBIFS");

	sprintf(command, "nand write.trimffs 0x%x rootfs%u 0x%x", addr, pEmb->OrderIdx, size);
	printf("%s\n", command);
	run_command(command, 0);
#endif /* !CONFIG_NVT_JFFS2_SUPPORT && !CONFIG_NVT_SQUASH_SUPPORT */
}

static int nvt_read_rootfs_partition(unsigned int addr, unsigned int size, unsigned int part_off, unsigned int part_size, unsigned int pat_id, EMB_PARTITION *pEmb)
{
	char command[128];
	int ret = 0;

	memset(command, 0, sizeof(command));

#ifdef CONFIG_NVT_BIN_CHKSUM_SUPPORT
	// Skip nvt head info
	addr += 64;
	size -= 64;
#endif

#if defined(CONFIG_NVT_LINUX_SPINAND_BOOT)
	/* We should avoid read size is not block size alignment problem occurred */
	unsigned short *buf = malloc(nand_get_block_size());
	unsigned int first_part_size = ALIGN_FLOOR(size, nand_get_block_size());
	unsigned int second_part_size = size - first_part_size;
	sprintf(command, "nand read 0x%x 0x%x 0x%x", addr, part_off, ALIGN_FLOOR(size, nand_get_block_size()));
	run_command(command, 0);
	sprintf(command, "nand read 0x%lx 0x%x 0x%x", (unsigned long)buf, part_off+first_part_size, nand_get_block_size());
	run_command(command, 0);
	memcpy((void*)(addr+first_part_size), (void*)buf, second_part_size);
	free(buf);
#elif defined(CONFIG_NVT_LINUX_SPINOR_BOOT)
	sprintf(command, "sf read 0x%x 0x%x 0x%x", addr, part_off, ALIGN_CEIL(size, board_init_spi_flash->erase_size));
	run_command(command, 0);
#elif defined(CONFIG_NVT_LINUX_EMMC_BOOT)
	if (is_sparse_image((void*)addr)) {
		struct sparse_storage sparse;
		sparse.blksz = MMC_MAX_BLOCK_LEN;
		sparse.start = (lbaint_t)part_off;
		sparse.size = (lbaint_t)part_size;
		sparse.write = NULL;
		sparse.read = nvt_mmc_sparse_read;
		sparse.reserve = nvt_mmc_sparse_reserve;
		printf("Read sparse ext4 image at offset 0x%lx\n", sparse.start);
		read_sparse_image(&sparse, "Rootfs.ext4", (void *)addr, (ALIGN_CEIL(size, MMC_MAX_BLOCK_LEN)/MMC_MAX_BLOCK_LEN));
	} else {
		printf("Read raw ext4 image at offset 0x%x\n", part_off);
		sprintf(command, "mmc read 0x%x 0x%x 0x%x", addr, part_off, (ALIGN_CEIL(size, MMC_MAX_BLOCK_LEN)/MMC_MAX_BLOCK_LEN));
		run_command(command, 0);
	}
#endif /* CONFIG_NVT_LINUX_SPINAND_BOOT */

#if defined(CONFIG_NVT_UBIFS_SUPPORT)
	printf("Read: %s %s\n", __func__, "UBIFS");
	sprintf(command, "ubi part rootfs%u", pEmb->OrderIdx);
	run_command(command, 0);

	sprintf(command, "ubifsmount ubi:rootfs");
	run_command(command, 0);

	sprintf(command, "ubifsls");
	run_command(command, 0);
#elif defined(CONFIG_NVT_SQUASH_SUPPORT)
	printf("Read: %s %s\n", __func__, "SQUASHFS");
#elif defined(CONFIG_NVT_EXT4_SUPPORT)
	printf("Read: %s %s\n", __func__, "EXT4");
#else
	printf("Read: %s %s\n", __func__, "JFFS2");
#endif

	return ret;
}

static UINT32 MemCheck_CalcCheckSum16Bit(UINT32 uiAddr,UINT32 uiLen)
{
	printf("addr=%08lX,size=%08lX\n",uiAddr,uiLen);
	UINT32 i,uiSum = 0;
	UINT16 *puiValue = (UINT16 *)uiAddr;

	for (i=0; i<(uiLen >> 1); i++)
	{
	uiSum += (*(puiValue + i) + i);
	}

	uiSum &= 0x0000FFFF;

	return uiSum;
}

static int nvt_chk_last_ebr(ulong ebr_addr, unsigned int ebr_part_offset, EMB_PARTITION *pEmb)
{
	unsigned char *buf;
	u32 disk_capacity, new_size, old_size, part_addr;
	int i, last_part_idx, partition_number, last_ebr_idx, first_mbr_idx;

	//Find the index of the last pstore/rootfs partition
	if(pEmb==NULL)
	{
		printf("failed to get current partition resource.\r\n");
		return CMD_RET_FAILURE;
	}

	last_part_idx = -1;
	for(i = (EMB_PARTITION_INFO_COUNT - 1) ; i >= 0 ; i--)
	{
		if(pEmb[i].EmbType == EMBTYPE_UNKNOWN)
			continue;
		else if(pEmb[i].EmbType == EMBTYPE_ROOTFS || pEmb[i].EmbType == EMBTYPE_PSTORE){
			if(pEmb[i].PartitionSize != 0){
				printf("size of last partition is not 0, no need to fix\r\n");
				return CMD_RET_SUCCESS;
			}
			else{
				last_part_idx = i;
			}
			break;
		}
		else{
			printf("last partition is of type %d, not rootfs or pstore, no need to fix\r\n", pEmb[i].EmbType);
			return CMD_RET_SUCCESS;
		}
	}

	if(last_part_idx == -1){
		printf("no rootfs,pstore partition, no need to fix\r\n");
		return CMD_RET_SUCCESS;
	}

	//Find the index of the last ebr
	last_ebr_idx = -1;
	for(i = (EMB_PARTITION_INFO_COUNT - 1) ; i >= 0 ; i--)
		if(pEmb[i].EmbType == EMBTYPE_MBR){
			last_ebr_idx = i;
			break;
		}

	if(last_ebr_idx == -1){
		printf("fail to get last ebr's index\r\n");
		return CMD_RET_FAILURE;
	}

	//Find the index of mbr
	first_mbr_idx = -1;
	for(i = 0 ; i < EMB_PARTITION_INFO_COUNT ; i++)
		if(pEmb[i].EmbType == EMBTYPE_MBR){
			first_mbr_idx = i;
			break;
		}

	if(first_mbr_idx == -1){
		printf("fail to get 1st mbr's index\r\n");
		return CMD_RET_FAILURE;
	}

	//If ebr from argument is neither 1st mbr nor last ebr, ignore it
	if(ebr_part_offset != pEmb[first_mbr_idx].PartitionOffset && ebr_part_offset != pEmb[last_ebr_idx].PartitionOffset)
		return CMD_RET_SUCCESS;

	//Get pstore/rootfs partition number
	partition_number = 0;
	for(i = 0 ; i < EMB_PARTITION_INFO_COUNT ; i++)
		if(pEmb[i].EmbType == EMBTYPE_ROOTFS || pEmb[i].EmbType == EMBTYPE_PSTORE)
			partition_number++;

	//If the last partition ls logical, make sure there is a EBR partition ahead
	if(partition_number >= 4){
		if((last_part_idx - 1) != last_ebr_idx){
			printf("last ebr index(%d) is not right ahead of the last mbr ebr index(%d)\r\n", last_ebr_idx, last_part_idx);
			return CMD_RET_FAILURE;
		}
	}
	//rootfs,pstore number < 4, there should be no ebr, so last_ebr_idx should eb equal to first_mbr_idx
	else if(first_mbr_idx != last_ebr_idx){
		printf("only %d rootfs,pstore partions, but mbr index(%d) != last ebr index(%d)\r\n", partition_number, first_mbr_idx, last_ebr_idx);
		return CMD_RET_FAILURE;
	}

	//Check mbr/ebr signature
	buf = (unsigned char*)ebr_addr;
	if(buf[510] != 0x55 || buf[511] != 0xAA){
		printf("invalid mbr ebr signature 0x%x 0x%x, they should be 0x55 0xAA\r\n", (unsigned int)buf[510], (unsigned int)buf[511]);
		return CMD_RET_FAILURE;
	}

	//Get emmc's max capacity
	extern u32 get_emmc_capacity(void);
	disk_capacity = get_emmc_capacity();
	printf("emmc capacity is %d sectors\r\n", disk_capacity);
	if(disk_capacity == 0){
		printf("fail to get emmc's capacity\r\n");
		return CMD_RET_FAILURE;
	}

	//Fix MBR's size field
	if(ebr_part_offset == pEmb[first_mbr_idx].PartitionOffset){
		//Fix primary partition's size
		if(partition_number < 4){
			new_size = disk_capacity - pEmb[last_part_idx].PartitionOffset;
			buf = (unsigned char*)(ebr_addr + 446 + ((partition_number - 1) * 16) + 12);
			old_size = (buf[0] | (buf[1] << 8) | (buf[2] << 16) | (buf[3] << 24));
			printf("last primary partition old size = %d ; new size = %d\r\n", old_size, new_size);
			buf[0] = (new_size & 0x0FF);
			buf[1] = ((new_size & 0x0FF00) >> 8);
			buf[2] = ((new_size & 0x0FF0000) >> 16);
			buf[3] = ((new_size & 0xFF000000) >> 24);
		}
		//Fix extend partition's size
		else{
			buf = (unsigned char*)(ebr_addr + 494 + 8);
			part_addr = (buf[0] | (buf[1] << 8) | (buf[2] << 16) | (buf[3] << 24));
			new_size = disk_capacity - part_addr;
			buf = (unsigned char*)(ebr_addr + 494 + 12);
			old_size = (buf[0] | (buf[1] << 8) | (buf[2] << 16) | (buf[3] << 24));
			printf("extended partition old size = %d ; new size = %d\r\n", old_size, new_size);
			buf[0] = (new_size & 0x0FF);
			buf[1] = ((new_size & 0x0FF00) >> 8);
			buf[2] = ((new_size & 0x0FF0000) >> 16);
			buf[3] = ((new_size & 0xFF000000) >> 24);
		}
	}
	//Fix EBR's size field
	else{
		new_size = disk_capacity - pEmb[last_part_idx].PartitionOffset;
		buf = (unsigned char*)(ebr_addr + 446 + 12);
		old_size = (buf[0] | (buf[1] << 8) | (buf[2] << 16) | (buf[3] << 24));
		printf("last ebr old size = %d ; new size = %d\r\n", old_size, new_size);
		buf[0] = (new_size & 0x0FF);
		buf[1] = ((new_size & 0x0FF00) >> 8);
		buf[2] = ((new_size & 0x0FF0000) >> 16);
		buf[3] = ((new_size & 0xFF000000) >> 24);
	}

	return CMD_RET_SUCCESS;
}

static int nvt_chk_loader(ulong addr, unsigned int size, unsigned int pat_id)
{
	if(MemCheck_CalcCheckSum16Bit(addr, size)!=0)
	{
		printf("loader pat%d, res check sum fail.\r\n",pat_id);
		return -1;
	}
	return 0;
}

static int nvt_chk_modelext(ulong addr, unsigned int size, unsigned int pat_id)
{
	struct chk_ver {
		char*         name;
		MODELEXT_TYPE type;
		unsigned int  version;
	};
	struct chk_ver chk_ver_list[] = {
		{"MODELEXT_INFO_VER", MODELEXT_TYPE_INFO, MODELEXT_INFO_VER},
		{"BIN_INFO_VER", MODELEXT_TYPE_BIN_INFO, BIN_INFO_VER},
		{"PIN_GROUP_CONFIG_VER", MODELEXT_TYPE_PINMUX_CFG, PIN_GROUP_CONFIG_VER},
		{"INT_GIC_ID_DST_VER", MODELEXT_TYPE_INTDIR_CFG, INT_GIC_ID_DST_VER},
		{"EMB_PARTITION_INFO_VER", MODELEXT_TYPE_EMB_PARTITION, EMB_PARTITION_INFO_VER},
		{"GPIO_INFO_VER", MODELEXT_TYPE_GPIO_INFO, GPIO_INFO_VER},
		{"DRAM_PARTITION_INFO_VER", MODELEXT_TYPE_DRAM_PARTITION, DRAM_PARTITION_INFO_VER},
		{"MODEL_CFG_VER", MODELEXT_TYPE_MODEL_CFG, MODEL_CFG_VER},
	};
	int i;
	MODELEXT_HEADER *header;
	if(MemCheck_CalcCheckSum16Bit(addr, size)!=0)
	{
		printf("modelext pat%d, res check sum fail.\r\n",pat_id);
		return -1;
	}
	//check all types version, TODO: int_dir's version
	for( i=0; i< ARRAY_SIZE(chk_ver_list); i++)
	{
		if(modelext_get_cfg((unsigned char*)addr, chk_ver_list[i].type, &header) == NULL)
		{
			printf("cannot get MODELEXT_TYPE_ = %d.\r\n", chk_ver_list[i].type);
			return -1;
		}
		else if(header->version != chk_ver_list[i].version)
		{
			printf("%s version not match. 0x%08X(bin) : 0x%08X(uboot)\r\n",
				chk_ver_list[i].name, header->version, chk_ver_list[i].version);
			return -1;
		}
	}
	return 0;
}

static int nvt_chk_uitron(ulong addr, unsigned int size, unsigned int pat_id)
{
	if(MemCheck_CalcCheckSum16Bit(addr, size)!=0)
	{
		printf("uitron pat%d, res check sum fail.\r\n",pat_id);
		return -1;
	}
	return 0;
}

static int nvt_chk_ecos(ulong addr, unsigned int size, unsigned int pat_id)
{
	if(MemCheck_CalcCheckSum16Bit(addr, size)!=0)
	{
		printf("ecos pat%d, res check sum fail.\r\n",pat_id);
		return -1;
	}
	return 0;
}

static int nvt_chk_uboot(ulong addr, unsigned int size, unsigned int pat_id)
{
#ifdef CONFIG_NVT_BIN_CHKSUM_SUPPORT
	extern HEADINFO gHeadInfo;
	u32 tag_offset = (u32)gHeadInfo.BinInfo_1 - (u32)gHeadInfo.CodeEntry;
	char *tag = (char*)(addr + tag_offset);
	NVTPACK_BFC_HDR *pbfc = (NVTPACK_BFC_HDR *)addr;

	/* This is for compression check */
	if(pbfc->uiFourCC == MAKEFOURCC('B', 'C', 'L', '1')) {
		lz_uncompress((unsigned char *)(addr + sizeof(NVTPACK_BFC_HDR)),
				(unsigned char *)CONFIG_MEM_SIZE - cpu_to_be32(pbfc->uiSizeUnComp),
				(size_t)cpu_to_be32(pbfc->uiSizeComp));

		char *tag = (char*)(CONFIG_MEM_SIZE - cpu_to_be32(pbfc->uiSizeUnComp) + tag_offset);

		if(MemCheck_CalcCheckSum16Bit(CONFIG_MEM_SIZE - cpu_to_be32(pbfc->uiSizeUnComp), cpu_to_be32(pbfc->uiSizeUnComp))!=0)
		{
			printf("uboot pat%d, check sum fail.\r\n",pat_id);
			return -1;
		} else if(strncmp(tag, gHeadInfo.BinInfo_1, 8) !=0 ) {
			printf("uboot pat%d, tag not match %8s(expect) != %8s(bin).\r\n"
			    ,pat_id ,gHeadInfo.BinInfo_1, tag);
			return -1;
		}
	} else {
		if(MemCheck_CalcCheckSum16Bit(addr, size)!=0)
		{
			printf("uboot pat%d, check sum fail.\r\n",pat_id);
			return -1;
		} else if(strncmp(tag, gHeadInfo.BinInfo_1, 8) !=0 ) {
			printf("uboot pat%d, tag not match %8s(expect) != %8s(bin).\r\n"
			    ,pat_id ,gHeadInfo.BinInfo_1, tag);
			return -1;
		}
	}
#endif /* CONFIG_NVT_BIN_CHKSUM_SUPPORT */
	return 0;
}

static int nvt_chk_linux(ulong addr, unsigned int size, unsigned int pat_id)
{
	if(image_check_dcrc((const image_header_t*)(addr))!=1)
	{
		printf("linux-kernel pat%d, res check sum fail.\r\n",pat_id);
		return -1;
	}
	return 0;
}

static int nvt_chk_dsp(ulong addr, unsigned int size, unsigned int pat_id)
{
	u32 j;
	DSP_FW_HEADER* p_hdr = (DSP_FW_HEADER*)(addr);
	DSP_BIN_INFO* pInfo[] = {
		&p_hdr->BLoader,
		&p_hdr->ProgInt,
		&p_hdr->DataInt,
		&p_hdr->ProgExt,
		&p_hdr->DataExt
	};

	if(p_hdr->uiFourCC == MAKEFOURCC('B', 'C', 'L', '1')) {
		if (MemCheck_CalcCheckSum16Bit(addr, size) == 0) {
			return 0;
		} else {
			printf("pat%d, dsp check sum failed.\r\n",pat_id);
		}
	} else if(p_hdr->uiFourCC == MAKEFOURCC('D','S','P','1') ||
		p_hdr->uiFourCC == MAKEFOURCC('D','S','P','2'))	{
		for(j=0; j<sizeof(pInfo)/sizeof(DSP_BIN_INFO*);j++)
		{
			UINT16 uiCheckSum = (UINT16)MemCheck_CalcCheckSum16Bit(addr+pInfo[j]->Offset,pInfo[j]->Size);
			UINT16 uiComplement = (UINT16)pInfo[j]->CheckSum;
			if((UINT16)(uiCheckSum+uiComplement)!=0)
			{
				printf("pat%d, dsp sub-pat[%d] check sum failed.\r\n",pat_id,j);
				return -1;
			}
		}
		return 0;
	}
	printf("pat%d, dsp error fourcc.\r\n",pat_id);
	return -1;
}

static int nvt_chk_rootfs(ulong addr, unsigned int size, unsigned int pat_id)
{
#ifdef CONFIG_NVT_BIN_CHKSUM_SUPPORT
	UINT32 uiContextBuf = addr;
	if(*(UINT32*)uiContextBuf == MAKEFOURCC('C','K','S','M'))
	{
		NVTPACK_CHKSUM_HDR* pHdr = (NVTPACK_CHKSUM_HDR*)uiContextBuf;
		if(pHdr->uiVersion != NVTPACK_CHKSUM_HDR_VERSION)
		{
			printf("Wrong HEADER_CHKSUM_VERSION %08X(uboot) %08X(root-fs).\r\n",NVTPACK_CHKSUM_HDR_VERSION,pHdr->uiVersion);
			return -1;
		}
		UINT32 uiLen = pHdr->uiDataOffset + pHdr->uiDataSize + pHdr->uiPaddingSize;
		if(MemCheck_CalcCheckSum16Bit(uiContextBuf,uiLen)!=0)
		{
			printf("pat%d, res check sum fail.\r\n",pat_id);
			return -1;
		}
	}
	else
	{
		printf("root-fs has no CKSM header\r\n");
		return -1;
	}
#endif /* CONFIG_NVT_BIN_CHKSUM_SUPPORT */
	return 0;
}

int nvt_chk_all_in_one_valid(unsigned short EmbType, unsigned int addr, unsigned int size, unsigned int id)
{
	switch(EmbType)
	{
		case EMBTYPE_LOADER:
			if(nvt_chk_loader((ulong)addr,size,id)!=0)
				return -1;
			break;
		case EMBTYPE_MODELEXT:
			if(nvt_chk_modelext((ulong)addr,size,id)!=0)
				return -1;
			break;
		case EMBTYPE_UITRON:
			if(nvt_chk_uitron((ulong)addr,size,id)!=0)
				return -1;
			break;
		case EMBTYPE_ECOS:
			if(nvt_chk_ecos((ulong)addr,size,id)!=0)
				return -1;
			break;
		case EMBTYPE_UBOOT:
			if(nvt_chk_uboot((ulong)addr,size,id)!=0)
				return -1;
			break;
		case EMBTYPE_DSP:
			if(nvt_chk_dsp((ulong)addr,size,id)!=0)
				return -1;
			break;
		case EMBTYPE_LINUX:
			if(nvt_chk_linux((ulong)addr,size,id)!=0)
				return -1;
			break;
		case EMBTYPE_ROOTFS:
			if(nvt_chk_rootfs((ulong)addr,size,id)!=0)
				return -1;
			break;
		default:
			break;
	}

	return 0;
}

static int nvt_on_partition_enum_sanity(unsigned int id, NVTPACK_MEM* p_mem, void* p_user_data)
{
	int ret = 0;
	#if defined(CONFIG_NVT_LINUX_SPINAND_BOOT) || defined(CONFIG_NVT_LINUX_SPINOR_BOOT) || defined(CONFIG_NVT_LINUX_EMMC_BOOT)
	EMB_PARTITION* pEmb = (EMB_PARTITION*)p_user_data;
	#if defined(CONFIG_NVT_LINUX_SPINAND_BOOT)
	nand_info_t *nand = &nand_info[0];
	unsigned int PartitionSize = pEmb[id].PartitionSize * nand->erasesize;
	#elif defined(CONFIG_NVT_IVOT_EMMC)
	unsigned int PartitionSize = pEmb[id].PartitionSize * MMC_MAX_BLOCK_LEN;
	#elif defined(CONFIG_NVT_LINUX_SPINOR_BOOT)
	unsigned int PartitionSize = pEmb[id].PartitionSize * board_init_spi_flash->erase_size;
	#endif
	unsigned int size = p_mem->len;


	if (pEmb[id].EmbType == EMBTYPE_UNKNOWN) {
		// user just put data in all-in-one without any partition
		return 0;
	}

	#ifdef CONFIG_NVT_BIN_CHKSUM_SUPPORT
	// Skip rootfs nvt head info
	if (pEmb[id].EmbType == EMBTYPE_ROOTFS) {
		size -= 64;
	}
	#endif
	//check partition size
	if(size > PartitionSize)
	{
		printf("Partition[%d] Size is too smaller than that you wanna update.(0x%08X > 0x%08X)\r\n"
			,id
			,size
			,PartitionSize);
		return -1;
	}

	ret = nvt_chk_all_in_one_valid(pEmb[id].EmbType, (unsigned int)p_mem->p_data, p_mem->len, id);
	#endif /* CONFIG_NVT_LINUX_SPINAND_BOOT || CONFIG_NVT_LINUX_SPINOR_BOOT || CONFIG_NVT_LINUX_EMMC_BOOT */
	return ret;
}

static int nvt_on_partition_enum_update(unsigned int id, NVTPACK_MEM* p_mem, void* p_user_data)
{
	#if defined(CONFIG_NVT_LINUX_SPINAND_BOOT) || defined(CONFIG_NVT_LINUX_SPINOR_BOOT) || defined(CONFIG_NVT_LINUX_EMMC_BOOT)
	EMB_PARTITION* pEmb = (EMB_PARTITION*)p_user_data;
	#if defined(CONFIG_NVT_LINUX_SPINAND_BOOT)
	nand_info_t *nand = &nand_info[0];
	unsigned int PartitionOffset = pEmb[id].PartitionOffset * nand->erasesize;
	unsigned int PartitionSize = pEmb[id].PartitionSize * nand->erasesize;
	#elif defined(CONFIG_NVT_IVOT_EMMC)
	/* Block unit */
	unsigned int PartitionOffset = pEmb[id].PartitionOffset;
	unsigned int PartitionSize = pEmb[id].PartitionSize;
	#elif defined(CONFIG_NVT_LINUX_SPINOR_BOOT)
	unsigned int PartitionOffset = pEmb[id].PartitionOffset * board_init_spi_flash->erase_size;
	unsigned int PartitionSize = pEmb[id].PartitionSize * board_init_spi_flash->erase_size;
	#endif

	switch(pEmb[id].EmbType)
	{
		case EMBTYPE_MBR:
			printf("%s Update: MBR(EMMC only) %s\r\n", ANSI_COLOR_YELLOW, ANSI_COLOR_RESET);
			nvt_chk_last_ebr((ulong)p_mem->p_data, PartitionOffset, pEmb);
			nvt_update_partitions((ulong)p_mem->p_data, p_mem->len, PartitionOffset, PartitionSize);
			break;
		case EMBTYPE_LOADER:
			printf("%s Update: loader %s\r\n", ANSI_COLOR_YELLOW, ANSI_COLOR_RESET);
			nvt_update_partitions((ulong)p_mem->p_data, p_mem->len, PartitionOffset, PartitionSize);
			break;
		case EMBTYPE_MODELEXT:
			printf("%s Update: modelext %s\r\n", ANSI_COLOR_YELLOW, ANSI_COLOR_RESET);
			nvt_update_partitions((ulong)p_mem->p_data, p_mem->len, PartitionOffset, PartitionSize);
			break;
		case EMBTYPE_UITRON:
			printf("%s Update: uitron %s\r\n", ANSI_COLOR_YELLOW, ANSI_COLOR_RESET);
			nvt_update_partitions((ulong)p_mem->p_data, p_mem->len, PartitionOffset, PartitionSize);
			break;
		case EMBTYPE_ECOS:
			printf("%s Update: ecos %s\r\n", ANSI_COLOR_YELLOW, ANSI_COLOR_RESET);
			nvt_update_partitions((ulong)p_mem->p_data, p_mem->len, PartitionOffset, PartitionSize);
			break;
		case EMBTYPE_UBOOT:
			printf("%s Update: uboot %s\r\n", ANSI_COLOR_YELLOW, ANSI_COLOR_RESET);
			nvt_update_partitions((ulong)p_mem->p_data, p_mem->len, PartitionOffset, PartitionSize);
			break;
		case EMBTYPE_DSP:
			printf("%s Update: dsp %s\r\n", ANSI_COLOR_YELLOW, ANSI_COLOR_RESET);
			nvt_update_partitions((ulong)p_mem->p_data, p_mem->len, PartitionOffset, PartitionSize);
			break;
		case EMBTYPE_LINUX:
			printf("%s Update: linux %s\r\n", ANSI_COLOR_YELLOW, ANSI_COLOR_RESET);
			nvt_update_partitions((ulong)p_mem->p_data, p_mem->len, PartitionOffset, PartitionSize);
			break;
		case EMBTYPE_ROOTFS:
			printf("%s Update: rootfs%u %s\r\n", ANSI_COLOR_YELLOW, pEmb[id].OrderIdx, ANSI_COLOR_RESET);
			nvt_update_rootfs_partition((ulong)p_mem->p_data, p_mem->len, PartitionOffset, PartitionSize, &pEmb[id]);
			break;
		case EMBTYPE_USER0:
		case EMBTYPE_USER1:
		case EMBTYPE_USER2:
			printf("%s Update: user %s\r\n", ANSI_COLOR_YELLOW, ANSI_COLOR_RESET);
			nvt_update_partitions((ulong)p_mem->p_data, p_mem->len, PartitionOffset, PartitionSize);
			break;
		default:
			printf("%s Update: skip partition %d, embtype=%d %s\r\n", ANSI_COLOR_YELLOW, id, pEmb[id].EmbType, ANSI_COLOR_RESET);
			break;
	}

	#endif /* CONFIG_NVT_LINUX_SPINAND_BOOT || CONFIG_NVT_LINUX_SPINOR_BOOT || CONFIG_NVT_LINUX_EMMC_BOOT */
	return 0;
}

static int nvt_on_partition_enum_mtd_readback(unsigned int id, NVTPACK_MEM* p_mem, void* p_user_data)
{
	int ret = 0;
	#if defined(CONFIG_NVT_LINUX_SPINAND_BOOT) || defined(CONFIG_NVT_LINUX_SPINOR_BOOT) || defined(CONFIG_NVT_LINUX_EMMC_BOOT)
	EMB_PARTITION* pEmb = (EMB_PARTITION*)p_user_data;
	#if defined(CONFIG_NVT_LINUX_SPINAND_BOOT)
	nand_info_t *nand = &nand_info[0];
	unsigned int PartitionOffset = pEmb[id].PartitionOffset * nand->erasesize;
	unsigned int PartitionSize = pEmb[id].PartitionSize * nand->erasesize;
	#elif defined(CONFIG_NVT_IVOT_EMMC)
	unsigned int PartitionOffset = pEmb[id].PartitionOffset;
	unsigned int PartitionSize = pEmb[id].PartitionSize;
	#elif defined(CONFIG_NVT_LINUX_SPINOR_BOOT)
	unsigned int PartitionOffset = pEmb[id].PartitionOffset * board_init_spi_flash->erase_size;
	unsigned int PartitionSize = pEmb[id].PartitionSize * board_init_spi_flash->erase_size;
	#endif

	switch(pEmb[id].EmbType)
	{
		case EMBTYPE_MBR:
			printf("%s Read: MBR %s\r\n", ANSI_COLOR_YELLOW, ANSI_COLOR_RESET);
			ret = nvt_read_partitions((ulong)p_mem->p_data, p_mem->len, PartitionOffset, id, pEmb[id].EmbType);
			break;
		case EMBTYPE_LOADER:
			printf("%s Read: loader %s\r\n", ANSI_COLOR_YELLOW, ANSI_COLOR_RESET);
			ret = nvt_read_partitions((ulong)p_mem->p_data, p_mem->len, PartitionOffset, id, pEmb[id].EmbType);
			break;
		case EMBTYPE_MODELEXT:
			printf("%s Read: modelext %s\r\n", ANSI_COLOR_YELLOW, ANSI_COLOR_RESET);
			ret = nvt_read_partitions((ulong)p_mem->p_data, p_mem->len, PartitionOffset, id, pEmb[id].EmbType);
			break;
		case EMBTYPE_UITRON:
			printf("%s Read: uitron %s\r\n", ANSI_COLOR_YELLOW, ANSI_COLOR_RESET);
			ret = nvt_read_partitions((ulong)p_mem->p_data, p_mem->len, PartitionOffset, id, pEmb[id].EmbType);
			break;
		case EMBTYPE_ECOS:
			printf("%s Read: ecos %s\r\n", ANSI_COLOR_YELLOW, ANSI_COLOR_RESET);
			ret = nvt_read_partitions((ulong)p_mem->p_data, p_mem->len, PartitionOffset, id, pEmb[id].EmbType);
			break;
		case EMBTYPE_UBOOT:
			printf("%s Read: uboot %s\r\n", ANSI_COLOR_YELLOW, ANSI_COLOR_RESET);
			ret = nvt_read_partitions((ulong)p_mem->p_data, p_mem->len, PartitionOffset, id, pEmb[id].EmbType);
			break;
		case EMBTYPE_DSP:
			printf("%s Read: dsp %s\r\n", ANSI_COLOR_YELLOW, ANSI_COLOR_RESET);
			ret = nvt_read_partitions((ulong)p_mem->p_data, p_mem->len, PartitionOffset, id, pEmb[id].EmbType);
			break;
		case EMBTYPE_LINUX:
			printf("%s Read: linux %s\r\n", ANSI_COLOR_YELLOW, ANSI_COLOR_RESET);
			ret = nvt_read_partitions((ulong)p_mem->p_data, p_mem->len, PartitionOffset, id, pEmb[id].EmbType);
			break;
		case EMBTYPE_ROOTFS:
			printf("%s Read: rootfs%u %s\r\n", ANSI_COLOR_YELLOW, pEmb[id].OrderIdx, ANSI_COLOR_RESET);
			ret = nvt_read_rootfs_partition((ulong)p_mem->p_data, p_mem->len, PartitionOffset, PartitionSize, id, &pEmb[id]);
			break;
		case EMBTYPE_USER0:
		case EMBTYPE_USER1:
		case EMBTYPE_USER2:
			printf("%s Read: user %s\r\n", ANSI_COLOR_YELLOW, ANSI_COLOR_RESET);
			ret = nvt_read_partitions((ulong)p_mem->p_data, p_mem->len, PartitionOffset, id, pEmb[id].EmbType);
			break;
		default:
			printf("%s Read: skip partition %d, embtype=%d %s\r\n", ANSI_COLOR_YELLOW, id, pEmb[id].EmbType, ANSI_COLOR_RESET);
			break;
	}

	#endif /* CONFIG_NVT_LINUX_SPINAND_BOOT || CONFIG_NVT_LINUX_SPINOR_BOOT || CONFIG_NVT_LINUX_EMMC_BOOT */
	return ret;
}

int nvt_check_should_rename_updfw(void)
{
	static int should_rename = -1;
        BININFO *p_bininfo;

	if(should_rename != -1)
		return should_rename;

	p_bininfo = (BININFO*)modelext_get_cfg((unsigned char*)CONFIG_MODELEXT_SDRAM_BASE, MODELEXT_TYPE_BIN_INFO, NULL);
	if(!p_bininfo){
		printf("can't get right bin info in %s \n", __func__);
		return 0;
	}

	should_rename = (p_bininfo->comm.Resv[COMM_RESV_IDX_OTA_NAME_ADDR] != 0);

	return should_rename;
}

int nvt_rename_updfw(int reverse)
{
	static char    ldr_fw_name[32] = { '\0' };
	static char    ota_fw_name[32] = { '\0' };
	char           *temp;
	BININFO        *p_bininfo;
	char           cmd[128];
	int            ret;

	if(strlen(ldr_fw_name) == 0){
		temp = get_nvt_bin_name(NVT_BIN_NAME_TYPE_FW);
		if(!temp){
			printf("fail to load full loader firmware name from bininfo\r\n");
			return -1;
		}
		else{
			strncpy(ldr_fw_name, temp, sizeof(ldr_fw_name));
			ldr_fw_name[sizeof(ldr_fw_name) - 1] = '\0';
		}
	}

	if(strlen(ota_fw_name) == 0){
		p_bininfo = (BININFO*)modelext_get_cfg((unsigned char*)CONFIG_MODELEXT_SDRAM_BASE, MODELEXT_TYPE_BIN_INFO, NULL);
		if(!p_bininfo){
			printf("can't get right bin info in %s \n", __func__);
			return -1;
		}
		if(!p_bininfo->comm.Resv[COMM_RESV_IDX_OTA_NAME_ADDR]){
			printf("COMM_RESV_IDX_OTA_NAME_ADDR is 0. can't get oat firmware name\r\n");
			return -1;
		}

		strncpy(ota_fw_name, (const char *)p_bininfo->comm.Resv[COMM_RESV_IDX_OTA_NAME_ADDR], sizeof(ota_fw_name));
		ota_fw_name[sizeof(ota_fw_name) - 1] = '\0';
	}

	if(reverse)//firmware update firmware : sucessful
		ret = snprintf(cmd, sizeof(cmd), "fatrename mmc 0:1 %s %s", ldr_fw_name, ota_fw_name);
	else//firmware update firmware : begin
		ret = snprintf(cmd, sizeof(cmd), "fatrename mmc 0:1 %s %s", ota_fw_name, ldr_fw_name);

	if(ret < 0 || ret == sizeof(cmd)){
		printf("fatrename command buffer overflow, max=%d, current=%d\r\n", sizeof(cmd), ret);
		return -1;
	}

	run_command(cmd, 0);
	return 0;
}

int nvt_create_delfw()
{
	run_command("fatwrite mmc 0:1 0 NVTDELFW 0", 0);
	return 0;
}

int nvt_delete_delfw()
{
	run_command("fatrm mmc 0:1 NVTDELFW", 0);
	return 0;
}

int nvt_process_all_in_one(ulong addr, unsigned int size, int firm_upd_firm)
{
	u32 i;
	int ret = 0;
	unsigned char md5_output_orig[16] = {0};
	unsigned char md5_output_cur[16] = {0};
	u32 need_chk_all_partition_exist = 0;
	NVTPACK_VERIFY_OUTPUT np_verify = {0};
	NVTPACK_GET_PARTITION_INPUT np_get_input;
	NVTPACK_ENUM_PARTITION_INPUT np_enum_input;
	NVTPACK_MEM mem_in = {(void*)addr, (unsigned int)size};
	NVTPACK_MEM mem_out = {0};

	memset(&np_get_input, 0, sizeof(np_get_input));
	memset(&np_enum_input, 0, sizeof(np_enum_input));

	if(nvtpack_verify(&mem_in, &np_verify) != NVTPACK_ER_SUCCESS)
	{
		printf("verify failed.\r\n");
		return -1;
	}
	if(np_verify.ver != NVTPACK_VER_16072017)
	{
		printf("wrong all-in-one bin version\r\n");
		return -1;
	}

	if(firm_upd_firm && nvt_check_should_rename_updfw())
	{
		if(nvt_rename_updfw(0))
		{
			printf("fail to rename fw update fw\r\n");
			return -1;
		}
		if(nvt_create_delfw())
		{
			printf("fail to create delete file\r\n");
			return -1;
		}
	}

	//check if modelext exists.
	EMB_PARTITION* pEmbNew = NULL;
	u32 modex_ext_size = 0;
	np_get_input.id = 1; // modelext must always put in partition[1]
	np_get_input.mem = mem_in;
	if(nvtpack_get_partition(&np_get_input,&mem_out) == NVTPACK_ER_SUCCESS)
	{
		if(strncmp((char*)((unsigned char*)mem_out.p_data+sizeof(MODELEXT_HEADER)),"MODELEXT",8)!=0)
		{
			printf("partition[1] is not modelext.\r\n");
			return -1;
		}

		//extract new partition from new modelext
		MODELEXT_HEADER* p_resource_new = NULL;
		pEmbNew = (EMB_PARTITION*)modelext_get_cfg((unsigned char*)mem_out.p_data,MODELEXT_TYPE_EMB_PARTITION,&p_resource_new);
		if(pEmbNew==NULL)
		{
			printf("failed to get new partition resource.\r\n");
			return -1;
		}

		MODELEXT_INFO* p_modelext_data = (MODELEXT_INFO*)modelext_get_cfg((unsigned char*)mem_out.p_data,MODELEXT_TYPE_INFO,&p_resource_new);
		if(p_modelext_data==NULL)
		{
			printf("failed to get modelext_data.\r\n");
			return -1;
		}
		modex_ext_size = p_modelext_data->ext_bin_length;
	}

	//get current partition in embbed storage
	EMB_PARTITION* pEmbCurr = NULL;
	if (nvt_chk_flash_modelext_is_null())
	{
		printf("%s modelext is empty %s\r\n", ANSI_COLOR_RED, ANSI_COLOR_RESET);
		need_chk_all_partition_exist = 1;
	}
	else if(strncmp((char*)(CONFIG_MODELEXT_SDRAM_BASE+sizeof(MODELEXT_HEADER)),"MODELEXT",8)==0)
	{
		//To do current modelext init.
		ret = nvt_modelext_init();
		if (ret < 0)
			printf("modelext init fail\n");

		MODELEXT_HEADER* p_resource_curr = NULL;
		pEmbCurr = (EMB_PARTITION*)modelext_get_cfg((unsigned char*)CONFIG_MODELEXT_SDRAM_BASE,MODELEXT_TYPE_EMB_PARTITION,&p_resource_curr);
		if(pEmbCurr==NULL)
		{
			printf("failed to get current partition resource.\r\n");
			return -1;
		}
		//check if partition changes.
		if(pEmbNew!=NULL && memcmp(pEmbCurr,pEmbNew,sizeof(EMB_PARTITION)*EMB_PARTITION_INFO_COUNT)!=0)
		{
			//must be fully all-in-one-bin
			printf("%s detected partition changed. %s\r\n", ANSI_COLOR_RED, ANSI_COLOR_RESET);
			need_chk_all_partition_exist = 1;
		}
	}
	else
	{
		if(pEmbNew == NULL) //also pEmbCurr is NULL
		{
			printf("%s modelext doesn't exist in neither embedded nor all-in-one %s\r\n", ANSI_COLOR_RED, ANSI_COLOR_RESET);
			return -1;
		}
		else
		{
			//the first burn, must be fully all-in-one-bin
			printf("%s detected partition changed. %s\r\n", ANSI_COLOR_RED, ANSI_COLOR_RESET);
			need_chk_all_partition_exist = 1;
		}
	}

	//check all-in-one firmware sanity
	EMB_PARTITION* pEmb = (pEmbNew!=NULL)? pEmbNew : pEmbCurr;
	np_enum_input.mem = mem_in;
	np_enum_input.p_user_data = pEmb;
	np_enum_input.fp_enum = nvt_on_partition_enum_sanity;

	if(nvtpack_enum_partition(&np_enum_input) != NVTPACK_ER_SUCCESS)
	{
		printf("failed sanity.\r\n");
		return -1;
	}

	if(need_chk_all_partition_exist)
	{
		for(i=1; i<EMB_PARTITION_INFO_COUNT; i++)
		{
			if(pEmbNew[i].PartitionSize!=0 && (pEmbNew[i].OrderIdx==0 || pEmbNew[i].EmbType==EMBTYPE_MBR))
			{
				switch(pEmbNew[i].EmbType)
				{
					case EMBTYPE_MBR:
					case EMBTYPE_MODELEXT:
					case EMBTYPE_UITRON:
					case EMBTYPE_ECOS:
					case EMBTYPE_UBOOT:
					//case EMBTYPE_DSP:
					case EMBTYPE_LINUX:
					case EMBTYPE_ROOTFS:
					{
						//check this type exist in all-in-one
						np_get_input.id = i;
						np_get_input.mem = mem_in;
						if(nvtpack_get_partition(&np_get_input,&mem_out) != NVTPACK_ER_SUCCESS)
						{
							printf("partition changed, need partition[%d]\n",i);
							return -1;
						}
					}break;
				}
			}
		}
	}

	/* partition table env init */
	ret = nvt_part_config(NULL, pEmb);
	if (ret < 0)
		return ret;

	/*
	 * start to update each partition of all-in-one
	 */

	//multi-bin
	np_enum_input.mem = mem_in;
	np_enum_input.p_user_data = pEmb;
	np_enum_input.fp_enum = nvt_on_partition_enum_update;
	if(nvtpack_enum_partition(&np_enum_input) != NVTPACK_ER_SUCCESS)
	{
		printf("failed to run nvt_update_partitions.\r\n");
		return -1;
	}

	/* To calculate original buffer the all-in-one image md5 sum */
	md5_wd((unsigned char *) addr, size, md5_output_orig, CHUNKSZ_MD5);

	/*
	 * Read back updated image from mtd to check if valid or not
	 */
	if (firm_upd_firm)
		nvt_disable_mem_protect();

	//multi-bin
	np_enum_input.mem = mem_in;
	np_enum_input.p_user_data = pEmb;
	np_enum_input.fp_enum = nvt_on_partition_enum_mtd_readback;
	if(nvtpack_enum_partition(&np_enum_input) != NVTPACK_ER_SUCCESS) {
		printf("failed to run nvt_read_partitions.\r\n");
		return -1;
	}

	//check all-in-one firmware sanity
	np_enum_input.mem = mem_in;
	np_enum_input.p_user_data = pEmb;
	np_enum_input.fp_enum = nvt_on_partition_enum_sanity;

	if(nvtpack_enum_partition(&np_enum_input) != NVTPACK_ER_SUCCESS)
	{
		printf("failed sanity.\r\n");
		ret = nvt_flash_mark_bad();
		if (ret)
			printf("flash mark bad process failed\n");
		return -1;
	} else {
		printf("%s Read back check sanity successfully. %s\r\n", ANSI_COLOR_YELLOW, ANSI_COLOR_RESET);
	}

	/* To calculate the all-in-one image md5 sum after read back */
	md5_wd((unsigned char *) addr, size, md5_output_cur, CHUNKSZ_MD5);
	if (memcmp(md5_output_orig, md5_output_cur, sizeof(md5_output_cur)) != 0) {
		printf("%s All-in-one image MD5 sum is not match %s\n", ANSI_COLOR_RED, ANSI_COLOR_RESET);
		ret = nvt_flash_mark_bad();
		if (ret)
			printf("flash mark bad process failed\n");
		return -1;
	} else {
		printf("%s All-in-one image MD5 sum is match %s\n", ANSI_COLOR_YELLOW, ANSI_COLOR_RESET);
	}

	if(modex_ext_size!=0)
	{
		int ret = 0;
		//reload new modelext but must keep loader information
		printf("Reload new modelext partition after updating\r\n");
		ret = nvt_modelext_init();
		if (ret < 0)
			printf("modelext init fail\n");
	}
	return 0;
}

/*
 * This function can be used to do rootfs format
 * Two conditions will do format:
 *				iTron cmd: key restore_rootfs
 *				Linux: uctrl usys -rootfs_broken => Send event to itron, itron can decide what to do
 */
int nvt_process_rootfs_format(void)
{
	MODELEXT_HEADER *header;
	EMB_PARTITION *p_emb_partition;
	int ret = 0;
	loff_t part_off=0, part_size=0;
	char command[128];
	unsigned long val = 0;

	memset(command, 0, sizeof(command));
	/* get partition table */
	p_emb_partition = (EMB_PARTITION *)modelext_get_cfg((unsigned char*)CONFIG_MODELEXT_SDRAM_BASE, MODELEXT_TYPE_EMB_PARTITION, &header);
	if (!p_emb_partition || !header) {
		printf("Modelext: emb_partition setting is null\n");
		goto err_fmt_rootfs;
	}

	if (header->version != EMB_PARTITION_INFO_VER) {
		printf("Modelext: intdir version not match. %08X(emb):%08X(code)\n", header->version, EMB_PARTITION_INFO_VER);
		goto err_fmt_rootfs;
	}

	/* partition table env init */
	ret = nvt_part_config(NULL, p_emb_partition);
	if (ret < 0)
		goto err_fmt_rootfs;

	/* Check if rootfs1 is existed */
	ret = get_part("rootfs1", &part_off, &part_size);
	if (ret < 0)
		goto err_fmt_rootfs;

	printf("Starting to format R/W rootfs partition\n");
	sprintf(command, "nand erase.part rootfs1");
	run_command(command, 0);

#if defined(CONFIG_NVT_UBIFS_SUPPORT)
	sprintf(command, "ubi part rootfs1");
	run_command(command, 0);

	sprintf(command, "ubi create rootfs1");
	run_command(command, 0);
#endif

	val = (nvt_readl((ulong)nvt_binfo_comm_uboot_boot_func) & ~COMM_UBOOT_BOOT_FUNC_BOOT_DONE_MASK) | COMM_UBOOT_BOOT_FUNC_BOOT_DONE;
	nvt_writel(val, (ulong)nvt_binfo_comm_uboot_boot_func);
	flush_dcache_all();
	while(1) {
		// Waiting for itron trigger reboot.
		printf(".");
		mdelay(1000);
	}

err_fmt_rootfs:
	val = (nvt_readl((ulong)nvt_binfo_comm_uboot_boot_func) & ~COMM_UBOOT_BOOT_FUNC_BOOT_DONE_MASK) | COMM_UBOOT_BOOT_FUNC_BOOT_NG;
	nvt_writel(val, (ulong)nvt_binfo_comm_uboot_boot_func);
	flush_dcache_all();
	return -1;
}

/*
 * This function can be used to do system recovery in emmc boot mode
 * Below condition will do system recovery:
 *				Loader send event
 */
int nvt_process_sys_recovery(void)
{
	int ret = 0;

#if defined(CONFIG_NVT_IVOT_EMMC)
	char dev_part_str[10];
	int size = 0;
	u32 i;
	ulong addr = 0;
	MODELEXT_HEADER* p_resource_curr = NULL;
	EMB_PARTITION* pEmbCurr = NULL;
	NVTPACK_GET_PARTITION_INPUT np_get_input;

	memset(&np_get_input, 0, sizeof(np_get_input));

	printf("%sStarting to do EMMC boot recovery %s\r\n", ANSI_COLOR_YELLOW, ANSI_COLOR_RESET);
	/* Switch to emmc bus partition 3 (cache partition) */

	sprintf(dev_part_str, "%d:3", CONFIG_NVT_IVOT_EMMC);
	if (fs_set_blk_dev("mmc", dev_part_str, FS_TYPE_FAT)) {
		return ERR_NVT_UPDATE_OPENFAILED;
	} else {
		if (!fs_exists(get_nvt_bin_name(NVT_BIN_NAME_TYPE_RECOVERY_FW))) {
			return ERR_NVT_UPDATE_NO_NEED;
		}
	}
	if (fs_set_blk_dev("mmc", dev_part_str, FS_TYPE_FAT))
		return ERR_NVT_UPDATE_OPENFAILED;
	else {
		size = fs_read(get_nvt_bin_name(NVT_BIN_NAME_TYPE_RECOVERY_FW), (ulong)CONFIG_ITRON_SDRAM_BASE, 0, 0);
		if (size <= 0) {
			printf("Read %s at 0x%x failed ret=%d\n", get_nvt_bin_name(NVT_BIN_NAME_TYPE_RECOVERY_FW), CONFIG_ITRON_SDRAM_BASE, size);
			return ERR_NVT_UPDATE_READ_FAILED;
		} else {
			printf("Read %s at 0x%x successfully, size=%d\n", get_nvt_bin_name(NVT_BIN_NAME_TYPE_RECOVERY_FW), CONFIG_ITRON_SDRAM_BASE, size);
			NVTPACK_MEM mem_in = {(void*)CONFIG_ITRON_SDRAM_BASE, (unsigned int)size};
			NVTPACK_MEM mem_out = {0};
			pEmbCurr = (EMB_PARTITION*)modelext_get_cfg((unsigned char*)CONFIG_MODELEXT_SDRAM_BASE,MODELEXT_TYPE_EMB_PARTITION,&p_resource_curr);
			if(pEmbCurr==NULL) {
				printf("failed to get current partition resource.\r\n");
				return -1;
			}
			for(i=1; i<EMB_PARTITION_INFO_COUNT; i++)
			{
				if(pEmbCurr[i].PartitionSize!=0 && (pEmbCurr[i].OrderIdx==0 || pEmbCurr[i].EmbType==EMBTYPE_MBR || pEmbCurr[i].EmbType==EMBTYPE_DSP))
				{
					switch(pEmbCurr[i].EmbType)
					{
						case EMBTYPE_MBR:
						case EMBTYPE_MODELEXT:
						case EMBTYPE_UITRON:
						case EMBTYPE_UBOOT:
						#if defined(CONFIG_DSP1_FREERTOS) || defined(CONFIG_DSP2_FREERTOS)
						case EMBTYPE_DSP:
						#endif
						case EMBTYPE_LINUX:
						case EMBTYPE_ROOTFS:
						{
							//check this type exist in all-in-one
							np_get_input.id = i;
							np_get_input.mem = mem_in;
							if(nvtpack_get_partition(&np_get_input,&mem_out) != NVTPACK_ER_SUCCESS)
							{
								printf("Recovery procedure: need partition[%d]\n",i);
								return -1;
							}
						}break;
					}
				}
			}
			ret = nvt_process_all_in_one((ulong)CONFIG_ITRON_SDRAM_BASE ,size, 0);
			if (ret < 0)
				return ERR_NVT_UPDATE_FAILED;
		}
	}
#endif

	return ret;
}

int nvt_fw_update(bool firm_upd_firm)
{
	int size = 0, ret = 0, retry = 0;
	BININFO *p_bininfo = NULL;
	MODELEXT_HEADER* p_resource = NULL;

	p_bininfo = (BININFO*)modelext_get_cfg((unsigned char*)CONFIG_MODELEXT_SDRAM_BASE, MODELEXT_TYPE_BIN_INFO, &p_resource);
	if (p_bininfo==NULL) {
		printf("can't get right bin info in %s \n", __func__);
		return ERR_NVT_UPDATE_FAILED;
	}

	if (firm_upd_firm) {
		unsigned long firm_addr = nvt_readl((ulong)nvt_binfo_comm_fw_update_addr);
		unsigned long firm_size = nvt_readl((ulong)nvt_binfo_comm_fw_update_len);

		p_bininfo->ld.LdCtrl2 |= LDCF_UPDATE_FW;
		flush_dcache_range((u32)p_bininfo, (u32)p_bininfo + sizeof(BININFO));

		printf("%s\tfirmware image at: 0x%08lx@0x%08lx %s\r\n", ANSI_COLOR_YELLOW, firm_size, firm_addr, ANSI_COLOR_RESET);
		ret = nvt_process_all_in_one(firm_addr, firm_size, firm_upd_firm);
		if (ret < 0)
			return ERR_NVT_UPDATE_FAILED;
	} else if ( p_bininfo->ld.LdCtrl2 & LDCF_BOOT_FLASH ) {
		return ERR_NVT_UPDATE_NO_NEED;
	} else {
		if (nvt_fs_set_blk_dev())
			return ERR_NVT_UPDATE_OPENFAILED;
		else {
			if (!fs_exists(get_nvt_bin_name(NVT_BIN_NAME_TYPE_FW))) {
				return ERR_NVT_UPDATE_NO_NEED;
			}
		}

		while (retry < UPDATE_ALL_IN_ONE_RETRY_TIMES) {
		if (nvt_fs_set_blk_dev())
			return ERR_NVT_UPDATE_OPENFAILED;
		else {
			size = fs_read(get_nvt_bin_name(NVT_BIN_NAME_TYPE_FW), (ulong)CONFIG_ITRON_SDRAM_BASE, 0, 0);
			if (size <= 0) {
				printf("Read %s at 0x%x failed ret=%d\n", get_nvt_bin_name(NVT_BIN_NAME_TYPE_FW), CONFIG_ITRON_SDRAM_BASE, size);
				return ERR_NVT_UPDATE_READ_FAILED;
			} else {
				printf("Read %s at 0x%x successfully, size=%d\n", get_nvt_bin_name(NVT_BIN_NAME_TYPE_FW), CONFIG_ITRON_SDRAM_BASE, size);
				p_bininfo->ld.LdCtrl2 |= LDCF_UPDATE_FW;
				flush_dcache_range((u32)p_bininfo, (u32)p_bininfo + sizeof(BININFO));
				ret = nvt_process_all_in_one((ulong)CONFIG_ITRON_SDRAM_BASE ,size, 0);
				if (ret < 0) {
					retry++;
				} else {
					/* This flag is for the first boot after update A.bin finished. */
					p_bininfo->ld.LdCtrl2 &= ~LDCF_BOOT_CARD;
					break;
				}
			}
		}
		if (retry >= UPDATE_ALL_IN_ONE_RETRY_TIMES)
			return ERR_NVT_UPDATE_FAILED;
		}
	}

	return 0;
}
