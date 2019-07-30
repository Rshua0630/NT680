
#include "AppControl.h"
#include "SysCommon.h"
#include "UIFramework.h"
#include "PrjCfg.h"
#include "uToken.h"
#include "UIInfo.h"
#include "UCtrlAppImage.h"
#include "NvtUctrlAPI.h"
#include "NvtIpcAPI.h"
#include "UCtrlMain.h"
#include "UISetup.h"
#include "ipl_utility.h"
#include "ipl_alg_infor.h"
#include "awb_task.h"
#include "awb_api.h"
#include "ae_task.h"
#include "ae_api.h"
#include "iq_api.h"
#include "iq_ui_info.h"
#include "ae_ui_info.h"
#include "awb_ui_info.h"
#include "UIAppMovie.h"
#if(PHOTO_MODE==ENABLE)
#include "UIAppPhoto.h"
#endif
#include "VideoCodec_H264.h"
#include "UIAppMovie_Param.h"
#if (IPCAM_FUNC==ENABLE)
#include "ImageApp_IPCam.h"
#include "UIAppIPCam_Param.h"
#include "alg_IPCam_PhotoFuncInfo.h"
#endif
#define THIS_DBGLVL         6 // 0=FATAL, 1=ERR, 2=WRN, 3=UNIT, 4=FUNC, 5=IND, 6=MSG, 7=VALUE, 8=USER
///////////////////////////////////////////////////////////////////////////////
#define __MODULE__          UCtrlAppImage
#define __DBGLVL__          ((THIS_DBGLVL>=PRJ_DBG_LVL)?THIS_DBGLVL:PRJ_DBG_LVL)
#define __DBGFLT__          "*" //*=All, [mark]=CustomClass
#include "DebugModule.h"

#define SXCMD_TEST          0


// flag define
#define FLG_UCTRL_IMAGE_START           FLGPTN_BIT(0)
#define FLG_UCTRL_IMAGE_EXIT            FLGPTN_BIT(1)

static UCtrlImageInfoType   imageInfo;
static UCtrlImageInfoType  *pImageInfo = &imageInfo;


static INT32 param_ipl_id_set(INT32 argc, char *argv[])
{
	UINT32 value;
	if (argc != 1) {
		DBG_ERR("argc!=1 (%d)\r\n", argc);
		return -1;
	}
	value = atoi(argv[0]);
	if (value >= IPL_ID_MAX_NUM) {
		DBG_ERR("value =%d\r\n", value);
		return -1;
	}

	pImageInfo->ipl_id.value = value;
	pImageInfo->ipl_id.isSet = 1;

	return 0;
}

static INT32 param_backlight(INT32 argc, char *argv[])
{
	if (argc != 2) {
		DBG_ERR("argc!=2 (%d)\r\n", argc);
		return -1;
	}
	pImageInfo->backlight.isSet = 1;
	pImageInfo->backlight.mode = atoi(argv[0]);
	pImageInfo->backlight.lv   = atoi(argv[1]);
	return 0;
}

static INT32 param_brightness(INT32 argc, char *argv[])
{
	if (argc != 1) {
		DBG_ERR("argc!=1 (%d)\r\n", argc);
		return -1;
	}

	pImageInfo->brightness.isSet = 1;
	pImageInfo->brightness.value = atoi(argv[0]);
	return 0;
}

static INT32 param_saturation(INT32 argc, char *argv[])
{
	if (argc != 1) {
		DBG_ERR("argc!=1 (%d)\r\n", argc);
		return -1;
	}
	pImageInfo->saturation.isSet = 1;
	pImageInfo->saturation.value = atoi(argv[0]);
	return 0;
}

static INT32 param_contrast(INT32 argc, char *argv[])
{
	if (argc != 1) {
		DBG_ERR("argc!=1 (%d)\r\n", argc);
		return -1;
	}
	pImageInfo->contrast.isSet = 1;
	pImageInfo->contrast.value = atoi(argv[0]);
	return 0;
}

static INT32 param_ae(INT32 argc, char *argv[])
{
	if (argc != 11) {
		DBG_ERR("argc!=11 (%d)\r\n", argc);
		return -1;
	}
	pImageInfo->ae.isSet = 1;
	pImageInfo->ae.mode    = atoi(argv[0]);
	pImageInfo->ae.priority = atoi(argv[1]);
	pImageInfo->ae.maxexp  = atoi(argv[2]);
	pImageInfo->ae.minexp  = atoi(argv[3]);
	pImageInfo->ae.maxgain = atoi(argv[4]);
	pImageInfo->ae.mingain = atoi(argv[5]);
	pImageInfo->ae.maxiris = atoi(argv[6]);
	pImageInfo->ae.miniris = atoi(argv[7]);
	pImageInfo->ae.exp     = atoi(argv[8]);
	pImageInfo->ae.gain    = atoi(argv[9]);
	pImageInfo->ae.iris    = atoi(argv[10]);
	return 0;
}

static INT32 param_aewin(INT32 argc, char *argv[])
{
	if (argc != 4) {
		DBG_ERR("argc!=4 (%d)\r\n", argc);
		return -1;
	}
	pImageInfo->aewin.isSet = 1;
	pImageInfo->aewin.left  = atoi(argv[0]);
	pImageInfo->aewin.right = atoi(argv[1]);
	pImageInfo->aewin.top   = atoi(argv[2]);
	pImageInfo->aewin.bottom = atoi(argv[3]);
	return 0;
}

static INT32 param_ircut(INT32 argc, char *argv[])
{
	if (argc != 1) {
		DBG_ERR("argc!=1 (%d)\r\n", argc);
		return -1;
	}
	pImageInfo->ircut.isSet = 1;
	pImageInfo->ircut.value  = atoi(argv[0]);
	return 0;
}

static INT32 param_wdr(INT32 argc, char *argv[])
{
	if (argc != 2) {
		DBG_ERR("argc!=2 (%d)\r\n", argc);
		return -1;
	}
	pImageInfo->wdr.isSet = 1;
	pImageInfo->wdr.mode  = atoi(argv[0]);
	pImageInfo->wdr.lv    = atoi(argv[1]);
	return 0;
}

static INT32 param_awb(INT32 argc, char *argv[])
{
	if (argc != 3) {
		DBG_ERR("argc!=3 (%d)\r\n", argc);
		return -1;
	}
	pImageInfo->awb.isSet = 1;
	pImageInfo->awb.mode  = atoi(argv[0]);
	pImageInfo->awb.rgain = atoi(argv[1]);
	pImageInfo->awb.bgain = atoi(argv[2]);
	return 0;
}

static INT32 param_dis(INT32 argc, char *argv[])
{
	if (argc != 2) {
		DBG_ERR("argc!=2 (%d)\r\n", argc);
		return -1;
	}
	pImageInfo->dis.isSet = 1;
	pImageInfo->dis.mode  = atoi(argv[0]);
//	pImageInfo->dis.lv    = atoi(argv[1]);
	return 0;
}


static INT32 param_sharpness(INT32 argc, char *argv[])
{
	if (argc != 1) {
		DBG_ERR("argc!=1 (%d)\r\n", argc);
		return -1;
	}
	pImageInfo->sharpness.isSet = 1;
	pImageInfo->sharpness.value  = atoi(argv[0]);
	return 0;
}

static INT32 param_image_setting_get(INT32 argc, char *argv[])
{
	UINT8 ipl_id;
	INT32 ipl_phy_id;
	ISPImageSetting sImageSetting = {0};
	IQ_LINUX_SET_INFO iq_linux_set_info = {0};

	if (argc != 1) {
		DBG_ERR("argc!=1 (%d)\r\n", argc);
		return -1;
	}

	if ((ipl_id = atoi(argv[0])) >= IPL_ID_MAX_NUM) {
		DBG_ERR("invalid IPL ID: %u (Max: %d)\r\n", ipl_id, IPL_ID_MAX_NUM);
		return -1;
	}
	ipl_phy_id = IPL_PATH(ipl_id);
	if (ipl_phy_id < 0)
		DBG_ERR("ipl_id=%d, phy_id=%d\r\n", ipl_id,ipl_phy_id);

	iq_get_linux_iq_info(ipl_phy_id, &iq_linux_set_info);
	sImageSetting.uiBrightness = iq_linux_set_info.bri_ratio;
	sImageSetting.uiSaturation = iq_linux_set_info.sat_ratio;
	sImageSetting.uiContrast = iq_linux_set_info.con_ratio;
	sImageSetting.uiSharpness = iq_linux_set_info.sharp_ratio;
	sImageSetting.uiHue = iq_linux_set_info.hue_angle;
	//#NT#2016/07/21#Charlie Chang -begin
	//#NT# adding aspect ratio
	//#if (IPCAM_FUNC==ENABLE)
	//sImageSetting.aspect_ratio = ImageApp_IPCam_GetConfig(IPCAM_CFG_RATIO);
	// #endif
	//#NT#2016/07/21#Charlie Chang -end
	switch (iq_get_ui_info(ipl_phy_id, IQ_UI_IMAGEEFFECT)) {
	case IQ_IMAGEEFFECT_BW:
		sImageSetting.eColor = ISP_COLOR_TYPE_GRAY;
		break;
	case IQ_IMAGEEFFECT_OFF:
	default:
		sImageSetting.eColor = ISP_COLOR_TYPE_NORMAL;
		break;
	}

	UINT32                       rotate_dir = 0;
	ImageUnit_GetVdoDirection(ISF_IPL(ipl_id), ISF_IN1, &rotate_dir);

	switch (rotate_dir) {

	//#NT#2016/03/02#David Tsai -begin
	//#NT#Replace sensor mirror/flip with DSP mirror/flip
	case (ISF_VDO_DIR_MIRRORX|ISF_VDO_DIR_MIRRORY):
		sImageSetting.bMirror = NVT_TRUE;
		sImageSetting.bFlip = NVT_TRUE;
		break;
	case ISF_VDO_DIR_MIRRORX:
		sImageSetting.bMirror = NVT_TRUE;
		sImageSetting.bFlip = NVT_FALSE;
		break;
	case ISF_VDO_DIR_MIRRORY:
		sImageSetting.bMirror = NVT_FALSE;
		sImageSetting.bFlip = NVT_TRUE;
		break;
	case ISF_VDO_DIR_NONE:
	default:
		sImageSetting.bMirror = NVT_FALSE;
		sImageSetting.bFlip = NVT_FALSE;
		break;
		//#NT#2016/03/02#David Tsai -end

	}

	//#NT#2016/07/28#David Tsai -begin
	//#NT#Add IR cut control in image setting
	switch (iq_get_ui_info(ipl_phy_id, IQ_UI_IRCUT)) {
	case IQ_IRCUT_NIGHT:
		sImageSetting.eIRCutMode = ISP_IRCUT_TYPE_NIGHT;
		break;
	case IQ_IRCUT_DAY:
		sImageSetting.eIRCutMode = ISP_IRCUT_TYPE_DAY;
		break;
	case IQ_IRCUT_AUTO:
	default:
		sImageSetting.eIRCutMode = ISP_IRCUT_TYPE_AUTO;
		break;
	}
	//#NT#2016/07/28#David Tsai -end

	NvtUctrl_SetConfigData((void *)&sImageSetting, sizeof(ISPImageSetting));

	return 0;
}

static INT32 param_image_setting_set(INT32 argc, char *argv[])
{
	UINT8 ipl_id;
	INT32 ipl_phy_id;
	ISPImageSetting sImageSetting = {0};

	if (argc != 1) {
		DBG_ERR("argc!=1 (%d)\r\n", argc);
		return -1;
	}

	if ((ipl_id = atoi(argv[0])) >= IPL_ID_MAX_NUM) {
		DBG_ERR("invalid IPL ID: %u (Max: %d)\r\n", ipl_id, IPL_ID_MAX_NUM);
		return -1;
	}
	ipl_phy_id = IPL_PATH(ipl_id);
	if (ipl_phy_id < 0)
		DBG_ERR("ipl_id=%d, phy_id=%d\r\n", ipl_id,ipl_phy_id);

	NvtUctrl_GetConfigData((void *)&sImageSetting, sizeof(ISPImageSetting));
	// IPL channel
	pImageInfo->ipl_id.isSet = 1;
	pImageInfo->ipl_id.value = ipl_id;

	// Brightness (0~100)
	pImageInfo->brightness.isSet = 1;
	pImageInfo->brightness.value = sImageSetting.uiBrightness;

	// Contrast (0~100)
	pImageInfo->contrast.isSet = 1;
	pImageInfo->contrast.value = sImageSetting.uiContrast;

	// Saturation (0~100)
	pImageInfo->saturation.isSet = 1;
	pImageInfo->saturation.value = sImageSetting.uiSaturation;

	// Hue (0~360)
	pImageInfo->hue.isSet = 1;
	pImageInfo->hue.value = sImageSetting.uiHue;

	// Sharpness (0~100)
	pImageInfo->sharpness.isSet = 1;
	pImageInfo->sharpness.value = sImageSetting.uiSharpness;

	if (ISP_COLOR_TYPE_NORMAL == sImageSetting.eColor) {
		iq_set_ui_info(ipl_phy_id, IQ_UI_IMAGEEFFECT, IQ_IMAGEEFFECT_OFF);
	} else {
		iq_set_ui_info(ipl_phy_id, IQ_UI_IMAGEEFFECT, IQ_IMAGEEFFECT_BW);
	}

#if(IPCAM_FUNC == ENABLE)
	UINT32   rotate_dir;
	ImageUnit_GetVdoDirection(ISF_IPL(ipl_id), ISF_IN1, &rotate_dir);
	UINT32 rotate_set=0;


	ImageUnit_Begin(ISF_IPL(ipl_id), 0);
	if(sImageSetting.bMirror == NVT_TRUE && sImageSetting.bFlip == NVT_TRUE){
		if(rotate_dir != (ISF_VDO_DIR_MIRRORX|ISF_VDO_DIR_MIRRORY)){
			ImageUnit_SetVdoDirection(ISF_IN1, ISF_VDO_DIR_MIRRORX|ISF_VDO_DIR_MIRRORY);
			rotate_set = 1;
		}
	}
	else if(sImageSetting.bMirror){
		if(rotate_dir != ISF_VDO_DIR_MIRRORX){
			ImageUnit_SetVdoDirection(ISF_IN1, ISF_VDO_DIR_MIRRORX);
			rotate_set = 1;
		}
	}
	else if(sImageSetting.bFlip){
		if(rotate_dir != ISF_VDO_DIR_MIRRORY){
			ImageUnit_SetVdoDirection(ISF_IN1, ISF_VDO_DIR_MIRRORY);
			rotate_set = 1;
		}
	}
	else if(sImageSetting.bMirror == NVT_FALSE && sImageSetting.bFlip == NVT_FALSE){

		if(rotate_dir != ISF_VDO_DIR_NONE){
			ImageUnit_SetVdoDirection(ISF_IN1, ISF_VDO_DIR_NONE);
			rotate_set = 1;
		}

	}
	ImageUnit_End();


	if(rotate_set== 1){
		IPCAM_STREAM_INFO *pStreamInfo = UIAppIPCam_get_streamInfo();

		UINT32 i=0;
		for(i=0;i < IPCAM_PATH_MAX_ID;i++){
			if(pStreamInfo[i].sensor == ipl_id && pStreamInfo[i].enable==IPCAM_STATE_ENABLE){
				ImageApp_IPCam_reset_path(i);
			}
		}
	}
#endif


	//#NT#2016/07/28#David Tsai -begin
	//#NT#Add IR cut control in image setting
	pImageInfo->ircut.isSet = 1;
	switch (sImageSetting.eIRCutMode) {
	case ISP_IRCUT_TYPE_NIGHT:
		pImageInfo->ircut.value = MOVIE_IR_CUT_OFF;
		break;
	case ISP_IRCUT_TYPE_DAY:
		pImageInfo->ircut.value = MOVIE_IR_CUT_ON;
		break;
	case ISP_IRCUT_TYPE_AUTO:
	default:
		pImageInfo->ircut.value = MOVIE_IR_CUT_AUTO;
		break;
	}
	//#NT#2016/07/28#David Tsai -end

	return 0;
}

