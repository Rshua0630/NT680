/*
    Public header file for IFE module.

    This file is the header file that define the API and data type for IFE
    module.

    @file       ife_lib.h
    @ingroup    mIDrvIPP_IFE

    Copyright   Novatek Microelectronics Corp. 2016.  All rights reserved.
*/
#ifndef _IFE_LIB_680_H
#define _IFE_LIB_680_H

/**
    @addtogroup mIDrvIPP_IFE
*/
//@{

/*
    Define IFE function enable.
*/
//@{
//#define IFE_ROWDEFC_EN                  0x00001000  ///< enable function: row defect concealment
#define IFE_OUTL_EN                     0x00002000  ///< enable function: outlier
#define IFE_FILTER_EN                   0x00004000  ///< enable function: filter
#define IFE_CGAIN_EN                    0x00008000  ///< enable function: color gain
#define IFE_VIG_EN                      0x00010000  ///< enable function: vignette
#define IFE_GBAL_EN                     0x00020000  ///< enable function: Gbalance
//#define IFE_FLIP_EN                   0x00200000  ///< removed in NT96680
#define IFE_FUNC_ALL                    0x0003F000  ///< all func enable mask
//@}


/**
    Define IFE interrput enable.
*/
//@{
#define IFE_INTE_FRMEND                 0x00000001  ///< frame end interrupt
//#define IFE_INTE_ROWDEFFAIL             0x00000004  ///< removed in NT96680
#define IFE_INTE_ALL                    0x00000001  ///< all interrupts
//@}

/**
    Define IFE interrput status.
*/
//@{
#define IFE_INT_FRMEND                 0x00000001  ///< frame end interrupt
//#define IFE_INT_ROWDEFFAIL             0x00000004  ///< removed in NT96680
#define IFE_INT_ALL                    0x00000001  ///< all interrupts
//@}

/**
    IFE Function Selection.

    IFE Function Selection.
\n  Used for ife_setMode()
*/
//@{
typedef enum {
	IFE_OPMODE_D2D      = 0,        ///< D2D
	//IFE_OPMODE_IPP      = 1,      ///< removed in NT96680
	//IFE_OPMODE_RDE_IPP  = 2,      ///< removed in NT96680
	IFE_OPMODE_RHE_IPP  = 3,        ///< RHE->IFE->DCE->IPE->IME
	IFE_OPMODE_SIE_IPP  = 4,        ///< (Debug mode)SIE + IFE->DCE->IPE->IME
	IFE_OPMODE_UNKNOWN  = 5,        ///< Undefined mode
	ENUM_DUMMY4WORD(IFE_OPMODE)
} IFE_OPMODE;
//@}

/**
    IFE Register type.

    IFE Register type.
\n  Used for ife_changeAll()
*/
//@{
typedef enum {
	IFE_ALL_REG         = 0,    ///< Set all registers
	IFE_FD_LATCHED      = 1,    ///< Update VD-latched registers only
	IFE_NON_FD_LATCHED  = 2,    ///< Updat Non-VD-latched registers only
	ENUM_DUMMY4WORD(IFE_REGTYPE)
} IFE_REGTYPE;
//@}

/**
    IFE Source Format.

    Set IFE input data format
\n  Used for ife_setMode()
*/
//@{
typedef enum {
	IFE_FMT_NORMAL   = 0,           ///< Prograssive data input
	IFE_FMT_RAW      = 1,           ///< RAW data input
	IFE_FMT_UVPACK   = 2,           ///< UV packing input
	ENUM_DUMMY4WORD(IFE_SRCFMT)
} IFE_SRCFMT;
//@}
#if 0//removed in NT96680
/**
    IFE Filter mode Selection.

    Set IFE filter mode
\n  Used for ife_setMode()
*/
//@{
typedef enum {
	IFE_NORMAL_9x9 = 0,           ///< standard filter in 9x9 area
	IFE_SONLY_9x9  = 1,           ///< spatial filter only in 9x9 area
	ENUM_DUMMY4WORD(IFE_FILTMODE)
} IFE_FILTMODE;
//@}
#endif

