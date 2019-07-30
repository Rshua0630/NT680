/**
    DC and its related utility.

    DC and its related utility.

    @file       GxDC.h
    @ingroup    mIGxgfx
    @note       Nothing

    DC : Drawing Canvas object, a bitmap use as a drawing target/source

    Copyright   Novatek Microelectronics Corp. 2012.  All rights reserved.
*/

/*
Ex:
    LAYER : Display layer, display a DC to specified window on screen
    Get DC from LAYER:
        pDC = DISPLAY_BeginDraw(uiLayerID);
        DISPLAY_EndDraw(uiLayerID, pDC);

    BITMAP : Image with Bitmap type object, contain memory for bitmap image (WORD align)
    Get DC from BITMAP:
        pDC = BITMAP_GetDC(pBm);
        BITMAP_ReleaseDC(pBm, pDC);

    ICON : Image with Icon type object, contain memory for icon image (bit-compacted)
    Get DC from ICON:
        ICON_MakeDC(&dc, pIconDB, uiIconID);

    JPEG : Image with Jpeg stream type object, contain memory for jpeg stream image (only for Blt)
    Get DC from JPEG:
        JPEG_MakeDC(&dc, pJpegStream, uiJpegStreamSize);
*/

#ifndef _GX_DC_H_
#define _GX_DC_H_

#include "GxCommon.h"
#include "GxRect.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
    @addtogroup mIGxgfx
*/
//@{


//--------------------------------------------------------------------------------------
//  type - DC
//--------------------------------------------------------------------------------------
/**
     DC reserved size for cast to internal data structure
*/
#define DC_RESERVE_DWORD            (28-8+16+16)

/**
     DC : Drawing Canvas object.

     DC : Drawing Canvas object, a bitmap use as a drawing target/source
     @note Nothing.
*/
typedef struct _DC {
	UINT32 uiFlag;      ///< Status flag
	UINT16 uiType;      ///< Type (ex: TYPE_XXX)
	UINT16 uiPxlfmt;    ///< Pixel format (ex: PXLFMT_XXX)
	ISIZE size;         ///< Size (w,h in pixel)
	IRECT winRect;      ///< Cropping window (x,y,w,h in pixels)
	IPOINT origPt;      ///< Original point (x,y in pixel)
	IPOINT lastPt;      ///< Last brush point (x,y in pixel)
	IPOINT thisPt;      ///< Current brush point (x,y in pixel)
	UINT32 uiReserved[DC_RESERVE_DWORD];    ///< Reserved data
}
DC, *PDC;


//--------------------------------------------------------------------------------------
//  enum - DC type
//--------------------------------------------------------------------------------------
/**
     @name Type of DC
*/
//@{
#define TYPE_NULL               0x00    ///< state = invalid
#define TYPE_FB                 0x01    ///< state = bitmap with pitch is 4 byes alignment
#define TYPE_BITMAP             0x02    ///< state = bitmap with pitch is 4 byes alignment
#define TYPE_ICON               0x03    ///< state = bitmap with compact bits
#define TYPE_JPEG               0x04    ///< state = JPEG compressed bit stream
//@}

//--------------------------------------------------------------------------------------
//  enum - DC fomat
//--------------------------------------------------------------------------------------
/**
     @name Plane name of different format buffer
*/
//@{
#define PLANE_Y                 0x00    ///< state = Support format: PXLFMT_YUV444, PXLFMT_YUV422, PXLFMT_YUV420
#define PLANE_U                 0x01    ///< state = Support format: PXLFMT_YUV444, PXLFMT_YUV422, PXLFMT_YUV420
#define PLANE_V                 0x02    ///< state = Support format: PXLFMT_YUV444, PXLFMT_YUV422, PXLFMT_YUV420
#define PLANE_R                 0x00    ///< state = Support format: PXLFMT_RGB888
#define PLANE_G                 0x01    ///< state = Support format: PXLFMT_RGB888
#define PLANE_B                 0x02    ///< state = Support format: PXLFMT_RGB888
#define PLANE_RGB               0x03    ///< state = Support format: PXLFMT_RGB888_PK
#define PLANE_RGB565            0x00    ///< state = Support format: PXLFMT_RGB565_PK, PXLFMT_RGBA5658_PK, PXLFMT_RGBA5654_PK
#define PLANE_IDX               0x03    ///< state = Support format: PXLFMT_INDEX1, PXLFMT_INDEX2, PXLFMT_INDEX4, PXLFMT_INDEX8
//@}

/**
     @name Pixel format of DC
*/
//@{
#define PXLFMT_PMODE            0xf0    ///< state = YUV, not RGB
#define PXLFMT_P1_IDX           0x00    ///< state = 1 color plane, index
#define PXLFMT_P1_PK            0x10    ///< state = 1 color plane (8888 packed)
#define PXLFMT_P1_PK2           0x20    ///< state = 2 color plane (565 packed)
#define PXLFMT_P1_PK3           0x30    ///< state = 1 color plane (4444 packed)
#define PXLFMT_P1_PK4           0x40    ///< state = 1 color plane (5551 packed)

