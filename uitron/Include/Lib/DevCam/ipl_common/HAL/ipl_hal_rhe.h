/**
    IPH module (RHE type define)

    @file       ipl_hal_rhe.h
    @ingroup    mILibIPH

    Copyright   Novatek Microelectronics Corp. 2011.  All rights reserved.
*/

#ifndef _IPL_Hal_RHE_H
#define _IPL_Hal_RHE_H
#include "ipl_hal_rhe_info.h"
/** \addtogroup mILibIPH */
//@{

/**
    query enum

    read from hw register
*/
typedef enum _IPH_RHE_QUE {
	RHE_REG_CH0CURRDYADDR   = 0x00000006,   ///< read rhe current dma ready address                 Data type: UINT32
	RHE_REG_CH1CURRDYADDR   = 0x00000009,   ///< read rhe current dma ready address LA              Data type: UINT32
	ENUM_DUMMY4WORD(IPH_RHE_QUE)
} IPH_RHE_QUE;

/**
     enum of RHE update selection
*/
typedef enum _RHE_UPDATE {
	RHE_DMA_IN          	= 0x00000001,   ///< update input address
	RHE_DMA_OUT         	= 0x00000002,   ///< update output address
	RHE_RAW_SIZE        	= 0x00000004,   ///< update input raw size
	RHE_DARK_FRAME_SUB  	= 0x00000008,   ///< update dark frame subtration
	RHE_DMA_SUBIN       	= 0x00000010,   ///< update sub-image input address
	RHE_SUBIN_SIZE      	= 0x00000020,   ///< update sub-image input size
	RHE_SUBIN_SCL       	= 0x00000040,   ///< update sub-image input scale info
	RHE_PATH_CURVE      	= 0x00000080,   ///< update path curve param(multi frame blending)(curve A~E)
	RHE_DECODE          	= 0x00000100,   ///< update decode info
	RHE_COLORGAIN       	= 0x00000200,   ///< update color gain info
	RHE_INIT            	= 0x00000400,   ///< init rhe
	RHE_SUBF_EN         	= 0x00000800,   ///< update func enable/disable by rheFuncEn
	RHE_CROP_INFO       	= 0x00001000,   ///< update crop information
	RHE_SUBIMG_FILTER   	= 0x00002000,   ///< update sub-image filter info
	RHE_FLICKER_INFO    	= 0x00004000,   ///< update flicker detection information
	RHE_BLEND           	= 0x00008000,   ///< update blending info
	RHE_LOCAL_CONTRAST  	= 0x00010000,   ///< update local contrast enhancement
	RHE_TONE_CURVE      	= 0x00020000,   ///< update tone curve info
	RHE_COLOR_SAT       	= 0x00040000,   ///< update color saturation info
	RHE_COLOR_NOISE_SUP 	= 0x00080000,   ///< update color noise suppression info
	RHE_SUBOUT          	= 0x00100000,   ///< update subout info
	RHE_DMA_SUBOUT      	= 0x00200000,   ///< update subout address
	RHE_COLOR_CONTRAST  	= 0x00400000,   ///< update color contrast adjustment info
	RHE_DMA_CADJ_SUBIN  	= 0x00800000,   ///< update color contrast adjustment subimage address
	RHE_DECODE_HEADER   	= 0x01000000,   ///< update decode header, output from sie ch5
	RHE_SUBF_UPDATE     	= 0x02000000,   ///< update func enable/disable by rhe_func_enable/rhe_func_disable
	RHE_FUNCMODE        	= 0x04000000,   ///< update rhe func mode(linear/phdr/shdr/dfs)
	RHE_BAYERPAT        	= 0x08000000,   ///< update rhe bayer pattern(bayer format, cfa pattern)
	RHE_PATH_CURVE_WEIGHT	= 0x10000000,   ///< update path curve param(multi frame blending)(Weight 1~3)
	ENUM_DUMMY4WORD(RHE_UPDATE)
} RHE_UPDATE;
/**
     enum of RHE read selection

     read from inner table, not from hw register.
*/
typedef enum _RHE_READ {
	RHE_R_SUBF_EN       = 0x00000001,   ///< read func enable/disable,                          Data type: RheFuncEn
	RHE_R_INTE_EN       = 0x00000002,   ///< read rhe interrupt enable bit                      Data type: RHE_INTE_EN
	RHE_R_RAW_SIZE      = 0x00000003,   ///< read rhe input size                                Data type: RHE_SIZE
	RHE_R_SUBOUT_ADDR   = 0x00000004,   ///< read rhe subout address                            Data type: UINT32
	RHE_R_SUBIN_SIZE    = 0x00000005,   ///< read rhe sub input size(PHDR/SHDR)                 Data type: RHE_SIZE
	RHE_R_FUNC_MODE     = 0x00000006,   ///< read rhe function mode(linear/PHDR/SHDR)           Data type: RHE_HAL_FUNC_MODE
	RHE_R_CRP_SIZE      = 0x00000007,   ///< read rhe crop size                                 Data type: RHE_SIZE
	RHE_R_CRP_START     = 0x00000008,   ///< read rhe crop start position                       Data type: RHE_CROP_POS
	RHE_R_CFA_PAT       = 0x00000009,   ///< read rhe cfa pattern                               Data type: RHE_IMG_PAT_INFO
	RHE_R_RAW_IN_ADDR   = 0x0000000A,   ///< read rhe input address                             Data type: UINT32[4]
	RHE_R_SUBIN_ADDR    = 0x0000000B,   ///< read rhe sub input address(PHDR/SHDR)              Data type: UINT32
	RHE_R_SUBOUT_SIZE   = 0x0000000C,   ///< read rhe sub output size(PHDR/SHDR)                Data type: RHE_SIZE
	ENUM_DUMMY4WORD(RHE_READ)
} RHE_READ;


/**
     RHE configuration

     all RHE configuration including all RHE sub function
*/
typedef struct _RHE_Manager {
	RHE_HAL_OPMODE              op_mode;                    ///< RHE operation mode
	RHE_HAL_FUNC_MODE           func_mode;
	RHE_HAL_HDR_FRAME_MODE      hdr_frame_mode;

	// input image info
	UINT32                      in_path_sel;                ///< desired setting channel, ex: (RHE_HAL_INPATH_1 | RHE_HAL_INPATH_2)
	RHE_SIZE                    RawInSize;                  ///< RHE input raw size
	RHE_SIZE                    RawCropSize;                ///< RHE input raw crop size
	RHE_CROP                    RawCropPos;                 ///< RHE crop infomation
	RHE_Dma_In                  DmaInputInfo;               ///< RHE dma input information
	RHE_HAL_BAYER_FMT           bayer_fmt;
	RHE_HAL_CFA_PAT_SEL         cfa_pattern;

	// input subimage info
	RHE_SIZE                    subimg_size;                ///< RHE sub-input raw size
	RHE_SIZE                    block_size;                 ///< RHE sub-input raw size
	RHE_Dma_In                  dma_subin_info;             ///< RHE dma sub-input information
	RHE_HAL_SCALE_COEF_INFO     subimg_scl_info;

	RHE_Dma_Out                 DmaOutInfo;                 ///< output image info, size will follow input crop size
	RHE_HAL_BURST_SEL           dma_in_burst;

	RHE_SUBFUNC                 *pSubFunc;                  ///<
} RHE_Manager, *pRHE_Manager;

/**
     RHE configuration
*/
typedef RHE_Manager RHE;

//@}
#endif
