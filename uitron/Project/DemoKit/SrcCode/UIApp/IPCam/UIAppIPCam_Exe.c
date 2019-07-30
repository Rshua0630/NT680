////////////////////////////////////////////////////////////////////////////////
#include "SysCommon.h"
//#include "AppCommon.h"
////////////////////////////////////////////////////////////////////////////////
#include "UIAppCommon.h"

#include "NVTUserCommand.h"
#include "SysCfg.h" //for memory POOL_ID
#include "GxDisplay.h"

#include "sensor.h"
#include "DxSensor.h"
#include "AppLib.h"

#include "UIMode.h"
#include "PrjCfg.h"

#include "SysMain.h"
#include "AppControl.h"
#include "nvtmpp.h"
#include "ImageApp_IPCam.h"
#include "UIAppIPCam.h"
#include "UIAppIPCam_Param.h"
#include "iq_api.h"
#include "UCtrlMain.h"
#include "media_def.h"
#include "UIPhotoFuncInfo.h"
#include "UCtrlAppMovie.h"
#include "videosprite.h"
#include "Audio.h"
#include "UVAC.h"
#include "NMediaRecVdoEnc.h"
#if (VIDEOSPRITEIPC==ENABLE)
#include "VideoSpriteIpc.h"
#endif
#if (STAMP_FUNC==ENABLE)
#include "MovieStamp.h"
#endif
#if IPCAM_FD_FUNC
#include "fd_lib.h"
#endif
#if IPCAM_OD_FUNC
#include "riff.h"
#endif
#if (AUDIO_FUNC_PCM == ENABLE)
#include "AudioCodec_PCM.h"
#endif
#if (AUDIO_FUNC_AAC == ENABLE)
#include "AudioCodec_AAC.h"
#endif
#if (AUDIO_FUNC_PPCM == ENABLE)
#include "AudioCodec_PPCM.h"
#endif
#if (AUDIO_FUNC_ULAW == ENABLE)
#include "AudioCodec_uLaw.h"
#endif
#if (AUDIO_FUNC_ALAW == ENABLE)
#include "AudioCodec_aLaw.h"
#endif
#if (VIDEO_FUNC_MJPG == ENABLE)
#include "VideoCodec_MJPG.h"
#endif
#if (VIDEO_FUNC_H264 == ENABLE)
#include "VideoCodec_H264.h"
#endif
#if (VIDEO_FUNC_H265 == ENABLE)
#include "VideoCodec_H265.h"
#endif
#if (LENS_FUNCTION == ENABLE)
#include "DeviceCtrl.h"
#endif

///////////////////////////////////////////sx_timeer set

#if(IPCAM_RESET_FPS == ENABLE)
int SX_TIMER_IPCAM_DET_FPS_ID = -1;
///set check fps per 1sec
SX_TIMER_ITEM(IPCam_FPS_DET, IPCam_checkFPS_CB, 50, TRUE);//period*20ms
#endif
//////////////////////////////////////////////////////////////////////////


#define THIS_DBGLVL        2 // 0=FATAL, 1=ERR, 2=WRN, 3=UNIT, 4=FUNC, 5=IND, 6=MSG, 7=VALUE, 8=USER
///////////////////////////////////////////////////////////////////////////////
#define __MODULE__          UiAppIPCam
#define __DBGLVL__          ((THIS_DBGLVL>=PRJ_DBG_LVL)?THIS_DBGLVL:PRJ_DBG_LVL)
#define __DBGFLT__          "*" //*=All, [mark]=CustomClass
//#define __DBGFLT__          "[cap]"
//#define __DBGFLT__          "[cb]"
//#define __DBGFLT__          "[set]"
//#define __DBGFLT__          "[zoom]"
//#define __DBGFLT__          "[af]"
#include "DebugModule.h"

#define RTSP_MAX_CLIENT 8
NVTMPP_VB_POOL  		g_sAudEncBufPool 				 = NVTMPP_VB_INVALID_POOL;

#if (IPCAM_AEC_FUNC == ENABLE)
#include "nvtaec_api.h"

static ER IPCamExe_SetAecBuf(UINT32 addr, UINT32 size)
{
	UINT32 internal_size = 0;
	UINT32 internal_addr = 0;
	UINT32 fore_size = 0;
	UINT32 fore_addr = 0;
	UINT32 back_size = 0;
	UINT32 back_addr = 0;
	UINT32 AllocSize = 0;

	internal_size = NvtAec_GetRequireBufSize(NVTAEC_MEM_ID_INTENAL);
	fore_size = NvtAec_GetRequireBufSize(NVTAEC_MEM_ID_FORE);
	back_size = NvtAec_GetRequireBufSize(NVTAEC_MEM_ID_BACK);
	AllocSize = internal_size + fore_size + back_size;

	if (AllocSize > size) {
		DBG_ERR("Buf is not enough, needed=%x, allocated=%x\r\n", AllocSize, size);
		return E_NOMEM;
	}

	internal_addr = addr;
	fore_addr	  = internal_addr + internal_size;
	back_addr	  = fore_addr + fore_size;

	NvtAec_SetRequireBuffer(NVTAEC_MEM_ID_INTENAL, internal_addr, internal_size);
	NvtAec_SetRequireBuffer(NVTAEC_MEM_ID_FORE, fore_addr, fore_size);
	NvtAec_SetRequireBuffer(NVTAEC_MEM_ID_BACK, back_addr, back_size);

	return E_OK;
}

WAVSTUD_AUD_AEC_OBJ aec_obj = {NvtAec_Open,
							   NvtAec_Close,
							   NvtAec_Start,
							   NvtAec_Stop,
							   NvtAec_Apply,
							   NvtAec_Enable,
							   IPCamExe_SetAecBuf
							  };
#endif

#if (IPCAM_ANR_FUNC == ENABLE)
#include "nvtanr_api.h"

static ER IPCamExe_SetAnrBuf(UINT32 addr, UINT32 size)
{
	nvtanr_setmem(addr, size);

	return E_OK;
}

AUDIN_ANR_OBJ anr_obj = {nvtanr_open,
						 nvtanr_close,
						 nvtanr_apply,
						 nvtanr_enable,
						 IPCamExe_SetAnrBuf
						};
#endif

#if (IPCAM_AUDFILT_FUNC == ENABLE)
#include "audFilt.h"

static BOOL IPCamExe_AudFiltOpen(UINT32 uiChannels, BOOL bSmooth)
{
	AUDFILT_INIT    AudFilterInit;

	if (uiChannels > 2) { // max support 2 channels
		uiChannels = 2;
	}

	AudFilterInit.FiltCH        = uiChannels;
	AudFilterInit.bSmoothEn     = bSmooth;

	AudFilt_Close();

	// open audio filter
	if (!AudFilt_Open(&AudFilterInit)) {
		DBG_ERR("Audio filter open err!\r\n");
		return FALSE;
	}

	// init audio filter
	if (!AudFilt_Init()) {
		DBG_ERR("Audio filter init err!\r\n");
		return FALSE;
	}

	return TRUE;
}

static BOOL IPCamExe_AudFiltClose(void)
{
	return AudFilt_Close();
}

static void IPCamExe_AudFiltDesign(void)
{
	AUDFILT_EQPARAM EqParam;
	UIAPPIPCAM_AUDIO_INFO *pAudioInfo = UIAppIPCam_get_AudioInfo();

	EqParam.SampleRate  = pAudioInfo->SampleRate;

	//Sample EQ

	AudFilt_EnableEQ(ENABLE, 2); //Enable EQ

	//200 Hz high pass filter
	EqParam.FiltType    = AUDFILT_DESIGNTYPE_HIGHPASS;
	EqParam.fFrequency  = 200;
	EqParam.fGainDB     = -20;
	EqParam.Q           = 1;
	AudFilt_DesignEQ(0, &EqParam);

	//1000 Hz notching filter
	EqParam.FiltType    = AUDFILT_DESIGNTYPE_NOTCH;
	EqParam.fFrequency  = 1000;
	EqParam.fGainDB     = -20;
	EqParam.Q           = 0.45;
	AudFilt_DesignEQ(1, &EqParam);

	return;
}

static BOOL IPCamExe_AudFiltApply(UINT32 uiInAddr, UINT32 uiOutAddr, UINT32 uiSize)
{
	AUDFILT_BITSTREAM   AudBitStream;
	BOOL    bStatus;

	AudBitStream.uiBsAddrIn     = uiInAddr;
	AudBitStream.uiBsAddrOut    = uiOutAddr;
	AudBitStream.uiBsLength     = uiSize;

	// apply audio filter
	if (!(bStatus = AudFilt_ApplyFiltering(&AudBitStream))) {
		DBG_ERR("Audio filtering err!\r\n");
	}

	return bStatus;
}

AUDIN_AUDFILT_OBJ audfilt_obj = {IPCamExe_AudFiltOpen,
                                 IPCamExe_AudFiltClose,
                                 IPCamExe_AudFiltApply,
                                 IPCamExe_AudFiltDesign
                                };

#endif


static INT32 IPCamExe_InitCommonMem(void)
{

	NVTMPP_ER        ret;
	NVTMPP_VB_CONF_S st_conf;
	UIAPPIPCAM_BLOCK_BUFFER_INFO  *pBlockConfig;
	INT32  i, w, h, j;
	memset(&st_conf, 0, sizeof(NVTMPP_VB_CONF_S));
	st_conf.ddr_mem[0].Addr = OS_GetMempoolAddr(POOL_ID_APP);
	st_conf.ddr_mem[0].Size = OS_GetMempoolSize(POOL_ID_APP);

#if _TODO
	if (POOL_ID_APP_ARBIT2)
#else
	if (0)
#endif
	{
		st_conf.ddr_mem[1].Addr = OS_GetMempoolAddr(POOL_ID_APP_ARBIT2);
		st_conf.ddr_mem[1].Size = OS_GetMempoolSize(POOL_ID_APP_ARBIT2);
	}
	st_conf.max_pool_cnt = 64;

	pBlockConfig = UIAppIPCam_get_BlockBufferInfo();

	//calculate require block size
	j = 0;
	for (i = 0; i < NVTMPP_VB_MAX_COMM_POOLS; i++) {
		UINT32 blk_size = 0;
		if (pBlockConfig[i].isEnabled == FALSE) {
			continue;
		}
		w = ALIGN_CEIL_16(pBlockConfig[i].sResolution.w);
		h = ALIGN_CEIL_16(pBlockConfig[i].sResolution.h);

#if defined(_BSP_NA51023_)
	#if (IPCAM_DIS_COMP_FUNC == ENABLE)
		w = (pBlockConfig[i].sResolution.w * 110) / 100;
		h = (pBlockConfig[i].sResolution.h * 110) / 100;
		w = ALIGN_CEIL_16(w);
		h = ALIGN_CEIL_16(h);
	#endif
#endif
		switch (pBlockConfig[i].defaultFormat) {
//#if defined(_BSP_NA51000_)
//#if (IPCAM_OD_FUNC == ENABLE)
//		case IPCAM_FMT_ODRL:
//			blk_size = 320*240*2; //sizeof(RIFF_CHUNK)+sizeof(VIDENC_SMART_ROI_INFO);
//			break;
//#endif
//#endif
		case IPCAM_FMT_420:
			blk_size = (ALIGN_CEIL_64(w) * h * 3 / 2);
			break;
		case IPCAM_FMT_422:
			blk_size = (ALIGN_CEIL_64(w) * h * 2);
			break;
		case IPCAM_FMT_RAW12:
			blk_size = (ALIGN_CEIL_8(w*12) * h / 8)+10*1024;
			break;
#if defined(_BSP_NA51023_)
		case IPCAM_FMT_420_MD:
			blk_size = (ALIGN_CEIL_64(w) * h * 3 / 2);
			blk_size += md_get_evt_size(h); //additional metadata buffer
			break;
		case IPCAM_FMT_422_MD:
			blk_size = (ALIGN_CEIL_64(w) * h * 2);
			blk_size += md_get_evt_size(h); //additional metadata buffer
			break;
#endif
		default:
			blk_size = 0;
			break;
		}
#if defined(_BSP_NA51023_)
		if (pBlockConfig[i].bCompress) {
			blk_size = blk_size * 3 / 4;
		}
#endif
		st_conf.common_pool[j].blk_size = blk_size + 1024;
		st_conf.common_pool[j].blk_cnt = pBlockConfig[i].count;
		st_conf.common_pool[j].ddr = pBlockConfig[i].ddr;
		#if (USE_IPC_RESERVED_MEM == ENABLE)
		st_conf.use_reserved_mem = 1;
		#endif
		j++;
	}


#if _TODO
	if (POOL_ID_APP_ARBIT2)
#else
	if (0)
#endif
	{
		st_conf.common_pool[2].blk_size = 640 * 480 * 2 + 1024;
		st_conf.common_pool[2].blk_cnt = 2;
		st_conf.common_pool[2].ddr = NVTMPP_DDR_2;
	}
	ret = nvtmpp_vb_set_conf(&st_conf);
	if (NVTMPP_ER_OK != ret) {
		DBG_ERR("nvtmpp set vb err: %d\r\n", ret);
		return -1;
	}
	ret = nvtmpp_vb_init();
	if (NVTMPP_ER_OK != ret) {
		DBG_ERR("nvtmpp init vb err: %d\r\n", ret);
		return -1;
	}

	SxCmd_DoCommand("nvtmpp status"); //show init buffer status

	return 0;

}

