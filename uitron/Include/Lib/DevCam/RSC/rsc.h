/**
    Rolling Shutter Correction.

    @file       rsc.h
    @ingroup    mILibRSC
    @version    V1.00.001
    @note       RSC Lib - public header

    Copyright   Novatek Microelectronics Corp. 2015.  All rights reserved.
*/

#ifndef _RSC_H
#define _RSC_H

#include "Type.h"
#include "dce_lib.h"

/**
    @addtogroup
*/
//@{

//////////////////////////////////////////////////////////////////////////
//  Lib definition, do not modify!
//////////////////////////////////////////////////////////////////////////

#define DCE_2DLUT_MAX_TAP (65)
#define DCE_GDCLUT_NUM (65)
#define RSC_DIST_TBL_TAP  (33)

//////////////////////////////////////////////////////////////////////////
//  DCE header duplication
//////////////////////////////////////////////////////////////////////////

typedef enum
{
  RSC_DCE_DCSEL_GDC_ONLY              = 0,    ///< GDC/CAC only
  RSC_DCE_DCSEL_2DLUT_ONLY            = 1,    ///< 2D lut only
  RSC_DCE_DCSEL_GDC_2DLUT_DIFFSUM     = 2,    ///< GDC/CAC, 2D grid LUT difference addition
  RSC_DCE_DCSEL_GDC_2DLUT_CASCADE     = 3,    ///< 2D grid LUT cascade with GDC/CAC
  ENUM_DUMMY4WORD(RSC_DCE_DC_SEL)
} RSC_DCE_DC_SEL;

typedef enum
{
    RSC_STAB_DIM_SEPARATED = 0,  ///< set stability for each dimension separately
    RSC_STAB_DIM_TOGETHER = 1,   ///< set stability for all dimensions together
} STAB_DIM_CTRL;

typedef enum
{
    RSC_STAB_FULLY   = 0,  ///< fully control on EIS
    RSC_STAB_PARTIAL = 1,  ///< partial control on EIS
} STAB_CTRL_TYPE;

typedef struct
{
    INT32    iXCent;    ///< Horizontal center, unit : pix
    INT32    iYCent;    ///< Vertical center, uint : line
} RSC_DCE_IMG_CENT;

typedef struct
{
    UINT32    uiXDist;    ///< Horizontal distance ratio
    UINT32    uiYDist;    ///< Vertical distance ratio
} RSC_DCE_IMG_RAT;

typedef struct
{
    UINT32    uiNFact;    ///< Normalization factor
    UINT32    uiNBit;     ///< Normalization bit
} RSC_DCE_DIST_NORM;

typedef struct
{
    RSC_DCE_IMG_RAT ImgRat;         ///< GDC/CAC image ratio setting
    RSC_DCE_DIST_NORM DistNorm;     ///< GDC/CAC distance normalization
} RSC_DCE_GDC_CAC_DZOOM_PARAM;

typedef enum
{
    RSC_DCE_CAC_RGB_GAIN = 0,         ///< Correct CA with G LUT and R, G, B LUTGAIN
    RSC_DCE_CAC_RGB_LUT  = 1,         ///< Correct CA with R, G, B LUT
    ENUM_DUMMY4WORD(RSC_DCE_CAC_SEL)
} RSC_DCE_CAC_SEL;

typedef struct
{
    UINT32    uiRLutGain;    ///< R channel lut gain
    UINT32    uiGLutGain;    ///< G channel lut gain
    UINT32    uiBLutGain;    ///< B channel lut gain
} RSC_DCE_CAC_GAIN;

typedef struct
{
    RSC_DCE_CAC_SEL CacSel;         ///< CAC compensation method selection
    RSC_DCE_CAC_GAIN CacGain;       ///< CAC RGB lut gain
    UINT32 *puiGGeoLut;         ///< GDC/CAC G geo lut
    INT32 *piRGeoLut;           ///< GDC/CAC R geo lut
    INT32 *piBGeoLut;           ///< GDC/CAC B geo lut
} RSC_DCE_GDC_CAC_OZOOM_PARAM;

typedef enum
{
    RSC_DCE_FOV_BOUND_DUPLICATE = 0,         ///< Replace out of boundary pixels with duplicate boundary pixels
    RSC_DCE_FOV_BOUND_REG_RGB   = 1,         ///< Replace out of boundary pixels with GEO_BOUNDR, GEO_BOUNDG, GEO_BOUNDB
    ENUM_DUMMY4WORD(RSC_DCE_FOV_SEL)
} RSC_DCE_FOV_SEL;

