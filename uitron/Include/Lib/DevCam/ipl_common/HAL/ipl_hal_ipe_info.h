/**
    IPH module (IPE sub function type define)

    @file       IPL_Hal_IPE_Info.h NT96660
    @ingroup    mILibIPH

    Copyright   Novatek Microelectronics Corp. 2011.  All rights reserved.
*/

#ifndef _IPL_Hal_IPE_Info_H
#define _IPL_Hal_IPE_Info_H
#include "Type.h"
#include "ipl_utility.h"
/** \addtogroup mILibIPH */
//@{

/**
    ipe engine supported clk
*/
typedef enum _IPH_IPECLK {
	IPH_IPECLK0 = 240,  ///< -240 mhz
	IPH_IPECLK1 = 480,  ///< -480 mhz
	ENUM_DUMMY4WORD(IPH_IPECLK)
} IPH_IPECLK;

/**
    IPE in/out size
*/
typedef IPL_YCC_CH_INFO IPE_SIZE;

/**
    IPE input source enum
*/
typedef enum _IPEMode {
	IPE_D2D             = 0,    ///< D2D
	IPE_RHE_IPP         = 3,    ///< RDE + IFE->DCE->IPE->IME
	IPE_SIE_IPP         = 4,    ///< SIE + IFE->DCE->IPE->IME (all direct)
	ENUM_DUMMY4WORD(IPEMode)
} IPEMode;

/**
    IPE input/output format
*/
typedef enum _IPEFmt {
	IPE_YCC_444_UVP = 0,    ///< 444 output (Y planer, UVUVUVUV)
	IPE_YCC_422_UVP = 1,    ///< 422 output (Y planer, UVUVUVUV)
	IPE_YCC_420_UVP = 2,    ///< 420 output (Y planer, UVUVUVUV)
	IPE_Y_ONLY      = 3,    ///< Y only (in/out must be the same)
	ENUM_DUMMY4WORD(IPEFmt)
} IPEFmt;


/**
    IPE DRAM output selection

    Select IPE output data type
\n Used for ipe_changeOutput().
*/
typedef enum _IPEDMAOutSel {
	DMA_SEL_ORG = 0,                ///< original size
	DMA_SEL_DIRECT = 2,             ///< edge direction(for debug)
	DMA_SEL_ETH = 3,                ///< eth
	ENUM_DUMMY4WORD(IPEDMAOutSel)
} IPEDMAOutSel;

/**
    YCC sub sample selection
*/
typedef enum _IPEDataDrop {
	Drop_R  = 0,    ///< drop right side pixel during subsampling When 422 or 420 format is chosen
	Drop_L  = 1,    ///< drop left side pixel during subsampling When 422 or 420 format is chosen
	ENUM_DUMMY4WORD(IPEDataDrop)
} IPEDataDrop;

/**
    edge input source
*/
typedef enum _EDGE_SRC {
	E_PRE_GAMMA = 0,    ///< before tone-remap
	E_POS_GAMMA = 1,    ///< after tone-remap
	ENUM_DUMMY4WORD(EDGE_SRC)
} EDGE_SRC;

/**
    cc input source
*/
typedef enum _CC_SRC {
	CC_PRE_GAMMA = 0,    ///< before tone-remap
	CC_POS_GAMMA = 1,    ///< after tone-remap
	ENUM_DUMMY4WORD(CC_SRC)
} CC_SRC;

/**
    edge channel select
*/
typedef enum _EDGE_CHSEL {
	EDGE_G_CH   = 0,    ///< G channel of bayer format
	EDGE_Y_CH   = 1,    ///< Y channel of YCC format
	ENUM_DUMMY4WORD(EDGE_CHSEL)
} EDGE_CHSEL;

/**
    low pass filter
*/
typedef struct _LPF {
	UINT32  lpfw;       ///< low pass filter weighting
	UINT32  sonlyw;     ///< spatial only fitler weighting
	UINT32  rangeth0;   ///< range threshold
	UINT32  rangeth1;   ///< -
	enum {
		_3x3    = 0,    ///< 3x3 window
		_5x5    = 1,    ///< 5x5 window
		_7x7    = 2,    ///< 7x7 window
		ENUM_DUMMY4WORD(lpfsize)
	} lpfsize;          ///< filter size
} LPF;

