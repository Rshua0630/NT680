#ifndef _IQ_PARAM_CTRL_H_
#define _IQ_PARAM_CTRL_H_
/**
    Internal header file for IPP_sensor

    Header files for differnt IPP_sensor

    @file       iq_param_ctrl.h
    @ingroup    Lib\DevCam\iqs_common
    @note       Nothing (or anything need to be mentioned).

    Copyright   Novatek Microelectronics Corp. 2017.  All rights reserved.
*/
#include <string.h>
#include <stdio.h>

#include "ipl_cmd.h"
#include "ipl_header_infor_raw.h"
#include "ipl_ctrl_par.h"

#include "Type.h"
#include "h264enc_api.h"
#include "DefogLib.h"
#include "SceneRenderLib.h"
#include "SensorHDRLib.h"

/**
    Define IQS Tuning Table Size.
*/
typedef enum {
	SENID1,
	SENID2,
	SENID3,
	SENID4,
	SENID5,
	SENID6,
	SENID7,
	SENID8,
	Total_SENIDNum
} IQS_SENIDNumEnum;

#define ISP_IQ_MAX_NUM      Total_SENIDNum

typedef enum {
    IQS_PARAM_TABLE_UNKONWN,
	IQS_PARAM_TABLE_1,
	IQS_PARAM_TABLE_2,
	IQS_PARAM_TABLE_3,
	IQS_PARAM_TABLE_4,
	IQS_PARAM_TABLE_5,
	IQS_PARAM_TABLE_6,
	IQS_PARAM_TABLE_7,
	IQS_PARAM_TABLE_8,
	IQS_PARAM_TABLE_MAX,
} iq_param_table;

#define ISP_IQ_MAX_TABLE_NUM IQS_PARAM_TABLE_MAX

#define IQS_ISOMAPTAB_NUM   12

/**
    IQS_FLOW_MSG.
*/
typedef enum {
	IQCB_PRV_START = 0x00000000,
	IQCB_PRV_SIE_INIT,
	IQCB_PRV_IPE_INIT,
	IQCB_PRV_IFE_INIT,
	IQCB_PRV_DCE_INIT,
	IQCB_PRV_IFE2_INIT,
	IQCB_PRV_IME_INIT,
	IQCB_PRV_ISE_INIT,
	IQCB_PRV_RDE_INIT,
	IQCB_PRV_RHE_INIT,
	IQCB_PRV_END,

	IQCB_CAP_SUB_START = 0x00000100,
	IQCB_PREIME_D2D_SIE_SUB,
	IQCB_PREIME_D2D_DCE_SUB,
	IQCB_PREIME_D2D_IFE_SUB,
	IQCB_PREIME_D2D_IPE_SUB,
	IQCB_PREIME_D2D_IME_SUB,
	IQCB_D2D_IFE2_SUB,
	IQCB_PREIME_D2D_RHE_SUB,
	IQCB_CAP_SUB_END,

	IQCB_CAP_PRI_START = 0x00000200,
	IQCB_PREIME_D2D_SIE_PRI,
	IQCB_PREIME_D2D_DCE_PRI,
	IQCB_PREIME_D2D_IFE_PRI,
	IQCB_PREIME_D2D_IPE_PRI,
	IQCB_PREIME_D2D_IME_PRI, //enable LCA
	IQCB_D2D_IFE2_PRI,
	IQCB_D2D_DRE_PRI,
	IQCB_PREIME_D2D_RHE_PRI,
	IQCB_CAP_PRI_END,

	IQS_FLOW_START = 0x00000400,
	IQS_AE_END,
	IQS_AWB_END,
	IQS_OZOOM_END,
	IQS_DZOOM_END,
	IQS_WDR_END,
	IQS_SHDR_END,
	IQS_IMAGE_EFFECT,
	IQS_MANUAL_TRIG,
	IQS_AUTO_TRIG,
	IQS_REORG_APPINFO,
	IQS_FLOW_MSG_NUM,

	IQCB_DCE_LUT_SIDE_FULL= 0x00000500, //for wall full and depth //IQCB_DCE_LUT00 = 0x00000500,
	IQCB_DCE_LUT_SIDE_HORI, //for horizontal dewarp //IQCB_DCE_LUT01,
	IQCB_DCE_LUT_TOP, //for top view //IQCB_DCE_LUT02,
	IQCB_DCE_LUT_USER,// other case   IQCB_DCE_LUT03,
} IQS_FLOW_MSG;

