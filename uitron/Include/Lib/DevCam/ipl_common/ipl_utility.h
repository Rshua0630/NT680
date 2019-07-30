/**
    Public header file for IPL utility

    This file is the header file that define the API and data type for IPL utility tool.

    @file       ipl_utility.h
    @ingroup    mILibIPLCom
    @note       Nothing (or anything need to be mentioned).

    Copyright   Novatek Microelectronics Corp. 2009.  All rights reserved.
*/
#ifndef _IPL_UTILITY_H_
#define _IPL_UTILITY_H_

#include "Type.h"
#include "ErrorNo.h"

/** \addtogroup mIIPL */
//@{

/**
    ipl flow CB function
*/
typedef void(*fpCallBack)(void);

/**
    IPL flow message(channel id)
*/
//@{
typedef enum _IPL_FLOW_PROC_CH_MSG {
	IPL_FLOW_Y = 0x00000001,                ///< process Y channel
	IPL_FLOW_U = 0x00000002,                ///< process U channel
	IPL_FLOW_V = 0x00000004,                ///< process V channel
	ENUM_DUMMY4WORD(IPL_FLOW_PROC_CH_MSG)
} IPL_FLOW_PROC_CH_MSG;

/**
    IPL flow message(flow id)
*/
typedef enum _IPL_FLOW_PROC_MSG {
	IPL_FLOW_RESERVE        = 0x000000ff,  ///< reserve LSB 8 bit for IPL_FLOW_PROC_CH_MSG or other information

	//0x100 ~ 0x1f00 for cpature d2d flow
	IPL_FLOW_RAW2YCC_LCA    = 0x00000100,  ///< process LCA image x 1: raw 2 ycc + process channel(IPL_FLOW_Y|IPL_FLOW_U|IPL_FLOW_V)
	IPL_FLOW_IFE2D2D_LCA    = 0x00000200,  ///< process LCA image x 1: D2D + process channel(IPL_FLOW_Y|IPL_FLOW_U|IPL_FLOW_V)
	IPL_FLOW_RAW2YCC_PRI    = 0x00000300,  ///< process PRI image x 3: raw 2 ycc image + process channel(IPL_FLOW_Y|IPL_FLOW_U|IPL_FLOW_V)
	IPL_FLOW_RAW2YCC_DRE    = 0x00000400,  ///< process DRE image x 2: raw 2 ycc image + process channel(IPL_FLOW_Y|IPL_FLOW_U|IPL_FLOW_V)

	IPL_FLOW_DRED2D_P1      = 0x00000500,  ///< process DRE(p1) image x 1: D2D + process channel(IPL_FLOW_Y|IPL_FLOW_U|IPL_FLOW_V)
	IPL_FLOW_DRED2D_P2      = 0x00000600,  ///< process DRE(p2) image x 1: D2D + process channel(IPL_FLOW_Y|IPL_FLOW_U|IPL_FLOW_V)

	IPL_FLOW_IMED2D_SCAL    = 0x00000700,  ///< process ime d2d scale(pure) + process channel(IPL_FLOW_Y|IPL_FLOW_U|IPL_FLOW_V)

	IPL_FLOW_RAWDEC         = 0x00000800,  ///< process PRI: raw dec
	IPL_FLOW_CAL_STRIPE     = 0x00000900,  ///< pre-calculate stripe (special case)
	IPL_FLOW_RAW2YCC_PHDR   = 0x00000A00,  ///< process LCA image + PRI: get pseudo hdr parameters
	IPL_FLOW_RAW2YCC_HDR    = 0x00000B00,  ///< process LCA image + PRI: get sensor hdr parameters
	IPL_FLOW_RAW2YCC_DEFOG  = 0x00000C00,  ///< process LCA image + PRI: get sensor defog parameters
	//0x2000 ~ 0x1f000 for get raw flow
	IPL_FLOW_NOR_GETRAW             = 0x00002000,  ///< process Get raw
	IPL_FLOW_HDR_GETRAW             = 0x00003000,  ///< process Get raw
	IPL_FLOW_VID_GETRAW             = 0x00004000,  ///< process Get raw
	IPL_FLOW_HS_GETRAW              = 0x00005000,  ///< process Get raw
	IPL_FLOW_3D_GETRAW              = 0x00006000,  ///< process Get raw
	IPL_FLOW_EB_GETRAW              = 0x00007000,  ///< process Get raw
	IPL_FLOW_AEB_GETRAW             = 0x00008000,  ///< process Get raw
	IPL_FLOW_AWB_GETRAW             = 0x00009000,  ///< process Get raw
	IPL_FLOW_SIM_GETRAW             = 0x0000A000,  ///< process Get raw
	IPL_FLOW_MFHDR_GETRAW           = 0x0000B000,  ///< process Get raw
	IPL_FLOW_MFANTISHAKE_GETRAW     = 0x0000C000,  ///< process Get raw
	IPL_FLOW_DUALSBS_GETRAW         = 0x0000D000,  ///< process Get raw
	IPL_FLOW_RHDR_GETRAW            = 0x0000E000,  ///< process Get raw
	//0x20000 ~ 0x1f0000 for post process flow
	//IPL_FLOW_HDR_IFE2D2D    = 0x00020000,  ///< post process WDR IFE2 d2d + process channel(IPL_FLOW_Y|IPL_FLOW_U|IPL_FLOW_V)
	//IPL_FLOW_HDR_IMED2D     = 0x00030000,  ///< post process WDR ime d2d

	ENUM_DUMMY4WORD(IPL_FLOW_PROC_MSG)
} IPL_FLOW_PROC_MSG;
//@}