/**
    edge map threshold
*/
typedef struct _EdgeMap {
	enum {
		EMI_ESD = 0,    ///< Select 1st order kernel output for edge mapping input
		EMI_E57 = 1,    ///< Select weighted kernel output of 5x5 & 7x7 2nd order kernels for edge mapping input
		EMI_AVG = 2,    ///< Select average of 1st order and 2nd order kernels for edge mapping input
		ENUM_DUMMY4WORD(EM_InSel)
	} EM_InSel;     ///< Edge mapping input selection
	UINT32  EthrL;  ///< Legal range : 0~1023, for edge mapping, if(Ein < EthrL){Eout=0}, else if(EthrA <= Ein <EthrH){Eout apply EtabL and EDtabA}, else if(Ein >= EthrB){Eout apply EtabH and EDtabB}
	UINT32  EthrH;  ///< Legal range : 0~1023, for edge mapping, if(Ein < EthrA){Eout=0}, else if(EthrA <= Ein <EthrB){Eout apply EtabL and EDtabA}, else if(Ein >= EthrH){Eout apply EtabH and EDtabB}
	UINT32  EtabL;  ///< Legal range : 0~6, for edge mapping, EDtabA table has 8 sections, each section width is (1 << etaA)
	UINT32  EtabH;  ///< Legal range : 0~6, for edge mapping, EDtabB table has 8 sections, each section width is (1 << etaB)
	UINT32  TAB_TblAddr;    ///< tab num:16, Data type:UINT8
} EdgeMap;


typedef struct _EdgeMapS {
	UINT32  EthrL;  ///< Legal range : 0~1023, for edge mapping, if(Ein < EthrL){Eout=0}, else if(EthrA <= Ein <EthrH){Eout apply EtabL and EDtabA}, else if(Ein >= EthrB){Eout apply EtabH and EDtabB}
	UINT32  EthrH;  ///< Legal range : 0~1023, for edge mapping, if(Ein < EthrA){Eout=0}, else if(EthrA <= Ein <EthrB){Eout apply EtabL and EDtabA}, else if(Ein >= EthrH){Eout apply EtabH and EDtabB}
	UINT32  EtabL;  ///< Legal range : 0~6, for edge mapping, EDtabA table has 8 sections, each section width is (1 << etaA)
	UINT32  EtabH;  ///< Legal range : 0~6, for edge mapping, EDtabB table has 8 sections, each section width is (1 << etaB)
	UINT32  TAB_TblAddr;    ///< tab num:16, Data type:UINT8
} EdgeMapS;

/**
    IPE edge threshold output

    current dma ready info
*/
typedef struct _IPE_ETH_DramRst {
	UINT32  addr;       ///< -
	UINT32  lineofs;    ///< -
	UINT32  width;      ///< -
	UINT32  height;     ///< -
	UINT32  fmt_8bit;
	UINT32  downsampled;
} IPE_ETH_DramRst;
/**
    ipe interrupt enable option
*/
typedef enum _IPEINTE {
	IPE__CLR                = 0,
	IPE__INTE_FMD           = 0X00000002,   ///< enable interrupt: frame done
	IPE__INTE_STPE          = 0x00000004,   ///< enable interrupt: current stripe done
	IPE__INTE_FMS           = 0x00000008,   ///< enable interrupt: frame start
	IPE__INTE_YCC_DMA_E     = 0x00000010,   ///< enable interrupt: YCC DRAM output done
	IPE__INTE_DAM0_IN_E     = 0x00000020,   ///< enable interrupt: DRAM input done(gamma)
	IPE__INTE_DAM1_IN_E     = 0x00000040,   ///< enable interrupt: DRAM input done(3Dlut)
	IPE__INTE_ALL           = 0X0000007E,   ///< All interrupt enable
	ENUM_DUMMY4WORD(IPEINTE)
} IPEINTE;

