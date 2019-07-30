
#include "Type.h"
#include "kernel.h"
#include "Utility.h" //for Perf API
#include "GxDisplay.h"
#include "UIDisplay.h"

#include "ide.h" //for ide_setV1En(), ide_setV2En()
#include "ide2.h" //for idec_setV1En(), idec_setV2En()
#include "PrjCfg.h"

///////////////////////////////////////////////////////////////////////////////
#define __MODULE__          UIShow
#define __DBGLVL__          2 // 0=FATAL, 1=ERR, 2=WRN, 3=UNIT, 4=FUNC, 5=IND, 6=MSG, 7=VALUE, 8=USER
#define __DBGFLT__          "*" //*=All, [mark]=CustomClass
#include "DebugModule.h"
///////////////////////////////////////////////////////////////////////////////

extern BOOL g_bDualUI;
extern BOOL g_DualVideo;

extern BOOL View_NullOSD1(UINT32 cmd, UINT32 param1, UINT32 param2);
extern BOOL View_NullOSD2(UINT32 cmd, UINT32 param1, UINT32 param2);
extern BOOL View_NullVDO1(UINT32 cmd, UINT32 param1, UINT32 param2);
extern BOOL View_NullVDO2(UINT32 cmd, UINT32 param1, UINT32 param2);

