#ifndef __NVT_SVM__H__
#define __NVT_SVM__H__
#include "svm_lib.h"

BOOL NvtSvm_tran16bFt(UINT32 ftAddr, UINT32 dstAddr, UINT32 length);
BOOL NvtSvm_tran32bFt(UINT32 ftAddr, UINT32 dstAddr, UINT32 length);
BOOL NvtSvm_tran16bAlpha(UINT32 alphaAddr, UINT32 dstAddr, UINT32 length);
BOOL NvtSvm_tran32bAlpha(UINT32 alphaAddr, UINT32 dstAddr, UINT32 length);
BOOL NvtSvm_tran16bRho(UINT32 rhoAddr, UINT32 dstAddr, UINT32 length);
BOOL NvtSvm_tran32bRho(UINT32 rhoAddr, UINT32 dstAddr, UINT32 length);
BOOL NvtSvm_tranRsltTo16b(UINT32 rsltAddr, UINT32 dstAddr, UINT32 length);
BOOL NvtSvm_tranRsltTo32b(UINT32 rsltAddr, UINT32 dstAddr, UINT32 length);

BOOL NvtSvm_Process(SVM_OPMODE mode, SVM_PARAM svmParms);
BOOL NvtSvm_SvmProc(SVM_KER_TYPE svmKerType, SVM_SV_SIZE svSize, UINT32 uiF_Num, SVM_KERL_PARM svmKerlParms, SVM_DMAIO_ADDR dmaIO_Addr);
BOOL NvtSvm_MultiLinearSvmProc(SVM_SV_SIZE svSize, UINT32 uiF_Num, SVM_KERL_PARM svmKerlParms, SVM_DMAIO_ADDR dmaIO_Addr);
BOOL NvtSvm_ReLUProc(SVM_RELU_TYPE reluType, SVM_SV_SIZE svSize, UINT32 uiF_Num, SVM_DMAIO_ADDR dmaIO_Addr);
BOOL NvtSvm_FullyConnectProc(SVM_SV_SIZE svSize, UINT32 uiF_Num, SVM_DMAIO_ADDR dmaIO_Addr);

#endif
