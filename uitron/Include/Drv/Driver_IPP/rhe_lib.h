/*
    Public header file for RDE module.

    This file is the header file that define the API and data type for RHE
    module.

    @file       rhe_lib.h
    @ingroup    mIDrvIPPRHE
    @note

    Copyright   Novatek Microelectronics Corp. 2011.  All rights reserved.
*/

#ifndef _RHE_LIB_H
#define _RHE_LIB_H

#include "Type.h"

/**
    @addtogroup mIIPPRHE
*/
//@{

typedef void (*RHEIntCB)(UINT32);


// interrupt enable
#define RHE_INTE_FRAMEEND           0x00000001   ///< frame-end interrupt enable
#define RHE_INTE_DEC1_ERR           0x00000010   ///< decoder set-1 error interrupt enable
#define RHE_INTE_DEC2_ERR           0x00000020   ///< decoder set-2 error interrupt enable
#define RHE_INTE_DEC3_ERR           0x00000040   ///< decoder set-3 error interrupt enable
#define RHE_INTE_DEC4_ERR           0x00000080   ///< decoder set-4 error interrupt enable
#define RHE_INTE_RDE1_DBGFRMEND     0x00000100   ///< decoder set-1 frame-end interrupt enable
#define RHE_INTE_RDE2_DBGFRMEND     0x00000200   ///< decoder set-2 frame-end interrupt enable
#define RHE_INTE_RDE3_DBGFRMEND     0x00000400   ///< decoder set-3 frame-end interrupt enable
#define RHE_INTE_RDE4_DBGFRMEND     0x00000800   ///< decoder set-4 frame-end interrupt enable
#define RHE_INTE_RDE_STPBD_ERR      0x00001000   ///< stripe boundary error interrupt enable


// interrupt status
#define RHE_INT_FRAMEEND            0x00000001   ///< frame-end interrupt status
#define RHE_INT_DEC1_ERR            0x00000010   ///< decoder set-1 error interrupt status
#define RHE_INT_DEC2_ERR            0x00000020   ///< decoder set-2 error interrupt status
#define RHE_INT_DEC3_ERR            0x00000040   ///< decoder set-3 error interrupt status
#define RHE_INT_DEC4_ERR            0x00000080   ///< decoder set-4 error interrupt status
#define RHE_INT_RDE1_DBGFRMEND      0x00000100   ///< decoder set-1 frame-end interrupt status
#define RHE_INT_RDE2_DBGFRMEND      0x00000200   ///< decoder set-2 frame-end interrupt status
#define RHE_INT_RDE3_DBGFRMEND      0x00000400   ///< decoder set-3 frame-end interrupt status
#define RHE_INT_RDE4_DBGFRMEND      0x00000800   ///< decoder set-4 frame-end interrupt status
#define RHE_INT_RDE_STP_TOP_ERR     0x00001000   ///< stripe boundary error interrupt enable
#define RHE_INT_RDE_STP_BOTTOM_ERR  0x00002000   ///< Stripe boundary status, bottom
#define RHE_INT_RDE_STP_LEFT_ERR    0x00004000   ///< Stripe boundary status, left
#define RHE_INT_RDE_STP_RIGHT_ERR   0x00008000   ///< Stripe boundary status, right

#define RHE_IN_CHL_NUM              4

/**
    RHE enum - function enable/disable selection
*/
typedef enum {
	RHE_FUNC_DISABLE    = 0,    ///< disable function
	RHE_FUNC_ENABLE     = 1,    ///< enable function
	ENUM_DUMMY4WORD(RHE_FUNC_EN)
} RHE_FUNC_EN;
//------------------------------------
/**
    RHE enum - parameter mode selection
*/
typedef enum {
	RHE_PARAM_AUTO_MODE  = 0,   ///< parameters using auto mode
	RHE_PARAM_USER_MODE  = 1,   ///< parameters using user mode
	ENUM_DUMMY4WORD(RHE_PARAM_MODE)
}  RHE_PARAM_MODE;
//------------------------------------


/**
    RHE enum - operation mode selection
*/
typedef enum {
	RHE_OPMODE_D2D     = 0,    ///< RHE only
	RHE_OPMODE_RHE_IPP = 1,    ///< RHE + IFE->DCE->IPE->IME(IFE2)
	RHE_OPMODE_SIE_IPP = 2,    ///< SIE + RHE->IFE->DCE->IPE->IME(IFE2) (all direct)
	RHE_OPMODE_UNKNOWN = 3,    ///< Undefined mode
	ENUM_DUMMY4WORD(RHE_OPMODE)
} RHE_OPMODE;
//------------------------------------


/**
    RHE enum - Function mode  selection
*/
typedef enum {
	RHE_FUNC_MODE_LINEAR    = 0,    ///< linear mode
	RHE_FUNC_MODE_DFS       = 1,    ///< linear mode with DFS
	RHE_FUNC_MODE_PHDR      = 2,    ///< pseudo-HDR mode
	RHE_FUNC_MODE_SHDR      = 3,    ///< sensor-HDR mode
	ENUM_DUMMY4WORD(RHE_FUNC_MODE)
} RHE_FUNC_MODE;
//------------------------------------

/**
    RHE enum - HDR frame mode selection
*/
typedef enum {
	RHE_HDR_2FRAME_MODE = 0,     ///< two frame mode
	RHE_HDR_3FRAME_MODE = 1,     ///< three frame mode
	RHE_HDR_4FRAME_MODE = 2,     ///< four frame mode
	ENUM_DUMMY4WORD(RHE_HDR_FRAME_MODE)
} RHE_HDR_FRAME_MODE;
//------------------------------------

/**
    RHE enum - HDR frame mode selection
*/
typedef enum {
	RHE_HDR_2FRAME_2EV = 0,     ///< using 2 frames, 2EV gap, meaning +1 / -1 EV
	RHE_HDR_2FRAME_4EV = 1,     ///< using 2 frames, 4EV gap, meaning +2 / -2 EV
	RHE_HDR_3FRAME_2EV = 2,     ///< using 3 frames, 2EV gap, meaning +1 / 0 / -1 EV
	RHE_HDR_3FRAME_4EV = 3,     ///< using 3 frames, 4EV gap, meaning +2 / 0 / -2 EV
	RHE_HDR_4FRAME_4EV = 4,     ///< using 4 frames, 4EV gap, meaning +2 / +1 / -1 / -2 EV
	ENUM_DUMMY4WORD(RHE_HDR_EVGAP_SEL)
} RHE_HDR_EVGAP_SEL;
//------------------------------------

/**
    RHE enum - Path curve selection
*/
typedef enum {
	RHE_PATH_CURVE_A = 0x01,     ///< path curve A
	RHE_PATH_CURVE_B = 0x02,     ///< path curve B
	RHE_PATH_CURVE_C = 0x04,     ///< path curve C
	RHE_PATH_CURVE_D = 0x08,     ///< path curve D
	RHE_PATH_CURVE_E = 0x10,     ///< path curve E
	ENUM_DUMMY4WORD(RHE_PATH_CURVE_SEL)
} RHE_PATH_CURVE_SEL;
//------------------------------------


/**
    RHE enum - Path weight selection
*/
typedef enum {
	RHE_PATH_WEIGHT_A = 0x1,     ///< path weight A
	RHE_PATH_WEIGHT_B = 0x2,     ///< path weight B
	RHE_PATH_WEIGHT_C = 0x4,     ///< path weight C
	ENUM_DUMMY4WORD(RHE_PATH_WEIGHT_SEL)
} RHE_PATH_WEIGHT_SEL;
//------------------------------------

/**
    RHE enum - system flag clear selection
*/
typedef enum _RHE_FLAG_CLEAR_SEL {
	RHE_FLAG_NO_CLEAR = 0,    ///< No clear flag
	RHE_FLAG_CLEAR    = 1,    ///< Clear flag
	ENUM_DUMMY4WORD(RHE_FLAG_CLEAR_SEL)
} RHE_FLAG_CLEAR_SEL;
//------------------------------------

