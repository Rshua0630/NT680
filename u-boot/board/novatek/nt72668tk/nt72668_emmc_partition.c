/*
 *  board/novatek/evb668/nt72668_emmc_partition.c
 *
 *  Author:	Alvin lin
 *  Created:	May 21, 2013
 *  Copyright:	Novatek Inc.
 *
 */
#include <common.h>
#include <asm/global_data.h>
#include <asm/arch/nt72668_stbc.h>
//#include <configs/evb656tk.h>
//#include <configs/nt72656tk.h>
#include <configs/nt72668tk.h>
#include "nt72668_emmc_partition.h"
#include "nt72668_emmc_fwupdate.h"
#include "nt72668_emmc_xbootdat.h"

DECLARE_GLOBAL_DATA_PTR;

static struct nvt_dos_partition nvt_part_table[MAX_ALLOW_PART_NUM];

static struct nvt_fs_partition nvt_fs_tbl[MAX_ALLOW_PART_NUM];

static struct mbr_dram nvt_mbr_dram_tbl[MAX_ALLOW_PART_NUM];

static int emmc_part_inited = 0;

#if defined(CONFIG_EVB_668)
#include "nt72668_emmc_nvt_img.c"
#elif defined (CONFIG_TCL_AU_668)
#include "nt72668_emmc_tcl_au_img.c"
#elif defined (CONFIG_TCL_CN_668)
#include "nt72668_emmc_tcl_cn_img.c"
#elif defined (CONFIG_TCL_EU_668)
#include "nt72668_emmc_tcl_eu_img.c"
#elif defined (CONFIG_N62001)
#include "nt72668_emmc_n62001_img.c"
#elif defined (CONFIG_N33002)
#include "nt72668_emmc_n33002_img.c"
#elif defined (CONFIG_EVB656)
#include "nt72656_emmc_nvt_img.c"
#elif defined (CONFIG_NT72656)
#include "nt72656_emmc_nvt_img.c"
#elif defined (CONFIG_SRI668)
#include "nt72668_emmc_sri_img.c"
#else
#error "You should define your product partition and image first !"
#endif


unsigned int nvt_emmc_get_image_cnt(void)
{
	return sizeof(image_info) / sizeof(nvt_emmc_image_info);
}

nvt_emmc_image_info* nvt_emmc_get_img_by_index(unsigned int index)
{
	int total_cnt = sizeof(image_info) / sizeof(nvt_emmc_image_info);

	if(index >= total_cnt)
		return NULL;
	else
		return (&(image_info[index]));
}

nvt_emmc_image_info* nvt_emmc_get_img_by_name(char *name)
{
	int i, total_cnt = sizeof(image_info) / sizeof(nvt_emmc_image_info);

	for(i=0;i < total_cnt;i++) {
		if(strcmp(image_info[i].image_name, name) == 0)
			break;
	}

	if(i >= total_cnt)
		return NULL;
	else
		return (&(image_info[i]));
}

int nvt_emmc_get_partition_id_by_name(char *name)
{
	int i, total_cnt = sizeof(image_info) / sizeof(nvt_emmc_image_info);
	int part_id;

	for(i=0;i < total_cnt;i++) {
		if(strcmp(image_info[i].image_name, name) == 0)
			break;
	}

	if(i >= total_cnt)
		return INVAILD_USER_PART_ID;
	else {
		part_id = image_info[i].user_part_id;

		if(part_id >=0 && part_id < 3)
			part_id += 1;
		else if(part_id >= 3)
			part_id += 2;
	}

	return part_id;

}

static int nvt_emmc_update_img_fs_part(struct nvt_fs_partition *pfstbl)
{
	int i,sz = sizeof(image_info) / sizeof(nvt_emmc_image_info);

	for(i=0;i < sz;i++) {
		if(image_info[i].user_part_id != INVAILD_USER_PART_ID)
		{
			image_info[i].pfs_part = (&(pfstbl[image_info[i].user_part_id]));

//#define PART_DEBUG
#ifdef PART_DEBUG
			struct nvt_fs_partition *pfstbl_check = image_info[i].pfs_part;

			printf("pfs infor : \n");
			printf("used: %d ", pfstbl_check->used);
			printf("id: %d ", pfstbl_check->id);
			printf("start: %d ", pfstbl_check->start_sector);
			printf("end: %d ", pfstbl_check->end_sector);
			printf("length: %d \n", pfstbl_check->length);
			if (image_info[i].size == 0)
			{
			    image_info[i].size = image_info[i].pfs_part->length;
			}
#endif
#if defined(CONFIG_N62001) || defined(CONFIG_N33002)
			if (image_info[i].size == 0)
			{
			    image_info[i].size = image_info[i].pfs_part->length;
			}
#endif //CONFIG_N62001 || CONFIG_N33002
	    }
	}


	return 0;
}

