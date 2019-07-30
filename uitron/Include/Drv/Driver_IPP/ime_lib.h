/**
    Public header file for IME module.

    @file       ime_lib.h
    @ingroup    mIIPPIME

    @brief

    Copyright   Novatek Microelectronics Corp. 2014.  All rights reserved.
*/


#ifndef _IME_LIB_H_
#define _IME_LIB_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "Type.h"

typedef void (*IMEIntCB)(UINT32);

//------------------------------------------------------------------------------
//enum data type




//------------------------------------------------------------------------------
/**
    @addtogroup mIDrvIPP_IME
*/
//@{

//define vairable
#define IME_INTE_STRP_END    0x40000000  ///< stripe-end enable
#define IME_INTE_FRM_END     0x80000000  ///< frame-end enable
#define IME_INTE_ALL         (IME_INTE_FRM_END | IME_INTE_STRP_END)   ///< all interrupt enable

#define IME_INT_STRP_END    0x40000000  ///< stripe-end enable
#define IME_INT_FRM_END     0x80000000  ///< frame-end status
#define IME_INT_ALL         (IME_INT_FRM_END | IME_INT_STRP_END)   ///< all interrupt status

#define IPE_To_IME  0         ///< IPE to IME
#define DRAM_To_IME 1         ///< DMA to IME



//------------------------------------------------------------------------------
// engine control
/**
    IME enum - function enalbe/disable
*/
typedef enum _IME_FUNC_EN {
	IME_FUNC_DISABLE = 0,   ///< function disable
	IME_FUNC_ENABLE  = 1,  ///< function enable
	ENUM_DUMMY4WORD(IME_FUNC_EN)
} IME_FUNC_EN;

/**
    IME enum - operation mode
*/
typedef enum _IME_OPMODE {
	IME_OPMODE_D2D     = 0,    ///< IME only
	IME_OPMODE_RHE_IPP = 1,    ///< RDE + IFE->DCE->IPE->IME
	IME_OPMODE_SIE_IPP = 2,    ///< SIE + IFE->DCE->IPE->IME (all direct)
	IME_OPMODE_UNKNOWN = 3,    ///< Undefined mode
	ENUM_DUMMY4WORD(IME_OPMODE)
} IME_OPMODE;


/**
    IME enum - load type of configned parameters
*/
typedef enum {
	IME_START_LOAD   = 0,  ///< Frame start loaded
	IME_FRMEND_LOAD  = 1,  ///< Frame end loaded
	ENUM_DUMMY4WORD(IME_LOAD_TYPE)
} IME_LOAD_TYPE;

/**
    IME enum - parameter mode
*/
typedef enum {
	IME_PARAM_AUTO_MODE = 0,    ///< Auto parameter mode
	IME_PARAM_USER_MODE = 1,    ///< User parameter mode
	ENUM_DUMMY4WORD(IME_PARAM_MODE)
} IME_PARAM_MODE;



//------------------------------------------------------------------------------
// input image size control
/**
    IME enum - input stripe mode selection
*/
typedef enum _IME_STRIPE_MODE {
	IME_STRIPE_AUTO_MODE = 0,    ///< Auto stripe information computation mode
	IME_STRIPE_USER_MODE = 1,    ///< User stripe information computation mode
	ENUM_DUMMY4WORD(IME_STRIPE_MODE)
} IME_STRIPE_MODE;

/**
    IME enum - input stripe overlap selection
*/
typedef enum _IME_H_STRIPE_OVLP_SEL {
	IME_H_ST_OVLP_16P  = 0,    ///< Horizontal stripe overlap size: 16 pixels
	IME_H_ST_OVLP_24P  = 1,    ///< Horizontal stripe overlap size: 24 pixels
	IME_H_ST_OVLP_32P  = 2,    ///< Horizontal stripe overlap size: 32 pixels
	IME_H_ST_OVLP_USER = 3,    ///< Horizontal stripe overlap size: user define
	ENUM_DUMMY4WORD(IME_H_STRIPE_OVLP_SEL)
} IME_H_STRIPE_OVLP_SEL;

/**
    IME enum - input stripe overlap partition selection
*/
typedef enum _IME_H_STRIPE_PRT_SEL {
	IME_H_ST_PRT_5P  = 0,    ///< Horizontal stripe overlap partition size: 5 pixels
	IME_H_ST_PRT_3P  = 1,    ///< Horizontal stripe overlap partition size: 3 pixels
	IME_H_ST_PRT_2P  = 2,    ///< Horizontal stripe overlap partition size: 2 pixels
	IME_H_ST_PRT_USER = 3,    ///< Horizontal stripe overlap partition size: user define
	ENUM_DUMMY4WORD(IME_H_STRIPE_PRT_SEL)
} IME_H_STRIPE_PRT_SEL;


/**
    IME enum - input image data format selection
*/
typedef enum _IME_INPUT_FORMAT_SEL {
	IME_INPUT_YCC_444  = 0,         ///< YCC444 separate planar format for input
	IME_INPUT_YCC_422  = 1,         ///< YCC422 separate planar format for input
	IME_INPUT_YCC_420  = 2,         ///< YCC420 separate planar format for input
	IME_INPUT_RESERVED = 3,         ///< reversed
	IME_INPUT_YCCP_422 = 4,         ///< YCC422 UV-packing format for input
	IME_INPUT_YCCP_420 = 5,         ///< YCC420 UV-packing format for input
	IME_INPUT_Y_ONLY   = 6,         ///< Only Y channel for input
	IME_INPUT_RGB      = 7,         ///< RGB for input (YCC444)
	ENUM_DUMMY4WORD(IME_INPUT_FORMAT_SEL)
} IME_INPUT_FORMAT_SEL;

/**
    IME enum - image data format selection of LCA function
*/
typedef enum _IME_LCA_FORMAT_SEL {
	IME_LCAF_YCCYCC  = 0,    ///< YCbCrYCbCr data format
	IME_LCAF_YCCP    = 1,    ///< Y / CbCr data format
	ENUM_DUMMY4WORD(IME_LCA_FORMAT_SEL)
} IME_LCA_FORMAT_SEL;

//------------------------------------------------------------------------------
// output control
/**
    IME enum - output path selection
*/
typedef enum _IME_PATH_SEL {
	IME_PATH1_SEL = 0x01,   ///< operation for output path1
	IME_PATH2_SEL = 0x02,   ///< operation for output path2
	IME_PATH3_SEL = 0x04,   ///< operation for output path3
	IME_PATH4_SEL = 0x08,   ///< operation for output path4
	IME_PATH5_SEL = 0x10,   ///< operation for output path5
	ENUM_DUMMY4WORD(IME_PATH_SEL)
} IME_PATH_SEL;

/**
    IME enum - output buffer selection
*/
typedef enum _IME_PATH_OUTBUF_SEL {
	IME_PATH_OUTBUF_SET0 = 0,   ///< output buffer set0 selection
	IME_PATH_OUTBUF_SET1 = 1,   ///< output buffer set1 selection
	ENUM_DUMMY4WORD(IME_PATH_OUTBUF_SEL)
} IME_PATH_OUTBUF_SEL;

/**
    IME enum - output image data format type
*/
typedef enum _IME_OUTPUT_FORMAT_TYPE {
	IME_OUTPUT_YCC_PLANAR   = 0,         ///< Y/Cb/Cr separate planar type
	IME_OUTPUT_YCC_UVPACKIN = 1,         ///< Y/UV-packing type
	ENUM_DUMMY4WORD(IME_OUTPUT_FORMAT_TYPE)
} IME_OUTPUT_FORMAT_TYPE;

/**
    IME enum - output image data format selection
*/
typedef enum _IME_OUTPUT_IMG_FORMAT_SEL {
	IME_OUTPUT_YCC_444     = 0,         ///< YCC444 format for output
	IME_OUTPUT_YCC_422_COS = 1,         ///< YCC422 cosited format for output
	IME_OUTPUT_YCC_422_CEN = 2,         ///< YCC422 centered format for output
	IME_OUTPUT_YCC_420_COS = 3,         ///< YCC420 cosited format for output
	IME_OUTPUT_YCC_420_CEN = 4,         ///< YCC420 centered format for output
	IME_OUTPUT_Y_ONLY      = 6,         ///< Y only for output
	IME_OUTPUT_RGB_444     = 7,         ///< RGB444 format for output
	ENUM_DUMMY4WORD(IME_OUTPUT_IMG_FORMAT_SEL)
} IME_OUTPUT_IMG_FORMAT_SEL;

/**
    IME enum - scale method selection
*/
typedef enum _IME_SCALE_METHOD_SEL {
	IMEALG_BICUBIC     = 0,        ///< bicubic interpolation
	IMEALG_BILINEAR    = 1,        ///< bilinear interpolation
	IMEALG_NEAREST     = 2,        ///< nearest interpolation
	IMEALG_INTEGRATION = 3,        ///< nearest interpolation
	ENUM_DUMMY4WORD(IME_SCALE_METHOD_SEL)
} IME_SCALE_METHOD_SEL;

/**
    IME enum - scale filter computation mode selection
*/
typedef enum _IME_SCALE_FILTER_COEF_MODE {
	IME_SCALE_FILTER_COEF_AUTO_MODE = 0,    ///< Auto Scale filter coefficeint computation mode
	IME_SCALE_FILTER_COEF_USER_MODE = 1,    ///< User Scale filter coefficeint computation mode
	ENUM_DUMMY4WORD(IME_SCALE_FILTER_COEF_MODE)
} IME_SCALE_FILTER_COEF_MODE;

/**
    IME enum - scale type selection
*/
typedef enum _IME_SCALE_TYPE_SEL {
	IME_SCALE_DOWN = 0,  ///< scaling down enable
	IME_SCALE_UP = 1,    ///< scaling up enable
	ENUM_DUMMY4WORD(IME_SCALE_TYPE_SEL)
} IME_SCALE_TYPE_SEL;

/**
    IME enum - scale factor computation mode selection
*/
typedef enum _IME_SCALE_FACTOR_COEF_MODE {
	IME_SCALE_FACTOR_COEF_AUTO_MODE = 0,    ///< Auto Scale factor coefficeint computation mode
	IME_SCALE_FACTOR_COEF_USER_MODE = 1,    ///< User Scale factor coefficeint computation mode
	ENUM_DUMMY4WORD(IME_SCALE_FACTOR_COEF_MODE)
} IME_SCALE_FACTOR_COEF_MODE;

/**
    IME enum - outupt path destination selection
*/
typedef enum _IME_OUTDST_CTRL_SEL {
	IME_OUTDST_DRAM = 0,         ///< output to Dram
	IME_OUTDST_H264 = 1,         ///< output to H.264
	ENUM_DUMMY4WORD(IME_OUTDST_CTRL_SEL)
} IME_OUTDST_CTRL_SEL;

//------------------------------------------------------------------------------
/**
    IME enum - system flag clear selection
*/
typedef enum _IME_FLAG_CLEAR_SEL {
	IME_FLAG_NO_CLEAR = 0,    ///< No clear flag
	IME_FLAG_CLEAR    = 1,    ///< Clear flag
	ENUM_DUMMY4WORD(IME_FLAG_CLEAR_SEL)
} IME_FLAG_CLEAR_SEL;

//------------------------------------------------------------------------------

/**
    IME enum - scale enhancing mode selection
*/
typedef enum _IME_SCALE_ENH_MODE {
	IME_SCALE_ENH_MODE_AUTO = 0,  ///< Scaling enhancement, parameter setting by auto
	IME_SCALE_ENH_MODE_USER = 1,  ///< Scaling enhancement, parameter setting by user
	ENUM_DUMMY4WORD(IME_SCALE_ENH_MODE)
} IME_SCALE_ENH_MODE;


/**
    IME enum - value range selection

    @note used for chroma adaptation
*/
typedef enum _IME_RANGE_SEL {
	IME_RANGE_8   = 0,  ///< range 8
	IME_RANGE_16  = 1,  ///< range 16
	IME_RANGE_32  = 2,  ///< range 32
	IME_RANGE_64  = 3,  ///< range 64
	IME_RANGE_128 = 4,  ///< range 128
	ENUM_DUMMY4WORD(IME_RANGE_SEL)
} IME_RANGE_SEL;


/**
    IME enum - Chroma adaptation input source
*/
typedef enum {
	IME_LCA_SRC_DRAM = 0, ///< From DRAM, using for capture mode
	IME_LCA_SRC_IFE2 = 1, ///< From IFE2 directly, using for video mode
	IME_LCA_SRC_ALL  = 2, ///< From IFE2 and setting all paramters
} IME_LCA_SRC;



/**
    IME enum - dark and bright chroma suppression mode selection
*/
typedef enum _IME_DBCS_MODE_SEL {
	IME_DBCS_DK_MODE  = 0,  ///< dark mode
	IME_DBCS_BT_MODE  = 1,  ///< bright mode
	IME_DBCS_BOTH_MODE  = 2,  ///< dark and bright mode
	ENUM_DUMMY4WORD(IME_DBCS_MODE_SEL)
} IME_DBCS_MODE_SEL;


/**
    IME enum - data stamp processing selection
*/
typedef enum _IME_DS_SETNUM {
	IME_DS_SET0  = 0,  ///< Set0
	IME_DS_SET1  = 1,  ///< Set1
	IME_DS_SET2  = 2,  ///< Set2
	IME_DS_SET3  = 3,  ///< Set3
	ENUM_DUMMY4WORD(IME_DS_SETNUM)
} IME_DS_SETNUM;


/**
    IME enum - data stamp color LUT mode selection
*/
typedef enum _IME_DS_FMT_SEL {
	IME_DS_FMT_RGB565   = 0,  ///< data stamp format, RGB565
	IME_DS_FMT_RGB1555  = 1,  ///< data stamp format, RGB1555
	IME_DS_FMT_RGB4444  = 2,  ///< data stamp format, RGB4444
	ENUM_DUMMY4WORD(IME_DS_FMT_SEL)
} IME_DS_FMT_SEL;

/**
    IME enum - SSR processing mode
*/
typedef enum _IME_SSR_MODE_SEL {
	IME_SSR_MODE_USER  = 0,  ///< user mode of SSR, function enable/disable is controlled by user
	IME_SSR_MODE_AUTO  = 1,  ///< auto mode of SSR, function enable/disable is controlled by IME driver
	ENUM_DUMMY4WORD(IME_SSR_MODE_SEL)
} IME_SSR_MODE_SEL;


/**
    IME enum - statistical edge kernel selection
*/
typedef enum _IME_STL_EKER_SEL {
	IME_STL_EKER_HA  = 0,  ///< H map A
	IME_STL_EKER_HB  = 1,  ///< H map B
	IME_STL_EKER_VA  = 2,  ///< V map A
	IME_STL_EKER_VB  = 3,  ///< V map B
	ENUM_DUMMY4WORD(IME_STL_EKER_SEL)
} IME_STL_EKER_SEL;


