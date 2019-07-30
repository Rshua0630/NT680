#include "AppDisp_MoviePlayView.h"
#include "PrjCfg.h"


///////////////////////////////////////////////////////////////////////////////
#define __MODULE__          MOVIEPLAYVIEW
#define __DBGLVL__          2 // 0=FATAL, 1=ERR, 2=WRN, 3=UNIT, 4=FUNC, 5=IND, 6=MSG, 7=VALUE, 8=USER
#define __DBGFLT__          "*" //*=All, [mark]=CustomClass
#include "DebugModule.h"
///////////////////////////////////////////////////////////////////////////////

#define _USE_2PASS_SCALE_               DISABLE                         ///< use 2 pass scaling flow

#define PLAY_IMETMP_WIDTH               640
#define PLAY_IMETMP_HEIGHT              540
#define PLAY_FHD_WIDTH                  1920
#define PLAY_FHD_HEIGHT                 1080
#define DISP_QUEUE_MAXSIZE              4                               ///< display queue maximum size
#define DISP_QUEUE_DEFSIZE              3                               ///< display queue default size
#define DISP_PATH_MAX                   2                               ///< output display path max number

static BOOL             g_mpv_is_open = FALSE;                          ///< movie play view module is opened
static BOOL             gMoviePlay_StartPlay = FALSE;
static USIZE            g_src_ratio = {0};                              ///< aspect ratio of source image (only has one source)
static USIZE            g_dev_ratio[DISP_PATH_MAX] = {0};               ///< aspect ratio of display device
static ISIZE            g_dev_size[DISP_PATH_MAX] = {0};
static UINT32           g_buf_idx[DISP_PATH_MAX] = {0};
static UINT32           g_buf_num[DISP_PATH_MAX] = {0};
extern UINT32           g_DualVideo;
static ISF_DATA         g_isf_data[DISP_QUEUE_MAXSIZE];                 ///< input isf data (pulling from previous unit)
static ISF_DATA         g_isf_new_data[DISP_QUEUE_MAXSIZE];             ///< output isf data for display1 (after scaling)
static ISF_DATA         g_isf_new_data2[DISP_QUEUE_MAXSIZE];            ///< output isf data for display2 (after scaling)
static IMG_BUF          g_dst_img[DISP_PATH_MAX][DISP_QUEUE_MAXSIZE];
static IMG_BUF          g_src_img;

// debug
static UINT32 g_movieplayview_debug = FALSE;
#define	MPV_DUMP(fmtstr, args...) if (g_movieplayview_debug) DBG_DUMP(fmtstr, ##args)
static void MoviePlayView_InstallCmd(void);

#if (NMEDIAPLAY_FUNC == DISABLE)
void _AppDisp_MoviePlayView_Scalar2Display(PIMG_BUF pSrc, PGXIMG_MULTI_OUT pMultiDst)
{
	IRECT             SrcRect = {0};
	IRECT             DstRect1 = {0}, DstRect2 = {0};
	SIZECONVERT_INFO  ScaleInfo = {0};

    // Media Play has been closed
    if (gMoviePlay_StartPlay == FALSE) {
		return;
	}

    // Get src region
    SrcRect.x = 0;
	SrcRect.y = 0;
	SrcRect.w = pSrc->Width;
	SrcRect.h = pSrc->Height;

    MPV_DUMP("[MPV]SrcRect x=%d, y=%d, w=%d, h=%d\r\n",
                    SrcRect.x,
                    SrcRect.y,
                    SrcRect.w,
                    SrcRect.h);

    // Calculate dst region
    if (g_DualVideo & DISPLAY_1) {
    	if (g_src_ratio.w == g_src_ratio.h) {
    		ScaleInfo.uiSrcWidth  = pSrc->Width;
    		ScaleInfo.uiSrcHeight = pSrc->Height;
    	} else {
    		ScaleInfo.uiSrcWidth  = g_src_ratio.w;
    		ScaleInfo.uiSrcHeight = g_src_ratio.h;
    	}
    	ScaleInfo.uiDstWidth  = pMultiDst->pImg[0]->Width;
    	ScaleInfo.uiDstHeight = pMultiDst->pImg[0]->Height;
    	ScaleInfo.uiDstWRatio = g_dev_ratio[0].w;
    	ScaleInfo.uiDstHRatio = g_dev_ratio[0].h;
        ScaleInfo.alignType   = SIZECONVERT_ALIGN_FLOOR_32;

    	DisplaySizeConvert(&ScaleInfo);

    	DstRect1.x = ScaleInfo.uiOutX;
    	DstRect1.y = ScaleInfo.uiOutY;
    	DstRect1.w = ScaleInfo.uiOutWidth;
    	DstRect1.h = ScaleInfo.uiOutHeight;

        MPV_DUMP("[MPV]DstRect1 x=%d, y=%d, w=%d, h=%d\r\n",
                        DstRect1.x,
                        DstRect1.y,
                        DstRect1.w,
                        DstRect1.h);

        pMultiDst->pRegion[0] = (PIRECT)&DstRect1;
    }
    if (g_DualVideo & DISPLAY_2) {
    	if (g_src_ratio.w == g_src_ratio.h) {
    		ScaleInfo.uiSrcWidth  = pSrc->Width;
    		ScaleInfo.uiSrcHeight = pSrc->Height;
    	} else {
    		ScaleInfo.uiSrcWidth  = g_src_ratio.w;
    		ScaleInfo.uiSrcHeight = g_src_ratio.h;
    	}
    	ScaleInfo.uiDstWidth  = pMultiDst->pImg[1]->Width;
    	ScaleInfo.uiDstHeight = pMultiDst->pImg[1]->Height;
        ScaleInfo.uiDstWRatio = g_dev_ratio[1].w;
    	ScaleInfo.uiDstHRatio = g_dev_ratio[1].h;
    	ScaleInfo.alignType   = SIZECONVERT_ALIGN_FLOOR_32;

    	DisplaySizeConvert(&ScaleInfo);

    	DstRect2.x = ScaleInfo.uiOutX;
    	DstRect2.y = ScaleInfo.uiOutY;
    	DstRect2.w = ScaleInfo.uiOutWidth;
    	DstRect2.h = ScaleInfo.uiOutHeight;

        MPV_DUMP("[MPV]DstRect2 x=%d, y=%d, w=%d, h=%d\r\n",
                        DstRect2.x,
                        DstRect2.y,
                        DstRect2.w,
                        DstRect2.h);

        pMultiDst->pRegion[1] = (PIRECT)&DstRect2;
    }

    GxImg_ScaleMultiOut(pSrc, &SrcRect, pMultiDst);
}

