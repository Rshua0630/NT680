/**
    Gfx drawing lib.

    Graphics lib for drawing shape, image & text.

    @file       GxGfx.h
    @ingroup    mIGxgfx
    @note       Nothing

                DC : Drawing Canvas object, a bitmap use as a drawing target/source

                BRUSH: Draw brush, a property-set to help DC drawing

                VAR: Draw variable, a variable array to help DC drawing custom items

    Copyright   Novatek Microelectronics Corp. 2012.  All rights reserved.
*/

#ifndef _GX_GRAPHICS_H_
#define _GX_GRAPHICS_H_

#include "GxCommon.h"
#include "GxRect.h"
#include "GxDC.h"
#include "GxData.h"
#include "GxColor.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
    @addtogroup mIGxgfx
*/
//@{
//--------------------------------------------------------------------------------------
//  enum - brush
//--------------------------------------------------------------------------------------
/**
     @name BRUSH for shape setting
*/
//@{
#define BR_SHAPELINESTYLE       0x00  ///< shape line style: line style, brush weight, brush type //(TODO) line style
#define BR_SHAPEFILLSTYLE       0x01  ///< shape rect style: draw line, fill inner
#define BR_SHAPEFORECOLOR       0x02  ///< shape fore color (line color)
#define BR_SHAPEBACKCOLOR       0x03  ///< shape back color (fill color)
#define BR_SHAPEMAPPING         0x04  ///< shape color mapping table
#define BR_SHAPELAYOUT          0x05  ///< shape layout style (TODO)
#define BR_SHAPEALIGN           0x06  ///< shape alignment control (TODO)
//#define BR_SHAPETABLE           0x07  ///< shape table (TODO)
//@}

/**
     @name BRUSH for image setting
*/
//@{
#define BR_IMAGEROP             0x08  ///< image ROP
#define BR_IMAGEPARAM           0x09  ///< image parameters for ROP
#define BR_IMAGEPALETTE         0x0A  ///< image palette = color LUT for mapping index to color
#define BR_IMAGEMAPPING         0x0B  ///< image mapping = index LUT for mapping index to index
#define BR_IMAGELAYOUT          0x0C  ///< image layout (TODO)
#define BR_IMAGEALIGN           0x0D  ///< image align
#define BR_IMAGETABLE           0x0E  ///< image table
//@}

/**
     @name BRUSH for text setting
*/
//@{
#define BR_TEXTFONT             0x0F  ///< text font table
#define BR_TEXTFONTSTYLE        0x10  ///< text font style: font style, font effect //(TODO) font style
#define BR_TEXTFONTSIZE         0x11  ///< text font size (TODO)
#define BR_TEXTFORECOLOR1       0x12  ///< text font face color
#define BR_TEXTFORECOLOR2       0x13  ///< text font shadow color
#define BR_TEXTFORECOLOR3       0x14  ///< text font line color (TODO)
#define BR_TEXTLAYOUT           0x15  ///< text layout: Line-break, Use-Margin
#define BR_TEXTALIGN            0x16  ///< text align: line, vertical, horizontal
#define BR_TEXTLINEHEIGHT       0x17  ///< text line height
#define BR_TEXTLETTERSPACE      0x18  ///< text letter space
#define BR_TEXTINDENTSPACE      0x19  ///< text indent space (TODO)
#define BR_TEXTSTRINGTABLE      0x1A  ///< text string table
#define BR_TEXTCOLORMAPPING     0x1B  ///< text color mapping table
//@}

//--------------------------------------------------------------------------------------
//  enum - brush value
//--------------------------------------------------------------------------------------
/**
     @name value for BR_SHAPELINESTYLE
*/
//@{
#define LINESTYLE_LINE          0  ///< line style : ----------------
#if 0
#define LINESTYLE_DOT           1  ///< line style : . . . . . . . .
#define LINESTYLE_DASH          2  ///< line style : -- -- -- -- --
#define LINESTYLE_DOTDASH       3  ///< line style : . -- . -- . -- .
#define LINESTYLE_DOTDOTDASH    4  ///< line style : . . -- . . -- .
#define LINESTYLE_DOTDASHDASH   5  ///< line style : . -- -- . -- --
#define LINESTYLE_SPARSEDOT     6  ///< line style : .   .   .   .
#define LINESTYLE_SPARSEDASH    7  ///< line style : --  --  --  --
#endif
#define LINESTYLE_MAX           7  ///< line style max value
#define LINEBRUSH_CIRCLE        0x00000000 ///< line brush : circle
#define LINEBRUSH_SQUARE        0x00000010 ///< line brush : square
#define LINEBRUSH_MAX           0x000000f0 ///< line brush max value
#define LINEWEIGHT_MAX          31 ///< line weight max value
//@}

/**
     @name Mask for BR_SHAPELINESTYLE
*/
//@{
#define DSF_LS_MASK             0x00000007 ///< Mask for accessing LINESTYLE in BR_SHAPELINESTYLE
#define DSF_LB_MASK             0x000000f0 ///< Mask for accessing LINEBRUSH in BR_SHAPELINESTYLE
#define DSF_LW_MASK             0x0000ff00 ///< Mask for accessing LINEWEIGHT in BR_SHAPELINESTYLE
//@}

/**
     Generate LINESTYLE value of BR_SHAPELINESTYLE.

     Generate LINESTYLE value of BR_SHAPELINESTYLE.
     @note Nothing.

     @param[in] ls              LINESTYLE_XXX.
     @return                    Value of BR_SHAPELINESTYLE.
*/
#define LINESTYLE(ls)           ((UINT32)(ls) & 0x07)

/**
     Get LINESTYLE value of BR_SHAPELINESTYLE.

     Get LINESTYLE value of BR_SHAPELINESTYLE.
     @note Nothing.

     @param[in] ls               Value of BR_SHAPELINESTYLE.
     @return                    LINESTYLE_XXX.
*/
#define GET_LINESTYLE(ls)       ((ls) & DSF_LS_MASK)

/**
     Generate LINEBRUSH value of BR_SHAPELINESTYLE.

     Generate LINEBRUSH value of BR_SHAPELINESTYLE.
     @note Nothing.

     @param[in] lt              LINEBRUSH_XXX.
     @return                    Value of BR_SHAPELINESTYLE.
*/
#define LINEBRUSH(lt)           (((UINT32)(lt) & 0x0f)<<4)

/**
     Get LINEBRUSH value of BR_SHAPELINESTYLE.

     Get LINEBRUSH value of BR_SHAPELINESTYLE.
     @note Nothing.

     @param[in] lb               Value of BR_SHAPELINESTYLE.
     @return                    LINEBRUSH_XXX.
*/
#define GET_LINEBRUSH(lb)       (((lb) & DSF_LB_MASK) >> 4)

