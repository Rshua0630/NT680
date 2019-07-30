/*
 *  board/novatek/evb668/nvtmbr.c
 *
 *  Author:	Alvin lin
 *  Created:	May 20, 2013
 *  Copyright:	Novatek Inc.
 *
 */
#include <asm-generic/errno.h>
#include "nvtmbr.h"

static struct dos_part_entry empty_part_entry;

unsigned long nvt_get_disk_total_sectors(void)
{
	struct mmc *mmc = find_mmc_device(CONFIG_SYS_MMC_SYS_DEV);

	mmc_init(mmc);

	return ((mmc->capacity) >> BYTE_TO_SECTOR_SFT);
}

char* nvt_ptype_to_str(unsigned char ptype)
{
	static char str[16];

	memset(str,0,sizeof(str));
	switch(ptype) {
		case PTYPE_EMPTY:
			strcpy(str,"PTYPE_EMPTY");
			break;

		case PTYPE_FAT12:
			strcpy(str,"PTYPE_FAT12");
			break;

		case PTYPE_FAT16:
			strcpy(str,"PTYPE_FAT16");
			break;

		case PTYPE_MSEXT_CHS:
			strcpy(str,"PTYPE_MSEXT_CHS");
			break;

		case PTYPE_FAT16L:
			strcpy(str,"PTYPE_FAT16L");
			break;

		case PTYPE_NTFS:
			strcpy(str,"PTYPE_NTFS");
			break;

		case PTYPE_FAT32_CHS:
			strcpy(str,"PTYPE_FAT32_CHS");
			break;

		case PTYPE_FAT32_LBA:
			strcpy(str,"PTYPE_FAT32_LBA");
			break;

		case PTYPE_FAT16_LBA:
			strcpy(str,"PTYPE_FAT16_LBA");
			break;

		case PTYPE_MSEXT_LBA:
			strcpy(str,"PTYPE_MSEXT_LBA");
			break;

		case PTYPE_HFAT12_CHS:
			strcpy(str,"PTYPE_HFAT12_CHS");
			break;

		case PTYPE_HFAT16_LCHS:
			strcpy(str,"PTYPE_HFAT16_LCHS");
			break;

		case PTYPE_HFAT16_MCHS:
			strcpy(str,"PTYPE_HFAT16_MCHS");
			break;

		case PTYPE_HFAT32_CHS:
			strcpy(str,"PTYPE_HFAT32_CHS");
			break;

		case PTYPE_HFAT32_LBA:
			strcpy(str,"PTYPE_HFAT32_LBA");
			break;

		case PTYPE_HFAT16_LBA:
			strcpy(str,"PTYPE_HFAT16_LBA");
			break;

		case PTYPE_MSMBR:
			strcpy(str,"PTYPE_MSMBR");
			break;

		case PTYPE_LINUX_SWAP:
			strcpy(str,"PTYPE_LINUX_SWAP");
			break;

		case PTYPE_LINUX_FS:
			strcpy(str,"PTYPE_LINUX_FS");
			break;

		case PTYPE_HIBERNATION1:
			strcpy(str,"PTYPE_HIBERNATION1");
			break;

		case PTYPE_LINUX_EXT:
			strcpy(str,"PTYPE_LINUX_EXT");
			break;

		case PTYPE_NTFS_VOL1:
			strcpy(str,"PTYPE_NTFS_VOL1");
			break;

		case PTYPE_NTFS_VOL2:
			strcpy(str,"PTYPE_NTFS_VOL2");
			break;

		case PTYPE_HIBERNATION2:
			strcpy(str,"PTYPE_HIBERNATION2");
			break;

		case PTYPE_HIBERNATION3:
			strcpy(str,"PTYPE_HIBERNATION3");
			break;

		case PTYPE_FREEBSD:
			strcpy(str,"PTYPE_FREEBSD");
			break;

		case PTYPE_OPENBSD:
			strcpy(str,"PTYPE_OPENBSD");
			break;

		case PTYPE_MAC_OSX:
			strcpy(str,"PTYPE_MAC_OSX");
			break;

		case PTYEP_NETBSD:
			strcpy(str,"PTYEP_NETBSD");
			break;

		case PTYPE_MAC_OSX_BOOT:
			strcpy(str,"PTYPE_MAC_OSX_BOOT");
			break;

		case PTYPE_BSDI:
			strcpy(str,"PTYPE_BSDI");
			break;

		case PTYPE_BSDI_SWAP:
			strcpy(str,"PTYPE_BSDI_SWAP");
			break;

		case PTYPE_EFI_GPT:
			strcpy(str,"PTYPE_EFI_GPT");
			break;

		case PTYPE_EFI_SYSPART:
			strcpy(str,"PTYPE_EFI_SYSPART");
			break;

		case PTYPE_VM_FS:
			strcpy(str,"PTYPE_VM_FS");
			break;

		case PTYPE_VM_SWAP:
			strcpy(str,"PTYPE_VM_SWAP");
			break;
	}

	return str;
}