BOOL View_NullOSD1(UINT32 cmd, UINT32 param1, UINT32 param2)
{
	if (cmd == SHOWCMD_BEGINDRAW) {
		DBG_IND("^Gbegin OSD1 src=NULL\r\n");
		GxDisplay_SetSwapEffect(LAYER_OSD1, SWAPEFFECT_DISCARD); //disable double buffer
		if (g_bDualUI) {
			GxDisplay_SetSwapEffect(DOUT2 | LAYER_OSD1, SWAPEFFECT_DISCARD);    //disable double buffer
		}

		return TRUE;
	}
	if (cmd == SHOWCMD_ENDDRAW) {
		DBG_IND("^Gend OSD1 src=NULL\r\n");
		/*
		        GxDisplay_Set(LAYER_OUTPUT, CTRL_STATE_OSD1COMPARE, COMPARE_KEY_EQUAL); //enable OSD1 color key
		        GxDisplay_Set(LAYER_OUTPUT, CTRL_STATE_OSD1COLORKEY, 0); //set color key index
		        GxDisplay_Set(LAYER_OUTPUT, CTRL_STATE_OSD1ALPHA, OSD_OPAQUE_000); //set blending OSD1=0%
		        GxDisplay_Set(LAYER_OUTPUT, CTRL_STATE_OSD1BLEND, BLEND_DISABLE); //OSD1 blending src
		*/
		return TRUE;
	}
	if (cmd == SHOWCMD_SETSIZE) {
		//ISIZE *pSize = (ISIZE*)param2;
		//Display_SetWindow(LAYER_OSD1, 0, 0, pSize->w, pSize->h); //change window size
		return TRUE;
	}
	return FALSE;
}
BOOL View_NullOSD2(UINT32 cmd, UINT32 param1, UINT32 param2)
{
	if (cmd == SHOWCMD_BEGINDRAW) {
		DBG_IND("^Gbegin OSD2 src=NULL\r\n");
		GxDisplay_SetSwapEffect(LAYER_OSD2, SWAPEFFECT_DISCARD); //disable double buffer

		return TRUE;
	}
	if (cmd == SHOWCMD_ENDDRAW) {
		DBG_IND("^Gend OSD2 src=NULL\r\n");
		/*
		        GxDisplay_Set(LAYER_OUTPUT, CTRL_STATE_OSD2COMPARE, COMPARE_KEY_NEVER); //disable OSD2 color key
		        GxDisplay_Set(LAYER_OUTPUT, CTRL_STATE_OSD2COLORKEY, 0); //set color key index
		        GxDisplay_Set(LAYER_OUTPUT, CTRL_STATE_OSD2ALPHA, OSD_OPAQUE_000); //set blending OSD2=0%
		        GxDisplay_Set(LAYER_OUTPUT, CTRL_STATE_OSD2BLEND, BLEND_DISABLE); //OSD2 blending src
		*/
		return TRUE;
	}
	if (cmd == SHOWCMD_SETSIZE) {
		//ISIZE *pSize = (ISIZE*)param2;
		//Display_SetWindow(LAYER_OSD2, 0, 0, pSize->w, pSize->h); //change window size
		return TRUE;
	}
	return FALSE;
}
BOOL View_NullVDO1(UINT32 cmd, UINT32 param1, UINT32 param2)
{
	if (cmd == SHOWCMD_BEGINDRAW) {
		DBG_IND("^Gbegin VDO1 src=NULL\r\n");
		return TRUE;
	}
	if (cmd == SHOWCMD_ENDDRAW) {
		DBG_IND("^Gend VDO1 src=NULL\r\n");
		/*
		        GxDisplay_Set(LAYER_OUTPUT, CTRL_STATE_VDO1COMPARE, COMPARE_KEY_NEVER);
		        GxDisplay_Set(LAYER_OUTPUT, CTRL_STATE_VDO1COLORKEY, 0);
		        GxDisplay_Set(LAYER_OUTPUT, CTRL_STATE_VDO1ALPHA, OSD_OPAQUE_000);
		        GxDisplay_Set(LAYER_OUTPUT, CTRL_STATE_VDO1BLEND, BLEND_DISABLE);
		*/
		return TRUE;
	}
	if (cmd == SHOWCMD_SETSIZE) {
		//ISIZE *pSize = (ISIZE*)param2;
		//Display_SetWindow(LAYER_VDO1, 0, 0, pSize->w, pSize->h); //change window size
		return TRUE;
	}
	return FALSE;
}
BOOL View_NullVDO2(UINT32 cmd, UINT32 param1, UINT32 param2)
{
	if (cmd == SHOWCMD_BEGINDRAW) {
		DBG_IND("^Gbegin VDO2 src=NULL\r\n");
		GxDisplay_SetSwapEffect(LAYER_VDO2, SWAPEFFECT_DISCARD); //disable double buffer

		return TRUE;
	}
	if (cmd == SHOWCMD_ENDDRAW) {
		DBG_IND("^Gend VDO2 src=NULL\r\n");
		GxDisplay_Set(LAYER_OUTPUT, CTRL_STATE_VDO2COMPARE, COMPARE_KEY_NEVER);
		GxDisplay_Set(LAYER_OUTPUT, CTRL_STATE_VDO2COLORKEY, 0);
		GxDisplay_Set(LAYER_OUTPUT, CTRL_STATE_VDO2ALPHA, OSD_OPAQUE_100);
		GxDisplay_Set(LAYER_OUTPUT, CTRL_STATE_VDO2BLEND, BLEND_DISABLE);
		return TRUE;
	}
	if (cmd == SHOWCMD_SETSIZE) {
		//ISIZE *pSize = (ISIZE*)param2;
		//Display_SetWindow(LAYER_VDO2, 0, 0, pSize->w, pSize->h); //change window size
		return TRUE;
	}
	return FALSE;
}

VIEW_TABLE vNullOsd1 = {LAYER_OSD1, 0, View_NullOSD1};
VIEW_TABLE vNullOsd2 = {LAYER_OSD2, 0, View_NullOSD2};
VIEW_TABLE vNullVdo1 = {LAYER_VDO1, 0, View_NullVDO1};
VIEW_TABLE vNullVdo2 = {LAYER_VDO2, 0, View_NullVDO2};

///////////////////////////////////////////////////////////////////////////////

char layer_name[4][5] = {
	"OSD1",
	"OSD2",
	"VDO1",
	"VDO2",
};

static UINT32 g_nView = 0;
static VIEW_TABLE *g_pViewList = 0;

void UI_RegisterView(UINT32 nView, VIEW_TABLE *pViewList)
{
	g_nView = nView;
	g_pViewList = pViewList;
}