//#NT#2018/05/31#Adam Su -begin
//#NT#2-pass flow
#if (_USE_2PASS_SCALE_ == ENABLE)
/*
    Use 2 pass scaling solution

    Display1 uses 2 pass scaling and Display2 used 1 pass scaling for performance consideration.
*/
void _AppDisp_MoviePlayView_2PassScalar(PIMG_BUF pSrc, PGXIMG_MULTI_OUT pMultiDst)
{
	IRECT             SrcRect = {0};
	IRECT             DstRect1 = {0}, DstRect2 = {0};
	SIZECONVERT_INFO  ScaleInfo = {0};
    PIMG_BUF          pScaleDst;
	UINT32            ImeBufWidth, ImeBufHeight;
	UINT32            tmp_buf_addr, tmp_buf_size;
	ISF_DATA          tmp_isf_data;
	BOOL              b_use_tmp_buf = FALSE;
	IMG_BUF           TmpImgBuf;
	ISF_RV            isf_rv;

    // Media Play has been closed
    if (gMoviePlay_StartPlay == FALSE) {
		return;
	}

    // Get src region
    SrcRect.x = 0;
	SrcRect.y = 0;
	SrcRect.w = pSrc->Width;
	SrcRect.h = pSrc->Height;

    MPV_DUMP("[MPV]SrcRect x=%d, y=%d, w=%d, h=%d\r\n",
                    SrcRect.x,
                    SrcRect.y,
                    SrcRect.w,
                    SrcRect.h);

    // Support 2 pass scaling
    if (g_DualVideo & DISPLAY_1) {
		// first Pass (Source Size -> (ImeBufWidth x ImeBufHeight))
		if (pMultiDst->pImg[0]->Width > PLAY_IMETMP_WIDTH || pMultiDst->pImg[0]->Height > PLAY_IMETMP_HEIGHT) {
			// Do 1st pass only on Large LCD Panel
			pScaleDst = pMultiDst->pImg[0];
		} else {
			ImeBufWidth  = ALIGN_CEIL_8(PLAY_IMETMP_WIDTH);
			ImeBufHeight = ALIGN_CEIL_8(PLAY_IMETMP_HEIGHT);

			tmp_buf_size = GxImg_CalcRequireSize(ImeBufWidth, ImeBufHeight, GX_IMAGE_PIXEL_FMT_YUV420_PACKED, GXIMAGE_LINEOFFSET_ALIGN(16));
			if (tmp_buf_size == 0) {
				DBG_ERR("Tmp buf size = 0\r\n");
				return;
			}

			tmp_buf_addr = ImageUnit_NewData(tmp_buf_size, &tmp_isf_data);
			if (tmp_buf_addr == 0) {
				DBG_ERR("Get buffer fail\r\n");
				return;
			} else {
				b_use_tmp_buf = TRUE;
			}

			GxImg_InitBuf(&TmpImgBuf, ImeBufWidth, ImeBufHeight, GX_IMAGE_PIXEL_FMT_YUV420_PACKED, GXIMAGE_LINEOFFSET_ALIGN(16), tmp_buf_addr, tmp_buf_size);

			pScaleDst = &TmpImgBuf;
		}

    	if (g_src_ratio.w == pSrc->Width) {
    		ScaleInfo.uiSrcWidth  = pSrc->Width;
    		ScaleInfo.uiSrcHeight = pSrc->Height;
    	} else {
    		ScaleInfo.uiSrcWidth  = g_src_ratio.w;
    		ScaleInfo.uiSrcHeight = g_src_ratio.h;
    	}
    	ScaleInfo.uiDstWidth  = pScaleDst->Width;
    	ScaleInfo.uiDstHeight = pScaleDst->Height;
    	ScaleInfo.uiDstWRatio = g_dev_ratio[0].w;
    	ScaleInfo.uiDstHRatio = g_dev_ratio[0].h;
        ScaleInfo.alignType   = SIZECONVERT_ALIGN_FLOOR_32;

    	DisplaySizeConvert(&ScaleInfo);

    	DstRect1.x = ScaleInfo.uiOutX;
    	DstRect1.y = ScaleInfo.uiOutY;
    	DstRect1.w = ScaleInfo.uiOutWidth;
    	DstRect1.h = ScaleInfo.uiOutHeight;

        MPV_DUMP("[MPV]DstRect1 x=%d, y=%d, w=%d, h=%d\r\n",
                        DstRect1.x,
                        DstRect1.y,
                        DstRect1.w,
                        DstRect1.h);

		if ((DstRect1.w * pScaleDst->Height) != (DstRect1.h * pScaleDst->Width)) { // If the ratio is not equal, need to fill dark screen
			GxImg_FillData(pScaleDst, NULL, COLOR_YUV_BLACK);
		}

		GxImg_ScaleDataFine(pSrc, &SrcRect, pScaleDst, &DstRect1); // (IMG_BUF, RECT, IMG_BUF, RECT)

		//
		// 2nd pass ((ImeBufWidth x ImeBufHeight)->Display Size)
		//
		if (pScaleDst != pMultiDst->pImg[0]) {
			SrcRect.x = 0;
			SrcRect.y = 0;
			SrcRect.w = pScaleDst->Width;
			SrcRect.h = pScaleDst->Height;

			/*if ((DstRect1.w * pScaleDst->Height) != (DstRect1.h * pScaleDst->Width)) { // If the ratio is not equal, need to fill dark screen
				GxImg_FillData(pMultiDst->pImg[0], NULL, COLOR_YUV_BLACK);
			}*/

			GxImg_ScaleDataFine(pScaleDst, &SrcRect, pMultiDst->pImg[0], NULL);
		}

		if (b_use_tmp_buf == TRUE) {
			isf_rv = ImageUnit_ReleaseData(&tmp_isf_data);
			if (isf_rv != ISF_OK) {
				DBG_ERR("release data fail\r\n");
				return;
			}
		}
    } // end of if(g_DualVideo & DISPLAY_1)

	// Only support 1 pass scaling
	if (g_DualVideo & DISPLAY_2) {
    	if (g_src_ratio.w == g_src_ratio.h) {
    		ScaleInfo.uiSrcWidth  = pSrc->Width;
    		ScaleInfo.uiSrcHeight = pSrc->Height;
    	} else {
    		ScaleInfo.uiSrcWidth  = g_src_ratio.w;
    		ScaleInfo.uiSrcHeight = g_src_ratio.h;
    	}
    	ScaleInfo.uiDstWidth  = pMultiDst->pImg[1]->Width;
    	ScaleInfo.uiDstHeight = pMultiDst->pImg[1]->Height;
        ScaleInfo.uiDstWRatio = g_dev_ratio[1].w;
    	ScaleInfo.uiDstHRatio = g_dev_ratio[1].h;
    	ScaleInfo.alignType   = SIZECONVERT_ALIGN_FLOOR_32;

    	DisplaySizeConvert(&ScaleInfo);

    	DstRect2.x = ScaleInfo.uiOutX;
    	DstRect2.y = ScaleInfo.uiOutY;
    	DstRect2.w = ScaleInfo.uiOutWidth;
    	DstRect2.h = ScaleInfo.uiOutHeight;

        MPV_DUMP("[MPV]DstRect2 x=%d, y=%d, w=%d, h=%d\r\n",
                        DstRect2.x,
                        DstRect2.y,
                        DstRect2.w,
                        DstRect2.h);

        pMultiDst->pRegion[1] = (PIRECT)&DstRect2;

		GxImg_ScaleDataFine(pSrc, &SrcRect, pMultiDst->pImg[1], NULL);
    } //// end of if(g_DualVideo & DISPLAY_2)
}
#endif
//#NT#2018/05/30#Adam Su -end

