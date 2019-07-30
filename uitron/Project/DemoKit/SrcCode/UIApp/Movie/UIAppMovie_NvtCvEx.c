/**
    NVT CV library test and example.

    Test of NVT CV functions and usage example.

    @file       UIAppMovie_NvtCvEx.c
    @ingroup    UIAppMovie
    @note       Nothing.

    Copyright   Novatek Microelectronics Corp. 2016.  All rights reserved.
*/
#include "FileSysTsk.h"
#include "SxCmd.h"
#include "SysKer.h"
#include "string.h"
#include "PrjCfg.h"
#include "UIAppMovie_NvtCvEx.h"

#if _TODO
#include "NvtCv_lib.h"
#include "NvtCvFuns.h"
#endif

#if _NVTCV_EX_


void UIAppMovie_NvtCvEx_testTranspose_NxM_S8(MEM_RANGE *buf);
void UIAppMovie_NvtCvEx_testTranspose_NxM_S16(MEM_RANGE *buf);
void UIAppMovie_NvtCvEx_testMatrixMult_NxM_MxL_S16(MEM_RANGE *buf);
void UIAppMovie_NvtCvEx_testMatrixMult_NxM_MxL_S16_S32(MEM_RANGE *buf);



void UIAppMovie_NvtCvEx_test(MEM_RANGE *buf)
{
	UIAppMovie_NvtCvEx_usageExample(buf);

	UIAppMovie_NvtCvEx_testTranspose(buf);
	UIAppMovie_NvtCvEx_testThresholdOtsu(buf);
	UIAppMovie_NvtCvEx_testMatrixMult(buf);
	UIAppMovie_NvtCvEx_testPatchMoments(buf);
}

void UIAppMovie_NvtCvEx_testPatchMoments(MEM_RANGE *buf)
{
	char *srcImgName = "A:\\Tests\\data\\OY0_1080Hx1920W.bin";
	UINT32 imgRows   = 1080;
	UINT32 imgCols   = 1920;

	INT32 *p_s32Mx, *p_s32My, option;

	NVTCV_FUN nvtFun;
	NVTCV_PATCHMOMENT_PRMS fprms;

	// memory allocation
	UINT32 memSizeImgIn = imgRows * imgCols;
	UINT32 memSizeRltMx = 4;
	UINT32 memSizeRltMy = 4;
	UINT32 memSizeWrkMem = NvtCv_GetWrkMemSize();

	UINT32 addrWrkMem = buf->Addr;
	UINT32 addrRltMx  = addrWrkMem + memSizeWrkMem;
	UINT32 addrRltMy  = addrRltMx + memSizeRltMx;
	UINT32 addrSrcImg = addrRltMy + memSizeRltMy;

	debug_msg("\n\rTest NVTCV_PATCHMOMENT --------------------\n");

	if (memSizeImgIn + memSizeRltMx + memSizeRltMy + memSizeWrkMem > buf->Size) {
		debug_msg("\n\rWorking buffer size is not large enough\n");
		return;
	}

	// option = 0: 17x17
	// option = 1: 31x31
	for (option = 0; option <= 1; option++) {
		UINT8 PatchMomentOption = option;
		UINT32 patchWidth = PatchMomentOption == 0 ? 17 : 31;

		UINT32 addrPatchCenter = addrSrcImg + imgCols * (patchWidth / 2) + (patchWidth / 2);

		// read source image
		FST_FILE filehdl = FileSys_OpenFile(srcImgName, FST_OPEN_READ);
		FileSys_ReadFile(filehdl, (UINT8 *)addrSrcImg, &memSizeImgIn, 0, NULL);
		FileSys_CloseFile(filehdl);

		if (option == 0) {
			debug_msg("\n\r  Test NVTCV_PATCHMOMENT_17X17_U8\n\r");
		} else {
			debug_msg("\n\r  Test NVTCV_PATCHMOMENT_31X31_U8\n\r");
		}

		// 1. open NvtCv
		NvtCv_Open(addrWrkMem);

		// 2. NvtCv setting
		// 2.1 set function enum
		nvtFun = NVTCV_PATCH_MOMENT;

		// 2.2 function parameters
		fprms.puiSrcCen = addrPatchCenter;
		fprms.piMx  = addrRltMx;
		fprms.piMy  = addrRltMy;
		fprms.iSrcStride = imgCols;
		fprms.uiOption = PatchMomentOption;

		// 3. preprocess
		NvtCv_Preprocess(nvtFun, (UINT32)(&fprms));

		// 4. application execution
		NvtCv_Execute(nvtFun, (UINT32)(&fprms));

		// can do other things (NOTE: nested NvtCv calls are not allowed)

		// 5. wait execution done
		NvtCv_WaitExecuteDone();

		// 6. postprocess
		NvtCv_Postprocess(nvtFun, (UINT32)(&fprms));

		// 7. close NvtCv
		NvtCv_Close();

		// get result
		p_s32Mx = (INT32 *)(addrRltMx);
		p_s32My = (INT32 *)(addrRltMy);
		debug_msg("\n\r  NVTCV_PATCHMOMENT result: Mx %d, My: %d\n", p_s32Mx[0], p_s32My[0]);

	}

	debug_msg("\n\r  NVTCV_PATCHMOMENT test done!\n\r");
}