//show
static BOOL g_bShowDirty[4]  = {FALSE, FALSE, FALSE, FALSE};
static BOOL g_bLayerShow[4]  = {FALSE, FALSE, FALSE, FALSE};
//source
static BOOL g_bSourceDirty[4]  = {FALSE, FALSE, FALSE, FALSE};
static UINT32 g_bLayerSource[4]  = {0, 0, 0, 0};
#if 1
//palette
static BOOL g_bPaletteDirty[2] = {FALSE, FALSE};
static UINT16 g_uiPaletteStartID[2];
static UINT16 g_uiPaletteCount[2];
static const PALETTE_ITEM *g_pPaletteTable[2];
#endif

UINT32 UI_GetDisplaySource(UINT8 LayerID)
{
	return g_bLayerSource[LayerID];
}

//not set palette to HW directly,until UI_EndScreen to set palette ,for change color in current screen
void UI_SetDisplayPalette(UINT8 LayerID, UINT16 uiStartID, UINT16 uiCount, const PALETTE_ITEM *pTable)
{
#if 1
	g_bPaletteDirty[LayerID] = TRUE;
	g_uiPaletteStartID[LayerID] =  uiStartID;
	g_uiPaletteCount[LayerID] = uiCount;
	g_pPaletteTable[LayerID] = pTable;
#else
	GxDisplay_SetPalette(LayerID, uiStartID, uiCount, pTable);
#endif
}

void UI_DirtyDisplayPalette(UINT8 LayerID)
{
	if (g_uiPaletteCount[LayerID] > 0) {
		g_bPaletteDirty[LayerID] = TRUE;
		DBG_DUMP("^Cforce dirty palette\r\n");
	}
}

//NOTE: this function is NOT apply immediately
void UI_DirtyDisplaySource(UINT8 iDD)
{
	switch (iDD) {
	case LAYER_OSD1:
	case LAYER_OSD2:
	case LAYER_VDO1:
	case LAYER_VDO2:
		//if(g_bLayerSource[iDD] != iSrc)
		{
			g_bSourceDirty[iDD]  = TRUE;
			DBG_IND("^YUI_SRC[%s]=%08x (dirty)\r\n", layer_name[iDD], g_bLayerSource[iDD]);
		}
		break;
	}
}

//NOTE: this function is NOT apply immediately
void UI_SetDisplaySource(UINT8 iDD, UINT32 iSrc)
{
	switch (iDD) {
	case LAYER_OSD1:
	case LAYER_OSD2:
	case LAYER_VDO1:
	case LAYER_VDO2:
		//if(g_bLayerSource[iDD] != iSrc)
		{
			g_bLayerSource[iDD]  = iSrc;
			g_bSourceDirty[iDD]  = TRUE;
			DBG_IND("^YUI_SRC[%s]=%08x\r\n", layer_name[iDD], g_bLayerSource[iDD]);
		}
		break;
	}
}


//NOTE: this function is NOT apply immediately
void UI_SetDisplayShow(UINT8 iDD, BOOL bEn)
{
	switch (iDD) {
	case LAYER_OSD1:
	case LAYER_OSD2:
	case LAYER_VDO1:
	case LAYER_VDO2:
		//if(g_bLayerShow[iDD] != bEn)
		{
			g_bLayerShow[iDD] = bEn;
			g_bShowDirty[iDD] = TRUE;
			DBG_IND("^YUI_SHOW[%s]=%d\r\n", layer_name[iDD], g_bLayerShow[iDD]);
		}
		break;
	}
}




BOOL UI_IsShowDirty(void)
{
	int i;
	for (i = 0; i < 4; i++) {
		if (g_bSourceDirty[i] || g_bShowDirty[i]) {
			return TRUE;
		}
	}
#if 0
	for (i = 0; i < 2; i++) {
		if (g_bPaletteDirty[i]) {
			return TRUE;
		}
	}
#endif
	return FALSE;
}