void _AppDisp_MoviePlayView_Close(void)
{
	UINT32 i = 0;

	if (g_DualVideo & DISPLAY_1) {
		for (i = 0; i < g_buf_num[0]; i++) {
			ImageUnit_ReleaseData(&g_isf_new_data[i]);
		}
		g_buf_idx[0] = 0;
	}

	if (g_DualVideo & DISPLAY_2) {
		for (i = 0; i < g_buf_num[1]; i++) {
			ImageUnit_ReleaseData(&g_isf_new_data2[i]);
		}
		g_buf_idx[1] = 0;
	}
}

void _AppDisp_MoviePlayView_PushData(ISF_PORT *pDst, PIMG_BUF pDataToPush, ISF_DATA* pIsfData)
{
    pIsfData->Sign = ISF_SIGN_DATA;
    memcpy(pIsfData->Desc, pDataToPush, sizeof(IMG_BUF));

    if (ImageUnit_IsAllowPush(pDst) == FALSE) {
		return;
	}

	ImageUnit_PushData(pDst, pIsfData, 0);
}

void _AppDisp_MoviePlayView_Trigger(void)
{
	ISF_UNIT         *pUnit;
	ISF_PORT         *pSrc = NULL, *pDst1 = NULL, *pDst2 = NULL;
    GXIMG_MULTI_OUT   MultiOut = {0};

	pUnit = &ISF_UserProc;

	if (g_DualVideo & DISPLAY_1) {
		pSrc = ImageUnit_Out(pUnit, ISF_OUT1);
	}
	if (g_DualVideo & DISPLAY_2) {
		pSrc = ImageUnit_Out(pUnit, ISF_OUT2);
	}

	if (ImageUnit_IsAllowPull(pSrc)) {
		// Pull data from UserProc
		g_isf_data[g_buf_idx[0]].Sign = ISF_SIGN_DATA;

		//#NT#2018/01/30#Adam Su
		//#NT#fix cim 106275 unchecked return value
		if (ImageUnit_PullData(pSrc, (ISF_DATA *)&g_isf_data[g_buf_idx[0]], 0) != ISF_OK) {
			//DBG_ERR("Pull data failed\r\n");
			return;
		}
		memcpy(&g_src_img, &g_isf_data[g_buf_idx[0]].Desc[0], sizeof(IMG_BUF)); // fix "pSrcImg is not initialized" problem

        // Get img to push
		if (g_DualVideo & DISPLAY_1) {
            if ((g_src_img.Width == (UINT32)g_dev_size[0].w) &&
                (g_src_img.Height == (UINT32)g_dev_size[0].h) &&
                (g_src_img.PxlFmt == g_dst_img[0][g_buf_idx[0]].PxlFmt)) {
                // Push src img directly
                pDst1 = ImageUnit_Out(pUnit, ISF_OUT1);
                _AppDisp_MoviePlayView_PushData(pDst1, &g_src_img, &g_isf_new_data[g_buf_idx[0]]);
    			g_buf_idx[0] = (g_buf_idx[0] + 1) % g_buf_num[0];
            } else {
                MultiOut.pImg[0] = &g_dst_img[0][g_buf_idx[0]];
            }
		}
        if (g_DualVideo & DISPLAY_2) {
            if ((g_src_img.Width == (UINT32)g_dev_size[1].w) &&
                (g_src_img.Height == (UINT32)g_dev_size[1].h) &&
                (g_src_img.PxlFmt == g_dst_img[1][g_buf_idx[1]].PxlFmt)) {
                // Push src img directly
                pDst2 = ImageUnit_Out(pUnit, ISF_OUT2);
                _AppDisp_MoviePlayView_PushData(pDst2, &g_src_img, &g_isf_new_data2[g_buf_idx[1]]);
    			g_buf_idx[1] = (g_buf_idx[1] + 1) % g_buf_num[1];
            } else {
                MultiOut.pImg[1] = &g_dst_img[1][g_buf_idx[1]];
            }
        }

        // Scale src to dst
        if (MultiOut.pImg[0] || MultiOut.pImg[1]) {
#if (_USE_2PASS_SCALE_ == ENABLE)
		_AppDisp_MoviePlayView_2PassScalar(&g_src_img, &MultiOut);
#else
        _AppDisp_MoviePlayView_Scalar2Display(&g_src_img, &MultiOut);
#endif
        }

        // Push dst img
        if ((g_DualVideo & DISPLAY_1) && MultiOut.pImg[0]) {
			pDst1 = ImageUnit_Out(pUnit, ISF_OUT1);
            _AppDisp_MoviePlayView_PushData(pDst1, MultiOut.pImg[0], &g_isf_new_data[g_buf_idx[0]]);
			g_buf_idx[0] = (g_buf_idx[0] + 1) % g_buf_num[0];
		}
        if ((g_DualVideo & DISPLAY_2) && MultiOut.pImg[1]) {
			pDst2 = ImageUnit_Out(pUnit, ISF_OUT2);
            _AppDisp_MoviePlayView_PushData(pDst2, MultiOut.pImg[1], &g_isf_new_data2[g_buf_idx[1]]);
			g_buf_idx[1] = (g_buf_idx[1] + 1) % g_buf_num[1];
        }
	}
}

