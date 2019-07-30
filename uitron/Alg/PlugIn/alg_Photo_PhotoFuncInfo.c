#include "SysCommon.h"
#include "GxImage.h"
#include "UIInfo.h"
#include "ImageUnit_UserProc.h"
#include "UIPhotoFuncInfo.h"

#if _FD_FUNC_
#include "fd_lib.h"
#include "UIPhotoMapping.h"
#include "ImageUnit_ImagePipe.h"
#include "UIPhotoInfo.h"
#define PHOTO_FD_FACESCALE	15
#endif

#if NVT_DEPTH_FUNC
#include "NvtDepth.h"
#endif


#define THIS_DBGLVL         2 // 0=FATAL, 1=ERR, 2=WRN, 3=UNIT, 4=FUNC, 5=IND, 6=MSG, 7=VALUE, 8=USER
///////////////////////////////////////////////////////////////////////////////
#define __MODULE__          alg_Photo_PhotoFuncInfo
#define __DBGLVL__          ((THIS_DBGLVL>=PRJ_DBG_LVL)?THIS_DBGLVL:PRJ_DBG_LVL)
#define __DBGFLT__          "*" //*=All, [mark]=CustomClass
#include "DebugModule.h"

#if _FD_FUNC_

static void Photo_FDInit(MEM_RANGE *buf, MEM_RANGE *cachebuf)
{
	//#if (_FPGA_EMULATION_ == ENABLE)
	FD_Config(FD_CFG_SCALE_ENG, FD_SCALE_ENG1);
	//#endif
	// Init FD buffer, set max face number to 10, and process rate as 3
	FD_Init(buf, cachebuf, MAX_FDNUM, PHOTO_FD_FACESCALE, 3);
}

static void Photo_FDProcess(MEM_RANGE *buf, MEM_RANGE *cachebuf)
{
	ISF_DATA         isf_data;
	ISF_UNIT         *pUnit;
	ISF_PORT         *pSrc;
	UINT32           ImageRatioIdx;

	ImageRatioIdx = GetPhotoSizeRatio(UI_GetData(FL_PHOTO_SIZE));
	pUnit = &ISF_UserProc;
	ImageUnit_Begin(pUnit, 0);
	ImageUnit_SetParam(ISF_OUT1, USERPROC_PARAM_ALLOW_PULL_IMM, TRUE);
	ImageUnit_End();
	pSrc = ImageUnit_Out(pUnit, ISF_OUT1);
	if (ImageUnit_IsAllowPull(pSrc) && ImageUnit_PullData(pSrc, (ISF_DATA *)&isf_data, 0) == ISF_OK) {

		#if 0
		{
			static UINT32 isdump = 0;
			PIMG_BUF pImgBuf = (IMG_BUF *) &isf_data.Desc[0];
			if (!isdump)
			{
				DBG_DUMP("[dump]pImgBuf w = %lu, h = %lu, fmt = %lu\r\n", pImgBuf->Width, pImgBuf->Height, pImgBuf->PxlFmt);
				GxImg_DumpBuf("A:\\fd.yuv", pImgBuf, GX_IMAGE_DUMP_ALL);
				isdump = 1;
			}
		}
		#endif
		FD_Process((IMG_BUF *)&isf_data.Desc[0], Get_FDImageRatioValue(ImageRatioIdx));
		ImageUnit_ReleaseData((ISF_DATA *)&isf_data);
	}
}

UINT32 Photo_FDCalcBuffSize(void)
{
	FD_Config(FD_CFG_FACE_SCALE, PHOTO_FD_FACESCALE);
	return FD_CalcBuffSize();
}

PHOTO_FUNC_INFO PhotoFuncInfo_fd = {
	{
		PHOTO_PRIORITY_M,       ///< function hook process Id.
		IPL_PATH_1,             ///< IPL Id.
		PHOTO_ISR_IME_FRM_END,  ///< isr tigger event
		"Photo_FDProcess",
		Photo_FDInit,           ///< init fp, only execute at first process
		Photo_FDProcess,        ///< process fp
		NULL,                   ///< process end fp
		NULL,                   ///< exit fp, only execute at photo close
		Photo_FDCalcBuffSize,   ///< get working buffer fp
		FD_CalcCacheBuffSize,   ///< get working cache buffer fp
	},
	NULL  ///< point next Function Obj, last Function must be set NULL
};

