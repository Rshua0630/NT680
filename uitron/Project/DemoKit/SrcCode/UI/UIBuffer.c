#include "Debug.h"
#include "PrjCfg.h"
#include "SysCfg.h"
#include "GxDisplay.h"

#include "GxColor.h"
#include "GxRect.h"
#include "GxGfx.h"
#include "GxImage.h"

///////////////////////////////////////////////////////////////////////////////
#define __MODULE__          UIBuf
#define __DBGLVL__          2 // 0=FATAL, 1=ERR, 2=WRN, 3=UNIT, 4=FUNC, 5=IND, 6=MSG, 7=VALUE, 8=USER
#define __DBGFLT__          "*" //*=All, [mark]=CustomClass
#include "DebugModule.h"
///////////////////////////////////////////////////////////////////////////////


#define LAYER_BUFFER_NUM    3

DC      _gDisplay1_OSD1[LAYER_BUFFER_NUM];
#if (OSD2_FUNC == ENABLE)
DC      _gDisplay1_OSD2[LAYER_BUFFER_NUM];
#endif
IMG_BUF _gDisplay1_VDO1[LAYER_BUFFER_NUM];
IMG_BUF _gDisplay1_VDO2[LAYER_BUFFER_NUM];
#if (DISPLAY2_FUNC == ENABLE)
DC      _gDisplay2_OSD1[LAYER_BUFFER_NUM];
IMG_BUF _gDisplay2_VDO1[LAYER_BUFFER_NUM];
#endif

void *UIDisplay1_OSD1_BM_get(UINT32 buffer_id)
{
	return (void *) & (_gDisplay1_OSD1[buffer_id]);
}
#if (OSD2_FUNC == ENABLE)
void *UIDisplay1_OSD2_BM_get(UINT32 buffer_id)
{
	return (void *) & (_gDisplay1_OSD2[buffer_id]);
}
#endif
#if (DISPLAY2_FUNC == ENABLE)
void *UIDisplay2_OSD1_BM_get(UINT32 buffer_id)
{
	return (void *) & (_gDisplay2_OSD1[buffer_id]);
}
#endif
UINT32 UIDisplay_OSD_BM_calcsize(LAYER_INIT *pLayerInit)
{
	UINT16 fmt = 0;
	switch (pLayerInit->uiPxlfmt) {
	case DISP_PXLFMT_INDEX8:
		fmt = PXLFMT_INDEX8;
		break;
	case DISP_PXLFMT_ARGB8565_PK:
		fmt = PXLFMT_RGBA5658_PK;
		break;
	//case DISP_PXLFMT_RGBA5654_PK: fmt = PXLFMT_RGBA5654_PK; break;
	case DISP_PXLFMT_ARGB8888_PK:
		fmt = PXLFMT_RGBA8888_PK;
		break;
	}
	if(pLayerInit->uiSwapEffect & SWAPEFFECT_XY)
	return GxGfx_CalcDCMemSize(
			   pLayerInit->uiHeight, pLayerInit->uiWidth, TYPE_FB, fmt);
	else
	return GxGfx_CalcDCMemSize(
			   pLayerInit->uiWidth, pLayerInit->uiHeight, TYPE_FB, fmt);
}
void UIDisplay_OSD_BM_attach(void *buf, LAYER_INIT *pLayerInit, UINT32 buffer_id)
{
	UINT16 fmt = 0;
	switch (pLayerInit->uiPxlfmt) {
	case DISP_PXLFMT_INDEX8:
		fmt = PXLFMT_INDEX8;
		break;
	case DISP_PXLFMT_ARGB8565_PK:
		fmt = PXLFMT_RGBA5658_PK;
		break;
	//case DISP_PXLFMT_RGBA5654_PK: fmt = PXLFMT_RGBA5654_PK; break;
	case DISP_PXLFMT_ARGB8888_PK:
		fmt = PXLFMT_RGBA8888_PK;
		break;
	}
	if(pLayerInit->uiSwapEffect & SWAPEFFECT_XY)
	GxGfx_AttachDC((DC *)buf, TYPE_FB,
				   fmt, pLayerInit->uiHeight, pLayerInit->uiWidth, 0, (UINT8 *)(pLayerInit->pBufAddr[buffer_id]), 0, 0);
	else
	GxGfx_AttachDC((DC *)buf, TYPE_FB,
				   fmt, pLayerInit->uiWidth, pLayerInit->uiHeight, 0, (UINT8 *)(pLayerInit->pBufAddr[buffer_id]), 0, 0);
}
void UIDisplay_OSD_BM_detach(void *buf)
{
	GxGfx_DetachDC((DC *)buf);
}
void UIDisplay_OSD_BM_copy(void *buf1, void *buf2)
{
	GxGfx_Copy((DC *)buf1, (DC *)buf2);
}
void UIDisplay_OSD_BM_clear(void *buf)
{
	GxGfx_Clear((DC *)buf);
}
void *UIDisplay1_VDO1_BM_get(UINT32 buffer_id)
{
	return (void *) & (_gDisplay1_VDO1[buffer_id]);
}
void *UIDisplay1_VDO2_BM_get(UINT32 buffer_id)
{
	return (void *) & (_gDisplay1_VDO2[buffer_id]);
}
#if (DISPLAY2_FUNC == ENABLE)
void *UIDisplay2_VDO1_BM_get(UINT32 buffer_id)
{
	return (void *) & (_gDisplay2_VDO1[buffer_id]);
}
#endif
UINT32 UIDisplay_VDO_BM_calcsize(LAYER_INIT *pLayerInit)
{
	UINT16 fmt = 0;
	switch (pLayerInit->uiPxlfmt) {
	case DISP_PXLFMT_YUV422_PK:
		fmt = GX_IMAGE_PIXEL_FMT_YUV422_PACKED;
		break;
	case DISP_PXLFMT_YUV420_PK:
		fmt = GX_IMAGE_PIXEL_FMT_YUV420_PACKED;
		break;
	}
	return GxImg_CalcRequireSize
		   (pLayerInit->uiWidth, pLayerInit->uiHeight, fmt, ALIGN_CEIL_4(pLayerInit->uiWidth));
}
void UIDisplay_VDO_BM_attach(void *buf, LAYER_INIT *pLayerInit, UINT32 buffer_id)
{
	UINT16 fmt = 0;
	switch (pLayerInit->uiPxlfmt) {
	case DISP_PXLFMT_YUV422_PK:
		fmt = GX_IMAGE_PIXEL_FMT_YUV422_PACKED;
		break;
	case DISP_PXLFMT_YUV420_PK:
		fmt = GX_IMAGE_PIXEL_FMT_YUV420_PACKED;
		break;
	}
	GxImg_InitBuf((IMG_BUF *)buf,
				  pLayerInit->uiWidth, pLayerInit->uiHeight, fmt, ALIGN_CEIL_4(pLayerInit->uiWidth), pLayerInit->pBufAddr[buffer_id], pLayerInit->uiBufSize);
}
void UIDisplay_VDO_BM_detach(void *buf)
{
	memset(buf, 0x00, sizeof(IMG_BUF));
}
void UIDisplay_VDO_BM_copy(void *buf1, void *buf2)
{
	GxImg_CopyData((IMG_BUF *)buf1, REGION_MATCH_IMG, (IMG_BUF *)buf2, REGION_MATCH_IMG);
}
void UIDisplay_VDO_BM_clear(void *buf)
{
	GxImg_FillData((IMG_BUF *)buf, REGION_MATCH_IMG, COLOR_YUV_BLACK);
}

