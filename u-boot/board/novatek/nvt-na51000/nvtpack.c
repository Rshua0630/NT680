//#include "stdafx.h" //for win32 include, and must include ahead of any definition. You only can just mark it in other compiler tools.
#define NVTPACK_UBOOT // NVTPACK_WIN32, NVTPACK_UITRON, NVTPACK_UBOOT, NVTPACK_LINUX_KERNEL, NVTPACK_LINUX_APP

#if defined(NVTPACK_WIN32)
#elif defined(NVTPACK_UITRON)
#include "Type.h"
#elif defined(NVTPACK_UBOOT)
#include <common.h>
#elif defined(NVTPACK_LINUX_KERNEL)
#include <linux/string.h> //for linux include
#include <linux/printk.h> //for linux include
#elif defined(NVTPACK_LINUX_APP)
#include <stdio.h>
#include <string.h>
#endif

#include <asm/arch/nvt_types.h>
#include "nvtpack.h"

#ifdef __cplusplus
#define _NVTPACK_EXTERN_C extern "C"
#else
#define _NVTPACK_EXTERN_C
#endif

#ifndef UINT32_SWAP
#define UINT32_SWAP(data)           (((((unsigned int)(data)) & 0x000000FF) << 24) | \
									 ((((unsigned int)(data)) & 0x0000FF00) << 8) | \
									 ((((unsigned int)(data)) & 0x00FF0000) >> 8) | \
									 ((((unsigned int)(data)) & 0xFF000000) >> 24))   ///< Swap [31:24] with [7:0] and [23:16] with [15:8].
#endif
#if defined(NVTPACK_UITRON)
#define __MODULE__          nvtpack
#define __DBGLVL__          1 // 0=OFF, 1=ERROR, 2=TRACE
#define __DBGFLT__          "*" //*=All, [mark]=CustomClass
#include "DebugModule.h"
#endif

// {B5CB64AE-05DD-46FA-B906-0C62D22958D6}
static const GUID GUID_RES =
{ 0xb5cb64ae, 0x05dd, 0x46fa, { 0xb9, 0x06, 0x0c, 0x62, 0xd2, 0x29, 0x58, 0xd6 } };

// {8827BE90-36CD-4FC2-A987-73A8484E84B1}
static const GUID GUID_FW =
{ 0x8827be90, 0x36cd, 0x4fc2, { 0xa9, 0x87, 0x73, 0xa8, 0x48, 0x4e, 0x84, 0xb1 } };

// {D6012E07-10BC-4F91-B28A-352F82261A50}
static const GUID GUID_FW2 =
{ 0xd6012e07, 0x10bc, 0x4f91, { 0xb2, 0x8a, 0x35, 0x2f, 0x82, 0x26, 0x1a, 0x50 } };

static NVTPACK_ER nvtpack_er(NVTPACK_ER er)
{
	if(er == NVTPACK_ER_SUCCESS) {
		return er;
	}

	#if defined(NVTPACK_WIN32) || defined(NVTPACK_UBOOT) || defined(NVTPACK_LINUX_APP)
	printf("nvtpack:%d\r\n", er);
	#elif defined(NVTPACK_LINUX_KERNEL)
	pr_info("nvtpack:%d\r\n", er);
	#elif defined(NVTPACK_UITRON)
	DBG_ERR("%d\r\n", er);
	#endif

	return er;
}