#define IPL_FLOW_CAPD2D_MIN (0x00000100)
#define IPL_FLOW_CAPD2D_MAX (0x00001f00)

#define IPL_FLOW_GETRAW_MIN (0x00002000)
#define IPL_FLOW_GETRAW_MAX (0x0001f000)

#define IPL_FLOW_PPROC_MIN (0x000020000)
#define IPL_FLOW_PPROC_MAX (0x0001f0000)


/**
    IPL process ID
*/
typedef enum _IPL_PROC_ID {
	IPL_ID_1 = 0,                       ///< process id 0
	IPL_ID_2,                           ///< process id 1
	IPL_ID_3,                           ///< process id 2
	IPL_ID_4,                           ///< process id 3
	IPL_ID_5,                           ///< process id 5
	IPL_ID_6,                           ///< process id 6
	IPL_ID_7,                           ///< process id 7
	IPL_ID_8,                           ///< process id 8
	IPL_ID_MAX_NUM,
	ENUM_DUMMY4WORD(IPL_PROC_ID)
} IPL_PROC_ID;

#define IPL_MAX_SEN_NUM IPL_ID_MAX_NUM  ///< ipl max process number


typedef enum _IPL_PROC_BIT_ID {
	IPL_BIT_ID_1 = (1 << IPL_ID_1),         ///< process id 0
	IPL_BIT_ID_2 = (1 << IPL_ID_2),         ///< process id 1
	IPL_BIT_ID_3 = (1 << IPL_ID_3),         ///< process id 2
	IPL_BIT_ID_4 = (1 << IPL_ID_4),         ///< process id 3
	IPL_BIT_ID_5 = (1 << IPL_ID_5),         ///< process id 5
	IPL_BIT_ID_6 = (1 << IPL_ID_6),         ///< process id 6
	IPL_BIT_ID_7 = (1 << IPL_ID_7),         ///< process id 7
	IPL_BIT_ID_8 = (1 << IPL_ID_8),         ///< process id 8
	IPL_BIT_ID_NONE = 0,
	ENUM_DUMMY4WORD(IPL_PROC_BIT_ID)
} IPL_PROC_BIT_ID;

