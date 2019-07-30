#ifndef _IQ_UART_CMD_H_
#define _IQ_UART_CMD_H_

#include "ipl_hal_ime_info.h"

#define IQ_DUMP_VAR(var) (DBG_DUMP("IQ %s = %d\r\n", #var, var))
#define IQ_DUMP_ARR(arr, len)     \
	do{                                 \
		UINT32 i;                       \
		DBG_DUMP("%s = { ", #arr);      \
		for(i=0; i<len; i++)            \
			DBG_DUMP("%d, ", arr[i]);   \
		DBG_DUMP("}\r\n");              \
	}while(0)

typedef struct {
	UINT32 sf;
	UINT32 gbal;
	UINT32 filter_addr;
	UINT32 edge_enh_addr;
	UINT32 ed_map;
	UINT32 score_th;//IPE_DirScorCalTh
	UINT32 ewp;//IQS_EWParam_Buf
	UINT32 lca;
	UINT32 la;
	UINT32 IFE2_ref_param;
	UINT32 IFE2_filter_param;
} iq_dbg_element_addr;

extern void set_sq_dbg_addr(IME_HAL_3DNR_INFO *pdbg_sq, UINT32 *pdbg);
extern void set_iq_dbg_addr(iq_dbg_element_addr *pdbg_sq, UINT32 *pdbg);

#endif
