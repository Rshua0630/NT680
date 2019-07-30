/**
    Auto Exposure Algorithm.

    depend on current Luminance to calculate LV, ISO, Exposure Time, and Iris.

    @file       ae_alg.h
    @ingroup    mILibAEAlg
    @note       Nothing (or anything need to be mentioned).

    Copyright   Novatek Microelectronics Corp. 2011.  All rights reserved.
*/
#ifndef _AE_ALG_H_
#define _AE_ALG_H_

#include "ae_task.h"
#include "SensorHDRLib.h"
#include "sensor.h"

/**
     @name AE Alg. default value
     @note
*/
//@{
#define AE_WIN_X                    8                           ///< ae weighting window x
#define AE_WIN_Y                    8                           ///< ae weighting window y
#define AE_WIN_NUM                  (AE_WIN_X*AE_WIN_Y)

#define AEALG_TAB_END                   0xffffffff      ///< AE table end flag
#define AEALG_BUF_SIZE                  8
#define AEALG_PREDICT_POINT_MAX         3
#define AEALG_DYNAMIC_LV_NUM            21
#define AE_BLK_WIN_NUM                  6               //divide image to 6 block
#define AE_GEN_CURVE_NODE_MAX           40

#define AEALG_EV_ACCURACY_BASE             100
#define AEALG_LV_ACCURACY_BASE             100

#define AEALG_CURVEGEN_NODE_MAX			12

//@}

/**
     AEALG error code.

     @note
*/
typedef enum _AEALG_ER {
	AEALG_OK = 0,         ///< AE OK
	AEALG_PAR,            ///< AE Parameter error
	AEALG_MAX,
	ENUM_DUMMY4WORD(AEALG_ER)
} AEALG_ER;

/**
     AEALG Curve dump type.

     @note
*/
typedef enum _AE_CURVEDUMP_TYPE
{
    AE_CURVEDUMP_PRV = 0,
    AE_CURVEDUMP_CAP,
    AE_CURVEDUMP_BOTH,
    AE_CURVEDUMP_MAXCNT,
	ENUM_DUMMY4WORD(AE_CURVEDUMP_TYPE)
} AE_CURVEDUMP_TYPE;

/**
     AEALG Movie ISO mode type (For Movie mode use).

     @note
*/
typedef enum _AE_ISOMODE_MOV_TYPE
{
    AE_ISOMODE_MOV_PARAM = 0,
    AE_ISOMODE_MOV_UIMAX,
    AE_ISOMODE_MOV_MAXCNT,
	ENUM_DUMMY4WORD(AE_ISOMODE_MOV_TYPE)
} AE_ISOMODE_MOV_TYPE;

/**
     AEALG Photo ISO mode type (For Photo mode use).

     @note
*/
typedef enum _AE_ISOMODE_PHOTO_TYPE
{
    AE_ISOMODE_PHOTO_PARAM = 0,
    AE_ISOMODE_PHOTO_SPEED,
    AE_ISOMODE_PHOTO_UIMAX,
    AE_ISOMODE_PHOTO_MAXCNT,
	ENUM_DUMMY4WORD(AE_ISOMODE_PHOTO_TYPE)
} AE_ISOMODE_PHOTO_TYPE;

/**
     AEALG Get Info Selection Item.

     @note
*/
typedef enum _AE_SEL_ITEM
{
    AE_SEL_PRV_LV = 0,
    AE_SEL_PRV_EV,
    AE_SEL_CAP_LV,
    AE_SEL_CAP_EV,
    AE_SEL_CURR_Y,
    AE_SEL_CURR_EXPY,
    AE_SEL_EXPY,
    AE_SEL_CURR_RAW,
    AE_SEL_TARGET_RAW,
    AE_SEL_EXPOTIME,
    AE_SEL_ISOGAIN,
    AE_SEL_STABLE_CNT,
    AE_SEL_FNO,
    AE_SEL_CLAMP_RATIO,
    AE_SEL_MINFPS,
    AE_SEL_ALL_EN,
    AE_SEL_DBG_EXPT,
    AE_SEL_DBG_ISO,
    AE_SEL_LA_AVG_Y,
    AE_SEL_ADJ_STATUS,
    AE_SEL_OVEREXP_ADJ,
    AE_SEL_OVEREXP_CNT,
    AE_SEL_ISOMAX,
    AE_SEL_ISP_GAIN,
    AE_SEL_ISP_GAIN_THRES,
    AE_SEL_HDR_UART,
    AE_SEL_HDR_EV_OPT,
    AE_SEL_HDR_SE_EXPY,
    AE_SEL_HDR_SE_CLAMP_L,
    AE_SEL_HDR_SE_CLAMP_H,
    AE_SEL_HDR_SE_SMOOTH,
    AE_SEL_HDR_EXPT,
    AE_SEL_HDR_ISO,
    AE_SEL_HDR_SE_ISO_UB,
    AE_SEL_EV_COMP,
    AE_SEL_FLICKER_MODE,
    AE_SEL_SLOWSHOOT_CYC,
    AE_SEL_SLOWSHOOT_THR,
    AE_SEL_FULLRANGE_EV,
    AE_SEL_FULLRANGE_LV,
    AE_SEL_ISOMODE,
    AE_SEL_DCIRIS_EN,
    AE_SEL_MAXCNT,
	ENUM_DUMMY4WORD(AE_SEL_ITEM)
} AE_SEL_ITEM;

