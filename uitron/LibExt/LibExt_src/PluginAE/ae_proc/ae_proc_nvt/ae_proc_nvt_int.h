/**
    Auto Exposure.


    @file       ae_sample_int.h
    @ingroup    mISYSAlg
    @note       Nothing (or anything need to be mentioned).

    Copyright   Novatek Microelectronics Corp. 2011.  All rights reserved.
*/
#ifndef _AE_SAMPLE_INT_H_
#define _AE_SAMPLE_INT_H_

#include "sensor.h"
#include "ae_api.h"
#include "ae_ui_info.h"

#define AE_CONV2_UINT32(a, b) (((a & 0x0000ffff) << 16) | (b & 0x0000fffff))

/**
     @name AE default value
     @note
*/
//@{
//FIXED PARAMETER BEGIN
#define AE_TOTAL_PARAMNUM           6                           ///< always keep six frames AE Setting. don't modify this value;
#define AE_STICH_LEFT_POS           0
#define AE_STICH_RIGHT_POS          1
//FIXED PARAMETER END

//LA INFO
#define LA_WIN_X                    32                          ///< SIE LA window number x
#define LA_WIN_Y                    32                          ///< SIE LA window number y
#define LA_WIN_NUM                  (LA_WIN_X*LA_WIN_Y)
#define AE_HIST_NUM                 64                          ///< historgram bin number
#define AE_HIST_DATA_NUM            65536                       ///< historgram total data number

//AE INFO
#define AE_COVER_RANGE_H            3                           ///< cover range H
#define AE_COVER_RANGE_L            3                           ///< cover range L
#define AE_COVER_RANGE_RF_H        500                           ///< cover range H rougth
#define AE_COVER_RANGE_RF_L        100                           ///< cover range L rougth
#define AE_EV_RATIO                 (100)
#define AE_NOR_CAL_SET_SHIFT       0
#define AE_NOR_PREDICT_POINT       3
#define AE_NOR_PEROID              3
#define AE_HISPED_CAL_SET_SHIFT    1
#define AE_HISPED_PREDICT_POINT    1
#define AE_HISPED_PEROID           4

//IR CUT PARAM
#define AE_IRCUT_DELAY_CNT          10
#define AE_IRCUT_LV_H               50
#define AE_IRCUT_LV_L               40
#define AE_IRCUT_DELAY_TIME         333330

//#NT#2016/06/30#Jarkko Chang -begin
//#NT# fixed ip cam frequency set fail
#define AE_60FREQ_NODE              ((1 << 1) | (1 << 2) | (1 << 4))
#define AE_50FREQ_NODE              ((1 << 1) | (1 << 2) | (1 << 4) | (1 << 8))

#define AE_60FREQ_EXPT_NODE_33333   33333 //us
#define AE_50FREQ_EXPT_NODE_20000   20000 //us
#define AE_50FREQ_EXPT_NODE_40000   40000 //us
//#NT#2016/06/30#Jarkko Chang -end

//
#define AE_TIMEOUT_MS               500
//@}

/**
     @name AE flag
     @note
*/
//@{
#define AE_FLAG_SET_EXPT    (1 << 0)
#define AE_FLAG_SET_GAIN    (1 << 1)
#define AE_FLAG_SET_IRIS    (1 << 2)
//@}

/**
     AE win type.

     @note
*/
typedef enum _AE_WIN {
	AE_WIN_CENTERWEIGHT = 0,    ///< ae center weighting
	AE_WIN_SPOT,                ///< ae spot weighting
	AE_WIN_MARTIX,              ///< ae matrix weighting
	AE_WIN_EVALUATIVE,          ///< ae evaluative weighting
	AE_WIN_USERDEFINE,          ///< ae user define weighting
	AE_WIN_MAX,
	ENUM_DUMMY4WORD(AE_WIN)
} AE_WIN;


/**
     AE Reset Flag.

     @note
*/
typedef enum _AE_RESET {
	AE_RESET_FALSE = 0, ///<
	AE_RESET_TRUE,      ///< set ae reset
	AE_RESET_MAX,
	ENUM_DUMMY4WORD(AE_RESET)
} AE_RESET;

#define AE_SYNC_TAB_SIZE    10
typedef struct _AE_SYNC_INFO {
	UINT32 frame_cnt;
	AEALG_ARG data;
} AE_SYNC_INFO;

typedef struct _AE_INT_CTRL_INFO {
	UINT32 CalSetShiftNum;  //Set this parameter to separate ae_converge and ae_set(ExpT/Gain) to different frame
	UINT32 PredictPoint;    //valid value 1~3
	UINT32 AEPeroid;        //minimum value: 3
} AE_INT_CTRL_INFO;


/**

     get IPL_ID_1 Raw value by input Y value

     @note

     @param[in] 12 Bit Raw lum
     @param[out] 10 Bit Y lum
*/
extern UINT32 AE_GetGammaValue_ID_1(UINT32 Data12Bit);

