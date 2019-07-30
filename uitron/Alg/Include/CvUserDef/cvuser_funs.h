/**
    CvUserDef library functions.

    User-Defined CV functions.

    @file       cvuser_funs.h
    @ingroup    CvUserDef
    @note       Nothing.

    Copyright   Novatek Microelectronics Corp. 2017.  All rights reserved.
*/
#ifndef _CVUSER_FUNS_H
#define _CVUSER_FUNS_H

#include "Type.h"
#include "NvtIpcAPI.h"

/**
    Function list.
*/
typedef enum {
	CVUSER_YUV2RGB,                         ///< YUV-to-RGB color transform
	CVUSER_CDNN,                            ///< CEVA CDNN initialization + execution + finalization
	CVUSER_CDNN_INIT,                       ///< CEVA CDNN initialization
	CVUSER_CDNN_EXEC,                       ///< CEVA CDNN execution
	CVUSER_CDNN_FINAL,                      ///< CEVA CDNN finalization
	CVUSER_RESIZE,                          ///< resizing example

	CVUSER_FUN_MAX,
	ENUM_DUMMY4WORD(CVUSER_FUN)
} CVUSER_FUN;

// Common types ----------------------------------------------------------------
/**
    IPC destination core.
*/
typedef enum {
	CVUSER_SENDTO_DSP1 = NVTIPC_SENDTO_CORE3,///< sending the request to DSP1 (core 3)
	CVUSER_SENDTO_DSP2 = NVTIPC_SENDTO_CORE4,///< sending the request to DSP2 (core 4)
	ENUM_DUMMY4WORD(CVUSER_SENDTO)
} CVUSER_SENDTO;

/**
    Return status.
*/
typedef enum {
	CVUSER_STA_OK            = 0,           ///< the status is OK
	CVUSER_STA_ERROR         = -1,          ///< some error occurred
	CVUSER_STA_INVALID_FUN   = -2,          ///< invalid function enum
	ENUM_DUMMY4WORD(CVUSER_STA)
} CVUSER_STA;

/**
    Data dimensions.
*/
typedef struct {
	INT32 num;                              ///< Caffe blob: batch size
	INT32 channels;                         ///< Caffe blob: number of channels
	INT32 height;                           ///< Caffe blob: height
	INT32 width;                            ///< Caffe blob: width
	INT32 lineoffset;                       ///< distance between two lines (rows); unit: byte
} CVUSER_SHAPE;

// YUV-to-RGB color transformation ---------------------------------------------
typedef struct {
	UINT8 *input_y;                         ///< address of input Y channel
	UINT8 *input_uv;                        ///< address of input U/V channel
	INT16 *output_r;                        ///< address of output R channel
	INT16 *output_g;                        ///< address of output G channel
	INT16 *output_b;                        ///< address of output B channel

	CVUSER_SHAPE input_shape;               ///< input data dimensions

	UINT8 reshape;                          ///< 1: reshape and color transform; 0: color transform only
} CVUSER_YUV2RGB_PRMS;

// Functions -------------------------------------------------------------------
extern CVUSER_STA cvuser_yuv2rgb_process        (CVUSER_YUV2RGB_PRMS *prms, CVUSER_SENDTO dsp_core);
extern CVUSER_STA cvuser_yuv2rgb_process_nowait (CVUSER_YUV2RGB_PRMS *prms, CVUSER_SENDTO dsp_core);
extern CVUSER_STA cvuser_yuv2rgb_waitdone       (CVUSER_YUV2RGB_PRMS *prms, CVUSER_SENDTO dsp_core);

#endif  //_CVUSER_FUNS_H