/**
    RHE enum - input path setting selection
*/
typedef enum {
	RHE_INPATH_1 = 0x1,  ///< setting input path1
	RHE_INPATH_2 = 0x2,  ///< setting input path2
	RHE_INPATH_3 = 0x4,  ///< setting input path3
	RHE_INPATH_4 = 0x8,  ///< setting input path4
	ENUM_DUMMY4WORD(RHE_INPATH_SEL)
} RHE_INPATH_SEL;
//------------------------------------

/**
    RHE enum - data packed bit
*/
typedef enum {
	RHE_PACKBUS_08 = 0,     ///< 8 bits packing
	RHE_PACKBUS_10 = 1,     ///< 10 bits packing
	RHE_PACKBUS_12 = 2,     ///< 12 bits packing
	RHE_PACKBUS_16 = 3,     ///< 16 bits packing
	ENUM_DUMMY4WORD(RHE_PACKBUS_SEL)
} RHE_PACKBUS_SEL;
//------------------------------------


/**
    RHE enum - Color filter array format
*/
typedef enum {
	RHE_BAYER_WITHOUT_IR    = 0,     ///< without Ir component
	RHE_BAYER_WITH_IR       = 1,     ///< with Ir component
	ENUM_DUMMY4WORD(RHE_BAYER_FMT)
} RHE_BAYER_FMT;
//------------------------------------

/**
    RHE enum - Color filter array format
*/
typedef enum {
	RHE_CFA_RGGB = 0,     ///< R/G/G/B;  or R/C/C/B;  R/I/G/B
	RHE_CFA_GRBG = 1,     ///< G/R/B/G;  or C/R/B/C;  R/G/I/B
	RHE_CFA_GBRG = 2,     ///< G/B/R/G;  or C/B/R/C;  G/B/R/I
	RHE_CFA_BGGR = 3,     ///< B/G/G/R;  or B/C/C/R;  I/B/R/G
	ENUM_DUMMY4WORD(RHE_CFA_PAT_SEL)
} RHE_CFA_PAT_SEL;
//------------------------------------

/**
    RHE enum - DP threshold input selection
*/
typedef enum {
	RHE_DFS_DP_BY_IN0  = 0, ///< DP threshold on input0
	RHE_DFS_DP_BY_IN1,      ///< DP threshold on input1
	ENUM_DUMMY4WORD(RHE_DFS_DP_SRC_SEL)
} RHE_DFS_DP_SRC_SEL;
//------------------------------------

typedef enum {
	RHE_DFS_OP_B_MINUS_A  = 0, ///< Input-1 minus Input-0
	RHE_DFS_OP_A_MINUS_B,      ///< Input-0 minus Input-1
	RHE_DFS_OP_A_PLUS_B,       ///< Input-0 plus  Input-1
	ENUM_DUMMY4WORD(RHE_DFS_OP_SEL)
} RHE_DFS_OP_SEL;
//------------------------------------

typedef enum {
	RHE_DEF_FTR_50  = 0, ///< Defect concealment cross-channel weight = 50%
	RHE_DEF_FTR_25  = 1, ///< Defect concealment cross-channel weight = 25%
	RHE_DEF_FTR_0   = 2, ///< Defect concealment cross-channel weight =  0% (disabled)
	ENUM_DUMMY4WORD(RHE_DEF_FTR_SEL)
} RHE_DEF_FTR_SEL;
//------------------------------------

typedef enum {
	RHE_BURST_64W  = 0,   ///< 64 words
	RHE_BURST_96W  = 1,   ///< 96 words
	RHE_BURST_128W = 2,   ///< 128 words
	ENUM_DUMMY4WORD(RHE_BURST_SEL)
} RHE_BURST_SEL;
//------------------------------------

/**
    RHE enum - Range of color gain coefficients
*/
typedef enum {
	RHE_CG_RNG_28 = 0,      ///< 2 bits for decimal and 8 bits for fraction
	RHE_CG_RNG_37 = 1,      ///< 2 bits for decimal and 8 bits for fraction
	ENUM_DUMMY4WORD(RHE_CGAIN_RNG)
}  RHE_CGAIN_RNG;
//------------------------------------

/**
    RHE enum - Range of color gain coefficients
*/
typedef enum {
	RHE_FRT_SIZE_3x3 = 0,      ///< Filter size selection, 3x3
	RHE_FRT_SIZE_5x5 = 1,      ///< Filter size selection, 5x5.
	RHE_FRT_SIZE_7x7 = 2,      ///< Filter size selection, 7x7
	ENUM_DUMMY4WORD(RHE_FRT_SIZE)
}  RHE_FRT_SIZE;
//------------------------------------

/**
    RHE enum - decoder format support multiple or single stripe in IPP
*/
typedef enum {
	RHE_DEC_MST_MODE = 0,      ///< multiple stripe
	RHE_DEC_SST_MODE = 1,      ///< single stripe
	ENUM_DUMMY4WORD(RHE_DEC_STP_MODE)
}  RHE_DEC_STP_MODE;
//------------------------------------

/**
    RHE enum - decoder output LSB selection
*/
typedef enum {
	RHE_DEC_RAND_PADDING_MODE = 0,      ///< Random padding
	RHE_DEC_ZERO_PADDING_MODE = 1,      ///< Zero padding
	ENUM_DUMMY4WORD(RHE_DEC_PADDING_MODE)
}  RHE_DEC_PADDING_MODE;
//------------------------------------

/**
    RHE enum - Number of entries of decoder VLC table
*/
typedef enum {
	RHE_DEC_VLC_08_ENTS = 0,      ///< 8 entries
	RHE_DEC_VLC_16_ENTS = 1,      ///< 16 entries
	RHE_DEC_VLC_20_ENTS = 2,      ///< 20 entries
	RHE_DEC_VLC_24_ENTS = 3,      ///< 24 entries
	ENUM_DUMMY4WORD(RHE_DEC_VLC_ENTS)
}  RHE_DEC_VLC_ENTS;
//------------------------------------

/**
    RHE enum - Decoder source selection
*/
typedef enum {
	RHE_DEC_SRC_0 = 0,      ///< 8 entries
	RHE_DEC_SRC_1 = 1,      ///< 16 entries
	RHE_DEC_SRC_2 = 2,      ///< 20 entries
	RHE_DEC_SRC_3 = 3,      ///< 24 entries
	ENUM_DUMMY4WORD(RHE_DEC_SRC_SEL)
}  RHE_DEC_SRC_SEL;
//------------------------------------


/**
    RHE enum - Input data selection for tone curve process
*/
typedef enum {
	RHE_TC_DATA_Y = 0,      ///< using Y channel
	RHE_TC_DATA_BAYER = 1,      ///< using Bayer
	ENUM_DUMMY4WORD(RHE_TC_DATA_MODE)
}  RHE_TC_DATA_MODE;
//------------------------------------

/**
    RHE enum - Tone curve read / write mode
*/
typedef enum {
	RHE_TC_RW_MODE_SRAM = 0,      ///< read global tone curve table from SRAM
	RHE_TC_RW_MODE_DRAM = 1,      ///< load global tone curve table from RHE_GTC_DRAM_SAI to SRAM, and then read table value from SRAM
	RHE_TC_RW_MODE_CPU  = 2,      ///< CPU read and write SRAM for global tone curve table
	ENUM_DUMMY4WORD(RHE_TC_RW_MODE)
}  RHE_TC_RW_MODE;
//------------------------------------

/**
    RHE enum - Contrast adjust output weighting reference
*/
typedef enum {
	RHE_CADJ_OUT_BADJ = 0,  ///< contrast adjust output weighting reference, before adj.
	RHE_CADJ_OUT_AADJ = 1,  ///< contrast adjust output weighting reference, after adj.
	ENUM_DUMMY4WORD(RHE_CADJ_OUT_REF)
} RHE_CADJ_OUT_REF;
//------------------------------------

