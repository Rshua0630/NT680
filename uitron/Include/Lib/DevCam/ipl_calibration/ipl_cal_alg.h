/**
    ipl calibration algorithm


    @file       ipl_cal_alg.h
    @ingroup    mILibCal

    Copyright   Novatek Microelectronics Corp. 2017.  All rights reserved.
*/
#ifndef _IPL_CAL_ALG_H_
#define _IPL_CAL_ALG_H_
#include "Type.h"
#include "ErrorNo.h"
#include "ipl_utility.h"
#include "ipl_calibration.h"
#include "ipl_cal_statlib.h"

/**
    calibration algorithm parameter
*/
// auto white balance (AWB) algorithm parameter
typedef struct {
	IPL_RAW_IMG *raw_info; ///< raw inforamtion
	CAL_ALG_AWB_SETTING setting;
} CAL_ALG_AWB_PARAM;

// defect pixel (DP) algorithm parameter
typedef enum {
	DP_PARAM_CHGFMT_SKIP,	///< not change format
	DP_PARAM_CHGFMT_AUTO,	///< only change format when dp count not overflow
	DP_PARAM_CHGFMT_FORCE,	///< force to change format

	ENUM_DUMMY4WORD(DP_PARAM_CHGFMT)
} DP_PARAM_CHGFMT;

typedef struct {
	IPL_RAW_IMG *raw_info; ///< raw inforamtion
	DP_SETTING setting;
	UINT32 max_dp_cnt;		///< Unit: pixel
	UINT32 ori_dp_cnt;
	UINT32 dp_pool_addr;    ///< defect pixel coordinate pool in DRAM
	DP_PARAM_CHGFMT b_chg_dp_format;	///< sort dp data and change format for SIE
	Coordinate sie_act_str;
	Coordinate sie_crp_str;
} CAL_ALG_DP_PARAM;

//  embedded color shading (ECS) algorithm parameter
typedef struct {
	IPL_RAW_IMG *raw_info; ///< raw inforamtion
	CAL_ALG_ECS_SETTING setting;
	UINT32 ecs_tbl_addr;    ///< ecs table in DRAM
	UINT32 y_tbl_addr; ///< ecs y value table in DRAM
} CAL_ALG_ECS_PARAM;

// ISO algorithm parameter
typedef struct {
	IPL_RAW_IMG *raw_info;  ///< raw inforamtion
	CAL_ALG_ISO_SETTING setting;
	CAL_AE_CONVERGE_RST ae_converge_rst; ///< ae converge result
} CAL_ALG_ISO_PARAM;

// lens center (LENSCEN) algorithm parameter
typedef struct {
	IPL_RAW_IMG *raw_info; ///< raw inforamtion
	CAL_ALG_LENSCEN_SETTING setting;
} CAL_ALG_LENSCEN_PARAM;

/**
    calibration algorithm result
*/
// auto white balance (AWB) algorithm result
typedef struct {
	UINT32 rgain; ///< rgain
	UINT32 bgain; ///< bgain
} CAL_ALG_AWB_RST;

// defect pixel (DP) algorithm result
typedef struct {
	UINT32 pixel_cnt;       ///< total defect pixel number
	UINT32 data_length;		///< dp data length
} CAL_ALG_DP_RST;

// embedded color shading (ECS) algorithm result
typedef struct {
	BOOL ecs_over_spec; ///< ecs value over spec
} CAL_ALG_ECS_RST;

// ISO algorithm result
typedef struct {
	UINT32 gain_ratio; ///< gain ratio
} CAL_ALG_ISO_RST;

// lens center (LENSCEN) algorithm result
typedef struct {
	UINT32 reliability; ///< 0~100 (%)
	iCoordinate center_sft; ///< lens center shift
} CAL_ALG_LENSCEN_RST;

/**
    calibration algorithm operation structure
*/
typedef struct {
	ER(*fp_awb_alg)(CAL_ALG_AWB_PARAM *awb_param, CAL_ALG_AWB_RST *awb_rst);
	ER(*fp_dp_alg)(CAL_ALG_DP_PARAM *dp_param, CAL_ALG_DP_RST *dp_rst);
	ER(*fp_ecs_alg)(CAL_ALG_ECS_PARAM *ecs_param, CAL_ALG_ECS_RST *ecs_rst);
	ER(*fp_iso_alg)(CAL_ALG_ISO_PARAM *iso_param, CAL_ALG_ISO_RST *iso_rst);
	ER(*fp_lenscen_alg)(CAL_ALG_LENSCEN_PARAM *lenscen_param, CAL_ALG_LENSCEN_RST *lenscen_rst);
	CAL_STATUS(*fp_awb_chk_rst)(CAL_ALG_AWB_PARAM *awb_param, CAL_ALG_AWB_RST *awb_rst);
	CAL_STATUS(*fp_dp_chk_rst)(CAL_ALG_DP_PARAM *dp_param, CAL_ALG_DP_RST *dp_rst);
	CAL_STATUS(*fp_ecs_chk_rst)(CAL_ALG_ECS_PARAM *ecs_param, CAL_ALG_ECS_RST *ecs_rst);
	CAL_STATUS(*fp_iso_chk_rst)(CAL_ALG_ISO_PARAM *iso_param, CAL_ALG_ISO_RST *iso_rst);
	CAL_STATUS(*fp_lenscen_chk_rst)(CAL_ALG_LENSCEN_PARAM *lenscen_param, CAL_ALG_LENSCEN_RST *lenscen_rst);
} CAL_ALG_OBJ, *pCAL_ALG_OBJ;

extern pCAL_ALG_OBJ cal_alg(void);
//@}
#endif