static void IPCamExe_SetStreamParam(void)
{

	/////set streaming structure
	IPCAM_STREAM_INFO *pStreamInfo = UIAppIPCam_get_streamInfo();
	UIAppIPCam_set_StreamInfo(pStreamInfo);
	INT32 dispPath = UIAppIPCam_get_DisplayPath();
	INT32 ALGPath = UIAppIPCam_get_ALGPath();
	INT32 YUVPath = UIAppIPCam_get_YUVPath();
	INT32 RAWPath = UIAppIPCam_get_RAWPath();

	///set raw path
	UIAppIPCam_set_RAWPath(RAWPath);

	///set display path
	UIAppIPCam_set_DisplayPath(dispPath);

	///set ALG path
	UIAppIPCam_set_ALGPath(ALGPath);

	///set YUV path
	UIAppIPCam_set_YUVPath(YUVPath);


	if(RAWPath!= -1){
		//raw enable, need set to IMEout4 and RAW type
		IPCAM_STREAM_INFO *pStreamInfo = UIAppIPCam_get_streamInfo();
		UIAPPIPCAM_SENSOR_INFO*	pSensor = UIAppIPCam_get_SensorInfo();
		if(pStreamInfo[RAWPath].enable == IPCAM_STATE_DISABLE){
			DBG_ERR("path raw:%d not enable!\r\n",RAWPath);

		}
		else{
			// raw path need set to IME4
			if(pStreamInfo[RAWPath].ime != IPCAM_IME4){
				DBG_ERR("raw path need be IPCAM_IME4\r\n");
			}
			else{
				if(pSensor[pStreamInfo[RAWPath].sensor].eVideoFormat[IPCAM_IME4] != IPCAM_FMT_RAW8 &&
					pSensor[pStreamInfo[RAWPath].sensor].eVideoFormat[IPCAM_IME4] != IPCAM_FMT_RAW10 &&
					pSensor[pStreamInfo[RAWPath].sensor].eVideoFormat[IPCAM_IME4] != IPCAM_FMT_RAW12 &&
					pSensor[pStreamInfo[RAWPath].sensor].eVideoFormat[IPCAM_IME4] != IPCAM_FMT_RAW16){
					DBG_ERR("raw path format should be IPCAM_FMT_RAW8,IPCAM_FMT_RAW10,IPCAM_FMT_RAW12,IPCAM_FMT_RAW16\r\n");
				}
			}

		}

	}

	if(YUVPath != -1){
		UIAPPIPCAM_ENCODE_CONFIG *pEncodeInfo = UIAppIPCam_get_EncodeConfig();
		pEncodeInfo[YUVPath].codec = IPCAMCODEC_YUV;

		////set streamsender common pool enable for linux
		DBG_DUMP("enable streamsender common pool\r\n");
		ImageUnit_Begin(&ISF_StreamSender, 0);
			ImageUnit_SetParam(ISF_CTRL, STMSND_PARAM_SET_COMM_POOL_ENABLE, TRUE);
		ImageUnit_End();

	}
	else{
		////set streamsender common pool disable for linux
		ImageUnit_Begin(&ISF_StreamSender, 0);
			ImageUnit_SetParam(ISF_CTRL, STMSND_PARAM_SET_COMM_POOL_ENABLE, FALSE);
		ImageUnit_End();
	}

}

static void IPCamExe_SetDispParam(void)
{

	if (System_GetEnableDisp() & DISPLAY_1) {
		ImageUnit_Begin(&ISF_VdoOut1, 0);
		ImageUnit_SetVdoImgSize(ISF_IN1, 0, 0); //buffer size = full device size
		USIZE DeviceRatio;
		DeviceRatio = GxVideo_GetDeviceAspect(0);
		ImageUnit_SetVdoAspectRatio(ISF_IN1, DeviceRatio.w, DeviceRatio.h);
		ImageUnit_SetVdoPreWindow(ISF_IN1, 0, 0, 0, 0);  //window range = full device range
#if (IPCAM_DISP_WAIT == ENABLE)
		ImageUnit_SetParam(ISF_IN1, VDOOUT_PARAM_FUNC, 0);
#else
		ImageUnit_SetParam(ISF_IN1, VDOOUT_PARAM_FUNC, VDOOUT_FUNC_NOWAIT);
#endif
		ImageUnit_End();

	}
	if (System_GetEnableDisp() & DISPLAY_2) {
		ImageUnit_Begin(&ISF_VdoOut2, 0);
		ImageUnit_SetVdoImgSize(ISF_IN1, 0, 0); //buffer size = full device size
		USIZE DeviceRatio2;
		DeviceRatio2 = GxVideo_GetDeviceAspect(1);
		ImageUnit_SetVdoAspectRatio(ISF_IN1, DeviceRatio2.w, DeviceRatio2.h);
		ImageUnit_SetVdoPreWindow(ISF_IN1, 0, 0, 0, 0);  //window range = full device range
#if (IPCAM_DISP_WAIT == ENABLE)
		ImageUnit_SetParam(ISF_IN1, VDOOUT_PARAM_FUNC, 0);
#else
		ImageUnit_SetParam(ISF_IN1, VDOOUT_PARAM_FUNC, VDOOUT_FUNC_NOWAIT);
#endif
		ImageUnit_End();
	}



}

#if 0//(IPCAM_AUDIO_IN_FUNC == ENABLE) || (IPCAM_AUDIO_OUT_FUNC == ENABLE)
static INT32 IPCamExe_CreateAudioPool(ISF_UNIT *pUnit, NVTMPP_VB_POOL *pool,UINT32 Max_Size,UINT32 *addr){


	NVTMPP_VB_BLK  blk;
	INT32 ret;
	char PoolName[16]={0};

	////if pool already be created, free it first
	if(*pool != NVTMPP_VB_INVALID_POOL){

		ret = nvtmpp_vb_destroy_pool(*pool);
		if (NVTMPP_ER_OK != ret) {
			DBG_ERR("destory pool error pool 0x%x   ret=%d\r\n", *pool, ret);
		}
	}

	sprintf(PoolName,"%s", pUnit->Name);
	*pool = nvtmpp_vb_create_pool(PoolName, Max_Size, 1, NVTMPP_DDR_1);

	if (NVTMPP_VB_INVALID_POOL == *pool) {
		DBG_ERR("create private pool err \r\n");
		return -1;
	}

	blk = nvtmpp_vb_get_block( 0, *pool, Max_Size, NVTMPP_DDR_1);

	if (NVTMPP_VB_INVALID_BLK != blk) {
		*addr = nvtmpp_vb_block2addr(blk);
	} else {
		DBG_ERR("%s get blk failed\r\n", pUnit->Name);
		if (nvtmpp_vb_destroy_pool(*pool) != NVTMPP_ER_OK) {
			DBG_ERR("%s destroy mpl failed!\r\n", pUnit->Name);
		}
		return -1;
	}

	return 0;
}

#endif

UINT32 IPCamExe_GetVdoEncBufFrames(UINT32 path)
{
	UIAPPIPCAM_ENCODE_CONFIG *pEncodeConfig = UIAppIPCam_get_EncodeConfig();
	UIAPPIPCAM_BRC_NONE_CONFIG *pBRCNoneConfig = UIAppIPCam_get_BRCNoneInfo();
	UIAPPIPCAM_VBR_CONFIG *pBRCVBRConfig = UIAppIPCam_get_VBRInfo();
	UIAPPIPCAM_EVBR_CONFIG *pBRCEVBRConfig = UIAppIPCam_get_EVBRInfo();
	UIAPPIPCAM_CBR_CONFIG *pBRCCBRConfig = UIAppIPCam_get_CBRInfo();
	UIAPPIPCAM_FIXQP_CONFIG *pBRCFIXEDConfig = UIAppIPCam_get_FixQPInfo();

	UINT32 encsec = 0;
	UINT32 fps=0;

	encsec = ImageUnit_GetParam(&ISF_VdoEnc, path + ISF_OUT_BASE ,VDOENC_PARAM_ENCBUF_MS)/1000;

	if(pEncodeConfig[path].eBrcType == UIAPPIPCAM_BRC_NONE){
		fps = pBRCNoneConfig[path].uiFrameRate;
	}
	else if(pEncodeConfig[path].eBrcType == UIAPPIPCAM_BRC_CBR){
		fps = pBRCCBRConfig[path].cbr_info.uiFrameRate;
	}
	else if(pEncodeConfig[path].eBrcType == UIAPPIPCAM_BRC_VBR){
		fps = pBRCVBRConfig[path].vbr_info.uiFrameRate;
	}
	else if(pEncodeConfig[path].eBrcType == UIAPPIPCAM_BRC_FIXQP){
		fps  =pBRCFIXEDConfig[path].sFixQPInfo.uiFrameRate;
	}
	else if(pEncodeConfig[path].eBrcType == UIAPPIPCAM_BRC_EVBR){
		fps = pBRCEVBRConfig[path].evbr_info.uiFrameRate;
	}
	else{
		DBG_ERR("path %d get brc type error %d\r\n",path,pEncodeConfig[path].eBrcType);
		return 0;
	}

	return (encsec*fps);
}

static void IPCamExe_SetVideoMaxBuf(UINT32 OutPortIndex){
	IPCAM_STREAM_INFO *pStreamInfo = UIAppIPCam_get_streamInfo();
	UIAPPIPCAM_VIDEO_MAXCONFIG *pVdoMaxConfig = UIAppIPCam_get_VdoMaxConfig();

	INT32 DisplayPath = UIAppIPCam_get_DisplayPath();
	INT32 AlgPath     = UIAppIPCam_get_ALGPath();
	INT32 YUVPath = UIAppIPCam_get_YUVPath();
	UINT32 path = OutPortIndex - ISF_OUT_BASE;;
	NMI_VDOENC_MAX_MEM_INFO MaxMemInfo = {0};

	if(   (DisplayPath == (INT32)path) ||
	      (pStreamInfo[path].enable != IPCAM_STATE_ENABLE) ||
	      (AlgPath == (INT32)path)||
	      (YUVPath == (INT32)path)){
		return;
	}

	MaxMemInfo.uiCodec          = pVdoMaxConfig[path].sMaxCodec;
	MaxMemInfo.uiWidth          = pVdoMaxConfig[path].uiMaxWidth;
	MaxMemInfo.uiHeight         = pVdoMaxConfig[path].uiMaxHeight;
	MaxMemInfo.uiTargetByterate = pVdoMaxConfig[path].uiMaxByteRate;
	MaxMemInfo.uiEncBufMs       = pVdoMaxConfig[path].uiMaxBufMs;
	MaxMemInfo.uiRecFormat      = pVdoMaxConfig[path].uiMaxRecFormat;
	MaxMemInfo.uiSVCLayer       = pVdoMaxConfig[path].uiMaxSVCLayer;
	MaxMemInfo.uiLTRInterval	= pVdoMaxConfig[path].uiLTRInterval;
	MaxMemInfo.uiRotate         = pVdoMaxConfig[path].uiRotate;
	MaxMemInfo.bAllocSnapshotBuf= pVdoMaxConfig[path].bAllocSnapshotBuf;

	ImageUnit_SetParam(OutPortIndex, VDOENC_PARAM_MAX_MEM_INFO, (UINT32) &MaxMemInfo);
}

static void IPCamExe_FreeVideoMaxBuf(UINT32 OutPortIndex){
	NMI_VDOENC_MAX_MEM_INFO MaxMemInfo = {0};
	MaxMemInfo.bRelease = TRUE;
	ImageUnit_Begin(&ISF_VdoEnc, 0);
	ImageUnit_SetParam(OutPortIndex, VDOENC_PARAM_MAX_MEM_INFO, (UINT32) &MaxMemInfo);
	ImageUnit_End();
}

static void IPCamExe_SetVideoDISParam(UINT32 path){

	UINT32 i=0;
	IPCAM_STREAM_INFO *pStreamInfo = UIAppIPCam_get_streamInfo();
	UIAPPIPCAM_IPL_FUNC *pIPLFuncConfig = UIAppIPCam_get_IPLFuncInfo();
	for(i=0; i < IPCAM_SENSOR_MAX ; i++){
		if(pStreamInfo[path].sensor == i){

			if(pIPLFuncConfig[i].uiDIS == SEL_DISCOMP_ON){
				ImageUnit_SetParam(path + ISF_OUT_BASE, VDOENC_PARAM_DIS, 1); //on
			}
			else{
				ImageUnit_SetParam(path + ISF_OUT_BASE, VDOENC_PARAM_DIS, 0); //off
			}
			break;
		}


	}



}

static void IPCamExe_SetVideoROI(UINT32 path){

	MP_VDOENC_ROI_INFO ROIInfo ={0};
	UIAPPIPCAM_ROI_CONFIG *pROIConfig = UIAppIPCam_get_ROIInfo();
	UINT32 i=0;

	ROIInfo.uiDeltaQp = pROIConfig[path].uiDeltaQp;
	for (i = 0; i < MP_VDOENC_ROI_NUM_MAX; i++){

			ROIInfo.roi[i].uiEnable  = pROIConfig[path].roi[i].uiEnable;
			ROIInfo.roi[i].uiHeight  = pROIConfig[path].roi[i].uiHeight;
			ROIInfo.roi[i].uiWidth   = pROIConfig[path].roi[i].uiWidth;
			ROIInfo.roi[i].iQP      =  pROIConfig[path].roi[i].iQP;
			ROIInfo.roi[i].uiCoord_X = pROIConfig[path].roi[i].uiCoord_X;
			ROIInfo.roi[i].uiCoord_Y = pROIConfig[path].roi[i].uiCoord_Y;

	}
	ImageUnit_SetParam(path+ISF_OUT_BASE,VDOENC_PARAM_ROI,(UINT32)&ROIInfo);


}