typedef struct _IQS_REFIMG_INFO {
	UINT32 width;
	UINT32 height;
	UINT32 vratio;
	UINT32 hratio;
} IQS_REFIMG_INFO;

/**
    Typedef struct for IQ Parameters.
*/
typedef struct _ISOMAP {
	UINT32  ISO;
	UINT32  Index;
} ISOMAP;

typedef enum {
	ECS_OFF,
    ECS_ON,

    ENUM_DUMMY4WORD(ECS_SWITCH)
} ECS_SWITCH;

typedef struct _ECS_CTRL {
    ECS_SWITCH enable;
    INT32 ratio;
} ECS_CTRL;

typedef enum {
	VIG_OFF,
    VIG_ON,

    ENUM_DUMMY4WORD(VIG_SWITCH)
} VIG_SWITCH;

typedef struct _VIG_CTRL {
    VIG_SWITCH enable;
    INT32 ratio;
} VIG_CTRL;

typedef enum {
	GDC_OFF,
    GDC_ON,

    ENUM_DUMMY4WORD(GDC_SWITCH)
} GDC_SWITCH;

typedef struct _GDC_CTRL {
    GDC_SWITCH enable;
    INT32 ratio;
} GDC_CTRL;

typedef enum {
	CAC_OFF,
    CAC_ON,

    ENUM_DUMMY4WORD(CAC_SWITCH)
} CAC_SWITCH;

typedef struct _CAC_CTRL {
    CAC_SWITCH enable;
    INT32 ratio;
} CAC_CTRL;

typedef enum {
	CCM_OFF,
    CCM_ON,

    ENUM_DUMMY4WORD(CCM_SWITCH)
} CCM_SWITCH;

typedef struct _CCM_CTRL {
    CCM_SWITCH enable;
    INT32 choice;
} CCM_CTRL;

typedef enum {
	CC3D_OFF,
    CC3D_ON,

    ENUM_DUMMY4WORD(CC3D_SWITCH)
} CC3D_SWITCH;

typedef struct _CC3D_CTRL {
    CC3D_SWITCH enable;
    INT32 choice;
} CC3D_CTRL;

typedef enum {
	CCTRL_OFF,
    CCTRL_ON,

    ENUM_DUMMY4WORD(CCTRL_SWITCH)
} CCTRL_SWITCH;

typedef struct _CCTRL_CTRL {
    CCTRL_SWITCH enable;
    INT32 sat_ratio;
    INT32 con_ratio;
    INT32 bri_ratio;
} CCTRL_CTRL;

typedef enum {
	HSV_OFF,
    HSV_ON,

    ENUM_DUMMY4WORD(HSV_SWITCH)
} HSV_SWITCH;

typedef struct _HSV_CTRL {
    HSV_SWITCH enable;
    INT32 angle;
} HSV_CTRL;

typedef enum {
	GAMMA_OFF,
    GAMMA_ON,

    ENUM_DUMMY4WORD(GAMMA_SWITCH)
} GAMMA_SWITCH;

typedef struct _GAMMA_CTRL {
    GAMMA_SWITCH enable;
    INT32 choice;
} GAMMA_CTRL;

typedef enum {
	Y_CURVE_OFF,
    Y_CURVE_ON,

    ENUM_DUMMY4WORD(Y_CURVE_SWITCH)
} Y_CURVE_SWITCH;

typedef struct _Y_CURVE_CTRL {
    Y_CURVE_SWITCH enable;
    INT32 choice;
} Y_CURVE_CTRL;

typedef enum {
	EDGE_OFF,
    EDGE_ON,

    ENUM_DUMMY4WORD(EDGE_SWITCH)
} EDGE_SWITCH;

