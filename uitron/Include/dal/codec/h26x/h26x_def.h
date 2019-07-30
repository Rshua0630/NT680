#ifndef _H26X_DEF_H_
#define _H26X_DEF_H_

#include "Type.h"

#define H26X_MAX_WIDTH      (5120)
#define H26X_MAX_HEIGHT     (5120)
#define H26X_MIN_MB         (16)
#define H26X_MAX_BSDMA_NUM  (H26X_MAX_WIDTH/H26X_MIN_MB)
#define H26X_MAX_ROI_W      (10)

#define H26XENC_BS_OUTBUF_MAX_SIZE 0x800000

#define VIR_2_PHY_ADDR(tmp)     (tmp & 0x1FFFFFFF)

#define SIZE_64X(a)             (((a + 63)>>6)<<6)
#define SIZE_32X(a)             (((a + 31)>>5)<<5)
#define SIZE_16X(a)             (((a + 15)>>4)<<4)
#define SIZE_4X(a)              (((a + 3)>>2)<<2)

#define H26XENC_SUCCESS         1
#define H26XENC_FAIL            0


#define H26X_SWAP(A,B,TT) {TT TMP;\
		TMP=A;\
		A=B;\
		B=TMP;}

#define IS_ISLICE(X) (X == I_SLICE || X == IDR_SLICE)
#define IS_PSLICE(X) (X == P_SLICE)

extern void h26x_setSystemReset(void);

typedef enum {
	PROFILE_BASELINE      =  66,
	PROFILE_MAIN          =  77,
	PROFILE_EXTENDED      =  88,
	PROFILE_HIGH          = 100,
	PROFILE_HIGH10        = 110,
} PROFILE_TYPE;

typedef enum
{
    HEVC_PROFILE_NONE = 0,
    HEVC_PROFILE_MAIN = 1,
    HEVC_PROFILE_MAIN10 = 2,
    HEVC_PROFILE_MAINSTILLPICTURE = 3,
}HEVC_PROFILE_TYPE;


typedef enum
{
    HEVC_TIER_MAIN = 0,
    HEVC_TIER_HIGH = 1,
}HEVC_TIER_TYPE;

typedef enum
{
    HEVC_LEVEL_NONE     = 0,
    HEVC_LEVEL_LEVEL1   = 30,
    HEVC_LEVEL_LEVEL2   = 60,
    HEVC_LEVEL_LEVEL2_1 = 63,
    HEVC_LEVEL_LEVEL3   = 90,
    HEVC_LEVEL_LEVEL3_1 = 93,
    HEVC_LEVEL_LEVEL4   = 120,
    HEVC_LEVEL_LEVEL4_1 = 123,
    HEVC_LEVEL_LEVEL5   = 150,
    HEVC_LEVEL_LEVEL5_1 = 153,
    HEVC_LEVEL_LEVEL5_2 = 156,
    HEVC_LEVEL_LEVEL6   = 180,
    HEVC_LEVEL_LEVEL6_1 = 183,
    HEVC_LEVEL_LEVEL6_2 = 186
}HEVC_LEVEL_TYPE;

typedef enum {
	INTERLACED_FIELD = 0,
	PROGRESSIVE_FRAME = 1,
} FRAME_MSB_FLAG;

typedef enum {
	P_SLICE = 0,
	B_SLICE = 1,
	I_SLICE = 2,
	IDR_SLICE = 3,
} SLICE_TYPE;

typedef struct {
	UINT32 uiGopLen;
	UINT32 uiRefreshLineNum;
	UINT32 uiBFrmNum;
	UINT32 uiLTRInterval;
	UINT32 uiSvcLayer;
	UINT32 uiLTRPreRef;
} H26XEncGop;

// picture quality structure //
typedef struct _H26XEncAq_ {
	INT32  aq_enable;                   // 0 = aq off, 1 = aq on
	UINT32 aq_str;                      // aq strength: 1~8 (680);
	INT32  sraq_init_aslog2;            // initial i_ASlog2 for sraq (680): -1: disable sraq @1st Frame; 0~63 (default: 36) #0
	INT32  sraq_ic2;                    // sraq coefficient: 0~255 (default: 59) #0
	INT32  max_sraq;                    // max ssim aq: 0~ 15 (default:  8)
	INT32  min_sraq;                    // min ssim aq: 0~-15 (default: -8)
	INT32  sraq_const_aslog2;			// 0 : update aslog2 by each frame, 1: use sraq_init_aslog2 for each frame.
	INT32  save_dqp_en;                 // enable save delta_qp: 0~1 (default: 1) #0
} H26XEncAq;