/**
     Iris index.

     big iris -> small iris
     AEALG_IRIS_0 -> AEALG_IRIS_1 -> ....... -> AEALG_IRIS_8
     @note
*/
typedef enum _AEALG_IRIS_IDX {
	AEALG_IRIS_BIG = 0,                 ///< iris index big
	AEALG_IRIS_0 = 0,                   ///< iris index 0
	AEALG_IRIS_1,                       ///< iris index 1
	AEALG_IRIS_2,                       ///< iris index 2
	AEALG_IRIS_3,                       ///< iris index 3
	AEALG_IRIS_4,                       ///< iris index 4
	AEALG_IRIS_5,                       ///< iris index 5
	AEALG_IRIS_6,                       ///< iris index 6
	AEALG_IRIS_7,                       ///< iris index 7
	AEALG_IRIS_8,                       ///< iris index 8
	AEALG_IRIS_SMALL = AEALG_IRIS_8,    ///< iris index small
	AEALG_IRIS_MAX,
	ENUM_DUMMY4WORD(AEALG_IRIS_IDX)
} AEALG_IRIS_IDX;


/**
     AE table information.

     AE table information.
     @note
*/
typedef enum _AEALG_TABLE_INFO {
	AEALG_EV = 0,          ///< EV value
	AEALG_EXP,             ///< Exposure time
	AEALG_ISO,             ///< ISO value
	AEALG_IRIS,            ///< IRIS value
	AEALG_ADJ_TARGET,      ///< adjust flag
	AEALG_INFO_MAX,
	ENUM_DUMMY4WORD(AEALG_TABLE_INFO)
} AEALG_TABLE_INFO;

/**
     @name Ae adjust flag(AEALG_ADJ_TARGET)
     @note
*/
//@{
#define AEALG_ADJ_EXP           0x01    ///< Adjust exposure time
#define AEALG_ADJ_GAIN          0x02    ///< Adjust iso
#define AEALG_ADJ_IRIS          0x04    ///< Adjust iris idx
//@}

/**
     AEALG converge speed.

     @note
*/
typedef enum _AEALG_SPEED {
	AEALG_SPEED_N3 = 0,         ///< speed -3
	AEALG_SPEED_N2,             ///< speed -2
	AEALG_SPEED_N1,             ///< speed -1
	AEALG_SPEED_NORMAL,         ///< normal
	AEALG_SPEED_P1,             ///< speed +1
	AEALG_SPEED_P2,             ///< speed +2
	AEALG_SPEED_P3,             ///< speed +3
	AEALG_SPEED_MAX,            ///< max number
	ENUM_DUMMY4WORD(AEALG_SPEED)
} AEALG_SPEED;

/**
     AE argument.

     AE argument.
     @note
*/
typedef struct _AEALG_ARG {
	UINT32 EVValue;         ///< evvalue
	UINT32 ExpoTime;        ///< Exposure Time(us)
	UINT32 ISOGain;         ///< ISO value(50, 100, 200......)
	UINT32 Iris;            ///< Iris Value(idx)
} AEALG_ARG;

/**
     AE luminance
     @note
*/
typedef struct _AE_LUMINANCE {
	UINT32 raw;     //raw luminance
	UINT32 y;       //yuv luminance
} AE_LUMINANCE;

/**
     AE boundary
     @note
*/
typedef struct _AE_BOUNDARY {
	UINT32 h;       //high
	UINT32 l;       //low
} AE_BOUNDARY;

/**
     AE gen node
     @note
*/
typedef struct _AE_GEN_NODE
{
    UINT32 expt;
    UINT32 iso;
	UINT32 iris_fno;
	UINT32 iris_fno_lin;
} AE_GEN_NODE;

/**
     AE extend fps
     @note
*/
typedef struct _AE_EXTEND_FPS {
    UINT32 sensor_fps;
    UINT32 extend_fps;
} AE_EXTEND_FPS;

/**
     AE histogram
     @note
*/
typedef struct _AE_HISTOGRAM {
	UINT32 bin_num; //total bin number
	UINT32 data_num; //total data number;
	UINT16 *y;      //yuv histogram pointer
} AE_HISTOGRAM;

/**
     AE weight item
     @note
*/
typedef struct _AE_WEIGHT {
	UINT32 ratio_bound;  //ratio boundary
	UINT32 exp_w;     //expect Y ratio
} AE_WEIGHT;

/**
     AE converage table
     @note
*/
typedef struct _AE_CONV_TAB {
	UINT32 thres_bound; // current average Y > thres_bound, using raw else yuv
	UINT32 weight_tab_num;
	AE_WEIGHT *weight_tab;
} AE_CONV_TAB;


typedef struct _AE_ENV_VALUE {
	UINT32 LV;         // lv
	UINT32 EVValue;    // ev value
} AE_ENV_VALUE;

typedef enum _FNO_SEL
{
    FNO_SEL_PREDEF=0,
    FNO_SEL_USER,
    FNO_SEL_MAX
} FNO_SEL;

typedef enum _FNO_IDX
{
    FNO_F10=0,
    FNO_F14,
    FNO_F18,
    FNO_F20,
    FNO_F24,
    FNO_F28,
    FNO_F30,
    FNO_F40,
    FNO_F56,
    FNO_F80,
    FNO_MAX
} FNO_IDX;

typedef enum {
	AE_IRIS_TYPE_FIXED,
	AE_IRIS_TYPE_DC,
	AE_IRIS_TYPE_P,
	AE_IRIS_TYPE_MAX,
} AEIrisType;

typedef enum {
	AE_IRIS_MODE_AUTO,      // AE Control
	AE_IRIS_MODE_CLOSE,     // Minimum Iris
	AE_IRIS_MODE_OPEN,      // Maximum Iris
	AE_IRIS_MODE_MANUAL,    // P-Iris only
	AE_IRIS_MODE_MAX,
} AEIrisMode;

typedef enum {
	AE_STRATEGY_HIGHLIGHT,
	AE_STRATEGY_LOWLIGHT,
	AE_STRATEGY_MAX,
	ENUM_DUMMY4WORD(AEStrategy)
} AEStrategy;

typedef enum {
    AE_SHDR_EV_OPT_FIXED = 0,
    AE_SHDR_EV_OPT_EXPY,
    AE_SHDR_EV_OPT_MAX,
    ENUM_DUMMY4WORD(AE_SHDR_EV_OPT)
} AE_SHDR_EV_OPT;