/**
     Generate LINEWEIGHT value of BR_SHAPELINESTYLE.

     Generate LINEWEIGHT value of BR_SHAPELINESTYLE.
     @note Nothing.

     @param[in] lw              LINEWEIGHT_XXX.
     @return                    Value of BR_SHAPELINESTYLE.
*/
#define LINEWEIGHT(lw)          (((UINT32)(lw) & 0xff)<<8)

/**
     Get LINEWEIGHT value of BR_SHAPELINESTYLE.

     Get LINEWEIGHT value of BR_SHAPELINESTYLE.
     @note Nothing.

     @param[in] lw               Value of BR_SHAPELINESTYLE.
     @return                    LINEWEIGHT_XXX.
*/
#define GET_LINEWEIGHT(lw)      (((lw) & DSF_LW_MASK) >> 8)

/**
     @name value for BR_SHAPEFILLSTYLE
*/
//@{
#define FILLSTYLE_FILL          0x00000000  ///< fill style: enable fill color at rect background
#define FILLSTYLE_BORDER        0x00000000  ///< fill style: enable draw frame at rect conner
#define FILLSTYLE_EMPTY         0x00000001  ///< fill style: disable fill color
#define FILLSTYLE_NOBORDER      0x00000002  ///< fill style: disable draw frame
#define FILLSTYLE_FILLALPHA     0x00000010  ///< fill style: fill alpha channel
#define FILLSTYLE_FILLRED       0x00000020  ///< fill style: fill color.r channel
#define FILLSTYLE_FILLGREEN     0x00000040  ///< fill style: fill color.g channel
#define FILLSTYLE_FILLBLUE      0x00000080  ///< fill style: fill color.b channel
#define FILLSTYLE_FILLCOLOR     (FILLSTYLE_FILLRED|FILLSTYLE_FILLGREEN|FILLSTYLE_FILLBLUE) ///< fill style: fill color.rgb channel
//@}

/**
     @name value for BR_IMAGEROP
*/
//@{
#define ROP_COPY                0x00000000 ///< image rop: COPY (Support INDEX,YUV format)
/*
#define ROP_NOT                 0x01000000 ///< image rop: NOT (Support INDEX,YUV format)
#define ROP_AND                 0x02000000 ///< image rop: AND (Support INDEX,YUV format)
#define ROP_OR                  0x03000000 ///< image rop: OR (Support INDEX,YUV format)
#define ROP_XOR                 0x04000000 ///< image rop: XOR (Support INDEX,YUV format)
*/
#define ROP_KEY                 0x05000000 ///< image rop: KEY (Support INDEX,YUV format) param: color = 0x0~0xfffff
//#define ROP_DESTNOT             0x06000000 ///< image rop: DESTNOT (Support INDEX,YUV format)
//#define ROP_DESTKEY             0x07000000 ///< image rop: DESTKEY (Support INDEX,YUV format) param: color = 0x0~0xfffff
#define ROP_DESTFILL            0x08000000 ///< image rop: DESTFILL (Support INDEX,YUV format) param: color = 0x0~0xfffff
/*
#define ROP_ADD                 0x10000000 ///< image rop: ADD (Support YUV format only)  sub-type: SRC_DIV***
#define ROP_SUB                 0x11000000 ///< image rop: SUB (Support YUV format only)  sub-type: SRC_DIV***
#define ROP_ABSSUB              0x12000000 ///< image rop: ABSSUB (Support YUV format only)  sub-type: SRC_DIV***
#define ROP_MUL                 0x13000000 ///< image rop: MUL (Support YUV format only)  sub-type: OUT_DIV***
*/
#define ROP_MUL_CNST_ALPHA      0x13000000 ///< image rop: MUL  alpha a constant value,stand alone ROP
#define ROP_BLEND               0x14000000 ///< image rop: BLEND (alpha plan blenging)
#define ROP_BLEND_CNST_ALPHA    0x15000000 ///< image rop: ROP_BLEND_CNST_ALPHA (constant alpha)
#define ROP_FONT                0x20000000 ///< image rop: FONT (Support INDEX format only) [always rop=ROP_KEY and colorkey =0]
#define ROP_ROTATE              0x40000000 ///< image rop: ROTATE (Support INDEX,YUV format) sub-type: SRC_RCW***,SRC_RCCW***,SRC_MRR***
#define ROP_MASK                0xFF000000 ///< image rop mask
#define ROP_SUB_MASK            0x00ff0000
#define SRC_CNST_ALPHA(a)       (ROP_MUL_CNST_ALPHA>>8 | ((a)&0x000000ff)) ///< image src: ALPHA (subtype for ROP_COPY, ROP_KEY,ROP_BLEND with ARGB image)

#define SRC_RCW_90              0x00000000 ///< image src: rotate CW 90 degree (subtype for ROP_ROTATE)
#define SRC_RCW_180             0x01000000 ///< image src: rotate CW 180 degree (subtype for ROP_ROTATE)
#define SRC_RCW_270             0x02000000 ///< image src: rotate CW 270 degree (subtype for ROP_ROTATE)
#define SRC_RCW_360             0x10000000 ///< image src: rotate CW 360 degree (subtype for ROP_ROTATE)
#define SRC_MRR_X               0x04000000 ///< image src: mirror in x (horizontal flip) (subtype for ROP_ROTATE)
#define SRC_MRR_Y               0x08000000 ///< image src: mirror in y (vertical flip) (subtype for ROP_ROTATE)
#define SRC_RCCW_90             SRC_RCW_270 ///< image src: rotate CCW 270 degree (subtype for ROP_ROTATE)
#define SRC_RCCW_180            SRC_RCW_180 ///< image src: rotate CCW 180 degree (subtype for ROP_ROTATE)
#define SRC_RCCW_270            SRC_RCW_90 ///< image src: rotate CCW 90 degree (subtype for ROP_ROTATE)
#define FILTER_NONE             0x00000000 ///< image filter: NONE (subtype for scaling API)
#define FILTER_PIXEL            0x00400000 ///< image filter: PIXEL (TODO) (subtype for scaling API)
#define FILTER_LINEAR           0x00800000 ///< image filter: LINEAR (subtype for scaling API)
#define FILTER_CUBIC            0x00C00000 ///< image filter: CUBIC (TODO) (subtype for scaling API)
#define FILTER_MASK             0x00C00000 ///< image filter mask (subtype for scaling API)
#define FILTER_SHIFT            22 ///< image filter shift
//@}

