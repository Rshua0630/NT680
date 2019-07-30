/**
    Graphics Data definition.

    Graphics Data definition.

    @file       GxData.h
    @ingroup    mIGxgfx
    @note       Nothing
    IMAGE : Image
        BITMAP : Image with Bitmap type object, contain memory for bitmap image (WORD align)
        ICON : Image with Icon type object, contain memory for icon image (bit-compacted)
        JPEG : Image with Jpeg stream type object, contain memory for jpeg stream image (only for Blt)

    STRING : String
        TCHAR : General string
        WCHAR : Wide char (2-byte) string

    Copyright   Novatek Microelectronics Corp. 2012.  All rights reserved.
*/

/*
*/

#ifndef _GX_DATA_H_
#define _GX_DATA_H_


#include "GxCommon.h"
#include "GxDC.h"
#include "GxRect.h"

#ifdef __cplusplus
extern "C" {
#endif
/**
    @addtogroup mIGxgfx
*/
//@{
//GxData feature:
#define GX_SUPPORT_WCHAR        1   ///<switch of support 16-bit char

//--------------------------------------------------------------------------------------
//  type - image
//--------------------------------------------------------------------------------------
/**
    The parameter of image structure
*/

typedef struct _IMAGE {
	DC          dc;                   ///<Drawing canvas
	IRECT       rect;                 ///<Image area
	UINT32      uiPixelSize;          ///<pixel size
	void       *pDynamicBuf;          ///<buffer addr
}
IMAGE;

//--------------------------------------------------------------------------------------
//  function - image
//--------------------------------------------------------------------------------------
/**
     Get Image size.

     @param[in] pImage          Pointer of image.
     @return                    Image size.
*/
ISIZE IMAGE_GetSize(const IMAGE *pImage);

/**
     Get Image active rectangel.

     @param[in] pImage          Pointer of image.
     @param[out] pRect          Pointer of active rectangle.
     @return                    Image size.
*/
RESULT IMAGE_GetActiveRect(const IMAGE *pImage, IRECT *pRect);

/**
     Set Image active rectangel.

     @param[in] pImage          Pointer of image.
     @param[in] x1          The left point.
     @param[in] y1          The top point.
     @param[in] x2;         The right point.
     @param[in] y2;         The bottom point.
     @return
        - @b GX_OK if success.
        - @b GX_NULL_POINTER no image or rectangle pointer.

*/
RESULT IMAGE_SetActiveRect(IMAGE *pImage, LVALUE x1, LVALUE y1, LVALUE x2, LVALUE y2);

/**
     Get Image DC.

     @param[in] pImage          Pointer of image.
     @return                    Image DC.

*/
_INLINE DC *IMAGE_GetDC(IMAGE *pImage)
{
	if (pImage == 0) {
		return 0;
	}
	return &(pImage->dc);
}
//--------------------------------------------------------------------------------------
//  type - string
//--------------------------------------------------------------------------------------
//char types:

typedef unsigned short          WCHAR;        ///<16-bits char
typedef CHAR                    TCHAR;        ///<8-bits char
typedef WCHAR                   TCHAR_VALUE;  ///<common char access type (maximal volume of all support char types)

#define GX_CHAR_T(str)            (str)           ///<8-bits character type
#define GX_WCHAR_T(str)           (L ## str)      ///<16-bits character type

#if (GX_SUPPORT_WCHAR)
#define CHAR_TYPE               WCHAR           ///<character type
#define _T(str)                 GX_WCHAR_T(str)   ///<character string type
#else
#define CHAR_TYPE               TCHAR           ///<character type
#define _T(str)                 GX_CHAR_T(str)    ///<character string type
#endif

//<enum>
#define FMT_WCHAR               ((WCHAR)(0xFEFF)) ///<WCHAR signature
#define FMT_WCHAR_1ST           ((CHAR)(0xFF))    ///<LOW BYTE
#define FMT_WCHAR_2ND           ((CHAR)(0xFE))    ///<HIGH BYTE
#define FMT_WCHAR_SIZE          1 ///<one WCHAR

//--------------------------------------------------------------------------------------
//  function - string
//--------------------------------------------------------------------------------------
/**
     Get string  length.

     @param[in] pszSrc          Pointer of string.
     @return                    String length.

*/
INT16 STRING_GetLength(const TCHAR *pszSrc);


//--------------------------------------------------------------------------------------
//  type - image table
//--------------------------------------------------------------------------------------
typedef UINT16                  IVALUE;        ///<table index value
typedef UINT32                  IMAGE_TABLE;   ///<image table

//--------------------------------------------------------------------------------------
//  function - image table
//--------------------------------------------------------------------------------------
/**
     Load image from image table.

     @param[out] pImage          Pointer of image
     @param[in] pTable          Pointer of image table
     @param[in] id              Image ID
     @return
        - @b GX_OK if success
        - @b GX_NULL_POINTER no image pointer
*/
RESULT IMAGE_LoadFromTable(IMAGE *pImage, const IMAGE_TABLE *pTable, IVALUE id);

/**
     Load image size from image table.

     @param[in] pTable          Pointer of image table
     @param[in] id              Image ID
     @param[out] pImageSize     Pointer of image size

     @return
        - @b GX_OK if success
        - @b GX_NULL_POINTER no size pointer
        - @b GX_NO_IMAGETABLE no image table
        - @b GX_OUTOF_IMAGEID image id out of range
*/
RESULT IMAGE_GetSizeFromTable(const IMAGE_TABLE *pTable, IVALUE id, ISIZE *pImageSize);

//--------------------------------------------------------------------------------------
//  type - string table
//--------------------------------------------------------------------------------------

typedef UINT32                  STRING_TABLE;   ///<stirng table
//--------------------------------------------------------------------------------------
//  function - string table
//--------------------------------------------------------------------------------------
/**
     Load string from string table.

     @param[in] pTable          Pointer of string table
     @param[in] id              string ID
     @return                    Pointer of image
*/
const TCHAR *
STRING_LoadFromTable(const STRING_TABLE *pTable, IVALUE id);

/**
     Get string size from string table.

     @param[in] pTable          Pointer of string table
     @param[in] id              String ID
     @param[out] pStringLen     Pointer of string length

     @return
        - @b GX_OK if success
        - @b GX_NULL_POINTER no length pointer
        - @b GX_NO_IMAGETABLE no string table
        - @b GX_OUTOF_IMAGEID string id out of range
*/
RESULT STRING_GetLengthFromTable(const STRING_TABLE *pTable, IVALUE id, INT16 *pStringLen);

//--------------------------------------------------------------------------------------
//  type - font
//--------------------------------------------------------------------------------------
typedef UINT32  FONT;       ///<font type
//--------------------------------------------------------------------------------------
//  function - font
//--------------------------------------------------------------------------------------


//--------------------------------------------------------------------------------------
//  type - drawing table
//--------------------------------------------------------------------------------------
typedef UINT32                  DRAWING_TABLE;   ///<type of drawing table

//--------------------------------------------------------------------------------------
//  function - drawing table
//--------------------------------------------------------------------------------------
//@}


#ifdef __cplusplus
} //extern "C"
#endif

#endif//_GX_DATA_H_