typedef enum {
    AE_UART_OFF = 0,
    AE_UART_ALL,
    AE_UART_SIMPLE,
    AE_UART_OVEREXP,
    AE_UART_LUM_INFO,
    AE_UART_HDR,
    AE_UART_IRIS,
    ENUM_DUMMY4WORD(AE_UART_OPT)
} AE_UART_OPT;

/**
     AE algorithm event.
     @note
*/

typedef UINT32(*GET_OVER_EXPO_RATIO)(UINT32 HistMaxNum, UINT16 *Hist);
typedef UINT32(*GET_FNO_RATIO)(UINT32 Id);
typedef UINT32(*GET_CURVE_OFS_RATIO)(void);
//typedef UINT32 (*USER_JUDGECONDITION)(AE_LUMINANCE *ExpectLum, AE_LUMINANCE *CurLum);
typedef struct _AEALG_EVENT_TAB {
	//AEAlg_getratio event
	GET_OVER_EXPO_RATIO GetOverExpoRatio;

	//AEAlg_Allot event(Fno x Fno x 10)
	GET_FNO_RATIO GetFnoRatio;

	//AEAlg_Allot event((FnoA^2 / FnoB^2) x 100)
	//FnoA = (Wide, big iris)
	//FnoB = (Current Section, big iris)
	GET_CURVE_OFS_RATIO GetCurveOfsRatio;
} AEALG_EVENT_TAB;

/**
     AE sub-settings
     @note
*/

typedef struct _AE_OVEREXP_INFO {
	AE_LUMINANCE AdjLum;                 ///< adjust luminace
	UINT32 CurCnt;
	UINT32 CurTotalStep;                 ///< current adjust total step
	UINT32 CurMaxStep;                   ///< current adjust Max step
	UINT32 CurMaxCnt;
	UINT32 CurMinCnt;
	UINT32 CurThreshold;
	UINT32 *AdjLumThresholdTab;          ///< over exposure luminance threshold table (LV0 ~ LV20, max index 21), AdjLum * x%
	UINT32 *AdjLumTab;                   ///< over exposure adjust luminance table (LV0 ~ LV20, max index 21, current expected Lum - adj lum)
	UINT32 *AdjMaxCntRatioTab;           ///< over exposure luminance counter table (LV0 ~ LV20, max index 21) (x / 1000)
	UINT32 *AdjMinCntRatioTab;           ///< over exposure luminance counter table (LV0 ~ LV20, max index 21) (x / 1000)
	UINT32 UserDefRatio;                 ///< if TRUE, CurCnt = GET_OVER_EXPO_RATIO()(user can set any value pass through callback function)
} AE_OVEREXP_INFO;

/**
     AE sub-settings
     @note
*/

typedef struct _AE_ADJHISTO_INFO {
	AE_LUMINANCE AdjLum;                 ///< (adjust luminance)
	AE_LUMINANCE Lum;
	UINT32 *RatioTab;
	UINT32 AvgL;
	UINT32 AvgLCnt;
	UINT32 AvgH;
	UINT32 AvgHCnt;
} AE_ADJHISTO_INFO;


typedef UINT32(*AEALG_ADD_GAMMA)(UINT32);
typedef UINT32(*AEALG_INV_GAMMA)(UINT32);
typedef struct _AE_EXT_SET {
	UINT32 *PrvLumRatioTab;                             ///< dynamic expected luminance table (LV0 ~ LV20, max index 21, expectLum x ratio / 100)
	AE_LUMINANCE DyLum;
	UINT32 ExpLumDiv;                                   ///< expect Luminance adjust div, DyLum/Div, for shdr using

	UINT32 AntiOverExpEn;                                  ///< anti overexposure switch
	AE_OVEREXP_INFO OverExp;                            ///< sub function for reducing over exposure condition

	UINT32 AdjHistoEn;                                  ///< adjust histogram switch
	AE_ADJHISTO_INFO AdjHistoInfo;                      ///< adjust histogram range parameter

	UINT32 AdjCapEn;
	AE_LUMINANCE CapExpectLum;                          ///< expected luminance
	UINT32 *CapExpectTab;

	UINT32 UnStableDelay;                               ///< reconvergence delay counter
} AE_EXT_SET;