/**
    IFE binning Selection.

    binning level selection
*/
//@{
typedef enum {
	IFE_BINNING_DISABLE = 0,        ///< Disable binning
	IFE_BINNINGX2       = 1,        ///< binning x2
	IFE_BINNINGX4       = 2,        ///< binning x4
	IFE_BINNINGX8       = 3,        ///< binning x8
	IFE_BINNINGX16      = 4,        ///< binning x16
	IFE_BINNINGX32      = 5,        ///< binning x32
	IFE_BINNINGX64      = 6,        ///< binning x64
	IFE_BINNINGX128     = 7,        ///< binning x128
	ENUM_DUMMY4WORD(IFE_BINNSEL)
} IFE_BINNSEL;
//@}

/**
    IFE CFA Pattern Selection.

    Set IFE start pixel of the CFA pattern
\n  Used for ife_setMode()
*/
//@{
typedef enum {
	IFE_SONLY_3x3 = 0,           ///< spatial filter only in 3x3 area
	IFE_SONLY_5x5 = 1,           ///< spatial filter only in 5x5 area
	IFE_SONLY_7x7 = 2,           ///< spatial filter only in 7x7 area
	IFE_SONLY_9x9 = 3,           ///< spatial filter only in 9x9 area
	ENUM_DUMMY4WORD(IFE_SONLYLEN)
} IFE_SONLYLEN;
//@}

#if 0
/**
    IFE Delta of Curve Mapping Selection.

    Set IFE step size between two table entries.
\n  Used for ife_setMode()
*/
//@{
typedef enum {
	IFE_DELTA_1 = 0,           ///< step size = 1
	IFE_DELTA_2 = 1,           ///< step size = 2
	IFE_DELTA_4 = 2,           ///< step size = 4
	ENUM_DUMMY4WORD(IFE_CRVMAP_DELTA)
} IFE_CRVMAP_DELTA;
//@}
#endif

#if 0///< removed in NT96680
/**
    IFE Outlier Count Selection.

    Set IFE number of neighbors to establish outlier condition.
\n  Used for ife_setMode()
*/
//@{
typedef enum {
	IFE_EIGHT_NEIGHBOURS = 0,           ///< eight neighbours fall in the criteria
	IFE_SEVEN_NEIGHBOURS = 1,           ///< seven neighbours fall in the criteria
	ENUM_DUMMY4WORD(IFE_OUTL_SEL)
} IFE_OUTL_SEL;
//@}
#endif

#if 0///< removed in NT96680
/**
    IFE Row Defect Concealment Factor Selection.

    Set IFE factor in row defect concealment.
\n  Used for ife_setMode()
*/
//@{
typedef enum {
	IFE_FACT_p5 = 0,           ///< factor = 0.5
	IFE_FACT_p25 = 1,          ///< factor = 0.25
	IFE_FACT_p0  = 2,          ///< factor = 0
	ENUM_DUMMY4WORD(IFE_ROWDEF_FACT)
} IFE_ROWDEF_FACT;
//@}
#endif

/**
    IFE Wait Mode Selection.

    IFE Wait Mode Selection.
\n  Used for ife_setMode()
*/
//@{
typedef enum {
	IFE_WAIT    = 0,                ///< Wait for frame end
	IFE_NOWAIT  = 1,                ///< Do not wait for frame end
	ENUM_DUMMY4WORD(IFE_WAITMODE)
} IFE_WAITMODE;
//@}


/**
    IFE Bit Depth.

    Set IFE input/output bitdepth
*/
//@{
typedef enum {
	IFE_8BIT    = 0,                ///<  8bit data
	IFE_10BIT   = 1,                ///< 10bit data
	IFE_12BIT   = 2,                ///< 12bit data
	IFE_16BIT   = 3,                ///< 16bit data
	ENUM_DUMMY4WORD(IFE_BITDEPTH)
} IFE_BITDEPTH;
//@}

/**
    IFE color gain range selection

    Select IFE color gain range
*/
//@{
typedef enum {
	IFE_CGRANGE_2_8 = 0,    ///< 8bit decimal, set 256 for 1x
	IFE_CGRANGE_3_7 = 1,    ///< 7bit decimal, set 128 for 1x
	ENUM_DUMMY4WORD(IFE_CGRANGE)
} IFE_CGRANGE;
//@}

/**
    IFE input Bayer data start CFA selection

    Select intput Bayer raw data start channel - R, Gr, Gb, B.
*/
//@{
typedef enum {
	IFE_PATR  = 0,  ///< Start channel is R for input Bayer raw data
	IFE_PATGR = 1,  ///< Start channel is Gr for input Bayer raw data
	IFE_PATGB = 2,  ///< Start channel is Gb for input Bayer raw data
	IFE_PATB  = 3,  ///< Start channel is B for input Bayer raw data
	ENUM_DUMMY4WORD(IFE_CFASEL)
} IFE_CFASEL;
//@}