static NVTPACK_ER nvtpack_get_uitron_offset(NVTPACK_MEM *p_mem, NVTPACK_MEM *p_uitron)
{
	unsigned char *p_curr = (unsigned char *)p_mem->p_data;
	unsigned char *p_end = (unsigned char *)p_mem->p_data + p_mem->len;
	NVTPACK_BFC_HDR *pBFC = (NVTPACK_BFC_HDR *)p_curr;
	NVTPACK_BININFO_HDR *pBinInfo = (NVTPACK_BININFO_HDR *)p_curr;

	memset(p_uitron, 0, sizeof(*p_uitron));

	if(pBFC->uiFourCC == MAKEFOURCC('B', 'C', 'L', '1')) {
		unsigned int bfc_size = UINT32_SWAP(pBFC->uiSizeComp);
		p_uitron->p_data = pBFC;
		p_uitron->len = bfc_size + sizeof(NVTPACK_BFC_HDR);
		if(p_curr + p_uitron->len > p_end) {
			return nvtpack_er(NVTPACK_ER_PARAM);
		}
		return nvtpack_er(NVTPACK_ER_SUCCESS);
	} else if(strncmp(pBinInfo->BinInfo_1, "NT9", 3) == 0) {
		unsigned int bin_size = pBinInfo->BinLength;
		p_uitron->p_data = pBinInfo;
		p_uitron->len = bin_size;
		if(p_curr + p_uitron->len > p_end) {
			return nvtpack_er(NVTPACK_ER_PARAM);
		}
		return nvtpack_er(NVTPACK_ER_SUCCESS);
	}
	return nvtpack_er(NVTPACK_ER_UNKNOWN_FORMAT);
}

_NVTPACK_EXTERN_C unsigned int nvtpack_calc_nvt_sum(NVTPACK_MEM *p_mem)
{
	unsigned int i, sum = 0;
	unsigned short *puiValue = (unsigned short *)p_mem->p_data;

	for(i = 0; i < (p_mem->len >> 1); i++) {
		sum += (*(puiValue + i) + i);
	}

	sum &= 0x0000FFFF;

	return sum;
}

_NVTPACK_EXTERN_C NVTPACK_ER nvtpack_verify(NVTPACK_MEM *p_mem, NVTPACK_VERIFY_OUTPUT *p_output)
{
	NVTPACK_MEM chk_mem = {0};

	memset(p_output, 0, sizeof(*p_output));

	if(IsEqualGUID(&GUID_FW2, (GUID *)p_mem->p_data)) {
		if(nvtpack_calc_nvt_sum(p_mem) != 0) {
			return nvtpack_er(NVTPACK_ER_CHECK_SUM);
		}
		p_output->ver = NVTPACK_VER_16072017;
		p_output->partition_cnt = ((NVTPACK_FW_HDR2 *)p_mem->p_data)->TotalRes;
		return nvtpack_er(NVTPACK_ER_SUCCESS);
	} else {
		//remove uITRON header
		unsigned char *p_curr = (unsigned char *)p_mem->p_data;
		unsigned char *p_end = (unsigned char *)p_mem->p_data + p_mem->len;
		if(!IsEqualGUID(&GUID_FW, (GUID *)p_mem->p_data)) {
			if(nvtpack_get_uitron_offset(p_mem, &chk_mem) != NVTPACK_ER_SUCCESS) {
				return nvtpack_er(NVTPACK_ER_PARAM);
			}

			if(nvtpack_calc_nvt_sum(&chk_mem) != 0) {
				return nvtpack_er(NVTPACK_ER_CHECK_SUM);
			}
			p_curr += chk_mem.len;
			if(p_curr >= p_end) {
				p_output->ver = NVTPACK_VER_13012816;
				p_output->partition_cnt = 1;
				return nvtpack_er(NVTPACK_ER_SUCCESS);
			} else {
				p_output->partition_cnt++;
			}
		}

		if(IsEqualGUID(&GUID_FW, (GUID *)p_curr)) {
			NVTPACK_FW_HDR *pFw = (NVTPACK_FW_HDR *)(p_curr);
			chk_mem.p_data = p_curr;
			chk_mem.len = sizeof(NVTPACK_FW_HDR) + pFw->TotalRes * sizeof(NVTPACK_PARTITION_HDR);
			if(nvtpack_calc_nvt_sum(&chk_mem) != 0) {
				return NVTPACK_ER_CHECK_SUM;
			}
			p_output->ver = NVTPACK_VER_13012816;
			p_output->partition_cnt += ((NVTPACK_FW_HDR *)p_curr)->TotalRes;
			return nvtpack_er(NVTPACK_ER_SUCCESS);
		}
	}
	return nvtpack_er(NVTPACK_ER_UNKNOWN_FORMAT);
}

