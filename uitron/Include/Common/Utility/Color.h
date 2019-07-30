/**
    Color utility.

    Color utility.

    @file       Color.h
    @ingroup    mIUtilColor
    @note       Nothing.

    Copyright   Novatek Microelectronics Corp. 2012.  All rights reserved.
*/

#ifndef _COLOR_H_
#define _COLOR_H_

#include "Type.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
    @addtogroup mIUtilColor
*/
//@{


//--------------------------------------------------------------------------------------
//  color
//--------------------------------------------------------------------------------------

//<type>

/**
     @name color type
*/
//@{
typedef UINT8   CVALUE; ///< single color value
typedef UINT32  COLOR_ITEM; ///< combo color value
//@}

//<enum>
/**
     @name Pre-defined RGBD color
*/
//@{
//                                  0x00RRGGBB
#define COLOR_RGB_BLACK             0x00000000 ///< built-in color
#define COLOR_RGB_DARKRED           0x00800000 ///< built-in color
#define COLOR_RGB_DARKGREEN         0x00008000 ///< built-in color
#define COLOR_RGB_DARKYELLOW        0x00808000 ///< built-in color
#define COLOR_RGB_DARKBLUE          0x00000080 ///< built-in color
#define COLOR_RGB_DARKMAGENTA       0x00800080 ///< built-in color
#define COLOR_RGB_DARKCYAN          0x00008080 ///< built-in color
#define COLOR_RGB_LIGHTGRAY         0x00C0C0C0 ///< built-in color
#define COLOR_RGB_GRAY              0x00808080 ///< built-in color
#define COLOR_RGB_RED               0x00FF0000 ///< built-in color
#define COLOR_RGB_GREEN             0x0000FF00 ///< built-in color
#define COLOR_RGB_YELLOW            0x00FFFF00 ///< built-in color
#define COLOR_RGB_BLUE              0x000000FF ///< built-in color
#define COLOR_RGB_MAGENTA           0x00FF00FF ///< built-in color
#define COLOR_RGB_CYAN              0x0000FFFF ///< built-in color
#define COLOR_RGB_WHITE             0x00FFFFFF ///< built-in color
#define COLOR_RGB_PASTELGREEN       0x00C0DCC0 ///< built-in color
#define COLOR_RGB_PASTELBLUE        0x00A6CAF0 ///< built-in color
#define COLOR_RGB_SOFTWHITE         0x00FFFBF0 ///< built-in color
#define COLOR_RGB_MEDIUMGRAY        0x00A0A0A4 ///< built-in color
//@}


/**
     @name Pre-defined YUVD color
    @note       This value is calc by Excel rgb2yuv matrix.
*/
//@{
//                                  0x00VVUUYY
#define COLOR_YUV_BLACK             0x00808000 ///< built-in color
#define COLOR_YUV_DARKRED           0x00C06A26 ///< built-in color
#define COLOR_YUV_DARKGREEN         0x004A564B ///< built-in color
#define COLOR_YUV_DARKYELLOW        0x008A4071 ///< built-in color
#define COLOR_YUV_DARKBLUE          0x0076C00F ///< built-in color
#define COLOR_YUV_DARKMAGENTA       0x00B6AA35 ///< built-in color
#define COLOR_YUV_DARKCYAN          0x0040965A ///< built-in color
#define COLOR_YUV_LIGHTGRAY         0x008080C0 ///< built-in color
#define COLOR_YUV_GRAY              0x00808080 ///< built-in color
#define COLOR_YUV_RED               0x00FF554C ///< built-in color
#define COLOR_YUV_GREEN             0x00152C96 ///< built-in color
#define COLOR_YUV_YELLOW            0x009500E2 ///< built-in color
#define COLOR_YUV_BLUE              0x006BFF1D ///< built-in color
#define COLOR_YUV_MAGENTA           0x00EBD469 ///< built-in color
#define COLOR_YUV_CYAN              0x0001ABB3 ///< built-in color
#define COLOR_YUV_WHITE             0x008080FF ///< built-in color
#define COLOR_YUV_PURPLE            0x00B5AA35 ///< built-in color
#define COLOR_YUV_ORANGE            0x00BA1DAD ///< built-in color
#define COLOR_YUV_PASTELGREEN       0x008080FF ///< built-in color
#define COLOR_YUV_PASTELBLUE        0x008080FF ///< built-in color
#define COLOR_YUV_SOFTWHITE         0x008080FF ///< built-in color
#define COLOR_YUV_MEDIUMGRAY        0x008080FF ///< built-in color
//@}