/**
    RHE enum - Contrast adjustment operation
*/
typedef enum {
	RHE_CADJ_OPT_A = 0,     ///< Contrast adjustment operation-A
	RHE_CADJ_OPT_B = 1,     ///< Contrast adjustment operation-B
	ENUM_DUMMY4WORD(RHE_CADJ_OPT_SEL)
} RHE_CADJ_OPT_SEL;
//------------------------------------


/**
    RHE enum - Contrast adjustment blending reference selection
*/
typedef enum {
	RHE_CADJ_BLD_REF_NONE  = 0,     ///< do not refer to gamma
	RHE_CADJ_BLD_REF_GAMMA = 1,     ///< refer to gamma
	ENUM_DUMMY4WORD(RHE_CADJ_BLD_REF)
} RHE_CADJ_BLD_REF;
//------------------------------------


/**
    RHE enum - Luma Reference of Local Contrast Gain
*/
typedef enum {
	RHE_LTC_MI = 0,     ///< luminance gain reference from main image
	RHE_LTC_SI = 1,     ///< luminance gain reference from sub image
	ENUM_DUMMY4WORD(RHE_LTC_REF)
} RHE_LTC_REF;



//---------------------------------------------------------------------

/**
    RHE structure - engine open object
*/
typedef struct _RHE_OPENOBJ {
	void (*FP_RHEISR_CB)(UINT32 intstatus);   ///< External ISR call-back function
	UINT32 uiRheClockSel;                     ///< Engine clock selection
} RHE_OPENOBJ, *pRHE_OPENOBJ;



/**
    RHE structure - Image size
*/
typedef struct _RHE_IMG_SIZE_ {
	UINT32 uiSizeH;   ///< image width
	UINT32 uiSizeV;   ///< image height
} RHE_IMG_SIZE;
//------------------------------------

/**
    RHE structure - Crop position
*/
typedef struct _RHE_CROP_POS_ {
	UINT32 uiCropPosH;   ///< horizontal crop position
	UINT32 uiCropPosV;   ///< vertical crop position
} RHE_CROP_POS;
//------------------------------------

typedef struct _RHE_IMG_CROP_INFO_ {
	UINT32        SetChl;       ///< desired setting channel, SetChl = (RHE_INPATH_1 | RHE_INPATH_2)
	RHE_CROP_POS  CropPos0;     ///< image crop position for set-0
	RHE_CROP_POS  CropPos1;     ///< image crop position for set-1
	RHE_CROP_POS  CropPos2;     ///< image crop position for set-2
	RHE_CROP_POS  CropPos3;     ///< image crop position for set-3
}  RHE_IMG_CROP_INFO;

typedef struct _RHE_IN_IMG_LOFS_ {
	UINT32 SetChl;       ///< desired setting channel, SetChl = (RHE_INPATH_1 | RHE_INPATH_2)
	UINT32 uiLofs0;    ////< input image lineoffset for set-0
	UINT32 uiLofs1;    ////< input image lineoffset for set-1
	UINT32 uiLofs2;    ////< input image lineoffset for set-2
	UINT32 uiLofs3;    ////< input image lineoffset for set-3
}  RHE_IN_IMG_LOFS;


typedef struct _RHE_IN_IMG_ADDR_ {
	UINT32 SetChl;       ///< desired setting channel, SetChl = (RHE_INPATH_1 | RHE_INPATH_2)
	UINT32 uiAddr0;    ////< input image DMA address for set-0
	UINT32 uiAddr1;    ////< input image DMA address for set-1
	UINT32 uiAddr2;    ////< input image DMA address for set-2
	UINT32 uiAddr3;    ////< input image DMA address for set-3
}  RHE_IN_IMG_ADDR;


/**
    RHE structure - Image scaling factors
*/
typedef struct _RHE_SCALE_COEF_INFO_ {
	RHE_PARAM_MODE InSubImgSclMode;   ///< input sub-image sclae mode selection

	RHE_IMG_SIZE        InImgSize;          ///< input image size, used to RHE_PARAM_AUTO_MODE
	RHE_IMG_SIZE        InSubImgSize;       ///< input sub-image size, used to RHE_PARAM_AUTO_MODE
	RHE_IMG_SIZE        InSubImgBlkSize;   ///< each block size of sub-image of SIE , used to RHE_PARAM_AUTO_MODE

	UINT32 uiFactH;   ///< horizontal scaling factor, setting when InSubImgSclMode = RHE_PARAM_USER_MODE
	UINT32 uiFactV;   ///< vertical scaling factor, setting when InSubImgSclMode = RHE_PARAM_USER_MODE
	UINT32 uiPOfsHInt;   ///< horizontal positive offset, integer part, setting when InSubImgSclMode = RHE_PARAM_USER_MODE
	UINT32 uiPOfsHDec;   ///< horizontal positive offset, decimal part, setting when InSubImgSclMode = RHE_PARAM_USER_MODE
	UINT32 uiPOfsVInt;   ///< vertical positive offset, integer part, setting when InSubImgSclMode = RHE_PARAM_USER_MODE
	UINT32 uiPOfsVDec;   ///< vertical positive offset, decimal part, setting when InSubImgSclMode = RHE_PARAM_USER_MODE
	UINT32 uiNOfsHInt;   ///< horizontal negative offset, integer part, setting when InSubImgSclMode = RHE_PARAM_USER_MODE
	UINT32 uiNOfsHDec;   ///< horizontal negative offset, decimal part, setting when InSubImgSclMode = RHE_PARAM_USER_MODE
	UINT32 uiNOfsVInt;   ///< vertical negative offset, integer part, setting when InSubImgSclMode = RHE_PARAM_USER_MODE
	UINT32 uiNOfsVDec;   ///< vertical negative offset, decimal part, setting when InSubImgSclMode = RHE_PARAM_USER_MODE
} RHE_SCALE_COEF_INFO;
//------------------------------------

/**
    RHE structure - Color gain parameters
*/
typedef struct _RHE_COLOR_GAIN_PARAM_ {
	UINT32 uiCGainR;        ///<  Color gain for R channel
	UINT32 uiCGainGr;       ///<  Color gain for Gr channel
	UINT32 uiCGainGb;       ///<  Color gain for Cb channel
	UINT32 uiCGainB;        ///<  Color gain for B channel
	RHE_CGAIN_RNG CGainRng;    ///< Range of color gain coefficients
} RHE_COLOR_GAIN_PARAM;
//------------------------------------

/**
    RHE structure - Color offset parameters
*/
typedef struct _RHE_COLOR_OFS_PARAM_ {
	UINT32 uiCOfsR;        ///<  Color offset for R channel
	UINT32 uiCOfsGr;       ///<  Color offset for Gr channel
	UINT32 uiCOfsGb;       ///<  Color offset for Cb channel
	UINT32 uiCOfsB;        ///<  Color offset for B channel
} RHE_COLOR_OFS_PARAM;
//------------------------------------

/**
    RHE structure - Color gain setting info.
*/
typedef struct _RHE_COLOR_GAIN_INFO_ {
	RHE_FUNC_EN     CgEn;    ///< color gain enable
	RHE_COLOR_GAIN_PARAM  CGainParam;  ///< color gain
	RHE_COLOR_OFS_PARAM   COfsParam;   ///< color offset
} RHE_COLOR_GAIN_INFO;
//------------------------------------

/**
    RHE structure - sub-image filter parameters
*/
typedef struct _RHE_FTR_PARAM_ {
	RHE_PARAM_MODE  FtrParamMode;   ///< sub-image input filter parameter mode selection
	RHE_FRT_SIZE    FtrSize;        ///< sub-image input filter size, support auto mode
	UINT32          *puiLpfCoefs;   ///< low pass filter coefficients, 10 elements, support auto mode
	UINT32          uiLpfNormM;     ///< low-pass-filter normalization factor M. , support auto mode
	UINT32          uiLpfNormS;     ///< low-pass-filter normalization factor S. , support auto mode
} RHE_FTR_PARAM;


