/**
    IPL calibration ctrl

    @file       ipl_cal_ctrl.h
    @ingroup    mILibCal

    Copyright   Novatek Microelectronics Corp. 2017.  All rights reserved.
*/
#ifndef _IPL_CAL_CTRL_H_
#define _IPL_CAL_CTRL_H_

#include "Type.h"
#include "ErrorNo.h"
#include "ipl_utility.h"
#include "ipl_cmd.h"
#include "ipl_calibration.h"
#include "ipl_cal_3a.h"

typedef struct {
	ER(*fp_open)(IPL_PROC_ID id);
	ER(*fp_close)(IPL_PROC_ID id);
	ER(*fp_prv)(IPL_PROC_ID id, CAL_PRV_PARAM *prv_param);
	ER(*fp_getraw)(IPL_PROC_ID id, CAL_GETRAW_CTRL_PARAM *getraw_ctrl_param, BOOL b_start_sie);
	ER(*fp_alg)(IPL_PROC_ID id);
} CAL_CTRL_OBJ, *pCAL_CTRL_OBJ;

extern pCAL_CTRL_OBJ cal_ctrl(void);
//@}
#endif