//<function>
/**
    @name color macro
    @note       This value is calc by Excel rgb2yuv matrix.
*/
/*
     -# CVALUE COLOR_YUVD_GET_Y(COLOR_ITEM yuvd);
     -# CVALUE COLOR_YUVD_GET_U(COLOR_ITEM yuvd);
     -# CVALUE COLOR_YUVD_GET_V(COLOR_ITEM yuvd);
     -# CVALUE COLOR_RGBD_GET_R(COLOR_ITEM rgbd);
     -# CVALUE COLOR_RGBD_GET_G(COLOR_ITEM rgbd);
     -# CVALUE COLOR_RGBD_GET_B(COLOR_ITEM rgbd);
     -# COLOR_ITEM COLOR_MAKE_RGBD(CVALUE r, CVALUE g, CVALUE b);
     -# COLOR_ITEM COLOR_MAKE_YUVD(CVALUE y, CVALUE u, CVALUE v);
*/
//@{
#define COLOR_YUVD_GET_V(c)         (CVALUE)(((c) & 0x00ff0000) >> 16) ///< get U channel value of YUVD color
#define COLOR_YUVD_GET_U(c)         (CVALUE)(((c) & 0x0000ff00) >> 8) ///< get U channel value of YUVD color
#define COLOR_YUVD_GET_Y(c)         (CVALUE)(((c) & 0x000000ff) >> 0) ///< get Y channel value of YUVD color

#define COLOR_YUVA_GET_A(c)         (CVALUE)(((c) & 0xff000000) >> 24) ///< get A channel value of YUVA color
#define COLOR_YUVA_GET_V(c)         (CVALUE)(((c) & 0x00ff0000) >> 16) ///< get V channel value of YUVA color
#define COLOR_YUVA_GET_U(c)         (CVALUE)(((c) & 0x0000ff00) >> 8) ///< get U channel value of YUVA color
#define COLOR_YUVA_GET_Y(c)         (CVALUE)(((c) & 0x000000ff) >> 0) ///< get Y channel value of YUVA color

#define COLOR_RGBD_GET_B(c)         (CVALUE)(((c) & 0x000000ff) >> 0) ///< get B channel value of RGBD color
#define COLOR_RGBD_GET_G(c)         (CVALUE)(((c) & 0x0000ff00) >> 8) ///< get G channel value of RGBD color
#define COLOR_RGBD_GET_R(c)         (CVALUE)(((c) & 0x00ff0000) >> 16) ///< get R channel value of RGBD color

#define COLOR_RGBA_GET_A(c)         (CVALUE)(((c) & 0xff000000) >> 24) ///< get A channel value of RGBA color
#define COLOR_RGBA_GET_B(c)         (CVALUE)(((c) & 0x000000ff) >> 0) ///< get B channel value of RGBA color
#define COLOR_RGBA_GET_G(c)         (CVALUE)(((c) & 0x0000ff00) >> 8) ///< get G channel value of RGBA color
#define COLOR_RGBA_GET_R(c)         (CVALUE)(((c) & 0x00ff0000) >> 16) ///< get R channel value of RGBA color
#define COLOR_RGBA_GET_COLOR(c)     (COLOR_ITEM)(((c) & 0x00ffffff) >> 0) ///< get RGB channel value of RGBA color

#define COLOR_RGB565_GET_COLOR(c)   (COLOR_ITEM)(((c) & 0x0000ffff) >> 0) ///< get RGB channel value of RGB5658 color
#define COLOR_RGB565_GET_A(c)       (CVALUE)(((c) & 0x00ff0000) >> 16) ///< get A channel value of RGB5658 color
#define COLOR_RGB565_GET_B(c)       ((CVALUE)(((c) & 0x0000001f) >> 0) << 3) ///< get B channel value of RGB5658 color
#define COLOR_RGB565_GET_G(c)       ((CVALUE)(((c) & 0x000007e0) >> 5) << 2) ///< get G channel value of RGB5658 color
#define COLOR_RGB565_GET_R(c)       ((CVALUE)(((c) & 0x0000f800) >> 11) << 3) ///< get R channel value of RGB5658 color

#define COLOR_MAKE_RGBA(r,g,b,a)    \
	(((COLOR_ITEM)(b)&0x000000ff)|\
	 (((COLOR_ITEM)(g)&0x000000ff)<<8)|\
	 (((COLOR_ITEM)(r)&0x000000ff)<<16)|\
	 (((COLOR_ITEM)(a)&0x000000ff)<<24)) ///< make RGBA color by R,G,B,A channel value
#define COLOR_MAKE_RGBD(r,g,b)      \
	(((COLOR_ITEM)(b)&0x000000ff)|\
	 (((COLOR_ITEM)(g)&0x000000ff)<<8)|\
	 (((COLOR_ITEM)(r)&0x000000ff)<<16)) ///< make RGBD color by R,G,B channel value
