/**
    IPH module (DCE sub function type define)

    @file       ipl_hal_dce_info.h NT96660
    @ingroup    mILibIPH

    Copyright   Novatek Microelectronics Corp. 2011.  All rights reserved.
*/
#ifndef _IPL_Hal_DCE_Info_H
#define _IPL_Hal_DCE_Info_H
#include "Type.h"
#include "ipl_utility.h"
/** \addtogroup mILibIPH */
//@{

/**
    engine clk
*/
typedef enum _IPH_DCECLK {
	IPH_DCECLK0 = 240,  ///< -240 mhz
	IPH_DCECLK1 = 480,  ///< -480 mhz
	ENUM_DUMMY4WORD(IPH_DCECLK)
} IPH_DCECLK;

/**
    DCE Input Data Source
*/
typedef enum _DCE_SRCMODE {
	DCE_IPP     = 0,    ///< RHE + IFE->DCE->IPE->IME(IFE2)
	DCE_SIE_IPP = 2,    ///< SIE + IFE->DCE->IPE->IME(IFE2) (all direct)
	DCE_D2D     = 3,    ///< DCE only
	ENUM_DUMMY4WORD(DCE_SRCMODE)
} DCE_SRCMODE;

/**
    Radius factor
*/
typedef struct _Fact_Norm {
	BOOL    Manual;     ///< if Manual is FALSE, Factor and Bit will be calculated automatically.
	UINT32  Factor;     ///< normalize factor
	UINT32  Bit;        ///< normalize bit
} Fact_Norm;

/**
    DCE input bayer CFA
*/
typedef IPL_RAW_PIX DCE_CFA;
/**
    CFA interpolation parameter set
*/
typedef struct _DCE_HAL_CFA_HFC {
	BOOL      bCLCheckEn;   ///< Enable cross line check
	BOOL      bHFCheckEn;   ///< Enable high frequency check
	BOOL      bAverageMode; ///< Interpolation method selection when flat region detect
	UINT32    uiHFGTh;      ///< G channel smoothness checking threshold
	UINT32    uiCLTh;       ///< Cross line checking threshold
	UINT32    uiHFDiff;     ///< Difference threshold value between Gr and Gb to determing high frequency pattern detected or not
	UINT32    uiHFEThr;     ///< Threshold to determine H or V direction high frequency pattern detected
} DCE_HAL_CFA_HFC;

typedef struct _DCE_CFAPARAM {
	UINT32  NsmarEdge;      ///< Noise margin for edge
	UINT32  NsmarDiff;      ///< Noise margin for pixel difference
	UINT32  DIFNormBit;     ///< Legal range : 0~3, Normalize bit of difference decision
	struct {
		BOOL    En;         ///< Enable for G channel correction
		UINT32  NormBit;    ///< G correction norm bit
		UINT32  CorrTh;     ///< G correction noise threshold
	} GC;                   ///< G channel correction
	struct {
		BOOL    En;         ///< Enable for RB channel correction
		UINT32  NormBit;    ///< RB correction norm bit
		UINT32  CorrTh1;    ///< RB correction noise threshold
		UINT32  CorrTh2;    ///< RB correction noise threshold
	} RBC;                  ///< RB channel correction
	DCE_HAL_CFA_HFC hfc;    ///< CFA high frequency check term
} DCE_CFAPARAM;
/**
    sie interrupt enable
*/
typedef enum _DceInteEn {
	DCE__CLR        = 0,
	DCE__INTE_FMS   = 0x00000001,   ///< frame start interrupt
	DCE__INTE_FMD   = 0X00000002,   ///< frame end interrupt
	DCE__INTE_STRPE = 0x00000004,   ///< current stripe end interrupt
	DCE__INT_STPERR = 0x00000008,   ///< Interrupt for DCE stripe error
	DCE__INT_LBOVF  = 0x00000010,   ///< Interrupt for DCE line buffer overflow error
	DCE__INT_CACOVF = 0x00000020,   ///< Interrupt for DCE cac buffer overflow error
	DCE__INT_STPOB  = 0x00000040,   ///< Interrupt for DCE stripe boundary overflow error
	DCE__INT_YBACK  = 0x00000080,   ///< Interrupt for DCE Y coordinate backward skip error
	ENUM_DUMMY4WORD(DceInteEn)
} DceInteEn;
/**
    DCE sub function enable enum
*/
typedef enum _DceFuncEn {
	FUNC_CFA        = 0x00000001,   ///< CFA interpolation function
	FUNC_DC         = 0x00000002,   ///< Distortion correction function
	FUNC_CAC        = 0x00000004,   ///< Chromatic aberration correction function
	FUNC_D2D_RAND   = 0x00000008,   ///< D2D random LSB function for verifcation
	FUNC_D2D_UVFILT = 0x00000010,   ///< D2D UV filter function
	FUNC_CROP       = 0x00000020,   ///< Output crop function
	FUNC_D2D_IOSTOP = 0x00000040,   ///< D2D In/Out Dram Stop
	ENUM_DUMMY4WORD(DceFuncEn)
} DceFuncEn;
typedef enum {
	FOV_BOUND_DUPLICATE = 0,        ///< Replace out of boundary pixels with duplicate boundary pixels
	FOV_BOUND_REG_RGB   = 1,        ///< Replace out of boundary pixels with GEO_BOUNDR, GEO_BOUNDG, GEO_BOUNDB
	ENUM_DUMMY4WORD(FOV_SELECT)
} FOV_SELECT;
/**
    Field of view
*/
typedef struct _FOV {
	UINT32  Gain;       ///< FOV lut gain
	FOV_SELECT FOV_SEL;
	struct {
		UINT32  R;      ///< boundary pixel of R channel,(enable when user mode enabled)
		UINT32  G;      ///< boundary pixel of G channel
		UINT32  B;      ///< boundary pixel of B channel
	} BoundryProcess;
} FOV;                  ///< Field of view setting