void UIAppMovie_NvtCvEx_testMatrixMult(MEM_RANGE *buf)
{
	debug_msg("\n\rTest NVTCV_MATRIXMULT --------------------\n");
	UIAppMovie_NvtCvEx_testMatrixMult_NxM_MxL_S16(buf);
	UIAppMovie_NvtCvEx_testMatrixMult_NxM_MxL_S16_S32(buf);
	debug_msg("\n\r  NVTCV_MATRIXMULT test done!\n\r");
}

void UIAppMovie_NvtCvEx_testMatrixMult_NxM_MxL_S16_S32(MEM_RANGE *buf)
{
	const UINT32 N = 5;
	const UINT32 M = 3;
	const UINT32 L = 8;
	const INT32  PrmRnd = 0;
	const UINT32 PrmShiftRight = 0;

	INT32 svalue;
	UINT32 i, j;

	NVTCV_FUN nvtFun;
	NVTCV_MATRIXMULT_PRMS fprms;

	// memory allocation
	UINT32 memSizeSrc1 = ALIGN_ROUND_4(N * M * 2);
	UINT32 memSizeSrc2 = ALIGN_ROUND_4(M * L * 2);
	UINT32 memSizeDst  = ALIGN_ROUND_4(N * L * 4);
	UINT32 memSizeWrkMem = NvtCv_GetWrkMemSize();

	UINT32 addrWrkMem = buf->Addr;
	UINT32 addrSrc1   = addrWrkMem + memSizeWrkMem;
	UINT32 addrSrc2   = addrSrc1 + memSizeSrc1;
	UINT32 addrDst    = addrSrc2 + memSizeSrc2;

	INT16 *p_s16Src1 = (INT16 *)addrSrc1;
	INT16 *p_s16Src2 = (INT16 *)addrSrc2;
	INT32 *p_s32Dst  = (INT32 *)addrDst;

	if (memSizeSrc1 + memSizeSrc2 + memSizeDst + memSizeWrkMem > buf->Size) {
		debug_msg("\n\rWorking buffer size is not large enough\n");
		return;
	}

	// set matrices
	svalue = 0;
	for (i = 0; i < N; i++)
		for (j = 0; j < M; j++) {
			p_s16Src1[i * M + j] = svalue++;
		}
	svalue = 10;
	for (i = 0; i < M; i++)
		for (j = 0; j < L; j++) {
			p_s16Src2[i * L + j] = svalue--;
		}

	debug_msg("\n\r  NvtCv_testMatrixMult_NxM_MxL_S16_S32 matrix 1:\n\r  ");
	for (i = 0; i < N; i++) {
		for (j = 0; j < M; j++) {
			debug_msg("%d ", p_s16Src1[i * M + j]);
		}
		debug_msg("\n\r  ");
	}
	debug_msg("\n\r  NvtCv_testMatrixMult_NxM_MxL_S16_S32 matrix 2:\n\r  ");
	for (i = 0; i < M; i++) {
		for (j = 0; j < L; j++) {
			debug_msg("%d ", p_s16Src2[i * L + j]);
		}
		debug_msg("\n\r  ");
	}

	// 1. open NvtCv
	NvtCv_Open(addrWrkMem);

	// 2. NvtCv setting
	// 2.1 set function enum
	nvtFun = NVTCV_MATRIX_MULT;

	// 2.2 function parameters
	fprms.piSrc1 = addrSrc1;
	fprms.piSrc2 = addrSrc2;
	fprms.piDst  = addrDst;
	fprms.iSrc1Stride = M;
	fprms.iSrc2Stride = L;
	fprms.iDstStride  = L;
	fprms.uiN = N;
	fprms.uiM = M;
	fprms.uiL = L;
	fprms.iRnd = PrmRnd;
	fprms.uiShiftRight = PrmShiftRight;
	fprms.uiOption = NVTCV_MATRIXMULT_S16_S32;

	// 3. preprocess
	NvtCv_Preprocess(nvtFun, (UINT32)(&fprms));

	// 4. application execution
	NvtCv_Execute(nvtFun, (UINT32)(&fprms));

	// can do other things (NOTE: nested NvtCv calls are not allowed)

	// 5. wait execution done
	NvtCv_WaitExecuteDone();

	// 6. postprocess
	NvtCv_Postprocess(nvtFun, (UINT32)(&fprms));

	// 7. close NvtCv
	NvtCv_Close();

	// show result
	debug_msg("\n\r  NvtCv_testMatrixMult_NxM_MxL_S16_S32 result:\n\r  ");
	for (i = 0; i < N; i++) {
		for (j = 0; j < L; j++) {
			debug_msg("%d ", p_s32Dst[i * L + j]);
		}
		debug_msg("\n\r  ");
	}

}

