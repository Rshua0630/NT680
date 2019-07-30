/**
    Color related utility.

    Color related utility.

    @file       GxColor.h
    @ingroup    mIGxgfx
    @note       Nothing

    Copyright   Novatek Microelectronics Corp. 2012.  All rights reserved.
*/

/*
    COLOR_ITEM; color value

    PALETTE_ITEM; Palette color value

    MAPPING_ITEM; Color index mapping value
*/

#ifndef _GX_COLOR_H_
#define _GX_COLOR_H_

#include "GxCommon.h"
#include "Color.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
    @addtogroup mIGxgfx
*/
//@{
//--------------------------------------------------------------------------------------
//  color
//--------------------------------------------------------------------------------------

/**
     @name Color value definition
*/
//@{
//#ifndef _WIN32
#define COLOR_BLACK                 COLOR_YUV_BLACK    ///< color value
#define COLOR_DARKRED               COLOR_YUV_DARKRED    ///< color value
#define COLOR_DARKGREEN             COLOR_YUV_DARKGREEN    ///< color value
#define COLOR_DARKYELLOW            COLOR_YUV_DARKYELLOW    ///< color value
#define COLOR_DARKBLUE              COLOR_YUV_DARKBLUE    ///< color value
#define COLOR_DARKMAGENTA           COLOR_YUV_DARKMAGENTA    ///< color value
#define COLOR_DARKCYAN              COLOR_YUV_DARKCYAN    ///< color value
#define COLOR_LIGHTGRAY             COLOR_YUV_LIGHTGRAY    ///< color value
#define COLOR_GRAY                  COLOR_YUV_GRAY    ///< color value
#define COLOR_RED                   COLOR_YUV_RED    ///< color value
#define COLOR_GREEN                 COLOR_YUV_GREEN    ///< color value
#define COLOR_YELLOW                COLOR_YUV_YELLOW    ///< color value
#define COLOR_BLUE                  COLOR_YUV_BLUE    ///< color value
#define COLOR_MAGENTA               COLOR_YUV_MAGENTA    ///< color value
#define COLOR_CYAN                  COLOR_YUV_CYAN    ///< color value
#define COLOR_WHITE                 COLOR_YUV_WHITE    ///< color value
#define COLOR_PASTELGREEN           COLOR_YUV_PASTELGREEN    ///< color value
#define COLOR_PASTELBLUE            COLOR_YUV_PASTELBLUE    ///< color value
#define COLOR_SOFTWHITE             COLOR_YUV_SOFTWHITE    ///< color value
#define COLOR_MEDIUMGRAY            COLOR_YUV_MEDIUMGRAY    ///< color value
/*#else
#define COLOR_BLACK                 COLOR_RGB_BLACK
#define COLOR_DARKRED               COLOR_RGB_DARKRED
#define COLOR_DARKGREEN             COLOR_RGB_DARKGREEN
#define COLOR_DARKYELLOW            COLOR_RGB_DARKYELLOW
#define COLOR_DARKBLUE              COLOR_RGB_DARKBLUE
#define COLOR_DARKMAGENTA           COLOR_RGB_DARKMAGENTA
#define COLOR_DARKCYAN              COLOR_RGB_DARKCYAN
#define COLOR_LIGHTGRAY             COLOR_RGB_LIGHTGRAY
#define COLOR_GRAY                  COLOR_RGB_GRAY
#define COLOR_RED                   COLOR_RGB_RED
#define COLOR_GREEN                 COLOR_RGB_GREEN
#define COLOR_YELLOW                COLOR_RGB_YELLOW
#define COLOR_BLUE                  COLOR_RGB_BLUE
#define COLOR_MAGENTA               COLOR_RGB_MAGENTA
#define COLOR_CYAN                  COLOR_RGB_CYAN
#define COLOR_WHITE                 COLOR_RGB_WHITE
#define COLOR_PASTELGREEN           COLOR_RGB_PASTELGREEN
#define COLOR_PASTELBLUE            COLOR_RGB_PASTELBLUE
#define COLOR_SOFTWHITE             COLOR_RGB_SOFTWHITE
#define COLOR_MEDIUMGRAY            COLOR_RGB_MEDIUMGRAY
#endif*/
//@}

/**
     @name Special purpose color value definition
*/
//@{
#define COLOR_TRANSPARENT           ((UINT32)(0x00FFFFFF))    ///< Transparent color
#define COLOR_DONTDRAW              COLOR_TRANSPARENT    ///< Do not drawing color
#define COLOR_DONTKEY               COLOR_TRANSPARENT    ///< Do not colorkey
//@}


