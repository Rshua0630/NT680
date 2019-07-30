#ifndef _IQ_UTILITY_H_
#define _IQ_UTILITY_H_
/**
    IQ Utility


    @file       iq_utility.h
    @ingroup    mISYSAlg
    @note       Nothing (or anything need to be mentioned).

    Copyright   Novatek Microelectronics Corp. 2011.  All rights reserved.
*/
#include "Type.h"
#include "ErrorNo.h"
#include "iq_param_ctrl.h"
#include "ipl_utility.h"
#include "ipl_hal_dce_info.h"
#include "ipl_hal_op.h"

#include "sie1_lib.h"
#include "sensor.h"

#define MIN(a, b)   ((a)<(b)?(a):(b))
#define MAX(a, b)   ((a)>(b)?(a):(b))
#define IQ_CLAMP(in,lb,ub) (((INT32)(in) < (INT32)(lb)) ? (INT32)(lb) : (((INT32)(in) > (INT32)(ub)) ? (INT32)(ub) : (INT32)(in)))

#define IQS_UD_INDEX_NOSET  999999

/** \addtogroup IQ_Utility */
//@{
#define IQS_SIE_ADDR  0xF0C00000
#define IQS_RHE_ADDR  0xF0CE0000
#define IQS_IFE_ADDR  0xF0C70000
#define IQS_DCE_ADDR  0xF0C20000
#define IQS_IPE_ADDR  0xF0C30000
#define IQS_IFE2_ADDR 0xF0D00000
#define IQS_IME_ADDR  0xF0C40000
#define IQS_DRE_ADDR  0xF0CD0000

#define IQS_SIE_REG_SIZE  0x8B4
#define IQS_RHE_REG_SIZE  0x8D0
#define IQS_IFE_REG_SIZE  0x18C
#define IQS_DCE_REG_SIZE  0x2a0
#define IQS_IPE_REG_SIZE  0x1e0c
#define IQS_IFE2_REG_SIZE 0x98
#define IQS_IME_REG_SIZE  0x9fc
#define IQS_DRE_REG_SIZE  0x3300  //0x300 * 10 * 2

#define IQS_ECS_TAB_SIZE  0x4204

typedef struct {
	UINT32 uiCntrPosX;  //in: ratio x/1000
	UINT32 uiCntrPosY;  //in
	UINT32 uiVigX;
	UINT32 uiVigY;
	UINT8  uiVigT;
	UINT32 uiVigXDiv;
	UINT32 uiVigYDiv;
} IQS_VIG_INFO;

typedef enum {
	IQS_CAL_MULTIPLY,
	IQS_CAL_INTPL,
	IQS_CAL_HUE,
	IQS_ADJ_ITEM_MAX_CNT,
} IQS_CAL_WAY;

extern int IQS_Intpl(int Index, int LowValue, int HighValue, int MinIndex, int MaxIndex);
extern void IQS_IntplTbl(UINT32 *aSourceL, UINT32 *aSourceH, int aSize, UINT32 *aTarget, int xIdx, int xIdxL, int xIdxH);
extern void IQS_IntplTblUint8(UINT8 *aSourceL, UINT8 *aSourceH, int aSize, UINT8 *aTarget, int xIdx, int xIdxL, int xIdxH);
extern void IQS_Gamma_Intpl(UINT32 *G1, UINT32 *G2, INT32 X1, INT32 X2, INT32 X, UINT32 *G);
extern void IQS_CC_Intpl(UINT16 *CC1, UINT16 *CC2, INT32 X1, INT32 X2, INT32 X, UINT16 *CC);
extern int IQS_IQ_Cal(int Ratio, int Value, int MinValue, int MaxValue, IQS_CAL_WAY WAY);
extern INT32 IQS_IQ_Intpl(int Ratio, int MinRatio, int MaxRatio, int Value, int MinValue, int MaxValue);
extern void IQS_DCE_Intpl(UINT32 *aSource, UINT32 *aTarget, int xIdx, int xIdxL, int xIdxH);

extern UINT16 *SIE_xfer2CAGamma(UINT32 Id, UINT32 *IPELut);

/**
     re-organize gamma lut

     @note convert Gamma Lut
     @param[in]
     @return
         - @b UINT32.
*/
extern UINT32 *IPE_GammaReorgon(UINT32 Id, UINT32 *RLut, UINT32 *GLut, UINT32 *BLut);
extern UINT32 *IPE_YCurveReorgon(UINT32 Id, UINT32 *YLut);
extern UINT32 *IPE_xfer2EdgeGamma(UINT32 Id, UINT32 *IPELut);
extern UINT32 *iq_get_ecs_tab(UINT32 id, UINT32 *ui_ecs_tab, UINT32 sensor_option);
extern void SIE_GetVigSetting(UINT32 Id, BOOL CapFlag, IQS_VIG_INFO *VigParam);
extern void IFE_GetVigSetting(UINT32 Id, BOOL CapFlag, IQS_VIG_INFO *VigParam);

/**
     The function to calc fov gain
     Return FOV gain

     @param[in] uiImgWidht    Image width
     @param[in] uiImgHeight   Image height

     @return
         - @b UINT32:       fov gain
*/
extern UINT32 DCE_GetFovGain(UINT32 Id, BOOL isCap, UINT32 *DCE_Lut);

extern void iq_get_lce_setting(UINT32 id, IQS_LCE lce_strength ,UINT32* diff_gain, UINT32 *luma_gain);

/**
     The function to dump the IPP register

     @param[in] Msg    Message from capture flow

     @return
         - @b E_OK: SUCCESS
         - @b E_SYS: FAIL
*/
extern BOOL IQS_DumpEngRegister(IPL_FLOW_PROC_MSG Msg);
extern BOOL IQS_DumpPrvRegister(void);
extern void IQS_SetEngDumpBuffer(IPL_HAL_ENG Eng, UINT32 Addr, UINT32 Size);
extern void IQS_GetEngDumpBuffer(IPL_HAL_ENG Eng, UINT32 *Addr, UINT32 *Size);

typedef void (*IQS_MOVIELOG_FP)(UINT32 Id, char *Buf, UINT32 BufLen);
extern void IQS_SetMovieLogFP(IQS_MOVIELOG_FP fp);
extern void IQS_GetLogString(UINT32 Id, char *Buf, UINT32 BufLen);
extern void IQS_ToneCurveAdj(UINT32 *gammaIn, UINT32 *gammaOut, UINT16 mid, UINT16 low, UINT16 high, UINT16 level);

#endif //_IQ_UTILITY_H_