/**
    IME enum - statistical source selection
*/
typedef enum _IME_STL_SRC_SEL {
	IME_STL_SRC_SET0  = 0,  ///< source from set0
	IME_STL_SRC_SET1  = 1,  ///< source from set1
	ENUM_DUMMY4WORD(IME_STL_SRC_SEL)
} IME_STL_SRC_SEL;


/**
    IME enum - histogram operating selection
*/
typedef enum _IME_STL_HIST_SET_SEL {
	IME_STL_HIST_SET0  = 0,  ///< set0 selection
	IME_STL_HIST_SET1  = 1,  ///< set1 selection
	ENUM_DUMMY4WORD(IME_STL_HIST_SET_SEL)
} IME_STL_HIST_SET_SEL;




/**
    IME enum - output image data type selection for Statistical
*/
typedef enum _IME_STL_IMGOUT_SEL {
	IME_STL_IMGOUT_AF  = 0,  ///< output after filter
	IME_STL_IMGOUT_BF  = 1,  ///< output before filter
	ENUM_DUMMY4WORD(IME_STL_IMGOUT_SEL)
} IME_STL_IMGOUT_SEL;


/**
    IME enum - privacy mask set number selection
*/
typedef enum _IME_PM_SET_SEL {
	IME_PM_SET0 = 0,  ///< set0
	IME_PM_SET1 = 1,  ///< set1
	IME_PM_SET2 = 2,  ///< set2
	IME_PM_SET3 = 3,  ///< set3
	IME_PM_SET4 = 4,  ///< set4
	IME_PM_SET5 = 5,  ///< set5
	IME_PM_SET6 = 6,  ///< set6
	IME_PM_SET7 = 7,  ///< set7
	ENUM_DUMMY4WORD(IME_PM_SET_SEL)
} IME_PM_SET_SEL;

/**
    IME enum - privacy mask type
*/
typedef enum _IME_PM_MASK_TYPE {
	IME_PM_MASK_TYPE_YUV = 0,  ///< Using YUV color
	IME_PM_MASK_TYPE_PXL = 1,  ///< Using pixilation
	ENUM_DUMMY4WORD(IME_PM_MASK_TYPE)
} IME_PM_MASK_TYPE;



/**
    IME enum - privacy mask pixelation blocks size selection
*/
typedef enum _IME_PM_PXL_BLK_SIZE {
	IME_PM_PIXELATION_08 = 0,  ///< 8x8
	IME_PM_PIXELATION_16 = 1,  ///< 16x16
	IME_PM_PIXELATION_32 = 2,  ///< 32x32
	IME_PM_PIXELATION_64 = 3,  ///< 64x64
	ENUM_DUMMY4WORD(IME_PM_PXL_BLK_SIZE)
} IME_PM_PXL_BLK_SIZE;


/**
    IME enum - motion estimator search mv-dif mode
*/
typedef enum _IME_3DNR_MER_MVDIF_MODE_ {
	IME_3DNR_MER_MVDIF_MODE0 = 0,  ///< off
	IME_3DNR_MER_MVDIF_MODE1 = 1,  ///< mvdiff for fs2d
	IME_3DNR_MER_MVDIF_MODE2 = 2,  ///< mvdiff for fs3d
	IME_3DNR_MER_MVDIF_MODE3 = 3,  ///< sad for fs2d
	IME_3DNR_MER_MVDIF_MODE4 = 4,  ///< sad for fs3d
	ENUM_DUMMY4WORD(IME_3DNR_MER_MVDIF_MODE)
} IME_3DNR_MER_MVDIF_MODE;

/**
    IME enum - motion estimator sub-mv mode selection
*/
typedef enum _IME_3DNR_MER_SUB_MV_MODE_ {
	IME_3DNR_MER_SUB_MV_MODE_DROP  = 0,  ///<
	IME_3DNR_MER_SUB_MV_MODE_MIN   = 1,  ///<
	IME_3DNR_MER_MV_MODE_MAX   = 2,  ///<
	ENUM_DUMMY4WORD(IME_3DNR_MER_SUB_MV_MODE)
} IME_3DNR_MER_SUB_MV_MODE;

/**
    IME enum - 3DNR motion scene change mode
*/
typedef enum _IME_3DNR_MOTION_SC_3D_MODE_ {
	IME_3DNR_MOTION_SC_3D_MODE_BV_SAD = 0,
	IME_3DNR_MOTION_SC_3D_MODE_S0_SAD = 1,
	ENUM_DUMMY4WORD(IME_3DNR_MOTION_SC_3D_MODE)
} IME_3DNR_MOTION_SC_3D_MODE;

/**
    IME enum - 3DNR motion reference input selection
*/
typedef enum _IME_3DNR_MOTION_LIMIT_IN_SEL_ {
	IME_3DNR_MOTION_LIMIT_IN_SEL_Y = 0,  ///< from Y motion
	IME_3DNR_MOTION_LIMIT_IN_SEL_C = 1,  ///< from C motion
	ENUM_DUMMY4WORD(IME_3DNR_MOTION_LIMIT_IN_SEL)
} IME_3DNR_MOTION_LIMIT_IN_SEL;


/**
    IME enum - 3DNR motion freeze control selection
*/
typedef enum _IME_3DNR_MOTION_FREEZE_ {
	IME_3DNR_MOTION_FREEZE_CTRL     = 0, ///< use motion contorl
	IME_3DNR_MOTION_FREEZE_BYPASS   = 1, ///< by pass
	ENUM_DUMMY4WORD(IME_3DNR_MOTION_FREEZE)
} IME_3DNR_MOTION_FREEZE;


/**
    IME enum - formation of side-information of codec
*/
typedef enum _IME_SINFO_FMT_ {
	IME_SINFO_FMT_HEVC = 0,  ///< 8x8
	IME_SINFO_FMT_H264 = 1,  ///< 16x16
	ENUM_DUMMY4WORD(IME_SINFO_FMT)
} IME_SINFO_FMT;


/**
    IME enum - burst length selection
*/
typedef enum _IME_BURST_SEL {
	IME_BURST_64W   = 0,  ///< burst size: 64 word
	IME_BURST_32W   = 1,  ///< burst size: 32 word
	IME_BURST_96W   = 2,  ///< burst size: 96 word
	IME_BURST_128W  = 3,  ///< burst size: 128 word
	ENUM_DUMMY4WORD(IME_BURST_SEL)
} IME_BURST_SEL;



/**
    IME enum - get burst size of channel selection
*/
typedef enum _IME_GET_BSTSIZE_SEL {
	IME_GET_BST_IN_Y    = 0,  ///< get input channel: Y
	IME_GET_BST_IN_U    = 1,  ///< get input channel: U
	IME_GET_BST_IN_V    = 2,  ///< get input channel: V
	IME_GET_BST_OUTP1_Y = 3,  ///< get output path1 channel: Y
	IME_GET_BST_OUTP1_U = 4,  ///< get output path1 channel: U
	IME_GET_BST_OUTP1_V = 5,  ///< get output path1 channel: V
	IME_GET_BST_OUTP2_Y = 6,  ///< get output path2 channel: Y
	IME_GET_BST_OUTP2_U = 7,  ///< get output path2 channel: U
	IME_GET_BST_OUTP3_Y = 8,  ///< get output path3 channel: Y
	IME_GET_BST_OUTP3_U = 9,  ///< get output path3 channel: U
	IME_GET_BST_OUTP4_Y = 10,  ///< get output path4 channel: Y
	IME_GET_BST_IN_LCA  = 11,  ///< input channel: LCA
	ENUM_DUMMY4WORD(IME_GET_BSTSIZE_SEL)
} IME_GET_BSTSIZE_SEL;



//------------------------------------------------------------------------------
/**
    IME enum - get engine information selection
*/
typedef enum _IME_GET_INFO_SEL {
	IME_GET_INPUT_SRC                   = 1,
	IME_GET_INT_ENABLE                  = 2,      ///< get interrupt enable information
	IME_GET_INT_STATUS                  = 3,       ///< get interrupt status
	IME_GET_LOCKED_STATUS               = 4,       ///< get IME engine locked status
	IME_GET_LCA_ENABLE                  = 5,       ///< get LCA enable status
	IME_GET_LCA_SUBOUT_ENABLE           = 6,       ///< get LCA subout enable status

	IME_GET_P1_ENABLE_STATUS            = 21,      ///< get output path1 enable status
	IME_GET_P1_OUTPUT_FORMAT            = 22,      ///< get output path1 data format
	IME_GET_P1_OUTPUT_TYPE              = 23,      ///< get output path1 data format type
	IME_GET_P1_SPRTOUT_ENABLE           = 24,      ///< get output path1 data format type

	IME_GET_P2_ENABLE_STATUS            = 41,      ///< get output path2 enable status
	IME_GET_P2_OUTPUT_FORMAT            = 42,      ///< get output path2 data format
	IME_GET_P2_OUTPUT_TYPE              = 43,      ///< get output path2 data format type
	IME_GET_P2_SPRTOUT_ENABLE           = 44,      ///< get output path1 data format type

	IME_GET_P3_ENABLE_STATUS            = 61,      ///< get output path3 enable status
	IME_GET_P3_OUTPUT_FORMAT            = 62,      ///< get output path3 data format
	IME_GET_P3_OUTPUT_TYPE              = 63,      ///< get output path3 data format type
	IME_GET_P3_SPRTOUT_ENABLE           = 64,      ///< get output path1 data format type

	IME_GET_P4_ENABLE_STATUS            = 81,      ///< get output path3 enable status
	IME_GET_P4_OUTPUT_FORMAT            = 82,      ///< get output path3 data format
	IME_GET_P4_OUTPUT_TYPE              = 83,      ///< get output path3 data format type

	IME_GET_P5_ENABLE_STATUS            = 101,      ///< get output path3 enable status
	IME_GET_P5_OUTPUT_FORMAT            = 102,      ///< get output path3 data format
	IME_GET_P5_OUTPUT_TYPE              = 103,      ///< get output path3 data format type
	IME_GET_P5_SPRTOUT_ENABLE           = 104,      ///< get output path1 data format type

	IME_GET_DEBUG_MESSAGE               = 997,      ///< get HW debug message
	IME_GET_FUNC_STATUS0                = 998,      ///< get function enable status
	IME_GET_FUNC_STATUS1                = 999,      ///< get function enable status
	ENUM_DUMMY4WORD(IME_GET_INFO_SEL)
} IME_GET_INFO_SEL;
//------------------------------------------------------------------------------

//structure data type
/**
    IME structure - engine open object
*/
typedef struct _IME_OPENOBJ {
	void (*FP_IMEISR_CB)(UINT32 intstatus);   ///< External ISR call-back function
	UINT32 uiImeClockSel;                     ///< Engine clock selection
} IME_OPENOBJ, *pIME_OPENOBJ;

/**
    IME structure - image size parameters for horizontal and vertical direction
*/
typedef struct _IME_SIZE_INFO {
	UINT32 uiSizeH;                  ///< horizontal size
	UINT32 uiSizeV;                  ///< vertical size
} IME_SIZE_INFO;


/**
    IME structure - DMA address parameters for YUV channels
*/
typedef struct _IME_DMA_ADDR_INFO {
	UINT32 uiAddrY;    ///< DMA address of Y channel
	UINT32 uiAddrCb;   ///< DMA address of Cb channel
	UINT32 uiAddrCr;   ///< DMA address of Cr channel
} IME_DMA_ADDR_INFO;


/**
    IME structure - image lineoffset parameters for luma and chroma channels
*/
typedef struct _IME_LINEOFFSET_INFO {
	UINT32 uiLineOffsetY;   ///< line-offset of Y channel
	UINT32 uiLineOffsetCb;  ///< line-offset of Cb channel
} IME_LINEOFFSET_INFO;


/**
    IME structure - image cooridinate for horizontal and vertical directions
*/
typedef struct _IME_POS_INFO {
	UINT32 uiPosX;
	UINT32 uiPosY;
} IME_POS_INFO;

/**
    IME structure - input stripe parameters
*/
typedef struct _IME_STRIPE_INFO {
	UINT32    uiN;        ///< stripe size in each stripe
	UINT32    uiL;        ///< stripe size of last stripe
	UINT32    uiM;        ///< stripe number
} IME_STRIPE_INFO;


/**
    IME structure - compute stripe size when Dram2Dram mode
*/
typedef struct _IME_STRIPE_CAL_INFO_
{
	IME_SIZE_INFO   InSize;		///< IME input image size
	IME_FUNC_EN		SsrEn;		///< IME SSR function enable/disable
} IME_STRIPE_CAL_INFO;


/**
    IME structure - horizontal and vertical stripe parameters
*/
typedef struct _IME_HV_STRIPE_INFO {
	IME_STRIPE_INFO       StpH;           ///< hroizontal stripe parameters
	IME_STRIPE_INFO       StpV;           ///< Vertical stripe parameters

	IME_H_STRIPE_OVLP_SEL HOverlapSel;    ///< IME input stripe overlap size selection for horizontal direction
	UINT32 uiHOverlap;                    ///< IME input stripe overlap size for horizontal direction

	IME_H_STRIPE_PRT_SEL HPrtSel;    ///< IME input stripe overlap parition size selection for horizontal direction
	UINT32 uiHPrt;                    ///< IME input stripe overlap parition size for horizontal direction
	IME_STRIPE_MODE StripeCalMode;  ///< IME input stripe information setting mode
} IME_HV_STRIPE_INFO;


/**
    IME structure - scaling factors for horizontal and vertical direction
*/
typedef struct _IME_SCALE_FACTOR_INFO {
	IME_SCALE_TYPE_SEL    HScaleUD;           ///< horizontal scale up/down selection
	UINT32                uiHScaleDR;         ///< horizontal scale down rate
	UINT32                uiHScaleFtr;        ///< horizontal scale factor
	UINT32                uiHIsdScaleBaseFtr; ///< horizontal scale base
	UINT32                uiIsdScaleFtrH[3];  ///< horizontal scale factor for integration method

	IME_SCALE_TYPE_SEL    VScaleUD;           ///< vertical scale up/down selection
	UINT32                uiVScaleDR;         ///< vertical scale down rate
	UINT32                uiVScaleFtr;        ///< vertical scale factor
	UINT32                uiVIsdScaleBaseFtr; ///< horizontal scale base
	UINT32                uiIsdScaleFtrV[3];  ///< vertical scale factor for integration method

	IME_SCALE_FACTOR_COEF_MODE CalScaleFactorMode;  ///< scale factor mode
} IME_SCALE_FACTOR_INFO;

