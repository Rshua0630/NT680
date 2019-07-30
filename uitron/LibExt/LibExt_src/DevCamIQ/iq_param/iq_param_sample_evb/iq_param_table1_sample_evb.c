#ifndef _IQ_PARAM_TABLE1_SAMPLE_EVB_C
#define _IQ_PARAM_TABLE1_SAMPLE_EVB_C

#include "iq_param_common_sample_evb.h"
#include "iq_param_sqnr1_sample_evb.c"

////////////////////////////////
//         Engine Ctrl         /
////////////////////////////////
static ENGINE_CTRL e_ctrl =
{
    {ECS_OFF, NON_SUPPORT},
    {VIG_OFF, 100},//0~200,default:100
    {GDC_OFF, 200},//0~200,default:200(100, same with OFF)
    {CAC_OFF, NON_SUPPORT},
    {CCM_ON, NON_SUPPORT},
    {CC3D_OFF, NON_SUPPORT},
    {CCTRL_ON, 100, 100, 100},//sat:0~200;con:0~200;bri:0~200,fedault:100
    {HSV_ON, 0},//H:0~360,default:0
    {GAMMA_ON, NON_SUPPORT},
    {Y_CURVE_OFF, NON_SUPPORT},
    {EDGE_ON, 100},//0~200,default:100
    {OUTL_ON, NON_SUPPORT},
    {RANGE_NR_ON, 100},//0~200,default:100
    {GBAL_ON, NON_SUPPORT},
    {LPF_ON, NON_SUPPORT},
    {LCA_ON, NON_SUPPORT},
    {LCA_C_ON, NON_SUPPORT},
    {LCA_Y_ON, NON_SUPPORT},
    {SQUARE_ON, NON_SUPPORT},
    {TNR_ON, 5},//0~10,default:5
    {LCE_ON, NON_SUPPORT},
    {RESERVED_OFF, NON_SUPPORT},
    {RESERVED_OFF, NON_SUPPORT},
    {RESERVED_OFF, NON_SUPPORT},
    {RESERVED_OFF, NON_SUPPORT},
    {RESERVED_OFF, NON_SUPPORT},
    {RESERVED_OFF, NON_SUPPORT},
    {RESERVED_OFF, NON_SUPPORT},
    {RESERVED_OFF, NON_SUPPORT},
    {RESERVED_OFF, NON_SUPPORT},
    {RESERVED_OFF, NON_SUPPORT},
    {RESERVED_OFF, NON_SUPPORT},
};

////////////////////////////////
//             SIE             /
////////////////////////////////

//////////////////////
//**  SIE-OB
//////////////////////
static UINT32 ob[TOTAL_GAIN_NUM][4] =
{
    {200, 200, 200, 200},
    {200, 200, 200, 200},
    {200, 200, 200, 200},
    {200, 200, 200, 200},
    {200, 200, 200, 200},
    {200, 200, 200, 200},
    {200, 200, 200, 200},
    {200, 200, 200, 200},
    {200, 200, 200, 200},
    {200, 200, 200, 200},
    {200, 200, 200, 200},
    {200, 200, 200, 200},
};

////////////////////////////////
//             RHE             /
////////////////////////////////

//////////////////////
//**  RHE-WDR
//////////////////////
static UINT8 wdr_strength[21] = //LV0~20, range: 0 ~ 128, 1x = 64;
{
    64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64
};

static IQS_WDR_LCE_OFFSET wdr_lce_offset =
{
    5,  //for WDR_AUTO Mode
    10, //for WDR_MANUAL Mode
};

static SR_WDR_GCE_PARAM gce_strength[TOTAL_GAIN_NUM] = //sth_range: 0 ~ 128, 0 = 1X
{   //only work on WDR_AUTO and WDR_MANUAL Mode
    //dark, bright
    {0, 0},
    {0, 0},
    {0, 0},
    {0, 0},
    {0, 0},
    {0, 0},
    {0, 0},
    {0, 0},
    {0, 0},
    {0, 0},
    {0, 0},
    {0, 0},
};

//////////////////////
//**  RHE-LCE
//////////////////////
static IQS_LCE lce_strength[TOTAL_GAIN_NUM] =
{
    //level(range: 0 ~ 128, 0 = OFF), clamp (range: 64 ~ 255, 64 = OFF)
    {0, 255},
    {0, 255},
    {0, 255},
    {0, 255},
    {0, 255},
    {0, 255},
    {0, 255},
    {0, 255},
    {0, 255},
    {0, 255},
    {0, 255},
    {0, 255},
};

////////////////////////////////
//             IFE             /
////////////////////////////////

//////////////////////
//**  IFE-Outlier
//////////////////////
static UINT32 outl_bri[TOTAL_GAIN_NUM][5] =
{
    {1280, 1868, 3084, 4095, 4095},
    {1184, 1728, 2853, 3942, 4095},
    {1184, 1728, 2853, 3942, 4095},
    {1184, 1728, 2853, 3942, 4095},
    {1184, 1728, 2853, 3942, 4095},
    {1184, 1728, 2853, 3942, 4095},
    { 710, 1036, 1711, 2365, 2457},
    { 710, 1036, 1711, 2365, 2457},
    { 710, 1036, 1711, 2365, 2457},
    { 710, 1036, 1711, 2365, 2457},
    { 710, 1036, 1711, 2365, 2457},
    { 710, 1036, 1711, 2365, 2457},
};

static UINT32 outl_dark[TOTAL_GAIN_NUM][5] =
{
    {1280, 1868, 3084, 4095, 4095},
    {1184, 1728, 2853, 3942, 4095},
    {1184, 1728, 2853, 3942, 4095},
    {1184, 1728, 2853, 3942, 4095},
    {1184, 1728, 2853, 3942, 4095},
    {1184, 1728, 2853, 3942, 4095},
    { 710, 1036, 1711, 2365, 2457},
    { 710, 1036, 1711, 2365, 2457},
    { 710, 1036, 1711, 2365, 2457},
    { 710, 1036, 1711, 2365, 2457},
    { 710, 1036, 1711, 2365, 2457},
    { 710, 1036, 1711, 2365, 2457},
};

static UINT32 outl_cnt[TOTAL_GAIN_NUM][2] =
{
    {1, 0},
    {1, 0},
    {1, 0},
    {1, 0},
    {1, 0},
    {1, 0},
    {1, 0},
    {1, 0},
    {1, 0},
    {1, 0},
    {1, 0},
    {1, 0},
};

static IFE_OUTL outl_set[TOTAL_GAIN_NUM] =
{                                                       //CrsChanEn, Weight, DarkOfs, BrightOfs
	{(UINT32*)outl_bri, (UINT32*)outl_dark, (UINT32*)outl_cnt, 0, 0, 0, 0},
    {(UINT32*)outl_bri, (UINT32*)outl_dark, (UINT32*)outl_cnt, 0, 0, 0, 0},
    {(UINT32*)outl_bri, (UINT32*)outl_dark, (UINT32*)outl_cnt, 0, 0, 0, 0},
    {(UINT32*)outl_bri, (UINT32*)outl_dark, (UINT32*)outl_cnt, 0, 0, 0, 0},
    {(UINT32*)outl_bri, (UINT32*)outl_dark, (UINT32*)outl_cnt, 0, 0, 0, 0},
    {(UINT32*)outl_bri, (UINT32*)outl_dark, (UINT32*)outl_cnt, 0, 0, 0, 0},
    {(UINT32*)outl_bri, (UINT32*)outl_dark, (UINT32*)outl_cnt, 0, 0, 0, 0},
    {(UINT32*)outl_bri, (UINT32*)outl_dark, (UINT32*)outl_cnt, 0, 0, 0, 0},
    {(UINT32*)outl_bri, (UINT32*)outl_dark, (UINT32*)outl_cnt, 0, 0, 0, 0},
    {(UINT32*)outl_bri, (UINT32*)outl_dark, (UINT32*)outl_cnt, 0, 0, 0, 0},
    {(UINT32*)outl_bri, (UINT32*)outl_dark, (UINT32*)outl_cnt, 0, 0, 0, 0},
    {(UINT32*)outl_bri, (UINT32*)outl_dark, (UINT32*)outl_cnt, 0, 0, 0, 0},
};

//////////////////////
//**  IFE-Spatial
//////////////////////
static UINT32 spatial_w[TOTAL_GAIN_NUM][10] =
{
    {31, 29, 30, 27, 24, 23, 21, 18, 14, 11},
    {31, 29, 30, 27, 24, 23, 21, 18, 14, 11},
    {31, 30, 30, 28, 28, 26, 26, 23, 21, 20},
    {31, 30, 29, 28, 26, 24, 23, 22, 20, 18},
    {31, 28, 28, 28, 24, 24, 18, 18, 18, 16},
    {31, 23, 21, 19, 17, 15, 13, 10,  8,  6},
	{31, 24, 21, 15, 12,  8,  6,  4,  2,  1},
	{31, 24, 21, 15, 12,  8,  6,  4,  2,  1},
	{31, 24, 21, 15, 12,  8,  6,  4,  2,  1},
	{31, 24, 21, 15, 12,  8,  6,  4,  2,  1},
	{31, 24, 21, 15, 12,  8,  6,  4,  2,  1},
	{31, 24, 21, 15, 12,  8,  6,  4,  2,  1},
};

static Spatial_Filter spatial[TOTAL_GAIN_NUM] =
{
// sOnlyEn, sOnlyFiltLeng, pWeight
    {DISABLE, Filt_3x3, (UINT32*)spatial_w},
    {DISABLE, Filt_3x3, (UINT32*)spatial_w},
    {DISABLE, Filt_3x3, (UINT32*)spatial_w},
    {DISABLE, Filt_3x3, (UINT32*)spatial_w},
    {DISABLE, Filt_3x3, (UINT32*)spatial_w},
    {DISABLE, Filt_3x3, (UINT32*)spatial_w},
    {DISABLE, Filt_3x3, (UINT32*)spatial_w},
    {DISABLE, Filt_3x3, (UINT32*)spatial_w},
    {DISABLE, Filt_3x3, (UINT32*)spatial_w},
    {DISABLE, Filt_3x3, (UINT32*)spatial_w},
    {DISABLE, Filt_3x3, (UINT32*)spatial_w},
    {DISABLE, Filt_3x3, (UINT32*)spatial_w},
};

//////////////////////
//**  IFE-NLM
//////////////////////
static UINT8 range_ker_radius[TOTAL_GAIN_NUM][8] =
{
	{6, 13, 19, 26, 32, 38, 45, 51},
	{6, 13, 19, 26, 32, 38, 45, 51},
	{8, 12, 15, 19, 23, 26, 30, 34},
	{11, 18, 25, 33, 40, 47, 54, 61},
	{16, 24, 33, 41, 49, 57, 65, 73},
	{25, 35, 44, 54, 64, 74, 84, 93},
	{38, 50, 62, 74, 86, 98, 110, 122},
	{38, 50, 62, 74, 86, 98, 110, 122},
	{38, 50, 62, 74, 86, 98, 110, 122},
	{38, 50, 62, 74, 86, 98, 110, 122},
	{38, 50, 62, 74, 86, 98, 110, 122},
	{38, 50, 62, 74, 86, 98, 110, 122},
};

static IFE_HAL_NLM_KER range_ker[TOTAL_GAIN_NUM] =
{
    //Ker en, kerslope0, kerslop1, locw_en, bilW1, Bilw2, ker_rad
    {1, 9, 15, 0, 1, 0, (UINT8 *)range_ker_radius},
    {1, 9, 15, 0, 0, 0, (UINT8 *)range_ker_radius},
    {1, 9, 15, 1, 3, 8, (UINT8 *)range_ker_radius},
    {1, 9, 15, 1, 2, 5, (UINT8 *)range_ker_radius},
    {1, 9, 15, 1, 1, 3, (UINT8 *)range_ker_radius},
    {1, 9, 15, 1, 3, 7, (UINT8 *)range_ker_radius},
    {1, 9, 15, 1, 3, 7, (UINT8 *)range_ker_radius},
    {1, 9, 15, 1, 3, 7, (UINT8 *)range_ker_radius},
    {1, 9, 15, 1, 3, 7, (UINT8 *)range_ker_radius},
    {1, 9, 15, 1, 3, 7, (UINT8 *)range_ker_radius},
    {1, 9, 15, 1, 3, 7, (UINT8 *)range_ker_radius},
    {1, 9, 15, 1, 3, 7, (UINT8 *)range_ker_radius},
};

static UINT8 range_mwth[TOTAL_GAIN_NUM][2] =
{
	{ 8, 11},
	{ 8, 11},
	{16, 32},
	{16, 32},
	{ 8, 16},
	{ 6, 10},
	{ 6, 10},
	{ 6, 10},
	{ 6, 10},
	{ 6, 10},
	{ 6, 10},
	{ 6, 10},
};

static UINT8 range_bwa[TOTAL_GAIN_NUM][8] =
{
    ///< Legal range : 0~15, weighting table A
	{15, 15, 15, 15, 9, 8, 4, 0},
	{15, 15, 15, 15, 9, 8, 4, 0},
	{15, 15, 15, 15, 4, 3, 2, 1},
	{15, 15, 15, 15, 4, 3, 2, 1},
	{15, 15, 15, 15, 3, 2, 1, 0},
	{15, 15, 15, 15, 5, 3, 2, 1},
	{15, 15, 15, 15, 5, 3, 2, 3},
	{15, 15, 15, 15, 5, 3, 2, 3},
	{15, 15, 15, 15, 5, 3, 2, 3},
	{15, 15, 15, 15, 5, 3, 2, 3},
	{15, 15, 15, 15, 5, 3, 2, 3},
	{15, 15, 15, 15, 5, 3, 2, 3},
};

static UINT8 range_bwb[TOTAL_GAIN_NUM][4] =
{
    ///< Legal range : 0~15, weighting table B
	{15, 15, 15, 15},
	{15, 15, 15, 15},
	{15,  2,  3, 15},
	{15,  2,  3, 15},
	{15,  2,  1, 15},
	{15,  5,  4, 15},
	{15,  5,  4, 15},
	{15,  5,  4, 15},
	{15,  5,  4, 15},
	{15,  5,  4, 15},
	{15,  5,  4, 15},
	{15,  5,  4, 15},
};

static UINT8 range_bwc[TOTAL_GAIN_NUM][7] =
{
    ///< Legal range : 0~15, weighting table C
	{15, 15, 15, 9, 8, 4, 15},
	{15, 15, 3, 2, 1, 0, 15},
	{15, 3, 2, 1, 0, 15, 15},
	{15, 3, 2, 1, 0, 15, 15},
	{15, 3, 2, 1, 0, 15, 15},
	{15, 3, 2, 1, 0, 15, 15},
	{15, 5, 3, 2, 0, 15, 15},
	{15, 5, 3, 2, 0, 15, 15},
	{15, 5, 3, 2, 0, 15, 15},
	{15, 5, 3, 2, 0, 15, 15},
	{15, 5, 3, 2, 0, 15, 15},
	{15, 5, 3, 2, 0, 15, 15},
};

static UINT8 range_bwbl[TOTAL_GAIN_NUM][8]=
{
    ///< weighting table BL
	{4, 10, 25, 32, 53, 63, 74, 84},
	{4, 10, 25, 32, 53, 63, 74, 84},
	{0, 0, 1, 3, 5, 6, 8, 9},
	{0, 0, 2, 5, 7, 10, 13, 13},
	{0, 0, 3, 7, 10, 14, 17, 18},
	{0, 0, 4, 9, 13, 18, 23, 26},
	{0, 0, 7, 13, 20, 27, 33, 39},
	{0, 0, 7, 13, 20, 27, 33, 39},
	{0, 0, 7, 13, 20, 27, 33, 39},
	{0, 0, 7, 13, 20, 27, 33, 39},
	{0, 0, 7, 13, 20, 27, 33, 39},
	{0, 0, 7, 13, 20, 27, 33, 39},
};