/**
     @name value for BR_SHAPELAYOUT, BR_IMAGELAYOUT, BR_TEXTLAYOUT
*/
//@{
#define LAYOUT_NORMAL           0x00000000 ///< layout : NORMAL
#define LAYOUT_LINEBREAK        0x00000100 ///< layout : LINEBREAK
#define LAYOUT_LINEWRAP         0x00000200 ///< layout : LINEWRAP
#if 0 //TODO
#define LAYOUT_ROTATE_L         0x00000001
#define LAYOUT_ROTATE_R         0x00000002
#define LAYOUT_MIRROR_X         0x00000004
#define LAYOUT_MIRROR_Y         0x00000008
#define LAYOUT_REPEAT_X         0x00000010
#define LAYOUT_REPEAT_Y         0x00000020
#define LAYOUT_MARGIN           0x00000400
#endif //TODO
#define LAYOUT_CLIPPING         0x00000800 ///< layout : CLIPPING
//@}

/**
     @name value for BR_SHAPEALIGN, BR_IMAGEALIGN, BR_TEXTALIGN
*/
//@{
#define ALIGN_LEFT              0x00000000  ///< alignment: horizontal align to left
#define ALIGN_CENTER            0x00000001  ///< alignment: horizontal align to center
#define ALIGN_RIGHT             0x00000002  ///< alignment: horizontal align to right
#define ALIGN_JUSTIFY           0x00000003  ///< alignment: horizontal align to justify (distributed) (TODO)
#define ALIGN_H_MASK            0x00000003  ///< alignment horizontal mask
#define ALIGN_TOP               0x00000000  ///< alignment: vertical align to top
#define ALIGN_MIDDLE            0x00000010  ///< alignment: vertical align to middle
#define ALIGN_BOTTOM            0x00000020  ///< alignment: vertical align to bottom
#define ALIGN_V_MASK            0x00000030  ///< alignment vertical mask
//@}


/**
     @name value for BR_TEXTFONT
*/
//@{
//scale
#define SCALE_1X                0     ///< font scale=1.00
#if 0 //TODO
#define SCALE_2X                100   ///< font scale=2.00
#define SCALE_3X                200   ///< font scale=3.00
#define SCALE_4X                300   ///< font scale=4.00
#endif //TODO
//@}

/**
     @name value for BR_TEXTFONTSTYLE
*/
//@{
#define FONTSTYLE_NORMAL        0x00000000  ///< font style : normal (use color index 1)
#if 0 //TODO
#define FONTSTYLE_ITALIC        0x00000001  ///< font style : italic (use color index 1)
#define FONTSTYLE_BOLD          0x00000002  ///< font style : bold  (use color index 1)
#define FONTSTYLE_UNDERLINE     0x00000004  ///< font style : underline (use color index 3)
#define FONTSTYLE_STRIKEOUT     0x00000008  ///< font style : strikeout (use color index 3)
#define DSF_FS(fs)              (((UINT32)(fs) & 0x000000ff))
#define DSF_FS_MASK             0x000000ff
#endif //TODO
#define FONTEFFECT_NONE         0x00000000  ///< font effect : none
#define FONTEFFECT_SHADOW       0x00000100  ///< font effect : shadow (use color index 2)
#define FONTEFFECT_SHADOW2      0x00000200  ///< font effect : shadow2 (use color index 2)
#define FONTEFFECT_HIGHLIGHT    0x00000300  ///< font effect : highlight (use color index 2)
#define FONTEFFECT_HIGHLIGHT2   0x00000400  ///< font effect : highlight2 (use color index 2)
#define FONTEFFECT_OUTLINE      0x00000500  ///< font effect : outline (use color index 2)
#define FONTEFFECT_OUTLINE2     0x00000600  ///< font effect : outline2 (use color index 2)
#define F_NOEXTEND              0x00008000  ///< font effect : no auto extend
#define DSF_FE(fs)              ((((UINT32)(fs) & 0x00007f00))>>8)
#define DSF_FE_MASK             0x0000ff00
//@}

/**
     @name value for BR_TEXTALIGN
*/
//@{
#define LETTERALIGN_TOP         0x00000000  ///< letter alignment : text letter align to top
#define LETTERALIGN_BASELINE    0x00000100  ///< letter alignment : text letter align to baseline = (bottom-2)
#define LETTERALIGN_BOTTOM      0x00000200  ///< letter alignment : text letter align to bottom
#define LETTERALIGN_MASK        0x00000300  ///< letter alignment text letter mask
#define STREAMALIGN_FORWARD     0x00000000  ///< stream alignment : text stream is forward
#define STREAMALIGN_BACKWARD    0x00000400  ///< stream alignment : text stream is backward (reverse order) (Ex: Arabic, Persian and Hebrew)
#define STREAMALIGN_MASK        0x00000400  ///< stream alignment text stream mask
//@}

//--------------------------------------------------------------------------------------
//  enum - brush default value
//--------------------------------------------------------------------------------------
/**
     @name default value for shape
*/
//@{
#define SHAPELINESTYLE_DEFAULT  (LINESTYLE_LINE|LINEBRUSH_CIRCLE|LINEWEIGHT(0)) ///< default value
#define SHAPEFILLSTYLE_DEFAULT  (FILLSTYLE_FILL|FILLSTYLE_BORDER) ///< default value
#define SHAPEFORECOLOR_DEFAULT  0 ///< default value
#define SHAPEBACKCOLOR_DEFAULT  0 ///< default value
#define SHAPEMAPPING_DEFAULT    0 ///< default value
#define SHAPELAYOUT_DEFAULT     0 ///< default value (TODO)
#define SHAPEALIGN_DEFAULT      0 ///< default value (TODO)
//@}

/**
     @name default value for image
*/
//@{
#define IMAGEROP_DEFAULT        (ROP_COPY|FILTER_NONE) ///< default value
#define IMAGEPARAM_DEFAULT      0 ///< default value
#define IMAGEPALETTE_DEFAULT    0 ///< default value
#define IMAGEMAPPING_DEFAULT    0 ///< default value
#define IMAGELAYOUT_DEFAULT     (LAYOUT_NORMAL) ///< default value (TODO)
#define IMAGEALIGN_DEFAULT      (ALIGN_LEFT|ALIGN_TOP) ///< default value
#define IMAGETABLE_DEFAULT      0  ///< default value

