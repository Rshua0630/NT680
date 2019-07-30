#ifndef _UIVIEWSLIDE_H_
#define _UIVIEWSLIDE_H_

#include "GxGfx.h"

typedef struct _UIPlay_SlideView {
	UINT32 scr_w;
	UINT32 scr_h;
	UINT32 x;
	UINT32 y;
	UINT32 w;
	UINT32 h;
	IMAGE *pMyImg1;
	IMAGE *pMyImg2;
}
UIPlay_SlideView;

extern void UISlide_OpenView(UIPlay_SlideView *pViewCfg, BOOL bOpenFromPB);
extern void UISlide_CloseView(BOOL bReturnToPB);

#endif //_UIVIEWSLIDE_H_