typedef enum _IPL_SIE_CH_ID {
	IPL_SIE_CH_0 = 0,
	IPL_SIE_CH_1 = 1,
	IPL_SIE_CH_2 = 2,
	IPL_SIE_CH_3 = 3,
	IPL_SIE_CH_4 = 4,
	IPL_SIE_CH_5 = 5,
	IPL_SIE_CH_MAX = 6,
	ENUM_DUMMY4WORD(IPL_SIE_CH_ID)
} IPL_SIE_CH_ID;

typedef enum _IPL_IME_PATH_ID {
	IPL_IME_PATH_ID_1 = 0,
	IPL_IME_PATH_ID_2 = 1,
	IPL_IME_PATH_ID_3 = 2,
	IPL_IME_PATH_ID_4 = 3,
	IPL_IME_PATH_ID_5 = 4,
	IPL_IME_PATH_ID_MAX = 5,
	ENUM_DUMMY4WORD(IPL_IME_PATH_ID)
} IPL_IME_PATH_ID;

#define IPL_MAX_SEN_NUM IPL_ID_MAX_NUM  ///< ipl max process number


/**
    YUV channel index
*/
//@{
typedef enum {
	IPL_IMG_YUV_Y = 0,                  ///< process channel Y
	IPL_IMG_YUV_U,                      ///< process channel U
	IPL_IMG_YUV_V,                      ///< process channel V
	ENUM_DUMMY4WORD(IPL_IMG_YUV_CH)
} IPL_IMG_YUV_CH;
#define IPL_IMG_YUV_UV IPL_IMG_YUV_U    ///< process channel UV for packed format
//@}

/**
    Image buffer address
    for IPL_UTI_SET_CONT_BUFINFO function (buf[0]: image start address, buf[1]: ignore, buf[2]: ignore)
    for IPL_UTI_SET_Y_UVPACK_SEC_BUFINFO function (buf[0]: Y address, buf[1]: UV address, buf[2]: ignore)
    for IPL_UTI_SET_YUVP_SEC_BUFINFO function (buf[0]: Y address, buf[1]: U address, buf[2]: V address)
*/
typedef struct _IPL_IMG_BUF_INFO {
	UINT32 Buf[3];      ///< buffer address
} IPL_IMG_BUF_INFO;

/**
    Image coordinates
    origin of coordinates is left-top point.
*/
typedef struct _Coordinate {
	UINT32  x;              ///< x point
	UINT32  y;              ///< y point
} Coordinate;

/**
    IPL YCC format
*/
typedef enum _IPL_YUV_IMG_TYPE {
	/* 3 channel */
	IPL_IMG_YUV444 = 0,                 ///< 444 planar
	IPL_IMG_YUV422,                     ///< 422 planar
	IPL_IMG_YUV420,                     ///< 420 planar

	/* 2 channel */
	IPL_IMG_Y_PACK_UV444 = 100,         ///< 444 format y planar UV pack UVUVUVUVUV.....
	IPL_IMG_Y_PACK_UV422,               ///< 422 format y planar UV pack UVUVUVUVUV.....
	IPL_IMG_Y_PACK_UV420,               ///< 420 format y planar UV pack UVUVUVUVUV.....

	/* 1 channel */
	IPL_IMG_PACK_YUV444 = 200,          ///< 444 format YUV pack YUVYUVYUVYUVYUV.....

	/* extend */
	IPL_IMG_Y            = 300,         ///< Only Y information
	//IPL_IMG_RGB,                      ///< R G B format
	//IPL_IMG_BAYER,                    ///< R Gr Gb B format
	ENUM_DUMMY4WORD(IPL_YUV_IMG_TYPE)
} IPL_YUV_IMG_TYPE;

/**
    channel information
*/
typedef struct _IPL_YCC_CH_INFO {
	UINT32 width;       ///< image width(pixel)
	UINT32 height;      ///< image height(pixel)
	UINT32 line_ofs;     ///< image line offset(byte)
} IPL_YCC_CH_INFO;

