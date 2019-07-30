/*
    IPL calibration ext setting table

    @file       ipl_cal_setting_sample_ff.c
    @ingroup    mILibCal
    @note       Nothing.

    Copyright   Novatek Microelectronics Corp. 2017.  All rights reserved.
*/
#include "ipl_cal_setting_sample_ff_int.h"

#if 0
- static
#endif
/**
    calibration alg info
*/
static CAL_ALG_AWB_SETTING alg_setting_awb = {
	{512, 460}, 10, 3, 3
};

static CAL_ALG_DP_SETTING alg_setting_dp = {
	{100, 100, CAL_END},        // max_cnt_ratio
	{20, 32, 32, {0, 0, 0, 0}}, // bright_set
	{12, 32, 32, {0, 0, 0, 0}}, // dark_set
};

static CAL_ALG_ECS_SETTING alg_setting_ecs = {
	70, ecs65x65, ECS_D2F8, NULL,
};

static CAL_ALG_ISO_SETTING alg_setting_iso = {
	{1000}, 15,
};

static CAL_ALG_LENSCEN_SETTING alg_setting_lenscen = {
	75, 10, 200, 20, 50, {50, 50},
};

/**
    calibration ae info
*/
static BOOL cal_ae_valid_window[CAL_CA_WIN_X * CAL_CA_WIN_Y] = {
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // 0
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // 1
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // 2
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // 3
	0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, // 4
	0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, // 5
	0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, // 6
	0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, // 7
	0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, // 8
	0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, // 9
	0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, // 10
	0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, // 11
	0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, // 12
	0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, // 13
	0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, // 14
	0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, // 15
	0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, // 16
	0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, // 17
	0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, // 18
	0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, // 19
	0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, // 20
	0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, // 21
	0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, // 22
	0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, // 23
	0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, // 24
	0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, // 25
	0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, // 26
	0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, // 27
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // 28
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // 29
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // 30
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // 31
};

static CAL_AE_CONVERGE_PARAM ae_conv_param_awb = {
	4000,           // gain (x1000)
	5000,          // exp_time (us)
	40,             // target_lum
	3,              // tolerance
	10,             // times
	CAL_AE_ADJ_GAINRATIO, // adjust (CAL_AE_ADJ)
	{
		{ -50, -30, -15, -5, -3, 3, 5, 15, 30, 50}, // CAL_AE_CONV_STEP.lum_diff (current lum - target lum)
		{  15,  10,   5,  3,  1, 1, 3,  5, 10, 15}, // CAL_AE_CONV_STEP.ratio (units: %, range: 0~100)
	},
	&cal_ae_valid_window[0],
	{0},
	{0},
	NULL,
};

static CAL_AE_CONVERGE_PARAM ae_conv_param_dpd = {
	4000,           // gain (x1000)
	5000,          // exp_time (us)
	40,             // target_lum
	3,              // tolerance
	10,             // times
	CAL_AE_ADJ_EXPT,// adjust (CAL_AE_ADJ)
	{
		{ -50, -30, -15, -5, -3, 3, 5, 15, 30, 50}, // CAL_AE_CONV_STEP.lum_diff (current lum - target lum)
		{  15,  10,   5,  3,  1, 1, 3,  5, 10, 15}, // CAL_AE_CONV_STEP.ratio (units: %, range: 0~100)
	},
	&cal_ae_valid_window[0],
	{0},
	{0},
	NULL,
};

static CAL_AE_CONVERGE_PARAM ae_conv_param_dpb = { // dark environment
	4000,           // gain (x1000)
	5000,          // exp_time (us)
	1,              // target_lum
	3,              // tolerance
	1,              // times
	CAL_AE_ADJ_NONE,// adjust (CAL_AE_ADJ)
	{
		{ -50, -30, -15, -5, -3, 3, 5, 15, 30, 50}, // CAL_AE_CONV_STEP.lum_diff (current lum - target lum)
		{  15,  10,   5,  3,  1, 1, 3,  5, 10, 15}, // CAL_AE_CONV_STEP.ratio (units: %, range: 0~100)
	},
	&cal_ae_valid_window[0],
	{0},
	{0},
	&ae_conv_param_dpd,
};

static CAL_AE_CONVERGE_PARAM ae_conv_param_ecs = {
	4000,           // gain (x1000)
	5000,          // exp_time (us)
	40,             // target_lum
	3,              // tolerance
	10,             // times
	CAL_AE_ADJ_GAINRATIO, // adjust (CAL_AE_ADJ)
	{
		{ -50, -30, -15, -5, -3, 3, 5, 15, 30, 50}, // CAL_AE_CONV_STEP.lum_diff (current lum - target lum)
		{  15,  10,   5,  3,  1, 1, 3,  5, 10, 15}, // CAL_AE_CONV_STEP.ratio (units: %, range: 0~100)
	},
	&cal_ae_valid_window[0],
	{0},
	{0},
	NULL,
};

