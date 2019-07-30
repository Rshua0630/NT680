
// UI Graphics的防止閃爍策略
//
//  1.配合UI Framework, 集中在BeginScreen()/EndScreen()中間, 進行畫面Buffer的Redraw 動作
//
//  2.配合UI Framework, 集中在EndScreen()的兩次WaitVD中間, 進行GxDisplay的Setup 動作
//

#include "Type.h"
#include "kernel.h"
#include "Debug.h"
#include "Utility.h"
#include "UIFramework.h"
#include "UIFrameworkExt.h"
//#include "GxImageJPG.h"
#include "GxDisplay.h"
#include "UIDisplay.h"
#include "UIGraphics.h"
#include "SysCfg.h"
#if (OSD_USE_ROTATE_BUFFER == ENABLE)
#include "UIView.h"
#endif

#define FASTDRAW_FUNCTION   DISABLE //ENABLE, enable for fast boot
///////////////////////////////////////////////////////////////////////////////
#define __MODULE__          UIDisp
#define __DBGLVL__          2 // 0=FATAL, 1=ERR, 2=WRN, 3=UNIT, 4=FUNC, 5=IND, 6=MSG, 7=VALUE, 8=USER
#define __DBGFLT__          "*" //*=All, [mark]=CustomClass
#include "DebugModule.h"
///////////////////////////////////////////////////////////////////////////////

#define BENCH_GFX   DISABLE

extern BOOL g_bDualUI;

UINT32 gDrawSemID = 0;

UINT32 layout_w = 0;
UINT32 layout_h = 0;

DC *_sys_pDC[4];
IPOINT _sys_Orig[4];
typedef DC **DCLIST;

#if (BENCH_GFX == ENABLE)
UINT32 A1 = 0, A2 = 0, A3 = 0, A4 = 0;
UINT32 B1 = 0, B2 = 0, B3 = 0, B4 = 0;
#endif


/////////////////////////////////////////////////////////////////////////
//
//  Draw Dirty
//
/////////////////////////////////////////////////////////////////////////
#define DIRTY_MASK      0x00ff
VControl *pScreenWnd = 0;
BOOL UI_IsDrawDirty(void);

BOOL UI_IsDrawDirty(void)
{
	Ux_GetFocusedWindow(&pScreenWnd);
	if ((pScreenWnd) && (pScreenWnd->flag & DIRTY_MASK)) {
		return TRUE;
	}
	return FALSE;
}



/////////////////////////////////////////////////////////////////////////
//
//  Flip Control
//
/////////////////////////////////////////////////////////////////////////
static BOOL g_bFlip = TRUE;
void UI_SetDisplayFlip(BOOL bEn);
BOOL UI_GetDisplayFlip(void);

BOOL UI_GetDisplayFlip(void)
{
	return g_bFlip;
}
//set next redraw not change buffer
//after next redraw ,bNotFlip would be set true
void UI_SetDisplayFlip(BOOL bEn)
{
	g_bFlip = bEn;
}

/////////////////////////////////////////////////////////////////////////
//
//  Dirty Control
//
/////////////////////////////////////////////////////////////////////////
BOOL bScreenDirty = FALSE;
void UI_SetDisplayDirty(BOOL bEn);

//set force dirty (even do not modify show or draw)
void UI_SetDisplayDirty(BOOL bEn)
{
	bScreenDirty = bEn;
}


/////////////////////////////////////////////////////////////////////////
//
//  Customize Display Flow
//
/////////////////////////////////////////////////////////////////////////

void UI_BeginShow(void)
{
#if (BENCH_GFX == ENABLE)
	A1 = Perf_GetCurrent() / 1000;

	A2 = Perf_GetCurrent() / 1000;
#endif
	DBG_IND("^YDRAW-BEGIN\r\n");

	//#prepare source layer
	UI_PrepareDisplaySource();

#if (BENCH_GFX == ENABLE)
	A3 = Perf_GetCurrent() / 1000;

	A4 = Perf_GetCurrent() / 1000;
#endif
}