static UINT8 range_bwbm[TOTAL_GAIN_NUM][8]=
{
    ///< weighting table BM
	{23, 48, 88, 100, 158, 189, 221, 252},
	{23, 48, 88, 100, 158, 189, 221, 252},
	{0, 1, 2, 4, 5, 7, 8, 9},
	{0, 3, 5, 8, 10, 14, 16, 17},
	{0, 2, 5, 8, 11, 14, 17, 19},
	{0, 3, 7, 11, 15, 19, 24, 27},
	{0, 8, 15, 21, 28, 35, 41, 47},
	{0, 8, 15, 21, 28, 35, 41, 47},
	{0, 8, 15, 21, 28, 35, 41, 47},
	{0, 8, 15, 21, 28, 35, 41, 47},
	{0, 8, 15, 21, 28, 35, 41, 47},
	{0, 8, 15, 21, 28, 35, 41, 47},
};

static UINT8 range_bwbh[TOTAL_GAIN_NUM][8]=
{
    ///< weighting table BH
	{38, 92, 140, 175, 236, 255, 255, 255},
	{38, 92, 140, 175, 236, 255, 255, 255},
	{6, 7, 7, 7, 8, 8, 9, 10},
	{9, 10, 11, 11, 12, 15, 20, 22},
	{12, 13, 14, 15, 16, 17, 18, 20},
	{19, 20, 21, 22, 23, 24, 25, 28},
	{28, 30, 31, 33, 34, 36, 46, 48},
	{28, 30, 31, 33, 34, 36, 46, 48},
	{28, 30, 31, 33, 34, 36, 46, 48},
	{28, 30, 31, 33, 34, 36, 46, 48},
	{28, 30, 31, 33, 34, 36, 46, 48},
	{28, 30, 31, 33, 34, 36, 46, 48},
};

//////////////////////
//**  IFE-Range
//////////////////////
static UINT32 range_a_thr[TOTAL_GAIN_NUM][6] =
{
    {0, 1, 3, 4, 6, 7},
    {0, 2, 4, 6, 9, 11},
    {0, 4, 8, 12, 16, 20},
    {0, 5, 10, 15, 19, 24},
    {0, 8, 15, 23, 31, 39},
    {0, 12, 24, 36, 48, 60},
    {0, 19, 38, 57, 76, 94},
    {0, 19, 38, 57, 76, 94},
    {0, 19, 38, 57, 76, 94},
    {0, 19, 38, 57, 76, 94},
    {0, 19, 38, 57, 76, 94},
    {0, 19, 38, 57, 76, 94},
};

static UINT32 range_a_lut[TOTAL_GAIN_NUM][17] =
{
    {0, 1, 4, 6, 8, 9, 11, 12, 14, 15, 17, 18, 20, 21, 23, 24, 26},
    {0, 2, 6, 8, 10, 11, 13, 15, 17, 19, 21, 23, 25, 26, 28, 30, 32},
    {0, 3, 8, 10, 11, 13, 15, 17, 19, 20, 22, 24, 26, 28, 29, 31, 33},
    {0, 4, 11, 15, 18, 21, 25, 28, 31, 35, 38, 41, 44, 48, 51, 54, 58},
    {0, 6, 16, 20, 24, 28, 32, 35, 39, 43, 47, 51, 55, 59, 63, 67, 71},
    {0, 8, 20, 25, 29, 34, 37, 42, 46, 51, 55, 60, 63, 68, 72, 77, 81},
    {0, 12, 29, 34, 38, 43, 48, 54, 58, 63, 68, 73, 78, 82, 88, 93, 98},
    {0, 12, 29, 34, 38, 43, 48, 54, 58, 63, 68, 73, 78, 82, 88, 93, 98},
    {0, 12, 29, 34, 38, 43, 48, 54, 58, 63, 68, 73, 78, 82, 88, 93, 98},
    {0, 12, 29, 34, 38, 43, 48, 54, 58, 63, 68, 73, 78, 82, 88, 93, 98},
    {0, 12, 29, 34, 38, 43, 48, 54, 58, 63, 68, 73, 78, 82, 88, 93, 98},
    {0, 12, 29, 34, 38, 43, 48, 54, 58, 63, 68, 73, 78, 82, 88, 93, 98},
};

static UINT32 range_b_thr[TOTAL_GAIN_NUM][6] =
{
    {0, 2, 6, 8, 12, 14},
    {0, 4, 8, 12, 18, 22},
    {0, 8, 16, 24, 32, 40},
    {0, 10, 20, 30, 38, 48},
    {0, 16, 30, 46, 62, 78},
    {0, 18, 36, 53, 72, 90},
    {0, 29, 56, 86, 114, 141},
    {0, 29, 56, 86, 114, 141},
    {0, 29, 56, 86, 114, 141},
    {0, 29, 56, 86, 114, 141},
    {0, 29, 56, 86, 114, 141},
    {0, 29, 56, 86, 114, 141},
};

static UINT32 range_b_lut[TOTAL_GAIN_NUM][17] =
{
    {0, 3, 9, 12, 15, 18, 21, 24, 27, 30, 33, 37, 40, 43, 46, 49, 52},
    {0, 4, 12, 16, 19, 23, 27, 30, 34, 38, 42, 45, 49, 53, 57, 60, 64},
    {0, 6, 15, 19, 23, 26, 30, 34, 37, 41, 44, 48, 52, 55, 59, 63, 66},
    {0, 8, 23, 30, 36, 43, 49, 56, 63, 69, 76, 82, 89, 95, 102, 109, 115},
    {0, 12, 31, 39, 47, 55, 63, 71, 79, 87, 95, 103, 111, 119, 126, 134, 142},
    {0, 12, 30, 37, 44, 51, 56, 63, 69, 77, 83, 89, 95, 101, 109, 115, 121},
    {0, 18, 43, 50, 58, 66, 73, 80, 87, 94, 102, 110, 117, 124, 132, 139, 146},
    {0, 18, 43, 50, 58, 66, 73, 80, 87, 94, 102, 110, 117, 124, 132, 139, 146},
    {0, 18, 43, 50, 58, 66, 73, 80, 87, 94, 102, 110, 117, 124, 132, 139, 146},
    {0, 18, 43, 50, 58, 66, 73, 80, 87, 94, 102, 110, 117, 124, 132, 139, 146},
    {0, 18, 43, 50, 58, 66, 73, 80, 87, 94, 102, 110, 117, 124, 132, 139, 146},
    {0, 18, 43, 50, 58, 66, 73, 80, 87, 94, 102, 110, 117, 124, 132, 139, 146},
};

static RangeCFGB range_b[TOTAL_GAIN_NUM] =
{
    //pThres,                  pThLut,            BilatThEn, BilatTh1, BilatTh2
    {(UINT32*)range_b_thr, (UINT32*)range_b_lut, {TRUE, 2, 4}},
    {(UINT32*)range_b_thr, (UINT32*)range_b_lut, {TRUE, 4, 8}},
    {(UINT32*)range_b_thr, (UINT32*)range_b_lut, {TRUE, 30, 60}},
    {(UINT32*)range_b_thr, (UINT32*)range_b_lut, {TRUE, 30, 60}},
    {(UINT32*)range_b_thr, (UINT32*)range_b_lut, {TRUE, 44, 66}},
    {(UINT32*)range_b_thr, (UINT32*)range_b_lut, {TRUE, 29, 58}},
    {(UINT32*)range_b_thr, (UINT32*)range_b_lut, {TRUE, 96, 192}},
    {(UINT32*)range_b_thr, (UINT32*)range_b_lut, {TRUE, 96, 192}},
    {(UINT32*)range_b_thr, (UINT32*)range_b_lut, {TRUE, 96, 192}},
    {(UINT32*)range_b_thr, (UINT32*)range_b_lut, {TRUE, 96, 192}},
    {(UINT32*)range_b_thr, (UINT32*)range_b_lut, {TRUE, 96, 192}},
    {(UINT32*)range_b_thr, (UINT32*)range_b_lut, {TRUE, 96, 192}},
};

//////////////////////
//**  IFE-CLAMP
//////////////////////
static RANGE_FILT_CLAMP_PARAM filter_clamp[TOTAL_GAIN_NUM] =
{
    //clamp{clamp_Th,Mul,Dlt}, Rth_w, Bin, BitDither}
    {{0, 128, 0}, 0, 0, 0},
    {{32, 32, 0}, 0, 0, 0},
    {{36, 64, 0}, 0, 0, 0},
    {{38, 70, 0}, 0, 0, 0},
    {{32, 64, 0}, 0, 0, 0},
    {{32, 32, 0}, 0, 0, 0},
    {{32, 32, 0}, 0, 0, 0},
    {{32, 32, 0}, 0, 0, 0},
    {{32, 32, 0}, 0, 0, 0},
    {{32, 32, 0}, 0, 0, 0},
    {{32, 32, 0}, 0, 0, 0},
    {{32, 32, 0}, 0, 0, 0},
};

//////////////////////
//**  IFE-Vig
//////////////////////
static IQS_VIG_CENTER_RATIO vig_center =
{
    500, 500
};

static IQS_VIG_SET vig =
{
    VIG_TAB_GAIN_2X, {0, 1, 4, 7, 10, 13, 23, 36, 57, 82, 112, 144, 198, 259, 375, 586, 1023}
};

static IQS_VIG_ISO_TH vig_iso_th =
{
    400, 1600
};

//////////////////////
//**  IFE-Gbal
//////////////////////
static UINT8 gbal_s_tab[TOTAL_GAIN_NUM][17] =
{
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
};