/**
    IFE input Bayer format selection

    Select intput Bayer format
*/
//@{
typedef enum {
	IFE_BAYER_RGGB  = 0, ///< Traditional RGGB format
	IFE_BAYER_RGBIR = 1, ///< RGBIr format
	IFE_BAYER_RGBIR2 = 2, ///< RGBIr format2, these are H-flip of IFE_BAYER_RGBIR
	ENUM_DUMMY4WORD(IFE_BAYERFMTSEL)
} IFE_BAYERFMTSEL;
//@}

/**
    IFE burst length define
*/
//@{
typedef enum {
	IFE_BURST_64W    = 0,       ///< burst length 64 word
	IFE_BURST_48W    = 1,       ///< burst length 48 word
	IFE_BURST_32W    = 2,       ///< burst length 32 word
	//IFE_BURST_16W    = 3,       //removed in NT96680
	ENUM_DUMMY4WORD(IFE_BURST_SEL)
} IFE_BURST_SEL;

typedef struct _IFE_BURST_LENGTH {
	IFE_BURST_SEL   BurstLenInput;  ///< Input burst length
	IFE_BURST_SEL   BurstLenOutput; ///< Output burst length
} IFE_BURST_LENGTH;
//@}

/**
    Struct IFE open object.

    ISR callback function
*/
//@{
typedef struct _IFE_OPENOBJ {
	VOID (*FP_IFEISR_CB)(UINT32 uiIntStatus); ///< isr callback function
	UINT32 uiIfeClockSel; ///< TBD(240 is tested by now)
} IFE_OPENOBJ;
//@}

/**
    Struct IFE Range Filter Setting A.

    Parameters of range filter
*/
//@{
typedef struct _IFE_RANGESETA {
	UINT32 *pRngTh;            ///< range filter threshold
	UINT32 *pRngLut;           ///< range filter threshold adjustment
} IFE_RANGESETA;
//@}

/**
    Struct IFE Range Filter Advanced Parameter.

    Parameters of range filter
*/
//@{
typedef struct _IFE_RANGEPARM {
	BOOL   bBilatThEn;      ///< 3x3 bilateral filter threshold enable
	UINT32 uiBilatTh1;      ///< 3x3 bilateral filter threshold 1
	UINT32 uiBilatTh2;      ///< 3x3 bilateral filter threshold 2
} IFE_RANGEPARM;
//@}

/**
    Struct IFE Range Filter Setting B.

    Parameters of range filter
*/
//@{
typedef struct _IFE_RANGESETB {
	UINT32 *pRngTh;            ///< range filter threshold
	UINT32 *pRngLut;           ///< range filter threshold adjustment
	IFE_RANGEPARM RngParm;     ///< range filter advanced parameter
} IFE_RANGESETB;
//@}

/**
    Struct IFE Clamp and Weighting.

    Parameters of clamp and weight
*/
//@{
typedef struct _IFE_CLAMPWTSET {
	UINT32 uiClampTh;             ///< clamp threshold
	UINT32 uiClampMul;            ///< weighting multiplier
	UINT32 uiClampDlt;            ///< clamp threshold adjustment
} IFE_CLAMPWTSET;
//@}

/**
    Struct IFE Color Gain.

    Parameters of color gain
*/
//@{
typedef struct _IFE_CGAINSET {
	BOOL        bCgainInv;          ///< color gain invert
	BOOL        bCgainHinv;         ///< color gain H-invert
	IFE_CGRANGE CgainRange;         ///< select color gain decimal range
	UINT32      uiCgainMask;        ///< Cloro gain mask
	UINT32      *puiCGain;          ///< color gain, always R/Gr/Gb/B order
	UINT32      *puiCOfs;           ///< color offset, always R/Gr/Gb/B order
} IFE_CGAINSET;
//@}

/**
    Struct IFE Outlier Filter.

    Parameters of outlier filter
*/
//@{
typedef struct _IFE_OUTLSET {
	UINT32          *pOutlBriTh;    ///< outlier threshold for bright points
	UINT32          *pOutlDarkTh;   ///< outlier threshold for dark points
	UINT32          *pOutlCnt;      ///< outlier counter
	BOOL            OutlCrsChanEn;  ///< outlier reference cross channel
	UINT32          OutlWeight;     ///< outlier weighting
	//IFE_OUTL_SEL    OutlOp;         ///< outlier option for neighbour condition
	UINT32          OutlDarkOfs;    ///< outlier dark offset
	UINT32          OutlBrightOfs;  ///< outlier bright offset
} IFE_OUTLSET;
//@}

