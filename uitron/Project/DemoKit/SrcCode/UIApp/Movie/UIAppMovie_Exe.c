////////////////////////////////////////////////////////////////////////////////
#include "SysCommon.h"
//#include "AppCommon.h"
////////////////////////////////////////////////////////////////////////////////
#include "HwClock.h"
#include "UIAppCommon.h"
#include "UIAppMovie.h"
#include "UIAppPhoto.h"
#include "NVTUserCommand.h"
#include "UIMovieMapping.h"
#include "UIPhotoMapping.h"
#include "SysCfg.h" //for memory POOL_ID
#include "Audio.h"
#if(PLAY_MODE==ENABLE)
#include "UIAppPlay.h"
#endif
#include "GxDisplay.h"

#include "sensor.h"
#include "DxSensor.h"

#if(STAMP_FUNC==ENABLE)
#include "MovieStamp.h"
#endif
#include "UIMode.h"
#include "PrjCfg.h"
#if _TODO
#include "IQS_Utility.h"
#endif
#include "SysMain.h"
#if !defined(_GSensor_None_)
#include "GSensor.h"
#endif
#include "UIAppMovie_Param.h"
#include "iq_api.h"
#include "MovieUdtaVendor.h"
#if(WIFI_FUNC==ENABLE)
#include "UIModeWifi.h"
#endif
#if(WIFI_AP_FUNC==ENABLE)
#include "WifiAppCmd.h"
#include "RtspNvtApi.h"
#include "WifiAppXML.h"
#include "UIAppWiFiCmd.h"
#endif
#include "UIPhotoFuncInfo.h"
#include "rsc_api.h"

#include "ImageApp_Photo.h"
#include "ImageApp_Movie.h"
#include "ImageApp_MovieMulti.h"
#if(UVC_MULTIMEDIA_FUNC == ENABLE || IPCAM_UVC_FUNC == ENABLE)
#include "ImageApp_UsbMovie.h"
#include "ImageUnit_UsbUVAC.h"
#endif
#include "gpio.h"
#include "UIControlWnd.h"
#include "dma.h"
#if (defined(_BSP_NA51023_) && _TODO)
#include "dis_alg_fw.h"
#endif

#if(UCTRL_FUNC==ENABLE)
#include "UCtrlAppMovie.h"
#include "UCtrlMain.h"
#endif

#include "md_api.h"


#if (LOG_DBGINFO_IN_GPS_SECTION == ENABLE)
#include "mp4log.h"
#endif  // #if (LOG_DBGINFO_IN_GPS_SECTION == ENABLE)
#include "nvtmpp.h"
#include "videosprite.h"
#if (VIDEOSPRITEIPC==ENABLE)
#include "VideoSpriteIpc.h"
#endif

#include "AppDisp_PipView.h"
#include "iq_ui_info.h"
#include "ae_ui_info.h"
#include "af_ui_info.h"
#include "awb_ui_info.h"
#include "UIAppMovie_CommPoolTbl.h"
#include "alg_MovieExe.h"
#include "NMediaRecImgCap.h"
#if (AUDIO_FUNC_AAC == ENABLE)
#include "AudioCodec_AAC.h"
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


#define TEST_510_64MB       DISABLE



//#NT#2016/09/20#Bob Huang -begin
//#NT#Support HDMI Display with 3DNR Out
#if (_3DNROUT_FUNC == ENABLE)
BOOL gb3DNROut = FALSE;
#endif
//#NT#2016/09/20#Bob Huang -end

extern void System_DispSourceByTarget(ISIZE *pImgSize, USIZE *pAspectRatio);
extern UINT32 System_GetEnableDisp(void);
extern UINT32 System_GetEnableSensor(void);
extern void SMediaRec_SetEncPath(UINT32 uiVidEncId, MP_VDOENC_PATH path);

//#NT#2012/07/31#Hideo Lin -begin
//#NT#Test codes
#define MOVIE_YUV_SIZE_MAX      (1920*1080*2)   // maximum YUV raw data size
#define MOVIE_YUV_NUM           8   // 8 source images for fixed YUV mode

//#NT#2012/10/30#Calvin Chang#Generate Audio NR pattern by noises of zoom, focus and iris -begin
#define MOVIE_AUDIO_NR_PATTERN_ENABLE    DISABLE
//#NT#2012/10/30#Calvin Chang -end

static void MovieExe_CalcFDDispCord(URECT *dispCord);
static void MovieExe_CalcOSDDispCord(URECT *dispCord);
//#if (IPCAM_FUNC ==ENABLE)
//static void MovieExe_ConfigSpecificFunc(void);
//#endif

#define THIS_DBGLVL         2 // 0=FATAL, 1=ERR, 2=WRN, 3=UNIT, 4=FUNC, 5=IND, 6=MSG, 7=VALUE, 8=USER
///////////////////////////////////////////////////////////////////////////////
#define __MODULE__          UiAppMovie
#define __DBGLVL__          ((THIS_DBGLVL>=PRJ_DBG_LVL)?THIS_DBGLVL:PRJ_DBG_LVL)
#define __DBGFLT__          "*" //*=All, [mark]=CustomClass
//#define __DBGFLT__          "[cap]"
//#define __DBGFLT__          "[cb]"
//#define __DBGFLT__          "[set]"
//#define __DBGFLT__          "[zoom]"
//#define __DBGFLT__          "[af]"
#include "DebugModule.h"

//---------------------UIAPP_PHOTO Global Variables----------

BOOL _g_bFirstMovie = TRUE;
BOOL _g_bFirstMovieRec = FALSE;
BOOL bIsWiFiRecorded = FALSE;

//#NT#2010/05/21#Photon Lin -begin
#define THUMB_FRAME_SIZE 0x32000
//#NT#2010/05/21#Photon Lin -end
#define MINUTE_240 14400  // 4 hours
#define MINUTE_60  3600   //1 // 1 hour
#define MINUTE_29  1740   //29 minutes

#define RTSP_MAX_CLIENT         1

static USIZE IMAGERATIO_SIZE[IMAGERATIO_MAX_CNT] = {
	{9, 16}, //IMAGERATIO_9_16
	{2, 3}, //IMAGERATIO_2_3
	{3, 4}, //IMAGERATIO_3_4
	{1, 1}, //IMAGERATIO_1_1
	{4, 3}, //IMAGERATIO_4_3
	{3, 2}, //IMAGERATIO_3_2
	{16, 9}, //IMAGERATIO_16_9
};
#if _TODO
#if((WIFI_AP_FUNC==ENABLE)&&(SENSOR_CAPS_COUNT == 2))
static USIZE MOVIE_HTTPLVIEW_SIZE[IMAGERATIO_MAX_CNT] = {
	{HTTP_MJPG_W_4, HTTP_MJPG_H_3}, //IMAGERATIO_9_16
	{HTTP_MJPG_W_4, HTTP_MJPG_H_3}, //IMAGERATIO_2_3
	{HTTP_MJPG_W_4, HTTP_MJPG_H_3}, //IMAGERATIO_3_4
	{HTTP_MJPG_W_4, HTTP_MJPG_H_3}, //IMAGERATIO_1_1
	{HTTP_MJPG_W_4, HTTP_MJPG_H_3}, //IMAGERATIO_4_3
	{HTTP_MJPG_W_4, HTTP_MJPG_H_3}, //IMAGERATIO_3_2
	{HTTP_MJPG_W_16, HTTP_MJPG_H_9}, //IMAGERATIO_16_9
};
#endif
#endif

static BOOL    bSDIsSlow    = FALSE;
//#NT#2016/06/08#Lincy Lin -begin
//#NT#Implement generic OSD and video drawing mechanism for ALG function
URECT  gMovieFdDispCord;
URECT  gMovieOsdDispCord;
//#NT#2016/06/08#Lincy Lin -end
// For UCTRL use.
static UINT32 g_uiAudChannel = MOVREC_AUDTYPE_RIGHT;
static UINT32 g_uiAudSampleRate = 32000;
static UINT32 g_uiAudCodec = MOVAUDENC_AAC;

//#NT#2016/09/29#KCHong -begin
//#NT#The GPS related variables should not depend on ADAS.
#if (GPS_FUNCTION == ENABLE)
FLOAT g_CurSpeed = 0.0;
BOOL g_GPSLinked = FALSE;
BOOL g_GPSStatus = FALSE;
#endif
//#NT#2016/09/29#KCHong -end

//-------------------------------------------------------------------------------------------------
#if MOVIE_MAPPING_MULTIREC
extern MOVIE_RECODE_INFO gMovie_Rec_Info[SENSOR_MAX_NUM];
extern MOVIE_RECODE_INFO gMovie_Clone_Info[SENSOR_MAX_NUM];
extern MOVIE_STRM_INFO gMovie_Strm_Info;
extern MOVIE_RECODE_FILE_OPTION gMovie_Rec_Option;
extern MOVIE_RECODE_FOLDER_NAMING gMovie_Folder_Naming[SENSOR_MAX_NUM];
extern MOVIE_RECODE_FOLDER_NAMING gMovie_Clone_Folder_Naming[SENSOR_MAX_NUM];
extern MOVIE_AUDIO_INFO   gMovie_Audio_Info;
extern MOVIE_CFG_DISP_INFO gMovie_Disp_Info;
#else
static MOVIE_RECODE_INFO gMovie_Rec_Info[1];
static MOVIE_RECODE_FILE_OPTION gMovie_Rec_Option;
#endif
//-------------------------------------------------------------------------------------------------


ER MovieExe_SetGetReadyCB2_Sample(void);//2013/09/23 Meg
//#NT#2016/05/31#Ben Wang -begin
//#NT#Add UVC multimedia function.
#if(UVC_MULTIMEDIA_FUNC == ENABLE || IPCAM_UVC_FUNC == ENABLE)
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
#include "USBCDC.h"
static BOOL CdcPstnReqCB(CDC_COM_ID ComID, UINT8 Code, UINT8 *pData, UINT32 *pDataLen)
{
	BOOL bSupported = TRUE;
	CDCLineCoding LineCoding;
	DBGD(ComID);
	switch (Code) {
	case REQ_GET_LINE_CODING:
		DBG_DUMP("Get Line Coding\r\n");
		LineCoding.uiBaudRateBPS = 115200;
		LineCoding.uiCharFormat = 0;//CDC_LINEENCODING_OneStopBit;
		LineCoding.uiParityType = 0;//CDC_PARITY_None;
		LineCoding.uiDataBits = 8;
		*pDataLen = sizeof(LineCoding);
		memcpy(pData, &LineCoding, *pDataLen);
		break;
	case REQ_SET_LINE_CODING:
		DBG_DUMP("Set Line Coding\r\n");
		if (*pDataLen == sizeof(LineCoding)) {
			memcpy(&LineCoding, pData, *pDataLen);
		} else {
			bSupported = FALSE;
		}
		break;
	case REQ_SET_CONTROL_LINE_STATE:
		DBG_DUMP("Control Line State = 0x%X\r\n", *(UINT16 *)pData);
		//debug console test
		if (*(UINT16 *)pData == 0x3) { //console ready
		}
		break;
	default:
		bSupported = FALSE;
		break;
	}
	return bSupported;
}

#endif

//#NT#2016/05/31#Ben Wang -end
void MovieExe_IPL_SetIInfo(UINT32 index, UINT32 value)
{
    MOVIE_RECODE_IPL_INFO ipl_info = {0};
    UINT8 i;

    ipl_info.item  = index;
    ipl_info.value = value;
    for (i = 0; i < SENSOR_CAPS_COUNT; i++)	{
        ImageApp_MovieMulti_SetParam(i, MOVIEMULTI_PARAM_IPL_INFO, (UINT32) &ipl_info);
        //ImageApp_MovieMulti_SetParam(_CFG_REC_ID_1, MOVIEMULTI_PARAM_IPL_INFO, (UINT32) &ipl_info);
    }
}

void MovieExe_IQ_SetUIInfo(UINT32 index, UINT32 value)
{
	if (System_GetEnableSensor() & SENSOR_1) {
		iq_set_ui_info(IPL_PATH(0),index,value);
	}
	if (System_GetEnableSensor() & SENSOR_2) {
		iq_set_ui_info(IPL_PATH(1),index,value);
	}
}

void MovieExe_AE_SetUIInfo(UINT32 index, UINT32 value)
{
	if (System_GetEnableSensor() & SENSOR_1) {
		ae_set_ui_info(IPL_PATH(0),index,value);
	}
	if (System_GetEnableSensor() & SENSOR_2) {
		ae_set_ui_info(IPL_PATH(1),index,value);
	}
}

void MovieExe_AF_SetUIInfo(UINT32 index, UINT32 value)
{
	if (System_GetEnableSensor() & SENSOR_1) {
		af_set_ui_info(IPL_PATH(0),index,value);
	}
	if (System_GetEnableSensor() & SENSOR_2) {
		af_set_ui_info(IPL_PATH(1),index,value);
	}
}

void MovieExe_AWB_SetUIInfo(UINT32 index, UINT32 value)
{
	if (System_GetEnableSensor() & SENSOR_1) {
		awb_set_ui_info(IPL_PATH(0),index,value);
	}
	if (System_GetEnableSensor() & SENSOR_2) {
		awb_set_ui_info(IPL_PATH(1),index,value);
	}
}


void MovieExe_Cap_SetIInfo(UINT32 index, UINT32 value)
{
#if (MOVIE_PIM_CAPTURE == ENABLE)
	ImageUnit_Begin(&ISF_Cap, 0);
	ImageUnit_SetParam(ISF_CTRL, index, value);
	ImageUnit_End();
#endif
}

ISIZE Movie_GetBufferSize(void) //return USER CUSTOM buffer size
{
	return GxVideo_GetDeviceSize(DOUT1);
}

void Movie_SetSDSlow(BOOL IsSlow)
{
	bSDIsSlow = IsSlow;
}

void Movie_SetQPInitLevel(void)
{
#if _TODO
	UINT32  uiQPLevel = 20;

	// To do: set H.264 QP initial level according to ISO value

	MovRec_ChangeParameter(MOVREC_RECPARAM_H264QPLEVEL, uiQPLevel, 0, 0);
#endif
}

UINT32 Movie_GetAudChannel(void)
{
	return g_uiAudChannel;
}

void Movie_SetAudChannel(UINT32 uiAudChannel)
{
	g_uiAudChannel = uiAudChannel;
}

UINT32 Movie_GetAudSampleRate(void)
{
	return g_uiAudSampleRate;
}

void Movie_SetAudSampleRate(UINT32 uiAudSampleRate)
{
	g_uiAudSampleRate = uiAudSampleRate;
}

UINT32 Movie_GetAudCodec(void)
{
	return g_uiAudCodec;
}

void Movie_SetAudCodec(UINT32 uiAudCodec)
{
	g_uiAudCodec = uiAudCodec;
}



void MovieExe_SetDateStamp(void)
{

}

void MovieExe_CalcDispSize(ISIZE *pDispSize, ISIZE *pDevSize, ISIZE *pDevRatio, ISIZE *pImgRatio)
{
	if (((pDevRatio->w * 100) / pDevRatio->h) > ((pImgRatio->w * 100) / pImgRatio->h)) { // device aspect ratio > image ratio
		pDispSize->h = pDevSize->h;
		pDispSize->w = ALIGN_ROUND_16((pDevSize->w * pImgRatio->w * pDevRatio->h) / (pImgRatio->h * pDevRatio->w));
	} else {
		pDispSize->w = pDevSize->w;
		pDispSize->h = ALIGN_ROUND_4((pDevSize->h * pImgRatio->h * pDevRatio->w) / (pImgRatio->w * pDevRatio->h));
	}
}


extern UINT32 System_GetConnectSensor(void);

//#NT#2016/02/24#Hideo Lin -begin
//#NT#Check movie size setting by sensor connection
#if (MOVIE_MULTI_RECORD_FUNC)
#define DEFAULT_MOVIE_SIZE_SINGLE   MOVIE_SIZE_FRONT_1920x1080P30
#define DEFAULT_MOVIE_SIZE_DUAL     MOVIE_SIZE_DUAL_1920x1080P30_1920x1080P30
extern UINT32 System_GetEnableSensor(void);
void MovieExe_CheckMovieSizeBySensorConnect(void)
{
	static BOOL bFirstTime = TRUE;
	static UINT32 uiPrevSensor = 0; // previous active sensor
	UINT32 uiSensor; // current active sensor
	UINT32 uiMovieSize;

	uiMovieSize = SysGetFlag(FL_MOVIE_SIZE);
	uiSensor = System_GetEnableSensor();

	if (bFirstTime) {
		bFirstTime = FALSE;

		if (uiSensor != (SENSOR_1 | SENSOR_2)) {
			if (GetMovieRecType_2p(uiMovieSize) == MOVIE_REC_TYPE_DUAL) {
				SysSetFlag(FL_MOVIE_SIZE, DEFAULT_MOVIE_SIZE_SINGLE);
				SysSetFlag(FL_MOVIE_SIZE_MENU, DEFAULT_MOVIE_SIZE_SINGLE);
			}
			//#NT#2016/06/17#Brain Yen -begin
			//#NT#for special aspect panel and Pipview LR
#if (PIP_VIEW_LR == ENABLE)
			SysSetFlag(FL_DUAL_CAM, DUALCAM_LR_FRONT_FULL);
			PipView_SetStyle(UI_GetData(FL_DUAL_CAM));
#endif
			//#NT#2016/06/17#Brain Yen -end
		} else if (uiSensor == (SENSOR_1 | SENSOR_2)) {
			if (SysGetFlag(FL_MOVIE_SIZE) <= MOVIE_SIZE_FRONT_1920x1080P60) {
				// insert sensor, set movie to dual record
				SysSetFlag(FL_MOVIE_SIZE, DEFAULT_MOVIE_SIZE_DUAL);
				SysSetFlag(FL_MOVIE_SIZE_MENU, DEFAULT_MOVIE_SIZE_DUAL);
				//#NT#2016/06/17#Brain Yen -begin
				//#NT#for special aspect panel and Pipview LR
#if (PIP_VIEW_LR == ENABLE)
				SysSetFlag(FL_DUAL_CAM, DUALCAM_LR_16_9);
				PipView_SetStyle(UI_GetData(FL_DUAL_CAM));
#else
#if (PIP_VIEW_FUNC == ENABLE)
				SysSetFlag(FL_DUAL_CAM_MENU, DUALCAM_BOTH);
				SysSetFlag(FL_DUAL_CAM, DUALCAM_BOTH);
				PipView_SetStyle(UI_GetData(FL_DUAL_CAM));
#endif
#endif
				//#NT#2016/06/17#Brain Yen -end
			}
		}
	} else {
		if (uiPrevSensor != uiSensor) {
			if (uiPrevSensor == (SENSOR_1 | SENSOR_2)) {
				// remove sensor, set movie to single record if necessary
				if (GetMovieRecType_2p(uiMovieSize) == MOVIE_REC_TYPE_DUAL) {
					SysSetFlag(FL_MOVIE_SIZE, DEFAULT_MOVIE_SIZE_SINGLE);
					SysSetFlag(FL_MOVIE_SIZE_MENU, DEFAULT_MOVIE_SIZE_SINGLE);
				}
				//#NT#2016/06/17#Brain Yen -begin
				//#NT#for special aspect panel and Pipview LR
#if (PIP_VIEW_LR == ENABLE)
				SysSetFlag(FL_DUAL_CAM, DUALCAM_LR_FRONT_FULL);
				PipView_SetStyle(UI_GetData(FL_DUAL_CAM));
#endif
				//#NT#2016/06/17#Brain Yen -end
			} else if (uiSensor == (SENSOR_1 | SENSOR_2)) {
				// insert sensor, set movie to dual record
				SysSetFlag(FL_MOVIE_SIZE, DEFAULT_MOVIE_SIZE_DUAL);
				SysSetFlag(FL_MOVIE_SIZE_MENU, DEFAULT_MOVIE_SIZE_DUAL);
				//#NT#2016/06/17#Brain Yen -begin
				//#NT#for special aspect panel and Pipview LR
#if (PIP_VIEW_LR == ENABLE)
				SysSetFlag(FL_DUAL_CAM, DUALCAM_LR_16_9);
				PipView_SetStyle(UI_GetData(FL_DUAL_CAM));
#else
#if (PIP_VIEW_FUNC == ENABLE)
				SysSetFlag(FL_DUAL_CAM_MENU, DUALCAM_BOTH);
				SysSetFlag(FL_DUAL_CAM, DUALCAM_BOTH);

				PipView_SetStyle(UI_GetData(FL_DUAL_CAM));
#endif
#endif
				//#NT#2016/06/17#Brain Yen -end
			}
		}
	}
	uiPrevSensor = uiSensor;
}
#endif
//#NT#2016/02/24#Hideo Lin -end


INT32 MovieExe_InitCommonMem(void)
{
    NVTMPP_ER        ret;
    NVTMPP_VB_CONF_S st_conf;
	UINT32           i;
    ISIZE            devSize = GxVideo_GetDeviceSize(DOUT1);
	NVT_VB_CPOOL_S   *p_comm_pool;


    memset(&st_conf, 0, sizeof(NVTMPP_VB_CONF_S));
    st_conf.ddr_mem[0].Addr = OS_GetMempoolAddr(POOL_ID_APP);
    st_conf.ddr_mem[0].Size = OS_GetMempoolSize(POOL_ID_APP);

#if 1
    if (POOL_ID_APP_ARBIT2)
#else
    if (0)
#endif
    {
        st_conf.ddr_mem[1].Addr = OS_GetMempoolAddr(POOL_ID_APP_ARBIT2);
        st_conf.ddr_mem[1].Size = OS_GetMempoolSize(POOL_ID_APP_ARBIT2);
    }
    st_conf.max_pool_cnt = 64;
	if (devSize.w < 1920 && devSize.h < 1080) {
		p_comm_pool = &g_movie_common_pool_tbl[0];
	} else {
    	p_comm_pool = &g_movie_hdmi4k_common_pool_tbl[0];
	}
	for (i=0;i<NVTMPP_VB_MAX_COMM_POOLS;i++){
		if (p_comm_pool->blk_size == 0)
			break;
		st_conf.common_pool[i] = *p_comm_pool;
		p_comm_pool++;
	}
/*
#if (SENSOR_CAPS_COUNT == 1)
    st_conf.common_pool[0].blk_size = 3840 * 2160 * 3 / 2 + 1024;
    st_conf.common_pool[0].blk_cnt = 4;
    st_conf.common_pool[0].ddr = NVTMPP_DDR_1;
    #if (defined(_CHIP_96683_) && (defined(_MODEL_WiFiSDV_) || defined(_MODEL_FIREFLY_)) )
    devSize.w = 640; // TV PAL
    devSize.h = 528; // TV PAL
    st_conf.common_pool[1].blk_size = devSize.w * devSize.h * 3 / 2 + 1024;
    #else
	if (devSize.w < 1920 && devSize.h < 1080) {
		st_conf.common_pool[1].blk_size = 1920 * 1080 * 3 / 2 + 1024;
	} else {
    	st_conf.common_pool[1].blk_size = (devSize.w * devSize.h) * 3 / 2 + 1024;
	}
    #endif
	#if (MOVIE_UVAC_FUNC == 1)
	st_conf.common_pool[1].blk_cnt = 7;
	#else
	st_conf.common_pool[1].blk_cnt = 4;
	#endif
    st_conf.common_pool[1].ddr = NVTMPP_DDR_1;
    st_conf.common_pool[2].blk_size = 1280 * 720 * 3 / 2 + 1024;
    st_conf.common_pool[2].blk_cnt = 2;     //2 for ALG
    st_conf.common_pool[2].ddr = NVTMPP_DDR_1;
    st_conf.common_pool[3].blk_size = 640 * 480 * 3 / 2 + 1024;
    st_conf.common_pool[3].blk_cnt = 2;     //2 for ALG
    st_conf.common_pool[3].ddr = NVTMPP_DDR_1;
#if (TEST_510_64MB == ENABLE)
    st_conf.common_pool[0].blk_size = 1920 * 1080 * 3 / 2 + 1024;
    st_conf.common_pool[0].blk_cnt = 4; //default 3, enable IME 3dnr +1
    st_conf.common_pool[0].ddr = NVTMPP_DDR_1;
    st_conf.common_pool[1].blk_size = 720 * 576 * 3 / 2 + 1024;
    st_conf.common_pool[1].blk_cnt = 2;
    st_conf.common_pool[1].ddr = NVTMPP_DDR_1;
    st_conf.common_pool[2].blk_size = 640 * 480 * 3 / 2 + 1024;
    st_conf.common_pool[2].blk_cnt = 3;
    st_conf.common_pool[2].ddr = NVTMPP_DDR_1;
#endif
#endif
#if (SENSOR_CAPS_COUNT == 2)
    st_conf.common_pool[0].blk_size = 3840 * 2160 * 3 / 2 + 1024;
    st_conf.common_pool[0].blk_cnt = 5 * 2;
    st_conf.common_pool[0].ddr = NVTMPP_DDR_1;
    st_conf.common_pool[1].blk_size = (devSize.w * devSize.h) * 3 / 2 + 1024;
    st_conf.common_pool[1].blk_cnt = 4 * 3;
    st_conf.common_pool[1].ddr = NVTMPP_DDR_1;
    st_conf.common_pool[2].blk_size = 1080 * 720 * 3 / 2 + 1024;
    st_conf.common_pool[2].blk_cnt = 4 * 3;
    st_conf.common_pool[2].ddr = NVTMPP_DDR_1;
#endif
#if (SENSOR_CAPS_COUNT == 4)
    st_conf.common_pool[0].blk_size = 1920 * 1080 * 3 / 2 + 1024;
    st_conf.common_pool[0].blk_cnt = 5 * 4;
    st_conf.common_pool[0].ddr = NVTMPP_DDR_1;
    st_conf.common_pool[1].blk_size = (devSize.w * devSize.h) * 3 / 2 + 1024;
    st_conf.common_pool[1].blk_cnt = 4 * 3;
    st_conf.common_pool[1].ddr = NVTMPP_DDR_1;
    st_conf.common_pool[2].blk_size = 1080 * 720 * 3 / 2 + 1024;
    st_conf.common_pool[2].blk_cnt = 4 * 3;
    st_conf.common_pool[2].ddr = NVTMPP_DDR_1;
#endif
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
*/
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
    return 0;
}

INT32 MovieExe_ExitCommonMem(void)
{
	NVTMPP_ER        ret;
	ret = nvtmpp_vb_exit();
	if (NVTMPP_ER_OK != ret) {
		DBG_ERR("nvtmpp init vb err: %d\r\n", ret);
		return -1;
	}
	return 0;
}

//-------------------------------------------------------------------------------------------------
void MovieExe_RSC_SetSwitch(UINT32 index, UINT32 value)
{
#if !defined(_Gyro_None_)

#if !defined(_IPL1_IPL_FAKE_)
	// RSC just support IPL 1
	if (gMovie_Rec_Info[0].vid_in == _CFG_VID_IN_1){
        rsc_ext_setswitch(IPL_PATH_1, index, value);
		DBG_DUMP("set rsc index %d value %d\r\n",index,value);
	}
#endif

#endif
}

INT32 MovieExe_OnMovieSize(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
	UINT32 uiSelect = 0;
	//UINT32 uiRatioIdx = 0;

	if (paramNum > 0) {
		uiSelect = paramArray[0];
	}

	DBG_IND("uiSelect %d\r\n", uiSelect);

	UI_SetData(FL_MOVIE_SIZE, uiSelect);

#if 0
	gMovie_Rec_Info.size.w = GetMovieSizeWidth(uiSelect);
	gMovie_Rec_Info.size.h = GetMovieSizeHeight(uiSelect);
	gMovie_Rec_Info.frame_rate = GetMovieFrameRate(uiSelect);
	gMovie_Rec_Info.target_bitrate = GetMovieTargetBitrate(uiSelect, 0);
	uiRatioIdx = GetMovieSizeRatio(uiSelect);
	switch (uiRatioIdx) {
	case IMAGERATIO_16_9:
		gMovie_Rec_Info.ratio.w = 16;
		gMovie_Rec_Info.ratio.h = 9;
		break;
	default:
		gMovie_Rec_Info.ratio.w = 4;
		gMovie_Rec_Info.ratio.h = 3;
	}

	ImageApp_Movie_SetParam(_CFG_REC_ID_1, MOVIE_PARAM_FRAMERATE, gMovie_Rec_Info.frame_rate);
	ImageApp_Movie_SetParam(_CFG_REC_ID_1, MOVIE_PARAM_TARGETRATE, gMovie_Rec_Info.target_bitrate);
	ImageApp_Movie_SetParam(_CFG_REC_ID_1, MOVIE_PARAM_IMG_RATIO, (UINT32) & (gMovie_Rec_Info.ratio));
	ImageApp_Movie_SetParam(_CFG_REC_ID_1, MOVIE_PARAM_IMG_SIZE, (UINT32) & (gMovie_Rec_Info.size));
#endif

	return NVTEVT_CONSUME;
}

static USIZE MovieExe_RatioSizeConvert(USIZE *devSize, USIZE *devRatio, USIZE *Imgratio)
{
	USIZE  resultSize = *devSize;

	if ((!devRatio->w) || (!devRatio->h)) {
		DBG_ERR("devRatio w=%d, h=%d\r\n", devRatio->w, devRatio->h);
	} else if ((!Imgratio->w) || (!Imgratio->h)) {
		DBG_ERR("Imgratio w=%d, h=%d\r\n", Imgratio->w, Imgratio->h);
	} else {
		if (((float)Imgratio->w / Imgratio->h) >= ((float)devRatio->w / devRatio->h)) {
			resultSize.w = devSize->w;
			resultSize.h = ALIGN_ROUND_4(devSize->h * Imgratio->h / Imgratio->w * devRatio->w / devRatio->h);
		} else {
			resultSize.h = devSize->h;
			resultSize.w = ALIGN_ROUND_16(devSize->w * Imgratio->w / Imgratio->h * devRatio->h / devRatio->w);
		}
	}
	return resultSize;
}
//#NT#2016/06/08#Lincy Lin -begin
//#NT#Implement generic OSD and video drawing mechanism for ALG function
static void MovieExe_CalcFDDispCord(URECT *dispCord)
{
	UINT32 ImageRatioIdx = 0;
	USIZE  ImageRatioSize = {0};
	URECT  fdDispCoord;
	ISIZE  dev1size;
	USIZE  dev1Ratio;
	USIZE  finalSize = {0};

	ImageRatioIdx = GetMovieSizeRatio(UI_GetData(FL_MOVIE_SIZE));
	ImageRatioSize = IMAGERATIO_SIZE[ImageRatioIdx];

	//1.get current device size (current mode)
	dev1size = GxVideo_GetDeviceSize(DOUT1);
	//2.get current device aspect Ratio
	dev1Ratio = GxVideo_GetDeviceAspect(DOUT1);
	finalSize = MovieExe_RatioSizeConvert((USIZE *)&dev1size, &dev1Ratio, &ImageRatioSize);
	fdDispCoord.w = finalSize.w;
	fdDispCoord.h = finalSize.h;
	if (finalSize.w == (UINT32)dev1size.w) {
		fdDispCoord.x = 0;
		fdDispCoord.y = (dev1size.h - finalSize.h) >> 1;
	} else {
		fdDispCoord.y = 0;
		fdDispCoord.x = (dev1size.w - finalSize.w) >> 1;

	}
	*dispCord = fdDispCoord;
}

static void MovieExe_CalcOSDDispCord(URECT *dispCord)
{
	UINT32 ImageRatioIdx = 0;
	USIZE  ImageRatioSize = {0};
	URECT  OsdDispCoord;
	ISIZE  dev1size;
	USIZE  dev1Ratio;
	USIZE  finalSize = {0};

	ImageRatioIdx = GetMovieSizeRatio(UI_GetData(FL_MOVIE_SIZE));
	ImageRatioSize = IMAGERATIO_SIZE[ImageRatioIdx];

	//1.get current device size (current mode)
	dev1size.w = OSD_W;
	dev1size.h = OSD_H;
	//2.get current device aspect Ratio
	//#NT#2016/06/16#Lincy Lin -begin
	//#NT#Fix osd display ratio mismatch video bug
	//dev1Ratio.w = OSD_W;
	//dev1Ratio.h = OSD_H;
	dev1Ratio = GxVideo_GetDeviceAspect(DOUT1);
	//#NT#2016/06/16#Lincy Lin -end

	finalSize = MovieExe_RatioSizeConvert((USIZE *)&dev1size, &dev1Ratio, &ImageRatioSize);
	OsdDispCoord.w = finalSize.w;
	OsdDispCoord.h = finalSize.h;

	if (OsdDispCoord.w == (UINT32)dev1size.w) {
		OsdDispCoord.x = 0;
		OsdDispCoord.y = (dev1size.h - finalSize.h) >> 1;
	} else {
		OsdDispCoord.y = 0;
		OsdDispCoord.x = (dev1size.w - finalSize.w) >> 1;

	}
	*dispCord = OsdDispCoord;
}
//#NT#2016/06/08#Lincy Lin -end