_NVTPACK_EXTERN_C NVTPACK_ER nvtpack_get_partition(NVTPACK_GET_PARTITION_INPUT *p_input, NVTPACK_MEM *p_out)
{
	int i;
	int TotalRes = 0;
	NVTPACK_PARTITION_HDR *pPartition = NULL;
	unsigned char *p_curr = NULL;
	unsigned int id = p_input->id;
	NVTPACK_MEM *p_in = &p_input->mem;

	memset(p_out, 0, sizeof(*p_out));

	if(IsEqualGUID(&GUID_FW2, (GUID *)p_in->p_data)) {
		NVTPACK_FW_HDR2 *pFw = (NVTPACK_FW_HDR2 *)p_in->p_data;
		TotalRes = pFw->TotalRes;
		pPartition = (NVTPACK_PARTITION_HDR *)((unsigned int)p_in->p_data + sizeof(NVTPACK_FW_HDR2));
	} else {
		NVTPACK_MEM mem_uitron = {0};
		if(!IsEqualGUID(&GUID_FW, (GUID *)p_in->p_data)) {
			if(nvtpack_get_uitron_offset(p_in, &mem_uitron) != NVTPACK_ER_SUCCESS) {
				return nvtpack_er(NVTPACK_ER_PARAM);
			}
			if(id == 0) {
				p_out->p_data = mem_uitron.p_data;
				p_out->len = mem_uitron.len;
				return nvtpack_er(NVTPACK_ER_SUCCESS);
			}
		}

		p_curr = (unsigned char *)p_in->p_data + mem_uitron.len;

		if(IsEqualGUID(&GUID_FW, (GUID *)p_curr)) {
			NVTPACK_FW_HDR *pFw = (NVTPACK_FW_HDR *)p_curr;
			TotalRes = pFw->TotalRes;
			pPartition = (NVTPACK_PARTITION_HDR *)((unsigned int)p_curr + sizeof(NVTPACK_FW_HDR));
		} else {
			return nvtpack_er(NVTPACK_ER_UNKNOWN_FORMAT);
		}
	}

	for(i = 0; i < TotalRes; i++) {
		NVTPACK_PARTITION_HDR *p = &pPartition[i];
		if(p->PartitionID == id) {
			p_out->p_data = (unsigned char *)p_in->p_data + p->Offset;
			p_out->len = p->Size;
			return nvtpack_er(NVTPACK_ER_SUCCESS);
		}
	}
	return nvtpack_er(NVTPACK_ER_NOT_FOUND);
}

_NVTPACK_EXTERN_C NVTPACK_ER nvtpack_enum_partition(NVTPACK_ENUM_PARTITION_INPUT *p_input)
{
	int i;
	int TotalRes = 0;
	NVTPACK_MEM mem_out;
	NVTPACK_PARTITION_HDR *pPartition = NULL;
	unsigned char *p_curr = NULL;
	NVTPACK_MEM *p_in = &p_input->mem;

	if(IsEqualGUID(&GUID_FW2, (GUID *)p_in->p_data)) {
		NVTPACK_FW_HDR2 *pFw = (NVTPACK_FW_HDR2 *)p_in->p_data;
		TotalRes = pFw->TotalRes;
		pPartition = (NVTPACK_PARTITION_HDR *)((unsigned int)p_in->p_data + sizeof(NVTPACK_FW_HDR2));
	} else {
		NVTPACK_MEM mem_uitron = {0};
		if(!IsEqualGUID(&GUID_FW, (GUID *)p_in->p_data)) {
			if(nvtpack_get_uitron_offset(p_in, &mem_uitron) != NVTPACK_ER_SUCCESS) {
				return nvtpack_er(NVTPACK_ER_PARAM);
			}
			if(p_input->fp_enum) {
				if(p_input->fp_enum(0, &mem_uitron, p_input->p_user_data) != 0) {
					return nvtpack_er(NVTPACK_ER_USER_BREAK);
				}
			}
		}

		p_curr = (unsigned char *)p_in->p_data + mem_uitron.len;

		if(IsEqualGUID(&GUID_FW, (GUID *)p_curr)) {
			NVTPACK_FW_HDR *pFw = (NVTPACK_FW_HDR *)p_curr;
			TotalRes = pFw->TotalRes;
			pPartition = (NVTPACK_PARTITION_HDR *)((unsigned int)p_curr + sizeof(NVTPACK_FW_HDR));
		} else {
			return nvtpack_er(NVTPACK_ER_UNKNOWN_FORMAT);
		}
	}

	for(i = 0; i < TotalRes; i++) {
		NVTPACK_PARTITION_HDR *p = &pPartition[i];
		mem_out.p_data = (unsigned char *)p_in->p_data + p->Offset;
		mem_out.len = p->Size;
		if(p_input->fp_enum) {
			if(p_input->fp_enum(p->PartitionID, &mem_out, p_input->p_user_data) != 0) {
				return nvtpack_er(NVTPACK_ER_USER_BREAK);
			}
		}
	}

	return nvtpack_er(NVTPACK_ER_SUCCESS);
}