#if 0//removed in NT96680
/**
    Struct IFE Row Defect Concealment.

    Parameters of row defect concealment
*/
//@{
typedef struct _IFE_ROWDEFSET {
	IFE_ROWDEF_FACT RowDefFact; ///< factor of row defect concealment
	UINT32 uiStIdx;             ///< start index of row defect table
	UINT32 uiOffs;              ///< offset of row defect table entry
	UINT32 *pRowDefTbl;         ///< table of row defect concealment
} IFE_ROWDEFSET;
//@}
#endif
/**
    IFE structure - IFE VIG.
*/
//@{
typedef struct _IFE_VIG_PARAM {
	INT32 *piVigX;        ///< Legal range : -8192~8191,VIG x center of 4 channel
	INT32 *piVigY;        ///< Legal range : -8192~8191,VIG y center of 4 channel
	UINT32 uiVigT;       ///< Legal range : 0~1023,VIG disabled area
	UINT8  uiVigTabGain; ///< Legal range : 0~3,VIG gain factor
	BOOL   bVigDitherEn; ///< VIG dithering enable
	BOOL   bVigDitherRst;///< VIG dithering reset, HW clear
	UINT32 uiVigXDiv;    ///< Legal range : 0~4095,VIG distance gain factor in x direction. unsigned 6.6
	UINT32 uiVigYDiv;    ///< Legal range : 0~4095,VIG distance gain factor in y direction. unsigned 6.6
	UINT32 *puiVigLutC0; ///< Legal range : 0~1023,VIG LUT of channel 0
	UINT32 *puiVigLutC1; ///< Legal range : 0~1023,VIG LUT of channel 1
	UINT32 *puiVigLutC2; ///< Legal range : 0~1023,VIG LUT of channel 2
	UINT32 *puiVigLutC3; ///< Legal range : 0~1023,VIG LUT of channel 3
} IFE_VIG_PARAM;
//@}

/**
    IFE structure - IFE Gbal.
*/
//@{
typedef struct _IFE_GBAL_PARAM {
	UINT32 uiGbalThr;       ///< Legal range : 0~1023, Gbal threshold for decision
	UINT8  *puiGbalStab;    ///< Legal range : 0~7, Gbalstab table defines Gbal weighting for different luminance, 0 : NO Gbal, 7 : Max Gbal
	UINT8  *puiGbalDtab;    ///< Legal range : 0~7, Gbalstab table defines Gbal weighting for different variation, 0 : NO Gbal, 7 : Max Gbal
	UINT32 *puiIrsub;       ///< Legal range : 0~4095 for R,G,B,I
	UINT32 uiIrsubCtcGain;  ///< Legal range : 0~255 for Irsub CTC gain
} IFE_GBAL_PARAM;
//@}

/**
    IFE structure - IFE NlmKer.
*/
//@{
typedef struct _IFE_NLM_KER {
	BOOL  bNlmKerEn;    ///< Enable Bilateral/NLM kernel modification
	UINT8 uiKerSlope0;  ///< Legal range : 0~127, Kernel slope1 = uiKerSlope0/32
	UINT8 uiKerSlope1;  ///< Legal range : 0~127, Kernel slope1 = uiKerSlope1/4
	BOOL  bLocwEn;      ///< Enable Bilateral/NLM local weighting modification
	UINT8 uiBilatWD1;   ///< Legal range : 0~15, Bilateral weighting 1
	UINT8 uiBilatWD2;   ///< Legal range : 0~15, Bilateral weighting 2
	UINT8 *puiKerRadius;///< Kernel radius
} IFE_NLM_KER;
//@}

/**
    IFE structure - IFE NlmKer Weighting LUT.
*/
//@{
typedef struct _IFE_NLM_LUT {
	UINT8 *puiMwth;     ///<
	UINT8 *puiBilatWa;  ///< Legal range : 0~15, weighting table A
	UINT8 *puiBilatWb;  ///< Legal range : 0~15, weighting table B
	UINT8 *puiBilatWc;  ///< Legal range : 0~15, weighting table C
	UINT8 *puiBilatWbl; ///< weighting table BL
	UINT8 *puiBilatWbm; ///< weighting table BM
	UINT8 *puiBilatWbh; ///< weighting table BH
} IFE_NLM_LUT;
//@}

