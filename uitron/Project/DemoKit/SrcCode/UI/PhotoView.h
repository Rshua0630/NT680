
#ifndef _PHOTODISPLAY_H
#define _PHOTODISPLAY_H

#include "Type.h"
#include "UIDisplay.h"

extern void View_Preview_SetDIS(BOOL bEn); //sub mode

extern BOOL View_Preview(UINT32 cmd, UINT32 param1, UINT32 param2);
extern BOOL View_Quickview(UINT32 cmd, UINT32 param1, UINT32 param2);
extern BOOL View_CS_Quickview(UINT32 cmd, UINT32 param1, UINT32 param2);
extern BOOL View_Guideview_L(UINT32 cmd, UINT32 param1, UINT32 param2);
extern BOOL View_Guideview_R(UINT32 cmd, UINT32 param1, UINT32 param2);
extern BOOL View_Photoframe(UINT32 cmd, UINT32 param1, UINT32 param2);


#endif // _PHOTODISPLAY_H
