#include "SysCommon.h"
#if IPCAM_FD_DRAW_VIDEO
#include "fd_lib.h"
#endif

#if IPCAM_BC_FUNC
#include "bc_lib.h"
#endif

void VdoOut_DrawRect(PIMG_BUF pDstImg, IRECT  *DrawRect, UINT32 drawColor, UINT32 border)
{
	IRECT  LineRect;

	if (border < 2) {
		border = 2;
	}

	LineRect.x = DrawRect->x;
	LineRect.y = DrawRect->y;
	LineRect.w = DrawRect->w;
	LineRect.h = border;
	GxImg_FillData(pDstImg, &LineRect, drawColor);

	LineRect.x = DrawRect->x;
	LineRect.y = DrawRect->y + DrawRect->h - border;
	LineRect.w = DrawRect->w;
	LineRect.h = border;
	GxImg_FillData(pDstImg, &LineRect, drawColor);

	LineRect.x = DrawRect->x;
	LineRect.y = DrawRect->y;
	LineRect.w = border;
	LineRect.h = DrawRect->h;
	GxImg_FillData(pDstImg, &LineRect, drawColor);

	LineRect.x = DrawRect->x + DrawRect->w - border;
	LineRect.y = DrawRect->y;
	LineRect.w = border;
	LineRect.h = DrawRect->h;
	GxImg_FillData(pDstImg, &LineRect, drawColor);
}

#if (IPCAM_FD_DRAW_VIDEO == ENABLE)
static void VdoOut_DrawFD(PIMG_BUF pDstImg)
{
	UINT32            Fdnumber, i;
	static FD_FACE    FdInfo[MAX_FDNUM];
	IRECT             DrawRect;
	UINT32            border = 4; // border can not smaller than 2
	UINT32            drawColor = COLOR_YUV_WHITE;
	URECT             DispCord;

	if (pDstImg->Width < 1280) {
		border = 2;
	}
	DispCord.x = 0;
	DispCord.y = 0;
	DispCord.w = pDstImg->Width;
	DispCord.h = pDstImg->Height;
	Fdnumber = FD_GetRsltFace(FdInfo, &DispCord);
	if (Fdnumber > MAX_FDNUM) {
		Fdnumber = MAX_FDNUM;
	}
	if (Fdnumber) {
		for (i = 0; i < Fdnumber; i++) {
			DrawRect.x = FdInfo[i].x;
			DrawRect.y = FdInfo[i].y;
			DrawRect.w = FdInfo[i].w;
			DrawRect.h = FdInfo[i].h;
			//MovieStamp_DrawRectUV(&DrawRect, drawColor, pDstImg);
			VdoOut_DrawRect(pDstImg, &DrawRect, drawColor, border);
		}
	}

}
#endif

#if (IPCAM_BC_FUNC == ENABLE)
static void VdoOut_DrawBC(PIMG_BUF pDstImg){

	/*
	static URECT	  target[1024];
	UINT32            i, number, alarm;
	IRECT             drawrect;
	UINT32            border = 4; // border can not smaller than 2
	UINT32            drawcolor = COLOR_YUV_WHITE;
	URECT             dispcord;

	if (pDstImg->Width < 1280) {
		border = 2;
	}
	dispcord.x = 0;
	dispcord.y = 0;
	dispcord.w = pDstImg->Width;
	dispcord.h = pDstImg->Height;
	number = bc_gettarget(target, &dispcord, 256);
	//number = bc_getblktarget(target, &dispcord, 1024);
	//number = 0;
	alarm = bc_gettdprms(BC_TD_STATUS);
	if(alarm > 0)
	{
		drawcolor = COLOR_YUV_RED;
	}

	if (number) {
		for (i = 0; i < number; i++) {
			drawrect.x = (INT32)target[i].x;
			drawrect.y = (INT32)target[i].y;
			drawrect.w = (INT32)target[i].w;
			drawrect.h = (INT32)target[i].h;
			VdoOut_DrawRect(pDstImg, &drawrect, drawcolor, border);
		}
	}
	*/
}
#endif

void alg_VdoOut_Draw(PIMG_BUF pDstImg)
{
    #if (IPCAM_FD_DRAW_VIDEO == ENABLE)
	VdoOut_DrawFD(pDstImg);
	#endif
	#if (IPCAM_BC_FUNC == ENABLE)
	VdoOut_DrawBC((IMG_BUF *)pDstImg);
	#endif
}
