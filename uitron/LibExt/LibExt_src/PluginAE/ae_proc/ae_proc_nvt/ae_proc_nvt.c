/**
    Auto Exposure flow.

    ae flow.

    @file       ae_sample.c
    @ingroup    mISYSAlg
    @note       Nothing (or anything need to be mentioned).

    Copyright   Novatek Microelectronics Corp. 2011.  All rights reserved.
*/
#include <stdio.h>

#include "Type.h"
#include "ae_alg.h"
#include "ae_proc_nvt_int.h"
#include "ae_task.h"
#include "iq_ui_info.h"
#include "ae_ui_info.h"
#include "awb_ui_info.h"
#include "ipl_cmd.h"
#include "iq_api.h"
#include "aeafd_alg.h"
#include "grph.h"
#include "ipl_ctrl.h"
#include "Lens.h"
#include "iq_utility.h"
#include "SensorHDRLib.h"
#include "fd_lib.h"
#include "Delay.h"
#include "note.h"
#include "ipl_ctrl_infor.h"
#include "pq_obj.h"
#include "ae_cb_msg.h"
#include "pwm.h"

#define CAL_ISO DISABLE
#if CAL_ISO
#include "ipl_cal_datapassing_(IPL)_EVB_FF_int.h"
#endif

#define __MODULE__ AE
//#define __DBGLVL__ 0        //OFF mode, show nothing
//#define __DBGLVL__ 1        //ERROR mode, show err, wrn only
#define __DBGLVL__ 2        //TRACE mode, show err, wrn, ind, msg and func and ind, msg and func can be filtering by __DBGFLT__ settings
#define __DBGFLT__ "*"      //*=All
#include "DebugModule.h"

#include "NvtVerInfo.h"

NVTVER_VERSION_ENTRY(AE_PROC_NVT, 1, 00, 000, 00)

#define AE_STAMPLOG  1

#define ae_clamp(in,lb,ub) (((in) < (lb)) ? (lb) : (((in) > (ub)) ? (ub) : (in)))

static AE_PARAM_INFO *ae_param_info[AE_ID_MAX_NUM];
static AE_STATUS_INFO ae_status_info[AE_ID_MAX_NUM];
static AE_PARAMINFO AeParamInfo[AE_ID_MAX_NUM];
static void AE_SetCalGain(IPL_PROC_ID Id);

AE_SHDR_ExptTrans ae_shdr_trans[AE_ID_MAX_NUM];

UINT32 AEUartMsg = DISABLE;
UINT32 AE_HDR_Msg = DISABLE;

//static UINT8(*AeWin)[AE_WIN_X] = (UINT8(*)[AE_WIN_X])AE_Meter_Winow;
static UINT32 AeWin[AE_WIN_Y][AE_WIN_X];
static UINT32 AeFlag[AE_ID_MAX_NUM] = {0};
static AE_INFO AeInfo[AE_ID_MAX_NUM] = {0};
static AE_INFO AeCapInfo[AE_ID_MAX_NUM] = {0};
static AE_EVENT_TAB AEEventTAB = {0};
static UINT16 AeHistBuf[AE_ID_MAX_NUM][AE_HIST_NUM];
static UINT16 AeHist2BufY[AE_ID_MAX_NUM][AE_HIST_NUM];
static AEALG_ARG PrvNewAeArg[AE_ID_MAX_NUM][3], CapNewAeArg[AE_ID_MAX_NUM];
static AEALG_ARG PrvLastAeArg[AE_ID_MAX_NUM][3];
static UINT32 AECnt[AE_ID_MAX_NUM] = {0};
static UINT32 AECntFlag[AE_ID_MAX_NUM] = {TRUE};
static UINT32 AEStep[AE_ID_MAX_NUM] = {0};
static GAIN_SETTING gGain[AE_ID_MAX_NUM][AE_TOTAL_PARAMNUM];  //Keep 6 frames setting.
static EXPOSURE_SETTING gExpTSetting[AE_ID_MAX_NUM][AE_TOTAL_PARAMNUM];  //Keep 6 frames setting.

static AE_BLK_INFO AeBlkInfo[AE_ID_MAX_NUM] = {0};
static AE_SYNC_INFO AeSyncInfo[AE_ID_MAX_NUM][AE_SYNC_TAB_SIZE] = {0};
static UINT32 AeSyncCnt[AE_ID_MAX_NUM] = {0};

//Auto Curve Gen Parameters
static AE_GEN_INFO AeCurveGenInfo[AE_ID_MAX_NUM] = {0};
static AE_GEN_PHOTO AeCurveGenPhoto[AE_ID_MAX_NUM] = {0};
static UINT32 AeAutoCurveTab_Prv[AE_ID_MAX_NUM][AE_GEN_CURVE_NODE_MAX][AEALG_INFO_MAX];
static UINT32 AeAutoCurveTab_Cap[AE_ID_MAX_NUM][AE_GEN_CURVE_NODE_MAX][AEALG_INFO_MAX];
static BOOL AeCurveUpdate_Prv[AE_ID_MAX_NUM] = {0};
static BOOL AeCurveUpdate_Cap[AE_ID_MAX_NUM] = {0};

static UINT32 AeIspGain[AE_ID_MAX_NUM] = {128, 128, 128, 128, 128, 128, 128, 128};

static UINT32 Ratio[SEN_MFRAME_MAX_NUM] = {0};
static UINT32 ISOGain[SEN_MFRAME_MAX_NUM] = {0}, ExpoTime[SEN_MFRAME_MAX_NUM] = {0};

static AE_IRIS_INFO AeIrisInfo[AE_ID_MAX_NUM] = {0};

static BOOL IR_CUT_ENABLE[AE_ID_MAX_NUM] = {FALSE};
static AE_INT_CTRL_INFO AeIntCtrlInfo[AE_ID_MAX_NUM] = {{AE_NOR_CAL_SET_SHIFT, AE_NOR_PREDICT_POINT, AE_NOR_PEROID},
	{AE_NOR_CAL_SET_SHIFT, AE_NOR_PREDICT_POINT, AE_NOR_PEROID},
	{AE_NOR_CAL_SET_SHIFT, AE_NOR_PREDICT_POINT, AE_NOR_PEROID},
	{AE_NOR_CAL_SET_SHIFT, AE_NOR_PREDICT_POINT, AE_NOR_PEROID},
	{AE_NOR_CAL_SET_SHIFT, AE_NOR_PREDICT_POINT, AE_NOR_PEROID},
	{AE_NOR_CAL_SET_SHIFT, AE_NOR_PREDICT_POINT, AE_NOR_PEROID},
	{AE_NOR_CAL_SET_SHIFT, AE_NOR_PREDICT_POINT, AE_NOR_PEROID},
	{AE_NOR_CAL_SET_SHIFT, AE_NOR_PREDICT_POINT, AE_NOR_PEROID}
};
extern UINT32 uiISOGain[AE_ID_MAX_NUM], uiExpoTime[AE_ID_MAX_NUM], uiIris[AE_ID_MAX_NUM];
extern UINT32 uiAEUartCmd[AE_ID_MAX_NUM], uiAEOSDCmd[AE_ID_MAX_NUM];
extern UINT32 uiAEManualCmd[AE_ID_MAX_NUM], uiAEManualISOCmd[AE_ID_MAX_NUM];
static AE_HISTOGRAM weighting_histogram;
#if 0
static UINT32 GAcc[AE_WIN_X * AE_WIN_Y] = {0};
#endif
/* -4, -3.7, -3.5, -3.3,  -3, -2.7, -2.5, -2.3, -2, -1.7, -1.5, -1.3, -1, -0.7, -0.5, -0.3,   0, 0.3, 0.5, 0.7,   1, 1.3, 1.5, 1.7, 2.0, 2.3, 2.5, 2.7,   3, 3.3, 3.5, 3.7,   4*/
static UINT32 EVTable[AE_EV_MAX_CNT] =
{   6,    8,    9,   10,  13,   15,   18,   20, 25,   31,   35,   41, 50,   62,   71,   81, 100, 123, 141, 162, 200, 246, 283, 325, 400, 492, 566, 650, 800, 985,1131,1300,1600};

static UINT32 Ae_Sqrt_Tbl[65] = {
  0,100,141,173,200,223,244,264,282,300,
316,331,346,360,374,387,400,412,424,435,
447,458,469,479,489,500,509,519,529,538,
547,556,565,574,583,591,600,608,616,624,
632,640,648,655,663,670,678,685,692,700,
707,714,721,728,734,741,748,754,761,768,
774,781,787,793,800
};

static AEALG_EVENT_TAB AeAlgEventTab = {
	GetOverExpoRatio,
	GetFnoRatio,
	NULL
};

static UINT32 AE_HDR_Switch[AE_ID_MAX_NUM] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
static AE_FLICKER Flicker[AE_ID_MAX_NUM] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
static AE_ISO Iso[AE_ID_MAX_NUM] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
static UINT32 Fps[AE_ID_MAX_NUM] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
static UINT32 AE_Fno[AE_ID_MAX_NUM] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
static UINT32 AE_EVCompIdx[AE_ID_MAX_NUM] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};

static UINT32 AE_ClampRatio[AE_ID_MAX_NUM][AEALG_DYNAMIC_LV_NUM];

static UINT32 AE_Win_Face[AE_WIN_Y][AE_WIN_X] = {
	{0, 0, 0, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 0, 0, 0}
};

static const UINT32 AE_Win_CenterWieght[AE_WIN_Y][AE_WIN_X] = {
	{1, 1, 1, 1, 1, 1, 1, 1},
	{1, 2, 2, 2, 2, 2, 2, 1},
	{1, 2, 3, 3, 3, 3, 2, 1},
	{1, 2, 3, 4, 4, 3, 2, 1},
	{1, 2, 3, 4, 4, 3, 2, 1},
	{1, 2, 3, 3, 3, 3, 2, 1},
	{1, 2, 2, 2, 2, 2, 2, 1},
	{1, 1, 1, 1, 1, 1, 1, 1}
};

static const UINT32 AE_Win_Spot[AE_WIN_Y][AE_WIN_X] = {
	{0, 0, 0, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 0, 0, 0},
	{0, 0, 0, 1, 1, 0, 0, 0},
	{0, 0, 0, 1, 1, 0, 0, 0},
	{0, 0, 0, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 0, 0, 0},
};

static const UINT32 AE_OverExpo_Win_Normal[AE_WIN_Y][AE_WIN_X] = {
	{1, 1, 1, 1, 1, 1, 1, 1},
	{1, 1, 1, 1, 1, 1, 1, 1},
	{1, 1, 1, 1, 1, 1, 1, 1},
	{1, 1, 1, 1, 1, 1, 1, 1},
	{1, 1, 1, 3, 3, 1, 1, 1},
	{1, 1, 3, 6, 6, 3, 1, 1},
	{1, 1, 3, 6, 6, 3, 1, 1},
	{1, 1, 1, 1, 1, 1, 1, 1}
};
UINT32 AEAFD_START[AE_ID_MAX_NUM] = {FALSE};
UINT32 AEAFD_CurrentFlickerType = AE_FLICKER_60HZ;
UINT16 AfdBuffer[LA_WIN_X * LA_WIN_Y * AEAFD_DETECT_CYCLE]; //32*32*6
UINT16 Blk_Gacc[AE_ID_MAX_NUM][LA_WIN_X * LA_WIN_Y], Blk_Yacc[AE_ID_MAX_NUM][LA_WIN_X * LA_WIN_Y];
UINT16 Blk_LA_Data[AE_ID_MAX_NUM][AE_WIN_NUM];
UINT32 AEB_Ratio[AE_ID_MAX_NUM] = {100};

UINT32 AE_firstStable[AE_ID_MAX_NUM] = {FALSE};
UINT32 AE_stableLum[AE_ID_MAX_NUM] = {1015, 1000, 1000, 1000, 1000, 1000, 1000, 1000};
UINT32 AE_stableEV[AE_ID_MAX_NUM] = {540, 1000, 1000, 1000, 1000, 1000, 1000, 1000};

static UINT16 AE_LumGamma[AE_ID_MAX_NUM][4096] = {0};

static AE_SHDR_INFO ae_shdr_info[AE_ID_MAX_NUM];

#define AE_GET_GAMMA_VAL(Id) (Id == IPL_ID_1 ? AE_GetGammaValue_ID_1 : ((Id == IPL_ID_2) ? AE_GetGammaValue_ID_2 : ((Id == IPL_ID_3) ? AE_GetGammaValue_ID_3 : ((Id == IPL_ID_4) ? AE_GetGammaValue_ID_4 : ((Id == IPL_ID_5) ? AE_GetGammaValue_ID_5 : ((Id == IPL_ID_6) ? AE_GetGammaValue_ID_6 : ((Id == IPL_ID_7) ? AE_GetGammaValue_ID_7 : AE_GetGammaValue_ID_8)))))))
#define AE_GET_INV_GAMMA_VAL(Id) (Id == IPL_ID_1 ? AE_GetInvGammaValue_ID_1 : ((Id == IPL_ID_2) ? AE_GetInvGammaValue_ID_2 : ((Id == IPL_ID_3) ? AE_GetInvGammaValue_ID_3 : ((Id == IPL_ID_4) ? AE_GetInvGammaValue_ID_4 : ((Id == IPL_ID_5) ? AE_GetInvGammaValue_ID_5 : ((Id == IPL_ID_6) ? AE_GetInvGammaValue_ID_6 : ((Id == IPL_ID_7) ? AE_GetInvGammaValue_ID_7 : AE_GetInvGammaValue_ID_8)))))))

void AE_LumGamma_Init(UINT32 Id);

void AE_LumGamma_Init(UINT32 Id)
{
    UINT16 data, idx, LumIdx, LumIdx1, YLum;

    for(data=0; data<4096; data++)
    {
		if (data < 1) {
			idx = 0;
			LumIdx = 0;
		} else {
			idx = (data * 128) / 4096;
			LumIdx = (idx * 4096) / 128;
		}

		LumIdx1 = ((idx + 1) * 4096) / 128;
		YLum = (((ae_param_info[Id]->lum_gamma->gamma[(idx + 1)] - ae_param_info[Id]->lum_gamma->gamma[idx]) * (data - LumIdx))+((LumIdx1 - LumIdx)>>1)) / (LumIdx1 - LumIdx);
		YLum += ae_param_info[Id]->lum_gamma->gamma[idx];
        AE_LumGamma[Id][data] = YLum;
    }
}

UINT32 AE_Get_LumGammaValue(UINT32 Id, UINT32 data)
{
    if(data > 4095)
        data = 4095;

    return (UINT32)AE_LumGamma[Id][data];
}

UINT32 AE_Get_InvLumGammaValue(UINT32 Id, UINT32 Data10Bit)
{
    UINT32 i;
    UINT32 ChkFlag = FALSE;
    UINT32 RawLum;
	UINT32 Idx1 = 0, Idx = 0;

    if(Data10Bit > 1023)
        Data10Bit = 1023;

	if (Data10Bit < 1) {
		RawLum = 0;
		return RawLum;
	}

	for (i = 1; i < 129; i ++) {
		if (Data10Bit <= ae_param_info[Id]->lum_gamma->gamma[i]) {
			Idx1 = i;
			Idx = (Idx1 - 1);
			ChkFlag = TRUE;
			break;
		}
        if (i == 128) {
			RawLum = 4095;
		    return RawLum;
        }
	}

	if ((ae_param_info[Id]->lum_gamma->gamma[Idx1] - ae_param_info[Id]->lum_gamma->gamma[Idx]) == 0) {
		RawLum = (Idx << 10);
		RawLum = ((RawLum << 12) >> 7) >> 10;

		if (RawLum > 4096) {
			DBG_ERR("%d %d %d %d %d\r\n", Idx, Idx1, Data10Bit, ae_param_info[Id]->lum_gamma->gamma[Idx], ae_param_info[Id]->lum_gamma->gamma[Idx1]);
		} else if (RawLum == 4096) {
			RawLum = 4095;
		}

		return RawLum;
	}

	if (ChkFlag == TRUE) {
		RawLum = ((UINT64)((UINT64)(Idx1 - Idx) * (UINT64)(Data10Bit - ae_param_info[Id]->lum_gamma->gamma[Idx])) << 10) / (ae_param_info[Id]->lum_gamma->gamma[Idx1] - ae_param_info[Id]->lum_gamma->gamma[Idx]);
		RawLum += (Idx << 10);
		RawLum = ((RawLum << 12) >> 7) >> 10;

		if (RawLum > 4096) {
			DBG_ERR("%d %d %d %d %d\r\n", Idx, Idx1, Data10Bit, ae_param_info[Id]->lum_gamma->gamma[Idx], ae_param_info[Id]->lum_gamma->gamma[Idx1]);
		} else if (RawLum == 4096) {
			RawLum = 4095;
		}

		return RawLum;
	} else {
		DBG_ERR("input overflow %d\r\n", Data10Bit);
		return 0;
	}
}

UINT32 AE_GetGammaValue_ID_1(UINT32 Data12Bit)
{
    if(ae_param_info[IPL_ID_1]->lum_gamma->type==AE_LUMGAMMA_IPP)
        return iq_get_gamma_value(IPL_ID_1, Data12Bit);
    else
        return AE_Get_LumGammaValue(IPL_ID_1, Data12Bit);
}

UINT32 AE_GetGammaValue_ID_2(UINT32 Data12Bit)
{
    if(ae_param_info[IPL_ID_2]->lum_gamma->type==AE_LUMGAMMA_IPP)
        return iq_get_gamma_value(IPL_ID_2, Data12Bit);
    else
        return AE_Get_LumGammaValue(IPL_ID_2, Data12Bit);
}

UINT32 AE_GetGammaValue_ID_3(UINT32 Data12Bit)
{
    if(ae_param_info[IPL_ID_3]->lum_gamma->type==AE_LUMGAMMA_IPP)
        return iq_get_gamma_value(IPL_ID_3, Data12Bit);
    else
        return AE_Get_LumGammaValue(IPL_ID_3, Data12Bit);
}

UINT32 AE_GetGammaValue_ID_4(UINT32 Data12Bit)
{
    if(ae_param_info[IPL_ID_4]->lum_gamma->type==AE_LUMGAMMA_IPP)
        return iq_get_gamma_value(IPL_ID_4, Data12Bit);
    else
        return AE_Get_LumGammaValue(IPL_ID_4, Data12Bit);
}

UINT32 AE_GetGammaValue_ID_5(UINT32 Data12Bit)
{
    if(ae_param_info[IPL_ID_5]->lum_gamma->type==AE_LUMGAMMA_IPP)
        return iq_get_gamma_value(IPL_ID_5, Data12Bit);
    else
        return AE_Get_LumGammaValue(IPL_ID_5, Data12Bit);
}

UINT32 AE_GetGammaValue_ID_6(UINT32 Data12Bit)
{
    if(ae_param_info[IPL_ID_6]->lum_gamma->type==AE_LUMGAMMA_IPP)
        return iq_get_gamma_value(IPL_ID_6, Data12Bit);
    else
        return AE_Get_LumGammaValue(IPL_ID_6, Data12Bit);
}

UINT32 AE_GetGammaValue_ID_7(UINT32 Data12Bit)
{
    if(ae_param_info[IPL_ID_7]->lum_gamma->type==AE_LUMGAMMA_IPP)
        return iq_get_gamma_value(IPL_ID_7, Data12Bit);
    else
        return AE_Get_LumGammaValue(IPL_ID_7, Data12Bit);
}

UINT32 AE_GetGammaValue_ID_8(UINT32 Data12Bit)
{
    if(ae_param_info[IPL_ID_8]->lum_gamma->type==AE_LUMGAMMA_IPP)
        return iq_get_gamma_value(IPL_ID_8, Data12Bit);
    else
        return AE_Get_LumGammaValue(IPL_ID_8, Data12Bit);
}

UINT32 AE_GetInvGammaValue_ID_1(UINT32 Data10Bit)
{
    if(ae_param_info[IPL_ID_1]->lum_gamma->type==AE_LUMGAMMA_IPP)
    	return iq_get_inv_gamma_value(IPL_ID_1, Data10Bit);
    else
        return AE_Get_InvLumGammaValue(IPL_ID_1, Data10Bit);
}

UINT32 AE_GetInvGammaValue_ID_2(UINT32 Data10Bit)
{
    if(ae_param_info[IPL_ID_2]->lum_gamma->type==AE_LUMGAMMA_IPP)
    	return iq_get_inv_gamma_value(IPL_ID_2, Data10Bit);
    else
        return AE_Get_InvLumGammaValue(IPL_ID_2, Data10Bit);
}

UINT32 AE_GetInvGammaValue_ID_3(UINT32 Data10Bit)
{
    if(ae_param_info[IPL_ID_3]->lum_gamma->type==AE_LUMGAMMA_IPP)
    	return iq_get_inv_gamma_value(IPL_ID_3, Data10Bit);
    else
        return AE_Get_InvLumGammaValue(IPL_ID_3, Data10Bit);
}

UINT32 AE_GetInvGammaValue_ID_4(UINT32 Data10Bit)
{
    if(ae_param_info[IPL_ID_4]->lum_gamma->type==AE_LUMGAMMA_IPP)
    	return iq_get_inv_gamma_value(IPL_ID_4, Data10Bit);
    else
        return AE_Get_InvLumGammaValue(IPL_ID_4, Data10Bit);
}
UINT32 AE_GetInvGammaValue_ID_5(UINT32 Data10Bit)
{
    if(ae_param_info[IPL_ID_5]->lum_gamma->type==AE_LUMGAMMA_IPP)
    	return iq_get_inv_gamma_value(IPL_ID_5, Data10Bit);
    else
        return AE_Get_InvLumGammaValue(IPL_ID_5, Data10Bit);
}

UINT32 AE_GetInvGammaValue_ID_6(UINT32 Data10Bit)
{
    if(ae_param_info[IPL_ID_6]->lum_gamma->type==AE_LUMGAMMA_IPP)
    	return iq_get_inv_gamma_value(IPL_ID_6, Data10Bit);
    else
        return AE_Get_InvLumGammaValue(IPL_ID_6, Data10Bit);
}
UINT32 AE_GetInvGammaValue_ID_7(UINT32 Data10Bit)
{
    if(ae_param_info[IPL_ID_7]->lum_gamma->type==AE_LUMGAMMA_IPP)
    	return iq_get_inv_gamma_value(IPL_ID_7, Data10Bit);
    else
        return AE_Get_InvLumGammaValue(IPL_ID_7, Data10Bit);
}

UINT32 AE_GetInvGammaValue_ID_8(UINT32 Data10Bit)
{
    if(ae_param_info[IPL_ID_8]->lum_gamma->type==AE_LUMGAMMA_IPP)
    	return iq_get_inv_gamma_value(IPL_ID_8, Data10Bit);
    else
        return AE_Get_InvLumGammaValue(IPL_ID_8, Data10Bit);
}
static void AE_SetIntCtrlInfo(UINT32 Id, AE_INT_CTRL_INFO CtrlInfo)
{
	AeIntCtrlInfo[Id] = CtrlInfo;
}

void AE_LoadEventTAB(AE_EVENT_TAB *EventTab)
{
	AEEventTAB = *EventTab;
}

//MovieStamp
#if AE_STAMPLOG
extern void AWB_GetColorGain(UINT32 id, UINT32 *Rg, UINT32 *Gg, UINT32 *Bg);
extern void H264Test_GetEncInfo(INT32 puiParam[8]);
static void AE_SetMovieLog(UINT32 Id, char *Buf, UINT32 BufLen)
{
	INT32 uiParam[8];
	UINT32 RGain, GGain, BGain, g3dnr;
	H264Test_GetEncInfo(uiParam);
	AWB_GetColorGain(Id, &RGain, &GGain, &BGain);
    g3dnr = iq_get_tnr_level(Id);

	snprintf(Buf, BufLen, "%3d %3d %3d %3d %3d %3d %3d %3d %5d %4d %3d %3d %3d %3d\0",
			 AeInfo[Id].PrvAvgEV.LV,
			 //AeInfo[Id].ExtSet.AdjHistoInfo.Lum.y,
			 AeInfo[Id].ExtSet.OverExp.CurTotalStep,
			 AeInfo[Id].ExtSet.OverExp.CurMaxStep,
			 AeInfo[Id].ExtSet.OverExp.CurCnt,
			 AeInfo[Id].ExtSet.OverExp.CurMinCnt,
			 AeInfo[Id].ExtSet.OverExp.CurMaxCnt,
			 AeInfo[Id].PrvAvgLum.y,
			 AeInfo[Id].CurExpectLum.y,
			 PrvNewAeArg[Id][AEStep[Id]].ExpoTime,
			 PrvNewAeArg[Id][AEStep[Id]].ISOGain,
			 g3dnr,
			 IPL_AlgGetUIInfo((IPL_PROC_ID)Id, IPL_SEL_WDR_IMM),
			 RGain, BGain
			);
}
#endif

static UINT32 AE_GetFrameRate(UINT32 id)
{
#if 1
    ER rt = E_OK;
	FPS_PARAM fps_param = {0};
    UINT32 frame_rate = 0;

	fps_param.sensor_mode = SENSOR_MODE_CUR;
	rt = sensor_get_info((SENSOR_ID)id, SEN_GET_FPS, (void *)&fps_param);
	if (rt == E_OK && fps_param.chg_fps != 0) {
		frame_rate = SEN_FPS_FMT_CONV2_REAL(fps_param.chg_fps);
	} else {
	    frame_rate = SEN_FPS_FMT_CONV2_REAL(fps_param.dft_fps);
	}

    frame_rate /= 100;
#else
    UINT32 frame_rate = 0;
	frame_rate = IPL_AlgGetUIInfo((IPL_PROC_ID)id, IPL_SEL_VIDEO_FPS);
	frame_rate = (((frame_rate & 0xffff0000) >> 16) / (frame_rate & 0xffff));
    frame_rate *= 10;
    //DBG_ERR("Frame Rate = %X, %d\r\n", IPL_AlgGetUIInfo((IPL_PROC_ID)id, IPL_SEL_VIDEO_FPS), frame_rate);
#endif

    return frame_rate;
}

