#include "SysCommon.h"
#include "UIFramework.h"
#include "PrjCfg.h"
#include "uToken.h"
#include "UIInfo.h"
#include "UIAppMovie.h"
#if(PHOTO_MODE==ENABLE)
#include "UIAppPhoto.h"
#endif
#include "UIControl.h"
#include "UIControlWnd.h"
#include "UISetup.h"
#include "UCtrlAppMovie.h"
#include "NvtUctrlAPI.h"
#include "NvtIpcAPI.h"
#include "UCtrlMain.h"
#if(IPCAM_FUNC !=ENABLE)
#include "UIMovieMapping.h"
#endif
#include "UIPhotoFuncInfo.h"
#include "ImageApp_IPCam.h"
#include "ImageStream.h"
#include "iq_api.h"
#include "FileSysTsk.h"
#include "md_api.h"
#if(STAMP_FUNC==ENABLE)
#include "MovieStamp.h"
#endif
#include "Audio.h"
#include "WavStudioTsk.h"
#if(IPCAM_MODE == ENABLE)
#include "UIAppIPCam_Param.h"
#else
#include "UIAppMovie_Param.h"
#endif
#include "UIAppIPCam.h"
#if MOVIE_TD_FUNC
#include "TD_lib.h"
#endif

#if (IPCAM_DIS_MV_FUNC == ENABLE)
#include "dis_alg.h"
#endif

#if (IPCAM_BC_FUNC == ENABLE)
#include "bc_lib.h"

#endif

#if MOVIE_FTG_FUNC
#include "ftg_lib.h"
#include "ftg_fd_lib.h"
#endif
#if IPCAM_FD_FUNC
#include "fd_lib.h"
#endif
#include "JpgEnc.h"
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

#if IPCAM_AEC_FUNC
#include "nvtaec_api.h"
#endif
#if IPCAM_ANR_FUNC
#include "nvtanr_api.h"
#endif

#define THIS_DBGLVL         6 // 0=FATAL, 1=ERR, 2=WRN, 3=UNIT, 4=FUNC, 5=IND, 6=MSG, 7=VALUE, 8=USER
///////////////////////////////////////////////////////////////////////////////
#define __MODULE__          UCtrlAppMovie
#define __DBGLVL__          ((THIS_DBGLVL>=PRJ_DBG_LVL)?THIS_DBGLVL:PRJ_DBG_LVL)
#define __DBGFLT__          "*" //*=All, [mark]=CustomClass
#include "DebugModule.h"

#if defined(_BSP_NA51000_)
#define MAXAUDIO_CH			8
#else
#define MAXAUDIO_CH			16
#endif
#define SXCMD_TEST          0

#define ON                  1
#define OFF                 0

#define ISF_TEST			1


#if (FTG_SNAPSHOT_FUN==ENABLE)
ftg_face g_ftg_list[FTG_MAX_SNAPSHOT_NUM];
UINT32 g_face_number = 0;
BOOL g_bFTGOpen = FALSE;
#endif

typedef struct _UCtrlMovieLTRType {
	UINT32 isSet;
	UINT32 uiLTRInterval;    ///< long-term reference frame interval. range : 0 ~ 4095
	UINT32 uiLTRPreRef;    	 ///< long-term reference setting. 0 : all long-term reference to IDR frame, 1: reference latest long-term reference frame.
} UCtrlMovieLTRType;

typedef struct _UCtrlMovieNRType {
	UINT32 isSet;
	BOOL bEnable;
} UCtrlMovieNRType;

typedef struct _UCtrlMovieParmType {
	UINT32 isSet;
	UINT32 index;
} UCtrlMovieParmType;

typedef struct _UCtrlMovieSizeType {
	UINT32 isSet;
	ISIZE size;
} UCtrlMovieSizeType;

typedef struct _UCtrlMovieSEIType{
	UINT32 isSet;
	BOOL bEnable;
}UCtrlMovieSEIType;

typedef struct {
	UINT32 isSet;
	UINT32 uiGOP;
	UINT32 uiFrameRate;
	UINT32 uiTargetByterate;
} UCtrlMovieBRCNoneParmType;



typedef struct _UCtrlMovieInfoType {
	BOOL                isErr;

	UCtrlMovieParmType  		recsize;
	UCtrlMovieParmType  		liveview_channel_id;
	UCtrlMovieParmType  		liveview_vid_enc_fmt;
	UCtrlMovieSizeType  		liveview_vid_size;
	//UCtrlMovieParmType  liveview_vid_framerate;
	//UCtrlMovieParmType  liveview_vid_bitrate;
	//UCtrlMovieBitrateParmType  liveview_vid_advanced_bitrate;
	UCtrlMovieCBRParmType 		liveview_vid_cbr;
	UCtrlMovieVBRParmType 		liveview_vid_vbr;
	UCtrlMovieEVBRParmType 		liveview_vid_evbr;
	UCtrlMovieBRCNoneParmType   liveview_vid_brc_none;
	UCtrlMovieFixQPParmType     liveview_vid_fixqp;
	UCtrlMovieAQParamType       liveview_vid_aq;
	//UCtrlMovieParmType  liveview_vid_gop;
	UCtrlMovieParmType  		liveview_vid_svc_t;
	UCtrlMovieCropPosParmType  	liveview_crop;
	UCtrlMovieParmType  liveview_aud_enc_fmt;
	UCtrlMovieParmType  liveview_aud_channel;
	UCtrlMovieParmType  liveview_aud_samplerate;
	UCtrlMovieParmType  cycrec;
	UCtrlMovieParmType  wdr;
	UCtrlMovieParmType  hdr;
	UCtrlMovieParmType  ev;
	UCtrlMovieParmType  motion;
	UCtrlMovieParmType  audio;
	UCtrlMovieParmType  dstamp;
	UCtrlMovieParmType  rsc;
	UCtrlMovieParmType  gsensor;
	UCtrlMovieParmType  timelap;
	UCtrlMovieParmType  ircut;
	UCtrlMovieParmType  rotate;
	UCtrlMovieParmType  murgpro;
	UCtrlMovieParmType  aurgpro;
	UCtrlMovieParmType  pim;
	UCtrlMovieParmType  dualcam;
	UCtrlMovieParmType  start;
	UCtrlMovieParmType  stop;
	UCtrlMovieParmType  fdstart;
	UCtrlMovieParmType  fdcfgset;
	UCtrlMovieParmType  fdcfgget;
	UCtrlMovieParmType  fddataget;
	UCtrlMovieParmType  aout_channel;
	UCtrlMovieNRType    NRSetting;
	UCtrlMovieLTRType   LTRset;
	UCtrlMovieParmType  profile;
	UCtrlMovieSEIType	liveview_vid_sei;
} UCtrlMovieInfoType;

/*
static USIZE IMAGERATIO_SIZE[IMAGERATIO_MAX_CNT]=
{
    {9,16},//IMAGERATIO_9_16
    {2,3}, //IMAGERATIO_2_3
    {3,4}, //IMAGERATIO_3_4
    {1,1}, //IMAGERATIO_1_1
    {4,3}, //IMAGERATIO_4_3
    {3,2}, //IMAGERATIO_3_2
    {16,9},//IMAGERATIO_16_9
};
*/



static UCtrlMovieInfoType   movieInfo;
static UCtrlMovieInfoType  *pMovieInfo     = &movieInfo;

static BOOL                 bUctrlRecStart = TRUE;





static INT32 param_recsize(INT32 argc, char *argv[])
{
	UINT32 value;
	if (argc != 1) {
		DBG_ERR("argc!=1 (%d)\r\n", argc);
		return -1;
	}
	DBG_IND("get: %d\r\n", atoi(argv[0]));
	value = atoi(argv[0]);
	if (value >= MOVIE_SIZE_ID_MAX) {
		DBG_ERR("value =%d\r\n", value);
		return -1;
	}

	pMovieInfo->recsize.index = value;
	pMovieInfo->recsize.isSet = 1;

	return 0;
}


/* need move to image uctrl
//#NT#2016/04/13#Charlie Chang -begin
//#NT# add liveview ratio set function
static INT32 param_liveviewratio_set(INT32 argc, char* argv[])
{
    UINT32 value = 0;
    if(argc!=1){
        DBG_ERR("argc!=1 (%d)\r\n",argc);
        return -1;
    }
    value = atoi(argv[0]);
    if(value >= IMAGERATIO_MAX_CNT){
        DBG_ERR("ratio value error, value=%d\r\n",value);
        return -1;
    }

    if(ImageApp_IPCam_GetConfig(IPCAM_CFG_RATIO) != value){

        pMovieInfo->vratio.isSet = 1;
        pMovieInfo->vratio.index = value;
    }
    return 0;
}
//#NT#2016/04/13#Charlie Chang -end
*/
static INT32 param_channelid_set(INT32 argc, char *argv[])
{
	UINT32 value;
	if (argc != 1) {
		DBG_ERR("argc!=1 (%d)\r\n", argc);
		return -1;
	}
	value = atoi(argv[0]);
	if (value >= MOVIE_MAX_CHANNEL) {
		DBG_ERR("value =%d\r\n", value);
		return -1;
	}

	pMovieInfo->liveview_channel_id.index = value;
	pMovieInfo->liveview_channel_id.isSet = 1;

	return 0;
}

static INT32 param_liveviewencfmt_set(INT32 argc, char *argv[])
{
	UINT32 value;
	if (argc != 1) {
		DBG_ERR("argc!=1 (%d)\r\n", argc);
		return -1;
	}
	//DBG_IND("got lv size: %d\r\n",atoi(argv[0]));
	value = atoi(argv[0]);
	if ((value != MEDIAVIDENC_MJPG) &&
		(value != MEDIAVIDENC_H264) &&
		(value != MEDIAVIDENC_H265)) {
		DBG_ERR("value =%d\r\n", value);
		return -1;
	}
	pMovieInfo->liveview_vid_enc_fmt.index = value;
	pMovieInfo->liveview_vid_enc_fmt.isSet = 1;
	return 0;
}

static INT32 param_liveviewencfmt_get(INT32 argc, char *argv[])
{
#if(MOVIE_MODE==ENABLE)
	UINT32 uilvencfmt;
	char   retStr[32];

	uilvencfmt = GetMovieVidEncFmt_2p(1);

	snprintf(retStr, sizeof(retStr), "%d", uilvencfmt);
	NvtUctrl_SetRetString(retStr);
#else
	DBG_ERR("not support !!\r\n");
#endif
	return 0;
}


static INT32 param_liveviewsize_set(INT32 argc, char *argv[])
{
	UINT32 width;
	UINT32 height;
	if (argc != 2) {
		DBG_ERR("argc!=2 (%d)\r\n", argc);
		return -1;
	}
	DBG_IND("got lv size: %d %d\r\n", atoi(argv[0]), atoi(argv[1]));

	width = atoi(argv[0]);
	height = atoi(argv[1]);

	pMovieInfo->liveview_vid_size.size.w = width;
	pMovieInfo->liveview_vid_size.size.h = height;
	pMovieInfo->liveview_vid_size.isSet = 1;

	return 0;
}

static INT32 param_liveviewsize_get(INT32 argc, char *argv[])
{
#if(MOVIE_MODE==ENABLE)
	UINT32 uilvsize_w, uilvsize_h;
	char   retStr[32];

	uilvsize_w = GetMovieSizeWidth_2p(1, 0);  // image width
	uilvsize_h = GetMovieSizeHeight_2p(1, 0); // image height

	snprintf(retStr, sizeof(retStr), "%d %d", uilvsize_w, uilvsize_h);
	NvtUctrl_SetRetString(retStr);

#else
	DBG_ERR("not support!!\r\n");
#endif
	return 0;
}

static INT32 param_liveviewframerate_set(INT32 argc, char *argv[])
{
	#if 0
	UINT32 value;

	if (argc != 1) {
		DBG_ERR("argc!=1 (%d)\r\n", argc);
		return -1;
	}
	//DBG_IND("got FR: %d\r\n",atoi(argv[0]));
	value = atoi(argv[0]);
	if (value > 30) { // Max frame rate is 30FPS.
		DBG_ERR("value =%d\r\n", value);
		return -1;
	}
#if (MEDIA_VASTREAM==DISABLE)
	// 10/15/20/24/25/30/48/50/60/120/240fps
	if (!((value == 10) || (value == 15) || (value == 20) || (value == 24) || (value == 25) || (value == 30))) {
		DBG_ERR("value =%d\r\n", value);
		return -1;
	}
#endif

	pMovieInfo->liveview_vid_framerate.index = value;
	pMovieInfo->liveview_vid_framerate.isSet = 1;
	#else
		DBG_ERR("not support!!\r\n");
	#endif
	return 0;
}

static INT32 param_liveviewframerate_get(INT32 argc, char *argv[])
{
#if(MOVIE_MODE == ENABLE)
	UINT32 uilvframerate;
	char   retStr[32];

	uilvframerate = GetMovieFrameRate_2p(1, 0);

	snprintf(retStr, sizeof(retStr), "%d", uilvframerate);
	NvtUctrl_SetRetString(retStr);
#else
	DBG_ERR("not support !!\r\n");
#endif
	return 0;
}

static INT32 param_liveviewbitrate_set(INT32 argc, char *argv[])
{
	#if 0
	UINT32 value;

	if (argc != 1) {
		DBG_ERR("argc!=1 (%d)\r\n", argc);
		return -1;
	}
	//DBG_IND("got BR: %d\r\n",atoi(argv[0]));
	value = atoi(argv[0]);
	//if (value > (1500 * 1024)) // Max bit rate is 1500 KBytes.
	//{
	//    DBG_ERR("value =%d\r\n",value);
	//    return -1;
	//}
	pMovieInfo->liveview_vid_bitrate.index = value;
	pMovieInfo->liveview_vid_bitrate.isSet = 1;
	#else
	DBG_ERR("not support\r\n");
	#endif

	return 0;
}

static INT32 param_liveviewbitrate_get(INT32 argc, char *argv[])
{


#if(MOVIE_MODE == ENABLE)
	char   retStr[32];
	UINT32 uilvbitrate;
	uilvbitrate = GetMovieTargetBitrate_2p(1, 0);
	snprintf(retStr, sizeof(retStr), "%d", uilvbitrate);
	NvtUctrl_SetRetString(retStr);
#else
	DBG_ERR("not support!!\r\n");
#endif


	return 0;
}

static INT32 param_liveviewgop_set(INT32 argc, char *argv[])
{
	#if 0
	UINT32 value;

	if (argc != 1) {
		DBG_ERR("argc!=1 (%d)\r\n", argc);
		return -1;
	}
	value = atoi(argv[0]);

	pMovieInfo->liveview_vid_gop.index = value;
	pMovieInfo->liveview_vid_gop.isSet = 1;
	#else
	DBG_ERR("not support!!\r\n");
	#endif
	return 0;
}

#if 0
static INT32 param_liveviewcrop_set(INT32 argc, char *argv[])
{
	if (argc != 3) {
		DBG_ERR("argc!=3 (%d)\r\n", argc);
		return -1;
	}
	pMovieInfo->liveview_crop.bCropEnable = atoi(argv[0]);
	pMovieInfo->liveview_crop.uiCoordinateX = atoi(argv[1]);
	pMovieInfo->liveview_crop.uiCoordinateY = atoi(argv[2]);
	pMovieInfo->liveview_crop.isSet = 1;

	DBG_IND("cropen=%d ,x = %d, y=%d\r\n", pMovieInfo->liveview_crop.bCropEnable, pMovieInfo->liveview_crop.uiCoordinateX, pMovieInfo->liveview_crop.uiCoordinateY);
	return 0;
}
#endif


static INT32 param_lvaudsamprate_set(INT32 argc, char *argv[])
{
#if(AUDIO_CMD_UCTRL_ENABLE == ENABLE)
	UINT32 value;

	if (argc != 1) {
		DBG_ERR("argc!=1 (%d)\r\n", argc);
		return -1;
	}
	//DBG_IND("got BR: %d\r\n",atoi(argv[0]));
	value = atoi(argv[0]);
	if (value > 32) { // Max sample rate is 32Khz.
		DBG_ERR("value =%d\r\n", value);
		return -1;
	}

	pMovieInfo->liveview_aud_samplerate.index = value;
	pMovieInfo->liveview_aud_samplerate.isSet = 1;
#endif
	return 0;
}

static INT32 param_lvaudsamprate_get(INT32 argc, char *argv[])
{
#if(AUDIO_CMD_UCTRL_ENABLE == ENABLE)

	char   retStr[32];
#if(MOVIE_MODE== ENABLE)
	snprintf(retStr, sizeof(retStr), "%d", Movie_GetAudSampleRate());
#elif(IPCAM_MODE==ENABLE)
	UIAPPIPCAM_AUDIO_INFO *audioInfo = UIAppIPCam_get_AudioInfo();
	snprintf(retStr, sizeof(retStr), "%d", audioInfo->SampleRate);

#else
	DBG_ERR("not support!!\r\n");
	snprintf(retStr, sizeof(retStr), "%d", 0);
#endif
	NvtUctrl_SetRetString(retStr);
#endif
	return 0;
}

static INT32 param_lvaudenctype_set(INT32 argc, char *argv[])
{
#if(AUDIO_CMD_UCTRL_ENABLE == ENABLE)
	UINT32 value;

	if (argc != 1) {
		DBG_ERR("argc!=1 (%d)\r\n", argc);
		return -1;
	}
	//DBG_IND("got BR: %d\r\n",atoi(argv[0]));
	value = atoi(argv[0]);
#if 0
	if (value != 2 && value != 3) { // Only AAC(2) and PPcm(3) can be chosen.
		DBG_ERR("value =%d\r\n", value);
		return -1;
	}
#endif

	pMovieInfo->liveview_aud_enc_fmt.index = value;
	pMovieInfo->liveview_aud_enc_fmt.isSet = 1;
#endif
	return 0;

}

static INT32 param_lvaudenctype_get(INT32 argc, char *argv[])
{
#if(AUDIO_CMD_UCTRL_ENABLE == ENABLE)

	char   retStr[32];
#if(MOVIE_MODE == ENABLE)
	UINT32 uilvaudenctype;
	uilvaudenctype = Movie_GetAudCodec();

	if (uilvaudenctype == MOVAUDENC_AAC) {
		uilvaudenctype = 2;
	} else {
		uilvaudenctype = 0;
	}
	snprintf(retStr, sizeof(retStr), "%d", uilvaudenctype);
#elif(IPCAM_MODE == ENABLE)
	UIAPPIPCAM_AUDIO_INFO *audioInfo = UIAppIPCam_get_AudioInfo();
	snprintf(retStr, sizeof(retStr), "%d", audioInfo->codec);
#else
	DBG_ERR("not support !!\r\n");
	snprintf(retStr, sizeof(retStr), "%d", 0);
#endif

	NvtUctrl_SetRetString(retStr);
#endif
	return 0;

}

static INT32 param_venc_get(INT32 argc, char *argv[])
{
#if(IPCAM_MODE == ENABLE)
	UINT8 venc_channel = 0;
	CodecVencSetting sVencSetting = {0};
	// MOVIE_ENCODE_CROP *pEncodeCrop=NULL;
	// UINT32 uiBrcInfoAddr;

	if (argc != 1) {
		DBG_ERR("argc!=1 (%d)\r\n", argc);
		return -1;
	}

	if ((venc_channel = atoi(argv[0])) >= MOVIE_MAX_CHANNEL) {
		DBG_ERR("invalid venc channel: %u (Max: %d)\r\n", venc_channel, 1);
		return -1;
	}

	UIAPPIPCAM_ENCODE_CONFIG *pStreamConfig = NULL;
	pStreamConfig = UIAppIPCam_get_EncodeConfig();


	if (pStreamConfig == NULL) {
		DBG_ERR("get ImageApp_IPCam_get_StreamConfig fail!!\r\n");
		return -1;
	}

	/////get codec
	switch (pStreamConfig[venc_channel].codec) {
	case IPCAMCODEC_MJPG:
		sVencSetting.eVencFormat = CODEC_VENC_FORMAT_MJPEG;
		break;
	case IPCAMCODEC_H264:
		sVencSetting.eVencFormat = CODEC_VENC_FORMAT_H264;
		break;
	case IPCAMCODEC_H265:
		sVencSetting.eVencFormat = CODEC_VENC_FORMAT_H265;
		break;
	case IPCAMCODEC_YUV:
		sVencSetting.eVencFormat = CODEC_VENC_FORMAT_YUV;
		break;
	default:
		DBG_ERR("get codec info error (%d)\r\n", pStreamConfig[venc_channel].codec);
		break;
	}

	//get SVC layer
#if 1

	switch (pStreamConfig[venc_channel].eSVCType) {
	case MP_VDOENC_SVC_LAYER1:
		sVencSetting.sSVCSetting.eSVCTemporalLayer = CODEC_VENC_SVCT_LAYER_1;
		break;
	case MP_VDOENC_SVC_LAYER2:
		sVencSetting.sSVCSetting.eSVCTemporalLayer = CODEC_VENC_SVCT_LAYER_2;
		break;
	case MP_VDOENC_SVC_DISABLE:
	default:
		sVencSetting.sSVCSetting.eSVCTemporalLayer = CODEC_VENC_SVCT_DISABLE;
		break;
	}
#endif
	//get bitrate control
	//sVencSetting.sVencBitRateCtrl.uiTargetBitrate = pStreamConfig[venc_channel].bitrate * 8; //byte to bit

	UIAPPIPCAM_BRC_TYPE  eBRCType = pStreamConfig[venc_channel].eBrcType;
	UIAPPIPCAM_CBR_CONFIG *pCBRConfig = UIAppIPCam_get_CBRInfo();
	UIAPPIPCAM_VBR_CONFIG *pVBRConfig = UIAppIPCam_get_VBRInfo();
	UIAPPIPCAM_EVBR_CONFIG *pEVBRConfig = UIAppIPCam_get_EVBRInfo();
	UIAPPIPCAM_FIXQP_CONFIG *pFixQPConfig = UIAppIPCam_get_FixQPInfo();
	UIAPPIPCAM_BRC_NONE_CONFIG *pBRCNoneConfig = UIAppIPCam_get_BRCNoneInfo();
	UIAPPIPCAM_AQ_CONFIG *pAQConfig = UIAppIPCam_get_AQInfo();

	////get AQ info
	sVencSetting.sAQSetting.iAqEnable = pAQConfig[venc_channel].sAQInfo.iAqEnable;
	sVencSetting.sAQSetting.uiAqStr= pAQConfig[venc_channel].sAQInfo.uiAqStr;
	sVencSetting.sAQSetting.iMaxSraq = pAQConfig[venc_channel].sAQInfo.iMaxSraq;
	sVencSetting.sAQSetting.iMinSraq = pAQConfig[venc_channel].sAQInfo.iMinSraq;
	sVencSetting.sAQSetting.iSraqInitAslog2 = pAQConfig[venc_channel].sAQInfo.iSraqInitAslog2;
	sVencSetting.sAQSetting.iSraqConstAslog2 = pAQConfig[venc_channel].sAQInfo.iSraqConstAslog2;

	sVencSetting.uiBRCType = eBRCType;

	sVencSetting.sVencCBRConfig.uiEnable = pCBRConfig[venc_channel].cbr_info.uiEnable;
	sVencSetting.sVencCBRConfig.uiStaticTime = pCBRConfig[venc_channel].cbr_info.uiStaticTime;
	sVencSetting.sVencCBRConfig.uiByteRate = pCBRConfig[venc_channel].cbr_info.uiByteRate;
	sVencSetting.sVencCBRConfig.uiFrameRate = pCBRConfig[venc_channel].cbr_info.uiFrameRate;
	sVencSetting.sVencCBRConfig.uiGOP = pCBRConfig[venc_channel].cbr_info.uiGOP;
	sVencSetting.sVencCBRConfig.uiInitIQp = pCBRConfig[venc_channel].cbr_info.uiInitIQp;
	sVencSetting.sVencCBRConfig.uiMinIQp = pCBRConfig[venc_channel].cbr_info.uiMinIQp;
	sVencSetting.sVencCBRConfig.uiMaxIQp = pCBRConfig[venc_channel].cbr_info.uiMaxIQp;
	sVencSetting.sVencCBRConfig.uiInitPQp = pCBRConfig[venc_channel].cbr_info.uiInitPQp;
	sVencSetting.sVencCBRConfig.uiMinPQp = pCBRConfig[venc_channel].cbr_info.uiMinPQp;
	sVencSetting.sVencCBRConfig.uiMaxPQp = pCBRConfig[venc_channel].cbr_info.uiMaxPQp;
	sVencSetting.sVencCBRConfig.iIPWeight = pCBRConfig[venc_channel].cbr_info.iIPWeight;
	sVencSetting.sVencCBRConfig.uiRowRcEnalbe = pCBRConfig[venc_channel].cbr_info.uiRowRcEnalbe;
	sVencSetting.sVencCBRConfig.uiRowRcQpRange = pCBRConfig[venc_channel].cbr_info.uiRowRcQpRange;
	sVencSetting.sVencCBRConfig.uiRowRcQpStep = pCBRConfig[venc_channel].cbr_info.uiRowRcQpStep;

	sVencSetting.sVencVBRConfig.uiEnable = pVBRConfig[venc_channel].vbr_info.uiEnable;
	sVencSetting.sVencVBRConfig.uiStaticTime = pVBRConfig[venc_channel].vbr_info.uiStaticTime;
	sVencSetting.sVencVBRConfig.uiByteRate = pVBRConfig[venc_channel].vbr_info.uiByteRate;
	sVencSetting.sVencVBRConfig.uiFrameRate = pVBRConfig[venc_channel].vbr_info.uiFrameRate;
	sVencSetting.sVencVBRConfig.uiGOP = pVBRConfig[venc_channel].vbr_info.uiGOP;
	sVencSetting.sVencVBRConfig.uiInitIQp = pVBRConfig[venc_channel].vbr_info.uiInitIQp;
	sVencSetting.sVencVBRConfig.uiMinIQp = pVBRConfig[venc_channel].vbr_info.uiMinIQp;
	sVencSetting.sVencVBRConfig.uiMaxIQp = pVBRConfig[venc_channel].vbr_info.uiMaxIQp;
	sVencSetting.sVencVBRConfig.uiInitPQp = pVBRConfig[venc_channel].vbr_info.uiInitPQp;
	sVencSetting.sVencVBRConfig.uiMinPQp = pVBRConfig[venc_channel].vbr_info.uiMinPQp;
	sVencSetting.sVencVBRConfig.uiMaxPQp = pVBRConfig[venc_channel].vbr_info.uiMaxPQp;
	sVencSetting.sVencVBRConfig.iIPWeight = pVBRConfig[venc_channel].vbr_info.iIPWeight;
	sVencSetting.sVencVBRConfig.uiChangePos = pVBRConfig[venc_channel].vbr_info.uiChangePos;
	sVencSetting.sVencVBRConfig.uiRowRcEnalbe = pVBRConfig[venc_channel].vbr_info.uiRowRcEnalbe;
	sVencSetting.sVencVBRConfig.uiRowRcQpRange = pVBRConfig[venc_channel].vbr_info.uiRowRcQpRange;
	sVencSetting.sVencVBRConfig.uiRowRcQpStep = pVBRConfig[venc_channel].vbr_info.uiRowRcQpStep;

	sVencSetting.sVencEVBRConfig.uiEnable = pEVBRConfig[venc_channel].evbr_info.uiEnable;
	sVencSetting.sVencEVBRConfig.uiStaticTime = pEVBRConfig[venc_channel].evbr_info.uiStaticTime;
	sVencSetting.sVencEVBRConfig.uiByteRate = pEVBRConfig[venc_channel].evbr_info.uiByteRate;
	sVencSetting.sVencEVBRConfig.uiFrameRate = pEVBRConfig[venc_channel].evbr_info.uiFrameRate;
	sVencSetting.sVencEVBRConfig.uiGOP = pEVBRConfig[venc_channel].evbr_info.uiGOP;
	sVencSetting.sVencEVBRConfig.uiKeyPPeriod = pEVBRConfig[venc_channel].evbr_info.uiKeyPPeriod;
	sVencSetting.sVencEVBRConfig.uiInitIQp = pEVBRConfig[venc_channel].evbr_info.uiInitIQp;
	sVencSetting.sVencEVBRConfig.uiMinIQp = pEVBRConfig[venc_channel].evbr_info.uiMinIQp;
	sVencSetting.sVencEVBRConfig.uiMaxIQp = pEVBRConfig[venc_channel].evbr_info.uiMaxIQp;
	sVencSetting.sVencEVBRConfig.uiInitPQp = pEVBRConfig[venc_channel].evbr_info.uiInitPQp;
	sVencSetting.sVencEVBRConfig.uiMinPQp = pEVBRConfig[venc_channel].evbr_info.uiMinPQp;
	sVencSetting.sVencEVBRConfig.uiMaxPQp = pEVBRConfig[venc_channel].evbr_info.uiMaxPQp;
	sVencSetting.sVencEVBRConfig.iIPWeight = pEVBRConfig[venc_channel].evbr_info.iIPWeight;
	sVencSetting.sVencEVBRConfig.iKeyPWeight = pEVBRConfig[venc_channel].evbr_info.iKeyPWeight;
	sVencSetting.sVencEVBRConfig.iMotionAQStrength = pEVBRConfig[venc_channel].evbr_info.iMotionAQStrength;
	sVencSetting.sVencEVBRConfig.uiRowRcEnalbe = pEVBRConfig[venc_channel].evbr_info.uiRowRcEnalbe;
	sVencSetting.sVencEVBRConfig.uiRowRcQpRange = pEVBRConfig[venc_channel].evbr_info.uiRowRcQpRange;
	sVencSetting.sVencEVBRConfig.uiRowRcQpStep = pEVBRConfig[venc_channel].evbr_info.uiRowRcQpStep;
	sVencSetting.sVencEVBRConfig.uiStillFrameCnd = pEVBRConfig[venc_channel].evbr_info.uiStillFrameCnd;
	sVencSetting.sVencEVBRConfig.uiMotionRatioThd = pEVBRConfig[venc_channel].evbr_info.uiMotionRatioThd;
	sVencSetting.sVencEVBRConfig.uiIPsnrCnd = pEVBRConfig[venc_channel].evbr_info.uiIPsnrCnd;
	sVencSetting.sVencEVBRConfig.uiPPsnrCnd = pEVBRConfig[venc_channel].evbr_info.uiPPsnrCnd;
	sVencSetting.sVencEVBRConfig.uiKeyPPsnrCnd = pEVBRConfig[venc_channel].evbr_info.uiKeyPPsnrCnd;

	sVencSetting.sVencFixQPConfig.uiByteRate  = pFixQPConfig[venc_channel].sFixQPInfo.uiByteRate;
	sVencSetting.sVencFixQPConfig.uiEnable = pFixQPConfig[venc_channel].sFixQPInfo.uiEnable;
	sVencSetting.sVencFixQPConfig.uiFrameRate = pFixQPConfig[venc_channel].sFixQPInfo.uiFrameRate;
	sVencSetting.sVencFixQPConfig.uiGOP = pFixQPConfig[venc_channel].sFixQPInfo.uiGOP;
	sVencSetting.sVencFixQPConfig.uiIFixQP = pFixQPConfig[venc_channel].sFixQPInfo.uiIFixQP;
	sVencSetting.sVencFixQPConfig.uiPFixQP = pFixQPConfig[venc_channel].sFixQPInfo.uiPFixQP;

	sVencSetting.sVencBRCNoneInfo.uiFrameRate = pBRCNoneConfig[venc_channel].uiFrameRate;
	sVencSetting.sVencBRCNoneInfo.uiGOP = pBRCNoneConfig[venc_channel].uiGOP;
	sVencSetting.sVencBRCNoneInfo.uiTargetByterate = pBRCNoneConfig[venc_channel].uiTargetByterate;


	///get H264 profile
#if 1
	sVencSetting.eVencProfile = pStreamConfig[venc_channel].eProfile;
#endif


	//get 3DNR info
	#if(IPCAM_FUNC == ENABLE)
		sVencSetting.sNRSetting.bEnable =  pStreamConfig[venc_channel].sNRSetting.bEnable;
	#endif

    //get rotate type
    sVencSetting.eRotateType = pStreamConfig[venc_channel].eRotateType;

	sVencSetting.uiTargetWidth = pStreamConfig[venc_channel].resolution.w;
	sVencSetting.uiTargetHeight = pStreamConfig[venc_channel].resolution.h;

	//get LTR info
	sVencSetting.sLTRInfo.uiLTRInterval = pStreamConfig[venc_channel].sLTRInfo.uiLTRInterval;
	sVencSetting.sLTRInfo.uiLTRPreRef   = pStreamConfig[venc_channel].sLTRInfo.uiLTRPreRef;

	//get SEI info
	sVencSetting.sSEISetting.bEnable = pStreamConfig[venc_channel].sSEISetting.bEnable;
	DBG_IND("SEI setting =%d\r\n",pStreamConfig[venc_channel].sSEISetting.bEnable);
	DBG_DUMP("=======ch=%d\r\n", venc_channel);
	DBG_DUMP("  codec=  %d\r\n", sVencSetting.eVencFormat);
	DBG_DUMP("  width=  %d\r\n", sVencSetting.uiTargetWidth);
	DBG_DUMP("  height= %d\r\n", sVencSetting.uiTargetHeight);

	if(pStreamConfig[venc_channel].eBrcType == UIAPPIPCAM_BRC_CBR){
		DBG_DUMP("  CBR byterate=%d\r\n", pCBRConfig[venc_channel].cbr_info.uiByteRate);
		DBG_DUMP("  fps=    %d\r\n", pCBRConfig[venc_channel].cbr_info.uiFrameRate);
		DBG_DUMP("  GOP=    %d\r\n", pCBRConfig[venc_channel].cbr_info.uiGOP);

	} else if(pStreamConfig[venc_channel].eBrcType == UIAPPIPCAM_BRC_VBR){

		DBG_DUMP("  VBR byterate=%d\r\n", pVBRConfig[venc_channel].vbr_info.uiByteRate);
		DBG_DUMP("  fps=    %d\r\n", pVBRConfig[venc_channel].vbr_info.uiFrameRate);
		DBG_DUMP("  GOP=    %d\r\n", pVBRConfig[venc_channel].vbr_info.uiGOP);

	} else if(pStreamConfig[venc_channel].eBrcType == UIAPPIPCAM_BRC_NONE){

		DBG_DUMP(" None BRC byterate=%d\r\n", sVencSetting.sVencBRCNoneInfo.uiTargetByterate);
		DBG_DUMP("  fps=    %d\r\n", sVencSetting.sVencBRCNoneInfo.uiFrameRate);
		DBG_DUMP("  GOP=    %d\r\n", sVencSetting.sVencBRCNoneInfo.uiGOP);

	} else if(pStreamConfig[venc_channel].eBrcType == UIAPPIPCAM_BRC_FIXQP){

		DBG_DUMP(" FixQP byterate=%d\r\n", pFixQPConfig[venc_channel].sFixQPInfo.uiByteRate);
		DBG_DUMP("  fps=    %d\r\n", pFixQPConfig[venc_channel].sFixQPInfo.uiFrameRate);
		DBG_DUMP("  GOP=    %d\r\n", pFixQPConfig[venc_channel].sFixQPInfo.uiGOP);
	}
	else if(pStreamConfig[venc_channel].eBrcType == UIAPPIPCAM_BRC_EVBR){

		DBG_DUMP(" FixQP byterate=%d\r\n", pEVBRConfig[venc_channel].evbr_info.uiByteRate);
		DBG_DUMP("  fps=    %d\r\n", pEVBRConfig[venc_channel].evbr_info.uiFrameRate);
		DBG_DUMP("  GOP=    %d\r\n", pEVBRConfig[venc_channel].evbr_info.uiGOP);
	}

	else{
		DBG_ERR("get ch %d BRC error(%d)\r\n",venc_channel,pStreamConfig[venc_channel].eBrcType);

	}
	DBG_DUMP("  3dnr=   %d\r\n", sVencSetting.sNRSetting.bEnable);
	NvtUctrl_SetConfigData((void *)&sVencSetting, sizeof(CodecVencSetting));
#else
	DBG_ERR("not support !!\r\n");
#endif
	return 0;
}

