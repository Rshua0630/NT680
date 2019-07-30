#ifndef _IQ_PARAM_TABLE1_OV7251_ID1_EVB_C
#define _IQ_PARAM_TABLE1_OV7251_ID1_EVB_C

#include "iq_param_common_ov7251_id1_evb.h"
#include "iq_param_sqnr1_ov7251_id1_evb.c"

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
    471, 533
};

static IQS_VIG_SET vig =
{
    VIG_TAB_GAIN_2X, {0, 0, 0, 0, 0, 0, 0, 3, 33, 69, 112, 168, 231, 313, 407, 511, 677}
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
	0x296a42a5, 0x283a0e94, 0x283a1e74, 0x2599b66a,
	0x25e98255, 0x24994e4b, 0x23b8f63f, 0x23c8c22c,
	0x2248762f, 0x21684e19, 0x20280206, 0x1fc7d600,
	0x1eb7a1ed, 0x1da771dd, 0x1ce749d5, 0x1c8729c7,
	0x1bd6f9c3, 0x1bd6f9bd, 0x1b26bdaf, 0x1a9691af,
	0x1a6695a1, 0x19d6819f, 0x19e6619e, 0x19766196,
	0x19464197, 0x18f64d90, 0x18b63991, 0x18e6298a,
	0x18a61d89, 0x18d6258a, 0x18a62986, 0x1876318b,
	0x18b6318b, 0x18b6318c, 0x18b63190, 0x1956318f,
	0x19065190, 0x1956519d, 0x19b6759b, 0x19b685a2,
	0x1a4689a7, 0x1b26a9b5, 0x1b56d5ba, 0x1b76edbf,
	0x1c2715c7, 0x1ce729cf, 0x1d4759db, 0x1e0791e8,
	0x1e57b1eb, 0x1fd7f206, 0x2088320c, 0x22784a10,
	0x2328a623, 0x2358c63a, 0x2439124a, 0x24b95a52,
	0x25997266, 0x2659be77, 0x2859d67b, 0x27ba0e89,
	0x283a328a, 0x298a2297, 0x2a1a7ea7, 0x2a9a7eb7,
	0x2b0ab6b5, 0x292a3297, 0x28ba1e83, 0x26d9ae64,
	0x2599725a, 0x25093652, 0x2448fa36, 0x2328d63f,
	0x22089a27, 0x21383a12, 0x20880a06, 0x2037d200,
	0x1e67a5ec, 0x1dc76de1, 0x1d5745d6, 0x1c9725cb,
	0x1c06f5c1, 0x1b76d1b0, 0x1b46b9ac, 0x1a6699aa,
	0x19f671a0, 0x19d6619c, 0x19664d95, 0x18d6418f,
	0x18c6358c, 0x18861d87, 0x18761d89, 0x18260d85,
	0x1806017e, 0x1815f982, 0x1825f580, 0x1826017e,
	0x1805f985, 0x1825fd82, 0x18260583, 0x18161186,
	0x18261d8b, 0x1896218d, 0x18b62d89, 0x19064194,
	0x19165596, 0x19a65d95, 0x1a467da1, 0x1a568da4,
	0x1a96adad, 0x1b76c5bb, 0x1c06f9c3, 0x1d2721cd,
	0x1ce739d7, 0x1db77de2, 0x1ec7a1ed, 0x1f27ce00,
	0x2027f605, 0x21385215, 0x22185a26, 0x2398c63e,
	0x24e8f64a, 0x24c92a44, 0x25f96651, 0x26d97264,
	0x2679b683, 0x2699de7e, 0x27ea1e79, 0x296a0e9b,
	0x29aa5ea5, 0x29fa9ea9, 0x27e9d66f, 0x26f9966c,
	0x25695258, 0x24e9263f, 0x2398da3b, 0x2248962b,
	0x21c87a1c, 0x21f8360c, 0x2057f9ff, 0x1f07c5e8,
	0x1e7785e0, 0x1da759d9, 0x1cb729cd, 0x1c6701c1,
	0x1bb6e1b8, 0x1aa6bdac, 0x1a9689a6, 0x19f66d9e,
	0x19a64997, 0x19064592, 0x18b6318a, 0x18b61989,
	0x18160d8a, 0x18260d84, 0x1815f17c, 0x17e5f17c,
	0x17b5e578, 0x1745d578, 0x1745d177, 0x1765d179,
	0x1725c175, 0x1785c574, 0x1755cd76, 0x1755d973,
	0x1735d579, 0x17b5e979, 0x17b5e97c, 0x17d5e97c,
	0x18660d82, 0x18961985, 0x18e62d8b, 0x1926358c,
	0x1966599c, 0x19b665a2, 0x1a1695a1, 0x1ae6adac,
	0x1b76d9b4, 0x1c2709c0, 0x1ce71dd1, 0x1d3751d6,
	0x1e2775e1, 0x1e87adf5, 0x1fc7ddfb, 0x20681606,
	0x21f84a14, 0x22f88e29, 0x2348ce32, 0x24091a3f,
	0x24e92a54, 0x25594a57, 0x26d97664, 0x2769aa79,
	0x27e9ea71, 0x28ba168c, 0x28ea7697, 0x27299677,
	0x26197667, 0x25692251, 0x2308f644, 0x22e8ba2c,
	0x22087e22, 0x21784e10, 0x208801fc, 0x1f57c5f5,
	0x1e8799e6, 0x1d8755d5, 0x1d472dcc, 0x1ca6f9c2,
	0x1bf6ddb4, 0x1a86a9a6, 0x1a068da4, 0x19c6719e,
	0x19364597, 0x19263d90, 0x18b6318e, 0x18661187,
	0x18460181, 0x17e5e977, 0x17b5e579, 0x1705d175,
	0x1735c975, 0x1755bd6e, 0x16e5c16d, 0x16f5b16e,
	0x16c5b56e, 0x16f5ad70, 0x16d5ad6d, 0x16d5b96d,
	0x16f5b170, 0x1715b170, 0x1705c176, 0x1725d577,
	0x1785dd77, 0x17c5e577, 0x17e5f983, 0x17f60183,
	0x18462187, 0x18f6298a, 0x1916419a, 0x19966596,
	0x1a2679a4, 0x1a96b1ab, 0x1b36ddb2, 0x1bb6e9bc,
	0x1c4719cc, 0x1d5735d3, 0x1df77dea, 0x1ed7c1ef,
	0x1fb7e201, 0x20481207, 0x21685618, 0x21888e27,
	0x2298c22d, 0x2398fa3d, 0x24590a52, 0x25696654,
	0x25c97660, 0x2659aa60, 0x27e9de79, 0x2859fa90,
	0x25c97e5b, 0x25393e47, 0x2449123d, 0x22c8be2f,
	0x21f8821f, 0x21f83613, 0x20580a06, 0x1f47c1f1,
	0x1e2791e9, 0x1dc759d9, 0x1c9725cf, 0x1c36f5c0,
	0x1b46cdaf, 0x1a9691a8, 0x1a467d9d, 0x19a65996,
	0x19364192, 0x18962d91, 0x18960d84, 0x18060d7f,
	0x17d5dd78, 0x1765d576, 0x1725c178, 0x1705b570,
	0x16a5b16d, 0x1695a16a, 0x16859966, 0x16559966,
	0x16459163, 0x16358565, 0x16458566, 0x16159568,
	0x16259562, 0x1675956a, 0x1655996a, 0x16759566,
	0x16b59d69, 0x16d5a970, 0x16e5c971, 0x1755cd73,
	0x17a5d577, 0x1785e57e, 0x18160584, 0x18861d8b,
	0x19162d90, 0x19764d9c, 0x19b675a0, 0x1aa6a1a5,
	0x1af6adb2, 0x1b86e1b9, 0x1c170dc9, 0x1cc741d2,
	0x1df779e2, 0x1e67a1f5, 0x1f17f1f2, 0x20280e09,
	0x20e85613, 0x22187624, 0x2278922c, 0x2338d63d,
	0x24390a44, 0x23c92a52, 0x24e97260, 0x26a9826c,
	0x2859be72, 0x26793e5e, 0x2488f64d, 0x23b8be3d,
	0x22687a30, 0x21284e13, 0x20180a03, 0x1fa7cdf3,
	0x1e87a5e6, 0x1db769d6, 0x1c872dcd, 0x1c06edc0,
	0x1b16c5b0, 0x1a868dad, 0x19c67d9e, 0x19265594,
	0x18d63591, 0x18661189, 0x18160584, 0x17c5fd7b,
	0x1765d57b, 0x1725c571, 0x16a5ad6d, 0x16a5a56a,
	0x16359168, 0x16258966, 0x16058562, 0x15f57d60,
	0x15c5715e, 0x15a57560, 0x15956d5d, 0x15d5655d,
	0x15a56d5d, 0x15f56d5c, 0x15c5715f, 0x15f5755f,
	0x15d57963, 0x16158d64, 0x16358969, 0x1695916a,
	0x1695a16b, 0x16c5b56f, 0x16f5c971, 0x1795e578,
	0x17f5f17f, 0x18660589, 0x18c6218e, 0x18f64599,
	0x19b6599a, 0x1a3691a8, 0x1a86a5ab, 0x1b26ddbc,
	0x1bf701c1, 0x1d0731d6, 0x1d8771db, 0x1e6789e8,
	0x1f47c1fa, 0x1fb7fa06, 0x2118420f, 0x21987620,
	0x2288c639, 0x23b8d23a, 0x24790a40, 0x24894e52,
	0x2499765e, 0x26f95a69, 0x24c91a4a, 0x2448ea42,
	0x22489626, 0x21185a26, 0x202819ff, 0x1f17ddf5,
	0x1e77b1eb, 0x1d9769d6, 0x1ce731cf, 0x1c46f9c4,
	0x1b16c5b4, 0x1a3691a6, 0x19e671a0, 0x1966419a,
	0x18d63188, 0x18261d86, 0x17d5f580, 0x1785e57c,
	0x1735cd76, 0x16c5b56d, 0x1665a56b, 0x16659969,
	0x16258161, 0x15c57561, 0x15c5695b, 0x15c55d5a,
	0x15555d55, 0x15955156, 0x15355155, 0x15454556,
	0x15454554, 0x15054953, 0x15354d55, 0x15554d53,
	0x15654956, 0x15555d59, 0x15855d5a, 0x1595755b,
	0x16057d60, 0x16058d69, 0x16358962, 0x1695ad70,
	0x16e5b16d, 0x1755d573, 0x1795e177, 0x1835fd82,
	0x1826218b, 0x18e6398e, 0x1946599b, 0x19d681a3,
	0x1a96b5ac, 0x1b46c5b8, 0x1c26f5c1, 0x1d4731cf,
	0x1d5761df, 0x1eb7adf2, 0x1ee7c5f8, 0x1fc80607,
	0x21484e19, 0x22086e26, 0x22e88a2d, 0x2358c632,
	0x2438e23d, 0x24392a52, 0x2599665b, 0x2528f63b,
	0x23f8d62d, 0x22387a1d, 0x21883a14, 0x2017ee00,
	0x1f17bdf1, 0x1ea771df, 0x1d4741cf, 0x1cb711c8,
	0x1b26ddb2, 0x1ad699ac, 0x1a36759e, 0x19a64997,
	0x18d62991, 0x18561184, 0x17f5f57f, 0x17b5dd77,
	0x1755d170, 0x16b5b56f, 0x16859966, 0x16758d62,
	0x15d57d5f, 0x1595695c, 0x15f5615b, 0x15a55557,
	0x15654956, 0x15054551, 0x15253d4e, 0x1505354c,
	0x1505354e, 0x14d5394f, 0x14e52d4c, 0x15153d4c,
	0x1515354d, 0x14f5394f, 0x15053950, 0x15454951,
	0x15955553, 0x1595655a, 0x15b5755f, 0x16257961,
	0x16358d65, 0x1695a969, 0x16e5b972, 0x1745cd72,
	0x17c5e57c, 0x18160182, 0x18761d88, 0x1906418f,
	0x19865d98, 0x1a068da2, 0x1ac69db4, 0x1bb6d9ba,
	0x1c3715c3, 0x1d4739d6, 0x1e0781dc, 0x1ec795ef,
	0x1fe7f1fc, 0x2028160c, 0x21084a15, 0x21787e25,
	0x2338923a, 0x23d8e634, 0x24392244, 0x2489125a,
	0x2398da35, 0x22f8c62a, 0x21f84619, 0x2067fe05,
	0x1f67c5ec, 0x1e6779e6, 0x1db751d7, 0x1c871dbf,
	0x1b76d9ba, 0x1a76b1a9, 0x1a267da4, 0x19665599,
	0x18b6398b, 0x18960185, 0x17f5f97d, 0x1775d978,
	0x16e5b56b, 0x16c5a56a, 0x16759165, 0x15f57d60,
	0x15b5755d, 0x15955d57, 0x15854953, 0x15354152,
	0x14c5394e, 0x14f5354d, 0x14a5294c, 0x14c52147,
	0x14751d48, 0x14c51547, 0x14851d47, 0x14551948,
	0x14451545, 0x14a51d47, 0x14951d49, 0x14752549,
	0x14752d48, 0x14c53950, 0x15053d4e, 0x15455155,
	0x15555d5b, 0x15a5655a, 0x15e58160, 0x16459167,
	0x16959d6a, 0x1715bd6f, 0x1765cd75, 0x1805fd7d,
	0x1896218a, 0x18e64191, 0x19965d9a, 0x1a1685a3,
	0x1b06a9b1, 0x1b96e5ba, 0x1c771dc6, 0x1d3759d6,
	0x1e477ddb, 0x1ef7b5ed, 0x1f87edfe, 0x2138160a,
	0x20d8561c, 0x22887e26, 0x22f8be2f, 0x2378f639,
	0x25f8f243, 0x23c8ba30, 0x2228921d, 0x20682605,
	0x2007d1fe, 0x1e9795f5, 0x1d7769d9, 0x1cb72dc9,
	0x1bd6f1ba, 0x1b26a5ad, 0x1a167da8, 0x19166197,
	0x18e6258c, 0x18861583, 0x17d5e179, 0x1765d176,
	0x16f5a571, 0x1675a567, 0x16258563, 0x15a56d5f,
	0x15d5615a, 0x15355156, 0x14e54150, 0x14d53551,
	0x14a5254a, 0x1465294a, 0x14651949, 0x14250544,
	0x14251144, 0x14250142, 0x14150541, 0x14050143,
	0x14250140, 0x1414fd40, 0x1404fd3e, 0x13e50542,
	0x14151143, 0x14350547, 0x14652147, 0x14b51949,
	0x14b52d4f, 0x14e54152, 0x15555557, 0x15756158,
	0x15c5795f, 0x16159164, 0x1675a56d, 0x16d5b96b,
	0x1755d175, 0x17d6017d, 0x18260587, 0x18e62993,
	0x1966659d, 0x1a3689a9, 0x1a86c1b2, 0x1bb6e9c3,
	0x1c6719d0, 0x1cd755d7, 0x1e3779e6, 0x1e87c1f5,
	0x1f87f608, 0x20a82a0e, 0x21983a1d, 0x2218a230,
	0x22e8c22f, 0x23c8ea40, 0x2268823a, 0x20e84a1d,
	0x20181200, 0x1e97cdf4, 0x1e3769e6, 0x1cc731d7,
	0x1c170dc2, 0x1b06b9b1, 0x1a3691a3, 0x198665a0,
	0x18c63593, 0x18560d85, 0x17d5f17f, 0x1705cd75,
	0x16f5b16f, 0x16459d67, 0x15f57d61, 0x15b56d5e,
	0x15355159, 0x15055156, 0x14f5394f, 0x1485214a,
	0x14551548, 0x14351146, 0x14350142, 0x1404f940,
	0x13d4ed3d, 0x1394e93f, 0x13d4e93e, 0x1394e53a,
	0x1374e53a, 0x1394e53b, 0x13b4e13b, 0x1394e13b,
	0x13c4e53d, 0x13d4f13c, 0x13c4fd41, 0x14050542,
	0x14451543, 0x14651547, 0x14b51d4c, 0x14f5314f,
	0x15054150, 0x1515515a, 0x15b57d5d, 0x15c57d63,
	0x1655996b, 0x16f5b970, 0x1765d575, 0x17f5e981,
	0x1826098c, 0x19064193, 0x1996559e, 0x1a1691a7,
	0x1ad6b9b1, 0x1b26edc3, 0x1c871dc8, 0x1d7759dd,
	0x1e9795eb, 0x1f17c9f5, 0x1ff7f610, 0x20583610,
	0x21787a1c, 0x21d88a30, 0x2358d22a, 0x22686a1c,
	0x21783e0c, 0x1f57e1f8, 0x1e5799e7, 0x1db765d9,
	0x1cb70dc7, 0x1b96d9b7, 0x1ad6a1ab, 0x19f6759d,
	0x18f65195, 0x18561d86, 0x17f5f581, 0x1765d174,
	0x1705a96e, 0x16859d66, 0x16458161, 0x15a57959,
	0x15956158, 0x15454551, 0x14b5354d, 0x14652149,
	0x14651545, 0x14350541, 0x13e4f93d, 0x13b4e53c,
	0x13b4f13e, 0x13a4e939, 0x13a4d939, 0x1374d939,
	0x1364d537, 0x1364cd36, 0x1344d136, 0x1344dd38,
	0x1374d534, 0x1364d939, 0x1374e138, 0x1394e53d,
	0x13c4e93b, 0x13d4f53e, 0x14050143, 0x14250946,
	0x14751d4a, 0x14e5294f, 0x14f53d51, 0x15555557,
	0x1595655b, 0x15f58162, 0x16859569, 0x16f5c172,
	0x1795d975, 0x17e5f181, 0x1886218f, 0x19363995,
	0x19a665a2, 0x1a669dab, 0x1b46cdb9, 0x1bf6f9ca,
	0x1ca741dc, 0x1e0769db, 0x1e67b1f3, 0x1f77c9f9,
	0x1fb80e0a, 0x21484a13, 0x21986e29, 0x22c8ae2a,
	0x21d84e02, 0x20f819ff, 0x1f37c1e6, 0x1e5785df,
	0x1d5739cb, 0x1c36f1bf, 0x1b46c1ad, 0x1a1675a2,
	0x1926519a, 0x18c6258a, 0x1805fd81, 0x1795d572,
	0x16e5c16d, 0x16759966, 0x16358963, 0x16056d5b,
	0x15555d55, 0x15353d50, 0x14752d4e, 0x14650d45,
	0x14450941, 0x14050141, 0x13d4f53a, 0x13a4dd3b,
	0x1394dd38, 0x1374d135, 0x1334d134, 0x1324cd32,
	0x1324bd31, 0x1314bd32, 0x1304c12f, 0x1314c131,
	0x1314b92f, 0x1314bd32, 0x1304bd31, 0x1314c931,
	0x1324cd33, 0x1354d135, 0x1384e13b, 0x13a4e93e,
	0x13e4f541, 0x14150543, 0x14551545, 0x14751d4b,
	0x14d5414d, 0x15254954, 0x15a5655c, 0x15c57560,
	0x1675996a, 0x16e5ad6e, 0x1775dd75, 0x17e5f984,
	0x1866258c, 0x18f64995, 0x19b685a4, 0x1a96a1ac,
	0x1be6c9bd, 0x1c171dc3, 0x1d7749d3, 0x1e3771e6,
	0x1eb7bdf0, 0x1f47e9fb, 0x20081a0b, 0x21984e18,
	0x2288661c, 0x21682204, 0x1fd7f5f6, 0x1e67a1e6,
	0x1d574de2, 0x1c4715c3, 0x1af6cdb7, 0x1a5691ab,
	0x19966197, 0x1916418d, 0x18760d85, 0x1795dd7c,
	0x16f5bd6e, 0x16b5a568, 0x16558563, 0x15e56558,
	0x15655955, 0x1515454d, 0x14c52549, 0x14951145,
	0x14350143, 0x13f4f141, 0x13d4e138, 0x1384e137,
	0x1344d136, 0x1324c133, 0x1324bd2e, 0x12f4b52e,
	0x12e4b12e, 0x12d4ad2c, 0x12c4a92b, 0x12d4ad2b,
	0x12d4a52b, 0x12c4a12c, 0x12a4a92b, 0x12d4ad2c,
	0x12c4b52d, 0x12d4b52e, 0x1324c131, 0x1334c534,
	0x1354d937, 0x1374e138, 0x13a4f53f, 0x14050543,
	0x14351145, 0x14851d4c, 0x14d5354f, 0x15355554,
	0x15655d5c, 0x15e58161, 0x16359d6b, 0x16e5b973,
	0x1765e57b, 0x18160181, 0x1876218d, 0x1986519b,
	0x1a0685a4, 0x1ad6b1b2, 0x1bb6e1c3, 0x1c6725cd,
	0x1d4755d9, 0x1e078de6, 0x1eb7c9f7, 0x1f87e202,
	0x20883212, 0x21885e26, 0x1fe7ea00, 0x1f47d1f7,
	0x1de76ddf, 0x1cf735cd, 0x1bc6edbe, 0x1ac6b9ac,
	0x1a0665a0, 0x19264d96, 0x18561587, 0x17c5e97e,
	0x1755d573, 0x16b5ad6a, 0x16359163, 0x15e56d5b,
	0x15555557, 0x15253551, 0x14a5254c, 0x14551147,
	0x14050542, 0x13d4f13d, 0x13b4e538, 0x1364cd36,
	0x1314cd33, 0x1304bd32, 0x12e4b530, 0x12e4ad2d,
	0x12a4a92b, 0x1294a12a, 0x12749527, 0x12848d25,
	0x12749526, 0x12549526, 0x12649527, 0x12449524,
	0x12749928, 0x12849929, 0x12a4a929, 0x12b4b12a,
	0x12e4b530, 0x12e4c130, 0x1314c937, 0x1344d938,
	0x13b4e93d, 0x13c4f941, 0x14251141, 0x14752549,
	0x14f53551, 0x1535515a, 0x15756d5a, 0x16058964,
	0x1665a56d, 0x16d5c173, 0x17a5ed7c, 0x18160d85,
	0x18a63195, 0x199665a0, 0x1a4689ac, 0x1b26c5b6,
	0x1bb70dc5, 0x1cc745d2, 0x1d9771dd, 0x1e67a9ec,
	0x1ed7d9f8, 0x20380207, 0x20e8360e, 0x1fe7c204,
	0x1f07b1f7, 0x1d4755dd, 0x1cb70dca, 0x1b26e1b7,
	0x1a768da6, 0x19a65d9c, 0x18f63189, 0x18460981,
	0x17b5d17b, 0x16d5bd6f, 0x1655916a, 0x15d5795e,
	0x15855d5a, 0x15554154, 0x14b5354d, 0x14852147,
	0x14550140, 0x13d4f541, 0x13b4e13b, 0x1364d536,
	0x1304c534, 0x1324b531, 0x12c4b12d, 0x1294a12c,
	0x12849d2a, 0x12749528, 0x12549924, 0x12448924,
	0x12348d25, 0x12348924, 0x12449125, 0x12348523,
	0x12448d23, 0x12348d24, 0x12549126, 0x12649129,
	0x12749d29, 0x12a4a52a, 0x12c4b130, 0x12d4bd30,
	0x1354c935, 0x1354dd39, 0x13a4ed3c, 0x1404f941,
	0x14451144, 0x14a5214e, 0x15054553, 0x15455159,
	0x15957160, 0x16258d69, 0x16c5a96f, 0x1765d576,
	0x17d5f57b, 0x1846258a, 0x19164999, 0x19a679a6,
	0x1ac6adab, 0x1b56f1bf, 0x1c771dc5, 0x1d675dde,
	0x1dc779f2, 0x1f27b5f9, 0x1f47ea07, 0x2057f60b,
	0x1f57c1ef, 0x1eb78de4, 0x1cc739d2, 0x1c16f1c0,
	0x1ad6b5ae, 0x1a267da5, 0x19264996, 0x18862185,
	0x17d5f57c, 0x1725c171, 0x1685a56a, 0x16358d62,
	0x15956d5c, 0x15654d51, 0x14e5354d, 0x14b51d49,
	0x14450942, 0x13d4f93d, 0x1384dd3a, 0x1374d136,
	0x1314c132, 0x12d4b92d, 0x12e4a92d, 0x1294a12b,
	0x12749928, 0x12449126, 0x12348524, 0x12448120,
	0x12047d20, 0x11f47920, 0x11f47520, 0x11f4751d,
	0x11f47d1e, 0x11f47920, 0x12148120, 0x12047d24,
	0x12348d22, 0x12448d26, 0x12549529, 0x1264a129,
	0x12b4a92b, 0x12c4bd30, 0x1324c932, 0x1374d939,
	0x13b4e53d, 0x14250140, 0x14250947, 0x1495354d,
	0x15054552, 0x1565655a, 0x15f58166, 0x1655996a,
	0x1715bd70, 0x1765e579, 0x18061984, 0x18e63d8d,
	0x19a66199, 0x1a5695a9, 0x1b56c5b2, 0x1c2701c5,
	0x1cb73dd2, 0x1da769e8, 0x1ee7a1f1, 0x1f37edf7,
	0x1fe7fa02, 0x1f17a1e7, 0x1e5769dd, 0x1ce719ca,
	0x1bf6e1b6, 0x1a769daa, 0x19965d96, 0x18c6398b,
	0x1825f17f, 0x1755d577, 0x16e5b16d, 0x16359163,
	0x1605715b, 0x15a55155, 0x1505414f, 0x14a51948,
	0x14651542, 0x13e4f53d, 0x13a4e13a, 0x1364d536,
	0x1324c930, 0x12d4b52d, 0x12a4ad2b, 0x12749927,
	0x12449125, 0x12448523, 0x12048521, 0x1204791f,
	0x11e4711d, 0x11d4711c, 0x11e46d1b, 0x11a4691a,
	0x1194691d, 0x11946d19, 0x11b46d1b, 0x11c4751c,
	0x11e4751d, 0x11d4791e, 0x12048121, 0x12248d22,
	0x12548d26, 0x12749d27, 0x12c4a52d, 0x1314bd2f,
	0x1314c936, 0x1374d938, 0x13b4e53c, 0x1424fd41,
	0x14751d48, 0x14e52d50, 0x15554152, 0x15a5695b,
	0x15d58564, 0x1695a56d, 0x1715cd75, 0x17c5ed82,
	0x1876198e, 0x19264994, 0x19e6719d, 0x1a56b5b0,
	0x1b86e5be, 0x1c8701cd, 0x1d4745de, 0x1de789e5,
	0x1e57adf6, 0x1fc7d605, 0x1db775e6, 0x1d674dd5,
	0x1c370dc3, 0x1b46c9ad, 0x19c6919a, 0x19964d92,
	0x18962589, 0x17e5e97e, 0x1725c172, 0x16c59d6c,
	0x16058160, 0x15c55d56, 0x15154552, 0x14b5254d,
	0x14650d44, 0x1434fd40, 0x13c4e93b, 0x1374d934,
	0x1304c133, 0x12f4b92c, 0x12a4a128, 0x12749128,
	0x12148924, 0x12148121, 0x11e4751d, 0x11c4711d,
	0x11946d1a, 0x11a4611a, 0x1184611a, 0x11946118,
	0x11745d18, 0x11745917, 0x11745d16, 0x11846118,
	0x11945d18, 0x11a4651a, 0x11e4651b, 0x11d4711c,
	0x11e47521, 0x12048122, 0x12349125, 0x12649929,
	0x12a4a52c, 0x12f4b931, 0x1354bd33, 0x1364dd38,
	0x13d4ed3c, 0x14150542, 0x14751d49, 0x1505394d,
	0x15555559, 0x16156d63, 0x15f59964, 0x16b5b970,
	0x1745e17b, 0x17f60d85, 0x18b6318e, 0x1986559d,
	0x19f689ac, 0x1b06b5b2, 0x1bb709c5, 0x1c9735d3,
	0x1d7759e0, 0x1e77a1ea, 0x1f17e9f9, 0x1dc765e1,
	0x1d1745ce, 0x1c46fdc0, 0x1ad6a9ad, 0x19e679a1,
	0x1906398f, 0x18260986, 0x1775dd79, 0x16c5b56f,
	0x16258d64, 0x15d57162, 0x15355553, 0x1515354d,
	0x14851d4b, 0x14050142, 0x13d4ed3c, 0x1384e138,
	0x1314cd35, 0x12c4b531, 0x12b4a929, 0x12749926,
	0x12448d23, 0x12148122, 0x11e4791e, 0x11b46d1d,
	0x11b4611b, 0x11945d1a, 0x11645918, 0x11545516,
	0x11545117, 0x11645917, 0x11445117, 0x11445514,
	0x11445516, 0x11645918, 0x11745d18, 0x11645d18,
	0x11b4611a, 0x11c46d1d, 0x11e4751f, 0x12348124,
	0x12348d27, 0x1274992a, 0x12d4a92d, 0x12f4b931,
	0x1334c936, 0x13b4e53b, 0x13f4f142, 0x14251546,
	0x14b5314c, 0x15254155, 0x15656559, 0x16458565,
	0x1685a96d, 0x16f5c977, 0x1795f57e, 0x18a6218d,
	0x19264196, 0x19b681a5, 0x1ae6b1ac, 0x1be6f9c7,
	0x1c4725d2, 0x1d4761da, 0x1db7a5e8, 0x1eb7b5ed,
	0x1dc751d8, 0x1cb71dc9, 0x1bc6e1b9, 0x1a76a5ab,
	0x1966599a, 0x1896258d, 0x1815f97e, 0x1725c576,
	0x16b5a569, 0x16158163, 0x15c5615f, 0x15254555,
	0x14a52d49, 0x14650946, 0x1414f93e, 0x13b4e139,
	0x1344d136, 0x12e4c12f, 0x12a4a92c, 0x12849d29,
	0x12448924, 0x12147920, 0x1204711f, 0x11c4691a,
	0x1184611a, 0x11745518, 0x11645516, 0x11545115,
	0x11144d13, 0x11244912, 0x11244512, 0x11044513,
	0x11244112, 0x11344913, 0x11444d15, 0x11344d15,
	0x11445116, 0x11545516, 0x1194611b, 0x11c46d1c,
	0x11d4751d, 0x12248521, 0x12549125, 0x12749d2a,
	0x12c4b12d, 0x1314bd34, 0x1384dd37, 0x1394ed3f,
	0x14250142, 0x14651d49, 0x14e53152, 0x1555515b,
	0x15d57962, 0x16359969, 0x16c5bd77, 0x1725d97b,
	0x17e6018a, 0x18e63591, 0x199669a2, 0x1a5699ad,
	0x1b26d5be, 0x1c3719ca, 0x1d2735da, 0x1d9781e6,
	0x1f17bdf3, 0x1d0765d1, 0x1d0715c2, 0x1b36d1b1,
	0x1a5691a0, 0x19365994, 0x18661d88, 0x1785f57c,
	0x1715bd6d, 0x16759565, 0x16056d5e, 0x15a55557,
	0x14c5314f, 0x14751d49, 0x14150941, 0x13d4f13b,
	0x1354d934, 0x1334c131, 0x12b4b12d, 0x1284a526,
	0x12648d25, 0x12048122, 0x11d4711d, 0x11b4651b,
	0x1174651a, 0x11845516, 0x11545116, 0x11344d13,
	0x11144512, 0x11144111, 0x1104410f, 0x10f43d11,
	0x11143d0e, 0x10f4410f, 0x11043910, 0x11044511,
	0x11144511, 0x11244913, 0x11345114, 0x11745917,
	0x11846118, 0x11d46d1b, 0x11f4791f, 0x12248922,
	0x12549527, 0x12b4a52b, 0x12d4b530, 0x1324c934,
	0x1384dd3c, 0x13d4f93d, 0x14351545, 0x14b5294c,
	0x15454951, 0x15b5615a, 0x16158561, 0x1675ad69,
	0x1705cd79, 0x17f5f97f, 0x18a6258a, 0x19165d95,
	0x1a368d9f, 0x1ad6c9b7, 0x1bf6f1ca, 0x1c874dd0,
	0x1d5769df, 0x1e9779e0, 0x1d6729cc, 0x1c6701bd,
	0x1af6c1ad, 0x1a1675a0, 0x1936498e, 0x18261185,
	0x1795dd79, 0x16b5a96a, 0x16158964, 0x15a56559,
	0x15354551, 0x14e5254a, 0x14451543, 0x13f4f93d,
	0x1394e53a, 0x1344c535, 0x12e4b12f, 0x12c49d27,
	0x12649124, 0x12048521, 0x11f4791e, 0x11a4651b,
	0x11746117, 0x11745515, 0x11344d13, 0x11244512,
	0x10f43912, 0x11043d10, 0x10f4350e, 0x10c4350e,
	0x10e42d0d, 0x10d4390f, 0x10e4310d, 0x10d4310d,
	0x10f4390d, 0x11043d11, 0x11044111, 0x11344512,
	0x11444913, 0x11545916, 0x11845d19, 0x1194691c,
	0x1204751f, 0x12348d22, 0x12749927, 0x12a4ad2c,
	0x1324b930, 0x1344d137, 0x13a4e93b, 0x14250541,
	0x14351d4a, 0x14f5394f, 0x15355155, 0x15f5815f,
	0x16759d69, 0x16c5c96e, 0x17a5e97d, 0x18660d83,
	0x18e64995, 0x1a16759f, 0x1a66a9ae, 0x1bb6ddb9,
	0x1ca719cc, 0x1d474ddc, 0x1de78de3, 0x1c8741c8,
	0x1c06f5bc, 0x1ad6a9ac, 0x19e6699a, 0x1916398f,
	0x1805f97d, 0x1765c973, 0x16e59969, 0x16257162,
	0x15955d5b, 0x15153552, 0x14851947, 0x14050944,
	0x13b4ed3b, 0x1374d936, 0x1304c531, 0x12d4b52c,
	0x1274a126, 0x12349125, 0x11f4811f, 0x11e46d1b,
	0x11945d19, 0x11545517, 0x11245114, 0x11344511,
	0x11143d0e, 0x10f43d0f, 0x10d4350e, 0x10e42d0e,
	0x10a4310d, 0x10c4290c, 0x10b4310c, 0x10d4310c,
	0x10c4310d, 0x10b4390d, 0x10d4350d, 0x10e43910,
	0x11143911, 0x11244113, 0x11345114, 0x11545918,
	0x1194611b, 0x11d46d1e, 0x12148123, 0x12448d27,
	0x12a4a52b, 0x12d4b930, 0x1324d534, 0x13a4e13d,
	0x13e50140, 0x14650d46, 0x14d52d51, 0x15655156,
	0x15b5715f, 0x16659965, 0x16d5b570, 0x1745e97c,
	0x18160988, 0x18d64193, 0x19966d9d, 0x1a76a1ad,
	0x1b96ddb8, 0x1c6705c9, 0x1d4751d9, 0x1dc75ddf,
	0x1c7701d2, 0x1b36d9bb, 0x1a9691aa, 0x19666199,
	0x18661d89, 0x17f5e57e, 0x1725c171, 0x16459564,
	0x1605715c, 0x15255555, 0x14b53151, 0x14651144,
	0x13d4fd3f, 0x1394e138, 0x1334c533, 0x12e4b12d,
	0x1294a52b, 0x12449123, 0x12048120, 0x11b4751d,
	0x11846519, 0x11745117, 0x11244d14, 0x11044111,
	0x10f43910, 0x10d4310e, 0x10c4310d, 0x10b42d0a,
	0x10b4250b, 0x1094250a, 0x10942909, 0x10c4250a,
	0x1084250a, 0x10a4210a, 0x10b4290c, 0x10a4310b,
	0x10d4310c, 0x10e4350f, 0x11044111, 0x11144512,
	0x11344d15, 0x11746118, 0x11a4691d, 0x11c47920,
	0x12148924, 0x1274a128, 0x12b4a92d, 0x1304c132,
	0x1354dd39, 0x13b4ed3c, 0x13e50945, 0x1495294a,
	0x15253d52, 0x15a5655b, 0x16258563, 0x1695ad71,
	0x1725d577, 0x17c5f17e, 0x18562d8d, 0x19165194,
	0x1a2699a6, 0x1af6bdb5, 0x1bf705c6, 0x1c9741d6,
	0x1d4779e5, 0x1bf715c1, 0x1b86d5b4, 0x1a469da1,
	0x19264997, 0x18760989, 0x17c5d579, 0x16b5b16c,
	0x16058d63, 0x15a55d5d, 0x15154953, 0x14a5294a,
	0x14550944, 0x13b4f13f, 0x1364d537, 0x1304c131,
	0x12b4b12c, 0x12749d27, 0x12148921, 0x11f47d1e,
	0x11b4691a, 0x11845d19, 0x11245115, 0x11244513,
	0x10d43910, 0x10d4310f, 0x10a4290b, 0x1094290b,
	0x1084210b, 0x10942109, 0x10841d0a, 0x10842108,
	0x10741d08, 0x10742108, 0x10941d09, 0x1094210a,
	0x10a4290b, 0x10c4290a, 0x10c4350d, 0x10f4390f,
	0x10f44111, 0x11244514, 0x11645917, 0x11845d1b,
	0x11c46d1e, 0x12047d22, 0x12448d25, 0x1294a12a,
	0x12e4b92d, 0x1314c935, 0x13b4e13d, 0x13f50141,
	0x14851549, 0x14d53552, 0x1535595a, 0x15f57d64,
	0x1675996f, 0x1715c175, 0x1775e980, 0x18262d88,
	0x19163995, 0x19e67da1, 0x1ad6b9b4, 0x1bf701c4,
	0x1ca729d2, 0x1d774de0, 0x1bc6e9be, 0x1b56c1af,
	0x19e6699d, 0x18e63d90, 0x18760581, 0x1775d173,
	0x16e5a969, 0x1645855e, 0x15855557, 0x1515414f,
	0x14651d49, 0x14250141, 0x13b4e53b, 0x1364cd35,
	0x1304bd2f, 0x12b4a52b, 0x12848926, 0x12348920,
	0x11e4711b, 0x11945919, 0x11745113, 0x11244912,
	0x10f43d10, 0x10f4350f, 0x10c42d0c, 0x10a42909,
	0x10842507, 0x10741908, 0x10741d07, 0x10841d06,
	0x10841505, 0x10741507, 0x10641508, 0x10841507,
	0x10941908, 0x10942109, 0x10a4250a, 0x10b42d0b,
	0x10c4350d, 0x10d4390f, 0x11244511, 0x11444d14,
	0x1174591a, 0x11b4691a, 0x11e47d20, 0x12148523,
	0x12849d27, 0x12b4ad2c, 0x1314bd36, 0x1374d938,
	0x13e4f13f, 0x14350945, 0x14b5314b, 0x15354d58,
	0x15a57560, 0x16359566, 0x16f5bd6d, 0x1795dd7a,
	0x18661d86, 0x19263592, 0x19966d9d, 0x1a66b1ad,
	0x1b96e1b9, 0x1c1721cf, 0x1d6745d6, 0x1bd6ddb4,
	0x1af6b5ae, 0x19c6719c, 0x1906418d, 0x18360d7d,
	0x1765c974, 0x16b59d66, 0x16358561, 0x15755154,
	0x15052d4f, 0x14551146, 0x1414f940, 0x13b4e13c,
	0x1364c935, 0x12f4b92e, 0x1294a129, 0x12648925,
	0x11f48121, 0x11b4711a, 0x11945d19, 0x11544d16,
	0x11244511, 0x10d43d0d, 0x10d42d0d, 0x10b4290a,
	0x10942108, 0x10842107, 0x10641906, 0x10641905,
	0x10541506, 0x10641505, 0x10641505, 0x10641505,
	0x10641504, 0x10641908, 0x10742107, 0x1094210b,
	0x10c4290a, 0x10a42d0b, 0x10e43110, 0x11043d0e,
	0x11444915, 0x11545517, 0x11a4651b, 0x11c4751e,
	0x12248522, 0x12749128, 0x12d4ad2c, 0x1304c935,
	0x1364d937, 0x13b4f53c, 0x14450947, 0x14b5314c,
	0x15354156, 0x15a57961, 0x16358d65, 0x16c5b570,
	0x1755d97b, 0x18460986, 0x18f63d8f, 0x1996719d,
	0x1a96a9ae, 0x1b36ddb5, 0x1c371dc5, 0x1d1741cd,
	0x1b66e1b6, 0x1ab6a9ac, 0x19865d9c, 0x1876298a,
	0x17e6017d, 0x16f5c170, 0x16559968, 0x15c56d5c,
	0x15354151, 0x14b52d4a, 0x14451145, 0x13d4ed3d,
	0x13b4dd37, 0x1324c131, 0x12d4b52b, 0x12849926,
	0x12448923, 0x1204791e, 0x1184691b, 0x11745d16,
	0x11244914, 0x11043d0f, 0x10e4310d, 0x10c4310c,
	0x10a42509, 0x10741d07, 0x10941906, 0x10641506,
	0x10441106, 0x10441106, 0x10441104, 0x10540d04,
	0x10641106, 0x10641507, 0x10541907, 0x10541d08,
	0x10742108, 0x10942109, 0x10b4250a, 0x10d4310e,
	0x1104390f, 0x11244912, 0x11345116, 0x11846519,
	0x11b4751d, 0x11f48120, 0x12649927, 0x12b4a52c,
	0x12e4bd30, 0x1344d934, 0x13c4f13d, 0x14351145,
	0x14a52d4d, 0x14f53d53, 0x1585655e, 0x16157d67,
	0x1675a572, 0x1745d579, 0x18060583, 0x18a63991,
	0x1966719c, 0x1a568dac, 0x1b36cdb6, 0x1c3701ca,
	0x1cf735d9, 0x1ad6b9b2, 0x1aa69da9, 0x19a65995,
	0x18e61d86, 0x17d5f178, 0x16f5b96f, 0x16559965,
	0x15a5695b, 0x15254552, 0x14d5314b, 0x14550d44,
	0x13c4ed3d, 0x1364d937, 0x12d4b932, 0x12a4a92b,
	0x12549927, 0x11f48522, 0x11e4711c, 0x11945d18,
	0x11445115, 0x11244110, 0x10d4390f, 0x10e4310d,
	0x1094290a, 0x10942109, 0x10741908, 0x10641906,
	0x10441105, 0x10340906, 0x10340904, 0x10340905,
	0x10440d05, 0x10441104, 0x10340d04, 0x10441103,
	0x10541906, 0x10841d07, 0x1074210a, 0x10a4290a,
	0x10b42d0c, 0x10d4350f, 0x11044111, 0x11245515,
	0x11745d18, 0x11b46d1d, 0x11e47520, 0x12349126,
	0x1284a52d, 0x12e4c130, 0x1334d137, 0x1394e93d,
	0x13f4fd43, 0x14551d4e, 0x14d53552, 0x15a5615e,
	0x15f57966, 0x1685a56b, 0x1715cd79, 0x18160584,
	0x18963591, 0x1946659a, 0x19c68dad, 0x1ac6c9b7,
	0x1c1701ca, 0x1ce729d3, 0x1af6b1ae, 0x1a768da3,
	0x19865192, 0x18861588, 0x1795e578, 0x16e5b16c,
	0x16259162, 0x1575615b, 0x15054150, 0x14951d4c,
	0x14250142, 0x13d4e53b, 0x1354d134, 0x1304b931,
	0x1274a528, 0x12548d24, 0x12047d20, 0x11d4691d,
	0x11945d18, 0x11445113, 0x1114410f, 0x10d4350f,
	0x10c4250c, 0x1094250a, 0x10641907, 0x10541506,
	0x10541104, 0x10540d04, 0x10440d03, 0x10340d02,
	0x10340902, 0x10240d03, 0x10240905, 0x10340d03,
	0x10341104, 0x10441105, 0x10441506, 0x10741d08,
	0x10842109, 0x10a42d0b, 0x10c42d0d, 0x11043d11,
	0x11344d14, 0x11445517, 0x1184691a, 0x11e47920,
	0x12148929, 0x1274a92a, 0x12c4b531, 0x1354c935,
	0x1374e13d, 0x1404fd45, 0x14451945, 0x14e5354f,
	0x15755d5a, 0x15b57962, 0x1695a56b, 0x1745cd74,
	0x1815f580, 0x18b6258b, 0x19665996, 0x1a0689a6,
	0x1aa6c9b7, 0x1c170dc3, 0x1ce735d9, 0x1af6b1ac,
	0x1a7691a1, 0x19564d99, 0x18962187, 0x17e5e179,
	0x16e5ad6a, 0x16458560, 0x15b55d58, 0x15153d4f,
	0x14951947, 0x1435053e, 0x1394e539, 0x1344cd34,
	0x12d4bd2e, 0x12a49d28, 0x12248925, 0x11d47d1e,
	0x11b46917, 0x11945916, 0x11544914, 0x10f43d12,
	0x10e4350c, 0x10c4290c, 0x10941d0a, 0x10841906,
	0x10641105, 0x10540d05, 0x10440905, 0x10440903,
	0x10340902, 0x10340901, 0x10340d03, 0x10340902,
	0x10440903, 0x10240d03, 0x10440d05, 0x10641906,
	0x10741505, 0x10941d09, 0x10a4250b, 0x10c4350b,
	0x10e43510, 0x11144d12, 0x11545516, 0x1184651a,
	0x11d4791e, 0x12248526, 0x1264a12a, 0x12c4b52e,
	0x1344cd35, 0x13a4e13b, 0x13f4fd43, 0x14851548,
	0x15053d4f, 0x15a55958, 0x15f57d5e, 0x1665a169,
	0x1715c975, 0x1815f97f, 0x1886218a, 0x19165598,
	0x1a4689a2, 0x1b66a9b4, 0x1c36fdbd, 0x1d3731cd,
	0x1b46c1ac, 0x1a6689a1, 0x19663d92, 0x18362183,
	0x1775e577, 0x16d5ad6b, 0x16258160, 0x15755959,
	0x14d5354d, 0x14851d47, 0x13f50141, 0x1394e53a,
	0x1354c933, 0x12c4a92c, 0x12849d29, 0x12448522,
	0x11e4751e, 0x11b4611a, 0x11845516, 0x11344111,
	0x1104390f, 0x10d4310b, 0x10a4250b, 0x10941d09,
	0x10641905, 0x10641103, 0x10340d05, 0x10440d03,
	0x10340102, 0x10140501, 0x10240502, 0x10340502,
	0x10140502, 0x10240904, 0x10440904, 0x10640d05,
	0x10541105, 0x10741906, 0x10941d08, 0x10842909,
	0x10d42d0c, 0x1104390e, 0x11244912, 0x11645916,
	0x11846119, 0x11f4711d, 0x12148925, 0x12549d29,
	0x12d4b12d, 0x1324c133, 0x1394e53b, 0x13f4f940,
	0x14651948, 0x14e5354f, 0x15755556, 0x15d57960,
	0x1695916b, 0x1725d973, 0x17c5f17e, 0x18662190,
	0x1946519a, 0x1a1681a4, 0x1af6cdb0, 0x1bf701bf,
	0x1ce71dca, 0x1aa69dad, 0x1a267da4, 0x1906498b,
	0x18460d83, 0x1765cd73, 0x16959d69, 0x15d58960,
	0x15855553, 0x14e52d4e, 0x14551544, 0x1414fd3f,
	0x1374d938, 0x1334c932, 0x12c4b12a, 0x12749d26,
	0x12348523, 0x11b4751d, 0x11a46519, 0x11545115,
	0x11244912, 0x10e43911, 0x10b42d0c, 0x1094290a,
	0x10941d07, 0x10641108, 0x10341104, 0x10440503,
	0x10340904, 0x10240902, 0x10240101, 0x10040502,
	0x10240501, 0x10140500, 0x10140901, 0x10240503,
	0x10440905, 0x10541107, 0x10641506, 0x10841d09,
	0x10842509, 0x10c4310d, 0x10f4390d, 0x10f44511,
	0x11545115, 0x1174611a, 0x12047520, 0x12148525,
	0x12649526, 0x12b4b12e, 0x1314c534, 0x1374e13b,
	0x13e4f93e, 0x1455154a, 0x14c5354e, 0x15255559,
	0x15e57d5f, 0x1675a169, 0x1705b571, 0x1795ed7e,
	0x1876198e, 0x19865197, 0x19c68da8, 0x1b16c5b8,
	0x1c06e5c5, 0x1cb729d2, 0x1b06a1ac, 0x1a167da1,
	0x19063191, 0x18160584, 0x1785d975, 0x16b5a96d,
	0x16357d60, 0x15455556, 0x14e5394c, 0x14751546,
	0x1414f93f, 0x1374d93a, 0x1324c133, 0x12c4ad2b,
	0x12549928, 0x12148524, 0x11e47120, 0x11a4611a,
	0x11645116, 0x11044113, 0x10f43d0f, 0x10c4290e,
	0x10b4290a, 0x10841d08, 0x10641505, 0x10440d05,
	0x10340901, 0x10240901, 0x10240102, 0x10140101,
	0x10040100, 0x10140103, 0x10140103, 0x10140503,
	0x10240903, 0x10440d06, 0x10541105, 0x10641908,
	0x10641d0a, 0x1094250b, 0x10c42d0d, 0x10d4390f,
	0x11244511, 0x11344d16, 0x1194611b, 0x11c4711f,
	0x12148522, 0x12649d2a, 0x12d4b52f, 0x1324c934,
	0x1364e13a, 0x13d4fd42, 0x14551d46, 0x14d53550,
	0x15355557, 0x15e5815c, 0x16459d6a, 0x16e5bd72,
	0x17b5f584, 0x1866218e, 0x19265595, 0x1a1699a5,
	0x1af6cdb8, 0x1c26f1c4, 0x1ce735d6, 0x1ac6a1a8,
	0x1a06819c, 0x19463d91, 0x1836017e, 0x1795cd73,
	0x16c5a569, 0x15e57d5e, 0x15754955, 0x14e5394c,
	0x14650d46, 0x13e4ed3d, 0x1374d936, 0x1314c531,
	0x12d4ad2e, 0x12749527, 0x1234891e, 0x11e4711d,
	0x11845d17, 0x11445515, 0x11244511, 0x10f4390e,
	0x10b4290c, 0x10942508, 0x10741d07, 0x10641105,
	0x10541105, 0x10340902, 0x10240502, 0x10240900,
	0x10340500, 0x10040101, 0x10240100, 0x10140501,
	0x10240902, 0x10340d03, 0x10440d03, 0x10341505,
	0x10641106, 0x10741909, 0x1094250a, 0x10b4290c,
	0x10f4390f, 0x11144512, 0x11545515, 0x1184611b,
	0x11e4711e, 0x12348923, 0x12649926, 0x12c4b12c,
	0x1324c534, 0x1374e139, 0x13d4f541, 0x14351547,
	0x14d5394f, 0x15555556, 0x15e57960, 0x16359968,
	0x1745b973, 0x17c5e97b, 0x1886258a, 0x1966499b,
	0x1a26899f, 0x1ad6c5b4, 0x1bd709c3, 0x1c4729d3,
	0x1aa699a5, 0x1a2679a2, 0x1916398d, 0x1825fd86,
	0x1755d971, 0x1695a569, 0x15f5755c, 0x15754d53,
	0x14c52d4d, 0x14650d44, 0x13f4f140, 0x1374d134,
	0x1324c132, 0x12c4ad2c, 0x12749926, 0x12248522,
	0x11e4791d, 0x11a45d1a, 0x11445114, 0x11344d11,
	0x1124390f, 0x10c4310e, 0x10b4250a, 0x10842506,
	0x10541906, 0x10640d03, 0x10340d02, 0x10240502,
	0x10240502, 0x10140500, 0x10140502, 0x10140502,
	0x10140502, 0x10240902, 0x10240903, 0x10340d04,
	0x10640d07, 0x10741507, 0x10841d08, 0x10b4290a,
	0x10c42d0d, 0x10e44110, 0x11344513, 0x11545515,
	0x11b4691a, 0x11f4711f, 0x12348924, 0x12649d28,
	0x12c4ad30, 0x1314c536, 0x1374e53b, 0x13e50141,
	0x14751549, 0x14c53950, 0x15655d57, 0x16057561,
	0x1675a969, 0x1735c973, 0x17d60185, 0x18862d8d,
	0x1966599e, 0x1a0689a4, 0x1af6b9b1, 0x1c26f9c5,
	0x1d0735d2, 0x1ab68da8, 0x1a36699b, 0x18d63189,
	0x1805f981, 0x1755c572, 0x16859566, 0x1615715a,
	0x15954d54, 0x14c5294c, 0x14750d43, 0x13e4f13b,
	0x1364e137, 0x1334bd31, 0x12e4a52b, 0x12949525,
	0x12448123, 0x11e47121, 0x11b46518, 0x11545116,
	0x11244511, 0x10f4310f, 0x10d4310d, 0x10b42109,
	0x10841d08, 0x10641505, 0x10441103, 0x10340d03,
	0x10240501, 0x10040501, 0x10140101, 0x10240502,
	0x10140101, 0x10340502, 0x10340102, 0x10340d03,
	0x10340904, 0x10741104, 0x10741908, 0x10741d0a,
	0x10b4290a, 0x10d4310b, 0x11043911, 0x11145111,
	0x11645915, 0x11b4651a, 0x11c4791f, 0x12348925,
	0x1284992b, 0x12c4b530, 0x1314cd34, 0x1374e53a,
	0x13d50141, 0x14951d45, 0x14c53954, 0x15755d57,
	0x15d58560, 0x1695a16a, 0x1765d178, 0x1826017f,
	0x18d62590, 0x19965999, 0x1a9699a4, 0x1b46c5b2,
	0x1bf6f9c2, 0x1c7739cf, 0x1a66a1a4, 0x19e67598,
	0x18d62991, 0x1815fd81, 0x1755d573, 0x16859d6b,
	0x16056d63, 0x15655552, 0x14d52d49, 0x14451547,
	0x13f4f13e, 0x1374d938, 0x1344c932, 0x12b4ad2b,
	0x12749d27, 0x12348523, 0x11c4751d, 0x1184611a,
	0x11645515, 0x11144110, 0x10f43910, 0x10d42d0c,
	0x10c4250b, 0x10942109, 0x10641507, 0x10541105,
	0x10340d05, 0x10440902, 0x10040501, 0x10340502,
	0x10140504, 0x10240502, 0x10340503, 0x10240503,
	0x10340903, 0x10441505, 0x10541106, 0x10741909,
	0x10a4250a, 0x10b4290b, 0x10d4350e, 0x10f43d10,
	0x11345116, 0x11645919, 0x11b4691e, 0x11f47920,
	0x12348d26, 0x1294a12b, 0x12d4b930, 0x1334d136,
	0x13b4e13d, 0x14050544, 0x14951d49, 0x15253d50,
	0x15956158, 0x16058d62, 0x1685ad6b, 0x1745cd79,
	0x18260180, 0x18b62593, 0x1966719b, 0x1a56a5a5,
	0x1b06c5b8, 0x1bf6f9c3, 0x1d0735cd, 0x1ac699a7,
	0x1a266999, 0x18f63989, 0x1845fd81, 0x1745c96f,
	0x16e59d67, 0x15e57d5e, 0x15755153, 0x14c53550,
	0x14651147, 0x13f4fd3b, 0x1374d939, 0x1324c532,
	0x12d4ad2c, 0x12949925, 0x12448521, 0x11d4751d,
	0x11b46919, 0x11645515, 0x11344512, 0x10e4390f,
	0x10e4310e, 0x10b4250a, 0x10941d08, 0x10541506,
	0x10741105, 0x10540d04, 0x10140902, 0x10140502,
	0x10240901, 0x10240502, 0x10240501, 0x10240902,
	0x10440904, 0x10441104, 0x10441103, 0x10641906,
	0x10741d09, 0x1084250b, 0x10c42d0d, 0x10e4390e,
	0x11244113, 0x11445116, 0x11645d18, 0x11b46d1c,
	0x12048121, 0x12349127, 0x1294a52c, 0x12d4bd33,
	0x1344d536, 0x1394ed3d, 0x14250543, 0x14b5254b,
	0x14f54153, 0x1575695b, 0x15f58561, 0x16e5a96b,
	0x1755d97b, 0x18560584, 0x18b64192, 0x19c6699f,
	0x1a969dac, 0x1af6cdb5, 0x1c370dc7, 0x1ca735d1,
	0x1a969da0, 0x1a06799d, 0x18f6298b, 0x17e5f57e,
	0x1755cd71, 0x16759d64, 0x15f57d5d, 0x15655552,
	0x14d5294d, 0x14750944, 0x13d4f13e, 0x13a4d534,
	0x1314c133, 0x12f4b12c, 0x12849d26, 0x12448922,
	0x11e4751e, 0x11c4611b, 0x11845d17, 0x11444d11,
	0x10f4410f, 0x10c4310d, 0x10b4250d, 0x1094210a,
	0x10941507, 0x10641506, 0x10540d05, 0x10340903,
	0x10340902, 0x10140903, 0x10140d04, 0x10240d02,
	0x10441104, 0x10440d03, 0x10541104, 0x10541905,
	0x10641d07, 0x10842109, 0x10a4250a, 0x10b4310c,
	0x10f43d10, 0x11244914, 0x11445515, 0x1184611a,
	0x11c4711d, 0x12348121, 0x12649526, 0x12a4b12b,
	0x12f4c132, 0x1384d938, 0x13c4ed3d, 0x14350943,
	0x14a52948, 0x15354952, 0x15a55d5b, 0x15f58d68,
	0x16f5b171, 0x1745e178, 0x18260d85, 0x18d63598,
	0x19a6759f, 0x1a869db0, 0x1b86d9bc, 0x1cb719ce,
	0x1d0741d5, 0x1ad699a4, 0x19d6759b, 0x1916318b,
	0x1866097e, 0x1765cd78, 0x1675a968, 0x15e57560,
	0x15455554, 0x14c52d4c, 0x14451145, 0x1404f13e,
	0x1394d938, 0x1334bd31, 0x12d4b12b, 0x12949927,
	0x12549124, 0x11f4791d, 0x11c4711b, 0x11745917,
	0x11544513, 0x11144111, 0x10e4350e, 0x10c42d0a,
	0x10842508, 0x10941907, 0x10841d06, 0x10541506,
	0x10540d03, 0x10440d04, 0x10540902, 0x10240903,
	0x10340d03, 0x10341104, 0x10441105, 0x10741505,
	0x10641907, 0x10942108, 0x10b4290a, 0x10b4290f,
	0x10d4310f, 0x11043d0f, 0x11244913, 0x11445915,
	0x11a4691a, 0x11e4711d, 0x12148522, 0x12649d28,
	0x1294b12c, 0x1314c532, 0x1384d938, 0x13e4ed3e,
	0x14551143, 0x14a52d4b, 0x15254956, 0x15e5695d,
	0x16358d65, 0x16f5c16e, 0x1795e17a, 0x18561989,
	0x18f64993, 0x19f6759c, 0x1ad6a1b0, 0x1b56ddbd,
	0x1c6709c4, 0x1d6741d8, 0x1a7699a6, 0x1a067599,
	0x19663d88, 0x1846057f, 0x1775d577, 0x16b5a165,
	0x16257960, 0x15855555, 0x14f5394c, 0x14651145,
	0x1434f53f, 0x1394e537, 0x1354cd33, 0x1314b12d,
	0x1284a12a, 0x12448d27, 0x11f48120, 0x11c46d1b,
	0x11946118, 0x11545114, 0x11144912, 0x10f43d10,
	0x10e4310c, 0x10c42d0b, 0x10942509, 0x10841908,
	0x10741507, 0x10541505, 0x10541503, 0x10640d04,
	0x10441103, 0x10441105, 0x10540d05, 0x10541106,
	0x10841d07, 0x10841908, 0x10742509, 0x1094290b,
	0x10e4350e, 0x1104390f, 0x11144513, 0x11444d15,
	0x11745918, 0x11b4691b, 0x11f47521, 0x12348d25,
	0x12a4a129, 0x12d4b92e, 0x1334c935, 0x1384e139,
	0x13f4fd41, 0x14551548, 0x14b52d4e, 0x1545555a,
	0x1625715e, 0x16759965, 0x1745d572, 0x1795f97f,
	0x18760d8b, 0x19265194, 0x1a2671a3, 0x1af6b5ab,
	0x1b96e1bf, 0x1c6725ca, 0x1d4749d2, 0x1a46a5aa,
	0x1a267da4, 0x1926518b, 0x18160d80, 0x1765cd72,
	0x1695a964, 0x1615715f, 0x15956156, 0x1515394e,
	0x14951547, 0x14250143, 0x13b4e53b, 0x1364c535,
	0x12f4b52f, 0x12a4ad29, 0x12749526, 0x12047922,
	0x11f4751e, 0x11a45d1a, 0x11445516, 0x11144513,
	0x10f43d10, 0x10d4350f, 0x10c42d0c, 0x10942908,
	0x1084210a, 0x10641d07, 0x10641506, 0x10541505,
	0x10541506, 0x10540d06, 0x10641106, 0x10541107,
	0x10841906, 0x10941d07, 0x10942508, 0x10a42509,
	0x10b4310c, 0x10e4350e, 0x11143d11, 0x11244913,
	0x11745515, 0x11a45d1a, 0x11b46d1d, 0x11f48520,
	0x12549527, 0x1294a92b, 0x12f4bd31, 0x1374cd35,
	0x1394e93c, 0x14050141, 0x14752149, 0x14f53553,
	0x15755d5a, 0x15b57563, 0x16859968, 0x1745cd73,
	0x17b5fd7e, 0x1896218d, 0x19864da0, 0x1a367da4,
	0x1ac6bdb7, 0x1b76e1be, 0x1cd735cf, 0x1d8741d9,
	0x1a96a1ab, 0x1a36799f, 0x1906558c, 0x18860d85,
	0x17b5e57b, 0x16f5a56d, 0x16257d5f, 0x15a56556,
	0x1505354d, 0x14851946, 0x13d50140, 0x1394ed3a,
	0x1344d135, 0x12f4bd2e, 0x12c4a52a, 0x12749526,
	0x12348521, 0x11f4711c, 0x11b4691a, 0x11745d16,
	0x11444d13, 0x11244111, 0x11043d0e, 0x10c4310c,
	0x10d4310c, 0x10b42109, 0x10841d0a, 0x10841d08,
	0x10641d06, 0x10741507, 0x10641507, 0x10741d09,
	0x10841d08, 0x10941d09, 0x10941d08, 0x10b4290b,
	0x10c4290a, 0x10d4350e, 0x10f43d10, 0x11144912,
	0x11345515, 0x11745916, 0x11a4691a, 0x11e4791e,
	0x12348924, 0x12649d27, 0x12a4a92e, 0x1324c534,
	0x1354d937, 0x13d4f53f, 0x14150544, 0x14d5294d,
	0x15054556, 0x1585695d, 0x16158564, 0x16d5a96d,
	0x1725cd74, 0x17f6017e, 0x18962d8d, 0x19965996,
	0x1a3689a5, 0x1b76c5af, 0x1c16f5c3, 0x1cb739cf,
	0x1da779d5, 0x1ab699a7, 0x1a167d9f, 0x19664194,
	0x18861984, 0x1795dd75, 0x1755ad6b, 0x16758963,
	0x15d56158, 0x14f5414e, 0x14b51948, 0x1464fd3f,
	0x13b4e539, 0x1344d134, 0x1314b92e, 0x12c4a92b,
	0x12649d27, 0x12348924, 0x1204751f, 0x11c46d1c,
	0x11a45d17, 0x11644d14, 0x11444912, 0x11144110,
	0x1104350c, 0x10e42d0d, 0x10b4310b, 0x10a4250a,
	0x10942508, 0x10942108, 0x10842107, 0x10742108,
	0x10942108, 0x10a41d09, 0x10a42507, 0x10a4290b,
	0x10c4250c, 0x10e4310e, 0x10e43910, 0x11144112,
	0x11344d13, 0x11645916, 0x1184611b, 0x11b4711c,
	0x12148121, 0x12348d23, 0x12a49d29, 0x12e4b52d,
	0x1354c931, 0x1394e139, 0x13e4fd40, 0x14451147,
	0x14d5314e, 0x15654956, 0x15f5715c, 0x16359565,
	0x16b5c171, 0x17a5d97c, 0x18660583, 0x18d6418b,
	0x19a6699d, 0x1a769daa, 0x1b86d9b2, 0x1c4705c0,
	0x1cd739d1, 0x1db741d6, 0x1ac6a5a6, 0x1a5679a0,
	0x19464994, 0x18862187, 0x17d5e978, 0x1745b570,
	0x16358961, 0x15e56d59, 0x15553d4f, 0x14a52547,
	0x14550943, 0x13e4f13c, 0x1384d137, 0x1334c12e,
	0x12f4ad2b, 0x12a4a125, 0x12448d24, 0x12347d1e,
	0x11e4691b, 0x11a46918, 0x11945916, 0x11544914,
	0x11444513, 0x11143d11, 0x1114350e, 0x10d4350d,
	0x10b42d0c, 0x10b4290a, 0x10a42509, 0x10b42509,
	0x1094290a, 0x10b4250a, 0x10b4290b, 0x10b4290a,
	0x10b42d0e, 0x10d4350d, 0x10f4350e, 0x11144111,
	0x11344913, 0x11545115, 0x1184591a, 0x11a4711b,
	0x11e47522, 0x12348923, 0x12a49925, 0x1294a129,
	0x12f4bd30, 0x1354d936, 0x13c4ed3a, 0x14150140,
	0x14551d4a, 0x14f53d4d, 0x15c55156, 0x15c5795e,
	0x1695a568, 0x1705c170, 0x17c5e97b, 0x18860d85,
	0x19063d92, 0x1a1679a2, 0x1ac6adaa, 0x1b46d1b6,
	0x1c2715c1, 0x1d0735d1, 0x1d4779da, 0x1a96a9a9,
	0x1a3691a2, 0x19364d94, 0x18c61d8b, 0x17a5e97d,
	0x16f5c171, 0x16759165, 0x15e5695c, 0x15454952,
	0x14b52949, 0x14350d44, 0x1414f13e, 0x13a4d938,
	0x1334c532, 0x12e4b92e, 0x12849d28, 0x12849523,
	0x12348922, 0x11f4791d, 0x11c4691b, 0x11746118,
	0x11645115, 0x11444d13, 0x10f44513, 0x1124410f,
	0x10f4390e, 0x10e4310d, 0x10d4310c, 0x10b4310d,
	0x10b42d0c, 0x10d4290b, 0x10c42d0b, 0x10d42d0c,
	0x10d4310e, 0x10c4390c, 0x11043d11, 0x11144111,
	0x11244510, 0x11645514, 0x11745119, 0x11a46918,
	0x11c46d1f, 0x12047d1f, 0x12549125, 0x1274a529,
	0x12d4b52c, 0x1324cd34, 0x1384d537, 0x13c4f93f,
	0x14250943, 0x14852949, 0x14f53d53, 0x15a56958,
	0x16258163, 0x1675a568, 0x1785cd7b, 0x17f5e980,
	0x18a6258a, 0x19164d95, 0x1a2671a0, 0x1b36a1ac,
	0x1b96e1bb, 0x1c8721c3, 0x1ce73dcf, 0x1dc761d9,
	0x1ad6a9ab, 0x1a2679a3, 0x19965594, 0x18d6318b,
	0x1805f97e, 0x1745c173, 0x1695996a, 0x15f57161,
	0x15755155, 0x14e52d4a, 0x14751944, 0x13d4f53d,
	0x13a4e13c, 0x1364d132, 0x1304bd2f, 0x12b4a52b,
	0x12749927, 0x12548923, 0x1204791f, 0x11e4711b,
	0x11a46117, 0x11745917, 0x11745513, 0x11244d13,
	0x11044111, 0x10f44111, 0x11043d0e, 0x10e4310d,
	0x10f4350d, 0x10d4310c, 0x10d4350d, 0x10c4350d,
	0x10e4350d, 0x10f4390e, 0x11043d10, 0x11244110,
	0x11344514, 0x11544d13, 0x11545918, 0x1184611a,
	0x11d4691c, 0x11e4791e, 0x12448523, 0x12649925,
	0x12b4a92c, 0x12e4bd33, 0x1334cd34, 0x1394e13c,
	0x1414fd41, 0x14552145, 0x1485294e, 0x15654d56,
	0x15b5655d, 0x16259165, 0x16e5ad6b, 0x1775d579,
	0x1866017b, 0x18a6218d, 0x19865d99, 0x1a468da5,
	0x1b26b9af, 0x1bd6e1c0, 0x1ce719c4, 0x1d6751d8,
	0x1d6775dc, 0x1a66a9af, 0x1a76999b, 0x19c64d95,
	0x19061d88, 0x17c6097d, 0x1775cd76, 0x16b5ad6a,
	0x1645815e, 0x15755d56, 0x14e53550, 0x14551545,
	0x1414fd3f, 0x13e4e93a, 0x1334d533, 0x12f4bd2e,
	0x12b4b12c, 0x1274a127, 0x12648d23, 0x12348523,
	0x11e4751f, 0x11e46d1b, 0x11846919, 0x11745517,
	0x11545115, 0x11444d13, 0x11244914, 0x11144111,
	0x11144112, 0x11043d11, 0x10f4390f, 0x10e4390f,
	0x10f43910, 0x10f43d10, 0x11244110, 0x11244510,
	0x11244512, 0x11545915, 0x11745d16, 0x11945d1a,
	0x11b46d1d, 0x11f4751e, 0x12148925, 0x12649926,
	0x12b4a92a, 0x12e4b530, 0x1324c134, 0x1374d937,
	0x13e4ed3d, 0x14350945, 0x14951d4e, 0x15253d4f,
	0x15555156, 0x15e5795d, 0x16359966, 0x16f5bd76,
	0x1795e179, 0x18660989, 0x18f6458f, 0x1976719d,
	0x1a6695a8, 0x1b16c5b7, 0x1be6f9b7, 0x1ce735cc,
	0x1d3759d6, 0x1de771dd, 0x1af6b5ad, 0x1a9685a5,
	0x19b66996, 0x18e62d89, 0x18a5fd7c, 0x1765d176,
	0x16e5a967, 0x16257d5d, 0x15c56557, 0x1535414d,
	0x14c5214a, 0x14350141, 0x13f4ed3b, 0x1394d535,
	0x1314c930, 0x12f4b12d, 0x12b4ad29, 0x12749527,
	0x12748d23, 0x12348120, 0x11e4791e, 0x11d46d1d,
	0x11b4691a, 0x11a45917, 0x11745515, 0x11544d14,
	0x11444d13, 0x11544112, 0x11544511, 0x11344110,
	0x11344511, 0x11344511, 0x11544d12, 0x11344911,
	0x11444d13, 0x11745515, 0x11745d17, 0x11946117,
	0x11d46d1a, 0x11e4791d, 0x12148121, 0x12549124,
	0x12b4a12a, 0x12d4ad2c, 0x1344c531, 0x1334cd34,
	0x13f4ed3a, 0x1414f543, 0x14651546, 0x14d5294c,
	0x15054153, 0x15856156, 0x16458164, 0x16d5a569,
	0x1745cd72, 0x17c5e977, 0x18a61d82, 0x19264992,
	0x1a3675a1, 0x1ad699ad, 0x1b66d1bb, 0x1c3719c0,
	0x1ce739cc, 0x1df765dd, 0x1e3781e1, 0x1b26b5ad,
	0x1a5691a5, 0x19b6699a, 0x18f6398e, 0x18460182,
	0x17c5dd79, 0x16e5ad6b, 0x16458962, 0x15b5615a,
	0x15554950, 0x14952d49, 0x14950941, 0x1434f93c,
	0x13b4d535, 0x1354cd34, 0x1314b92e, 0x12d4b12a,
	0x1294a128, 0x12649523, 0x12548921, 0x1224811f,
	0x11f4711f, 0x11946d1c, 0x11b4651a, 0x11a45d16,
	0x11745515, 0x11645516, 0x11645113, 0x11544d12,
	0x11244d14, 0x11444d14, 0x11544d12, 0x11545113,
	0x11745114, 0x11845916, 0x11a45d17, 0x11846d19,
	0x11c4691b, 0x11f4791c, 0x12148121, 0x12448d23,
	0x12549925, 0x12d4ad2b, 0x12e4b52f, 0x1344d134,
	0x1394d537, 0x13d4f53c, 0x14450540, 0x14952549,
	0x14f53550, 0x15455955, 0x15f5795d, 0x16458564,
	0x1705b56e, 0x1785cd78, 0x18661180, 0x18c6318d,
	0x19765995, 0x1a2681a8, 0x1ae6c1a8, 0x1bb6e1b6,
	0x1c670dc3, 0x1d073dd6, 0x1d9779d6, 0x1e5799e3,
	0x1ac6a9a9, 0x1a9699a5, 0x19e6619a, 0x19364d8e,
	0x18261983, 0x1795e979, 0x1725ad6d, 0x16b59164,
	0x16157159, 0x15555154, 0x14d5354d, 0x14751145,
	0x1414fd3d, 0x13e4e538, 0x1394d536, 0x1324c530,
	0x1304ad2e, 0x12b4a92a, 0x1284a127, 0x12649125,
	0x12448d22, 0x12248120, 0x11e4791e, 0x11d4711d,
	0x11a46d19, 0x11846119, 0x11a46118, 0x11645917,
	0x11745916, 0x11645116, 0x11745516, 0x11745916,
	0x11845918, 0x11845d18, 0x11a46118, 0x11b4691b,
	0x1204711c, 0x11f47d20, 0x12248120, 0x12548923,
	0x12549525, 0x12d4a929, 0x12e4b52f, 0x1334bd32,
	0x1374d938, 0x13d4e93c, 0x1434fd3f, 0x14451d46,
	0x14c5254d, 0x15554551, 0x15856559, 0x15e5855e,
	0x1685996b, 0x16e5b96d, 0x1795e17b, 0x18560187,
	0x18c63d8f, 0x1996619a, 0x1a1689a5, 0x1af6c9b6,
	0x1bf6f1bd, 0x1c8719cc, 0x1d1749d2, 0x1dc779dc,
	0x1eb775e1, 0x1bd6b1b0, 0x1ac69daf, 0x19f6759e,
	0x1966518e, 0x18d62188, 0x1815e97b, 0x1725bd6f,
	0x16a59567, 0x1625795f, 0x15b55955, 0x14d54150,
	0x14751947, 0x14350541, 0x13e4ed3c, 0x1384e137,
	0x1344c932, 0x1304c12f, 0x12f4a92a, 0x1294a12b,
	0x12749524, 0x12649126, 0x12348920, 0x1214811e,
	0x11e47d1f, 0x11d4751c, 0x11b46d1a, 0x11d4711a,
	0x11a46919, 0x11946119, 0x1184651a, 0x11846518,
	0x11b46118, 0x11b46918, 0x11b4711b, 0x11b46d1e,
	0x11e4711d, 0x11f47521, 0x12348521, 0x12548922,
	0x12549527, 0x1294a92b, 0x12d4b530, 0x1334c130,
	0x1374d536, 0x13a4d938, 0x13d4f93e, 0x14550d40,
	0x14a52949, 0x1515394f, 0x15555d5a, 0x15c56d5c,
	0x16658d66, 0x16f5b56e, 0x1745d977, 0x17f6017c,
	0x18962186, 0x19163d95, 0x19d6719b, 0x1a56adab,
	0x1b56c1af, 0x1c16f9c2, 0x1cc731cd, 0x1d8751d6,
	0x1d7775d8, 0x1ed78de1, 0x1b96c5ab, 0x1aa6a9ab,
	0x1a4675a2, 0x19c6598e, 0x1926298b, 0x18060180,
	0x17b5d573, 0x16d5a169, 0x16258163, 0x15d5655b,
	0x15354953, 0x14e5254a, 0x14851145, 0x1414f93e,
	0x13c4e937, 0x1384dd33, 0x1334bd33, 0x1314b52c,
	0x12c4b12b, 0x12b4a129, 0x12849d26, 0x12549526,
	0x12548d23, 0x12448522, 0x1204811f, 0x11e4791d,
	0x11d4791e, 0x11e4751d, 0x11a46d1d, 0x11d4751e,
	0x11e4691c, 0x11e4711d, 0x11c4751f, 0x11d4791e,
	0x12047d20, 0x12348122, 0x12348923, 0x12549125,
	0x1284a128, 0x12e4a12a, 0x12f4b52d, 0x1324c131,
	0x1344c934, 0x13a4e137, 0x13d4ed3e, 0x14050541,
	0x14952148, 0x14e5394d, 0x15654554, 0x15a5655e,
	0x16157d61, 0x16859d6a, 0x1725c570, 0x17d5dd7b,
	0x18460982, 0x18c63592, 0x19965d99, 0x1a167da1,
	0x1af6a9a9, 0x1bb6d9b9, 0x1c3709c1, 0x1ce72dd0,
	0x1d574dcf, 0x1e0769e4, 0x1e4799e7, 0x1b06c5ad,
	0x1ab6a5ad, 0x1a26819e, 0x19766197, 0x1906298d,
	0x1866097f, 0x1795dd78, 0x16f5b56d, 0x16859965,
	0x15e5715c, 0x15655154, 0x14e52d4c, 0x14951d45,
	0x14550540, 0x13d4e938, 0x13b4dd36, 0x1374c533,
	0x1324c130, 0x1304b92c, 0x12d4a92c, 0x12c4a526,
	0x12949927, 0x12749522, 0x12648d24, 0x12348122,
	0x12048120, 0x11f47d20, 0x1204791f, 0x12047920,
	0x1214751d, 0x11f4791f, 0x1214791d, 0x1224791e,
	0x12047d1f, 0x12348522, 0x12649125, 0x12549525,
	0x12749925, 0x12a4a52b, 0x12d4b52e, 0x1334bd2e,
	0x1364c934, 0x1364e136, 0x13f4e93c, 0x1414fd3e,
	0x14650d44, 0x14b52d4a, 0x15253d51, 0x15755559,
	0x15f57959, 0x16558d61, 0x16e5a96d, 0x1725cd73,
	0x1805f17d, 0x18362187, 0x19464593, 0x19466d9c,
	0x1a86899f, 0x1b76cdb0, 0x1bc6e1bb, 0x1c671dc1,
	0x1d7735cb, 0x1d9759d0, 0x1e5779e0, 0x1e57a5eb,
	0x1b76c1b6, 0x1b269da8, 0x1a6681a2, 0x19a66195,
	0x1926418b, 0x18660d86, 0x17f5e97c, 0x1745b570,
	0x16d59d67, 0x16057d5b, 0x15855956, 0x15453d4e,
	0x14e5254b, 0x14a51143, 0x1444f53f, 0x13f4f139,
	0x13a4d936, 0x1374d134, 0x1334c12e, 0x12f4c12c,
	0x12d4a92b, 0x12d4a928, 0x12b49d28, 0x12749926,
	0x12649125, 0x12548d22, 0x12348921, 0x12648921,
	0x12448520, 0x12448120, 0x12648920, 0x12548921,
	0x12648d23, 0x12549122, 0x12749524, 0x12949927,
	0x12c4a128, 0x12f4b12c, 0x12e4b12e, 0x1314c130,
	0x1374d132, 0x1394d536, 0x13c4e53b, 0x1404fd3e,
	0x14550942, 0x14b5214c, 0x15253d4f, 0x15554954,
	0x15d5615c, 0x1675895f, 0x16b59d6a, 0x1725bd6d,
	0x17b5e578, 0x18660581, 0x1916358d, 0x19364596,
	0x1a467da0, 0x1ac6a9a8, 0x1bb6ddb7, 0x1bf6f9ba,
	0x1c770dc6, 0x1d7741d2, 0x1df761db, 0x1e978de6,
	0x1f37b9ec, 0x1b66d5ac, 0x1ac6a9ad, 0x1a568da3,
	0x19b6699b, 0x19363d91, 0x18a62988, 0x1845f97b,
	0x1765dd73, 0x1715a568, 0x16358564, 0x16056959,
	0x15754d51, 0x14e53949, 0x1495254c, 0x14550942,
	0x1414f540, 0x13b4e138, 0x1384dd36, 0x1354c532,
	0x1334c12f, 0x1324bd2e, 0x1304b12d, 0x12d4a92a,
	0x12d4a529, 0x12949929, 0x12849d26, 0x12749d26,
	0x12749927, 0x12549524, 0x12749524, 0x12749925,
	0x12749527, 0x12949d27, 0x12a49527, 0x12b4a529,
	0x12f4ad2b, 0x12f4b92c, 0x1304bd31, 0x1334c131,
	0x1344cd2f, 0x1394dd37, 0x13c4e93c, 0x13f4f93d,
	0x14150943, 0x14851946, 0x15053550, 0x15054552,
	0x15656558, 0x1625815f, 0x16459d65, 0x16e5ad6b,
	0x1765cd75, 0x1805f17d, 0x18761d83, 0x19263993,
	0x19965d9a, 0x1a768da6, 0x1b16b1ae, 0x1bb6e9b5,
	0x1c66f9c1, 0x1d2721cc, 0x1d974dd0, 0x1e0761dd,
	0x1ec799ea, 0x1f87a5f5, 0x1b56ddbd, 0x1b36c1af,
	0x1aa69da6, 0x1a66799c, 0x19665192, 0x18d6298b,
	0x18960583, 0x17c5d573, 0x1705b96c, 0x16a59963,
	0x16657d5e, 0x15a56159, 0x1535454e, 0x14d52d4b,
	0x14951946, 0x14350142, 0x13f4f53c, 0x13d4e539,
	0x1394d936, 0x1384d134, 0x1334c932, 0x12f4b932,
	0x12f4b930, 0x12c4b52d, 0x12c4ad2b, 0x12c4a928,
	0x12a4a527, 0x12b4a92b, 0x12c4a12b, 0x12a4a52a,
	0x12c4a92a, 0x12a4a929, 0x12d4a52c, 0x12d4b12c,
	0x1314b12c, 0x1334b92e, 0x1334bd33, 0x1324d134,
	0x1374d136, 0x1394e13b, 0x13e4ed3c, 0x14150140,
	0x14351549, 0x14a51d4a, 0x14d5314f, 0x15254551,
	0x15955955, 0x1605795e, 0x16758d66, 0x16c5ad6a,
	0x1735cd71, 0x17b5e97c, 0x18761181, 0x18d6318b,
	0x19565595, 0x1a267da3, 0x1ad6a9a4, 0x1b56c5b6,
	0x1bd701b7, 0x1cb719ca, 0x1d172dc8, 0x1dc74dd9,
	0x1eb77dda, 0x1ec795ec, 0x1f57d5f5, 0x1c36f9bf,
	0x1b46d9ac, 0x1ac6a1a4, 0x1a06759f, 0x19a65596,
	0x19663d8c, 0x18961d81, 0x1805f179, 0x1775c96c,
	0x16c5a568, 0x16857961, 0x16056959, 0x15854d53,
	0x15453550, 0x14d52549, 0x14951143, 0x1444fd41,
	0x13d4f93d, 0x13b4e137, 0x1384d535, 0x1364cd35,
	0x1354c930, 0x1314bd33, 0x1304bd2f, 0x12d4b92c,
	0x12c4b52b, 0x12e4b12b, 0x12f4b12b, 0x12c4b12b,
	0x12f4ad2c, 0x1304b12c, 0x12f4b12d, 0x1304b92d,
	0x1324b92f, 0x1344bd2e, 0x1354c932, 0x1374d133,
	0x1394e136, 0x13d4e536, 0x13d4f93c, 0x1414fd3f,
	0x14550d43, 0x14751944, 0x14d52548, 0x15153d51,
	0x15755954, 0x15d56958, 0x1655815e, 0x1695a165,
	0x1715b56f, 0x1795d575, 0x1805fd82, 0x18b62d82,
	0x19363994, 0x19f65d97, 0x1a5699a2, 0x1b26adaa,
	0x1b96d9ad, 0x1be705c1, 0x1cf725c4, 0x1d373dd0,
	0x1de75dd3, 0x1e5785e5, 0x1f07b5ed, 0x1f37d1fb,
	0x1c3701bc, 0x1bb6d1b6, 0x1b56b5a6, 0x1a567d9b,
	0x19d65d9a, 0x1926418c, 0x18a62587, 0x1845fd7b,
	0x17f5d571, 0x1735b56c, 0x16c5a166, 0x1675795f,
	0x15c55d56, 0x15454950, 0x1505314c, 0x14f52149,
	0x14750943, 0x1425053f, 0x1404f140, 0x13b4ed39,
	0x13a4d934, 0x1354cd32, 0x1354d131, 0x1354cd31,
	0x1334c533, 0x1344c52e, 0x1344bd2f, 0x1324c132,
	0x1334c12e, 0x1324bd30, 0x1344c12f, 0x1324c12f,
	0x1344c534, 0x1364d534, 0x1354cd33, 0x13c4e536,
	0x13a4e538, 0x13f4e938, 0x1424f93d, 0x1435013e,
	0x14750945, 0x14951d46, 0x14b52d4b, 0x15253d4e,
	0x15655555, 0x15c57157, 0x16557d5d, 0x16b5a566,
	0x16f5b56c, 0x1745d16f, 0x17b5e57e, 0x18860d85,
	0x19162989, 0x19964d94, 0x1a26759e, 0x1ac699a7,
	0x1bb6c9ab, 0x1bf6f1bc, 0x1c371dc4, 0x1d4739cb,
	0x1d5741d8, 0x1e0775d9, 0x1ea78de5, 0x1f57d5f3,
	0x2017edf9, 0x1c871dc5, 0x1c36e5b7, 0x1b56b9ac,
	0x1a768da1, 0x1a56699b, 0x19864990, 0x19062d8b,
	0x18b60982, 0x1825f579, 0x17a5c971, 0x16f5a56a,
	0x16659162, 0x1625715b, 0x15855d52, 0x15754d51,
	0x15453d4d, 0x14a52547, 0x14951943, 0x14250d3f,
	0x13f5013d, 0x13d4ed39, 0x13b4e138, 0x1394dd34,
	0x1374d133, 0x1394d535, 0x1384d533, 0x1364cd35,
	0x1354cd34, 0x1384d132, 0x1384d132, 0x1364d534,
	0x1374d934, 0x1374d936, 0x13a4e137, 0x13d4e137,
	0x13e4ed38, 0x1414ed3e, 0x1424f53d, 0x14550540,
	0x14751543, 0x14b52948, 0x14e52d4b, 0x15553d4f,
	0x15855557, 0x15c56959, 0x15f5855e, 0x16959965,
	0x1695b16a, 0x1725cd70, 0x17d5e979, 0x1846057f,
	0x18f62586, 0x19764995, 0x1a266597, 0x1a66a1a5,
	0x1b36c5ac, 0x1bc6cdb4, 0x1c8709c0, 0x1c870dc5,
	0x1d3739ce, 0x1d6755d2, 0x1e2785dd, 0x1f27a1ec,
	0x1fd7e1f0, 0x20a7f5fc, 0x1dc739c8, 0x1d0701c3,
	0x1b56c5b4, 0x1a5691a1, 0x1a5685a0, 0x19a6518c,
	0x18f63d8b, 0x18961985, 0x1865fd7e, 0x17a5d576,
	0x1715b56e, 0x16c5a167, 0x1685855d, 0x15d57559,
	0x15b55555, 0x15454551, 0x14f52d4d, 0x14c52149,
	0x14951945, 0x14550d41, 0x1424f540, 0x13d4f53b,
	0x13e4e53b, 0x13b4e539, 0x13d4e538, 0x13a4d938,
	0x13c4d936, 0x13d4e538, 0x13d4dd37, 0x13a4dd36,
	0x13a4e538, 0x13a4e538, 0x13e4ed38, 0x13d4f93b,
	0x1434fd3d, 0x14250540, 0x14550d42, 0x14651543,
	0x14952545, 0x14c5254c, 0x14f5354f, 0x1545414d,
	0x15756157, 0x15b5695a, 0x16157d60, 0x16759569,
	0x16e5a96b, 0x1745c171, 0x17c5e57a, 0x18760981,
	0x18b62586, 0x19663d96, 0x19c65999, 0x1a5681a1,
	0x1ac6a1ab, 0x1af6c9b5, 0x1c26edba, 0x1c9715c7,
	0x1d4725c6, 0x1d4741cd, 0x1e0761db, 0x1e9799e6,
	0x1ec7bdf3, 0x1ff80201, 0x21383a15, 0x1d8741d8,
	0x1cf71dc3, 0x1bf6d5b4, 0x1a769daa, 0x1a468999,
	0x19d65998, 0x19664993, 0x18d6298b, 0x18660582,
	0x1815f978, 0x17d5cd71, 0x1715b968, 0x16459965,
	0x1655815c, 0x15a56557, 0x15655952, 0x1565414c,
	0x14f5254a, 0x14d5254a, 0x14751544, 0x14850540,
	0x14450140, 0x13f4fd3e, 0x13f4f53c, 0x13d4ed3d,
	0x13f4ed3b, 0x13e4e537, 0x1434f139, 0x13f4e53a,
	0x13e4ed3d, 0x13e4ed3a, 0x1414f13c, 0x1424fd3e,
	0x1414fd40, 0x14450542, 0x14950944, 0x14852546,
	0x14b52949, 0x15352d4e, 0x1505454d, 0x15454953,
	0x15b55956, 0x15e5695c, 0x16057960, 0x16458d64,
	0x16d5a96b, 0x1715c972, 0x17c5e177, 0x1825fd81,
	0x18b62186, 0x19663d8d, 0x19c64d97, 0x1a167da1,
	0x1ad6adaa, 0x1b26c1b0, 0x1bf6e5b7, 0x1ca701c1,
	0x1cd725c5, 0x1d372dcc, 0x1d974dd8, 0x1e3775da,
	0x1e5791eb, 0x1fe7ddf5, 0x20580206, 0x21185615,
	0x1e3779d3, 0x1ce73dc9, 0x1c86e1b5, 0x1b26c1ad,
	0x1a7691a6, 0x19f66197, 0x19764595, 0x19362d8a,
	0x18c6157e, 0x1805f579, 0x17a5e975, 0x1745c16c,
	0x16d5ad69, 0x16a5995e, 0x16357d5c, 0x15d56958,
	0x15856150, 0x15753d4e, 0x1515394c, 0x14b52d49,
	0x15052147, 0x14c51140, 0x14550942, 0x14950d40,
	0x14550541, 0x1404f93d, 0x1415013e, 0x1424fd41,
	0x1414f93d, 0x1475013f, 0x14250d3e, 0x14a50d40,
	0x14851140, 0x14951142, 0x14951945, 0x14b52d46,
	0x15052d49, 0x1515494a, 0x1585454d, 0x15855155,
	0x16056156, 0x15f5715b, 0x16458560, 0x16759165,
	0x16f5a169, 0x1755c973, 0x1785e573, 0x1825f581,
	0x18a62586, 0x1906458f, 0x19d65995, 0x19b6859e,
	0x1aa6a1a5, 0x1b76b5a6, 0x1bf6f1b2, 0x1c96f9c1,
	0x1ca729c0, 0x1d0735c9, 0x1d974dd2, 0x1db761d6,
	0x1de771e0, 0x1f37b5ec, 0x1fc7f9f9, 0x20f8220b,
	0x21785214,
};

