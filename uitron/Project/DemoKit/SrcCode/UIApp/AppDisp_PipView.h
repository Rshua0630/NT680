#ifndef _PIPVIEW_H
#define _PIPVIEW_H

#include "AppDisp_View.h"

extern void               PipView_SetStyle(UINT32 uiStyle);
extern INT32              PipView_GetInfo(APPDISP_VIEW_INFO *info);
extern INT32              PipView_OnDraw(APPDISP_VIEW_DRAW *pDraw);

#endif