/**
     AE Information.

     AE Information.
     @note
*/
typedef struct _AE_INFO {
	UINT32 Id;
	AE_LUMINANCE PrvExpectLum;                      ///< standard expected luminance
	AE_LUMINANCE PrvAvgLum;                         ///< standard average luminance
	AE_ENV_VALUE PrvAvgEV;                          ///< standard ev value for average luminance
	AE_BOUNDARY EV;                                 ///< max, min ev
	AE_BOUNDARY MovieEV;                                 ///< max, min ev
	AE_BOUNDARY PhotoEV;                                 ///< max, min ev
	AE_BOUNDARY ISO;                                ///< Preview max, min iso
	AE_BOUNDARY CapISO;                             ///< Capture max, min iso
	AE_BOUNDARY StableRange;                        ///< stable tolerance (100 +H -L) %
	AE_BOUNDARY StableRangeRF;                      ///< stable tolerance (100 +H -L) %
	AE_HISTOGRAM histogram;                         ///< histogram information
	UINT32(* PrvCurve)[AEALG_INFO_MAX];             ///< preview Ae curve
	UINT32(* CapCurve)[AEALG_INFO_MAX];             ///< capture Ae curve
	UINT32 FirstRunFlag;                            ///< ae first run flag
	UINT32 FirstStableFlag;
	UINT32 Reset;                                   ///< ae reset flag
	UINT32 Counter;                                 ///< ae stable counter
	UINT32 CounterRF;                               ///< ae stable counter
	AE_LUMINANCE CurExpectLum;                      ///< current expected luminance
	AE_LUMINANCE CurLum;                            ///< current luminance
	UINT32 CurConvDir;                              ///< preview converage direction
	AE_ENV_VALUE PrvParam[AEALG_PREDICT_POINT_MAX]; ///< preview parameter
	AE_ENV_VALUE CapParam;                          ///< capture parameter
	AEALG_EVENT_TAB EventTab;
	AEALG_SPEED ConvSpeed;                          ///< converage speed
	AE_CONV_TAB *ConvTab;                           ///< manual converge table
	AEALG_INV_GAMMA InvGammaFunc;                   ///< inverse gamma function
	AEALG_ADD_GAMMA AddGammaFunc;                   ///< additive gamma function
	UINT32 IsRawLum;
	UINT32 EVCompRatio;                             ///< ev comp ratio (raw * ratio /100)
	UINT32 EVCompIdx;                               ///< ev comp ratio index
	UINT32 PrvBiningMode;                           ///< bining mode 1x, 2x, 3x, 4x......(x 100)
	UINT32 CapBiningMode;                           ///< bining mode 1x, 2x, 3x, 4x......(x 100)
	AE_EXT_SET ExtSet;                              ///< extend parameter for AE control
	UINT32 DePulseEn;
    UINT32 DePulseFrm;
    UINT32 DePulseRatio;
	UINT32 SmoothRatioThr;
    UINT32 SlowShooting_Cyc;
    UINT32 SlowShooting_Thr;
    UINT32 LA_Avg_Y;
    UINT32 FPS;
    UINT32 MF_Num;
    UINT16 *LA_Data;
	UINT32 Rev[4];                                  ///< for debug do not access
} AE_INFO;
/**
     AE Block Y information

     @note
*/
typedef struct _AE_BLK_INFO {
	UINT32 BLKLum_En;                   ///Block Luminance calculate function switch
	UINT32 LV;                          ///current LV value
	UINT32 FirstRunFlg;                 ///< BLK first run flag
	UINT16 *Yacc;                       ///Y acc
	UINT16 *Gacc;                       ///G acc
	UINT32 LA_Num_X;                    ///LA window X number
	UINT32 LA_Num_Y;                    ///LA window Y number
	AE_BOUNDARY Raw_OverBound_Lum;      ///raw over  exposure luminance boundary (up/low bound)
	AE_BOUNDARY Raw_UnderBound_Lum;     ///raw under exposure luminance boundary (up/low bound)
	UINT32 PrvBlk_Num[AE_BLK_WIN_NUM];  ///previous luminance
	AEALG_ADD_GAMMA AddGammaFunc;       ///< additive gamma function
	UINT32 Rev[5];                      ///< for debug do not access
} AE_BLK_INFO;

typedef struct _AE_PARAMINFO {
	AE_BOUNDARY EV;                 ///< Maximum/Minimum EV
	AE_BOUNDARY PrvISO;             ///< Maximum/Minimum ISO for streaming mode
	AE_BOUNDARY CapISO;             ///< Maximum/Minimum ISO for capture mode
	AE_BOUNDARY AFD_Lum;            ///< Auto anti Flicker Parameter
	UINT32 ExpLum_Mov;              ///< @10b post-gamma Lum for Video/Recording mode
	UINT32 ExpLum_Photo;            ///< @10b post-gamma Lum for Photo mode
	UINT32 ExpLum_Histo;            ///< @10b post-gamma Lum for Histogram
	UINT32 ExpLum_SHDR;            ///< @12b pre-gamma Lum
	UINT32 ExpLum_OverExp;          ///< @10b post-gamma Lum for Anti over exposure
	AE_BOUNDARY Blk_OVER_Y_Thr;     ///< @10b post-gamma over exposure upbound/lowbound threshold
	AE_BOUNDARY Blk_UNDER_Y_Thr;    ///< @10b post-gamma under exposure upbound/lowbound threshold
	UINT32 BaseISO;                 ///< base iso value
	UINT32 BaseGainRatio;           ///< base gain ratio (min 1000 (1x))
} AE_PARAMINFO;

typedef struct {
	AEIrisType IrisType;
	AEIrisMode IrisMode;
	UINT32 DCIrisSpeed;
	UINT32 PIrisManualSize;     // P-Iris only
	AE_BOUNDARY PIris;          // P-Iris only
} AE_IRISINFO;

typedef struct _AE_EXT_SET_INFO {
	AE_BOUNDARY ExpTBound;            ///< exp time(us) boundary
	AE_BOUNDARY IsoBound;             ///< iso boundary
	UINT32 ExpT;
	UINT32 ISO;
	UINT32 ConvergenceSpeed;
	AE_IRISINFO IrisSetting;
	UINT32 AETolerance;
	UINT32 AEDelayCnt;
	UINT8 *ExpAreaWeight;
    AEStrategy AEStrategy; /* lowlight/highlight first */
} AE_EXT_SET_INFO;

typedef struct _AE_SHDR_INFO {
    UINT32 se_ev_ratio;
    AE_BOUNDARY range_w;
    UINT32 curr_se_lum;
} AE_SHDR_INFO;


/**

     AEAlg_allot function

     depend on EV value to calculate iso, exposure time, and iris.

     @note

     @param[in] EVValue    Current EVValue.
     @param[in] SenBinning Sensor binning ratio.
     @param[in] AECurve    AE Curve.
     @param[in] GET_FNO_RATIO callback function fp.
     @param[in] GET_CURVE_OFS_RATIO callback function fp.
     @param[out] Info    iso, exposure time, and iris.
     @return
         - @b AEALG_OK:   done with no error.
         - Others: Error occured.
*/
extern AEALG_ER AEAlg_Allot(UINT32 Id, UINT32 EVValue, UINT32 SenBinning, UINT32(*AeCurve)[AEALG_INFO_MAX], GET_FNO_RATIO GetFnoCB, GET_CURVE_OFS_RATIO GetCurveOfsCB, AEALG_ARG *Info);

