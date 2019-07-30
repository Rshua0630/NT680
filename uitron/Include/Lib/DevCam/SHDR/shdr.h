/**
    Rolling Shutter Correction.

    @file       shdr.h
    @ingroup    mILibRSC
    @note       Nothing.

    Copyright   Novatek Microelectronics Corp. 2014.  All rights reserved.
*/

#ifndef _SHDR_H
#define _SHDR_H

#include "Type.h"

/**
    @addtogroup
*/
//@{


typedef enum {
	SHDR_FUNC_STAGGERED    = 0, ///< Sensor-HDR Staggered Mode
	SHDR_FUNC_CURVED,          ///< Sensor-HDR Curved    Mode
	SHDR_FUNC_MAX_CNT_DEF,
	ENUM_DUMMY4WORD(SHDR_FUNCTION_SELECTION)
} SHDR_FUNCTION_SELECTION;

typedef enum {
	SHDR_EVP_1_EV    = 0, ///< EV gap = 1 EV
	SHDR_EVP_2_EV,       ///< EV gap = 2 EV
	SHDR_EVP_3_EV,       ///< EV gap = 3 EV
	SHDR_EVP_4_EV,       ///< EV gap = 4 EV, suggested
	SHDR_EVP_MAX_CNT_DEF,
	ENUM_DUMMY4WORD(SHDR_EV_GAP_SELECTION)
} SHDR_EV_GAP_SELECTION;

typedef enum {
	SHDR_GTL_0         = 0, ///< Global Tone Adjust level 0 (OFF)
	SHDR_GTL_1,            ///< Global Tone Adjust level 1 (low)
	SHDR_GTL_2,            ///< Global Tone Adjust level 2 (low)
	SHDR_GTL_3,            ///< Global Tone Adjust level 3 (low)
	SHDR_GTL_4,            ///< Global Tone Adjust level 4 (default)
	SHDR_GTL_5,            ///< Global Tone Adjust level 5 (high)
	SHDR_GTL_6,            ///< Global Tone Adjust level 6 (high)
	SHDR_GTL_7,            ///< Global Tone Adjust level 7 (high)
	SHDR_GTL_8,            ///< Global Tone Adjust level 8 (highest)
	SHDR_GTL_NULL,         ///< NULL COMMAND; condiguration would be skipped
	SHDR_GTL_MAX_CNT_DEF,
	ENUM_DUMMY4WORD(SHDR_GLOBAL_TONE_LEVEL)
} SHDR_GLOBAL_TONE_LEVEL;

typedef enum {
	SHDR_LTL_0         = 0, ///< Local  Tone Adjust level 0 (OFF)
	SHDR_LTL_1,            ///< Local  Tone Adjust level 1 (low)
	SHDR_LTL_2,            ///< Local  Tone Adjust level 2 (low)
	SHDR_LTL_3,            ///< Local  Tone Adjust level 3 (low)
	SHDR_LTL_4,            ///< Local  Tone Adjust level 4 (default)
	SHDR_LTL_5,            ///< Local  Tone Adjust level 5 (high)
	SHDR_LTL_6,            ///< Local  Tone Adjust level 6 (high)
	SHDR_LTL_7,            ///< Local  Tone Adjust level 7 (high)
	SHDR_LTL_8,            ///< Local  Tone Adjust level 8 (highest)
	SHDR_LTL_NULL,         ///< NULL COMMAND; condiguration would be skipped
	SHDR_LTL_MAX_CNT_DEF,
	ENUM_DUMMY4WORD(SHDR_LOCALL_TONE_LEVEL)
} SHDR_LOCAL_TONE_LEVEL;


