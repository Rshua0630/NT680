/**
    IPH module (RHE sub function type define)

    @file       IPL_Hal_RHE_Info.h NT96660
    @ingroup    mILibIPH

    Copyright   Novatek Microelectronics Corp. 2011.  All rights reserved.
*/

#ifndef _IPL_Hal_RHE_Info_H
#define _IPL_Hal_RHE_Info_H
#include "Type.h"
#include "ipl_utility.h"
/** \addtogroup mILibIPH */
//@{

/**
    rhe engine supported clk
*/
typedef enum _IPH_RHECLK {
	IPH_RHECLK0 = 240,  ///< -240 mhz
	IPH_RHECLK1 = 480,  ///< -480 mhz
	ENUM_DUMMY4WORD(IPH_RHECLK)
} IPH_RHECLK;

/**
    rhe interrupt enable
*/
typedef enum _RHE_INTE_EN {
	RHE__INT_FLDEND             = 0x00000001, ///< Enable function input-raw-decode
	RHE__INT_DEC1_ERR           = 0x00000010, ///< decoder set-1 error interrupt enable
	RHE__INT_DEC2_ERR           = 0x00000020, ///< decoder set-2 error interrupt enable
	RHE__INT_DEC3_ERR           = 0x00000040, ///< decoder set-3 error interrupt enable
	RHE__INT_DEC4_ERR           = 0x00000080, ///< decoder set-4 error interrupt enable
	RHE__INTE_RDE1_DBGFRMEND    = 0x00000100, ///< decoder set-1 frame-end interrupt enable
	RHE__INTE_RDE2_DBGFRMEND    = 0x00000200, ///< decoder set-2 frame-end interrupt enable
	RHE__INTE_RDE3_DBGFRMEND    = 0x00000400, ///< decoder set-3 frame-end interrupt enable
	RHE__INTE_RDE4_DBGFRMEND    = 0x00000800, ///< decoder set-4 frame-end interrupt enable
	RHE__INTE_RDE_STPBD_ERR     = 0x00001000, ///< stripe boundary error interrupt enable

	RHE__INT_ALL                = 0xffffffff, ///< Interrupt - All
	ENUM_DUMMY4WORD(RHE_INTE_EN)
} RHE_INTE_EN;

/**
     RHE sub fucntion enable
*/
typedef enum _RheFuncEn {
	ColorGainEn     = 0x00000001,   ///< Enable function color gains
	SubImgFilterEn  = 0x00000002,   ///< Enable function sub-image filter
	ImgBlendingEn   = 0x00000004,   ///< Enable function image blending
	LocalContrastEn = 0x00000008,   ///< Enable function local contrast enhancement
	ToneCurveEn     = 0x00000010,   ///< Enable function tone curve
	ColorSatEn      = 0x00000020,   ///< Enable function color saturation process
	CNSEn           = 0x00000040,   ///< Enable function color noise suppression
	SubOutEn        = 0x00000080,   ///< Enable function sub-image output
	ContrastAdjEn   = 0x00000100,   ///< Enable function contrast adjustment
	RawDecEn        = 0x00000200,   ///< Enable function raw decode
	FlickerDetEN    = 0x00000400,   ///< Enable function flicker-detection(For 2-frames HDR mode and single stripe mode only)
	HFlipEn         = 0x00000800,   ///< Enable function h flip
	VFlipEn         = 0x00001000,   ///< Enable function v flip

	ENUM_DUMMY4WORD(RheFuncEn)
} RheFuncEn;

/**
    rhe out buffer number
*/
typedef enum _RHE_PPBNum {
	RHE_BUF_1 = 0,  ///< -
	RHE_BUF_2 = 1,  ///< -
	RHE_BUF_3 = 2,  ///< -
	ENUM_DUMMY4WORD(RHE_PPBNum)
} RHE_PPBNum;