void AppDisp_MoviePlayView_Init(USIZE *pDispAR)
{
	UINT32			 buff_addr[DISP_QUEUE_MAXSIZE];
	UINT32			 i = 0;
	UINT32			 img_buf_size = {0};

	if (pDispAR) {
		g_src_ratio = *pDispAR;
	} else {
		g_src_ratio.w = 1;
		g_src_ratio.h = 1;
	}

	MoviePlayView_InstallCmd();

	if (g_DualVideo & DISPLAY_1) {
        g_dev_size[0] = GxVideo_GetDeviceSize(DOUT1);
		g_dev_ratio[0] = GxVideo_GetDeviceAspect(DOUT1);

		MPV_DUMP("[MPV]OUT1 device_size w=%d, h=%d\r\n", g_dev_size[0].w, g_dev_size[0].h);

		// Check display queue number
		if (g_dev_size[0].w > 1920 || g_dev_size[0].h > 1080) { // 4K device size
			g_buf_num[0] = DISP_QUEUE_MAXSIZE;
		} else {
			g_buf_num[0] = DISP_QUEUE_DEFSIZE;
		}

		img_buf_size = GxImg_CalcRequireSize(g_dev_size[0].w, g_dev_size[0].h, GX_IMAGE_PIXEL_FMT_YUV420_PACKED, GXIMAGE_LINEOFFSET_ALIGN(16));

		// Get common buffer
		for (i = 0; i < g_buf_num[0]; i++) {
			buff_addr[i] = ImageUnit_NewData(img_buf_size, &g_isf_new_data[i]);
			if (buff_addr[i] == 0) {
				DBG_ERR("OUT1 Get buffer[%d] fail\r\n", i);
				return;
			}
			GxImg_InitBuf(&g_dst_img[0][i], g_dev_size[0].w, g_dev_size[0].h, GX_IMAGE_PIXEL_FMT_YUV420_PACKED, GXIMAGE_LINEOFFSET_ALIGN(16), buff_addr[i], img_buf_size);
			GxImg_FillData(&g_dst_img[0][i], NULL, COLOR_YUV_BLACK);
		}
	}
	if (g_DualVideo & DISPLAY_2) {
        g_dev_size[1] = GxVideo_GetDeviceSize(DOUT2);
		g_dev_ratio[1] = GxVideo_GetDeviceAspect(DOUT2);

		MPV_DUMP("[MPV]OUT2 device_size w=%d, h=%d\r\n", g_dev_size[1].w, g_dev_size[1].h);

		g_buf_num[1] = DISP_QUEUE_MAXSIZE;

		img_buf_size = GxImg_CalcRequireSize(g_dev_size[1].w, g_dev_size[1].h, GX_IMAGE_PIXEL_FMT_YUV420_PACKED, GXIMAGE_LINEOFFSET_ALIGN(16));

		// Get common buffer
		for (i = 0; i < g_buf_num[1]; i++) {
			buff_addr[i] = ImageUnit_NewData(img_buf_size, &g_isf_new_data2[i]);
			if (buff_addr[i] == 0) {
				DBG_ERR("OUT2 Get buffer[%d] fail\r\n", i);
				return;
			}
			GxImg_InitBuf(&g_dst_img[1][i], g_dev_size[1].w, g_dev_size[1].h, GX_IMAGE_PIXEL_FMT_YUV420_PACKED, GXIMAGE_LINEOFFSET_ALIGN(16), buff_addr[i], img_buf_size);
			GxImg_FillData(&g_dst_img[1][i], NULL, COLOR_YUV_BLACK);
		}
	}

	gMoviePlay_StartPlay = TRUE;
}

void AppDisp_MoviePlayView_Tsk(void)
{
	FLGPTN          uiFlag = 0, waitFlag = 0;

	MPV_DUMP("[MPV]task start\r\n");

	kent_tsk();

	clr_flg(APPDISP_MOVIEPLAYVIEW_FLG_ID, FLG_APPDISP_MOVIEPLAYVIEW_IDLE);
	waitFlag = FLG_APPDISP_MOVIEPLAYVIEW_CLOSE |
			   FLG_APPDISP_MOVIEPLAYVIEW_TRIG;

	while (APPDISP_MOVIEPLAYVIEW_TSK_ID) {
		set_flg(APPDISP_MOVIEPLAYVIEW_FLG_ID, FLG_APPDISP_MOVIEPLAYVIEW_IDLE);
		PROFILE_TASK_IDLE();
		wai_flg(&uiFlag, APPDISP_MOVIEPLAYVIEW_FLG_ID, waitFlag, TWF_ORW | TWF_CLR);
		PROFILE_TASK_BUSY();
		clr_flg(APPDISP_MOVIEPLAYVIEW_FLG_ID, FLG_APPDISP_MOVIEPLAYVIEW_IDLE);

		MPV_DUMP("[MPV]uiFlag=0x%x\r\n", uiFlag);

		if (uiFlag & FLG_APPDISP_MOVIEPLAYVIEW_CLOSE) {
			_AppDisp_MoviePlayView_Close();
		}

		if (uiFlag & FLG_APPDISP_MOVIEPLAYVIEW_TRIG) {
			if (gMoviePlay_StartPlay == TRUE) {
				_AppDisp_MoviePlayView_Trigger();
			}
		}
	}
	ext_tsk();
}

void AppDisp_MoviePlayView_Open(void)
{
	if (g_mpv_is_open == TRUE) {
		DBG_WRN("Already open\r\n");
		return;
	}

	if (!APPDISP_MOVIEPLAYVIEW_FLG_ID) {
		DBG_ERR("ID not installed\r\n");
		return;
	}

	clr_flg(APPDISP_MOVIEPLAYVIEW_FLG_ID, 0xFFFFFFFF);
	sta_tsk(APPDISP_MOVIEPLAYVIEW_TSK_ID, 0);

	g_mpv_is_open = TRUE;
}

void AppDisp_MoviePlayView_Close(void)
{
	FLGPTN          uiFlag = 0;

	if (g_mpv_is_open == FALSE) {
		DBG_ERR("No need to close\r\n");
		return;
	}

	if (!APPDISP_MOVIEPLAYVIEW_FLG_ID) {
		DBG_ERR("ID not installed\r\n");
		return;
	}
	set_flg(APPDISP_MOVIEPLAYVIEW_FLG_ID, FLG_APPDISP_MOVIEPLAYVIEW_CLOSE);
	wai_flg(&uiFlag, APPDISP_MOVIEPLAYVIEW_FLG_ID, FLG_APPDISP_MOVIEPLAYVIEW_IDLE, TWF_ORW);
	ter_tsk(APPDISP_MOVIEPLAYVIEW_TSK_ID);

	g_mpv_is_open = FALSE;
}

void AppDisp_MoviePlayView_OnDraw(void)
{
	set_flg(APPDISP_MOVIEPLAYVIEW_FLG_ID, FLG_APPDISP_MOVIEPLAYVIEW_TRIG);
}

#else // (NMEDIAPLAY_FUNC == ENABLE)
static ISF_VIDEO_STREAM_BUF g_src_strm_buf = {0};
static UINT32 g_curr_frm_index = 0;
static UINT32 g_old_sec = 0, g_curr_sec = 0;