int nvt_parse_dos_part_table(struct nvt_dos_partition *ptable)
{
	int ret = 0;
	struct nvt_dos_partition *tmp_table = ptable;
	struct mbr tmp_mbr __attribute__((aligned(64)));
	struct mbr_lite tmp_lite;
	unsigned int primary_offset = 0;
	unsigned int curr_offset = 0;
	int i,tbl_cnt = 0,no_more_mbr = 1;
	char cmd[64] = {0};


	if(ptable == NULL) {
		printf("Error ! Table is null\n");
		ret = -EINVAL; 
		goto out;
	}

	do {
		//read mbr entry
		memset(cmd, 0, sizeof(cmd));
		sprintf(cmd, "mmc read 0x%x 0x%x 0x1", (unsigned int)(&tmp_mbr), curr_offset);
		ret = run_command(cmd, 0);
		if(ret < 0) {
			printf("read mbr entry error !\n");
			goto out;
		}

		memcpy(&tmp_lite, tmp_mbr.part_entrys, sizeof(struct mbr_lite));

		//check signature
		if(tmp_lite.signature[0] != 0x55 && tmp_lite.signature[1] != 0xaa) {
			printf("Invalid mbr entry !\n");
			ret = -EIO;
			goto out;
		}

		//get partition entrys
		for(i=0;i < MAX_PART_ENTRY_PER_MBR;i++) {
			if(memcmp(&(tmp_lite.part_entrys[i]),&empty_part_entry,sizeof(empty_part_entry)) == 0)
				break;
			else {
				strcpy(tmp_table->fs_str,nvt_ptype_to_str(tmp_lite.part_entrys[i].part_type));
				tmp_table->fs_type = tmp_lite.part_entrys[i].part_type;

				//if type is extend partition, add primary offset
				//calculate start sector
				if(tmp_lite.part_entrys[i].part_type == PTYPE_MSEXT_CHS || 
						tmp_lite.part_entrys[i].part_type == PTYPE_MSEXT_LBA) {
					tmp_table->start_sector = tmp_lite.part_entrys[i].start_lba + primary_offset;
					no_more_mbr = 0;
				}else
					tmp_table->start_sector = tmp_lite.part_entrys[i].start_lba + curr_offset;

				tmp_table->ori_start = tmp_lite.part_entrys[i].start_lba;

				//calculate end sector
				tmp_table->end_sector = tmp_table->start_sector + 
					tmp_lite.part_entrys[i].size_in_sector - 1;

				//save length 
				tmp_table->length = tmp_lite.part_entrys[i].size_in_sector;

				tmp_table++;
				tbl_cnt++;

				if(tbl_cnt > MAX_ALLOW_PART_NUM) {
					printf("Parrtition too much !\n");
					ret = -EINVAL;
					goto out;
				}

				//update current offset
				if(tmp_lite.part_entrys[i].part_type == PTYPE_MSEXT_CHS || 
						tmp_lite.part_entrys[i].part_type == PTYPE_MSEXT_LBA) {
					curr_offset = primary_offset + tmp_lite.part_entrys[i].start_lba;
				}

				//set primary_offset 
				if(primary_offset == 0 && (tmp_lite.part_entrys[i].part_type == PTYPE_MSEXT_CHS || 
							tmp_lite.part_entrys[i].part_type == PTYPE_MSEXT_LBA)) {
					primary_offset = tmp_lite.part_entrys[i].start_lba;
					printf("primary_offset = %d\n",primary_offset);
				}

			}
		}

		if(no_more_mbr)
			break;
		else
			no_more_mbr = 1;

	}while(1);

out:
	return ret;

}