/**
    rhe op mode
*/
typedef enum {
	RHE_HAL_OPMODE_D2D     = 0,    ///< RHE only
	RHE_HAL_OPMODE_RHE_IPP = 1,    ///< RHE + IFE->DCE->IPE->IME(IFE2)
	RHE_HAL_OPMODE_SIE_IPP = 2,    ///< SIE + RHE->IFE->DCE->IPE->IME(IFE2) (all direct)
	RHE_HAL_OPMODE_UNKNOWN = 3,    ///< Undefined mode
	ENUM_DUMMY4WORD(RHE_HAL_OPMODE)
} RHE_HAL_OPMODE;

/**
    rhe func mode
*/
typedef enum {
	RHE_HAL_FUNC_MODE_LINEAR    = 0,    ///< linear mode
	RHE_HAL_FUNC_MODE_DFS       = 1,    ///< linear mode with DFS
	RHE_HAL_FUNC_MODE_PHDR      = 2,    ///< pseudo-HDR mode
	RHE_HAL_FUNC_MODE_SHDR      = 3,    ///< sensor-HDR mode
	ENUM_DUMMY4WORD(RHE_HAL_FUNC_MODE)
} RHE_HAL_FUNC_MODE;

/**
    rhe hdr frame mode
*/
typedef enum {
	RHE_HAL_HDR_2FRAME_MODE = 0,     ///< two frame mode
	RHE_HAL_HDR_3FRAME_MODE = 1,     ///< three frame mode
	RHE_HAL_HDR_4FRAME_MODE = 2,     ///< four frame mode
	ENUM_DUMMY4WORD(RHE_HAL_HDR_FRAME_MODE)
} RHE_HAL_HDR_FRAME_MODE;

/**
    rhe frame ev gap selection
*/
typedef enum {
	RHE_HAL_HDR_2FRAME_2EV = 0,     ///< using 2 frames, 2EV gap, meaning +1 / -1 EV
	RHE_HAL_HDR_2FRAME_4EV = 1,     ///< using 2 frames, 4EV gap, meaning +2 / -2 EV
	RHE_HAL_HDR_3FRAME_2EV = 2,     ///< using 3 frames, 2EV gap, meaning +1 / 0 / -1 EV
	RHE_HAL_HDR_3FRAME_4EV = 3,     ///< using 3 frames, 4EV gap, meaning +2 / 0 / -2 EV
	RHE_HAL_HDR_4FRAME_4EV = 4,     ///< using 4 frames, 4EV gap, meaning +2 / +1 / -1 / -2 EV
	ENUM_DUMMY4WORD(RHE_HAL_HDR_EVGAP_SEL)
} RHE_HAL_HDR_EVGAP_SEL;

/**
    RHE enum - parameter mode selection
*/
typedef enum {
	RHE_HAL_PARAM_AUTO_MODE  = 0,   ///< parameters using auto mode
	RHE_HAL_PARAM_USER_MODE  = 1,   ///< parameters using user mode
	ENUM_DUMMY4WORD(RHE_HAL_PARAM_MODE)
}  RHE_HAL_PARAM_MODE;


/**
    RHE data bitdepth
*/
typedef IPL_RAW_BIT RHE_PackBus;

/**
    rhe dma input
*/
typedef enum {
	RHE_HAL_INPATH_1 = 0x1,  ///< setting input path1
	RHE_HAL_INPATH_2 = 0x2,  ///< setting input path2
	RHE_HAL_INPATH_3 = 0x4,  ///< setting input path3
	RHE_HAL_INPATH_4 = 0x8,  ///< setting input path4
	ENUM_DUMMY4WORD(RHE_HAL_INPATH_SEL)
} RHE_HAL_INPATH_SEL;

typedef enum {
	RHE_HAL_BAYER_WITHOUT_IR    = 0,     ///< without Ir component
	RHE_HAL_BAYER_WITH_IR       = 1,     ///< with Ir component
	ENUM_DUMMY4WORD(RHE_HAL_BAYER_FMT)
} RHE_HAL_BAYER_FMT;

typedef IPL_RAW_PIX RHE_HAL_CFA_PAT_SEL;