static void IPCamExe_SetVideoEncodeParam(void)
{
	/*
	    this function is for ImageUnit_VdoEnc parameters

	    for movie width, height, fps, bitrate...etc

	*/


	IPCAM_STREAM_INFO *pStreamInfo = UIAppIPCam_get_streamInfo();
	UIAPPIPCAM_EVBR_CONFIG *pEVBRConfig = UIAppIPCam_get_EVBRInfo();
	UIAPPIPCAM_CBR_CONFIG *pCBRConfig = UIAppIPCam_get_CBRInfo();
	UIAPPIPCAM_VBR_CONFIG *pVBRConfig = UIAppIPCam_get_VBRInfo();
	UIAPPIPCAM_FIXQP_CONFIG *pFixQPConfig = UIAppIPCam_get_FixQPInfo();
	UIAPPIPCAM_BRC_NONE_CONFIG *pBRCNoneConfig = UIAppIPCam_get_BRCNoneInfo();
	UIAPPIPCAM_ENCODE_CONFIG *pIPCamConfig = UIAppIPCam_get_EncodeConfig();
	UIAPPIPCAM_AQ_CONFIG *pAQInfo = UIAppIPCam_get_AQInfo();
	UIAPPIPCAM_SENSOR_INFO *pSensorInfo;
	UIAPPIPCAM_VIDEO_MAXCONFIG *pVodMaxConfig = UIAppIPCam_get_VdoMaxConfig();

	UINT32 OutPortIndex = 0;
	UINT32 path = 0;
	INT32 DisplayPath = ImageApp_IPCam_GetConfig(IPCAM_DISP_PATH);

	for (OutPortIndex = ISF_OUT1; OutPortIndex < (ISF_OUT_BASE + IPCAM_PATH_MAX_ID); OutPortIndex++) {
		path = OutPortIndex - ISF_OUT_BASE;
		pSensorInfo = UIAppIPCam_get_SensorInfo();

		if (DisplayPath == (INT32)path) {
			//////display path will get resolution by itself
			ISIZE DeviceSize = {0, 0};
			UINT32 DevID = 0; ////IDE id
			DeviceSize = GxVideo_GetDeviceSize(DevID);

			pIPCamConfig[OutPortIndex].resolution.w = DeviceSize.w;
			pIPCamConfig[OutPortIndex].resolution.h = DeviceSize.h;
			DBG_DUMP("set display path resolution %dx%d\r\n",
					 pIPCamConfig[OutPortIndex].resolution.w,
					 pIPCamConfig[OutPortIndex].resolution.h);
		}

		ImageUnit_Begin(&ISF_VdoEnc, 0);
		//ImageUnit_SetVdoBufferCount(path+ISF_IN_BASE, 2);
		ImageUnit_SetVdoBufferCount(path + ISF_IN_BASE, 1); //680 only allow queue 1 buffer!
		ImageUnit_SetVdoImgSize(ISF_IN_BASE + path, pIPCamConfig[OutPortIndex].resolution.w, pIPCamConfig[OutPortIndex].resolution.h);
		ImageUnit_SetParam(OutPortIndex, VDOENC_PARAM_CODEC, pIPCamConfig[OutPortIndex].codec);

		if (pIPCamConfig[OutPortIndex].codec == MEDIAVIDENC_MJPG) {
			#if (VIDEO_FUNC_MJPG == ENABLE)
			MP_VDOENC_ENCODER *pObj = MP_MjpgEnc_getVideoObject(OutPortIndex);
			ImageUnit_SetParam(OutPortIndex, VDOENC_PARAM_ENCODER_OBJ, (UINT32)pObj);
			#else
			ImageUnit_SetParam(OutPortIndex, VDOENC_PARAM_ENCODER_OBJ, 0);
			DBG_ERR("[IPCAM][%d] vdo codec = %d not support\r\n", OutPortIndex, pIPCamConfig[OutPortIndex].codec);
			#endif
		}
		if (pIPCamConfig[OutPortIndex].codec == MEDIAVIDENC_H264) {
			#if (VIDEO_FUNC_H264 == ENABLE)
			MP_VDOENC_ENCODER *pObj = MP_H264Enc_getVideoObject(OutPortIndex);
			ImageUnit_SetParam(OutPortIndex, VDOENC_PARAM_ENCODER_OBJ, (UINT32)pObj);
			#else
			ImageUnit_SetParam(OutPortIndex, VDOENC_PARAM_ENCODER_OBJ, 0);
			DBG_ERR("[IPCAM][%d] vdo codec = %d not support\r\n", OutPortIndex, pIPCamConfig[OutPortIndex].codec);
			#endif
		}
		if (pIPCamConfig[OutPortIndex].codec == MEDIAVIDENC_H265) {
			#if (VIDEO_FUNC_H265 == ENABLE)
			MP_VDOENC_ENCODER *pObj = MP_H265Enc_getVideoObject(OutPortIndex);
			ImageUnit_SetParam(OutPortIndex, VDOENC_PARAM_ENCODER_OBJ, (UINT32)pObj);
			#else
			ImageUnit_SetParam(OutPortIndex, VDOENC_PARAM_ENCODER_OBJ, 0);
			DBG_ERR("[IPCAM][%d] vdo codec = %d not support\r\n", OutPortIndex, pIPCamConfig[OutPortIndex].codec);
			#endif
		}

		ImageUnit_SetParam(OutPortIndex, VDOENC_PARAM_GOPTYPE, MEDIAREC_GOP_IPONLY);
		ImageUnit_SetParam(OutPortIndex, VDOENC_PARAM_PROFILE, pIPCamConfig[OutPortIndex].eProfile);

		ImageUnit_SetParam(OutPortIndex, VDOENC_PARAM_RECFORMAT, MEDIAREC_LIVEVIEW);
		/////set video encode buf size,   size = VDOENC_PARAM_ENCBUF_MS* fps
		ImageUnit_SetParam(OutPortIndex, VDOENC_PARAM_ENCBUF_MS , IPCAM_VDOENC_MAX_BUF_MS);
		ImageUnit_SetParam(OutPortIndex, VDOENC_PARAM_ENCBUF_RESERVED_MS, IPCAM_VDOENC_RESERVED_BUF_MS);
		///init QP , for only first GOP, please do not change these value
		ImageUnit_SetParam(OutPortIndex, VDOENC_PARAM_INITQP, 20);
		ImageUnit_SetParam(OutPortIndex, VDOENC_PARAM_MINQP, 15);
		ImageUnit_SetParam(OutPortIndex, VDOENC_PARAM_MAXQP, 45);

		////set SVC info
		ImageUnit_SetParam(OutPortIndex, VDOENC_PARAM_SVC, pIPCamConfig[OutPortIndex].eSVCType);

		////set SmartROI
		ImageUnit_SetParam(OutPortIndex, VDOENC_PARAM_START_SMART_ROI, pIPCamConfig[OutPortIndex].bSmartROI);

		////set ROI
		IPCamExe_SetVideoROI(path);

		//// disable ImgCap function, this is for dvcam snapshot

		ImageUnit_SetParam(OutPortIndex, VDOENC_PARAM_IMGCAP_FUNC_DISABLE, 1);

		////set LTR
		if(pVodMaxConfig[path].uiLTRInterval > 0){
		    ImageUnit_SetParam(OutPortIndex, VDOENC_PARAM_LTR, (UINT32)&pIPCamConfig[OutPortIndex].sLTRInfo);
		}
		else{
			if(pIPCamConfig[OutPortIndex].sLTRInfo.uiLTRInterval > 0){
				DBG_WRN("LTR buffer not allocate, disable LTR function!\r\n");
				pIPCamConfig[OutPortIndex].sLTRInfo.uiLTRInterval = 0;
				ImageUnit_SetParam(OutPortIndex, VDOENC_PARAM_LTR, (UINT32)&pIPCamConfig[OutPortIndex].sLTRInfo);
			}
			else{
				ImageUnit_SetParam(OutPortIndex, VDOENC_PARAM_LTR, (UINT32)&pIPCamConfig[OutPortIndex].sLTRInfo);
			}

		}

		//set SEI
		ImageUnit_SetParam(OutPortIndex, VDOENC_PARAM_SEI, pIPCamConfig[OutPortIndex].sSEISetting.bEnable);

#if (VDOENC_MULTI_TEMPORARY_LAYER == ENABLE)
		ImageUnit_SetParam(OutPortIndex, VDOENC_PARAM_MULTI_TEMPORARY_LAYER, 1);
#endif

#if (YUV_TV_RANGE == ENABLE)
		ImageUnit_SetParam(OutPortIndex, VDOENC_PARAM_TV_RANGE, 1);
#endif

		switch(OutPortIndex)
		{
		case ISF_OUT1:
#if (IPCAM_MAIN_WAIT == ENABLE)
			ImageUnit_SetParam(OutPortIndex, VDOENC_PARAM_FUNC, 0);
#else
			ImageUnit_SetParam(OutPortIndex, VDOENC_PARAM_FUNC, VDOENC_FUNC_NOWAIT);
#endif
			break;
		default:
			ImageUnit_SetParam(OutPortIndex, VDOENC_PARAM_FUNC, 0);
			break;
		}

		////set AQ info
		ImageUnit_SetParam(OutPortIndex, VDOENC_PARAM_AQINFO, (UINT32)&pAQInfo[path].sAQInfo);
		/////set bitrate control
		if (pIPCamConfig[OutPortIndex].eBrcType == UIAPPIPCAM_BRC_CBR &&
			(pIPCamConfig[OutPortIndex].codec == MEDIAVIDENC_H264 || pIPCamConfig[OutPortIndex].codec == MEDIAVIDENC_H265)) {
			////set fps 1/1, for IPL trigger mode
			if (pStreamInfo[path].trig == IPCAM_TRIGGER_DEST) {
				UINT32 src_fr = pSensorInfo[path].uiFrontFPS;
				UINT32 dst_fr = pCBRConfig[path].cbr_info.uiFrameRate;
				ImageUnit_SetVdoFramerate(path + ISF_IN_BASE, ISF_VDO_FRC(dst_fr, src_fr));
				ImageUnit_SetParam(OutPortIndex, VDOENC_PARAM_FRAMERATE, dst_fr);
			} else {
				ImageUnit_SetVdoFramerate(path + ISF_IN_BASE, ISF_VDO_FRC(1, 1));
			}
			ImageUnit_SetParam(OutPortIndex, VDOENC_PARAM_CBRINFO, (UINT32) &pCBRConfig[OutPortIndex].cbr_info);
			ImageUnit_SetParam(OutPortIndex, VDOENC_PARAM_GOPNUM, pCBRConfig[OutPortIndex].cbr_info.uiGOP);
			ImageUnit_SetParam(OutPortIndex, VDOENC_PARAM_TARGETRATE, pCBRConfig[OutPortIndex].cbr_info.uiByteRate);
			//  ImageUnit_SetParam(OutPortIndex, VDOENC_PARAM_BUFRATE, pCBRConfig[OutPortIndex].cbr_info.uiByteRate*5); //for calculate VdoEnc buff
		} else if (pIPCamConfig[OutPortIndex].eBrcType == UIAPPIPCAM_BRC_VBR &&
				   (pIPCamConfig[OutPortIndex].codec == MEDIAVIDENC_H264 || pIPCamConfig[OutPortIndex].codec == MEDIAVIDENC_H265)) {
			////set fps 1/1, for IPL trigger mode
			if (pStreamInfo[path].trig == IPCAM_TRIGGER_DEST) {
				UINT32 src_fr = pSensorInfo[path].uiFrontFPS;
				UINT32 dst_fr = pVBRConfig[path].vbr_info.uiFrameRate;
				ImageUnit_SetVdoFramerate(path + ISF_IN_BASE, ISF_VDO_FRC(dst_fr, src_fr));
				ImageUnit_SetParam(OutPortIndex, VDOENC_PARAM_FRAMERATE, dst_fr);
			} else {
				ImageUnit_SetVdoFramerate(path + ISF_IN_BASE, ISF_VDO_FRC(1, 1));
			}
			ImageUnit_SetParam(OutPortIndex, VDOENC_PARAM_VBRINFO, (UINT32) &pVBRConfig[OutPortIndex].vbr_info);
			ImageUnit_SetParam(OutPortIndex, VDOENC_PARAM_GOPNUM, pVBRConfig[OutPortIndex].vbr_info.uiGOP);
			ImageUnit_SetParam(OutPortIndex, VDOENC_PARAM_TARGETRATE, pVBRConfig[OutPortIndex].vbr_info.uiByteRate);
			//  ImageUnit_SetParam(OutPortIndex, VDOENC_PARAM_BUFRATE, pVBRConfig[OutPortIndex].vbr_info.uiByteRate*5); //for calculate VdoEnc buff
		} else if (pIPCamConfig[OutPortIndex].eBrcType == UIAPPIPCAM_BRC_FIXQP) {
			////set fps 1/1, for IPL trigger mode
			if (pStreamInfo[path].trig == IPCAM_TRIGGER_DEST) {
				UINT32 src_fr = pSensorInfo[path].uiFrontFPS;
				UINT32 dst_fr = pFixQPConfig[path].sFixQPInfo.uiFrameRate;
				ImageUnit_SetVdoFramerate(path + ISF_IN_BASE, ISF_VDO_FRC(dst_fr, src_fr));
				ImageUnit_SetParam(OutPortIndex, VDOENC_PARAM_FRAMERATE, dst_fr);
			} else {
				ImageUnit_SetVdoFramerate(path + ISF_IN_BASE, ISF_VDO_FRC(1, 1));
			}
			ImageUnit_SetParam(OutPortIndex, VDOENC_PARAM_FIXQPINFO, (UINT32) &pFixQPConfig[OutPortIndex].sFixQPInfo);
			ImageUnit_SetParam(OutPortIndex, VDOENC_PARAM_GOPNUM, pFixQPConfig[OutPortIndex].sFixQPInfo.uiGOP);
			ImageUnit_SetParam(OutPortIndex, VDOENC_PARAM_TARGETRATE, pFixQPConfig[OutPortIndex].sFixQPInfo.uiByteRate);
			//  ImageUnit_SetParam(OutPortIndex, VDOENC_PARAM_BUFRATE, pFixQPConfig[OutPortIndex].sFixQPInfo.uiByteRate*5); //for calculate VdoEnc buff
		} else if (pIPCamConfig[OutPortIndex].eBrcType == UIAPPIPCAM_BRC_NONE) {
			////set fps 1/1, for IPL trigger mode
			if (pStreamInfo[path].trig == IPCAM_TRIGGER_DEST) {
				UINT32 src_fr = pSensorInfo[path].uiFrontFPS;
				UINT32 dst_fr = pBRCNoneConfig[path].uiFrameRate;
				ImageUnit_SetVdoFramerate(path + ISF_IN_BASE, ISF_VDO_FRC(dst_fr, src_fr));
				ImageUnit_SetParam(OutPortIndex, VDOENC_PARAM_FRAMERATE, dst_fr);
			} else {
				ImageUnit_SetVdoFramerate(path + ISF_IN_BASE, ISF_VDO_FRC(1, 1));
			}
			ImageUnit_SetParam(OutPortIndex, VDOENC_PARAM_GOPNUM, pBRCNoneConfig[OutPortIndex].uiGOP);
			ImageUnit_SetParam(OutPortIndex, VDOENC_PARAM_TARGETRATE, pBRCNoneConfig[OutPortIndex].uiTargetByterate);
			//  ImageUnit_SetParam(OutPortIndex, VDOENC_PARAM_BUFRATE, pBRCNoneConfig[OutPortIndex].uiTargetByterate*5); //for calculate VdoEnc buff
		} else if (pIPCamConfig[OutPortIndex].eBrcType == UIAPPIPCAM_BRC_EVBR &&
				   (pIPCamConfig[OutPortIndex].codec == MEDIAVIDENC_H264 || pIPCamConfig[OutPortIndex].codec == MEDIAVIDENC_H265)){

			ImageUnit_SetParam(OutPortIndex, VDOENC_PARAM_EVBRINFO, (UINT32) &pEVBRConfig[OutPortIndex].evbr_info);
			ImageUnit_SetParam(OutPortIndex, VDOENC_PARAM_GOPNUM, pEVBRConfig[OutPortIndex].evbr_info.uiGOP);
			ImageUnit_SetParam(OutPortIndex, VDOENC_PARAM_TARGETRATE, pEVBRConfig[OutPortIndex].evbr_info.uiByteRate);
			if (pStreamInfo[path].trig == IPCAM_TRIGGER_DEST) {
				UINT32 src_fr = pSensorInfo[path].uiFrontFPS;
				UINT32 dst_fr = pEVBRConfig[path].evbr_info.uiFrameRate;
				ImageUnit_SetVdoFramerate(path + ISF_IN_BASE, ISF_VDO_FRC(dst_fr, src_fr));
				ImageUnit_SetParam(OutPortIndex, VDOENC_PARAM_FRAMERATE, dst_fr);
			} else {
				ImageUnit_SetVdoFramerate(path + ISF_IN_BASE, ISF_VDO_FRC(1, 1));
			}

		}else {
			DBG_WRN("path=%d brc type (%d)\r\n", path, pIPCamConfig[OutPortIndex].eBrcType);
		}

		/////set encode 3dnr
		if (pIPCamConfig[OutPortIndex].sNRSetting.bEnable == TRUE) {
			DBG_MSG("enable path %d encoder 3DNR!!\r\n", OutPortIndex);
			ImageUnit_SetParam(OutPortIndex, VDOENC_PARAM_3DNR_CB, (UINT32)IPCamExe_UIDefine_3DNR);
		} else {
			ImageUnit_SetParam(OutPortIndex, VDOENC_PARAM_3DNR_CB, 0);
		}
		////set VdoEnc CB function
		ImageUnit_SetParam(ISF_CTRL, VDOENC_PARAM_EVENT_CB, (UINT32)IPCamExe_VdoEnc_CB);
		ImageUnit_SetParam(ISF_CTRL, VDOENC_PARAM_PROC_CB, (UINT32)IPCamExe_VdoEnc_ProcCB);
		////set snapshot alloc buf
		//ImageUnit_SetParam(OutPortIndex, VDOENC_PARAM_ALLOC_SNAPSHOT_BUF, 1);

		////set video Max buf
		IPCamExe_SetVideoMaxBuf(OutPortIndex);

		///set video max frame number
		ImageUnit_SetParam(OutPortIndex, VDOENC_PARAM_MAX_FRAME_QUEUE, IPCAM_STEAMSENDER_V_SLOT);

		////set video DIS param
		IPCamExe_SetVideoDISParam(path);

		ImageUnit_End();
		ImageApp_IPCam_CfgImgSize(path, pIPCamConfig[OutPortIndex].resolution.w, pIPCamConfig[OutPortIndex].resolution.h);
	}



}


