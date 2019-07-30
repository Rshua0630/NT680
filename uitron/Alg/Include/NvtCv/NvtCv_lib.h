/**
    NVT CV library functions.

    Definitions of NVT CV library functions.

    @file       NvtCvFuns.h
    @ingroup    NvtCv
    @note       Nothing.

    Copyright   Novatek Microelectronics Corp. 2017.  All rights reserved.
*/
#ifndef _NVTCV_LIB_H
#define _NVTCV_LIB_H

#include "Type.h"
#include "NvtCvFuns.h"

typedef enum _NVTCV_SENDTO {
	NVTCV_SENDTO_DSP1,
	NVTCV_SENDTO_DSP2,
	ENUM_DUMMY4WORD(NVTCV_SENDTO)
} NVTCV_SENDTO;

/**
    Return status.
*/
typedef enum {
	NVTCV_STA_OK                = 0,    ///< the status is OK
	NVTCV_STA_ERROR             = -1,   ///< some error occurred
	NVTCV_STA_INVALID_FUN       = -2,   ///< invalid function enum

	ENUM_DUMMY4WORD(NVTCV_STA)
} NVTCV_STA;

/**
    Application preprocess callback
*/
typedef void (*NVTCV_PREPROCESS_CB)(UINT32 uiAddrPrms);

/**
    Application postprocess callback
*/
typedef void (*NVTCV_POSTPROCESS_CB)(UINT32 uiAddrPrms);

/**
    Information of callback function.
*/
typedef struct _NVTCV_CB_INFO {
	NVTCV_FUN nvtCvFun;                     ///< function enum
	NVTCV_PREPROCESS_CB  pfPreprocessCb;    ///< preprocess callback
	NVTCV_POSTPROCESS_CB pfPostprocessCb;   ///< postprocess callback
} NVTCV_CB_INFO;


/**
   Install required system resource.
*/
extern void NvtCv_InstallID(void) _SECTION(".kercfg_text");

/**
   Query working memory size.
*/
extern UINT32 NvtCv_GetWrkMemSize(void);

/**
    Set preprocess/postprocess callback, which is called in NvtCv_Preprocess/NvtCv_Postprocess

    @param[in] pInfo: pointer of callback info

    @return the result of setting callback
*/
extern NVTCV_STA NvtCv_SetProcessCb(NVTCV_CB_INFO *pfunCb);

/**
   Open NvtCv
*/
extern void NvtCv_Open(NVTCV_SENDTO dspCore, UINT32 uiAddrWorkMem);

/**
   Application preprocess
*/
extern void NvtCv_Preprocess(NVTCV_FUN nvtCvFun, UINT32 uiAddrPrms);

/**
   NvtCv function execution

   Note
   1. Both the input image(s) and the structure of function parameter(s) MUST be written back if they are in cache.
   2. Nested NvtCv function execution is not allowed
   3. NvtCv is non-blocking execution, thus user can do other things after a NvtCv function is issued.
*/
extern void NvtCv_Execute(NVTCV_FUN nvtCvFun, UINT32 uiAddrPrms, NVTCV_SENDTO dspCore);

/**
   Wait NvtCv execution done
*/
extern void NvtCv_WaitExecuteDone(NVTCV_SENDTO dspCore);

/**
   Application postprocess
*/
extern void NvtCv_Postprocess(NVTCV_FUN nvtCvFun, UINT32 uiAddrPrms);

/**
   Close NvtCv
*/
extern void NvtCv_Close(NVTCV_SENDTO dspCore);


#endif