/* //100%
static UINT32 ecs_tab[4225] =
{
	0x3fffffff, 0x3fffffff, 0x3fffffff, 0x3fffffff,
	0x3ffff3ff, 0x3e5f6bdd, 0x3daf0fc0, 0x3baeab9e,
	0x399e0b90, 0x399e2b8a, 0x368db35d, 0x354d0350,
	0x339caf26, 0x325c570f, 0x2ffb9ee5, 0x2e6b4ed4,
	0x2c9b06c0, 0x2b3a96ad, 0x2a6a6298, 0x285a3e87,
	0x2769b66f, 0x26b9ae5a, 0x2659665a, 0x25894253,
	0x25190e47, 0x24290243, 0x2478f641, 0x2398ce3c,
	0x2448de32, 0x23c89e32, 0x22d8ca27, 0x2388ce35,
	0x2358ce2f, 0x2308de27, 0x2418f22e, 0x23b91635,
	0x24293641, 0x24a92a50, 0x25e99241, 0x25e99e5d,
	0x2729b267, 0x288a0a77, 0x2a1a4e89, 0x298a7e99,
	0x2beaeab9, 0x2beb06da, 0x2e9bb6dc, 0x30bc0b04,
	0x332c9319, 0x368d4f40, 0x359d9f6c, 0x3abe4f82,
	0x3c1edbaa, 0x3d4f5bd0, 0x3fffffee, 0x3fffffff,
	0x3fffffff, 0x3fffffff, 0x3fffffff, 0x3fffffff,
	0x3fffffff, 0x3fffffff, 0x3fffffff, 0x3fffffff,
	0x3fffffff, 0x3fffffff, 0x3fffffff, 0x3fffffff,
	0x3f9fffff, 0x3e8f9fe7, 0x3c4f33dd, 0x3b7e7baa,
	0x394e1f85, 0x380ddf62, 0x35ed1f55, 0x347cef34,
	0x325c4f20, 0x307c3efe, 0x2e0b8eda, 0x2d1b36cd,
	0x2b9ae6ad, 0x2a3abe9c, 0x28ba428d, 0x27e9fa7c,
	0x26d9ce6c, 0x25b97655, 0x24a93a54, 0x25392648,
	0x2418f242, 0x2328c635, 0x22e89223, 0x22a89619,
	0x2298921e, 0x21c88212, 0x21f8721a, 0x21e85e1a,
	0x22885a16, 0x21886a15, 0x22187e1d, 0x22288a23,
	0x2238aa23, 0x2328a628, 0x2368ce31, 0x2468e238,
	0x2479163e, 0x24e95653, 0x26d97e65, 0x2779c273,
	0x28ca0e85, 0x297a7a95, 0x2b8ac6ac, 0x2c5b02d1,
	0x2e6b8ec6, 0x2ffbe2ee, 0x313c4f24, 0x318d1f32,
	0x363d7744, 0x39ce0b82, 0x3b4e6fa7, 0x3caf33da,
	0x3ffffff8, 0x3fffffff, 0x3fffffff, 0x3fffffff,
	0x3fffffff, 0x3fffffff, 0x3fffffff, 0x3fffffff,
	0x3fffffff, 0x3fffffff, 0x3fffffff, 0x3fffffff,
	0x3f2fe3e7, 0x3fff5bea, 0x3b7edba1, 0x391e5b87,
	0x383deb72, 0x372dbf6c, 0x351d1f50, 0x337caf2b,
	0x309c0317, 0x2efbaef6, 0x2d7b46d1, 0x2c7ae6be,
	0x2aea7a9b, 0x29aa4a9e, 0x2729d682, 0x26b9a25f,
	0x26198a56, 0x25793240, 0x2359223f, 0x2268be2e,
	0x22989a25, 0x22087220, 0x21e85610, 0x20f84e0e,
	0x20b82605, 0x20880208, 0x1fe805fe, 0x1ff7f1fa,
	0x1fe7f9fc, 0x2027fa00, 0x2047fe00, 0x200815fe,
	0x20d82207, 0x21181a07, 0x20e83a16, 0x21685a14,
	0x21d8761a, 0x2218861a, 0x2348d62c, 0x24391637,
	0x24c94a53, 0x26197a55, 0x26e9b273, 0x28ca0277,
	0x2a1a4a98, 0x2acacaaa, 0x2c4b16bc, 0x2efb7ed3,
	0x2f9c1b04, 0x320ca717, 0x359d2734, 0x36ddd374,
	0x388e877f, 0x3c4ef7ad, 0x3cdf6bdd, 0x3fffd7ff,
	0x3fffffff, 0x3fffffff, 0x3fffffff, 0x3fffffff,
	0x3fffffff, 0x3fffffff, 0x3fffffff, 0x3fffffff,
	0x3f6fe3f5, 0x3cdfabe4, 0x3cdef7b6, 0x3a8e939e,
	0x383e4f72, 0x375db350, 0x354d273d, 0x332c9b2b,
	0x303c5f11, 0x2efbcafa, 0x2d7b5acf, 0x2ccb1ab9,
	0x29ba8ab2, 0x29ba2a98, 0x281a027b, 0x2759b269,
	0x25496a52, 0x24791e4a, 0x2418da3f, 0x2248b62e,
	0x22288226, 0x21c85e13, 0x21084610, 0x20882a00,
	0x2067fdf8, 0x1f97e5f6, 0x1f57e5f2, 0x1f57c9ed,
	0x1f67c1ea, 0x1f37a9e8, 0x1f17ade0, 0x1f17aded,
	0x1f57c1f7, 0x1fa7b9f2, 0x1f07ddf7, 0x205805fe,
	0x20781a06, 0x20885a06, 0x21d84217, 0x22486e1c,
	0x2248be28, 0x2308ea37, 0x24a90e49, 0x25b97a55,
	0x26a9be6b, 0x2769f27b, 0x29aa6295, 0x2ababea9,
	0x2d1b16c5, 0x2d9b86e1, 0x2fbc1afe, 0x32ecbf15,
	0x345d1f47, 0x36ddb36f, 0x3a8e3793, 0x3bdf0fad,
	0x3e5f6bb0, 0x3ffff3ff, 0x3fffffff, 0x3fffffff,
	0x3fffffff, 0x3fffffff, 0x3fffffff, 0x3fffffff,
	0x3fffffee, 0x3fffe3ee, 0x3def0fc9, 0x3a5e6393,
	0x388e6387, 0x365deb6f, 0x34fd7744, 0x320cdb29,
	0x311c330a, 0x2ebbb6e3, 0x2ceb4edf, 0x2b3ae6b5,
	0x2a6a86a4, 0x29ba1e8a, 0x2729f270, 0x2699965b,
	0x25994a48, 0x24590a43, 0x2348b62c, 0x22388e1d,
	0x21384618, 0x21184607, 0x1fe7fe06, 0x1f6801f2,
	0x1f37b9f0, 0x1ea7ade7, 0x1e9795e0, 0x1e3789dc,
	0x1e6765d6, 0x1dc771d3, 0x1d876ddc, 0x1d2769d3,
	0x1de76dd6, 0x1df779dc, 0x1e3781d9, 0x1e1771e0,
	0x1ea7b5e9, 0x1f47b1e6, 0x1f67d1f6, 0x2067f9f8,
	0x2068260c, 0x20f84a14, 0x22387620, 0x2248b628,
	0x23f8e645, 0x24993649, 0x26d98262, 0x2829fa75,
	0x28ea2687, 0x29ea96a4, 0x2c0aeab7, 0x2e2b72dc,
	0x301c1af0, 0x32ec8320, 0x354d1f47, 0x363dab7f,
	0x3b1e1f90, 0x3baf03b0, 0x3e8f77c9, 0x3fffffd6,
	0x3fffffff, 0x3fffffff, 0x3fffffff, 0x3fffffff,
	0x3fffffff, 0x3fff77f8, 0x3def43dd, 0x3a2ee7a1,
	0x380e2b8a, 0x370dbf77, 0x360d3b50, 0x34accb13,
	0x30fc3f19, 0x2efbaef8, 0x2d7b5acc, 0x2b9af2c0,
	0x2aea66a1, 0x287a2a85, 0x2809e675, 0x26998264,
	0x24e93e40, 0x23b8de31, 0x22d8a624, 0x21a8761a,
	0x20e84610, 0x20182202, 0x2027d1f8, 0x1f77bdf4,
	0x1de791df, 0x1db76dde, 0x1d9751d3, 0x1d873dd1,
	0x1d7735cf, 0x1cc739c7, 0x1c870dc1, 0x1cc725c9,
	0x1cc715c8, 0x1c5721c0, 0x1ce711c6, 0x1ce721c6,
	0x1d4721d5, 0x1de751d9, 0x1d8765d9, 0x1e777dda,
	0x1ea799e4, 0x1fa7dde8, 0x2007f202, 0x21181a05,
	0x2188561d, 0x22588e1c, 0x23a8ce3e, 0x24393246,
	0x2579664e, 0x27b9c273, 0x295a3287, 0x2a6aa69b,
	0x2c0aeabe, 0x2e7b5ad3, 0x301bd2ed, 0x31ac9b22,
	0x332d3b49, 0x375dcb72, 0x3a2e2b9b, 0x3e1ecfaa,
	0x3ebf77b6, 0x3fff9fe4, 0x3fffffff, 0x3fffffff,
	0x3fffffff, 0x3fffffff, 0x3f6f93c3, 0x3d1ec3ad,
	0x3abe6fa7, 0x370deb7a, 0x370d635d, 0x339cef29,
	0x311c5722, 0x2f9be2ee, 0x2e0b76c6, 0x2c9afab4,
	0x2aca7e9e, 0x295a3695, 0x2729be6c, 0x25e96e54,
	0x24694245, 0x22e8de33, 0x22889621, 0x21988611,
	0x20781e08, 0x1fc7e1f2, 0x1fc7c1e8, 0x1e1785e0,
	0x1df789d6, 0x1ce759d3, 0x1d272dcb, 0x1c16fdc0,
	0x1c3719bd, 0x1b46f5c1, 0x1b86ddb7, 0x1b86d9af,
	0x1b46c5b3, 0x1b26d1b0, 0x1b36c9bb, 0x1ba6ddae,
	0x1b46edb9, 0x1bd6f1b9, 0x1c3705be, 0x1ca721c6,
	0x1d172dc8, 0x1d475dd3, 0x1e776ddc, 0x1f4799e5,
	0x1f67d1ed, 0x204821fe, 0x20e8560c, 0x22889619,
	0x2328be33, 0x24691e43, 0x25c9565e, 0x2709ce73,
	0x2879fe93, 0x298a8aa9, 0x2b9afaaf, 0x2e4b86d8,
	0x301c030c, 0x32ecaf32, 0x35ed1f47, 0x370dd382,
	0x391e7ba1, 0x3c1ef7d0, 0x3eff9fe4, 0x3defd7ee,
	0x3fffffff, 0x3fffffff, 0x3fffffff, 0x3d1f03d6,
	0x3b7e93b6, 0x38be1793, 0x368d975d, 0x337d1334,
	0x325c8b2b, 0x318c2b00, 0x2e4b6ad3, 0x2d7b32ca,
	0x2bbaaaaf, 0x28ca4290, 0x27d9f282, 0x26f98a59,
	0x24d94e46, 0x23e8fa37, 0x22889622, 0x21486613,
	0x20f83200, 0x1fc7e5fa, 0x1ec7b9ee, 0x1e578de3,
	0x1d8751d5, 0x1d3735c9, 0x1c470dc8, 0x1c1705bb,
	0x1af6f1b3, 0x1b86d5af, 0x1b76bdab, 0x1b06ada5,
	0x1a869daa, 0x1a86b1a7, 0x1ae6ada9, 0x1a66a9a4,
	0x1ae69da7, 0x1b06b5ab, 0x1b06b9b5, 0x1b26d5b3,
	0x1bd6edbe, 0x1c5705c2, 0x1c9721c9, 0x1cd765d1,
	0x1dd769e1, 0x1e9785e8, 0x1fc7c1f3, 0x205809f9,
	0x21883611, 0x22188a22, 0x22f8c629, 0x24590e4c,
	0x26997e58, 0x2759ee7b, 0x292a3e92, 0x2a6acaaf,
	0x2c4b22c3, 0x2e6b9ef4, 0x30fc5719, 0x340cdb24,
	0x363d6b55, 0x38ee4382, 0x3caeabb0, 0x3cdf33c3,
	0x3e8f93e7, 0x3f9ffff8, 0x3fffffff, 0x3fffffff,
	0x3baf1baa, 0x3a5ecf93, 0x394deb8a, 0x363d5753,
	0x329c9b32, 0x311c3308, 0x2f7b9ee9, 0x2d9b3ecf,
	0x2abac6ac, 0x29da7a95, 0x2769fe7b, 0x26b9ae67,
	0x25292652, 0x23c8ce31, 0x22a8861f, 0x21784a11,
	0x20180a05, 0x1fb7ddf2, 0x1e97a1e8, 0x1dd759d9,
	0x1d474dcc, 0x1c1725c8, 0x1be6f5ba, 0x1ba6ddaf,
	0x1af6c1ae, 0x1a86a5a6, 0x1a0691a1, 0x19e67da0,
	0x19f6759b, 0x19766597, 0x19c64d98, 0x19a65d99,
	0x1a165196, 0x19866593, 0x19c6759d, 0x1a268da0,
	0x1a9691a5, 0x1a96adaa, 0x1b46c9af, 0x1ba6ddb8,
	0x1c5705bc, 0x1cc721c6, 0x1d9739d3, 0x1df781e3,
	0x1f57b1ea, 0x2017f9ff, 0x20482a08, 0x21f89a1c,
	0x2368be35, 0x24a91e4a, 0x25e95254, 0x2779ee77,
	0x28ea5a8d, 0x2a9abeb7, 0x2ccb36cc, 0x2f7bcb04,
	0x31ac9b19, 0x33ece33b, 0x375dd35f, 0x3a8e439b,
	0x39feabb3, 0x3e1f43bd, 0x3f6fabff, 0x3fffffff,
	0x3fffffff, 0x3bae7b98, 0x38be379b, 0x354d8b6c,
	0x347cf738, 0x323c6f24, 0x2f7bbaf0, 0x2d5b6ad6,
	0x2bbacab2, 0x2a0a8698, 0x284a2a82, 0x2659ae5e,
	0x25493258, 0x23f8f639, 0x22289a24, 0x21a84e0d,
	0x20a80dfc, 0x1f67d1f1, 0x1e6795e1, 0x1d8761d3,
	0x1d2725cf, 0x1ca705bb, 0x1b96d5b7, 0x1b16adb1,
	0x1a8699a7, 0x19d6819d, 0x19965d94, 0x19564597,
	0x19263591, 0x19264189, 0x18f6298a, 0x18862d87,
	0x18662987, 0x1876218a, 0x18a62d8c, 0x1936458a,
	0x18f6518b, 0x19865592, 0x19e66995, 0x19b67da1,
	0x1a769da1, 0x1b86d1a6, 0x1bb6fdb2, 0x1c271dc4,
	0x1cd741d0, 0x1e7761df, 0x1ec7ade7, 0x1f27f1fc,
	0x20e83206, 0x21f8721a, 0x2328ce3e, 0x24e9224a,
	0x25497a54, 0x2739e679, 0x289a629e, 0x2bbad6b7,
	0x2dab52ca, 0x2efbeaee, 0x323c9319, 0x360d2753,
	0x385dcb7f, 0x394e9f93, 0x3cdec3b0, 0x3d7f5bd0,
	0x3fffe3ff, 0x3fffffff, 0x39fe8785, 0x36ddf36f,
	0x363d3b5a, 0x345ca734, 0x301beb0a, 0x2e6b6ada,
	0x2c4b1ac8, 0x2a6a929f, 0x28ca2687, 0x26b9b665,
	0x25796a55, 0x23a9023c, 0x2248a628, 0x21784218,
	0x20f7fe00, 0x1f27cdf3, 0x1de785dc, 0x1d8731d3,
	0x1cf721c1, 0x1c06e5bc, 0x1b16b1af, 0x1a56919f,
	0x19f679a4, 0x19966196, 0x19863591, 0x18c6258a,
	0x18d6218a, 0x1825f57f, 0x1845f97e, 0x1805fd7c,
	0x17c5e97b, 0x17e5f177, 0x17d5ed7b, 0x17d5f17d,
	0x1805f981, 0x18261184, 0x1886198c, 0x18b63d8b,
	0x19564191, 0x1a06659b, 0x1a868da6, 0x1aa69dac,
	0x1b16d9af, 0x1c36fdbc, 0x1c6721cb, 0x1d8751d9,
	0x1e9781e0, 0x1f77e5f3, 0x20983209, 0x21d89620,
	0x23a8e626, 0x24794248, 0x25899e65, 0x291a1685,
	0x29aa969f, 0x2bbb06ac, 0x2e4ba6dc, 0x301c230c,
	0x32bcd338, 0x360dbf53, 0x380e178d, 0x3abe7bb0,
	0x3baf33bd, 0x3def9fdd, 0x3f9fffff, 0x378e1f90,
	0x368dbf50, 0x33cce33b, 0x311c9317, 0x2f7ba6f8,
	0x2c9b6ac6, 0x2b1acab4, 0x295a5a96, 0x2739f275,
	0x2659925b, 0x24c93243, 0x2398b632, 0x2258821f,
	0x21082a09, 0x1f27ddf2, 0x1e9799de, 0x1d8775cd,
	0x1c971dc4, 0x1be6ddbb, 0x1b96bdb2, 0x1a86a9a1,
	0x1a067598, 0x19265591, 0x18c63585, 0x18561584,
	0x17d60980, 0x17f5f17c, 0x17a5dd75, 0x17a5d974,
	0x1725d970, 0x1715b56d, 0x1715c56e, 0x1735cd73,
	0x1755c174, 0x1735cd74, 0x1775dd76, 0x1805f17a,
	0x18260182, 0x18561584, 0x18f63d91, 0x19165599,
	0x1a26759e, 0x1ab6b5a3, 0x1b66cdb3, 0x1c06fdbd,
	0x1cf731cc, 0x1d9769dc, 0x1ed7a1f1, 0x1fe81e00,
	0x21482e13, 0x22489623, 0x23f8fe42, 0x25994e54,
	0x2769d26b, 0x291a368c, 0x2b6ac6a5, 0x2c9b16ba,
	0x301baef6, 0x320c8b13, 0x34ad4f44, 0x372d9f7a,
	0x3a2e2b7f, 0x3cdecfb6, 0x3e5f27cc, 0x3f9fbbff,
	0x380db367, 0x351d4f55, 0x337cb72b, 0x2ffbe306,
	0x2ebb86eb, 0x2b9b06c0, 0x2a1a66a9, 0x288a0282,
	0x26797660, 0x24692646, 0x2318da2a, 0x21e87624,
	0x21383e09, 0x1f47d9f2, 0x1e87ade1, 0x1db75dd8,
	0x1cd735c6, 0x1b96f9b9, 0x1ad6c5a9, 0x1a0689a5,
	0x19d65996, 0x18f6418b, 0x1886118a, 0x1805fd80,
	0x17c5e977, 0x1735d575, 0x16f5c16e, 0x16a5a572,
	0x1655ad69, 0x1685a168, 0x16859166, 0x16858967,
	0x1655a168, 0x16459964, 0x16d5a966, 0x1695b16e,
	0x16f5c571, 0x1755dd72, 0x17b5e97a, 0x18460d83,
	0x18861d8a, 0x18d64595, 0x19b6799b, 0x1a768da9,
	0x1b36c1b0, 0x1c16fdbc, 0x1ce735ce, 0x1d8759d4,
	0x1e97b5f2, 0x1ff7fa00, 0x21684a10, 0x2258aa2c,
	0x2459023a, 0x26196e54, 0x27d9e277, 0x295a3e92,
	0x2b3adeb4, 0x2dcb52df, 0x2f7c0b0f, 0x332caf26,
	0x35bd7f50, 0x375dff85, 0x3bae37aa, 0x3c1f4fc9,
	0x3e1fc7dd, 0x356d974e, 0x339cef47, 0x311c470f,
	0x2f3b9ee5, 0x2d9b4ec0, 0x2a6aaab2, 0x291a268d,
	0x26e9be6b, 0x25b95a58, 0x23e8f242, 0x22889a1f,
	0x2118560f, 0x1ff809f9, 0x1e97a5f0, 0x1d876ddb,
	0x1cc735c8, 0x1b86f5b9, 0x1b06b9ac, 0x1a56899e,
	0x19c65596, 0x19064188, 0x18460188, 0x17e5e17a,
	0x1735dd75, 0x16c5b56b, 0x1695b96c, 0x16558963,
	0x15c5915e, 0x16156d5a, 0x15c57558, 0x15a5655b,
	0x15e55d58, 0x15a5755e, 0x15e5715e, 0x16057d56,
	0x16158161, 0x1625915f, 0x16b5a16c, 0x1725d16a,
	0x1795e173, 0x1785fd7b, 0x18561583, 0x19462d8c,
	0x1986659d, 0x1a7691a7, 0x1b36c1ac, 0x1be701c1,
	0x1c8725ce, 0x1da779d5, 0x1f37c9ec, 0x20680e0a,
	0x20e85e15, 0x22d8ba2e, 0x24c92642, 0x25d99259,
	0x2899ee79, 0x29da8a9c, 0x2d0b2aba, 0x2e0bbae3,
	0x320c3f13, 0x33ccf732, 0x372d7f62, 0x391e1f8d,
	0x3baec398, 0x3d1edbd6, 0x34ad2f38, 0x337ce32d,
	0x2ffc0b04, 0x2e7b8ee7, 0x2b8adeba, 0x297a5698,
	0x27da1a80, 0x26099e60, 0x24492247, 0x22d8ca28,
	0x21985e12, 0x1fc82607, 0x1f47a9f2, 0x1da765db,
	0x1cc731cd, 0x1c070db9, 0x1ae6b1ab, 0x1a268d9d,
	0x19565592, 0x18f6218d, 0x1815f981, 0x1785e574,
	0x1715c570, 0x16c5b56d, 0x16859562, 0x15f5715d,
	0x15d5695a, 0x15756151, 0x15554d57, 0x15553950,
	0x1535314d, 0x14e54d4c, 0x15454555, 0x14d5454f,
	0x15754d50, 0x15755555, 0x15d5655a, 0x15a56d5d,
	0x16859162, 0x16d5ad6b, 0x1755c176, 0x17a5e179,
	0x18160982, 0x1916358c, 0x19865d99, 0x19c685a8,
	0x1ba6c5ae, 0x1c16f9c4, 0x1d4741d3, 0x1e977de5,
	0x1fa7c5f4, 0x20482a11, 0x22288611, 0x2368de36,
	0x25294a5d, 0x27a9b66f, 0x287a2a8a, 0x2b9aaaa5,
	0x2d1b32d3, 0x301beaed, 0x33ec932f, 0x354d6350,
	0x368dbf77, 0x3b1e6f93, 0x3baf03c9, 0x34ad3b24,
	0x31acaf29, 0x2f5bcb00, 0x2d0b3ed1, 0x2a6ab6b1,
	0x291a4285, 0x26e9c662, 0x25895653, 0x2348ca3e,
	0x22e88e1e, 0x21483209, 0x1fe7edf6, 0x1e979de0,
	0x1d973dcd, 0x1c3709c2, 0x1b26d5b3, 0x1ab6a5a3,
	0x1a266d99, 0x1906358b, 0x18360580, 0x1795e97b,
	0x1725d56e, 0x16959d65, 0x16159162, 0x15d5755e,
	0x15855157, 0x15354551, 0x15054d4d, 0x1505294a,
	0x14a5214a, 0x14d51d49, 0x1485294b, 0x14851d47,
	0x1485254a, 0x14e5294a, 0x15053d4d, 0x15555552,
	0x15a55d56, 0x16057d5e, 0x16559161, 0x16b5b169,
	0x1705b96c, 0x1785f175, 0x18360988, 0x18e64d8f,
	0x19866d9a, 0x1aa6a1a9, 0x1b66d5b9, 0x1c6711c8,
	0x1d5759da, 0x1f3799e4, 0x2007f201, 0x21c85215,
	0x22e89622, 0x24a90a48, 0x26097e5e, 0x281a0a83,
	0x2a1a6e8a, 0x2c5b16b7, 0x2e9b9ee5, 0x318c66fe,
	0x340d1350, 0x370dcb6f, 0x388e4f93, 0x3abecfb9,
	0x325cdb40, 0x316c3313, 0x2e0b76d8, 0x2c4b02bc,
	0x2a0a929b, 0x27e9f282, 0x26199e62, 0x24293248,
	0x22e8be2f, 0x21885a18, 0x2027ea00, 0x1f47c1ea,
	0x1dc775d3, 0x1ca71dc1, 0x1bc6d9b5, 0x1ae6a9ac,
	0x19b6659d, 0x1916358e, 0x1826097f, 0x17b5e97b,
	0x1715c56f, 0x16958966, 0x16257960, 0x15c55d56,
	0x15154956, 0x14e52d4d, 0x14c52144, 0x14651944,
	0x14550544, 0x14050d3d, 0x14750542, 0x1425053f,
	0x13f5053c, 0x14450542, 0x14250943, 0x14851943,
	0x14b52547, 0x14d5314f, 0x15455155, 0x15956958,
	0x15c5755f, 0x1675a563, 0x16d5c570, 0x17a5e97f,
	0x18761186, 0x19564594, 0x19d6859b, 0x1a769da4,
	0x1bc6edb7, 0x1ca72dc7, 0x1db765dd, 0x1ea7c9f1,
	0x208819ff, 0x2208521a, 0x2358ca31, 0x25694254,
	0x2699c267, 0x295a428d, 0x2bdaf2b2, 0x2e0b62d3,
	0x301c1afa, 0x32bca736, 0x363d6358, 0x385e0b7f,
	0x39feabb9, 0x31aca72f, 0x307beb04, 0x2d9b52d3,
	0x2acad6b4, 0x291a6693, 0x27a9e67c, 0x25d96256,
	0x2308e236, 0x22389225, 0x2128320a, 0x1f57c9fc,
	0x1df775e5, 0x1d3749d0, 0x1c26f5bf, 0x1af6b5b5,
	0x19f68999, 0x19164590, 0x18561d84, 0x17c5e180,
	0x1755b172, 0x1665a965, 0x15e5755b, 0x1585615a,
	0x15254151, 0x1495214c, 0x14551946, 0x1405013f,
	0x13d50539, 0x1394ed36, 0x1394e53d, 0x1394e538,
	0x1354d539, 0x13b4d537, 0x1394dd3b, 0x13d4e53c,
	0x1444ed3b, 0x14250143, 0x14451946, 0x14b52d4d,
	0x1505414f, 0x15755d54, 0x15f5815f, 0x16559564,
	0x1735c570, 0x1755dd79, 0x18260987, 0x19764d91,
	0x1a2689a5, 0x1b16bda7, 0x1c0705bb, 0x1d574dd1,
	0x1e7791dc, 0x2037e5f3, 0x21383210, 0x22989a27,
	0x24591a4d, 0x25c96e68, 0x280a1e80, 0x29ba92a5,
	0x2ceaf2b9, 0x2f5bcae3, 0x32bc8b24, 0x35ecef49,
	0x375dd362, 0x3a5e439e, 0x303c2afa, 0x2f1baef2,
	0x2cab16b4, 0x29eaa6a2, 0x281a1e8c, 0x26e9b26d,
	0x24c93e53, 0x22a8ba31, 0x21885615, 0x20580209,
	0x1ea7b9ec, 0x1d2775d6, 0x1c9721c8, 0x1b56ddb1,
	0x1a868da7, 0x19965594, 0x18661d86, 0x1815f97c,
	0x1735c570, 0x1695a96c, 0x15f5795d, 0x15856956,
	0x15153d4c, 0x14a51949, 0x14150940, 0x1434f13d,
	0x13a4f535, 0x1384d132, 0x1354dd32, 0x1334c931,
	0x12f4c52f, 0x12f4c932, 0x1324c932, 0x1344d12e,
	0x1364c937, 0x1394e134, 0x13c4e93b, 0x13f4fd37,
	0x14351543, 0x14951d4b, 0x14f5394a, 0x15456155,
	0x15d5755a, 0x16a5a168, 0x1755d56f, 0x17f5f57f,
	0x18d62d8b, 0x19865994, 0x1a5691a6, 0x1b46ddbb,
	0x1c8725be, 0x1dd75ddc, 0x1ed7b5ee, 0x20781604,
	0x21686a1d, 0x22b8e638, 0x25293248, 0x26f9c26d,
	0x297a5a90, 0x2b1adeb7, 0x2e9b72d4, 0x307c2b06,
	0x345d1f2f, 0x375d975f, 0x380df37f, 0x2e0c0302,
	0x2e2b7ee5, 0x2b0ae6be, 0x2a1a86ac, 0x2829e671,
	0x26097e5f, 0x24b8f642, 0x22f89e25, 0x21f83611,
	0x1f87e1f0, 0x1e679dea, 0x1d474dcf, 0x1be6f5c0,
	0x1a66b9ad, 0x1a36759d, 0x19065591, 0x18360982,
	0x1795d976, 0x16a5a96d, 0x16057960, 0x15655d53,
	0x1505494b, 0x14a5214b, 0x14351143, 0x13e4fd3c,
	0x13a4e937, 0x1334d139, 0x1354c12e, 0x12f4c12b,
	0x12f4b529, 0x12b4a92b, 0x12c4b12a, 0x12b4ad2b,
	0x12f4b52e, 0x1334b930, 0x1334c933, 0x1344d537,
	0x1384e937, 0x1404f53b, 0x1425093e, 0x1465294c,
	0x15053550, 0x15b56d5a, 0x1625895c, 0x1685a96b,
	0x1745d578, 0x18261d80, 0x19664992, 0x19d685a1,
	0x1b06b9af, 0x1bd6edc2, 0x1cc741d3, 0x1e479de1,
	0x2027edf9, 0x21183e0e, 0x22a8a634, 0x25793e45,
	0x26b9965f, 0x28ea2680, 0x2abacaa5, 0x2d9b4ecc,
	0x303bfaf4, 0x337ccb22, 0x354d6b5f, 0x378df377,
	0x2ffbbb0c, 0x2d9b52d6, 0x2b0a96b1, 0x297a3293,
	0x2699be62, 0x2519424a, 0x2388e235, 0x21d88624,
	0x20e80208, 0x1f27c9f3, 0x1de761de, 0x1c572dc3,
	0x1b36d9b2, 0x1a56919d, 0x19964d96, 0x18c62188,
	0x17b5f179, 0x16f5bd72, 0x16559164, 0x1585715b,
	0x14e5414f, 0x14752549, 0x14751543, 0x13f4f13c,
	0x1344e134, 0x13a4c92f, 0x12d4b52d, 0x12c4ad28,
	0x12849926, 0x1264a125, 0x12549524, 0x1274a526,
	0x12848d28, 0x1294a124, 0x12b4a929, 0x12c4a92d,
	0x1314c12e, 0x1344c532, 0x1364dd33, 0x13c5053a,
	0x14351542, 0x14852549, 0x14f5394f, 0x1575695a,
	0x16658564, 0x16f5b970, 0x17e5ed80, 0x18c62588,
	0x19564d95, 0x1a66a5a9, 0x1b86edb6, 0x1c4721c8,
	0x1e1765d9, 0x1f07d1e7, 0x20e81613, 0x22287a25,
	0x2438d63e, 0x25996e55, 0x27d9e67c, 0x2a3a6e9c,
	0x2c7b0eca, 0x2f9bb6f0, 0x320c6f0a, 0x351d1f53,
	0x372dbf72, 0x2d7b96ed, 0x2bbb22c6, 0x2a9a9ea2,
	0x287a0282, 0x2669b264, 0x24e90e46, 0x2288ba28,
	0x20e84a17, 0x1f87e9f2, 0x1ea791df, 0x1d5751d1,
	0x1bd6e5c1, 0x1b06bdab, 0x19d679a3, 0x18f6358d,
	0x1855f97c, 0x1735c172, 0x16e5a166, 0x15e5755c,
	0x15555152, 0x14b5314c, 0x14450d41, 0x13d4ed40,
	0x1374dd38, 0x1314c12d, 0x12e4a52c, 0x12949927,
	0x12649924, 0x12048d23, 0x11f48523, 0x12148121,
	0x1244811e, 0x11f4891f, 0x11f48525, 0x11f48925,
	0x12749524, 0x1294a127, 0x12a4ad2d, 0x1314cd32,
	0x13a4cd34, 0x1404ed3c, 0x14650d46, 0x14d5354a,
	0x15454950, 0x16156d5d, 0x16959d69, 0x1735d175,
	0x18360981, 0x18c63993, 0x19c691a2, 0x1b56bdb3,
	0x1be70dbc, 0x1d474dd6, 0x1eb799e3, 0x200805fe,
	0x21587210, 0x2308be35, 0x24793e4c, 0x26a9ae6d,
	0x2a1a369b, 0x2b1b0eb7, 0x2e2b96e5, 0x316c0b0f,
	0x34ad1f40, 0x360d2f53, 0x2e7b52cd, 0x2b6b36b9,
	0x295a869c, 0x2879de77, 0x25697e58, 0x23c8fa41,
	0x22a89621, 0x20d8120d, 0x1ee7e5f6, 0x1d8771dc,
	0x1cc725d0, 0x1bc6ddb0, 0x1aa6a9a9, 0x19965597,
	0x18c61d82, 0x17d5f180, 0x16d5b572, 0x16258561,
	0x1525615b, 0x14e5414b, 0x14a50d42, 0x13d4fd3b,
	0x13d4dd34, 0x1344c12f, 0x12c4ad29, 0x12a4a526,
	0x11d49526, 0x12247d21, 0x1204791f, 0x11f4751b,
	0x11f46d1c, 0x1194691c, 0x11a4711b, 0x11a4751c,
	0x11d4811e, 0x12048520, 0x12348525, 0x12a49d29,
	0x12e4ad29, 0x1324c132, 0x1364d938, 0x1404f53a,
	0x14c51145, 0x14e5394c, 0x15556554, 0x15e58561,
	0x16d5b56c, 0x1795f17c, 0x18862989, 0x19865997,
	0x1a36a9ae, 0x1b86d9b3, 0x1cb72dc7, 0x1e5781e0,
	0x1f37ddf4, 0x21085e0e, 0x22989226, 0x24c8fe43,
	0x2609965d, 0x292a4a90, 0x2b1ad2b5, 0x2dab4ecd,
	0x311c12fa, 0x320caf2b, 0x356d6b38, 0x2c7b6ad3,
	0x2b8afab2, 0x29da3292, 0x2739c27e, 0x26693a55,
	0x2398e641, 0x21d88616, 0x20b81a00, 0x1ec7c1ed,
	0x1d4759d9, 0x1c5715c3, 0x1b36d5b1, 0x19e6959e,
	0x19464195, 0x18560182, 0x1795e574, 0x1685a968,
	0x15d5855c, 0x15254558, 0x14652147, 0x14350941,
	0x13c4e539, 0x1324c934, 0x12f4b529, 0x1264a126,
	0x12849923, 0x12247d21, 0x11f4711c, 0x11a46917,
	0x11c46918, 0x11845d15, 0x11d45d1c, 0x1184591b,
	0x11646118, 0x11847119, 0x11c4691e, 0x11f48520,
	0x12648d20, 0x1284a526, 0x1304b930, 0x1334c935,
	0x1384f139, 0x13d50d41, 0x14b53549, 0x15555152,
	0x15b5695f, 0x1685a16d, 0x17a5d575, 0x18c62187,
	0x19065199, 0x1a3685a4, 0x1b56d5b5, 0x1c5705c1,
	0x1d8769e0, 0x1ef7cdff, 0x20782a04, 0x22387e2c,
	0x2438fe45, 0x25c9765d, 0x280a1a85, 0x2aea92af,
	0x2ceb5ad3, 0x301bbaee, 0x33ccdb38, 0x339d633d,
	0x2b8b06d3, 0x2a3a92b1, 0x280a268d, 0x2699a270,
	0x24793648, 0x22f8d631, 0x21285618, 0x1fc7edf9,
	0x1ee7addf, 0x1d274dcf, 0x1ba6edbb, 0x1af6a5af,
	0x19b669a0, 0x18c6298a, 0x17d60578, 0x1705b96b,
	0x16158966, 0x15956157, 0x14e53553, 0x14251943,
	0x13b4ed3d, 0x1384c936, 0x12d4b52d, 0x12a49927,
	0x11f48d20, 0x11d47d1c, 0x11a4711c, 0x11a46117,
	0x11745517, 0x11245515, 0x11545112, 0x11545916,
	0x11045118, 0x11445914, 0x11845918, 0x11c46918,
	0x11c47519, 0x12147922, 0x12148524, 0x12749d26,
	0x12e4b52c, 0x1334d533, 0x13c4f93f, 0x14251547,
	0x14c54150, 0x1555695d, 0x16658563, 0x16f5b973,
	0x17c5f982, 0x18963595, 0x19f6759a, 0x1b06b9b0,
	0x1c06f9bd, 0x1db759d3, 0x1ea799e8, 0x1fe7e9ff,
	0x21d86a15, 0x23a8de34, 0x25696a48, 0x2769d675,
	0x29da7e9c, 0x2d5b02b9, 0x2e9baedf, 0x32bc8319,
	0x345cd33d, 0x2bbaeaba, 0x29ba96a4, 0x27d9e66f,
	0x2589825b, 0x2449063f, 0x21f89e24, 0x20f83e12,
	0x1f57d5f8, 0x1e3781e3, 0x1c7725d1, 0x1b36ddba,
	0x1a2685a6, 0x19a65192, 0x18961d88, 0x17d5dd77,
	0x16b5b169, 0x15d5755e, 0x15755151, 0x14b53149,
	0x1434fd3e, 0x1374e139, 0x1314c932, 0x12c4a927,
	0x12249126, 0x1214811e, 0x11c46517, 0x11945d18,
	0x11745d17, 0x11544914, 0x11044911, 0x10f43d12,
	0x10e44d10, 0x11243913, 0x11344112, 0x11544913,
	0x11445116, 0x11746919, 0x11a45d1b, 0x12247520,
	0x12448923, 0x12a4a929, 0x1314c531, 0x13a4e138,
	0x14350144, 0x14e52d4a, 0x15255557, 0x16257d60,
	0x16b5b16b, 0x1775e17c, 0x18762985, 0x1906559b,
	0x1ad69da6, 0x1bc6f1bc, 0x1d372dcb, 0x1e479ddd,
	0x1ff7e9fa, 0x21784a12, 0x2308ca2e, 0x25393e46,
	0x2709c269, 0x297a7a8a, 0x2c0ae6bc, 0x2f7b76f0,
	0x303c6715, 0x33ed433d, 0x2acad2b2, 0x298a7ea1,
	0x26d9e285, 0x25795a50, 0x2338de35, 0x2228861c,
	0x20b81e10, 0x2007b1f1, 0x1d8779d9, 0x1ca719c3,
	0x1b26c5af, 0x1a3679a1, 0x1936518c, 0x1815f181,
	0x1755c576, 0x16858962, 0x15a5695b, 0x14f5494e,
	0x14651942, 0x13b4f93b, 0x1384d934, 0x12f4b52b,
	0x12649d27, 0x1204811e, 0x11b47517, 0x11445917,
	0x11244917, 0x11144512, 0x10d4390f, 0x10c4350d,
	0x10f4290d, 0x10f4390e, 0x10c4350d, 0x10f4410f,
	0x11143910, 0x11044911, 0x11444514, 0x11445918,
	0x1164711e, 0x11f4851e, 0x12549928, 0x12b4ad2d,
	0x1334d137, 0x13c4f541, 0x13f50945, 0x14d54d4e,
	0x15d5595c, 0x16859165, 0x1745c578, 0x18160d82,
	0x19865591, 0x1a569da1, 0x1b76e5b5, 0x1cc719cb,
	0x1de769e7, 0x1f47edf8, 0x2138520a, 0x22589a20,
	0x23c93640, 0x2769a66b, 0x297a4e93, 0x2bbaf2b5,
	0x2e0b86df, 0x2ffc2b17, 0x329cdb38, 0x2a6ad2ac,
	0x298a569e, 0x2899d274, 0x25395a53, 0x23b8e22f,
	0x2188661d, 0x20780208, 0x1ed7b9ed, 0x1d8765d2,
	0x1c0701bc, 0x1b46bdac, 0x1a3675a0, 0x19062991,
	0x17c5f580, 0x16d5bd6d, 0x16659164, 0x15855956,
	0x14c5354f, 0x14650142, 0x13b4d93b, 0x12f4d935,
	0x1284b128, 0x12248d25, 0x1204811d, 0x11c4691c,
	0x11745113, 0x11244912, 0x10f43d0f, 0x10f4350f,
	0x10b42d09, 0x10b4290a, 0x10942d0d, 0x10a4290a,
	0x10e4310b, 0x10c4390e, 0x10b44110, 0x11444d13,
	0x11745517, 0x11b4651a, 0x11f4791e, 0x11f48525,
	0x12c4a92b, 0x1334c933, 0x13e4ed3c, 0x14450945,
	0x15253951, 0x15855d58, 0x16559962, 0x1715c578,
	0x1805f982, 0x19364591, 0x1a0675a4, 0x1b76c5b5,
	0x1c5721c5, 0x1d5765e0, 0x1ee7c9f3, 0x21382a06,
	0x2318ae2d, 0x2459064a, 0x26999258, 0x281a1e96,
	0x2acacaaa, 0x2d3b4ed8, 0x309c5702, 0x329c6f29,
	0x2a9ac6b2, 0x28ea6696, 0x2669b675, 0x25491e45,
	0x2308ba3a, 0x21887613, 0x1fe805f5, 0x1ee795ed,
	0x1ce751d1, 0x1ba6edbf, 0x1a66adad, 0x19866d9d,
	0x18e6298e, 0x1775ed80, 0x1725b170, 0x16158166,
	0x15554d58, 0x14a51d4e, 0x1404fd3c, 0x1384dd37,
	0x12e4bd2c, 0x1274a12a, 0x12048125, 0x11e4691e,
	0x11845514, 0x1144550f, 0x10e4410f, 0x10c4410e,
	0x10b42d0c, 0x10a4290a, 0x1084250a, 0x10642909,
	0x10c42107, 0x1084210b, 0x10a4350b, 0x10c4350d,
	0x10f44511, 0x11345117, 0x11746518, 0x11746d1d,
	0x12447d21, 0x1244a92b, 0x1324cd32, 0x1384dd37,
	0x14050545, 0x14f52547, 0x15055157, 0x15e58962,
	0x16f5c173, 0x17d5ed7c, 0x19062d8d, 0x1a06799d,
	0x1af6b9ae, 0x1c26fdbe, 0x1d5759dc, 0x1ef7adf0,
	0x205811ff, 0x21c89a22, 0x2428fe40, 0x25e98a5a,
	0x284a167e, 0x2aea96a5, 0x2dab5ad6, 0x307c0b0c,
	0x320caf1b, 0x2a1a86b2, 0x285a368a, 0x2629925f,
	0x24b90a49, 0x2268b229, 0x21385217, 0x1fe7e5f8,
	0x1e1791e1, 0x1cf735d0, 0x1b76f1bc, 0x1a86a5ab,
	0x19b66599, 0x18862d84, 0x17e5d17b, 0x1665b165,
	0x15d57d60, 0x15454551, 0x14851d49, 0x1404f53b,
	0x1344dd36, 0x1304b931, 0x12849524, 0x11d4791e,
	0x11946916, 0x11545117, 0x10f44115, 0x10e4310f,
	0x10b42d09, 0x1084290a, 0x10541d07, 0x1074250b,
	0x10641909, 0x10a41d07, 0x10741908, 0x10942908,
	0x10b4310d, 0x10e4390f, 0x10f44512, 0x11244916,
	0x11d46d1c, 0x12047924, 0x12849928, 0x12d4b932,
	0x1374d535, 0x13e4f53d, 0x14b5254a, 0x15855954,
	0x15d57560, 0x16d5bd6b, 0x17e5ed78, 0x18662d8a,
	0x19f66d92, 0x1a86b9a9, 0x1bd6f5bc, 0x1d774ddc,
	0x1f37adf1, 0x1fa81203, 0x22485214, 0x23e8fa35,
	0x26097e52, 0x282a0a7c, 0x2a4a7eaa, 0x2e0b4eca,
	0x2ffbfb0a, 0x330c8329, 0x294a9290, 0x282a1e7f,
	0x25e98a5a, 0x23e90e3c, 0x22d89224, 0x21083e12,
	0x1f57e1f1, 0x1e6769e0, 0x1ca72dc7, 0x1b56c5b7,
	0x1a5699a5, 0x19865999, 0x18460980, 0x1785d175,
	0x16a59967, 0x15e5755e, 0x15153d4f, 0x14451545,
	0x13c4ed3c, 0x1334c933, 0x1284a92a, 0x12248d25,
	0x11d47121, 0x11845d16, 0x11344912, 0x1104390e,
	0x10a4350f, 0x10b4210b, 0x10841d08, 0x10241906,
	0x10241907, 0x10541505, 0x10441103, 0x10641506,
	0x10841d06, 0x1094290e, 0x10c4290d, 0x11044510,
	0x11444912, 0x11745d1a, 0x12147921, 0x12448d26,
	0x12c4ad29, 0x1354c932, 0x13f4f140, 0x14652545,
	0x14f54951, 0x15c57d5f, 0x1665b16c, 0x1795e973,
	0x18c62d88, 0x19466595, 0x1aa6a5a6, 0x1c06f5c1,
	0x1c7741d3, 0x1ea79dea, 0x20380204, 0x20b8821e,
	0x2388da38, 0x25896e5a, 0x2769de87, 0x2a1a7eaa,
	0x2cab36d4, 0x2efc12fe, 0x339c9b2d, 0x29aa6690,
	0x285a1682, 0x25d9625e, 0x24290a3a, 0x22289e21,
	0x20d83e12, 0x1f47ddee, 0x1e3771df, 0x1c7721c7,
	0x1b26c9b8, 0x19f691a4, 0x18f64d95, 0x18660d83,
	0x1725d577, 0x16559d67, 0x15856d58, 0x14e5494f,
	0x14751546, 0x13b4e13d, 0x1314c533, 0x12a4a529,
	0x12248523, 0x11c4791c, 0x1144551a, 0x11344913,
	0x10e43d0f, 0x10a4250b, 0x10b4210c, 0x10741908,
	0x10541504, 0x10640d08, 0x10641903, 0x10441506,
	0x10541903, 0x10642107, 0x1094210b, 0x1094310d,
	0x10e4410f, 0x11444913, 0x11645d1a, 0x11c4711f,
	0x12449924, 0x1284ad2f, 0x1324c932, 0x13b4ed3a,
	0x14850d49, 0x14d54950, 0x15c5755c, 0x16b5b168,
	0x1755ed7a, 0x18b61d86, 0x19a65997, 0x1ab6b5a9,
	0x1bb6f9bc, 0x1ce739d6, 0x1e67a5ec, 0x20080dfe,
	0x21f8761d, 0x23590236, 0x24e97658, 0x27d9fa7e,
	0x2a4a9eb4, 0x2cab3ecc, 0x2ffbdb02, 0x311c8b1b,
	0x297a4290, 0x27a9fe89, 0x24d96e65, 0x23c8ea42,
	0x22389626, 0x20e82612, 0x1f67e9f8, 0x1d8765da,
	0x1be70dd0, 0x1b76e1b3, 0x19f6899b, 0x1946458c,
	0x18361989, 0x1715c972, 0x16659163, 0x1565595a,
	0x1485294d, 0x14550941, 0x1374dd3a, 0x1314c931,
	0x1284a129, 0x12448524, 0x11c4711b, 0x11945516,
	0x10f44911, 0x10f4390c, 0x10a4290a, 0x10842107,
	0x10741505, 0x10241103, 0x10541501, 0x10341104,
	0x10141505, 0x10741506, 0x1074210a, 0x10742109,
	0x10b4290a, 0x10d4390e, 0x11444916, 0x11446115,
	0x11d4791e, 0x12348522, 0x1274a529, 0x1314c534,
	0x13a4ed3f, 0x1425153f, 0x15354552, 0x15f5695f,
	0x1655a969, 0x1765d979, 0x18761588, 0x19865d95,
	0x19c6adac, 0x1bc6f1bd, 0x1d0741d3, 0x1e67a9e7,
	0x207801fe, 0x21686a17, 0x2328da3a, 0x25c9825b,
	0x281a027f, 0x2a1a72a1, 0x2cab2ad8, 0x2ebbf30a,
	0x31cc8b1b, 0x294a6686, 0x27d9f27e, 0x2599765b,
	0x2348e63e, 0x21e87e1d, 0x2078160a, 0x1f47a5f3,
	0x1db74dd4, 0x1c070dbb, 0x1b46c1b1, 0x19b67da5,
	0x18864190, 0x18960582, 0x16c5c573, 0x16258564,
	0x1575615a, 0x14b52954, 0x1404fd41, 0x1394e93c,
	0x12a4c934, 0x12349929, 0x11f4851f, 0x11b46918,
	0x11645916, 0x10f44514, 0x10c42d0e, 0x1084290b,
	0x1064290a, 0x10541507, 0x10240906, 0x10140d03,
	0x10540100, 0x10140d03, 0x10540d07, 0x10641d09,
	0x10941d0a, 0x10b4290d, 0x10e4350e, 0x11244916,
	0x11345515, 0x11c4711f, 0x11e48524, 0x12a4a928,
	0x1344c937, 0x13b4f13c, 0x14550d42, 0x14f54550,
	0x15956d5f, 0x1685a168, 0x1745dd7c, 0x18761d89,
	0x1976599b, 0x1a66b9a7, 0x1bb6f5b9, 0x1d0751d5,
	0x1e679de9, 0x1fe7edfb, 0x2108761e, 0x2348e633,
	0x25996a5d, 0x2859fa85, 0x2aba969b, 0x2ccb32cf,
	0x2efc02f4, 0x31ac8b19, 0x2a4a2a90, 0x2859ce7b,
	0x25896a54, 0x23a8ca34, 0x21687a1c, 0x20880201,
	0x1f57c9f4, 0x1de769da, 0x1ca70dc5, 0x1b26c5b7,
	0x19e67d9d, 0x1936458c, 0x17e5fd7c, 0x1735cd6f,
	0x16458564, 0x1575655b, 0x1505294a, 0x1424f943,
	0x1394d937, 0x12f4c532, 0x12549d26, 0x1214851e,
	0x11b4611d, 0x11545d15, 0x10f44110, 0x10942d0d,
	0x10a42508, 0x10642109, 0x10741507, 0x10241104,
	0x10140d05, 0x10440d06, 0x10240904, 0x10740d02,
	0x10441906, 0x10842508, 0x10a4310b, 0x10e43d0f,
	0x11344515, 0x11944d16, 0x11c46d1c, 0x12148124,
	0x12849d2d, 0x1314cd35, 0x13b4f139, 0x14050d46,
	0x15054153, 0x15b56d60, 0x1685a966, 0x1715d975,
	0x18862185, 0x19265596, 0x1a86a9a9, 0x1b56fdc0,
	0x1cd735cf, 0x1ea79df1, 0x20180a00, 0x2148761c,
	0x23a8ca40, 0x25996e60, 0x282a1682, 0x297a96ba,
	0x2d0b32cd, 0x2f3bdb04, 0x30dc8b11, 0x28ca329e,
	0x2779fa82, 0x24d9665b, 0x2338c639, 0x22087a24,
	0x20880a07, 0x1f17bdeb, 0x1d8761d8, 0x1c7705c2,
	0x1b86c1b1, 0x19f6859e, 0x1906398b, 0x17b5f183,
	0x1705c972, 0x1635896c, 0x15756157, 0x14b5314d,
	0x13d50540, 0x1374d53c, 0x1304c12e, 0x1264a528,
	0x1214851f, 0x11a46d1a, 0x11545517, 0x1104450e,
	0x10e4350c, 0x1094290a, 0x10641d07, 0x10641508,
	0x10241503, 0x10140903, 0x10241502, 0x10240904,
	0x10241905, 0x10641906, 0x1094250e, 0x10c42d0d,
	0x10c44510, 0x11144d15, 0x11845918, 0x1194751c,
	0x12148523, 0x12a4a52a, 0x1334c533, 0x1384f13c,
	0x14350d48, 0x15254552, 0x15f5755d, 0x1685b16a,
	0x1735e976, 0x18760d8a, 0x1946599d, 0x1a669da9,
	0x1b76f5b9, 0x1c9741d3, 0x1e9799ea, 0x2017f1fc,
	0x2118721f, 0x2318de3a, 0x25896a5b, 0x2829e67f,
	0x2aba7ab1, 0x2dab1ad8, 0x2ffbd2fe, 0x325c8b20,
	0x29ba6693, 0x2889e66f, 0x25d96e52, 0x22f8de2f,
	0x2188721e, 0x1fe80607, 0x1ed7c5ed, 0x1d8765d0,
	0x1c870dc3, 0x1b26b9b3, 0x1a267d9a, 0x1906358d,
	0x1805ed7c, 0x1735bd73, 0x16258d69, 0x15955d5a,
	0x14b5314a, 0x14250945, 0x1374d938, 0x12f4bd30,
	0x1254a52b, 0x11d48123, 0x11646919, 0x11545118,
	0x1124450f, 0x10e4390d, 0x10d4290a, 0x1064210a,
	0x10841104, 0x10241503, 0x10241506, 0x10041106,
	0x10541505, 0x10241507, 0x10742106, 0x1084250c,
	0x10b4390a, 0x10e43912, 0x11044913, 0x11846919,
	0x11d4751f, 0x12449122, 0x1274ad2f, 0x1334d135,
	0x13a4e941, 0x14851947, 0x15054154, 0x15c5795e,
	0x16c5a96d, 0x1765ed79, 0x18a61d89, 0x19b64d96,
	0x1a66b1ac, 0x1be6edbb, 0x1d2759cf, 0x1ee7a1e8,
	0x20281608, 0x2168921a, 0x2388fa35, 0x26095a5b,
	0x28ba1682, 0x2aeacaad, 0x2d7b4ed4, 0x2f1c130f,
	0x311cb72f, 0x297a4a90, 0x2779f280, 0x25b9625b,
	0x2448e239, 0x2188661c, 0x20382611, 0x1ec7b9f5,
	0x1da751d3, 0x1ca709c0, 0x1b26d9b6, 0x19c6759e,
	0x18763193, 0x17f5fd82, 0x16f5c972, 0x16159163,
	0x15556958, 0x1485394c, 0x14050143, 0x1374ed37,
	0x1304c931, 0x12a4a52d, 0x12148522, 0x11a4791a,
	0x11445517, 0x11044114, 0x10b4350e, 0x10c4310a,
	0x1084250b, 0x10541d06, 0x10340d06, 0x10241905,
	0x10441103, 0x10741906, 0x10641506, 0x1094210a,
	0x10841d08, 0x1094350d, 0x10d44513, 0x11245515,
	0x1184691a, 0x11f47d1f, 0x12349526, 0x12d4b92f,
	0x1354c935, 0x13a4f941, 0x14851d48, 0x15254953,
	0x15e57963, 0x16a5b96e, 0x1795e57f, 0x18d62184,
	0x1936659a, 0x1a96c5aa, 0x1c26f9be, 0x1d0745da,
	0x1ec7adf5, 0x20a80a07, 0x21d88623, 0x2428e63a,
	0x25b9826c, 0x282a168d, 0x2b0acab7, 0x2e2b62d1,
	0x2f5beaf6, 0x32ecbf1b, 0x27da728d, 0x2759fe74,
	0x25c96e54, 0x2398d23a, 0x21c87226, 0x20480203,
	0x1e87b9f6, 0x1d7755db, 0x1c2711c3, 0x1b26d5b9,
	0x1a26759e, 0x18c64192, 0x1845fd7c, 0x1775c972,
	0x16459964, 0x1585655b, 0x15153552, 0x14351546,
	0x13c4f138, 0x1304c133, 0x12a4ad2a, 0x12348d23,
	0x1194711c, 0x11645d14, 0x11344914, 0x10d4350e,
	0x10c4350f, 0x1084210f, 0x10741d09, 0x10641907,
	0x10341906, 0x10241508, 0x10741909, 0x10641d0a,
	0x10a4290b, 0x1084310e, 0x10d43110, 0x11144910,
	0x11345518, 0x11546d1d, 0x11e47920, 0x12849926,
	0x12b4b931, 0x1354d53b, 0x13a4f141, 0x14a52d4b,
	0x1575495a, 0x15f58965, 0x16d5b171, 0x17d5e97e,
	0x18f63993, 0x19e67d9b, 0x1a56c5b5, 0x1c470dbf,
	0x1d2751d4, 0x1f07adf3, 0x20082207, 0x22089214,
	0x23690e4c, 0x26599e64, 0x289a4287, 0x2acacab5,
	0x2edb72da, 0x311c02f6, 0x325cb724, 0x289a668d,
	0x2759e67b, 0x25295664, 0x2398e643, 0x21e88220,
	0x1fc8220c, 0x1ef7c9f3, 0x1d8765db, 0x1c3719c6,
	0x1b46c5b8, 0x1a3681a1, 0x19463992, 0x17f60587,
	0x1745cd72, 0x16a59d66, 0x1585715c, 0x14f54552,
	0x14751545, 0x13a4e93d, 0x1324d938, 0x12b4a92d,
	0x12348d24, 0x11d4811e, 0x11b46116, 0x11445516,
	0x11043914, 0x10e4310d, 0x10b4310c, 0x1084250b,
	0x10a41d09, 0x1074190a, 0x10741907, 0x10842508,
	0x10842109, 0x10a42d0a, 0x10a4350d, 0x10b43d0e,
	0x11244115, 0x11845d19, 0x11d4711f, 0x12548d23,
	0x12849929, 0x1324c130, 0x1364e136, 0x1424f543,
	0x1485314a, 0x15355155, 0x15f58d62, 0x16d5c578,
	0x17b60987, 0x18f6298f, 0x19b66da4, 0x1b66c9b3,
	0x1c271dcd, 0x1d9775d3, 0x1f67b5f0, 0x2098260d,
	0x2268962e, 0x24792a48, 0x2679b674, 0x295a3693,
	0x2b8adeb7, 0x2d7b8ed8, 0x307c3322, 0x329caf2f,
	0x28ea5a9b, 0x27d9fe86, 0x25d97a59, 0x2338de4a,
	0x21989627, 0x20a8220f, 0x1f07c9f8, 0x1db78ddc,
	0x1be721ce, 0x1b26cdb3, 0x1a2691a5, 0x19064d8e,
	0x18260987, 0x1755d176, 0x16a5a56d, 0x15c5755e,
	0x14f54152, 0x1495254c, 0x1404fd3f, 0x1334d938,
	0x12e4b532, 0x12449926, 0x12148120, 0x11d47119,
	0x1134551a, 0x11144911, 0x10f4410f, 0x10c4310d,
	0x10a42d0f, 0x10a42d0c, 0x1074250c, 0x1094310a,
	0x1094290a, 0x10b42d0b, 0x10a4390e, 0x10f4390e,
	0x11144512, 0x11245517, 0x1174651b, 0x11e47d1f,
	0x12648d26, 0x12d4a52a, 0x1344c933, 0x1394f13d,
	0x14050d42, 0x14f5394b, 0x15a5595b, 0x16858964,
	0x16d5cd76, 0x17b6117f, 0x18c64195, 0x1a2681a4,
	0x1b36d1b8, 0x1c772dc5, 0x1df781dc, 0x1ed7d9f4,
	0x20d85a11, 0x22f8b22c, 0x2449264d, 0x2659be73,
	0x291a5a92, 0x2beaf2c0, 0x2e7baedc, 0x323c5f0c,
	0x329cbf3d, 0x294a4a99, 0x27d9fa80, 0x2589825e,
	0x2368fe46, 0x21c89622, 0x20383610, 0x1f57c1f0,
	0x1d4775e5, 0x1cf721c6, 0x1b86e9b0, 0x1a4691a5,
	0x19065599, 0x18461586, 0x1755cd73, 0x1695b16a,
	0x15c5755f, 0x15454d52, 0x14a51548, 0x14050142,
	0x1374dd39, 0x12e4b92f, 0x12849d2c, 0x12148126,
	0x11a47d1f, 0x11845d19, 0x11245114, 0x11145117,
	0x10e43d10, 0x10b4390d, 0x10e42d0d, 0x10942d0d,
	0x10a42d0c, 0x1104310e, 0x10e43d12, 0x10e4310d,
	0x10f44111, 0x11544d17, 0x11846517, 0x11c4751d,
	0x11e48923, 0x12249927, 0x12e4bd2f, 0x1354d533,
	0x13c4f53d, 0x14451d4b, 0x14e54d54, 0x15d57562,
	0x1695a56c, 0x1745d976, 0x18360d84, 0x19565999,
	0x1a4691aa, 0x1b26cdbf, 0x1cc72dd0, 0x1dc791e0,
	0x1f87c9fa, 0x20b84211, 0x2318ba29, 0x24c93649,
	0x2769d275, 0x2a0a6e9f, 0x2beb02ba, 0x2efb96eb,
	0x30fc5726, 0x32eccb40, 0x2a1a7e9e, 0x288a0a87,
	0x2629565f, 0x23f9064c, 0x22289226, 0x2068460f,
	0x1f57ddfc, 0x1d5781e4, 0x1cf71dd5, 0x1ba6f5ba,
	0x1a66a9a9, 0x1956759a, 0x1856218a, 0x17a5e97e,
	0x1685b172, 0x16258964, 0x15a5595a, 0x14753151,
	0x14350946, 0x13f4ed3a, 0x1314cd34, 0x12b4b12d,
	0x12649127, 0x11c47d22, 0x11a4711b, 0x11645917,
	0x11345118, 0x11145111, 0x11044510, 0x10c43d10,
	0x1104390f, 0x10f43512, 0x10d43911, 0x10f43d10,
	0x11245113, 0x11444917, 0x1164551a, 0x1194711c,
	0x12147921, 0x12248923, 0x1284ad28, 0x1304c936,
	0x13b4dd3a, 0x14450d45, 0x1445294c, 0x15655558,
	0x15f58562, 0x16d5b56e, 0x1775e982, 0x1876358c,
	0x1976699b, 0x1a56b1b1, 0x1be6f5c1, 0x1cb735d2,
	0x1e0781ed, 0x1f9801ff, 0x21786e1c, 0x2368ea38,
	0x24b95262, 0x27da0274, 0x29ea7eaa, 0x2beb16cc,
	0x2edbcaee, 0x316c8317, 0x32ed2f29, 0x28ca7aa5,
	0x292a0a98, 0x25b99e69, 0x24491e40, 0x22889a25,
	0x20d84a12, 0x1fc7ddfa, 0x1e6785e3, 0x1c8735ce,
	0x1b76edc2, 0x1a46ada9, 0x1986699b, 0x18762d8f,
	0x17d5e97d, 0x16c5c173, 0x16358965, 0x15655d5a,
	0x14e54550, 0x14451549, 0x13c4f13f, 0x1384d138,
	0x12c4b931, 0x12749128, 0x12049124, 0x11f47120,
	0x11c4651a, 0x11645519, 0x11345513, 0x11144916,
	0x11144513, 0x11043d11, 0x11043510, 0x11444915,
	0x11144116, 0x11445116, 0x11345517, 0x1194691a,
	0x11d47920, 0x11c48d27, 0x1224992c, 0x12d4b530,
	0x1324cd3a, 0x13a4e93d, 0x14051945, 0x14853154,
	0x15655d58, 0x16858567, 0x16c5c575, 0x1765fd84,
	0x1886398b, 0x19d679a0, 0x1ae6c5b3, 0x1be6f5c8,
	0x1d2755d5, 0x1ea795f5, 0x1fe80608, 0x21a86623,
	0x2448fe47, 0x2599926d, 0x2769fe7b, 0x2acaaa9e,
	0x2c9b32ca, 0x2efbfafa, 0x316caf3b, 0x339cdb55,
	0x294a9696, 0x282a1e8d, 0x26299e6c, 0x25194256,
	0x2258b227, 0x21383615, 0x1f67f1fb, 0x1e4781e6,
	0x1d7751d6, 0x1c2705bd, 0x1ab6adb5, 0x19e675a7,
	0x18a6318d, 0x18160182, 0x1755c976, 0x16759969,
	0x15c5755c, 0x15254952, 0x14653147, 0x13f50d3e,
	0x1364ed3a, 0x1304c532, 0x1294a52d, 0x12249929,
	0x12048524, 0x11c47d1c, 0x11b47118, 0x11945d1b,
	0x11545d15, 0x11145514, 0x11345113, 0x11545517,
	0x11445914, 0x11845917, 0x1174591b, 0x1194691e,
	0x11b4751d, 0x12148d26, 0x1244a128, 0x12a4b12f,
	0x12f4c935, 0x1384e938, 0x13f50144, 0x1435254b,
	0x15254d5a, 0x15c57561, 0x16b59d6e, 0x1755dd79,
	0x17f61984, 0x1946419d, 0x19f689a1, 0x1ad6d5b9,
	0x1c5715c7, 0x1d9761e0, 0x1ec7adf3, 0x20782a0a,
	0x22989227, 0x24290e3f, 0x25c9b671, 0x28ea1693,
	0x2a3ae6c3, 0x2d0b46e1, 0x2f5bfb0a, 0x33cc6f3b,
	0x345d0365, 0x2a1aaab2, 0x298a4a99, 0x26e9be73,
	0x25694255, 0x2348b638, 0x2148821c, 0x1fc801f8,
	0x1e77a1e8, 0x1d874dd8, 0x1c2705c0, 0x1ad6c1b8,
	0x199679a1, 0x18764995, 0x18161182, 0x1715e179,
	0x16c5a96b, 0x15f58564, 0x15155d58, 0x14b53548,
	0x13f51542, 0x13c4f542, 0x1324d139, 0x12f4b531,
	0x12a4a52b, 0x12748d27, 0x12448523, 0x11d47d20,
	0x11c47d1d, 0x11a46d1c, 0x1164691d, 0x11945d1a,
	0x11645d18, 0x1184691c, 0x11746d1f, 0x11a47d22,
	0x1204791d, 0x12249524, 0x12649928, 0x1294a52b,
	0x12a4c134, 0x1394dd38, 0x13d4ed3e, 0x14651d4a,
	0x14e5454a, 0x15855d5d, 0x16458165, 0x16a5ad6c,
	0x17d5e97e, 0x18563188, 0x19c6699b, 0x1b16a9a4,
	0x1ba6e9bd, 0x1c772dce, 0x1de781ea, 0x1f47e1f8,
	0x21384a0e, 0x22e8b232, 0x2439424a, 0x2629c26c,
	0x28ca3299, 0x2bbad2bc, 0x2e7b4ee9, 0x2f9c0313,
	0x327cef44, 0x330d4355, 0x2a4a9eb4, 0x292a4e95,
	0x26e9ce75, 0x25196a55, 0x2348f238, 0x21e86a18,
	0x20181e08, 0x1f17b1f5, 0x1db755d4, 0x1c46fdc5,
	0x1b26c1b7, 0x1a3691a5, 0x19465595, 0x18661988,
	0x17a5e577, 0x16e5c16e, 0x1645956a, 0x1595615d,
	0x15154954, 0x14b52d49, 0x14350d41, 0x13b4e53b,
	0x1324d134, 0x12f4b52f, 0x12b4a52b, 0x12149d25,
	0x12048d23, 0x11f4851e, 0x11d4791f, 0x1194751f,
	0x11846d1e, 0x1184791f, 0x11b47d21, 0x11f48122,
	0x12048525, 0x12349522, 0x1264a12b, 0x1254b132,
	0x12e4bd31, 0x1324d535, 0x1394e93c, 0x13e50542,
	0x14852d4f, 0x15255156, 0x15657563, 0x16359169,
	0x1745d975, 0x18260583, 0x18864992, 0x19f6759e,
	0x1b26b1b1, 0x1b8701c2, 0x1d2749d0, 0x1e17b5e8,
	0x1f77fe02, 0x21c87220, 0x2308de31, 0x25394e52,
	0x2769b27f, 0x29aa729b, 0x2c0b06c3, 0x2e6bc2f6,
	0x30fc330f, 0x337d033d, 0x365d576c, 0x2acac6a1,
	0x298a7293, 0x2709c268, 0x25d9765b, 0x23090638,
	0x21f8922f, 0x20880e09, 0x1f57c9f7, 0x1d476de6,
	0x1c2715c9, 0x1be6f1bb, 0x1a069dae, 0x19765d9a,
	0x18d63190, 0x17d5f57e, 0x1705c978, 0x1665a16d,
	0x15d56d5f, 0x15055957, 0x14e52d4f, 0x14251546,
	0x13d4f941, 0x1374e138, 0x1314c538, 0x12e4b534,
	0x12a4ad2d, 0x12349d26, 0x12248525, 0x12248923,
	0x11f47d21, 0x12048121, 0x11d48523, 0x11f48525,
	0x12548d25, 0x12349927, 0x1294a52a, 0x12b4a52e,
	0x12f4b932, 0x1324cd32, 0x1394dd3a, 0x13a50141,
	0x14751147, 0x14e54552, 0x1545615a, 0x15f5895f,
	0x16a5b16b, 0x1755e17e, 0x18561d88, 0x1936599a,
	0x19f699a7, 0x1aa6c5b9, 0x1c5709c5, 0x1db75dd0,
	0x1eb7a5f2, 0x20181e08, 0x21a87e26, 0x22b8f242,
	0x2589625f, 0x27e9f290, 0x294a86a1, 0x2d3b32d4,
	0x2f3bdaf4, 0x332c6f24, 0x340d1f34, 0x34adcb53,
	0x29eaaab2, 0x291a6eaa, 0x26fa0283, 0x25d97a5f,
	0x23990242, 0x2268aa2c, 0x20484216, 0x1f57cdf4,
	0x1db785da, 0x1cd72dcd, 0x1be701bd, 0x1af6bdb2,
	0x19c6759b, 0x18f63d91, 0x18760587, 0x1765e979,
	0x16a5b970, 0x15d57d68, 0x1575695c, 0x1505454f,
	0x14852948, 0x14350945, 0x13a4e940, 0x1384dd39,
	0x1304cd3a, 0x12d4ad32, 0x12a4ad2a, 0x1274a92a,
	0x1284a127, 0x12249926, 0x12349928, 0x12749d28,
	0x1284a129, 0x1264a52d, 0x1284a92b, 0x12b4bd2e,
	0x1304bd37, 0x1324e136, 0x1384f13f, 0x13f4f944,
	0x14450945, 0x14a5354d, 0x15155159, 0x15658161,
	0x1655a96d, 0x16e5c976, 0x1775f982, 0x18a62d8c,
	0x19c6759e, 0x1ab6c5ae, 0x1b86f5bb, 0x1c8719d4,
	0x1d977de8, 0x1f17e1ee, 0x2118420f, 0x2228962f,
	0x24a91e42, 0x2629be70, 0x27aa428c, 0x2b8a96b7,
	0x2ceb52d6, 0x2ebbe304, 0x323c7738, 0x332d6358,
	0x365d436c, 0x29ba96ba, 0x29ea72a4, 0x27d9fa82,
	0x26699e62, 0x24590a4d, 0x22f8a22e, 0x22085218,
	0x2007e204, 0x1e479de8, 0x1cd74dd2, 0x1c26f5c4,
	0x1b26c1b6, 0x1a4699a5, 0x1926559a, 0x1896358b,
	0x1755f985, 0x1715cd75, 0x1685a16b, 0x15c58962,
	0x15555958, 0x14e54154, 0x1445214b, 0x14250543,
	0x13d4fd3f, 0x1374e13a, 0x1314c934, 0x12d4c934,
	0x12e4c136, 0x12c4b532, 0x1294b12c, 0x12d4ad2f,
	0x12a4ad33, 0x12e4ad31, 0x12e4bd31, 0x12e4bd34,
	0x1334d135, 0x1374e137, 0x1394f13b, 0x13f50543,
	0x14351543, 0x14852d4b, 0x14e55554, 0x15a57563,
	0x15d59569, 0x16a5c170, 0x17a5ed7c, 0x18660d8f,
	0x19464599, 0x1a6695a5, 0x1b26cdb3, 0x1be721c6,
	0x1cc761dc, 0x1ea799ec, 0x2007fe01, 0x2118761c,
	0x2328de32, 0x24b9265b, 0x2779d285, 0x29ba56a5,
	0x2beb02c5, 0x2e7b7ed6, 0x2fbc1b00, 0x329ccb2d,
	0x351d574b, 0x365dd372, 0x2acaeab4, 0x2a0a7aa9,
	0x272a1e89, 0x2619a26c, 0x25792a3f, 0x2328da34,
	0x21786617, 0x20880a10, 0x1ea7c9e8, 0x1db769da,
	0x1c7721c9, 0x1be6ddc3, 0x1a66a9ae, 0x19b659a7,
	0x18963191, 0x18060184, 0x1795e17c, 0x16a5bd68,
	0x1655996b, 0x15556d5e, 0x15656158, 0x14f53d53,
	0x1475294b, 0x14251143, 0x13a4fd3e, 0x1374e93c,
	0x1344e93b, 0x1384d937, 0x1314cd31, 0x1304c933,
	0x1314c939, 0x1314c537, 0x1344cd39, 0x1334d93b,
	0x1384e93b, 0x1374ed3d, 0x13d4f140, 0x13f50143,
	0x14751d4d, 0x14652d4d, 0x14c54555, 0x15e5555a,
	0x16658566, 0x1685b16e, 0x16f5e176, 0x17f5fd82,
	0x18964193, 0x19e66d9f, 0x1a26a9ac, 0x1b86f5b5,
	0x1c2719d0, 0x1e0765e0, 0x1ef7c9fa, 0x20481a11,
	0x21989227, 0x2338f64e, 0x25b9826b, 0x285a1a83,
	0x2a4a86ac, 0x2beb16c5, 0x2e0bc2f8, 0x309c4719,
	0x330ca72f, 0x356d6b65, 0x372dbf77, 0x2beacac3,
	0x2b1a7aa4, 0x292a328a, 0x26d9b273, 0x25395a58,
	0x2288d636, 0x21688624, 0x21380613, 0x1f27c9ed,
	0x1de78de7, 0x1c972dcc, 0x1b66fdc3, 0x1ab6b1b3,
	0x1a167da3, 0x19564d99, 0x18b62d8a, 0x17d5ed83,
	0x1725d173, 0x16c5a171, 0x16457d68, 0x1585595d,
	0x15255156, 0x14a53550, 0x1425254f, 0x14050d45,
	0x13b50545, 0x13a4fd42, 0x1364ed3d, 0x1374e53c,
	0x1354e93a, 0x1364e53c, 0x1354e939, 0x13a4e53b,
	0x1374ed3e, 0x1374f540, 0x13c4fd43, 0x13d51143,
	0x1445214a, 0x14753d4e, 0x15254557, 0x1565715a,
	0x16257d63, 0x1645a96f, 0x16d5d573, 0x17a5e97e,
	0x1806158d, 0x1926599b, 0x19b689a1, 0x1b96b9b7,
	0x1c5705c2, 0x1ca74dd0, 0x1e1791e8, 0x1ed805fc,
	0x21685212, 0x2238ba2d, 0x23f9325a, 0x26d99668,
	0x28ca429e, 0x2b3acab1, 0x2e0b46d4, 0x2f1be2eb,
	0x313ccb19, 0x335d1f32, 0x356d8b67, 0x385dcb77,
	0x2a9ae6ca, 0x2a3a92b4, 0x294a4296, 0x2769d280,
	0x25396249, 0x23a90a42, 0x2258b22a, 0x21482e15,
	0x1f57e5ff, 0x1df791f1, 0x1cf741dd, 0x1c0705c7,
	0x1b26d5bd, 0x1a6699a9, 0x195675a0, 0x19064191,
	0x18061588, 0x1795e181, 0x1695c97e, 0x1655b96d,
	0x15c58166, 0x15956d5e, 0x15556158, 0x15054156,
	0x1495214f, 0x14252946, 0x14351945, 0x1424f946,
	0x13e4f944, 0x13b4f543, 0x13d4fd42, 0x13d50543,
	0x13b50143, 0x13e50549, 0x14451548, 0x14451d4c,
	0x14b5254b, 0x14d52d57, 0x15454958, 0x1565615f,
	0x16158563, 0x1665b16f, 0x16c5c973, 0x17c5f17a,
	0x1886158a, 0x18b64593, 0x1996899f, 0x1ac6adb1,
	0x1ba6e9ba, 0x1cf731d1, 0x1d8771dc, 0x1ef7c5ed,
	0x20580a07, 0x21a87221, 0x2388d649, 0x24a96a62,
	0x26e9c27b, 0x292a729c, 0x2b4af2ca, 0x2d5b52e3,
	0x2ebc1b06, 0x325caf34, 0x345cf753, 0x360dcb74,
	0x36de2b77, 0x2b0ae6cd, 0x2aeaa6b7, 0x28ca6e9f,
	0x275a167f, 0x2659926d, 0x24c92642, 0x2268c633,
	0x2198621a, 0x20180e09, 0x1e67d1f8, 0x1d475ddf,
	0x1c8741cf, 0x1ba6f9bf, 0x1ab6bdb3, 0x19c681a5,
	0x1926699f, 0x18762591, 0x17b60987, 0x1745dd7a,
	0x16a5cd70, 0x1645a56f, 0x16258966, 0x15757560,
	0x15655d5a, 0x15155156, 0x14f53552, 0x14953151,
	0x14a52d4f, 0x1445294a, 0x1475194a, 0x14651948,
	0x13f52547, 0x14651d4c, 0x1495294a, 0x14a53d50,
	0x14654554, 0x14d55156, 0x15855159, 0x15c57561,
	0x15e59167, 0x1685a970, 0x1715cd75, 0x1755e980,
	0x1806118b, 0x18c63194, 0x18f6759e, 0x1a26a9aa,
	0x1b26c5b7, 0x1c270dd0, 0x1ce765d8, 0x1ea78df5,
	0x1fe7e5f6, 0x21384a17, 0x22989a31, 0x24292a55,
	0x2629b269, 0x280a1696, 0x2a3a96b2, 0x2b8b3ecd,
	0x2e4bb6f0, 0x318c4f2b, 0x31ccb72f, 0x33cd4f5f,
	0x363db35f, 0x368e4390, 0x2c0b02ca, 0x2a9ab6be,
	0x292a72a5, 0x285a028d, 0x2669a26b, 0x24596655,
	0x2358e23a, 0x21d89e29, 0x1fe8260d, 0x1ea7e1f9,
	0x1e2795ed, 0x1ce73dd1, 0x1c9719bd, 0x1b26e1bb,
	0x1af69dae, 0x19e675a2, 0x19263d97, 0x18662d89,
	0x17e60182, 0x1795dd78, 0x16d5d171, 0x1685a170,
	0x15e59962, 0x15a58562, 0x1595715b, 0x1555655b,
	0x15255956, 0x15054557, 0x15254151, 0x14a54952,
	0x14b54151, 0x14b54550, 0x14f54956, 0x14e55554,
	0x1545555a, 0x15956958, 0x15657560, 0x15e58162,
	0x1625916c, 0x16c5a96d, 0x1725c978, 0x1765f57e,
	0x17e60585, 0x18c63993, 0x1956699d, 0x1a7689b1,
	0x1b26bdb5, 0x1bd6f9c3, 0x1c8745d8, 0x1db781e5,
	0x1eb7e1f9, 0x20282a08, 0x21587a27, 0x2418d64c,
	0x24b95255, 0x2729ce82, 0x295a669c, 0x2b3aeac3,
	0x2ccb36da, 0x305be2fe, 0x318c4f38, 0x330cf738,
	0x351d3b5d, 0x36ddbf7f, 0x378e5b90, 0x2beb1ad3,
	0x2acac6c5, 0x2a0a86a9, 0x282a3296, 0x2669d673,
	0x25495662, 0x2418ea4a, 0x2248a228, 0x20f84a17,
	0x1fa7ee00, 0x1e57b5f1, 0x1d1759d8, 0x1c4721d0,
	0x1b86edbf, 0x1b16c1b5, 0x1a169da8, 0x19165d9f,
	0x19264191, 0x18b62188, 0x17f5f584, 0x1795e57a,
	0x16b5b972, 0x1665a96e, 0x1635956e, 0x16258964,
	0x15b56960, 0x1555655d, 0x1565595c, 0x15454d58,
	0x1565615e, 0x14c5495a, 0x15455958, 0x1535595d,
	0x1585615d, 0x15e56d5e, 0x15858964, 0x1615896a,
	0x1635996b, 0x1655bd71, 0x1735c976, 0x1755ed7c,
	0x17d60982, 0x18b63d92, 0x1906599d, 0x19b685a7,
	0x1a96bdac, 0x1b56d1ba, 0x1cc72dcb, 0x1d075de3,
	0x1e67a1ed, 0x1f47ee0a, 0x20584219, 0x2268b22e,
	0x23c90249, 0x25798a65, 0x281a1a83, 0x28ea7e9b,
	0x2bdb0ec6, 0x2dcb76da, 0x2f9bf304, 0x320c832f,
	0x34acdb53, 0x345d575f, 0x375dd385, 0x37de4fa4,
	0x2b4b4ecf, 0x2b6b02c8, 0x29baaaaf, 0x289a5a93,
	0x2779fa83, 0x2699925f, 0x24b91e4e, 0x2328ca37,
	0x21887624, 0x20683204, 0x1e77d5f7, 0x1df789f2,
	0x1d074ddd, 0x1bc725ce, 0x1b56edbd, 0x1a46a5ae,
	0x19d68da9, 0x1996719f, 0x19062d91, 0x1816118a,
	0x17a60584, 0x1735e178, 0x1735dd75, 0x16a5b972,
	0x16a5b56d, 0x16359166, 0x16259166, 0x15c58160,
	0x15a58968, 0x15e57962, 0x15c57967, 0x15a58162,
	0x15e58d63, 0x16159166, 0x1645a16b, 0x16758972,
	0x16b5c171, 0x16e5d57a, 0x1765d979, 0x17e5f97f,
	0x18261187, 0x18b63d96, 0x196659a0, 0x19e67da8,
	0x1a56adaf, 0x1b26e1bb, 0x1bc71dc3, 0x1cf741d3,
	0x1dd79dea, 0x1f47e203, 0x20882e18, 0x21e89629,
	0x2398fe4c, 0x25195267, 0x2709e67b, 0x287a6698,
	0x2b0ac6b5, 0x2ccb46cf, 0x2e9be2fa, 0x307c4717,
	0x323cb740, 0x34fce350, 0x36dd9f62, 0x37dddf98,
	0x38be639e, 0x2d7b1ad6, 0x2b9ae6be, 0x29eabeac,
	0x28ba4aa1, 0x2829fa85, 0x2729be77, 0x25696a65,
	0x23f8f643, 0x2218a226, 0x20f85e16, 0x1f680a07,
	0x1ee7c9f3, 0x1d7795ea, 0x1c7749d3, 0x1c0721c7,
	0x1b26c5c3, 0x1a56a5ac, 0x19f691a8, 0x1986659c,
	0x18864d99, 0x18262190, 0x17f61584, 0x1735f580,
	0x1775d97b, 0x1725d578, 0x1705cd77, 0x16b5ad6c,
	0x16959971, 0x1665b56c, 0x1645b171, 0x1625a971,
	0x1675b16b, 0x16a5b171, 0x16c5b175, 0x1695c574,
	0x16b5c577, 0x1745dd7d, 0x1796057e, 0x17f60185,
	0x1886218d, 0x19265994, 0x195665a0, 0x19e689a7,
	0x1ad6a9ad, 0x1b26edbb, 0x1c1715c6, 0x1c7751d8,
	0x1e2781ea, 0x1f37d5f1, 0x2058120f, 0x21285a25,
	0x2218aa39, 0x24693652, 0x26699669, 0x27ea1e95,
	0x29ea86a9, 0x2b8b32c3, 0x2e0b9eeb, 0x2ebc130f,
	0x316c8320, 0x330cef47, 0x347d1353, 0x370dbf85,
	0x380e438d, 0x399e7bc3, 0x2d0b72da, 0x2c5b16d3,
	0x2a6acac8, 0x297a8aa9, 0x282a4a99, 0x2669fe74,
	0x25697e6b, 0x24492250, 0x2338c643, 0x2138862a,
	0x1ff8420d, 0x1fb7fe03, 0x1e67a9f5, 0x1d5761df,
	0x1cc73ddb, 0x1b66f1cb, 0x1af6d5b8, 0x1a66b1b3,
	0x19e675ae, 0x19465da1, 0x18b64993, 0x18762d89,
	0x18561992, 0x1825fd88, 0x17b5ed82, 0x1725d97e,
	0x16c5d17e, 0x1735cd7f, 0x1725d974, 0x16f5d57d,
	0x16d5cd7a, 0x1705d979, 0x1745c17c, 0x1775d97a,
	0x1745f180, 0x1785f983, 0x17f60983, 0x1836118b,
	0x18264994, 0x19066197, 0x19b671a2, 0x19f6a1ae,
	0x1aa6c9ae, 0x1bc6f5b9, 0x1bb70dc8, 0x1be73ddf,
	0x1d877de9, 0x1e67c5f0, 0x2057f605, 0x2108621a,
	0x22b8be38, 0x23e9064d, 0x24d97e5e, 0x2759c280,
	0x291a5a98, 0x2b8acabe, 0x2c5b4ee1, 0x2e4bdb04,
	0x301c2320, 0x313cd347, 0x339d1f5a, 0x34fd9f5f,
	0x368ddf87, 0x394e6fb0, 0x3bae9fb3, 0x2dab72df,
	0x2c5b2acd, 0x2b3aeaba, 0x297ab29e, 0x288a429e,
	0x2709fa86, 0x2549ce75, 0x24d93e60, 0x23690241,
	0x2268a62e, 0x21284217, 0x2038220c, 0x1f57bdfb,
	0x1de791e9, 0x1d2759dd, 0x1c5719ce, 0x1be6e9bd,
	0x1b46d5b6, 0x19e6b9a9, 0x19b68da3, 0x197669a4,
	0x18e64d9b, 0x18964593, 0x18862d90, 0x17d60d8a,
	0x17e60587, 0x1795fd85, 0x1745f184, 0x1795f184,
	0x1795f17a, 0x1755f17b, 0x17b5f17c, 0x1745f986,
	0x1795f57c, 0x17e60d8a, 0x1826218d, 0x18862991,
	0x18c64199, 0x1926559f, 0x19b685a1, 0x19f689aa,
	0x1a06c5b3, 0x1b26ddbb, 0x1b96fdcb, 0x1c3721cf,
	0x1d3771e5, 0x1e97c1ed, 0x1f37de06, 0x20883220,
	0x21889625, 0x2368e23a, 0x24995262, 0x26d9be6d,
	0x280a0a93, 0x29dab2aa, 0x2b4afacc, 0x2d3b62ed,
	0x2f5c0315, 0x30fc4724, 0x32bccb44, 0x335d3b4b,
	0x360d637c, 0x378e0b82, 0x380e87aa, 0x3baf5bc9,
	0x2e7b76da, 0x2ccb46cf, 0x2a9b02be, 0x2a3aa6b9,
	0x298a66a1, 0x285a1e8d, 0x2699be80, 0x26196e68,
	0x24c91654, 0x22f8da3c, 0x2218a626, 0x21084a1d,
	0x1fc80a0d, 0x1e97c9fb, 0x1d476de8, 0x1d573de0,
	0x1c072dcc, 0x1b3705c6, 0x1af6ddb6, 0x1a669db1,
	0x1a2689ae, 0x195675a4, 0x19666999, 0x1936559d,
	0x18c62d92, 0x18a63996, 0x18861d90, 0x1846298d,
	0x1856258a, 0x18062d8c, 0x1856258d, 0x1826158d,
	0x1856298c, 0x18363191, 0x18964590, 0x18f64d93,
	0x1946659b, 0x195689a7, 0x19d68da7, 0x1a969db3,
	0x1b56c9b8, 0x1b86e9be, 0x1c070dcf, 0x1cf745d9,
	0x1dc75de3, 0x1e37b5f3, 0x1ef7f9fc, 0x20584e14,
	0x21287626, 0x2318d243, 0x23f94a53, 0x25b9a675,
	0x2879fa90, 0x29ba4e96, 0x2b3af2c8, 0x2c9b46d8,
	0x2edbeaf2, 0x2ffc1306, 0x30fcaf2f, 0x33ccdb47,
	0x339d9f5d, 0x354db372, 0x385e43a1, 0x399e93d6,
	0x3caf83ee, 0x2ebbaeed, 0x2dab6ad1, 0x2b4b1abe,
	0x2b6ab6b4, 0x28ea92b1, 0x288a4e9b, 0x277a1a85,
	0x26d9c67b, 0x25996667, 0x23c9024e, 0x22a8d636,
	0x21a88222, 0x20782213, 0x1fc7fe05, 0x1e87b5f4,
	0x1db781ea, 0x1d474ddb, 0x1c272dd6, 0x1bc705c1,
	0x1b06ddbd, 0x1a56b9bb, 0x1ae6a1a9, 0x19f691a7,
	0x19c67da7, 0x18f665a5, 0x18a66da0, 0x18b6519a,
	0x19165192, 0x18a65d9a, 0x18d64599, 0x18b64599,
	0x18a63d9a, 0x18f65d9a, 0x1906559e, 0x197675a9,
	0x195685a7, 0x1a3689a8, 0x1ac695ae, 0x1a96c5b7,
	0x1b46d1c5, 0x1ba705c8, 0x1cb725c2, 0x1c8749de,
	0x1d8775e5, 0x1e97aded, 0x1f07f608, 0x20682612,
	0x21788625, 0x22d8ba3f, 0x24391e54, 0x26699669,
	0x2729fa89, 0x294a2a9c, 0x2b1ab2b4, 0x2ccb32cf,
	0x2dcbcaf6, 0x2f3c0b00, 0x318c5720, 0x329cf753,
	0x340d3b5a, 0x356d5765, 0x363df38d, 0x39ce37aa,
	0x3b7f0fc3, 0x3d1f43dd, 0x2f5be306, 0x2d9b9ee7,
	0x2c4b36da, 0x2b1aeac8, 0x2a0ab2b5, 0x288a4ea5,
	0x284a2a92, 0x26d9d679, 0x26999e62, 0x24c94a59,
	0x23690243, 0x2258b62c, 0x21386e22, 0x20481e12,
	0x2007ea07, 0x1e67c1f8, 0x1d8775e8, 0x1cc74dd7,
	0x1c0735d0, 0x1bc70dcc, 0x1b56f1c2, 0x1b16e5c1,
	0x1a86cdb5, 0x1a56a5ad, 0x1a2691b3, 0x199699ad,
	0x19c695a9, 0x197691a5, 0x19b679a3, 0x195679ab,
	0x19b66da1, 0x19c675a9, 0x19f681a5, 0x19c685ac,
	0x1a26adac, 0x1a36b9b9, 0x1af6c9b5, 0x1b06ddc6,
	0x1b66f1bf, 0x1be71dcd, 0x1c4731d4, 0x1d475de0,
	0x1df78df2, 0x1e17adf7, 0x1f97f609, 0x2048421e,
	0x21689622, 0x2298e639, 0x24990a52, 0x26298a71,
	0x2679be7f, 0x289a56a4, 0x2a3ab2b9, 0x2b8b2acd,
	0x2d9b6ae5, 0x2f1beb0a, 0x30dc4f15, 0x311c8347,
	0x34ad133b, 0x33ed775a, 0x36ddbf87, 0x375e4fa4,
	0x394eabbd, 0x3b4f5be7, 0x3c1fffff, 0x303c1b15,
	0x2dabbb04, 0x2d1b46e5, 0x2b1afac5, 0x2abab2b4,
	0x29ea72ad, 0x288a3ea2, 0x27ea0285, 0x26b99e71,
	0x25497a6d, 0x23e92a5e, 0x22a8d22f, 0x21f8862d,
	0x2128561f, 0x20682613, 0x1f47e200, 0x1e47adf6,
	0x1d5785e5, 0x1d1761e0, 0x1c3731d3, 0x1bd71dcd,
	0x1b7701cd, 0x1b26d9be, 0x1ad6cdb8, 0x1aa6c5b9,
	0x1a26b1b6, 0x1a56b1b1, 0x1a56a1b0, 0x1a46a5ad,
	0x1a3699bb, 0x1a16a9b0, 0x1a36a9b0, 0x1a36a5bb,
	0x1ae6c5b7, 0x1b26d5b9, 0x1b66e9bd, 0x1bb6f1cb,
	0x1b870dcb, 0x1c1721d6, 0x1c7735d5, 0x1d7751e7,
	0x1dd7b1e6, 0x1df7b5fe, 0x1f581201, 0x20284e1f,
	0x21588628, 0x2288b235, 0x23b91e4c, 0x24797e65,
	0x26b9c274, 0x27aa1693, 0x297a7eb2, 0x2bdb02c5,
	0x2c0b32eb, 0x2e2bd302, 0x2f5c0b17, 0x316c8326,
	0x332ccb49, 0x339d2f72, 0x34fd7772, 0x365dff82,
	0x388e3798, 0x3aef0fb0, 0x3b4f5bee, 0x3efffff8,
	0x30bc932d, 0x2fbc230a, 0x2d5b72eb, 0x2ccb32e5,
	0x298acac3, 0x29aab2b5, 0x291a669f, 0x27ea3e99,
	0x2659e685, 0x2659766c, 0x25199264, 0x2449265e,
	0x2328d245, 0x21689232, 0x20a85221, 0x20583a11,
	0x1fe7e202, 0x1e77d603, 0x1df791e9, 0x1cd791e3,
	0x1cd755e1, 0x1c5739cf, 0x1c2705ce, 0x1bc705c9,
	0x1b86ddc6, 0x1b76cdc3, 0x1b76f5c1, 0x1b06d5c6,
	0x1b46f1bf, 0x1a76e5c1, 0x1b46bdc1, 0x1b36e5bf,
	0x1b26e5bf, 0x1b66e9c3, 0x1b66e9c9, 0x1bc70dcb,
	0x1cb72dd5, 0x1cf759e3, 0x1cf785ec, 0x1db799f5,
	0x1e17ca00, 0x1ea7e200, 0x20380e0c, 0x20d85224,
	0x21f88626, 0x22b8aa3e, 0x2399164e, 0x25898a6c,
	0x25d9ae6f, 0x26fa2a9c, 0x291a86b2, 0x2b3aeabe,
	0x2c0b72d6, 0x2efbcb04, 0x2fbc6720, 0x316c673d,
	0x32bc935d, 0x325d3b5d, 0x35ed8b5f, 0x368dd382,
	0x36ddff93, 0x3abe5bb6, 0x3baef7e4, 0x3e5fbbff,
	0x3f9fffff,
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
void iq_param_ov7251_id1_evb_table1_init(UINT32 id,UINT32 mode_idx)
{
	//iqs_set_range
	UINT32 *iq_param_addr[IQ_PARAM_MAX];
	static UINT32 num_3DNR_param = TOTAL_UI_3DNR_LV;
	gISO_Index[id].Id = id;
    gISO_Index[id].num = IQS_ISOMAPTAB_NUM - 1;

	iq_param_addr[IQ_ISOIDX] = (UINT32 *)&gISO_Index[id];

    iq_param_addr[IQ_ISOIDX_MAP ] = (UINT32*)&iso_map_tab_ov7251_id1_evb[0];

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
    iq_param_addr[IQ_DCE_DIST	] = (UINT32*)&dist_ov7251_id1_evb;

    iq_param_addr[IQ_DCE_RADIOUS] = (UINT32*)&radious_ov7251_id1_evb;

    iq_param_addr[IQ_CFAINTER	] = (UINT32*)&cfa_ov7251_id1_evb;

	iq_param_addr[IQ_GDC_FOV	] = (UINT32*)&fov_ov7251_id1_evb;
    iq_param_addr[IQ_GDC_GLUT	] = (UINT32*)ldc_g_lut_ov7251_id1_evb;
    iq_param_addr[IQ_GDC_RLUT	] = (UINT32*)ldc_r_lut_ov7251_id1_evb;
    iq_param_addr[IQ_GDC_BLUT	] = (UINT32*)ldc_b_lut_ov7251_id1_evb;
	iq_param_addr[IQ_DCE_ENH    ] = (UINT32 *)&ldc_enh;

	iq_param_addr[IQ_DCE_CA	    ] = (UINT32*)&cac_ov7251_id1_evb;

	iq_param_addr[IQ_DCE_2D_PARAM   ] = (UINT32*)&dc_2d_param_ov7251_id1_evb[0];
	iq_param_addr[IQ_DCE_2DLUT00    ] = (UINT32*)dc_2dlut00_ov7251_id1_evb;
	iq_param_addr[IQ_DCE_2DLUT01    ] = (UINT32*)dc_2dlut01_ov7251_id1_evb;
	iq_param_addr[IQ_DCE_2DLUT02    ] = (UINT32*)dc_2dlut02_ov7251_id1_evb;
	iq_param_addr[IQ_DCE_2DLUT03    ] = (UINT32*)dc_2dlut03_ov7251_id1_evb;

	iq_param_addr[IQ_DCE_STRP_INFO  ] = (UINT32*)&strp_info_ov7251_id1_evb[0];

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

    iq_param_addr[IQ_CCM_TH     ] = (UINT32*)&color_temperature_th_ov7251_id1_evb;
	iq_param_addr[IQ_Color_L    ] = (UINT32*)&color_low_ov7251_id1_evb;
	iq_param_addr[IQ_Color_M    ] = (UINT32*)&color_middle_ov7251_id1_evb;
	iq_param_addr[IQ_Color_H    ] = (UINT32*)&color_high_ov7251_id1_evb;
    iq_param_addr[IQ_FSTAB      ] = (UINT32*)fs_tab_ov7251_id1_evb[0];
	iq_param_addr[IQ_FDTAB	    ] = (UINT32*)fd_tab_ov7251_id1_evb[0];
    iq_param_addr[IQ_CC         ] = (UINT32*)&cc_ov7251_id1_evb;

    iq_param_addr[IQ_Y_CURVE    ] = (UINT32*)y_curve_ov7251_id1_evb;

	iq_param_addr[IQ_GAMMA_TH	] = (UINT32*)&gamma_lv_th_ov7251_id1_evb;
    iq_param_addr[IQ_GAMMA_DAY	] = (UINT32*)gamma_day_ov7251_id1_evb;
	iq_param_addr[IQ_GAMMA_NIGHT] = (UINT32*)gamma_night_ov7251_id1_evb;

	iq_param_addr[IQ_3DCC_TAB   ] = (UINT32*)color_3d_lut_ov7251_id1_evb;
    iq_param_addr[IQ_3DCC_ROUND ] = (UINT32*)&color_3d_ov7251_id1_evb;

	iq_param_addr[IQ_EDGTAB     ] = (UINT32*)edg_tab_ov7251_id1_evb;
    iq_param_addr[IQ_DDSTAB     ] = (UINT32*)dds_tab_ov7251_id1_evb;

	iq_param_addr[IQ_SATURATION ] = (UINT32 *)&saturation[0];
	iq_param_addr[IQ_CONTRAST   ] = (UINT32 *)&contrast[0];
    iq_param_addr[IQ_CCTRL_INFO ] = (UINT32 *)&cctrl_info[0];

	iq_param_addr[IQ_CST_COEF	] = (UINT32*)cst_coef_ov7251_id1_evb;
    iq_param_addr[IQ_CST_PARAM	] = (UINT32*)&cst_ov7251_id1_evb;

	iq_param_addr[IQ_YCCFIX	    ] = (UINT32*)&ycc_fix_ov7251_id1_evb;

    iq_param_addr[IQ_NOISE_PARAM] = (UINT32*)&noise_param_ov7251_id1_evb;

    iq_param_addr[IQ_GAMMA_RAND ] = (UINT32*)&gamma_rand_ov7251_id1_evb;

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
	iq_param_addr[IQ_HDR            ] = (UINT32 *)&hdr_ov7251_id1_evb;

	//***********
	//** Defog **
	//***********
	iq_param_addr[IQ_DEFOG_INIT_PARAM     ] = (UINT32 *)&defog_init_param_ov7251_id1_evb;
	iq_param_addr[IQ_DEFOG_WET     ] = (UINT32 *)&defog_wet_ov7251_id1_evb[0];

	iq_set_param_addr(id,mode_idx, iq_param_addr);
}

#endif
