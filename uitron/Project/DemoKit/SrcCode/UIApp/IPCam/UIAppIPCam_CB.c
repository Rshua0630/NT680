////////////////////////////////////////////////////////////////////////////////
#include "SysCommon.h"
////////////////////////////////////////////////////////////////////////////////
#include "UIAppCommon.h"
#include "ImageApp_IPCam.h"
#include "UIAppIPCam.h"
#include "UIAppIPCam_Param.h"
#include "iq_api.h"
#include "videosprite.h"
#include "ae_task.h"
#include "NMediaRecVdoEnc.h"
#if (STAMP_FUNC==ENABLE)
#include "MovieStamp.h"
#endif
#if (defined(_BSP_NA51000_) && (IPCAM_DIS_COMP_FUNC == ENABLE))
#include "dis_alg_fw.h"
#endif

#define THIS_DBGLVL        2 // 0=FATAL, 1=ERR, 2=WRN, 3=UNIT, 4=FUNC, 5=IND, 6=MSG, 7=VALUE, 8=USER
///////////////////////////////////////////////////////////////////////////////
#define __MODULE__          UiAppIPCamCB
#define __DBGLVL__          ((THIS_DBGLVL>=PRJ_DBG_LVL)?THIS_DBGLVL:PRJ_DBG_LVL)
#define __DBGFLT__          "*" //*=All, [mark]=CustomClass
//#define __DBGFLT__          "[cap]"
//#define __DBGFLT__          "[cb]"
//#define __DBGFLT__          "[set]"
//#define __DBGFLT__          "[zoom]"
//#define __DBGFLT__          "[af]"
#include "DebugModule.h"

extern BOOL bEnableAE;
extern BOOL bStableAE;
extern BOOL bEnableVdo;
extern BOOL bShowPreview;
static BOOL bFirstIPL = TRUE;
static BOOL bFirstENC = TRUE;

//IMAGEPIPE_PARAM_IPL_CB
#include "ipl_cb_msg.h"
#if (IPCAM_RESET_FPS == ENABLE)
static ISF_OUT   g_ISFOutMap[IPCAM_PATH_MAX_ID * 2] = {     ISF_OUT1, ISF_OUT2,
															ISF_OUT3, ISF_OUT4,
															ISF_OUT5, ISF_OUT6,
															ISF_OUT7, ISF_OUT8,
															ISF_OUT9, ISF_OUT10,
															ISF_OUT11, ISF_OUT12,
															ISF_OUT13, ISF_OUT14,
															ISF_OUT15, ISF_OUT16,
#if defined(_BSP_NA51000_)
															ISF_OUT17, ISF_OUT18,
															ISF_OUT19, ISF_OUT20,
															ISF_OUT21, ISF_OUT22,
															ISF_OUT23, ISF_OUT24,
															ISF_OUT25, ISF_OUT26,
															ISF_OUT27, ISF_OUT28,
															ISF_OUT29, ISF_OUT14,
															ISF_OUT31, ISF_OUT32
#endif
													  };

#endif

static UINT32 IPCamExe_IPLProcCB(UINT32 id, UINT32 state)
{
	switch (state) {
	default:
		if (bFirstIPL) {
			TM_BOOT_BEGIN("ImagePipe", "start");
		}
		break;
	}
	return  0;
}

static void IPCamExe_IPLCB(IPL_PROC_ID Id, IPL_CBMSG uiMsgID, void *Data)
{
	switch (uiMsgID) {
	case IPL_CBMSG_PREVIEW:
		DBG_IND("===IPL_CBMSG_PREVIEW===\r\n");
		if (bFirstIPL) {
			TM_BOOT_END("ImagePipe", "start");
			bFirstIPL = FALSE;
		}
		if(!bEnableAE)
			bEnableAE = ae_isopen(AE_ID_1);
		break;
	default:
		break;
	}
}

//AE_CBMsgInit
#include "ae_cb_msg.h"
static void IPCamExe_AE_CB(AE_CBMSG uiMsgID, void *Data)
{
	switch (uiMsgID) {

	case AE_CBMSG_PREVIEWSTABLE:
		DBG_IND("===AE_CBMSG_PREVIEWSTABLE===\r\n");
		bStableAE = TRUE;
		if(bEnableVdo) {
			if((DX_HANDLE)GxVideo_GetDevice(DOUT1) && !bShowPreview) {
				bShowPreview = TRUE;
				Display_ShowPreview(); //show preview immediately (AE should be last one)
				DBG_DUMP("^YShow Preview! (IPL > Video VSync > AE stable)\r\n");
			}
		}
		break;

	default:
		break;
	}
}

