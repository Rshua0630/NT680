/**
    UI draw header file.

    UI draw export variables and function prototypes.

    @file       UIDrawShowObj.h
    @ingroup    mIUIControl
    @note       Nothing (or anything need to be mentioned).

    Copyright   Novatek Microelectronics Corp. 2011.  All rights reserved.
*/

#ifndef UIDRAW_SHOWOBJ_H
#define UIDRAW_SHOWOBJ_H
#include "GxGfx.h"


/**
     @addtogroup mIUIControl
*/
//@{

/**
    Show object base structure define
*/
#define DECLARE_BASE   \
	INT32      ItemType;\
	INT32      x1;      \
	INT32      y1;      \
	INT32      x2;      \
	INT32      y2;


/**
    Show object base structure
*/
typedef struct _ITEM_BASE {
	DECLARE_BASE
} ITEM_BASE;

/**
    Shape base structure
*/
#define DECLARE_SHAPE_BASE    \
	UINT32  uiLineStyle;  \
	UINT32  uiFillStyle;  \
	UINT32  uiForeColor;  \
	UINT32  uiBackColor;  \
	UINT32  uiLayout;     \
	UINT32  uiAlignment;  \
	void    *uiColorMapTable;




/**
    Rectangel shape structure
*/
typedef struct _ITEM_RECTANGLE {
	DECLARE_BASE
	DECLARE_SHAPE_BASE
} ITEM_RECTANGLE;

/**
    Round rectangel shape structure
*/
typedef struct _ITEM_ROUNDRECT {
	DECLARE_BASE                ///< Base item
	DECLARE_SHAPE_BASE          ///< Base shape item
	UINT16   RoundX;            ///< Round x
	UINT16   RoundY;            ///< Round y
} ITEM_ROUNDRECT;

/**
    Ellipse shape structure
*/
typedef struct _ITEM_ELLIPSE {
	DECLARE_BASE                ///< Base item
	DECLARE_SHAPE_BASE          ///< Base shape item
} ITEM_ELLIPSE;

/**
    Line structure
*/
typedef struct _ITEM_LINE {
	DECLARE_BASE                ///< Base item
	UINT32  uiLineStyle;        ///< Line style
	UINT32  uiFillStyle;        ///< Fill syle
	UINT32  uiLineColor;        ///< Line color(refer to palette index)
	void    *uiColorMapTable;   ///< Color mapping table
} ITEM_LINE;

/**
    Image structure
*/
typedef struct _ITEM_IMAGE {
	DECLARE_BASE                    ///< Base item
	UINT32  Content;                ///< Content,icon id
	UINT32  uiBltROP;               ///< Image ROP(refer to BR_IMAGEROP)
	UINT32  uiParamROP;             ///< Parameter fo ROP
	UINT32  uiColorKey;             ///< Color key index
	void    *uiColorMapTable;       ///< Color mapping table
	UINT32  uiLayout;               ///< Image layout
	UINT32  uiCLipping;             ///< Image clipping
	UINT32  uiAlignment;            ///< Alignment(refer to the value for BR_IMAGEALIGN)
	void    *pImageTable;           ///< Image table
} ITEM_IMAGE;


/**
    Text structure
*/
typedef struct _ITEM_TEXT {
	DECLARE_BASE                    ///< Base item
	UINT32  Content;                ///< Content,string id
	void    *uiFontTable;           ///< Font table
	UINT32  uiFontStyle;            ///< Font style
	UINT32  uiFillStyle;            ///< Fill style
	UINT32  uiFontSize;             ///< Font size
	UINT32  uiTextColor;            ///< Text color index
	UINT32  uiShadowColor;          ///< Shadow color index
	UINT32  uiLineColor;            ///< Line color index
	UINT32  uiLayout;               ///< Layout
	UINT32  uiCLipping;             ///< String clipping
	UINT32  uiAlignment;            ///< Alignment (refer to the value for BR_TEXTALIGN)
	UINT16  uiLineHeight;           ///< Line height
	UINT16  uiLetterSpace;          ///< Letter space
	UINT16  uiIndentSpace;          ///< Indent space
	void    *pStringTable;          ///< String table
} ITEM_TEXT;