/**
    IME structure - scale filter parameters
*/
typedef struct _IME_SCALE_FILTER_INFO {
	IME_FUNC_EN                 HScaleFilterEnable;           ///< scaling filter enable of horizontal direction
	UINT32                      uiHFilterCoef;     ///< scaling filter parameters of horizontal direction
	IME_FUNC_EN                 VScaleFilterEnable;           ///< scaling filter enable of vertical direction
	UINT32                      uiVFilterCoef;     ///< scaling filter parameters of vertical direction
	IME_SCALE_FILTER_COEF_MODE  CoefCalMode;   ///< filter parameter mode
} IME_SCALE_FILTER_INFO;


/**
    IME structure - scale enhancement parameters
*/
//@{
typedef struct _IME_SCALE_ENH_INFO {
	UINT32  uiEnhFactor;         ///< scaling enhancement factor
	UINT32  uiEnhBit;            ///< scaling enhancement normailizaion bit
	IME_SCALE_ENH_MODE EnhMode;  ///< scaling enhancement mode
} IME_SCALE_ENH_INFO;
//@}


/**
    IME structure - output image data format parameters
*/
typedef struct _IME_OUTPUT_FORMAT_INFO {
	IME_OUTPUT_IMG_FORMAT_SEL  OutFormatSel;    ///< path output format
	IME_OUTPUT_FORMAT_TYPE     OutFormatTypeSel;  ///< paht output format type
} IME_OUTPUT_FORMAT_INFO;


//=====================================================================================
// IQ function structure
/**
    IME structure - color space transformation parameters
*/
typedef struct _IME_CST_INFO {
	IME_FUNC_EN   Rgb2YccEnable; ///< Function enable, RGB to YCbCr
	IME_FUNC_EN   Ycc2RgbEnable; ///< Function enable, YcbCr to RGB
} IME_CST_INFO;


/**
    IME structure - image parameters of local chroma adaptation
*/
typedef struct _IME_CHROMA_ADAPTION_IMAGE_INFO {
	IME_SIZE_INFO       LcaImgSize;     ///< input reference image size
	IME_LINEOFFSET_INFO LcaLofs;        ///< input reference image lineoffset
	IME_LCA_FORMAT_SEL  LcaFmt;         ///< input reference image format
	IME_DMA_ADDR_INFO   LcaDmaAddr0;    ///< input reference imaeg buffer address
	IME_LCA_SRC         LcaSrc;         ///< input reference image source
} IME_CHROMA_ADAPTION_IMAGE_INFO;

/**
    IME structure - local chroma adaptation parameters
*/
typedef struct _IME_CHROMA_ADAPTION_IQC_INFO {
	UINT32              LcaRefYWt;      ///< Chroma reference weighting for Y channels
	UINT32              LcaRefUVWt;      ///< Chroma reference weighting for UV channels
	UINT32              LcaOutUVWt;        ///< Chroma adaptation output weighting

	IME_RANGE_SEL       LcaYRng;         ///< Chroma adaptation range selection for UV channels, Max = IME_RANGE_128
	IME_RANGE_SEL       LcaYWtPrc;        ///< Chroma adaptation weighting precision for UV channel, Max = IME_RANGE_32
	UINT32              LcaYTh;          ///< Chroma adaptation range threshold for UV channels
	UINT32              LcaYWtS;         ///< Chroma adaptation weighting start value for UV channels
	UINT32              LcaYWtE;        ///< Chroma adaptation weighting end value for UV channels


	IME_RANGE_SEL       LcaUVRng;         ///< Chroma adaptation range selection for UV channels, Max = IME_RANGE_128
	IME_RANGE_SEL       LcaUVWtPrc;        ///< Chroma adaptation weighting precision for UV channel, Max = IME_RANGE_64
	UINT32              LcaUVTh;          ///< Chroma adaptation range threshold for UV channels
	UINT32              LcaUVWtS;         ///< Chroma adaptation weighting start value for UV channels
	UINT32              LcaUVWtE;        ///< Chroma adaptation weighting end value for UV channels
} IME_CHROMA_ADAPTION_IQC_INFO;

/**
    IME structure - chroma adjustment parameters of local chroma adaptation
*/
typedef struct _IME_CHROMA_ADAPTION_CA_INFO {
	IME_FUNC_EN     LcaCaEnable;         ///< Function enable
	UINT32          LcaCaCentU;      ///< chroma adjustment reference center of U channel
	UINT32          LcaCaCentV;      ///< chroma adjustment reference center of V channel
	IME_RANGE_SEL   LcaCaUVRng;         ///< Chroma adjustment range selection for UV channels, Max = IME_RANGE_128
	IME_RANGE_SEL   LcaCaUVWtPrc;        ///< Chroma adjustment weighting precision for UV channel, Max = IME_RANGE_64
	UINT32          LcaCaUVTh;          ///< Chroma adjustment range threshold for UV channels
	UINT32          LcaCaUVWtS;         ///< Chroma adjustment weighting start value for UV channels
	UINT32          LcaCaUVWtE;        ///< Chroma adjustment weighting end value for UV channels
} IME_CHROMA_ADAPTION_CA_INFO;

/**
    IME structure - luma suppression parameters of local chroma adaptation
*/
typedef struct _IME_CHROMA_ADAPTION_IQL_INFO {
	IME_FUNC_EN     LcaLaEnable;         ///< Function enable
	UINT32          LcaLaRefWt;      ///< Luma adaptation reference weighting for UV channels
	UINT32          LcaLaOutWt;        ///< Luma adaptation output weighting
	IME_RANGE_SEL   LcaLaRng;         ///< Luma adaptation range selection for UV channels, Max = IME_RANGE_128
	IME_RANGE_SEL   LcaLaWtPrc;        ///< Luma adaptation weighting precision for UV channel, Max = IME_RANGE_64
	UINT32          LcaLaTh;          ///< Luma adaptation range threshold for UV channels
	UINT32          LcaLaWtS;         ///< Luma adaptation weighting start value for UV channels
	UINT32          LcaLaWtE;        ///< Luma adaptation weighting end value for UV channels
} IME_CHROMA_ADAPTION_IQL_INFO;

/**
    IME structure - local chroma adaptation parameters

    @note integrated structure
*/
typedef struct _IME_CHROMA_ADAPTION_INFO {
	IME_FUNC_EN                     LcaEnable;          ///< Function enable

	IME_CHROMA_ADAPTION_IMAGE_INFO  LcaImageInfo;       ///< Reference image information
	IME_CHROMA_ADAPTION_IQC_INFO    LcaIqCInfo;           ///< Function quality information for chroma channel
	IME_CHROMA_ADAPTION_IQL_INFO    LcaIqLInfo;           ///< Function quality information for luma channel
	IME_CHROMA_ADAPTION_CA_INFO     LcaCaInfo;          ///< Chroma adjustment information
} IME_CHROMA_ADAPTION_INFO;


/**
    IME structure - local chroma adaptation subout scale parameters

    @note integrated structure
*/
typedef struct _IME_CHROMA_APAPTION_SUBOUT_PARAM {
	IME_SIZE_INFO   PriImgSize;                 ///< principal image size
	IME_SIZE_INFO   RefImgSize;                 ///< reference image size
	IME_SCALE_FACTOR_INFO   SuboutSclFactor;    ///< scale factor
} IME_CHROMA_APAPTION_SUBOUT_PARAM;

/**
    IME structure - local chroma adaptation subout info.

    @note integrated structure
*/
typedef struct _IME_CHROMA_ADAPTION_SUBOUT_INFO {
	IME_FUNC_EN                         LcaSuboutEnable;        ///< LCA subout enable
	IME_CHROMA_APAPTION_SUBOUT_PARAM    LcaSuboutScaleInfo;     ///< LCA subout scale info.
	IME_LINEOFFSET_INFO                 LcaSuboutLofs;           ///< LCA subout lineoffset
	IME_DMA_ADDR_INFO                   LcaSuboutAddr;           ///< LCA subout buffer address
} IME_CHROMA_ADAPTION_SUBOUT_INFO;


/**
    IME structure - quality parameters of dark and bright region chroma suppression
*/
typedef struct _IME_DBCS_IQ_INFO {
	IME_DBCS_MODE_SEL OpMode;     ///< Process mode
	UINT32            uiCentU;    ///< Center value for U channel
	UINT32            uiCentV;    ///< Center value for V channel
	UINT32            uiStepY;    ///< Step for luma
	UINT32            uiStepC;   ///<  Step for chroma
	UINT32            *puiWtY;    ///< Weighting LUT for luma channel, 16 elements
	UINT32            *puiWtC;    ///< Weighting LUT for luma channel, 16 elements
} IME_DBCS_IQ_INFO;

/**
    IME structure - dark and bright region chroma suppression

    @note integrated structure
*/
typedef struct _IME_DBCS_INFO {
	IME_FUNC_EN       DbcsEnable; ///< Function enable
	IME_DBCS_IQ_INFO  DbcsIqInfo; ///< function quality parameters
} IME_DBCS_INFO;


/**
    IME structure - edge direction thresholds of SSR function
*/
typedef struct _IME_SSR_IQ_INFO {
	UINT32        uiDTh;   ///< Threahold for diagonal direction
	UINT32        uiHVTh;   ///< Threahold for horizontal and vertical direction
} IME_SSR_IQ_INFO;


/**
    IME structure - single image super-resolution parameters

    @note integrated structure
*/
typedef struct _IME_SSR_INFO {
	IME_FUNC_EN       SsrEnable;    ///< Function enable
	IME_SSR_MODE_SEL  SsrMode;      ///< Processing mode
	IME_SSR_IQ_INFO   SsrIqInfo;    ///< Function quality parameters
} IME_SSR_INFO;



/**
    IME structure - quaility parameters of film grain noise
*/
typedef struct _IME_FILM_GRAIN_IQ_INFO {
	UINT32    uiFgnLumTh;           ///< luminance value threshold
	UINT32    uiFgnNLP1;           ///< path1 noise level
	UINT32    uiFgnInitP1;          ///< Grain noise initial value for path1
	UINT32    uiFgnNLP2;           ///< path2 noise level
	UINT32    uiFgnInitP2;          ///< Grain noise initial value for path2
	UINT32    uiFgnNLP3;           ///< path3 noise level
	UINT32    uiFgnInitP3;          ///< Grain noise initial value for path3
	UINT32    uiFgnNLP5;           ///< path5 noise level
	UINT32    uiFgnInitP5;          ///< Grain noise initial value for path5
} IME_FILM_GRAIN_IQ_INFO;

/**
    IME structure - film grain noise parameters

    @note integrated structure
*/
typedef struct _IME_FILM_GRAIN_INFO {
	IME_FUNC_EN               FgnEnable;    ///< film grain enable
	IME_FILM_GRAIN_IQ_INFO    FgnIqInfo;    ///< function quality parameters
} IME_FILM_GRAIN_INFO;


/**
    IME structure - image parameters for data stamp
*/
typedef struct _IME_STAMP_IMAGE_INFO {
	IME_SIZE_INFO   DsImgSize;  ///< Image size
	IME_DS_FMT_SEL  DsFmt;     ///< Image format selection
	IME_POS_INFO    DsPos;      ///< Blending position
	UINT32          uiDsLofs;   ///< Data lineoffset
	UINT32          uiDsAddr;   ///< Data address
} IME_STAMP_IMAGE_INFO;

/**
    IME structure - image parameters for data stamp

    Blending weighting Usage:
    if RGB565, weight = uiBldWt0 + (uiBldWt1 << 4).
    if ARGB1555, A = 0, by using uiBldWt0 to index internal weighting table; A = 1, by using uiBldWt1 to index internal weighting table
    if ARGB1555, by using value of A to index internal weighting table
    Internal weighting table: [0,  17,  34,  51,  68,  85,  102,  119,  136,  153,  170,  187,  204,  221,  238,  255]
*/
typedef struct _IME_STAMP_IQ_INFO {
	IME_FUNC_EN     DsCKEn;        ///< color key enable, only for RGB565 format
	UINT16          uiDsCk;         ///< Color key, 16bit value
	UINT32          uiBldWt0;     ///< Blending weighting, if RGB565, range: [0, 16]; others, range: [0, 15]
	UINT32          uiBldWt1;     ///< Blending weighting, range: [0, 15]
} IME_STAMP_IQ_INFO;

/**
    IME structure - data stamp parameters of each set
*/
typedef struct _IME_STAMP_SET_INFO {
	IME_FUNC_EN             DsEnable;      ///< Function enable
	IME_STAMP_IMAGE_INFO    DsImageInfo;   ///< Image information
	IME_STAMP_IQ_INFO       DsIqInfo;      ///< Data stamp quality information
} IME_STAMP_SET_INFO;

/**
    IME structure - YUV color LUT parameters of data stamp
*/
typedef struct _IME_STAMP_CST_INFO {
	IME_PARAM_MODE  DsCstParamMode;    ///< parameter mode
	UINT32  uiDsCstCeof0; ///< Coefficient of color space transform
	UINT32  uiDsCstCeof1; ///< Coefficient of color space transform
	UINT32  uiDsCstCeof2; ///< Coefficient of color space transform
	UINT32  uiDsCstCeof3; ///< Coefficient of color space transform
} IME_STAMP_CST_INFO;

/**
    IME structure - data stamp parameters

    @note integrated structure
*/
typedef struct _IME_STAMP_INFO {
	IME_FUNC_EN         DsCstEn;        ///< color space transformation of data stamp
	IME_STAMP_SET_INFO  DsSet0;         ///< data stamp set0
	IME_STAMP_SET_INFO  DsSet1;         ///< data stamp set1
	IME_STAMP_SET_INFO  DsSet2;         ///< data stamp set2
	IME_STAMP_SET_INFO  DsSet3;         ///< data stamp set3
	IME_STAMP_CST_INFO  DsCstCoef;      ///< color space transformation coefficients
} IME_STAMP_INFO;


/**
    IME structure - row position parameters of statistical
*/
typedef struct _IME_STL_ROWPOS_INFO {
	UINT32 uiStlRow0;  ///< row position0
	UINT32 uiStlRow1;  ///< row posotion1
} IME_STL_ROWPOS_INFO;

/**
    IME structure - accmulation target parameters of statistical
*/
typedef struct _IME_STL_ACCTAG_INFO {
	UINT32 uiAccTag;  ///< accmulation target
} IME_STL_ACCTAG_INFO;