typedef struct _RHE_Dma_In {
	UINT32  In0_Addr;       ///< -
	UINT32  In1_Addr;       ///< -
	UINT32  In2_Addr;
	UINT32  In3_Addr;

	UINT32  In0_LineOffset;     ///< -
	UINT32  In1_LineOffset;     ///< -
	UINT32  In2_LineOffset;     ///< -
	UINT32  In3_LineOffset;     ///< -

	RHE_PackBus packBus;    ///< data bit depth
} RHE_Dma_In;

typedef struct _RHE_CROP {
	Coordinate crop_pos_0;
	Coordinate crop_pos_1;
	Coordinate crop_pos_2;
	Coordinate crop_pos_3;
} RHE_CROP;

typedef struct _RHE_SIZE {
	UINT32      width;  ///< horizontal size
	UINT32      height; ///< vertical size
} RHE_SIZE;

typedef struct _RHE_HAL_SCALE_COEF_INFO_ {
	RHE_HAL_PARAM_MODE InSubImgSclMode;   ///< input sub-image sclae mode selection

	UINT32 uiFactH;   ///< horizontal scaling factor, setting when InSubImgSclMode = RHE_HAL_PARAM_USER_MODE
	UINT32 uiFactV;   ///< vertical scaling factor, setting when InSubImgSclMode = RHE_HAL_PARAM_USER_MODE
	UINT32 uiPOfsHInt;   ///< horizontal positive offset, integer part, setting when InSubImgSclMode = RHE_HAL_PARAM_USER_MODE
	UINT32 uiPOfsHDec;   ///< horizontal positive offset, decimal part, setting when InSubImgSclMode = RHE_HAL_PARAM_USER_MODE
	UINT32 uiPOfsVInt;   ///< vertical positive offset, integer part, setting when InSubImgSclMode = RHE_HAL_PARAM_USER_MODE
	UINT32 uiPOfsVDec;   ///< vertical positive offset, decimal part, setting when InSubImgSclMode = RHE_HAL_PARAM_USER_MODE
	UINT32 uiNOfsHInt;   ///< horizontal negative offset, integer part, setting when InSubImgSclMode = RHE_HAL_PARAM_USER_MODE
	UINT32 uiNOfsHDec;   ///< horizontal negative offset, decimal part, setting when InSubImgSclMode = RHE_HAL_PARAM_USER_MODE
	UINT32 uiNOfsVInt;   ///< vertical negative offset, integer part, setting when InSubImgSclMode = RHE_HAL_PARAM_USER_MODE
	UINT32 uiNOfsVDec;   ///< vertical negative offset, decimal part, setting when InSubImgSclMode = RHE_HAL_PARAM_USER_MODE
} RHE_HAL_SCALE_COEF_INFO;

/**
    rhe dma output
*/
typedef enum {
	RHE_HAL_BURST_64W  = 0,   ///< 64 words
	RHE_HAL_BURST_96W  = 1,   ///< 96 words
	RHE_HAL_BURST_128W = 2,   ///< 128 words
	ENUM_DUMMY4WORD(RHE_HAL_BURST_SEL)
} RHE_HAL_BURST_SEL;

typedef struct _RHE_Dma_Out {
	UINT32  Out_Addr;      ///< -
	RHE_PackBus packBus;    ///< data bit depth
	UINT32  LineOffset;     ///< -
} RHE_Dma_Out;

/**
    RHE_HAL structure - Multi-frame blending parameters
*/
typedef struct _RHE_HAL_PATH_CRUVE_PARAM_ {
	RHE_HAL_PARAM_MODE      PathCurveParamMode;    ///< path curve parameter mode

	UINT32              *puiCurveA;      ///< curve of path A, 33 entries, support auto mode
	UINT32              *puiCurveB;      ///< curve of path B, 33 entries, support auto mode
	UINT32              *puiCurveC;      ///< curve of path C, 33 entries, support auto mode
	UINT32              *puiCurveD;      ///< curve of path D, 33 entries, support auto mode
	UINT32              *puiCurveE;      ///< curve of path E, 33 entries, support auto mode

	UINT32              *puiWetA;      ///< weight of path A, 17 entries, support auto mode
	UINT32              *puiWetB;      ///< weight of path B, 17 entries, support auto mode
	UINT32              *puiWetC;      ///< weight of path C, 17 entries, support auto mode
} RHE_HAL_PATH_CRUVE_PARAM;