/**
    ipe function enable selection
*/
typedef enum _IPEFUNCEN {
	IPE_RGBLPEn         = 0x00000001,       ///< RGB low-pass filter function enable
	IPE_RGBGAMMAEn      = 0x00000002,       ///< RGB Gamma function enable
	IPE_YGAMMAEn        = 0x00000004,       ///< Y Gamma function enable,(curve share with ife chroma curve table)
	IPE_CCREn           = 0x00000100,       ///< Color correction function enable
	IPE_3DCCREn         = 0x00000200,       ///< 3D color correction function enable
	IPE_CSTEn           = 0x00000400,       ///< Color space transform function enable, RGB->YCC
	IPE_CCTRLEn         = 0x00001000,       ///< Color control function enable
	IPE_HADJEn          = 0x00002000,       ///< Hue adjustment function enable
	IPE_CADJEn          = 0x00004000,       ///< Color component adjust function enable
	IPE_CADJ_Y_ENHEn    = 0x00008000,       ///< Color component edge enhance function enable, sub-function of IPE_CADJEn
	IPE_CADJ_Y_CONEn    = 0x00010000,       ///< Color component Y contrast adjust function enable, sub-function of IPE_CADJEn
	IPE_CADJ_CC_CONEn   = 0x00020000,       ///< Color component CbCr contrast adjust function enable, sub-function of IPE_CADJEn
	IPE_YCTHEn          = 0x00040000,       ///< Color component YCbCr substitution function enable, sub-function of IPE_CADJEn
	IPE_CSTPEn          = 0x00080000,       ///< Color space transform protect function enable, RGB->YCC
	ENUM_DUMMY4WORD(IPEFUNCEN)
} IPEFUNCEN;


/**
    ipe y different range
*/
typedef enum _IPE_YDRange {
	IPE_YDR_9x9 = 0,      ///<  Select 9x9 for y diff calculation
	IPE_YDR_7x7 = 1,      ///<  Select 7x7 for y diff calculation
	IPE_YDR_5x5 = 2,      ///<  Select 5x5 for y diff calculation
	IPE_YDR_3x3 = 3,      ///<  Select 3x3 for y diff calculation
	ENUM_DUMMY4WORD(IPE_YDRange)
} IPE_YDRange;


/**
    ipe edge dir kernel selection
*/
typedef enum _IPE_EKerSel {
	KER_SEL_A = 0,      ///< kernel A
	KER_SEL_B = 1,      ///< kernel B
	ENUM_DUMMY4WORD(IPE_EKerSel)
} IPE_EKerSel;

/**
    ipe edge div
*/
typedef enum _IPE_EdgeDiv {
	EdgeDiv_0   = 0,    ///< Normal division
	EdgeDiv_1   = 1,    ///< normal division - 1
	EdgeDiv_2   = 2,    ///< normal division - 2
	EdgeDiv_3   = 3,    ///< normal division - 3
	ENUM_DUMMY4WORD(IPE_EdgeDiv)
} IPE_EdgeDiv;

/**
    IPE structure - IPE directed edge score calculation
*/
//@{
typedef struct _IPE_DirScorCal {
	INT8        iA0;              ///< a coefficient of score 0
	INT8        iB0;              ///< b coefficient of score 0
	INT8        iC0;              ///< c coefficient of score 0
	UINT8       uiD0;             ///< d coefficient of score 0
	INT8        iA1;              ///< a coefficient of score 1
	INT8        iB1;              ///< b coefficient of score 1
	INT8        iC1;              ///< c coefficient of score 1
	UINT8       uiD1;             ///< d coefficient of score 1
	INT8        iA2;              ///< a coefficient of score 2
	INT8        iB2;              ///< b coefficient of score 2
	INT8        iC2;              ///< c coefficient of score 2
	UINT8       uiD2;             ///< d coefficient of score 2
} IPE_DirScorCal;


/**
    IPE structure - IPE directed edge score calculation threshold
*/
//@{
typedef struct _IPE_DirScorCalTh {
	UINT8       uiScoreTh0;       ///< score threshold 0
	UINT8       uiScoreTh1;       ///< score threshold 1
	UINT8       uiScoreTh2;       ///< score threshold 2
	UINT8       uiScoreTh3;       ///< score threshold 3
} IPE_DirScorCalTh;


/**
    IPE directed edge: connection score

    Select connection score of directed edge

*/
typedef enum _IPE_DEConScr {
	DE_SCR_ADD1 = 0,      ///< add 1
	DE_SCR_ADD2 = 1,      ///< add 2
	ENUM_DUMMY4WORD(IPE_DEConScr)
} IPE_DEConScr;

/**
    IPE directed edge: index selection in connection

    Select index selection in connection of directed edge

*/
typedef enum _IPE_DEConSel {
	DECON_COMPLEX = 0,      ///< as complex area
	DECON_FLAT = 1,         ///< as flat area
	ENUM_DUMMY4WORD(IPE_DEConSel)
} IPE_DEConSel;

