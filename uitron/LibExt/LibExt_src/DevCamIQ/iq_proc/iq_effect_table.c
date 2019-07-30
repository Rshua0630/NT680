#include "iq_common.h"

                                                    //CbOfs CrOfs YCon CCon IntOfs SatOfs HueRotate
IQS_COLOR_EFFECT_PARAM ColorEffectTable_Normal       = {128, 128,  128, 128,    0,     0,   FALSE };    //SEL_IMAGEEFFECT_OFF = 0,
IQS_COLOR_EFFECT_PARAM ColorEffectTable_BW           = {128, 128,  128,   0,    0,     0,   FALSE };    //SEL_IMAGEEFFECT_BW,
IQS_COLOR_EFFECT_PARAM ColorEffectTable_SEPIA        = {116, 144,  128,   0,    0,     0,   FALSE };    //SEL_IMAGEEFFECT_SEPIA,
IQS_COLOR_EFFECT_PARAM ColorEffectTable_VIVID        = {128, 128,  128,  128,   0,    50,   FALSE };    //SEL_IMAGEEFFECT_VIVID,
IQS_COLOR_EFFECT_PARAM ColorEffectTable_PencilSketch = {128, 128,  224,    0,   0,  -128,   FALSE };    //SEL_IMAGEEFFECT_COLORPENCIL,
IQS_COLOR_EFFECT_PARAM ColorEffectTable_ColorPencil  = {128, 128,  224,  128,   0,     0,   FALSE };    //SEL_IMAGEEFFECT_SKETCH,
IQS_COLOR_EFFECT_PARAM ColorEffectTable_CoolGreen    = {128,  64,  128,  128,   0,     0,   FALSE };    //SEL_IMAGEEFFECT_SKETCH,
IQS_COLOR_EFFECT_PARAM ColorEffectTable_WarmYellow   = {64,  160,  128,  128,   0,     0,   FALSE };    //SEL_IMAGEEFFECT_SKETCH,

//////////////////////////////////
// Color pencil
//////////////////////////////////
IPE_YCCFIX IE_ColorPencil_Param =
{
  //yth1{YTH,  ETH,  HIT1SEL,NHIT1SEL,HIT,NHIT},  yth2{YTH, HIT2SEL,NHIT2SEL,HIT,NHIT}, CC{ETH, YTG, CBTH,  CRTH, HITSEL,NHITSEL,CB_HIT,CB_NHIT,CR_HIT,CR_NHIT}
        {0,     0,   ENABLE,  DISABLE,  128, 0},      {0,    FALSE,  FALSE,    0,   0},   {0,   0,0,  0,0,  0,0,  FALSE,  FALSE,     0,      0,     0,     0},
};

//////////////////////////////////
// Sketch
//////////////////////////////////
IPE_YCCFIX IE_PencilSketch_Param =
{
  //yth1{YTH,  ETH,  HIT1SEL,NHIT1SEL,HIT,NHIT},  yth2{YTH, HIT2SEL,NHIT2SEL,HIT,NHIT}, CC{ETH, YTG, CBTH,  CRTH, HITSEL,NHITSEL,CB_HIT,CB_NHIT,CR_HIT,CR_NHIT}
        {0,    0,   ENABLE,  DISABLE,  128, 0},      {0,    FALSE,  FALSE,    0,   0},   {0,   0,0,  0,0,  0,0,  FALSE,  FALSE,     0,      0,     0,     0},
};

///////////////////////
// IPP Rock
///////////////////////
IPE_YCCFIX IPE_RockEffect_YccFix =
{
	//yth1{YTH,  ETH,  HIT1SEL,NHIT1SEL,HIT,NHIT},  yth2{YTH, HIT2SEL,NHIT2SEL,HIT,NHIT}, CC{ETH, YTG, CBTH,  CRTH, HITSEL,NHITSEL,CB_HIT,CB_NHIT,CR_HIT,CR_NHIT}
	{0,      0,    FALSE,  FALSE,    0,   0},    {128,   TRUE,  TRUE,    112,   0},   {0,  128, 255, 0, 255, 0, 255, TRUE,  TRUE,    165,    128,   235,    128} // Pink
    //{0,      0,    FALSE,  FALSE,    0,   0},    {128,   TRUE,  TRUE,    192,   0},   {0,  128,255,0,255,0,255, TRUE,  TRUE,    64,    128,   160,    128} // Yellow
};