typedef struct
{
    UINT32    uiRBound;    ///< R boundary value
    UINT32    uiGBound;    ///< G boundary value
    UINT32    uiBBound;    ///< B boundary value
} RSC_DCE_FOV_RGB;

typedef struct
{
    UINT32 uiFovGain;           ///< FOV lut gain
    RSC_DCE_FOV_SEL FovSel;         ///< FOV boundary select
    RSC_DCE_FOV_RGB FovRGB;         ///< FOV boundary RGB definition
} RSC_DCE_FOV_PARAM;

typedef struct
{
    UINT32    uiXOfsInt;   ///< 2D lut x integer offset
    UINT32    uiXOfsFrac;  ///< 2D lut x fraction offset
    UINT32    uiYOfsInt;   ///< 2D lut y integer offset
    UINT32    uiYOfsFrac;  ///< 2D lut y fraction offset
} RSC_DCE_2DLUT_OFS;

typedef enum
{
    RSC_DCE_2DLUT_65_65_GRID = 0,         ///< 65x65 grid 2D lut distortion correction
    RSC_DCE_2DLUT_33_33_GRID = 1,         ///< 33x33 grid 2D lut distortion correction
    RSC_DCE_2DLUT_17_17_GRID = 2,         ///< 17x17 grid 2D lut distortion correction
    RSC_DCE_2DLUT_9_9_GRID   = 3,         ///< 9x9 grid 2D lut distortion correction
    ENUM_DUMMY4WORD(RSC_DCE_2DLUT_NUM)
} RSC_DCE_2DLUT_NUM;

typedef struct
{
    UINT32 ui2DLutAddr;                 ///< 2D lut DRAM address
    RSC_DCE_2DLUT_OFS TwoDLutXYOfs;         ///< 2D lut x y offset
    RSC_DCE_2DLUT_NUM TwoDLutNum;           ///< 2D lut grid number
} RSC_DCE_2DLUT_PARAM;


//////////////////////////////////////////////////////////////////////////
//  Definition
//////////////////////////////////////////////////////////////////////////

#define RSC_E_OK          E_OK    ///< Normal completion
#define RSC_E_SYS         E_SYS   ///< Normal completion
#define RSC_E_NOSPT       E_NOSPT ///< Feature not supported
#define RSC_E_PAR         E_PAR      ///< Parameter error

//////////////////////////////////////////////////////////////////////////
//  Enumeration
//////////////////////////////////////////////////////////////////////////

