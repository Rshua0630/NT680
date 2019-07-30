#ifndef _SBSVIEW_H
#define _SBSVIEW_H
#include "AppDisp_View.h"

extern void  SbsView_SetStyle(UINT32 uiStyle);
extern INT32 SbsView_GetInfo(APPDISP_VIEW_INFO *info);
extern INT32 SbsView_OnDraw(APPDISP_VIEW_DRAW *pDraw);

#endif