/**
    RHE structure - sub-image filter setting info.
*/
typedef struct _RHE_FTR_INFO_ {
	RHE_FUNC_EN     FtrEn;      ///< sub-image input filter
	RHE_FTR_PARAM   FtrParam;   ///< sub-image filter parameters
} RHE_FTR_INFO;
//------------------------------------

/**
    RHE structure - Multi-frame blending parameters
*/
typedef struct _RHE_BLEND_PARAM_ {
	RHE_PARAM_MODE      BldParamMode;    ///< image blending parameter mode
	UINT32              uiRefWet;        ///< reference map weight
	UINT32              *puiGammaCurve;  ///< IPE RGB Gamma curve, 130 entries

	UINT32              *puiBldCurve;   ///< image blending curve, 65 entries, support auto mode
} RHE_BLEND_PARAM;
//------------------------------------


/**
    RHE structure - Multi-frame blending process
*/
typedef struct _RHE_BLEND_INFO_ {
	RHE_FUNC_EN     BldEn;      ///< image blending process enable
	RHE_BLEND_PARAM BldParam;   ///< image blending parameters
} RHE_BLEND_INFO;
//------------------------------------

/**
    RHE structure - Local contrast enhancement parameters
*/
typedef struct _RHE_LTC_PARAM_ {
	UINT32 *puiDifGain;     ///< local diference gain, 17 entries
	UINT32 *puiLumaGain;    ///< local luma gain, 17 entries
	RHE_LTC_REF RefSrc;     ///< luma gain reference source
} RHE_LTC_PARAM;
//------------------------------------

/**
    RHE structure - Local contrast enhancement setting info.
*/
typedef struct _RHE_LTC_INFO_ {
	RHE_FUNC_EN     LtcEn;      ///< local contrast enh. enable
	RHE_LTC_PARAM   LtcParam;   ///< local contrast enh. parameters
} RHE_LTC_INFO;
//------------------------------------


/**
    RHE structure - tone curve parameters
*/
typedef struct _RHE_TONE_CURVE_PARAM_ {
	RHE_TC_RW_MODE      TcMode;         ///< load tone curve mode
	RHE_TC_DATA_MODE    TcDataMode;     ///< Input data selection for tone curve process
	UINT32              *puiTcCurve;    ///< tone curve table, 129 entries
	UINT32 				uiTcAddr;		///< tone curve DRAM buffer address
} RHE_TONE_CURVE_PARAM;
//------------------------------------


/**
    RHE structure - tone curve setting info.
*/
typedef struct _RHE_TONE_CRUVE_INFO_ {
	RHE_FUNC_EN             TcEn;           ///< tone curve processing enable
	RHE_TONE_CURVE_PARAM    TcParam;        ///< tone curve parameters
} RHE_TONE_CRUVE_INFO;
//------------------------------------

/**
    RHE structure - Color saturation parameters
*/
typedef struct _RHE_CSAT_PARAM_ {
	RHE_PARAM_MODE      CSatParamMode;    ///<RHE color saturation control parameter mode, support auto mode
	UINT32              *puiCSatWet;      ///< RHE color saturation control weight table, 17 entries, support auto mode
} RHE_CSAT_PARAM;
//------------------------------------


/**
    RHE structure - Color saturation processing
*/
typedef struct _RHE_CSAT_INFO_ {
	RHE_FUNC_EN     CSatEn;     ///< color saturation processing enable
	RHE_CSAT_PARAM  CSatParam;  ///< color saturation parameters
} RHE_CSAT_INFO;
//------------------------------------

/**
    RHE structure - Color noise parameters
*/
typedef struct _RHE_CNS_PARAM_ {
	UINT32              uiCnsShift;      ///< color noise suppress shift value
	UINT32              *puiCnsWet;      ///< color noise suppression weight table, 17 entries
} RHE_CNS_PARAM;
//------------------------------------

/**
    RHE structure - Color noise suppress
*/
typedef struct _RHE_CNS_INFO_ {
	RHE_FUNC_EN     CnsEn;      ///< color noise suppression enable
	RHE_CNS_PARAM   CnsParam;   ///< color noise suppression parameters
} RHE_CNS_INFO;
//------------------------------------

/**
    RHE structure - Contrast adjustment sub-image center factor
*/
typedef struct _RHE_CADJ_SUBOUT_CENT_FACTOR_ {
	UINT32  uiFactorH;   ///< block horizontal center factor
	UINT32  uiFactorV;   ///< block vertical center factor
} RHE_CADJ_SUBOUT_CENT_FACTOR;
//------------------------------------

/**
    RHE structure - Contrast adjustment sub-image parameters
*/
typedef struct _RHE_CADJ_SUBOUT_PARAM_ {
	RHE_PARAM_MODE      SubOutParamMode;        ///< contrast adjustment sub-image output parameter mode
	RHE_IMG_SIZE        SubOutBlkSize;          ///< contrast adjustment sub-image output block size
	RHE_CADJ_SUBOUT_CENT_FACTOR SubOutBlkFact;  ///< contrast adjustment sub-image output block center factors
	UINT32              *puiSuboutGamma;       ///< Contrast adjust gamma table, 33 entries, support auto mode
} RHE_CADJ_SUBOUT_PARAM;
//------------------------------------


/**
    RHE structure - Contrast adjustment sub-image output
*/
typedef struct _RHE_CADJ_SUBOUT_INFO_ {
	RHE_FUNC_EN             SubOutEn;       ///< contrast adjustment sub-image output enable
	RHE_IMG_SIZE            SubOutSize;     ///< contrast adjustment sub-image size
	RHE_CADJ_SUBOUT_PARAM   SubOutParam;    ///< contrast adjustment sub-image parameters
	UINT32                  uiSubOutOfs;    ///< contrast adjustment sub-image output lineoffset
	UINT32                  uiSubOutAddr;   ///< contrast adjustment sub-image output DMA Address
} RHE_CADJ_SUBOUT_INFO;
//------------------------------------

/**
    RHE structure - Contrast adjustment parameters
*/
typedef struct _RHE_CADJ_FTR_COEFS_ {
	RHE_PARAM_MODE  FtrParamMode;      ///< contrast adjustment
	UINT32          *uiFtrCoef;  ///< sub-image low pass filter coefficients, 3 entries
} RHE_CADJ_FTR_COEFS;
//------------------------------------

/**
    RHE structure - Contrast adjustment sub-image scale factor
*/
typedef struct _RHE_CADJ_SCL_FTR_ {
	UINT32 uiSclFactorH;    ///< horizontal sub-image scale factor
	UINT32 uiSclFactorV;    ///< vertical sub-image scale factor
} RHE_CADJ_SCL_FTR;
//------------------------------------

/**
    RHE structure - Contrast adjustment division and clamp
*/
typedef struct _RHE_CADJ_DIV_CLAMP_ {
	UINT32 uiDiv;       ///< division
	UINT32 uiClamp;     ///< clamp
} RHE_CADJ_DIV_CLAMP;
//------------------------------------



/**
    RHE structure - Contrast adjustment output coefficients
*/
typedef struct _RHE_CADJ_OUTPUT_COEFS_ {
	UINT32                  *puiCAdjOutputWet;  ///< output weighting
	RHE_CADJ_OUT_REF        CAdjOutWetRef;      ///< output weighting reference
	RHE_FUNC_EN             CAdjRefGmaEn;       ///< contrast adjust output weighting reference gamma enable
} RHE_CADJ_OUTPUT_COEFS;
//------------------------------------

/**
    RHE structure - Contrast adjustment blending
*/
typedef struct _RHE_CADJ_BLEND_ {
	UINT32              *puiCAdjBldRto;     ///< contrast adjustment blending ratio, 9 entries
	RHE_CADJ_BLD_REF    CadjBldRef;         ///< blending reference selection
} RHE_CADJ_BLEND;
//------------------------------------