/**
    IPL Path1 encode select
*/
typedef enum _IPL_IME_P1_ENC_FMT {
	IME_P1_ENC_HEVC = 0,
	IME_P1_ENC_H264,
	IME_P1_ENC_NONE,
	ENUM_DUMMY4WORD(IPL_IME_P1_ENC_FMT)
} IPL_IME_P1_ENC_FMT;

typedef struct _IPL_YCC_CROP_INFO {
	Coordinate Start;   ///< crop start
	UINT32 Width;       ///< crop size H
	UINT32 Height;      ///< crop size V
} IPL_YCC_CROP_INFO;

/**
    IPL YCC Address Information
*/
typedef struct _IPL_YCC_ADDR_INFO {
	UINT32 buf_handle;       ///< buffer handle, for memory control using
	UINT32 addr_start;       ///< start address, need shift 128 byte(system debug data)
	UINT32 addr_side_info;   ///< side_info address
	UINT32 pixel_addr[3];    ///< pixel YUV address
	UINT32 pixel_yuv_header; ///< yuv header address
	IPL_YUV_IMG_TYPE type;   ///< image type
	IPL_YCC_CH_INFO size;    ///< image size information
	IPL_IME_P1_ENC_FMT p1_enc_fmt; ///< ime path1 encode format
} IPL_YCC_ADDR_INFO;

/**
    IPL YCC Image Information
*/
typedef struct _IPL_YCC_IMG_INFO {
	IPL_YUV_IMG_TYPE type;  ///< image type
	UINT32 pixel_addr[3];    ///< pixel YUV address
	IPL_YCC_CH_INFO ch[3];  ///< image channel information
} IPL_YCC_IMG_INFO;

/**
	IPL IME Scale Method
*/
typedef enum _IPL_IME_SCALER {
	IPL_IME_BICUBIC     = 0,    ///< bicubic interpolation
	IPL_IME_BILINEAR    = 1,    ///< bilinear interpolation
	IPL_IME_NEAREST     = 2,    ///< nearest interpolation
	IPL_IME_INTEGRATION = 3,    ///< integration interpolation,support only scale down
	IPL_IME_AUTO        = 4,    ///< automatical calculation
	IPL_IME_SCALER_MAX,
	ENUM_DUMMY4WORD(IPL_IME_SCALER)
} IPL_IME_SCALER;

/**
    IPL RAW Image bits
*/
typedef enum _IPL_RAW_BIT {
	IPL_RAW_BIT_8 = 8,      ///< 8 bits
	IPL_RAW_BIT_10 = 10,    ///< 10 bits
	IPL_RAW_BIT_12 = 12,    ///< 12 bits
	IPL_RAW_BIT_16 = 16,    ///< 16 bits
	IPL_RAW_BIT_MAX_CNT,
	ENUM_DUMMY4WORD(IPL_RAW_BIT)
} IPL_RAW_BIT;

/**
    IPL RAW Image start pixel
*/
typedef enum _IPL_RAW_PIX {
	// RGB start pixel
	IPL_RGGB_PIX_R = 0,      ///< start pixel R
	IPL_RGGB_PIX_GR = 1,     ///< start pixel Gr
	IPL_RGGB_PIX_GB = 2,     ///< start pixel Gb
	IPL_RGGB_PIX_B = 3,      ///< start pixel B
	IPL_RGGB_PIX_MAX_CNT = 4,

	// RGBIR data format
	IPL_RGBIR_PIX_RIR = 0,      ///< start pixel RIR
	IPL_RGBIR_PIX_RG = 1,       ///< start pixel RG
	IPL_RGBIR_PIX_MAX_CNT = 2,

	// RCCB start pixel
	IPL_RCCB_PIX_RC = 0,        ///< start pixel RC
	IPL_RCCB_PIX_CR = 1,        ///< start pixel CR
	IPL_RCCB_PIX_CB = 2,        ///< start pixel CB
	IPL_RCCB_PIX_BC = 3,        ///< start pixel BC
	IPL_RCCB_PIX_MAX_CNT = 4,

	ENUM_DUMMY4WORD(IPL_RAW_PIX)
} IPL_RAW_PIX;