static INT32 param_ae_get(INT32 argc, char *argv[])
{
	UINT8 ipl_id;
	INT32 ipl_phy_id;
	ISPAESetting sAESetting = {0};
	AE_EXT_SET_INFO AeSetInfo = {0};

	if (argc != 1) {
		DBG_ERR("argc!=1 (%d)\r\n", argc);
		return -1;
	}

	if ((ipl_id = atoi(argv[0])) >= IPL_ID_MAX_NUM) {
		DBG_ERR("invalid IPL ID: %u (Max: %d)\r\n", ipl_id, IPL_ID_MAX_NUM);
		return -1;
	}
	ipl_phy_id = IPL_PATH(ipl_id);
	if (ipl_phy_id < 0)
		DBG_ERR("ipl_id=%d, phy_id=%d\r\n", ipl_id,ipl_phy_id);


    AeSetInfo.ExpAreaWeight = sAESetting.uiExpAreaWeight;
	AE_GetExtInfo(ipl_phy_id, &AeSetInfo);

    if((AeSetInfo.ExpT!=0)||(AeSetInfo.ISO!=0)){
        sAESetting.eAEMode = ISP_CTRL_MODE_MANUAL;
        DBG_DUMP("Manual Mode!\r\n");
    } else {
        sAESetting.eAEMode = ISP_CTRL_MODE_AUTO;
        DBG_DUMP("Auto Mode!\r\n");
    }

	sAESetting.uiShutterSpeedMax = AeSetInfo.ExpTBound.h;
	sAESetting.uiShutterSpeedMin = AeSetInfo.ExpTBound.l;
	sAESetting.uiGainMax = AeSetInfo.IsoBound.h;
	sAESetting.uiGainMin = AeSetInfo.IsoBound.l;
	sAESetting.uiConvergenceSpeed = AeSetInfo.ConvergenceSpeed;
	sAESetting.uiManualShutterSpeed = AeSetInfo.ExpT;
	sAESetting.uiManualGain = AeSetInfo.ISO;
	// sAESetting.uiConvergenceStep
	sAESetting.eAntiFlicker = ae_get_ui_info(ipl_phy_id, AE_UI_FREQUENCY);
	// sAESetting.sIrisSetting
	sAESetting.uiEV = ae_get_ui_info(ipl_phy_id, AE_UI_EV);
	sAESetting.uiAETolerance = AeSetInfo.AETolerance;
	sAESetting.uiAEDelay = AeSetInfo.AEDelayCnt;
    sAESetting.eAEStrategy = AeSetInfo.AEStrategy;

	NvtUctrl_SetConfigData((void *)&sAESetting, sizeof(ISPAESetting));

	return 0;
}

static INT32 param_ae_set(INT32 argc, char *argv[])
{
	UINT8 ipl_id;
	INT32 ipl_phy_id;
	ISPAESetting sAESetting = {0};
	AE_EXT_SET_INFO AESetInfo = {0};

	if (argc != 1) {
		DBG_ERR("argc!=1 (%d)\r\n", argc);
		return -1;
	}

	if ((ipl_id = atoi(argv[0])) >= IPL_ID_MAX_NUM) {
		DBG_ERR("invalid IPL ID: %u (Max: %d)\r\n", ipl_id, IPL_ID_MAX_NUM);
		return -1;
	}
	ipl_phy_id = IPL_PATH(ipl_id);
	if (ipl_phy_id < 0)
		DBG_ERR("ipl_id=%d, phy_id=%d\r\n", ipl_id,ipl_phy_id);

	NvtUctrl_GetConfigData((void *)&sAESetting, sizeof(ISPAESetting));

	pImageInfo->ipl_id.value = ipl_id;
	pImageInfo->ipl_id.isSet = 1;

	if (sAESetting.uiManualShutterSpeed == 0) {
		sAESetting.uiManualShutterSpeed = 33333;
	}
	if (sAESetting.uiManualGain == 0) {
		sAESetting.uiManualGain = 100;
	}

	pImageInfo->ae.isSet = 1;
	if (sAESetting.eAEMode == ISP_CTRL_MODE_MANUAL) {
		pImageInfo->ae.mode    = 1;
		pImageInfo->ae.exp     = sAESetting.uiManualShutterSpeed;
		pImageInfo->ae.gain    = sAESetting.uiManualGain;
    	AESetInfo.ExpT = sAESetting.uiManualShutterSpeed;
    	AESetInfo.ISO = sAESetting.uiManualGain;
		//pImageInfo->ae.iris    = atoi(argv[10]);
	} else {
		pImageInfo->ae.mode    = 0;
		pImageInfo->ae.maxexp  = sAESetting.uiShutterSpeedMax;
		pImageInfo->ae.minexp  = sAESetting.uiShutterSpeedMin;
		pImageInfo->ae.maxgain = sAESetting.uiGainMax;
		pImageInfo->ae.mingain = sAESetting.uiGainMin;
    	AESetInfo.ExpT = 0;
    	AESetInfo.ISO = 0;
	}

    AESetInfo.ExpAreaWeight = sAESetting.uiExpAreaWeight;

	AESetInfo.AEDelayCnt = sAESetting.uiAEDelay; // Unit: frame
	AESetInfo.AETolerance = sAESetting.uiAETolerance; // 3~100, Default: 3
	AESetInfo.ConvergenceSpeed = sAESetting.uiConvergenceSpeed;  // 0~6, Default: 4
	AESetInfo.ExpTBound.h = sAESetting.uiShutterSpeedMax;
	AESetInfo.ExpTBound.l = sAESetting.uiShutterSpeedMin;
	AESetInfo.IsoBound.h = sAESetting.uiGainMax;
	AESetInfo.IsoBound.l = sAESetting.uiGainMin;
    AESetInfo.AEStrategy = sAESetting.eAEStrategy;
	//AESetInfo.FnRatioBound

    ae_set_ui_info(ipl_phy_id, AE_UI_FREQUENCY, sAESetting.eAntiFlicker);
    ae_set_ui_info(ipl_phy_id, AE_UI_EV, sAESetting.uiEV);
    //ae_set_ui_info(ipl_id, AE_UI_METER, AE_METER_USERDEFINE);

	AE_SetExtInfo(ipl_phy_id, AESetInfo);

	return 0;
}

static INT32 param_ae_status_get(INT32 argc, char *argv[])
{
	UINT8 ipl_id;
	INT32 ipl_phy_id;
	ISPAEStatus sAEStatus = {0};
	AEStatus nativeAEStatus = {0};

	if (argc != 1) {
		DBG_ERR("argc!=1 (%d)\r\n", argc);
		return -1;
	}

	if ((ipl_id = atoi(argv[0])) >= IPL_ID_MAX_NUM) {
		DBG_ERR("invalid IPL ID: %u (Max: %d)\r\n", ipl_id, IPL_ID_MAX_NUM);
		return -1;
	}
	ipl_phy_id = IPL_PATH(ipl_id);
	if (ipl_phy_id < 0)
		DBG_ERR("ipl_id=%d, phy_id=%d\r\n", ipl_id,ipl_phy_id);

	AE_GetStatus(ipl_phy_id, &nativeAEStatus);
	sAEStatus.uiShutter = nativeAEStatus.ExpT;
	sAEStatus.uiGain = nativeAEStatus.Gain;
	memcpy(sAEStatus.uiLumaValue, nativeAEStatus.LumaValue,
		   sizeof(UINT16) * AE_LUMA_ROW_NUM * AE_LUMA_COLUMN_NUM);
	sAEStatus.uiLumaAverage = nativeAEStatus.LumaAverage;
	memcpy(sAEStatus.uiLumaHistogram, nativeAEStatus.LumaHistogram,
		   sizeof(UINT16) * AE_LUMA_HIST_NUM);
	sAEStatus.uiPreviewLumaValue = AE_GetPrvEVValue(ipl_phy_id);

	NvtUctrl_SetConfigData((void *)&sAEStatus, sizeof(ISPAEStatus));

	return 0;
}

static INT32 param_wb_get(INT32 argc, char *argv[])
{
	UINT8 ipl_id;
	INT32 ipl_phy_id;
	ISPWBSetting sWBSetting = {0};
	AwbSetParam awbsetting = {0};

	if (argc != 1) {
		DBG_ERR("argc!=1 (%d)\r\n", argc);
		return -1;
	}

	if ((ipl_id = atoi(argv[0])) >= IPL_ID_MAX_NUM) {
		DBG_ERR("invalid IPL ID: %u (Max: %d)\r\n", ipl_id, IPL_ID_MAX_NUM);
		return -1;
	}
	ipl_phy_id = IPL_PATH(ipl_id);
	if (ipl_phy_id < 0)
		DBG_ERR("ipl_id=%d, phy_id=%d\r\n", ipl_id,ipl_phy_id);


	AWB_GetInfo(ipl_phy_id, &awbsetting);
	if (awb_get_ui_info(ipl_phy_id, AWB_UI_WB) == AWB_WB_AUTO) {
		sWBSetting.eWBMode = ISP_CTRL_MODE_AUTO;
	}
	else if(awb_get_ui_info(ipl_phy_id, AWB_UI_WB) == AWB_WB_MGAIN){
		sWBSetting.eWBMode = ISP_CTRL_MODE_MANUAL;
	}
	else{
		DBG_ERR("WB mode error (%d)\r\n", awb_get_ui_info(ipl_phy_id, AWB_UI_WB));
		sWBSetting.eWBMode = ISP_CTRL_MODE_AUTO;
	}

	sWBSetting.uiConvergenceSkipFrame = awbsetting.uiConvSkipFrame;
	sWBSetting.uiConvergenceSpeed = awbsetting.uiConvSpeed;
    sWBSetting.uiRratio = awbsetting.uiRratio;
    sWBSetting.uiBratio = awbsetting.uiBratio;
	sWBSetting.uiManualRGain = awbsetting.uiManualRGain;
	sWBSetting.uiManualGGain = awbsetting.uiManualGGain;
	sWBSetting.uiManualBGain = awbsetting.uiManualBGain;

	NvtUctrl_SetConfigData((void *)&sWBSetting, sizeof(ISPWBSetting));

	return 0;
}

