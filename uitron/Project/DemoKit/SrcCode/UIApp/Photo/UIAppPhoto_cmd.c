//global debug level: PRJ_DBG_LVL
#include "PrjCfg.h"

#include "SysCommon.h"
#include "SysCfg.h"
#include "AppLib.h"
#include "UIAppCommon.h"
#include "UIModePhoto.h"
#include "UIAppPhoto.h"
#include "UIPhotoMapping.h"
#include "FileDB.h"
#include "ImageUnit_ImgTrans.h"
#include "ImageUnit_VdoEnc.h"
#if (VIDEO_FUNC_MJPG == ENABLE)
#include "VideoCodec_MJPG.h"
#endif
#if (VIDEO_FUNC_H264 == ENABLE)
#include "VideoCodec_H264.h"
#endif
#if (VIDEO_FUNC_H265 == ENABLE)
#include "VideoCodec_H265.h"
#endif

//local debug level: THIS_DBGLVL
#define THIS_DBGLVL         2 // 0=FATAL, 1=ERR, 2=WRN, 3=UNIT, 4=FUNC, 5=IND, 6=MSG, 7=VALUE, 8=USER
///////////////////////////////////////////////////////////////////////////////
#define __MODULE__          UiAppPhotoCmd
#define __DBGLVL__          ((THIS_DBGLVL>=PRJ_DBG_LVL)?THIS_DBGLVL:PRJ_DBG_LVL)
#define __DBGFLT__          "*" //*=All, [mark]=CustomClass
#include "DebugModule.h"
///////////////////////////////////////////////////////////////////////////////
#if NVT_DEPTH_FUNC
BOOL depth_ntr_en = FALSE;
BOOL depth_invalid_show = FALSE;
BOOL sbs_showCase = TRUE;
BOOL depth_dump_en = FALSE;
BOOL depth_change_flag = FALSE;
UINT32 depth_dump_cnt = 0;
UINT32 depth_mode = 1;
UINT32 depth_show_ver = 0;

static BOOL cmd_depth_mode(CHAR *strCmd)
{
	BOOL value;
	sscanf_s(strCmd, "%d", &value);
	depth_mode = value;
	depth_change_flag = TRUE;
	return TRUE;
}

static BOOL cmd_depth_ver(CHAR *strCmd)
{
	BOOL value;
	sscanf_s(strCmd, "%d", &value);
	depth_show_ver = value;
	return TRUE;
}

static BOOL cmd_depth_ntr(CHAR *strCmd)
{
	BOOL value;
	sscanf_s(strCmd, "%d", &value);
	depth_ntr_en = value;
	depth_change_flag = TRUE;
	return TRUE;
}

static BOOL cmd_depth_inval(CHAR *strCmd)
{
	BOOL value;
	sscanf_s(strCmd, "%d", &value);
	depth_invalid_show = value;
	depth_change_flag = TRUE;
	return TRUE;
}

static BOOL cmd_sbs_show(CHAR *strCmd)
{
	BOOL value;
	sscanf_s(strCmd, "%d", &value);
	sbs_showCase = value;
	return TRUE;
}

static BOOL cmd_depth_dump(CHAR *strCmd)
{
	UINT32 value, cnt;
	sscanf_s(strCmd, "%d %d", &value, &cnt);
	depth_dump_en = value;
	depth_dump_cnt = cnt;
	return TRUE;
}
#endif

static void redraw_photo_ui(void)
{
	Ux_PostEvent(NVTEVT_UPDATE_INFO,       1,  UIAPPPHOTO_CB_UCTRL_UPDATE_UI);
}

