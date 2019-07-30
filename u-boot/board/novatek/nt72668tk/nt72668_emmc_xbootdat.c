/*
 *  board/novatek/nt72668tk/nt72668_emmc_xbootdat.c
 *
 *  Author:	Alvin lin
 *  Created:	Jun 5, 2015
 *  Copyright:	Novatek Inc.
 *
 */
#include <asm-generic/errno.h>
#include <common.h>
#include <asm/global_data.h>
#include "nt72668_emmc_xbootdat.h"
#include "nt72668_utils.h"
#include "nt72668_emmc_partition.h"

#if defined(CONFIG_EVB_668) || defined(CONFIG_SRI668) || defined(CONFIG_N62001)
#include "nt72668_emmc_nvt_xbootdat.c"
#elif defined(CONFIG_N33002)
#include "nt72668_emmc_n33002_xbootdat.c"
#else
#error "You should define your production xbootdat info first !"
#endif

static nvt_emmc_xbootdat_info* nvt_emmc_get_xdat_by_name(char *name)
{
	int i, total_cnt = sizeof(xdat_info) / sizeof(nvt_emmc_xbootdat_info);

	for(i=0;i < total_cnt;i++) {
		if(strcmp(xdat_info[i].xdat_name, name) == 0)
			break;
	}

	if(i >= total_cnt)
		return NULL;
	else
		return (&(xdat_info[i]));
}

static u32 nvt_emmc_get_xdat_size(void)
{
	return sizeof(xdat_info) / sizeof(nvt_emmc_xbootdat_info);
}

static nvt_emmc_xbootdat_info* nvt_emmc_get_xdat_by_index(u32 idx)
{
	int total_cnt = sizeof(xdat_info) / sizeof(nvt_emmc_xbootdat_info);

	if (idx >= total_cnt)
		return NULL;
	else
		return(&(xdat_info[idx]));
}

void xbootdat_init(unsigned int xbootdat_start_offset)
{
	int i;

	for(i=0; i < (sizeof(xdat_info) / sizeof(nvt_emmc_xbootdat_info)); i++)
		xdat_info[i].start_sector += xbootdat_start_offset;
}

int do_nvt_xsave(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
	nvt_emmc_xbootdat_info *pxdat_info;
	int ret = 0;

	if(argc < 2) { 
		ret = CMD_RET_USAGE;
		goto out;
	}

	if(strcmp(argv[1], "help") == 0) {
		int i, total_cnt;

		total_cnt = nvt_emmc_get_xdat_size();

		for(i = 0; i < total_cnt;i++) {
			pxdat_info = nvt_emmc_get_xdat_by_index(i);

			if(pxdat_info != NULL)
				pxdat_info->xdathelp(pxdat_info, XDAT_HELP_SAVE);
		}

		goto out;
	}


	pxdat_info = nvt_emmc_get_xdat_by_name(argv[1]);

	if(pxdat_info == NULL) {
		ret = CMD_RET_USAGE;
		goto out;
	}


	ret = pxdat_info->xdatsave(pxdat_info, argc, argv);
out:
	return ret;
}

U_BOOT_CMD(
	xsave, 4, 1, do_nvt_xsave,
	"save parameter to xbootdat",
	"xsave panel index idx_id - save panel index for xboot use\n"
	"xsave kercmd - save kernel command line for xboot use\n"
);

int do_nvt_xdump(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
	nvt_emmc_xbootdat_info *pxdat_info;
	int ret = 0;

	if(argc != 2) { 
		ret = CMD_RET_USAGE;
		goto out;
	}

	if(strcmp(argv[1], "help") == 0) {
		int i, total_cnt;

		total_cnt = nvt_emmc_get_xdat_size();

		for(i = 0; i < total_cnt;i++) {
			pxdat_info = nvt_emmc_get_xdat_by_index(i);

			if(pxdat_info != NULL)
				pxdat_info->xdathelp(pxdat_info, XDAT_HELP_DUMP);
		}

		goto out;
	}


	pxdat_info = nvt_emmc_get_xdat_by_name(argv[1]);

	if(pxdat_info == NULL) {
		ret = CMD_RET_USAGE;
		goto out;
	}


	ret = pxdat_info->xdatdump(pxdat_info, argc, argv);


out:
	return ret;
}

U_BOOT_CMD(
	xdump, 2, 1, do_nvt_xdump,
	"dump xbootdat parameter",
	"xdump panel - dump panel parameters in xbootdat\n"
	"xdump kercmd - dump kernel command in xbootdat\n"
);

int do_nvt_xerase(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
	nvt_emmc_xbootdat_info *pxdat_info;
	int ret = 0;

	if(argc != 2) { 
		ret = CMD_RET_USAGE;
		goto out;
	}

	if(strcmp(argv[1], "help") == 0) {
		int i, total_cnt;

		total_cnt = nvt_emmc_get_xdat_size();

		for(i = 0; i < total_cnt;i++) {
			pxdat_info = nvt_emmc_get_xdat_by_index(i);

			if(pxdat_info != NULL)
				pxdat_info->xdathelp(pxdat_info, XDAT_HELP_ERASE);
		}

		goto out;
	}

	pxdat_info = nvt_emmc_get_xdat_by_name(argv[1]);

	if(pxdat_info == NULL) {
		ret = CMD_RET_USAGE;
		goto out;
	}


	ret = pxdat_info->xdaterase(pxdat_info, argc, argv);


out:
	return ret;
}


U_BOOT_CMD(
	xerase, 2, 1, do_nvt_xerase,
	"erase xbootdat parameter",
	"xerase panel - erase panel parameters in xbootdat\n"
	"xerase kercmd -erase kernel command line in xbootdat\n"
);