static INT32 param_venc_set(INT32 argc, char *argv[])
{
	#if(IPCAM_FUNC ==ENABLE)
	UINT8 venc_channel = 0;
	CodecVencSetting sVencSetting = {0};
	if (argc != 1) {
		DBG_ERR("argc!=1 (%d)\r\n", argc);
		return -1;
	}

	if ((venc_channel = atoi(argv[0])) >= MOVIE_MAX_CHANNEL) {
		DBG_ERR("invalid venc channel: %u (Max: %d)\r\n", venc_channel, MOVIE_MAX_CHANNEL);
		return -1;
	}
	UIAPPIPCAM_ENCODE_CONFIG *pStreamConfig = NULL;
	pStreamConfig = UIAppIPCam_get_EncodeConfig();
	// Channel
	pMovieInfo->liveview_channel_id.index = venc_channel;
	pMovieInfo->liveview_channel_id.isSet = 1;





	NvtUctrl_GetConfigData((void *)&sVencSetting, sizeof(CodecVencSetting));
	//UIAPPIPCAM_CBR_CONFIG *pCBRConfig = UIAppIPCam_get_CBRInfo();
	//UIAPPIPCAM_VBR_CONFIG *pVBRConfig = UIAppIPCam_get_VBRInfo();
	//UIAPPIPCAM_BRC_NONE_CONFIG *pBRCNoneConfig = UIAppIPCam_get_BRCNoneInfo();
	DBG_DUMP("=================\r\n");
	DBG_DUMP("set video config:\r\n");
	DBG_DUMP("channel:%d\r\n"
		     "codec=%d\r\n"
		     "size=%dx%d\r\n"
		     "3DNR=%d\r\n",
			 venc_channel,
			 sVencSetting.eVencFormat,
			 sVencSetting.uiTargetWidth, sVencSetting.uiTargetHeight,
			 sVencSetting.sNRSetting.bEnable);
	DBG_DUMP("BRC type=%d\r\n",sVencSetting.uiBRCType);
	if(sVencSetting.uiBRCType == UIAPPIPCAM_BRC_NONE){
		DBG_DUMP("NONE FPS=%d\tGOP=%d\tbitrate=%d\r\n",
			sVencSetting.sVencBRCNoneInfo.uiFrameRate,
			sVencSetting.sVencBRCNoneInfo.uiGOP,
			sVencSetting.sVencBRCNoneInfo.uiTargetByterate);

	}
	else if(sVencSetting.uiBRCType == UIAPPIPCAM_BRC_CBR){
		DBG_DUMP("CBR FPS=%d\tGOP=%d\tbitrate=%d\r\n",
			sVencSetting.sVencCBRConfig.uiFrameRate,
			sVencSetting.sVencCBRConfig.uiGOP,
			sVencSetting.sVencCBRConfig.uiByteRate);

	}
	else if(sVencSetting.uiBRCType == UIAPPIPCAM_BRC_VBR){
		DBG_DUMP("VBR FPS=%d\tGOP=%d\tbitrate=%d\r\n",
			sVencSetting.sVencVBRConfig.uiFrameRate,
			sVencSetting.sVencVBRConfig.uiGOP,
			sVencSetting.sVencVBRConfig.uiByteRate);
	}
	else if(sVencSetting.uiBRCType == UIAPPIPCAM_BRC_EVBR){
		DBG_DUMP("EVBR FPS=%d\tGOP=%d\tbitrate=%d\r\n",
			sVencSetting.sVencEVBRConfig.uiFrameRate,
			sVencSetting.sVencEVBRConfig.uiGOP,
			sVencSetting.sVencEVBRConfig.uiByteRate);
	}

	UIAPPIPCAM_VIDEO_MAXCONFIG *pMaxConfig = UIAppIPCam_get_VdoMaxConfig();
	UINT32 uiCheckByteRate = pMaxConfig[venc_channel].uiMaxByteRate;
	UINT32 uiCheckWidth = pMaxConfig[venc_channel].uiMaxWidth;
	UINT32 uiCheckHeight = pMaxConfig[venc_channel].uiMaxHeight;

	if(uiCheckWidth < sVencSetting.uiTargetWidth){

		DBG_ERR("Error: path:%d maxWidth:%d < set Width:%d\r\n",
			venc_channel,uiCheckWidth,sVencSetting.uiTargetWidth);
		return -1;
	}

	if(uiCheckHeight < sVencSetting.uiTargetHeight){

		DBG_ERR("Error: path:%d maxHeight:%d < set Height:%d\r\n",
			venc_channel,uiCheckHeight,sVencSetting.uiTargetHeight);
		return -1;
	}

	if(sVencSetting.uiBRCType == UIAPPIPCAM_BRC_NONE){
		if(uiCheckByteRate < sVencSetting.sVencBRCNoneInfo.uiTargetByterate){
			DBG_ERR("Error: path:%d maxbyterate:%d  < set ByteRate:%d\r\n",
				venc_channel,
				uiCheckByteRate,
				sVencSetting.sVencBRCNoneInfo.uiTargetByterate);
			return -1;
		}
	}
	if(sVencSetting.uiBRCType == UIAPPIPCAM_BRC_CBR){
		if(uiCheckByteRate < sVencSetting.sVencCBRConfig.uiByteRate){
			DBG_ERR("Error CBR: path:%d maxbyterate:%d  < set ByteRate:%d!\r\n",
				venc_channel,
				uiCheckByteRate,
				sVencSetting.sVencCBRConfig.uiByteRate);
			return -1;
		}
	}

	if(sVencSetting.uiBRCType == UIAPPIPCAM_BRC_VBR){
		if(uiCheckByteRate < sVencSetting.sVencVBRConfig.uiByteRate){
			DBG_ERR("Error VBR: path:%d maxbyterate:%d  < set ByteRate:%d!!\r\n",
				venc_channel,
				uiCheckByteRate,
				sVencSetting.sVencVBRConfig.uiByteRate);
			return -1;
		}
	}

	if(sVencSetting.uiBRCType == UIAPPIPCAM_BRC_EVBR){
		if(uiCheckByteRate < sVencSetting.sVencEVBRConfig.uiByteRate){
			DBG_ERR("Error EVBR: path:%d maxbyterate:%d  < set ByteRate:%d!!\r\n",
				venc_channel,
				uiCheckByteRate,
				sVencSetting.sVencEVBRConfig.uiByteRate);
			return -1;
		}
	}

	if(sVencSetting.uiBRCType == UIAPPIPCAM_BRC_FIXQP){
		if(uiCheckByteRate < sVencSetting.sVencFixQPConfig.uiByteRate){
			DBG_ERR("Error FIXQP: path:%d maxbyterate:%d  < set ByteRate:%d!!!\r\n",
				venc_channel,
				uiCheckByteRate,
				sVencSetting.sVencFixQPConfig.uiByteRate);
			return -1;
		}
	}


	// rotate

	if(sVencSetting.eRotateType != pStreamConfig[venc_channel].eRotateType){
		pMovieInfo->rotate.isSet = 1;
		pMovieInfo->rotate.index = sVencSetting.eRotateType;
		if(sVencSetting.eRotateType == UIAPPIPCAM_RT_EXTEND_90 ||
			sVencSetting.eRotateType == UIAPPIPCAM_RT_EXTEND_270){
			if((sVencSetting.uiTargetHeight%16) != 0){
				DBG_ERR("after rotate, width(%d) need alignment 16!!!!!\r\n",sVencSetting.uiTargetHeight);
				return -1;
			}

		}
	}


	// profile
	pMovieInfo->profile.isSet =1;
	pMovieInfo->profile.index = sVencSetting.eVencProfile;

	// Encoded Format
	switch (sVencSetting.eVencFormat) {
	case CODEC_VENC_FORMAT_MJPEG:
		pMovieInfo->liveview_vid_enc_fmt.index = IPCAMCODEC_MJPG;
		pStreamConfig[venc_channel].sSEISetting.bEnable = FALSE;
		pMovieInfo->liveview_vid_sei.isSet = 0;
		break;
	case CODEC_VENC_FORMAT_H265:
		pMovieInfo->liveview_vid_enc_fmt.index = IPCAMCODEC_H265;
		//pStreamConfig[venc_channel].sSEISetting.bEnable = sVencSetting.sSEISetting.bEnable;
		pMovieInfo->liveview_vid_sei.bEnable = sVencSetting.sSEISetting.bEnable;
		pMovieInfo->liveview_vid_sei.isSet = 1;
		//DBG_WRN("[VDOENC][%d] SEI enable = %d \r\n",venc_channel,sVencSetting.sSEISetting.bEnable);
		break;
	case CODEC_VENC_FORMAT_YUV:
		pMovieInfo->liveview_vid_enc_fmt.index = IPCAMCODEC_YUV;
		pStreamConfig[venc_channel].sSEISetting.bEnable = FALSE;
		pMovieInfo->liveview_vid_sei.isSet = 0;
		break;
	case CODEC_VENC_FORMAT_H264:
	default:
		pMovieInfo->liveview_vid_enc_fmt.index = IPCAMCODEC_H264;
		//pStreamConfig[venc_channel].sSEISetting.bEnable = sVencSetting.sSEISetting.bEnable;
		pMovieInfo->liveview_vid_sei.bEnable = sVencSetting.sSEISetting.bEnable;
		pMovieInfo->liveview_vid_sei.isSet = 1;
	    //DBG_WRN("[VDOENC][%d] SEI enable = %d \r\n",venc_channel,sVencSetting.sSEISetting.bEnable);
		break;
	}
	pMovieInfo->liveview_vid_enc_fmt.isSet = 1;


    /////set aq info
	pMovieInfo->liveview_vid_aq.isSet = 1;
	pMovieInfo->liveview_vid_aq.sAQInfo.iAqEnable = sVencSetting.sAQSetting.iAqEnable;
	pMovieInfo->liveview_vid_aq.sAQInfo.uiAqStr = sVencSetting.sAQSetting.uiAqStr;
	pMovieInfo->liveview_vid_aq.sAQInfo.iMaxSraq = sVencSetting.sAQSetting.iMaxSraq;
	pMovieInfo->liveview_vid_aq.sAQInfo.iMinSraq = sVencSetting.sAQSetting.iMinSraq;
	pMovieInfo->liveview_vid_aq.sAQInfo.iSraqInitAslog2 = sVencSetting.sAQSetting.iSraqInitAslog2;
	pMovieInfo->liveview_vid_aq.sAQInfo.iSraqConstAslog2 = sVencSetting.sAQSetting.iSraqConstAslog2;
	// Bitrate
	if(
	 sVencSetting.uiBRCType == UIAPPIPCAM_BRC_CBR){
		//CBR mode
		pMovieInfo->liveview_vid_cbr.isSet = 1;
		pMovieInfo->liveview_vid_cbr.sCBRInfo.uiEnable = sVencSetting.sVencCBRConfig.uiEnable;
		pMovieInfo->liveview_vid_cbr.sCBRInfo.uiStaticTime = sVencSetting.sVencCBRConfig.uiStaticTime;
		pMovieInfo->liveview_vid_cbr.sCBRInfo.uiByteRate = sVencSetting.sVencCBRConfig.uiByteRate;
		pMovieInfo->liveview_vid_cbr.sCBRInfo.uiFrameRate = sVencSetting.sVencCBRConfig.uiFrameRate;
		pMovieInfo->liveview_vid_cbr.sCBRInfo.uiGOP = sVencSetting.sVencCBRConfig.uiGOP;
		pMovieInfo->liveview_vid_cbr.sCBRInfo.uiInitIQp = sVencSetting.sVencCBRConfig.uiInitIQp;
		pMovieInfo->liveview_vid_cbr.sCBRInfo.uiMinIQp = sVencSetting.sVencCBRConfig.uiMinIQp;
		pMovieInfo->liveview_vid_cbr.sCBRInfo.uiMaxIQp = sVencSetting.sVencCBRConfig.uiMaxIQp;
		pMovieInfo->liveview_vid_cbr.sCBRInfo.uiInitPQp = sVencSetting.sVencCBRConfig.uiInitPQp;
		pMovieInfo->liveview_vid_cbr.sCBRInfo.uiMinPQp = sVencSetting.sVencCBRConfig.uiMinPQp;
		pMovieInfo->liveview_vid_cbr.sCBRInfo.uiMaxPQp = sVencSetting.sVencCBRConfig.uiMaxPQp;
		pMovieInfo->liveview_vid_cbr.sCBRInfo.iIPWeight = sVencSetting.sVencCBRConfig.iIPWeight;
		pMovieInfo->liveview_vid_cbr.sCBRInfo.uiRowRcEnalbe = sVencSetting.sVencCBRConfig.uiRowRcEnalbe;
		pMovieInfo->liveview_vid_cbr.sCBRInfo.uiRowRcQpRange = sVencSetting.sVencCBRConfig.uiRowRcQpRange;
		pMovieInfo->liveview_vid_cbr.sCBRInfo.uiRowRcQpStep = sVencSetting.sVencCBRConfig.uiRowRcQpStep;
	}
	else if(sVencSetting.uiBRCType == UIAPPIPCAM_BRC_VBR){
		//VBR mode
		pMovieInfo->liveview_vid_vbr.isSet = 1;
		pMovieInfo->liveview_vid_vbr.sVBRInfo.uiEnable = sVencSetting.sVencVBRConfig.uiEnable;
		pMovieInfo->liveview_vid_vbr.sVBRInfo.uiStaticTime = sVencSetting.sVencVBRConfig.uiStaticTime;
		pMovieInfo->liveview_vid_vbr.sVBRInfo.uiByteRate = sVencSetting.sVencVBRConfig.uiByteRate;
		pMovieInfo->liveview_vid_vbr.sVBRInfo.uiFrameRate = sVencSetting.sVencVBRConfig.uiFrameRate;
		pMovieInfo->liveview_vid_vbr.sVBRInfo.uiGOP = sVencSetting.sVencVBRConfig.uiGOP;
		pMovieInfo->liveview_vid_vbr.sVBRInfo.uiInitIQp = sVencSetting.sVencVBRConfig.uiInitIQp;
		pMovieInfo->liveview_vid_vbr.sVBRInfo.uiMinIQp = sVencSetting.sVencVBRConfig.uiMinIQp;
		pMovieInfo->liveview_vid_vbr.sVBRInfo.uiMaxIQp = sVencSetting.sVencVBRConfig.uiMaxIQp;
		pMovieInfo->liveview_vid_vbr.sVBRInfo.uiInitPQp = sVencSetting.sVencVBRConfig.uiInitPQp;
		pMovieInfo->liveview_vid_vbr.sVBRInfo.uiMinPQp = sVencSetting.sVencVBRConfig.uiMinPQp;
		pMovieInfo->liveview_vid_vbr.sVBRInfo.uiMaxPQp = sVencSetting.sVencVBRConfig.uiMaxPQp;
		pMovieInfo->liveview_vid_vbr.sVBRInfo.iIPWeight = sVencSetting.sVencVBRConfig.iIPWeight;
		pMovieInfo->liveview_vid_vbr.sVBRInfo.uiChangePos = sVencSetting.sVencVBRConfig.uiChangePos;
		pMovieInfo->liveview_vid_vbr.sVBRInfo.uiRowRcEnalbe = sVencSetting.sVencVBRConfig.uiRowRcEnalbe;
		pMovieInfo->liveview_vid_vbr.sVBRInfo.uiRowRcQpRange = sVencSetting.sVencVBRConfig.uiRowRcQpRange;
		pMovieInfo->liveview_vid_vbr.sVBRInfo.uiRowRcQpStep = sVencSetting.sVencVBRConfig.uiRowRcQpStep;
	}
	else if(sVencSetting.uiBRCType == UIAPPIPCAM_BRC_EVBR){
		//EVBR mode
		pMovieInfo->liveview_vid_evbr.isSet = 1;
		pMovieInfo->liveview_vid_evbr.sEVBRInfo.uiEnable = sVencSetting.sVencEVBRConfig.uiEnable;
		pMovieInfo->liveview_vid_evbr.sEVBRInfo.uiStaticTime = sVencSetting.sVencEVBRConfig.uiStaticTime;
		pMovieInfo->liveview_vid_evbr.sEVBRInfo.uiByteRate = sVencSetting.sVencEVBRConfig.uiByteRate;
		pMovieInfo->liveview_vid_evbr.sEVBRInfo.uiFrameRate = sVencSetting.sVencEVBRConfig.uiFrameRate;
		pMovieInfo->liveview_vid_evbr.sEVBRInfo.uiGOP = sVencSetting.sVencEVBRConfig.uiGOP;
		pMovieInfo->liveview_vid_evbr.sEVBRInfo.uiKeyPPeriod = sVencSetting.sVencEVBRConfig.uiKeyPPeriod;
		pMovieInfo->liveview_vid_evbr.sEVBRInfo.uiInitIQp = sVencSetting.sVencEVBRConfig.uiInitIQp;
		pMovieInfo->liveview_vid_evbr.sEVBRInfo.uiMinIQp = sVencSetting.sVencEVBRConfig.uiMinIQp;
		pMovieInfo->liveview_vid_evbr.sEVBRInfo.uiMaxIQp = sVencSetting.sVencEVBRConfig.uiMaxIQp;
		pMovieInfo->liveview_vid_evbr.sEVBRInfo.uiInitPQp = sVencSetting.sVencEVBRConfig.uiInitPQp;
		pMovieInfo->liveview_vid_evbr.sEVBRInfo.uiMinPQp = sVencSetting.sVencEVBRConfig.uiMinPQp;
		pMovieInfo->liveview_vid_evbr.sEVBRInfo.uiMaxPQp = sVencSetting.sVencEVBRConfig.uiMaxPQp;
		pMovieInfo->liveview_vid_evbr.sEVBRInfo.iIPWeight = sVencSetting.sVencEVBRConfig.iIPWeight;
		pMovieInfo->liveview_vid_evbr.sEVBRInfo.iKeyPWeight = sVencSetting.sVencEVBRConfig.iKeyPWeight;
		pMovieInfo->liveview_vid_evbr.sEVBRInfo.iMotionAQStrength = sVencSetting.sVencEVBRConfig.iMotionAQStrength;
		pMovieInfo->liveview_vid_evbr.sEVBRInfo.uiRowRcEnalbe = sVencSetting.sVencEVBRConfig.uiRowRcEnalbe;
		pMovieInfo->liveview_vid_evbr.sEVBRInfo.uiRowRcQpRange = sVencSetting.sVencEVBRConfig.uiRowRcQpRange;
		pMovieInfo->liveview_vid_evbr.sEVBRInfo.uiRowRcQpStep = sVencSetting.sVencEVBRConfig.uiRowRcQpStep;
		pMovieInfo->liveview_vid_evbr.sEVBRInfo.uiStillFrameCnd = sVencSetting.sVencEVBRConfig.uiStillFrameCnd;
		pMovieInfo->liveview_vid_evbr.sEVBRInfo.uiMotionRatioThd = sVencSetting.sVencEVBRConfig.uiMotionRatioThd;
		pMovieInfo->liveview_vid_evbr.sEVBRInfo.uiIPsnrCnd = sVencSetting.sVencEVBRConfig.uiIPsnrCnd;
		pMovieInfo->liveview_vid_evbr.sEVBRInfo.uiPPsnrCnd = sVencSetting.sVencEVBRConfig.uiPPsnrCnd;
		pMovieInfo->liveview_vid_evbr.sEVBRInfo.uiKeyPPsnrCnd = sVencSetting.sVencEVBRConfig.uiKeyPPsnrCnd;
	}
	else if(sVencSetting.uiBRCType == UIAPPIPCAM_BRC_FIXQP){
		//fixQP mode
		pMovieInfo->liveview_vid_fixqp.isSet = 1;
		pMovieInfo->liveview_vid_fixqp.sFixQPInfo.uiByteRate = sVencSetting.sVencFixQPConfig.uiByteRate;
		pMovieInfo->liveview_vid_fixqp.sFixQPInfo.uiEnable = sVencSetting.sVencFixQPConfig.uiEnable;
		pMovieInfo->liveview_vid_fixqp.sFixQPInfo.uiFrameRate = sVencSetting.sVencFixQPConfig.uiFrameRate;
		pMovieInfo->liveview_vid_fixqp.sFixQPInfo.uiGOP = sVencSetting.sVencFixQPConfig.uiGOP;
		pMovieInfo->liveview_vid_fixqp.sFixQPInfo.uiIFixQP = sVencSetting.sVencFixQPConfig.uiIFixQP;
		pMovieInfo->liveview_vid_fixqp.sFixQPInfo.uiPFixQP = sVencSetting.sVencFixQPConfig.uiPFixQP;

	}
	else if(sVencSetting.uiBRCType == UIAPPIPCAM_BRC_NONE){
		pMovieInfo->liveview_vid_brc_none.isSet = 1;
		pMovieInfo->liveview_vid_brc_none.uiFrameRate = sVencSetting.sVencBRCNoneInfo.uiFrameRate;
		pMovieInfo->liveview_vid_brc_none.uiGOP= sVencSetting.sVencBRCNoneInfo.uiGOP;
		pMovieInfo->liveview_vid_brc_none.uiTargetByterate= sVencSetting.sVencBRCNoneInfo.uiTargetByterate;

	}
	else{
		DBG_ERR("uiBRCType error %d\r\n",sVencSetting.uiBRCType);
		return -1;
	}

	// SVC-T
#if 1
	switch (sVencSetting.sSVCSetting.eSVCTemporalLayer) {
	case CODEC_VENC_SVCT_LAYER_1:
		pMovieInfo->liveview_vid_svc_t.index = MP_VDOENC_SVC_LAYER1;
		break;
	case CODEC_VENC_SVCT_LAYER_2:
		pMovieInfo->liveview_vid_svc_t.index = MP_VDOENC_SVC_LAYER2;
		break;
	case CODEC_VENC_SVCT_DISABLE:
	default:
		pMovieInfo->liveview_vid_svc_t.index = MP_VDOENC_SVC_DISABLE;
		break;
	}
	pMovieInfo->liveview_vid_svc_t.isSet = 1;
#endif


	// Resolution
	if (sVencSetting.uiTargetWidth <= CROP_SOURCE_W && sVencSetting.uiTargetHeight <= CROP_SOURCE_H) {
		pMovieInfo->liveview_vid_size.size.h = sVencSetting.uiTargetHeight;
		pMovieInfo->liveview_vid_size.size.w = sVencSetting.uiTargetWidth;
		pMovieInfo->liveview_vid_size.isSet = 1;
	} else {
		DBG_WRN("New resolution(%u, %u) is over limitation(%u, %u)\r\n", sVencSetting.uiTargetWidth,
				sVencSetting.uiTargetHeight,
				CROP_SOURCE_W,
				CROP_SOURCE_H);
	}


	//encoder 3DNR
	DBG_DUMP("encode 3DNR set:%d\r\n",sVencSetting.sNRSetting.bEnable);
	pMovieInfo->NRSetting.bEnable= sVencSetting.sNRSetting.bEnable;
	pMovieInfo->NRSetting.isSet = 1;

	//LTR
	pMovieInfo->LTRset.isSet = 1;
	pMovieInfo->LTRset.uiLTRPreRef = sVencSetting.sLTRInfo.uiLTRPreRef;

	if(pMaxConfig[venc_channel].uiLTRInterval > 0){
		pMovieInfo->LTRset.uiLTRInterval = sVencSetting.sLTRInfo.uiLTRInterval;
	}
	else{
		if(sVencSetting.sLTRInfo.uiLTRInterval > 0){
			DBG_WRN("LTR buffer not allocate, disable LTR function\r\n");
			pMovieInfo->LTRset.uiLTRInterval = 0;
		}
		else{
			pMovieInfo->LTRset.uiLTRInterval = sVencSetting.sLTRInfo.uiLTRInterval;
		}
	}

	ImageUnit_Begin(&ISF_VdoEnc, 0);
	if (sVencSetting.eVencFormat == CODEC_VENC_FORMAT_MJPEG) {
		#if (VIDEO_FUNC_MJPG == ENABLE)
		MP_VDOENC_ENCODER *pObj = MP_MjpgEnc_getVideoObject(venc_channel);
		ImageUnit_SetParam(venc_channel, VDOENC_PARAM_ENCODER_OBJ, (UINT32)pObj);
		#else
		ImageUnit_SetParam(venc_channel, VDOENC_PARAM_ENCODER_OBJ, 0);
		DBG_ERR("[UCTRL][%d] codec = %d not support\r\n", venc_channel, sVencSetting.eVencFormat);
		#endif
	}
	if (sVencSetting.eVencFormat == CODEC_VENC_FORMAT_H264) {
		#if (VIDEO_FUNC_H264 == ENABLE)
		MP_VDOENC_ENCODER *pObj = MP_H264Enc_getVideoObject(venc_channel);
		ImageUnit_SetParam(venc_channel, VDOENC_PARAM_ENCODER_OBJ, (UINT32)pObj);
		#else
		ImageUnit_SetParam(venc_channel, VDOENC_PARAM_ENCODER_OBJ, 0);
		DBG_ERR("[UCTRL][%d] codec = %d not support\r\n", venc_channel, sVencSetting.eVencFormat);
		#endif
	}
	if (sVencSetting.eVencFormat == CODEC_VENC_FORMAT_H265) {
		#if (VIDEO_FUNC_H265 == ENABLE)
		MP_VDOENC_ENCODER *pObj = MP_H265Enc_getVideoObject(venc_channel);
		ImageUnit_SetParam(venc_channel, VDOENC_PARAM_ENCODER_OBJ, (UINT32)pObj);
		#else
		ImageUnit_SetParam(venc_channel, VDOENC_PARAM_ENCODER_OBJ, 0);
		DBG_ERR("[UCTRL][%d] codec = %d not support\r\n", venc_channel, sVencSetting.eVencFormat);
		#endif
	}
	ImageUnit_End();
	#else
	DBG_ERR("not support\r\n");
	#endif

	return 0;
}

static INT32 param_vencROI_get(INT32 argc, char *argv[])
{
	#if(IPCAM_FUNC== ENABLE)
		UINT8 venc_channel = 0;
		CodecVencROISetting sVencROISetting = {0};
		UIAPPIPCAM_ROI_CONFIG *pROIConfig = UIAppIPCam_get_ROIInfo();
    	UINT32 i = 0;

		if (argc != 1) {
			DBG_ERR("argc!=1 (%d)\r\n", argc);
			return -1;
		}

		if ((venc_channel = atoi(argv[0])) >= MOVIE_MAX_CHANNEL) {
			DBG_ERR("invalid venc channel: %u (Max: %d)\r\n", venc_channel, MOVIE_MAX_CHANNEL);
			return -1;
		}



		for (i = 0; i < MP_VDOENC_ROI_NUM_MAX; i++) {
			//sVencROISetting.uiCount 		  = pRoi->uiCount;
			sVencROISetting.roi[i].bROIEnable = pROIConfig[venc_channel].roi[i].uiEnable;
			sVencROISetting.roi[i].uiHeight   = pROIConfig[venc_channel].roi[i].uiHeight;
			sVencROISetting.roi[i].uiWidth	  = pROIConfig[venc_channel].roi[i].uiWidth;
			sVencROISetting.roi[i].iQP        = pROIConfig[venc_channel].roi[i].iQP;
			sVencROISetting.roi[i].uiX        = pROIConfig[venc_channel].roi[i].uiCoord_X;
			sVencROISetting.roi[i].uiY        = pROIConfig[venc_channel].roi[i].uiCoord_Y;
		}
        sVencROISetting.uiDeltaQp = pROIConfig[venc_channel].uiDeltaQp;
		sVencROISetting.bSmartROI = pROIConfig[venc_channel].bSmartROI;
		NvtUctrl_SetConfigData((void *)&sVencROISetting, sizeof(CodecVencROISetting));
	#else
		DBG_ERR("not support!!\r\n");
	#endif
	return 0;

//#NT#2016/07/26#Niven Cho -end
}

static INT32 param_vencROI_set(INT32 argc, char *argv[])
{
	#if(IPCAM_FUNC==ENABLE)
		UINT8 venc_channel = 0;
		CodecVencROISetting sVencROISetting = {0};
		UIAPPIPCAM_ROI_CONFIG *pROIConfig = UIAppIPCam_get_ROIInfo();
        UINT32 i = 0;

		if (argc != 1) {
			DBG_ERR("argc!=1 (%d)\r\n", argc);
			return -1;
		}

		if ((venc_channel = atoi(argv[0])) >= MOVIE_MAX_CHANNEL) {
			DBG_ERR("invalid venc channel: %u (Max: %d)\r\n", venc_channel, MOVIE_MAX_CHANNEL);
			return -1;
		}

		NvtUctrl_GetConfigData((void *)&sVencROISetting, sizeof(CodecVencROISetting));

		MP_VDOENC_ROI_INFO ROIInfo ={0};
		for (i = 0; i < MP_VDOENC_ROI_NUM_MAX; i++) {
			//ROIInfo.uiCount          = sVencROISetting.uiCount;
			ROIInfo.roi[i].uiEnable  = sVencROISetting.roi[i].bROIEnable;
			ROIInfo.roi[i].uiHeight  = sVencROISetting.roi[i].uiHeight;
			ROIInfo.roi[i].uiWidth   = sVencROISetting.roi[i].uiWidth;
			ROIInfo.roi[i].iQP      = sVencROISetting.roi[i].iQP;
			ROIInfo.roi[i].uiCoord_X = sVencROISetting.roi[i].uiX;
			ROIInfo.roi[i].uiCoord_Y = sVencROISetting.roi[i].uiY;

			pROIConfig[venc_channel].roi[i].uiEnable 	= ROIInfo.roi[i].uiEnable;
			pROIConfig[venc_channel].roi[i].uiHeight 	= ROIInfo.roi[i].uiHeight;
			pROIConfig[venc_channel].roi[i].uiWidth 	= ROIInfo.roi[i].uiWidth;
			pROIConfig[venc_channel].roi[i].iQP 		= ROIInfo.roi[i].iQP;
			pROIConfig[venc_channel].roi[i].uiCoord_X 	= ROIInfo.roi[i].uiCoord_X;
			pROIConfig[venc_channel].roi[i].uiCoord_Y 	= ROIInfo.roi[i].uiCoord_Y;
			#if 0
			DBG_WRN("ROI i=%d\r\n",i);
			DBG_WRN("ROI enable:%d\r\n",ROIInfo.roi[i].uiEnable);
			DBG_WRN("ROI h=%d\r\n",ROIInfo.roi[i].uiHeight );
			DBG_WRN("ROI w=%d\r\n",ROIInfo.roi[i].uiHeight );
			DBG_WRN("ROI uiQP=%d\r\n",ROIInfo.roi[i].iQP );
			DBG_WRN("ROI uiCoord_X=%d\r\n",ROIInfo.roi[i].uiCoord_X );
			DBG_WRN("ROI uiCoord_Y=%d\r\n",ROIInfo.roi[i].uiCoord_Y );
			#endif

			if(sVencROISetting.roi[i].bROIEnable == TRUE && sVencROISetting.bSmartROI == TRUE){
				DBG_WRN("Can not run smart ROI, ROI index:%d is enable, please close it\r\n",i);

			}

		}
        ROIInfo.uiDeltaQp= sVencROISetting.uiDeltaQp;
		pROIConfig[venc_channel].uiDeltaQp = ROIInfo.uiDeltaQp;
		pROIConfig[venc_channel].bSmartROI = sVencROISetting.bSmartROI;
		ImageUnit_Begin(&ISF_VdoEnc, 0);
			ImageUnit_SetParam(venc_channel+ISF_OUT_BASE,VDOENC_PARAM_ROI,(UINT32)&ROIInfo);
			ImageUnit_SetParam(venc_channel+ISF_OUT_BASE,VDOENC_PARAM_START_SMART_ROI,(UINT32)pROIConfig[venc_channel].bSmartROI);
		ImageUnit_End();
		ImageApp_IPCam_reset_path(venc_channel);
	#else
		DBG_ERR("not support!!\r\n");

	#endif
	return 0;

//#NT#2016/07/26#Niven Cho -end
}