/**

     get IPL_ID_2 Raw value by input Y value

     @note

     @param[in] 12 Bit Raw lum
     @param[out] 10 Bit Y lum
*/
extern UINT32 AE_GetGammaValue_ID_2(UINT32 Data12Bit);

/**

     get IPL_ID_3 Raw value by input Y value

     @note

     @param[in] 12 Bit Raw lum
     @param[out] 10 Bit Y lum
*/
extern UINT32 AE_GetGammaValue_ID_3(UINT32 Data12Bit);

/**

     get IPL_ID_4 Raw value by input Y value

     @note

     @param[in] 12 Bit Raw lum
     @param[out] 10 Bit Y lum
*/
extern UINT32 AE_GetGammaValue_ID_4(UINT32 Data12Bit);

/**

     get IPL_ID_5 Raw value by input Y value

     @note

     @param[in] 12 Bit Raw lum
     @param[out] 10 Bit Y lum
*/
extern UINT32 AE_GetGammaValue_ID_5(UINT32 Data12Bit);

/**

     get IPL_ID_6 Raw value by input Y value

     @note

     @param[in] 12 Bit Raw lum
     @param[out] 10 Bit Y lum
*/
extern UINT32 AE_GetGammaValue_ID_6(UINT32 Data12Bit);

/**

     get IPL_ID_7 Raw value by input Y value

     @note

     @param[in] 12 Bit Raw lum
     @param[out] 10 Bit Y lum
*/
extern UINT32 AE_GetGammaValue_ID_7(UINT32 Data12Bit);

/**

     get IPL_ID_8 Raw value by input Y value

     @note

     @param[in] 12 Bit Raw lum
     @param[out] 10 Bit Y lum
*/
extern UINT32 AE_GetGammaValue_ID_8(UINT32 Data12Bit);

/**

     get IPL_ID_1 10 Bit Y value by input 12 Bit Raw value

     @note

     @param[in] 10 Bit Y lum
     @param[out] 12 Bit Raw lum
*/
extern UINT32 AE_GetInvGammaValue_ID_1(UINT32 Data10Bit);

/**

     get IPL_ID_2 10 Bit Y value by input 12 Bit Raw value

     @note

     @param[in] 10 Bit Y lum
     @param[out] 12 Bit Raw lum
*/
extern UINT32 AE_GetInvGammaValue_ID_2(UINT32 Data10Bit);

/**

     get IPL_ID_3 10 Bit Y value by input 12 Bit Raw value

     @note

     @param[in] 10 Bit Y lum
     @param[out] 12 Bit Raw lum
*/
extern UINT32 AE_GetInvGammaValue_ID_3(UINT32 Data10Bit);

/**

     get IPL_ID_4 10 Bit Y value by input 12 Bit Raw value

     @note

     @param[in] 10 Bit Y lum
     @param[out] 12 Bit Raw lum
*/
extern UINT32 AE_GetInvGammaValue_ID_4(UINT32 Data10Bit);

/**

     get IPL_ID_5 10 Bit Y value by input 12 Bit Raw value

     @note

     @param[in] 10 Bit Y lum
     @param[out] 12 Bit Raw lum
*/
extern UINT32 AE_GetInvGammaValue_ID_5(UINT32 Data10Bit);

/**

     get IPL_ID_6 10 Bit Y value by input 12 Bit Raw value

     @note

     @param[in] 10 Bit Y lum
     @param[out] 12 Bit Raw lum
*/
extern UINT32 AE_GetInvGammaValue_ID_6(UINT32 Data10Bit);

/**

     get IPL_ID_7 10 Bit Y value by input 12 Bit Raw value

     @note

     @param[in] 10 Bit Y lum
     @param[out] 12 Bit Raw lum
*/
extern UINT32 AE_GetInvGammaValue_ID_7(UINT32 Data10Bit);

/**

     get IPL_ID_8 10 Bit Y value by input 12 Bit Raw value

     @note

     @param[in] 10 Bit Y lum
     @param[out] 12 Bit Raw lum
*/
extern UINT32 AE_GetInvGammaValue_ID_8(UINT32 Data10Bit);

/**

     get LA Information from SIE

     @note

     @param[in] process id
     @param[out] Luminance 1 : Pre-GG/Post-CG Lum @12b
     @param[out] Luminance 2 : Post-Gamma Lum @10b
     @param[out] Histogram :   @12b Pre-Gamma/@10b Post-Gamma
*/
extern void AE_GetLA(UINT32 Id, UINT16 *Lum1, UINT16 *Lum2, UINT16 *Histo);

/**

     judge preview current and previous setting

     @note

     @param[in] new setting
     @param[in] old setting
     @param[out] update flag
     @return
         - @b TRUE: update setting.
*/
extern UINT32 AE_judgePrvSetting(AEALG_ARG *CurInfo, AEALG_ARG *PrevInfo, UINT32 *Flag);

/**

     get statisic information

     @note

     @param[in/out] AEINFO
     @param[in/out] AEBLKINFO
     @return
         - @b TRUE: current luminance.
*/
extern UINT32 AE_getStatisticInfo(AE_INFO *AeInfo, AE_BLK_INFO *AeBlkInfo);