//Online-debug option
//----------------------------------
typedef enum
{
	//skip 0 to prevent your typo
	RSC_DBG_OPT_PRINT_2DLUT = 1,
	RSC_DBG_OPT_PRINT_FMA_TBL = 2,
	RSC_DBG_OPT_PRINT_3X3_MAT = 3,
	RSC_DBG_OPT_FMA_NEUTRAL = 4,
	RSC_DBG_OPT_FHI_NEUTRAL = 5,
	RSC_DBG_OPT_PRINT_GD = 6,
	RSC_DBG_OPT_PRINT_GD_OFST = 7,
	RSC_DBG_OPT_PRINT_SAT = 8,
	RSC_DBG_OPT_MOD_FOCAL_GAIN = 9,
	RSC_DBG_OPT_LIB_ON_OFF_TOGGLE = 10,
	RSC_DBG_OPT_DISABLE_GD_PREPROC = 11,
	RSC_DBG_OPT_DISABLE_Z_CORRECT = 12,
	RSC_DBG_OPT_CALG_UP = 13,
	RSC_DBG_OPT_CALG_DOWN = 14,
	RSC_DBG_OPT_GD_OFS_LOCK = 15,
	RSC_DBG_OPT_PRINT_AR = 16,
	RSC_DBG_OPT_PRINT_IDX = 17,
	RSC_DBG_OPT_PRINT_OPEN = 18,
	RSC_DBG_OPT_PRINT_CRP = 19,
	RSC_DBG_OPT_MA_DIST_OFF = 20,
	RSC_DBG_OPT_MA_UDIST_OFF = 21,
	RSC_DBG_OPT_AR_LOCK = 22,
	RSC_DBG_OPT_AT_EIS_ONLY = 23,
	RSC_DBG_OPT_DELAY_UP = 24,
	RSC_DBG_OPT_DELAY_DOWN = 25,
	RSC_DBG_PRINT_TIMING_PARAM = 26,
	RSC_DBG_OPT_PRINT_LT_TWO = 27,
	RSC_DBG_OPT_TAPE_ASSIGN = 28,
	RSC_DBG_OPT_TAPE_DUMP = 30,
	RSC_DBG_OPT_AT_LOCK_X = 31,
	RSC_DBG_OPT_AT_LOCK_Y = 32,
	RSC_DBG_OPT_AT_LOCK_Z = 33,
	RSC_DBG_OPT_LT_LOCK_LEFT = 34,
	RSC_DBG_OPT_MOD_FAR_DIST = 35,
    RSC_DBG_OPT_DISABLE_RSC_MARGIN = 36,
    RSC_DBG_OPT_MOD_NORM_GAIN = 37,
	RSC_DBG_OPT_ENABLE_NO_AR_MARGIN = 38,
    RSC_DBG_OPT_PRINT_FMR_TBL = 39,
    RSC_DBG_OPT_ENABLE_FULL_AR_MARGIN = 40,
    RSC_DBG_OPT_PRINT_GDC_LUT_R = 41,
    RSC_DBG_OPT_PRINT_DIST_RATE = 42,
    RSC_DBG_OPT_DISABLE_INT4_FMR = 43,
    RSC_DBG_OPT_MOD_ROTATE_ANGLE = 44,
    RSC_DBG_OPT_PRINT_UDIST_CAL = 45,
    RSC_DBG_OPT_MOD_RSC_CRP_CENT_SHFT_X = 46,
    RSC_DBG_OPT_MOD_RSC_CRP_CENT_SHFT_Y = 47,
    RSC_DBG_OPT_PRINT_MR_MSG = 48,
    RSC_DBG_OPT_MOD_GD_ST_TIME = 49,
	RSC_DBG_OPT_MOD_GD_ED_TIME = 50,
	RSC_DBG_OPT_PRINT_MR_STATUS = 51,
	RSC_DBG_OPT_MOD_AR_MARGIN = 52,
    //...
	RSC_DBG_OPT_MOD_RSC_CRP_ST_LINE_TIME = 55,
	RSC_DBG_OPT_FIX_C2C_RATIO_TO_1 = 56,
	RSC_DBG_OPT_PRINT_FAT = 57,
	RSC_DBG_OPT_PRINT_FMR_4CORNER = 58,
	RSC_DBG_OPT_PRINT_EVERY_GD = 59,
    //...
	RSC_DBG_OPT_PRINT_PRC_FRM = 63,
    ENUM_DUMMY4WORD(RSC_DBG_OPT)
} RSC_DBG_OPT;
//----------------------------------

typedef enum
{
    RSC_FUNC_RSC_EIS=0,     ///< RSC on,  EIS on
    RSC_FUNC_RSC_ONLY,      ///< RSC on,  EIS off
    RSC_FUNC_EIS_ONLY,      ///< RSC off, EIS on
    RSC_FUNC_NULL,          ///< RSC off, EIS off
    RSC_FUNC_MAX_CNT_DEF,
    ENUM_DUMMY4WORD(RSC_FUNC_SELECTION)
} RSC_FUNC_SELECTION;

typedef enum
{
    RSC_MODE_VIDEO=0,       ///< Video
    RSC_MODE_STILL,       ///< Still (for still-capture)
    RSC_MODE_MAX_CNT_DEF,
    ENUM_DUMMY4WORD(RSC_MODE_SELECTION)
} RSC_MODE_SELECTION;

typedef enum
{
    RSC_GYRO_DATA_INT32,    ///< as signed 32 bits integer
    RSC_GYRO_DATA_INT16,    ///< as signed 16 bits integer
    ENUM_DUMMY4WORD(RSC_GYRO_DATA_FORMAT)
} RSC_GYRO_DATA_FORMAT;

typedef enum
{
    RSC_GYRO_AXIS_X=0,    ///< Yaw,   as view horizontal motion, with head-toe axis
    RSC_GYRO_AXIS_Y,      ///< Pitch, as view vertical   motion, with ear-ear axis
    RSC_GYRO_AXIS_Z,      ///< Roll,  as view rotation         , with front-back axis
    RSC_GYRO_AXIS_MAX_CNT_DEF,
    ENUM_DUMMY4WORD(RSC_GYRO_AXIS)
} RSC_GYRO_AXIS;

typedef enum
{
    RSC_DIR_POS=0,      ///< positive direction, X: right, Y: up  , Z: clockwise
    RSC_DIR_NEG=1,      ///< negative direction, X: left , Y: down, Z: counter-clockwise
    ENUM_DUMMY4WORD(RSC_GYRO_DIR)
} RSC_GYRO_DIR;