void UI_EndShow(void)
{
#if (BENCH_GFX == ENABLE)
	B1 = Perf_GetCurrent() / 1000;

	B2 = Perf_GetCurrent() / 1000;
#endif

///////////////////////////////////////////////////////////////////////
//>>>>> UI Graphics的防止閃爍策略
//>>>>> 配合UI Framework, 集中在此時進行GxDisplay的Setup 動作  - begin
	GxDisplay_WaitVD();

	//apply disable layer
	UI_FlushDisplayShow_Disable();
	//apply source layer
	UI_FlushDisplaySource();
	//apply blending
	GxDisplay_FlushEx(LAYER_OUTPUT, FALSE); //flush blending
	//swap buffer
	GxDisplay_Flip(FALSE);
	if (g_bDualUI) {
		GxDisplay_ForceFlip(DOUT2 | LAYER_OSD1);
		GxDisplay_FlipEx(DOUT2 | LAYER_ALL, FALSE);
	}

#if (BENCH_GFX == ENABLE)
	B3 = Perf_GetCurrent() / 1000;
#endif

	//apply palette
	UI_FlushDisplayPalette();
	//apply enable layer
	UI_FlushDisplayShow_Enable();

	GxDisplay_WaitVD();
//>>>>> 配合UI Framework, 集中在此時進行GxDisplay的Setup 動作  - end
///////////////////////////////////////////////////////////////////////

	DBG_IND("^YDRAW-END\r\n");

#if (BENCH_GFX == ENABLE)
	B4 = Perf_GetCurrent() / 1000;
	DBG_MSG("^Y UI Draw cost = %d, %d, %d, %d, %d, %d, %d ms\r\n", A2 - A1, A3 - A2, A4 - A3, B1 - A4, B2 - B1, B3 - B2, B4 - B3);
#endif
}

void UI_SetLayoutSize(UINT32 Width, UINT32 Height)
{
	layout_w = Width;
	layout_h = Height;
}

UIScreen UI_BeginScreen(void)
{
#if (BENCH_GFX == ENABLE)
	A1 = Perf_GetCurrent() / 1000;
#endif
	if (UICTRL_GFX_SEM_ID) {
		wai_sem(UICTRL_GFX_SEM_ID);
	}

#if (FASTDRAW_FUNCTION == ENABLE)
	if (UI_IsDrawDirty() || UI_IsShowDirty()) {
		bScreenDirty = TRUE;
	}
#endif

#if (BENCH_GFX == ENABLE)
	A2 = Perf_GetCurrent() / 1000;
#endif


#if (FASTDRAW_FUNCTION == ENABLE)
	if (!bScreenDirty) {
		//Skip
		return (UIScreen)(DCLIST)(_sys_pDC);
	}
#endif

	DBG_IND("^YUI_BeginScreen\r\n");

	//prepare source layer
	UI_PrepareDisplaySource();

	DBG_IND("^YDRAW-BEGIN\r\n");
#if (BENCH_GFX == ENABLE)
	A3 = Perf_GetCurrent() / 1000;
#endif
	_sys_pDC[0] = GxDisplay_BeginDraw(LAYER_OSD1);
#if (OSD_USE_ROTATE_BUFFER == ENABLE)
	_sys_pDC[0] = View_Window_BeginRotate(_sys_pDC[0]); //copy show to paint with rotate
#endif

#if(OSD2_FUNC == ENABLE)
	_sys_pDC[1] = GxDisplay_BeginDraw(LAYER_OSD2);
#endif
	// _sys_pDC[2] = GxDisplay_BeginDraw(LAYER_VDO1);
	_sys_pDC[3] = GxDisplay_BeginDraw(LAYER_VDO2);
	_sys_Orig[0] = GxGfx_GetOrigin(_sys_pDC[0]);
	_sys_Orig[1] = GxGfx_GetOrigin(_sys_pDC[1]);
	// _sys_Orig[2] = GxGfx_GetOrigin(_sys_pDC[2]);
	_sys_Orig[3] = GxGfx_GetOrigin(_sys_pDC[3]);

	//modify coordinate from 640 x 240 to 640 x 480
	GxGfx_SetCoord(_sys_pDC[0], layout_w, layout_h);
	GxGfx_SetWindow(_sys_pDC[0], 0, 0, layout_w - 1, layout_h - 1);
	GxGfx_SetCoord(_sys_pDC[1], layout_w, layout_h);
	GxGfx_SetWindow(_sys_pDC[1], 0, 0, layout_w - 1, layout_h - 1);
	//GxGfx_SetCoord(_sys_pDC[2], layout_w1, layout_h);
	//GxGfx_SetWindow(_sys_pDC[2], 0, 0, layout_w-1, layout_h-1);
	GxGfx_SetCoord(_sys_pDC[3], layout_w, layout_h);
	GxGfx_SetWindow(_sys_pDC[3], 0, 0, layout_w - 1, layout_h - 1);

#if (BENCH_GFX == ENABLE)
	A4 = Perf_GetCurrent() / 1000;
#endif
	return (UIScreen)(DCLIST)(_sys_pDC);

///////////////////////////////////////////////////////////////////////
//>>>>> UI Graphics的防止閃爍策略
//>>>>> 配合UI Framework, 集中在此時進行畫面Buffer的Redraw 動作 - begin
}

