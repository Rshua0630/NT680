#ifndef _APPDISP_VIEW_H
#define _APPDISP_VIEW_H

#include "GxImage.h"

typedef struct {
    PIMG_BUF                p_src_img[4];
	PIMG_BUF                p_dst_img;
    UINT32                  viewcnt;
} APPDISP_VIEW_DRAW;

typedef struct {
	UINT32                  w;
	UINT32                  h;
} APPDISP_VIEW_SCALE;

typedef struct {
	APPDISP_VIEW_SCALE      scale[4];
} APPDISP_VIEW_INFO;


typedef void  APPDISP_VIEW_SETSTYLE_CB(UINT32 uiStyle);
typedef INT32 APPDISP_VIEW_GETINFO_CB(APPDISP_VIEW_INFO *info);
typedef INT32 APPDISP_VIEW_DRAW_CB(APPDISP_VIEW_DRAW *pDraw);

typedef struct {
	APPDISP_VIEW_SETSTYLE_CB         *SetStyle;
	APPDISP_VIEW_GETINFO_CB          *GetInfo;
	APPDISP_VIEW_DRAW_CB             *OnDraw;
} APPDISP_VIEW_OPS;


extern void            AppDispView_Init(void);
extern void            AppDispView_SetStyle(UINT32 uiStyle);
extern INT32           AppDispView_OnDraw(APPDISP_VIEW_DRAW *pDraw);
extern INT32           AppDispView_GetInfo(APPDISP_VIEW_INFO* info);

#endif