typedef enum
{
    RSC_BEHAVIOR_NORMAL,    ///< Apply normal function
    RSC_BEHAVIOR_ABORT,     ///< Abort function(for current frame)
    ENUM_DUMMY4WORD(RSC_BEHAVIOR_SELECTION)
} RSC_BEHAVIOR_SELECTION;

typedef enum
{
    RSC_INFO_PAR_IPP_INCRP,         ///< parameter - IPP Dram Input Crop        , please refer to "RSC_IPP_INCROP_PARAM"
    RSC_INFO_PAR_DCE_2DLUT,         ///< parameter - DCE 2D-LUT                 , please refer to "RSC_DCE_2D_LUT_PARAM"
    RSC_INFO_PAR_DCE_CENT,          ///< parameter - DCE center setting         , please refer to "RSC_DCE_GDCCNTR_PARAM"

    RSC_INFO_SET_CONFIG,            ///< setting - RSC_CONFIG_SETTING           , please refer to "RSC_CONFIG_SETTING"
    RSC_INFO_SET_GYRO,              ///< setting - RSC_GYRO_SETTING             , please refer to "RSC_GYRO_SETTING"
    RSC_INFO_SET_LENS,              ///< setting - RSC_LENS_SETTING             , please refer to "RSC_LENS_SETTING"
    RSC_INFO_SET_SENSOR_DIMENSION,  ///< setting - RSC_SENSOR_DIMENSION_SETTING , please refer to "RSC_SENSOR_DIMENSION_SETTING"
    RSC_INFO_SET_SENSOR_TIMING,     ///< setting - RSC_SENSOR_TIMING_SETTING    , please refer to "RSC_SENSOR_TIMING_SETTING"
    RSC_INFO_SET_SENSOR_EXPOSIRE,   ///< setting - RSC_SENSOR_EXPOSURE_SETTING  , please refer to "RSC_SENSOR_EXPOSURE_SETTING"
    RSC_INFO_SET_IPP_SIZE,          ///< setting - RSC_IPP_SIZE_SETTING         , please refer to "RSC_IPP_SIZE_SETTING"
    RSC_INFO_SET_IPP_DCE,           ///< setting - RSC_IPP_DCE_SETTING          , please refer to "RSC_IPP_DCE_SETTING"

    RSC_INFO_SET_DBG,                ///< setting - RSC_DBG_SETTING              , please refer to "RSC_DBG_SETTING"

    ENUM_DUMMY4WORD(RSC_INFO_SELECTION)
} RSC_INFO_SELECTION;

typedef enum
{
    RSC_CHG_SET_CONFIG,            ///< change - RSC_CONFIG_SETTING           , please refer to "RSC_CONFIG_SETTING"
    RSC_CHG_SET_GYRO,              ///< change - RSC_GYRO_SETTING             , please refer to "RSC_GYRO_SETTING"
    RSC_CHG_SET_LENS,              ///< change - RSC_LENS_SETTING             , please refer to "RSC_LENS_SETTING"
    RSC_CHG_SET_LENS_DISTORT,      ///< change - RSC_LENS_SETTING_DISTORT     , please refer to "RSC_LENS_DISTORT_SETTING"
    RSC_CHG_SET_SENSOR_DIMENSION,  ///< change - RSC_SENSOR_DIMENSION_SETTING , please refer to "RSC_SENSOR_DIMENSION_SETTING"
    RSC_CHG_SET_SENSOR_TIMING,     ///< change - RSC_SENSOR_TIMING_SETTING    , please refer to "RSC_SENSOR_TIMING_SETTING"
    RSC_CHG_SET_SENSOR_EXPOSIRE,   ///< change - RSC_SENSOR_EXPOSURE_SETTING  , please refer to "RSC_SENSOR_EXPOSURE_SETTING"
    RSC_CHG_SET_IPP_SIZE,          ///< change - RSC_IPP_SIZE_SETTING         , please refer to "RSC_IPP_SIZE_SETTING"
    RSC_CHG_SET_IPP_DCE,           ///< change - RSC_IPP_DCE_SETTING          , please refer to "RSC_IPP_DCE_SETTING"
    RSC_CHG_SET_FLIP,              ///< change - RSC_FLIP_PARAM               , please refer to "RSC_FLIP_PARAM"
    RSC_CHG_SET_AXIS_SWITCH,       ///< change - RSC_AXIS_SWITCH              , please refer to "RSC_AXIS_SWITCH"

    RSC_CHG_SET_DBG,               ///< change - RSC_DBG_SETTING              , please refer to "RSC_DBG_SETTING"
    ENUM_DUMMY4WORD(RSC_CHG_SELECTION)
} RSC_CHG_SELECTION;