void _AppDisp_MoviePlayView_Scalar2Display(PIMG_BUF pSrc, PGXIMG_MULTI_OUT pMultiDst)
{
	IRECT             SrcRect = {0};
	IRECT             DstRect1 = {0}, DstRect2 = {0};
	SIZECONVERT_INFO  ScaleInfo = {0};

    // Media Play has been closed
    if (gMoviePlay_StartPlay == FALSE) {
		return;
	}

    // Get src region
    SrcRect.x = 0;
	SrcRect.y = 0;
	SrcRect.w = pSrc->Width;
	SrcRect.h = pSrc->Height;

    MPV_DUMP("[MPV]SrcRect x=%d, y=%d, w=%d, h=%d\r\n",
                    SrcRect.x,
                    SrcRect.y,
                    SrcRect.w,
                    SrcRect.h);

    // Calculate dst region
    if (g_DualVideo & DISPLAY_1) {
    	if (g_src_ratio.w == g_src_ratio.h) {
    		ScaleInfo.uiSrcWidth  = pSrc->Width;
    		ScaleInfo.uiSrcHeight = pSrc->Height;
    	} else {
    		ScaleInfo.uiSrcWidth  = g_src_ratio.w;
    		ScaleInfo.uiSrcHeight = g_src_ratio.h;
    	}
    	ScaleInfo.uiDstWidth  = pMultiDst->pImg[0]->Width;
    	ScaleInfo.uiDstHeight = pMultiDst->pImg[0]->Height;
    	ScaleInfo.uiDstWRatio = g_dev_ratio[0].w;
    	ScaleInfo.uiDstHRatio = g_dev_ratio[0].h;
        ScaleInfo.alignType   = SIZECONVERT_ALIGN_FLOOR_32;

    	DisplaySizeConvert(&ScaleInfo);

    	DstRect1.x = ScaleInfo.uiOutX;
    	DstRect1.y = ScaleInfo.uiOutY;
    	DstRect1.w = ScaleInfo.uiOutWidth;
    	DstRect1.h = ScaleInfo.uiOutHeight;

        MPV_DUMP("[MPV]DstRect1 x=%d, y=%d, w=%d, h=%d\r\n",
                        DstRect1.x,
                        DstRect1.y,
                        DstRect1.w,
                        DstRect1.h);

        pMultiDst->pRegion[0] = (PIRECT)&DstRect1;
    }
    if (g_DualVideo & DISPLAY_2) {
    	if (g_src_ratio.w == g_src_ratio.h) {
    		ScaleInfo.uiSrcWidth  = pSrc->Width;
    		ScaleInfo.uiSrcHeight = pSrc->Height;
    	} else {
    		ScaleInfo.uiSrcWidth  = g_src_ratio.w;
    		ScaleInfo.uiSrcHeight = g_src_ratio.h;
    	}
    	ScaleInfo.uiDstWidth  = pMultiDst->pImg[1]->Width;
    	ScaleInfo.uiDstHeight = pMultiDst->pImg[1]->Height;
        ScaleInfo.uiDstWRatio = g_dev_ratio[1].w;
    	ScaleInfo.uiDstHRatio = g_dev_ratio[1].h;
    	ScaleInfo.alignType   = SIZECONVERT_ALIGN_FLOOR_32;

    	DisplaySizeConvert(&ScaleInfo);

    	DstRect2.x = ScaleInfo.uiOutX;
    	DstRect2.y = ScaleInfo.uiOutY;
    	DstRect2.w = ScaleInfo.uiOutWidth;
    	DstRect2.h = ScaleInfo.uiOutHeight;

        MPV_DUMP("[MPV]DstRect2 x=%d, y=%d, w=%d, h=%d\r\n",
                        DstRect2.x,
                        DstRect2.y,
                        DstRect2.w,
                        DstRect2.h);

        pMultiDst->pRegion[1] = (PIRECT)&DstRect2;
    }

    GxImg_ScaleMultiOut(pSrc, &SrcRect, pMultiDst);
}

