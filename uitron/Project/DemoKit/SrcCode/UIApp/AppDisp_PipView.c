#include "Color.h"
#include <stdio.h>
#include "SysCfg.h"
#include "UIInfo.h"
#include "AppDisp_PipView.h"

#define THIS_DBGLVL         2       // 0=FATAL, 1=ERR, 2=WRN, 3=UNIT, 4=FUNC, 5=IND, 6=MSG, 7=VALUE, 8=USER
#define __MODULE__          Pip
#define __DBGLVL__          THIS_DBGLVL
#define __DBGFLT__          "*" //*=All, [mark]=CustomClass
#include "DebugModule.h"

#if (SENSOR_CAPS_COUNT == 2)
static APPDISP_VIEW_INFO   pip_view_info={
							 0x00010001, 0x00010001 , // w 1/1    h 1/1
                      		 0x00010002, 0x00010002 , // w 1/2    h 1/2
					  		 0x00010002, 0x00010002 , // w 1/2    h 1/2
							 0x00010002, 0x00010002 , // w 1/2    h 1/2
#else
static APPDISP_VIEW_INFO   pip_view_info={
							 0x00010002, 0x00010002 , // w 1/2    h 1/2
                      		 0x00010002, 0x00010002 , // w 1/2    h 1/2
					  		 0x00010002, 0x00010002 , // w 1/2    h 1/2
							 0x00010002, 0x00010002 , // w 1/2    h 1/2
#endif

};


#if 0 //(SBS_VIEW_FUNC == ENABLE)
static void PipLRView_SetStyle(UINT32 uiStyle)
{
	pip_scale_info_4sensor.scale[0].w = 0x00010002;
	pip_scale_info_4sensor.scale[0].h = 0x00010001;
	pip_scale_info_4sensor.scale[1].w = 0x00010002; // w 1/2
	pip_scale_info_4sensor.scale[1].h = 0x00010001; // h 1/1
}

static INT32 PipLRView_OnDraw_2sensor(PIP_DRAW *pDraw) //PIP = Picture In Picture
{
	IPOINT dstLos = {0, 0};
	//IRECT            dst_region;

    #if 1
    {
        static BOOL isSave = FALSE;

		if (!isSave && pDraw->p_src_img[0]->Width > 1000) {
			GxImg_DumpBuf("A:\\src1.yuv", pDraw->p_src_img[0], GX_IMAGE_DUMP_ALL);
			GxImg_DumpBuf("A:\\src2.yuv", pDraw->p_src_img[1], GX_IMAGE_DUMP_ALL);
			DBG_DUMP("Saved\r\n");
			isSave = TRUE;
		}
    }
	#endif

    //GxImg_FillData(pDraw->p_dst_img, REGION_MATCH_IMG, COLOR_YUV_BLACK);
	// img1
	//dst_region.x = 0;
	//dst_region.y = pDraw->p_dst_img->Height /4;
	//dst_region.w = pDraw->p_dst_img->Width / 2;
	//dst_region.h = pDraw->p_dst_img->Height / 2;
	GxImg_CopyData(pDraw->p_src_img[0], REGION_MATCH_IMG, pDraw->p_dst_img, &dstLos);

    // img2
    //dst_region.x = pDraw->p_dst_img->Width / 2;
	//dst_region.y = pDraw->p_dst_img->Height / 4;
	dstLos.x = pDraw->p_dst_img->Width / 2;
	GxImg_CopyData(pDraw->p_src_img[1], REGION_MATCH_IMG, pDraw->p_dst_img, &dstLos);

	return E_OK;
}
#endif



void PipView_SetImg(ISIZE *pImgSize, USIZE *pAspectRatio)
{

}

void PipView_SetStyle(UINT32 uiStyle)
{

	#if (SENSOR_CAPS_COUNT == 2)
	pip_view_info.scale[0].w = 0x00010001;
	pip_view_info.scale[0].h = 0x00010001;
	pip_view_info.scale[1].w = 0x00010002; // w 1/2
	pip_view_info.scale[1].h = 0x00010002; // h 1/2
	#else

	#endif

}


INT32 PipView_GetInfo(APPDISP_VIEW_INFO *info)
{
    if (!info)
		return E_PAR;
	*info = pip_view_info;
	return E_OK;
}

#if (SENSOR_CAPS_COUNT == 2)
static INT32 PipView_OnDraw_2sensor(APPDISP_VIEW_DRAW *pDraw) //PIP = Picture In Picture
{
	IRECT            dst_region;

    // img2
	dst_region.x = 0;
	dst_region.y = 0;
	dst_region.w = pDraw->p_dst_img->Width / 2;
	dst_region.h = pDraw->p_dst_img->Height / 2;
	GxImg_ScaleData(pDraw->p_src_img[1], REGION_MATCH_IMG, pDraw->p_dst_img, &dst_region);

	return E_OK;
}
#endif

#if (SENSOR_CAPS_COUNT == 4)

static INT32 PipView_OnDraw_4sensor_2view(APPDISP_VIEW_DRAW *pDraw) //PIP = Picture In Picture
{
	IRECT            dst_region;

    // img2
	dst_region.x = 0;
	dst_region.y = 0;
	dst_region.w = pDraw->p_dst_img->Width / 2;
	dst_region.h = pDraw->p_dst_img->Height / 2;
	GxImg_ScaleData(pDraw->p_src_img[1], REGION_MATCH_IMG, pDraw->p_dst_img, &dst_region);

	return E_OK;
}

static INT32 PipView_OnDraw_4sensor(APPDISP_VIEW_DRAW *pDraw) //PIP = Picture In Picture
{
	IRECT            dst_region;

    // img1
	dst_region.x = 0;
	dst_region.y = 0;
	dst_region.w = pDraw->p_dst_img->Width / 2;
	dst_region.h = pDraw->p_dst_img->Height / 2;
	GxImg_ScaleData(pDraw->p_src_img[0], REGION_MATCH_IMG, pDraw->p_dst_img, &dst_region);

    // img2
    dst_region.x = pDraw->p_dst_img->Width / 2;
	dst_region.y = 0;
	GxImg_ScaleData(pDraw->p_src_img[1], REGION_MATCH_IMG, pDraw->p_dst_img, &dst_region);

    // img3
    dst_region.x = 0;
	dst_region.y = pDraw->p_dst_img->Height / 2;
	GxImg_ScaleData(pDraw->p_src_img[2], REGION_MATCH_IMG, pDraw->p_dst_img, &dst_region);

    // img4
    dst_region.x = pDraw->p_dst_img->Width / 2;
	dst_region.y = pDraw->p_dst_img->Height / 2;
	GxImg_ScaleData(pDraw->p_src_img[3], REGION_MATCH_IMG, pDraw->p_dst_img, &dst_region);

	return E_OK;
}
#endif



INT32 PipView_OnDraw(APPDISP_VIEW_DRAW *pDraw) //PIP = Picture In Picture
{
    #if (SENSOR_CAPS_COUNT == 4)
    if (pDraw->viewcnt == 2)
        PipView_OnDraw_4sensor_2view(pDraw);
    else
        PipView_OnDraw_4sensor(pDraw);
	#elif (SENSOR_CAPS_COUNT == 2)
	PipView_OnDraw_2sensor(pDraw);
	#else
	DBG_ERR("Not Support %d sensor\r\n",SENSOR_CAPS_COUNT);
	#endif
	return E_OK;
}