/**
    IPL RAW Image Information
*/
typedef struct _IPL_RAW_IMG_INFO {
	UINT32 width;           ///< image width(pixel)
	UINT32 height;          ///< image height(pixel)
	UINT32 line_offset;      ///< image line offset(byte)
	IPL_RAW_BIT bit;        ///< bits /pixel
	IPL_RAW_PIX st_pix;      ///< image start pixel
} IPL_RAW_IMG_INFO;


typedef struct _IPL_RAW_IMG {
	UINT32 pixel_addr;           ///< image buffer address
	IPL_RAW_IMG_INFO img_info;   ///< image information
} IPL_RAW_IMG;

/**
    Image coordinates with negative range.
*/
typedef struct _iCoordinate {
	INT32  x;              ///< x point
	INT32  y;              ///< y point
} iCoordinate;
/**
    Image offset
*/
typedef struct _ImgOffset {
	INT32  x;              ///< x point
	INT32  y;              ///< y point
} ImgOffset;

/**
	IPL Virtual Flow input info
*/
typedef struct {
	IPL_RAW_IMG raw_info;
	IPL_RAW_IMG crp_info;
	iCoordinate crp_start;
} IPL_VIRTUAL_IN_RAW_INFO;

typedef IPL_YCC_IMG_INFO IPL_VIRTUAL_IN_YCC_INFO;



#define IMG_EISDIS_RATIO_UINT  1000     ///< eis/dis scale uint
#define IMG_LCA_HDR_RATIO_UINT 100  ///< Lca/Hdr scale uint


/**

     IPL_UTI_Y_INFO_CONV2

     @note convert Y information to UV pack format

     @param[in] Type        image type
     @param[in] YInfor      y channel information
     @return
         - @b IPL_YCC_CH_INFO:  UV channel information
*/
extern IPL_YCC_CH_INFO IPL_UTI_Y_INFO_CONV2(IPL_YUV_IMG_TYPE Type, IPL_YCC_CH_INFO YInfor);

/**

     IPL_UTI_SET_CONT_BUFINFO

     @note depend on buffer information to set YCC address

     @param[in] Type            image type
     @param[in] buf_info         buffer address(only reference buf[0])
     @param[in,out] YCCInfo    input: image information, output: image address
     @return
         - @b void.

*/
extern void IPL_UTI_SET_CONT_BUFINFO(IPL_YUV_IMG_TYPE Type, IPL_IMG_BUF_INFO buf_info, IPL_YCC_IMG_INFO *YCCInfo);

/**

     IPL_UTI_SET_YUVP_SEC_BUFINFO

     @note depend on buffer information to set YCC address

     @param[in] buf_info         buffer address(Y: buf[0], U: buf[1], V: buf[2])
     @param[out] YCCInfo        output: image address
     @return
         - @b void.
*/
#define IPL_UTI_SET_YUVP_SEC_BUFINFO(buf_info, YCCInfo) \
	{ \
		YCCInfo.PixelAddr[0] = buf_info.Buf[0]; \
		YCCInfo.PixelAddr[1] = buf_info.Buf[1]; \
		YCCInfo.PixelAddr[2] = buf_info.Buf[2]; \
	}

/**

     IPL_UTI_SET_Y_UVPACK_SEC_BUFINFO

     @note depend on buffer information to set YCC address

     @param[in] buf_info         buffer address(Y: buf[0], UV: buf[1])
     @param[out] YCCInfo        output: image address
     @return
         - @b void.
*/
#define IPL_UTI_SET_Y_UVPACK_SEC_BUFINFO(buf_info, YCCInfo) \
	{ \
		YCCInfo.PixelAddr[0] = buf_info.Buf[0]; \
		YCCInfo.PixelAddr[1] = buf_info.Buf[1]; \
		YCCInfo.PixelAddr[2] = buf_info.Buf[1]; \
	}

