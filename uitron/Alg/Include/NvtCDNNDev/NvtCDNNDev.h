/**
    NVT CDNN library for development.

    Library of NVT CDNN (CEVA Deep Neural Network) functions.

    @file       NvtCDNNDev.h
    @ingroup    NvtCDNNDev

    Copyright   Novatek Microelectronics Corp. 2018.  All rights reserved.
*/
#ifndef _NVTCDNNDEV_H
#define _NVTCDNNDEV_H

// Include ---------------------------------------------------------------------

#include "Type.h"
#include "ImageStream.h"

// Functions -------------------------------------------------------------------

/**
    Calculate the required working buffer size.

    @return the required working buffer size.
*/
extern UINT32 cdnndev_app_calc_buf_size(VOID);
extern UINT32 cdnndev2_app_calc_buf_size(VOID);

/**
    Calculate the required working cache buffer size.

    @return the required working cache buffer size.
*/
extern UINT32 cdnndev_app_calc_cache_buf_size(VOID);
extern UINT32 cdnndev2_app_calc_cache_buf_size(VOID);

/**
    Init.

    @param[in] isf_out: Stream output port ID.
*/
extern VOID cdnndev_app_init(ISF_OUT isf_out);
extern VOID cdnndev2_app_init(ISF_OUT isf_out);

/**
    Process input data.

    @param[in] buf: working buffer.
    @param[in] cachebuf: working cache buffer.
*/
extern VOID cdnndev_app_process(MEM_RANGE *buf, MEM_RANGE *cachebuf);
extern VOID cdnndev2_app_process(MEM_RANGE *buf, MEM_RANGE *cachebuf);

#endif  //_NVTCDNNDEV_H