static void ae_param_transfer_tmp(UINT32 id)
{
	AeParamInfo[id].PrvISO.h = ae_param_info[id]->curve_gen_param->iso_max;     ///< Maximum/Minimum ISO for streaming mode
	AeParamInfo[id].PrvISO.l = ae_param_info[id]->curve_gen_param->node[0].iso; ///< Maximum/Minimum ISO for streaming mode
	AeParamInfo[id].CapISO.h = ae_param_info[id]->curve_gen_photo->iso_max;     ///< Maximum/Minimum ISO for capture mode
	AeParamInfo[id].CapISO.l = ae_param_info[id]->curve_gen_photo->node[0].iso; ///< Maximum/Minimum ISO for capture mode
	AeParamInfo[id].AFD_Lum.h = (60 << 4);            ///< Auto anti Flicker Parameter
	AeParamInfo[id].AFD_Lum.l = (30 << 4);            ///< Auto anti Flicker Parameter
	AeParamInfo[id].ExpLum_Mov = ae_param_info[id]->expect_lum->lum_mov;              ///< @10b post-gamma Lum for Video/Recording mode
	AeParamInfo[id].ExpLum_Photo = ae_param_info[id]->expect_lum->lum_photo;            ///< @10b post-gamma Lum for Photo mode
	AeParamInfo[id].ExpLum_Histo = ae_param_info[id]->histogram->lum;            ///< @10b post-gamma Lum for Histogram
	AeParamInfo[id].ExpLum_SHDR = ae_param_info[id]->shdr->expy_le;            ///< @10b post-gamma Lum
	AeParamInfo[id].ExpLum_OverExp = ae_param_info[id]->over_exposure->lum;          ///< @10b post-gamma Lum for Anti over exposure
	AeParamInfo[id].Blk_OVER_Y_Thr.h = (220 << 2);     ///< @10b post-gamma over exposure upbound/lowbound threshold
	AeParamInfo[id].Blk_OVER_Y_Thr.l = (200 << 2);     ///< @10b post-gamma over exposure upbound/lowbound threshold
	AeParamInfo[id].Blk_UNDER_Y_Thr.h = (40 << 2);    ///< @10b post-gamma under exposure upbound/lowbound threshold
	AeParamInfo[id].Blk_UNDER_Y_Thr.l = (24 << 2);    ///< @10b post-gamma under exposure upbound/lowbound threshold
	AeParamInfo[id].BaseISO = ae_param_info[id]->base_iso;                 ///< base iso value
	AeParamInfo[id].BaseGainRatio = ae_param_info[id]->base_gain_ratio;           ///< base gain ratio (min 1000 (1x))
}

void ae_param_init(UINT32 id, UINT32* init)
{
    ae_param_info[id] = (AE_PARAM_INFO*)(*init);
}

static void ae_set_init_ev(AE_INFO *AeInfo)
{
	AeInfo->PrvAvgEV.EVValue = AeInfo->PrvCurve[1][AEALG_EV];
	AeInfo->PrvAvgEV.LV = AEAlg_Conv2LV(AeInfo->PrvAvgEV.EVValue);
    //DBG_DUMP("Init EV ======================== %d\r\n", AeInfo->PrvAvgEV.EVValue);
}

static void ae_curvegen_init(UINT32 Id)
{
    EXPOSURE_PARAM exp_param = {0};
    ER rt = E_OK;

    if(ae_param_info[Id]->curve_gen_param->fno_sel==FNO_SEL_PREDEF)
        AeCurveGenInfo[Id].fno_rat = GetFnoRatio(Id);
    else {
        AeCurveGenInfo[Id].fno_idx = 0;
        AeCurveGenInfo[Id].fno_rat = ae_param_info[Id]->curve_gen_param->fno_rat;
    }

    if(ae_param_info[Id]->curve_gen_param->iso_max < ae_param_info[Id]->curve_gen_param->isp_gain_thres)
    {
        DBG_ERR("Force set ISP gain threshold to ISO_Max! (ISP Gain > ISO Max)\r\n");
        ae_param_info[Id]->curve_gen_param->iso_max = ae_param_info[Id]->curve_gen_param->isp_gain_thres;
    }

    // Auto calculate minimum exposure time
	exp_param.exp_time[0] = 10000;
	exp_param.sensor_mode = SENSOR_MODE_CUR;
	rt = sensor_get_info((SENSOR_ID)Id, SEN_GET_EXPO_SETTING, (void *)&exp_param);

    if(rt != E_OK)
        DBG_ERR("Get sensor information error! (SEN_GET_EXPO_SETTING)\r\n");

    ae_param_info[Id]->curve_gen_param->node[0].expt = ((exp_param.exp_time[0]+(exp_param.exp_setting.ui_line[0]>>1))/exp_param.exp_setting.ui_line[0]);

    // Check max ev overflow
	AeCurveGenInfo[Id].fno_rat = AE_Check_Fno(Id, AeCurveGenInfo[Id].fno_rat);
    ae_param_info[Id]->curve_gen_param->fno_rat = AeCurveGenInfo[Id].fno_rat;

	AeCurveGenInfo[Id].node_num = ae_param_info[Id]->curve_gen_param->node_num;
    memcpy(AeCurveGenInfo[Id].node, ae_param_info[Id]->curve_gen_param->node, sizeof(AeCurveGenInfo[Id].node));
    AeCurveGenInfo[Id].iso_max = ae_param_info[Id]->curve_gen_param->iso_max;
    AeCurveGenInfo[Id].freq = (ae_param_info[Id]->curve_gen_param->freq == AE_FLICKER_50HZ) ? AE_GEN_FREQ_50 : AE_GEN_FREQ_60;
    AeCurveGenInfo[Id].frame_rate = AE_GetFrameRate(Id);
    AeCurveGenInfo[Id].hdr_frame = IPL_CtrlGetInfor(Id, IPLCTRL_MF_TOTAL_FRM_NUM);
    memcpy(AeCurveGenInfo[Id].hdr_ratio, ae_param_info[Id]->curve_gen_param->hdr_ratio, sizeof(AeCurveGenInfo[Id].hdr_ratio));
    AeCurveGenInfo[Id].hdr_exp_gap = 0;
    AeCurveGenInfo[Id].auto_lowlight_en = ae_param_info[Id]->curve_gen_param->auto_lowlight_en;
    memcpy(AeCurveGenInfo[Id].ext_fps, ae_param_info[Id]->curve_gen_param->ext_fps, sizeof(AeCurveGenInfo[Id].ext_fps));
    AeCurveGenInfo[Id].iso_mode = ae_param_info[Id]->curve_gen_param->iso_mode;
    AeCurveGenInfo[Id].scene_mode = ae_get_ui_info((IPL_PROC_ID)Id, AE_UI_SCENE);
    AeCurveGenInfo[Id].dbg_expt = 0;
    AeCurveGenInfo[Id].dbg_iso = 0;
    AeCurveGenInfo[Id].iso_value = ae_get_ui_info((IPL_PROC_ID)Id, AE_UI_ISO);

    AeInfo[Id].MF_Num = AeCurveGenInfo[Id].hdr_frame;

    // For Capture
    if(ae_param_info[Id]->curve_gen_param->fno_sel==FNO_SEL_PREDEF)
        AeCurveGenPhoto[Id].fno_rat = GetFnoRatio(Id);
    else {
        AeCurveGenPhoto[Id].fno_idx = 0;
        AeCurveGenPhoto[Id].fno_rat = ae_param_info[Id]->curve_gen_param->fno_rat;
    }
    AeCurveGenPhoto[Id].iso_value = ae_get_ui_info((IPL_PROC_ID)Id, AE_UI_ISO);
    AeCurveGenPhoto[Id].freq = (ae_param_info[Id]->curve_gen_param->freq == AE_FLICKER_50HZ) ? AE_GEN_FREQ_50 : AE_GEN_FREQ_60;
    AeCurveGenPhoto[Id].scene_mode = ae_get_ui_info((IPL_PROC_ID)Id, AE_UI_SCENE);
    ae_param_info[Id]->curve_gen_photo->node[0].expt = ae_param_info[Id]->curve_gen_param->node[0].expt;
    memcpy(AeCurveGenPhoto[Id].node, ae_param_info[Id]->curve_gen_photo->node, sizeof(AeCurveGenPhoto[Id].node));
	AeCurveGenPhoto[Id].node_num = ae_param_info[Id]->curve_gen_photo->node_num;
    AeCurveGenPhoto[Id].expt_max = ae_param_info[Id]->curve_gen_photo->expt_max;
    AeCurveGenPhoto[Id].iso_max = ae_param_info[Id]->curve_gen_photo->iso_max;
    AeCurveGenPhoto[Id].iso_mode = ae_param_info[Id]->curve_gen_photo->iso_mode;
}

void AE_Init(UINT32 Id, UINT32 CurrentStatus)
{
	SENSOR_MODE Mode;
	static BOOL Init_Flag[AE_ID_MAX_NUM] = {FALSE};
	IPL_MODE_INFOR Info = {0};
	ER rt = E_OK;
	//AE_EXT_SET_INFO ExtInfo = {0};
	GAIN_PARAM gain_param = {0};
	EXPOSURE_PARAM exp_param = {0};
    //UINT8 uiExpAreaWeight[AE_WIN_Y][AE_WIN_X];
    UINT32 i;

    if(!pqs_ae_check(Id)) {
    	UINT32 ae_init = 0;
    	pqs_ae_init(Id, &ae_init);
    	ae_param_init(Id, &ae_init);
    }

	AeInfo[Id].Id = Id;

	AeInfo[Id].PrvBiningMode = AE_getPrvBiningMode(Id);
	AeInfo[Id].CapBiningMode = AE_getCapBiningMode(Id);

    AE_LumGamma_Init(Id);

	//initial ae algorithm
	if (FALSE == Init_Flag[Id]) {
        //aes_map_init();
       // ae_param_info[Id] = aes_getptr(Id);//&ae_param_sample;
        ae_param_transfer_tmp(Id);

        AeCurveGenInfo[Id].dbg_expt = 0;
        AeCurveGenInfo[Id].dbg_iso = 0;

		Init_Flag[Id] = TRUE;
		AE_getInfo(&AeInfo[Id], &AeBlkInfo[Id]);
        AE_setWin(Id, AE_getWinType(ae_get_ui_info((IPL_PROC_ID)Id, AE_UI_METER)));
        ae_curvegen_init(Id);
		//get ae table
		AE_getPrvTab(&AeInfo[Id]);
        //ExtInfo.ExpAreaWeight = &uiExpAreaWeight[0][0];
        //AE_GetExtInfo(Id, &ExtInfo);
		//AE_SetExtInfo(Id, ExtInfo);
        //set ae init ev use flicker base + 1x gain
        ae_set_init_ev(&AeInfo[Id]);
        AE_getCapTab(&AeInfo[Id]);

		//get & save iso, exposure time, iris
		AEAlg_Allot(Id, AeInfo[Id].PrvAvgEV.EVValue, AeInfo[Id].PrvBiningMode, AeInfo[Id].PrvCurve, AeInfo[Id].EventTab.GetFnoRatio, AeInfo[Id].EventTab.GetCurveOfsRatio, &PrvNewAeArg[Id][0]);
		if (PrvNewAeArg[Id][0].ISOGain > AeParamInfo[Id].PrvISO.h) {
			PrvNewAeArg[Id][0].ISOGain = AeParamInfo[Id].PrvISO.h;
		}
		if (PrvNewAeArg[Id][0].ISOGain < AeParamInfo[Id].PrvISO.l) {
			PrvNewAeArg[Id][0].ISOGain = AeParamInfo[Id].PrvISO.l;
		}
	} else {
//        ae_param_info[Id] = aes_getptr(Id);//&ae_param_sample;
        ae_param_transfer_tmp(Id);
		//get ae table
		AE_getPrvTab(&AeInfo[Id]);
        AE_getCapTab(&AeInfo[Id]);
		//get & save iso, exposure time, iris
		AEAlg_Allot(Id, AeInfo[Id].PrvParam[0].EVValue, AeInfo[Id].PrvBiningMode, AeInfo[Id].PrvCurve, AeInfo[Id].EventTab.GetFnoRatio, AeInfo[Id].EventTab.GetCurveOfsRatio, &PrvNewAeArg[Id][0]);
		if (PrvNewAeArg[Id][0].ISOGain > AeParamInfo[Id].PrvISO.h) {
			PrvNewAeArg[Id][0].ISOGain = AeParamInfo[Id].PrvISO.h;
		}
		if (PrvNewAeArg[Id][0].ISOGain < AeParamInfo[Id].PrvISO.l) {
			PrvNewAeArg[Id][0].ISOGain = AeParamInfo[Id].PrvISO.l;
		}
	}

    //AE_DumpCurveInfo(Id);

	AE_setFlag(Id, (AE_FLAG_SET_IRIS | AE_FLAG_SET_GAIN | AE_FLAG_SET_EXPT));
	AE_SetCalGain(Id);

	//Switch AE Function for different IPL mode.
	Info.id = Id;
	rt = IPL_GetCmd(IPL_GET_CUR_MODE, (void *)&Info);
	if (rt != E_OK) {
		return;
	}

	//rt = Sensor_GetCurMode(IPL_UTI_CONV2_SEN_ID(Id), &Mode);
	rt = sensor_get_info((SENSOR_ID)Id, SEN_GET_CUR_MODE, (void *)&Mode);
	if (rt != E_OK) {
		return;
	}

    // Init HDR parameter for change HDR mode during preview
    ae_shdr_trans[Id].shdr_frm = IPL_CtrlGetInfor((IPL_PROC_ID)Id, IPLCTRL_MF_TOTAL_FRM_NUM);

    for(i=0; i<(sizeof(ae_shdr_trans[Id].ev_ratio.uiEVratio)/sizeof(ae_shdr_trans[Id].ev_ratio.uiEVratio[0])); i++){
        ae_shdr_trans[Id].ev_ratio.uiEVratio[i] = ae_param_info[Id]->curve_gen_param->hdr_ratio[i];
    }

    ae_shdr_info[Id].se_ev_ratio = ae_param_info[Id]->curve_gen_param->hdr_ratio[1];
    ae_shdr_info[Id].curr_se_lum = 0;
    ae_shdr_info[Id].range_w.h = 1000;
    ae_shdr_info[Id].range_w.l = 1;

	if (IPL_AlgGetUIInfo((IPL_PROC_ID)Id, IPL_SEL_SHDR) == SEL_SHDR_ON) {

        for(i=0; i<ae_shdr_trans[Id].shdr_frm; i++) {
            if(ae_shdr_trans[Id].ev_ratio.uiEVratio[i] >= 64){
        		ae_shdr_trans[Id].expo_time[i] = ((PrvNewAeArg[Id][0].ExpoTime<<6)/ae_shdr_trans[Id].ev_ratio.uiEVratio[i]);
        		exp_param.exp_time[i] = ae_shdr_trans[Id].expo_time[i];
            }
            else{
        		DBG_ERR("HDR EV ratio error (%d)\r\n", ae_shdr_trans[Id].ev_ratio.uiEVratio[i]);
            }
        }

		exp_param.sensor_mode = SENSOR_MODE_CUR;
		rt = sensor_get_info((SENSOR_ID)Id, SEN_GET_EXPO_SETTING, (void *)&exp_param);
        if(rt != E_OK)
            DBG_ERR("Get sensor informatoin error! (SEN_GET_EXPO_SETTING)\r\n");

        for(i=0; i<ae_shdr_trans[Id].shdr_frm; i++) {
    		Ratio[i] = exp_param.compensate_ratio[i];
    		ae_shdr_trans[Id].iso_gain[i] = ((PrvNewAeArg[Id][0].ISOGain * Ratio[i]) / 100);
    		gain_param.gain_ratio[i] = (ae_shdr_trans[Id].iso_gain[i] * AeParamInfo[Id].BaseGainRatio) / AeParamInfo[Id].BaseISO;
        }

		gain_param.sensor_mode = SENSOR_MODE_CUR;
		rt = sensor_get_info((SENSOR_ID)Id, SEN_GET_GAIN_SETTING, (void *)&gain_param);
        if(rt != E_OK)
            DBG_ERR("Get sensor informatoin error! (SEN_GET_GAIN_SETTING)\r\n");

	} else {
		ExpoTime[0] = PrvNewAeArg[Id][0].ExpoTime;
		exp_param.exp_time[0] = ExpoTime[0];
		//Sensor_GetExpoSetting(IPL_UTI_CONV2_SEN_ID(Id), &ExpoTime[0], Mode, &ExpTSetting, &Ratio[0]);
		exp_param.sensor_mode = SENSOR_MODE_CUR;
		rt = sensor_get_info((SENSOR_ID)Id, SEN_GET_EXPO_SETTING, (void *)&exp_param);
        if(rt != E_OK)
            DBG_ERR("Get sensor information error! (SEN_GET_EXPO_SETTING)\r\n");

		Ratio[0] = exp_param.compensate_ratio[0];
		ISOGain[0] = PrvNewAeArg[Id][0].ISOGain * Ratio[0] / 100;
		gain_param.gain_ratio[0] = (ISOGain[0] * AeParamInfo[Id].BaseGainRatio) / AeParamInfo[Id].BaseISO;
		//Sensor_GetGainSetting(IPL_UTI_CONV2_SEN_ID(Id), &ISOGain[0], &Gain);
		gain_param.sensor_mode = SENSOR_MODE_CUR;
		rt = sensor_get_info((SENSOR_ID)Id, SEN_GET_GAIN_SETTING, (void *)&gain_param);
        if(rt != E_OK)
            DBG_ERR("Get sensor information error! (SEN_GET_GAIN_SETTING)\r\n");
	}

	//AE_setExpT(Id, &ExpTSetting);
	AE_setExpT(Id, &exp_param.exp_setting);
	//AE_setGain(Id, &Gain);
	AE_setGain(Id, &gain_param.gain_setting);

    // DC Iris initial
    ae_dciris_init(Id);

	//Reset to default value
	AECnt[Id] = 0;
    AECntFlag[Id] = TRUE;
	AEStep[Id] = 0;
	AeInfo[Id].Counter = 0;
	AeInfo[Id].CounterRF = 0;
	AeInfo[Id].FirstStableFlag = FALSE;
	memset(gGain[Id], 0xff, sizeof(gGain[Id]));
	memset(gExpTSetting[Id], 0xff, sizeof(gExpTSetting[Id]));
	AeSyncCnt[Id] = 0;
	memset(&AeSyncInfo[Id][0], 0x0, (sizeof(AE_SYNC_INFO) * AE_SYNC_TAB_SIZE));

    ae_shdr_trans[Id].iso_min = ae_param_info[Id]->curve_gen_param->node[0].iso;
    ae_shdr_trans[Id].iso_max = ae_param_info[Id]->curve_gen_param->iso_max;
    ae_shdr_trans[Id].se_iso_ub = ae_param_info[Id]->shdr->se_iso_ub;
    ae_shdr_trans[Id].ev_opt = ae_param_info[Id]->shdr->ev_opt;

    AE_HDR_Switch[Id] = IPL_AlgGetUIInfo((IPL_PROC_ID)Id, IPL_SEL_SHDR);
    ae_shdr_trans[Id].shdr_frm = IPL_CtrlGetInfor(Id, IPLCTRL_MF_TOTAL_FRM_NUM);
    AeInfo[Id].MF_Num = ae_shdr_trans[Id].shdr_frm;

    for(i=0; i<AEALG_DYNAMIC_LV_NUM; i++)
        AE_ClampRatio[Id][i] = ae_param_info[Id]->la_clamp->tab_shdr_h[i];

#if AE_STAMPLOG
	IQS_SetMovieLogFP(AE_SetMovieLog);// for AE debug message log
#endif

	isp_ae_init(Id, (void *)&ae_status_info[Id], (void *)ae_param_info[Id], (void*)&AeInfo[Id]);
}

static void AE_SetCalGain(IPL_PROC_ID Id)
{
#if CAL_ISO
	pCAL_TAG pTag;
	CAL_ISO_FMT *pFmt;
	BOOL Cal_Status;
	UINT32 Cal_Gain = 0;
	CAL_DATA_INFO CaldataInfo = {0};
	CaldataInfo.StationID = SEC_ISO;
	CaldataInfo.SenMode = IPL_AlgGetUIInfo((IPL_PROC_ID)Id, IPL_SEL_CAPSENMODE);
	CaldataInfo.CalCondition[0] = CAL_CONDITION_DEFAULT;

	pTag = GetCalData(Id, CaldataInfo);

	Cal_Status = FALSE;

	if (pTag != NULL) {
		DBG_IND("get ISO Calibration data\r\n");
		if (pTag->CAL_STATUS == _CAL_OK) {
			pFmt = (CAL_ISO_FMT *)&pTag->TagData;

			if (pFmt->GainBase != 0) {
				Cal_Status = TRUE;
				Cal_Gain = pFmt->GainBase;
				DBG_IND("iso gain 0x%x\r\n", Cal_Gain);
			} else {
				Cal_Status = FALSE;
				DBG_ERR("read ISO Calibration gain Fail\r\n");
			}

			DBG_IND("read ISO Calibration gain = %d %d\r\n", pFmt->GainBase, IPL_AlgGetUIInfo((IPL_PROC_ID)Id, IPL_SEL_CAPSENMODE));
		} else {
			DBG_IND("cal status not OK\r\n");
		}
	} else {
		DBG_IND("NO ISO cal data\r\n");
	}

	Sensor_SetAGC_ISOBase(IPL_UTI_CONV2_SEN_ID(Id), Cal_Status, Cal_Gain);
#endif
}

static void AE_SetSyncInfo(UINT32 Id, AEALG_ARG *data)
{
	IPL_SIE_CUR_FRAME_INFO Info = {0};

	Info.id = Id;
	IPL_GetCmd(IPL_GET_SIE_CUR_FRAME_INFOR, &Info);

	AeSyncInfo[Id][AeSyncCnt[Id]].frame_cnt = Info.frame_cnt + 3;
	AeSyncInfo[Id][AeSyncCnt[Id]].data = *data;

	AeSyncCnt[Id] ++;
	if (AeSyncCnt[Id] >= AE_SYNC_TAB_SIZE) {
		AeSyncCnt[Id] = 0;
	}
}

ER AE_GetSyncInfo(UINT32 Id, UINT32 frame_cnt, AEALG_ARG *data)
{
	UINT32 i;
	for (i = 0; i < AE_SYNC_TAB_SIZE; i ++) {
		if (frame_cnt == AeSyncInfo[Id][i].frame_cnt) {
			*data = AeSyncInfo[Id][i].data;
			return E_OK;
		}
	}
	return E_SYS;
}

#define AE_CNT_MAX      6

#define AE_GAIN_IMM_SYNC_STEP       1