#endif


#if NVT_DEPTH_FUNC
extern BOOL depth_ntr_en;
extern BOOL depth_invalid_show;
extern BOOL sbs_showCase;
extern BOOL depth_dump_en;
extern BOOL depth_change_flag;
extern UINT32 depth_dump_cnt;
extern UINT32 depth_mode;
extern UINT32 depth_show_ver;

static UINT32 Photo_DepthCalcBuffSize(void)
{
	UINT32 size = 0;
	size = Depth_CalcBuffSize(640, 480, MAX_DISP_D128);
	//size = Depth_GetBuffSize();
	return size;
}


static UINT32 Photo_DepthCalcCacheBuffSize(void)
{
	UINT32 size = 0;
	size = Depth_CalcCacheBuffSize(640, 480, MAX_DISP_D128);
	return size;
}



static void Photo_DepthInit(MEM_RANGE *buf, MEM_RANGE *cachebuf)
{
	debug_msg("^YPhoto_DepthInit\r\n");
	debug_msg("WorkBuf=0x%x, size=0x%x\r\n", buf->Addr, buf->Size);
	DEPTH_INIT_PARAM  depth_initobj;
	memset((void *)&depth_initobj, 0, sizeof(DEPTH_INIT_PARAM));
	depth_initobj.buf = *buf;
	depth_initobj.cachebuf = *cachebuf;
	depth_initobj.disp_mode = (DISP_MODE)depth_mode;
	depth_initobj.ntr_on = depth_ntr_en;
	depth_initobj.inval_show = depth_invalid_show;
	Depth_Init(&depth_initobj);
}

