#include "SysKer.h"
#include "PrjCfg.h"
#include "Color.h"
#include "SizeConvert.h"
#include "AppDisp_PBView.h"
#include "PlaybackTsk.h"
#include "GxVideo.h"
#include "GxImage.h"
#include "ImageUnit_UserProc.h"
#include "ImageStream.h"


///////////////////////////////////////////////////////////////////////////////
#define __MODULE__          PBVIEW
#define __DBGLVL__          2 // 0=FATAL, 1=ERR, 2=WRN, 3=UNIT, 4=FUNC, 5=IND, 6=MSG, 7=VALUE, 8=USER
#define __DBGFLT__          "*" //*=All, [mark]=CustomClass
#include "DebugModule.h"
///////////////////////////////////////////////////////////////////////////////

USIZE  aspect_ratio[2];
ER(*gfpDrawCb)(PIMG_BUF pSrc, PIMG_BUF pDst, PUSIZE dst_ratio);
UINT32 gPbState = PB_VIEW_STATE_NONE;
IMG_BUF dst_img[2];
extern UINT32 g_DualVideo;

static void PBView_DrawSingleView(PIMG_BUF pSrc, PIMG_BUF pDst, PUSIZE dst_ratio, UINT32 id)
{
	BOOL   bIsMovieFile;
	PIRECT SrcRegion = 0;
	IRECT  DstRegion = {0};
	UINT32 SrcWidth, SrcHeight;
	UINT32 DstWidth, DstHeight;
	UINT32 usPBDisplayWidth = pDst->Width, usPBDisplayHeight = pDst->Height;
	PB_KEEPAR ImageRatioTrans;
	USIZE size = {0};
	SIZECONVERT_INFO ScaleInfo = {0};
	PURECT prcVdoWIN = 0;

	PB_GetParam(PBPRMID_VDO_WIN_ARRAY, (UINT32 *) &prcVdoWIN);
	prcVdoWIN = prcVdoWIN + id;

	DBG_IND("id=%d, w=%d, h=%d\r\n", id, prcVdoWIN->w, prcVdoWIN->h);
	DBG_IND("dst id=%d, w=%d, h=%d\r\n", id, pDst->Width, pDst->Height);

	UINT8 curr_mode = 0;
	PB_GetParam(PBPRMID_PLAYBACK_MODE, (UINT32 *) &curr_mode);

	UINT32 enable_flag = 0;
	PB_GetParam(PBPRMID_EN_FLAGS, (UINT32 *) &enable_flag);

	if ((curr_mode == PLAYMODE_ASF) || (curr_mode == PLAYMODE_AVI) || (curr_mode == PLAYMODE_MOVMJPG)) {
		bIsMovieFile = TRUE;
	} else {
		bIsMovieFile = FALSE;
	}

	//start to user draw
	PB_GetParam(PBPRMID_SRC_REGION, (UINT32 *) &SrcRegion);
	PB_GetParam(PBPRMID_SRC_SIZE, (UINT32 *) &size);
	SrcWidth = size.w;
	SrcHeight = size.h;

	if (SrcHeight == 0) {
		DBG_ERR("SrcHeight is 0\r\n");
		return;
	}

	DstWidth  = usPBDisplayWidth;
	DstHeight = usPBDisplayHeight;
	DstRegion.w = DstWidth;
	DstRegion.h = DstHeight;

	ScaleInfo.uiSrcWidth  = SrcWidth;
	ScaleInfo.uiSrcHeight = SrcHeight;
	ScaleInfo.uiDstWRatio = dst_ratio->w;
	ScaleInfo.uiDstHRatio = dst_ratio->h;

	if (SrcWidth == usPBDisplayWidth && SrcHeight == usPBDisplayHeight) {
		ImageRatioTrans = PB_KEEPAR_NONE;
	} else {
		ImageRatioTrans = PB_GetImageRatioTrans(SrcWidth, SrcHeight, dst_ratio->w, dst_ratio->h);
	}
#if 0
	// For printer define out width & height
	if ((guiUserJPGOutWidth != 0) && (guiUserJPGOutHeight != 0) &&
		(guiUserJPGOutWidth <= DstWidth) && (guiUserJPGOutHeight <= DstHeight)) {
		if ((guiUserJPGOutStartX != 0) || (guiUserJPGOutStartY != 0)) {
			DstRegion.x = (usPBDisplayWidth * guiUserJPGOutStartY) + (guiUserJPGOutStartX);
		}
	} else
#endif
		if ((ImageRatioTrans) && (FALSE == bIsMovieFile)) {
			// non-Full Screen images
			ScaleInfo.uiDstWidth  = usPBDisplayWidth;
			ScaleInfo.uiDstHeight = usPBDisplayHeight;
			ScaleInfo.alignType   = SIZECONVERT_ALIGN_FLOOR_16;
			DisplaySizeConvert(&ScaleInfo);

			DstRegion.x = ScaleInfo.uiOutX;
			DstRegion.y = ScaleInfo.uiOutY;
			DstRegion.w = ScaleInfo.uiOutWidth;
			DstRegion.h = ScaleInfo.uiOutHeight;

			if ((0 != prcVdoWIN->x) || (0 != prcVdoWIN->y)) {
				COORDSYS_CONV CSConv;

				CSConv.uiGlobalCoordX = ScaleInfo.uiOutX;
				CSConv.uiGlobalCoordY = ScaleInfo.uiOutY;
				CSConv.uiGlobalCoordW = ScaleInfo.uiOutWidth;
				CSConv.uiGlobalCoordH = ScaleInfo.uiOutHeight;
				CSConv.uiGlobalCoord_RngW = usPBDisplayWidth;
				CSConv.uiGlobalCoord_RngH = usPBDisplayHeight;
				CSConv.uiClientCoord_OriX = prcVdoWIN->x;
				CSConv.uiClientCoord_OriY = prcVdoWIN->y;
				CSConv.uiClientCoord_RngW = prcVdoWIN->w;
				CSConv.uiClientCoord_RngH = prcVdoWIN->h;
				CSConv.prcOut = (PURECT)&DstRegion;
				PB_CoordSysConvert(&CSConv);
			}
		} else if (FALSE == bIsMovieFile) {
			// Full Screen images
			DstRegion.x = prcVdoWIN->x;
			DstRegion.y = prcVdoWIN->y;
			DstRegion.w = prcVdoWIN->w;
			DstRegion.h = prcVdoWIN->h;
		}

	if (TRUE == bIsMovieFile) {
		// Specify the first video frame display size, not auto-scale to full screen.
		PURECT prc1stVdo = 0;

		PB_GetParam(PBPRMID_VDO_RECT_ARRAY, (UINT32 *) &prc1stVdo);

		prc1stVdo = prc1stVdo + id;

		ScaleInfo.uiDstWidth  = prc1stVdo->w;
		ScaleInfo.uiDstHeight = prc1stVdo->h;
		ScaleInfo.alignType   = SIZECONVERT_ALIGN_FLOOR_16;
		DisplaySizeConvert(&ScaleInfo);

		DstRegion.x = ScaleInfo.uiOutX;
		DstRegion.y = ScaleInfo.uiOutY;
		DstRegion.w = ScaleInfo.uiOutWidth;
		DstRegion.h = ScaleInfo.uiOutHeight;

		if ((0 != prcVdoWIN->x) || (0 != prcVdoWIN->y)) {
			COORDSYS_CONV CSConv;

			CSConv.uiGlobalCoordX = ScaleInfo.uiOutX;
			CSConv.uiGlobalCoordY = ScaleInfo.uiOutY;
			CSConv.uiGlobalCoordW = ScaleInfo.uiOutWidth;
			CSConv.uiGlobalCoordH = ScaleInfo.uiOutHeight;
			CSConv.uiGlobalCoord_RngW = usPBDisplayWidth;
			CSConv.uiGlobalCoord_RngH = usPBDisplayHeight;
			CSConv.uiClientCoord_OriX = prcVdoWIN->x;
			CSConv.uiClientCoord_OriY = prcVdoWIN->y;
			CSConv.uiClientCoord_RngW = prcVdoWIN->w;
			CSConv.uiClientCoord_RngH = prcVdoWIN->h;
			CSConv.prcOut = (PURECT)&DstRegion;
			PB_CoordSysConvert(&CSConv);
		}

		if (enable_flag & PB_ENABLE_SHOW_BG_IN_MOVIE) {
			// Not support yet
			DBG_ERR("Not support movie BG\r\n");
		}
	}
	// clear dst buffer
	GxImg_FillData(pDst, REGION_MATCH_IMG, COLOR_YUV_BLACK);
	GxImg_ScaleDataFine(pSrc, SrcRegion, pDst, &DstRegion);
}