void nvt_emmc_part_init(void)
{
	int i, sz = sizeof(image_info) / sizeof(nvt_emmc_image_info);
	unsigned int raddr;
	nvt_emmc_image_info *pimg;

	if(!emmc_part_inited) {
		int ret = 0;

		ret = nvt_gen_mbr_dram_tbl(nvt_part_parm_tbl, nvt_mbr_dram_tbl);

		if(ret < 0) {
			printf("%s create dram mbr table fail !\n",__func__);
			goto out;
		}

		ret = nvt_dram_mbr_conv_fs_part(nvt_mbr_dram_tbl, nvt_fs_tbl);

		if(ret < 0) {
			printf("%s create fs table fail !\n",__func__);
			goto out;
		}

		ret = nvt_emmc_update_img_fs_part(nvt_fs_tbl);

		if(ret < 0) {
			printf("%s update image start sector fail !\n",__func__);
			goto out;
		}

		pimg = nvt_emmc_get_img_by_name("xbootdat");

		xbootdat_init(pimg->part_offset);

		emmc_part_inited = 1;
	}

out:
	return;
}

int do_nvt_emmc_part(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
	int i,ret = 0;
	if (argc < 2)
		return CMD_RET_USAGE;

	nvt_emmc_part_init();

	if(strcmp(argv[1],"info") == 0) {
		for(i=0;i < MAX_ALLOW_PART_NUM;i++) {
			if(nvt_fs_tbl[i].used == 0)
				break;
			else {
				printf("PART:%d start:%d end:%d len:%d\n",nvt_fs_tbl[i].id,
						nvt_fs_tbl[i].start_sector, nvt_fs_tbl[i].end_sector,
						nvt_fs_tbl[i].length);
			}
		}
	} else if (strcmp(argv[1],"read") == 0) {
		ret = nvt_dump_dos_part_table(nvt_part_table);
	} else if(strcmp(argv[1],"write") == 0) {
		ret = nvt_write_mbr_dram_tbl_to_disk(nvt_mbr_dram_tbl);
	} else
		ret = CMD_RET_USAGE;

	return ret;
}

U_BOOT_CMD(
	epart, 2, 1, do_nvt_emmc_part,
	"EMMC partition tool",
	"epart info - print current firmware partition setting\n"
	"epart read - read current emmc partition\n"
	"epart write - write partition table to emmc (this may destroy all data on emmc chip, make sure you know what you are doing)\n"
);

typedef struct _nvt_ram_text{
	unsigned int start_addr;
	unsigned int flen;
}nvt_ram_text;

int nvt_read_file_to_ram(char* fname, nvt_ram_text *pram_txt)
{
	int ret = 0;
	char cmd[64] = {};
	char *ptr;

	sprintf(cmd,"fatload usb 0 0x%x %s",pram_txt->start_addr, fname);

	ret = run_command(cmd, 0);
	if(ret < 0) {
		printf("%s read file fail !\n",__func__);
		goto out;
	}

	pram_txt->flen = simple_strtoul(getenv("filesize"), NULL, 16);

	ptr = pram_txt->start_addr + pram_txt->flen;
	*ptr = '\0';
out:
	return ret;
}