/**
    rhe decode info
*/
typedef enum {
	RHE_HAL_DEC_MST_MODE = 0,      ///< multiple stripe
	RHE_HAL_DEC_SST_MODE = 1,      ///< single stripe
	ENUM_DUMMY4WORD(RHE_HAL_DEC_STP_MODE)
}  RHE_HAL_DEC_STP_MODE;

typedef enum {
	RHE_HAL_DEC_RAND_PADDING_MODE = 0,      ///< Random padding
	RHE_HAL_DEC_ZERO_PADDING_MODE = 1,      ///< Zero padding
	ENUM_DUMMY4WORD(RHE_HAL_DEC_PADDING_MODE)
}  RHE_HAL_DEC_PADDING_MODE;

typedef enum {
	RHE_HAL_DEC_VLC_08_ENTS = 0,      ///< 8 entries
	RHE_HAL_DEC_VLC_16_ENTS = 1,      ///< 16 entries
	RHE_HAL_DEC_VLC_20_ENTS = 2,      ///< 20 entries
	RHE_HAL_DEC_VLC_24_ENTS = 3,      ///< 24 entries
	ENUM_DUMMY4WORD(RHE_HAL_DEC_VLC_ENTS)
}  RHE_HAL_DEC_VLC_ENTS;

typedef enum {
	RHE_HAL_DEC_SRC_0 = 0,      ///< 8 entries
	RHE_HAL_DEC_SRC_1 = 1,      ///< 16 entries
	RHE_HAL_DEC_SRC_2 = 2,      ///< 20 entries
	RHE_HAL_DEC_SRC_3 = 3,      ///< 24 entries
	ENUM_DUMMY4WORD(RHE_HAL_DEC_SRC_SEL)
}  RHE_HAL_DEC_SRC_SEL;

typedef struct _RHE_HAL_DEC_RAND_INIT_ {
	UINT32 uiRandInit0;     ///<  Decoder random sequence init setting 0
	UINT32 uiRandInit1;     ///<  Decoder random sequence init setting 1
}  RHE_HAL_DEC_RAND_INIT;

typedef struct {
	RHE_HAL_PARAM_MODE          DecParamMode;      ///< input decoder parameter mode
	RHE_HAL_DEC_STP_MODE        DceStpMode;        ///< input decoder stripe mode
	RHE_HAL_DEC_PADDING_MODE    DcePadMode;        ///< input decoder output LSB padding mode
	RHE_HAL_DEC_VLC_ENTS        DecVlcEnts;        ///< input decoder VLC entries, support auto mode
	UINT32                      *puiDecCSBTab;     ///< input decoder CSB table, max: 24 entries, support auto mode
	UINT32                      *puiDecLFNTab;     ///< input decoder LFN table, max: 24 entries, support auto mode

	struct {
		BOOL DeGamma;       ///< enable De-Gamma
		UINT32 *pDgmaTbl;   ///< Pointer of degamma table
	} DecGamma;             ///< input decoder de-gamma info.

	struct {
		RHE_HAL_DEC_RAND_INIT   InDecRandInit0;     ///< decoder initial value of set0
		RHE_HAL_DEC_RAND_INIT   InDecRandInit1;     ///< decoder initial value of set1
		RHE_HAL_DEC_RAND_INIT   InDecRandInit2;     ///< decoder initial value of set2
		RHE_HAL_DEC_RAND_INIT   InDecRandInit3;     ///< decoder initial value of set3
	} DecRandInit;                                  ///< input decoder random init value, support auto mode

	struct {
		UINT32 addr0;
		UINT32 addr1;
		UINT32 addr2;
		UINT32 addr3;
	} DecHeaderAddr;        ///< input decoder header DMA address
} RHE_RAW_DECODE_INFO;