void UIAppMovie_NvtCvEx_testMatrixMult_NxM_MxL_S16(MEM_RANGE *buf)
{
	const UINT32 N = 5;
	const UINT32 M = 3;
	const UINT32 L = 8;
	const INT32  PrmRnd = 0;
	const UINT32 PrmShiftRight = 0;

	UINT32 i, j;
	INT16 svalue;

	NVTCV_FUN nvtFun;
	NVTCV_MATRIXMULT_PRMS fprms;

	// memory allocation
	UINT32 memSizeSrc1 = ALIGN_ROUND_4(N * M * 2);
	UINT32 memSizeSrc2 = ALIGN_ROUND_4(M * L * 2);
	UINT32 memSizeDst  = ALIGN_ROUND_4(N * L * 2);
	UINT32 memSizeWrkMem = NvtCv_GetWrkMemSize();

	UINT32 addrWrkMem = buf->Addr;
	UINT32 addrSrc1   = addrWrkMem + memSizeWrkMem;
	UINT32 addrSrc2   = addrSrc1 + memSizeSrc1;
	UINT32 addrDst    = addrSrc2 + memSizeSrc2;

	INT16 *p_s16Src1 = (INT16 *)addrSrc1;
	INT16 *p_s16Src2 = (INT16 *)addrSrc2;
	INT16 *p_s16Dst  = (INT16 *)addrDst;

	if (memSizeSrc1 + memSizeSrc2 + memSizeDst + memSizeWrkMem > buf->Size) {
		debug_msg("\n\rWorking buffer size is not large enough\n");
		return;
	}

	// set matrices
	svalue = 0;
	for (i = 0; i < N; i++)
		for (j = 0; j < M; j++) {
			p_s16Src1[i * M + j] = svalue++;
		}
	svalue = 10;
	for (i = 0; i < M; i++)
		for (j = 0; j < L; j++) {
			p_s16Src2[i * L + j] = svalue--;
		}

	debug_msg("\n\r  NvtCv_testMatrixMult_NxM_MxL_S16 matrix 1:\n\r  ");
	for (i = 0; i < N; i++) {
		for (j = 0; j < M; j++) {
			debug_msg("%d ", p_s16Src1[i * M + j]);
		}
		debug_msg("\n\r  ");
	}
	debug_msg("\n\r  NvtCv_testMatrixMult_NxM_MxL_S16 matrix 2:\n\r  ");
	for (i = 0; i < M; i++) {
		for (j = 0; j < L; j++) {
			debug_msg("%d ", p_s16Src2[i * L + j]);
		}
		debug_msg("\n\r  ");
	}

	// 1. open NvtCv
	NvtCv_Open(addrWrkMem);

	// 2. NvtCv setting
	// 2.1 set function enum
	nvtFun = NVTCV_MATRIX_MULT;

	// 2.2 function parameters
	fprms.piSrc1 = addrSrc1;
	fprms.piSrc2 = addrSrc2;
	fprms.piDst  = addrDst;
	fprms.iSrc1Stride = M;
	fprms.iSrc2Stride = L;
	fprms.iDstStride  = L;
	fprms.uiN = N;
	fprms.uiM = M;
	fprms.uiL = L;
	fprms.iRnd = PrmRnd;
	fprms.uiShiftRight = PrmShiftRight;
	fprms.uiOption = NVTCV_MATRIXMULT_S16_S16;

	// 3. preprocess
	NvtCv_Preprocess(nvtFun, (UINT32)(&fprms));

	// 4. application execution
	NvtCv_Execute(nvtFun, (UINT32)(&fprms));

	// can do other things (NOTE: nested NvtCv calls are not allowed)

	// 5. wait execution done
	NvtCv_WaitExecuteDone();

	// 6. postprocess
	NvtCv_Postprocess(nvtFun, (UINT32)(&fprms));

	// 7. close NvtCv
	NvtCv_Close();

	// show result
	debug_msg("\n\r  NvtCv_testMatrixMult_NxM_MxL_S16 result:\n\r  ");
	for (i = 0; i < N; i++) {
		for (j = 0; j < L; j++) {
			debug_msg("%d ", p_s16Dst[i * L + j]);
		}
		debug_msg("\n\r  ");
	}
}