int nvt_gen_mbr_dram_tbl(struct part_parm *pparm_tbl, struct mbr_dram *pmbr_dram_tbl)
{
	int ret = 0;
	int i,num_of_part = 0;
	unsigned long max_sectors = nvt_get_disk_total_sectors();
	struct mbr_dram *pdrm_tbl = pmbr_dram_tbl;
	struct mbr_dram *pprev_drm_tbl;
	unsigned int primary_offset;
	unsigned long total_part_sectors = 0;

	for(i=0;i < MAX_ALLOW_PART_NUM;i++) 
		if(pparm_tbl[i].sectors == 0 && pparm_tbl[i].id == 0)
			break;
		else
			total_part_sectors += pparm_tbl[i].sectors;


	//total_part_sectors += i * (MBR_FS_GAP);


	if(total_part_sectors > max_sectors) {
		printf("total partition sectors(%lu) large than disk space(%lu) ! Stop create partition table !\n",
				total_part_sectors, max_sectors);
		ret = -EINVAL;
		goto out;
	}

	num_of_part = i;

	//update the rest_all partition if exist
	if(pparm_tbl[num_of_part-1].sectors == 0) 
		pparm_tbl[num_of_part-1].sectors = (max_sectors - total_part_sectors - EXT_PART_EXTRA_TCL_NOUSE - 1); // data part left 300M 

	//create lightweight mbr entry in dram
	for(i=0;i < num_of_part;i++) {
		//partition 0~2 put in primary partition
		if(i < (MAX_PRIMARY_PART_NUM - 1)) {
			//fill signature
			pdrm_tbl->used = 1;
			pdrm_tbl->curr_offset = 0;
			pdrm_tbl->lmbr.signature[0] = 0x55;
			pdrm_tbl->lmbr.signature[1] = 0xaa;
			pdrm_tbl->lmbr.part_entrys[i].boot_flag = 0;
			//fill start chs
			pdrm_tbl->lmbr.part_entrys[i].start_chs[0] = 0xfe;
			pdrm_tbl->lmbr.part_entrys[i].start_chs[1] = 0xff;
			pdrm_tbl->lmbr.part_entrys[i].start_chs[2] = 0xff;

			//fill end chs
			pdrm_tbl->lmbr.part_entrys[i].end_chs[0] = 0xfe;
			pdrm_tbl->lmbr.part_entrys[i].end_chs[1] = 0xff;
			pdrm_tbl->lmbr.part_entrys[i].end_chs[2] = 0xff;

			//fill partition type, we always set to Liunx fs
			pdrm_tbl->lmbr.part_entrys[i].part_type = PTYPE_LINUX_FS;


			//file partition size, count in sector
			pdrm_tbl->lmbr.part_entrys[i].size_in_sector = pparm_tbl[i].sectors;

			//first primary partition, fs gap is 2048 sectors
			if(i == 0) 
				pdrm_tbl->lmbr.part_entrys[i].start_lba = MBR_FS_GAP;
			else
				pdrm_tbl->lmbr.part_entrys[i].start_lba = (pdrm_tbl->lmbr.part_entrys[i-1].start_lba + 
						pdrm_tbl->lmbr.part_entrys[i-1].size_in_sector);

		} else if(i==(MAX_PRIMARY_PART_NUM - 1)) {
			//partition 3, if this is last partition, set it as primary partition, else set it as primary extend partition
			if(num_of_part == MAX_PRIMARY_PART_NUM) {
				//this is the last partition, so set it as primary partition
				pdrm_tbl->lmbr.part_entrys[i].boot_flag = 0;

				pdrm_tbl->lmbr.part_entrys[i].start_chs[0] = 0xfe;
				pdrm_tbl->lmbr.part_entrys[i].start_chs[1] = 0xff;
				pdrm_tbl->lmbr.part_entrys[i].start_chs[2] = 0xff;

				pdrm_tbl->lmbr.part_entrys[i].end_chs[0] = 0xfe;
				pdrm_tbl->lmbr.part_entrys[i].end_chs[1] = 0xff;
				pdrm_tbl->lmbr.part_entrys[i].end_chs[2] = 0xff;

				pdrm_tbl->lmbr.part_entrys[i].part_type = PTYPE_LINUX_FS;
				pdrm_tbl->lmbr.part_entrys[i].size_in_sector = pparm_tbl[i].sectors;

				pdrm_tbl->lmbr.part_entrys[i].start_lba = (pdrm_tbl->lmbr.part_entrys[i-1].start_lba + 
						pdrm_tbl->lmbr.part_entrys[i-1].size_in_sector);
			} else {
				//this is not the last partition, so create primary extend partition
				pdrm_tbl->lmbr.part_entrys[i].boot_flag = 0;

				pdrm_tbl->lmbr.part_entrys[i].start_chs[0] = 0xfe;
				pdrm_tbl->lmbr.part_entrys[i].start_chs[1] = 0xff;
				pdrm_tbl->lmbr.part_entrys[i].start_chs[2] = 0xff;

				pdrm_tbl->lmbr.part_entrys[i].end_chs[0] = 0xfe;
				pdrm_tbl->lmbr.part_entrys[i].end_chs[1] = 0xff;
				pdrm_tbl->lmbr.part_entrys[i].end_chs[2] = 0xff;

				pdrm_tbl->lmbr.part_entrys[i].part_type = PTYPE_MSEXT_CHS;

				pdrm_tbl->lmbr.part_entrys[i].size_in_sector = (max_sectors - pdrm_tbl->lmbr.part_entrys[i-1].size_in_sector
						- pdrm_tbl->lmbr.part_entrys[i-1].start_lba);
				pdrm_tbl->lmbr.part_entrys[i].start_lba = (pdrm_tbl->lmbr.part_entrys[i-1].size_in_sector + 
						pdrm_tbl->lmbr.part_entrys[i-1].start_lba);
				primary_offset = pdrm_tbl->lmbr.part_entrys[i].start_lba;

				//create the real fs partiton
				pprev_drm_tbl = pdrm_tbl;
				pdrm_tbl++;
				pdrm_tbl->used = 1;
				pdrm_tbl->curr_offset = primary_offset;
				pdrm_tbl->lmbr.signature[0] = 0x55;
				pdrm_tbl->lmbr.signature[1] = 0xaa;

				pdrm_tbl->lmbr.part_entrys[0].boot_flag = 0;

				pdrm_tbl->lmbr.part_entrys[0].start_chs[0] = 0xfe;
				pdrm_tbl->lmbr.part_entrys[0].start_chs[1] = 0xff;
				pdrm_tbl->lmbr.part_entrys[0].start_chs[2] = 0xff;

				pdrm_tbl->lmbr.part_entrys[0].end_chs[0] = 0xfe;
				pdrm_tbl->lmbr.part_entrys[0].end_chs[1] = 0xff;
				pdrm_tbl->lmbr.part_entrys[0].end_chs[2] = 0xff;

				pdrm_tbl->lmbr.part_entrys[0].part_type = PTYPE_LINUX_FS;

				pdrm_tbl->lmbr.part_entrys[0].size_in_sector = pparm_tbl[i].sectors;
				pdrm_tbl->lmbr.part_entrys[0].start_lba = MBR_FS_GAP;
			}
		} else {
			//extend partitions...
			//create extend partiton
			pdrm_tbl->lmbr.part_entrys[1].boot_flag = 0;
			pdrm_tbl->lmbr.part_entrys[1].start_chs[0] = 0xfe;
			pdrm_tbl->lmbr.part_entrys[1].start_chs[1] = 0xff;
			pdrm_tbl->lmbr.part_entrys[1].start_chs[2] = 0xff;

			pdrm_tbl->lmbr.part_entrys[1].end_chs[0] = 0xfe;
			pdrm_tbl->lmbr.part_entrys[1].end_chs[1] = 0xff;
			pdrm_tbl->lmbr.part_entrys[1].end_chs[2] = 0xff;

			pdrm_tbl->lmbr.part_entrys[1].part_type = PTYPE_MSEXT_CHS;

			pdrm_tbl->lmbr.part_entrys[1].size_in_sector =  EXT_PART_EXTRA_GAP + pparm_tbl[i].sectors;
			pdrm_tbl->lmbr.part_entrys[1].start_lba = (pdrm_tbl->curr_offset + pdrm_tbl->lmbr.part_entrys[0].start_lba + 
				pdrm_tbl->lmbr.part_entrys[0].size_in_sector - primary_offset);


			//create the real fs partition
			pprev_drm_tbl = pdrm_tbl;
			pdrm_tbl++;
			pdrm_tbl->used = 1;
			pdrm_tbl->curr_offset = primary_offset + pprev_drm_tbl->lmbr.part_entrys[1].start_lba;
			pdrm_tbl->lmbr.signature[0] = 0x55;
			pdrm_tbl->lmbr.signature[1] = 0xaa;

			pdrm_tbl->lmbr.part_entrys[0].boot_flag = 0;

			pdrm_tbl->lmbr.part_entrys[0].start_chs[0] = 0xfe;
			pdrm_tbl->lmbr.part_entrys[0].start_chs[1] = 0xff;
			pdrm_tbl->lmbr.part_entrys[0].start_chs[2] = 0xff;

			pdrm_tbl->lmbr.part_entrys[0].end_chs[0] = 0xfe;
			pdrm_tbl->lmbr.part_entrys[0].end_chs[1] = 0xff;
			pdrm_tbl->lmbr.part_entrys[0].end_chs[2] = 0xff;

			pdrm_tbl->lmbr.part_entrys[0].part_type = PTYPE_LINUX_FS;

			pdrm_tbl->lmbr.part_entrys[0].size_in_sector =  pparm_tbl[i].sectors;
			pdrm_tbl->lmbr.part_entrys[0].start_lba = MBR_FS_GAP;

		}

	}

out:
	return ret;
}