void AE_Process(UINT32 Id, UINT32 CurrentStatus)
{
	UINT32 i = 0, j = 0;
	AE_RESET AeRst = AE_RESET_FALSE;
	static UINT32 InStrPos[AE_ID_MAX_NUM];
	static SENSOR_INFO SenInfo;
	static SEN_STATUS_PARAM sensor_status = {0};
	ER rt = E_OK;
	GAIN_PARAM gain_param = {0};
	EXPOSURE_PARAM exp_param = {0};
    UINT32 shdr_min_expt;
	UINT32 sqrt_idx;

    AECnt[Id] ++;

	sensor_status.sensor_mode = SENSOR_MODE_CUR;
	rt |= sensor_get_info((SENSOR_ID)Id, SEN_GET_STATUS, (void *)&sensor_status);
	SenInfo = sensor_status.sensor_info;
	if (rt != E_OK || SenInfo.mode == NULL) {
		return;
	}

	if (AECntFlag[Id]==TRUE) {
        DBG_DUMP("========== AE_CBMSG_PREVIEWSTABLE ==========\r\n");
		AE_CBMsgProc(Id, AE_CBMSG_PREVIEWSTABLE, NULL);
        AECntFlag[Id] = FALSE;
    }

	if (AECnt[Id] < AE_CNT_MAX){
        return;
    }

	//Set ExpT and ISOGain by AE Uart CMD (for Debug).
	if (uiISOGain[Id] != 0 && uiExpoTime[Id] != 0) {
		PrvNewAeArg[Id][0].ExpoTime = uiExpoTime[Id];
		PrvNewAeArg[Id][0].ISOGain = uiISOGain[Id];

        AE_getStatisticInfo(&AeInfo[Id], &AeBlkInfo[Id]);

		if (AEUartMsg == ENABLE || uiAEUartCmd[Id] > AE_UART_OFF) {
			DBG_DUMP("MAE Id: %d, Y: %d, Exp: %d, ISO: %d\r\n", Id, AE_AlgGetUIInfo((AE_PROC_ID)Id, AE_SEL_CURR_Y), uiExpoTime[Id], uiISOGain[Id]);
		}

        if (IPL_AlgGetUIInfo((IPL_PROC_ID)Id, IPL_SEL_SHDR) == SEL_SHDR_ON){
            ae_shdr_trans[Id].shdr_frm = IPL_CtrlGetInfor(Id, IPLCTRL_MF_TOTAL_FRM_NUM);
            AeInfo[Id].MF_Num = ae_shdr_trans[Id].shdr_frm;

            ae_shdr_trans[Id].ev_ratio = SensorHDR_GetAeRatio();

            ae_shdr_trans[Id].expo_time[0] = PrvNewAeArg[Id][0].ExpoTime;
            ae_shdr_trans[Id].iso_gain[0] = PrvNewAeArg[Id][0].ISOGain;

            AEAlg_SHDR_ExposureTransfer(&ae_shdr_trans[Id]);

            for(j=0; j<ae_shdr_trans[Id].shdr_frm; j++) {

                shdr_min_expt = ((ae_param_info[Id]->shdr->min_expt_se*ae_shdr_trans[Id].ev_ratio.uiEVratio[ae_shdr_trans[Id].shdr_frm-j-1])>>6);

                if(ae_shdr_trans[Id].expo_time[j] < shdr_min_expt)
                   ae_shdr_trans[Id].expo_time[j] = shdr_min_expt;
        		exp_param.exp_time[j] = ae_shdr_trans[Id].expo_time[j];
        		gain_param.gain_ratio[j] = (ae_shdr_trans[Id].iso_gain[j] * AeParamInfo[Id].BaseGainRatio) / AeParamInfo[Id].BaseISO;
            }

    		if (AEUartMsg == ENABLE || uiAEUartCmd[Id] > AE_UART_OFF) {
                UINT32 ev_ratio_base;
                DBG_DUMP("SHDR_EV_Ratio = ");
                for(j=0; j<ae_shdr_trans[Id].shdr_frm; j++) {
                    DBG_DUMP("%4d, ", ae_shdr_trans[Id].ev_ratio.uiEVratio[j]);
                }
                DBG_DUMP("\r\n");
                for(j=0; j<ae_shdr_trans[Id].shdr_frm; j++) {
                    DBG_DUMP("MAE frm%d : %6d, %4d\r\n", j, ae_shdr_trans[Id].expo_time[j], ae_shdr_trans[Id].iso_gain[j]);
                }
                DBG_DUMP("MAE_EV_Ratio = ");
                ev_ratio_base = (ae_shdr_trans[Id].expo_time[0] * ae_shdr_trans[Id].iso_gain[0]);
                for(j=1; j<ae_shdr_trans[Id].shdr_frm; j++) {
                    DBG_DUMP("%4d, ", ((ev_ratio_base*100)/(ae_shdr_trans[Id].expo_time[j]*ae_shdr_trans[Id].iso_gain[j])));
                }
    		}

			exp_param.sensor_mode = SENSOR_MODE_CUR;
			rt = sensor_get_info((SENSOR_ID)Id, SEN_GET_EXPO_SETTING, (void *)&exp_param);
            if(rt != E_OK)
                DBG_ERR("Get sensor information error! (SEN_GET_EXPO_SETTING)\r\n");

			for (j = 0; j < ae_shdr_trans[Id].shdr_frm; j++) {
				gExpTSetting[Id][0].ui_frame[j] = exp_param.exp_setting.ui_frame[j];
				gExpTSetting[Id][0].ui_line[j] = exp_param.exp_setting.ui_line[j];
				gExpTSetting[Id][0].ui_pixel[j] = exp_param.exp_setting.ui_pixel[j];
			}

			gain_param.sensor_mode = SENSOR_MODE_CUR;
			rt = sensor_get_info((SENSOR_ID)Id, SEN_GET_GAIN_SETTING, (void *)&gain_param);
            if(rt != E_OK)
                DBG_ERR("Get sensor information error! (SEN_GET_GAIN_SETTING)\r\n");

			for (j = 0; j < ae_shdr_trans[Id].shdr_frm; j++) {
				gGain[Id][0].data1[j] = gain_param.gain_setting.data1[j];
				gGain[Id][0].data2[j] = gain_param.gain_setting.data2[j];
				gGain[Id][0].data3[j] = gain_param.gain_setting.data3[j];
			}

    		AE_setFlag(Id, AE_FLAG_SET_GAIN | AE_FLAG_SET_EXPT);
    		AE_setExpT(Id, &gExpTSetting[Id][0]);
    		AE_setGain(Id, &gGain[Id][0]);
    		iq_set_flow(Id, IQS_AE_END);
    		CapNewAeArg[Id].ExpoTime = uiExpoTime[Id];
    		CapNewAeArg[Id].ISOGain = uiISOGain[Id];
        }
        else{

    		ExpoTime[Id] = uiExpoTime[Id];
    		exp_param.exp_time[0] = ExpoTime[Id];

    		//Sensor_GetExpoSetting(IPL_UTI_CONV2_SEN_ID(Id), &ExpoTime[0], Mode, &gExpTSetting[Id][0], &Ratio[0]);
    		exp_param.sensor_mode = SENSOR_MODE_CUR;
    		rt = sensor_get_info((SENSOR_ID)Id, SEN_GET_EXPO_SETTING, (void *)&exp_param);
            if(rt != E_OK)
                DBG_ERR("Get sensor information error! (SEN_GET_EXPO_SETTING)\r\n");

    		memcpy(&gExpTSetting[Id][0], &exp_param.exp_setting, sizeof(EXPOSURE_SETTING) / sizeof(UINT32));

    		gExpTSetting[Id][0].ui_frame[0] = exp_param.exp_setting.ui_frame[0];
    		gExpTSetting[Id][0].ui_line[0] = exp_param.exp_setting.ui_line[0];
    		gExpTSetting[Id][0].ui_pixel[0] = exp_param.exp_setting.ui_pixel[0];

    		ISOGain[0] = uiISOGain[Id];

			if (ISOGain[0] > ae_param_info[Id]->curve_gen_param->isp_gain_thres){
                AeIspGain[Id] = (ISOGain[0] * 128)/ae_param_info[Id]->curve_gen_param->isp_gain_thres;
                ISOGain[0] = ae_param_info[Id]->curve_gen_param->isp_gain_thres;
			} else {
			    AeIspGain[Id] = 128;
			}

            AeIspGain[Id] = ae_clamp(AeIspGain[Id], 128, 1023);

    		gain_param.gain_ratio[0] = (ISOGain[0] * AeParamInfo[Id].BaseGainRatio) / AeParamInfo[Id].BaseISO;

    		gain_param.sensor_mode = SENSOR_MODE_CUR;
    		rt = sensor_get_info((SENSOR_ID)Id, SEN_GET_GAIN_SETTING, (void *)&gain_param);
            if(rt != E_OK)
                DBG_ERR("Get sensor information error! (SEN_GET_GAIN_SETTING)\r\n");

    		memcpy(&gGain[Id][0], &gain_param.gain_setting, sizeof(GAIN_SETTING) / sizeof(UINT32));

    		gGain[Id][0].data1[0] = gain_param.gain_setting.data1[0];
    		gGain[Id][0].data2[0] = gain_param.gain_setting.data2[0];
    		gGain[Id][0].data3[0] = gain_param.gain_setting.data3[0];

    		AE_setFlag(Id, AE_FLAG_SET_GAIN | AE_FLAG_SET_EXPT);
    		AE_setExpT(Id, &gExpTSetting[Id][0]);
    		AE_setGain(Id, &gGain[Id][0]);
    		iq_set_flow(Id, IQS_AE_END);
    		CapNewAeArg[Id].ExpoTime = uiExpoTime[Id];
    		CapNewAeArg[Id].ISOGain = uiISOGain[Id];
        }

		//Trigger SHDR Process here
		AE_DRC_NotifyEvent(IPL_UTI_CONV2_AE_ID(Id));

        AE_SetSyncInfo(Id, &PrvNewAeArg[Id][0]);

		return ;
	}

	if (AEStep[Id] == 0) {
		if (AECnt[Id] % 2 == 0) {
			InStrPos[Id] = 0;
		} else {
			InStrPos[Id] = AE_TOTAL_PARAMNUM / 2;
		}

		//set ae control parameter
		AE_getCtrlInfo(&AeInfo[Id], &AeBlkInfo[Id]);
		//set ae window
        AeRst |= AE_setWin(Id, AE_getWinType(ae_get_ui_info((IPL_PROC_ID)Id, AE_UI_METER)));
		//get preview ae curve
		AeRst |= AE_getPrvTab(&AeInfo[Id]);
		//get capture ae curve
		AeRst |= AE_getCapTab(&AeInfo[Id]);
		AeInfo[Id].Reset |= AeRst;

        AE_Check_EVChange(Id);

		// get statistic informaiton
		AE_getStatisticInfo(&AeInfo[Id], &AeBlkInfo[Id]);

        // For pseudo driver command change
		if(AE_EVCompIdx[Id] != AeInfo[Id].EVCompIdx) {
            ae_set_ui_info((IPL_PROC_ID)Id, AE_UI_EV, AeInfo[Id].EVCompIdx);
            AE_EVCompIdx[Id] = ae_get_ui_info((IPL_PROC_ID)Id, AE_UI_EV);
		}

		//comp ev
        AeInfo[Id].EVCompRatio = EVTable[ae_get_ui_info((IPL_PROC_ID)Id, AE_UI_EV)];

		if (IPL_AlgGetUIInfo((IPL_PROC_ID)Id, IPL_SEL_SHDR) == SEL_SHDR_ON){

            ae_shdr_trans[Id].shdr_frm = IPL_CtrlGetInfor(Id, IPLCTRL_MF_TOTAL_FRM_NUM);
            AeInfo[Id].MF_Num = ae_shdr_trans[Id].shdr_frm;

            if(ae_param_info[Id]->shdr->ev_opt == AE_SHDR_EV_OPT_EXPY) {
                ae_shdr_trans[Id].ev_ratio.uiEVratio[0] = 64;
                switch(ae_shdr_trans[Id].shdr_frm)
                {
                case 2:
                    ae_shdr_trans[Id].ev_ratio.uiEVratio[1] = ae_shdr_info[Id].se_ev_ratio;
                    break;
                case 3:
                    sqrt_idx = (ae_shdr_info[Id].se_ev_ratio+ae_shdr_trans[Id].ev_ratio.uiEVratio[0]);
                    sqrt_idx = (sqrt_idx+32)>>6;
                    ae_shdr_trans[Id].ev_ratio.uiEVratio[1] = ((Ae_Sqrt_Tbl[sqrt_idx]<<6)/100);
                    ae_shdr_trans[Id].ev_ratio.uiEVratio[2] = ae_shdr_info[Id].se_ev_ratio;
                    break;
                case 1:
                case 4:
                    ae_shdr_trans[Id].ev_ratio.uiEVratio[1] = ae_param_info[Id]->curve_gen_param->hdr_ratio[1];
                    ae_shdr_trans[Id].ev_ratio.uiEVratio[2] = ae_param_info[Id]->curve_gen_param->hdr_ratio[2];
                    ae_shdr_trans[Id].ev_ratio.uiEVratio[3] = ae_param_info[Id]->curve_gen_param->hdr_ratio[3];
                    break;
                }
            } else {
                ae_shdr_trans[Id].ev_ratio = SensorHDR_GetAeRatio();
            }

			AEAlg_Converge(&AeInfo[Id], AeIntCtrlInfo[Id].PredictPoint, &PrvNewAeArg[Id][0], &CapNewAeArg[Id]);

 			for (i = 0; i < AeIntCtrlInfo[Id].PredictPoint; i ++) {
                ae_shdr_trans[Id].expo_time[0] = PrvNewAeArg[Id][i].ExpoTime;
                ae_shdr_trans[Id].iso_gain[0] = PrvNewAeArg[Id][i].ISOGain;

                AEAlg_SHDR_ExposureTransfer(&ae_shdr_trans[Id]);

                if(i==0)
                {
                    if(ae_param_info[Id]->shdr->ev_opt == AE_SHDR_EV_OPT_EXPY) {
                        SensorHDR_SetAeRatio(ae_shdr_trans[Id].ev_ratio);
                    }
                }

                for(j=0; j<ae_shdr_trans[Id].shdr_frm; j++) {

                    shdr_min_expt = ((ae_param_info[Id]->shdr->min_expt_se*ae_shdr_trans[Id].ev_ratio.uiEVratio[ae_shdr_trans[Id].shdr_frm-j-1])>>6);

                    if(ae_shdr_trans[Id].expo_time[j] < shdr_min_expt)
                       ae_shdr_trans[Id].expo_time[j] = shdr_min_expt;

            		exp_param.exp_time[j] = ae_shdr_trans[Id].expo_time[j];
            		gain_param.gain_ratio[j] = (ae_shdr_trans[Id].iso_gain[j] * AeParamInfo[Id].BaseGainRatio) / AeParamInfo[Id].BaseISO;
                }

				exp_param.sensor_mode = SENSOR_MODE_CUR;
				rt = sensor_get_info((SENSOR_ID)Id, SEN_GET_EXPO_SETTING, (void *)&exp_param);
                if(rt != E_OK)
                    DBG_ERR("Get sensor information error! (SEN_GET_EXPO_SETTING)\r\n");

				for (j = 0; j < ae_shdr_trans[Id].shdr_frm; j++) {
					gExpTSetting[Id][InStrPos[Id] + i + AeIntCtrlInfo[Id].CalSetShiftNum].ui_frame[j] = exp_param.exp_setting.ui_frame[j];
					gExpTSetting[Id][InStrPos[Id] + i + AeIntCtrlInfo[Id].CalSetShiftNum].ui_line[j] = exp_param.exp_setting.ui_line[j];
					gExpTSetting[Id][InStrPos[Id] + i + AeIntCtrlInfo[Id].CalSetShiftNum].ui_pixel[j] = exp_param.exp_setting.ui_pixel[j];
				}

				gain_param.sensor_mode = SENSOR_MODE_CUR;
				rt = sensor_get_info((SENSOR_ID)Id, SEN_GET_GAIN_SETTING, (void *)&gain_param);
                if(rt != E_OK)
                    DBG_ERR("Get sensor information error! (SEN_GET_GAIN_SETTING)\r\n");

				for (j = 0; j < ae_shdr_trans[Id].shdr_frm; j++) {
					gGain[Id][(InStrPos[Id] + i + SenInfo.sync_timing + AeIntCtrlInfo[Id].CalSetShiftNum) % AE_TOTAL_PARAMNUM].data1[j] = gain_param.gain_setting.data1[j];
					gGain[Id][(InStrPos[Id] + i + SenInfo.sync_timing + AeIntCtrlInfo[Id].CalSetShiftNum) % AE_TOTAL_PARAMNUM].data2[j] = gain_param.gain_setting.data2[j];
					gGain[Id][(InStrPos[Id] + i + SenInfo.sync_timing + AeIntCtrlInfo[Id].CalSetShiftNum) % AE_TOTAL_PARAMNUM].data3[j] = gain_param.gain_setting.data3[j];
				}
			}
 		} else {

 		    if ((ae_param_info[Id]->iris_cfg->enable == DISABLE)||(AeIrisInfo[Id].exposure_freeze == 0)) {

    			AEAlg_Converge(&AeInfo[Id], AeIntCtrlInfo[Id].PredictPoint, &PrvNewAeArg[Id][0], &CapNewAeArg[Id]);

    			for (i = 0; i < AeIntCtrlInfo[Id].PredictPoint; i ++) {
    				ExpoTime[0] = PrvNewAeArg[Id][i].ExpoTime;
    				exp_param.exp_time[0] = ExpoTime[0];
    				exp_param.sensor_mode = SENSOR_MODE_CUR;
    				rt = sensor_get_info((SENSOR_ID)Id, SEN_GET_EXPO_SETTING, (void *)&exp_param);
                    if(rt != E_OK)
                        DBG_ERR("Get sensor information error! (SEN_GET_GAIN_SETTING)\r\n");

    				gExpTSetting[Id][InStrPos[Id] + i + AeIntCtrlInfo[Id].CalSetShiftNum].ui_frame[0] = exp_param.exp_setting.ui_frame[0];
    				gExpTSetting[Id][InStrPos[Id] + i + AeIntCtrlInfo[Id].CalSetShiftNum].ui_line[0] = exp_param.exp_setting.ui_line[0];
    				gExpTSetting[Id][InStrPos[Id] + i + AeIntCtrlInfo[Id].CalSetShiftNum].ui_pixel[0] = exp_param.exp_setting.ui_pixel[0];

    				Ratio[0] = exp_param.compensate_ratio[0];
    				PrvNewAeArg[Id][i].ISOGain = (PrvNewAeArg[Id][i].ISOGain * Ratio[0] / 100);
    				ISOGain[0] = PrvNewAeArg[Id][i].ISOGain;

    				if (ISOGain[0] > ae_param_info[Id]->curve_gen_param->isp_gain_thres){
                        AeIspGain[Id] = (ISOGain[0] * 128)/ae_param_info[Id]->curve_gen_param->isp_gain_thres;
                        ISOGain[0] = ae_param_info[Id]->curve_gen_param->isp_gain_thres;
    				} else {
    				    AeIspGain[Id] = 128;
    				}

                    AeIspGain[Id] = ae_clamp(AeIspGain[Id], 128, 1023);

    				gain_param.gain_ratio[0] = (ISOGain[0] * AeParamInfo[Id].BaseGainRatio) / AeParamInfo[Id].BaseISO;
    				gain_param.sensor_mode = SENSOR_MODE_CUR;
    				rt = sensor_get_info((SENSOR_ID)Id, SEN_GET_GAIN_SETTING, (void *)&gain_param);
                    if(rt != E_OK)
                        DBG_ERR("Get sensor information error! (SEN_GET_GAIN_SETTING)\r\n");

    				gGain[Id][(InStrPos[Id] + i + SenInfo.sync_timing + AeIntCtrlInfo[Id].CalSetShiftNum) % AE_TOTAL_PARAMNUM].data1[0] = gain_param.gain_setting.data1[0];
    				gGain[Id][(InStrPos[Id] + i + SenInfo.sync_timing + AeIntCtrlInfo[Id].CalSetShiftNum) % AE_TOTAL_PARAMNUM].data2[0] = gain_param.gain_setting.data2[0];
    				gGain[Id][(InStrPos[Id] + i + SenInfo.sync_timing + AeIntCtrlInfo[Id].CalSetShiftNum) % AE_TOTAL_PARAMNUM].data3[0] = gain_param.gain_setting.data3[0];
    			}

    			if (iq_get_ui_info((IPL_PROC_ID)Id, IQ_UI_IRCUT) == IQ_IRCUT_AUTO) {
    				AE_SetIRCut(Id, AeInfo[Id].PrvAvgEV.LV);
    			}
 		    }
		}
	} else if (AEStep[Id] == 1) {
		if (AEUartMsg == ENABLE || uiAEUartCmd[Id] == ENABLE) {
			AE_getStatisticInfo(&AeInfo[Id], &AeBlkInfo[Id]);
		}
	} else if (AEStep[Id] == 2) {
		if (AEUartMsg == ENABLE || uiAEUartCmd[Id] == ENABLE) {
			AE_getStatisticInfo(&AeInfo[Id], &AeBlkInfo[Id]);
		}
	}

    // dciris operation
    if (ae_param_info[Id]->iris_cfg->enable == ENABLE) {
        AeIrisInfo[Id].conv_l = AeInfo[Id].CurExpectLum.y * (100 - ae_param_info[Id]->convergence->range_conv.l) / 100;
        AeIrisInfo[Id].conv_h = AeInfo[Id].CurExpectLum.y * (100 + ae_param_info[Id]->convergence->range_conv.h) / 100;
        AeIrisInfo[Id].expo_time = PrvNewAeArg[Id][0].ExpoTime;
        AeIrisInfo[Id].iso_gain = PrvNewAeArg[Id][0].ISOGain;
        ae_do_dciris(&AeInfo[Id], &AeIrisInfo[Id]);
        ae_dciris_ctrl(Id, AeIrisInfo[Id].drv_ratio, AeIrisInfo[Id].drv_max);
    } else {
        if (AeIrisInfo[Id].pwm_id > 0) {
            ae_dciris_ctrl(Id, 0, AeIrisInfo[Id].drv_max);
        }
    }

    if ((ae_param_info[Id]->iris_cfg->enable == DISABLE)||(AeIrisInfo[Id].exposure_freeze == 0)) {
    	AE_setFlag(Id, AE_FLAG_SET_GAIN | AE_FLAG_SET_EXPT);
    	AE_setExpT(Id, &gExpTSetting[Id][InStrPos[Id] + AEStep[Id]]);
        AE_setGain(Id, &gGain[Id][InStrPos[Id] + AEStep[Id]]);
    }

	AE_SetSyncInfo(Id, &PrvNewAeArg[Id][AEStep[Id]]);

	iq_set_flow(Id, IQS_AE_END);

	if (AEStep[Id] == 0) {
		//Trigger SHDR Process here
		AE_DRC_NotifyEvent(IPL_UTI_CONV2_AE_ID(Id));
	}

	if (AEUartMsg == ENABLE || uiAEUartCmd[Id] > AE_UART_OFF) {
        AE_UartProc(Id);
	}

    ae_status_info[Id].lv = AeInfo[Id].PrvAvgEV.LV;
    ae_status_info[Id].hist_lum = AeInfo[Id].ExtSet.AdjHistoInfo.Lum.y;
    ae_status_info[Id].overexp_adj = AeInfo[Id].ExtSet.OverExp.CurTotalStep;
    ae_status_info[Id].lum = AeInfo[Id].PrvAvgLum.y;
    ae_status_info[Id].expect_lum = AeInfo[Id].CurExpectLum.y;
    ae_status_info[Id].expotime = PrvNewAeArg[Id][AEStep[Id]].ExpoTime;
    ae_status_info[Id].iso_gain = PrvNewAeArg[Id][AEStep[Id]].ISOGain;
    ae_status_info[Id].ev = AeInfo[Id].PrvAvgEV.EVValue;

    ae_param_transfer_tmp(Id);

	AEStep[Id] = (AEStep[Id] + 1) % AeIntCtrlInfo[Id].AEPeroid; //AE Peroid
}

void AE_GetLA(UINT32 Id, UINT16 *Lum1, UINT16 *Lum2, UINT16 *Histo)
{
	IPL_LA_INFOR LAInfo;

	LAInfo.id = Id;
	LAInfo.lum1 = Lum1;
	LAInfo.lum2 = Lum2;
	LAInfo.histo = Histo;
	LAInfo.pnext = NULL;

	IPL_GetCmd(IPL_GET_LA_CUR_INFOR, (void *)&LAInfo);
}

//#NT#2016/10/12#Jarkko Chang -begin
//#NT# avoid IPL_ID_3/IPL_ID_4 call ae api
void AE_GetPrvAEArg(UINT32 Id, UINT32 *ISO, UINT32 *ExpT, UINT32 *Iris)
{
	if (Id >= AE_ID_MAX_NUM) {
		*ExpT = 0;
		*ISO = 0;
		*Iris = 0;
	} else {
		*ExpT = PrvNewAeArg[Id][0].ExpoTime;
		*ISO = PrvNewAeArg[Id][0].ISOGain;
		*Iris = PrvNewAeArg[Id][0].Iris;
	}
}

void AE_SetPrvAEArg(UINT32 Id)
{
	UINT32 prv_iso[SEN_MFRAME_MAX_NUM] = {0},prv_expt[SEN_MFRAME_MAX_NUM] = {0};
	GAIN_PARAM gain_param = {0};
	EXPOSURE_PARAM exp_param = {0};
	ER rt = E_OK;

    if (Id >= AE_ID_MAX_NUM) {
        DBG_ERR("Capture Id Error (%d)\r\n", Id);
        return;
    } else {
		if (uiExpoTime[Id] == 0) {
			prv_expt[0] = AE_AlgGetUIInfo((AE_PROC_ID)Id, AE_SEL_EXPOTIME);
			prv_iso[0] = AE_AlgGetUIInfo((AE_PROC_ID)Id, AE_SEL_ISOGAIN);
			//iris[0] = CapNewAeArg[Id].Iris;
		} else {
			prv_expt[0] = uiExpoTime[Id];
			prv_iso[0] = uiISOGain[Id];
			//iris[0] = CapNewAeArg[Id].Iris;
		}
    }
    //DBG_ERR("%d, %d\r\n", prv_expt[0], prv_iso[0]);
    //get exposure
    exp_param.sensor_mode = SENSOR_MODE_CUR;
	exp_param.exp_time[0] = prv_expt[0];
	rt = sensor_get_info(IPL_UTI_CONV2_SEN_ID(Id), SEN_GET_EXPO_SETTING, (void *)&exp_param);

    if(rt != E_OK)
        DBG_ERR("Get sensor information error! (SEN_GET_EXPO_SETTING)\r\n");

	//get gain
	prv_iso[0] = prv_iso[0]*exp_param.compensate_ratio[0]/100;
	gain_param.gain_ratio[0] = (prv_iso[0]*AeParamInfo[Id].BaseGainRatio) / AeParamInfo[Id].BaseISO;
	gain_param.sensor_mode = SENSOR_MODE_CUR;
	rt = sensor_get_info(IPL_UTI_CONV2_SEN_ID(Id), SEN_GET_GAIN_SETTING, (void *)&gain_param);

    if(rt != E_OK)
        DBG_ERR("Get sensor information error! (SEN_GET_GAIN_SETTING)\r\n");

	rt = sensor_set_info(IPL_UTI_CONV2_SEN_ID(Id), SEN_SET_EXPT, (void *)&exp_param.exp_setting);

    if(rt != E_OK)
        DBG_ERR("Set sensor information error! (SEN_SET_EXPT)\r\n");

	rt = sensor_set_info(IPL_UTI_CONV2_SEN_ID(Id), SEN_SET_GAIN, (void *)&gain_param.gain_setting);

    if(rt != E_OK)
        DBG_ERR("Set sensor information error! (SEN_SET_GAIN)\r\n");
}

