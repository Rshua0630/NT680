/**
    IPH module (IFE sub function type define)

    @file       ipl_hal_ife.h NT96660
    @ingroup    mILibIPH

    Copyright   Novatek Microelectronics Corp. 2011.  All rights reserved.
*/

#ifndef _IPL_Hal_IFE_H
#define _IPL_Hal_IFE_H
#include "ipl_hal_ife_info.h"
/** \addtogroup mILibIPH */
//@{
/**
     query enum

     read from hw register.
*/
typedef enum _IPH_IFE_QUE {
	ENUM_DUMMY4WORD(IPH_IFE_QUE)
} IPH_IFE_QUE;

/**
     enum of IFE update selection
*/
typedef enum _IFE_UPDATE {
	IFE_I_ADDR          = 0X00000001,       ///< update input addr
	IFE_O_ADDR          = 0X00000002,       ///< update output addr
	IFE_IO_SIZE         = 0x00000004,       ///< update input/output size/lineoffset
	IFE_SUBFEN          = 0X00000008,       ///< update func enable/disable
	IFE_OUTLIER         = 0X00000010,       ///< update outlier
	IFE_CGAIN           = 0x00000040,       ///< update color gain
	IFE_CGOFS           = 0x00000080,       ///< update offset(bayer stage)
	IFE_CGEFFECT        = 0x00000100,       ///< update color gain output inverse/mask
	IFE_FILTER          = 0x00000200,       ///< update noise reduction filter
	IFE_VIGPARAM        = 0x00000400,       ///< update vignette parameters
	IFE_GBALPARAM       = 0x00000800,       ///< update Gbalance parameters
	IFE_INIT            = 0x00002000,       ///< init ife
	IFE_VIG_CEN         = 0x00008000,       ///< update vig center parameters
	IFE_LOADALL_FD      = 0x00010000,       ///< speical case for IPL pipe mode, only valid at _IFE_Load function
	IFE_LOADALL_NON_FD  = 0x00020000,       ///< speical case for IPL pipe mode, only valid at _IFE_Load function
	IFE_CFAPAT          = 0x00040000,       ///< update color interpolatite cfa pattern
	ENUM_DUMMY4WORD(IFE_UPDATE)
} IFE_UPDATE;

/**
     enum of ife read selection

     read from inner table, not from hw register.
*/
typedef enum _IFE_READ {
	IFE_R_IO        = 0x00000001,   ///< read in/out info. exclude IFE_SUBFUNC,     Data type: IFE
	IFE_R_SUBFEN    = 0x00000002,   ///< read func enable/disable,                  Data type: IfeFuncEn
	IFE_R_OUTL      = 0X00000003,   ///< read out liner,                            Data type: IFE_OUTL
	IFE_R_CGAIN     = 0x00000005,   ///< read digital color gain,                   Data type: IFE_CG
	IFE_R_CGOFS     = 0x00000006,   ///< read digital color gain offset,            Data type: IFE_CG
	IFE_R_CGEFFECT  = 0x00000007,   ///< read digital color gain effect,            Data type: IFE_CG
	IFE_R_FILTER    = 0X00000008,   ///< read ife image filter,                     Data type: IFE_FILT
	IFE_R_VIG       = 0X00000009,   ///< read ife vig parameter,                    Data type: IFE_VIG
	IFE_R_GBAL      = 0X0000000a,   ///< read ife G balance parameter,              Data type: IFE_GBAL
	IFE_R_INTEEN    = 0x0000000b,   ///< read interruppt enabe bit                  Data type: IfeInteEn
	ENUM_DUMMY4WORD(IFE_READ)
} IFE_READ;

/**
     IFE configuration

     all IFE configuration including all IFE sub function
*/
typedef struct _IFE_Manager {

	IFE_SRCMODE IFEmode;            ///< IFE op mode

	IFEFMT IFEFmt;                  ///< ife in/out data format
	IFE_BAYERFMT_SEL IFEBayerFmt;   ///< ife bayer format(RGGB/RGBIr)

	IFE_IN_DMA IFE_In;              ///< ife input information

	IFE_DMA_OUT IFE_Out;            ///< ife output information

	IFE_SUBFUNC *pSubFunc;          ///< -
	//reverse
	//UINT32 rev;                   //reserve for IPH paraset idx
} IFE_Manager, *pIFE_Manager;

/**
     IFE configuration
*/
typedef IFE_Manager IFE;
//@}
#endif
