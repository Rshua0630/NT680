#ifndef _IQ_PARAM_TABLE1_OV7251_ID0_EVB_C
#define _IQ_PARAM_TABLE1_OV7251_ID0_EVB_C

#include "iq_param_common_ov7251_id0_evb.h"
#include "iq_param_sqnr1_ov7251_id0_evb.c"

////////////////////////////////
//         Engine Ctrl         /
////////////////////////////////
static ENGINE_CTRL e_ctrl =
{
    {ECS_ON, NON_SUPPORT},
    {VIG_ON, 100},//0~200,default:100
    {GDC_ON, 100},//0~200,default:100(same with OFF)
    {CAC_OFF, NON_SUPPORT},
    {CCM_OFF, NON_SUPPORT},
    {CC3D_OFF, NON_SUPPORT},
    {CCTRL_OFF, 100, 100, 100},//sat:0~200;con:0~200;bri:0~200,fedault:100
    {HSV_OFF, 0},//H:0~360,default:0
    {GAMMA_ON, NON_SUPPORT},
    {Y_CURVE_OFF, NON_SUPPORT},
    {EDGE_ON, 100},//0~200,default:100
    {OUTL_ON, NON_SUPPORT},
    {RANGE_NR_ON, 100},//0~200,default:100
    {GBAL_OFF, NON_SUPPORT},
    {LPF_OFF, NON_SUPPORT},
    {LCA_OFF, NON_SUPPORT},
    {LCA_C_OFF, NON_SUPPORT},
    {LCA_Y_OFF, NON_SUPPORT},
    {SQUARE_ON, NON_SUPPORT},
    {TNR_ON, 5},//0~10,default:5
    {LCE_OFF, NON_SUPPORT},
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
    {64, 64, 64, 64},
    {64, 64, 64, 64},
    {64, 64, 64, 64},
    {64, 64, 64, 64},
    {64, 64, 64, 64},
    {64, 64, 64, 64},
    {64, 64, 64, 64},
    {64, 64, 64, 64},
    {64, 64, 64, 64},
    {64, 64, 64, 64},
    {64, 64, 64, 64},
    {64, 64, 64, 64},
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
    {860, 1247, 2058, 2984, 4095},
    {778, 1128, 1861, 3070, 4095},
    {710, 1036, 1711, 2522, 3905},
    {410, 595, 982, 1424, 2350},
    {410, 595, 982, 1424, 2350},
    {410, 595, 982, 1424, 2350},
    {410, 595, 982, 1424, 2350},
    {410, 595, 982, 1424, 2350},
    {410, 595, 982, 1424, 2350},
};

static UINT32 outl_dark[TOTAL_GAIN_NUM][5] =
{
    {1280, 1868, 3084, 4095, 4095},
    {1184, 1728, 2853, 3942, 4095},
    {1184, 1728, 2853, 3942, 4095},
    {860, 1247, 2058, 2984, 4095},
    {778, 1128, 1861, 3070, 4095},
    {710, 1036, 1711, 2522, 3905},
    {410, 595, 982, 1424, 2350},
    {410, 595, 982, 1424, 2350},
    {410, 595, 982, 1424, 2350},
    {410, 595, 982, 1424, 2350},
    {410, 595, 982, 1424, 2350},
    {410, 595, 982, 1424, 2350},
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
    {0, 1, 2, 2, 3, 4},
    {0, 1, 2, 3, 3, 4},
    {0, 2, 3, 4, 5, 7},
    {0, 4, 6, 8, 9, 13},
    {0, 6, 9, 11, 12, 18},
    {0, 8, 11, 14, 16, 23},
    {0, 9, 13, 16, 18, 26},
    {0, 11, 16, 20, 23, 33},
    {0, 11, 16, 20, 23, 33},
    {0, 11, 16, 20, 23, 33},
    {0, 11, 16, 20, 23, 33},
    {0, 11, 16, 20, 23, 33},
};

static UINT32 range_a_lut[TOTAL_GAIN_NUM][17] =
{
    {0, 1, 2, 3, 3, 4, 4, 5, 5, 6, 6, 6, 7, 7, 7, 8, 9},
    {0, 2, 3, 4, 4, 5, 5, 6, 6, 7, 7, 7, 8, 8, 8, 9, 9},
    {0, 2, 3, 4, 4, 5, 5, 6, 6, 7, 7, 7, 8, 8, 8, 9, 9},
    {0, 3, 4, 5, 6, 7, 7, 8, 9, 9, 10, 10, 11, 11, 11, 12, 12},
    {0, 3, 5, 6, 7, 8, 8, 9, 10, 10, 11, 11, 12, 12, 13, 13, 14},
    {0, 5, 7, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 18, 19, 20, 20},
    {0, 6, 9, 11, 13, 15, 16, 17, 19, 20, 21, 22, 23, 24, 25, 26, 27},
    {0, 8, 11, 14, 16, 18, 20, 22, 23, 25, 26, 27, 29, 30, 31, 32, 33},
    {0, 8, 11, 14, 16, 18, 20, 22, 23, 25, 26, 27, 29, 30, 31, 32, 33},
    {0, 8, 11, 14, 16, 18, 20, 22, 23, 25, 26, 27, 29, 30, 31, 32, 33},
    {0, 8, 11, 14, 16, 18, 20, 22, 23, 25, 26, 27, 29, 30, 31, 32, 33},
    {0, 8, 11, 14, 16, 18, 20, 22, 23, 25, 26, 27, 29, 30, 31, 32, 33},
};

static UINT32 range_b_thr[TOTAL_GAIN_NUM][6] =
{
    {0, 2, 3, 5, 6, 8},
    {0, 3, 4, 6, 7, 9},
    {0, 5, 7, 9, 10, 14},
    {0, 7, 10, 13, 15, 21},
    {0, 11, 16, 19, 22, 32},
    {0, 13, 19, 23, 27, 38},
    {0, 17, 24, 29, 34, 48},
    {0, 17, 24, 30, 35, 49},
    {0, 17, 24, 30, 35, 49},
    {0, 17, 24, 30, 35, 49},
    {0, 17, 24, 30, 35, 49},
    {0, 17, 24, 30, 35, 49},
};

static UINT32 range_b_lut[TOTAL_GAIN_NUM][17] =
{
    {0, 3, 5, 7, 8,  9, 10, 11, 12, 13, 14, 14, 15, 16, 16, 17, 18},
    {0, 4, 6, 8, 9, 10, 11, 12, 13, 14, 15, 15, 16, 17, 17, 18, 19},
    {0, 4, 6, 8, 9, 10, 11, 12, 13, 14, 15, 15, 16, 17, 17, 18, 19},
    {0, 5, 7, 9, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 20, 21, 22},
    {0, 7, 10, 12, 14, 16, 17, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28},
    {0, 7, 10, 13, 15, 16, 18, 20, 21, 22, 24, 25, 26, 27, 28, 29, 30},
    {0, 13, 18, 22, 26, 29, 32, 34, 37, 39, 41, 43, 45, 47, 49, 51, 52},
    {0, 16, 23, 28, 32, 36, 40, 43, 46, 49, 51, 54, 56, 59, 61, 63, 65},
    {0, 16, 23, 28, 32, 36, 40, 43, 46, 49, 51, 54, 56, 59, 61, 63, 65},
    {0, 16, 23, 28, 32, 36, 40, 43, 46, 49, 51, 54, 56, 59, 61, 63, 65},
    {0, 16, 23, 28, 32, 36, 40, 43, 46, 49, 51, 54, 56, 59, 61, 63, 65},
    {0, 16, 23, 28, 32, 36, 40, 43, 46, 49, 51, 54, 56, 59, 61, 63, 65},
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
    {{64, 128, 0}, 0, 0, 0},
    {{64, 128, 0}, 0, 0, 0},
    {{64, 128, 0}, 0, 0, 0},
    {{64, 108, 0}, 0, 0, 0},
    {{56, 108, 0}, 0, 0, 0},
    {{56, 108, 0}, 0, 0, 0},
    {{50, 80, 0}, 0, 0, 0},
    {{50, 80, 0}, 0, 0, 0},
    {{50, 80, 0}, 0, 0, 0},
    {{50, 80, 0}, 0, 0, 0},
    {{50, 80, 0}, 0, 0, 0},
};

//////////////////////
//**  IFE-Vig
//////////////////////
static IQS_VIG_CENTER_RATIO vig_center =
{
    514, 506
};