void AE_SetCapAEArg(UINT32 Id)
{
	UINT32 cap_iso[SEN_MFRAME_MAX_NUM] = {0},cap_expt[SEN_MFRAME_MAX_NUM] = {0};
    //UINT32 iris[SEN_MFRAME_MAX_NUM] = {0};
	GAIN_PARAM gain_param = {0};
	EXPOSURE_PARAM exp_param = {0};
	ER rt = E_OK;

	if (Id >= AE_ID_MAX_NUM) {
        DBG_ERR("Capture Id Error (%d)\r\n", Id);
        return;
	} else {
		if (uiExpoTime[Id] == 0) {
            if(ae_get_ui_info((IPL_PROC_ID)Id, AE_UI_CAP_LONGEXP) == AE_CAP_LONGEXP_ON) {
                cap_expt[0] = ae_get_ui_info((IPL_PROC_ID)Id, AE_UI_CAP_LONGEXP_EXPT);
                cap_iso[0] = ae_get_ui_info((IPL_PROC_ID)Id, AE_UI_CAP_LONGEXP_ISO);
            } else {
				cap_expt[0] = CapNewAeArg[Id].ExpoTime;
				cap_iso[0] = CapNewAeArg[Id].ISOGain;
            }
			//iris[0] = CapNewAeArg[Id].Iris;
		} else {
			cap_expt[0] = uiExpoTime[Id];
			cap_iso[0] = uiISOGain[Id];
			//iris[0] = CapNewAeArg[Id].Iris;
		}

        DBG_DUMP("Cap AE Info = %d, %d\r\n", cap_expt[0], cap_iso[0]);

		AeCapInfo[Id] = AeInfo[Id];
		PrvLastAeArg[Id][0] = PrvNewAeArg[Id][0];
		PrvLastAeArg[Id][1] = PrvNewAeArg[Id][1];
		PrvLastAeArg[Id][2] = PrvNewAeArg[Id][2];
	}

    //get exposure
    exp_param.sensor_mode = SENSOR_MODE_CUR;
	exp_param.exp_time[0] = cap_expt[0];
	rt = sensor_get_info((SENSOR_ID)Id, SEN_GET_EXPO_SETTING, (void *)&exp_param);
    if(rt != E_OK)
        DBG_ERR("Get sensor information error! (SEN_GET_EXPO_SETTING)\r\n");

	//get gain
	cap_iso[0] = cap_iso[0]*exp_param.compensate_ratio[0]/100;
	gain_param.gain_ratio[0] = (cap_iso[0]*AeParamInfo[Id].BaseGainRatio) / AeParamInfo[Id].BaseISO;
	gain_param.sensor_mode = SENSOR_MODE_CUR;
	rt = sensor_get_info((SENSOR_ID)Id, SEN_GET_GAIN_SETTING, (void *)&gain_param);

    if(rt != E_OK)
        DBG_ERR("Get sensor information error! (SEN_GET_GAIN_SETTING)\r\n");

	rt = sensor_set_info((SENSOR_ID)Id, SEN_SET_EXPT, (void *)&exp_param.exp_setting);

    if(rt != E_OK)
        DBG_ERR("Set sensor information error! (SEN_SET_EXPT)\r\n");

	rt = sensor_set_info((SENSOR_ID)Id, SEN_SET_GAIN, (void *)&gain_param.gain_setting);

    if(rt != E_OK)
        DBG_ERR("Set sensor information error! (SEN_SET_GAIN)\r\n");
}

void AE_SetCapAEArg_MFHDR(UINT32 Id, UINT32 MFHDR_Frm)
{
	UINT32 cap_iso[SEN_MFRAME_MAX_NUM] = {0},cap_expt[SEN_MFRAME_MAX_NUM] = {0};
    //UINT32 iris[SEN_MFRAME_MAX_NUM] = {0};
	GAIN_PARAM gain_param = {0};
	EXPOSURE_PARAM exp_param = {0};
	ER rt = E_OK;
    static UINT32 mfhdr_frm_cnt = 0;
    UINT32 ev_value, mfhdr_ev_gap;

	if (Id >= AE_ID_MAX_NUM) {
        DBG_ERR("Capture Id Error (%d)\r\n", Id);
        return;
	} else {

        mfhdr_ev_gap = ae_get_ui_info((AE_PROC_ID)Id, AE_UI_CAP_MFHDR_EV) + 1;

		if (uiExpoTime[Id] == 0) {
			cap_iso[0] = CapNewAeArg[Id].ISOGain;
			cap_expt[0] = CapNewAeArg[Id].ExpoTime;
			//iris[0] = CapNewAeArg[Id].Iris;
		} else {
			cap_expt[0] = uiExpoTime[Id];
			cap_iso[0] = uiISOGain[Id];
			//iris[0] = CapNewAeArg[Id].Iris;
		}

        if(cap_expt[0] < (ae_param_info[Id]->curve_gen_param->node[0].expt << mfhdr_ev_gap)){
            // For min exposure case
            cap_expt[0] = ae_param_info[Id]->curve_gen_param->node[0].expt;
            ev_value = (cap_expt[0] * cap_iso[0]);
        } else {
            ev_value = ((cap_expt[0] * cap_iso[0]) >> mfhdr_ev_gap);
        }

        ev_value = (ev_value << mfhdr_frm_cnt);
        cap_expt[0] = (ev_value / cap_iso[0]);

        if((mfhdr_frm_cnt+1) >= MFHDR_Frm)
            mfhdr_frm_cnt = 0;
        else
            mfhdr_frm_cnt++;

		AeCapInfo[Id] = AeInfo[Id];
		PrvLastAeArg[Id][0] = PrvNewAeArg[Id][0];
		PrvLastAeArg[Id][1] = PrvNewAeArg[Id][1];
		PrvLastAeArg[Id][2] = PrvNewAeArg[Id][2];
	}

    //get exposure
    exp_param.sensor_mode = SENSOR_MODE_CUR;
	exp_param.exp_time[0] = cap_expt[0];
	rt = sensor_get_info((SENSOR_ID)Id, SEN_GET_EXPO_SETTING, (void *)&exp_param);
    if(rt != E_OK)
        DBG_ERR("Get sensor information error! (SEN_GET_EXPO_SETTING)\r\n");

	//get gain
	cap_iso[0] = cap_iso[0]*exp_param.compensate_ratio[0]/100;
	gain_param.gain_ratio[0] = (cap_iso[0]*AeParamInfo[Id].BaseGainRatio) / AeParamInfo[Id].BaseISO;
	gain_param.sensor_mode = SENSOR_MODE_CUR;
	rt = sensor_get_info((SENSOR_ID)Id, SEN_GET_GAIN_SETTING, (void *)&gain_param);

    if(rt != E_OK)
        DBG_ERR("Get sensor information error! (SEN_GET_GAIN_SETTING)\r\n");

	rt = sensor_set_info((SENSOR_ID)Id, SEN_SET_EXPT, (void *)&exp_param.exp_setting);

    if(rt != E_OK)
        DBG_ERR("Set sensor information error! (SEN_SET_EXPT)\r\n");

	rt = sensor_set_info((SENSOR_ID)Id, SEN_SET_GAIN, (void *)&gain_param.gain_setting);

    if(rt != E_OK)
        DBG_ERR("Set sensor information error! (SEN_SET_GAIN)\r\n");
}

void AE_GetCapAEArg(UINT32 Id, UINT32 *ISO, UINT32 *ExpT, UINT32 *Iris)
{
	if (Id >= AE_ID_MAX_NUM) {
		*ExpT = 0;
		*ISO = 0;
		*Iris = 0;
	} else {
		if (uiExpoTime[Id] == 0) {
			*ExpT = CapNewAeArg[Id].ExpoTime;
			*ISO = CapNewAeArg[Id].ISOGain;
			*Iris = CapNewAeArg[Id].Iris;
		} else {
			*ExpT = uiExpoTime[Id];
			*ISO = uiISOGain[Id];
			*Iris = CapNewAeArg[Id].Iris;
		}

		AeCapInfo[Id] = AeInfo[Id];
		PrvLastAeArg[Id][0] = PrvNewAeArg[Id][0];
		PrvLastAeArg[Id][1] = PrvNewAeArg[Id][1];
		PrvLastAeArg[Id][2] = PrvNewAeArg[Id][2];
	}
}
//#NT#2016/10/12#Jarkko Chang -end

void AE_GetWin(UINT32 Id, UINT8 *win)
{
	UINT32 i, j, idx;

	for (j = 0; j < AE_WIN_Y; j ++) {
		idx = (j * AE_WIN_X);
		for (i = 0; i < AE_WIN_X; i ++) {
			win[idx] = (UINT8)AeWin[j][i];
			idx += 1;
		}
	}
}

void AE_GetStatus(UINT32 Id, AEStatus *pAEStatus)
{
	pAEStatus->ExpT = PrvNewAeArg[Id][AEStep[Id]].ExpoTime;
	pAEStatus->Gain = PrvNewAeArg[Id][AEStep[Id]].ISOGain;
    pAEStatus->GainRatio = (PrvNewAeArg[Id][AEStep[Id]].ISOGain * AeParamInfo[Id].BaseGainRatio) / AeParamInfo[Id].BaseISO;
	pAEStatus->LumaValue = AeBlkInfo[Id].Yacc;
    pAEStatus->RawAverage = AeInfo[Id].PrvAvgLum.raw;
	pAEStatus->LumaAverage = AeInfo[Id].PrvAvgLum.y;
	pAEStatus->LumaHistogram = AeInfo[Id].histogram.y;
}

void AE_WaitStableRF(UINT32 Id, UINT32 StableCnt)
{
	UINT32 TimeOutCnt = 0;
	UINT32 TimeOutFrameCnt = 100;//0xfffff;

	if (StableCnt == 0) {
		StableCnt = TimeOutFrameCnt;
	}

	while (TimeOutCnt < TimeOutFrameCnt) {
		if (AeInfo[Id].CounterRF > StableCnt) {
			return;
		} else {
			TimeOutCnt ++;
		}

		Delay_DelayMs(3);
	}

	DBG_MSG("AE Stable Time Out but not stable %d\r\n", AeInfo[Id].CounterRF);
}

UINT32 AE_WriteDebugInfo(UINT32 Id, UINT32 Addr)
{
//    UINT32 i,j;
	UINT32 buf_cnt;

	buf_cnt = snprintf(NULL, 0, "\r\n\r\n\r\nAE Info------------------------------------------------------------------------------------------------------------------------\r\n");
	Addr += snprintf((char *)Addr, buf_cnt + 1, "\r\n\r\n\r\nAE Info------------------------------------------------------------------------------------------------------------------------\r\n");

	buf_cnt = snprintf(NULL, 0,  "ExpectY = \t%ld\r\nAdd_Y = \t%ld\r\nLumY =  \t%ld\r\nStableCnt = \t%ld\r\n",
					   AeCapInfo[Id].CurExpectLum.y, AeCapInfo[Id].ExtSet.OverExp.CurTotalStep, AeCapInfo[Id].CurLum.y, AeCapInfo[Id].Counter);
	Addr += snprintf((char *)Addr, buf_cnt + 1, "ExpectY = \t%ld\r\nAdd_Y = \t%ld\r\nLumY =  \t%ld\r\nStableCnt = \t%ld\r\n",
					 AeCapInfo[Id].CurExpectLum.y, AeCapInfo[Id].ExtSet.OverExp.CurTotalStep, AeCapInfo[Id].CurLum.y, AeCapInfo[Id].Counter);

	buf_cnt = snprintf(NULL, 0, "PrvEV_Value = \t%ld\r\nPrvLV = \t%ld\r\n",
					   AeCapInfo[Id].PrvAvgEV.EVValue, AeCapInfo[Id].PrvAvgEV.LV);
	Addr += snprintf((char *)Addr, buf_cnt + 1, "PrvEV_Value = \t%ld\r\nPrvLV = \t%ld\r\n",
					 AeCapInfo[Id].PrvAvgEV.EVValue, AeCapInfo[Id].PrvAvgEV.LV);

	buf_cnt = snprintf(NULL, 0, "PrvEV_Value = \t%ld\r\nPrvLV = \t%ld\r\n",
					   AeCapInfo[Id].PrvParam[0].EVValue, AeCapInfo[Id].PrvParam[0].LV);
	Addr += snprintf((char *)Addr, buf_cnt + 1, "PrvEV_Value = \t%ld\r\nPrvLV = \t%ld\r\n",
					 AeCapInfo[Id].PrvParam[0].EVValue, AeCapInfo[Id].PrvParam[0].LV);

	buf_cnt = snprintf(NULL, 0, "PrvEV_Value = \t%ld\r\nPrvLV = \t%ld\r\n",
					   AeCapInfo[Id].PrvParam[1].EVValue, AeCapInfo[Id].PrvParam[1].LV);
	Addr += snprintf((char *)Addr, buf_cnt + 1, "PrvEV_Value = \t%ld\r\nPrvLV = \t%ld\r\n",
					 AeCapInfo[Id].PrvParam[1].EVValue, AeCapInfo[Id].PrvParam[1].LV);

	buf_cnt = snprintf(NULL, 0, "PrvEV_Value = \t%ld\r\nPrvLV = \t%ld\r\n",
					   AeCapInfo[Id].PrvParam[2].EVValue, AeCapInfo[Id].PrvParam[2].LV);
	Addr += snprintf((char *)Addr, buf_cnt + 1, "PrvEV_Value = \t%ld\r\nPrvLV = \t%ld\r\n",
					 AeCapInfo[Id].PrvParam[2].EVValue, AeCapInfo[Id].PrvParam[2].LV);

	buf_cnt = snprintf(NULL, 0, "CapEV_Value = \t%ld\r\nCapLV = \t%ld\r\n",
					   AeCapInfo[Id].CapParam.EVValue, AeCapInfo[Id].CapParam.LV);
	Addr += snprintf((char *)Addr, buf_cnt + 1, "CapEV_Value = \t%ld\r\nCapLV = \t%ld\r\n",
					 AeCapInfo[Id].CapParam.EVValue, AeCapInfo[Id].CapParam.LV);

	buf_cnt = snprintf(NULL, 0, "Prv_ISOGain = \t%ld\r\nPrv_ExpoTime = \t%ld\r\nPrv_Iris = \t%ld\r\n",
					   PrvLastAeArg[Id][0].ISOGain, PrvLastAeArg[Id][0].ExpoTime, PrvLastAeArg[Id][0].Iris);
	Addr += snprintf((char *)Addr, buf_cnt + 1, "Prv_ISOGain = \t%ld\r\nPrv_ExpoTime = \t%ld\r\nPrv_Iris = \t%ld\r\n",
					 PrvLastAeArg[Id][0].ISOGain, PrvLastAeArg[Id][0].ExpoTime, PrvLastAeArg[Id][0].Iris);

	buf_cnt = snprintf(NULL, 0, "Prv_ISOGain = \t%ld\r\nPrv_ExpoTime = \t%ld\r\nPrv_Iris = \t%ld\r\n",
					   PrvLastAeArg[Id][1].ISOGain, PrvLastAeArg[Id][1].ExpoTime, PrvLastAeArg[Id][1].Iris);
	Addr += snprintf((char *)Addr, buf_cnt + 1, "Prv_ISOGain = \t%ld\r\nPrv_ExpoTime = \t%ld\r\nPrv_Iris = \t%ld\r\n",
					 PrvLastAeArg[Id][1].ISOGain, PrvLastAeArg[Id][1].ExpoTime, PrvLastAeArg[Id][1].Iris);

	buf_cnt = snprintf(NULL, 0, "Prv_ISOGain = \t%ld\r\nPrv_ExpoTime = \t%ld\r\nPrv_Iris = \t%ld\r\n",
					   PrvLastAeArg[Id][2].ISOGain, PrvLastAeArg[Id][2].ExpoTime, PrvLastAeArg[Id][2].Iris);
	Addr += snprintf((char *)Addr, buf_cnt + 1, "Prv_ISOGain = \t%ld\r\nPrv_ExpoTime = \t%ld\r\nPrv_Iris = \t%ld\r\n",
					 PrvLastAeArg[Id][2].ISOGain, PrvLastAeArg[Id][2].ExpoTime, PrvLastAeArg[Id][2].Iris);

	buf_cnt = snprintf(NULL, 0, "Cap_ISOGain = \t%ld\r\nCap_ExpoTime = \t%ld\r\nCap_Iris = \t%ld\r\n",
					   CapNewAeArg[Id].ISOGain, CapNewAeArg[Id].ExpoTime, CapNewAeArg[Id].Iris);
	Addr += snprintf((char *)Addr, buf_cnt + 1, "Cap_ISOGain = \t%ld\r\nCap_ExpoTime = \t%ld\r\nCap_Iris = \t%ld\r\n",
					 CapNewAeArg[Id].ISOGain, CapNewAeArg[Id].ExpoTime, CapNewAeArg[Id].Iris);

	buf_cnt = snprintf(NULL, 0, "UI setting: Metermode=%d, ISO=%d, EV bios = %d, Flash=%d, Scene=%d, Freq=%d, ImgEffect=%d, WB=%d, WDR=%d, SHDR=%d\r\n",
					   ae_get_ui_info((IPL_PROC_ID)Id, AE_UI_METER), ae_get_ui_info((IPL_PROC_ID)Id, AE_UI_ISO), ae_get_ui_info(Id, AE_UI_EV), ae_get_ui_info((IPL_PROC_ID)Id, AE_UI_FLASH)
					   , ae_get_ui_info((IPL_PROC_ID)Id, AE_UI_SCENE), ae_get_ui_info((IPL_PROC_ID)Id, AE_UI_FREQUENCY), iq_get_ui_info((IPL_PROC_ID)Id, IQ_UI_IMAGEEFFECT), awb_get_ui_info((IPL_PROC_ID)Id, AWB_UI_WB), IPL_AlgGetUIInfo((IPL_PROC_ID)Id, IPL_SEL_WDR_IMM), IPL_AlgGetUIInfo((IPL_PROC_ID)Id, IPL_SEL_SHDR));
	Addr += snprintf((char *)Addr, buf_cnt + 1, "UI setting: Metermode=%d, ISO=%d, EV bios = %d, Flash=%d, Scene=%d, Freq=%d, ImgEffect=%d, WB=%d, WDR=%d, SHDR=%d\r\n",
					 ae_get_ui_info((IPL_PROC_ID)Id, AE_UI_METER), ae_get_ui_info((IPL_PROC_ID)Id, AE_UI_ISO), ae_get_ui_info((IPL_PROC_ID)Id, AE_UI_EV), ae_get_ui_info((IPL_PROC_ID)Id, AE_UI_FLASH)
					 , ae_get_ui_info((IPL_PROC_ID)Id, AE_UI_SCENE), ae_get_ui_info((IPL_PROC_ID)Id, AE_UI_FREQUENCY), iq_get_ui_info((IPL_PROC_ID)Id, IQ_UI_IMAGEEFFECT), awb_get_ui_info((IPL_PROC_ID)Id, AWB_UI_WB), IPL_AlgGetUIInfo((IPL_PROC_ID)Id, IPL_SEL_WDR_IMM), IPL_AlgGetUIInfo((IPL_PROC_ID)Id, IPL_SEL_SHDR));

	Addr += snprintf((char *)Addr, 256, "------------------------------\r\n");
	Addr = AE_BlkWriteDebugInfo(Addr);
	Addr += snprintf((char *)Addr, 256, "------------------------------\r\n");
#if 0
	for (i = 0; i < AE_HIST_NUM; i++) {
		Addr += snprintf((char *)Addr, sizeof(AeInfo[Id]), "%d\r\n", AeInfo[Id].histogram.raw[i]);
	}
	Addr += snprintf((char *)Addr, sizeof(AeInfo[Id]), "\r\n");

	for (i = 0; i < AE_HIST_NUM; i++) {
		Addr += snprintf((char *)Addr, sizeof(AeInfo[Id]), "%d\r\n", AeInfo[Id].histogram.y[i]);
	}
#endif

//#NT#2016/11/01#Jarkko Chang -begin
//#NT# reduce unused code
#if 0
	for (j = 0; j < LA_WIN_Y; j++) {
		for (i = 0; i < LA_WIN_X; i++) {
			Addr += snprintf((char *)Addr, sizeof(AeBlkInfo[Id]), "%d\t", AeBlkInfo[Id].Gacc[j * LA_WIN_X + i] >> 4);
		}
		Addr += snprintf((char *)Addr, sizeof(AeInfo[Id]), "\r\n");
	}
#endif
//#NT#2016/11/01#Jarkko Chang -end
	return Addr;
}

UINT32 AE_GetPrvLVValue(UINT32 Id)
{
	return AeInfo[Id].PrvParam[0].LV;
}

UINT32 AE_GetCapLVValue(UINT32 Id)
{
	return AeInfo[Id].CapParam.LV;
}

UINT32 AE_GetPrvEVValue(UINT32 Id)
{
	return AeInfo[Id].PrvAvgEV.EVValue;
}

UINT32 AE_GetCapEVValue(UINT32 Id)
{
	return AeInfo[Id].CapParam.EVValue;
}

void AE_GetLAInfo(UINT32 Id, AE_LAINFO *LAInfo)
{
	LAInfo->WinNumX = LA_WIN_X;
	LAInfo->WinNumY = LA_WIN_Y;
}

UINT32 AE_GetCurRawLum(UINT32 Id, UINT32 LumBit)
{
	UINT32 Lum;

	if (LumBit < 12) {
		Lum = AeInfo[Id].PrvAvgLum.raw >> (12 - LumBit);
	} else {
		Lum = AeInfo[Id].PrvAvgLum.raw << (LumBit - 12);
	}

	return Lum;
}

BOOL AE_GetIRCutEnable(UINT32 Id)
{
	return IR_CUT_ENABLE[Id];
}

AE_BOUNDARY AE_GetLVBoundary(UINT32 Id)
{
	AE_BOUNDARY Lv;

	Lv.h = AEAlg_Conv2LV(AeInfo[Id].EV.h);
	Lv.l = AEAlg_Conv2LV(AeInfo[Id].EV.l);

	return Lv;
}

UINT32 AE_GetPrvOverExpAdjValue(UINT32 Id)
{
	return AeInfo[Id].ExtSet.OverExp.CurTotalStep;
}

UINT32 AE_Check_Fno(UINT32 Id, UINT32 fno_rat)
{
    UINT64 tmp, fno_value;

	tmp = 1000000;
	tmp = (tmp * 100);
    tmp = (tmp * AEALG_EV_ACCURACY_BASE);
	tmp = (tmp * fno_rat);

	tmp = tmp / ae_param_info[Id]->curve_gen_param->node[0].expt;
	tmp = tmp / ae_param_info[Id]->curve_gen_param->node[0].iso;

	if (tmp > 0xFFFFFFFF) {
		DBG_ERR("cal ev overflow! (%d, %d, %d, 0x%l0x)\r\n", ae_param_info[Id]->curve_gen_param->node[0].expt, ae_param_info[Id]->curve_gen_param->node[0].iso, AeCurveGenInfo[Id].fno_rat, tmp);
        tmp = 0xFFFFFFFF;
        tmp *= (UINT64)ae_param_info[Id]->curve_gen_param->node[0].expt;
        tmp *= (UINT64)ae_param_info[Id]->curve_gen_param->node[0].iso;
        tmp /= (UINT64)(1000000);
        tmp /= (UINT64)(100);
        tmp /= (UINT64)(AEALG_EV_ACCURACY_BASE);
        fno_value = (UINT32)tmp;

        DBG_ERR("force change fno_rat to auto calculate value = %d\r\n", fno_value);
	} else {
        fno_value = fno_rat;
	}

    return fno_value;
}

void AE_Check_EVChange(UINT32 Id)
{
	IPL_MODE_INFOR Info = {0};
    ER rt = E_OK;

	Info.id = Id;
	rt = IPL_GetCmd(IPL_GET_CUR_MODE, (void *)&Info);

    if(rt == E_OK) {
        if(Info.mode == IPL_MODE_PREVIEW){
            if((AeInfo[Id].EV.l != AeInfo[Id].PhotoEV.l)||(AeInfo[Id].EV.h != AeInfo[Id].PhotoEV.h)){
                AeInfo[Id].EV.l = AeInfo[Id].PhotoEV.l;
                AeInfo[Id].EV.h = AeInfo[Id].PhotoEV.h;
            }
        } else {
            if((AeInfo[Id].EV.l != AeInfo[Id].MovieEV.l)||(AeInfo[Id].EV.h != AeInfo[Id].MovieEV.h)){
                AeInfo[Id].EV.l = AeInfo[Id].MovieEV.l;
                AeInfo[Id].EV.h = AeInfo[Id].MovieEV.h;
            }
        }
    } else {
        DBG_ERR("Get IPL mode error!\r\n");
    }
}
#if 0
#endif
//------------------------------------------------------------------------------
UINT32 AE_judgePrvSetting(AEALG_ARG *CurInfo, AEALG_ARG *PrevInfo, UINT32 *Flag)
{
	UINT32 JudgeFlag = FALSE;

	*Flag = 0;

	if (PrevInfo->ExpoTime != CurInfo->ExpoTime) {
		*Flag |= AE_FLAG_SET_EXPT;
		JudgeFlag = TRUE;
	}

	if (PrevInfo->ISOGain != CurInfo->ISOGain) {
		*Flag |= AE_FLAG_SET_GAIN;
		JudgeFlag = TRUE;
	}

	if (PrevInfo->Iris != CurInfo->Iris) {
		*Flag |= AE_FLAG_SET_IRIS;
		JudgeFlag = TRUE;
	}

	return JudgeFlag;
}

