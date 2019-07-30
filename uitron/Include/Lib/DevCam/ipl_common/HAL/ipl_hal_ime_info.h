/**
    IPH module (IME sub function type define)

    @file       IPL_Hal_IME_Info.h NT96660
    @ingroup    mILibIPH

    Copyright   Novatek Microelectronics Corp. 2011.  All rights reserved.
*/

#ifndef _IPL_Hal_IME_Info_H
#define _IPL_Hal_IME_Info_H
#include "Type.h"
#include "ipl_utility.h"


/** \addtogroup mILibIPH */
//@{

/**
    ime engine supported clk
*/
typedef enum _IPH_IMECLK {
	IPH_IMECLK0 = 240,  ///< -240 mhz
	IPH_IMECLK1 = 480,  ///< -480 mhz
	ENUM_DUMMY4WORD(IPH_IMECLK)
} IPH_IMECLK;


typedef enum _IME_SRCMODE {
	IME_D2D             = 0,        ///< IME only
	IME_RHE_IPP         = 1,        ///< RHE + IFE->DCE->IPE->IME
	IME_SIE_IPP         = 2,        ///< SIE + IFE->DCE->IPE->IME (all direct)
	ENUM_DUMMY4WORD(IME_SRCMODE)
} IME_SRCMODE;                     ///< IME operation mode
/**
    IME in/out data format
*/
typedef IPL_YUV_IMG_TYPE IMEFMT;

typedef struct {
	UINT32  Y;
	UINT32  Cb;
	UINT32  Cr;
} IMEADDR;

typedef struct {
	IPL_YCC_CH_INFO Y;
	UINT32  Lineoffset_CC;
} IMESIZE;

/**
    IME interrupt enable enum.
*/
typedef enum _ImeInteEn {
	IME__CLR        = 0,
	IME__INTE_STPE  = 0x40000000,   ///< enable interrupt: stipe end
	IME__INTE_FMD   = 0x80000000,   ///< enable interrupt: frame end
} ImeInteEn;

/**
    IME sub function enable
*/
typedef enum _ImeFuncEn {
	ChromaAdaptEn   = 0x00000001,   ///< enable function: chroma adaption (LCA)
	ChromaMedianEn  = 0x00000002,   ///< enable function: chroma median filter
	ChromaAdjLchrom = 0x00000004,   ///< enable function: chroma adjustment of local chroma adaptation
	ChromaAdjLum    = 0x00000008,   ///< enable function: chroma adjustment of local luma adaptation
	ChromaDBSuppres = 0x00000010,   ///< enable function: dark and bright region chroma suppression
	SingleImgSuperR = 0x00000020,   ///< enable function: single image super-resolution
	FilmGrainEn     = 0x00000040,   ///< enable function: film grain
	IMEP2IEn        = 0x00000080,   ///< enable function: P frame to I frame
	RGBtoYCCEn      = 0x00000100,   ///< enable function: RGB spaec to YCC space
	YCCtoRGBEn      = 0x00000200,   ///< enable function: YCC space to RGB space
	//IME3DNREn       = 0x00000400,   ///< enable function: IME 3DNR
	//STLEn           = 0x00000800,   ///< enable function: IME Path4 satistical information
	//STLFilterEn     = 0x00001000,   ///< enable function: IME Path4 satistical filter
	//DSRGBtoYCCEn    = 0x00002000,   ///< enable function: data stamp color space transfer RGB to YUV
	//DS0En           = 0x00004000,   ///< enable function: data stamp 0
	//DS1En           = 0x00008000,   ///< enable function: data stamp 1
	//DS2En           = 0x00010000,   ///< enable function: data stamp 2
	//DS3En           = 0x00020000,   ///< enable function: data stamp 3
	//ChroamSubOutEn  = 0x00040000,   ///< enable function: chroma sub output to dram
	//P1EncEn         = 0x00080000,   ///< enable function: ime path1 encode enable
	//IME3DNRDecEn    = 0x00100000,   ///< enable function: 3dnr input reference decoder
	//PM0En           = 0x00200000,   ///< enable function: privacy mask 0
	//PM1En           = 0x00400000,   ///< enable function: privacy mask 1
	//PM2En           = 0x00800000,   ///< enable function: privacy mask 2
	//PM3En           = 0x01000000,   ///< enable function: privacy mask 3
	//PM4En           = 0x02000000,   ///< enable function: privacy mask 4
	//PM5En           = 0x04000000,   ///< enable function: privacy mask 5
	//PM6En           = 0x08000000,   ///< enable function: privacy mask 6
	//PM7En           = 0x10000000,   ///< enable function: privacy mask 7
	SQUAREEn        = 0x20000000,   ///< enable function: SQUARE(3DNR), output mv,mo
	ENUM_DUMMY4WORD(ImeFuncEn)
} ImeFuncEn;

typedef struct _IME_GET_STATS_INFO {
	UINT32 uiGetHistMax0;      ///< max value of histogram area of set0
	UINT32 uiGetAccTagBin0;    ///< hit bin of histogram accumulation of set0

	UINT32 uiGetHistMax1;      ///< max value of histogram area of set1
	UINT32 uiGetAccTagBin1;    ///< hit bin of histogram accumulation of set1
} IME_GET_STATS_INFO;

/**
    stripe overlap enum
*/
typedef enum _IME_STPESEL {
	IME_STPE_OVERLAP_16     = 0,    ///< overlap 16 pixels
	IME_STPE_OVERLAP_24     = 1,    ///< overlap 24 pixels
	IME_STPE_OVERLAP_32     = 2,    ///< overlap 32 pixels
	IME_STPE_OVERLAP_USER   = 3,    ///< overlap 32 pixels
	ENUM_DUMMY4WORD(IME_STPESEL)
} IME_STPESEL;