typedef enum
{
    RSC_FLIP_SIE,
    RSC_FLIP_RDE
} RSC_FLIP_ENG;

//////////////////////////////////////////////////////////////////////////
//  Structure - Setting
//////////////////////////////////////////////////////////////////////////

typedef struct
{
    UINT32 uiIppInMgnH;             ///< the IPP input    crop (as EIS) horizontal margin-ratio;  in %(ratio=(total-crop-out)/total); shot-changing; shot-related
    UINT32 uiIppInMgnV;             ///< the IPP input    crop (as EIS) vertical   margin-ratio;  in %(ratio=(total-crop-out)/total); shot-changing; shot-related
    UINT32 uiDceCpMgnH;             ///< the DCE internal crop (as RSC) horizontal margin-ratio;  in %(ratio=(total-crop-out)/total); shot-changing; shot-related
    UINT32 uiDceCpMgnV;             ///< the DCE internal crop (as RSC) vertical   margin-ratio;  in %(ratio=(total-crop-out)/total); shot-changing; shot-related
    UINT32 uiStability;             ///< the magnitude of image-stability, range=0~100; higher value, more stable; 50 for default; frame-changing; shot-related
    UINT32 uiGdPerFrm;              ///< the number of gyro-data in a frame; recommend to be 1024/frame-rate; shot-changing; shot-related
    RSC_FUNC_SELECTION FuncSelect;  ///< the selection of RSC-Lib function; shot-changing; shot-related
    RSC_MODE_SELECTION ModeSelect;  ///< the selection of Camera operation mode; shot-changing; shot-related
    UINT32 ui2dLutSz;               ///< [invalid, no need to configure] DCE 2D LUT Size (H=V); selection = 65/33/17/9; frame-changing; shot-related
} RSC_CONFIG_SETTING;

typedef struct
{
    FLOAT fSmallAng[3];             ///< small angle definition(degree), fill 0.0 to switch to old method
    FLOAT fLargeAng[3];             ///< large angle definition(degree), fill 0.0 to switch to old method
    UINT32 uiSmallStab[3];          ///< stability parameter for small angle 0~100
    UINT32 uiLargeStab[3];          ///< stability parameter for large angle 0~100
    STAB_DIM_CTRL StabDimCtrl;      ///< stability dim control, separated or together, note that when set to RSC_STAB_DIM_TOGETHER, only RSC_CONFIG_SETTING.uiStability is effective
    STAB_CTRL_TYPE StabType;        ///< stability type, fully control or partial control
} RSC_STAB_TUNING;

typedef struct
{
    RSC_GYRO_DATA_FORMAT    gdFormat;       ///< Gyro data format; project-changing; project-related
    RSC_GYRO_AXIS           AxisSelec[3];   ///< Gyro data axis; project-changing; project-related
    RSC_GYRO_DIR            DirSelec[3];    ///< Gyro data direction; project-changing; project-related
    UINT32 uiDataUnit;                      ///< Full range of angular velocity(e.g. 500 = -250~250 deg/sec)
    //removed//UINT32 uiCalGain[3];                    ///< calibration gain, in 1/1024, recommended fixed to 1024
    //removed//INT32  iCalOfs[3];                      ///< Calibration offset, recommended fixed to 0
//no discussed yet//    INT32  iCalDelay;
} RSC_GYRO_SETTING;

typedef struct
{
    UINT32 uiFocalLength;       ///< Focal length in pixel
} RSC_LENS_SETTING;

typedef struct
{
    UINT16 *puiDistortTbl;    ///< Distortion table pointer of UINT16[32] with RSC_DIST_TBL_FLOAT_PREC=4096; project-changing; project-related
    UINT32 uiDistortNorm;     ///< Distortion Table normalization factor  with DIST_TBL_1TAP_NORM=16;   project-changing; project-related
    UINT16 *puiUndistoTbl;    ///< Undistortion table pointer of UINT16[32] with RSC_DIST_TBL_FLOAT_PREC=4096; project-changing; project-related; null for auto-calculation
    UINT32 uiUndistoNorm;     ///< Undistortion Table normalization factor  with DIST_TBL_1TAP_NORM=16;   project-changing; project-related;
} RSC_LENS_DISTORT_SETTING;