/**
     @name default value for text
*/
//@{
#define TEXTFONT_DEFAULT        0  ///< default value
#define TEXTFONTSTYLE_DEFAULT   (FONTSTYLE_NORMAL|FONTEFFECT_NONE) ///< default value
#define TEXTFONTSIZE_DEFAULT    SCALE_1X ///< default value (TODO)
#define TEXTFORECOLOR1_DEFAULT  0  ///< default value
#define TEXTFORECOLOR2_DEFAULT  0  ///< default value
#define TEXTFORECOLOR3_DEFAULT  0  ///< default value
#define TEXTLAYOUT_DEFAULT      (LAYOUT_NORMAL)  ///< default value
#define TEXTALIGN_DEFAULT       (LETTERALIGN_TOP|ALIGN_LEFT|ALIGN_TOP)  ///< default value
#define TEXTLINEHEIGHT_DEFAULT  SCALE_1X  ///< default value
#define TEXTLETTERSPACE_DEFAULT 0  ///< default value
#define TEXTINDENTSPACE_DEFAULT 0  ///< default value (TODO)
#define TEXTSTRINGTABLE_DEFAULT 0   ///< default value
//@}

//--------------------------------------------------------------------------------------
//  type - brush
//--------------------------------------------------------------------------------------
/**
    bursh property value
*/
typedef UINT32  BVALUE;

#define BR_STATE_NUM            0x20 ///<- total brush state numbers

/**
     BRUSH info.

     BRUSH info.
     @note Nothing.
*/
typedef struct _BRUSH {
	BVALUE uiState[BR_STATE_NUM]; ///<- state array
} BRUSH, *PBRUSH;

#define FLOAT_TO_BVALUE(fv)     (*(BVALUE*)&(fv))       ///< convert float to BVALUE

/**
    config string buffer size,used for GxGfx_Config
*/
#define CFG_STRING_BUF_SIZE     0x00000001

/**
     Config gfx.

     @note Nothing.

     @param[in] cfg           configure item
     @param[in] vale          configure vale.
*/
RESULT GxGfx_Config(UINT32 cfg, UINT32 vale);

//--------------------------------------------------------------------------------------
//  function - init/exit
//--------------------------------------------------------------------------------------
/**
     Initialize gfx.

     Initialize gfx.
     @note Nothing.

     @param[in] pWorkBuf        Working buffer address. (NOTE: need word alignment)
     @param[in] nWorkBufSize    Working buffer size.
*/
RESULT GxGfx_Init(UINT32 *pWorkBuf, UINT32 nWorkBufSize);

/**
     Uninitialize gfx.

     Uninitialize gfx.
     @note Nothing.
*/
RESULT GxGfx_Exit(void);

/**
     Reset working stack.

     Reset working stack.
     @note The memory space is using working buffer.
*/
void GxGfx_ResetStack(void);

/**
     Alloc space from stack.

     Alloc space from stack.
     @note The memory space is using working buffer.

     @param[in] uiSize          Buffer size.
     @return                    Buffer address.
*/
UINT32 *GxGfx_PushStack(UINT32 uiSize);

/**
     Free space to stack.

     Free space to stack.
     @note The memory space is using working buffer.
*/
void GxGfx_PopStack(void);

/**
     Alloc DC from stack.

     Alloc DC from stack.
     @note The memory space is using working buffer.

     @param[in,out] pDC         Pointer to DC.
     @param[in] uiType          Type. (ex: TYPE_XXX).
     @param[in] uiPxlfmt        Format (ex: PXLFMT_XXX).
     @param[in] uiWidth         Buffer width (in pixels).
     @param[in] uiHeight        Buffer height (in pixels).
     @return
         - @b GX_OK:            Successed
         - @b GX_ERROR_TYPE:    Failed, not support format with this type.
              GX_ERROR_FORMAT:  Failed, not support format.
              GX_NULL_POINTER:  Failed, not given pDC pointer.
              GX_NULL_BUF:      Failed, not enough memory space in stack.
              GX_ERROR_SIZEZERO:  Failed, buffer size (width, height, pitch) is zero.
              GX_ERROR_SIZEALIGN:  Failed, not match size align requirement.
*/
RESULT GxGfx_PushStackDC(DC *pDC, UINT16 uiType, UINT16 uiPxlfmt, UINT32 uiWidth, UINT32 uiHeight);

/**
     Free DC to stack.

     Free DC to stack.
     @note The memory space is using working buffer.

     @param[in,out] pDC         Pointer to DC.
*/
void GxGfx_PopStackDC(DC *pDC);

//--------------------------------------------------------------------------------------
//  function - set/get brush state and variable
//--------------------------------------------------------------------------------------
/**
     Get all brush state.

     Get all brush state.
     @note Nothing.

     @param[in] pBrush          Pointer to BRUSH.
*/
void GxGfx_GetAll(BRUSH *pBrush);

/**
     Set all brush state.

     Set all brush state.
     @note Nothing.

     @param[in] pBrush          Pointer to BRUSH.
*/
void GxGfx_SetAll(const BRUSH *pBrush);

/**
     Set all brush state to default.

     Set all brush state to default.
     @note Nothing.
*/
void GxGfx_SetAllDefault(void);

/**
     internal function.

     internal function.
     @note User should not call this function.
*/
void BR_SetP(UINT8 uiBrush, BVALUE uiValue);

/**
     internal function.

     internal function.
     @note User should not call this function.
*/
BVALUE BR_GetP(UINT8 uiBrush);

/**
     Set BRUSH state.

     Set BRUSH state.
     @note Nothing.

     @param[in] n         Property (ex: BR_XXX).
     @param[in] p         value.
*/
//void   GxGfx_Set                (UINT8 uiProperty, BVALUE uiValue);
#define GxGfx_Set(n, p)         BR_SetP(n, (BVALUE)(p))

/**
     Get BRUSH state.

     Get BRUSH state.
     @note Nothing.

     @param[in] n               Property (ex: BR_XXX).
     @return                    value.
*/
//BVALUE GxGfx_Get                (UINT8 uiProperty);
#define GxGfx_Get(n)            BR_GetP(n)

/**
     internal function.

     internal function.
     @note User should not call this function.
*/
void VAR_SetV(UINT8 nVarId, BVALUE nValue);

/**
     internal function.

     internal function.
     @note User should not call this function.
*/
BVALUE VAR_GetV(UINT8 nVarId);

/**
     Set variable.

     Set variable.
     @note Nothing.

     @param[in] n           variable Id.
     @param[in] v           value.
*/
//void GxGfx_SetVar               (UINT8 nVarId, BVALUE nValue);
#define GxGfx_SetVar(n, v)      VAR_SetV(n, (BVALUE)(v))

/**
     Get variable.

     Get variable.
     @note Nothing.

     @param[in] n               variable Id (ex: BR_XXX).
     @return                    value.
*/
//BVALUE GxGfx_GetVar             (UINT8 nVarId);
#define GxGfx_GetVar(n)         VAR_GetV(n)