static INT32 param_vstat_get(INT32 argc, char *argv[])
{
	CodecVencStatus sVencStatus = {0};

	if (argc != 1) {
		DBG_ERR("argc!=1 (%d)\r\n", argc);
		return -1;
	}

	//if ((venc_channel = atoi(argv[0])) >= IPL_ID_MAX_NUM)
	//{
	//    DBG_ERR("invalid venc channel: %u (Max: %d)\r\n",venc_channel , IPL_ID_MAX_NUM);
	//    return -1;
	//}

	sVencStatus.bStart = bUctrlRecStart;

	NvtUctrl_SetConfigData((void *)&sVencStatus, sizeof(CodecVencStatus));

	return 0;
}

static INT32 param_venc_ireq(INT32 argc, char *argv[])
{
	UINT8 venc_channel = 0;

	if (argc != 1) {
		DBG_ERR("argc!=1 (%d)\r\n", argc);
		return -1;
	}

	if ((venc_channel = atoi(argv[0])) >= MOVIE_MAX_CHANNEL) {
		DBG_ERR("invalid venc channel: %u (Max: %d)\r\n", venc_channel, MOVIE_MAX_CHANNEL);
		return -1;
	}

	ImageUnit_Begin(&ISF_VdoEnc, 0);
		ImageUnit_SetParam(venc_channel, VDOENC_PARAM_RESET_IFRAME, 1);
	ImageUnit_End();

	return 0;
}

static INT32 param_aenc_get(INT32 argc, char *argv[])
{
#if(IPCAM_MODE == ENABLE)
#if(AUDIO_CMD_UCTRL_ENABLE == ENABLE)
	UINT8 aenc_channel = 0;
	CodecAinSetting sAinSetting = {0};
	UINT32 volume;

	if (argc != 1) {
		DBG_ERR("argc!=1 (%d)\r\n", argc);
		return -1;
	}

	if ((aenc_channel = atoi(argv[0])) >= MAXAUDIO_CH) {
		DBG_ERR("invalid aenc channel: %u (Max: %d)\r\n", aenc_channel, MAXAUDIO_CH-1);
		return -1;
	}

	UIAPPIPCAM_AUDIO_INFO *pAudioInfo = UIAppIPCam_get_AudioInfo();

	sAinSetting.sAinNRSetting.bNREnable = ImageUnit_GetParam(&ISF_AudIn, ISF_CTRL, AUDIN_PARAM_ANR_EN);
	#if IPCAM_ANR_FUNC
	sAinSetting.sAinNRSetting.iNRdB = nvtanr_getconfig(NVTANR_CONFIG_ID_NRDB);
	sAinSetting.sAinNRSetting.iHPFCutOffFreq = nvtanr_getconfig(NVTANR_CONFIG_ID_HPF_FREQ);
	sAinSetting.sAinNRSetting.iBiasSensitive = nvtanr_getconfig(NVTANR_CONFIG_ID_BIAS_SENSITIVE);
	#endif

	switch (pAudioInfo->SampleRate) {
	case 8000:
		sAinSetting.eAinSampleRate = CODEC_AIO_SAMPLE_RATE_8000;
		break;
	case 11025:
		sAinSetting.eAinSampleRate = CODEC_AIO_SAMPLE_RATE_11025;
		break;
	case 12000:
		sAinSetting.eAinSampleRate = CODEC_AIO_SAMPLE_RATE_12000;
		break;
	case 16000:
		sAinSetting.eAinSampleRate = CODEC_AIO_SAMPLE_RATE_16000;
		break;
	case 22050:
		sAinSetting.eAinSampleRate = CODEC_AIO_SAMPLE_RATE_22050;
		break;
	case 24000:
		sAinSetting.eAinSampleRate = CODEC_AIO_SAMPLE_RATE_24000;
		break;
	case 32000:
		sAinSetting.eAinSampleRate = CODEC_AIO_SAMPLE_RATE_32000;
		break;
	case 44100:
		sAinSetting.eAinSampleRate = CODEC_AIO_SAMPLE_RATE_44100;
		break;
	case 48000:
	default:
		sAinSetting.eAinSampleRate = CODEC_AIO_SAMPLE_RATE_48000;
		break;
	}

	switch (pAudioInfo->codec) {
	case MOVAUDENC_AAC:
		sAinSetting.eAinFormat = CODEC_AENC_FORMAT_AAC;
		break;
	case MOVAUDENC_ALAW:
		sAinSetting.eAinFormat = CODEC_AENC_FORMAT_ALAW;
		break;
	case MOVAUDENC_PCM:
		sAinSetting.eAinFormat = CODEC_AENC_FORMAT_PCM;
		break;
	case MOVAUDENC_ULAW:
	default:
		sAinSetting.eAinFormat = CODEC_AENC_FORMAT_ULAW;
		break;
	}

	switch (pAudioInfo->channel) {
	case AUDIO_CH_LEFT:
		sAinSetting.eAinSoundMode = CODEC_AIO_SOUND_MODE_LEFT;
		break;
	case AUDIO_CH_RIGHT:
		sAinSetting.eAinSoundMode = CODEC_AIO_SOUND_MODE_RIGHT;
		break;
	case AUDIO_CH_MONO:
		sAinSetting.eAinSoundMode = CODEC_AIO_SOUND_MODE_MONO;
		break;
	case AUDIO_CH_STEREO:
	default:
		sAinSetting.eAinSoundMode = CODEC_AIO_SOUND_MODE_STEREO;
		break;
	}
#if 1
	//now always FULL
	sAinSetting.eAinDuplexMode = CODEC_AIO_DUPLEX_MODE_FULL;
#else

	if (System_GetAudioMode() == TRUE) {
		sAinSetting.eAinDuplexMode = CODEC_AIO_DUPLEX_MODE_FULL;
	} else {
		sAinSetting.eAinDuplexMode = CODEC_AIO_DUPLEX_MODE_HALF;
	}
#endif
	volume = pAudioInfo->volume;
	if (volume == RECMOVIE_AUD_OFF) {
		sAinSetting.uiAinVolume = 0;
	} else if (volume <= RECMOVIE_AUD_VOL1) {
		sAinSetting.uiAinVolume = 1;
	} else if (volume <= RECMOVIE_AUD_VOL2) {
		sAinSetting.uiAinVolume = 2;
	} else if (volume <= RECMOVIE_AUD_VOL3) {
		sAinSetting.uiAinVolume = 3;
	} else if (volume <= RECMOVIE_AUD_VOL4) {
		sAinSetting.uiAinVolume = 4;
	} else if (volume <= RECMOVIE_AUD_VOL5) {
		sAinSetting.uiAinVolume = 5;
	} else if (volume <= RECMOVIE_AUD_VOL6) {
		sAinSetting.uiAinVolume = 6;
	} else if (volume <= RECMOVIE_AUD_VOL7) {
		sAinSetting.uiAinVolume = 7;
	} else if (volume <= RECMOVIE_AUD_ON) {
		sAinSetting.uiAinVolume = 8;
	} else {
		DBG_ERR("volume error (%d)\r\n", volume);
		sAinSetting.uiAinVolume = 8;
	}

	sAinSetting.sAinAudFiltSetting.bFiltEnable = ImageUnit_GetParam(&ISF_AudIn, ISF_CTRL, AUDIN_PARAM_AUDFILT_EN);

	NvtUctrl_SetConfigData((void *)&sAinSetting, sizeof(CodecAinSetting));
#endif
#endif
	return 0;
}

static INT32 param_aenc_set(INT32 argc, char *argv[])
{
#if(IPCAM_MODE == ENABLE)
#if(AUDIO_CMD_UCTRL_ENABLE == ENABLE)
	UINT8 aenc_channel = 0;
	CodecAinSetting sAinSetting = {0};

	if (argc != 1) {
		DBG_ERR("argc!=1 (%d)\r\n", argc);
		return -1;
	}

	if ((aenc_channel = atoi(argv[0])) >= MAXAUDIO_CH) {
		DBG_ERR("invalid aenc channel: %u (Max: %d)\r\n", aenc_channel, MAXAUDIO_CH-1);
		return -1;
	}

	NvtUctrl_GetConfigData((void *)&sAinSetting, sizeof(CodecAinSetting));

	UIAPPIPCAM_AUDIO_INFO *pAudioInfo = UIAppIPCam_get_AudioInfo();
	UIAPPIPCAM_AUDIO_INFO *pAudioOutInfo = UIAppIPCam_get_AudioOutInfo();
	switch (sAinSetting.eAinSampleRate) {
	case CODEC_AIO_SAMPLE_RATE_8000:
		pAudioInfo->SampleRate = 8000;
		break;
	case CODEC_AIO_SAMPLE_RATE_11025:
		pAudioInfo->SampleRate = 11025;
		break;
	case CODEC_AIO_SAMPLE_RATE_12000:
		pAudioInfo->SampleRate = 12000;
		break;
	case CODEC_AIO_SAMPLE_RATE_16000:
		pAudioInfo->SampleRate = 16000;
		break;
	case CODEC_AIO_SAMPLE_RATE_22050:
		pAudioInfo->SampleRate = 22050;
		break;
	case CODEC_AIO_SAMPLE_RATE_24000:
		pAudioInfo->SampleRate = 24000;
		break;
	case CODEC_AIO_SAMPLE_RATE_32000:
		pAudioInfo->SampleRate = 32000;
		break;
	case CODEC_AIO_SAMPLE_RATE_44100:
		pAudioInfo->SampleRate = 44100;
		break;
	case CODEC_AIO_SAMPLE_RATE_48000:
		pAudioInfo->SampleRate = 48000;
		break;
	default:
		DBG_ERR("audio sample rate error (%d)\r\n", sAinSetting.eAinSampleRate);
		break;
	}


	//#NT#2016/05/13#David Tsai -begin
	//#NT#Disable unsupported audio codec
	switch (sAinSetting.eAinFormat) {
	case CODEC_AENC_FORMAT_AAC:
		pAudioInfo->codec = AUDENC_ENCODER_AAC;
		break;
	case CODEC_AENC_FORMAT_PCM:
		pAudioInfo->codec = AUDENC_ENCODER_PCM;
		break;
	case CODEC_AENC_FORMAT_ALAW:
		pAudioInfo->codec = AUDENC_ENCODER_ALAW;
		break;
	case CODEC_AENC_FORMAT_ULAW:
		pAudioInfo->codec = AUDENC_ENCODER_ULAW;
		break;
	default:
		DBG_ERR("audio encode not support (%d)\r\n", sAinSetting.eAinFormat);
		break;

	}
	//#NT#2016/05/13#David Tsai -end
	//channel

	switch (sAinSetting.eAinSoundMode) {
	case CODEC_AIO_SOUND_MODE_MONO:
		pAudioInfo->channel = AUDIO_CH_MONO;
		break;
	case CODEC_AIO_SOUND_MODE_STEREO:
		pAudioInfo->channel = AUDIO_CH_STEREO;
		break;
	case CODEC_AIO_SOUND_MODE_LEFT:
		pAudioInfo->channel = AUDIO_CH_LEFT;
		break;
	case CODEC_AIO_SOUND_MODE_RIGHT:
		pAudioInfo->channel = AUDIO_CH_RIGHT;
		break;
	default:
		DBG_ERR("audio channel error (%d)\r\n", sAinSetting.eAinSoundMode);
		break;
	}

	//mode = 0:half-duplex or 1:full-duplex
#if 0
	switch (sAinSetting.eAinDuplexMode) {
	case CODEC_AIO_DUPLEX_MODE_HALF:
		Ux_SendEvent(0, NVTEVT_AUDIO_MODE, 1, 0);
		break;
	case CODEC_AIO_DUPLEX_MODE_FULL:
	default:
		Ux_SendEvent(0, NVTEVT_AUDIO_MODE, 1, 1);
		break;
	}
#endif
	//vol=0~8
	switch (sAinSetting.uiAinVolume) {
	case 0:
		pAudioInfo->volume = RECMOVIE_AUD_OFF;
		break;
	case 1:
		pAudioInfo->volume = RECMOVIE_AUD_VOL1;
		break;
	case 2:
		pAudioInfo->volume = RECMOVIE_AUD_VOL2;
		break;
	case 3:
		pAudioInfo->volume = RECMOVIE_AUD_VOL3;
		break;
	case 4:
		pAudioInfo->volume = RECMOVIE_AUD_VOL4;
		break;
	case 5:
		pAudioInfo->volume = RECMOVIE_AUD_VOL5;
		break;
	case 6:
		pAudioInfo->volume = RECMOVIE_AUD_VOL6;
		break;
	case 7:
		pAudioInfo->volume = RECMOVIE_AUD_VOL7;
		break;
	case 8:
		pAudioInfo->volume = RECMOVIE_AUD_ON;
		break;
	default:
		DBG_ERR("audio volume error (%d)\r\n", sAinSetting.uiAinVolume);
		break;
	}


	UINT32 path_index = 0;
	DBG_DUMP("audio enc vol:%d codec:%d sr:%d ch:%d\r\n",
			 pAudioInfo->volume, pAudioInfo->codec, pAudioInfo->SampleRate, pAudioInfo->channel);
	ImageApp_IPCam_audio_set_status(ISF_PORT_STATE_OFF);
	ImageUnit_Begin(&ISF_AudIn, 0);
	ImageUnit_SetParam(ISF_CTRL, AUDIN_PARAM_VOL_IMM, pAudioInfo->volume);
	ImageUnit_SetParam(ISF_CTRL, AUDIN_PARAM_ANR_EN, sAinSetting.sAinNRSetting.bNREnable);
	#if IPCAM_ANR_FUNC
	nvtanr_setconfig(NVTANR_CONFIG_ID_NRDB, sAinSetting.sAinNRSetting.iNRdB);
	nvtanr_setconfig(NVTANR_CONFIG_ID_HPF_FREQ, sAinSetting.sAinNRSetting.iHPFCutOffFreq);
	nvtanr_setconfig(NVTANR_CONFIG_ID_BIAS_SENSITIVE, sAinSetting.sAinNRSetting.iBiasSensitive);
	#endif
	#if IPCAM_AEC_FUNC
	NvtAec_SetConfig(NVTAEC_CONFIG_ID_SAMPLERATE, pAudioInfo->SampleRate);
	if(pAudioInfo->channel == AUDIO_CH_STEREO || pAudioInfo->channel == AUDIO_CH_DUAL_MONO){
		NvtAec_SetConfig(NVTAEC_CONFIG_ID_RECORD_CH_NO, 2);
	} else {
		NvtAec_SetConfig(NVTAEC_CONFIG_ID_RECORD_CH_NO, 1);
	}
	#endif
	ImageUnit_SetParam(ISF_IN1, AUDIN_PARAM_CHANNEL, pAudioInfo->channel);
	ImageUnit_SetParam(ISF_CTRL, AUDIN_PARAM_AUDFILT_EN, sAinSetting.sAinAudFiltSetting.bFiltEnable);
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
	ImageUnit_SetParam(ISF_IN1, AUDENC_PARAM_CODEC, pAudioInfo->codec);
	//ImageUnit_SetParam(ISF_IN1, AUDENC_PARAM_SAMPLERATE, pAudioInfo->SampleRate);
	//ImageUnit_SetParam(ISF_IN1, AUDENC_PARAM_CHS, pAudioInfo->channel);
	if(pAudioInfo->channel == AUDIO_CH_STEREO || pAudioInfo->channel == AUDIO_CH_DUAL_MONO){
		ImageUnit_SetAudSample(ISF_IN1,16,2,pAudioInfo->SampleRate);
	}
	else{
		ImageUnit_SetAudSample(ISF_IN1,16,1,pAudioInfo->SampleRate);
	}

	if (pAudioInfo->codec == AUDENC_ENCODER_PCM) {
		ImageUnit_SetParam((path_index + ISF_OUT_BASE), AUDENC_PARAM_PORT_OUTPUT_FMT, AUDENC_OUTPUT_UNCOMPRESSION);
	} else {
		ImageUnit_SetParam((path_index + ISF_OUT_BASE), AUDENC_PARAM_PORT_OUTPUT_FMT, AUDENC_OUTPUT_COMPRESSION);
	}

	ImageUnit_End();

	/////sample rate should be the same between ISF_StreamReceiver and ISF_AudEnc
	ImageUnit_Begin(&ISF_StreamReceiver, 0);
		//ImageUnit_SetParam(ISF_IN1, STMRCV_PARAM_SAMPLERATE, pAudioInfo->SampleRate);

		#if (ISF_TEST == 0)
		if(pAudioOutInfo->channel == AUDIO_CH_STEREO || pAudioOutInfo->channel == AUDIO_CH_DUAL_MONO){
			ImageUnit_SetAudSample(ISF_IN1,16,2,pAudioInfo->SampleRate);
		}
		else{
			ImageUnit_SetAudSample(ISF_IN1,16,1,pAudioInfo->SampleRate);
		}
		#else
		if(pAudioOutInfo->channel == AUDIO_CH_STEREO || pAudioOutInfo->channel == AUDIO_CH_DUAL_MONO){
			ImageUnit_SetAudSample(ISF_OUT1,16,2,pAudioInfo->SampleRate);
		}
		else{
			ImageUnit_SetAudSample(ISF_OUT1,16,1,pAudioInfo->SampleRate);
		}
		#endif

	ImageUnit_End();
	pAudioOutInfo->SampleRate = pAudioInfo->SampleRate;
	ImageApp_IPCam_audio_set_status(ISF_PORT_STATE_RUN);
	//ImageApp_IPCam_update_all_path();
#endif
#endif
	return 0;
}



extern DX_HANDLE gDevLCDObj;
extern DX_HANDLE gDevTVObj;
extern DX_HANDLE gDevHDMIObj;


static INT32 param_vout_get(INT32 argc, char *argv[])
{
#if(IPCAM_MODE == ENABLE)
	UINT8 vout_channel = 0;
	CodecVoutSetting sVoutSetting = {0};

	if (argc != 1) {
		DBG_ERR("argc!=1 (%d)\r\n", argc);
		return -1;
	}

	if ((vout_channel = atoi(argv[0])) > 1) {
		DBG_ERR("invalid vout channel: %u (Max: %d)\r\n", vout_channel, 1);
		return -1;
	}

	sVoutSetting.eVoutInterface = CODEC_VOUT_INTERFACE_MAX;
#if ((HDMI_FUNC == ENABLE) || (TV_FUNC == ENABLE))
	UIAPPIPCAM_VOUT_SETTING *pVoutInfo = UIAppIPCam_get_VoutInfo();
#endif
#if (HDMI_FUNC == ENABLE)
	if ((DX_HANDLE)GxVideo_GetDevice(DOUT1) == gDevHDMIObj) {
		DBG_DUMP("get HDMI device info\r\n");

		switch (pVoutInfo->sMode) {
		case UIAPPIPCAM_VOUT_AUTO:
			sVoutSetting.eVoutInterface = CODEC_VOUT_INTERFACE_AUTO;
			break;
		case UIAPPIPCAM_VOUT_FHDP30:
			sVoutSetting.eVoutInterface = CODEC_VOUT_INTERFACE_HDMI_1920x1080P30;
			break;
		case UIAPPIPCAM_VOUT_FHDI60:
			sVoutSetting.eVoutInterface = CODEC_VOUT_INTERFACE_HDMI_1920x1080I60;
			break;
		case UIAPPIPCAM_VOUT_UHDP30:
			sVoutSetting.eVoutInterface = CODEC_VOUT_INTERFACE_HDMI_3840x2160P30;
			break;
		case UIAPPIPCAM_VOUT_UHDP25:
			sVoutSetting.eVoutInterface = CODEC_VOUT_INTERFACE_HDMI_3840x2160P25;
			break;
		case UIAPPIPCAM_VOUT_UHDP24:
			sVoutSetting.eVoutInterface = CODEC_VOUT_INTERFACE_HDMI_3840x2160P24;
			break;
		case UIAPPIPCAM_VOUT_4K2KP30:
			sVoutSetting.eVoutInterface = CODEC_VOUT_INTERFACE_HDMI_4096x2160P30;
			break;
		case UIAPPIPCAM_VOUT_4K2KP25:
			sVoutSetting.eVoutInterface = CODEC_VOUT_INTERFACE_HDMI_4096x2160P25;
			break;
		case UIAPPIPCAM_VOUT_4K2KP24:
			sVoutSetting.eVoutInterface = CODEC_VOUT_INTERFACE_HDMI_4096x2160P24;
			break;
		//case VIDEOOUT2_HD:
		//  sVoutSetting.eVoutInterface = CODEC_VOUT_INTERFACE_HDMI_720x480P60;
		default:
			DBG_ERR("get HDMI mode error %d\r\n", pVoutInfo->sMode);
			sVoutSetting.eVoutInterface = CODEC_VOUT_INTERFACE_AUTO;
			break;
		}

	} else
#endif
#if (TV_FUNC == ENABLE)
	if ((DX_HANDLE)GxVideo_GetDevice(DOUT1) == gDevTVObj) {
		DBG_DUMP("get TV device info!!\r\n");
		if (pVoutInfo->sMode == UIAPPIPCAM_VOUT_CVBS_NTSC) {
			sVoutSetting.eVoutInterface = CODEC_VOUT_INTERFACE_CVBS_NTSC;
		} else if (pVoutInfo->sMode == UIAPPIPCAM_VOUT_CVBS_PAL) {
			sVoutSetting.eVoutInterface = CODEC_VOUT_INTERFACE_CVBS_PAL;
		}else if (pVoutInfo->sMode == UIAPPIPCAM_VOUT_AUTO) {
			sVoutSetting.eVoutInterface = CODEC_VOUT_INTERFACE_AUTO;
		}
		else {
			DBG_ERR("get TV mode error %d\r\n", pVoutInfo->sMode);
		}
	} else
#endif
	if ((DX_HANDLE)GxVideo_GetDevice(DOUT1) == gDevLCDObj) {
		sVoutSetting.eVoutInterface = CODEC_VOUT_INTERFACE_LCD;
	}
	if(sVoutSetting.eVoutInterface == CODEC_VOUT_INTERFACE_MAX) {
		DBG_WRN("No device info\r\n");
		return 0;
	}


#if(IPCAM_FUNC == ENABLE)
	sVoutSetting.iChannel = ImageApp_IPCam_GetConfig(IPCAM_DISP_PATH);
#endif

	NvtUctrl_SetConfigData((void *)&sVoutSetting, sizeof(CodecVoutSetting));
#else
	DBG_ERR("not support !!\r\n");
#endif
	return 0;
}


static INT32 param_vout_enable(INT32 argc, char *argv[]){

#if(IPCAM_FUNC == ENABLE)
	if (argc != 1) {
		DBG_ERR("argc!=1 (%d)\r\n", argc);
		return -1;
	}

	BOOL bEnable = atoi(argv[0]);
	UIAPPIPCAM_VOUT_SETTING *pVoutInfo = UIAppIPCam_get_VoutInfo();
	pVoutInfo->bEnable = bEnable;
	if(bEnable == TRUE){
		DBG_DUMP("^Yenable display\r\n");
		SysVideo_OnVideoInsert();
		////need change mode to show streaming
		//Ux_SendEvent(0,NVTEVT_SYSTEM_MODE,1,System_GetState(SYS_STATE_CURRMODE));
		//Ux_PostEvent(NVTEVT_SYSTEM_MODE, 1, System_GetState(SYS_STATE_CURRMODE));

	}
	else{
		DBG_DUMP("^Ydisable display\r\n");
		SysVideo_OnVideoRemove();
	}



#else
	DBG_ERR("not support!!\r\n");
#endif
    return 0;
}
static INT32 param_vout_set(INT32 argc, char *argv[])
{

#if(IPCAM_MODE == ENABLE)
	BOOL bChangeMode = FALSE;
	UINT8 vout_channel = 0;
	CodecVoutSetting sVoutSetting = {0};
	enum {TV_MODE, HDMI_MODE, LCD_MODE, UNKNOWN_MODE};
	UINT32 current_mode = UNKNOWN_MODE;
	if (argc != 1) {
		DBG_ERR("argc!=1 (%d)\r\n", argc);
		return -1;
	}

	if ((vout_channel = atoi(argv[0])) > 1) {
		DBG_ERR("invalid vout channel: %u (Max: %d)\r\n", vout_channel, 1);
		return -1;
	}
	NvtUctrl_GetConfigData((void *)&sVoutSetting, sizeof(CodecVoutSetting));
#if(IPCAM_FUNC == ENABLE)
	INT32 iDispPath = UIAppIPCam_get_DisplayPath();

	if(iDispPath != sVoutSetting.iChannel){
		bChangeMode = TRUE;
	}
	UIAppIPCam_set_DisplayPath(sVoutSetting.iChannel);
	//ImageApp_IPCam_Config(IPCAM_DISP_PATH, sVoutSetting.iChannel);
#endif

#if (HDMI_FUNC == ENABLE)

	if ((DX_HANDLE)GxVideo_GetDevice(DOUT1) == gDevHDMIObj) {
		current_mode = HDMI_MODE;
	} else
#endif
#if (TV_FUNC == ENABLE)
	if ((DX_HANDLE)GxVideo_GetDevice(DOUT1) == gDevTVObj) {
		current_mode = TV_MODE;
	} else
#endif
	if ((DX_HANDLE)GxVideo_GetDevice(DOUT1) == gDevLCDObj) {
		current_mode = LCD_MODE;

	}
	if(current_mode == UNKNOWN_MODE)
	{
		DBG_WRN("can not get device!\r\n");
		return -1;
	}

#if ((HDMI_FUNC == ENABLE) || (TV_FUNC == ENABLE))
	UIAPPIPCAM_VOUT_SETTING *pVoutInfo = UIAppIPCam_get_VoutInfo();
#endif
	switch (sVoutSetting.eVoutInterface) {
#if (TV_FUNC == ENABLE)
	case CODEC_VOUT_INTERFACE_CVBS_NTSC:
		pVoutInfo->sMode = UIAPPIPCAM_VOUT_CVBS_NTSC;
		GxVideo_ConfigDeviceFirstMode((UINT32)gDevTVObj, DISP_TVMODE_NTSC_SD);
		//UctrlMain_SendEvent(NVTEVT_VIDEO_MODE, 2, gDevTVObj, DISP_TVMODE_NTSC_SD);
		break;
	case CODEC_VOUT_INTERFACE_CVBS_PAL:
		pVoutInfo->sMode = UIAPPIPCAM_VOUT_CVBS_PAL;
		GxVideo_ConfigDeviceFirstMode((UINT32)gDevTVObj, DISP_TVMODE_PAL_SD);
		//UctrlMain_SendEvent(NVTEVT_VIDEO_MODE, 2, gDevTVObj, DISP_TVMODE_PAL_SD);

		break;
#endif
#if (HDMI_FUNC == ENABLE)
	case CODEC_VOUT_INTERFACE_HDMI_1920x1080I60:
		pVoutInfo->sMode = UIAPPIPCAM_VOUT_FHDI60;
		GxVideo_ConfigDeviceFirstMode((UINT32)gDevHDMIObj, DISP_HDMIMODE_1920X1080I60);
		//UctrlMain_SendEvent(NVTEVT_VIDEO_MODE, 2, gDevHDMIObj, DISP_HDMIMODE_1920X1080I60);

		break;
	case CODEC_VOUT_INTERFACE_HDMI_1920x1080P30:
		pVoutInfo->sMode = UIAPPIPCAM_VOUT_FHDP30;
		GxVideo_ConfigDeviceFirstMode((UINT32)gDevHDMIObj, DISP_HDMIMODE_1920X1080P30);
		//UctrlMain_SendEvent(NVTEVT_VIDEO_MODE, 2, gDevHDMIObj, DISP_HDMIMODE_1920X1080P30);

		break;
	case CODEC_VOUT_INTERFACE_HDMI_3840x2160P30:
		pVoutInfo->sMode = UIAPPIPCAM_VOUT_UHDP30;
		GxVideo_ConfigDeviceFirstMode((UINT32)gDevHDMIObj, DISP_HDMIMODE_3840X2160P30);
		//UctrlMain_SendEvent(NVTEVT_VIDEO_MODE, 2, gDevHDMIObj, DISP_HDMIMODE_3840X2160P30);

		break;
	case CODEC_VOUT_INTERFACE_HDMI_3840x2160P25:
		pVoutInfo->sMode = UIAPPIPCAM_VOUT_UHDP25;
		GxVideo_ConfigDeviceFirstMode((UINT32)gDevHDMIObj, DISP_HDMIMODE_3840X2160P25);
		//UctrlMain_SendEvent(NVTEVT_VIDEO_MODE, 2, gDevHDMIObj, DISP_HDMIMODE_3840X2160P25);

		break;
	case CODEC_VOUT_INTERFACE_HDMI_3840x2160P24:
		pVoutInfo->sMode = UIAPPIPCAM_VOUT_UHDP24;
		GxVideo_ConfigDeviceFirstMode((UINT32)gDevHDMIObj, DISP_HDMIMODE_3840X2160P24);
		//UctrlMain_SendEvent(NVTEVT_VIDEO_MODE, 2, gDevHDMIObj, DISP_HDMIMODE_3840X2160P24);

		break;
	case CODEC_VOUT_INTERFACE_HDMI_4096x2160P30:
		pVoutInfo->sMode = UIAPPIPCAM_VOUT_4K2KP30;
		GxVideo_ConfigDeviceFirstMode((UINT32)gDevHDMIObj, DISP_HDMIMODE_4096X2160P30);
		//UctrlMain_SendEvent(NVTEVT_VIDEO_MODE, 2, gDevHDMIObj, DISP_HDMIMODE_4096X2160P30);
		break;
	case CODEC_VOUT_INTERFACE_HDMI_4096x2160P25:
		pVoutInfo->sMode = UIAPPIPCAM_VOUT_4K2KP25;
		GxVideo_ConfigDeviceFirstMode((UINT32)gDevHDMIObj, DISP_HDMIMODE_4096X2160P25);
		//UctrlMain_SendEvent(NVTEVT_VIDEO_MODE, 2, gDevHDMIObj, DISP_HDMIMODE_4096X2160P25);
		break;
	case CODEC_VOUT_INTERFACE_HDMI_4096x2160P24:
		pVoutInfo->sMode = UIAPPIPCAM_VOUT_4K2KP24;
		GxVideo_ConfigDeviceFirstMode((UINT32)gDevHDMIObj, DISP_HDMIMODE_4096X2160P24);
		//UctrlMain_SendEvent(NVTEVT_VIDEO_MODE, 2, gDevHDMIObj, DISP_HDMIMODE_4096X2160P24);
		break;
	case CODEC_VOUT_INTERFACE_AUTO:
		pVoutInfo->sMode = UIAPPIPCAM_VOUT_AUTO;
		GxVideo_ConfigDeviceFirstMode((UINT32)gDevHDMIObj, DISP_HDMIMODE_MAX);
		//UctrlMain_SendEvent(NVTEVT_VIDEO_MODE, 2, gDevHDMIObj, DISP_HDMIMODE_MAX); //change to max resoluition
		break;
#endif
	default:
		break;
	}
#if(IPCAM_FUNC == ENABLE)
	SysVideo_OnVideoRemove();
	SysVideo_OnVideoInsert();
	//////display path will get resolution by itself
	ISIZE DeviceSize = {0, 0};
	UINT32 DevID = 0; ////IDE id
	DeviceSize = GxVideo_GetDeviceSize(DevID);
	UIAPPIPCAM_ENCODE_CONFIG *pIPCamConfig = UIAppIPCam_get_EncodeConfig();
	pIPCamConfig[iDispPath].resolution.w = DeviceSize.w;
	pIPCamConfig[iDispPath].resolution.h = DeviceSize.h;
	DBG_DUMP("set display path resolution %dx%d\r\n",
			 pIPCamConfig[iDispPath].resolution.w,
			 pIPCamConfig[iDispPath].resolution.h);

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

#else

	if ((sVoutSetting.eVoutInterface > CODEC_VOUT_INTERFACE_LCD) && current_mode != HDMI_MODE) {
		SxCmd_DoCommand("sysvideo disp hd");
	} else if ((sVoutSetting.eVoutInterface < CODEC_VOUT_INTERFACE_LCD) && current_mode != TV_MODE) {
		SxCmd_DoCommand("sysvideo disp tv");
	} else if((sVoutSetting.eVoutInterface == CODEC_VOUT_INTERFACE_LCD) && current_mode != LCD_MODE){
		SxCmd_DoCommand("sysvideo disp lcd");
	}
#endif
#if 0
	if(sVoutSetting.bEnable == TRUE){
		UIAppIPCam_Set_Unit_Status(UIAPPIPCAM_VDOOUT_UNIT,ISF_PORT_STATE_RUN);
		//ImageApp_IPCam_VideoOut_set_status(sVoutSetting.iChannel,ISF_PORT_STATE_RUN);
	}
	else{
		UIAppIPCam_Set_Unit_Status(UIAPPIPCAM_VDOOUT_UNIT,ISF_PORT_STATE_READY);
		//ImageApp_IPCam_VideoOut_set_status(sVoutSetting.iChannel,ISF_PORT_STATE_READY);
	}
#endif
	if(bChangeMode == TRUE){
		DBG_DUMP("^G Chnage mode!!\r\n");
		Ux_SendEvent(0,NVTEVT_SYSTEM_MODE,1,System_GetState(SYS_STATE_CURRMODE));
	}
	else{
		#if(IPCAM_FUNC == ENABLE)
			ImageApp_IPCam_reset_path(iDispPath);
		#endif
	}
	//Ux_PostEvent(NVTEVT_SYSTEM_MODE, 1, System_GetState(SYS_STATE_CURRMODE));
#else
	DBG_ERR("not support !!\r\n");
#endif

	return 0;
}

