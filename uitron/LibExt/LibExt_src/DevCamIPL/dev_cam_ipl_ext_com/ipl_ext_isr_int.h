#ifndef IPL_EXT_ISR_INT_H_
#define IPL_EXT_ISR_INT_H_
/**
    ipl_ext_isr_int.h


    @file       ipl_ext_isr_Int.h
    @ingroup    mISYSAlg
    @note       Nothing (or anything need to be mentioned).

    Copyright   Novatek Microelectronics Corp. 2011.  All rights reserved.
*/

extern void IPL_IsrSIECB(IPL_PROC_ID Id, UINT32 IntStatus, UINT32 idx);
extern void IPL_IsrRHECB(IPL_PROC_ID Id, UINT32 IntStatus, UINT32 idx);
extern void IPL_IsrDCECB(IPL_PROC_ID Id, UINT32 IntStatus, UINT32 idx);
extern void IPL_IsrIFECB(IPL_PROC_ID Id, UINT32 IntStatus, UINT32 idx);
extern void IPL_IsrIPECB(IPL_PROC_ID Id, UINT32 IntStatus, UINT32 idx);
extern void IPL_IsrIMECB(IPL_PROC_ID Id, UINT32 IntStatus, UINT32 idx);

#endif //IPL_EXT_ISR_INT_H_