/**
    rhe color gain info
*/
typedef enum {
	RHE_HAL_CG_RNG_28 = 0,      ///< 2 bits for decimal and 8 bits for fraction
	RHE_HAL_CG_RNG_37 = 1,      ///< 2 bits for decimal and 8 bits for fraction
	ENUM_DUMMY4WORD(RHE_HAL_CGAIN_RNG)
}  RHE_HAL_CGAIN_RNG;

typedef struct _RHE_HAL_COLOR_GAIN_INFO_ {
	// color gain param
	UINT32 uiCGainR;            ///<  Color gain for R channel
	UINT32 uiCGainGr;           ///<  Color gain for Gr channel
	UINT32 uiCGainGb;           ///<  Color gain for Cb channel
	UINT32 uiCGainB;            ///<  Color gain for B channel
	RHE_HAL_CGAIN_RNG CGainRng; ///< Range of color gain coefficients

	// color offset param
	UINT32 uiCOfsR;        ///<  Color offset for R channel
	UINT32 uiCOfsGr;       ///<  Color offset for Gr channel
	UINT32 uiCOfsGb;       ///<  Color offset for Cb channel
	UINT32 uiCOfsB;        ///<  Color offset for B channel
} RHE_HAL_COLOR_GAIN_INFO;

/**
    rhe sub-image filter
*/
typedef enum {
	RHE_HAL_FRT_SIZE_3x3 = 0,      ///< Filter size selection, 3x3
	RHE_HAL_FRT_SIZE_5x5 = 1,      ///< Filter size selection, 5x5.
	RHE_HAL_FRT_SIZE_7x7 = 2,      ///< Filter size selection, 7x7
	ENUM_DUMMY4WORD(RHE_HAL_FRT_SIZE)
}  RHE_HAL_FRT_SIZE;

typedef struct _RHE_HAL_FILTER_INFO {
	RHE_HAL_PARAM_MODE  FtrParamMode;   ///< sub-image input filter parameter mode selection
	RHE_HAL_FRT_SIZE    FtrSize;        ///< sub-image input filter size, support auto mode
	UINT32          *puiLpfCoefs;   ///< low pass filter coefficients, 12 elements, support auto mode
	UINT32          uiLpfNormM;     ///< low-pass-filter normalization factor M. , support auto mode
	UINT32          uiLpfNormS;     ///< low-pass-filter normalization factor S. , support auto mode
} RHE_HAL_FILTER_INFO;

/**
    rhe image blending info
*/
typedef struct _RHE_HAL_BLEND_PARAM_ {
	RHE_HAL_PARAM_MODE  BldParamMode;   ///< image blending parameter mode
	UINT32              uiRefWet;       ///< reference map weight
	UINT32              *puiGammaCurve; ///< RGB Gamma curve, 129 entries
	UINT32              *puiBldCurve;   ///< image blending curve, 65 entries, support auto mode
} RHE_HAL_BLEND_PARAM;

/**
    rhe local contrast enhancement info
*/
typedef enum {
	RHE_HAL_LTC_MI = 0,     ///< luminance gain reference from main image
	RHE_HAL_LTC_SI = 1,     ///< luminance gain reference from sub image
	ENUM_DUMMY4WORD(RHE_HAL_LTC_REF)
} RHE_HAL_LTC_REF;

typedef struct _RHE_HAL_LTC_PARAM_ {
	UINT32 *puiDifGain;     ///< local diference gainm, 17 element
	UINT32 *puiLumaGain;    ///< local luma gain, 17 element
	RHE_HAL_LTC_REF RefSrc; ///< luma gain reference source
} RHE_HAL_LTC_PARAM;