/**
    RHE structure - Contrast adjustment scale parameters
*/
typedef struct _RHE_CADJ_SCL_INFO_ {
	RHE_PARAM_MODE      CAdjParamMode;      ///< contrast adjustment
	RHE_CADJ_SCL_FTR    CAdjSclFactor;      ///< sub-image scale factor
} RHE_CADJ_SCL_INFO;

/**
    RHE structure - Contrast adjustment enhancing parameters
*/
typedef struct _RHE_CADJ_ENH_INFO_ {
	UINT32              uiCAdjRto;      ///< contrast adjustment ratio
	UINT32              uiCAdjMinDif;   ///< contrast adjustment minimun
	UINT32              uiCAdjGainRto;  ///< contrast adjustment gain ratio
	RHE_CADJ_OPT_SEL    CAdjOpt;        ///< contrast adjustment selection
} RHE_CADJ_ENH_INFO;


/**
    RHE structure - Contrast adjustment parameters
*/
typedef struct _RHE_CADJ_PARAM_ {
	RHE_CADJ_DIV_CLAMP      CAdjMin;            ///< division and clamp of min part
	RHE_CADJ_DIV_CLAMP      CAdjMax;            ///< division and clamp of max part
	RHE_CADJ_ENH_INFO       CAdjEnh;            ///< contrast adjustment enhancing control
	RHE_CADJ_BLEND          CAdjBld;            ///< contrast adjustment blending control
	RHE_CADJ_OUTPUT_COEFS   CAdjOutCoefs;       ///< output coefficients control

	RHE_PARAM_MODE          CAdjParamMode;      ///< contrast adjustment parameter mode, support mod-A, mod-B and gamma tables
	UINT32                  *puiCAdjModLutA;    ///< contrast adjustment mod-A LUT, 9 entries, support auto mode
	UINT32                  *puiCAdjModLutB;    ///< contrast adjustment mod-B LUT, 9 entries, support auto mode
	UINT32                  *puiCAdjGamma;      ///< Contrast adjust gamma table, 33 entries, support auto mode
} RHE_CADJ_PARAM;
//------------------------------------



/**
    RHE structure - Contrast adjustment setting info
*/
typedef struct _RHE_CADJ_INFO_ {
	RHE_FUNC_EN         CAdjEn;             ///< contrast adjustment enable
	RHE_IMG_SIZE        CAdjSubImgSize;     ///< contrast adjustment sub-in image size
	RHE_CADJ_PARAM      CAdjParam;          ///< contrast adjustment parameters
	RHE_CADJ_FTR_COEFS  CAdjFilter;         ///< contrast adjustment sub-in filter coefficients
	RHE_CADJ_SCL_INFO   CAdjSclFtr;         ///< contrast adjustment scale factor
	UINT32              uiCAdjSubImgOfs;    ///< contrast adjustment sub-image output lineoffset
	UINT32              uiCAdjSubInAddr;   ///< contrast adjustment  sub-in image buffer address

} RHE_CADJ_INFO;
//------------------------------------


/**
    RHE structure - Dark frame subtraction setting info
*/
typedef struct {
	UINT32              uiDpTh;     ///< Defect upper threshold
	RHE_DFS_DP_SRC_SEL  DpSrc;      ///< Defect thresholding source
	RHE_DFS_OP_SEL      DfsOp;      ///< Operation type
	RHE_DEF_FTR_SEL     DefFtr;     ///< Defect concealment cross-channel factor
} RHE_DFS_INFO;

/**
    RHE structure - flip function enable
*/
typedef struct _RHE_FLIP_INFO_ {
	RHE_FUNC_EN HFlipEn;        ///< horizontal flip enable
	RHE_FUNC_EN VFlipEn;        ///< vertical flip enable
}  RHE_FLIP_INFO;
//------------------------------------

/**
    RHE structure - Flicker detection parameters
*/
typedef struct _RHE_FLICKER_DET_PARAM_ {
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
} RHE_FLICKER_DET_PARAM;


typedef struct _RHE_FLICKER_DETECT_RSLT_INFO_ {
	UINT32      uiIn0Rslt[16];  ///< flicker detection result for input-0
	UINT32      uiIn1Rslt[16];  ///< flicker detection result for input-1
} RHE_FLICKER_DETECT_RSLT_INFO;

/**
    RHE structure - Flicker detection setting info.
*/
typedef struct _RHE_FLICKER_DET_INFO_ {
	RHE_FUNC_EN                 FlkEn;      ///< flicker detection enable
	RHE_FLICKER_DET_PARAM    FlkParam;   ///< flicker parameters
} RHE_FLICKER_DET_INFO;
//------------------------------------


/**
    RHE structure - Decoder random init setting
*/
typedef struct _RHE_DEC_RAND_INIT_ {
	UINT32 uiRandInit0;     ///<  Decoder random sequence init setting 0
	UINT32 uiRandInit1;     ///<  Decoder random sequence init setting 1
}  RHE_DEC_RAND_INIT;
//------------------------------------

/**
    RHE structure - Decoder random init setting of 4 sets
*/
typedef struct _RHE_DEC_INIT_ {
	UINT32              SetChl;             ///< desired setting channel, SetChl = (RHE_INPATH_1 | RHE_INPATH_2)
	RHE_DEC_RAND_INIT   InDecRandInit0;     ///< decoder initial value of set0
	RHE_DEC_RAND_INIT   InDecRandInit1;     ///< decoder initial value of set1
	RHE_DEC_RAND_INIT   InDecRandInit2;     ///< decoder initial value of set2
	RHE_DEC_RAND_INIT   InDecRandInit3;     ///< decoder initial value of set3
}  RHE_DEC_INIT;
//------------------------------------

/**
    RHE structure - Decoder gamma parameters
*/
typedef struct _RHE_DEC_GAMMA_PARAM_ {
	RHE_FUNC_EN         DecGammaEn;        ///< input decoder gamma enable
	UINT32              *puiDecGamma;      ///< input decoder gamma, 33 elements
} RHE_DEC_GAMMA_PARAM;
//------------------------------------

/**
    RHE structure - Decoder parameters
*/
typedef struct _RHE_DECODER_PARAM_ {
	RHE_PARAM_MODE          DecParamMode;      ///< input decoder parameter mode
	RHE_DEC_STP_MODE        DceStpMode;        ///< input decoder stripe mode
	RHE_DEC_PADDING_MODE    DcePadMode;        ///< input decoder output LSB padding mode
	RHE_DEC_INIT            DecRandInit;       ///< input decoder random init value, support auto mode
	RHE_DEC_VLC_ENTS        DecVlcEnts;        ///< input decoder VLC entries, support auto mode
	UINT32                  *puiDecCSBTab;     ///< input decoder CSB table, max: 24 entries, support auto mode
	UINT32                  *puiDecLFNTab;     ///< input decoder LFN table, max: 24 entries, support auto mode
	RHE_DEC_GAMMA_PARAM     DecGamma;          ///< input decoder de-gamma info.
}  RHE_DECODER_PARAM;
//------------------------------------


/**
    RHE structure - Decoder setting info.
*/
typedef struct _RHE_DECODER_INFO_ {
	RHE_FUNC_EN         DecEn;             ///< input decoder enable
	RHE_DECODER_PARAM   DecParam;       ///< input decoder parameters
	RHE_IN_IMG_ADDR     DecHeaderAddr;     ///< input decoder header DMA address
}  RHE_DECODER_INFO;
//------------------------------------

/**
    RHE structure - input image setting info.
*/
typedef struct _RHE_IN_IMG_INFO_ {
	RHE_IMG_SIZE        InImgSize;  ///< input image size
	RHE_IN_IMG_LOFS     InImgLofs;     ///< input image lineoffset
	RHE_IN_IMG_ADDR     InImgAddr;     ///< input image DMA address
	RHE_IMG_SIZE        ImgCropSize;  ///< input image crop size
	RHE_IMG_CROP_INFO   ImgCropPos;     ///< input image crop position
}  RHE_IN_IMG_INFO;
//------------------------------------