/**

     IPL_YUV_SIZE

     @note depend on Y width, height and format to calculate buffer size
     @param[in] Type    image type
     @param[in] YSizeH  image width(pixel)
     @param[in] YSizeV  image height(pixel)
     @return
         - @b UINT32    image buffer size
*/
extern UINT32 IPL_YUV_SIZE(IPL_YUV_IMG_TYPE Type, UINT32 YSizeH, UINT32 YSizeV);


/**

     IPL_YSIZEH_CONV2LOFS

     @note depend on Y width, convert to line offset
     @param[in] Type    image type
     @param[in] YSizeH  image width(pixel)
     @return
         - @b UINT32    line offset
*/
extern UINT32 IPL_YSIZEH_CONV2LOFS(IPL_YUV_IMG_TYPE Type, UINT32 YSizeH);

/**

     IPL_UVSIZEH_CONV2LOFS

     @note depend on U/V width, convert to line offset
     @param[in] Type    image type
     @param[in] UVSizeH  image width(pixel)
     @return
         - @b UINT32    line offset
*/
extern UINT32 IPL_UVSIZEH_CONV2LOFS(IPL_YUV_IMG_TYPE Type, UINT32 UVSizeH);

/**

     IPL_UTI_RAW_SIZE

     @note depend on raw with, height and bits to calculate image size
     @param[in] Bit     raw bit number(IPL_RAW_BIT)
     @param[in] SizeH   raw width(pixel)
     @param[in] SizeV   raw height(pixel)
     @return
         - @b UINT32    raw image size
*/
#define IPL_UTI_RAW_SIZE(Bit, SizeH, SizeV) ((SizeH * SizeV * Bit) / IPL_RAW_BIT_8)

/**

     IPL_UTI_RAW_BUF_SIZE

     @note depend on raw with, height, and line offset to calculate buffer size
     @param[in] SizeH   raw width(pixel)
     @param[in] LineOFS raw line offset(byte)
     @param[in] SizeV   raw height(pixel)
     @return
         - @b UINT32    raw buffer size
*/
#define IPL_UTI_RAW_BUF_SIZE(SizeH, LineOFS, SizeV) (LineOFS * SizeV)


/**

     IPL_UTI_RAWH_CONV2_LOFS

     @note depend on raw width & bit convert to line offset(4 word align)
     @param[in] SizeH   pixel
     @param[in] Bit     IPL_RAW_BIT)
     @return
         - @b UINT32    line offset(byte)
*/
#define IPL_UTI_RAWH_CONV2_LOFS(SizeH, Bit) (((((SizeH * Bit) / IPL_RAW_BIT_8) + 3) / 4) * 4)


/**

     IPL_UTI_CONV_SEN_STPIX

     @note convert sensor start pix iformation(SENSOR_STPIX) to IPL index(IPL_RAW_PIX)
     @param[in] Idx     SENSOR_STPIX
     @return
         - @b UINT32    raw start pixel(IPL_RAW_PIX)
*/
extern UINT32 IPL_UTI_CONV_SEN_STPIX(UINT32 Idx);

/**

     IPL_UTI_CONV2_ORDER

     @note convert IPL ID(IPL_PROC_ID) to trigger order
     @param[in] id                  IPL_PROC_ID
     @param[in] mf_total_frm_bit    IPLCTRL_MF_TOTAL_FRM_BIT
     @return
         - @b UINT32    trigger order
*/
extern UINT32 IPL_UTI_CONV2_ORDER(UINT32 id, UINT32 mf_total_frm_bit);

/**

     IPL_UTI_CONV2_SEN_ID

     @note convert IPL ID(IPL_PROC_ID) to Sensor ID(SENSOR_ID)
     @param[in] Idx     IPL_PROC_ID
     @return
         - @b UINT32    sensor id(SENSOR_ID)
*/
extern UINT32 IPL_UTI_CONV2_SEN_ID(UINT32 Idx);