typedef enum _IME_STRP_PRT_SEL {
	IME_STRP_PRT_5P  = 0,    ///< Horizontal stripe overlap partition size: 5 pixels
	IME_STRP_PRT_3P  = 1,    ///< Horizontal stripe overlap partition size: 3 pixels
	IME_STRP_PRT_2P  = 2,    ///< Horizontal stripe overlap partition size: 2 pixels
	IME_STRP_PRT_USER = 3,    ///< Horizontal stripe overlap partition size: user define
	ENUM_DUMMY4WORD(_IME_STRP_PRT_SEL)
} IME_STRP_PRT_SEL;

/**
    stripe unit
*/
typedef struct _IME_STRIPE {
	UINT32  N;          ///< Size of stripe except for last one, Horn. unit : 16 pix, Ver. unit : 1 line
	UINT32  L;          ///< Size of last stripe, Horn. unit : 16 pix, Ver. unit : 1 line
	UINT32  M;          ///< Number of total stripe - 1
} IME_STRIPE;

typedef struct _IME_HAL_STRIPE_INFO {
	IME_STRIPE       strp_h;        ///< hroizontal stripe parameters
	IME_STRIPE       strp_v;        ///< Vertical stripe parameters
	IME_STPESEL      overlap_sel;   ///< IME input stripe overlap size selection for horizontal direction
	UINT32           overlap_user;  ///< IME input stripe overlap size for horizontal direction
	IME_STRP_PRT_SEL prt_sel;       ///< IME input stripe overlap parition size selection for horizontal direction
	UINT32           prt_user;      ///< IME input stripe overlap parition size for horizontal direction
	BOOL             manual_mode;   ///< IME input stripe information setting mode
} IME_HAL_STRIPE_INFO;

/**
     ime scaling method
     @note integration is supported only when image scaling down.
*/
typedef enum _IME_SCALER {
	IME_BICUBIC     = 0,    ///< bicubic interpolation
	IME_BILINEAR    = 1,    ///< bilinear interpolation
	IME_NEAREST     = 2,    ///< nearest interpolation
	IME_INTEGRATION = 3,    ///< integration interpolation,support only scale down
	IME_AUTO        = 4,    ///< automatical calculation
	IME_SCALER_MAX,
	ENUM_DUMMY4WORD(IME_SCALER)
} IME_SCALER;

/**
    IME output configuration
*/
typedef struct _STITCH_IMG {
	UINT32      LineOfsY;       ///< path output lineoffset for separating output
	UINT32      LineOfsCC;      ///< path output lineoffset for separating output
	UINT32      StPos;          ///< path output separating position
} STITCH_IMG;

typedef struct _STITCH_INFO {
	IMEADDR     Addr;           ///< path output DMA address for separating output
	STITCH_IMG  Img;            ///< stitch image information
} STITCH_INFO;

typedef enum _IME_HAL_SINFO_FMT {
	IME_HAL_SINFO_MT_HEVC = 0,  ///< 8x8
	IME_HAL_SINFO_MT_H264 = 1,  ///< 16x16
} IME_HAL_SINFO_FMT;

typedef struct _IME_HAL_CODEC_KT_INFO {
	UINT32          uiKt0;   ///< K-table information for codec
	UINT32          uiKt1;   ///< K-table information for codec
	UINT32          uiKt2;   ///< K-table information for codec
} IME_HAL_CODEC_KT_INFO;

typedef struct _IME_HAL_ENCODE_INFO_ {
	BOOL                    EncodeEn;           ///< encoder enable
	IME_HAL_SINFO_FMT       EncodeSInfoFmt;     ///< encode side-information output format
	IME_HAL_CODEC_KT_INFO   EncodeKTab;         ///< K-table information for encoder
	BOOL                    EncodeClearKTabEn;  ///< encoder clear K-table enable
	BOOL                    EncodeUpdateKTabEn; ///< encoder clear K-table enable
	UINT32                  uiEncodeSideInfoAddr;   ///< encoder side information
} IME_HAL_ENCODE_INFO;

typedef struct _OUTCH {
	BOOL    PathEn;         ///< path enable
	BOOL    DmaOutEn;       ///< data output to dram enable
	BOOL    StitchEn;       ///< path separated output to dram enable, also set "OutPathSprtPos"
	IMEFMT  Fmt;            ///< format
	IMESIZE Size;           ///< scale output size
	struct {
		Coordinate Start;  ///< -
		UINT32  SizeH;      ///< -
		UINT32  SizeV;      ///< -
	} Crop;                 ///< ime output crop window
	IMEADDR Addr;           ///< YCC address
	IME_SCALER ScalMethod;  ///< CbCr channel have no bicubic, scaling up have no integration(all 3 path).
	STITCH_INFO StitchInfo; ///< stitch information
} OUTCH;

/**
	IME Scale Method Select
*/
typedef struct _IME_HAL_SCL_SEL {
	UINT32 scl_th;			///< scale done ratio threshold, [31..16]output, [15..0]input
	IME_SCALER method_l;	///< scale method when scale down ratio <= scl_th(output/input)
	IME_SCALER method_h;	///< scale method when scale down ratio >  scl_th(output/input)
} IME_HAL_SCL_SEL;


/**
    IME output path
*/
typedef struct _IME_OUT {
	OUTCH Path1;            ///< path1 output (output fmt support y/(uv pack))
	OUTCH Path2;            ///< path2 output (output fmt support y/(uv pack))
	OUTCH Path3;            ///< path3 output (output fmt support y/(uv pack))
	OUTCH Path4;            ///< path4 output (output fmt support only y output)
	UINT32 Path4_EdgeAddr;  ///< path4 edge map output
	UINT32 Path4_EdgeLineOfs;   ///< path4 edge line offset
	UINT32 Path4_HistoAddr; ///< path4 histo output


	OUTCH Path5;            ///< path5 output (output fmt support y/(uv pack))

	BOOL LcaOutEn;          ///< reference image output enable
	UINT32  LcaOutAddr;     ///< reference image address(output)
	IMESIZE LcaOutSize;     ///< reference image size(output)

	IME_HAL_ENCODE_INFO encode_info;    ///< encode information, only for ime_path1
	IME_HAL_SCL_SEL scl_sel;			///< path scale method select rule
} IME_OUT;