//#NT#2018/05/31#Adam Su -begin
//#NT#2-pass flow
#if (_USE_2PASS_SCALE_ == ENABLE)
/*
    Use 2 pass scaling solution

    Display1 uses 2 pass scaling and Display2 used 1 pass scaling for performance consideration.
*/
void _AppDisp_MoviePlayView_2PassScalar(PIMG_BUF pSrc, PGXIMG_MULTI_OUT pMultiDst)
{
	IRECT             SrcRect = {0};
	IRECT             DstRect1 = {0}, DstRect2 = {0};
	SIZECONVERT_INFO  ScaleInfo = {0};
    PIMG_BUF          pScaleDst;
	UINT32            ImeBufWidth, ImeBufHeight;
	UINT32            tmp_buf_addr, tmp_buf_size;
	ISF_DATA          tmp_isf_data;
	BOOL              b_use_tmp_buf = FALSE;
	IMG_BUF           TmpImgBuf;
	ISF_RV            isf_rv;

    // Media Play has been closed
    if (gMoviePlay_StartPlay == FALSE) {
		return;
	}

    // Get src region
    SrcRect.x = 0;
	SrcRect.y = 0;
	SrcRect.w = pSrc->Width;
	SrcRect.h = pSrc->Height;

    MPV_DUMP("[MPV]SrcRect x=%d, y=%d, w=%d, h=%d\r\n",
                    SrcRect.x,
                    SrcRect.y,
                    SrcRect.w,
                    SrcRect.h);

    // Support 2 pass scaling
    if (g_DualVideo & DISPLAY_1) {
		// first Pass (Source Size -> (ImeBufWidth x ImeBufHeight))
		if (pMultiDst->pImg[0]->Width > PLAY_IMETMP_WIDTH || pMultiDst->pImg[0]->Height > PLAY_IMETMP_HEIGHT) {
			// Do 1st pass only on Large LCD Panel
			pScaleDst = pMultiDst->pImg[0];
		} else {
			ImeBufWidth  = ALIGN_CEIL_8(PLAY_IMETMP_WIDTH);
			ImeBufHeight = ALIGN_CEIL_8(PLAY_IMETMP_HEIGHT);

			tmp_buf_size = GxImg_CalcRequireSize(ImeBufWidth, ImeBufHeight, GX_IMAGE_PIXEL_FMT_YUV420_PACKED, GXIMAGE_LINEOFFSET_ALIGN(16));
			if (tmp_buf_size == 0) {
				DBG_ERR("Tmp buf size = 0\r\n");
				return;
			}

			tmp_buf_addr = ImageUnit_NewData(tmp_buf_size, &tmp_isf_data);
			if (tmp_buf_addr == 0) {
				DBG_ERR("Get buffer fail\r\n");
				return;
			} else {
				b_use_tmp_buf = TRUE;
			}

			GxImg_InitBuf(&TmpImgBuf, ImeBufWidth, ImeBufHeight, GX_IMAGE_PIXEL_FMT_YUV420_PACKED, GXIMAGE_LINEOFFSET_ALIGN(16), tmp_buf_addr, tmp_buf_size);

			pScaleDst = &TmpImgBuf;
		}

    	if (g_src_ratio.w == pSrc->Width) {
    		ScaleInfo.uiSrcWidth  = pSrc->Width;
    		ScaleInfo.uiSrcHeight = pSrc->Height;
    	} else {
    		ScaleInfo.uiSrcWidth  = g_src_ratio.w;
    		ScaleInfo.uiSrcHeight = g_src_ratio.h;
    	}
    	ScaleInfo.uiDstWidth  = pScaleDst->Width;
    	ScaleInfo.uiDstHeight = pScaleDst->Height;
    	ScaleInfo.uiDstWRatio = g_dev_ratio[0].w;
    	ScaleInfo.uiDstHRatio = g_dev_ratio[0].h;
        ScaleInfo.alignType   = SIZECONVERT_ALIGN_FLOOR_32;

    	DisplaySizeConvert(&ScaleInfo);

    	DstRect1.x = ScaleInfo.uiOutX;
    	DstRect1.y = ScaleInfo.uiOutY;
    	DstRect1.w = ScaleInfo.uiOutWidth;
    	DstRect1.h = ScaleInfo.uiOutHeight;

        MPV_DUMP("[MPV]DstRect1 x=%d, y=%d, w=%d, h=%d\r\n",
                        DstRect1.x,
                        DstRect1.y,
                        DstRect1.w,
                        DstRect1.h);

		if ((DstRect1.w * pScaleDst->Height) != (DstRect1.h * pScaleDst->Width)) { // If the ratio is not equal, need to fill dark screen
			GxImg_FillData(pScaleDst, NULL, COLOR_YUV_BLACK);
		}

		GxImg_ScaleDataFine(pSrc, &SrcRect, pScaleDst, &DstRect1); // (IMG_BUF, RECT, IMG_BUF, RECT)

		//
		// 2nd pass ((ImeBufWidth x ImeBufHeight)->Display Size)
		//
		if (pScaleDst != pMultiDst->pImg[0]) {
			SrcRect.x = 0;
			SrcRect.y = 0;
			SrcRect.w = pScaleDst->Width;
			SrcRect.h = pScaleDst->Height;

			/*if ((DstRect1.w * pScaleDst->Height) != (DstRect1.h * pScaleDst->Width)) { // If the ratio is not equal, need to fill dark screen
				GxImg_FillData(pMultiDst->pImg[0], NULL, COLOR_YUV_BLACK);
			}*/

			GxImg_ScaleDataFine(pScaleDst, &SrcRect, pMultiDst->pImg[0], NULL);
		}

		if (b_use_tmp_buf == TRUE) {
			isf_rv = ImageUnit_ReleaseData(&tmp_isf_data);
			if (isf_rv != ISF_OK) {
				DBG_ERR("release data fail\r\n");
				return;
			}
		}
    } // end of if(g_DualVideo & DISPLAY_1)

	// Only support 1 pass scaling
	if (g_DualVideo & DISPLAY_2) {
    	if (g_src_ratio.w == g_src_ratio.h) {
    		ScaleInfo.uiSrcWidth  = pSrc->Width;
    		ScaleInfo.uiSrcHeight = pSrc->Height;
    	} else {
    		ScaleInfo.uiSrcWidth  = g_src_ratio.w;
    		ScaleInfo.uiSrcHeight = g_src_ratio.h;
    	}
    	ScaleInfo.uiDstWidth  = pMultiDst->pImg[1]->Width;
    	ScaleInfo.uiDstHeight = pMultiDst->pImg[1]->Height;
        ScaleInfo.uiDstWRatio = g_dev_ratio[1].w;
    	ScaleInfo.uiDstHRatio = g_dev_ratio[1].h;
    	ScaleInfo.alignType   = SIZECONVERT_ALIGN_FLOOR_32;

    	DisplaySizeConvert(&ScaleInfo);

    	DstRect2.x = ScaleInfo.uiOutX;
    	DstRect2.y = ScaleInfo.uiOutY;
    	DstRect2.w = ScaleInfo.uiOutWidth;
    	DstRect2.h = ScaleInfo.uiOutHeight;

        MPV_DUMP("[MPV]DstRect2 x=%d, y=%d, w=%d, h=%d\r\n",
                        DstRect2.x,
                        DstRect2.y,
                        DstRect2.w,
                        DstRect2.h);

        pMultiDst->pRegion[1] = (PIRECT)&DstRect2;

		GxImg_ScaleDataFine(pSrc, &SrcRect, pMultiDst->pImg[1], NULL);
    } //// end of if(g_DualVideo & DISPLAY_2)
}
#endif
//#NT#2018/05/30#Adam Su -end

void _AppDisp_MoviePlayView_Close(void)
{
	UINT32 i = 0;

	if (g_DualVideo & DISPLAY_1) {
		for (i = 0; i < g_buf_num[0]; i++) {
			ImageUnit_ReleaseData(&g_isf_new_data[i]);
		}
		g_buf_idx[0] = 0;
	}

	if (g_DualVideo & DISPLAY_2) {
		for (i = 0; i < g_buf_num[1]; i++) {
			ImageUnit_ReleaseData(&g_isf_new_data2[i]);
		}
		g_buf_idx[1] = 0;
	}
}

void _AppDisp_MoviePlayView_CheckOneSecond(void) {
	if (g_curr_frm_index == 0) {
		g_old_sec = 0;
	}

	g_curr_sec = AppDisp_MoviePlayView_GetCurrPlaySecs();
	MPV_DUMP("[MPV]ChkOneSec i=%d, cur=%d\r\n", g_curr_frm_index, g_curr_sec);

	if (g_curr_sec != g_old_sec) {
		MPV_DUMP("[MPV]one sec arrives\r\n");
		g_old_sec = g_curr_sec;
		// post event to update time info icon
		Ux_PostEvent(NVTEVT_CB_MOVIE_ONE_SEC, 0);
	}
}

void _AppDisp_MoviePlayView_PushData(ISF_PORT *pDst, PIMG_BUF pDataToPush, ISF_DATA* pIsfData)
{
    pIsfData->Sign = ISF_SIGN_DATA;
    memcpy(pIsfData->Desc, pDataToPush, sizeof(IMG_BUF));

    if (ImageUnit_IsAllowPush(pDst) == FALSE) {
		return;
	}

	ImageUnit_PushData(pDst, pIsfData, 0);
}

