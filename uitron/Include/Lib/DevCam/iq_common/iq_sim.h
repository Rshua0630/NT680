#ifndef _IQ_SIM_H_
#define _IQ_SIM_H_
/**
    IQSim.h

    @file       iq_sim.h
    @ingroup    Lib\DevCam\iqs_common
    @note       Nothing (or anything need to be mentioned).

    Copyright   Novatek Microelectronics Corp. 2017.  All rights reserved.
*/
#include "Type.h"
#include "ErrorNo.h"

#include "iq_param_ctrl.h"

typedef struct {
	ISIZE Size;
	UINT32 BitNum;
	IPL_RAW_PIX cfa;
	IFE_BAYERFMT_SEL format;
} IQSIM_RAW_INFO;


typedef struct {
	ISIZE Size;
	UINT32 BitRate;
} IQSIM_JPG_INFO;

typedef struct {
	IQSIM_RAW_INFO  RawInfo;

	//FuncEn
	RheFuncEn       RHE_FuncEn;
	SieFuncEn       SIE_FuncEn;
	IfeFuncEn       IFE_FuncEn;
	DceFuncEn       DCE_FuncEn;
	IPEFUNCEN       IPE_FuncEn;
	IFE2_FUNC_En    IFE2_FuncEn;
	ImeFuncEn       IME_FuncEn;

#if 0
	//SIE
	SIE_OBSUB       IQS_OBSub;

	UINT32          IQS_ECSTab[4225];
	ECSMAP_ENUM     IQS_ECSMap;
	SIE_EmbeddedCS  IQS_ECSSet;
#endif

	//RHE
	RHE_HAL_COLOR_GAIN_INFO IQS_RHE_CGain;

	UINT32                  IQS_RHE_Lpf_Coef[12];
	RHE_HAL_FILTER_INFO     IQS_RHE_Lpf;

	UINT32                  IQS_RHE_Blend_Gamma[129];
	UINT32                  IQS_RHE_Blend_Cuv[65];
	RHE_HAL_BLEND_PARAM     IQS_RHE_Blend;

	UINT32                  IQS_RHE_Local_DGain[17];
	UINT32                  IQS_RHE_Local_LGain[17];
	RHE_HAL_LTC_PARAM       IQS_RHE_Local;

	UINT32                  IQS_RHE_ToneCuv[129];
	RHE_HAL_TONE_CRUVE_INFO IQS_RHE_Tone;

	UINT32                  IQS_RHE_SatTab[17];
	RHE_HAL_CSAT_INFO       IQS_RHE_Sat;

	UINT32                  IQS_RHE_CNSTab[17];
	RHE_HAL_CNS_INFO        IQS_RHE_CNS;

	UINT32                  IQS_RHE_ConAdj_Blend_Rto[9];
	UINT32                  IQS_RHE_ConAdj_Out_Weight[17];
	UINT32                  IQS_RHE_ConAdj_ModA_Lut[9];
	UINT32                  IQS_RHE_ConAdj_ModB_Lut[9];
	UINT32                  IQS_RHE_ConAdj_Gamma[33];
	RHE_HAL_CADJ_INFO       IQS_RHE_ContrastAdj;

	//IFE
	UINT32          IQS_IFE_CGain_Gain[4];
	UINT32          IQS_IFE_CGain_Offset[4];
	IFE_CG          IQS_IFE_CGain;

	UINT32          IQS_IFE_Outl_Bri[5];
	UINT32          IQS_IFE_Outl_Dark[5];
	UINT32          IQS_IFE_Outl_Cnt[2];
	IFE_OUTL        IQS_IFE_Outl;

	UINT32          IQS_IFE_SF_Weight[10];
	Spatial_Filter  IQS_IFE_SpatialFilter;
	UINT32          IQS_IFE_RangeA_Thr[6];
	UINT32          IQS_IFE_RangeA_Lut[17];
	RangeCFGA       IQS_IFE_RangeA;
	UINT32          IQS_IFE_RangeB_Thr[6];
	UINT32          IQS_IFE_RangeB_Lut[17];
	RangeCFGB       IQS_IFE_RangeB;
	Range_Filter    IQS_IFE_RangeFilter;
	UINT8           IQS_IFE_NlmKer_Radius[8];
	UINT8           IQS_IFE_NlmLut_Mwth[2];
	UINT8           IQS_IFE_NlmLut_B_Wa[8];
	UINT8           IQS_IFE_NlmLut_B_Wb[4];
	UINT8           IQS_IFE_NlmLut_B_Wc[7];
	UINT8           IQS_IFE_NlmLut_B_Wbl[8];
	UINT8           IQS_IFE_NlmLut_B_Wbm[8];
	UINT8           IQS_IFE_NlmLut_B_Wbh[8];
	IFE_FILT        IQS_IFE_Filter;

	UINT8           IQS_IFE_Gbal_Stab[17];
	UINT8           IQS_IFE_Gbal_Dtab[17];
	UINT32          IQS_IFE_Gbal_IrSub[4];
	IFE_GBAL        IQS_IFE_Gbal;

	UINT32          IQS_IFE_Vig_CH0[17];
	UINT32          IQS_IFE_Vig_CH1[17];
	UINT32          IQS_IFE_Vig_CH2[17];
	UINT32          IQS_IFE_Vig_CH3[17];
	IFE_VIG         IQS_IFE_Vig;

	//DCE
	DIST            IQS_DCE_Dist;

	Fact_Norm       IQS_DCE_Radious;

	UINT32          IQS_DCE_GLUT[65];
	INT32           IQS_DCE_RLUT[65];
	INT32           IQS_DCE_BLUT[65];
	DCE_Distortion  IQS_DCE_Distortion;

	DCE_CFAPARAM    IQS_DCE_CFAInter;

	//IPE
	INT16               IQS_IPE_EdgeKernel[12];
	UINT8               IQS_IPE_EDirTab[8];
	UINT32              IQS_IPE_EdgeGammaLut[65];
	IPE_DEE             IQS_IPE_DirEdgeExt;
	IPE_DirScorCal      IQS_IPE_DirScoreTab;
	IPE_DirScorCalTh    IQS_IPE_DirScoreTh;
	IPE_DEConParam      IQS_IPE_DECon;
	IPE_KAEdgeW         IQS_IPE_EWA;
	IPE_KBEdgeW         IQS_IPE_EWB;
	IPE_EdgeW           IQS_IPE_EWParam;
	IPE_EDGEEXT         IQS_IPE_EdgeExt;

	IPE_EDGEENH         IQS_IPE_EdgeEnh;

	UINT8               IQS_IPE_EDTab[16];
	UINT8               IQS_IPE_EStab[16];
	IPE_EDGEMAP         IQS_IPE_EdgeMap;

	IPE_RGBLPF          IQS_IPE_RGBLpf;

	UINT16              IQS_IPE_ColorMatrix[9];
	UINT8               IQS_IPE_FStab[16];
	UINT8               IQS_IPE_FDtab[16];
	IPE_CC              IQS_IPE_ColorCorrection;

	UINT8               IQS_IPE_hueTab[24];
	INT32               IQS_IPE_satTab[24];
	INT32               IQS_IPE_intTab[24];
	UINT8               IQS_IPE_edgTab[24];
	UINT8               IQS_IPE_ddsTab[8];
	IPE_CCTRL           IQS_IPE_Cctrl;

	IPE_CCOFS           IQS_IPE_CCOfs;

	IPE_CCON            IQS_IPE_YCCcon;

	IPE_RAND_NR         IQS_IPE_NoiseParam;

	INT16               IQS_IPE_CSTCoef[9];
	IPE_CST             IQS_IPE_CSTParam;

	IPE_YCCFIX          IQS_IPE_YCCFix;

	UINT32              IQS_IPE_GammaLut[130];

	UINT32              IQS_IPE_YCurveLut[130];

	IPE_GamRand         IQS_IPE_GammaRand;

	UINT32              IQS_IPE_3DCCLut[900];
	IPE_3DCCRound       IQS_IPE_CC3dRnd;
	IPE_3DCC            IQS_IPE_3DColorCtrl;

	//IFE2
	IFE2_GRAY_Statal    IQS_IFE2_GrayStatical;

	UINT32              IQS_IFE2_RefCenter_YTh[3];
	UINT32              IQS_IFE2_RefCenter_YWt[4];
	UINT32              IQS_IFE2_RefCenter_UVTh[3];
	UINT32              IQS_IFE2_RefCenter_UVWt[4];
	IFE2_Refcent_Param  IQS_IFE2_RefCenter;

	UINT32              IQS_IFE2_Filter_YTh[5];
	UINT32              IQS_IFE2_Filter_YWt[6];
	UINT32              IQS_IFE2_Filter_UTh[5];
	UINT32              IQS_IFE2_Filter_UWt[6];
	UINT32              IQS_IFE2_Filter_VTh[5];
	UINT32              IQS_IFE2_Filter_VWt[6];
	IQS_IFE2_FILTER     IQS_IFE2_Filter;

	//IME
	IME_CHRA_ADAP   IQS_IME_ChromaAdapt;
	IME_CHRA_LADAP  IQS_IME_ChromaAdapt_Lum;
	IME_CHRA_CADAP  IQS_IME_ChromaAdapt_Color;

	UINT32          IQS_IME_CS_WeightY[16];
	UINT32          IQS_IME_CS_WeightC[16];
	IME_ChromaSupp  IQS_IME_ChromaSupp;

	IME_SuperRes    IQS_IME_SuperRes;

	//DRE
	DRAM_DG_INFO        IQS_DRE_DGMask[DRE_DG_PROC_MAX_NUM];
	IQS_DRE_NR_IQPARAM  IQS_MSNR_NR[DRE_NR_PROC_MAX_NUM];
	IQS_DRE_IQPARAM     IQS_MSNR;
} IQSIM_CHINFO;


typedef struct {
	char version[32];
	IQSIM_JPG_INFO IQSimJpgInfo;
	IQSIM_CHINFO IQSimPriInfo;
	//IQSIM_CHINFO IQSimQVInfo;
} IQSIM_APPINFO;

extern IQSIM_CHINFO *IQSIM_GetChInfo(IQS_FLOW_MSG MsgID);
extern IQSIM_APPINFO *IQSIM_GetAppInfo(void);
extern void IQSIM_SetAppInfo(UINT32 BinAddr);
extern void IQSIM_ReorgAppInfo(void);
extern void IQSIM_SetRawAddr(UINT32 ch0, UINT32 ch2);
extern void IQSIM_GetRawAddr(UINT32 *ch0, UINT32 *ch2);

#endif