static INT32 param_aout_get(INT32 argc, char *argv[])
{
#if(IPCAM_MODE == ENABLE)
#if(AUDIO_CMD_UCTRL_ENABLE == ENABLE)

	CodecAoutSetting sAoutSetting = {0};
	//#NT#2016/09/08#HM Tseng -begin
	//#NT#Support audio channel
	UINT32 aout_audch = 0;
	//#NT#2016/09/08#HM Tseng -end

	if (argc != 1) {
		DBG_ERR("argc!=1 (%d)\r\n", argc);
		return -1;
	}



	UIAPPIPCAM_AUDIO_INFO *pAudioOutInfo = UIAppIPCam_get_AudioOutInfo();

#if 1
	//now for  speaker
	sAoutSetting.eAoutTarget = CODEC_AOUT_TARGET_SPEAKER;
#else
	sAoutSetting.eAoutTarget = System_GetAudioOut() == TRUE ? CODEC_AOUT_TARGET_LINEOUT : CODEC_AOUT_TARGET_SPEAKER;
#endif
	sAoutSetting.uiAoutVolume = (pAudioOutInfo->volume * 64) / 100;
	//#NT#2016/09/08#HM Tseng -begin
	//#NT#Support audio channel
	aout_audch = pAudioOutInfo->channel;

	switch (aout_audch) {
	case AUDIO_CH_MONO:
		sAoutSetting.eAoutSoundMode = CODEC_AIO_SOUND_MODE_MONO;
		break;
	case AUDIO_CH_STEREO:
		sAoutSetting.eAoutSoundMode = CODEC_AIO_SOUND_MODE_STEREO;
		break;
	case AUDIO_CH_LEFT:
		sAoutSetting.eAoutSoundMode = CODEC_AIO_SOUND_MODE_LEFT;
		break;
	case AUDIO_CH_RIGHT:
		sAoutSetting.eAoutSoundMode = CODEC_AIO_SOUND_MODE_RIGHT;
		break;
	default:
		sAoutSetting.eAoutSoundMode = CODEC_AIO_SOUND_MODE_LEFT;
		break;
	}
	//#NT#2016/09/08#HM Tseng -end

	//#NT#2016/12/22#HM Tseng -begin
	//#NT# support AEC eanble

	sAoutSetting.sAoutAECSetting.bAECEnable = ImageUnit_GetParam(&ISF_AudIn, ISF_CTRL, AUDIN_PARAM_AEC_EN);
	//#NT#2016/12/22#HM Tseng -end

	#if IPCAM_AEC_FUNC
	sAoutSetting.sAoutAECSetting.bLeakEstEnable 	= NvtAec_GetConfig(NVTAEC_CONFIG_ID_LEAK_ESTIMATE_EN);
	sAoutSetting.sAoutAECSetting.iEchoCancelLevel	= NvtAec_GetConfig(NVTAEC_CONFIG_ID_ECHO_CANCEL_LVL);
	sAoutSetting.sAoutAECSetting.iLeakEstValue   	= NvtAec_GetConfig(NVTAEC_CONFIG_ID_LEAK_ESTIMATE_VAL);
	sAoutSetting.sAoutAECSetting.iNoiseCancelLevel	= NvtAec_GetConfig(NVTAEC_CONFIG_ID_NOISE_CANCEL_LVL);
	sAoutSetting.sAoutAECSetting.uiFilterLen		= NvtAec_GetConfig(NVTAEC_CONFIG_ID_FILTER_LEN);
	sAoutSetting.sAoutAECSetting.uiFrameSize		= NvtAec_GetConfig(NVTAEC_CONFIG_ID_FRAME_SIZE);
	sAoutSetting.sAoutAECSetting.uiNotchRadius		= NvtAec_GetConfig(NVTAEC_CONFIG_ID_NOTCH_RADIUS);
	#endif

	NvtUctrl_SetConfigData((void *)&sAoutSetting, sizeof(CodecAoutSetting));
#endif
#endif
	return 0;
}

static INT32 param_aout_set(INT32 argc, char *argv[])
{
#if(IPCAM_MODE == ENABLE)
#if(AUDIO_CMD_UCTRL_ENABLE == ENABLE)
	UINT8 aout_channel = 0;
	CodecAoutSetting sAoutSetting = {0};
	//#NT#2016/09/08#HM Tseng -begin
	//#NT#Support audio channel
	// UINT32 aout_audch = 0;
	//#NT#2016/09/08#HM Tseng -end

	UIAPPIPCAM_AUDIO_INFO *pAudioOutInfo = UIAppIPCam_get_AudioOutInfo();

	if (argc != 1) {
		DBG_ERR("argc!=1 (%d)\r\n", argc);
		return -1;
	}

	aout_channel = atoi(argv[0]);
	DBG_DUMP("aout ch:%d\r\n",aout_channel);

	NvtUctrl_GetConfigData((void *)&sAoutSetting, sizeof(CodecAoutSetting));
#if 1
	switch (sAoutSetting.eAoutTarget) {
	case CODEC_AOUT_TARGET_LINEOUT:
		ImageUnit_Begin(&ISF_AudOut, 0);
		ImageUnit_SetParam(ISF_CTRL, AUDOUT_PARAM_OUT_DEV, 1);
		ImageUnit_End();
		break;
	case CODEC_AOUT_TARGET_SPEAKER:
	default:
		ImageUnit_Begin(&ISF_AudOut, 0);
		ImageUnit_SetParam(ISF_CTRL, AUDOUT_PARAM_OUT_DEV, 0);
		ImageUnit_End();
		break;
	}
#endif
	//#NT#2016/09/08#HM Tseng -begin
	//#NT#Support audio channel
	switch (sAoutSetting.eAoutSoundMode) {
	case CODEC_AIO_SOUND_MODE_MONO:
		pAudioOutInfo->channel = AUDIO_CH_MONO;
		break;
	case CODEC_AIO_SOUND_MODE_STEREO:
		pAudioOutInfo->channel = AUDIO_CH_STEREO;
		break;
	case CODEC_AIO_SOUND_MODE_LEFT:
		pAudioOutInfo->channel = AUDIO_CH_LEFT;
		break;
	case CODEC_AIO_SOUND_MODE_RIGHT:
		pAudioOutInfo->channel = AUDIO_CH_RIGHT;
		break;
	default:
		DBG_ERR("audio codec error(%d)\r\n", sAoutSetting.eAoutSoundMode);
		break;
	}


	pAudioOutInfo->volume = (sAoutSetting.uiAoutVolume * 100) / 64;
	pAudioOutInfo->SampleRate = sAoutSetting.eAoutSampleRate;


	switch(sAoutSetting.eAoutFormat){
		case CODEC_AENC_FORMAT_PCM:
			pAudioOutInfo->codec = MOVAUDENC_PCM;
			break;
		case CODEC_AENC_FORMAT_AAC:
			pAudioOutInfo->codec = MOVAUDENC_AAC;
			break;
		case CODEC_AENC_FORMAT_ULAW:
			pAudioOutInfo->codec = MOVAUDENC_ULAW;
			break;
		case CODEC_AENC_FORMAT_ALAW:
			pAudioOutInfo->codec = MOVAUDENC_ALAW;
			break;
		default:
			DBG_ERR("error eAoutFormat:%d\r\n",sAoutSetting.eAoutFormat);
			return -1;

	}


	ImageApp_IPCam_audio_set_status(ISF_PORT_STATE_OFF);

	ImageUnit_Begin(&ISF_StreamReceiver, 0);
		ImageUnit_SetParam(ISF_IN1, STMRCV_PARAM_CODEC, pAudioOutInfo->codec);
		if(pAudioOutInfo->channel == AUDIO_CH_STEREO || pAudioOutInfo->channel == AUDIO_CH_DUAL_MONO){
			ImageUnit_SetParam(ISF_IN1, STMRCV_PARAM_CHS, 2);
		} else {
			ImageUnit_SetParam(ISF_IN1, STMRCV_PARAM_CHS, 1);
		}
	//ImageUnit_SetParam(ISF_CTRL, ISF_STREAMRECEIVER_PARAM_SAMPLERATE, pAudioOutInfo->sample_rate);
	ImageUnit_End();

	ImageUnit_Begin(&ISF_AudOut, 0);
		ImageUnit_SetParam(ISF_CTRL, AUDOUT_PARAM_VOL_IMM, pAudioOutInfo->volume);
		ImageUnit_SetParam(ISF_OUT1, AUDOUT_PARAM_CHANNEL, pAudioOutInfo->channel);
	ImageUnit_End();

	ImageUnit_Begin(&ISF_AudIn, 0);
	ImageUnit_SetParam(ISF_CTRL, AUDIN_PARAM_AEC_EN, sAoutSetting.sAoutAECSetting.bAECEnable);
	ImageUnit_SetParam(ISF_CTRL, AUDIN_PARAM_AEC_PLAY_CHANNEL, pAudioOutInfo->channel);
	ImageUnit_End();
	///set AEC config
	#if IPCAM_AEC_FUNC
	NvtAec_SetConfig(NVTAEC_CONFIG_ID_LEAK_ESTIMATE_EN	,(INT32)sAoutSetting.sAoutAECSetting.bLeakEstEnable);
	NvtAec_SetConfig(NVTAEC_CONFIG_ID_LEAK_ESTIMATE_VAL	,(INT32)sAoutSetting.sAoutAECSetting.iLeakEstValue);
	NvtAec_SetConfig(NVTAEC_CONFIG_ID_NOISE_CANCEL_LVL	,(INT32)sAoutSetting.sAoutAECSetting.iNoiseCancelLevel);
	NvtAec_SetConfig(NVTAEC_CONFIG_ID_ECHO_CANCEL_LVL	,(INT32)sAoutSetting.sAoutAECSetting.iEchoCancelLevel);
	NvtAec_SetConfig(NVTAEC_CONFIG_ID_FILTER_LEN	,(INT32)sAoutSetting.sAoutAECSetting.uiFilterLen);
	NvtAec_SetConfig(NVTAEC_CONFIG_ID_FRAME_SIZE	,(INT32)sAoutSetting.sAoutAECSetting.uiFrameSize);
	NvtAec_SetConfig(NVTAEC_CONFIG_ID_NOTCH_RADIUS	,(INT32)sAoutSetting.sAoutAECSetting.uiNotchRadius);
	if(pAudioOutInfo->channel == AUDIO_CH_STEREO || pAudioOutInfo->channel == AUDIO_CH_DUAL_MONO){
		NvtAec_SetConfig(NVTAEC_CONFIG_ID_PLAYBACK_CH_NO, 2);
	} else {
		NvtAec_SetConfig(NVTAEC_CONFIG_ID_PLAYBACK_CH_NO, 1);
	}
	#endif

	//ImageApp_IPCam_update_all_path();
	ImageApp_IPCam_audio_set_status(ISF_PORT_STATE_RUN);
#endif
#endif
	return 0;
}

static INT32 param_aout_switch(INT32 argc, char *argv[])
{
#if(AUDIO_CMD_UCTRL_ENABLE == ENABLE)
	UINT8 aout_channel = 0;

	if (argc != 2) {
		DBG_ERR("argc!=2 (%d)\r\n", argc);
		return -1;
	}

	if ((aout_channel = atoi(argv[0])) > 1) {
		DBG_ERR("invalid aout channel: %u (Max: %d)\r\n", aout_channel, 1);
		return -1;
	}

	/*
	    //mode = 0:disable or 1:enable
	    if (atoi(argv[1]) == 0)
	        Ux_SendEvent(0, NVTEVT_AUDIO_INPUT, 1, 0);
	    else
	        Ux_SendEvent(0, NVTEVT_AUDIO_INPUT, 1, 1);
	*/
#endif
	return 0;
}

static INT32 param_aout_status(INT32 argc, char *argv[])
{
#if(AUDIO_CMD_UCTRL_ENABLE == ENABLE)
	UINT8 aout_channel = 0;
	char retStr[32];

	if (argc != 1) {
		DBG_ERR("argc!=1 (%d)\r\n", argc);
		return -1;
	}

	if ((aout_channel = atoi(argv[0])) > 1) {
		DBG_ERR("invalid aout channel: %u (Max: %d)\r\n", aout_channel, 1);
		return -1;
	}

	snprintf(retStr, sizeof(retStr), "%lu", System_GetAudioInput() == TRUE ? 1 : 0);
	NvtUctrl_SetRetString(retStr);
#endif
	return 0;
}
#if 0
static INT32 param_hdr_get(INT32 argc, char *argv[])
{
	UINT8 ipl_id;


	if (argc != 1) {
		DBG_ERR("argc!=1 (%d)\r\n", argc);
		return -1;
	}

	if ((ipl_id = atoi(argv[0])) >= IPL_ID_MAX_NUM) {
		DBG_ERR("invalid IPL ID: %u (Max: %d)\r\n", ipl_id, IPL_ID_MAX_NUM);
		return -1;
	}
#if(MOVIE_MODE==ENABLE)
	ISPHDRSetting sHDRSetting = {0};
	switch (UI_GetData(FL_MOVIE_HDR_MENU)) {
	case MOVIE_HDR_ON:
		sHDRSetting.bHDREnable = NVT_TRUE;
		break;
	case MOVIE_HDR_OFF:
	default:
		sHDRSetting.bHDREnable = NVT_FALSE;
		break;
	}

	NvtUctrl_SetConfigData((void *)&sHDRSetting, sizeof(ISPHDRSetting));
#endif
	return 0;
}

static INT32 param_hdr_set(INT32 argc, char *argv[])
{
	UINT8 ipl_id;
	ISPHDRSetting sHDRSetting = {0};

	if (argc != 1) {
		DBG_ERR("argc!=1 (%d)\r\n", argc);
		return -1;
	}

	if ((ipl_id = atoi(argv[0])) >= IPL_ID_MAX_NUM) {
		DBG_ERR("invalid IPL ID: %u (Max: %d)\r\n", ipl_id, IPL_ID_MAX_NUM);
		return -1;
	}

	NvtUctrl_GetConfigData((void *)&sHDRSetting, sizeof(ISPHDRSetting));

	if (sHDRSetting.bHDREnable == NVT_TRUE) {
		pMovieInfo->hdr.index = MOVIE_HDR_ON;
	} else {
		pMovieInfo->hdr.index = MOVIE_HDR_OFF;
	}

	pMovieInfo->hdr.isSet = 1;

	return 0;
}
#endif

static INT32 param_cycrec(INT32 argc, char *argv[])
{
	UINT32 value;
	if (argc != 1) {
		DBG_ERR("argc!=1 (%d)\r\n", argc);
		return -1;
	}
	DBG_IND("get: %d\r\n", atoi(argv[0]));
	value = atoi(argv[0]);
	if (value >= MOVIE_CYCLICREC_ID_MAX) {
		DBG_ERR("value =%d\r\n", value);
		return -1;
	}

	pMovieInfo->cycrec.index = value;
	pMovieInfo->cycrec.isSet = 1;

	return 0;
}

static INT32 param_wdr(INT32 argc, char *argv[])
{
	UINT32 value;
	if (argc != 1) {
		DBG_ERR("argc!=1 (%d)\r\n", argc);
		return -1;
	}
	DBG_IND("get: %d\r\n", atoi(argv[0]));
	value = atoi(argv[0]);
	if (value > ON) {
		DBG_ERR("value =%d\r\n", value);
		return -1;
	}

	if (value == ON) {
		pMovieInfo->wdr.index = MOVIE_WDR_ON;
	} else {
		pMovieInfo->wdr.index = MOVIE_WDR_OFF;
	}

	pMovieInfo->wdr.isSet = 1;

	return 0;
}


static INT32 param_hdr(INT32 argc, char *argv[])
{
	UINT32 value;
	if (argc != 1) {
		DBG_ERR("argc!=1 (%d)\r\n", argc);
		return -1;
	}
	DBG_IND("get: %d\r\n", atoi(argv[0]));
	value = atoi(argv[0]);
	if (value > ON) {
		DBG_ERR("value =%d\r\n", value);
		return -1;
	}

	if (value == ON) {
		pMovieInfo->hdr.index = MOVIE_HDR_ON;
	} else {
		pMovieInfo->hdr.index = MOVIE_HDR_OFF;
	}

	pMovieInfo->hdr.isSet = 1;

	return 0;
}

static INT32 param_ev(INT32 argc, char *argv[])
{
	UINT32 value;
	if (argc != 1) {
		DBG_ERR("argc!=1 (%d)\r\n", argc);
		return -1;
	}
	DBG_IND("get: %d\r\n", atoi(argv[0]));
	value = atoi(argv[0]);
	if (value >= EV_SETTING_MAX) {
		DBG_ERR("value =%d\r\n", value);
		return -1;
	}

	pMovieInfo->ev.index = value;
	pMovieInfo->ev.isSet = 1;

	return 0;
}

static INT32 param_motion(INT32 argc, char *argv[])
{
	UINT32 value;
	if (argc != 1) {
		DBG_ERR("argc!=1 (%d)\r\n", argc);
		return -1;
	}
	DBG_IND("get: %d\r\n", atoi(argv[0]));
	value = atoi(argv[0]);
	if (value > ON) {
		DBG_ERR("value =%d\r\n", value);
		return -1;
	}

	if (value == ON) {
		pMovieInfo->motion.index = MOVIE_MOTIONDET_ON;
	} else {
		pMovieInfo->motion.index = MOVIE_MOTIONDET_OFF;
	}

	pMovieInfo->motion.isSet = 1;

	return 0;
}

static INT32 param_audio(INT32 argc, char *argv[])
{
#if(AUDIO_CMD_UCTRL_ENABLE == ENABLE)
	UINT32 value;
	if (argc != 1) {
		DBG_ERR("argc!=1 (%d)\r\n", argc);
		return -1;
	}
	DBG_IND("get: %d\r\n", atoi(argv[0]));
	value = atoi(argv[0]);
	if (value > ON) {
		DBG_ERR("value =%d\r\n", value);
		return -1;
	}

	if (value == ON) {
		pMovieInfo->audio.index = MOVIE_AUDIO_ON;
	} else {
		pMovieInfo->audio.index = MOVIE_AUDIO_OFF;
	}

	pMovieInfo->audio.isSet = 1;
#endif
	return 0;
}

static INT32 param_dstamp(INT32 argc, char *argv[])
{
	UINT32 value;
	if (argc != 1) {
		DBG_ERR("argc!=1 (%d)\r\n", argc);
		return -1;
	}
	DBG_IND("get: %d\r\n", atoi(argv[0]));
	value = atoi(argv[0]);
	if (value > ON) {
		DBG_ERR("value =%d\r\n", value);
		return -1;
	}

	if (value == ON) {
		pMovieInfo->dstamp.index = MOVIE_DATEIMPRINT_ON;
	} else {
		pMovieInfo->dstamp.index = MOVIE_DATEIMPRINT_OFF;
	}

	pMovieInfo->dstamp.isSet = 1;

	return 0;
}

static INT32 param_rsc(INT32 argc, char *argv[])
{
	#if 0
	UINT32 value;
	if (argc != 1) {
		DBG_ERR("argc!=1 (%d)\r\n", argc);
		return -1;
	}
	DBG_IND("get: %d\r\n", atoi(argv[0]));
	value = atoi(argv[0]);
	if (value > ON) {
		DBG_ERR("value =%d\r\n", value);
		return -1;
	}

	if (value == ON) {
		pMovieInfo->rsc.index = MOVIE_RSC_ON;
	} else {
		pMovieInfo->rsc.index = MOVIE_RSC_OFF;
	}

	pMovieInfo->rsc.isSet = 1;
	#else
		DBG_ERR("not support now!\r\n");
	#endif
	return 0;
}

static INT32 param_gsensor(INT32 argc, char *argv[])
{
	UINT32 value;
	if (argc != 1) {
		DBG_ERR("argc!=1 (%d)\r\n", argc);
		return -1;
	}
	DBG_IND("get: %d\r\n", atoi(argv[0]));
	value = atoi(argv[0]);
	if (value >= GSENSOR_ID_MAX) {
		DBG_ERR("value =%d\r\n", value);
		return -1;
	}

	pMovieInfo->gsensor.index = value;
	pMovieInfo->gsensor.isSet = 1;

	return 0;
}

static INT32 param_timelap(INT32 argc, char *argv[])
{
	UINT32 value;
	if (argc != 1) {
		DBG_ERR("argc!=1 (%d)\r\n", argc);
		return -1;
	}
	DBG_IND("get: %d\r\n", atoi(argv[0]));
	value = atoi(argv[0]);
	if (value >= MOVIE_TIMELAPSEREC_ID_MAX) {
		DBG_ERR("value =%d\r\n", value);
		return -1;
	}

	pMovieInfo->timelap.index = value;
	pMovieInfo->timelap.isSet = 1;

	return 0;
}

static INT32 param_ircut(INT32 argc, char *argv[])
{
	UINT32 value;
	if (argc != 1) {
		DBG_ERR("argc!=1 (%d)\r\n", argc);
		return -1;
	}
	DBG_IND("get: %d\r\n", atoi(argv[0]));
	value = atoi(argv[0]);
	if (value > ON) {
		DBG_ERR("value =%d\r\n", value);
		return -1;
	}

	if (value == ON) {
		pMovieInfo->ircut.index = MOVIE_IR_CUT_ON;
	} else {
		pMovieInfo->ircut.index = MOVIE_IR_CUT_OFF;
	}

	pMovieInfo->ircut.isSet = 1;

	return 0;
}
static INT32 param_rotateget(INT32 argc, char *argv[])
{
	#if (IPCAM_FUNC == ENABLE)
	char retStr[32]={0};

	UIAPPIPCAM_ENCODE_CONFIG *pIPCamConfig = UIAppIPCam_get_EncodeConfig();

	snprintf(retStr, sizeof(retStr), "%d", pIPCamConfig->eRotateType);
	NvtUctrl_SetRetString(retStr);
	#else
		DBG_ERR("not support!!\r\n");
	#endif
	return 0;

}
static INT32 param_rotateset(INT32 argc, char *argv[])
{
	#if (IPCAM_FUNC == ENABLE)

		if (argc != 2) {
			DBG_ERR("argc!=1 (%d)\r\n", argc);
			return -1;
		}
		UIAPPIPCAM_ENCODE_CONFIG *pIPCamConfig = UIAppIPCam_get_EncodeConfig();
		UIAPPIPCAM_RT_TYPE uiRotateType = atoi(argv[0]);
		UINT32 uiPath = atoi(argv[1]);
		UINT32 OutPortIndex = uiPath + ISF_OUT_BASE;
		if(uiRotateType == UIAPPIPCAM_RT_BUILDIN_90){
			ImageUnit_Begin(&ISF_VdoEnc, 0);
				ImageUnit_SetVdoDirection(ISF_IN_BASE+uiPath, ISF_VDO_DIR_ROTATE_90);
			ImageUnit_End();
		}
		else if(uiRotateType == UIAPPIPCAM_RT_BUILDIN_270){
			ImageUnit_Begin(&ISF_VdoEnc, 0);
				ImageUnit_SetVdoDirection(ISF_IN_BASE+uiPath, ISF_VDO_DIR_ROTATE_270);
			ImageUnit_End();
		}
		else if(uiRotateType == UIAPPIPCAM_RT_NONE){
			ImageUnit_Begin(&ISF_VdoEnc, 0);
				ImageUnit_SetVdoDirection(ISF_IN_BASE+uiPath, ISF_VDO_DIR_ROTATE_0);
			ImageUnit_End();
			ImageUnit_Begin(&ISF_UserProc, 0);
				ImageUnit_SetVdoDirection(ISF_IN_BASE+uiPath, ISF_VDO_DIR_NONE);
			ImageUnit_End();
		}
		else if(uiRotateType == UIAPPIPCAM_RT_EXTEND_90){
			ImageUnit_Begin(&ISF_UserProc, 0);
				ImageUnit_SetVdoDirection(ISF_IN_BASE+uiPath, ISF_VDO_DIR_ROTATE_90);
			ImageUnit_End();
			ImageUnit_Begin(&ISF_VdoEnc, 0);
				ImageUnit_SetVdoImgSize(ISF_IN_BASE+uiPath,pIPCamConfig[OutPortIndex].resolution.h,pIPCamConfig[OutPortIndex].resolution.w);
			ImageUnit_End();
		}
		else if(uiRotateType == UIAPPIPCAM_RT_EXTEND_180){
			ImageUnit_Begin(&ISF_UserProc, 0);
				ImageUnit_SetVdoDirection(ISF_IN_BASE+uiPath, ISF_VDO_DIR_ROTATE_180);
			ImageUnit_End();
			ImageUnit_Begin(&ISF_VdoEnc, 0);
				ImageUnit_SetVdoImgSize(ISF_IN_BASE+uiPath,pIPCamConfig[OutPortIndex].resolution.w,pIPCamConfig[OutPortIndex].resolution.h);
			ImageUnit_End();
		}
		else if(uiRotateType == UIAPPIPCAM_RT_EXTEND_270){
			ImageUnit_Begin(&ISF_UserProc, 0);
				ImageUnit_SetVdoDirection(ISF_IN_BASE+uiPath, ISF_VDO_DIR_ROTATE_270);
			ImageUnit_End();
			ImageUnit_Begin(&ISF_VdoEnc, 0);
				ImageUnit_SetVdoImgSize(ISF_IN_BASE+uiPath,pIPCamConfig[OutPortIndex].resolution.h,pIPCamConfig[OutPortIndex].resolution.w);
			ImageUnit_End();
		}
		pMovieInfo->rotate.isSet = 1;
		pMovieInfo->rotate.index = uiRotateType;
	#else
		DBG_ERR("not support!!\r\n");
	#endif
	return 0;
}

static INT32 param_livestreamqrcode(INT32 argc, char *argv[])
{
    CHAR szRetQrCode[3 * 128] = { 0 };
    UINT32 i = 0;

	NvtUctrl_GetConfigData((void *)&szRetQrCode, sizeof(szRetQrCode));

    //DBG_IND("QR code:%s\r\n", szRetQrCode);

    char* split_request;
    split_request = strtok(szRetQrCode, "\n");
    while(split_request != NULL)
    {
       switch(i)
       {
            default:
            case 0:
                DBG_DUMP("SSID:");
                break;
            case 1:
                DBG_DUMP("PASSWD:");
                break;
            case 2:
                DBG_DUMP("RTMP URL:");
                break;
       }
       DBG_DUMP("%s\r\n",split_request);  // SSID, PASSWD or RTMP URL.
       split_request = strtok(NULL,"\n");

       i++;
    }

	return 0;

}

static INT32 param_murgpro(INT32 argc, char *argv[])
{
	UINT32 value;
	if (argc != 1) {
		DBG_ERR("argc!=1 (%d)\r\n", argc);
		return -1;
	}
	DBG_IND("get: %d\r\n", atoi(argv[0]));
	value = atoi(argv[0]);
	if (value > ON) {
		DBG_ERR("value =%d\r\n", value);
		return -1;
	}

	if (value == ON) {
		pMovieInfo->murgpro.index = MOVIE_URGENT_PROTECT_MANUAL_ON;
	} else {
		pMovieInfo->murgpro.index = MOVIE_URGENT_PROTECT_MANUAL_OFF;
	}

	pMovieInfo->murgpro.isSet = 1;

	return 0;
}

static INT32 param_aurgpro(INT32 argc, char *argv[])
{
	UINT32 value;
	if (argc != 1) {
		DBG_ERR("argc!=1 (%d)\r\n", argc);
		return -1;
	}
	DBG_IND("get: %d\r\n", atoi(argv[0]));
	value = atoi(argv[0]);
	if (value > ON) {
		DBG_ERR("value =%d\r\n", value);
		return -1;
	}

	if (value == ON) {
		pMovieInfo->aurgpro.index = MOVIE_URGENT_PROTECT_AUTO_ON;
	} else {
		pMovieInfo->aurgpro.index = MOVIE_URGENT_PROTECT_AUTO_OFF;
	}

	pMovieInfo->aurgpro.isSet = 1;

	return 0;
}

static INT32 param_pim(INT32 argc, char *argv[])
{
	UINT32 value;
	if (argc != 1) {
		DBG_ERR("argc!=1 (%d)\r\n", argc);
		return -1;
	}
	DBG_IND("get: %d\r\n", atoi(argv[0]));
	value = atoi(argv[0]);
	if (value > ON) {
		DBG_ERR("value =%d\r\n", value);
		return -1;
	}

	if (value == ON) {
		pMovieInfo->pim.index = MOVIE_PIM_ON;
	} else {
		pMovieInfo->pim.index = MOVIE_PIM_OFF;
	}

	pMovieInfo->pim.isSet = 1;

	return 0;
}

static INT32 param_dualcam(INT32 argc, char *argv[])
{
	UINT32 value;
	if (argc != 1) {
		DBG_ERR("argc!=1 (%d)\r\n", argc);
		return -1;
	}
	DBG_IND("get: %d\r\n", atoi(argv[0]));
	value = atoi(argv[0]);
	if (value >= DUALCAM_SETTING_MAX) {
		DBG_ERR("value =%d\r\n", value);
		return -1;
	}

	pMovieInfo->dualcam.index = value;
	pMovieInfo->dualcam.isSet = 1;

	return 0;
}

static INT32 param_start(INT32 argc, char *argv[])
{
	pMovieInfo->start.isSet = 1;
	return 0;
}

static INT32 param_stop(INT32 argc, char *argv[])
{
	pMovieInfo->stop.isSet = 1;
	return 0;
}

static INT32 param_snapshot(INT32 argc, char *argv[])
{

	if(argc != 3){
		DBG_ERR("argc!=3 (%d)\r\n", argc);
		return -1;

	}


#if(IPCAM_FUNC == ENABLE)
	UINT32 channelID;
	channelID = atoi(argv[0]);
	ISIZE ScaleInfo ={0};
	ScaleInfo.w = atoi(argv[1]);
	ScaleInfo.h = atoi(argv[2]);
	char retStr[64]={0};
	MEM_RANGE snapShotData;
	UIAPPIPCAM_SENSOR_INFO *pSensorInfo = UIAppIPCam_get_SensorInfo();
	IPCAM_STREAM_INFO *pStreamInfo = UIAppIPCam_get_streamInfo();
	UINT32 uiSensorIndex = pStreamInfo[channelID].sensor;
	BOOL bCompress = NVT_FALSE;
	if(pSensorInfo[uiSensorIndex].bIME1Compress == NVT_TRUE && pStreamInfo[channelID].ime == IPCAM_IME1  ){
		bCompress = NVT_TRUE;
	}

	snapShotData.Size = ImageApp_IPCam_Snapshot(channelID, &snapShotData.Addr, ScaleInfo, bCompress, uiSensorIndex);
    if(snapShotData.Size <=0){
	    DBG_ERR("get snapshot error snapshotInfo.Size%d\r\n",snapShotData.Size);
		return -1;
	}

	snprintf(retStr, sizeof(retStr), "%lu %lu", NvtIPC_GetPhyAddr((UINT32)snapShotData.Addr), snapShotData.Size);
    DBG_IND("reStr= %s\r\n",retStr);
    NvtUctrl_SetRetString(retStr);


#else

	DBG_ERR("not support\r\n");
#endif
	return 0;
}


static INT32 param_fdstart(INT32 argc, char *argv[])
{
#if IPCAM_FD_FUNC
	UINT32 value;
	if (argc != 1) {
		DBG_ERR("argc!=1 (%d)\r\n", argc);
		return -1;
	}
	DBG_IND("get: %d\r\n", atoi(argv[0]));
	value = atoi(argv[0]);
	if (value == 0) {
		FD_Lock(TRUE);
	} else {
		FD_Lock(FALSE);
	}
	return 0;
#else
	DBG_ERR("not support\r\n");
	return -1;
#endif
}


static INT32 param_fdset(INT32 argc, char *argv[])
{
#if IPCAM_FD_FUNC
	UINT32 maxFace, faceScale, ProcessRate;
	if (argc != 3) {
		DBG_ERR("argc!=3 (%d)\r\n", argc);
		return -1;
	}
	maxFace = atoi(argv[0]);
	FD_Config(FD_CFG_MAX_FACE, maxFace);
	faceScale = atoi(argv[1]);
	FD_Config(FD_CFG_FACE_SCALE, faceScale);
	ProcessRate = atoi(argv[2]);
	FD_Config(FD_CFG_PROCESS_RATE, ProcessRate);
	DBG_DUMP("fd:%d %d %d\r\n", maxFace, faceScale, ProcessRate);
	return 0;
#else
	DBG_ERR("not support\r\n");
	return -1;
#endif
}

static INT32 param_fdget(INT32 argc, char *argv[])
{
#if IPCAM_FD_FUNC
	UINT32 maxFace, faceScale, ProcessRate;
	char   retStr[32];

	maxFace = FD_GetConfig(FD_CFG_MAX_FACE);
	faceScale = FD_GetConfig(FD_CFG_FACE_SCALE);
	ProcessRate = FD_GetConfig(FD_CFG_PROCESS_RATE);
	snprintf(retStr, sizeof(retStr), "%d %d %d", maxFace, faceScale, ProcessRate);
	NvtUctrl_SetRetString(retStr);
	return 0;
#else
	DBG_ERR("not support\r\n");
	return -1;
#endif
}

