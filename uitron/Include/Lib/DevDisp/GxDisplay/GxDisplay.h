/**
    Display control lib.

    Display control of buffer, blending & scaling window.

    @file       GxDisplay.h
    @note       Nothing

                DISPLAY : Display device, contain many display layers

                LAYER : Display layer, display a DC to specified window on screen

                DC : source buffer for display

    Copyright   Novatek Microelectronics Corp. 2012.  All rights reserved.
*/

#ifndef _GX_DISPLAY_H_
#define _GX_DISPLAY_H_

#include "GxCommon.h"
#include "GxColor.h" //for PALETTE_ITEM type

#ifdef __cplusplus
extern "C" {
#endif

//--------------------------------------------------------------------------------------
//  device
//--------------------------------------------------------------------------------------

/**
     Set display enable  (Low level).

     Set display enable.
     @note This is an low level function for control display in eary boot flow

     @param[in] LayerID    Layer id.(ex: LAYER_XXX).
     @param[in] bEnable    State (TRUE for enable, FALSE for disable).
*/
extern void Display_SetEnable(UINT8 LayerID, BOOL bEnable);

/**
     Set display buffer (Low level).

     Set display buffer.
     @note This is an low level function for control display in eary boot flow

     @param[in] LayerID    Layer id. (ex: LAYER_XXX).
     @param[in] w          Width of buffer (pixels).
     @param[in] h          Height of buffer (pixels).
     @param[in] off        Line offset of buffer (bytes)(y plane).
     @param[in] fmt        Format of buffer (ex: PXLFMT_XXX).
     @param[in] uiBufY     Start address of y plane.
     @param[in] uiBufCb    Start address of cb plane. (or UV-Packed plane)
     @param[in] uiBufCr    Start address of cr plane.
*/
extern void Display_SetBuffer(UINT8 LayerID, UINT32 w, UINT32 h, UINT32 off, UINT32 fmt,
							  UINT32 uiBufY, UINT32 uiBufCb, UINT32 uiBufCr);

/**
     Set display window (Low level).

     Set display window.
     @note This is an low level function for control display in eary boot flow

     @param[in] LayerID    Layer id.(ex: LAYER_XXX).
     @param[in] x          Left of window (base on device physical coordinate)
     @param[in] y          Top of window (base on device physical coordinate)
     @param[in] w          Width of window (base on device physical coordinate)
     @param[in] h          Height of window (base on device physical coordinate)
*/
extern void Display_SetWindow(UINT8 LayerID, int x, int y, int w, int h);




//--------------------------------------------------------------------------------------
//  type - display
//--------------------------------------------------------------------------------------
/**
     Layer initialize struct.

     Layer initialize struct.
     @note For LayerInit().
*/
typedef struct _LAYER_INIT {
	UINT32 uiFlag;          ///< Reserved
//-input buffer
	UINT16 uiType;          ///< buffer type
	UINT16 uiPxlfmt;        ///< buffer format
	UINT32 uiWidth;         ///< buffer width (pixels)
	UINT32 uiHeight;        ///< buffer height (pixels)
	UINT32 uiBufSize;       ///< buffer memory size
	UINT32 pBufAddr[3];     ///< buffer memory address of plane 0,1,2
	UINT16 uiSwapEffect;    ///< buffer swapping method (ex: 1 = DISCARD, 2 = COPY, 3 = FLIP)
	UINT16 uiBufCount;      ///< total buffer count (0 = one buffer, 1 = double buffer, 2 = triple buffer)
//-output window
	IRECT win;              ///< destination scaling window
	UINT32  uiWinAttr;      ///< destination mirror flag (0x01 = MIRROR X, 0x02 = MIRROR Y)
} LAYER_INIT, *PLAYER_INIT;


typedef struct _LAYER_BUFMAN {
	void *(*get)(UINT32 buffer_id);
	UINT32(*calcsize)(LAYER_INIT *pLayerInit);
	void (*attach)(void *buf, LAYER_INIT *pLayerInit, UINT32 buffer_id);
	void (*detach)(void *buf);
	void (*copy)(void *buf1, void *buf2);
	void (*clear)(void *buf);
}
LAYER_BUFMAN;


//--------------------------------------------------------------------------------------
//  enum - display
//--------------------------------------------------------------------------------------
/**
     @name Get Device id and layer id from parameteres
*/
//@{
#define _DD(id)                     (((id)>>4)&0x0f)
#define _DL(id)                     ((id)&0x0f)
#define _LayerID(dev,lyr)           ((((dev)&0x0f)<<4)|((lyr)&0x0f))
//@}

/**
     @name DISPLAY device id
*/
//@{
#ifndef DOUT1
#define DOUT1                       0x00    ///< 1st device
#define DOUT2                       0x10    ///< 2nd device
#define DOUT_MASK                   0xf0    ///< output device mask
#endif
//@}

#ifndef LAYER_NUM
/**
     @name DISPLAY layer id
*/
//@{
#define LAYER_OSD1                  0x00    ///< on screen display 1
#define LAYER_OSD2                  0x01    ///< on screen display 2
#define LAYER_VDO1                  0x02    ///< video 1
#define LAYER_VDO2                  0x03    ///< video 2
#define LAYER_OUTPUT                0x04    ///< output = mix ( osd1, osd2, vdo1, vdo2 )
#define LAYER_FACE                  0x05    ///< face tracing frame
#define LAYER_ALL                   0x0F    ///< all layers
#define LAYER_NUM                   4       ///< total input layer
#define LAYER_MAX                   (LAYER_NUM+1)
//@}
#endif

/**
     @name DISPLAY layer state of LAYER_OSD1
                                  LAYER_OSD2
                                  LAYER_VDO1
                                  LAYER_VDO2
*/
//@{
#define LAYER_STATE_FLAG            0x00    ///< bit0:valid, bit1:draw (RO)
//-input buffer
#define LAYER_STATE_TYPEFMT         0x01    ///< uiType and uiPxlfmt (RO)  (init)
#define LAYER_STATE_BUFWIDTH        0x02    ///< uiWidth (RO)  (init)
#define LAYER_STATE_BUFHEIGHT       0x03    ///< uiHeight (RO)  (init)
#define LAYER_STATE_BUFSIZE         0x04    ///< uiBufSize (RO)  (init)
#define LAYER_STATE_BUFADDR0        0x05    ///< pBufAddr[0] (RO)  (init)
#define LAYER_STATE_BUFADDR1        0x06    ///< pBufAddr[1] (RO)  (init)
#define LAYER_STATE_BUFADDR2        0x07    ///< pBufAddr[2] (RO)  (init)
#define LAYER_STATE_BUFATTR         0x08    ///< uiBufCount and uiSwapEffect (RO)  (init)
//-output window
#define LAYER_STATE_WINX            0x09    ///< window x (R/W)  (init)
#define LAYER_STATE_WINY            0x0A    ///< window y (R/W)  (init)
#define LAYER_STATE_WINW            0x0B    ///< window w (R/W)  (init)
#define LAYER_STATE_WINH            0x0C    ///< window h (R/W)  (init)
#define LAYER_STATE_WINATTR         0x0D    ///< uiMirrorEffect (R/W)  (init)
//-status
#define LAYER_STATE_ENABLE          0x0E    ///< enable/disable
#define LAYER_STATE_INFO            0x0F    ///< current show/current draw/palette color count (RO)
#define LAYER_STATE_NUM             0x10
//@}

/**
     @name DISPLAY layer state of LAYER_OUTPUT
*/
//@{
#define CTRL_STATE_OSD_MIN          0x00
#define CTRL_STATE_OSD1COMPARE      0x00    ///< color compare func = COMPARE_KEY_NEVER/EQUAL
#define CTRL_STATE_OSD1COLORKEY     0x01    ///< OSD1 do color key to see VDO_mix: color key = [index]
#define CTRL_STATE_OSD1BLEND        0x02    ///< blend ctrl of OSD1
#define CTRL_STATE_OSD1ALPHA        0x03    ///< const alpha of OSD1
#define CTRL_STATE_OSD2COMPARE      0x04    ///< color compare func = COMPARE_KEY_NEVER/EQUAL
#define CTRL_STATE_OSD2COLORKEY     0x05    ///< OSD2 do color key to see OSD1: color key = [index]
#define CTRL_STATE_OSD2BLEND        0x06    ///< blend ctrl of OSD2
#define CTRL_STATE_OSD2ALPHA        0x07    ///< const alpha of OSD2
#define CTRL_STATE_OSD_MAX          0x07
#define CTRL_STATE_VDO_MIN          0x08
#define CTRL_STATE_VDO1COMPARE      0x08    ///< color compare func = COMPARE_KEY_NEVER/EQUAL/LESS/GREATER
#define CTRL_STATE_VDO1COLORKEY     0x09    ///< VDO2 do color key to see VDO1: color key = [color]
#define CTRL_STATE_VDO1BLEND        0x0a    ///< (reserved)
#define CTRL_STATE_VDO1ALPHA        0x0b    ///< (reserved)
#define CTRL_STATE_VDO2COMPARE      0x0c    ///< color compare func = COMPARE_KEY_NEVER/EQUAL/LESS/GREATER
#define CTRL_STATE_VDO2COLORKEY     0x0d    ///< VDO2 do color key to see VDO1: color key = [color]
#define CTRL_STATE_VDO2BLEND        0x0e    ///< blend ctrl of VDO2
#define CTRL_STATE_VDO2ALPHA        0x0f    ///< const alpha of VDO2
#define CTRL_STATE_VDO_MAX          0x0f
#define CTRL_STATE_BLEND_MIN        0x10
#define CTRL_STATE_BACKCOLOR        0x10    ///< Background: color
#define CTRL_STATE_ALLCTRL          0x11    ///< bit 0:Swap VDO1 and VDO2 (color key)
#define CTRL_STATE_ALLENABLE        0x12    ///< bit 1:OSD1 enable, bit 2:OSD2 enable, bit 3:VDO1 enable, bit 4:VDO2 enable
#define CTRL_STATE_BLEND_MAX        0x12
#define CTRL_STATE_NUM              0x13
//@}

#define DISP_OSD1_EN                0x02
#define DISP_OSD2_EN                0x04
#define DISP_VDO1_EN                0x08
#define DISP_VDO2_EN                0x10

#define TYPEFMT_TYPE                0x0000ffff ///< Mask for accessing TYPE in LAYER_STATE_TYPEFMT
#define TYPEFMT_TYPE_SHIFT          0 ///< Shift for accessing TYPE in LAYER_STATE_TYPEFMT
#define TYPEFMT_FMT                 0xffff0000 ///< Mask for accessing FMT in LAYER_STATE_TYPEFMT
#define TYPEFMT_FMT_SHIFT           16 ///< Shift for accessing FMT in LAYER_STATE_TYPEFMT

//#define DISP_PXLFMT_INDEX1          0x0001
//#define DISP_PXLFMT_INDEX2          0x0002
//#define DISP_PXLFMT_INDEX4          0x0003
#define DISP_PXLFMT_INDEX8          0x0004
#define DISP_PXLFMT_ARGB8565_PK     0x0101
//#define DISP_PXLFMT_ARGB4565_PK     0x0102
#define DISP_PXLFMT_ARGB8888_PK     0x0103
#define DISP_PXLFMT_ARGB1555_PK     0x0104
#define DISP_PXLFMT_ARGB4444_PK     0x0105
#define DISP_PXLFMT_YUV422_PK       0x0201
#define DISP_PXLFMT_YUV420_PK       0x0202

/**
     If TYPE value is alpha format.

     If TYPE value is alpha format.
     @note Nothing.

     @param[in] v               Value of LAYER_STATE_TYPEFMT.
     @return                    TYPE value is alpha format.
*/
#define IS_ALPHA_TYPEFMT(v)         (((v) & TYPEFMT_TYPE) & 0x0100)

/**
     Get TYPE value in LAYER_STATE_TYPEFMT.

     Get TYPE value in LAYER_STATE_TYPEFMT.
     @note Nothing.

     @param[in] v               Value of LAYER_STATE_TYPEFMT.
     @return                    TYPE value in LAYER_STATE_TYPEFMT.
*/
#define TYPEFMT_GET_TYPE(v)         (((v) & TYPEFMT_TYPE) >> TYPEFMT_TYPE_SHIFT)

/**
     Get FMT value in LAYER_STATE_TYPEFMT.

     Get FMT value in LAYER_STATE_TYPEFMT.
     @note Nothing.

     @param[in] v               Value of LAYER_STATE_TYPEFMT.
     @return                    FMT value in LAYER_STATE_TYPEFMT.
*/
#define TYPEFMT_GET_FMT(v)          (((v) & TYPEFMT_FMT) >> TYPEFMT_FMT_SHIFT)

/**
     Generate value of LAYER_STATE_TYPEFMT

     Generate value of LAYER_STATE_TYPEFMT.
     @note Nothing.

     @param[in] tp              buffer TYPE
     @param[in] fmt             buffer FMT
     @return                    Value of LAYER_STATE_TYPEFMT.
*/
#define TYPEFMT_MAKE(tp, fmt)       (((fmt) << TYPEFMT_FMT_SHIFT) | ((tp) << TYPEFMT_TYPE_SHIFT))

/**
     @name SWAPEFFECT methods
*/
//@{
#define SWAPEFFECT_DISCARD          0x00    ///< Discard method. Draw on front (Maybe flicker defect)
#define SWAPEFFECT_COPY             0x01    ///< Copy method. Copy font->back, draw on back, swap back->front
#define SWAPEFFECT_FLIP             0x02    ///< Flip method. Draw on back, swap back->front
//@}

#define SWAPEFFECT_DEFAULT          SWAPEFFECT_DISCARD ///< Defalut value for SWAPEFFECT

#define SWAPEFFECT_XY               0x8000    ///< Rotate

#define BUFCOUNT_DEFAULT            (0) ///< Defalut value for BUFCOUNT

#define BUFATTR_SWAPEFFECT          0x00007fff ///< Mask for accessing SWAPEFFECT in LAYER_STATE_BUFATTR
#define BUFATTR_SWAPEFFECT_SHIFT    0  ///< Shift for accessing SWAPEFFECT in LAYER_STATE_BUFATTR
#define BUFATTR_BUFCOUNT            0xffff0000  ///< Mask for accessing BUFCOUNT in LAYER_STATE_BUFATTR
#define BUFATTR_BUFCOUNT_SHIFT      16  ///< Shift for accessing BUFCOUNT in LAYER_STATE_BUFATTR

/**
     Get BUFCOUNT value in LAYER_STATE_BUFATTR.

     Get BUFCOUNT value in LAYER_STATE_BUFATTR.
     @note Nothing.

     @param[in] v               Value of LAYER_STATE_BUFATTR.
     @return                    BUFCOUNT value in LAYER_STATE_BUFATTR.
*/
#define BUFATTR_GET_BUFCOUNT(v)     (((v) & BUFATTR_BUFCOUNT) >> BUFATTR_BUFCOUNT_SHIFT)

/**
     Get SWAPEFFEFT method in LAYER_STATE_BUFATTR.

     Get SWAPEFFEFT method in LAYER_STATE_BUFATTR.
     @note Nothing.

     @param[in] v               Value of LAYER_STATE_BUFATTR.
     @return                    SWAPEFFEFT method in LAYER_STATE_BUFATTR.
*/
#define BUFATTR_GET_SWAPEFFECT(v)   (((v) & BUFATTR_SWAPEFFECT) >> BUFATTR_SWAPEFFECT_SHIFT)

/**
     Generate value of LAYER_STATE_BUFATTR

     Generate value of LAYER_STATE_BUFATTR.
     @note Nothing.

     @param[in] swap            SWAPEFFECT method
     @param[in] bufc            BUFCOUNT value
     @return                    Value of LAYER_STATE_BUFATTR.
*/
#define BUFATTR_MAKE(swap, bufc)    (((bufc) << BUFATTR_BUFCOUNT_SHIFT) | ((swap) << BUFATTR_SWAPEFFECT_SHIFT))

#define BUFATTR_DEFAULT             BUFATTR_MAKE(BUFCOUNT_DEFAULT, SWAPEFFECT_DEFAULT) ///< Default value for LAYER_STATE_BUFATTR

#ifndef MIRROR_X
/**
     @name MIRROR methods
*/
//@{
//
#define MIRROR_DISCARD              0x00    ///< no mirror
#define MIRROR_X                    0x01    ///< mirror in x direction
#define MIRROR_Y                    0x02    ///< mirror in y direction
#define MIRROR_KEEP                 0xff    ///< keep current direction
//@}

#define MIRROR_DEFAULT              (MIRROR_DISCARD) ///< Default value for MIRROR
#endif

#define WINATTR_MIRROR              0x0000ffff ///< Mask for accessing LAYER_STATE_WINATTR

/**
     Get MIRROR method in LAYER_STATE_WINATTR.

     Get MIRROR method in LAYER_STATE_WINATTR.
     @note Nothing.

     @param[in] v               Value of LAYER_STATE_WINATTR.
     @return                    MIRROR method in LAYER_STATE_WINATTR.
*/
#define WINATTR_GET_MIRROR(v)       ((v) & WINATTR_MIRROR)

/**
     Generate value of LAYER_STATE_WINATTR

     Generate value of LAYER_STATE_WINATTR.
     @note Nothing.

     @param[in] mirr            MIRROR method
     @return                    Value of LAYER_STATE_WINATTR.
*/
#define WINATTR_MAKE(mirr)          ((mirr) & WINATTR_MIRROR)

#define WINATTR_DEFAULT             WINATTR_MAKE(MIRROR_DEFAULT) ///< Default value for LAYER_STATE_WINATTR


/**
     @name Flag for CTRL_STATE_OSD1COMPARE
                    CTRL_STATE_OSD2COMPARE
                    CTRL_STATE_VDO1COMPARE
                    CTRL_STATE_VDO2COMPARE
*/
//@{
#define COMPARE_KEY_NEVER           0       ///< never key
#define COMPARE_KEY_LESS            1       ///< key if pixel color is less then color key
#define COMPARE_KEY_EQUAL           2       ///< key if pixel color is equal to color key
#define COMPARE_KEY_GREATER         3       ///< key if pixel color is grater then color key
//@}

/**
     @name Flag for CTRL_STATE_OSD1BLEND
                    CTRL_STATE_OSD2BLEND
                    CTRL_STATE_VDO1BLEND
                    CTRL_STATE_VDO2BLEND
*/
//@{
#ifndef GX_BLEND_OP
#define GX_BLEND_OP
#define BLEND_OPAQUE_000            0x00    ///< Output = Under_Layer
#define BLEND_OPAQUE_012            0x01    ///< Output = This_Layer * 12% + Under_Layer * 88%
#define BLEND_OPAQUE_025            0x02    ///< Output = This_Layer * 25% + Under_Layer * 75%
#define BLEND_OPAQUE_050            0x03    ///< Output = This_Layer * 50% + Under_Layer * 50%
#define BLEND_OPAQUE_075            0x04    ///< Output = This_Layer * 75% + Under_Layer * 25%
#define BLEND_OPAQUE_088            0x05    ///< Output = This_Layer * 88% + Under_Layer * 12%
#define BLEND_OPAQUE_100            0x06    ///< Output = This_Layer
#endif //GX_BLEND_OP
//@}

/**
     @name Ctrl value for  CTRL_STATE_OSD1BLEND
                           CTRL_STATE_OSD2BLEND
                           CTRL_STATE_VDO1BLEND
                           CTRL_STATE_VDO2BLEND
*/
//@{
#define BLEND_DISABLE               0x00    ///< blending disable
#define BLEND_CONSTALPHA            0x01    ///< blending by layer const alpha
#define BLEND_PIXELALPHA            0x02    ///< blending by layer pixel alpha
#define BLEND_R1                    0x03    ///< (reserved)
//@}


//--------------------------------------------------------------------------------------
//  function - init/exit
//--------------------------------------------------------------------------------------
extern void  GxDisplay_RegBufMan(UINT32 LayerID, LAYER_BUFMAN *pBufMan);

/**
     Initialize display.

     Initialize display to start control.
     @note Nothing

     @param[in] DevID           Device id and Layer id.(ex: DOUT1|LAYER_XXX or DOUT2|LAYER_XXX ).
     @param[in] uiDispWidth     Width of device physical size.
     @param[in] uiDispHeight    Height of device physical size.
*/
extern void GxDisplay_Init(UINT32 DevID, UINT32 uiDispWidth, UINT32 uiDispHeight);

/**
     Uninitialize display.

     Uninitialize display to stop control.
     @note Nothing

     @param[in] DevID           Device id and Layer id.(ex: DOUT1|LAYER_XXX or DOUT2|LAYER_XXX ).
*/
extern void GxDisplay_Exit(UINT32 DevID);

//--------------------------------------------------------------------------------------
//  function - init layer
//--------------------------------------------------------------------------------------
/**
     Initialize display layer.

     Initialize display layer buffer & window.
     @note User need to call GxDisplay_Flush() to present visual effect on screen.

     @param[in] DevID           Device id and Layer id.(ex: DOUT1|LAYER_XXX or DOUT2|LAYER_XXX ).
     @param[in] pInit           Initial struct LAYER_INIT.
     @param[in] bClear          Is clear buffer? (TRUE for enable, FALSE for disable).
     @return
         - @b GX_OK:   Successed
         - @b others:  Failed
*/
extern RESULT GxDisplay_InitLayer(UINT32 LayerID, const LAYER_INIT *pInit, BOOL bClear);

/**
     Uninitialize display layer.

     Uninitialize display layer.
     @note Only return buffer to system, not affect any visual effect on screen.

     @param[in] DevID           Device id and Layer id.(ex: DOUT1|LAYER_XXX or DOUT2|LAYER_XXX ).
*/
extern void GxDisplay_ExitLayer(UINT32 LayerID);

/**
     Check valid of display layer.

     Check valid of display layer.
     @note Valid = Already initialzed.

     @param[in] DevID           Device id and Layer id.(ex: DOUT1|LAYER_XXX or DOUT2|LAYER_XXX ).
     @return
         - @b TRUE:   Valid
         - @b FALSE:  Invalid
*/
extern BOOL GxDisplay_IsReady(UINT32 LayerID);

//--------------------------------------------------------------------------------------
//  function - set layer state
//--------------------------------------------------------------------------------------
/**
     Set state of display layer.

     Set state of display layer.
     @note User need to call GxDisplay_Flush() to present visual effect on screen.

     @param[in] DevID           Device id and Layer id.(ex: DOUT1|LAYER_XXX or DOUT2|LAYER_XXX ).
     @param[in] nState          State name. (ex: LAYER_STATE_XXX)
     @param[in] nValue          Value of this state.
*/
extern void GxDisplay_Set(UINT32 LayerID, UINT16 nState, UINT32 nValue);

/**
     Get state of display layer.

     Get state of display layer.
     @note Nothing.

     @param[in] DevID           Device id and Layer id.(ex: DOUT1|LAYER_XXX or DOUT2|LAYER_XXX ).
     @param[in] nState          State name. (ex: LAYER_STATE_XXX)
     @return                    Value of this state.
*/
extern UINT32 GxDisplay_Get(UINT32 LayerID, UINT16 nState);

/**
     Set source window of display layer.

     Set source window of display layer.
     @note Source window is an cropping rectangle of source buffer.

     @param[in] DevID           Device id and Layer id.(ex: DOUT1|LAYER_XXX or DOUT2|LAYER_XXX ).
     @param[in] x               Left of window (base on buffer coordinate)
     @param[in] y               Top of window (base on buffer coordinate)
     @param[in] w               Width of window (base on buffer coordinate)
     @param[in] h               Height of window (base on buffer coordinate)
*/
extern void GxDisplay_SetSrcWindow(UINT32 LayerID, INT32 x, INT32 y, INT32 w, INT32 h);

/**
     Set destination window of display layer.

     Set destination window of display layer.
     @note Destination window is an effective rectangle on device screen.
           User need to call GxDisplay_Flush() to present visual effect on screen.

     @param[in] DevID           Device id and Layer id.(ex: DOUT1|LAYER_XXX or DOUT2|LAYER_XXX ).
     @param[in] x               Left of window (base on device physical coordinate)
     @param[in] y               Top of window (base on device physical coordinate)
     @param[in] w               Width of window (base on device physical coordinate)
     @param[in] h               Height of window (base on device physical coordinate)
     @param[in] uiMirror        Mirror & flip control flag (ex: MIRROR_XXX).
*/
extern void GxDisplay_SetDestWindow(UINT32 LayerID, INT32 x, INT32 y, INT32 w, INT32 h, UINT32 uiMirror);

/**
     Set swapping of display layer.

     Set buffer swapping control method of display layer.
     @note User need to call GxDisplay_Flush() to present visual effect on screen.

     @param[in] DevID           Device id and Layer id.(ex: DOUT1|LAYER_XXX or DOUT2|LAYER_XXX ).
     @param[in] uiSwapEffect    Buffer swapping control method (ex: SWAPEFFECT_XXX).
*/
extern void GxDisplay_SetSwapEffect(UINT32 LayerID, UINT32 uiSwapEffect);

/**
     Set palette of display layer.

     Set palette of display layer.
     @note Palette is only for INDEX format buffer.
           User need to call GxDisplay_Flush() to present visual effect on screen.

     @param[in] DevID           Device id and Layer id.(ex: DOUT1|LAYER_XXX or DOUT2|LAYER_XXX ).
     @param[in] uiStartID       Start index of current change range (ex: 0~255).
     @param[in] uiCount         Total index count of current change range (ex: 0~255).
     @param[in] pTable          Pointer to palette color items of current change.
*/
extern void GxDisplay_SetPalette(UINT32 LayerID, UINT16 uiStartID, UINT16 uiCount, const PALETTE_ITEM *pTable);

/**
     Get palette of display layer.

     Get palette of display layer.
     @note Palette is only for INDEX format buffer.

     @param[in] DevID           Device id and Layer id.(ex: DOUT1|LAYER_XXX or DOUT2|LAYER_XXX ).
     @param[in] uiStartID       Start index of current readback range (ex: 0~255).
     @param[in] uiCount         Total index count of current readback range (ex: 0~255).
     @param[in] pTable          Pointer to palette color items of current readback.
*/
extern void GxDisplay_GetPalette(UINT32 LayerID, UINT16 uiStartID, UINT16 uiCount, PALETTE_ITEM *pTable);

/**
     Set backgorund color of display device.

     Set backgorund color of display device.
     @note User need to call GxDisplay_Flush() to present visual effect on screen.

     @param[in] uiColor         Backgound color (ex: this is a YCbCr color).
*/
/*
extern void GxDisplay_SetBack       (UINT32 uiColor);
*/
#define GxDisplay_SetBack(uiColor)  GxDisplay_Set(DOUT1|LAYER_OUTPUT, CTRL_STATE_BACKCOLOR, (uiColor));

//--------------------------------------------------------------------------------------
//  function - state flush control
//--------------------------------------------------------------------------------------

/**
     Flush states of display layer.

     Flush states of display layer.
     @note Nothing.

     @param[in] LayerID         Device id and Layer id.(ex: DOUT1|LAYER_XXX or DOUT2|LAYER_XXX ).
     @param[in] bWait           Wait VD (TRUE for wait, FALSE for not wait).
*/
extern void GxDisplay_FlushEx(UINT32 LayerID, BOOL bWait);

/**
     Flush states of display layer.

     Flush states of display layer.
     @note Different between FlushEx() is, it is always auto wait VD.

     @param[in] LayerID         Device id and Layer id.(ex: DOUT1|LAYER_XXX or DOUT2|LAYER_XXX ).
*/
/*
extern void GxDisplay_Flush         (UINT32 LayerID); //this will auto wait VD
*/
#define GxDisplay_Flush(LayerID)    GxDisplay_FlushEx(DOUT1|(LayerID), TRUE)

/**
     Flush states of all display layer.

     Flush states of all display layer.
     @note Different between FlushEx() is, it is flush DOUT1 all layers.

     @param[in] bWait           Wait VD (TRUE for wait, FALSE for not wait).
*/
/*
extern void GxDisplay_FlushAllEx    (BOOL bWait);
*/
#define GxDisplay_FlushAllEx(bWait) GxDisplay_FlushEx(DOUT1|LAYER_ALL, (BOOL)(bWait))

/**
     Flush states of all display layer.

     Flush states of all display layer.
     @note Different between FlushEx() is, it is flush DOUT1 all layers, and always auto wait VD.
*/
/*
extern void GxDisplay_FlushAll      (void); //this will auto wait VD
*/
#define GxDisplay_FlushAll()        GxDisplay_Flush(DOUT1|LAYER_ALL)


//--------------------------------------------------------------------------------------
//  function - buffer swapping control
//--------------------------------------------------------------------------------------
//  DISPLAY : DC.uiType = TYPE_FB

/**
     Begin draw operation to display layer.

     Begin draw operation to display layer.
     @note Nothing.

     @param[in] LayerID         Device id and Layer id.(ex: DOUT1|LAYER_XXX or DOUT2|LAYER_XXX ).
     @return                    Current DC of this layer.
*/
extern void  *GxDisplay_BeginDraw(UINT32 LayerID);

/**
     End draw operation to display layer.

     End draw operation to display layer.
     @note Nothing.

     @param[in] LayerID         Device id and Layer id.(ex: DOUT1|LAYER_XXX or DOUT2|LAYER_XXX ).
     @param[in] pLayerDC        Current DC of this layer.
*/
extern void GxDisplay_EndDraw(UINT32 LayerID, void *pLayerBuf);

/**
     Force flip one times to display layer.

     Force flip one times to display layer.
     @note Nothing.

     @param[in] LayerID         Device id and Layer id.(ex: DOUT1|LAYER_XXX or DOUT2|LAYER_XXX ).
*/
extern void GxDisplay_ForceFlip(UINT32 LayerID);

/**
     Swapping buffer of display layer.

     Swapping buffer of display layer.
     @note Nothing.

     @param[in] LayerID         Device id and Layer id.(ex: DOUT1|LAYER_XXX or DOUT2|LAYER_XXX ).
     @param[in] bWait           Wait VD (TRUE for wait, FALSE for not wait).
*/
extern void GxDisplay_FlipEx(UINT32 LayerID, BOOL bWait);          //NEW API

/**
     Wait VD of display device.

     Swapping buffer of display layer.
     @note Nothing.

     @param[in] LayerID         Device id.(ex: DOUT1 or DOUT2).
*/
extern void GxDisplay_WaitVDEx(UINT32 LayerID);        //NEW API

/**
     Swapping buffer of display layer.

     Swapping buffer of display layer.
     @note Different between FlipEx() is, it is flip DOUT1 all layers.

     @param[in] bWait           Wait VD (TRUE for wait, FALSE for not wait).
*/
/*
extern void GxDisplay_Flip          (BOOL bWait); //update all layers, and show output to screen
*/
#define GxDisplay_Flip(bWait)       GxDisplay_FlipEx(DOUT1|LAYER_ALL, (BOOL)(bWait))

/**
     Wait VD of display device.

     Swapping buffer of display layer.
     @note Different between WaitVDEx() is, it is only wait DOUT1 device.

     @param[in] LayerID         Device id.(ex: DOUT1 or DOUT2).
*/
/*
extern void GxDisplay_WaitVD        (void);
*/
#define GxDisplay_WaitVD()          GxDisplay_WaitVDEx(DOUT1)


#ifdef __cplusplus
} //extern "C"
#endif


#endif//_GX_DISPLAY_H_