/**
    IME structure - edge map parameters of statistical
*/
typedef struct _IME_STL_EDGE_INFO {
	IME_FUNC_EN      StlEdgeKerEn0; ///< edge kernel enable for set0
	IME_STL_EKER_SEL StlEdgeKer0;   ///< edge map kernel selection for map0
	UINT32           uiStlSft0;     ///< edge map shift for map0

	IME_FUNC_EN      StlEdgeKerEn1; ///< edge kernel enable for set1
	IME_STL_EKER_SEL StlEdgeKer1;   ///< edge map kernel selection for map1
	UINT32           uiStlSft1;     ///< edge map shift for map1
} IME_STL_EDGE_INFO;

/**
    IME structure - ROI threshold parameters of statistical
*/
typedef struct _IME_STL_ROITH_INFO {
	IME_STL_SRC_SEL     RoiSrc; ///< ///< statistical source of ROI
	UINT32              uiRoiTh0;    ///< statistical threshold of ROI for section0
	UINT32              uiRoiTh1;    ///< statistical threshold of ROI for section1
	UINT32              uiRoiTh2;    ///< statistical threshold of ROI for section2
} IME_STL_ROITH_INFO;

/**
    IME structure - edge processing parameters of statistic

    @note integrated structure
*/
typedef struct _IME_STL_ROI_INFO {
	IME_STL_ROWPOS_INFO StlRowPos;      ///< row position of statistic
	IME_STL_ROITH_INFO  StlRoi0;           ///< ROI0 for edge map
	IME_STL_ROITH_INFO  StlRoi1;           ///< ROI1 for edge map
	IME_STL_ROITH_INFO  StlRoi2;           ///< ROI2 for edge map
	IME_STL_ROITH_INFO  StlRoi3;           ///< ROI3 for edge map
	IME_STL_ROITH_INFO  StlRoi4;           ///< ROI4 for edge map
	IME_STL_ROITH_INFO  StlRoi5;           ///< ROI5 for edge map
	IME_STL_ROITH_INFO  StlRoi6;           ///< ROI6 for edge map
	IME_STL_ROITH_INFO  StlRoi7;           ///< ROI7 for edge map
} IME_STL_ROI_INFO;

/**
    IME structure - histogram of statistic

    @note integrated structure
*/
typedef struct _IME_STL_HIST_INFO {
	IME_STL_HIST_SET_SEL    SetSel;          ///< operation set selection
	IME_POS_INFO            StlHistPos;     ///< start posotion for edge statistic
	IME_SIZE_INFO           StlHistSize;     ///< image size for edge statistic
	IME_STL_ACCTAG_INFO     StlHistAccTag;  ///< histogram accumulation target
} IME_STL_HIST_INFO;

/**
    IME structure - statistical parameters

    @note integrated structure
*/
typedef struct _IME_STL_INFO {
	IME_FUNC_EN         StlEnable;      ///< enable of statistic
	IME_FUNC_EN         StlFtrEnable;    ///< filter enable of statistic
	IME_STL_EDGE_INFO   StlEdgeMap;     ///< edge map of statistic
	IME_STL_HIST_INFO   StlHist0;        ///< edge histogram of set0
	IME_STL_HIST_INFO   StlHist1;        ///< edge histogram of set1
	IME_STL_ROI_INFO    StlRoi;         ///< ROI threshold of edge map
	IME_STL_IMGOUT_SEL  StlImgOutType;  ///< output before or after filter image

	UINT32              uiStlEdgeMapLofs; ///< edge map output lineoffset
	UINT32              uiStlEdgeMapAddr; ///< edge map output address
	UINT32              uiStlHistAddr;    ///< output address for edge statistic
} IME_STL_INFO;

/**
    IME structure - get statistical parameters
*/
typedef struct _IME_GET_STL_INFO {
	UINT32 uiGetHistMax0;      ///< max value of histogram area of set0
	UINT32 uiGetAccTagBin0;    ///< hit bin of histogram accumulation of set0

	UINT32 uiGetHistMax1;      ///< max value of histogram area of set1
	UINT32 uiGetAccTagBin1;    ///< hit bin of histogram accumulation of set1
} IME_GET_STL_INFO;


/**
    IME structure - stitching parameters
*/
typedef struct _IME_STITCH_INFO {
	UINT32 uiStitchPos;                     ///< stitching position of image
	BOOL bLofsUpdate;                       ///< stitching output lineoffset update, or not
	IME_LINEOFFSET_INFO StitchLineoffset;   ///< stitching image lineoffset
	BOOL bDmaAddrUpdate;                    ///< stitching output DMA address update, or not
	IME_DMA_ADDR_INFO StitchDmaAddr;        ///< stitching image DMA address
} IME_STITCH_INFO;


/**
    IME structure - privacy mask coordinate
*/
typedef struct _IME_PM_POINT {
	INT32 uiX; ///< coordinate of horizontal direction
	INT32 uiY; ///< coordinate of vertical direction
} IME_PM_POINT;

/**
    IME structure - privacy mask using color mask
*/
typedef struct _IME_PM_MASK_COLOR {
	UINT32 uiY; ///< Y channel value, range: [0, 255]
	UINT32 uiU; ///< U channel value, range: [0, 255]
	UINT32 uiV; ///< V channel value, range: [0, 255]
} IME_PM_MASK_COLOR;



/**
    IME structure - privacy mask parameter of each set
*/
typedef struct _IME_PM_PARAM {
	IME_FUNC_EN         PmEnable;       ///< privacy mask function enable
	IME_PM_POINT        PmCoord[4];     ///< privacy mask 4 coordinates, must be Convex Hull
	IME_PM_MASK_TYPE    PmMskType;      ///< privacy mask type selection
	IME_PM_MASK_COLOR   PmColor;        ///< privacy mask using YUV color
	UINT32              uiPmWet;        ///< privacy mask alpha weight, range:[0, 255]
} IME_PM_PARAM;


/**
    IME structure - privacy mask parameters
*/
typedef struct _IME_PM_INFO {
	IME_PM_PARAM PmSet0;                    ///< privacy mask set0
	IME_PM_PARAM PmSet1;                    ///< privacy mask set1
	IME_PM_PARAM PmSet2;                    ///< privacy mask set2
	IME_PM_PARAM PmSet3;                    ///< privacy mask set3
	IME_PM_PARAM PmSet4;                    ///< privacy mask set4
	IME_PM_PARAM PmSet5;                    ///< privacy mask set5
	IME_PM_PARAM PmSet6;                    ///< privacy mask set6
	IME_PM_PARAM PmSet7;                    ///< privacy mask set7

	IME_SIZE_INFO       PmPxlImgSize;  ///< pixelation input image size of privacy mask
	IME_PM_PXL_BLK_SIZE PmPxlBlkSize;   ///< block size of pixelation
	UINT32              PmPxlLofs;      ///< pixelation input image lineoffset
	UINT32              PmPxlDmaAddr;   ///< pixelation input imaeg buffer address
} IME_PM_INFO;


/**
    IME structure - 3DNR motion estimator search parameters
*/
typedef struct _IME_3DNR_MER_PARAM_ {
	UINT32                  *puiDetailPenalty;  ///< 3DRS detail penalty, 6 elements, range = [0, 15]
	UINT32                  *puiSadPenalty;     ///< 3DRS sad penalty, 8 elements, range = [0, 15]

	IME_3DNR_MER_MVDIF_MODE    MvDifMode;      ///< 3DRS mv difference mode selection
	UINT32              uiMvDifShift;           ///< 3DRS mv difference shift
	UINT32              uiMvDifCoring;          ///< 3DRS mv difference coring
	UINT32              uiMvDifRatio;           ///< 3DRS mv difference ratio

	IME_FUNC_EN PeriodicEn;                     ///< 3DRS periodic enable
	IME_FUNC_EN PeriodicSmallerMvDifEn;             ///< 3DRS periodic prefer smaller mvdiff enable
	IME_FUNC_EN PeriodicAplsadRulEn;            ///< 3DRS periodic refer to apl
	UINT32  uiPeriodicSadSimilarDif;            ///< 3DRS Periodic sad similar difference
	UINT32  *puiPeriodicSadSimilar;             ///< 3DRS Periodic sad similar, 4 elements

	IME_3DNR_MER_SUB_MV_MODE SubMvMode;        ///< step size 4 for sub mv
	UINT32 uiSadT0Shift;                        ///< shift T0 sad, max 6 since max sad 14bit
	UINT32 uiBvToZmv;                           ///<
	UINT32 uiZeroMvCoringTh;                    ///<

	IME_FUNC_EN BvMvDiffForce;                  ///< force mv difference threshold enable
	UINT32 uiBvMvDiffTh;                        ///< set force mv_diff_thd
	UINT32 *puiCandiatePosX;                    ///< mv position for horizontal direction, 7 elements
	UINT32 *puiCandiatePosY;                    ///< mv position for vertical direction, 7 elements
} IME_3DNR_MER_PARAM;


/**
    IME structure - 3DNR motion parameters
*/
typedef struct _IME_3DNR_MOTION_INFO_ {
	IME_3DNR_MOTION_SC_3D_MODE  MotionSCMode;  ///< scene change 3d dist
	UINT32                      MotionSCRatio; ///< scene change detect
	UINT32                      MotionSCCoring; ///< scene change detect
	UINT32                      MotionSCTensor; ///< scene change detect

	UINT32                      MotionTensorMixTh0; ///< determin edge for mix tensor & ratio
	UINT32                      MotionTensorMixTh1; ///< determin edge for mix tensor & ratio

	UINT32                      *puiMotionTensorValMap; ///< tensor remapping, 4 elements, range = [0, 63]
	UINT32                      *puiMotionMap;          ///< decide number of full search patches, 5 elements, range = [0, 63]
} IME_3DNR_MOTION_INFO;


/**
    IME structure - 3DNR luma function control
*/
typedef struct _IME_3DNR_LUMA_FUNC_CTRL_ {
	IME_FUNC_EN             FreqSoftEn;     ///< freq. soft function enable
	IME_FUNC_EN             DitherBlendEn;  ///< dither for blending output enable
	IME_FUNC_EN             FullSearch2DEn; ///< full search enable for 2D
	IME_FUNC_EN             Trans3DEn;      ///< 3D transform enable
	IME_FUNC_EN             FilmAwareEn;    ///< film aware enable
	IME_FUNC_EN             TensorModeEn;   ///< always select bv while film mode
	IME_FUNC_EN             EdgeModifyEn;   ///< edge weighting enable
	IME_FUNC_EN             PiGmvEn;        ///<
} IME_3DNR_LUMA_FUNC_CTRL;

/**
    IME structure - 3DNR luma parameters
*/
typedef struct _IME_3DNR_LUMA_PARAM_ {
	UINT32                  uiSigma;        ///< square strength
	UINT32                  uiHardTh;       ///< sigma adjustment with tiny movement

	UINT32                  uiGlbClrTSad;           ///<
	UINT32                  uiPatchSimilarity;      ///< 3d full search patch similarity
	UINT32                  uiPatchReserveTh1;      ///< 3d full search patch sad threshold
	UINT32                  uiZeroMvCoringTh;       ///<
	UINT32                  uiBlKFlatTh;            ///< detail value threshold to judge as flat

	IME_3DNR_MER_PARAM     MerSearchInfo;    ///< 3DNR motion estimator search parameters
	IME_3DNR_MOTION_INFO    MotionInfo;             ///< 3DNR motion parameters

	UINT32                  *puiSnrRatio;   /// set SNR ratio in diff full search patch, 6 elements, range = [0, 127]
	UINT32                  *puiTnrRatio;   /// set tnr ratio in diff full search patch, 6 elements, range = [0, 127]
	UINT32                  *puiLumaGain;   /// SQUARE strength adjust at luma, 8 elements, range = [0, 255]
	UINT32                  *puiFreq;       /// set diff ratio in diff freq, 7 elements, range = [0, 127]
	UINT32                  *puiSnrFreq;    /// snr ratio depend on frequency, 4 elements, range = [0, 255]
} IME_3DNR_LUMA_PARAM;

/**
    IME structure - 3DNR parameters for luma
*/
typedef struct _IME_3DNR_LUMA_INFO_ {
	IME_FUNC_EN             LumaNrEn;       ///< luma chennel enable
	IME_3DNR_LUMA_FUNC_CTRL LumaCtrlInfo;       ///< luma function control info.
	IME_3DNR_LUMA_PARAM     LumaInfo;       ///< luma parameter info.
	UINT32 uiLumaMvInAddr;      ///< motion vector input address
	UINT32 uiLumaMvOutAddr;     ///< motion vector output address
} IME_3DNR_LUMA_INFO;


/**
    IME structure - 3DNR chroma function control
*/
typedef struct _IME_3DNR_CHROMA_FUNC_CTRL_ {
	IME_FUNC_EN                     ModifyDiffEn;                       ///< refine the pseudo motion by strong SNR
	IME_FUNC_EN                     MotionExpandEn;                     ///< motion expand enable
	IME_FUNC_EN                     ExpandDiffEn;                       ///< enable lpf for temporal difference
	IME_3DNR_MOTION_LIMIT_IN_SEL    MotionInSel;    ///< motion detection input using
	IME_3DNR_MOTION_FREEZE          MotionFreeze;   ///< motion control
} IME_3DNR_CHROMA_FUNC_CTRL;


/**
    IME structure - 3DNR chroma parameter
*/
typedef struct _IME_3DNR_CHROMA_PARAM_ {
	UINT32                          uiExpectNoiseLuma;  ///< expected noise strength of luma, range = [0, 1023]
	UINT32                          uiExpectNoiseChroma;    ///< expected noise strength of chroma, range = [0, 1023]
	UINT32                          uiExpandLevel;          ///<

	UINT32                          uiFalseCurve0;          ///< Low threshold of difference
	UINT32                          uiFalseCurve1;          ///< Low threshold of difference
	UINT32                          uiMixYCFalseYWet;       ///< Y/C mix luma weight for difference calculation
	UINT32                          uiMixYCFalseCWet;       ///< Y/C mix chroma weight for difference calculation
	UINT32                          uiMixYCLumaYWet;        ///< Y/C mix luma weight for luma channel
	UINT32                          uiMixYCLumaCWet;        ///< Y/C mix chroma weight for luma channel
	UINT32                          uiMixYCChromaYWet;      ///< Y/C mix luma weight for chroma channel
	UINT32                          uiMixYCChromaCWet;      ///< Y/C mix chroma weight for chroma channel

	UINT32                          *puiLumaGain;   ///< TNR strength adjust at luma, 8 elements, range = [0, 255]
	UINT32                          *puiMotionCtrl; ///< mapped motion value of Xth motion level, 16 elements, range = [0, 255]
	UINT32                          *puiMap2DWets;  ///< 4 elements, puiMap2DWeights[0]: chroma threshold for still; puiMap2DWeights[1]: chroma weighting for HSNR when still; puiMap2DWeights[2]: chroma threshold for motion; puiMap2DWeights[3]: chroma curve slope
	UINT32                          *puiMap3DWets;  ///< 4 elements, puiMap3DWeights[0]: chroma threshold for still; puiMap3DWeights[1]: chroma weighting for HSNR when still; puiMap3DWeights[2]: chroma threshold for motion; puiMap3DWeights[3]: chroma curve slope
	UINT32                          *puiMixTh;              ///< luma threshold to mix the motion detect value, and chroma threshold to mix the motion detect value, 4 elements
	UINT32                          *puiLevelMap;           ///< Memc level mapping for motion noise, 16 elements
} IME_3DNR_CHROMA_PARAM;