static void IPCamExe_SetVideoFormat(void)
{

	UINT32 path_index = 0;
	UINT32 sensor_index = 0;
	UINT32 ime_index = 0;
	IPCAM_STREAM_INFO *pStreamInfo = UIAppIPCam_get_streamInfo();
	UIAPPIPCAM_ENCODE_CONFIG *pIPCamConfig = UIAppIPCam_get_EncodeConfig();
	INT32 iAlgPath = UIAppIPCam_get_ALGPath();
	INT32 iDispPath = UIAppIPCam_get_DisplayPath();
	INT32 iYUVPath = UIAppIPCam_get_YUVPath();
	UIAPPIPCAM_SENSOR_INFO *pSensorInfo;

	///get senser info
	pSensorInfo = UIAppIPCam_get_SensorInfo();
	if (pSensorInfo == NULL) {
		DBG_ERR("get pSensorInfo error\r\n");
		return;
	}

	ISF_IN ISFInMap[IPCAM_PATH_MAX_ID * 2] = {
		ISF_IN1, ISF_IN2, ISF_IN3, ISF_IN4,
		ISF_IN5, ISF_IN6, ISF_IN7, ISF_IN8,
		ISF_IN9, ISF_IN10, ISF_IN11, ISF_IN12,
		ISF_IN13, ISF_IN14, ISF_IN15, ISF_IN16,
#if defined(_BSP_NA51000_)
		ISF_IN17, ISF_IN18, ISF_IN19, ISF_IN20,
		ISF_IN21, ISF_IN22, ISF_IN23, ISF_IN24,
		ISF_IN25, ISF_IN26, ISF_IN27, ISF_IN28,
		ISF_IN29, ISF_IN30, ISF_IN31, ISF_IN32
#endif
	};

	for (path_index = UIAPPIPCAM_PATH_ID_0; path_index < UIAPPIPCAM_PATH_MAX_ID; path_index++) {
		sensor_index = pStreamInfo[path_index].sensor;
		ime_index = pStreamInfo[path_index].ime;
		if (pStreamInfo[path_index].enable == IPCAM_STATE_ENABLE) {
			//set video format
			ImageUnit_Begin(&ISF_ImgTrans, 0);
			if (pStreamInfo[path_index].type == IPCAM_STREAM_MAIN) {
				ImageUnit_SetVdoImgFormat(ISFInMap[path_index], pSensorInfo[sensor_index].eVideoFormat[ime_index]);
			}
			ImageUnit_End();

			ImageUnit_Begin(&ISF_UserProc, 0);
			ImageUnit_SetVdoImgFormat(ISFInMap[path_index], pSensorInfo[sensor_index].eVideoFormat[ime_index]);
			ImageUnit_End();

			////set IPL codec
			ImageUnit_Begin(ISF_IPL(sensor_index), 0);
			if (pStreamInfo[path_index].type == IPCAM_STREAM_MAIN) {
				if(iAlgPath == (INT32)path_index || iDispPath == (INT32)path_index|| (iYUVPath == (INT32)path_index)){
					ImageUnit_SetParam(path_index + ISF_OUT_BASE, IMAGEPIPE_PARAM_CODEC, 0);
				}
				else{
					if(pIPCamConfig[path_index].codec == MEDIAVIDENC_H264){
						ImageUnit_SetParam(path_index + ISF_OUT_BASE, IMAGEPIPE_PARAM_CODEC, MAKEFOURCC('H', '2', '6', '4'));
					}
					else if(pIPCamConfig[path_index].codec == MEDIAVIDENC_H265){
						ImageUnit_SetParam(path_index + ISF_OUT_BASE, IMAGEPIPE_PARAM_CODEC, MAKEFOURCC('H', '2', '6', '5'));
					}
					else if(pIPCamConfig[path_index].codec == MEDIAVIDENC_MJPG){
						ImageUnit_SetParam(path_index + ISF_OUT_BASE, IMAGEPIPE_PARAM_CODEC, MAKEFOURCC('R', 'J', 'P', 'G'));
					}
					else{
						DBG_ERR("get path:%d codec (%d) error!!\r\n",pIPCamConfig[path_index].codec);
					}
				}
			}
			ImageUnit_End();
		}


	}


}
static void IPCamExe_SetIPLCompress(void)
{

	UINT32 sensor_index = 0;
	UINT32 path_index = 0;

	IPCAM_STREAM_INFO *pStreamInfo = UIAppIPCam_get_streamInfo();
	UIAPPIPCAM_ENCODE_CONFIG *pEncodeConfig = UIAppIPCam_get_EncodeConfig();
	UIAPPIPCAM_SENSOR_INFO *pSensorInfo;
	UINT32 IPL_func2 = 0;

	///get senser info
	pSensorInfo = UIAppIPCam_get_SensorInfo();
	if (pSensorInfo == NULL) {
		DBG_ERR("get pSensorInfo error\r\n");
		return;
	}
	for (path_index = UIAPPIPCAM_PATH_ID_0; path_index < UIAPPIPCAM_PATH_MAX_ID; path_index++) {
		sensor_index = pStreamInfo[path_index].sensor;
		ImageUnit_Begin(&ISF_ImgTrans, 0);
		if ((pSensorInfo[sensor_index].bIME1Compress == TRUE) && (pStreamInfo[path_index].ime == IPCAM_IME1) &&
			(pStreamInfo[path_index].enable == IPCAM_STATE_ENABLE)) {

			ImageUnit_SetParam(path_index + ISF_OUT_BASE, IMGTRANS_PARAM_BYPASS_IMM, TRUE);
		} else {
			ImageUnit_SetParam(path_index + ISF_OUT_BASE, IMGTRANS_PARAM_BYPASS_IMM, FALSE);
		}
		ImageUnit_End();

		ImageUnit_Begin(ISF_IPL(sensor_index), 0);
		if (pSensorInfo[sensor_index].bIME1Compress == TRUE) {
			IPL_func2 |= IMAGEPIPE_FUNC2_YUVCOMPRESS;
			if ((pEncodeConfig[path_index].codec == MEDIAVIDENC_H264) &&
				(pStreamInfo[path_index].enable == IPCAM_STATE_ENABLE) &&
				(pStreamInfo[path_index].ime == IPCAM_IME1)) {
				DBG_DUMP("path%d sensor %d compress Set IPL IME1 H 264 compress!!\r\n", path_index, sensor_index);
				ImageUnit_SetParam(ISF_CTRL, IMAGEPIPE_PARAM_NVX_CODEC, MAKEFOURCC('H', '2', '6', '4'));
			} else if ((pEncodeConfig[path_index].codec == MEDIAVIDENC_H265) &&
					   (pStreamInfo[path_index].enable == IPCAM_STATE_ENABLE) &&
					   (pStreamInfo[path_index].ime == IPCAM_IME1)) {
				DBG_DUMP("path%d sensor %d compress Set IPL IME1 H 265 compress!!\r\n", path_index, sensor_index);
				ImageUnit_SetParam(ISF_CTRL, IMAGEPIPE_PARAM_NVX_CODEC, MAKEFOURCC('H', '2', '6', '5'));
			}
			/*
			else{
			    DBG_MSG("path%d sensor %d no compress Set!!\r\n",path_index,sensor_index);
			    ImageUnit_SetParam(ISF_CTRL, IMAGEPIPE_PARAM_NVX_CODEC, 0 );
			}
			*/
		} else {
			DBG_MSG("path%d sensor %d no compress Set!!\r\n", path_index, sensor_index);
			ImageUnit_SetParam(ISF_CTRL, IMAGEPIPE_PARAM_NVX_CODEC, 0);
		}
#if defined(_BSP_NA51023_)
#if (IPCAM_IPL_DIRECT == ENABLE)
		IPL_func2 |= IMAGEPIPE_FUNC2_DIRECT;
#endif
#if (IPCAM_IPL_ONERAW == ENABLE)
		IPL_func2 |= IMAGEPIPE_FUNC2_ONERAW;
#endif
#if (IPCAM_MAIN_3DNRSHARE == ENABLE)
	#if (IPCAM_MAIN_ONE_BUFFER == ENABLE)
		IPL_func2 |= IMAGEPIPE_FUNC2_TMNRSHARE;
	#else
		DBG_ERR("sensor %d main path is not ONE-buffer! Cannot support TMNR-share!\r\n", sensor_index);
	#endif
#endif
#if ((IPCAM_DIS_COMP_FUNC == ENABLE) || (IPCAM_FTG_FUNC == ENABLE) || (IPCAM_ODT_FUNC == ENABLE))
		IPL_func2 |= IMAGEPIPE_FUNC2_VIEWTRACKING1;
		IPL_func2 |= IMAGEPIPE_FUNC2_VIEWTRACKING2;
		IPL_func2 |= IMAGEPIPE_FUNC2_VIEWTRACKING3;
		IPL_func2 |= IMAGEPIPE_FUNC2_VIEWTRACKING4;
#endif
#endif
#if defined(_BSP_NA51023_)
		IPL_func2 |= IMAGEPIPE_FUNC2_DROPOLDSIZE; //drop old-size frame
#endif
#if defined(_BSP_NA51023_)
#ifdef IPCAM_MD_PATH
#if (IPCAM_MD_FUNC == ENABLE)
		if (IPCAM_MD_PATH == 1) {
			IPL_func2 |= IMAGEPIPE_FUNC2_MOTIONDETECT2;
		} else {
			IPL_func2 |= IMAGEPIPE_FUNC2_MOTIONDETECT3;
		}
#endif
#else
#if (IPCAM_MD_FUNC == ENABLE)
		IPL_func2 |= IMAGEPIPE_FUNC2_MOTIONDETECT3;
#endif
#endif
#endif
#if defined(_BSP_NA51000_)
#if (IPCAM_IPL_ONERAW == ENABLE)
		IPL_func2 |= IMAGEPIPE_FUNC2_ONERAW;
#endif
#if (IPCAM_DIS_COMP_FUNC == ENABLE)
		IPL_func2 |= IMAGEPIPE_FUNC2_VIEWTRACKING1;
		IPL_func2 |= IMAGEPIPE_FUNC2_VIEWTRACKING2;
		IPL_func2 |= IMAGEPIPE_FUNC2_VIEWTRACKING3;
		IPL_func2 |= IMAGEPIPE_FUNC2_VIEWTRACKING4;
#endif
#endif
		ImageUnit_SetParam(ISF_CTRL, IMAGEPIPE_PARAM_MAX_FUNC2, IPL_func2);
		ImageUnit_SetParam(ISF_CTRL, IMAGEPIPE_PARAM_FUNC2, IPL_func2);
		ImageUnit_End();
	}


}

static void IPCamExe_SetEndFPS(void){


	UINT32 path=0;
	IPCAM_STREAM_INFO *pStreamInfo = UIAppIPCam_get_streamInfo();
	UIAPPIPCAM_SENSOR_INFO *pSensorInfo = UIAppIPCam_get_SensorInfo();
	UIAPPIPCAM_ENCODE_CONFIG *pEncodeConfig = UIAppIPCam_get_EncodeConfig();
	UIAPPIPCAM_FIXQP_CONFIG *pFixQPConfig = UIAppIPCam_get_FixQPInfo();
	UIAPPIPCAM_CBR_CONFIG *pCBRConfig = UIAppIPCam_get_CBRInfo();
	UIAPPIPCAM_VBR_CONFIG *pVBRConfig = UIAppIPCam_get_VBRInfo();
	UIAPPIPCAM_EVBR_CONFIG *pEVBRConfig = UIAppIPCam_get_EVBRInfo();
	UIAPPIPCAM_BRC_NONE_CONFIG *BRCNONEConfig = UIAppIPCam_get_BRCNoneInfo();


	UINT32 sensor=0;
	UINT32 ime = 0;
	UINT32 EndFPS=0;
	UINT32 FrontFPS=0;
	for(path =0 ; path <UIAPPIPCAM_PATH_MAX_ID; path++ ){
		sensor = pStreamInfo[path].sensor;
		ime = pStreamInfo[path].ime;
		EndFPS = pSensorInfo[sensor].uiIMEFPS[ime];
		FrontFPS = pSensorInfo[sensor].uiFrontFPS;
		ImageUnit_Begin(&ISF_ImgTrans, 0);
		if (pStreamInfo[path].trig == IPCAM_TRIGGER_DEST) {
			ImageUnit_SetVdoFramerate(path + ISF_IN_BASE, ISF_VDO_FRC_NOTIFY);
		} else {
			ImageUnit_SetVdoFramerate(path + ISF_IN_BASE, ISF_VDO_FRC(EndFPS, FrontFPS));
		}
		ImageUnit_End();


		////set end fps
		ImageUnit_Begin(&ISF_UserProc, 0);
			if(pEncodeConfig[path].eBrcType == UIAPPIPCAM_BRC_CBR){
				ImageUnit_SetVdoFramerate(path+ISF_IN_BASE, ISF_VDO_FRC(pCBRConfig[path].cbr_info.uiFrameRate, EndFPS));
			}
			else if(pEncodeConfig[path].eBrcType == UIAPPIPCAM_BRC_VBR){
				ImageUnit_SetVdoFramerate(path+ISF_IN_BASE, ISF_VDO_FRC(pVBRConfig[path].vbr_info.uiFrameRate, EndFPS));
			}
			else if(pEncodeConfig[path].eBrcType == UIAPPIPCAM_BRC_FIXQP){
				ImageUnit_SetVdoFramerate(path+ISF_IN_BASE, ISF_VDO_FRC(pFixQPConfig[path].sFixQPInfo.uiFrameRate, EndFPS));
			}
			else if(pEncodeConfig[path].eBrcType == UIAPPIPCAM_BRC_NONE){
				ImageUnit_SetVdoFramerate(path+ISF_IN_BASE, ISF_VDO_FRC(BRCNONEConfig[path].uiFrameRate, EndFPS));
			}
			else if(pEncodeConfig[path].eBrcType == UIAPPIPCAM_BRC_EVBR){
				ImageUnit_SetVdoFramerate(path+ISF_IN_BASE, ISF_VDO_FRC(pEVBRConfig[path].evbr_info.uiFrameRate, EndFPS));
			}
			else{
				DBG_ERR("pEncodeConfig[%d].eBrcType=%d error\r\n",path,pEncodeConfig[path].eBrcType);
			}
		ImageUnit_End();
	}


}