/**
    scaling quality(enhance) parameter set

    @note: only work when ScalMethod = _Bicubic
*/
typedef struct _IMESCALE_Enh {
	UINT32  Factor;     ///< -
	UINT32  Norm_bit;   ///< -
} IMESCALE_Enh;

/**
    scaling quality(filter)parameter set
*/
typedef struct _IMESCALE_Filt {
	BOOL    CoefUserDefEn;      ///< user define mode enable bit
	struct {
		BOOL    HsfEn;          ///< H direction scaling filter
		BOOL    VsfEn;          ///< V direction scaling filter
		UINT32  HsfCoef;        ///< H direction coefficient
		UINT32  VsfCoef;        ///< V direction coefficient
	} UserDefine;               ///< user defined
} IMESCALE_Filt;

/**
    @name IME enum - Value Range Selection
*/
//@{
typedef enum _RANGE_SEL {
	RANGE_8   = 0,  ///< range 8
	RANGE_16  = 1,  ///< range 16
	RANGE_32  = 2,  ///< range 32
	RANGE_64  = 3,  ///< range 64
	RANGE_128 = 4,  ///< range 128
	ENUM_DUMMY4WORD(RANGE_SEL)
} RANGE_SEL;
//@}
/**
    chroma adaption parameter set
*/
typedef struct _IME_CHRA_ADAP {
	struct {
		UINT32          LcaRefY;        ///< Chroma reference weighting for Y channels
		UINT32          LcaRefUV;       ///< Chroma reference weighting for UV channels
		UINT32          LcaOutUV;       ///< Chroma adaptation output weighting
	} LcaRefWt;
	struct {
		RANGE_SEL       LcaY;           ///< Chroma adaptation range selection for Y channels, Max = IME_RANGE_128
		UINT32          LcaYTh;         ///< Chroma adaptation range threshold for Y channels
		RANGE_SEL       LcaYWtPrc;      ///< Chroma adaptation weighting precision for Y channel, Max = IME_RANGE_64
		UINT32          LcaYWtS;        ///< Chroma adaptation weighting start value for Y channels
		UINT32          LcaYWtE;        ///< Chroma adaptation weighting end value for Y channels
	} LcaYinfo;
	struct {
		RANGE_SEL       LcaUV;           ///< Chroma adaptation range selection for UV channels, Max = IME_RANGE_128
		UINT32          LcaUVTh;         ///< Chroma adaptation range threshold for UV channels
		RANGE_SEL       LcaUVWtPrc;      ///< Chroma adaptation weighting precision for UV channel, Max = IME_RANGE_64
		UINT32          LcaUVWtS;        ///< Chroma adaptation weighting start value for UV channels
		UINT32          LcaUVWtE;        ///< Chroma adaptation weighting end value for UV channels
	} LcaUVinfo;
	UINT32  SubRatio;       ///< sub out size: size/(1<<(SubRatio))
} IME_CHRA_ADAP;

typedef struct _IME_CHRA_CADAP {
	BOOL            LcaCLREn;           ///< Function enable
	struct {
		UINT32          LcaCaCentU;     ///< chroma adjustment reference center of U channel
		UINT32          LcaCaCentV;     ///< chroma adjustment reference center of V channel
	} LcaRefWt;

	struct {
		RANGE_SEL       LcaCLRRng;      ///< Color adaptation range selection for UV channels, Max = IME_RANGE_128
		UINT32          LcaCLRTh;       ///< Color adaptation range threshold for UV channels
		RANGE_SEL       LcaCLRWtPrc;    ///< Color adaptation weighting precision for UV channel, Max = IME_RANGE_64
		UINT32          LcaCLRWtS;      ///< Color adaptation weighting start value for UV channels
		UINT32          LcaCLRWtE;      ///< Color adaptation weighting end value for UV channels
	} LcaCLRinfo;
} IME_CHRA_CADAP;