static void Photo_DepthProcess(MEM_RANGE *buf, MEM_RANGE *cachebuf)
{

	ISF_DATA         isf_data1, isf_data2;
	ISF_UNIT         *pUnit;
	ISF_PORT         *pSrc1, *pSrc2;
	pUnit = &ISF_UserProc;
	ImageUnit_Begin(pUnit, 0);
	ImageUnit_SetParam(ISF_OUT5, USERPROC_PARAM_ALLOW_PULL_IMM, TRUE);
	ImageUnit_SetParam(ISF_OUT6, USERPROC_PARAM_ALLOW_PULL_IMM, TRUE);
	ImageUnit_End();
	pSrc1 = ImageUnit_Out(pUnit, ISF_OUT5);
	pSrc2 = ImageUnit_Out(pUnit, ISF_OUT6);
	if (ImageUnit_IsAllowPull(pSrc1) && ImageUnit_IsAllowPull(pSrc2)) {
		if (ImageUnit_PullData(pSrc1, (ISF_DATA *)&isf_data1, 0) != ISF_OK) {
			return;
		}
		if (ImageUnit_PullData(pSrc2, (ISF_DATA *)&isf_data2, 0) != ISF_OK) {
			ImageUnit_ReleaseData(&isf_data1);
			return;
		}
		static UINT32 g_frame_count = 0;

#if 0
		if(Depth_dump_en > 0) {
			static UINT32 isdump = 0;
			static UINT32 frame_count = 0;

			DBG_DUMP("^Rg_frame_count = %d, frame_count =%d \r\n", g_frame_count,frame_count);

			if (!isdump && frame_count < Depth_dump_cnt) {
				PIMG_BUF pImgBuf = (IMG_BUF *) &isf_data2.Desc[0];
				DBG_DUMP("[dump]pImgBuf w = %lu, h = %lu, fmt = %lu\r\n", pImgBuf->Width, pImgBuf->Height, pImgBuf->PxlFmt);
				char img_path[64];
				sprintf(img_path, "A:\\stereo1_%d.yuv", g_frame_count+frame_count);
				GxImg_DumpBuf(img_path, pImgBuf, GX_IMAGE_DUMP_ALL);

				pImgBuf = (IMG_BUF *) &isf_data1.Desc[0];
				DBG_DUMP("[dump]pImgBuf w = %lu, h = %lu, fmt = %lu\r\n", pImgBuf->Width, pImgBuf->Height, pImgBuf->PxlFmt);
				sprintf(img_path, "A:\\stereo2_%d.yuv", g_frame_count+frame_count);
				GxImg_DumpBuf(img_path, pImgBuf, GX_IMAGE_DUMP_ALL);
			}
			frame_count++;
			if(frame_count==Depth_dump_cnt){
				//isdump = 1;
				frame_count = 0;
			}
		}
#endif
		DEPTH_PARAM depthObj;
		memset((void *)&depthObj, 0, sizeof(DEPTH_PARAM));
		depthObj.inval_show = depth_invalid_show;
		depthObj.ntr_on = depth_ntr_en;
		depthObj.disp_mode = (DISP_MODE)depth_mode;

		if(depth_change_flag){
			Depth_ChangeParam(&depthObj);
			depth_change_flag	= 0;
		}
		if(depth_show_ver){
			Depth_PrintVersion();
			depth_show_ver	= 0;
		}
		// process stereo here
		Depth_Process((IMG_BUF *)&isf_data2.Desc[0], (IMG_BUF *)&isf_data1.Desc[0]);

#if 1
		if(depth_dump_en > 0) {
			static UINT32 frame_count2 = 0;

			if (depth_dump_en && frame_count2 < depth_dump_cnt) {
				STEREO_DISP_RESULT img_depth;
				memset((void *)&img_depth, 0, sizeof(STEREO_DISP_RESULT));

				PIMG_BUF pImgBuf = (IMG_BUF *) &isf_data2.Desc[0];
				//DBG_DUMP("[dump]pImgBuf w = %lu, h = %lu, fmt = %lu\r\n", pImgBuf->Width, pImgBuf->Height, pImgBuf->PxlFmt);
				char img_path[64];
				sprintf(img_path, "A:\\stereo1_%d.yuv", g_frame_count+frame_count2);
				GxImg_DumpBuf(img_path, pImgBuf, GX_IMAGE_DUMP_ALL);

				pImgBuf = (IMG_BUF *) &isf_data1.Desc[0];
				//DBG_DUMP("[dump]pImgBuf w = %lu, h = %lu, fmt = %lu\r\n", pImgBuf->Width, pImgBuf->Height, pImgBuf->PxlFmt);
				sprintf(img_path, "A:\\stereo2_%d.yuv", g_frame_count+frame_count2);
				GxImg_DumpBuf(img_path, pImgBuf, GX_IMAGE_DUMP_ALL);

				Depth_DumpDispResult(g_frame_count+frame_count2);
			}
			frame_count2++;
			if(frame_count2 == depth_dump_cnt){
				//isdump2 = 1;
				depth_dump_en = 0;
				DBG_DUMP("^YDepth dump end: g_frame_count = %d, frame_count =%d \r\n", g_frame_count,frame_count2);
				frame_count2 = 0;
				g_frame_count += depth_dump_cnt;
			}
		}
#endif

        // disp
#if NVT_DEPTH_VIEW
		{
			ISF_DATA            isf_new_data;
			ISF_PORT            *pDispSrc;
			IMG_BUF             dst_img;
			UINT32              img_buf_size , buff_addr, dst_w, dst_h, dst_pxlfmt;
			APPDISP_VIEW_DRAW   appdisp_draw;
			APPDISP_VIEW_DRAW   *pdraw = &appdisp_draw;
			STEREO_DISP_RESULT img_depth;
			img_depth.show_case = sbs_showCase;

		    Depth_GetDispResult(&img_depth);

        	pDispSrc = ImageUnit_Out(pUnit, ISF_OUT1 + PHOTO_DISP_ID_1);
			ImageUnit_GetVdoImgSize(pUnit, pDispSrc->oPort,&dst_w, &dst_h);
	        dst_pxlfmt = GX_IMAGE_PIXEL_FMT_YUV420_PACKED;
			img_buf_size = GxImg_CalcRequireSize(dst_w, dst_h, dst_pxlfmt, GXIMAGE_LINEOFFSET_ALIGN(16));
			buff_addr = ImageUnit_NewData(img_buf_size, &isf_new_data);
			if (buff_addr == 0) {
				DBG_ERR("Get buffer fail\r\n");
				goto depth_process_end;
			}
			GxImg_InitBuf(&dst_img, dst_w, dst_h, dst_pxlfmt, GXIMAGE_LINEOFFSET_ALIGN(16), buff_addr, img_buf_size);
			memcpy(&isf_new_data.Desc[0], &dst_img, sizeof(IMG_BUF));
			pdraw->p_src_img[0] = (IMG_BUF *)&isf_data2.Desc[0];
			pdraw->p_src_img[1] = &img_depth.disp_out;
			pdraw->p_dst_img = &dst_img;
			AppDispView_OnDraw(pdraw);
			ImageUnit_PushData(pDispSrc, (ISF_DATA *)&isf_new_data, 0);
			ImageUnit_ReleaseData(&isf_new_data);
		}
depth_process_end:
#endif
		// process end , release buffer
		ImageUnit_ReleaseData((ISF_DATA *)&isf_data1);
		ImageUnit_ReleaseData((ISF_DATA *)&isf_data2);
	}
}