int nvt_write_mbr_dram_tbl_to_disk(struct mbr_dram *pmbr_dram_tbl)
{
	int ret = 0;
	struct mbr_dram *pmbr_tbl = pmbr_dram_tbl;
	int i;
	char cmd[64] = {0};
	struct mbr mbr_buff __attribute__((aligned(64)));

	if(pmbr_tbl == NULL) {
		printf("dram mbr table is null !");
		ret = -EINVAL;
		goto out;
	}


	for(i=0;i < MAX_ALLOW_PART_NUM;i++) {
		if(pmbr_tbl->used == 0)
			break;

		memset(cmd, 0, sizeof(cmd));
		memset(&mbr_buff, 0, sizeof(mbr_buff));

		memcpy(&(mbr_buff.part_entrys), &(pmbr_tbl->lmbr), sizeof(struct mbr_lite));

		sprintf(cmd, "mmc write 0x%x 0x%x 1", (unsigned int)(&mbr_buff), pmbr_tbl->curr_offset);
		ret = run_command(cmd, 0);

		if(ret < 0) {
			printf("write device fail ! abort to write parition table !\n");
			ret = -EIO;
			goto out;
		}


		pmbr_tbl++;
	}



out:
	return ret;
}

int nvt_test_gen_mbr(struct part_parm *ppart_tbl, struct mbr_dram *dram_tbl)
{

	int i,j;
	int ret = 0;

	ret = nvt_gen_mbr_dram_tbl(ppart_tbl, dram_tbl);

	if(ret < 0)
		goto out;

	for(i=0;i<MAX_ALLOW_PART_NUM;i++) {
		if(dram_tbl[i].used == 0)
			break;
		for(j=0;j<MAX_PART_ENTRY_PER_MBR;j++) {
			if(dram_tbl[i].lmbr.part_entrys[j].part_type == 0)
				break;
			else
				printf("fs_type:%s start:(%d) len:%d\n",nvt_ptype_to_str(dram_tbl[i].lmbr.part_entrys[j].part_type),
						dram_tbl[i].lmbr.part_entrys[j].start_lba,dram_tbl[i].lmbr.part_entrys[j].size_in_sector);
		}

	}

out:
	return ret;
}