static INT32 param_fdrslt(INT32 argc, char *argv[])
{
#if IPCAM_FD_FUNC
	char             retStr[32];
	FD_FACE          FdInfo[FD_NUM_MAX];
	UINT32           i, faceNumber;
	URECT            FdDispCoord = {0, 0, 640, 480};
	CodecIAObjData   rtnFdInfo[FD_NUM_MAX];
	BOOL bUpdated;

	if (argc != 4) {
		DBG_ERR("argc!=4 (%d)\r\n", argc);
		return -1;
	}

	bUpdated = FD_GetRsltFlag();

	FdDispCoord.x = atoi(argv[0]);
	FdDispCoord.y = atoi(argv[1]);
	FdDispCoord.w = atoi(argv[2]);
	FdDispCoord.h = atoi(argv[3]);
	faceNumber = FD_GetRsltFace(FdInfo, &FdDispCoord);
	for (i = 0; i < faceNumber; i++) {
		rtnFdInfo[i].uiCoordinateX = FdInfo[i].x;
		rtnFdInfo[i].uiCoordinateY = FdInfo[i].y;
		rtnFdInfo[i].uiWidth = FdInfo[i].w;
		rtnFdInfo[i].uiHeight = FdInfo[i].h;
	}
	snprintf(retStr, sizeof(retStr),  "%d %d", faceNumber, bUpdated);
	DBG_IND("fd: %s\r\n",retStr);
	NvtUctrl_SetConfigData(rtnFdInfo, sizeof(CodecIAObjData)*faceNumber);
	NvtUctrl_SetRetString(retStr);
	return 0;
#else
	DBG_ERR("not support\r\n");
	return -1;
#endif
}

static INT32 param_ftgstart(INT32 argc, char *argv[])
{
#if (MOVIE_FTG_FUNC == ENABLE && IPCAM_DIS_MV_FUNC == ENABLE)
	UINT32 value;
	if (argc != 1) {
		DBG_ERR("argc!=1 (%d)\r\n", argc);
		return -1;
	}
	DBG_IND("get: %d\r\n", atoi(argv[0]));
	value = atoi(argv[0]);
	if (value == 0) {
#if (FTG_SNAPSHOT_FUN == ENABLE)
		g_bFTGOpen = FALSE;
#endif
		Movie_FTGLock(TRUE);
		if (UI_GetData(FL_MovieDisIndex) == MOVIE_DIS_OFF) {
			ImageUnit_Begin(ISF_IPL(0), 0);
			ImageUnit_SetParam(ISF_CTRL, IPL_SEL_DISCOMP_IMM, SEL_DISCOMP_OFF);
			ImageUnit_End();
		}
	} else {
#if (FTG_SNAPSHOT_FUN == ENABLE)
		g_bFTGOpen = TRUE;
#endif
		Movie_FTGLock(FALSE);
		// movie dis menu is off, so we need enable ipl dis by ourself
		if (UI_GetData(FL_MovieDisIndex) == MOVIE_DIS_OFF) {
			//dis_setDisViewRatio(1000);
			ImageUnit_Begin(ISF_IPL(0), 0);
			ImageUnit_SetParam(ISF_CTRL, IPL_SEL_DISCOMP_IMM, SEL_DISCOMP_ON);
			ImageUnit_End();
		}
		//#NT#2016/11/22#Charlie Chang -begin
		//#NT# support FTG snapshot
#if (FTG_SNAPSHOT_FUN == ENABLE)
		FTG_SNAPSHOT *ftg_snapshot_list = (FTG_SNAPSHOT *)OS_GetMempoolAddr(POOL_ID_FTG_SNAPSHOT);
		////init pool
		memset(ftg_snapshot_list, 0, sizeof(FTG_SNAPSHOT)*FTG_MAX_SNAPSHOT_NUM);
#endif
		//#NT#2016/11/22#Charlie Chang -end
	}

	return 0;
#else
	DBG_ERR("not support\r\n");
	return -1;
#endif
}


static INT32 param_ftgset(INT32 argc, char *argv[])
{
#if (MOVIE_FTG_FUNC == ENABLE)
	UINT32 maxFace, faceScale, ProcessRate, MinFaceScore, drawRate, allInfo;
	UINT32 sensitivity;
	if (argc != 7) {
		DBG_ERR("argc!=7 (%d)\r\n", argc);
		return -1;
	}
	maxFace = atoi(argv[0]);
	ftg_config(ftg_fd_cfg_max_face, maxFace);
	faceScale = atoi(argv[1]);
	ftg_config(ftg_fd_cfg_face_scale, faceScale);
	ProcessRate = atoi(argv[2]);
	ftg_config(ftg_fd_cfg_process_rate, ProcessRate);
	MinFaceScore = atoi(argv[3]);
	ftg_config(ftg_cfg_min_face_score, MinFaceScore);
	drawRate = atoi(argv[4]);
	MovieStamp_set_ftg_draw_config(moviestatmp_ftg_draw_delay_count, drawRate);
	allInfo = atoi(argv[5]);
	MovieStamp_set_ftg_draw_config(moviestatmp_ftp_rlst_type, allInfo);
	sensitivity = atoi(argv[6]);
	ftg_config(ftg_fd_cfg_sensitivity, sensitivity);


	DBG_IND("%d %d %d %d %d %d %d\r\n",
			maxFace, faceScale, ProcessRate, MinFaceScore, drawRate, allInfo, sensitivity);
	return 0;
#else
	DBG_ERR("not support\r\n");
	return -1;
#endif
}

static INT32 param_ftgget(INT32 argc, char *argv[])
{
#if (MOVIE_FTG_FUNC == ENABLE)
	UINT32 maxFace = 0, faceScale = 0, ProcessRate = 0, MinFaceScore = 0, drawRate = 0, allInfo = 0;
	UINT32 sensitivity = 0;
	char   retStr[32];

	sensitivity = ftg_getconfig(ftg_fd_cfg_sensitivity);
	maxFace = ftg_getconfig(ftg_fd_cfg_max_face);
	faceScale = ftg_getconfig(ftg_fd_cfg_face_scale);
	ProcessRate = ftg_getconfig(ftg_fd_cfg_process_rate);
	MinFaceScore = ftg_getconfig(ftg_cfg_min_face_score);
	drawRate = MovieStamp_get_ftg_draw_config(moviestatmp_ftg_draw_delay_count);
	allInfo = MovieStamp_get_ftg_draw_config(moviestatmp_ftp_rlst_type);
	snprintf(retStr, sizeof(retStr), "%d %d %d %d %d %d %d",
			 maxFace, faceScale, ProcessRate, MinFaceScore, drawRate, allInfo, sensitivity);
	NvtUctrl_SetRetString(retStr);
	return 0;
#else
	DBG_ERR("not support\r\n");
	return -1;
#endif
}
#if (FTG_SNAPSHOT_FUN == ENABLE)
static INT32 mapping_ftg_snapshot_id(UINT32 ID)
{

	UINT32 i;
	FTG_SNAPSHOT *ftg_snapshot_list = (FTG_SNAPSHOT *)OS_GetMempoolAddr(POOL_ID_FTG_SNAPSHOT);
	////find same face id
	for (i = 0; i < FTG_MAX_SNAPSHOT_NUM; i++) {
		if (ftg_snapshot_list[i].pic_num == 0) {
			continue;
		}
		/////if id exist ,return the index
		if (ftg_snapshot_list[i].id == ID) {
			return i;
		}

	}
	////can not find id, assigned new index
	for (i = 0; i < FTG_MAX_SNAPSHOT_NUM; i++) {
		if (ftg_snapshot_list[i].pic_num == 0) {
			return i;
		}
	}
	DBG_ERR("snapshot buffer is max, can not buffer id:%d anymore\r\n", ID);
	return -1;
}


static INT32 find_ftg_snapshot_index(UINT32 ID)
{

	UINT32 i;
	FTG_SNAPSHOT *ftg_snapshot_list = (FTG_SNAPSHOT *)OS_GetMempoolAddr(POOL_ID_FTG_SNAPSHOT);

	////find same face id
	for (i = 0; i < FTG_MAX_SNAPSHOT_NUM; i++) {
		if (ftg_snapshot_list[i].pic_num == 0) {
			continue;
		}
		/////if id exist ,return the index
		if (ftg_snapshot_list[i].id == ID) {
			return i;
		}

	}

	DBG_ERR("can not find snapshot ID(%d)\r\n", ID);
	return -1;
}

static void ftg_snapshot_insert(UINT32 face_id, UINT32 face_index, UINT32 over_addr, UINT32 over_size, UINT32 face_addr, UINT32 face_size, UINT32 score)
{

	FTG_SNAPSHOT *ftg_snapshot_list = (FTG_SNAPSHOT *)OS_GetMempoolAddr(POOL_ID_FTG_SNAPSHOT);
	UINT32 i;
	UINT32 min_index = 0;
	UINT32 min_score = 0;

	for (i = 0; i < FTG_MAX_FACE_PIC_NUM ; i++) {
		if (i == 0) {
			min_score = ftg_snapshot_list[face_index].pic_list[i].score;
		}

		if (ftg_snapshot_list[face_index].pic_list[i].score < min_score) {
			min_score = ftg_snapshot_list[face_index].pic_list[i].score;
			min_index = i;
		}

	}


	if (over_size > FTG_OVERRALL_MAX_BUF_SIZE) {
		DBG_ERR("overrall size(%d) is bigger than FTG_FACE_MAX_BUF_SIZE(%d)\r\n", over_size, FTG_OVERRALL_MAX_BUF_SIZE);
	} else {
		memcpy(ftg_snapshot_list[face_index].pic_list[min_index].overrall_pic, (char *)over_addr, over_size);
		ftg_snapshot_list[face_index].pic_list[min_index].overrall_size = over_size;
	}

	if (face_size > FTG_FACE_MAX_BUF_SIZE) {
		DBG_ERR("face size(%d) is bigger than FTG_FACE_MAX_BUF_SIZE(%d)\r\n", face_size, FTG_FACE_MAX_BUF_SIZE);
	} else {
		memcpy(ftg_snapshot_list[face_index].pic_list[min_index].face_pic, (char *)face_addr, face_size);
		ftg_snapshot_list[face_index].pic_list[min_index].face_size = face_size;
	}

	ftg_snapshot_list[face_index].pic_list[min_index].score = score;

	ftg_snapshot_list[face_index].id = face_id;
	if (ftg_snapshot_list[face_index].pic_num < FTG_MAX_FACE_PIC_NUM) {
		ftg_snapshot_list[face_index].pic_num ++;
	}

	//charlie test
	DBG_DUMP("insert ftg snapshot=======\r\n");
	DBG_DUMP("ftg_snapshot_list[%d].id=%d\r\n", face_index, ftg_snapshot_list[face_index].id);
	DBG_DUMP("ftg_snapshot_list[%d].pic_num=%d\r\n", face_index, ftg_snapshot_list[face_index].pic_num);

	for (i = 0; i < ftg_snapshot_list[face_index].pic_num; i++) {
		DBG_DUMP("pic [%d]=======\r\n", i);
		DBG_DUMP("socre =%d\r\n", ftg_snapshot_list[face_index].pic_list[i].score);
		DBG_DUMP("overrall =%x %x %x %x %x %x\r\n",
				 ftg_snapshot_list[face_index].pic_list[i].overrall_pic[0],
				 ftg_snapshot_list[face_index].pic_list[i].overrall_pic[1],
				 ftg_snapshot_list[face_index].pic_list[i].overrall_pic[2],
				 ftg_snapshot_list[face_index].pic_list[i].overrall_pic[3],
				 ftg_snapshot_list[face_index].pic_list[i].overrall_pic[4],
				 ftg_snapshot_list[face_index].pic_list[i].overrall_pic[5]
				);
		DBG_DUMP("overrall size=%d\r\n", ftg_snapshot_list[face_index].pic_list[i].overrall_size);
		DBG_DUMP("face =%x %x %x %x %x %x\r\n",
				 ftg_snapshot_list[face_index].pic_list[i].face_pic[0],
				 ftg_snapshot_list[face_index].pic_list[i].face_pic[1],
				 ftg_snapshot_list[face_index].pic_list[i].face_pic[2],
				 ftg_snapshot_list[face_index].pic_list[i].face_pic[3],
				 ftg_snapshot_list[face_index].pic_list[i].face_pic[4],
				 ftg_snapshot_list[face_index].pic_list[i].face_pic[5]
				);
		DBG_DUMP("face size=%d\r\n", ftg_snapshot_list[face_index].pic_list[i].face_size);
	}
	//end charlie test



	return;

}

#endif
static INT32 param_ftgsnapshotnotify(INT32 argc, char *argv[])
{

#if (FTG_SNAPSHOT_FUN == ENABLE)


	UINT32 face_id = atoi(argv[0]);
	UINT32 score  = atoi(argv[1]);
	UINT32 i;
	ER er;
	UINT32  uiLineOffset[MAX_PLANE_NUM];
	UINT32  uiPxlAddr[MAX_PLANE_NUM];
	IMG_BUF SrcImg, FaceSrcImg;
	UINT32 channelID = 0;
	UINT32  Width = GetMovieSizeWidth_2p(channelID, 0);
	UINT32  Height = GetMovieSizeHeight_2p(channelID, 0);
	// MEDIAREC_YUV_INFO   mediaRecInfo={0};
	UINT32  BufAddr = OS_GetMempoolAddr(POOL_ID_IPCAM_SNAPSHOT);
	UINT32  BufSize = OS_GetMempoolSize(POOL_ID_IPCAM_SNAPSHOT);
	UINT32  jpgSize;
	INT32 ftg_snapshot_index = -1;

	UINT32 overrall_size, face_size;
	//MEM_RANGE  overrall_ftg_snapShotData = {0};
	//MEM_RANGE  face_ftg_snapShotData = {0};
	UINT32 YUV_size = CROP_SOURCE_W * CROP_SOURCE_H * 2;
	UINT32 YUV_addr = OS_GetMempoolAddr(POOL_ID_FTG_SNAPSHOT) + FTG_MAX_SNAPSHOT_SIZE - YUV_size;
	UINT32 overrall_snapshot_addr = OS_GetMempoolAddr(POOL_ID_FTG_SNAPSHOT) + FTG_MAX_SNAPSHOT_SIZE - YUV_size - FTG_OVERRALL_MAX_BUF_SIZE;
	UINT32 face_snapshot_addr = OS_GetMempoolAddr(POOL_ID_FTG_SNAPSHOT) + FTG_MAX_SNAPSHOT_SIZE - YUV_size - FTG_OVERRALL_MAX_BUF_SIZE - FTG_FACE_MAX_BUF_SIZE;
	DBG_DUMP("face_id=%d score=%d\r\n", face_id, score);

	if (argc != 2) {
		DBG_ERR("argc!=2 (%d)\r\n", argc);
		return -1;
	}
	//charlie test

	/*
	    if(g_bFTGOpen== FALSE){
	        DBG_ERR("FTG is not open!!\r\n");
	        return -1;
	    }
	*/
//end charlie test

	if (face_id <= 0) {
		DBG_ERR("face_id:%d error\r\n", face_id);
		return -1;
	}

	BOOL check_ftg_list_id = FALSE;
	for (i = 0; i < FTG_MAX_SNAPSHOT_NUM; i++) {
		if (g_ftg_list[i].id == face_id) {
			ftg_snapshot_index = i;
			check_ftg_list_id = TRUE;
			break;
		}
	}
	if (check_ftg_list_id == FALSE) {
		DBG_ERR("can not find id (%d) in ftg list\r\n", face_id);
		return -1;
	}


	////need to do, to get face tracking granding YUV buffer
	// MovRec_GetLastV_YUV(channelID,&mediaRecInfo);

	uiPxlAddr[0] = g_ftg_list[ftg_snapshot_index].p_frame->PxlAddr[0];
	uiPxlAddr[1] = g_ftg_list[ftg_snapshot_index].p_frame->PxlAddr[1];
	uiPxlAddr[2] = g_ftg_list[ftg_snapshot_index].p_frame->PxlAddr[2];
	uiLineOffset[0]    = g_ftg_list[ftg_snapshot_index].p_frame->LineOffset[0];
	uiLineOffset[1]    = g_ftg_list[ftg_snapshot_index].p_frame->LineOffset[1];
	uiLineOffset[2]    = g_ftg_list[ftg_snapshot_index].p_frame->LineOffset[2];

	//DBG_IND("mediaRecInfo.Yaddr 0x%x, Ylot=0x%x\r\n",mediaRecInfo.Yaddr, mediaRecInfo.Ylot);
	/*
	    uiPxlAddr[0] = mediaRecInfo.Yaddr;
	    uiPxlAddr[1] = mediaRecInfo.UVaddr;
	    uiPxlAddr[2] = mediaRecInfo.UVaddr;
	    uiLineOffset[0]    = mediaRecInfo.Ylot;
	    uiLineOffset[1]    = mediaRecInfo.Ylot;
	    uiLineOffset[2]    = mediaRecInfo.Ylot;
	*/
	////snapshot overrall
	er = GxImg_InitBufEx(&SrcImg, Width, Height, GX_IMAGE_PIXEL_FMT_YUV420, uiLineOffset, uiPxlAddr);
	if (er != E_OK) {
		DBG_ERR("GxImg_InitBufEx error er=%d\r\n", er);
		return -1;

	}
	jpgSize = SnapShot_EncJPG(BufAddr, BufSize, &SrcImg);
	memcpy((char *)overrall_snapshot_addr, (char *) BufAddr, jpgSize);
	overrall_size = jpgSize;
	//overrall_ftg_snapShotData.Addr = BufAddr;
	//overrall_ftg_snapShotData.Size = jpgSize;
	DBG_IND("Snapshot overrall size=0x%x\r\n", overrall_size);

	//////////////////////snapshot face
	er = GxImg_InitBufEx(&SrcImg, Width, Height, GX_IMAGE_PIXEL_FMT_YUV420, uiLineOffset, uiPxlAddr);
	if (er != E_OK) {
		DBG_ERR("GxImg_InitBufEx face error er=%d\r\n", er);
		return -1;
	}
	IRECT face_rec = {0};
	face_rec.h = g_ftg_list[ftg_snapshot_index].height;
	face_rec.w = g_ftg_list[ftg_snapshot_index].width;
	face_rec.x = g_ftg_list[ftg_snapshot_index].stx;
	face_rec.y = g_ftg_list[ftg_snapshot_index].sty;


	er = GxImg_InitBuf(&FaceSrcImg, face_rec.w, face_rec.h, GX_IMAGE_PIXEL_FMT_YUV420,
					   GXIMAGE_LINEOFFSET_ALIGN(16), YUV_addr, YUV_size);
	if (er != E_OK) {
		DBG_ERR("GxImg_InitBuf error er=%d\r\n");
		return -1;
	}
	er = GxImg_CopyData(&SrcImg, &face_rec, &FaceSrcImg, NULL);
	if (er != E_OK) {
		DBG_ERR("GxImg_CopyData error er=%d\r\n");
		return -1;
	}

	jpgSize = SnapShot_EncJPG(BufAddr, BufSize, &FaceSrcImg);
	memcpy((char *)face_snapshot_addr, (char *)BufAddr, jpgSize);
	face_size = jpgSize;

	DBG_IND("Snapshot  size=0x%x\r\n", face_size);

	////mapping face id
	ftg_snapshot_index = mapping_ftg_snapshot_id(face_id);
	if (ftg_snapshot_index == -1) {
		DBG_ERR("assgind FTG snapshot index error!!\r\n");
		return -1;
	}
	DBG_IND("ftg_snapshot_index=%d\r\n", ftg_snapshot_index);
	ftg_snapshot_insert(face_id, ftg_snapshot_index, overrall_snapshot_addr,
						overrall_size, face_snapshot_addr, face_size, score);

#endif

	return 0;

}

static INT32 param_ftgsetroi(INT32 argc, char *argv[])
{

#if (MOVIE_FTG_FUNC == ENABLE)
	URECT roi_rec = {0};
	UINT32 use_roi;
	if (argc != 5) {
		DBG_ERR("argc!=5 (%d)\r\n", argc);
		return -1;
	}
	use_roi =  atoi(argv[0]);
	roi_rec.x = atoi(argv[1]);
	roi_rec.y = atoi(argv[2]);
	roi_rec.w = atoi(argv[3]);
	roi_rec.h = atoi(argv[4]);
	ftg_setfdroi(use_roi, &roi_rec);
	DBG_IND("%d %d %d\r\n", use_roi, roi_rec.x, roi_rec.y, roi_rec.w, roi_rec.h);
	return 0;
#else
	DBG_ERR("not support\r\n");
	return -1;
#endif

}


static INT32 param_ftgsnapshotget(INT32 argc, char *argv[])
{

#if (FTG_SNAPSHOT_FUN == ENABLE)

	if (argc != 1) {
		DBG_ERR("argc!=1 (%d)\r\n", argc);
		return -1;
	}
	UINT32 id = atoi(argv[0]);
	INT32 face_index;
	char   retStr[128];
	FTG_SNAPSHOT *ftg_snapshot_list = (FTG_SNAPSHOT *)OS_GetMempoolAddr(POOL_ID_FTG_SNAPSHOT);
	face_index = find_ftg_snapshot_index(id);
	if (face_index < 0) {
		DBG_ERR("find_ftg_snapshot_index error face_index=%d\r\n", face_index);
		return -1;

	}

	snprintf(retStr, sizeof(retStr), "%d %lu", face_index < 0 ? -1 : 0, NvtIPC_GetPhyAddr((UINT32)&ftg_snapshot_list[face_index]));
	DBG_DUMP("snapshot:%s\r\n", retStr);
	//charlie test
	DBG_DUMP("get ftg snapshot=======\r\n");
	DBG_DUMP("ftg_snapshot_list[%d].id=%d\r\n", face_index, ftg_snapshot_list[face_index].id);
	DBG_DUMP("ftg_snapshot_list[%d].pic_num=%d\r\n", face_index, ftg_snapshot_list[face_index].pic_num);
	UINT32 i;
	for (i = 0; i < ftg_snapshot_list[face_index].pic_num; i++) {
		DBG_DUMP("pic [%d]=======\r\n", i);
		DBG_DUMP("socre =%d\r\n", ftg_snapshot_list[face_index].pic_list[i].score);
		DBG_DUMP("overrall =%x %x %x %x %x %x\r\n",
				 ftg_snapshot_list[face_index].pic_list[i].overrall_pic[0],
				 ftg_snapshot_list[face_index].pic_list[i].overrall_pic[1],
				 ftg_snapshot_list[face_index].pic_list[i].overrall_pic[2],
				 ftg_snapshot_list[face_index].pic_list[i].overrall_pic[3],
				 ftg_snapshot_list[face_index].pic_list[i].overrall_pic[4],
				 ftg_snapshot_list[face_index].pic_list[i].overrall_pic[5]
				);
		DBG_DUMP("overrall size=%d\r\n", ftg_snapshot_list[face_index].pic_list[i].overrall_size);
		DBG_DUMP("face =%x %x %x %x %x %x\r\n",
				 ftg_snapshot_list[face_index].pic_list[i].face_pic[0],
				 ftg_snapshot_list[face_index].pic_list[i].face_pic[1],
				 ftg_snapshot_list[face_index].pic_list[i].face_pic[2],
				 ftg_snapshot_list[face_index].pic_list[i].face_pic[3],
				 ftg_snapshot_list[face_index].pic_list[i].face_pic[4],
				 ftg_snapshot_list[face_index].pic_list[i].face_pic[5]
				);
		DBG_DUMP("face size=%d\r\n", ftg_snapshot_list[face_index].pic_list[i].face_size);
	}
	//end charlie test


	NvtUctrl_SetRetString(retStr);
#endif

	return 0;

}
static INT32 param_ftgsnapshotremove(INT32 argc, char *argv[])
{

#if (FTG_SNAPSHOT_FUN == ENABLE)

	if (argc != 1) {
		DBG_ERR("argc!=1 (%d)\r\n", argc);
		return -1;
	}
	UINT32 id = atoi(argv[0]);
	INT32 face_index;
	FTG_SNAPSHOT *ftg_snapshot_list = (FTG_SNAPSHOT *)OS_GetMempoolAddr(POOL_ID_FTG_SNAPSHOT);
	face_index = find_ftg_snapshot_index(id);
	if (face_index < 0) {
		DBG_ERR("find_ftg_snapshot_index error face_index=%d\r\n", face_index);
		return -1;

	}
	memset(&ftg_snapshot_list[face_index], 0, sizeof(FTG_SNAPSHOT));
	DBG_DUMP("remove face id:%d\r\n", id);
#endif

	return 0;

}



static INT32 param_ftggetroi(INT32 argc, char *argv[])
{
#if (MOVIE_FTG_FUNC == ENABLE)
	URECT roi_rec = {0};
	BOOL use_roi;
	char   retStr[128];

	use_roi = ftg_getfdroi(&roi_rec);
	snprintf(retStr, sizeof(retStr), "%d %d %d %d", use_roi, roi_rec.x, roi_rec.y, roi_rec.w, roi_rec.h);
	NvtUctrl_SetRetString(retStr);
	return 0;
#else
	DBG_ERR("not support\r\n");
	return -1;
#endif
}

static INT32 param_ftgrslt(INT32 argc, char *argv[])
{
#if (MOVIE_FTG_FUNC == ENABLE)
	char             retStr[32];
	ftg_face         FtgInfo[FD_NUM_MAX];
	UINT32           i, faceNumber;
	URECT            FdDispCoord = {0, 0, 640, 480};
	CodecFTGObjData  rtnFdInfo[FD_NUM_MAX];
	BOOL             bUpdated = TRUE;

	if (argc != 4) {
		DBG_ERR("argc!=4 (%d)\r\n", argc);
		return -1;
	}
	//bUpdated = FD_GetRsltFlag();
	FdDispCoord.x = atoi(argv[0]);
	FdDispCoord.y = atoi(argv[1]);
	FdDispCoord.w = atoi(argv[2]);
	FdDispCoord.h = atoi(argv[3]);
	ftg_rlst_type ftg_type;
	ftg_type = MovieStamp_get_ftg_draw_config(moviestatmp_ftp_rlst_type);
	faceNumber = ftg_getrsltface2(FtgInfo, &FdDispCoord, ftg_type);

	for (i = 0; i < faceNumber; i++) {
		rtnFdInfo[i].stx = FtgInfo[i].stx;
		rtnFdInfo[i].sty = FtgInfo[i].sty;
		rtnFdInfo[i].width = FtgInfo[i].width;
		rtnFdInfo[i].height = FtgInfo[i].height;
		rtnFdInfo[i].id = FtgInfo[i].id;
		rtnFdInfo[i].type = FtgInfo[i].type;
		rtnFdInfo[i].clarity = FtgInfo[i].clarity;
		rtnFdInfo[i].luma = FtgInfo[i].luma;
		rtnFdInfo[i].angle_horizontal = FtgInfo[i].angle_horizontal;
		rtnFdInfo[i].angle_vertical = FtgInfo[i].angle_vertical;
		rtnFdInfo[i].angle_roll = FtgInfo[i].angle_roll;
		rtnFdInfo[i].score = FtgInfo[i].score;

	}

	//#NT#2016/11/21#Charlie Chang -begin
	//#NT# support FTG snapshot
#if (FTG_SNAPSHOT_FUN == ENABLE)
	if (faceNumber > FTG_MAX_SNAPSHOT_NUM) {
		DBG_ERR("face number (%d) is over FTG_MAX_NUM (%d), only snapshot (%d)\r\n",
				faceNumber, FTG_MAX_SNAPSHOT_NUM, FTG_MAX_SNAPSHOT_NUM);
		faceNumber = FTG_MAX_SNAPSHOT_NUM;
	}
	memcpy(&g_ftg_list, &FtgInfo, sizeof(ftg_face)*faceNumber);
	g_face_number = faceNumber;

	//charlie test
	/*
	DBG_DUMP("FTG detect==============\r\n");
	for (i=0;i<g_face_number;i++)
	{
	    DBG_DUMP("index=%d \r\n"
	             "x=%d y=%d w=%d h=%d id=%d type=%d clarity=%d \r\n"
	             "a_h=%d a_v=%d a_r=%d score=%d\r\n",
	        i,
	        g_ftg_list[i].stx,
	        g_ftg_list[i].sty,
	        g_ftg_list[i].width,
	        g_ftg_list[i].height,
	        g_ftg_list[i].id,
	        g_ftg_list[i].type,
	        g_ftg_list[i].clarity,
	        g_ftg_list[i].angle_horizontal,
	        g_ftg_list[i].angle_vertical,
	        g_ftg_list[i].angle_roll,
	        g_ftg_list[i].score

	    );
	}
	*/
	//end charlie test

#endif
	//#NT#2016/11/21#Charlie Chang -end
	snprintf(retStr, sizeof(retStr),  "%d %d", faceNumber, bUpdated);
	NvtUctrl_SetConfigData(rtnFdInfo, sizeof(ftg_face)*faceNumber);
	NvtUctrl_SetRetString(retStr);
	return 0;
#else
	DBG_ERR("not support\r\n");
	return -1;
#endif
}

static INT32 param_tdstart(INT32 argc, char *argv[])
{
#if (IPCAM_BC_FUNC == ENABLE)
	UINT32 value;
	if (argc != 1) {
		DBG_ERR("argc!=1 (%d)\r\n", argc);
		return -1;
	}
	DBG_IND("get: %d\r\n", atoi(argv[0]));
	value = atoi(argv[0]);
	if (value == 1) {

		IPCam_BCLock(TRUE);
	} else {
		IPCam_BCLock(FALSE);
	}
	return 0;
#else
	DBG_ERR("not support\r\n");
	return -1;
#endif
}


static INT32 param_tdset(INT32 argc, char *argv[])
{
#if (IPCAM_BC_FUNC == ENABLE)
	CodecTDSetting pTDSetting = {0};
	NvtUctrl_GetConfigData((void *)&pTDSetting, sizeof(CodecTDSetting));
	DBG_DUMP("set td pct:%d   frame:%d\r\n",pTDSetting.uiPCT,pTDSetting.uiFrameNum);

	bc_settdprms(BC_TAMPER_PCT, pTDSetting.uiPCT);
	bc_settdprms(BC_TAMPER_FRM_NUM, pTDSetting.uiFrameNum);

	return 0;
#else
	DBG_ERR("not support\r\n");
	return -1;
#endif
}

static INT32 param_tdget(INT32 argc, char *argv[])
{
#if (IPCAM_BC_FUNC == ENABLE)
	CodecTDSetting TDSetting = {0};

	TDSetting.uiFrameNum = bc_gettdprms(BC_TAMPER_FRM_NUM);
	TDSetting.uiPCT = bc_gettdprms(BC_TAMPER_PCT);

	NvtUctrl_SetConfigData((void *)&TDSetting, sizeof(CodecTDSetting));

	return 0;
#else
	DBG_ERR("not support\r\n");
	return -1;
#endif
}

static INT32 param_tdrslt(INT32 argc, char *argv[])
{
#if (IPCAM_BC_FUNC == ENABLE)


	CodecTDStatus TDStatus = {0};
	TDStatus.bStatus = bc_gettdprms(BC_TD_STATUS);
	TDStatus.uiFrameNum_Cur = bc_gettdprms(BC_TAMPER_FRM_NUM_CUR);
	TDStatus.uiPCT_Cur  = bc_gettdprms(BC_TAMPER_PCT_CUR);
	NvtUctrl_SetConfigData((void *)&TDStatus, sizeof(CodecTDStatus));
	return 0;
#else
	DBG_ERR("not support\r\n");
	return -1;
#endif
}


//#NT#2016/08/26#Lincy Lin -begin
//#NT# Add uctrl command for BC/MV/IPL buffer APIs .
static INT32 param_dspbuf_get(INT32 argc, char *argv[])
{
	IVSBuf  sBuf = {0};

#if (LINUX_CVHOST_FUNC == ENABLE)
	sBuf.uiAddr = NvtIPC_GetPhyAddr(OS_GetMempoolAddr(POOL_ID_CVHOST));
	sBuf.uiSize = OS_GetMempoolSize(POOL_ID_CVHOST);

	//DBGH(sBuf.uiAddr);
	//DBGH(sBuf.uiSize);
#endif
	NvtUctrl_SetConfigData((void *)&sBuf, sizeof(sBuf));
	return 0;
}

static CodecPixelFormat xGximg_pixelfmt_to_ivs_pixelfmt(GX_IMAGE_PIXEL_FMT  Gximg_pixelfmt)
{
	switch (Gximg_pixelfmt) {
	case GX_IMAGE_PIXEL_FMT_YUV422_PLANAR:
		return CODEC_PIXEL_FMT_YUV422_PLANAR;

	case GX_IMAGE_PIXEL_FMT_YUV420_PLANAR:
		return CODEC_PIXEL_FMT_YUV420_PLANAR;

	case GX_IMAGE_PIXEL_FMT_YUV422_PACKED:
		return CODEC_PIXEL_FMT_YUV422_PACKED;

	case GX_IMAGE_PIXEL_FMT_YUV420_PACKED:
		return CODEC_PIXEL_FMT_YUV420_PACKED;

	case GX_IMAGE_PIXEL_FMT_Y_ONLY:
		return CODEC_PIXEL_FMT_Y_ONLY;

	case GX_IMAGE_PIXEL_FMT_ARGB565:
		return CODEC_PIXEL_FMT_ARGB565;

	case GX_IMAGE_PIXEL_FMT_RGB888_PLANAR:
		return CODEC_PIXEL_FMT_RGB888_PLANAR;

	default:
		DBG_ERR("Unknown pxlfmt %d\r\n", Gximg_pixelfmt);
		return CODEC_PIXEL_FMT_YUV420_PACKED;

	}
}