//VDOENC_PARAM_3DNR_CB
void IPCamExe_UIDefine_3DNR(UINT32 pathID, UINT32 p3DNRConfig)
{
    IPCAM_SENSOR sensor_id;
    IPCAM_STREAM_INFO *pStreamInfo = (IPCAM_STREAM_INFO *)UIAppIPCam_get_streamInfo();
    sensor_id = pStreamInfo[pathID].sensor;
    iq_get_tnr_config(IPL_PATH(sensor_id), p3DNRConfig);
}

UINT32 IPCamExe_VdoEnc_ProcCB(UINT32 id, UINT32 state)
{
	switch (state) {
	default:
		if (bFirstENC) {
			TM_BOOT_BEGIN("VdoEnc", "start");
		}
		break;
	}
	return  0;
}

//VDOENC_PARAM_EVENT_CB
void IPCamExe_VdoEnc_CB(CHAR* Name, UINT32 event_id, UINT32 value)
{
	MP_VDOENC_YUV_SRC* p_yuv_src = (MP_VDOENC_YUV_SRC*)value;

	if(!p_yuv_src)
		return;

	if(p_yuv_src->uiSrcCompression)
		return;

	if (strcmp(Name, "VdoEnc") == 0)
	{
		switch (event_id)
		{
			case NMI_VDOENC_EVENT_STAMP_CB:
				{

				videosprite_draw_btn_ext(p_yuv_src->uiVidPathID,
							p_yuv_src->uiYAddr,
							p_yuv_src->uiCbAddr,
							p_yuv_src->uiCrAddr,
							p_yuv_src->uiYLineOffset,
							p_yuv_src->uiWidth,
							p_yuv_src->uiHeight);

				#if (STAMP_FUNC==ENABLE)
				MovieStamp_CopyData(p_yuv_src->uiVidPathID,
					p_yuv_src->uiYAddr,
					p_yuv_src->uiCbAddr,
					p_yuv_src->uiCrAddr,
					p_yuv_src->uiYLineOffset,
					p_yuv_src->uiWidth);
				#endif
				}
			break;

			case NMI_VDOENC_EVENT_BS_CB:
				if (bFirstENC) {
					TM_BOOT_END("VdoEnc", "start");
					bFirstENC = FALSE;
				}
			break;

			default:
			break;
		}
	}
}

//NETRTSP_PARAM_IFRAME_REQ_CB
#if(FORCE_UNPLUG_RTSP != ENABLE)
void IPCamExe_RtspIFramReq_CB(UINT32 pathId)
{
	ImageUnit_Begin(&ISF_VdoEnc, 0);
		ImageUnit_SetParam(pathId, VDOENC_PARAM_RESET_IFRAME, 1);
	ImageUnit_End();
	return;
}
#endif

#if defined(_BSP_NA51023_) && (IPCAM_DIS_COMP_FUNC == ENABLE)
#include "dis_alg_fw.h"
static UINT32 IPCamExe_ViewTracking_CB(UINT32 id, UINT32 frame_cnt, INT32* pOffx, INT32* pOffy)
{
	DISALG_VECTOR track_ret;
	dis_getFrameCorrVec(&track_ret, frame_cnt);
	//DBG_DUMP("tracking score=%d\r\n", track_ret.score);
	if(pOffx)pOffx[0] = track_ret.vector.iX;
	if(pOffy)pOffy[0] = track_ret.vector.iY;
	return track_ret.score;
}
#endif