/**

     IPL_UTI_CONV2_SEN_ACT_MODE

     @note convert SENSOR_SIGNAL_MODE to SIE_ACT_MODE
     @param[in] Idx     SENSOR_SIGNAL_MODE
     @return
         - @b UINT32    SIE_ACT_MODE (SIE_ACT_MODE)
*/
extern UINT32 IPL_UTI_CONV2_SEN_ACT_MODE(UINT32 Idx);

/**
     IPL_UTI_CONV2_AE_ID

     @note convert IPL ID(IPL_PROC_ID) to AE ID(AE_PROC_ID)
     @param[in] Idx     IPL_PROC_ID
     @return
         - @b UINT32    ae process id(AE_PROC_ID)
*/
extern UINT32 IPL_UTI_CONV2_AE_ID(UINT32 Idx);

/**
     IPL_UTI_CONV2_AWB_ID

     @note convert IPL ID(IPL_PROC_ID) to AWB ID(AWB_PROC_ID)
     @param[in] Idx     IPL_PROC_ID
     @return
         - @b UINT32    awb process id(AWB_PROC_ID)
*/
extern UINT32 IPL_UTI_CONV2_AWB_ID(UINT32 Idx);

/**
     IPL_UTI_CONV2_AF_ID

     @note convert IPL ID(IPL_PROC_ID) to AF ID(AF_PROC_ID)
     @param[in] Idx     IPL_PROC_ID
     @return
         - @b UINT32    af process id(AF_PROC_ID)
*/
extern UINT32 IPL_UTI_CONV2_AF_ID(UINT32 Idx);

/**
     IPL_UTI_CONV2_IQS_ID

     @note convert IPL ID(IPL_PROC_ID) to IQS ID(IQS_PROC_ID)
     @param[in] Idx     IPL_PROC_ID
     @return
         - @b UINT32    iqs process id(IQS_PROC_ID)
*/
extern UINT32 IPL_UTI_CONV2_IQS_ID(UINT32 Idx);

/**
     IPL_UTI_CONV2_WDR_ID

     @note convert IPL ID(IPL_PROC_ID) to WDR ID(SR_WDR_PROCID)
     @param[in] Idx     IPL_PROC_ID
     @return
         - @b UINT32    wdr process id(SR_WDR_PROCID)
*/
extern UINT32 IPL_UTI_CONV2_WDR_ID(UINT32 Idx);
/**
     IPL_UTI_SENCROPRATIO_ADJ_VSIZE

     @note adjust image v size by image ratio
     @param[in] ImgRatioIdx     SEL_IMAGERATIO
     @param[in] DefRatioIdx     SEL_IMAGERATIO
     @param[in] InSize          pixel
     @param[in] Align           align value
     @return
         - @b UINT32    OutSize(pixel)
*/
extern UINT32 IPL_UTI_SENCROPRATIO_ADJ_VSIZE(UINT32 ImgRatioIdx, UINT32 DefRatioIdx, UINT32 InSize, UINT32 Align);

/**
     IPL_UTI_SENCROPRATIO_ADJ_HSIZE

     @note adjust image h size by image ratio
     @param[in] ImgRatioIdx     SEL_IMAGERATIO
     @param[in] DefRatioIdx     SEL_IMAGERATIO
     @param[in] InSize          pixel
     @param[in] Align           align value
     @return
         - @b UINT32    OutSize(pixel)
*/
extern UINT32 IPL_UTI_SENCROPRATIO_ADJ_HSIZE(UINT32 ImgRatioIdx, UINT32 DefRatioIdx, UINT32 InSize, UINT32 Align);

/**
     IPL_UTI_EISDIS_ADJ_HSIZE

     @note adjust image h size by dis/eis ratio
     @param[in] InSize      pixel
     @param[in] Ratio       ratio value
     @param[in] Align       align value
     @return
         - @b UINT32    OutSize(pixel)
*/
extern UINT32 IPL_UTI_EISDIS_ADJ_HSIZE(UINT32 InSize, UINT32 Ratio, UINT32 Align);