typedef struct
{
    UINT16 uiDistortTbl[RSC_DIST_TBL_TAP];    ///< Distortion table; project-changing; project-related
    UINT16 uiUndistoTbl[RSC_DIST_TBL_TAP];    ///< Distortion table; project-changing; project-related
} RSC_LENS_DISTORT_SETTING_TBL;

typedef struct
{
    UINT32 uiSnrDmH;        ///< The horizontal size of sensor-active-pixel-region, in um; shot-changing; project-related
    UINT32 uiSnrDmV;        ///< The vertical size of sensor-active-pixel-region, in um; shot-changing; project-related
} RSC_SENSOR_DIMENSION_SETTING;

typedef struct
{
    UINT32 uiFrmTimeUs; 	///< Frame (base)     time, in us; shot-changing;  shot-related;  ex: 16666 for 60fps
    UINT32 uiFrmExtTUs; 	///< Frame externsion time, in us; frame-changing; frame-related; ex:  3334 for 60fps-->50fps
    UINT32 uiStartLnUs; 	///< the time 1st line of active-pixel-region image is transferd, in us; shot-changing; shot-related
    UINT32 uiEndLnUs;   	///< the time last line of active-pixel-region image is transferd, in us; shot-changing; shot-related
    UINT32 uiTransDlyUs;	///< the time between the-end-of-exposure and transfering, in us; zero for unknown; shot-changing; shot-related
    UINT32 uiSnrOutSzH; 	///< The Sensor output horizontal size, active-pixel-region; shot-changing; shot-related
    UINT32 uiSnrOutSzV; 	///< The Sensor output vertical size, active-pixel-region; shot-changing; shot-related
    UINT32 uiSnrModeMaxSzH;	///< The horizontal size of the maximal sensor mode (the image size being used to calculate distortion table)
    UINT32 uiSnrModeMaxSzV;	///< The vertical size of the maximal sensor mode (the image size being used to calculate distortion table)
    UINT32 uiSnrBinnRatioH; ///< The horizontal size of sensor crop window
    UINT32 uiSnrBinnRatioV; ///< The vertical size of sensor crop window
    UINT32 uiSnrCrpRatioH; 	///< The horizontal size of sensor crop window
    UINT32 uiSnrCrpRatioV; 	///< The vertical size of sensor crop window
} RSC_SENSOR_TIMING_SETTING;

typedef struct
{
    UINT32 uiExpTimeUs; ///< The exposure time, in us; frame-changing; frame(AE)-related
} RSC_SENSOR_EXPOSURE_SETTING;

typedef struct
{
    UINT32 uiSieCrpSzH; ///< SIE crop horizontal size; frame-changing; frame(Dzoom)-related
    UINT32 uiSieCrpSzV; ///< SIE crop vertical size; frame-changing; frame(Dzoom)-related
    UINT32 uiSieOutSzH; ///< SIE output horizontal size; after scaling if any; frame-changing; frame(Dzoom)-related
    UINT32 uiSieOutSzV; ///< SIE output vertical size; after scaling if any; frame-changing; frame(Dzoom)-related
    UINT32 uiIppInSzH;  ///< IPP input(crop) horizontal size; frame-changing; frame(Dzoom)-related
    UINT32 uiIppInSzV;  ///< IPP input(crop) vertical   size; frame-changing; frame(Dzoom)-related
    UINT32 uiDceOutSzH; ///< DCE output horizontal size; frame-changing; frame(Dzoom)-related
    UINT32 uiDceOutSzV; ///< DCE output vertical   size; frame-changing; frame(Dzoom)-related
} RSC_IPP_SIZE_SETTING;


typedef struct
{
    RSC_DCE_DC_SEL DcSel;                       ///< Distortion correction combination selection; frame-changing; frame(Ozoom)-related
    RSC_DCE_IMG_CENT ImgCent;                   ///< GDC/CAC center setting; frame-changing; frame(Ozoom)-related
    RSC_DCE_GDC_CAC_DZOOM_PARAM GdcCacDzParam;  ///< GDC/CAC digital zoom parameters; frame-changing; frame(Ozoom)-related
    RSC_DCE_GDC_CAC_OZOOM_PARAM GdcCacOzParam;  ///< GDC/CAC optical zoom parameters; frame-changing; frame(Ozoom)-related
    RSC_DCE_FOV_PARAM FovParam;                 ///< FOV parameters; frame-changing; frame(Ozoom)-related
} RSC_IPP_DCE_SETTING;