/**

     converage luminance

     @note

     @param[in] AE_INFO(AEInfo)
     @param[in] UINT32
     @param[out] preview iso, exposure time, iris
     @param[out] capture iso, exposure time, iris
     @return
         - @b void.
*/
extern void AEAlg_Converge(AE_INFO *AEInfo, UINT32 PointNum, AEALG_ARG *PrvInfo, AEALG_ARG *CapInfo);

/**

     convert ev value to lv value

     @note

     @param[in] ev vaule
     @return
         - @b lv value(x10).
*/
extern UINT32 AEAlg_Conv2LV(UINT32 EVValue);

/**

     calculate block based luminance

     @note

     @param[in] AE_BLK_INFO(AeBlkInfo)
     @param[in] Y and Raw CA, CA window number, LV
     @return
         - @b raw and y luminance
*/
extern AE_LUMINANCE AEAlg_GetBLKBasedLum(AE_BLK_INFO *AeBlkInfo);
extern UINT32 AE_BlkWriteDebugInfo(UINT32 Addr);
/**

     mdsc cb for debug use

     @note

     @return
         - @b bool.
*/
extern BOOL AEAlg_MsdcCB(void);



typedef enum _AE_GEN_IDX {
	AE_GEN_IDX_MIN = 0,
	AE_GEN_IDX_MAX,
	AE_GEN_IDX_NUM,
	ENUM_DUMMY4WORD(AE_GEN_IDX)
} AE_GEN_IDX;

typedef enum _AE_GEN_FREQ {
	AE_GEN_FREQ_NONE = 0,
	AE_GEN_FREQ_27_5,
	AE_GEN_FREQ_50,
	AE_GEN_FREQ_60,
	AE_GEN_FREQ_NUM,
	ENUM_DUMMY4WORD(AE_GEN_FREQ)
} AE_GEN_FREQ;

/**
     AE flicker type.

     @note
*/
typedef enum _AE_FLICKER {
	AE_FLICKER_50HZ = 0, ///< flicker 50Mz
	AE_FLICKER_60HZ,     ///< flicker 60Mz
	AE_FLICKER_MAX,
	ENUM_DUMMY4WORD(AE_FLICKER)
} AE_FLICKER;

typedef enum _AE_LUMGAMMA_TYPE
{
    AE_LUMGAMMA_IPP=0, // auto search 1/120 or 1/100 exposure time and ISO min
    AE_LUMGAMMA_PREDEF,
    ENUM_DUMMY4WORD(AE_LUMGAMMA_TYPE)
} AE_LUMGAMMA_TYPE;

typedef enum _AE_EV_INIT_MODE
{
    EV_INIT_AUTO=0, // auto search 1/120 or 1/100 exposure time and ISO min
    EV_INIT_MANUAL,
    ENUM_DUMMY4WORD(AE_EV_INIT_MODE)
} AE_EV_INIT_MODE;

typedef struct _AE_INIT {
    AE_EV_INIT_MODE mode;
    AE_GEN_NODE node;
} AE_INIT;

typedef enum _AE_FLICKER_MODE
{
    AE_FLICKER_ON = 0,
    AE_FLICKER_OFF,
    AE_FLICKER_FORCE,
    ENUM_DUMMY4WORD(AE_FLICKER_MODE)
} AE_FLICKER_MODE;

typedef struct _AE_CURVE_GEN_PHOTO
{
    AE_GEN_NODE node[AEALG_CURVEGEN_NODE_MAX];
    UINT32 node_num;
    UINT32 expt_max;
    UINT32 iso_max;
    AE_ISOMODE_PHOTO_TYPE iso_mode;
    UINT32 isp_gain_thres;
} AE_CURVE_GEN_PHOTO;

typedef struct _AE_CURVE_GEN_PARAM
{
    UINT32 fno_sel;
    UINT32 fno_rat;                          ///< fno * fno * 10
    UINT32 fno_idx;                          ///< fno Idx
    AE_FLICKER freq;
    AE_GEN_NODE node[AEALG_CURVEGEN_NODE_MAX];
    UINT32 node_num;
    UINT32 iso_max;
    UINT32 hdr_ratio[SEN_MFRAME_MAX_NUM];
    UINT32 auto_lowlight_en;
    AE_EXTEND_FPS ext_fps[5];
    AE_ISOMODE_MOV_TYPE iso_mode;
    UINT32 isp_gain_thres;
    AE_FLICKER_MODE flicker_mode;
} AE_CURVE_GEN_PARAM;

typedef struct _AE_GEN_INFO
{
    UINT32 fno_rat;                          ///< fno * fno * 10
    UINT32 fno_idx;                          ///< fno Idx
    AE_GEN_NODE node[AEALG_CURVEGEN_NODE_MAX];
	UINT32 node_num;
    UINT32 iso_max;
    AE_GEN_FREQ freq;                       ///< anti flicker frequence 50 or 60
    UINT32 frame_rate;
    UINT32 hdr_frame;
    UINT32 hdr_ratio[SEN_MFRAME_MAX_NUM];
    UINT32 hdr_exp_gap;
    UINT32 auto_lowlight_en;
    AE_EXTEND_FPS ext_fps[5];
    AE_ISOMODE_MOV_TYPE iso_mode;
    UINT32 iso_value;
    UINT32 scene_mode;
    UINT32 dbg_expt;
    UINT32 dbg_iso;
    AE_FLICKER_MODE flicker_mode;
    UINT32 iris_en;
    UINT32 iris_min_expt;
} AE_GEN_INFO;

typedef struct _AE_GEN_PHOTO
{
    UINT32 fno_rat;                          ///< fno * fno * 10
    UINT32 fno_idx;                          ///< fno Idx
    UINT32 iso_value;
    AE_GEN_FREQ freq;                       ///< anti flicker frequence 50 or 60
    UINT32 scene_mode;
    AE_GEN_NODE node[AEALG_CURVEGEN_NODE_MAX];
    UINT32 node_num;
    UINT32 expt_max;
    UINT32 iso_max;
    AE_ISOMODE_PHOTO_TYPE iso_mode;
    UINT32 iris_en;
    UINT32 iris_min_expt;
} AE_GEN_PHOTO;