static void IPCamExe_SetMaxIPLFuncParam(UINT32 sensor_index){

	UIAPPIPCAM_SENSOR_INFO *pSensorInfo;
	UINT32 MAX_IPL_FUNC = 0;

	///get senser info
	pSensorInfo = UIAppIPCam_get_SensorInfo();
	if (pSensorInfo == NULL) {
		DBG_ERR("get pSensorInfo error\r\n");
		return;
	}
	////set MAX IPL_Func for create buffer
	#if (IPCAM_SQUARE_3DNR ==ENABLE)
		MAX_IPL_FUNC |= IMAGEPIPE_FUNC_SQUARE; //square
	#endif

	#if (IPCAM_DEPTH_FUNC == ENABLE)
		MAX_IPL_FUNC |= IPL_FUNC_NVTDEPTH;
	#endif

	#if(IPCAM_DEFOG_FUNC == ENABLE)
		MAX_IPL_FUNC |= IPL_FUNC_DEFOG;
	#endif

	#if(IPCAM_WDR_FUNC == ENABLE)
		MAX_IPL_FUNC |= IPL_FUNC_WDR;
	#endif

	#if((IPCAM_DIS_MV_FUNC == ENABLE))
		MAX_IPL_FUNC |= IPL_FUNC_SIE_CH4_ETH;
	#endif

	#if (VIDEOSPRITE_MOSAIC == ENABLE)
		MAX_IPL_FUNC |= IPL_FUNC_PRIMACY_MASK;
	#endif

	#if (IPCAM_SHDR_FUNC == ENABLE)

		MAX_IPL_FUNC |= IPL_FUNC_SHDR;
		////need set max frame number
		//UIAPPIPCAM_IPL_FUNC *pIPLFunc = UIAppIPCam_get_IPLFuncInfo();
		ImageUnit_SetParam(ISF_CTRL, IPL_SEL_SHDR_FRAME_NUM, IPCAM_SHDR_MAX_FRAME);
	#endif



	DBG_DUMP("set Max IPL%d Func:%x\r\n",sensor_index,MAX_IPL_FUNC);
	ImageUnit_SetParam(ISF_CTRL, IMAGEPIPE_PARAM_MAX_FUNC, MAX_IPL_FUNC);



}



static void IPCamExe_SetIPLParam(void)
{

	UIAPPIPCAM_SENSOR_INFO *pSensorInfo;
	UIAPPIPCAM_IPL_FUNC *pIPLFuncInfo;
	UIAPPIPCAM_IME_SCALE_TYPE *pIMEScaleInfo =  UIAppIPCam_get_IMEScaleInfo();
	UINT32 sensor_index = 0;
	//UINT32 IME_index=0;
	UINT32 IPL_FUNC=0;
	UINT32 FrontFPS=0;

	///get senser info
	pSensorInfo = UIAppIPCam_get_SensorInfo();
	if (pSensorInfo == NULL) {
		DBG_ERR("get pSensorInfo error\r\n");
		return;
	}

	//get IPL func info
	pIPLFuncInfo= UIAppIPCam_get_IPLFuncInfo();
	if (pIPLFuncInfo == NULL) {
		DBG_ERR("get pIPLFuncInfo error\r\n");
		return;
	}


	///////set IPL MAX buffer
	//ImageApp_IPCam_Config(IPCAM_CFG_IPL_MAXBUF, 5);
	////set sensor number
	//ImageApp_IPCam_Config(IPCAM_SENSOR_NUMBER, SENSOR_CAPS_COUNT);

	for (sensor_index = 0; sensor_index < IPCAM_SENSOR_MAX; sensor_index++) {

        if (ISF_IPL(sensor_index) == NULL)
			break;
		ImageUnit_Begin(ISF_IPL(sensor_index), 0);
		/////set IPL SIZE
		//ImageUnit_SetVdoImgSize(ISF_IN1, 1280, 720); //user perferred IPL input w,h (= main stream size)
		/////set IPL FPS front
		FrontFPS = pSensorInfo[sensor_index].uiFrontFPS;
		ImageUnit_SetVdoFramerate(ISF_IN1, ISF_VDO_FRC(FrontFPS,1)); //user perferred IPL input fps (= main stream fps)

		/////set IPL ratio
		ImageUnit_SetVdoAspectRatio(ISF_IN1, pSensorInfo[sensor_index].sRatio.w, pSensorInfo[sensor_index].sRatio.h);

		///set IME 3DNR
		#if (IPCAM_SQUARE_3DNR == ENABLE)
			if (pSensorInfo[sensor_index].bIME3DNR == TRUE) {
				DBG_MSG("enable sensor %d IME_3dnr\r\n", sensor_index);
				IPL_FUNC |= IMAGEPIPE_FUNC_SQUARE;
			}
		#endif
#if defined(_BSP_NA51023_)
#if (IPCAM_MD_FUNC == ENABLE)
		DBG_MSG("enable MD\r\n");
		IPL_FUNC |= IPL_FUNC_MD;
#endif
#endif

		///set IPCam Depth
		#if (IPCAM_DEPTH_FUNC == ENABLE)
			IPL_FUNC |= IPL_FUNC_NVTDEPTH;
		#endif
		///set IPL HDR
		#if(IPCAM_SHDR_FUNC == ENABLE)
			if(pIPLFuncInfo[sensor_index].sHDR.uiHDRMode == SEL_SHDR_ON){
				DBG_MSG("IPL%d HDR enable\r\n",sensor_index);
				IPL_FUNC |= IPL_FUNC_SHDR;
			}
			ImageUnit_SetParam(ISF_CTRL, IPL_SEL_SHDR, pIPLFuncInfo[sensor_index].sHDR.uiHDRMode);
			ImageUnit_SetParam(ISF_CTRL, IPL_SEL_SHDR_FRAME_NUM, pIPLFuncInfo[sensor_index].sHDR.uiFrameNum);
		#endif
		///set IPL defog
		#if (IPCAM_DEFOG_FUNC == ENABLE)
			if(pIPLFuncInfo[sensor_index].sDefog.eDeFogMode == SEL_DEFOG_OFF){
				ImageUnit_SetParam(ISF_CTRL, IPL_SEL_DEFOG_IMM, SEL_DEFOG_OFF);
			}
			else{
				IPL_FUNC |= IPL_FUNC_DEFOG;
				ImageUnit_SetParam(ISF_CTRL, IPL_SEL_DEFOG_IMM, SEL_DEFOG_ON);
				ImageUnit_SetParam(ISF_CTRL, IPL_SEL_DEFOG_LEVEL_IMM, pIPLFuncInfo[sensor_index].sDefog.uiLevel);
			}
		#endif
		///set IPL DIS
#if (IPCAM_DIS_MV_FUNC == ENABLE)
		IPL_FUNC |= IPL_FUNC_SIE_CH4_ETH;
#endif
		if(pIPLFuncInfo[sensor_index].uiDIS == SEL_DISCOMP_ON){
			DBG_MSG("IPL%d DIS enable\r\n",sensor_index);
			IPL_FUNC |= IPL_FUNC_SIE_CH4_ETH;
			ImageUnit_SetParam(ISF_CTRL, IPL_SEL_DISCOMP_IMM, SEL_DISCOMP_ON);
		}
		else{
			ImageUnit_SetParam(ISF_CTRL, IPL_SEL_DISCOMP_IMM, SEL_DISCOMP_OFF);
		}

		///set LCE
		#if(IPCAM_LCE == ENABLE)
			IPL_FUNC |= IPL_FUNC_WDR;/// LCE need WDR func
		#endif


		#if (VIDEOSPRITE_MOSAIC == ENABLE)
			IPL_FUNC |= IPL_FUNC_PRIMACY_MASK;
		#endif

		#if(IPCAM_WDR_FUNC == ENABLE)
			if(pIPLFuncInfo[sensor_index].sWDR.uiWDRMode == SEL_WDR_OFF ){
				ImageUnit_SetParam(ISF_CTRL, IPL_SEL_WDR_IMM, SEL_WDR_OFF);

			}
			else if(pIPLFuncInfo[sensor_index].sWDR.uiWDRMode == SEL_WDR_AUTO){
				DBG_MSG("ipl%d enable AUTO WDR\r\n",sensor_index);
				IPL_FUNC |= IPL_FUNC_WDR;
				ImageUnit_SetParam(ISF_CTRL, IPL_SEL_WDR_IMM, SEL_WDR_AUTO);
				ImageUnit_SetParam(ISF_CTRL, IPL_SEL_WDR_M_LEVEL_IMM, pIPLFuncInfo[sensor_index].sWDR.uiWDRLevel);

			}
			else if(pIPLFuncInfo[sensor_index].sWDR.uiWDRMode == SEL_WDR_MANUAL ){
				DBG_MSG("ipl%d enable MANUAL WDR\r\n",sensor_index);
				IPL_FUNC |= IPL_FUNC_WDR;
				ImageUnit_SetParam(ISF_CTRL, IPL_SEL_WDR_IMM, SEL_WDR_MANUAL);
				ImageUnit_SetParam(ISF_CTRL, IPL_SEL_WDR_M_LEVEL_IMM, pIPLFuncInfo[sensor_index].sWDR.uiWDRLevel);
			}
			else{
				DBG_ERR("set IPL%d WDR error (%d)\r\n",sensor_index,pIPLFuncInfo[sensor_index].sWDR.uiWDRMode);
			}
		#endif
		////set IPL func Max info
		IPCamExe_SetMaxIPLFuncParam(sensor_index);
		//ImageUnit_SetParam(ISF_CTRL, IMAGEPIPE_PARAM_MAX_FUNC, IPL_FUNC);
		ImageUnit_SetParam(ISF_CTRL, IMAGEPIPE_PARAM_FUNC, IPL_FUNC);



		////set IPL IME scale info
		ImageUnit_SetParam(ISF_CTRL, IMAGEPIPE_PARAM_SCALE_QUALITY_RATIO, pIMEScaleInfo[sensor_index].uiRatioThreshold);
		ImageUnit_SetParam(ISF_CTRL, IMAGEPIPE_PARAM_SCALE_MATHOD_SMALL, pIMEScaleInfo[sensor_index].eMathodSmall);
		ImageUnit_SetParam(ISF_CTRL, IMAGEPIPE_PARAM_SCALE_MATHOD_LARGE, pIMEScaleInfo[sensor_index].eMathodLarge);

		ImageUnit_End();


		/////set IPL mirror and flip
		if(pSensorInfo[sensor_index].eFlipType != FLIP_NONE){
			DBG_MSG("set sensor %d flip=%d\r\n",sensor_index,pSensorInfo[sensor_index].eFlipType);
		}
        ImageUnit_Begin(ISF_IPL(sensor_index), 0);
           	if (pSensorInfo[sensor_index].eFlipType == FLIP_H_V) {
           		ImageUnit_SetVdoDirection(ISF_IN1, ISF_VDO_DIR_MIRRORX|ISF_VDO_DIR_MIRRORY);
           	} else if (pSensorInfo[sensor_index].eFlipType == FLIP_H) {
           		ImageUnit_SetVdoDirection(ISF_IN1, ISF_VDO_DIR_MIRRORX);
           	} else if (pSensorInfo[sensor_index].eFlipType == FLIP_V) {
           		ImageUnit_SetVdoDirection(ISF_IN1, ISF_VDO_DIR_MIRRORY);
           	} else {
           		ImageUnit_SetVdoDirection(ISF_IN1, ISF_VDO_DIR_NONE);
           	}

		ImageUnit_End();
	}

    // set  FPS
    IPCamExe_SetEndFPS();

	//set IPL video format
	IPCamExe_SetVideoFormat();

	//set IPL compress (only for IME 1)
	IPCamExe_SetIPLCompress();

	//set IPL CB
	IPCam_RegCB();

}

