/**
    ICFFunc.

    @file       IPL_ICFFunc.h
    @ingroup    mISYSAlg
    @note       Nothing (or anything need to be mentioned).

    Copyright   Novatek Microelectronics Corp. 2011.  All rights reserved.
*/
#ifndef _IPL_PLUG_ICF_H_
#define _IPL_PLUG_ICF_H_

#include "Type.h"
#include "ipl_utility.h"
/**
    ICFFUNC event function type
*/
typedef void (*ICFFUNC_EVENT_FP)(UINT32 Id);

/**
    ICFFunc event

    control state:

        ICFFunc_Open:    do nothing
        ICFFunc_Trig:    Process Event -> Process Event -> Process Event....
        ICFFunc_Pause:   ....->Process Event -> Process Event -> Process Pause
        ICFFunc_Resume:  Start Event -> Process Event -> Process Event -> Process Event....
*/
typedef struct _ICFFUNC_EVENT_OBJ {
	ICFFUNC_EVENT_FP Process;    ///< process event
} ICFFUNC_EVENT_OBJ;

/**
    ICFFunc input object
*/
typedef struct {
	ICFFUNC_EVENT_OBJ EventTab;   ///< event table
} ICFFUNC_OBJ_INFO;

/**
    ICF Funct Select
*/
typedef enum _ICFFUNC_EN {
	ICF_FUNC_NONE   	= 0x00,    ///< NONE
	ICF_FUNC_WDR    	= 0x01,    ///< WDR
	ICF_FUNC_SHDR   	= 0x02,    ///< Sensor HDR
	ICF_FUNC_DEFOG  	= 0x04,    ///< RHE Defog
	ICF_FUNC_STITCH 	= 0x08,    ///< Image Stitch
	ICF_FUNC_MAX    	= 0x10,
	ENUM_DUMMY4WORD(ICFFUNC_EN)
} ICFFUNC_EN;


/**
     open ICFFunc process
     @param[in] Id          IPL id
     @param[in] Obj         process event
     @return Description of data returned.
         - @b ER: E_OK success
*/
extern ER ICFFunc_Open(IPL_PROC_ID Id, ICFFUNC_OBJ_INFO *Obj);

/**
     close ICFFunc process
     @param[in] Id          IPL id

     @return Description of data returned.
         - @b ER: E_OK success
*/
extern ER ICFFunc_Close(IPL_PROC_ID Id);

/**
     resume ICFFunc process(for IPL control)

     @param[in] Id          IPL id
     @return Description of data returned.
         - @b ER: E_OK success
*/
extern ER ICFFunc_Resume(IPL_PROC_ID Id);

/**
     pause ICFFunc process(for IPL control)

     @param[in] Id          IPL id
     @param[in] WaitFinish  wait pause event finish
     @return Description of data returned.
         - @b ER: E_OK success
*/
extern ER ICFFunc_Pause(IPL_PROC_ID Id, BOOL WaitFinish);

/**
     check ICFFunc is paused or not(for IPL control)

     @param[in] Id          IPL id
     @return Description of data returned.
         - @b BOOL: TRUE paused, not
*/
extern BOOL ICFFunc_IsPause(IPL_PROC_ID Id);
/**
     trigger event, for IPL hook

     @param[in] Id          IPL id
*/
extern void ICFFunc_TrigEvent(UINT32 Id);

#endif //_IPL_PLUG_ICF_INT_H_