typedef struct _IME_CHRA_LADAP {
	BOOL            LcaLumEn;           ///< Function enable

	struct {
		UINT32          LcaLumRefWt;    ///< Luma adaptation reference weighting for UV channels
		UINT32          LcaLumOutWt;    ///< Luma adaptation output weighting
	} LcaRefWt;

	struct {
		RANGE_SEL       LcaLumRng;      ///< Luma adaptation range selection for UV channels, Max = IME_RANGE_128
		UINT32          LcaLumTh;       ///< Luma adaptation range threshold for UV channels
		RANGE_SEL       LcaLumWtPrc;    ///< Luma adaptation weighting precision for UV channel, Max = IME_RANGE_64
		UINT32          LcaLumWtS;      ///< Luma adaptation weighting start value for UV channels
		UINT32          LcaLumWtE;      ///< Luma adaptation weighting end value for UV channels
	} LcaLuminfo;
} IME_CHRA_LADAP;
/**
    @name IME enum - Dark and bright chroma suppression mode
*/
//@{
typedef enum _DBCS_MODE_SEL {
	DBCS_DK_MODE  = 0,      ///< dark mode
	DBCS_BT_MODE  = 1,      ///< bright mode
	DBCS_BOTH_MODE = 2,     ///< dark and bright mode
	ENUM_DUMMY4WORD(DBCS_MODE_SEL)
} DBCS_MODE_SEL;
/**
    dark and bright region chroma supression parameters
*/
typedef struct _IME_ChromaSupp {
	UINT32 ChromaSuppEn;           ///< function eanble
	DBCS_MODE_SEL     ModeSel;     ///< Process mode
	struct {
		UINT32            uiU;      ///< Center value for U channel
		UINT32            uiV;      ///< Center value for V channel
	} Cetner;
	struct {
		UINT32            uiY;      ///< Step for luma
		UINT32            uiC;      ///<  Step for chroma
	} Step;
	UINT32            *puiWeightY;  ///< Weighting LUT for luma channel, 16 elements
	UINT32            *puiWeightC;  ///< Weighting LUT for luma channel, 16 elements
} IME_ChromaSupp;
/**
    Single Image Super-resolution setting structure
*/
//@{
typedef struct _IME_SuperRes {
	UINT32        EnFlag;     ///< enable flag
	UINT32        AutoModeEn; ///< SSR auto mode switch
	UINT32        DiagTh;     ///< Threahold for diagonal direction
	UINT32        HVDirTh;    ///< Threahold for horizontal and vertical direction
} IME_SuperRes;
/**
    color space transfor parameter set
*/
typedef struct _IME_CSPACE {
	BOOL        YUV2RGBEn;  ///< enable bit for YUV domain transfor to RGB domain
	BOOL        RGB2YUVEn;  ///< -
} IME_CSPACE;
/**
    film grain parameter set
*/
typedef struct _IME_FG {
	UINT32  EnFlag;         ///< enable flag
	UINT32  LumiThres;      ///< luminance-Noise difference threshold

	UINT32  GNInit_p1;      ///< Grain noise initial value for path1
	UINT32  Weight_p1;      ///< path1 weighting between pixel and noise value
	UINT32  GNInit_p2;      ///< Grain noise initial value for path2
	UINT32  Weight_p2;      ///< path2 weighting between pixel and noise value
	UINT32  GNInit_p3;      ///< Grain noise initial value for path3
	UINT32  Weight_p3;      ///< path3 weighting between pixel and noise value
	UINT32  GNInit_p5;      ///< Grain noise initial value for path5
	UINT32  Weight_p5;      ///< path5 weighting between pixel and noise value
} IME_FG;

/**
    3D noise reduction prarmeter set
*/
typedef struct _IME_3D_NR {
	UINT32  *pThresY;       ///< thresholds for Y channel, tab num:3, Data type:UINT32
	UINT32  *pWeightY;      ///< weightings for Y channel, tab num:4, Data type:UINT32
	UINT32  *pThresCb;      ///< thresholds for Cb channel, tab num:3, Data type:UINT32
	UINT32  *pWeightCb;     ///< weightings for Cb channel, tab num:4, Data type:UINT32
	UINT32  *pThresCr;      ///< thresholds for Cr channel, tab num:3, Data type:UINT32
	UINT32  *pWeightCr;     ///< weightings for Cr channel, tab num:4, Data type:UINT32
} IME_3D_NR;

/**
    IME structure - Data Stamp parameter setting structure
*/
//@{
/**
    @name IME enum - Data stamp mode selection
*/
//@{
typedef enum _DS_FMT_SEL {
	DS_FMT_RGB565    = 0,   ///< data stamp format RGB565
	DS_FMT_ARGB1555  = 1,   ///< data stamp format ARGB1555
	DS_FMT_ARGB4444  = 2,   ///< data stamp format ARGB4444
	ENUM_DUMMY4WORD(DS_FMT_SEL)
} DS_FMT_SEL;

//@}
typedef struct _STAMP_IMAGE_INFO {
	struct {
		UINT32 uiSizeH;             ///< horizontal size
		UINT32 uiSizeV;             ///< vertical size
	} DS_SIZE;                      ///< Image size
	DS_FMT_SEL      DsFmt;          ///< data stamp format
	Coordinate      DsBlendPos;     ///< Blending position
	UINT32          uiDSLineoffset; ///< Data lineoffset
	UINT32          uiDSAddress;    ///< Data address
} STAMP_IMAGE_INFO;


typedef struct _STAMP_IQ_INFO {
	BOOL DSColorKeyEn;       ///< color key enable
	UINT32  uiDsColorKey;           ///< Color key RGB565 format
	UINT32  uiBlendWt0;             ///< Blending weighting 0
	UINT32  uiBlendWt1;             ///< Blending weighting 1
} STAMP_IQ_INFO;

typedef struct _STAMP_SET_INFO {
	BOOL                DSEn;           ///< Function enable
	STAMP_IMAGE_INFO    DSImgInfo;      ///< Image information

	STAMP_IQ_INFO       DSColorKeyInfo; ///< Data stamp quality information
} STAMP_SET_INFO;


typedef struct _IMESTAMP_INFO {
	BOOL                DSRGB2YUVEn;        ///< RGB 2 YUV function enable
	BOOL                RGB2YUVUserdefine;  ///< user define coefficient
	UINT32              RGB2YUVCoef[4];     ///< color space transfer coefficient

	STAMP_SET_INFO      DS_Set0;        ///< data stamp set0
	STAMP_SET_INFO      DS_Set1;        ///< data stamp set1
	STAMP_SET_INFO      DS_Set2;        ///< data stamp set2
	STAMP_SET_INFO      DS_Set3;        ///< data stamp set3
} IMESTAMP_INFO;
/**
    @name IME enum - Statistical edge kernel selection
*/
//@{
typedef enum _STATS_EKER_SEL {
	STATS_EKER_HA  = 0,     ///< H map A
	STATS_EKER_HB  = 1,     ///< H map B
	STATS_EKER_VA  = 2,     ///< V map A
	STATS_EKER_VB  = 3,     ///< V map B
	ENUM_DUMMY4WORD(STATS_EKER_SEL)
} STATS_EKER_SEL;
//@}

typedef enum _IME_STATS_SRC_SEL {
	IME_STATS_SRC_SET0  = 0,  ///< source from set0
	IME_STATS_SRC_SET1  = 1,  ///< source from set1
	ENUM_DUMMY4WORD(IME_STATS_SRC_SEL)
} IME_STATS_SRC_SEL;

