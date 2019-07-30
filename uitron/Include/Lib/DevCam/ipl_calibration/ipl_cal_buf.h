/**
    IPL calibration working buffer

    this buffer control mechanism is for storing in/out image during IPL calibration.

    @file       ipl_cal_buf.h
    @ingroup    mILibCal

    Copyright   Novatek Microelectronics Corp. 2017.  All rights reserved.
*/
#ifndef _IPL_CAL_BUF_H_
#define _IPL_CAL_BUF_H_

#include "Type.h"
#include "ErrorNo.h"
#include "ipl_utility.h"


#define IPL_CAL_BUF_4WORDALIGN(x) IPL_ALIGN_ROUNDUP(x, 16)
typedef struct {
	ER(*fp_init)(UINT32 addr, UINT32 size);
	ER(*fp_uninit)(void);

	BOOL(*fp_isset_buf)(UINT32 cal_buf_id);
	ER(*fp_set_buf)(UINT32 cal_buf_id, UINT32 size);
	ER(*fp_get_buf)(UINT32 cal_buf_id, UINT32 *addr, UINT32 *size);
	ER(*fp_release_buf)(UINT32 cal_buf_id);

	BOOL(*fp_isset_buf_item)(UINT32 cal_buf_id, UINT32 cal_buf_item);
	ER(*fp_set_buf_item)(UINT32 cal_buf_id, UINT32 cal_buf_item, UINT32 addr, UINT32 size);
	ER(*fp_get_buf_item)(UINT32 cal_buf_id, UINT32 cal_buf_item, UINT32 *addr, UINT32 *size);

	ER(*fp_cal_get_buf_info)(UINT32 cal_buf_id, UINT32 *addr, UINT32 *size, UINT32 *buf_id_map);
} CAL_BUF_OBJ, *pCAL_BUF_OBJ;

extern pCAL_BUF_OBJ cal_buf(void);
//@}
#endif