void IPCam_RegCB(void)
{
	if (System_GetEnableSensor() == (SENSOR_2)) {
		ImageUnit_Begin(ISF_IPL(1), 0);
		ImageUnit_SetParam(ISF_CTRL, IMAGEPIPE_PARAM_PROC_CB, (UINT32)IPCamExe_IPLProcCB);
		ImageUnit_SetParam(ISF_CTRL, IMAGEPIPE_PARAM_IPL_CB, (UINT32)IPCamExe_IPLCB);
		ImageUnit_SetParam(ISF_CTRL, IMAGEPIPE_PARAM_VIDEOOSD_CB,  (UINT32)videosprite_draw_ps_image);
		ImageUnit_SetParam(ISF_CTRL, IMAGEPIPE_PARAM_VIDEOMASK_CB, (UINT32)videosprite_draw_ps_mask);
		#if defined(_BSP_NA51023_) && (IPCAM_DIS_COMP_FUNC == ENABLE)
		ImageUnit_SetParam(ISF_CTRL, IMAGEPIPE_PARAM_VIEWTRACKING_CB, (UINT32)IPCamExe_ViewTracking_CB);
		#endif
		#if defined(_BSP_NA51023_) && (IPCAM_DIS_COMP_FUNC == ENABLE)
		ImageUnit_SetParam(ISF_OUT1, IMAGEPIPE_PARAM_SIZEFACTOR, 1100);
		ImageUnit_SetParam(ISF_OUT2, IMAGEPIPE_PARAM_SIZEFACTOR, 1100);
		ImageUnit_SetParam(ISF_OUT3, IMAGEPIPE_PARAM_SIZEFACTOR, 1100);
		ImageUnit_SetParam(ISF_OUT4, IMAGEPIPE_PARAM_SIZEFACTOR, 1100);
		ImageUnit_SetParam(ISF_OUT5, IMAGEPIPE_PARAM_SIZEFACTOR, 1100);
		#endif
		#if defined(_BSP_NA51000_) && (IPCAM_DIS_COMP_FUNC == ENABLE)
		dis_setDisViewRatio(909);
		#endif
		ImageUnit_End();
		AE_CBMsgInit(IPL_PATH(1), IPCamExe_AE_CB);
	} else {
		ImageUnit_Begin(ISF_IPL(0), 0);
		ImageUnit_SetParam(ISF_CTRL, IMAGEPIPE_PARAM_PROC_CB, (UINT32)IPCamExe_IPLProcCB);
		ImageUnit_SetParam(ISF_CTRL, IMAGEPIPE_PARAM_IPL_CB, (UINT32)IPCamExe_IPLCB);
		ImageUnit_SetParam(ISF_CTRL, IMAGEPIPE_PARAM_VIDEOOSD_CB,  (UINT32)videosprite_draw_ps_image);
		ImageUnit_SetParam(ISF_CTRL, IMAGEPIPE_PARAM_VIDEOMASK_CB, (UINT32)videosprite_draw_ps_mask);
		#if defined(_BSP_NA51023_) && (IPCAM_DIS_COMP_FUNC == ENABLE)
		ImageUnit_SetParam(ISF_CTRL, IMAGEPIPE_PARAM_VIEWTRACKING_CB, (UINT32)IPCamExe_ViewTracking_CB);
		#endif
		#if defined(_BSP_NA51023_) && (IPCAM_DIS_COMP_FUNC == ENABLE)
		ImageUnit_SetParam(ISF_OUT1, IMAGEPIPE_PARAM_SIZEFACTOR, 1100);
		ImageUnit_SetParam(ISF_OUT2, IMAGEPIPE_PARAM_SIZEFACTOR, 1100);
		ImageUnit_SetParam(ISF_OUT3, IMAGEPIPE_PARAM_SIZEFACTOR, 1100);
		ImageUnit_SetParam(ISF_OUT4, IMAGEPIPE_PARAM_SIZEFACTOR, 1100);
		ImageUnit_SetParam(ISF_OUT5, IMAGEPIPE_PARAM_SIZEFACTOR, 1100);
		#endif
		#if defined(_BSP_NA51000_) && (IPCAM_DIS_COMP_FUNC == ENABLE)
		dis_setDisViewRatio(909);
		#endif
		ImageUnit_End();
		AE_CBMsgInit(IPL_PATH(0), IPCamExe_AE_CB);
	}
}

