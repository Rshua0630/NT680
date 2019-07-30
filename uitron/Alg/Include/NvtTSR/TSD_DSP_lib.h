#ifndef __TSD_DSP_LIB_H__
#define __TSD_DSP_LIB_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include <Type.h>
#include <Debug.h>
#include "FileSysTsk.h"
#include "GxImage.h"
#include "NvtCv_lib.h"
#include "tsr_lib.h"

#define NVTCV_TSDDSP_PRECIMODE        0X1;
#define NVTCV_TSDDSP_VERSION          0x20170921

typedef enum
{
    TSD_DSP_CFG_NONE = -1,
    TSD_DSP_CFG_PRECIMODE = 0,
    ENUM_DUMMY4WORD(TSD_DSP_CFG)
}TSD_DSP_CFG;

typedef struct _NVTCV_TSDDSP_PRMS
{
    UINT32 imgAddr;
	UINT32 tsdModelAddr;
    UINT32 ocrModelAddr;//add by SH00211,add ocr model address in prms struct
    UINT32 freeAddr;
    UINT32 height;
    UINT32 width;
    UINT32 lineOffset;
    UINT32 nObject;
    UINT32 scale;
    UINT32 nWindows;
    UINT32 preciMode;//add by SH00211,1:high precision mode,0:normal mode
    UINT32 tsAddr;
    UINT32 version;
}NVTCV_TSDDSP_PRMS;
#if TSR_READ_TSD_MODEL_FROM_SDCARD
VOID NvtCv_TSDDSP_Init(UINT32 buf, UINT32 cachebuf, UINT32 tsdModelAddr, UINT32 ocrModelAddr);
#else
VOID NvtCv_TSDDSP_Init(UINT32 buf, UINT32 cachebuf, UINT32 ocrModelAddr);
#endif
void NvtCv_TSDDSP_Preprocess(UINT32 uiPrams);
void NvtCv_TSDDSP_Postprocess(UINT32 uiPrams);
VOID NvtCv_TSDDSP_Process(VOID);

UINT32 NvtCv_TSDDSP_TSDMaxModelSize(void);
UINT32 NvtCv_TSDDSP_OCRMaxModelSize(void);
UINT32 NvtCv_TSDDSP_MaxModelSize(void);
UINT32 NvtCv_TSDDSP_CalcCacheBuffSize(void);
UINT32 NvtCv_TSDDSP_CalcBuffSize(void);
UINT32 NvtCv_TSDDSP_GetRsltTS(TRAFFIC_SIGN *TS, URECT* TargetCoord);

VOID TSR_SetConfig(TSD_DSP_CFG cfg, INT32 value);
INT32 TSR_GetConfig(TSD_DSP_CFG cfg);

UINT32 TSR_GetLog(UINT32 *bufAddr);

#endif
