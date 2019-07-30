#ifndef IPL_EXT_FLOW_CB_INT_H_
#define IPL_EXT_FLOW_CB_INT_H_
/*
    IPL flow Callback Function

    This file is the API of the flow callback function.

    @file       ipl_ext_flow_cb_int.h
    @ingroup    mISYSAlg
    @note       Nothing.

    Copyright   Novatek Microelectronics Corp. 2009.  All rights reserved.
*/

/**
     IPL flow callback function
*/

/**
     prepare cap AE function

     @note (Optional)

     @param[in] IPL Id
*/
void IPL_FCB_PrepareCapAE(IPL_PROC_ID Id);

/**
     preflash flow function

     @note (Optional)

     @param[in] IPL Id
*/
void IPL_FCB_PreFlash(IPL_PROC_ID Id);

/**
     capture flow(Sensor -> SIE -> Dram:bayer pattern)

     @note (Optional)

     @param[in] IPL Id
     @param[in] SIE settings
*/
void IPL_FCB_GetCapRaw(IPL_PROC_ID id, IPC_SIE_Info *info, IPLCTRL_CAP_EXT_INFO *ext_info);


/**
     set capture AWB gain

     @note (Optional)

     @param[in] IPL Id
*/
void IPL_FCB_SetCapAWB(IPL_PROC_ID Id, IPC_SIE_Info *Info);

/**
     Alg Information CB
*/
void IPL_FCB_AlgGDC(IPL_PROC_ID Id, UINT32 SelIdx, UINT32 Value);
void IPL_FCB_AlgDR(IPL_PROC_ID Id, UINT32 SelIdx, UINT32 Value);
void IPL_FCB_AlgDIS(IPL_PROC_ID Id, UINT32 SelIdx, UINT32 Value);
void IPL_FCB_AlgIE(IPL_PROC_ID Id, UINT32 SelIdx, UINT32 Value);
void IPL_FCB_ImageFlip(IPL_PROC_ID Id, UINT32 SelIdx, UINT32 Value);
//extern void IPL_FCB_FlashTrig(FLASH_INFOR *Data); //Jarkko tmp
extern void IPL_FCB_MshutterOpen(IPL_PROC_ID Id);
extern void IPL_FCB_MshutterClose(UINT32 uiEvent);

extern void IPL_RAWHeaderCB(IPL_PROC_ID Id, UINT32 FrmCnt);
extern UINT32 IPL_FCB_CAPSIEIQ(IPL_PROC_ID id, SIE_SUBFUNC *subF, SIE_UPDATE *Update);
#endif //IPL_EXT_FLOW_CB_INT_H_