#define COLOR_MAKE_YUVD(y,u,v)      \
	(((COLOR_ITEM)(y)&0x000000ff)|\
	 (((COLOR_ITEM)(u)&0x000000ff)<<8)|\
	 (((COLOR_ITEM)(v)&0x000000ff)<<16)) ///< make YUVD color by Y,U,V channel value
#define COLOR_MAKE_YUVA(y,u,v,a)    \
	(((COLOR_ITEM)(y)&0x000000ff)|\
	 (((COLOR_ITEM)(u)&0x000000ff)<<8)|\
	 (((COLOR_ITEM)(v)&0x000000ff)<<16)|\
	 (((COLOR_ITEM)(a)&0x000000ff)<<24)) ///< make YUVA color by Y,U,V,A channel value
#define COLOR_MAKE_RGB565(r,g,b,a)  \
	(\
	 (((COLOR_ITEM)(b)&0x000000f8)>>3)|\
	 (((COLOR_ITEM)(g)&0x000000fc)<<3)|\
	 (((COLOR_ITEM)(r)&0x000000f8)<<8)|\
	 (((COLOR_ITEM)(a)&0x000000ff)<<16)) ///< make RGBA5658 color by R,G,B,A channel value
#define COLOR_MAKE_ARGB1555(r,g,b,a)  \
	(\
	 (((COLOR_ITEM)(b)&0x000000f8)>>3)|\
	 (((COLOR_ITEM)(g)&0x000000f8)<<2)|\
	 (((COLOR_ITEM)(r)&0x000000f8)<<7)|\
	 (((COLOR_ITEM)(a)&0x00000080)<<15)) ///< make RGBA5658 color by R,G,B,A channel value
#define COLOR_MAKE_ARGB4444(r,g,b,a)  \
	(\
	 (((COLOR_ITEM)(b)&0x000000f0)>>4)|\
	 (((COLOR_ITEM)(g)&0x000000f0))|\
	 (((COLOR_ITEM)(r)&0x000000f0)<<4)|\
	 (((COLOR_ITEM)(a)&0x000000f0)<<8)) ///< make RGBA5658 color by R,G,B,A channel value
//@}

/**
     Convert RGBD color to YUVD color.

     Convert RGBD color to YUVD color.

     @param[in] rgbd      RGBD color.

     @return YUVD color.
*/
COLOR_ITEM COLOR_RGBD_2_YUVD(COLOR_ITEM rgbd);

/**
     Convert YUVD color to RGBD color.

     Convert YUVD color to RGBD color.

     @param[in] yuvd      YUVD color.

     @return RGBD color.
*/
COLOR_ITEM COLOR_YUVD_2_RGBD(COLOR_ITEM yuvd);

/**
     Convert RGBD color to RGBA5658 color.

     Convert RGBD color to RGBA5658 color.

     @param[in] rgbd      RGBD color.
     @param[in] a         A channel.

     @return RGBA5658 color.
*/
COLOR_ITEM COLOR_RGBD_2_RGB565(COLOR_ITEM rgbd, UINT32 a);

/**
     Convert RGBA color to RGBA5658 color.

     Convert RGBA color to RGBA5658 color.

     @param[in] rgba      RGBA5658 color.

     @return RGBA5658 color.
*/
COLOR_ITEM COLOR_RGBA_2_RGB565(COLOR_ITEM rgba);

/**
     Convert RGBD color to ARGB1555 color.

     Convert RGBD color to ARGB1555 color.

     @param[in] rgbd      RGBD color.
     @param[in] a         A channel.

     @return ARGB1555 color.
*/
COLOR_ITEM COLOR_RGBD_2_ARGB1555(COLOR_ITEM rgbd, UINT32 a);

/**
     Convert RGBA color to ARGB1555 color.

     Convert RGBA color to ARGB1555 color.

     @param[in] rgba      ARGB color.

     @return ARGB1555 color.
*/
COLOR_ITEM COLOR_RGBA_2_ARGB1555(COLOR_ITEM rgba);

/**
     Convert RGBD color to ARGB4444 color.

     Convert RGBD color to ARGB4444 color.

     @param[in] rgbd      RGBD color.
     @param[in] a         A channel.

     @return ARGB4444 color.
*/
COLOR_ITEM COLOR_RGBD_2_ARGB4444(COLOR_ITEM rgbd, UINT32 a);

/**
     Convert RGBA color to ARGB4444 color.

     Convert RGBA color to ARGB4444 color.

     @param[in] rgba      ARGB color.

     @return ARGB4444 color.
*/
COLOR_ITEM COLOR_RGBA_2_ARGB4444(COLOR_ITEM rgba);


#ifdef __cplusplus
} //extern "C"
#endif

//@}

#endif//_COLOR_H_