/**
    Group object structure
*/
typedef struct _ITEM_GROUP {
	DECLARE_BASE                     ///< Base item
	struct _ITEM_BASE **ShowTable;   ///< Show table
} ITEM_GROUP;

/**
    @name show object command
*/
//@{
#define CMD_MASK          0x00FF
#define CMD_DrawNull      0         ///< Null
#define CMD_Rectangle     1         ///< Rectangle type
#define CMD_RoundRect     2         ///< Round Rectangle type
#define CMD_Ellipse       3         ///< Ellipse type
#define CMD_Line          4         ///< Line type
#define CMD_Image         5         ///< Image type
#define CMD_Text          6         ///< Text type
#define CMD_Group         7         ///< Group type,it can be all command group,ex:On backgroun user can draw all kind of shape
#define CMD_MAX           8         ///< Max type command
//@}

/**
    @name show object extention command
*/
//@{
#define CMD_EXT_MASK     0xFF00     ///< Extention command mask
#define CMD_ITEM         0x0100     ///< Item command.
#define CMD_VALUE        0x0200     ///< Value command
//@}

/**
    Special text define for distinguish string id and string pointer.
*/
#define TEXT_POINTER            0x10000000  ///< Use TEXT_POINTER to define string pointer.

/**
    Make string pointer identifed
    @param[in] txt       The string pointer

    @return string pointer with identification
    Example:
    @code
    {
        static char showString[6]="test";
        UxStatic_SetData(&StaticCtrl,STATIC_VALUE,Txt_Pointer(showString));
    }
    @endcode
*/
#define Txt_Pointer(txt)       ((UINT32)txt|TEXT_POINTER)

/**
    Special string id
*/
#define STRID_NULL       0          ///< Reserved 0 as string id null
/**
    Special icon id
*/
#define ICONID_NULL     0xFFFFFFFF  ///< Reserved -1 as icon id null


/**
    show object property type
*/
typedef enum {
	PRO_ICONID = 0x0,             ///< Icon ID for image type.
	PRO_STRID,                    ///< String ID for text type.
	PRO_STRCOLOR,                 ///< String color for ext type.
	PRO_SHAPE_COLOR,              ///< Shape color. All shape object has this property.
	SHOWOBJ_PROPERTY_TYPE_MAX     ///< Max porperty type
} SHOWOBJ_PROPERTY_TYPE;

/**
    Draw function prototype.
    @note ecah show type has its own default draw function
*/
typedef void (*SHOWOBJ_FUNC_PTR)(UIScreen ScreenObj, ITEM_BASE *pShowObj);

/**
     Set origin point of screen.

     @param[in] ScreenObj       The pointer to screen object
     @param[in] x               x of origin point.
     @param[in] y               y of origin point.
*/
extern void Ux_SetOrigin(UIScreen ScreenObj, LVALUE x, LVALUE y);

/**
     Get origin point of screen.

     @param[in] ScreenObj       The pointer to screen object

     @return The point of screen.
*/
extern IPOINT Ux_GetOrigin(UIScreen ScreenObj);

/**
     Set show object position.

     @param[in] pShowObj       The pointer of show object
     @param[in] x1             The left point
     @param[in] y1             The top point
     @param[in] x2             The right point
     @param[in] y2             The bottom point
*/
extern void Ux_SetShowObjPos(ITEM_BASE *pShowObj, INT32 x1, INT32 y1, INT32 x2, INT32 y2);

/**
     Get show object position.

     @param[in] pShowObj       The pointer of show object
     @param[out] x1            The left point
     @param[out] y1            The top point
     @param[out] x2            The right point
     @param[out] y2            The bottom point
*/
extern void Ux_GetShowObjPos(ITEM_BASE *pShowObj, INT32 *x1, INT32 *y1, INT32 *x2, INT32 *y2);

/**
     Draw show object in screen object.
     This API would according to the item type to draw different sharp.

     @param[in] ScreenObj      The pointer of screen object
     @param[in] pShowObj       The pointer of show object
*/
extern void Ux_DrawShowObj(UIScreen ScreenObj, ITEM_BASE *pShowObj);

