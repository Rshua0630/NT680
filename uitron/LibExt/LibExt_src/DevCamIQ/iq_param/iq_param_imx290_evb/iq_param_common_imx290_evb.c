#ifndef _IQ_PARAM_COMMON_IMX290_EVB_C
#define _IQ_PARAM_COMMON_IMX290_EVB_C

#include "iq_param_common_imx290_evb.h"

ISOMAP iso_map_tab_imx290_evb[IQS_ISOMAPTAB_NUM] =
{
    {  100, IQS_GAIN_1X},
    {  200, IQS_GAIN_2X},
    {  400, IQS_GAIN_4X},
    {  800, IQS_GAIN_8X},
    { 1600, IQS_GAIN_16X},
    { 3200, IQS_GAIN_32X},
    { 6400, IQS_GAIN_64X},
    {12800, IQS_GAIN_128X},
	{25600, IQS_GAIN_256X},
	{51200, IQS_GAIN_512X},
	{102400, IQS_GAIN_1024X},
	{204800, IQS_GAIN_2048X},
};

////////////////////////////////
//             DCE             /
////////////////////////////////

//////////////////////
///**  DCE-DIST
//////////////////////
DIST dist_imx290_evb =
{
   //Fact_X,Fact_Y
    0xfff, 0xfff
};

//////////////////////
///**  DCE-Radious
//////////////////////
Fact_Norm  radious_imx290_evb =
{
   //Manual_NormTermEn,Factor,Bit
    DISABLE, 0, 0
};

//////////////////////
///**  DCE-CFA
//////////////////////
DCE_CFAPARAM cfa_imx290_evb =
{
//NsmarEdge, NsmarDiff, DIFNormBit, {GCEn, GCNBit, Thr},{RBCEn,RBCNBit, Thr, Thr2}
          8,         8,          0, {ENABLE,    0,   0},{ENABLE,     0,   0,    0},{0,1,1,3,16,128,64}
};

//////////////////////
//**  DCE-GDC
//////////////////////
FOV fov_imx290_evb =
{
   //Gain,  FOV_SEL,             BoundryProcess
    0x400, FOV_BOUND_DUPLICATE, {1023,1023,1023}
};

UINT32 ldc_g_lut_imx290_evb[65] =
{
	65535 ,65442 ,65351 ,65262 ,65175 ,65088 ,65004 ,64921 ,64840 ,64760 ,
	64681 ,64604 ,64529 ,64455 ,64382 ,64310 ,64240 ,64171 ,64104 ,64038 ,
	63972 ,63909 ,63846 ,63785 ,63724 ,63665 ,63607 ,63550 ,63494 ,63440 ,
	63386 ,63333 ,63281 ,63230 ,63180 ,63131 ,63083 ,63035 ,62989 ,62943 ,
	62898 ,62854 ,62811 ,62768 ,62726 ,62685 ,62644 ,62604 ,62565 ,62526 ,
	62488 ,62450 ,62413 ,62376 ,62340 ,62304 ,62269 ,62234 ,62200 ,62165 ,
	62132 ,62098 ,62065 ,62032 ,62000
};

INT32 ldc_r_lut_imx290_evb[65] =
{
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0
};

INT32 ldc_b_lut_imx290_evb[65] =
{
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0
};

//////////////////////
//**  DCE-CAC
//////////////////////
Aberation cac_imx290_evb =
{
   //Cacsel,        Fact_R, Fact_G, Fact_B
    CAC_RGB_GAIN, {4096, 4096, 4096}
};

//////////////////////
//**  DCE-2DLUT
//////////////////////
_2DLUT_PARAM dc_2d_param_imx290_evb[4] =
{
	{
		0,
		{0, 0, 0, 0},
		_2DLUT_65_65_GRID
	},
	{
		0,
		{0, 0, 0, 0},
		_2DLUT_65_65_GRID
	},
	{
		0,
		{0, 0, 0, 0},
		_2DLUT_65_65_GRID
	},
	{
		0,
		{0, 0, 0, 0},
		_2DLUT_65_65_GRID
	}
};