static void PBView_DrawRotateView(PIMG_BUF pSrc, PIMG_BUF pDst, PUSIZE dst_ratio)
{
	UINT32 DoNotScalar;
	UINT32 usPBDisplayWidth = pDst->Width, usPBDisplayHeight = pDst->Height;
	PB_KEEPAR ImageRatioTrans;
	IRECT  DstRegion = {0};
	SIZECONVERT_INFO ScaleInfo = {0};
	PURECT prcVdoWIN = 0;
	UINT32 SrcWidth, SrcHeight;
	PIRECT  SrcRegion = 0;
	USIZE size = {0};
	UINT32 rot_dir = 0;

	PB_GetParam(PBPRMID_VDO_WIN_ARRAY, (UINT32 *) &prcVdoWIN);

	//start to user draw
	PB_GetParam(PBPRMID_SRC_REGION, (UINT32 *) &SrcRegion);
	PB_GetParam(PBPRMID_SRC_SIZE, (UINT32 *) &size);
	SrcWidth = size.w;
	SrcHeight = size.h;

	if (SrcHeight == 0) {
		DBG_ERR("SrcHeight is 0\r\n");
		return;
	}

	PB_GetParam(PBPRMID_ROTATE_DIR, (UINT32 *) &rot_dir);

	if (SrcWidth < SrcHeight) {
		if ((rot_dir == PLAY_ROTATE_DIR_180) || (rot_dir == PLAY_ROTATE_DIR_VER) || (rot_dir == PLAY_ROTATE_DIR_HOR)) {
			DoNotScalar = 0;
		} else {
			DoNotScalar = 1;
		}
	} else {
		if ((rot_dir == PLAY_ROTATE_DIR_180) || (rot_dir == PLAY_ROTATE_DIR_VER) || (rot_dir == PLAY_ROTATE_DIR_HOR)) {
			DoNotScalar = 1;
		} else {
			DoNotScalar = 0;
		}
	}
	if (rot_dir == PLAY_ROTATE_DIR_180) {
		ImageRatioTrans = PB_GetImageRatioTrans(SrcWidth, SrcHeight, dst_ratio->w, dst_ratio->h);
	} else {
		ImageRatioTrans = PB_GetImageRatioTrans(SrcHeight, SrcWidth, dst_ratio->w, dst_ratio->h);
	}

	if ((DoNotScalar == 1) && (!ImageRatioTrans)) {
		DstRegion.x = prcVdoWIN->x;
		DstRegion.y = prcVdoWIN->y;
		DstRegion.w = prcVdoWIN->w;
		DstRegion.h = prcVdoWIN->h;

		GxImg_FillData(pDst, REGION_MATCH_IMG, COLOR_YUV_BLACK);
		GxImg_ScaleData(pSrc, SrcRegion, pDst, &DstRegion);
	} else {
		if ((rot_dir == PLAY_ROTATE_DIR_90) || (rot_dir == PLAY_ROTATE_DIR_270)) {
			ScaleInfo.uiSrcWidth  = SrcHeight;
			ScaleInfo.uiSrcHeight = SrcWidth;
		} else {
			ScaleInfo.uiSrcWidth  = SrcWidth;
			ScaleInfo.uiSrcHeight = SrcHeight;
		}
		ScaleInfo.uiDstWidth  = usPBDisplayWidth;
		ScaleInfo.uiDstHeight = usPBDisplayHeight;
		ScaleInfo.uiDstWRatio = dst_ratio->w;
		ScaleInfo.uiDstHRatio = dst_ratio->h;
		ScaleInfo.alignType   = SIZECONVERT_ALIGN_FLOOR_16;
		DisplaySizeConvert(&ScaleInfo);

		DstRegion.x = ScaleInfo.uiOutX;
		DstRegion.y = ScaleInfo.uiOutY;
		DstRegion.w = ScaleInfo.uiOutWidth;
		DstRegion.h = ScaleInfo.uiOutHeight;

#if 0//(PB_ROTATEDISPLAY_KEEP_RATIO == DISABLE)
		DstRegion.w = usPBDisplayWidth;
		DstRegion.h = usPBDisplayHeight;
#endif

		// Draw image in specific area
		if ((0 != prcVdoWIN->x) || (0 != prcVdoWIN->y)) {
			COORDSYS_CONV CSConv;

			CSConv.uiGlobalCoordX = ScaleInfo.uiOutX;
			CSConv.uiGlobalCoordY = ScaleInfo.uiOutY;
			CSConv.uiGlobalCoordW = ScaleInfo.uiOutWidth;
			CSConv.uiGlobalCoordH = ScaleInfo.uiOutHeight;
			CSConv.uiGlobalCoord_RngW = usPBDisplayWidth;
			CSConv.uiGlobalCoord_RngH = usPBDisplayHeight;
			CSConv.uiClientCoord_OriX = prcVdoWIN->x;
			CSConv.uiClientCoord_OriY = prcVdoWIN->y;
			CSConv.uiClientCoord_RngW = prcVdoWIN->w;
			CSConv.uiClientCoord_RngH = prcVdoWIN->h;
			CSConv.prcOut = (PURECT)&DstRegion;
			PB_CoordSysConvert(&CSConv);
		}

		GxImg_FillData(pDst, REGION_MATCH_IMG, COLOR_YUV_BLACK);
		GxImg_ScaleData(pSrc, SrcRegion, pDst, &DstRegion);
	}
}