static NVTPACK_ER nvtpack_make_parition_pkg_fw1(NVTPACK_MAKE_PARTITION_INPUT *p_input, NVTPACK_MEM *p_out)
{
	NVTPACK_PARTITION_ITEM *p_list = p_input->p_list;
	int num = p_input->num;
	NVTPACK_MEM *p_work = &p_input->mem_work;
	unsigned char *p_head = (unsigned char *)p_work->p_data;
	unsigned char *p_curr = p_head;
	unsigned char *p_end = (unsigned char *)p_work->p_data + p_work->len;
	NVTPACK_PARTITION_ITEM *p_list_curr = p_list;
	NVTPACK_FW_HDR *p_fw_hdr = NULL;
	NVTPACK_PARTITION_HDR *p_pat_hdr = NULL;
	NVTPACK_PARTITION_HDR *p_pat_curr = NULL;
	NVTPACK_MEM mem_sum = {0};
	unsigned int total_size;

	memset(p_out, 0, sizeof(*p_out));

	if(num <= 0) {
		return nvtpack_er(NVTPACK_ER_PARAM);
	}

	//check uITRON existing
	if(p_list_curr->id == 0) {
		if(p_list_curr->mem.len != ALIGN_CEIL_4(p_list_curr->mem.len)) {
			return nvtpack_er(NVTPACK_ER_UITRON_SIZE_ALIGN4);
		}
		if(p_curr + p_list_curr->mem.len > p_end) {
			return nvtpack_er(NVTPACK_ER_MEM_NOT_ENOUGH);
		}
		memcpy(p_curr, p_list_curr->mem.p_data, p_list_curr->mem.len);
		p_curr += p_list_curr->mem.len;
		p_list_curr++;
		num--;
	}

	if(num == 0) {
		p_out->p_data = p_head;
		p_out->len = (unsigned int)(p_curr - p_head);
		return nvtpack_er(NVTPACK_ER_SUCCESS);
	}

	//make header
	if(p_curr + sizeof(NVTPACK_FW_HDR) > p_end) {
		return nvtpack_er(NVTPACK_ER_MEM_NOT_ENOUGH);
	}

	p_fw_hdr = (NVTPACK_FW_HDR *)p_curr;
	p_curr += sizeof(NVTPACK_FW_HDR);

	memset(p_fw_hdr, 0 , sizeof(*p_fw_hdr));
	p_fw_hdr->guid = GUID_FW;

	//keep NVTPACK_PARTITION_HDR space
	if(p_curr + sizeof(NVTPACK_PARTITION_HDR)*num > p_end) {
		return nvtpack_er(NVTPACK_ER_MEM_NOT_ENOUGH);
	}

	p_pat_hdr = (NVTPACK_PARTITION_HDR *) p_curr;
	memset(p_pat_hdr, 0, sizeof(*p_pat_hdr)*num);
	p_curr += sizeof(*p_pat_hdr) * num;
	p_fw_hdr->TotalRes = (unsigned int)num;
	p_fw_hdr->TotalSize = sizeof(*p_fw_hdr) + sizeof(*p_pat_hdr) * p_fw_hdr->TotalRes;

	p_pat_curr = p_pat_hdr;
	while(num > 0) {
		p_pat_curr->PartitionID = p_list_curr->id;
		p_pat_curr->Offset = (unsigned int)(p_curr - p_head);
		p_pat_curr->Size = p_list_curr->mem.len;

		if(p_curr + ALIGN_CEIL_4(p_pat_curr->Size) > p_end) {
			return nvtpack_er(NVTPACK_ER_MEM_NOT_ENOUGH);
		}
		memset(p_curr, 0, ALIGN_CEIL_4(p_pat_curr->Size));
		memcpy(p_curr, p_list_curr->mem.p_data, p_pat_curr->Size);

		p_curr += ALIGN_CEIL_4(p_pat_curr->Size);
		p_pat_curr++;
		p_list_curr++;
		num--;
	}

	memset(&mem_sum, 0, sizeof(mem_sum));
	mem_sum.p_data = p_fw_hdr;
	mem_sum.len = p_fw_hdr->TotalSize;
	p_fw_hdr->CheckSum = (~nvtpack_calc_nvt_sum(&mem_sum) + 1) & 0xFFFF;

	total_size = (unsigned int)(p_curr - (unsigned char *)p_fw_hdr);
	if(total_size != ALIGN_CEIL_4(total_size)) {
		return nvtpack_er(NVTPACK_ER_INVALID_WHOLE_SIZE);
	}

	p_out->p_data = p_head;
	p_out->len = (unsigned int)(p_curr - p_head);

	return nvtpack_er(NVTPACK_ER_SUCCESS);
}