UINT32 dc_2dlut00_imx290_evb[65*65] =
{
	0
};

UINT32 dc_2dlut01_imx290_evb[65*65] =
{
	0
};

UINT32 dc_2dlut02_imx290_evb[65*65] =
{
	0
};

UINT32 dc_2dlut03_imx290_evb[65*65] =
{
	0
};

//////////////////////
//**  DCE-IPL STRIPE INFO
//////////////////////
IPL_STRP_INFO strp_info_imx290_evb[4] =
{
	{
		IPL_STRPMODE_AUTO_124ST,
		6,
		{256, 256, 256, 256, 384, 384, 384, 384,
		384, 256, 256, 256, 256, 256, 256, 256},
		6,6
	},
	{
		IPL_STRPMODE_AUTO_124ST,
		6,
		{256, 256, 256, 256, 384, 384, 384, 384,
		384, 256, 256, 256, 256, 256, 256, 256},
		6,6
	},
	{
		IPL_STRPMODE_AUTO_124ST,
		6,
		{256, 256, 256, 256, 384, 384, 384, 384,
		384, 256, 256, 256, 256, 256, 256, 256},
		6,6
	},
	{
		IPL_STRPMODE_AUTO_124ST,
		6,
		{256, 256, 256, 256, 384, 384, 384, 384,
		384, 256, 256, 256, 256, 256, 256, 256},
		6,6
	}
};

////////////////////////////////
//             IPE             /
////////////////////////////////

//////////////////////
//**  IPE-CC
//////////////////////
IQS_COLOR_TEMPERATURE_TH color_temperature_th_imx290_evb =
{
    //Low,  Middle, High
    2900,   3400,   7000
};

IQS_COLOR_TEMPERATURE_PARAM color_low_imx290_evb =
{
    {//CCM A
    0x1A0,	0xF93,	0xFCD,
    0xF9B,	0x182,	0xFE3,
    0x064,	0xE81,	0x21C,
    },
    //hue table, Legal range : 0~255, 128 : NO sat adjust
    {128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128},
    //sat table, Legal range : -128~127, 0 : NO sat adjust
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    //int table, Legal range : -128~127, 0 : NO sat adjust
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
};

IQS_COLOR_TEMPERATURE_PARAM color_middle_imx290_evb =
{
    {//CCM TL84
    0x1AB,	0xF7A,	0xFDB,
    0xF9E,	0x179,	0xFEA,
    0x03B,	0xEF4,	0x1D1,
    },
    //hue table, Legal range : 0~255, 128 : NO sat adjust
    {128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128},
    //sat table, Legal range : -128~127, 0 : NO sat adjust
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    //int table, Legal range : -128~127, 0 : NO sat adjust
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
};

IQS_COLOR_TEMPERATURE_PARAM color_high_imx290_evb =
{
    {//CCM D65
    0x1BF,	0xF47,	0xFFA,
    0xF9C,	0x196,	0xFCE,
    0x00D,	0xEE3,	0x20F,
    },
    //hue table, Legal range : 0~255, 128 : NO sat adjust
    {128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128},
    //sat table, Legal range : -128~127, 0 : NO sat adjust
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    //int table, Legal range : -128~127, 0 : NO sat adjust
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
};