typedef enum {
	SHDR_NPL_0         = 0, ///< Noise Protection level 0 (OFF)      ; Ex:for ISO 100
	SHDR_NPL_1,            ///< Noise Protection level 1 (low)      ; Ex:for ISO 100
	SHDR_NPL_2,            ///< Noise Protection level 2 (low)      ; Ex:for ISO 200
	SHDR_NPL_3,            ///< Noise Protection level 3 (low)      ; Ex:for ISO 200
	SHDR_NPL_4,            ///< Noise Protection level 4 (default)  ; Ex:for ISO 400
	SHDR_NPL_5,            ///< Noise Protection level 5 (high)     ; Ex:for ISO 400
	SHDR_NPL_6,            ///< Noise Protection level 6 (high)     ; Ex:for ISO 800
	SHDR_NPL_7,            ///< Noise Protection level 7 (high)     ; Ex:for ISO 800
	SHDR_NPL_8,            ///< Noise Protection level 8 (highest)  ; Ex:for ISO 1600
	SHDR_NPL_NULL,         ///< NULL COMMAND; condiguration would be skipped
	SHDR_NPL_MAX_CNT_DEF,
	ENUM_DUMMY4WORD(SHDR_NOISE_PROTECT_LEVEL)
} SHDR_NOISE_PROTECT_LEVEL;


typedef enum {
	SHDR_CSL_0         = 0, ///< Noise Protection level 0 (OFF)
	SHDR_CSL_1,            ///< Noise Protection level 1 (low)
	SHDR_CSL_2,            ///< Noise Protection level 2 (low)
	SHDR_CSL_3,            ///< Noise Protection level 3 (low)
	SHDR_CSL_4,            ///< Noise Protection level 4 (default)
	SHDR_CSL_5,            ///< Noise Protection level 5 (high)
	SHDR_CSL_6,            ///< Noise Protection level 6 (high)
	SHDR_CSL_7,            ///< Noise Protection level 7 (high)
	SHDR_CSL_8,            ///< Noise Protection level 8 (highest)
	SHDR_CSL_NULL,         ///< NULL COMMAND; condiguration would be skipped
	SHDR_CSL_MAX_CNT_DEF,
	ENUM_DUMMY4WORD(SHDR_CONVERGE_SPEED_LEVEL)
} SHDR_CONVERGE_SPEED_LEVEL;




typedef struct {
	SHDR_FUNCTION_SELECTION funcSel;
	SHDR_EV_GAP_SELECTION   evGapSel;
} SHDR_MODE_SETTING;

typedef struct {
	SHDR_GLOBAL_TONE_LEVEL      globalTnSel;    ///< Global Tone adjust level
	SHDR_LOCAL_TONE_LEVEL       localTnSel;     ///< Local Tone adjust level
	SHDR_NOISE_PROTECT_LEVEL    noiseProtSel;   ///< Noise Protect adjust level
	SHDR_CONVERGE_SPEED_LEVEL   convSpeedSel;   ///< Converge Speed adjust level
} SHDR_ADJUST_SETTING;

typedef struct {
	UINT32  uiLaSzH;    ///< Luminance Accumulation horizontal window number
	UINT32  uiLaSzV;    ///< Luminance Accumulation vertical   window number
	UINT16  uiLa[32 * 32];   ///< Luminance Accumulation
	UINT16  uiHisto[64];  ///< Histogram
} SHDR_STCS_INPUT;

typedef struct {
	UINT32  uiRawTarget;    ///< Raw Target in 12bits
	UINT8   uiAeMeter[8 * 8];  ///< AE Metering Matrix, 8x8
} SHDR_AE_INPUT;

typedef struct {
	UINT32  uiTargetRatio;  ///< Target ratio in 1/1024
} SHDR_AE_OUTPUT;

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

typedef enum {
	SHDR_RHE_HDR_RAD_WGT_BY_IN0  = 0, ///< Radiance weight table source = input-0
	SHDR_RHE_HDR_RAD_WGT_BY_IN1  = 1, ///< Radiance weight table source = input-1
	ENUM_DUMMY4WORD(SHDR_RHE_HDR_RAD_WGT_SRC_SEL)
} SHDR_RHE_HDR_RAD_WGT_SRC_SEL;

