/**
    IPH module (ISE type define)

    @file       ipl_hal_ise.h NT96660
    @ingroup    mILibIPH

    Copyright   Novatek Microelectronics Corp. 2011.  All rights reserved.
*/

#ifndef _IPL_Hal_ISE_H
#define _IPL_Hal_ISE_H
#include "ipl_hal_ise_info.h"
/** \addtogroup mILibIPH */
//@{

/**
    query enum

    read from hw register
*/
typedef enum _IPH_ISE_QUE {
	ISE_REG_CLOCKRATE           = 0x00000001,   ///< read engine clock rate     Data type: UINT32
	ENUM_DUMMY4WORD(IPH_ISE_QUE)
} IPH_ISE_QUE;

/**
     enum of ISE update selection
*/
typedef enum _ISE_UPDATE {
	ISE_IOPACK      = 0x00000001,   ///< update IOPackFormat
	ISE_INPUT       = 0x00000002,   ///< update Ise_In
	ISE_OUTPUT      = 0x00000004,   ///< update Ise_Out
	ISE_SCALEMETHOD = 0x00000008,   ///< update Ise SCALEMETHOD
	ISE_INIT        = 0x04000000,   ///< init ise
} ISE_UPDATE;

/**
     enum of ISE read selection

     read from inner table, not from hw register.
*/
typedef enum _ISE_READ {
	ISE_R_INPUT         = 0x00000001,   ///< read ISE input image info, data type ISE_IN
	ISE_R_OUTPUT        = 0x00000002,   ///< read ISE output image info, data type ISE_OUT
	ISE_R_IOPACK        = 0x00000003,   ///< read input and output packing format, data type ISE_PACK_FMT
	ISE_R_SCALEMETHOD   = 0x00000004,   ///< read scaling method selection, data type ISE_SCALE_METHOD_SEL
	ENUM_DUMMY4WORD(ISE_READ)
} ISE_READ;

/**
     ISE configuration

     all ISE configuration including all ISE sub function
*/
typedef struct _ISE_Manager {
	ISE_PACK_FMT fmt;                               ///< scaling format
	ISE_SCALE_METHOD_SEL ScaleMethod;               ///< scaling method selection
	ISE_SCLFTR ScaleFilter;                         ///< scaling filter(always setting auto)
	ISE_IN In;                                      ///< input image information
	ISE_OUT Out;                                    ///< output image information
	ISE_SUBFUNC *pSubFunc;                          ///< -
} ISE_Manager, *pISE_Manager;

/**
     ISE configuration
*/
typedef ISE_Manager ISE;

//@}
#endif