static void IPCamExe_SetRTSPParam(void)
{

#if(FORCE_UNPLUG_RTSP == ENABLE)
	ImageApp_IPCam_Config(IPCAM_CFG_PLUG_RTSP, 0);
#else
	ImageApp_IPCam_Config(IPCAM_CFG_PLUG_RTSP, (UINT32) &ISF_NetRTSP);
	ImageUnit_Begin(&ISF_NetRTSP, 0);
		ImageUnit_SetParam(ISF_CTRL, NETRTSP_PARAM_MAX_CLIENT, RTSP_MAX_CLIENT);
		#if(RTSP_AUDIO_ENABLE == ENABLE)
			ImageUnit_SetParam(ISF_CTRL, NETRTSP_PARAM_AUDIO, 1);
		#else
			ImageUnit_SetParam(ISF_CTRL, NETRTSP_PARAM_AUDIO, 0);
		#endif
		ImageUnit_SetParam(ISF_CTRL, NETRTSP_PARAM_MEDIA_SOURCE, RTSPNVT_MEDIA_SOURCE_BY_URL);
		ImageUnit_SetParam(ISF_CTRL, NETRTSP_PARAM_IFRAME_REQ_CB, (UINT32)IPCamExe_RtspIFramReq_CB);
		//set VI WMM category, refer to http://sourcedaddy.com/windows-7/selecting-dscp-values.html
		ImageUnit_SetParam(ISF_CTRL, NETRTSP_PARAM_TOS, (0x20) << 2);


		/////set FPS param
		UINT32 i=0;
		IPCAM_STREAM_INFO *pStreamInfo = UIAppIPCam_get_streamInfo();
		UIAPPIPCAM_ENCODE_CONFIG *pEncodeConfig = UIAppIPCam_get_EncodeConfig();
		UIAPPIPCAM_CBR_CONFIG *pCBRConfig = UIAppIPCam_get_CBRInfo();
		UIAPPIPCAM_VBR_CONFIG *pVBRConfig = UIAppIPCam_get_VBRInfo();
		UIAPPIPCAM_EVBR_CONFIG *pEVBRConfig = UIAppIPCam_get_EVBRInfo();
		UIAPPIPCAM_BRC_NONE_CONFIG *pNONEConfig = UIAppIPCam_get_BRCNoneInfo();
		UIAPPIPCAM_FIXQP_CONFIG *pFIXQPConfig  = UIAppIPCam_get_FixQPInfo();

		UINT32 tmp_FPS =0;


		for(i=0; i < IPCAM_PATH_MAX_ID ; i++){
			if(pStreamInfo[i].enable != IPCAM_STATE_ENABLE){
				continue;
			}

			if(pEncodeConfig[i].eBrcType == UIAPPIPCAM_BRC_NONE){
				tmp_FPS = pNONEConfig[i].uiFrameRate;
			}
			else if(pEncodeConfig[i].eBrcType == UIAPPIPCAM_BRC_CBR){
				tmp_FPS = pCBRConfig[i].cbr_info.uiFrameRate;
			}
			else if(pEncodeConfig[i].eBrcType == UIAPPIPCAM_BRC_VBR){
				tmp_FPS = pVBRConfig[i].vbr_info.uiFrameRate;
			}
			else if(pEncodeConfig[i].eBrcType == UIAPPIPCAM_BRC_FIXQP){
				tmp_FPS = pFIXQPConfig[i].sFixQPInfo.uiFrameRate;
			}
			else if(pEncodeConfig[i].eBrcType == UIAPPIPCAM_BRC_EVBR){
				tmp_FPS = pEVBRConfig[i].evbr_info.uiFrameRate;
			}
			else{
				DBG_ERR("RTSP path=%d get BRC type error(%d)\r\n",i,pEncodeConfig[i].eBrcType);
				continue;
			}

			ImageUnit_SetParam(i+ISF_IN_BASE, NETRTSP_PORT_PARAM_FPS, tmp_FPS);


		}

	ImageUnit_End();
#endif
}

static void IPCamExe_SetAudioParam(void)
{
#if (IPCAM_AUDIO_IN_FUNC == ENABLE)

	//set audio path
	UINT32 i=0;
	IPCAM_STREAM_INFO *pStreamInfo = UIAppIPCam_get_streamInfo();
	ImageApp_IPCam_Config(IPCAM_AUDIO_IN, -1);
	for(i=0; i< UIAPPIPCAM_PATH_MAX_ID;i++){
		if(pStreamInfo[i].enable == IPCAM_STATE_ENABLE && pStreamInfo[i].avtype == IPCAM_AV_AUDIO){
			ImageApp_IPCam_Config(IPCAM_AUDIO_IN, i);
			break;
		}
	}

	UIAPPIPCAM_AUDIO_INFO *pAudioInfo = UIAppIPCam_get_AudioInfo();
	UINT32 path_index = 0;
	WAVSTUD_INFO_SET *wsis_max = UIAppIPCam_get_AudioInMaxInfo();
	NMI_AUDENC_MAX_MEM_INFO *pAudioEncMaxBuf = UIAppIPCam_get_AudioEncMaxInfo();
	AUDIN_MAX_MEM_INFO AudInMaxInfo  = {wsis_max, 0};

	ImageUnit_Begin(&ISF_AudIn, 0);
	ImageUnit_SetParam(ISF_CTRL, AUDIN_PARAM_VOL_IMM, pAudioInfo->volume);
	ImageUnit_SetParam(ISF_IN1, AUDIN_PARAM_CHANNEL, pAudioInfo->channel);
#if (IPCAM_AEC_FUNC == ENABLE)
	UIAPPIPCAM_AUDIO_INFO *pAudioOutInfo = UIAppIPCam_get_AudioOutInfo();
	WAVSTUD_INFO_SET *wsis_out_max = UIAppIPCam_get_AudioOutMaxInfo();
	UINT32 internal_size = 0;
	UINT32 fore_size = 0;
	UINT32 back_size = 0;
	UINT32 aec_need_size = 0;

	ImageUnit_SetParam(ISF_CTRL, AUDIN_PARAM_AEC_OBJ, (UINT32)&aec_obj);
	ImageUnit_SetParam(ISF_CTRL, AUDIN_PARAM_AEC_PLAY_CHANNEL, pAudioOutInfo->channel);

	//Set AEC config before get required memory
	NvtAec_SetConfig(NVTAEC_CONFIG_ID_RECORD_CH_NO, wsis_max->audInfo.ch_num);
	NvtAec_SetConfig(NVTAEC_CONFIG_ID_PLAYBACK_CH_NO, wsis_out_max->audInfo.ch_num);
	NvtAec_SetConfig(NVTAEC_CONFIG_ID_SAMPLERATE, wsis_max->audInfo.audSR);

	internal_size = NvtAec_GetRequireBufSize(NVTAEC_MEM_ID_INTENAL);
	fore_size = NvtAec_GetRequireBufSize(NVTAEC_MEM_ID_FORE);
	back_size = NvtAec_GetRequireBufSize(NVTAEC_MEM_ID_BACK);
	aec_need_size = internal_size + fore_size + back_size;

	ImageUnit_SetParam(ISF_CTRL, AUDIN_PARAM_AEC_BUFSIZE, aec_need_size);
#endif
#if (IPCAM_ANR_FUNC == ENABLE)
	UINT32 anr_need_size = 0;
	anr_need_size = nvtanr_getmem(wsis_max->audInfo.audSR, wsis_max->audInfo.ch_num);
	ImageUnit_SetParam(ISF_CTRL, AUDIN_PARAM_ANR_OBJ, (UINT32)&anr_obj);
	ImageUnit_SetParam(ISF_CTRL, AUDIN_PARAM_ANR_BUFSIZE, anr_need_size);
#endif
#if (IPCAM_AUDFILT_FUNC == ENABLE)
	ImageUnit_SetParam(ISF_CTRL, AUDIN_PARAM_AUDFILT_OBJ, (UINT32)&audfilt_obj);
#endif
	ImageUnit_SetParam(ISF_IN1, AUDIN_PARAM_MAX_MEM_INFO, (UINT32)&AudInMaxInfo);
	ImageUnit_End();

	ImageUnit_Begin(&ISF_AudEnc, 0);
	if (pAudioInfo->codec == AUDENC_ENCODER_PCM) {
		#if (AUDIO_FUNC_PCM == ENABLE)
		MP_AUDENC_ENCODER *pObj = MP_PCMEnc_getEncodeObject();
		ImageUnit_SetParam(ISF_IN1, AUDENC_PARAM_ENCODER_OBJ, (UINT32)pObj);
		#else
		ImageUnit_SetParam(ISF_IN1, AUDENC_PARAM_ENCODER_OBJ, 0);
		DBG_ERR("aud codec = %d not support\r\n", pAudioInfo->codec);
		#endif
	}
	if (pAudioInfo->codec == AUDENC_ENCODER_AAC) {
		#if (AUDIO_FUNC_AAC == ENABLE)
		MP_AUDENC_ENCODER *pObj = MP_AACEnc_getEncodeObject();
		ImageUnit_SetParam(ISF_IN1, AUDENC_PARAM_ENCODER_OBJ, (UINT32)pObj);
		ImageUnit_SetParam(ISF_IN1, AUDENC_PARAM_AAC_ADTS_HEADER, TRUE);
		#else
		ImageUnit_SetParam(ISF_IN1, AUDENC_PARAM_ENCODER_OBJ, 0);
		DBG_ERR("aud codec = %d not support\r\n", pAudioInfo->codec);
		#endif
	}
	else{
		ImageUnit_SetParam(ISF_IN1, AUDENC_PARAM_AAC_ADTS_HEADER, FALSE);
	}
	if (pAudioInfo->codec == AUDENC_ENCODER_PPCM) {
		#if (AUDIO_FUNC_PPCM == ENABLE)
		MP_AUDENC_ENCODER *pObj = MP_PPCMEnc_getEncodeObject();
		ImageUnit_SetParam(ISF_IN1, AUDENC_PARAM_ENCODER_OBJ, (UINT32)pObj);
		#else
		ImageUnit_SetParam(ISF_IN1, AUDENC_PARAM_ENCODER_OBJ, 0);
		DBG_ERR("aud codec = %d not support\r\n", pAudioInfo->codec);
		#endif
	}
	if (pAudioInfo->codec == AUDENC_ENCODER_ULAW) {
		#if (AUDIO_FUNC_ULAW == ENABLE)
		MP_AUDENC_ENCODER *pObj = MP_uLawEnc_getEncodeObject();
		ImageUnit_SetParam(ISF_IN1, AUDENC_PARAM_ENCODER_OBJ, (UINT32)pObj);
		#else
		ImageUnit_SetParam(ISF_IN1, AUDENC_PARAM_ENCODER_OBJ, 0);
		DBG_ERR("aud codec = %d not support\r\n", pAudioInfo->codec);
		#endif
	}
	if (pAudioInfo->codec == AUDENC_ENCODER_ALAW) {
		#if (AUDIO_FUNC_ALAW == ENABLE)
		MP_AUDENC_ENCODER *pObj = MP_aLawEnc_getEncodeObject();
		ImageUnit_SetParam(ISF_IN1, AUDENC_PARAM_ENCODER_OBJ, (UINT32)pObj);
		#else
		ImageUnit_SetParam(ISF_IN1, AUDENC_PARAM_ENCODER_OBJ, 0);
		DBG_ERR("aud codec = %d not support\r\n", pAudioInfo->codec);
		#endif
	}

	ImageUnit_SetParam(ISF_IN1, AUDENC_PARAM_MAX_FRAME_QUEUE, IPCAM_STEAMSENDER_A_SLOT);
	ImageUnit_SetParam(ISF_IN1, AUDENC_PARAM_MAX_MEM_INFO, (UINT32)pAudioEncMaxBuf);
	ImageUnit_SetParam(ISF_IN1, AUDENC_PARAM_RECFORMAT, MEDIAREC_LIVEVIEW);
	ImageUnit_SetParam(ISF_IN1, AUDENC_PARAM_CODEC, pAudioInfo->codec);
	//ImageUnit_SetParam(ISF_IN1, AUDENC_PARAM_SAMPLERATE, pAudioInfo->SampleRate);
	//ImageUnit_SetParam(ISF_IN1, AUDENC_PARAM_CHS, pAudioInfo->channel);

	if(pAudioInfo->channel == AUDIO_CH_STEREO || pAudioInfo->channel == AUDIO_CH_DUAL_MONO){
		ImageUnit_SetAudSample(ISF_IN1,16,2,pAudioInfo->SampleRate);
	}
	else{
		ImageUnit_SetAudSample(ISF_IN1,16,1,pAudioInfo->SampleRate);
	}

	////now all of path , audio type are the same
	for (path_index = UIAPPIPCAM_PATH_ID_0; path_index < UIAPPIPCAM_PATH_MAX_ID; path_index++) {
		if (pAudioInfo->codec == AUDENC_ENCODER_PCM) {
			ImageUnit_SetParam((path_index + ISF_OUT_BASE), AUDENC_PARAM_PORT_OUTPUT_FMT, AUDENC_OUTPUT_UNCOMPRESSION);
		} else {
			ImageUnit_SetParam((path_index + ISF_OUT_BASE), AUDENC_PARAM_PORT_OUTPUT_FMT, AUDENC_OUTPUT_COMPRESSION);
		}
	}
	ImageUnit_End();
#else
	ImageApp_IPCam_Config(IPCAM_AUDIO_IN,-1);
	DBG_WRN("Not Support Audio In\r\n");
#endif
}

static void IPCamExe_SetAudioOutParam(void)
{
#if(IPCAM_AUDIO_OUT_FUNC == ENABLE)
	ImageApp_IPCam_Config(IPCAM_AUDIO_OUT,1);
	UIAPPIPCAM_AUDIO_INFO *pAudioOutInfo = UIAppIPCam_get_AudioOutInfo();
	WAVSTUD_INFO_SET *wsis_max = UIAppIPCam_get_AudioOutMaxInfo();
	STMRCV_MAX_MEM_INFO* pStrmRcvMaxInfo  = UIAppIPCam_get_StreamReceiverMaxInfo();
	AUDOUT_MAX_MEM_INFO AudOutMaxInfo  = {wsis_max, 0};
	NMI_AUDDEC_MAX_MEM_INFO *pAudioDecMaxBuf = UIAppIPCam_get_AudioDecMaxInfo();
	ImageUnit_Begin(&ISF_AudDec, 0);
	ImageUnit_SetParam(ISF_IN1, AUDDEC_PARAM_MAX_MEM_INFO, (UINT32)pAudioDecMaxBuf);
	ImageUnit_End();


	ImageUnit_Begin(&ISF_StreamReceiver, 0);
	ImageUnit_SetParam(ISF_IN1, STMRCV_PARAM_CODEC, pAudioOutInfo->codec);
	ImageUnit_SetParam(ISF_IN1, STMRCV_PARAM_CHS, pAudioOutInfo->channel); //MONO always left channel for EVB
	ImageUnit_SetParam(ISF_IN1, STMRCV_PARAM_SAMPLERATE, pAudioOutInfo->SampleRate);
	ImageUnit_SetParam(ISF_IN1, STMRCV_PARAM_MAX_MEM_INFO, (UINT32)pStrmRcvMaxInfo);
	ImageUnit_End();

	ImageUnit_Begin(&ISF_AudOut, 0);
	ImageUnit_SetParam(ISF_CTRL, AUDOUT_PARAM_VOL_IMM, pAudioOutInfo->volume);
	ImageUnit_SetParam(ISF_OUT1, AUDOUT_PARAM_CHANNEL, pAudioOutInfo->channel);
	ImageUnit_SetParam(ISF_OUT1, AUDOUT_PARAM_MAX_MEM_INFO, (UINT32)&AudOutMaxInfo);
	ImageUnit_End();
#else

	ImageApp_IPCam_Config(IPCAM_AUDIO_OUT,0);
	DBG_WRN("Not Support Audio Out\r\n");
#endif
}

