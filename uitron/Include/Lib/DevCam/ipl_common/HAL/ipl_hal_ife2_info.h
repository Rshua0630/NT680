/**
    IPH module (IFE2 sub function type define)

    @file       IPL_Hal_IFE2_Info.h NT96660
    @ingroup    mILibIPH

    Copyright   Novatek Microelectronics Corp. 2014.  All rights reserved.
*/
#ifndef _IPL_Hal_IFE2_Info_H
#define _IPL_Hal_IFE2_Info_H
#include "Type.h"
#include "ipl_utility.h"

/** \addtogroup mILibIPH */
//@{

/**
    ife2 engine supported clk
*/
typedef enum _IPH_IFE2CLK {
	IPH_IFE2CLK0 = 240,  ///< -240 mhz
	IPH_IFE2CLK1 = 480,  ///< -480 mhz
	ENUM_DUMMY4WORD(IPH_IFE2CLK)
} IPH_IFE2CLK;
//#endif

/**
    IFE2 interrupt enable enum.
*/
typedef enum _IFE2_INTE_En {
	IFE2__INTE_OVFL     =   0x40000000,                             ///< enable ife2 overflow interrupt
	IFE2__INTE_FRMEND   =   0x80000000,                             ///< enable ife2 frame end interrupt
	IFE2__INTE_ALL      = (IFE2__INTE_OVFL | IFE2__INTE_FRMEND),    ///< enable ife2 all interrup
	ENUM_DUMMY4WORD(IFE2_INTE_En)
} IFE2_INTE_En;

/**
    IFE2 interrupt status enum.
*/
typedef enum _IFE2_INTE_Status {
	IFE2__INT_OVFL     =   0x40000000,                           ///< status ife2 overflow interrupt
	IFE2__INT_FRMEND   =   0x80000000,                           ///< status ife2 frame end interrupt
	IFE2__INT_ALL      = (IFE2__INT_OVFL | IFE2__INT_FRMEND),    ///< status ife2 all interrup
	ENUM_DUMMY4WORD(IFE2_INTE_Status)
} IFE2_INTE_Status;

/**
    IFE2 sub function enable enum
*/
typedef enum _IFE2_FUNC_En {
	YFtrEn      = 0x00000001,       ///< enable y filter
	ENUM_DUMMY4WORD(IFE2_FUNC_En)
} IFE2_FUNC_En;

/**
    IFE2 Input Data Source
    Input Always from Dram, only differ in output destination
*/
typedef enum _IFE2_SRCMODE {
	IFE2_D2D     = 0,    ///< IFE2 only
	IFE2_RHE_IPP = 1,    ///< RHE + IFE->DCE->IPE->IME(IFE2)
	IFE2_SIE_IPP = 2,    ///< SIE + IFE->DCE->IPE->IME(IFE2) (all direct)
	ENUM_DUMMY4WORD(IFE2_SRCMODE)
} IFE2_SRCMODE;

/**
    IFE2 Source Format
*/
typedef enum _IFE2_FORMAT {
	FMT_YUVP   = 0,           ///< YUVYUV Packing
	FMT_ONE_P  = 1,           ///< planar(only one channel)
	FMT_Y_UVP  = 2,           ///< Y/UV-Packing
	ENUM_DUMMY4WORD(IFE2_FORMAT)
} IFE2_FORMAT;

/**
    Struct IFE2 DMA Address
*/
typedef struct _IFE2_ADDR {
	UINT32 uiAddrY;   ///< lineoffset for Y channel
} IFE2_ADDR;

/**
    Struct IFE2 Image Size
*/
typedef struct _IFE2_SIZE {
	UINT32 uiImgSizeH;   ///< input horizontal size, image width
	UINT32 uiImgSizeV;  ///< input vertical size, image height
} IFE2_SIZE;

/**
    Struct IFE2 Lineoffset
*/
typedef struct _IFE2_LOFS {
	UINT32 uiLineoffsetY;   ///< lineoffset for Y channel
} IFE2_LOFS;

/**
    Struct IFE2 Gray Statistical Setting
*/
typedef struct _IFE2_GRAY_Statal {
	UINT32 uiUTh0;  ///< Gray Statistical threshold for U channel
	UINT32 uiUTh1;  ///< Gray Statistical threshold for U channel
	UINT32 uiVTh0;  ///< Gray Statistical threshold for V channel
	UINT32 uiVTh1;  ///< Gray Statistical threshold for V channel
} IFE2_GRAY_Statal;