void UI_EndScreen(UIScreen ScreenObj)
{
///////////////////////////////////////////////////////////////////////
//>>>>> UI Graphics的防止閃爍策略
//>>>>> 配合UI Framework, 集中在此時進行畫面Buffer的Redraw 動作 - end

#if (FASTDRAW_FUNCTION == ENABLE)
	if (!bScreenDirty) {
		DBG_MSG("^Y UI Draw skip!\r\n");
		if (UICTRL_GFX_SEM_ID) {
			sig_sem(UICTRL_GFX_SEM_ID);
		}
		return; //Skip
	}
#endif

#if (FASTDRAW_FUNCTION == ENABLE)
	bScreenDirty = FALSE; //clear
#endif

#if (BENCH_GFX == ENABLE)
	B1 = Perf_GetCurrent() / 1000;
#endif

	GxGfx_SetOrigin(_sys_pDC[0], _sys_Orig[0].x, _sys_Orig[0].y);
#if (OSD_USE_ROTATE_BUFFER == ENABLE)
	View_Window_EndRotate(); //copy paint to show with rotate
#endif
	GxDisplay_EndDraw(LAYER_OSD1, _sys_pDC[0]);

#if(OSD2_FUNC == ENABLE)
	GxGfx_SetOrigin(_sys_pDC[1], _sys_Orig[1].x, _sys_Orig[1].y);
	GxDisplay_EndDraw(LAYER_OSD2, _sys_pDC[1]);
#endif
#if 0
	GxGfx_SetOrigin(_sys_pDC[2], _sys_Orig[2].x, _sys_Orig[2].y);
	GxDisplay_EndDraw(LAYER_VDO1, _sys_pDC[2]);
#endif
	GxGfx_SetOrigin(_sys_pDC[3], _sys_Orig[3].x, _sys_Orig[3].y);
	GxDisplay_EndDraw(LAYER_VDO2, _sys_pDC[3]);

	DBG_IND("^YDRAW-END\r\n");
	if (!UI_GetDisplayFlip()) {
		//skip this flip
		UI_SetDisplayFlip(TRUE); //continue to flip (next time)
		DBG_IND("^YIGNORE FLIP!!!\r\n");

		DBG_IND("^YDRAW-END\r\n");
		if (UICTRL_GFX_SEM_ID) {
			sig_sem(UICTRL_GFX_SEM_ID);
		}
		return;
	}

#if (BENCH_GFX == ENABLE)
	B2 = Perf_GetCurrent() / 1000;
#endif

///////////////////////////////////////////////////////////////////////
//>>>>> UI Graphics的防止閃爍策略
//>>>>> 配合UI Framework, 集中在此時進行GxDisplay的Setup 動作  - begin
#if 1 //add for 650 GxDisplay
	//apply palette
	UI_FlushDisplayPalette(); //must set palette before flush
#endif
	GxDisplay_WaitVD();

	//apply disable layer
	UI_FlushDisplayShow_Disable();
	//apply source layer
	UI_FlushDisplaySource();
	//apply blending
	GxDisplay_FlushEx(LAYER_OUTPUT, FALSE); //flush palette & blending
	if (g_bDualUI) {
		GxDisplay_FlushEx(DOUT2 | LAYER_OUTPUT, FALSE); //flush palette & blending
	}
	//swap buffer
	GxDisplay_Flip(FALSE); //buffer flip
	if (g_bDualUI) {
		GxDisplay_ForceFlip(DOUT2 | LAYER_OSD1);
		GxDisplay_FlipEx(DOUT2 | LAYER_ALL, FALSE);
	}

#if (BENCH_GFX == ENABLE)
	B3 = Perf_GetCurrent() / 1000;
#endif

#if 0 //remove for 650 GxDisplay
	//apply palette
	UI_FlushDisplayPalette();
#endif
	//apply enable layer
	UI_FlushDisplayShow_Enable(); //to avoid see garbage, must enable after buffer flip

	GxDisplay_WaitVD();
//>>>>> UI Graphics的防止閃爍策略
//>>>>> 配合UI Framework, 集中在此時進行GxDisplay的Setup 動作  - end
///////////////////////////////////////////////////////////////////////

	DBG_IND("^YUI_EndScreen\r\n");
	if (UICTRL_GFX_SEM_ID) {
		sig_sem(UICTRL_GFX_SEM_ID);
	}

#if (BENCH_GFX == ENABLE)
	B4 = Perf_GetCurrent() / 1000;
	DBG_MSG("^Y UI Draw cost = %d, %d, %d, %d, %d, %d, %d ms\r\n", A2 - A1, A3 - A2, A4 - A3, B1 - A4, B2 - B1, B3 - B2, B4 - B3);
#endif
}