UINT32 AE_getStatisticInfo(AE_INFO *AeInfo, AE_BLK_INFO *AeBlkInfo)
{
	UINT32 i, j;
	UINT32 GTotal, YTotal, WTotal;
    UINT32 LA_Total;
	UINT32 Ratio_X, Ratio_Y;
    UINT32 lv0, lv1;
    UINT32 idx0, idx1;
    UINT32 clamp_y_l, clamp_y_h;
    UINT32 clamp_raw_l, clamp_raw_h;
    UINT32 clamp0, clamp1;
    UINT32 curr_lv, curr_expt_y, curr_expt_raw;
    UINT32 lum_raw, lum_y;
    UINT32 la_raw, la_y;
    UINT32 la_long_exp_id, la_short_exp_id;
    UINT32 ipl_mf_bit;
    UINT32 is_skip_clamp = FALSE;

	//initial
	LA_Total = 0;
	GTotal = 0;
	WTotal = 0;
	YTotal = 0;
	memset(Blk_LA_Data, 0, AE_WIN_X * AE_WIN_Y * 2);
    memset(AeBlkInfo->Yacc, 0, LA_WIN_X * LA_WIN_Y * 2);
	memset(AeBlkInfo->Gacc, 0, LA_WIN_X * LA_WIN_Y * 2);
	memset(AeInfo->histogram.y, 0, AE_HIST_NUM * 2);
	memset(weighting_histogram.y, 0, AE_HIST_NUM * 2);

	Ratio_X = LA_WIN_X / AE_WIN_X;
	Ratio_Y = LA_WIN_Y / AE_WIN_Y;

    curr_lv = AeInfo->PrvAvgEV.LV;
    curr_expt_y = AeInfo->CurExpectLum.y;
    curr_expt_raw = AeInfo->CurExpectLum.raw;

	idx0 = curr_lv / AEALG_LV_ACCURACY_BASE;
	idx1 = idx0 + 1;

	lv0 = (idx0 * AEALG_LV_ACCURACY_BASE);
	lv1 = (idx1 * AEALG_LV_ACCURACY_BASE);

	if (idx0 >= (AEALG_DYNAMIC_LV_NUM - 1)) {
		idx1 = idx0;
	}

    if (IPL_AlgGetUIInfo((IPL_PROC_ID)AeInfo->Id, IPL_SEL_SHDR) == SEL_SHDR_ON){
        clamp0 = ae_param_info[AeInfo->Id]->la_clamp->tab_shdr_l[idx0];
        clamp1 = ae_param_info[AeInfo->Id]->la_clamp->tab_shdr_l[idx1];

        // Find clamp ratio lower boundary
    	if (clamp0 > clamp1) {
    		clamp_y_l = clamp1 + (((clamp0 - clamp1) * (lv1 - curr_lv)) / (lv1 - lv0));
    	} else {
    		clamp_y_l = clamp0 + (((clamp1 - clamp0) * (curr_lv - lv0)) / (lv1 - lv0));
    	}

        // Calculate clamp lower boundary use expect lum
        clamp_y_l = ((curr_expt_y * clamp_y_l)/100);

        clamp0 = ae_param_info[AeInfo->Id]->la_clamp->tab_shdr_h[idx0];
        clamp1 = ae_param_info[AeInfo->Id]->la_clamp->tab_shdr_h[idx1];

        // Find clamp ratio upper boundary
    	if (clamp0 > clamp1) {
    		clamp_y_h = clamp1 + (((clamp0 - clamp1) * (lv1 - curr_lv)) / (lv1 - lv0));
    	} else {
    		clamp_y_h = clamp0 + (((clamp1 - clamp0) * (curr_lv - lv0)) / (lv1 - lv0));
    	}

        // Calculate clamp upper boundary use expect lum
        clamp_y_h = ((curr_expt_y * clamp_y_h)/100);

        //DBG_DUMP("%d, %d, %d, %d\r\n", curr_lv, curr_expt_y, clamp_l, clamp_h);
    } else {

        clamp0 = ae_param_info[AeInfo->Id]->la_clamp->tab_normal_l[idx0];
        clamp1 = ae_param_info[AeInfo->Id]->la_clamp->tab_normal_l[idx1];

    	if (clamp0 > clamp1) {
    		clamp_y_l = clamp1 + (((clamp0 - clamp1) * (lv1 - curr_lv)) / (lv1 - lv0));
    	} else {
    		clamp_y_l = clamp0 + (((clamp1 - clamp0) * (curr_lv - lv0)) / (lv1 - lv0));
    	}

        clamp0 = ae_param_info[AeInfo->Id]->la_clamp->tab_normal_h[idx0];
        clamp1 = ae_param_info[AeInfo->Id]->la_clamp->tab_normal_h[idx1];

    	if (clamp0 > clamp1) {
    		clamp_y_h = clamp1 + (((clamp0 - clamp1) * (lv1 - curr_lv)) / (lv1 - lv0));
    	} else {
    		clamp_y_h = clamp0 + (((clamp1 - clamp0) * (curr_lv - lv0)) / (lv1 - lv0));
    	}

        //DBG_DUMP("%d, %d, %d, %d, %d\r\n", curr_lv, idx0, idx1, clamp_l, clamp_h);
    }

    clamp_raw_l = AeInfo->InvGammaFunc(clamp_y_l);
    clamp_raw_h = AeInfo->InvGammaFunc(clamp_y_h);

    // For avoid low expy setting issue
    if((clamp_raw_h < curr_expt_raw)||(clamp_y_h < curr_expt_y)||(clamp_raw_l > curr_expt_raw)||(clamp_y_l > curr_expt_y))
        is_skip_clamp = TRUE;
    else
        is_skip_clamp = FALSE;

    UINT32 HistIntervalY = (1 << 10) / weighting_histogram.bin_num;
	weighting_histogram.data_num = 0;

    la_long_exp_id = AeInfo->Id;
    la_short_exp_id = AeInfo->Id;
    ipl_mf_bit = IPL_CtrlGetInfor(AeInfo->Id, IPLCTRL_MF_TOTAL_FRM_BIT);
    for(i=0; i<IPL_CtrlGetInfor(AeInfo->Id, IPLCTRL_MF_TOTAL_FRM_NUM); i++){
        if(((ipl_mf_bit>>i)&0x01)!=0x0){
            la_long_exp_id = i;
            break;
        }
    }
    for(i=IPL_CtrlGetInfor(AeInfo->Id, IPLCTRL_MF_TOTAL_FRM_NUM)-1; i>0; i--){
        if(((ipl_mf_bit>>i)&0x01)!=0x0){
            la_short_exp_id = i;
            break;
        }
    }

    //la_ref_id = la_long_exp_id;
    UINT32 la_se_sum = 0;
    UINT32 la_se_conv_l, la_se_conv_h;
    UINT32 ratio, hdr_ratio;

    if((IPL_AlgGetUIInfo((IPL_PROC_ID)AeInfo->Id, IPL_SEL_SHDR) == SEL_SHDR_ON)&&(ae_param_info[AeInfo->Id]->shdr->ev_opt == AE_SHDR_EV_OPT_EXPY)&&(AEStep[AeInfo->Id] == 0)) {
    //if(IPL_AlgGetUIInfo(AeInfo->Id, IPL_SEL_SHDR) == SEL_SHDR_ON) {
		AE_GetLA(la_short_exp_id, AeBlkInfo->Gacc, AeBlkInfo->Yacc, AeInfo->histogram.y);
		for (j = 0; j < LA_WIN_Y; j ++) {
			for (i = 0; i < LA_WIN_X; i ++) {
                if(AeBlkInfo->Gacc[j * LA_WIN_X + i] < ae_param_info[AeInfo->Id]->shdr->clamp.l)
                    la_se_sum += ae_param_info[AeInfo->Id]->shdr->clamp.l;
                else if(AeBlkInfo->Gacc[j * LA_WIN_X + i] > ae_param_info[AeInfo->Id]->shdr->clamp.h)
                    la_se_sum += ae_param_info[AeInfo->Id]->shdr->clamp.h;
                else
                    la_se_sum += AeBlkInfo->Gacc[j * LA_WIN_X + i];
			}
		}

        la_se_conv_l = (ae_param_info[AeInfo->Id]->shdr->expy_se*(100-ae_param_info[AeInfo->Id]->shdr->range_conv.l))/100;
        la_se_conv_h = (ae_param_info[AeInfo->Id]->shdr->expy_se*(100+ae_param_info[AeInfo->Id]->shdr->range_conv.h))/100;

        la_se_sum = (la_se_sum+(LA_WIN_NUM>>1))/LA_WIN_NUM;
        ae_shdr_info[AeInfo->Id].curr_se_lum = la_se_sum;

        if((la_se_sum > la_se_conv_h)||(la_se_sum < la_se_conv_l)) {
            ratio = (la_se_sum*1000)/ae_param_info[AeInfo->Id]->shdr->expy_se;
            hdr_ratio = (ae_shdr_info[AeInfo->Id].se_ev_ratio*ratio)/1000;
            ae_shdr_info[AeInfo->Id].se_ev_ratio = ((hdr_ratio*ae_param_info[AeInfo->Id]->shdr->smooth)+(ae_shdr_info[AeInfo->Id].se_ev_ratio*(100-ae_param_info[AeInfo->Id]->shdr->smooth)))/100;
            ae_shdr_info[AeInfo->Id].se_ev_ratio = ae_clamp(ae_shdr_info[AeInfo->Id].se_ev_ratio, ae_param_info[AeInfo->Id]->shdr->range_ev.l, ae_param_info[AeInfo->Id]->shdr->range_ev.h);
        }
    }

    AE_GetLA(la_long_exp_id, AeBlkInfo->Gacc, AeBlkInfo->Yacc, AeInfo->histogram.y);

	for (j = 0; j < LA_WIN_Y; j ++) {
		for (i = 0; i < LA_WIN_X; i ++) {

            la_raw = AeBlkInfo->Gacc[j * LA_WIN_X + i];

            if(is_skip_clamp==TRUE)
                lum_raw = la_raw;
            else
                lum_raw = ae_clamp(la_raw, clamp_raw_l, clamp_raw_h);

            if(ae_param_info[AeInfo->Id]->lum_gamma->type==AE_LUMGAMMA_IPP) {
                la_y = AeBlkInfo->Yacc[j * LA_WIN_X + i];
            }
            else {
                la_y = AE_Get_LumGammaValue(AeInfo->Id, la_raw);
            }

            if(is_skip_clamp==TRUE)
                lum_y = la_y;
            else
                lum_y = ae_clamp(la_y, clamp_y_l, clamp_y_h);

            Blk_LA_Data[AeInfo->Id][(j / Ratio_Y)*AE_WIN_X + (i / Ratio_X)] += lum_y;

			GTotal += (AeWin[j / Ratio_Y][i / Ratio_X] * lum_raw);
			YTotal += (AeWin[j / Ratio_Y][i / Ratio_X] * lum_y);
			WTotal += AeWin[j / Ratio_Y][i / Ratio_X];
            LA_Total += la_y;
			weighting_histogram.y[la_y / HistIntervalY] += AE_OverExpo_Win_Normal[j / Ratio_Y][i / Ratio_X];
			weighting_histogram.data_num += (AE_OverExpo_Win_Normal[j / Ratio_Y][i / Ratio_X]);
		}
	}

	for (i = 0; i < AE_WIN_NUM; i ++)
        Blk_LA_Data[AeInfo->Id][i] = (Blk_LA_Data[AeInfo->Id][i]/(Ratio_X*Ratio_Y));

	if (AeBlkInfo->BLKLum_En == ENABLE) {
		AeBlkInfo->LV = AeInfo->PrvParam[0].LV;
		AeInfo->PrvAvgLum = AEAlg_GetBLKBasedLum(AeBlkInfo);
	} else {
		AeInfo->PrvAvgLum.y = (YTotal / WTotal);
		AeInfo->PrvAvgLum.raw = (GTotal / WTotal);
        AeInfo->LA_Avg_Y = (LA_Total / (LA_WIN_X * LA_WIN_Y));
	}

	return 0;
}

void AE_getCtrlInfo(AE_INFO *AeInfo, AE_BLK_INFO *AeBlkInfo)
{
	static IPL_MODE IplMode[AE_ID_MAX_NUM] = {IPL_MODE_NONE};
	//static AE_APPMODE AppMode = AE_APPMODE_END;
    static IQ_IRCUT IRCutMode = IQ_IRCUT_MAX_CNT;
	IPL_MODE_INFOR Info = {0};
	AE_INT_CTRL_INFO AeCtrlInfo = {AE_NOR_CAL_SET_SHIFT, AE_NOR_PREDICT_POINT, AE_NOR_PEROID};
	UINT32 frame_rate;
    UINT32 ircut_mode;
	ER rt = E_OK;

	Info.id = AeInfo->Id;
	rt = IPL_GetCmd(IPL_GET_CUR_MODE, (void *)&Info);
	if (rt != E_OK) {
		return;
	}

	AeInfo->ConvSpeed = ae_param_info[AeInfo->Id]->convergence->speed;
	AeInfo->StableRange = ae_param_info[AeInfo->Id]->convergence->range_conv;
	AeInfo->ExtSet.UnStableDelay = ae_param_info[AeInfo->Id]->convergence->delay_cnt;
	AeInfo->ExtSet.AntiOverExpEn = ae_param_info[AeInfo->Id]->over_exposure->enable;
    AeInfo->SlowShooting_Cyc = ae_param_info[AeInfo->Id]->convergence->slowshoot_cyc;
    AeInfo->SlowShooting_Thr = ae_param_info[AeInfo->Id]->convergence->slowshoot_thr;

    ircut_mode = iq_get_ui_info(AeInfo->Id, IQ_UI_IRCUT);

	if (IplMode[AeInfo->Id] == Info.mode && IRCutMode == ircut_mode) {
		return;
	}

	if(IPL_AlgGetUIInfo((IPL_PROC_ID)AeInfo->Id, IPL_SEL_SHDR) == SEL_SHDR_ON){
		AeInfo->ExtSet.AdjHistoEn = DISABLE;
		AeBlkInfo->BLKLum_En = DISABLE;

		if ((Info.mode == IPL_MODE_PREVIEW) || (Info.mode == IPL_MODE_CAP)) {
			AeInfo->ExtSet.PrvLumRatioTab = NULL;
			AeInfo->PrvExpectLum.y = ae_param_info[AeInfo->Id]->expect_lum->lum_photo;
			AeInfo->PrvExpectLum.raw = AeInfo->InvGammaFunc(AeInfo->PrvExpectLum.y);
            //AeInfo->PrvExpectLum.raw = AeInfo->PrvExpectLum.y;
            AeInfo->ExtSet.PrvLumRatioTab = ae_param_info[AeInfo->Id]->expect_lum->tab_ratio_photo;
            AeInfo->ExtSet.OverExp.AdjLumTab = ae_param_info[AeInfo->Id]->over_exposure->tab_thr_photo;
		} else {
			AeInfo->ExtSet.PrvLumRatioTab = NULL;
			AeInfo->PrvExpectLum.y = ae_param_info[AeInfo->Id]->shdr->expy_le;
			AeInfo->PrvExpectLum.raw = AeInfo->InvGammaFunc(AeInfo->PrvExpectLum.y);
            //AeInfo->PrvExpectLum.raw = AeInfo->PrvExpectLum.y;
            AeInfo->ExtSet.PrvLumRatioTab = ae_param_info[AeInfo->Id]->shdr->tab_ratio_le;
			AeInfo->ExtSet.OverExp.AdjLumTab = ae_param_info[AeInfo->Id]->over_exposure->tab_thr_mov;
		}
	} else {
		if ((Info.mode == IPL_MODE_PREVIEW) || (Info.mode == IPL_MODE_CAP)) {
			AeBlkInfo->BLKLum_En = DISABLE;
			AeInfo->ExtSet.AdjHistoEn = DISABLE;
			AeInfo->ExtSet.PrvLumRatioTab = ae_param_info[AeInfo->Id]->expect_lum->tab_ratio_photo;
			AeInfo->ExtSet.OverExp.AdjLumTab = ae_param_info[AeInfo->Id]->over_exposure->tab_thr_photo;

			AeInfo->PrvExpectLum.y = ae_param_info[AeInfo->Id]->expect_lum->lum_photo;
			AeInfo->PrvExpectLum.raw = AeInfo->InvGammaFunc(AeInfo->PrvExpectLum.y);
            //AeInfo->PrvExpectLum.raw = AeInfo->PrvExpectLum.y;
		} else {
			AeBlkInfo->BLKLum_En = DISABLE;
			AeInfo->ExtSet.AdjHistoEn = DISABLE;

            if (ircut_mode == IQ_IRCUT_NIGHT) {
    			AeInfo->ExtSet.PrvLumRatioTab = ae_param_info[AeInfo->Id]->expect_lum->tab_ratio_ir;
    			AeInfo->ExtSet.OverExp.AdjLumTab = ae_param_info[AeInfo->Id]->over_exposure->tab_thr_ir;
            } else {
    			AeInfo->ExtSet.PrvLumRatioTab = ae_param_info[AeInfo->Id]->expect_lum->tab_ratio_mov;
    			AeInfo->ExtSet.OverExp.AdjLumTab = ae_param_info[AeInfo->Id]->over_exposure->tab_thr_mov;
            }

			AeInfo->PrvExpectLum.y = ae_param_info[AeInfo->Id]->expect_lum->lum_mov;//AeParamInfo[AeInfo->Id].ExpLum_Mov;
			AeInfo->PrvExpectLum.raw = AeInfo->InvGammaFunc(AeInfo->PrvExpectLum.y);
            //AeInfo->PrvExpectLum.raw = AeInfo->PrvExpectLum.y;

			//Hi Speed AE Mode
			//Only Set 1 time for one AE Peroid
			if (Info.mode == IPL_MODE_VIDEOREC) {
				frame_rate = AE_GetFrameRate(AeInfo->Id);
				if (frame_rate >= 1000) {
					AeCtrlInfo.CalSetShiftNum = AE_HISPED_CAL_SET_SHIFT;
					AeCtrlInfo.PredictPoint = AE_HISPED_PREDICT_POINT;
					AeCtrlInfo.AEPeroid = AE_HISPED_PEROID;
				}
			}
		}
	}

	AE_SetIntCtrlInfo(AeInfo->Id, AeCtrlInfo);
	IplMode[AeInfo->Id] = Info.mode;
    IRCutMode = ircut_mode;
}

void AE_getInfo(AE_INFO *AeInfo, AE_BLK_INFO *AeBlkInfo)
{
//    memset((void *)AeInfo, 0, sizeof(AE_INFO));

	AeInfo->StableRange = ae_param_info[AeInfo->Id]->convergence->range_conv;;

	AeInfo->StableRangeRF.h = AE_COVER_RANGE_RF_H;
	AeInfo->StableRangeRF.l = AE_COVER_RANGE_RF_L;

	AeInfo->histogram.bin_num = AE_HIST_NUM;
	AeInfo->histogram.data_num = AE_HIST_DATA_NUM;
	AeInfo->histogram.y = AeHistBuf[AeInfo->Id];

	weighting_histogram.bin_num = AE_HIST_NUM;
	weighting_histogram.data_num = (LA_WIN_X * LA_WIN_Y);
	weighting_histogram.y = AeHist2BufY[AeInfo->Id];

	// block luminance calculate
	//used in photo preview and capture only
	AeBlkInfo->FirstRunFlg = TRUE;
	AeBlkInfo->LA_Num_X = LA_WIN_X;
	AeBlkInfo->LA_Num_Y = LA_WIN_Y;
	AeBlkInfo->Yacc = Blk_Yacc[AeInfo->Id];
	AeBlkInfo->Gacc = Blk_Gacc[AeInfo->Id];

    //AeInfo->LA_Data = AeBlkInfo->Yacc;
    AeInfo->LA_Data = Blk_LA_Data[AeInfo->Id];

#if 1
	AeBlkInfo->Raw_OverBound_Lum.h = AeParamInfo[AeInfo->Id].Blk_OVER_Y_Thr.h;
	AeBlkInfo->Raw_OverBound_Lum.l = AeParamInfo[AeInfo->Id].Blk_OVER_Y_Thr.l;
	AeBlkInfo->Raw_UnderBound_Lum.h = AeParamInfo[AeInfo->Id].Blk_UNDER_Y_Thr.h;
	AeBlkInfo->Raw_UnderBound_Lum.l = AeParamInfo[AeInfo->Id].Blk_UNDER_Y_Thr.l;
#else
	AeBlkInfo->Raw_OverBound_Lum.h = AeInfo->InvGammaFunc(AeParamInfo[AeInfo->Id].Blk_OVER_Y_Thr.h);
	AeBlkInfo->Raw_OverBound_Lum.l = AeInfo->InvGammaFunc(AeParamInfo[AeInfo->Id].Blk_OVER_Y_Thr.l);
	AeBlkInfo->Raw_UnderBound_Lum.h = AeInfo->InvGammaFunc(AeParamInfo[AeInfo->Id].Blk_UNDER_Y_Thr.h);
	AeBlkInfo->Raw_UnderBound_Lum.l = AeInfo->InvGammaFunc(AeParamInfo[AeInfo->Id].Blk_UNDER_Y_Thr.l);
#endif
	AeBlkInfo->AddGammaFunc = AE_GET_GAMMA_VAL(AeInfo->Id);

	AeInfo->FirstRunFlag = TRUE;
	AeInfo->Reset = TRUE;
	AeInfo->Counter = 0;
	AeInfo->CounterRF = 0;
	AeInfo->CurConvDir = 0;
	AeInfo->EventTab = AeAlgEventTab;
	AeInfo->ConvSpeed = ae_param_info[AeInfo->Id]->convergence->speed;
	AeInfo->ConvTab = NULL;
	AeInfo->InvGammaFunc = AE_GET_INV_GAMMA_VAL(AeInfo->Id);
	AeInfo->AddGammaFunc = AE_GET_GAMMA_VAL(AeInfo->Id);

	//ext
	AeInfo->ExtSet.OverExp.AdjLum.y = AeParamInfo[AeInfo->Id].ExpLum_OverExp;
	AeInfo->ExtSet.OverExp.AdjLum.raw = AeInfo->InvGammaFunc(AeInfo->ExtSet.OverExp.AdjLum.y);
    //AeInfo->ExtSet.OverExp.AdjLum.raw = AeInfo->ExtSet.OverExp.AdjLum.y;
	AeInfo->ExtSet.OverExp.AdjMaxCntRatioTab = ae_param_info[AeInfo->Id]->over_exposure->tab_maxcnt;
	AeInfo->ExtSet.OverExp.AdjMinCntRatioTab = ae_param_info[AeInfo->Id]->over_exposure->tab_mincnt;
	AeInfo->ExtSet.OverExp.AdjLumThresholdTab = ae_param_info[AeInfo->Id]->over_exposure->tab_ratio;
	AeInfo->ExtSet.OverExp.CurCnt = 0;
	AeInfo->ExtSet.OverExp.CurTotalStep = 0;
	AeInfo->ExtSet.OverExp.CurMaxStep = 0;
	AeInfo->ExtSet.OverExp.CurMaxCnt = 0;
	AeInfo->ExtSet.OverExp.CurMinCnt = 0;
	AeInfo->ExtSet.OverExp.CurThreshold = 0;
	AeInfo->ExtSet.OverExp.UserDefRatio = TRUE;

	AeInfo->ExtSet.AdjHistoInfo.RatioTab = ae_param_info[AeInfo->Id]->histogram->tab_ratio;
	AeInfo->ExtSet.AdjHistoInfo.AdjLum.y = AeParamInfo[AeInfo->Id].ExpLum_Histo;
	AeInfo->ExtSet.AdjHistoInfo.AdjLum.raw = AeInfo->InvGammaFunc(AeInfo->ExtSet.AdjHistoInfo.AdjLum.y);
    //AeInfo->ExtSet.AdjHistoInfo.AdjLum.raw = AeInfo->ExtSet.AdjHistoInfo.AdjLum.y;

	AeInfo->ExtSet.AdjCapEn = DISABLE;
	AeInfo->ExtSet.CapExpectLum.y = AeParamInfo[AeInfo->Id].ExpLum_Photo;
	AeInfo->ExtSet.CapExpectLum.raw = AeInfo->InvGammaFunc(AeInfo->ExtSet.CapExpectLum.y);
    //AeInfo->ExtSet.CapExpectLum.raw = AeInfo->ExtSet.CapExpectLum.y;
	AeInfo->ExtSet.CapExpectTab = ae_param_info[AeInfo->Id]->expect_lum->tab_ratio_photo;

	AeInfo->ExtSet.UnStableDelay = ae_param_info[AeInfo->Id]->convergence->delay_cnt;

	//comp ev
	AeInfo->EVCompIdx = ae_get_ui_info((IPL_PROC_ID)AeInfo->Id, AE_UI_EV);
    AeInfo->EVCompRatio = EVTable[ae_get_ui_info((IPL_PROC_ID)AeInfo->Id, AE_UI_EV)];

    AeInfo->SmoothRatioThr = ae_param_info[AeInfo->Id]->convergence->smooth_ratio_thr;
    AeInfo->DePulseEn = ae_param_info[AeInfo->Id]->convergence->depulse_en;
    AeInfo->DePulseRatio = ae_param_info[AeInfo->Id]->convergence->depulse_ratio;
    AeInfo->DePulseFrm = ae_param_info[AeInfo->Id]->convergence->depulse_frm;
}

static void AE_GetFaceWin(UINT32 Id)
{
	UINT32 i, j, k, tmp;
	UINT32 FaceNum = 0;
	FD_FACE faceRect[10] = {0};
	URECT reSize;
	IPL_LA_SET_INFOR LA_Info = {0};
	UINT8 FaceWeighting = 32;
	Coordinate bin; //win number for face start
	USIZE binSize;  //win number for face size
	USIZE SizeofEachBin;

	LA_Info.id = Id;

    IPL_GetCmd(IPL_GET_LA_SET_INFOR, (void *)&LA_Info);

	reSize.x = reSize.y = 0;
    reSize.w = LA_Info.crop_size_h;
    reSize.h = LA_Info.crop_size_v;

	if ((FaceNum = FD_GetRsltFace(faceRect, &reSize)) != 0) {
		SizeofEachBin.w = LA_Info.crop_size_h / AE_WIN_X;
		SizeofEachBin.h = LA_Info.crop_size_v / AE_WIN_Y;

		memset(AE_Win_Face, 0, sizeof(UINT32)*AE_WIN_Y * AE_WIN_X);

		if (FaceNum > 10) {
			FaceNum = 10;
			DBG_ERR("\r\n");
		}

		for (i = 0 ; i < FaceNum ; i++) {
			tmp = faceRect[i].x;
			bin.x = 0;
			while (tmp >= SizeofEachBin.w) {
				tmp -= SizeofEachBin.w;
				bin.x++;
			}

			tmp = faceRect[i].y;
			bin.y = 0;
			while (tmp >= SizeofEachBin.h) {
				tmp -= SizeofEachBin.h;
				bin.y++;
			}

			binSize.w = (faceRect[i].w + SizeofEachBin.w - 1) / SizeofEachBin.w;
			binSize.h = (faceRect[i].h  + SizeofEachBin.h  - 1) / SizeofEachBin.h;
			//check boundary
			if ((bin.x + binSize.w) > AE_WIN_X) {
				binSize.w = AE_WIN_X - bin.x;
			}
			if ((bin.y + binSize.h) > AE_WIN_Y) {
				binSize.h = AE_WIN_Y - bin.y;
			}

			for (j = bin.y ; j < (bin.y + binSize.h) ; j++) {
				for (k = bin.x; k < (bin.x + binSize.w); k++) {
					AE_Win_Face[j][k] = FaceWeighting;
				}
			}

#if 0
            DBG_DUMP("\r\n");
            for(j=0; j<AE_WIN_Y; j++){
                for(k=0; k<AE_WIN_X; k++){
                    DBG_DUMP("%d, ", AE_Win_Face[j][k]);
                }
                DBG_DUMP("\r\n");
            }
#endif
		}
	}
}

void AE_SetExtInfo(UINT32 Id, AE_EXT_SET_INFO SetInfo)
{
	AE_SetPLINEInfo(Id, SetInfo);
}