typedef struct
{
//RSC_20150909_JJ: DCE REVIEW
    UINT32  uiGGeoLut[DCE_GDCLUT_NUM];  ///< remove related process, not effective now
    INT32   iRGeoLut[DCE_GDCLUT_NUM];   ///< remove related process, not effective now
    INT32   iBGeoLut[DCE_GDCLUT_NUM];   ///< remove related process, not effective now
//~RSC_20150909_JJ: DCE REVIEW
} RSC_IPP_DCE_SETTING_TBL;


typedef struct
{
    BOOL   Enable;
    UINT32 uiOption;
    UINT32 uiStageOpt;
    INT32  iVal;
    UINT32 uiData[2];
    UINT32 uiDce2DLutBuffAddr;
    UINT32 uiDce2DLutBuffSize;
} RSC_DBG_SETTING;

typedef struct
{
    INT32 iCalGain;     ///< the over-all magnitude gain for calibration; sample-changing; project-related
    INT32 iCalDelay;    ///< the over-all delay for calibration; frame-changing; shot-related
} RSC_CAL_SETTING;

//////////////////////////////////////////////////////////////////////////
//  Structure - Command (
//////////////////////////////////////////////////////////////////////////

typedef struct
{
    BOOL  bAbortGd;     ///< abort gyro-data which was/were set but not processed
    UINT32 uiNum;       ///< gyro-data number, max=32
    INT32 iGD[32][3];   ///< gyro-data, start from[0][X/Y/Z]
} RSC_PRC_SMP_COMMAND;

typedef struct
{
    UINT32                  uiFrmId;    ///< frame index
} RSC_PRC_FRM_COMMAND;

//////////////////////////////////////////////////////////////////////////
//  Structure - Parameters (Output)
//////////////////////////////////////////////////////////////////////////



typedef struct
{
    BOOL  bValid;   ///< weither if this configuration is valid/effective or not.
    INT16 iCropBiasX; ///< IPP input crop X position bias
    INT16 iCropBiasY; ///< IPP input crop Y position bias
} RSC_IPP_INCROP_PARAM;

typedef struct
{
    BOOL  bValid;                   ///< weither if this configuration is valid/effective or not.
    RSC_DCE_2DLUT_PARAM Dce2dLutParam;  ///< DCE 2D LUT Parameter
} RSC_DCE_2D_LUT_PARAM;

typedef struct
{
    UINT32  ui2DLut[DCE_2DLUT_MAX_TAP*DCE_2DLUT_MAX_TAP];
} RSC_DCE_2D_LUT_PARAM_TBL;

typedef struct
{
    BOOL  bValid;               ///< weither if this configuration is valid/effective or not.
    INT16 iCntrBiasX; ///< DCE GDC Center X position bias
    INT16 iCntrBiasY; ///< DCE GDC Center Y position bias
} RSC_DCE_GDCCNTR_PARAM;

typedef struct
{
    UINT32 uiFrmCnt;    ///< frame count, since the 1st one after power-on
    UINT32 uiGdCnt;     ///< gyro data count, in the current frame
} RSC_PROCESS_STATUS;

typedef struct
{
    BOOL bHflipEn;              ///< Horizontal flip enable
    BOOL bVflipEn;              ///< Vertical flip enable
} RSC_FLIP_PARAM;

typedef struct
{
    BOOL bCompXEn;              ///< Image X axis compensation enable
    BOOL bCompYEn;              ///< Image Y axis compensation enable
    BOOL bCompZEn;              ///< Image Z axis compensation enable
} RSC_AXIS_SWITCH;

//////////////////////////////////////////////////////////////////////////
//  Structure - Open
//////////////////////////////////////////////////////////////////////////

typedef struct _RSC_OPENOBJ{
    RSC_CONFIG_SETTING              ConfigSet;  ///< input-setting of configuration
    RSC_GYRO_SETTING                GyroSet;    ///< input-setting of gyro
    RSC_LENS_SETTING                LensSet;    ///< input-setting of lens
    RSC_LENS_DISTORT_SETTING        LensDtSet;  ///< input-setting of lens-distortion
    RSC_SENSOR_DIMENSION_SETTING    SnrDmSet;   ///< input-setting of sensor dimension
    RSC_SENSOR_TIMING_SETTING       SnrTmSet;   ///< input-setting of sensor timing
    RSC_SENSOR_EXPOSURE_SETTING     SnrExpSet;  ///< input-setting of sensor exposure-time
    RSC_IPP_SIZE_SETTING            IppSizeSet; ///< input-setting of IPP-Size
    RSC_IPP_DCE_SETTING             IppDceSet;  ///< input-setting of IPP-DCE
    RSC_CAL_SETTING                 CalibSet;   ///< input-setting of calibration
    RSC_STAB_TUNING                 StabSet;    ///< stability configuration
    RSC_FLIP_PARAM                  FlipSet;    ///< image H,V flip configuration
    RSC_AXIS_SWITCH                 AxisSet;    ///< switch of image axis X,Y,Z compensation
} RSC_OPENOBJ;