/**
    Struct IFE2 Reference Center Computation Setting
*/
typedef struct _IFE2_REFCENT_Set {
	UINT32 uiRThAddr;   ///< Range filter threshold for reference center ,Data Type: UINT32 Array[3]
	UINT32 uiRWtAddr;   ///< Range filter weighting for reference center ,Data Type: UINT32 Array[4]
	UINT32 uiCntWt;    ///< Reference center weighting
	UINT32 uiOutlDth;  ///< Outlier difference threshold
	UINT32 uiOutlTh;   ///< Reference center outlier threshold
} IFE2_REFCENT_Set;

/**
    Struct IFE2 Reference Center Computation Setting for Y/UV channel
*/
typedef struct _IFE2_Refcent_Param {
	IFE2_REFCENT_Set  RefCentY;       ///< for Y channel
	IFE2_REFCENT_Set  RefCentUV;      ///< for UV channel
} IFE2_Refcent_Param;

/**
    IFE2 Filter Size Selection
*/
typedef enum _IFE2_FILT_Size {
	FLTR_SIZE_3x3 = 0,           ///< 3x3 window
	FLTR_SIZE_5x5 = 1,           ///< 5x5 window
	FLTR_SIZE_7x7 = 2,           ///< 7x7 window
	FLTR_SIZE_9x9 = 3,           ///< 9x9 window
	ENUM_DUMMY4WORD(IFE2_FILT_Size)
} IFE2_FILT_Size;

/**
    Struct IFE2 Edge Direction Threshold
*/
typedef struct _IFE2_EDRCT_Th {
	UINT32  uiEdthHV;   ///< Edge direction threshold for two diagonal directions
	UINT32  uiEdthPN;   ///< Edge direction threshold for horizontal and vertical directions
} IFE2_EDRCT_Th;


/**
    Struct IFE2 gray average
*/
typedef struct _IFE2_GRAY_AVG {
	UINT32 AvgU;
	UINT32 AvgV;
} IFE2_GRAY_AVG;

/**
    IFE2 Edge Kernel Filter Size Selection
*/
typedef enum _IFE2_EDGE_Kernel_Size {
	EKNL_SIZE_3x3 = 0,           ///< 3x3 window
	EKNL_SIZE_5x5 = 1,           ///< 5x5 window
	ENUM_DUMMY4WORD(IFE2_EDGE_Kernel_Size)
} IFE2_EDGE_Kernel_Size;

/**
    Struct IFE2 Filter Computation Setting
*/
typedef struct _IFE2_FILTER_Para {
	UINT32 uiFtrThAddr;  ///< Filter threshold LUT ,Data Type: UINT32 Array[5]
	UINT32 uiFtrWtAddr;  ///< Filter weighting LUT ,Data Type: UINT32 Array[6]
} IFE2_FILTER_Para;


/**
    IFE2_FILTER
*/
typedef struct _IFE2_Filter_Param {
	IFE2_FILT_Size          FiltSize;       ///< filter size
	IFE2_EDRCT_Th           FiltEdth;       ///< filter edge direction threshold
	IFE2_EDGE_Kernel_Size   EdgeKernelSize; ///< edge kernel size
	IFE2_FILTER_Para        FiltSetY;   ///< Filter para for Y Channel
	IFE2_FILTER_Para        FiltSetU;   ///< Filter para for U Channel
	IFE2_FILTER_Para        FiltSetV;   ///< Filter para for V Channel
} IFE2_Filter_Param;

/**
    IFE2_BURSTLEN
*/

typedef enum _IFE2_BURST_LEN {
	IFE2_BURST_LEN_64   = 0,        ///< burst 64 word
	IFE2_BURST_LEN_48   = 1,        ///< burst 48 word
	IFE2_BURST_LEN_96   = 2,        ///< burst 96 word
	IFE2_BURST_LEN_128  = 3,        ///< burst 128 word
} IFE2_BURST_LEN;

typedef struct _IFE2_BURST {
	IFE2_BURST_LEN input;
	//IFE2_BURST_LEN output;    ///< not available for ipl
} IFE2_BURST;

/**
     ife2 sub function configuration.
*/
typedef struct _IFE2_SUBFUNC {
	IFE2_INTE_En ife2InteEn;        ///< ife2 interrupt enable
	IFE2_FUNC_En ife2FuncEn;        ///< ife2 function enable

	IFE2_GRAY_Statal    GrayStatal;          ///< gray statistical (only for U/V channel)

	IFE2_Refcent_Param  *pRefcent;           ///< reference center parameters for Y/UV channel
	IFE2_Filter_Param   *pFilter;            ///< IFE2 filter para set

	IFE2_BURST burst;
} IFE2_SUBFUNC;

//@}
#endif