/**
    IPE structure - IPE directed edge connection parameters
*/
//@{
typedef struct _IPE_DEConParam {
	IPE_DEConScr ScrCenter;        ///< score of center
	IPE_DEConScr ScrEdg;           ///< score of edges
	IPE_DEConScr ScrDetail;        ///< score of details
	UINT8        uiCntNumTh;       ///< threshold of count number
	UINT8        uiMax1Th;         ///< threshold of max 1
	UINT8        uiMaxCntTh;       ///< threshold of max count
	UINT8        uiMax2Th;         ///< threshold of max 2
	IPE_DEConSel IdxSel;           ///< index selection
} IPE_DEConParam;
//@}

/**
    IPE structure - IPE edge weighting paramters of kernel A
*/
//@{
typedef struct _IPE_KAEdgeW {
	IPE_EKerSel  EWKer2DSel;       ///< 2D kernel selection of edge weighting
	UINT8        uiAdwt0;          ///< kernal A's dir weight 0
	UINT8        uiAdwt1;          ///< kernal A's dir weight 1
	UINT8        uiAdwt2;          ///< kernal A's dir weight 2
	UINT8        uiAdwt3;          ///< kernal A's dir weight 3
	UINT8        uiAdwt4;          ///< kernal A's dir weight 4
	UINT8        uiAdwt5;          ///< kernal A's dir weight 5
	INT8         iAds0;            ///< kernal A's dir shift 0
	INT8         iAds1;            ///< kernal A's dir shift 1
	INT8         iAds2;            ///< kernal A's dir shift 2
	INT8         iAds3;            ///< kernal A's dir shift 3
	INT8         iAds4;            ///< kernal A's dir shift 4
	INT8         iAds5;            ///< kernal A's dir shift 5
} IPE_KAEdgeW;
//@}


/**
    IPE edge weighting: kernal B output selection

    Select index selection in connection of directed edge

*/
typedef enum IPE_KBEdgeOutSel {
	SEL_AVG_HV = 0,         ///< (ED0+ED1)>>1
	SEL_AVG_4D = 1,         ///< (ED0+ED1+ ED2+ED3)>>2
	ENUM_DUMMY4WORD(IPE_KBEdgeOutSel)
} IPE_KBEdgeOutSel;

/**
    IPE structure - IPE edge weighting paramters of kernel B
*/
//@{
typedef struct _IPE_KBEdgeW {
	UINT8           uiBdwt0;          ///< kernal B's dir weight 0
	UINT8           uiBdwt1;          ///< kernal B's dir weight 1
	INT8            iBds0;            ///< kernal B's dir shift 0
	INT8            iBds1;            ///< kernal B's dir shift 1
	IPE_KBEdgeOutSel EwKerBOutSel;     ///< Kernal B's output selection
} IPE_KBEdgeW;
//@}


/**
    IPE structure - IPE edge weighting paramters
*/
//@{
typedef struct _IPE_EdgeW {
	UINT8           uiEWDir0;         ///< weighting 0 with E7
	UINT8           uiEWDir1;         ///< weighting 1 with E7
	UINT8           uiEWDir2;         ///< weighting 2 with E7
	UINT32          DynEMEn;          ///< enable of dynamic e-mapping
} IPE_EdgeW;
//@}

/**
    directed edge extraction parameter set
*/
typedef struct _IPE_DEE {
	IPE_YDRange YDRange;            ///< range of calculation y difference
	UINT32 uiYDMaxTh;               ///< threshold of max y difference
	UINT32 PndAdjEn;                ///< enable of adjusting PD/ND edges
	UINT32 uiPndShft;               ///< PNDEdge = PNDedge - (PNDedge>>(uiPndShft+1))
	UINT32 uiES;                    ///< Edge shift for all directions
	IPE_EKerSel EdirSel;            ///< Select kernel of directed eext
} IPE_DEE;

/**
    IPE Edge extraction
*/
typedef struct _IPE_EDGEEXT {
	EDGE_SRC    GamSel;             ///< source selection(post or pre gamma)
	EDGE_CHSEL  ChSel;              ///< channel selection(G or Y channel)

	UINT32  EEXT_TblAddr;           ///< user defined edge kernel , tab num:11, Data type:INT16 (idx 0~9 is edge kernal, idx 10 is div)

	IPE_EdgeDiv EdgeDiv;            ///< Legal range : 0~3, 0: Normal division for directed edge kernel, 1: -1 division, 2: -2 division, 3: -3 division
	UINT32  EDirTabAddr;            ///<Legal range : 0~8, wieghting for directed edge, 0 : no direction decision, 8 : sensitive edge decision
	UINT32  ToneMapLutTabAddr;
	IPE_DEE *pDirEdgeExt;
	IPE_DirScorCal *ScrCalTab;
	IPE_DirScorCalTh *ScrCalThTab;
	UINT32 PowerSav;
	IPE_DEConParam *ConParam;
	IPE_KAEdgeW *pEWA;
	IPE_KBEdgeW *pEWB;
	IPE_EdgeW *pEWParam;
} IPE_EDGEEXT;