//-------------------------------------------------------------------------------------------------
INT32 MovieExe_OnImageRatio(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
	UINT32  ImageRatioIdx = 0;
	USIZE   ImageRatioSize = {0};
	//#NT#2016/06/03#Brain Yen -begin
	//#NT#for special aspect panel and pipview
#if (PIP_VIEW_LR == ENABLE || defined(_LCDTYPE_TG078UW006A0_DSI_))
	USIZE DeviceRatioSize = GxVideo_GetDeviceAspect(DOUT1);
#endif
	//#NT#2016/06/03#Brain Yen -end

	DBG_IND("\r\n");

	if (paramNum > 0) {
		ImageRatioIdx = paramArray[0];    // parameter 0: image size
	}
	DBG_MSG("ImageRatioIdx =%d\r\n", ImageRatioIdx);

	//check if imagestream is already open?
	if (ImageStream_IsOpen(&ISF_Stream[0]) != ISF_OK) {
		// ignore this change
		return NVTEVT_CONSUME;
	}

	if (ImageRatioIdx >= IMAGERATIO_MAX_CNT) {
		DBG_ERR("ImageRatioIdx =%d\r\n", ImageRatioIdx);
		return NVTEVT_CONSUME;
	}

	ImageRatioSize = IMAGERATIO_SIZE[ImageRatioIdx];
	DBG_MSG("ImageRatioSize=%d:%d\r\n", ImageRatioSize.w, ImageRatioSize.h);
	//#NT#2016/06/08#Lincy Lin -begin
	//#NT#Implement generic OSD and video drawing mechanism for ALG function
	MovieExe_CalcFDDispCord(&gMovieFdDispCord);
	MovieExe_CalcOSDDispCord(&gMovieOsdDispCord);
	//#NT#2016/06/08#Lincy Lin -end

	//1. Set Display image ratio
	if (System_GetEnableDisp() & DISPLAY_1) {
		ImageUnit_Begin(&ISF_VdoOut1, 0);
		ImageUnit_SetVdoImgSize(ISF_IN1, 0, 0); //buffer size = full device size
		//#NT#2016/06/03#Brain Yen -begin
		//#NT#for special aspect panel and pipview
#if (PIP_VIEW_LR == ENABLE || defined(_LCDTYPE_TG078UW006A0_DSI_))
		ImageUnit_SetVdoAspectRatio(ISF_IN1, DeviceRatioSize.w, DeviceRatioSize.h);
#elif (SBS_VIEW_FUNC == ENABLE)
		ImageUnit_SetVdoAspectRatio(ISF_IN1, ImageRatioSize.w << 1, ImageRatioSize.h);
#else
		ImageUnit_SetVdoAspectRatio(ISF_IN1, ImageRatioSize.w, ImageRatioSize.h);
#endif
		//#NT#2016/06/03#Brain Yen -end
		ImageUnit_SetVdoPreWindow(ISF_IN1, 0, 0, 0, 0);  //window range = full device range
		ImageUnit_End();
	}
	if (System_GetEnableDisp() & DISPLAY_2) {
		ImageUnit_Begin(&ISF_VdoOut2, 0);
		ImageUnit_SetVdoImgSize(ISF_IN1, 0, 0); //buffer size = full device size
#if (SBS_VIEW_FUNC == ENABLE)
		ImageUnit_SetVdoAspectRatio(ISF_IN1, ImageRatioSize.w << 1, ImageRatioSize.h);
#else
		ImageUnit_SetVdoAspectRatio(ISF_IN1, ImageRatioSize.w, ImageRatioSize.h);
#endif
		ImageUnit_SetVdoPreWindow(ISF_IN1, 0, 0, 0, 0);  //window range = full device range
		ImageUnit_End();
	}
	//Set UserDraw Size
	{
		ISIZE devSize = GxVideo_GetDeviceSize(DOUT1);
		System_DispSourceByTarget(&devSize, &ImageRatioSize);
	}

	//2. Set Preview image ratio, and direction
	if (System_GetEnableSensor() & SENSOR_1) {
		ImageUnit_Begin(ISF_IPL(0), 0);
		ImageUnit_SetVdoAspectRatio(ISF_IN1, ImageRatioSize.w, ImageRatioSize.h);
		//#NT#2016/03/02#Niven Cho -begin
		//#NT#Use chip filp instead of sensor flip
		ImageUnit_SetVdoDirection(ISF_IN1, (UI_GetData(FL_MOVIE_SENSOR_ROTATE) == SEN_ROTATE_ON) ? FLIP_V : FLIP_NONE);
		//#NT#2016/03/02#Niven Cho -end
		ImageUnit_End();
	}
	if (System_GetEnableSensor() & SENSOR_2) {
		ImageUnit_Begin(ISF_IPL(1), 0);
		ImageUnit_SetVdoAspectRatio(ISF_IN1, ImageRatioSize.w, ImageRatioSize.h);
		ImageUnit_SetVdoDirection(ISF_IN1, 0);
		ImageUnit_End();
	}
	//ImageStream_UpdateSize();
	ImageStream_UpdateAll(&ISF_Stream[0]);

	return NVTEVT_CONSUME;
}

//-------------------------------------------------------------------------------------------------
INT32 MovieExe_OnMovieQuality(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
#if _TODO
	UINT32 uiQuality = 0;
	UINT32 uiSize;

	if (paramNum > 0) {
		uiQuality = paramArray[0];
	}

	UI_SetData(FL_MOVIE_QUALITY, uiQuality);
	uiSize = UI_GetData(FL_MOVIE_SIZE);

	MovRec_ChangeParameter(MOVREC_RECPARAM_TARGETRATE, GetMovieTargetBitrate(uiSize, uiQuality), 0, 0);
#endif

	return NVTEVT_CONSUME;
}

//-------------------------------------------------------------------------------------------------
INT32 MovieExe_OnWB(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
	UINT32 uiSelect = 0;

	if (paramNum) {
		uiSelect = paramArray[0];
	}

	UI_SetData(FL_WB, uiSelect);
	MovieExe_AWB_SetUIInfo(AWB_UI_WB, Get_WBValue(uiSelect));

	return NVTEVT_CONSUME;
}

//-------------------------------------------------------------------------------------------------
INT32 MovieExe_OnColor(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
	UINT32 uiSelect = 0;

	if (paramNum) {
		uiSelect = paramArray[0];
	}

	UI_SetData(FL_MOVIE_COLOR, uiSelect);
	MovieExe_IQ_SetUIInfo(IQ_UI_IMAGEEFFECT, Get_ColorValue(uiSelect));

	return NVTEVT_CONSUME;
}

//-------------------------------------------------------------------------------------------------
INT32 MovieExe_OnEV(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
	UINT32 uiSelect = 0;

	if (paramNum) {
		uiSelect = paramArray[0];
	}

	UI_SetData(FL_EV, uiSelect);
	//photo and movie use the same EV value,should syncronize photo EV setting
	Photo_SetUserIndex(PHOTO_USR_EV, uiSelect);
	MovieExe_AE_SetUIInfo(AE_UI_EV, Get_EVValue(uiSelect));

	return NVTEVT_CONSUME;
}
//-------------------------------------------------------------------------------------------------