typedef enum _IME_STATS_HIST_SEL {
	IME_STATS_HIST_SET0  = 0,  ///< source from set0
	IME_STATS_HIST_SET1  = 1,  ///< source from set1
	ENUM_DUMMY4WORD(IME_STATS_HIST_SEL)
} IME_STATS_HIST_SEL;

typedef struct _IME_STATS_EDGE_INFO {
	BOOL            StatsEdgeKerEn0;    ///< edge kernel enable for set0
	STATS_EKER_SEL  StatsEdgeKer0;      ///< edge map kernel selection for map0
	UINT32          uiStatsShift0;      ///< edge map shift for map0

	BOOL            StatsEdgeKerEn1;    ///< edge kernel enable for set1
	STATS_EKER_SEL  StatsEdgeKer1;      ///< edge map kernel selection for map1
	UINT32          uiStatsSft1;        ///< edge map shift for map1
} IME_STATS_EDGE_INFO;

typedef struct _IME_STATS_HIST_INFO {
	IME_STATS_HIST_SEL  SetSel;             ///< operation set selection
	Coordinate          StatsHistPos;       ///< start posotion for edge statistic
	struct {
		UINT32 sizeH;                       ///< horizontal size
		UINT32 sizeV;                       ///< vertical size
	} StatsHistSize;                        ///< image size for edge statistic
	UINT32 AccTag;                          ///< histogram accumulation target
} IME_STATS_HIST_INFO;

typedef struct _IME_STATS_ROITH_INFO {
	IME_STATS_SRC_SEL     RoiSrc; ///< ///< statistical source of ROI
	UINT32  RoiTh0;     ///< statistical threshold of ROI for section0
	UINT32  RoiTh1;     ///< statistical threshold of ROI for section1
	UINT32  RoiTh2;     ///< statistical threshold of ROI for section2
} IME_STATS_ROITH_INFO;

typedef struct _IME_STATS_ROI_INFO {
	struct {
		UINT32 uiStatsRow0;  ///< row position0
		UINT32 uiStatsRow1;  ///< row posotion1
	} StatsRowPos;      ///< row position of statistic
	IME_STATS_ROITH_INFO  StatsRoi0;            ///< ROI0 for edge map
	IME_STATS_ROITH_INFO  StatsRoi1;            ///< ROI1 for edge map
	IME_STATS_ROITH_INFO  StatsRoi2;            ///< ROI2 for edge map
	IME_STATS_ROITH_INFO  StatsRoi3;            ///< ROI3 for edge map
	IME_STATS_ROITH_INFO  StatsRoi4;            ///< ROI4 for edge map
	IME_STATS_ROITH_INFO  StatsRoi5;            ///< ROI5 for edge map
	IME_STATS_ROITH_INFO  StatsRoi6;            ///< ROI6 for edge map
	IME_STATS_ROITH_INFO  StatsRoi7;            ///< ROI7 for edge map
} IME_STATS_ROI_INFO;

typedef struct _IME_STATS_INFO {
	BOOL                StatsEnable;            ///< enable of statistic
	BOOL                StatsFilterEnable;      ///< filter enable of statistic
	BOOL                StatsImgOutBF;          ///< ime out before filter
	IME_STATS_EDGE_INFO *StatsEdgeMap;          ///< edge map of statistic
	IME_STATS_HIST_INFO *StatsHistogram0;       ///< edge histogram
	IME_STATS_HIST_INFO *StatsHistogram1;       ///< edge histogram
	IME_STATS_ROI_INFO  *StatsRoi;              ///< ROI threshold of edge map
} IME_STATS_INFO;


/**
    IME enum - privacy mask type
*/
typedef enum _IME_PRI_MASK_TYPE {
	IME_PRI_MASK_TYPE_YUV = 0,  ///< Using YUV color
	IME_PRI_MASK_TYPE_PXL = 1,  ///< Using pixilation
	ENUM_DUMMY4WORD(IME_RPI_MASK_TYPE)
} IME_RPI_MASK_TYPE;

/**
    IME enum - privacy mask pixelation blocks size selection
*/
typedef enum _IME_PRI_PXL_BLK_SIZE {
	IME_PRI_PIXELATION_08 = 0,  ///< 8x8
	IME_PRI_PIXELATION_16 = 1,  ///< 16x16
	IME_PRI_PIXELATION_32 = 2,  ///< 32x32
	IME_PRI_PIXELATION_64 = 3,  ///< 64x64
	ENUM_DUMMY4WORD(IME_PRI_PXL_BLK_SIZE)
} IME_PRI_PXL_BLK_SIZE;

/**
    IME structure - privacy mask parameter of each set
*/
typedef struct _IME_PRI_MASK_ITEM {
	BOOL                EnFlag;       ///< privacy mask function enable
	Coordinate          Coord[4];     ///< privacy mask 4 coordinates, must be Convex Hull
	IME_RPI_MASK_TYPE   PmMskType;    ///< privacy mask type selection
	UINT32              PmColor[3];   ///< privacy mask using YUV color
	UINT32              AWeight;      ///< privacy mask alpha weight
} IME_PRI_MASK_ITEM;

/**
    IME structure - 3DNR paramters
*/
typedef enum _IME_HAL_3DNR_MVDIF_MODE {
	IME_HAL_3DNR_MVDIF_MODE0 = 0,   ///< off
	IME_HAL_3DNR_MVDIF_MODE1 = 1,   ///< mvdiff for fs2d
	IME_HAL_3DNR_MVDIF_MODE2 = 2,   ///< mvdiff for fs3d
	IME_HAL_3DNR_MVDIF_MODE3 = 3,   ///< sad for fs2d
	IME_HAL_3DNR_MVDIF_MODE4 = 4,   ///< sad for fs3d
	ENUM_DUMMY4WORD(IME_HAL_3DNR_MVDIF_MODE)
} IME_HAL_3DNR_MVDIF_MODE;