BOOL UI_IsShowSourceDirty(void)
{
	int i;
	for (i = 0; i < 4; i++) {
		if (g_bSourceDirty[i]) {
			return TRUE;
		}
	}
	return FALSE;
}

// prepare source layer
void UI_PrepareDisplaySource(void)
{
	int i;
	for (i = 0; i < 4; i++) {
		if (g_bSourceDirty[i]) {
			VIEW_TABLE *src = (VIEW_TABLE *)UI_GetDisplaySource(i);
			DBG_IND("BEGIN SRC[%s]\r\n", layer_name[i]);
			if (src && src->pfCommand) {
				src->pfCommand(SHOWCMD_BEGINDRAW, 0, 0);
			}
			//#flush (do init layer) to prepare buffer for redraw
			GxDisplay_FlushEx(i, FALSE);
			//GxDisplay_Flush(i);
		}
	}
}

// apply source layer
void UI_FlushDisplaySource(void)
{
	int i;
	for (i = 0; i < 4; i++) {
		if (g_bSourceDirty[i]) {
			VIEW_TABLE *src = (VIEW_TABLE *)UI_GetDisplaySource(i);
			DBG_IND("END SRC[%s]\r\n", layer_name[i]);
			if (src && src->pfCommand) {
				src->pfCommand(SHOWCMD_ENDDRAW, 0, 0);
			}
			g_bSourceDirty[i] = FALSE;
		}
	}
}

// apply disable layer
void UI_FlushDisplayShow_Disable(void)
{
	int i;

	for (i = 0; i < 4; i++) {
		if (g_bShowDirty[i] && !g_bLayerShow[i]) {
			if (i == LAYER_VDO1) { //this DD is not initial by GxDisplay
				DBG_IND("EN[%s]=%d\r\n", layer_name[i], g_bLayerShow[i]);
#if (DISPLAY2_FUNC == ENABLE)
				if (g_DualVideo == DISPLAY_1) {
					idec_setV1En(IDE_ID_1, g_bLayerShow[i]);
				} else if (g_DualVideo == DISPLAY_2) {
					idec_setV1En(IDE_ID_2, g_bLayerShow[i]);
				} else if (g_DualVideo == (DISPLAY_1|DISPLAY_2)) {
					idec_setV1En(IDE_ID_1, g_bLayerShow[i]);
					idec_setV1En(IDE_ID_2, g_bLayerShow[i]);
				}
#else
				idec_setV1En(IDE_ID_1, g_bLayerShow[i]);
#endif
			} else if (i == LAYER_VDO2) { //this DD is not initial by GxDisplay
				DBG_IND("EN[%s]=%d\r\n", layer_name[i], g_bLayerShow[i]);
#if (DISPLAY2_FUNC == ENABLE)
				if (g_DualVideo == DISPLAY_1) {
					idec_setV2En(IDE_ID_1, g_bLayerShow[i]);
				} else if (g_DualVideo == DISPLAY_2) {
					idec_setV2En(IDE_ID_2, g_bLayerShow[i]);
				} else if (g_DualVideo == (DISPLAY_1|DISPLAY_2)) {
					idec_setV2En(IDE_ID_1, g_bLayerShow[i]);
					idec_setV2En(IDE_ID_2, g_bLayerShow[i]);
				}
#else
				idec_setV2En(IDE_ID_1, g_bLayerShow[i]);
#endif
			} else {
				DBG_IND("EN[%s]=%d\r\n", layer_name[i], g_bLayerShow[i]);
				GxDisplay_Set(i, LAYER_STATE_ENABLE, g_bLayerShow[i]);
#if (DISPLAY2_FUNC == ENABLE)
				if (g_bDualUI) {
					GxDisplay_Set(DOUT2 | i, LAYER_STATE_ENABLE, g_bLayerShow[i]);
				}
#endif
			}
			g_bShowDirty[i] = FALSE;
		}
	}

	for (i = 0; i < 4; i++) {
		VIEW_TABLE *src = (VIEW_TABLE *)UI_GetDisplaySource(i);
		DBG_IND("END SRC[%s]\r\n", layer_name[i]);
		if (src && src->pfCommand) {
			src->pfCommand(SHOWCMD_SYNCDRAW, 0, 0);
		}
	}

	//GxDisplay_FlushEx(LAYER_VDO1, FALSE);
	GxDisplay_FlushEx(LAYER_VDO2, FALSE);
	GxDisplay_FlushEx(LAYER_OSD1, FALSE);
	GxDisplay_FlushEx(LAYER_OSD2, FALSE);
#if (DISPLAY2_FUNC == ENABLE)
	if (g_bDualUI) {
		GxDisplay_FlushEx(DOUT2 | LAYER_OSD1, FALSE);
	}
#endif
}