typedef struct _AE_LUM_GAMMA {
    AE_LUMGAMMA_TYPE type;
    UINT32 gamma[129];
} AE_LUM_GAMMA;

typedef struct _AE_UI_CMD {
    UINT32 ev_comp;
    UINT32 reserve1;
    UINT32 reserve2;
    UINT32 reserve3;
    UINT32 reserve4;
    UINT32 reserve5;
} AE_UI_CMD;

typedef struct _AE_IRIS_PID {
    INT32 kp;
    INT32 ki;
    INT32 kd;
} AE_IRIS_PID;

typedef struct _AE_IRIS_CFG {
    UINT32 enable;
    UINT32 exptime_min;
    UINT32 probe_balance;
    INT32 balance_ratio;
    INT32 driving_ratio;
    AE_IRIS_PID iris_ctrl;
    UINT32 pwm_id;
    UINT32 drv_max;
    UINT32 freeze_time;
    UINT32 unfreeze_time;
} AE_IRIS_CFG;

typedef struct _AE_IRIS_INFO {
    UINT32 pwm_id;
    INT32 drv_ratio;
    INT32 drv_max;
    INT32 probe_balance;
    INT32 balance_ratio;
    INT32 driving_ratio;
    AE_IRIS_PID iris_ctrl;
    UINT32 conv_h, conv_l;
    UINT32 expo_time, iso_gain;
    UINT32 exposure_freeze;
    UINT32 exptime_min;
    UINT32 freeze_time;
    UINT32 unfreeze_time;
} AE_IRIS_INFO;

typedef struct _AE_SHDR_ExptTrans
{
    UINT32 shdr_frm;
    AE_SHDR_EV_OPT ev_opt;
    UINT32 expo_time[SEN_MFRAME_MAX_NUM];
    UINT32 iso_gain[SEN_MFRAME_MAX_NUM];
    UINT32 expo_time_max[SEN_MFRAME_MAX_NUM];
    SHDR_EV_RATIO_INFO ev_ratio;
    SHDR_EV_RATIO_INFO ev_ratio_new;
    UINT32 iso_min;
    UINT32 iso_max;
    UINT32 se_iso_ub;
} AE_SHDR_ExptTrans;

ER AEAlg_GenCurve(AE_GEN_INFO *Info, UINT32 *CurveAdr, UINT32 *MaxEV, UINT32 *MinEV, UINT32 *SHDR_ExpT_Max);
ER AEAlg_GenCurvePhoto(AE_GEN_PHOTO *Info, UINT32 *CurveAdr, UINT32 *MaxEV, UINT32 *MinEV);

/*AE Tuning tool *************************************************************/
#if 0
#endif
typedef enum _AE_SET_ITEM {
	AE_SET_EXPECTLUM        =   0,
	AE_SET_HISTOGRAMADJ,
	AE_SET_OVEREXP,
	AE_SET_BOUNDARY,
	AE_SET_CONVERGE,
	AE_SET_MANUAL,
	AE_SET_CURVEGEN,
	AE_SET_LACLAMP,
	AE_SET_WINDOW,
	AE_SET_LUMGAMMA,
	AE_SET_SHDR,
	AE_SET_UICMD,
	AE_SET_IRIS_CFG,
	AE_SET_CURVEGEN_PHOTO,
	AE_SET_ALL,
	AE_SET_MAX
} AE_SET_ITEM;

typedef enum _AE_GET_ITEM {
	AE_GET_EXPECTLUM        =   0,
	AE_GET_HISTOGRAMADJ,
	AE_GET_OVEREXP,
	AE_GET_BOUNDARY,
	AE_GET_CONVERGE,
	AE_GET_MANUAL,
	AE_GET_STATUS,
	AE_GET_CURVEGEN,
	AE_GET_LACLAMP,
	AE_GET_WINDOW,
	AE_GET_LADATA,
	AE_GET_LUMGAMMA,
	AE_GET_SHDR,
	AE_GET_UICMD,
	AE_GET_IRIS_CFG,
	AE_GET_CURVEGEN_PHOTO,
	AE_GET_ALL,
	AE_GET_MAX
} AE_GET_ITEM;

typedef enum _AE_TUNING_UPDATE {
	AE_UPDATE_EXPECTLUM     = FLGPTN_BIT(AE_SET_EXPECTLUM),
	AE_UPDATE_HISTOGRAMADJ  = FLGPTN_BIT(AE_SET_HISTOGRAMADJ),
	AE_UPDATE_OVEREXP       = FLGPTN_BIT(AE_SET_OVEREXP),
	AE_UPDATE_BOUNDARY      = FLGPTN_BIT(AE_SET_BOUNDARY),
	AE_UPDATE_CURVEGEN      = FLGPTN_BIT(AE_SET_CURVEGEN),
	AE_UPDATE_CONVERGE      = FLGPTN_BIT(AE_SET_CONVERGE),
	AE_UPDATE_MANUAL        = FLGPTN_BIT(AE_SET_MANUAL),
	AE_UPDATE_LACLAMP       = FLGPTN_BIT(AE_SET_LACLAMP),
	AE_UPDATE_WINDOW        = FLGPTN_BIT(AE_SET_WINDOW),
	AE_UPDATE_LUMGAMMA      = FLGPTN_BIT(AE_SET_LUMGAMMA),
	AE_UPDATE_SHDR          = FLGPTN_BIT(AE_SET_SHDR),
	AE_UPDATE_UICMD         = FLGPTN_BIT(AE_SET_UICMD),
	AE_UPDATE_IRIS_CFG      = FLGPTN_BIT(AE_SET_IRIS_CFG),
	AE_UPDATE_CURVEGEN_PHOTO = FLGPTN_BIT(AE_SET_CURVEGEN_PHOTO),
	AE_UPDATE_ALL           = FLGPTN_BIT_ALL
} AE_TUNING_UPDATE;