/**
     IPL_UTI_EISDIS_ADJ_VSIZE

     @note adjust image v size by dis/eis ratio
     @param[in] InSize      pixel
     @param[in] Ratio       ratio value
     @param[in] Align       align value
     @return
         - @b UINT32    OutSize(pixel)
*/
extern UINT32 IPL_UTI_EISDIS_ADJ_VSIZE(UINT32 InSize, UINT32 Ratio, UINT32 Align);

/**
     IPL_UTI_IPESUB_ADJ_HSIZE

     @note adjust image h size by iq ratio
     @param[in] InSize      pixel
     @param[in] Ratio       ratio value
     @param[in] Align       align value
     @return
         - @b UINT32    OutSize(pixel)
*/
extern UINT32 IPL_UTI_IMESUB_ADJ_HSIZE(UINT32 InSize, UINT32 Ratio, UINT32 Align);

/**
     IPL_UTI_IPESUB_ADJ_VSIZE

     @note adjust image v size by ratio
     @param[in] InSize      pixel
     @param[in] Ratio       ratio value
     @param[in] Align       align value
     @return
         - @b UINT32    OutSize(pixel)
*/
extern UINT32 IPL_UTI_IMESUB_ADJ_VSIZE(UINT32 InSize, UINT32 Ratio, UINT32 Align);

/**
     IPL_UTI_IME_ADJ_VSIZE

     @note adjust image v size by ratio
     @param[in] InSize      pixel
     @param[in] Ratio       ratio value
     @param[in] Align       align value
     @return
         - @b UINT32    OutSize(pixel)
*/
extern UINT32 IPL_UTI_IME_ADJ_VSIZE(UINT32 InSize, UINT32 Ratio, UINT32 Align);

/**
     IPL_UTI_IME_ADJ_HSIZE

     @note adjust image v size by ratio
     @param[in] InSize      pixel
     @param[in] Ratio       ratio value
     @param[in] Align       align value
     @return
         - @b UINT32    OutSize(pixel)
*/
extern UINT32 IPL_UTI_IME_ADJ_HSIZE(UINT32 InSize, UINT32 Ratio, UINT32 Align);

/**
     IPL_UTI_YLOF2UV

     @note convert Y lineoffset to UV pack format

     @param[in] Type        image type
     @param[in] YInfor      y channel lineoffset
     @return
         - @b UINT32:  UV channel lineoffset
*/
extern UINT32 IPL_UTI_YLOF2UV(IPL_YUV_IMG_TYPE type, UINT32 y_lof);

#define IPL_UTI_SIESUB_ADJ_HSIZE IPL_UTI_IMESUB_ADJ_HSIZE
#define IPL_UTI_SIESUB_ADJ_VSIZE IPL_UTI_IMESUB_ADJ_VSIZE

#define IPL_ALIGN_ROUNDUP(x, y) ((((x) + (y) - 1) / (y)) * (y))
#define IPL_ALIGN_ROUNDDOWN(x, y) (((x) / (y)) * (y))
#define IPL_ALIGN_ROUND(x, y) ((((x) + ((y)/2)) / (y)) * (y))


#define IPL_UTI_CONV2_BIT_ID(id) (1 << id)

#define IPL_UTI_CONV2_UINT32(a, b) (((a & 0x0000ffff) << 16) | (b & 0x0000fffff))


/**
	SQUARE Patch Size Adjust Related
*/
extern UINT32 IPL_UTI_SQUARE_PATCH_ADJUST_INPUT(void* img, UINT32 proc_w, UINT32 proc_h, UINT32 pid, UINT32 ref_path);
extern UINT32 IPL_UTI_SQUARE_PATCH_ADJUST_OUTPUT(void* img, UINT32 proc_w, UINT32 proc_h, UINT32 pid);


//@}
#endif //_IPL_UTILITY_H_