static INT32 param_wb_set(INT32 argc, char *argv[])
{
	UINT8 ipl_id;
	INT32 ipl_phy_id;
	ISPWBSetting sWBSetting = {0};
	AwbSetParam awbsetting = {0};

	if (argc != 1) {
		DBG_ERR("argc!=1 (%d)\r\n", argc);
		return -1;
	}

	if ((ipl_id = atoi(argv[0])) >= IPL_ID_MAX_NUM) {
		DBG_ERR("invalid IPL ID: %u (Max: %d)\r\n", ipl_id, IPL_ID_MAX_NUM);
		return -1;
	}
	ipl_phy_id = IPL_PATH(ipl_id);
	if (ipl_phy_id < 0)
		DBG_ERR("ipl_id=%d, phy_id=%d\r\n", ipl_id,ipl_phy_id);

	NvtUctrl_GetConfigData((void *)&sWBSetting, sizeof(ISPWBSetting));

	pImageInfo->ipl_id.value = ipl_id;
	pImageInfo->ipl_id.isSet = 1;

	pImageInfo->awb.isSet = 1;
	if (ISP_CTRL_MODE_AUTO == sWBSetting.eWBMode) {
		pImageInfo->awb.mode  = 0;
        if (sWBSetting.uiRratio < 400) {
			pImageInfo->awb.uiRatio= sWBSetting.uiRratio;
		} else {
			pImageInfo->awb.uiRatio = 400;
		}

        if (sWBSetting.uiBratio < 400) {
			pImageInfo->awb.uiBatio= sWBSetting.uiBratio;
		} else {
			pImageInfo->awb.uiBatio = 400;
		}

	} else {
		pImageInfo->awb.mode  = 1;
		if (sWBSetting.uiManualRGain < 2047) {
			pImageInfo->awb.rgain = sWBSetting.uiManualRGain;
		} else {
			pImageInfo->awb.rgain = 2047;
		}

		// Manual green gain is reserved
		/*
		if (psWBSetting->uiManualGGain < 256)
		    pImageInfo->awb.ggain = psWBSetting->uiManualGGain;
		else
		    pImageInfo->awb.ggain = 256;
		*/
		if (sWBSetting.uiManualBGain < 2047) {
			pImageInfo->awb.bgain = sWBSetting.uiManualBGain;
		} else {
			pImageInfo->awb.bgain = 2047;
		}
	}

	if (sWBSetting.uiConvergenceSkipFrame < SEL_AWB_CONV_SKIP_FRAME_LEVEL_MAX_CNT) {
		awbsetting.uiConvSkipFrame = sWBSetting.uiConvergenceSkipFrame;
	} else {
		awbsetting.uiConvSkipFrame = SEL_AWB_CONV_SKIP_FRAME_LEVEL_MAX_CNT - 1;
	}

	AWB_SetInfo(ipl_phy_id, AWB_SET_SKIP_FRAME, &awbsetting);

	if (sWBSetting.uiConvergenceSpeed < SEL_AWB_CONV_SPEED_LEVEL_MAX_CNT) {
		awbsetting.uiConvSpeed = sWBSetting.uiConvergenceSpeed;
	} else {
		awbsetting.uiConvSpeed = SEL_AWB_CONV_SPEED_LEVEL_MAX_CNT - 1;
	}

	AWB_SetInfo(ipl_phy_id, AWB_SET_SPEED, &awbsetting);

	return 0;
}

static INT32 param_wb_status_get(INT32 argc, char *argv[])
{
	UINT8 ipl_id;
	INT32 ipl_phy_id;
	ISPWBStatus sWBStatus = {0};
	AWBStatus nativeAWBStatus = {0};

	if (argc != 1) {
		DBG_ERR("argc!=1 (%d)\r\n", argc);
		return -1;
	}

	if ((ipl_id = atoi(argv[0])) >= IPL_ID_MAX_NUM) {
		DBG_ERR("invalid IPL ID: %u (Max: %d)\r\n", ipl_id, IPL_ID_MAX_NUM);
		return -1;
	}
	ipl_phy_id = IPL_PATH(ipl_id);
	if (ipl_phy_id < 0)
		DBG_ERR("ipl_id=%d, phy_id=%d\r\n", ipl_id,ipl_phy_id);


	AWB_GetStatus(ipl_phy_id, &nativeAWBStatus);
	sWBStatus.uiRGain = nativeAWBStatus.RGain;
	sWBStatus.uiGGain = nativeAWBStatus.GGain;
	sWBStatus.uiBGain = nativeAWBStatus.BGain;
    sWBStatus.uiRratio = nativeAWBStatus.UserRratio;
    sWBStatus.uiBratio = nativeAWBStatus.UserBratio;
	sWBStatus.uiColorTemperature = nativeAWBStatus.ColorTemperature;
	AWB_Get_ColorAvg(ipl_phy_id, &(sWBStatus.uiRColorAverage), &(sWBStatus.uiGColorAverage), &(sWBStatus.uiBColorAverage));

	NvtUctrl_SetConfigData((void *)&sWBStatus, sizeof(ISPWBStatus));

	return 0;
}

static INT32 param_wdr_get(INT32 argc, char *argv[])
{
#if (IPCAM_FUNC == ENABLE)
	UINT8 ipl_id;
	INT32 ipl_phy_id;
	ISPWDRSetting sWDRSetting = {0};
	UIAPPIPCAM_IPL_FUNC *pIPLFuncInfo= UIAppIPCam_get_IPLFuncInfo();
	if (argc != 1) {
		DBG_ERR("argc!=1 (%d)\r\n", argc);
		return -1;
	}

	if ((ipl_id = atoi(argv[0])) >= IPL_ID_MAX_NUM) {
		DBG_ERR("invalid IPL ID: %u (Max: %d)\r\n", ipl_id, IPL_ID_MAX_NUM);
		return -1;
	}
	ipl_phy_id = IPL_PATH(ipl_id);
	if (ipl_phy_id < 0)
		DBG_ERR("ipl_id=%d, phy_id=%d\r\n", ipl_id,ipl_phy_id);

	switch (pIPLFuncInfo[ipl_id].sWDR.uiWDRMode) {
	case SEL_WDR_MANUAL:

		sWDRSetting.eWDRMode = ISP_CTRL_MODE_MANUAL;
		break;
	case SEL_WDR_AUTO:

		sWDRSetting.eWDRMode = ISP_CTRL_MODE_AUTO;
		break;

	case SEL_WDR_OFF:
	default:

		sWDRSetting.eWDRMode = SEL_WDR_OFF;
		break;
	}
	sWDRSetting.uiWDRStrength =pIPLFuncInfo[ipl_id].sWDR.uiWDRLevel;


	NvtUctrl_SetConfigData((void *)&sWDRSetting, sizeof(ISPWDRSetting));
#else
	DBG_ERR("not support!\r\n");
#endif

	return 0;
}

static INT32 param_wdr_set(INT32 argc, char *argv[])
{
	UINT8 ipl_id;
	ISPWDRSetting sWDRSetting = {0};

	if (argc != 1) {
		DBG_ERR("argc!=1 (%d)\r\n", argc);
		return -1;
	}

	if ((ipl_id = atoi(argv[0])) >= IPL_ID_MAX_NUM) {
		DBG_ERR("invalid IPL ID: %u (Max: %d)\r\n", ipl_id, IPL_ID_MAX_NUM);
		return -1;
	}
	NvtUctrl_GetConfigData((void *)&sWDRSetting, sizeof(ISPWDRSetting));
	DBG_DUMP("WDR mode:%d level:%d\r\n",sWDRSetting.eWDRMode,sWDRSetting.uiWDRStrength);
	pImageInfo->ipl_id.value = ipl_id;
	pImageInfo->ipl_id.isSet = 1;

	pImageInfo->wdr.isSet = 1;

	if (sWDRSetting.eWDRMode == ISP_CTRL_MODE_AUTO) {
		pImageInfo->wdr.mode = SEL_WDR_AUTO;
	}
	else if(sWDRSetting.eWDRMode == ISP_CTRL_MODE_MANUAL){
		pImageInfo->wdr.mode = SEL_WDR_MANUAL;
	}
	else{
		pImageInfo->wdr.mode = SEL_WDR_OFF;
	}

	pImageInfo->wdr.lv = sWDRSetting.uiWDRStrength;

	return 0;
}

static INT32 param_nr_get(INT32 argc, char *argv[])
{

#if(IPCAM_FUNC == ENABLE)
	UINT8 ipl_id;
	INT32 ipl_phy_id;
	ISPNRSetting sNRSetting = {0};

	 IQ_LINUX_SET_INFO iq_linux_set_info = {0};
	// UINT32 path_id;

	if (argc != 1) {
		DBG_ERR("argc!=1 (%d)\r\n", argc);
		return -1;
	}

	if ((ipl_id = atoi(argv[0])) >= IPL_ID_MAX_NUM) {
		DBG_ERR("invalid IPL ID: %u (Max: %d)\r\n", ipl_id, IPL_ID_MAX_NUM);
		return -1;
	}
	ipl_phy_id = IPL_PATH(ipl_id);
	if (ipl_phy_id < 0)
		DBG_ERR("ipl_id=%d, phy_id=%d\r\n", ipl_id,ipl_phy_id);



	//#NT#2017/05/03#Charlie Chang -begin
	//#NT# for 3DNR seeting, for test getting from ImageAppIPCam

	UIAPPIPCAM_SENSOR_INFO *sensorInfo = UIAppIPCam_get_SensorInfo();
	if (sensorInfo == NULL) {
		DBG_ERR("get sensor info error\r\n");
		return -1;
	}
	sNRSetting.uiNR3DStrength = sensorInfo[ipl_id].bIME3DNR;

	//NvtUctrl_SetConfigData((void *)&sNRSetting, sizeof(ISPNRSetting));



	//#NT#2017/05/03#Charlie Chang -end


	iq_get_linux_iq_info(ipl_phy_id, &iq_linux_set_info);
	sNRSetting.uiNR2DStrength = iq_linux_set_info.nr_ratio;

	NvtUctrl_SetConfigData((void *)&sNRSetting, sizeof(ISPNRSetting));
#endif
	return 0;
}

static INT32 param_nr_set(INT32 argc, char *argv[])
{
	UINT8 ipl_id;
	ISPNRSetting sNRSetting = {0};

	if (argc != 1) {
		DBG_ERR("argc!=1 (%d)\r\n", argc);
		return -1;
	}

	if ((ipl_id = atoi(argv[0])) >= IPL_ID_MAX_NUM) {
		DBG_ERR("invalid IPL ID: %u (Max: %d)\r\n", ipl_id, IPL_ID_MAX_NUM);
		return -1;
	}

	NvtUctrl_GetConfigData((void *)&sNRSetting, sizeof(ISPNRSetting));

	pImageInfo->ipl_id.value = ipl_id;
	pImageInfo->ipl_id.isSet = 1;

	pImageInfo->nr2D.isSet = 1;
	pImageInfo->nr2D.value = sNRSetting.uiNR2DStrength; // 0~100

	pImageInfo->nr3D.isSet = 1;
	pImageInfo->nr3D.mode = 1; // Manual
	pImageInfo->nr3D.ratio = sNRSetting.uiNR3DStrength; // 0~10

	return 0;
}

static INT32 param_dis_get(INT32 argc, char *argv[])
{
	#if(IPCAM_FUNC ==ENABLE)
	UINT8 ipl_id;
	ISPDISSetting sDISSetting = {0};
	UIAPPIPCAM_IPL_FUNC *pIPLFuncInfo= UIAppIPCam_get_IPLFuncInfo();
	if (argc != 1) {
		DBG_ERR("argc!=1 (%d)\r\n", argc);
		return -1;
	}

	if ((ipl_id = atoi(argv[0])) >= IPL_ID_MAX_NUM) {
		DBG_ERR("invalid IPL ID: %u (Max: %d)\r\n", ipl_id, IPL_ID_MAX_NUM);
		return -1;
	}

	switch (pIPLFuncInfo[ipl_id].uiDIS) {
	case SEL_DISCOMP_ON:
		sDISSetting.bDISEnable = NVT_TRUE;
		break;
	case SEL_DISCOMP_OFF:
	default:
		sDISSetting.bDISEnable = NVT_FALSE;
		break;
	}

	NvtUctrl_SetConfigData((void *)&sDISSetting, sizeof(ISPDISSetting));
	#else
		DBG_ERR("not support!\r\n");
	#endif
	return 0;
}

static INT32 param_dis_set(INT32 argc, char *argv[])
{
	#if(IPCAM_FUNC == ENABLE)
	#if (IPCAM_DIS_MV_FUNC == ENABLE)
		UINT8 ipl_id;
		ISPDISSetting sDISSetting = {0};

		if (argc != 1) {
			DBG_ERR("argc!=1 (%d)\r\n", argc);
			return -1;
		}

		if ((ipl_id = atoi(argv[0])) >= IPL_ID_MAX_NUM) {
			DBG_ERR("invalid IPL ID: %u (Max: %d)\r\n", ipl_id, IPL_ID_MAX_NUM);
			return -1;
		}

		NvtUctrl_GetConfigData((void *)&sDISSetting, sizeof(ISPDISSetting));

		pImageInfo->ipl_id.value = ipl_id;
		pImageInfo->ipl_id.isSet = 1;

		pImageInfo->dis.isSet = 1;
		if (sDISSetting.bDISEnable == NVT_TRUE) {
			pImageInfo->dis.mode = 1;
		} else {
			pImageInfo->dis.mode = 0;
		}
	#else
		DBG_ERR("not support!!\r\n");
		return -1;
	#endif
	#else
		DBG_ERR("not support!\r\n");
	#endif
	return 0;
}

static INT32 param_ldc_get(INT32 argc, char *argv[])
{
	UINT8 ipl_id;
	INT32 ipl_phy_id;
	ISPLDCSetting sLDCSetting = {0};
	IQ_LINUX_SET_INFO iq_linux_set_info = {0};

	if (argc != 1) {
		DBG_ERR("argc!=1 (%d)\r\n", argc);
		return -1;
	}

	if ((ipl_id = atoi(argv[0])) >= IPL_ID_MAX_NUM) {
		DBG_ERR("invalid IPL ID: %u (Max: %d)\r\n", ipl_id, IPL_ID_MAX_NUM);
		return -1;
	}
	ipl_phy_id = IPL_PATH(ipl_id);
	if (ipl_phy_id < 0)
		DBG_ERR("ipl_id=%d, phy_id=%d\r\n", ipl_id,ipl_phy_id);


	iq_get_linux_iq_info(ipl_phy_id, &iq_linux_set_info);

	sLDCSetting.bLDCEnable = iq_linux_set_info.gdc.enable;
	sLDCSetting.uiRatio = iq_linux_set_info.gdc.ratio;

	NvtUctrl_SetConfigData((void *)&sLDCSetting, sizeof(ISPLDCSetting));

	return 0;
}