//--------------------------------------------------------------------------------------
//  function - set dc unit/origin/window/pos
//--------------------------------------------------------------------------------------
/*
Xfm Relation:
Loc/Size -> Orig -> 0 -> Coord
            Win  -> 0 -> Coord
*/

/**
     Set origin point of DC.

     Set origin point of DC.
     @note Origin point is used to support transform (shift).

     @param[in,out] pDC         Pointer to DC.
     @param[in] x               x of origin point.
     @param[in] y               y of origin point.
     @return
         - @b GX_OK:            Successed
         - @b Other:            Failed.
*/
RESULT GxGfx_SetOrigin(DC *pDC, LVALUE x, LVALUE y);

/**
     Get origin point of DC.

     Get origin point of DC.
     @note Origin point is used to support transform (shift).

     @param[in,out] pDC         Pointer to DC.
     @return                    Origin point.
*/
IPOINT GxGfx_GetOrigin(const DC *pDC);

/**
     Set coordinate system of DC.

     Set coordinate system of DC.
     @note Coordinate system is used to support transform (shift+scale).

     @param[in,out] pDC         Pointer to DC.
     @param[in] vw              Virtual width.
     @param[in] vh              Virtual height.
     @return
         - @b GX_OK:            Successed
         - @b Other:            Failed.
*/
RESULT GxGfx_SetCoord(DC *pDC, LVALUE vw, LVALUE vh);

/**
     Set coordinate system of DC.

     Set coordinate system of DC.
     @note Coordinate system is used to support transform (shift+scale).

     @param[in,out] pDC         Pointer to DC.
     @param[in] vw              Virtual width.
     @param[in] vh              Virtual height.
     @param[in] rx              Real x (base on buffer coordinate).
     @param[in] ry              Real y (base on buffer coordinate).
     @param[in] rw              Real width (base on buffer coordinate).
     @param[in] rh              Real height (base on buffer coordinate).
     @return
         - @b GX_OK:            Successed
         - @b Other:            Failed.
*/
RESULT GxGfx_SetCoordEx(DC *pDC, LVALUE vw, LVALUE vh, LVALUE rx, LVALUE ry, LVALUE rw, LVALUE rh);

/**
     Get coordinate system of DC.

     Get coordinate system of DC.
     @note Coordinate system is used to support transform (shift+scale).

     @param[in,out] pDC         Pointer to DC.
     @return                    Virtual width, height.
         - @b GX_OK:            Successed
         - @b Other:            Failed.
*/
ISIZE GxGfx_GetCoord(const DC *pDC);

/**
     Get coordinate system of DC.

     Get coordinate system of DC.
     @note Coordinate system is used to support transform (shift+scale).

     @param[in,out] pDC         Pointer to DC.
     @return                    Real x,y,width,height (base on buffer coordinate).
*/
IRECT GxGfx_GetCoordEx(const DC *pDC);

/**
     Set clipping window of DC.

     Set clipping window of DC.
     @note Clipping window is used to skip drawing action on non-crop range.

     @param[in,out] pDC         Pointer to DC.
     @param[in] x1              Left of window.
     @param[in] y1              Top of window.
     @param[in] x2              Right of window.
     @param[in] y2              Bottom of window.
     @return
         - @b GX_OK:            Successed
         - @b Other:            Failed.
*/
RESULT GxGfx_SetWindow(DC *pDC, LVALUE x1, LVALUE y1, LVALUE x2, LVALUE y2);

/**
     Get clipping window of DC.

     Get clipping window of DC.
     @note Clipping window is used to skip drawing action on non-crop range.

     @param[in,out] pDC         Pointer to DC.
     @return                    The window
*/
IRECT GxGfx_GetWindow(const DC *pDC);

/**
     Move cursor of DC to new location.

     Move cursor of DC to new location.
     @note Each DC has an cursor to record last drawing location.

     @param[in,out] pDC         Pointer to DC.
     @param[in] x               x of cursor.
     @param[in] y               y of cursor.
     @return
         - @b GX_OK:            Successed
         - @b Other:            Failed.
*/
RESULT GxGfx_MoveTo(DC *pDC, LVALUE x, LVALUE y);

/**
     Move cursor of DC to previous location.

     Move cursor of DC to previous location.
     @note Each DC has an cursor to record last drawing location.

     @param[in,out] pDC         Pointer to DC.
     @return
         - @b GX_OK:            Successed
         - @b Other:            Failed.
*/
RESULT GxGfx_MoveBack(DC *pDC);

/**
     Get cursor location of DC.

     Get cursor location of DC.
     @note Each DC has an cursor to record last drawing location.

     @param[in,out] pDC         Pointer to DC.
     @return                    Current location.
*/
IPOINT GxGfx_GetPos(const DC *pDC);

/**
     Get last cursor movement of DC.

     Get last cursor movement of DC.
     @note Each DC has an cursor to record last drawing location.

     @param[in,out] pDC         Pointer to DC.
     @return                    Last movement (vector).
*/
ISIZE GxGfx_GetLastMove(const DC *pDC);

//--------------------------------------------------------------------------------------
//  function - clear/copy/convert dc
//--------------------------------------------------------------------------------------
//dc clear/copy/convert
/**
     Clear DC.

     Clear DC to zero or black color.
     @note Nothing.

     @param[in,out] pDestDC     Pointer to destination DC.
     @return
         - @b GX_OK:            Successed
         - @b Other:            Failed.
*/
RESULT GxGfx_Clear(DC *pDestDC);

/**
     Copy DC to another DC.

     Copy DC to another DC.
     @note Nothing.

     @param[in,out] pDestDC     Pointer to destination DC.
     @param[in] pSrcDC          Pointer to source DC.
     @return
         - @b GX_OK:            Successed
         - @b Other:            Failed.
*/
RESULT GxGfx_Copy(DC *pDestDC, const DC *pSrcDC);

/**
     Enhanced copy DC to another DC.

     Enhanced copy DC to another DC.
     @note Nothing.

     @param[in,out] pDestDC     Pointer to destination DC.
     @param[in] pSrcDC          Pointer to source DC.
     @param[in] uiRop           Raster OP.
     @param[in] uiParam         Raster parameter.
     @return
         - @b GX_OK:            Successed
         - @b Other:            Failed.
*/
RESULT GxGfx_CopyEx(DC *pDestDC, const DC *pSrcDC, UINT32 uiRop, UINT32 uiParam);

/**
     Convert DC format to another DC format.

     Convert DC format to another DC format.
     @note Nothing.

     @param[in,out] pDestDC     Pointer to destination DC.
     @param[in] pSrcDC          Pointer to source DC.
     @param[in] pConvertTable   Reference convert table.
     @return
         - @b GX_OK:            Successed
         - @b Other:            Failed.
*/
RESULT GxGfx_Convert(DC *pDestDC, const DC *pSrcDC, const void *pConvertTable);

