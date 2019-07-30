#include "GxImage.h"
#include "UIInfo.h"
#include "SysCfg.h"
#include "AppDisp_View.h"
#include "AppDisp_SbsView.h"

#define THIS_DBGLVL         2       // 0=FATAL, 1=ERR, 2=WRN, 3=UNIT, 4=FUNC, 5=IND, 6=MSG, 7=VALUE, 8=USER
#define __MODULE__          Sbs
#define __DBGLVL__          THIS_DBGLVL
#define __DBGFLT__          "*" //*=All, [mark]=CustomClass
//#define __DBGFLT__          "[buf]"
#include "DebugModule.h"

#if (SBS_VIEW_FUNC == ENABLE)

static APPDISP_VIEW_INFO   sbs_view_info={
							 0x00010002, 0x00010001 , // w 1/2    h
                      		 0x00010002, 0x00010001 , // w 1/2    h
					  		 0x00010002, 0x00010001 , // w 1/2    h
							 0x00010002, 0x00010001 , // w 1/2    h
};

void SbsView_SetStyle(UINT32 uiStyle)
{
	sbs_view_info.scale[0].w = 0x00010002;
	sbs_view_info.scale[0].h = 0x00010001;
	sbs_view_info.scale[1].w = 0x00010002; // w 1/2
	sbs_view_info.scale[1].h = 0x00010001; // h 1/1
}

#if (SENSOR_CAPS_COUNT == 2)
static INT32 SbsView_OnDraw_2sensor(APPDISP_VIEW_DRAW *pDraw)
{
	#if 0
	IPOINT dstLos = {0, 0};

    #if 0
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

    // img1
	GxImg_CopyData(pDraw->p_src_img[0], REGION_MATCH_IMG, pDraw->p_dst_img, &dstLos);

    // img2
    dstLos.x = pDraw->p_dst_img->Width / 2;
	GxImg_CopyData(pDraw->p_src_img[1], REGION_MATCH_IMG, pDraw->p_dst_img, &dstLos);

    #else
	IRECT     dst_region;



    GxImg_FillData(pDraw->p_dst_img, REGION_MATCH_IMG, COLOR_YUV_BLACK);
	// img1
	dst_region.x = 0;
	dst_region.y = 0;
	dst_region.w = pDraw->p_dst_img->Width / 2;
	dst_region.h = pDraw->p_dst_img->Height;
	GxImg_ScaleData(pDraw->p_src_img[0], REGION_MATCH_IMG, pDraw->p_dst_img, &dst_region);

    // img2
    dst_region.x = pDraw->p_dst_img->Width / 2;
	dst_region.y = 0;
	#if NVT_DEPTH_VIEW
	GxImg_SetParm(GXIMG_PARM_SCALE_METHOD, GXIMG_SCALE_NEAREST);
	GxImg_ScaleData(pDraw->p_src_img[1], REGION_MATCH_IMG, pDraw->p_dst_img, &dst_region);
	GxImg_SetParm(GXIMG_PARM_SCALE_METHOD, GXIMG_SCALE_AUTO);
	#else
	GxImg_ScaleData(pDraw->p_src_img[1], REGION_MATCH_IMG, pDraw->p_dst_img, &dst_region);
	#endif

    #endif
	return E_OK;
}
#endif

INT32 SbsView_GetInfo(APPDISP_VIEW_INFO *info)
{
    if (!info)
		return E_PAR;
	*info = sbs_view_info;
	return E_OK;
}

INT32 SbsView_OnDraw(APPDISP_VIEW_DRAW *pDraw)
{
    #if (SENSOR_CAPS_COUNT == 2)
	SbsView_OnDraw_2sensor(pDraw);
	#else
	DBG_ERR("Not Support %d sensor\r\n",SENSOR_CAPS_COUNT);
	#endif
	return E_OK;
}
#endif
