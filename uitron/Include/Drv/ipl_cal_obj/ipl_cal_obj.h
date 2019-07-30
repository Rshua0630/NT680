/**
    Public header file for ipl cal obj

    This file is the header file that define the API and data type for ipl ctrl obj.

    @file       ipl_cal_obj.h
    @ingroup
    @note       Nothing (or anything need to be mentioned).

    Copyright   Novatek Microelectronics Corp. 2017.  All rights reserved.
*/
#ifndef _IPL_CAL_OBJ_H
#define _IPL_CAL_OBJ_H

// system
#include "Type.h"
#include "sensor.h"

#define IPL_CAL_MAX 8

/**
    ipl calibration extend library common interface
*/

typedef struct {
	// flow
	ER(*fp_open)(UINT32 id, void *open_param);
	ER(*fp_close)(UINT32 id);
	ER(*fp_prv)(UINT32 id, void *prv_param);
	ER(*fp_getraw)(UINT32 id, void *getraw_param, void *getraw_rst);
	ER(*fp_alg)(UINT32 id, void *alg_param, void *alg_rst);

	ER(*fp_get_data_buf_addr)(void *buf_info, UINT32 *addr);

	// get cal info & setting & rst
	ER(*fp_getcalrst)(void *rw_header_info, UINT32 *data_addr);
	ER(*fp_getcalrst_incal)(void *rw_header_info, UINT32 *data_addr);
	void*(*fp_ctrl)(void);
	void*(*fp_setting)(void);
} IPL_CAL_EXT_LIB_TAB;

typedef struct {
	IPL_CAL_EXT_LIB_TAB *cal_ext_lib_tab;
} IPL_CAL_HANDLE;

extern BOOL ipl_cal_ext_lib_init(UINT32 id, IPL_CAL_HANDLE *handle);

//@}
#endif