//--------------------------------------------------------------------------------------
//  function - draw shape on dc
//--------------------------------------------------------------------------------------
/**
     Set shape stroke.

     Set shape stroke: line style and fill scale.
     @note Nothing.

     @param[in] uiLineStyle     Line style. (ex: LINESTYLE_XXX|LINEBRUSH_XXX|LINEWEIGHT(n))
     @param[in] uiFillStyle     Fill style (ex: FILLSTYLE_XXX).
*/
void GxGfx_SetShapeStroke(BVALUE uiLineStyle, BVALUE uiFillStyle);

/**
     Set shape color.

     Set shape color: fore-color, back-color and index mapping table.
     @note Nothing.

     @param[in] uiForeColor     Fore-color (ex: index color or y-cb-cr color).
     @param[in] uiBackColor     Back-color (ex: index color or y-cb-cr color).
     @param[in] pTable          Index mapping table.
*/
void GxGfx_SetShapeColor(BVALUE uiForeColor, BVALUE uiBackColor, const MAPPING_ITEM *pTable);

/**
     Set shape layout.

     Set shape layout: shape layout, shape alignment.
     @note Nothing.

     @param[in] uiLayout        Shape layout (ex: LAYOUT_XXX). (TODO)
     @param[in] uiAlignment     Shape alignment (ex: ALIGN_XXX). (TODO)
*/
void GxGfx_SetShapeLayout(BVALUE uiLayout, BVALUE uiAlignment);

/**
     Draw point shape to DC.

     Draw point shape to DC.
     @note Nothing.

     @param[in,out] pDestDC     Pointer to DC.
     @param[in] x               x of point.
     @param[in] y               y of point.
     @return
         - @b GX_OK:            Successed
         - @b Other:            Failed.
*/
RESULT GxGfx_Point(DC *pDestDC, LVALUE x, LVALUE y);

/**
     Draw line shape to DC.

     Draw line shape to DC.
     @note Nothing.

     @param[in,out] pDestDC     Pointer to DC.
     @param[in] x1              x of start point.
     @param[in] y1              y of start point.
     @param[in] x2              x of end point.
     @param[in] y2              y of end point.
     @return
         - @b GX_OK:            Successed
         - @b Other:            Failed.
*/
RESULT GxGfx_Line(DC *pDestDC, LVALUE x1, LVALUE y1, LVALUE x2, LVALUE y2);

/**
     Draw line shape to DC. (from last cursor)

     Draw line shape to DC. (from last cursor)
     @note Nothing.

     @param[in,out] pDestDC     Pointer to DC.
     @param[in] x               x of end point.
     @param[in] y               y of end point.
     @return
         - @b GX_OK:            Successed
         - @b Other:            Failed.
*/
RESULT GxGfx_LineTo(DC *pDestDC, LVALUE x, LVALUE y);

/**
     Draw fill-only rectangle shape to DC.

     Draw fill-only rectangle shape to DC.
     @note Nothing.

     @param[in,out] pDestDC     Pointer to DC.
     @param[in] x1              x of start point.
     @param[in] y1              y of start point.
     @param[in] x2              x of end point.
     @param[in] y2              y of end point.
     @return
         - @b GX_OK:            Successed
         - @b Other:            Failed.
*/
RESULT GxGfx_FillRect(DC *pDestDC, LVALUE x1, LVALUE y1, LVALUE x2, LVALUE y2);

/**
     Draw frame-only rectangle shape to DC.

     Draw frame-only rectangle shape to DC.
     @note Nothing.

     @param[in,out] pDestDC     Pointer to DC.
     @param[in] x1              x of start point.
     @param[in] y1              y of start point.
     @param[in] x2              x of end point.
     @param[in] y2              y of end point.
     @return
         - @b GX_OK:            Successed
         - @b Other:            Failed.
*/
RESULT GxGfx_FrameRect(DC *pDestDC, LVALUE x1, LVALUE y1, LVALUE x2, LVALUE y2);

/**
     Draw color-inverted rectangle shape to DC.

     Draw color-inverted rectangle shape to DC.
     @note Nothing.

     @param[in,out] pDestDC     Pointer to DC.
     @param[in] x1              x of start point.
     @param[in] y1              y of start point.
     @param[in] x2              x of end point.
     @param[in] y2              y of end point.
     @return
         - @b GX_OK:            Successed
         - @b Other:            Failed.
*/
//RESULT GxGfx_InvertRect         (DC* pDestDC, LVALUE x1, LVALUE y1, LVALUE x2, LVALUE y2);

/**
     Draw font-connverted to DC.

     Draw rectangle _pSrcDC to _pDestDC to do font-connverted.
     @note Nothing.

     @param[in,out] _pDestDC     Pointer to destination DC.
     @param[in,out] _pSrcDC     Pointer to source DC.
     @param[in] pColorTable     Pointer of palette table.

     @return
         - @b GX_OK:            Successed
         - @b Other:            Failed.
*/
RESULT GxGfx_ConvertFont(DC *_pDestDC,  const DC *_pSrcDC, const PALETTE_ITEM *pColorTable);

/**
     Draw rectangle shape to DC.

     Draw rectangle shape to DC.
     @note Nothing.

     @param[in,out] pDestDC     Pointer to DC.
     @param[in] x1              x of start point.
     @param[in] y1              y of start point.
     @param[in] x2              x of end point.
     @param[in] y2              y of end point.
     @return
         - @b GX_OK:            Successed
         - @b Other:            Failed.
*/
RESULT GxGfx_Rectangle(DC *pDestDC, LVALUE x1, LVALUE y1, LVALUE x2, LVALUE y2);

/**
     Draw rectangle shape to DC. (from last cursor)

     Draw rectangle shape to DC. (from last cursor)
     @note Nothing.

     @param[in,out] pDestDC     Pointer to DC.
     @param[in] x               x of end point.
     @param[in] y               y of end point.
     @return
         - @b GX_OK:            Successed
         - @b Other:            Failed.
*/
RESULT GxGfx_RectTo(DC *pDestDC, LVALUE x, LVALUE y);              //like GxGfx_Rectangle()

/**
     Draw ellipse shape to DC.

     Draw ellipse shape to DC.
     @note Nothing.

     @param[in,out] pDestDC     Pointer to DC.
     @param[in] x1              x of start point.
     @param[in] y1              y of start point.
     @param[in] x2              x of end point.
     @param[in] y2              y of end point.
     @return
         - @b GX_OK:            Successed
         - @b Other:            Failed.
*/
RESULT GxGfx_Ellipse(DC *pDestDC, LVALUE x1, LVALUE y1, LVALUE x2, LVALUE y2);