static NVTPACK_ER nvtpack_make_parition_pkg_fw2(NVTPACK_MAKE_PARTITION_INPUT *p_input, NVTPACK_MEM *p_out)
{
	NVTPACK_PARTITION_ITEM *p_list = p_input->p_list;
	int num = p_input->num;
	NVTPACK_MEM *p_work = &p_input->mem_work;
	unsigned char *p_head = (unsigned char *)p_work->p_data;
	unsigned char *p_curr = p_head;
	unsigned char *p_end = (unsigned char *)p_work->p_data + p_work->len;
	NVTPACK_PARTITION_ITEM *p_list_curr = p_list;
	NVTPACK_FW_HDR2 *p_fw_hdr = NULL;
	NVTPACK_PARTITION_HDR *p_pat_hdr = NULL;
	NVTPACK_PARTITION_HDR *p_pat_curr = NULL;
	NVTPACK_MEM mem_sum = {0};

	memset(p_out, 0, sizeof(*p_out));

	if(num <= 0) {
		return nvtpack_er(NVTPACK_ER_PARAM);
	}

	//make header
	if(p_curr + sizeof(NVTPACK_FW_HDR2) > p_end) {
		return nvtpack_er(NVTPACK_ER_MEM_NOT_ENOUGH);
	}

	p_fw_hdr = (NVTPACK_FW_HDR2 *)p_curr;
	p_curr += sizeof(*p_fw_hdr);

	memset(p_fw_hdr, 0 , sizeof(*p_fw_hdr));
	p_fw_hdr->guid = GUID_FW2;
	p_fw_hdr->uiVersion = NVTPACK_FW_HDR2_VERSION;
	p_fw_hdr->uiHdrSize = sizeof(*p_fw_hdr);
	//keep NVTPACK_PARTITION_HDR space
	if(p_curr + sizeof(NVTPACK_PARTITION_HDR)*num > p_end) {
		return nvtpack_er(NVTPACK_ER_MEM_NOT_ENOUGH);
	}

	p_pat_hdr = (NVTPACK_PARTITION_HDR *) p_curr;
	memset(p_pat_hdr, 0, sizeof(*p_pat_hdr)*num);
	p_curr += sizeof(*p_pat_hdr) * num;
	p_fw_hdr->TotalRes = (unsigned int)num;

	p_pat_curr = p_pat_hdr;
	while(num > 0) {
		p_pat_curr->PartitionID = p_list_curr->id;
		p_pat_curr->Offset = (unsigned int)(p_curr - p_head);
		p_pat_curr->Size = p_list_curr->mem.len;

		if(p_curr + ALIGN_CEIL_4(p_pat_curr->Size) > p_end) {
			return nvtpack_er(NVTPACK_ER_MEM_NOT_ENOUGH);
		}
		memset(p_curr, 0, ALIGN_CEIL_4(p_pat_curr->Size));
		memcpy(p_curr, p_list_curr->mem.p_data, p_pat_curr->Size);

		p_curr += ALIGN_CEIL_4(p_pat_curr->Size);
		p_pat_curr++;
		p_list_curr++;
		num--;
	}

	p_fw_hdr->TotalSize = (unsigned int)(p_curr - (unsigned char *)p_head);

	memset(&mem_sum, 0, sizeof(mem_sum));
	mem_sum.p_data = p_head;
	mem_sum.len = p_fw_hdr->TotalSize;
	p_fw_hdr->uiChkValue = (~nvtpack_calc_nvt_sum(&mem_sum) + 1) & 0xFFFF;
	if(p_fw_hdr->TotalSize != ALIGN_CEIL_4(p_fw_hdr->TotalSize)) {
		return nvtpack_er(NVTPACK_ER_INVALID_WHOLE_SIZE);
	}

	p_out->p_data = p_head;
	p_out->len = (unsigned int)(p_curr - p_head);

	return nvtpack_er(NVTPACK_ER_SUCCESS);
}