static void IPCamExe_FreeAudioMaxBuf(void){
	NMI_AUDENC_MAX_MEM_INFO MaxMemInfo = {0};
	MaxMemInfo.bRelease = TRUE;
	ImageUnit_Begin(&ISF_AudEnc, 0);
	ImageUnit_SetParam(ISF_IN1, AUDENC_PARAM_MAX_MEM_INFO, (UINT32) &MaxMemInfo);
	ImageUnit_End();

	#if(IPCAM_AUDIO_IN_FUNC == ENABLE)
	AUDIN_MAX_MEM_INFO AudIn_MaxMemInfo = {0};
	// AudIn
	AudIn_MaxMemInfo.bRelease = TRUE;
	ImageUnit_Begin(&ISF_AudIn, 0);
	ImageUnit_SetParam(ISF_IN1, AUDIN_PARAM_MAX_MEM_INFO, (UINT32) &AudIn_MaxMemInfo);
	ImageUnit_End();
	#endif

	#if(IPCAM_AUDIO_OUT_FUNC == ENABLE)
	STMRCV_MAX_MEM_INFO StrmRcv_MaxMemInfo = {0};
	AUDOUT_MAX_MEM_INFO AudOut_MaxMemInfo = {0};

	// StreamReceiver
	StrmRcv_MaxMemInfo.bRelease = TRUE;
	ImageUnit_Begin(&ISF_StreamReceiver, 0);
	ImageUnit_SetParam(ISF_IN1, STMRCV_PARAM_MAX_MEM_INFO, (UINT32) &StrmRcv_MaxMemInfo);
	ImageUnit_End();

	// AudDec
	NMI_AUDDEC_MAX_MEM_INFO AudDec_MaxMemInfo = {0};
	AudDec_MaxMemInfo.bRelease = TRUE;
	ImageUnit_Begin(&ISF_AudDec, 0);
	ImageUnit_SetParam(ISF_IN1, AUDDEC_PARAM_MAX_MEM_INFO, (UINT32) &AudDec_MaxMemInfo);
	ImageUnit_End();


	// AudOut
	AudOut_MaxMemInfo.bRelease = TRUE;
	ImageUnit_Begin(&ISF_AudOut, 0);
	ImageUnit_SetParam(ISF_OUT1, AUDOUT_PARAM_MAX_MEM_INFO, (UINT32) &AudOut_MaxMemInfo);
	ImageUnit_End();
	#endif
}

static void IPCamExe_SetVideoRotateParam(void){


	UIAPPIPCAM_ENCODE_CONFIG *pIPCamConfig = UIAppIPCam_get_EncodeConfig();
	UINT32 OutPortIndex = 0;
	UINT32 path;
	for (OutPortIndex = ISF_OUT1; OutPortIndex < (ISF_OUT_BASE + IPCAM_PATH_MAX_ID); OutPortIndex++) {
		path = OutPortIndex - ISF_OUT_BASE;

		if(pIPCamConfig[path].eRotateType == UIAPPIPCAM_RT_BUILDIN_90){
			ImageUnit_Begin(&ISF_VdoEnc, 0);
				ImageUnit_SetVdoDirection(ISF_IN_BASE + path, ISF_VDO_DIR_ROTATE_90);
			ImageUnit_End();
		}
		else if(pIPCamConfig[path].eRotateType == UIAPPIPCAM_RT_BUILDIN_270){
			ImageUnit_Begin(&ISF_VdoEnc, 0);
				ImageUnit_SetVdoDirection(ISF_IN_BASE + path, ISF_VDO_DIR_ROTATE_270);
			ImageUnit_End();
		}
		else if(pIPCamConfig[path].eRotateType == UIAPPIPCAM_RT_NONE){
			ImageUnit_Begin(&ISF_VdoEnc, 0);
				ImageUnit_SetVdoDirection(ISF_IN_BASE + path, ISF_VDO_DIR_ROTATE_0);
			ImageUnit_End();
			ImageUnit_Begin(&ISF_UserProc, 0);
				ImageUnit_SetVdoDirection(ISF_IN_BASE+OutPortIndex, ISF_VDO_DIR_NONE);
			ImageUnit_End();
		}
		else if(pIPCamConfig[path].eRotateType == UIAPPIPCAM_RT_EXTEND_90){
			ImageUnit_Begin(&ISF_UserProc, 0);
				ImageUnit_SetVdoDirection(ISF_IN_BASE+OutPortIndex, ISF_VDO_DIR_ROTATE_90);
			ImageUnit_End();
			ImageUnit_Begin(&ISF_VdoEnc, 0);
				ImageUnit_SetVdoImgSize(ISF_IN_BASE+path, pIPCamConfig[OutPortIndex].resolution.h, pIPCamConfig[OutPortIndex].resolution.w);
			ImageUnit_End();
			//ImageApp_IPCam_CfgImgSize(OutPortIndex, ALIGN_CEIL_16(pIPCamConfig[OutPortIndex].resolution.w), ALIGN_CEIL_16(pIPCamConfig[OutPortIndex].resolution.h));
		}
		else if(pIPCamConfig[path].eRotateType == UIAPPIPCAM_RT_EXTEND_180){
			ImageUnit_Begin(&ISF_UserProc, 0);
				ImageUnit_SetVdoDirection(ISF_IN_BASE+OutPortIndex, ISF_VDO_DIR_ROTATE_180);
			ImageUnit_End();
			ImageUnit_Begin(&ISF_VdoEnc, 0);
				ImageUnit_SetVdoImgSize(ISF_IN_BASE+path, pIPCamConfig[OutPortIndex].resolution.w, pIPCamConfig[OutPortIndex].resolution.h);
			ImageUnit_End();
			//ImageApp_IPCam_CfgImgSize(OutPortIndex, ALIGN_CEIL_16(pIPCamConfig[OutPortIndex].resolution.w), ALIGN_CEIL_16(pIPCamConfig[OutPortIndex].resolution.h));
		}
		else if(pIPCamConfig[path].eRotateType == UIAPPIPCAM_RT_EXTEND_270){
			ImageUnit_Begin(&ISF_UserProc, 0);
				ImageUnit_SetVdoDirection(ISF_IN_BASE+OutPortIndex, ISF_VDO_DIR_ROTATE_270);
			ImageUnit_End();
			ImageUnit_Begin(&ISF_VdoEnc, 0);
				ImageUnit_SetVdoImgSize(ISF_IN_BASE+path, pIPCamConfig[OutPortIndex].resolution.h, pIPCamConfig[OutPortIndex].resolution.w);
			ImageUnit_End();
			//ImageApp_IPCam_CfgImgSize(OutPortIndex, ALIGN_CEIL_16(pIPCamConfig[OutPortIndex].resolution.w), ALIGN_CEIL_16(pIPCamConfig[OutPortIndex].resolution.h));
		}
	}
}
#if 1
static void IPCamExe_SetVideoCropParam(void){

	UIAPPIPCAM_CROP_INFO *pCropInfo = UIAppIPCam_get_CropInfo();
	UIAPPIPCAM_SENSOR_INFO* pSensorInfo = UIAppIPCam_get_SensorInfo();
	UIAPPIPCAM_ENCODE_CONFIG *pEncodeInfo = UIAppIPCam_get_EncodeConfig();
	IPCAM_STREAM_INFO *pStreamInfo = UIAppIPCam_get_streamInfo();
	UINT32 uiSensorIndex=0;
	UINT32 uiIMEIndex=0;
	UINT32 uiPath =0;



	for(uiPath = 0; uiPath < UIAPPIPCAM_PATH_MAX_ID ; uiPath++){
		uiSensorIndex = pStreamInfo[uiPath].sensor;
		uiIMEIndex    = pStreamInfo[uiPath].ime;
		if(pStreamInfo[uiPath].enable == IPCAM_STATE_ENABLE){


			if(pSensorInfo[uiSensorIndex].bCropEnable[uiIMEIndex] == TRUE){
				if(uiIMEIndex == IPCAM_IME1 && (pSensorInfo[uiSensorIndex].bIME3DNR == TRUE || pSensorInfo[uiSensorIndex].bIME1Compress == TRUE)){
					DBG_ERR("Sensor%d IME3DNR:%d compress:%d can not crop for IME1!!\r\n",
						uiSensorIndex,
						pSensorInfo[uiSensorIndex].bIME3DNR,
						pSensorInfo[uiSensorIndex].bIME1Compress);
						continue;
				}


				///enable crop
				////get Sensor Input size
				#if 0
				IPL_IN_SIZE_INFO sIPLSensorInfo = {0};
				sIPLSensorInfo.id = uiSensorIndex;


				DBG_DUMP("charlie setp1\r\n");
				IPL_GetCmd(IPL_GET_CAL_IPL_IN_SIZE ,&sIPLSensorInfo);

				DBG_DUMP("charlie %dx%d\r\n",sIPLSensorInfo.ipl_in_width,sIPLSensorInfo.ipl_in_height);
				#endif
				UINT32 uiScaleCropW = pCropInfo[uiSensorIndex].sCropInfo[uiIMEIndex].sCropRECT.w*pCropInfo[uiSensorIndex].sCropInfo[uiIMEIndex].uiScaleWidth/pCropInfo[uiSensorIndex].sCropInfo[uiIMEIndex].sCropRECT.w;
				UINT32 uiScaleCropH = pCropInfo[uiSensorIndex].sCropInfo[uiIMEIndex].sCropRECT.h*pCropInfo[uiSensorIndex].sCropInfo[uiIMEIndex].uiScaleHeight/pCropInfo[uiSensorIndex].sCropInfo[uiIMEIndex].sCropRECT.h;
				UINT32 uiScaleCropY = pCropInfo[uiSensorIndex].sCropInfo[uiIMEIndex].sCropRECT.y*pCropInfo[uiSensorIndex].sCropInfo[uiIMEIndex].uiScaleHeight/pCropInfo[uiSensorIndex].sCropInfo[uiIMEIndex].sCropRECT.y;
				UINT32 uiScaleCropX = pCropInfo[uiSensorIndex].sCropInfo[uiIMEIndex].sCropRECT.x*pCropInfo[uiSensorIndex].sCropInfo[uiIMEIndex].uiScaleWidth/pCropInfo[uiSensorIndex].sCropInfo[uiIMEIndex].sCropRECT.x;


				////set MAIN streaming resolution for Senor Input size
				//// in this moment, IPL task not ready can not get sensor input size
				UINT32 uiScaleW		= pEncodeInfo[uiPath].resolution.w*pCropInfo[uiSensorIndex].sCropInfo[uiIMEIndex].uiScaleWidth/pCropInfo[uiSensorIndex].sCropInfo[uiIMEIndex].sCropRECT.w;
				UINT32 uiScaleH		= pEncodeInfo[uiPath].resolution.h*pCropInfo[uiSensorIndex].sCropInfo[uiIMEIndex].uiScaleHeight/pCropInfo[uiSensorIndex].sCropInfo[uiIMEIndex].sCropRECT.h;

				if(pStreamInfo[uiPath].type == IPCAM_STREAM_MAIN){

				#if 1
				DBG_DUMP("set Crop sensor%d ime%d Crop (%d %d %d %d) scale %dx%d\r\n",
					uiSensorIndex,uiIMEIndex,uiScaleCropX,uiScaleCropY,uiScaleCropW,uiScaleCropH,
					uiScaleW,uiScaleH);
				#endif
					ImageUnit_Begin(&ISF_ImgTrans, 0);
						ImageUnit_SetVdoImgSize(uiPath + ISF_IN_BASE, uiScaleW, uiScaleH);
						ImageUnit_SetVdoPostWindow(uiPath + ISF_IN_BASE,
											uiScaleCropX,
											uiScaleCropY,
											uiScaleCropW,
											uiScaleCropH);
					ImageUnit_End();
				}
				ImageUnit_Begin(&ISF_UserProc, 0);

						ImageUnit_SetVdoImgSize(uiPath + ISF_IN_BASE, pCropInfo[uiSensorIndex].sCropInfo[uiIMEIndex].uiScaleWidth, pCropInfo[uiSensorIndex].sCropInfo[uiIMEIndex].uiScaleHeight);
				ImageUnit_End();

				ImageUnit_Begin(&ISF_VdoEnc, 0);
					ImageUnit_SetVdoImgSize(uiPath + ISF_IN_BASE, pCropInfo[uiSensorIndex].sCropInfo[uiIMEIndex].uiScaleWidth, pCropInfo[uiSensorIndex].sCropInfo[uiIMEIndex].uiScaleHeight);

				ImageUnit_End();

			}

		}
		else{
			ImageUnit_Begin(&ISF_ImgTrans, 0);
				ImageUnit_SetVdoPostWindow(uiPath + ISF_IN_BASE,0,0,0,0);
			ImageUnit_End();

		}
	}

/*

	for(uiSensorIndex = 0; uiSensorIndex < UIAPPIPCAM_SENSOR_MAX; uiSensorIndex++){
		for(uiIMEIndex = 0; uiIMEIndex < 5; uiIMEIndex++){
			if(pSensorInfo[uiSensorIndex].bCropEnable[uiIMEIndex] == TRUE){


				if(uiIMEIndex == 0){
					if(pSensorInfo[uiSensorIndex].bIME3DNR == TRUE || pSensorInfo[uiSensorIndex].bIME1Compress == TRUE){
						DBG_ERR("uiSensorIndex=%d uiIMEIndex=%d 3DNR=%d Compress=%d , can not Crop\r\n",
							uiSensorIndex,uiIMEIndex,pSensorInfo[uiSensorIndex].bIME3DNR,pSensorInfo[uiSensorIndex].bIME1Compress);
							continue;
					}
				}

				/////crop enable
				DBG_DUMP("crop sensor:%d ime:%d x:%d y:%d w:%d h:%d\r\n",
					uiSensorIndex,uiIMEIndex,
					pCropInfo[uiSensorIndex].sCropInfo[uiIMEIndex].x,
					pCropInfo[uiSensorIndex].sCropInfo[uiIMEIndex].y,
					pCropInfo[uiSensorIndex].sCropInfo[uiIMEIndex].w,
					pCropInfo[uiSensorIndex].sCropInfo[uiIMEIndex].h
					);
				ImageUnit_Begin(&ISF_UserProc, 0);
					ImageUnit_SetVdoPostWindow(uiIMEIndex + ISF_OUT_BASE,
											pCropInfo[uiSensorIndex].sCropInfo[uiIMEIndex].x,
						                    pCropInfo[uiSensorIndex].sCropInfo[uiIMEIndex].y,
						                    pCropInfo[uiSensorIndex].sCropInfo[uiIMEIndex].w,
						                    pCropInfo[uiSensorIndex].sCropInfo[uiIMEIndex].h);
				ImageUnit_End();
				ImageUnit_Begin(&ISF_UserProc, 0);
					ImageUnit_SetVdoPostWindow(uiIMEIndex + ISF_OUT_BASE,
											pCropInfo[uiSensorIndex].sCropInfo[uiIMEIndex].x,
						                    pCropInfo[uiSensorIndex].sCropInfo[uiIMEIndex].y,
						                    pCropInfo[uiSensorIndex].sCropInfo[uiIMEIndex].w,
						                    pCropInfo[uiSensorIndex].sCropInfo[uiIMEIndex].h);
				ImageUnit_End();

			}
		}



	}
*/
}
#endif

static void IPCamExe_InitVideosprite(void){
	static int init = 0;

       if(init)
               return;

       videosprite_open();

#if (VIDEOSPRITEIPC != ENABLE)
       if(videosprite_set_bookkeep_buf(OS_GetMempoolAddr(POOL_ID_VDS_BK), POOL_SIZE_VDS_BK)){
               DBG_ERR("Fail to set videosprite's bookkeep buffer\r\n");
               return ;
       }
#endif

	init = 1;
}