///////////////////////////////
// DCE_Init
///////////////////////////////
UINT32 IQS_DCE_GLUT_Init[65] =
{
	65535, 65535, 65535, 65535, 65535, 65535, 65535, 65535, 65535, 65535,
	65535, 65535, 65535, 65535, 65535, 65535, 65535, 65535, 65535, 65535,
	65535, 65535, 65535, 65535, 65535, 65535, 65535, 65535, 65535, 65535,
	65535, 65535, 65535, 65535, 65535, 65535, 65535, 65535, 65535, 65535,
	65535, 65535, 65535, 65535, 65535, 65535, 65535, 65535, 65535, 65535,
	65535, 65535, 65535, 65535, 65535, 65535, 65535, 65535, 65535, 65535,
	65535, 65535, 65535, 65535, 65535
};

//////////////////////////////////
// Fisheye
//////////////////////////////////
UINT32 IE_FISYEYE_LUT[65] =
{
	55705, 55933, 56161, 56390, 56617, 56842, 57065, 57286, 57503, 57717,
	57927, 58133, 58335, 58533, 58726, 58915, 59100, 59281, 59458, 59631,
	59801, 59967, 60130, 60291, 60449, 60605, 60760, 60913, 61064, 61215,
	61365, 61515, 61666, 61816, 61967, 62118, 62270, 62423, 62577, 62732,
	62888, 63045, 63203, 63361, 63520, 63678, 63836, 63993, 64148, 64301,
	64450, 64596, 64737, 64872, 65000, 65119, 65228, 65326, 65411, 65481,
	65534, 65535, 65535, 65535, 65535
};