int nvt_dos_part_conv_fs_part(struct nvt_dos_partition *pdostbl, struct nvt_fs_partition *pfstbl)
{
	int ret = 0;
	int i,j=0;
	struct nvt_dos_partition *pdos = pdostbl;
	struct nvt_fs_partition *pfs = pfstbl;

	if(pdos == NULL || pfstbl == NULL) {
		ret = -EINVAL;
		printf("Invalid table pointer !\n");
		goto out;
	}

	for(i=0;i<MAX_ALLOW_PART_NUM;i++) {
		if(strlen(pdos->fs_str) <= 0)
			break;

		if(pdos->fs_type != PTYPE_MSEXT_CHS) {
			pfs->used = 1;
			pfs->id = j;
			pfs->start_sector = pdos->start_sector;
			pfs->end_sector = pdos->end_sector;
			pfs->length = pdos->length;
			pfs++;
			j++;
		}

		pdos++;
	}

out:
	return ret;
}

int nvt_dram_mbr_conv_fs_part(struct mbr_dram *pmbr_tbl, struct nvt_fs_partition *pfstbl)
{
	int ret = 0;
	struct mbr_dram *pmbr = pmbr_tbl;
	struct nvt_fs_partition *pfs = pfstbl;
	struct dos_part_entry empty_dos_part_entry = {0};
	int i,j,k = 0;

	if(pmbr == NULL || pfs == NULL) {
		ret = -EINVAL;
		printf("Invalid table pointer !\n");
		goto out;
	}

	for(i=0;i < MAX_ALLOW_PART_NUM;i++) {
		if(pmbr->used == 0)
			break;

		for(j=0;j < MAX_PART_ENTRY_PER_MBR;j++) {
			if(memcmp(&empty_dos_part_entry, &(pmbr->lmbr.part_entrys[j]),sizeof(struct dos_part_entry)) == 0)
				break;
			else if(pmbr->lmbr.part_entrys[j].part_type != PTYPE_MSEXT_CHS){
				pfs->used = 1;
				pfs->id = k;
				pfs->start_sector = pmbr->lmbr.part_entrys[j].start_lba + pmbr->curr_offset;
				pfs->end_sector = pmbr->lmbr.part_entrys[j].start_lba + pmbr->curr_offset + pmbr->lmbr.part_entrys[j].size_in_sector - 1;
				pfs->length = pmbr->lmbr.part_entrys[j].size_in_sector;
				pfs++;
				k++;
			}
		}

		pmbr++;
	}

out:
	return ret;
}