typedef struct _EDGE_CTRL {
    EDGE_SWITCH enable;
    INT32 ratio;
} EDGE_CTRL;

typedef enum {
	OUTL_OFF,
    OUTL_ON,

    ENUM_DUMMY4WORD(OUTL_SWITCH)
} OUTL_SWITCH;

typedef struct _OUTL_CTRL {
    OUTL_SWITCH enable;
    INT32 ratio;
} OUTL_CTRL;

typedef enum {
	RANGE_NR_OFF,
    RANGE_NR_ON,

    ENUM_DUMMY4WORD(RANGE_NR_SWITCH)
} RANGE_NR_SWITCH;

typedef struct _RANGE_NR_CTRL {
    RANGE_NR_SWITCH enable;
    INT32 ratio;
} RANGE_NR_CTRL;

typedef enum {
	GBAL_OFF,
    GBAL_ON,

    ENUM_DUMMY4WORD(GBAL_SWITCH)
} GBAL_SWITCH;

typedef struct _GBAL_CTRL {
    GBAL_SWITCH enable;
    INT32 ratio;
} GBAL_CTRL;

typedef enum {
	LPF_OFF,
    LPF_ON,

    ENUM_DUMMY4WORD(LPF_SWITCH)
} LPF_SWITCH;

typedef struct _LPF_CTRL {
    LPF_SWITCH enable;
    INT32 ratio;
} LPF_CTRL;

typedef enum {
	LCA_OFF,
    LCA_ON,

    ENUM_DUMMY4WORD(LCA_SWITCH)
} LCA_SWITCH;

typedef struct _LCA_CTRL {
    LCA_SWITCH enable;
    INT32 ratio;
} LCA_CTRL;

typedef enum {
	LCA_C_OFF,
    LCA_C_ON,

    ENUM_DUMMY4WORD(LCA_C_SWITCH)
} LCA_C_SWITCH;

typedef struct _LCA_C_CTRL {
    LCA_C_SWITCH enable;
    INT32 ratio;
} LCA_C_CTRL;

typedef enum {
	LCA_Y_OFF,
    LCA_Y_ON,

    ENUM_DUMMY4WORD(LCA_Y_SWITCH)
} LCA_Y_SWITCH;

typedef struct _LCA_Y_CTRL {
    LCA_Y_SWITCH enable;
    INT32 ratio;
} LCA_Y_CTRL;

typedef enum {
	SQUARE_OFF,
    SQUARE_ON,

    ENUM_DUMMY4WORD(SQUARE_SWITCH)
} SQUARE_SWITCH;

typedef struct _SQUARE_CTRL {
    SQUARE_SWITCH enable;
    INT32 level;
} SQUARE_CTRL;

typedef enum {
	TNR_OFF,
    TNR_ON,

    ENUM_DUMMY4WORD(TNR_SWITCH)
} TNR_SWITCH;

typedef struct _TNR_CTRL {
    TNR_SWITCH enable;
    INT32 level;
} TNR_CTRL;

typedef enum {
	LCE_OFF,
    LCE_ON,

    ENUM_DUMMY4WORD(LCE_SWITCH)
} LCE_SWITCH;

typedef struct _LCE_CTRL {
    LCE_SWITCH enable;
    INT32 ratio;
} LCE_CTRL;

typedef enum {
	RESERVED_OFF,
    RESERVED_ON,

    ENUM_DUMMY4WORD(RESERVED_SWITCH)
} RESERVED_SWITCH;

typedef struct _RESERVED_CTRL {
    RESERVED_SWITCH enable;
    INT32 ratio;
} RESERVED_CTRL;

#define NON_SUPPORT  0x7FFFFFFF