static ER PBView_DrawMethod(PIMG_BUF pSrc, PIMG_BUF pDst, PUSIZE dst_ratio)
{
	UINT32   rot_dir = 0;
	PIMG_BUF pDst2 = pDst + 1;
	PUSIZE dst_ratio2 = dst_ratio + 1;

	PB_GetParam(PBPRMID_ROTATE_DIR, (UINT32 *) &rot_dir);

	//--------------------Customer Draw Begin-------------------------------
	if (PLAY_ROTATE_DIR_UNKNOWN != rot_dir) {
		PBView_DrawRotateView(pSrc, pDst, dst_ratio);
	} else {
		if (g_DualVideo & DISPLAY_1) {
			PBView_DrawSingleView(pSrc, pDst, dst_ratio, 0);
		}
		if (g_DualVideo & DISPLAY_2) {
			PBView_DrawSingleView(pSrc, pDst2, dst_ratio2, 1);
		}
	}
	//--------------------Customer Draw End---------------------------------

	PB_SetPBFlag(PB_SET_FLG_DRAW_END);

	return E_OK;
}

static ER PBView_OnSingleDraw(PIMG_BUF pSrc, PIMG_BUF pDst, PUSIZE dst_ratio)
{
	return PBView_DrawMethod(pSrc, pDst, dst_ratio);
}