int nvt_ewritepart(unsigned int ptableaddr,int ptablelen)
{
	int ret = 0;
	nvt_ram_text rtxt;
	char line[64];
	char *str_id;
	char *str_sectors;
	char *str_tmp;
	int idx = 0;
    char *ptr;


	rtxt.start_addr = ptableaddr;
    rtxt.flen = ptablelen;
    ptr = rtxt.start_addr + rtxt.flen;
    *ptr = '\0';

	//reset partition table
	memset(nvt_part_table, 0, sizeof(nvt_part_table));
	memset(nvt_fs_tbl, 0, sizeof(nvt_fs_tbl));
	memset(nvt_mbr_dram_tbl, 0, sizeof(nvt_mbr_dram_tbl));
	memset(nvt_part_parm_tbl, 0, sizeof(nvt_part_parm_tbl));
	emmc_part_inited = 0;

	do {
		memset(line,0,sizeof(line));
		ret = nvt_read_line(line, sizeof(line), &rtxt);
		//printf("line = %s, ret = %d\n",line, ret);
		if(strlen(line) > 0) {
			str_id = line;
			str_tmp = strchr(line,',');
			*str_tmp = '\0';
			str_sectors = ++str_tmp;
			nvt_part_parm_tbl[idx].id = simple_strtoul(str_id, NULL, 10);
			nvt_part_parm_tbl[idx].sectors = simple_strtoul(str_sectors, NULL, 10);
			idx++;
		}

	} while(ret != -1);

	nvt_emmc_part_init();

out:
	return 0;
}
int nvt_read_line(char *buff,unsigned int buff_len, nvt_ram_text *pram_txt)
{
	char *ptr1 = (char*)(pram_txt->start_addr);
	char *ptr2;
	int ret = 0;

	//kwinyee debug
	//printf("flen = %d\n",pram_txt->flen);
	if(pram_txt->flen <= 0)
		return -1;

	ptr2 = strchr(ptr1,'\n');

	if(ptr2 == NULL) {
		//printf("read line done !\n");
		return -1;
	} else {
		*ptr2 = '\0';
		ptr2++;
		memset(buff,0,buff_len);
		memcpy(buff,ptr1,strlen(ptr1));
		pram_txt->start_addr = ptr2;
		//kwinyee debug
		//printf("ptr1 len = %d\n",strlen(ptr1));
		pram_txt->flen -= (strlen(ptr1) + 1);
	}

	return ret;
}

int do_nvt_eloadpart(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
	int ret = 0;
	nvt_ram_text rtxt;
	char line[64];
	char *str_id;
	char *str_sectors;
	char *str_tmp;
	int idx = 0;

	if(argc !=  2)
		return CMD_RET_USAGE;

	rtxt.start_addr = CONFIG_FWUP_RAM_START;

	ret = nvt_read_file_to_ram(argv[1], &rtxt);

	if(ret != 0) {
		printf("partition file %s read error !\n",argv[1]);
		goto out;
	}

	//reset partition table
	memset(nvt_part_table, 0, sizeof(nvt_part_table));
	memset(nvt_fs_tbl, 0, sizeof(nvt_fs_tbl));
	memset(nvt_mbr_dram_tbl, 0, sizeof(nvt_mbr_dram_tbl));
	memset(nvt_part_parm_tbl, 0, sizeof(nvt_part_parm_tbl));
	emmc_part_inited = 0;

	do {
		memset(line,0,sizeof(line));
		ret = nvt_read_line(line, sizeof(line), &rtxt);
		//printf("line = %s, ret = %d\n",line, ret);
		if(strlen(line) > 0) {
			str_id = line;
			str_tmp = strchr(line,',');
			*str_tmp = '\0';
			str_sectors = ++str_tmp;
			nvt_part_parm_tbl[idx].id = simple_strtoul(str_id, NULL, 10);
			nvt_part_parm_tbl[idx].sectors = simple_strtoul(str_sectors, NULL, 10);
			//kwinyee debug
			//printf("str_id : %s(%d) str_sectors : %s(%d)\n",str_id,simple_strtoul(str_id, NULL, 10),
			//		str_sectors, simple_strtoul(str_sectors, NULL, 10));
			//printf("idx = %d, id : %d, sectors : %d\n",idx,nvt_part_parm_tbl[idx].id,nvt_part_parm_tbl[idx].sectors);
			idx++;
		}

	} while(ret != -1);

	nvt_emmc_part_init();

out:
	return 0;
}

U_BOOT_CMD(
	eloadpart, 2, 1, do_nvt_eloadpart,
	"",
	""
);


