/**
    IPL_Ctrl module (ISR Event)

    @file       IPL_ISREvent.h
    @ingroup    mILibIPH

    Copyright   Novatek Microelectronics Corp. 2011.  All rights reserved.
*/

#ifndef _IPL_ISREVENT_H_
#define _IPL_ISREVENT_H_
#include "Type.h"
#include "ipl_utility.h"
#include "ipl_hal_op.h"
#include "ipl_ctrl_flow.h"

/**
    Engine ISR enum
*/
typedef enum _IPL_ISR {
	IPL_ISR_UNKNOWN = IPL_HAL_UNKNOW,
	IPL_ISR_SIE1 = IPL_HAL_SIE1,   ///< SIE1
	IPL_ISR_SIE2 = IPL_HAL_SIE2,   ///< SIE2
	IPL_ISR_SIE3 = IPL_HAL_SIE3,   ///< SIE3
	IPL_ISR_SIE4 = IPL_HAL_SIE4,   ///< SIE4
	IPL_ISR_SIE5 = IPL_HAL_SIE5,   ///< SIE5
	IPL_ISR_SIE6 = IPL_HAL_SIE6,   ///< SIE6
	IPL_ISR_SIE7 = IPL_HAL_SIE7,   ///< SIE7
	IPL_ISR_SIE8 = IPL_HAL_SIE8,   ///< SIE8
	IPL_ISR_IFE  = IPL_HAL_IFE,    ///< IFE
	IPL_ISR_DCE  = IPL_HAL_DCE,    ///< DCE
	IPL_ISR_IPE  = IPL_HAL_IPE,    ///< IPE
	IPL_ISR_IME  = IPL_HAL_IME,    ///< IME
	IPL_ISR_IFE2 = IPL_HAL_IFE2,   ///< IFE2
	IPL_ISR_RHE  = IPL_HAL_RHE,    ///< RHE
	IPL_ISR_MAX_CNT = IPL_HAL_MAX,
	IPL_ISR_SIEx = 0x0000ffff, ///< for internal auto set
	ENUM_DUMMY4WORD(IPL_ISR)
} IPL_ISR;

typedef void (*IPL_ISR_EVENT_FP)(IPL_PROC_ID, UINT32, UINT32);
typedef void (*IPL_ISR_RAWHEADER_CB)(IPL_PROC_ID, UINT32);

/**
    ime datastamp callback

    param:
    IMESTAMP_INFO* info:    stamp info, set by callback function
    UINT32 ipl_id
    UINT32 width:           current ime input image width
    UINT32 height:          current ime input image height
    UINT32 reserved
*/
typedef void (*IPL_DATASTAMP_CB)(IMESTAMP_INFO *, UINT32, UINT32, UINT32, UINT32);

/**
    ime privacy mask callback

    param:
    IME_PRI_MASK_INFO* info:    privacy mask info, set by callback function,
                                PxlWidth, PxlHeight, PmPxlLofs will automatically calc by PmPxlBlkSize
    UINT32 ipl_id
    UINT32 width:               current ime input image width
    UINT32 height:              current ime input image height
    UINT32 reserved
*/
typedef void (*IPL_PM_CB)(IME_PRI_MASK_INFO *, UINT32, UINT32, UINT32, UINT32);

/**
    SIE RAW Post Process callback

    param:
    IPL_FRONTEND_IMG info:   raw info(addr, w, h...)
    UINT32 sie_id
    UINT32 reserved
*/
typedef void (*SIE_RAW_POSTPROC_CB)(IPL_FRONTEND_IMG, UINT32, UINT32);

typedef struct {
	IPL_ISR_EVENT_FP FP[IPL_ISR_MAX_CNT];
} IPL_ISR_EVENT_TAB;

typedef struct {
	IPL_ISR_EVENT_TAB EventTab;             ///<for IPL_lib inner modules
	IPL_ISR_EVENT_TAB CB_FP;                ///<for modules those are not in IPL_lib
	IPL_ISR_RAWHEADER_CB RAWHeader_CB;      ///<for raw hearder callback with ext_lib

	IPL_DATASTAMP_CB    datastamp_cb;       ///< for ime datastamp callback when trigger start
	IPL_PM_CB           pm_cb;              ///< for ime privacy mask callback when trigger start

	SIE_RAW_POSTPROC_CB raw_postproc_cb;    ///< for sie raw post process callback when sie dramend
} IPL_ISR_OBJ_INFO;

//@}
#endif //_IPL_ISREVENT_H_