typedef struct _ENGINE_CTRL {
    ECS_CTRL ecs;
    VIG_CTRL vig;
    GDC_CTRL gdc;
    CAC_CTRL cac;

    CCM_CTRL ccm;
    CC3D_CTRL cc3d;
    CCTRL_CTRL cctrl;
    HSV_CTRL hsv;

    GAMMA_CTRL gamma;
    Y_CURVE_CTRL y_curve;
    EDGE_CTRL edge;

    OUTL_CTRL outl;
    RANGE_NR_CTRL range_nr;
    GBAL_CTRL gbal;
    LPF_CTRL lpf;

    LCA_CTRL lca;
    LCA_C_CTRL lca_c;
    LCA_Y_CTRL lca_y;
    SQUARE_CTRL square;
    TNR_CTRL tnr;

    LCE_CTRL lce;

    RESERVED_CTRL reserved2;
    RESERVED_CTRL reserved3;
    RESERVED_CTRL reserved4;
    RESERVED_CTRL reserved5;
    RESERVED_CTRL reserved6;
    RESERVED_CTRL reserved7;
    RESERVED_CTRL reserved8;
    RESERVED_CTRL reserved9;
    RESERVED_CTRL reserved10;
    RESERVED_CTRL reserved11;
    RESERVED_CTRL reserved12;
} ENGINE_CTRL;

typedef struct {
	UINT8 mode_auto;
	UINT8 mode_manual;
} IQS_WDR_LCE_OFFSET;

typedef struct _IQS_LCE_
{
	UINT32	uiLceLevel;		///< local contast enhancement strength level, range = 0 ~ 128
	UINT32	uiLceClamp;		///< local contast enhancement gain clamp, range = 64 ~ 255
} IQS_LCE;

typedef struct _RANGE_FILT_CLAMP_PARAM {
	struct {
		UINT32  Th;         ///< threshold
		UINT32  Mul;        ///< weighting multiplier
		UINT32  Dlt;        ///< threshold adjustment
	} Clamp;                ///< clamp configuration

	UINT32  Rth_W;          ///< range filter and spatial filter weighting
	UINT32  Bin;            ///< range:0~7.     ( Denominator: 2<<bin  )
	UINT8   BitDither;      ///< Select bit dithering while truncation
} RANGE_FILT_CLAMP_PARAM;

typedef struct {
	UINT32 CenterXOffset;
	UINT32 CenterYOffset;
} IQS_VIG_CENTER_RATIO;

typedef enum {
	VIG_TAB_GAIN_1X,
	VIG_TAB_GAIN_2X,
	VIG_TAB_GAIN_4X,
	VIG_TAB_GAIN_8X,
} IQS_VIG_TAB_GAIN;

typedef struct {
	UINT32 TabGain;
	UINT32 Tab[17];
} IQS_VIG_SET;

typedef struct {
	UINT16 iso_l;
	UINT16 iso_h;
} IQS_VIG_ISO_TH;

typedef struct {
	UINT16 Low;
	UINT16 Middle;
	UINT16 High;
} IQS_COLOR_TEMPERATURE_TH;

typedef struct {
	UINT16 IQS_CC[9];
	UINT8 IQS_hueTab[24];
	INT32 IQS_satTab[24];
	INT32 IQS_intTab[24];
} IQS_COLOR_TEMPERATURE_PARAM;

typedef struct {
	IPE_CCtrlSel CctrlSel;
	UINT8 uiVdetDiv;
} IQS_CCTRL_INFO;

typedef struct {
	UINT16 DAY;
	UINT16 NIGHT;
} IQS_GAMMA_LV_TH;

typedef struct {
	UINT32 RepNum;
	IFE2_Filter_Param Filter;
} IQS_IFE2_FILTER;

typedef struct _DRE_NR_IQPARAM {
	UINT32 IQS_Msnr_Filt_Weight[6];
	UINT32 IQS_Msnr_Filt_RngThr_Y[8];
	UINT32 IQS_Msnr_Filt_RngThr_U[8];
	UINT32 IQS_Msnr_Filt_RngThr_V[8];
	UINT32 IQS_Msnr_GamLutY_En;
	UINT32 IQS_Msnr_GamLutU_En;
	UINT32 IQS_Msnr_GamLutV_En;
	UINT32 IQS_Msnr_GamLut_Y[128];
	UINT32 IQS_Msnr_GamLut_U[128];
	UINT32 IQS_Msnr_GamLut_V[128];
	UINT32 IQS_Msnr_YCMLut_Y[16];
	UINT32 IQS_Msnr_YCMLut_C[16];
	DRE_YCMOD_CTRL_INFO IQS_Msnr_YCMCtrl;
} IQS_DRE_NR_IQPARAM;

