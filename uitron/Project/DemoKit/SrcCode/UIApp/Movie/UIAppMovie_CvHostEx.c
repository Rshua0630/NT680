#include <stdio.h>
#include "Type.h"
#include "Debug.h"
#include "FileSysTsk.h"
#include "PrjCfg.h"
#if _TODO
#include "CvHost_lib.h"
#endif
#include "UIAppMovie_CvHostEx.h"

#if _CVHOST_EX_


#define CVHOST_STRING_LEN_MAX     1024
#define CVHOST_IMG_ONAME_PATH     "A:\\Tests\\results\\"

static CVHOST_SEQHAND g_seq;
static CHAR g_CvHostImgFile[CVHOST_STRING_LEN_MAX];


void UIAppMovie_CvHostEx_Gaussian3X3(MEM_RANGE *buf)
{
	UINT32 i;
	CVHOST_STAT rlt;
	frame_buf_t srcFrame[3], dstFrame[3], *p_frameIn[3], *p_frameOut[3];

	// settings
	char *srcImgName = "A:\\Tests\\data\\OY0_1080Hx1920W.bin";
	char *dstImgName = "OY0_1080Hx1920W_gaussian3x3.bin";
	UINT32 imgRows   = 1080;
	UINT32 imgCols   = 1920;

	// memory allocation
	UINT32 memSizeImgIn  = imgRows * imgCols;
	UINT32 memSizeImgOut = imgRows * imgCols;
	UINT32 memSizeWrkMem = CvHost_GetWorkMemSize();
	if (memSizeImgIn + memSizeImgOut + memSizeWrkMem > buf->Size) {
		debug_msg("\n\rWorking buffer size is not large enough\n");
		return;
	}

	UINT32 addrWrkMem = buf->Addr;
	UINT32 addrSrcImg = addrWrkMem + memSizeWrkMem;
	UINT32 addrDstImg = addrSrcImg + memSizeImgIn;

	// read image
	FST_FILE filehdl = FileSys_OpenFile(srcImgName, FST_OPEN_READ);
	FileSys_ReadFile(filehdl, (UINT8 *)addrSrcImg, &memSizeImgIn, 0, NULL);
	FileSys_CloseFile(filehdl);

	//
	g_seq.srcFrameNum = 0;
	g_seq.dstFrameNum = 0;
	for (i = 0; i < 3; i++) {
		g_seq.seq_prmSetFlag[i] = 0;
	}
	for (i = 0; i < CVHOST_KERNEL_NUM_MAX; i++) {
		g_seq.kernelFlag[i] = 0;
	}

	debug_msg("\n\rCvHost test: CEVA_CV_gaussian3x3 ...\n");

	// Gaussian
	//   (a) open CvHost
	if ((rlt = CvHost_Open(&g_seq, addrWrkMem, memSizeWrkMem)) != CVHOST_SUCCEED) {
		debug_msg("\r  CvHost_Open failed (err-code: %d)\n", rlt);
		return;
	}
	debug_msg("\r  CvHost_Open\n");

	//   (b) add kernel
	if ((rlt = CvHost_AddKernel(&g_seq, SMARTFRAME_KERNEL_INDEX_0, "CEVA_CV_gaussian3x3", SMARTFRAME_DATA_TYPE_U8, SMARTFRAME_DATA_TYPE_U8)) != CVHOST_SUCCEED) {
		debug_msg("\r  CvHost_AddKernel failed (err-code: %d)\n", rlt);
		return;
	}
	debug_msg("\r  CvHost_AddKernel\n");

	//   (c) set kernel parameters

	//   (d) set sequence parameters
	if ((rlt = CvHost_SetSequenceParam(&g_seq, SMARTFRAME_SEQ_PADD_TYPE, SMARTFRAME_PADDING_REPLICATE)) != CVHOST_SUCCEED) {
		debug_msg("\r  CvHost_SetSequenceParam failed (err-code: %d)\n", rlt);
		return;
	}
	debug_msg("\r  CvHost_SetSequenceParam\n");

	//   (e) execution
	srcFrame[0].data   = (UINT8 *)(addrSrcImg);
	srcFrame[0].width  = imgCols;
	srcFrame[0].stride = imgCols;
	srcFrame[0].height = imgRows;

	dstFrame[0].data   = (UINT8 *)(addrDstImg);
	dstFrame[0].width  = imgCols;
	dstFrame[0].stride = imgCols;
	dstFrame[0].height = imgRows;

	for (i = 0; i < 3; i++) {
		p_frameIn[i]  = &srcFrame[i];
		p_frameOut[i] = &dstFrame[i];
	}
	if ((rlt = CvHost_ExecuteSequence(&g_seq, p_frameIn, 1, p_frameOut, 1)) != CVHOST_SUCCEED) {
		debug_msg("\r  CvHost_ExecuteSequence failed (err-code: %d)\n", rlt);
		return;
	}
	debug_msg("\r  CvHost_ExecuteSequence\n");

	// -> can do other things here

	if ((rlt = CvHost_WaitExecuteDone(&g_seq)) != CVHOST_SUCCEED) {
		debug_msg("\r  CvHost_WaitExecuteDone failed (err-code: %d)\n", rlt);
		return;
	}
	debug_msg("\r  CvHost_WaitExecuteDone\n");

	//   (f) close CvHost
	if ((rlt = CvHost_Close(&g_seq)) != CVHOST_SUCCEED) {
		debug_msg("\r  CvHost_Close failed (err-code: %d)\n", rlt);
		return;
	}
	debug_msg("\r  CvHost_Close\n");
	debug_msg("\r  CEVA_CV_GAUSSIAN3X3 ... done\n");

	// output
	snprintf(g_CvHostImgFile, sizeof(g_CvHostImgFile), "%s%s", (char *)CVHOST_IMG_ONAME_PATH, (char *)dstImgName);
	//sprintf(g_CvHostImgFile, "%s%s", (char*)CVHOST_IMG_ONAME_PATH, (char*)dstImgName);
	filehdl = FileSys_OpenFile(g_CvHostImgFile, FST_OPEN_WRITE | FST_CREATE_ALWAYS);
	FileSys_WriteFile(filehdl, (UINT8 *)addrDstImg, &memSizeImgOut, 0, NULL);
	FileSys_CloseFile(filehdl);
	debug_msg("\rWrite result to %s\n\n", g_CvHostImgFile);

}