void _AppDisp_MoviePlayView_Trigger(void)
{
	ISF_UNIT         *pUnit;
	ISF_PORT         *pSrc = NULL, *pDst1 = NULL, *pDst2 = NULL;
    GXIMG_MULTI_OUT   MultiOut = {0};
	UINT32            uiLineOff[3] = {0};
	UINT32            PxlAddr[3] = {0};
	UINT32            bIsEOF = 0;

	pUnit = &ISF_UserProc;

	if (g_DualVideo & DISPLAY_1) {
		pSrc = ImageUnit_Out(pUnit, ISF_OUT1);
	}
	if (g_DualVideo & DISPLAY_2) {
		pSrc = ImageUnit_Out(pUnit, ISF_OUT2);
	}

	if (ImageUnit_IsAllowPull(pSrc)) {
		// Pull data from UserProc
		g_isf_data[g_buf_idx[0]].Sign = ISF_SIGN_DATA;

		//#NT#2018/01/30#Adam Su
		//#NT#fix cim 106275 unchecked return value
		if (ImageUnit_PullData(pSrc, (ISF_DATA *)&g_isf_data[g_buf_idx[0]], 0) != ISF_OK) {
			//DBG_ERR("Pull data failed\r\n");
			return;
		}
		memcpy(&g_src_strm_buf, &g_isf_data[g_buf_idx[0]].Desc[0], sizeof(IMG_BUF)); // fix "pSrcImg is not initialized" problem

		// Init source buffer
		uiLineOff[0] = uiLineOff[1] = uiLineOff[2] = ALIGN_CEIL_64(g_src_strm_buf.Width); // Algien 64-lines base for HW Codec limitation
		PxlAddr[0]   = PxlAddr[1]   = PxlAddr[2]   = 0; // Set Null first
		GxImg_InitBufEx(&g_src_img,
						g_src_strm_buf.Width,
						g_src_strm_buf.Height,
						GX_IMAGE_PIXEL_FMT_YUV420_PACKED,
						uiLineOff,
						PxlAddr);
		g_src_img.PxlAddr[0] = g_src_strm_buf.Resv[0];
		g_src_img.PxlAddr[1] = g_src_strm_buf.Resv[1];
		g_src_img.PxlAddr[2] = 0;

		// update current frame index
		g_curr_frm_index = g_src_strm_buf.Resv[2];

		// check end of frame
		bIsEOF = g_src_strm_buf.Resv[3];
		if (bIsEOF == TRUE) {
			DBG_DUMP("movie play finish!\r\n");
			gMoviePlay_StartPlay = FALSE; // stop MPV trigger
			Ux_PostEvent(NVTEVT_CB_MOVIE_FINISH, 0); // post event movie finish
		}

		// check one second arriving
		_AppDisp_MoviePlayView_CheckOneSecond();

        // Get img to push
		if (g_DualVideo & DISPLAY_1) {
            if ((g_src_img.Width == (UINT32)g_dev_size[0].w) &&
                (g_src_img.Height == (UINT32)g_dev_size[0].h) &&
                (g_src_img.PxlFmt == g_dst_img[0][g_buf_idx[0]].PxlFmt)) {
                // Push src img directly
                pDst1 = ImageUnit_Out(pUnit, ISF_OUT1);
                _AppDisp_MoviePlayView_PushData(pDst1, &g_src_img, &g_isf_new_data[g_buf_idx[0]]);
    			g_buf_idx[0] = (g_buf_idx[0] + 1) % g_buf_num[0];
            } else {
                MultiOut.pImg[0] = &g_dst_img[0][g_buf_idx[0]];
            }
		}
        if (g_DualVideo & DISPLAY_2) {
            if ((g_src_img.Width == (UINT32)g_dev_size[1].w) &&
                (g_src_img.Height == (UINT32)g_dev_size[1].h) &&
                (g_src_img.PxlFmt == g_dst_img[1][g_buf_idx[1]].PxlFmt)) {
                // Push src img directly
                pDst2 = ImageUnit_Out(pUnit, ISF_OUT2);
                _AppDisp_MoviePlayView_PushData(pDst2, &g_src_img, &g_isf_new_data2[g_buf_idx[1]]);
    			g_buf_idx[1] = (g_buf_idx[1] + 1) % g_buf_num[1];
            } else {
                MultiOut.pImg[1] = &g_dst_img[1][g_buf_idx[1]];
            }
        }

        // Scale src to dst
#if (_USE_2PASS_SCALE_ == ENABLE)
		_AppDisp_MoviePlayView_2PassScalar(&g_src_img, &MultiOut);
#else
        _AppDisp_MoviePlayView_Scalar2Display(&g_src_img, &MultiOut);
#endif

        // Push dst img
        if ((g_DualVideo & DISPLAY_1) && MultiOut.pImg[0]) {
			pDst1 = ImageUnit_Out(pUnit, ISF_OUT1);
            _AppDisp_MoviePlayView_PushData(pDst1, MultiOut.pImg[0], &g_isf_new_data[g_buf_idx[0]]);
			g_buf_idx[0] = (g_buf_idx[0] + 1) % g_buf_num[0];
		}
        if ((g_DualVideo & DISPLAY_2) && MultiOut.pImg[1]) {
			pDst2 = ImageUnit_Out(pUnit, ISF_OUT2);
            _AppDisp_MoviePlayView_PushData(pDst2, MultiOut.pImg[1], &g_isf_new_data2[g_buf_idx[1]]);
			g_buf_idx[1] = (g_buf_idx[1] + 1) % g_buf_num[1];
        }
	}
}