void AE_GetExtInfo(UINT32 Id, AE_EXT_SET_INFO *SetInfo)
{
    UINT32 i, curve_cnt;

    curve_cnt= 0;
    for(i=0; i<AE_GEN_CURVE_NODE_MAX; i++){
        curve_cnt++;
        if(AeInfo[Id].PrvCurve[i][AEALG_EV] == 0xffffffff)
            break;
    }

	SetInfo->ExpTBound.l = AeInfo[Id].PrvCurve[0][AEALG_EXP];
    SetInfo->ExpTBound.h = AeInfo[Id].PrvCurve[curve_cnt-2][AEALG_EXP];
	SetInfo->IsoBound.l = AeInfo[Id].PrvCurve[0][AEALG_ISO];
    SetInfo->IsoBound.h = AeInfo[Id].PrvCurve[curve_cnt-2][AEALG_ISO];

	SetInfo->ExpT = uiExpoTime[Id];
	SetInfo->ISO = uiISOGain[Id];

	SetInfo->ConvergenceSpeed = ae_param_info[Id]->convergence->speed;

	SetInfo->IrisSetting.PIrisManualSize = uiIris[Id];
	SetInfo->IrisSetting.IrisType = ae_param_info[Id]->iris_info->IrisType;
	SetInfo->IrisSetting.IrisMode = ae_param_info[Id]->iris_info->IrisMode;
	SetInfo->IrisSetting.PIris = ae_param_info[Id]->iris_info->PIris;
	SetInfo->IrisSetting.PIrisManualSize = ae_param_info[Id]->iris_info->PIrisManualSize;
	SetInfo->IrisSetting.DCIrisSpeed = ae_param_info[Id]->iris_info->DCIrisSpeed;

	SetInfo->AETolerance = ae_param_info[Id]->convergence->range_conv.h;
	SetInfo->AEDelayCnt = ae_param_info[Id]->convergence->delay_cnt;
	AE_GetWin(Id, SetInfo->ExpAreaWeight);

    SetInfo->AEStrategy = AeCurveGenInfo[Id].auto_lowlight_en;
}

void AE_SetPLINEInfo(UINT32 Id, AE_EXT_SET_INFO SetInfo)
{
    UINT32 i, j, idx;

    ae_param_info[Id]->convergence->speed = SetInfo.ConvergenceSpeed;
    ae_param_info[Id]->convergence->range_conv.h = SetInfo.AETolerance;
    ae_param_info[Id]->convergence->range_conv.l = SetInfo.AETolerance;
    ae_param_info[Id]->convergence->delay_cnt = SetInfo.AEDelayCnt;

	for (j = 0; j < AE_WIN_Y; j ++) {
		idx = (j * AE_WIN_X);
		for (i = 0; i < AE_WIN_X; i ++) {
			ae_param_info[Id]->meter_win[idx] = (UINT32)SetInfo.ExpAreaWeight[idx];
			idx += 1;
		}
	}

    AeInfo[Id].Reset = TRUE;

    //AeCurveGenInfo[Id].auto_lowlight_en = ae_param_info[Id]->curve_gen_param->auto_lowlight_en;
    //AeCurveGenInfo[Id].frame_rate = AE_GetFrameRate(Id);
    //memcpy(AeCurveGenInfo[Id].hdr_ratio, ae_param_info[Id]->curve_gen_param->hdr_ratio, sizeof(AeCurveGenInfo[Id].hdr_ratio));
    //memcpy(AeCurveGenInfo[Id].ext_fps, ae_param_info[Id]->curve_gen_param->ext_fps, sizeof(AeCurveGenInfo[Id].ext_fps));

    AeCurveGenInfo[Id].auto_lowlight_en = SetInfo.AEStrategy;
    AeCurveGenInfo[Id].node[0].expt = SetInfo.ExpTBound.l;
    AE_AlgSetUIInfo((AE_PROC_ID)Id, AE_SEL_MINFPS, (10000000/SetInfo.ExpTBound.h));
    AeCurveGenInfo[Id].node[0].iso = SetInfo.IsoBound.l;
    AeCurveGenInfo[Id].node[1].iso = (AeCurveGenInfo[Id].node[0].iso<<1);
    AeCurveGenInfo[Id].iso_max = SetInfo.IsoBound.h;
    ae_param_info[Id]->curve_gen_param->iso_max = AeCurveGenInfo[Id].iso_max;

    uiExpoTime[Id] = SetInfo.ExpT;
    uiISOGain[Id] = SetInfo.ISO;
	//uiIris[Id] = 0;

    AE_CurveUpdate(Id);
}

#define swap(x, y)  {UINT32 t; t=x;  x=y; y=t; }

AE_RESET AE_setWin(UINT32 Id, AE_WIN mode)
{
	UINT32 ix, iy;
	static AE_WIN prvmode[AE_ID_MAX_NUM] = {0xFF};
    static UINT32 pre_img_flip[AE_ID_MAX_NUM] = {0xFF};
    UINT32 img_flip;

    // Just for dsp flip
    img_flip = IPL_CtrlGetInfor(Id, IPLCTRL_FLIP_TYPE);

    if(IPL_AlgGetUIInfo((IPL_PROC_ID)Id, IPL_SEL_FD_IMM)==SEL_FD_OFF){
    	if ((mode == prvmode[Id])&&(pre_img_flip[Id] == img_flip)&& AeInfo[Id].Reset !=TRUE ) {
    		return AE_RESET_FALSE;
    	}
    }

    switch(mode)
    {
    case AE_WIN_SPOT:
    	for (iy = 0; iy < AE_WIN_Y; iy++) {
    		for (ix = 0; ix < AE_WIN_X; ix++) {
    			AeWin[iy][ix] = AE_Win_Spot[iy][ix];
    		}
    	}
        break;
    case AE_WIN_CENTERWEIGHT:
    	for (iy = 0; iy < AE_WIN_Y; iy++) {
    		for (ix = 0; ix < AE_WIN_X; ix++) {
    			AeWin[iy][ix] = AE_Win_CenterWieght[iy][ix];
    		}
    	}
        break;
    case AE_WIN_USERDEFINE:
    default:
    	for (iy = 0; iy < AE_WIN_Y; iy++) {
    		for (ix = 0; ix < AE_WIN_X; ix++) {
    			AeWin[iy][ix] = ae_param_info[Id]->meter_win[iy*AE_WIN_X+ix];
    		}
    	}
        break;
    }

	if (IPL_AlgGetUIInfo((IPL_PROC_ID)Id, IPL_SEL_FD_IMM) == SEL_FD_ON) {
		//FD AE
		AE_GetFaceWin(Id);
    	for (iy = 0; iy < AE_WIN_Y; iy++) {
    		for (ix = 0; ix < AE_WIN_X; ix++) {
				if (AE_Win_Face[iy][ix] != 0) {
					AeWin[iy][ix] += AE_Win_Face[iy][ix];
				}
			}
		}
	}

    if((img_flip == SEL_IMAGEFLIP_H)||(img_flip == SEL_IMAGEFLIP_H_V)){
    	for (iy = 0; iy < AE_WIN_Y; iy++) {
    		for (ix = 0; ix < (AE_WIN_X>>1); ix++) {
                swap(AeWin[iy][ix], AeWin[iy][AE_WIN_X-1-ix]);
    		}
    	}
    }

    if((img_flip == SEL_IMAGEFLIP_V)||(img_flip == SEL_IMAGEFLIP_H_V)){
    	for (iy = 0; iy < (AE_WIN_Y>>1); iy++) {
    		for (ix = 0; ix < AE_WIN_X; ix++) {
                swap(AeWin[iy][ix], AeWin[AE_WIN_Y-1-iy][ix]);
    		}
    	}
    }

    pre_img_flip[Id] = img_flip;

	prvmode[Id] = mode;

	return AE_RESET_TRUE;
}

UINT32 AE_getFlag(UINT32 Id)
{
	return AeFlag[Id];
}

void AE_setFlag(UINT32 Id, UINT32 flag)
{
	AeFlag[Id] |= flag;
}

void AE_clrFlag(UINT32 Id, UINT32 flag)
{
	AeFlag[Id] &= ~flag;
}

void AE_reset(UINT32 Id)
{
	AeInfo[Id].Reset = TRUE;
}

AE_RESET AE_getCapTab(AE_INFO *AeInfo)
{
	static UINT32 Cap_Scene[AE_ID_MAX_NUM] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
    static UINT32 Cap_Iso[AE_ID_MAX_NUM] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
    static UINT32 Cap_Flicker[AE_ID_MAX_NUM] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
    static UINT32 Cap_LongExp_En[AE_ID_MAX_NUM] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
    static UINT32 Cap_LongExp_ExpT[AE_ID_MAX_NUM] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
    static UINT32 Cap_LongExp_ISO[AE_ID_MAX_NUM] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
	UINT32 Curve = 0;
	UINT32 status = E_OK;
    UINT32 iso_mode_tmp;
    AE_ISO nISO;
    AE_FLICKER nFlicker;
    AE_SCENE nScene;
    UINT32 nLongexp_En, nLongexp_ExpT, nLongexp_ISO;
    UINT32 base_shutter;

    nISO = ae_get_ui_info((IPL_PROC_ID)AeInfo->Id, AE_UI_ISO);
    nFlicker = AE_getFlickerType(ae_get_ui_info((IPL_PROC_ID)AeInfo->Id, AE_UI_FREQUENCY));
    nScene = ae_get_ui_info((IPL_PROC_ID)AeInfo->Id, AE_UI_SCENE);
    nLongexp_En = ae_get_ui_info((IPL_PROC_ID)AeInfo->Id, AE_UI_CAP_LONGEXP);
    nLongexp_ExpT = ae_get_ui_info((IPL_PROC_ID)AeInfo->Id, AE_UI_CAP_LONGEXP_EXPT);
    nLongexp_ISO = ae_get_ui_info((IPL_PROC_ID)AeInfo->Id, AE_UI_CAP_LONGEXP_ISO);

	if (AeCurveUpdate_Cap[AeInfo->Id] == TRUE)
		AeCurveUpdate_Cap[AeInfo->Id] = FALSE;
	else if ((Cap_Scene[AeInfo->Id] == nScene) && (Cap_Iso[AeInfo->Id] == nISO) && (Cap_Flicker[AeInfo->Id] == nFlicker)
        &&(Cap_LongExp_En[AeInfo->Id] == nLongexp_En)&&(Cap_LongExp_ExpT[AeInfo->Id] == nLongexp_ExpT)&&(Cap_LongExp_ISO[AeInfo->Id] == nLongexp_ISO)) {
		return AE_RESET_FALSE;
	}

	memset(&AeAutoCurveTab_Cap[AeInfo->Id][0][0], 0x0, sizeof(UINT32) * (AE_GEN_CURVE_NODE_MAX * AEALG_INFO_MAX));

    if(ae_get_ui_info((IPL_PROC_ID)AeInfo->Id, AE_UI_CAP_LONGEXP)==AE_CAP_LONGEXP_ON){
        //DBG_ERR("%d, %d, %d\r\n", ae_get_ui_info((IPL_PROC_ID)AeInfo->Id, AE_UI_CAP_LONGEXP), ae_get_ui_info((IPL_PROC_ID)AeInfo->Id, AE_UI_CAP_LONGEXP_EXPT), ae_get_ui_info((IPL_PROC_ID)AeInfo->Id, AE_UI_CAP_LONGEXP_ISO));
		AeInfo->CapISO.h = ae_get_ui_info((IPL_PROC_ID)AeInfo->Id, AE_UI_CAP_LONGEXP_ISO);
		AeInfo->CapISO.l = AeCurveGenPhoto[AeInfo->Id].node[0].iso;
        AeAutoCurveTab_Cap[AeInfo->Id][0][AEALG_EV] = 0;
        AeAutoCurveTab_Cap[AeInfo->Id][0][AEALG_ISO] = AeCurveGenPhoto[AeInfo->Id].node[0].iso;
        AeAutoCurveTab_Cap[AeInfo->Id][0][AEALG_EXP] = ae_get_ui_info((IPL_PROC_ID)AeInfo->Id, AE_UI_CAP_LONGEXP_EXPT);
        AeAutoCurveTab_Cap[AeInfo->Id][0][AEALG_IRIS] = 0;
        AeAutoCurveTab_Cap[AeInfo->Id][0][AEALG_ADJ_TARGET] = AEALG_ADJ_GAIN;

        AeAutoCurveTab_Cap[AeInfo->Id][1][AEALG_EV] = 0xFFFFFFFF;
        AeAutoCurveTab_Cap[AeInfo->Id][1][AEALG_ISO] = 0;
        AeAutoCurveTab_Cap[AeInfo->Id][1][AEALG_EXP] = 0;
        AeAutoCurveTab_Cap[AeInfo->Id][1][AEALG_IRIS] = 0;
        AeAutoCurveTab_Cap[AeInfo->Id][1][AEALG_ADJ_TARGET] = AEALG_ADJ_GAIN;

        AeInfo->CapCurve = (UINT32(*)[AEALG_INFO_MAX])&AeAutoCurveTab_Cap[AeInfo->Id][0][0];
    } else {

        AeCurveGenPhoto[AeInfo->Id].iris_en = ae_param_info[AeInfo->Id]->iris_cfg->enable;
        AeCurveGenPhoto[AeInfo->Id].iris_min_expt = ae_param_info[AeInfo->Id]->iris_cfg->exptime_min;

        if (ae_param_info[AeInfo->Id]->iris_cfg->enable == ENABLE) {
            base_shutter = (nFlicker == AE_FLICKER_50HZ) ? 10000 : 8333;
            AeCurveGenPhoto[AeInfo->Id].iris_min_expt = (ae_param_info[AeInfo->Id]->iris_cfg->exptime_min > 0) ? ae_param_info[AeInfo->Id]->iris_cfg->exptime_min : base_shutter;
        }

        iso_mode_tmp = AeCurveGenPhoto[AeInfo->Id].iso_mode;

        if(ae_param_info[AeInfo->Id]->curve_gen_param->fno_sel==FNO_SEL_PREDEF)
            AeCurveGenPhoto[AeInfo->Id].fno_rat = GetFnoRatio(AeInfo->Id);
        else {
            AeCurveGenPhoto[AeInfo->Id].fno_idx = 0;
            AeCurveGenPhoto[AeInfo->Id].fno_rat = ae_param_info[AeInfo->Id]->curve_gen_param->fno_rat;
        }

        if(Cap_Flicker[AeInfo->Id] != ae_param_info[AeInfo->Id]->curve_gen_param->freq) {
            // From ISP tool
            AeCurveGenPhoto[AeInfo->Id].freq = (ae_param_info[AeInfo->Id]->curve_gen_param->freq == AE_FLICKER_50HZ) ? AE_GEN_FREQ_50 : AE_GEN_FREQ_60;

            if(ae_param_info[AeInfo->Id]->curve_gen_param->freq == AE_FLICKER_50HZ){
                ae_set_ui_info((IPL_PROC_ID)AeInfo->Id, AE_UI_FREQUENCY, AE_FREQUENCY_50HZ);
            } else {
                ae_set_ui_info((IPL_PROC_ID)AeInfo->Id, AE_UI_FREQUENCY, AE_FREQUENCY_60HZ);
            }
            Cap_Flicker[AeInfo->Id] = ae_param_info[AeInfo->Id]->curve_gen_param->freq;
        } else if(Cap_Flicker[AeInfo->Id] != nFlicker) {
            // From UI
            AeCurveGenPhoto[AeInfo->Id].freq = (nFlicker == AE_FLICKER_50HZ) ? AE_GEN_FREQ_50 : AE_GEN_FREQ_60;
            ae_param_info[AeInfo->Id]->curve_gen_param->freq = nFlicker;
            Cap_Flicker[AeInfo->Id] = nFlicker;
        }

        AeCurveGenPhoto[AeInfo->Id].iso_value = ae_get_ui_info(AeInfo->Id, AE_UI_ISO);

        if(AeCurveGenPhoto[AeInfo->Id].iso_value == 0)
           AeCurveGenPhoto[AeInfo->Id].iso_mode = AE_ISOMODE_PHOTO_PARAM;

        if(AeCurveGenPhoto[AeInfo->Id].iso_mode == AE_ISOMODE_PHOTO_PARAM) {
            AeCurveGenPhoto[AeInfo->Id].iso_max = ae_param_info[AeInfo->Id]->curve_gen_photo->iso_max;
        } else {
            if(AeCurveGenPhoto[AeInfo->Id].iso_value==0)
                AeCurveGenPhoto[AeInfo->Id].iso_max = ae_param_info[AeInfo->Id]->curve_gen_photo->iso_max;
            else
                AeCurveGenPhoto[AeInfo->Id].iso_max = ae_get_ui_info(AeInfo->Id, AE_UI_ISO);
        }

    	status = AEAlg_GenCurvePhoto(&AeCurveGenPhoto[AeInfo->Id], &Curve, &AeInfo->PhotoEV.h, &AeInfo->PhotoEV.l);

    	if (status == E_OK)
        {
            memcpy(&AeAutoCurveTab_Cap[AeInfo->Id][0][0], (UINT32 *)Curve, sizeof(UINT32) * (AE_GEN_CURVE_NODE_MAX * AEALG_INFO_MAX));
    		AeInfo->CapISO.h = AeCurveGenPhoto[AeInfo->Id].iso_max;
    		AeInfo->CapISO.l = AeCurveGenPhoto[AeInfo->Id].node[0].iso;
    		AeInfo->CapCurve = (UINT32(*)[AEALG_INFO_MAX])&AeAutoCurveTab_Cap[AeInfo->Id][0][0];
    	}

        AeCurveGenPhoto[AeInfo->Id].iso_mode = iso_mode_tmp;
    }

	Cap_Scene[AeInfo->Id] = nScene;
	Cap_Iso[AeInfo->Id] = nISO;
	Cap_Flicker[AeInfo->Id] = nFlicker;
    Cap_LongExp_En[AeInfo->Id] = nLongexp_En;
    Cap_LongExp_ExpT[AeInfo->Id] = nLongexp_ExpT;
    Cap_LongExp_ISO[AeInfo->Id] = nLongexp_ISO;

	return AE_RESET_TRUE;
}

AE_RESET AE_getPrvTab(AE_INFO *AeInfo)
{
	UINT32 Curve = 0;
	UINT32 status = E_OK;
    UINT32 iso_mode_tmp;
    AE_ISO nISO;
    AE_FLICKER nFlicker;
    UINT32 hdr_switch;
	IPL_MODE_INFOR Info = {0};
    ER rt = E_OK;
    SHDR_EV_RATIO_INFO ev_ratio;
    UINT32 i, hdr_size;
    UINT32 base_shutter;
    EXPOSURE_PARAM exp_param = {0};

    nISO = ae_get_ui_info((IPL_PROC_ID)AeInfo->Id, AE_UI_ISO);
    nFlicker = AE_getFlickerType(ae_get_ui_info((IPL_PROC_ID)AeInfo->Id, AE_UI_FREQUENCY));
	AeInfo->FPS = AE_GetFrameRate(AeInfo->Id);
    hdr_switch = IPL_AlgGetUIInfo((IPL_PROC_ID)AeInfo->Id, IPL_SEL_SHDR);

	//if ((AeCurveUpdate_Prv[AeInfo->Id] == TRUE) || (AeExtSetDefaultParamInfo[AeInfo->Id].IrisInfo.IrisMode == AE_IRIS_MODE_AUTO)) {
	if (AeCurveUpdate_Prv[AeInfo->Id] == TRUE) {
		AeCurveUpdate_Prv[AeInfo->Id] = FALSE;
	} else {
		if (((Flicker[AeInfo->Id] == nFlicker)&&(Flicker[AeInfo->Id] == ae_param_info[AeInfo->Id]->curve_gen_param->freq)) &&
			(Iso[AeInfo->Id] == nISO) &&
			(AE_Fno[AeInfo->Id] == AeCurveGenInfo[AeInfo->Id].fno_rat) &&
			(Fps[AeInfo->Id] == AeInfo->FPS)&&
			(AE_HDR_Switch[AeInfo->Id] == hdr_switch)&&
			 AeInfo->Reset != TRUE) {
			return AE_RESET_FALSE;
		}
	}

    // Auto calculate minimum exposure time
	exp_param.exp_time[0] = 10000;
	exp_param.sensor_mode = SENSOR_MODE_CUR;
	rt = sensor_get_info((SENSOR_ID)AeInfo->Id, SEN_GET_EXPO_SETTING, (void *)&exp_param);

    if(rt != E_OK)
        DBG_ERR("Get sensor information error! (SEN_GET_EXPO_SETTING)\r\n");

    ae_param_info[AeInfo->Id]->curve_gen_param->node[0].expt = ((exp_param.exp_time[0]+(exp_param.exp_setting.ui_line[0]>>1))/exp_param.exp_setting.ui_line[0]);

    if(ae_param_info[AeInfo->Id]->curve_gen_param->fno_sel==FNO_SEL_PREDEF)
        AeCurveGenInfo[AeInfo->Id].fno_rat = GetFnoRatio(AeInfo->Id);
    else {
        AeCurveGenInfo[AeInfo->Id].fno_idx = 0;
        AeCurveGenInfo[AeInfo->Id].fno_rat = ae_param_info[AeInfo->Id]->curve_gen_param->fno_rat;
    }

	AeCurveGenInfo[AeInfo->Id].fno_rat = AE_Check_Fno(AeInfo->Id, AeCurveGenInfo[AeInfo->Id].fno_rat);
    ae_param_info[AeInfo->Id]->curve_gen_param->fno_rat = AeCurveGenInfo[AeInfo->Id].fno_rat;

    AeCurveGenInfo[AeInfo->Id].flicker_mode = ae_param_info[AeInfo->Id]->curve_gen_param->flicker_mode;

	memset(&AeAutoCurveTab_Prv[AeInfo->Id][0][0], 0x0, sizeof(UINT32) * (AE_GEN_CURVE_NODE_MAX * AEALG_INFO_MAX));

    AeCurveGenInfo[AeInfo->Id].hdr_frame = IPL_CtrlGetInfor(AeInfo->Id, IPLCTRL_MF_TOTAL_FRM_NUM);

    // Get HDR ratio
    hdr_size = (sizeof(AeCurveGenInfo[AeInfo->Id].hdr_ratio)/sizeof(AeCurveGenInfo[AeInfo->Id].hdr_ratio[0]));

    for(i=0; i<hdr_size; i++) {
        AeCurveGenInfo[AeInfo->Id].hdr_ratio[i] = ae_param_info[AeInfo->Id]->curve_gen_param->hdr_ratio[i];
        ev_ratio.uiEVratio[i] = ae_param_info[AeInfo->Id]->curve_gen_param->hdr_ratio[i];
    }

    if(AE_checkHDRRatio(ev_ratio) == FALSE){
        DBG_WRN("Get HDR ratio error! Force change to default (64, 512, 4096).\r\n");
        AeCurveGenInfo[AeInfo->Id].hdr_ratio[0] = 64;
        AeCurveGenInfo[AeInfo->Id].hdr_ratio[1] = 512;
        AeCurveGenInfo[AeInfo->Id].hdr_ratio[2] = 4096;
    }

    if(Flicker[AeInfo->Id] != ae_param_info[AeInfo->Id]->curve_gen_param->freq) {
        // From ISP tool
        AeCurveGenInfo[AeInfo->Id].freq = (ae_param_info[AeInfo->Id]->curve_gen_param->freq == AE_FLICKER_50HZ) ? AE_GEN_FREQ_50 : AE_GEN_FREQ_60;

        if(ae_param_info[AeInfo->Id]->curve_gen_param->freq == AE_FLICKER_50HZ){
            ae_set_ui_info((IPL_PROC_ID)AeInfo->Id, AE_UI_FREQUENCY, AE_FREQUENCY_50HZ);
        } else {
            ae_set_ui_info((IPL_PROC_ID)AeInfo->Id, AE_UI_FREQUENCY, AE_FREQUENCY_60HZ);
        }
        Flicker[AeInfo->Id] = ae_param_info[AeInfo->Id]->curve_gen_param->freq;
    } else if(Flicker[AeInfo->Id] != nFlicker) {
        // From UI
        AeCurveGenInfo[AeInfo->Id].freq = (nFlicker == AE_FLICKER_50HZ) ? AE_GEN_FREQ_50 : AE_GEN_FREQ_60;
        ae_param_info[AeInfo->Id]->curve_gen_param->freq = nFlicker;
        Flicker[AeInfo->Id] = nFlicker;
    }

    AeCurveGenInfo[AeInfo->Id].iris_en = ae_param_info[AeInfo->Id]->iris_cfg->enable;

    if (ae_param_info[AeInfo->Id]->iris_cfg->enable == ENABLE) {
        base_shutter = (nFlicker == AE_FLICKER_50HZ) ? 10000 : 8333;
        AeIrisInfo[AeInfo->Id].exptime_min = (ae_param_info[AeInfo->Id]->iris_cfg->exptime_min > 0) ? ae_param_info[AeInfo->Id]->iris_cfg->exptime_min : base_shutter;
    } else {
        AeIrisInfo[AeInfo->Id].exposure_freeze = 0;
    }

    AeCurveGenInfo[AeInfo->Id].iris_min_expt = AeIrisInfo[AeInfo->Id].exptime_min;

    AeIrisInfo[AeInfo->Id].freeze_time = ((AeInfo->FPS*ae_param_info[AeInfo->Id]->iris_cfg->freeze_time)/10000);
    AeIrisInfo[AeInfo->Id].unfreeze_time = ((AeInfo->FPS*ae_param_info[AeInfo->Id]->iris_cfg->unfreeze_time)/10000);

	AeCurveGenInfo[AeInfo->Id].frame_rate = AeInfo->FPS;

    AeCurveGenInfo[AeInfo->Id].hdr_exp_gap = 0; // Exposure time gap by sensor limit

    AeCurveGenInfo[AeInfo->Id].iso_value = ae_get_ui_info((IPL_PROC_ID)AeInfo->Id, AE_UI_ISO);

    iso_mode_tmp = AeCurveGenInfo[AeInfo->Id].iso_mode;

	Info.id = AeInfo->Id;
	rt = IPL_GetCmd(IPL_GET_CUR_MODE, (void *)&Info);

    if(rt == E_OK) {
        if(Info.mode == IPL_MODE_PREVIEW){
            AeCurveGenInfo[AeInfo->Id].iso_max = ae_param_info[AeInfo->Id]->curve_gen_param->iso_max;
        } else {
            if(AeCurveGenInfo[AeInfo->Id].iso_mode == AE_ISOMODE_MOV_PARAM) {
                AeCurveGenInfo[AeInfo->Id].iso_max = ae_param_info[AeInfo->Id]->curve_gen_param->iso_max;
            } else {
                if(ae_get_ui_info((IPL_PROC_ID)AeInfo->Id, AE_UI_ISO)==0) // ISO = Auto
                    AeCurveGenInfo[AeInfo->Id].iso_max = ae_param_info[AeInfo->Id]->curve_gen_param->iso_max;
                else
                    AeCurveGenInfo[AeInfo->Id].iso_max = ae_get_ui_info((IPL_PROC_ID)AeInfo->Id, AE_UI_ISO);
            }
        }
    } else {
        DBG_ERR("Get IPL mode error!\r\n");
    }

    AeCurveGenInfo[AeInfo->Id].iso_mode = AE_ISOMODE_MOV_PARAM;

    AeCurveGenInfo[AeInfo->Id].auto_lowlight_en = ae_param_info[AeInfo->Id]->curve_gen_param->auto_lowlight_en;

	AeCurveGenInfo[AeInfo->Id].node_num = ae_param_info[AeInfo->Id]->curve_gen_param->node_num;

    memcpy(AeCurveGenInfo[AeInfo->Id].node, ae_param_info[AeInfo->Id]->curve_gen_param->node, sizeof(AeCurveGenInfo[AeInfo->Id].node));
    memcpy(AeCurveGenInfo[AeInfo->Id].ext_fps, ae_param_info[AeInfo->Id]->curve_gen_param->ext_fps, sizeof(AeCurveGenInfo[AeInfo->Id].ext_fps));

	status = AEAlg_GenCurve(&AeCurveGenInfo[AeInfo->Id], &Curve, &AeInfo->MovieEV.h, &AeInfo->MovieEV.l, ae_shdr_trans[AeInfo->Id].expo_time_max);

    // For re-check extend fps setting
    memcpy(ae_param_info[AeInfo->Id]->curve_gen_param->ext_fps, AeCurveGenInfo[AeInfo->Id].ext_fps, sizeof(AeCurveGenInfo[AeInfo->Id].ext_fps));

	if (status == E_OK)
    {
        memcpy(&AeAutoCurveTab_Prv[AeInfo->Id][0][0], (UINT32 *)Curve, sizeof(UINT32) * (AE_GEN_CURVE_NODE_MAX * AEALG_INFO_MAX));
		AeInfo->ISO.h = AeCurveGenInfo[AeInfo->Id].iso_max;
		AeInfo->ISO.l = AeCurveGenInfo[AeInfo->Id].node[0].iso;
		AeInfo->PrvCurve = (UINT32(*)[AEALG_INFO_MAX])&AeAutoCurveTab_Prv[AeInfo->Id][0][0];
	}
    else
    {
		DBG_ERR("AEAlg_GenCurve ERR = %d\r\n", status);
	}

    AeCurveGenInfo[AeInfo->Id].iso_mode = iso_mode_tmp;

	//Flicker[AeInfo->Id] = nFlicker;
	Iso[AeInfo->Id] = nISO;
	Fps[AeInfo->Id] = AeInfo->FPS;
    AE_Fno[AeInfo->Id] = AeCurveGenInfo[AeInfo->Id].fno_rat;
    AE_HDR_Switch[AeInfo->Id] = hdr_switch;

    //AE_DumpCurveInfo(AeInfo->Id, AE_CURVEDUMP_PRV);

	return AE_RESET_TRUE;
}