_NVTPACK_EXTERN_C NVTPACK_ER nvtpack_make_parition_pkg(NVTPACK_MAKE_PARTITION_INPUT *p_input, NVTPACK_MEM *p_out)
{
	switch(p_input->ver) {
		case NVTPACK_VER_13012816:
			return nvtpack_make_parition_pkg_fw1(p_input, p_out);
		case NVTPACK_VER_16072017:
			return nvtpack_make_parition_pkg_fw2(p_input, p_out);
		default:
			return nvtpack_er(NVTPACK_ER_PARAM);
	}
}

_NVTPACK_EXTERN_C NVTPACK_ER nvtpack_get_resource(NVTPACK_GET_RESOURCE_INPUT *p_input, NVTPACK_MEM *p_out)
{
	const char *filename = p_input->filename;
	NVTPACK_MEM *p_in = &p_input->mem;
	unsigned char *p_data = (unsigned char *)p_in->p_data;
	int remain_len = (int)p_in->len;

	memset(p_out, 0, sizeof(*p_out));

	while(remain_len > 0) {
		NVTPACK_MEM mem_check = {0};
		NVTPACK_RES_HDR *pRes = (NVTPACK_RES_HDR *)p_data;

		if(!IsEqualGUID(&GUID_RES, (GUID *)p_data)) {
			return nvtpack_er(NVTPACK_ER_UNKNOWN_FORMAT);
		}

		if(strncmp(pRes->DstName, filename, strlen(filename)) == 0) {
			mem_check.p_data = pRes;
			mem_check.len = pRes->TotalSize;
			if(nvtpack_calc_nvt_sum(&mem_check) != 0) {
				return nvtpack_er(NVTPACK_ER_CHECK_SUM);
			}
			p_out->p_data = p_data + sizeof(NVTPACK_RES_HDR);
			p_out->len = pRes->ContentSize;
			return nvtpack_er(NVTPACK_ER_SUCCESS);
		}

		p_data += pRes->TotalSize;
		remain_len -= (int)pRes->TotalSize;
	}

	return nvtpack_er(NVTPACK_ER_NOT_FOUND);
}

_NVTPACK_EXTERN_C NVTPACK_ER nvtpack_enum_resource(NVTPACK_ENUM_RESOURCE_INPUT *p_input)
{
	unsigned char *p_data = (unsigned char *)p_input->mem.p_data;
	int remain_len = (int)p_input->mem.len;

	while(remain_len > 0) {
		NVTPACK_MEM mem_check = {0};
		NVTPACK_MEM mem_out = {0};
		NVTPACK_RES_HDR *pRes = (NVTPACK_RES_HDR *)p_data;

		if(!IsEqualGUID(&GUID_RES, (GUID *)p_data)) {
			return nvtpack_er(NVTPACK_ER_UNKNOWN_FORMAT);
		}

		mem_check.p_data = pRes;
		mem_check.len = pRes->TotalSize;
		if(nvtpack_calc_nvt_sum(&mem_check) != 0) {
			return nvtpack_er(NVTPACK_ER_CHECK_SUM);
		}

		mem_out.p_data = p_data + sizeof(NVTPACK_RES_HDR);
		mem_out.len = pRes->ContentSize;
		if(p_input->fp_enum) {
			if(p_input->fp_enum(pRes, &mem_out, p_input->p_user_data) != 0) {
				return nvtpack_er(NVTPACK_ER_USER_BREAK);
			}
		}

		p_data += pRes->TotalSize;
		remain_len -= (int)pRes->TotalSize;
	}

	return nvtpack_er(NVTPACK_ER_SUCCESS);
}