static ER PBView_OnRotateDraw(PIMG_BUF pSrc, PIMG_BUF pDst, PUSIZE dst_ratio)
{
	return PBView_DrawMethod(pSrc, pDst, dst_ratio);
}

static void PBView_DrawPZoomView(PIMG_BUF pSrc, PIMG_BUF pDst, PUSIZE pDstRatio)
{
	UINT32 SrcWidth, SrcHeight, SrcStart_X, SrcStart_Y, DstWidth, DstHeight;
	PIRECT pSrcRegion = 0;
	IRECT  SrcRegion = {0};
	PIRECT pDstRegion = 0;
	IRECT  DstRegion = {0};
	UINT32 usPBDisplayWidth = pDst->Width;
#if 0//PB_ZOOMROTATED_WIDTH_LIMIT
	UINT32 usPBDisplayHeight = pDst->Height;
	UINT32 uiWidthLimit;
#endif

	PB_GetParam(PBPRMID_SRC_REGION, (UINT32 *) &pSrcRegion);
	PB_GetParam(PBPRMID_DST_REGION, (UINT32 *) &pDstRegion);

	SrcWidth   = pSrcRegion->w;
	SrcHeight  = pSrcRegion->h;
	SrcStart_X = pSrcRegion->x;
	SrcStart_Y = pSrcRegion->y;

	DstWidth   = pDstRegion->w;
	DstHeight  = pDstRegion->h;

#if 0//PB_ZOOMROTATED_WIDTH_LIMIT
	uiWidthLimit = usPBDisplayHeight * 3 / 4 * usPBDisplayWidth / usPBDisplayHeight * pDstRatio->h / pDstRatio->w;
#else
	if (DstWidth < usPBDisplayWidth)
#endif
	{
		GxImg_FillData(pDst, REGION_MATCH_IMG, COLOR_YUV_BLACK);
	}
	SrcRegion.x = SrcStart_X & 0xFFFFFFF0;
	SrcRegion.y = SrcStart_Y;
	SrcRegion.w = SrcWidth;
	SrcRegion.h = SrcHeight;

#if 0//PB_ZOOMROTATED_WIDTH_LIMIT
	DstRegion.w = ALIGN_CEIL_8(uiWidthLimit);
#else
	DstRegion.w = DstWidth;
#endif
	DstRegion.h = DstHeight;
	GxImg_ScaleDataFine(pSrc, &SrcRegion, pDst, &DstRegion);
}

