/**
    Image operation module.

    This module include the image operation like fill image,copy image, scale image, rotate image ...

    @file       GxImage.h
    @ingroup    mILibImageProcess

    Copyright   Novatek Microelectronics Corp. 2011.  All rights reserved.
*/
#ifndef _GX_IMAGE_H_
#define _GX_IMAGE_H_

#include "Type.h"
#ifdef __cplusplus
extern "C" {
#endif

/**
    @addtogroup mILibImageProcess
*/

/**
     Image buffer pixel format.
*/
typedef enum _GX_IMAGE_PIXEL_FMT_ {
	GX_IMAGE_PIXEL_FMT_YUV422_PLANAR  = 0x00,    ///< 3 color plane, pixel=Y(w,h), U(w/2,h), and V(w/2,h)
	GX_IMAGE_PIXEL_FMT_YUV420_PLANAR  = 0x01,    ///< 3 color plane, pixel=Y(w,h), U(w/2,h/2), and V(w/2,h/2)
	GX_IMAGE_PIXEL_FMT_YUV422_PACKED  = 0x02,    ///< 2 color plane, pixel=Y(w,h), UV(w,h)
	GX_IMAGE_PIXEL_FMT_YUV420_PACKED  = 0x03,    ///< 2 color plane, pixel=Y(w,h), UV(w,h/2)
	GX_IMAGE_PIXEL_FMT_Y_ONLY         = 0x04,    ///< 1 color plane, pixel=Y(w,h),
	GX_IMAGE_PIXEL_FMT_ARGB565        = 0x05,    ///< 2 color plane, pixel=A8(w,h),RGB565(2w,h)
	GX_IMAGE_PIXEL_FMT_RGB888_PLANAR  = 0x06,    ///< 3 color plane, pixel=R(w,h), G(w,h), B(w,h)
	GX_IMAGE_PIXEL_FMT_ARGB8888_PACKED = 0x07,   ///< 1 color plane, pixel=ARGB(4w,h)
	GX_IMAGE_PIXEL_FMT_ARGB1555_PACKED = 0x08,   ///< 1 color plane, pixel=ARGB(2w,h)
	GX_IMAGE_PIXEL_FMT_ARGB4444_PACKED = 0x09,   ///< 1 color plane, pixel=ARGB(2w,h)
	GX_IMAGE_PIXEL_FMT_RGB565_PACKED   = 0x0A,   ///< 1 color plane, pixel=RGB(2w,h)
	GX_IMAGE_PIXEL_FMT_RAW8            = 0x10,   ///< 1 color plane, pixel=RAW(w,h) x 8bits
	GX_IMAGE_PIXEL_FMT_RAW10           = 0x11,   ///< 1 color plane, pixel=RAW(w,h) x 10bits
	GX_IMAGE_PIXEL_FMT_RAW12           = 0x12,   ///< 1 color plane, pixel=RAW(w,h) x 12bits
	GX_IMAGE_PIXEL_FMT_RAW16           = 0x13,   ///< 1 color plane, pixel=RAW(w,h) x 16bits
	GX_IMAGE_PIXEL_FMT_YUV444_PLANAR   = 0x14,   ///< 3 color plane, pixel=Y(w,h), U(w,h), and V(w,h)
	ENUM_DUMMY4WORD(GX_IMAGE_PIXEL_FMT)
} GX_IMAGE_PIXEL_FMT;

/**
     Image buffer dump plane select.
*/
typedef enum _GX_IMAGE_DUMP_PLANE_ {
	GX_IMAGE_DUMP_Y = 0x00,                     ///< dump Y only
	GX_IMAGE_DUMP_U = 0x01,                     ///< dump U only
	GX_IMAGE_DUMP_V = 0x02,                     ///< dump V only
	GX_IMAGE_DUMP_ALL = 0x03,                   ///< dump all planes
	ENUM_DUMMY4WORD(GX_IMAGE_DUMP_PLANE)
} GX_IMAGE_DUMP_PLANE;

/**
     @name Lineoffset alignment pattern
*/
//@{
#define GXIMAGE_LINEOFFSET_PATTERN     0x80000000                          ///<   lineoffset base pattern
#define GXIMAGE_LINEOFFSET_ALIGN(n)    (GXIMAGE_LINEOFFSET_PATTERN | (n))  ///<   n bytes alignment lineoffset
//@}

/**
     @name Image buffer pixel format for 650
*/
//@{
#define GX_IMAGE_PIXEL_FMT_YUV422     GX_IMAGE_PIXEL_FMT_YUV422_PACKED   ///<   for 650 paltform , YUV 422 means YUV 422 packed
#define GX_IMAGE_PIXEL_FMT_YUV420     GX_IMAGE_PIXEL_FMT_YUV420_PACKED   ///<   for 650 paltform , YUV 420 means YUV 420 packed
//@}


/**
     @name Image buffer plane number maximum value
*/
//@{
#define MAX_PLANE_NUM                 3            ///<  3 if for YUV planner
//@}

/**
     @name Image region default value
*/
//@{
#define REGION_MATCH_IMG              NULL         ///<  use in region w,h,x,y match pImgBuf
//@}

/**
     @name ScaleMultiOut max number
*/
//@{
#define GX_IMAGE_MULTI_OUT_MAX        3            ///<  used by GxImg_ScaleMultiOut()
//@}

/**
     Image buffer structure.
*/
typedef struct _IMG_BUF {
	UINT32              flag;                      ///< used for check if this image buffer is initialized
	UINT32              Width;                     ///< image width
	UINT32              Height;                    ///< image height
	GX_IMAGE_PIXEL_FMT  PxlFmt;                    ///< pixel format
	UINT32              PxlAddr[MAX_PLANE_NUM];    ///< pixel address
	UINT32              LineOffset[MAX_PLANE_NUM]; ///< pixel lineoffset
	IPOINT              ScaleRatio;                ///< Scale ratio for virtual/Real coordinate translation
	IRECT               VirtCoord;                 ///< Virtual coordinate
	IRECT               RealCoord;                 ///< Real    coordinate
	MEM_RANGE           Metadata;                  ///< Metadata memory range
} IMG_BUF, *PIMG_BUF;

/**
     Structure of image buffer and region. Used by GxImg_ScaleMultiOut()
*/
typedef struct _GXIMG_MULTI_OUT {
	PIMG_BUF    pImg[GX_IMAGE_MULTI_OUT_MAX];
	PIRECT		pRegion[GX_IMAGE_MULTI_OUT_MAX];
} GXIMG_MULTI_OUT, *PGXIMG_MULTI_OUT;

/**
     Structure of image cover description.
*/
typedef struct {
	IPOINT pointTopLeft;            ///< point 0 (left/top)
	IPOINT pointTopRight;           ///< point 1 (right/top)
	IPOINT pointBottomRight;        ///< point 2 (right/bottom)
	IPOINT pointBottomLeft;         ///< point 3 (left/bottom)
	UINT32 uiYUVA;                  ///< Alpha[31..24], V[23..16], U[15..8], Y[7..0], Please refer to COLOR_MAKE_YUVA macro
	///< If this value is not zero, mosaic setting will be omitted
	ISIZE MosaicBlkSize;            ///< mosaic block size (only support 8*8, 16*16, 32*32, 64*64)
	PIMG_BUF pMosaicImg;            ///< mosaic image buffer
} GXIMG_COVER_DESC, *PGXIMG_COVER_DESC;

/**
     Video Display layer ID.
*/
typedef enum _GXDISP_ID {
	GXDISP_VIDEO1 = 1,      ///< video 1
	GXDISP_VIDEO2 = 2,      ///< video 2
	ENUM_DUMMY4WORD(GXDISP_ID)
} GXDISP_ID;


/**
     Rotation Engine ID.
*/
typedef enum _GX_IMAGE_RT_ENG {
	GX_IMAGE_ROTATE_ENG1 = 0,      ///< Rotation Engine 1
	GX_IMAGE_ROTATE_RESERVE,       ///< Rotation Engine reserved
	ENUM_DUMMY4WORD(GX_IMAGE_RT_ENG)
} GX_IMAGE_RT_ENG;

/**
     Rotation orientation.

     The rotation type choosed in API GxImg_RotateData() & GxImg_SelfRotate().
*/
typedef enum _GX_IMAGE_RT_ORI {
	GX_IMAGE_ROTATE_90 = 0,     ///< Rotate 90 degree clockwise
	GX_IMAGE_ROTATE_270,        ///< Rotate 270 degree clockwise
	GX_IMAGE_ROTATE_180,        ///< Rotate 180 degree
	GX_IMAGE_ROTATE_HRZ,        ///< Horizontal flip
	GX_IMAGE_ROTATE_VTC,        ///< Vertical flip
	GX_IMAGE_ROTATE_HRZ_90,     ///< Horizontal flip and Rotate 90 degree clockwise
	GX_IMAGE_ROTATE_HRZ_270,    ///< Horizontal flip and Rotate 270 degree clockwise
	ENUM_DUMMY4WORD(GX_IMAGE_RT_ORI)
} GX_IMAGE_RT_ORI;

/**
     Fill Engine ID.
*/
typedef enum _GX_IMAGE_FILL_ENG {
	GX_IMAGE_FILL_ENG1 = 0,      ///< Fill Engine 1
	GX_IMAGE_FILL_ENG2,          ///< Fill Engine 2
	ENUM_DUMMY4WORD(GX_IMAGE_FILL_ENG)
} GX_IMAGE_FILL_ENG;


/**
     Copy Engine ID.
*/
typedef enum _GX_IMAGE_CP_ENG {
	GX_IMAGE_CP_ENG1 = 0,					///< Copy Engine 1
	GX_IMAGE_CP_ENG2,						///< Copy Engine 2
	GX_IMAGE_CP_ENG_MAX,					///< Copy Engine MAX
	GX_IMAGE_CP_ENG_DFT = GX_IMAGE_CP_ENG1,	///< Copy Engine Default
	ENUM_DUMMY4WORD(GX_IMAGE_CP_ENG)
} GX_IMAGE_CP_ENG;

/**
     Scale Engine ID.
*/
typedef enum _GX_IMAGE_SC_ENG {
	GX_IMAGE_SC_ENG1 = 0,      ///< Scale Engine 1
	GX_IMAGE_SC_ENG2,          ///< Scale Engine 2
	ENUM_DUMMY4WORD(GX_IMAGE_SC_ENG)
} GX_IMAGE_SC_ENG;

/**
     Encode information.

*/
typedef struct _GXIMG_ENCODE_INFO {
	PIMG_BUF   pSrcImg;          ///< the source raw image buffer
	UINT32     outBufAddr;       ///< Out bitstream buffer starting address
	UINT32     outBufSize;       ///< Out bitstream buffer size
	void      *param[4];         ///< param[0] is Q-table addr Y, param[1] is Q-table addr UV , param[2] & param[3] are reserved.
} GXIMG_ENCODE_INFO, *PGXIMG_ENCODE_INFO;

/**
    Parameter ID.

*/
typedef enum _GXIMG_PARM_ID {
	GXIMG_PARM_SCALE_METHOD = 0,        ///< set scale method for scale function, the setting value please reference GXIMG_SCALE_METHOD.
	GXIMG_PARM_FILTER_P2I = 1,          ///< set IME scale filter for enable progressive to interlaced operation, the setting value is 1 or 0.
	ENUM_DUMMY4WORD(GXIMG_PARM_ID)
} GXIMG_PARM_ID;


/**
    Scale method.

    Used when set parameter GXIMG_PARM_SCALE_METHOD. The default scale method for scale function is GXIMG_SCALE_AUTO.

*/
typedef enum _GXIMG_SCALE_METHOD {
	GXIMG_SCALE_BICUBIC = 0,        ///< bicubic interpolation
	GXIMG_SCALE_BILINEAR,           ///< bilinear interpolation
	GXIMG_SCALE_NEAREST,            ///< nearest interpolation
	GXIMG_SCALE_INTEGRATION,        ///< integration interpolation
	GXIMG_SCALE_AUTO,               ///< auto choose scale method above, this is the default value.
	GXIMG_SCALE_MAX_ID,             ///< total scale method numbers
	ENUM_DUMMY4WORD(GXIMG_SCALE_METHOD)
} GXIMG_SCALE_METHOD;


//------------------------------------------------------------------------------
// GxImage Basic
//------------------------------------------------------------------------------
/**
     Calculate required buffer size.

     User can firstly call this API to get required buffer size before calling GxImg_InitBuf().

     @param[in] Width: The image width.
     @param[in] Height: The image height.
     @param[in] PxlFmt: The image pixel format.
     @param[in] LineOff: The image pixel lineoffset. GXIMAGE_LINEOFFSET_ALIGN(n) means the lineoffset is the input image width align n.\n
                         0 means the lineoffset is input image width align 4.\n
     @return required buffer size.

*/
extern UINT32 GxImg_CalcRequireSize(UINT32 Width, UINT32 Height, GX_IMAGE_PIXEL_FMT PxlFmt, UINT32 LineOff);


/**
     Init Image Buffer.

     When user use this API to init buffer, it is assumed that for the Y UVpacked image, the width, lineoffset of Y & UV are the same.\n
     And the Y UV data are put in sequence and continuous in the buffer address.

     @param[out] pImgBuf: The image buffer.
     @param[in] Width: The image width.
     @param[in] Height: The image Height.
     @param[in] PxlFmt: The image pixel format.
     @param[in] LineOff: The image pixel lineoffset. GXIMAGE_LINEOFFSET_ALIGN(n) means the lineoffset is the input image width align n.\n
                         0 means the lineoffset is input image width align 4.\n
     @param[in] Addr: The image raw buffer address.
     @param[in] AvailableSize: The image raw buffer available size.
     @return
         - @b E_OK:    Init Image buffer ok.
         - @b E_SYS:   Init Image buffer fail, input buffer size not enough.
         - @b E_PAR:   Init Image buffer fail, input parameter error.

     Example:
     @code
     {
         UINT32    uiPool,uiPoolSize;
         ISIZE     DeviceSize;
         IMG_BUF   DispImg;

         // Get display size
         DeviceSize = GxVideo_GetDeviceSize(DOUT1);
         // Init Display image buffer as pixel format 422
         GxImg_InitBuf(&DispImg,DeviceSize.w,DeviceSize.h,GX_IMAGE_PIXEL_FMT_YUV422, GXIMAGE_LINEOFFSET_ALIGN(16),uiPool,uiPoolSize);
     }
     @endcode
*/
extern ER GxImg_InitBuf(PIMG_BUF pImgBuf, UINT32 Width, UINT32 Height, GX_IMAGE_PIXEL_FMT PxlFmt, UINT32 LineOff, UINT32 Addr, UINT32 AvailableSize);

/**
     Init Image Buffer extension API.

     User can use this API when the Y UV data address & lineoffset is already known in other image buffer structure.
     The Y UV data address may be not in sequence or continuous.

     @param[out] pImgBuf: The image buffer.
     @param[in] Width: The image width.
     @param[in] Height: The image height.
     @param[in] PxlFmt: The image pixel format.
     @param[in] LineOff: The image pixel lineoffset of each plane.
     @param[in] PxlAddr: The image raw buffer address of each plane.
     @return
         - @b E_OK:    Init Image buffer ok.
         - @b E_PAR:   Init Image buffer fail, input parameter error.

     Example:
     @code
     {
         uiLineOffset[0] = StampBuffer.uiYLineOffset;
         uiLineOffset[1] = StampBuffer.uiUVLineOffset;
         uiPxlAddr[0]    = StampBuffer.uiYAddr;
         uiPxlAddr[1]    = StampBuffer.uiUAddr;
         GxImg_InitBufEx(&SrcImg, uiWidth, uiStampDataHeight,GX_IMAGE_PIXEL_FMT_YUV420, uiLineOffset, uiPxlAddr);
     }
     @endcode
*/
extern ER GxImg_InitBufEx(PIMG_BUF pImgBuf, UINT32 Width, UINT32 Height, GX_IMAGE_PIXEL_FMT PxlFmt, UINT32 LineOff[MAX_PLANE_NUM], UINT32 PxlAddr[MAX_PLANE_NUM]);

/**
     Get image buffer start & End address.

     User can use this API to get start & end address when the image buffer is initialized by GxImg_InitBuf(),\n.
     because the Y UV data is in sequence & continuous.
     User can't use this API to get start & end address when the image buffer is initialized by GxImg_InitBufEx(),\n
     because the Y UV data may be not in sequence or continuous.

     @param[in] pImgBuf: The image buffer.
     @param[out] StartAddr: The image buffer start address.
     @param[out] EndAddr: The image buffer end address.
     @return
         - @b TRUE:    Get buffer address ok.
         - @b FALSE:   Get buffer address fail, input parameter error.

*/
extern ER GxImg_GetBufAddr(PIMG_BUF pImgBuf, UINT32 *StartAddr, UINT32 *EndAddr);
/**
     Dump image buffer raw data to a file.

     This API will dump the image buffer raw data to a file, user can save all the color planes to one file or separate files.

     @param[in] filePath: The file path.
     @param[in] pImgBuf: The image buffer.
     @param[in] plane: The plane want to dump.
     @return
         - @b E_OK:    Dump data to file ok.
         - @b E_PAR:   input parameter error.
         - @b E_SYS:   Dump file fail because of filesystem return error.

*/
extern ER GxImg_DumpBuf(CHAR *filePath, PIMG_BUF pImgBuf, GX_IMAGE_DUMP_PLANE plane);

//------------------------------------------------------------------------------
// GxImage Process
//------------------------------------------------------------------------------


/**
     Fill image buffer with specific YCbCr color.

     @param[in] pDstImg: The image buffer.
     @param[in] pDstRegion: Fill region of image buffer. REGION_MATCH_IMG means the region matches image buffer.
     @param[in] YCbCrValue: The fill color value, format is 0x00CrCbYY.
     @return
         - @b E_OK:    Fill color success.
         - @b E_PAR:   Input parameter error.
*/
extern ER GxImg_FillData(PIMG_BUF pDstImg, IRECT *pDstRegion, UINT32 YCbCrValue);

/**
     Fill image buffer with specific YCbCr color.

     @param[in] pDstImg: The image buffer.
     @param[in] pDstRegion: Fill region of image buffer. REGION_MATCH_IMG means the region matches image buffer.
     @param[in] YCbCrValue: The fill color value, format is 0x00CrCbYY.
     @param[in] FillEng: The fill data engine.
     @return
         - @b E_OK:    Fill color success.
         - @b E_PAR:   Input parameter error.
*/
extern ER GxImg_FillDataEx(PIMG_BUF pDstImg, IRECT *pDstRegion, UINT32 YCbCrValue, GX_IMAGE_FILL_ENG FillEng);

/**
     Copy image data.

     Copy image data of specific region from source buffer to specific location of destionation buff.

     @note Naturely, the source region should not over the range of source image. So if the input soucre region is over the source image.\n
     This fuction will auto calculate clipped rectangle for copying. The region has limitation of minimum width value is 2, height value is 2.

     @param[in] pSrcImg: The source image buffer.
     @param[in] pSrcRegion: The source buffer region for copying. REGION_MATCH_IMG means the region matches source image buffer.
     @param[in] pDstImg: The destionation image buffer.
     @param[in] pDstLocation: The destionation image buffer location to copy to.
     @return
         - @b E_OK:    Copy data success.
         - @b E_PAR:   Input parameter error.
*/
extern ER GxImg_CopyData(PIMG_BUF pSrcImg, IRECT *pSrcRegion, PIMG_BUF pDstImg, IPOINT *pDstLocation);

/**
     Copy image data.

     Copy image data of specific region from source buffer to specific location of destionation buff.

     @note Naturely, the source region should not over the range of source image. So if the input soucre region is over the source image.\n
     This fuction will auto calculate clipped rectangle for copying. The region has limitation of minimum width value is 2, height value is 2.

     @param[in] pSrcImg: The source image buffer.
     @param[in] pSrcRegion: The source buffer region for copying. REGION_MATCH_IMG means the region matches source image buffer.
     @param[in] pDstImg: The destionation image buffer.
     @param[in] pDstLocation: The destionation image buffer location to copy to.
     @param[in] CpEng: The copy data engine.
     @return
         - @b E_OK:    Copy data success.
         - @b E_PAR:   Input parameter error.
*/
extern ER GxImg_CopyDataEx(PIMG_BUF pSrcImg, IRECT *pSrcRegion, PIMG_BUF pDstImg, IPOINT *pDstLocation, GX_IMAGE_CP_ENG CpEng);

/**
     Copy image data with colorkey effect.

     This API will do the color key (=) copy effect. \n
     A: is source image. B: is key image. It means if (B == colorkey) ? A:B -> destination buffer.

     @param[in] pSrcImg: The source image buffer.
     @param[in] pCopyRegion: The buffer region for copying. REGION_MATCH_IMG means the region matches source image buffer.
     @param[in] pKeyImg: The color key image buffer.
     @param[in] pKeyLocation: The key image buffer location to copy to.
     @param[in] colorkey: The color key (0x00VVUUYY).
     @param[in] isCopyToKeyImg: If the destination buffer is pKeyImg, else the destination buffer will be pSrcImg.
     @param[in] CpEng: The copy data engine.
     @return
         - @b E_OK:    Copy data success.
         - @b E_PAR:   Input parameter error.
*/
extern ER GxImg_CopyColorkeyData(PIMG_BUF pSrcImg, IRECT *pCopyRegion, PIMG_BUF pKeyImg, IPOINT *pKeyLocation, UINT32 colorkey, BOOL isCopyToKeyImg, GX_IMAGE_CP_ENG CpEng);

/**
     Copy image data with alpha blending effect.

     This API will do the alpha blending effect. \n
     A: is source image. B: is destion image. The copy blending weight is (alpha/256)*A + ((256-alpha)/256)*B.

     @param[in] pSrcImg: The source image buffer.
     @param[in] pSrcRegion: The source buffer region for copying. REGION_MATCH_IMG means the region matches source image buffer.
     @param[in] pDstImg: The destionation image buffer.
     @param[in] pDstLocation: The destionation image buffer location to copy to.
     @param[in] alpha: The alpha blending value, range is from 0 ~ 256, all the image use the same constant alpha.
     @param[in] CpEng: The copy data engine.
     @return
         - @b E_OK:    Copy data success.
         - @b E_PAR:   Input parameter error.
*/
extern ER GxImg_CopyBlendingData(PIMG_BUF pSrcImg, IRECT *pSrcRegion, PIMG_BUF pDstImg, IPOINT *pDstLocation, UINT32 alpha, GX_IMAGE_CP_ENG CpEng);

/**
     Copy image data with alpha blending effect.

     This API will do the alpha blending effect. \n
     A: is source image. B: is alpha plane. C: is destion image. The copy blending weight is (B/256)*A + ((256-B)/256)*C.

     @param[in] pSrcImg: The source image buffer.
     @param[in] pSrcRegion: The source buffer region for copying. REGION_MATCH_IMG means the region matches source image buffer.
     @param[in] pDstImg: The destionation image buffer.
     @param[in] pDstLocation: The destionation image buffer location to copy to.
     @param[in] alphaPlane: The alpha plane. The width, height and lineoffset is the same with source image.
     @return
         - @b E_OK:    Copy data success.
         - @b E_PAR:   Input parameter error.
*/
extern ER GxImg_CopyBlendingDataEx(PIMG_BUF pSrcImg, IRECT *pSrcRegion, PIMG_BUF pDstImg, IPOINT *pDstLocation, UINT8 *alphaPlane);


/**
     Set parameter of GxIamge.

     @param[in] ParmID: The parameter ID.
     @param[in] value: The parameter value.
     @return
         - @b E_OK:    Setting success.
         - @b E_PAR:   Input parameter error.
*/
extern ER GxImg_SetParm(GXIMG_PARM_ID ParmID, UINT32 value);

/**
     Scaling up/down image data to multiple destinations.

     This API will scale up/down image data, the max scale up factor is 32, the max scale down factor is 16.\n
     This API have start address word alignment limitation & width/height word alignment limitation.\n
     The scaling method supports all that defined in GXIMG_SCALE_METHOD.

     @note
     -# If the scaling up/down factor over the HW limitation, or the address alignment not meet the HW limitation,
        it will return fail.
     -# The output pixel formats should be identical

     @param[in] pSrcImg: The source image buffer.
     @param[in] pSrcRegion: The source buffer region for scaling.REGION_MATCH_IMG means the region matches source image buffer.
     @param[in] pOut: The destionation pointers of image buffers and regions to be scaled. REGION_MATCH_IMG means the region matches destionation image buffer.
     @return
         - @b E_OK:    Scale data success.
         - @b E_PAR:   Input parameter error.
*/
ER GxImg_ScaleMultiOut(PIMG_BUF pSrcImg, IRECT *pSrcRegion, GXIMG_MULTI_OUT *pOut);

/**
     Scaling up/down image data.

     This API will scale up/down image data, the max scale up/down factor is 16.\n
     This API have no start address word alignment limitation or width/height word alignment limitation.\n
     The scaling method supports all that defined in GXIMG_SCALE_METHOD.

     @note If the scaling up/down factor over the HW limitation, it will auto change to FW scaling.

     @param[in] pSrcImg: The source image buffer.
     @param[in] pSrcRegion: The source buffer region for scaling. REGION_MATCH_IMG means the region matches source image buffer.
     @param[in] pDstImg: The destionation image buffer.
     @param[in] pDstRegion: The destionation image buffer region to scaling to. REGION_MATCH_IMG means the region matches destionation image buffer.
     @return
         - @b E_OK:    Scaling data success.
         - @b E_PAR:   Input parameter error.
*/
extern ER GxImg_ScaleData(PIMG_BUF pSrcImg, IRECT *pSrcRegion, PIMG_BUF pDstImg, IRECT *pDstRegion);

/**
     Scaling up/down image data.

     This API will scale up/down image data, the max scale up/down factor is 16.\n
     This API have no start address word alignment limitation or width/height word alignment limitation.\n
     The scaling method supports all that defined in GXIMG_SCALE_METHOD.

     @note If the scaling up/down factor over the HW limitation, it will auto change to FW scaling.

     @param[in] pSrcImg: The source image buffer.
     @param[in] pSrcRegion: The source buffer region for scaling. REGION_MATCH_IMG means the region matches source image buffer.
     @param[in] pDstImg: The destionation image buffer.
     @param[in] pDstRegion: The destionation image buffer region to scaling to. REGION_MATCH_IMG means the region matches destionation image buffer.
     @param[in] ScEng: The scale data engine.
     @return
         - @b E_OK:    Scaling data success.
         - @b E_PAR:   Input parameter error.
*/
extern ER GxImg_ScaleDataEx(PIMG_BUF pSrcImg, IRECT *pSrcRegion, PIMG_BUF pDstImg, IRECT *pDstRegion, GX_IMAGE_SC_ENG ScEng);

/**
     Scaling up/down image data with high quality.

     This API will scale up/down image data, the max scale up factor is 32, the max scale down factor is 16.\n
     This API have start address word alignment limitation & width/height word alignment limitation.\n
     The scaling method supports all that defined in GXIMG_SCALE_METHOD.

     @note If the scaling up/down factor over the HW limitation, or the address alignment not meet the HW limitation,\n
     it wll auto change to FW scaling.

     @param[in] pSrcImg: The source image buffer.
     @param[in] pSrcRegion: The source buffer region for scaling.REGION_MATCH_IMG means the region matches source image buffer.
     @param[in] pDstImg: The destionation image buffer.
     @param[in] pDstRegion: The destionation image buffer region to scaling to. REGION_MATCH_IMG means the region matches destionation image buffer.
     @return
         - @b E_OK:    Scale data success.
         - @b E_PAR:   Input parameter error.
*/
extern ER GxImg_ScaleDataFine(PIMG_BUF pSrcImg, IRECT *pSrcRegion, PIMG_BUF pDstImg, IRECT *pDstRegion);


/**
     Scaling down Y image data only.

     This API will scale down Y image data only, The scaling method is bilinear.

     @note If the scaling up/down factor over the HW limitation, or the address alignment not meet the HW limitation,\n
     it wll auto change to FW scaling.

     @param[in] pSrcImg: The source image buffer.
     @param[in] pSrcRegion: The source buffer region for scaling.REGION_MATCH_IMG means the region matches source image buffer.
     @param[in] pDstImg: The destionation image buffer.
     @param[in] pDstRegion: The destionation image buffer region to scaling to. REGION_MATCH_IMG means the region matches destionation image buffer.
     @return
         - @b E_OK:    Scale data success.
         - @b E_PAR:   Input parameter error.
*/
extern ER GxImg_ScaleDataDownYonly(PIMG_BUF pSrcImg, IRECT *pSrcRegion, PIMG_BUF pDstImg, IRECT *pDstRegion);


/**
     Rotate image data.

     This API will rotate a image data and put the rotated image to dstBuff, the rotated image info will be put in pOutImgBuf.\n
     The needed destination buffer size is align16(w)*align8(h)*3/2;

     @note If the source image is 422 format, the roated image will be translated to 420 format, because the rotate operation need 420 foramt.

     @param[in] pImgBuf: The source image buffer.
     @param[in] dstBuff: The destination buffer for putting rotated image.
     @param[in] dstBufSize: The destionation buffer size.
     @param[in] RotateDir: The rotation direction.
     @param[out] pOutImgBuf: The rotated image buffer.
     @return
         - @b E_OK:    Rotate data success.
         - @b E_PAR:   Input parameter error or destination buffer too small.
*/
extern ER GxImg_RotateData(PIMG_BUF pImgBuf, UINT32 dstBuff, UINT32 dstBufSize, GX_IMAGE_RT_ORI RotateDir, PIMG_BUF pOutImgBuf);

/**
     Rotate image data with specific rotation engine.

     This API will rotate a image data and put the rotated image to dstBuff, the rotated image info will be put in pOutImgBuf.\n
     The needed destination buffer size is align16(w)*align8(h)*3/2;

     @note If the source image is 422 format, the roated image will be translated to 420 format, because the rotate operation need 420 foramt.

     @param[in] pImgBuf: The source image buffer.
     @param[in] dstBuff: The destination buffer for putting rotated image.
     @param[in] dstBufSize: The destionation buffer size.
     @param[in] RotateDir: The rotation direction.
     @param[out] pOutImgBuf: The rotated image buffer.
     @param[in] RotateEng: The rotation engine.
     @return
         - @b E_OK:    Rotate data success.
         - @b E_PAR:   Input parameter error or destination buffer too small.
*/
extern ER GxImg_RotateDataEx(PIMG_BUF pImgBuf, UINT32 dstBuff, UINT32 dstBufSize, GX_IMAGE_RT_ORI RotateDir, PIMG_BUF pOutImgBuf, GX_IMAGE_RT_ENG RotateEng);


/**
     Rotate image data with specific source region to specific postion of destination image.

     @note The source image & destination image should be 420 format, because the rotate operation need 420 foramt.

     @param[in] pSrcImg: The source image buffer.
     @param[in] pSrcRegion: The source buffer region for scaling.REGION_MATCH_IMG means the region matches source image buffer.
     @param[in] pDstImg: The destionation image buffer.
     @param[in] pDstLocation: The destionation image buffer location to rotate to.
     @param[in] RotateDir: The rotation direction.
     @param[in] RotateEng: The rotation engine.
     @return
         - @b E_OK:    Rotate data success.
         - @b E_PAR:   Input parameter error or destination buffer too small.
*/
extern ER GxImg_RotatePasteData(PIMG_BUF pSrcImg, IRECT *pSrcRegion, PIMG_BUF pDstImg, IPOINT *pDstLocation, GX_IMAGE_RT_ORI RotateDir, GX_IMAGE_RT_ENG RotateEng);


/**
     Self rotate image data.

     This API will rotate a image data and put the rotated image to original image buffer address.\n
     So the rotated image buffer size should not over the original image buffer size.

     @note If the source image is 422 format, the roated image will be translated to 420 format, because the rotate operation need 420 foramt.

     @param[in,out] pImgBuf: The image buffer.
     @param[in] tmpBuff: The temp buffer for roate operaion.
     @param[in] tmpBufSize: The temp buffer size (need about dstLineoff * dstHeight).
     @param[in] RotateDir: The rotation direction.
     @return
         - @b E_OK:    Rotate data success.
         - @b E_PAR:   Input parameter error or temp buffer too small.
*/
extern ER GxImg_SelfRotate(PIMG_BUF pImgBuf, UINT32 tmpBuff, UINT32 tmpBufSize, GX_IMAGE_RT_ORI RotateDir);

/**
     Color Transform between YUV and RGB.

     This API support the color transform of 3 cases.
     1. YUV packed -> RGB888 planar
     2. RGB888 planar -> YUV packed
     3. ARGB888 packed -> YUV packed (need tmpBuff W*ALIGN_8(H)*7)

     @param[in] pSrcImg: The source image buffer.
     @param[in] pDstImg: The destionation image buffer.
     @param[in] tmpBuff: The temp buffer for color transform.
     @param[in] tmpBufSize: The temp buffer size.


*/
extern ER GxImg_ColorTransform(PIMG_BUF pSrcImg, PIMG_BUF pDstImg, UINT32 tmpBuff, UINT32 tmpBufSize);

/**
     Transform a Gray(8bits-Y) image to YUV packed.

     @param[in] pSrcImg: The source image buffer.
     @param[in] pDstImg: The destionation image buffer.
     @param[in] lookupTblAdr: The color transform lookup table.

*/
extern ER GxImg_GrayToYUV(PIMG_BUF pSrcImg, PIMG_BUF pDstImg, UINT32 lookupTblAdr[MAX_PLANE_NUM]);

/**
     Transform ARGB to YUV with alpha value by graphic engine.

     Supported input formats: \n
     GX_IMAGE_PIXEL_FMT_ARGB1555_PACKED (Alpha0: uiAlpha[3..0], Alpha1: uiAlpha[7..4])
     GX_IMAGE_PIXEL_FMT_ARGB4444_PACKED
     GX_IMAGE_PIXEL_FMT_RGB565_PACKED (Alpha: uiAlpha)

     @param[in] pSrcImg: The source image buffer.
     @param[in] pSrcRegion: The source buffer region for copying. REGION_MATCH_IMG means the region matches source image buffer.
     @param[in] pDstImg: The destionation image buffer.
     @param[in] pDstLocation: The destionation image buffer location.
     @param[in] uiAlpha: The alpha blending value.
     @return
         - @b E_OK:    Operation success.
         - @b E_PAR:   Input parameter error.
         - @b E_SYS:   HW engine or system error
*/
extern ER GxImg_ARGBToYUV_Blend(PIMG_BUF pSrcImg, IRECT *pSrcRegion, PIMG_BUF pDstImg, IPOINT *pDstLocation, UINT32 uiAlpha);

/**
     Transform RGB to YUV with colorkey and alpha by graphic engine.

     Supported input formats: \n
     GX_IMAGE_PIXEL_FMT_RGB565_PACKED

     @param[in] pSrcImg: The source image buffer.
     @param[in] pSrcRegion: The source buffer region for copying. REGION_MATCH_IMG means the region matches source image buffer.
     @param[in] pDstImg: The destionation image buffer.
     @param[in] pDstLocation: The destionation image buffer location.
     @param[in] uiAlpha: The alpha blending value. (Applied only when the Src pixel is not ColorKey)
     @param[in] uiColorKey: The RGB color key. (0x00RRGGBB)
     @return
         - @b E_OK:    Operation success.
         - @b E_PAR:   Input parameter error.
         - @b E_SYS:   HW engine or system error
*/
extern ER GxImg_RGBToYUV_ColorKey(PIMG_BUF pSrcImg, IRECT *pSrcRegion, PIMG_BUF pDstImg, IPOINT *pDstLocation, UINT32 uiAlpha, UINT32 uiColorKey);

/**
     Set a convex quadrilateral cover to the image.

     Supported input formats: \n
     GX_IMAGE_PIXEL_FMT_YUV422_PLANAR
     GX_IMAGE_PIXEL_FMT_YUV420_PLANAR
     GX_IMAGE_PIXEL_FMT_YUV422_PACKED
     GX_IMAGE_PIXEL_FMT_YUV420_PACKED
     GX_IMAGE_PIXEL_FMT_Y_ONLY

     @note The size of pMosaicImg is limited to SrcW/BlkSize and SrcH/BlkSize

     @param[in] pCover: The image cover description.
     @param[in] pDstImg: The destionation image buffer.
     @param[in] pDstRegion: The destionation image region to cover.
     @return
         - @b E_OK:    Operation success.
         - @b E_PAR:   Input parameter error.
         - @b E_SYS:   HW engine or system error
*/
extern ER GxImg_QuadCover(GXIMG_COVER_DESC *pCover, PIMG_BUF pDstImg, IRECT *pDstRegion);
//------------------------------------------------------------------------------
// GxImage Virtual coordinate
//------------------------------------------------------------------------------
/**
     Set virtual coordinate for one image buffer.

     When user set the virtual coordinate rectangle differ with real coordinate rectangle.\n
     The image buffer will work in the virtual coordinate view.

     @param[in] pImgBuf: The image buffer.
     @param[in] pVirtualCoord: The virtual coordinate rectangle.
     @param[in] pRealCoord: The real coordinate rectangle. NULL means pRealCoord = image buff rectangle.
     @return
         - @b E_OK:    Set ok.
         - @b E_PAR:   Input parameter error.
*/
extern ER     GxImg_SetCoord(PIMG_BUF pImgBuf, IRECT *pVirtualCoord, IRECT *pRealCoord);

/**
     Get virtual coordinate of one image buffer.

     @param[in] pImgBuf: The image buffer.
     @return the virtual coordinate rectangle value.

*/
extern IRECT  GxImg_GetVirtualCoord(PIMG_BUF pImgBuf);

/**
     Get real coordinate of one image buffer.

     @param[in] pImgBuf: The image buffer.
     @return the real coordinate rectangle value.

*/
extern IRECT  GxImg_GetRealCoord(PIMG_BUF pImgBuf);

//------------------------------------------------------------------------------
// GxImage Display
//------------------------------------------------------------------------------

/**
     Set display buffer & window to display engine.

     User can call this API to set display buffer & window at the same time, \n
     or just set dispaly buffer but not set window region by input window region with NULL.

     @note This setting will not have the effect until calling GxDisp_Load();

     @param[in] id: The display layer ID.
     @param[in] pSrcImg: The source image buffer.
     @param[in] pSrcRegion: The source buffer region for display. REGION_MATCH_IMG means the region matches source image buffer.
     @param[in] pWindowRegin: The window region size for display. NULL means not to set window region.
     @return
         - @b E_OK:    Set ok.
         - @b E_PAR:   Input parameter error.
*/

extern ER GxDisp_SetData(GXDISP_ID id, PIMG_BUF pSrcImg, IRECT *pSrcRegion, IRECT *pWindowRegin);
/**
     Enable/Disable specific display layer.

     @note This setting will not have the effect until calling GxDisp_Load();

     @param[in] id: The display layer ID.
     @param[in] bEnable: Enable or disable.

*/
extern void GxDisp_Enable(GXDISP_ID id, BOOL bEnable);

/**
     Load the setting to display engine.

     @note This API will auto wait the frame end to avoid screen flash after load setting to display engine.

     Example:
     @code
     {
         GxDisp_SetData(GXDISP_VIDEO1,&m_ImgDisp[id],REGION_MATCH_IMG,&tmpWinRegion);
         GxDisp_Enable(GXDISP_VIDEO1, TRUE);
         GxDisp_Load();
     }
     @endcode
*/
extern void GxDisp_Load(void);


#ifdef __cplusplus
} //extern "C"
#endif

#endif //_GX_IMAGE_H_