PHOTO_FUNC_INFO PhotoFuncInfo_Depth = {
	{
		PHOTO_PRIORITY_H,       ///< function hook process Id.
		IPL_PATH_2,             ///< IPL Id.
		PHOTO_ISR_IME_FRM_END,  ///< isr tigger event
		"Photo_DepthProcess",
		Photo_DepthInit,           ///< init fp, only execute at first process
		Photo_DepthProcess,        ///< process fp
		NULL,                      ///< process end fp
		NULL,                      ///< exit fp, only execute at photo close
		Photo_DepthCalcBuffSize,        ///< get working buffer fp
		Photo_DepthCalcCacheBuffSize,   ///< get working cache buffer fp
	},
	NULL  ///< point next Function Obj, last Function must be set NULL
};

#endif

void alg_Photo_InitPhotoFunc(PHOTO_FUNC_INFO *phead)
{
#if MOVIE_DIS_MV_FUNC
		PHOTO_FUNC_INFO *pfunc = &PhotoFuncInfo_dis;
		PHOTO_FUNC_INFO *pcurr = 0;
		pcurr = phead;
		while (pcurr->pNext) {
			pcurr = pcurr->pNext;    //find last func
		}
		pcurr->pNext = pfunc; //append this func
		pfunc->pNext = NULL; //mark end
#endif
#if _FD_FUNC_
	{
		UINT32      myDualCam;
		PHOTO_FUNC_INFO *pfunc = &PhotoFuncInfo_fd;
		PHOTO_FUNC_INFO *pcurr = 0;
		pcurr = phead;
		while (pcurr->pNext) {
			pcurr = pcurr->pNext;    //find last func
		}
		pcurr->pNext = pfunc; //append this func
		pfunc->pNext = NULL; //mark end

		myDualCam = Photo_getDualCamSetting();

		if (myDualCam == DUALCAM_FRONT || myDualCam == DUALCAM_BOTH) {
			PhotoFuncInfo_fd.Info.IPLId = IPL_PATH_1;
		} else {
			PhotoFuncInfo_fd.Info.IPLId = IPL_PATH_2;
		}

		DBG_IND("myDualCam = %d, FD IPLID = %d\r\n", myDualCam, PhotoFuncInfo_fd.Info.IPLId);
	}
#endif
#if NVT_DEPTH_FUNC
	{
		PHOTO_FUNC_INFO *pfunc = &PhotoFuncInfo_Depth;
		PHOTO_FUNC_INFO *pcurr = 0;
		pcurr = phead;
		while (pcurr->pNext) {
			pcurr = pcurr->pNext;    //find last func
		}
		pcurr->pNext = pfunc; //append this func
		pfunc->pNext = NULL;  //mark end
	}
#endif
}