// apply enable layer
void UI_FlushDisplayShow_Enable(void)
{
	BOOL bLoad = FALSE;
	int i;
	for (i = 0; i < 4; i++) {
		if (g_bShowDirty[i] && g_bLayerShow[i]) {
			if (i == LAYER_VDO1) { //this DD is not initial by GxDisplay
				DBG_IND("EN[%s]=%d\r\n", layer_name[i], g_bLayerShow[i]);
#if (DISPLAY2_FUNC == ENABLE)
				if (g_DualVideo == DISPLAY_1) {
					idec_setV1En(IDE_ID_1, g_bLayerShow[i]);
				} else if (g_DualVideo == DISPLAY_2) {
					idec_setV1En(IDE_ID_2, g_bLayerShow[i]);
				} else if (g_DualVideo == (DISPLAY_1|DISPLAY_2)) {
					idec_setV1En(IDE_ID_1, g_bLayerShow[i]);
					idec_setV1En(IDE_ID_2, g_bLayerShow[i]);
				}
#else
				idec_setV1En(IDE_ID_1, g_bLayerShow[i]);
#endif
			} else if (i == LAYER_VDO2) { //this DD is not initial by GxDisplay
				DBG_IND("EN[%s]=%d\r\n", layer_name[i], g_bLayerShow[i]);
#if (DISPLAY2_FUNC == ENABLE)
				if (g_DualVideo == DISPLAY_1) {
					idec_setV2En(IDE_ID_1, g_bLayerShow[i]);
				} else if (g_DualVideo == DISPLAY_2) {
					idec_setV2En(IDE_ID_2, g_bLayerShow[i]);
				} else if (g_DualVideo == (DISPLAY_1|DISPLAY_2)) {
					idec_setV2En(IDE_ID_1, g_bLayerShow[i]);
					idec_setV2En(IDE_ID_2, g_bLayerShow[i]);
				}
#else
				idec_setV2En(IDE_ID_1, g_bLayerShow[i]);
#endif
			} else {
				DBG_IND("EN[%s]=%d\r\n", layer_name[i], g_bLayerShow[i]);
				GxDisplay_Set(i, LAYER_STATE_ENABLE, g_bLayerShow[i]);
#if (DISPLAY2_FUNC == ENABLE)
				if (g_bDualUI) {
					GxDisplay_Set(DOUT2 | i, LAYER_STATE_ENABLE, g_bLayerShow[i]);
				}
#endif
			}
			g_bShowDirty[i] = FALSE;
			bLoad = TRUE;
		}
	}

	//GxDisplay_FlushEx(LAYER_VDO1, FALSE);
	GxDisplay_FlushEx(LAYER_VDO2, FALSE);
	GxDisplay_FlushEx(LAYER_OSD1, FALSE);
	GxDisplay_FlushEx(LAYER_OSD2, FALSE);
#if (DISPLAY2_FUNC == ENABLE)
	if (g_bDualUI) {
		GxDisplay_FlushEx(DOUT2 | LAYER_OSD1, FALSE);
	}
#endif

	if (bLoad) {
		//GxDisplay_WaitVD();
	}
}

