#ifndef _UI_GFX_H_
#define _UI_GFX_H_
#include "PrjCfg.h"
#if(IPCAM_FUNC != ENABLE)
#include "UIResource.h"
#endif
#include "UIFramework.h"
#include "UIFrameworkExt.h"

//UIGfx
extern void UI_InstallID(void) _SECTION(".kercfg_text");
extern UINT32 _SECTION(".kercfg_data") UICTRL_GFX_SEM_ID; ///< sem id

extern void UI_GfxInit(void);
extern void UI_DispInit(void);
extern void UI_ControlShowInit(void);

extern void UI_SetLayoutSize(UINT32 Width, UINT32 Height);
extern void UI_CleanDisplay(BOOL bFast);
extern void UI_ResetDisplay(void);
extern void UI_InitDisplay(UINT8 iDD, BOOL bClear);

extern UINT32 UI_BeginScreen(void);
extern void UI_EndScreen(UINT32 ScreenObj);
extern void UI_BeginShow(void);
extern void UI_EndShow(void);
extern void UI_ClearScreen(UIScreen ScreenObj);
//extern void UI_BeginWindow(VControl* pWnd, UIScreen ScreenObj);
//extern void UI_EndWindow(VControl* pWnd, UIScreen ScreenObj);
//extern void UI_BeginControl(VControl* pParent, VControl* pControl, UIScreen ScreenObj);
//extern void UI_EndControl(VControl* pCtrl, VControl* pControl, UIScreen ScreenObj);

extern UIRender demoRender;

extern void UI_SetDisplayDirty(BOOL bEn); //set TRUE to force dirty current begin/end
extern void UI_SetDisplayFlip(BOOL bEn); //set FALSE to skip current flip

// TEST!!! Only for engineer mode!
extern void UI_ClearOSD(UINT32 uiColorIdx); // fill OSD1 with color
extern void UI_DrawOsdString(char *str, UINT32 x, UINT32 y, UINT32 uiColor, BOOL bClear);

#endif //_UI_GFX_H_
