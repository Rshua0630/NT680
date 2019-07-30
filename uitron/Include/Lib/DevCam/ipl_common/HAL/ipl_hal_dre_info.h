/**
    IPH module (DRE sub function type define)

    @file       IPL_Hal_DRE_Info.h NT96660
    @ingroup    mILibIPH

    Copyright   Novatek Microelectronics Corp. 2011.  All rights reserved.
*/

#ifndef _IPL_Hal_DRE_Info_H
#define _IPL_Hal_DRE_Info_H
#include "Type.h"
#include "ipl_utility.h"
/** \addtogroup mILibIPH */
//@{
#define DRE_MaxParaSet  2


/**
    @name DRE enum - Input and output packing format selection
*/
typedef enum _DRE_IN_FMT {
	DRE_SEL_422_C = 0,      ///< complete process of YUV422 pack input
	DRE_SEL_422_S = 1,      ///< simplified process of YUV422 pack input
	DRE_SEL_420 = 2,        ///< complete process of YUV420 pack input
	ENUM_DUMMY4WORD(DRE_IN_FMT)
} DRE_IN_FMT;


/**
    work buffer informaton
*/
typedef struct _DRE_WORKBUF_INFO {
	UINT32 uiWidth;                    ///< image width
	UINT32 uiHeight;                   ///< image height
	UINT32 uiProcRepNum;               ///< proc repeat number (only used for MSNR flow)
	DRE_IN_FMT fmt;                    ///< image format
	UINT32 WorkBufSize;
} DRE_WORKBUF_INFO;

/**
     DRE sub function configuration.
*/

/**
    DRE structure - DRE image information
*/
typedef struct _DRE_IMG_INFO {
	UINT32 uiWidth;                    ///< image width
	UINT32 uiHeight;                   ///< image height
	UINT32 uiLineoffset;               ///< lineoffset
	UINT32 uiAddr;                     ///< buffer address
} DRE_IMG_INFO;

/**
    DRE structure - DRE NR information
*/
typedef struct _DRE_NR_INFO {
	UINT32 *puiWt;   ///< Spatial weight ,please assign a UINT32[6] array
	UINT32 *puiRngThrY;     ///< Table of range filter threshold, please assign a UINT32[8] array
	UINT32 *puiRngThrU;     ///< Table of range filter threshold, please assign a UINT32[8] array
	UINT32 *puiRngThrV;     ///< Table of range filter threshold, please assign a UINT32[8] array
} DRE_NR_INFO;

/**
    DRE structure - DRE NR information
*/
typedef struct _DRE_NR_GAM_LUT_INFO {
	UINT32 *puiLutY;    ///< NR gamma table for Y, please assign a UINT32[128] array
	UINT32 *puiLutU;    ///< NR gamma table for U, please assign a UINT32[128] array
	UINT32 *puiLutV;    ///< NR gamma table for V, please assign a UINT32[128] array
} DRE_NR_GAM_LUT_INFO;

/**
    DRE structure - DRE YCMOD information
*/
typedef struct _DRE_YCMOD_TAB_INFO {
	UINT32 *puiYcLutY;      ///< Table of YC modify LUT for Y, please assign a UINT32[16] array
	UINT32 *puiYcLutC;      ///< Table of YC modify LUT for C, please assign a UINT32[16] array
} DRE_YCMOD_TAB_INFO;

/**
    DRE structure - DRE YCMOD information
*/
typedef struct _DRE_YCMOD_CTRL_INFO {
	UINT32 uiCbOfs;
	UINT32 uiCrOfs;
	UINT32 uiStepY;
	UINT32 uiStepC;
	BOOL bModSel;
	BOOL bModLutSel;
} DRE_YCMOD_CTRL_INFO;

/**
    DRE structure - DRE DG information
*/
#define DRE_DG_PROC_MAX_NUM 10

typedef struct _DRAM_DG_INFO {
	UINT32 uiSAddr;
	UINT32 uiLofst;
} DRAM_DG_INFO;

typedef struct _DRE_HDR_DG_CTRL_INFO {
	BOOL bDgEn;                                     ///<Enable of HDR deghost function
	BOOL bDgImgRef;                                 ///<Reference image of deghost function, only effective when bDgEn = ENABLE
	UINT32 uiDgLyrNum;                              ///<Deghost layer number
	DRAM_DG_INFO DgMskIn[DRE_DG_PROC_MAX_NUM];      ///<Deghost mask address, only effective when bDgEn = ENABLE
} DRE_HDR_DG_CTRL_INFO;


#define DRE_NR_PROC_MAX_NUM 10
/**
    DRE structure - Overlap size setting structure
*/
typedef struct _DRE_SUBFUNC {
	//HDR
	DRE_HDR_DG_CTRL_INFO DgCtrl;

	//NR
	UINT32 uiProcRepNum;
	BOOL bNrGamYEn[DRE_NR_PROC_MAX_NUM];
	BOOL bNrGamUEn[DRE_NR_PROC_MAX_NUM];
	BOOL bNrGamVEn[DRE_NR_PROC_MAX_NUM];
	DRE_NR_INFO NrJBFilt[DRE_NR_PROC_MAX_NUM];
	DRE_NR_GAM_LUT_INFO GamLut[DRE_NR_PROC_MAX_NUM];
	DRE_YCMOD_TAB_INFO YCMTbl[DRE_NR_PROC_MAX_NUM];
	DRE_YCMOD_CTRL_INFO YCMCtrl[DRE_NR_PROC_MAX_NUM];

	//DUMP DBG information
	UINT32 DumpEn;
	UINT32 DumpBufAdr;

} DRE_SUBFUNC;

//@}
#endif