static INT32 param_ldc_set(INT32 argc, char *argv[])
{
	UINT8 ipl_id;
	ISPLDCSetting sLDCSetting = {0};

	if (argc != 1) {
		DBG_ERR("argc!=1 (%d)\r\n", argc);
		return -1;
	}

	if ((ipl_id = atoi(argv[0])) >= IPL_ID_MAX_NUM) {
		DBG_ERR("invalid IPL ID: %u (Max: %d)\r\n", ipl_id, IPL_ID_MAX_NUM);
		return -1;
	}

	NvtUctrl_GetConfigData((void *)&sLDCSetting, sizeof(ISPLDCSetting));

	pImageInfo->ipl_id.value = ipl_id;
	pImageInfo->ipl_id.isSet = 1;

	pImageInfo->LDC.isSet = 1;
	pImageInfo->LDC.isEnable = sLDCSetting.bLDCEnable;
	pImageInfo->LDC.ratio = sLDCSetting.uiRatio; // 0~100

	return 0;
}

static INT32 param_lsc_get(INT32 argc, char *argv[])
{
	UINT8 ipl_id;
	INT32 ipl_phy_id;
	ISPLSCSetting sLSCSetting = {0};
	IQ_LINUX_SET_INFO iq_linux_set_info = {0};

	if (argc != 1) {
		DBG_ERR("argc!=1 (%d)\r\n", argc);
		return -1;
	}

	if ((ipl_id = atoi(argv[0])) >= IPL_ID_MAX_NUM) {
		DBG_ERR("invalid IPL ID: %u (Max: %d)\r\n", ipl_id, IPL_ID_MAX_NUM);
		return -1;
	}
	ipl_phy_id = IPL_PATH(ipl_id);
	if (ipl_phy_id < 0)
		DBG_ERR("ipl_id=%d, phy_id=%d\r\n", ipl_id,ipl_phy_id);

	iq_get_linux_iq_info(ipl_phy_id, &iq_linux_set_info);

	sLSCSetting.bLSCEnable = iq_linux_set_info.vig.enable;
	sLSCSetting.uiRatio = iq_linux_set_info.vig.ratio;
	sLSCSetting.uiCenterXOffset = iq_linux_set_info.vig.center_x;
	sLSCSetting.uiCenterYOffset = iq_linux_set_info.vig.center_y;

	NvtUctrl_SetConfigData((void *)&sLSCSetting, sizeof(ISPLSCSetting));

	return 0;
}

static INT32 param_lsc_set(INT32 argc, char *argv[])
{
	UINT8 ipl_id;
	ISPLSCSetting sLSCSetting = {0};

	if (argc != 1) {
		DBG_ERR("argc!=1 (%d)\r\n", argc);
		return -1;
	}

	if ((ipl_id = atoi(argv[0])) >= IPL_ID_MAX_NUM) {
		DBG_ERR("invalid IPL ID: %u (Max: %d)\r\n", ipl_id, IPL_ID_MAX_NUM);
		return -1;
	}

	NvtUctrl_GetConfigData((void *)&sLSCSetting, sizeof(ISPLSCSetting));

	pImageInfo->ipl_id.value = ipl_id;
	pImageInfo->ipl_id.isSet = 1;

	pImageInfo->LSC.isSet = 1;
	pImageInfo->LSC.isEnable = sLSCSetting.bLSCEnable;
	pImageInfo->LSC.ratio = sLSCSetting.uiRatio;  // 0~100, default 0
	pImageInfo->LSC.CenterXOffset = sLSCSetting.uiCenterXOffset; // 0~100, default 50
	pImageInfo->LSC.CenterYOffset = sLSCSetting.uiCenterYOffset; // 0~100, default 50

	return 0;
}

static INT32 param_defog_get(INT32 argc, char *argv[])
{
#if (IPCAM_FUNC== ENABLE)
	UINT8 ipl_id;
	ISPDeFogSetting sDeFogSetting = {0};


	if (argc != 1) {
		DBG_ERR("argc!=1 (%d)\r\n", argc);
		return -1;
	}

	if ((ipl_id = atoi(argv[0])) >= IPL_ID_MAX_NUM) {
		DBG_ERR("invalid IPL ID: %u (Max: %d)\r\n", ipl_id, IPL_ID_MAX_NUM);
		return -1;
	}



	UIAPPIPCAM_IPL_FUNC *psIPLFunc = UIAppIPCam_get_IPLFuncInfo();

	sDeFogSetting.eDeFogMode = psIPLFunc->sDefog.eDeFogMode;
	sDeFogSetting.uiDeFogStrength = psIPLFunc->sDefog.uiLevel;
	NvtUctrl_SetConfigData((void *)&sDeFogSetting, sizeof(ISPDeFogSetting));
#else
	DBG_ERR("not support!!\r\n");
#endif


	return 0;
}

static INT32 param_defog_set(INT32 argc, char *argv[])
{
	UINT8 ipl_id;
	INT32 ipl_phy_id;
	ISPDeFogSetting sDeFogSetting = {0};

	if (argc != 1) {
		DBG_ERR("argc!=1 (%d)\r\n", argc);
		return -1;
	}

	if ((ipl_id = atoi(argv[0])) >= IPL_ID_MAX_NUM) {
		DBG_ERR("invalid IPL ID: %u (Max: %d)\r\n", ipl_id, IPL_ID_MAX_NUM);
		return -1;
	}
	ipl_phy_id = IPL_PATH(ipl_id);
	if (ipl_phy_id < 0)
		DBG_ERR("ipl_id=%d, phy_id=%d\r\n", ipl_id,ipl_phy_id);

	if (iq_get_ui_info(ipl_phy_id, IQ_UI_IPC_MODE) != IQ_IPC_ON) {
		iq_set_ui_info(ipl_phy_id, IQ_UI_IPC_MODE, IQ_IPC_ON);
	}

	NvtUctrl_GetConfigData((void *)&sDeFogSetting, sizeof(ISPDeFogSetting));

	pImageInfo->ipl_id.value = ipl_id;
	pImageInfo->ipl_id.isSet = 1;

	pImageInfo->defog.isSet = 1;
	pImageInfo->defog.mode =  sDeFogSetting.eDeFogMode;

	pImageInfo->defog.lv = sDeFogSetting.uiDeFogStrength; // 0~255

	if (pImageInfo->defog.lv > 255) {
		pImageInfo->defog.lv = 255;
	}

	return 0;
}

static INT32 param_gamma_get(INT32 argc, char *argv[])
{
	#if 1
		DBG_WRN("not support!\r\n");
	#else
	UINT8 ipl_id;
	ISPGammaSetting sGammaSetting = {0};

	if (argc != 1) {
		DBG_ERR("argc!=1 (%d)\r\n", argc);
		return -1;
	}

	if ((ipl_id = atoi(argv[0])) >= IPL_ID_MAX_NUM) {
		DBG_ERR("invalid IPL ID: %u (Max: %d)\r\n", ipl_id, IPL_ID_MAX_NUM);
		return -1;
	}

	ImageUnit_Begin(ISF_IPL(ipl_id), 0);
	if (ImageUnit_GetParam(ISF_IPL(ipl_id), ISF_CTRL, IPL_SEL_IPC_MODE_IMM) != SEL_IPC_MODE_ON) {
		ImageUnit_SetParam(ISF_CTRL, IPL_SEL_IPC_MODE_IMM, SEL_IPC_MODE_ON);
	}
	ImageUnit_End();

	switch (ImageUnit_GetParam(ISF_IPL(ipl_id), ISF_CTRL, IPL_SEL_USER_GAMMA_MODE_IMM)) {
	case SEL_USER_GAMMA_LINEAR:
		sGammaSetting.eGammaCurveMode = ISP_GAMMA_CURVE_MODE_LINEAR;
		break;
	case SEL_USER_GAMMA_CURVE_MODE_16:
		sGammaSetting.eGammaCurveMode = ISP_GAMMA_CURVE_MODE_16;
		break;
	case SEL_USER_GAMMA_CURVE_MODE_18:
		sGammaSetting.eGammaCurveMode = ISP_GAMMA_CURVE_MODE_18;
		break;
	case SEL_USER_GAMMA_CURVE_MODE_20:
		sGammaSetting.eGammaCurveMode = ISP_GAMMA_CURVE_MODE_20;
		break;
	case SEL_USER_GAMMA_CURVE_MODE_22:
		sGammaSetting.eGammaCurveMode = ISP_GAMMA_CURVE_MODE_22;
		break;
	case SEL_USER_GAMMA_CURVE_MODE_24:
		sGammaSetting.eGammaCurveMode = ISP_GAMMA_CURVE_MODE_24;
		break;
	case SEL_USER_GAMMA_CURVE_MODE_USER:
		sGammaSetting.eGammaCurveMode = ISP_GAMMA_CURVE_MODE_USER;
		break;
	case SEL_USER_GAMMA_CURVE_DEFAULT:
	default:
		sGammaSetting.eGammaCurveMode = ISP_GAMMA_CURVE_MODE_DEFAULT;
		break;
	}
#if _TODO
#if ((_SENSORLIB_ == _SENSORLIB_CMOS_IMX291M_) || \
     (_SENSORLIB_ == _SENSORLIB_CMOS_IMX123LQT_) || \
     (_SENSORLIB_ == _SENSORLIB_CMOS_AR0230CS_) || \
     (_SENSORLIB_ == _SENSORLIB_CMOS_MN34110_))
	IQS_getUserDefineGamma(sGammaSetting.uiUserGammaTable);
#endif
#endif
	NvtUctrl_SetConfigData((void *)&sGammaSetting, sizeof(ISPGammaSetting));
#endif
	return 0;
}

static INT32 param_gamma_set(INT32 argc, char *argv[])
{

	#if 1
		DBG_WRN("not support!\r\n");
	#else
	UINT8 ipl_id;
	ISPGammaSetting sGammaSetting = {0};

	if (argc != 1) {
		DBG_ERR("argc!=1 (%d)\r\n", argc);
		return -1;
	}

	if ((ipl_id = atoi(argv[0])) >= IPL_ID_MAX_NUM) {
		DBG_ERR("invalid IPL ID: %u (Max: %d)\r\n", ipl_id, IPL_ID_MAX_NUM);
		return -1;
	}

	NvtUctrl_GetConfigData((void *)&sGammaSetting, sizeof(ISPGammaSetting));

	ImageUnit_Begin(ISF_IPL(ipl_id), 0);
	if (ImageUnit_GetParam(ISF_IPL(ipl_id), ISF_CTRL, IPL_SEL_IPC_MODE_IMM) != SEL_IPC_MODE_ON) {
		ImageUnit_SetParam(ISF_CTRL, IPL_SEL_IPC_MODE_IMM, SEL_IPC_MODE_ON);
	}
	ImageUnit_End();
#if _TODO
#if ((_SENSORLIB_ == _SENSORLIB_CMOS_IMX291M_) || \
     (_SENSORLIB_ == _SENSORLIB_CMOS_IMX123LQT_) || \
     (_SENSORLIB_ == _SENSORLIB_CMOS_AR0230CS_) || \
     (_SENSORLIB_ == _SENSORLIB_CMOS_MN34110_))
	IQS_setUserDefineGamma(sGammaSetting.uiUserGammaTable);
#endif
#endif
	ImageUnit_Begin(ISF_IPL(ipl_id), 0);
	switch (sGammaSetting.eGammaCurveMode) {
	case ISP_GAMMA_CURVE_MODE_LINEAR:
	    	ImageUnit_SetParam(ISF_CTRL, IPL_SEL_USER_GAMMA_MODE_IMM, SEL_USER_GAMMA_LINEAR);
		break;
	case ISP_GAMMA_CURVE_MODE_16:
		ImageUnit_SetParam(ISF_CTRL, IPL_SEL_USER_GAMMA_MODE_IMM, SEL_USER_GAMMA_CURVE_MODE_16);
		break;
	case ISP_GAMMA_CURVE_MODE_18:
		ImageUnit_SetParam(ISF_CTRL, IPL_SEL_USER_GAMMA_MODE_IMM, SEL_USER_GAMMA_CURVE_MODE_18);
		break;
	case ISP_GAMMA_CURVE_MODE_20:
		ImageUnit_SetParam(ISF_CTRL, IPL_SEL_USER_GAMMA_MODE_IMM, SEL_USER_GAMMA_CURVE_MODE_20);
		break;
	case ISP_GAMMA_CURVE_MODE_22:
		ImageUnit_SetParam(ISF_CTRL, IPL_SEL_USER_GAMMA_MODE_IMM, SEL_USER_GAMMA_CURVE_MODE_22);
		break;
	case ISP_GAMMA_CURVE_MODE_24:
		ImageUnit_SetParam(ISF_CTRL, IPL_SEL_USER_GAMMA_MODE_IMM, SEL_USER_GAMMA_CURVE_MODE_24);
		break;
	case ISP_GAMMA_CURVE_MODE_USER:
		ImageUnit_SetParam(ISF_CTRL, IPL_SEL_USER_GAMMA_MODE_IMM, SEL_USER_GAMMA_CURVE_MODE_USER);
		break;
	case ISP_GAMMA_CURVE_MODE_DEFAULT:
	default:
		ImageUnit_SetParam(ISF_CTRL, IPL_SEL_USER_GAMMA_MODE_IMM, SEL_USER_GAMMA_CURVE_DEFAULT);
		break;
	}
	ImageUnit_End();
	#endif
	return 0;
}