/**
    IME structure - 3DNR Y parameter
*/
typedef struct _IME_3DNR_Y_PARAM_ {
	UINT32                          uiExpectNoiseStrth;  ///< Y expected noise strength in spatial, range = [0, 1023]
	UINT32                          *puiMap2DWets;  ///< 4 elements, puiMap2DWeights[0]: Y threshold for still; puiMap2DWeights[1]: Y weighting for HSNR when still; puiMap2DWeights[2]: Y threshold for motion; puiMap2DWeights[3]: Y curve slope
	UINT32                          *puiMap3DWets;  ///< 4 elements, puiMap3DWeights[0]: Y threshold for still; puiMap3DWeights[1]: Y weighting for HSNR when still; puiMap3DWeights[2]: Y threshold for motion; puiMap3DWeights[3]: Y curve slope
} IME_3DNR_Y_PARAM;



/**
    IME structure - 3DNR parameters for chroma
*/
typedef struct _IME_3DNR_CHROMA_INFO_ {
	IME_FUNC_EN                     ChromaNrEn;             ///< chroma channel enable
	IME_3DNR_CHROMA_FUNC_CTRL       ChromaCtrlInfo;         ///< chroma function control information
	IME_3DNR_CHROMA_PARAM           ChromaInfo;             ///< chroma parameter information
	IME_FUNC_EN                     YNrEn;             		///< Y channel enable
	IME_3DNR_Y_PARAM				YInfo;					///< Y parameter information
	UINT32                          uiChromaNrMoInLofs;     ///< motion input lineoffset
	UINT32                          uiChromaNrMoInAddr;     ///< motion input address
	UINT32                          uiChromaNrMoOutLofs;    ///< motion output lineoffset
	UINT32                          uiChromaNrMoOutAddr;    ///< motion output address
} IME_3DNR_CHROMA_INFO;


/**
    IME structure - codec K-table parameters
*/
typedef struct _IME_CODEC_KT_INFO_ {
	UINT32          uiKt0;   ///< K-table information for codec
	UINT32          uiKt1;   ///< K-table information for codec
	UINT32          uiKt2;   ///< K-table information for codec
} IME_CODEC_KT_INFO;


/**
    IME structure - encode parameters
*/
typedef struct _IME_ENCODE_INFO_ {
	IME_FUNC_EN         EncodeEn;           ///< encoder enable
	IME_SINFO_FMT       EncodeSInfoFmt;     ///< encode side-information output format
	IME_CODEC_KT_INFO   EncodeKTab;         ///< K-table information for encoder
	IME_FUNC_EN         EncodeClearKTabEn;  ///< encoder clear K-table enable
	IME_FUNC_EN         EncodeUpdateKTabEn; ///< encoder clear K-table enable
	UINT32          uiEncodeSideInfoAddr;   ///< encoder side information
} IME_ENCODE_INFO;


/**
    IME structure - decoder parameters for 3DNR
*/
typedef struct _IME_3DNR_DECODE_INFO_ {
	IME_FUNC_EN         DecodeEn;               ///< 3DNR input reference decoder enable
	IME_CODEC_KT_INFO   DecodeKTab;             ///< K-table information for decoder
	IME_FUNC_EN         EncodeClearKTabEn;  ///< encoder clear K-table enable
	IME_FUNC_EN         EncodeUpdateKTabEn; ///< encoder clear K-table enable
	UINT32              uiDecodeSideInfoAddr;   ///< decoder side information
} IME_3DNR_DECODE_INFO;


/**
    IME structure - 3DNR parameters
*/
typedef struct _IME_3DNR_INFO_ {
	IME_FUNC_EN     TplNrEn;        ///< 3DNR enable
	IME_3DNR_LUMA_INFO TplLuma;     ///< 3DNR for luminance channel
	IME_3DNR_CHROMA_INFO TplChroma; ///< 3DNR for chroma channel
	IME_LINEOFFSET_INFO PrvImgLofs; ///< 3DNR buffer lineoffset of preivous image
	IME_DMA_ADDR_INFO   PrvImgAddr; ///< 3DNR buffer address of preivous image
	IME_3DNR_DECODE_INFO TplDecode; ///< 3DNR reference input decode
} IME_3DNR_INFO;


//------------------------------------------------------------------------------
// global flow structure

/**
    IME structure - grouped function parameters

    @note integrated structure - used to ime_setMode for initial state
*/
typedef struct _IME_IQ_FLOW_INFO {
	IME_FUNC_EN                     *pCmfInfo;                  ///< chroma median filter parameter, if useless, please set NULL
	IME_CHROMA_ADAPTION_INFO        *pChromaAdaptionInfo;       ///< chroma adaption parameters, if useless, please set NULL
	IME_CHROMA_ADAPTION_SUBOUT_INFO *pChromaAdaptionSuboutInfo; ///< chroma adaption subout parameters, if useless, please set NULL
	IME_DBCS_INFO                   *pDbcsInfo;                 ///< dark and bright region chroma supression parameters, if useless, please set NULL
	IME_SSR_INFO                    *pSsrInfo;                  ///< single image supre-resolution parameters, if useless, please set NULL
	IME_CST_INFO                    *pColorSpaceTrans;          ///< color space transformation parameters, if useless, please set NULL
	IME_FILM_GRAIN_INFO             *pFilmGrainInfo;            ///< film grain parameters, if useless, please set NULL
	IME_STAMP_INFO                  *pDataStampInfo;            ///< Data stamp parameters, if useless, please set NULL
	IME_FUNC_EN                     *pP2IInfo;                  ///< progressive to interlaced parameter, if useless, please set NULL
	IME_STL_INFO                    *pStlInfo;                  ///< edge statistic parameters, if useless, please set NULL
	IME_PM_INFO                     *pPmInfo;                   ///< privacy mask parameters, if useless, please set NULL
	IME_3DNR_INFO                   *pTplNrInfo;                ///< 3DNR parameters, if useless, please set NULL
} IME_IQ_FLOW_INFO;


/**
    IME structure - all input path parameters

    @note integrated structure
*/
typedef struct _IME_INPATH_INFO {
	IME_SIZE_INFO             InSize;               ///< input image size
	IME_INPUT_FORMAT_SEL      InFormat;                ///< input image format
	IME_LINEOFFSET_INFO       InLineoffset;            ///< input line offset
	IME_DMA_ADDR_INFO         InAddr;               ///< input DMA address
} IME_INPATH_INFO;


/**
    IME structure - get input path information

    @note integrated structure
*/
typedef struct _IME_GET_INPATH_INFO {
	UINT32					  InPathSrc;				///< input source
	IME_SIZE_INFO             InPathSize;               ///< input image size
	IME_INPUT_FORMAT_SEL      InPathFormat;                ///< input image format
	IME_LINEOFFSET_INFO       InPathLineoffset;            ///< input line offset
	IME_DMA_ADDR_INFO         InPathAddr;               ///< input DMA address
} IME_GET_INPATH_INFO;
//------------------------------------------------------------------------------


/**
    IME structure - all output path parameters

    @note integrated structure
*/
typedef struct _IME_OUTPATH_INFO {
	IME_FUNC_EN             OutPathEnable;          ///< path enable
	IME_FUNC_EN             OutPathDramEnable;      ///< path output to dram enable
	IME_FUNC_EN             OutPathSprtOutEnable;   ///< path separated output to dram enable, also set "OutPathSprtPos"
	IME_OUTDST_CTRL_SEL     OutPathOutDest;         ///< path output destination; path1: dram/H264, others: dram
	IME_OUTPUT_FORMAT_INFO  OutPathImageFormat;     ///< path output image format
	IME_SIZE_INFO           OutPathScaleSize;       ///< path image scaling output size
	IME_SCALE_FACTOR_INFO   OutPathScaleFactors;    ///< path image scaling facotrs
	IME_SCALE_FILTER_INFO   OutPathScaleFilter;     ///< path scaling filter parameters
	IME_SCALE_METHOD_SEL    OutPathScaleMethod;     ///< path scale method
	IME_SCALE_ENH_INFO      OutPathScaleEnh;        ///< path scale enhancement, for bicubic scaling method only
	IME_DMA_ADDR_INFO       OutPathAddr;            ///< path output DMA address
	IME_POS_INFO            OutPathCropPos;         ///< path crop position
	IME_SIZE_INFO           OutPathOutSize;         ///< path image crop output size
	IME_LINEOFFSET_INFO     OutPathOutLineoffset;   ///< path output lineoffset

	IME_DMA_ADDR_INFO       OutPathAddr2;           ///< path output DMA address for separating output
	IME_LINEOFFSET_INFO     OutPathOutLineoffset2;        ///< path output lineoffset for separating output
	UINT32                  OutPathSprtPos;           ///< path output separating position

	IME_ENCODE_INFO         OutPathEncode;        ///< path encode enable, only for output path1
} IME_OUTPATH_INFO;
//------------------------------------------------------------------------------
/**
    IME structure - get output path information

    @note integrated structure
*/
typedef struct _IME_GET_OUTPATH_INFO {
	IME_FUNC_EN             OutPathEnable;          ///< path enable
	IME_OUTDST_CTRL_SEL     OutPathOutDest;         ///< path output destination; path1: dram/H264, others: dram
	IME_OUTPUT_FORMAT_INFO  OutPathImageFormat;     ///< path output image format
	IME_SIZE_INFO           OutPathScaleSize;       ///< path image scaling output size


	IME_SCALE_METHOD_SEL    OutPathScaleMethod;     ///< path scale method
	IME_DMA_ADDR_INFO       OutPathAddr;            ///< path output DMA address
	IME_POS_INFO            OutPathCropPos;         ///< path crop position
	IME_SIZE_INFO           OutPathOutSize;         ///< path image crop output size
	IME_LINEOFFSET_INFO     OutPathOutLineoffset;   ///< path output lineoffset
} IME_GET_OUTPATH_INFO;
//------------------------------------------------------------------------------


/**
    IME structure - full engine parameters

    @note integrated structure - used to ime_setMode for initial state
*/
typedef struct _IME_MODE_PRAM {
	// input info
	IME_OPMODE                OperationMode;      ///< IME performing mode
	IME_INPATH_INFO           InPathInfo;         ///< IME input path parameters

	//output info
	IME_OUTPATH_INFO          OutPath1;           ///< IME output path1 parameters
	IME_OUTPATH_INFO          OutPath2;           ///< IME output path2 parameters
	IME_OUTPATH_INFO          OutPath3;           ///< IME output path3 parameters
	IME_OUTPATH_INFO          OutPath4;           ///< IME output path4 parameters
	IME_OUTPATH_INFO          OutPath5;           ///< IME output path3 parameters

	UINT32                    uiInterruptEnable;  ///< interrupt enable
	IME_HV_STRIPE_INFO        SetStripe;

	IME_IQ_FLOW_INFO          *pImeIQInfo;        ///< IQ related parameters, if useless, please set NULL
} IME_MODE_PRAM, *pIME_MODE_PRAM;



/**
    IME structure - change DMA channel burst length parameters
*/
typedef struct _IME_BURST_LENGTH_ {
	IME_BURST_SEL BurstLenInputY;      ///< input Y, recommend: 64 words for DRAM1 and 32 words for DRAM2
	IME_BURST_SEL BurstLenInputU;      ///< input U, recommend: 64 words for DRAM1 and 32 words for DRAM2
	IME_BURST_SEL BurstLenInputV;      ///< input V, recommend: 64 words for DRAM1 and 32 words for DRAM2
	IME_BURST_SEL BurstLenInputLCA;    ///< LCA input, recommend: 64 words for DRAM1 and 32 words for DRAM2
	IME_BURST_SEL BurstLenStamp;       ///< data stamp input, recommend: 64 words for DRAM1 and 32 words for DRAM2
	IME_BURST_SEL BurstLenTplY;        ///< 3DNR of luma input, recommend: 64 words for DRAM1 and 32 words for DRAM2
	IME_BURST_SEL BurstLenTplC;        ///< 3DNR of chroma input, recommend: 128 words for DRAM1 and 64 words for DRAM2
	IME_BURST_SEL BurstLenTplInMo;     ///< 3DNR of chroma input motion, recommend: 64 words for DRAM1 and 32 words for DRAM2
	IME_BURST_SEL BurstLenPrivacyMask; ///< privacy mask input pixelation, recommend: 64 words for DRAM1 and 32 words for DRAM2
	IME_BURST_SEL BurstLenOutputP1Y;   ///< output path1 Y, recommend: 128 words for DRAM1 and 64 words for DRAM2
	IME_BURST_SEL BurstLenOutputP1U;   ///< output path1 U, recommend: 128 words for DRAM1 and 64 words for DRAM2
	IME_BURST_SEL BurstLenOutputP1V;   ///< output path1 V, recommend: 64 words for DRAM1 and 32 words for DRAM2
	IME_BURST_SEL BurstLenOutputP2Y;   ///< output path2 Y, recommend: 128 words for DRAM1 and 64 words for DRAM2
	IME_BURST_SEL BurstLenOutputP2U;   ///< output path2 U, recommend: 128 words for DRAM1 and 64 words for DRAM2
	IME_BURST_SEL BurstLenOutputP3Y;   ///< output path3 Y, recommend: 64 words for DRAM1 and 32 words for DRAM2
	IME_BURST_SEL BurstLenOutputP3U;   ///< output path3 U, recommend: 64 words for DRAM1 and 32 words for DRAM2
	IME_BURST_SEL BurstLenOutputP4Y;   ///< output path4 Y, recommend: 64 words for DRAM1 and 32 words for DRAM2
	IME_BURST_SEL BurstLenOutputP5Y;   ///< output path5 Y, recommend: 64 words for DRAM1 and 32 words for DRAM2
	IME_BURST_SEL BurstLenOutputP5U;   ///< output path5 U, recommend: 64 words for DRAM1 and 32 words for DRAM2
	IME_BURST_SEL BurstLenSuboutLCA;   ///< LCA subout, recommend: 64 words for DRAM1 and 32 words for DRAM2
	IME_BURST_SEL BurstLenTplOutMo;    ///< 3DNR of chroma output motion, recommend: 64 words for DRAM1 and 32 words for DRAM2
} IME_BURST_LENGTH;