static INT32 param_img_get(INT32 argc, char *argv[])
{
	IMG_BUF       InputImg = {0};
	IMG_BUF       tmpImg;
	UINT32        channel;
	BOOL          bIsCopy;
	UINT32        tmpbufAddr, tmpbufSize, i;
	CodecImageBuf   imgbuf = {0};
	ISF_PORT     *pSrc;


	if (argc != 4) {
		DBG_ERR("argc!=4 (%d)\r\n", argc);
		goto img_get_fail;
	}
	channel = atoi(argv[0]);
	bIsCopy = atoi(argv[1]);
	sscanf_s(argv[2], "%x", &tmpbufAddr);
	sscanf_s(argv[3], "%x", &tmpbufSize);
	DBG_IND("channel= %d, bIsCopy =%d, tmpbufAddr = 0x%x, tmpbufSize = 0x%x\r\n", channel, bIsCopy, tmpbufAddr, tmpbufSize);
	if (channel > 4) {
		DBG_ERR("channel %d\r\n", channel);
		goto img_get_fail;
	}
	if (bIsCopy && (tmpbufAddr == 0 || tmpbufSize == 0)) {
		DBG_ERR("tmpbufAddr 0x%x, tmpbufSize 0x%x\r\n", tmpbufAddr, tmpbufSize);
		goto img_get_fail;
	}
	pSrc = ImageUnit_Out(ISF_IPL(0), ISF_OUT1 + channel);
	if (ImageUnit_IsAllowPull(pSrc)) {
		DBG_ERR("IPL path %d is not enable\r\n", channel);
		goto img_get_fail;
	}
	ISF_RV ret = ImageUnit_PullData(ImageUnit_Out(ISF_IPL(0), ISF_OUT1 + channel), (ISF_DATA *)&InputImg, 0);
	if(ret != ISF_OK){
		DBG_ERR("ImageUnit_PullData error ret:%d\r\n",ret);
		goto img_get_fail;
	}
	if (bIsCopy) {
		tmpbufAddr = NvtIPC_GetNonCacheAddr(tmpbufAddr);
		if (GxImg_InitBuf(&tmpImg, InputImg.Width, InputImg.Height, GX_IMAGE_PIXEL_FMT_Y_ONLY, InputImg.LineOffset[0], tmpbufAddr, tmpbufSize) != E_OK) {
			DBG_ERR("GxImg_InitBuf fail\r\n");
			goto img_get_fail;
		}
		GxImg_ScaleData(&InputImg, REGION_MATCH_IMG, &tmpImg, REGION_MATCH_IMG);
		InputImg = tmpImg;
	}
	//GxImg_DumpBuf("A:\\dspimg.raw", &InputImg, GX_IMAGE_DUMP_ALL);
	imgbuf.uiWidth = InputImg.Width;
	imgbuf.uiHeight = InputImg.Height;
	imgbuf.ePxlFmt = xGximg_pixelfmt_to_ivs_pixelfmt(InputImg.PxlFmt);
	for (i = 0; i < MAX_PLANE_NUM; i++) {
		if (InputImg.PxlAddr[i]) {
			imgbuf.uiPxlAddr[i] = NvtIPC_GetPhyAddr(InputImg.PxlAddr[i]);
		}
	}
	memcpy(imgbuf.uiLineOffset, InputImg.LineOffset, sizeof(UINT32)*MAX_PLANE_NUM);
	NvtUctrl_SetConfigData((void *)&imgbuf, sizeof(imgbuf));
	return 0;

img_get_fail:
	NvtUctrl_SetConfigData((void *)&imgbuf, sizeof(imgbuf));
	return -1;

}

STATIC_ASSERT(sizeof(IVSMVInfor) == sizeof(MOTION_INFOR));


static INT32 param_mvstart(INT32 argc, char *argv[])
{
	#if 0
#if (IPCAM_DIS_MV_FUNC == ENABLE)
	UINT32 value;
	if (argc != 1) {
		DBG_ERR("argc!=1 (%d)\r\n", argc);
		return -1;
	}
	DBG_IND("get: %d\r\n", atoi(argv[0]));
	value = atoi(argv[0]);
	if (value == 0) {
		if (UI_GetData(FL_MovieDisIndex) == MOVIE_DIS_OFF) {
			ImageUnit_Begin(ISF_IPL(0), 0);
			ImageUnit_SetParam(ISF_CTRL, IPL_SEL_DISCOMP_IMM, SEL_DISCOMP_OFF);
			ImageUnit_End();
		}
	} else {
		// movie dis menu is off, so we need enable ipl dis by ourself
		if (UI_GetData(FL_MovieDisIndex) == MOVIE_DIS_OFF) {
			dis_setDisViewRatio(1000);
			ImageUnit_Begin(ISF_IPL(0), 0);
			ImageUnit_SetParam(ISF_CTRL, IPL_SEL_DISCOMP_IMM, SEL_DISCOMP_ON);
			ImageUnit_End();
		}
	}
	return 0;
#else
	DBG_ERR("not support\r\n");
	return -1;
#endif
#endif
return 0;
}
static INT32 param_mvset(INT32 argc, char *argv[])
{
	#if 0
#if (IPCAM_DIS_MV_FUNC == ENABLE)
	IVSMVSetting   mvsetting = {0};
	DIS_ROIGMV_IN  roiIn;
	DISALG_CFGINFO DisConfigInfo;

	NvtUctrl_GetConfigData((void *)&mvsetting, sizeof(mvsetting));
	DisConfigInfo.iCfgVal = mvsetting.bGlobalMotionEnable;
	dis_SetConfig(DISALG_CFG_GLOBALMOTEN, &DisConfigInfo);
	DisConfigInfo.iCfgVal = mvsetting.bROIMotionEnable;
	dis_SetConfig(DISALG_CFG_ROIMOTEN, &DisConfigInfo);
	roiIn.uiStartX = mvsetting.uiROIStartX;
	roiIn.uiStartY = mvsetting.uiROIStartY;
	roiIn.uiSizeX = mvsetting.uiROISizeX;
	roiIn.uiSizeY = mvsetting.uiROISizeY;
	dis_roi_setInputInfo(&roiIn);
	DBG_IND("GM=%d,ROIM=%d,roi %d,%d,%d,%d, inH %d,inV %d\r\n", mvsetting.bGlobalMotionEnable, mvsetting.bROIMotionEnable, mvsetting.uiROIStartX, mvsetting.uiROIStartY, mvsetting.uiROISizeX, mvsetting.uiROISizeY, mvsetting.uiInputSizeH, mvsetting.uiInputSizeV);
	return 0;
#else
	DBG_ERR("not support\r\n");
	return -1;
#endif
#endif
return 0;
}

static INT32 param_mvget(INT32 argc, char *argv[])
{
	#if 0
#if (IPCAM_DIS_MV_FUNC == ENABLE)
	IVSMVSetting   mvsetting = {0};
	DISALG_PARAM   DisInputInfo;
	DIS_ROIGMV_IN  roiIn = {0};
	DISALG_CFGINFO DisConfigInfo;

	dis_GetConfig(DISALG_CFG_GLOBALMOTEN, &DisConfigInfo);
	mvsetting.bGlobalMotionEnable = DisConfigInfo.iCfgVal;
	dis_GetConfig(DISALG_CFG_ROIMOTEN, &DisConfigInfo);
	mvsetting.bROIMotionEnable = DisConfigInfo.iCfgVal;
	dis_roi_getInputInfo(&roiIn);
	mvsetting.uiROIStartX = roiIn.uiStartX;
	mvsetting.uiROIStartY = roiIn.uiStartY;
	mvsetting.uiROISizeX = roiIn.uiSizeX;
	mvsetting.uiROISizeY = roiIn.uiSizeY;
	dis_getInputInfo(&DisInputInfo);
	mvsetting.uiInputSizeH = DisInputInfo.InSizeH;
	mvsetting.uiInputSizeV = DisInputInfo.InSizeV;
	DBG_IND("GM=%d,ROIM=%d,roi %d,%d,%d,%d, inH %d,inV %d\r\n", mvsetting.bGlobalMotionEnable, mvsetting.bROIMotionEnable, mvsetting.uiROIStartX, mvsetting.uiROIStartY, mvsetting.uiROISizeX, mvsetting.uiROISizeY, mvsetting.uiInputSizeH, mvsetting.uiInputSizeV);
	NvtUctrl_SetConfigData((void *)&mvsetting, sizeof(mvsetting));
	return 0;
#else
	DBG_ERR("not support\r\n");
	return -1;
#endif
#endif
return 0;
}


static INT32 param_mvrslt(INT32 argc, char *argv[])
{
	#if 0
#if (IPCAM_DIS_MV_FUNC == ENABLE)
	static MV_INFO_SIZE   DisMvInfo;
	static IVSMVData      Mvdata;
	DisMvInfo.motVec = (MOTION_INFOR *)&Mvdata.sIVSMVROIResult;
	dis_getReadyMotionVec(&DisMvInfo);
	Mvdata.uiNumH = DisMvInfo.uiNumH;
	Mvdata.uiNumV = DisMvInfo.uiNumV;
	NvtUctrl_SetConfigData((void *)&Mvdata, sizeof(Mvdata));
#if 0
	DBG_IND("uiNumH %d, uiNumV %d\r\n", DisMvInfo.uiNumH, DisMvInfo.uiNumV);
	{
		UINT32      num, i;
		IVSMVInfor *pMvInfo;

		num = Mvdata.uiNumH * Mvdata.uiNumV;
		for (i = 0; i < num; i++) {
			pMvInfo = &Mvdata.sIVSMVROIResult[i];
			DBG_IND("pMvInfo[%d]=>x %d, y %d, Sad %d, Cnt %d, blkidx %d, valid %d\r\n", i, pMvInfo->iX, pMvInfo->iY, pMvInfo->uiSad, pMvInfo->uiCnt, pMvInfo->uiIdx, pMvInfo->bValid);
			if (i > 20) {
				DBG_IND("====Ignore====\r\n");
				break;
			}
		}
	}
#endif
	return 0;
#else
	DBG_ERR("not support\r\n");
	return -1;
#endif
#endif
return 0;
}

static INT32 param_bcstart(INT32 argc, char *argv[])
{
#if (IPCAM_BC_FUNC == ENABLE)
	UINT32 value;
	if (argc != 1) {
		DBG_ERR("argc!=1 (%d)\r\n", argc);
		return -1;
	}
	DBG_IND("get: %d\r\n", atoi(argv[0]));
	value = atoi(argv[0]);
	if (value == 1) {

		IPCam_BCLock(TRUE);
	} else {
		IPCam_BCLock(FALSE);
	}
	return 0;
#else
	DBG_ERR("not support\r\n");
	return -1;
#endif
}


static INT32 param_bcset(INT32 argc, char *argv[])
{
#if (IPCAM_BC_FUNC == ENABLE)
	CodecBCSetting   bcsetting;

	NvtUctrl_GetConfigData((void *)&bcsetting, sizeof(bcsetting));

	//bc_setprms(BC_RESET,			bcsetting.bReset);
	//bc_setprms(BC_RSLT_NEW,			bcsetting.bUpdateReset);
	bc_setprms(BC_UPDATE_PROB,		bcsetting.uiUpdateProb);
	bc_setprms(BC_UPDATE_NEIGHBOR_EN,bcsetting.bUpdateNeighborEn);
	bc_setprms(BC_DEGHOST_EN,		bcsetting.bDeghostEn);
	bc_setprms(BC_MAX_FG_FRM_NUM,	bcsetting.uiMaxFGFrameNum);
	bc_setprms(BC_MIN_OBJ_AREA_PCT,	bcsetting.uiMinOBJAreaPCT);
	bc_setprms(BC_TH_CROSS_FRM_NUM,	bcsetting.uiTHCrossFrameNum);
	bc_setprms(BC_TH_FEATURE,		bcsetting.uiTHFeature);
	bc_setprms(BC_SENSI,			bcsetting.uiSensi);
	bc_setblkprms(BC_BLK_NUM_H,		bcsetting.uiBlkNumH);
	bc_setblkprms(BC_BLK_NUM_W,		bcsetting.uiBlkNumW);
	bc_setblkprms(BC_MIN_OBJ_BLK_NUM,bcsetting.MinOBJBlkNum);
	bc_setblkprms(BC_FG_PCT_PER_BLK,bcsetting.FGPCTperBlk);

	DBG_DUMP("BBC set===\r\n");
	//DBG_DUMP("Reset:              %d\r\n",bcsetting.bReset);
	//DBG_DUMP("bUpdateReset:       %d\r\n",bcsetting.bUpdateReset);
	DBG_DUMP("uiUpdateProb:       %d\r\n",bcsetting.uiUpdateProb);
	DBG_DUMP("bUpdateNeighborEn:  %d\r\n",bcsetting.bUpdateNeighborEn);
	DBG_DUMP("bDeghostEn:         %d\r\n",bcsetting.bDeghostEn);
	DBG_DUMP("uiMaxFGFrameNum:    %d\r\n",bcsetting.uiMaxFGFrameNum);
	DBG_DUMP("uiMinOBJAreaPCT:    %d\r\n",bcsetting.uiMinOBJAreaPCT);
	DBG_DUMP("uiTHCrossFrameNum:  %d\r\n",bcsetting.uiTHCrossFrameNum);
	DBG_DUMP("uiTHFeature:        %d\r\n",bcsetting.uiTHFeature);
	DBG_DUMP("uiSensi:            %d\r\n",bcsetting.uiSensi);
	DBG_DUMP("uiBlkNumH:          %d\r\n",bcsetting.uiBlkNumH);
	DBG_DUMP("uiBlkNumW:          %d\r\n",bcsetting.uiBlkNumW);
	DBG_DUMP("MinOBJBlkNum:       %d\r\n",bcsetting.MinOBJBlkNum);
	DBG_DUMP("FGPCTperBlk:        %d\r\n",bcsetting.FGPCTperBlk);


	return 0;
#else
	DBG_ERR("not support\r\n");
	return -1;
#endif
}


static INT32 param_bcget(INT32 argc, char *argv[])
{
#if (IPCAM_BC_FUNC == ENABLE)
	CodecBCSetting   bcsetting;

	//bcsetting.bReset 			= bc_getprms(BC_RESET);
	//bcsetting.bUpdateReset 		= bc_getprms(BC_RSLT_NEW);
	bcsetting.uiUpdateProb 		= bc_getprms(BC_UPDATE_PROB);
	bcsetting.bUpdateNeighborEn = bc_getprms(BC_UPDATE_NEIGHBOR_EN);
	bcsetting.bDeghostEn 		= bc_getprms(BC_DEGHOST_EN);
	bcsetting.uiMaxFGFrameNum 	= bc_getprms(BC_MAX_FG_FRM_NUM);
	bcsetting.uiMinOBJAreaPCT 	= bc_getprms(BC_MIN_OBJ_AREA_PCT);
	bcsetting.uiTHCrossFrameNum = bc_getprms(BC_TH_CROSS_FRM_NUM);
	bcsetting.uiTHFeature 		= bc_getprms(BC_TH_FEATURE);
	bcsetting.uiSensi		 	= bc_getprms(BC_SENSI);
	bcsetting.uiBlkNumH 		= bc_getblkprms(BC_BLK_NUM_H);
	bcsetting.uiBlkNumW 		= bc_getblkprms(BC_BLK_NUM_W);
	bcsetting.MinOBJBlkNum 		= bc_getblkprms(BC_MIN_OBJ_BLK_NUM);
	bcsetting.FGPCTperBlk 		= bc_getblkprms(BC_FG_PCT_PER_BLK);


	NvtUctrl_SetConfigData((void *)&bcsetting, sizeof(CodecBCSetting));


	return 0;
#else
	DBG_ERR("not support\r\n");
	return -1;
#endif
}

#if 1
static INT32 param_bcreset(INT32 argc, char *argv[])
{
#if (IPCAM_BC_FUNC == ENABLE)
	bc_setprms(BC_RESET,1);
	DBG_IND("\r\n");
	return 0;
#else
	DBG_ERR("not support\r\n");
	return -1;
#endif
}
#endif

static INT32 param_bcfgget(INT32 argc, char *argv[])
{
#if (IPCAM_BC_FUNC == ENABLE)
	IMG_BUF      	InputImg;
	CodecImageBuf  	imgbuf = {0};
	UINT32       	i;

	bc_getforeground(&InputImg);
	imgbuf.uiWidth = InputImg.Width;
	imgbuf.uiHeight = InputImg.Height;
	imgbuf.ePxlFmt = InputImg.PxlFmt;

	for (i = 0; i < MAX_PLANE_NUM; i++) {
		if (InputImg.PxlAddr[i]) {
			imgbuf.uiPxlAddr[i] = NvtIPC_GetPhyAddr(InputImg.PxlAddr[i]);
		}
	}
	memcpy(imgbuf.uiLineOffset, InputImg.LineOffset, sizeof(UINT32)*MAX_PLANE_NUM);
	NvtUctrl_SetConfigData((void *)&imgbuf, sizeof(imgbuf));
	DBG_DUMP("w=%d,h=%d Fmt=%d,Y=0x%x,U=0x%x,V=0x%x\r\n", imgbuf.uiWidth, imgbuf.uiHeight, imgbuf.ePxlFmt, imgbuf.uiPxlAddr[0], imgbuf.uiPxlAddr[1], imgbuf.uiPxlAddr[2]);
	return 0;
#else
	DBG_ERR("not support\r\n");
	return -1;
#endif
}

static INT32 param_bcbgget(INT32 argc, char *argv[])
{
#if (IPCAM_BC_FUNC == ENABLE)
	IMG_BUF      InputImg;
	CodecImageBuf  imgbuf = {0};
	UINT32       i;

	bc_getbackground(&InputImg);
	imgbuf.uiWidth = InputImg.Width;
	imgbuf.uiHeight = InputImg.Height;
	imgbuf.ePxlFmt = InputImg.PxlFmt;
	for (i = 0; i < MAX_PLANE_NUM; i++) {
		if (InputImg.PxlAddr[i]) {
			imgbuf.uiPxlAddr[i] = NvtIPC_GetPhyAddr(InputImg.PxlAddr[i]);
			DBG_DUMP("logical:%x   phy:%x\r\n",InputImg.PxlAddr[i],imgbuf.uiPxlAddr[i]);
		}
	}
	memcpy(imgbuf.uiLineOffset, InputImg.LineOffset, sizeof(UINT32)*MAX_PLANE_NUM);
	NvtUctrl_SetConfigData((void *)&imgbuf, sizeof(imgbuf));
	DBG_IND("w=%d,h=%d Fmt=%d,Y=0x%x,U=0x%x,V=0x%x\r\n", imgbuf.uiWidth, imgbuf.uiHeight, imgbuf.ePxlFmt, imgbuf.uiPxlAddr[0], imgbuf.uiPxlAddr[1], imgbuf.uiPxlAddr[2]);
	return 0;
#else
	DBG_ERR("not support\r\n");
	return -1;
#endif
}

STATIC_ASSERT(sizeof(URECT) == sizeof(CodecObjData));

static INT32 param_bcrslt(INT32 argc, char *argv[])
{
#if (IPCAM_BC_FUNC == ENABLE)
	URECT              TargetCoord = {0};
	static CodecBCData  ivsBCData;

	if (argc != 4) {
		DBG_ERR("argc!=4 (%d)\r\n", argc);
		return -1;
	}

	ivsBCData.bUpdated = bc_getprms(BC_RSLT_NEW);

	TargetCoord.x = atoi(argv[0]);
	TargetCoord.y = atoi(argv[1]);
	TargetCoord.w = atoi(argv[2]);
	TargetCoord.h = atoi(argv[3]);

	//DBG_DUMP("bc atrgetcoord %d %d %d %d\r\n",TargetCoord.x,TargetCoord.y,TargetCoord.w,TargetCoord.h);

	ivsBCData.uiObjectNum = bc_gettarget((URECT *)&ivsBCData.sObjectData, &TargetCoord, BC_MAX_OBJ_NUM);

	bc_setprms(BC_RSLT_NEW, 0);
	NvtUctrl_SetConfigData((void *)&ivsBCData, sizeof(ivsBCData));
#if 0
	DBG_IND("uiObjectNum = %d\r\n", ivsBCData.uiObjectNum);
	{
		UINT32      i;
		IVSObjData  *pObjData;
		for (i = 0; i < ivsBCData.uiObjectNum; i++) {
			pObjData = &ivsBCData.sObjectData[i];
			DBG_IND("bc[%d] = x=%d y=%d w=%d h=%d\r\n", i, pObjData->uiCoordinateX, pObjData->uiCoordinateY, pObjData->uiWidth, pObjData->uiHeight);
		}
	}
#endif
	return 0;
#else
	DBG_ERR("not support\r\n");
	return -1;
#endif
}

static INT32 param_bcblkrslt(INT32 argc, char *argv[])
{
#if (IPCAM_BC_FUNC == ENABLE)
	URECT                TargetCoord = {0};
	static CodecBCBLKData  ivsBCBlkData;

	if (argc != 4) {
		DBG_ERR("argc!=4 (%d)\r\n", argc);
		return -1;
	}

	ivsBCBlkData.bUpdated = bc_getprms(BC_RSLT_NEW);

	TargetCoord.x = atoi(argv[0]);
	TargetCoord.y = atoi(argv[1]);
	TargetCoord.w = atoi(argv[2]);
	TargetCoord.h = atoi(argv[3]);
	ivsBCBlkData.uiObjectNum = bc_getblktarget((URECT *)&ivsBCBlkData.sObjectData, &TargetCoord,BC_MAX_BLK_NUM);
	bc_setprms(BC_RSLT_NEW, 0);
	NvtUctrl_SetConfigData((void *)&ivsBCBlkData, sizeof(ivsBCBlkData));
#if 0
	DBG_IND("uiObjectNum = %d\r\n", ivsBCBlkData.uiObjectNum);
	{
		UINT32      i;
		IVSObjData  *pObjData;
		for (i = 0; i < ivsBCBlkData.uiObjectNum; i++) {
			pObjData = &ivsBCBlkData.sObjectData[i];
			DBG_IND("bc[%d] = x=%d y=%d w=%d h=%d\r\n", i, pObjData->uiCoordinateX, pObjData->uiCoordinateY, pObjData->uiWidth, pObjData->uiHeight);
		}
	}
#endif
	return 0;
#else
	DBG_ERR("not support\r\n");
	return -1;
#endif
}

static INT32 param_begin(char *str, INT32 len)
{
	//DBG_IND("get: %s\r\n",str);
	memset(pMovieInfo, 0, sizeof(UCtrlMovieInfoType));
	return 0;
}

#if(IPCAM_FUNC==ENABLE && IPCAM_MODE==ENABLE)
#if (IPCAM_RESET_FPS == ENABLE)
extern void IPCam_dirtyFPS(UINT32 path_index);
#endif
#endif