static INT32 param_ccm_get(INT32 argc, char *argv[])
{
	#if 1
		DBG_WRN("not support!\r\n");
	#else
	UINT8 ipl_id;
	ISPCCMSetting sCCMSetting = {0};

	if (argc != 1) {
		DBG_ERR("argc!=1 (%d)\r\n", argc);
		return -1;
	}

	if ((ipl_id = atoi(argv[0])) >= IPL_ID_MAX_NUM) {
		DBG_ERR("invalid IPL ID: %u (Max: %d)\r\n", ipl_id, IPL_ID_MAX_NUM);
		return -1;
	}

	ImageUnit_Begin(ISF_IPL(ipl_id), 0);
	if (ImageUnit_GetParam(ISF_IPL(ipl_id), ISF_CTRL, IPL_SEL_IPC_MODE_IMM) != SEL_IPC_MODE_ON) {
		ImageUnit_SetParam(ISF_CTRL, IPL_SEL_IPC_MODE_IMM, SEL_IPC_MODE_ON);
	}
	ImageUnit_End();

	switch (ImageUnit_GetParam(ISF_IPL(ipl_id), ISF_CTRL, IPL_SEL_USER_CC_IMM)) {
	case SEL_USER_CC_MANUAL:
		sCCMSetting.eCCMMode = ISP_CTRL_MODE_MANUAL;
		break;
	case SEL_USER_CC_AUTO:
	default:
		sCCMSetting.eCCMMode = ISP_CTRL_MODE_AUTO;
		break;
	}
#if _TODO
#if ((_SENSORLIB_ == _SENSORLIB_CMOS_IMX291M_) || \
     (_SENSORLIB_ == _SENSORLIB_CMOS_IMX123LQT_) || \
     (_SENSORLIB_ == _SENSORLIB_CMOS_AR0230CS_) || \
     (_SENSORLIB_ == _SENSORLIB_CMOS_MN34110_))
	IQS_getUserDefineCC(0, sCCMSetting.uiLowCCM);
	IQS_getUserDefineCC(1, sCCMSetting.uiMiddleCCM);
	IQS_getUserDefineCC(2, sCCMSetting.uiHighCCM);
#endif
#endif
	NvtUctrl_SetConfigData((void *)&sCCMSetting, sizeof(ISPCCMSetting));
	#endif
	return 0;
}

static INT32 param_ccm_set(INT32 argc, char *argv[])
{
	#if 1
		DBG_WRN("not support!\r\n");
	#else
	UINT8 ipl_id;
	ISPCCMSetting sCCMSetting = {0};

	if (argc != 1) {
		DBG_ERR("argc!=1 (%d)\r\n", argc);
		return -1;
	}

	if ((ipl_id = atoi(argv[0])) >= IPL_ID_MAX_NUM) {
		DBG_ERR("invalid IPL ID: %u (Max: %d)\r\n", ipl_id, IPL_ID_MAX_NUM);
		return -1;
	}

	NvtUctrl_GetConfigData((void *)&sCCMSetting, sizeof(ISPCCMSetting));

	ImageUnit_Begin(ISF_IPL(ipl_id), 0);
	if (ImageUnit_GetParam(ISF_IPL(ipl_id), ISF_CTRL, IPL_SEL_IPC_MODE_IMM) != SEL_IPC_MODE_ON) {
		ImageUnit_SetParam(ISF_CTRL, IPL_SEL_IPC_MODE_IMM, SEL_IPC_MODE_ON);
	}
	ImageUnit_End();

	ImageUnit_Begin(ISF_IPL(ipl_id), 0);
	switch (sCCMSetting.eCCMMode) {
	case ISP_CTRL_MODE_MANUAL:
		ImageUnit_SetParam(ISF_CTRL, IPL_SEL_USER_CC_IMM, SEL_USER_CC_MANUAL);
		break;
	case ISP_CTRL_MODE_AUTO:
	default:
		ImageUnit_SetParam(ISF_CTRL, IPL_SEL_USER_CC_IMM, SEL_USER_CC_AUTO);
		break;
	}
	ImageUnit_End();
#if _TODO
#if ((_SENSORLIB_ == _SENSORLIB_CMOS_IMX291M_) || \
     (_SENSORLIB_ == _SENSORLIB_CMOS_IMX123LQT_) || \
     (_SENSORLIB_ == _SENSORLIB_CMOS_AR0230CS_) || \
     (_SENSORLIB_ == _SENSORLIB_CMOS_MN34110_))
	IQS_setUserDefineCC(0, sCCMSetting.uiLowCCM);
	IQS_setUserDefineCC(1, sCCMSetting.uiMiddleCCM);
	IQS_setUserDefineCC(2, sCCMSetting.uiHighCCM);
#endif
#endif
	#endif
	return 0;
}


static INT32 param_iplmap_get(INT32 argc, char *argv[])
{
#if(IPCAM_FUNC == ENABLE)

	IPCAM_SENSOR SensorMap[IPCAM_PATH_MAX_ID] = {0};
	char   retStr[32];
	ImageApp_IPCam_Get_Sensor_Mapping(&SensorMap[0]);
	DBG_DUMP("ipl path map: %d %d %d\r\n", SensorMap[0], SensorMap[1], SensorMap[2]);
	snprintf(retStr, sizeof(retStr), "%d %d %d", SensorMap[0], SensorMap[1], SensorMap[2]);
	NvtUctrl_SetRetString(retStr);
#endif
	return 0;
}

static INT32 param_iplmap_set(INT32 argc, char *argv[])
{
#if(IPCAM_FUNC == ENABLE)

	if (argc != IPCAM_PATH_MAX_ID) {
		DBG_ERR("argc!=%d (%d)\r\n", IPCAM_PATH_MAX_ID, argc);
		return -1;
	}
	IPCAM_SENSOR ipl_list[IPCAM_PATH_MAX_ID];
	int i = 0;
	for (i = 0; i < IPCAM_PATH_MAX_ID; i++) {
		ipl_list[i] = atoi(argv[i]);

	}
	ImageApp_IPCam_Set_Sensor_Mapping(ipl_list);

	////change mode for restart ImageApp_IPCam
	Ux_PostEvent(NVTEVT_SYSTEM_MODE, 1, PRIMARY_MODE_IPCAM);

#endif
	return 0;
}


static INT32 param_dewarp_set(INT32 argc, char *argv[])
{
	return 0;
}
static INT32 param_crop_set(INT32 argc, char *argv[]){


	#if(IPCAM_FUNC == ENABLE)
		ISPImageCropSetting sCropSetting = {0};
		UIAPPIPCAM_CROP_INFO *pCropInfo = UIAppIPCam_get_CropInfo();
		NvtUctrl_GetConfigData((void *)&sCropSetting, sizeof(ISPImageCropSetting));
		UINT32 uiPath=0;

		UINT32 uiSensorIndex = sCropSetting.uiSensor;
		UINT32 uiIMEIndex    = sCropSetting.uiIME;
		IPCAM_STREAM_INFO *pStreamInfo = UIAppIPCam_get_streamInfo();
		UIAPPIPCAM_SENSOR_INFO* pSensorInfo = UIAppIPCam_get_SensorInfo();
		UIAPPIPCAM_ENCODE_CONFIG *pEncodeInfo = UIAppIPCam_get_EncodeConfig();


		////get Sensor Input size
		IPL_CUR_INPUT_INFO sIPLSensorInfo = {0};
		sIPLSensorInfo.id = uiSensorIndex;
		IPL_GetCmd(IPL_GET_IPL_CUR_INPUT_INFOR ,&sIPLSensorInfo);



		for(uiPath = 0; uiPath < UIAPPIPCAM_PATH_MAX_ID ; uiPath++){
			if(pStreamInfo[uiPath].enable != IPCAM_STATE_ENABLE){
				continue;
			}
			if(uiSensorIndex ==  pStreamInfo[uiPath].sensor && uiIMEIndex == pStreamInfo[uiPath].ime){
				if(uiIMEIndex == IPCAM_IME1 && (pSensorInfo[uiSensorIndex].bIME3DNR == TRUE || pSensorInfo[uiSensorIndex].bIME1Compress == TRUE)){
					DBG_ERR("Sensor%d IME3DNR:%d compress:%d can not crop for IME1!!\r\n",
					uiSensorIndex,
					pSensorInfo[uiSensorIndex].bIME3DNR,
					pSensorInfo[uiSensorIndex].bIME1Compress);
					return -1;
				}



				if(sCropSetting.bEnable == TRUE){

					if((sCropSetting.sCropInfo.x + sCropSetting.sCropInfo.w) > (UINT32)sIPLSensorInfo.in_size_h){
						DBG_ERR("crop  x:%d w:%d  Sensor input width:%d  (x+w) > Sensor input width error\r\n",
							sCropSetting.sCropInfo.x,sCropSetting.sCropInfo.w,sIPLSensorInfo.in_size_h);
						return -1;
					}
					if((sCropSetting.sCropInfo.y + sCropSetting.sCropInfo.h) > (UINT32)sIPLSensorInfo.in_size_v){
						DBG_ERR("crop  y:%d h:%d  Sensor input height:%d  (y+h) > resolution height error\r\n",
							sCropSetting.sCropInfo.y,sCropSetting.sCropInfo.h,sIPLSensorInfo.in_size_v);
						return -1;
					}

					if(!sCropSetting.sCropInfo.w || !sCropSetting.sCropInfo.h){


						DBG_ERR("sCropInfo.w:%d sCropInfo.h:%d  error\r\n",sCropSetting.sCropInfo.w,sCropSetting.sCropInfo.h);
						return -1;
					}

					////IME crop,   first ime scale and than ime crop, so crop size is final review size

					UINT32 uiScaleCropW = sCropSetting.sCropInfo.w*sCropSetting.uiScaleWidth/sCropSetting.sCropInfo.w;
					UINT32 uiScaleCropH = sCropSetting.sCropInfo.h*sCropSetting.uiScaleHeight/sCropSetting.sCropInfo.h;
					UINT32 uiScaleCropY = sCropSetting.sCropInfo.y*sCropSetting.uiScaleHeight/sCropSetting.sCropInfo.h;
					UINT32 uiScaleCropX = sCropSetting.sCropInfo.x*sCropSetting.uiScaleWidth/sCropSetting.sCropInfo.w;
					UINT32 uiScaleW		= sIPLSensorInfo.in_size_h*sCropSetting.uiScaleWidth/sCropSetting.sCropInfo.w;
					UINT32 uiScaleH		= sIPLSensorInfo.in_size_v*sCropSetting.uiScaleHeight/sCropSetting.sCropInfo.h;


					ImageUnit_Begin(&ISF_UserProc, 0);
						ImageUnit_SetVdoImgSize(uiPath + ISF_IN_BASE, sCropSetting.uiScaleWidth, sCropSetting.uiScaleHeight);
					ImageUnit_End();

					ImageUnit_Begin(&ISF_VdoEnc, 0);
						ImageUnit_SetVdoImgSize(uiPath + ISF_IN_BASE, sCropSetting.uiScaleWidth, sCropSetting.uiScaleHeight);
					ImageUnit_End();
					if(pStreamInfo[uiPath].type == IPCAM_STREAM_MAIN){
						ImageUnit_Begin(&ISF_ImgTrans, 0);
							ImageUnit_SetVdoImgSize(uiPath + ISF_IN_BASE, uiScaleW, uiScaleH);
							ImageUnit_SetVdoPostWindow(uiPath + ISF_IN_BASE,
											uiScaleCropX,
											uiScaleCropY,
											uiScaleCropW,
											uiScaleCropH);
						ImageUnit_End();
						pCropInfo[uiSensorIndex].sCropInfo[uiIMEIndex].sCropRECT.h =sCropSetting.sCropInfo.h;
						pCropInfo[uiSensorIndex].sCropInfo[uiIMEIndex].sCropRECT.w =sCropSetting.sCropInfo.w;
						pCropInfo[uiSensorIndex].sCropInfo[uiIMEIndex].sCropRECT.x =sCropSetting.sCropInfo.x;
						pCropInfo[uiSensorIndex].sCropInfo[uiIMEIndex].sCropRECT.y =sCropSetting.sCropInfo.y;
						pCropInfo[uiSensorIndex].sCropInfo[uiIMEIndex].uiScaleWidth =sCropSetting.uiScaleWidth;
						pCropInfo[uiSensorIndex].sCropInfo[uiIMEIndex].uiScaleHeight=sCropSetting.uiScaleHeight;
					}

				}
				else{
					ImageUnit_Begin(&ISF_UserProc, 0);
						ImageUnit_SetVdoImgSize(uiPath + ISF_IN_BASE, pEncodeInfo[uiPath].resolution.w, pEncodeInfo[uiPath].resolution.h);
					ImageUnit_End();

					ImageUnit_Begin(&ISF_VdoEnc, 0);
						ImageUnit_SetVdoImgSize(uiPath + ISF_IN_BASE, pEncodeInfo[uiPath].resolution.w, pEncodeInfo[uiPath].resolution.h);
					ImageUnit_End();
					if(pStreamInfo[uiPath].type == IPCAM_STREAM_MAIN){
						ImageUnit_Begin(&ISF_ImgTrans, 0);
							if(pStreamInfo[uiPath].type == IPCAM_STREAM_MAIN){
								ImageUnit_SetVdoImgSize(uiPath + ISF_IN_BASE, pEncodeInfo[uiPath].resolution.w, pEncodeInfo[uiPath].resolution.h);
							}
							ImageUnit_SetVdoPostWindow(uiPath + ISF_IN_BASE,0,0,0,0);
						ImageUnit_End();
						pCropInfo[uiSensorIndex].sCropInfo[uiIMEIndex].sCropRECT.h = sCropSetting.sCropInfo.h;
						pCropInfo[uiSensorIndex].sCropInfo[uiIMEIndex].sCropRECT.w = sCropSetting.sCropInfo.w;
						pCropInfo[uiSensorIndex].sCropInfo[uiIMEIndex].sCropRECT.x = sCropSetting.sCropInfo.x;
						pCropInfo[uiSensorIndex].sCropInfo[uiIMEIndex].sCropRECT.y = sCropSetting.sCropInfo.y;
						pCropInfo[uiSensorIndex].sCropInfo[uiIMEIndex].uiScaleWidth = sCropSetting.uiScaleWidth;
						pCropInfo[uiSensorIndex].sCropInfo[uiIMEIndex].uiScaleHeight= sCropSetting.uiScaleHeight;
					}
				}
				pSensorInfo[uiSensorIndex].bCropEnable[uiIMEIndex] = sCropSetting.bEnable;
				ImageApp_IPCam_reset_path(uiPath);
			}
		}


	#else
		DBG_ERR("not support!!\r\n");
	#endif

	return 0;
}