typedef enum _IME_HAL_3DNR_SUBMV_MODE {
	IME_HAL_3DNR_SUBMV_MODE_DROP    = 0,  ///<
	IME_HAL_3DNR_SUBMV_MODE_MIN     = 1,  ///<
	IME_HAL_3DNR_SUBMV_MODE_MAX     = 2,  ///<
	ENUM_DUMMY4WORD(IME_HAL_3DNR_SUBMV_MODE)
} IME_HAL_3DNR_SUBMV_MODE;

typedef enum _IME_HAL_3DNR_MOTION_MODE {
	IME_HAL_3DNR_MOTION_MODE_BV_SAD = 0,
	IME_HAL_3DNR_MOTION_MODE_S0_SAD = 1,
	ENUM_DUMMY4WORD(IME_HAL_3DNR_MOTION_MODE)
} IME_HAL_3DNR_MOTION_MODE;

typedef enum _IME_HAL_3DNR_MOTION_LIMIT_IN_SEL {
	IME_HAL_3DNR_MOTION_LIMIT_IN_SEL_Y = 0,  ///< from Y motion
	IME_HAL_3DNR_MOTION_LIMIT_IN_SEL_C = 1,  ///< from C motion
	ENUM_DUMMY4WORD(IME_HAL_3DNR_MOTION_LIMIT_IN_SEL)
} IME_HAL_3DNR_MOTION_LIMIT_IN_SEL;

typedef enum _IME_HAL_3DNR_MOTION_FREEZE {
	IME_HAL_3DNR_MOTION_FREEZE_CTRL     = 0, ///< use motion contorl
	IME_HAL_3DNR_MOTION_FREEZE_BYPASS   = 1, ///< by pass
	ENUM_DUMMY4WORD(IME_HAL_3DNR_MOTION_FREEZE)
} IME_HAL_3DNR_MOTION_FREEZE;

typedef struct _IME_HAL_3DNR_LUMA_FUNC_CTRL_ {
	BOOL    FreqSoftEn;     ///< freq. soft function enable
	BOOL    DitherBlendEn;  ///< dither for blending output enable
	BOOL    FullSearch2DEn; ///< full search enable for 2D
	BOOL    Trans3DEn;      ///< 3D transform enable
	BOOL    FilmAwareEn;    ///< film aware enable
	BOOL    TensorModeEn;   ///< always select bv while film mode
	BOOL    EdgeModifyEn;   ///< edge weighting enable
	BOOL    PiGmvEn;        ///<
} IME_HAL_3DNR_LUMA_FUNC_CTRL;

typedef struct _IME_HAL_3DNR_MER_PARAM_ {
	UINT32                      *puiDetailPenalty;  ///< detail penalty, 6 elements, range = [0, 15]
	UINT32                      *puiSadPenalty;     ///< sad penalty, 8 elements, range = [0, 15]

	IME_HAL_3DNR_MVDIF_MODE     MvDifMode;          ///< mv difference mode selection
	UINT32                      uiMvDifShift;       ///< mv difference shift
	UINT32                      uiMvDifCoring;      ///< mv difference coring
	UINT32                      uiMvDifRatio;       ///< mv difference ratio

	BOOL                        PeriodicEn;                     ///< periodic enable
	BOOL                        PeriodicSmallerMvDifEn;         ///< periodic prefer smaller mvdiff enable
	BOOL                        PeriodicAplsadRulEn;            ///< periodic refer to apl
	UINT32                      uiPeriodicSadSimilarDif;        ///< Periodic sad similar difference
	UINT32                      *puiPeriodicSadSimilar;         ///< Periodic sad similar, 4 elements

	IME_HAL_3DNR_SUBMV_MODE     SubMvMode;          ///< step size 4 for sub mv
	UINT32                      uiSadT0Shift;       ///< shift T0 sad, max 6 since max sad 14bit
	UINT32                      uiBvToZmv;          ///<
	UINT32                      uiZeroMvCoringTh;   ///<

	BOOL    BvMvDiffForce;                      ///< force mv difference threshold enable
	UINT32  uiBvMvDiffTh;                       ///< set force mv_diff_thd
	UINT32 *puiCandiatePosX;                    ///< mv position for horizontal direction, 7 elements
	UINT32 *puiCandiatePosY;                    ///< mv position for vertical direction, 7 elements
} IME_HAL_3DNR_MER_PARAM;

typedef struct _IME_HAL_3DNR_MOTION_INFO_ {
	IME_HAL_3DNR_MOTION_MODE    MotionSCMode;       ///< scene change 3d dist
	UINT32                      MotionSCRatio;      ///< scene change detect
	UINT32                      MotionSCCoring;     ///< scene change detect
	UINT32                      MotionSCTensor;     ///< scene change detect

	UINT32                      MotionTensorMixTh0; ///< determin edge for mix tensor & ratio
	UINT32                      MotionTensorMixTh1; ///< determin edge for mix tensor & ratio

	UINT32                      *puiMotionTensorValMap; ///< tensor remapping, 4 elements, range = [0, 63]
	UINT32                      *puiMotionMap;          ///< decide number of full search patches, 5 elements, range = [0, 63]
} IME_HAL_3DNR_MOTION_INFO;