void UI_BeginWindow(VControl *pWnd, UIScreen ScreenObj)
{
	DC **pDCList = (DC **)ScreenObj;
	//set relative position by Window
	GxGfx_SetOrigin(pDCList[GxGfx_OSD],
					pWnd->rect.x1,
					pWnd->rect.y1);

	GxGfx_SetOrigin(pDCList[GxGfx_OSD2],
					pWnd->rect.x1,
					pWnd->rect.y1);

	GxGfx_SetOrigin(pDCList[GxGfx_VDO2],
					pWnd->rect.x1,
					pWnd->rect.y1);
}

void UI_EndWindow(VControl *pWnd, UIScreen ScreenObj)
{
}

void UI_BeginControl(VControl *pParent, VControl *pControl, UIScreen ScreenObj)
{
	DC **pDCList = (DC **)ScreenObj;
	Ux_RECT  rect = {0, 0, 0, 0};
	rect.x1 =  pControl->rect.x1;
	rect.y1 =  pControl->rect.y1;

	while (pParent) {
		rect.x1 += pParent->rect.x1;
		rect.y1 += pParent->rect.y1;
		pParent = pParent->pParent;
	}

	GxGfx_SetOrigin(pDCList[GxGfx_OSD], rect.x1, rect.y1);

	GxGfx_SetOrigin(pDCList[GxGfx_OSD2], rect.x1, rect.y1);

	GxGfx_SetOrigin(pDCList[GxGfx_VDO2], rect.x1, rect.y1);

}

void UI_EndControl(VControl *pCtrl, VControl *pControl, UIScreen ScreenObj)
{
}

void UI_ClearScreen(UIScreen ScreenObj)
{
	DC **pDCList = (DC **)ScreenObj;

	DBG_MSG("^YClear OSD1\r\n");
	GxGfx_Clear(pDCList[GxGfx_OSD]);
	DBG_MSG("^YClear OSD2\r\n");
	GxGfx_Clear(pDCList[GxGfx_OSD2]);
}

UIRender demoRender = {
	UI_BeginScreen,
	UI_EndScreen,
	UI_BeginWindow,
	UI_EndWindow,
	UI_BeginControl,
	UI_EndControl,
	UI_ClearScreen
};


/////////////////////////////////////////////////////////////////////////////
void UI_CleanDisplay(BOOL bFast)
{
	UINT32 uiBufSize = 0;
	UINT32 uiBufAddr = 0;
	static BOOL bFastClean = TRUE;
	UI_GetOSDBuf(&uiBufAddr, &uiBufSize);

	if (bFastClean)
		//fast path to clear 4 OSD buffers (one pass)
		// - OSD1,OSDTEMP,OSD2,OSD2TEMP must be continuous
	{
		DBG_MSG("^YFastClear OSD1/OSD1TEMP\r\n");
		DBG_MSG("^YFastClear OSD2/OSD1TEMP\r\n");
		// for USB charge dead battery or no battery power on
		if (bFast) {
			memset((void *)uiBufAddr, 0x00000000, uiBufSize);
		} else {
			hwmem_open();
			hwmem_memset32(uiBufAddr, 0x00000000, uiBufSize);
			hwmem_close();
		}
	}
//#if (WAITPHOTO_FUNCTION == ENABLE)
	//skip control VDO2
//#else
//    UI_Show(UI_SHOW_BACKGND, FALSE); //VDO2
//#endif
	//UI_Show(UI_SHOW_PREVIEW, TRUE); //VDO1
	#if(OSD2_FUNC)
	UI_Show(UI_SHOW_INFO, FALSE); //OSD2
	#endif
	UI_Show(UI_SHOW_WINDOW, TRUE);  //OSD1 (first time UI_BeginDisplay() is here! also for GxGfx_Clear())
	if (!bFastClean) {
		DC     **pDCList;
		UI_SetDisplayDirty(TRUE); //set TRUE to force dirty current begin/end

		pDCList = (DC **)UI_BeginScreen();
		DBG_MSG("^YClear OSD1\r\n");
		GxGfx_Clear(pDCList[GxGfx_OSD]);
		DBG_MSG("^YClear OSD2\r\n");
		GxGfx_Clear(pDCList[GxGfx_OSD2]);

		UI_EndScreen((UINT32)pDCList); //(first time flip is here!)
	}
	bFastClean = FALSE;
}