void UIAppMovie_NvtCvEx_testThresholdOtsu(MEM_RANGE *buf)
{
	UINT32 i, j, bin;
	UINT8  *p_u8Src;
	UINT32 *p_u32Hist;
	INT32 s32Thresh;

	CHAR *srcImgName = "A:\\Tests\\data\\OY0_1080Hx1920W.bin";
	UINT32 imgRows   = 1080;
	UINT32 imgCols   = 1920;

	NVTCV_FUN nvtFun;
	NVTCV_THRSH_OTSU_PRMS fprms;

	debug_msg("\n\rTest NVTCV_THRESHOLD_OTSU ----------------\n");

	// memory allocation
	UINT32 memSizeImgIn  = imgRows * imgCols;
	UINT32 memSizeHist   = 256 * 4;
	UINT32 memSizeWrkMem = NvtCv_GetWrkMemSize();

	UINT32 addrWrkMem = buf->Addr;
	UINT32 addrHist   = addrWrkMem + memSizeWrkMem;
	UINT32 addrSrcImg = addrHist + memSizeHist;

	if (memSizeImgIn + memSizeHist + memSizeWrkMem > buf->Size) {
		debug_msg("\n\rWorking buffer size is not large enough\n");
		return;
	}

	// read source image
	FST_FILE filehdl = FileSys_OpenFile(srcImgName, FST_OPEN_READ);
	FileSys_ReadFile(filehdl, (UINT8 *)addrSrcImg, &memSizeImgIn, 0, NULL);
	FileSys_CloseFile(filehdl);

	// 1. open NvtCv
	NvtCv_Open(addrWrkMem);

	// 2. NvtCv setting
	// 2.1 set function enum
	nvtFun = NVTCV_THRESHOLD_OTSU;

	// 2.2 accumulate histogram
	p_u32Hist = (UINT32 *)addrHist;
	memset(p_u32Hist, 0, sizeof(UINT32) * 256);

	p_u8Src = (UINT8 *)addrSrcImg;
	for (i = 0; i < imgRows; i++)
		for (j = 0; j < imgCols; j++) {
			bin = p_u8Src[i * imgCols + j];
			p_u32Hist[bin]++;
		}

	// 2.3 function parameters
	fprms.puiHist = addrHist;

	// 3. preprocess
	NvtCv_Preprocess(nvtFun, (UINT32)(&fprms));

	// 4. application execution
	NvtCv_Execute(nvtFun, (UINT32)(&fprms));

	// can do other things (NOTE: nested NvtCv calls are not allowed)

	// 5. wait execution done
	NvtCv_WaitExecuteDone();

	// 6. postprocess
	NvtCv_Postprocess(nvtFun, (UINT32)(&fprms));

	// 7. close NvtCv
	NvtCv_Close();

	// get result
	s32Thresh = fprms.iOtsuThresh;
	debug_msg("\n\r  NVTCV_THRESHOLD_OTSU result: %d\n\r", s32Thresh);

	debug_msg("\n\r  NVTCV_THRESHOLD_OTSU test done!\n\r");
}

void UIAppMovie_NvtCvEx_testTranspose(MEM_RANGE *buf)
{
	debug_msg("\n\rTest NVTCV_TRANSPOSE ---------------------\n");
	UIAppMovie_NvtCvEx_testTranspose_NxM_S8(buf);
	UIAppMovie_NvtCvEx_testTranspose_NxM_S16(buf);
	debug_msg("\n\r  NVTCV_TRANSPOSE test done!\n\r");
}