#if (VIDEOSPRITEIPC==ENABLE)
static void IPCamExe_InitVideoSpriteIpc(void){
	static int init = 0;
	if(!init)
		VideoSpriteIpc_Open();

	init = 1;
}
#endif

#if (STAMP_FUNC==ENABLE)
static void IPCamExe_InitMovieStamp(void)
{
	STAMP_COLOR StampColorBg = {RGB_GET_Y( 16,  16,  16), RGB_GET_U( 16,  16,  16), RGB_GET_V( 16,  16,  16)}; // date stamp background color
	STAMP_COLOR StampColorFr = {RGB_GET_Y( 16,  16,  16), RGB_GET_U( 16,  16,  16), RGB_GET_V( 16,  16,  16)}; // date stamp frame color
	STAMP_COLOR StampColorFg = {RGB_GET_Y(224, 224, 192), RGB_GET_U(224, 224, 192), RGB_GET_V(224, 224, 192)}; // date stamp foreground color

	UIAPPIPCAM_ENCODE_CONFIG *pIPCamConfig = UIAppIPCam_get_EncodeConfig();
	if(!pIPCamConfig){
		DBG_ERR("fail to get UIAPPIPCAM_ENCODE_CONFIG\r\n");
		return;
	}

	UINT32      uiStampAddr;
#if (STAMP_PRIVATE_POOL == DISABLE)
	// use POOL_ID_DATEIMPRINT as movie data stamp buffer
	uiStampAddr = OS_GetMempoolAddr(POOL_ID_DATEIMPRINT);
#else
	uiStampAddr=MovieStamp_GetBufAddr(0, POOL_SIZE_DATEIMPRINT/2);
#endif
	MovieStamp_SetBuffer(0, uiStampAddr, POOL_SIZE_DATEIMPRINT/2);

	MovieStamp_SetColor(0, &StampColorBg, &StampColorFr, &StampColorFg);

	MovieStamp_Setup(
		0,
		STAMP_ON |
		STAMP_AUTO |
		STAMP_DATE_TIME |
		STAMP_BOTTOM_RIGHT |
		STAMP_POS_NORMAL |
		STAMP_BG_TRANSPARENT |
		STAMP_YY_MM_DD |
		STAMP_IMG_420UV,
		pIPCamConfig[0].resolution.w,
		pIPCamConfig[0].resolution.h,
		NULL);

	MovieStamp_Enable();
}
#endif
static UVAC_VID_RESO gUIUvacVidReso[1] = {
	{1920,  1080,   1,      30,      0,      0},        //16:9
};
static UINT32 gUIUvacAudSampleRate[1] = {
	32000
};
static UVAC_VEND_DEV_DESC gUIMovUvacDevDesc = {0};

_ALIGNED(4) const static UINT8 m_UVACManuStrDesc[] = {
	USB_VENDER_DESC_STRING_LEN * 2 + 2, // size of String Descriptor = 6 bytes
	0x03,                       // 03: String Descriptor type
	USB_VENDER_DESC_STRING
};

_ALIGNED(4) const static UINT8 m_UVACProdStrDesc[] = {
	USB_PRODUCT_DESC_STRING_LEN * 2 + 2, // size of String Descriptor = 6 bytes
	0x03,                       // 03: String Descriptor type
	USB_PRODUCT_DESC_STRING
};
_ALIGNED(4) static UINT16 m_UVACSerialStrDesc3[] = {
	0x0320,                             // 20: size of String Descriptor = 32 bytes
	// 03: String Descriptor type
	'9', '6', '6', '6', '0',            // 96611-00000-001 (default)
	'0', '0', '0', '0', '0',
	'0', '0', '1', '0', '0'
};
static void xUSBMakerInit_UVAC(UVAC_VEND_DEV_DESC *pUVACDevDesc)
{
	pUVACDevDesc->pManuStringDesc = (UVAC_STRING_DESC *)m_UVACManuStrDesc;
	pUVACDevDesc->pProdStringDesc = (UVAC_STRING_DESC *)m_UVACProdStrDesc;
	pUVACDevDesc->pSerialStringDesc = (UVAC_STRING_DESC *)m_UVACSerialStrDesc3;
	pUVACDevDesc->VID = USB_VID;
	pUVACDevDesc->PID = USB_PID_PCCAM;
}



void IPCam_SetUVACParam(void){

    // Uvac
		//set UVAC path
	INT32 iUVACPath = UIAppIPCam_get_UVACPath();
	UIAppIPCam_set_UVACPath(iUVACPath);

	if(iUVACPath >= 0 ){
    	UIAPPIPCAM_ENCODE_CONFIG *pIPCamConfig = UIAppIPCam_get_EncodeConfig();
		UINT32 uiCodec =pIPCamConfig[iUVACPath].codec;
		UVAC_VID_RESO_ARY uvacVidResoAry = {0};
		UVAC_AUD_SAMPLERATE_ARY uvacAudSampleRateAry = {0};

		uvacVidResoAry.aryCnt = 1;//MOVIE_SIZE_ID_MAX;
		uvacVidResoAry.pVidResAry = &gUIUvacVidReso[0];

		uvacAudSampleRateAry.aryCnt = 1;
		uvacAudSampleRateAry.pAudSampleRateAry = &gUIUvacAudSampleRate[0];

		xUSBMakerInit_UVAC(&gUIMovUvacDevDesc);
		UVAC_INFO    UvacInfo = {0};

		UvacInfo.channel = UVAC_CHANNEL_1V1A;
		UvacInfo.fpStartVideoCB = IPCam_StartVideoCB;
		UvacInfo.fpStopVideoCB = IPCam_StopVideoCB;
		UvacInfo.fpSetVolCB= IPCam_SetVolumeCB;
		DBG_DUMP("charlie test!!\r\n");
    	//ImageApp_Movie_Config(MOVIE_CONFIG_UVAC_INFO, (UINT32) &gMovie_Uvac_Info);

		ImageUnit_Begin(&ISF_UsbUVAC, 0);
			ImageUnit_SetParam(ISF_CTRL, UVAC_PARAM_VID_RESO_ARY, (UINT32)&uvacVidResoAry);
			ImageUnit_SetParam(ISF_CTRL, UVAC_PARAM_AUD_SAMPLERATE_ARY, (UINT32)&uvacAudSampleRateAry);
			ImageUnit_SetParam(ISF_CTRL, UVAC_PARAM_UVAC_VEND_DEV_DESC, (UINT32)&gUIMovUvacDevDesc);
			if (uiCodec == MEDIAVIDENC_MJPG) {
				ImageUnit_SetParam(ISF_CTRL, UVAC_PARAM_VID_FMT_TYPE, IMGUNIT_UVAC_VIDEO_FORMAT_MJPEG_ONLY);
			} else {
				ImageUnit_SetParam(ISF_CTRL, UVAC_PARAM_VID_FMT_TYPE, IMGUNIT_UVAC_VIDEO_FORMAT_H264_ONLY);
			}
			ImageUnit_SetParam(ISF_CTRL, UVAC_PARAM_UVAC_INFO, (UINT32)&UvacInfo);
		ImageUnit_End();


	}


}

void IPCam_SetStreamSenderParam(void){
#if 1
	IPCAM_STREAM_INFO *pStreamConfig = UIAppIPCam_get_streamInfo();
	UINT32 InPortIndex =0;
	UINT32 path=0;


	for (InPortIndex = ISF_IN1; InPortIndex < (ISF_IN_BASE + IPCAM_PATH_MAX_ID); InPortIndex++) {
		path = InPortIndex - ISF_IN_BASE;
		if(pStreamConfig[path].enable == IPCAM_STATE_ENABLE){

			DBG_DUMP("path:%d set streamsender v slot:%d a slot:%d\r\n", path,
				IPCAM_STEAMSENDER_V_SLOT,IPCAM_STEAMSENDER_A_SLOT);

			ImageUnit_Begin(&ISF_StreamSender, 0);
				ImageUnit_SetParam(path+ISF_IN_BASE, STMSND_PARAM_SLOTNUM_AUDIO_IMM, IPCAM_STEAMSENDER_A_SLOT);
				ImageUnit_SetParam(path+ISF_IN_BASE, STMSND_PARAM_SLOTNUM_VIDEO_IMM, IPCAM_STEAMSENDER_V_SLOT);

				ImageUnit_SetParam(ISF_CTRL, STMSND_PARAM_SET_AUTO_REL_ISF, IPCAM_STEAMSENDER_AUTO_REL);
				DBG_DUMP("charlie set STMSND_PARAM_SET_AUTO_REL_ISF:%d\r\n",IPCAM_STEAMSENDER_AUTO_REL);
			ImageUnit_End();
		}
	}

	//ImageApp_IPCam_Config(IPCAM_STEAMSENDER_BUF,);
#endif
    return;
}

void ImageApp_IPCam_Unit_Set(void){
#if 0
	/////set vdoOut status
	INT32 iDispPath=0;
	ISF_PORT_STATE DispStatus = 0;
	iDispPath= UIAppIPCam_get_DisplayPath();
	DispStatus =UIAppIPCam_Get_Unit_Status(UIAPPIPCAM_VDOOUT_UNIT);
	if(iDispPath != -1){
		ImageApp_IPCam_VideoOut_set_status(iDispPath,DispStatus);
	}
#endif
}


static void IPCamExe_SetsxTimer(void){


#if (IPCAM_RESET_FPS == ENABLE)

	static int first_run=0;
	if(first_run == 0){
		SX_TIMER_IPCAM_DET_FPS_ID = SxTimer_AddItem(&Timer_IPCam_FPS_DET);
		first_run++;
	}
	SxTimer_SetFuncActive(SX_TIMER_IPCAM_DET_FPS_ID, TRUE);
#endif
}

static void IPCamExe_StopsxTimer(void){
#if (IPCAM_RESET_FPS == ENABLE)
	SxTimer_SetFuncActive(SX_TIMER_IPCAM_DET_FPS_ID, FALSE);
#endif
}

extern SX_CMD_ENTRY ipcam[];

INT32 IPCamExe_OnOpen(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
	DBG_FUNC_BEGIN("\r\n");

    System_WaitFS();

	SxCmd_AddTable(ipcam);

	Ux_DefaultEvent(pCtrl, NVTEVT_EXE_OPEN, paramNum, paramArray);
	if (IPCamExe_InitCommonMem() < 0) {
		return NVTEVT_CONSUME;
	}

	ImageStream_InstallCmd();
	//SxCmd_DoCommand("isf debug ImagePipe1 *"); //enable debug (default) STATE msg
	//SxCmd_DoCommand("isf trace ImagePipe1 * /010"); //enable trace CTRL msg

	IPCamExe_InitVideosprite();

	#if (VIDEOSPRITEIPC==ENABLE)
	IPCamExe_InitVideoSpriteIpc();
	#endif

	#if (STAMP_FUNC==ENABLE)
	IPCamExe_InitMovieStamp();
	#endif

	///set stream info
	IPCamExe_SetStreamParam();

	///set disp info
	IPCamExe_SetDispParam();

	///set video encode info
	IPCamExe_SetVideoEncodeParam();

	////set IPL info
	IPCamExe_SetIPLParam();

	///set rotate info  --> maybe will change ISF_VdoEnc resolution value
	IPCamExe_SetVideoRotateParam();

	///set crop info
    IPCamExe_SetVideoCropParam();

	///set rtsp info
	IPCamExe_SetRTSPParam();

	/// set audio info
	IPCamExe_SetAudioParam();

	///set audioout info
	IPCamExe_SetAudioOutParam();

	///Init Photo func
	IPCam_InitPhotoFunc();

    ///set StreamSender info
    IPCam_SetStreamSenderParam();

	///set UVAC info
	IPCam_SetUVACParam();

	/// open Image streaming
	ImageApp_IPCam_Open();

	/// start uCtrl movie
#if UCTRL_FUNC
	UCtrlAppMovie_Open();
#endif

#if (LENS_FUNCTION == ENABLE)
	DevCtrl_LensDriverInit();
	DevCtrl_Lens(TRUE);
#endif

	//set sxtimer
	IPCamExe_SetsxTimer();

	//change ImageUnit status
	ImageApp_IPCam_Unit_Set();

	DBG_FUNC_END("\r\n");
	return NVTEVT_CONSUME;
}


static INT32 IPCamExe_ExitCommonMem(void)
{
	NVTMPP_ER        ret;

	SxCmd_DoCommand("nvtmpp status"); //show exit buffer status

	ret = nvtmpp_vb_exit();
	if (NVTMPP_ER_OK != ret) {
		DBG_ERR("nvtmpp init vb err: %d\r\n", ret);
		return -1;
	}
	return 0;
}

static void IPCamExe_FreePool(void){

	UINT32 i=0;
	INT32 ret=0;
	for(i=0; i < IPCAM_PATH_MAX_ID ; i++){
		IPCamExe_FreeVideoMaxBuf(i);
	}

	IPCamExe_FreeAudioMaxBuf();

	if (g_sAudEncBufPool != NVTMPP_VB_INVALID_POOL) {
		ret = nvtmpp_vb_destroy_pool(g_sAudEncBufPool);
		if (NVTMPP_ER_OK != ret) {
			DBG_ERR("destory pool error pool 0x%x   ret=%d\r\n", g_sAudEncBufPool, ret);
		}
	}
	g_sAudEncBufPool = NVTMPP_VB_INVALID_POOL;

}

INT32 IPCamExe_OnClose(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
	DBG_FUNC_BEGIN("\r\n");
#if IPCAM_FD_FUNC
	// unlock FD
	FD_Lock(TRUE);
	FD_UnInit();
#endif
	IPCamExe_StopsxTimer();
	UIPhotoFunc_Close();
	ImageApp_IPCam_all_path_Close();

#if (STAMP_FUNC==ENABLE)
	MovieStamp_Disable();
	MovieStamp_DestroyBuff();
#endif
	IPCamExe_FreePool();


	if (IPCamExe_ExitCommonMem() < 0) {
		return NVTEVT_CONSUME;
	}

	Ux_DefaultEvent(pCtrl, NVTEVT_EXE_CLOSE, paramNum, paramArray);


	DBG_FUNC_END("\r\n");
	return NVTEVT_CONSUME;
}

EVENT_ENTRY CustomIPCamObjCmdMap[] = {
	{NVTEVT_EXE_OPEN,                 IPCamExe_OnOpen                     },
	{NVTEVT_EXE_CLOSE,                IPCamExe_OnClose                    },
	{NVTEVT_NULL,                     0},  //End of Command Map
};

CREATE_APP(CustomIPCamObj, APP_SETUP)