typedef enum _AE_FUNC_MODE {
	AE_FUNC_DISABLE  = 0,
	AE_FUNC_ENABLE,
	AE_FUNC_AUTO,
	AE_FUNC_MAX
} AE_FUNC_MODE;

typedef struct _AE_EXPECT_LUM
{
    UINT32 lum_mov;
    UINT32 lum_photo;
	UINT32 tab_ratio_mov[AEALG_DYNAMIC_LV_NUM];
    UINT32 tab_ratio_photo[AEALG_DYNAMIC_LV_NUM];
	UINT32 tab_ratio_ir[AEALG_DYNAMIC_LV_NUM];
} AE_EXPECT_LUM;

typedef struct _AE_LA_CLAMP
{
    UINT32 tab_normal_h[AEALG_DYNAMIC_LV_NUM];
    UINT32 tab_normal_l[AEALG_DYNAMIC_LV_NUM];
    UINT32 tab_shdr_h[AEALG_DYNAMIC_LV_NUM];
    UINT32 tab_shdr_l[AEALG_DYNAMIC_LV_NUM];
} AE_LA_CLAMP;

typedef struct _AE_HISTO {
	AE_FUNC_MODE mode;
	UINT32 lum;
	UINT32 tab_ratio[AEALG_DYNAMIC_LV_NUM];
} AE_HISTO;

typedef struct _AE_SHDR {
	UINT32 expy_le;
	UINT32 tab_ratio_le[AEALG_DYNAMIC_LV_NUM];
    AE_SHDR_EV_OPT ev_opt;
    UINT32 expy_se;
    UINT32 smooth;
    AE_BOUNDARY range_conv;
    AE_BOUNDARY clamp;
    AE_BOUNDARY range_ev;
	UINT32 tab_ratio_se[AEALG_DYNAMIC_LV_NUM];
    UINT32 se_iso_ub;
    UINT32 min_expt_se;
} AE_SHDR;

typedef struct _AE_OVER_EXPOSURE {
	UINT32 enable;
	UINT32 lum;
	UINT32 tab_ratio[AEALG_DYNAMIC_LV_NUM];
	UINT32 tab_thr_mov[AEALG_DYNAMIC_LV_NUM];
    UINT32 tab_thr_photo[AEALG_DYNAMIC_LV_NUM];
	UINT32 tab_thr_ir[AEALG_DYNAMIC_LV_NUM];
	UINT32 tab_maxcnt[AEALG_DYNAMIC_LV_NUM];
	UINT32 tab_mincnt[AEALG_DYNAMIC_LV_NUM];
} AE_OVER_EXPOSURE;

typedef struct _AE_CONVERGENCE {
	UINT32 speed;
	AE_BOUNDARY range_conv;
	UINT32 smooth_ratio_thr;
    UINT32 depulse_en;
    UINT32 depulse_ratio;
    UINT32 depulse_frm;
    UINT32 delay_cnt;
    UINT32 slowshoot_cyc;
    UINT32 slowshoot_thr;
    UINT32 reserved0;
    UINT32 reserved1;
} AE_CONVERGENCE;

typedef struct _AE_PROC_BOUNDARY {
	AE_BOUNDARY iso_prv;
    AE_BOUNDARY iso_cap;
} AE_PROC_BOUNDARY;

typedef struct _AE_STATUS_INFO {
    UINT32 lv;
	UINT32 ev;
	UINT32 lum;
    UINT32 hist_lum;
	UINT32 expotime;
	UINT32 iso_gain;
	UINT32 expect_lum;
    UINT32 overexp_adj;
    UINT32 fps;
    UINT32 state_adj;
    UINT32 overexp_cnt;
    UINT32 mf_num;
    UINT32 reserve3;
    UINT32 reserve4;
    UINT32 reserve5;
} AE_STATUS_INFO;

typedef struct _AE_TUNING_EXPECTLUM {
    UINT32 id;
    UINT32 size;
    AE_EXPECT_LUM expect_lum;
} AE_TUNING_EXPECTLUM;

typedef struct _AE_TUNING_HISTO {
    UINT32 id;
    UINT32 size;
	AE_HISTO histo;
} AE_TUNING_HISTO;

typedef struct _AE_TUNING_SHDR {
    UINT32 id;
    UINT32 size;
	AE_SHDR shdr;
} AE_TUNING_SHDR;

typedef struct _AE_TUNING_OVEREXP {
    UINT32 id;
    UINT32 size;
	AE_OVER_EXPOSURE over_expoure;
} AE_TUNING_OVEREXP;

typedef struct _AE_TUNING_BOUNDARY {
    UINT32 id;
    UINT32 size;
	AE_PROC_BOUNDARY proc_boundary;
} AE_TUNING_BOUNDARY;

typedef struct _AE_TUNING_CONVERGE {
    UINT32 id;
    UINT32 size;
	AE_CONVERGENCE converge;
} AE_TUNING_CONVERGE;

typedef struct _AE_TUNING_STATUS {
    UINT32 id;
    UINT32 size;
    AE_STATUS_INFO status_info;
} AE_TUNING_STATUS;

typedef struct _AE_TUNING_LA {
    UINT32 id;
    UINT32 size;
    UINT16 la_data[AE_WIN_NUM];
} AE_TUNING_LA;

typedef struct _AE_TUNING_MANUAL {
    UINT32 id;
    UINT32 size;
	UINT32 manualEn;
	UINT32 manualFixISOEn;
	UINT32 manualExpT;
	UINT32 manualISO;
} AE_TUNING_MANUAL;

typedef struct _AE_TUNING_CURVEGEN {
    UINT32 id;
    UINT32 size;
    AE_CURVE_GEN_PARAM curvegen;
} AE_TUNING_CURVEGEN;

typedef struct _AE_TUNING_LACLAMP {
    UINT32 id;
    UINT32 size;
    AE_LA_CLAMP la_clamp;
} AE_TUNING_LACLAMP;