void UIAppMovie_CvHostEx_Sobel3x3(MEM_RANGE *buf)
{
	UINT32 i;
	CVHOST_STAT rlt;
	frame_buf_t srcFrame[3], dstFrame[3], *p_frameIn[3], *p_frameOut[3];

	// settings
	CHAR *srcImgName = "A:\\Tests\\data\\OY0_1080Hx1920W.bin";
	CHAR *dstImgName = "OY0_1080Hx1920W_sobelDx3x3.bin";
	UINT32 imgRows   = 1080;
	UINT32 imgCols   = 1920;

	// memory allocation
	UINT32 memSizeImgIn  = imgRows * imgCols;
	UINT32 memSizeImgOut = imgRows * imgCols;
	UINT32 memSizeWrkMem = CvHost_GetWorkMemSize();
	if (memSizeImgIn + memSizeImgOut + memSizeWrkMem > buf->Size) {
		debug_msg("\n\rWorking buffer size is not large enough\n");
		return;
	}

	UINT32 addrWrkMem = buf->Addr;
	UINT32 addrSrcImg = addrWrkMem + memSizeWrkMem;
	UINT32 addrDstImg = addrSrcImg + memSizeImgIn;

	// read image
	FST_FILE filehdl = FileSys_OpenFile(srcImgName, FST_OPEN_READ);
	FileSys_ReadFile(filehdl, (UINT8 *)addrSrcImg, &memSizeImgIn, 0, NULL);
	FileSys_CloseFile(filehdl);

	//
	g_seq.srcFrameNum = 0;
	g_seq.dstFrameNum = 0;
	for (i = 0; i < 3; i++) {
		g_seq.seq_prmSetFlag[i] = 0;
	}
	for (i = 0; i < CVHOST_KERNEL_NUM_MAX; i++) {
		g_seq.kernelFlag[i] = 0;
	}

	debug_msg("\n\rCvHost test: CEVA_CV_sobel3x3dx ...\n");

	// Sobel dx, 3x3
	//   (a) open CvHost
	if ((rlt = CvHost_Open(&g_seq, addrWrkMem, memSizeWrkMem)) != CVHOST_SUCCEED) {
		debug_msg("\r  CvHost_Open failed (err-code: %d)\n", rlt);
		return;
	}
	debug_msg("\r  CvHost_Open\n");

	//   (b) add kernel
	if ((rlt = CvHost_AddKernel(&g_seq, SMARTFRAME_KERNEL_INDEX_0, "CEVA_CV_sobel3x3dx", SMARTFRAME_DATA_TYPE_U8, SMARTFRAME_DATA_TYPE_S16)) != CVHOST_SUCCEED) {
		debug_msg("\r  CvHost_AddKernel failed (err-code: %d)\n", rlt);
		return;
	}
	if ((rlt = CvHost_AddKernel(&g_seq, SMARTFRAME_KERNEL_INDEX_1, "CEVA_CV_absdiffScalar", SMARTFRAME_DATA_TYPE_S16, SMARTFRAME_DATA_TYPE_S16)) != CVHOST_SUCCEED) {
		debug_msg("\r  CvHost_AddKernel failed (err-code: %d)\n", rlt);
		return;
	}
	if ((rlt = CvHost_AddKernel(&g_seq, SMARTFRAME_KERNEL_INDEX_2, "CEVA_CV_formatConvert", SMARTFRAME_DATA_TYPE_S16, SMARTFRAME_DATA_TYPE_U8)) != CVHOST_SUCCEED) {
		debug_msg("\r  CvHost_AddKernel failed (err-code: %d)\n", rlt);
		return;
	}
	debug_msg("\r  CvHost_AddKernel\n");

	//   (c) set kernel parameters
	if ((rlt = CvHost_SetKernelParam(&g_seq, SMARTFRAME_KERNEL_INDEX_1, SMARTFRAME_PARAM_0, 0) != CVHOST_SUCCEED)) {
		debug_msg("\r  CvHost_SetKernelParam failed (err-code: %d)\r\n", rlt);
		return;
	}
	// - for CevaCv 1.9.3 ----------------------------------
	if ((rlt = CvHost_SetKernelParam(&g_seq, SMARTFRAME_KERNEL_INDEX_2, SMARTFRAME_PARAM_0, 0) != CVHOST_SUCCEED)) {
		debug_msg("\r  CvHost_SetKernelParam failed (err-code: %d)\r\n", rlt);
		return;
	}
	if ((rlt = CvHost_SetKernelParam(&g_seq, SMARTFRAME_KERNEL_INDEX_2, SMARTFRAME_PARAM_1, 1) != CVHOST_SUCCEED)) {
		debug_msg("\r  CvHost_SetKernelParam failed (err-code: %d)\r\n", rlt);
		return;
	}
	// --------------------------------------------------

	debug_msg("\r  CvHost_SetKernelParam\n");

	//   (d) set sequence parameters
	if ((rlt = CvHost_SetSequenceParam(&g_seq, SMARTFRAME_SEQ_PADD_TYPE, SMARTFRAME_PADDING_REPLICATE)) != CVHOST_SUCCEED) {
		debug_msg("\r  CvHost_SetSequenceParam failed (err-code: %d)\n", rlt);
		return;
	}
	debug_msg("\r  CvHost_SetSequenceParam\n");

	//   (e) execution
	srcFrame[0].data   = (UINT8 *)(addrSrcImg);
	srcFrame[0].width  = imgCols;
	srcFrame[0].stride = imgCols;
	srcFrame[0].height = imgRows;

	dstFrame[0].data   = (UINT8 *)(addrDstImg);
	dstFrame[0].width  = imgCols;
	dstFrame[0].stride = imgCols;
	dstFrame[0].height = imgRows;

	for (i = 0; i < 3; i++) {
		p_frameIn[i]  = &srcFrame[i];
		p_frameOut[i] = &dstFrame[i];
	}
	if ((rlt = CvHost_ExecuteSequence(&g_seq, p_frameIn, 1, p_frameOut, 1)) != CVHOST_SUCCEED) {
		debug_msg("\r  CvHost_ExecuteSequence failed (err-code: %d)\n", rlt);
		return;
	}
	debug_msg("\r  CvHost_ExecuteSequence\n");

	// -> can do other things here

	if ((rlt = CvHost_WaitExecuteDone(&g_seq)) != CVHOST_SUCCEED) {
		debug_msg("\r  CvHost_WaitExecuteDone failed (err-code: %d)\n", rlt);
		return;
	}
	debug_msg("\r  CvHost_WaitExecuteDone\n");

	//   (f) close CvHost
	if ((rlt = CvHost_Close(&g_seq)) != CVHOST_SUCCEED) {
		debug_msg("\r  CvHost_Close failed (err-code: %d)\n", rlt);
		return;
	}
	debug_msg("\r  CvHost_Close\n");
	debug_msg("\r  CEVA_CV_sobel3x3dx ... done\n");

	// output
	snprintf(g_CvHostImgFile, sizeof(g_CvHostImgFile), "%s%s", (char *)CVHOST_IMG_ONAME_PATH, (char *)dstImgName);
	//sprintf(g_CvHostImgFile, "%s%s", (char*)CVHOST_IMG_ONAME_PATH, (char*)dstImgName);
	filehdl = FileSys_OpenFile(g_CvHostImgFile, FST_OPEN_WRITE | FST_CREATE_ALWAYS);
	FileSys_WriteFile(filehdl, (UINT8 *)addrDstImg, &memSizeImgOut, 0, NULL);
	FileSys_CloseFile(filehdl);
	debug_msg("\rWrite result to %s\n\n", g_CvHostImgFile);
}