/**
     Draw round-conner rectangle shape to DC.

     Draw round-conner rectangle shape to DC.
     @note Nothing.

     @param[in,out] pDestDC     Pointer to DC.
     @param[in] x1              x of start point.
     @param[in] y1              y of start point.
     @param[in] x2              x of end point.
     @param[in] y2              y of end point.
     @param[in] rw              Diameter of x of round-conner.
     @param[in] rh              Diameter of y of round-conner.
     @return
         - @b GX_OK:            Successed
         - @b Other:            Failed.
*/
RESULT GxGfx_RoundRect(DC *pDestDC, LVALUE x1, LVALUE y1, LVALUE x2, LVALUE y2, LVALUE rw, LVALUE rh);

//NOT SUPPORT ANY MORE!
//RESULT GxGfx_ShapeInRect        (DC* pDestDC, LVALUE x1, LVALUE y1, LVALUE x2, LVALUE y2, UINT8 uiShapeType);

//--------------------------------------------------------------------------------------
//  function - draw image on dc
//--------------------------------------------------------------------------------------
/**
     Set image stroke.

     Set image stroke: raster OP and related parameter.
     @note Nothing.

     @param[in] uiRop           Raster OP (ex: ROP_XXX or ROP_XXX|SRC_XXX or ROP_XXX|OUT_XXX or ROP_XXX|FILTER_XXX).
     @param[in] uiParam         Parameter.
*/
void GxGfx_SetImageStroke(BVALUE uiRop, BVALUE uiParam);

/**
     Set text color.

     Set text color: color palette and index mapping table.
     @note Nothing.

     @param[in] pPalette        Color palette.
     @param[in] pTable          Index mapping table.
*/
void GxGfx_SetImageColor(const PALETTE_ITEM *pPalette, const MAPPING_ITEM *pTable);

/**
     Set image layout.

     Set image layout: image layout, image alignment.
     @note Nothing.

     @param[in] uiLayout        Image layout (ex: LAYOUT_XXX).
     @param[in] uiAlignment     Image alignment (ex: ALIGN_XXX).
*/
void GxGfx_SetImageLayout(BVALUE uiLayout, BVALUE uiAlignment);

/**
     Set image current IMAGE_TABLE.

     Set image current IMAGE_TABLE.
     @note Nothing.

     @param[in] pTable          Text IMAGE_TABLE resource.
*/
void GxGfx_SetImageTable(const IMAGE_TABLE *pTable);

/**
     Get image size from current IMAGE_TABLE.

     Get image size from current IMAGE_TABLE.
     @note Nothing.

     @param[in] img_id          Image id.
     @param[out] pImageSize     Size w,h of this image.
     @return                    Image of current IMAGE_TABLE with given id.
*/
RESULT GxGfx_GetImageIDSize(IVALUE img_id, ISIZE *pImageSize);     //(refe to current IMAGE_TABLE)

/**
     Get image from current IMAGE_TABLE.

     Get image from current IMAGE_TABLE.
     @note Nothing.

     @param[in] img_id          Image id.
     @return                    Image of current IMAGE_TABLE with given id.
*/
const IMAGE *GxGfx_GetImageID(IVALUE img_id);

/**
     Get sub-image from current IMAGE_TABLE.

     Get sub-image from current IMAGE_TABLE.
     @note Nothing.

     @param[in] img_id          Image id.
     @param[in] x1              Left of sub-image rectangle (base on image coordinate).
     @param[in] y1              Top of sub-image rectangle (base on image coordinate).
     @param[in] x2              Right of sub-image rectangle (base on image coordinate).
     @param[in] y2              Bottom of sub-image rectangle (base on image coordinate).
     @return                    Image of current IMAGE_TABLE with given id.
*/
const IMAGE *GxGfx_GetImageIDEx(IVALUE img_id, LVALUE x1, LVALUE y1, LVALUE x2, LVALUE y2);

/**
     Draw image to DC.

     Draw image to DC.
     @note Nothing.

     @param[in,out] pDestDC     Pointer to DC.
     @param[in] x               x of Left-Top point.
     @param[in] y               y of Left-Top point.
     @param[in] pImage          Pointer to image resource.
     @return
         - @b GX_OK:            Successed
         - @b Other:            Failed.
*/
RESULT GxGfx_Image(DC *pDestDC, LVALUE x, LVALUE y, const IMAGE *pImage);

/**
     Draw scaling image to DC.

     Draw scaling image to DC.
     @note Nothing.

     @param[in,out] pDestDC     Pointer to DC.
     @param[in] x               x of Left-Top point.
     @param[in] y               y of Left-Top point.
     @param[in] w               Width of scaling image.
     @param[in] h               Height of scaling image.
     @param[in] pImage          Pointer to image resource.
     @return
         - @b GX_OK:            Successed
         - @b Other:            Failed.
*/
RESULT GxGfx_ImageScale(DC *pDestDC, LVALUE x, LVALUE y, LVALUE w, LVALUE h, const IMAGE *pImage);

/**
     Draw image inside rectangle to DC.

     Draw image inside rectangle to DC.
     @note Nothing.

     @param[in,out] pDestDC     Pointer to DC.
     @param[in] x1              x of start point.
     @param[in] y1              y of start point.
     @param[in] x2              x of end point.
     @param[in] y2              y of end point.
     @param[in] pImage          Pointer to image resource.
     @return
         - @b GX_OK:            Successed
         - @b Other:            Failed.
*/
RESULT GxGfx_ImageInRect(DC *pDestDC, LVALUE x1, LVALUE y1, LVALUE x2, LVALUE y2, const IMAGE *pImage);

/**
     Draw scaling image inside rectangle to DC.

     Draw scaling image inside rectangle to DC.
     @note Nothing.

     @param[in,out] pDestDC     Pointer to DC.
     @param[in] x1              x of start point.
     @param[in] y1              y of start point.
     @param[in] x2              x of end point.
     @param[in] y2              y of end point.
     @param[in] w               Width of scaling image.
     @param[in] h               Height of scaling image.
     @param[in] pImage          Pointer to image resource.
     @return
         - @b GX_OK:            Successed
         - @b Other:            Failed.
*/
RESULT GxGfx_ImageScaleInRect(DC *pDestDC, LVALUE x1, LVALUE y1, LVALUE x2, LVALUE y2, LVALUE w, LVALUE h, const IMAGE *pImage);