/**
    Struct IFE Filter Parameters.

    IFE filter detailed parameters.
*/
//@{
typedef struct _IFE_FILT_PARA {
	IFE_SRCFMT      SrcFmt;         ///< source format select
	IFE_CFASEL      CfaPat;         ///< CFA pattern if RAW format selected
	//IFE_FILTMODE    FiltMode;       ///< filter mode select
	//BOOL            bRowDefEn;      ///< enable of row defect concealment
	BOOL            bOutlEn;        ///< enable of outlier filter
	BOOL            bFilterEn;      ///< enable of IFE filter
	//BOOL            bCrvMapEn;      ///< enable of curve mapping
	BOOL            bCgainEn;       ///< enable of color gain
	BOOL            bVigEn;         ///< enable of vignette
	BOOL            bGbalEn;        ///< enable of Gbalance
	UINT32          uiBayerFormat;  ///< 0: Bayer, 1: RGBIr, 2: RGBIr
	//BOOL            bFlipEn;        ///< enable of flip
	UINT32          *puiWeightS;    ///< spatial weighting
	IFE_RANGESETA   RngThA;         ///< range filter threshold setting for filterA
	IFE_RANGESETB   RngThB;         ///< range filter threshold setting for filterB
	IFE_NLM_KER     NlmKerSet;      ///< NLM Kernel setting
	IFE_NLM_LUT     NlmLutSet;      ///< NLM Kernel look up tables
	//UINT32          uiBilatW;       ///< range filter A and B weighting
	UINT32          uiRthW;         ///< range filter and spatial filter weighting
	IFE_BINNSEL     Binn;           ///< image binning
	BOOL            bSOnlyEn;       ///< spatial only enable
	IFE_SONLYLEN    SOnlyLen;       ///< filter length of spatial only
	IFE_CLAMPWTSET  ClampWtSet;     ///< clamp and weighting setting
	IFE_CGAINSET    CgainSet;       ///< color gain setting
	IFE_OUTLSET     OutlSet;        ///< outlier filter setting
	//IFE_ROWDEF_FACT RowDefFact;     //removed in NT96680
	UINT32          uiRowDefStIdx;  ///< start index of row defect table
	UINT32          uiRowDefVIni;   ///< vertical start of cropped window
	UINT32          *puiRowDefTbl;  ///< table of row defect concealment
	IFE_VIG_PARAM   VigSet;         ///< vignette setting
	IFE_GBAL_PARAM  GbalSet;        ///< gbalance setting
	UINT8           uiBitDither;    ///< Select bit dithering while truncation
	UINT32          uiRepeat;       ///< repeat times
} IFE_FILT_PARA;
//@}

/**
    Struct IFE size Parameters.

    IFE Size related parameters.
*/
//@{
typedef struct _IFE_SIZE_PARA {
	UINT32 uiWidth;               ///< image width
	UINT32 uiHeight;              ///< image height
	UINT32 uiOfsi;                ///< image input lineoffset
	UINT32 uiOfso;                ///< image output lineoffset
} IFE_SIZE_PARA;
//@}

/**
    Struct IFE ROI Parameters.

    IFE Address, Cropping detailed parameters.
*/
//@{
typedef struct _IFE_ROI_PARA {
	IFE_SIZE_PARA   Size;           ///< input starting address
	UINT32          uiInAddr;       ///< input starting address
	UINT32          uiOutAddr;      ///< output starting address
	UINT32          uiCropStx;      ///< image horizontal crop pixel
	IFE_BITDEPTH    InBit;          ///< input bit select
	IFE_BITDEPTH    OutBit;         ///< out bit select
	BOOL            bFlipEn;        ///< enable of flip
} IFE_ROI_PARA;
//@}

