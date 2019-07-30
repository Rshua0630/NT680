#include "Debug.h"
#include "PrjCfg.h"
#include "SysCfg.h"
#include "GxRect.h"
#include "GxDisplay.h"
#include "UIDisplay.h"
#include "GxVideo.h"
//#NT#2014/03/25#KS Hung -begin
#if (OSD_USE_ROTATE_BUFFER == ENABLE)
#include "UIView.h"
#endif
//#NT#2014/03/25#KS Hung -end
//#NT#2016/10/14#KCHong -begin
//#NT#Forced use 320x240 UI for engineer mode
#if (CALIBRATION_FUNC == ENABLE)
#include "EngineerMode.h"
#endif
//#NT#2016/10/14#KCHong -end

///////////////////////////////////////////////////////////////////////////////
#define __MODULE__          UIDisp
#define __DBGLVL__          2 // 0=FATAL, 1=ERR, 2=WRN, 3=UNIT, 4=FUNC, 5=IND, 6=MSG, 7=VALUE, 8=USER
#define __DBGFLT__          "*" //*=All, [mark]=CustomClass
#include "DebugModule.h"
///////////////////////////////////////////////////////////////////////////////
#if (OSD_USE_ROTATE_BUFFER == ENABLE)
static UINT32 gbOsdRotate[2] = {0};  // 0:DOUT1,1:DOUT2
static DC LocalDC;
static DC *pPaintDC = &LocalDC;
static DC *pShowDC = 0;
#endif
#if (DISPLAY2_FUNC == ENABLE)
extern BOOL g_bDualUI;
extern BOOL g_bDualUI_dev2_osd_mask;
#endif

#if (OSD_USE_ROTATE_BUFFER == ENABLE)
static BOOL UIDisplay_IsUsingPaintDC(void)
{
#if (DISPLAY2_FUNC == ENABLE)
	if (g_bDualUI) { //dual display
		if ((gbOsdRotate[0] == OSD_ROTATE_0_DEGREE) && (gbOsdRotate[1] == OSD_ROTATE_0_DEGREE)) { //two normal display
			//use double buffer(for show DC and paint DC)
			return FALSE;
		} else { //one normal display + one rotate display
			//use single buffer(for show DC only)
			return TRUE;
		}
	}
	else
#endif
	{ //single display
		if (gbOsdRotate[0] == OSD_ROTATE_0_DEGREE) { //one normal display
			//use double buffer(for show DC and paint DC)
			return FALSE;
		} else { //one rotate display
			//use single buffer(for show DC only)
			return TRUE;
		}
	}

	return FALSE;
}
#endif

BOOL UIDisplay_GetCleanBuf(UINT32 *pAddr, UINT32 *pSize)
{
	//if all OSD buffers are continuous memory block, fill current addr & size
	UINT32 uiBufAddr;
	uiBufAddr = OS_GetMempoolAddr(POOL_ID_DISP_OSD1);
	*pAddr = uiBufAddr;
	*pSize = OS_GetMempoolSize(POOL_ID_DISP_OSD1) + OS_GetMempoolSize(POOL_ID_DISP_OSD1TEMP);
	return TRUE;
}