//////////////////////////////////////////////////////////////////////////
//  Function
//////////////////////////////////////////////////////////////////////////


extern ER rsc_open(RSC_OPENOBJ *popenObj);
extern ER rsc_close(void);

extern ER rsc_chgSet(RSC_CHG_SELECTION ChgSelect, void *pObj);
extern ER rsc_prcSmp(RSC_PRC_SMP_COMMAND *pPrcSmpCmd);
extern ER rsc_prcFrm(RSC_PRC_FRM_COMMAND *pPrcFrmCmd);

extern ER rsc_getInfo(RSC_INFO_SELECTION InfoSelect, void *pObj);
extern void rsc_showDbgEnStatus(INT32 Idx);

//////////////////////////////////////////////////////////////////////////
//  backward compatible
//////////////////////////////////////////////////////////////////////////

typedef struct
{
    UINT32 uiNum;       ///< gyro-data number, max=32
    INT32 piGD[32][3];   ///< gyro-data, start from[0][X/Y/Z]
} RSC_GYRO_DATA;
typedef struct
{
    RSC_BEHAVIOR_SELECTION  bhSelect;   ///< Abort current frame
    BOOL                    bJumpFrm;   ///< 0: process secussive frames in order; 1: process selected frames in order
    UINT32                  uiFrmId;    ///< frame index, only valid when bJumpFrm==1
} RSC_ACTION_SETTING;

extern ER rsc_setGyroData(RSC_GYRO_DATA *pGyroData);
extern ER rsc_setAction(RSC_ACTION_SETTING *pActionSet);
extern ER rsc_chgConfigSet(RSC_CONFIG_SETTING *pConfigSet);
extern ER rsc_chgGyroSet(RSC_GYRO_SETTING *pGyroSet);
extern ER rsc_chgLensSet(RSC_LENS_SETTING *pLensSet);
extern ER rsc_chgLensDtSet(RSC_LENS_DISTORT_SETTING *pLensDtSet);
extern ER rsc_chgSensorDimensionSet(RSC_SENSOR_DIMENSION_SETTING *pSnrDmSet);
extern ER rsc_chgSensorTimingSet(RSC_SENSOR_TIMING_SETTING *pSnrTmSet);
extern ER rsc_chgSensorExposureSet(RSC_SENSOR_EXPOSURE_SETTING *pSnrExpSet);
extern ER rsc_chgIppSizeSet(RSC_IPP_SIZE_SETTING *pIppSizeSet);
extern ER rsc_chgIppDceSet(RSC_IPP_DCE_SETTING *pIppDceSet);
extern ER rsc_chgAllSet(RSC_OPENOBJ *popenObj);
extern ER rsc_chgFlipSet(RSC_FLIP_PARAM *pFlipSet);
extern ER rsc_chgAxisSet(RSC_AXIS_SWITCH *pAxisSet);
extern ER rsc_GenerateNeutralOutParam( RSC_IPP_INCROP_PARAM *pIppInCrpParam,
                                RSC_DCE_GDCCNTR_PARAM *pDceGdCntrParam,
                                RSC_DCE_2D_LUT_PARAM *pDce2dLutParam,
                                UINT32 uiSieOutSzH, UINT32 uiSieOutSzV,
                                UINT32 uiIppInSzH,  UINT32 uiIppInSzV,
                                UINT32 uiDceOutSzH, UINT32 uiDceOutSzV,
                                UINT32 uiDceCpMgnH, UINT32 uiDceCpMgnV,
                                UINT32 uiDistortRate, UINT32 uiDcPcnt);

//////////////////////////////////////////////////////////////////////////
//  debug
//////////////////////////////////////////////////////////////////////////
extern INT32 g_iRscFrmCntStamp;
extern BOOL g_bRscDump2DLutEn;
extern UINT32 g_uiRscRecFrmCnt;
extern UINT32 g_uiRscDbgSignal;
extern INT32 g_iRscDbgRotAngle[3];
extern INT32 g_iRscDbgFAT[3];
extern INT32 g_iRscDbgFAR[3];
extern INT32 g_iRscDbgGyro[3];

//@}


#endif//_RSC_H
