#ifndef _IQ_EFFECT_TABLE_H_
#define _IQ_EFFECT_TABLE_H_

typedef struct {
	UINT8 CbOfs;        ///< Legal range : 0~255, Cb color offset, 128 : NO color offset
	UINT8 CrOfs;        ///< Legal range : 0~255, Cr color offset, 128 : NO color offset
	UINT8 YCon;         ///< Legal range : 0~255, Y contrast adjust, 128 : NO contrst adjust, Y' = (Y * Ycon) >> 7
	UINT8 CCon;         ///< Legal range : 0~255, CbCr contrast adjust, 128 : NO contrst adjust, C' = (C * CCon) >> 7
	INT16 IntOfs;       ///< Legal range : -128~127, Y offset, Y' = Y + Intofs
	INT16 SatOfs;       ///< Legal range : -128~127, color saturation adjust, 0 : NO color saturation adjust
	BOOL  HueRotateEn;  ///< Hue adjust rotation option, 0 : NO rotation, 1 : CHUEM[n] LSB 2 bit = 0 : 0 degree rotation, 1 : 90 degree rotation, 2 bit = 2 : 180 degree rotation, 3 : 270 degree rotation
} IQS_COLOR_EFFECT_PARAM;

extern IQS_COLOR_EFFECT_PARAM ColorEffectTable_Normal;
extern IQS_COLOR_EFFECT_PARAM ColorEffectTable_BW;
extern IQS_COLOR_EFFECT_PARAM ColorEffectTable_SEPIA;
extern IQS_COLOR_EFFECT_PARAM ColorEffectTable_VIVID;
extern IQS_COLOR_EFFECT_PARAM ColorEffectTable_PencilSketch;
extern IQS_COLOR_EFFECT_PARAM ColorEffectTable_ColorPencil;
extern IQS_COLOR_EFFECT_PARAM ColorEffectTable_CoolGreen;
extern IQS_COLOR_EFFECT_PARAM ColorEffectTable_WarmYellow;

extern IPE_YCCFIX IE_ColorPencil_Param;
extern IPE_YCCFIX IE_PencilSketch_Param;
extern UINT32 IQS_DCE_GLUT_Init[65];
extern UINT32 IE_FISYEYE_LUT[65];
extern IQS_VIG_SET g_vig_init;
extern IQS_COLOR_EFFECT_PARAM ColorEffectTable;

extern INT32 IPE_SelectiveCC_SatTab[15][24];
extern IPE_YCCFIX IPE_RockEffect_YccFix;
extern UINT32 IE_Gamma_Negative[129];

extern IPE_EethParam g_ipe_eth;

#define IQS_MAX_DGMASK_NUM 1
#define IQS_MAX_MSNR_NR_NUM 20
extern DRAM_DG_INFO IQS_DRE_DGMask[DRE_DG_PROC_MAX_NUM];
extern IQS_DRE_NR_IQPARAM *IQS_MSNR_NR[IQS_MAX_MSNR_NR_NUM];
extern IQS_DRE_IQPARAM IQS_MSNR;
#endif