/**
    Struct IFE Information.

    IFE parameter and function selection

\n  Used for ife_setMode()
*/
//@{
typedef struct _IFE_PARAM {
	IFE_OPMODE        Mode;           ///< operation mode
	IFE_SRCFMT      SrcFmt;         ///< source format select
	IFE_CFASEL      CfaPat;         ///< CFA pattern if RAW format selected
	//IFE_FILTMODE    FiltMode;       ///< filter mode select
	IFE_BITDEPTH    InBit;          ///< input bit select
	IFE_BITDEPTH    OutBit;         ///< out bit select
	//BOOL            bRowDefEn;      ///< enable of row defect concealment
	BOOL            bOutlEn;        ///< enable of outlier filter
	BOOL            bFilterEn;      ///< enable of IFE filter
	BOOL            bCgainEn;       ///< enable of color gain
	BOOL            bVigEn;         ///< enable of vignette
	BOOL            bGbalEn;        ///< enable of Gbalance
	UINT32          uiBayerFormat;  ///< 0: Bayer, 1: RGBIr, 2: RGBIr2
	//BOOL            bFlipEn;      ///< enable of flip
	UINT32          *puiWeightS;    ///< spatial weighting
	IFE_RANGESETA   RngThA;         ///< range filter threshold setting for filterA
	IFE_RANGESETB   RngThB;         ///< range filter threshold setting for filterB
	IFE_NLM_KER     NlmKerSet;      ///< NLM Kernel setting
	IFE_NLM_LUT     NlmLutSet;      ///< NLM Kernel look up tables
	//UINT32          uiBilatW;     ///< range filter A and B weighting
	UINT32          uiRthW;         ///< range filter and spatial filter weighting
	IFE_BINNSEL     Binn;           ///< image binning
	BOOL            bSOnlyEn;       ///< spatial only enable
	IFE_SONLYLEN    SOnlyLen;       ///< filter length of spatial only
	IFE_CLAMPWTSET  ClampWtSet;     ///< clamp and weighting setting
	IFE_CGAINSET    CgainSet;       ///< color gain setting
	IFE_OUTLSET     OutlSet;        ///< outlier filter setting
	//IFE_ROWDEF_FACT RowDefFact;     //removed in NT96680
	UINT32          uiRowDefStIdx;  ///< start index of row defect table
	UINT32          uiRowDefVIni;   ///< vertical start of cropped window
	UINT32          *puiRowDefTbl;  ///< table of row defect concealment
	IFE_VIG_PARAM   VigSet;         ///< vignette setting
	IFE_GBAL_PARAM  GbalSet;        ///< gbalance setting
	UINT8           uiBitDither;    ///< Select bit dithering while truncation
	UINT32          uiInAddr;       ///< input starting address
	UINT32          uiOutAddr;      ///< output starting address
	UINT32          uiWidth;        ///< image width
	UINT32          uiHeight;       ///< image height
	UINT32          uiInOfs;        ///< image input lineoffset
	UINT32          uiOutOfs;       ///< image output lineoffset
	UINT32          uiIntrEn;       ///< interrupt enable
} IFE_PARAM;
//@}

extern ER       ife_setMode(IFE_PARAM *pFilterInfo);
extern ER       ife_changeAll(IFE_PARAM *pFilterInfo, IFE_REGTYPE RegType);
//extern ID       ife_getLockStatus(VOID);
extern ER       ife_open(IFE_OPENOBJ *pObjCB);
extern BOOL     ife_isOpened(VOID);
extern ER       ife_close(VOID);
extern ER       ife_enableInt(UINT32 uiIntr);
extern UINT32   ife_getIntEnable(VOID);
extern UINT32   ife_getIntStatus(VOID);
extern ER       ife_clearInt(UINT32 uiIntr);
extern VOID     ife_waitFlagFrameEnd(VOID);
extern ER       ife_enableFunction(BOOL bEnable, UINT32 uiFunc);
extern BOOL     ife_checkFunctionEnable(UINT32 uiFunc);
extern IFE_OPMODE ife_checkOpMode(VOID);
extern UINT32 ife_getStripeCnt(VOID);
extern BOOL     ife_D2DisBusy(VOID);
extern VOID     ife_changeParam(IFE_FILT_PARA *pFiltPara);
extern VOID     ife_changeSize(IFE_SIZE_PARA *pSizePara);
extern VOID     ife_changeROI(IFE_ROI_PARA *pROIPara);
extern ER       ife_start(VOID);
extern ER       ife_dbgStart(UINT32 loadtype);
extern ER       ife_pause(VOID);
extern UINT32   ife_getClockRate(VOID);
extern ER       ife_getBurstLength(IFE_BURST_LENGTH *pBurstLen);
//FPGA USE
extern ER ife_setOpMode(IFE_OPMODE Mode);
#endif

//@}