/**
    Edge Enhancement/Edge inverse
*/
typedef struct _IPE_EDGEENH {
	UINT32  EnhP;   ///< positive edge enhancement
	UINT32  EnhN;   ///< negative edge enhancement
	BOOL    InvP;   ///< inverse positive edge to negative edge if true
	BOOL    InvN;   ///< inverse negative edge to positive edge if true
} IPE_EDGEENH;


/**
    IPE Edge mapping
*/
typedef struct _IPE_EDGEMAP {
	EdgeMap edMap;          ///< edge mapping threshold
	EdgeMapS esMap;         ///< edge luminance map
	enum {
		_EEXT   = 0,    ///< Select 5x5 user defined kernel
		_EEXT2  = 1,    ///< Select 3x3 fixed kernel
		ENUM_DUMMY4WORD(EMapOut)
	} EMapOut;          ///< kernel selection for edge mapping output
} IPE_EDGEMAP;


/**
    RGB plane low pass filter
*/
typedef struct _IPE_RGBLPF {
	LPF R;  ///< R channel
	LPF G;  ///< -
	LPF B;  ///< -
} IPE_RGBLPF;

/**
    color correction
*/
typedef enum _CCRange {
	CC_2_9 = 0,           ///< format 2.9
	CC_3_8 = 1,           ///< format 3.8
	CC_4_7 = 2,           ///< format 4.7
	ENUM_DUMMY4WORD(CCRange)
} CCRange;

typedef enum _CCSupSel {
	toIdentity  = 0,    ///< target: identity matrix
	toGray      = 1,    ///< target: gray
	ENUM_DUMMY4WORD(CCSupSel)
} CCSupSel;

typedef enum _CC_StabSel {
	CC_SEL_MAX = 0,             ///< Select max
	CC_SEL_MIN = 1,             ///< Select min
	CC_SEL_MAX_MINUS_MIN = 2,   ///< Select max-min
	ENUM_DUMMY4WORD(CC_StabSel)
} CC_StabSel;

typedef struct _IPE_CC {
	CCRange Range;
	CCSupSel SupSel;
	CC_SRC SrcSel;
	CC_StabSel StabSel;
	UINT32  CCcoef_TblAddr;     ///< tab num:9, Data type:INT16
	UINT32  Fstab_TblAddr;      ///< tab num:16, Data type:UINT8
	UINT32  Fdtab_TblAddr;      ///< tab num:16, Data type:UINT8
} IPE_CC;

typedef enum _IPE_CSTOffSel {
	CST_SEL_NOOP = 0,               ///< No operation
	CST_SEL_MINUS128 = 1,           ///< out = in - 128
	ENUM_DUMMY4WORD(IPE_CSTOffSel)
} IPE_CSTOffSel;

/**
    color space transfor parameter set
*/
typedef struct _IPE_CST {
	UINT32 CSTCoefTabAddr;            ///< color space transform matrix [yr yg yb ur ug ub vr vg vb],  tab num:9, Data type:INT16
	IPE_CSTOffSel CSTOffSel;
	UINT8 CSTPratio;
} IPE_CST;


typedef enum _IPE_CCtrlSel {
	CCTRL_SEL_1STORDER = 0,
	CCTRL_SEL_E5 = 1,
	CCTRL_SEL_E7 = 2,
	ENUM_DUMMY4WORD(IPE_CCtrlSel)
} IPE_CCtrlSel;

