/**
    Public header file for extend lib

    This file is the header file that define the API and data type for extend lib.

    @file       ipl_ext_drv_int.h
    @ingroup
    @note       Nothing (or anything need to be mentioned).

    Copyright   Novatek Microelectronics Corp. 2009.  All rights reserved.
*/
#ifndef _IPL_OBJ_DRV_H
#define _IPL_OBJ_DRV_H

//----------Header file include-------------------------------------------------
#include "pq_obj.h"

//----------Add extend lib drv tab here-----------------------------------------
/*
extern IPL_EXT_LIB_TAB *IPL_SAMPLE_FF_EXT_LIB_TAB(void);
extern IPL_EXT_LIB_TAB *IPL_DVCAM_SAMPLE_FF_EXT_LIB_TAB(void);
extern IPL_EXT_LIB_TAB *IPL_IMX377M_EVB_EXT_LIB_TAB(void);
extern IPL_EXT_LIB_TAB *IPL_IMX117_FF_EXT_LIB_TAB(void);
*/
extern void iq_param_init_sample_evb(UINT32 Id);
extern void iq_param_init_imx290_evb(UINT32 Id);
extern void iq_param_init_imx291_evb(UINT32 Id);
extern void iq_param_init_imx317_evb(UINT32 Id);
extern void iq_param_init_imx377_evb(UINT32 Id);
extern void iq_param_init_imx117_evb(UINT32 Id);
extern void iq_param_init_ov7251_id0_evb(UINT32 Id);
extern void iq_param_init_ov7251_id1_evb(UINT32 Id);
extern void iq_param_init_ov12895_evb(UINT32 Id);

extern void awb_get_param_sample_evb(UINT32* param);
extern void awb_get_param_imx290_evb(UINT32* param);
extern void awb_get_param_imx291_evb(UINT32* param);
extern void awb_get_param_imx317_evb(UINT32* param);
extern void awb_get_param_imx377_evb(UINT32* param);
extern void awb_get_param_imx117_evb(UINT32* param);
extern void awb_get_param_imx117_ykj(UINT32* param);
extern void awb_get_param_ov7251_evb(UINT32* param);
extern void awb_get_param_ov12895_evb(UINT32* param);

extern void ae_get_param_sample_evb(UINT32 *param);
extern void ae_get_param_imx290_evb(UINT32 *param);
extern void ae_get_param_imx291_evb(UINT32 *param);
extern void ae_get_param_imx377_evb(UINT32 *param);
extern void ae_get_param_imx117_evb(UINT32 *param);
extern void ae_get_param_imx317_evb(UINT32 *param);
extern void ae_get_param_ov7251_evb(UINT32 *param);
extern void ae_get_param_ov12895_evb(UINT32 *param);

#endif