/**
    RHE structure - input sub-image setting info.
*/
typedef struct _RHE_IN_SUB_IMG_INFO_ {
	RHE_IMG_SIZE        InSubImgSize;       ///< input sub-image size
	RHE_IN_IMG_LOFS     InSubImgLofs;       ///< input sub-image lineoffset
	RHE_IN_IMG_ADDR     InSubImgAddr;       ///< input sub-image DMA address
}  RHE_IN_SUB_IMG_INFO;
//------------------------------------

/**
    RHE structure - output image setting info.
*/
typedef struct _RHE_OUT_IMG_INFO_ {
	UINT32 uiLofs;  // output image lineoffest
	UINT32 uiAddr;  // output image DMA address
}  RHE_OUT_IMG_INFO;
//------------------------------------

/**
    RHE structure - image data pattern
*/
typedef struct _RHE_IMG_PAT_INFO_ {
	RHE_BAYER_FMT   ByrFmt;     ///< bayer format
	RHE_CFA_PAT_SEL CfaPat;     ///< byaer pattern
}  RHE_IMG_PAT_INFO;
//------------------------------------

/**
    RHE structure - input and output data packed
*/
typedef struct _RHE_IOPACKED_INFO_ {
	RHE_PACKBUS_SEL  InPackedBit;     ///< input data packed, 0: 8bit, 1: 10bit, 2: 12bit, 3:16bit
	RHE_PACKBUS_SEL  OutPackedBit;     ///< output data packed, only 12bit
}  RHE_IOPACKED_INFO;
//------------------------------------


/**
    RHE structure - Multi-frame blending parameters
*/
typedef struct _RHE_PATH_CRUVE_PARAM_ {
	RHE_PARAM_MODE      PathCurveParamMode;    ///< path curve parameter mode

	UINT32              *puiCurveA;      ///< curve of path A, 33 entries, support auto mode
	UINT32              *puiCurveB;      ///< curve of path B, 33 entries, support auto mode
	UINT32              *puiCurveC;      ///< curve of path C, 33 entries, support auto mode
	UINT32              *puiCurveD;      ///< curve of path D, 33 entries, support auto mode
	UINT32              *puiCurveE;      ///< curve of path E, 33 entries, support auto mode

	UINT32              *puiWetA;      ///< weight of path A, 17 entries, support auto mode
	UINT32              *puiWetB;      ///< weight of path B, 17 entries, support auto mode
	UINT32              *puiWetC;      ///< weight of path C, 17 entries, support auto mode
} RHE_PATH_CRUVE_PARAM;
//------------------------------------

/**
*/
typedef struct _RHE_FUNC_CTRL_PARAM_ {
	RHE_FUNC_MODE           FuncMode;           ///< Function mode
	RHE_HDR_FRAME_MODE      HdrFrmMode;         ///< Sensor HDR frame mode
	RHE_HDR_EVGAP_SEL       HdrFrmGaps;         ///< Sensor HDR EV gap of frame
	RHE_PATH_CRUVE_PARAM    PathCurveInfo;      ///< path curve parameter setting info.
} RHE_FUNC_CTRL_PARAM;


/**
    RHE structure - engine parameters
*/
typedef struct _RHE_MODE_PARAM_ {
	RHE_OPMODE              OpMode;             ///< Operation mode
	RHE_FUNC_MODE           FuncMode;           ///< Function mode
	RHE_HDR_FRAME_MODE      HdrFrmMode;         ///< Sensor HDR frame mode
	RHE_HDR_EVGAP_SEL       HdrFrmGaps;         ///< Sensor HDR EV gap of frame

	RHE_IN_IMG_INFO         InImgInfo;          ///< input main image parameter setting info.
	RHE_IMG_PAT_INFO        InImgPat;           ///< input main image pattern
	RHE_IN_SUB_IMG_INFO     InSubImgInfo;       ///< input sub image parameter setting info.
	RHE_SCALE_COEF_INFO     InSubImgSclInfo;    ///< input sub-image scaling parameter setting info.
	RHE_OUT_IMG_INFO        OutImgInfo;         ///< output main image parameter setting info.
	RHE_IOPACKED_INFO       IOPacked;           ///< input and output data packed-bits, if IPP mode, 12bit only for both input and output
	RHE_PATH_CRUVE_PARAM    PathCurveInfo;      ///< path curve parameter setting info.

	UINT32                  uiInterruptEn;      ///< interrupt enable setting, uiInterruptEn = (RHE_INT_FRAMEEND | RHE_INT_DEC1_ERR)
	RHE_BURST_SEL           InBurst;           ///< output burst length

	RHE_DECODER_INFO        *pDecInfo;          ///< input decoder parameter setting info., if useless, please set NULL
	RHE_COLOR_GAIN_INFO     *pCGainInfo;        ///< color gain, if useless, please set NULL
	RHE_FTR_INFO            *pSubImgFtrInfo;    ///< input sub-image filter, if useless, please set NULL
	RHE_BLEND_INFO          *pBldInfo;          ///< multi-frame blending, if useless, please set NULL
	RHE_LTC_INFO            *pLtcInfo;          ///< local contrast enhancement, if useless, please set NULL
	RHE_TONE_CRUVE_INFO     *pToneCurveInfo;    ///< tone curve, if useless, please set NULL
	RHE_CSAT_INFO           *pCSatInfo;         ///< color saturation control, if useless, please set NULL
	RHE_CNS_INFO            *pCnsInfo;          ///< color noise suppression for dark region, if useless, please set NULL
	RHE_CADJ_SUBOUT_INFO    *pCAdjSuboutInfo;   ///< contrast sub-out image control, if useless, please set NULL
	RHE_CADJ_INFO           *pCAdjInfo;         ///< contrast adjustment, if useless, please set NULL
	RHE_DFS_INFO            *pDfsInfo;          ///< dark frame substraction, if useless, please set NULL
	RHE_FLICKER_DET_INFO    *pFlkDetInfo;       ///< flicker detection, if useless, please set NULL
	RHE_FLIP_INFO           *pFlipInfo;         ///< flip control, if useless, please set NULL
}  RHE_MODE_PARAM;
//-----------------------------------------------------------------------


/**
RHE API

@name   RHE_API
*/
//@{


extern VOID rhe_isr(VOID);

/**
    RHE Open Driver

    Open RHE engine

    @param[in] pObjCB RHE open object

    @return
        - @b E_OK: Done with no error.
        - Others: Error occured.
*/
extern ER rhe_open(RHE_OPENOBJ *pObjCB);

/**
    RHE Get Open Status

    Check if RHE engine is opened

    @return
        - @b FALSE: engine is not opened
        - @b TRUE: engine is opened
*/
extern BOOL rhe_isOpened(VOID);
//-----------------------------------------------------

/**
    RHE Close Driver

    Close RHE engine

    @return
        - @b E_OK: Done with no error.
        - Others: Error occured.
*/
extern ER rhe_close(VOID);
//-----------------------------------------------------

/**
    RHE Pause Operation

    Pause RHE engine

    @return
        - @b E_OK: Done with no error.
        - Others: Error occured.
*/
extern ER rhe_pause(VOID);
//-----------------------------------------------------

/**
    RHE Start Operation

    Start RHE engine

    @return
        - @b E_OK: Done with no error.
        - Others: Error occured.
*/
extern ER rhe_start(VOID);
//-----------------------------------------------------

/**
    RHE Set Mode

    Set RHE engine mode

    @param[in] pRheParam RHE configuration.

    @return
        - @b E_OK: Done with no error.
        - Others: Error occured.
*/
extern ER rhe_setMode(RHE_MODE_PARAM *pRheParam);
//-----------------------------------------------------

extern ER emu_rhe_setMode(RHE_MODE_PARAM *pRheParam);
//-----------------------------------------------------