static BOOL cmd_fd(CHAR *strCmd)
{
	UINT32 value;
	sscanf_s(strCmd, "%d", &value);

	if (value == 0)
		//Ux_SendEvent(&CustomPhotoObjCtrl, NVTEVT_EXE_FD, 1, FD_OFF);
		Ux_PostEvent(NVTEVT_EXE_FD, 1, FD_OFF);
	else
		//Ux_SendEvent(&CustomPhotoObjCtrl, NVTEVT_EXE_FD, 1, FD_ON);
		Ux_PostEvent(NVTEVT_EXE_FD, 1, FD_ON);
	redraw_photo_ui();
	//exam_msg("photo fd %d\r\n", value);
	return TRUE;
}

static BOOL cmd_ev(CHAR *strCmd)
{
	UINT32 value;
	sscanf_s(strCmd, "%d", &value);

	if (value >= EV_SETTING_MAX) {
		DBG_ERR("photo ev %d\r\n", value);
		return TRUE;
	}
	//Ux_SendEvent(&CustomPhotoObjCtrl, NVTEVT_EXE_EV, 1, value);
	Ux_PostEvent(NVTEVT_EXE_EV, 1, value);
	redraw_photo_ui();
	//exam_msg("photo ev %d\r\n", value);
	return TRUE;
}

static BOOL cmd_iso(CHAR *strCmd)
{
	UINT32 value;
	sscanf_s(strCmd, "%d", &value);

	if (value >= ISO_ID_MAX) {
		DBG_ERR("photo iso %d\r\n", value);
		return TRUE;
	}

	//Ux_SendEvent(&CustomPhotoObjCtrl, NVTEVT_EXE_ISO, 1, value);
	Ux_PostEvent(NVTEVT_EXE_ISO, 1, value);
	redraw_photo_ui();
	//exam_msg("photo iso %d\r\n", value);
	return TRUE;
}

static BOOL cmd_wb(CHAR *strCmd)
{
	UINT32 value;
	sscanf_s(strCmd, "%d", &value);

	if (value >= WB_ID_MAX) {
		DBG_ERR("photo wb %d\r\n", value);
		return TRUE;
	}

	//Ux_SendEvent(&CustomPhotoObjCtrl, NVTEVT_EXE_WB, 1, value);
	Ux_PostEvent(NVTEVT_EXE_WB, 1, value);
	redraw_photo_ui();
	//exam_msg("photo wb %d\r\n", value);
	return TRUE;
}



static BOOL cmd_size(CHAR *strCmd)
{
	UINT32 value;
	sscanf_s(strCmd, "%d", &value);

	if (value >= PHOTO_SIZE_ID_MAX) {
		DBG_ERR("photo capture size %d\r\n", value);
		return TRUE;
	}

	//Ux_SendEvent(&CustomPhotoObjCtrl, NVTEVT_EXE_CAPTURE_SIZE, 1, value);
	Ux_PostEvent(NVTEVT_EXE_CAPTURE_SIZE, 1, value);
	redraw_photo_ui();
	//exam_msg("photo capture size %d\r\n", value);
	return TRUE;
}

static BOOL cmd_quality(CHAR *strCmd)
{
	UINT32 value;
	sscanf_s(strCmd, "%d", &value);

	if (value >= QUALITY_ID_MAX) {
		DBG_ERR("photo quality %d\r\n", value);
		return TRUE;
	}


	//Ux_SendEvent(&CustomPhotoObjCtrl, NVTEVT_EXE_QUALITY, 1, value);
	Ux_PostEvent(NVTEVT_EXE_QUALITY, 1, value);
	redraw_photo_ui();
	//exam_msg("photo quality %d\r\n", value);
	return TRUE;
}

static BOOL cmd_datestamp(CHAR *strCmd)
{
	UINT32 value;
	sscanf_s(strCmd, "%d", &value);

	if (value >= DATE_STAMP_ID_MAX) {
		DBG_ERR("photo datestamp %d\r\n", value);
		return TRUE;
	}

	//Ux_SendEvent(&CustomPhotoObjCtrl, NVTEVT_EXE_DATE_PRINT, 1, value);
	Ux_PostEvent(NVTEVT_EXE_DATE_PRINT, 1, value);
	redraw_photo_ui();
	//exam_msg("photo datestamp %d\r\n", value);
	return TRUE;
}