static IQS_VIG_SET vig =
{
    VIG_TAB_GAIN_2X, {0, 0, 0, 0, 0, 0, 0, 13, 45, 82, 132, 186, 254, 339, 426, 528, 642}
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
    {12, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
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
    {8, (UINT8*)gbal_s_tab, (UINT8*)gbal_d_tab, (UINT32*)gbal_ir_sub, 0},
    {8, (UINT8*)gbal_s_tab, (UINT8*)gbal_d_tab, (UINT32*)gbal_ir_sub, 0},
    {8, (UINT8*)gbal_s_tab, (UINT8*)gbal_d_tab, (UINT32*)gbal_ir_sub, 0},
    {30, (UINT8*)gbal_s_tab, (UINT8*)gbal_d_tab, (UINT32*)gbal_ir_sub, 0},
    {30, (UINT8*)gbal_s_tab, (UINT8*)gbal_d_tab, (UINT32*)gbal_ir_sub, 0},
    {40, (UINT8*)gbal_s_tab, (UINT8*)gbal_d_tab, (UINT32*)gbal_ir_sub, 0},
    {70, (UINT8*)gbal_s_tab, (UINT8*)gbal_d_tab, (UINT32*)gbal_ir_sub, 0},
    {70, (UINT8*)gbal_s_tab, (UINT8*)gbal_d_tab, (UINT32*)gbal_ir_sub, 0},
    {70, (UINT8*)gbal_s_tab, (UINT8*)gbal_d_tab, (UINT32*)gbal_ir_sub, 0},
    {70, (UINT8*)gbal_s_tab, (UINT8*)gbal_d_tab, (UINT32*)gbal_ir_sub, 0},
    {70, (UINT8*)gbal_s_tab, (UINT8*)gbal_d_tab, (UINT32*)gbal_ir_sub, 0},
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
    {63, 0, 63, 0},
    {63, 0, 63, 0},//ISO12800
    {63, 0, 63, 0},
    {63, 0, 63, 0},
    {63, 0, 63, 0},
    {63, 0, 63, 0},
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
    {KER_SEL_A, 8, 8, 12, 12, 16, 16, 3, 3, 3, 3, 3, 3},
    {KER_SEL_A, 8, 8, 12, 12, 16, 16, 3, 3, 3, 3, 3, 3},
    {KER_SEL_A, 8, 8, 12, 12, 16, 16, 3, 3, 3, 3, 3, 3},  //ISO400
    {KER_SEL_A, 8, 8, 12, 12, 16, 16, 3, 3, 3, 3, 3, 3},
    {KER_SEL_A, 8, 8, 12, 12, 16, 16, 3, 3, 3, 3, 3, 3},
    {KER_SEL_A, 8, 8, 12, 12, 16, 16, 3, 3, 3, 3, 3, 3},
    {KER_SEL_A, 8, 8, 12, 12, 16, 16, 3, 3, 3, 3, 3, 3},
    {KER_SEL_A, 8, 8, 12, 12, 16, 16, 3, 3, 3, 3, 3, 3},
    {KER_SEL_A, 8, 8, 12, 12, 16, 16, 3, 3, 3, 3, 3, 3},
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
    {8, 6, 8, 0},
    {8, 6, 8, 0},
    {10, 8, 10, 0},
    {14, 8, 14, 0},
    {14, 8, 14, 0},
    {16, 12, 16, 0},
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
    {15,  15, FALSE, FALSE},
    {15,  15, FALSE, FALSE},
    {15,  15, FALSE, FALSE},
    {15,  15, FALSE, FALSE},
    {15,  15, FALSE, FALSE},
    {15,  15, FALSE, FALSE},
    {15,  15, FALSE, FALSE},
    {15,  15, FALSE, FALSE},
    {15,  15, FALSE, FALSE},
    {15,  15, FALSE, FALSE},
    {15,  15, FALSE, FALSE},
    {15,  15, FALSE, FALSE},
};

//////////////////////
//**  IPE-Edge Map
//////////////////////
static UINT8 ed_tab[TOTAL_GAIN_NUM][16] =
{
    {0, 36, 72, 108, 144, 180, 216, 255, 255, 224, 192, 170, 154, 144, 136, 128},//100
    {0, 36, 72, 108, 144, 180, 216, 255, 255, 224, 192, 170, 154, 144, 136, 128},//200
    {0, 36, 72, 108, 144, 180, 216, 255, 255, 224, 192, 170, 154, 144, 136, 128},//400
    {0, 36, 72, 108, 144, 180, 216, 255, 255, 224, 192, 170, 154, 144, 136, 128},//800
    {0, 36, 72, 108, 144, 180, 216, 255, 255, 224, 192, 170, 154, 144, 136, 128},
    {0, 36, 72, 108, 144, 180, 216, 255, 255, 224, 192, 170, 154, 144, 136, 128},//3200
    {0, 36, 72, 108, 144, 180, 216, 255, 255, 224, 192, 170, 154, 144, 136, 128},
    {0, 36, 72, 108, 144, 180, 216, 255, 255, 224, 192, 170, 154, 144, 136, 128},//12800
    {0, 36, 72, 108, 144, 180, 216, 255, 255, 224, 192, 170, 154, 144, 136, 128},
    {0, 36, 72, 108, 144, 180, 216, 255, 255, 224, 192, 170, 154, 144, 136, 128},
    {0, 36, 72, 108, 144, 180, 216, 255, 255, 224, 192, 170, 154, 144, 136, 128},
    {0, 36, 72, 108, 144, 180, 216, 255, 255, 224, 192, 170, 154, 144, 136, 128},
};

static EdgeMap ed_map[TOTAL_GAIN_NUM] =
{
 //EM_InSel,EthrL, EthrH,EtabL, EtabH, TAB_TblAddr
    {EMI_E57,   0,    80,    1,     2, (UINT32)ed_tab},
    {EMI_E57,   0,   128,    1,     2, (UINT32)ed_tab},
    {EMI_E57,   0,   128,    1,     2, (UINT32)ed_tab},  //ISO400
    {EMI_E57,   0,   128,    1,     2, (UINT32)ed_tab},  //ISO800
    {EMI_E57,   0,   128,    2,     2, (UINT32)ed_tab},  //ISO1600
    {EMI_E57,   0,   128,    2,     2, (UINT32)ed_tab},  //ISO3200
    {EMI_E57 ,  0,   128,    3,     2, (UINT32)ed_tab},  //ISO6400
    {EMI_E57,   0,   128,    3,     2, (UINT32)ed_tab},  //ISO12800
    {EMI_E57,   0,   128,    3,     2, (UINT32)ed_tab},
    {EMI_E57,   0,   128,    3,     2, (UINT32)ed_tab},
    {EMI_E57,   0,   128,    3,     2, (UINT32)ed_tab},
    {EMI_E57,   0,   128,    3,     2, (UINT32)ed_tab},
};

static UINT8 es_tab[TOTAL_GAIN_NUM][16] =
{
    {32, 42, 53, 64, 64, 64, 64, 64, 64, 64, 64, 52, 44, 38, 32, 32},
    {32, 42, 53, 64, 64, 64, 64, 64, 64, 64, 64, 52, 44, 38, 32, 32},
    {32, 42, 53, 64, 64, 64, 64, 64, 64, 64, 64, 52, 44, 38, 32, 32},
    {32, 42, 53, 64, 64, 64, 64, 64, 64, 64, 64, 52, 44, 38, 32, 32},//ISO800
    {32, 42, 53, 64, 64, 64, 64, 64, 64, 64, 64, 52, 44, 38, 32, 32},//ISO1600
    {32, 42, 53, 64, 64, 64, 64, 64, 64, 64, 64, 52, 44, 38, 32, 32},//ISO3200
    {32, 42, 53, 64, 64, 64, 64, 64, 64, 64, 64, 52, 44, 38, 32, 32},//ISO6400
    {32, 42, 53, 64, 64, 64, 64, 64, 64, 64, 64, 52, 44, 38, 32, 32},//ISO12800
    {32, 42, 53, 64, 64, 64, 64, 64, 64, 64, 64, 52, 44, 38, 32, 32},//
    {32, 42, 53, 64, 64, 64, 64, 64, 64, 64, 64, 52, 44, 38, 32, 32},//
    {32, 42, 53, 64, 64, 64, 64, 64, 64, 64, 64, 52, 44, 38, 32, 32},//
    {32, 42, 53, 64, 64, 64, 64, 64, 64, 64, 64, 52, 44, 38, 32, 32},//
};

static EdgeMapS es_map[TOTAL_GAIN_NUM] =
{
 //EthrL,EthrH, EtabL, EtabH, TAB_TblAddr
	{0,   512,     0,     5, (UINT32)es_tab},
	{0,   512,     0,     5, (UINT32)es_tab},
	{0,   512,     0,     5, (UINT32)es_tab},
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
    {8, 11, 14},
    {15, 22, 29},
    {15, 22, 29},//{6, 9, 12},//{22, 33, 44},
    {15, 22, 29},//{6, 9, 12},//{22, 33, 44},//12800
    {15, 22, 29},//
    {15, 22, 29},//
    {15, 22, 29},//
    {15, 22, 29},//
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
    {7, 10, 13},
    {10, 15, 18},
    {13, 19, 26},//{7, 10, 13},//{13, 19, 26},
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
    {8, 11, 14, 17, 20},
    {15, 22, 29, 36, 43},
    {15, 22, 29, 36, 43},
    {15, 22, 29, 36, 43},
    {15, 22, 29, 36, 43},
    {15, 22, 29, 36, 43},
    {15, 22, 29, 36, 43},
    {15, 22, 29, 36, 43},
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
    {10, 14, 18, 21, 25},
    {10, 14, 18, 21, 25},
    {10, 14, 18, 21, 25},
    {10, 14, 18, 21, 25},
    {10, 14, 18, 21, 25},
    {10, 14, 18, 21, 25},
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
    {1, {FLTR_SIZE_9x9, {18, 18}, EKNL_SIZE_3x3, {(UINT32)ife2filter_y_th, (UINT32)ife2filter_y_wt},  {(UINT32)ife2filter_uv_th, (UINT32)ife2filter_uv_wt}, {(UINT32)ife2filter_uv_th, (UINT32)ife2filter_uv_wt}}},
    {1, {FLTR_SIZE_9x9, {18, 18}, EKNL_SIZE_3x3, {(UINT32)ife2filter_y_th, (UINT32)ife2filter_y_wt},  {(UINT32)ife2filter_uv_th, (UINT32)ife2filter_uv_wt}, {(UINT32)ife2filter_uv_th, (UINT32)ife2filter_uv_wt}}},
    {1, {FLTR_SIZE_9x9, {14, 14}, EKNL_SIZE_3x3, {(UINT32)ife2filter_y_th, (UINT32)ife2filter_y_wt},  {(UINT32)ife2filter_uv_th, (UINT32)ife2filter_uv_wt}, {(UINT32)ife2filter_uv_th, (UINT32)ife2filter_uv_wt}}},
    {1, {FLTR_SIZE_9x9, {14, 14}, EKNL_SIZE_3x3, {(UINT32)ife2filter_y_th, (UINT32)ife2filter_y_wt},  {(UINT32)ife2filter_uv_th, (UINT32)ife2filter_uv_wt}, {(UINT32)ife2filter_uv_th, (UINT32)ife2filter_uv_wt}}},
    {1, {FLTR_SIZE_9x9, {18, 18}, EKNL_SIZE_3x3, {(UINT32)ife2filter_y_th, (UINT32)ife2filter_y_wt},  {(UINT32)ife2filter_uv_th, (UINT32)ife2filter_uv_wt}, {(UINT32)ife2filter_uv_th, (UINT32)ife2filter_uv_wt}}},
    {1, {FLTR_SIZE_9x9, {18, 18}, EKNL_SIZE_3x3, {(UINT32)ife2filter_y_th, (UINT32)ife2filter_y_wt},  {(UINT32)ife2filter_uv_th, (UINT32)ife2filter_uv_wt}, {(UINT32)ife2filter_uv_th, (UINT32)ife2filter_uv_wt}}},
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
        {{14, 14,  8},        {RANGE_8,   0, RANGE_8,  8,       0},            {RANGE_8, 0, RANGE_8, 8, 0}, 25},
        {{14, 14,  8},        {RANGE_8,   0, RANGE_8,  8,       0},            {RANGE_8, 0, RANGE_8, 8, 0}, 25},
        {{14, 14,  8},        {RANGE_8,   0, RANGE_8,  8,       0},            {RANGE_8, 0, RANGE_8, 8, 0}, 25},
        {{14, 14, 12},        {RANGE_8,   0, RANGE_8,  8,       0},            {RANGE_8, 0, RANGE_8, 8, 0}, 25},
        {{18, 18, 10},        {RANGE_8,   0, RANGE_8,  8,       2},            {RANGE_8, 0, RANGE_8, 8, 0}, 25},
        {{18, 18, 10},        {RANGE_8,   0, RANGE_8,    8,       2},            {RANGE_8, 0, RANGE_8, 8, 0}, 25},
		{{20, 20,  8},        {RANGE_8,   0, RANGE_8,    8,       5},            {RANGE_8, 0, RANGE_8, 8, 0}, 25},
        {{20, 20, 12},        {RANGE_8,   0, RANGE_8,    8,       5},            {RANGE_8, 0, RANGE_8, 8, 0}, 25},
        {{20, 20, 12},        {RANGE_8,   0, RANGE_8,    8,       5},            {RANGE_8, 0, RANGE_8, 8, 0}, 25},
        {{20, 20, 12},        {RANGE_8,   0, RANGE_8,    8,       5},            {RANGE_8, 0, RANGE_8, 8, 0}, 25},
        {{20, 20, 12},        {RANGE_8,   0, RANGE_8,    8,       5},            {RANGE_8, 0, RANGE_8, 8, 0}, 25},
        {{20, 20, 12},        {RANGE_8,   0, RANGE_8,    8,       5},            {RANGE_8, 0, RANGE_8, 8, 0}, 25},
};

static IME_CHRA_CADAP chroma_adapt_color[TOTAL_GAIN_NUM] =
{
 //LcaCLREn, LcaRefWt{CentU, CentV}, LcaCLRinfo{LumRng,LumTh,LumWtPrc,LumWtS,LumWtE}
    {DISABLE,         {128,     128},          {RANGE_8,   0,   RANGE_8,   8,     0}},
    {DISABLE,         {128,     128},          {RANGE_8,   0,   RANGE_8,   8,     0}},
    {DISABLE,         {128,     128},          {RANGE_8,   0,   RANGE_8,   8,     0}},
    {DISABLE,         {128,     128},          {RANGE_8,   0,   RANGE_8,   8,     0}},
    {DISABLE,         {128,     128},          {RANGE_8,   0,   RANGE_8,   8,     0}},
    {DISABLE,         {128,     128},          {RANGE_8,   0,   RANGE_8,   8,     0}},
    {DISABLE,         {128,     128},          {RANGE_8,   0,   RANGE_8,   8,     0}},
    {DISABLE,         {128,     128},          {RANGE_8,   0,   RANGE_8,   8,     0}},
    {DISABLE,         {128,     128},          {RANGE_8,   0,   RANGE_8,   8,     0}},
    {DISABLE,         {128,     128},          {RANGE_8,   0,   RANGE_8,   8,     0}},
    {DISABLE,         {128,     128},          {RANGE_8,   0,   RANGE_8,   8,     0}},
    {DISABLE,         {128,     128},          {RANGE_8,   0,   RANGE_8,   8,     0}},
};

static IME_CHRA_LADAP chroma_adapt_lum[TOTAL_GAIN_NUM] =
{
  //LcaLumEn, LcaRefWt{RefWt,OutWt}, LcaLuminfo{LumRng, LumTh,LumWtPrc,LumWtS,LumWtE}
    {DISABLE,         {0,        0},           {RANGE_8,    0, RANGE_8,     0,     0}},
    {DISABLE,         {0,        0},           {RANGE_8,    0, RANGE_8,     0,     0}},
    {DISABLE,         {0,        0},           {RANGE_8,    0, RANGE_8,     0,     0}},
    {DISABLE,         {0,        0},           {RANGE_8,    0, RANGE_8,     0,     0}},
    {DISABLE,         {16,      24},           {RANGE_8,    0, RANGE_8,     0,     6}},
    {DISABLE,          {14,      24},           {RANGE_8,    0, RANGE_8,     0,     6}},
    {DISABLE,          {24,      24},           {RANGE_8,    0, RANGE_8,     0,     6}},
    {DISABLE,          {24,      24},           {RANGE_8,    0, RANGE_8,     0,     6}},
    {DISABLE,          {24,      24},           {RANGE_8,    0, RANGE_8,     0,     6}},
    {DISABLE,          {24,      24},           {RANGE_8,    0, RANGE_8,     0,     6}},
    {DISABLE,          {24,      24},           {RANGE_8,    0, RANGE_8,     0,     6}},
    {DISABLE,          {24,      24},           {RANGE_8,    0, RANGE_8,     0,     6}},
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
	0x2b6aaebe, 0x2a3a6a9c, 0x285a2e86, 0x2759da76,
	0x2649b262, 0x25598e55, 0x25296655, 0x24a90e3c,
	0x23a8ce3a, 0x22688a29, 0x21883e15, 0x20682608,
	0x1f57d1f3, 0x1ee7a9e9, 0x1dd761da, 0x1d572dd3,
	0x1c6705c8, 0x1bd6f5b7, 0x1b96cdaf, 0x1aa6a5a5,
	0x19c6819c, 0x19b6619a, 0x19665593, 0x1916418b,
	0x1896318a, 0x18360d86, 0x17f60583, 0x18260180,
	0x17e5f979, 0x1775e57a, 0x17c5e978, 0x17b5e174,
	0x17b5e57a, 0x1795d979, 0x1785ed78, 0x17c5ed7d,
	0x17e5f182, 0x17f5fd83, 0x18260986, 0x1866218d,
	0x18b6298c, 0x18d64992, 0x1956519c, 0x19f671a1,
	0x1a16a9a8, 0x1ac6bdab, 0x1b96d1bb, 0x1cb721c2,
	0x1ca74dce, 0x1d876ddb, 0x1de79deb, 0x1ec7e1fa,
	0x2098120a, 0x2138661d, 0x2298ba30, 0x2398f23b,
	0x24893e50, 0x26f98e6a, 0x2759ee89, 0x279a228c,
	0x298a5a92, 0x2a3a8e9f, 0x2a7a92af, 0x2bcae6bd,
	0x2ceb06d7, 0x296a72a9, 0x293a4297, 0x2809da83,
	0x2689b670, 0x2619965e, 0x25194650, 0x24b9124a,
	0x23c8e637, 0x2258aa2c, 0x21886e15, 0x20581a00,
	0x1f77ddf6, 0x1e77a9e7, 0x1d776dd9, 0x1d2741ce,
	0x1c970dc5, 0x1bd6ddb9, 0x1b26bdb0, 0x1aa695a7,
	0x1a0675a0, 0x19666198, 0x1906398c, 0x18a6298b,
	0x18961989, 0x1825f982, 0x17c5fd7f, 0x17b5e579,
	0x17a5d978, 0x1745d174, 0x1745c972, 0x16d5c570,
	0x1705b971, 0x16d5c970, 0x16e5c970, 0x1725c573,
	0x1725c974, 0x1745d175, 0x1795e57b, 0x1775e97b,
	0x17c5fd81, 0x18361584, 0x18862188, 0x18f6358f,
	0x19165995, 0x19e6819c, 0x1a468da4, 0x1aa6c1b7,
	0x1b66d9b9, 0x1c0711c8, 0x1cd745cd, 0x1da771e0,
	0x1e97b5ec, 0x1fa7e1fc, 0x20582a14, 0x2198721e,
	0x22c8c23c, 0x23e8ee43, 0x25b95a59, 0x26499664,
	0x2789d676, 0x280a2287, 0x280a5698, 0x29ba96b2,
	0x2a7ab6ab, 0x2a5b06c7, 0x291a6286, 0x2889f686,
	0x2709be68, 0x26195651, 0x24d94a4c, 0x23f91646,
	0x23a8da32, 0x22489a30, 0x21e85217, 0x20581a06,
	0x1f97c5f8, 0x1e5799e9, 0x1d8765d8, 0x1cf729cd,
	0x1c46f9c0, 0x1b96d9ba, 0x1a86adaf, 0x19f681a2,
	0x19965d94, 0x18f64194, 0x18c6258f, 0x18461584,
	0x17d5f980, 0x1765e17d, 0x1775d973, 0x1735c975,
	0x16f5b970, 0x16b5ad6e, 0x16a5a167, 0x16659d68,
	0x16259968, 0x16459168, 0x16559167, 0x16559968,
	0x16859967, 0x16859969, 0x1695a96b, 0x1695b56c,
	0x16e5bd71, 0x16f5c570, 0x1755d979, 0x17c5f17b,
	0x18060983, 0x18a6198b, 0x18f63d90, 0x19465595,
	0x19e6759f, 0x1a36a5a4, 0x1ae6d1b4, 0x1c36f9c0,
	0x1cb715ce, 0x1d275dd5, 0x1e0795ea, 0x1f57cdf3,
	0x2028060a, 0x2138661f, 0x22289e2d, 0x2358e24c,
	0x24b94256, 0x25998a69, 0x26c9b66d, 0x2859fa7f,
	0x28aa2a8a, 0x292a6aa0, 0x29fa92b3, 0x286a1287,
	0x2749da73, 0x25e98a62, 0x2569264a, 0x24091e3f,
	0x2348d234, 0x22689e26, 0x22685a16, 0x21082211,
	0x1fd7f9f6, 0x1e97a5e6, 0x1dd761d2, 0x1ce739cb,
	0x1c06f9c0, 0x1b66ddb5, 0x1aa6a5aa, 0x19f6819d,
	0x19366597, 0x18d63d8e, 0x18962187, 0x18261182,
	0x17b5f17a, 0x1765d575, 0x1725c56f, 0x1715b56d,
	0x16a5a56b, 0x16559967, 0x16359166, 0x16258563,
	0x16057d5f, 0x15e5755e, 0x15e5755e, 0x15e5755e,
	0x15d5795d, 0x16157d61, 0x15f57d63, 0x16258d61,
	0x16559166, 0x16759d6a, 0x1695b16b, 0x16a5bd6f,
	0x1745cd72, 0x1765e179, 0x17c5f180, 0x18360d8a,
	0x1886398e, 0x19364d91, 0x19b679a2, 0x1a3699a9,
	0x1b06c1b2, 0x1bd6f5c5, 0x1c6725ca, 0x1d375dda,
	0x1e17a9e7, 0x1f67c9f9, 0x2077fa0c, 0x21885613,
	0x2258aa33, 0x2378f642, 0x2549424f, 0x25d99e61,
	0x26c9da6d, 0x27b9fe7a, 0x284a328f, 0x28ca629b,
	0x2849e677, 0x26f9be6f, 0x24f94a59, 0x23f91641,
	0x2278d232, 0x22589a27, 0x21786e16, 0x20e83208,
	0x1fe7ddf6, 0x1ea7a5e9, 0x1df761d7, 0x1cc725ca,
	0x1c16e9c0, 0x1b16cdb3, 0x1ac69da8, 0x19b6759c,
	0x19264d92, 0x18c63190, 0x18861189, 0x18260181,
	0x1795dd79, 0x1725cd71, 0x16b5a96d, 0x16b5a166,
	0x16358962, 0x1665855f, 0x1605755a, 0x15956156,
	0x15755957, 0x15755955, 0x15455956, 0x15355552,
	0x15655155, 0x15355153, 0x15655558, 0x15755556,
	0x1575615a, 0x1595695c, 0x15956d61, 0x16057d63,
	0x16459562, 0x16b5a166, 0x16d5b96e, 0x1735c973,
	0x1765e97b, 0x17e60183, 0x18762186, 0x18c64591,
	0x19a6559a, 0x19f681a3, 0x1a96bdb3, 0x1b26d9b7,
	0x1c471dc9, 0x1d274dd8, 0x1e4789e4, 0x1f77c9f3,
	0x20781609, 0x2198761a, 0x22789a31, 0x2358ea3f,
	0x24a92e52, 0x2599765a, 0x2689ba71, 0x2799ea84,
	0x288a1e8c, 0x2689a672, 0x2589865d, 0x2479124c,
	0x23c8de38, 0x22a89a2c, 0x2188661e, 0x20f83e11,
	0x1fa7e9fb, 0x1f07c1f0, 0x1db771d3, 0x1d3735cd,
	0x1bd701be, 0x1b26c9b0, 0x1a6691a6, 0x19e67d9e,
	0x19763d93, 0x18b6358a, 0x18560983, 0x1795f179,
	0x1745d174, 0x16d5c16c, 0x16a5a167, 0x16258d63,
	0x15b57563, 0x15c56d5c, 0x15955d58, 0x15255553,
	0x15054150, 0x15153d52, 0x14e5354f, 0x14b5354d,
	0x14c52d4d, 0x14b5294f, 0x14a52d4a, 0x14c53150,
	0x14f53d4e, 0x15154151, 0x15154151, 0x15354d54,
	0x15655d54, 0x1595655c, 0x15f57d61, 0x16458964,
	0x16659d6a, 0x1705b96f, 0x1755d577, 0x17c5f17e,
	0x18361184, 0x18c6358b, 0x19465d97, 0x19f671a1,
	0x1a86adae, 0x1b86e1b9, 0x1c8711c3, 0x1d1749d6,
	0x1e0789e4, 0x1f57bdf0, 0x1fa81206, 0x2108461a,
	0x2298a629, 0x2388fa37, 0x24c92654, 0x2589625e,
	0x26b9ce68, 0x274a027d, 0x2589a663, 0x2529524d,
	0x2468ee46, 0x22d8aa2b, 0x21c87a1e, 0x21283e12,
	0x20180207, 0x1ec7bdeb, 0x1df775df, 0x1d0731cf,
	0x1c1709bd, 0x1b26d1b2, 0x1a9699a6, 0x19b6859e,
	0x19064592, 0x18863589, 0x18260181, 0x1795e978,
	0x1725c972, 0x16b5ad6d, 0x16959165, 0x15f58161,
	0x15e56d5b, 0x15655d57, 0x15154950, 0x14f54150,
	0x14e52d4c, 0x14a5294a, 0x14851d46, 0x14851547,
	0x14651546, 0x14351145, 0x14251544, 0x14251148,
	0x14551146, 0x14652148, 0x14751d49, 0x14752d4b,
	0x14b5294e, 0x14e5314f, 0x15254555, 0x15555556,
	0x15a56d5a, 0x15f57562, 0x16359566, 0x16c5b56c,
	0x1755c176, 0x17b5e579, 0x18160186, 0x18862d90,
	0x19264997, 0x19a679a2, 0x1a96b1ad, 0x1b36d9bb,
	0x1c871dc6, 0x1d0751d4, 0x1e0795eb, 0x1f37c5f5,
	0x1fe82204, 0x21986e19, 0x2228b22f, 0x23490240,
	0x24894652, 0x2589725b, 0x2769b67d, 0x26394a61,
	0x24a93a4e, 0x2378ce38, 0x22888a25, 0x21685211,
	0x20882206, 0x1f97cdf6, 0x1e8795e4, 0x1d474dd9,
	0x1c371dc9, 0x1bc6ddb8, 0x1a86a5a7, 0x1a267d9f,
	0x19664593, 0x18b63588, 0x18460581, 0x1795e17d,
	0x1735c974, 0x16b5b16f, 0x16459566, 0x15f5795d,
	0x15956158, 0x15555153, 0x15254952, 0x14d5314d,
	0x14a5294b, 0x14651947, 0x14551145, 0x14450d43,
	0x14150541, 0x13e5013f, 0x13e4f940, 0x13e5013f,
	0x13c4f93f, 0x13f4f93e, 0x13f5053f, 0x14150543,
	0x14150d42, 0x14551546, 0x14a5214d, 0x14c5354d,
	0x15154150, 0x15355155, 0x15b5655b, 0x15d57961,
	0x16359566, 0x16a5a96c, 0x1735c975, 0x17c5e97f,
	0x18260984, 0x1906298d, 0x1916599a, 0x1a2689a1,
	0x1ae6b9af, 0x1bc6e9bc, 0x1cc725ca, 0x1d475de0,
	0x1e5799e6, 0x1f77de00, 0x2098320d, 0x2198761f,
	0x22f8c636, 0x23c90a43, 0x24f93e58, 0x26398672,
	0x25794653, 0x24490643, 0x22f8ae2c, 0x21b87219,
	0x20d81a0d, 0x1fc7e604, 0x1ec79dea, 0x1da771d7,
	0x1c8721ce, 0x1ba6e9bc, 0x1b26b1b0, 0x19d685a4,
	0x19865991, 0x18c6298f, 0x18360d81, 0x1785e97c,
	0x1725cd73, 0x16b5a96c, 0x16259563, 0x15e5795b,
	0x15856157, 0x15454d51, 0x14d5354d, 0x14c5254a,
	0x14551d46, 0x14350d44, 0x13f50140, 0x13e4f53d,
	0x13a4e93c, 0x13b4ed3a, 0x1374e13a, 0x1394e539,
	0x1394dd3a, 0x1394e138, 0x1374e13a, 0x13b4e93b,
	0x13a4f53b, 0x13e4f13d, 0x13e50140, 0x14050943,
	0x14651546, 0x1495254a, 0x14c5354e, 0x15154d53,
	0x15655559, 0x15a56d5c, 0x16258d63, 0x16b5ad6d,
	0x16f5c971, 0x17a5e97c, 0x18060d86, 0x18a62d8d,
	0x19465198, 0x19e679a6, 0x1ae6b1af, 0x1b86e9bb,
	0x1c672dcb, 0x1d4769de, 0x1ec7b5f6, 0x1fc7fa00,
	0x20b85214, 0x21d8921e, 0x2318ca3c, 0x2429124d,
	0x25096661, 0x24a93241, 0x23e8ca36, 0x22988e20,
	0x2118320f, 0x2027edfa, 0x1f17b1f3, 0x1e376de2,
	0x1d2741ca, 0x1bf6f9c3, 0x1ae6bdaf, 0x1a8695a4,
	0x19865997, 0x18e6358f, 0x18160983, 0x1795e979,
	0x1735c173, 0x16c5b16c, 0x16359165, 0x15e5755c,
	0x15755555, 0x15154551, 0x14d5314c, 0x14751948,
	0x14350943, 0x13f4f940, 0x13c4ed3e, 0x1394e539,
	0x1374d937, 0x1354d536, 0x1354cd33, 0x1334d133,
	0x1324c932, 0x1324c532, 0x1324c932, 0x1314c932,
	0x1344d134, 0x1334d535, 0x1374d936, 0x1384dd3b,
	0x13a4ed3c, 0x13e4f540, 0x14150942, 0x14651148,
	0x1495254a, 0x14e53d51, 0x15554d56, 0x15a5695b,
	0x15e58163, 0x1695a56c, 0x1725bd71, 0x17b5e97a,
	0x18260d83, 0x18c62d8e, 0x19365997, 0x19c68da9,
	0x1af6c5b3, 0x1b96fdc2, 0x1cd72dd0, 0x1e0775e3,
	0x1e87c1f1, 0x20081606, 0x20d8521b, 0x2288a628,
	0x2348fa43, 0x24a93256, 0x2418ee37, 0x2318be2a,
	0x2168621f, 0x2018120c, 0x1f87e5f7, 0x1ea795e0,
	0x1d5741d3, 0x1c6721cb, 0x1b56d9b7, 0x1a6699a8,
	0x19a6799a, 0x18f65192, 0x18460985, 0x17c5ed7d,
	0x1775d572, 0x16d5a96b, 0x16658d64, 0x15d5755b,
	0x15555d57, 0x15053d50, 0x14d5254c, 0x14451547,
	0x14150543, 0x13d4f93e, 0x13b4e139, 0x1354d537,
	0x1354d532, 0x1324c933, 0x1304bd2f, 0x12f4b52e,
	0x12d4b52c, 0x12d4b92e, 0x12b4a92d, 0x12b4ad2d,
	0x12d4b52c, 0x12e4b92d, 0x12e4b52f, 0x1304c532,
	0x1334c933, 0x1354d936, 0x1384dd37, 0x13b4f53c,
	0x13f5013f, 0x14550d45, 0x14a52147, 0x14d53d52,
	0x15555155, 0x15956d5a, 0x15e5855f, 0x1695a56b,
	0x16e5c172, 0x1795e97d, 0x18460983, 0x18c63d8f,
	0x1956659b, 0x1ab6a1ac, 0x1b06d1b3, 0x1c4705c7,
	0x1d0741d4, 0x1e6785e7, 0x1fc7d9fb, 0x20781e04,
	0x21886e21, 0x2328d635, 0x23f8f64b, 0x23d8e237,
	0x23089623, 0x21384e13, 0x201801fa, 0x1f87c1ea,
	0x1dd775da, 0x1ce739c9, 0x1c06fdba, 0x1ac6bdae,
	0x1a468da4, 0x19865193, 0x18b63589, 0x17f6057f,
	0x1795d977, 0x16d5bd6e, 0x16359967, 0x16057560,
	0x15b55955, 0x15154d53, 0x14c5314d, 0x14651146,
	0x1434fd40, 0x13c4f53b, 0x13a4e538, 0x1344d536,
	0x12f4cd32, 0x1324c12f, 0x12e4b52c, 0x12c4b12c,
	0x1294a92a, 0x12a4a92a, 0x12b4a128, 0x1284a129,
	0x1274a129, 0x1284a128, 0x1294a92b, 0x12b4a92d,
	0x12c4b92c, 0x1304bd2f, 0x12e4c132, 0x1334cd34,
	0x1394dd39, 0x13c4e93d, 0x1404fd40, 0x14351144,
	0x1485254b, 0x14e5394f, 0x15755556, 0x1595695c,
	0x16259166, 0x16b5b16d, 0x1715d572, 0x17c5f57f,
	0x1856218b, 0x19365198, 0x19a675a2, 0x1ac6a9b3,
	0x1b96e9be, 0x1c7729c7, 0x1da771e1, 0x1e87a9f0,
	0x1f87f206, 0x2108521e, 0x2278a62d, 0x2378d247,
	0x22e8b22d, 0x22287e1e, 0x20c82e08, 0x1f67d5f9,
	0x1e078de4, 0x1d0751cf, 0x1c3711c1, 0x1b66d5b7,
	0x1a36a1a9, 0x19766999, 0x18e63990, 0x18360582,
	0x1795dd78, 0x16f5b56e, 0x1685a568, 0x1615815d,
	0x15956959, 0x15154953, 0x14b52d4c, 0x14651547,
	0x13f50941, 0x13b4f13b, 0x1384dd38, 0x1344c933,
	0x1304c130, 0x12e4b52e, 0x12a4ad2c, 0x1294a128,
	0x1264a128, 0x12549126, 0x12549524, 0x12548d25,
	0x12248d23, 0x12248d24, 0x12349124, 0x12549125,
	0x12549927, 0x12749d29, 0x12a4a92b, 0x12c4b12d,
	0x1304bd30, 0x1324c934, 0x1354dd37, 0x13a4e53a,
	0x13e4f941, 0x14150544, 0x1475254b, 0x15053d50,
	0x15455957, 0x15b5795c, 0x16359966, 0x1675b170,
	0x1765d97a, 0x17c60181, 0x18562d8e, 0x19565997,
	0x1a2695a7, 0x1af6d1b4, 0x1c1701c9, 0x1d3745d2,
	0x1dc795e9, 0x1ec7d1fb, 0x20381208, 0x21b86e21,
	0x2378be35, 0x2328ba29, 0x21a86616, 0x1fe7fe03,
	0x1ec7a1f0, 0x1dd76dde, 0x1ce72dc8, 0x1bb6e9ba,
	0x1af6adae, 0x1a06799f, 0x19564592, 0x18c61986,
	0x17e5ed7b, 0x1735c574, 0x16b5a168, 0x16358962,
	0x15956d59, 0x15354d52, 0x14c52d4f, 0x14751d49,
	0x1415013f, 0x13c4f53a, 0x1364dd37, 0x1334cd34,
	0x1314bd2f, 0x12c4ad2b, 0x1294a529, 0x12749925,
	0x12549526, 0x12548922, 0x12147d21, 0x11f48120,
	0x12047d1f, 0x11f48120, 0x11e47921, 0x11f4811f,
	0x12148121, 0x12348521, 0x12348d24, 0x12549925,
	0x1274a128, 0x12a4a92c, 0x12c4b92e, 0x1314c531,
	0x1354d536, 0x1394e139, 0x13d4f540, 0x14350d45,
	0x14a5214d, 0x14f54552, 0x15555959, 0x15e57563,
	0x16359967, 0x16d5c973, 0x17a5dd7b, 0x18460984,
	0x18963191, 0x19a675a0, 0x1a36a5ab, 0x1b66ddb9,
	0x1c1725cd, 0x1d6769d6, 0x1ec7aded, 0x1fb81204,
	0x2148661d, 0x22089228, 0x21a8821d, 0x20e83611,
	0x1fc7d1fc, 0x1e6795ec, 0x1d5761cf, 0x1c371dc5,
	0x1b36d9b6, 0x1ac6a1a7, 0x19c65d97, 0x1926358b,
	0x18260182, 0x1765d974, 0x16d5b16d, 0x16559564,
	0x15a5715e, 0x15555154, 0x14e5354c, 0x14752149,
	0x14350942, 0x13b4f13c, 0x1364e138, 0x1334cd33,
	0x12e4bd2e, 0x12c4ad2c, 0x1294a129, 0x12449525,
	0x12148d21, 0x12148120, 0x12047d1f, 0x11c4751e,
	0x11c4711d, 0x11c46d1a, 0x11b4711b, 0x11b46d1c,
	0x11d46d1c, 0x11d4751d, 0x11c4751d, 0x11e47d1f,
	0x12048521, 0x12449123, 0x12649526, 0x1294a52b,
	0x12b4b52b, 0x12f4bd32, 0x1334cd34, 0x1374e93b,
	0x13e4f940, 0x14051144, 0x14a5254a, 0x15054153,
	0x15555d59, 0x16057d61, 0x1655a56a, 0x1725cd76,
	0x17a5ed7d, 0x1836198a, 0x19264d94, 0x1a067da7,
	0x1ab6b5b1, 0x1bd6f9c6, 0x1d073dd8, 0x1da789ed,
	0x1f47d1f7, 0x20c83215, 0x21a88e1f, 0x21686617,
	0x20e8220c, 0x1f77cdf1, 0x1df779dc, 0x1cd731d1,
	0x1c26e9be, 0x1b06c1b0, 0x1a2689a3, 0x19863d92,
	0x18862585, 0x17c5ed7e, 0x1715c973, 0x16a5a16b,
	0x16258562, 0x15755959, 0x15153d53, 0x14b52d4a,
	0x14151542, 0x13d4fd3d, 0x1394e93a, 0x1324d534,
	0x1314c130, 0x12c4ad2c, 0x1294a129, 0x12549124,
	0x12248523, 0x11f4751f, 0x11d4751e, 0x11a4691b,
	0x11a4691a, 0x11946119, 0x11946519, 0x11945d19,
	0x1184611a, 0x11846518, 0x11a4651b, 0x11946d1b,
	0x11d4711e, 0x11b47d1d, 0x12048120, 0x12448921,
	0x12649528, 0x1274a52a, 0x12c4b52c, 0x12f4c532,
	0x1344d536, 0x1394e93b, 0x13f50141, 0x14452148,
	0x14c5394d, 0x15355157, 0x15c5755e, 0x16158d67,
	0x16c5c171, 0x1755d579, 0x17f60d87, 0x18963992,
	0x19a6619c, 0x1a36a9ad, 0x1b26e9be, 0x1c872dcb,
	0x1d676de2, 0x1ec7b9f4, 0x20581209, 0x20c8721e,
	0x21485612, 0x2088220a, 0x1ec7b1ea, 0x1d5761d7,
	0x1ce715cd, 0x1b76d9ba, 0x1ac6a1a8, 0x19a66d9c,
	0x18f6418e, 0x18560580, 0x1775d976, 0x16f5b16d,
	0x16358d64, 0x1595695d, 0x15454d56, 0x14c5314d,
	0x14351547, 0x1404fd40, 0x1394e93b, 0x1354d535,
	0x1314c12f, 0x12b4b52b, 0x1284a129, 0x12549124,
	0x12248922, 0x11e4791e, 0x11c46d1d, 0x11a46519,
	0x11746118, 0x11745916, 0x11645517, 0x11545516,
	0x11445115, 0x11445915, 0x11545917, 0x11645516,
	0x11845d18, 0x11946119, 0x11a4691a, 0x11d4751d,
	0x11e47d1f, 0x12048924, 0x12549525, 0x1284a528,
	0x12c4b52d, 0x1324c530, 0x1354d537, 0x13e4ed3d,
	0x14050142, 0x14852148, 0x14e53950, 0x15755d59,
	0x15f58160, 0x1675a969, 0x1735cd72, 0x17a5f17f,
	0x18362989, 0x19465995, 0x19c68da5, 0x1b36c9b6,
	0x1be70dc7, 0x1cf749d7, 0x1e67a5f4, 0x1fe7f9fe,
	0x2078560c, 0x2128320e, 0x2017f1fd, 0x1ed79de3,
	0x1d1749d0, 0x1c0701c2, 0x1b36cdb3, 0x1aa67da0,
	0x19765d99, 0x18d6218b, 0x17d5f180, 0x1735c974,
	0x1675a16a, 0x16057d5e, 0x15855d57, 0x14f54150,
	0x1495214c, 0x14150543, 0x13b4ed3a, 0x1354dd35,
	0x1304c931, 0x12f4b52c, 0x1274a129, 0x12549126,
	0x12147d20, 0x11c4791e, 0x11d46d1b, 0x11946519,
	0x11445917, 0x11445514, 0x11445114, 0x11344913,
	0x11244d14, 0x11244512, 0x11144513, 0x11344911,
	0x11245114, 0x11445114, 0x11545915, 0x11645d17,
	0x1174651a, 0x11b46d1d, 0x11e4791f, 0x11f48521,
	0x12649526, 0x1284a52b, 0x12c4b12f, 0x1314c933,
	0x13a4e138, 0x13d4f53f, 0x14351545, 0x14a52d4b,
	0x15154951, 0x1595655c, 0x16358d64, 0x16c5a96f,
	0x1755dd7a, 0x17e61985, 0x19163d90, 0x1996819e,
	0x1a56a5aa, 0x1ba6fdc4, 0x1c8721cd, 0x1df779e5,
	0x1f17edf8, 0x20182a17, 0x20181204, 0x1f47d9f4,
	0x1e077ddd, 0x1cc73dca, 0x1bc6e1be, 0x1ad6b5ad,
	0x19e66d9e, 0x19063d91, 0x18560d84, 0x1795dd78,
	0x16e5b96f, 0x16458d63, 0x15b5695a, 0x15154953,
	0x14b52d4d, 0x14551545, 0x13d4f53d, 0x1374dd3a,
	0x1324cd33, 0x12e4bd2b, 0x1294a929, 0x12549126,
	0x12148922, 0x11e4751e, 0x11b46519, 0x11746118,
	0x11545916, 0x11345114, 0x11244912, 0x11244111,
	0x10e44510, 0x10e43d10, 0x10e4410e, 0x1104410f,
	0x11043d10, 0x10f44110, 0x11144511, 0x11244513,
	0x11145115, 0x11545916, 0x11946119, 0x11c46d1b,
	0x11e4791f, 0x11f48523, 0x12449525, 0x12a4a12a,
	0x12c4bd2f, 0x1334cd35, 0x13a4e93b, 0x14150141,
	0x1465154a, 0x14c5314f, 0x15355d57, 0x15c5795e,
	0x1675a16a, 0x16d5d175, 0x1795f180, 0x1886218a,
	0x19a65598, 0x1a3691a9, 0x1b06d9be, 0x1c2721c9,
	0x1d6755e0, 0x1e77a9ee, 0x20082206, 0x20881206,
	0x1f27cdf5, 0x1da76dd7, 0x1c8721cc, 0x1ba6ddb5,
	0x1ab69da9, 0x19866d9d, 0x18c6318c, 0x1826057f,
	0x1785d177, 0x16d5ad6b, 0x16358163, 0x15c56559,
	0x15154551, 0x14a52949, 0x14550542, 0x13c4f13b,
	0x1374d936, 0x1314c530, 0x12b4a92c, 0x12649928,
	0x12548d23, 0x11e47d1f, 0x11a4711a, 0x11846119,
	0x11645516, 0x11445113, 0x11144912, 0x11044111,
	0x10e43d0f, 0x10e4350e, 0x10c4350f, 0x10e4390d,
	0x10e4310e, 0x10e4390d, 0x10f4390e, 0x11043d11,
	0x11144512, 0x11144512, 0x11444d15, 0x11545516,
	0x1174651a, 0x11b4791e, 0x11e48120, 0x12248d24,
	0x12749d28, 0x12d4b12d, 0x1314c532, 0x1394e538,
	0x13d4f141, 0x14351145, 0x14b53150, 0x14f55155,
	0x1595695e, 0x16358d68, 0x16e5c171, 0x1785e579,
	0x18261d86, 0x18e64591, 0x1a1675a6, 0x1ab6c1b4,
	0x1bf705c5, 0x1ce755d8, 0x1ec7a5f0, 0x1fc7fe05,
	0x2097fe04, 0x1f07c1ec, 0x1d4755d8, 0x1c3715c3,
	0x1b76c9b5, 0x1a969da2, 0x19265995, 0x18762989,
	0x17c5f57f, 0x1715c171, 0x16a59d69, 0x15d5755e,
	0x15855555, 0x14d5314f, 0x14551149, 0x13f4fd42,
	0x1374e139, 0x1344cd31, 0x12c4b52d, 0x1284a128,
	0x12348d23, 0x12148520, 0x11c4751d, 0x1194691a,
	0x11545915, 0x11444d13, 0x11144910, 0x10f43d0f,
	0x10d4350f, 0x10c4310c, 0x10a42d0b, 0x10c4310b,
	0x10a42d0d, 0x10b4290b, 0x10c42d0b, 0x10a42d0c,
	0x10c4310d, 0x10d4390d, 0x10d44110, 0x11044511,
	0x11244d12, 0x11445917, 0x1194651b, 0x11d4711e,
	0x11f48122, 0x12649525, 0x1284a52b, 0x12e4b52f,
	0x1324d536, 0x1374ed3b, 0x14050141, 0x1465214a,
	0x15054551, 0x15856158, 0x15d58964, 0x1675ad6c,
	0x1745d97b, 0x18060984, 0x18d63992, 0x1996759e,
	0x1ac6adad, 0x1bb6f1c5, 0x1cf735d2, 0x1e3795ea,
	0x1f57f9f8, 0x2017e1f3, 0x1ef7a5e9, 0x1d4741d6,
	0x1c0701c1, 0x1b46c5b0, 0x1a16799e, 0x19664594,
	0x18561985, 0x17b5f57b, 0x1715b570, 0x16558d67,
	0x15b56d5a, 0x15054953, 0x14952d4a, 0x14350943,
	0x13d4f13d, 0x1374d536, 0x1314bd2e, 0x12a4a92b,
	0x12549526, 0x12248922, 0x11e4791c, 0x11946d1b,
	0x11745916, 0x11445113, 0x11144512, 0x10e43d0f,
	0x10c4310d, 0x10c42d0c, 0x10a4290c, 0x1094290a,
	0x10a4290a, 0x10942109, 0x1084250a, 0x1084290a,
	0x10a4250a, 0x10a42d0b, 0x10c42d0e, 0x10d4350f,
	0x10e43d0e, 0x11044511, 0x11144d14, 0x11545918,
	0x11a4691c, 0x11d4791f, 0x12048d23, 0x12749929,
	0x12a4b12d, 0x1304c933, 0x1354e138, 0x13c4f941,
	0x14551546, 0x14b53150, 0x15455957, 0x15d57561,
	0x16559d6c, 0x1705cd74, 0x17f5f980, 0x1876258d,
	0x1946619e, 0x1a46a5a9, 0x1b76d9bd, 0x1c6729d2,
	0x1dd785e4, 0x1ee7d5fd, 0x1f27d5f8, 0x1e9785dd,
	0x1d373dc8, 0x1be6f5bc, 0x1ab6b5af, 0x19d6699c,
	0x18f6398f, 0x18460986, 0x17a5e176, 0x16c5ad6d,
	0x16158961, 0x15a5655a, 0x14d53d50, 0x14751948,
	0x13f4fd40, 0x1384ed3a, 0x1344cd33, 0x12f4b92d,
	0x1284a128, 0x12448d24, 0x1204791f, 0x11b4691c,
	0x11745d17, 0x11545115, 0x10f44512, 0x10e43d10,
	0x10d4310e, 0x10b4290d, 0x10a4290a, 0x10942109,
	0x10742108, 0x10742508, 0x10642108, 0x10842108,
	0x10741d08, 0x1074210a, 0x10842109, 0x10a4290a,
	0x10b42d0c, 0x10c4310d, 0x10f43d0f, 0x11244512,
	0x11345515, 0x11646119, 0x11c46d1e, 0x12047d20,
	0x12448d26, 0x1294a129, 0x12e4bd30, 0x1354d537,
	0x1394ed3f, 0x14250d43, 0x1485294b, 0x15254553,
	0x1595655b, 0x16359964, 0x16c5b970, 0x1785e17c,
	0x1846158c, 0x18f65599, 0x1a4689a4, 0x1ac6d1b8,
	0x1c3725c8, 0x1d3761df, 0x1e97adf1, 0x1fb7c1f4,
	0x1e3781e1, 0x1ce71dd0, 0x1b66f5bb, 0x1ae6a1aa,
	0x19a66597, 0x18a6398e, 0x18260581, 0x1755d173,
	0x1695a56a, 0x1615795f, 0x15455957, 0x14e5394d,
	0x14651946, 0x13e4fd3e, 0x1374e138, 0x1324c531,
	0x12b4b12d, 0x12949d27, 0x12248923, 0x11d4791c,
	0x11a4651b, 0x11445916, 0x11344914, 0x10f44111,
	0x10d4390c, 0x10b42d0c, 0x10b4290a, 0x10742509,
	0x10641d07, 0x10641d07, 0x10741d07, 0x10641906,
	0x10641507, 0x10641906, 0x10541908, 0x10842106,
	0x10842509, 0x10842509, 0x10b4310c, 0x10c4310e,
	0x11044111, 0x11445113, 0x11545919, 0x11a4691a,
	0x11d4791f, 0x12248924, 0x12849d28, 0x12c4b52e,
	0x1324cd33, 0x1384dd3c, 0x14150140, 0x14552547,
	0x14c54151, 0x1575655a, 0x15f58965, 0x1695b96f,
	0x1755e97d, 0x18561584, 0x19164194, 0x19e685a1,
	0x1ad6bdb5, 0x1c1709cb, 0x1d576ddc, 0x1ee7adf1,
	0x1ea7b9e9, 0x1df765df, 0x1c571dc5, 0x1b36d5b7,
	0x1a8699a7, 0x19865598, 0x18b6218a, 0x17b5f97e,
	0x1715c970, 0x16759966, 0x15d56d5e, 0x15255154,
	0x14d5254b, 0x14351145, 0x13d4f13e, 0x1364d935,
	0x1314b930, 0x12b4a92a, 0x12749525, 0x12148920,
	0x11c4711c, 0x11745d18, 0x11645115, 0x11144511,
	0x10e4390e, 0x10c4310d, 0x10a4290a, 0x10742109,
	0x10841d08, 0x10641906, 0x10541505, 0x10541505,
	0x10441506, 0x10441504, 0x10541505, 0x10641906,
	0x10641907, 0x10741d08, 0x10941d07, 0x1084290b,
	0x10e4310e, 0x10e43d10, 0x11044913, 0x11345515,
	0x11746119, 0x11c4711d, 0x11f48120, 0x12449126,
	0x1284ad2a, 0x1304c134, 0x1364d938, 0x13d4f93f,
	0x14551d46, 0x14d53d4e, 0x15855958, 0x16058961,
	0x1665a56c, 0x1775e175, 0x17d60986, 0x18e63593,
	0x1996759e, 0x1a56b9af, 0x1bc6f5c8, 0x1d2755d8,
	0x1e379de4, 0x1e87c1e9, 0x1d7769da, 0x1c670dc2,
	0x1b36c9b6, 0x1a5691a2, 0x19365d93, 0x18861986,
	0x17d5f17c, 0x1725bd70, 0x16559164, 0x15c5715e,
	0x15254d53, 0x1495294a, 0x14150d44, 0x13b4ed3c,
	0x1364d535, 0x12f4b92e, 0x12a4a92a, 0x12348924,
	0x11e4791f, 0x1194691b, 0x11645915, 0x11244d12,
	0x11043d0f, 0x10c4350d, 0x10a42d0a, 0x10942509,
	0x10641d06, 0x10641506, 0x10441505, 0x10340d06,
	0x10441104, 0x10440d03, 0x10241105, 0x10441104,
	0x10441105, 0x10541506, 0x10541507, 0x10741d08,
	0x1074210a, 0x10b4290b, 0x10d4390e, 0x11044112,
	0x11344d14, 0x11545d17, 0x11a4691c, 0x11e47d21,
	0x12349125, 0x12a4a52b, 0x12e4c131, 0x1354dd37,
	0x13c4f53e, 0x14451147, 0x14a5314d, 0x15655156,
	0x15c57561, 0x1685a56e, 0x1725cd75, 0x17f5fd82,
	0x18c6358c, 0x1996659f, 0x1a76a5b0, 0x1bb6e9c2,
	0x1cd745d5, 0x1e0789e1, 0x1e579de4, 0x1d3761d6,
	0x1c5709c6, 0x1b26bdb3, 0x1a268da4, 0x19163d8f,
	0x18761d85, 0x1775ed78, 0x1715bd6e, 0x16358963,
	0x1565655c, 0x14e54153, 0x14952145, 0x14050540,
	0x1394e93a, 0x1324cd34, 0x12c4b52d, 0x12749926,
	0x12248d23, 0x11d4751d, 0x11b4651b, 0x11445514,
	0x11444512, 0x11043d0e, 0x10d4310c, 0x10942508,
	0x10742109, 0x10641506, 0x10341505, 0x10340d04,
	0x10341103, 0x10340d04, 0x10240d03, 0x10340903,
	0x10340903, 0x10340d04, 0x10340d04, 0x10441504,
	0x10541906, 0x10642108, 0x10a42909, 0x10c4310c,
	0x10f43910, 0x11044913, 0x11545517, 0x1194691a,
	0x11f4791f, 0x12248923, 0x1274a129, 0x12e4b52f,
	0x1334d537, 0x13a4ed3c, 0x14150d45, 0x14952d4d,
	0x15154d55, 0x15b5755e, 0x16759567, 0x16f5c975,
	0x17f5f97c, 0x1886298c, 0x1946659c, 0x1ab6a1a9,
	0x1b86e5ba, 0x1ca741cf, 0x1de789e5, 0x1e2795e4,
	0x1da755d6, 0x1bf705c8, 0x1af6c1b2, 0x1a2681a2,
	0x19263990, 0x18560d86, 0x17a5e978, 0x16b5b56d,
	0x16358d61, 0x15856157, 0x14f53d4f, 0x14852148,
	0x1405013f, 0x1394dd3a, 0x1344c932, 0x12c4b12d,
	0x12649926, 0x12248122, 0x11d4711d, 0x11846118,
	0x11445514, 0x11044111, 0x10d4390e, 0x10b42d0b,
	0x1094210b, 0x10741d07, 0x10641505, 0x10441505,
	0x10241504, 0x10340903, 0x10140902, 0x10240902,
	0x10240902, 0x10240903, 0x10240902, 0x10340d04,
	0x10341105, 0x10541506, 0x10841d07, 0x1094250a,
	0x10c42d0d, 0x10e43d0f, 0x11044913, 0x11445516,
	0x11846119, 0x11b4751e, 0x12048922, 0x1284a528,
	0x12c4b92f, 0x1324d534, 0x13a4e93d, 0x14050144,
	0x1485294a, 0x15054555, 0x15c5715d, 0x16459967,
	0x16e5bd73, 0x17a5f17f, 0x18a6298f, 0x19265d98,
	0x1a7699ab, 0x1b26ddbd, 0x1ce73dd2, 0x1df77de0,
	0x1e4791e0, 0x1d5751d2, 0x1c16f5bd, 0x1b06bdad,
	0x19c679a0, 0x19263d90, 0x18361181, 0x1795d578,
	0x16d5ad6c, 0x15e58161, 0x15855959, 0x14e5394c,
	0x14851945, 0x13c4fd3f, 0x1384dd38, 0x1304c131,
	0x1294ad2a, 0x12749526, 0x12147d21, 0x11c4711c,
	0x11745916, 0x11444913, 0x10f43d10, 0x10e4310d,
	0x10b42d0b, 0x10741d08, 0x10741904, 0x10541105,
	0x10341104, 0x10240903, 0x10340904, 0x10140d03,
	0x10240502, 0x10140502, 0x10240902, 0x10240903,
	0x10240904, 0x10240d04, 0x10441505, 0x10441d08,
	0x10841d09, 0x10a4290a, 0x10d4350e, 0x11244513,
	0x11345115, 0x11745d19, 0x11c4711e, 0x12148524,
	0x12549926, 0x12c4b52d, 0x1334c934, 0x1394ed3c,
	0x13f50142, 0x14752147, 0x15054955, 0x1595695a,
	0x16459167, 0x16d5c173, 0x1775e583, 0x18462989,
	0x1926519b, 0x19e691a9, 0x1b46d9b9, 0x1cb72dd2,
	0x1d9771e1, 0x1de795da, 0x1d8755d5, 0x1be6f9ba,
	0x1ac6b9ab, 0x19c6699b, 0x19063d8b, 0x18060584,
	0x1745d178, 0x1685a56b, 0x15e57560, 0x15655957,
	0x14f5394e, 0x14650d46, 0x13f4f13e, 0x1384d937,
	0x1314c130, 0x12b4a92a, 0x12349525, 0x1204791d,
	0x1194691a, 0x11645916, 0x11344d11, 0x11043d0f,
	0x10b4310d, 0x10a42509, 0x10641d07, 0x10541906,
	0x10441505, 0x10240903, 0x10140902, 0x10140501,
	0x10140101, 0x10140501, 0x10240501, 0x10240501,
	0x10240502, 0x10140904, 0x10240d03, 0x10441104,
	0x10641506, 0x10741d09, 0x10a42509, 0x10e4310e,
	0x11043d11, 0x11344d13, 0x11645d19, 0x11d4711e,
	0x11f48921, 0x12549928, 0x12a4ad2f, 0x1324cd34,
	0x1384e53b, 0x1414fd43, 0x14851d4c, 0x14f54553,
	0x1575655a, 0x16159164, 0x16d5b972, 0x1765e979,
	0x1836198a, 0x19365596, 0x1a0689a9, 0x1b26d5b7,
	0x1c472dcd, 0x1da775e4, 0x1df779dc, 0x1d473dd1,
	0x1be6f9bd, 0x1ac6a9aa, 0x19a6699d, 0x19063d8c,
	0x18260982, 0x1775d975, 0x16b5a566, 0x15f57d5d,
	0x15655154, 0x14e5294e, 0x14451143, 0x13d4f53d,
	0x1364d137, 0x12f4bd30, 0x12a4a92a, 0x12448d22,
	0x11d47d1e, 0x11a4691a, 0x11445517, 0x11244911,
	0x10f4390f, 0x10a4310c, 0x10842108, 0x10641d08,
	0x10541507, 0x10340d04, 0x10340904, 0x10240501,
	0x10040502, 0x10040501, 0x10040502, 0x10140101,
	0x10040102, 0x10140501, 0x10240501, 0x10340d03,
	0x10340d05, 0x10441505, 0x10641908, 0x10a4250b,
	0x10c4310e, 0x11143d10, 0x11144d14, 0x11645d18,
	0x11c46d1c, 0x11e47d23, 0x12549529, 0x12a4b12d,
	0x1304c934, 0x1364e53a, 0x13e4fd3f, 0x14751d4a,
	0x15053d50, 0x15655d5c, 0x16558d66, 0x16d5bd6f,
	0x1785f17d, 0x18261587, 0x1926519a, 0x1a2689a7,
	0x1b36d9b4, 0x1c072dcb, 0x1d676de2, 0x1dd781e5,
	0x1d5735d3, 0x1ba6edbb, 0x1aa6adaa, 0x19a66d9d,
	0x18d6318e, 0x18360984, 0x1735d578, 0x16a59d6c,
	0x1615795e, 0x15655155, 0x14a5354e, 0x14551145,
	0x13c4f13e, 0x1374d936, 0x12f4c12e, 0x1294a527,
	0x12448d21, 0x11e4791f, 0x11b46919, 0x11645516,
	0x11144511, 0x10e4350f, 0x10a42d0b, 0x10742108,
	0x10641905, 0x10541103, 0x10340d03, 0x10240902,
	0x10040501, 0x10140501, 0x10040500, 0x10040101,
	0x10140501, 0x10140103, 0x10240501, 0x10140902,
	0x10240d03, 0x10440d03, 0x10541105, 0x10741908,
	0x10a42508, 0x10b4310d, 0x10f44112, 0x11345115,
	0x11745d19, 0x11c4711c, 0x11f47d22, 0x12549126,
	0x12c4a92c, 0x1304c135, 0x1384e139, 0x14050541,
	0x1455154b, 0x15054153, 0x15756159, 0x16159165,
	0x16d5c16e, 0x1765ed81, 0x18462185, 0x19265196,
	0x19f68da5, 0x1b36ddb9, 0x1c8725cb, 0x1d575ddc,
	0x1df775da, 0x1d0741cd, 0x1bb6e9ba, 0x1ab6a9a9,
	0x19c66997, 0x18e6358d, 0x1805fd7d, 0x1735c974,
	0x16a59d6b, 0x15b5755f, 0x15355154, 0x14c52d4a,
	0x14550d43, 0x13c4f13e, 0x1354d534, 0x1304bd2d,
	0x1294a928, 0x12348d24, 0x11d4791f, 0x1194651a,
	0x11745516, 0x11144512, 0x10f4390d, 0x10e42d0c,
	0x10742508, 0x10741907, 0x10541105, 0x10440d04,
	0x10240902, 0x10040101, 0x10040101, 0x10040100,
	0x10040101, 0x10140100, 0x10040100, 0x10140101,
	0x10140502, 0x10340903, 0x10341103, 0x10541506,
	0x10641908, 0x1094250a, 0x10c4310c, 0x11043d10,
	0x11344915, 0x11546118, 0x11a4711c, 0x12048522,
	0x12749527, 0x12b4ad2d, 0x1324c533, 0x1384e13a,
	0x13f50540, 0x14852549, 0x15054152, 0x15755d59,
	0x16058d67, 0x16c5bd73, 0x17a5e581, 0x18461988,
	0x19465193, 0x19e699a9, 0x1b66d5bc, 0x1cc725cc,
	0x1db775dc, 0x1e1775e0, 0x1ce731ce, 0x1be6ddb7,
	0x1ab6b5a7, 0x19c6659e, 0x18c6358c, 0x17f6017e,
	0x1735d173, 0x16c5a969, 0x15e5795e, 0x15555956,
	0x14c5314a, 0x14551143, 0x13d4f13d, 0x1364d535,
	0x1304c12e, 0x12a4a128, 0x12548923, 0x11d47d1e,
	0x11a4651a, 0x11645914, 0x11244912, 0x10f4390d,
	0x10b4310a, 0x10842108, 0x10841d06, 0x10541504,
	0x10341103, 0x10240d02, 0x10140501, 0x10040101,
	0x10140100, 0x10040100, 0x10140100, 0x10040501,
	0x10140101, 0x10140d01, 0x10240903, 0x10440d04,
	0x10441507, 0x10742108, 0x1094290a, 0x10b4350e,
	0x11043d10, 0x11344d15, 0x11846116, 0x11c4711d,
	0x12048122, 0x12449926, 0x12a4b52e, 0x1304c933,
	0x1384e53b, 0x1414fd42, 0x1485214a, 0x15054552,
	0x15a56559, 0x16359567, 0x16e5c570, 0x1775e97e,
	0x18462189, 0x19664992, 0x1a2695a5, 0x1b26d9b9,
	0x1c8731ca, 0x1de769e0, 0x1db761e0, 0x1cf735d0,
	0x1bc6e1b9, 0x1ac6a5a8, 0x19966997, 0x18d6318b,
	0x17f6057d, 0x1765d174, 0x1695a16a, 0x1605755d,
	0x15654d55, 0x14c5314e, 0x14651544, 0x13b4f13b,
	0x1364d535, 0x12e4bd30, 0x12b4a528, 0x12548d22,
	0x11d4751e, 0x11b4691b, 0x11445516, 0x11244912,
	0x10f4390f, 0x10a4290b, 0x10842108, 0x10541d07,
	0x10541106, 0x10340d03, 0x10340502, 0x10240501,
	0x10040501, 0x10140101, 0x10040500, 0x10040100,
	0x10040500, 0x10140102, 0x10240902, 0x10240d03,
	0x10341103, 0x10541906, 0x10742109, 0x10a4290b,
	0x10b4350f, 0x10f44111, 0x11445115, 0x11745d18,
	0x11a46d1f, 0x12248523, 0x12749d26, 0x12c4b12f,
	0x1314cd33, 0x1384ed3e, 0x14050941, 0x14952549,
	0x15054554, 0x15b5695c, 0x16458d67, 0x1725bd6f,
	0x1795ed7f, 0x1886298a, 0x19565599, 0x1a7699aa,
	0x1b76e9bc, 0x1d0731cc, 0x1e0775e1, 0x1dd775da,
	0x1cc745d1, 0x1bb6e9b9, 0x1ac6a5ab, 0x19c66d9c,
	0x18a6358c, 0x18160180, 0x1755d176, 0x1675a568,
	0x15d57d5d, 0x15755555, 0x14c52d4c, 0x14550d44,
	0x13c4f53d, 0x1374d936, 0x12e4bd2f, 0x12a4a529,
	0x12449124, 0x11d4791f, 0x1184691a, 0x11645515,
	0x11244912, 0x10e4350f, 0x10c4290b, 0x10942109,
	0x10641d06, 0x10541104, 0x10241104, 0x10140901,
	0x10040901, 0x10140901, 0x10140100, 0x10140102,
	0x10040100, 0x10040501, 0x10140901, 0x10240902,
	0x10340d05, 0x10441106, 0x10541906, 0x10842108,
	0x10a42d0c, 0x10c4390e, 0x11044112, 0x11444d15,
	0x11846519, 0x11d4751e, 0x12148923, 0x1264a128,
	0x12d4b930, 0x1344d534, 0x13a4e93c, 0x14250542,
	0x1485214d, 0x15154553, 0x15a56d60, 0x16559968,
	0x16f5c972, 0x17f5f17f, 0x18d6218a, 0x19865599,
	0x1a36a5ac, 0x1b76e1bc, 0x1cb731d3, 0x1d6775e5,
	0x1db775e0, 0x1cf741d5, 0x1b96e9be, 0x1ab6a5ab,
	0x19e6719e, 0x18c6358a, 0x18260583, 0x1735d973,
	0x16c5a56b, 0x16057d60, 0x15555554, 0x14c5314b,
	0x14551544, 0x13e4f53e, 0x1374d935, 0x1304bd30,
	0x12a4a52a, 0x12449524, 0x11f47d1e, 0x11a4691a,
	0x11645916, 0x11244d14, 0x10f4410e, 0x10c4310c,
	0x1094290a, 0x10741d07, 0x10641506, 0x10441104,
	0x10340d03, 0x10240502, 0x10140501, 0x10140502,
	0x10140501, 0x10140101, 0x10240902, 0x10140502,
	0x10340d02, 0x10341104, 0x10541106, 0x10741d07,
	0x1084250b, 0x10b4290d, 0x10e4350f, 0x11244913,
	0x11545517, 0x1184651a, 0x1204791f, 0x12148d25,
	0x12a4a12b, 0x12e4b92f, 0x1344d536, 0x13c4f13b,
	0x14151143, 0x14952d4e, 0x15354956, 0x15b57560,
	0x1665a16c, 0x1715c576, 0x17e60580, 0x18762590,
	0x19366d9e, 0x1a76a5aa, 0x1b76f1c1, 0x1cb741d6,
	0x1de77de5, 0x1e0785e2, 0x1ce73dd2, 0x1be6edc0,
	0x1b06a1ad, 0x19d6799e, 0x18d6358e, 0x18260184,
	0x1755d173, 0x16a5a968, 0x15f5815f, 0x15555956,
	0x14f5314b, 0x14551542, 0x13f4f53d, 0x1374dd37,
	0x1314bd31, 0x12c4a92b, 0x12649526, 0x12248121,
	0x11b46d1c, 0x11645917, 0x11444d14, 0x11044110,
	0x10e4310c, 0x10a4250a, 0x10a42107, 0x10641d06,
	0x10441506, 0x10440d03, 0x10340d03, 0x10240901,
	0x10140902, 0x10240902, 0x10140901, 0x10140902,
	0x10240903, 0x10340903, 0x10541506, 0x10641906,
	0x10942108, 0x1084290b, 0x10c4350e, 0x11044510,
	0x11144d15, 0x11645d18, 0x11946d1b, 0x11e47d21,
	0x12349125, 0x12a4a92a, 0x12f4c131, 0x1374d537,
	0x13e4f540, 0x14351548, 0x14b5314e, 0x15555559,
	0x15c5795d, 0x16a5a56c, 0x1735cd77, 0x17f60982,
	0x18b6318d, 0x19c6619f, 0x1ac6adb1, 0x1bb6f1c0,
	0x1cf741d3, 0x1e7791e4, 0x1de765d9, 0x1d2735ca,
	0x1bc6f9bd, 0x1ae6adaa, 0x19e6719e, 0x1916318e,
	0x18161181, 0x1775d577, 0x16d5ad6b, 0x15e5815f,
	0x15656156, 0x14f5354b, 0x14751546, 0x13e4f93f,
	0x1384dd36, 0x1324c932, 0x12b4ad2a, 0x12549528,
	0x11f48120, 0x11b4751c, 0x1184611a, 0x11545114,
	0x11144511, 0x10e4390f, 0x10c42d0b, 0x10a42509,
	0x10841d09, 0x10641506, 0x10441505, 0x10341104,
	0x10240d03, 0x10240d03, 0x10340d01, 0x10240d03,
	0x10240d03, 0x10441103, 0x10341105, 0x10541505,
	0x10741908, 0x10942108, 0x10b42d0b, 0x10e4350d,
	0x10f44511, 0x11345115, 0x11846119, 0x11b46d1c,
	0x12048523, 0x12649525, 0x12b4b12e, 0x12f4c531,
	0x1384e138, 0x13e5053f, 0x14551d48, 0x14b54151,
	0x15755d59, 0x16157d61, 0x16a5b16c, 0x17b5d579,
	0x18360183, 0x18d63d90, 0x19f67da2, 0x1ab6b5b0,
	0x1c070dc1, 0x1cf751d7, 0x1e4795e9, 0x1e2775d7,
	0x1d9735cf, 0x1b96f1bd, 0x1ae6a5ad, 0x1a26759b,
	0x19164193, 0x18660583, 0x1795d975, 0x16e5b16b,
	0x16258162, 0x15956159, 0x14e5354f, 0x14751947,
	0x1404fd3f, 0x1394dd3a, 0x1324c932, 0x12e4ad2c,
	0x12849d27, 0x12248123, 0x11c4751e, 0x11945d1a,
	0x11445517, 0x11244911, 0x10f43d0f, 0x10b4310c,
	0x10a4290a, 0x10842109, 0x10741906, 0x10641506,
	0x10541506, 0x10341104, 0x10440d05, 0x10340d02,
	0x10540d02, 0x10440d03, 0x10441505, 0x10541506,
	0x10741907, 0x10741d07, 0x10a4290b, 0x10c4310d,
	0x11043d10, 0x11244513, 0x11545515, 0x11a46519,
	0x11d4791d, 0x12248922, 0x12649d28, 0x12c4b52d,
	0x1314cd35, 0x1394e139, 0x14050142, 0x1465214a,
	0x14e54153, 0x15856958, 0x16259161, 0x16c5ad6e,
	0x1755e178, 0x18260d86, 0x19164191, 0x1a36899f,
	0x1b26cdb1, 0x1c2715c9, 0x1d6751da, 0x1ec7a1ed,
	0x1dd771df, 0x1d3741d0, 0x1be6edbf, 0x1b26b5b1,
	0x1a067da0, 0x19363d91, 0x18760987, 0x1775d97b,
	0x1725bd6c, 0x16358d61, 0x15956d57, 0x1505454f,
	0x14852148, 0x1415053e, 0x13c4e93b, 0x1344d534,
	0x12f4bd2f, 0x12949d28, 0x12349123, 0x11f47d1f,
	0x11b46d1a, 0x11645917, 0x11244d13, 0x11044110,
	0x10f4350d, 0x10c42d0b, 0x10a42509, 0x10841d08,
	0x10641907, 0x10641506, 0x10541506, 0x10441503,
	0x10441505, 0x10441505, 0x10641506, 0x10541907,
	0x10741d08, 0x10742109, 0x10842509, 0x10c42d0d,
	0x10d4350e, 0x11044111, 0x11444d15, 0x11745d19,
	0x11b46d1b, 0x11e4811f, 0x12349125, 0x1284ad28,
	0x12e4b930, 0x1354d537, 0x13a4e53d, 0x14350944,
	0x14b5294b, 0x15354954, 0x15a5755e, 0x16359567,
	0x1705bd70, 0x1795e17d, 0x18762189, 0x19764997,
	0x19f685a8, 0x1b56cdb8, 0x1c5725ce, 0x1d976ddd,
	0x1ee7b9e7, 0x1e0785df, 0x1d474dd1, 0x1be6f9c0,
	0x1af6b9af, 0x19d67d9e, 0x19363995, 0x18661188,
	0x17b5e579, 0x16f5b96d, 0x16359564, 0x15b56959,
	0x15254553, 0x14b5254b, 0x14350943, 0x13e4e93b,
	0x1354d935, 0x1304bd30, 0x12b4a52c, 0x12549527,
	0x11f4811f, 0x11c4711b, 0x11846118, 0x11645514,
	0x11144512, 0x10f43d0f, 0x10e4350d, 0x10b42d0c,
	0x10942909, 0x10942509, 0x10742108, 0x10741907,
	0x10741907, 0x10641d06, 0x10641d06, 0x10741907,
	0x10842108, 0x10742509, 0x1094250b, 0x10a4290e,
	0x10d4350e, 0x11043d11, 0x11144913, 0x11645916,
	0x1194651b, 0x11b4751e, 0x12148522, 0x12449926,
	0x12c4a92d, 0x1304c533, 0x1364d939, 0x13d4f53d,
	0x14451546, 0x14d52d4f, 0x15655954, 0x15e57d5e,
	0x1685a56b, 0x1715cd74, 0x17c5fd7f, 0x18962d8b,
	0x19665d99, 0x1a5699a8, 0x1b76d9bc, 0x1c5729d1,
	0x1db771e2, 0x1f07b5f5, 0x1e5789d4, 0x1d5755d5,
	0x1c2705c3, 0x1ae6c5b3, 0x1a6685a1, 0x19865593,
	0x18761d87, 0x17b5e97b, 0x1735c573, 0x16559565,
	0x15e5715c, 0x15454952, 0x14e5354b, 0x14551545,
	0x13e4f93f, 0x1394e139, 0x1314c531, 0x12c4ad2b,
	0x12849926, 0x12248d21, 0x11e4751d, 0x11a46919,
	0x11745917, 0x11345113, 0x11344110, 0x11043d0e,
	0x10d4350d, 0x10b4310c, 0x10b4290a, 0x1094210b,
	0x10942509, 0x1074210a, 0x10841d09, 0x10a42108,
	0x10942509, 0x10a42509, 0x10a4250b, 0x10b4310b,
	0x10c4390e, 0x1104410f, 0x11144914, 0x11345114,
	0x11745d19, 0x11a4711d, 0x11d47d1f, 0x12249124,
	0x1274a529, 0x12d4b92f, 0x1344cd36, 0x1394e93a,
	0x14050142, 0x14851d48, 0x15154551, 0x15356159,
	0x16258563, 0x1695ad6c, 0x1755d975, 0x17d60183,
	0x18f63991, 0x19a67d9c, 0x1a96b5b1, 0x1ba6f1be,
	0x1cf73dcd, 0x1dd785e9, 0x1ef7c9f7, 0x1e5771dc,
	0x1d675dd5, 0x1c4705bc, 0x1b46c9af, 0x1a9691a6,
	0x19565597, 0x18c61985, 0x1805ed7b, 0x1725cd72,
	0x1695a564, 0x15d5755f, 0x15755157, 0x14c5394e,
	0x14551545, 0x1404f940, 0x1394e938, 0x1364cd32,
	0x12c4b52d, 0x1294a127, 0x12548d23, 0x11e48120,
	0x11c46d1c, 0x11846519, 0x11545515, 0x11344513,
	0x11044110, 0x10e43d0f, 0x10d4390d, 0x10c4310c,
	0x10b42d0b, 0x10b42909, 0x10a4250a, 0x10842509,
	0x10a42509, 0x1094290a, 0x10b42d0b, 0x10c4310c,
	0x10d4350f, 0x10f43d0e, 0x11244511, 0x11545114,
	0x11745918, 0x11a46919, 0x11d4791f, 0x12048923,
	0x12549125, 0x1294a92b, 0x12f4c12f, 0x1374d535,
	0x13b4ed3d, 0x14350542, 0x14b5254b, 0x15054952,
	0x15956d5c, 0x16359163, 0x16d5b56f, 0x1795e179,
	0x18261586, 0x18c64d91, 0x1a166da1, 0x1af6b1b0,
	0x1c0701c5, 0x1d3761cf, 0x1e87a5eb, 0x1f87e5f6,
	0x1e5781dc, 0x1d9765d8, 0x1c5719c3, 0x1b66d5b1,
	0x1a9699a2, 0x1a066593, 0x18d62588, 0x17e5f97f,
	0x1755d173, 0x16a59d6b, 0x16257d5d, 0x15855558,
	0x15254150, 0x14a52148, 0x14150941, 0x13d4ed3a,
	0x1344d535, 0x12f4bd31, 0x12a4ad29, 0x12749527,
	0x12248521, 0x11d4751d, 0x11b46919, 0x11646118,
	0x11645514, 0x11244d12, 0x11044510, 0x11044110,
	0x10e4390f, 0x10d4350e, 0x10d4350b, 0x10b4310d,
	0x10c42d0c, 0x10b4310c, 0x10b4310e, 0x10c4350d,
	0x10e4390e, 0x10f44510, 0x11044912, 0x11244d14,
	0x11545516, 0x11846119, 0x11c4751b, 0x12147d21,
	0x12348d26, 0x12849d28, 0x12e4b92e, 0x1324c933,
	0x1364e539, 0x13e4f942, 0x14551d45, 0x14b5394d,
	0x15855156, 0x15e57961, 0x1655a169, 0x1705c975,
	0x17a5f57f, 0x1896218a, 0x19265596, 0x1a368da8,
	0x1b06d5b4, 0x1c4719c0, 0x1d4769db, 0x1ee7a1ef,
	0x2007d601, 0x1dd78de4, 0x1d8765d9, 0x1ce729c6,
	0x1bd6e5b9, 0x1ab6adab, 0x19a6699c, 0x18f6398f,
	0x1835fd82, 0x1775d174, 0x16c5a96b, 0x16358560,
	0x15b56959, 0x15254551, 0x14b52d4a, 0x14250d44,
	0x13f4f93d, 0x1364d937, 0x1324c532, 0x12e4b52e,
	0x1294a529, 0x12649122, 0x1214811f, 0x11d4711d,
	0x11a4651a, 0x11745d17, 0x11645516, 0x11444d13,
	0x11344512, 0x11044510, 0x11043d0f, 0x10f43910,
	0x10f4350f, 0x10f4390e, 0x10f4390e, 0x11043d0f,
	0x11143d11, 0x10f44512, 0x11244913, 0x11345113,
	0x11545d16, 0x1194651a, 0x11b46d1d, 0x11e47d20,
	0x12448d24, 0x12749529, 0x12c4a92c, 0x12f4c131,
	0x1364d536, 0x13a4ed3b, 0x14250943, 0x1495254b,
	0x15154551, 0x15a5615a, 0x16158d63, 0x16b5b569,
	0x1735d178, 0x17d60182, 0x18a6318e, 0x19766d99,
	0x1a8699ad, 0x1ba6e9c0, 0x1cc72dcf, 0x1dc779d9,
	0x1ee7c5f5, 0x1fe80a01, 0x1e679de2, 0x1dc769d8,
	0x1cd72dc9, 0x1b96ddba, 0x1ad6b5ad, 0x19d6819d,
	0x19265593, 0x18861585, 0x17c5e57a, 0x1715bd6b,
	0x16759964, 0x15f5715c, 0x15355956, 0x14d5394f,
	0x14451943, 0x13f4fd3f, 0x13b4e53a, 0x1354d933,
	0x1314c130, 0x12d4b12b, 0x12549d26, 0x12348d24,
	0x1204791f, 0x11c4751c, 0x11946519, 0x11946117,
	0x11745116, 0x11344d14, 0x11344912, 0x11244914,
	0x11244512, 0x11044511, 0x11244511, 0x11244510,
	0x11244512, 0x11244913, 0x11344d13, 0x11445516,
	0x11645918, 0x1194651a, 0x11a4711d, 0x11e4791d,
	0x12248d24, 0x12549527, 0x1294a92a, 0x12c4b92e,
	0x1344cd34, 0x1394e537, 0x13f5013e, 0x14551946,
	0x14d5394d, 0x15455956, 0x15d5715d, 0x16259d66,
	0x16e5c56d, 0x1785dd7b, 0x18561d86, 0x19164590,
	0x19e67da2, 0x1ab6c1b2, 0x1be701bf, 0x1d0745d8,
	0x1df77de6, 0x1f67e5f6, 0x20981a07, 0x1ed795e7,
	0x1e276dd8, 0x1cb72dca, 0x1bc6f1ba, 0x1af6c9ad,
	0x1a368da3, 0x19364d93, 0x18a61d86, 0x17c5f578,
	0x1715c570, 0x1695a965, 0x16057d5c, 0x15855d57,
	0x15153d4e, 0x14951946, 0x14350943, 0x13c4f13b,
	0x1374dd35, 0x1324c532, 0x12e4b12d, 0x12a49d29,
	0x12549125, 0x12148520, 0x11f4791e, 0x11c46d1b,
	0x11a46919, 0x11946118, 0x11745916, 0x11545914,
	0x11345514, 0x11544d15, 0x11445114, 0x11344d12,
	0x11245113, 0x11444913, 0x11545115, 0x11645d16,
	0x11945d18, 0x11a46519, 0x11c46d1a, 0x11e4791e,
	0x12148122, 0x12649125, 0x1284a52a, 0x12f4b52b,
	0x1324c132, 0x1344d937, 0x13b4ed3b, 0x14150543,
	0x14852149, 0x15153d51, 0x1565615b, 0x16058563,
	0x1695a16a, 0x1745c571, 0x17f5f581, 0x18b6258c,
	0x19465993, 0x1a268da1, 0x1b16c9b4, 0x1c770dc8,
	0x1d6751d6, 0x1e4799e6, 0x1f87ea00, 0x20a82a01,
	0x1ef7a9eb, 0x1df779db, 0x1ce73dc9, 0x1c3705c0,
	0x1b16d1ae, 0x1a5691a3, 0x19965d98, 0x18b62d8c,
	0x1826017f, 0x1735d175, 0x16c5a169, 0x16058963,
	0x15b56d59, 0x15354952, 0x14c52d4b, 0x14651946,
	0x1414fd3c, 0x13a4e53a, 0x1364d935, 0x12f4c530,
	0x12d4b12e, 0x1294a528, 0x12349923, 0x12248920,
	0x11f47d1f, 0x11c4711e, 0x11b46d1b, 0x11946118,
	0x11846519, 0x11646117, 0x11645d17, 0x11745916,
	0x11745917, 0x11745917, 0x11845916, 0x11845d18,
	0x11a46517, 0x11c46d1d, 0x11d4751d, 0x1204791f,
	0x12148923, 0x12549924, 0x12a4a12a, 0x12c4b12d,
	0x12e4bd2f, 0x1344d535, 0x1384e13c, 0x14050141,
	0x14751d46, 0x14c5394d, 0x15455955, 0x15e5755c,
	0x16359967, 0x1705b56e, 0x1725dd77, 0x18260986,
	0x18d64191, 0x19e6759d, 0x1a96a9ac, 0x1be6e5b9,
	0x1cc731d0, 0x1dc771d9, 0x1ec7b9e9, 0x2007ee05,
	0x21482e12, 0x1eb7a5e2, 0x1e078ddd, 0x1d1741d0,
	0x1c5705bc, 0x1be6e1b1, 0x1aa69da5, 0x19a66597,
	0x1936398c, 0x18361986, 0x17a5d975, 0x1735bd6f,
	0x16659567, 0x15e5795d, 0x15454d54, 0x1505394d,
	0x14852546, 0x14350943, 0x13e4f53a, 0x1394dd36,
	0x1344cd33, 0x1314b92f, 0x12b4ad2c, 0x1274a528,
	0x12649123, 0x12348923, 0x12048120, 0x12047920,
	0x11e4711b, 0x11b46d1b, 0x1194691b, 0x11c4651a,
	0x11a46519, 0x11a4691a, 0x11a4651a, 0x11b4691a,
	0x11c46d1a, 0x11d4711c, 0x11e4791f, 0x12048520,
	0x12048d24, 0x12449927, 0x12a4a92a, 0x12d4b12c,
	0x12f4c130, 0x1334d133, 0x1394e139, 0x13d4f140,
	0x14351143, 0x14c5294c, 0x15254d52, 0x15a56557,
	0x16158161, 0x1695a969, 0x1715cd78, 0x17a5f57c,
	0x1826218a, 0x19364993, 0x19f685a2, 0x1ae6bdb1,
	0x1c0705c1, 0x1d173dd7, 0x1e377de2, 0x1f37d9f3,
	0x20580600, 0x21283216, 0x1eb7b1e9, 0x1e2785e3,
	0x1d574dd2, 0x1cc719c6, 0x1b96edba, 0x1a96b1aa,
	0x1a16759f, 0x19364593, 0x18762184, 0x17c5f17d,
	0x1755c970, 0x16b59d67, 0x1615855f, 0x15a56557,
	0x15154551, 0x14d52d4b, 0x14651146, 0x1414fd40,
	0x13b4ed3a, 0x13a4d935, 0x1334cd32, 0x12f4b52e,
	0x12d4a92b, 0x1284a129, 0x12649926, 0x12449123,
	0x1214851f, 0x12348120, 0x1204811f, 0x11e4751e,
	0x11a4751f, 0x11d4751e, 0x11e4751d, 0x11e4751d,
	0x11f4791e, 0x11f4811f, 0x12248120, 0x12548923,
	0x12349123, 0x12849928, 0x1294a52a, 0x12d4b12f,
	0x1304c130, 0x1374cd35, 0x1394e138, 0x13d4ed41,
	0x14050d45, 0x14652149, 0x14d54150, 0x15555d58,
	0x15c57560, 0x16659d65, 0x16c5b96b, 0x1775dd76,
	0x18161580, 0x18e63591, 0x19d66d9b, 0x1aa6a9ac,
	0x1b46d9b8, 0x1c6719cc, 0x1d775dde, 0x1e17a9f0,
	0x1fa7e5fe, 0x20481e0b, 0x21485e15, 0x1f17adf7,
	0x1e9799df, 0x1d274dd7, 0x1cc721c6, 0x1ba6e9bc,
	0x1b26b5ae, 0x1a36899d, 0x19765991, 0x18d62588,
	0x17e5f57c, 0x1745d173, 0x1705a96a, 0x16559163,
	0x15c5715a, 0x15455555, 0x1505394e, 0x14952147,
	0x14550942, 0x13e5013d, 0x1394e938, 0x1354d533,
	0x1324c932, 0x12f4b92e, 0x12c4a92b, 0x1294a528,
	0x12949927, 0x12549525, 0x12448d23, 0x12248923,
	0x1204851f, 0x11f48120, 0x12048120, 0x1204811f,
	0x12047d20, 0x11f48921, 0x12148521, 0x12449124,
	0x12549926, 0x12749d28, 0x1294a92a, 0x12c4b12c,
	0x12e4c12e, 0x1344cd33, 0x1384e13a, 0x13b4f13b,
	0x1404f942, 0x14851546, 0x14d5314a, 0x15054d54,
	0x15a5655b, 0x16157d63, 0x1695a167, 0x1715c972,
	0x17b5f57a, 0x18761185, 0x19664999, 0x19e681a2,
	0x1ae6bdac, 0x1b86f1bc, 0x1cb73dcd, 0x1db769e1,
	0x1f27aded, 0x2017f5f8, 0x20e8360d, 0x21b86216,
	0x1f47c9f1, 0x1e7795e2, 0x1d8765d8, 0x1d4739c9,
	0x1c1701b6, 0x1b26c1b2, 0x1a56a1a4, 0x19c65d94,
	0x18f63d8c, 0x18561183, 0x17a5e578, 0x1725b96d,
	0x16a59563, 0x16157d5f, 0x15a55d58, 0x1525454f,
	0x14e5314b, 0x14951d46, 0x14350940, 0x13e4f93b,
	0x1384e538, 0x1364d535, 0x1364c533, 0x1304bd2e,
	0x12e4b12d, 0x12c4ad28, 0x1294a128, 0x12949926,
	0x12549525, 0x12448d24, 0x12448922, 0x12448d23,
	0x12348d25, 0x12649123, 0x12549525, 0x12749127,
	0x1284a128, 0x12b4a529, 0x12d4a92c, 0x12f4bd2f,
	0x1304c52f, 0x1344d133, 0x1384dd37, 0x13b4ed39,
	0x13f50141, 0x14751144, 0x14a52d4d, 0x14f54153,
	0x15956557, 0x15b5795f, 0x16559d67, 0x16f5c56e,
	0x1775e176, 0x18260582, 0x18c6398e, 0x19866599,
	0x1a8689a6, 0x1b76cdb4, 0x1c0721bf, 0x1d375dd7,
	0x1e6799e1, 0x1f27ddf5, 0x20280e06, 0x21685213,
	0x21a87e12, 0x1f97d1f3, 0x1e979de6, 0x1db765d5,
	0x1d3735cf, 0x1c06fdc1, 0x1ba6e1b4, 0x1b06adaa,
	0x1a4675a0, 0x1956458f, 0x18862186, 0x1815f97d,
	0x1755d573, 0x16d5ad68, 0x16459161, 0x15d5755a,
	0x15455554, 0x15054150, 0x14b52949, 0x14651143,
	0x14250941, 0x13e4f93e, 0x1384e93a, 0x1374d936,
	0x1344cd30, 0x1304c12f, 0x1314bd2c, 0x12c4b12b,
	0x1294b129, 0x1294a52c, 0x1284a127, 0x1284a127,
	0x12649d27, 0x12749d28, 0x1284a528, 0x12b4a529,
	0x12b4a92a, 0x12c4ad2e, 0x12d4b92c, 0x1314c12f,
	0x1324c931, 0x1364d535, 0x1394dd37, 0x13b4f13d,
	0x14050140, 0x14350d46, 0x1485294a, 0x14f53d52,
	0x15755157, 0x15d56d5a, 0x16658d63, 0x16d5a96b,
	0x1735cd78, 0x1805f57e, 0x18862587, 0x19464d8e,
	0x19e6759f, 0x1ae6b1ad, 0x1be6edbe, 0x1cf735c7,
	0x1da769d8, 0x1e97ade7, 0x1fa7ddfd, 0x2038220a,
	0x21685613, 0x22288626, 0x1f47d9f8, 0x1ec7a9e6,
	0x1dc779dd, 0x1d0741cf, 0x1ca71dc3, 0x1bd6edb8,
	0x1b46c1ad, 0x1aa68da0, 0x19a66198, 0x18e6358b,
	0x18260d84, 0x17a5e17a, 0x16f5bd6f, 0x16659d67,
	0x16257d5e, 0x15d56959, 0x15655154, 0x15153950,
	0x14b5294a, 0x14652144, 0x1425093f, 0x13d4f13f,
	0x13b4e938, 0x13a4dd36, 0x1374d535, 0x1334cd32,
	0x1314bd31, 0x12e4b92e, 0x12d4b92e, 0x12d4b52c,
	0x12d4ad2b, 0x12e4a92c, 0x12e4b12c, 0x12c4b12d,
	0x12d4b92d, 0x12e4b92f, 0x1304bd31, 0x1334c532,
	0x1334cd32, 0x1374dd37, 0x13c4e138, 0x13d4fd3d,
	0x14050140, 0x14651545, 0x1495254b, 0x14d5414e,
	0x15655555, 0x15b5715a, 0x16158962, 0x16859d69,
	0x1715c171, 0x1775e976, 0x18260d84, 0x18c63593,
	0x1986619c, 0x1ab6a1a9, 0x1b66ddb1, 0x1c4715c6,
	0x1d0745d1, 0x1e3779e0, 0x1ee7b9f1, 0x1fe7f9fd,
	0x20783e0b, 0x2148661d, 0x22288a23, 0x2017d5f4,
	0x1f47aded, 0x1e377ddb, 0x1d574dd9, 0x1c771dca,
	0x1c06f1ba, 0x1b76d1b3, 0x1a76a9a4, 0x19e66999,
	0x1916398b, 0x18861986, 0x17c5ed7b, 0x1735cd74,
	0x16e5a96a, 0x16458d61, 0x15b5755c, 0x15755d55,
	0x15454d52, 0x14f5314c, 0x14a52948, 0x14551942,
	0x14350141, 0x13e4f93c, 0x13d4e938, 0x1394dd35,
	0x1364d536, 0x1354d133, 0x1314c531, 0x1314c130,
	0x1314b92f, 0x1304bd30, 0x1304b92d, 0x1304bd2d,
	0x1314c12f, 0x1304c92f, 0x1344c532, 0x1344cd33,
	0x1364d533, 0x1364e135, 0x13b4e539, 0x13f4f13f,
	0x1414fd3f, 0x14351545, 0x14851d4a, 0x1505394d,
	0x15354952, 0x15c55d58, 0x15f57961, 0x16459967,
	0x16d5b96d, 0x1735d973, 0x1825fd82, 0x18a62987,
	0x19164d92, 0x19c67da1, 0x1af6b5ad, 0x1b96f5be,
	0x1c972dca, 0x1d6765d9, 0x1e9795e6, 0x1f87c1ed,
	0x202811fe, 0x20e8520e, 0x21a8721b, 0x2268ca2a,
	0x2017f1fb, 0x1f47b9f2, 0x1e3785e4, 0x1d8769d3,
	0x1d3731cf, 0x1c0711bd, 0x1b86e1b2, 0x1b26b1ac,
	0x19f6799e, 0x19c65d94, 0x18d6318b, 0x18260180,
	0x17a5e174, 0x1725c96f, 0x1675a166, 0x16558560,
	0x15d5755c, 0x15755d55, 0x15154150, 0x15052d4d,
	0x14d52547, 0x14751544, 0x14350543, 0x1414f93e,
	0x13b4f53c, 0x1394ed3b, 0x1394e537, 0x1374d935,
	0x1344d935, 0x1354d932, 0x1344c935, 0x1344cd32,
	0x1364d135, 0x1344d133, 0x1374d936, 0x1384dd37,
	0x1394e938, 0x13d4e53a, 0x13d4f53e, 0x13f4f93f,
	0x14450d40, 0x14751548, 0x14c5294c, 0x15053d4d,
	0x15354951, 0x15955d58, 0x15d5795f, 0x16959168,
	0x16e5bd6e, 0x1755d974, 0x17e5f17d, 0x18761585,
	0x19264190, 0x19c6699b, 0x1a66a9ad, 0x1b56ddb7,
	0x1c4711c6, 0x1d4759d8, 0x1e477de6, 0x1f27b5f4,
	0x1f87de02, 0x20582e10, 0x21484a15, 0x22288e21,
	0x2298b22c, 0x203821fd, 0x1f47ddf2, 0x1e7799db,
	0x1de761d7, 0x1d973dd2, 0x1c8721c7, 0x1b96f1b9,
	0x1b86cdb0, 0x1ac699a6, 0x19a66599, 0x19764190,
	0x18e62187, 0x17f6017e, 0x1765d174, 0x16b5bd6c,
	0x16859965, 0x16157d60, 0x15c56d59, 0x15456156,
	0x15154d50, 0x1505354b, 0x14d5294a, 0x14651545,
	0x14350942, 0x14150141, 0x1414f93d, 0x13f4f53c,
	0x13b4ed3a, 0x13a4e937, 0x1394dd37, 0x13b4e138,
	0x1384e138, 0x13c4e137, 0x13d4e939, 0x13b4ed39,
	0x13d4f53c, 0x13e4f13e, 0x14050540, 0x1445093e,
	0x14651543, 0x14952149, 0x14c5314c, 0x15053953,
	0x15555552, 0x15756558, 0x1605815d, 0x16359564,
	0x16b5a56d, 0x1735cd75, 0x17c5f17a, 0x18361584,
	0x18f6398c, 0x19b6659a, 0x1a4699a2, 0x1b16d1b0,
	0x1bf6f5c0, 0x1cd739cd, 0x1df76dd8, 0x1e67b5e4,
	0x1f57ddf4, 0x2037f202, 0x21482e0f, 0x21386e19,
	0x22e8aa27, 0x2348ca35, 0x20d8220a, 0x1fc7edfd,
	0x1eb7a9e9, 0x1de779d8, 0x1da745d6, 0x1cc731c8,
	0x1c5709b9, 0x1b86d9b2, 0x1ae6b9ae, 0x1a66859f,
	0x19d65d92, 0x19064189, 0x18561183, 0x17a5f57b,
	0x1745c572, 0x16c5b16e, 0x16659165, 0x1625795e,
	0x15a5715a, 0x15955953, 0x15354151, 0x1515354d,
	0x14c5294a, 0x14a52947, 0x14651545, 0x14250d43,
	0x14150941, 0x14050540, 0x1404fd3f, 0x13f4fd3d,
	0x13f4f53e, 0x13f4fd3e, 0x13f4f53c, 0x1404f53e,
	0x1425013f, 0x14250540, 0x14550d44, 0x14651944,
	0x14c52549, 0x14c5314c, 0x14e53d4c, 0x15154d52,
	0x15555957, 0x15c5755b, 0x1605815e, 0x16659d62,
	0x16d5bd6d, 0x1745cd72, 0x1795ed7d, 0x18461582,
	0x18e63d8e, 0x19966597, 0x1a3691a4, 0x1b16c1b3,
	0x1c16f1bb, 0x1c773dc8, 0x1d3761d8, 0x1e779de2,
	0x1f27c1f1, 0x1fd7e200, 0x20581a0d, 0x21883e0e,
	0x22288a21, 0x22f8aa28, 0x2398ee35, 0x21a83e11,
	0x20780204, 0x1f07b5ea, 0x1df771e0, 0x1df761d7,
	0x1d0735cc, 0x1c6719c0, 0x1bf6f1b6, 0x1b36cdaf,
	0x1aa68da4, 0x1a26719e, 0x19465593, 0x18a62585,
	0x1826017b, 0x17a5e579, 0x1735c172, 0x1695ad68,
	0x1655895f, 0x1615855c, 0x15c56958, 0x15755d53,
	0x15354952, 0x1515394e, 0x14f53549, 0x14b52148,
	0x14a51d47, 0x14951d45, 0x14751544, 0x14350d42,
	0x14350940, 0x1455093f, 0x1445093f, 0x14450d41,
	0x14351141, 0x14651145, 0x14752544, 0x14a51d43,
	0x14c53149, 0x14e5394b, 0x15054151, 0x15455155,
	0x15955d58, 0x15c5715b, 0x1605815f, 0x16759d67,
	0x16d5b16d, 0x1725cd73, 0x17a5e979, 0x18160d83,
	0x18b6318c, 0x19465995, 0x19f68da0, 0x1aa6b5aa,
	0x1b76e9b5, 0x1c1719c2, 0x1d4749d1, 0x1e3785e0,
	0x1ee7b5eb, 0x1fc7d9f6, 0x20080207, 0x20d83e0a,
	0x21885e13, 0x22588a1e, 0x22c8ba2e, 0x2368ee43,
	0x22287617, 0x21385608, 0x2037ddf3, 0x1e77a9e6,
	0x1db771dc, 0x1d5755d1, 0x1cb72dca, 0x1c5709c1,
	0x1bc6ddb3, 0x1ae6bdac, 0x1a868da0, 0x19a66598,
	0x1946458b, 0x18b62d89, 0x17e6057e, 0x17a5e579,
	0x1735c16f, 0x16d5a967, 0x16859562, 0x1635855d,
	0x15c5715d, 0x15956558, 0x15655554, 0x15254551,
	0x15254550, 0x1515354c, 0x14d5354a, 0x14b52d49,
	0x14b52d4b, 0x14b52547, 0x14952147, 0x14a52147,
	0x14b52148, 0x14b52d49, 0x14b52d4c, 0x14e5354b,
	0x14f5394f, 0x15454950, 0x15255154, 0x15855d57,
	0x15f56d5c, 0x15e57960, 0x16658d63, 0x16a5a169,
	0x16d5b56c, 0x1755dd76, 0x17a5f17c, 0x18661584,
	0x18f63d8f, 0x19465596, 0x19d681a3, 0x1ae6a5ad,
	0x1ba6ddb7, 0x1c7719c7, 0x1d3745cf, 0x1dc77dde,
	0x1ea7a5e9, 0x1f57ddf9, 0x1fe80200, 0x21082a09,
	0x21386219, 0x22387a21, 0x23289e33, 0x2378ce36,
	0x25190e49, 0x22288a21, 0x21285214, 0x205819f9,
	0x1f27b5ed, 0x1e1785e0, 0x1df76dd7, 0x1d0735d5,
	0x1cc709c8, 0x1c26f9bc, 0x1b56d5b1, 0x1b26a5a6,
	0x1a867da1, 0x1986699a, 0x19963d8b, 0x18962188,
	0x1825f97e, 0x17a5e178, 0x1765d571, 0x16b5b56a,
	0x16b5a167, 0x16358561, 0x1605755c, 0x15c57159,
	0x15c56157, 0x15855d54, 0x15654d52, 0x15655151,
	0x15554952, 0x1515394e, 0x1515414f, 0x15553d4f,
	0x1525414d, 0x1515454f, 0x15554552, 0x15354d51,
	0x15555152, 0x15a55d56, 0x15d56557, 0x15b5695c,
	0x1615855e, 0x16658d63, 0x16959967, 0x1705b56c,
	0x1745cd73, 0x17a5dd78, 0x1805f97f, 0x18762184,
	0x18f64190, 0x19965d98, 0x1a2681a2, 0x1b16b5ab,
	0x1ba6e5b7, 0x1c670dc5, 0x1ce745d4, 0x1da779df,
	0x1ee7a1ec, 0x1f87d9f9, 0x2017fa03, 0x20c8260d,
	0x21086612, 0x2228721b, 0x22489225, 0x2328da31,
	0x2428fe43, 0x24a9524f, 0x2348c626, 0x22888a1e,
	0x21183e11, 0x1fa7edf7, 0x1f07a9e9, 0x1e3781da,
	0x1db761d0, 0x1d6735cd, 0x1c7721c6, 0x1c16e9bc,
	0x1b66d1af, 0x1ac6b9ac, 0x1aa681a4, 0x19f67196,
	0x1946518d, 0x18e62d87, 0x18460982, 0x17e5f178,
	0x1775d576, 0x1715c16f, 0x16c5a568, 0x16959965,
	0x16558d60, 0x16258560, 0x15e5795a, 0x15f56d5a,
	0x15f56956, 0x15d56158, 0x15b56557, 0x15a56154,
	0x15d55d56, 0x15a56556, 0x15b56156, 0x15b56558,
	0x15e57159, 0x16156d59, 0x1615815c, 0x16458561,
	0x16659d61, 0x16b5a167, 0x16e5b96a, 0x1735c973,
	0x1785dd76, 0x17f5f17f, 0x18460d84, 0x18a62d87,
	0x19464990, 0x19e66d99, 0x1a5695aa, 0x1b06bdaf,
	0x1bf6edb9, 0x1cc71dca, 0x1d4755d1, 0x1e0775df,
	0x1ed7a9e5, 0x1fa7d5f5, 0x20580a00, 0x20d8320a,
	0x21884a13, 0x21d86613, 0x22988a23, 0x2348ce34,
	0x23d8da3c, 0x24d92649, 0x2559725a, 0x23f8d235,
	0x22e8aa28, 0x21b86612, 0x207811fc, 0x1f67b9f0,
	0x1f0795e7, 0x1e4771dc, 0x1d7755d0, 0x1d5731cd,
	0x1cc711c3, 0x1c06fdbb, 0x1ba6d1b4, 0x1b16a9a6,
	0x1a868d9f, 0x19f66194, 0x19564990, 0x18e62988,
	0x18960d83, 0x17e5f577, 0x17c5e176, 0x1765c971,
	0x1735b56d, 0x16f5b169, 0x16c59d67, 0x16a59561,
	0x1675915e, 0x1655855f, 0x1635855e, 0x1615795d,
	0x1625715c, 0x15f57959, 0x1635795e, 0x1635795b,
	0x1655855d, 0x1655855f, 0x16659562, 0x16859965,
	0x16f5a967, 0x1725b56d, 0x1755d173, 0x17a5e172,
	0x17d5e979, 0x18260181, 0x1876198b, 0x18e63590,
	0x19765592, 0x1a367199, 0x1ad6a1a4, 0x1b16bdb1,
	0x1bf6f9ba, 0x1c9721c3, 0x1d5749d5, 0x1df781e1,
	0x1ee7ade8, 0x1fa7ddf0, 0x20780a00, 0x21083206,
	0x21485214, 0x2258721d, 0x22988e25, 0x22a8b233,
	0x2398e23f, 0x24391a45, 0x25695e62, 0x26e9a277,
	0x24191247, 0x2378e231, 0x21e87e25, 0x20a84a04,
	0x2037e5f8, 0x1f27d5f3, 0x1e67a5e6, 0x1e6771df,
	0x1d675dd6, 0x1d973dcd, 0x1d072dc0, 0x1c4719be,
	0x1be6e5b4, 0x1b56c9ad, 0x1ac69da2, 0x1a466da0,
	0x19d65992, 0x1946418f, 0x18f62184, 0x18661980,
	0x1855f57d, 0x17a5d977, 0x17d5d573, 0x1795c96e,
	0x1735bd6b, 0x16d5b96b, 0x16d5ad69, 0x16f59d69,
	0x16c5a965, 0x16d5a165, 0x16e59d66, 0x16c5a169,
	0x16c5ad69, 0x1695ad6c, 0x1735b96b, 0x1765c96f,
	0x1775c173, 0x1785d974, 0x17a5e975, 0x17d6017d,
	0x18760d7e, 0x18b62187, 0x1906318f, 0x19b66594,
	0x19c6759c, 0x1a4691a6, 0x1b16bdb1, 0x1ba6e9b6,
	0x1c8711c6, 0x1d1739d2, 0x1dd769da, 0x1eb79de0,
	0x1f47cded, 0x2037f1fc, 0x20981208, 0x21084a13,
	0x21a87e1b, 0x22e8a223, 0x22a8aa2f, 0x2378ea36,
	0x23e91238, 0x23d9163a, 0x24f93655, 0x26299a63,
	0x2779c674,

};


/* //100%
static UINT32 ecs_tab[4225] =
{
	0x3ffffff3, 0x3fffffe9, 0x3d2f97dc, 0x3d8f33cc, 0x3c8edbae, 0x3adea3a0,
	0x3a8e1397, 0x391dc75e, 0x36cd634b, 0x334cc732, 0x325c3708, 0x2f8bd2fc,
	0x2e1b56e2, 0x2ceb22c3, 0x2b3abab3, 0x29da7ea2, 0x291a167a, 0x2739ca6a,
	0x26997a60, 0x2529465a, 0x24991a42, 0x2368ea48, 0x23b8ae2f, 0x22c88221,
	0x21d8721b, 0x21686a0c, 0x21984a0c, 0x21583a03, 0x20f81e01, 0x2118060a,
	0x20781608, 0x21180602, 0x21582201, 0x21583e0a, 0x22084212, 0x21886610,
	0x2228a21c, 0x22d8922c, 0x23b8b636, 0x23d90a40, 0x25694249, 0x25496e6a,
	0x27798e61, 0x289a227a, 0x2a3a2a9a, 0x2a3a96aa, 0x2d3b1aba, 0x2cfb66df,
	0x302be2e8, 0x336c5b10, 0x348c8b3f, 0x354d6f72, 0x381df372, 0x3c5e7f94,
	0x3def57df, 0x3f9fa7e5, 0x3fffffff, 0x3fffffff, 0x3fffffff, 0x3fffffff,
	0x3fffffff, 0x3fffffff, 0x3fffffff, 0x3fffffff, 0x3fffffff, 0x3ffff7ff,
	0x3fff8be2, 0x3ceedbc3, 0x3b0ebbb1, 0x3aae7f81, 0x399e0792, 0x389d976d,
	0x356d7752, 0x344ceb41, 0x31ac3f04, 0x30abdaf3, 0x2e2b72e1, 0x2cab0ed3,
	0x2b3aa6b2, 0x2a0a6a97, 0x286a0a84, 0x2729a66e, 0x26297a5b, 0x25092e4f,
	0x24a8ee42, 0x2348ca2b, 0x22a8821f, 0x21e8661a, 0x21583a13, 0x21783209,
	0x1fc7f5fe, 0x2067fdfa, 0x1fd7ddf3, 0x1f97ddf8, 0x1ff7e5f4, 0x1fd7c9f1,
	0x1fa7b9ee, 0x1fe7e1ee, 0x2057de01, 0x2067ea04, 0x20b81e05, 0x20e83a09,
	0x21c84e13, 0x22586624, 0x22c89e2e, 0x23e8ce33, 0x24b8fa45, 0x24b9424c,
	0x25f9ae61, 0x2869d67e, 0x29aa3e9c, 0x2afabaad, 0x2bbb0eb2, 0x2d4b56d3,
	0x2f3bbaef, 0x323c6b1c, 0x359cdb29, 0x359d6359, 0x384e3b81, 0x3b0ebba0,
	0x3fff57c0, 0x3f5fdbdc, 0x3fffffff, 0x3fffffff, 0x3fffffff, 0x3fffffff,
	0x3fffffff, 0x3fffffff, 0x3fffffff, 0x3fffffff, 0x3fffbfd9, 0x3cef1bc6,
	0x39cebba2, 0x39ce1b94, 0x37ce135e, 0x36cd8357, 0x34fd073f, 0x32dc9329,
	0x30ec0706, 0x2edba6f3, 0x2dbb36d1, 0x2c2ae6b6, 0x2a6a7ea5, 0x294a1690,
	0x2779ca6a, 0x26596261, 0x25b92a49, 0x2418e23a, 0x22f8ae31, 0x21f8821e,
	0x21384217, 0x20a82a0c, 0x2007e5fb, 0x1ff7c9f3, 0x1f27b9eb, 0x1f07a9e5,
	0x1e87a1e7, 0x1ec789db, 0x1e378dd5, 0x1db775e0, 0x1dc765d2, 0x1d4769d1,
	0x1e5771d6, 0x1e577de2, 0x1e578de1, 0x1eb79de7, 0x1ec7bdef, 0x1f57f1f4,
	0x2027f9fb, 0x20b82e0c, 0x21a84e0a, 0x22b87e1a, 0x23c8be2e, 0x24890233,
	0x25294a4a, 0x25f98668, 0x273a0672, 0x297a3284, 0x2b2ab2a5, 0x2d4b26c7,
	0x2eab66e4, 0x314bf2f8, 0x329c9325, 0x352d2354, 0x367d9f74, 0x397e3b97,
	0x3ceea3b4, 0x3d8f63df, 0x3ffff7e5, 0x3fffffff, 0x3fffffff, 0x3fffffff,
	0x3fffffff, 0x3fffffff, 0x3fffffff, 0x3cbedbcf, 0x3c5ebba8, 0x3a5e4797,
	0x37eddf7c, 0x36fd9f60, 0x352d2338, 0x32bcaf2d, 0x318c46fe, 0x2e6bcaf8,
	0x2e2b2ec7, 0x2baabab5, 0x2a6a8e90, 0x291a2289, 0x2779b273, 0x26d97e5a,
	0x24891a4b, 0x2418e232, 0x22a89623, 0x21b8521e, 0x20f81e0a, 0x1fd80a01,
	0x1fd7b9f2, 0x1ec7ade6, 0x1e6791e4, 0x1e5771dd, 0x1e5765d6, 0x1da73dd7,
	0x1cd735cb, 0x1d1725cd, 0x1cc721cb, 0x1cf719cb, 0x1d3721cb, 0x1d172dca,
	0x1cf739d5, 0x1de745d6, 0x1da76dd2, 0x1e078de2, 0x1e979ddb, 0x1f27b1ed,
	0x1f57e5f4, 0x2007fdfd, 0x21182616, 0x21b87213, 0x22a89e2e, 0x23f8e23c,
	0x25192e4a, 0x2659a264, 0x27b9ee7e, 0x291a2a97, 0x2b0aa2a5, 0x2cab0eca,
	0x2ddb7ad8, 0x306bdafe, 0x327c7b16, 0x359cf354, 0x362dbf7a, 0x3a5e47a2,
	0x3b0f1bb4, 0x3d2f73dc, 0x3e8fbff3, 0x3fffffff, 0x3fffffff, 0x3fffffff,
	0x3fffffff, 0x3fffffff, 0x3b0ed3b7, 0x39ce47a2, 0x371df377, 0x36ad6f57,
	0x344cff4b, 0x336c9318, 0x308c2712, 0x2edbb6f3, 0x2ddb5ecf, 0x2bbab2ac,
	0x2a0a76a2, 0x285a2a71, 0x26e9ba6e, 0x25d94e61, 0x2529123a, 0x2308be29,
	0x21f89225, 0x21985215, 0x21080e02, 0x2007ddf2, 0x1f07c9e4, 0x1df781df,
	0x1e2755df, 0x1cd749cd, 0x1cd71dc9, 0x1cc709c4, 0x1c46fdbe, 0x1be6e9b9,
	0x1be6ddb5, 0x1b96e1b6, 0x1b76c9b7, 0x1be6d5b4, 0x1c06e1b1, 0x1b96f1b4,
	0x1c2709c0, 0x1c2701c2, 0x1c8719c8, 0x1d275dcf, 0x1d575dd5, 0x1e277dda,
	0x1f07a1eb, 0x2017b9f8, 0x2087fe01, 0x20f83210, 0x22987222, 0x2368ce2a,
	0x24e91e4b, 0x25b93e51, 0x27f9ba6a, 0x294a268a, 0x2a3a7297, 0x2cfaf2c7,
	0x2d8b7adb, 0x2f8bdaf3, 0x314c6325, 0x360d0f3f, 0x362d9784, 0x397e5fa8,
	0x3b3ef3b4, 0x3f9f73c9, 0x3e8f97e9, 0x3f9fffff, 0x3fffffff, 0x3fffffff,
	0x3fffffff, 0x39ce8b9a, 0x379e1b81, 0x36ad6f4d, 0x348d0f4d, 0x336c931c,
	0x310c3710, 0x2f8bc2eb, 0x2d6b56cd, 0x2c8abebd, 0x2aca72a2, 0x294a1286,
	0x2719ba6b, 0x25694e4f, 0x23c90a40, 0x2348be2c, 0x21e86e1a, 0x21182205,
	0x2057f1fe, 0x1f27bdeb, 0x1e6785e6, 0x1df75dd1, 0x1d5749d2, 0x1d0715c7,
	0x1c46f5be, 0x1bd6e9ba, 0x1b16c1aa, 0x1b46b9a7, 0x1ad6adab, 0x1b06b1a9,
	0x1aa699a3, 0x1a668daa, 0x1a7681a2, 0x1a968da9, 0x1ab699a6, 0x1ad6b5a9,
	0x1b06b1b0, 0x1b86d5b2, 0x1c46edb8, 0x1cc705bf, 0x1d3729c9, 0x1db735d5,
	0x1e4785e3, 0x1ec7b9ef, 0x1fa7e5fa, 0x20c80a04, 0x21f86e16, 0x2338aa2e,
	0x23f9063c, 0x25993656, 0x26c9ae6f, 0x291a2283, 0x2afa829f, 0x2ccafec3,
	0x2eab52d8, 0x306bfaf7, 0x32bc6b12, 0x346d1b5b, 0x381dd37f, 0x3a2e47a5,
	0x3c2eafa8, 0x3e8f57c3, 0x3f5fdbfa, 0x3fffe7ff, 0x3fffffff, 0x3fffffff,
	0x389df374, 0x36cdab72, 0x352d3f3f, 0x334c9327, 0x310c0718, 0x2f5beaf8,
	0x2ddb66d8, 0x2c5afeb8, 0x2a7a829e, 0x2919fa7a, 0x2699926a, 0x25e94e4f,
	0x23e9163b, 0x2348b625, 0x22086e17, 0x20683203, 0x1fe7f1f1, 0x1ef799ea,
	0x1e0771dd, 0x1d973dd4, 0x1cf721c9, 0x1c46e9bd, 0x1bd6d1be, 0x1b36b5b2,
	0x1a96a9a8, 0x1a46899d, 0x19e66da0, 0x19b6599c, 0x19b66d99, 0x19964596,
	0x19364191, 0x19c65d8f, 0x19e64d95, 0x1996619c, 0x1a0665a0, 0x19f67d9c,
	0x1a968da3, 0x1b16a9a5, 0x1b76cdaa, 0x1b96e9b8, 0x1be6fdc3, 0x1d572dcc,
	0x1da759d5, 0x1e678de7, 0x1f07c5ee, 0x20b80dfc, 0x21985216, 0x2288aa25,
	0x23d8f240, 0x25894e4f, 0x2729b676, 0x28ea1695, 0x2afa969f, 0x2bfb12b3,
	0x2dab7ad4, 0x302c02f7, 0x32dc8b29, 0x346d0f52, 0x381df372, 0x3a5ea394,
	0x3c8ed3a5, 0x3d8f57b7, 0x3f2fb3d9, 0x3ffffff6, 0x3fffffff, 0x371dbf89,
	0x377d6360, 0x344cc734, 0x31ec7318, 0x30cc16f8, 0x2efb6ae4, 0x2ccb1ac0,
	0x2ada9aa5, 0x295a5e95, 0x27e9d66d, 0x2699625b, 0x2488f64a, 0x22e8ce2f,
	0x22c85e1c, 0x20b83e10, 0x1ff7f1fc, 0x1ec7b1ef, 0x1e076ddf, 0x1cf73dc9,
	0x1c5711c3, 0x1c06e9b4, 0x1b96c5b7, 0x1ac6a1ac, 0x1aa685a2, 0x19e67d9f,
	0x19a64197, 0x19264592, 0x1906418a, 0x18e6298b, 0x1906258b, 0x1876358a,
	0x18e62986, 0x19062d8a, 0x18d6358e, 0x19564189, 0x19465592, 0x19c6699a,
	0x1a968d9b, 0x1ab699a5, 0x1b06a5b1, 0x1ba6e1b7, 0x1c56edc0, 0x1cc72dd0,
	0x1de761d5, 0x1ed78de3, 0x1f97b9f3, 0x20681604, 0x21d86215, 0x2338a22f,
	0x2488fa4d, 0x25795a59, 0x2779da72, 0x291a4e95, 0x2b0aa2af, 0x2c5b0eca,
	0x302bc2e8, 0x320c1718, 0x34dcf325, 0x360d9f6f, 0x386e5392, 0x3b6ebb9d,
	0x3c8f1bae, 0x3e5fbfd5, 0x3fffe7e9, 0x3fffffff, 0x360dab4d, 0x346d234b,
	0x348c8336, 0x310c0706, 0x2eabaeef, 0x2d6b2ecf, 0x2bfac6b3, 0x2a0a5a90,
	0x2819fa7c, 0x25e97e63, 0x24992244, 0x23f8ba2f, 0x22c87a1a, 0x20f8320e,
	0x1fd7e5f0, 0x1f579de6, 0x1d876ddf, 0x1cf735d2, 0x1bf701c6, 0x1bb6e5b4,
	0x1af6adb0, 0x1a76919f, 0x19e6659d, 0x1926558f, 0x18c63d8b, 0x18961d8a,
	0x18161986, 0x1826017d, 0x1845fd78, 0x17a5f17d, 0x1825f17d, 0x1805ed7d,
	0x17b5e979, 0x17d5f57d, 0x1846017f, 0x18861183, 0x18d6218d, 0x19363989,
	0x19965998, 0x19f66da0, 0x1ae685a5, 0x1af6c5aa, 0x1bb6e1b8, 0x1c772dc3,
	0x1d675dd4, 0x1e578de5, 0x1f17c5ee, 0x206821fd, 0x22885217, 0x2378b633,
	0x24191a40, 0x2659725a, 0x27a9e67a, 0x298a4e8e, 0x2bdaf2ac, 0x2ccb52ca,
	0x2f1bcaef, 0x325c9314, 0x346d0f4d, 0x36fd976a, 0x386e4792, 0x3b6ed3ab,
	0x3d5f03e9, 0x3f5f97e2, 0x3f2ffffd, 0x34fd9748, 0x346d0f34, 0x316c6b10,
	0x2f7bdaf8, 0x2d8b5ed9, 0x2c8adabd, 0x2a3a8aa2, 0x290a2283, 0x26e99e6e,
	0x25292648, 0x2368de33, 0x22a88a27, 0x21983a0c, 0x1ff7f9fd, 0x1f17b9e9,
	0x1de769da, 0x1cb72dca, 0x1c4701bd, 0x1b66cdb3, 0x1af69da8, 0x1a16719c,
	0x19666191, 0x1886358a, 0x18561980, 0x18860d81, 0x1815e97d, 0x17b5e173,
	0x1745d570, 0x17a5c973, 0x1715bd6a, 0x1725b970, 0x1735c571, 0x1725c56f,
	0x1735d56d, 0x17a5d170, 0x17a5d976, 0x17c60179, 0x1876017d, 0x18662588,
	0x1956558b, 0x19e65d99, 0x1a3679a0, 0x1ad6b1aa, 0x1bf6d9b4, 0x1c8705c0,
	0x1d2741d1, 0x1e679de2, 0x1ef7d9f3, 0x202805fe, 0x22586e1e, 0x22b8c231,
	0x24b92a3e, 0x2729926a, 0x282a0283, 0x2a1a52a5, 0x2c8aeac0, 0x2e8b96d9,
	0x304bfaf1, 0x332ca325, 0x360d7754, 0x394df37c, 0x39fe4794, 0x3d8f03c6,
	0x3d2f57c9, 0x3ebfffff, 0x341d4754, 0x327c9327, 0x306beb14, 0x2ebbbae6,
	0x2bfb26be, 0x2b8aa2ad, 0x295a4a8e, 0x2779b277, 0x25295657, 0x23f8f23d,
	0x22989a2e, 0x21585209, 0x1fe801ff, 0x1ef7c5f1, 0x1d7769d9, 0x1cb729d2,
	0x1bc6f5c2, 0x1b76b9ad, 0x1a4685a6, 0x1a36619f, 0x19363d94, 0x18c62986,
	0x18160581, 0x17d5e979, 0x1775cd78, 0x1755bd6c, 0x16e5a96a, 0x16859d67,
	0x16a59162, 0x16458d62, 0x16758568, 0x16659161, 0x16758164, 0x16459165,
	0x16959965, 0x16d5a16c, 0x1755bd70, 0x1785cd72, 0x17e5e97f, 0x1835fd82,
	0x18c62d85, 0x18e65591, 0x19f6819b, 0x1a769daa, 0x1b46ddb5, 0x1c5711c2,
	0x1cf745cd, 0x1e8775e5, 0x1f87d1f0, 0x20e80e0a, 0x22388a1b, 0x2348d22c,
	0x25e94e47, 0x2699c663, 0x289a3687, 0x2afadaa5, 0x2cfb22c7, 0x2edb9edf,
	0x31ac5b0a, 0x344cff48, 0x362d6368, 0x391de784, 0x3c5e97a0, 0x3d2f0fc9,
	0x3f9fe7c9, 0x33dcf334, 0x32bc832b, 0x30abfaf3, 0x2d8b82d1, 0x2baaceb3,
	0x2a7a72a4, 0x2869f687, 0x27299a5e, 0x24993647, 0x2308ba2b, 0x22085e0f,
	0x21682dff, 0x1f87b9fd, 0x1df791dd, 0x1d3751d2, 0x1c5701c6, 0x1b66c9b0,
	0x1ae69da6, 0x1996719b, 0x18d64590, 0x18662186, 0x18260180, 0x1775e576,
	0x1775bd70, 0x1705a96b, 0x16859166, 0x16558d5f, 0x15f5855d, 0x15f5795a,
	0x16156d57, 0x15d5655b, 0x15f56957, 0x16156158, 0x15d56d58, 0x16457563,
	0x16558d61, 0x1675a964, 0x1715b171, 0x16f5d970, 0x17b5d57a, 0x1815fd85,
	0x18b62588, 0x19766194, 0x1a1675a0, 0x1a96b1aa, 0x1ba6d5b3, 0x1c4715c3,
	0x1d9751d5, 0x1e87b9ea, 0x1fa7ddff, 0x21a83e0a, 0x22b8a62f, 0x2489063f,
	0x26197e5b, 0x27f9da7f, 0x29aa369a, 0x2c2ac6ad, 0x2e8b5ed4, 0x308bfb0c,
	0x334caf30, 0x352d3748, 0x38cdb381, 0x3b3e3ba5, 0x3cbf1bb1, 0x3e8f63cf,
	0x323c9b1c, 0x30cc0f0a, 0x2ebb9eeb, 0x2d8b06cd, 0x2a7a8eac, 0x295a2695,
	0x2799d27a, 0x25094a56, 0x2448ee3b, 0x22889228, 0x20c83e0f, 0x1ff7e1fa,
	0x1ec78de3, 0x1d774dd2, 0x1c8715c4, 0x1bc6c9b4, 0x1a669da3, 0x1a06599c,
	0x1936358d, 0x18760d89, 0x17f5f57e, 0x1785d575, 0x16a5a970, 0x16a59d61,
	0x1625895f, 0x15f55d5c, 0x15955d5c, 0x15755554, 0x15554951, 0x15052953,
	0x15153d51, 0x1525454d, 0x15553d50, 0x15454553, 0x15755555, 0x15955d57,
	0x15d56d58, 0x16058960, 0x16959d64, 0x16e5bd6b, 0x1765d97a, 0x17c5fd80,
	0x18a61986, 0x19464594, 0x19c6799d, 0x1ac6c1a9, 0x1b96f5bf, 0x1c8719ca,
	0x1db765d7, 0x1ed7adea, 0x2067f603, 0x21e86e1a, 0x22c89e2c, 0x2499164a,
	0x25e98663, 0x286a0680, 0x2ada82a5, 0x2c2b1ad9, 0x2eab7ae8, 0x314c370e,
	0x346d233f, 0x371d976a, 0x397e4784, 0x3ade7fa5, 0x3e8f33bd, 0x31ac9b0e,
	0x300c02fe, 0x2d8b4ad9, 0x2bdae6ba, 0x29da5a92, 0x2769ea73, 0x26297e58,
	0x23f91a40, 0x2368aa28, 0x21c84a12, 0x2067e5fe, 0x1f979de6, 0x1e076dd3,
	0x1cd731cb, 0x1b76ddb9, 0x1ad68da9, 0x1a266198, 0x1966458d, 0x18960186,
	0x17b5f17a, 0x16e5c574, 0x16d5a16d, 0x16658562, 0x1595655d, 0x15356156,
	0x1545494e, 0x1505354f, 0x14d5214a, 0x14e51947, 0x14951d46, 0x14650d43,
	0x14751945, 0x14751943, 0x14c5194d, 0x14b52148, 0x14e52d4f, 0x15154d4e,
	0x1565555a, 0x1605695b, 0x1645855f, 0x16759967, 0x1755c570, 0x17f5f578,
	0x18661986, 0x19263990, 0x1a667d9d, 0x1b56adac, 0x1c66e1bc, 0x1ca735ce,
	0x1ea775e0, 0x1f77b9eb, 0x20b81209, 0x21f88a1f, 0x2398e238, 0x25494a4f,
	0x2709a272, 0x290a528a, 0x2b7aaea4, 0x2e4b5ed4, 0x30ebfb00, 0x332c8329,
	0x359d634b, 0x37ede77a, 0x39fe8b94, 0x3c5ef3c3, 0x310c5af8, 0x2f1bcaed,
	0x2d1b36c3, 0x2afa8aa4, 0x28aa2a95, 0x26e9a66f, 0x25994256, 0x2348de31,
	0x2298861b, 0x20581e0b, 0x1f47b1eb, 0x1e0781e1, 0x1d2731d7, 0x1c06f1bd,
	0x1af6b9ae, 0x1a066599, 0x19765598, 0x18561185, 0x17c5e178, 0x1745c973,
	0x16a5996a, 0x16458d63, 0x15d5615b, 0x15854550, 0x15252d50, 0x14e51d46,
	0x14750946, 0x1425153d, 0x13f4f93e, 0x1444f13c, 0x13e4f13f, 0x13a4f13b,
	0x13d4f140, 0x13f4fd3c, 0x14450941, 0x14351944, 0x14e51548, 0x14e5254e,
	0x1515494f, 0x15c55d58, 0x16257560, 0x16c5a168, 0x1745c96d, 0x17c5e57a,
	0x18a61187, 0x19364d8f, 0x1a66899f, 0x1b86b9b2, 0x1bf701c4, 0x1d3741d6,
	0x1ea779e4, 0x1fe7e9f6, 0x21684a13, 0x23587a27, 0x24991642, 0x27098a63,
	0x2909e687, 0x2a6a82a2, 0x2bfb12cf, 0x2f1b9eea, 0x31ac4f0a, 0x32bceb34,
	0x36fd9765, 0x397e077a, 0x3b9e9787, 0x310c2f08, 0x2e4b8af1, 0x2d1adeb5,
	0x2a7a8ea1, 0x2799f67c, 0x26598664, 0x24d91e4c, 0x2368b634, 0x21e8561b,
	0x20680e00, 0x1ef799ea, 0x1da745d7, 0x1ca715ca, 0x1b56d1b2, 0x1ab695a3,
	0x19b65d91, 0x18b62d8d, 0x1875f181, 0x17a5d174, 0x1715b56c, 0x1645855f,
	0x15a5655d, 0x1555494f, 0x14b53149, 0x14852145, 0x14250941, 0x13f4f53f,
	0x13d4e538, 0x13b4dd3b, 0x13c4d938, 0x1374d538, 0x1394d937, 0x1374e138,
	0x13b4dd37, 0x13b4e53b, 0x1425013c, 0x14250943, 0x14751d42, 0x14b5254b,
	0x15653d53, 0x15d56d5b, 0x16458561, 0x16e5a969, 0x1725d573, 0x17e60583,
	0x19263591, 0x19765998, 0x1ab6a1ab, 0x1c36ddb7, 0x1c4719cd, 0x1da771dd,
	0x1ef7c9eb, 0x20f81a09, 0x22388216, 0x2408d63d, 0x2579425d, 0x27b9d27f,
	0x28ca6a9a, 0x2c2afab8, 0x2ddb66e4, 0x2f7c1708, 0x332cff30, 0x360d776a,
	0x389df37c, 0x391e338f, 0x2fabc2ef, 0x2d6b4ad6, 0x2b0a8ab8, 0x2a4a328d,
	0x2739ae7a, 0x25e95a5d, 0x23c8ea3c, 0x22b86621, 0x21182210, 0x1fb7c9f6,
	0x1dc76ddd, 0x1cf729d1, 0x1bd6f1b2, 0x1ab6b1a8, 0x19966599, 0x18f6418d,
	0x18660180, 0x1755d572, 0x16a5ad69, 0x16558564, 0x15c56555, 0x15553d52,
	0x14c52d4d, 0x14650946, 0x13d50141, 0x13c4e939, 0x1354d535, 0x1364d131,
	0x1344c132, 0x1324b92f, 0x1304bd2e, 0x1314bd2d, 0x1304b52c, 0x1334b52e,
	0x1354cd32, 0x1374d536, 0x13c4ed3b, 0x1434f13e, 0x14650d3e, 0x14c5254c,
	0x15053952, 0x15755559, 0x16057d61, 0x1695a16b, 0x1785e178, 0x1836117d,
	0x1906458e, 0x1a266da2, 0x1b76b1ad, 0x1c06e1bf, 0x1d1735d3, 0x1ef78de3,
	0x2007e5f9, 0x21b82e16, 0x23489e32, 0x24591a47, 0x27097e68, 0x28aa0276,
	0x2a9a8ea1, 0x2d8b52cc, 0x300be2f7, 0x32dc3716, 0x356d233b, 0x37edbf68,
	0x39ce1ba0, 0x2f8b72d9, 0x2d3b22d4, 0x2a1aa6a4, 0x290a2289, 0x26d98a6d,
	0x25d9224f, 0x22e8d22d, 0x21886e18, 0x200809fd, 0x1f479ded, 0x1d875dd6,
	0x1c2701bf, 0x1b26c1b5, 0x1a368d9a, 0x19365d91, 0x18461d85, 0x17b5e177,
	0x16e5b970, 0x16758165, 0x15d56d5f, 0x15353950, 0x1505254d, 0x14250942,
	0x13f4f53a, 0x1374dd36, 0x1354c933, 0x1334b92f, 0x1304a92d, 0x12d4a529,
	0x1274a529, 0x12f4a52a, 0x1264a12a, 0x1294a52b, 0x12e4ad2b, 0x12d4b930,
	0x12e4b130, 0x1324bd32, 0x1344d939, 0x13c4ed3b, 0x14450d3f, 0x14c5194a,
	0x14f5394f, 0x15956955, 0x16758d64, 0x1705b56b, 0x1825ed79, 0x18860589,
	0x19965198, 0x1a5689a0, 0x1b36cdad, 0x1c871dc6, 0x1e2751df, 0x1f07b1f3,
	0x1ff80e0a, 0x21f88226, 0x23e8ea3a, 0x2579665a, 0x26e9ea78, 0x29ba529b,
	0x2c5af2b3, 0x2f3b7ae1, 0x30ec2f0a, 0x338d0f34, 0x36fd6357, 0x397e4777,
	0x2d1b6ae2, 0x2b7b0ebb, 0x2a6a82a5, 0x27a9da84, 0x2519665f, 0x23c9123d,
	0x22589a1f, 0x20e83210, 0x1f87d5f8, 0x1e3781de, 0x1d4735cb, 0x1c26e9bd,
	0x1a369dac, 0x19b66d97, 0x19261d89, 0x1806017e, 0x1715c570, 0x1685916b,
	0x15c5695e, 0x15754553, 0x14e5254e, 0x1444fd41, 0x13b4ed3a, 0x1354dd36,
	0x12d4c131, 0x12a4ad2c, 0x12c4a128, 0x12749d2b, 0x12948d24, 0x12548d23,
	0x12448127, 0x12147d26, 0x12248d23, 0x12649525, 0x12849528, 0x12849928,
	0x12e4b12f, 0x1344b535, 0x1354dd36, 0x13b4e93a, 0x14050940, 0x14a51949,
	0x15654d4e, 0x15b5655f, 0x16b59d68, 0x1765c172, 0x1816017b, 0x1906358e,
	0x19a66199, 0x1aa6b1a6, 0x1be6e9c4, 0x1d5731d4, 0x1e2785e5, 0x1f97e5fd,
	0x21884a18, 0x2228b62d, 0x24791a58, 0x26d99a6a, 0x291a2a8e, 0x2b2aaeb6,
	0x2ddb66c3, 0x2f7bcafe, 0x332caf10, 0x34fd4754, 0x397e1b89, 0x2d1b56c7,
	0x2bfac6af, 0x29ea4a95, 0x2799d678, 0x25894a56, 0x23e8e23d, 0x21e88217,
	0x2087fe04, 0x1f47b9e7, 0x1dc75dda, 0x1ca721c4, 0x1b36c5bb, 0x1a3685a5,
	0x19764993, 0x18160983, 0x17d5e17b, 0x16d5a96b, 0x16058162, 0x15e55956,
	0x14d5314c, 0x14551943, 0x13c4f540, 0x1384d53a, 0x1324c132, 0x12e4ad2d,
	0x12b49926, 0x12348922, 0x12548921, 0x1234811f, 0x11e47920, 0x1204711f,
	0x11d47121, 0x1204811f, 0x12048521, 0x12448d23, 0x12549525, 0x12949529,
	0x12d4a52c, 0x1304bd2e, 0x1374d933, 0x13c4f53b, 0x14351149, 0x14c52d4c,
	0x15655957, 0x16457d5c, 0x16c5a96c, 0x17b5d577, 0x18660988, 0x19765594,
	0x1a26819d, 0x1b86c9b6, 0x1c8719c6, 0x1d9771e0, 0x1f97cded, 0x20c825fe,
	0x2288b22a, 0x24890645, 0x26397e63, 0x2779e67f, 0x2b3a8ea2, 0x2ceb3acd,
	0x2f5bcaeb, 0x318c7314, 0x34dd0741, 0x389d975e, 0x2d1b3ad3, 0x2a7aa2b0,
	0x290a128d, 0x26a99a6f, 0x24891a4c, 0x22b8b22e, 0x21d85e16, 0x20580dfe,
	0x1e879dee, 0x1d3751d2, 0x1c06f9bf, 0x1af6adac, 0x19c6719e, 0x18f61d91,
	0x17e5f17b, 0x1785b56d, 0x16459167, 0x15f5615b, 0x15154550, 0x14852145,
	0x1414f93e, 0x13a4e538, 0x1384c530, 0x12c4a12b, 0x12649d23, 0x12548923,
	0x12147d1f, 0x11e4711c, 0x11f4691e, 0x11b46119, 0x11c4651d, 0x11846519,
	0x11a46517, 0x11e47119, 0x1204791c, 0x1204751e, 0x12349121, 0x12549526,
	0x1294b12d, 0x1314c530, 0x13b4d939, 0x13f4f140, 0x14851549, 0x1545394f,
	0x15d56d5d, 0x16759168, 0x1745c174, 0x17e5fd7a, 0x18b6318d, 0x19c6799c,
	0x1ab6adad, 0x1c36f5c6, 0x1de741d2, 0x1ee79df1, 0x2077f1fd, 0x21c86a1c,
	0x23e8ba33, 0x26994e50, 0x2829ca7e, 0x29ea5ea4, 0x2caae6b6, 0x2e6ba6e1,
	0x308c2f0a, 0x334ccf30, 0x36cd8b59, 0x2c5b26ca, 0x2aca96aa, 0x285a1a84,
	0x2729625e, 0x23d9063d, 0x2278b22a, 0x20e84e0c, 0x1f97c5f8, 0x1e2771dd,
	0x1cb741ca, 0x1ba6e1b9, 0x1a9699ac, 0x19864196, 0x18761182, 0x1775d978,
	0x16e5ad69, 0x15f5755f, 0x15855954, 0x14a5294b, 0x14250146, 0x13b4e93d,
	0x12f4cd33, 0x12d4ad2c, 0x12849d27, 0x12248522, 0x11e4711f, 0x11a4611d,
	0x11b45d18, 0x11845119, 0x11845516, 0x11345513, 0x11345116, 0x11744d17,
	0x11a45915, 0x11845d1a, 0x11b46d1d, 0x12047d1d, 0x12148921, 0x12549127,
	0x12e4a52d, 0x1324c530, 0x13a4dd3c, 0x1414f942, 0x14852549, 0x15655551,
	0x15f56d5a, 0x16c5a16d, 0x17e5d979, 0x18a6118c, 0x19864994, 0x1a669da0,
	0x1bb6e9b7, 0x1d2721cf, 0x1e578ddd, 0x1fa7d5f8, 0x21884617, 0x22e8ba3d,
	0x24e92649, 0x26c9ae69, 0x28da2a8b, 0x2b0afaac, 0x2d6b4ad9, 0x302bfb06,
	0x32dc9338, 0x341d5348, 0x2bdaaea7, 0x2a3a4e9a, 0x2759ee8a, 0x25b95e59,
	0x23d8ee3b, 0x22789e1f, 0x20a81604, 0x1f47bdf0, 0x1de75dda, 0x1ca711be,
	0x1b06b9b1, 0x19b681a3, 0x19363993, 0x1806057f, 0x1745bd71, 0x16959565,
	0x15b56d59, 0x15154550, 0x14a51544, 0x13e4f53d, 0x1374d536, 0x1314b52c,
	0x12949926, 0x12348123, 0x1194711f, 0x11b4691b, 0x11645517, 0x11845115,
	0x11544d11, 0x11443d11, 0x11444113, 0x11245112, 0x11343d11, 0x11644514,
	0x11845514, 0x11746519, 0x1194651b, 0x12146d1d, 0x12447d23, 0x1274992b,
	0x12d4a131, 0x1354c531, 0x13e4e93b, 0x14c51145, 0x14f5394b, 0x1565655b,
	0x16659d64, 0x1745cd73, 0x1825fd82, 0x19763d8e, 0x1a66859d, 0x1b76c1b5,
	0x1cc709c2, 0x1e2769dd, 0x1f77bdee, 0x20c82e13, 0x22588e26, 0x24491e3a,
	0x26498a63, 0x294a1a8d, 0x2baaa2ac, 0x2dab5ebe, 0x30ebdafc, 0x341c9b27,
	0x352d2b4f, 0x2a4ab2a5, 0x28ea7292, 0x2769c273, 0x25b96251, 0x23e8ce37,
	0x22088621, 0x20680dfd, 0x1ef799eb, 0x1d974dd3, 0x1ca711be, 0x1b069dad,
	0x19f6719a, 0x18b6318b, 0x17b5f97c, 0x1705b56e, 0x16458d5f, 0x15355d56,
	0x14d5354d, 0x14850945, 0x1394e53f, 0x1334c132, 0x12d4a52b, 0x12849126,
	0x12047d1c, 0x11b47119, 0x11a46118, 0x11544d16, 0x11444511, 0x11344111,
	0x11144511, 0x10f43510, 0x1114350f, 0x1124350e, 0x10f4490f, 0x11344112,
	0x11445516, 0x11746118, 0x11c4651c, 0x11c48120, 0x12448d24, 0x1274a528,
	0x1354bd30, 0x1394dd39, 0x14450943, 0x14d52d4b, 0x15955157, 0x16458162,
	0x1735bd73, 0x1835f581, 0x18c6258a, 0x1a06699c, 0x1ac6b5b0, 0x1ca701c0,
	0x1d8751df, 0x1f17b1f9, 0x20b80e0a, 0x21f8a628, 0x2498f63a, 0x26197651,
	0x2839e283, 0x2a6a8aa4, 0x2cfb42cf, 0x2febf2ea, 0x31ecaf29, 0x346ce338,
	0x2c2a96ad, 0x291a3e8b, 0x26a9a26e, 0x25293e48, 0x2368b232, 0x21884a14,
	0x1fa7f9f6, 0x1ec795e9, 0x1d373dd9, 0x1c76e9be, 0x1a9695af, 0x19966198,
	0x18960d87, 0x17a5e180, 0x16c5ad6f, 0x1605755e, 0x15154951, 0x14b52147,
	0x1404f940, 0x1394d535, 0x12e4b92c, 0x1274a126, 0x12048d22, 0x11f46518,
	0x11746119, 0x11545917, 0x11044910, 0x10f43913, 0x10f4310b, 0x10f42d10,
	0x10b4310b, 0x11042d0d, 0x10b4390d, 0x1104350f, 0x10e4410e, 0x11244910,
	0x11344d14, 0x1154591b, 0x11a4651d, 0x12247d22, 0x12349124, 0x12d4b12d,
	0x1344d533, 0x1404f142, 0x14651548, 0x15255152, 0x1635795e, 0x16c5a968,
	0x17e5d97b, 0x18761d86, 0x19d64d95, 0x1a769da3, 0x1c06f5c3, 0x1cf745d0,
	0x1ec78de3, 0x1fd805fb, 0x22385e1d, 0x23b8ca35, 0x25094e57, 0x27e9ea7e,
	0x2a7a82a7, 0x2dab06c7, 0x2f3b8ae4, 0x325c470e, 0x360cc738, 0x29daa29b,
	0x28aa3e7f, 0x25c9b661, 0x23f90a48, 0x22d89628, 0x21383610, 0x1fb7e9f4,
	0x1e3781e0, 0x1ce735cb, 0x1b66c9b4, 0x1a3691a0, 0x19564996, 0x18561186,
	0x1765c576, 0x16959166, 0x15e56d59, 0x1545454e, 0x14651945, 0x13f4f138,
	0x1324d134, 0x12f4b128, 0x12349527, 0x11f4751d, 0x11b46519, 0x11645118,
	0x11444911, 0x1114350f, 0x10d42d0d, 0x1084290d, 0x10b4250c, 0x10f42109,
	0x10a4290a, 0x10e42109, 0x10b4290b, 0x10c42d0b, 0x10e43510, 0x11444913,
	0x11345515, 0x11a46517, 0x11e47921, 0x12448d26, 0x12b4a527, 0x1344c531,
	0x1404ed3c, 0x14751544, 0x15354150, 0x15a5695d, 0x16b59d6c, 0x1755d575,
	0x1846117e, 0x1966618e, 0x1a6691a6, 0x1bd6e5bc, 0x1cc735d1, 0x1ec78de0,
	0x2007f5fc, 0x21e85a16, 0x22b8ea31, 0x25b94e50, 0x27f9d273, 0x2a0a729a,
	0x2c7adec3, 0x2e4b96e2, 0x318c0312, 0x33dcbf29, 0x291a66a5, 0x27ea1a7f,
	0x26297263, 0x24590649, 0x22c87a32, 0x20584206, 0x1f07d1f6, 0x1df771d9,
	0x1c6719c8, 0x1b36ddb3, 0x1a7679a0, 0x1926498e, 0x1835f57d, 0x1705c575,
	0x16958d65, 0x15855957, 0x14b5314d, 0x14350d43, 0x13a4d936, 0x1324c130,
	0x12b4952c, 0x12448522, 0x11b4751d, 0x11b4591a, 0x11245113, 0x10d43911,
	0x10e4290d, 0x10a4290a, 0x10942509, 0x10641907, 0x1074210a, 0x1074210b,
	0x10841d09, 0x10b42906, 0x10c4210b, 0x10e4350b, 0x1124390f, 0x11444d12,
	0x11545916, 0x11d4651b, 0x12348121, 0x1294a529, 0x1314b530, 0x1374d936,
	0x1435093f, 0x15152d4d, 0x15755d57, 0x16d59164, 0x1735c170, 0x18360185,
	0x18c63d8e, 0x1a668da5, 0x1bc6cdb2, 0x1c8735c6, 0x1e7785da, 0x1fc7ddff,
	0x20f8560f, 0x2298a628, 0x24f92a48, 0x2729a26b, 0x295a4292, 0x2bdafac0,
	0x2dab52df, 0x302c3f0e, 0x332c9b21, 0x290a4295, 0x279a0689, 0x26596259,
	0x23d8e63a, 0x22088e22, 0x20a81608, 0x1ef7c5ee, 0x1df759d7, 0x1c7709c0,
	0x1bd6b9ac, 0x19e6799a, 0x19462192, 0x17f5f582, 0x16d5b574, 0x16658d64,
	0x15555956, 0x14a5314a, 0x1434f942, 0x13a4e537, 0x1334b530, 0x12a4a527,
	0x12348123, 0x11a46517, 0x11945117, 0x11344111, 0x1114390c, 0x10d42907,
	0x10742109, 0x10a41907, 0x10741508, 0x10641506, 0x10240d06, 0x10642105,
	0x10441d0a, 0x10842109, 0x10a4210b, 0x10f4310e, 0x11244910, 0x11545916,
	0x11c45d1a, 0x12247d1f, 0x12b48d22, 0x12e4b12d, 0x13a4dd35, 0x13f4fd41,
	0x14c51945, 0x15a55559, 0x16458963, 0x1745c173, 0x17c6017e, 0x1956358e,
	0x1a06799f, 0x1b56d1b2, 0x1cb71dcb, 0x1dc771de, 0x1fa7d9fb, 0x20e83a12,
	0x23088628, 0x24b92e49, 0x26d9a668, 0x29aa5a8b, 0x2bdadeb3, 0x2e4b8ad3,
	0x306c0306, 0x34acf32b, 0x29aa3690, 0x2819ee83, 0x25995654, 0x23f8ee38,
	0x2268621a, 0x20680e09, 0x1ed7a9f0, 0x1dc75dd6, 0x1c570dc3, 0x1ae6a9ad,
	0x199671a1, 0x18e63189, 0x17b5e57c, 0x16f5a96a, 0x15d57965, 0x15454555,
	0x14852149, 0x13c5013e, 0x13b4d537, 0x1304ad2a, 0x12649127, 0x11e47522,
	0x1194611b, 0x11744914, 0x11043910, 0x1104310f, 0x10a42509, 0x10941d0b,
	0x10741507, 0x10740d07, 0x10441505, 0x10941506, 0x10441907, 0x10841507,
	0x10841d08, 0x10b4210c, 0x10d4310b, 0x11143910, 0x11044916, 0x11846519,
	0x11d4691f, 0x12348922, 0x12b4a52c, 0x1314cd38, 0x13f4f93e, 0x1465194a,
	0x15255153, 0x16258562, 0x1705b170, 0x17e5f57c, 0x18e6318f, 0x1a36799f,
	0x1b36c1b3, 0x1c4711c9, 0x1da769da, 0x1f47d5f0, 0x20c82e15, 0x22789a26,
	0x2529164a, 0x26a98a60, 0x290a2a89, 0x2b7ac6c0, 0x2d4b82d4, 0x304c0f02,
	0x32bc9b1c, 0x287a3695, 0x2759e271, 0x25195e5e, 0x23b8d631, 0x22087e1b,
	0x1fe81a13, 0x1ed7b1ea, 0x1da745d2, 0x1c76fdc0, 0x1ac6a1ac, 0x19865d99,
	0x18862983, 0x1745e97b, 0x16d5b16a, 0x15f56561, 0x15054553, 0x14951d41,
	0x1404e13e, 0x1324cd35, 0x12d49d28, 0x12449124, 0x11d46d1d, 0x11845d18,
	0x11344912, 0x10e4310f, 0x10a4350c, 0x10841908, 0x1054110a, 0x10440d04,
	0x10640d01, 0x10640503, 0x10340d04, 0x10340903, 0x10440d03, 0x10841909,
	0x10641d07, 0x10c42509, 0x10e4390e, 0x11144514, 0x11645517, 0x11d46d1d,
	0x12849525, 0x12a4b12b, 0x1364c132, 0x1394ed41, 0x1465194c, 0x15554955,
	0x15c5795e, 0x16b5ad6b, 0x1795e580, 0x18c61d8a, 0x19c66da0, 0x1ae6b1ae,
	0x1cb6fdc2, 0x1d7761d8, 0x1ee7c1f1, 0x20c82e09, 0x22e88a23, 0x2478f638,
	0x26a99a6a, 0x28da2286, 0x2b0aaead, 0x2d4b56cc, 0x30cbdaf5, 0x327c8b25,
	0x291a069f, 0x27e9d677, 0x25895a54, 0x2338d233, 0x21e85621, 0x20a80602,
	0x1ed7a1e3, 0x1d573dcc, 0x1bb6f5b9, 0x1ad6b1a9, 0x19965d98, 0x18b6158b,
	0x17a5dd79, 0x16d5b169, 0x1605755f, 0x15054157, 0x14c51547, 0x13d4ed3f,
	0x1374bd34, 0x12b4b12b, 0x12048923, 0x11f4691d, 0x11645919, 0x11244512,
	0x10d4350c, 0x10741d10, 0x10841d09, 0x10841907, 0x10340903, 0x10540503,
	0x10340502, 0x10340503, 0x10340504, 0x10640d05, 0x10541507, 0x10742105,
	0x10b4310a, 0x10f43511, 0x11544910, 0x11645d15, 0x11a4751e, 0x12448922,
	0x12b4a129, 0x12f4cd32, 0x13f4e93d, 0x14751547, 0x15254552, 0x16257961,
	0x1705a16c, 0x17b5e57b, 0x18e6198f, 0x19e6719d, 0x1b06c1b2, 0x1c3701be,
	0x1d975ddd, 0x1f37c9ee, 0x20183613, 0x22b89a28, 0x23f91243, 0x26398e65,
	0x28da1a92, 0x2b2adac3, 0x2e1b3ae8, 0x2f8bdafc, 0x323c8330, 0x29aa5290,
	0x27b9d277, 0x25793e4a, 0x2448ce36, 0x21a8561f, 0x20080e04, 0x1ec795e0,
	0x1d1745d1, 0x1bd6f5bc, 0x1a76a1a5, 0x19c6659b, 0x1876118b, 0x1755e17a,
	0x16d59d6d, 0x15e5755c, 0x15153d50, 0x1465114b, 0x13e4ed3a, 0x1324c534,
	0x12c4a528, 0x12249124, 0x11d4791c, 0x11845d16, 0x11044113, 0x10c4390e,
	0x10b4250c, 0x10641507, 0x10640d06, 0x10640905, 0x10340100, 0x10340100,
	0x10040503, 0x10741103, 0x10440d03, 0x10841506, 0x10742106, 0x10a4290c,
	0x1114390f, 0x11344511, 0x11745916, 0x11d4651d, 0x12648925, 0x1284a92d,
	0x1324c934, 0x13d4f13e, 0x14951549, 0x15054150, 0x15d57162, 0x17059d6f,
	0x1795e97f, 0x18b62d8a, 0x19c6659c, 0x1ab6b5ad, 0x1be701c1, 0x1d974dd6,
	0x1f07c1f1, 0x20f82e0f, 0x22d8922b, 0x2418f244, 0x26498e68, 0x27ea3e97,
	0x2adac6af, 0x2d8b52e4, 0x304c06fe, 0x323c9325, 0x27aa1a90, 0x2719b280,
	0x24d92e56, 0x22f8ca34, 0x20f85a16, 0x1fd7f1f9, 0x1ed799e3, 0x1d6745ce,
	0x1b86e9bc, 0x1ab6a1a7, 0x19665599, 0x18760583, 0x1735d576, 0x16b59d69,
	0x15f57161, 0x15353951, 0x14551144, 0x13f4f13b, 0x1304c531, 0x12c4a928,
	0x12347d24, 0x1184711c, 0x11445917, 0x11244112, 0x10e4310e, 0x10b4250c,
	0x10941d09, 0x10641503, 0x10540504, 0x10240902, 0x10040500, 0x10440102,
	0x10340501, 0x10340d06, 0x10641906, 0x10941d08, 0x1094210b, 0x10e4310e,
	0x11143d15, 0x1154511a, 0x11f46d1d, 0x12148d25, 0x12c4a92b, 0x1334c135,
	0x1394ed3d, 0x14551946, 0x1525414f, 0x16257562, 0x16b5b16f, 0x1785e578,
	0x18b6218a, 0x1a1671a2, 0x1ae6c1ab, 0x1c8705c0, 0x1d675ddd, 0x1eb7d5f0,
	0x20d83217, 0x22a88e2a, 0x25190645, 0x25b98e68, 0x282a2a89, 0x2ada8eaf,
	0x2d4b5ed9, 0x302c0300, 0x31acb718, 0x27ea367f, 0x26c9d272, 0x25b93a4f,
	0x2448b62f, 0x21884a13, 0x20080a04, 0x1eb789e5, 0x1cc741cf, 0x1c16ddc0,
	0x19f6b1ad, 0x19465997, 0x18861986, 0x1785d172, 0x16a59166, 0x15b5755b,
	0x14f54551, 0x14751145, 0x1364e13f, 0x1314c530, 0x1294b12a, 0x12448d22,
	0x11a4751d, 0x11845916, 0x11344514, 0x10d43511, 0x10f4290b, 0x10741506,
	0x10641109, 0x10340d07, 0x10240503, 0x10140505, 0x10340501, 0x10441106,
	0x10040d05, 0x10441508, 0x10642507, 0x10742d0f, 0x10b43511, 0x11144d14,
	0x11745516, 0x11b4751c, 0x12148d21, 0x12c4ad2d, 0x1354cd30, 0x13b4e93e,
	0x1485254a, 0x15455956, 0x16057160, 0x16e5a96f, 0x17d5dd7d, 0x1846218d,
	0x19f6819f, 0x1b06b9b1, 0x1c4709cd, 0x1df765d9, 0x1ef7d1f7, 0x20f82a15,
	0x22a89627, 0x24d91654, 0x26e96e63, 0x294a1a86, 0x2b5ad2b3, 0x2dbb5ed1,
	0x2f8bfafc, 0x32bca334, 0x286a0a86, 0x2639a673, 0x25693657, 0x2388ca2e,
	0x21985e1a, 0x2037fa01, 0x1ea78de2, 0x1d672dce, 0x1bc6e9bc, 0x1a5695ab,
	0x19465596, 0x18c60983, 0x1795dd76, 0x1695a96b, 0x15d56d5a, 0x14e54553,
	0x14452145, 0x13b4ed3b, 0x1364cd32, 0x12b4b128, 0x12549123, 0x11c4711c,
	0x11645919, 0x11144d14, 0x1124350f, 0x10d4290c, 0x10b41909, 0x10642108,
	0x10340d03, 0x10440907, 0x10340d02, 0x10440902, 0x10340d05, 0x10541103,
	0x10641507, 0x10a41d08, 0x10a4290b, 0x10d4350f, 0x11444d15, 0x11645d1b,
	0x11c4751b, 0x12448124, 0x12b4a52c, 0x1374d136, 0x13e4f53e, 0x14852149,
	0x15355155, 0x16257160, 0x16f5b96c, 0x17c5ed7f, 0x18a6258f, 0x19c6719b,
	0x1b36b9b6, 0x1c870dc6, 0x1d9765dd, 0x1f47c9f1, 0x20b83a09, 0x22c8a22c,
	0x24d8fe49, 0x26198665, 0x283a168b, 0x2bfad2b5, 0x2d1b72e1, 0x2f5c2f00,
	0x332c8329, 0x28a9f68b, 0x26a9b665, 0x25895a57, 0x2348c233, 0x2198461b,
	0x2037e1fb, 0x1e7789e0, 0x1d373dcb, 0x1bb6fdbd, 0x1ab695a7, 0x19a65996,
	0x18860d85, 0x17c5d97a, 0x1695a56c, 0x1605715d, 0x15153d52, 0x1445154a,
	0x1414f13d, 0x1364cd34, 0x1294a52e, 0x12849523, 0x11e4711d, 0x1174591a,
	0x11244d13, 0x10e4390f, 0x10d4290b, 0x10842109, 0x10642107, 0x10340d05,
	0x10440d04, 0x10441503, 0x10640904, 0x10140905, 0x10541102, 0x10842108,
	0x10642508, 0x10b4290f, 0x10f43911, 0x11645113, 0x11646118, 0x11b47d1f,
	0x12649929, 0x1294ad30, 0x1344d536, 0x13d4fd3e, 0x14a5154b, 0x15655157,
	0x1605855f, 0x1725bd70, 0x17c5f17d, 0x18f6318e, 0x1a0675a5, 0x1b76c1b7,
	0x1ca725c9, 0x1e8771df, 0x1f37c9f0, 0x21a8420a, 0x22b89e2d, 0x24e9424b,
	0x2729a263, 0x286a4289, 0x2bfaa6b6, 0x2e8b5edb, 0x30ac0f04, 0x332c9325,
	0x28aa2289, 0x26d9ca7e, 0x24f93e4f, 0x2348de28, 0x21e84616, 0x2007ee02,
	0x1e7799ee, 0x1cc745d2, 0x1b96e9c0, 0x1aa6a5ad, 0x19a6599a, 0x18860d88,
	0x1795dd7d, 0x16b5ad6b, 0x15c5855c, 0x15655155, 0x14a52149, 0x1404ed41,
	0x1364d136, 0x1314a92f, 0x12649d26, 0x11f47923, 0x11746117, 0x11545117,
	0x11143d12, 0x10a4390d, 0x10c42d0a, 0x10841d08, 0x10441909, 0x10741105,
	0x10740d07, 0x10240d05, 0x10640905, 0x10741508, 0x1084210a, 0x10a4290d,
	0x10f4310e, 0x11044112, 0x11245515, 0x11a45d1c, 0x12048926, 0x1244912b,
	0x1304b92f, 0x1354cd37, 0x1424f144, 0x14b52550, 0x15655958, 0x16459168,
	0x1725c574, 0x17e5fd85, 0x19062593, 0x19e6859f, 0x1b06c9b9, 0x1c9711c9,
	0x1df75de0, 0x1f37c1f7, 0x2108461b, 0x2348aa2a, 0x25191e45, 0x2779c26b,
	0x295a5e95, 0x2b5adac0, 0x2d8b66e1, 0x304bfb04, 0x31ecbf1c, 0x286a1a84,
	0x2769b678, 0x25496252, 0x23a8ae2e, 0x21d86a22, 0x2067f606, 0x1ee799e7,
	0x1d2745d2, 0x1c06f1be, 0x1aa6a5aa, 0x19c661a0, 0x18a6218b, 0x17c5e57a,
	0x16c5a96d, 0x15e57563, 0x15655951, 0x1485194d, 0x1424f943, 0x1394d932,
	0x12c4b92d, 0x12849926, 0x12148922, 0x11b4691e, 0x11545517, 0x11044912,
	0x10f43510, 0x10d42d0c, 0x10a42109, 0x10541d0b, 0x1074190b, 0x10641908,
	0x10641909, 0x10641509, 0x10641d0a, 0x10c42d0a, 0x10d4250f, 0x10d4390f,
	0x11144d13, 0x11545919, 0x11a4691e, 0x12348925, 0x12b4a128, 0x1324c12f,
	0x13e4d93e, 0x14850d43, 0x14c53951, 0x15c56157, 0x1665956a, 0x1755d177,
	0x18261180, 0x19764d96, 0x1a5695a8, 0x1b76ddbe, 0x1cb72dd2, 0x1e5771e6,
	0x1f57cdfc, 0x21e85618, 0x2378ca28, 0x25892e57, 0x27e9b272, 0x298a4297,
	0x2baaf2bd, 0x2eab8ae6, 0x30ac0f0e, 0x336cb738, 0x283a1a90, 0x2729ca7c,
	0x24b93a59, 0x2358e636, 0x21a85221, 0x2007fe06, 0x1e67a9ed, 0x1d4759d2,
	0x1bf701bd, 0x1ad6adb0, 0x19e66599, 0x18c63593, 0x17c5f17c, 0x1705ad75,
	0x16458d65, 0x15d55159, 0x15152d4d, 0x1444fd43, 0x1374e138, 0x1324c131,
	0x12b4a12c, 0x12448125, 0x11b4751e, 0x11a45918, 0x11545116, 0x11144111,
	0x10a43514, 0x10b4310f, 0x10a4210b, 0x10a42907, 0x1094190b, 0x10842107,
	0x1084210b, 0x1064250c, 0x1104310b, 0x11143511, 0x11144514, 0x11545d19,
	0x11a4611a, 0x11d4791d, 0x12549d26, 0x12a4b131, 0x12c4d533, 0x13e4f13e,
	0x1475114b, 0x15254557, 0x15e5795a, 0x1655a969, 0x1735dd7c, 0x18660585,
	0x19665599, 0x1a669da4, 0x1c06f1c0, 0x1cb73dcb, 0x1df799ea, 0x1f97f5fe,
	0x21c85e17, 0x2308ce33, 0x24d95e54, 0x26c9c671, 0x295a52a2, 0x2bfadecf,
	0x2e1b8aed, 0x30ac4f1c, 0x33acb748, 0x290a428e, 0x27a9d680, 0x25695a5e,
	0x23e8c23c, 0x21e85e21, 0x1ff7fe04, 0x1ec79dee, 0x1d9759d1, 0x1ba711ba,
	0x1b26b9b4, 0x1a066d98, 0x18f63590, 0x17d5e982, 0x1725c574, 0x16459164,
	0x1595655b, 0x14b5354c, 0x14451142, 0x13a4f13e, 0x1304c933, 0x12a4a52d,
	0x12448d27, 0x11e47920, 0x11b4611a, 0x1154591a, 0x11244110, 0x10f43511,
	0x10c4350e, 0x1084310b, 0x10d4210c, 0x10b42908, 0x10b4290a, 0x10c42d0d,
	0x10e42d0c, 0x10f4390f, 0x10f43d11, 0x11844d15, 0x11845917, 0x1184651c,
	0x12147d25, 0x1254a12a, 0x12d4b12f, 0x1394d13a, 0x1404f545, 0x14852948,
	0x15355553, 0x15c58566, 0x16a5ad6c, 0x1795e97a, 0x1876318d, 0x19a66d9a,
	0x1aa6a1b6, 0x1c26f5c0, 0x1d5755d5, 0x1ec7b5e9, 0x2087fe02, 0x21987e22,
	0x2398e636, 0x25495a64, 0x27b9ea71, 0x2a1a76a5, 0x2c8b1acf, 0x2e1b96e8,
	0x30ac731c, 0x34acaf30, 0x291a5290, 0x2729ee76, 0x25897656, 0x23d8f23f,
	0x22288622, 0x20280e02, 0x1ec7b1f2, 0x1db75dd5, 0x1c4719c6, 0x1b16b5b5,
	0x1a3681a0, 0x18c64595, 0x18360184, 0x1755d579, 0x16959d65, 0x16056d62,
	0x14f53d51, 0x14652149, 0x13e4f53f, 0x1324d536, 0x12e4a933, 0x1224992c,
	0x11f48924, 0x11a46d1e, 0x11a46517, 0x11545918, 0x11044911, 0x11044112,
	0x10d43d11, 0x10e43110, 0x10c42d0f, 0x10e42d0f, 0x10d43111, 0x11043513,
	0x11444114, 0x11245116, 0x11645d17, 0x11b4651b, 0x11f47d22, 0x12449127,
	0x12b49d2f, 0x1314cd32, 0x1394e13e, 0x14450149, 0x14a5394e, 0x15355557,
	0x16458d66, 0x16b5b971, 0x17d5f986, 0x18d63190, 0x1a2679a1, 0x1b56c9b8,
	0x1c970dca, 0x1d6765e0, 0x1f07c1f2, 0x20f83a0f, 0x21e88e1e, 0x23490642,
	0x26498663, 0x27ea0a83, 0x2afa8aad, 0x2c8b4ac8, 0x2eabc2e4, 0x310c731c,
	0x33ace33b, 0x29da7290, 0x287a0a7f, 0x25d98a64, 0x2458de40, 0x22487a28,
	0x2068060e, 0x1ed7c9fd, 0x1de761e1, 0x1ca729c8, 0x1b06c9b6, 0x1a6685a1,
	0x1936498e, 0x1846098a, 0x1775cd76, 0x16a5a56b, 0x1625655f, 0x15455157,
	0x14c52547, 0x13e50143, 0x1364e537, 0x1314c930, 0x12b4a52e, 0x12448d2a,
	0x12347922, 0x11c4711e, 0x11b46519, 0x11444914, 0x11144116, 0x10f44912,
	0x10f43d12, 0x11343511, 0x11243912, 0x10f44113, 0x11244111, 0x11445113,
	0x11745d19, 0x11a4691a, 0x11f4711e, 0x12348524, 0x12a4992b, 0x12f4bd32,
	0x1354d939, 0x1414f93d, 0x14451946, 0x15353d57, 0x1575715c, 0x16b5996a,
	0x1715d177, 0x18361187, 0x19264599, 0x1a268d9e, 0x1b56d9bd, 0x1c6715cd,
	0x1dc775e5, 0x1f77ddef, 0x20e82e13, 0x2288b233, 0x23d9263f, 0x2629ae61,
	0x28ca328a, 0x2acaaeb5, 0x2ccb1adf, 0x2f1bfafc, 0x320c8327, 0x341d1b3f,
	0x2a1a6692, 0x287a0274, 0x26398a56, 0x23e90643, 0x22788e2f, 0x20f83a0c,
	0x1fa7ddf2, 0x1dc781e2, 0x1cc725d2, 0x1b76ddba, 0x1ab69da8, 0x19c64998,
	0x18361d88, 0x17b5e579, 0x16d5b56d, 0x15f58963, 0x15856961, 0x14d5354e,
	0x14150945, 0x13d4e93e, 0x1364d937, 0x12f4b933, 0x1264a52c, 0x12348926,
	0x11f47d1e, 0x11a46d1f, 0x11945919, 0x11846118, 0x11645117, 0x11444916,
	0x11245116, 0x11445114, 0x11345118, 0x11545516, 0x1164591a, 0x11c4651b,
	0x11f4751f, 0x12048127, 0x12b49d29, 0x12c4b130, 0x1314d133, 0x13d4dd37,
	0x14650945, 0x15152d50, 0x15355558, 0x15e58d64, 0x16c5a970, 0x17b5fd7f,
	0x18a6298d, 0x1946619f, 0x1a66b5aa, 0x1b96f9c4, 0x1cf73dd4, 0x1ef789df,
	0x1f87e60e, 0x21685e1a, 0x22f8ae36, 0x24792252, 0x26d9c67e, 0x28ea3e94,
	0x2bbaeabb, 0x2d8b66df, 0x30ac0efc, 0x323caf34, 0x352d5b59, 0x29aa669b,
	0x2919f692, 0x26598e71, 0x24591a4b, 0x22a8ba34, 0x20f84a14, 0x1f97f1f7,
	0x1e878de6, 0x1c972dc9, 0x1be6e1b9, 0x1ae6a9b0, 0x19a66598, 0x18b6318b,
	0x17e5e97e, 0x16d5c573, 0x16658965, 0x15856d5b, 0x15354557, 0x14a5314a,
	0x13d4f541, 0x13b4d939, 0x1314c531, 0x12c4ad2c, 0x12c49d2c, 0x12948926,
	0x12147925, 0x11b4711d, 0x1184611e, 0x11b45d1b, 0x11945d1a, 0x1174591a,
	0x11446119, 0x11745d1a, 0x11a4611b, 0x11b4711b, 0x11b47520, 0x12248922,
	0x12049127, 0x12a4b12d, 0x12e4c131, 0x1354dd3a, 0x1394f13f, 0x1445194a,
	0x14d53151, 0x1585655f, 0x16759967, 0x16e5c977, 0x1805f186, 0x18a63194,
	0x19d6819c, 0x1a66c5b5, 0x1c4709c6, 0x1d5759db, 0x1e57a5eb, 0x2007fe03,
	0x21e85e23, 0x22f8e238, 0x24795a48, 0x2729ea74, 0x29aa769e, 0x2afaeacc,
	0x2e1b96f1, 0x310c270a, 0x32fce332, 0x356d775e, 0x29aaa28e, 0x28da228a,
	0x26c9a261, 0x24a92256, 0x2308b22d, 0x21984e18, 0x1f57f9fe, 0x1e578ded,
	0x1d4735d9, 0x1c06f9c3, 0x1b46adac, 0x19e6719d, 0x18e63998, 0x1835f986,
	0x1715cd77, 0x16c5bd6c, 0x16357961, 0x15855957, 0x1505314d, 0x14452143,
	0x13f4f13e, 0x1394e138, 0x12e4c134, 0x12f4a92d, 0x12549529, 0x12448925,
	0x12348122, 0x11e47920, 0x11b4711f, 0x11e4711d, 0x1194651b, 0x11a4651c,
	0x11e4691d, 0x11e4791e, 0x12047d22, 0x12148121, 0x1284952a, 0x12c4a92d,
	0x1314bd33, 0x1344cd34, 0x13a4f141, 0x14251141, 0x14c5314e, 0x15755d58,
	0x15f57d65, 0x16d5b16f, 0x1785d97a, 0x18661186, 0x18c64997, 0x1a3699a7,
	0x1bc6cdba, 0x1c4715cb, 0x1da75de7, 0x1ee7a9fd, 0x20382a0e, 0x22088e2c,
	0x23f8ea3f, 0x25f97259, 0x27e9ea84, 0x297a72aa, 0x2c7b2ecf, 0x2e8bc2fc,
	0x306c170e, 0x32fc9344, 0x352d7748, 0x2a7a8ac0, 0x28da3e90, 0x2769ae71,
	0x24d94e52, 0x2388d22f, 0x22985e1b, 0x1fa7fe02, 0x1ee7a5f0, 0x1d5759d9,
	0x1c571dc3, 0x1b76b9b7, 0x1a3681a2, 0x1976559a, 0x18361187, 0x17c5e57e,
	0x1705b16d, 0x16059162, 0x1555695b, 0x15054555, 0x14651d4c, 0x14550543,
	0x13a4f53d, 0x1374d13a, 0x1324bd31, 0x12e4a530, 0x12b4a526, 0x12249128,
	0x12248d23, 0x12348924, 0x12048122, 0x11c48124, 0x12148523, 0x11f48126,
	0x12449127, 0x1224952a, 0x12349929, 0x1294a92b, 0x12f4c934, 0x1324cd35,
	0x13b4f13f, 0x14250945, 0x1485214c, 0x15354152, 0x15c56d61, 0x16259d69,
	0x16d5c576, 0x17c5fd80, 0x1886258e, 0x19f66598, 0x1a76b9ad, 0x1bd6f1bd,
	0x1d4731d9, 0x1de79dde, 0x1f07edfd, 0x21085218, 0x2268b227, 0x24690a4c,
	0x2649a26a, 0x289a3e94, 0x2b7ab2ad, 0x2dab3acf, 0x2e8bf2f8, 0x30ec9329,
	0x33acf354, 0x362d536a, 0x2bdad2b0, 0x29aa5e9c, 0x27b9da80, 0x25794659,
	0x23d8ee43, 0x21b87226, 0x20680607, 0x1f27d5f4, 0x1d6779e2, 0x1cc72dca,
	0x1b56ddbd, 0x1a1695a8, 0x19b6659c, 0x18b6318f, 0x17960184, 0x1755cd77,
	0x1675ad6f, 0x16057d64, 0x15155959, 0x15153551, 0x14a51d4b, 0x14150942,
	0x13e4f13d, 0x1364e538, 0x1314c535, 0x12f4b130, 0x12c4a92b, 0x1284952b,
	0x12849d29, 0x12649527, 0x12649129, 0x12249129, 0x1284992a, 0x1264a128,
	0x1274a92d, 0x12d4b132, 0x12f4cd34, 0x1354d939, 0x13b4f13f, 0x13f4f144,
	0x14251948, 0x15154553, 0x1565695b, 0x15f58568, 0x1665c570, 0x1785e57a,
	0x18361189, 0x19264d9a, 0x1a368da5, 0x1af6cdb7, 0x1c2719c7, 0x1d0751da,
	0x1eb795f8, 0x1fd8120b, 0x21a86a25, 0x22c8d237, 0x25d93657, 0x2739c27a,
	0x291a4e9e, 0x2baac6ba, 0x2d6b7ae8, 0x2efc0304, 0x325c9b2d, 0x336d4752,
	0x36cd976f, 0x2baadeb3, 0x294a7ea4, 0x2739da7e, 0x2659765b, 0x24590245,
	0x2298a229, 0x2138461a, 0x1f47c9f2, 0x1e1779e7, 0x1cb735cf, 0x1bd6e9bb,
	0x1b06b5af, 0x19c6759f, 0x18c64194, 0x18460989, 0x1795e17f, 0x16e5b16f,
	0x16459565, 0x15c5695d, 0x15354d57, 0x14b5294f, 0x14351d44, 0x14150541,
	0x13a4e53d, 0x1394d13b, 0x1314c535, 0x12c4bd31, 0x12c4b52c, 0x12b4b52c,
	0x1284a12b, 0x1294a92c, 0x1294ad2f, 0x12a4ad2e, 0x1314b132, 0x12d4bd30,
	0x1314c535, 0x1354cd39, 0x1384ed3e, 0x13e50542, 0x14651948, 0x14a53951,
	0x15354d5a, 0x15d58163, 0x1685916b, 0x1705c980, 0x1805f17e, 0x18462191,
	0x195661a1, 0x1aa69dad, 0x1b76e9c1, 0x1c5721d3, 0x1e2761e3, 0x1fa7bdf9,
	0x20382613, 0x22588a25, 0x2358e642, 0x25695663, 0x26e9da80, 0x298a76ac,
	0x2c5b06cd, 0x2e1b66f1, 0x318bf31c, 0x323c8334, 0x344d6365, 0x35bd9f74,
	0x2b2ad2c0, 0x2a0a76a2, 0x2899ee86, 0x25f97678, 0x2509064c, 0x2298b62f,
	0x21e85219, 0x1fa7ddff, 0x1e878dea, 0x1d8755d4, 0x1be70dcb, 0x1b26cdb2,
	0x1a4691aa, 0x19565d99, 0x18c6218e, 0x17c5f184, 0x1725c978, 0x16c5bd6b,
	0x16359166, 0x1585615e, 0x15454556, 0x14a52d4e, 0x14751d48, 0x13d4f945,
	0x13b4f13f, 0x13a4e93a, 0x1314cd3b, 0x1364cd36, 0x1354c533, 0x1354c535,
	0x1314cd31, 0x12e4c933, 0x1334cd35, 0x1314d937, 0x1364d93c, 0x1384d53c,
	0x13d4ed3d, 0x1404fd43, 0x14852d4a, 0x14953153, 0x15355158, 0x15b56563,
	0x1645956c, 0x1705c976, 0x1745e17e, 0x18661d8a, 0x19264196, 0x1a2685a6,
	0x1ae6c1b9, 0x1c0701c6, 0x1c9755df, 0x1e5799eb, 0x1ff80e06, 0x21384626,
	0x22d8aa36, 0x23992e4a, 0x26399269, 0x273a0a97, 0x29eaaeb2, 0x2c7b52d3,
	0x2e6b8efc, 0x304c7b18, 0x327cc744, 0x359d6f5b, 0x356dc77c, 0x2c5ad2bb,
	0x2aca96a2, 0x27ea2289, 0x26e97a76, 0x24f94256, 0x2368ea36, 0x21587224,
	0x20681e08, 0x1f27b9f2, 0x1df751db, 0x1cc735ce, 0x1b86e1bc, 0x1aa6b1ac,
	0x19a6759d, 0x18a64192, 0x18060589, 0x17c5f97c, 0x1735b170, 0x16c5a16b,
	0x16258963, 0x1595695d, 0x15155152, 0x14d53151, 0x1455214d, 0x14050947,
	0x14150542, 0x13d4f942, 0x1384e53a, 0x1364d93d, 0x1384d53a, 0x1354e93b,
	0x1394d53b, 0x13b4d93d, 0x1394ed3f, 0x13e4f140, 0x1404f545, 0x14550948,
	0x14a5194f, 0x15253d56, 0x15055157, 0x1625655e, 0x16259965, 0x16e5c56f,
	0x1785dd7d, 0x18260d89, 0x19063596, 0x193679a2, 0x1aa6adaa, 0x1bb6f9b9,
	0x1c971dd1, 0x1da78ddd, 0x1f47b9f8, 0x2068220c, 0x22586e2d, 0x2358ea40,
	0x25495a51, 0x2639d274, 0x2a1a369c, 0x2b8ae6cc, 0x2cab52d3, 0x2efc0efe,
	0x314c5b18, 0x33dceb44, 0x360d6357, 0x35bdbf6f, 0x2c0adabe, 0x2a1a8eac,
	0x28ca3ea4, 0x26e9c277, 0x25695a57, 0x23f8ea47, 0x21e88a27, 0x20c82613,
	0x1f77f1fd, 0x1e1779e5, 0x1ce741d7, 0x1b86f9c6, 0x1ae6c1b9, 0x1a4685a5,
	0x1946659f, 0x18a62d95, 0x1805f986, 0x1735e179, 0x16f5b972, 0x1645a56b,
	0x15e57567, 0x15456d61, 0x15055559, 0x14e54153, 0x14b5314e, 0x14651147,
	0x14450d4a, 0x13e4f945, 0x1404f946, 0x1414fd44, 0x13e50141, 0x13f50141,
	0x1454fd45, 0x14050544, 0x1425154a, 0x14451d4b, 0x14a52d4d, 0x15154151,
	0x1565615c, 0x15f58164, 0x16759569, 0x1715b173, 0x1775d17b, 0x17a60986,
	0x1886218f, 0x1976659d, 0x1a36a9a8, 0x1b76c5be, 0x1c2711c9, 0x1d0771d5,
	0x1e47a9ee, 0x1fc7f200, 0x20f8461a, 0x21f8a62b, 0x23c8f648, 0x25896269,
	0x27e9ea86, 0x295a96a2, 0x2bab26ca, 0x2e6b96ef, 0x2edc0f12, 0x32dc8b3b,
	0x34dd1b4f, 0x362db377, 0x397e1b8f, 0x2c8b4abe, 0x2b0aaeac, 0x290a429c,
	0x27e9e27a, 0x26198668, 0x24b9064a, 0x23489a33, 0x21084e1e, 0x1fa7f605,
	0x1e57a9ee, 0x1d374dd9, 0x1cc71dcf, 0x1ba6e9c0, 0x1ac6a1ab, 0x19f6759e,
	0x19264d96, 0x18c6218a, 0x17960183, 0x1735d57b, 0x16c5b971, 0x16258969,
	0x15e57d62, 0x1585695e, 0x15355555, 0x14f54154, 0x14f5354f, 0x1475254d,
	0x1485214a, 0x1465214a, 0x14451545, 0x14551546, 0x14751d45, 0x14751947,
	0x14252d4a, 0x1495294d, 0x14f53d58, 0x15054d5b, 0x15855d5d, 0x15a56d61,
	0x16258d69, 0x1695a571, 0x1765cd78, 0x17e5f181, 0x18462188, 0x19364d9b,
	0x19b679a6, 0x1aa6b9ab, 0x1b9701be, 0x1ca735d2, 0x1d7779e1, 0x1ec7d1eb,
	0x20c81e0a, 0x21c8661d, 0x22a8c23b, 0x24892a5d, 0x27299e7c, 0x28ca2a92,
	0x2adaaeb6, 0x2c7b3adb, 0x2ebbc2e4, 0x314c2708, 0x329cc738, 0x33dd5352,
	0x35edab74, 0x37ee3377, 0x2c8b2edb, 0x2b2ab2b3, 0x29ea7ea2, 0x286a0284,
	0x2679a66a, 0x25794259, 0x2378ce3c, 0x2238762e, 0x1ff81e0a, 0x1f07a9fd,
	0x1d6771e7, 0x1cf725d2, 0x1b8701c9, 0x1b36c9ba, 0x1a769da9, 0x1946719f,
	0x19264d93, 0x18761187, 0x17c5f583, 0x1705d177, 0x16d5ad6c, 0x1675a56f,
	0x16258d64, 0x15857965, 0x1585655c, 0x1515495a, 0x15254957, 0x15653d56,
	0x14c53d50, 0x14a53952, 0x14d53950, 0x14f53951, 0x14e54950, 0x15354556,
	0x1545515d, 0x1555615b, 0x15d5715f, 0x16358968, 0x16359969, 0x1695ad75,
	0x1725dd79, 0x17c5f582, 0x1866198b, 0x19264594, 0x19c669a6, 0x1a96a9b7,
	0x1b3701c0, 0x1c5711cb, 0x1d9759e0, 0x1e579de9, 0x201809ff, 0x21884216,
	0x2288862b, 0x23a91a49, 0x25197a5d, 0x2799d68a, 0x297a52a2, 0x2acad2c3,
	0x2eab6aed, 0x2f3bf302, 0x318c7323, 0x33acc746, 0x348d3760, 0x367e078f,
	0x384e278f, 0x2d1b1acd, 0x2c8afab5, 0x298aa6b5, 0x286a2297, 0x2659d27f,
	0x25496661, 0x2458ee3e, 0x22e89235, 0x21384a17, 0x1fa7e202, 0x1e2795ea,
	0x1d9751dd, 0x1c4719c9, 0x1bd6f1c1, 0x1ad6b5b0, 0x19f681a8, 0x19c6619d,
	0x18f63591, 0x1836198a, 0x17c5ed86, 0x1755e17a, 0x16d5c178, 0x1675c570,
	0x16358969, 0x15e5916a, 0x15e57563, 0x15b5715c, 0x15356161, 0x1555595d,
	0x15855959, 0x15c55951, 0x15c5655d, 0x1565615f, 0x15d5715f, 0x15b5815b,
	0x15e58165, 0x16459d69, 0x16e5b970, 0x1705c974, 0x1775e17d, 0x17c60d8a,
	0x1826358c, 0x19164d96, 0x196675a3, 0x1a96a1b4, 0x1b36d5b9, 0x1ca709c0,
	0x1cf745da, 0x1e27a9f3, 0x1f57d201, 0x20f82e0d, 0x21e85e27, 0x22b8de3c,
	0x23b93656, 0x26c9a674, 0x289a429e, 0x2a4aa2b3, 0x2c0b36d3, 0x2e2bbae8,
	0x2fabf321, 0x327c9b32, 0x34fd075b, 0x362d8b77, 0x374dc772, 0x391e3ba8,
	0x2d4b56cf, 0x2b0b0ed1, 0x298aaeb2, 0x29ea529a, 0x2839e284, 0x26198a6d,
	0x24a93a52, 0x2388de37, 0x21487a1f, 0x2058060e, 0x1e77da02, 0x1e177dea,
	0x1cf751db, 0x1c1705c9, 0x1b56e9be, 0x1a46a1b3, 0x19b679a2, 0x19465d9c,
	0x18c63d8f, 0x18961588, 0x17c60585, 0x1775e57f, 0x16e5cd7b, 0x1705bd73,
	0x1675b570, 0x16259d6a, 0x1625996a, 0x16258d68, 0x15f58d65, 0x15b58561,
	0x15c57d65, 0x15e59165, 0x15f5856b, 0x16759d66, 0x1675a16b, 0x16c5b572,
	0x16e5b175, 0x1715d97d, 0x1775e57e, 0x17f61185, 0x18e6418d, 0x18f655a3,
	0x19d671aa, 0x1a76a1af, 0x1b66d1b8, 0x1c2711c4, 0x1ca741d5, 0x1de779e3,
	0x1ed7b9fe, 0x1fd80e0a, 0x2158721a, 0x2208b22f, 0x24491245, 0x25c9626e,
	0x2769e28a, 0x2a0a6aac, 0x2a9af2c3, 0x2d6b5ee1, 0x300bf308, 0x30cc7b10,
	0x329ce334, 0x344d235e, 0x354dab7a, 0x381e2787, 0x37ee7387, 0x2d8b3acf,
	0x2cfb0ecf, 0x2b0adab2, 0x2a7a76a7, 0x27ea0a97, 0x26598a6e, 0x25295650,
	0x2368f240, 0x22089a26, 0x2098420f, 0x1fc7f5ff, 0x1ec7a9f6, 0x1da75de2,
	0x1c5735d5, 0x1ba6f9c2, 0x1ad6c5be, 0x1a5695aa, 0x19e689a1, 0x19065598,
	0x18b63994, 0x18361187, 0x17a60d89, 0x17f5e57d, 0x1735e579, 0x1735d178,
	0x16d5c172, 0x16a5a974, 0x1685a16c, 0x16659d6f, 0x1675a16e, 0x16259d6e,
	0x1695a56c, 0x1685a96b, 0x16b5b573, 0x16d5bd76, 0x1715c578, 0x1795dd7a,
	0x17a5fd86, 0x17e61589, 0x18961d8e, 0x1906559e, 0x197679a3, 0x1a5691ac,
	0x1af6c5c0, 0x1b76edc6, 0x1cc735d5, 0x1db771e4, 0x1dc7a9f9, 0x1f880a05,
	0x2058421c, 0x21d86e25, 0x23c8ea3e, 0x24694252, 0x2659a678, 0x290a369c,
	0x29eaaeb3, 0x2cab1ad4, 0x2edb8aeb, 0x2f7c1716, 0x320c7b3b, 0x32fcff54,
	0x359d5359, 0x36cddf81, 0x386e07b1, 0x3aae739a, 0x2e1b4aea, 0x2ceb22cc,
	0x2b5afac0, 0x29ba82b2, 0x289a4a97, 0x2739ea78, 0x25897a65, 0x24e93a4f,
	0x2298d235, 0x21d88621, 0x20583210, 0x1f87cdfd, 0x1e378deb, 0x1d3751e2,
	0x1c4721cf, 0x1be6e9c7, 0x1b16c9b5, 0x1a469db0, 0x1a1675a0, 0x194661a0,
	0x18b63d96, 0x18762993, 0x1886158a, 0x18160187, 0x1775f57c, 0x17c5e57c,
	0x1765e177, 0x1755c97b, 0x16e5c575, 0x1735c976, 0x1755d577, 0x1715d97e,
	0x1795e178, 0x1755e17d, 0x1755e981, 0x1775ed83, 0x17e6158e, 0x18762592,
	0x18f65592, 0x19065da2, 0x19c681a4, 0x1a76b5b3, 0x1b36d5ba, 0x1c070dcd,
	0x1d1735d5, 0x1d675de6, 0x1ec7a1ee, 0x1f27ddff, 0x20681a0d, 0x2198662f,
	0x22f8de3d, 0x23f93a56, 0x26499e72, 0x27b9ea80, 0x29ba72a5, 0x2bfadebe,
	0x2dab6ae1, 0x302bbb04, 0x30cc4f2d, 0x31acdb38, 0x34fd474f, 0x362d7760,
	0x377e0787, 0x386e5fa5, 0x3a2e97c9, 0x2ddb8eea, 0x2d1b6adb, 0x2c2aeaca,
	0x2a9aa6b5, 0x291a52a5, 0x27aa1687, 0x2649a274, 0x25095e5d, 0x23a8e649,
	0x2238aa2f, 0x21086216, 0x1f980e15, 0x1f17c9f6, 0x1e179deb, 0x1d274dda,
	0x1c572dd2, 0x1be701c4, 0x1b16ddbc, 0x1a66adaf, 0x1a1689aa, 0x196659a3,
	0x1916559d, 0x18f63d93, 0x18962d92, 0x18361d8e, 0x18460583, 0x17c60589,
	0x1795fd88, 0x17d60185, 0x1835f186, 0x1795f587, 0x1825fd84, 0x17d60989,
	0x17c62183, 0x18461991, 0x18e63d8f, 0x18b64d96, 0x19466d9d, 0x19b671a2,
	0x1a068daa, 0x1b36c1b7, 0x1b86e9be, 0x1c2729c9, 0x1c5731d1, 0x1d9779e3,
	0x1d8799f3, 0x1ee7c9ff, 0x2068320d, 0x21686a29, 0x22f8a637, 0x23c92248,
	0x25995a61, 0x2769f67e, 0x283a5a9c, 0x2acaeabe, 0x2c7b56eb, 0x2e6bc2ef,
	0x2fac2714, 0x318cb72d, 0x32fd073f, 0x338d3f68, 0x362d9784, 0x386e1387,
	0x381e73b1, 0x3a2eafcf, 0x2e8bbb00, 0x2ddb66e1, 0x2b8b22bb, 0x2a9ac6aa,
	0x29ea8aa7, 0x27fa269a, 0x2779d67f, 0x25c99e6e, 0x24d91256, 0x2338ee3c,
	0x22289628, 0x20f84a16, 0x1f9809ff, 0x1f27b9f9, 0x1da781eb, 0x1d6755dd,
	0x1c8729d5, 0x1c26f9c9, 0x1b46ddc0, 0x1aa6b5ba, 0x1a469da9, 0x1a2679a4,
	0x19866d9f, 0x1916519c, 0x18c6559a, 0x18e64596, 0x18663190, 0x18b62594,
	0x18b63187, 0x1896258d, 0x1876158f, 0x18363590, 0x18c63193, 0x18a64199,
	0x18f64999, 0x1966659f, 0x1a0675a5, 0x1a0699a9, 0x1aa6a5b0, 0x1b36d5bb,
	0x1ba6f9be, 0x1c171dcd, 0x1cc751db, 0x1db781e7, 0x1e77a1ed, 0x1fb7edfb,
	0x20181614, 0x21785221, 0x22f8d235, 0x23a8f654, 0x25b98a6f, 0x2739d27f,
	0x289a2a90, 0x2a7abec3, 0x2bfb2ecd, 0x2d4bb6ed, 0x300c0f0c, 0x310c7329,
	0x332d0f57, 0x352d2368, 0x359d837a, 0x36cdc781, 0x37ee1389, 0x3aae97ab,
	0x3bfee7c9, 0x2f5bb6fc, 0x2dab8aed, 0x2bdb3aca, 0x2b2b0ec0, 0x298aa2af,
	0x295a4e9f, 0x27aa0a8a, 0x2699b278, 0x24d9724c, 0x23e91e45, 0x2228b238,
	0x21d86e29, 0x20083614, 0x1f07da03, 0x1e87a9f1, 0x1db791e7, 0x1ca749d8,
	0x1c4719cc, 0x1be6f9c9, 0x1b16e1bb, 0x1ad6bdb7, 0x1a26b5af, 0x1a2695aa,
	0x19e66da0, 0x19566da2, 0x19965da2, 0x1946559c, 0x1916499d, 0x18f66d9a,
	0x19564d98, 0x18b64999, 0x19265d99, 0x18f6619f, 0x196679a3, 0x19a675a9,
	0x19f695aa, 0x1a36adb2, 0x1af6c5b8, 0x1b36d9c6, 0x1bd6f9c2, 0x1c8721d1,
	0x1cf745dd, 0x1d0769e0, 0x1e57a1ee, 0x1f17e608, 0x20182e0d, 0x21186622,
	0x2248a235, 0x23d8f248, 0x2469425b, 0x26799a73, 0x286a1692, 0x28ca76a2,
	0x2afaeabe, 0x2ceb56eb, 0x2ddbc30a, 0x302c3f1c, 0x320ccf41, 0x348d1b54,
	0x354d536d, 0x360ddf7f, 0x37ee1b87, 0x39ce6ba5, 0x39fec7c0, 0x3bff3fd2,
	0x2f8bbb00, 0x2ebb82f7, 0x2cab42d6, 0x2cfafabb, 0x2a3aa2b5, 0x297a5ea2,
	0x28ea269a, 0x27b9ca82, 0x26198e6e, 0x24793e54, 0x2378e63c, 0x22a88e36,
	0x21186221, 0x20881213, 0x1fa7e601, 0x1ec7b1ef, 0x1df779e7, 0x1ce745d7,
	0x1c9719ce, 0x1c2719c9, 0x1b4701c6, 0x1ac6d5bf, 0x1ac6c1bb, 0x1aa6a5af,
	0x1a6691aa, 0x1a0691ad, 0x1a068dab, 0x19b679a5, 0x19e68daa, 0x19a679a9,
	0x19e68da8, 0x1a4689a6, 0x19a68db1, 0x19d695ad, 0x1ab6b9b4, 0x1b16cdbb,
	0x1ac6f1c3, 0x1be6f1c9, 0x1be71dcb, 0x1c8739d9, 0x1d4765e9, 0x1d779de9,
	0x1ed7c9f9, 0x1fc7fa07, 0x1fb81614, 0x20d84e25, 0x2288ae2d, 0x23a8e651,
	0x24e94257, 0x25e9ba72, 0x277a168a, 0x290a4eaf, 0x2afabac5, 0x2ccb36d3,
	0x2e4bb6ed, 0x2fac270e, 0x320c8329, 0x338ce35b, 0x336d5b5e, 0x360d9f68,
	0x374dfb97, 0x377e338f, 0x394e8ba5, 0x3bff1be2, 0x3bff97e9, 0x310c1f08,
	0x2e6bb6ef, 0x2ccb7adf, 0x2c0b22cf, 0x2b0abec7, 0x2a1aa2af, 0x290a66a4,
	0x283a0287, 0x2659c67e, 0x25297a65, 0x25193651, 0x22c8e640, 0x2208aa28,
	0x21485e22, 0x1fd8260d, 0x1fa7fa06, 0x1e57b5fa, 0x1e1795e6, 0x1d5759ee,
	0x1ca741d5, 0x1c7711d2, 0x1c470dce, 0x1ba6f5c4, 0x1b06f5bf, 0x1b46c1bd,
	0x1a66c1be, 0x1a96b1b8, 0x1a76c5b2, 0x1a66c1b9, 0x1a76b1b9, 0x1aa6cdb1,
	0x1ad6b9b4, 0x1ae6c9bf, 0x1af6e1c0, 0x1b76f9c9, 0x1bc711cc, 0x1c3721d5,
	0x1c8739d4, 0x1cd765db, 0x1d9769ea, 0x1e77a9f9, 0x1f07c9fd, 0x1fa8120c,
	0x20b84e19, 0x21886226, 0x22e8aa36, 0x23d8fe4c, 0x24995e61, 0x26799a72,
	0x2719fa97, 0x28da4a98, 0x2acac6c5, 0x2bdb3ad9, 0x2e1b82f1, 0x2fec1712,
	0x316c931e, 0x32fce348, 0x33dd2b5e, 0x36fd5b6d, 0x367db394, 0x37ce6b84,
	0x39fe53a5, 0x3b0ebbc9, 0x3b9f63df, 0x3fffdbff, 0x30cc6b44, 0x2fec1f08,
	0x2dab8aea, 0x2c7b4ad6, 0x2b3afecf, 0x2a7ac6ba, 0x29ba9ab6, 0x28da329b,
	0x279a228d, 0x2699c672, 0x25696e5f, 0x24a93a49, 0x2358e238, 0x22a8be33,
	0x21a87627, 0x20684a12, 0x1fa7f20e, 0x1ec7c9f7, 0x1e0799fb, 0x1e3789ee,
	0x1d375dec, 0x1ca755df, 0x1c672dd6, 0x1c2719cc, 0x1c0725c8, 0x1c1715c9,
	0x1b8705c4, 0x1c0701cc, 0x1b86edc3, 0x1b8701c9, 0x1b96f1c9, 0x1b76fdcb,
	0x1c3715cb, 0x1c2715d0, 0x1c5731d7, 0x1d0735da, 0x1d1759e2, 0x1db769ea,
	0x1df791f1, 0x1ef7d601, 0x1f77f602, 0x20582214, 0x2118561d, 0x22788e35,
	0x2368e640, 0x23d91a51, 0x25197a6e, 0x2709b27f, 0x28aa2a95, 0x294a66af,
	0x2b0adac5, 0x2cab22e2, 0x2e1bb6f7, 0x300c0f18, 0x318c6b36, 0x338cff46,
	0x346d8359, 0x35ed9774, 0x367de787, 0x36adfb9a, 0x377ed3a0, 0x397ec7c3,
	0x3c2ef3df, 0x3defbfff, 0x3fffffff, 0x318c6b18, 0x2fac2f12, 0x2dbbb6f8,
	0x2d4b52d3, 0x2bfb12c8, 0x2c0ae6c5, 0x2b3aa2b6, 0x287a96ac, 0x28ea2695,
	0x2829ea8d, 0x2639ae6e, 0x2529625a, 0x24092e51, 0x2398ea48, 0x22789a36,
	0x20e88e21, 0x20f83222, 0x1fa81208, 0x1fa7d9fb, 0x1f07adf9, 0x1df795f7,
	0x1d8779ea, 0x1d975de6, 0x1d173ddd, 0x1ca739da, 0x1c6735d9, 0x1c4735d5,
	0x1c372dd9, 0x1c8729ce, 0x1c5731da, 0x1c8735d5, 0x1c8735db, 0x1c873ddd,
	0x1d0755e2, 0x1ce769ea, 0x1d777de5, 0x1e679df6, 0x1e57c1ff, 0x1f77f200,
	0x20081e10, 0x2078461b, 0x21787227, 0x21f89233, 0x22d8ee4c, 0x23f92a59,
	0x25797e68, 0x26a9c278, 0x283a1a92, 0x290a72b6, 0x2afacec0, 0x2c5b22e2,
	0x2ceb96f3, 0x2ebbf30e, 0x308c632d, 0x32bcc74b, 0x32dd1b7a, 0x36adbf77,
	0x377df37f, 0x379e279d, 0x371e8bb1, 0x39cec7c3, 0x3cef1bc9, 0x3dbfb3df,
	0x3fffffe9, 0x3fffffff, 0x332ce34d, 0x325c7b1c, 0x2e6bc2f8, 0x2d6b9ef8,
	0x2cfb2edb, 0x2bdb12c7, 0x2b0ae6c0, 0x2afaaeba, 0x286a52af, 0x277a028b,
	0x277a0a90, 0x2679a674, 0x25696668, 0x24491e51, 0x22d8ee44, 0x2228c237,
	0x21987e2b, 0x21186229, 0x20383a17, 0x1f781614, 0x1f37e205, 0x1ee7b5f8,
	0x1e5799fd, 0x1d67b1ed, 0x1e0789ea, 0x1d6781ea, 0x1d8771e7, 0x1d1771db,
	0x1d477de4, 0x1d677dea, 0x1ce775f1, 0x1d878de9, 0x1d87a1f7, 0x1e87bdf3,
	0x1e67aa05, 0x1fb7ea05, 0x1f37fa19, 0x1fa82e1b, 0x20984e1b, 0x20f86e1e,
	0x2208ae2c, 0x22d8be34, 0x23b8fe5b, 0x2479465e, 0x2469a278, 0x26c9e28b,
	0x277a3ea1, 0x29baaeb6, 0x2a9aeac8, 0x2c8b56df, 0x2e4bb702, 0x304c1718,
	0x318c7338, 0x323cff44, 0x34dd475e, 0x354d8b87, 0x36ce3b87, 0x374e1b89,
	0x38ce73bd, 0x399ea3b4, 0x3c5ee7c6, 0x3e8f7ff6, 0x3e8ff7ff, 0x3fffffff,
	0x3fffffff,
};
*/

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
    {3, 4, 6},        //TNR_P2P_SAD_THRESHOLD (0~255)
    {16, 8, 0},          //TNR_P2P_WEIGHT (0~16)
    5,                 //TNR_P2P_BORDER_CHECK_THRES (0~255)
    4,                  //TNR_P2P_BORDER_CHECK_SCALE (0~7)
    0,                  //TNR_P2P_INPUT (0: original source, 1: mctf result)
    10,                 //TNR_P2P_MCTF_MOTION_THRESHOLD (0~255)
    {2, 2, 2, 2},        //TNR_P2P_MCTF_MOTION_WEIGHT (0~3)
    //P2P TNR Config----------------------------------------------------------------------------
    {1, 2, 2},        //TNR_P2P_REF_Y_WEIGHT (0~7)
    {1, 3, 2},        //TNR_P2P_REF_C_WEIGHT (0~7)
    {40, 50, 70},        //TNR_P2P_RANGE_Y_THRESHOLD (0~255)
    {12, 18, 30},        //TNR_P2P_RANGE_C_THRESHOLD (0~255)
    0,                  //TNR_P2P_CLAMP_Y_THRESHOLD (0~255)
    2,                  //TNR_P2P_CLAMP_Y_LSR (0~7)
    0,                  //TNR_P2P_CLAMP_C_THRESHOLD (0~255)
    2,                  //TNR_P2P_CLAMP_C_LSR (0~7)
    //MCTF TNR Config----------------------------------------------------------------------------
    {1, 4, 1},          //TNR_MCTF_REF_Y_WEIGHT (0~7)
    {1, 4, 1},          //TNR_MCTF_REF_C_WEIGHT (0~7)
    {15, 20, 40},        //TNR_MCTF_RANGE_Y_THRESHOLD (0~255)
    {12, 18, 30},        //TNR_MCTF_RANGE_C_THRESHOLD (0~255)
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
    {4, 7, 9},        //TNR_P2P_SAD_THRESHOLD (0~255)
    {16, 8, 0},          //TNR_P2P_WEIGHT (0~16)
    5,                 //TNR_P2P_BORDER_CHECK_THRES (0~255)
    4,                  //TNR_P2P_BORDER_CHECK_SCALE (0~7)
    0,                  //TNR_P2P_INPUT (0: original source, 1: mctf result)
    8,          //TNR_P2P_MCTF_MOTION_THRESHOLD (0~255)
    {2, 2, 2, 2},        //TNR_P2P_MCTF_MOTION_WEIGHT (0~3)
    //P2P TNR Config----------------------------------------------------------------------------
    {1, 2, 2},        //TNR_P2P_REF_Y_WEIGHT (0~7)
    {1, 3, 2},        //TNR_P2P_REF_C_WEIGHT (0~7)
    {50, 65, 90},        //TNR_P2P_RANGE_Y_THRESHOLD (0~255)
    {18, 30, 40},        //TNR_P2P_RANGE_C_THRESHOLD (0~255)
    0,                  //TNR_P2P_CLAMP_Y_THRESHOLD (0~255)
    2,                  //TNR_P2P_CLAMP_Y_LSR (0~7)
    0,                  //TNR_P2P_CLAMP_C_THRESHOLD (0~255)
    2,                  //TNR_P2P_CLAMP_C_LSR (0~7)
    //MCTF TNR Config----------------------------------------------------------------------------
    {1, 5, 2},          //TNR_MCTF_REF_Y_WEIGHT (0~7)
    {1, 5, 2},          //TNR_MCTF_REF_C_WEIGHT (0~7)
    {30, 45, 70},        //TNR_MCTF_RANGE_Y_THRESHOLD (0~255)
    {15, 22, 35},        //TNR_MCTF_RANGE_C_THRESHOLD (0~255)
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
    {5, 8, 10},        //TNR_P2P_SAD_THRESHOLD (0~255)
    {16, 8, 0},          //TNR_P2P_WEIGHT (0~16)
    10,                 //TNR_P2P_BORDER_CHECK_THRES (0~255)
    4,                  //TNR_P2P_BORDER_CHECK_SCALE (0~7)
    0,                  //TNR_P2P_INPUT (0: original source, 1: mctf result)
    4,          //TNR_P2P_MCTF_MOTION_THRESHOLD (0~255)
    {3, 3, 3, 3},        //TNR_P2P_MCTF_MOTION_WEIGHT (0~3)
    //P2P TNR Config----------------------------------------------------------------------------
    {1, 2, 1},        //TNR_P2P_REF_Y_WEIGHT (0~7)
    {1, 2, 1},        //TNR_P2P_REF_C_WEIGHT (0~7)
    {65, 80, 90},        //TNR_P2P_RANGE_Y_THRESHOLD (0~255)
    {30, 40, 50},        //TNR_P2P_RANGE_C_THRESHOLD (0~255)
    0,                  //TNR_P2P_CLAMP_Y_THRESHOLD (0~255)
    2,                  //TNR_P2P_CLAMP_Y_LSR (0~7)
    0,                  //TNR_P2P_CLAMP_C_THRESHOLD (0~255)
    2,                  //TNR_P2P_CLAMP_C_LSR (0~7)
    //MCTF TNR Config----------------------------------------------------------------------------
    {1, 5, 1},        //TNR_MCTF_REF_Y_WEIGHT (0~7)
    {1, 5, 1},        //TNR_MCTF_REF_C_WEIGHT (0~7)
    {40, 60, 80},        //TNR_MCTF_RANGE_Y_THRESHOLD (0~255)
    {20, 30, 40},        //TNR_MCTF_RANGE_C_THRESHOLD (0~255)
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

static H26XEncTnr tnr_param_lv8 = //ISO12800
{   1,                  //enable temporal noise reduction (0: disable, 1:enable)
    0,                  //TNR Mode as OSD enabled: fully TNR (0: fully TNR, 1: P2P only, 2: no TNR)
    1,                  //enable mctf/p2p pixel blending (0: disable, 1:enable)
    0,                  //P2P SAD Mode (0: pixel, 3: SAD + pixel)
    0,                  //MCTF SAD Mode (0: pixel, 1: b8x8, 2: b16x16, 3: auto)
    0,                  //MCTF Bias Mode (0: disable, 1: b8x8, 2: b16x16, 3: auto)
    {10, 12, 14},        //TNR_P2P_SAD_THRESHOLD (0~255)
    {16, 8, 0},          //TNR_P2P_WEIGHT (0~16)
    10,                 //TNR_P2P_BORDER_CHECK_THRES (0~255)
    4,                  //TNR_P2P_BORDER_CHECK_SCALE (0~7)
    0,                  //TNR_P2P_INPUT (0: original source, 1: mctf result)
    4,          //TNR_P2P_MCTF_MOTION_THRESHOLD (0~255)
    {3, 3, 3, 3},        //TNR_P2P_MCTF_MOTION_WEIGHT (0~3)
    //P2P TNR Config----------------------------------------------------------------------------
    {1, 3, 2},        //TNR_P2P_REF_Y_WEIGHT (0~7)
    {1, 3, 2},        //TNR_P2P_REF_C_WEIGHT (0~7)
    {80, 90, 110},        //TNR_P2P_RANGE_Y_THRESHOLD (0~255)
    {35, 50, 60},        //TNR_P2P_RANGE_C_THRESHOLD (0~255)
    0,                  //TNR_P2P_CLAMP_Y_THRESHOLD (0~255)
    2,                  //TNR_P2P_CLAMP_Y_LSR (0~7)
    0,                  //TNR_P2P_CLAMP_C_THRESHOLD (0~255)
    2,                  //TNR_P2P_CLAMP_C_LSR (0~7)
    //MCTF TNR Config----------------------------------------------------------------------------
    {1, 4, 2},        //TNR_MCTF_REF_Y_WEIGHT (0~7)
    {1, 4, 2},        //TNR_MCTF_REF_C_WEIGHT (0~7)
    {40, 60, 90},        //TNR_MCTF_RANGE_Y_THRESHOLD (0~255)
    {20, 30, 40},        //TNR_MCTF_RANGE_C_THRESHOLD (0~255)
    0,          //TNR_MCTF_CLAMP_Y_THRESHOLD (0~255)
    0,          //TNR_MCTF_CLAMP_Y_LSR (0~7)
    0,          //TNR_MCTF_CLAMP_C_THRESHOLD (0~255)
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
    {10, 12, 14},        //TNR_P2P_SAD_THRESHOLD (0~255)
    {16, 8, 0},          //TNR_P2P_WEIGHT (0~16)
    10,                 //TNR_P2P_BORDER_CHECK_THRES (0~255)
    4,                  //TNR_P2P_BORDER_CHECK_SCALE (0~7)
    0,                  //TNR_P2P_INPUT (0: original source, 1: mctf result)
    4,          //TNR_P2P_MCTF_MOTION_THRESHOLD (0~255)
    {3, 3, 3, 3},        //TNR_P2P_MCTF_MOTION_WEIGHT (0~3)
    //P2P TNR Config----------------------------------------------------------------------------
    {1, 3, 2},        //TNR_P2P_REF_Y_WEIGHT (0~7)
    {1, 3, 2},        //TNR_P2P_REF_C_WEIGHT (0~7)
    {80, 90, 110},        //TNR_P2P_RANGE_Y_THRESHOLD (0~255)
    {35, 50, 60},        //TNR_P2P_RANGE_C_THRESHOLD (0~255)
    0,                  //TNR_P2P_CLAMP_Y_THRESHOLD (0~255)
    2,                  //TNR_P2P_CLAMP_Y_LSR (0~7)
    0,                  //TNR_P2P_CLAMP_C_THRESHOLD (0~255)
    2,                  //TNR_P2P_CLAMP_C_LSR (0~7)
    //MCTF TNR Config----------------------------------------------------------------------------
    {1, 4, 2},        //TNR_MCTF_REF_Y_WEIGHT (0~7)
    {1, 4, 2},        //TNR_MCTF_REF_C_WEIGHT (0~7)
    {40, 60, 90},        //TNR_MCTF_RANGE_Y_THRESHOLD (0~255)
    {20, 30, 40},        //TNR_MCTF_RANGE_C_THRESHOLD (0~255)
    0,          //TNR_MCTF_CLAMP_Y_THRESHOLD (0~255)
    0,          //TNR_MCTF_CLAMP_Y_LSR (0~7)
    0,          //TNR_MCTF_CLAMP_C_THRESHOLD (0~255)
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
    {10, 12, 14},        //TNR_P2P_SAD_THRESHOLD (0~255)
    {16, 8, 0},          //TNR_P2P_WEIGHT (0~16)
    10,                 //TNR_P2P_BORDER_CHECK_THRES (0~255)
    4,                  //TNR_P2P_BORDER_CHECK_SCALE (0~7)
    0,                  //TNR_P2P_INPUT (0: original source, 1: mctf result)
    4,          //TNR_P2P_MCTF_MOTION_THRESHOLD (0~255)
    {3, 3, 3, 3},        //TNR_P2P_MCTF_MOTION_WEIGHT (0~3)
    //P2P TNR Config----------------------------------------------------------------------------
    {1, 3, 2},        //TNR_P2P_REF_Y_WEIGHT (0~7)
    {1, 3, 2},        //TNR_P2P_REF_C_WEIGHT (0~7)
    {80, 90, 110},        //TNR_P2P_RANGE_Y_THRESHOLD (0~255)
    {35, 50, 60},        //TNR_P2P_RANGE_C_THRESHOLD (0~255)
    0,                  //TNR_P2P_CLAMP_Y_THRESHOLD (0~255)
    2,                  //TNR_P2P_CLAMP_Y_LSR (0~7)
    0,                  //TNR_P2P_CLAMP_C_THRESHOLD (0~255)
    2,                  //TNR_P2P_CLAMP_C_LSR (0~7)
    //MCTF TNR Config----------------------------------------------------------------------------
    {1, 4, 2},        //TNR_MCTF_REF_Y_WEIGHT (0~7)
    {1, 4, 2},        //TNR_MCTF_REF_C_WEIGHT (0~7)
    {40, 60, 90},        //TNR_MCTF_RANGE_Y_THRESHOLD (0~255)
    {20, 30, 40},        //TNR_MCTF_RANGE_C_THRESHOLD (0~255)
    0,          //TNR_MCTF_CLAMP_Y_THRESHOLD (0~255)
    0,          //TNR_MCTF_CLAMP_Y_LSR (0~7)
    0,          //TNR_MCTF_CLAMP_C_THRESHOLD (0~255)
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
    {10, 12, 14},        //TNR_P2P_SAD_THRESHOLD (0~255)
    {16, 8, 0},          //TNR_P2P_WEIGHT (0~16)
    10,                 //TNR_P2P_BORDER_CHECK_THRES (0~255)
    4,                  //TNR_P2P_BORDER_CHECK_SCALE (0~7)
    0,                  //TNR_P2P_INPUT (0: original source, 1: mctf result)
    4,          //TNR_P2P_MCTF_MOTION_THRESHOLD (0~255)
    {3, 3, 3, 3},        //TNR_P2P_MCTF_MOTION_WEIGHT (0~3)
    //P2P TNR Config----------------------------------------------------------------------------
    {1, 3, 2},        //TNR_P2P_REF_Y_WEIGHT (0~7)
    {1, 3, 2},        //TNR_P2P_REF_C_WEIGHT (0~7)
    {80, 90, 110},        //TNR_P2P_RANGE_Y_THRESHOLD (0~255)
    {35, 50, 60},        //TNR_P2P_RANGE_C_THRESHOLD (0~255)
    0,                  //TNR_P2P_CLAMP_Y_THRESHOLD (0~255)
    2,                  //TNR_P2P_CLAMP_Y_LSR (0~7)
    0,                  //TNR_P2P_CLAMP_C_THRESHOLD (0~255)
    2,                  //TNR_P2P_CLAMP_C_LSR (0~7)
    //MCTF TNR Config----------------------------------------------------------------------------
    {1, 4, 2},        //TNR_MCTF_REF_Y_WEIGHT (0~7)
    {1, 4, 2},        //TNR_MCTF_REF_C_WEIGHT (0~7)
    {40, 60, 90},        //TNR_MCTF_RANGE_Y_THRESHOLD (0~255)
    {20, 30, 40},        //TNR_MCTF_RANGE_C_THRESHOLD (0~255)
    0,          //TNR_MCTF_CLAMP_Y_THRESHOLD (0~255)
    0,          //TNR_MCTF_CLAMP_Y_LSR (0~7)
    0,          //TNR_MCTF_CLAMP_C_THRESHOLD (0~255)
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
    {10, 12, 14},        //TNR_P2P_SAD_THRESHOLD (0~255)
    {16, 8, 0},          //TNR_P2P_WEIGHT (0~16)
    10,                 //TNR_P2P_BORDER_CHECK_THRES (0~255)
    4,                  //TNR_P2P_BORDER_CHECK_SCALE (0~7)
    0,                  //TNR_P2P_INPUT (0: original source, 1: mctf result)
    4,          //TNR_P2P_MCTF_MOTION_THRESHOLD (0~255)
    {3, 3, 3, 3},        //TNR_P2P_MCTF_MOTION_WEIGHT (0~3)
    //P2P TNR Config----------------------------------------------------------------------------
    {1, 3, 2},        //TNR_P2P_REF_Y_WEIGHT (0~7)
    {1, 3, 2},        //TNR_P2P_REF_C_WEIGHT (0~7)
    {80, 90, 110},        //TNR_P2P_RANGE_Y_THRESHOLD (0~255)
    {35, 50, 60},        //TNR_P2P_RANGE_C_THRESHOLD (0~255)
    0,                  //TNR_P2P_CLAMP_Y_THRESHOLD (0~255)
    2,                  //TNR_P2P_CLAMP_Y_LSR (0~7)
    0,                  //TNR_P2P_CLAMP_C_THRESHOLD (0~255)
    2,                  //TNR_P2P_CLAMP_C_LSR (0~7)
    //MCTF TNR Config----------------------------------------------------------------------------
    {1, 4, 2},        //TNR_MCTF_REF_Y_WEIGHT (0~7)
    {1, 4, 2},        //TNR_MCTF_REF_C_WEIGHT (0~7)
    {40, 60, 90},        //TNR_MCTF_RANGE_Y_THRESHOLD (0~255)
    {20, 30, 40},        //TNR_MCTF_RANGE_C_THRESHOLD (0~255)
    0,          //TNR_MCTF_CLAMP_Y_THRESHOLD (0~255)
    0,          //TNR_MCTF_CLAMP_Y_LSR (0~7)
    0,          //TNR_MCTF_CLAMP_C_THRESHOLD (0~255)
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
void iq_param_ov7251_id0_evb_table1_init(UINT32 id,UINT32 mode_idx)
{
	//iqs_set_range
	UINT32 *iq_param_addr[IQ_PARAM_MAX];
	static UINT32 num_3DNR_param = TOTAL_UI_3DNR_LV;
	gISO_Index[id].Id = id;
    gISO_Index[id].num = IQS_ISOMAPTAB_NUM - 1;

	iq_param_addr[IQ_ISOIDX] = (UINT32 *)&gISO_Index[id];

    iq_param_addr[IQ_ISOIDX_MAP ] = (UINT32*)&iso_map_tab_ov7251_id0_evb[0];

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
    iq_param_addr[IQ_DCE_DIST	] = (UINT32*)&dist_ov7251_id0_evb;

    iq_param_addr[IQ_DCE_RADIOUS] = (UINT32*)&radious_ov7251_id0_evb;

    iq_param_addr[IQ_CFAINTER	] = (UINT32*)&cfa_ov7251_id0_evb;

	iq_param_addr[IQ_GDC_FOV	] = (UINT32*)&fov_ov7251_id0_evb;
    iq_param_addr[IQ_GDC_GLUT	] = (UINT32*)ldc_g_lut_ov7251_id0_evb;
    iq_param_addr[IQ_GDC_RLUT	] = (UINT32*)ldc_r_lut_ov7251_id0_evb;
    iq_param_addr[IQ_GDC_BLUT	] = (UINT32*)ldc_b_lut_ov7251_id0_evb;
	iq_param_addr[IQ_DCE_ENH    ] = (UINT32 *)&ldc_enh;

	iq_param_addr[IQ_DCE_CA	    ] = (UINT32*)&cac_ov7251_id0_evb;

	iq_param_addr[IQ_DCE_2D_PARAM   ] = (UINT32*)&dc_2d_param_ov7251_id0_evb[0];
	iq_param_addr[IQ_DCE_2DLUT00    ] = (UINT32*)dc_2dlut00_ov7251_id0_evb;
	iq_param_addr[IQ_DCE_2DLUT01    ] = (UINT32*)dc_2dlut01_ov7251_id0_evb;
	iq_param_addr[IQ_DCE_2DLUT02    ] = (UINT32*)dc_2dlut02_ov7251_id0_evb;
	iq_param_addr[IQ_DCE_2DLUT03    ] = (UINT32*)dc_2dlut03_ov7251_id0_evb;

	iq_param_addr[IQ_DCE_STRP_INFO  ] = (UINT32*)&strp_info_ov7251_id0_evb[0];

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

    iq_param_addr[IQ_CCM_TH     ] = (UINT32*)&color_temperature_th_ov7251_id0_evb;
	iq_param_addr[IQ_Color_L    ] = (UINT32*)&color_low_ov7251_id0_evb;
	iq_param_addr[IQ_Color_M    ] = (UINT32*)&color_middle_ov7251_id0_evb;
	iq_param_addr[IQ_Color_H    ] = (UINT32*)&color_high_ov7251_id0_evb;
    iq_param_addr[IQ_FSTAB      ] = (UINT32*)fs_tab_ov7251_id0_evb[0];
	iq_param_addr[IQ_FDTAB	    ] = (UINT32*)fd_tab_ov7251_id0_evb[0];
    iq_param_addr[IQ_CC         ] = (UINT32*)&cc_ov7251_id0_evb;

    iq_param_addr[IQ_Y_CURVE    ] = (UINT32*)y_curve_ov7251_id0_evb;

	iq_param_addr[IQ_GAMMA_TH	] = (UINT32*)&gamma_lv_th_ov7251_id0_evb;
    iq_param_addr[IQ_GAMMA_DAY	] = (UINT32*)gamma_day_ov7251_id0_evb;
	iq_param_addr[IQ_GAMMA_NIGHT] = (UINT32*)gamma_night_ov7251_id0_evb;

	iq_param_addr[IQ_3DCC_TAB   ] = (UINT32*)color_3d_lut_ov7251_id0_evb;
    iq_param_addr[IQ_3DCC_ROUND ] = (UINT32*)&color_3d_ov7251_id0_evb;

	iq_param_addr[IQ_EDGTAB     ] = (UINT32*)edg_tab_ov7251_id0_evb;
    iq_param_addr[IQ_DDSTAB     ] = (UINT32*)dds_tab_ov7251_id0_evb;

	iq_param_addr[IQ_SATURATION ] = (UINT32 *)&saturation[0];
	iq_param_addr[IQ_CONTRAST   ] = (UINT32 *)&contrast[0];
    iq_param_addr[IQ_CCTRL_INFO ] = (UINT32 *)&cctrl_info[0];

	iq_param_addr[IQ_CST_COEF	] = (UINT32*)cst_coef_ov7251_id0_evb;
    iq_param_addr[IQ_CST_PARAM	] = (UINT32*)&cst_ov7251_id0_evb;

	iq_param_addr[IQ_YCCFIX	    ] = (UINT32*)&ycc_fix_ov7251_id0_evb;

    iq_param_addr[IQ_NOISE_PARAM] = (UINT32*)&noise_param_ov7251_id0_evb;

    iq_param_addr[IQ_GAMMA_RAND ] = (UINT32*)&gamma_rand_ov7251_id0_evb;

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
	iq_param_addr[IQ_HDR            ] = (UINT32 *)&hdr_ov7251_id0_evb;

	//***********
	//** Defog **
	//***********
	iq_param_addr[IQ_DEFOG_INIT_PARAM     ] = (UINT32 *)&defog_init_param_ov7251_id0_evb;
	iq_param_addr[IQ_DEFOG_WET     ] = (UINT32 *)&defog_wet_ov7251_id0_evb[0];

	iq_set_param_addr(id,mode_idx, iq_param_addr);
}

#endif