typedef struct _DRE_IQPARAM {
	//NR
	UINT32 uiProcRepNum;
	UINT32 uiProcRepIdx[DRE_NR_PROC_MAX_NUM];
	BOOL bDgEn;
	BOOL bDgImgRef;
	UINT32 uiDgLyrNum;
	UINT32 DgMskInIdx[DRE_DG_PROC_MAX_NUM];
} IQS_DRE_IQPARAM;
/************************************************************/

/**
    All IQ Items.
*/
typedef enum  {
	IQ_MODE = 0,
	IQ_ISOIDX,

	IQ_ISOIDX_MAP,

    IQ_ENGINE_CTRL,

	////////////////////////////////
	//             SIE             /
	////////////////////////////////
	//**  SIE-OB
	IQ_OBSUB,

	//**  SIE-ECS
	IQ_ECS_TAB,
	IQ_ECS,

	////////////////////////////////
	//             RHE             /
	////////////////////////////////
	IQ_WDR,
	IQ_WDR_LCE_OFFSET,
	IQ_LCE,

	////////////////////////////////
	//             IFE             /
	////////////////////////////////
	//**  IFE-Outlier
	IQ_OUTL_BRI,
	IQ_OUTL_DARK,
	IQ_OUTL_CNT,
	IQ_OUTL,

	//**  IFE-Spatial
	IQ_SPATIAL_W,
	IQ_SPATAL,

	//**  IFE-NLM
	IQ_NLM_KER_RAD,
	IQ_NLM_KER,
	IQ_NLM_MW,
	IQ_NLM_BWA,
	IQ_NLM_BWB,
	IQ_NLM_BWC,
	IQ_NLM_BWBL,
	IQ_NLM_BWBM,
	IQ_NLM_BWBH,

	//**  IFE-Range
	IQ_RANGEA_THR,
	IQ_RANGEA_LUT,
	IQ_RANGEB_THR,
	IQ_RANGEB_LUT,
	IQ_RANGEB,

	//**  IFE-CLAMP
	IQ_CLAMP,

	//**  IFE-Vig
	IQ_VIG_CENTER,
	IQ_VIG_TAB,
	IQ_VIG_ISO_TH,

	//**  IFE-Gbal
	IQ_GBAL_STAB,
	IQ_GBAL_DTAB,
	IQ_GBAL_IRSUB,
	IQ_GBAL,

	////////////////////////////////
	//             DCE             /
	////////////////////////////////
	//**  DCE-DIST
	IQ_DCE_DIST,

	//**  DCE-RADIOUS
	IQ_DCE_RADIOUS,

	//**  DCE-CFA
	IQ_CFAINTER,

	//**  DCE-GDC
	IQ_GDC_FOV,
	IQ_GDC_GLUT,
	IQ_GDC_RLUT,
	IQ_GDC_BLUT,
	IQ_DCE_ENH,

	//**  DCE-CAC
	IQ_DCE_CA,

	//**  DCE-2DLUT
	IQ_DCE_2D_PARAM,
	IQ_DCE_2DLUT00,
	IQ_DCE_2DLUT01,
	IQ_DCE_2DLUT02,
	IQ_DCE_2DLUT03,

    //**  DCE-IPL STRIPE INFO
	IQ_DCE_STRP_INFO,

	////////////////////////////////
	//             IPE             /
	////////////////////////////////
	//**  IPE-Edge EXT
	IQ_EDGEKER,
	IQ_DIR_TAB,
	IQ_DIREDGE_EXT,
	IQ_DIRSCORETAB,
	IQ_DIRSCORETH,
	IQ_DECON,
	IQ_EWA,
	IQ_EWB,
	IQ_EWPARAM,

	//**  IPE-Edge Enh
	IQ_EDGEENH,

	//**  IPE-Edge Map
	IQ_EDMAP,
	IQ_EDTAB,
	IQ_ESMAP,
	IQ_ESTAB,

	//**  IPE-LPF
	IQ_RGBLPF,

	//**  IPE-CC
	IQ_CCM_TH,
	IQ_Color_L,
	IQ_Color_M,
	IQ_Color_H,
	IQ_FSTAB,
	IQ_FDTAB,
	IQ_CC,

	//**  IPE-Ycurve
	IQ_Y_CURVE,

	//**  IPE-Gamma
	IQ_GAMMA_TH,
	IQ_GAMMA_DAY,
	IQ_GAMMA_NIGHT,

	//**  IPE-3DCC
	IQ_3DCC_TAB,
	IQ_3DCC_ROUND,

	//**  IPE-CCtrl
	IQ_EDGTAB,
	IQ_DDSTAB,

	IQ_SATURATION,
	IQ_CONTRAST,
	IQ_CCTRL_INFO,

	//**  IPE-CST
	IQ_CST_COEF,
	IQ_CST_PARAM,

	//**  IPE-YCCFIX
	IQ_YCCFIX,

	//**  IPE-NoiseParam
	IQ_NOISE_PARAM,

	//**  IPE-GammaRand
	IQ_GAMMA_RAND,

	////////////////////////////////
	//             IFE2            /
	////////////////////////////////
	//**  IFE2-ReferenceCenter
	IQ_REFCENTER_YTH,
	IQ_REFCENTER_YWT,
	IQ_REFCENTER_Y,
	IQ_REFCENTER_UVTH,
	IQ_REFCENTER_UVWT,
	IQ_REFCENTER_UV,

	//**  IFE2-Filter
	IQ_IFE2FILTER_YTH,
	IQ_IFE2FILTER_YWT,
	IQ_IFE2FILTER_UVTH,
	IQ_IFE2FILTER_UVWT,
	IQ_IFE2FILTER,

	//**  IFE2-Gray
	IQ_GRAY,

	////////////////////////////////
	//             IME             /
	////////////////////////////////
	//**  IME-LCA
	IQ_CHROMAADAPT,
	IQ_CHROMAADAPT_COLOR,
	IQ_CHROMAADAPT_LUM,

	//**  IME-ChromaSuppression
	IQ_CHROMASUPP_WEIGHTY,
	IQ_CHROMASUPP_WEIGHTC,
	IQ_CHROMASUPP,

	//**  IME-SuperResolution
	IQ_SUPER_RES,

	//**  IME-SQUARE
	IQ_SQ_3DNR_LUMA_PARAM,
	IQ_SQ_3DNR_CHROMA_PARAM,
	IQ_SQ_3DNR_CHROMA_Y_PARAM,

	////////////////////////////////
	//            Codec            /
	////////////////////////////////
	IQ_NUM_3DNR_PARAM,
	IQ_3DNR_LEVEL,
	IQ_3DNR_PARAM,

	////////////////////////////////
	//             HDR             /
	////////////////////////////////
    IQ_HDR,

	////////////////////////////////
	//            Defog            /
	////////////////////////////////
	IQ_DEFOG_INIT_PARAM,
	IQ_DEFOG_WET,

	////////////////////////////////
	//            GCE              /
	////////////////////////////////
	IQ_GCE,
	IQ_PARAM_MAX
} IQ_PARAM_ITEM;

/**
    EXT  iq_func.c
*/
extern void iq_get_ref_image_info(UINT32 Id, INT32 SensorMode, IQS_REFIMG_INFO *Info);
extern UINT32 iq_get_inv_gamma_value(UINT32 Id, UINT32 Data10Bit);

/**
    iq_param_ctrl.c
*/
extern void iq_set_param_addr(UINT32 id, UINT32 mode_idx, UINT32** addr);
extern void iq_get_param_addr(UINT32 id, UINT32 mode_idx, UINT32** addr);

#endif