/**
    rhe tone curve info
*/
typedef enum {
	RHE_HAL_TC_RW_MODE_SRAM = 0,      ///< read global tone curve table from SRAM
	RHE_HAL_TC_RW_MODE_DRAM = 1,      ///< load global tone curve table from RHE_HAL_GTC_DRAM_SAI to SRAM, and then read table value from SRAM
	RHE_HAL_TC_RW_MODE_CPU  = 2,      ///< CPU read and write SRAM for global tone curve table
	ENUM_DUMMY4WORD(RHE_HAL_TC_RW_MODE)
}  RHE_HAL_TC_RW_MODE;

typedef enum {
	RHE_HAL_TC_DATA_Y = 0,      ///< using Y channel
	RHE_HAL_TC_DATA_BAYER = 1,      ///< using Bayer
	ENUM_DUMMY4WORD(RHE_HAL_TC_DATA_MODE)
}  RHE_HAL_TC_DATA_MODE;

typedef struct _RHE_HAL_TONE_CRUVE_INFO_ {
	RHE_HAL_TC_RW_MODE      TcMode;         ///< load tone curve mode
	RHE_HAL_TC_DATA_MODE    TcDataMode;     ///< Input data selection for tone curve process
	UINT32                  *puiTcCurve;    ///< tone curve table, 129 entries
	UINT32                  uiTcAddr;       ///< DRAM address of tone curve
} RHE_HAL_TONE_CRUVE_INFO;

/**
    rhe color staturation process info
*/
typedef struct _RHE_HAL_CSAT_INFO_ {
	RHE_HAL_PARAM_MODE  CSatParamMode;    ///< image blending parameter mode, support auto mode
	UINT32              *puiCSatWet;      ///< RHE color saturation control weight table, 17 entries, support auto mode
} RHE_HAL_CSAT_INFO;

/**
    rhe color noise suppression
*/
typedef struct _RHE_HAL_CNS_INFO_ {
	UINT32              uiCnsShift;      ///< color noise suppress shift value
	UINT32              *puiCnsWet;      ///< color noise suppression weight table, 17 entries
} RHE_HAL_CNS_INFO;

/**
    rhe sub image output info
*/
typedef struct _RHE_HAL_CADJ_SUBOUT_INFO_ {
	RHE_SIZE            SubOutSize;             ///< contrast adjustment sub-image size
	struct {
		RHE_HAL_PARAM_MODE  SubOutParamMode;    ///< contrast adjustment sub-image output parameter mode
		RHE_SIZE            SubOutBlkSize;      ///< contrast adjustment sub-image output block size
		UINT32              uiFactorH;          ///< block horizontal center factor
		UINT32              uiFactorV;          ///< block vertical center factor
	} SubOutParam;                              ///< contrast adjustment sub-image parameters

	UINT32                  uiSubOutOfs;            ///< contrast adjustment sub-image output lineoffset
	UINT32                  uiSubOutAddr;           ///< contrast adjustment sub-image output DMA Address
	UINT32                  *puiSuboutGamma;        ///< Contrast adjust gamma table, 33 entries, support auto mode

} RHE_HAL_CADJ_SUBOUT_INFO;

/**
    rhe contrast adjustment info
*/
typedef enum {
	RHE_HAL_CADJ_BLD_REF_NONE  = 0,     ///< do not refer to gamma
	RHE_HAL_CADJ_BLD_REF_GAMMA = 1,     ///< refer to gamma
	ENUM_DUMMY4WORD(RHE_HAL_CADJ_BLD_REF)
} RHE_HAL_CADJ_BLD_REF;

typedef enum {
	RHE_HAL_CADJ_OPT_A = 0,     ///< Contrast adjustment operation-A
	RHE_HAL_CADJ_OPT_B = 1,     ///< Contrast adjustment operation-B
	ENUM_DUMMY4WORD(RHE_HAL_CADJ_OPT_SEL)
} RHE_HAL_CADJ_OPT_SEL;

typedef enum {
	RHE_HAL_CADJ_OUT_BADJ = 0,  ///< contrast adjust output weighting reference, before adj.
	RHE_HAL_CADJ_OUT_AADJ = 1,  ///< contrast adjust output weighting reference, after adj.
	ENUM_DUMMY4WORD(RHE_HAL_CADJ_OUT_REF)
} RHE_HAL_CADJ_OUT_REF;

