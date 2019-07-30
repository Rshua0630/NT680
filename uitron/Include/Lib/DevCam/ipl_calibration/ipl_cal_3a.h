/**
    ipl calibration 3A

    this calibration 3A is for getting proper cal. condition.

    @file       ipl_cal_3a.h
    @ingroup    mILibCal

    Copyright   Novatek Microelectronics Corp. 2017.  All rights reserved.
*/
#ifndef _IPL_CAL_3A_H_
#define _IPL_CAL_3A_H_
#include "Type.h"
#include "ErrorNo.h"
#include "ipl_calibration.h"

#define CAL_CA_WIN_X 32
#define CAL_CA_WIN_Y 32
#define CAL_LA_WIN_X 32
#define CAL_LA_WIN_Y 32

typedef enum {
	CAL_AE_CONVERT_GAINRATIO    = 0,
	CAL_AE_CONVERT_EXPT         = 1,
	CAL_AE_CONVERT_BINNING      = 2,
	CAL_AE_CONVERT_LUM          = 3,
	CAL_AE_CONVERT_MAX          = 4,
	ENUM_DUMMY4WORD(CAL_AE_CONVERT)
} CAL_AE_CONVERT;

typedef struct {
	UINT32 gain_ratio; ///< gain (1X = 1 x 1000 = 1000)
	UINT32 exp_time;
	UINT32 lum; // 8 bits
} CAL_AE_PARAM;

typedef struct {
	CAL_AE_PARAM ae_param;
	UINT32 binning; // get from sensor driver
} CAL_AE_PRESET_PARAM;

typedef struct {
	ER(*fp_preset)(IPL_PROC_ID id, CAL_AE_PRESET_PARAM *ae_preset_param); // set last prview parameter, ae_preset_param set NULL to get current ae info
	ER(*fp_open)(IPL_PROC_ID id, AE_OBJ_INFO *ae_obj_info); // ae_obj_info set NULL to use default
	ER(*fp_convert_param)(IPL_PROC_ID id, CAL_AE_PARAM *ae_input_param, CAL_AE_PARAM *ae_output_param, CAL_AE_ADJ adjust, CAL_ITEM cal_item); // need to preset reference parameter first
	ER(*fp_wait_converge)(IPL_PROC_ID id, CAL_AE_CONVERGE_PARAM *converge_param, CAL_AE_CONVERGE_RST *converge_rst);
	ER(*fp_close)(IPL_PROC_ID id);
} CAL_AE_OBJ, *pCAL_AE_OBJ;

typedef struct {
	UINT32 value;
} CAL_AWB_PARAM;

typedef struct {
	ER(*fp_awb)(CAL_AWB_PARAM *awb_info);
} CAL_AWB_OBJ, *pCAL_AWB_OBJ;

typedef struct {
	pCAL_AE_OBJ(*fp_ae)(void);
	pCAL_AWB_OBJ(*fp_awb)(void);
} CAL_3A_OBJ, *pCAL_3A_OBJ;

extern pCAL_3A_OBJ cal_3a(void);
//@}
#endif