//------------------------------------------------------------------------------
// flow control APIs

/**
    IME engine is opened or not

    To check IME engine is opened or closed.

    @param[in] VOID

    @return BOOL  Status of performing this API.\n
    -@b TRUE:   Engine is opened.\n
    -@b FALSE:  Engine is closed.\n
*/
extern BOOL ime_isOpen(VOID);
//------------------------------------------------------------------------------

/**
    Open IME engine

    This function should be called before calling any other functions.

    @param[in] pImeOpenInfo Open and get engine resource

    @return ER  error code\n
    -@b E_OK:   setting done\n
    -@b Others: setting error\n
*/
extern ER ime_open(IME_OPENOBJ *pImeOpenInfo);
//------------------------------------------------------------------------------

/**
    Set IME engine mode and parameters

    @param[in] pEngineInfo mode information and parameters.\n

    @return ER  error code\n
    -@b E_OK:   setting done\n
    -@b Others: setting error\n
*/
extern ER ime_setMode(IME_MODE_PRAM *pEngineInfo);
//------------------------------------------------------------------------------

/**
    IME engine start

    @param[in] VOID

    @return ER  error code\n
    -@b E_OK:   setting done\n
    -@b Others: setting error\n
*/
extern ER ime_setStart(VOID);
//------------------------------------------------------------------------------

/**
    IME engine pause

    @param[in] VOID

    @return ER  error code\n
    -@b E_OK:   setting done\n
    -@b Others: setting error\n
*/
extern ER ime_pause(VOID);
//------------------------------------------------------------------------------

/**
    IME engine close

    @param[in] VOID

    @return ER  error code\n
    -@b E_OK:   setting done\n
    -@b Others: setting error\n
*/
extern ER ime_close(VOID);
//------------------------------------------------------------------------------

/**
    Wait system flag when frame-end occurred

    @param[in] IsClearFlag  Clear flag selection.\n

    @return ER error code\n
    -@b E_OK: desired flag is done.\n
*/
extern ER ime_waitFlagFrameEnd(IME_FLAG_CLEAR_SEL IsClearFlag);
//------------------------------------------------------------------------------


/**
    Clear frame-end flag of system

    @param[in] VOID\n

    @return VOID\n
*/
extern VOID ime_clearFlagFrameEnd(VOID);
//------------------------------------------------------------------------------


/**
    Set Stripe Information

    @param[in] pStripeInfo stripe information of horizontal and vertical directions.

    @return ER  error code\n
    -@b E_OK:   setting done\n
    -@b Others: setting error\n
*/
extern ER ime_chgStripeParam(IME_HV_STRIPE_INFO *pStripeInfo);
//------------------------------------------------------------------------------

/**
    Set Stripe Information

    @param[in] pStripeInfo stripe information of horizontal and vertical directions.

    @return ER  error code\n
    -@b E_OK:   setting done\n
    -@b Others: setting error\n
*/
extern ER ime_calD2D_HStripeSize(IME_STRIPE_CAL_INFO *pCalStpInfo, IME_STRIPE_INFO *pStripeH);
//------------------------------------------------------------------------------


/**
    Get input stripe size information

    @param[in] pGetStripeInfo  information of input stripe size

    @return VOID\n
*/
extern VOID ime_getInputPathStripeInfo(IME_HV_STRIPE_INFO *pGetStripeInfo);
//------------------------------------------------------------------------------


/**
    Set input path parameters

    @param[in] pSetInInfo Input path setting\n
    @param[in] LoadType load type selection\n

    @return ER  error code\n
    -@b E_OK:   setting done\n
    -@b Others: setting error\n
*/
extern ER ime_chgInputPathParam(IME_INPATH_INFO *pSetInInfo);
//------------------------------------------------------------------------------

/**
    Set output path parameters

    @param[in] OutPathSel output path selection\n
    @param[in] pSetOutInfo output path setting\n

    @return ER  error code\n
    -@b E_OK:   setting done\n
    -@b Others: setting error\n
*/
extern ER ime_chgOutputPathParam(IME_PATH_SEL OutPathSel, IME_OUTPATH_INFO *pSetOutInfo);
//------------------------------------------------------------------------------

/**
    Change Output Path Enable

    @param[in] PathSel  output path selection\n
    @param[in] SetEn    enable or disable setting\n

    @return ER  error code\n
    -@b E_OK:   setting done\n
    -@b Others: setting error\n
*/
extern ER ime_chgOutputPathEnable(IME_PATH_SEL PathSel, IME_FUNC_EN SetEn);
//------------------------------------------------------------------------------

/**
    Change Output Path DRAM-Out Enable

    @param[in] PathSel  output path selection\n
    @param[in] SetEn    enable or disable setting\n

    @return ER  error code\n
    -@b E_OK:   setting done\n
    -@b Others: setting error\n
*/
extern ER ime_chgOutputPathDramOutEnable(IME_PATH_SEL PathSel, IME_FUNC_EN SetEn);
//------------------------------------------------------------------------------

/**
    Change Output Scaling Filter Parameters

    @param[in] PathSel      output path selection\n
    @param[in] pSetSclFilter   scale filter setting\n

    @return ER  error code\n
    -@b E_OK:   setting done\n
    -@b Others: setting error\n
*/
extern ER ime_chgOutputScalingFilterParam(IME_PATH_SEL PathSel, IME_SCALE_FILTER_INFO *pSetSclFilter);
//------------------------------------------------------------------------------

/**
    Change Output Scaling Factor Parameters

    @param[in] PathSel      output path selection\n
    @param[in] pSetSclFactor   scale factor setting\n

    @return ER  error code\n
    -@b E_OK:   setting done\n
    -@b Others: setting error\n
*/
extern ER ime_chgOutputScalingFactorParam(IME_PATH_SEL PathSel, IME_SCALE_FACTOR_INFO *pSetSclFactor);
//------------------------------------------------------------------------------


/**
    Change Output Scaling Enhancement Parameters

    @param[in] PathSel      output path selection\n
    @param[in] pSetSclEnh   image enhancement setting\n

    @return ER  error code\n
    -@b E_OK:   setting done\n
    -@b Others: setting error\n
*/
extern ER ime_chgOutputScalingEnhanceParam(IME_PATH_SEL PathSel, IME_SCALE_ENH_INFO *pSetSclEnh);
//------------------------------------------------------------------------------

/**
    Change Chroma Median Filter Enable

    @param[in] SetEn         function enable/disable

    @return ER      error code\n
    -@b E_OK:       setting done\n
    -@b Others:     setting error\n
*/
extern ER ime_chgChromaMedianFilterEnable(IME_FUNC_EN SetEn);
//------------------------------------------------------------------------------

/**
    Change Chroma Adaptation Enable

    @param[in] SetEn         function enable/disable

    @return ER      error code\n
    -@b E_OK:       setting done\n
    -@b Others:     setting error\n
*/
extern ER ime_chgChromaAdaptEnable(IME_FUNC_EN SetEn);
//------------------------------------------------------------------------------

/**
    Change Chroma Adaptation - Chroma Adjustment Enable

    @param[in] SetEn         function enable/disable

    @return ER      error code\n
    -@b E_OK:       setting done\n
    -@b Others:     setting error\n
*/
extern ER ime_chgChromaAdaptChromaAdjustEnable(IME_FUNC_EN SetEn);
//------------------------------------------------------------------------------

/**
    Change Chroma Adaptation - Luma Suppression Enable

    @param[in] SetEn         function enable/disable

    @return ER      error code\n
    -@b E_OK:       setting done\n
    -@b Others:     setting error\n
*/
extern ER ime_chgChromaAdaptLumaSuppressEnable(IME_FUNC_EN SetEn);
//------------------------------------------------------------------------------

/**
    Change Chroma Adaptation Image Parameters

    @param[in] pSetInfo       image information

    @return ER      error code\n
    -@b E_OK:       setting done\n
    -@b Others:     setting error\n
*/
extern ER ime_chgChromaAdaptImageParam(IME_CHROMA_ADAPTION_IMAGE_INFO *pSetInfo);
//------------------------------------------------------------------------------

/**
    Change Chroma Adaptation - Chroma Adjustment Parameters

    @param[in] pSetInfo       chroma adjustment parameters

    @return ER      error code\n
    -@b E_OK:       setting done\n
    -@b Others:     setting error\n
*/
extern ER ime_chgChromaAdaptChromaAdjustParam(IME_CHROMA_ADAPTION_CA_INFO *pSetInfo);
//------------------------------------------------------------------------------

/**
    Change Chroma Adaptation Parameters

    @param[in] pSetInfo       chroma adjustment parameters

    @return ER      error code\n
    -@b E_OK:       setting done\n
    -@b Others:     setting error\n
*/
extern ER ime_chgChromaAdaptParam(IME_CHROMA_ADAPTION_IQC_INFO *pSetInfo);
//------------------------------------------------------------------------------

/**
    Change Chroma Adaptation - Luma Suppress Parameters

    @param[in] pSetInfo       chroma adjustment parameters

    @return ER      error code\n
    -@b E_OK:       setting done\n
    -@b Others:     setting error\n
*/
extern ER ime_chgChromaAdaptLumaSuppressParam(IME_CHROMA_ADAPTION_IQL_INFO *pSetInfo);
//------------------------------------------------------------------------------

/**
    Get Chroma Adaptation - Image size

    @param[in] pGetSize     get imaeg size

    @return VOID\n
*/
extern VOID ime_getChromaAdaptImageSizeInfo(IME_SIZE_INFO *pGetSize);
//------------------------------------------------------------------------------

/**
    Get Chroma Adaptation - Lineoffset

    @param[in] pGetLofs     get lineoffset size

    @return VOID\n
*/
extern VOID ime_getChromaAdaptLineoffsetInfo(IME_LINEOFFSET_INFO *pGetLofs);
//------------------------------------------------------------------------------

/**
    Get Chroma Adaptation - DMA address

    @param[in] pGetLofs     get DMA address

    @return VOID\n
*/
extern VOID ime_getChromaAdaptDmaAddrInfo(IME_DMA_ADDR_INFO *pGetAddr0, IME_DMA_ADDR_INFO *pGetAddr1);
//------------------------------------------------------------------------------

/**
    Change Chroma Adaptation Subout Enable

    @param[in] SetEn         function enable/disable

    @return ER      error code\n
    -@b E_OK:       setting done\n
    -@b Others:     setting error\n
*/
extern ER ime_chgChromaAdaptSuboutEnable(IME_FUNC_EN SetEn);
//------------------------------------------------------------------------------

/**
    Change Chroma Adaptation Subout Scale Parameters

    @param[in] pSetInfo      scale info.

    @return ER      error code\n
    -@b E_OK:       setting done\n
    -@b Others:     setting error\n
*/
extern ER ime_chgChromaAdaptSuboutParam(IME_CHROMA_APAPTION_SUBOUT_PARAM *pSetInfo);
//------------------------------------------------------------------------------

/**
    Change Chroma Adaptation Subout Lineoffset

    @param[in] pSetLofs      image lineoffset

    @return ER      error code\n
    -@b E_OK:       setting done\n
    -@b Others:     setting error\n
*/
extern ER ime_chgChromaAdaptSuboutLineoffsetInfo(IME_LINEOFFSET_INFO *pSetLofs);
//------------------------------------------------------------------------------


/**
    Get Chroma Adaptation Subout Lineoffset

    @param[in] pGetLofs      image lineoffset

    @return VOID
*/
extern VOID ime_getChromaAdaptSuboutLineoffsetInfo(IME_LINEOFFSET_INFO *pGetLofs);
//------------------------------------------------------------------------------



/**
    Change Chroma Adaptation Subout DMA Address

    @param[in] pSetLofs      DMA address

    @return ER      error code\n
    -@b E_OK:       setting done\n
    -@b Others:     setting error\n
*/
extern ER ime_chgChromaAdaptSuboutDmaAddrInfo(IME_DMA_ADDR_INFO *pSetAddr);
//------------------------------------------------------------------------------

/**
    Get Chroma Adaptation Subout DMA Address

    @param[in] pGetLofs      DMA address

    @return VOID
*/
extern VOID ime_getChromaAdaptSuboutDmaAddrInfo(IME_DMA_ADDR_INFO *pGetAddr);
//------------------------------------------------------------------------------



/**
    Change Color Space Transformation Parameters

    Change Color Space Transformation Parameters

    @param[in] pCstParam    Color space transformation enable

    @return ER      error code\n
    -@b E_OK:       setting done\n
    -@b Others:     setting error\n
*/
extern ER ime_chgColorSpaceTransformEnable(IME_CST_INFO *pSetInfo);
//------------------------------------------------------------------------------

/**
    Change Dark and Bright Region Chroma Suppression Enable

    @param[in] SetEn         function enable/disable

    @return ER      error code\n
    -@b E_OK:       setting done\n
    -@b Others:     setting error\n
*/
extern ER ime_chgDrkBrtChromaSuppressEnable(IME_FUNC_EN SetEn);
//------------------------------------------------------------------------------


/**
    Change Dark and Bright Region Chroma Suppression Parameters

    @param[in] pSetInfo      dark and rright region chroma suppression parameters

    @return ER      error code\n
    -@b E_OK:       setting done\n
    -@b Others:     setting error\n
*/
extern ER ime_chgDrkBrtChromaSuppressParam(IME_DBCS_IQ_INFO *pSetInfo);
//------------------------------------------------------------------------------

/**
    Change Single Image Super-resolution Enable

    @param[in] SetEn         function enable/disable

    @return ER      error code\n
    -@b E_OK:       setting done\n
    -@b Others:     setting error\n
*/
extern ER ime_chgSingleSuperResEnable(IME_FUNC_EN SetEn);
//------------------------------------------------------------------------------

/**
    Change Single Image Super-resolution Parameters

    @param[in] SetEn         function enable/disable

    @return ER      error code\n
    -@b E_OK:       setting done\n
    -@b Others:     setting error\n
*/
extern ER ime_chgSingleSuperResParam(IME_SSR_IQ_INFO *pSetInfo);
//------------------------------------------------------------------------------


/**
    Change Film Grain Noise Enable

    @param[in] SetEn         function enable/disable

    @return ER      error code\n
    -@b E_OK:       setting done\n
    -@b Others:     setting error\n
*/
extern ER ime_chgFilmGrainNoiseEnable(IME_FUNC_EN SetEn);
//------------------------------------------------------------------------------


/**
    Change Film Grain Noise Parameters

    Change Film Grain Noise Parameters

    @param pFgnParam Film grain noise parameters, please refer to data sturcture "IME_FILM_GRAIN_INFO"

    @return ER      error code\n
    -@b E_OK:       setting done\n
    -@b Others:     setting error\n
*/
extern ER ime_chgFilmGrainNoiseParam(IME_FILM_GRAIN_IQ_INFO *pSetInfo);
//------------------------------------------------------------------------------