static INT32 param_crop_get(INT32 argc, char *argv[])
{

	#if(IPCAM_FUNC == ENABLE)

		if (argc != 2) {
			DBG_ERR("argc!=%d (%d)\r\n", 2, argc);
			return -1;
		}
		UINT32 uiSensorIndex = atoi(argv[0]);
		UINT32 uiIMEIndex 	 = atoi(argv[1]);

		UIAPPIPCAM_SENSOR_INFO *pSenSorInfo = UIAppIPCam_get_SensorInfo();
		UIAPPIPCAM_CROP_INFO *pCropInfo = UIAppIPCam_get_CropInfo();
		ISPImageCropSetting sCropSetting = {0};

		sCropSetting.uiSensor = uiSensorIndex;
		sCropSetting.uiIME	 = uiIMEIndex;
		sCropSetting.bEnable = pSenSorInfo[uiSensorIndex].bCropEnable[uiIMEIndex];
		sCropSetting.sCropInfo.x = pCropInfo[uiSensorIndex].sCropInfo[uiIMEIndex].sCropRECT.x;
		sCropSetting.sCropInfo.y = pCropInfo[uiSensorIndex].sCropInfo[uiIMEIndex].sCropRECT.y;
		sCropSetting.sCropInfo.w = pCropInfo[uiSensorIndex].sCropInfo[uiIMEIndex].sCropRECT.w;
		sCropSetting.sCropInfo.h = pCropInfo[uiSensorIndex].sCropInfo[uiIMEIndex].sCropRECT.h;
		sCropSetting.uiScaleHeight = pCropInfo[uiSensorIndex].sCropInfo[uiIMEIndex].uiScaleHeight;
		sCropSetting.uiScaleWidth = pCropInfo[uiSensorIndex].sCropInfo[uiIMEIndex].uiScaleWidth;
		////get Sensor Input size
		IPL_CUR_INPUT_INFO sIPLSensorInfo = {0};
		sIPLSensorInfo.id = uiSensorIndex;
		IPL_GetCmd(IPL_GET_IPL_CUR_INPUT_INFOR ,&sIPLSensorInfo);
		sCropSetting.uiSensorHeight = sIPLSensorInfo.in_size_v;
		sCropSetting.uiSensorWidth = sIPLSensorInfo.in_size_h;



		DBG_DUMP("sensor%d ime%d  sensor(%dx%d) crop info=%d %d %d %d %d %d %d\r\n",uiSensorIndex,
			        uiIMEIndex,
			        sCropSetting.uiSensorWidth,
			        sCropSetting.uiSensorHeight,
					sCropSetting.bEnable,
			        sCropSetting.sCropInfo.x,
			        sCropSetting.sCropInfo.y,
			        sCropSetting.sCropInfo.w,
			        sCropSetting.sCropInfo.h,
			        sCropSetting.uiScaleHeight,
			        sCropSetting.uiScaleWidth);
		NvtUctrl_SetConfigData((void *)&sCropSetting, sizeof(ISPImageCropSetting));

	#else
		DBG_ERR("not support!!\r\n");

	#endif
	return 0;
}

static INT32 param_hdr_get(INT32 argc, char* argv[]){

	#if (IPCAM_FUNC == ENABLE)
	if(argc!=1)
    {
        DBG_ERR("argc!=1 (%d)\r\n",argc);
        return -1;
    }
    UINT32 ipl_id = atoi(argv[0]);
	ISPHDRSetting sHDRSetting={0};
	UIAPPIPCAM_IPL_FUNC *pIPLFunc = UIAppIPCam_get_IPLFuncInfo();
	sHDRSetting.bHDREnable = pIPLFunc[ipl_id].sHDR.uiHDRMode;
	sHDRSetting.uiFrameNum = pIPLFunc[ipl_id].sHDR.uiFrameNum;
	NvtUctrl_SetConfigData((void *)&sHDRSetting, sizeof(ISPHDRSetting));
	#else
		DBG_ERR("not support!\r\n");
	#endif

    return 0;
}


#define IPLMODE_NEW DISABLE

#if (IPLMODE_NEW == ENABLE)


static INT32 param_hdr_set(INT32 argc, char* argv[]){

	#if(IPCAM_FUNC == ENABLE)
	if(argc!=1)
    {
        DBG_ERR("argc!=1 (%d)\r\n",argc);
        return -1;
    }
	#if(IPCAM_SHDR_FUNC == ENABLE)
		DBG_DUMP("new set hdr\r\n");
    	UINT32 ipl_id = atoi(argv[0]);
		ISPHDRSetting sHDRSetting={0};
		UINT32 i =0;
		IPCAM_STREAM_INFO *pStreamInfo = UIAppIPCam_get_streamInfo();

		UIAPPIPCAM_IPL_FUNC *pIPLFunc = UIAppIPCam_get_IPLFuncInfo();
		NvtUctrl_GetConfigData((void *)&sHDRSetting, sizeof(ISPHDRSetting));
		DBG_DUMP("set ipl_id=%d HDR:%d frameNum:%d\r\n",ipl_id,sHDRSetting.bHDREnable,sHDRSetting.uiFrameNum);


		UINT32 uiIPLFunc = ImageUnit_GetParam(ISF_IPL(ipl_id),ISF_CTRL,IMAGEPIPE_PARAM_FUNC);
		if(sHDRSetting.bHDREnable == SEL_SHDR_ON){
			DBG_DUMP("IPL%d HDR enable\r\n",ipl_id);
			uiIPLFunc |= IPL_FUNC_SHDR;
			uiIPLFunc = uiIPLFunc & (~IPL_FUNC_WDR);

		}
		else{

			#if(IPCAM_LCE == ENABLE || IPCAM_WDR_FUNC == ENABLE)
				uiIPLFunc |= IPL_FUNC_WDR;/// LCE need WDR func
			#endif
			uiIPLFunc = uiIPLFunc & (~IPL_FUNC_SHDR);
		}

		pIPLFunc[ipl_id].sHDR.uiHDRMode = sHDRSetting.bHDREnable;
		pIPLFunc[ipl_id].sHDR.uiFrameNum = sHDRSetting.uiFrameNum;
		ImageUnit_Begin(ISF_IPL(ipl_id), 0);
			ImageUnit_SetParam(ISF_CTRL, IMAGEPIPE_PARAM_FUNC, uiIPLFunc);
			ImageUnit_SetParam(ISF_CTRL, IPL_SEL_SHDR, pIPLFunc[ipl_id].sHDR.uiHDRMode);
			ImageUnit_SetParam(ISF_CTRL, IPL_SEL_SHDR_FRAME_NUM, pIPLFunc[ipl_id].sHDR.uiFrameNum);
		ImageUnit_End();


		for(i=0;i < IPCAM_PATH_MAX_ID;i++){
			if(pStreamInfo[i].sensor == (IPCAM_SENSOR)ipl_id && pStreamInfo[i].enable==IPCAM_STATE_ENABLE){
				ImageApp_IPCam_BeginStream(i);
				ImageApp_IPCam_ImagePipe_set_status(i,ISF_PORT_STATE_READY );
				//ImageApp_IPCam_reset_path(i);
				DBG_DUMP("ipl%d wait\r\n",ipl_id);
				break;
			}
		}

		//set ipl mode off
		ImageUnit_Begin(ISF_IPL(ipl_id), 0);
			ImageUnit_SetParam(ISF_CTRL, IMAGEPIPE_PARAM_MODE, IMAGEPIPE_MODE_DIRTY|IMAGEPIPE_MODE_OFF);
		ImageUnit_End();

		#if 1
		for(i=0;i < IPCAM_PATH_MAX_ID;i++){
			if(pStreamInfo[i].sensor == (IPCAM_SENSOR)ipl_id && pStreamInfo[i].enable==IPCAM_STATE_ENABLE){
				ImageApp_IPCam_ImagePipe_set_status(i,ISF_PORT_STATE_RUN );
				//ImageApp_IPCam_reset_path(i);
				DBG_DUMP("ipl%d off mode\r\n",ipl_id);
				break;
			}
		}
		#endif


		#if 1
		for(i=0;i < IPCAM_PATH_MAX_ID;i++){
			if(pStreamInfo[i].sensor == (IPCAM_SENSOR)ipl_id && pStreamInfo[i].enable==IPCAM_STATE_ENABLE){
				ImageApp_IPCam_ImagePipe_set_status(i,ISF_PORT_STATE_READY );
			//	ImageApp_IPCam_reset_path(i);

				break;
			}
		}
		//set ipl mode video
		ImageUnit_Begin(ISF_IPL(ipl_id), 0);
			ImageUnit_SetParam(ISF_CTRL, IMAGEPIPE_PARAM_MODE, IMAGEPIPE_MODE_DIRTY|IMAGEPIPE_MODE_VIDEO);
		ImageUnit_End();

		#endif
		for(i=0;i < IPCAM_PATH_MAX_ID;i++){
			if(pStreamInfo[i].sensor == (IPCAM_SENSOR)ipl_id && pStreamInfo[i].enable==IPCAM_STATE_ENABLE){
				ImageApp_IPCam_ImagePipe_set_status(i,ISF_PORT_STATE_RUN );
				ImageApp_IPCam_EndStream();
				ImageApp_IPCam_reset_path(i);
				DBG_DUMP("ipl%d video mode\r\n",ipl_id);
				break;
			}
		}

		//Ux_PostEvent(NVTEVT_SYSTEM_MODE, 1, System_GetState(SYS_STATE_CURRMODE));
	#else
		DBG_ERR("not Enable!\r\n");
	#endif
	#else
		DBG_ERR("not support!\r\n");
	#endif
    return 0;
}
#else
static INT32 param_hdr_set(INT32 argc, char* argv[]){

	#if(IPCAM_FUNC == ENABLE)
	if(argc!=1)
    {
        DBG_ERR("argc!=1 (%d)\r\n",argc);
        return -1;
    }
	#if(IPCAM_SHDR_FUNC == ENABLE)
		DBG_DUMP("old set hdr\r\n");
    	UINT32 ipl_id = atoi(argv[0]);
		ISPHDRSetting sHDRSetting={0};
		UINT32 i =0;
		IPCAM_STREAM_INFO *pStreamInfo = UIAppIPCam_get_streamInfo();

		UIAPPIPCAM_IPL_FUNC *pIPLFunc = UIAppIPCam_get_IPLFuncInfo();
		NvtUctrl_GetConfigData((void *)&sHDRSetting, sizeof(ISPHDRSetting));
		DBG_DUMP("set ipl_id=%d HDR:%d frameNum:%d\r\n",ipl_id,sHDRSetting.bHDREnable,sHDRSetting.uiFrameNum);


		UINT32 uiIPLFunc = ImageUnit_GetParam(ISF_IPL(ipl_id),ISF_CTRL,IMAGEPIPE_PARAM_FUNC);
		if(sHDRSetting.bHDREnable == SEL_SHDR_ON){
			DBG_DUMP("IPL%d HDR enable\r\n",ipl_id);
			uiIPLFunc |= IPL_FUNC_SHDR;
			uiIPLFunc = uiIPLFunc & (~IPL_FUNC_WDR);

		}
		else{

			#if(IPCAM_LCE == ENABLE || IPCAM_WDR_FUNC == ENABLE)
				uiIPLFunc |= IPL_FUNC_WDR;/// LCE need WDR func
			#endif
			uiIPLFunc = uiIPLFunc & (~IPL_FUNC_SHDR);
		}

		pIPLFunc[ipl_id].sHDR.uiHDRMode = sHDRSetting.bHDREnable;
		pIPLFunc[ipl_id].sHDR.uiFrameNum = sHDRSetting.uiFrameNum;
		ImageUnit_Begin(ISF_IPL(ipl_id), 0);
			ImageUnit_SetParam(ISF_CTRL, IMAGEPIPE_PARAM_FUNC, uiIPLFunc);
			ImageUnit_SetParam(ISF_CTRL, IPL_SEL_SHDR, pIPLFunc[ipl_id].sHDR.uiHDRMode);
			ImageUnit_SetParam(ISF_CTRL, IPL_SEL_SHDR_FRAME_NUM, pIPLFunc[ipl_id].sHDR.uiFrameNum);
		ImageUnit_End();


		for(i=0;i < IPCAM_PATH_MAX_ID;i++){
			if(pStreamInfo[i].sensor == (IPCAM_SENSOR)ipl_id && pStreamInfo[i].enable==IPCAM_STATE_ENABLE){
				ImageApp_IPCam_BeginStream(i);
				ImageApp_IPCam_ImagePipe_set_status(i,ISF_PORT_STATE_READY );
				ImageApp_IPCam_EndStream();
				ImageApp_IPCam_reset_path(i);

				DBG_DUMP("ipl%d wait\r\n",ipl_id);
				break;
			}
		}

		//set ipl mode off
		ImageUnit_Begin(ISF_IPL(ipl_id), 0);
			ImageUnit_SetParam(ISF_CTRL, IMAGEPIPE_PARAM_MODE, IMAGEPIPE_MODE_DIRTY|IMAGEPIPE_MODE_OFF);
		ImageUnit_End();

		#if 1
		for(i=0;i < IPCAM_PATH_MAX_ID;i++){
			if(pStreamInfo[i].sensor == (IPCAM_SENSOR)ipl_id && pStreamInfo[i].enable==IPCAM_STATE_ENABLE){
				ImageApp_IPCam_BeginStream(i);
				ImageApp_IPCam_ImagePipe_set_status(i,ISF_PORT_STATE_RUN );
				ImageApp_IPCam_EndStream();
				ImageApp_IPCam_reset_path(i);
				DBG_DUMP("ipl%d off mode\r\n",ipl_id);
				break;
			}
		}
		#endif


		#if 1
		for(i=0;i < IPCAM_PATH_MAX_ID;i++){
			if(pStreamInfo[i].sensor == (IPCAM_SENSOR)ipl_id && pStreamInfo[i].enable==IPCAM_STATE_ENABLE){
				ImageApp_IPCam_BeginStream(i);
				ImageApp_IPCam_ImagePipe_set_status(i,ISF_PORT_STATE_READY );
				ImageApp_IPCam_EndStream();
				ImageApp_IPCam_reset_path(i);

				break;
			}
		}
		//set ipl mode video
		ImageUnit_Begin(ISF_IPL(ipl_id), 0);
			ImageUnit_SetParam(ISF_CTRL, IMAGEPIPE_PARAM_MODE, IMAGEPIPE_MODE_DIRTY|IMAGEPIPE_MODE_VIDEO);
		ImageUnit_End();

		#endif
		for(i=0;i < IPCAM_PATH_MAX_ID;i++){
			if(pStreamInfo[i].sensor == (IPCAM_SENSOR)ipl_id && pStreamInfo[i].enable==IPCAM_STATE_ENABLE){
				ImageApp_IPCam_BeginStream(i);
				ImageApp_IPCam_ImagePipe_set_status(i,ISF_PORT_STATE_RUN );
				ImageApp_IPCam_EndStream();
				ImageApp_IPCam_reset_path(i);
				DBG_DUMP("ipl%d video mode\r\n",ipl_id);
				break;
			}
		}

		//Ux_PostEvent(NVTEVT_SYSTEM_MODE, 1, System_GetState(SYS_STATE_CURRMODE));
	#else
		DBG_ERR("not Enable!\r\n");
	#endif
	#else
		DBG_ERR("not support!\r\n");
	#endif
    return 0;
}