typedef struct _AE_TUNING_WINDOW {
    UINT32 id;
    UINT32 size;
    UINT32 matrix[AE_WIN_NUM];
} AE_TUNING_WINDOW;

typedef struct _AE_TUNING_LUMGAMMA {
    UINT32 id;
    UINT32 size;
    AE_LUM_GAMMA lum_gamma;
} AE_TUNING_LUMGAMMA;

typedef struct _AE_TUNING_UICMD {
    UINT32 id;
    UINT32 size;
    AE_UI_CMD ui_cmd;
} AE_TUNING_UICMD;

typedef struct _AE_TUNING_IRIS {
    UINT32 id;
    UINT32 size;
    AE_IRIS_CFG iris_cfg;
} AE_TUNING_IRIS;

typedef struct _AE_TUNING_CURVEGEN_PHOTO {
    UINT32 id;
    UINT32 size;
    AE_CURVE_GEN_PHOTO curvegen_photo;
} AE_TUNING_CURVEGEN_PHOTO;

typedef struct _AE_TUNING_ALL {
    UINT32 id;
    UINT32 size;
	AE_TUNING_EXPECTLUM tExpectLum;
	AE_TUNING_OVEREXP   tOverExposure;
	AE_TUNING_HISTO     tHistogram;
	AE_TUNING_BOUNDARY  tBoundary;
	AE_TUNING_CURVEGEN  tCurvegen;
	AE_TUNING_CONVERGE  tConverge;
	AE_TUNING_MANUAL    tManual;
	AE_TUNING_LACLAMP   tLaclamp;
	AE_TUNING_WINDOW    tMeter_win;
	AE_TUNING_LUMGAMMA  tLumGamma;
	AE_TUNING_SHDR      tSHDR;
	AE_TUNING_UICMD     tUICmd;
	AE_TUNING_IRIS      tIRISCfg;
    AE_TUNING_CURVEGEN_PHOTO tCurvegenPhoto;
	// READ ONLY
	AE_TUNING_STATUS    tStatus;
	AE_TUNING_LA        tLA_Data;
} AE_TUNING_ALL;

extern UINT32 AETuning_Set_Cmd(UINT32 Id, AE_SET_ITEM item, UINT32 addr, UINT32 size);
extern UINT32 AETuning_Get_Cmd(UINT32 Id, AE_GET_ITEM item, UINT32 addr, UINT32 size);

/* Internel used function for ae_ext */
void AETuning_setInit(UINT32 Id, UINT32 addr, UINT32 size);
void AETuning_getPtr(UINT32 Id, UINT32 addr);
void AETuning_getUpdate(UINT32 Id, UINT32 addr);
void AETuning_setUpdate(UINT32 Id, UINT32 up);

extern void AEAlg_CurveDump(void);
extern void AEAlg_CurveInfo(void);
extern void AE_DumpCurveInfo(UINT32 id, AE_CURVEDUMP_TYPE dump_type);
extern void AE_DumpWindow(UINT32 id);

extern UINT32 AE_AlgGetUIInfo(AE_PROC_ID id, AE_SEL_ITEM sel_idx);
extern void AE_AlgSetUIInfo(AE_PROC_ID id, AE_SEL_ITEM sel_idx, UINT32 value);

extern UINT32 uiISOGain[AE_ID_MAX_NUM], uiExpoTime[AE_ID_MAX_NUM]; //, uiIris[AE_ID_MAX_NUM];
extern UINT32 uiAEManualCmd[AE_ID_MAX_NUM], uiAEManualISOCmd[AE_ID_MAX_NUM];
extern UINT32 AE_stableLum[AE_ID_MAX_NUM];
extern UINT32 AE_stableEV[AE_ID_MAX_NUM];
//extern void AEAlg_SHDR_ExposureTransfer(UINT32 shdr_frm, UINT32 *ExpoTime, UINT32 *IsoGain, SHDR_EV_RATIO_INFO *shdr_ev_ratio, UINT32 *ExpoTimeMax, UINT32 IsoMin);
extern void AEAlg_SHDR_ExposureTransfer(AE_SHDR_ExptTrans *shdr_expt_trans);

extern void AE_CurveUpdate(UINT32 id);

UINT32 AE_Get_LumGammaValue(UINT32 Id, UINT32 data);
UINT32 AE_Get_InvLumGammaValue(UINT32 Id, UINT32 data);

extern INT32 ae_do_dciris(AE_INFO *ae_info, AE_IRIS_INFO *ae_iris_info);

typedef struct _AE_PARAM_INFO
{
    UINT32 base_iso;
    UINT32 base_gain_ratio;
    AE_EXPECT_LUM *expect_lum;
    AE_LA_CLAMP *la_clamp;
    AE_HISTO *histogram;
    AE_OVER_EXPOSURE *over_exposure;
    AE_CONVERGENCE *convergence;
    AE_IRISINFO *iris_info;
    AE_PROC_BOUNDARY *proc_boundary;
    AE_CURVE_GEN_PARAM *curve_gen_param;
    UINT32 *meter_win;
    AE_LUM_GAMMA *lum_gamma;
    AE_SHDR *shdr;
    AE_IRIS_CFG *iris_cfg;
    AE_CURVE_GEN_PHOTO *curve_gen_photo;
} AE_PARAM_INFO;

/* AET UART MSG */
#define AE_UART_DUMP_VAR(var) (DBG_DUMP("AET %s = %d\r\n", #var, var))
#define AE_UART_DUMP_ARR(arr, len)      \
	do{                                 \
		UINT32 i;                       \
		DBG_DUMP("%s = { ", #arr);      \
		for(i=0; i<len; i++)            \
			DBG_DUMP("%d, ", arr[i]);   \
		DBG_DUMP("}\r\n");              \
	}while(0)

/******************************************************************************/
#endif //_AE_ALG_H_
