#ifndef IPL_EXT_CTRL_FLOW_COMMON_INT_H_
#define IPL_EXT_CTRL_FLOW_COMMON_INT_H_

/**
    ipl_ext_ctrl_common_int.h


    @file       ipl_ext_ctrl_common_int.h
    @ingroup    mISYSAlg
    @note       Nothing (or anything need to be mentioned).

    Copyright   Novatek Microelectronics Corp. 2011.  All rights reserved.
*/
typedef struct {
	IPL_HAL_PARASET IPL_HalParamId[IPL_ID_MAX_NUM];
	fpEngInit fp_IFE2Init;
	fpEngSubFunc fp_AdaptSetting;
	fpEngCLK fp_SIEClk;
	fpSIEClkSrc fp_SIEClkSrc;
	fpSIEPclkPad fp_SIEPclkSel;
	fpEngCLK fp_RHEClk;
	fpEngCLK fp_IFEClk;
	fpEngCLK fp_DCEClk;
	fpEngCLK fp_IPEClk;
	fpEngCLK fp_IFE2Clk;
	fpEngCLK fp_IMEClk;
	fpEngInit fp_SieInit;
	fpEngInit fp_RheInit;
	fpEngInit fp_IfeInit;
	fpEngInit fp_DceInit;
	fpEngInit fp_IpeInit;
	fpEngInit fp_ImeInit;
} IPLCTRL_FLOW_INFO;

void IPL_CtrlPrvInitFlow_IMEPxOutInfo(UINT32 id, UINT32 path_en_idx, IPL_IME_PATH_INFO *path_info);
void IPL_CtrlInitIpcInfoFlowCommon(UINT32 id, IPC_INFO *ipc_info);
void IPL_CtrlInitIpcInfoFlowCCIR(UINT32 id, IPC_INFO *ipc_info);
void IPL_CtrlInitIpcInfoFlowVirtual(UINT32 id, IPC_INFO *ipc_info);
void IPL_CtrlInitbuf_infoFlowCommon(UINT32 id, IPLBUF_ENG2DRAMINFO *buf_info);

void IPL_CtrlPrvInitFlowCommon(UINT32 id, IPLCTRL_INFO *ctrl_info);
void IPL_CtrlPrvInitFlowCCIR(UINT32 id, IPLCTRL_INFO *ctrl_info);
void IPL_CtrlPrvInitFlowVirtual(UINT32 id, IPLCTRL_INFO *ctrl_info);
IPL_HAL_PARASET IPL_CtrlGetHalSetIdFlow(UINT32 id);
#endif //IPL_EXT_CTRL_FLOW_COMMON_INT_H_