/**
    RHE Wait Frame-end flag

    Wait for frame end flag.

    @param[in] IsClearFlag clearing flag or not after frame-end flag

    @return None.
*/
extern VOID rhe_waitFlagFrameEnd(RHE_FLAG_CLEAR_SEL IsClearFlag);
//-----------------------------------------------------

/**
    RHE clear frame-end flag

    clear frame-end flag

    @param[in] VOID

    @return VOID.
*/
extern VOID rhe_clearFlagFrameEnd(VOID);
//-----------------------------------------------------
// parameter change API

/**
    RHE change input image size

    @param[in] pChgInImgSize  input image size

    @return ER
*/
extern ER rhe_chgInputImageSize(RHE_IMG_SIZE *pChgInImgSize);
//-----------------------------------------------------

/**
    RHE change input image lineoffset

    @param[in] pChgLofs  input image lineoffset

    @return ER
*/
extern ER rhe_chgInputImageLineoffset(RHE_IN_IMG_LOFS *pChgLofs);
//-----------------------------------------------------

/**
    RHE change input image buffer DMA address

    @param[in] pInAddr  input buffer information

    @return ER
*/
extern ER rhe_chgInputImageDMAAddr(RHE_IN_IMG_ADDR *pChgInAddr);
//-----------------------------------------------------

/**
    RHE change input sub-image size

    @param[in] pChgInSubImgSize  input sub-image size

    @return ER
*/
extern ER rhe_chgInputSubImageSize(RHE_IMG_SIZE *pChgInSubImgSize);
//-----------------------------------------------------


/**
    RHE change input sub_image lineoffset

    @param[in] pChgLofs  input sub-image lineoffset

    @return ER
*/
extern ER rhe_chgInputSubImageLineoffset(RHE_IN_IMG_LOFS *pChgLofs);
//-----------------------------------------------------

/**
    RHE change input sub-image buffer DMA address

    @param[in] pInAddr  input buffer information

    @return ER
*/
extern ER rhe_chgInputSubImageDMAAddr(RHE_IN_IMG_ADDR *pChgAddr);
//-----------------------------------------------------

/**
    RHE change sub-image scale factors

    @param[in] pChgFtr  scale factors

    @return ER
*/
extern ER rhe_chgSubImageScaleFactor(RHE_SCALE_COEF_INFO *pChgFtr);
//-----------------------------------------------------

/**
    RHE change output image lineoffset

    @param[in] uiAddr  output image lineoffset

    @return ER
*/
extern ER rhe_chgOutputImageLineoffset(UINT32 uiAddr);
//-----------------------------------------------------

/**
    RHE change output buffer DMA address

    @param[in] uiChgAddr  output buffer information

    @return ER
*/
extern ER rhe_chgOutputImageDMAAddr(UINT32 uiChgAddr);
//-----------------------------------------------------



extern ER rhe_chgDecoderEnable(RHE_FUNC_EN SetEn);

/**
    RHE change decoder parameter

    @param[in] pDecParam decoder parameter for setting

    @return ER  E_OK: setting done
                 Others: setting error
*/
extern ER rhe_chgDecoderParam(RHE_DECODER_PARAM *pChgParam);
//-----------------------------------------------------

/**
    RHE change decoder header info. address

    @param[in] pChgAddr  decoder header info. address

    @return ER  E_OK: setting done
                 Others: setting error
*/
extern ER rhe_chgDecoderHeaderDMAAddr(RHE_IN_IMG_ADDR *pChgAddr);
//-----------------------------------------------------


/**
    RHE change color gain enable

    @param[in] SetEn  enable / disable

    @return ER  E_OK: setting done
                 Others: setting error
*/
extern ER rhe_chgColorGainEnable(RHE_FUNC_EN SetEn);
//-----------------------------------------------------

/**
    RHE change color gain parameters

    @param[in] pChgParam  color gain parameters

    @return ER  E_OK: setting done
                 Others: setting error
*/
extern ER rhe_chgColorGainParam(RHE_COLOR_GAIN_PARAM *pChgParam);
//-----------------------------------------------------

/**
    RHE change color offset parameters

    @param[in] pChgParam  offset gain parameters

    @return ER  E_OK: setting done
                 Others: setting error
*/
extern ER rhe_chgColorOffsetParam(RHE_COLOR_OFS_PARAM *pChgParam);
//-----------------------------------------------------

/**
    RHE change sub-image path filter enable

    @param[in] SetEn  enable / disable

    @return ER  E_OK: setting done
                 Others: setting error
*/
extern ER rhe_chgSubFilterEnable(RHE_FUNC_EN SetEn);
//-----------------------------------------------------

/**
    RHE change sub-image path filter parameters

    @param[in] pChgParam  sub-image path filter parameters

    @return ER  E_OK: setting done
                 Others: setting error
*/
extern ER rhe_chgSubFilterParam(RHE_FTR_PARAM *pChgParam);
//-----------------------------------------------------

/**
    RHE change image blending enable

    @param[in] SetEn  enable / disable

    @return ER  E_OK: setting done
                 Others: setting error
*/
extern ER rhe_chgBlendEnable(RHE_FUNC_EN SetEn);
//-----------------------------------------------------


/**
    RHE change image blending parameters

    @param[in] pChgParam  image blending parameters

    @return ER  E_OK: setting done
                 Others: setting error
*/
extern ER rhe_chgBlendParam(RHE_BLEND_PARAM *pChgParam);
//-----------------------------------------------------

/**
    RHE change path curve parameter

    @param[in] PathSel  path selection
    @param[in] puiCurve  curve parameter, 17 entries

    @return ER  E_OK: setting done
                 Others: setting error
*/
extern ER rhe_chgPathCurveParam(RHE_PARAM_MODE ModeSel, RHE_PATH_CURVE_SEL PathSel, UINT32 *puiCurve);
//-----------------------------------------------------


/**
    RHE change path weight parameter

    @param[in] PathSel  path selection
    @param[in] puiCurve  weight parameter, 17 entries

    @return ER  E_OK: setting done
                 Others: setting error
*/
extern ER rhe_chgPathWeightParam(RHE_PARAM_MODE ModeSel, RHE_PATH_WEIGHT_SEL PathSel, UINT32 *puiWeight);
//-----------------------------------------------------



/**
    RHE change local contrast enhancement enable

    @param[in] SetEn  enable / disable

    @return ER  E_OK: setting done
                 Others: setting error
*/
extern ER rhe_chgLocalContrastEnhEnable(RHE_FUNC_EN SetEn);
//-----------------------------------------------------

/**
    RHE change local contrast enhancement parameters

    @param[in] pChgParam  local contrast enhancement parameters

    @return ER  E_OK: setting done
                 Others: setting error
*/
extern ER rhe_chgLocalContrastEnhParam(RHE_LTC_PARAM *pChgParam);
//-----------------------------------------------------


/**
    RHE change tone curve enable

    @param[in] SetEn  enable / disable

    @return ER  E_OK: setting done
                 Others: setting error
*/
extern ER rhe_chgToneCurveEnable(RHE_FUNC_EN SetEn);
//-----------------------------------------------------

/**
    RHE change tone curve parameters

    @param[in] pChgParam  tone curve parameters

    @return ER  E_OK: setting done
                 Others: setting error
*/
extern ER rhe_chgToneCurveParam(RHE_TONE_CURVE_PARAM *pChgParam);
//-----------------------------------------------------

/**
    RHE change tone curve input address

    @param[in] uiChgAddr  tone curve input address

    @return ER  E_OK: setting done
                 Others: setting error
*/
extern ER rhe_chgToneCurveAddr(UINT32 uiChgAddr);
//-----------------------------------------------------

/**
    RHE change tone curve data to DMA buffer

    @param[in] uiAddr  		tone curve DMA buffer address
    @param[in] puiSetCurve  tone curve table

    @return ER  E_OK: setting done
                 Others: setting error
*/
extern ER rhe_chgToneCurveToDram(UINT32 uiAddr, UINT32 *puiSetCurve);
//-----------------------------------------------------