typedef struct _RHE_HAL_CADJ_DIV_CLAMP_ {
	UINT32 uiDiv;       ///< division
	UINT32 uiClamp;     ///< clamp
} RHE_HAL_CADJ_DIV_CLAMP;

typedef struct _RHE_HAL_CADJ_ENH_INFO_ {
	UINT32              uiCAdjRto;      ///< contrast adjustment ratio
	UINT32              uiCAdjMinDif;   ///< contrast adjustment minimun
	UINT32              uiCAdjGainRto;  ///< contrast adjustment gain ratio
	RHE_HAL_CADJ_OPT_SEL    CAdjOpt;        ///< contrast adjustment selection
} RHE_HAL_CADJ_ENH_INFO;

typedef struct _RHE_HAL_CADJ_BLEND_ {
	UINT32              *puiCAdjBldRto;     ///< contrast adjustment blending ratio, 9 entries
	RHE_HAL_CADJ_BLD_REF    CadjBldRef;         ///< blending reference selection
} RHE_HAL_CADJ_BLEND;

typedef struct _RHE_HAL_CADJ_OUTPUT_COEFS_ {
	UINT32                  *puiCAdjOutputWet;  ///< output weighting
	RHE_HAL_CADJ_OUT_REF    CAdjOutWetRef;      ///< output weighting reference
	BOOL                    CAdjRefGmaEn;       ///< contrast adjust output weighting reference gamma enable
} RHE_HAL_CADJ_OUTPUT_COEFS;

typedef struct _RHE_HAL_CADJ_PARAM_ {
	RHE_HAL_CADJ_DIV_CLAMP      CAdjMin;            ///< division and clamp of min part
	RHE_HAL_CADJ_DIV_CLAMP      CAdjMax;            ///< division and clamp of max part
	RHE_HAL_CADJ_ENH_INFO       CAdjEnh;            ///< contrast adjustment enhancing control
	RHE_HAL_CADJ_BLEND          CAdjBld;            ///< contrast adjustment blending control
	RHE_HAL_CADJ_OUTPUT_COEFS   CAdjOutCoefs;       ///< output coefficients control
	RHE_HAL_PARAM_MODE      CAdjParamMode;          ///< contrast adjustment parameter mode, support mod-A, mod-B and gamma tables
	UINT32                  *puiCAdjModLutA;    ///< contrast adjustment mod-A LUT, 9 entries, support auto mode
	UINT32                  *puiCAdjModLutB;    ///< contrast adjustment mod-B LUT, 9 entries, support auto mode
	UINT32                  *puiCAdjGamma;      ///< Contrast adjust gamma table, 33 entries, support auto mode
} RHE_HAL_CADJ_PARAM;

typedef struct _RHE_HAL_CADJ_INFO_ {
	RHE_HAL_CADJ_PARAM  CAdjParam;          ///< contrast adjustment parameters
	UINT32              uiCAdjSubInAddr;    ///< contrast adjustment sub-image input buffer address
	RHE_SIZE            CAdjSubImgSize;     ///< contrast adjustment sub-in image size
	UINT32              uiCAdjImgOfs;       ///< contrast adjustment sub-image output lineoffset

	struct {
		RHE_HAL_PARAM_MODE  FtrParamMode;      ///< contrast adjustment
		UINT32              uiFtrCoef[3];  ///< sub-image low pass filter coefficients, 3 entries
	} CAdjFilter;   ///< contrast adjustment sub-in filter coefficients

	struct {
		RHE_HAL_PARAM_MODE  CAdjParamMode;      ///< contrast adjustment
		UINT32              uiSclFactorH;       ///< horizontal sub-image scale factor
		UINT32              uiSclFactorV;        ///< vertical sub-image scale factor
	} CAdjSclFtr;   ///< contrast adjustment scale factor
} RHE_HAL_CADJ_INFO;