//--------------------------------------------------------------------------------------
//  palette color
//--------------------------------------------------------------------------------------

//<type>

/**
     Color item of palette.

     Color item of palette.
     @note Nothing.
*/
typedef COLOR_ITEM PALETTE_ITEM;

//<enum>

/**
     @name Blending coefficent
*/
//@{
//#ifndef _WIN32
//blend op
#ifndef GX_BLEND_OP
#define GX_BLEND_OP
#define BLEND_OPAQUE_000            0x00    ///< Output = Under_Layer
#define BLEND_OPAQUE_012            0x20    ///< Output = This_Layer * 12% + Under_Layer * 88%
#define BLEND_OPAQUE_025            0x40    ///< Output = This_Layer * 25% + Under_Layer * 75%
#define BLEND_OPAQUE_050            0x80    ///< Output = This_Layer * 50% + Under_Layer * 50%
#define BLEND_OPAQUE_075            0xC0    ///< Output = This_Layer * 75% + Under_Layer * 25%
#define BLEND_OPAQUE_088            0xE0    ///< Output = This_Layer * 88% + Under_Layer * 12%
#define BLEND_OPAQUE_100            0xff    ///< Output = This_Layer
#endif //GX_BLEND_OP
#if 0
#define BLEND_LOGIC_AND             0x07    ///< Output = This_Layer AND Under_Layer
#define BLEND_LOGIC_OR              0x08    ///< Output = This_Layer OR Under_Layer
#define BLEND_LOGIC_XOR             0x09    ///< Output = This_Layer XOR Under_Layer
#define BLEND_LOGIC_INVERT          0x0A    ///< Output = INVERT Under_Layer
#endif
#define BLEND_TRANSPARENT           BLEND_OPAQUE_000 ///< Full transparent value
#define BLEND_DEFAULT               BLEND_OPAQUE_100 ///< Default = Full opaque value
//@}

#if 0
//blink op
#define BLINK_OFF                   0x00    //No blink effect
#define BLINK_F1                    0x01    //Using blink with frequence 1
#define BLINK_F2                    0x10    //Using blink with frequence 2
#define BLINK_F3                    0x11    //Using blink with frequence 3
#define BLINK_ON                    BLINK_F1    //Using blink effect
#define BLINK_DEFAULT               BLINK_OFF
#endif

//<function>

/**
     @name Palette data mask
*/
//@{
#define PALETTE_COLOR_MASK          ((COLOR_ITEM)0x00ffffff)    ///< color value mask of palette
#if 0
#define PALETTE_BLENDOP_MASK        ((COLOR_ITEM)0x0f000000)
#define PALETTE_BLINKOP_MASK        ((COLOR_ITEM)0x30000000)
#endif
#define PALETTE_ALPHA_MASK          ((COLOR_ITEM)0xff000000)    ///< alpah value mask of palette
#define PALETTE_ALL_MASK            ((COLOR_ITEM)0xffffffff)    ///< full mask of palette
//@}

/*
//inline function
PALETTE_ITEM PALETTE_MAKE(UINT8 blend_op, UINT8 blink_op, COLOR_ITEM color);
COLOR_ITEM PALETTE_GET_COLOR(PALETTE_ITEM pal);
UINT8 PALETTE_GET_BLENDOP(PALETTE_ITEM pal);
UINT8 PALETTE_GET_BLINKOP(PALETTE_ITEM pal);
*/


/*
//palette : set color and attr
#define PALETTE_MAKE(blend_op, blink_op, color)   \
        (\
        (((COLOR_ITEM)(color)) & 0x00ffffff) | \
        ((((COLOR_ITEM)(blend_op)) & 0x0f) << 24) | \
        (((COLOR_ITEM)(blink_op!=0)) << 28) \
        )
*/

/**
     Make YUVA palette color.

     Make YUVA palette color by alpha value & color value.
     @note Nothing

     @param[in] alpha           Alpha value.
     @param[in] color           YUVA Color value.
     @return                    YUVA Palette color.
*/
#define PALETTE_YUVA_MAKE(alpha, color)   \
	(\
	 (((COLOR_ITEM)(color)) & 0x00ffffff) | \
	 ((((COLOR_ITEM)(alpha)) & 0x000000ff)<<24) \
	)