void UIAppMovie_CvHostEx_Opening(MEM_RANGE *buf)
{
	UINT32 i;
	CVHOST_STAT rlt;
	frame_buf_t srcFrame[3], dstFrame[3], *p_frameIn[3], *p_frameOut[3];

	// settings
	UINT8 kernel[9] = {
		0, 1, 0,
		1, 1, 1,
		0, 1, 0
	};

	char *srcImgName    = "A:\\Tests\\data\\binimg_224Hx379W.bin";
	char *dstImgNameOpn = "binimg_224Hx379W_open3x3.bin";
	UINT32 imgRows      = 224;
	UINT32 imgCols      = 379;

	// memory allocation
	UINT32 memSizeImgIn  = imgRows * imgCols;
	UINT32 memSizeImgOut = imgRows * imgCols;
	UINT32 memSizeWrkMem = CvHost_GetWorkMemSize();
	if (memSizeImgIn + memSizeImgOut + memSizeWrkMem > buf->Size) {
		debug_msg("\n\rWorking buffer size is not large enough\n");
		return;
	}

	UINT32 addrWrkMem = buf->Addr;
	UINT32 addrSrcImg = addrWrkMem + memSizeWrkMem;
	UINT32 addrDstImg = addrSrcImg + memSizeImgIn;

	// read image
	FST_FILE filehdl = FileSys_OpenFile(srcImgName, FST_OPEN_READ);
	FileSys_ReadFile(filehdl, (UINT8 *)addrSrcImg, &memSizeImgIn, 0, NULL);
	FileSys_CloseFile(filehdl);

	//
	g_seq.srcFrameNum = 0;
	g_seq.dstFrameNum = 0;
	for (i = 0; i < 3; i++) {
		g_seq.seq_prmSetFlag[i] = 0;
	}
	for (i = 0; i < CVHOST_KERNEL_NUM_MAX; i++) {
		g_seq.kernelFlag[i] = 0;
	}

	debug_msg("\n\rCvHost test: opening 3x3 (CEVA_CV_erode3x3 + CEVA_CV_dilate3x3) ...\n");

	//   (a) initial
	if ((rlt = CvHost_Open(&g_seq, addrWrkMem, memSizeWrkMem)) != CVHOST_SUCCEED) {
		debug_msg("\r  CvHost_Open failed (err-code: %d)\n", rlt);
		return;
	}
	debug_msg("\r  CvHost_Open\n", rlt);

	//   (b) add kernel
	if ((rlt = CvHost_AddKernel(&g_seq, SMARTFRAME_KERNEL_INDEX_0, "CEVA_CV_erode3x3", SMARTFRAME_DATA_TYPE_U8, SMARTFRAME_DATA_TYPE_U8)) != CVHOST_SUCCEED) {
		debug_msg("\r CvHost_AddKernel failed (err-code: %d)\n", rlt);
		return;
	}
	if ((rlt = CvHost_AddKernel(&g_seq, SMARTFRAME_KERNEL_INDEX_1, "CEVA_CV_dilate3x3", SMARTFRAME_DATA_TYPE_U8, SMARTFRAME_DATA_TYPE_U8)) != CVHOST_SUCCEED) {
		debug_msg("\r  CvHost_AddKernel failed (err-code: %d)\n", rlt);
		return;
	}
	debug_msg("\r  CvHost_AddKernel\n");

	//   (c) set kernel parameters
	if ((rlt = CvHost_SetKernelTableParam(&g_seq, SMARTFRAME_KERNEL_INDEX_0, SMARTFRAME_PARAM_0, kernel, 9)) != CVHOST_SUCCEED) {
		debug_msg("\r  CvHost_SetKernelTableParam failed (err-code: %d)\n", rlt);
	}
	if ((rlt = CvHost_SetKernelTableParam(&g_seq, SMARTFRAME_KERNEL_INDEX_1, SMARTFRAME_PARAM_0, kernel, 9)) != CVHOST_SUCCEED) {
		debug_msg("\r  CvHost_SetKernelTableParam failed (err-code: %d)\n", rlt);
	}
	debug_msg("\r  CvHost_SetKernelTableParam\n");

	//   (d) set sequence parameters
	if ((rlt = CvHost_SetSequenceParam(&g_seq, SMARTFRAME_SEQ_PADD_TYPE, SMARTFRAME_PADDING_REPLICATE)) != CVHOST_SUCCEED) {
		debug_msg("\r  CvHost_SetSequenceParam failed (err-code: %d)\n", rlt);
		return;
	}
	debug_msg("\r  CvHost_SetSequenceParam\n");

	//   (e) execution
	srcFrame[0].data   = (UINT8 *)(addrSrcImg);
	srcFrame[0].width  = imgCols;
	srcFrame[0].stride = imgCols;
	srcFrame[0].height = imgRows;

	dstFrame[0].data   = (UINT8 *)(addrDstImg);
	dstFrame[0].width  = imgCols;
	dstFrame[0].stride = imgCols;
	dstFrame[0].height = imgRows;

	for (i = 0; i < 3; i++) {
		p_frameIn[i]  = &srcFrame[i];
		p_frameOut[i] = &dstFrame[i];
	}
	if ((rlt = CvHost_ExecuteSequence(&g_seq, p_frameIn, 1, p_frameOut, 1)) != CVHOST_SUCCEED) {
		debug_msg("\r  CvHost_ExecuteSequence failed (err-code: %d)\n", rlt);
		return;
	}
	debug_msg("\r  CvHost_ExecuteSequence\n");

	// -> can do other things here

	if ((rlt = CvHost_WaitExecuteDone(&g_seq)) != CVHOST_SUCCEED) {
		debug_msg("\r  CvHost_WaitExecuteDone failed (err-code: %d)\n", rlt);
		return;
	}
	debug_msg("\r  CvHost_WaitExecuteDone\n");

	//   (f) close CvHost
	if ((rlt = CvHost_Close(&g_seq)) != CVHOST_SUCCEED) {
		debug_msg("\r  CvHost_Close failed (err-code: %d)\n", rlt);
		return;
	}
	debug_msg("\r  CvHost_Close\n");
	debug_msg("\r  Opening 3x3 ... done\n");

	// output
	snprintf(g_CvHostImgFile, sizeof(g_CvHostImgFile), "%s%s", (char *)CVHOST_IMG_ONAME_PATH, (char *)dstImgNameOpn);
	//sprintf(g_CvHostImgFile, "%s%s", (char*)CVHOST_IMG_ONAME_PATH, (char*)dstImgNameOpn);
	filehdl = FileSys_OpenFile(g_CvHostImgFile, FST_OPEN_WRITE | FST_CREATE_ALWAYS);
	FileSys_WriteFile(filehdl, (UINT8 *)addrDstImg, &memSizeImgOut, 0, NULL);
	FileSys_CloseFile(filehdl);
	debug_msg("\rWrite result to %s\n\n", g_CvHostImgFile);
}


#endif // _CVHOST_EX_