static ER PBView_OnSpeZoomDraw(PIMG_BUF pSrc, PIMG_BUF pDst, PUSIZE dst_ratio)
{
	//--------------------Customer Draw Begin-------------------------------
	//start to user draw
	PBView_DrawPZoomView(pSrc, pDst, dst_ratio);
	//--------------------Customer Draw End---------------------------------

	PB_SetPBFlag(PB_SET_FLG_DRAW_END);

	return E_OK;
}

static ER PBView_OnByPassDraw(PIMG_BUF pSrc, PIMG_BUF pDst, PUSIZE dst_ratio)
{
	//--------------------Customer Draw Begin-----------------------------------
	GxImg_ScaleDataFine(pSrc, REGION_MATCH_IMG, pDst, REGION_MATCH_IMG);
	//--------------------Customer Draw End-------------------------------------

	PB_SetPBFlag(PB_SET_FLG_DRAW_END);

	return E_OK;
}

static void PBView_DrawThumbView(UINT32 i, PIMG_BUF pSrc, PIMG_BUF pDst, PUSIZE dst_ratio, UINT32 id)
{
	UINT32 SrcWidth, SrcHeight; //SrcStart_X, SrcStart_Y
	UINT32 DstWidth, DstHeight, DstX, DstY;
	UINT8  ucOrientation;
	IRECT  DstRegion = {0};
	PURECT idx_view;
	UINT32 width_array, height_array;
	BOOL  dec_err_array;
	BOOL   dec_err, exif_exist;
	UINT32 exif_orient = 0;

	PB_GetParam(PBPRMID_THUMB_RECT, (UINT32 *) &idx_view);
	PB_GetParam(PBPRMID_THUMB_WIDTH_ARRAY, (UINT32 *) &width_array);
	PB_GetParam(PBPRMID_THUMB_HEIGHT_ARRAY, (UINT32 *) &height_array);
	PB_GetParam(PBPRMID_THUMB_DEC_ARRAY, (UINT32 *) &dec_err_array);
	PB_GetParam(PBPRMID_EXIF_EXIST, (UINT32 *) &exif_exist);
	PB_GetParam(PBPRMID_EXIF_ORIENT, (UINT32 *) &exif_orient);
	dec_err = *((((BOOL *)dec_err_array) + i));

	//idx_view += id*25;

	SrcWidth  = *((((UINT32 *)width_array) + i));
	SrcHeight = *((((UINT32 *)height_array) + i));
	//SrcStart_X = 0;
	//SrcStart_Y = 0;
	DstWidth  = (idx_view + i)->w;
	DstHeight = (idx_view + i)->h;
	DstX = (idx_view + i)->x;
	DstY = (idx_view + i)->y;

	// clear BG
	if (i == 0) {
		GxImg_FillData(pDst, REGION_MATCH_IMG, COLOR_YUV_BLACK);
	}
	DstRegion.x = DstX;
	DstRegion.y = DstY;
	DstRegion.w = DstWidth;
	DstRegion.h = DstHeight;
	// decode error
	if (dec_err == TRUE) {
		GxImg_FillData(pDst, &DstRegion, COLOR_YUV_BLACK);
	}
	// decode ok
	else {
		UINT16 usPBDisplayWidth = pDst->Width;
		PB_KEEPAR ImageRatioTrans;

		if ((SrcWidth == pDst->Width) && (SrcHeight == pDst->Height)) {
			ImageRatioTrans = PB_KEEPAR_NONE;
		} else {
			ImageRatioTrans = PB_GetImageRatioTrans(SrcWidth, SrcHeight, dst_ratio->w, dst_ratio->h);
		}

		if (ImageRatioTrans == PB_KEEPAR_LETTERBOXING) {
			UINT32 uiTmpHeight;
			UINT32 temp;

			// Clear buffer..
			DstRegion.w = DstWidth;
			DstRegion.h = DstHeight;
			GxImg_FillData(pDst, &DstRegion, COLOR_YUV_BLACK);

			uiTmpHeight = DstWidth * SrcHeight / SrcWidth * pDst->Height / usPBDisplayWidth * dst_ratio->w / dst_ratio->h;
			temp = ((DstHeight - uiTmpHeight) + 1) & 0xFFFFFFFE;
			DstHeight = uiTmpHeight;
			DstRegion.y = DstRegion.y + (temp >> 1);
		}

		if (exif_exist) {
			ucOrientation = exif_orient;
		} else {
			ucOrientation = JPEG_EXIF_ORI_DEFAULT;
		}
		if (ucOrientation != JPEG_EXIF_ORI_DEFAULT) {
			if (ucOrientation != JPEG_EXIF_ORI_ROTATE_180) {
				UINT32 ulTmpWidth;

				ulTmpWidth = DstHeight * SrcHeight / SrcWidth * usPBDisplayWidth / pDst->Height * dst_ratio->h / dst_ratio->w;
				if (ulTmpWidth > DstWidth) {
					// Prevent user from setting strange-ratio thumbnail window
					ulTmpWidth = DstWidth / 2;
				}
				DstRegion.x += ((DstWidth - ulTmpWidth) >> 1);
				DstRegion.w = ulTmpWidth;
				DstRegion.h = DstHeight;
			} else {
				DstRegion.w = DstWidth;
				DstRegion.h = DstHeight;
			}
		} else {
			DstRegion.w = DstWidth;
			DstRegion.h = DstHeight;
		}
		GxImg_ScaleData(pSrc, REGION_MATCH_IMG, pDst, &DstRegion);
	}
}