typedef struct _IME_HAL_3DNR_LUMA_PARAM_ {
	UINT32                  uiSigma;        ///< square strength
	UINT32                  uiHardTh;       ///< sigma adjustment with tiny movement

	UINT32                  uiGlbClrTSad;           ///<
	UINT32                  uiPatchSimilarity;      ///< 3d full search patch similarity
	UINT32                  uiPatchReserveTh1;      ///< 3d full search patch sad threshold
	UINT32                  uiZeroMvCoringTh;       ///<
	UINT32                  uiBlKFlatTh;            ///< detail value threshold to judge as flat

	IME_HAL_3DNR_MER_PARAM     MerSearchInfo;    		///< 3DNR 3D mer search parameters
	IME_HAL_3DNR_MOTION_INFO    MotionInfo;             ///< 3DNR motion parameters

	UINT32                  *puiSnrRatio;   /// set SNR ratio in diff full search patch, 6 elements, range = [0, 127]
	UINT32                  *puiTnrRatio;   /// set tnr ratio in diff full search patch, 6 elements, range = [0, 127]
	UINT32                  *puiLumaGain;   /// SQUARE strength adjust at luma, 8 elements, range = [0, 255]
	UINT32                  *puiFreq;       /// set diff ratio in diff freq, 7 elements, range = [0, 127]
	UINT32                  *puiSnrFreq;    /// snr ratio depend on frequency, 4 elements, range = [0, 255]
} IME_HAL_3DNR_LUMA_PARAM;

typedef struct _IME_HAL_3DNR_LUMA_INFO_ {
	BOOL                            LumaNrEn;       ///< luma chennel enable, not used, currently follow 3dnr enable
	IME_HAL_3DNR_LUMA_FUNC_CTRL     LumaCtrlInfo;   ///< luma function control info.
	IME_HAL_3DNR_LUMA_PARAM         LumaInfo;       ///< luma parameter info.
	UINT32 uiLumaMvInAddr;      ///< motion vector input address
	UINT32 uiLumaMvOutAddr;     ///< motion vector output address
} IME_HAL_3DNR_LUMA_INFO;

typedef struct _IME_HAL_3DNR_CHROMA_FUNC_CTRL_ {
	BOOL                                ModifyDiffEn;   ///< refine the pseudo motion by strong SNR
	BOOL                                MotionExpandEn; ///< motion expand enable
	BOOL                                ExpandDiffEn;   ///< enable lpf for temporal difference
	IME_HAL_3DNR_MOTION_LIMIT_IN_SEL    MotionInSel;    ///< motion detection input using
	IME_HAL_3DNR_MOTION_FREEZE          MotionFreeze;   ///< motion control
} IME_HAL_3DNR_CHROMA_FUNC_CTRL;

typedef struct _IME_HAL_3DNR_CHROMA_PARAM_ {
	UINT32                          uiExpectNoiseLuma;  ///< expected noise strength of luma, range = [0, 1023]
	UINT32                          uiExpectNoiseChroma;    ///< expected noise strength of chroma, range = [0, 1023]
	UINT32                          uiExpandLevel;          ///<

	UINT32                          uiFalseCurve0;          ///< Low threshold of difference
	UINT32                          uiFalseCurve1;          ///< Low threshold of difference
	UINT32                          uiMixYCFalseYWet;       ///< Y/C mix luma weight for difference calculation
	UINT32                          uiMixYCFalseCWet;       ///< Y/C mix chroma weight for difference calculation
	UINT32                          uiMixYCLumaYWet;        ///< Y/C mix luma weight for luma channel
	UINT32                          uiMixYCLumaCWet;        ///< Y/C mix chroma weight for luma channel
	UINT32                          uiMixYCChromaYWet;      ///< Y/C mix luma weight for chroma channel
	UINT32                          uiMixYCChromaCWet;      ///< Y/C mix chroma weight for chroma channel

	UINT32                          *puiLumaGain;   ///< TNR strength adjust at luma, 8 elements, range = [0, 255]
	UINT32                          *puiMotionCtrl; ///< mapped motion value of Xth motion level, 16 elements, range = [0, 255]
	UINT32                          *puiMap2DWets;  ///< 4 elements, puiMap2DWeights[0]: chroma threshold for still; puiMap2DWeights[1]: chroma weighting for HSNR when still; puiMap2DWeights[2]: chroma threshold for motion; puiMap2DWeights[3]: chroma curve slope
	UINT32                          *puiMap3DWets;  ///< 4 elements, puiMap3DWeights[0]: chroma threshold for still; puiMap3DWeights[1]: chroma weighting for HSNR when still; puiMap3DWeights[2]: chroma threshold for motion; puiMap3DWeights[3]: chroma curve slope
	UINT32                          *puiMixTh;              ///< luma threshold to mix the motion detect value, and chroma threshold to mix the motion detect value, 4 elements
	UINT32                          *puiLevelMap;           ///< Memc level mapping for motion noise, 16 elements
} IME_HAL_3DNR_CHROMA_PARAM;

typedef struct _IME_HAL_3DNR_Y_PARAM_ {
	UINT32                          uiExpectNoiseStrth;  ///< Y expected noise strength in spatial, range = [0, 1023]
	UINT32                          *puiMap2DWets;  ///< 4 elements, puiMap2DWeights[0]: Y threshold for still; puiMap2DWeights[1]: Y weighting for HSNR when still; puiMap2DWeights[2]: Y threshold for motion; puiMap2DWeights[3]: Y curve slope
	UINT32                          *puiMap3DWets;  ///< 4 elements, puiMap3DWeights[0]: Y threshold for still; puiMap3DWeights[1]: Y weighting for HSNR when still; puiMap3DWeights[2]: Y threshold for motion; puiMap3DWeights[3]: Y curve slope
} IME_HAL_3DNR_Y_PARAM;