static BOOL cmd_sharp(CHAR *strCmd)
{
	UINT32 value;
	sscanf_s(strCmd, "%d", &value);

	if (value >= SHARPNESS_ID_MAX) {
		DBG_ERR("photo sharpness %d\r\n", value);
		return TRUE;
	}

	//Ux_SendEvent(&CustomPhotoObjCtrl, NVTEVT_EXE_SHARPNESS, 1, value);
	Ux_PostEvent(NVTEVT_EXE_SHARPNESS, 1, value);
	redraw_photo_ui();
	//exam_msg("photo sharpness %d\r\n", value);
	return TRUE;
}


static BOOL cmd_qviewtime(CHAR *strCmd)
{
	UINT32 value;
	sscanf_s(strCmd, "%d", &value);

	if (value >= QUICK_REVIEW_ID_MAX) {
		DBG_ERR("photo quick view time %d\r\n", value);
		return TRUE;
	}

	//Ux_SendEvent(&CustomPhotoObjCtrl, NVTEVT_EXE_PREVIEW, 1, value);
	Ux_PostEvent(NVTEVT_EXE_PREVIEW, 1, value);
	redraw_photo_ui();
	//exam_msg("photo quick view time %d\r\n", value);
	return TRUE;
}

static BOOL cmd_selftimer(CHAR *strCmd)
{
	UINT32 value;
	sscanf_s(strCmd, "%d", &value);

	if (value >= SELFTIMER_ID_MAX) {
		DBG_ERR("photo selftimer %d\r\n", value);
		return TRUE;
	}

	//Ux_SendEvent(&CustomPhotoObjCtrl, NVTEVT_EXE_SELFTIMER, 1, value);
	Ux_PostEvent(NVTEVT_EXE_SELFTIMER, 1, value);
	redraw_photo_ui();
	//exam_msg("photo selftimer %d\r\n", value);
	return TRUE;
}

static BOOL cmd_contshot(CHAR *strCmd)
{
	UINT32 value;
	sscanf_s(strCmd, "%d", &value);

	if (value >= CONTINUE_SHOT_SETTING_MAX) {
		DBG_ERR("photo contshot %d\r\n", value);
		return TRUE;
	}

	//Ux_SendEvent(&CustomPhotoObjCtrl, NVTEVT_EXE_CONTSHOT, 1, value);
	Ux_PostEvent(NVTEVT_EXE_CONTSHOT, 1, value);
	redraw_photo_ui();
	//exam_msg("photo contshot %d\r\n", value);
	return TRUE;
}

static BOOL cmd_scene(CHAR *strCmd)
{
	UINT32 value;
	sscanf_s(strCmd, "%d", &value);

	if (value >= SCENE_ID_MAX) {
		DBG_ERR("photo scene %d\r\n", value);
		return TRUE;
	}
	//Ux_SendEvent(&CustomPhotoObjCtrl, NVTEVT_EXE_SCENEMODE, 1, value);
	Ux_PostEvent(NVTEVT_EXE_SCENEMODE, 1, value);
	redraw_photo_ui();
	//exam_msg("photo scene %d\r\n", value);
	return TRUE;
}

static BOOL cmd_rsc(CHAR *strCmd)
{
	UINT32 value;
	sscanf_s(strCmd, "%d", &value);

	if (value == 0)
		Ux_PostEvent(NVTEVT_EXE_RSC, 1, RSC_OFF);
	else
		Ux_PostEvent(NVTEVT_EXE_RSC, 1, RSC_ON);
	redraw_photo_ui();
	return TRUE;
}