int nvt_list_partition(struct nvt_fs_partition *pfstbl)
{
	int i;
	int ret = 0;

	for(i=0;i<MAX_ALLOW_PART_NUM;i++) {
		if (pfstbl[i].used) {
			printf("id:%d start:%d end:%d len:%d\n",pfstbl[i].id, pfstbl[i].start_sector,
					pfstbl[i].end_sector,pfstbl[i].length);
		}
	}

	return ret;	
}

int nvt_dump_dos_part_table(struct nvt_dos_partition *pdostbl)
{
	int ret = 0;
	int i;

	ret = nvt_parse_dos_part_table(pdostbl);

	for(i=0;i<MAX_ALLOW_PART_NUM;i++) {
		if(strlen(pdostbl[i].fs_str) > 0) {
			printf("fs_type:%s  start:%d(%d) end:%d len:%d\n",pdostbl[i].fs_str,
					pdostbl[i].start_sector,pdostbl[i].ori_start,pdostbl[i].end_sector,
					pdostbl[i].length);
		}
	}


	return ret;
}

int nvt_write_fs_image(char *img_buff, unsigned int img_size, unsigned int part_id, struct nvt_fs_partition *pfstbl)
{
	int ret = 0;
	unsigned int write_blk_cnt = 0;
	char cmd[64] = {0};

	if(pfstbl == NULL || pfstbl[part_id].used == 0) {
		printf("id or fs partition table invalid !\n");
		ret = -EINVAL;
		goto out;
	}

	if(img_size > (pfstbl[part_id].length * SECTOR_IN_BYTES)) {
		printf("image size too large , abort update partition !\n");
		goto out;
	}

	//calculate image size to block count
	write_blk_cnt = (img_size % SECTOR_IN_BYTES) > 0 ? ((img_size / SECTOR_IN_BYTES) + 1) : (img_size / SECTOR_IN_BYTES);


	sprintf(cmd,"mmc write 0x%x 0x%x 0x%x", (unsigned int)img_buff, pfstbl[part_id].start_sector, write_blk_cnt);
	ret = run_command(cmd, 0);

	if(ret < 0) {
		printf("write to file system partition fail !");
		goto out;
	}


out:

	return ret;
}