/**
     Make RGBA palette color.

     Make RGBA palette color by alpha value & color value.
     @note Nothing

     @param[in] alpha           Alpha value.
     @param[in] color           RGBA Color value.
     @return                    RGBA Palette color.
*/
#define PALETTE_RGBA_MAKE(alpha, color)   \
	(\
	 (((COLOR_ITEM)(color)) & 0x00ffffff) | \
	 ((((COLOR_ITEM)(alpha)) & 0x000000ff)<<24) \
	)

/**
     Get color value of palette color.

     Get color value of palette color.
     @note Nothing

     @param[in] dw              Palette color.
     @return                    Color value.
*/
#define PALETTE_GET_COLOR(dw)       (((COLOR_ITEM)(dw)) & PALETTE_COLOR_MASK)

/*
#define PALETTE_GET_BLENDOP(dw)     ((((COLOR_ITEM)(dw)) & PALETTE_BLENDOP_MASK) >> 24)
#define PALETTE_GET_BLINKOP(dw)     ((((COLOR_ITEM)(dw)) & PALETTE_BLINKOP_MASK) >> 28)
*/

/**
     Get alpha value of palette color.

     Get alpha value of palette color.
     @note Nothing

     @param[in] dw              Palette color.
     @return                    Alpha value.
*/
#define PALETTE_GET_ALPHA(dw)       ((((COLOR_ITEM)(dw)) & PALETTE_ALPHA_MASK) >> 24)

/**
     Fill default palette.

     Fill default palette.
     @note Nothing

     @param[out] pPalette       Pointer to Palette.
     @param[in] uiPxlfmt        Pixel format (PXLFMT_XXX)
     @param[in] bpp             Bits-per-pixel (1bpp/2bpp/4bpp/8bpp)
     @return
         - @b GX_OK:            Successed
         - @b GX_NULL_POINTER:  Failed, pPalette is NULL.
              GX_ERROR_FORMAT:  Failed, not support format.
*/
RESULT PALETTE_FILL_DEFAULT(PALETTE_ITEM *pPalette, UINT32 uiPxlfmt, UINT8 bpp);

/**
     Fill alpha colors to palette.

     Fill alpha colors to palette.
     @note Nothing

     @param[out] pPalette       Pointer to Palette.
     @param[in] color_first     First color value.
     @param[in] color_last      Last color value.
     @param[in] alpha           Alpha value.
     @return
         - @b GX_OK:            Successed
         - @b GX_NULL_POINTER:  Failed, pPalette is NULL.
*/
RESULT PALETTE_FILL_ALPHA(PALETTE_ITEM *pPalette, COLOR_ITEM color_first, COLOR_ITEM color_last, UINT8 alpha);

/**
     Fill gradient colors to palette.

     Fill gradient colors to palette.
     @note Nothing

     @param[out] pPalette       Pointer to Palette.
     @param[in] color_first     First color value.
     @param[in] color_last      Last color value.
     @param[in] nColor          Number colors of this gradient.
     @return
         - @b GX_OK:            Successed
         - @b GX_NULL_POINTER:  Failed, pPalette is NULL.
              GX_ERROR_SIZEZERO:Failed, nColor is zero.
*/
RESULT PALETTE_FILL_GRADIENT(PALETTE_ITEM *pPalette, COLOR_ITEM color_first, COLOR_ITEM color_last, int nColor);

//--------------------------------------------------------------------------------------
//  color index
//--------------------------------------------------------------------------------------

//<type>

/**
     Color mapping item of palette.

     Color mapping item of palette.
     @note Nothing.
*/
typedef UINT32 MAPPING_ITEM;

//<enum>

/**
     @name Color index definition of INDEX1 format
*/
//@{
#define COLOR_INDEX1_BLACK          0 ///< color index
#define COLOR_INDEX1_WHITE          1 ///< color index
//@}

/**
     @name Color index definition of INDEX2 format
*/
//@{
#define COLOR_INDEX2_BLACK          0 ///< color index
#define COLOR_INDEX2_GRAY           1 ///< color index
#define COLOR_INDEX2_LIGHTGRAY      2 ///< color index
#define COLOR_INDEX2_WHITE          3 ///< color index
//@}