static BOOL cmd_wdr(CHAR *strCmd)
{
	UINT32 value;
	sscanf_s(strCmd, "%d", &value);

	if (value == 0)
		Ux_PostEvent(NVTEVT_EXE_WDR, 1, WDR_OFF);
	else
		Ux_PostEvent(NVTEVT_EXE_WDR, 1, WDR_ON);
	redraw_photo_ui();
	return TRUE;
}
static BOOL cmd_defog(CHAR *strCmd)
{ 
	UINT32 value; 
	sscanf_s(strCmd, "%d", &value); 
	
	if (value == 0) 
		Ux_PostEvent(NVTEVT_EXE_DEFOG, 1, DEFOG_OFF); 
	else 
		Ux_PostEvent(NVTEVT_EXE_DEFOG, 1, DEFOG_ON); 
	redraw_photo_ui(); 
	return TRUE; 
}

static BOOL cmd_delete_file(CHAR *strCmd)
{
	FILEDB_HANDLE      FileDBHandle = 0;
	PFILEDB_FILE_ATTR  FileAttr = NULL;
	INT32              ret;
	INT32              FileNum, i;
	UINT32             freeSpaceMB, reservSizeMB = 20;

	sscanf_s(strCmd, "%d", &reservSizeMB);

	FileNum = FileDB_GetTotalFileNum(FileDBHandle);
	for (i = FileNum - 1; i >= 0; i--) {
		freeSpaceMB = (UINT32)(FileSys_GetDiskInfo(FST_INFO_FREE_SPACE) / (1024*1024));
		if (freeSpaceMB > reservSizeMB)
			break;
		FileAttr = FileDB_SearhFile(FileDBHandle, 0);
		if (FileAttr) {
			if (M_IsReadOnly(FileAttr->attrib)) {
				//DBG_IND("File Locked\r\n");
				DBG_IND("File %s is locked\r\n", FileAttr->filePath);
				continue;
			}
			ret = FileSys_DeleteFile(FileAttr->filePath);
			//DBG_IND("i = %04d path=%s\r\n",i,FileAttr->filePath);
			if (ret != FST_STA_OK) {
				DBG_ERR("Delete %s failed\r\n", FileAttr->filePath);
			} else {
				FileDB_DeleteFile(FileDBHandle, 0);
				DBG_DUMP("Delete %s OK\r\n", FileAttr->filePath);
			}
		} else {
			DBG_IND("FileAttr not existed\r\n");
		}
	}
	return TRUE;
}


static BOOL cmd_cap(CHAR *strCmd)
{
	UINT32 value;
	sscanf_s(strCmd, "%d", &value);

    DBG_DUMP("cap cmd %d\r\n", value);
	if (value == 0)
		//Ux_SendEvent(&CustomPhotoObjCtrl, NVTEVT_EXE_CAPTURE_STOP, 1, value);
		Ux_PostEvent(NVTEVT_KEY_SHUTTER2, 1, NVTEVT_KEY_RELEASE);
	else {
		cmd_delete_file("");
		Ux_PostEvent(NVTEVT_KEY_SHUTTER2, 1, NVTEVT_KEY_PRESS);
		//Ux_SendEvent(&CustomPhotoObjCtrl, NVTEVT_EXE_CAPTURE_START, 1, value);
	}
	redraw_photo_ui();
	return TRUE;
}

static BOOL cmd_waitsavefin(CHAR *strCmd)
{
	DBG_DUMP("wait save finish\r\n");
    if (ImageUnit_GetParam(&ISF_Cap, ISF_CTRL, CAP_PARAM_WAITTSK) != 0) {
		DBG_ERR("WAITTSK\r\n");
	}
	exam_msg("photo save finish\r\n");
	return TRUE;
}

