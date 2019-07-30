/**
    NVT CDNN application.

    Application of NVT CDNN (CEVA Deep Neural Network) functions.

    @file       alg_CDNN_PhotoFuncInfo.h
    @ingroup    NvtCDNN

    Copyright   Novatek Microelectronics Corp. 2018.  All rights reserved.
*/
#ifndef _ALG_CDNN_PHOTOFUNCINFO_H
#define _ALG_CDNN_PHOTOFUNCINFO_H

// Include ---------------------------------------------------------------------

#include "Type.h"

// Definitions -----------------------------------------------------------------

/**
    Network list.

    @note       align to enum values in NvtCDNN.h CDNN_NET.
*/
#define CDNN_GOOGLENET                  0
#define CDNN_FRCNN_RESNET50             1
#define CDNN_MOBILENET_SSD              2

/**
    CDNN creation parameters.
*/
#define CDNN_USE_DRAM2                  ENABLE
#define CDNN_DRAM2_ADDR                 (_BOARD_EXTDRAM_ADDR_ + 0x00100000) // offset 0x00100000 for CDNN may access (CDNN_DRAM2_ADDR - 0x10)
#define CDNN2_DRAM2_ADDR                (_BOARD_EXTDRAM_ADDR_ + 0x10000000)


/**
    CDNN 1.
*/
// CDNN_GOOGLENET, CDNN_FRCNN_RESNET50
#define CDNN_INPUT_WIDTH                224
#define CDNN_INPUT_HEIGHT               224

// CDNN_MOBILENET_SSD
//#define CDNN_INPUT_WIDTH                300
//#define CDNN_INPUT_HEIGHT               300

/**
    CDNN 2.
*/
// CDNN_GOOGLENET, CDNN_FRCNN_RESNET50
#define CDNN2_INPUT_WIDTH               224
#define CDNN2_INPUT_HEIGHT              224

// Functions -------------------------------------------------------------------

VOID   alg_CDNNInit             (MEM_RANGE *buf, MEM_RANGE *cachebuf);
VOID   alg_CDNNProcess          (MEM_RANGE *buf, MEM_RANGE *cachebuf);
UINT32 alg_CDNNCalcBufSize      (VOID);
UINT32 alg_CDNNCalcCacheBufSize (VOID);

VOID   alg_CDNN2Init            (MEM_RANGE *buf, MEM_RANGE *cachebuf);
VOID   alg_CDNN2Process         (MEM_RANGE *buf, MEM_RANGE *cachebuf);
UINT32 alg_CDNN2CalcBufSize     (VOID);
UINT32 alg_CDNN2CalcCacheBufSize(VOID);

#endif  //_ALG_CDNN_PHOTOFUNCINFO_H