static UINT8 gbal_d_tab[TOTAL_GAIN_NUM][17] =
{
    {8, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {8, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {8, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {12, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {12, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {15, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {15, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {15, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {15, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {15, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {15, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {15, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
};

static UINT32 gbal_ir_sub[TOTAL_GAIN_NUM][4] =
{
    {0, 0, 0, 0},
    {0, 0, 0, 0},
    {0, 0, 0, 0},
    {0, 0, 0, 0},
    {0, 0, 0, 0},
    {0, 0, 0, 0},
    {0, 0, 0, 0},
    {0, 0, 0, 0},
    {0, 0, 0, 0},
    {0, 0, 0, 0},
    {0, 0, 0, 0},
    {0, 0, 0, 0},
};

static IFE_GBAL gbal[TOTAL_GAIN_NUM] =
{
   //Thres,pGbalStab,            pGbalDtab
    {8, (UINT8*)gbal_s_tab, (UINT8*)gbal_d_tab, (UINT32*)gbal_ir_sub, 0},
    {24, (UINT8*)gbal_s_tab, (UINT8*)gbal_d_tab, (UINT32*)gbal_ir_sub, 0},
    {24, (UINT8*)gbal_s_tab, (UINT8*)gbal_d_tab, (UINT32*)gbal_ir_sub, 0},
    {24, (UINT8*)gbal_s_tab, (UINT8*)gbal_d_tab, (UINT32*)gbal_ir_sub, 0},
    {30, (UINT8*)gbal_s_tab, (UINT8*)gbal_d_tab, (UINT32*)gbal_ir_sub, 0},
    {48, (UINT8*)gbal_s_tab, (UINT8*)gbal_d_tab, (UINT32*)gbal_ir_sub, 0},
    {48, (UINT8*)gbal_s_tab, (UINT8*)gbal_d_tab, (UINT32*)gbal_ir_sub, 0},
    {48, (UINT8*)gbal_s_tab, (UINT8*)gbal_d_tab, (UINT32*)gbal_ir_sub, 0},
    {48, (UINT8*)gbal_s_tab, (UINT8*)gbal_d_tab, (UINT32*)gbal_ir_sub, 0},
    {48, (UINT8*)gbal_s_tab, (UINT8*)gbal_d_tab, (UINT32*)gbal_ir_sub, 0},
    {48, (UINT8*)gbal_s_tab, (UINT8*)gbal_d_tab, (UINT32*)gbal_ir_sub, 0},
    {48, (UINT8*)gbal_s_tab, (UINT8*)gbal_d_tab, (UINT32*)gbal_ir_sub, 0},
};


static EnH ldc_enh =
{
   //Factor_Y,Norm_Y,Factor_UV,Norm_UV
    0, 0, 0, 0
};


////////////////////////////////
//             IPE             /
////////////////////////////////

//////////////////////
//**  IPE-Edge EXT
//////////////////////
static INT16 edge_kernel[TOTAL_GAIN_NUM][12] =
{
    //eext0 ~ eext9, eextdiv
    {0x017c, 0x002a, 0x03b0, 0x03e2, 0x03dd, 0x0004, 0x000a, 0x000a, 0x0004, 0x0001, 0x000a, 0x0004},
    {0x017c, 0x002a, 0x03b0, 0x03e2, 0x03dd, 0x0004, 0x000a, 0x000a, 0x0004, 0x0001, 0x000a, 0x0004},
    {0x017c, 0x002a, 0x03b0, 0x03e2, 0x03dd, 0x0004, 0x000a, 0x000a, 0x0004, 0x0001, 0x000a, 0x0004},
    {0x017c, 0x002a, 0x03b0, 0x03e2, 0x03dd, 0x0004, 0x000a, 0x000a, 0x0004, 0x0001, 0x000a, 0x0004},
    {0x017c, 0x002a, 0x03b0, 0x03e2, 0x03dd, 0x0004, 0x000a, 0x000a, 0x0004, 0x0001, 0x000a, 0x0004},
    {0x017c, 0x002a, 0x03b0, 0x03e2, 0x03dd, 0x0004, 0x000a, 0x000a, 0x0004, 0x0001, 0x000a, 0x0004},
    {0x017c, 0x002a, 0x03b0, 0x03e2, 0x03dd, 0x0004, 0x000a, 0x000a, 0x0004, 0x0001, 0x000a, 0x0004},
    {0x017c, 0x002a, 0x03b0, 0x03e2, 0x03dd, 0x0004, 0x000a, 0x000a, 0x0004, 0x0001, 0x000a, 0x0004},
    {0x017c, 0x002a, 0x03b0, 0x03e2, 0x03dd, 0x0004, 0x000a, 0x000a, 0x0004, 0x0001, 0x000a, 0x0004},
    {0x017c, 0x002a, 0x03b0, 0x03e2, 0x03dd, 0x0004, 0x000a, 0x000a, 0x0004, 0x0001, 0x000a, 0x0004},
    {0x017c, 0x002a, 0x03b0, 0x03e2, 0x03dd, 0x0004, 0x000a, 0x000a, 0x0004, 0x0001, 0x000a, 0x0004},
    {0x017c, 0x002a, 0x03b0, 0x03e2, 0x03dd, 0x0004, 0x000a, 0x000a, 0x0004, 0x0001, 0x000a, 0x0004},
};

static UINT8 edir_tab[TOTAL_GAIN_NUM][8]=
{
    {8, 8, 8, 8, 8, 8, 8, 8},
    {8, 8, 8, 8, 8, 8, 8, 8},
    {8, 8, 8, 8, 8, 8, 8, 8},
    {8, 8, 8, 8, 8, 8, 8, 8},
    {8, 8, 8, 8, 8, 8, 8, 8},
    {8, 8, 8, 8, 8, 8, 8, 8},
    {8, 8, 8, 8, 8, 8, 8, 8},
    {8, 8, 8, 8, 8, 8, 8, 8},
    {8, 8, 8, 8, 8, 8, 8, 8},
    {8, 8, 8, 8, 8, 8, 8, 8},
    {8, 8, 8, 8, 8, 8, 8, 8},
    {8, 8, 8, 8, 8, 8, 8, 8},
};

static IPE_DEE edir_ext[TOTAL_GAIN_NUM] =
{
   //YDRange,uiYDMaxTh,PndAdjEn,uiPndShft,uiES,EdirSel
    {IPE_YDR_7x7, 22, DISABLE, 3, 2, KER_SEL_A},
    {IPE_YDR_7x7, 22, DISABLE, 3, 2, KER_SEL_A},
    {IPE_YDR_7x7, 22, DISABLE, 3, 2, KER_SEL_A},
    {IPE_YDR_7x7, 22, DISABLE, 3, 2, KER_SEL_A},
    {IPE_YDR_7x7, 22, DISABLE, 3, 2, KER_SEL_A},
    {IPE_YDR_7x7, 22, DISABLE, 3, 2, KER_SEL_A},
    {IPE_YDR_7x7, 22, DISABLE, 3, 2, KER_SEL_A},
    {IPE_YDR_7x7, 22, DISABLE, 3, 2, KER_SEL_A},
    {IPE_YDR_7x7, 22, DISABLE, 3, 2, KER_SEL_A},
    {IPE_YDR_7x7, 22, DISABLE, 3, 2, KER_SEL_A},
    {IPE_YDR_7x7, 22, DISABLE, 3, 2, KER_SEL_A},
    {IPE_YDR_7x7, 22, DISABLE, 3, 2, KER_SEL_A},
};

static IPE_DirScorCal score_tab[TOTAL_GAIN_NUM] =
{
    //iA0; iB0; iC0; uiD0; iA1; iB1; iC1; uiD1; iA2; iB2; iC2; uiD2;
    {   3,  15,  15,    0,   5,   1,   1,    0,   3,   0,  15,   0},
    {   3,  15,  15,    0,   5,   1,   1,    0,   3,   0,  15,   0},
    {   3,  15,  15,    0,   5,   1,   1,    0,   3,   0,  15,   0},
    {   3,  15,  15,    0,   5,   1,   1,    0,   3,   0,  15,   0},
    {   3,  15,  15,    0,   5,   1,   1,    0,   3,   0,  15,   0},
    {   3,  15,  15,    0,   5,   1,   1,    0,   3,   0,  15,   0},
    {   3,  15,  15,    0,   5,   1,   1,    0,   3,   0,  15,   0},
    {   3,  15,  15,    0,   5,   1,   1,    0,   3,   0,  15,   0},
    {   3,  15,  15,    0,   5,   1,   1,    0,   3,   0,  15,   0},
    {   3,  15,  15,    0,   5,   1,   1,    0,   3,   0,  15,   0},
    {   3,  15,  15,    0,   5,   1,   1,    0,   3,   0,  15,   0},
    {   3,  15,  15,    0,   5,   1,   1,    0,   3,   0,  15,   0},
};

static IPE_DirScorCalTh score_th[TOTAL_GAIN_NUM] =
{
   //uiScoreTh0~3
    {7, 28, 3, 21},
    {5, 28, 3, 18},
    {5, 28, 2, 17},
    {5, 28, 3, 16},
    {9, 28, 4, 27},
    {5, 28, 3, 16},
    {6, 14, 5, 51},
    {6, 14, 5, 51},
    {6, 14, 5, 51},
    {6, 14, 5, 51},
    {6, 14, 5, 51},
    {6, 14, 5, 51},
};

static IPE_DEConParam decon[TOTAL_GAIN_NUM] =
{
   //ScrCenter,  ScrEdg,     ScrDetail,  uiCntNumTh,uiMax1Th,uiMaxCntTh,uiMax2Th,IdxSel
    {DE_SCR_ADD1,DE_SCR_ADD2,DE_SCR_ADD1,2,3,0,7,DECON_COMPLEX},
    {DE_SCR_ADD1,DE_SCR_ADD2,DE_SCR_ADD1,2,3,0,7,DECON_COMPLEX},
    {DE_SCR_ADD1,DE_SCR_ADD2,DE_SCR_ADD1,2,3,0,7,DECON_COMPLEX},
    {DE_SCR_ADD1,DE_SCR_ADD2,DE_SCR_ADD1,2,3,0,7,DECON_COMPLEX},
    {DE_SCR_ADD1,DE_SCR_ADD2,DE_SCR_ADD1,2,3,0,7,DECON_COMPLEX},
    {DE_SCR_ADD1,DE_SCR_ADD2,DE_SCR_ADD1,2,3,0,7,DECON_COMPLEX},
    {DE_SCR_ADD1,DE_SCR_ADD2,DE_SCR_ADD1,2,3,0,7,DECON_COMPLEX},
    {DE_SCR_ADD1,DE_SCR_ADD2,DE_SCR_ADD1,2,3,0,7,DECON_COMPLEX},
    {DE_SCR_ADD1,DE_SCR_ADD2,DE_SCR_ADD1,2,3,0,7,DECON_COMPLEX},
    {DE_SCR_ADD1,DE_SCR_ADD2,DE_SCR_ADD1,2,3,0,7,DECON_COMPLEX},
    {DE_SCR_ADD1,DE_SCR_ADD2,DE_SCR_ADD1,2,3,0,7,DECON_COMPLEX},
    {DE_SCR_ADD1,DE_SCR_ADD2,DE_SCR_ADD1,2,3,0,7,DECON_COMPLEX},
};

static IPE_KAEdgeW ewa[TOTAL_GAIN_NUM] =
{
  //EWKer2DSel, uiAdwt0~5,           iAds0~5
    {KER_SEL_A, 8, 8, 16, 16, 16, 16, 3, 3, 3, 3, 3, 3},
    {KER_SEL_A, 8, 8, 16, 16, 16, 16, 3, 3, 3, 3, 3, 3},
    {KER_SEL_A, 8, 8, 16, 16, 16, 16, 3, 3, 3, 3, 3, 3},
    {KER_SEL_A, 8, 8, 16, 16, 16, 16, 3, 3, 3, 3, 3, 3},
    {KER_SEL_A, 8, 8, 16, 16, 16, 16, 3, 3, 3, 3, 3, 3},
    {KER_SEL_A, 8, 8, 12, 12, 16, 16, 3, 3, 3, 3, 3, 3},
    {KER_SEL_A, 8, 8, 12, 12, 16, 16, 3, 3, 3, 3, 3, 3},
    {KER_SEL_A, 8, 8, 12, 12, 16, 16, 3, 3, 3, 3, 3, 3},
    {KER_SEL_A, 8, 8, 16, 16, 16, 16, 3, 3, 3, 3, 3, 3},
    {KER_SEL_A, 8, 8, 12, 12, 16, 16, 3, 3, 3, 3, 3, 3},
    {KER_SEL_A, 8, 8, 12, 12, 16, 16, 3, 3, 3, 3, 3, 3},
    {KER_SEL_A, 8, 8, 12, 12, 16, 16, 3, 3, 3, 3, 3, 3},
};

static IPE_KBEdgeW ewb[TOTAL_GAIN_NUM] =
{
   //uiBdwt0~1, iBds0~1, EwKerBOutSel
    {1, 1, 2, 2, SEL_AVG_4D},
    {1, 1, 2, 2, SEL_AVG_4D},
    {1, 1, 2, 2, SEL_AVG_4D},
    {1, 1, 2, 2, SEL_AVG_4D},
    {1, 1, 2, 2, SEL_AVG_4D},
    {1, 1, 2, 2, SEL_AVG_4D},
    {1, 1, 2, 2, SEL_AVG_4D},
    {1, 1, 2, 2, SEL_AVG_4D},
    {1, 1, 2, 2, SEL_AVG_4D},
    {1, 1, 2, 2, SEL_AVG_4D},
    {1, 1, 2, 2, SEL_AVG_4D},
    {1, 1, 2, 2, SEL_AVG_4D},
};

static IPE_EdgeW ewparam[TOTAL_GAIN_NUM] =
{
//uiEWDir0,1,2, DynEMEn
    {8, 2, 8, 0},
    {8, 0, 8, 0},
    {12, 2, 12, 0},
    {14, 4, 14, 0},
    {16, 8, 16, 0},
    {16, 14, 16, 0},
    {16, 16, 16, 0},
    {16, 16, 16, 0},
    {16, 16, 16, 0},
    {16, 16, 16, 0},
    {16, 16, 16, 0},
    {16, 16, 16, 0},
};

//////////////////////
//**  IPE-Edge Enh
//////////////////////
static IPE_EDGEENH edge_enh[TOTAL_GAIN_NUM] =
{
  //EnhP,EnhN, InvP,  InvN
    {128,  128, FALSE, FALSE},
    {112,  112, FALSE, FALSE},
    {112,  112, FALSE, FALSE},
    {112,  112, FALSE, FALSE},
    {112,  112, FALSE, FALSE},
    {112,  112, FALSE, FALSE},
    {112,  112, FALSE, FALSE},
    {112,  112, FALSE, FALSE},
    {112,  112, FALSE, FALSE},
    {112,  112, FALSE, FALSE},
    {112,  112, FALSE, FALSE},
    {112,  112, FALSE, FALSE},
};

//////////////////////
//**  IPE-Edge Map
//////////////////////
static UINT8 ed_tab[TOTAL_GAIN_NUM][16] =
{
    {64, 128, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255},
    {32, 112, 224, 224, 224, 224, 224, 224, 255, 255, 255, 255, 255, 255, 255, 255},
    {32, 64, 144, 192, 192, 192, 224, 224, 255, 255, 255, 255, 255, 255, 255, 255},
    {64, 112, 192, 192, 192, 192, 224, 224, 255, 255, 255, 255, 255, 255, 255, 255},
    {32, 112, 192, 192, 192, 192, 224, 224, 255, 255, 255, 255, 255, 255, 255, 255},
    {32, 112, 224, 224, 224, 224, 224, 224, 255, 255, 255, 255, 255, 255, 255, 255},
    {0, 64, 192, 224, 224, 224, 224, 224, 255, 255, 255, 255, 255, 255, 255, 255},
    {0, 64, 192, 224, 224, 224, 224, 224, 255, 255, 255, 255, 255, 255, 255, 255},
    {0, 64, 192, 224, 224, 224, 224, 224, 255, 255, 255, 255, 255, 255, 255, 255},
    {0, 64, 192, 224, 224, 224, 224, 224, 255, 255, 255, 255, 255, 255, 255, 255},
    {0, 64, 192, 224, 224, 224, 224, 224, 255, 255, 255, 255, 255, 255, 255, 255},
    {0, 64, 192, 224, 224, 224, 224, 224, 255, 255, 255, 255, 255, 255, 255, 255},
};

static EdgeMap ed_map[TOTAL_GAIN_NUM] =
{
 //EM_InSel,EthrL, EthrH,EtabL, EtabH, TAB_TblAddr
    {EMI_E57,   0,   256,    0,     5, (UINT32)ed_tab},
    {EMI_E57,   0,   256,    2,     5, (UINT32)ed_tab},
    {EMI_E57,   0,   256,    1,     5, (UINT32)ed_tab},
    {EMI_E57,   0,   256,    2,     5, (UINT32)ed_tab},
    {EMI_E57,   0,   256,    2,     5, (UINT32)ed_tab},
    {EMI_E57,   0,   256,    3,     5, (UINT32)ed_tab},
    {EMI_E57,   0,   256,    3,     5, (UINT32)ed_tab},
    {EMI_E57,   0,   256,    3,     5, (UINT32)ed_tab},
    {EMI_E57,   0,   256,    3,     5, (UINT32)ed_tab},
    {EMI_E57,   0,   256,    3,     5, (UINT32)ed_tab},
    {EMI_E57,   0,   256,    3,     5, (UINT32)ed_tab},
    {EMI_E57,   0,   256,    3,     5, (UINT32)ed_tab},
};

static UINT8 es_tab[TOTAL_GAIN_NUM][16] =
{
    {64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 52, 44, 38},
    {64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 52, 44, 38, 32, 32},
    {64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 52, 44, 38, 32, 32},
    {64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 52, 44, 38, 32, 32},
    {64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 52, 44, 38, 32, 32},
    {64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 52, 44, 38, 32, 32},
    {32, 48, 52, 64, 64, 64, 64, 64, 64, 64, 64, 52, 44, 38, 32, 32},
    {32, 48, 52, 64, 64, 64, 64, 64, 64, 64, 64, 52, 44, 38, 32, 32},
    {32, 48, 52, 64, 64, 64, 64, 64, 64, 64, 64, 52, 44, 38, 32, 32},
    {32, 48, 52, 64, 64, 64, 64, 64, 64, 64, 64, 52, 44, 38, 32, 32},
    {32, 48, 52, 64, 64, 64, 64, 64, 64, 64, 64, 52, 44, 38, 32, 32},
    {32, 48, 52, 64, 64, 64, 64, 64, 64, 64, 64, 52, 44, 38, 32, 32},
};

static EdgeMapS es_map[TOTAL_GAIN_NUM] =
{
 //EthrL,EthrH, EtabL, EtabH, TAB_TblAddr
	{0,   512,     5,     5, (UINT32)es_tab},
	{0,   512,     5,     5, (UINT32)es_tab},
	{0,   512,     5,     5, (UINT32)es_tab},
	{0,   512,     5,     5, (UINT32)es_tab},
	{0,   512,     5,     5, (UINT32)es_tab},
	{0,   512,     5,     5, (UINT32)es_tab},
	{0,   512,     5,     5, (UINT32)es_tab},
	{0,   512,     5,     5, (UINT32)es_tab},
	{0,   512,     5,     5, (UINT32)es_tab},
	{0,   512,     5,     5, (UINT32)es_tab},
	{0,   512,     5,     5, (UINT32)es_tab},
	{0,   512,     5,     5, (UINT32)es_tab},
};

//////////////////////
//**  IPE-LPF
//////////////////////
static LPF lpf[TOTAL_GAIN_NUM] =
{
   //lpfw,sonlyw,rangeth0,rangeth1,lpfsize
    {4, 0, 4, 8, _5x5},
    {4, 0, 5, 10, _5x5},
    {4, 0, 6, 13, _5x5},
    {4, 0, 8, 16, _5x5},
    {4, 0, 8, 16, _5x5},
    {4, 0, 8, 16, _5x5},
    {4, 0, 10, 22, _5x5},
    {4, 0, 10, 22, _5x5},
    {4, 0, 10, 22, _5x5},
    {4, 0, 10, 22, _5x5},
    {4, 0, 10, 22, _5x5},
    {4, 0, 10, 22, _5x5},
};


static INT8 saturation[TOTAL_GAIN_NUM]=
{
    18,
    18,
    18,
    18,
    18,
    18,
    18,
    18,
    18,
    18,
    18,
    18,
};

static INT8 contrast[TOTAL_GAIN_NUM] =
{
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
};

static IQS_CCTRL_INFO cctrl_info[TOTAL_GAIN_NUM] =
{
    {CCTRL_SEL_E7,  0x80},
    {CCTRL_SEL_E7,  0x80},
    {CCTRL_SEL_E7,  0x80},
    {CCTRL_SEL_E7,  0x80},
    {CCTRL_SEL_E7,  0x80},
    {CCTRL_SEL_E7,  0x80},
    {CCTRL_SEL_E7,  0x80},
    {CCTRL_SEL_E7,  0x80},
    {CCTRL_SEL_E7,  0x80},
    {CCTRL_SEL_E7,  0x80},
    {CCTRL_SEL_E7,  0x80},
    {CCTRL_SEL_E7,  0x80},
};

////////////////////////////////
//             IFE2            /
////////////////////////////////

//////////////////////
//**  IFE2-ReferenceCenter
//////////////////////
static UINT32 ref_center_y_th[TOTAL_GAIN_NUM][3] =
{
    {1, 2, 3},
    {1, 2, 3},
    {2, 4, 6},
    {6, 9, 12},
    {15, 22, 29},
    {6, 9, 12},
    {22, 33, 44},
    {22, 33, 44},
    {22, 33, 44},
    {22, 33, 44},
    {22, 33, 44},
    {22, 33, 44},
};

static UINT32 ref_center_y_wt[TOTAL_GAIN_NUM][4] =
{
    {8, 4, 2, 1},
    {8, 4, 2, 1},
    {8, 6, 4, 2},
    {8, 6, 4, 2},
    {8, 6, 4, 2},
    {8, 6, 4, 2},
    {8, 6, 4, 2},
    {8, 6, 4, 2},
    {8, 6, 4, 2},
    {8, 6, 4, 2},
    {8, 6, 4, 2},
    {8, 6, 4, 2},
};

static IFE2_REFCENT_Set ref_center_y[TOTAL_GAIN_NUM] =
{
    {(UINT32)ref_center_y_th, (UINT32)ref_center_y_wt, 31, 9, 6},
    {(UINT32)ref_center_y_th, (UINT32)ref_center_y_wt, 31, 9, 6},
    {(UINT32)ref_center_y_th, (UINT32)ref_center_y_wt, 31, 9, 6},
    {(UINT32)ref_center_y_th, (UINT32)ref_center_y_wt, 31, 9, 6},
    {(UINT32)ref_center_y_th, (UINT32)ref_center_y_wt, 31, 9, 6},
    {(UINT32)ref_center_y_th, (UINT32)ref_center_y_wt, 31, 9, 6},
    {(UINT32)ref_center_y_th, (UINT32)ref_center_y_wt, 31, 9, 6},
    {(UINT32)ref_center_y_th, (UINT32)ref_center_y_wt, 31, 9, 6},
    {(UINT32)ref_center_y_th, (UINT32)ref_center_y_wt, 31, 9, 6},
    {(UINT32)ref_center_y_th, (UINT32)ref_center_y_wt, 31, 9, 6},
    {(UINT32)ref_center_y_th, (UINT32)ref_center_y_wt, 31, 9, 6},
    {(UINT32)ref_center_y_th, (UINT32)ref_center_y_wt, 31, 9, 6},
};

static UINT32 ref_center_uv_th[TOTAL_GAIN_NUM][3] =
{
    {1, 2, 3},
    {1, 2, 3},
    {2, 4, 6},
    {7, 10, 13},
    {10, 15, 18},
    {7, 10, 13},
    {13, 19, 26},
    {13, 19, 26},
    {13, 19, 26},
    {13, 19, 26},
    {13, 19, 26},
    {13, 19, 26},
};

static UINT32 ref_center_uv_wt[TOTAL_GAIN_NUM][4] =
{
    {8, 4, 2, 1},
    {8, 4, 2, 1},
    {8, 6, 4, 2},
    {8, 6, 4, 2},
    {8, 6, 4, 2},
    {8, 6, 4, 2},
    {8, 6, 4, 2},
    {8, 6, 4, 2},
    {8, 6, 4, 2},
    {8, 6, 4, 2},
    {8, 6, 4, 2},
    {8, 6, 4, 2},
};

static IFE2_REFCENT_Set ref_center_uv[TOTAL_GAIN_NUM] =
{
    {(UINT32)ref_center_uv_th, (UINT32)ref_center_uv_wt, 31, 9, 6},
    {(UINT32)ref_center_uv_th, (UINT32)ref_center_uv_wt, 31, 9, 6},
    {(UINT32)ref_center_uv_th, (UINT32)ref_center_uv_wt, 31, 9, 6},
    {(UINT32)ref_center_uv_th, (UINT32)ref_center_uv_wt, 31, 9, 6},
    {(UINT32)ref_center_uv_th, (UINT32)ref_center_uv_wt, 31, 9, 6},
    {(UINT32)ref_center_uv_th, (UINT32)ref_center_uv_wt, 31, 9, 6},
    {(UINT32)ref_center_uv_th, (UINT32)ref_center_uv_wt, 31, 9, 6},
    {(UINT32)ref_center_uv_th, (UINT32)ref_center_uv_wt, 31, 9, 6},
    {(UINT32)ref_center_uv_th, (UINT32)ref_center_uv_wt, 31, 9, 6},
    {(UINT32)ref_center_uv_th, (UINT32)ref_center_uv_wt, 31, 9, 6},
    {(UINT32)ref_center_uv_th, (UINT32)ref_center_uv_wt, 31, 9, 6},
    {(UINT32)ref_center_uv_th, (UINT32)ref_center_uv_wt, 31, 9, 6},
};

//////////////////////
//**  IFE2-Filter
//////////////////////
static UINT32 ife2filter_y_th[TOTAL_GAIN_NUM][5] =
{
    {1, 2, 3, 4, 5},
    {1, 2, 3, 4, 5},
    {4, 6, 8, 10, 12},
    {8, 11, 14, 17, 20},
    {15, 22, 29, 36, 43},
    {8, 11, 14, 17, 20},
    {22, 33, 44, 55, 66},
    {22, 33, 44, 55, 66},
    {22, 33, 44, 55, 66},
    {22, 33, 44, 55, 66},
    {22, 33, 44, 55, 66},
    {22, 33, 44, 55, 66},
};

static UINT32 ife2filter_y_wt[TOTAL_GAIN_NUM][6] =
{
    {16, 8, 4, 2, 1, 0},
    {16, 8, 4, 2, 1, 0},
    {16, 8, 4, 2, 1, 0},
    {16, 13, 10, 7, 4, 1},
    {16, 13, 10, 7, 4, 1},
    {16, 13, 10, 7, 4, 1},
    {16, 13, 10, 7, 4, 1},
    {16, 13, 10, 7, 4, 1},
    {16, 13, 10, 7, 4, 1},
    {16, 13, 10, 7, 4, 1},
    {16, 13, 10, 7, 4, 1},
    {16, 13, 10, 7, 4, 1},
};

static UINT32 ife2filter_uv_th[TOTAL_GAIN_NUM][5] =
{
    {1, 1, 3, 3, 5},
    {1, 1, 3, 3, 5},
    {6, 8, 10, 12, 14},
    {10, 14, 18, 22, 26},
    {10, 15, 20, 25, 30},
    {10, 14, 18, 21, 25},
    {13, 19, 26, 32, 39},
    {13, 19, 26, 32, 39},
    {13, 19, 26, 32, 39},
    {13, 19, 26, 32, 39},
    {13, 19, 26, 32, 39},
    {13, 19, 26, 32, 39},
};

static UINT32 ife2filter_uv_wt[TOTAL_GAIN_NUM][6] =
{
    {16, 16, 8, 8, 4, 4},
    {16, 16, 8, 8, 4, 4},
    {16, 13, 10, 7, 4, 1},
    {16, 13, 10, 7, 4, 1},
    {16, 13, 10, 7, 4, 1},
    {16, 13, 10, 7, 4, 1},
    {16, 13, 10, 7, 4, 1},
    {16, 13, 10, 7, 4, 1},
    {16, 13, 10, 7, 4, 1},
    {16, 13, 10, 7, 4, 1},
    {16, 13, 10, 7, 4, 1},
    {16, 13, 10, 7, 4, 1},
};

static IQS_IFE2_FILTER ife2filter[TOTAL_GAIN_NUM] =
{
//RepNum,FiltSize,     {uiEdthHV, uiEdthPN},EdgeKernelSize, FiltSetY,                                        FiltSetU,                                                   FiltSetV
    {1, {FLTR_SIZE_9x9, {32, 32}, EKNL_SIZE_3x3, {(UINT32)ife2filter_y_th, (UINT32)ife2filter_y_wt},  {(UINT32)ife2filter_uv_th, (UINT32)ife2filter_uv_wt}, {(UINT32)ife2filter_uv_th, (UINT32)ife2filter_uv_wt}}},
    {1, {FLTR_SIZE_9x9, {32, 32}, EKNL_SIZE_3x3, {(UINT32)ife2filter_y_th, (UINT32)ife2filter_y_wt},  {(UINT32)ife2filter_uv_th, (UINT32)ife2filter_uv_wt}, {(UINT32)ife2filter_uv_th, (UINT32)ife2filter_uv_wt}}},
    {1, {FLTR_SIZE_9x9, {18, 18}, EKNL_SIZE_3x3, {(UINT32)ife2filter_y_th, (UINT32)ife2filter_y_wt},  {(UINT32)ife2filter_uv_th, (UINT32)ife2filter_uv_wt}, {(UINT32)ife2filter_uv_th, (UINT32)ife2filter_uv_wt}}},
    {1, {FLTR_SIZE_9x9, {18, 18}, EKNL_SIZE_3x3, {(UINT32)ife2filter_y_th, (UINT32)ife2filter_y_wt},  {(UINT32)ife2filter_uv_th, (UINT32)ife2filter_uv_wt}, {(UINT32)ife2filter_uv_th, (UINT32)ife2filter_uv_wt}}},
    {1, {FLTR_SIZE_9x9, {14, 14}, EKNL_SIZE_3x3, {(UINT32)ife2filter_y_th, (UINT32)ife2filter_y_wt},  {(UINT32)ife2filter_uv_th, (UINT32)ife2filter_uv_wt}, {(UINT32)ife2filter_uv_th, (UINT32)ife2filter_uv_wt}}},
    {1, {FLTR_SIZE_9x9, {18, 18}, EKNL_SIZE_3x3, {(UINT32)ife2filter_y_th, (UINT32)ife2filter_y_wt},  {(UINT32)ife2filter_uv_th, (UINT32)ife2filter_uv_wt}, {(UINT32)ife2filter_uv_th, (UINT32)ife2filter_uv_wt}}},
    {1, {FLTR_SIZE_9x9, {14, 14}, EKNL_SIZE_3x3, {(UINT32)ife2filter_y_th, (UINT32)ife2filter_y_wt},  {(UINT32)ife2filter_uv_th, (UINT32)ife2filter_uv_wt}, {(UINT32)ife2filter_uv_th, (UINT32)ife2filter_uv_wt}}},
    {1, {FLTR_SIZE_9x9, {14, 14}, EKNL_SIZE_3x3, {(UINT32)ife2filter_y_th, (UINT32)ife2filter_y_wt},  {(UINT32)ife2filter_uv_th, (UINT32)ife2filter_uv_wt}, {(UINT32)ife2filter_uv_th, (UINT32)ife2filter_uv_wt}}},
    {1, {FLTR_SIZE_9x9, {14, 14}, EKNL_SIZE_3x3, {(UINT32)ife2filter_y_th, (UINT32)ife2filter_y_wt},  {(UINT32)ife2filter_uv_th, (UINT32)ife2filter_uv_wt}, {(UINT32)ife2filter_uv_th, (UINT32)ife2filter_uv_wt}}},
    {1, {FLTR_SIZE_9x9, {14, 14}, EKNL_SIZE_3x3, {(UINT32)ife2filter_y_th, (UINT32)ife2filter_y_wt},  {(UINT32)ife2filter_uv_th, (UINT32)ife2filter_uv_wt}, {(UINT32)ife2filter_uv_th, (UINT32)ife2filter_uv_wt}}},
    {1, {FLTR_SIZE_9x9, {14, 14}, EKNL_SIZE_3x3, {(UINT32)ife2filter_y_th, (UINT32)ife2filter_y_wt},  {(UINT32)ife2filter_uv_th, (UINT32)ife2filter_uv_wt}, {(UINT32)ife2filter_uv_th, (UINT32)ife2filter_uv_wt}}},
    {1, {FLTR_SIZE_9x9, {14, 14}, EKNL_SIZE_3x3, {(UINT32)ife2filter_y_th, (UINT32)ife2filter_y_wt},  {(UINT32)ife2filter_uv_th, (UINT32)ife2filter_uv_wt}, {(UINT32)ife2filter_uv_th, (UINT32)ife2filter_uv_wt}}},
};

//////////////////////
//**  IFE2-Gray
//////////////////////
static IFE2_GRAY_Statal gray_statical[TOTAL_GAIN_NUM] =
{
    {125, 131, 125, 131},
    {125, 131, 125, 131},
    {125, 131, 125, 131},
    {125, 131, 125, 131},
    {125, 131, 125, 131},
    {125, 131, 125, 131},
    {125, 131, 125, 131},
    {125, 131, 125, 131},
    {125, 131, 125, 131},
    {125, 131, 125, 131},
    {125, 131, 125, 131},
    {125, 131, 125, 131},
};

////////////////////////////////
//             IME             /
////////////////////////////////

//////////////////////
//**  IME-LCA
//////////////////////
static IME_CHRA_ADAP chroma_adapt[TOTAL_GAIN_NUM] =
{
//LcaRefWt{RefY,RefUV,OutUV},LcaYinfo{LcaY,LcaYTh,LcaYWtPrc,LcaYWtS,LcaYWtE}, LcaUVinfo{UV, UVTh, UVWtPrc, UVWtS, UVWtE}, SubRatio
        {{31, 31,  8},        {RANGE_8,   0, RANGE_8,  8,       0},            {RANGE_8, 0, RANGE_8, 8, 0}, 25},
        {{31, 31,  8},        {RANGE_8,   0, RANGE_8,  8,       0},            {RANGE_8, 0, RANGE_8, 8, 0}, 25},
        {{31, 31,  8},        {RANGE_8,   0, RANGE_8,  8,       0},            {RANGE_8, 0, RANGE_8, 8, 0}, 25},
        {{31, 31, 12},        {RANGE_8,   0, RANGE_8,  8,       0},            {RANGE_8, 0, RANGE_8, 8, 0}, 25},
        {{31, 31, 12},        {RANGE_8,   0, RANGE_8,  8,       0},            {RANGE_8, 0, RANGE_8, 8, 0}, 25},
        {{31, 31, 12},        {RANGE_8,   0, RANGE_8,  8,       0},            {RANGE_8, 0, RANGE_8, 8, 0}, 25},
        {{31, 31, 12},        {RANGE_8,   0, RANGE_8,  8,       0},            {RANGE_8, 0, RANGE_8, 8, 0}, 25},
        {{31, 31, 12},        {RANGE_8,   0, RANGE_8,  8,       0},            {RANGE_8, 0, RANGE_8, 8, 0}, 25},
        {{31, 31, 12},        {RANGE_8,   0, RANGE_8,  8,       0},            {RANGE_8, 0, RANGE_8, 8, 0}, 25},
        {{31, 31, 12},        {RANGE_8,   0, RANGE_8,  8,       0},            {RANGE_8, 0, RANGE_8, 8, 0}, 25},
        {{31, 31, 12},        {RANGE_8,   0, RANGE_8,  8,       0},            {RANGE_8, 0, RANGE_8, 8, 0}, 25},
        {{31, 31, 12},        {RANGE_8,   0, RANGE_8,  8,       0},            {RANGE_8, 0, RANGE_8, 8, 0}, 25},
};

static IME_CHRA_CADAP chroma_adapt_color[TOTAL_GAIN_NUM] =
{
 //LcaCLREn, LcaRefWt{CentU, CentV}, LcaCLRinfo{LumRng,LumTh,LumWtPrc,LumWtS,LumWtE}
    {ENABLE,         {128,     128},          {RANGE_8,   0,   RANGE_8,   8,     0}},
    {ENABLE,         {128,     128},          {RANGE_8,   0,   RANGE_8,   8,     0}},
    {ENABLE,         {128,     128},          {RANGE_8,   0,   RANGE_8,   8,     0}},
    {ENABLE,         {128,     128},          {RANGE_8,   0,   RANGE_8,   8,     0}},
    {ENABLE,         {128,     128},          {RANGE_8,   0,   RANGE_8,   8,     0}},
    {ENABLE,         {128,     128},          {RANGE_8,   0,   RANGE_8,   8,     0}},
    {ENABLE,         {128,     128},          {RANGE_8,   0,   RANGE_8,   8,     0}},
    {ENABLE,         {128,     128},          {RANGE_8,   0,   RANGE_8,   8,     0}},
    {ENABLE,         {128,     128},          {RANGE_8,   0,   RANGE_8,   8,     0}},
    {ENABLE,         {128,     128},          {RANGE_8,   0,   RANGE_8,   8,     0}},
    {ENABLE,         {128,     128},          {RANGE_8,   0,   RANGE_8,   8,     0}},
    {ENABLE,         {128,     128},          {RANGE_8,   0,   RANGE_8,   8,     0}},
};

static IME_CHRA_LADAP chroma_adapt_lum[TOTAL_GAIN_NUM] =
{
  //LcaLumEn, LcaRefWt{RefWt,OutWt}, LcaLuminfo{LumRng, LumTh,LumWtPrc,LumWtS,LumWtE}
    {DISABLE,         {0,        0},           {RANGE_8,    0, RANGE_8,     0,     0}},
    {DISABLE,         {0,        0},           {RANGE_8,    0, RANGE_8,     0,     0}},
    {DISABLE,         {0,        0},           {RANGE_8,    0, RANGE_8,     0,     0}},
    {DISABLE,         {0,        0},           {RANGE_8,    0, RANGE_8,     0,     0}},
    {DISABLE,         {0,        0},           {RANGE_8,    0, RANGE_8,     0,     0}},
    {DISABLE,         {0,        0},           {RANGE_8,    0, RANGE_8,     0,     0}},
    {DISABLE,         {0,        0},           {RANGE_8,    0, RANGE_8,     0,     0}},
    {DISABLE,         {0,        0},           {RANGE_8,    0, RANGE_8,     0,     0}},
    {DISABLE,         {0,        0},           {RANGE_8,    0, RANGE_8,     0,     0}},
    {DISABLE,         {0,        0},           {RANGE_8,    0, RANGE_8,     0,     0}},
    {DISABLE,         {0,        0},           {RANGE_8,    0, RANGE_8,     0,     0}},
    {DISABLE,         {0,        0},           {RANGE_8,    0, RANGE_8,     0,     0}},
};

//////////////////////
//**  IME-ChromaSuppression
//////////////////////
static UINT32 chroma_supp_w_y[TOTAL_GAIN_NUM][16] =
{
    {16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1},
    {16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1},
    {16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1},
    {16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1},
    {16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1},
    {16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1},
    {16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1},
    {16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1},
    {16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1},
    {16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1},
    {16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1},
    {16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1},
};

static UINT32 chroma_supp_w_c[TOTAL_GAIN_NUM][16] =
{
    {16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1},
    {16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1},
    {16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1},
    {16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1},
    {16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1},
    {16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1},
    {16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1},
    {16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1},
    {16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1},
    {16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1},
    {16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1},
    {16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1},
};

static IME_ChromaSupp chroma_supp[TOTAL_GAIN_NUM] =
{
    //ModeSel,      Cetner,     Step,   puiWeightY,                      puiWeightC
    {0,DBCS_DK_MODE, {128, 128}, {0, 0}, (UINT32*)chroma_supp_w_y, (UINT32*)chroma_supp_w_c},
    {0,DBCS_DK_MODE, {128, 128}, {0, 0}, (UINT32*)chroma_supp_w_y, (UINT32*)chroma_supp_w_c},
    {0,DBCS_DK_MODE, {128, 128}, {0, 0}, (UINT32*)chroma_supp_w_y, (UINT32*)chroma_supp_w_c},
    {0,DBCS_DK_MODE, {128, 128}, {0, 0}, (UINT32*)chroma_supp_w_y, (UINT32*)chroma_supp_w_c},
    {0,DBCS_DK_MODE, {128, 128}, {0, 0}, (UINT32*)chroma_supp_w_y, (UINT32*)chroma_supp_w_c},
    {0,DBCS_DK_MODE, {128, 128}, {0, 0}, (UINT32*)chroma_supp_w_y, (UINT32*)chroma_supp_w_c},
    {0,DBCS_DK_MODE, {128, 128}, {0, 0}, (UINT32*)chroma_supp_w_y, (UINT32*)chroma_supp_w_c},
    {0,DBCS_DK_MODE, {128, 128}, {0, 0}, (UINT32*)chroma_supp_w_y, (UINT32*)chroma_supp_w_c},
    {0,DBCS_DK_MODE, {128, 128}, {0, 0}, (UINT32*)chroma_supp_w_y, (UINT32*)chroma_supp_w_c},
    {0,DBCS_DK_MODE, {128, 128}, {0, 0}, (UINT32*)chroma_supp_w_y, (UINT32*)chroma_supp_w_c},
    {0,DBCS_DK_MODE, {128, 128}, {0, 0}, (UINT32*)chroma_supp_w_y, (UINT32*)chroma_supp_w_c},
    {0,DBCS_DK_MODE, {128, 128}, {0, 0}, (UINT32*)chroma_supp_w_y, (UINT32*)chroma_supp_w_c},
};

//////////////////////
//**  IME-SuperResolution
//////////////////////
static IME_SuperRes super_res = {0, 0, 0, 0};

//////////////////////
//**  IME-SQUARE
//////////////////////

////////////////////////////////
//            Codec            /
////////////////////////////////

////////////////////
// SIE-ECS
////////////////////
static SIE_EmbeddedCS ecs = {ECS_D2F8, ENABLE, ENABLE, _BIT_0_1};
static UINT32 ecs_tab[4225] =
{
    0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,
    0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,
    0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,
    0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,
    0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,
    0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,
    0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,
    0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,
    0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,
    0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,
    0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,
    0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,
    0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,
    0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,
    0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,
    0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,
    0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,
    0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,
    0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,
    0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,
    0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,
    0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,
    0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,
    0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,
    0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,
    0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,
    0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,
    0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,
    0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,
    0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,
    0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,
    0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,
    0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,
    0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,
    0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,
    0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,
    0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,
    0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,
    0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,
    0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,
    0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,
    0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,
    0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,
    0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,
    0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,
    0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,
    0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,
    0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,
    0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,
    0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,
    0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,
    0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,
    0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,
    0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,
    0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,
    0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,
    0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,
    0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,
    0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,
    0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,
    0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,
    0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,
    0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,
    0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,
    0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100,0x10040100
};


static H26XEncTnr tnr_param_off =
{   0,                  //enable temporal noise reduction (0: disable, 1:enable)
    0,                  //TNR Mode as OSD enabled: fully TNR (0: fully TNR, 1: P2P only, 2: no TNR)
    1,                  //enable mctf/p2p pixel blending (0: disable, 1:enable)
    3,                  //P2P SAD Mode (0: pixel, 3: SAD + pixel)
    2,                  //MCTF SAD Mode (0: pixel, 1: b8x8, 2: b16x16, 3: auto)
    0,                  //MCTF Bias Mode (0: disable, 1: b8x8, 2: b16x16, 3: auto)
    {3, 7, 12},         //TNR_P2P_SAD_THRESHOLD (0~255)
    {8, 6, 2},          //TNR_P2P_WEIGHT (0~16)
    12,                 //TNR_P2P_BORDER_CHECK_THRES (0~255)
    4,                  //TNR_P2P_BORDER_CHECK_SCALE (0~7)
    0,                  //TNR_P2P_INPUT (0: original source, 1: mctf result)
    16,                 //TNR_P2P_MCTF_MOTION_THRESHOLD (0~255)
    {1, 1, 1, 1},       //TNR_P2P_MCTF_MOTION_WEIGHT (0~3)
    //P2P TNR Config----------------------------------------------------------------------------
    {3, 2, 3},          //TNR_P2P_REF_Y_WEIGHT (0~7)
    {3, 2, 3},          //TNR_P2P_REF_C_WEIGHT (0~7)
    {6, 10, 16},        //TNR_P2P_RANGE_Y_THRESHOLD (0~255)
    {0, 2, 3},          //TNR_P2P_RANGE_C_THRESHOLD (0~255)
    2,                  //TNR_P2P_CLAMP_Y_THRESHOLD (0~255)
    1,                  //TNR_P2P_CLAMP_Y_LSR (0~7)
    2,                  //TNR_P2P_CLAMP_C_THRESHOLD (0~255)
    1,                  //TNR_P2P_CLAMP_C_LSR (0~7)
    //MCTF TNR Config----------------------------------------------------------------------------
    {3, 2, 3},          //TNR_MCTF_REF_Y_WEIGHT (0~7)
    {3, 2, 3},          //TNR_MCTF_REF_C_WEIGHT (0~7)
    {5, 6, 7},          //TNR_MCTF_RANGE_Y_THRESHOLD (0~255)
    {0, 2, 3},          //TNR_MCTF_RANGE_C_THRESHOLD (0~255)
    2,                  //TNR_MCTF_CLAMP_Y_THRESHOLD (0~255)
    1,                  //TNR_MCTF_CLAMP_Y_LSR (0~7)
    2,                  //TNR_MCTF_CLAMP_C_THRESHOLD (0~255)
    1,                  //TNR_MCTF_CLAMP_C_LSR (0~7)
    //ADDITIONAL TNR Config---------------------------------------------------------------------
	{1, 2, 1},          //NR3D_MCTF_REF_Y_WEIGHT (0~7)
	{1, 2, 1},          //NR3D_MCTF_REF_C_WEIGHT (0~7)
	{1, 2, 1},          //NR3D_P2P_REF_Y_WEIGHT (0~7)
	{1, 2, 1},          //NR3D_P2P_REF_C_WEIGHT (0~7)
    0,                  //enable I frame weight refresh (0: disable, 1:enable)
};

static H26XEncTnr tnr_param_lv1 =
{   0,                  //enable temporal noise reduction (0: disable, 1:enable)
    0,                  //TNR Mode as OSD enabled: fully TNR (0: fully TNR, 1: P2P only, 2: no TNR)
    1,                  //enable mctf/p2p pixel blending (0: disable, 1:enable)
    3,                  //P2P SAD Mode (0: pixel, 3: SAD + pixel)
    2,                  //MCTF SAD Mode (0: pixel, 1: b8x8, 2: b16x16, 3: auto)
    0,                  //MCTF Bias Mode (0: disable, 1: b8x8, 2: b16x16, 3: auto)
    {3, 7, 12},         //TNR_P2P_SAD_THRESHOLD (0~255)
    {8, 6, 2},          //TNR_P2P_WEIGHT (0~16)
    12,                 //TNR_P2P_BORDER_CHECK_THRES (0~255)
    4,                  //TNR_P2P_BORDER_CHECK_SCALE (0~7)
    0,                  //TNR_P2P_INPUT (0: original source, 1: mctf result)
    16,                 //TNR_P2P_MCTF_MOTION_THRESHOLD (0~255)
    {1, 1, 1, 1},       //TNR_P2P_MCTF_MOTION_WEIGHT (0~3)
    //P2P TNR Config----------------------------------------------------------------------------
    {3, 2, 3},          //TNR_P2P_REF_Y_WEIGHT (0~7)
    {3, 2, 3},          //TNR_P2P_REF_C_WEIGHT (0~7)
    {6, 10, 16},        //TNR_P2P_RANGE_Y_THRESHOLD (0~255)
    {0, 2, 3},          //TNR_P2P_RANGE_C_THRESHOLD (0~255)
    2,                  //TNR_P2P_CLAMP_Y_THRESHOLD (0~255)
    1,                  //TNR_P2P_CLAMP_Y_LSR (0~7)
    2,                  //TNR_P2P_CLAMP_C_THRESHOLD (0~255)
    1,                  //TNR_P2P_CLAMP_C_LSR (0~7)
    //MCTF TNR Config----------------------------------------------------------------------------
    {3, 2, 3},          //TNR_MCTF_REF_Y_WEIGHT (0~7)
    {3, 2, 3},          //TNR_MCTF_REF_C_WEIGHT (0~7)
    {5, 6, 7},          //TNR_MCTF_RANGE_Y_THRESHOLD (0~255)
    {0, 2, 3},          //TNR_MCTF_RANGE_C_THRESHOLD (0~255)
    2,                  //TNR_MCTF_CLAMP_Y_THRESHOLD (0~255)
    1,                  //TNR_MCTF_CLAMP_Y_LSR (0~7)
    2,                  //TNR_MCTF_CLAMP_C_THRESHOLD (0~255)
    1,                  //TNR_MCTF_CLAMP_C_LSR (0~7)
    //ADDITIONAL TNR Config---------------------------------------------------------------------
	{1, 2, 1},          //NR3D_MCTF_REF_Y_WEIGHT (0~7)
	{1, 2, 1},          //NR3D_MCTF_REF_C_WEIGHT (0~7)
	{1, 2, 1},          //NR3D_P2P_REF_Y_WEIGHT (0~7)
	{1, 2, 1},          //NR3D_P2P_REF_C_WEIGHT (0~7)
    0,                  //enable I frame weight refresh (0: disable, 1:enable)
};

static H26XEncTnr tnr_param_lv2 =
{   0,                  //enable temporal noise reduction (0: disable, 1:enable)
    0,                  //TNR Mode as OSD enabled: fully TNR (0: fully TNR, 1: P2P only, 2: no TNR)
    1,                  //enable mctf/p2p pixel blending (0: disable, 1:enable)
    3,                  //P2P SAD Mode (0: pixel, 3: SAD + pixel)
    2,                  //MCTF SAD Mode (0: pixel, 1: b8x8, 2: b16x16, 3: auto)
    0,                  //MCTF Bias Mode (0: disable, 1: b8x8, 2: b16x16, 3: auto)
    {3, 7, 12},         //TNR_P2P_SAD_THRESHOLD (0~255)
    {8, 6, 2},          //TNR_P2P_WEIGHT (0~16)
    12,                 //TNR_P2P_BORDER_CHECK_THRES (0~255)
    4,                  //TNR_P2P_BORDER_CHECK_SCALE (0~7)
    0,                  //TNR_P2P_INPUT (0: original source, 1: mctf result)
    16,                 //TNR_P2P_MCTF_MOTION_THRESHOLD (0~255)
    {1, 1, 1, 1},       //TNR_P2P_MCTF_MOTION_WEIGHT (0~3)
    //P2P TNR Config----------------------------------------------------------------------------
    {3, 2, 3},          //TNR_P2P_REF_Y_WEIGHT (0~7)
    {3, 2, 3},          //TNR_P2P_REF_C_WEIGHT (0~7)
    {6, 10, 16},        //TNR_P2P_RANGE_Y_THRESHOLD (0~255)
    {0, 2, 3},          //TNR_P2P_RANGE_C_THRESHOLD (0~255)
    2,                  //TNR_P2P_CLAMP_Y_THRESHOLD (0~255)
    1,                  //TNR_P2P_CLAMP_Y_LSR (0~7)
    2,                  //TNR_P2P_CLAMP_C_THRESHOLD (0~255)
    1,                  //TNR_P2P_CLAMP_C_LSR (0~7)
    //MCTF TNR Config----------------------------------------------------------------------------
    {3, 2, 3},          //TNR_MCTF_REF_Y_WEIGHT (0~7)
    {3, 2, 3},          //TNR_MCTF_REF_C_WEIGHT (0~7)
    {5, 6, 7},          //TNR_MCTF_RANGE_Y_THRESHOLD (0~255)
    {0, 2, 3},          //TNR_MCTF_RANGE_C_THRESHOLD (0~255)
    2,                  //TNR_MCTF_CLAMP_Y_THRESHOLD (0~255)
    1,                  //TNR_MCTF_CLAMP_Y_LSR (0~7)
    2,                  //TNR_MCTF_CLAMP_C_THRESHOLD (0~255)
    1,                  //TNR_MCTF_CLAMP_C_LSR (0~7)
    //ADDITIONAL TNR Config---------------------------------------------------------------------
	{1, 2, 1},          //NR3D_MCTF_REF_Y_WEIGHT (0~7)
	{1, 2, 1},          //NR3D_MCTF_REF_C_WEIGHT (0~7)
	{1, 2, 1},          //NR3D_P2P_REF_Y_WEIGHT (0~7)
	{1, 2, 1},          //NR3D_P2P_REF_C_WEIGHT (0~7)
    0,                  //enable I frame weight refresh (0: disable, 1:enable)
};

static H26XEncTnr tnr_param_lv3 =
{   0,                  //enable temporal noise reduction (0: disable, 1:enable)
    0,                  //TNR Mode as OSD enabled: fully TNR (0: fully TNR, 1: P2P only, 2: no TNR)
    1,                  //enable mctf/p2p pixel blending (0: disable, 1:enable)
    3,                  //P2P SAD Mode (0: pixel, 3: SAD + pixel)
    2,                  //MCTF SAD Mode (0: pixel, 1: b8x8, 2: b16x16, 3: auto)
    0,                  //MCTF Bias Mode (0: disable, 1: b8x8, 2: b16x16, 3: auto)
    {3, 7, 12},         //TNR_P2P_SAD_THRESHOLD (0~255)
    {8, 6, 2},          //TNR_P2P_WEIGHT (0~16)
    12,                 //TNR_P2P_BORDER_CHECK_THRES (0~255)
    4,                  //TNR_P2P_BORDER_CHECK_SCALE (0~7)
    0,                  //TNR_P2P_INPUT (0: original source, 1: mctf result)
    16,                 //TNR_P2P_MCTF_MOTION_THRESHOLD (0~255)
    {1, 1, 1, 1},       //TNR_P2P_MCTF_MOTION_WEIGHT (0~3)
    //P2P TNR Config----------------------------------------------------------------------------
    {3, 2, 3},          //TNR_P2P_REF_Y_WEIGHT (0~7)
    {3, 2, 3},          //TNR_P2P_REF_C_WEIGHT (0~7)
    {6, 10, 16},        //TNR_P2P_RANGE_Y_THRESHOLD (0~255)
    {0, 2, 3},          //TNR_P2P_RANGE_C_THRESHOLD (0~255)
    2,                  //TNR_P2P_CLAMP_Y_THRESHOLD (0~255)
    1,                  //TNR_P2P_CLAMP_Y_LSR (0~7)
    2,                  //TNR_P2P_CLAMP_C_THRESHOLD (0~255)
    1,                  //TNR_P2P_CLAMP_C_LSR (0~7)
    //MCTF TNR Config----------------------------------------------------------------------------
    {3, 2, 3},          //TNR_MCTF_REF_Y_WEIGHT (0~7)
    {3, 2, 3},          //TNR_MCTF_REF_C_WEIGHT (0~7)
    {5, 6, 7},          //TNR_MCTF_RANGE_Y_THRESHOLD (0~255)
    {0, 2, 3},          //TNR_MCTF_RANGE_C_THRESHOLD (0~255)
    2,                  //TNR_MCTF_CLAMP_Y_THRESHOLD (0~255)
    1,                  //TNR_MCTF_CLAMP_Y_LSR (0~7)
    2,                  //TNR_MCTF_CLAMP_C_THRESHOLD (0~255)
    1,                  //TNR_MCTF_CLAMP_C_LSR (0~7)
    //ADDITIONAL TNR Config---------------------------------------------------------------------
	{1, 2, 1},          //NR3D_MCTF_REF_Y_WEIGHT (0~7)
	{1, 2, 1},          //NR3D_MCTF_REF_C_WEIGHT (0~7)
	{1, 2, 1},          //NR3D_P2P_REF_Y_WEIGHT (0~7)
	{1, 2, 1},          //NR3D_P2P_REF_C_WEIGHT (0~7)
    0,                  //enable I frame weight refresh (0: disable, 1:enable)
};

static H26XEncTnr tnr_param_lv4 = //ISO800 QP32
{   1,                  //enable temporal noise reduction (0: disable, 1:enable)
    0,                  //TNR Mode as OSD enabled: fully TNR (0: fully TNR, 1: P2P only, 2: no TNR)
    0,                  //enable mctf/p2p pixel blending (0: disable, 1:enable)
    0,                  //P2P SAD Mode (0: pixel, 3: SAD + pixel)
    0,                  //MCTF SAD Mode (0: pixel, 1: b8x8, 2: b16x16, 3: auto)
    0,                  //MCTF Bias Mode (0: disable, 1: b8x8, 2: b16x16, 3: auto)
    {2, 3, 6},         //TNR_P2P_SAD_THRESHOLD (0~255)
    {16, 8, 0},          //TNR_P2P_WEIGHT (0~16)
    5,                 //TNR_P2P_BORDER_CHECK_THRES (0~255)
    4,                  //TNR_P2P_BORDER_CHECK_SCALE (0~7)
    0,                  //TNR_P2P_INPUT (0: original source, 1: mctf result)
    10,                 //TNR_P2P_MCTF_MOTION_THRESHOLD (0~255)
    {1, 1, 1, 1},       //TNR_P2P_MCTF_MOTION_WEIGHT (0~3)
    //P2P TNR Config----------------------------------------------------------------------------
    {1, 3, 1},          //TNR_P2P_REF_Y_WEIGHT (0~7)
    {1, 3, 1},          //TNR_P2P_REF_C_WEIGHT (0~7)
    {8, 15, 30},        //TNR_P2P_RANGE_Y_THRESHOLD (0~255)
    {8, 15, 30},          //TNR_P2P_RANGE_C_THRESHOLD (0~255)
    0,                  //TNR_P2P_CLAMP_Y_THRESHOLD (0~255)
    2,                  //TNR_P2P_CLAMP_Y_LSR (0~7)
    0,                  //TNR_P2P_CLAMP_C_THRESHOLD (0~255)
    2,                  //TNR_P2P_CLAMP_C_LSR (0~7)
    //MCTF TNR Config----------------------------------------------------------------------------
    {1, 4, 1},          //TNR_MCTF_REF_Y_WEIGHT (0~7)
    {1, 4, 1},          //TNR_MCTF_REF_C_WEIGHT (0~7)
    {8, 15, 30},          //TNR_MCTF_RANGE_Y_THRESHOLD (0~255)
    {8, 15, 30},          //TNR_MCTF_RANGE_C_THRESHOLD (0~255)
    0,                  //TNR_MCTF_CLAMP_Y_THRESHOLD (0~255)
    0,                  //TNR_MCTF_CLAMP_Y_LSR (0~7)
    0,                  //TNR_MCTF_CLAMP_C_THRESHOLD (0~255)
    0,                  //TNR_MCTF_CLAMP_C_LSR (0~7)
    //ADDITIONAL TNR Config---------------------------------------------------------------------
	{1, 2, 1},          //NR3D_MCTF_REF_Y_WEIGHT (0~7)
	{1, 2, 1},          //NR3D_MCTF_REF_C_WEIGHT (0~7)
	{1, 2, 1},          //NR3D_P2P_REF_Y_WEIGHT (0~7)
	{1, 2, 1},          //NR3D_P2P_REF_C_WEIGHT (0~7)
    0,                  //enable I frame weight refresh (0: disable, 1:enable)
};

static H26XEncTnr tnr_param_lv5 = //ISO1600 QP32
{   1,                  //enable temporal noise reduction (0: disable, 1:enable)
    0,                  //TNR Mode as OSD enabled: fully TNR (0: fully TNR, 1: P2P only, 2: no TNR)
    1,                  //enable mctf/p2p pixel blending (0: disable, 1:enable)
    0,                  //P2P SAD Mode (0: pixel, 3: SAD + pixel)
    0,                  //MCTF SAD Mode (0: pixel, 1: b8x8, 2: b16x16, 3: auto)
    0,                  //MCTF Bias Mode (0: disable, 1: b8x8, 2: b16x16, 3: auto)
    {2, 4, 6},         //TNR_P2P_SAD_THRESHOLD (0~255)
    {16, 8, 0},          //TNR_P2P_WEIGHT (0~16)
    5,                 //TNR_P2P_BORDER_CHECK_THRES (0~255)
    4,                  //TNR_P2P_BORDER_CHECK_SCALE (0~7)
    0,                  //TNR_P2P_INPUT (0: original source, 1: mctf result)
    10,                 //TNR_P2P_MCTF_MOTION_THRESHOLD (0~255)
    {1, 1, 1, 1},       //TNR_P2P_MCTF_MOTION_WEIGHT (0~3)
    //P2P TNR Config----------------------------------------------------------------------------
    {1, 3, 1},          //TNR_P2P_REF_Y_WEIGHT (0~7)
    {1, 3, 1},          //TNR_P2P_REF_C_WEIGHT (0~7)
    {8, 15, 30},        //TNR_P2P_RANGE_Y_THRESHOLD (0~255)
    {8, 15, 30},          //TNR_P2P_RANGE_C_THRESHOLD (0~255)
    0,                  //TNR_P2P_CLAMP_Y_THRESHOLD (0~255)
    2,                  //TNR_P2P_CLAMP_Y_LSR (0~7)
    0,                  //TNR_P2P_CLAMP_C_THRESHOLD (0~255)
    2,                  //TNR_P2P_CLAMP_C_LSR (0~7)
    //MCTF TNR Config----------------------------------------------------------------------------
    {1, 4, 1},          //TNR_MCTF_REF_Y_WEIGHT (0~7)
    {1, 4, 1},          //TNR_MCTF_REF_C_WEIGHT (0~7)
    {8, 15, 30},          //TNR_MCTF_RANGE_Y_THRESHOLD (0~255)
    {8, 15, 30},          //TNR_MCTF_RANGE_C_THRESHOLD (0~255)
    0,                  //TNR_MCTF_CLAMP_Y_THRESHOLD (0~255)
    0,                  //TNR_MCTF_CLAMP_Y_LSR (0~7)
    0,                  //TNR_MCTF_CLAMP_C_THRESHOLD (0~255)
    0,                  //TNR_MCTF_CLAMP_C_LSR (0~7)
    //ADDITIONAL TNR Config---------------------------------------------------------------------
	{1, 2, 1},          //NR3D_MCTF_REF_Y_WEIGHT (0~7)
	{1, 2, 1},          //NR3D_MCTF_REF_C_WEIGHT (0~7)
	{1, 2, 1},          //NR3D_P2P_REF_Y_WEIGHT (0~7)
	{1, 2, 1},          //NR3D_P2P_REF_C_WEIGHT (0~7)
    0,                  //enable I frame weight refresh (0: disable, 1:enable)
};

static H26XEncTnr tnr_param_lv6 = //ISO3200 QP32
{   1,                  //enable temporal noise reduction (0: disable, 1:enable)
    0,                  //TNR Mode as OSD enabled: fully TNR (0: fully TNR, 1: P2P only, 2: no TNR)
    1,                  //enable mctf/p2p pixel blending (0: disable, 1:enable)
    0,                  //P2P SAD Mode (0: pixel, 3: SAD + pixel)
    0,                  //MCTF SAD Mode (0: pixel, 1: b8x8, 2: b16x16, 3: auto)
    0,                  //MCTF Bias Mode (0: disable, 1: b8x8, 2: b16x16, 3: auto)
    {3, 5, 8},         //TNR_P2P_SAD_THRESHOLD (0~255)
    {16, 8, 0},          //TNR_P2P_WEIGHT (0~16)
    5,                 //TNR_P2P_BORDER_CHECK_THRES (0~255)
    4,                  //TNR_P2P_BORDER_CHECK_SCALE (0~7)
    0,                  //TNR_P2P_INPUT (0: original source, 1: mctf result)
    10,                 //TNR_P2P_MCTF_MOTION_THRESHOLD (0~255)
    {1, 1, 1, 1},       //TNR_P2P_MCTF_MOTION_WEIGHT (0~3)
    //P2P TNR Config----------------------------------------------------------------------------
    {1, 5, 2},          //TNR_P2P_REF_Y_WEIGHT (0~7)
    {1, 5, 2},          //TNR_P2P_REF_C_WEIGHT (0~7)
    {10, 20, 30},        //TNR_P2P_RANGE_Y_THRESHOLD (0~255)
    {10, 20, 30},          //TNR_P2P_RANGE_C_THRESHOLD (0~255)
    0,                  //TNR_P2P_CLAMP_Y_THRESHOLD (0~255)
    2,                  //TNR_P2P_CLAMP_Y_LSR (0~7)
    0,                  //TNR_P2P_CLAMP_C_THRESHOLD (0~255)
    2,                  //TNR_P2P_CLAMP_C_LSR (0~7)
    //MCTF TNR Config----------------------------------------------------------------------------
    {1, 5, 2},          //TNR_MCTF_REF_Y_WEIGHT (0~7)
    {1, 5, 2},          //TNR_MCTF_REF_C_WEIGHT (0~7)
    {10, 20, 30},          //TNR_MCTF_RANGE_Y_THRESHOLD (0~255)
    {10, 20, 30},          //TNR_MCTF_RANGE_C_THRESHOLD (0~255)
    0,                  //TNR_MCTF_CLAMP_Y_THRESHOLD (0~255)
    0,                  //TNR_MCTF_CLAMP_Y_LSR (0~7)
    0,                  //TNR_MCTF_CLAMP_C_THRESHOLD (0~255)
    0,                  //TNR_MCTF_CLAMP_C_LSR (0~7)
    //ADDITIONAL TNR Config---------------------------------------------------------------------
	{1, 2, 1},          //NR3D_MCTF_REF_Y_WEIGHT (0~7)
	{1, 2, 1},          //NR3D_MCTF_REF_C_WEIGHT (0~7)
	{1, 2, 1},          //NR3D_P2P_REF_Y_WEIGHT (0~7)
	{1, 2, 1},          //NR3D_P2P_REF_C_WEIGHT (0~7)
    0,                  //enable I frame weight refresh (0: disable, 1:enable)
};

static H26XEncTnr tnr_param_lv7 = //ISO6400 QP32
{   1,                  //enable temporal noise reduction (0: disable, 1:enable)
    0,                  //TNR Mode as OSD enabled: fully TNR (0: fully TNR, 1: P2P only, 2: no TNR)
    1,                  //enable mctf/p2p pixel blending (0: disable, 1:enable)
    0,                  //P2P SAD Mode (0: pixel, 3: SAD + pixel)
    0,                  //MCTF SAD Mode (0: pixel, 1: b8x8, 2: b16x16, 3: auto)
    0,                  //MCTF Bias Mode (0: disable, 1: b8x8, 2: b16x16, 3: auto)
    {3, 6, 8},         //TNR_P2P_SAD_THRESHOLD (0~255)
    {16, 8, 0},          //TNR_P2P_WEIGHT (0~16)
    10,                 //TNR_P2P_BORDER_CHECK_THRES (0~255)
    4,                  //TNR_P2P_BORDER_CHECK_SCALE (0~7)
    0,                  //TNR_P2P_INPUT (0: original source, 1: mctf result)
    10,                 //TNR_P2P_MCTF_MOTION_THRESHOLD (0~255)
    {1, 1, 1, 1},       //TNR_P2P_MCTF_MOTION_WEIGHT (0~3)
    //P2P TNR Config----------------------------------------------------------------------------
    {1, 5, 2},          //TNR_P2P_REF_Y_WEIGHT (0~7)
    {1, 5, 2},          //TNR_P2P_REF_C_WEIGHT (0~7)
    {10, 20, 30},        //TNR_P2P_RANGE_Y_THRESHOLD (0~255)
    {10, 20, 30},          //TNR_P2P_RANGE_C_THRESHOLD (0~255)
    0,                  //TNR_P2P_CLAMP_Y_THRESHOLD (0~255)
    2,                  //TNR_P2P_CLAMP_Y_LSR (0~7)
    0,                  //TNR_P2P_CLAMP_C_THRESHOLD (0~255)
    2,                  //TNR_P2P_CLAMP_C_LSR (0~7)
    //MCTF TNR Config----------------------------------------------------------------------------
    {1, 5, 2},          //TNR_MCTF_REF_Y_WEIGHT (0~7)
    {1, 5, 2},          //TNR_MCTF_REF_C_WEIGHT (0~7)
    {10, 20, 30},          //TNR_MCTF_RANGE_Y_THRESHOLD (0~255)
    {10, 20, 30},          //TNR_MCTF_RANGE_C_THRESHOLD (0~255)
    0,                  //TNR_MCTF_CLAMP_Y_THRESHOLD (0~255)
    0,                  //TNR_MCTF_CLAMP_Y_LSR (0~7)
    0,                  //TNR_MCTF_CLAMP_C_THRESHOLD (0~255)
    0,                  //TNR_MCTF_CLAMP_C_LSR (0~7)
    //ADDITIONAL TNR Config---------------------------------------------------------------------
	{1, 2, 1},          //NR3D_MCTF_REF_Y_WEIGHT (0~7)
	{1, 2, 1},          //NR3D_MCTF_REF_C_WEIGHT (0~7)
	{1, 2, 1},          //NR3D_P2P_REF_Y_WEIGHT (0~7)
	{1, 2, 1},          //NR3D_P2P_REF_C_WEIGHT (0~7)
    0,                  //enable I frame weight refresh (0: disable, 1:enable)
};

static H26XEncTnr tnr_param_lv8 = //ISO6400 QP32
{   1,                  //enable temporal noise reduction (0: disable, 1:enable)
    0,                  //TNR Mode as OSD enabled: fully TNR (0: fully TNR, 1: P2P only, 2: no TNR)
    1,                  //enable mctf/p2p pixel blending (0: disable, 1:enable)
    0,                  //P2P SAD Mode (0: pixel, 3: SAD + pixel)
    0,                  //MCTF SAD Mode (0: pixel, 1: b8x8, 2: b16x16, 3: auto)
    0,                  //MCTF Bias Mode (0: disable, 1: b8x8, 2: b16x16, 3: auto)
    {3, 6, 8},         //TNR_P2P_SAD_THRESHOLD (0~255)
    {16, 8, 0},          //TNR_P2P_WEIGHT (0~16)
    10,                 //TNR_P2P_BORDER_CHECK_THRES (0~255)
    4,                  //TNR_P2P_BORDER_CHECK_SCALE (0~7)
    0,                  //TNR_P2P_INPUT (0: original source, 1: mctf result)
    10,                 //TNR_P2P_MCTF_MOTION_THRESHOLD (0~255)
    {1, 1, 1, 1},       //TNR_P2P_MCTF_MOTION_WEIGHT (0~3)
    //P2P TNR Config----------------------------------------------------------------------------
    {1, 5, 2},          //TNR_P2P_REF_Y_WEIGHT (0~7)
    {1, 5, 2},          //TNR_P2P_REF_C_WEIGHT (0~7)
    {10, 20, 30},        //TNR_P2P_RANGE_Y_THRESHOLD (0~255)
    {10, 20, 30},          //TNR_P2P_RANGE_C_THRESHOLD (0~255)
    0,                  //TNR_P2P_CLAMP_Y_THRESHOLD (0~255)
    2,                  //TNR_P2P_CLAMP_Y_LSR (0~7)
    0,                  //TNR_P2P_CLAMP_C_THRESHOLD (0~255)
    2,                  //TNR_P2P_CLAMP_C_LSR (0~7)
    //MCTF TNR Config----------------------------------------------------------------------------
    {1, 5, 2},          //TNR_MCTF_REF_Y_WEIGHT (0~7)
    {1, 5, 2},          //TNR_MCTF_REF_C_WEIGHT (0~7)
    {10, 20, 30},          //TNR_MCTF_RANGE_Y_THRESHOLD (0~255)
    {10, 20, 30},          //TNR_MCTF_RANGE_C_THRESHOLD (0~255)
    0,                  //TNR_MCTF_CLAMP_Y_THRESHOLD (0~255)
    0,                  //TNR_MCTF_CLAMP_Y_LSR (0~7)
    0,                  //TNR_MCTF_CLAMP_C_THRESHOLD (0~255)
    0,                  //TNR_MCTF_CLAMP_C_LSR (0~7)
    //ADDITIONAL TNR Config---------------------------------------------------------------------
	{1, 2, 1},          //NR3D_MCTF_REF_Y_WEIGHT (0~7)
	{1, 2, 1},          //NR3D_MCTF_REF_C_WEIGHT (0~7)
	{1, 2, 1},          //NR3D_P2P_REF_Y_WEIGHT (0~7)
	{1, 2, 1},          //NR3D_P2P_REF_C_WEIGHT (0~7)
    0,                  //enable I frame weight refresh (0: disable, 1:enable)
};

static H26XEncTnr tnr_param_lv9 =
{   1,                  //enable temporal noise reduction (0: disable, 1:enable)
    0,                  //TNR Mode as OSD enabled: fully TNR (0: fully TNR, 1: P2P only, 2: no TNR)
    1,                  //enable mctf/p2p pixel blending (0: disable, 1:enable)
    0,                  //P2P SAD Mode (0: pixel, 3: SAD + pixel)
    0,                  //MCTF SAD Mode (0: pixel, 1: b8x8, 2: b16x16, 3: auto)
    0,                  //MCTF Bias Mode (0: disable, 1: b8x8, 2: b16x16, 3: auto)
    {3, 6, 8},         //TNR_P2P_SAD_THRESHOLD (0~255)
    {16, 8, 0},          //TNR_P2P_WEIGHT (0~16)
    10,                 //TNR_P2P_BORDER_CHECK_THRES (0~255)
    4,                  //TNR_P2P_BORDER_CHECK_SCALE (0~7)
    0,                  //TNR_P2P_INPUT (0: original source, 1: mctf result)
    10,                 //TNR_P2P_MCTF_MOTION_THRESHOLD (0~255)
    {1, 1, 1, 1},       //TNR_P2P_MCTF_MOTION_WEIGHT (0~3)
    //P2P TNR Config----------------------------------------------------------------------------
    {1, 5, 2},          //TNR_P2P_REF_Y_WEIGHT (0~7)
    {1, 5, 2},          //TNR_P2P_REF_C_WEIGHT (0~7)
    {10, 20, 30},        //TNR_P2P_RANGE_Y_THRESHOLD (0~255)
    {10, 20, 30},          //TNR_P2P_RANGE_C_THRESHOLD (0~255)
    0,                  //TNR_P2P_CLAMP_Y_THRESHOLD (0~255)
    2,                  //TNR_P2P_CLAMP_Y_LSR (0~7)
    0,                  //TNR_P2P_CLAMP_C_THRESHOLD (0~255)
    2,                  //TNR_P2P_CLAMP_C_LSR (0~7)
    //MCTF TNR Config----------------------------------------------------------------------------
    {1, 5, 2},          //TNR_MCTF_REF_Y_WEIGHT (0~7)
    {1, 5, 2},          //TNR_MCTF_REF_C_WEIGHT (0~7)
    {10, 20, 30},          //TNR_MCTF_RANGE_Y_THRESHOLD (0~255)
    {10, 20, 30},          //TNR_MCTF_RANGE_C_THRESHOLD (0~255)
    0,                  //TNR_MCTF_CLAMP_Y_THRESHOLD (0~255)
    0,                  //TNR_MCTF_CLAMP_Y_LSR (0~7)
    0,                  //TNR_MCTF_CLAMP_C_THRESHOLD (0~255)
    0,                  //TNR_MCTF_CLAMP_C_LSR (0~7)
    //ADDITIONAL TNR Config---------------------------------------------------------------------
	{1, 2, 1},          //NR3D_MCTF_REF_Y_WEIGHT (0~7)
	{1, 2, 1},          //NR3D_MCTF_REF_C_WEIGHT (0~7)
	{1, 2, 1},          //NR3D_P2P_REF_Y_WEIGHT (0~7)
	{1, 2, 1},          //NR3D_P2P_REF_C_WEIGHT (0~7)
    0,                  //enable I frame weight refresh (0: disable, 1:enable)
};

static H26XEncTnr tnr_param_lv10 =
{   1,                  //enable temporal noise reduction (0: disable, 1:enable)
    0,                  //TNR Mode as OSD enabled: fully TNR (0: fully TNR, 1: P2P only, 2: no TNR)
    1,                  //enable mctf/p2p pixel blending (0: disable, 1:enable)
    0,                  //P2P SAD Mode (0: pixel, 3: SAD + pixel)
    0,                  //MCTF SAD Mode (0: pixel, 1: b8x8, 2: b16x16, 3: auto)
    0,                  //MCTF Bias Mode (0: disable, 1: b8x8, 2: b16x16, 3: auto)
    {3, 6, 8},         //TNR_P2P_SAD_THRESHOLD (0~255)
    {16, 8, 0},          //TNR_P2P_WEIGHT (0~16)
    10,                 //TNR_P2P_BORDER_CHECK_THRES (0~255)
    4,                  //TNR_P2P_BORDER_CHECK_SCALE (0~7)
    0,                  //TNR_P2P_INPUT (0: original source, 1: mctf result)
    10,                 //TNR_P2P_MCTF_MOTION_THRESHOLD (0~255)
    {1, 1, 1, 1},       //TNR_P2P_MCTF_MOTION_WEIGHT (0~3)
    //P2P TNR Config----------------------------------------------------------------------------
    {1, 5, 2},          //TNR_P2P_REF_Y_WEIGHT (0~7)
    {1, 5, 2},          //TNR_P2P_REF_C_WEIGHT (0~7)
    {10, 20, 30},        //TNR_P2P_RANGE_Y_THRESHOLD (0~255)
    {10, 20, 30},          //TNR_P2P_RANGE_C_THRESHOLD (0~255)
    0,                  //TNR_P2P_CLAMP_Y_THRESHOLD (0~255)
    2,                  //TNR_P2P_CLAMP_Y_LSR (0~7)
    0,                  //TNR_P2P_CLAMP_C_THRESHOLD (0~255)
    2,                  //TNR_P2P_CLAMP_C_LSR (0~7)
    //MCTF TNR Config----------------------------------------------------------------------------
    {1, 5, 2},          //TNR_MCTF_REF_Y_WEIGHT (0~7)
    {1, 5, 2},          //TNR_MCTF_REF_C_WEIGHT (0~7)
    {10, 20, 30},          //TNR_MCTF_RANGE_Y_THRESHOLD (0~255)
    {10, 20, 30},          //TNR_MCTF_RANGE_C_THRESHOLD (0~255)
    0,                  //TNR_MCTF_CLAMP_Y_THRESHOLD (0~255)
    0,                  //TNR_MCTF_CLAMP_Y_LSR (0~7)
    0,                  //TNR_MCTF_CLAMP_C_THRESHOLD (0~255)
    0,                  //TNR_MCTF_CLAMP_C_LSR (0~7)
    //ADDITIONAL TNR Config---------------------------------------------------------------------
	{1, 2, 1},          //NR3D_MCTF_REF_Y_WEIGHT (0~7)
	{1, 2, 1},          //NR3D_MCTF_REF_C_WEIGHT (0~7)
	{1, 2, 1},          //NR3D_P2P_REF_Y_WEIGHT (0~7)
	{1, 2, 1},          //NR3D_P2P_REF_C_WEIGHT (0~7)
    0,                  //enable I frame weight refresh (0: disable, 1:enable)
};

static H26XEncTnr tnr_param_lv11 =
{   1,                  //enable temporal noise reduction (0: disable, 1:enable)
    0,                  //TNR Mode as OSD enabled: fully TNR (0: fully TNR, 1: P2P only, 2: no TNR)
    1,                  //enable mctf/p2p pixel blending (0: disable, 1:enable)
    0,                  //P2P SAD Mode (0: pixel, 3: SAD + pixel)
    0,                  //MCTF SAD Mode (0: pixel, 1: b8x8, 2: b16x16, 3: auto)
    0,                  //MCTF Bias Mode (0: disable, 1: b8x8, 2: b16x16, 3: auto)
    {3, 6, 8},         //TNR_P2P_SAD_THRESHOLD (0~255)
    {16, 8, 0},          //TNR_P2P_WEIGHT (0~16)
    10,                 //TNR_P2P_BORDER_CHECK_THRES (0~255)
    4,                  //TNR_P2P_BORDER_CHECK_SCALE (0~7)
    0,                  //TNR_P2P_INPUT (0: original source, 1: mctf result)
    10,                 //TNR_P2P_MCTF_MOTION_THRESHOLD (0~255)
    {1, 1, 1, 1},       //TNR_P2P_MCTF_MOTION_WEIGHT (0~3)
    //P2P TNR Config----------------------------------------------------------------------------
    {1, 5, 2},          //TNR_P2P_REF_Y_WEIGHT (0~7)
    {1, 5, 2},          //TNR_P2P_REF_C_WEIGHT (0~7)
    {10, 20, 30},        //TNR_P2P_RANGE_Y_THRESHOLD (0~255)
    {10, 20, 30},          //TNR_P2P_RANGE_C_THRESHOLD (0~255)
    0,                  //TNR_P2P_CLAMP_Y_THRESHOLD (0~255)
    2,                  //TNR_P2P_CLAMP_Y_LSR (0~7)
    0,                  //TNR_P2P_CLAMP_C_THRESHOLD (0~255)
    2,                  //TNR_P2P_CLAMP_C_LSR (0~7)
    //MCTF TNR Config----------------------------------------------------------------------------
    {1, 5, 2},          //TNR_MCTF_REF_Y_WEIGHT (0~7)
    {1, 5, 2},          //TNR_MCTF_REF_C_WEIGHT (0~7)
    {10, 20, 30},          //TNR_MCTF_RANGE_Y_THRESHOLD (0~255)
    {10, 20, 30},          //TNR_MCTF_RANGE_C_THRESHOLD (0~255)
    0,                  //TNR_MCTF_CLAMP_Y_THRESHOLD (0~255)
    0,                  //TNR_MCTF_CLAMP_Y_LSR (0~7)
    0,                  //TNR_MCTF_CLAMP_C_THRESHOLD (0~255)
    0,                  //TNR_MCTF_CLAMP_C_LSR (0~7)
    //ADDITIONAL TNR Config---------------------------------------------------------------------
	{1, 2, 1},          //NR3D_MCTF_REF_Y_WEIGHT (0~7)
	{1, 2, 1},          //NR3D_MCTF_REF_C_WEIGHT (0~7)
	{1, 2, 1},          //NR3D_P2P_REF_Y_WEIGHT (0~7)
	{1, 2, 1},          //NR3D_P2P_REF_C_WEIGHT (0~7)
    0,                  //enable I frame weight refresh (0: disable, 1:enable)
};

static H26XEncTnr tnr_param_lv12 =
{   1,                  //enable temporal noise reduction (0: disable, 1:enable)
    0,                  //TNR Mode as OSD enabled: fully TNR (0: fully TNR, 1: P2P only, 2: no TNR)
    1,                  //enable mctf/p2p pixel blending (0: disable, 1:enable)
    0,                  //P2P SAD Mode (0: pixel, 3: SAD + pixel)
    0,                  //MCTF SAD Mode (0: pixel, 1: b8x8, 2: b16x16, 3: auto)
    0,                  //MCTF Bias Mode (0: disable, 1: b8x8, 2: b16x16, 3: auto)
    {3, 6, 8},         //TNR_P2P_SAD_THRESHOLD (0~255)
    {16, 8, 0},          //TNR_P2P_WEIGHT (0~16)
    10,                 //TNR_P2P_BORDER_CHECK_THRES (0~255)
    4,                  //TNR_P2P_BORDER_CHECK_SCALE (0~7)
    0,                  //TNR_P2P_INPUT (0: original source, 1: mctf result)
    10,                 //TNR_P2P_MCTF_MOTION_THRESHOLD (0~255)
    {1, 1, 1, 1},       //TNR_P2P_MCTF_MOTION_WEIGHT (0~3)
    //P2P TNR Config----------------------------------------------------------------------------
    {1, 5, 2},          //TNR_P2P_REF_Y_WEIGHT (0~7)
    {1, 5, 2},          //TNR_P2P_REF_C_WEIGHT (0~7)
    {10, 20, 30},        //TNR_P2P_RANGE_Y_THRESHOLD (0~255)
    {10, 20, 30},          //TNR_P2P_RANGE_C_THRESHOLD (0~255)
    0,                  //TNR_P2P_CLAMP_Y_THRESHOLD (0~255)
    2,                  //TNR_P2P_CLAMP_Y_LSR (0~7)
    0,                  //TNR_P2P_CLAMP_C_THRESHOLD (0~255)
    2,                  //TNR_P2P_CLAMP_C_LSR (0~7)
    //MCTF TNR Config----------------------------------------------------------------------------
    {1, 5, 2},          //TNR_MCTF_REF_Y_WEIGHT (0~7)
    {1, 5, 2},          //TNR_MCTF_REF_C_WEIGHT (0~7)
    {10, 20, 30},          //TNR_MCTF_RANGE_Y_THRESHOLD (0~255)
    {10, 20, 30},          //TNR_MCTF_RANGE_C_THRESHOLD (0~255)
    0,                  //TNR_MCTF_CLAMP_Y_THRESHOLD (0~255)
    0,                  //TNR_MCTF_CLAMP_Y_LSR (0~7)
    0,                  //TNR_MCTF_CLAMP_C_THRESHOLD (0~255)
    0,                  //TNR_MCTF_CLAMP_C_LSR (0~7)
    //ADDITIONAL TNR Config---------------------------------------------------------------------
	{1, 2, 1},          //NR3D_MCTF_REF_Y_WEIGHT (0~7)
	{1, 2, 1},          //NR3D_MCTF_REF_C_WEIGHT (0~7)
	{1, 2, 1},          //NR3D_P2P_REF_Y_WEIGHT (0~7)
	{1, 2, 1},          //NR3D_P2P_REF_C_WEIGHT (0~7)
    0,                  //enable I frame weight refresh (0: disable, 1:enable)
};

static INT8 tnr_level[TOTAL_GAIN_NUM]=
{
    H264_UI_3DNR_LV1,
    H264_UI_3DNR_LV2,//ISO200
    H264_UI_3DNR_LV3,
    H264_UI_3DNR_LV4,//ISO800
    H264_UI_3DNR_LV5,
    H264_UI_3DNR_LV6,//3200
    H264_UI_3DNR_LV7,
    H264_UI_3DNR_LV8,//12800
    H264_UI_3DNR_LV9,
    H264_UI_3DNR_LV10,
    H264_UI_3DNR_LV11,
    H264_UI_3DNR_LV12,
};

static H26XEncTnr* tnr_param[TOTAL_UI_3DNR_LV]=
{
    &tnr_param_off,
    &tnr_param_lv1,
    &tnr_param_lv2,
    &tnr_param_lv3,
    &tnr_param_lv4,
    &tnr_param_lv5,
    &tnr_param_lv6,
    &tnr_param_lv7,
    &tnr_param_lv8,
    &tnr_param_lv9,
    &tnr_param_lv10,
    &tnr_param_lv11,
    &tnr_param_lv12,
};

static sISP_ISOINDEX gISO_Index[Total_SENIDNum] = {0};
void iq_param_sample_evb_table1_init(UINT32 id,UINT32 mode_idx)
{
	//iqs_set_range
	UINT32 *iq_param_addr[IQ_PARAM_MAX];
	static UINT32 num_3DNR_param = TOTAL_UI_3DNR_LV;
	gISO_Index[id].Id = id;
    gISO_Index[id].num = IQS_ISOMAPTAB_NUM - 1;

	iq_param_addr[IQ_ISOIDX] = (UINT32 *)&gISO_Index[id];

    iq_param_addr[IQ_ISOIDX_MAP ] = (UINT32*)&iso_map_tab_sample_evb[0];

    iq_param_addr[IQ_ENGINE_CTRL ] = (UINT32*)&e_ctrl;

	//***********
	//**  SIE  **
	//***********
    iq_param_addr[IQ_OBSUB      ] = (UINT32 *)ob[0];

	iq_param_addr[IQ_ECS_TAB    ] = (UINT32 *)ecs_tab;
	iq_param_addr[IQ_ECS        ] = (UINT32 *)&ecs;

    //***********
    //**  RHE  **
    //***********
    iq_param_addr[IQ_WDR            ] = (UINT32*) &wdr_strength[0];
    iq_param_addr[IQ_WDR_LCE_OFFSET ] = (UINT32*) &wdr_lce_offset;
    iq_param_addr[IQ_LCE            ] = (UINT32*) &lce_strength[0];
    iq_param_addr[IQ_GCE			] = (UINT32*) &gce_strength[0];

	//***********
	//**  IFE  **
	//***********
	iq_param_addr[IQ_OUTL_BRI   ] = (UINT32 *)outl_bri[0];
	iq_param_addr[IQ_OUTL_DARK  ] = (UINT32 *)outl_dark[0];
	iq_param_addr[IQ_OUTL_CNT   ] = (UINT32 *)outl_cnt[0];
	iq_param_addr[IQ_OUTL       ] = (UINT32 *)&outl_set[0];

	iq_param_addr[IQ_SPATIAL_W  ] = (UINT32 *)spatial_w[0];
	iq_param_addr[IQ_SPATAL     ] = (UINT32 *)&spatial[0];

	iq_param_addr[IQ_NLM_KER_RAD] = (UINT32 *)range_ker_radius[0];
	iq_param_addr[IQ_NLM_KER    ] = (UINT32 *)&range_ker[0];
	iq_param_addr[IQ_NLM_MW     ] = (UINT32 *)range_mwth[0];
	iq_param_addr[IQ_NLM_BWA    ] = (UINT32 *)range_bwa[0];
	iq_param_addr[IQ_NLM_BWB    ] = (UINT32 *)range_bwb[0];
	iq_param_addr[IQ_NLM_BWC    ] = (UINT32 *)range_bwc[0];
	iq_param_addr[IQ_NLM_BWBL   ] = (UINT32 *)range_bwbl[0];
	iq_param_addr[IQ_NLM_BWBM   ] = (UINT32 *)range_bwbm[0];
	iq_param_addr[IQ_NLM_BWBH   ] = (UINT32 *)range_bwbh[0];

	iq_param_addr[IQ_RANGEA_THR ] = (UINT32 *)range_a_thr[0];
	iq_param_addr[IQ_RANGEA_LUT ] = (UINT32 *)range_a_lut[0];
	iq_param_addr[IQ_RANGEB_THR ] = (UINT32 *)range_b_thr[0];
	iq_param_addr[IQ_RANGEB_LUT ] = (UINT32 *)range_b_lut[0];
	iq_param_addr[IQ_RANGEB     ] = (UINT32 *)&range_b[0];

	iq_param_addr[IQ_CLAMP      ] = (UINT32 *)&filter_clamp[0];

	iq_param_addr[IQ_VIG_CENTER ] = (UINT32 *)&vig_center;
	iq_param_addr[IQ_VIG_TAB    ] = (UINT32 *)&vig;
    iq_param_addr[IQ_VIG_ISO_TH ] = (UINT32 *)&vig_iso_th;

	iq_param_addr[IQ_GBAL_STAB  ] = (UINT32 *)gbal_s_tab[0];
	iq_param_addr[IQ_GBAL_DTAB  ] = (UINT32 *)gbal_d_tab[0];
	iq_param_addr[IQ_GBAL_IRSUB ] = (UINT32 *)gbal_ir_sub[0];
	iq_param_addr[IQ_GBAL       ] = (UINT32 *)&gbal[0];

	//***********
	//**  DCE  **
	//***********
    iq_param_addr[IQ_DCE_DIST	] = (UINT32*)&dist_sample_evb;

    iq_param_addr[IQ_DCE_RADIOUS] = (UINT32*)&radious_sample_evb;

    iq_param_addr[IQ_CFAINTER	] = (UINT32*)&cfa_sample_evb;

	iq_param_addr[IQ_GDC_FOV	] = (UINT32*)&fov_sample_evb;
    iq_param_addr[IQ_GDC_GLUT	] = (UINT32*)ldc_g_lut_sample_evb;
    iq_param_addr[IQ_GDC_RLUT	] = (UINT32*)ldc_r_lut_sample_evb;
    iq_param_addr[IQ_GDC_BLUT	] = (UINT32*)ldc_b_lut_sample_evb;
	iq_param_addr[IQ_DCE_ENH    ] = (UINT32 *)&ldc_enh;

	iq_param_addr[IQ_DCE_CA	    ] = (UINT32*)&cac_sample_evb;

	iq_param_addr[IQ_DCE_2D_PARAM   ] = (UINT32*)&dc_2d_param_sample_evb[0];
	iq_param_addr[IQ_DCE_2DLUT00    ] = (UINT32*)dc_2dlut00_sample_evb;
	iq_param_addr[IQ_DCE_2DLUT01    ] = (UINT32*)dc_2dlut01_sample_evb;
	iq_param_addr[IQ_DCE_2DLUT02    ] = (UINT32*)dc_2dlut02_sample_evb;
	iq_param_addr[IQ_DCE_2DLUT03    ] = (UINT32*)dc_2dlut03_sample_evb;

	iq_param_addr[IQ_DCE_STRP_INFO  ] = (UINT32*)&strp_info_sample_evb[0];

	//***********
	//**  IPE  **
	//***********
	iq_param_addr[IQ_EDGEKER    ] = (UINT32 *)edge_kernel[0];
	iq_param_addr[IQ_DIR_TAB    ] = (UINT32 *)edir_tab[0];
	iq_param_addr[IQ_DIREDGE_EXT] = (UINT32 *)&edir_ext[0];
	iq_param_addr[IQ_DIRSCORETAB] = (UINT32 *)&score_tab[0];
	iq_param_addr[IQ_DIRSCORETH ] = (UINT32 *)&score_th[0];
	iq_param_addr[IQ_DECON      ] = (UINT32 *)&decon[0];
	iq_param_addr[IQ_EWA        ] = (UINT32 *)&ewa[0];
	iq_param_addr[IQ_EWB        ] = (UINT32 *)&ewb[0];
	iq_param_addr[IQ_EWPARAM    ] = (UINT32 *)&ewparam[0];

	iq_param_addr[IQ_EDGEENH    ] = (UINT32 *)&edge_enh[0];

	iq_param_addr[IQ_EDMAP      ] = (UINT32 *)&ed_map[0];
	iq_param_addr[IQ_EDTAB      ] = (UINT32 *)ed_tab[0];
	iq_param_addr[IQ_ESMAP      ] = (UINT32 *)&es_map[0];
	iq_param_addr[IQ_ESTAB      ] = (UINT32 *)es_tab[0];

	iq_param_addr[IQ_RGBLPF     ] = (UINT32 *)&lpf[0];

    iq_param_addr[IQ_CCM_TH     ] = (UINT32*)&color_temperature_th_sample_evb;
	iq_param_addr[IQ_Color_L    ] = (UINT32*)&color_low_sample_evb;
	iq_param_addr[IQ_Color_M    ] = (UINT32*)&color_middle_sample_evb;
	iq_param_addr[IQ_Color_H    ] = (UINT32*)&color_high_sample_evb;
    iq_param_addr[IQ_FSTAB      ] = (UINT32*)fs_tab_sample_evb[0];
	iq_param_addr[IQ_FDTAB	    ] = (UINT32*)fd_tab_sample_evb[0];
    iq_param_addr[IQ_CC         ] = (UINT32*)&cc_sample_evb;

    iq_param_addr[IQ_Y_CURVE    ] = (UINT32*)y_curve_sample_evb;

	iq_param_addr[IQ_GAMMA_TH	] = (UINT32*)&gamma_lv_th_sample_evb;
    iq_param_addr[IQ_GAMMA_DAY	] = (UINT32*)gamma_day_sample_evb;
	iq_param_addr[IQ_GAMMA_NIGHT] = (UINT32*)gamma_night_sample_evb;

	iq_param_addr[IQ_3DCC_TAB   ] = (UINT32*)color_3d_lut_sample_evb;
    iq_param_addr[IQ_3DCC_ROUND ] = (UINT32*)&color_3d_sample_evb;

	iq_param_addr[IQ_EDGTAB     ] = (UINT32*)edg_tab_sample_evb;
    iq_param_addr[IQ_DDSTAB     ] = (UINT32*)dds_tab_sample_evb;

	iq_param_addr[IQ_SATURATION ] = (UINT32 *)&saturation[0];
	iq_param_addr[IQ_CONTRAST   ] = (UINT32 *)&contrast[0];
    iq_param_addr[IQ_CCTRL_INFO ] = (UINT32 *)&cctrl_info[0];

	iq_param_addr[IQ_CST_COEF	] = (UINT32*)cst_coef_sample_evb;
    iq_param_addr[IQ_CST_PARAM	] = (UINT32*)&cst_sample_evb;

	iq_param_addr[IQ_YCCFIX	    ] = (UINT32*)&ycc_fix_sample_evb;

    iq_param_addr[IQ_NOISE_PARAM] = (UINT32*)&noise_param_sample_evb;

    iq_param_addr[IQ_GAMMA_RAND ] = (UINT32*)&gamma_rand_sample_evb;

	//***********
	//** IFE2  **
	//***********
	iq_param_addr[IQ_REFCENTER_YTH  ] = (UINT32 *)ref_center_y_th[0];
	iq_param_addr[IQ_REFCENTER_YWT  ] = (UINT32 *)ref_center_y_wt[0];
	iq_param_addr[IQ_REFCENTER_Y    ] = (UINT32 *)&ref_center_y[0];
	iq_param_addr[IQ_REFCENTER_UVTH ] = (UINT32 *)ref_center_uv_th[0];
	iq_param_addr[IQ_REFCENTER_UVWT ] = (UINT32 *)ref_center_uv_wt[0];
	iq_param_addr[IQ_REFCENTER_UV   ] = (UINT32 *)&ref_center_uv[0];

	iq_param_addr[IQ_IFE2FILTER_YTH ] = (UINT32 *)ife2filter_y_th[0];
	iq_param_addr[IQ_IFE2FILTER_YWT ] = (UINT32 *)ife2filter_y_wt[0];
	iq_param_addr[IQ_IFE2FILTER_UVTH] = (UINT32 *)ife2filter_uv_th[0];
	iq_param_addr[IQ_IFE2FILTER_UVWT] = (UINT32 *)ife2filter_uv_wt[0];
	iq_param_addr[IQ_IFE2FILTER     ] = (UINT32 *)&ife2filter[0];

	iq_param_addr[IQ_GRAY           ] = (UINT32 *)&gray_statical[0];

	//***********
	//**  IME  **
	//***********
	iq_param_addr[IQ_CHROMAADAPT        ] = (UINT32 *)&chroma_adapt[0];
	iq_param_addr[IQ_CHROMAADAPT_COLOR  ] = (UINT32 *)&chroma_adapt_color[0];
	iq_param_addr[IQ_CHROMAADAPT_LUM    ] = (UINT32 *)&chroma_adapt_lum[0];

	iq_param_addr[IQ_CHROMASUPP_WEIGHTY ] = (UINT32 *)chroma_supp_w_y[0];
	iq_param_addr[IQ_CHROMASUPP_WEIGHTC ] = (UINT32 *)chroma_supp_w_c[0];
	iq_param_addr[IQ_CHROMASUPP         ] = (UINT32 *)&chroma_supp[0];

	iq_param_addr[IQ_SUPER_RES          ] = (UINT32 *)&super_res;

    iq_param_addr[IQ_SQ_3DNR_LUMA_PARAM     ] = (UINT32 *)&sq_luma_param;
    iq_param_addr[IQ_SQ_3DNR_CHROMA_PARAM   ] = (UINT32 *)&sq_chroma_param;
    iq_param_addr[IQ_SQ_3DNR_CHROMA_Y_PARAM ] = (UINT32 *)&sq_chroma_y_param;

	//***********
	//** Codec **
	//***********
	iq_param_addr[IQ_NUM_3DNR_PARAM ] = (UINT32 *)&num_3DNR_param;
	iq_param_addr[IQ_3DNR_LEVEL     ] = (UINT32 *)&tnr_level[0];
	iq_param_addr[IQ_3DNR_PARAM     ] = (UINT32 *)&tnr_param;

	//***********
	//**  HDR  **
	//***********
	iq_param_addr[IQ_HDR            ] = (UINT32 *)&hdr_sample_evb;

	//***********
	//** Defog **
	//***********
	iq_param_addr[IQ_DEFOG_INIT_PARAM     ] = (UINT32 *)&defog_init_param_sample_evb;
	iq_param_addr[IQ_DEFOG_WET     ] = (UINT32 *)&defog_wet_sample_evb[0];

	iq_set_param_addr(id, mode_idx, iq_param_addr);
}

#endif