typedef struct _H26XEncRdo_ {
	INT32   rdopt;
	INT32   rate_est;

	float   rdo_slope[3][4];

	INT32   rdo_cost_bias_I4;
	INT32   rdo_cost_bias_I8;
	INT32   rdo_cost_bias_I16;
	INT32   rdo_cost_bias_P4;
	INT32   rdo_cost_bias_P8;
	INT32   rdo_cost_bias_SKIP;
	INT32   rdo_cost_bias_MERGE;

	INT32   I16_cost_tweak_V;
	INT32   I16_cost_tweak_H;
	INT32   I16_cost_tweak_DC;
	INT32   I16_cost_tweak_PL;

	INT32   ICm_cost_tweak_DC;
	INT32   ICm_cost_tweak_H;
	INT32   ICm_cost_tweak_V;
	INT32   ICm_cost_tweak_PL;

	INT32   luma_coeff_cost;
	INT32   chroma_coeff_cost;
} H26XEncRdo;

typedef struct _H26XEncFro_ {
	INT32 rounding_offset_method;
	INT32 fro_dc[2][3][2];
	INT32 fro_ac[2][3][2];
	INT32 fro_st[2][3][2];
	INT32 fro_mx[2][3][2];
} H26XEncFro;

typedef struct _H26XEncTnr_ {
	INT32  nr_3d_mode;                      // 0 = nr off, 1 = nr on
	INT32  tnr_osd_mode;
	INT32  mctf_p2p_pixel_blending;
	INT32  tnr_p2p_sad_mode;
	INT32  tnr_mctf_sad_mode;
	INT32  tnr_mctf_bias_mode;

	INT32  nr_3d_adp_th_p2p[3];
	INT32  nr_3d_adp_weight_p2p[3];
	INT32  tnr_p2p_border_check_th;
	INT32  tnr_p2p_border_check_sc;
	INT32  tnr_p2p_input;
	INT32  tnr_p2p_mctf_motion_th;
	INT32  tnr_p2p_mctf_motion_wt[4];

	INT32  nr3d_temporal_spatial_y[3];
	INT32  nr3d_temporal_spatial_c[3];
	INT32  nr3d_temporal_range_y[3];
	INT32  nr3d_temporal_range_c[3];
	INT32  nr3d_clampy_th;
	INT32  nr3d_clampy_div;
	INT32  nr3d_clampc_th;
	INT32  nr3d_clampc_div;

	INT32  nr3d_temporal_spatial_y_mctf[3];
	INT32  nr3d_temporal_spatial_c_mctf[3];
	INT32  nr3d_temporal_range_y_mctf[3];
	INT32  nr3d_temporal_range_c_mctf[3];
	INT32  nr3d_clampy_th_mctf;
	INT32  nr3d_clampy_div_mctf;
	INT32  nr3d_clampc_th_mctf;
	INT32  nr3d_clampc_div_mctf;
	
	INT32  nr3d_temporal_spatial_y_mctf_1[3];
	INT32  nr3d_temporal_spatial_c_mctf_1[3];

	INT32  nr3d_temporal_spatial_y_1[3];
	INT32  nr3d_temporal_spatial_c_1[3];

	INT32  nr3d_twr_in_key_frame_en;
} H26XEncTnr;

// user control structure //
typedef struct _H26XEncRoi_ {
	UINT32 uiEnable;
	INT32  iQP;
	UINT32 uiCoord_X;
	UINT32 uiCoord_Y;
	UINT32 uiWidth;
	UINT32 uiHeight;
    UINT32 uiDeltaQp;  // 0 : fixed qp, 1 : delta qp
} H26XEncRoi;

typedef struct _H26XEncSmartRoiInfo_{
	UINT32 uiClass;
	UINT32 uiScore;
	UINT32 uiCoord_X;
	UINT32 uiCoord_Y;
	UINT32 uiWidth;
	UINT32 uiHeight;
} H26XEncSmartRoiInfo;

typedef struct _H26XEncSmartRoi_{
	UINT32 uiRoiNum;
	UINT32 uiRange;
	H26XEncSmartRoiInfo RoiInfo[H26X_MAX_ROI_W];
} H26XEncSmartRoi;