void UIDisplay_Init(UINT8 iDD, BOOL bClear, ISIZE *pDeviceSize)
{
	RESULT r;
	LAYER_INIT LayerInit;

	UINT32 uiBufAddr;
	//fixed buffer size
	UINT32 osd_w = OSD_W;
	UINT32 osd_h = OSD_H;
	//fixed buffer size
	UINT32 vdo_w = OSD_W;
	UINT32 vdo_h = OSD_H;
	//ISIZE DeviceSize;
	LAYER_INIT *pLayerInit = (LAYER_INIT *)&LayerInit; //layer init parameter
	//DeviceSize = GxVideo_GetDeviceSize(DOUT1);
	ISIZE DeviceSize = pDeviceSize[0];
#if (OSD_USE_ROTATE_BUFFER == ENABLE)
	UINT32 did = iDD >> 4;
#endif
#if (DISPLAY2_FUNC == ENABLE)
	DBG_MSG("^Ydual=%d\r\n", g_bDualUI);
#endif
	DBG_MSG("^YDOUT=%d\r\n", iDD >> 4);
#if (OSD_USE_ROTATE_BUFFER == ENABLE)
	DBG_MSG("^Yrotate[%d]=%d\r\n", did, gbOsdRotate[did]);
#endif
	switch (iDD & 0x0f) { //check layer
	case LAYER_OSD1:

		uiBufAddr = OS_GetMempoolAddr(POOL_ID_DISP_OSD1);

		pLayerInit->uiType = TYPE_FB;
		pLayerInit->uiPxlfmt = DISPLAY_OSD_FMT;
		//#NT#2016/10/14#KCHong -begin
		//#NT#Forced use 320x240 UI for engineer mode
#if (CALIBRATION_FUNC == ENABLE)
		if (IsEngineerMode()) {
			pLayerInit->uiWidth = 320;
			pLayerInit->uiHeight = 240;
		} else {
#else
		{
#endif
			pLayerInit->uiWidth = osd_w;
			pLayerInit->uiHeight = osd_h;
		}
		//#NT#2016/10/14#KCHong -end
		switch (pLayerInit->uiPxlfmt) {
		case DISP_PXLFMT_INDEX8:
			pLayerInit->uiBufSize = (osd_w * osd_h) >> 0;
			break;
		case DISP_PXLFMT_ARGB8565_PK:
			pLayerInit->uiBufSize = (osd_w * osd_h) * 3;
			break;
		case DISP_PXLFMT_ARGB8888_PK:
			pLayerInit->uiBufSize = (osd_w * osd_h) * 4;
			break;
		default:
			DBG_ERR("unknown pLayerInit->uiPxlfmt=%d\r\n", pLayerInit->uiPxlfmt);
		}
		//dynamic window size
		pLayerInit->win.x = 0;
		pLayerInit->win.y = 0;
		pLayerInit->win.w = DeviceSize.w;
		pLayerInit->win.h = DeviceSize.h;
		pLayerInit->uiWinAttr = 0;
		//for dummy LCD, window is very small, avoid scale-down buffer too much
		if ((DeviceSize.w == 80) && (DeviceSize.h == 60)) { //if dummy LCD?
			if (pLayerInit->win.w < (INT32)(osd_w / 2)) {
				DBG_WRN("devSize.w=%d, osd w=%d, force osd.win w=%d\r\n", DeviceSize.w, osd_w, osd_w / 2);
				pLayerInit->win.w = (INT32)(osd_w / 2);
			}
			if (pLayerInit->win.h < (INT32)(osd_h / 2)) {
				DBG_WRN("devSize.h=%d, osd h=%d, force osd.win h=%d\r\n", DeviceSize.h, osd_h, osd_h / 2);
				pLayerInit->win.h = (INT32)(osd_h / 2);
			}
		}

		//always clean OSD buffer before OSD rotate and before OSD not-rotate
#if (OSD_USE_ROTATE_BUFFER == ENABLE)
		{
			UINT32 osd_addr = 0;
			UINT32 osd_size = 0;
			UIDisplay_GetCleanBuf(&osd_addr, &osd_size);
			if (osd_addr != 0) {
				memset((void *)osd_addr, 0, osd_size); //clear last pShowDC (non-rotate buffer cannot reuse)
			}
		}
#endif

#if (OSD_USE_ROTATE_BUFFER == ENABLE)
#if (DISPLAY2_FUNC == ENABLE)
		if (g_bDualUI) { //dual display
			if ((gbOsdRotate[0] == OSD_ROTATE_0_DEGREE) && (gbOsdRotate[1] == OSD_ROTATE_0_DEGREE)) { //two normal display
				DBG_MSG("case 2-1\r\n");
				pLayerInit->uiBufCount = 1;
				pLayerInit->uiSwapEffect = SWAPEFFECT_COPY; //use double buffer(for show DC and paint DC)
				pLayerInit->pBufAddr[0] = uiBufAddr; //1st buffer
				pLayerInit->pBufAddr[1] = uiBufAddr + pLayerInit->uiBufSize; //2nd buffer
				pLayerInit->pBufAddr[2] = 0;
			} else { //one normal display + one rotate display
				DBG_MSG("case 2-2\r\n");
				pLayerInit->uiBufCount = 0;
				pLayerInit->uiSwapEffect = SWAPEFFECT_DISCARD; //use single buffer(for show DC only)
				if (did == 0) { //1st display
					pLayerInit->pBufAddr[0] = uiBufAddr;    //1st buffer
				} else { //2nd display
					pLayerInit->pBufAddr[0] = uiBufAddr + pLayerInit->uiBufSize;    //2nd buffer
				}
				pLayerInit->pBufAddr[1] = 0;
				pLayerInit->pBufAddr[2] = 0;
				if (did == 0) { //1st display
					pLayerInit->uiSwapEffect |= SWAPEFFECT_XY;
				} else { //2nd display
				}
			}
		}
		else
#endif
		{ //single display
			if (gbOsdRotate[did] == OSD_ROTATE_0_DEGREE) { //one normal display
				DBG_MSG("case 1-1\r\n");
				pLayerInit->uiBufCount = 1;
				pLayerInit->uiSwapEffect = SWAPEFFECT_COPY; //use double buffer(for show DC and paint DC)
				pLayerInit->pBufAddr[0] = uiBufAddr; //1st buffer
				pLayerInit->pBufAddr[1] = uiBufAddr + pLayerInit->uiBufSize; //2nd buffer
				pLayerInit->pBufAddr[2] = 0;
			} else { //one rotate display
				DBG_MSG("case 1-2\r\n");
				pLayerInit->uiBufCount = 0;
				pLayerInit->uiSwapEffect = SWAPEFFECT_DISCARD; //use single buffer(for show DC only)
				pLayerInit->pBufAddr[0] = uiBufAddr; //1st buffer
				pLayerInit->pBufAddr[1] = 0;
				pLayerInit->pBufAddr[2] = 0;
				pLayerInit->uiSwapEffect |= SWAPEFFECT_XY;
			}
		}
#else
		pLayerInit->uiBufCount = 1;
		pLayerInit->uiSwapEffect = SWAPEFFECT_COPY; //use double buffer(for show DC and paint DC)
		pLayerInit->pBufAddr[0] = uiBufAddr; //1st buffer
		pLayerInit->pBufAddr[1] = uiBufAddr + pLayerInit->uiBufSize; //2nd buffer
		pLayerInit->pBufAddr[2] = 0;
#endif

#if (OSD_USE_ROTATE_BUFFER == ENABLE)
		if (gbOsdRotate[did] != OSD_ROTATE_0_DEGREE) {
			//prepare additional paint DC
			UINT32 buf_fmt = 0;
			UINT8 *pBuffer = 0;
			switch (pLayerInit->uiPxlfmt) {
			case DISP_PXLFMT_INDEX8:
				buf_fmt = PXLFMT_INDEX8;
				break;
			case DISP_PXLFMT_ARGB8565_PK:
				buf_fmt = PXLFMT_RGBA5658_PK;
				break;
			case DISP_PXLFMT_ARGB8888_PK:
				buf_fmt = PXLFMT_RGBA8888_PK;
				break;
			default:
				DBG_ERR("unknown pLayerInit->uiPxlfmt=%d\r\n", pLayerInit->uiPxlfmt);
			}
#if (DISPLAY2_FUNC == ENABLE)
			if (g_bDualUI) { //dual display
				pBuffer = (UINT8 *)(uiBufAddr + pLayerInit->uiBufSize + pLayerInit->uiBufSize);    //3rd buffer
			}
			else
#endif

			{ //single display
				pBuffer = (UINT8 *)(uiBufAddr + pLayerInit->uiBufSize);    //2nd buffer
			}
			memset(pPaintDC, 0, sizeof(DC));
			GxGfx_AttachDC(pPaintDC, TYPE_FB,
						   buf_fmt, osd_w, osd_h, 0, pBuffer, 0, 0);
			DBG_MSG("paint buf=%08x, size=%08x\n\r", pBuffer, LayerInit.uiBufSize);
			DBG_MSG("paint buf.w=%d, buf.h=%d;\n\r", osd_w, osd_h);
		}
#endif

		r = GxDisplay_InitLayer(iDD, &LayerInit, bClear);
		if (r != GX_OK) {
			DBG_ERR("GxDisplay_InitLayer %d\r\n", r);
		}
		UI_DirtyDisplaySource(iDD); //must force flush to update new swap-buffer and new swap-effect
		DBG_MSG("OSD1 buf=%08x, size=%08x\n\r", LayerInit.pBufAddr[0], LayerInit.uiBufSize);
		DBG_MSG("OSD1 buf.w=%d, buf.h=%d;\n\r", LayerInit.uiWidth, LayerInit.uiHeight);
		DBG_MSG("OSD1 win.w=%d, win.h=%d;\n\r", LayerInit.win.w, LayerInit.win.h);

		break;

	case LAYER_OSD2:

		uiBufAddr = OS_GetMempoolAddr(POOL_ID_DISP_OSD2);

		pLayerInit->uiType = TYPE_FB;
		pLayerInit->uiPxlfmt = DISPLAY_OSD2_FMT;
		pLayerInit->uiWidth = osd_w;
		pLayerInit->uiHeight = osd_h;
#if (OSD2_USE_DOUBLE_BUFFER == ENABLE)
		pLayerInit->uiBufCount = 1;
		pLayerInit->uiSwapEffect = SWAPEFFECT_COPY;
#else
		pLayerInit->uiBufCount = 0;
		pLayerInit->uiSwapEffect = SWAPEFFECT_DISCARD;
#endif
		switch (pLayerInit->uiPxlfmt) {
		case DISP_PXLFMT_INDEX8:
			pLayerInit->uiBufSize = (osd_w * osd_h) >> 0;
			break;
		case DISP_PXLFMT_ARGB8565_PK:
			pLayerInit->uiBufSize = (osd_w * osd_h) * 3;
			break;
		case DISP_PXLFMT_ARGB8888_PK:
			pLayerInit->uiBufSize = (osd_w * osd_h) * 4;
			break;
		default:
			DBG_ERR("unknown pLayerInit->uiPxlfmt=%d\r\n", pLayerInit->uiPxlfmt);
		}
		pLayerInit->pBufAddr[0] = uiBufAddr;
#if (OSD2_USE_DOUBLE_BUFFER == ENABLE)
		pLayerInit->pBufAddr[1] = uiBufAddr + pLayerInit->uiBufSize;
#else
		pLayerInit->pBufAddr[1] = 0;
#endif
		pLayerInit->pBufAddr[2] = 0;
		//dynamic window size
		pLayerInit->win.x = 0;
		pLayerInit->win.y = 0;
		pLayerInit->win.w = DeviceSize.w;
		pLayerInit->win.h = DeviceSize.h;
		pLayerInit->uiWinAttr = 0;
		if (pLayerInit->win.w < (INT32)(osd_w / 2)) {
			DBG_WRN("devSize.w=%d, osd2 w=%d, force osd2.win w=%d\r\n", DeviceSize.w, osd_w, osd_w / 2);
			pLayerInit->win.w = (INT32)(osd_w / 2);
		}
		if (pLayerInit->win.h < (INT32)(osd_h / 2)) {
			DBG_WRN("devSize.h=%d, osd2 h=%d, force osd2.win h=%d\r\n", DeviceSize.h, osd_h, osd_h / 2);
			pLayerInit->win.h = (INT32)(osd_h / 2);
		}

		r = GxDisplay_InitLayer(iDD, &LayerInit, bClear);
		if (r != GX_OK) {
			DBG_ERR("GxDisplay_InitLayer %d\r\n", r);
		}
		DBG_MSG("OSD2 buf=%08x, size=%08x\n\r", LayerInit.pBufAddr[0], LayerInit.uiBufSize);
		DBG_MSG("OSD2 buf.w=%d, buf.h=%d;\n\r", LayerInit.uiWidth, LayerInit.uiHeight);
		DBG_MSG("OSD2 win.w=%d, win.h=%d;\n\r", LayerInit.win.w, LayerInit.win.h);

		break;

	case LAYER_VDO1:

		//uiBufAddr = OS_GetMempoolAddr(POOL_ID_DISP_VDO1);
		DBG_ERR("POOL_ID_DISP_VDO1 is not found!");
		uiBufAddr = 0;

		pLayerInit->uiType = TYPE_FB;
		pLayerInit->uiPxlfmt = DISPLAY_VDO_FMT;
		pLayerInit->uiWidth = vdo_w;
		pLayerInit->uiHeight = vdo_h;
#if (VDO_USE_DOUBLE_BUFFER == ENABLE)
		pLayerInit->uiBufCount = 1;
		pLayerInit->uiSwapEffect = SWAPEFFECT_COPY;
#else
		pLayerInit->uiBufCount = 0;
		pLayerInit->uiSwapEffect = SWAPEFFECT_DISCARD;
#endif
		switch (pLayerInit->uiPxlfmt) {
		case DISP_PXLFMT_YUV422_PK:
			pLayerInit->uiBufSize = (vdo_w * vdo_h) * 2 + 64;
			break;
			break;
		default:
			DBG_ERR("Pxlfmt %d", pLayerInit->uiPxlfmt);

		}
		pLayerInit->pBufAddr[0] = uiBufAddr;
#if (VDO_USE_DOUBLE_BUFFER == ENABLE)
		pLayerInit->pBufAddr[1] = uiBufAddr + pLayerInit->uiBufSize;
#else
		pLayerInit->pBufAddr[1] = 0;
#endif
		pLayerInit->pBufAddr[2] = 0;
		//dynamic window size
		pLayerInit->win.x = 0;
		pLayerInit->win.y = 0;
		pLayerInit->win.w = DeviceSize.w;
		pLayerInit->win.h = DeviceSize.h;
		pLayerInit->uiWinAttr = 0;

		r = GxDisplay_InitLayer(iDD, &LayerInit, bClear);
		if (r != GX_OK) {
			DBG_ERR("GxDisplay_InitLayer %d\r\n", r);
		}
		DBG_MSG("VDO1 buf=%08x, size=%08x\n\r", LayerInit.pBufAddr[0], LayerInit.uiBufSize);
		DBG_MSG("VDO1 buf.w=%d, buf.h=%d;\n\r", LayerInit.uiWidth, LayerInit.uiHeight);
		DBG_MSG("VDO1 win.w=%d, win.h=%d;\n\r", LayerInit.win.w, LayerInit.win.h);

		break;

	case LAYER_VDO2:

		uiBufAddr = OS_GetMempoolAddr(POOL_ID_DISP_VDO2);

		pLayerInit->uiType = TYPE_FB;
		pLayerInit->uiPxlfmt = DISPLAY_VDO_FMT;
		#if(IS_ALPHA_TYPEFMT(DISPLAY_OSD_FMT))
		//for save buffer size
		pLayerInit->uiWidth = 8;
		pLayerInit->uiHeight = 6;
		#else
		pLayerInit->uiWidth = vdo_w;
		pLayerInit->uiHeight = vdo_h;
		#endif
#if (VDO2_USE_DOUBLE_BUFFER == ENABLE)
		pLayerInit->uiBufCount = 1;
		pLayerInit->uiSwapEffect = SWAPEFFECT_COPY;
#else
		pLayerInit->uiBufCount = 0;
		pLayerInit->uiSwapEffect = SWAPEFFECT_DISCARD;
#endif
		switch (pLayerInit->uiPxlfmt) {
		case DISP_PXLFMT_YUV422_PK:
			pLayerInit->uiBufSize = (vdo_w * vdo_h) * 2 + 64;
			break;
		default:
			DBG_ERR("Pxlfmt %d", pLayerInit->uiPxlfmt);
		}
		pLayerInit->pBufAddr[0] = uiBufAddr;
#if (VDO2_USE_DOUBLE_BUFFER == ENABLE)
		pLayerInit->pBufAddr[1] = uiBufAddr + pLayerInit->uiBufSize;
#else
		pLayerInit->pBufAddr[1] = 0;
#endif
		pLayerInit->pBufAddr[2] = 0;
		pLayerInit->win.x = 0;
		pLayerInit->win.y = 0;
		pLayerInit->win.w = DeviceSize.w;
		pLayerInit->win.h = DeviceSize.h;
		pLayerInit->uiWinAttr = 0;

		r = GxDisplay_InitLayer(iDD, &LayerInit, bClear);
		if (r != GX_OK) {
			DBG_ERR("GxDisplay_InitLayer %d\r\n", r);
		}
		DBG_MSG("VDO2 buf1=%08x, size=%08x\n\r", LayerInit.pBufAddr[0], LayerInit.uiBufSize);
		DBG_MSG("VDO2 buf2=%08x, size=%08x\n\r", LayerInit.pBufAddr[1], LayerInit.uiBufSize);
		DBG_MSG("VDO2 buf.w=%d, buf.h=%d;\n\r", LayerInit.uiWidth, LayerInit.uiHeight);
		DBG_MSG("VDO2 win.w=%d, win.h=%d;\n\r", LayerInit.win.w, LayerInit.win.h);

		break;
	}
}

#if (OSD_USE_ROTATE_BUFFER == ENABLE)
void View_Window_ConfigAttr(UINT32 did, UINT32 attr, UINT32 value)
{
	if (attr == OSD_ATTR_ROTATE) {
		DBG_MSG("...............Set rot[%d]=%d\r\n", did, value);
		gbOsdRotate[did] = value;
	}
}

DC *View_Window_BeginRotate(DC *pDC)
{
	if (UIDisplay_IsUsingPaintDC()) {
		//#NT#2014/03/25#KS Hung -begin
		if (gbOsdRotate[0] == OSD_ROTATE_90_DEGREE) {
			DBG_MSG("case b-1\r\n");
			GxDisplay_WaitVD();
			pShowDC = pDC;
			GxGfx_CopyEx(pPaintDC, pShowDC, ROP_ROTATE | SRC_RCCW_90, 0); //copy show to paint with rotate CCW 90
			DBG_MSG("^YBegin Rotate CCW 90\r\n");
			return pPaintDC;
		} else if (gbOsdRotate[0] == OSD_ROTATE_270_DEGREE) {
			DBG_MSG("case b-2\r\n");
			GxDisplay_WaitVD();
			pShowDC = pDC;
			GxGfx_CopyEx(pPaintDC, pShowDC, ROP_ROTATE | SRC_RCCW_270, 0); //copy show to paint with rotate CCW 90
			DBG_MSG("^YBegin Rotate CCW 270\r\n");
			return pPaintDC;
		}
		//#NT#2014/03/25#KS Hung -end
	}
	return pDC;
}
void View_Window_EndRotate(void)
{
	if (UIDisplay_IsUsingPaintDC()) {
		//#NT#2014/03/25#KS Hung -begin
		if (gbOsdRotate[0] == OSD_ROTATE_90_DEGREE) {
			DBG_MSG("case e-1\r\n");
			GxDisplay_WaitVD();
			GxGfx_CopyEx(pShowDC, pPaintDC, ROP_ROTATE | SRC_RCW_90, 0); //copy paint to show with rotate CW 90
#if (DISPLAY2_FUNC == ENABLE)
			if (g_bDualUI) { //dual display
				DC *p2ndShowDC = GxDisplay_BeginDraw(DOUT2 | LAYER_OSD1);
				GxGfx_Copy(p2ndShowDC, pPaintDC); //copy paint to 2nd show
				GxDisplay_EndDraw(DOUT2 | LAYER_OSD1, p2ndShowDC);
			}
#endif
			pShowDC = 0;
			DBG_MSG("^YEnd Rotate CW 90\r\n");
		} else if (gbOsdRotate[0] == OSD_ROTATE_270_DEGREE) {
			DBG_MSG("case e-2\r\n");
			GxDisplay_WaitVD();
			GxGfx_CopyEx(pShowDC, pPaintDC, ROP_ROTATE | SRC_RCW_270, 0); //copy paint to show with rotate CW 90
#if (DISPLAY2_FUNC == ENABLE)
			if (g_bDualUI) { //dual display
				DC *p2ndShowDC = GxDisplay_BeginDraw(DOUT2 | LAYER_OSD1);
				GxGfx_Copy(p2ndShowDC, pPaintDC); //copy paint to 2nd show
				GxDisplay_EndDraw(DOUT2 | LAYER_OSD1, p2ndShowDC);
			}
#endif
			pShowDC = 0;
			DBG_MSG("^YEnd Rotate CW 270\r\n");
		}
		//#NT#2014/03/25#KS Hung -end
	}
}
#endif


BOOL bInitOSD1 = FALSE;
BOOL bInitOSD2 = FALSE;
BOOL bInitVDO1 = FALSE;
BOOL bInitVDO2 = FALSE;
#if (DISPLAY_OSD_FMT == DISP_PXLFMT_ARGB8888_PK)
extern PALETTE_ITEM gDemoKit_Palette_RGBA8888[256];
#elif (DISPLAY_OSD_FMT == DISP_PXLFMT_ARGB8565_PK)
extern PALETTE_ITEM gDemoKit_Palette_RGBA5658[256];
#endif

//OSD1
BOOL View_Window(UINT32 cmd, UINT32 param1, UINT32 param2)
{
	if (cmd == SHOWCMD_BEGINDRAW) {
		BOOL bEn;
		UINT32 uiShowID;
		ISIZE DeviceSize;
		DeviceSize = GxVideo_GetDeviceSize(DOUT1);
		//restore to default (UI backgound)
		DBG_IND("^Gbegin OSD1 src=WINDOW\r\n");

		bEn = GxDisplay_Get(LAYER_OSD1, LAYER_STATE_ENABLE);
		uiShowID = (GxDisplay_Get(LAYER_OSD1, LAYER_STATE_INFO) & 0x000000FF) >> 0;
		if (bInitOSD1 && (uiShowID != 0)) {
			//dummy flip for avoid flicker effect (double buffer fail when re-init layer)
			DC *pDC = GxDisplay_BeginDraw(LAYER_OSD1); //dummy draw for swap buffer
			GxDisplay_EndDraw(LAYER_OSD1, pDC);
			GxDisplay_Flip(FALSE);
		}
		UIDisplay_Init(LAYER_OSD1, FALSE, &DeviceSize); //attacth to UI source
		//if single buffert cannot SWAPEFFECT_COPY
		//GxDisplay_SetSwapEffect(iDD, SWAPEFFECT_COPY); //enable double buffer
		GxDisplay_Set(LAYER_OSD1, LAYER_STATE_ENABLE, bEn); //keep enable status before init layer

#if (DISPLAY2_FUNC == ENABLE)
		if (g_bDualUI) {
			DeviceSize = GxVideo_GetDeviceSize(DOUT2);
			//only refernece OSD1 mask ,because View_Window is for OSD1
			if((g_bDualUI_dev2_osd_mask & (DISP_OSD1_EN))==DISP_OSD1_EN)
				bEn = 1;
			else
				bEn = 0;

			uiShowID = (GxDisplay_Get(DOUT2 | LAYER_OSD1, LAYER_STATE_INFO) & 0x000000FF) >> 0;
			if (bInitOSD1 && (uiShowID != 0)) {
				//dummy flip for avoid flicker effect (double buffer fail when re-init layer)
				DC *pDC = GxDisplay_BeginDraw(DOUT2 | LAYER_OSD1); //dummy draw for swap buffer
				GxDisplay_EndDraw(DOUT2 | LAYER_OSD1, pDC);
				GxDisplay_FlipEx(DOUT2 | LAYER_ALL, FALSE);
			}
			UIDisplay_Init(DOUT2 | LAYER_OSD1, FALSE, &DeviceSize); //attacth to UI source
			GxDisplay_FlipEx(DOUT2 | LAYER_OSD1, TRUE);
			GxDisplay_Set(DOUT2 | LAYER_OSD1, LAYER_STATE_ENABLE, bEn); //keep enable status before init layer

		}
#endif

		bInitOSD1 = TRUE;

		return TRUE;
	}
	if (cmd == SHOWCMD_ENDDRAW) {
		DBG_IND("^Gend OSD1 src=WINDOW\r\n");
		GxDisplay_Set(LAYER_OUTPUT, CTRL_STATE_OSD1COMPARE, COMPARE_KEY_EQUAL); //enable OSD1 color key
		#if (DISPLAY_OSD_FMT == DISP_PXLFMT_ARGB8888_PK)
		GxDisplay_Set(LAYER_OUTPUT, CTRL_STATE_OSD1COLORKEY, gDemoKit_Palette_RGBA8888[0]); //set color key by RGB888
		#elif (DISPLAY_OSD_FMT == DISP_PXLFMT_ARGB8565_PK)
		GxDisplay_Set(LAYER_OUTPUT, CTRL_STATE_OSD1COLORKEY, gDemoKit_Palette_RGBA5658[0]); //set color key by RGB565
		#else
		GxDisplay_Set(LAYER_OUTPUT, CTRL_STATE_OSD1COLORKEY, 0); //set color key by index
		#endif
		GxDisplay_Set(LAYER_OUTPUT, CTRL_STATE_OSD1ALPHA, OSD_OPAQUE_100); //set blending OSD1=100%
		GxDisplay_Set(LAYER_OUTPUT, CTRL_STATE_OSD1BLEND, BLEND_PIXELALPHA); //OSD1 blending src
#if (DISPLAY2_FUNC == ENABLE)
		if (g_bDualUI) {
			GxDisplay_Set(DOUT2 | LAYER_OUTPUT, CTRL_STATE_OSD1COMPARE, COMPARE_KEY_EQUAL); //enable OSD1 color key
			#if (DISPLAY_OSD_FMT == DISP_PXLFMT_ARGB8888_PK)
			GxDisplay_Set(DOUT2 | LAYER_OUTPUT, CTRL_STATE_OSD1COLORKEY, gDemoKit_Palette_RGBA8888[0]); //set color key by RGB888
			#elif (DISPLAY_OSD_FMT == DISP_PXLFMT_ARGB8565_PK)
			GxDisplay_Set(DOUT2 | LAYER_OUTPUT, CTRL_STATE_OSD1COLORKEY, gDemoKit_Palette_RGBA5658[0]); //set color key by RGB565
			#else
			GxDisplay_Set(DOUT2 | LAYER_OUTPUT, CTRL_STATE_OSD1COLORKEY, 0); //set color key by index
			#endif
			GxDisplay_Set(DOUT2 | LAYER_OUTPUT, CTRL_STATE_OSD1ALPHA, OSD_OPAQUE_100); //set blending OSD1=100%
			GxDisplay_Set(DOUT2 | LAYER_OUTPUT, CTRL_STATE_OSD1BLEND, BLEND_PIXELALPHA); //OSD1 blending src
		}
#endif
		return TRUE;
	}
	if (cmd == SHOWCMD_SETSIZE) {
		UINT32 DevID = param1;
		ISIZE *pSize = (ISIZE *)param2;
		DBG_IND("^Gresize OSD1 src=WINDOW\r\n");

#if (DISPLAY2_FUNC == ENABLE)
		if ((DevID == 1) && (g_bDualUI)) {
			UINT32 uiShowID;
			UINT32 uiShowID_2;
			GxDisplay_FlipEx(DOUT2 | LAYER_OSD1, TRUE); //load palette?
			UIDisplay_Init(DOUT2 | LAYER_OSD1, FALSE, pSize); //attacth to UI source
			GxDisplay_FlipEx(DOUT2 | LAYER_OSD1, TRUE);

			//check Dev-1 OSD1 sync to Dev-0 OSD1?
			uiShowID = (GxDisplay_Get(LAYER_OSD1, LAYER_STATE_INFO) & 0x000000FF) >> 0;
			uiShowID_2 = (GxDisplay_Get(DOUT2 | LAYER_OSD1, LAYER_STATE_INFO) & 0x000000FF) >> 0;
			if (uiShowID_2 != uiShowID) {
				//dummy flip for avoid Dev-1 OSD1 is not sync to Dev-0 OSD1
				DC *pDC = GxDisplay_BeginDraw(DOUT2 | LAYER_OSD1); //dummy draw for swap buffer
				GxDisplay_EndDraw(DOUT2 | LAYER_OSD1, pDC);
				debug_msg("^GDUMMY FLIP!!!\r\n");
				GxDisplay_FlipEx(DOUT2 | LAYER_OSD1, TRUE);
			}
		}
		else
#endif
		if (DevID == 0) {
			UIDisplay_Init(DOUT1 | LAYER_OSD1, FALSE, pSize); //attacth to UI source
			GxDisplay_FlipEx(DOUT1 | LAYER_OSD1, TRUE);
		}

		//Display_SetWindow(LAYER_OSD1, 0, 0, pSize->w, pSize->h); //change window size
		//DBG_MSG("OSD1 win.w=%d, win.h=%d;\n\r",pSize->w, pSize->h);
		return TRUE;
	}
	return FALSE;
}

//OSD2
BOOL View_Info(UINT32 cmd, UINT32 param1, UINT32 param2)
{
	if (cmd == SHOWCMD_BEGINDRAW) {
		BOOL bEn;
		UINT32 uiShowID;
		ISIZE DeviceSize;
		DeviceSize = GxVideo_GetDeviceSize(DOUT1);
		//restore to default (UI backgound)
		DBG_IND("^Gbegin OSD2 src=INFO\r\n");

		bEn = GxDisplay_Get(LAYER_OSD2, LAYER_STATE_ENABLE);
		uiShowID = (GxDisplay_Get(LAYER_OSD2, LAYER_STATE_INFO) & 0x000000FF) >> 0;
		if (bInitOSD2 && (uiShowID != 0)) {
			//dummy flip for avoid flicker effect (double buffer fail when re-init layer)
			DC *pDC = GxDisplay_BeginDraw(LAYER_OSD2); //dummy draw for swap buffer
			GxDisplay_EndDraw(LAYER_OSD2, pDC);
			GxDisplay_Flip(FALSE);
		}
		UIDisplay_Init(LAYER_OSD2, FALSE, &DeviceSize); //attacth to UI source
		//if single buffert cannot SWAPEFFECT_COPY
		//GxDisplay_SetSwapEffect(iDD, SWAPEFFECT_COPY); //enable double buffer
		GxDisplay_Set(LAYER_OSD2, LAYER_STATE_ENABLE, bEn); //keep enable status before init layer
		bInitOSD2 = TRUE;

		return TRUE;
	}
	if (cmd == SHOWCMD_ENDDRAW) {
		DBG_IND("^Gend OSD2 src=INFO\r\n");
		GxDisplay_Set(LAYER_OUTPUT, CTRL_STATE_OSD2COMPARE, COMPARE_KEY_EQUAL); //enable OSD2 color key
		GxDisplay_Set(LAYER_OUTPUT, CTRL_STATE_OSD2COLORKEY, 0); //set color key index
		GxDisplay_Set(LAYER_OUTPUT, CTRL_STATE_OSD2ALPHA, OSD_OPAQUE_100); //set blending OSD2=100%
		GxDisplay_Set(LAYER_OUTPUT, CTRL_STATE_OSD2BLEND, BLEND_PIXELALPHA); //OSD2 blending src
		return TRUE;
	}
	if (cmd == SHOWCMD_SETSIZE) {
		ISIZE *pSize = (ISIZE *)param2;
		DBG_IND("^Gresize OSD2 src=INFO\r\n");
		UIDisplay_Init(LAYER_OSD2, FALSE, pSize); //attacth to UI source
		GxDisplay_FlipEx(LAYER_OSD2, TRUE);
		//Display_SetWindow(LAYER_OSD2, 0, 0, pSize->w, pSize->h); //change window size
		//DBG_MSG("OSD2 win.w=%d, win.h=%d;\n\r",pSize->w, pSize->h);
		return TRUE;
	}
	return FALSE;
}

//VDO1


//VDO2
BOOL View_Background(UINT32 cmd, UINT32 param1, UINT32 param2)
{
	if (cmd == SHOWCMD_BEGINDRAW) {
		BOOL bEn;
		UINT32 uiShowID;
		ISIZE DeviceSize;
		DeviceSize = GxVideo_GetDeviceSize(DOUT1);
		//restore to default (UI backgound)
		DBG_IND("^Gbegin VDO2 src=BACKGROUND\r\n");

		bEn = GxDisplay_Get(LAYER_VDO2, LAYER_STATE_ENABLE);
		uiShowID = (GxDisplay_Get(LAYER_VDO2, LAYER_STATE_INFO) & 0x000000FF) >> 0;
		if (bInitVDO2 && (uiShowID != 0)) {
			//dummy flip for avoid flicker effect (double buffer fail when re-init layer)
			DC *pDC = GxDisplay_BeginDraw(LAYER_VDO2);
			GxDisplay_EndDraw(LAYER_VDO2, pDC);
			GxDisplay_WaitVD();
			GxDisplay_Flip(FALSE);
			GxDisplay_WaitVD();
		}
		UIDisplay_Init(LAYER_VDO2, TRUE, &DeviceSize); //attacth to UI source
#if (VDO2_USE_DOUBLE_BUFFER == ENABLE)
		GxDisplay_SetSwapEffect(LAYER_VDO2, SWAPEFFECT_COPY); //enable double buffer
#endif
		GxDisplay_Set(LAYER_VDO2, LAYER_STATE_ENABLE, bEn); //keep enable status before init layer
		bInitVDO2 = TRUE;

		return TRUE;
	}
	if (cmd == SHOWCMD_ENDDRAW) {
		DBG_IND("^Gend VDO2 src=BACKGROUND\r\n");
		#if(IS_ALPHA_TYPEFMT(DISPLAY_OSD_FMT))
		GxDisplay_Set(LAYER_OUTPUT, CTRL_STATE_ALLCTRL, 1);
		#endif
		GxDisplay_Set(LAYER_OUTPUT, CTRL_STATE_VDO2COMPARE, COMPARE_KEY_NEVER);
		GxDisplay_Set(LAYER_OUTPUT, CTRL_STATE_VDO2COLORKEY, 0);
		GxDisplay_Set(LAYER_OUTPUT, CTRL_STATE_VDO2ALPHA, OSD_OPAQUE_100);
		GxDisplay_Set(LAYER_OUTPUT, CTRL_STATE_VDO2BLEND, BLEND_DISABLE);
		return TRUE;
	}
	if (cmd == SHOWCMD_SETSIZE) {
		ISIZE *pSize = (ISIZE *)param2;
		DBG_IND("^Gresize VDO2 src=BACKGROUND\r\n");
		UIDisplay_Init(LAYER_VDO2, FALSE, pSize); //attacth to UI source
		GxDisplay_FlipEx(LAYER_VDO2, TRUE);
		//Display_SetWindow(LAYER_VDO2, 0, 0, pSize->w, pSize->h); //change window size
		//DBG_MSG("VDO2 win.w=%d, win.h=%d;\n\r",pSize->w, pSize->h);
		return TRUE;
	}
	return FALSE;
}

BOOL View_Background_CK(UINT32 cmd, UINT32 param1, UINT32 param2)
{
	if (cmd == SHOWCMD_BEGINDRAW) {
		BOOL bEn;
		UINT32 uiShowID;
		ISIZE DeviceSize;
		DeviceSize = GxVideo_GetDeviceSize(DOUT1);
		//restore to default (UI backgound)
		DBG_IND("^Gbegin VDO2 src=BACKGROUND_CK\r\n");

		bEn = GxDisplay_Get(LAYER_VDO2, LAYER_STATE_ENABLE);
		uiShowID = (GxDisplay_Get(LAYER_VDO2, LAYER_STATE_INFO) & 0x000000FF) >> 0;
		if (bInitVDO2 && (uiShowID != 0)) {
			//dummy flip for avoid flicker effect (double buffer fail when re-init layer)
			DC *pDC = GxDisplay_BeginDraw(LAYER_VDO2);
			GxDisplay_EndDraw(LAYER_VDO2, pDC);
			GxDisplay_WaitVD();
			GxDisplay_Flip(FALSE);
			GxDisplay_WaitVD();
		}
		UIDisplay_Init(LAYER_VDO2, FALSE, &DeviceSize); //attacth to UI source
		GxDisplay_SetSwapEffect(LAYER_VDO2, SWAPEFFECT_COPY); //enable double buffer
		GxDisplay_Set(LAYER_VDO2, LAYER_STATE_ENABLE, bEn); //keep enable status before init layer
		bInitVDO2 = TRUE;

		return TRUE;
	}
	if (cmd == SHOWCMD_ENDDRAW) {
		DBG_IND("^Gend VDO2 src=BACKGROUND_CK\r\n");
		GxDisplay_Set(LAYER_OUTPUT, CTRL_STATE_VDO2COMPARE, COMPARE_KEY_EQUAL); //enable color key
		GxDisplay_Set(LAYER_OUTPUT, CTRL_STATE_VDO2COLORKEY, VDO_TRANSPARENT_COLOR); //color key = VDO_TRANSPARENT_COLOR
		GxDisplay_Set(LAYER_OUTPUT, CTRL_STATE_VDO2ALPHA, OSD_OPAQUE_100);
		GxDisplay_Set(LAYER_OUTPUT, CTRL_STATE_VDO2BLEND, BLEND_DISABLE);
		return TRUE;
	}
	if (cmd == SHOWCMD_SETSIZE) {
		ISIZE *pSize = (ISIZE *)param2;
		DBG_IND("^Gresize VDO2 src=BACKGROUND_CK\r\n");
		UIDisplay_Init(LAYER_VDO2, FALSE, pSize); //attacth to UI source
		GxDisplay_FlipEx(LAYER_VDO2, TRUE);
		//Display_SetWindow(LAYER_VDO2, 0, 0, pSize->w, pSize->h); //change window size
		//DBG_MSG("VDO2 win.w=%d, win.h=%d;\n\r",pSize->w, pSize->h);
		return TRUE;
	}
	return FALSE;
}