#endif



static INT32 param_IMEScale_get(INT32 argc, char* argv[]){

	#if(IPCAM_FUNC == ENABLE)
	if(argc!=1)
    {
        DBG_ERR("argc!=1 (%d)\r\n",argc);
        return -1;
    }
    UINT32 ipl_id = atoi(argv[0]);
	UIAPPIPCAM_IME_SCALE_TYPE *pIMEScaleInfo =  UIAppIPCam_get_IMEScaleInfo();
	ISPIMEScaleSetting IMEScaleInfo = {0};

	IMEScaleInfo.eMathodLarge = pIMEScaleInfo[ipl_id].eMathodLarge;
	IMEScaleInfo.eMathodSmall = pIMEScaleInfo[ipl_id].eMathodSmall;
	IMEScaleInfo.uiRatioThreshold = pIMEScaleInfo[ipl_id].uiRatioThreshold;
	NvtUctrl_SetConfigData((void *)&IMEScaleInfo, sizeof(ISPIMEScaleSetting));
	#else
	DBG_ERR("not support!\r\n");
	#endif

    return 0;
}

static INT32 param_IMEScale_set(INT32 argc, char* argv[]){

	#if(IPCAM_FUNC == ENABLE)
	if(argc!=1)
    {
        DBG_ERR("argc!=1 (%d)\r\n",argc);
        return -1;
    }


	IPCAM_STREAM_INFO *pStreamInfo = UIAppIPCam_get_streamInfo();
    UINT32 ipl_id = atoi(argv[0]);
	UIAPPIPCAM_IME_SCALE_TYPE *pIMEScaleInfo =  UIAppIPCam_get_IMEScaleInfo();
	ISPIMEScaleSetting IMEScaleInfo = {0};

	NvtUctrl_GetConfigData((void *)&IMEScaleInfo, sizeof(ISPIMEScaleSetting));

	pIMEScaleInfo[ipl_id].eMathodLarge = IMEScaleInfo.eMathodLarge;
	pIMEScaleInfo[ipl_id].eMathodSmall = IMEScaleInfo.eMathodSmall;
	pIMEScaleInfo[ipl_id].uiRatioThreshold = IMEScaleInfo.uiRatioThreshold;

	UINT32 i=0;
	for(i=0;i < IPCAM_PATH_MAX_ID;i++){
		if(pStreamInfo[i].sensor == ipl_id && pStreamInfo[i].enable==IPCAM_STATE_ENABLE){
			ImageApp_IPCam_BeginStream(i);
			ImageApp_IPCam_ImagePipe_set_status(i,ISF_PORT_STATE_READY );
			ImageApp_IPCam_EndStream();
			ImageApp_IPCam_reset_path(i);
		//	break;
		}
	}

	ImageUnit_Begin(ISF_IPL(ipl_id), 0);
		ImageUnit_SetParam(ISF_CTRL, IMAGEPIPE_PARAM_SCALE_QUALITY_RATIO, pIMEScaleInfo[ipl_id].uiRatioThreshold);
		ImageUnit_SetParam(ISF_CTRL, IMAGEPIPE_PARAM_SCALE_MATHOD_SMALL, pIMEScaleInfo[ipl_id].eMathodSmall);
		ImageUnit_SetParam(ISF_CTRL, IMAGEPIPE_PARAM_SCALE_MATHOD_LARGE, pIMEScaleInfo[ipl_id].eMathodLarge);
		ImageUnit_SetParam(ISF_CTRL, IMAGEPIPE_PARAM_MODE, IMAGEPIPE_MODE_DIRTY|IMAGEPIPE_MODE_VIDEO);
	ImageUnit_End();

	for(i=0;i < IPCAM_PATH_MAX_ID;i++){
		if(pStreamInfo[i].sensor == ipl_id && pStreamInfo[i].enable==IPCAM_STATE_ENABLE){
			ImageApp_IPCam_BeginStream(i);
			ImageApp_IPCam_ImagePipe_set_status(i,ISF_PORT_STATE_RUN );
			ImageApp_IPCam_EndStream();
			ImageApp_IPCam_reset_path(i);
		//	break;
		}
	}

	#else
	DBG_ERR("not support!\r\n");
	#endif

    return 0;
}



static INT32 param_begin(char *str, INT32 len)
{
	DBG_DUMP("get: %s\r\n", str);
	DBG_IND("get: %s\r\n", str);
	memset(pImageInfo, 0, sizeof(UCtrlImageInfoType));
	return 0;
}