typedef struct {
	UINT32      uiIn0EvGap;     ///< The EV-gap for input-0, for the bright one
	UINT32      uiIn1EvGap;     ///< The EV-gap for input-1, for the dark one
	SHDR_RHE_HDR_RAD_WGT_SRC_SEL RadWgtSrc;  ///< Radiance weight table source
	UINT8       uiWgtTbl[17];   ///< radiance weight table
} SHDR_RHE_HDR_RAD_INFO;


typedef struct {
	UINT8       uiLpf[8];    ///< low pass filter A/B/C/D/E/F/G/H
	UINT32      uiLpfNormM;  ///< low-pass-filter normalization factor M. No need to configure.
	UINT32      uiLpfNormS;  ///< low-pass-filter normalization factor S. No need to configure.
} SHDR_RHE_HDR_REF_LA_LPF_INFO;

typedef struct {
	UINT16 uiGblTnTbl[41];  ///< global tone table, 41taps x 12bits
} SHDR_RHE_HDR_GLOBAL_TONE_INFO;

typedef struct {
	UINT32  uiBand0Gain;    ///< gain for band-0, 10 bits as 1.9 bits
	UINT32  uiBand1Gain;    ///< gain for band-1, 10 bits as 1.9 bits
} SHDR_RHE_HDR_LOCAL_TONE_INFO;

typedef struct {
	UINT8   uiARto[9];    ///< reaulation on ratio, considering data A, 9taps x 8bits
	UINT8   uiAMax[9];    ///< reaulation on max. , considering data A, 9taps x 8bits
	UINT8   uiAMin[9];    ///< reaulation on min. , considering data A, 9taps x 8bits
	UINT8   uiBMax[9];    ///< reaulation on max. , considering data B, 9taps x 8bits
	UINT8   uiBMin[9];    ///< reaulation on min. , considering data B, 9taps x 8bits
} SHDR_RHE_HDR_REGULATION_INFO;

typedef struct {
	UINT16  uiPointX[4];
	UINT16  uiPointY[4];
	UINT16  uiSlopeM[4];
	UINT8   uiSlopeS[4];
} SHDR_RHE_HDR_RECURVEN_INFO;


/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

typedef struct {
	SHDR_RHE_HDR_RAD_INFO            rheRadInfo;         ///<
	SHDR_RHE_HDR_REF_LA_LPF_INFO     rheRefLaLpfInfo;    ///<
	SHDR_RHE_HDR_GLOBAL_TONE_INFO    rheGblTnInfo;       ///<
	SHDR_RHE_HDR_LOCAL_TONE_INFO     rheLocTnInfo;       ///<
} SHDR_RHE_OUTPUT;

typedef struct {
	SHDR_RHE_HDR_RAD_INFO            rheRadInfo;         ///<
	SHDR_RHE_HDR_REF_LA_LPF_INFO     rheRefLaLpfInfo;    ///<
	SHDR_RHE_HDR_GLOBAL_TONE_INFO    rheGblTnInfo;       ///<
	SHDR_RHE_HDR_LOCAL_TONE_INFO     rheLocTnInfo;       ///<
	SHDR_RHE_HDR_REGULATION_INFO     rheReguInfo;        ///<
	SHDR_RHE_HDR_RECURVEN_INFO       rheReCrvInfo;       ///<
} SHDR_RHE_INIT_OUTPUT;


typedef struct {
	SHDR_MODE_SETTING   modeSet;
} SHDR_OPENOBJ;


extern ER shdr_open(SHDR_OPENOBJ *popenObj);
extern ER shdr_close(void);

extern ER shdr_setAdjSet(SHDR_ADJUST_SETTING *pAdjSet);
extern ER shdr_setStcsIn(SHDR_STCS_INPUT *pStcsIn);
extern ER shdr_setAeIn(SHDR_AE_INPUT *pAeIn);

extern ER shdr_run(void);

extern ER shdr_getAeOut(SHDR_AE_OUTPUT *pAeOut);
extern ER shdr_getRheOut(SHDR_RHE_OUTPUT *pRheOut);
extern ER shdr_getRheInitOut(SHDR_RHE_INIT_OUTPUT *pRheInitOut);


//@}



#endif//_SHDR_H