void UIAppMovie_NvtCvEx_testTranspose_NxM_S16(MEM_RANGE *buf)
{
	const UINT32 Cols = 5;
	const UINT32 Rows = 5;

	UINT32 i, j, cnt;
	NVTCV_FUN nvtFun;
	NVTCV_TRANSPOSE_PRMS fprms;

	// memory allocation
	UINT32 memSizeSrc = Cols * Rows * 2;
	UINT32 memSizeDst = memSizeSrc;
	UINT32 memSizeWrkMem = NvtCv_GetWrkMemSize();

	UINT32 addrWrkMem = buf->Addr;
	UINT32 addrSrcDat = addrWrkMem + memSizeWrkMem;
	UINT32 addrDstDat = addrSrcDat + memSizeSrc;

	UINT16 *p_s16Src = (UINT16 *)addrSrcDat;
	UINT16 *p_s16Dst = (UINT16 *)addrDstDat;

	if (memSizeSrc + memSizeDst + memSizeWrkMem > buf->Size) {
		debug_msg("\n\rWorking buffer size is not large enough\n");
		return;
	}

	// set input data
	cnt = 30;
	for (i = 0; i < Rows; i++)
		for (j = 0; j < Cols; j++) {
			p_s16Src[i * Cols + j] = cnt--;
		}

	debug_msg("\n\r  NvtCv_testTranspose_NxM_S16 input data:\n\r  ");
	for (i = 0; i < Rows; i++) {
		for (j = 0; j < Cols; j++) {
			debug_msg("%d ", p_s16Src[i * Cols + j]);
		}
		debug_msg("\n\r  ");
	}

	// 1. open NvtCv
	NvtCv_Open(addrWrkMem);

	// 2. NvtCv setting
	// 2.1 set function enum
	nvtFun = NVTCV_TRANSPOSE;

	// 2.2 function parameters
	fprms.piSrc = addrSrcDat;
	fprms.piDst = addrDstDat;
	fprms.uiWidth  = Cols;
	fprms.uiHeight = Rows;
	fprms.uiSrcStride = Cols;
	fprms.uiDstStride = Cols;
	fprms.uiOption = NVTCV_TRANSPOSE_NxM_S16;

	// 3. preprocess
	NvtCv_Preprocess(nvtFun, (UINT32)(&fprms));

	// 4. application execution
	NvtCv_Execute(nvtFun, (UINT32)(&fprms));

	// can do other things (NOTE: nested NvtCv calls are not allowed)

	// 5. wait execution done
	NvtCv_WaitExecuteDone();

	// 6. postprocess
	NvtCv_Postprocess(nvtFun, (UINT32)(&fprms));

	// 7. close NvtCv
	NvtCv_Close();

	// show result
	debug_msg("\n\r  NvtCv_testTranspose_NxM_S16 result:\n\r  ");
	for (i = 0; i < Rows; i++) {
		for (j = 0; j < Cols; j++) {
			debug_msg("%d ", p_s16Dst[i * Cols + j]);
		}
		debug_msg("\n\r  ");
	}
}