static BOOL cmd_stream(CHAR *strCmd)
{
	UINT32             stream_type = PHOTO_STRM_TYPE_HTTP;
	UINT32             codec = PHOTO_CODEC_MJPG;
	UINT32             width = 800;
	UINT32             height = 600;
	UINT32             target_byte_rate = 1*1024*1024;
	PHOTO_STRM_INFO    *p_strm = NULL;

    exam_msg("cmd %s\r\n", strCmd);

	sscanf_s(strCmd, "%d %d %d %d %d", &stream_type,&codec,&width,&height,&target_byte_rate);

	if (stream_type > PHOTO_STRM_TYPE_RTSP){
		DBG_ERR("stream_type=%d\r\n",stream_type);
		return TRUE;
	}
	if (codec < PHOTO_CODEC_MJPG || codec > PHOTO_CODEC_H265){
		DBG_ERR("codec=%d\r\n",codec);
		return TRUE;
	}
	if (width > 1920 || width < 320){
		DBG_ERR("width=%d\r\n",width);
		return TRUE;
	}
	if (height > 1080 || height < 240){
		DBG_ERR("height=%d\r\n",height);
		return TRUE;
	}
	width = ALIGN_CEIL_16(width);
	height = ALIGN_CEIL_16(height);
	p_strm = UIAppPhoto_get_StreamConfig(UIAPP_PHOTO_STRM_ID_1);
	p_strm->strm_type = stream_type;
	p_strm->codec = codec;
	p_strm->width = width;
	p_strm->height = height;
	p_strm->target_bitrate = target_byte_rate;
	//exam_msg("photo stream set type =%d, codec=%d, w = %d, h= %d, byterate = %d\r\n",stream_type,codec,width,height,target_byte_rate);
	exam_msg("photo stream set\r\n",stream_type,codec,width,height,target_byte_rate);

	ImageUnit_Begin(&ISF_VdoEnc, 0);
	if (codec == PHOTO_CODEC_MJPG) {
#if (VIDEO_FUNC_MJPG == ENABLE)
		MP_VDOENC_ENCODER *pObj = MP_MjpgEnc_getVideoObject(UIAPP_PHOTO_STRM_ID_1);
		ImageUnit_SetParam(UIAPP_PHOTO_STRM_ID_1, VDOENC_PARAM_ENCODER_OBJ, (UINT32)pObj);
#else
		ImageUnit_SetParam(UIAPP_PHOTO_STRM_ID_1, VDOENC_PARAM_ENCODER_OBJ, 0);
		DBG_ERR("[PHOTO][%d] codec = %d not support\r\n", UIAPP_PHOTO_STRM_ID_1, codec);
#endif
	}
	if (codec == PHOTO_CODEC_H264) {
#if (VIDEO_FUNC_H264 == ENABLE)
		MP_VDOENC_ENCODER *pObj = MP_H264Enc_getVideoObject(UIAPP_PHOTO_STRM_ID_1);
		ImageUnit_SetParam(UIAPP_PHOTO_STRM_ID_1, VDOENC_PARAM_ENCODER_OBJ, (UINT32)pObj);
#else
		ImageUnit_SetParam(UIAPP_PHOTO_STRM_ID_1, VDOENC_PARAM_ENCODER_OBJ, 0);
		DBG_ERR("[PHOTO][%d] codec = %d not support\r\n", UIAPP_PHOTO_STRM_ID_1, codec);
#endif
	}
	if (codec == PHOTO_CODEC_H265) {
#if (VIDEO_FUNC_H265 == ENABLE)
		MP_VDOENC_ENCODER *pObj = MP_H265Enc_getVideoObject(UIAPP_PHOTO_STRM_ID_1);
		ImageUnit_SetParam(UIAPP_PHOTO_STRM_ID_1, VDOENC_PARAM_ENCODER_OBJ, (UINT32)pObj);
#else
		ImageUnit_SetParam(UIAPP_PHOTO_STRM_ID_1, VDOENC_PARAM_ENCODER_OBJ, 0);
		DBG_ERR("[PHOTO][%d] codec = %d not support\r\n", UIAPP_PHOTO_STRM_ID_1, codec);
#endif
	}
	ImageUnit_End();

	return TRUE;
}