_NVTPACK_EXTERN_C NVTPACK_ER nvtpack_make_resource_pkg(NVTPACK_MAKE_RESOURCE_INPUT *p_input, NVTPACK_MEM *p_out)
{
	int i, j;
	NVTPACK_RESOURCE_ITEM *p_list = p_input->p_list;
	int num = p_input->num;
	NVTPACK_MEM *p_work = &p_input->mem_work;
	unsigned char *p_head = (unsigned char *)p_work->p_data;
	unsigned char *p_curr = p_head;
	unsigned char *p_end = (unsigned char *)p_work->p_data + p_work->len;
	NVTPACK_RESOURCE_ITEM *p_list_curr = p_list;

	memset(p_out, 0, sizeof(*p_out));

	//check all item name
	for(i = 0; i < num; i++) {
		p_list_curr = &p_list[i];
		if(strlen(p_list_curr->DstName) == 0) {
			return nvtpack_er(NVTPACK_ER_SRC_NAME_BLANK);
		} else if(strlen(p_list_curr->DstName) > 12) {
			return nvtpack_er(NVTPACK_ER_SRC_NAME_EXCEED);
		} else if(strstr(p_list_curr->DstName, " ") != NULL) {
			return nvtpack_er(NVTPACK_ER_SRC_NAME_INVALID);
		}
		//check same section name
		for(j = i + 1; j < num; j++) {
			if(strcmp(p_list_curr->DstName, p_list[j].DstName) == 0) {
				return nvtpack_er(NVTPACK_ER_SRC_NAME_ALIKE);
			}
		}
	}

	p_list_curr = p_list;
	for(i = 0; i < num; i++) {
		unsigned int require_size = ALIGN_CEIL(sizeof(NVTPACK_RES_HDR) + p_list_curr->mem.len, 32);
		unsigned char *p_next = p_curr + require_size;
		NVTPACK_RES_HDR *p_hdr = (NVTPACK_RES_HDR *)p_curr;
		NVTPACK_MEM mem_sum = {0};

		if(p_curr + require_size > p_end) {
			return nvtpack_er(NVTPACK_ER_MEM_NOT_ENOUGH);
		}

		memset(p_curr, 0, require_size);
		p_curr += sizeof(*p_hdr);
		p_hdr->guid = GUID_RES;
		p_hdr->TotalSize = require_size;
		p_hdr->ContentSize = p_list_curr->mem.len;
		memset(p_hdr->SrcName, 0, sizeof(p_hdr->SrcName));
		memset(p_hdr->DstName, 0, sizeof(p_hdr->DstName));
		strncpy(p_hdr->SrcName, p_list_curr->SrcName, 12);
		strncpy(p_hdr->DstName, p_list_curr->DstName, 12);

		if(p_curr + p_list_curr->mem.len > p_end) {
			return nvtpack_er(NVTPACK_ER_MEM_NOT_ENOUGH);
		}

		memcpy(p_curr, p_list_curr->mem.p_data, p_list_curr->mem.len);
		//check sum
		memset(&mem_sum, 0, sizeof(mem_sum));
		mem_sum.p_data = p_hdr;
		mem_sum.len = p_hdr->TotalSize;
		p_hdr->CheckSum = (~nvtpack_calc_nvt_sum(&mem_sum) + 1) & 0xFFFF;
		p_curr = p_next;
		p_list_curr++;
	}

	p_out->p_data = p_head;
	p_out->len = (unsigned int)(p_curr - p_head);

	return nvtpack_er(NVTPACK_ER_SUCCESS);
}