#if 0
#endif
//------------------------------------------------------------------------------
UINT32 GetOverExpoRatio(UINT32 HistMaxNum, UINT16 *Hist)
{
	UINT32 i;
	UINT32 AvgHCnt = 0;
	UINT32 step = (AeInfo[AE_ID_1].ExtSet.OverExp.CurThreshold - (((1 << 10) / weighting_histogram.bin_num) >> 1)) / ((1 << 10) / weighting_histogram.bin_num);

	for (i = 0; i < weighting_histogram.bin_num; i ++) {
		if (i < step) {
		} else {
			AvgHCnt += (UINT32)weighting_histogram.y[i];
		}
	}

	AvgHCnt = (AvgHCnt * 1000) / weighting_histogram.data_num;
	return AvgHCnt;
}

static UINT32 AE_FNO_Table[FNO_MAX] = {20, 26, 35, 40, 53, 70, 80, 160, 485, 2560};

UINT32 GetFnoRatio(UINT32 Id)
{
    UINT32 fno_ratio;

    if(ae_param_info[Id]->curve_gen_param->fno_sel==FNO_SEL_PREDEF)
        fno_ratio = AE_FNO_Table[AeCurveGenInfo[Id].fno_idx];
    else
        fno_ratio = AeCurveGenInfo[Id].fno_rat;

	return fno_ratio;
}
#if 0
#endif
//------------------------------------------------------------------------------
//#NT#2017/10/31#Photon Lin -begin
#if 0
AE_SCENE AE_getSceneType(UINT32 Idx)
{
	switch (Idx) {
	case SEL_SCENEMODE_AUTO:
		return AE_SCENE_AUTO;

	case SEL_SCENEMODE_LANDSCAPE:
		return AE_SCENE_LANDSCAPE;

	case SEL_SCENEMODE_POTRAIT:
		return AE_SCENE_POTRAIT;

	case SEL_SCENEMODE_SPORT:
		return AE_SCENE_SPORT;

	case SEL_SCENEMODE_NIGHTSCENE:
		return AE_SCENE_NIGHTSCENE;

	case SEL_SCENEMODE_AV:
		return AE_SCENE_AV;

	case SEL_SCENEMODE_TV:
		return AE_SCENE_TV;

	case SEL_SCENEMODE_MANUAL:
		return AE_SCENE_MANUAL;

	case SEL_SCENEMODE_NIGHTPOTRAIT:
		return AE_SCENE_NIGHTPOTRAIT;

	case SEL_SCENEMODE_NIGHTLANDSCAPE:
		return AE_SCENE_NIGHTLANDSCAPE;

	case SEL_SCENEMODE_PARTY:
		return AE_SCENE_PARTY;

	case SEL_SCENEMODE_SUNSET:
		return AE_SCENE_SUNSET;

	case SEL_SCENEMODE_BACKLIGHT:
		return AE_SCENE_BACKLIGHT;

	case SEL_SCENEMODE_DOCUMENT:
		return AE_SCENE_DOCUMENT;

	case SEL_SCENEMODE_FIREWORK:
		return AE_SCENE_FIREWORK;

	case SEL_SCENEMODE_BEACH:
		return AE_SCENE_BEACH;

	case SEL_SCENEMODE_BMODE:
		return AE_SCENE_BMODE;

	case SEL_SCENEMODE_PROGRAM:
		return AE_SCENE_PROGRAM;

	case SEL_SCENEMODE_HIGHISO:
		return AE_SCENE_HIGHISO;

	default:
		return AE_SCENE_AUTO;
	}
}
#endif
//#NT#2017/10/31#Photon Lin -end

BOOL AE_checkHDRRatio(SHDR_EV_RATIO_INFO ev_ratio)
{
    UINT32 i;

    for(i=0; i<(sizeof(ev_ratio.uiEVratio)/sizeof(ev_ratio.uiEVratio[0])); i++) {
        if(ev_ratio.uiEVratio[i] < 64) {
            return FALSE;
        }
    }

    return TRUE;
}

AE_FLICKER AE_getFlickerType(UINT32 Idx)
{
	if (Idx == AE_FREQUENCY_AUTO) {
		if (AEAFD_GetRslt() == AEAFD_FLICKER_50Hz) {
			AEAFD_CurrentFlickerType = AE_FLICKER_50HZ;
		}
		if (AEAFD_GetRslt() == AEAFD_FLICKER_60Hz) {
			AEAFD_CurrentFlickerType = AE_FLICKER_60HZ;
		}
		return AEAFD_CurrentFlickerType;
	} else if (Idx == AE_FREQUENCY_50HZ) {
		return AE_FLICKER_50HZ;
	} else {
		return AE_FLICKER_60HZ;
	}
}

AE_ANTISHAKE AE_getAntiShakeLv(UINT32 Idx)
{

	return AE_ANTISHAKE_LV0;
}

AE_WIN AE_getWinType(UINT32 Idx)
{
	if (Idx == AE_METER_CENTERWEIGHTED) {
		return AE_WIN_CENTERWEIGHT;
	}
	if (Idx == AE_METER_SPOT) {
		return AE_WIN_SPOT;
	}
	if (Idx == AE_METER_MATRIX) {
		return AE_WIN_MARTIX;
	}
	if (Idx == AE_METER_EVALUATIVE) {
		return AE_WIN_EVALUATIVE;
	}
	if (Idx == AE_METER_USERDEFINE) {
		return AE_WIN_USERDEFINE;
	}
	return AE_WIN_CENTERWEIGHT;
}

#if 0
#endif
//------------------------------------------------------------------------------
UINT32 AE_getPrvBiningMode(UINT32 Id)
{
	SENSOR_INFO SensorInfo = {0};
	SEN_STATUS_PARAM sensor_status = {0};
	SENSOR_MODE PrvSensorMode;
	ER rt = E_OK;
	UINT32 bining_ratio;

	PrvSensorMode = (SENSOR_MODE)IPL_AlgGetUIInfo((IPL_PROC_ID)Id, IPL_SEL_PRVSENMODE);
	sensor_status.sensor_mode = PrvSensorMode;
	rt = sensor_get_info((SENSOR_ID)Id, SEN_GET_STATUS, (void *)&sensor_status);
	SensorInfo = sensor_status.sensor_info;
	if (rt != E_OK || SensorInfo.mode == NULL) {
		bining_ratio = 100;
	} else {
		bining_ratio = SensorInfo.mode->bining_ratio;
	}

	return bining_ratio;
}

UINT32 AE_getCapBiningMode(UINT32 Id)
{
	SENSOR_INFO SensorInfo = {0};
	SEN_STATUS_PARAM sensor_status = {0};
	SENSOR_MODE CapSensorMode;
	ER rt = E_OK;
	UINT32 bining_ratio;

	CapSensorMode = (SENSOR_MODE)IPL_AlgGetUIInfo((IPL_PROC_ID)Id, IPL_SEL_CAPSENMODE);
	sensor_status.sensor_mode = CapSensorMode;
	rt = sensor_get_info((SENSOR_ID)Id, SEN_GET_STATUS, (void *)&sensor_status);
	SensorInfo = sensor_status.sensor_info;
	if (rt != E_OK || SensorInfo.mode == NULL) {
		bining_ratio = 100;
	} else {
		bining_ratio = SensorInfo.mode->bining_ratio;
	}

	return bining_ratio;
}

void AE_setExpT(UINT32 Id, EXPOSURE_SETTING *ExpTSetting)
{

	if ((AE_getFlag(Id) & AE_FLAG_SET_EXPT) == AE_FLAG_SET_EXPT) {
		if ((ExpTSetting->ui_frame[0] == 0xffffffff) && (ExpTSetting->ui_line[0] == 0xffffffff) && (ExpTSetting->ui_pixel[0] == 0xffffffff)) {
			DBG_MSG("Exposure Line Value Invalid\r\n");
		} else {
			sensor_set_info(Id, SEN_SET_EXPT, (void *)ExpTSetting);
		}

		ExpTSetting->ui_frame[0] = 0xffffffff;
		ExpTSetting->ui_line[0]  = 0xffffffff;
		ExpTSetting->ui_pixel[0] = 0xffffffff;

		AE_clrFlag(Id, AE_FLAG_SET_EXPT);
	}
}

void AE_setGain(UINT32 Id, GAIN_SETTING *Gain)
{
	if ((AE_getFlag(Id) & AE_FLAG_SET_GAIN) == AE_FLAG_SET_GAIN) {
		if ((Gain->data1[0] == 0xffffffff) && (Gain->data2[0] == 0xffffffff) && (Gain->data3[0] == 0xffffffff)) {
			DBG_MSG("AGC Gain Value Invalid\r\n");
		} else
			//Sensor_SetGain(IPL_UTI_CONV2_SEN_ID(Id) ,Gain);
		{
			sensor_set_info(Id, SEN_SET_GAIN, (void *)Gain);
		}

		Gain->data1[0] = 0xffffffff;
		Gain->data2[0] = 0xffffffff;
		Gain->data3[0] = 0xffffffff;

		AE_clrFlag(Id, AE_FLAG_SET_GAIN);
	}
}

BOOL AE_Wait_Stable(UINT32 Id, UINT32 StableCnt, UINT32 TimeOutFrame)
{
	UINT32 TimeOutCnt = 0;

	while (TimeOutCnt < TimeOutFrame) {
		if (AeInfo[Id].Counter > StableCnt) {
			return TRUE;
		} else {
			TimeOutCnt ++;
		}

		IPL_Wait_Vd(Id, FALSE, AE_TIMEOUT_MS);
	}

	DBG_MSG("AE Stable Time Out but not stable\r\n");
	return FALSE;
}

void AE_StartAFD(UINT32 Id)
{
	AEAFD_START[Id] = TRUE;
}

void AE_GetExifInfo(UINT32 Id, UINT32 *ISO, UINT32 *ExpT, UINT32 *Aperture, UINT32 *MaxApture)
{
	UINT32 ApertureValue;
	UINT32 ISOValue = 0, ExpTValue = 0;

	switch (CapNewAeArg[Id].Iris) {
	case AEALG_IRIS_0:
	default:
		ApertureValue = 18;
		break;

	case AEALG_IRIS_1:
		ApertureValue = 28;
		break;

	case AEALG_IRIS_2:
		ApertureValue = 40;
		break;

	case AEALG_IRIS_3:
		ApertureValue = 56;
		break;

	case AEALG_IRIS_4:
		ApertureValue = 80;
		break;
	}

    ExpTValue = CapNewAeArg[Id].ExpoTime;

	if (ExpTValue == 0) {
		ExpTValue = 1;
	}

    if(AeCurveGenPhoto[AeInfo->Id].iso_mode == AE_ISOMODE_PHOTO_SPEED) {
        ISOValue = ae_get_ui_info((IPL_PROC_ID)Id, AE_UI_ISO);
    } else {
        ISOValue = CapNewAeArg[Id].ISOGain;
    }

#if 0
    DBG_DUMP("=== Exif info ===\r\n");
    DBG_DUMP("ISO mode = %d\r\n", AeCurveGenInfo[AeInfo->Id].iso_mode);
    DBG_DUMP("ExpT Value = %d\r\n", ExpTValue);
    DBG_DUMP("ISO Value  = %d\r\n", ISOValue);
#endif

	*ExpT = ExpTValue;
	*ISO = ISOValue;
	*Aperture = ApertureValue;
	*MaxApture = 18;
}

UINT32 AEB_GetRatioFromEV(UINT32 Id, AE_EV AEB_Step)
{
	AEB_Ratio[Id] = EVTable[AEB_Step];
	return AEB_Ratio[Id];
}

void AEB_SetRatio2Cap(UINT32 Id, UINT32 Ratio)
{

	AeInfo[Id].CapParam.EVValue = AeInfo[Id].PrvParam[0].EVValue * AE_EV_RATIO / Ratio;

	AEAlg_Allot(Id, AeInfo[Id].CapParam.EVValue, AeInfo[Id].CapBiningMode, AeInfo[Id].CapCurve, AeInfo[Id].EventTab.GetFnoRatio, AeInfo[Id].EventTab.GetCurveOfsRatio, &CapNewAeArg[Id]);
}


AE_SETIRCUT_FP AE_fpSetIRCut = NULL;
void AE_SetIRCutFP(AE_SETIRCUT_FP fp)
{
	AE_fpSetIRCut = fp;
}

void AE_SetIRCut(UINT32 Id, UINT32 lv)
{
	static UINT32 delay_cnt_h = 0, delay_cnt_l = 0;
	static UINT32 cur_status = 0xff;
	//#NT#2016/08/12#XL_Wei -begin
	//#NT#Use perf_getCurrent to get accurate time
	static UINT32 lastT = 0;
	UINT32 curT;
	curT = Perf_GetCurrent();
	// ignore first run which can't get accurate process time
	if (lastT == 0) {
		lastT = curT;
	}

	if (AE_fpSetIRCut != NULL) {
		//#NT#2016/08/11#XL_Wei -begin
		//#NT#Update ircut mechanism, use time as condition
		if (lv > AE_IRCUT_LV_H && cur_status != ENABLE) {
			delay_cnt_h += curT - lastT;
			delay_cnt_l = 0;
			if (delay_cnt_h > AE_IRCUT_DELAY_TIME) {
				cur_status = ENABLE;
				(AE_fpSetIRCut)(Id, ENABLE);
				IR_CUT_ENABLE[Id] = TRUE;
			}
		} else if (lv < AE_IRCUT_LV_L && cur_status != DISABLE) {
			delay_cnt_l += curT - lastT;
			delay_cnt_h = 0;
			if (delay_cnt_l > AE_IRCUT_DELAY_TIME) {
				cur_status = DISABLE;
				(AE_fpSetIRCut)(Id, DISABLE);
				IR_CUT_ENABLE[Id] = FALSE;
			}
		}
		//#NT#2016/08/11#XL_Wei -end
		else {
			delay_cnt_l = 0;
			delay_cnt_h = 0;
		}
	} else {
		IR_CUT_ENABLE[Id] = FALSE;
	}

	lastT = curT;
	//#NT#2016/08/12#XL_Wei -end
}

void ae_dciris_init(UINT32 id)
{
    UINT32 clk_div;    // WATTIS
    PWM_CFG pwm_cfg = { 200, 0, 100, 0, 0 };    // uiPrd, uiRise, uiFall, uiOnCycle (0 ~ 65535), uiInv    WATTIS
    UINT32 uiPwmOutputPin = 0xFFFFFFFF;

    AeIrisInfo[id].exposure_freeze = 0;

    uiPwmOutputPin = ae_param_info[id]->iris_cfg->pwm_id;

    if (uiPwmOutputPin > 0) {

        if (pwm_open(uiPwmOutputPin) != E_OK) {
            DBG_ERR("fail to request PWM%u!\r\n", uiPwmOutputPin);
        }

        AeIrisInfo[id].pwm_id = uiPwmOutputPin;
        AeIrisInfo[id].drv_ratio = ae_param_info[id]->iris_cfg->balance_ratio;
        AeIrisInfo[id].drv_max = ae_param_info[id]->iris_cfg->drv_max;
        AeIrisInfo[id].probe_balance = ae_param_info[id]->iris_cfg->probe_balance;
        AeIrisInfo[id].balance_ratio = ae_param_info[id]->iris_cfg->balance_ratio;
        AeIrisInfo[id].driving_ratio = ae_param_info[id]->iris_cfg->driving_ratio;
        AeIrisInfo[id].iris_ctrl.kd = ae_param_info[id]->iris_cfg->iris_ctrl.kd;
        AeIrisInfo[id].iris_ctrl.ki = ae_param_info[id]->iris_cfg->iris_ctrl.ki;
        AeIrisInfo[id].iris_ctrl.kp = ae_param_info[id]->iris_cfg->iris_ctrl.kp;

        // output freq = 120M / (clk_div+1) / base_period = uiPwmFreq (30 kHz)
        clk_div = 120000000 / 200 / 30000 - 1;    // = 19
        if ((uiPwmOutputPin >= PWMID_0) && (uiPwmOutputPin <= PWMID_3)) {
            pwm_pwmConfigClockDiv(PWM0_3_CLKDIV, clk_div);
        } else if ((uiPwmOutputPin >= PWMID_4) && (uiPwmOutputPin <= PWMID_7)) {
            pwm_pwmConfigClockDiv(PWM4_7_CLKDIV, clk_div);
        } else if ((uiPwmOutputPin >= PWMID_8) && (uiPwmOutputPin <= PWMID_11)) {
            pwm_pwmConfigClockDiv(PWM8_11_CLKDIV, clk_div);
        } else if (uiPwmOutputPin == PWMID_12) {
            pwm_pwmConfigClockDiv(PWM12_CLKDIV, clk_div);
        } else if (uiPwmOutputPin == PWMID_13) {
            pwm_pwmConfigClockDiv(PWM13_CLKDIV, clk_div);
        } else if (uiPwmOutputPin == PWMID_14) {
            pwm_pwmConfigClockDiv(PWM14_CLKDIV, clk_div);
        } else if (uiPwmOutputPin == PWMID_15) {
            pwm_pwmConfigClockDiv(PWM15_CLKDIV, clk_div);
        }

        pwm_cfg.uiFall = ae_param_info[id]->iris_cfg->balance_ratio;    // 1 ~ 200 (0.5% ~ 100.0%)
        pwm_pwmConfig(uiPwmOutputPin, &pwm_cfg);
        pwm_pwmEnable(uiPwmOutputPin);
    }
}

void ae_dciris_ctrl(UINT32 id, UINT32 drv_ratio, UINT32 max_drv)
{
    UINT32 uiPwmOutputPin = 0xFFFFFFFF;

    drv_ratio = ((drv_ratio < max_drv) ? drv_ratio : max_drv);

    uiPwmOutputPin = ae_param_info[id]->iris_cfg->pwm_id;

    if (uiPwmOutputPin > 0) {
        pwm_pwmReloadConfig(uiPwmOutputPin, -1, drv_ratio, -1);
        pwm_pwmReload(uiPwmOutputPin);
    }
}

SHDR_EV_RATIO_INFO ae_get_shdr_ev_ratio(UINT32 id)
{
    UINT32 i;
    SHDR_EV_RATIO_INFO shdr_ev_ratio;

    for(i=0; i<SEN_MFRAME_MAX_NUM; i++)
        shdr_ev_ratio.uiEVratio[i] = ae_param_info[id]->curve_gen_param->hdr_ratio[i];

    return shdr_ev_ratio;
}

void AE_DumpWindow(UINT32 id)
{
    UINT32 i;

    DBG_DUMP("AE Weight Window\r\n");

    for(i=0; i<AE_WIN_NUM; i++) {

        DBG_DUMP("%d, ", ae_param_info[id]->meter_win[i]);

        if(((i+1)%AE_WIN_X)==0)
            DBG_DUMP("\r\n");
    }
}

void AE_DumpCurvePrv(UINT32 id, AE_GEN_INFO *ae_curve_info, UINT32(*AeCurve)[AEALG_INFO_MAX])
{
    UINT32 i, hdr_ratio_size;
    UINT32 curve_cnt;

    hdr_ratio_size = sizeof(ae_curve_info->hdr_ratio)/sizeof(UINT32);

	DBG_DUMP("^C---------- [Preview] Curve Info (Id = %d) ---------\r\n", id);

    if(ae_param_info[id]->curve_gen_param->fno_sel==FNO_SEL_PREDEF) {
        DBG_DUMP("^CFnoSel = FNO_SEL_PREDEF\r\n");
        DBG_DUMP("^CFnoIdx = %d\r\n", ae_curve_info->fno_idx);
        DBG_DUMP("^CFnoRat = %d\r\n", ae_curve_info->fno_rat);
    }
    else {
        DBG_DUMP("^CFnoSel = FNO_SEL_USER\r\n");
        DBG_DUMP("^CFnoRat = %d\r\n", ae_curve_info->fno_rat);
    }

    for(i=0; i<ae_curve_info->node_num; i++)
        DBG_DUMP("^CGen Node %d = %d, %d\r\n", i, ae_curve_info->node[i].expt, ae_curve_info->node[i].iso);

    DBG_DUMP("^CISO_Max = %d\r\n", ae_curve_info->iso_max);
    DBG_DUMP("^CAE_GEN_FREQ = %d\r\n", ae_curve_info->freq);
    DBG_DUMP("^CVideoFps = %d\r\n", ae_curve_info->frame_rate);

    if(ae_curve_info->iso_mode == AE_ISOMODE_MOV_PARAM)
        DBG_DUMP("^CISO Mode = AE_ISOMODE_MOV_PARAM\r\n");
    else if(ae_curve_info->iso_mode == AE_ISOMODE_MOV_UIMAX)
        DBG_DUMP("^CISO Mode = AE_ISOMODE_MOV_UIMAX\r\n");
    else
        DBG_ERR("ISO Mode = UNKNOWN MODE\r\n");

    DBG_DUMP("^CAuto Low Light = %d\r\n", ae_curve_info->auto_lowlight_en);

    for(i=0; i<hdr_ratio_size; i++)
        DBG_DUMP("^CHDR_Ratio %d = %d\r\n", i, ae_curve_info->hdr_ratio[i]);

    for(i=0; i<(sizeof(ae_curve_info->ext_fps)/sizeof(ae_curve_info->ext_fps[0])); i++)
        DBG_DUMP("^CExtFPS %d : %d -> %d\r\n", i, ae_curve_info->ext_fps[i].sensor_fps, ae_curve_info->ext_fps[i].extend_fps);

    DBG_DUMP("^CISO_Value = %d\r\n", ae_curve_info->iso_value);
    DBG_DUMP("^CScene_Mode = %d\r\n", ae_curve_info->scene_mode);

    DBG_DUMP("^CDCIris_En = %d\r\n", ae_curve_info->iris_en);
    DBG_DUMP("^CDCIris_ExpT_Min = %d\r\n", ae_curve_info->iris_min_expt);

	DBG_DUMP("^C------------------------------------\r\n");

	DBG_DUMP("^C--------- AE Curve Start -----------\r\n");

    curve_cnt= 0;
    for(i=0; i<AE_GEN_CURVE_NODE_MAX; i++){
        curve_cnt++;
        if(AeCurve[i][AEALG_EV] == 0xffffffff)
            break;
    }

	for (i = 0; i < curve_cnt; i ++) {
        if(AeCurve[i][AEALG_EV]==0xFFFFFFFF)
		    DBG_DUMP("^C%10d\t%d\t%d\t%d\t", AeCurve[i][AEALG_EV], AeCurve[i][AEALG_EXP], AeCurve[i][AEALG_ISO], AeCurve[i][AEALG_IRIS], AeCurve[i][AEALG_ADJ_TARGET]);
        else
            DBG_DUMP("^C%10u\t%d\t%d\t%d\t", AeCurve[i][AEALG_EV], AeCurve[i][AEALG_EXP], AeCurve[i][AEALG_ISO], AeCurve[i][AEALG_IRIS], AeCurve[i][AEALG_ADJ_TARGET]);

        switch(AeCurve[i][AEALG_ADJ_TARGET]) {
        case AEALG_ADJ_EXP:
            DBG_DUMP("^CADJ_EXP\r\n");
            break;
        case AEALG_ADJ_GAIN:
            DBG_DUMP("^CADJ_ISO\r\n");
            break;
        case AEALG_ADJ_IRIS:
            DBG_DUMP("^CADJ_IRIS\r\n");
            break;
        default:
            DBG_DUMP("^C\r\n");
            break;
        }
	}

	DBG_DUMP("^C---------- [Preview] Curve Info End ---------\r\n");
    DBG_DUMP("^C[Preview] EV Range : %d <--> %d\r\n\r\n", AeCurve[0][AEALG_EV], AeCurve[curve_cnt-3][AEALG_EV]);
}