/**
    rhe dark frame info, only support d2d mode
*/
typedef enum {
	RHE_HAL_DFS_DP_BY_IN0  = 0, ///< DP threshold on input0
	RHE_HAL_DFS_DP_BY_IN1,      ///< DP threshold on input1
	ENUM_DUMMY4WORD(RHE_HAL_DFS_DP_SRC_SEL)
} RHE_HAL_DFS_DP_SRC_SEL;

typedef enum {
	RHE_HAL_DFS_OP_B_MINUS_A  = 0, ///< Input-1 minus Input-0
	RHE_HAL_DFS_OP_A_MINUS_B,      ///< Input-0 minus Input-1
	RHE_HAL_DFS_OP_A_PLUS_B,       ///< Input-0 plus  Input-1
	ENUM_DUMMY4WORD(RHE_HAL_DFS_OP_SEL)
} RHE_HAL_DFS_OP_SEL;

typedef enum {
	RHE_HAL_DEF_FTR_50  = 0, ///< Defect concealment cross-channel weight = 50%
	RHE_HAL_DEF_FTR_25  = 1, ///< Defect concealment cross-channel weight = 25%
	RHE_HAL_DEF_FTR_0   = 2, ///< Defect concealment cross-channel weight =  0% (disabled)
	ENUM_DUMMY4WORD(RHE_HAL_DEF_FTR_SEL)
} RHE_HAL_DEF_FTR_SEL;

typedef struct {
	UINT32                  uiDpTh;     ///< Defect upper threshold
	RHE_HAL_DFS_DP_SRC_SEL  DpSrc;      ///< Defect thresholding source
	RHE_HAL_DFS_OP_SEL      DfsOp;      ///< Operation type
	RHE_HAL_DEF_FTR_SEL     DefFtr;     ///< Defect concealment cross-channel factor
} RHE_HAL_DFS_INFO;

/**
    rhe filcker detection info
*/
typedef struct _RHE_HAL_FLICKER_DET_INFO_ {
	UINT32      uiStX;     ///< start position X
	UINT32      uiStY;     ///< start position Y
	UINT32      uiWinSzX;  ///< window size X
	UINT32      uiWinSzY;  ///< window size Y
	UINT32      uiWinSpY;  ///< window skip(spacing) Y
	UINT32      uiSmplX;   ///< subsample ratio X (inside window)
	UINT32      uiSmplY;   ///< subsample ratio Y (inside window)
	BOOL        bChSel[4];  ///< channel selection
	UINT32      uiSatTh;    ///< upper thershold
	UINT32      uiLowTh;    ///< lower thershold
} RHE_HAL_FLICKER_DET_INFO;


/**
     RHE sub function configuration.
*/
typedef struct _RHE_SUBFUNC {
	RHE_INTE_EN InteEn;     ///< interrupt enable
	RheFuncEn   rheFuncEn;  ///< function enable

	RheFuncEn   rhe_func_enable;    ///< update function enable
	RheFuncEn   rhe_func_disable;   ///< update function disable

	RHE_RAW_DECODE_INFO         decode_info;
	RHE_HAL_COLOR_GAIN_INFO     color_gain_info;
	RHE_HAL_FILTER_INFO         subimg_filter_info;
	RHE_HAL_BLEND_PARAM         blending_info;
	RHE_HAL_LTC_PARAM           local_contrast_info;
	RHE_HAL_TONE_CRUVE_INFO     tone_curve_info;
	RHE_HAL_CSAT_INFO           color_sat_info;
	RHE_HAL_CNS_INFO            cns_info;
	RHE_HAL_CADJ_SUBOUT_INFO    subout_info;
	RHE_HAL_CADJ_INFO           contrast_adj_info;
	RHE_HAL_DFS_INFO            dark_frame_info;
	RHE_HAL_FLICKER_DET_INFO    filcker_detect_info;
	RHE_HAL_PATH_CRUVE_PARAM    path_curve_info;            ///< Multi-frame blending parameters
} RHE_SUBFUNC;
//@}
#endif