// apply palette
void UI_FlushDisplayPalette(void)
{
#if 1
	int i;

//    GxDisplay_FlushEx(LAYER_OUTPUT, FALSE); //flush blending

	for (i = 0; i < 2; i++) {
		if (g_bPaletteDirty[i]) {
			DBG_IND("PAL[%s]=%08x, %d, %d\r\n",
					layer_name[i], g_pPaletteTable[i], g_uiPaletteStartID[i], g_uiPaletteCount[i]);
#if (DISPLAY_OSD_FMT == DISP_PXLFMT_INDEX8)
			GxDisplay_SetPalette
			(i, g_uiPaletteStartID[i], g_uiPaletteCount[i], g_pPaletteTable[i]);
			//if(g_bDualUI)
			GxDisplay_SetPalette
			(DOUT2 | i, g_uiPaletteStartID[i], g_uiPaletteCount[i], g_pPaletteTable[i]);
#endif
			g_bPaletteDirty[i] = FALSE;
		}
	}

#endif
}

/////////////////////////////////////////////////////////////////////////
//
//  Customize Display Control
//
/////////////////////////////////////////////////////////////////////////

//only for FlowMode_OnOpen(),FlowMode_OnClose()
BOOL UI_IsShow(int iShow)
{
	int i = 0;
	int find = -1;

	for (i = 0; (i < (int)g_nView) && (find < 0); i++) {
		if ((int)(g_pViewList[i].Mode) == iShow) {
			find = i;
		}
	}
	if (find == -1) {
		DBG_ERR("VIEW IS NOT FOUND!\r\n");
		return FALSE;
	}

	//TODO:
	//return ((iShow_OSD1 == iShow) && (bShow_OSD1 == TRUE));
	return TRUE;
}

extern UINT32 UICTRL_GFX_SEM_ID;

void UI_Show(int iShow, BOOL bEn)
{
	int i = 0;
	int find = -1;
	UINT32 layer = ((UINT32)(-1));
	wai_sem(UICTRL_GFX_SEM_ID);
	DBG_MSG("^YUI_Show(%d, %d)\r\n", iShow, bEn);

	for (i = 0; (i < (int)g_nView) && (find < 0); i++) {
		if ((int)(g_pViewList[i].Mode) == iShow) {
			find = i;
			layer = g_pViewList[i].Layer;
		}
	}
	if (find == -1) {
		DBG_ERR("VIEW IS NOT FOUND!\r\n");
		sig_sem(UICTRL_GFX_SEM_ID); //fix for CID 44968
		return;
	}

	if (bEn) {
		UI_SetDisplaySource(layer, (UINT32)(&(g_pViewList[find])));

		UI_SetDisplayShow(layer, TRUE);
	} else {
		//g_pViewList[find].Mode = FALSE;

		if (layer == LAYER_OSD1) {
			UI_SetDisplaySource(layer, (UINT32)&vNullOsd1);
		}
		if (layer == LAYER_OSD2) {
			UI_SetDisplaySource(layer, (UINT32)&vNullOsd2);
		}
		if (layer == LAYER_VDO1) {
			UI_SetDisplaySource(layer, (UINT32)&vNullVdo1);
		}
		if (layer == LAYER_VDO2) {
			UI_SetDisplaySource(layer, (UINT32)&vNullVdo2);
		}

		UI_SetDisplayShow(layer, FALSE);
	}

	sig_sem(UICTRL_GFX_SEM_ID);
}

///////////////////////////////////////////////////////////////////////////////

void UI_OnChangeSize(UINT8 iDD, ISIZE *pSize)
{
	//OSD1,OSD2,VDO2 change size policy is controlled by UI
	//VDO1 change size policy is controlled by each app
	int i;
	for (i = 0; i < 4; i++) {
		if (g_bLayerShow[i]) {
			VIEW_TABLE *src = (VIEW_TABLE *)UI_GetDisplaySource(i);
			DBG_IND("SETSIZE SRC[%s]\r\n", layer_name[i]);
			if (src && src->pfCommand) {
				src->pfCommand(SHOWCMD_SETSIZE, (UINT32)iDD, (UINT32)pSize);
			}
		}
	}
}

///////////////////////////////////////////////////////////////////////////////