static CAL_AE_CONVERGE_PARAM ae_conv_param_iso = {
	4000,           // gain (x1000)
	5000,          // exp_time (us)
	40,             // target_lum
	1,              // tolerance
	20,             // times
	CAL_AE_ADJ_GAINRATIO, // adjust (CAL_AE_ADJ)
	{
		{ -50, -30, -15, -5, -3, 3, 5, 15, 30, 50}, // CAL_AE_CONV_STEP.lum_diff (current lum - target lum)
		{  15,  10,   5,  2,  1, 1, 2,  5, 10, 15}, // CAL_AE_CONV_STEP.ratio (units: %, range: 0~100)
	},
	&cal_ae_valid_window[0],
	{0},
	{0},
	NULL,
};

static CAL_AE_CONVERGE_PARAM ae_conv_param_lenscen = {
	4000,           // gain (x1000)
	5000,          // exp_time (us)
	40,             // target_lum
	3,              // tolerance
	10,             // times
	CAL_AE_ADJ_GAINRATIO, // adjust (CAL_AE_ADJ)
	{
		{ -50, -30, -15, -5, -3, 3, 5, 15, 30, 50}, // CAL_AE_CONV_STEP.lum_diff (current lum - target lum)
		{  15,  10,   5,  3,  1, 1, 3,  5, 10, 15}, // CAL_AE_CONV_STEP.ratio (units: %, range: 0~100)
	},
	&cal_ae_valid_window[0],
	{0},
	{0},
	NULL,
};

//#NT#2017/05/24#Silvia Wu -begin
//#NT# need to check different condition  use *pnext?
//#NT#2017/05/24#Silvia Wu -end
/**
    calibration setting

    please follow CAL_ITEM sorting !!!!!!!
*/
static CAL_SETTING_INFO g_setting_table_sample_ff[CAL_ITEM_MAX_NUM] = {
	{&alg_setting_awb,      &ae_conv_param_awb,     {SENSOR_MODE_1, SENSOR_MODE_2, SENSOR_MODE_UNKNOWN}}, // CAL_ITEM_AWB
	{&alg_setting_dp,       &ae_conv_param_dpb,     {SENSOR_MODE_1, SENSOR_MODE_UNKNOWN}}, // CAL_ITEM_DP
	{&alg_setting_ecs,      &ae_conv_param_ecs,     {SENSOR_MODE_1, SENSOR_MODE_UNKNOWN}}, // CAL_ITEM_ECS
	{&alg_setting_iso,      &ae_conv_param_iso,     {SENSOR_MODE_1, SENSOR_MODE_UNKNOWN}}, // CAL_ITEM_ISO
	{&alg_setting_lenscen,  &ae_conv_param_lenscen, {SENSOR_MODE_1, SENSOR_MODE_UNKNOWN}}, // CAL_ITEM_LENSCEN
	{NULL,                  NULL,                   {SENSOR_MODE_UNKNOWN}}, // CAL_ITEM_USER1
	{NULL,                  NULL,                   {SENSOR_MODE_UNKNOWN}}, // CAL_ITEM_USER2
	{NULL,                  NULL,                   {SENSOR_MODE_UNKNOWN}}, // CAL_ITEM_USER3
	{NULL,                  NULL,                   {SENSOR_MODE_UNKNOWN}}, // CAL_ITEM_USER4
	{NULL,                  NULL,                   {SENSOR_MODE_UNKNOWN}}, // CAL_ITEM_USER5
};

static CAL_SETTING_INFO *get_cal_setting_info_sample_ff(CAL_ITEM cal_item)
{
	return &g_setting_table_sample_ff[cal_item];
}

/**
    calibration ctrl info
*/
static OB_INFO g_ob_info = {
	NULL,
	0x00,
};

static CA_INFO g_ca_info = {
	TRUE,
	{0},
//	32,
//	32,
	TRUE,
	{0, 4095},
	{0, 4095},
	{0, 4095},
	{0, 4095},
	FALSE,
};

static CAL_CTRL_SETTING_SIE g_sie_info = {
	4,
	&g_ob_info,
	&g_ca_info,
	NULL,
};

static CAL_CTRL_GROUP g_group_info = {
	IPL_BIT_ID_NONE,
	{2, 2},
};

static CAL_CTRL_SETTING_INFO g_ctrl_info_sample_ff = {
	&g_sie_info,
	&g_group_info,
};

static CAL_CTRL_SETTING_INFO *get_cal_ctrl_info_sample_ff(void)
{
	return &g_ctrl_info_sample_ff;
}

static CAL_EXT_SETTING_OBJ m_ext_setting_obj = {
	get_cal_setting_info_sample_ff,
	get_cal_ctrl_info_sample_ff,
};

#if 0
- extern
#endif

pCAL_EXT_SETTING_OBJ ipl_cal_ext_setting_sample(void)
{
	return &m_ext_setting_obj;
}