//#NT#2016/06/03#Charlie Chang -begin
//#NT# add contrast ,audio in, flip mirror, quality set
INT32 MovieExe_OnContrast(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{

	if (paramNum != 1) {
		DBG_ERR("paramNum =%d error!!!\r\n", paramNum);
		return NVTEVT_CONSUME;
	}
	UINT32 uiSelect = 0;
	uiSelect = paramArray[0];
	if (uiSelect >= MOVIE_CONTRAST_ID_MAX) {
		DBG_ERR("Contrast uiSelect =%d error\r\n", uiSelect);
		return NVTEVT_CONSUME;
	}
	MovieExe_IQ_SetUIInfo(IQ_UI_CONTRAST, uiSelect);

	DBG_DUMP("Contrast:%d\r\n", uiSelect);
	UI_SetData(FL_MOVIE_CONTRAST, uiSelect);

	return NVTEVT_CONSUME;
}
INT32 MovieExe_OnAudioIn(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{

	DBG_ERR("MovieExe_OnAudioin not implement need to do!!!!!!!!!!\r\n");

	return NVTEVT_CONSUME;
}
INT32 MovieExe_OnAudioInSR(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{

	DBG_ERR("MovieExe_OnAudioInSR not implement need to do!!!!!!!!!!\r\n");

	return NVTEVT_CONSUME;
}
INT32 MovieExe_OnFipMirror(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
#if _TODO
	if (paramNum < 1) {
		DBG_ERR("paramNum =%d error!!!\r\n", paramNum);
		return NVTEVT_CONSUME;
	}
	UINT32 uiSelect = 0;
	uiSelect = paramArray[0];
	UINT32 ipl_id = 0;

	if (uiSelect >= MOVIE_FLIP_MIRROR_ID_MAX) {
		DBG_ERR("flip value=%d error\r\n", uiSelect);
		return NVTEVT_CONSUME;
	}
	UI_SetData(FL_MOVIE_CONTRAST, uiSelect);
	ImageUnit_Begin(ISF_IPL(0), 0);
	if (uiSelect == MOVIE_FLIP_MIRROR_H_V) {
		DBG_DUMP("movie flip_H_V\r\n");
		ImageUnit_SetVdoDirection(ISF_IN1, ISF_VDO_DIR_MIRRORX|ISF_VDO_DIR_MIRRORY);
	} else if (uiSelect == MOVIE_FLIP_MIRROR_H) {
		DBG_DUMP("movie flip x\r\n");
		ImageUnit_SetVdoDirection(ISF_IN1, ISF_VDO_DIR_MIRRORX);
	} else if (uiSelect == MOVIE_FLIP_MIRROR_V) {
		DBG_DUMP("movie flip y\r\n");
		ImageUnit_SetVdoDirection(ISF_IN1, ISF_VDO_DIR_MIRRORY);
	} else {
		DBG_DUMP("movie flip_NONE\r\n");
		ImageUnit_SetVdoDirection(ISF_IN1, ISF_VDO_DIR_NONE);
	}
	ImageUnit_End();
#endif

	return NVTEVT_CONSUME;
}
INT32 MovieExe_OnQualitySet(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{

	if (paramNum < 1) {
		DBG_ERR("paramNum =%d error!!!\r\n", paramNum);
		return NVTEVT_CONSUME;
	}
	UINT32 uiSelect = 0;
	uiSelect = paramArray[0];

	switch (uiSelect) {

	case MOVIE_QUALITY_SET_MAX:
		UI_SetData(FL_MOVIE_SIZE, MOVIE_SIZE_FRONT_1920x1080P60);
		break;
	case MOVIE_QUALITY_SET_HIGH:
		UI_SetData(FL_MOVIE_SIZE, MOVIE_SIZE_FRONT_1280x720P30);
		break;
	case MOVIE_QUALITY_SET_MIDDLE:
		UI_SetData(FL_MOVIE_SIZE, MOVIE_SIZE_FRONT_848x480P30);
		break;
	case MOVIE_QUALITY_SET_LOW:
		UI_SetData(FL_MOVIE_SIZE, MOVIE_SIZE_FRONT_640x480P30);
		break;
	case MOVIE_QUALITY_SET_MIN:
		UI_SetData(FL_MOVIE_SIZE, MOVIE_SIZE_FRONT_320x240P30);
		break;
	default:
		DBG_ERR("uiSelect =%d error!! \r\n", uiSelect);
		return NVTEVT_CONSUME;
	}
	//reopen movie mode
	Ux_SendEvent(0, NVTEVT_SYSTEM_MODE, 1, PRIMARY_MODE_MOVIE);
	return NVTEVT_CONSUME;
}
//#NT#2016/06/03#Charlie Chang -end
//-------------------------------------------------------------------------------------------------
INT32 MovieExe_OnMovieAudio(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
	UINT32 uiSelect = 0;

	if (paramNum) {
		uiSelect = paramArray[0];
	}

	UI_SetData(FL_MOVIE_AUDIO, uiSelect);

	if (uiSelect == MOVIE_AUDIO_OFF) {
		ImageUnit_Begin(&ISF_AudIn, 0);
		ImageUnit_SetParam(ISF_CTRL, AUDIN_PARAM_VOL_IMM, RECMOVIE_AUD_OFF); //set mute!
		ImageUnit_End();
		//DBG_DUMP("===Movie Rec Set Vol = %d\r\n", RECMOVIE_AUD_OFF);
	} else {
		UINT32 current_vol = UI_GetData(FL_MovieAudioRecIndex);
		ImageUnit_Begin(&ISF_AudIn, 0);
		ImageUnit_SetParam(ISF_CTRL, AUDIN_PARAM_VOL_IMM, current_vol); //restore to current vol
		ImageUnit_End();
		//DBG_DUMP("===Movie Rec Set Vol = %d\r\n", current_vol);
	}

	return NVTEVT_CONSUME;
}

INT32 MovieExe_OnMovieAudioRecVolume(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
	UINT32 uhSelect = 0;
	UINT32 current_vol = UI_GetData(FL_MovieAudioRecIndex);
	if (paramNum > 0) {
		uhSelect = paramArray[0];
	}
	DBG_IND("+-idx=%d\r\n", uhSelect);
	if (current_vol != uhSelect) {
		UI_SetData(FL_MovieAudioRecIndex, uhSelect);
		ImageUnit_Begin(&ISF_AudIn, 0);
		ImageUnit_SetParam(ISF_CTRL, AUDIN_PARAM_VOL_IMM, uhSelect); //set current vol
		ImageUnit_End();
		//DBG_DUMP("===Movie Rec Set Vol = %d\r\n", uhSelect);
	}
	return NVTEVT_CONSUME;
}

INT32 MovieExe_OnMovieAudioRec(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
	UINT32 uhSelect = 0;
	UINT32 current_audio_stream = UI_GetData(FL_MovieAudioRec);
	if (paramNum > 0) {
		uhSelect = paramArray[0];
	}
	DBG_IND("+-idx=%d\r\n", uhSelect);
	if (current_audio_stream != uhSelect) {
		UI_SetData(FL_MovieAudioRec, uhSelect);
	}
	return NVTEVT_CONSUME;
}

INT32 MovieExe_OnDigitalZoom(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
	UINT32 uhSelect = 0;
	if (paramNum > 0) {
		uhSelect = paramArray[0];
	}
	DBG_IND("+-MovieExe_OnDigitalZoom:idx=%d\r\n", uhSelect);
	UI_SetData(FL_Dzoom, uhSelect);
	if (uhSelect == DZOOM_OFF) {
		UI_SetData(FL_DzoomIndex, DZOOM_10X);
	}
	return NVTEVT_CONSUME;
}

INT32 MovieExe_OnDis(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
	UINT32 uiSelect = 0;

	if (paramNum) {
		uiSelect = paramArray[0];
	}

	DBG_IND("idx=%d\r\n", uiSelect);

	UI_SetData(FL_MovieDisIndex, uiSelect);

	if(uiSelect == TRUE) {
    //if (uiSelect == MOVIE_DIS_ON) {
        // enable DIS
		MovieExe_IPL_SetIInfo(IPL_SEL_DISCOMP_IMM, SEL_DISCOMP_ON);
    } else {
        // enable DIS
		MovieExe_IPL_SetIInfo(IPL_SEL_DISCOMP_IMM, SEL_DISCOMP_OFF);
    }

#if 0
	//if (UI_GetData(FL_ModeIndex) == DSC_MODE_MOVIE)
	{

#if (MOVIE_DIS == ENABLE)
		if (uiSelect == MOVIE_DIS_ON) {
			// disable RSC
			UI_SetData(FL_MovieRSCIndex, MOVIE_RSC_OFF);
#if (MOVIE_RSC == ENABLE)
			//rsc tmp
			MovieExe_RSC_SetSwitch(SEL_RSC_RUNTIME, SEL_RSC_OFF);
#endif
			// enable DIS
			MovieExe_IPL_SetIInfo(IPL_SEL_DISCOMP_IMM, SEL_DISCOMP_ON);
		} else {
			MovieExe_IPL_SetIInfo(IPL_SEL_DISCOMP_IMM, SEL_DISCOMP_OFF);
		}
#endif
	}
#endif

	return NVTEVT_CONSUME;
}

INT32 MovieExe_OnMCTF(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
	UINT32 uiSelect = 0;

	if (paramNum) {
		uiSelect = paramArray[0];
	}

	UI_SetData(FL_MovieMCTFIndex, uiSelect);

	return NVTEVT_CONSUME;
}

INT32 MovieExe_OnEdge(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
	UINT32 uiSelect = 0;

	if (paramNum) {
		uiSelect = paramArray[0];
	}
	UI_SetData(FL_EDGE, uiSelect);

	if (uiSelect == MOVIE_EDGE_ON) {
		MovieExe_IQ_SetUIInfo(IQ_UI_IPPEDGE, IQ_EDGE_ON);
	} else {
		MovieExe_IQ_SetUIInfo(IQ_UI_IPPEDGE, IQ_EDGE_OFF);
	}
	return NVTEVT_CONSUME;
}

INT32 MovieExe_OnNR(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
	UINT32 uiSelect = 0;

	if (paramNum) {
		uiSelect = paramArray[0];
	}

	UI_SetData(FL_NR, uiSelect);

	if (uiSelect == MOVIE_NR_ON) {
		MovieExe_IQ_SetUIInfo(IQ_UI_IPPNR, IQ_NR_ON);
	} else {
		MovieExe_IQ_SetUIInfo(IQ_UI_IPPNR, IQ_NR_OFF);
	}
	return NVTEVT_CONSUME;
}

INT32 MovieExe_OnRSC(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{

#if (MOVIE_RSC == ENABLE)

	UINT32 uiSelect = 0;
	if (paramNum) {
		uiSelect = paramArray[0];
	}

	UI_SetData(FL_MovieRSCIndex, uiSelect);
#if !defined(_Gyro_None_)
	if (uiSelect == MOVIE_RSC_ON) {
		// disable DIS
		UI_SetData(FL_MovieDisIndex, MOVIE_DIS_OFF);
#if (MOVIE_DIS_COMP_FUNC == ENABLE)
		MovieExe_IPL_SetIInfo(IPL_SEL_DISCOMP_IMM, SEL_DISCOMP_OFF);
#endif
		// enable RSC if necessary (only support frame rate smaller than 60fps)
		//rsc tmp
		if ((UI_GetData(FL_MovieRSCIndex) == MOVIE_RSC_ON) &&
			(gMovie_Rec_Info[0].frame_rate <= 120)) {
			MovieExe_RSC_SetSwitch(SEL_RSC_RUNTIME, SEL_RSC_ON);

			if (UI_GetData(FL_MOVIE_HDR) == MOVIE_HDR_ON) {
				Ux_SendEvent(&CustomMovieObjCtrl, NVTEVT_EXE_SHDR, 1, SHDR_OFF);
			}
		} else {
			MovieExe_RSC_SetSwitch(SEL_RSC_RUNTIME, SEL_RSC_OFF);
		}
	} else {
		MovieExe_RSC_SetSwitch(SEL_RSC_RUNTIME, SEL_RSC_OFF);
	}
#endif
#endif
	return NVTEVT_CONSUME;
}

INT32 MovieExe_OnWDR(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
	UINT32 uiSelect = 0;

	if (paramNum) {
		uiSelect = paramArray[0];
	}

	if (uiSelect == MOVIE_WDR_ON) {
		UI_SetData(FL_MOVIE_WDR, MOVIE_WDR_ON);
		MovieExe_IPL_SetIInfo(IPL_SEL_WDR_IMM, SEL_WDR_AUTO);

		if (UI_GetData(FL_MOVIE_HDR) == MOVIE_HDR_ON) {
			Ux_SendEvent(&CustomMovieObjCtrl, NVTEVT_EXE_SHDR, 1, MOVIE_HDR_OFF);
		}
	} else {
		UI_SetData(FL_MOVIE_WDR, MOVIE_WDR_OFF);
		MovieExe_IPL_SetIInfo(IPL_SEL_WDR_IMM, SEL_WDR_OFF);
	}

	return NVTEVT_CONSUME;
}

INT32 MovieExe_OnSHDR(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    UINT32 uhSelect = 0;

    DBG_IND("%d \r\n", paramArray[0]);

	if (paramNum > 0) {
		uhSelect = paramArray[0];
	}

    UI_SetData(FL_MOVIE_HDR, uhSelect);

	if (UI_GetData(FL_MOVIE_HDR) == MOVIE_HDR_ON) {
		MovieExe_IPL_SetIInfo(IPL_SEL_SHDR, SEL_SHDR_ON);
		MovieExe_IPL_SetIInfo(IPL_SEL_SHDR_FRAME_NUM, DVCAM_SHDR_MAX_FRAME);
	} else {
		MovieExe_IPL_SetIInfo(IPL_SEL_SHDR, SEL_SHDR_OFF);
	}


#if 0
#if SHDR_FUNC
	UINT32 uhSelect = 0;
	UINT32 bChangeMode = FALSE;

	DBG_IND("%d \r\n", paramArray[0]);
	if (paramNum > 0) {
		uhSelect = paramArray[0];
	}

	if (paramNum > 1) {
		bChangeMode = paramArray[1];
	}

	UI_SetData(FL_MOVIE_HDR, uhSelect);

	if (UI_GetData(FL_MOVIE_HDR) == MOVIE_HDR_ON) {
		MovieExe_IPL_SetIInfo(IPL_SEL_SHDR, SEL_SHDR_ON);
	} else {
		MovieExe_IPL_SetIInfo(IPL_SEL_SHDR, SEL_SHDR_OFF);
	}

	if (UI_GetData(FL_MOVIE_HDR) == MOVIE_HDR_ON) { // && UI_GetData(FL_MOVIE_WDR) == MOVIE_WDR_ON)
		UI_SetData(FL_MOVIE_WDR, MOVIE_WDR_OFF);
		UI_SetData(FL_MovieRSCIndex, MOVIE_RSC_OFF);
	}

	// need to off to preview again
	if (bChangeMode) {
		Ux_PostEvent(NVTEVT_SYSTEM_MODE, 1, System_GetState(SYS_STATE_CURRMODE));
	} else {
		DBG_WRN("should change mode\r\n");
	}
#endif
#endif

	return NVTEVT_CONSUME;
}

INT32 MovieExe_OnSensorRotate(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
	UINT32 uiSelect = 0;

	if (paramNum) {
		uiSelect = paramArray[0];
	}
	//#NT#2016/03/02#Niven Cho -begin
	//#NT#Use chip filp instead of sensor flip
	//#NT#2018/02/14#KCHong -begin
	//#NT#support sensor rotate setting
	if (uiSelect == SEN_ROTATE_ON) {
		UI_SetData(FL_MOVIE_SENSOR_ROTATE, SEN_ROTATE_ON);
		gMovie_Rec_Info[0].sensor_rotate = TRUE;
	} else {
		UI_SetData(FL_MOVIE_SENSOR_ROTATE, SEN_ROTATE_OFF);
		gMovie_Rec_Info[0].sensor_rotate = FALSE;
	}
	//#NT#2018/02/14#KCHong -end
	if (System_GetEnableSensor() & SENSOR_1) {
		ImageUnit_Begin(ISF_IPL(0), 0);
		ImageUnit_SetVdoDirection(ISF_IN1, (UI_GetData(FL_MOVIE_SENSOR_ROTATE) == SEN_ROTATE_ON) ? ISF_VDO_DIR_MIRRORY : ISF_VDO_DIR_NONE);
		ImageUnit_End();
	}
	//#NT#2016/03/02#Niven Cho -end
	if (System_GetEnableSensor() & SENSOR_2) {
		ImageUnit_Begin(ISF_IPL(1), 0);
		ImageUnit_SetVdoDirection(ISF_IN1, 0);
		ImageUnit_End();
	}
	//ImageStream_UpdateSize();
	ImageStream_UpdateAll(&ISF_Stream[0]);
	return NVTEVT_CONSUME;
}

INT32 MovieExe_OnIRCut(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
//#NT#2016/02/22#Niven Cho -begin
//#NT#add IR_CUT_FUNC to enable IR CUT function.
#if (IR_CUT_FUNC == ENABLE)
	UINT32 uiSelect = 0;

	if (paramNum) {
		uiSelect = paramArray[0];
	}

	DBG_IND("%d \r\n", uiSelect);
	if (uiSelect == MOVIE_IR_CUT_ON) {
		UI_SetData(FL_MOVIE_IR_CUT, MOVIE_IR_CUT_ON);
		MovieExe_IQ_SetUIInfo(IQ_UI_IRCUT, IQ_IRCUT_DAY);
		UIMovie_IRCutCtrl(ON);
	}
#if defined(_UI_STYLE_IPCAM_)
	else if (uiSelect == MOVIE_IR_CUT_AUTO) {
		UI_SetData(FL_MOVIE_IR_CUT, MOVIE_IR_CUT_AUTO);
		MovieExe_IQ_SetUIInfo(IQ_UI_IRCUT,  IQ_IRCUT_AUTO);
	}
#endif
	else {
		UI_SetData(FL_MOVIE_IR_CUT, MOVIE_IR_CUT_OFF);
		MovieExe_IQ_SetUIInfo(IQ_UI_IRCUT,  IQ_IRCUT_NIGHT);
		UIMovie_IRCutCtrl(OFF);
	}
#endif
//#NT#2016/02/22#Niven Cho -end
	return NVTEVT_CONSUME;
}

INT32 MovieExe_OnGdc(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
	UINT32 uhSelect = 0;
	if (paramNum > 0) {
		uhSelect = paramArray[0];
	}

	DBG_IND("%d \r\n", uhSelect);
	if (uhSelect == TRUE) {
#if !defined(_Gyro_None_)
		if (UI_GetData(FL_MovieRSCIndex) == MOVIE_RSC_ON) {
			MovieExe_RSC_SetSwitch(SEL_RSC_RUNTIME, SEL_RSC_OFF);
			DBG_WRN("GDC is enable, so RSC is forced off\r\n");
		}
#endif
		MovieExe_IQ_SetUIInfo(IQ_UI_GDCCOMP, IQ_GDCCOMP_ON);
	} else {
		MovieExe_IQ_SetUIInfo(IQ_UI_GDCCOMP, IQ_GDCCOMP_OFF);
	}
	return NVTEVT_CONSUME;
}

INT32 MovieExe_OnSmear(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
	UINT32 uhSelect = 0;
	if (paramNum > 0) {
		uhSelect = paramArray[0];
	}
	DBG_IND("+-idx=%d\r\n", uhSelect);

	UI_SetData(FL_MovieSmearIndex, uhSelect);
#if(MOVIE_SMEAR_R_FUNC ==ENABLE)
	AppPhoto_SetData(&CustomPhotoObjCtrl, _SmearModeEn,
					 (uhSelect == MOVIE_SMEAR_ON) ? _SmearMode_on : _SmearMode_off);
#endif
	return NVTEVT_CONSUME;
}

//-------------------------------------------------------------------------------------------------
INT32 MovieExe_OnDateImprint(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
	UINT32 uiSelect = 0;

	if (paramNum) {
		uiSelect = paramArray[0];
	}

	UI_SetData(FL_MOVIE_DATEIMPRINT, uiSelect);

	return NVTEVT_CONSUME;
}
//-------------------------------------------------------------------------------------------------
INT32 MovieExe_OnGSENSOR(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
	UINT32 uiSelect = 0;

	if (paramNum) {
		uiSelect = paramArray[0];
	}

	UI_SetData(FL_GSENSOR, uiSelect);

#if !defined(_GSensor_None_)
	switch (UI_GetData(FL_GSENSOR)) {
	case GSENSOR_OFF:
		GSensor_SetSensitivity(GSENSOR_SENSITIVITY_OFF);
		break;
	case GSENSOR_LOW:
		GSensor_SetSensitivity(GSENSOR_SENSITIVITY_LOW);
		break;
	case GSENSOR_MED:
		GSensor_SetSensitivity(GSENSOR_SENSITIVITY_MED);
		break;
	case GSENSOR_HIGH:
		GSensor_SetSensitivity(GSENSOR_SENSITIVITY_HIGH);
		break;
	default:
		GSensor_SetSensitivity(GSENSOR_SENSITIVITY_OFF);
		break;
	}
#endif

	//debug_err(("MovieExe_OnGSENSOR =%d\r\n ",FL_GSENSOR));
	return NVTEVT_CONSUME;
}

//-------------------------------------------------------------------------------------------------
#if MOVIE_MAPPING_MULTIREC
INT32 MovieExe_OnCyclicRec(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
	UINT32 uiSelect = 0;
	UINT32 i, mask;
	UINT32 movie_rec_mask, clone_rec_mask;

	uiSelect = paramNum ? paramArray[0] : MOVIE_CYCLICREC_3MIN;

	UI_SetData(FL_MOVIE_CYCLIC_REC, uiSelect);

	switch (uiSelect) {
	case MOVIE_CYCLICREC_1MIN:
		gMovie_Rec_Option.seamless_sec = 60;
		break;

	case MOVIE_CYCLICREC_3MIN:
		gMovie_Rec_Option.seamless_sec = 180;
		break;

	case MOVIE_CYCLICREC_5MIN:
		gMovie_Rec_Option.seamless_sec = 300;
		break;

	case MOVIE_CYCLICREC_10MIN:
		gMovie_Rec_Option.seamless_sec = 600;
		break;

	case MOVIE_CYCLICREC_OFF:
		break;

	default:
		gMovie_Rec_Option.seamless_sec = 300;
		break;
	}

	movie_rec_mask = Movie_GetMovieRecMask();
	clone_rec_mask = Movie_GetCloneRecMask();

	mask = 1;
	for (i = 0; i < SENSOR_CAPS_COUNT; i++) {
		if (movie_rec_mask & mask) {
			ImageApp_MovieMulti_SetParam(gMovie_Rec_Info[i].rec_id, MOVIEMULTI_PARAM_FILE_SEAMLESSSEC, gMovie_Rec_Option.seamless_sec);
			ImageApp_MovieMulti_SetParam(gMovie_Rec_Info[i].rec_id, MOVIEMULTI_PARAM_FILE_ENDTYPE, (uiSelect == MOVIE_CYCLICREC_OFF)? MOVREC_ENDTYPE_NORMAL : MOVREC_ENDTYPE_CUTOVERLAP);
		}

		if (clone_rec_mask & mask) {
			ImageApp_MovieMulti_SetParam(gMovie_Clone_Info[i].rec_id, MOVIEMULTI_PARAM_FILE_SEAMLESSSEC, gMovie_Rec_Option.seamless_sec);
			ImageApp_MovieMulti_SetParam(gMovie_Clone_Info[i].rec_id, MOVIEMULTI_PARAM_FILE_ENDTYPE, (uiSelect == MOVIE_CYCLICREC_OFF)? MOVREC_ENDTYPE_NORMAL : MOVREC_ENDTYPE_CUTOVERLAP);
		}

		mask <<= 1;
	}

	return NVTEVT_CONSUME;
}
#else
INT32 MovieExe_OnCyclicRec(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
	UINT32 uiSelect = 0;

	uiSelect = paramNum ? paramArray[0] : MOVIE_CYCLICREC_3MIN;

	UI_SetData(FL_MOVIE_CYCLIC_REC, uiSelect);

	switch (uiSelect) {
	case MOVIE_CYCLICREC_1MIN:
		gMovie_Rec_Option.seamless_sec = 60;
		break;

	case MOVIE_CYCLICREC_3MIN:
		gMovie_Rec_Option.seamless_sec = 180;
		break;

	case MOVIE_CYCLICREC_5MIN:
		gMovie_Rec_Option.seamless_sec = 300;
		break;

	case MOVIE_CYCLICREC_10MIN:
		gMovie_Rec_Option.seamless_sec = 600;
		break;

	case MOVIE_CYCLICREC_OFF:
		break;

	default:
		gMovie_Rec_Option.seamless_sec = 300;
		break;
	}

	if (System_GetState(SYS_STATE_CURRMODE) == PRIMARY_MODE_MOVIE) {
		ImageApp_Movie_SetParam(_CFG_REC_ID_1, MOVIE_PARAM_FILE_SEAMLESSSEC, gMovie_Rec_Option.seamless_sec);
		ImageApp_Movie_SetParam(_CFG_REC_ID_1, MOVIE_PARAM_FILE_ENDTYPE, (uiSelect == MOVIE_CYCLICREC_OFF)? MOVREC_ENDTYPE_NORMAL : MOVREC_ENDTYPE_CUTOVERLAP);
	}
	return NVTEVT_CONSUME;
}
#endif

//-------------------------------------------------------------------------------------------------
INT32 MovieExe_OnMotionDet(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
	UINT32 uiSelect = 0;

	if (paramNum) {
		uiSelect = paramArray[0];
	}

	UI_SetData(FL_MOVIE_MOTION_DET, uiSelect);

#if (MOVIE_BC_MD == ENABLE)
	Movie_BCProcessRateSet(GetMovieFrameRate(UI_GetData(FL_MOVIE_SIZE)) / 10);
	Movie_BCLock(uiSelect ? FALSE : TRUE);
#endif

	return NVTEVT_CONSUME;
}

static void MovieExe_InitPhotoFunc(void)
{
	PHOTO_FUNC_INFO *phead = NULL;
	//
	// Setup IPL information.
	//
	phead = &PhotoFuncInfo_dummy;
	phead->pNext = NULL; //mark end

	alg_Movie_InitPhotoFunc(phead);

	UIPhotoFunc_Open(phead);
}

static void MovieExe_FileNamingCB(MOVIE_CFG_REC_ID id, char *pFileName)
{
    static struct tm   CurDateTime = {0};

    CurDateTime = HwClock_GetTime(TIME_ID_CURRENT);

    snprintf(pFileName, NMC_TOTALFILEPATH_MAX_LEN, "%04d%02d%02d_%02d%02d%02d",
            CurDateTime.tm_year, CurDateTime.tm_mon, CurDateTime.tm_mday,
            CurDateTime.tm_hour, CurDateTime.tm_min, CurDateTime.tm_sec);

}

#define MOVIE_DUAL_RECORD_TEST          0//1
#define MOVIE_TIMELAPSE_TIME           60000
#define MOVIE_TIMELAPSE_PLAYFRAMERATE     30

#if (MOVIE_MAPPING_MULTIREC != ENABLE)
static MOVIE_RECODE_INFO gMovie_Rec_Info[1] = {
	_CFG_REC_ID_1,            //MOVIE_CFG_REC_ID
	_CFG_VID_IN_1,            //MOVIE_CFG_VID_IN
	{1920,1080},              //MOVIE_CFG_IMG_SIZE
	30,                       //MOVIE_CFG_FRAME_RATE
	2 * 1024 * 1024,          //MOVIE_CFG_TARGET_RATE
	#if (defined(_CHIP_96683_) && (defined(_MODEL_WiFiSDV_) || defined(_MODEL_FIREFLY_)|| (defined(_MODEL_B5_))) )
	_CFG_CODEC_H265,          //MOVIE_CFG_CODEC
	#else
	_CFG_CODEC_H264,          //MOVIE_CFG_CODEC
    #endif
	_CFG_AUD_CODEC_AAC,       //MOVIE_CFG_AUD_CODEC
	_CFG_REC_TYPE_AV,         //MOVIE_CFG_REC_MODE
	_CFG_FILE_FORMAT_MP4,     //MOVIE_CFG_FILE_FORMAT
	{16, 9},                  //MOVIE_CFG_IMG_RATIO
	{1920, 1080},             //MOVIE_CFG_RAWENC_SIZE
	0,                        //MOVIE_CFG_IMAGEPIPE_FUNC (IMAGEPIPE_FUNC_SQUARE)
	ENABLE,                   //MOVIE_CFG_DISP_ENABLE
	TRUE,                     //ipl_set_enable,  FALSE: can not set sensor info, TRUE: can set sensor info
	_CFG_DAR_DEFAULT,         //MOVIE_CFG_DAR
	{0},                      //MOVIE_CFG_AQ_INFO
	{0},                      //MOVIE_CFG_CBR_INFO
	FALSE,                    //Sensor rotate setting
	0,                        //MOVIE_CFG_IMAGEPIPE_FUNC2
};

//#NT#2018/04/02#IsiahChang -begin
//#NT#To support Wi-Fi command 2025 BRC.
static MOVIE_STRM_INFO gMovie_Strm_Info = {
	_CFG_STRM_ID_1,            //MOVIE_CFG_REC_ID
	_CFG_VID_IN_1,             //MOVIE_CFG_VID_IN
	//{848,480},                 //MOVIE_CFG_IMG_SIZE
	{0},                       //MOVIE_CFG_IMG_SIZE. Defined by g_MovieRecSizeTable[].
	//30,                        //MOVIE_CFG_FRAME_RATE
	0,                         //MOVIE_CFG_FRAME_RATE. Defined by g_MovieRecSizeTable[].
	//350 * 1024,                //MOVIE_CFG_TARGET_RATE
	0,                         //MOVIE_CFG_TARGET_RATE. Defined by g_MovieRecSizeTable[].
	_CFG_CODEC_H264,           //MOVIE_CFG_CODEC
	//15,                        //MOVIE_CFG_GOP_NUM
	0,                         //MOVIE_CFG_GOP_NUM. Defined by g_MovieRecSizeTable[].
	_CFG_AUD_CODEC_AAC,        //MOVIE_CFG_AUD_CODEC
	TRUE,                      //MOVIE_CFG_RTSP_STRM_OUT
	//{1,	2, 36, 6, -6, 0},	 //MOVIE_CFG_AQ_INFO
	{0},	                   //MOVIE_CFG_AQ_INFO, Defined by g_MovieRecSizeTable[].
	//{1, 4, 30, 350 * 1024, 15, 26, 10, 40, 26, 10, 40, 6, 0, 0, 0},		//MOVIE_CFG_CBR_INFO
	{0},                       // MOVIE_CFG_CBR_INFO, Defined by g_MovieRecSizeTable[].
	FALSE, 						// ipl_set_enable,  FALSE: can not set sensor info, TRUE: can set sensor info
	{16,9},						//MOVIE_CFG_IMG_RATIO
	#if(MOVIE_STRM_PATH_USERPROC == ENABLE)
		TRUE						//userproc_pull
	#else
		FALSE						//userproc_pull
	#endif
};
//#NT#2018/04/02#IsiahChang -end

static MOVIE_RECODE_FILE_OPTION gMovie_Rec_Option = {
	_CFG_REC_ID_1,             //MOVIE_CFG_REC_ID
	180,                       //MOVIE_CFG_FILE_SECOND seamless_sec
#if (MOVIE_MAPPING_MULTIREC == ENABLE)
	_CFG_EMR_OFF,              //MOVIE_CFG_FILE_OPTION emr_on
#else
	FALSE,                     //MOVIE_CFG_FILE_OPTION emr_on
#endif
	3,                         //MOVIE_CFG_FILE_OPTION emr_sec (rollback sec)
	5,                         //MOVIE_CFG_FILE_SECOND keep_sec
	0,                         //MOVIE_CFG_FILE_SECOND overlap_sec (only 0 or 1 is valid)
	MOVREC_ENDTYPE_CUTOVERLAP, //MOVIE_CFG_FILE_ENDTYPE  end_type
};

#if MOVIE_DUAL_RECORD_TEST
static MOVIE_RECODE_INFO gMovie_Rec_Info2 = {
	_CFG_REC_ID_2,             //MOVIE_CFG_REC_ID
	_CFG_VID_IN_2,             //MOVIE_CFG_VID_IN
	{1920,1080},               //MOVIE_CFG_IMG_SIZE
	30,                        //MOVIE_CFG_FRAME_RATE
	2 * 1024 * 1024,           //MOVIE_CFG_TARGET_RATE
	_CFG_CODEC_H264,           //MOVIE_CFG_CODEC
	_CFG_AUD_CODEC_AAC,        //MOVIE_CFG_AUD_CODEC
	_CFG_REC_TYPE_AV,          //MOVIE_CFG_REC_MODE
	_CFG_FILE_FORMAT_MP4,      //MOVIE_CFG_FILE_FORMAT
	{16, 9},                   //MOVIE_CFG_IPL_RATIO
	{1920, 1080},              //MOVIE_CFG_RAWENC_SIZE
	0,                         //MOVIE_CFG_IMAGEPIPE_FUNC (IMAGEPIPE_FUNC_SQUARE)
	ENABLE,                    //MOVIE_CFG_DISP_ENABLE
	FALSE,                     //ipl_set_enable,  FALSE: can not set sensor info, TRUE: can set sensor info
	_CFG_DAR_DEFAULT,          //MOVIE_CFG_DAR
	{0},                       //MOVIE_CFG_AQ_INFO
	{0},                       //MOVIE_CFG_CBR_INFO
	FALSE,                     //Sensor rotate setting
	0,                         //MOVIE_CFG_IMAGEPIPE_FUNC2

};

static MOVIE_RECODE_FILE_OPTION gMovie_Rec_Option2 = {
	_CFG_REC_ID_2,             //MOVIE_CFG_REC_ID
	180,                       //MOVIE_CFG_FILE_SECOND seamless_sec
	FALSE,                     //MOVIE_CFG_FILE_OPTION emr_on
	3,                         //MOVIE_CFG_FILE_OPTION emr_sec (rollback sec)
	5,                         //MOVIE_CFG_FILE_SECOND keep_sec
	0,                         //MOVIE_CFG_FILE_SECOND overlap_sec (only 0 or 1 is valid)
	MOVREC_ENDTYPE_CUTOVERLAP, //MOVIE_CFG_FILE_ENDTYPE  end_type
};

#endif

static MOVIE_AUDIO_INFO   gMovie_Audio_Info = {
	32000,                     //MOVIE_CFG_AUD_RATE
	AUDIO_CH_STEREO,           //MOVIE_CFG_AUD_CH
};

MOVIE_CFG_DISP_INFO gMovie_Disp_Info = {
    VDO_ROTATE_DIR,
#if MOVIE_DUAL_RECORD_TEST
    TRUE,
#else
    FALSE,
#endif
};
#endif

#if MOVIE_UVAC_FUNC
#include "ImageUnit_UsbUVAC.h"
#include "UVAC.h"

static MOVIE_UVAC_INFO gMovie_Uvac_Info = {
    _CFG_UVAC_ID_1,            //MOVIE_CFG_REC_ID
    _CFG_VID_IN_1,             //MOVIE_CFG_VID_IN
    {1920,1080},               //MOVIE_CFG_IMG_SIZE
    30,                        //MOVIE_CFG_FRAME_RATE
    2 * 1024 * 1024,           //MOVIE_CFG_TARGET_RATE
    _CFG_UVC_CODEC_H264,       //MOVIE_CFG_UVC_CODEC
    _CFG_AUD_CODEC_NONE,       //MOVIE_CFG_AUD_CODECLE
	{1,	2, 36, 6, -6, 0},	   //MOVIE_CFG_AQ_INFO
	{1, 4, 30, 2 * 1024 * 1024, 15, 10, 40, 10, 40, 6, 0, 0, 0},		//MOVIE_CFG_CBR_INFO
    FALSE,    					// ipl_set_enable,  FALSE: can not set sensor info, TRUE: can set sensor info
    {16,9}						//MOVIE_CFG_IMG_RATIO
};

static UVAC_VID_RESO gUIUvacVidReso[1] = {
	{1920,  1080,   1,      30,      0,      0},        //16:9
};

//NVT_UI_UVAC_AUD_SAMPLERATE_CNT <= UVAC_AUD_SAMPLE_RATE_MAX_CNT
static UINT32 gUIUvacAudSampleRate[1] = {
	32000
};

_ALIGNED(64) static UINT16 m_UVACSerialStrDesc3[] = {
	0x0320,                             // 20: size of String Descriptor = 32 bytes
	// 03: String Descriptor type
	'9', '6', '6', '8', '0',            // 96611-00000-001 (default)
	'0', '0', '0', '0', '0',
	'0', '0', '1', '0', '0'
};

_ALIGNED(64) const static UINT8 m_UVACManuStrDesc[] = {
	USB_VENDER_DESC_STRING_LEN * 2 + 2, // size of String Descriptor = 6 bytes
	0x03,                       // 03: String Descriptor type
	USB_VENDER_DESC_STRING
};

_ALIGNED(64) const static UINT8 m_UVACProdStrDesc[] = {
	USB_PRODUCT_DESC_STRING_LEN * 2 + 2, // size of String Descriptor = 6 bytes
	0x03,                       // 03: String Descriptor type
	USB_PRODUCT_DESC_STRING
};

static UVAC_VEND_DEV_DESC gUIMovUvacDevDesc = {0};

static void xUSBMakerInit_UVAC(UVAC_VEND_DEV_DESC *pUVACDevDesc)
{
	pUVACDevDesc->pManuStringDesc = (UVAC_STRING_DESC *)m_UVACManuStrDesc;
	pUVACDevDesc->pProdStringDesc = (UVAC_STRING_DESC *)m_UVACProdStrDesc;
	pUVACDevDesc->pSerialStringDesc = (UVAC_STRING_DESC *)m_UVACSerialStrDesc3;
	pUVACDevDesc->VID = USB_VID;
	pUVACDevDesc->PID = USB_PID_PCCAM;
}
#endif

#define MOVIE_ROOT_PATH         "A:\\Novatek\\"
#define MOVIE_ROOT_PATH2        "B:\\Novatek\\"

//static void MovieExe_UserEventCb(MOVIE_CFG_REC_ID id, MOVIE_USER_CB_EVENT event_id, UINT32 value)
static void MovieExe_UserEventCb(UINT32 id, MOVIE_USER_CB_EVENT event_id, UINT32 value)
{
	switch ((UINT32)event_id) {
	case MOVIE_USER_CB_EVENT_STAMP_CB: {
        		if (value) {
        	            MP_VDOENC_YUV_SRC* p_yuv_src   = NULL;
        	            p_yuv_src = (MP_VDOENC_YUV_SRC*)value;
#if (VIDEOSPRITE_STAMP_FUNC == ENABLE)
        	            NMI_IMGCAP_YUV * pImgCapYUV = (NMI_IMGCAP_YUV *)value;

        	            if ((pImgCapYUV->thumb== 2) && (UI_GetData(FL_MOVIE_PIM) == MOVIE_PIM_ON) && (UI_GetData(FL_MOVIE_DATEIMPRINT) == MOVIE_DATEIMPRINT_ON))//2:PIM; 1:Thumb
        	            {
        	                videosprite_draw_ext(pImgCapYUV->path_id, pImgCapYUV->y_addr, pImgCapYUV->u_addr, pImgCapYUV->v_addr, pImgCapYUV->y_loff,  pImgCapYUV->img_w, pImgCapYUV->img_h);
        	            }
        	            else if(pImgCapYUV->thumb== 1 && (UI_GetData(FL_MOVIE_DATEIMPRINT) == MOVIE_DATEIMPRINT_ON))//2:PIM; 1:Thumb
        	            {
            	                //set coe thumb data
        	                MovieStamp_UpdateData_Thumb(pImgCapYUV->path_id, FALSE);
        	                videosprite_draw_ext(pImgCapYUV->path_id, pImgCapYUV->y_addr, pImgCapYUV->u_addr, pImgCapYUV->v_addr, pImgCapYUV->y_loff,  pImgCapYUV->img_w, pImgCapYUV->img_h);
        	                //restore main stream coe data
        	                MovieStamp_RestoreData(pImgCapYUV->path_id);
        	            }
        	            else
#endif
        	            {
        	                videosprite_draw_btn_ext(p_yuv_src->uiVidPathID,
        	                p_yuv_src->uiYAddr,
        	                p_yuv_src->uiCbAddr,
        	                p_yuv_src->uiCrAddr,
        	                p_yuv_src->uiYLineOffset,
        	                p_yuv_src->uiWidth,
        	                p_yuv_src->uiHeight);
        	            }


        	            if (UI_GetData(FL_MOVIE_DATEIMPRINT) == MOVIE_DATEIMPRINT_ON) {
#if (THUMB_STAMP_FUNC == ENABLE)
        		        NMI_IMGCAP_YUV * pCapYUV = (NMI_IMGCAP_YUV *)value;

        	            if (pCapYUV->thumb==1) {
        	                MovieStamp_CopyData_Thumb(pCapYUV->path_id,
        	                pCapYUV->y_addr,
        	                pCapYUV->u_addr,
        	                pCapYUV->v_addr,
        	                pCapYUV->y_loff,
        	                pCapYUV->img_w);
        	            }
        	            else
#endif
        	            {
        	                MovieStamp_CopyData(p_yuv_src->uiVidPathID,
        	                p_yuv_src->uiYAddr,
        	                p_yuv_src->uiCbAddr,
        	                p_yuv_src->uiCrAddr,
        	                p_yuv_src->uiYLineOffset,
        	                p_yuv_src->uiWidth);
        	            }
        	        }
        	    }
        	}
		break;
	case MOVIE_USER_CB_EVENT_REC_ONE_SECOND: {
			#if (LOG_DBGINFO_IN_GPS_SECTION == ENABLE)
			if (value == 0) {
				mp4log_ResetFrmCnt();
			}
			#if (GPS_FUNCTION == ENABLE)
			RMCINFO RMCInfo;
			GPSRec_GetRMCDate(&RMCInfo);
			mp4log_AddLog(0, (char *)&RMCInfo, sizeof(RMCINFO));
			#endif  // #if (GPS_FUNCTION == ENABLE)
			MEM_RANGE log_buf;
			if (mp4log_ToggleBuf(&(log_buf.Addr), NULL) == E_OK) {
				log_buf.Size = mp4log_GetTotalLogSize() * 1024 - 16;
#if (MOVIE_MAPPING_MULTIREC == ENABLE)
				// since all file share same gps data, only config once
				ImageApp_MovieMulti_SetParam(_CFG_REC_ID_1, MOVIEMULTI_PARAM_GPS_DATA, (UINT32)&log_buf);
#else
				ImageApp_Movie_SetParam(_CFG_REC_ID_1, MOVIE_PARAM_GPS_DATA, (UINT32)&log_buf);
#endif
			}
			#endif  // #if (LOG_DBGINFO_IN_GPS_SECTION == ENABLE)
			Ux_PostEvent(NVTEVT_CB_MOVIE_REC_ONE_SEC, 1,  value);
		}
		break;

	case MOVIE_USER_CB_EVENT_FILENAMING_MOV_CB:
	case MOVIE_USER_CB_EVENT_FILENAMING_EMR_CB: {
		// if MOVIE_CFG_FILE_NAMING is TRUE, will callback for naming file
		CHAR *pFileName = (CHAR *) value;

		// Make custome file name
		MovieExe_FileNamingCB(id, pFileName);
        }
        break;

	case MOVIE_USER_CB_ERROR_CARD_SLOW: {
		DBG_ERR("Slow card!,id=%d\r\n", value);
		//#NT#2017/11/02#Adam Su -begin
		//#NT#AUTO_TEST
		exam_msg("slow card\r\n");
		//#NT#2017/11/02#Adam Su -end
		Ux_PostEvent(NVTEVT_CB_MOVIE_SLOW, 0);
		}
		break;

	case MOVIE_USER_CB_ERROR_SEAMLESS_REC_FULL: {
		DBG_ERR("EVENT 0x%08x: looprec full!\r\n", event_id);
		Ux_PostEvent(NVTEVT_CB_MOVIE_LOOPREC_FULL, 0);
		}
		break;

	case MOVIE_USER_CB_EVENT_SNAPSHOT_OK: {
		DBG_IND("EVENT 0x%08x: snapshop ok!\r\n", event_id);
		Ux_PostEvent(NVTEVT_CB_RAWENC_OK, 0);
		}
		break;

	case MOVIE_USER_CB_ERROR_SNAPSHOT_ERR: {
		DBG_ERR("EVENT 0x%08x: snapshop err!\r\n", event_id);
		Ux_PostEvent(NVTEVT_CB_RAWENC_ERR, 0);
		}
		break;

	case MOVIE_USER_CB_EVENT_CUTFILE: {
		// Cyclic record cut file callback
        DBG_IND("----->EVENT 0x%08x: Cut file end!\r\n", event_id);
		}
		break;

	case MOVIE_USER_CB_EVENT_OVERTIME: {
		DBG_IND("EVENT 0x%08x: Overtime!\r\n", event_id);
		Ux_PostEvent(NVTEVT_CB_MOVIE_OVERTIME, 0);
		}
		break;
	case MOVIE_USER_CB_EVENT_EMR_FILE_COMPLETED:{
		Ux_PostEvent(NVTEVT_CB_EMR_COMPLETED, 2, id, event_id);
		}
		break;
	case MOVIE_USER_CB_EVENT_CARSH_FILE_COMPLETED:{
		Ux_PostEvent(NVTEVT_CB_EMR_COMPLETED, 2, id, event_id);
		}
		break;
	case MOVIE_USER_CB_EVENT_CLOSE_RESULT: {
		if (value == MOVREC_EVENT_RESULT_NORMAL) {
			// normal stop
			DBG_IND("EVENT 0x%08x: REC_FILE_COMPLETED!\r\n", event_id);
			Ux_PostEvent(NVTEVT_CB_MOVIE_REC_FINISH, 1, value);
		} else if (value == MOVREC_EVENT_RESULT_WRITE_ERR) {
			// write error
			DBG_ERR("EVENT %d: HW error!\r\n", event_id);
			Ux_PostEvent(NVTEVT_CB_MOVIE_WR_ERROR, 0);
		} else if (value == MOVREC_EVENT_LOOPREC_FULL) {
			// loop record full
			DBG_ERR("EVENT 0x%08x: looprec full!\r\n", event_id);
		} else if (value == MOVREC_EVENT_RESULT_SLOW) {
			// slow
			// do nothing due to restart flow in controlled by MOVIE_USER_CB_ERROR_CARD_SLOW callback messge.
			DBG_ERR("EVENT 0x%08x: Slow card!\r\n", event_id);
		} else if (value == MOVREC_EVENT_RESULT_FULL) {
			DBG_IND("EVENT 0x%08x: REC_FILE_COMPLETED!\r\n", event_id);
			// card full
			DBG_ERR("EVENT %d: Full!\r\n", event_id);
			Ux_PostEvent(NVTEVT_CB_MOVIE_FULL, 0);
		}
		}
		break;
	}

}

#if MOVIE_DUAL_RECORD_TEST
static USERPROC_TRIG_PROC_VAL MovieExe_UserProc_PushCB(ISF_PORT *pPort, ISF_DATA *pData)
{
	return USERPROC_TRIG_PROC_EN;
}

static void MovieExe_UserProc_ProcessCB(UINT32 iPort)
{
    // PIP view
	ISF_DATA         isf_data1, isf_data2;
	ISF_PORT         *pSrc1, *pSrc2;

	APPDISP_VIEW_DRAW         pip_draw;
	APPDISP_VIEW_DRAW         *pdraw = &pip_draw;

    pSrc1 = ImageApp_Movie_Get_Pip_ImgSrc(gMovie_Rec_Info[0].rec_id);

    pSrc2 = ImageApp_Movie_Get_Pip_ImgSrc(gMovie_Rec_Info2.rec_id);

    if (pSrc1 && pSrc2)
    {
    	if (ImageUnit_IsAllowPull(pSrc1) && ImageUnit_IsAllowPull(pSrc2)) {
    		if (ImageUnit_PullData(pSrc1, (ISF_DATA *)&isf_data1, 0) != ISF_OK) {
    			return;
    		}
    		if (ImageUnit_PullData(pSrc2, (ISF_DATA *)&isf_data2, 0) != ISF_OK) {
    			ImageUnit_ReleaseData(&isf_data1);
    			return;
    		}
    		pdraw->p_src_img[0] = (IMG_BUF *)&isf_data1.Desc[0];
    		pdraw->p_src_img[1] = (IMG_BUF *)&isf_data2.Desc[0];
            pdraw->p_dst_img = pdraw->p_src_img[0];
            pdraw->viewcnt   = 2;
    		AppDispView_OnDraw(pdraw);
            ImageUnit_PushData(pSrc1, (ISF_DATA *)&isf_data1, 0);
    		ImageUnit_ReleaseData(&isf_data1);
    		ImageUnit_ReleaseData(&isf_data2);
    	}
    }

}
#endif


#if SENSOR_CAPS_COUNT == 2
static void MovieExe_2sensor_PIP_DispCB(void)
{
    // PIP view
	ISF_DATA         isf_data1, isf_data2;
	ISF_PORT         *pSrc1, *pSrc2;
	ISF_PORT         *pDist;

	APPDISP_VIEW_DRAW         pip_draw;
	APPDISP_VIEW_DRAW         *pdraw = &pip_draw;

    pSrc1 = ImageUnit_Out(&ISF_UserProc, ImageApp_MovieMulti_GetDispDataPort(_CFG_REC_ID_1));
    pSrc2 = ImageUnit_Out(&ISF_UserProc, ImageApp_MovieMulti_GetDispDataPort(_CFG_REC_ID_2));
	pDist = ImageUnit_In(&ISF_ImgTrans,  ImageApp_MovieMulti_GetDispImgTransPort(_CFG_DISP_ID_1));
    if (pSrc1 && pSrc2) {
    	if (ImageUnit_IsAllowPull(pSrc1) && ImageUnit_IsAllowPull(pSrc2)) {
    		if (ImageUnit_PullData(pSrc1, (ISF_DATA *)&isf_data1, 0) != ISF_OK) {
    			return;
    		}
    		if (ImageUnit_PullData(pSrc2, (ISF_DATA *)&isf_data2, 0) != ISF_OK) {
    			ImageUnit_ReleaseData(&isf_data1);
    			return;
    		}
    		pdraw->p_src_img[0] = (IMG_BUF *)&isf_data1.Desc[0];
    		pdraw->p_src_img[1] = (IMG_BUF *)&isf_data2.Desc[0];
            pdraw->p_dst_img = pdraw->p_src_img[0];
			DBG_IND("p_src_img 0 w =%d, h=%d \r\n",pdraw->p_src_img[0]->Width,pdraw->p_src_img[0]->Height);
			DBG_IND("p_src_img 1 w =%d, h=%d \r\n",pdraw->p_src_img[1]->Width,pdraw->p_src_img[1]->Height);
			DBG_IND("p_dst_img  w =%d, h=%d \r\n",pdraw->p_dst_img->Width,pdraw->p_dst_img->Height);
            pdraw->viewcnt   = 2;
    		AppDispView_OnDraw(pdraw);
            ImageUnit_PushData(pDist, (ISF_DATA *)&isf_data1, 0);
    		ImageUnit_ReleaseData(&isf_data1);
    		ImageUnit_ReleaseData(&isf_data2);
    	}
    }
}
#elif SENSOR_CAPS_COUNT == 4
static void MovieExe_4sensor_PIP_DispCB(void)
{
    // PIP view
	ISF_DATA         isf_data1, isf_data2, isf_data3, isf_data4, isf_new_data;
	ISF_PORT         *pSrc1, *pSrc2, *pSrc3, *pSrc4;
	ISF_PORT         *pDist;
	IMG_BUF          dst_img;
	UINT32           img_buf_size , buff_addr, dst_w, dst_h, dst_pxlfmt;


	APPDISP_VIEW_DRAW         pip_draw= {0};
	APPDISP_VIEW_DRAW         *pdraw = &pip_draw;

    pSrc1 = ImageUnit_Out(&ISF_UserProc, ImageApp_MovieMulti_GetDispDataPort(_CFG_REC_ID_1));
    pSrc2 = ImageUnit_Out(&ISF_UserProc, ImageApp_MovieMulti_GetDispDataPort(_CFG_REC_ID_2));
	pSrc3 = ImageUnit_Out(&ISF_UserProc, ImageApp_MovieMulti_GetDispDataPort(_CFG_REC_ID_3));
	pSrc4 = ImageUnit_Out(&ISF_UserProc, ImageApp_MovieMulti_GetDispDataPort(_CFG_REC_ID_4));
	pDist = ImageUnit_In(&ISF_ImgTrans,  ImageApp_MovieMulti_GetDispImgTransPort(_CFG_DISP_ID_1));
    if (pSrc1 && pSrc2 && pSrc3 && pSrc4) {
    	if (ImageUnit_IsAllowPull(pSrc1) && ImageUnit_PullData(pSrc1, (ISF_DATA *)&isf_data1, 0) == ISF_OK) {
			pdraw->p_src_img[0] = (IMG_BUF *)&isf_data1.Desc[0];
		}
		if (ImageUnit_IsAllowPull(pSrc2) && ImageUnit_PullData(pSrc2, (ISF_DATA *)&isf_data2, 0) == ISF_OK) {
			pdraw->p_src_img[1] = (IMG_BUF *)&isf_data2.Desc[0];
		}
		if (ImageUnit_IsAllowPull(pSrc3) && ImageUnit_PullData(pSrc3, (ISF_DATA *)&isf_data3, 0) == ISF_OK) {
			pdraw->p_src_img[2] = (IMG_BUF *)&isf_data3.Desc[0];
		}
		if (ImageUnit_IsAllowPull(pSrc4) && ImageUnit_PullData(pSrc4, (ISF_DATA *)&isf_data4, 0) == ISF_OK) {
			pdraw->p_src_img[3] = (IMG_BUF *)&isf_data4.Desc[0];
		}
    	ImageUnit_GetVdoImgSize(&ISF_VdoOut1, ISF_IN1,&dst_w, &dst_h);
		dst_pxlfmt = GX_IMAGE_PIXEL_FMT_YUV420;
		img_buf_size = GxImg_CalcRequireSize(dst_w, dst_h, dst_pxlfmt, GXIMAGE_LINEOFFSET_ALIGN(16));
		buff_addr = ImageUnit_NewData(img_buf_size, &isf_new_data);
		if (buff_addr == 0) {
			DBG_ERR("Get buffer fail\r\n");
			goto isf_data_rel;
		}
		if (pdraw->p_src_img[0]) {
			isf_new_data.TimeStamp = isf_data1.TimeStamp;
		}
		else if (pdraw->p_src_img[1]) {
			isf_new_data.TimeStamp = isf_data2.TimeStamp;
		}
		else if (pdraw->p_src_img[2]) {
			isf_new_data.TimeStamp = isf_data3.TimeStamp;
		}
		else if (pdraw->p_src_img[3]) {
			isf_new_data.TimeStamp = isf_data4.TimeStamp;
		}
		GxImg_InitBuf(&dst_img, dst_w, dst_h, dst_pxlfmt, GXIMAGE_LINEOFFSET_ALIGN(16), buff_addr, img_buf_size);
		memcpy(&isf_new_data.Desc[0], &dst_img, sizeof(IMG_BUF));
		pdraw->p_dst_img = &dst_img;
		pdraw->viewcnt   = 4;
		DBG_IND("p_src_img 0 w =%d, h=%d \r\n",pdraw->p_src_img[0]->Width,pdraw->p_src_img[0]->Height);
		DBG_IND("p_src_img 1 w =%d, h=%d \r\n",pdraw->p_src_img[1]->Width,pdraw->p_src_img[1]->Height);
		DBG_IND("p_src_img 2 w =%d, h=%d \r\n",pdraw->p_src_img[2]->Width,pdraw->p_src_img[2]->Height);
		DBG_IND("p_src_img 3 w =%d, h=%d \r\n",pdraw->p_src_img[3]->Width,pdraw->p_src_img[3]->Height);
		DBG_IND("p_dst_img  w =%d, h=%d \r\n",pdraw->p_dst_img->Width,pdraw->p_dst_img->Height);
		AppDispView_OnDraw(pdraw);
		if (ImageUnit_IsAllowPush(pDist)) {
			ImageUnit_PushData(pDist, (ISF_DATA *)&isf_new_data, 0);
		}
		ImageUnit_ReleaseData(&isf_new_data);
isf_data_rel:
		if (pdraw->p_src_img[0]) {
			ImageUnit_ReleaseData(&isf_data1);
		}
		if (pdraw->p_src_img[1]) {
			ImageUnit_ReleaseData(&isf_data2);
		}
		if (pdraw->p_src_img[2]) {
			ImageUnit_ReleaseData(&isf_data3);
		}
		if (pdraw->p_src_img[3]) {
			ImageUnit_ReleaseData(&isf_data4);
		}
    }
}

static void MovieExe_4sensor_PIP_WifiCB(void)
{
    // PIP view
	ISF_DATA         isf_data1, isf_data2, isf_data3, isf_data4, isf_new_data;
	ISF_PORT         *pSrc1, *pSrc2, *pSrc3, *pSrc4;
	ISF_PORT         *pDist;
	IMG_BUF          dst_img;
	UINT32           img_buf_size , buff_addr, dst_w, dst_h, dst_pxlfmt;


	APPDISP_VIEW_DRAW         pip_draw= {0};
	APPDISP_VIEW_DRAW         *pdraw = &pip_draw;

	pSrc1 = ImageUnit_Out(&ISF_UserProc, ImageApp_MovieMulti_GetStreamingDataPort(_CFG_REC_ID_1));
    pSrc2 = ImageUnit_Out(&ISF_UserProc, ImageApp_MovieMulti_GetStreamingDataPort(_CFG_REC_ID_2));
	pSrc3 = ImageUnit_Out(&ISF_UserProc, ImageApp_MovieMulti_GetStreamingDataPort(_CFG_REC_ID_3));
	pSrc4 = ImageUnit_Out(&ISF_UserProc, ImageApp_MovieMulti_GetStreamingDataPort(_CFG_REC_ID_4));
	pDist = ImageUnit_In(&ISF_VdoEnc, (ImageApp_MovieMulti_GetVdoEncPort(_CFG_STRM_ID_1) - ISF_OUT_BASE + ISF_IN_BASE));
    if (pSrc1 && pSrc2 && pSrc3 && pSrc4) {
    	if (ImageUnit_IsAllowPull(pSrc1) && ImageUnit_PullData(pSrc1, (ISF_DATA *)&isf_data1, 0) == ISF_OK) {
			pdraw->p_src_img[0] = (IMG_BUF *)&isf_data1.Desc[0];
		}
		if (ImageUnit_IsAllowPull(pSrc2) && ImageUnit_PullData(pSrc2, (ISF_DATA *)&isf_data2, 0) == ISF_OK) {
			pdraw->p_src_img[1] = (IMG_BUF *)&isf_data2.Desc[0];
		}
		if (ImageUnit_IsAllowPull(pSrc3) && ImageUnit_PullData(pSrc3, (ISF_DATA *)&isf_data3, 0) == ISF_OK) {
			pdraw->p_src_img[2] = (IMG_BUF *)&isf_data3.Desc[0];
		}
		if (ImageUnit_IsAllowPull(pSrc4) && ImageUnit_PullData(pSrc4, (ISF_DATA *)&isf_data4, 0) == ISF_OK) {
			pdraw->p_src_img[3] = (IMG_BUF *)&isf_data4.Desc[0];
		}
		ImageUnit_GetVdoImgSize(&ISF_VdoEnc, (ImageApp_MovieMulti_GetVdoEncPort(_CFG_STRM_ID_1) - ISF_OUT_BASE + ISF_IN_BASE), &dst_w, &dst_h);
		if (dst_w == 0 || dst_h == 0) {
			goto isf_data_rel;
		}
		dst_pxlfmt = GX_IMAGE_PIXEL_FMT_YUV420;
		img_buf_size = GxImg_CalcRequireSize(dst_w, dst_h, dst_pxlfmt, GXIMAGE_LINEOFFSET_ALIGN(16));
		buff_addr = ImageUnit_NewData(img_buf_size, &isf_new_data);
		if (buff_addr == 0) {
			DBG_ERR("Get buffer fail\r\n");
			goto isf_data_rel;
		}
		if (pdraw->p_src_img[0]) {
			isf_new_data.TimeStamp = isf_data1.TimeStamp;
		}
		else if (pdraw->p_src_img[1]) {
			isf_new_data.TimeStamp = isf_data2.TimeStamp;
		}
		else if (pdraw->p_src_img[2]) {
			isf_new_data.TimeStamp = isf_data3.TimeStamp;
		}
		else if (pdraw->p_src_img[3]) {
			isf_new_data.TimeStamp = isf_data4.TimeStamp;
		}
		GxImg_InitBuf(&dst_img, dst_w, dst_h, dst_pxlfmt, GXIMAGE_LINEOFFSET_ALIGN(16), buff_addr, img_buf_size);
		memcpy(&isf_new_data.Desc[0], &dst_img, sizeof(IMG_BUF));
		pdraw->p_dst_img = &dst_img;
		pdraw->viewcnt   = 4;
		DBG_IND("p_src_img 0 w =%d, h=%d \r\n",pdraw->p_src_img[0]->Width,pdraw->p_src_img[0]->Height);
		DBG_IND("p_src_img 1 w =%d, h=%d \r\n",pdraw->p_src_img[1]->Width,pdraw->p_src_img[1]->Height);
		DBG_IND("p_src_img 2 w =%d, h=%d \r\n",pdraw->p_src_img[2]->Width,pdraw->p_src_img[2]->Height);
		DBG_IND("p_src_img 3 w =%d, h=%d \r\n",pdraw->p_src_img[3]->Width,pdraw->p_src_img[3]->Height);
		DBG_IND("p_dst_img w =%d, h=%d \r\n",pdraw->p_dst_img->Width,pdraw->p_dst_img->Height);
		#if 0
		{
			static UINT32 testCount = 0;
			char path[40];

			if (testCount % 100 ==0) {
				snprintf(path,sizeof(path),"A:\\src%d_848x480.yuv",testCount);
				GxImg_DumpBuf(path, pdraw->p_src_img[0], GX_IMAGE_DUMP_ALL);
				DBG_DUMP("dump %s\r\n",path);
			}
			testCount++;
		}
		#endif
		AppDispView_OnDraw(pdraw);
		if (ImageUnit_IsAllowPush(pDist)) {
			ImageUnit_PushData(pDist, (ISF_DATA *)&isf_new_data, 0);
		}
		ImageUnit_ReleaseData(&isf_new_data);
isf_data_rel:
		if (pdraw->p_src_img[0]) {
			ImageUnit_ReleaseData(&isf_data1);
		}
		if (pdraw->p_src_img[1]) {
			ImageUnit_ReleaseData(&isf_data2);
		}
		if (pdraw->p_src_img[2]) {
			ImageUnit_ReleaseData(&isf_data3);
		}
		if (pdraw->p_src_img[3]) {
			ImageUnit_ReleaseData(&isf_data4);
		}
    }
}
#endif


#if (MOVIE_STRM_CODEC_3DNR_ENABLE == ENABLE)
static void MovieExe_UIDefine_3DNR_STRM(UINT32 pathID, UINT32 p3DNRConfig){
	UINT32 sensor_id = 0;
	sensor_id = ImageApp_Movie_GetVidIn_ByPath_Strm(pathID);

    if (sensor_id <= _CFG_VID_IN_4) {
        iq_get_tnr_config(IPL_PATH(sensor_id), p3DNRConfig);
    } else if (sensor_id != 0xFFFF) {   //not movie stream
        DBG_ERR("Sensor ID(%d) out of range!!\r\n",sensor_id);
    }

}
#endif
#if (MOVIE_MAPPING_MULTIREC == DISABLE)
static void MovieExe_UIDefine_3DNR(UINT32 pathID, UINT32 p3DNRConfig)
{
    UINT32 sensor_id = 0;
    sensor_id = ImageApp_Movie_GetVidIn_ByPath(pathID);

    if (sensor_id <= _CFG_VID_IN_4) {
        iq_get_tnr_config(IPL_PATH(sensor_id), p3DNRConfig);
    } else if (sensor_id != 0xFFFF) {   //not movie stream
        DBG_ERR("Sensor ID(%d) out of range!!\r\n",sensor_id);
    }
}
#endif
static void MovieExe_ImageStreamOpenMulti(void)
{
	#if (SENSOR_CAPS_COUNT == 2)
	ImageApp_MovieMulti_SetParam(0, MOVIEMULTI_PARAM_DISP_REG_CB, (UINT32)MovieExe_2sensor_PIP_DispCB);
	#elif (SENSOR_CAPS_COUNT == 4)
	ImageApp_MovieMulti_SetParam(0, MOVIEMULTI_PARAM_DISP_REG_CB, (UINT32)MovieExe_4sensor_PIP_DispCB);
	ImageApp_MovieMulti_SetParam(0, MOVIEMULTI_PARAM_WIFI_REG_CB, (UINT32)MovieExe_4sensor_PIP_WifiCB);
	#endif
}

#if MOVIE_MAPPING_MULTIREC
static void MovieExe_SetVdoCodecByRecID(MOVIE_CFG_REC_ID rec_id, MOVIE_CFG_CODEC codec_type)
{
	UINT32 pathID;

	//if (System_GetState(SYS_STATE_CURRMODE) == PRIMARY_MODE_MOVIE) {
		ImageApp_MovieMulti_SetParam(rec_id, MOVIEMULTI_PARAM_CODEC, codec_type);
		ImageApp_MovieMulti_SetParam(rec_id, MOVIEMULTI_PARAM_PROFILE, (codec_type == _CFG_CODEC_H264) ? _CFG_PROFILE_HIGH : _CFG_PROFILE_MAIN);
	//}

	pathID = (UINT32)ImageApp_MovieMulti_GetVdoEncPort(rec_id);

	if (codec_type == _CFG_CODEC_MJPG) {
		#if (VIDEO_FUNC_MJPG == ENABLE)
			MP_VDOENC_ENCODER *pObj = MP_MjpgEnc_getVideoObject((MP_VDOENC_ID)pathID);
			ImageApp_MovieMulti_SetParam(rec_id, MOVIEMULTI_PARAM_VDO_ENCODER_OBJ, (UINT32)pObj);
		#else
			ImageApp_MovieMulti_SetParam(rec_id, MOVIEMULTI_PARAM_VDO_ENCODER_OBJ, 0);
			DBG_ERR("[MOVIE][%d] codec = %d not support\r\n", pathID, codec_type);
		#endif
	}

	if (codec_type == _CFG_CODEC_H264) {
		#if (VIDEO_FUNC_H264 == ENABLE)
			MP_VDOENC_ENCODER *pObj = MP_H264Enc_getVideoObject((MP_VDOENC_ID)pathID);
			ImageApp_MovieMulti_SetParam(rec_id, MOVIEMULTI_PARAM_VDO_ENCODER_OBJ, (UINT32)pObj);
		#else
			ImageApp_MovieMulti_SetParam(rec_id, MOVIEMULTI_PARAM_VDO_ENCODER_OBJ, 0);
			DBG_ERR("[MOVIE][%d] codec = %d not support\r\n", pathID, codec_type);
		#endif
	}

	if (codec_type == _CFG_CODEC_H265) {
		#if (VIDEO_FUNC_H265 == ENABLE)
			MP_VDOENC_ENCODER *pObj = MP_H265Enc_getVideoObject((MP_VDOENC_ID)pathID);
			ImageApp_MovieMulti_SetParam(rec_id, MOVIEMULTI_PARAM_VDO_ENCODER_OBJ, (UINT32)pObj);
		#else
			ImageApp_MovieMulti_SetParam(rec_id, MOVIEMULTI_PARAM_VDO_ENCODER_OBJ, 0);
			DBG_ERR("[MOVIE][%d] codec = %d not support\r\n", pathID, codec_type);
		#endif
	}
}

static void MovieExe_ImageStreamOpen_MOVIE(void)
{
	MOVIE_CFG_FDB_INFO fdb_info = {0};

	UINT32 movie_size_idx, size_idx = 0;
	UINT32 movie_aspect_ratio_idx = 0;
	UINT32 i, rec_type;
	UINT32 mask, ipl_id = 0, setting_count = 0;
	UINT32 movie_rec_mask, clone_rec_mask;

	MovieExe_ImageStreamOpenMulti();

	movie_size_idx = UI_GetData(FL_MOVIE_SIZE);
	rec_type = MovieMapping_GetRecType(movie_size_idx);
	movie_rec_mask = Movie_GetMovieRecMask();
	clone_rec_mask = Movie_GetCloneRecMask();

	mask = 1;
	for (i = 0; i < SENSOR_CAPS_COUNT; i++)	{

		if (rec_type == MOVIE_REC_TYPE_FRONT || rec_type == MOVIE_REC_TYPE_CLONE) { // single recording
			if (movie_rec_mask & mask) { // active sensor, for movie recording
				ipl_id = 0; // get data from the mapping table of single recording, so IPL ID = 0
				size_idx = movie_size_idx;
			} else { // inactive sensor, just for display
				ipl_id = 0; // get data from the mapping table of single recording, so IPL ID = 0
				if (rec_type == MOVIE_REC_TYPE_CLONE) {
					size_idx = MOVIE_SIZE_CLONE_1920x1080P30_848x480P30; // default, use the mapping table of FHD + clone
				} else {
					size_idx = MOVIE_SIZE_FRONT_1920x1080P30; // default, use the mapping table of single FHD
				}
			}
		} else if (rec_type == MOVIE_REC_TYPE_DUAL) { // dual recording
			if ((movie_rec_mask & mask) && (setting_count < 2)) { // active sensor, for movie recording
				if (setting_count == 0) { // 1st active sensor
					ipl_id = 0; // get data from the mapping table of dual recording, 1st IPL ID = 0
				} else { // 2nd active sensor
					ipl_id = 1; // get data from the mapping table of dual recording, 2nd IPL ID = 1
				}
				size_idx = movie_size_idx;
				setting_count++;
			} else { // inactive sensor, just for display
				ipl_id = 0; // get data from the mapping table of single recording, so IPL ID = 0
				size_idx = MOVIE_SIZE_FRONT_1920x1080P30; // default, use the mapping table of single FHD
			}
		} else if (rec_type == MOVIE_REC_TYPE_TRI) { // tri recording
			if ((movie_rec_mask & mask) && (setting_count < 3)) { // active sensor, for movie recording
				if (setting_count == 0) { // 1st active sensor
					ipl_id = 0; // get data from the mapping table of tri recording, 1st IPL ID = 0
				} else if (setting_count == 1) { // 2nd active sensor
					ipl_id = 1; // get data from the mapping table of tri recording, 2nd IPL ID = 1
				} else { // 3rd active sensor
					ipl_id = 2; // get data from the mapping table of tri recording, 3rd IPL ID = 2
				}
				size_idx = movie_size_idx;
				setting_count++;
			} else { // inactive sensor, just for display
				ipl_id = 0; // get data from the mapping table of single recording, so IPL ID = 0
				size_idx = MOVIE_SIZE_FRONT_1920x1080P30; // default, use the mapping table of single FHD
			}
		} else { // quad recording
			ipl_id = i;
			size_idx = movie_size_idx;
		}

		// set main movie
		gMovie_Rec_Info[i].size.w = MovieMapping_GetWidth(size_idx, ipl_id);
		gMovie_Rec_Info[i].size.h = MovieMapping_GetHeight(size_idx, ipl_id);
		gMovie_Rec_Info[i].raw_enc_size.w = MovieMapping_GetWidth(size_idx, ipl_id);
		gMovie_Rec_Info[i].raw_enc_size.h = MovieMapping_GetHeight(size_idx, ipl_id);
		gMovie_Rec_Info[i].frame_rate = MovieMapping_GetFrameRate(size_idx, ipl_id);
		gMovie_Rec_Info[i].target_bitrate = MovieMapping_GetTargetBitrate(size_idx, ipl_id);
		gMovie_Rec_Info[i].dar = MovieMapping_GetDispAspectRatio(size_idx, ipl_id);
		gMovie_Rec_Info[i].codec = (UI_GetData(FL_MOVIE_CODEC) == MOVIE_CODEC_H265)?_CFG_CODEC_H265:_CFG_CODEC_H264;
		//#NT#2018/02/14#KCHong -begin
	    //#NT#support sensor rotate setting
		gMovie_Rec_Info[i].sensor_rotate = (UI_GetData(FL_MOVIE_SENSOR_ROTATE) == SEN_ROTATE_ON) ? TRUE : FALSE;
		//#NT#2018/02/14#KCHong -end
		MovieMapping_GetAqInfo(size_idx, ipl_id, (UINT32)&gMovie_Rec_Info[i].aq_info);
		MovieMapping_GetCbrInfo(size_idx, ipl_id, (UINT32)&gMovie_Rec_Info[i].cbr_info);
		movie_aspect_ratio_idx = MovieMapping_GetImageRatio(size_idx, ipl_id);
		switch (movie_aspect_ratio_idx) {
		case IMAGERATIO_16_9:
			gMovie_Rec_Info[i].ratio.w = 16;
			gMovie_Rec_Info[i].ratio.h = 9;
			break;
		default:
			gMovie_Rec_Info[i].ratio.w = 4;
			gMovie_Rec_Info[i].ratio.h = 3;
		}
#if MOVIE_DIS_MV_FUNC
    gMovie_Rec_Info[i].imagepipe_func |= IPL_FUNC_SIE_CH4_ETH;
#endif

#if SHDR_FUNC
    gMovie_Rec_Info[i].imagepipe_func |= IPL_FUNC_SHDR;
#endif

#if WDR_FUNC
    gMovie_Rec_Info[i].imagepipe_func |= IPL_FUNC_WDR;
#endif

#if (_ADAS_FUNC_ == ENABLE)
    gMovie_Rec_Info[i].imagepipe_func |= IPL_FUNC_ADAS;
#endif  // #if (_ADAS_FUNC_ == ENABLE)

#if (VIDEOSPRITE_MOSAIC == ENABLE)
    gMovie_Rec_Info[i].imagepipe_func |= IPL_FUNC_PRIMACY_MASK;
#endif

#if (MOVIE_RSC == ENABLE)
#if !defined(_Gyro_None_)
	if ((UI_GetData(FL_MovieRSCIndex) == MOVIE_RSC_ON) &&
		(gMovie_Rec_Info[i].frame_rate <= 120)) {
		MovieExe_RSC_SetSwitch(SEL_RSC_CHANGEMODE, SEL_RSC_ON);
		gMovie_Rec_Info[i].imagepipe_func2 |= IMAGEPIPE_FUNC2_RSCBUF;
	} else {
		MovieExe_RSC_SetSwitch(SEL_RSC_CHANGEMODE, SEL_RSC_OFF);
		gMovie_Rec_Info[i].imagepipe_func2 &= ~IMAGEPIPE_FUNC2_RSCBUF;
	}
#endif
#endif
		// set clone movie
		if (rec_type == MOVIE_REC_TYPE_CLONE) {
			gMovie_Clone_Info[i].size.w = MovieMapping_GetCloneWidth(size_idx, ipl_id);
			gMovie_Clone_Info[i].size.h = MovieMapping_GetCloneHeight(size_idx, ipl_id);
			gMovie_Clone_Info[i].raw_enc_size.w = MovieMapping_GetCloneWidth(size_idx, ipl_id);
			gMovie_Clone_Info[i].raw_enc_size.h = MovieMapping_GetCloneHeight(size_idx, ipl_id);
			gMovie_Clone_Info[i].frame_rate = MovieMapping_GetCloneFrameRate(size_idx, ipl_id);
			gMovie_Clone_Info[i].target_bitrate = MovieMapping_GetCloneTargetBitrate(size_idx, ipl_id);
			gMovie_Clone_Info[i].dar = MovieMapping_GetCloneDispAspectRatio(size_idx, ipl_id);
			gMovie_Clone_Info[i].codec = (UI_GetData(FL_MOVIE_CODEC) == MOVIE_CODEC_H265)?_CFG_CODEC_H265:_CFG_CODEC_H264;
			MovieMapping_GetCloneAqInfo(size_idx, ipl_id, (UINT32)&gMovie_Clone_Info[i].aq_info);
			MovieMapping_GetCloneCbrInfo(size_idx, ipl_id, (UINT32)&gMovie_Clone_Info[i].cbr_info);
			movie_aspect_ratio_idx = MovieMapping_GetCloneImageRatio(size_idx, ipl_id);
			switch (movie_aspect_ratio_idx) {
			case IMAGERATIO_16_9:
				gMovie_Clone_Info[i].ratio.w = 16;
				gMovie_Clone_Info[i].ratio.h = 9;
				break;
			default:
				gMovie_Clone_Info[i].ratio.w = 4;
				gMovie_Clone_Info[i].ratio.h = 3;
			}
		}

		mask <<= 1;
	}

    // Display
    ImageApp_MovieMulti_SetParam(0,MOVIEMULTI_PARAM_DISP_DIR, SysVideo_GetDirbyID(DOUT1));

#if (DUALDISP_FUNC == ENABLE)
	if (GxVideo_GetDevice(DOUT2)) {
		ISIZE dout1_size = GxVideo_GetDeviceSize(DOUT1);
		ISIZE dout2_size = GxVideo_GetDeviceSize(DOUT2);
		ISIZE dout2_ar = {16, 9};       // disp2 aspect ratio

		ImageApp_Movie_SetParam(_CFG_REC_ID_1, MOVIE_PARAM_DUAL_DISP_EN, TRUE);
		if (dout1_size.w * dout1_size.h > dout2_size.w * dout2_size.h) {
			ImageApp_Movie_SetParam(_CFG_REC_ID_1, MOVIE_PARAM_DUAL_DISP_IPL_SIZE, (UINT32)&dout1_size);
		} else {
			ImageApp_Movie_SetParam(_CFG_REC_ID_1, MOVIE_PARAM_DUAL_DISP_IPL_SIZE, (UINT32)&dout2_size);
		}
		ImageApp_Movie_SetParam(_CFG_REC_ID_1, MOVIE_PARAM_DUAL_DISP_AR, (UINT32)&dout2_ar);
	} else {
		ImageApp_Movie_SetParam(_CFG_REC_ID_1, MOVIE_PARAM_DUAL_DISP_EN, FALSE);
	}
#endif

    // Audio
    ImageApp_MovieMulti_Config(MOVIE_CONFIG_AUDIO_INFO, (UINT32) &gMovie_Audio_Info);

	// Recording file
	USIZE sz = {1920, 1080};
	ImageApp_MovieMulti_SetParam(0, MOVIEMULTI_PARAM_IMGCAP_MAXSIZE, (UINT32) &sz);
	for (i = 0; i < SENSOR_CAPS_COUNT; i++) {
		ImageApp_MovieMulti_Config(MOVIE_CONFIG_RECORD_INFO, (UINT32) &gMovie_Rec_Info[i]);
		ImageApp_MovieMulti_Config(MOVIE_CONFIG_RECORD_INFO, (UINT32) &gMovie_Clone_Info[i]);
		ImageApp_MovieMulti_Config(MOVIE_CONFIG_ALG_INFO, (UINT32) &gMovie_Alg_Info[i]);
	}

	// Folder Naming

	fdb_info.drive = 'A';
	fdb_info.p_root_path = MOVIE_ROOT_PATH;
	fdb_info.mem_range.Addr = dma_getCacheAddr(OS_GetMempoolAddr(POOL_ID_FILEDB));
	fdb_info.mem_range.Size = POOL_SIZE_FILEDB;
	ImageApp_MovieMulti_FMConfig(&fdb_info);
	#ifdef _NVT_ITRON_DRIVE_SD_3_
	fdb_info.drive = 'B';
	fdb_info.p_root_path = MOVIE_ROOT_PATH2;
	fdb_info.mem_range.Addr = dma_getCacheAddr(OS_GetMempoolAddr(POOL_ID_FILEDB)) + POOL_SIZE_FILEDB;
	fdb_info.mem_range.Size = POOL_SIZE_FILEDB;
	ImageApp_MovieMulti_FMConfig(&fdb_info);
	#endif

	for (i = 0; i < SENSOR_CAPS_COUNT; i++)	{
		ImageApp_MovieMulti_Root_Path(MOVIE_ROOT_PATH, gMovie_Folder_Naming[i].rec_id);
		ImageApp_MovieMulti_Folder_Naming((MOVIEMULTI_RECODE_FOLDER_NAMING*)&gMovie_Folder_Naming[i]);

    		ImageApp_MovieMulti_Root_Path(MOVIE_ROOT_PATH, gMovie_Clone_Folder_Naming[i].rec_id);
		ImageApp_MovieMulti_Folder_Naming((MOVIEMULTI_RECODE_FOLDER_NAMING*)&gMovie_Clone_Folder_Naming[i]);
	}

	// File option
	ImageApp_MovieMulti_FileOption((MOVIE_RECODE_FILE_OPTION *) &gMovie_Rec_Option);


	//#NT#2018/04/02#IsiahChang -begin
	//#NT#To support Wi-Fi command 2025 BRC.
	// Streaming
    MovieMapping_GetStreamInfo(UI_GetData(FL_MOVIE_SIZE), (UINT32) &gMovie_Strm_Info);
    ImageApp_MovieMulti_Config(MOVIE_CONFIG_STREAM_INFO, (UINT32) &gMovie_Strm_Info);
	//#NT#2018/04/02#IsiahChang -end

#if (MOVIE_MAPPING_MULTIREC == DISABLE)
	// Codec 3DNR
    ImageApp_Movie_SetParam(gMovie_Rec_Info[0].rec_id, MOVIE_PARAM_3DNR_CB, (UINT32)MovieExe_UIDefine_3DNR);
#if (MOVIE_STRM_CODEC_3DNR_ENABLE == ENABLE)
    ImageApp_Movie_SetParam(gMovie_Strm_Info.strm_id, MOVIE_PARAM_3DNR_CB, (UINT32)MovieExe_UIDefine_3DNR_STRM);
#endif

#if MOVIE_DUAL_RECORD_TEST
    ImageApp_Movie_SetParam(gMovie_Rec_Info2.rec_id, MOVIE_PARAM_3DNR_CB, (UINT32)MovieExe_UIDefine_3DNR);
#endif
#endif

#if 0
	static MOVIE_ALG_INFO ImgSetting = {
		// ipl 1 path 3
		GX_IMAGE_PIXEL_FMT_YUV420_PACKED,		// format
		{640, 480},								// image size
		{0, 0, 0, 0},							// window size, x/y/w/h
		FALSE,									// preview enable
		// ipl 1 path 4
		0,										// format
		{0, 0},									// image size
		{0, 0, 0, 0},							// window size, x/y/w/h
		FALSE,									// preview enable
	};
	ImageApp_Movie_SetParam(_CFG_REC_ID_1, MOVIE_PARAM_ALG_PARAM_IPL1_PATH3, (UINT32) &ImgSetting);
#endif

#if ((VDOENC_MULTI_TEMPORARY_LAYER == ENABLE) && (MOVIE_MAPPING_MULTIREC == DISABLE))
	ImageApp_Movie_SetParam(gMovie_Rec_Info[0].rec_id, MOVIE_PARAM_MULTI_TEMPORARY_LAYER, 1);
#endif

#if (YUV_TV_RANGE == ENABLE)
	ImageApp_Movie_SetParam(gMovie_Rec_Info[0].rec_id, MOVIE_PARAM_TV_RANGE, 1);
#endif

	if (gMovie_Rec_Info[0].aud_codec == _CFG_AUD_CODEC_AAC){
		#if (AUDIO_FUNC_AAC == ENABLE)
			MP_AUDENC_ENCODER *pObj = MP_AACEnc_getEncodeObject();
			ImageApp_MovieMulti_SetParam(_CFG_REC_ID_1, MOVIEMULTI_PARAM_AUD_ENCODER_OBJ, (UINT32)pObj);
		#else
			ImageApp_MovieMulti_SetParam(_CFG_REC_ID_1, MOVIEMULTI_PARAM_AUD_ENCODER_OBJ, 0);
			DBG_ERR("aud codec = %d not support\r\n", gMovie_Rec_Info.aud_codec);
		#endif
	}

    // Config timelapse time
	for (i = 0; i < SENSOR_CAPS_COUNT; i++) {
		if (gMovie_Rec_Info[i].rec_mode == _CFG_REC_TYPE_TIMELAPSE) {
			ImageApp_MovieMulti_SetParam(gMovie_Rec_Info[i].rec_id, MOVIEMULTI_PARAM_TIMELAPSE_TIME, MOVIE_TIMELAPSE_TIME);
			ImageApp_MovieMulti_SetParam(gMovie_Rec_Info[i].rec_id, MOVIEMULTI_PARAM_FILE_PLAYFRAMERATE, MOVIE_TIMELAPSE_PLAYFRAMERATE);
		}
    }

#if MOVIE_DUAL_RECORD_TEST
    if (gMovie_Rec_Info2.rec_mode == _CFG_REC_TYPE_TIMELAPSE) {
        ImageApp_MovieMulti_SetParam(gMovie_Rec_Info2.rec_id, MOVIEMULTI_PARAM_TIMELAPSE_TIME, MOVIE_TIMELAPSE_TIME);
        ImageApp_MovieMulti_SetParam(gMovie_Rec_Info2.rec_id, MOVIEMULTI_PARAM_FILE_PLAYFRAMERATE, MOVIE_TIMELAPSE_PLAYFRAMERATE);
    }
#endif

#if MOVIE_UVAC_FUNC
    // Uvac
	UVAC_VID_RESO_ARY uvacVidResoAry = {0};
	UVAC_AUD_SAMPLERATE_ARY uvacAudSampleRateAry = {0};
	UVAC_INFO    UvacInfo = {0};


	uvacVidResoAry.aryCnt = 1;//MOVIE_SIZE_ID_MAX;
	uvacVidResoAry.pVidResAry = &gUIUvacVidReso[0];
	uvacAudSampleRateAry.aryCnt = 1;
	uvacAudSampleRateAry.pAudSampleRateAry = &gUIUvacAudSampleRate[0];

	xUSBMakerInit_UVAC(&gUIMovUvacDevDesc);
	#if defined(_USB1_DEVICE_)
	int_setConfig(INT_CONFIG_ID_USB3_GIC_DESTINATION, CC_CORE_CA53_CORE1);
	UVAC_SetObject(UVAC_TYPE_U3);
	#elif defined(_USB2_DEVICE_)
	UVAC_SetObject(UVAC_TYPE_U2);
	#endif

	UvacInfo.channel = UVAC_CHANNEL_1V1A;
	ImageUnit_Begin(&ISF_UsbUVAC, 0);
		ImageUnit_SetParam(ISF_CTRL, UVAC_PARAM_VID_RESO_ARY, (UINT32)&uvacVidResoAry);
		ImageUnit_SetParam(ISF_CTRL, UVAC_PARAM_AUD_SAMPLERATE_ARY, (UINT32)&uvacAudSampleRateAry);
		ImageUnit_SetParam(ISF_CTRL, UVAC_PARAM_UVAC_VEND_DEV_DESC, (UINT32)&gUIMovUvacDevDesc);
		ImageUnit_SetParam(ISF_CTRL, UVAC_PARAM_VID_FMT_TYPE, IMGUNIT_UVAC_VIDEO_FORMAT_H264_ONLY);
		ImageUnit_SetParam(ISF_CTRL, UVAC_PARAM_SUSPEND_CLOSE_IMM, TRUE);
		ImageUnit_SetParam(ISF_CTRL, UVAC_PARAM_UVAC_INFO, (UINT32)&UvacInfo);
	ImageUnit_End();

#endif

	// User Callback
	ImageApp_MovieMulti_RegUserCB(MovieExe_UserEventCb);

#if MOVIE_DUAL_RECORD_TEST
    if (gMovie_Disp_Info.pip_view) {
        // register PIP view CB
        ImageApp_Movie_SetParam(gMovie_Rec_Info2.rec_id, MOVIE_PARAM_PIPVIEW_PUSH_CB, (UINT32)MovieExe_UserProc_PushCB);
        ImageApp_Movie_SetParam(gMovie_Rec_Info2.rec_id, MOVIE_PARAM_PIPVIEW_PROCESS_CB, (UINT32)MovieExe_UserProc_ProcessCB);
    }
#endif


	ImageApp_MovieMulti_Open();

	//#NT#2018/06/11#KCHong -begin
	//#NT#For MovieMulti Test
	#if (SENSOR_CAPS_COUNT == 2)
	ImageApp_MovieMulti_ImgLinkForDisp(_CFG_REC_ID_1, ENABLE, TRUE);
	ImageApp_MovieMulti_ImgLinkForDisp(_CFG_REC_ID_2, ENABLE, TRUE);
	#elif (SENSOR_CAPS_COUNT == 4)
	ImageApp_MovieMulti_ImgLinkForDisp(_CFG_REC_ID_1, ENABLE, TRUE);
	ImageApp_MovieMulti_ImgLinkForDisp(_CFG_REC_ID_2, ENABLE, TRUE);
	ImageApp_MovieMulti_ImgLinkForDisp(_CFG_REC_ID_3, ENABLE, TRUE);
	ImageApp_MovieMulti_ImgLinkForDisp(_CFG_REC_ID_4, ENABLE, TRUE);
	#else
	ImageApp_MovieMulti_ImgLinkForDisp(_CFG_REC_ID_1, ENABLE, FALSE);
	#endif
	ImageApp_MovieMulti_DispStart(_CFG_DISP_ID_1);
	//#NT#2018/06/11#KCHong -end

	// For ALG function
	for (i = 0; i < SENSOR_CAPS_COUNT; i++) {
		if (gMovie_Alg_Info[_CFG_REC_ID_1 + i].path13.ImgSize.w && gMovie_Alg_Info[_CFG_REC_ID_1 + i].path13.ImgSize.h) {
			ImageApp_MovieMulti_ImgLinkForAlg((_CFG_REC_ID_1 + i), _CFG_ALG_PATH3, ENABLE, TRUE);
		}
		if (gMovie_Alg_Info[_CFG_REC_ID_1 + i].path14.ImgSize.w && gMovie_Alg_Info[_CFG_REC_ID_1 + i].path14.ImgSize.h) {
			ImageApp_MovieMulti_ImgLinkForAlg((_CFG_REC_ID_1 + i), _CFG_ALG_PATH4, ENABLE, TRUE);
		}
	}

	// set video codec for main movie and clone movie
	mask = 1;
	for (i = 0; i < SENSOR_CAPS_COUNT; i++) {
		if (movie_rec_mask & mask) {
			MovieExe_SetVdoCodecByRecID(gMovie_Rec_Info[i].rec_id, gMovie_Rec_Info[i].codec);
		}

		if (clone_rec_mask & mask) {
			MovieExe_SetVdoCodecByRecID(gMovie_Clone_Info[i].rec_id, gMovie_Clone_Info[i].codec);
		}

		mask <<= 1;
	}

	// set video codec for streaming
	MovieExe_SetVdoCodecByRecID(gMovie_Strm_Info.strm_id, gMovie_Strm_Info.codec);

//#NT#2018/03/21#Charlie Chang -begin
//#NT# UVAC support encode obj
#if MOVIE_UVAC_FUNC
	{
		UINT32 pathID;
		pathID = (UINT32)ImageApp_Movie_GetVdoEncPort(gMovie_Uvac_Info.uvac_id);
		if (gMovie_Uvac_Info.codec == _CFG_UVC_CODEC_MJPG) {
			#if (VIDEO_FUNC_MJPG == ENABLE)
				MP_VDOENC_ENCODER *pObj = MP_MjpgEnc_getVideoObject((MP_VDOENC_ID)pathID);
				ImageApp_Movie_SetParam(gMovie_Uvac_Info.uvac_id, MOVIE_PARAM_VDO_ENCODER_OBJ, (UINT32)pObj);
			#else
				ImageApp_Movie_SetParam(gMovie_Uvac_Info.uvac_id, MOVIE_PARAM_VDO_ENCODER_OBJ, 0);
				DBG_ERR("[MOVIE_UVAC][%d] codec = %d not support\r\n", pathID, gMovie_Uvac_Info.codec);
			#endif
		}
		if (gMovie_Uvac_Info.codec == _CFG_UVC_CODEC_H264) {
			#if (VIDEO_FUNC_H264 == ENABLE)
				MP_VDOENC_ENCODER *pObj = MP_H264Enc_getVideoObject((MP_VDOENC_ID)pathID);
				ImageApp_Movie_SetParam(gMovie_Uvac_Info.uvac_id, MOVIE_PARAM_VDO_ENCODER_OBJ, (UINT32)pObj);
			#else
				ImageApp_Movie_SetParam(gMovie_Uvac_Info.uvac_id, MOVIE_PARAM_VDO_ENCODER_OBJ, 0);
				DBG_ERR("[MOVIE_UVAC][%d] codec = %d not support\r\n", pathID, gMovie_Uvac_Info.codec);
			#endif
		}
	}
#endif
//#NT#2018/03/21#Charlie Chang -end

    if (UI_GetData(FL_MovieDisIndex) == MOVIE_DIS_ON) {
        // enable DIS
		MovieExe_IPL_SetIInfo(IPL_SEL_DISCOMP_IMM, SEL_DISCOMP_ON);
    } else {
        // disble DIS
		MovieExe_IPL_SetIInfo(IPL_SEL_DISCOMP_IMM, SEL_DISCOMP_OFF);
    }

#if(WIFI_AP_FUNC==ENABLE)
    if (System_GetState(SYS_STATE_CURRSUBMODE) == SYS_SUBMODE_WIFI) {
    	Ux_SendEvent(&CustomMovieObjCtrl, NVTEVT_EXE_MOVIE_STRM_START, 1,gMovie_Strm_Info.strm_id);
        Ux_PostEvent(NVTEVT_WIFI_EXE_MODE_DONE,1,E_OK);
    }
    else{
        #if (defined(_MODEL_ARTOSYN_DVCAM_EVB_))
			////start strm
			//wait linux
			Delay_DelayMs(1000);
	    	Ux_SendEvent(&CustomMovieObjCtrl, NVTEVT_EXE_MOVIE_STRM_START, 1,gMovie_Strm_Info.strm_id);
            Ux_PostEvent(NVTEVT_WIFI_EXE_MODE_DONE,1,E_OK);
			#if MOVIE_UVAC_FUNC
				////start uvac
				ImageApp_Movie_Uvac_Start(gMovie_Uvac_Info.uvac_id);
			#endif
        #endif
		#if MOVIE_UVAC_FUNC
			#if (SENSOR_CAPS_COUNT == 2)
			ImageApp_MovieMulti_ImgLinkForStreaming(_CFG_REC_ID_1, ENABLE, TRUE);
			ImageApp_MovieMulti_ImgLinkForStreaming(_CFG_REC_ID_2, ENABLE, TRUE);
			#elif (SENSOR_CAPS_COUNT == 4)
			ImageApp_MovieMulti_ImgLinkForStreaming(_CFG_REC_ID_1, ENABLE, TRUE);
			ImageApp_MovieMulti_ImgLinkForStreaming(_CFG_REC_ID_2, ENABLE, TRUE);
			ImageApp_MovieMulti_ImgLinkForStreaming(_CFG_REC_ID_3, ENABLE, TRUE);
			ImageApp_MovieMulti_ImgLinkForStreaming(_CFG_REC_ID_4, ENABLE, TRUE);
			#else
			ImageApp_MovieMulti_ImgLinkForStreaming(_CFG_REC_ID_1, ENABLE, FALSE);
			#endif
			ImageApp_MovieMulti_UvacStart(_CFG_UVAC_ID_1);
#endif
    }
#endif
}
#else
static void MovieExe_ImageStreamOpen_MOVIE(void)
{
	MOVIE_CFG_FDB_INFO fdb_info = {0};//MEM_RANGE filedb_Pool = {0};
	UINT32 movie_size_idx = 0;
	UINT32 movie_aspect_ratio_idx = 0;
	UINT32 pathID = 0;

	// Get movie setting from PStore
	movie_size_idx = UI_GetData(FL_MOVIE_SIZE);
	gMovie_Rec_Info[0].size.w = GetMovieSizeWidth(movie_size_idx);
	gMovie_Rec_Info[0].size.h = GetMovieSizeHeight(movie_size_idx);
	gMovie_Rec_Info[0].raw_enc_size.w = GetMovieSizeWidth(movie_size_idx);
	gMovie_Rec_Info[0].raw_enc_size.h = GetMovieSizeHeight(movie_size_idx);
	gMovie_Rec_Info[0].frame_rate = GetMovieFrameRate(movie_size_idx);
	gMovie_Rec_Info[0].target_bitrate = GetMovieTargetBitrate(movie_size_idx, 0);
	gMovie_Rec_Info[0].dar = GetMovieDispAspectRatio(movie_size_idx);
	gMovie_Rec_Info[0].codec = (UI_GetData(FL_MOVIE_CODEC) == MOVIE_CODEC_H265)?_CFG_CODEC_H265:_CFG_CODEC_H264;
	//#NT#2018/02/14#KCHong -begin
    //#NT#support sensor rotate setting
	gMovie_Rec_Info[0].sensor_rotate = (UI_GetData(FL_MOVIE_SENSOR_ROTATE) == SEN_ROTATE_ON) ? TRUE : FALSE;
	//#NT#2018/02/14#KCHong -end
	GetMovieAqInfo(movie_size_idx, (UINT32)&gMovie_Rec_Info[0].aq_info);
	GetMovieCbrInfo(movie_size_idx, (UINT32)&gMovie_Rec_Info[0].cbr_info);
	movie_aspect_ratio_idx = GetMovieSizeRatio(movie_size_idx);
	switch (movie_aspect_ratio_idx) {
	case IMAGERATIO_16_9:
		gMovie_Rec_Info[0].ratio.w = 16;
		gMovie_Rec_Info[0].ratio.h = 9;
		break;
	default:
		gMovie_Rec_Info[0].ratio.w = 4;
		gMovie_Rec_Info[0].ratio.h = 3;
	}

#if MOVIE_DIS_MV_FUNC
    gMovie_Rec_Info[0].imagepipe_func |= IPL_FUNC_SIE_CH4_ETH;
#endif

#if SHDR_FUNC
    gMovie_Rec_Info[0].imagepipe_func |= IPL_FUNC_SHDR;
#endif

#if WDR_FUNC
    gMovie_Rec_Info[0].imagepipe_func |= IPL_FUNC_WDR;
#endif

#if DEFOG_FUNC
    gMovie_Rec_Info[0].imagepipe_func |= IPL_FUNC_DEFOG;
#endif

#if (_ADAS_FUNC_ == ENABLE)
    gMovie_Rec_Info[0].imagepipe_func |= IPL_FUNC_ADAS;
#endif  // #if (_ADAS_FUNC_ == ENABLE)

#if (VIDEOSPRITE_MOSAIC == ENABLE)
    gMovie_Rec_Info[0].imagepipe_func |= IPL_FUNC_PRIMACY_MASK;
#endif

#if (MOVIE_RSC == ENABLE)
#if !defined(_Gyro_None_)
	if ((UI_GetData(FL_MovieRSCIndex) == MOVIE_RSC_ON) &&
		(gMovie_Rec_Info[0].frame_rate <= 120)) {
		MovieExe_RSC_SetSwitch(SEL_RSC_CHANGEMODE, SEL_RSC_ON);
		gMovie_Rec_Info[0].imagepipe_func2 |= IMAGEPIPE_FUNC2_RSCBUF;
	} else {
		MovieExe_RSC_SetSwitch(SEL_RSC_CHANGEMODE, SEL_RSC_OFF);
		gMovie_Rec_Info[0].imagepipe_func2 &= ~IMAGEPIPE_FUNC2_RSCBUF;
	}
#endif
#endif

#if (MOVIE_DIS_COMP_FUNC == ENABLE)
        gMovie_Rec_Info.imagepipe_func2 |= IMAGEPIPE_FUNC2_VIEWTRACKING1;
        gMovie_Rec_Info.imagepipe_func2 |= IMAGEPIPE_FUNC2_VIEWTRACKING2;
        gMovie_Rec_Info.imagepipe_func2 |= IMAGEPIPE_FUNC2_VIEWTRACKING3;
        gMovie_Rec_Info.imagepipe_func2 |= IMAGEPIPE_FUNC2_VIEWTRACKING4;
#endif


    // Display
    gMovie_Disp_Info.rotate_dir = SysVideo_GetDirbyID(DOUT1);
    ImageApp_Movie_Config(MOVIE_CONFIG_DISP_INFO, (UINT32) &gMovie_Disp_Info);

#if (DUALDISP_FUNC == ENABLE)
	if (GxVideo_GetDevice(DOUT2)) {
		ISIZE dout1_size = GxVideo_GetDeviceSize(DOUT1);
		ISIZE dout2_size = GxVideo_GetDeviceSize(DOUT2);
		ISIZE dout2_ar = {16, 9};       // disp2 aspect ratio

		ImageApp_Movie_SetParam(_CFG_REC_ID_1, MOVIE_PARAM_DUAL_DISP_EN, TRUE);
		if (dout1_size.w * dout1_size.h > dout2_size.w * dout2_size.h) {
			ImageApp_Movie_SetParam(_CFG_REC_ID_1, MOVIE_PARAM_DUAL_DISP_IPL_SIZE, (UINT32)&dout1_size);
		} else {
			ImageApp_Movie_SetParam(_CFG_REC_ID_1, MOVIE_PARAM_DUAL_DISP_IPL_SIZE, (UINT32)&dout2_size);
		}
		ImageApp_Movie_SetParam(_CFG_REC_ID_1, MOVIE_PARAM_DUAL_DISP_AR, (UINT32)&dout2_ar);
	} else {
		ImageApp_Movie_SetParam(_CFG_REC_ID_1, MOVIE_PARAM_DUAL_DISP_EN, FALSE);
	}
#endif

    // Audio
    ImageApp_Movie_Config(MOVIE_CONFIG_AUDIO_INFO, (UINT32) &gMovie_Audio_Info);

	// Alg
    ImageApp_Movie_Config(MOVIE_CONFIG_ALG_INFO, (UINT32) &gMovie_Alg_Info);

	// Recording file
	ImageApp_Movie_Config(MOVIE_CONFIG_RECORD_INFO, (UINT32) &gMovie_Rec_Info[0]);

	// Folder Naming
	fdb_info.drive = 'A';
	fdb_info.p_root_path = MOVIE_ROOT_PATH;
	fdb_info.mem_range.Addr = dma_getCacheAddr(OS_GetMempoolAddr(POOL_ID_FILEDB));
	fdb_info.mem_range.Size = POOL_SIZE_FILEDB;
	ImageApp_Movie_FileDB_Config(&fdb_info, 0);

	// File option
	ImageApp_Movie_File_Option((MOVIE_RECODE_FILE_OPTION *) &gMovie_Rec_Option);

#if MOVIE_DUAL_RECORD_TEST
	GetMovieAqInfo_2p(movie_size_idx, (UINT32)&gMovie_Rec_Info2.aq_info);
	GetMovieCbrInfo_2p(movie_size_idx, (UINT32)&gMovie_Rec_Info2.cbr_info);
	ImageApp_Movie_Config(MOVIE_CONFIG_RECORD_INFO, (UINT32) &gMovie_Rec_Info2);
	ImageApp_Movie_File_Option((MOVIE_RECODE_FILE_OPTION *) &gMovie_Rec_Option2);
	ImageApp_Movie_Root_Path(MOVIE_ROOT_PATH, 1);//2017/12/20
	ImageApp_Movie_Folder_Naming(&gMovie_Folder_Naming2);
#endif

//#NT#2018/04/02#IsiahChang -begin
//#NT#To support Wi-Fi command 2025 BRC.
	// Streaming
    GetMovieStreamInfo(UI_GetData(FL_MOVIE_SIZE), (UINT32)&gMovie_Strm_Info);
    ImageApp_Movie_Config(MOVIE_CONFIG_STREAM_INFO, (UINT32) &gMovie_Strm_Info);
//#NT#2018/04/02#IsiahChang -end

	// Codec 3DNR
    ImageApp_Movie_SetParam(gMovie_Rec_Info[0].rec_id, MOVIE_PARAM_3DNR_CB, (UINT32)MovieExe_UIDefine_3DNR);

#if (MOVIE_STRM_CODEC_3DNR_ENABLE == ENABLE)
    ImageApp_Movie_SetParam(gMovie_Strm_Info.strm_id, MOVIE_PARAM_3DNR_CB, (UINT32)MovieExe_UIDefine_3DNR_STRM);
#endif

#if MOVIE_DUAL_RECORD_TEST
    ImageApp_Movie_SetParam(gMovie_Rec_Info2.rec_id, MOVIE_PARAM_3DNR_CB, (UINT32)MovieExe_UIDefine_3DNR);
#endif

#if 0
	static MOVIE_ALG_INFO ImgSetting = {
		// ipl 1 path 3
		GX_IMAGE_PIXEL_FMT_YUV420_PACKED,       // format
		{640, 480},                             // image size
		{0, 0, 0, 0},                           // window size, x/y/w/h
		FALSE,                                  // preview enable
		// ipl 1 path 4
		0,              			// format
		{0, 0},                           	// image size
		{0, 0, 0, 0},                   	// window size, x/y/w/h
		FALSE,                                  // preview enable
	};
	ImageApp_Movie_SetParam(_CFG_REC_ID_1, MOVIE_PARAM_ALG_PARAM_IPL1_PATH3, (UINT32) &ImgSetting);
#endif

#if (VDOENC_MULTI_TEMPORARY_LAYER == ENABLE)
	ImageApp_Movie_SetParam(gMovie_Rec_Info[0].rec_id, MOVIE_PARAM_MULTI_TEMPORARY_LAYER, 1);
#endif

#if (YUV_TV_RANGE == ENABLE)
	ImageApp_Movie_SetParam(gMovie_Rec_Info[0].rec_id, MOVIE_PARAM_TV_RANGE, 1);
#endif

	if (gMovie_Rec_Info[0].aud_codec == _CFG_AUD_CODEC_AAC){
#if (AUDIO_FUNC_AAC == ENABLE)
		MP_AUDENC_ENCODER *pObj = MP_AACEnc_getEncodeObject();
		ImageApp_Movie_SetParam(_CFG_REC_ID_1, MOVIE_PARAM_AUD_ENCODER_OBJ, (UINT32)pObj);
#else
		ImageApp_Movie_SetParam(_CFG_REC_ID_1, MOVIE_PARAM_AUD_ENCODER_OBJ, 0);
		DBG_ERR("aud codec = %d not support\r\n", gMovie_Rec_Info.aud_codec);
#endif
	}

    // Config timelapse time
    if (gMovie_Rec_Info[0].rec_mode == _CFG_REC_TYPE_TIMELAPSE) {
        ImageApp_Movie_SetParam(gMovie_Rec_Info[0].rec_id, MOVIE_PARAM_TIMELAPSE_TIME, MOVIE_TIMELAPSE_TIME);
        ImageApp_Movie_SetParam(gMovie_Rec_Info[0].rec_id, MOVIE_PARAM_FILE_PLAYFRAMERATE, MOVIE_TIMELAPSE_PLAYFRAMERATE);
    }

#if MOVIE_DUAL_RECORD_TEST
    if (gMovie_Rec_Info2.rec_mode == _CFG_REC_TYPE_TIMELAPSE) {
        ImageApp_Movie_SetParam(gMovie_Rec_Info2.rec_id, MOVIE_PARAM_TIMELAPSE_TIME, MOVIE_TIMELAPSE_TIME);
        ImageApp_Movie_SetParam(gMovie_Rec_Info2.rec_id, MOVIE_PARAM_FILE_PLAYFRAMERATE, MOVIE_TIMELAPSE_PLAYFRAMERATE);
    }
#endif

#if MOVIE_UVAC_FUNC
    // Uvac
	UVAC_VID_RESO_ARY uvacVidResoAry = {0};
	UVAC_AUD_SAMPLERATE_ARY uvacAudSampleRateAry = {0};

	uvacVidResoAry.aryCnt = 1;//MOVIE_SIZE_ID_MAX;
	uvacVidResoAry.pVidResAry = &gUIUvacVidReso[0];

	uvacAudSampleRateAry.aryCnt = 1;
	uvacAudSampleRateAry.pAudSampleRateAry = &gUIUvacAudSampleRate[0];

	xUSBMakerInit_UVAC(&gUIMovUvacDevDesc);




    ImageApp_Movie_Config(MOVIE_CONFIG_UVAC_INFO, (UINT32) &gMovie_Uvac_Info);

	ImageUnit_Begin(&ISF_UsbUVAC, 0);
		ImageUnit_SetParam(ISF_CTRL, UVAC_PARAM_VID_RESO_ARY, (UINT32)&uvacVidResoAry);
		ImageUnit_SetParam(ISF_CTRL, UVAC_PARAM_AUD_SAMPLERATE_ARY, (UINT32)&uvacAudSampleRateAry);
		ImageUnit_SetParam(ISF_CTRL, UVAC_PARAM_UVAC_VEND_DEV_DESC, (UINT32)&gUIMovUvacDevDesc);
	ImageUnit_End();

#endif

	// User Callback
	ImageApp_Movie_RegUserCB(MovieExe_UserEventCb);

#if MOVIE_DUAL_RECORD_TEST
    if (gMovie_Disp_Info.pip_view) {
        // register PIP view CB
        ImageApp_Movie_SetParam(gMovie_Rec_Info2.rec_id, MOVIE_PARAM_PIPVIEW_PUSH_CB, (UINT32)MovieExe_UserProc_PushCB);
        ImageApp_Movie_SetParam(gMovie_Rec_Info2.rec_id, MOVIE_PARAM_PIPVIEW_PROCESS_CB, (UINT32)MovieExe_UserProc_ProcessCB);
    }
#endif

#if SHDR_FUNC
	if (UI_GetData(FL_MOVIE_HDR) == MOVIE_HDR_ON) {
		MovieExe_IPL_SetIInfo(IPL_SEL_SHDR, SEL_SHDR_ON);
		MovieExe_IPL_SetIInfo(IPL_SEL_SHDR_FRAME_NUM, DVCAM_SHDR_MAX_FRAME);
	}
#endif
	ImageApp_Movie_Open();

 	pathID = (UINT32)ImageApp_Movie_GetVdoEncPort(gMovie_Rec_Info[0].rec_id);
	if (gMovie_Rec_Info[0].codec == _CFG_CODEC_MJPG) {
#if (VIDEO_FUNC_MJPG == ENABLE)
		MP_VDOENC_ENCODER *pObj = MP_MjpgEnc_getVideoObject((MP_VDOENC_ID)pathID);
		ImageApp_Movie_SetParam(gMovie_Rec_Info[0].rec_id, MOVIE_PARAM_VDO_ENCODER_OBJ, (UINT32)pObj);
#else
		ImageApp_Movie_SetParam(gMovie_Rec_Info[0].rec_id, MOVIE_PARAM_VDO_ENCODER_OBJ, 0);
		DBG_ERR("[MOVIE][%d] codec = %d not support\r\n", pathID, gMovie_Rec_Info[0].codec);
#endif
	}
	if (gMovie_Rec_Info[0].codec == _CFG_CODEC_H264) {
#if (VIDEO_FUNC_H264 == ENABLE)
		MP_VDOENC_ENCODER *pObj = MP_H264Enc_getVideoObject((MP_VDOENC_ID)pathID);
		ImageApp_Movie_SetParam(gMovie_Rec_Info[0].rec_id, MOVIE_PARAM_VDO_ENCODER_OBJ, (UINT32)pObj);
#else
		ImageApp_Movie_SetParam(gMovie_Rec_Info[0].rec_id, MOVIE_PARAM_VDO_ENCODER_OBJ, 0);
		DBG_ERR("[MOVIE][%d] codec = %d not support\r\n", pathID, gMovie_Rec_Info[0].codec);
#endif
	}
	if (gMovie_Rec_Info[0].codec == _CFG_CODEC_H265) {
#if (VIDEO_FUNC_H265 == ENABLE)
		MP_VDOENC_ENCODER *pObj = MP_H265Enc_getVideoObject((MP_VDOENC_ID)pathID);
		ImageApp_Movie_SetParam(gMovie_Rec_Info[0].rec_id, MOVIE_PARAM_VDO_ENCODER_OBJ, (UINT32)pObj);
#else
		ImageApp_Movie_SetParam(gMovie_Rec_Info[0].rec_id, MOVIE_PARAM_VDO_ENCODER_OBJ, 0);
		DBG_ERR("[MOVIE][%d] codec = %d not support\r\n", pathID, gMovie_Rec_Info[0].codec);
#endif
	}

 	pathID = (UINT32)ImageApp_Movie_GetVdoEncPort(gMovie_Strm_Info.strm_id);
	if (gMovie_Strm_Info.codec == _CFG_CODEC_MJPG) {
#if (VIDEO_FUNC_MJPG == ENABLE)
		MP_VDOENC_ENCODER *pObj = MP_MjpgEnc_getVideoObject((MP_VDOENC_ID)pathID);
		ImageApp_Movie_SetParam(gMovie_Strm_Info.strm_id, MOVIE_PARAM_VDO_ENCODER_OBJ, (UINT32)pObj);
#else
		ImageApp_Movie_SetParam(gMovie_Strm_Info.strm_id, MOVIE_PARAM_VDO_ENCODER_OBJ, 0);
		DBG_ERR("[MOVIE_STRM][%d] codec = %d not support\r\n", pathID, gMovie_Strm_Info.codec);
#endif
	}
	if (gMovie_Strm_Info.codec == _CFG_CODEC_H264) {
#if (VIDEO_FUNC_H264 == ENABLE)
		MP_VDOENC_ENCODER *pObj = MP_H264Enc_getVideoObject((MP_VDOENC_ID)pathID);
		ImageApp_Movie_SetParam(gMovie_Strm_Info.strm_id, MOVIE_PARAM_VDO_ENCODER_OBJ, (UINT32)pObj);
#else
		ImageApp_Movie_SetParam(gMovie_Strm_Info.strm_id, MOVIE_PARAM_VDO_ENCODER_OBJ, 0);
		DBG_ERR("[MOVIE_STRM][%d] codec = %d not support\r\n", pathID, gMovie_Strm_Info.codec);
#endif
	}
	if (gMovie_Strm_Info.codec == _CFG_CODEC_H265) {
#if (VIDEO_FUNC_H265 == ENABLE)
		MP_VDOENC_ENCODER *pObj = MP_H265Enc_getVideoObject((MP_VDOENC_ID)pathID);
		ImageApp_Movie_SetParam(gMovie_Strm_Info.strm_id, MOVIE_PARAM_VDO_ENCODER_OBJ, (UINT32)pObj);
#else
		ImageApp_Movie_SetParam(gMovie_Strm_Info.strm_id, MOVIE_PARAM_VDO_ENCODER_OBJ, 0);
		DBG_ERR("[MOVIE_STRM][%d] codec = %d not support\r\n", pathID, gMovie_Strm_Info.codec);
#endif
	}
//#NT#2018/03/21#Charlie Chang -begin
//#NT# UVAC support encode obj
#if MOVIE_UVAC_FUNC
 	pathID = (UINT32)ImageApp_Movie_GetVdoEncPort(gMovie_Uvac_Info.uvac_id);
	if (gMovie_Uvac_Info.codec == _CFG_UVC_CODEC_MJPG) {
#if (VIDEO_FUNC_MJPG == ENABLE)
		MP_VDOENC_ENCODER *pObj = MP_MjpgEnc_getVideoObject((MP_VDOENC_ID)pathID);
		ImageApp_Movie_SetParam(gMovie_Uvac_Info.uvac_id, MOVIE_PARAM_VDO_ENCODER_OBJ, (UINT32)pObj);
#else
		ImageApp_Movie_SetParam(gMovie_Uvac_Info.uvac_id, MOVIE_PARAM_VDO_ENCODER_OBJ, 0);
		DBG_ERR("[MOVIE_UVAC][%d] codec = %d not support\r\n", pathID, gMovie_Uvac_Info.codec);
#endif
	}
	if (gMovie_Uvac_Info.codec == _CFG_UVC_CODEC_H264) {
#if (VIDEO_FUNC_H264 == ENABLE)
		MP_VDOENC_ENCODER *pObj = MP_H264Enc_getVideoObject((MP_VDOENC_ID)pathID);
		ImageApp_Movie_SetParam(gMovie_Uvac_Info.uvac_id, MOVIE_PARAM_VDO_ENCODER_OBJ, (UINT32)pObj);
#else
		ImageApp_Movie_SetParam(gMovie_Uvac_Info.uvac_id, MOVIE_PARAM_VDO_ENCODER_OBJ, 0);
		DBG_ERR("[MOVIE_UVAC][%d] codec = %d not support\r\n", pathID, gMovie_Uvac_Info.codec);
#endif
	}

#endif
//#NT#2018/03/21#Charlie Chang -end

#if MOVIE_DUAL_RECORD_TEST
	pathID = (UINT32)ImageApp_Movie_GetVdoEncPort(gMovie_Rec_Info2.rec_id);
	if (gMovie_Rec_Info2.codec == _CFG_CODEC_MJPG) {
#if (VIDEO_FUNC_MJPG == ENABLE)
		MP_VDOENC_ENCODER *pObj = MP_MjpgEnc_getVideoObject((MP_VDOENC_ID)pathID);
		ImageApp_Movie_SetParam(gMovie_Rec_Info2.rec_id, MOVIE_PARAM_VDO_ENCODER_OBJ, (UINT32)pObj);
#else
		ImageApp_Movie_SetParam(gMovie_Rec_Info2.rec_id, MOVIE_PARAM_VDO_ENCODER_OBJ, 0);
		DBG_ERR("[MOVIE2][%d] codec = %d not support\r\n", pathID, gMovie_Rec_Info2.codec);
#endif
	}
	if (gMovie_Rec_Info2.codec == _CFG_CODEC_H264) {
#if (VIDEO_FUNC_H264 == ENABLE)
		MP_VDOENC_ENCODER *pObj = MP_H264Enc_getVideoObject((MP_VDOENC_ID)pathID);
		ImageApp_Movie_SetParam(gMovie_Rec_Info2.rec_id, MOVIE_PARAM_VDO_ENCODER_OBJ, (UINT32)pObj);
#else
		ImageApp_Movie_SetParam(gMovie_Rec_Info2.rec_id, MOVIE_PARAM_VDO_ENCODER_OBJ, 0);
		DBG_ERR("[MOVIE2][%d] codec = %d not support\r\n", pathID, gMovie_Rec_Info2.codec);
#endif
	}
	if (gMovie_Rec_Info2.codec == _CFG_CODEC_H265) {
#if (VIDEO_FUNC_H265 == ENABLE)
		MP_VDOENC_ENCODER *pObj = MP_H265Enc_getVideoObject((MP_VDOENC_ID)pathID);
		ImageApp_Movie_SetParam(gMovie_Rec_Info2.rec_id, MOVIE_PARAM_VDO_ENCODER_OBJ, (UINT32)pObj);
#else
		ImageApp_Movie_SetParam(gMovie_Rec_Info2.rec_id, MOVIE_PARAM_VDO_ENCODER_OBJ, 0);
		DBG_ERR("[MOVIE2][%d] codec = %d not support\r\n", pathID, gMovie_Rec_Info2.codec);
#endif
	}
#endif

    if (UI_GetData(FL_MovieDisIndex) == MOVIE_DIS_ON) {
        // enable DIS
		MovieExe_IPL_SetIInfo(IPL_SEL_DISCOMP_IMM, SEL_DISCOMP_ON);
    } else {
        // disble DIS
		MovieExe_IPL_SetIInfo(IPL_SEL_DISCOMP_IMM, SEL_DISCOMP_OFF);
    }

#if(WIFI_AP_FUNC==ENABLE)
    if (System_GetState(SYS_STATE_CURRSUBMODE) == SYS_SUBMODE_WIFI) {
    	Ux_SendEvent(&CustomMovieObjCtrl, NVTEVT_EXE_MOVIE_STRM_START, 1,gMovie_Strm_Info.strm_id);
        Ux_PostEvent(NVTEVT_WIFI_EXE_MODE_DONE,1,E_OK);
    }
    else{
        #if (defined(_MODEL_ARTOSYN_DVCAM_EVB_))
			////start strm
			//wait linux
			Delay_DelayMs(1000);
	    	Ux_SendEvent(&CustomMovieObjCtrl, NVTEVT_EXE_MOVIE_STRM_START, 1,gMovie_Strm_Info.strm_id);
            Ux_PostEvent(NVTEVT_WIFI_EXE_MODE_DONE,1,E_OK);
			#if MOVIE_UVAC_FUNC
				////start uvac
				ImageApp_Movie_Uvac_Start(gMovie_Uvac_Info.uvac_id);
			#endif
        #endif
    }
#endif
}
#endif

static void MovieExe_InitVideosprite(void){
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
static void MovieExe_InitVideoSpriteIpc(void){
	static int init = 0;
	if(!init)
		VideoSpriteIpc_Open();

	init = 1;
}
#endif

#if (LOG_DBGINFO_IN_GPS_SECTION == ENABLE)
static NVTMPP_VB_POOL mp4log_pool = 0;

static void MovieExe_InitMp4DbgLog(void)
{
	NVTMPP_VB_BLK  blk;
	UINT32         blk_size = ALIGN_CEIL_64(1024*100*2);    // Max GPS buffer 100K * 2

	mp4log_Open();

    // Add debug log sub-section here
    // Ex: mp4log_AddSection("TEST", 1, &DbgLogTestID);
    alg_mp4log_AddSection();
	if ((blk_size = mp4log_GetTotalLogSize()) != 0) {
		blk_size = ALIGN_CEIL_64(blk_size * 1024 * 2);

		mp4log_pool = nvtmpp_vb_create_pool("MP4Log", blk_size, 1, NVTMPP_DDR_1);
		if (NVTMPP_VB_INVALID_POOL == mp4log_pool)	{
			DBG_ERR("create private pool err\r\n");
			return;
		}
		blk = nvtmpp_vb_get_block(0, mp4log_pool, blk_size, NVTMPP_DDR_1);
		if (NVTMPP_VB_INVALID_BLK == blk) {
			DBG_ERR("get vb block err\r\n");
			return;
		}
		mp4log_SetMem(nvtmpp_vb_block2addr(blk), blk_size);
	}
}

static void MovieExe_UnInitMp4DbgLog(void)
{
	NVTMPP_ER ret;

	mp4log_Close();

	//destroy mempool
	ret = nvtmpp_vb_destroy_pool(mp4log_pool);
	if (ret != NVTMPP_ER_OK) {
		DBG_ERR("destory mp4log pool fail (ret = %d)\r\n", ret);
	}
}
#endif  // #if (LOG_DBGINFO_IN_GPS_SECTION == ENABLE)

void MovieExe_Init_XMLMem(void)
{
    #if(WIFI_AP_FUNC==ENABLE)
    //change to WiFi mode would not re-open movie mode,need alloc before enter WiFi
    //and not release
	//if (System_GetState(SYS_STATE_CURRSUBMODE) == SYS_SUBMODE_WIFI)
	{
		//xmlPool = AppMem_Alloc("XML", POOL_SIZE_TEMP);
		//XML_SetTempMem(xmlPool.Addr,xmlPool.Size);
		NVTMPP_VB_POOL pool;
		NVTMPP_VB_BLK  blk;
		UINT32         blk_size = POOL_SIZE_TEMP;
		pool = nvtmpp_vb_create_pool("XML", blk_size, 1, NVTMPP_DDR_1);
		if (NVTMPP_VB_INVALID_POOL == pool)
		{
			DBG_ERR("create private pool err\r\n");
		}
		blk = nvtmpp_vb_get_block(0, pool, blk_size, NVTMPP_DDR_1);
		if (NVTMPP_VB_INVALID_BLK == blk)
		{
			DBG_ERR("get vb block err\r\n");
		}
		XML_SetTempMem(nvtmpp_vb_block2addr(blk), blk_size);
	}
    #endif
}

INT32 MovieExe_OnOpen(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
	//call default
	Ux_DefaultEvent(pCtrl, NVTEVT_EXE_OPEN, paramNum, paramArray);

	if (MovieExe_InitCommonMem() < 0) {
		return NVTEVT_CONSUME;
	}

#if USE_FILEDB
	UI_SetData(FL_IsUseFileDB, 1);
#else
	UI_SetData(FL_IsUseFileDB, 0);
#endif

    MovieExe_InitPhotoFunc();

	MovieExe_InitVideosprite();

	#if (VIDEOSPRITEIPC==ENABLE)
	MovieExe_InitVideoSpriteIpc();
	#endif

	#if (LOG_DBGINFO_IN_GPS_SECTION == ENABLE)
	MovieExe_InitMp4DbgLog();
	#endif  // #if (LOG_DBGINFO_IN_GPS_SECTION == ENABLE)

	Movie_RegCB();

    //MovieExe_FileDB_Create();
    TM_BOOT_BEGIN("ImageStream", "open");
	MovieExe_ImageStreamOpen_MOVIE();
	TM_BOOT_END("ImageStream", "open");
	System_WaitFS();
	// set reserved size
	Movie_SetResvSize();
	TM_BOOT_BEGIN("fdb", "create");
#if MOVIE_MAPPING_MULTIREC
	ImageApp_MovieMulti_FMOpen('A');
	#ifdef _NVT_ITRON_DRIVE_SD_3_
	ImageApp_MovieMulti_FMOpen('B');
	#endif
#else
	ImageApp_Movie_FileDB_Create('A');
	#ifdef _NVT_ITRON_DRIVE_SD_3_
	ImageApp_Movie_FileDB_Create('B');
	#endif
#endif
	TM_BOOT_END("fdb", "create");

    //update display coordinate
	MovieExe_CalcFDDispCord(&gMovieFdDispCord);
	MovieExe_CalcOSDDispCord(&gMovieOsdDispCord);

	return NVTEVT_CONSUME;
}

INT32 MovieExe_OnClose(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
	UINT32 i;

    UIPhotoFunc_Close();

	//MovieExe_FileDB_Release();
#if(WIFI_AP_FUNC==ENABLE)
    if (System_GetState(SYS_STATE_CURRSUBMODE) == SYS_SUBMODE_WIFI) {
    	Ux_SendEvent(&CustomMovieObjCtrl, NVTEVT_EXE_MOVIE_STRM_STOP, 1,gMovie_Strm_Info.strm_id);
    }
#endif
    	MovieStamp_Disable();
	MovieStamp_DestroyBuff();
#if MOVIE_UVAC_FUNC
	ImageApp_MovieMulti_UvacStop(_CFG_UVAC_ID_1);
	#if (SENSOR_CAPS_COUNT == 2)
	ImageApp_MovieMulti_ImgLinkForStreaming(_CFG_REC_ID_1, DISABLE, TRUE);
	ImageApp_MovieMulti_ImgLinkForStreaming(_CFG_REC_ID_2, DISABLE, TRUE);
	#elif (SENSOR_CAPS_COUNT == 4)
	ImageApp_MovieMulti_ImgLinkForStreaming(_CFG_REC_ID_1, DISABLE, TRUE);
	ImageApp_MovieMulti_ImgLinkForStreaming(_CFG_REC_ID_2, DISABLE, TRUE);
	ImageApp_MovieMulti_ImgLinkForStreaming(_CFG_REC_ID_3, DISABLE, TRUE);
	ImageApp_MovieMulti_ImgLinkForStreaming(_CFG_REC_ID_4, DISABLE, TRUE);
	#else
	ImageApp_MovieMulti_ImgLinkForStreaming(_CFG_REC_ID_1, DISABLE, FALSE);
	#endif
#endif

	//#NT#2018/06/11#KCHong -begin
	//#NT#For MovieMulti Test
	ImageApp_MovieMulti_DispStop(_CFG_DISP_ID_1);
	#if (SENSOR_CAPS_COUNT == 2)
	ImageApp_MovieMulti_ImgLinkForDisp(_CFG_REC_ID_1, DISABLE, TRUE);
	ImageApp_MovieMulti_ImgLinkForDisp(_CFG_REC_ID_2, DISABLE, TRUE);
	#elif (SENSOR_CAPS_COUNT == 4)
	ImageApp_MovieMulti_ImgLinkForDisp(_CFG_REC_ID_1, DISABLE, TRUE);
	ImageApp_MovieMulti_ImgLinkForDisp(_CFG_REC_ID_2, DISABLE, TRUE);
	ImageApp_MovieMulti_ImgLinkForDisp(_CFG_REC_ID_3, DISABLE, TRUE);
	ImageApp_MovieMulti_ImgLinkForDisp(_CFG_REC_ID_4, DISABLE, TRUE);
	#else
	ImageApp_MovieMulti_ImgLinkForDisp(_CFG_REC_ID_1, DISABLE, FALSE);
	#endif
	//#NT#2018/06/11#KCHong -end

	// For ALG function
	for (i = 0; i < SENSOR_CAPS_COUNT; i++) {
		if (gMovie_Alg_Info[_CFG_REC_ID_1 + i].path13.ImgSize.w && gMovie_Alg_Info[_CFG_REC_ID_1 + i].path13.ImgSize.h) {
			ImageApp_MovieMulti_ImgLinkForAlg((_CFG_REC_ID_1 + i), _CFG_ALG_PATH3, DISABLE, TRUE);
		}
		if (gMovie_Alg_Info[_CFG_REC_ID_1 + i].path14.ImgSize.w && gMovie_Alg_Info[_CFG_REC_ID_1 + i].path14.ImgSize.h) {
			ImageApp_MovieMulti_ImgLinkForAlg((_CFG_REC_ID_1 + i), _CFG_ALG_PATH4, DISABLE, TRUE);
		}
	}

#if MOVIE_MAPPING_MULTIREC
	ImageApp_MovieMulti_Close();
#else
	ImageApp_Movie_Close();
#endif

#if MOVIE_MAPPING_MULTIREC
	ImageApp_MovieMulti_FMClose('A');
	#ifdef _NVT_ITRON_DRIVE_SD_3_
	ImageApp_MovieMulti_FMClose('B');
	#endif
#else
	ImageApp_Movie_FileDB_Release('A');
	#ifdef _NVT_ITRON_DRIVE_SD_3_
	ImageApp_Movie_FileDB_Release('B');
	#endif
#endif

	#if (LOG_DBGINFO_IN_GPS_SECTION == ENABLE)
	MovieExe_UnInitMp4DbgLog();
	#endif  // #if (LOG_DBGINFO_IN_GPS_SECTION == ENABLE)

	if (MovieExe_ExitCommonMem() < 0) {
		return NVTEVT_CONSUME;
	}

	//call default
	Ux_DefaultEvent(pCtrl, NVTEVT_EXE_CLOSE, paramNum, paramArray);

	return NVTEVT_CONSUME;
}

#if MOVIE_MAPPING_MULTIREC
static void MovieExe_SetRecParamByRecID(MOVIE_CFG_REC_ID rec_id)
{
	// set record type (video only, video and audio, time-lapse, ...)
	if (SysGetFlag(FL_MOVIE_TIMELAPSE_REC) == MOVIE_TIMELAPSEREC_OFF) {
		if (SysGetFlag(FL_MovieAudioRec) == MOVIE_AUD_REC_OFF) {
			ImageApp_MovieMulti_SetParam(rec_id, MOVIEMULTI_PARAM_REC_FORMAT, _CFG_REC_TYPE_VID);
		} else {
			ImageApp_MovieMulti_SetParam(rec_id, MOVIEMULTI_PARAM_REC_FORMAT, _CFG_REC_TYPE_AV);
		}
		ImageApp_MovieMulti_SetParam(rec_id, MOVIEMULTI_PARAM_TIMELAPSE_TIME, 0);
		ImageApp_MovieMulti_SetParam(rec_id, MOVIEMULTI_PARAM_FILE_ENDTYPE, (SysGetFlag(FL_MOVIE_CYCLIC_REC) == MOVIE_CYCLICREC_OFF) ? MOVREC_ENDTYPE_NORMAL : MOVREC_ENDTYPE_CUTOVERLAP);
	} else {
		ImageApp_MovieMulti_SetParam(rec_id, MOVIEMULTI_PARAM_REC_FORMAT, _CFG_REC_TYPE_TIMELAPSE);
		ImageApp_MovieMulti_SetParam(rec_id, MOVIEMULTI_PARAM_TIMELAPSE_TIME, Get_MovieTimeLapseValue(SysGetFlag(FL_MOVIE_TIMELAPSE_REC)));
		ImageApp_MovieMulti_SetParam(rec_id, MOVIEMULTI_PARAM_FILE_PLAYFRAMERATE, MOVIE_TIMELAPSE_PLAYFRAMERATE);
		ImageApp_MovieMulti_SetParam(rec_id, MOVIEMULTI_PARAM_FILE_ENDTYPE, MOVREC_ENDTYPE_NORMAL);
	}
}

INT32 MovieExe_OnRecStart(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
	UINT32 skip_frame;
    UINT32 uifile_buffer_reserved_sec = 10;  // 7~29 seconds.
	UINT32 i, mask;
	UINT32 movie_rec_mask, clone_rec_mask;

    #if 0
    // JIRA
    ISIZE devSize = GxVideo_GetDeviceSize(DOUT1);

    if (devSize.w >= 3840 && gMovie_Rec_Info.size.w >= 3840)
    {
        DBG_ERR("can't record 4K movie under 4K HDMI!\r\n");
        return NVTEVT_CONSUME;
    }
    #endif

	//#NT# save rec status
	UI_SetData(FL_MOVIE_REC, 1);

	// Set UDTA and GPS data
    Movie_SetUserData();

	movie_rec_mask = Movie_GetMovieRecMask();
	clone_rec_mask = Movie_GetCloneRecMask();

	mask = 1;
	for (i = 0; i < SENSOR_CAPS_COUNT; i++) {
		if (movie_rec_mask & mask) {
			MovieExe_SetRecParamByRecID(gMovie_Rec_Info[i].rec_id);
		}

		if (clone_rec_mask & mask) {
			MovieExe_SetRecParamByRecID(gMovie_Clone_Info[i].rec_id);
		}

		mask <<= 1;
	}

	// Set skip frame
	if ((SysGetFlag(FL_MOVIE_AUDIO) == MOVIE_AUDIO_OFF) ||                      // Video only
		(SysGetFlag(FL_MOVIE_TIMELAPSE_REC) != MOVIE_TIMELAPSEREC_OFF)) {       // Timelapse mode
		skip_frame = 0;
	} else {
		// Note: need adjust skip frame for different frame rate
		skip_frame = 3;
	}

	for (i = 0; i < SENSOR_CAPS_COUNT; i++) {
		ImageApp_MovieMulti_SetParam(gMovie_Rec_Info[i].rec_id, MOVIEMULTI_PARAM_SKIP_FRAME, skip_frame);
		// Set buffer reserved second. Default 10 (7~29).
		ImageApp_MovieMulti_SetParam(gMovie_Rec_Info[i].rec_id, MOVIEMULTI_PARAM_FILE_BUFRESSEC, uifile_buffer_reserved_sec);
		ImageApp_MovieMulti_SetParam(gMovie_Rec_Info[i].rec_id, MOVIEMULTI_PARAM_PRECALC_BUFFER, 40 * 1024 * 1024);
	}

    if (UI_GetData(FL_MOVIE_DATEIMPRINT) == MOVIE_DATEIMPRINT_ON) {

        STAMP_COLOR StampColorBg = {RGB_GET_Y( 16,  16,  16), RGB_GET_U( 16,  16,  16), RGB_GET_V( 16,  16,  16)}; // date stamp background color
        STAMP_COLOR StampColorFr = {RGB_GET_Y( 16,  16,  16), RGB_GET_U( 16,  16,  16), RGB_GET_V( 16,  16,  16)}; // date stamp frame color
        STAMP_COLOR StampColorFg = {RGB_GET_Y(224, 224, 192), RGB_GET_U(224, 224, 192), RGB_GET_V(224, 224, 192)}; // date stamp foreground color

        UINT32      uiStampAddr;
        UINT32      uiVidEncId=0;
#if (STAMP_PRIVATE_POOL == DISABLE)
        // use POOL_ID_DATEIMPRINT as movie data stamp buffer
        uiStampAddr = OS_GetMempoolAddr(POOL_ID_DATEIMPRINT);


        MovieStamp_SetBuffer(uiVidEncId, uiStampAddr, POOL_SIZE_DATEIMPRINT/2);

        MovieStamp_SetColor(uiVidEncId, &StampColorBg, &StampColorFr, &StampColorFg);

        MovieStamp_Setup(
            uiVidEncId,
            STAMP_ON |
            STAMP_AUTO |
            STAMP_DATE_TIME |
#if MOVIE_AE_LOG
			STAMP_TOP_LEFT |
#else
            STAMP_BOTTOM_RIGHT |
#endif
            STAMP_POS_NORMAL |
            STAMP_BG_TRANSPARENT |
            STAMP_YY_MM_DD |
            STAMP_IMG_420UV,
            gMovie_Rec_Info[0].size.w,
            gMovie_Rec_Info[0].size.h,
            NULL);

#else
        UINT32 movie_rec_mask, clone_rec_mask;
        UINT32 i, mask;
        movie_rec_mask = Movie_GetMovieRecMask();
        clone_rec_mask = Movie_GetCloneRecMask();

        mask = 1;
        for (i = 0; i < SENSOR_CAPS_COUNT; i++) {

            //DBG_DUMP("i=%d, main_rec=%d ,clone_rec=%d\r\n",i, (movie_rec_mask & mask), (clone_rec_mask & mask));
            //DBG_DUMP("i=%d, rec_id=%d ,path_id=%d\r\n",i, gMovie_Rec_Info[i].rec_id, ImageApp_MovieMulti_GetVdoEncPort(gMovie_Rec_Info[i].rec_id));

            if (movie_rec_mask & mask) {
                uiVidEncId=ImageApp_MovieMulti_GetVdoEncPort(gMovie_Rec_Info[i].rec_id);
                //DBG_DUMP("main uiVidEncId=%d\r\n",uiVidEncId);
                uiStampAddr=MovieStamp_GetBufAddr(uiVidEncId, MovieStamp_CalcBufSize(gMovie_Rec_Info[i].size.w, gMovie_Rec_Info[i].size.h));//POOL_SIZE_DATEIMPRINT/2);

                MovieStamp_SetBuffer(uiVidEncId, uiStampAddr, MovieStamp_CalcBufSize(gMovie_Rec_Info[i].size.w, gMovie_Rec_Info[i].size.h));//POOL_SIZE_DATEIMPRINT/2);

                MovieStamp_SetColor(uiVidEncId, &StampColorBg, &StampColorFr, &StampColorFg);

                MovieStamp_Setup(
                    uiVidEncId,
                    STAMP_ON |
                    STAMP_AUTO |
                    STAMP_DATE_TIME |
#if MOVIE_AE_LOG
        			STAMP_TOP_LEFT |
#else
                    STAMP_BOTTOM_RIGHT |
#endif
                    STAMP_POS_NORMAL |
                    STAMP_BG_TRANSPARENT |
                    STAMP_YY_MM_DD |
                    STAMP_IMG_420UV,
                    gMovie_Rec_Info[0].size.w,
                    gMovie_Rec_Info[0].size.h,
#if defined (WATERLOGO_FUNCTION) && (WATERLOGO_FUNCTION == ENABLE)
            (WATERLOGO_BUFFER *)&g_WaterLogo);
#else
                    NULL);
#endif
            }
            if (clone_rec_mask & mask) {
                uiVidEncId=ImageApp_MovieMulti_GetVdoEncPort(gMovie_Clone_Info[i].rec_id);
                //DBG_DUMP("clone uiVidEncId=%d\r\n",uiVidEncId);
                uiStampAddr=MovieStamp_GetBufAddr(uiVidEncId, MovieStamp_CalcBufSize(gMovie_Rec_Info[i].size.w, gMovie_Rec_Info[i].size.h));//POOL_SIZE_DATEIMPRINT/2);

                MovieStamp_SetBuffer(uiVidEncId, uiStampAddr, MovieStamp_CalcBufSize(gMovie_Rec_Info[i].size.w, gMovie_Rec_Info[i].size.h));//POOL_SIZE_DATEIMPRINT/2);

                MovieStamp_SetColor(uiVidEncId, &StampColorBg, &StampColorFr, &StampColorFg);

                MovieStamp_Setup(
                    uiVidEncId,
                    STAMP_ON |
                    STAMP_AUTO |
                    STAMP_DATE_TIME |
#if MOVIE_AE_LOG
        			STAMP_TOP_LEFT |
#else
                    STAMP_BOTTOM_RIGHT |
#endif
                    STAMP_POS_NORMAL |
                    STAMP_BG_TRANSPARENT |
                    STAMP_YY_MM_DD |
                    STAMP_IMG_420UV,
                    gMovie_Rec_Info[0].size.w,
                    gMovie_Rec_Info[0].size.h,
#if defined (WATERLOGO_FUNCTION) && (WATERLOGO_FUNCTION == ENABLE)
            (WATERLOGO_BUFFER *)&g_WaterLogo);
#else
                    NULL);
#endif
		}

		mask <<= 1;
	}
#endif



#if (THUMB_STAMP_FUNC == ENABLE)
	UINT32 Ratio, ThmWid, ThmHei;
	INT32  Tmp1, Tmp2;

	MovieStampThumb_SetBuffer();

	if (gMovie_Rec_Info[0].dar == _CFG_DAR_16_9) {
		ThmWid = NMI_IMGCAP_16_9_W;
		ThmHei = NMI_IMGCAP_16_9_H;
	} else {
		// get thumbnail size from rec resolution
		Ratio = gMovie_Rec_Info[0].size.w *100 / gMovie_Rec_Info[0].size.h;
		Tmp1 = (INT32)abs(Ratio - 100 * 16 / 9);
		Tmp2 = (INT32)abs(Ratio - 100 * 4 / 3);

		if (Tmp1 < 5) { // <5%, ignore
			ThmWid = NMI_IMGCAP_16_9_W;
			ThmHei = NMI_IMGCAP_16_9_H;
		}
		else if (Tmp2 < 5) { // <5%, ignore
			ThmWid = NMI_IMGCAP_4_3_W;
			ThmHei = NMI_IMGCAP_4_3_H;
		}
		else {
			DBG_ERR("Get thumb size error! w=%d h=%d\r\n", gMovie_Rec_Info[0].size.w, gMovie_Rec_Info[0].size.h);
			return NVTEVT_CONSUME;
		}
	}

	MovieStamp_Setup_Thumb(
			0,
            STAMP_ON |
            STAMP_AUTO |
            STAMP_DATE_TIME |
            STAMP_BOTTOM_RIGHT |
            STAMP_POS_NORMAL |
            STAMP_BG_TRANSPARENT |
            STAMP_YY_MM_DD |
            STAMP_IMG_420UV,
            ThmWid,
            ThmHei,
            NULL);
#endif

         MovieStamp_Enable();
    }

	alg_MovieExe_OnRecStart();

	//#NT#2017/10/26#Adam Su -begin
	//#NT#AUTO_TEST
	exam_msg("record start\r\n");
	//#NT#2017/10/26#Adam Su -end

	// trigger record
	mask = 1;
	for (i = 0; i < SENSOR_CAPS_COUNT; i++) {
		if (movie_rec_mask & mask) {
			ImageApp_MovieMulti_RecStart(gMovie_Rec_Info[i].rec_id);
		}

		if (clone_rec_mask & mask) {
			ImageApp_MovieMulti_RecStart(gMovie_Clone_Info[i].rec_id);
		}

		mask <<= 1;
	}

	// disable auto power off/USB detect timer
	KeyScan_EnableMisc(FALSE);

	return NVTEVT_CONSUME;

}
#else
INT32 MovieExe_OnRecStart(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
	UINT32 skip_frame;
    UINT32 uifile_buffer_reserved_sec = 10;  // 7~29 seconds.

    #if 0
    // JIRA
    ISIZE devSize = GxVideo_GetDeviceSize(DOUT1);

    if (devSize.w >= 3840 && gMovie_Rec_Info.size.w >= 3840)
    {
        DBG_ERR("can't record 4K movie under 4K HDMI!\r\n");
        return NVTEVT_CONSUME;
    }
    #endif

	//#NT# save rec status
	UI_SetData(FL_MOVIE_REC, 1);

	// Set UDTA and GPS data
    Movie_SetUserData();

	if (SysGetFlag(FL_MOVIE_TIMELAPSE_REC) == MOVIE_TIMELAPSEREC_OFF) {
		if (SysGetFlag(FL_MovieAudioRec) == MOVIE_AUD_REC_OFF) {
			ImageApp_Movie_SetParam(_CFG_REC_ID_1, MOVIE_PARAM_REC_FORMAT, _CFG_REC_TYPE_VID);
		} else {
			ImageApp_Movie_SetParam(_CFG_REC_ID_1, MOVIE_PARAM_REC_FORMAT, _CFG_REC_TYPE_AV);
		}
        ImageApp_Movie_SetParam(_CFG_REC_ID_1, MOVIE_PARAM_TIMELAPSE_TIME, 0);
		ImageApp_Movie_SetParam(_CFG_REC_ID_1, MOVIE_PARAM_FILE_ENDTYPE, (SysGetFlag(FL_MOVIE_CYCLIC_REC) == MOVIE_CYCLICREC_OFF)? MOVREC_ENDTYPE_NORMAL : MOVREC_ENDTYPE_CUTOVERLAP);
	} else {
		ImageApp_Movie_SetParam(_CFG_REC_ID_1, MOVIE_PARAM_REC_FORMAT, _CFG_REC_TYPE_TIMELAPSE);
		ImageApp_Movie_SetParam(_CFG_REC_ID_1, MOVIE_PARAM_TIMELAPSE_TIME, Get_MovieTimeLapseValue(SysGetFlag(FL_MOVIE_TIMELAPSE_REC)));
		ImageApp_Movie_SetParam(_CFG_REC_ID_1, MOVIE_PARAM_FILE_PLAYFRAMERATE, MOVIE_TIMELAPSE_PLAYFRAMERATE);
		ImageApp_Movie_SetParam(_CFG_REC_ID_1, MOVIE_PARAM_FILE_ENDTYPE, MOVREC_ENDTYPE_NORMAL);
	}

	// Set skip frame
	if ((SysGetFlag(FL_MOVIE_AUDIO) == MOVIE_AUDIO_OFF) ||                      // Video only
		(SysGetFlag(FL_MOVIE_TIMELAPSE_REC) != MOVIE_TIMELAPSEREC_OFF)) {       // Timelapse mode
		skip_frame = 0;
	} else {
		// Note: need adjust skip frame for different frame rate
		skip_frame = 3;
	}
	ImageApp_Movie_SetParam(_CFG_REC_ID_1, MOVIE_PARAM_SKIP_FRAME, skip_frame);

    // Set buffer reserved second. Default 10 (7~29).
    ImageApp_Movie_SetParam(_CFG_REC_ID_1, MOVIE_PARAM_FILE_BUFRESSEC, uifile_buffer_reserved_sec);
    ImageApp_Movie_SetParam(_CFG_REC_ID_1, MOVIE_PARAM_PRECALC_BUFFER, 40 * 1024 * 1024);

    if (UI_GetData(FL_MOVIE_DATEIMPRINT) == MOVIE_DATEIMPRINT_ON) {

        STAMP_COLOR StampColorBg = {RGB_GET_Y( 16,  16,  16), RGB_GET_U( 16,  16,  16), RGB_GET_V( 16,  16,  16)}; // date stamp background color
        STAMP_COLOR StampColorFr = {RGB_GET_Y( 16,  16,  16), RGB_GET_U( 16,  16,  16), RGB_GET_V( 16,  16,  16)}; // date stamp frame color
        STAMP_COLOR StampColorFg = {RGB_GET_Y(224, 224, 192), RGB_GET_U(224, 224, 192), RGB_GET_V(224, 224, 192)}; // date stamp foreground color

        UINT32      uiStampAddr;
        UINT32      uiVidEncId=0;
#if (STAMP_PRIVATE_POOL == DISABLE)
        // use POOL_ID_DATEIMPRINT as movie data stamp buffer
        uiStampAddr = OS_GetMempoolAddr(POOL_ID_DATEIMPRINT);
#else
        uiStampAddr=MovieStamp_GetBufAddr(uiVidEncId, MovieStamp_CalcBufSize(gMovie_Rec_Info[0].size.w, gMovie_Rec_Info[0].size.h));//POOL_SIZE_DATEIMPRINT/2);
#endif
        MovieStamp_SetBuffer(uiVidEncId, uiStampAddr, MovieStamp_CalcBufSize(gMovie_Rec_Info[0].size.w, gMovie_Rec_Info[0].size.h));//POOL_SIZE_DATEIMPRINT/2);

        MovieStamp_SetColor(uiVidEncId, &StampColorBg, &StampColorFr, &StampColorFg);

        MovieStamp_Setup(
            uiVidEncId,
            STAMP_ON |
            STAMP_AUTO |
            STAMP_DATE_TIME |
#if MOVIE_AE_LOG
			STAMP_TOP_LEFT |
#else
            STAMP_BOTTOM_RIGHT |
#endif
            STAMP_POS_NORMAL |
            STAMP_BG_TRANSPARENT |
            STAMP_YY_MM_DD |
            STAMP_IMG_420UV,
            gMovie_Rec_Info[0].size.w,
            gMovie_Rec_Info[0].size.h,
            NULL);
DBG_ERR("W=%d, %d\r\n",gMovie_Rec_Info[0].size.w,gMovie_Rec_Info[0].size.h);

        uiVidEncId=2;
#if (STAMP_PRIVATE_POOL == DISABLE)
        MovieStamp_SetBuffer(uiVidEncId, uiStampAddr+POOL_SIZE_DATEIMPRINT/2, POOL_SIZE_DATEIMPRINT/2);
#else
        uiStampAddr=MovieStamp_GetBufAddr(uiVidEncId, MovieStamp_CalcBufSize(gMovie_Rec_Info[0].size.w, gMovie_Rec_Info[0].size.h));//POOL_SIZE_DATEIMPRINT/2);
        MovieStamp_SetBuffer(uiVidEncId, uiStampAddr, MovieStamp_CalcBufSize(gMovie_Rec_Info[0].size.w, gMovie_Rec_Info[0].size.h));//POOL_SIZE_DATEIMPRINT/2);
#endif
        MovieStamp_SetColor(uiVidEncId, &StampColorBg, &StampColorFr, &StampColorFg);

        MovieStamp_Setup(
            uiVidEncId,
            STAMP_ON |
            STAMP_AUTO |
            STAMP_DATE_TIME |
#if MOVIE_AE_LOG
			STAMP_TOP_LEFT |
#else
            STAMP_BOTTOM_RIGHT |
#endif
            STAMP_POS_NORMAL |
            STAMP_BG_TRANSPARENT |
            STAMP_YY_MM_DD |
            STAMP_IMG_420UV,
            gMovie_Rec_Info[0].size.w,
            gMovie_Rec_Info[0].size.h,
            NULL);

        uiVidEncId=4;
#if (STAMP_PRIVATE_POOL == DISABLE)
        MovieStamp_SetBuffer(uiVidEncId, uiStampAddr+POOL_SIZE_DATEIMPRINT/2, POOL_SIZE_DATEIMPRINT/2);
#else
        uiStampAddr=MovieStamp_GetBufAddr(uiVidEncId, MovieStamp_CalcBufSize(gMovie_Rec_Info[0].size.w, gMovie_Rec_Info[0].size.h));//POOL_SIZE_DATEIMPRINT/2);
        MovieStamp_SetBuffer(uiVidEncId, uiStampAddr, MovieStamp_CalcBufSize(gMovie_Rec_Info[0].size.w, gMovie_Rec_Info[0].size.h));//POOL_SIZE_DATEIMPRINT/2);
#endif
        MovieStamp_SetColor(uiVidEncId, &StampColorBg, &StampColorFr, &StampColorFg);

        MovieStamp_Setup(
            uiVidEncId,
            STAMP_ON |
            STAMP_AUTO |
            STAMP_DATE_TIME |
#if MOVIE_AE_LOG
			STAMP_TOP_LEFT |
#else
            STAMP_BOTTOM_RIGHT |
#endif
            STAMP_POS_NORMAL |
            STAMP_BG_TRANSPARENT |
            STAMP_YY_MM_DD |
            STAMP_IMG_420UV,
            gMovie_Rec_Info[0].size.w,
            gMovie_Rec_Info[0].size.h,
            NULL);

        uiVidEncId=6;
#if (STAMP_PRIVATE_POOL == DISABLE)
        MovieStamp_SetBuffer(uiVidEncId, uiStampAddr+POOL_SIZE_DATEIMPRINT/2, POOL_SIZE_DATEIMPRINT/2);
#else
        uiStampAddr=MovieStamp_GetBufAddr(uiVidEncId, MovieStamp_CalcBufSize(gMovie_Rec_Info[0].size.w, gMovie_Rec_Info[0].size.h));//POOL_SIZE_DATEIMPRINT/2);
        MovieStamp_SetBuffer(uiVidEncId, uiStampAddr, MovieStamp_CalcBufSize(gMovie_Rec_Info[0].size.w, gMovie_Rec_Info[0].size.h));//POOL_SIZE_DATEIMPRINT/2);
#endif
        MovieStamp_SetColor(uiVidEncId, &StampColorBg, &StampColorFr, &StampColorFg);

        MovieStamp_Setup(
            uiVidEncId,
            STAMP_ON |
            STAMP_AUTO |
            STAMP_DATE_TIME |
#if MOVIE_AE_LOG
			STAMP_TOP_LEFT |
#else
            STAMP_BOTTOM_RIGHT |
#endif
            STAMP_POS_NORMAL |
            STAMP_BG_TRANSPARENT |
            STAMP_YY_MM_DD |
            STAMP_IMG_420UV,
            gMovie_Rec_Info[0].size.w,
            gMovie_Rec_Info[0].size.h,
            NULL);

#if (THUMB_STAMP_FUNC == ENABLE)
	UINT32 Ratio, ThmWid, ThmHei;
	INT32  Tmp1, Tmp2;

	MovieStampThumb_SetBuffer();

	if (gMovie_Rec_Info[0].dar == _CFG_DAR_16_9) {
		ThmWid = NMI_IMGCAP_16_9_W;
		ThmHei = NMI_IMGCAP_16_9_H;
	} else {
		// get thumbnail size from rec resolution
		Ratio = gMovie_Rec_Info[0].size.w *100 / gMovie_Rec_Info[0].size.h;
		Tmp1 = (INT32)abs(Ratio - 100 * 16 / 9);
		Tmp2 = (INT32)abs(Ratio - 100 * 4 / 3);

		if (Tmp1 < 5) { // <5%, ignore
			ThmWid = NMI_IMGCAP_16_9_W;
			ThmHei = NMI_IMGCAP_16_9_H;
		}
		else if (Tmp2 < 5) { // <5%, ignore
			ThmWid = NMI_IMGCAP_4_3_W;
			ThmHei = NMI_IMGCAP_4_3_H;
		}
		else {
			DBG_ERR("Get thumb size error! w=%d h=%d\r\n", gMovie_Rec_Info[0].size.w, gMovie_Rec_Info[0].size.h);
			return NVTEVT_CONSUME;
		}
	}

	MovieStamp_Setup_Thumb(
			0,
            STAMP_ON |
            STAMP_AUTO |
            STAMP_DATE_TIME |
#if MOVIE_AE_LOG
			STAMP_TOP_LEFT |
#else
            STAMP_BOTTOM_RIGHT |
#endif
            STAMP_POS_NORMAL |
            STAMP_BG_TRANSPARENT |
            STAMP_YY_MM_DD |
            STAMP_IMG_420UV,
            ThmWid,
            ThmHei,
            NULL);
#endif

#if (MOVIE_DUAL_RECORD_TEST)

#if (STAMP_PRIVATE_POOL == DISABLE)
        MovieStamp_SetBuffer(1, uiStampAddr+POOL_SIZE_DATEIMPRINT/2, POOL_SIZE_DATEIMPRINT/2);
#else
        uiStampAddr=MovieStamp_GetBufAddr(1, POOL_SIZE_DATEIMPRINT/2);
        MovieStamp_SetBuffer(1, uiStampAddr, POOL_SIZE_DATEIMPRINT/2);
#endif
        MovieStamp_SetColor(1, &StampColorBg, &StampColorFr, &StampColorFg);

        MovieStamp_Setup(
            1,
            STAMP_ON |
            STAMP_AUTO |
            STAMP_DATE_TIME |
#if MOVIE_AE_LOG
			STAMP_TOP_LEFT |
#else
            STAMP_BOTTOM_RIGHT |
#endif
            STAMP_POS_NORMAL |
            STAMP_BG_TRANSPARENT |
            STAMP_YY_MM_DD |
            STAMP_IMG_420UV,
            gMovie_Rec_Info2.size.w,
            gMovie_Rec_Info2.size.h,
            NULL);
#endif

         MovieStamp_Enable();
    }
	alg_MovieExe_OnRecStart();

	//#NT#2017/10/26#Adam Su -begin
	//#NT#AUTO_TEST
	exam_msg("record start\r\n");
	//#NT#2017/10/26#Adam Su -end

	ImageApp_Movie_Record_Start(gMovie_Rec_Info[0].rec_id);
    #if MOVIE_DUAL_RECORD_TEST
    ImageApp_Movie_Record_Start(gMovie_Rec_Info2.rec_id);
    #endif

	// disable auto power off/USB detect timer
	KeyScan_EnableMisc(FALSE);

	return NVTEVT_CONSUME;

}
#endif

INT32 MovieExe_OnStrmStart(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
//#NT#2018/06/15#KCHong -begin
//#NT#multirec test
#if 0
    UINT32 id = gMovie_Strm_Info.strm_id;

    if (paramNum) {
        id = paramArray[0];
    }
    ImageApp_Movie_Streaming_Start(id);
#else
	#if (SENSOR_CAPS_COUNT == 2)
	ImageApp_MovieMulti_ImgLinkForStreaming(_CFG_REC_ID_1, ENABLE, TRUE);
	ImageApp_MovieMulti_ImgLinkForStreaming(_CFG_REC_ID_2, ENABLE, TRUE);
	#elif (SENSOR_CAPS_COUNT == 4)
	ImageApp_MovieMulti_ImgLinkForStreaming(_CFG_REC_ID_1, ENABLE, TRUE);
	ImageApp_MovieMulti_ImgLinkForStreaming(_CFG_REC_ID_2, ENABLE, TRUE);
	ImageApp_MovieMulti_ImgLinkForStreaming(_CFG_REC_ID_3, ENABLE, TRUE);
	ImageApp_MovieMulti_ImgLinkForStreaming(_CFG_REC_ID_4, ENABLE, TRUE);
	#else
	ImageApp_MovieMulti_ImgLinkForStreaming(_CFG_REC_ID_1, ENABLE, FALSE);
	#endif
	ImageApp_MovieMulti_StreamingStart(_CFG_STRM_ID_1);
#endif
//#NT#2018/06/15#KCHong -end
#if 1
	if (UI_GetData(FL_MOVIE_DATEIMPRINT) == MOVIE_DATEIMPRINT_ON) {
	UINT32		uiStampAddr;
	STAMP_COLOR StampColorBg = {RGB_GET_Y( 16,	16,  16), RGB_GET_U( 16,  16,  16), RGB_GET_V( 16,	16,  16)}; // date stamp background color
	STAMP_COLOR StampColorFr = {RGB_GET_Y( 16,	16,  16), RGB_GET_U( 16,  16,  16), RGB_GET_V( 16,	16,  16)}; // date stamp frame color
	STAMP_COLOR StampColorFg = {RGB_GET_Y(224, 224, 192), RGB_GET_U(224, 224, 192), RGB_GET_V(224, 224, 192)}; // date stamp foreground color
	UINT32 path_id=ImageApp_MovieMulti_GetVdoEncPort(_CFG_STRM_ID_1);


#if (STAMP_PRIVATE_POOL == DISABLE)

	// use POOL_ID_DATEIMPRINT as movie data stamp buffer
	uiStampAddr = OS_GetMempoolAddr(POOL_ID_DATEIMPRINT);
	MovieStamp_SetBuffer(path_id, uiStampAddr+POOL_SIZE_DATEIMPRINT/2, POOL_SIZE_DATEIMPRINT/2);
#else
	uiStampAddr=MovieStamp_GetBufAddr(path_id, MovieStamp_CalcBufSize(gMovie_Strm_Info.size.w, gMovie_Strm_Info.size.h));//POOL_SIZE_DATEIMPRINT/2);
	MovieStamp_SetBuffer(path_id, uiStampAddr, MovieStamp_CalcBufSize(gMovie_Strm_Info.size.w, gMovie_Strm_Info.size.h));//POOL_SIZE_DATEIMPRINT/2);
#endif

	MovieStamp_SetColor(path_id, &StampColorBg, &StampColorFr, &StampColorFg);

	MovieStamp_Setup(
		path_id,
		STAMP_ON |
		STAMP_AUTO |
		STAMP_DATE_TIME |
		STAMP_BOTTOM_RIGHT |
		STAMP_POS_NORMAL |
		STAMP_BG_TRANSPARENT |
		STAMP_YY_MM_DD |
		STAMP_IMG_420UV,
		gMovie_Strm_Info.size.w,
		gMovie_Strm_Info.size.h,
		NULL);

        MovieStamp_Enable();
	}
#endif
	return NVTEVT_CONSUME;
}

INT32 MovieExe_OnRecStep(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
	return NVTEVT_CONSUME;
}

#if MOVIE_MAPPING_MULTIREC
INT32 MovieExe_OnRecStop(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
	UINT32 i, mask;
	UINT32 movie_rec_mask, clone_rec_mask;

	UI_SetData(FL_MOVIE_REC, 0);

	movie_rec_mask = Movie_GetMovieRecMask();
	clone_rec_mask = Movie_GetCloneRecMask();

	mask = 1;
	for (i = 0; i < SENSOR_CAPS_COUNT; i++) {
		if (movie_rec_mask & mask) {
			ImageApp_MovieMulti_RecStop(gMovie_Rec_Info[i].rec_id);
		}

		if (clone_rec_mask & mask) {
			ImageApp_MovieMulti_RecStop(gMovie_Clone_Info[i].rec_id);
		}

		mask <<= 1;
	}

	alg_MovieExe_OnRecStop();

	// Enable auto power off/USB detect timer
	KeyScan_EnableMisc(TRUE);

    if (UI_GetData(FL_MOVIE_DATEIMPRINT) == MOVIE_DATEIMPRINT_ON) {
         MovieStamp_Disable();
    }

	return NVTEVT_CONSUME;
}
#else
INT32 MovieExe_OnRecStop(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
	UI_SetData(FL_MOVIE_REC, 0);

	ImageApp_Movie_Record_Stop(gMovie_Rec_Info[0].rec_id);

    #if MOVIE_DUAL_RECORD_TEST
    ImageApp_Movie_Record_Stop(gMovie_Rec_Info2.rec_id);
    #endif

	alg_MovieExe_OnRecStop();

	// Enable auto power off/USB detect timer
	KeyScan_EnableMisc(TRUE);

    if (UI_GetData(FL_MOVIE_DATEIMPRINT) == MOVIE_DATEIMPRINT_ON) {
         MovieStamp_Disable();
    }

	return NVTEVT_CONSUME;
}
#endif

INT32 MovieExe_OnStrmStop(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
//#NT#2018/06/15#KCHong -begin
//#NT#multirec test
#if 0
    UINT32 id = gMovie_Strm_Info.strm_id;

    if (paramNum) {
        id = paramArray[0];
    }
    //ImageApp_Movie_Streaming_Stop(id);
#else
	ImageApp_MovieMulti_StreamingStop(_CFG_STRM_ID_1);
	#if (SENSOR_CAPS_COUNT == 2)
	ImageApp_MovieMulti_ImgLinkForStreaming(_CFG_REC_ID_1, DISABLE, TRUE);
	ImageApp_MovieMulti_ImgLinkForStreaming(_CFG_REC_ID_2, DISABLE, TRUE);
	#elif (SENSOR_CAPS_COUNT == 4)
	ImageApp_MovieMulti_ImgLinkForStreaming(_CFG_REC_ID_1, DISABLE, TRUE);
	ImageApp_MovieMulti_ImgLinkForStreaming(_CFG_REC_ID_2, DISABLE, TRUE);
	ImageApp_MovieMulti_ImgLinkForStreaming(_CFG_REC_ID_3, DISABLE, TRUE);
	ImageApp_MovieMulti_ImgLinkForStreaming(_CFG_REC_ID_4, DISABLE, TRUE);
	#else
	ImageApp_MovieMulti_ImgLinkForStreaming(_CFG_REC_ID_1, DISABLE, FALSE);
	#endif
#endif
//#NT#2018/06/15#KCHong -end
	return NVTEVT_CONSUME;
}

INT32 MovieExe_OnRecPIM(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
	return NVTEVT_CONSUME;
}

INT32 MovieExe_OnRecRawEnc(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
#if (MOVIE_MAPPING_MULTIREC == ENABLE)
	UINT32 i, mask, movie_rec_mask;

 	movie_rec_mask = Movie_GetMovieRecMask();
	mask = 1;
	for (i = 0; i < SENSOR_CAPS_COUNT; i++) {
		if (movie_rec_mask & mask) {
			ImageApp_MovieMulti_TriggerSnapshot(gMovie_Rec_Info[i].rec_id);
		}
		mask <<= 1;
	}
#else
	ImageApp_Movie_Trigger_Snapshot(gMovie_Rec_Info[0].rec_id);
#endif

	return NVTEVT_CONSUME;
}

INT32 MovieExe_OnCallback(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
	NVTEVT event;

	event = paramArray[0];

	DBG_IND("[cb]%d\r\n", event);
	switch (event) {
	case NVTEVT_ALGMSG_FLASH:
		DBG_IND("[cb]NVTEVT_ALGMSG_FLASH\r\n");
		break;

	case NVTEVT_ALGMSG_QVSTART:
		DBG_IND("[cb]NVTEVT_ALGMSG_QVSTART\r\n");
		break;

	case NVTEVT_ALGMSG_JPGOK:
		DBG_IND("[cb]NVTEVT_ALGMSG_JPGOK\r\n");
		break;

	case NVTEVT_ALGMSG_CAPFSTOK:
		DBG_IND("[cb]NVTEVT_ALGMSG_CAPFSTOK\r\n");
		break;


	case NVTEVT_ALGMSG_CAPTUREEND:
		DBG_IND("[cb]NVTEVT_ALGMSG_CAPTUREEND \r\n");
		break;

	case NVTEVT_ALGMSG_DZOOMSTEP:
		DBG_IND("[cb]NVTEVT_ALGMSG_DZOOMSTEP \r\n");
		break;

	case NVTEVT_ALGMSG_PREVIEW_STABLE:
		DBG_IND("[cb]NVTEVT_ALGMSG_PREVIEW_STABLE \r\n");
		break;

	default:

		break;
	}
	return NVTEVT_PASS;
}

INT32 MovieExe_OnVideoChange(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
#if (PIP_VIEW_FUNC == ENABLE)
	PipView_SetStyle(UI_GetData(FL_DUAL_CAM));
#endif
	//DBGD(Perf_GetCurrent());
	if (System_GetState(SYS_STATE_CURRMODE) == PRIMARY_MODE_MOVIE) {
		Ux_SendEvent(0, NVTEVT_EXE_IMAGE_RATIO, 1, GetMovieSizeRatio(UI_GetData(FL_MOVIE_SIZE)));
	}
	//DBGD(Perf_GetCurrent());
	return NVTEVT_PASS; //PASS this event to UIWnd!
}

INT32 MovieExe_OnDualcam(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
#if _TODO
	UINT32 uhSelect = 0;
	if (paramNum > 0) {
		uhSelect = paramArray[0];
	}

	DBG_IND("%d \r\n", uhSelect);

#if (SENSOR_CAPS_COUNT == 1)
	DBG_ERR("single sensor, not support this option!\r\n");
#endif
#if (SENSOR_CAPS_COUNT == 2)

	UI_SetData(FL_DUAL_CAM, uhSelect);

	// also change the capture id
	//PhotoExe_OnCaptureID(pCtrl, paramNum, paramArray);
	//if (localInfo->DualCam != uhSelect)
#if (PIP_VIEW_FASTSWITCH==ENABLE)
	{
		Ux_SendEvent(0, NVTEVT_SENSOR_DISPLAY, 1, (SENSOR_1 | SENSOR_2)); //for Always trigger PIP View
		//#NT#2015/11/25#Niven Cho#[87393][88541] -begin
		//Here be invoked at startup without in any mode
		if (System_GetState(SYS_STATE_CURRMODE) == PRIMARY_MODE_MOVIE) {
			Ux_SendEvent(0, NVTEVT_EXE_IMAGE_RATIO, 1, GetMovieSizeRatio(UI_GetData(FL_MOVIE_SIZE)));
		}
		PipView_SetStyle(UI_GetData(FL_DUAL_CAM));
		//#NT#2015/11/25#Niven Cho -end
	}
#else
	{
		// set display display
		if (uhSelect == DUALCAM_FRONT) {
			Ux_SendEvent(0, NVTEVT_SENSOR_DISPLAY, 1, SENSOR_1);
		}
		if (uhSelect == DUALCAM_BEHIND) {
			Ux_SendEvent(0, NVTEVT_SENSOR_DISPLAY, 1, SENSOR_2);
		}
		if (uhSelect == DUALCAM_BOTH) {
			Ux_SendEvent(0, NVTEVT_SENSOR_DISPLAY, 1, (SENSOR_1 | SENSOR_2));
		}
	}
#endif //(PIP_VIEW_FASTSWITCH==ENABLE)

#if(WIFI_AP_FUNC==ENABLE)
	if (System_GetState(SYS_STATE_CURRSUBMODE) == SYS_SUBMODE_WIFI) {
		// config pip view parameter
		{
			USIZE   HttpLviewSize;
			UINT32  ImageRatioIdx;

			ImageRatioIdx = GetMovieSizeRatio(UI_GetData(FL_MOVIE_SIZE));
			HttpLviewSize = MOVIE_HTTPLVIEW_SIZE[ImageRatioIdx];
			PhotoExe_CfgNetHttpPIP(HttpLviewSize, UI_GetData(FL_DUAL_CAM));

		}
	}
#endif

#endif

#endif
	return NVTEVT_CONSUME;
}

//#NT#2016/06/08#Lincy Lin -begin
//#NT#Implement generic OSD and video drawing mechanism for ALG function
static void MovieExe_CallBackUpdateInfo(UINT32 callBackEvent)
{
#if(UI_FUNC==ENABLE)
	VControl *pCurrnetWnd;

	Ux_GetFocusedWindow(&pCurrnetWnd);
	Ux_SendEvent(pCurrnetWnd, NVTEVT_UPDATE_INFO, 1, callBackEvent);
#endif
}
//#NT#2016/06/08#Lincy Lin -end


INT32 MovieExe_OnProtectAuto(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
#if 0
	UINT32 uiSelect    = 0;
	UINT32 uiWidth     = GetMovieSizeWidth(UI_GetData(FL_MOVIE_SIZE));

	if (paramNum) {
		uiSelect = paramArray[0];
	}

	UI_SetData(FL_MOVIE_URGENT_PROTECT_AUTO, uiSelect);

	if (UI_GetData(FL_MOVIE_URGENT_PROTECT_AUTO) == MOVIE_URGENT_PROTECT_AUTO_ON ||
		UI_GetData(FL_MOVIE_URGENT_PROTECT_MANUAL) == MOVIE_URGENT_PROTECT_MANUAL_ON) {
#if (MOVIE_MULTI_RECORD_FUNC)
		if (GetMovieRecType_2p(UI_GetData(FL_MOVIE_SIZE)) == MOVIE_REC_TYPE_DUAL
			|| GetMovieRecType_2p(UI_GetData(FL_MOVIE_SIZE)) == MOVIE_REC_TYPE_CLONE) {
			DBG_ERR("dual record, not support this option!\r\n");
#if _TODO
			MovRec_SetEmergencyRecording(0, 0);
#endif
			return NVTEVT_CONSUME;
		}
#endif
		// protect on
		if (uiWidth <= 1920) {
#if _TODO
			//#NT#2016/08/09#Niven Cho -begin
			//#NT#LINUX-CARDV, Protect memory is not enough
#if (_ADAS_FUNC_ == ENABLE || defined(_CPU2_LINUX_))
			MovRec_SetMaxStopFlashSec(10); // 10 seconds
#else
			MovRec_SetMaxStopFlashSec(30); // 30 seconds
#endif
			MovRec_SetEmergencyRecording(0, 1);
			//#NT#2016/08/09#Niven Cho -end
#endif
		} else {
			DBG_DUMP("use MovRec_SetCrash() under higher resolution!\r\n");
#if _TODO
			MovRec_SetEmergencyRecording(0, 0);  // use MovRec_SetCrash() under higher resolution.
#endif
		}

	} else {
#if _TODO
		// protect off
		MovRec_SetEmergencyRecording(0, 0);
#endif
	}
#else
	UINT32 uiSelect    = 0;
#if (MOVIE_MAPPING_MULTIREC == ENABLE)
	UINT32 i;
#endif

	if (paramNum) {
		uiSelect = paramArray[0];
	}
	UI_SetData(FL_MOVIE_URGENT_PROTECT_AUTO, uiSelect);

	if (UI_GetData(FL_MOVIE_URGENT_PROTECT_AUTO) == MOVIE_URGENT_PROTECT_AUTO_ON ||
		UI_GetData(FL_MOVIE_URGENT_PROTECT_MANUAL) == MOVIE_URGENT_PROTECT_MANUAL_ON) {
#if (MOVIE_MAPPING_MULTIREC == ENABLE)
		if (GetMovieRecType_2p(UI_GetData(FL_MOVIE_SIZE)) == MOVIE_REC_TYPE_FRONT) {
			gMovie_Rec_Option.emr_on = _CFG_EMR_MODE1;      // main + emr
		} else {
			gMovie_Rec_Option.emr_on = _CFG_EMR_MODE4;      // set crash
		}
		for (i = 0; i < SENSOR_CAPS_COUNT; i++) {
			ImageApp_MovieMulti_SetParam(gMovie_Rec_Info[i].rec_id, MOVIEMULTI_PARAM_FILE_ROLLBACKSEC, gMovie_Rec_Option.emr_sec);
			ImageApp_MovieMulti_SetParam(gMovie_Rec_Info[i].rec_id, MOVIEMULTI_PARAM_FILE_KEEPSEC, gMovie_Rec_Option.keep_sec);
			ImageApp_MovieMulti_SetParam(gMovie_Rec_Info[i].rec_id, MOVIEMULTI_PARAM_FILE_EMRON, gMovie_Rec_Option.emr_on);
		}
#else
		if (gMovie_Rec_Option.emr_on == FALSE) {
			gMovie_Rec_Option.emr_on = TRUE;
			UI_SetData(FL_MOVIE_TIMELAPSE_REC_MENU, MOVIE_TIMELAPSEREC_OFF);
			ImageApp_Movie_SetParam(_CFG_REC_ID_1, MOVIE_PARAM_FILE_EMRSEC, gMovie_Rec_Option.emr_sec);
			ImageApp_Movie_SetParam(_CFG_REC_ID_1, MOVIE_PARAM_FILE_KEEPSEC, gMovie_Rec_Option.keep_sec);
			ImageApp_Movie_SetParam(_CFG_REC_ID_1, MOVIE_PARAM_FILE_EMRON, gMovie_Rec_Option.emr_on);
		}
#endif
	} else {
#if (MOVIE_MAPPING_MULTIREC == ENABLE)
		if (gMovie_Rec_Option.emr_on != _CFG_EMR_OFF) {
			gMovie_Rec_Option.emr_on = _CFG_EMR_OFF;
			for (i = 0; i < SENSOR_CAPS_COUNT; i++) {
				ImageApp_MovieMulti_SetParam(gMovie_Rec_Info[i].rec_id, MOVIEMULTI_PARAM_FILE_EMRON, gMovie_Rec_Option.emr_on);
			}
		}
#else
		if (gMovie_Rec_Option.emr_on == TRUE) {
			gMovie_Rec_Option.emr_on = FALSE;
			ImageApp_Movie_SetParam(_CFG_REC_ID_1, MOVIE_PARAM_FILE_EMRON, gMovie_Rec_Option.emr_on);
		}
#endif
	}
#endif
	return NVTEVT_CONSUME;
}
INT32 MovieExe_OnProtectManual(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
#if 0
	UINT32 uiSelect    = 0;
	UINT32 uiWidth     = GetMovieSizeWidth(UI_GetData(FL_MOVIE_SIZE));

	if (paramNum) {
		uiSelect = paramArray[0];
	}

	UI_SetData(FL_MOVIE_URGENT_PROTECT_MANUAL, uiSelect);

	if (UI_GetData(FL_MOVIE_URGENT_PROTECT_AUTO) == MOVIE_URGENT_PROTECT_AUTO_ON ||
		UI_GetData(FL_MOVIE_URGENT_PROTECT_MANUAL) == MOVIE_URGENT_PROTECT_MANUAL_ON) {
#if (MOVIE_MULTI_RECORD_FUNC)
		if (GetMovieRecType_2p(UI_GetData(FL_MOVIE_SIZE)) == MOVIE_REC_TYPE_DUAL
			|| GetMovieRecType_2p(UI_GetData(FL_MOVIE_SIZE)) == MOVIE_REC_TYPE_CLONE) {
			DBG_ERR("dual record, not support this option!\r\n");
#if _TODO
			MovRec_SetEmergencyRecording(0, 0);
#endif
			return NVTEVT_CONSUME;
		}
#endif
		// protect manual on
		if (uiWidth <= 1920) {
#if _TODO
			//#NT#2016/08/09#Niven Cho -begin
			//#NT#LINUX-CARDV, Protect memory is not enough
#if (_ADAS_FUNC_ == ENABLE || defined(_CPU2_LINUX_))
			MovRec_SetMaxStopFlashSec(10); // 10 seconds
#else
			MovRec_SetMaxStopFlashSec(30); // 30 seconds
#endif

			MovRec_SetEmergencyRecording(0, 1);
			//#NT#2016/08/09#Niven Cho -end
#endif
		} else {
			DBG_DUMP("use MovRec_SetCrash() under higher resolution!\r\n");
#if _TODO
			MovRec_SetEmergencyRecording(0, 0);  // use MovRec_SetCrash() under higher resolution.
#endif
		}
	} else {
		// protect manual off
#if _TODO
		MovRec_SetEmergencyRecording(0, 0);
#endif
	}
#else
	UINT32 uiSelect    = 0;
#if (MOVIE_MAPPING_MULTIREC == ENABLE)
	UINT32 i;
#endif

	if (paramNum) {
		uiSelect = paramArray[0];
	}
	UI_SetData(FL_MOVIE_URGENT_PROTECT_MANUAL, uiSelect);

	if (UI_GetData(FL_MOVIE_URGENT_PROTECT_AUTO) == MOVIE_URGENT_PROTECT_AUTO_ON ||
		UI_GetData(FL_MOVIE_URGENT_PROTECT_MANUAL) == MOVIE_URGENT_PROTECT_MANUAL_ON) {
#if (MOVIE_MAPPING_MULTIREC == ENABLE)
		if (GetMovieRecType_2p(UI_GetData(FL_MOVIE_SIZE)) == MOVIE_REC_TYPE_FRONT) {
			gMovie_Rec_Option.emr_on = _CFG_EMR_MODE1;      // main + emr
		} else {
			gMovie_Rec_Option.emr_on = _CFG_EMR_MODE4;      // set crash
		}
		for (i = 0; i < SENSOR_CAPS_COUNT; i++) {
			ImageApp_MovieMulti_SetParam(gMovie_Rec_Info[i].rec_id, MOVIEMULTI_PARAM_FILE_ROLLBACKSEC, gMovie_Rec_Option.emr_sec);
			ImageApp_MovieMulti_SetParam(gMovie_Rec_Info[i].rec_id, MOVIEMULTI_PARAM_FILE_KEEPSEC, gMovie_Rec_Option.keep_sec);
			ImageApp_MovieMulti_SetParam(gMovie_Rec_Info[i].rec_id, MOVIEMULTI_PARAM_FILE_EMRON, gMovie_Rec_Option.emr_on);
		}
#else
		if (gMovie_Rec_Option.emr_on == FALSE) {
			gMovie_Rec_Option.emr_on = TRUE;
			UI_SetData(FL_MOVIE_TIMELAPSE_REC_MENU, MOVIE_TIMELAPSEREC_OFF);
			ImageApp_Movie_SetParam(_CFG_REC_ID_1, MOVIE_PARAM_FILE_EMRSEC, gMovie_Rec_Option.emr_sec);
			ImageApp_Movie_SetParam(_CFG_REC_ID_1, MOVIE_PARAM_FILE_KEEPSEC, gMovie_Rec_Option.keep_sec);
			ImageApp_Movie_SetParam(_CFG_REC_ID_1, MOVIE_PARAM_FILE_EMRON, gMovie_Rec_Option.emr_on);
		}
#endif
	} else {
#if (MOVIE_MAPPING_MULTIREC == ENABLE)
		if (gMovie_Rec_Option.emr_on != _CFG_EMR_OFF) {
			gMovie_Rec_Option.emr_on = _CFG_EMR_OFF;
			for (i = 0; i < SENSOR_CAPS_COUNT; i++) {
				ImageApp_MovieMulti_SetParam(gMovie_Rec_Info[i].rec_id, MOVIEMULTI_PARAM_FILE_EMRON, gMovie_Rec_Option.emr_on);
			}
		}
#else
		if (gMovie_Rec_Option.emr_on == TRUE) {
			gMovie_Rec_Option.emr_on = FALSE;
			ImageApp_Movie_SetParam(_CFG_REC_ID_1, MOVIE_PARAM_FILE_EMRON, gMovie_Rec_Option.emr_on);
		}
#endif
	}
#endif
	return NVTEVT_CONSUME;
}


INT32 MovieExe_OnLDWS(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
	return alg_MovieExe_OnLDWS(pCtrl, paramNum, paramArray);
}

INT32 MovieExe_OnFCW(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
	return alg_MovieExe_OnFCW(pCtrl, paramNum, paramArray);
}

INT32 MovieExe_OnPauseResume(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
	return NVTEVT_CONSUME;
}

void FlowMovie_FileDBCreate(void)
{
#if 0//USE_FILEDB
	MEM_RANGE Pool;

	Pool.Addr = dma_getCacheAddr(OS_GetMempoolAddr(POOL_ID_FILEDB));
	Pool.Size = POOL_SIZE_FILEDB;
	ImageUnit_Begin(&ISF_CamFile, 0);
	ImageUnit_SetParam(ISF_CTRL, CAMFILE_PARAM_USE_FILEDB, (UINT32)&Pool);
	ImageUnit_End();
	// config FDB root
//#NT#2016/08/12#Hideo Lin -begin
//#NT#For small size clone movie
#if (CUSTOM_NAMING_RULE == DISABLE)
	// sample code for FileDB Naming Rule
	// disable custom naming
	ImageUnit_Begin(&ISF_CamFile, 0);
	ImageUnit_SetParam(ISF_CTRL, CAMFILE_PARAM_CUSTOM_NAMING, FALSE);
	ImageUnit_End();

	// set root folder
	ImageApp_CamFile_SetFolder(FILEDB_CARDV_ROOT);

	// set folder name of movie, RO, and photo (sample code)
#if 0
	ImageApp_CamFile_SetMovieFolder(0, "MOVIE");
	ImageApp_CamFile_SetROFolder(0, "LOCK");
	ImageApp_CamFile_SetEMRFolder(0, "EMERGENCY");
	ImageApp_CamFile_SetPhotoFolder(0, "PHOTO");
#endif

	//#NT#2016/03/07#KCHong -begin
	//#NT#Low power timelapse function
#if ((YOUKU_SDK == ENABLE) || (TIMELAPSE_LPR_FUNCTION == ENABLE))
	char RootFolder[NMC_OTHERS_MAX_LEN];
	// enable custom naming
	ImageUnit_Begin(&ISF_CamFile, 0);
	ImageUnit_SetParam(ISF_CTRL, CAMFILE_PARAM_CUSTOM_NAMING, TRUE);
	ImageUnit_End();
	// set root folder
	snprintf(RootFolder, NMC_OTHERS_MAX_LEN, "A:\\%s\\", FILEDB_CARDV_ROOT);
	ImageApp_CamFile_SetRootFolder(RootFolder);
	// set folder name of 1st movie path
	ImageApp_CamFile_SetMovieFolder(0, "MOVIE");
	//ImageApp_CamFile_SetROFolder(0, "RO");
	ImageApp_CamFile_SetPhotoFolder(0, "PHOTO");
#if (SAVE_DUAL_STREAM_FUNC == ENABLE)
	// set folder name of 2nd movie path
	ImageApp_CamFile_SetMovieFolder(1, WIFI_MOVIE_FOLDER);
#endif
#if ((SAVE_DUAL_STREAM_FUNC == ENABLE) || (TIMELAPSE_LPR_FUNCTION == ENABLE))
	// set custom file name
	//manual naming method
	ImageApp_CamFile_SetFileNamingMethod(FILE_NAMING_MANUAL);
	// register callback functions for manual file naming method
	ImageApp_CamFile_RegMovieFileNamingCB(UIMovie_MovieFileNamingCB);
	ImageApp_CamFile_RegPhotoFileNamingCB(UIMovie_PhotoFileNamingCB);
	ImageApp_CamFile_RegEMRMovFileNamingCB(UIMovie_EMRMovFileNamingCB);
#endif
#if (CUT_MOVIE_FUNC == ENABLE)
	ImageApp_CamFile_SetROFolder(1, CUT_MOVIE_FOLDER);
#endif
#endif//(YOUKU_SDK == ENABLE)
	//#NT#2016/03/07#KCHong -end
#else // sample code for Custom Naming Rule

	// enable custom naming
	ImageUnit_Begin(&ISF_CamFile, 0);
	ImageUnit_SetParam(ISF_CTRL, CAMFILE_PARAM_CUSTOM_NAMING, TRUE);
	ImageUnit_End();

#if (SMALL_CLONE_MOVIE == DISABLE)
	// set root folder
#if (FS_MULTI_STRG_FUNC == DISABLE)
	ImageApp_CamFile_SetRootFolder("A:\\CarDV\\");
#else
	ImageApp_CamFile_SetRootFolderByPath(0, "A:\\CarDV\\");
	ImageApp_CamFile_SetRootFolderByPath(1, "B:\\CarDV\\");
#endif

	// set folder name of 1st movie path
	ImageApp_CamFile_SetMovieFolder(0, "Movie_Front");
	ImageApp_CamFile_SetROFolder(0, "RO_Front");
	ImageApp_CamFile_SetEMRFolder(0, "EMR_Front");
	ImageApp_CamFile_SetPhotoFolder(0, "Photo_Front");

	// set folder name of 2nd movie path
	ImageApp_CamFile_SetMovieFolder(1, "Movie_Rear");
	ImageApp_CamFile_SetROFolder(1, "RO_Rear");
	ImageApp_CamFile_SetEMRFolder(1, "EMR_Rear");
	ImageApp_CamFile_SetPhotoFolder(1, "Photo_Rear");

	// set custom file name
#if 0 // manual naming method
	{
		ImageApp_CamFile_SetFileNamingMethod(FILE_NAMING_MANUAL);
		// register callback functions for manual file naming method
		ImageApp_CamFile_RegMovieFileNamingCB(UIMovie_MovieFileNamingCB);
		ImageApp_CamFile_RegPhotoFileNamingCB(UIMovie_PhotoFileNamingCB);
		ImageApp_CamFile_RegEMRMovFileNamingCB(UIMovie_EMRMovFileNamingCB);
	}
#elif 1 // semi-auto naming method
	{
		switch (SysGetFlag(FL_DATE_FORMAT)) {
		case DATE_FORMAT_DMY:
			ImageApp_CamFile_SetFileNamingYMD(FILE_NAMING_DATEFMT_DMY);
			break;
		case DATE_FORMAT_MDY:
			ImageApp_CamFile_SetFileNamingYMD(FILE_NAMING_DATEFMT_MDY);
			break;
		case DATE_FORMAT_YMD:
		default:
			ImageApp_CamFile_SetFileNamingYMD(FILE_NAMING_DATEFMT_YMD);
			break;
		}
		ImageApp_CamFile_SetFileNamingMethod(FILE_NAMING_SEMIAUTO);
		ImageApp_CamFile_SetFileNamingType(FILE_NAMING_SEMIAUTO_TYPE2);
		ImageApp_CamFile_SetFileNameEndChar(0, ENABLE, "_FR");
		ImageApp_CamFile_SetFileNameEndChar(1, ENABLE, "_RE");
		ImageApp_CamFile_SetFileNameSerialNum(0, DISABLE);
		ImageApp_CamFile_SetFileNameSerialNum(1, DISABLE);
		ImageApp_CamFile_SetSemiAutoFileNaming(); // activate semi-auto file naming
	}
#else // auto naming method => need to do nothing
#endif
#else // small size clone movie
	// set root folder
#if (FS_MULTI_STRG_FUNC == DISABLE)
	ImageApp_CamFile_SetRootFolder(DV_ROOT_PATH);
#else
	ImageApp_CamFile_SetRootFolderByPath(0, DV_ROOT_PATH);
	ImageApp_CamFile_SetRootFolderByPath(1, "B:\\CarDV\\");
#endif

	// set folder name of 1st movie path
	ImageApp_CamFile_SetMovieFolder(0, MOVIE_FOLDER);
	ImageApp_CamFile_SetROFolder(0, MOVIE_RO_FOLDER);
	ImageApp_CamFile_SetEMRFolder(0, MOVIE_RO_FOLDER);
	ImageApp_CamFile_SetPhotoFolder(0, PHOTO_FOLDER);

	// set folder name of 2nd movie path
	ImageApp_CamFile_SetMovieFolder(1, MOVIE_SHARE_FOLDER);
	ImageApp_CamFile_SetROFolder(1, MOVIE_SHARE_RO_FOLDER);
	ImageApp_CamFile_SetEMRFolder(1, MOVIE_SHARE_RO_FOLDER);
	ImageApp_CamFile_SetPhotoFolder(1, PHOTO_FOLDER);

	// set custom file name
	ImageApp_CamFile_SetFileNamingMethod(FILE_NAMING_MANUAL);
	// register callback functions for manual file naming method
	ImageApp_CamFile_RegMovieFileNamingCB(UIMovie_MovieFileNamingCB);
	//ImageApp_CamFile_RegEMRMovFileNamingCB(UIMovie_EMRMovieFileNamingCB);
	ImageApp_CamFile_RegPhotoFileNamingCB(UIMovie_PhotoFileNamingCB);
#endif
#endif
//#NT#2016/08/12#Hideo Lin -end
#endif
}

INT32 SetupExe_OnFILEDB_FILEID_RESET(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray);

void FlowMovie_FileDBCreate_Fast(void)
{
	if (System_GetState(SYS_STATE_POWERON) == SYSTEM_POWERON_NORMAL) {
#if 0//USE_FILEDB
		MEM_RANGE Pool;

		Pool.Addr = dma_getCacheAddr(OS_GetMempoolAddr(POOL_ID_FILEDB));
		Pool.Size = POOL_SIZE_FILEDB;
		ImageUnit_Begin(&ISF_CamFile, 0);
		ImageUnit_SetParam(ISF_CTRL, CAMFILE_PARAM_USE_FILEDB, (UINT32)&Pool);
		ImageUnit_End();
		// config FDB root
		ImageApp_CamFile_SetFolder(FILEDB_CARDV_ROOT);
		// create FDB
		ImageUnit_Begin(&ISF_CamFile, 0);
		ImageUnit_SetParam(ISF_CTRL, CAMFILE_PARAM_MOVIE_FILEDB_CRATE, 1);
		ImageUnit_End();
		// reset FDB file id
		if (UI_GetData(FL_IsUseFileDB)) {
			//Ux_SendEvent(&UISetupObjCtrl,NVTEVT_FILEID_RESET,0);
			SetupExe_OnFILEDB_FILEID_RESET(NULL, 0, NULL);
		}
#endif
	}
}
BOOL FlowMovie_CheckReOpenItem(void)
{
	BOOL bReOpen = FALSE, bMovieSizeMenu = FALSE;

	if (UI_GetData(FL_MOVIE_SIZE_MENU) != UI_GetData(FL_MOVIE_SIZE)) {
		// Set resolution on mode open to avoid wrong preview image.
		UI_SetData(FL_MOVIE_SIZE, UI_GetData(FL_MOVIE_SIZE_MENU));
		bReOpen = TRUE;
		bMovieSizeMenu = TRUE;
	}

#if (MOVIE_MAPPING_MULTIREC == ENABLE)
	// if multi recored, set EMR mode to _CFG_EMR_MODE4 (set crash)
	if (UI_GetData(FL_MOVIE_URGENT_PROTECT_AUTO) == MOVIE_URGENT_PROTECT_AUTO_ON) {
		Ux_SendEvent(&CustomMovieObjCtrl, NVTEVT_EXE_MOVIE_PROTECT_AUTO, 1, MOVIE_URGENT_PROTECT_AUTO_ON);
	}
	if (UI_GetData(FL_MOVIE_URGENT_PROTECT_MANUAL) == MOVIE_URGENT_PROTECT_MANUAL_ON) {
		Ux_SendEvent(&CustomMovieObjCtrl, NVTEVT_EXE_MOVIE_PROTECT_MANUAL, 1, MOVIE_URGENT_PROTECT_MANUAL_ON);
	}
#endif

	if (UI_GetData(FL_MOVIE_HDR_MENU) != UI_GetData(FL_MOVIE_HDR)) {
		if (UI_GetData(FL_MOVIE_HDR_MENU) == MOVIE_HDR_ON) {
			UI_SetData(FL_MOVIE_WDR, MOVIE_WDR_OFF);
			UI_SetData(FL_MOVIE_WDR_MENU, MOVIE_WDR_OFF);
			UI_SetData(FL_MovieRSCIndex, MOVIE_RSC_OFF);
			UI_SetData(FL_MovieRSCIndex_MENU, MOVIE_RSC_OFF);
		}
		UI_SetData(FL_MOVIE_HDR, UI_GetData(FL_MOVIE_HDR_MENU));
		bReOpen = TRUE;
	}

	if (UI_GetData(FL_MOVIE_WDR_MENU) != UI_GetData(FL_MOVIE_WDR)) {

		if (UI_GetData(FL_MOVIE_WDR_MENU) == MOVIE_WDR_ON) {
			UI_SetData(FL_MOVIE_HDR, MOVIE_HDR_OFF);
			UI_SetData(FL_MOVIE_HDR_MENU, MOVIE_HDR_OFF);
		}
		UI_SetData(FL_MOVIE_WDR, UI_GetData(FL_MOVIE_WDR_MENU));
		bReOpen = TRUE;
	}

	if (UI_GetData(FL_MovieRSCIndex_MENU) != UI_GetData(FL_MovieRSCIndex)) {

		if (UI_GetData(FL_MovieRSCIndex_MENU) == MOVIE_RSC_ON) {
			UI_SetData(FL_MOVIE_HDR, MOVIE_HDR_OFF);
			UI_SetData(FL_MOVIE_HDR_MENU, MOVIE_HDR_OFF);
		}
		UI_SetData(FL_MovieRSCIndex, UI_GetData(FL_MovieRSCIndex_MENU));
		bReOpen = TRUE;
	}

#if !defined(_SENSORLIB2_OFF_)
	if ((bMovieSizeMenu) && (System_GetState(SYS_STATE_CURRMODE) == PRIMARY_MODE_MOVIE)) {
		switch (UI_GetData(FL_MOVIE_SIZE_MENU)) {
		case MOVIE_SIZE_FRONT_2880x2160P50:
		case MOVIE_SIZE_FRONT_3840x2160P30:
		case MOVIE_SIZE_FRONT_2880x2160P24:
		case MOVIE_SIZE_FRONT_2704x2032P60:
		case MOVIE_SIZE_FRONT_2560x1440P80:
		case MOVIE_SIZE_FRONT_2560x1440P60:
		case MOVIE_SIZE_FRONT_2560x1440P30:
		case MOVIE_SIZE_FRONT_1920x1080P120:
		case MOVIE_SIZE_FRONT_1920x1080P60:
		case MOVIE_SIZE_FRONT_1280x720P240:
		case MOVIE_SIZE_FRONT_1280x720P120:
		case MOVIE_SIZE_FRONT_1280x720P60:
		case MOVIE_SIZE_FRONT_640x480P240:
			//#NT#2016/06/17#Brain Yen -begin
			//#NT#for special aspect panel and Pipview LR
#if (PIP_VIEW_LR == ENABLE)
			UI_SetData(FL_DUAL_CAM_MENU, DUALCAM_LR_FRONT_FULL);
#else
			UI_SetData(FL_DUAL_CAM_MENU, DUALCAM_FRONT);
#endif
			//#NT#2016/06/17#Brain Yen -end
			break;

		default:
			//#NT#2016/06/17#Brain Yen -begin
			//#NT#for special aspect panel and Pipview LR
#if (PIP_VIEW_LR == ENABLE)
			if (System_GetEnableSensor() & SENSOR_2) {
				UI_SetData(FL_DUAL_CAM_MENU, DUALCAM_LR_16_9);
			} else {
				UI_SetData(FL_DUAL_CAM_MENU, DUALCAM_LR_FRONT_FULL);
			}
#else
			if (System_GetEnableSensor() & SENSOR_2) {
				UI_SetData(FL_DUAL_CAM_MENU, DUALCAM_BOTH);
			} else {
				UI_SetData(FL_DUAL_CAM_MENU, DUALCAM_FRONT);
			}
#endif
			//#NT#2016/06/17#Brain Yen -end
			break;
		}
	}
#endif

	if (UI_GetData(FL_MOVIE_LDWS) != UI_GetData(FL_MOVIE_LDWS_MENU)) {
		UI_SetData(FL_MOVIE_LDWS, UI_GetData(FL_MOVIE_LDWS_MENU));
		bReOpen = TRUE;
	}

	if (UI_GetData(FL_MOVIE_FCW) != UI_GetData(FL_MOVIE_FCW_MENU)) {
		UI_SetData(FL_MOVIE_FCW, UI_GetData(FL_MOVIE_FCW_MENU));
		bReOpen = TRUE;
	}

	if (UI_GetData(FL_MOVIE_TIMELAPSE_REC) != UI_GetData(FL_MOVIE_TIMELAPSE_REC_MENU)) {
		if ((UI_GetData(FL_MOVIE_TIMELAPSE_REC) == MOVIE_TIMELAPSEREC_OFF) &&
			(UI_GetData(FL_MOVIE_LDWS) == MOVIE_LDWS_ON || UI_GetData(FL_MOVIE_FCW) == MOVIE_FCW_ON)) {
			bReOpen = TRUE;
		}
		UI_SetData(FL_MOVIE_TIMELAPSE_REC, UI_GetData(FL_MOVIE_TIMELAPSE_REC_MENU));

		if (UI_GetData(FL_MOVIE_TIMELAPSE_REC) != MOVIE_TIMELAPSEREC_OFF) {
			if (UI_GetData(FL_MOVIE_URGENT_PROTECT_AUTO) == MOVIE_URGENT_PROTECT_AUTO_ON || UI_GetData(FL_MOVIE_URGENT_PROTECT_MANUAL) == MOVIE_URGENT_PROTECT_MANUAL_ON) {
				gMovie_Rec_Option.emr_on = FALSE;
				ImageApp_Movie_SetParam(_CFG_REC_ID_1, MOVIE_PARAM_FILE_EMRON, gMovie_Rec_Option.emr_on);
				UI_SetData(FL_MOVIE_URGENT_PROTECT_AUTO, MOVIE_URGENT_PROTECT_AUTO_OFF);
				UI_SetData(FL_MOVIE_URGENT_PROTECT_MANUAL, MOVIE_URGENT_PROTECT_MANUAL_OFF);
			}
		}
	}

	if (UI_GetData(FL_DUAL_CAM_MENU) != UI_GetData(FL_DUAL_CAM)) {
		if (System_GetState(SYS_STATE_CURRMODE) == PRIMARY_MODE_MOVIE) {
			if (bMovieSizeMenu == FALSE) {
				switch (UI_GetData(FL_MOVIE_SIZE_MENU)) {
				case MOVIE_SIZE_FRONT_2880x2160P50:
				case MOVIE_SIZE_FRONT_3840x2160P30:
				case MOVIE_SIZE_FRONT_2880x2160P24:
				case MOVIE_SIZE_FRONT_2704x2032P60:
				case MOVIE_SIZE_FRONT_2560x1440P80:
				case MOVIE_SIZE_FRONT_2560x1440P60:
				case MOVIE_SIZE_FRONT_2560x1440P30:
				case MOVIE_SIZE_FRONT_1920x1080P120:
				case MOVIE_SIZE_FRONT_1920x1080P60:
				case MOVIE_SIZE_FRONT_1280x720P240:
				case MOVIE_SIZE_FRONT_1280x720P120:
				case MOVIE_SIZE_FRONT_1280x720P60:
				case MOVIE_SIZE_FRONT_640x480P240:
					break;

				default:
					Ux_PostEvent(NVTEVT_EXE_DUALCAM, 1, UI_GetData(FL_DUAL_CAM_MENU));
					break;
				}
			} else {
				UI_SetData(FL_DUAL_CAM, UI_GetData(FL_DUAL_CAM_MENU));
#if _TODO
				PipView_SetStyle(UI_GetData(FL_DUAL_CAM));
#endif
			}
		} else {
			Ux_PostEvent(NVTEVT_EXE_DUALCAM, 1, UI_GetData(FL_DUAL_CAM_MENU));
		}

#if _TODO//(PIP_VIEW_FASTSWITCH == DISABLE) //when PIP_VIEW_FASTSWITCH enabled, NVTEVT_EXE_DUALCAM will not ReOpen inside.
		bReOpen = FALSE; // because in NVTEVT_EXE_DUALCAM, NVTEVT_EXE_MOVIE_DUALCAM will invoke ReOpen
#endif
	}

	return bReOpen;
}

//#NT#2016/06/08#Lincy Lin -begin
//#NT#Implement generic OSD and video drawing mechanism for ALG function

INT32 MovieExe_OnALGEnd(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
	//Flush ALG event before draw
	Ux_FlushEventByRange(NVTEVT_EXE_MOVIE_ALGEND, NVTEVT_EXE_MOVIE_ALGEND);
	MovieExe_CallBackUpdateInfo(UIAPPPHOTO_CB_ALGEND);
	return NVTEVT_CONSUME;
}
//#NT#2016/06/08#Lincy Lin -end


INT32 MovieExe_OnADASSetCropWin(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
	URECT CropWin;

	CropWin.x = (paramArray[0] & 0xffff0000) >> 16;
	CropWin.y = (paramArray[0] & 0x0000ffff);
	CropWin.w = (paramArray[1] & 0xffff0000) >> 16;
	CropWin.h = (paramArray[1] & 0x0000ffff);

	//DBG_DUMP("UI: Set crop = %d/%d/%d/%d\r\n", CropWin.x, CropWin.y, CropWin.w, CropWin.h);
	ImageUnit_Begin(ISF_IPL(0), 0);
		ImageUnit_SetVdoPostWindow(ISF_OUT4, CropWin.x, CropWin.y, CropWin.w, CropWin.h);
	ImageUnit_End();
	ImageStream_UpdateAll(&ISF_Stream[0]);
	return NVTEVT_CONSUME;
}

INT32 MovieExe_OnMotionDetRun(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
	UINT32 ret;

	ret = Movie_BCGetBlkTarget(NULL, NULL, 100);
	*(UINT32 *)(paramArray[0]) = (ret > 20) ? TRUE : FALSE;

	return NVTEVT_CONSUME;
}

#if MOVIE_MAPPING_MULTIREC
INT32 MovieExe_OnCodec(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
	UINT32 uiSelect = 0;
    UINT32 uiValue = 0;
	UINT32 i, movie_rec_mask, clone_rec_mask, mask = 1;

	if (paramNum > 0) {
		uiSelect = paramArray[0];
	}

	DBG_IND("uiSelect %d\r\n", uiSelect);
	UI_SetData(FL_MOVIE_CODEC, uiSelect);

    switch(uiSelect){
        case MOVIE_CODEC_H264:
            uiValue = _CFG_CODEC_H264;
        break;
        case MOVIE_CODEC_H265:
            uiValue = _CFG_CODEC_H265;
        break;
        case MOVIE_CODEC_MJPG:
            uiValue = _CFG_CODEC_MJPG;
        break;
        default:
            DBG_ERR("not support\r\n");
    }

	movie_rec_mask = Movie_GetMovieRecMask();
	clone_rec_mask = Movie_GetCloneRecMask();

	for (i = 0; i < SENSOR_CAPS_COUNT; i++) {
		if (movie_rec_mask & mask) {
			MovieExe_SetVdoCodecByRecID(gMovie_Rec_Info[i].rec_id, uiValue);
		}

		if (clone_rec_mask & mask) {
			MovieExe_SetVdoCodecByRecID(gMovie_Clone_Info[i].rec_id, uiValue);
		}

		mask <<= 1;
	}

	return NVTEVT_CONSUME;
}
#else
INT32 MovieExe_OnCodec(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
	UINT32 uiSelect = 0;
    UINT32 uiValue = 0;
	UINT32 pathID = 0;
	if (paramNum > 0) {
		uiSelect = paramArray[0];
	}

	DBG_IND("uiSelect %d\r\n", uiSelect);
	UI_SetData(FL_MOVIE_CODEC, uiSelect);

    switch(uiSelect){
        case MOVIE_CODEC_H264:
            uiValue = _CFG_CODEC_H264;
        break;
        case MOVIE_CODEC_H265:
            uiValue = _CFG_CODEC_H265;
        break;
        case MOVIE_CODEC_MJPG:
            uiValue = _CFG_CODEC_MJPG;
        break;
        default:
            DBG_ERR("not support\r\n");
    }

    //set record format
	if (System_GetState(SYS_STATE_CURRMODE) == PRIMARY_MODE_MOVIE) {
		ImageApp_Movie_SetParam(_CFG_REC_ID_1, MOVIE_PARAM_CODEC, uiValue);
		ImageApp_Movie_SetParam(_CFG_REC_ID_1, MOVIE_PARAM_PROFILE, (uiValue == _CFG_CODEC_H264) ? _CFG_PROFILE_HIGH : _CFG_PROFILE_MAIN);
	}

	pathID = (UINT32)ImageApp_Movie_GetVdoEncPort(gMovie_Rec_Info[0].rec_id);
	if (uiValue == _CFG_CODEC_MJPG) {
#if (VIDEO_FUNC_MJPG == ENABLE)
		MP_VDOENC_ENCODER *pObj = MP_MjpgEnc_getVideoObject((MP_VDOENC_ID)pathID);
		ImageApp_Movie_SetParam(gMovie_Rec_Info[0].rec_id, MOVIE_PARAM_VDO_ENCODER_OBJ, (UINT32)pObj);
#else
		ImageApp_Movie_SetParam(gMovie_Rec_Info[0].rec_id, MOVIE_PARAM_VDO_ENCODER_OBJ, 0);
		DBG_ERR("[MOVIE][%d] codec = %d not support\r\n", pathID, uiValue);
#endif
	}
	if (uiValue == _CFG_CODEC_H264) {
#if (VIDEO_FUNC_H264 == ENABLE)
		MP_VDOENC_ENCODER *pObj = MP_H264Enc_getVideoObject((MP_VDOENC_ID)pathID);
		ImageApp_Movie_SetParam(gMovie_Rec_Info[0].rec_id, MOVIE_PARAM_VDO_ENCODER_OBJ, (UINT32)pObj);
#else
		ImageApp_Movie_SetParam(gMovie_Rec_Info[0].rec_id, MOVIE_PARAM_VDO_ENCODER_OBJ, 0);
		DBG_ERR("[MOVIE][%d] codec = %d not support\r\n", pathID, uiValue);
#endif
	}
	if (uiValue == _CFG_CODEC_H265) {
#if (VIDEO_FUNC_H265 == ENABLE)
		MP_VDOENC_ENCODER *pObj = MP_H265Enc_getVideoObject((MP_VDOENC_ID)pathID);
		ImageApp_Movie_SetParam(gMovie_Rec_Info[0].rec_id, MOVIE_PARAM_VDO_ENCODER_OBJ, (UINT32)pObj);
#else
		ImageApp_Movie_SetParam(gMovie_Rec_Info[0].rec_id, MOVIE_PARAM_VDO_ENCODER_OBJ, 0);
		DBG_ERR("[MOVIE][%d] codec = %d not support\r\n", pathID, uiValue);
#endif
	}

#if MOVIE_DUAL_RECORD_TEST
	pathID = (UINT32)ImageApp_Movie_GetVdoEncPort(gMovie_Rec_Info2.rec_id);
	if (uiValue == _CFG_CODEC_MJPG) {
#if (VIDEO_FUNC_MJPG == ENABLE)
		MP_VDOENC_ENCODER *pObj = MP_MjpgEnc_getVideoObject((MP_VDOENC_ID)pathID);
		ImageApp_Movie_SetParam(gMovie_Rec_Info2.rec_id, MOVIE_PARAM_VDO_ENCODER_OBJ, (UINT32)pObj);
#else
		ImageApp_Movie_SetParam(gMovie_Rec_Info2.rec_id, MOVIE_PARAM_VDO_ENCODER_OBJ, 0);
		DBG_ERR("[MOVIE2][%d] codec = %d not support\r\n", pathID, uiValue);
#endif
	}
	if (uiValue == _CFG_CODEC_H264) {
#if (VIDEO_FUNC_H264 == ENABLE)
		MP_VDOENC_ENCODER *pObj = MP_H264Enc_getVideoObject((MP_VDOENC_ID)pathID);
		ImageApp_Movie_SetParam(gMovie_Rec_Info2.rec_id, MOVIE_PARAM_VDO_ENCODER_OBJ, (UINT32)pObj);
#else
		ImageApp_Movie_SetParam(gMovie_Rec_Info2.rec_id, MOVIE_PARAM_VDO_ENCODER_OBJ, 0);
		DBG_ERR("[MOVIE2][%d] codec = %d not support\r\n", pathID, uiValue);
#endif
	}
	if (uiValue == _CFG_CODEC_H265) {
#if (VIDEO_FUNC_H265 == ENABLE)
		MP_VDOENC_ENCODER *pObj = MP_H265Enc_getVideoObject((MP_VDOENC_ID)pathID);
		ImageApp_Movie_SetParam(gMovie_Rec_Info2.rec_id, MOVIE_PARAM_VDO_ENCODER_OBJ, (UINT32)pObj);
#else
		ImageApp_Movie_SetParam(gMovie_Rec_Info2.rec_id, MOVIE_PARAM_VDO_ENCODER_OBJ, 0);
		DBG_ERR("[MOVIE2][%d] codec = %d not support\r\n", pathID, uiValue);
#endif
	}
#endif

	return NVTEVT_CONSUME;
}
#endif

////////////////////////////////////////////////////////////

EVENT_ENTRY CustomMovieObjCmdMap[] = {
	{NVTEVT_EXE_OPEN,                 MovieExe_OnOpen                     },
	{NVTEVT_EXE_CLOSE,                MovieExe_OnClose                    },
	{NVTEVT_EXE_MOVIESIZE,            MovieExe_OnMovieSize                },
	{NVTEVT_EXE_IMAGE_RATIO,          MovieExe_OnImageRatio               },
	{NVTEVT_EXE_MOVIEQUALITY,         MovieExe_OnMovieQuality             },
	{NVTEVT_EXE_WB,                   MovieExe_OnWB                       },
	{NVTEVT_EXE_MOVIECOLOR,           MovieExe_OnColor                    },
	{NVTEVT_EXE_MOVIE_EV,             MovieExe_OnEV                       },
	{NVTEVT_EXE_MOVIE_AUDIO,          MovieExe_OnMovieAudio               },
	{NVTEVT_EXE_MOVIE_AUDIO_VOL,      MovieExe_OnMovieAudioRecVolume      },
	{NVTEVT_EXE_MOVIE_AUDIO_REC,      MovieExe_OnMovieAudioRec            },
	{NVTEVT_EXE_MOVIEDZOOM,           MovieExe_OnDigitalZoom              },
	{NVTEVT_EXE_MOVIEDIS,             MovieExe_OnDis                      },
	{NVTEVT_EXE_MOVIE_MCTF,           MovieExe_OnMCTF                     },
	{NVTEVT_EXE_MOVIE_EDGE,           MovieExe_OnEdge                     },
	{NVTEVT_EXE_MOVIE_NR,             MovieExe_OnNR                       },
	{NVTEVT_EXE_MOVIE_RSC,            MovieExe_OnRSC                      },
	{NVTEVT_EXE_MOVIE_WDR,            MovieExe_OnWDR                      },
	{NVTEVT_EXE_SHDR,                 MovieExe_OnSHDR                     },
	{NVTEVT_EXE_MOVIE_SENSOR_ROTATE,  MovieExe_OnSensorRotate             },
	{NVTEVT_EXE_MOVIE_IR_CUT,         MovieExe_OnIRCut                    },
	{NVTEVT_EXE_MOVIEGDC,             MovieExe_OnGdc                      },
	{NVTEVT_EXE_MOVIESMEAR,           MovieExe_OnSmear                    },
	{NVTEVT_EXE_MOVIE_DATE_IMPRINT,   MovieExe_OnDateImprint              },
	{NVTEVT_EXE_GSENSOR,              MovieExe_OnGSENSOR                  },
	{NVTEVT_EXE_CYCLIC_REC,           MovieExe_OnCyclicRec                },
	{NVTEVT_EXE_MOTION_DET,           MovieExe_OnMotionDet                },
	{NVTEVT_CALLBACK,                 MovieExe_OnCallback                 },
	//#NT#2012/07/31#Hideo Lin -begin
	//#NT#For custom movie record/stop event
	{NVTEVT_EXE_MOVIE_REC_START,      MovieExe_OnRecStart                 },
	{NVTEVT_EXE_MOVIE_REC_STEP,       MovieExe_OnRecStep                  },
	{NVTEVT_EXE_MOVIE_REC_STOP,       MovieExe_OnRecStop                  },
	{NVTEVT_EXE_MOVIE_REC_PIM,        MovieExe_OnRecPIM                   },
	{NVTEVT_EXE_MOVIE_REC_RAWENC,     MovieExe_OnRecRawEnc                },
	//#NT#2012/07/31#Hideo Lin -end
	{NVTEVT_VIDEO_CHANGE,             MovieExe_OnVideoChange              },
	{NVTEVT_EXE_DUALCAM,              MovieExe_OnDualcam                  },
	{NVTEVT_EXE_MOVIE_PROTECT_AUTO,   MovieExe_OnProtectAuto              },
	{NVTEVT_EXE_MOVIE_PROTECT_MANUAL, MovieExe_OnProtectManual           },

	{NVTEVT_EXE_MOVIE_LDWS,           MovieExe_OnLDWS                     },
	{NVTEVT_EXE_MOVIE_FCW,            MovieExe_OnFCW                      },
	{NVTEVT_EXE_VIDEO_PAUSE_RESUME,   MovieExe_OnPauseResume              },
	//#NT#2016/03/25#KCHong -begin
	//#NT#New ADAS
	{NVTEVT_CB_ADAS_SETCROPWIN,       MovieExe_OnADASSetCropWin           },
	//#NT#2016/03/25#KCHong -end
	{NVTEVT_EXE_MOVIE_ALGEND,         MovieExe_OnALGEnd                    },
	//#NT#2016/06/03#Charlie Chang -begin
	//#NT# support contrast, audioIn, flip, quality set
	{NVTEVT_EXE_MOVIE_CONTRAST,       MovieExe_OnContrast                 },
	{NVTEVT_EXE_MOVIE_AUDIOIN,        MovieExe_OnAudioIn                  },
	{NVTEVT_EXE_MOVIE_AUDIOIN_SR,     MovieExe_OnAudioInSR                },
	{NVTEVT_EXE_MOVIE_FLIP_MIRROR,    MovieExe_OnFipMirror                },
	{NVTEVT_EXE_MOVIE_QUALITY_SET,    MovieExe_OnQualitySet               },
	//#NT#2016/06/03#Charlie Chang -end
	{NVTEVT_EXE_MOTION_DET_RUN,       MovieExe_OnMotionDetRun             },
	{NVTEVT_EXE_MOVIE_STRM_START,     MovieExe_OnStrmStart                },
	{NVTEVT_EXE_MOVIE_STRM_STOP,      MovieExe_OnStrmStop                 },
	{NVTEVT_EXE_MOVIE_CODEC,          MovieExe_OnCodec                 },
	{NVTEVT_NULL,                     0},  //End of Command Map
};

CREATE_APP(CustomMovieObj, APP_SETUP)

