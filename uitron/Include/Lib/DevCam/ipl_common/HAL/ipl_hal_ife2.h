/**
    IPH module (IFE2 type define)

    @file       ipl_hal_ife2.h NT96660
    @ingroup    mILibIPH

    Copyright   Novatek Microelectronics Corp. 2014.  All rights reserved.
*/

#ifndef _IPL_Hal_IFE2_H
#define _IPL_Hal_IFE2_H
#include "ipl_hal_ife2_info.h"
/** \addtogroup mILibIPH */
//@{

/**
     Query enum

     read from hw register.
*/
typedef enum _IPH_IFE2_QUE {
	IFE2_Q_INTSTATUS        = 0x00000001,   ///< read Input addr from reg.      Data type: UINT32
	IFE2_Q_INSIZE           = 0x00000002,   ///< read Input addr from reg.      Data type: IFE2_SIZE
	IFE2_Q_SRCFMT           = 0x00000003,   ///< read Input addr from reg.      Data type: IFE2_REG_STATUS
	IFE2_Q_DSTFMT           = 0x00000004,   ///< read Input addr from reg.      Data type: IFE2_FORMAT
	IFE2_Q_INLOFS           = 0x00000005,   ///< read Input addr from reg.      Data type: IFE2_FORMAT
	IFE2_Q_INADDR           = 0x00000006,   ///< read Input addr from reg.      Data type: IFE2_ADDR
	IFE2_Q_OUTOFS           = 0x00000007,   ///< read Input addr from reg.      Data type: IFE2_LOFS
	IFE2_Q_OUTADDR0         = 0x00000008,   ///< read Input addr from reg.      Data type: IFE2_ADDR
	IFE2_Q_GRAY_AVG         = 0x0000000B,   ///< read Input addr from reg.      Data type: IFE2_GRAY_AVG
	ENUM_DUMMY4WORD(IPH_IFE2_QUE)
} IPH_IFE2_QUE;

/**
     enum of IFE2 update selection
*/
typedef enum _IFE2_UPDATE {
	IFE2_ADDR_INPUT         = 0x00000001,       ///< update input addr
	IFE2_ADDR_OUTPUT        = 0x00000002,       ///< update output ID0 addr
	IFE2_LINEOFT_INPUT      = 0x00000004,       ///< update input lineoffset
	IFE2_LINEOFT_OUTPUT     = 0x00000008,       ///< update output lineoffset
	IFE2_INSIZE             = 0x00000010,       ///< update input size
	IFE2_SRCFMT             = 0x00000020,       ///< update input source format
	IFE2_DSTFMT             = 0x00000040,       ///< update output source format
	IFE2_REFCENTERPARA      = 0x00000080,       ///< update reference center para.
	IFE2_FILTERPARAM        = 0x00000100,       ///< update filter para.
	IFE2_GRAYSTAL           = 0x00000200,       ///< update filter para.
	IFE2_INIT               = 0x00000400,       ///< init ife2
	ENUM_DUMMY4WORD(IFE2_UPDATE)
} IFE2_UPDATE;

/**
     enum of ife2 read selection

     read from inner table, not from hw register.
*/
typedef enum _IFE2_READ {
	IFE2_R_IO               = 0x00000001,       ///< read in/out info. exclude IFE_SUBFUNC,             Data type: IFE2
	IFE2_R_SUBFEN           = 0x00000002,       ///< read func enable/disable,                          Data type: Ife2FuncEn
	IFE2_R_REFCENTERPARA    = 0x00000003,       ///< read reference center parameters                   Data type: IFE2_Refcent_Param
	IFE2_R_FILTERPARAM      = 0X00000004,       ///< read filter parameters                             Data type: IFE2_Filter_Param,
	IFE2_R_GRAYSTATALTH     = 0x00000005,       ///< read Gray Statistical threshold                    Data type: IFE2_GRAY_Statal,
	IFE2_R_INTEEN           = 0x00000006,       ///< read interruppt enabe bit                          Data type: Ife2InteEn,
	ENUM_DUMMY4WORD(IFE2_READ)
} IFE2_READ;

/**
     IFE2 configuration

     all ife2 configuration including all IFE2 sub function
*/
typedef struct _IFE2_Manager {
	IFE2_SRCMODE     SourceMode;        ///< ife2 operation mode

	IFE2_FORMAT      InFmt;             ///< ife2 input data format
	IFE2_FORMAT      OutFmt;            ///< ife2 output data format
	IFE2_ADDR        InAddr;            ///< ife2 dma input address
	IFE2_ADDR        OutAddr;          ///< ife2 dma output address
	IFE2_LOFS        InOfs;             ///< ife2 input lineofs
	IFE2_LOFS        OutOfs;            ///< ife2 output lineofs for Y/UV

	IFE2_SIZE        InSize;            ///< ife2 input size
	IFE2_SUBFUNC     *pSubFunc;         ///< ife2 sub function pointer
} IFE2_Manager, *pIFE2_Manager;

/**
     IFE2 configuration
*/
typedef IFE2_Manager IFE2;

//@}
#endif