/**
    RHE change color saturation enable

    @param[in] SetEn  enable / disable

    @return ER  E_OK: setting done
                 Others: setting error
*/
extern ER rhe_chgColorSatEnable(RHE_FUNC_EN SetEn);
//-----------------------------------------------------

/**
    RHE change color saturation parameters

    @param[in] pChgParam  color saturation parameters

    @return ER  E_OK: setting done
                 Others: setting error
*/
extern ER rhe_chgColorSatParam(RHE_CSAT_PARAM *pChgParam);
//-----------------------------------------------------


/**
    RHE change color noise suppress enable

    @param[in] SetEn  enable / disable

    @return ER  E_OK: setting done
                 Others: setting error
*/
extern ER rhe_chgColorNoiseSuppressEnable(RHE_FUNC_EN SetEn);
//-----------------------------------------------------

/**
    RHE change color noise suppress enable

    @param[in] pChgParam  color noise suppress paramters

    @return ER  E_OK: setting done
                 Others: setting error
*/
extern ER rhe_chgColorNoiseSuppressParam(RHE_CNS_PARAM *pChgParam);
//-----------------------------------------------------


/**
    RHE change contrast subout image enable

    @param[in] SetEn  enable / disable

    @return ER  E_OK: setting done
                 Others: setting error
*/
extern ER rhe_chgCAdjSuboutEnable(RHE_FUNC_EN SetEn);
//-----------------------------------------------------

/**
    RHE change contrast subout image size

    @param[in] pChgSize  subout image size

    @return ER  E_OK: setting done
                 Others: setting error
*/
extern ER rhe_chgCAdjSuboutSize(RHE_IMG_SIZE *pChgSize);
//-----------------------------------------------------

/**
    RHE change contrast subout image parameters

    @param[in] pChgParam  contrast subout image parameters

    @return ER  E_OK: setting done
                 Others: setting error
*/
extern ER rhe_chgCAdjSuboutParam(RHE_CADJ_SUBOUT_PARAM *pChgParam);
//-----------------------------------------------------

/**
    RHE change contrast subout image lineoffset

    @param[in] uiChgLofs  contrast subout image lineoffset

    @return ER  E_OK: setting done
                 Others: setting error
*/
extern ER rhe_chgCAdjSuboutLineoffset(UINT32 uiChgLofs);
//-----------------------------------------------------

/**
    RHE change contrast subout image buffer address

    @param[in] uiChgAddr  contrast subout image buffer address

    @return ER  E_OK: setting done
                 Others: setting error
*/
extern ER rhe_chgCAdjSuboutAddr(UINT32 uiChgAddr);
//-----------------------------------------------------

/**
    RHE change contrast adjustment enable

    @param[in] SetEn  enable / disable

    @return ER  E_OK: setting done
                 Others: setting error
*/
extern ER rhe_chgCAdjEnable(RHE_FUNC_EN SetEn);
//-----------------------------------------------------

/**
    RHE change contrast adjustment parameters

    @param[in] pChgParam  contrast adjustment parameters

    @return ER  E_OK: setting done
                 Others: setting error
*/
extern ER rhe_chgCAdjParam(RHE_CADJ_PARAM *pChgParam);
//-----------------------------------------------------

/**
    RHE change contrast adjustment sub-in image buffer address

    @param[in] uiChgAddr  sub-in image buffer address

    @return ER  E_OK: setting done
                 Others: setting error
*/
extern ER rhe_chgCAdjSubinAddr(UINT32 uiChgAddr);
//-----------------------------------------------------

/**
    RHE change contrast adjustment sub-in image scale factor

    @param[in] uiChgAddr  contrast adjustment sub-in image scale factor

    @return ER  E_OK: setting done
                 Others: setting error
*/
extern ER rhe_chgCAdjScaleFactor(RHE_CADJ_SCL_INFO *pChgFtr);
//-----------------------------------------------------

/**
    RHE change contrast adjustment sub-in filter coefficients

    @param[in] pChgCoef  contrast adjustment sub-in filter coefficients

    @return ER  E_OK: setting done
                 Others: setting error
*/
extern ER rhe_chgCAdjSubinFilterCoef(RHE_CADJ_FTR_COEFS *pChgCoef);
//-----------------------------------------------------


extern ER rhe_chgDarkFrameSubParam(RHE_DFS_INFO *pChgParam);
//-----------------------------------------------------

/**
    RHE change flicker detection enable

    @param[in] SetEn  enable / disable

    @return ER  E_OK: setting done
                 Others: setting error
*/
extern ER rhe_chgFlickerDetEnable(RHE_FUNC_EN SetEn);
//-----------------------------------------------------

/**
    RHE change flicker detection parameters

    @param[in] pChgParam  flicker detection parameters

    @return ER  E_OK: setting done
                 Others: setting error
*/
extern ER rhe_chgFlickerDetParam(RHE_FLICKER_DET_PARAM *pChgParam);
//-----------------------------------------------------

/**
    RHE change horizontal flip paramerers

    @param[in] SetEn  function enable/disable

    @return ER  E_OK: setting done
                 Others: setting error
*/
extern ER rhe_chgHorizontalFlipEnable(RHE_FUNC_EN SetEn);
//-----------------------------------------------------

/**
    RHE change vertical flip paramerers

    @param[in] SetEn  function enable/disable

    @return ER  E_OK: setting done
                 Others: setting error
*/
extern ER rhe_chgVerticalFlipEnable(RHE_FUNC_EN SetEn);
//-----------------------------------------------------

/**
    RHE by pass all function

    @param[in] SetEn  function enable/disable

    @return ER  E_OK: setting done
                 Others: setting error
*/
extern ER rhe_chgByPassMode(RHE_FUNC_EN SetEn);
//-----------------------------------------------------

/**
    RHE change decoder source

    @param[in] DecSrc  source selection

    @return ER  E_OK: setting done
                 Others: setting error
*/
extern ER rhe_chgDecoderSource(RHE_DEC_SRC_SEL DecSrc);
//-----------------------------------------------------

/**
    RHE Change All Parameters

    Change RHE All Parameters

    @param[in] pRheParam RHE configuration.

    @return
        - @b E_OK: Done with no error.
        - Others: Error occured.
*/
extern ER rhe_chgAllParam(RHE_MODE_PARAM *pRheParam);
//-----------------------------------------------------

/**
    RHE Change Function Control Mode

    @param[in] pChgParam  RHE function control mode parameters

    @return
        - @b E_OK: Done with no error.
        - Others: Error occured.
*/
extern ER rhe_chgFuncCtrlMode(RHE_FUNC_CTRL_PARAM *pChgParam);
//-----------------------------------------------------


/**
    RHE Change Bayer Pattern



    @param[in] pChgParam        Bayer pattern parameters.

    @return
        - @b E_OK: Done with no error.
        - Others: Error occured.
*/
extern ER rhe_chgBayerPatParam(RHE_IMG_PAT_INFO *pChgParam);
//-----------------------------------------------------

/**
    RHE Change Input Image Crop Parameters

    @param[in] pImgCropSize     Image crop size
    @param[in] pImgCropPos      Image crop position

    @return
        - @b E_OK: Done with no error.
        - Others: Error occured.
*/
extern ER rhe_chgInputImageCropParam(RHE_IMG_SIZE *pImgCropSize, RHE_IMG_CROP_INFO *pImgCropPos);
//-----------------------------------------------------



//-----------------------------------------------------
// parameter get API
extern VOID rhe_getInputImageSize(RHE_IMG_SIZE *pGetSize);
extern VOID rhe_getInputImageLineoffset(RHE_IN_IMG_LOFS *pGetLofs);
extern VOID rhe_getInputSubImageLineoffset(RHE_IN_IMG_LOFS *pGetLofs);
extern UINT32 rhe_getClockRate(VOID);
extern VOID rhe_getBurstLength(RHE_BURST_SEL *pBurstLen);




#endif
//@}

//@}