#if (IPCAM_RESET_FPS == ENABLE)
static UINT32 uiCheckFPS[IPCAM_PATH_MAX_ID] ={0};
//force to update dynamic FPS
void IPCam_dirtyFPS(UINT32 path_index)
{
	uiCheckFPS[path_index] = 0;
}
//check and update dynamic FPS
void IPCam_checkFPS_CB(void){


	UINT32 path_index=0;
	IPCAM_STREAM_INFO *pStreamInfo = UIAppIPCam_get_streamInfo();
	UINT32 uiNowFPS[IPCAM_PATH_MAX_ID] = {0};
	UIAPPIPCAM_ENCODE_CONFIG *pEncodeConfig = UIAppIPCam_get_EncodeConfig();
	UIAPPIPCAM_CBR_CONFIG *pCBRConfig = UIAppIPCam_get_CBRInfo();
	UIAPPIPCAM_VBR_CONFIG *pVBRConfig = UIAppIPCam_get_VBRInfo();
	UIAPPIPCAM_EVBR_CONFIG *pEVBRConfig = UIAppIPCam_get_EVBRInfo();
	UIAPPIPCAM_BRC_NONE_CONFIG *pBRCNoneConfig = UIAppIPCam_get_BRCNoneInfo();
	UIAPPIPCAM_FIXQP_CONFIG *pFixQPConfig = UIAppIPCam_get_FixQPInfo();

	UINT32 tmpFPS =0;

	UINT32 uiCodecFPS =0;

	for (path_index = 0 ; path_index < IPCAM_PATH_MAX_ID ; path_index++){
		IPCAM_SENSOR sensor_index = pStreamInfo[path_index].sensor;

		if(!pStreamInfo[path_index].enable)
			continue; //ignore disabled path

		#if (IPCAM_DISP_ON_OFF != 0)
		if(path_index == IPCAM_DISP_PATH_NO)
			continue; //ignore display path
		#endif

		tmpFPS = ImageUnit_GetParam(ISF_IPL(sensor_index) ,ISF_IN1, IMAGEPIPE_PARAM_LIVE_INFPS_IMM);
		uiNowFPS[path_index] =  SEN_FPS_FMT_CONV2_REAL(tmpFPS) / 1000;

		if(uiCheckFPS[path_index] != uiNowFPS[path_index]){

			if(pEncodeConfig[path_index].eBrcType == UIAPPIPCAM_BRC_CBR){
				uiCodecFPS = pCBRConfig[path_index].cbr_info.uiFrameRate;
			}
			else if(pEncodeConfig[path_index].eBrcType == UIAPPIPCAM_BRC_VBR){
				uiCodecFPS = pVBRConfig[path_index].vbr_info.uiFrameRate;
			}
			else if(pEncodeConfig[path_index].eBrcType == UIAPPIPCAM_BRC_FIXQP){
				uiCodecFPS = pFixQPConfig[path_index].sFixQPInfo.uiFrameRate;
			}
			else if(pEncodeConfig[path_index].eBrcType == UIAPPIPCAM_BRC_EVBR){
				uiCodecFPS = pEVBRConfig[path_index].evbr_info.uiFrameRate;
			}
			else if(pEncodeConfig[path_index].eBrcType == UIAPPIPCAM_BRC_NONE){
				uiCodecFPS = pBRCNoneConfig[path_index].uiFrameRate;
			}
			else{
				DBG_ERR("brc type error:%d path:%d\r\n",pEncodeConfig[path_index].eBrcType,path_index);
				continue;
			}

			ImageUnit_Begin(&ISF_VdoEnc, 0);
				////set fps 1/1, for IPL trigger mode
				if (pStreamInfo[path_index].trig == IPCAM_TRIGGER_DEST) {
					ImageUnit_SetParam(g_ISFOutMap[path_index] , VDOENC_PARAM_FPS_IMM, ISF_VDO_FRC(uiCodecFPS, uiNowFPS[path_index]));
				} else {
					// ....
				}
			ImageUnit_End();

			ImageUnit_Begin(&ISF_ImgTrans, 0);
				if (pStreamInfo[path_index].trig == IPCAM_TRIGGER_DEST) {
					ImageUnit_SetParam(g_ISFOutMap[path_index] , IMGTRANS_PARAM_FPS_IMM, ISF_VDO_FRC(1, 1));
				} else {
					ImageUnit_SetParam(g_ISFOutMap[path_index] , IMGTRANS_PARAM_FPS_IMM, ISF_VDO_FRC(uiCodecFPS, uiNowFPS[path_index]));
				}
			ImageUnit_End();


		}

		uiCheckFPS[path_index] = uiNowFPS[path_index];
	}


}

#endif

UINT32 IPCam_StartVideoCB(UVAC_VID_DEV_CNT vidDevIdx, UVAC_STRM_INFO *pStrmInfo)
{
	return E_OK;
}

void IPCam_StopVideoCB(UVAC_VID_DEV_CNT vidDevIdx)
{

}

UINT32 IPCam_SetVolumeCB(UINT32 volume)
{
	return 0;
}