#define PXLFMT_RGBA4444_PK      (PXLFMT_P1_PK3)   ///< state = 1 color plane, pixel=RGBA4444, packed 2-byte with B4-G4-R4-A4 order.
#define PXLFMT_RGBA5551_PK      (PXLFMT_P1_PK4)   ///< state = 1 color plane, pixel=RGBA4444, packed 2-byte with B5-G5-R5-A1 order.
#define PXLFMT_RGB888_PK        (PXLFMT_P1_PK | 0x00)   ///< state = 1 color plane, pixel=RGB888, packed 3-byte with B8-G8-R8 order.
#define PXLFMT_RGBD8888_PK      (PXLFMT_P1_PK | 0x03)   ///< state = 1 color plane, pixel=RGBD8888, packed 3-byte with B8-G8-R8 order.
#define PXLFMT_RGBA8888_PK      (PXLFMT_P1_PK | 0x02)   ///< state = 1 color plane, pixel=RGBA8888, packed 4-byte with B8-G8-R8-A8 order.
#define PXLFMT_RGB565_PK        (PXLFMT_P1_PK2 | 0x00)  ///< state = 1 color plane, pixel=RGB565, packed 2-byte with B5-G6-R5 order.
#define PXLFMT_RGBA5658_PK      (PXLFMT_P1_PK2 | 0x01)  ///< state = 2 color plane, pixel=RGB565, and A8, packed 2-byte with B5-G6-R5 order.
#define PXLFMT_RGBA5654_PK      (PXLFMT_P1_PK2 | 0x02)  ///< state = 2 color plane, pixel=RGB565, and A4, packed 2-byte with B5-G6-R5 order.
#define PXLFMT_INDEX1           (PXLFMT_P1_IDX | 0x00)  ///< state = 1 color plane, pixel=INDEX1
#define PXLFMT_INDEX2           (PXLFMT_P1_IDX | 0x01)  ///< state = 1 color plane, pixel=INDEX2
#define PXLFMT_INDEX4           (PXLFMT_P1_IDX | 0x02)  ///< state = 1 color plane, pixel=INDEX4
#define PXLFMT_INDEX8           (PXLFMT_P1_IDX | 0x03)  ///< state = 1 color plane, pixel=INDEX8

//tmp define
#define PXLFMT_YUV422_PK        (0x50)
#define PXLFMT_YUV420_PK        (0x60)

#define PXLFMT_A8               PXLFMT_INDEX8
#define PXLFMT_A4               PXLFMT_INDEX4
//@}



//--------------------------------------------------------------------------------------
//  function - DC create and destory
//--------------------------------------------------------------------------------------
/**
     Verify DC type and format.

     Verify DC type and format.
     @note Nothing

     @param[in] uiType          Type. (ex: TYPE_XXX).
     @param[in] uiPxlfmt        Format (ex: PXLFMT_XXX).
     @return
         - @b GX_OK:            Successed
         - @b GX_ERROR_TYPE:    Failed, not support format with this type.
              GX_ERROR_FORMAT:  Failed, not support format.
*/
RESULT GxGfx_CheckDCTypeFmt(UINT16 uiType, UINT16 uiPxlfmt);

/**
     Calculate DC pitch size.

     Calculate DC pitch size.
     @note Nothing

     @param[in] uiWidth         Width.
     @param[in] uiType          Type. (ex: TYPE_XXX).
     @param[in] uiPxlfmt        Format (ex: PXLFMT_XXX).
     @return                    Buffer pitch size (in bytes)
*/
UINT32 GxGfx_CalcDCPitchSize(UINT32 uiWidth, UINT16 uiType, UINT16 uiPxlfmt);

/**
     Calculate DC memory size.

     Calculate DC memory size.
     @note Nothing

     @param[in] uiWidth         Buffer width..
     @param[in] uiHeight        Buffer height..
     @param[in] uiType          Type. (ex: TYPE_XXX).
     @param[in] uiPxlfmt        Format (ex: PXLFMT_XXX).
     @return                    Buffer total memory size (in bytes)
*/
UINT32 GxGfx_CalcDCMemSize(UINT32 uiWidth, UINT32 uiHeight, UINT16 uiType, UINT16 uiPxlfmt);

/**
     Attach memory to DC.

     Attach memory to DC.
     @note Nothing

     @param[out] pDC            Pointer to DC.
     @param[in] uiType          Type. (ex: TYPE_XXX).
     @param[in] uiPxlfmt        Format (ex: PXLFMT_XXX).
     @param[in] uiWidth         Buffer width (in pixels).
     @param[in] uiHeight        Buffer height (in pixels).
     @param[in] uiPitch         Buffer pitch (in bytes) of plane-0.
     @param[in] pBuf0           Buffer address of plane-0.
     @param[in] pBuf1           Buffer address of plane-1.
     @param[in] pBuf2           Buffer address of plane-2.
     @return
         - @b GX_OK:            Successed
         - @b GX_ERROR_TYPE:    Failed, not support format with this type.
              GX_ERROR_FORMAT:  Failed, not support format.
              GX_NULL_POINTER:  Failed, not given buffer address.
              GX_ERROR_SIZEZERO:  Failed, buffer size (width, height, pitch) is zero.
              GX_ERROR_SIZEALIGN:  Failed, not match size align requirement.
*/
RESULT GxGfx_AttachDC(DC *pDC,
					  UINT16 uiType, UINT16 uiPxlfmt, UINT32 uiWidth, UINT32 uiHeight,
					  UINT32 uiPitch, UINT8 *pBuf0, UINT8 *pBuf1, UINT8 *pBuf2);

/**
     Detach memory from DC.

     Detach memory from DC.
     @note Nothing

     @param[in] pDC             Pointer to DC.
*/
void   GxGfx_DetachDC(DC *pDC);

/**
     Get size of DC.

     Get size of DC.
     @note Nothing

     @param[in] pDC             Pointer to DC.
     @return                    Size (w,h) of DC.
*/
ISIZE  GxGfx_GetSize(const DC *pDC);
//@}

RESULT GxGfx_GetAddr(const DC *_pDC, UINT32 *pPlan1, UINT32 *pPlan2, UINT32 *pPlan3);
RESULT GxGfx_GetLineOffset(const DC *_pDC, UINT32 *pPlan1, UINT32 *pPlan2, UINT32 *pPlan3);

#ifdef __cplusplus
} //extern "C"
#endif

#endif//_GX_DC_H_