//////////////////
// IFE-Vignette
//////////////////
IQS_VIG_SET g_vig_init =
{
    VIG_TAB_GAIN_2X, {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
};

///////////////////////
// Selective CC
///////////////////////
INT32 IPE_SelectiveCC_SatTab[15][24] =
{
    //  0                        5                       10                       15                       20
	{   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0},          //CCID_NO_EFFECT,
	{  99,  99,  99,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,  99},          //CCID_ENHANCE_RED,
	{   0,   0,   0,  99,  99,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0},          //CCID_ENHANCE_YELLOW,
	{   0,   0,   0,   0,   0,   0,   0,  99,  99,  99,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0},          //CCID_ENHANCE_GREEN,
	{   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,  99,  99,  99,   0,   0,   0,   0,   0,   0,   0},          //CCID_ENHANCE_BLUE,
	{   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,  99,  99,   0,   0,   0,   0,   0,   0,   0,   0,   0},          //CCID_ENHANCE_CYAN,
	{   0,   0,  99,  99,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0},          //CCID_ENHANCE_ORANGE,
	{   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,  99,  99,  99,   0},          //CCID_ENHANCE_MAGENTA,
	{   0,   0,   0, -128, -128, -128, -128, -128, -128, -128, -128, -128, -128, -128, -128, -128, -128, -128, -128, -128, -128, -128, -128,   0}, //CCID_REMOVAL_RED,
	{ -128, -128, -128,   0,   0, -128, -128, -128, -128, -128, -128, -128, -128, -128, -128, -128, -128, -128, -128, -128, -128, -128, -128, -128}, //CCID_REMOVAL_YELLOW,
	{ -128, -128, -128, -128, -128, -128, -128,   0,   0,   0, -128, -128, -128, -128, -128, -128, -128, -128, -128, -128, -128, -128, -128, -128}, //CCID_REMOVAL_GREEN,
	{ -128, -128, -128, -128, -128, -128, -128, -128, -128, -128, -128, -128, -128, -128,   0,   0,   0, -128, -128, -128, -128, -128, -128, -128}, //CCID_REMOVAL_BLUE,
	{ -128, -128, -128, -128, -128, -128, -128, -128, -128, -128, -128, -128, -128,   0,   0, -128, -128, -128, -128, -128, -128, -128, -128, -128}, //CCID_REMOVAL_CYAN,
	{ -128, -128,   0,   0, -128, -128, -128, -128, -128, -128, -128, -128, -128, -128, -128, -128, -128, -128, -128, -128, -128, -128, -128, -128}, //CCID_REMOVAL_ORANGE,
	{ -128, -128, -128, -128, -128, -128, -128, -128, -128, -128, -128, -128, -128, -128, -128, -128, -128, -128, -128, -128,   0,   0,   0, -128}, //CCID_REMOVAL_MAGENTA

};

UINT32 IE_Gamma_Negative[129] =
{
    1023 ,1003 ,967 ,932 ,899 ,869 ,835 ,800 ,768 ,728 ,687 ,648 ,612 ,576 ,542 ,510 ,483 ,467 ,452 ,
    437 ,422 ,406 ,385 ,366 ,346 ,327 ,309 ,292 ,278 ,264 ,251 ,237 ,225 ,212 ,200 ,188 ,179 ,170 ,
    162 ,153 ,145 ,137 ,128 ,121 ,113 ,106 ,100 ,93 ,88 ,82 ,76 ,70 ,64 ,59 ,54 ,49 ,44 ,
    38 ,34 ,31 ,29 ,28 ,26 ,25 ,23 ,22 ,20 ,20 ,19 ,18 ,17 ,17 ,16 ,16 ,15 ,14 ,
    14 ,13 ,13 ,12 ,12 ,11 ,11 ,10 ,10 ,9 ,9 ,8 ,8 ,8 ,7 ,7 ,7 ,7 ,6 ,6 ,6 ,6 ,6 ,5 ,5 ,5 ,5 ,5 ,
    4 ,4 ,4 ,4 ,4 ,3 ,3 ,3 ,3 ,3 ,2 ,2 ,2 ,2 ,2 ,2 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,0 ,0
};

IPE_EethParam g_ipe_eth =
{
    SEL_ETH_ORG, ETH_8BITS, 16, 48, 128, 0, 0
};

//////////////////////////////////
// DRE param
//////////////////////////////////
DRAM_DG_INFO IQS_DRE_DGMask[DRE_DG_PROC_MAX_NUM] = {{0, 0}};

static IQS_DRE_NR_IQPARAM IQS_MSNR_NR1 =
{
    { 1, 1, 1, 1, 1, 1},
    { 0x1c, 0x1c, 0x20, 0x1c, 0x18, 0x14,  0xc,  0x4},
    {  0x2,  0x2,  0x2,  0x2,  0x2,  0x2,  0x2,  0x2},
    {  0x2,  0x2,  0x2,  0x2,  0x2,  0x2,  0x2,  0x2},
    ENABLE, ENABLE, ENABLE,  //Gamma En
    {   0x00, 0x01, 0x01, 0x02, 0x02, 0x03, 0x03, 0x04, 0x04, 0x05, 0x05, 0x06, 0x06, 0x07, 0x07, 0x08,  0x08, 0x09, 0x09, 0x0A, 0x0A, 0x0B, 0x0B, 0x0C, 0x0C, 0x0D, 0x0D, 0x0E, 0x0E, 0x0F, 0x0F, 0x10,
        0x10, 0x11, 0x11, 0x12, 0x12, 0x13, 0x13, 0x14, 0x14, 0x15, 0x15, 0x16, 0x16, 0x17, 0x17, 0x18,  0x18, 0x19, 0x19, 0x1A, 0x1A, 0x1B, 0x1B, 0x1C, 0x1C, 0x1D, 0x1D, 0x1E, 0x1E, 0x1F, 0x1F, 0x20,
        0x20, 0x21, 0x21, 0x22, 0x22, 0x23, 0x23, 0x24, 0x24, 0x25, 0x25, 0x26, 0x26, 0x27, 0x27, 0x28,  0x28, 0x29, 0x29, 0x2A, 0x2A, 0x2B, 0x2B, 0x2C, 0x2C, 0x2D, 0x2D, 0x2E, 0x2E, 0x2F, 0x2F, 0x30,
        0x30, 0x31, 0x31, 0x32, 0x32, 0x33, 0x33, 0x34, 0x34, 0x35, 0x35, 0x36, 0x36, 0x37, 0x37, 0x38,  0x38, 0x39, 0x39, 0x3A, 0x3A, 0x3B, 0x3B, 0x3C, 0x3C, 0x3D, 0x3D, 0x3E, 0x3E, 0x3F, 0x3F, 0x40},
    {   0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,        0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
        0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,        0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
        0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,        0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
        0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,        0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0},
    {   0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,        0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
        0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,        0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
        0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,        0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
        0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,        0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0},
    { 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10},
    { 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10},
    {128, 128, 0, 0, FALSE, FALSE},
};

static IQS_DRE_NR_IQPARAM IQS_MSNR_NR2 =
{
    { 1, 1, 1, 1, 1, 1},
    {  0xa,  0xa,  0xc,  0xa,  0x8,  0x6,  0x4,  0x4},
    { 0x19, 0x19, 0x19, 0x19, 0x19, 0x14,  0xf,  0x5},
    { 0x19, 0x19, 0x19, 0x19, 0x19, 0x14,  0xf,  0x5},
    ENABLE, ENABLE, ENABLE,  //Gamma En
    {   0x0,  0x0,  0x0,  0x1,  0x1,  0x1,  0x1,  0x1,  0x5,  0x5,  0x6,  0x7,  0x7,  0x8,  0x8,  0x9,  0x0D, 0x0E, 0x0E, 0x0F, 0x10, 0x11, 0x12, 0x12, 0x13, 0x14, 0x15, 0x16, 0x16, 0x17, 0x18, 0x19,
        0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28, 0x29, 0x2A, 0x2B, 0x2C, 0x2D, 0x2E, 0x2F,  0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x3A, 0x3B, 0x3C, 0x3D, 0x3E, 0x3F,
        0x40, 0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x47, 0x48, 0x49, 0x4A, 0x4B, 0x4C, 0x4D, 0x4E, 0x4F,  0x50, 0x51, 0x52, 0x53, 0x54, 0x55, 0x56, 0x57, 0x58, 0x59, 0x5A, 0x5B, 0x5C, 0x5D, 0x5E, 0x5F,
        0x60, 0x61, 0x62, 0x63, 0x64, 0x65, 0x66, 0x67, 0x68, 0x69, 0x6A, 0x6B, 0x6C, 0x6D, 0x6E, 0x6F,  0x70, 0x71, 0x72, 0x73, 0x74, 0x75, 0x76, 0x77, 0x78, 0x79, 0x7A, 0x7B, 0x7C, 0x7D, 0x7E, 0x7F},
    {   0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,        0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
        0x10, 0x11, 0x11, 0x12, 0x12, 0x13, 0x13, 0x14, 0x14, 0x15, 0x15, 0x16, 0x16, 0x17, 0x17, 0x18,        0x18, 0x19, 0x19, 0x1A, 0x1A, 0x1B, 0x1B, 0x1C, 0x1C, 0x1D, 0x1D, 0x1E, 0x1E, 0x1F, 0x1F, 0x20,
        0x40, 0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x47, 0x48, 0x49, 0x4A, 0x4B, 0x4C, 0x4D, 0x4E, 0x4F,        0x50, 0x51, 0x52, 0x53, 0x54, 0x55, 0x56, 0x57, 0x58, 0x59, 0x5A, 0x5B, 0x5C, 0x5D, 0x5E, 0x5F,
        0x60, 0x61, 0x62, 0x63, 0x64, 0x65, 0x66, 0x67, 0x68, 0x69, 0x6A, 0x6B, 0x6C, 0x6D, 0x6E, 0x6F,        0x70, 0x71, 0x72, 0x73, 0x74, 0x75, 0x76, 0x77, 0x78, 0x79, 0x7A, 0x7B, 0x7C, 0x7D, 0x7E, 0x7F},
    {   0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,        0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
        0x10, 0x11, 0x11, 0x12, 0x12, 0x13, 0x13, 0x14, 0x14, 0x15, 0x15, 0x16, 0x16, 0x17, 0x17, 0x18,        0x18, 0x19, 0x19, 0x1A, 0x1A, 0x1B, 0x1B, 0x1C, 0x1C, 0x1D, 0x1D, 0x1E, 0x1E, 0x1F, 0x1F, 0x20,
        0x40, 0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x47, 0x48, 0x49, 0x4A, 0x4B, 0x4C, 0x4D, 0x4E, 0x4F,        0x50, 0x51, 0x52, 0x53, 0x54, 0x55, 0x56, 0x57, 0x58, 0x59, 0x5A, 0x5B, 0x5C, 0x5D, 0x5E, 0x5F,
        0x60, 0x61, 0x62, 0x63, 0x64, 0x65, 0x66, 0x67, 0x68, 0x69, 0x6A, 0x6B, 0x6C, 0x6D, 0x6E, 0x6F,        0x70, 0x71, 0x72, 0x73, 0x74, 0x75, 0x76, 0x77, 0x78, 0x79, 0x7A, 0x7B, 0x7C, 0x7D, 0x7E, 0x7F},
    { 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10},
    { 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10},
    {128, 128, 0, 0, FALSE, FALSE},
};

static IQS_DRE_NR_IQPARAM IQS_MSNR_NR3 =
{
    { 1, 1, 1, 1, 1, 1},
    {  0x4,  0x4,  0x6,  0x4,  0x4,  0x4,  0x2,  0x2},
    {  0xf,  0xf,  0xf,  0xf,  0xf,  0xc,  0x7,  0x2},
    {  0xf,  0xf,  0xf,  0xf,  0xf,  0xc,  0x7,  0x2},
    ENABLE, ENABLE, ENABLE,  //Gamma En
    {   0x0,  0x0,  0x0,  0x1,  0x1,  0x1,  0x1,  0x1,  0x5,  0x5,  0x6,  0x7,  0x7,  0x8,  0x8,  0x9,  0x0D, 0x0E, 0x0E, 0x0F, 0x10, 0x11, 0x12, 0x12, 0x13, 0x14, 0x15, 0x16, 0x16, 0x17, 0x18, 0x19,
        0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28, 0x29, 0x2A, 0x2B, 0x2C, 0x2D, 0x2E, 0x2F,  0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x3A, 0x3B, 0x3C, 0x3D, 0x3E, 0x3F,
        0x40, 0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x47, 0x48, 0x49, 0x4A, 0x4B, 0x4C, 0x4D, 0x4E, 0x4F,  0x50, 0x51, 0x52, 0x53, 0x54, 0x55, 0x56, 0x57, 0x58, 0x59, 0x5A, 0x5B, 0x5C, 0x5D, 0x5E, 0x5F,
        0x60, 0x61, 0x62, 0x63, 0x64, 0x65, 0x66, 0x67, 0x68, 0x69, 0x6A, 0x6B, 0x6C, 0x6D, 0x6E, 0x6F,  0x70, 0x71, 0x72, 0x73, 0x74, 0x75, 0x76, 0x77, 0x78, 0x79, 0x7A, 0x7B, 0x7C, 0x7D, 0x7E, 0x7F},
    {   0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x1,        0x2, 0x3, 0x4, 0x5, 0x6, 0x7, 0x8, 0x9, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x10, 0x12,
        0x14, 0x16, 0x18, 0x1A, 0x1C, 0x1E, 0x20, 0x22, 0x24, 0x26, 0x28, 0x2A, 0x2C, 0x2D, 0x2E, 0x2F,        0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x3A, 0x3B, 0x3C, 0x3D, 0x3E, 0x3F,
        0x40, 0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x47, 0x48, 0x49, 0x4A, 0x4B, 0x4C, 0x4D, 0x4E, 0x4F,        0x50, 0x51, 0x52, 0x53, 0x54, 0x55, 0x56, 0x57, 0x58, 0x59, 0x5A, 0x5B, 0x5C, 0x5D, 0x5E, 0x5F,
        0x60, 0x61, 0x62, 0x63, 0x64, 0x65, 0x66, 0x67, 0x68, 0x69, 0x6A, 0x6B, 0x6C, 0x6D, 0x6E, 0x6F,        0x70, 0x71, 0x72, 0x73, 0x74, 0x75, 0x76, 0x77, 0x78, 0x79, 0x7A, 0x7B, 0x7C, 0x7D, 0x7E, 0x7F},
    {   0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x1,        0x2, 0x3, 0x4, 0x5, 0x6, 0x7, 0x8, 0x9, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x10, 0x12,
        0x14, 0x16, 0x18, 0x1A, 0x1C, 0x1E, 0x20, 0x22, 0x24, 0x26, 0x28, 0x2A, 0x2C, 0x2D, 0x2E, 0x2F,        0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x3A, 0x3B, 0x3C, 0x3D, 0x3E, 0x3F,
        0x40, 0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x47, 0x48, 0x49, 0x4A, 0x4B, 0x4C, 0x4D, 0x4E, 0x4F,        0x50, 0x51, 0x52, 0x53, 0x54, 0x55, 0x56, 0x57, 0x58, 0x59, 0x5A, 0x5B, 0x5C, 0x5D, 0x5E, 0x5F,
        0x60, 0x61, 0x62, 0x63, 0x64, 0x65, 0x66, 0x67, 0x68, 0x69, 0x6A, 0x6B, 0x6C, 0x6D, 0x6E, 0x6F,        0x70, 0x71, 0x72, 0x73, 0x74, 0x75, 0x76, 0x77, 0x78, 0x79, 0x7A, 0x7B, 0x7C, 0x7D, 0x7E, 0x7F},
    { 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10},
    { 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10},
    {128, 128, 0, 0, FALSE, FALSE},
};

static IQS_DRE_NR_IQPARAM IQS_MSNR_NR4 =
{
    { 1, 1, 1, 1, 1, 1},
    {  0x2,  0x2,  0x2,  0x2,  0x2,  0x2,  0x2,  0x2},
    {  0xa,  0xa,  0xa,  0xa,  0xa,  0x7,  0x2,  0x2},
    {  0xa,  0xa,  0xa,  0xa,  0xa,  0x7,  0x2,  0x2},
    ENABLE, ENABLE, ENABLE,  //Gamma En
    {   0x0,  0x0,  0x0,  0x1,  0x2,  0x3,  0x4,  0x5,  0x6,  0x7,  0x8,  0x9, 0x0B, 0x0D, 0x0E, 0x0F,  0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1A, 0x1B, 0x1C, 0x1D, 0x1E, 0x1F,
        0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28, 0x29, 0x2A, 0x2B, 0x2C, 0x2D, 0x2E, 0x2F,  0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x3A, 0x3B, 0x3C, 0x3D, 0x3E, 0x3F,
        0x40, 0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x47, 0x48, 0x49, 0x4A, 0x4B, 0x4C, 0x4D, 0x4E, 0x4F,  0x50, 0x51, 0x52, 0x53, 0x54, 0x55, 0x56, 0x57, 0x58, 0x59, 0x5A, 0x5B, 0x5C, 0x5D, 0x5E, 0x5F,
        0x60, 0x61, 0x62, 0x63, 0x64, 0x65, 0x66, 0x67, 0x68, 0x69, 0x6A, 0x6B, 0x6C, 0x6D, 0x6E, 0x6F,  0x70, 0x71, 0x72, 0x73, 0x74, 0x75, 0x76, 0x77, 0x78, 0x79, 0x7A, 0x7B, 0x7C, 0x7D, 0x7E, 0x7F},
    {   0x0, 0x0, 0x0, 0x0, 0x1, 0x2, 0x3, 0x4, 0x5, 0x6, 0x7, 0x9, 0x0A, 0x0C, 0x0E, 0x0F,        0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1A, 0x1B, 0x1C, 0x1D, 0x1E, 0x1F,
        0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28, 0x29, 0x2A, 0x2B, 0x2C, 0x2D, 0x2E, 0x2F,        0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x3A, 0x3B, 0x3C, 0x3D, 0x3E, 0x3F,
        0x40, 0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x47, 0x48, 0x49, 0x4A, 0x4B, 0x4C, 0x4D, 0x4E, 0x4F,        0x50, 0x51, 0x52, 0x53, 0x54, 0x55, 0x56, 0x57, 0x58, 0x59, 0x5A, 0x5B, 0x5C, 0x5D, 0x5E, 0x5F,
        0x60, 0x61, 0x62, 0x63, 0x64, 0x65, 0x66, 0x67, 0x68, 0x69, 0x6A, 0x6B, 0x6C, 0x6D, 0x6E, 0x6F,        0x70, 0x71, 0x72, 0x73, 0x74, 0x75, 0x76, 0x77, 0x78, 0x79, 0x7A, 0x7B, 0x7C, 0x7D, 0x7E, 0x7F},
    {   0x0, 0x0, 0x0, 0x0, 0x1, 0x2, 0x3, 0x4, 0x5, 0x6, 0x7, 0x9, 0x0A, 0x0C, 0x0E, 0x0F,        0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1A, 0x1B, 0x1C, 0x1D, 0x1E, 0x1F,
        0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28, 0x29, 0x2A, 0x2B, 0x2C, 0x2D, 0x2E, 0x2F,        0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x3A, 0x3B, 0x3C, 0x3D, 0x3E, 0x3F,
        0x40, 0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x47, 0x48, 0x49, 0x4A, 0x4B, 0x4C, 0x4D, 0x4E, 0x4F,        0x50, 0x51, 0x52, 0x53, 0x54, 0x55, 0x56, 0x57, 0x58, 0x59, 0x5A, 0x5B, 0x5C, 0x5D, 0x5E, 0x5F,
        0x60, 0x61, 0x62, 0x63, 0x64, 0x65, 0x66, 0x67, 0x68, 0x69, 0x6A, 0x6B, 0x6C, 0x6D, 0x6E, 0x6F,        0x70, 0x71, 0x72, 0x73, 0x74, 0x75, 0x76, 0x77, 0x78, 0x79, 0x7A, 0x7B, 0x7C, 0x7D, 0x7E, 0x7F},
    { 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10},
    { 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10},
    {128, 128, 0, 0, FALSE, FALSE},
};

static IQS_DRE_NR_IQPARAM IQS_MSNR_NR5 =
{
    { 1, 1, 1, 1, 1, 1},
    {  0x2,  0x2,  0x2,  0x2,  0x2,  0x2,  0x2,  0x2},
    {  0xa,  0xa,  0xa,  0xa,  0xa,  0x7,  0x2,  0x2},
    {  0xa,  0xa,  0xa,  0xa,  0xa,  0x7,  0x2,  0x2},
    ENABLE, ENABLE, ENABLE,  //Gamma En
    {   0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F,  0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1A, 0x1B, 0x1C, 0x1D, 0x1E, 0x1F,
        0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28, 0x29, 0x2A, 0x2B, 0x2C, 0x2D, 0x2E, 0x2F,  0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x3A, 0x3B, 0x3C, 0x3D, 0x3E, 0x3F,
        0x40, 0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x47, 0x48, 0x49, 0x4A, 0x4B, 0x4C, 0x4D, 0x4E, 0x4F,  0x50, 0x51, 0x52, 0x53, 0x54, 0x55, 0x56, 0x57, 0x58, 0x59, 0x5A, 0x5B, 0x5C, 0x5D, 0x5E, 0x5F,
        0x60, 0x61, 0x62, 0x63, 0x64, 0x65, 0x66, 0x67, 0x68, 0x69, 0x6A, 0x6B, 0x6C, 0x6D, 0x6E, 0x6F,  0x70, 0x71, 0x72, 0x73, 0x74, 0x75, 0x76, 0x77, 0x78, 0x79, 0x7A, 0x7B, 0x7C, 0x7D, 0x7E, 0x7F},
    {   0x0, 0x0, 0x0, 0x1, 0x3, 0x5, 0x6, 0x7, 0x8, 0x9, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F,        0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1A, 0x1B, 0x1C, 0x1D, 0x1E, 0x1F,
        0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28, 0x29, 0x2A, 0x2B, 0x2C, 0x2D, 0x2E, 0x2F,        0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x3A, 0x3B, 0x3C, 0x3D, 0x3E, 0x3F,
        0x40, 0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x47, 0x48, 0x49, 0x4A, 0x4B, 0x4C, 0x4D, 0x4E, 0x4F,        0x50, 0x51, 0x52, 0x53, 0x54, 0x55, 0x56, 0x57, 0x58, 0x59, 0x5A, 0x5B, 0x5C, 0x5D, 0x5E, 0x5F,
        0x60, 0x61, 0x62, 0x63, 0x64, 0x65, 0x66, 0x67, 0x68, 0x69, 0x6A, 0x6B, 0x6C, 0x6D, 0x6E, 0x6F,        0x70, 0x71, 0x72, 0x73, 0x74, 0x75, 0x76, 0x77, 0x78, 0x79, 0x7A, 0x7B, 0x7C, 0x7D, 0x7E, 0x7F},
    {   0x0, 0x0, 0x0, 0x1, 0x3, 0x5, 0x6, 0x7, 0x8, 0x9, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F,        0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1A, 0x1B, 0x1C, 0x1D, 0x1E, 0x1F,
        0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28, 0x29, 0x2A, 0x2B, 0x2C, 0x2D, 0x2E, 0x2F,        0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x3A, 0x3B, 0x3C, 0x3D, 0x3E, 0x3F,
        0x40, 0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x47, 0x48, 0x49, 0x4A, 0x4B, 0x4C, 0x4D, 0x4E, 0x4F,        0x50, 0x51, 0x52, 0x53, 0x54, 0x55, 0x56, 0x57, 0x58, 0x59, 0x5A, 0x5B, 0x5C, 0x5D, 0x5E, 0x5F,
        0x60, 0x61, 0x62, 0x63, 0x64, 0x65, 0x66, 0x67, 0x68, 0x69, 0x6A, 0x6B, 0x6C, 0x6D, 0x6E, 0x6F,        0x70, 0x71, 0x72, 0x73, 0x74, 0x75, 0x76, 0x77, 0x78, 0x79, 0x7A, 0x7B, 0x7C, 0x7D, 0x7E, 0x7F},
    { 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10},
    { 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10},
    {128, 128, 0, 0, FALSE, FALSE},
};

IQS_DRE_NR_IQPARAM *IQS_MSNR_NR[IQS_MAX_MSNR_NR_NUM] = {NULL, &IQS_MSNR_NR1, &IQS_MSNR_NR2, &IQS_MSNR_NR3, &IQS_MSNR_NR4, &IQS_MSNR_NR5};

IQS_DRE_IQPARAM IQS_MSNR =
{
    //ProcRep, uiProcRepIdx,            bDgEn, bDgImgRef, DgMskInIdx
    0, { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, FALSE, FALSE, 0, { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}
};