/**
     Draw show object by status in screen object.
     This API is for some control which has diffenent staus to draw its item.
     Ex:Menu control has many item and each item has diffetent status.

     @param[in] ScreenObj      The pointer of screen object
     @param[in] pShowObj       The pointer of show object of one item.
     @param[in] stringID       String ID
     @param[in] iconID         Icion ID
     @param[in] value          value (only menu control has value attribute)
*/
extern void Ux_DrawItemByStatus(UIScreen ScreenObj, ITEM_BASE *pShowObj, UINT32 stringID, UINT32 iconID, UINT32 value);

/**
     Draw show tabe in screen object.
     This API would call Ux_DrawShowObj() to draw all show objects in show table.

     @param[in] ScreenObj      The pointer of screen object
     @param[in] ShowTable      The pointer of show table
*/
extern INT32 Ux_DrawShowTable(UIScreen ScreenObj, ITEM_BASE **ShowTable);

/**
     Set show object property
     Each show object type has its own property.

     @param[in] pShowObj       The pointer of show object
     @param[in] propertyIndex  Property index
     @param[in] value          Property value
*/
extern void Ux_SetShowObjProperty(ITEM_BASE *pShowObj, SHOWOBJ_PROPERTY_TYPE propertyIndex, UINT32 value);

/**
     Set draw command function.
     Each show object type has its own default draw function.
     User can use this API replace default function.

     @param[in] cmd    Draw command which would be replaced.
     @param[in] pFunc  Draw function pointer

     Example:
     @code
     {
        void UI_DrawUserShowText(UIScreen ScreenObj,ITEM_BASE* pOrgShowObj)
        {
            //user define draw text behavior
        }
        Ux_SetShowObjFunc(CMD_Text, (SHOWOBJ_FUNC_PTR)UI_DrawUserShowText);
     }
     @endcode
*/
extern void Ux_SetShowObjFunc(UINT32 cmd, SHOWOBJ_FUNC_PTR pFunc);


/**
     Set font table array

     @note There is no limit only one table

     @param[in] pFont   The pointer of font table array

     Example:
     @code
     {
        FONT* UIFontTable[2]=
        {
            (FONT*)gDemoKit_Font,
            (FONT*)gDemoKit_BigFont
        };

        Ux_SetFontTable(UIFontTable);
     }
     @endcode
*/
extern void Ux_SetFontTable(FONT **pFont);

/**
     Get font table

     @param[in] index    The index of font table array

     @return font table
*/
extern FONT *Ux_GetFontTable(UINT32 index);

/**
     Set palette table array

     @note There is no limit only one table

     @param[in] pPaletteTable   The pointer of palette table array

*/
extern void Ux_SetPaletteTable(MAPPING_ITEM **pPaletteTable);

/**
     Get palette table

     @param[in] index    The index of palette table array

     @return palette table
*/
extern PALETTE_ITEM *Ux_GetPaletteTable(UINT32 index);

/**
     Set color mapping table array

     @note There is no limit only one table

     @param[in] pColorMapTable   The pointer of color mapping table array

*/
extern void Ux_SetColorMapTable(MAPPING_ITEM **pColorMapTable);


/**
     Get color mapping table

     @param[in] index    The index of color mapping table array

     @return color mapping table
*/
extern MAPPING_ITEM *Ux_GetColorMapTable(UINT32 index);

/**
     Set global color key
     This is for INDEX8 icon but display on ARBG format display buffer,

     @param[in] bEnable     enable global color key
     @param[in] colorKey    GxGfx_SetImageStroke(ROP_KEY, colorKey); when golbal color key enable

*/
extern void Ux_SetGlobalColorKey(UINT32 bEnable, UINT32 colorKey);

/**
     Get global color key
     This is for INDEX8 icon but display on ARBG format display buffer,

     @param[out] bEnable     get enable value
     @param[out] colorKey    get colorkey value

*/
extern void Ux_GetGlobalColorKey(UINT32 *bEnable, UINT32 *colorKey);

//@}
#endif