static INT32 param_end(UTOKEN_END_INFO *p_info)
{

	char retStr[64];

	BOOL   bIsSizeChange = FALSE;
	//int    iCurrMode = System_GetState(SYS_STATE_CURRMODE);
	#if(IPCAM_MODE==ENABLE && IPCAM_FUNC==ENABLE)
	BOOL   bIsRestartMovie = FALSE;
	BOOL   bIsChangeMode  = FALSE;
	#endif

//    BOOL   bIsRestartAudio = FALSE;
#if(IPCAM_MODE== ENABLE)
	UINT32 channel_id = 0;
	// set channel
	if (pMovieInfo->liveview_channel_id.isSet) {
		channel_id = pMovieInfo->liveview_channel_id.index;
	}
	UIAPPIPCAM_ENCODE_CONFIG *pStreamConfig = UIAppIPCam_get_EncodeConfig();
	UIAPPIPCAM_SENSOR_INFO *pSensorInfo     = UIAppIPCam_get_SensorInfo();
	IPCAM_STREAM_INFO *pStreamInfo          = UIAppIPCam_get_streamInfo();
	UIAPPIPCAM_AQ_CONFIG *pAQConfig         = UIAppIPCam_get_AQInfo();
#endif
	if (p_info->err_code < 0) {
		snprintf(retStr, sizeof(retStr), "ERR: parm %s", p_info->failed_name);
		NvtUctrl_SetRetString(retStr);
		DBG_ERR("%s\r\n", retStr);
		return p_info->err_code;
	}

#if 0
	// set parameters
	if (pMovieInfo->recsize.isSet) {
		UI_SetData(FL_MOVIE_SIZE_MENU, pMovieInfo->recsize.index);
	}
#endif

	//set format
	if (pMovieInfo->liveview_vid_enc_fmt.isSet) {
#if(IPCAM_MODE== ENABLE && IPCAM_FUNC==ENABLE)
		if (pStreamConfig[channel_id].codec != pMovieInfo->liveview_vid_enc_fmt.index) {
			bIsSizeChange = TRUE;
			bIsRestartMovie = TRUE;
			if(pStreamConfig[channel_id].codec == IPCAMCODEC_YUV ||
				     pMovieInfo->liveview_vid_enc_fmt.index== IPCAMCODEC_YUV){
				bIsChangeMode = TRUE;

				if(pMovieInfo->liveview_vid_enc_fmt.index == IPCAMCODEC_YUV){
					UIAppIPCam_set_YUVPath(channel_id);
				}
				else{
					UIAppIPCam_set_YUVPath(-1);
				}
			}
			pStreamConfig[channel_id].codec = pMovieInfo->liveview_vid_enc_fmt.index;
			ImageUnit_Begin(&ISF_VdoEnc, 0);
				ImageUnit_SetParam(channel_id, VDOENC_PARAM_CODEC, pStreamConfig[channel_id].codec);
			ImageUnit_End();
		}
#endif

	}

	if(pMovieInfo->liveview_vid_sei.isSet)
	{
#if(IPCAM_MODE== ENABLE && IPCAM_FUNC==ENABLE)
		if(pStreamConfig[channel_id].sSEISetting.bEnable != pMovieInfo->liveview_vid_sei.bEnable)
		{
		            //set SEI setting
            if((pStreamConfig[channel_id].codec == IPCAMCODEC_H264) || (pStreamConfig[channel_id].codec == IPCAMCODEC_H265))
            {
				pStreamConfig[channel_id].sSEISetting.bEnable = pMovieInfo->liveview_vid_sei.bEnable;
                ImageUnit_Begin(&ISF_VdoEnc, 0);
                    ImageUnit_SetParam(channel_id, VDOENC_PARAM_SEI, pStreamConfig[channel_id].sSEISetting.bEnable);
                ImageUnit_End();
            }

		}
#endif
	 }
     if(pMovieInfo->profile.isSet){
#if(IPCAM_MODE== ENABLE && IPCAM_FUNC==ENABLE)

		bIsRestartMovie = TRUE;
		pStreamConfig[channel_id].eProfile = pMovieInfo->profile.index;
		ImageUnit_Begin(&ISF_VdoEnc, 0);
			ImageUnit_SetParam(channel_id+ ISF_OUT_BASE, VDOENC_PARAM_PROFILE, pStreamConfig[channel_id].eProfile);
		ImageUnit_End();

#endif
	 }

	//set preview size
	if (pMovieInfo->liveview_vid_size.isSet) {
#if(IPCAM_MODE== ENABLE && IPCAM_FUNC==ENABLE)
		if (pStreamConfig[channel_id].resolution.w != pMovieInfo->liveview_vid_size.size.w ||
			pStreamConfig[channel_id].resolution.h != pMovieInfo->liveview_vid_size.size.h) {

			pStreamConfig[channel_id].resolution.w = pMovieInfo->liveview_vid_size.size.w;
			pStreamConfig[channel_id].resolution.h = pMovieInfo->liveview_vid_size.size.h;
			bIsSizeChange = TRUE;
			bIsRestartMovie = TRUE;
			/*
			ImageUnit_Begin(&ISF_VdoEnc, 0);
			    ImageUnit_SetVdoImgSize(channel_id + ISF_IN_BASE, pStreamConfig[channel_id].resolution.w, pStreamConfig[channel_id].resolution.h);
			ImageUnit_End();
			*/
		}
#endif
	}
	if (bIsSizeChange) {
#if(IPCAM_MODE== ENABLE && IPCAM_FUNC==ENABLE)
		UINT32            uiWidth = pStreamConfig[channel_id].resolution.w;
		UINT32            uiHeight = pStreamConfig[channel_id].resolution.h;

		ImageApp_IPCam_CfgImgSize(channel_id, uiWidth, uiHeight);
		UINT32 SensorIndex = pStreamInfo[channel_id].sensor;
		UINT32 IMEIndex = pStreamInfo[channel_id].ime;

		if((pSensorInfo[SensorIndex].bCropEnable[IMEIndex] ==TRUE) ){

			UIAPPIPCAM_CROP_INFO *pCropInfo = UIAppIPCam_get_CropInfo();

			IPL_CUR_INPUT_INFO sIPLSensorInfo = {0};
				sIPLSensorInfo.id = SensorIndex;
				IPL_GetCmd(IPL_GET_IPL_CUR_INPUT_INFOR ,&sIPLSensorInfo);




			UINT32 uiScaleCropW = pCropInfo[SensorIndex].sCropInfo[IMEIndex].sCropRECT.w*pCropInfo[SensorIndex].sCropInfo[IMEIndex].uiScaleWidth/pCropInfo[SensorIndex].sCropInfo[IMEIndex].sCropRECT.w;
			UINT32 uiScaleCropH = pCropInfo[SensorIndex].sCropInfo[IMEIndex].sCropRECT.h*pCropInfo[SensorIndex].sCropInfo[IMEIndex].uiScaleHeight/pCropInfo[SensorIndex].sCropInfo[IMEIndex].sCropRECT.h;
			UINT32 uiScaleCropY = pCropInfo[SensorIndex].sCropInfo[IMEIndex].sCropRECT.y*pCropInfo[SensorIndex].sCropInfo[IMEIndex].uiScaleHeight/pCropInfo[SensorIndex].sCropInfo[IMEIndex].sCropRECT.y;
			UINT32 uiScaleCropX = pCropInfo[SensorIndex].sCropInfo[IMEIndex].sCropRECT.x*pCropInfo[SensorIndex].sCropInfo[IMEIndex].uiScaleWidth/pCropInfo[SensorIndex].sCropInfo[IMEIndex].sCropRECT.x;
			UINT32 uiScaleW		= sIPLSensorInfo.in_size_h*pCropInfo[SensorIndex].sCropInfo[IMEIndex].uiScaleWidth/pCropInfo[SensorIndex].sCropInfo[IMEIndex].sCropRECT.w;
			UINT32 uiScaleH		= sIPLSensorInfo.in_size_v*pCropInfo[SensorIndex].sCropInfo[IMEIndex].uiScaleHeight/pCropInfo[SensorIndex].sCropInfo[IMEIndex].sCropRECT.h;


			ImageUnit_Begin(&ISF_UserProc, 0);
				ImageUnit_SetVdoImgSize(channel_id + ISF_IN_BASE, pCropInfo[SensorIndex].sCropInfo[IMEIndex].uiScaleWidth,
					                         pCropInfo[SensorIndex].sCropInfo[IMEIndex].uiScaleHeight);
			ImageUnit_End();

			ImageUnit_Begin(&ISF_VdoEnc, 0);
				ImageUnit_SetVdoImgSize(channel_id + ISF_IN_BASE, pCropInfo[SensorIndex].sCropInfo[IMEIndex].uiScaleWidth,
					                          pCropInfo[SensorIndex].sCropInfo[IMEIndex].uiScaleHeight);
			ImageUnit_End();
			DBG_DUMP("Crop Enable, Scale %dx%d  Crop(%d %d %d %d)\r\n",uiScaleW,uiScaleH,
				uiScaleCropX,uiScaleCropY,uiScaleCropW,uiScaleCropH);

			if(pStreamInfo[channel_id].type == IPCAM_STREAM_MAIN){
				ImageUnit_Begin(&ISF_ImgTrans, 0);
					ImageUnit_SetVdoImgSize(channel_id + ISF_IN_BASE, uiScaleW, uiScaleH);
					ImageUnit_SetVdoPostWindow(channel_id + ISF_IN_BASE,
										uiScaleCropX,
										uiScaleCropY,
										uiScaleCropW,
										uiScaleCropH);
				ImageUnit_End();
			}

		}
#endif

	}

	if(pMovieInfo->liveview_vid_aq.isSet){
		#if(IPCAM_MODE== ENABLE)
			pAQConfig[channel_id].sAQInfo.iAqEnable =pMovieInfo->liveview_vid_aq.sAQInfo.iAqEnable;
			pAQConfig[channel_id].sAQInfo.uiAqStr =pMovieInfo->liveview_vid_aq.sAQInfo.uiAqStr;
			pAQConfig[channel_id].sAQInfo.iMaxSraq =pMovieInfo->liveview_vid_aq.sAQInfo.iMaxSraq;
			pAQConfig[channel_id].sAQInfo.iMinSraq =pMovieInfo->liveview_vid_aq.sAQInfo.iMinSraq;
			pAQConfig[channel_id].sAQInfo.iSraqInitAslog2 =pMovieInfo->liveview_vid_aq.sAQInfo.iSraqInitAslog2;
			pAQConfig[channel_id].sAQInfo.iSraqConstAslog2 =pMovieInfo->liveview_vid_aq.sAQInfo.iSraqConstAslog2;
			#if 0
			DBG_WRN("id:%d\r\n",channel_id);
			DBG_WRN("aq enable:%d\r\n",pAQConfig[channel_id].sAQInfo.iAqEnable);
			DBG_WRN("aq_str:%d\r\n",pAQConfig[channel_id].sAQInfo.uiAqStr);
			DBG_WRN("max_sraq:%d\r\n",pAQConfig[channel_id].sAQInfo.iMaxSraq);
			DBG_WRN("min_sraq:%d\r\n",pAQConfig[channel_id].sAQInfo.iMinSraq);
			DBG_WRN("sraq_init_aslog2:%d\r\n",pAQConfig[channel_id].sAQInfo.iSraqInitAslog2);
			#endif
			ImageUnit_Begin(&ISF_VdoEnc, 0);
				ImageUnit_SetParam(channel_id, VDOENC_PARAM_AQINFO, (UINT32)&pAQConfig[channel_id].sAQInfo);
			ImageUnit_End();
		#endif
	}


	//set encode 3DNR
	if (pMovieInfo->NRSetting.isSet) {
#if(IPCAM_FUNC==ENABLE && IPCAM_MODE==ENABLE)
		if (pStreamConfig[channel_id].sNRSetting.bEnable!= pMovieInfo->NRSetting.bEnable) {
			pStreamConfig[channel_id].sNRSetting.bEnable= pMovieInfo->NRSetting.bEnable;
			bIsRestartMovie = TRUE;
			if (pStreamConfig[channel_id].sNRSetting.bEnable == FALSE) {
				ImageUnit_Begin(&ISF_VdoEnc, 0);
				ImageUnit_SetParam(channel_id, VDOENC_PARAM_3DNR_CB, 0);
				ImageUnit_End();

			} else {
				ImageUnit_Begin(&ISF_VdoEnc, 0);
				ImageUnit_SetParam(channel_id, VDOENC_PARAM_3DNR_CB, (UINT32)IPCamExe_UIDefine_3DNR);
				ImageUnit_End();
			}
		}
#endif


	}

	//set bitrate
#if(IPCAM_FUNC==ENABLE && IPCAM_MODE==ENABLE)
	UIAPPIPCAM_VBR_CONFIG *pVBRConfig = UIAppIPCam_get_VBRInfo();
	UIAPPIPCAM_EVBR_CONFIG *pEVBRConfig = UIAppIPCam_get_EVBRInfo();
	UIAPPIPCAM_CBR_CONFIG *pCBRConfig = UIAppIPCam_get_CBRInfo();
	UIAPPIPCAM_BRC_NONE_CONFIG *pBRCNoneConfig = UIAppIPCam_get_BRCNoneInfo();
	UIAPPIPCAM_FIXQP_CONFIG *pBRCFixQPConfig = UIAppIPCam_get_FixQPInfo();
	UINT32 tmpFPS = 0;
	UINT32 uiCodecFPS =0;
	UINT32 uiIPLFPS = 0;
	IPCAM_SENSOR sensor_index = pStreamInfo[channel_id].sensor;
	tmpFPS = ImageUnit_GetParam(ISF_IPL(sensor_index) ,ISF_IN1, IMAGEPIPE_PARAM_LIVE_INFPS_IMM);
	uiIPLFPS = (SEN_FPS_FMT_CONV2_REAL(tmpFPS) / 1000);
	pVBRConfig[channel_id].vbr_info.uiEnable = 0;
	pCBRConfig[channel_id].cbr_info.uiEnable = 0;
	pEVBRConfig[channel_id].evbr_info.uiEnable =0;
	pBRCFixQPConfig[channel_id].sFixQPInfo.uiEnable = 0;
    if(pMovieInfo->liveview_vid_cbr.isSet == 1){
		DBG_WRN("set cbr info!!\r\n");
		bIsRestartMovie = TRUE;
		pStreamConfig[channel_id].eBrcType = UIAPPIPCAM_BRC_CBR;
		pCBRConfig[channel_id].cbr_info.uiEnable = 1;
		pCBRConfig[channel_id].cbr_info.uiStaticTime = pMovieInfo->liveview_vid_cbr.sCBRInfo.uiStaticTime;
		pCBRConfig[channel_id].cbr_info.uiByteRate = pMovieInfo->liveview_vid_cbr.sCBRInfo.uiByteRate;
		pCBRConfig[channel_id].cbr_info.uiFrameRate = pMovieInfo->liveview_vid_cbr.sCBRInfo.uiFrameRate;
		pCBRConfig[channel_id].cbr_info.uiGOP = pMovieInfo->liveview_vid_cbr.sCBRInfo.uiGOP;
		pCBRConfig[channel_id].cbr_info.uiInitIQp = pMovieInfo->liveview_vid_cbr.sCBRInfo.uiInitIQp;
		pCBRConfig[channel_id].cbr_info.uiMinIQp = pMovieInfo->liveview_vid_cbr.sCBRInfo.uiMinIQp;
		pCBRConfig[channel_id].cbr_info.uiMaxIQp = pMovieInfo->liveview_vid_cbr.sCBRInfo.uiMaxIQp;
		pCBRConfig[channel_id].cbr_info.uiInitPQp = pMovieInfo->liveview_vid_cbr.sCBRInfo.uiInitPQp;
		pCBRConfig[channel_id].cbr_info.uiMinPQp = pMovieInfo->liveview_vid_cbr.sCBRInfo.uiMinPQp;
		pCBRConfig[channel_id].cbr_info.uiMaxPQp = pMovieInfo->liveview_vid_cbr.sCBRInfo.uiMaxPQp;
		pCBRConfig[channel_id].cbr_info.iIPWeight = pMovieInfo->liveview_vid_cbr.sCBRInfo.iIPWeight;
		pCBRConfig[channel_id].cbr_info.uiRowRcEnalbe = pMovieInfo->liveview_vid_cbr.sCBRInfo.uiRowRcEnalbe;
		pCBRConfig[channel_id].cbr_info.uiRowRcQpRange = pMovieInfo->liveview_vid_cbr.sCBRInfo.uiRowRcQpRange;
		pCBRConfig[channel_id].cbr_info.uiRowRcQpStep = pMovieInfo->liveview_vid_cbr.sCBRInfo.uiRowRcQpStep;
		UIAppIPCam_set_CBRInfo(pCBRConfig);
		uiCodecFPS = pCBRConfig[channel_id].cbr_info.uiFrameRate;
		//disable other type

		ImageUnit_Begin(&ISF_VdoEnc, 0);
			////set fps 1/1, for IPL trigger mode
			if (pStreamInfo[channel_id].trig == IPCAM_TRIGGER_DEST) {
				ImageUnit_SetVdoFramerate(channel_id + ISF_IN_BASE, ISF_VDO_FRC(uiCodecFPS, uiIPLFPS));
				ImageUnit_SetParam(channel_id, VDOENC_PARAM_FRAMERATE, uiCodecFPS);
			} else {
				ImageUnit_SetVdoFramerate(channel_id + ISF_IN_BASE, ISF_VDO_FRC(1, 1));
			}
			ImageUnit_SetParam(channel_id, VDOENC_PARAM_CBRINFO, (UINT32)&pCBRConfig[channel_id].cbr_info);
			ImageUnit_SetParam(channel_id, VDOENC_PARAM_VBRINFO, (UINT32)&pVBRConfig[channel_id].vbr_info);
			ImageUnit_SetParam(channel_id, VDOENC_PARAM_EVBRINFO, (UINT32)&pEVBRConfig[channel_id].evbr_info);
			ImageUnit_SetParam(channel_id, VDOENC_PARAM_FIXQPINFO, (UINT32)&pBRCFixQPConfig[channel_id].sFixQPInfo);
			ImageUnit_SetParam(channel_id, VDOENC_PARAM_GOPNUM, pCBRConfig[channel_id].cbr_info.uiGOP);
			ImageUnit_SetParam(channel_id, VDOENC_PARAM_TARGETRATE, pCBRConfig[channel_id].cbr_info.uiByteRate);
			//ImageUnit_SetParam(channel_id, VDOENC_PARAM_BUFRATE, pCBRConfig[channel_id].cbr_info.uiByteRate*2);
		ImageUnit_End();

			#if(FORCE_UNPLUG_RTSP != ENABLE)
				ImageUnit_Begin(&ISF_NetRTSP, 0);
					ImageUnit_SetParam(channel_id+ISF_IN_BASE, NETRTSP_PORT_PARAM_FPS, uiCodecFPS);
				ImageUnit_End();
			#endif
			ImageUnit_Begin(&ISF_UserProc, 0);
			if (pStreamInfo[channel_id].trig == IPCAM_TRIGGER_DEST) {
				ImageUnit_SetVdoFramerate(channel_id+ISF_IN_BASE, ISF_VDO_FRC(1, 1));
			} else {
				ImageUnit_SetVdoFramerate(channel_id+ISF_IN_BASE, ISF_VDO_FRC(uiCodecFPS, uiIPLFPS));
			}
			ImageUnit_End();
			ImageUnit_Begin(&ISF_StreamSender, 0);
				ImageUnit_SetParam(channel_id+ISF_IN_BASE, STMSND_PARAM_SLOTNUM_VIDEO_IMM, IPCAM_STEAMSENDER_V_SLOT);
			ImageUnit_End();


	}
	else if(pMovieInfo->liveview_vid_vbr.isSet == 1){
		bIsRestartMovie = TRUE;
		DBG_WRN("set vbr! =%d\r\n",pMovieInfo->liveview_vid_vbr.sVBRInfo.uiEnable);
		pStreamConfig[channel_id].eBrcType = UIAPPIPCAM_BRC_VBR;
		pVBRConfig[channel_id].vbr_info.uiEnable = 1;
		pVBRConfig[channel_id].vbr_info.uiStaticTime = pMovieInfo->liveview_vid_vbr.sVBRInfo.uiStaticTime;
		pVBRConfig[channel_id].vbr_info.uiByteRate = pMovieInfo->liveview_vid_vbr.sVBRInfo.uiByteRate;
		pVBRConfig[channel_id].vbr_info.uiFrameRate = pMovieInfo->liveview_vid_vbr.sVBRInfo.uiFrameRate;
		pVBRConfig[channel_id].vbr_info.uiGOP = pMovieInfo->liveview_vid_vbr.sVBRInfo.uiGOP;
		pVBRConfig[channel_id].vbr_info.uiInitIQp = pMovieInfo->liveview_vid_vbr.sVBRInfo.uiInitIQp;
		pVBRConfig[channel_id].vbr_info.uiMinIQp = pMovieInfo->liveview_vid_vbr.sVBRInfo.uiMinIQp;
		pVBRConfig[channel_id].vbr_info.uiMaxIQp = pMovieInfo->liveview_vid_vbr.sVBRInfo.uiMaxIQp;
		pVBRConfig[channel_id].vbr_info.uiInitPQp = pMovieInfo->liveview_vid_vbr.sVBRInfo.uiInitPQp;
		pVBRConfig[channel_id].vbr_info.uiMinPQp = pMovieInfo->liveview_vid_vbr.sVBRInfo.uiMinPQp;
		pVBRConfig[channel_id].vbr_info.uiMaxPQp = pMovieInfo->liveview_vid_vbr.sVBRInfo.uiMaxPQp;
		pVBRConfig[channel_id].vbr_info.iIPWeight = pMovieInfo->liveview_vid_vbr.sVBRInfo.iIPWeight;
		pVBRConfig[channel_id].vbr_info.uiChangePos = pMovieInfo->liveview_vid_vbr.sVBRInfo.uiChangePos;
		pVBRConfig[channel_id].vbr_info.uiRowRcEnalbe = pMovieInfo->liveview_vid_vbr.sVBRInfo.uiRowRcEnalbe;
		pVBRConfig[channel_id].vbr_info.uiRowRcQpRange = pMovieInfo->liveview_vid_vbr.sVBRInfo.uiRowRcQpRange;
		pVBRConfig[channel_id].vbr_info.uiRowRcQpStep = pMovieInfo->liveview_vid_vbr.sVBRInfo.uiRowRcQpStep;
		UIAppIPCam_set_VBRInfo(pVBRConfig);
		uiCodecFPS = pVBRConfig[channel_id].vbr_info.uiFrameRate;
		ImageUnit_Begin(&ISF_VdoEnc, 0);
			////set fps 1/1, for IPL trigger mode
			if (pStreamInfo[channel_id].trig == IPCAM_TRIGGER_DEST) {
				ImageUnit_SetVdoFramerate(channel_id + ISF_IN_BASE, ISF_VDO_FRC(uiCodecFPS, uiIPLFPS));
				ImageUnit_SetParam(channel_id, VDOENC_PARAM_FRAMERATE, uiCodecFPS);
			} else {
				ImageUnit_SetVdoFramerate(channel_id + ISF_IN_BASE, ISF_VDO_FRC(1, 1));
			}
			ImageUnit_SetParam(channel_id, VDOENC_PARAM_CBRINFO, (UINT32)&pCBRConfig[channel_id].cbr_info);
			ImageUnit_SetParam(channel_id, VDOENC_PARAM_VBRINFO, (UINT32)&pVBRConfig[channel_id].vbr_info);
			ImageUnit_SetParam(channel_id, VDOENC_PARAM_EVBRINFO, (UINT32)&pEVBRConfig[channel_id].evbr_info);
			ImageUnit_SetParam(channel_id, VDOENC_PARAM_FIXQPINFO, (UINT32)&pBRCFixQPConfig[channel_id].sFixQPInfo);
			ImageUnit_SetParam(channel_id, VDOENC_PARAM_GOPNUM, pVBRConfig[channel_id].vbr_info.uiGOP);
			ImageUnit_SetParam(channel_id, VDOENC_PARAM_TARGETRATE, pVBRConfig[channel_id].vbr_info.uiByteRate);
		//	ImageUnit_SetParam(channel_id, VDOENC_PARAM_BUFRATE, pVBRConfig[channel_id].vbr_info.uiByteRate*2);
		ImageUnit_End();

			#if(FORCE_UNPLUG_RTSP != ENABLE)
			ImageUnit_Begin(&ISF_NetRTSP, 0);
				ImageUnit_SetParam(channel_id+ISF_IN_BASE, NETRTSP_PORT_PARAM_FPS, pVBRConfig[channel_id].vbr_info.uiFrameRate);
			ImageUnit_End();
			#endif
			ImageUnit_Begin(&ISF_UserProc, 0);
			if (pStreamInfo[channel_id].trig == IPCAM_TRIGGER_DEST) {
				ImageUnit_SetVdoFramerate(channel_id+ISF_IN_BASE, ISF_VDO_FRC(1, 1));
			} else {
				ImageUnit_SetVdoFramerate(channel_id+ISF_IN_BASE, ISF_VDO_FRC(uiCodecFPS, uiIPLFPS));
			}
			ImageUnit_End();
			ImageUnit_Begin(&ISF_StreamSender, 0);
				ImageUnit_SetParam(channel_id+ISF_IN_BASE, STMSND_PARAM_SLOTNUM_VIDEO_IMM, IPCAM_STEAMSENDER_V_SLOT);
			ImageUnit_End();


	}
	else if(pMovieInfo->liveview_vid_evbr.isSet == 1){
		bIsRestartMovie = TRUE;
		DBG_WRN("set evbr! =%d\r\n",pMovieInfo->liveview_vid_evbr.sEVBRInfo.uiEnable);

		pStreamConfig[channel_id].eBrcType = UIAPPIPCAM_BRC_EVBR;
		pEVBRConfig[channel_id].evbr_info.uiEnable = 1;
		pEVBRConfig[channel_id].evbr_info.uiStaticTime = pMovieInfo->liveview_vid_evbr.sEVBRInfo.uiStaticTime;
		pEVBRConfig[channel_id].evbr_info.uiByteRate = pMovieInfo->liveview_vid_evbr.sEVBRInfo.uiByteRate;
		pEVBRConfig[channel_id].evbr_info.uiFrameRate = pMovieInfo->liveview_vid_evbr.sEVBRInfo.uiFrameRate;
		pEVBRConfig[channel_id].evbr_info.uiGOP = pMovieInfo->liveview_vid_evbr.sEVBRInfo.uiGOP;
		pEVBRConfig[channel_id].evbr_info.uiKeyPPeriod = pMovieInfo->liveview_vid_evbr.sEVBRInfo.uiKeyPPeriod;
		pEVBRConfig[channel_id].evbr_info.uiInitIQp = pMovieInfo->liveview_vid_evbr.sEVBRInfo.uiInitIQp;
		pEVBRConfig[channel_id].evbr_info.uiMinIQp = pMovieInfo->liveview_vid_evbr.sEVBRInfo.uiMinIQp;
		pEVBRConfig[channel_id].evbr_info.uiMaxIQp = pMovieInfo->liveview_vid_evbr.sEVBRInfo.uiMaxIQp;
		pEVBRConfig[channel_id].evbr_info.uiInitPQp = pMovieInfo->liveview_vid_evbr.sEVBRInfo.uiInitPQp;
		pEVBRConfig[channel_id].evbr_info.uiMinPQp = pMovieInfo->liveview_vid_evbr.sEVBRInfo.uiMinPQp;
		pEVBRConfig[channel_id].evbr_info.uiMaxPQp = pMovieInfo->liveview_vid_evbr.sEVBRInfo.uiMaxPQp;
		pEVBRConfig[channel_id].evbr_info.iIPWeight = pMovieInfo->liveview_vid_evbr.sEVBRInfo.iIPWeight;
		pEVBRConfig[channel_id].evbr_info.iKeyPWeight = pMovieInfo->liveview_vid_evbr.sEVBRInfo.iKeyPWeight;
		pEVBRConfig[channel_id].evbr_info.iMotionAQStrength = pMovieInfo->liveview_vid_evbr.sEVBRInfo.iMotionAQStrength;

		pEVBRConfig[channel_id].evbr_info.uiRowRcEnalbe = pMovieInfo->liveview_vid_evbr.sEVBRInfo.uiRowRcEnalbe;
		pEVBRConfig[channel_id].evbr_info.uiRowRcQpRange = pMovieInfo->liveview_vid_evbr.sEVBRInfo.uiRowRcQpRange;
		pEVBRConfig[channel_id].evbr_info.uiRowRcQpStep = pMovieInfo->liveview_vid_evbr.sEVBRInfo.uiRowRcQpStep;
		pEVBRConfig[channel_id].evbr_info.uiStillFrameCnd = pMovieInfo->liveview_vid_evbr.sEVBRInfo.uiStillFrameCnd;
		pEVBRConfig[channel_id].evbr_info.uiMotionRatioThd = pMovieInfo->liveview_vid_evbr.sEVBRInfo.uiMotionRatioThd;
		pEVBRConfig[channel_id].evbr_info.uiIPsnrCnd = pMovieInfo->liveview_vid_evbr.sEVBRInfo.uiIPsnrCnd;
		pEVBRConfig[channel_id].evbr_info.uiPPsnrCnd = pMovieInfo->liveview_vid_evbr.sEVBRInfo.uiPPsnrCnd;
		pEVBRConfig[channel_id].evbr_info.uiKeyPPsnrCnd = pMovieInfo->liveview_vid_evbr.sEVBRInfo.uiKeyPPsnrCnd;

		#if 0
		DBG_DUMP("uiEnable    	 :%d\r\n",pEVBRConfig[channel_id].evbr_info.uiEnable);
		DBG_DUMP("uiStaticTime   :%d\r\n",pEVBRConfig[channel_id].evbr_info.uiStaticTime);
		DBG_DUMP("uiByteRate   :%d\r\n",pEVBRConfig[channel_id].evbr_info.uiByteRate);
		DBG_DUMP("uiFrameRate   :%d\r\n",pEVBRConfig[channel_id].evbr_info.uiFrameRate);
		DBG_DUMP("uiGOP   :%d\r\n",pEVBRConfig[channel_id].evbr_info.uiGOP);
		DBG_DUMP("uiKeyPPeriod   :%d\r\n",pEVBRConfig[channel_id].evbr_info.uiKeyPPeriod);
		DBG_DUMP("uiInitIQp   :%d\r\n",pEVBRConfig[channel_id].evbr_info.uiInitIQp);
		DBG_DUMP("uiMinIQp   :%d\r\n",pEVBRConfig[channel_id].evbr_info.uiMinIQp);
		DBG_DUMP("uiMaxIQp   :%d\r\n",pEVBRConfig[channel_id].evbr_info.uiMaxIQp);
		DBG_DUMP("uiInitPQp   :%d\r\n",pEVBRConfig[channel_id].evbr_info.uiInitPQp);
		DBG_DUMP("uiMinPQp   :%d\r\n",pEVBRConfig[channel_id].evbr_info.uiMinPQp);
		DBG_DUMP("uiMaxPQp   :%d\r\n",pEVBRConfig[channel_id].evbr_info.uiMaxPQp);
		DBG_DUMP("iIPWeight   :%d\r\n",pEVBRConfig[channel_id].evbr_info.iIPWeight);
		DBG_DUMP("iKeyPWeight   :%d\r\n",pEVBRConfig[channel_id].evbr_info.iKeyPWeight);
		DBG_DUMP("iMotionAQStrength   :%d\r\n",pEVBRConfig[channel_id].evbr_info.iMotionAQStrength);
		DBG_DUMP("uiRowRcEnalbe   :%d\r\n",pEVBRConfig[channel_id].evbr_info.uiRowRcEnalbe);
		DBG_DUMP("uiRowRcQpRange   :%d\r\n",pEVBRConfig[channel_id].evbr_info.uiRowRcQpRange);
		DBG_DUMP("uiRowRcQpStep   :%d\r\n",pEVBRConfig[channel_id].evbr_info.uiRowRcQpStep);
		DBG_DUMP("uiStillFrameCnd   :%d\r\n",pEVBRConfig[channel_id].evbr_info.uiStillFrameCnd);
		DBG_DUMP("uiMotionRatioThd   :%d\r\n",pEVBRConfig[channel_id].evbr_info.uiMotionRatioThd);
		DBG_DUMP("uiIPsnrCnd   :%d\r\n",pEVBRConfig[channel_id].evbr_info.uiIPsnrCnd);
		DBG_DUMP("uiPPsnrCnd   :%d\r\n",pEVBRConfig[channel_id].evbr_info.uiPPsnrCnd);
		DBG_DUMP("uiKeyPPsnrCnd   :%d\r\n",pEVBRConfig[channel_id].evbr_info.uiKeyPPsnrCnd);
		#endif
		uiCodecFPS = pEVBRConfig[channel_id].evbr_info.uiFrameRate;
		UIAppIPCam_set_EVBRInfo(pEVBRConfig);
		UINT32 uiIPLFPS = pSensorInfo[pStreamInfo[channel_id].sensor].uiIMEFPS[pStreamInfo[channel_id].ime];
		ImageUnit_Begin(&ISF_VdoEnc, 0);
			if (pStreamInfo[channel_id].trig == IPCAM_TRIGGER_DEST) {
				ImageUnit_SetVdoFramerate(channel_id + ISF_IN_BASE, ISF_VDO_FRC(uiCodecFPS, uiIPLFPS));
				ImageUnit_SetParam(channel_id, VDOENC_PARAM_FRAMERATE, uiCodecFPS);
			} else {
				ImageUnit_SetVdoFramerate(channel_id + ISF_IN_BASE, ISF_VDO_FRC(1, 1));
			}
			ImageUnit_SetParam(channel_id, VDOENC_PARAM_CBRINFO, (UINT32)&pCBRConfig[channel_id].cbr_info);
			ImageUnit_SetParam(channel_id, VDOENC_PARAM_VBRINFO, (UINT32)&pVBRConfig[channel_id].vbr_info);
			ImageUnit_SetParam(channel_id, VDOENC_PARAM_EVBRINFO, (UINT32)&pEVBRConfig[channel_id].evbr_info);
			ImageUnit_SetParam(channel_id, VDOENC_PARAM_FIXQPINFO, (UINT32)&pBRCFixQPConfig[channel_id].sFixQPInfo);
			ImageUnit_SetParam(channel_id, VDOENC_PARAM_GOPNUM, pEVBRConfig[channel_id].evbr_info.uiGOP);
			ImageUnit_SetParam(channel_id, VDOENC_PARAM_TARGETRATE, pEVBRConfig[channel_id].evbr_info.uiByteRate);
		//	ImageUnit_SetParam(channel_id, VDOENC_PARAM_BUFRATE, pVBRConfig[channel_id].vbr_info.uiByteRate*2);
		ImageUnit_End();

			#if(FORCE_UNPLUG_RTSP != ENABLE)
			ImageUnit_Begin(&ISF_NetRTSP, 0);
				ImageUnit_SetParam(channel_id+ISF_IN_BASE, NETRTSP_PORT_PARAM_FPS, pEVBRConfig[channel_id].evbr_info.uiFrameRate);
			ImageUnit_End();
			#endif
			ImageUnit_Begin(&ISF_UserProc, 0);
			if (pStreamInfo[channel_id].trig == IPCAM_TRIGGER_DEST) {
				ImageUnit_SetVdoFramerate(channel_id+ISF_IN_BASE, ISF_VDO_FRC(1, 1));
			} else {
				ImageUnit_SetVdoFramerate(channel_id+ISF_IN_BASE, ISF_VDO_FRC(uiCodecFPS, uiIPLFPS));
			}
			ImageUnit_End();
			ImageUnit_Begin(&ISF_StreamSender, 0);
				ImageUnit_SetParam(channel_id+ISF_IN_BASE, STMSND_PARAM_SLOTNUM_VIDEO_IMM, IPCAM_STEAMSENDER_V_SLOT);
			ImageUnit_End();
	}
	else if(pMovieInfo->liveview_vid_fixqp.isSet == 1){
		DBG_WRN("set fixQP\r\n");
		bIsRestartMovie = TRUE;
		pStreamConfig[channel_id].eBrcType = UIAPPIPCAM_BRC_FIXQP;
		pBRCFixQPConfig[channel_id].sFixQPInfo.uiEnable = 1;
		pBRCFixQPConfig[channel_id].sFixQPInfo.uiByteRate = pMovieInfo->liveview_vid_fixqp.sFixQPInfo.uiByteRate;
		pBRCFixQPConfig[channel_id].sFixQPInfo.uiFrameRate = pMovieInfo->liveview_vid_fixqp.sFixQPInfo.uiFrameRate;
		pBRCFixQPConfig[channel_id].sFixQPInfo.uiGOP = pMovieInfo->liveview_vid_fixqp.sFixQPInfo.uiGOP;
		pBRCFixQPConfig[channel_id].sFixQPInfo.uiIFixQP = pMovieInfo->liveview_vid_fixqp.sFixQPInfo.uiIFixQP;
		pBRCFixQPConfig[channel_id].sFixQPInfo.uiPFixQP = pMovieInfo->liveview_vid_fixqp.sFixQPInfo.uiPFixQP;
		UIAppIPCam_set_FixQPInfo(pBRCFixQPConfig);
		uiCodecFPS = pBRCFixQPConfig[channel_id].sFixQPInfo.uiFrameRate;
		ImageUnit_Begin(&ISF_VdoEnc, 0);
			////set fps 1/1, for IPL trigger mode
			if (pStreamInfo[channel_id].trig == IPCAM_TRIGGER_DEST) {
				ImageUnit_SetVdoFramerate(channel_id + ISF_IN_BASE, ISF_VDO_FRC(uiCodecFPS, uiIPLFPS));
				ImageUnit_SetParam(channel_id, VDOENC_PARAM_FRAMERATE, uiCodecFPS);
			} else {
				ImageUnit_SetVdoFramerate(channel_id + ISF_IN_BASE, ISF_VDO_FRC(1, 1));
			}
						ImageUnit_SetParam(channel_id, VDOENC_PARAM_CBRINFO, (UINT32)&pCBRConfig[channel_id].cbr_info);
			ImageUnit_SetParam(channel_id, VDOENC_PARAM_VBRINFO, (UINT32)&pVBRConfig[channel_id].vbr_info);
			ImageUnit_SetParam(channel_id, VDOENC_PARAM_EVBRINFO, (UINT32)&pEVBRConfig[channel_id].evbr_info);
			ImageUnit_SetParam(channel_id, VDOENC_PARAM_FIXQPINFO, (UINT32)&pBRCFixQPConfig[channel_id].sFixQPInfo);
			ImageUnit_SetParam(channel_id, VDOENC_PARAM_GOPNUM, pBRCFixQPConfig[channel_id].sFixQPInfo.uiGOP);
			ImageUnit_SetParam(channel_id, VDOENC_PARAM_TARGETRATE, pBRCFixQPConfig[channel_id].sFixQPInfo.uiByteRate);
		//	ImageUnit_SetParam(channel_id, VDOENC_PARAM_BUFRATE, pBRCFixQPConfig[channel_id].sFixQPInfo.uiByteRate*2);
		ImageUnit_End();


			#if(FORCE_UNPLUG_RTSP != ENABLE)
			ImageUnit_Begin(&ISF_NetRTSP, 0);
				ImageUnit_SetParam(channel_id+ISF_IN_BASE, NETRTSP_PORT_PARAM_FPS, pBRCFixQPConfig[channel_id].sFixQPInfo.uiFrameRate);
			ImageUnit_End();
			#endif
			ImageUnit_Begin(&ISF_UserProc, 0);
			if (pStreamInfo[channel_id].trig == IPCAM_TRIGGER_DEST) {
				ImageUnit_SetVdoFramerate(channel_id+ISF_IN_BASE, ISF_VDO_FRC(1, 1));
			} else {
				ImageUnit_SetVdoFramerate(channel_id+ISF_IN_BASE, ISF_VDO_FRC(uiCodecFPS, uiIPLFPS));
			}
			ImageUnit_End();
			ImageUnit_Begin(&ISF_StreamSender, 0);
				ImageUnit_SetParam(channel_id+ISF_IN_BASE, STMSND_PARAM_SLOTNUM_VIDEO_IMM, IPCAM_STEAMSENDER_V_SLOT);
			ImageUnit_End();


	}
	else if(pMovieInfo->liveview_vid_brc_none.isSet ==1){
		DBG_WRN("set none brc\r\n");
		bIsRestartMovie = TRUE;
		pStreamConfig[channel_id].eBrcType = UIAPPIPCAM_BRC_NONE;

		pBRCNoneConfig[channel_id].uiFrameRate =pMovieInfo->liveview_vid_brc_none.uiFrameRate;
		pBRCNoneConfig[channel_id].uiGOP =pMovieInfo->liveview_vid_brc_none.uiGOP;
		pBRCNoneConfig[channel_id].uiTargetByterate =pMovieInfo->liveview_vid_brc_none.uiTargetByterate;
		uiCodecFPS = pBRCNoneConfig[channel_id].uiFrameRate;
		ImageUnit_Begin(&ISF_VdoEnc, 0);
			////set fps 1/1, for IPL trigger mode
			if (pStreamInfo[channel_id].trig == IPCAM_TRIGGER_DEST) {
				ImageUnit_SetVdoFramerate(channel_id + ISF_IN_BASE, ISF_VDO_FRC(uiCodecFPS, uiIPLFPS));
				ImageUnit_SetParam(channel_id, VDOENC_PARAM_FRAMERATE, uiCodecFPS);
			} else {
				ImageUnit_SetVdoFramerate(channel_id + ISF_IN_BASE, ISF_VDO_FRC(1, 1));
			}
			ImageUnit_SetParam(channel_id, VDOENC_PARAM_CBRINFO, (UINT32)&pCBRConfig[channel_id].cbr_info);
			ImageUnit_SetParam(channel_id, VDOENC_PARAM_VBRINFO, (UINT32)&pVBRConfig[channel_id].vbr_info);
			ImageUnit_SetParam(channel_id, VDOENC_PARAM_EVBRINFO, (UINT32)&pEVBRConfig[channel_id].evbr_info);
			ImageUnit_SetParam(channel_id, VDOENC_PARAM_FIXQPINFO, (UINT32)&pBRCFixQPConfig[channel_id].sFixQPInfo);
			ImageUnit_SetParam(channel_id, VDOENC_PARAM_GOPNUM, pBRCNoneConfig[channel_id].uiGOP);
			ImageUnit_SetParam(channel_id, VDOENC_PARAM_TARGETRATE, pBRCNoneConfig[channel_id].uiTargetByterate);
		//	ImageUnit_SetParam(channel_id, VDOENC_PARAM_BUFRATE, pBRCNoneConfig[channel_id].uiTargetByterate*2);
		ImageUnit_End();


			#if(FORCE_UNPLUG_RTSP != ENABLE)
			ImageUnit_Begin(&ISF_NetRTSP, 0);
				ImageUnit_SetParam(channel_id+ISF_IN_BASE, NETRTSP_PORT_PARAM_FPS, pBRCNoneConfig[channel_id].uiFrameRate);
			ImageUnit_End();
			#endif
			ImageUnit_Begin(&ISF_UserProc, 0);
			if (pStreamInfo[channel_id].trig == IPCAM_TRIGGER_DEST) {
				ImageUnit_SetVdoFramerate(channel_id+ISF_IN_BASE, ISF_VDO_FRC(1, 1));
			} else {
				ImageUnit_SetVdoFramerate(channel_id+ISF_IN_BASE, ISF_VDO_FRC(uiCodecFPS, uiIPLFPS));
			}
			ImageUnit_End();
			ImageUnit_Begin(&ISF_StreamSender, 0);
				ImageUnit_SetParam(channel_id+ISF_IN_BASE, STMSND_PARAM_SLOTNUM_VIDEO_IMM, IPCAM_STEAMSENDER_V_SLOT);
			ImageUnit_End();


	}


#endif
#if(IPCAM_FUNC==ENABLE && IPCAM_MODE==ENABLE)
	// rotate

	if(pMovieInfo->rotate.isSet == 1){
		bIsRestartMovie = TRUE;
		pStreamConfig[channel_id].eRotateType = pMovieInfo->rotate.index;
		if(pMovieInfo->rotate.index == UIAPPIPCAM_RT_BUILDIN_90){
			ImageUnit_Begin(&ISF_VdoEnc, 0);
				ImageUnit_SetVdoDirection(channel_id+ISF_IN_BASE,ISF_VDO_DIR_ROTATE_90);
			ImageUnit_End();
		}
		else if(pMovieInfo->rotate.index == UIAPPIPCAM_RT_BUILDIN_270){
			ImageUnit_Begin(&ISF_VdoEnc, 0);
				ImageUnit_SetVdoDirection(channel_id+ISF_IN_BASE,ISF_VDO_DIR_ROTATE_270);
			ImageUnit_End();
		}
		else if(pMovieInfo->rotate.index == UIAPPIPCAM_RT_NONE){


			ImageUnit_Begin(&ISF_VdoEnc, 0);
				ImageUnit_SetVdoDirection(channel_id+ISF_IN_BASE,ISF_VDO_DIR_ROTATE_0);
				ImageUnit_SetVdoImgSize(channel_id+ISF_IN_BASE,pStreamConfig[channel_id].resolution.w,pStreamConfig[channel_id].resolution.h);
			ImageUnit_End();
			ImageUnit_Begin(&ISF_UserProc, 0);
				ImageUnit_SetVdoDirection(ISF_IN_BASE+channel_id, ISF_VDO_DIR_NONE);
			ImageUnit_End();
		}
		else if(pMovieInfo->rotate.index == UIAPPIPCAM_RT_EXTEND_90){
			ImageUnit_Begin(&ISF_UserProc, 0);
				ImageUnit_SetVdoDirection(ISF_IN_BASE+channel_id, ISF_VDO_DIR_ROTATE_90);
			ImageUnit_End();
			ImageUnit_Begin(&ISF_VdoEnc, 0);
				ImageUnit_SetVdoImgSize(channel_id+ISF_IN_BASE,pStreamConfig[channel_id].resolution.h,pStreamConfig[channel_id].resolution.w);
			ImageUnit_End();
		}
		else if(pMovieInfo->rotate.index == UIAPPIPCAM_RT_EXTEND_180){
			ImageUnit_Begin(&ISF_UserProc, 0);
				ImageUnit_SetVdoDirection(ISF_IN_BASE+channel_id, ISF_VDO_DIR_ROTATE_180);
			ImageUnit_End();
			ImageUnit_Begin(&ISF_VdoEnc, 0);
				ImageUnit_SetVdoImgSize(channel_id+ISF_IN_BASE,pStreamConfig[channel_id].resolution.w,pStreamConfig[channel_id].resolution.h);
			ImageUnit_End();
		}
		else if(pMovieInfo->rotate.index == UIAPPIPCAM_RT_EXTEND_270){
			ImageUnit_Begin(&ISF_UserProc, 0);
				ImageUnit_SetVdoDirection(ISF_IN_BASE+channel_id, ISF_VDO_DIR_ROTATE_270);
			ImageUnit_End();
			ImageUnit_Begin(&ISF_VdoEnc, 0);
				ImageUnit_SetVdoImgSize(channel_id+ISF_IN_BASE,pStreamConfig[channel_id].resolution.h,pStreamConfig[channel_id].resolution.w);
			ImageUnit_End();
		}
		else{
			DBG_ERR("get error rotate index:%d\r\n",pMovieInfo->rotate.index);

		}

	}

#endif

#if(IPCAM_FUNC==ENABLE && IPCAM_MODE==ENABLE)
	if (pMovieInfo->liveview_vid_svc_t.isSet) {
		bIsRestartMovie = TRUE;

		pStreamConfig[channel_id].eSVCType = pMovieInfo->liveview_vid_svc_t.index;
		ImageUnit_Begin(&ISF_VdoEnc, 0);

			ImageUnit_SetParam(channel_id, VDOENC_PARAM_SVC, pStreamConfig[channel_id].eSVCType);

		ImageUnit_End();
	}

	if(pMovieInfo->LTRset.isSet){
		bIsRestartMovie = TRUE;
		pStreamConfig[channel_id].sLTRInfo.uiLTRInterval = pMovieInfo->LTRset.uiLTRInterval;
		pStreamConfig[channel_id].sLTRInfo.uiLTRPreRef   = pMovieInfo->LTRset.uiLTRPreRef;
		ImageUnit_Begin(&ISF_VdoEnc, 0);

			ImageUnit_SetParam(channel_id, VDOENC_PARAM_LTR, (UINT32)&pStreamConfig[channel_id].sLTRInfo);

		ImageUnit_End();
	}
#endif




#if 0
	if (pMovieInfo->liveview_crop.isSet) {
		MOVIE_ENCODE_CROP EncodeCrop;
		UINT32 CurrentWidth = GetMovieSizeWidth_2p(channel_id, 0);
		UINT32 CurrentHeight = GetMovieSizeHeight_2p(channel_id, 0);

		bIsCropChange = TRUE;
		EncodeCrop.bCropEnable = pMovieInfo->liveview_crop.bCropEnable;
		//#NT#2016/03/30#David Tsai -begin
		//#NT#Add cropping window position check, avoid cropping area overflow
		if (CROP_SOURCE_W < pMovieInfo->liveview_crop.uiCoordinateX + CurrentWidth) {
			EncodeCrop.uiCoordinateX = CROP_SOURCE_W - CurrentWidth - 1;
		} else {
			EncodeCrop.uiCoordinateX = pMovieInfo->liveview_crop.uiCoordinateX;
		}

		if (CROP_SOURCE_H < pMovieInfo->liveview_crop.uiCoordinateY + CurrentHeight) {
			EncodeCrop.uiCoordinateY = CROP_SOURCE_H - CurrentHeight - 1;
		} else {
			EncodeCrop.uiCoordinateY = pMovieInfo->liveview_crop.uiCoordinateY;
		}
		//#NT#2016/03/30#David Tsai -end
		SetMovieCrop_2p(channel_id, &EncodeCrop);
		bIsRestartMovie = TRUE;
	}
#endif
#if 0
	if (pMovieInfo->liveview_aud_enc_fmt.isSet) {
		switch (pMovieInfo->liveview_aud_enc_fmt.index) {
		case 0:
			Movie_SetAudCodec(MOVAUDENC_ULAW);
			bIsRestartAudio = TRUE;
			break;
		case 1:
			Movie_SetAudCodec(MOVAUDENC_AAC);
			bIsRestartAudio = TRUE;
			break;
		case 2:
			Movie_SetAudCodec(MOVAUDENC_ALAW);
			bIsRestartAudio = TRUE;
			break;
		case 3:
			Movie_SetAudCodec(MOVAUDENC_PPCM);
			bIsRestartAudio = TRUE;
			break;
		}
	}
#endif
#if 0
	if (pMovieInfo->liveview_aud_samplerate.isSet) {
		Movie_SetAudSampleRate(pMovieInfo->liveview_aud_samplerate.index * 1000);
		bIsRestartAudio = TRUE;
	}
#endif

#if 0
	if (pMovieInfo->liveview_aud_channel.isSet) {
		switch (pMovieInfo->liveview_aud_channel.index) {
		case 0:
			Movie_SetAudChannel(MOVREC_AUDTYPE_LEFT);
			bIsRestartAudio = TRUE;
			break;
		case 1:
			Movie_SetAudChannel(MOVREC_AUDTYPE_RIGHT);
			bIsRestartAudio = TRUE;
			break;
		case 2:
			Movie_SetAudChannel(MOVREC_AUDTYPE_STEREO);
			bIsRestartAudio = TRUE;
			break;
		case 3:
			Movie_SetAudChannel(MOVREC_AUDTYPE_MONO);
			bIsRestartAudio = TRUE;
			break;
		}
	}
#endif

#if 0
	if (pMovieInfo->wdr.isSet) {
		UI_SetData(FL_MOVIE_WDR_MENU, pMovieInfo->wdr.index);
	}
	if (pMovieInfo->hdr.isSet) {
		UI_SetData(FL_MOVIE_HDR_MENU, pMovieInfo->hdr.index);
	}
	if (pMovieInfo->rsc.isSet) {
		UI_SetData(FL_MovieRSCIndex_MENU, pMovieInfo->rsc.index);
	}

	if (pMovieInfo->cycrec.isSet) {
		UctrlMain_SendEvent(NVTEVT_EXE_CYCLIC_REC,           1,  pMovieInfo->cycrec.index);
	}
	if (pMovieInfo->ev.isSet) {
		UctrlMain_SendEvent(NVTEVT_EXE_MOVIE_EV,             1,  pMovieInfo->ev.index);
	}
	if (pMovieInfo->motion.isSet) {
		UctrlMain_SendEvent(NVTEVT_EXE_MOTION_DET,           1,  pMovieInfo->motion.index);
	}
	if (pMovieInfo->audio.isSet) {
		UctrlMain_SendEvent(NVTEVT_EXE_MOVIE_AUDIO,          1,  pMovieInfo->audio.index);
	}
	if (pMovieInfo->dstamp.isSet) {
		UctrlMain_SendEvent(NVTEVT_EXE_MOVIE_DATE_IMPRINT,   1,  pMovieInfo->dstamp.index);
	}
	if (pMovieInfo->gsensor.isSet) {
		UctrlMain_SendEvent(NVTEVT_EXE_GSENSOR,              1,  pMovieInfo->gsensor.index);
	}
	if (pMovieInfo->ircut.isSet) {
		UctrlMain_SendEvent(NVTEVT_EXE_MOVIE_IR_CUT,         1,  pMovieInfo->ircut.index);
	}
	if (pMovieInfo->rotate.isSet) {
		UctrlMain_SendEvent(NVTEVT_EXE_MOVIE_SENSOR_ROTATE,  1,  pMovieInfo->rotate.index);
	}
	if (pMovieInfo->murgpro.isSet) {
		UctrlMain_SendEvent(NVTEVT_EXE_MOVIE_PROTECT_MANUAL, 1,  pMovieInfo->murgpro.index);
	}
	if (pMovieInfo->aurgpro.isSet) {
		UctrlMain_SendEvent(NVTEVT_EXE_MOVIE_PROTECT_AUTO,   1,  pMovieInfo->aurgpro.index);
	}
#if(PHOTO_MODE==ENABLE)
	if (pMovieInfo->dualcam.isSet) {
		UctrlMain_SendEvent(NVTEVT_EXE_DUALCAM,     1,  pMovieInfo->dualcam.index);
	}
#endif

	if (pMovieInfo->timelap.isSet) {
		SysSetFlag(FL_MOVIE_TIMELAPSE_REC_MENU, pMovieInfo->timelap.index);
	}

	if (pMovieInfo->pim.isSet) {
		SysSetFlag(FL_MOVIE_PIM, pMovieInfo->pim.index);
	}
#endif
#if 0
	if (pMovieInfo->aout_channel.isSet) {
		bIsRestartAudio = TRUE;
	}
#endif







	#if(IPCAM_FUNC==ENABLE && IPCAM_MODE==ENABLE)
	if(bIsChangeMode){
		Ux_SendEvent(0,NVTEVT_SYSTEM_MODE,1,System_GetState(SYS_STATE_CURRMODE));
	}
	else if (bIsRestartMovie) {
		UctrlMain_Lock();
		//now using change mode to reset parameters

		ImageApp_IPCam_reset_path(channel_id);
		#if (IPCAM_RESET_FPS == ENABLE)
	    if((pMovieInfo->liveview_vid_cbr.isSet == 1)
		|| (pMovieInfo->liveview_vid_vbr.isSet == 1)
		|| (pMovieInfo->liveview_vid_fixqp.isSet == 1)
		|| (pMovieInfo->liveview_vid_evbr.isSet == 1)
		|| (pMovieInfo->liveview_vid_brc_none.isSet ==1)) {
			//force to update dynamic FPS
			IPCam_dirtyFPS(channel_id);
		}

		#endif
		UctrlMain_UnLock();

	}
	#endif
#if 0
	if (bIsRestartAudio) {
		UINT32 g_uiAudSampleRate = 32000;
		UINT32 g_uiAudCodec = MOVAUDENC_AAC;
		UINT32 g_uiAudChannel = MOVREC_AUDTYPE_RIGHT;


		g_uiAudCodec = Movie_GetAudCodec();
		g_uiAudSampleRate = Movie_GetAudSampleRate();
		g_uiAudChannel = Movie_GetAudChannel();

		//#NT#2016/08/29#HM Tseng -begin
		//#NT#For audio play testing mode
#if (AUDIO_PLAY_FUNCTION == ENABLE && AUDIO_PLAY_TEST == DISABLE)
		System_AudioOutStop();//stop output
#endif
		//#NT#2016/08/29#HM Tseng -end
		VSMediaRecAudClose();
		Delay_DelayMs(40);
		MovRec_UpdateParam(MOVREC_RECPARAM_AUD_CODEC, g_uiAudCodec, 0, 0);
		MovRec_UpdateParam(MOVREC_RECPARAM_AUD_SAMPLERATE, g_uiAudSampleRate, 0, 0);
		//#NT#2016/08/29#HM Tseng -begin
		//#NT#2016/09/30#David Tsai -begin
		//#NT# Switch audio source
		switch (g_uiAudChannel) {
		case MOVREC_AUDTYPE_MONO:
			MovRec_ChangeParameter(MOVREC_RECPARAM_AUD_SRC, AUDIO_CH_MONO, 0, 0);
			break;
		case MOVREC_AUDTYPE_RIGHT:
			MovRec_ChangeParameter(MOVREC_RECPARAM_AUD_SRC, AUDIO_CH_RIGHT, 0, 0);
			break;
		case MOVREC_AUDTYPE_STEREO:
			MovRec_ChangeParameter(MOVREC_RECPARAM_AUD_SRC, AUDIO_CH_STEREO, 0, 0);
			break;
		case MOVREC_AUDTYPE_LEFT:
		default:
			MovRec_ChangeParameter(MOVREC_RECPARAM_AUD_SRC, AUDIO_CH_LEFT, 0, 0);
			break;
		}
		//#NT#2016/09/30#David Tsai -end
		//#NT#For audio play testing mode
#if (AUDIO_PLAY_FUNCTION == ENABLE && AUDIO_PLAY_TEST == DISABLE)
		//#NT#2016/12/22#HM Tseng -begin
		//#NT# Support AEC function
#if (AUDIO_AEC_FUNC == ENABLE)
		if (g_uiAudChannel == MOVREC_AUDTYPE_STEREO) {
			NvtAec_SetConfig(NVTAEC_CONFIG_ID_RECORD_CH_NO, 2);
		} else {
			NvtAec_SetConfig(NVTAEC_CONFIG_ID_RECORD_CH_NO, 1);
		}
#endif
		//#NT#2016/12/22#HM Tseng -end
		//#NT#2016/09/08#HM Tseng -begin
		//#NT#Support audio channel
		UINT32 aout_channel = 0;
		if (pMovieInfo->aout_channel.isSet) {
			aout_channel = pMovieInfo->aout_channel.index;
		} else {
			aout_channel = System_GetAudioChannel();
		}
		System_SetAudioParam(0, g_uiAudSampleRate, aout_channel, 16);
		//#NT#2016/09/08#HM Tseng -end
#endif
#if (AUDIO_PLAY_FUNCTION == ENABLE && AUDIO_PLAY_TEST == DISABLE)
		System_AudioOutStart();//start output
#endif
		//#NT#2016/08/29#HM Tseng -end

		//#NT#2016/04/07#Charlie Chang -begin
		//#NT# pause resume ImageStream when audio config changed

		UctrlMain_Lock();
		int i = 0;
		for (i = 0; i < IPCAM_STATE_MAX_ID ; i++) {
			ImageApp_IPCam_reset_path(i);
		}
		UctrlMain_UnLock();
		//#NT#2016/04/07#Charlie Chang -end

		VSWA_Record_OpenAll();
	}
#endif
#if 0
	// record
	if (pMovieInfo->start.isSet) {
		if (!bUctrlRecStart) {
			bUctrlRecStart = TRUE;
			// start record
			Ux_PostEvent(NVTEVT_EXE_MOVIE_REC_START, 0);
		}
	}
#endif
#if 0
	// stop
	if (pMovieInfo->stop.isSet) {
		if (bUctrlRecStart) {
			bUctrlRecStart = FALSE;
			Ux_PostEvent(NVTEVT_EXE_MOVIE_REC_STOP, 0);

		}
	}
#endif


	return 0;
}