static INT32 param_end(UTOKEN_END_INFO *p_info)
{
	char retStr[64];
	//AE_EXT_SET_INFO AeSetInfo = {0};
	IPL_PROC_ID ipl_id = 0;
	INT32 ipl_phy_id;
	IQ_LINUX_SET_INFO iq_linux_set_info = {0};
	//BOOL bRestart = FALSE;
	BOOL bUpdate = FALSE;
	if (p_info->err_code < 0) {
		snprintf(retStr, sizeof(retStr), "ERR: parm %s", p_info->failed_name);
		NvtUctrl_SetRetString(retStr);
		DBG_ERR("%s\r\n", retStr);
		return p_info->err_code;
	}

	// set channel ID
	if (pImageInfo->ipl_id.isSet) {
		ipl_id = pImageInfo->ipl_id.value;
	}
	ipl_phy_id = IPL_PATH(ipl_id);
	if (ipl_phy_id < 0)
		DBG_ERR("ipl_id=%d, phy_id=%d\r\n", ipl_id,ipl_phy_id);
	DBG_IND("ipl_id=%d\r\n", ipl_id);
	if (pImageInfo->backlight.isSet) {
		DBG_IND("backlight %d %d\r\n", pImageInfo->backlight.mode, pImageInfo->backlight.lv);
		iq_set_ui_info(ipl_phy_id, IQ_UI_BLC, pImageInfo->backlight.mode);
		iq_set_ui_info(ipl_phy_id, IQ_UI_BLC_LEVEL, pImageInfo->backlight.lv);
	}

	if (pImageInfo->brightness.isSet) {
		DBG_IND("brightness %d\r\n", pImageInfo->brightness.value);
		iq_linux_set_info.bri_ratio = pImageInfo->brightness.value;
		iq_set_linux_iq_info(ipl_phy_id, IQ_LINUX_SET_BRI, iq_linux_set_info);
	}

	if (pImageInfo->saturation.isSet) {
		DBG_IND("saturation %d\r\n", pImageInfo->saturation.value);
		iq_linux_set_info.sat_ratio = pImageInfo->saturation.value;
		iq_set_linux_iq_info(ipl_phy_id, IQ_LINUX_SET_SAT, iq_linux_set_info);
	}

	if (pImageInfo->contrast.isSet) {
		DBG_IND("contrast %d\r\n", pImageInfo->contrast.value);
		iq_linux_set_info.con_ratio = pImageInfo->contrast.value;
		iq_set_linux_iq_info(ipl_phy_id, IQ_LINUX_SET_CON, iq_linux_set_info);
	}

	if (pImageInfo->sharpness.isSet) {
		DBG_IND("sharpness %d\r\n", pImageInfo->sharpness.value);
		iq_linux_set_info.sharp_ratio = pImageInfo->sharpness.value;
		iq_set_linux_iq_info(ipl_phy_id, IQ_LINUX_SET_SHARP, iq_linux_set_info);
	}

	if (pImageInfo->hue.isSet) {
		DBG_IND("hue %d\r\n", pImageInfo->hue.value);
		iq_linux_set_info.hue_angle = pImageInfo->hue.value;
		iq_set_linux_iq_info(ipl_phy_id, IQ_LINUX_SET_HUE, iq_linux_set_info);
	}

	if (pImageInfo->nr2D.isSet) {
		DBG_IND("nr2D %d\r\n", pImageInfo->nr2D.value);
		iq_linux_set_info.nr_ratio = pImageInfo->nr2D.value;
		iq_set_linux_iq_info(ipl_phy_id, IQ_LINUX_SET_NR, iq_linux_set_info);
	}

	if (pImageInfo->nr3D.isSet) {
		DBG_IND("nr3D %d %d\r\n", pImageInfo->nr3D.mode, pImageInfo->nr3D.ratio);

#if 0
		iq_linux_set_info.tnr.mode = pImageInfo->nr3D.mode;
		iq_linux_set_info.tnr.ratio = pImageInfo->nr3D.ratio;
		iq_set_linux_iq_info(ipl_id, IQ_LINUX_SET_TNR, iq_linux_set_info);
#else//charlie test for 3DNR
#if(IPCAM_FUNC == ENABLE)
		#if (IPCAM_SQUARE_3DNR == ENABLE)
			UIAPPIPCAM_SENSOR_INFO *SensorInfo = UIAppIPCam_get_SensorInfo();
			if (SensorInfo == NULL) {
				DBG_ERR("get sensor info fail\r\n");
				return 0;
			}
			DBG_DUMP("IME 3DNR:%d\r\n", pImageInfo->nr3D.ratio);
			BOOL tmp_value = (pImageInfo->nr3D.ratio == 0 ? 0 : 1);
			if (tmp_value != SensorInfo[ipl_id].bIME3DNR) {
				SensorInfo[ipl_id].bIME3DNR = (pImageInfo->nr3D.ratio == 0 ? 0 : 1);
				bUpdate = TRUE;
				UINT32 uiIPLFunc = ImageUnit_GetParam(ISF_IPL(ipl_id),ISF_CTRL,IMAGEPIPE_PARAM_FUNC);
				if(SensorInfo[ipl_id].bIME3DNR == 1){
					uiIPLFunc |= IMAGEPIPE_FUNC_SQUARE;
				}
				else{
					uiIPLFunc =  uiIPLFunc & (~IMAGEPIPE_FUNC_SQUARE);
				}
				ImageUnit_Begin(ISF_IPL(ipl_id), 0);
					ImageUnit_SetParam(ISF_CTRL, IMAGEPIPE_PARAM_FUNC, uiIPLFunc);
				ImageUnit_End();

			}
		#endif
#endif


#endif
	}

	if (pImageInfo->LDC.isSet) {
		DBG_IND("LDC %d %d\r\n", pImageInfo->LDC.isEnable, pImageInfo->LDC.ratio);
		iq_linux_set_info.gdc.enable = pImageInfo->LDC.isEnable;
		iq_linux_set_info.gdc.ratio = pImageInfo->LDC.ratio;
		iq_set_linux_iq_info(ipl_phy_id, IQ_LINUX_SET_GDC, iq_linux_set_info);
	}

	if (pImageInfo->LSC.isSet) {
		DBG_IND("LSC %d %d %d %d\r\n", pImageInfo->LSC.isEnable, pImageInfo->LSC.ratio, pImageInfo->LSC.CenterXOffset, pImageInfo->LSC.CenterYOffset);
		iq_linux_set_info.vig.enable = pImageInfo->LSC.isEnable;
		iq_linux_set_info.vig.ratio = pImageInfo->LSC.ratio;                  // 0~100; default:0
		iq_linux_set_info.vig.center_x = pImageInfo->LSC.CenterXOffset;  // 0~100; default:50
		iq_linux_set_info.vig.center_y = pImageInfo->LSC.CenterYOffset;  // 0~100; default:50
		iq_set_linux_iq_info(ipl_phy_id, IQ_LINUX_SET_VIG, iq_linux_set_info);
	}

	//#NT#2017/10/05#Iyen Chiang -begin
	//#NT# Mark for not required, param_ae_set only
#if 0
	if (pImageInfo->ae.isSet) {
		DBG_IND("ae %d %d (%d %d) (%d %d) (%d %d) %d %d %d\r\n",  pImageInfo->ae.mode,    pImageInfo->ae.priority,
				pImageInfo->ae.maxexp,  pImageInfo->ae.minexp,
				pImageInfo->ae.maxgain, pImageInfo->ae.mingain,
				pImageInfo->ae.maxiris, pImageInfo->ae.miniris,
				pImageInfo->ae.exp, pImageInfo->ae.gain, pImageInfo->ae.iris);
		if (pImageInfo->ae.mode == 0) {
			AeSetInfo.ExpTBound.h = pImageInfo->ae.maxexp;
			AeSetInfo.ExpTBound.l = pImageInfo->ae.minexp;
			AeSetInfo.IsoBound.h = pImageInfo->ae.maxgain;
			AeSetInfo.IsoBound.l = pImageInfo->ae.mingain;
		} else {
			AeSetInfo.ExpT = pImageInfo->ae.exp;
			AeSetInfo.ISO = pImageInfo->ae.gain;
			AeSetInfo.IrisSetting.PIrisManualSize = pImageInfo->ae.iris;
		}
		AE_SetPLINEInfo(ipl_id, AeSetInfo);
	}
#endif
	//#NT#2017/10/05#Iyen Chiang -end

	if (pImageInfo->aewin.isSet) {
		DBG_IND("aewin %d %d %d %d\r\n", pImageInfo->aewin.left, pImageInfo->aewin.right, pImageInfo->aewin.top, pImageInfo->aewin.bottom);
	}

	if (pImageInfo->ircut.isSet) {
		DBG_IND("ircut %d\r\n", pImageInfo->ircut.value);
		UctrlMain_SendEvent(NVTEVT_EXE_MOVIE_IR_CUT, 1,  pImageInfo->ircut.value);
	}

	if (pImageInfo->wdr.isSet) {



		DBG_DUMP("wdr %d %d\r\n", pImageInfo->wdr.mode, pImageInfo->wdr.lv);

		#if(IPCAM_FUNC == ENABLE)
		#if (IPCAM_WDR_FUNC == ENABLE)
			UINT32 uiIPLFunc = ImageUnit_GetParam(ISF_IPL(ipl_id),ISF_CTRL,IMAGEPIPE_PARAM_FUNC);
			UIAPPIPCAM_IPL_FUNC *pIPLFuncInfo = UIAppIPCam_get_IPLFuncInfo();
			pIPLFuncInfo[ipl_id].sWDR.uiWDRLevel = pImageInfo->wdr.lv;
			pIPLFuncInfo[ipl_id].sWDR.uiWDRMode = pImageInfo->wdr.mode;

			#if 1
			if(uiIPLFunc & IPL_FUNC_WDR){
				if(pImageInfo->wdr.mode == SEL_WDR_OFF){
					bUpdate = TRUE;
				}
			}
			else{
				if(pImageInfo->wdr.mode != SEL_WDR_OFF){
					bUpdate = TRUE;
				}
			}
			#endif

			#if (IPCAM_LCE == ENABLE)
				uiIPLFunc |= IPL_FUNC_WDR;
				uiIPLFunc = uiIPLFunc & (~IPL_FUNC_SHDR);
			#endif

			ImageUnit_Begin(ISF_IPL(ipl_id), 0);
			if(pIPLFuncInfo[ipl_id].sWDR.uiWDRMode == SEL_WDR_OFF ){
				ImageUnit_SetParam(ISF_CTRL, IPL_SEL_WDR_IMM, SEL_WDR_OFF);

			}
			else if(pIPLFuncInfo[ipl_id].sWDR.uiWDRMode == SEL_WDR_AUTO){
				ImageUnit_SetParam(ISF_CTRL, IPL_SEL_WDR_IMM, SEL_WDR_AUTO);
				ImageUnit_SetParam(ISF_CTRL, IPL_SEL_WDR_M_LEVEL_IMM, pIPLFuncInfo[ipl_id].sWDR.uiWDRLevel);
				uiIPLFunc |= IPL_FUNC_WDR;
				uiIPLFunc = uiIPLFunc & (~IPL_FUNC_SHDR);

			}
			else if(pIPLFuncInfo[ipl_id].sWDR.uiWDRMode == SEL_WDR_MANUAL ){

				ImageUnit_SetParam(ISF_CTRL, IPL_SEL_WDR_IMM, SEL_WDR_MANUAL);
				ImageUnit_SetParam(ISF_CTRL, IPL_SEL_WDR_M_LEVEL_IMM, pIPLFuncInfo[ipl_id].sWDR.uiWDRLevel);
				uiIPLFunc |= IPL_FUNC_WDR;
				uiIPLFunc = uiIPLFunc & (~IPL_FUNC_SHDR);
			}
			else{
				DBG_ERR("set IPL%d WDR error (%d)\r\n",ipl_id,pIPLFuncInfo[ipl_id].sWDR.uiWDRMode);
			}
			ImageUnit_SetParam(ISF_CTRL, IMAGEPIPE_PARAM_FUNC, uiIPLFunc);
			DBG_DUMP("IPL func=%x\r\n",uiIPLFunc);
			ImageUnit_End();
		#endif
		#endif

	}

	if (pImageInfo->awb.isSet) {
		DBG_IND("awb %d %d %d\r\n", pImageInfo->awb.mode, pImageInfo->awb.rgain, pImageInfo->awb.bgain);
		AwbSetParam awbsetting;
		if (pImageInfo->awb.mode == 0) {
            awb_set_ui_info(ipl_phy_id, AWB_UI_WB, AWB_WB_AUTO);
            awbsetting.uiRratio = pImageInfo->awb.uiRatio;
            awbsetting.uiBratio = pImageInfo->awb.uiBatio;
            AWB_SetInfo(ipl_phy_id, AWB_SET_USER_RATIO, &awbsetting);
		} else if (pImageInfo->awb.mode == 1) {
            awb_set_ui_info(ipl_phy_id, AWB_UI_WB, AWB_WB_MGAIN);
			awbsetting.uiManualRGain = pImageInfo->awb.rgain;
			awbsetting.uiManualGGain = 256;
			awbsetting.uiManualBGain = pImageInfo->awb.bgain;
			AWB_SetInfo(ipl_phy_id, AWB_SET_GAIN, &awbsetting);
		}
	}

	if (pImageInfo->dis.isSet) {

		#if(IPCAM_FUNC == ENABLE )
			#if (IPCAM_DIS_MV_FUNC == ENABLE)
			bUpdate = TRUE;
			UIAPPIPCAM_IPL_FUNC *pIPLFuncInfo = UIAppIPCam_get_IPLFuncInfo();
			UINT32 uiIPLFunc = ImageUnit_GetParam(ISF_IPL(ipl_id),ISF_CTRL,IMAGEPIPE_PARAM_FUNC);
			ImageUnit_Begin(ISF_IPL(ipl_id), 0);
			if (pImageInfo->dis.mode  == 1) {
				pIPLFuncInfo[ipl_id].uiDIS =SEL_DISCOMP_ON;
				uiIPLFunc |=  IPL_FUNC_SIE_CH4_ETH;
				ImageUnit_SetParam(ISF_CTRL, IPL_SEL_DISCOMP_IMM, SEL_DISCOMP_ON);

			} else {
				pIPLFuncInfo[ipl_id].uiDIS =SEL_DISCOMP_OFF;
				uiIPLFunc = uiIPLFunc & (~IPL_FUNC_SIE_CH4_ETH);
				ImageUnit_SetParam(ISF_CTRL, IPL_SEL_DISCOMP_IMM, SEL_DISCOMP_OFF);
			}

				ImageUnit_SetParam(ISF_CTRL, IMAGEPIPE_PARAM_FUNC, (UINT32)uiIPLFunc);

			ImageUnit_End();

			#endif
		#endif
	}

	if (pImageInfo->defog.isSet) {
		#if (IPCAM_FUNC == ENABLE)
		bUpdate = TRUE;
		UINT32 uiIPLFunc = ImageUnit_GetParam(ISF_IPL(ipl_id),ISF_CTRL,IMAGEPIPE_PARAM_FUNC);
		UIAPPIPCAM_IPL_FUNC *pIPLFuncInfo = UIAppIPCam_get_IPLFuncInfo();
		DBG_IND("defog %d %d\r\n", pImageInfo->defog.mode, pImageInfo->defog.lv);

		pIPLFuncInfo[ipl_id].sDefog.eDeFogMode = pImageInfo->defog.mode;
		pIPLFuncInfo[ipl_id].sDefog.uiLevel = pImageInfo->defog.lv;
		#if 0
		if(uiIPLFunc & IPL_FUNC_DEFOG){
			if(pImageInfo->defog.mode == ISP_DEFOG_OFF){
				bRestart = TRUE;
			}
		}
		else{
			if(pImageInfo->defog.mode != ISP_DEFOG_OFF){
				bRestart = TRUE;
			}
		}
		#endif
		if(pImageInfo->defog.mode == SEL_DEFOG_ON){
			uiIPLFunc |= IPL_FUNC_DEFOG;

		}
		else{
			uiIPLFunc = uiIPLFunc & (~IPL_FUNC_DEFOG);
		}
		ImageUnit_Begin(ISF_IPL(ipl_id), 0);
			ImageUnit_SetParam(ISF_CTRL, IPL_SEL_DEFOG_IMM, pImageInfo->defog.mode);
			ImageUnit_SetParam(ISF_CTRL, IPL_SEL_DEFOG_LEVEL_IMM, pImageInfo->defog.lv);
			ImageUnit_SetParam(ISF_CTRL, IMAGEPIPE_PARAM_FUNC, (UINT32)uiIPLFunc);
		ImageUnit_End();
		#endif
	}

	if(bUpdate == TRUE){
		#if (IPCAM_FUNC == ENABLE)

		#if (IPCAM_DIS_MV_FUNC == ENABLE)
			if(pImageInfo->dis.isSet){
				IPCam_DIS_WaitSem();
			}
		#endif
		UINT32 i =0;
		IPCAM_STREAM_INFO *pStreamInfo = UIAppIPCam_get_streamInfo();

		ImageUnit_Begin(ISF_IPL(ipl_id), 0);
			ImageUnit_SetParam(ISF_CTRL, IMAGEPIPE_PARAM_MODE, IMAGEPIPE_MODE_DIRTY|IMAGEPIPE_MODE_PREVIEW);
		ImageUnit_End();


		for(i=0;i < IPCAM_PATH_MAX_ID;i++){
			if(pStreamInfo[i].sensor == (IPCAM_SENSOR)ipl_id && pStreamInfo[i].enable==IPCAM_STATE_ENABLE){
				ImageApp_IPCam_BeginStream(i);
				ImageApp_IPCam_ImagePipe_set_status(i,ISF_PORT_STATE_READY );
				ImageApp_IPCam_EndStream();
				ImageApp_IPCam_reset_path(i);
			//	break;
			}
		}
		for(i=0;i < IPCAM_PATH_MAX_ID;i++){
			if(pStreamInfo[i].sensor == (IPCAM_SENSOR)ipl_id && pStreamInfo[i].enable==IPCAM_STATE_ENABLE){
				ImageApp_IPCam_BeginStream(i);
				ImageApp_IPCam_ImagePipe_set_status(i,ISF_PORT_STATE_RUN );
				ImageApp_IPCam_EndStream();
				ImageApp_IPCam_reset_path(i);
			//	break;
			}
		}
		#if (IPCAM_DIS_MV_FUNC == ENABLE)
			if(pImageInfo->dis.isSet){
				IPCam_DIS_SigSem();
			}
		#endif
		#endif
	}

	#if 0
	if (bRestart == TRUE) {
		Ux_PostEvent(NVTEVT_SYSTEM_MODE, 1, System_GetState(SYS_STATE_CURRMODE));
	}
	#endif
	return 0;
}

static INT32 param_unknown(char *name)
{
	DBG_ERR("unknown param: %s\r\n", name);
	return 0;
}

UTOKEN_PARAM tbl_image_param[] = {
	{"channelset", param_ipl_id_set},       // [IPL ID set]
	{"bkc", param_backlight },
	{"bri", param_brightness},
	{"sat", param_saturation},
	{"con", param_contrast  },
	{"ae", param_ae        },
	{"aewin", param_aewin     },
	{"ircut", param_ircut     },
	{"wdr", param_wdr       },
	{"awb", param_awb       },
	{"dis", param_dis       },
	{"edge", param_sharpness },
	{"ImgSGet", param_image_setting_get},
	{"ImgSSet", param_image_setting_set},
	{"AEGet", param_ae_get},
	{"AESet", param_ae_set},
	{"AEStaGet", param_ae_status_get},
	{"WBGet", param_wb_get},
	{"WBSet", param_wb_set},
	{"WBStaGet", param_wb_status_get},
	{"WDRGet", param_wdr_get},
	{"WDRSet", param_wdr_set},
	{"NRGet", param_nr_get},
	{"NRSet", param_nr_set},
	{"DISGet", param_dis_get},
	{"DISSet", param_dis_set},
	{"LDCGet", param_ldc_get},
	{"LDCSet", param_ldc_set},
	{"LSCGet", param_lsc_get},
	{"LSCSet", param_lsc_set},
	{"DeFogGet", param_defog_get},
	{"DeFogSet", param_defog_set},
	{"GammaGet", param_gamma_get},
	{"GammaSet", param_gamma_set},
	{"CCMGet", param_ccm_get},
	{"CCMSet", param_ccm_set},
	{"IPLMapSet", param_iplmap_set},
	{"IPLMapGet", param_iplmap_get},
	{"DewarpSet", param_dewarp_set},
	{"CropGet",  param_crop_get},
	{"CropSet",  param_crop_set},
    {"HDRGet",param_hdr_get},
	{"HDRSet",param_hdr_set},
	{"IMEScaleSet",param_IMEScale_set},
	{"IMEScaleGet",param_IMEScale_get},
	{NULL, NULL}, //last tag, it must be
};

UTOKEN_MODULE module_image = {
	"uimage",
	tbl_image_param,
	param_begin,
	param_end,
	param_unknown,
	NULL
};


void UCtrlAppImage_Open(void)
{
//    clr_flg(UCTRL_IMAGE_FLG_ID, 0xFFFFFFFF);
}

void UCtrlAppImage_Close(void)
{
}






















#if SXCMD_TEST

static BOOL Cmd_uimage_set(CHAR *strCmd)
{
	uToken_Parse(strCmd, &module_image);
	return TRUE;
}

SXCMD_BEGIN(uimage, "uimage")
SXCMD_ITEM("set %", Cmd_uimage_set, "")
SXCMD_END()
#endif