void UIAppMovie_NvtCvEx_testTranspose_NxM_S8(MEM_RANGE *buf)
{
	const UINT32 Cols = 5;
	const UINT32 Rows = 5;

	UINT32 i, j, cnt;
	NVTCV_FUN nvtFun;
	NVTCV_TRANSPOSE_PRMS fprms;

	// memory allocation
	UINT32 memSizeSrc = Cols * Rows;
	UINT32 memSizeDst = memSizeSrc;
	UINT32 memSizeWrkMem = NvtCv_GetWrkMemSize();

	UINT32 addrWrkMem = buf->Addr;
	UINT32 addrSrcDat = addrWrkMem + memSizeWrkMem;
	UINT32 addrDstDat = addrSrcDat + memSizeSrc;

	UINT8 *p_s8Src = (UINT8 *)addrSrcDat;
	UINT8 *p_s8Dst = (UINT8 *)addrDstDat;

	if (memSizeSrc + memSizeDst + memSizeWrkMem > buf->Size) {
		debug_msg("\n\rWorking buffer size is not large enough\n");
		return;
	}

	// set input data
	cnt = 0;
	for (i = 0; i < Rows; i++)
		for (j = 0; j < Cols; j++) {
			p_s8Src[i * Cols + j] = cnt++;
		}

	debug_msg("\n\r  NvtCv_testTranspose_NxM_S8 input data:\n\r  ");
	for (i = 0; i < Rows; i++) {
		for (j = 0; j < Cols; j++) {
			debug_msg("%d ", p_s8Src[i * Cols + j]);
		}
		debug_msg("\n\r  ");
	}

	// 1. open NvtCv
	NvtCv_Open(addrWrkMem);

	// 2. NvtCv setting
	// 2.1 set function enum
	nvtFun = NVTCV_TRANSPOSE;

	// 2.2 function parameters
	fprms.piSrc = addrSrcDat;
	fprms.piDst = addrDstDat;
	fprms.uiWidth  = Cols;
	fprms.uiHeight = Rows;
	fprms.uiSrcStride = Cols;
	fprms.uiDstStride = Cols;
	fprms.uiOption = NVTCV_TRANSPOSE_NxM_S8;

	// 3. preprocess
	NvtCv_Preprocess(nvtFun, (UINT32)(&fprms));

	// 4. application execution
	NvtCv_Execute(nvtFun, (UINT32)(&fprms));

	// can do other things (NOTE: nested NvtCv calls are not allowed)

	// 5. wait execution done
	NvtCv_WaitExecuteDone();

	// 6. postprocess
	NvtCv_Postprocess(nvtFun, (UINT32)(&fprms));

	// 7. close NvtCv
	NvtCv_Close();

	// show result
	debug_msg("\n\r  NvtCv_testTranspose_NxM_S8 result:\n\r  ");
	for (i = 0; i < Rows; i++) {
		for (j = 0; j < Cols; j++) {
			debug_msg("%d ", p_s8Dst[i * Cols + j]);
		}
		debug_msg("\n\r  ");
	}
}

void UIAppMovie_NvtCvEx_usageExample(MEM_RANGE *buf)
{
	char *srcImgName = "A:\\Tests\\data\\OY0_1080Hx1920W.bin";
	char *dstImgName = "A:\\Tests\\results\\OY0_1080Hx1920W_intAdd.bin";
	UINT32 imgRows   = 1080;
	UINT32 imgCols   = 1920;

	NVTCV_FUN nvtFun;
	NVTCV_TESTFUN_PRMS fprms;

	// memory allocation
	UINT32 memSizeImgIn  = imgRows * imgCols;
	UINT32 memSizeImgOut = imgRows * imgCols;
	UINT32 memSizeWrkMem = NvtCv_GetWrkMemSize();

	UINT32 addrWrkMem = buf->Addr;
	UINT32 addrSrcImg = addrWrkMem + memSizeWrkMem;
	UINT32 addrDstImg = addrSrcImg + memSizeImgIn;

	if (memSizeImgIn + memSizeImgOut + memSizeWrkMem > buf->Size) {
		debug_msg("\n\rWorking buffer size is not large enough\n");
		return;
	}

	// read source image
	FST_FILE filehdl = FileSys_OpenFile(srcImgName, FST_OPEN_READ);
	FileSys_ReadFile(filehdl, (UINT8 *)addrSrcImg, &memSizeImgIn, 0, NULL);
	FileSys_CloseFile(filehdl);

	// 1. open NvtCv
	NvtCv_Open(addrWrkMem);

	// 2. NvtCv setting
	// 2.1 set function enum
	nvtFun = NVTCV_TEST;

	// 2.2 function parameters
	fprms.addrSrcImg = addrSrcImg;
	fprms.addrDstImg = addrDstImg;
	fprms.width  = imgCols;
	fprms.height = imgRows;

	// 3. preprocess
	NvtCv_Preprocess(nvtFun, (UINT32)(&fprms));

	// 4. application execution
	NvtCv_Execute(nvtFun, (UINT32)(&fprms));

	// can do other things (NOTE: nested NvtCv calls are not allowed)

	// 5. wait execution done
	NvtCv_WaitExecuteDone();

	// 6. postprocess
	NvtCv_Postprocess(nvtFun, (UINT32)(&fprms));

	// 7. close NvtCv
	NvtCv_Close();

	// write result
	filehdl = FileSys_OpenFile(dstImgName, FST_OPEN_WRITE | FST_CREATE_ALWAYS);
	FileSys_WriteFile(filehdl, (UINT8 *)addrDstImg, &memSizeImgOut, 0, NULL);
	FileSys_CloseFile(filehdl);
	debug_msg("\rWrite result to %s\n\n", dstImgName);
}


#endif // _NVTCV_EX_