typedef struct _H26XEncSliceSplit_ {
	UINT32 uiEnable;        // enable multiple slice , range : 0 ~ 1 //
	UINT32 uiSliceRowNum;   // number of macroblock rows occupied by a slice , range : 1 ~ SIZE16X(picture_height)
} H26XEncSliceSplit;

typedef struct _H26XEncSrcDecomp_ {
	UINT32 uiVirSrcDecSideInfoAddr;
	UINT32 uiSrcDecktable0;
	UINT32 uiSrcDecktable1;
	UINT32 uiSrcDecktable2;
	UINT32 uiSrcDecStripNum;
	UINT32 uiSrcDecStrip01Size;
	UINT32 uiSrcDecStrip23Size;
} H26XEncSrcDecomp;

typedef struct _H26XEncRowRc_ {
	UINT32 uiEnable;

	UINT32 uiQPRange;
	UINT32 uiQPStep;	
} H26XEncRowRc;

// rate control //
typedef struct _H26XEncRcCBR_ {
	UINT32 uiEnable;

	UINT32 uiStaticTime;	
    UINT32 uiInitIQp;
	UINT32 uiMinIQp;
	UINT32 uiMaxIQp;
    UINT32 uiInitPQp;
	UINT32 uiMinPQp;
	UINT32 uiMaxPQp;
	INT32  iIPQPOffset;

	H26XEncRowRc sRowRc;	

	UINT32 uiBitRate;
	UINT32 uiFrameRate;
	UINT32 uiGOP;

	// unused start
	//UINT32 uiKeyPPeriod;
	//INT32  iKeyPQPOffset;
	// unused done
#if 0
	// shall be remove later //
	UINT32 uiRowRcEnalbe;
	UINT32 uiRowRcQpDelta;
#endif	
    INT32  iIPWeight;
} H26XEncRcCBR;

typedef struct _H26XEncRcVBR_ {
	UINT32 uiEnable;

	UINT32 uiStaticTime;	
	UINT32 uiInitIQp;
	UINT32 uiMinIQp;
	UINT32 uiMaxIQp;
	UINT32 uiInitPQp;
	UINT32 uiMinPQp;
	UINT32 uiMaxPQp;
	INT32  iIPQPOffset;
	UINT32 uiChangePos;

	H26XEncRowRc sRowRc;
	
	UINT32 uiMaxBitRate;
	UINT32 uiBitRate;
	UINT32 uiFrameRate;
	UINT32 uiGOP;

	// unused start
	//UINT32 uiKeyPPeriod;
	//INT32  iKeyPQPOffset;
	// unused done
#if 0
	// shall be remove later //
	UINT32 uiRowRcEnalbe;
	UINT32 uiRowRcQpDelta;
#endif	
    INT32  iIPWeight;
} H26XEncRcVBR;

typedef struct _H26XEncRcEVBR_ {
	UINT32 uiEnable;

	UINT32 uiBitRate;
	UINT32 uiFrameRate;
	UINT32 uiGOP;
    UINT32 uiKeyPPeriod;

	UINT32 uiStaticTime;
	UINT32 uiInitIQp;
	UINT32 uiMinIQp;
	UINT32 uiMaxIQp;
	UINT32 uiInitPQp;
	UINT32 uiMinPQp;
	UINT32 uiMaxPQp;
	INT32  iIPWeight;
    INT32  iKeyPWeight;
    INT32  iMotionAQStrength;

	H26XEncRowRc sRowRc;

    UINT32 uiStillFrameCnd;
    UINT32 uiMotionRatioThd;
	UINT32 uiIPsnrCnd;
	UINT32 uiPPsnrCnd;
	UINT32 uiKeyPPsnrCnd;
    
    UINT32 uiReserved[32];
} H26XEncRcEVBR;

typedef struct _H26XEncRcFixQP_ {
	UINT32 uiEnable;

	UINT32 uiIFixQP;
	UINT32 uiPFixQP;
} H26XEncRcFixQP;

typedef struct _H26XEncMDInfo_{		
	UINT32 uiMbWidthNum;
	UINT32 uiMbHeightNum;
	UINT32 uiMbWidth;
	UINT32 uiMbHeight;
	UINT8* pMDBitMap;
}H26XEncMDInfo;

typedef struct _H26XEncMemAttr_{
	UINT32 uiWidth;
	UINT32 uiHeight;
	UINT32 uiSVCLayer;
	UINT32 uiLTRInterval;
}H26XEncMemAttr;

#endif

