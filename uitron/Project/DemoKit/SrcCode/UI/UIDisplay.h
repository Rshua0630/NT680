#ifndef _UI_SHOW_H_
#define _UI_SHOW_H_

#include "GxDisplay.h"
#include "GxGfx.h"
#include "Color.h"

#undef BLEND_OPAQUE_000
#undef BLEND_OPAQUE_012
#undef BLEND_OPAQUE_025
#undef BLEND_OPAQUE_050
#undef BLEND_OPAQUE_075
#undef BLEND_OPAQUE_088
#undef BLEND_OPAQUE_100
#define BLEND_OPAQUE_000            0x00    //Output = Under_Layer
#define BLEND_OPAQUE_012            0x20    //Output = This_Layer * 12% + Under_Layer * 88%
#define BLEND_OPAQUE_025            0x40    //Output = This_Layer * 25% + Under_Layer * 75%
#define BLEND_OPAQUE_050            0x80    //Output = This_Layer * 50% + Under_Layer * 50%
#define BLEND_OPAQUE_075            0xC0    //Output = This_Layer * 75% + Under_Layer * 25%
#define BLEND_OPAQUE_088            0xE0    //Output = This_Layer * 88% + Under_Layer * 12%
#define BLEND_OPAQUE_100            0xff    //Output = This_Layer
#define OSD_OPAQUE_000              BLEND_OPAQUE_000
#define OSD_OPAQUE_050              BLEND_OPAQUE_050
#define OSD_OPAQUE_100              BLEND_OPAQUE_100

//Device Display
//#define DD_OSD 0
//#define DD_OSD2 1
//#define DD_VDO  2
//#define DD_VDO2 3
//#define DD_OUT  4   //output control

//Device Context
#define GxGfx_OSD 0
#define GxGfx_OSD2 1
#define GxGfx_VDO 2
#define GxGfx_VDO2 3
#define PC_SIM      DISABLE

//for iShow of UI_Show()
#define UI_SHOW_WINDOW      0 //OSD UI window
#define UI_SHOW_INFO        1 //OSD face detection frame
#define UI_SHOW_BACKGND     2 //VDO backgound
#define UI_SHOW_BACKGND_CK  3 //VDO backgound with ColorKey (by VDO_TRANSPARENT_COLOR)
#define UI_SHOW_PLAYBACK    4 //VDO playback
//#if (VDO_USE_PHOTO_SOURCE == ENABLE)
#define UI_SHOW_PREVIEW     5 //VDO preview
#define UI_SHOW_QUICKVIEW   6 //VDO quickview
#define UI_SHOW_PANL2RVIEW  7 //VDO guideview of pancapture (L2R)
#define UI_SHOW_PANR2LVIEW  8 //VDO guideview of pancapture (R2L)
#define UI_SHOW_PHOTOFRAME  9 //VDO Photo frame preview
#define UI_SHOW_PS_VIEW    10 //VDO PS previw
#define UI_SHOW_CS_VIEW    11 //VDO CS previw
//#endif

#define SHOWCMD_BEGINDRAW   1 //<<< UI_BeginScreen(),UI_BeginShow(): prepare buffer
#define SHOWCMD_SYNCDRAW    2 //<<< UI_EndScreen(),UI_EndShow(): sync timing for all layer
#define SHOWCMD_ENDDRAW     3 //<<< UI_EndScreen(),UI_EndShow(): apply effect
#define SHOWCMD_SETSIZE     4 //<<< UI_OnChangeSize(): change size by resize buffer or scaling-control

#define VDO_TRANSPARENT_COLOR   COLOR_YUV_BLACK


typedef struct _VIEW_TABLE {
	UINT32 Mode;
	UINT32 Layer;
	BOOL (*pfCommand)(UINT32 cmd, UINT32 param1, UINT32 param2);
}
VIEW_TABLE;

extern void UI_RegisterView(UINT32 nView, VIEW_TABLE *pViewList);

extern BOOL UI_IsShow(int iShow);
extern void UI_Show(int iShow, BOOL bEn);
extern void UI_OnChangeSize(UINT8 iDD, ISIZE *pSize);

extern void UI_SetDisplaySource(UINT8 iDD, UINT32 iSrc);
extern UINT32 UI_GetDisplaySource(UINT8 iDD);
extern void UI_DirtyDisplaySource(UINT8 iDD);

extern void UI_SetDisplayShow(UINT8 iDD, BOOL bEn);
extern BOOL UI_GetDisplayShow(UINT8 iDD);

extern void UI_SetDisplayPalette(UINT8 LayerID, UINT16 uiStartID, UINT16 uiCount, const PALETTE_ITEM *pTable);
extern void UI_DirtyDisplayPalette(UINT8 LayerID);

extern BOOL UI_IsShowDirty(void);

extern void UI_PrepareDisplaySource(void);
extern void UI_FlushDisplaySource(void);
extern void UI_FlushDisplayShow_Disable(void);
extern void UI_FlushDisplayShow_Enable(void);
extern void UI_FlushDisplayPalette(void);


#endif