#if _TODO
static INT32 convert_motion_level(MD_LEVEL level)
{


	if (level == MD_LEVEL_N5) {
		return 0;
	} else if (level == MD_LEVEL_N4) {
		return 1;
	} else if (level == MD_LEVEL_N3) {
		return 2;
	} else if (level == MD_LEVEL_N2) {
		return 3;
	} else if (level == MD_LEVEL_N1) {
		return 4;
	} else if (level == MD_LEVEL_NORMAL) {
		return 5;
	} else if (level == MD_LEVEL_P1) {
		return 6;
	} else if (level == MD_LEVEL_P2) {
		return 7;
	} else if (level == MD_LEVEL_P3) {
		return 8;
	} else if (level == MD_LEVEL_P4) {
		return 9;
	} else if (level == MD_LEVEL_P5) {
		return 10;
	} else {
		DBG_ERR("convert error level=%d\r\n", level);
		return 5;
	}

	return 5;

}
#endif
//#NT#2016/11/01#Charlie Chang -begin
//#NT# support motion detect start function
static INT32 param_motion_start(INT32 argc, char *argv[])
{
#if (IPCAM_FUNC == ENABLE)
	if (argc >= 2) {
		//UINT32 MD_ID = atoi(argv[0]);
		UINT32 enable = atoi(argv[1]);


		if (enable == 0) {
			Movie_MDLock(FALSE);
			DBG_DUMP("stop notify motion event!\r\n");
		} else {
			Movie_MDLock(TRUE);
			DBG_DUMP("start notify motion event!\r\n");
		}

	}
#endif
	return 0;
}
//#NT#2016/11/01#Charlie Chang -end
static INT32 param_motion_get(INT32 argc, char *argv[])
{
#if _TODO
	MD_INFO Info;
	MD_GetInfo(&Info);
	INT32 level = convert_motion_level(Info.Level);
	INT8 Wnd[NVT_MD_WND_Y][NVT_MD_WND_X];
	char response_buf[1024] = {0};
	UINT32 buf_len = 0;
	INT32 i = 0, j = 0;
	INT32 Wnd_value[NVT_MD_WND_Y];



	memcpy(Wnd, Info.WeightWin, sizeof(UINT8)*NVT_MD_WND_X * NVT_MD_WND_Y);
	DBG_DUMP("motion windows!!!\r\n");
	for (i = 0 ; i < NVT_MD_WND_Y; i++) {
		INT32 tmp_value = 0;
		for (j = 0; j < NVT_MD_WND_X; j++) {

			if (Wnd[i][j] == 1) {
				tmp_value = tmp_value + (1 << j);
			}
			DBG_DUMP("%d ", Wnd[i][j]);
		}
		DBG_DUMP("\r\n");
		Wnd_value[i] = tmp_value;

	}
	if (level == 10) {
		buf_len = snprintf(response_buf, sizeof(response_buf) - 1, "level:High wnd:");
	} else if (level == 0) {
		buf_len = snprintf(response_buf, sizeof(response_buf) - 1, "level:Low wnd:");
	} else {
		buf_len = snprintf(response_buf, sizeof(response_buf) - 1, "level:%d wnd:", level);
	}
	for (i = 0; i < NVT_MD_WND_Y; i++) {
		if (i == (NVT_MD_WND_Y - 1)) {
			buf_len += snprintf(response_buf + buf_len, sizeof(response_buf) - 1, "%d", Wnd_value[i]);
		} else {
			buf_len += snprintf(response_buf + buf_len, sizeof(response_buf) - 1, "%d,", Wnd_value[i]);
		}
	}

	NvtUctrl_SetRetString(response_buf);
#endif
	return 0;
}


static INT32 param_motion_set(INT32 argc, char *argv[])
{
#if _TODO
	INT32 i = 0, j = 0;
	UINT8 md_wnd[NVT_MD_WND_Y][NVT_MD_WND_X] = {
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}
	};
#if(IPCAM_FUNC ==ENABLE)
	BOOL init_flag = FALSE;
	UINT32 count = 0;
	do {
		init_flag = ImageApp_IPCam_GetConfig(IPCAM_CFG_INIT_FLAG);
		if (init_flag == FALSE) {
			Delay_DelayMs(100);
			DBG_DUMP("motion detect process not ready, please wait\r\n");
			count++;
		}
		if (count > 10) {
			DBG_ERR("motion detect process init fail\r\n");
			return -1;
		}
	} while (init_flag == FALSE);

#endif
	//////////////////////////////////////////////////////////////////
	////first parameter : ID  second : windows config     third: level
	//////////////////////////////////////////////////////////////////
	if (argc != 3) {
		DBG_ERR("argc!=3 (%d)\r\n", argc);
		return -1;
	}
	INT32 md_id = atoi(argv[0]);
	if (md_id < 0) {
		DBG_ERR("md_id error md_id=%d\r\n", md_id);
	}
	MD_LEVEL md_level;
	if (!strcmp(argv[2], "High")) {
		md_level = MD_LEVEL_P5;
	} else if (!strcmp(argv[2], "9")) {
		md_level = MD_LEVEL_P4;
	} else if (!strcmp(argv[2], "8")) {
		md_level = MD_LEVEL_P3;
	} else if (!strcmp(argv[2], "7")) {
		md_level = MD_LEVEL_P2;
	} else if (!strcmp(argv[2], "6")) {
		md_level = MD_LEVEL_P1;
	} else if (!strcmp(argv[2], "5")) {
		md_level = MD_LEVEL_NORMAL;
	} else if (!strcmp(argv[2], "4")) {
		md_level = MD_LEVEL_N1;
	} else if (!strcmp(argv[2], "3")) {
		md_level = MD_LEVEL_N2;
	} else if (!strcmp(argv[2], "2")) {
		md_level = MD_LEVEL_N3;
	} else if (!strcmp(argv[2], "1")) {
		md_level = MD_LEVEL_N4;
	} else if (!strcmp(argv[2], "Low")) {
		md_level = MD_LEVEL_N5;
	} else {
		DBG_ERR("level error  argv[2]=%s\n", argv[2]);
		md_level = MD_LEVEL_NORMAL;
	}

	MD_SetLevel(md_id, md_level);

	char *token = NULL;
	INT32 wnd_index = 0;
	INT32 wnd_index2 = 0;
	for (i = 0; i < NVT_MD_WND_Y ; i++) {
		if (i == 0) {
			token = strtok(argv[1], ",");
		} else {
			token = strtok(NULL, ",");
		}

		wnd_index = atoi(token);
		for (j = 0 ; j < NVT_MD_WND_X ; j++) {
			wnd_index2 = (1 << j);

			if ((wnd_index2 & wnd_index) == wnd_index2) {
				md_wnd[i][j] = 1;
			}
		}
	}
	MD_SetDetWin(md_id, &md_wnd[0][0]);
	NvtUctrl_SetRetString("OK");
#endif
	return 0;
}


static INT32 param_disppath(INT32 argc, char *argv[])
{
	#if(IPCAM_FUNC == ENABLE)
		if (argc != 1) {
			DBG_ERR("argc!=1 (%d)\r\n", argc);
			return -1;
		}
		INT32 path = atoi(argv[0]);

		if (path < -1 || path >= IPCAM_PATH_MAX_ID) {

			DBG_ERR("set disp path error path =%d\r\n", path);
			return -1;
		}
		UIAppIPCam_set_DisplayPath(path);
		Ux_SendEvent(0,NVTEVT_SYSTEM_MODE,1,System_GetState(SYS_STATE_CURRMODE));
		//Ux_PostEvent(NVTEVT_SYSTEM_MODE, 1, System_GetState(SYS_STATE_CURRMODE));
	#else
		DBG_ERR("not support!!\r\n");
	#endif
	return 0;
}

static INT32 param_bc_set(INT32 argc, char *argv[])
{
#if (IPCAM_FUNC == ENABLE && IPCAM_BC_FUNC == ENABLE)
	BCParam bc_param = {0};
	NvtUctrl_GetConfigData((void *)&bc_param, sizeof(BCParam));
	Movie_SetBcParam(&bc_param);
#endif
	return 0;
}
static INT32 param_vqa_set(INT32 argc, char *argv[])
{
#if (IPCAM_FUNC == ENABLE)
	IVSVQAParam sIVSVQAParam = {0};
	NvtUctrl_GetConfigData((void *)&sIVSVQAParam, sizeof(IVSVQAParam));
	vqa_param_t vqa_param = {0};

	//enable
	vqa_param.enable_param.en_too_light = sIVSVQAParam.enable_param.en_too_light;
	vqa_param.enable_param.en_too_dark = sIVSVQAParam.enable_param.en_too_dark;
	vqa_param.enable_param.en_blur = sIVSVQAParam.enable_param.en_blur;
	vqa_param.enable_param.en_auto_adj_param = sIVSVQAParam.enable_param.en_auto_adj_param;
	vqa_param.enable_param.en_ref_md = sIVSVQAParam.enable_param.en_ref_md;
	//global
	vqa_param.global_param.height = sIVSVQAParam.global_param.height;
	vqa_param.global_param.width = sIVSVQAParam.global_param.width;
	vqa_param.global_param.mb_y_size = sIVSVQAParam.global_param.mb_y_size;
	vqa_param.global_param.mb_x_size = sIVSVQAParam.global_param.mb_x_size;
	vqa_param.global_param.auto_adj_period = sIVSVQAParam.global_param.auto_adj_period;
	vqa_param.global_param.g_alarm_frame_num = sIVSVQAParam.global_param.g_alarm_frame_num;
	//overexposure
	vqa_param.light_param.too_light_strength_th = sIVSVQAParam.light_param.too_light_strength_th;
	vqa_param.light_param.too_light_cover_th = sIVSVQAParam.light_param.too_light_cover_th;
	vqa_param.light_param.too_light_alarm_times = sIVSVQAParam.light_param.too_light_alarm_times;
	//underexposure
	vqa_param.light_param.too_dark_strength_th = sIVSVQAParam.light_param.too_dark_strength_th;
	vqa_param.light_param.too_dark_cover_th = sIVSVQAParam.light_param.too_dark_cover_th;
	vqa_param.light_param.too_dark_alarm_times = sIVSVQAParam.light_param.too_dark_alarm_times;
	//blur
	vqa_param.contrast_param.blur_strength = sIVSVQAParam.contrast_param.blur_strength;
	vqa_param.contrast_param.blur_cover_th = sIVSVQAParam.contrast_param.blur_cover_th;
	vqa_param.contrast_param.blur_alarm_times = sIVSVQAParam.contrast_param.blur_alarm_times;
	vqa_set_param(0, &vqa_param);
	Movie_VqaSetFps(sIVSVQAParam.fps);
#endif
	return 0;
}

static INT32 param_vqa_get(INT32 argc, char *argv[])
{
#if (IPCAM_FUNC == ENABLE)
	IPCAM_VQA_RESULT IPCamVQAResult = {0};
	vqa_res_t vqa_res = Movie_VqaGetResult();
	IPCamVQAResult.Event = IPCAM_EVENT_VQA;
	IPCamVQAResult.TooLight = vqa_res.res_too_light;
	IPCamVQAResult.TooDark = vqa_res.res_too_dark;
	IPCamVQAResult.Blur = vqa_res.res_blur;
	NvtUctrl_SetConfigData((void *)&IPCamVQAResult, sizeof(IPCAM_VQA_RESULT));
#endif
	return 0;
}



/////////////////////
static INT32 param_unknown(char *name)
{
	DBG_ERR("unknown param: %s\r\n", name);
	return 0;
}

UTOKEN_PARAM tbl_movie_param[] = {
	{"recsize", param_recsize},  // [resolution index]
	{"lvchannelset", param_channelid_set},       // [channel set]
	{"lvencfmtset", param_liveviewencfmt_set},     // [video encode format index set]
	{"lvencfmtget", param_liveviewencfmt_get},     // [video encode format  index get]
	{"lvsizeset", param_liveviewsize_set},       // [resolution index set]
	{"lvsizeget", param_liveviewsize_get},       // [resolution index get]
	{"lvframerateset", param_liveviewframerate_set},  // [frame rate number set]
	{"lvframerateget", param_liveviewframerate_get},  // [frame rate number get]
	{"lvbitrateset", param_liveviewbitrate_set},    // [bit rate number set]
	{"lvbitrateget", param_liveviewbitrate_get},    // [bit rate number get]
	{"lvgopset", param_liveviewgop_set},            // [gop number set]
	//{"lvcropset", param_liveviewcrop_set},           // [gop number set]
	{"lvaudenctypeset", param_lvaudenctype_set},      // [Live view audio codec set]
	{"lvaudenctypeget", param_lvaudenctype_get},      // [Live view audio codec get]
	{"lvaudsamprateset", param_lvaudsamprate_set},    // [Live view audio sampling rate set]
	{"lvaudsamprateget", param_lvaudsamprate_get},    // [Live view audio sampling rate get]
	//{"lvratioset"    , param_liveviewratio_set},  // [video ratio set]
	{"vencGet", param_venc_get},    // get video encoder setting
	{"vencSet", param_venc_set},    // set video encoder setting
	{"vencROIGet", param_vencROI_get},    // get video encoder ROI setting
	{"vencROISet", param_vencROI_set},    // set video encoder ROI setting
	{"vencStat", param_vstat_get},  // get video encoder status
	{"vencIreq", param_venc_ireq},  // I-frame request
	{"aencGet", param_aenc_get},    // get audio encoder setting
	{"aencSet", param_aenc_set},    // set audio encoder setting
	{"voutGet", param_vout_get},    // get video output setting
	{"voutSet", param_vout_set},    // set video output setting
	{"voutEnable", param_vout_enable}, // enabel/disable video out setting
	{"aoutGet", param_aout_get},    // get audio output setting
	{"aoutSet", param_aout_set},    // set audio output setting
	{"aoutSwitch", param_aout_switch},  //  on/off audio output
	{"aoutStatus", param_aout_status},  //  get audio output status
	//{"HDRGet", param_hdr_get},    // get HDR setting
	//{"HDRSet", param_hdr_set},    // set HDR setting
	{"cycrec", param_cycrec},    // [time index]
	{"wdr", param_wdr},          // [on/off index]
	{"hdr", param_hdr},          // [on/off index]
	{"ev", param_ev},            // [value index]
	{"motion", param_motion},    // [on/off index]
	{"audio", param_audio},      // [on/off index]
	{"dstamp", param_dstamp},    // [on/off index]
	{"rsc", param_rsc},          // [on/off index]
	{"gsensor", param_gsensor},  // [level index]
	{"timelap", param_timelap},  // [interval index]
	{"ircut", param_ircut},      // [on/off index]

	{"murgpro", param_murgpro},  // [on/off index]
	{"aurgpro", param_aurgpro},  // [on/off index]
	{"pim", param_pim},          // [on/off index]
	{"dualcam", param_dualcam},  // [display index]
	{"start", param_start},
	{"stop", param_stop},
	{"snapshot", param_snapshot},
	{"fdstart", param_fdstart},  // fd start/stop
	{"fdset", param_fdset},      // fd setting get
	{"fdget", param_fdget},      // fd setting get
	{"fdrslt", param_fdrslt},    // get fd detect result
	{"ftgstart", param_ftgstart},  // fgt start/stop
	{"ftgset", param_ftgset},      // ftg setting get
	{"ftgget", param_ftgget},      // ftg setting get
	{"ftgrslt", param_ftgrslt},    // get ftg detect result
	{"ftgsetroi", param_ftgsetroi},    // set ftg roi
	{"ftggetroi", param_ftggetroi}, // get ftg roi
	{"ftgsnapshotnotify", param_ftgsnapshotnotify}, // encode to snapshot
	{"ftgsnapshotget", param_ftgsnapshotget}, // get ftg snapshot
	{"ftgsnapshotremove", param_ftgsnapshotremove}, // remove ftg snapshot
	{"motionset", param_motion_set}, // set motion config
	{"motionget", param_motion_get}, //get motion config
	//#NT#2016/11/01#Charlie Chang -begin
	//#NT# control motion start/stop
	{"motionstart", param_motion_start}, //get motion config
	//#NT#2016/11/01#Charlie Chang -end
	//#NT#2016/05/23#David Tsai -begin
	//#NT# Add tampering detection control for uctrl
	{"tdstart", param_tdstart},  // td start/stop
	{"tdset", param_tdset},      // td setting get
	{"tdget", param_tdget},      // td setting get
	{"tdrslt", param_tdrslt},    // get td detect result
	//#NT#2016/05/23#David Tsai -end
	//#NT#2016/08/26#Lincy Lin -begin
	//#NT# Add uctrl command for BC/MV/IPL buffer APIs .
	{"dspbufget", param_dspbuf_get},   // get dsp temp buffer
	{"imgget", param_img_get},      // get current ipl image
	{"mvstart", param_mvstart},  // mv start
	{"mvset", param_mvset},      // mv setting set
	{"mvget", param_mvget},      // mv setting get
	{"mvrslt", param_mvrslt},    // get mv result
	{"bcstart", param_bcstart},  // bc start
	{"bcset", param_bcset},      // bc setting set
	{"bcget", param_bcget},      // bc setting get
	{"bcreset", param_bcreset},   // reset bc
	{"bcfgget", param_bcfgget},   // get bc foreground
	{"bcbgget", param_bcbgget},   // get bc background
	{"bcrslt", param_bcrslt},     // get bc result
	{"bcmdset", param_bc_set},    // set bc md param
	{"vqaset", param_vqa_set},    // set vqa
	{"vqaget", param_vqa_get},    // get vqa
	{"bcblkrslt", param_bcblkrslt},     // get bc block result
	//#NT#2016/08/26#Lincy Lin -end
	{"disppath", param_disppath},//set display path
	{"rotateset", param_rotateset},    // set rotate
	{"rotateget", param_rotateget},    // get rotate
	{"livestreamqrcode", param_livestreamqrcode},    // get qrcode decode result
	{NULL, NULL}, //last tag, it must be
};

UTOKEN_MODULE module_movie = {
	"umovie",
	tbl_movie_param,
	param_begin,
	param_end,
	param_unknown,
	NULL
};

#if SXCMD_TEST
static BOOL Cmd_umovie_set(CHAR *strCmd)
{
	uToken_Parse(strCmd, &module_movie);
	return TRUE;
}

SXCMD_BEGIN(umovie, "umovie")
SXCMD_ITEM("set %", Cmd_umovie_set, "")
SXCMD_END()
#endif


void UCtrlAppMovie_Open(void)
{
	memset(pMovieInfo, 0, sizeof(UCtrlMovieInfoType));
#if SXCMD_TEST
	SxCmd_AddTable(umovie);
#endif

}

void UCtrlAppMovie_Close(void)
{
}