static BOOL cmd_setfps(CHAR *strCmd)
{
	UINT32 src_fps = 30,dst_fps = 30;

	sscanf_s(strCmd, "%d %d", &src_fps,&dst_fps);

	DBG_DUMP("cmd_set src_fps %d, dst_fps %d\r\n", src_fps,dst_fps);
	ImageUnit_Begin(&ISF_ImgTrans, 0);
	ImageUnit_SetParam(ISF_OUT1 , IMGTRANS_PARAM_FPS_IMM, ISF_VDO_FRC(dst_fps, src_fps));
	ImageUnit_End();
	return TRUE;
}

static BOOL cmd_quefull(CHAR *strCmd)
{
	UINT32 i;
	for (i=0;i<257;i++)
		Ux_PostEvent(NVTEVT_EXE_FDEND, 1, NVTEVT_EXE_FDEND);
	return TRUE;

}



SXCMD_BEGIN(uiphoto, "uiphoto command")
SXCMD_ITEM("fd %",         cmd_fd,      "fd off/on,  indxe value 0 ~ 1")
SXCMD_ITEM("ev %",         cmd_ev,      "ev -2 ~ +2, index value 0 ~ 12")
SXCMD_ITEM("iso %",        cmd_iso,     "ISO Auto, 100 ~ 1600, index value 0 ~ 5")
SXCMD_ITEM("wb %",         cmd_wb,      "WB,  index value 0 ~ 4")
SXCMD_ITEM("size %",       cmd_size,    "capture size, index value 0 ~ 5")
SXCMD_ITEM("quality %",    cmd_quality, "image quality, index value 0 ~ 2")
SXCMD_ITEM("datestamp %",  cmd_datestamp, "datestamp,  index value 0 ~ 2")
SXCMD_ITEM("sharp %",      cmd_sharp,     "sharp,  index value 0 ~ 2")
SXCMD_ITEM("qviewtime %",  cmd_qviewtime,"quick view time 0, 2s, 5s,  index value 0 ~ 2")
SXCMD_ITEM("selftimer %",  cmd_selftimer,"selftimer 0, 2s, 5s, 10s index value 0 ~ 3")
SXCMD_ITEM("contshot %",   cmd_contshot, "continue shot setting, index value 0 ~ 3")
SXCMD_ITEM("scene %",      cmd_scene,    "scene mode , index value 0 ~ 2")
SXCMD_ITEM("rsc %",        cmd_rsc,      "rsc off/on,  indxe value 0 ~ 1")
SXCMD_ITEM("wdr %",        cmd_wdr,      "wdr off/on,  indxe value 0 ~ 1")
SXCMD_ITEM("defog %",      cmd_defog,    "defog off/on,  indxe value 0 ~ 1")
SXCMD_ITEM("del %",        cmd_delete_file,"delete file until free space larger than 15MB")
SXCMD_ITEM("cap %",        cmd_cap,      "capture start/stop")
SXCMD_ITEM("waitsavefin", cmd_waitsavefin, "wait capture write all files finish")
SXCMD_ITEM("stream %",     cmd_stream,   "photo streaming setting")
SXCMD_ITEM("setfps %",     cmd_setfps,   "photo display fps setting")
SXCMD_ITEM("quefull %",    cmd_quefull,  "ui queue full")
#if NVT_DEPTH_FUNC
SXCMD_ITEM("depthver %",   cmd_depth_ver, "show version of NvtDepth")
SXCMD_ITEM("depthshow %",  cmd_sbs_show, "show depth/right image, 0: right image, 1: depth image")
SXCMD_ITEM("depthinval %", cmd_depth_inval, "change depth invalid as white or black")
SXCMD_ITEM("depthdump % %",cmd_depth_dump, "dump N depth images, depthdump 1 N")
SXCMD_ITEM("depthntr %",   cmd_depth_ntr, "enable depth non-texture removal")
SXCMD_ITEM("depthmode %",  cmd_depth_mode, "change depth mode, 0:dmax=32, 1: dmax=128")
#endif
SXCMD_END()
