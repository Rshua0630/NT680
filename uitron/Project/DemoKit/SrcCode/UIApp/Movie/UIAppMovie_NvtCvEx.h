/**
    NVT CV library functions.

    Definitions of NVT CV functions.

    @file       UIAppMovie_NvtCvEx.h
    @ingroup    UIAppMovie
    @note       Nothing.

    Copyright   Novatek Microelectronics Corp. 2016.  All rights reserved.
*/
#ifndef _UIAPPMOVIE_NVTCVEX_H
#define _UIAPPMOVIE_NVTCVEX_H


void UIAppMovie_NvtCvEx_test(MEM_RANGE *buf);

void UIAppMovie_NvtCvEx_usageExample(MEM_RANGE *buf);
void UIAppMovie_NvtCvEx_testTranspose(MEM_RANGE *buf);
void UIAppMovie_NvtCvEx_testThresholdOtsu(MEM_RANGE *buf);
void UIAppMovie_NvtCvEx_testMatrixMult(MEM_RANGE *buf);
void UIAppMovie_NvtCvEx_testPatchMoments(MEM_RANGE *buf);
void UIAppMovie_NvtCvEx_testFindHomography(MEM_RANGE *buf);


#endif  //_UIAPPMOVIE_NVTCVEX_H