LAYER_BUFMAN UIDisplay1_OSD1_BM = {
	UIDisplay1_OSD1_BM_get,
	UIDisplay_OSD_BM_calcsize,
	UIDisplay_OSD_BM_attach,
	UIDisplay_OSD_BM_detach,
	UIDisplay_OSD_BM_copy,
	UIDisplay_OSD_BM_clear,
};
#if (OSD2_FUNC == ENABLE)
LAYER_BUFMAN UIDisplay1_OSD2_BM = {
	UIDisplay1_OSD2_BM_get,
	UIDisplay_OSD_BM_calcsize,
	UIDisplay_OSD_BM_attach,
	UIDisplay_OSD_BM_detach,
	UIDisplay_OSD_BM_copy,
	UIDisplay_OSD_BM_clear,
};
#endif
LAYER_BUFMAN UIDisplay1_VDO1_BM = {
	UIDisplay1_VDO1_BM_get,
	UIDisplay_VDO_BM_calcsize,
	UIDisplay_VDO_BM_attach,
	UIDisplay_VDO_BM_detach,
	UIDisplay_VDO_BM_copy,
	UIDisplay_VDO_BM_clear,
};
LAYER_BUFMAN UIDisplay1_VDO2_BM = {
	UIDisplay1_VDO2_BM_get,
	UIDisplay_VDO_BM_calcsize,
	UIDisplay_VDO_BM_attach,
	UIDisplay_VDO_BM_detach,
	UIDisplay_VDO_BM_copy,
	UIDisplay_VDO_BM_clear,
};
#if (DISPLAY2_FUNC == ENABLE)
LAYER_BUFMAN UIDisplay2_OSD1_BM = {
	UIDisplay2_OSD1_BM_get,
	UIDisplay_OSD_BM_calcsize,
	UIDisplay_OSD_BM_attach,
	UIDisplay_OSD_BM_detach,
	UIDisplay_OSD_BM_copy,
	UIDisplay_OSD_BM_clear,
};
LAYER_BUFMAN UIDisplay2_VDO1_BM = {
	UIDisplay2_VDO1_BM_get,
	UIDisplay_VDO_BM_calcsize,
	UIDisplay_VDO_BM_attach,
	UIDisplay_VDO_BM_detach,
	UIDisplay_VDO_BM_copy,
	UIDisplay_VDO_BM_clear,
};
#endif
void UI_RegisterBuf(void)
{
	GxDisplay_RegBufMan(DOUT1 | LAYER_OSD1, &UIDisplay1_OSD1_BM);
#if (OSD2_FUNC == ENABLE)
	GxDisplay_RegBufMan(DOUT1 | LAYER_OSD2, &UIDisplay1_OSD2_BM);
#endif
	GxDisplay_RegBufMan(DOUT1 | LAYER_VDO1, &UIDisplay1_VDO1_BM);
	GxDisplay_RegBufMan(DOUT1 | LAYER_VDO2, &UIDisplay1_VDO2_BM);
#if (DISPLAY2_FUNC == ENABLE)
	GxDisplay_RegBufMan(DOUT2 | LAYER_OSD1, &UIDisplay2_OSD1_BM);
	GxDisplay_RegBufMan(DOUT2 | LAYER_OSD2, 0);
	GxDisplay_RegBufMan(DOUT2 | LAYER_VDO1, &UIDisplay2_VDO1_BM);
	GxDisplay_RegBufMan(DOUT2 | LAYER_VDO2, 0);
#endif
}



