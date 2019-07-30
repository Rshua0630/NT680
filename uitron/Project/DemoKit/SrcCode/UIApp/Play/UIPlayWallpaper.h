#ifndef _UIVIEWWALLPAPER_H_
#define _UIVIEWWALLPAPER_H_

#include "GxGfx.h"

extern void UIWallpaper_OpenView(BOOL bOpenFromPB);
extern void UIWallpaper_CloseView(BOOL bReturnToPB);

extern UINT32 UIWallpaper_PrepareUserWP(UINT32 addr, UINT32 size);

#endif //_UIVIEWWALLPAPER_H_

