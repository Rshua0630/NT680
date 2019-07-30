/**
    Convolutional Neural Networks library.

    @file       NvtNNFlow.h
    @ingroup

    Copyright   Novatek Microelectronics Corp. 2016.  All rights reserved.
*/
#ifndef _NVTNN_FLOW_H_
#define _NVTNN_FLOW_H_

#include    "Type.h"
#include    "GxImage.h"
#include    "nvtnn_lib.h"
#include    "svm_lib.h"
#include    "NvtNN_Common.h"

#define CDNN_IPC_PRM_MEM_SZ         0x800
#define NN_STA_OK   1
#define NN_STA_ERR  -1

extern UINT32 _SECTION(".kercfg_data") g_uiFLG_ODP;
void ODP_InstallID(void) _SECTION(".kercfg_text");

void NN_Process(IMG_BUF *pInputImg, UINT32 netid);

UINT32 NN_CalcCacheBuffSize(void);

UINT32 OD_CalcBuffSize(void);
UINT32 ODP_CalcBuffSize(void);
UINT32 NN_CalcBuffSize(void);

UINT32 NN_Init(MEM_RANGE *buf, MEM_RANGE *cachebuf, UINT32 ProcessRate);

UINT32 NN_LoadModel(void);

UINT32 NN_GetResult(NN_Result *result, URECT* pTargetCoord);

void NN_NetSelect(UINT32 sel);
#endif