void UI_ResetDisplay(void)
{
	DBG_IND("UI_ResetDisplay\n\r");

	// Disable IDE video1/video2 output
	#if(OSD2_FUNC)
	UI_Show(UI_SHOW_INFO, FALSE); //OSD2
	#endif
	UI_Show(UI_SHOW_WINDOW, FALSE);  //OSD1
	UI_Show(UI_SHOW_BACKGND, FALSE); //VDO2
	UI_Show(UI_SHOW_PREVIEW, FALSE); //VDO1
	{
		DC     **pDCList;
		UI_SetDisplayDirty(TRUE); //set TRUE to force dirty current begin/end

		pDCList = (DC **)UI_BeginScreen();

		DBG_MSG("^YClear OSD1\r\n");
		GxGfx_Clear(pDCList[GxGfx_OSD]);
		DBG_MSG("^YClear OSD2\r\n");
		GxGfx_Clear(pDCList[GxGfx_OSD2]);

		UI_EndScreen((UINT32)pDCList);
	}
	UI_Show(UI_SHOW_WINDOW, TRUE);  //OSD1
}

//!!! This Function is only for engineer debug, because it will cause OSD broken. !!!//
void UI_ForceShowString(CHAR *pStr, PURECT pRect, BOOL bClear)
{
	DC     **pCalDCList;
	UI_SetDisplayDirty(TRUE); //set TRUE to force dirty current begin/end

	pCalDCList = (DC **)UI_BeginScreen();
	if (bClear) {
		GxGfx_SetTextColor(_OSD_INDEX_TRANSPART, _OSD_INDEX_TRANSPART, 0);
		GxGfx_FillRect(pCalDCList[GxGfx_OSD], pRect->x, pRect->y, pRect->x + pRect->w, pRect->y + pRect->h);
	}
	GxGfx_SetShapeColor(_OSD_INDEX_TRANSPART, _OSD_INDEX_TRANSPART, 0);
	GxGfx_SetTextStroke((const FONT *)gDemoKit_Font, FONTSTYLE_NORMAL, SCALE_1X);
	GxGfx_SetTextColor(_OSD_INDEX_WHITE, _OSD_INDEX_GRAY, 0);
	GxGfx_Text(pCalDCList[GxGfx_OSD], pRect->x, pRect->y, pStr);
	UI_EndScreen((UINT32)pCalDCList);
}
// TEST!!! Only for engineer mode!
void UI_ClearOSD(UINT32 uiColorIdx) // fill OSD1 with color
{
	DC **pDCList;

	pDCList = (DC **)UI_BeginScreen();
	GxGfx_SetShapeColor(0, uiColorIdx, 0);
	GxGfx_FillRect(pDCList[GxGfx_OSD], 0, 0, pDCList[GxGfx_OSD]->size.w, pDCList[GxGfx_OSD]->size.h);
	UI_EndScreen((UINT32)pDCList);
}

// TEST!!! Only for engineer mode!
void UI_DrawOsdString(char *str, UINT32 x, UINT32 y, UINT32 uiColor, BOOL bClear)
{
	DC **pDCList;
	UINT32 uiWidth;

	pDCList = (DC **)UI_BeginScreen();
	uiWidth = strlen(str) * 12;
	if (uiWidth > OSD_W) {
		uiWidth = OSD_W;
	}

	if (bClear) {
		// Clear before draw (clear whole area of OSD width with font height)
		GxGfx_SetShapeColor(CLRID_IDX_TRANSPART, CLRID_IDX_TRANSPART, 0);
		//GxGfx_FillRect(pDCList[GxGfx_OSD], 0, y, OSD_W, y+24);
		GxGfx_FillRect(pDCList[GxGfx_OSD], 0, y, uiWidth, y + 26);
	}

	//GxGfx_SetTextColor(uiColor, CLRID_IDX_TRANSPART, 0);
	GxGfx_SetTextColor(uiColor, CLRID_IDX_BLACK, 0);
	GxGfx_Text(pDCList[GxGfx_OSD], x, y, str);
	UI_EndScreen((UINT32)pDCList);
}