/**
    color control parameter set
*/
typedef struct _IPE_CCTRL {
	INT16   IntOfs;         ///< Legal range : -128~127, Y offset, Y' = Y + Intofs
	INT16   SatOfs;         ///< Legal range : -128~127, color saturation adjust, 0 : NO color saturation adjust
	BOOL    HueRotateEn;    ///< Hue adjust rotation option, 0 : NO rotation, 1 : depend on hue tab LSB 2 bit, 0 : 0 degree rotation, 1 : 90 degree rotation, 2 bit = 2 : 180 degree rotation, 3 : 270 degree rotation
	IPE_CCtrlSel CctrlSel;
	UINT8 uiVdetDiv;
	UINT32  HueTab_TblAddr; ///< Legal range : 0~255, 128 : NO hue adjust. (tab num:24, Data Type:UINT8)
	UINT32  SatTab_TblAddr; ///< Legal range : -128~127, 0 : NO sat adjust. (tab num:24, Data Type:INT32)
	UINT32  IntTab_TblAddr; ///< Legal range : -128~127, 0 : NO sat adjust. (tab num:24, Data Type:INT32)
	UINT32  EdgTab_TblAddr; ///< Legal range : 0~255, 128 : NO edge adjust.(tab num:24, Data Type:UINT8)
	UINT32  DDSTab_TblAddr; ///< Legal range : 0~255, weighting table for SatTab/IntTab/EdgTab. 0 : NO adjust, 32 : 1X adjust weighting
} IPE_CCTRL;

/*
    Cb Cr color offset,
*/
typedef struct _IPE_CCOFS {
	UINT8   Cbofs;  ///< Legal range : 0~255, Cb color offset, 128 : NO color offset
	UINT8   Crofs;  ///< -
} IPE_CCOFS;

/**
    Y/CbCr contrast
*/
typedef struct _IPE_CCON {
	UINT8           YCon;   ///< Legal range : 0~255, Y contrast adjust, 128 : NO contrst adjust, Y' = (Y * Ycon) >> 7
	UINT8           UVCon;   ///< Legal range : 0~255, CbCr contrast adjust, 128 : NO contrst adjust, C' = (C * CCon) >> 7
} IPE_CCON;

/**
    ipe random noise
*/
typedef struct _IPE_RAND_NR {
	UINT32 YRandEn;
	UINT32 CRandEn;
	UINT32 uiYRandLevel;  ///< Legal range : 0~7, 0 : NO Y random noise, 7 : MAX Y random noise level
	UINT32 uiCRandLevel;  ///< Legal range : 0~7, 0 : NO CbCr random noise, 7 : MAX CbCr random noise level
	BOOL bRandReset;     ///< Legal range : 0~1, 0 : No reset for random pattern sequence, 1 : Frame start reset for random pattern seqeunce.
} IPE_RAND_NR;

/**
    color mask effect
*/
typedef struct _IPE_YCCMASK {
	UINT8   Ymask;  ///< -
	UINT8   Cbmask; ///< -
	UINT8   Crmask; ///< -
} IPE_YCCMASK;


typedef enum _IPE_YCTHSel {
	YCTH_SEL_ORG_VAL = 0,
	YCTH_SEL_REG_VAL = 1,
	ENUM_DUMMY4WORD(IPE_YCTHSel)
} IPE_YCTHSel;

/**
    YCC fix effect
*/
typedef struct _IPE_YCCFIX {
	struct {
		UINT32 uiYth;
		UINT32 uiEth;
		IPE_YCTHSel HitSel;
		IPE_YCTHSel NHitSel;
		UINT32 uiHitValue;
		UINT32 uiNHitValue;
	} Yth1;                             ///< Y fix effect(with edge condition)

	struct {
		UINT32 uiYth;
		IPE_YCTHSel HitSel;
		IPE_YCTHSel NHitSel;
		UINT32 uiHitValue;
		UINT32 uiNHitValue;
	} Yth2;                             ///< Y fix effect

	struct {
		UINT32 uiEth;
		UINT32 uiYthL;
		UINT32 uiYthH;
		UINT32 uiCBthL;
		UINT32 uiCBthH;
		UINT32 uiCRthL;
		UINT32 uiCRthH;
		IPE_YCTHSel HitSel;
		IPE_YCTHSel NHitSel;
		UINT32 uiCBHitValue;
		UINT32 uiCBNHitValue;
		UINT32 uiCRHitValue;
		UINT32 uiCRNHitValue;
	} CC;                               ///< CC fix effect
} IPE_YCCFIX;

/**
    ipe R/G/B gamma curve
*/
/**
    IPE structure - IPE random input of gamma and y curve
*/
//@{
typedef struct _IPE_GamRand {
	UINT32 GamYRandEn;  ///< enable of random input
	UINT32 GamYRandRst; ///< reset of random input
	UINT8 uiGamYRandShft;   ///< shift of random input
} IPE_GamRand;


