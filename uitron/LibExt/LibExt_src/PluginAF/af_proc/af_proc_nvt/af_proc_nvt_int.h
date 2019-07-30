/**
 * Novatek AF sample code.
 * 
 * Reference AF sample.
 * 
 * @file af_proc_nvt_int.h
 * @note Nothing.
 *
 * Copyright (C) 2017 Novatek Microelectronics Corp. All rights reserved.
 *
 */

#ifndef _AF_PROC_NVT_INT_H_
#define _AF_PROC_NVT_INT_H_

#include "Type.h"
#include "ipl_alg_infor.h"
#include "af_api.h"


//=============================================================================
// compile switch
//=============================================================================
#define USE_NVT_AF_ALG       ENABLE     // DISABLE
#define AF_FACE_DETECTION    DISABLE    // always disable because of HW bug


//=============================================================================
// constant define
//=============================================================================
#define AF_INDP_WIN_W_MAX           510
#define AF_INDP_WIN_H_MAX           510
#define AF_VA_DIV                   3


//=============================================================================
// struct & flag definition
//=============================================================================
#define PRINT_AF(mode, fmt, args...)  { if (mode & pinfo->AfSubInfo.uiDbgMsg) DBG_MSG(fmt, ## args); }


typedef enum {
    VA_GRP1_H = 1,
    VA_GRP1_V = 2,
    VA_GRP2_H = 3,
    VA_GRP2_V = 4,
    VA_GRP1   = 5,
    VA_GRP2   = 6,
    ENUM_DUMMY4WORD(VA_GRP)
} VA_GRP;


typedef struct {
    UINT32 uiId;
    BOOL bIsInit;

    AF_SUB_INFO AfSubInfo;

    IPL_SET_VA_DATA VaInfo;
    AF_DRV_MANUAL ManualSearchRange;

    BOOL bSetCropWin;
    VACROP CropWinPos;

    INT32 iFrameNum;
} AF_INFO;


#endif    // _AF_PROC_NVT_INT_H_