typedef struct _IME_HAL_3DNR_CHROMA_INFO_ {
	BOOL                            ChromaNrEn;             ///< chroma chennel enable, not used, currently follow 3dnr enable
	IME_HAL_3DNR_CHROMA_FUNC_CTRL   ChromaCtrlInfo;         ///< chroma function control info.
	IME_HAL_3DNR_CHROMA_PARAM       ChromaInfo;             ///< chroma parameter info
	UINT32                          uiChromaNrMoInLofs;     ///< motion input lineoffset
	UINT32                          uiChromaNrMoInAddr;     ///< motion input address
	UINT32                          uiChromaNrMoOutLofs;    ///< motion output lineoffset
	UINT32                          uiChromaNrMoOutAddr;    ///< motion output address
	BOOL							YNrEn;					///< Y Channel nr enable, not used, currently follow 3dnr enable
	IME_HAL_3DNR_Y_PARAM			YInfo;					///< Y Channel nr parameters
} IME_HAL_3DNR_CHROMA_INFO;

typedef struct _IME_HAL_3DNR_DECODE_INFO_ {
	BOOL                    DecodeEn;               ///< 3DNR input reference decoder enable
	IME_HAL_CODEC_KT_INFO   DecodeKTab;             ///< K-table information for decoder
	BOOL                    EncodeClearKTabEn;      ///< encoder clear K-table enable
	BOOL                    EncodeUpdateKTabEn;     ///< encoder clear K-table enable
	UINT32                  uiDecodeSideInfoAddr;   ///< decoder side information
} IME_HAL_3DNR_DECODE_INFO;

typedef struct _IME_HAL_3DNR_INFO_ {
	BOOL                        TplNrEn;        ///< 3DNR enable, not used, currently use imeFuncEn(SQUAREEn) to Enable
	IME_HAL_3DNR_LUMA_INFO      TplLuma;        ///< 3DNR for luminance channel
	IME_HAL_3DNR_CHROMA_INFO    TplChroma;      ///< 3DNR for chroma channel
	UINT32                      PrvImgLofs_Y;   ///< 3DNR buffer lineoffset of preivous image
	UINT32                      PrvImgLofs_Cb;  ///< 3DNR buffer lineoffset of preivous image
	IMEADDR                     PrvImgAddr;     ///< 3DNR buffer address of preivous image
	IME_HAL_3DNR_DECODE_INFO    TplDecode;      ///< 3DNR reference input decode
} IME_HAL_3DNR_INFO;

/**
    IME structure - privacy mask parameters
*/
typedef struct _IME_PRI_MASK_INFO {
	IME_PRI_MASK_ITEM PmSet0;                    ///< privacy mask set0
	IME_PRI_MASK_ITEM PmSet1;                    ///< privacy mask set1
	IME_PRI_MASK_ITEM PmSet2;                    ///< privacy mask set2
	IME_PRI_MASK_ITEM PmSet3;                    ///< privacy mask set3
	IME_PRI_MASK_ITEM PmSet4;                    ///< privacy mask set4
	IME_PRI_MASK_ITEM PmSet5;                    ///< privacy mask set5
	IME_PRI_MASK_ITEM PmSet6;                    ///< privacy mask set6
	IME_PRI_MASK_ITEM PmSet7;                    ///< privacy mask set7

	IME_PRI_PXL_BLK_SIZE PmPxlBlkSize;   ///< block size of pixelation
	UINT32               PxlWidth;
	UINT32               PxlHeight;
	UINT32               PmPxlLofs;      ///< pixelation input image lineoffset
} IME_PRI_MASK_INFO;

/**
    IME Path enable info for ime_read
*/
typedef struct {
	UINT32 path_enable[5];      ///< path enable (path1~5)
	UINT32 dma_enable[5];       ///< dma enable (path1~5)
	UINT32 stitch_enable[5];    ///< stitch enalbe (path1~5)
} IME_PATHEN_INFO;

//@}
typedef struct _IME_SUBFUNC {
	ImeInteEn   imeInteEn;  ///< interrupt enable
	ImeFuncEn   imeFuncEn;  ///< function enable

	ImeFuncEn   ime_func_enable;
	ImeFuncEn   ime_func_disable;

	struct {
		IME_CHRA_ADAP   *pChromaAdaption; ///< -   Function quality information for chroma channel
		IME_CHRA_LADAP  *pChromaLumAdaption;///< - Function quality information for luma channel
		IME_CHRA_CADAP  *pChromaColorAdaption;///< -Chroma adjustment information

		BOOL    bypass;     ///< bypass input path image data, only reference image is used
	} ChromaAdap;

	IME_FG          *pFilmGrain;    ///< film grain
	IME_ChromaSupp  *pChromaSuppres;///< chroma suppression info
	IME_SuperRes    *pSuperRes;     ///< super resolution info
	IMESCALE_Enh    *pP1_Enh;       ///< scaler quality enhancement for path1
	IMESCALE_Enh    *pP2_Enh;       ///< scaler quality enhancement for path2
	IMESCALE_Enh    *pP3_Enh;       ///< scaler quality enhancement for path3
	IMESCALE_Enh    *pP4_Enh;       ///< scaler quality enhancement for path4
	IMESCALE_Enh    *pP5_Enh;       ///< scaler quality enhancement for path5


	IMESCALE_Filt   *pP1_Filt;      ///< scaling filter for path1
	IMESCALE_Filt   *pP2_Filt;      ///< scaling filter for path2
	IMESCALE_Filt   *pP3_Filt;      ///< scaling filter for path3
	IMESCALE_Filt   *pP4_Filt;      ///< scaling filter for path4
	IMESCALE_Filt   *pP5_Filt;      ///< scaling filter for path5

	IMESTAMP_INFO   *pStampInfo;    ///< Data stamp parameters
	IME_STATS_INFO  *pStatsInfo;    ///< Edge statistic parameters
	IME_PRI_MASK_INFO *pPmInfo;     ///< Privacy mask paramters
	IME_HAL_3DNR_INFO *pSquareInfo; ///< SQUARE paramters
} IME_SUBFUNC;

#define SCALING_ISD_STP_OUT_H_LIMIT	(1024)
#define SCALING_ISD_MIN_RATIO (1599) // minimum ratio is scale down to 1/15.99x

//@}
#endif