/**
    IPE 3DCC rounding

    Select input of color correction stab
*/
typedef enum _IPE_3DCCRoundSel {
	SEL_ROUNDING = 0,             ///< rounding
	SEL_HALFTONE = 1,             ///< halftone rounding
	SEL_RANDOM = 2,               ///< random rounding
	ENUM_DUMMY4WORD(IPE_3DCCRoundSel)
} IPE_3DCCRoundSel;


typedef struct _IPE_3DCCRound {
	IPE_3DCCRoundSel RndOpt;
	UINT32 RndRst;
} IPE_3DCCRound;

/**
    ipe 3d color control
*/
typedef struct _IPE_3DCC {
	UINT32          TCC_TblAddr;        ///<3d color control, (tab num:729, Data type: IPE3DC)
	IPE_3DCCRound   *pCC3dRnd;
} IPE_3DCC;


/**
    ipe eth control
*/
typedef enum {
	SEL_ETH_ORG = 0,                    ///< original size
	SEL_ETH_DOWNSAMPLED = 1,            ///< downsampled by 2x2
	ENUM_DUMMY4WORD(IPE_EthSel)
} IPE_EthSel;

/**
    IPE ETH ouput format

    Select ETH output bitdepth
*/
typedef enum {
	ETH_2BITS = 0,             ///< 2 bits/pixel
	ETH_8BITS = 1,             ///< 8 bits/pixel
	ENUM_DUMMY4WORD(IPE_EthFmt)
} IPE_EthFmt;

/**
    IPE ETH ouput position

    Select ETH output position when downsampling
*/
typedef enum {
	ETH_EVEN = 0,             ///< even position
	ETH_ODD = 1,              ///< odd position
	ENUM_DUMMY4WORD(IPE_EthPosSel)
} IPE_EthPosSel;


typedef struct {
	IPE_EthSel EthOutSel;       ///< If ETH output is downsampled
	IPE_EthFmt EthOutFmt;       ///< ETH output format
	UINT32 EthLow;              ///< Low edge threshold value
	UINT32 EthMid;              ///< Middle edge threshold value
	UINT32 EthHigh;             ///< High edge threshold value
	IPE_EthPosSel EthHout;     ///< Horizontal position selection when downsampling
	IPE_EthPosSel EthVout;     ///< Vertical position selection when downsampling
} IPE_EethParam;

/**
    IPE H overlap selection for MST

    Select IPE H overlap format
*/
typedef enum {
	IPE_HAL_HOLAP_AUTO = 0,         ///< Auto calculation of H overlap pixels
	IPE_HAL_HOLAP_8PX  = 1,         ///< Keep H overlap to 8 pixels
	ENUM_DUMMY4WORD(IPE_HAL_HOVLP_SEL_ENUM)
} IPE_HAL_HOVLP_SEL_ENUM;

/**
     IPE sub function configuration.
*/
typedef struct _IPE_SUBFUNC {
	IPEINTE IpeInteEn;              ///< interrupt enable

	IPEFUNCEN IpeFuncEn;            ///< function enable


	IPE_EDGEEXT     *pedgeExt;      ///< edge extraction
	IPE_EDGEENH     *pedgeEnh;      ///< edge enhancement
	IPE_EDGEMAP     *pedgeMap;      ///< edge mapping

	IPE_RGBLPF      *prgblpf;       ///< rgb low pass filter
	IPE_CC          *pCC;           ///< color correction
	IPE_CCTRL       *pCctrl;        ///< color control
	IPE_CCOFS       *pCbCrofs;      ///< Cb/Cr color offset
	IPE_CCON        *pYCCcon;       ///< Y/CC contrast
	IPE_RAND_NR     *pRandNoise;    ///< Random noise
	IPE_YCCMASK     *pyccmask;      ///< color mask
	IPE_CST         *pCST;          ///< color space transfor
	IPE_YCCFIX      *pYccFix;       ///< Y/CC fix
	UINT32          GammaAddr;      ///< RGB gamma
	UINT32          YGammaAddr;     ///< Y gamma
	IPE_GamRand     *pGamRand;      ///< gamma rand
	IPE_3DCC        *p3d_cc;        ///< 3D color control
	IPE_EethParam   *pEthparam;     ///< eth parameters
} IPE_SUBFUNC;

//@}
#endif