void AppDisp_MoviePlayView_Init(USIZE *pDispAR)
{
	UINT32			 buff_addr[DISP_QUEUE_MAXSIZE];
	UINT32			 i = 0;
	UINT32			 img_buf_size = {0};

	if (pDispAR) {
		g_src_ratio = *pDispAR;
	} else {
		g_src_ratio.w = 1;
		g_src_ratio.h = 1;
	}

	MoviePlayView_InstallCmd();

	if (g_DualVideo & DISPLAY_1) {
        g_dev_size[0] = GxVideo_GetDeviceSize(DOUT1);
		g_dev_ratio[0] = GxVideo_GetDeviceAspect(DOUT1);

		MPV_DUMP("[MPV]OUT1 device_size w=%d, h=%d\r\n", g_dev_size[0].w, g_dev_size[0].h);

		// Check display queue number
		if (g_dev_size[0].w > 1920 || g_dev_size[0].h > 1080) { // 4K device size
			g_buf_num[0] = DISP_QUEUE_MAXSIZE;
		} else {
			g_buf_num[0] = DISP_QUEUE_DEFSIZE;
		}

		img_buf_size = GxImg_CalcRequireSize(g_dev_size[0].w, g_dev_size[0].h, GX_IMAGE_PIXEL_FMT_YUV420_PACKED, GXIMAGE_LINEOFFSET_ALIGN(16));

		// Get common buffer
		for (i = 0; i < g_buf_num[0]; i++) {
			buff_addr[i] = ImageUnit_NewData(img_buf_size, &g_isf_new_data[i]);
			if (buff_addr[i] == 0) {
				DBG_ERR("OUT1 Get buffer[%d] fail\r\n", i);
				return;
			}
			GxImg_InitBuf(&g_dst_img[0][i], g_dev_size[0].w, g_dev_size[0].h, GX_IMAGE_PIXEL_FMT_YUV420_PACKED, GXIMAGE_LINEOFFSET_ALIGN(16), buff_addr[i], img_buf_size);
			GxImg_FillData(&g_dst_img[0][i], NULL, COLOR_YUV_BLACK);
		}
	}
	if (g_DualVideo & DISPLAY_2) {
        g_dev_size[1] = GxVideo_GetDeviceSize(DOUT2);
		g_dev_ratio[1] = GxVideo_GetDeviceAspect(DOUT2);

		MPV_DUMP("[MPV]OUT2 device_size w=%d, h=%d\r\n", g_dev_size[1].w, g_dev_size[1].h);

		g_buf_num[1] = DISP_QUEUE_MAXSIZE;

		img_buf_size = GxImg_CalcRequireSize(g_dev_size[1].w, g_dev_size[1].h, GX_IMAGE_PIXEL_FMT_YUV420_PACKED, GXIMAGE_LINEOFFSET_ALIGN(16));

		// Get common buffer
		for (i = 0; i < g_buf_num[1]; i++) {
			buff_addr[i] = ImageUnit_NewData(img_buf_size, &g_isf_new_data2[i]);
			if (buff_addr[i] == 0) {
				DBG_ERR("OUT2 Get buffer[%d] fail\r\n", i);
				return;
			}
			GxImg_InitBuf(&g_dst_img[1][i], g_dev_size[1].w, g_dev_size[1].h, GX_IMAGE_PIXEL_FMT_YUV420_PACKED, GXIMAGE_LINEOFFSET_ALIGN(16), buff_addr[i], img_buf_size);
			GxImg_FillData(&g_dst_img[1][i], NULL, COLOR_YUV_BLACK);
		}
	}

	gMoviePlay_StartPlay = TRUE;
}

void AppDisp_MoviePlayView_Tsk(void)
{
	FLGPTN          uiFlag = 0, waitFlag = 0;

	MPV_DUMP("[MPV]task start\r\n");

	kent_tsk();

	clr_flg(APPDISP_MOVIEPLAYVIEW_FLG_ID, FLG_APPDISP_MOVIEPLAYVIEW_IDLE);
	waitFlag = FLG_APPDISP_MOVIEPLAYVIEW_CLOSE |
			   FLG_APPDISP_MOVIEPLAYVIEW_TRIG;

	while (APPDISP_MOVIEPLAYVIEW_TSK_ID) {
		set_flg(APPDISP_MOVIEPLAYVIEW_FLG_ID, FLG_APPDISP_MOVIEPLAYVIEW_IDLE);
		PROFILE_TASK_IDLE();
		wai_flg(&uiFlag, APPDISP_MOVIEPLAYVIEW_FLG_ID, waitFlag, TWF_ORW | TWF_CLR);
		PROFILE_TASK_BUSY();
		clr_flg(APPDISP_MOVIEPLAYVIEW_FLG_ID, FLG_APPDISP_MOVIEPLAYVIEW_IDLE);

		MPV_DUMP("[MPV]uiFlag=0x%x\r\n", uiFlag);

		if (uiFlag & FLG_APPDISP_MOVIEPLAYVIEW_CLOSE) {
			_AppDisp_MoviePlayView_Close();
		}

		if (uiFlag & FLG_APPDISP_MOVIEPLAYVIEW_TRIG) {
			if (gMoviePlay_StartPlay == TRUE) {
				_AppDisp_MoviePlayView_Trigger();
			}
		}
	}
	ext_tsk();
}

void AppDisp_MoviePlayView_Open(void)
{
	if (g_mpv_is_open == TRUE) {
		DBG_WRN("Already open\r\n");
		return;
	}

	if (!APPDISP_MOVIEPLAYVIEW_FLG_ID) {
		DBG_ERR("ID not installed\r\n");
		return;
	}

	clr_flg(APPDISP_MOVIEPLAYVIEW_FLG_ID, 0xFFFFFFFF);
	sta_tsk(APPDISP_MOVIEPLAYVIEW_TSK_ID, 0);

	g_mpv_is_open = TRUE;
}

void AppDisp_MoviePlayView_Close(void)
{
	FLGPTN          uiFlag = 0;

	if (g_mpv_is_open == FALSE) {
		DBG_ERR("No need to close\r\n");
		return;
	}

	if (!APPDISP_MOVIEPLAYVIEW_FLG_ID) {
		DBG_ERR("ID not installed\r\n");
		return;
	}
	set_flg(APPDISP_MOVIEPLAYVIEW_FLG_ID, FLG_APPDISP_MOVIEPLAYVIEW_CLOSE);
	wai_flg(&uiFlag, APPDISP_MOVIEPLAYVIEW_FLG_ID, FLG_APPDISP_MOVIEPLAYVIEW_IDLE, TWF_ORW);
	ter_tsk(APPDISP_MOVIEPLAYVIEW_TSK_ID);

	g_curr_frm_index = 0;
	g_mpv_is_open = FALSE;
}

void AppDisp_MoviePlayView_OnDraw(void)
{
	set_flg(APPDISP_MOVIEPLAYVIEW_FLG_ID, FLG_APPDISP_MOVIEPLAYVIEW_TRIG);
}

/**
    Get current playing time of the media file

    @return UINT32 current playing time of the media file
*/
UINT32 AppDisp_MoviePlayView_GetCurrPlaySecs(void)
{
	UINT32 play_time;
	UINT32 vdo_fr = ImageApp_MoviePlay_GetParam(MOVIEPLAY_NM_PARAM_VDO_FR);

	if (vdo_fr == 0) {
		DBG_ERR("Video frame rate = 0\r\n");
		return 0;
	}

	play_time = (UINT32)((g_curr_frm_index + 1) / vdo_fr);

	return play_time;
}

/**
    Get current display index

    @return UINT32 current display index
*/
UINT32 AppDisp_MoviePlayView_GetDispIdx(void)
{
	return g_curr_frm_index;
}
#endif

static BOOL Cmd_MoviePlayView_SetDebug(CHAR *strCmd)
{
	UINT32 dbg_msg;

	sscanf_s(strCmd, "%d", &dbg_msg);

	g_movieplayview_debug = (dbg_msg > 0);

	return TRUE;
}

SXCMD_BEGIN(mpview, "Movie play view command")
SXCMD_ITEM("dbg %d",      Cmd_MoviePlayView_SetDebug,        "set debug level (1->on, 0->off)")
SXCMD_END()

static void MoviePlayView_InstallCmd(void)
{
	SxCmd_AddTable(mpview);
}