ER PBView_OnThumbDraw(PIMG_BUF pSrc, PIMG_BUF pDst, PUSIZE dst_ratio)
{
	ER     er;
	UINT32 uiIdx;
	UINT32 i, thumb_num;
	ISF_DATA isf_data;
	ISF_OUT out = 0;

	if (g_DualVideo & DISPLAY_1) {
		out = ISF_OUT1;
	} else {
		out = ISF_OUT2;
	}

	PB_GetParam(PBPRMID_THUMB_CURR_NUM, (UINT32 *) &thumb_num);

	for (i = 0; i < thumb_num; i++) {
		uiIdx = (i & 0x1);

		if ((er = PB_LockThumb(uiIdx)) != E_OK) {
			//locked fail indicate skip this draw
			PB_SetPBFlag(PB_SET_FLG_BROWSER_END);
			return er;
		}
		isf_data.Sign = ISF_SIGN_DATA;
		if(ImageUnit_PullData(ImageUnit_Out(&ISF_UserProc, out), (ISF_DATA *)&isf_data, 0) == ISF_OK) {
			pSrc = (IMG_BUF *)&isf_data.Desc[0];

			//--------------------Customer Draw Begin---------------------------
			if (g_DualVideo & DISPLAY_1) {
				PBView_DrawThumbView(i, pSrc, pDst, dst_ratio, 0);
			}

			if (g_DualVideo & DISPLAY_2) {
				PBView_DrawThumbView(i, pSrc, pDst+1, dst_ratio+1, 1);
			}
			//--------------------Customer Draw End-----------------------------

			ImageUnit_ReleaseData(&isf_data);
		} else {
			DBG_ERR("Pull data failed\r\n");
		}
		PB_UnlockThumb(uiIdx);
	}
	PB_SetPBFlag(PB_SET_FLG_BROWSER_END);

	return E_OK;
}