void AE_DumpCurveCap(UINT32 id, AE_GEN_PHOTO *ae_curve_photo, UINT32(*AeCurve)[AEALG_INFO_MAX])
{
    UINT32 i;
    UINT32 curve_cnt;

	DBG_DUMP("^Y---------- [Capture] Curve Info (Id = %d) ---------\r\n", id);

    DBG_DUMP("^YFnoRat = %d\r\n", ae_curve_photo->fno_rat);

    for(i=0; i<ae_curve_photo->node_num; i++)
        DBG_DUMP("^YGen Node %d = %d, %d\r\n", i, ae_curve_photo->node[i].expt, ae_curve_photo->node[i].iso);

    DBG_DUMP("^YISO_Max = %d\r\n", ae_curve_photo->iso_max);
    DBG_DUMP("^YAE_GEN_FREQ = %d\r\n", ae_curve_photo->freq);

    if(ae_curve_photo->iso_mode == AE_ISOMODE_PHOTO_PARAM)
        DBG_DUMP("^YISO Mode = AE_ISOMODE_PHOTO_PARAM\r\n");
    else if(ae_curve_photo->iso_mode == AE_ISOMODE_PHOTO_SPEED)
        DBG_DUMP("^YISO Mode = AE_ISOMODE_PHOTO_SPEED\r\n");
    else if(ae_curve_photo->iso_mode == AE_ISOMODE_PHOTO_UIMAX)
        DBG_DUMP("^YISO Mode = AE_ISOMODE_PHOTO_UIMAX\r\n");
    else
        DBG_ERR("ISO Mode = UNKNOWN MODE\r\n");

    DBG_DUMP("^YISO_Mode_Value = %d\r\n", ae_curve_photo->iso_value);
    DBG_DUMP("^YScene_Mode = %d\r\n", ae_curve_photo->scene_mode);

    DBG_DUMP("^YDCIris_En = %d\r\n", ae_curve_photo->iris_en);
    DBG_DUMP("^YDCIris_ExpT_Min = %d\r\n", ae_curve_photo->iris_min_expt);

	DBG_DUMP("^Y------------------------------------\r\n");

	DBG_DUMP("^Y--------- AE Curve Start -----------\r\n");

    curve_cnt= 0;
    for(i=0; i<AE_GEN_CURVE_NODE_MAX; i++){
        curve_cnt++;
        if(AeCurve[i][AEALG_EV] == 0xffffffff)
            break;
    }

	for (i = 0; i < curve_cnt; i ++) {
        if(AeCurve[i][AEALG_EV]==0xFFFFFFFF)
		    DBG_DUMP("^Y%10d\t%d\t%d\t%d\t", AeCurve[i][AEALG_EV], AeCurve[i][AEALG_EXP], AeCurve[i][AEALG_ISO], AeCurve[i][AEALG_IRIS], AeCurve[i][AEALG_ADJ_TARGET]);
        else
            DBG_DUMP("^Y%10u\t%d\t%d\t%d\t", AeCurve[i][AEALG_EV], AeCurve[i][AEALG_EXP], AeCurve[i][AEALG_ISO], AeCurve[i][AEALG_IRIS], AeCurve[i][AEALG_ADJ_TARGET]);

        switch(AeCurve[i][AEALG_ADJ_TARGET]) {
        case AEALG_ADJ_EXP:
            DBG_DUMP("^YADJ_EXP\r\n");
            break;
        case AEALG_ADJ_GAIN:
            DBG_DUMP("^YADJ_ISO\r\n");
            break;
        case AEALG_ADJ_IRIS:
            DBG_DUMP("^YADJ_IRIS\r\n");
            break;
        default:
            DBG_DUMP("^Y\r\n");
            break;
        }
	}

	DBG_DUMP("^Y---------- [Capture] Curve Info End ---------\r\n");
    DBG_DUMP("^Y[Capture] EV Range : %d <--> %d\r\n\r\n", AeCurve[0][AEALG_EV], AeCurve[curve_cnt-3][AEALG_EV]);
}

void AE_DumpCurveInfo(UINT32 id, AE_CURVEDUMP_TYPE dump_type)
{
#if 1
    if((dump_type==AE_CURVEDUMP_PRV) || (dump_type==AE_CURVEDUMP_BOTH)) {
        AE_DumpCurvePrv(id, &AeCurveGenInfo[id], AeAutoCurveTab_Prv[id]);
    }

    if((dump_type==AE_CURVEDUMP_CAP) || (dump_type==AE_CURVEDUMP_BOTH)) {
        AE_DumpCurveCap(id, &AeCurveGenPhoto[id], AeAutoCurveTab_Cap[id]);
    }
#endif
}

UINT32 AE_AlgGetUIInfo(AE_PROC_ID id, AE_SEL_ITEM sel_idx)
{
    UINT32 i, value = 0;
    UINT32 ratio, diff_ratio;
	IPL_MODE_INFOR Info = {0};
    ER rt = E_OK;

    switch(sel_idx){
    case AE_SEL_EXPY:
        value = AeInfo[id].PrvExpectLum.y;
        break;
    case AE_SEL_PRV_LV:
    	value = AeInfo[id].PrvAvgEV.LV;
        break;
    case AE_SEL_PRV_EV:
    	value = AeInfo[id].PrvAvgEV.EVValue;
        break;
    case AE_SEL_CAP_LV:
    	value = AeInfo[id].CapParam.LV;
        break;
    case AE_SEL_CAP_EV:
	    value = AeInfo[id].CapParam.EVValue;
        break;
    case AE_SEL_CURR_Y:
        value = AeInfo[id].PrvAvgLum.y;
        break;
    case AE_SEL_CURR_EXPY:
        value = AeInfo[id].CurExpectLum.y;
        break;
    case AE_SEL_CURR_RAW:
        value = AeInfo[id].PrvAvgLum.raw;
        break;
    case AE_SEL_TARGET_RAW:
        value = AeInfo[id].CurExpectLum.raw;
        break;
    case AE_SEL_EXPOTIME:
        value = PrvNewAeArg[id][AEStep[id]].ExpoTime;
        break;
    case AE_SEL_ISOGAIN:
        value = PrvNewAeArg[id][AEStep[id]].ISOGain;
        break;
    case AE_SEL_STABLE_CNT:
        value = AeInfo[id].Counter;
        break;
    case AE_SEL_FNO:
        value = AeCurveGenInfo[AeInfo->Id].fno_rat;
        break;
    case AE_SEL_LA_AVG_Y:
        value = AeInfo[id].LA_Avg_Y;
        break;
    case AE_SEL_ADJ_STATUS:
        if(AeInfo[id].CurExpectLum.y < 1)
            AeInfo[id].CurExpectLum.y = 1;
        ratio = (AeInfo[id].PrvAvgLum.y * 100)/ AeInfo[id].CurExpectLum.y;
        diff_ratio = (ratio > 100) ? (ratio - 100) : (100 - ratio);
        if(diff_ratio < 10) // < 10%
            value = 0;
        else if(diff_ratio < 100) // < 200%
            value = 1;
        else
            value = 2;
        break;
    case AE_SEL_MINFPS:
        for(i=0; i<(sizeof(AeCurveGenInfo[id].ext_fps)/sizeof(AeCurveGenInfo[id].ext_fps[0])); i++){
            if(AeCurveGenInfo[id].frame_rate >= AeCurveGenInfo[id].ext_fps[i].sensor_fps){
                value = AeCurveGenInfo[id].ext_fps[i].extend_fps;
                break;
            }
        }
        break;
    case AE_SEL_ALL_EN:
        value = AeCurveGenInfo[id].auto_lowlight_en;
        break;
    case AE_SEL_DBG_EXPT:
        break;
    case AE_SEL_DBG_ISO:
        break;
    case AE_SEL_OVEREXP_ADJ:
        value = AeInfo[id].ExtSet.OverExp.CurTotalStep;
        break;
    case AE_SEL_OVEREXP_CNT:
        value = AeInfo[id].ExtSet.OverExp.CurCnt;
        break;
    case AE_SEL_ISOMAX:
        value = ae_param_info[id]->curve_gen_param->iso_max;
        break;
    case AE_SEL_ISP_GAIN:
        value = AeIspGain[id];
        break;
    case AE_SEL_ISP_GAIN_THRES:
        value = ae_param_info[id]->curve_gen_param->isp_gain_thres;
        break;
    case AE_SEL_HDR_EV_OPT:
        value = ae_param_info[id]->shdr->ev_opt;
        break;
    case AE_SEL_HDR_SE_EXPY:
        value = ae_param_info[id]->shdr->expy_se;
        break;
    case AE_SEL_HDR_SE_CLAMP_L:
        value = ae_param_info[id]->shdr->clamp.l;
        break;
    case AE_SEL_HDR_SE_CLAMP_H:
        value = ae_param_info[id]->shdr->clamp.h;
        break;
    case AE_SEL_HDR_SE_SMOOTH:
        value = ae_param_info[id]->shdr->smooth;
        break;
	case AE_SEL_HDR_EXPT:
		value = (UINT32)&ae_shdr_trans[id].expo_time[0];
		break;
	case AE_SEL_HDR_ISO:
		value = (UINT32)&ae_shdr_trans[id].iso_gain[0];
		break;
    case AE_SEL_HDR_SE_ISO_UB:
        value = ae_shdr_trans[id].se_iso_ub;
        break;
    case AE_SEL_EV_COMP:
        value = AeInfo[id].EVCompIdx;
        break;
    case AE_SEL_FLICKER_MODE:
        value = (UINT32)ae_param_info[id]->curve_gen_param->flicker_mode;
        break;
    case AE_SEL_SLOWSHOOT_CYC:
        value = (UINT32)ae_param_info[id]->convergence->slowshoot_cyc;
        break;
    case AE_SEL_SLOWSHOOT_THR:
        value = (UINT32)ae_param_info[id]->convergence->slowshoot_thr;
        break;
    case AE_SEL_FULLRANGE_EV:
        value = (AeInfo[id].PrvAvgEV.EVValue*AeInfo[id].PrvAvgLum.y)/AeInfo[id].CurExpectLum.y;
        break;
    case AE_SEL_FULLRANGE_LV:
        value = (AeInfo[id].PrvAvgEV.EVValue*AeInfo[id].PrvAvgLum.y)/AeInfo[id].CurExpectLum.y;
        value = AEAlg_Conv2LV(value);
        break;
    case AE_SEL_ISOMODE:
    	Info.id = id;
    	rt = IPL_GetCmd(IPL_GET_CUR_MODE, (void *)&Info);

        if(rt == E_OK) {
            if(Info.mode == IPL_MODE_PREVIEW){
                value = (UINT32)AeCurveGenPhoto[AeInfo->Id].iso_mode;
            } else {
                value = (UINT32)AeCurveGenInfo[AeInfo->Id].iso_mode;
            }
        } else {
            DBG_ERR("Get IPL mode error!\r\n");
        }
        break;
    default:
        DBG_ERR("Id = %d, SelIdx = %d error\r\n", id, sel_idx);
        break;
    };

    return value;
}

void AE_AlgSetUIInfo(AE_PROC_ID id, AE_SEL_ITEM sel_idx, UINT32 value)
{
    UINT32 i;
	IPL_MODE_INFOR Info = {0};
    ER rt = E_OK;

    switch(sel_idx){
    case AE_SEL_EXPY:
        if (IPL_AlgGetUIInfo((IPL_PROC_ID)id, IPL_SEL_SHDR) == SEL_SHDR_ON){
            ae_param_info[id]->shdr->expy_le = value;
        }

		AeInfo[id].PrvExpectLum.y = value;

        if(ae_param_info[id]->lum_gamma->type==AE_LUMGAMMA_IPP)
        	AeInfo[id].PrvExpectLum.raw = iq_get_inv_gamma_value(id, AeInfo[id].PrvExpectLum.y);
        else
            AeInfo[id].PrvExpectLum.raw = AE_Get_InvLumGammaValue(id, AeInfo[id].PrvExpectLum.y);

        if(AeInfo[id].PrvExpectLum.y < 1)
            AeInfo[id].PrvExpectLum.y = 1;

        if(AeInfo[id].PrvExpectLum.raw < 1)
            AeInfo[id].PrvExpectLum.raw = 1;

        AeInfo[id].Reset = TRUE;
        break;
    case AE_SEL_FNO:
        ae_param_info[AeInfo->Id]->curve_gen_param->fno_rat = value;
    	//AeCurveGenInfo[AeInfo->Id].fno_rat = value;
        AE_CurveUpdate(id);
        break;
    case AE_SEL_CLAMP_RATIO:
        for(i=0; i<AEALG_DYNAMIC_LV_NUM; i++)
            ae_param_info[id]->la_clamp->tab_shdr_h[i] = ((AE_ClampRatio[id][i]*value)/100);
        break;
    case AE_SEL_MINFPS:
        for(i=0; i<(sizeof(ae_param_info[id]->curve_gen_param->ext_fps)/sizeof(ae_param_info[id]->curve_gen_param->ext_fps[0])); i++){
            if(AeCurveGenInfo[id].frame_rate >= AeCurveGenInfo[id].ext_fps[i].sensor_fps){
                ae_param_info[id]->curve_gen_param->ext_fps[i].extend_fps = value;
                AE_CurveUpdate(id);
                break;
            }
        }
        break;
    case AE_SEL_ALL_EN:
        ae_param_info[id]->curve_gen_param->auto_lowlight_en = (value==0) ? DISABLE : ENABLE;
        AE_CurveUpdate(id);
        break;
    case AE_SEL_DBG_EXPT:
        AeCurveGenInfo[id].dbg_expt = value;
        AE_CurveUpdate(id);
        break;
    case AE_SEL_DBG_ISO:
        AeCurveGenInfo[id].dbg_iso = value;
        AE_CurveUpdate(id);
        break;
    case AE_SEL_ISOMAX:
        ae_param_info[id]->curve_gen_param->iso_max = value;
        AE_CurveUpdate(id);
        break;
    case AE_SEL_ISP_GAIN:
        AeIspGain[id] = value;
        break;
    case AE_SEL_ISP_GAIN_THRES:
        ae_param_info[id]->curve_gen_param->isp_gain_thres = value;
        break;
    case AE_SEL_HDR_UART:
        AE_HDR_Msg = value;
        break;
    case AE_SEL_HDR_EV_OPT:
        ae_param_info[id]->shdr->ev_opt = value;
        break;
    case AE_SEL_HDR_SE_EXPY:
        ae_param_info[id]->shdr->expy_se = value;
        break;
    case AE_SEL_HDR_SE_CLAMP_L:
        ae_param_info[id]->shdr->clamp.l = value;
        break;
    case AE_SEL_HDR_SE_CLAMP_H:
        ae_param_info[id]->shdr->clamp.h = value;
        break;
    case AE_SEL_HDR_SE_SMOOTH:
        ae_param_info[id]->shdr->smooth = value;
        break;
    case AE_SEL_HDR_SE_ISO_UB:
        ae_shdr_trans[id].se_iso_ub = value;
        ae_param_info[id]->shdr->se_iso_ub = value;
        break;
    case AE_SEL_EV_COMP:
        AeInfo[id].EVCompIdx = value;
        break;
    case AE_SEL_FLICKER_MODE:
        ae_param_info[id]->curve_gen_param->flicker_mode = (AE_FLICKER_MODE)value;
        AE_CurveUpdate(id);
        break;
    case AE_SEL_SLOWSHOOT_CYC:
        ae_param_info[id]->convergence->slowshoot_cyc = value;
        break;
    case AE_SEL_SLOWSHOOT_THR:
        ae_param_info[id]->convergence->slowshoot_thr = value;
        break;
    case AE_SEL_ISOMODE:
    	Info.id = id;
    	rt = IPL_GetCmd(IPL_GET_CUR_MODE, (void *)&Info);

        AE_ISOMODE_PHOTO_TYPE photo_iso_mode;
        AE_ISOMODE_MOV_TYPE movie_iso_mode;

        photo_iso_mode = (AE_ISOMODE_PHOTO_TYPE)value;
        movie_iso_mode = (AE_ISOMODE_MOV_TYPE)value;

        if(rt == E_OK) {
            if(Info.mode == IPL_MODE_PREVIEW){
                if(photo_iso_mode >= AE_ISOMODE_PHOTO_MAXCNT)
                    DBG_ERR("Unsupport iso mode!\r\n");
                else {
                    ae_param_info[id]->curve_gen_photo->iso_mode = photo_iso_mode;
                    AeCurveGenPhoto[id].iso_mode = photo_iso_mode;
                }
            } else {
                if(movie_iso_mode >= AE_ISOMODE_MOV_MAXCNT)
                    DBG_ERR("Unsupport iso mode!\r\n");
                else {
                    ae_param_info[id]->curve_gen_param->iso_mode = movie_iso_mode;
                    AeCurveGenInfo[id].iso_mode = movie_iso_mode;
                }
            }
        } else {
            DBG_ERR("Get IPL mode error!\r\n");
        }

        AE_CurveUpdate(id);
        break;

    case AE_SEL_DCIRIS_EN:
        ae_param_info[id]->iris_cfg->enable = value;
        AeCurveGenInfo[id].iris_en = value;
        ae_dciris_ctrl(id, 0, AeIrisInfo[id].drv_max);
        AE_CurveUpdate(id);
        break;

    default:
        DBG_ERR("Id = %d, SelIdx = %d error\r\n", id, sel_idx);
        break;
    }
}

void AE_UartProc(UINT32 Id)
{
    UINT32 i;
    UINT32 dbg_ev_ratio[SEN_MFRAME_MAX_NUM] = {100, 100, 100, 100};

    switch(uiAEUartCmd[Id]) {
    case AE_UART_ALL:
		DBG_DUMP("%d: %4d %4d %4d %4d %4d %10u %8d %6d %4d %4d %4d %4d %4d %4d\r\n", Id,
                                            AE_AlgGetUIInfo((AE_PROC_ID)Id, AE_SEL_PRV_LV),
                                            AE_AlgGetUIInfo((AE_PROC_ID)Id, AE_SEL_CURR_Y),
                                            AE_AlgGetUIInfo((AE_PROC_ID)Id, AE_SEL_CURR_EXPY),
                                            AE_AlgGetUIInfo((AE_PROC_ID)Id, AE_SEL_CURR_RAW),
                                            AE_AlgGetUIInfo((AE_PROC_ID)Id, AE_SEL_TARGET_RAW),
                                            AE_AlgGetUIInfo((AE_PROC_ID)Id, AE_SEL_PRV_EV),
                                            AE_AlgGetUIInfo((AE_PROC_ID)Id, AE_SEL_EXPOTIME),
                                            AE_AlgGetUIInfo((AE_PROC_ID)Id, AE_SEL_ISOGAIN),
                                            IPL_AlgGetUIInfo((IPL_PROC_ID)Id, IPL_SEL_WDR_IMM),
                                            AE_AlgGetUIInfo((AE_PROC_ID)Id, AE_SEL_STABLE_CNT),
                                            AE_AlgGetUIInfo((AE_PROC_ID)Id, AE_SEL_OVEREXP_CNT),
                                            AE_AlgGetUIInfo((AE_PROC_ID)Id, AE_SEL_OVEREXP_ADJ),
                                            AE_AlgGetUIInfo((AE_PROC_ID)Id, AE_SEL_ISP_GAIN),
                                            AEStep[Id]);
        break;
    case AE_UART_SIMPLE:
        DBG_DUMP("[SIMPLE] %d: %4d %4d %4d %8d %6d %4d %4d\r\n", Id,
                                            AE_AlgGetUIInfo((AE_PROC_ID)Id, AE_SEL_PRV_LV),
                                            AE_AlgGetUIInfo((AE_PROC_ID)Id, AE_SEL_CURR_Y),
                                            AE_AlgGetUIInfo((AE_PROC_ID)Id, AE_SEL_CURR_EXPY),
                                            AE_AlgGetUIInfo((AE_PROC_ID)Id, AE_SEL_EXPOTIME),
                                            AE_AlgGetUIInfo((AE_PROC_ID)Id, AE_SEL_ISOGAIN),
                                            AE_AlgGetUIInfo((AE_PROC_ID)Id, AE_SEL_ISP_GAIN),
                                            AE_AlgGetUIInfo((AE_PROC_ID)Id, AE_SEL_STABLE_CNT));
        break;
    case AE_UART_OVEREXP:
        DBG_DUMP("[OVEREXP] %d: %4d %4d %4d %8d %6d %4d %4d %4d\r\n", Id,
                                            AE_AlgGetUIInfo((AE_PROC_ID)Id, AE_SEL_PRV_LV),
                                            AE_AlgGetUIInfo((AE_PROC_ID)Id, AE_SEL_CURR_Y),
                                            AE_AlgGetUIInfo((AE_PROC_ID)Id, AE_SEL_CURR_EXPY),
                                            AE_AlgGetUIInfo((AE_PROC_ID)Id, AE_SEL_EXPOTIME),
                                            AE_AlgGetUIInfo((AE_PROC_ID)Id, AE_SEL_ISOGAIN),
                                            AE_AlgGetUIInfo((AE_PROC_ID)Id, AE_SEL_OVEREXP_CNT),
                                            AE_AlgGetUIInfo((AE_PROC_ID)Id, AE_SEL_OVEREXP_ADJ),
                                            AE_AlgGetUIInfo((AE_PROC_ID)Id, AE_SEL_STABLE_CNT));
        break;
    case AE_UART_LUM_INFO:
        DBG_DUMP("[LUM] %d: %4d %4d %4d %4d %10u %10u\r\n", Id,
                                            AE_AlgGetUIInfo((AE_PROC_ID)Id, AE_SEL_CURR_Y),
                                            AE_AlgGetUIInfo((AE_PROC_ID)Id, AE_SEL_CURR_EXPY),
                                            AE_AlgGetUIInfo((AE_PROC_ID)Id, AE_SEL_PRV_LV),
                                            AE_AlgGetUIInfo((AE_PROC_ID)Id, AE_SEL_FULLRANGE_LV),
                                            AE_AlgGetUIInfo((AE_PROC_ID)Id, AE_SEL_PRV_EV),
                                            AE_AlgGetUIInfo((AE_PROC_ID)Id, AE_SEL_FULLRANGE_EV));
        break;
    case AE_UART_HDR:
        if (IPL_AlgGetUIInfo((IPL_PROC_ID)Id, IPL_SEL_SHDR) == SEL_SHDR_ON){
            if(ae_param_info[Id]->shdr->ev_opt == AE_SHDR_EV_OPT_EXPY) {

                DBG_DUMP("[HDR] ExpY =%3d | SE_Y =%3d\r\n",
                                                            ae_param_info[Id]->shdr->expy_se,
                                                            ae_shdr_info[Id].curr_se_lum
                                                            );
                for(i=0; i<ae_shdr_trans[Id].shdr_frm; i++) {
                    dbg_ev_ratio[i] = (ae_shdr_trans[Id].expo_time[i]*ae_shdr_trans[Id].iso_gain[i]);
                    DBG_DUMP("[HDR_EXP_%d] %6d, %4d\r\n", i, ae_shdr_trans[Id].expo_time[i], ae_shdr_trans[Id].iso_gain[i]);
                }
            } else {
                for(i=0; i<ae_shdr_trans[Id].shdr_frm; i++) {
                    dbg_ev_ratio[i] = (ae_shdr_trans[Id].expo_time[i]*ae_shdr_trans[Id].iso_gain[i]);
                    DBG_DUMP("[HDR_EXP_%d] %6d, %4d\r\n", i, ae_shdr_trans[Id].expo_time[i], ae_shdr_trans[Id].iso_gain[i]);
                }
            }

            DBG_DUMP("[HDR_RATIO]");
            for(i=1; i<ae_shdr_trans[Id].shdr_frm; i++) {
                DBG_DUMP(" %4d", ((dbg_ev_ratio[0]*10)/dbg_ev_ratio[i]));
            }
            DBG_DUMP("\r\n\r\n");
        } else {
            DBG_DUMP("SHDR is not open!\r\n");
        }
        break;
    case AE_UART_IRIS:
        DBG_DUMP("[IRIS] %d: %4d %4d %4d %8d %6d %2d\r\n", Id,
                                            AE_AlgGetUIInfo((AE_PROC_ID)Id, AE_SEL_PRV_LV),
                                            AE_AlgGetUIInfo((AE_PROC_ID)Id, AE_SEL_CURR_Y),
                                            AE_AlgGetUIInfo((AE_PROC_ID)Id, AE_SEL_CURR_EXPY),
                                            AE_AlgGetUIInfo((AE_PROC_ID)Id, AE_SEL_EXPOTIME),
                                            AE_AlgGetUIInfo((AE_PROC_ID)Id, AE_SEL_ISOGAIN),
                                            AeIrisInfo[Id].exposure_freeze
                                            );
        break;
    default:
        DBG_DUMP("Not support command!\r\n");
        break;
    }
}

void AE_CurveUpdate(UINT32 id)
{
    AeCurveUpdate_Prv[id] = TRUE;
    AeCurveUpdate_Cap[id] = TRUE;
}
/*--------------------------------------------------------*/