/**
    Geometric enhancement
*/
typedef struct _EnH {
	UINT32  Factor_Y;   ///< factor
	UINT32  Norm_Y;     ///< normalize bit
	UINT32  Factor_UV;  ///< factor
	UINT32  Norm_UV;    ///< normalize bit
} EnH;

typedef enum {
	CAC_RGB_GAIN = 0,   ///< Correct CA with G LUT and R, G, B LUTGAIN
	CAC_RGB_LUT  = 1,   ///< Correct CA with R, G, B LUT
	ENUM_DUMMY4WORD(CAC_SEL)
} CAC_SEL;
/**
    Color aberration correction factor
*/
typedef struct _Aberation {
	CAC_SEL Cacsel;
	struct {
		UINT32  Fact_R;     ///<  R channel lut gain
		UINT32  Fact_G;     ///<  G channel lut gain
		UINT32  Fact_B;     ///<  R channel lut gain
	} CACGain;
} Aberation;

/**
    horizontal/vertical distance factors and distortion selection
*/
typedef struct _DIST {
	UINT32  Fact_X;         ///< X input distance factor for oval shape modeling
	UINT32  Fact_Y;         ///< Y input distance factor for oval shape modeling
} DIST;

//@}

/**
    @name DCE struct - 2D lut offset
*/
//@{
typedef struct {
	UINT32    XOfsInt;   ///< 2D lut x integer offset
	UINT32    XOfsFrac;  ///< 2D lut x fraction offset
	UINT32    YOfsInt;   ///< 2D lut y integer offset
	UINT32    YOfsFrac;  ///< 2D lut y fraction offset
} _2DLUT_OFFSET;
//@}
/**
    DCE 2D lut number selection

    Select 2D lut number.
\n Used for dce_set2DlutNum().
*/
//@}
typedef enum {
	_2DLUT_65_65_GRID = 0,         ///< 65x65 grid 2D lut distortion correction
	_2DLUT_33_33_GRID = 1,         ///< 33x33 grid 2D lut distortion correction
	_2DLUT_17_17_GRID = 2,         ///< 17x17 grid 2D lut distortion correction
	_2DLUT_9_9_GRID   = 3,         ///< 9x9 grid 2D lut distortion correction
	ENUM_DUMMY4WORD(_2DLUT_NUM)
} _2DLUT_NUM;
/**
    DCE structure - DCE 2D lut parameters.
*/
//@{
typedef struct {
	UINT32 _2DLutAddr;               ///< 2D lut DRAM address
	_2DLUT_OFFSET _2DLutXYOfs;      ///< 2D lut x y offset
	_2DLUT_NUM _2DLutNum;           ///< 2D lut grid number
} _2DLUT_PARAM;
//@}
/**
    distortion compensation
*/
typedef struct _DCE_Distortion {
	Coordinate  Center;         ///< Distortion center
	FOV         Fov;            ///< Field of view setting
	EnH         Enh;            ///< Geometric enhancement term
	Aberation   ColorAber;      ///< Color aberration correction factor setting
	struct {
		UINT32 *pGGeoLut;           ///< GDC/CAC G geo lut(tab num:65, Data type:UINT32)
		INT32 *pRGeoLut;            ///< GDC/CAC R geo lut(tab num:65, Data type:INT32)
		INT32 *pBGeoLut;            ///< GDC/CAC B geo lut(tab num:65, Data type:INT32)
	} GeoLUT;
	BOOL        UV_FilterEn;     ///< ///< Horizontal filter for UV channel
} DCE_Distortion;

/**
     DCE sub function configuration.
*/
typedef struct _DCE_SUBFUNC {
	DceInteEn   dceInteEn;              ///< interrupt enable
	UINT32      dceFuncEn;              ///< Function enable selection
	DIST        Dce_Dist;               ///< Distance configuration
	Fact_Norm   Radious;                ///< GEO normaliztion factor and shift bit setting
	DCE_Distortion  *pDistortion;       ///< distortion configuration
	DCE_CFAPARAM    *pCFAInterpolation; ///< CFA interpolation
	_2DLUT_PARAM     *_2DLutParam;      ///< 2D lut parameters
} DCE_SUBFUNC;
//@}
#endif