void PBView_OnDrawCB(PB_VIEW_STATE view_state)
{
	ISF_DATA         isf_data = {0}, isf_new_data = {0}, isf_new_data2 = {0};
	ISF_UNIT         *pUnit;
	ISF_PORT         *pSrc = NULL, *pSrc2 = NULL;
	ISIZE            device_size = {0};
	UINT32           img_w = 0, img_h = 0;
	UINT32           img_buf_size = 0, buff_addr = 0;


	if (view_state == PB_VIEW_STATE_SINGLE) {
		gfpDrawCb = PBView_OnSingleDraw;
	} else if (view_state == PB_VIEW_STATE_ROTATE) {
		gfpDrawCb = PBView_OnRotateDraw;
	} else if (view_state == PB_VIEW_STATE_SPEZOOM) {
		gfpDrawCb = PBView_OnSpeZoomDraw;
	} else if (view_state == PB_VIEW_STATE_THUMB) {
		gfpDrawCb = PBView_OnThumbDraw;
	} else if (view_state == PB_VIEW_STATE_BYPASS) {
		gfpDrawCb = PBView_OnByPassDraw;
	}
	gPbState = view_state;

	if (g_DualVideo & DISPLAY_1) {
		aspect_ratio[0] = GxVideo_GetDeviceAspect(DOUT1);
	}
	if (g_DualVideo & DISPLAY_2) {
		aspect_ratio[1] = GxVideo_GetDeviceAspect(DOUT2);
	}



	pUnit = &ISF_UserProc;

	if (g_DualVideo & DISPLAY_1) {
		pSrc = ImageUnit_Out(pUnit, ISF_OUT1);

		if (ImageUnit_IsAllowPull(pSrc)) {
			device_size = GxVideo_GetDeviceSize(DOUT1);
			img_w = (UINT32)device_size.w;
			img_h = (UINT32)device_size.h;
			img_buf_size = img_w * img_h * 3 / 2;

			buff_addr = ImageUnit_NewData(img_buf_size, &isf_new_data);
			if (buff_addr == 0) {
				DBG_ERR("Get buffer fail\r\n");
				return;
			}
			GxImg_InitBuf(&dst_img[0], img_w, img_h, GX_IMAGE_PIXEL_FMT_YUV420, GXIMAGE_LINEOFFSET_ALIGN(16), buff_addr, img_buf_size);
			DBG_IND("dst1 w=%d, h=%d\r\n", img_w, img_h);
		}

	}
	if (g_DualVideo & DISPLAY_2) {
		pSrc = ImageUnit_Out(pUnit, ISF_OUT2);

		if (ImageUnit_IsAllowPull(pSrc)) {
			device_size = GxVideo_GetDeviceSize(DOUT2);
			img_w = (UINT32)device_size.w;
			img_h = (UINT32)device_size.h;
			img_buf_size = img_w * img_h * 3 / 2;
			buff_addr = ImageUnit_NewData(img_buf_size, &isf_new_data2);
			if (buff_addr == 0) {
				DBG_ERR("Get buffer fail\r\n");
				return;
			}
			GxImg_InitBuf(&dst_img[1], img_w, img_h, GX_IMAGE_PIXEL_FMT_YUV420, GXIMAGE_LINEOFFSET_ALIGN(16), buff_addr, img_buf_size);
			DBG_IND("dst2 w=%d, h=%d\r\n", img_w, img_h);
		}
	}

	if (ImageUnit_IsAllowPull(pSrc)) {
		if (gPbState == PB_VIEW_STATE_THUMB) {
			gfpDrawCb(0, &dst_img[0], &aspect_ratio[0]);
		} else {
			isf_data.Sign = ISF_SIGN_DATA;
			if (ImageUnit_PullData(pSrc, (ISF_DATA *)&isf_data, 0) == ISF_OK) {
				gfpDrawCb((IMG_BUF *)&isf_data.Desc[0], &dst_img[0], &aspect_ratio[0]);
				ImageUnit_ReleaseData(&isf_data);
			} else {
				DBG_ERR("Pull data failed\r\n");
				ImageUnit_ReleaseData(&isf_new_data);
				return;
			}
		}

		if (g_DualVideo & DISPLAY_1) {
			pSrc = ImageUnit_Out(pUnit, ISF_OUT1);
			isf_new_data.Sign = ISF_SIGN_DATA;
			memcpy(&isf_new_data.Desc[0], &dst_img[0], sizeof(IMG_BUF));
			//pSrc->pDestUnit->pBase->Add(pSrc->pDestUnit,(ISF_DATA*)&isf_new_data);
			ImageUnit_PushData(pSrc, (ISF_DATA *)&isf_new_data, 0);
		}

		if (g_DualVideo & DISPLAY_2) {
			pSrc2 = ImageUnit_Out(pUnit, ISF_OUT2);
			isf_new_data2.Sign = ISF_SIGN_DATA;
			memcpy(&isf_new_data2.Desc[0], &dst_img[1], sizeof(IMG_BUF));
			//pSrc->pDestUnit->pBase->Add(pSrc->pDestUnit,(ISF_DATA*)&isf_new_data);
			ImageUnit_PushData(pSrc2, (ISF_DATA *)&isf_new_data2, 0);
		}
	}

	if (g_DualVideo & DISPLAY_1) {
		ImageUnit_ReleaseData(&isf_new_data);
	}

	if (g_DualVideo & DISPLAY_2) {
		ImageUnit_ReleaseData(&isf_new_data2);
	}

}