/**
    Change Progressive to interlace Enable

    @param[in] pSetEn         function enable/disable

    @return ER      error code\n
    -@b E_OK:       setting done\n
    -@b Others:     setting error\n
*/
extern ER ime_chgProgressiveToInterlaceEnable(IME_FUNC_EN SetEn);
//------------------------------------------------------------------------------

/**
    Change Data Stamp Color Space Transformation Enable

    @param[in] SetEn    function enable

    @return ER      error code\n
    -@b E_OK:       setting done\n
    -@b Others:     setting error\n
*/
extern ER ime_chgDataStampCSTEnable(IME_FUNC_EN SetEn);
//------------------------------------------------------------------------------


/**
    Change Data Stamp Color key Enable

    Available when RGB565 data format

    @param[in] SetNum    which Set selection
    @param[in] SetEn     function enable

    @return ER      error code\n
    -@b E_OK:       setting done\n
    -@b Others:     setting error\n
*/
extern ER ime_chgDataStampColorKeyEnable(IME_DS_SETNUM SetNum, IME_FUNC_EN SetEn);
//------------------------------------------------------------------------------



/**
    Change Data Stamp Enable

    @param[in] SetNum        which Set selection
    @param[in] SetEn         function enable/disable

    @return ER      error code\n
    -@b E_OK:       setting done\n
    -@b Others:     setting error\n
*/
extern ER ime_chgDataStampEnable(IME_DS_SETNUM SetNum, IME_FUNC_EN SetEn);
//------------------------------------------------------------------------------

/**
    Change Data Stamp Image Parameters

    @param[in] SetNum        which Set selection
    @param[in] pSetInfo     image information

    @return ER      error code\n
    -@b E_OK:       setting done\n
    -@b Others:     setting error\n
*/
extern ER ime_chgDataStampImageParam(IME_DS_SETNUM SetNum, IME_STAMP_IMAGE_INFO *pSetInfo);
//------------------------------------------------------------------------------


/**
    Change Data Stamp Effect Parameters

    @param[in] SetNum        which Set selection
    @param[in] pSetInfo     effect information

    @return ER      error code\n
    -@b E_OK:       setting done\n
    -@b Others:     setting error\n
*/
extern ER ime_chgDataStampParam(IME_DS_SETNUM SetNum, IME_STAMP_IQ_INFO *pSetInfo);
//------------------------------------------------------------------------------

/**
    Change Data Stamp Color LUT Parameters

    @param[in] pSetInfo     YUV color information for data stamp

    @return ER      error code\n
    -@b E_OK:       setting done\n
    -@b Others:     setting error\n
*/
extern ER ime_chgDataStampColorCoefsParam(IME_STAMP_CST_INFO *pSetInfo);
//------------------------------------------------------------------------------

/**
    Get Data Stamp Image Size

    @param[in] SetNum       which Set selection\n
    @param[in] pGetSizeInfo     get image size\n

    @return VOID\n
*/
extern VOID ime_getDataStampImageSizeInfo(IME_DS_SETNUM SetNum, IME_SIZE_INFO *pGetSizeInfo);
//------------------------------------------------------------------------------

/**
    Get Data Stamp Lineoffset

    @param[in] SetNum       which Set selection\n
    @param[in] pGetLofsInfo     get image lineoffset\n

    @return VOID\n
*/
extern VOID ime_getDataStampLineoffsetInfo(IME_DS_SETNUM SetNum, IME_LINEOFFSET_INFO *pGetLofsInfo);
//------------------------------------------------------------------------------

/**
    Get Data Stamp DMA Address

    @param[in] SetNum       which Set selection\n
    @param[in] pGetAddrInfo     get DMA address\n

    @return VOID\n
*/
extern VOID ime_getDataStampDmaAddrInfo(IME_DS_SETNUM SetNum, IME_DMA_ADDR_INFO *pGetAddrInfo);
//------------------------------------------------------------------------------

/**
    Change Privacy Mask enable

    @param[in] SetNum   which Set selection\n
    @param[in] SetEn    function enable\n

    @return ER      error code\n
    -@b E_OK:       setting done\n
    -@b Others:     setting error\n
*/
extern ER ime_chgPrivacyMaskEnable(IME_PM_SET_SEL SetNum, IME_FUNC_EN SetEn);
//------------------------------------------------------------------------------

/**
    Change Privacy Mask parameters

    @param[in] SetNum       which Set selection\n
    @param[in] pSetParam    parameter setting\n

    @return ER      error code\n
    -@b E_OK:       setting done\n
    -@b Others:     setting error\n
*/
extern ER ime_chgPrivacyMaskParam(IME_PM_SET_SEL SetNum, IME_PM_PARAM *pSetParam);
//------------------------------------------------------------------------------

/**
    Change Privacy Mask Pixelation Image Size

    @param[in] pSetSize    image size\n

    @return ER      error code\n
    -@b E_OK:       setting done\n
    -@b Others:     setting error\n
*/
extern ER ime_chgPrivacyMaskPixelationImageSize(IME_SIZE_INFO *pSetSize);
//------------------------------------------------------------------------------


/**
    Change Privacy Mask Pixelation Block Size

    @param[in] pSetSize    block size\n

    @return ER      error code\n
    -@b E_OK:       setting done\n
    -@b Others:     setting error\n
*/
extern ER ime_chgPrivacyMaskPixelationBlockSize(IME_PM_PXL_BLK_SIZE *pSetSize);
//------------------------------------------------------------------------------

/**
    Change Privacy Mask Pixelation Image Lineoffset

    @param[in] pSetLofs    image lineoffset\n

    @return ER      error code\n
    -@b E_OK:       setting done\n
    -@b Others:     setting error\n
*/
ER ime_chgPrivacyMaaskPixelationImageLineoffset(UINT32 *pSetLofs);
//------------------------------------------------------------------------------

/**
    Change Privacy Mask Pixelation Image DMA Address

    @param[in] pSetAddr    image DMA address\n

    @return ER      error code\n
    -@b E_OK:       setting done\n
    -@b Others:     setting error\n
*/
ER ime_chgPrivacyMaaskPixelationImageDmaAddr(UINT32 *pSetAddr);
//------------------------------------------------------------------------------

/**
    Get Privacy Mask Pixelation Image Size

    @param[in] pGetSize    image size\n

    @return VOID
*/
extern VOID ime_getPrivacyMaskPixelationImageSizeInfo(IME_SIZE_INFO *pGetSize);
//------------------------------------------------------------------------------

/**
    Get Privacy Mask Pixelation Image Lineoffset

    @param[in] pGetLofs    image lineoffset\n

    @return VOID
*/
extern VOID ime_getPrivacyMaaskPixelationImageLineoffsetInfo(UINT32 *pGetLofs);
//------------------------------------------------------------------------------

/**
    Get Privacy Mask Pixelation Image DMA Address

    @param[in] pGetAddr    image DMA address\n

    @return VOID
*/
extern VOID ime_getPrivacyMaaskPixelationImageDmaAddrInfo(UINT32 *pGetAddr);
//------------------------------------------------------------------------------


/**
    Get Engine Inforomation

    @param[in] GetImeInfoSel desired information selection

    @return UINT32      desired information\n
*/
extern UINT32 ime_getEngineInfo(IME_GET_INFO_SEL GetImeInfoSel);
//------------------------------------------------------------------------------

/**
    Get input path image size

    @param[in] pGetSize get input path image size

    @return VOID\n
*/
extern VOID ime_getInputPathImageSizeInfo(IME_SIZE_INFO *pGetSize);
//------------------------------------------------------------------------------

/**
    Get input path lineoffset

    @param[in] pGetLofs get input path lineoffset

    @return VOID\n
*/
extern VOID ime_getInputPathLineoffsetInfo(IME_LINEOFFSET_INFO *pGetLofs);
//------------------------------------------------------------------------------

/**
    Get input path DMA address

    @param[in] pGetAddr get input path DMA address

    @return VOID\n
*/
extern VOID ime_getInputPathDMAAddrInfo(IME_DMA_ADDR_INFO *pGetAddr);
//---------------------------------------------------------------------------------------------

/**
    Get output path image size

    @param[in] PathSel  output path selection\n
    @param[in] pGetSize get input path image size\n

    @return VOID\n
*/
extern VOID ime_getOutputPathImageSizeInfo(IME_PATH_SEL PathSel, IME_SIZE_INFO *pGetSize);
//------------------------------------------------------------------------------

/**
    Get output path lineoffset

    @param[in] PathSel  output path selection\n
    @param[in] BufSel   output path buffer set selection\n
    @param[in] pGetSize get input path image size\n

    @return VOID\n
*/
extern VOID ime_getOutputPathLineoffsetInfo(IME_PATH_SEL PathSel, IME_PATH_OUTBUF_SEL BufSel, IME_LINEOFFSET_INFO *pGetLofs);
//------------------------------------------------------------------------------

/**
    Get output path DMA Address

    @param[in] PathSel  output path selection\n
    @param[in] BufSel   output path buffer set selection\n
    @param[in] pGetAddr get input path image size\n

    @return VOID\n
*/
extern VOID ime_getOutputPathDMAAddrInfo(IME_PATH_SEL PathSel, IME_PATH_OUTBUF_SEL BufSel, IME_DMA_ADDR_INFO *pGetAddr);
//------------------------------------------------------------------------------

/**
    Change Statistic Enable

    @param[in] SetEn        function enable/disable

    @return ER      error code\n
    -@b E_OK:       setting done\n
    -@b Others:     setting error\n
*/
extern ER ime_chgStatisticEnable(IME_FUNC_EN SetEn);
//------------------------------------------------------------------------------

/**
    Change Statistic Filter Enable

    @param[in] SetEn        function enable/disable

    @return ER      error code\n
    -@b E_OK:       setting done\n
    -@b Others:     setting error\n
*/
extern ER ime_chgStatisticFilterEnable(IME_FUNC_EN SetEn);
//------------------------------------------------------------------------------

/**
    Change Output Image Type

    @param[in] ImgOutSel    image output type, output before or after filter image

    @return ER      error code\n
    -@b E_OK:       setting done\n
    -@b Others:     setting error\n
*/
extern ER ime_chgStatisticalImageOutputType(IME_STL_IMGOUT_SEL ImgOutSel);
//------------------------------------------------------------------------------


/**
    Change Statistic Edge Kernel Parameters

    @param[in] pSetInfo     effect information

    @return ER      error code\n
    -@b E_OK:       setting done\n
    -@b Others:     setting error\n
*/
extern ER ime_chgStatisticEdgeKerParam(IME_STL_EDGE_INFO *pSetInfo);
//------------------------------------------------------------------------------

/**
    Change Statistic Histogram Parameters

    @param[in] pSetInfo     effect information

    @return ER      error code\n
    -@b E_OK:       setting done\n
    -@b Others:     setting error\n
*/
extern ER ime_chgStatisticHistogramParam(IME_STL_HIST_INFO *pSetInfo);
//------------------------------------------------------------------------------

/**
    Change Statistic Edge Map Lineoffset

    @param[in] uiLofs     Lineoffset

    @return ER      error code\n
    -@b E_OK:       setting done\n
    -@b Others:     setting error\n
*/
extern ER ime_chgStatisticEdgeMapLineoffset(UINT32 uiLofs);
//------------------------------------------------------------------------------

/**
    Change Statistic Edge Map Address

    @param[in] uiAddr     buffer address

    @return ER      error code\n
    -@b E_OK:       setting done\n
    -@b Others:     setting error\n
*/
extern ER ime_chgStatisticEdgeMapAddr(UINT32 uiAddr);
//------------------------------------------------------------------------------



/**
    Change Statistic Histogram Output Buffer Address

    @param[in] uiHistAddr     buffer address

    @return ER      error code\n
    -@b E_OK:       setting done\n
    -@b Others:     setting error\n
*/
extern ER ime_chgStatisticHistogramAddr(UINT32 uiHistAddr);
//------------------------------------------------------------------------------


/**
    Change Statistic Edge Map Parameters

    @param[in] pSetInfo     effect information

    @return ER      error code\n
    -@b E_OK:       setting done\n
    -@b Others:     setting error\n
*/
extern ER ime_chgStatisticEdgeROIParam(IME_STL_ROI_INFO *pSetInfo);
//------------------------------------------------------------------------------


/**
    Get Statistic Information

    This API can get the max value with in hisgoram region and accmulation target histogram bin

    @param[in] pGetHistInfo     desired information\n

    @return VOID\n
*/
extern VOID ime_getStatisticInfo(IME_GET_STL_INFO *pGetHistInfo);
//------------------------------------------------------------------------------


/**
    Get Edge Map Output Address

    @param[in] pGetAddr     edge map output address

    @return VOID\n
*/
extern VOID ime_getStatisticEdgeMapDmaAddrInfo(IME_DMA_ADDR_INFO *pGetAddr);
//------------------------------------------------------------------------------

/**
    Get Edge Map Output lineoffset

    @param[in] pGetAddr     edge map output lineoffset

    @return VOID\n
*/
extern VOID ime_getStatisticEdgeMapLineoffsetInfo(UINT32 *pGetLofs);
//------------------------------------------------------------------------------


/**
    Get Histogram Output Address

    @param[in] pGetAddr     histogram output address

    @return VOID\n
*/
extern VOID ime_getStatisticHistogramDmaAddrInfo(IME_DMA_ADDR_INFO *pGetAddr);
//------------------------------------------------------------------------------


/**
    Change Stitching Enable

    @param[in] PathSel  select stitching function on output path\n
    @param[in] SetEn    disable / enable control\n

    @return ER      error code\n
    -@b E_OK:       setting done\n
    -@b Others:     setting error\n
*/
extern ER ime_chgStitchingEnable(IME_PATH_SEL PathSel, IME_FUNC_EN SetEn);
//------------------------------------------------------------------------------


/**
    Change Stitching Image Parameters

    @param[in] PathSel  path selection\n
    @param[in] pStitchInfo  Stitching parameters\n

    @return ER      error code\n
    -@b E_OK:       setting done\n
    -@b Others:     setting error\n
*/
extern ER ime_chgStitchingImageParam(IME_PATH_SEL PathSel, IME_STITCH_INFO *pStitchInfo);
//------------------------------------------------------------------------------


/**
    Change 3DNR Enable

    @param[in] SetEn    disable / enable control\n

    @return ER      error code\n
    -@b E_OK:       setting done\n
    -@b Others:     setting error\n
*/
extern ER ime_chg3DNREnable(IME_FUNC_EN SetEn);
//------------------------------------------------------------------------------

