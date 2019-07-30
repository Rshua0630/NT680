/**
    IPH module (DRE type define)

    @file       ipl_hal_dre.h NT96660
    @ingroup    mILibIPH

    Copyright   Novatek Microelectronics Corp. 2011.  All rights reserved.
*/

#ifndef _IPL_Hal_DRE_H
#define _IPL_Hal_DRE_H
#include "ipl_hal_dre_info.h"
/** \addtogroup mILibIPH */
//@{

/**
    query enum

    read from hw register
*/
typedef enum _IPH_DRE_QUE {
	ENUM_DUMMY4WORD(IPH_DRE_QUE)
} IPH_DRE_QUE;

/**
     enum of DRE update selection
     (DRE_UPDATE_INIT | DRE_UPDATE_NR) = NR process
     (DRE_UPDATE_INIT | DRE_UPDATE_HDR) = HDR process
*/
typedef enum _DRE_UPDATE {
	DRE_UPDATE_INIT        = 0x00000001,   ///< init dre
	DRE_UPDATE_NR          = 0x00000002,   ///< dre NR parameter
	DRE_UPDATE_HDR         = 0x00000004,   ///< dre HDR parameter
	DRE_UPDATE_DBG         = 0x00000005,   ///< dre dump register on/off & set buffer address
} DRE_UPDATE;

/**
     enum of DRE read selection

     read from inner table, not from hw register.
*/
typedef enum _DRE_READ {
	DRE_R_NR_WORKBUF        = 0x00000001,   ///< get NR process working buffer, data type: DRE_WORKBUF_INFO
	DRE_R_HDR_WORKBUF       = 0x00000002,   ///< get HDR process working buffer, data type: DRE_WORKBUF_INFO
	ENUM_DUMMY4WORD(DRE_READ)
} DRE_READ;

/**
     DRE configuration

     all DRE configuration including all DRE sub function
*/
typedef struct _DRE_Manager {
	DRE_IN_FMT fmt;
	UINT32 WorkBufAddr;
	DRE_IMG_INFO In[4];                             ///< Y1, UV1, Y2, UV2
	DRE_IMG_INFO Out[2];                            ///< Y1, UV1
	DRE_SUBFUNC *pSubFunc;                          ///< -
} DRE_Manager, *pDRE_Manager;

/**
     DRE configuration
*/
typedef DRE_Manager DRE;

//@}
#endif