//--------------------------------------------------------------------------------------
//  function - draw text on dc
//--------------------------------------------------------------------------------------
/**
     Set text stroke.

     Set text stroke: current FONT, font style and font scale.
     @note Nothing.

     @param[in] pFont           Text FONT resource.
     @param[in] uiFontStyle     Style (ex: FONTSTYLE_XXX).
     @param[in] uiScale         Scale (ex: SCALE_XXX).
*/
void GxGfx_SetTextStroke(const FONT *pFont, BVALUE uiFontStyle, BVALUE uiScale);

/**
     Set text color.

     Set text color: font fore-color1, fore-color2 and fore-color3.
     @note Nothing.

     @param[in] uiForeColor1     Fore-color1 (ex: index color or y-cb-cr color).
     @param[in] uiForeColor2     Fore-color2 (ex: index color or y-cb-cr color).
     @param[in] uiForeColor3     Fore-color3 (ex: index color or y-cb-cr color).
*/
void GxGfx_SetTextColor(BVALUE uiForeColor1, BVALUE uiForeColor2, BVALUE uiForeColor3);

/**
     Set text color mapping.

     Set text color mapping,according font format.
     @note Nothing.

     @param[in] pColorTabl     color mapping table.
*/

void GxGfx_SetTextColorMapping(const PALETTE_ITEM* pColorTabl);


/**
     Get text color mapping.

     Get text color mapping,according font format.
     @note Nothing.

     @return      current color mapping table.
*/

PALETTE_ITEM* GxGfx_GetTextColorMapping(void);

/**
     Set text layout.

     Set text layout: text layout, text alignment, line height, character space and ident space.
     @note Nothing.

     @param[in] uiLayout        Text layout (ex: LAYOUT_XXX).
     @param[in] uiAlignment     Text alignment (ex: ALIGN_XXX).
     @param[in] lh              Line height (in pixels, 0=current font height).
     @param[in] cs              Character space (in pixels).
     @param[in] is              Ident space (in pixels)(TODO).
*/
void GxGfx_SetTextLayout(BVALUE uiLayout, BVALUE uiAlignment, BVALUE lh, BVALUE cs, BVALUE is);

/**
     Set text current STRING_TABLE.

     Set text current STRING_TABLE.
     @note Nothing.

     @param[in] pTable          Text STRING_TABLE resource.
*/
void GxGfx_SetStringTable(const STRING_TABLE *pTable);

/**
     Get font average size.

     Get font average size.
     @note Nothing.

     @param[out] pFontSize      Get average character size w,h of current FONT.
     @return
         - @b GX_OK:            Successed
         - @b Other:            Failed.
*/
RESULT GxGfx_GetFontSize(ISIZE *pFontSize);

/**
     Get font character size.

     Get font character size.
     @note Nothing.

     @param[in] chr_id          Character id.
     @param[out] pCharSize      Get given character size w,h of current FONT.
     @return
         - @b GX_OK:            Successed
         - @b Other:            Failed.
*/
RESULT GxGfx_GetFontCharSize(IVALUE chr_id, ISIZE *pCharSize);

/**
     Get string from current STRING_TABLE.

     Get string from current STRING_TABLE.
     @note Nothing.

     @param[in] str_id          String id.
     @return                    String of current STRING_TABLE with given id.
*/
const TCHAR *GxGfx_GetStringID(IVALUE str_id);

/**
     Draw character to DC.

     Draw character to DC.
     @note Nothing.

     @param[in,out] pDestDC     Pointer to DC.
     @param[in] x               x of Left-Top point.
     @param[in] y               y of Left-Top point.
     @param[in] id              Character id (of current FONT).
     @return
         - @b GX_OK:            Successed
         - @b Other:            Failed.
*/
RESULT GxGfx_Char(DC *pDestDC, LVALUE x, LVALUE y, IVALUE id);

/**
     Draw text to DC.

     Draw text to DC.
     @note Nothing.

     @param[in,out] pDestDC     Pointer to DC.
     @param[in] x               x of Left-Top point.
     @param[in] y               y of Left-Top point.
     @param[in] pszSrc          Pointer to string resource.
     @return
         - @b GX_OK:            Successed
         - @b Other:            Failed.
*/
RESULT GxGfx_Text(DC *pDestDC, LVALUE x, LVALUE y, const TCHAR *pszSrc);                //no ESC command (fast)

/**
     Draw text to DC. (with printf format)

     Draw text to DC. (with printf format)
     @note Nothing.

     @param[in,out] pDestDC     Pointer to DC.
     @param[in] x               x of Left-Top point.
     @param[in] y               y of Left-Top point.
     @param[in] pszSrc          Pointer to string resource.
     @return
         - @b GX_OK:            Successed
         - @b Other:            Failed.
*/
RESULT GxGfx_TextPrint(DC *pDestDC, LVALUE x, LVALUE y, const TCHAR *pszSrc, ...);           //no ESC command (fast)

/**
     Draw text inside rectangle to DC.

     Draw text inside rectangle to DC.
     @note Nothing.

     @param[in,out] pDestDC     Pointer to DC.
     @param[in] x1              x of start point.
     @param[in] y1              y of start point.
     @param[in] x2              x of end point.
     @param[in] y2              y of end point.
     @param[in] pszSrc          Pointer to string resource.
     @return
         - @b GX_OK:            Successed
         - @b Other:            Failed.
*/
RESULT GxGfx_TextInRect(DC *pDestDC, LVALUE x1, LVALUE y1, LVALUE x2, LVALUE y2, const TCHAR *pszSrc);          //support ESC command

/**
     Draw text inside rectangle to DC. (with printf format)

     Draw text inside rectangle to DC. (with printf format)
     @note Nothing.

     @param[in,out] pDestDC     Pointer to DC.
     @param[in] x1              x of start point.
     @param[in] y1              y of start point.
     @param[in] x2              x of end point.
     @param[in] y2              y of end point.
     @param[in] pszSrc          Pointer to string resource.
     @return
         - @b GX_OK:            Successed
         - @b Other:            Failed.
*/
RESULT GxGfx_TextPrintInRect(DC *pDestDC, LVALUE x1, LVALUE y1, LVALUE x2, LVALUE y2, const TCHAR *pszSrc, ...);     //support ESC command

/**
     Get last text location.

     Get last text location.
     @note Nothing.

     @return                    Last text location
*/
IPOINT GxGfx_GetTextLastLoc(void);

/**
     Get last text size.

     Get last text size.
     @note Nothing.

     @return                    Last text size
*/
ISIZE GxGfx_GetTextLastSize(void);

//@}

#ifdef __cplusplus
} //extern "C"
#endif

#endif //_GX_GRAPHICS_H_