/**
    Change 3DNR Luma Enable

    @param[in] SetEn    disable / enable control\n

    @return ER      error code\n
    -@b E_OK:       setting done\n
    -@b Others:     setting error\n
*/
extern ER ime_chg3DNRLumaEnable(IME_FUNC_EN SetEn);
//------------------------------------------------------------------------------



/**
    Change 3DNR Luma Function Control Enable

    @param[in] pSetInfo luma function control enable\n

    @return ER      error code\n
    -@b E_OK:       setting done\n
    -@b Others:     setting error\n
*/
extern ER ime_chg3DNRLumaFuncCtrl(IME_3DNR_LUMA_FUNC_CTRL *pSetInfo);
//------------------------------------------------------------------------------


/**
    Change 3DNR Luma Parameters

    @param[in] pSetInfo 3DNR Luma Parameters\n

    @return ER      error code\n
    -@b E_OK:       setting done\n
    -@b Others:     setting error\n
*/
extern ER ime_chg3DNRLumaParam(IME_3DNR_LUMA_PARAM *pSetInfo);
//------------------------------------------------------------------------------




/**
    Change 3DNR chroma Enable

    @param[in] SetEn    disable / enable control\n

    @return ER      error code\n
    -@b E_OK:       setting done\n
    -@b Others:     setting error\n
*/
extern ER ime_chg3DNRChromaEnable(IME_FUNC_EN SetEn);
//------------------------------------------------------------------------------

/**
    Change 3DNR Y Enable

    @param[in] SetEn    disable / enable control\n

    @return ER      error code\n
    -@b E_OK:       setting done\n
    -@b Others:     setting error\n
*/
extern ER ime_chg3DNRYEnable(IME_FUNC_EN SetEn);
//------------------------------------------------------------------------------


/**
    Change 3DNR Chroma Function Control

    @param[in] pSetInfo chroma function control enable\n

    @return ER      error code\n
    -@b E_OK:       setting done\n
    -@b Others:     setting error\n
*/
extern ER ime_chg3DNRChromaFuncCtrl(IME_3DNR_CHROMA_FUNC_CTRL *pSetInfo);
//------------------------------------------------------------------------------


/**
    Change 3DNR Luma Parameters

    @param[in] pSetInfo 3DNR Luma Parameters\n

    @return ER      error code\n
    -@b E_OK:       setting done\n
    -@b Others:     setting error\n
*/
extern ER ime_chg3DNRChromaParam(IME_3DNR_CHROMA_PARAM *pSetInfo);
//------------------------------------------------------------------------------

/**
    Change 3DNR Y Parameters

    @param[in] pSetInfo 3DNR Y Parameters\n

    @return ER      error code\n
    -@b E_OK:       setting done\n
    -@b Others:     setting error\n
*/
extern ER ime_chg3DNRYParam(IME_3DNR_Y_PARAM *pSetInfo);
//------------------------------------------------------------------------------



/**
    Change 3DNR lineoffset of preivous image

    @param[in] pSetLofs    input Y/UV lineoffset\n

    @return ER      error code\n
    -@b E_OK:       setting done\n
    -@b Others:     setting error\n
*/
extern ER ime_chg3DNRPrvImageLineoffset(IME_LINEOFFSET_INFO *pSetLofs);
//------------------------------------------------------------------------------

/**
    Get 3DNR lineoffset of preivous image

    @param[in] pGetLofs    input Y/UV lineoffset\n

    @return VOID
*/
extern VOID ime_get3DNRPrvImageLineoffset(IME_LINEOFFSET_INFO *pGetLofs);
//------------------------------------------------------------------------------

/**
    Change 3DNR Buffer Address of preivous image

    @param[in] pSetAddr    input Y/UV buffer address\n

    @return ER      error code\n
    -@b E_OK:       setting done\n
    -@b Others:     setting error\n
*/
extern ER ime_chg3DNRPrvImageDmaAddr(IME_DMA_ADDR_INFO *pSetAddr);
//------------------------------------------------------------------------------

/**
    Get 3DNR Buffer Address of preivous image

    @param[in] pGetAddr    input Y/UV buffer address\n

    @return ER      error code\n
    -@b E_OK:       setting done\n
    -@b Others:     setting error\n
*/
extern VOID ime_get3DNRPrvImageDmaAddr(IME_DMA_ADDR_INFO *pGetAddr);
//------------------------------------------------------------------------------



/**
    Change 3DNR lineoffset of MO information

    @param[in] pSetLofs    input and output lineoffset\n

    @return ER      error code\n
    -@b E_OK:       setting done\n
    -@b Others:     setting error\n
*/
extern ER ime_chg3DNRMoLineoffset(UINT32 *puiInLofs, UINT32 *puiOutLofs);
//------------------------------------------------------------------------------

/**
    Get 3DNR lineoffset of MO information

    @param[in] puiGetInLofs    input lineoffset\n
    @param[in] puiGetOutLofs    output lineoffset\n

    @return     VOID
*/
extern VOID ime_get3DNRMoLineoffset(UINT32 *puiGetInLofs, UINT32 *puiGetOutLofs);
//------------------------------------------------------------------------------


/**
    Change 3DNR Input Buffer Address of MO information

    @param[in] pSetAddr    input buffer address\n

    @return ER      error code\n
    -@b E_OK:       setting done\n
    -@b Others:     setting error\n
*/
extern ER ime_chg3DNRMoInputDmaAddr(UINT32 *puiInAddr);
//------------------------------------------------------------------------------

/**
    Change 3DNR Output Buffer Address of MO information

    @param[in] pSetAddr    output buffer address\n

    @return ER      error code\n
    -@b E_OK:       setting done\n
    -@b Others:     setting error\n
*/
extern ER ime_chg3DNRMoOutputDmaAddr(UINT32 *puiOutAddr);
//------------------------------------------------------------------------------


/**
    Get 3DNR Buffer Address of MO information

    @param[in] puiGetInAddr             input  buffer address\n
    @param[in] puiGetOutAddr            output buffer address\n

    @return     VOID
*/
extern VOID ime_get3DNRMoDmaAddr(UINT32 *puiGetInAddr, UINT32 *puiGetOutAddr);
//------------------------------------------------------------------------------


/**
    Change 3DNR Input Buffer Address of MO information

    @param[in] pSetAddr    input buffer address\n

    @return ER      error code\n
    -@b E_OK:       setting done\n
    -@b Others:     setting error\n
*/
extern ER ime_chg3DNRMvInputDmaAddr(UINT32 *puiInAddr);
//------------------------------------------------------------------------------

/**
    Change 3DNR Output Buffer Address of MO information

    @param[in] pSetAddr    output buffer address\n

    @return ER      error code\n
    -@b E_OK:       setting done\n
    -@b Others:     setting error\n
*/
extern ER ime_chg3DNRMvOutputDmaAddr(UINT32 *puiOutAddr);
//------------------------------------------------------------------------------


/**
    Change 3DNR Buffer Address of MV information

    @param[in] puiGetInAddr     input buffer address\n
    @param[in] puiGetOutAddr    output buffer address\n

    @return ER      error code\n
    -@b E_OK:       setting done\n
    -@b Others:     setting error\n
*/
extern VOID ime_get3DNRMvDmaAddr(UINT32 *puiGetInAddr, UINT32 *puiGetOutAddr);
//------------------------------------------------------------------------------


/**
    Get 3DNR lineoffset of MV information

    @param[in] pOutputP1ImgSize    output path1 image size\n

    @return ER      error code\n
    -@b E_OK:       setting done\n
    -@b Others:     setting error\n
*/
extern UINT32 ime_get3DNRMvLineoffset(IME_SIZE_INFO *pOutputP1ImgSize);
//------------------------------------------------------------------------------

/**
    Change encoder Enable

    @param[in] SetEn    disable / enable control\n

    @return ER      error code\n
    -@b E_OK:       setting done\n
    -@b Others:     setting error\n
*/
extern ER ime_chgEncodeEnable(IME_FUNC_EN SetEn);
//------------------------------------------------------------------------------

/**
    Change side-information format

    @param[in] pSetSInfoFmt    side-information format\n

    @return ER      error code\n
    -@b E_OK:       setting done\n
    -@b Others:     setting error\n
*/
extern ER ime_chgEncodeSideInfoFmt(IME_SINFO_FMT *pSetSInfoFmt);
//------------------------------------------------------------------------------


/**
    Get encoder side-information DMA address

    @param[in] puiAddr    side-information DMA address\n

    @return ER      error code\n
    -@b E_OK:       setting done\n
    -@b Others:     setting error\n
*/
extern ER ime_chgEncodeSideInfoAddr(UINT32 *puiSetAddr);
//------------------------------------------------------------------------------

/**
    Change encoder K-table information

    @param[in] pSetKtInfo   K-table information\n

    @return ER      error code\n
    -@b E_OK:       setting done\n
    -@b Others:     setting error\n
*/
extern ER ime_chgEncodeKTable(IME_CODEC_KT_INFO *pSetKtInfo);
//------------------------------------------------------------------------------

/**
    Get encoder K-table information

    @param[in] pGetKtInfo   K-table information\n

    @return ER      error code\n
    -@b E_OK:       setting done\n
    -@b Others:     setting error\n
*/
VOID ime_getEncodeKTable(IME_CODEC_KT_INFO *pGetKtInfo);
//------------------------------------------------------------------------------



/**
    Change encoder K-Table Information

    @param[in] pSetKtInfo    K-Table Information\n

    @return ER      error code\n
    -@b E_OK:       setting done\n
    -@b Others:     setting error\n
*/
extern ER ime_chgEncodeKTable(IME_CODEC_KT_INFO *pSetKtInfo);
//------------------------------------------------------------------------------



/**
    Change encoder side-information DMA address

    @param[in] puiAddr    side-information DMA address\n

    @return ER      error code\n
    -@b E_OK:       setting done\n
    -@b Others:     setting error\n
*/
extern VOID ime_getEncodeSideInfoAddr(UINT32 *puiGetAddr);
//------------------------------------------------------------------------------


/**
    Change 3DNR decode Enable

    @param[in] SetEn    disable / enable control\n

    @return ER      error code\n
    -@b E_OK:       setting done\n
    -@b Others:     setting error\n
*/
extern ER ime_chg3DNRDecodeEnable(IME_FUNC_EN SetEn);
//------------------------------------------------------------------------------


/**
    Change 3DNR decode K-table

    @param[in] pSetKtInfo    K-Table Information\n

    @return ER      error code\n
    -@b E_OK:       setting done\n
    -@b Others:     setting error\n
*/
extern ER ime_chg3DNRDecodeKTable(IME_CODEC_KT_INFO *pSetKtInfo);
//------------------------------------------------------------------------------

/**
    Get 3DNR decode K-table

    @param[in] pGetKtInfo    K-Table Information\n

    @return VOID
*/
extern VOID ime_get3DNRDecodeKTable(IME_CODEC_KT_INFO *pGetKtInfo);
//------------------------------------------------------------------------------

/**
    Change 3DNR decode side-information address

    @param[in] uiSetAddr    side-information buffer address\n

    @return ER      error code\n
    -@b E_OK:       setting done\n
    -@b Others:     setting error\n
*/
extern ER ime_chg3DNRDecodeSideInfoAddr(UINT32 *uiSetAddr);
//------------------------------------------------------------------------------

/**
    Get 3DNR decode side-information address

    @param[in] uiGetAddr    side-information buffer address\n

    @return ER      error code\n
    -@b E_OK:       setting done\n
    -@b Others:     setting error\n
*/
extern VOID ime_get3DNRDecodeSideInfoAddr(UINT32 *uiGetAddr);
//------------------------------------------------------------------------------


/**
  Get IME Engine Clock Rate

  This function is used to get the in-used clock rate of IME
*/
extern UINT32 ime_getClockRate(VOID);
//------------------------------------------------------------------------------

/**
    Set input or output channel burst lenght

    @param[in] pSetInfo   burst size parameters

    @return ER      error code\n
    -@b E_OK:       setting done\n
    -@b Others:     setting error\n
*/
extern ER ime_chgBurstLength(IME_BURST_LENGTH *pSetInfo);
//------------------------------------------------------------------------------

/**
    Get available max stripe size for IPP mode

    @param[in] VOID

    @return UINT32  max stripe size\n
*/
extern UINT32 ime_getInputMaxStripeSize(VOID);
//------------------------------------------------------------------------------

/**
    Change all engine parameters except color key and LUT of data stamp function

    @param[in] pEngineInfo mode information and parameters.\n

    @return ER  error code\n
    -@b E_OK:   setting done\n
    -@b Others: setting error\n
*/
extern ER ime_chgAllParam(IME_MODE_PRAM *pEngineInfo);
//------------------------------------------------------------------------------

/**
    Get input and output burst length information

    @param[in] VOID

    @return VOID

*/
extern INT32 ime_getBurstLength(IME_GET_BSTSIZE_SEL GetBstSizeSel);
//------------------------------------------------------------------------------

/**
    Change IME operation mode

    @param[in] SetOpMode mode selection.\n

    @return ER      error code\n
    -@b E_OK:       setting done\n
    -@b Others:     setting error\n

*/
extern ER ime_chgOperationMode(IME_OPMODE SetOpMode);
//------------------------------------------------------------------------------

/**
    Change encode K table control

    @param[in] ClearEn  clear K-Table enable/disable\n
    @param[in] UpdateEn update K-Table enable/disable\n

    @return ER      error code\n
    -@b E_OK:       setting done\n
    -@b Others:     setting error\n

*/
extern ER ime_chgEncodeKTableCtrl(IME_FUNC_EN ClearEn, IME_FUNC_EN UpdateEn);
//------------------------------------------------------------------------------

/**
    Change 3DNR decode K table control

    @param[in] ClearEn  clear K-Table enable/disable\n
    @param[in] UpdateEn update K-Table enable/disable\n

    @return ER      error code\n
    -@b E_OK:       setting done\n
    -@b Others:     setting error\n

*/
extern ER ime_chg3DNRDecodeKTableCtrl(IME_FUNC_EN ClearEn, IME_FUNC_EN UpdateEn);
//------------------------------------------------------------------------------




/**
    get output path parameter information

    @param[in] PathSel  path selection\n
    @param[in] pGetInfo get parameters\n
*/
extern VOID ime_getOutputPathInfo(IME_PATH_SEL PathSel, IME_GET_OUTPATH_INFO *pGetInfo);
//------------------------------------------------------------------------------



/**
    get input path parameter information

    @param[in] pGetInfo get parameters\n
*/
extern VOID ime_getInputPathInfo(IME_GET_INPATH_INFO *pGetInfo);
//------------------------------------------------------------------------------


//@}


#ifdef __cplusplus
} //extern "C" {
#endif


#endif