UINT8 fs_tab_imx290_evb[TOTAL_GAIN_NUM][16] =
{
    {128, 64, 16, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {128, 64, 16, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {128, 64, 16, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},  //ISO400
    {160, 80, 20, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {192, 96, 24, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {158, 79, 20, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {192, 96, 24, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {192, 96, 24, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {192, 96, 24, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {192, 96, 24, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {192, 96, 24, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},  //ISO102400
    {192, 96, 24, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
};

UINT8 fd_tab_imx290_evb[TOTAL_GAIN_NUM][16] =
{
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {20, 15, 10, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5, 10, 15},
    {40, 30, 20, 10, 5, 0, 0, 0, 0, 0, 0, 0, 0, 10, 20, 30},
    {60, 40, 30, 15, 7, 0, 0, 0, 0, 0, 0, 0, 0, 15, 30, 60},
    {64, 48, 32, 16, 8, 0, 0, 0, 0, 0, 0, 0, 0, 30, 60, 120},
    {59, 39, 29, 15, 7, 0, 0, 0, 0, 0, 0, 0, 0, 30, 60, 120},
    {64, 48, 32, 16, 8, 0, 0, 0, 0, 0, 0, 0, 0, 30, 60, 120},
    {64, 48, 32, 16, 8, 0, 0, 0, 0, 0, 0, 0, 0, 30, 60, 120},//ISO12800
    {64, 48, 32, 16, 8, 0, 0, 0, 0, 0, 0, 0, 0, 30, 60, 120},
    {64, 48, 32, 16, 8, 0, 0, 0, 0, 0, 0, 0, 0, 30, 60, 120},
    {64, 48, 32, 16, 8, 0, 0, 0, 0, 0, 0, 0, 0, 30, 60, 120},//ISO102400
    {64, 48, 32, 16, 8, 0, 0, 0, 0, 0, 0, 0, 0, 30, 60, 120},
};

IPE_CC cc_imx290_evb =
{
    CC_3_8, toIdentity, CC_PRE_GAMMA, CC_SEL_MAX, (UINT32)color_high_imx290_evb.IQS_CC, (UINT32)fs_tab_imx290_evb, (UINT32)fd_tab_imx290_evb
};

//////////////////////
//**  IPE-Ycurve
//////////////////////
UINT32 y_curve_imx290_evb[129] = //linear
{
    0, 8, 16, 24, 32, 40, 48, 56, 64, 72,
    80, 88, 96, 104, 112, 120, 128, 136, 144, 152,
    160, 168, 176, 184, 192, 200, 208, 216, 224, 232,
    240, 248, 256, 264, 272, 280, 288, 296, 304, 312,
    320, 328, 336, 344, 352, 360, 368, 376, 384, 392,
    400, 408, 416, 424, 432, 440, 448, 456, 464, 472,
    480, 488, 496, 504, 512, 520, 528, 536, 544, 552,
    560, 568, 576, 584, 592, 600, 608, 616, 624, 632,
    640, 648, 656, 664, 672, 680, 688, 696, 704, 712,
    720, 728, 736, 744, 752, 760, 768, 776, 784, 792,
    800, 808, 816, 824, 832, 840, 848, 856, 864, 872,
    880, 888, 896, 904, 912, 920, 928, 936, 944, 952,
    960, 968, 976, 984, 992, 1000, 1008, 1016, 1023
};

//////////////////////
//**  IPE-Gamma
//////////////////////
IQS_GAMMA_LV_TH gamma_lv_th_imx290_evb =
{
    //DAY,  NIGHT,
    600,    400
};

UINT32 gamma_day_imx290_evb[129] =
{
    0,37,73,107,139,169,198,225,241,257,
    273,289,304,318,333,347,360,373,386,399,
    411,422,433,444,455,465,475,484,493,502,
    510,518,525,532,539,546,553,560,566,573,
    580,587,594,601,607,614,621,627,634,640,
    647,653,659,666,672,678,684,691,697,703,
    709,715,721,727,733,739,744,750,756,762,
    767,772,777,782,787,792,797,801,806,811,
    816,821,826,830,835,840,845,849,854,859,
    863,868,872,877,882,886,891,895,900,904,
    908,913,917,921,926,930,934,939,943,947,
    951,955,959,964,968,972,976,980,984,988,
    992,996,1000,1003,1007,1011,1015,1019,1023
};

UINT32 gamma_night_imx290_evb[129] =
{
    0,37,73,107,139,169,198,225,241,257,
    273,289,304,318,333,347,360,373,386,399,
    411,422,433,444,455,465,475,484,493,502,
    510,518,525,532,539,546,553,560,566,573,
    580,587,594,601,607,614,621,627,634,640,
    647,653,659,666,672,678,684,691,697,703,
    709,715,721,727,733,739,744,750,756,762,
    767,772,777,782,787,792,797,801,806,811,
    816,821,826,830,835,840,845,849,854,859,
    863,868,872,877,882,886,891,895,900,904,
    908,913,917,921,926,930,934,939,943,947,
    951,955,959,964,968,972,976,980,984,988,
    992,996,1000,1003,1007,1011,1015,1019,1023
};

//////////////////////
//**  IPE-CCtrl
//////////////////////
UINT8 edg_tab_imx290_evb[24] =
{
    128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128
};

UINT8 dds_tab_imx290_evb[8] =
{
    8, 16, 32, 32, 32, 32, 32, 32
};


//////////////////////
//**  IPE-CST
//////////////////////
INT16 cst_coef_imx290_evb[9] =
{
    77, 150, 29, -43, -85, 128, 128, -107, -21
};

IPE_CST cst_imx290_evb =
{
    (UINT32)cst_coef_imx290_evb, CST_SEL_MINUS128, 0
};

//////////////////////
//**  IPE-YCCFIX
//////////////////////
IPE_YCCFIX ycc_fix_imx290_evb =
{
  //yth1{ YTH,ETH,          HIT1SEL,        NHIT1SEL,HIT,NHIT}, yth2{YTH,           HIT2SEL,        NHIT2SEL, HIT,NHIT}, CC{ETH, YTG,   CBTH,  CRTH,            HITSEL,         NHITSEL,CB_HIT,CB_NHIT,CR_HIT,CR_NHIT}
        {   0,  0, YCTH_SEL_ORG_VAL,YCTH_SEL_ORG_VAL,  0,   0},     {  0,  YCTH_SEL_ORG_VAL,YCTH_SEL_ORG_VAL,   0,   0},   {  0,   0, 0,   0, 0,  0,0,YCTH_SEL_ORG_VAL,YCTH_SEL_ORG_VAL,     0,      0,     0,      0}
};

//////////////////////
//**  IPE-NoiseParam
//////////////////////
IPE_RAND_NR noise_param_imx290_evb =
{
    FALSE, FALSE, 0, 0, FALSE
};

//////////////////////
//**  IPE-GammaRand
//////////////////////
 IPE_GamRand gamma_rand_imx290_evb =
{
    FALSE, 0, 0
};

//////////////////////
//**  IPE-3D Color Control
//////////////////////
UINT32 color_3d_lut_imx290_evb[900] =
{
    0x00000000, 0x00000040, 0x00000080, 0x000000c0, 0x000000ff, 0x00004000, 0x00004040, 0x00004080,
    0x000040c0, 0x000040ff, 0x00008000, 0x00008040, 0x00008080, 0x000080c0, 0x000080ff, 0x0000c000,
    0x0000c040, 0x0000c080, 0x0000c0c0, 0x0000c0ff, 0x0000ff00, 0x0000ff40, 0x0000ff80, 0x0000ffc0,
    0x0000ffff, 0x00400000, 0x00400040, 0x00400080, 0x004000c0, 0x004000ff, 0x00404000, 0x00404040,
    0x00404080, 0x004040c0, 0x004040ff, 0x00408000, 0x00408040, 0x00408080, 0x004080c0, 0x004080ff,
    0x0040c000, 0x0040c040, 0x0040c080, 0x0040c0c0, 0x0040c0ff, 0x0040ff00, 0x0040ff40, 0x0040ff80,
    0x0040ffc0, 0x0040ffff, 0x00800000, 0x00800040, 0x00800080, 0x008000c0, 0x008000ff, 0x00804000,
    0x00804040, 0x00804080, 0x008040c0, 0x008040ff, 0x00808000, 0x00808040, 0x00808080, 0x008080c0,
    0x008080ff, 0x0080c000, 0x0080c040, 0x0080c080, 0x0080c0c0, 0x0080c0ff, 0x0080ff00, 0x0080ff40,
    0x0080ff80, 0x0080ffc0, 0x0080ffff, 0x00c00000, 0x00c00040, 0x00c00080, 0x00c000c0, 0x00c000ff,
    0x00c04000, 0x00c04040, 0x00c04080, 0x00c040c0, 0x00c040ff, 0x00c08000, 0x00c08040, 0x00c08080,
    0x00c080c0, 0x00c080ff, 0x00c0c000, 0x00c0c040, 0x00c0c080, 0x00c0c0c0, 0x00c0c0ff, 0x00c0ff00,
    0x00c0ff40, 0x00c0ff80, 0x00c0ffc0, 0x00c0ffff, 0x00ff0000, 0x00ff0040, 0x00ff0080, 0x00ff00c0,
    0x00ff00ff, 0x00ff4000, 0x00ff4040, 0x00ff4080, 0x00ff40c0, 0x00ff40ff, 0x00ff8000, 0x00ff8040,
    0x00ff8080, 0x00ff80c0, 0x00ff80ff, 0x00ffc000, 0x00ffc040, 0x00ffc080, 0x00ffc0c0, 0x00ffc0ff,
    0x00ffff00, 0x00ffff40, 0x00ffff80, 0x00ffffc0, 0x00ffffff, 0x00000020, 0x00000060, 0x000000a0,
    0x000000e0, 0x00000000, 0x00004020, 0x00004060, 0x000040a0, 0x000040e0, 0x00000000, 0x00008020,
    0x00008060, 0x000080a0, 0x000080e0, 0x00000000, 0x0000c020, 0x0000c060, 0x0000c0a0, 0x0000c0e0,
    0x00000000, 0x0000ff20, 0x0000ff60, 0x0000ffa0, 0x0000ffe0, 0x00000000, 0x00400020, 0x00400060,
    0x004000a0, 0x004000e0, 0x00000000, 0x00404020, 0x00404060, 0x004040a0, 0x004040e0, 0x00000000,
    0x00408020, 0x00408060, 0x004080a0, 0x004080e0, 0x00000000, 0x0040c020, 0x0040c060, 0x0040c0a0,
    0x0040c0e0, 0x00000000, 0x0040ff20, 0x0040ff60, 0x0040ffa0, 0x0040ffe0, 0x00000000, 0x00800020,
    0x00800060, 0x008000a0, 0x008000e0, 0x00000000, 0x00804020, 0x00804060, 0x008040a0, 0x008040e0,
    0x00000000, 0x00808020, 0x00808060, 0x008080a0, 0x008080e0, 0x00000000, 0x0080c020, 0x0080c060,
    0x0080c0a0, 0x0080c0e0, 0x00000000, 0x0080ff20, 0x0080ff60, 0x0080ffa0, 0x0080ffe0, 0x00000000,
    0x00c00020, 0x00c00060, 0x00c000a0, 0x00c000e0, 0x00000000, 0x00c04020, 0x00c04060, 0x00c040a0,
    0x00c040e0, 0x00000000, 0x00c08020, 0x00c08060, 0x00c080a0, 0x00c080e0, 0x00000000, 0x00c0c020,
    0x00c0c060, 0x00c0c0a0, 0x00c0c0e0, 0x00000000, 0x00c0ff20, 0x00c0ff60, 0x00c0ffa0, 0x00c0ffe0,
    0x00000000, 0x00ff0020, 0x00ff0060, 0x00ff00a0, 0x00ff00e0, 0x00000000, 0x00ff4020, 0x00ff4060,
    0x00ff40a0, 0x00ff40e0, 0x00000000, 0x00ff8020, 0x00ff8060, 0x00ff80a0, 0x00ff80e0, 0x00000000,
    0x00ffc020, 0x00ffc060, 0x00ffc0a0, 0x00ffc0e0, 0x00000000, 0x00ffff20, 0x00ffff60, 0x00ffffa0,
    0x00ffffe0, 0x00000000, 0x00002000, 0x00002040, 0x00002080, 0x000020c0, 0x000020ff, 0x00006000,
    0x00006040, 0x00006080, 0x000060c0, 0x000060ff, 0x0000a000, 0x0000a040, 0x0000a080, 0x0000a0c0,
    0x0000a0ff, 0x0000e000, 0x0000e040, 0x0000e080, 0x0000e0c0, 0x0000e0ff, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00402000, 0x00402040, 0x00402080, 0x004020c0, 0x004020ff,
    0x00406000, 0x00406040, 0x00406080, 0x004060c0, 0x004060ff, 0x0040a000, 0x0040a040, 0x0040a080,
    0x0040a0c0, 0x0040a0ff, 0x0040e000, 0x0040e040, 0x0040e080, 0x0040e0c0, 0x0040e0ff, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00802000, 0x00802040, 0x00802080, 0x008020c0,
    0x008020ff, 0x00806000, 0x00806040, 0x00806080, 0x008060c0, 0x008060ff, 0x0080a000, 0x0080a040,
    0x0080a080, 0x0080a0c0, 0x0080a0ff, 0x0080e000, 0x0080e040, 0x0080e080, 0x0080e0c0, 0x0080e0ff,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00c02000, 0x00c02040, 0x00c02080,
    0x00c020c0, 0x00c020ff, 0x00c06000, 0x00c06040, 0x00c06080, 0x00c060c0, 0x00c060ff, 0x00c0a000,
    0x00c0a040, 0x00c0a080, 0x00c0a0c0, 0x00c0a0ff, 0x00c0e000, 0x00c0e040, 0x00c0e080, 0x00c0e0c0,
    0x00c0e0ff, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00ff2000, 0x00ff2040,
    0x00ff2080, 0x00ff20c0, 0x00ff20ff, 0x00ff6000, 0x00ff6040, 0x00ff6080, 0x00ff60c0, 0x00ff60ff,
    0x00ffa000, 0x00ffa040, 0x00ffa080, 0x00ffa0c0, 0x00ffa0ff, 0x00ffe000, 0x00ffe040, 0x00ffe080,
    0x00ffe0c0, 0x00ffe0ff, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00002020,
    0x00002060, 0x000020a0, 0x000020e0, 0x00000000, 0x00006020, 0x00006060, 0x000060a0, 0x000060e0,
    0x00000000, 0x0000a020, 0x0000a060, 0x0000a0a0, 0x0000a0e0, 0x00000000, 0x0000e020, 0x0000e060,
    0x0000e0a0, 0x0000e0e0, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00402020, 0x00402060, 0x004020a0, 0x004020e0, 0x00000000, 0x00406020, 0x00406060, 0x004060a0,
    0x004060e0, 0x00000000, 0x0040a020, 0x0040a060, 0x0040a0a0, 0x0040a0e0, 0x00000000, 0x0040e020,
    0x0040e060, 0x0040e0a0, 0x0040e0e0, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00802020, 0x00802060, 0x008020a0, 0x008020e0, 0x00000000, 0x00806020, 0x00806060,
    0x008060a0, 0x008060e0, 0x00000000, 0x0080a020, 0x0080a060, 0x0080a0a0, 0x0080a0e0, 0x00000000,
    0x0080e020, 0x0080e060, 0x0080e0a0, 0x0080e0e0, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00c02020, 0x00c02060, 0x00c020a0, 0x00c020e0, 0x00000000, 0x00c06020,
    0x00c06060, 0x00c060a0, 0x00c060e0, 0x00000000, 0x00c0a020, 0x00c0a060, 0x00c0a0a0, 0x00c0a0e0,
    0x00000000, 0x00c0e020, 0x00c0e060, 0x00c0e0a0, 0x00c0e0e0, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00ff2020, 0x00ff2060, 0x00ff20a0, 0x00ff20e0, 0x00000000,
    0x00ff6020, 0x00ff6060, 0x00ff60a0, 0x00ff60e0, 0x00000000, 0x00ffa020, 0x00ffa060, 0x00ffa0a0,
    0x00ffa0e0, 0x00000000, 0x00ffe020, 0x00ffe060, 0x00ffe0a0, 0x00ffe0e0, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00200000, 0x00200040, 0x00200080, 0x002000c0,
    0x002000ff, 0x00204000, 0x00204040, 0x00204080, 0x002040c0, 0x002040ff, 0x00208000, 0x00208040,
    0x00208080, 0x002080c0, 0x002080ff, 0x0020c000, 0x0020c040, 0x0020c080, 0x0020c0c0, 0x0020c0ff,
    0x0020ff00, 0x0020ff40, 0x0020ff80, 0x0020ffc0, 0x0020ffff, 0x00600000, 0x00600040, 0x00600080,
    0x006000c0, 0x006000ff, 0x00604000, 0x00604040, 0x00604080, 0x006040c0, 0x006040ff, 0x00608000,
    0x00608040, 0x00608080, 0x006080c0, 0x006080ff, 0x0060c000, 0x0060c040, 0x0060c080, 0x0060c0c0,
    0x0060c0ff, 0x0060ff00, 0x0060ff40, 0x0060ff80, 0x0060ffc0, 0x0060ffff, 0x00a00000, 0x00a00040,
    0x00a00080, 0x00a000c0, 0x00a000ff, 0x00a04000, 0x00a04040, 0x00a04080, 0x00a040c0, 0x00a040ff,
    0x00a08000, 0x00a08040, 0x00a08080, 0x00a080c0, 0x00a080ff, 0x00a0c000, 0x00a0c040, 0x00a0c080,
    0x00a0c0c0, 0x00a0c0ff, 0x00a0ff00, 0x00a0ff40, 0x00a0ff80, 0x00a0ffc0, 0x00a0ffff, 0x00e00000,
    0x00e00040, 0x00e00080, 0x00e000c0, 0x00e000ff, 0x00e04000, 0x00e04040, 0x00e04080, 0x00e040c0,
    0x00e040ff, 0x00e08000, 0x00e08040, 0x00e08080, 0x00e080c0, 0x00e080ff, 0x00e0c000, 0x00e0c040,
    0x00e0c080, 0x00e0c0c0, 0x00e0c0ff, 0x00e0ff00, 0x00e0ff40, 0x00e0ff80, 0x00e0ffc0, 0x00e0ffff,
    0x00200020, 0x00200060, 0x002000a0, 0x002000e0, 0x00000000, 0x00204020, 0x00204060, 0x002040a0,
    0x002040e0, 0x00000000, 0x00208020, 0x00208060, 0x002080a0, 0x002080e0, 0x00000000, 0x0020c020,
    0x0020c060, 0x0020c0a0, 0x0020c0e0, 0x00000000, 0x0020ff20, 0x0020ff60, 0x0020ffa0, 0x0020ffe0,
    0x00000000, 0x00600020, 0x00600060, 0x006000a0, 0x006000e0, 0x00000000, 0x00604020, 0x00604060,
    0x006040a0, 0x006040e0, 0x00000000, 0x00608020, 0x00608060, 0x006080a0, 0x006080e0, 0x00000000,
    0x0060c020, 0x0060c060, 0x0060c0a0, 0x0060c0e0, 0x00000000, 0x0060ff20, 0x0060ff60, 0x0060ffa0,
    0x0060ffe0, 0x00000000, 0x00a00020, 0x00a00060, 0x00a000a0, 0x00a000e0, 0x00000000, 0x00a04020,
    0x00a04060, 0x00a040a0, 0x00a040e0, 0x00000000, 0x00a08020, 0x00a08060, 0x00a080a0, 0x00a080e0,
    0x00000000, 0x00a0c020, 0x00a0c060, 0x00a0c0a0, 0x00a0c0e0, 0x00000000, 0x00a0ff20, 0x00a0ff60,
    0x00a0ffa0, 0x00a0ffe0, 0x00000000, 0x00e00020, 0x00e00060, 0x00e000a0, 0x00e000e0, 0x00000000,
    0x00e04020, 0x00e04060, 0x00e040a0, 0x00e040e0, 0x00000000, 0x00e08020, 0x00e08060, 0x00e080a0,
    0x00e080e0, 0x00000000, 0x00e0c020, 0x00e0c060, 0x00e0c0a0, 0x00e0c0e0, 0x00000000, 0x00e0ff20,
    0x00e0ff60, 0x00e0ffa0, 0x00e0ffe0, 0x00000000, 0x00202000, 0x00202040, 0x00202080, 0x002020c0,
    0x002020ff, 0x00206000, 0x00206040, 0x00206080, 0x002060c0, 0x002060ff, 0x0020a000, 0x0020a040,
    0x0020a080, 0x0020a0c0, 0x0020a0ff, 0x0020e000, 0x0020e040, 0x0020e080, 0x0020e0c0, 0x0020e0ff,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00602000, 0x00602040, 0x00602080,
    0x006020c0, 0x006020ff, 0x00606000, 0x00606040, 0x00606080, 0x006060c0, 0x006060ff, 0x0060a000,
    0x0060a040, 0x0060a080, 0x0060a0c0, 0x0060a0ff, 0x0060e000, 0x0060e040, 0x0060e080, 0x0060e0c0,
    0x0060e0ff, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00a02000, 0x00a02040,
    0x00a02080, 0x00a020c0, 0x00a020ff, 0x00a06000, 0x00a06040, 0x00a06080, 0x00a060c0, 0x00a060ff,
    0x00a0a000, 0x00a0a040, 0x00a0a080, 0x00a0a0c0, 0x00a0a0ff, 0x00a0e000, 0x00a0e040, 0x00a0e080,
    0x00a0e0c0, 0x00a0e0ff, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00e02000,
    0x00e02040, 0x00e02080, 0x00e020c0, 0x00e020ff, 0x00e06000, 0x00e06040, 0x00e06080, 0x00e060c0,
    0x00e060ff, 0x00e0a000, 0x00e0a040, 0x00e0a080, 0x00e0a0c0, 0x00e0a0ff, 0x00e0e000, 0x00e0e040,
    0x00e0e080, 0x00e0e0c0, 0x00e0e0ff, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00202020, 0x00202060, 0x002020a0, 0x002020e0, 0x00000000, 0x00206020, 0x00206060, 0x002060a0,
    0x002060e0, 0x00000000, 0x0020a020, 0x0020a060, 0x0020a0a0, 0x0020a0e0, 0x00000000, 0x0020e020,
    0x0020e060, 0x0020e0a0, 0x0020e0e0, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00602020, 0x00602060, 0x006020a0, 0x006020e0, 0x00000000, 0x00606020, 0x00606060,
    0x006060a0, 0x006060e0, 0x00000000, 0x0060a020, 0x0060a060, 0x0060a0a0, 0x0060a0e0, 0x00000000,
    0x0060e020, 0x0060e060, 0x0060e0a0, 0x0060e0e0, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00a02020, 0x00a02060, 0x00a020a0, 0x00a020e0, 0x00000000, 0x00a06020,
    0x00a06060, 0x00a060a0, 0x00a060e0, 0x00000000, 0x00a0a020, 0x00a0a060, 0x00a0a0a0, 0x00a0a0e0,
    0x00000000, 0x00a0e020, 0x00a0e060, 0x00a0e0a0, 0x00a0e0e0, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00e02020, 0x00e02060, 0x00e020a0, 0x00e020e0, 0x00000000,
    0x00e06020, 0x00e06060, 0x00e060a0, 0x00e060e0, 0x00000000, 0x00e0a020, 0x00e0a060, 0x00e0a0a0,
    0x00e0a0e0, 0x00000000, 0x00e0e020, 0x00e0e060, 0x00e0e0a0, 0x00e0e0e0, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000
};

IPE_3DCCRound color_3d_imx290_evb = {SEL_RANDOM, TRUE};

SHDR_IQ_PARA hdr_imx290_evb =
{
    {0, 16, 32, 144, 255, 0, 64, 255, 64, 0},
    128, // uiRefMapWeight 0~255
    16, // Bright Contrast Enance level 0~32
    SHDR_CTRL_byHDRLib
};

S_DEFOG_LIB_INIT_PARA defog_init_param_imx290_evb =
{
	{32,32, 40, 40, 50, 50, 64, 64, 64},
	255
};

UINT32 defog_wet_imx290_evb[17] =
{
    255, 255, 255, 255, 255, 255, 255, 255, 255,
    255, 255, 255, 255, 255, 255, 255, 255
};
#endif