/**

     ae initial informaiton

     @note

     @param[out] ae information
     @param[out] AEBLKINFO
*/
extern void AE_getInfo(AE_INFO *AeInfo, AE_BLK_INFO *AeBlkInfo);

/**

     set ae window

     @note
     @param[in] process id

     @param[out] ae information
*/
extern AE_RESET AE_setWin(UINT32 Id, AE_WIN mode);

/**

     get ae flag

     @note
     @param[in] process id

     @return
         - @b ae flag.
*/
extern UINT32 AE_getFlag(UINT32 Id);

/**

     set ae flag

     @note
     @param[in] process id
     @param[in] ae flag
*/
extern void AE_setFlag(UINT32 Id, UINT32 flag);

/**

     clear ae flag

     @note
     @param[in] process id
     @param[in] ae flag
*/
extern void AE_clrFlag(UINT32 Id, UINT32 flag);

/**

     ae reset flag
     @note
     @param[in] process id
*/
extern void AE_reset(UINT32 Id);

/**

     ae get capture curve and information
     @note

     @param[in] iso parameter
     @param[in] flicker parameter
     @param[in] scene parameter
     @param[out] ae information
*/
extern AE_RESET AE_getCapTab(AE_INFO *AeInfo);

/**

     ae get preview curve and information
     @note

     @param[in] iso parameter
     @param[in] flicker parameter
     @param[out] ae information
*/
extern AE_RESET AE_getPrvTab(AE_INFO *AeInfo);

/**

     get over exposure ratio
     @note

     @param[in] histogram size
     @param[out] histogram
     @return
         - @b over exposure cnt.
*/
extern UINT32 GetOverExpoRatio(UINT32 HistMaxNum, UINT16 *Hist);

/**

     get fno ratio
     @note

     @param[in] iris index
     @return
         - @b fno ratio.
*/
extern UINT32 GetFnoRatio(UINT32 Id);

/**

     scene map function
     @note

     @param[in] Scene index
     @return
         - @b Scene type.
*/
extern AE_SCENE AE_getSceneType(UINT32 Idx);

/**
     flicker map function
     @note

     @param[in] flicker index
     @return
         - @b flicker type.
*/
extern AE_FLICKER AE_getFlickerType(UINT32 Idx);

/**
     anti-shake map function
     @note

     @param[in] anti-shake index
     @return
         - @b anti-shake type.
*/
extern AE_ANTISHAKE AE_getAntiShakeLv(UINT32 Idx);

/**
     window map function
     @note

     @param[in] window index
     @return
         - @b window type.
*/
extern AE_WIN AE_getWinType(UINT32 Idx);

/**
     get preview sensor bining mode (binning ratio * 100)
     @note

     @param[in] process id
     @return
         - @b bining mode
*/
extern UINT32 AE_getPrvBiningMode(UINT32 Id);

/**
     get capture sensor bining mode (binning ratio * 100)
     @note

     @param[in] process id
     @return
         - @b bining mode
*/
extern UINT32 AE_getCapBiningMode(UINT32 Id);

/**
     ae get control information
     @note

     @param[in/out] AEINFO
     @param[in/out] AEBLKINFO
*/
extern void AE_getCtrlInfo(AE_INFO *AeInfo, AE_BLK_INFO *AeBlkInfo);

/**
     ae set exposure time
     @note
     @param[in] process id
     @param[in] exposure time
*/
extern void AE_setExpT(UINT32 Id, EXPOSURE_SETTING *ExpTSetting);

/**
     ae set iso gain
     @note
     @param[in] process id
     @param[in] iso gain
*/
extern void AE_setGain(UINT32 Id, GAIN_SETTING *Gain);

/**
     ae wait preview stable
     @note

     @param[in] process id
     @param[in] StableCnt : wait stable count
     @param[in] TimeOutFrame : time out frame
     @return
         - @b TRUE: ae already stable
         - @b FALSE: ae not stable and time out
*/
extern BOOL AE_Wait_Stable(UINT32 Id, UINT32 StableCnt, UINT32 TimeOutFrame);

/**
     ae set ir cut switch
     @note
     @param[in] process id
     @param[in] current lv
*/
extern void AE_SetIRCut(UINT32 Id, UINT32 lv);

void AE_DumpCurvePrv(UINT32 id, AE_GEN_INFO *ae_curve_info, UINT32(*AeCurve)[AEALG_INFO_MAX]);
void AE_DumpCurveCap(UINT32 id, AE_GEN_PHOTO *ae_curve_photo, UINT32(*AeCurve)[AEALG_INFO_MAX]);

void AE_Check_EVChange(UINT32 Id);
UINT32 AE_Check_Fno(UINT32 Id, UINT32 fno_rat);
void AE_UartProc(UINT32 Id);
BOOL AE_checkHDRRatio(SHDR_EV_RATIO_INFO ev_ratio);

void ae_dciris_init(UINT32 id);
void ae_dciris_ctrl(UINT32 id, UINT32 drv_ratio, UINT32 max_drv);

#endif //_AE_SAMPLE_INT_H_

