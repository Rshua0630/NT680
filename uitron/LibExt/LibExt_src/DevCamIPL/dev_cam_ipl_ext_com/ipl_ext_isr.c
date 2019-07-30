/**
    ISR sample.

    @file       ipl_ext_isr.c
    @ingroup    mISYSAlg
    @note       Nothing (or anything need to be mentioned).

    Copyright   Novatek Microelectronics Corp. 2011.  All rights reserved.
*/

#include "ipl_ext_common.h"
#include "dis_alg.h"

DISALG_PARAM ipl_disInfo = {0};
IPC_QINFO QInfo = {0};

void IPL_IsrSIECB(IPL_PROC_ID Id, UINT32 IntStatus, UINT32 idx)
{
	if (IntStatus & SIE_FLDEND) {
		if (idx > 1) {
			AE_TrigEvent(IPL_UTI_CONV2_AE_ID(Id), IntStatus);
			AWB_TrigEvent(IPL_UTI_CONV2_AWB_ID(Id));
		}
	}
	if (IntStatus & SIE_DRAM0_END) {
		if (idx > 1) {
            AE_TrigEvent(IPL_UTI_CONV2_AE_ID(Id), IntStatus);
        }
    }

	static UINT32 PrvAddr = 0, DIS_State = 0;

	if (IntStatus & SIE_DRAM4_END) {
		if (IPL_CtrlGetInfor(Id, IPLCTRL_SIE_CH4_EN) == ENABLE) {
			QInfo = IPL_Ctrl_Query(Id, ICF_ETHOUT_RDY);

			ipl_disInfo.frame_cnt = idx;
			ipl_disInfo.InSizeH = IPL_CtrlGetInfor(Id, IPLCTRL_SIE_CH0_RAW_SIZE_H);

			ipl_disInfo.InLineOfs = IPL_CtrlGetInfor(Id, IPLCTRL_SIE_CH4_SIZE_HLOS);
			ipl_disInfo.InSizeV = IPL_CtrlGetInfor(Id, IPLCTRL_SIE_CH0_RAW_SIZE_V);
			if (PrvAddr == 0) {
				ipl_disInfo.InAdd0 = QInfo.Rdy_Eth.addr;
			} else {
				ipl_disInfo.InAdd0 = PrvAddr;
			}

            ipl_disInfo.InAdd1 = QInfo.Rdy_Eth.addr;
            //dis_setDisInfor(&ipl_disInfo);
			PrvAddr = QInfo.Rdy_Eth.addr;
            DIS_State = 1;
		}
	}

	if (IntStatus & SIE_FLDEND){
        if(DIS_State == 1){
            dis_setDisInfor(&ipl_disInfo);
            DIS_State = 0;
        }
    }
    

	if (IntStatus & SIE_BP1) {
	}
	if (IntStatus & SIE_BP2) {
	}
}

void IPL_IsrRHECB(IPL_PROC_ID Id, UINT32 IntStatus, UINT32 idx)
{
	if (IntStatus & RHE_FMD) {
		if (idx > 1) {
			if ((IPL_CtrlGetInfor(Id, IPLCTRL_EXT_FUNC_EN) & ICF_FUNC_WDR) || (IPL_CtrlGetInfor(Id, IPLCTRL_EXT_FUNC_EN) & ICF_FUNC_SHDR) || (IPL_CtrlGetInfor(Id, IPLCTRL_EXT_FUNC_EN) & ICF_FUNC_DEFOG)) {
				ICFFunc_TrigEvent(Id);
			}
		}
	}
}

void IPL_IsrIMECB(IPL_PROC_ID Id, UINT32 IntStatus, UINT32 idx)
{
	if (IntStatus & IME_FMD) {
		IQS_Ctrl_ISR(Id);
	}
}

void IPL_IsrIPECB(IPL_PROC_ID Id, UINT32 IntStatus, UINT32 idx)
{
	if (IntStatus & IPE_FMD) {
		//if ((idx > 1) && (Id == IPL_ID_1)) {
		//	AF_TrigEvent(IPL_UTI_CONV2_AF_ID(Id));
		//}
	}
}

void IPL_IsrIFECB(IPL_PROC_ID Id, UINT32 IntStatus, UINT32 idx)
{
	if (IntStatus & IFE_FMD) {
	}
}


void IPL_IsrDCECB(IPL_PROC_ID Id, UINT32 IntStatus, UINT32 idx)
{
	if (IntStatus & DCE_FMD) {
		IQS_TrigEvent(IPL_UTI_CONV2_IQS_ID(Id));
	}
}