/**
     @name Color index definition of INDEX4 format
*/
//@{
#define COLOR_INDEX4_BLACK          0 ///< color index
#define COLOR_INDEX4_DARKRED        1 ///< color index
#define COLOR_INDEX4_DARKGREEN      2 ///< color index
#define COLOR_INDEX4_DARKYELLOW     3 ///< color index
#define COLOR_INDEX4_DARKBLUE       4 ///< color index
#define COLOR_INDEX4_DARKMAGENTA    5 ///< color index
#define COLOR_INDEX4_DARKCYAN       6 ///< color index
#define COLOR_INDEX4_LIGHTGRAY      7 ///< color index
#define COLOR_INDEX4_GRAY           8 ///< color index
#define COLOR_INDEX4_RED            9 ///< color index
#define COLOR_INDEX4_GREEN          10 ///< color index
#define COLOR_INDEX4_YELLOW         11 ///< color index
#define COLOR_INDEX4_BLUE           12 ///< color index
#define COLOR_INDEX4_MAGENTA        13 ///< color index
#define COLOR_INDEX4_CYAN           14 ///< color index
#define COLOR_INDEX4_WHITE          15 ///< color index
//@}

/**
     @name Color index definition of INDEX8 format
*/
//@{
#define COLOR_INDEX8_BLACK          0 ///< color index
#define COLOR_INDEX8_DARKRED        1 ///< color index
#define COLOR_INDEX8_DARKGREEN      2 ///< color index
#define COLOR_INDEX8_DARKYELLOW     3 ///< color index
#define COLOR_INDEX8_DARKBLUE       4 ///< color index
#define COLOR_INDEX8_DARKMAGENTA    5 ///< color index
#define COLOR_INDEX8_DARKCYAN       6 ///< color index
#define COLOR_INDEX8_LIGHTGRAY      7 ///< color index
#define COLOR_INDEX8_PASTELGREEN    8 ///< color index
#define COLOR_INDEX8_PASTELBLUE     9 ///< color index
#define COLOR_INDEX8_SOFTWHITE      246 ///< color index
#define COLOR_INDEX8_MEDIUMGRAY     247 ///< color index
#define COLOR_INDEX8_GRAY           248 ///< color index
#define COLOR_INDEX8_RED            249 ///< color index
#define COLOR_INDEX8_GREEN          250 ///< color index
#define COLOR_INDEX8_YELLOW         251 ///< color index
#define COLOR_INDEX8_BLUE           252 ///< color index
#define COLOR_INDEX8_MAGENTA        253 ///< color index
#define COLOR_INDEX8_CYAN           254 ///< color index
#define COLOR_INDEX8_WHITE          255 ///< color index
//@}

/**
     Return color index of default 8bpp palette.

     Return color index of default 8bpp palette by grayscale.
     @note Nothing

     @param[in] gray            Gray color.
     @return                    Color index of default 8bpp palette.
*/
MAPPING_ITEM
COLOR_INDEX8_GRAYLEVEL(CVALUE gray);

/**
     Return color index of default 8bpp palette.

     Return color index of default 8bpp palette by red, green, blue.
     @note Nothing

     @param[in] r               Red color.
     @param[in] g               Grees color.
     @param[in] b               Blue color.
     @return                    Color index of default 8bpp palette.
*/
MAPPING_ITEM
COLOR_INDEX8_COLORRGB(CVALUE r, CVALUE g, CVALUE b);



//--------------------------------------------------------------------------------------
//  decode color from color index
//--------------------------------------------------------------------------------------
/**
     Lookup color from color index.

     Lookup color from color index with given mapping table & palette.
     @note Nothing

     @param[in] index           Color index.
     @param[in] pMap            Pointer to Mapping Table.
     @param[in] pPal            Pointer to Palette.
     @return                    Color value.
*/
COLOR_ITEM INDEX_2_COLOR(UINT8 index, MAPPING_ITEM *pMap, PALETTE_ITEM *pPal);


//--------------------------------------------------------------------------------------
//  color index of font
//--------------------------------------------------------------------------------------

//<enum>

/**
     @name FONT's pixel index : predefined font color index
*/
//@{
#define COLOR_INDEX_FONTBACK        0   ///< background-> always Transparent
#define COLOR_INDEX_FONTFACE        1   ///< face      -> Use Face Color
#define COLOR_INDEX_FONTSHADOW      2   ///< shadow    -> Use Shadow Color
#define COLOR_INDEX_FONTLINE        3   ///< line      -> Use Line Color
//@}

//@}

#ifdef __cplusplus
} //extern "C"
#endif


#endif//_GX_COLOR_H_
