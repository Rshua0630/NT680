#ifndef __RESIZE_LIB_H__
#define __RESIZE_LIB_H__
#include "GxImage.h"
#include "Type.h"
#include "cvuser_lib.h"

VOID NvtResize_Init(MEM_RANGE *buf, MEM_RANGE *cache);

UINT32 NvtResize_CalBufSize(VOID);

UINT32 NvtResize_CalCacheBufSize(VOID);

VOID NvtResize_Process(IMG_BUF* pInput, IMG_BUF* pOutput, CVUSER_SENDTO dspId);

VOID NvtResize_UnInit(VOID);
#endif
