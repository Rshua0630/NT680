#ifndef _IQ_CTRL_H_
#define _IQ_CTRL_H_
/**
    IQS_Ctrl.h


    @file       iq_ctrl.h
    @ingroup    mILibIPLCom
    @note       Nothing (or anything need to be mentioned).

    Copyright   Novatek Microelectronics Corp. 2011.  All rights reserved.
*/
#include "Type.h"
#include "ErrorNo.h"
#include "ipl_utility.h"
#include "iq_param_ctrl.h"

/** \addtogroup IQ_Ctrl */
//@{
#define IQS_ENG_MASK    0x0000ffff
#define IQS_TBL_MASK    0xffff0000

#define IQS_ENG_SIE     0x00000001
#define IQS_ENG_PRE     0x00000002
#define IQS_ENG_DCE     0x00000004
#define IQS_ENG_IFE     0x00000008
#define IQS_ENG_IPE     0x00000010
#define IQS_ENG_IFE2    0x00000020
#define IQS_ENG_IME     0x00000040
#define IQS_ENG_RHE     0x00000080

#define IQS_TBL_GAMMA   0x00010000
#define IQS_TBL_3DCC    0x00020000
#define IQS_TBL_ECS     0x00040000
#define IQS_TBL_YCURVE  0x00080000
#define IQS_TBL_VIG     0x00100000

typedef ER(fpSIEx_IQPARAM)(UINT32 Id, IQS_FLOW_MSG MsgID, SIE_SUBFUNC *subf, IPL_HAL_GROUP *group);
typedef ER(fpRHE_IQPARAM)(UINT32 Id, IQS_FLOW_MSG MsgID, RHE_SUBFUNC *subf, IPL_HAL_GROUP *group);
typedef ER(fpIFE_IQPARAM)(UINT32 Id, IQS_FLOW_MSG MsgID, IFE_SUBFUNC *subf, IPL_HAL_GROUP *group);
typedef ER(fpDCE_IQPARAM)(UINT32 Id, IQS_FLOW_MSG MsgID, DCE_SUBFUNC *subf, IPL_HAL_GROUP *group);
typedef ER(fpIPE_IQPARAM)(UINT32 Id, IQS_FLOW_MSG MsgID, IPE_SUBFUNC *subf, IPL_HAL_GROUP *group);
typedef ER(fpIFE2_IQPARAM)(UINT32 Id, IQS_FLOW_MSG MsgID, IFE2_SUBFUNC *subf, IPL_HAL_GROUP *group);
typedef ER(fpIME_IQPARAM)(UINT32 Id, IQS_FLOW_MSG MsgID, IME_SUBFUNC *subf, IPL_HAL_GROUP *group);
typedef void(*fp_iqsparam_init)(UINT32 Id, UINT32 mode_idx);
typedef ER(*fq_iq_set_flow)(UINT32 Id, IQS_FLOW_MSG MsgID);

typedef struct {
	UINT32 rhe_param;
	UINT32 ipe_param;
} IQS_WDRparam;
extern ER IQS_SIEx_IQparam(UINT32 Id, IQS_FLOW_MSG MsgID, IPL_HAL_PARASET idx, fpSIEx_IQPARAM cb);
extern ER IQS_RHE_IQparam(UINT32 Id, IQS_FLOW_MSG MsgID, IPL_HAL_PARASET idx, fpRHE_IQPARAM cb);
extern ER IQS_IFE_IQparam(UINT32 Id, IQS_FLOW_MSG MsgID, IPL_HAL_PARASET idx, fpIFE_IQPARAM cb);
extern ER IQS_DCE_IQparam(UINT32 Id, IQS_FLOW_MSG MsgID, IPL_HAL_PARASET idx, fpDCE_IQPARAM cb);
extern ER IQS_IPE_IQparam(UINT32 Id, IQS_FLOW_MSG MsgID, IPL_HAL_PARASET idx, fpIPE_IQPARAM cb);
extern ER IQS_IFE2_IQparam(UINT32 Id, IQS_FLOW_MSG MsgID, IPL_HAL_PARASET idx, fpIFE2_IQPARAM cb);
extern ER IQS_IME_IQparam(UINT32 Id, IQS_FLOW_MSG MsgID, IPL_HAL_PARASET idx, fpIME_IQPARAM cb);
extern void IQS_SetUpdateBit(UINT32 Id, UINT32 UpdateBit);
extern void IQS_ClearUpdateBit(UINT32 Id);
extern UINT32 IQS_GetUpdateBit(UINT32 Id);
//#NT#2018/01/15#Photon Lin -begin
//#NT#Mark unused code 
#if 0
extern void IQS_SetIsrUpdateBit(UINT32 Id, UINT32 UpdateBit);
extern void IQS_ClearIsrUpdateBit(UINT32 Id);
extern UINT32 IQS_GetIsrUpdateBit(UINT32 Id);
#endif
//#NT#2018/01/15#Photon Lin -end
//#NT#2018/04/19#Photon Lin -begin
//Remove
//extern void IQS_SetRate(UINT32 rate);
//#NT#2018/04/19#Photon Lin -end
extern void IQS_Ctrl_Init(UINT32 Id, UINT32 CurrentStatus);
extern void IQS_Ctrl_Process(UINT32 Id, UINT32 CurrentStatus);
extern void IQS_Ctrl_ISR(UINT32 Id);
//#NT#2018/04/09#Photon Lin -begin
//#NT#Mark unused code 
//extern void IQS_Ctrl_cb1(fp_iqsparam_init func);
//#NT#2018/04/09#Photon Lin -end
extern void IQS_Ctrl_register_flow_cb(fq_iq_set_flow cb_func);
extern void IQS_Ctrl_Pause(UINT32 Id, UINT32 CurrentStatus);
//@}
#endif //_IQS_CTRL_H_
