////////////////////////////////////////////////////////////////////////////////
#include "SysCommon.h"
//#include "AppCommon.h"
////////////////////////////////////////////////////////////////////////////////
#include "UIFramework.h"
#include "GxImage.h"
#include "UIAppPhoto.h"
#include "NVTUserCommand.h"
#include "ImageApp_Photo.h"
#include "UIPhotoMapping.h"
#include "UIPhotoInfo.h"
#include "UIDateImprint.h"
#include "UIAppCommon.h"
#include "UIAppPhoto.h"
#include "UIAppMovie.h" //for NVTEVT_EXE_MOVIEDIS, NVTEVT_EXE_MOVIEGDC, VTEVT_EXE_MOVIESMEAR
#include "UIInfo.h"
#include "UIMode.h"
#include "dis_alg_fw.h"

#include "UIStorageCheck.h"
//nclude "Demo_Sound.h"
//#include "FlowPanInit.h"
//#include "MenuDatetime.h"
#include "DateTimeInfo.h"
//#NT#2010/09/02#Niven Cho -begin
//#NT#NEW_DATE_IMPRINT
#include "UIDateImprint.h"
//#NT#2010/09/02#Niven Cho -end

#include "PStore.h"
#include "sensor.h"
#include "DxSensor.h"
#include "UIControlWnd.h"
#include "UIBackgroundObj.h"


#include "DxOutput.h"
#include "FileSysTsk.h"
#include "GxFlash.h"
#include "DeviceCtrl.h"
#include "GxFlash.h"
#include "ae_task.h"
#include "awb_task.h"
#include "PrjCfg.h"
#include "SysMain.h"

//for IPL DZoom - end
#if (LENS_FUNCTION == ENABLE)
#include "Lens.h"
#include "af_api.h"
#include "af_lib.h"
#include "af_task.h"
#endif

#include "SoundData.h"
#include "SizeConvert.h"

//#if (PIP_VIEW_FUNC == ENABLE)
#include "AppDisp_PipView.h"
//#endif
#if(WIFI_FUNC==ENABLE)
#include "UIModeWifi.h"
#endif
#if (WIFI_AP_FUNC==ENABLE)
#include "WifiAppCmd.h"
#include "WifiAppXML.h"
#include "UIAppWiFiCmd.h"
#endif
#include "UIPhotoFuncInfo.h"
#include "LviewNvtAPI.h"
#include "lviewd.h"
#if !defined(_Gyro_None_)
#include "rsc_api.h"
#endif
#include "ImageUnit_VdoIn.h"
#include "ImageUnit_VdoEnc.h"
#include "ImageUnit_VdoOut.h"
#include "ImageUnit_NetHTTP.h"
#include "ImageUnit_UserProc.h"
#include "UCtrlAppPhoto.h"
#include "dma.h"
#include "MovieStamp.h"
#include "PrjCfg.h"
#include "nvtmpp.h"
#include "iq_ui_info.h"
#include "ae_ui_info.h"
#include "af_ui_info.h"
#include "awb_ui_info.h"
#if (VIDEO_FUNC_MJPG == ENABLE)
#include "VideoCodec_MJPG.h"
#endif
#if (VIDEO_FUNC_H264 == ENABLE)
#include "VideoCodec_H264.h"
#endif
#if (VIDEO_FUNC_H265 == ENABLE)
#include "VideoCodec_H265.h"
#endif
#include "videosprite.h"

#define THIS_DBGLVL         2 // 0=FATAL, 1=ERR, 2=WRN, 3=UNIT, 4=FUNC, 5=IND, 6=MSG, 7=VALUE, 8=USER
///////////////////////////////////////////////////////////////////////////////
#define __MODULE__          UiAppPhoto
#define __DBGLVL__          ((THIS_DBGLVL>=PRJ_DBG_LVL)?THIS_DBGLVL:PRJ_DBG_LVL)
#define __DBGFLT__          "*" //*=All, [mark]=CustomClass
//#define __DBGFLT__          "[cap]"
//#define __DBGFLT__          "[cb]"
//#define __DBGFLT__          "[set]"
//#define __DBGFLT__          "[zoom]"
//#define __DBGFLT__          "[af]"
#include "DebugModule.h"

extern void System_DispSourceByTarget(ISIZE *pImgSize, USIZE *pAspectRatio);
extern UINT32 System_GetEnableDisp(void);
extern UINT32 System_GetEnableSensor(void);

extern SX_CMD_ENTRY uiphoto[];


//---------------------UIAPP_PHOTO Global Variables----------



BOOL _g_bFirstPhoto = TRUE;


#ifndef PHOTODISTSK_ENABLE
#define PHOTODISTSK_ENABLE   0
#endif

static PHOTO_CAP_FOLDER_NAMING gPhoto_Folder_Naming = {
	PHOTO_CAP_ID_1,             //cap_id
	IPL_PATH_1,                 //ipl_id
	"Photo"                    //folder_path
};


#define PHOTO_ROOT_PATH         "A:\\Novatek\\"


#define CFG_JPG_HEADER_SIZE (64*1024)       //16K
#define CFG_JPG_SCREENNAIL_SIZE (128*1024)  //128K

#if (_SCREENNAIL_SIZE_ == _SCREENNAIL_SIZE_VGA_)
#define    CFG_SCREENNAIL_SIZE  PHOTO_SIZE_VGA
#define    CFG_SCREENNAIL_W     640
#define    CFG_SCREENNAIL_H     480
#else
#define    CFG_SCREENNAIL_SIZE  PHOTO_SIZE_3M
#define    CFG_SCREENNAIL_W     2048
#define    CFG_SCREENNAIL_H     1536
#endif

static UINT32 gPhotoDzoomStop = 0;


UIAppPhotoExeInfoType UIAppPhotoExeInfoInitVal = {
	0,                                //FreePicNum;
	FALSE,                            //isFolderFull;
	FALSE,                            //isCardFull;
	FALSE,                           //isFocusEnd
	FALSE,                           //isStartCapture
	FALSE,                           //isContShotStateSetOn;
	FALSE,                           //isStopingContShot
	FALSE,                           //isDoingContShot;
	FALSE,                           //isStartSmileDetect;
	0,                               //uiTakePicNum;
	FALSE,                           // isAscnOn;
	FALSE,                           // isCafOn;
	FALSE,                           // isSdOn;
	FALSE,                           // isFdOn;
	FALSE,                           // IsJPGok
	FALSE,                           // IsAFProcess;              //
	FALSE,                           //BDstatus
	FALSE,                           //IsCallBack
	UIAPPPHOTO_CB_MAX,               //CallBackEvent
	FALSE,                           //isPhotoFrameOn
	FALSE,                           //isFlashTrigger
	FALSE,                           //isSingleViewOpen
	FALSE,                           //isPrvPostProcess
	FALSE,                           //isAFBeam
	0,                               //uiMaxImageSize
	FALSE,                           //isDzoomStart
	UI_ZOOM_CTRL_STOP,               //ZoomCtrl
	FALSE,                           //isStartHighSpeedTone
	{0, 0, 640, 480},                //FdDispCoord
	0,                               //sensorCount
	0,                               //DualCam
};

static UIAppPhotoExeInfoType UIAppPhotoExeInfo;
static UIAppPhotoExeInfoType *localInfo = &UIAppPhotoExeInfo;
UIAppPhotoExeInfoType *pPhotoExeInfo = &UIAppPhotoExeInfo;

static USIZE g_photo_ImageRatioSize;

PhotoFrameInfoType      gPFInfo = {0};


#if DEWARP_FUNC
static USIZE IMAGERATIO_SIZE[IMAGERATIO_MAX_CNT] = {
	{1, 1}, //IMAGERATIO_9_16
	{1, 1}, //IMAGERATIO_2_3
	{1, 1}, //IMAGERATIO_3_4
	{1, 1}, //IMAGERATIO_1_1
	{1, 1}, //IMAGERATIO_4_3
	{1, 1}, //IMAGERATIO_3_2
	{1, 1}, //IMAGERATIO_16_9
};
#else
static USIZE IMAGERATIO_SIZE[IMAGERATIO_MAX_CNT] = {
	{9, 16}, //IMAGERATIO_9_16
	{2, 3}, //IMAGERATIO_2_3
	{3, 4}, //IMAGERATIO_3_4
	{1, 1}, //IMAGERATIO_1_1
	{4, 3}, //IMAGERATIO_4_3
	{3, 2}, //IMAGERATIO_3_2
	{16, 9}, //IMAGERATIO_16_9
};
#endif

//---------------------UIAPP_PHOTO Local Functions-------------------------
//return Primary Size or Header+Primary+Screennail Size
static UINT32 PhotoExe_GetExpectSize_RhoBRCrtl(UINT32 ImgIdx, BOOL bPrimaryOnly);

void AF_LOCK(BOOL bLock)
{
	// AF just support AF_ID_1
	if (System_GetEnableSensor() & SENSOR_1) {
#if _TODO
		ImageUnit_Begin(ISF_IPL(0), 0);
		//ImageUnit_SetParam(ISF_CTRL, IMAGEPIPE_PARAM_LOCKAF, (bLock));
		ImageUnit_End();
#endif
	}
}

BOOL AF_ISLOCK(void)
{
	// AF just support AF_ID_1
	if (System_GetEnableSensor() & SENSOR_1) {
#if _TODO
		//return ImageUnit_GetParam(ISF_IPL(0), ISF_CTRL, IMAGEPIPE_PARAM_LOCKAF);
#else
		return FALSE;
#endif
	} else {
		return FALSE;
	}
}

void AF_WAIT_IDLE(void)
{
	// AF just support AF_ID_1
	if (System_GetEnableSensor() & SENSOR_1) {
#if _TODO
		//if (ImageUnit_GetParam(ISF_IPL(0), ISF_CTRL, IMAGEPIPE_PARAM_WAITAF) != 0) {
		if(0) {
			DBG_ERR("WAITAF\r\n");
		}
#endif
	}
}
void PhotoExe_IPL_SetUIInfo(UINT32 index, UINT32 value)
{
	UINT32 i;

	for (i=0;i<PHOTO_VID_IN_MAX;i++){
		if (System_GetEnableSensor() & (SENSOR_1 << i)){
			ImageUnit_Begin(ISF_IPL(i), 0);
			ImageUnit_SetParam(ISF_CTRL, index, value);
			ImageUnit_End();
		}
	}
	if (index == IPL_SEL_SHDR) {
		DBG_IND("HDR %d\r\n", value);
	} else if (index == IPL_SEL_WDR_IMM) {
		DBG_IND("WDR %d\r\n", value);
	}
}


void PhotoExe_IQ_SetUIInfo(UINT32 index, UINT32 value)
{
	UINT32 i;

	for (i=0;i<IPL_ID_MAX_NUM;i++){
		if (System_GetEnableSensor() & (SENSOR_1 << i)){
			iq_set_ui_info(IPL_PATH(i),index,value);
		}
	}
}

void PhotoExe_AE_SetUIInfo(UINT32 index, UINT32 value)
{
	UINT32 i;

	for (i=0;i<IPL_ID_MAX_NUM;i++){
		if (System_GetEnableSensor() & (SENSOR_1 << i)){
			ae_set_ui_info(IPL_PATH(i),index,value);
		}
	}
}

void PhotoExe_AF_SetUIInfo(UINT32 index, UINT32 value)
{
	UINT32 i;

	for (i=0;i<IPL_ID_MAX_NUM;i++){
		if (System_GetEnableSensor() & (SENSOR_1 << i)){
			af_set_ui_info(IPL_PATH(i),index,value);
		}
	}
}

void PhotoExe_AWB_SetUIInfo(UINT32 index, UINT32 value)
{
	UINT32 i;

	for (i=0;i<IPL_ID_MAX_NUM;i++){
		if (System_GetEnableSensor() & (SENSOR_1 << i)){
			awb_set_ui_info(IPL_PATH(i),index,value);
		}
	}
}

void PhotoExe_Cap_SetUIInfo(UINT32 sensor_id, UINT32 index, UINT32 value)
{
    ImageUnit_Begin(&ISF_Cap, 0);
	ImageUnit_SetParam(IPL_PATH(sensor_id) + ISF_IN1, index, value);
	ImageUnit_End();
}

void PhotoExe_RSC_SetSwitch(UINT32 index, UINT32 value)
{
#if RSC_FUNC
	if (System_GetEnableSensor() & SENSOR_1) {
		rsc_ext_setswitch(IPL_PATH(0), index, value);
	}
	DBG_IND("RSC index = %d , value %d\r\n", index, value);
#endif
}

void PhotoExe_IPL_AlgSetFD_AEInfo(UINT32 value)
{
	// sensor is not init
	if (System_GetEnableSensor() == 0)
		return;

	if (System_GetEnableSensor() & SENSOR_1) {
		ImageUnit_Begin(ISF_IPL(0), 0);
		ImageUnit_SetParam(ISF_CTRL,  IPL_SEL_FD_IMM, SEL_FD_OFF);
		ImageUnit_End();
	}
	if (System_GetEnableSensor() & SENSOR_2) {
		ImageUnit_Begin(ISF_IPL(1), 0);
		ImageUnit_SetParam(ISF_CTRL,  IPL_SEL_FD_IMM, SEL_FD_OFF);
		ImageUnit_End();
	}
	if (localInfo->DualCam == DUALCAM_FRONT || localInfo->DualCam == DUALCAM_BOTH) {
		ImageUnit_Begin(ISF_IPL(0), 0);
		ImageUnit_SetParam(ISF_CTRL,  IPL_SEL_FD_IMM, value);
		ImageUnit_End();
	} else {
		ImageUnit_Begin(ISF_IPL(1), 0);
		ImageUnit_SetParam(ISF_CTRL,  IPL_SEL_FD_IMM, value);
		ImageUnit_End();
	}
}
//---------------------UIAPP_PHOTO MISC-------------------------


static ER PhotoExe_LensZoomIn(void)
{
	DBG_IND("[zoom]\r\n");
#if (LENS_FUNCTION == ENABLE)
	return Lens_ZoomIn();
#else
	return E_OK;
#endif

}

static ER PhotoExe_LensZoomOut(void)
{
	DBG_IND("[zoom]\r\n");
#if (LENS_FUNCTION == ENABLE)
	return Lens_ZoomOut();
#else
	return E_OK;
#endif
}

static ER PhotoExe_LensZoomStop(void)
{
	DBG_IND("[zoom]\r\n");
#if (LENS_FUNCTION == ENABLE)
	return Lens_ZoomStop();
#else
	return E_OK;
#endif
}

static ER PhotoExe_LensReset(void)
{
	DBG_IND("[zoom]\r\n");
#if (LENS_FUNCTION == ENABLE)
	return Lens_Reset();
#else
	return E_OK;
#endif
}

UINT32 PhotoExe_GetMaxOzoomIdx(void)
{
#if (LENS_FUNCTION == ENABLE)
	return Lens_ZoomGetSection(ZOOM_MAX_SECTION);
#else
	return 1;
#endif
}

UINT32 PhotoExe_GetMinOzoomIdx(void)
{
#if (LENS_FUNCTION == ENABLE)
	return Lens_ZoomGetSection(ZOOM_MIN_SECTION);
#else
	return 1;
#endif
}


UINT32 PhotoExe_GetCurOzoomIdx(void)
{
#if (LENS_FUNCTION == ENABLE)
	return Lens_ZoomGetSection(ZOOM_CURR_SECTION);
#else
	return 1;
#endif
}

UINT32 PhotoExe_GetMaxDzoomIdx(void)
{
	UINT32 max;

	if (localInfo->DualCam == DUALCAM_FRONT || localInfo->DualCam == DUALCAM_BOTH) {
		ImageUnit_GetZoom_MaxIdx(ISF_IPL(0), &max);
	} else {
		ImageUnit_GetZoom_MaxIdx(ISF_IPL(1), &max);
	}
	DBG_MSG("DZ: get MaxIdx=%d\r\n", max);
	return max;
}

UINT32 PhotoExe_GetDZoomIdx(void)
{
	UINT32 index;
	if (localInfo->DualCam == DUALCAM_FRONT || localInfo->DualCam == DUALCAM_BOTH) {
		ImageUnit_GetZoom_CurIdx(ISF_IPL(0), &index);
	} else {
		ImageUnit_GetZoom_CurIdx(ISF_IPL(1), &index);
	}
	DBG_MSG("DZ: get CurIdx=%d\r\n", index);
	return index;
}

UINT32 PhotoExe_GetDZoomRatio(void)
{
	UINT32 ratio;
	if (localInfo->DualCam == DUALCAM_FRONT || localInfo->DualCam == DUALCAM_BOTH) {
		ImageUnit_GetZoom_Ratio(ISF_IPL(0), &ratio);
	} else {
		ImageUnit_GetZoom_Ratio(ISF_IPL(1), &ratio);
	}
	DBG_MSG("DZ: get Ratio=%d\r\n", ratio);
	return ratio;
}


static void PhotoExe_DZoomIn(void)
{
	DBG_MSG("DZ: STOP=0\r\n");
	gPhotoDzoomStop = FALSE;
	AppBKW_SetData(BKW_DZOOM_STOP, (UINT32)&gPhotoDzoomStop);
	BKG_PostEvent(NVTEVT_BKW_DZOOM_IN);
}

static void PhotoExe_DZoomOut(void)
{
	DBG_MSG("DZ: STOP=0\r\n");
	gPhotoDzoomStop = FALSE;
	AppBKW_SetData(BKW_DZOOM_STOP, (UINT32)&gPhotoDzoomStop);
	BKG_PostEvent(NVTEVT_BKW_DZOOM_OUT);
}

static void PhotoExe_DZoomStop(void)
{
	DBG_MSG("DZ: STOP=1\r\n");
	gPhotoDzoomStop = TRUE;
}


void PhotoExe_DZoomReset(void)
{
	if (localInfo->DualCam == DUALCAM_FRONT || localInfo->DualCam == DUALCAM_BOTH) {
		ImageUnit_SetZoom_CurIdx(ISF_IPL(0), DZOOM_IDX_MIN());
	} else {
		ImageUnit_SetZoom_CurIdx(ISF_IPL(1), DZOOM_IDX_MIN());
	}
	DBG_MSG("DZ: Reset CurIdx=%d\r\n", 0);
}

void PhotoExe_DZoomInBK(UINT32 *pIsDzoomStop)
{
	UINT32    index;
	DBG_MSG("DZ: IN start\r\n");
	index = DZOOM_IDX_GET();
	while (!(*pIsDzoomStop) && index < DZOOM_IDX_MAX()) {
		if (localInfo->DualCam == DUALCAM_FRONT || localInfo->DualCam == DUALCAM_BOTH) {
			ImageUnit_SetZoom_CurIdx(ISF_IPL(0), ++index);
		} else {
			ImageUnit_SetZoom_CurIdx(ISF_IPL(1), ++index);
		}
		Delay_DelayMs(33); //delay 1 VD to see new frame between dzoom steps
		DBG_MSG("DZ: IN CurIdx=%d\r\n", index);
	}
	DBG_MSG("DZ: IN stop\r\n");
}

void PhotoExe_DZoomOutBK(UINT32 *pIsDzoomStop)
{
	UINT32    index;
	DBG_MSG("DZ: OUT start\r\n");
	index = DZOOM_IDX_GET();
	while (!(*pIsDzoomStop) && index > DZOOM_IDX_MIN()) {
		if (localInfo->DualCam == DUALCAM_FRONT || localInfo->DualCam == DUALCAM_BOTH) {
			ImageUnit_SetZoom_CurIdx(ISF_IPL(0), --index);
		} else {
			ImageUnit_SetZoom_CurIdx(ISF_IPL(1), --index);
		}
		Delay_DelayMs(33); //delay 1 VD to see new frame between dzoom steps
		DBG_MSG("DZ: OUT CurIdx=%d\r\n", index);
	}
	DBG_MSG("DZ: OUT stop\r\n");
}


UINT32 PhotoExe_GetFdFaceNum(void)
{
#if _FD_FUNC_
	return FD_GetRsltFaceNum();
#else
	return 0;
#endif
}


static USIZE PhotoExe_RatioSizeConvert(USIZE *devSize, USIZE *devRatio, USIZE *Imgratio)
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

static UINT32 PhotoExe_GetScreenNailSize(void)
{
	UINT32           uiImageSize, ScreenNailSize;
	UINT32           BitStreamSize;


	ScreenNailSize = CFG_SCREENNAIL_SIZE;
	uiImageSize = UI_GetData(FL_PHOTO_SIZE);
	if (uiImageSize < ScreenNailSize) {
		BitStreamSize = CFG_SCREENNAIL_W * CFG_SCREENNAIL_H / 4;
		/*
		if (uiImageSize == SIZE_12M32)
		{
		    BitStreamSize = BitStreamSize*2/3*4/3;

		}
		*/
#if 0
		if (uiImageSize == PHOTO_SIZE_2MHD) {
			BitStreamSize = BitStreamSize * 9 / 16 * 4 / 3;
		}
#endif
	} else {
		BitStreamSize = 0;
	}
	//#NT#2016/04/26#Lincy Lin -begin
	//#NT#Support sidebyside capture mode
	if (UI_GetData(FL_CONTINUE_SHOT) == CONTINUE_SHOT_SIDE) {
		BitStreamSize = BitStreamSize << 1;
	}
	//#NT#2016/04/26#Lincy Lin -end
	DBG_IND("[cap]ScreenNail BitStreamSize =%d K\r\n", BitStreamSize / 1024);
	return BitStreamSize;
}


static void PhotoExe_SetScreenNailSize(UINT32 sensor_id)
{
	UINT32           uiImageSize, ScreenNailSize;
	USIZE            devRatio = {4, 3};
	USIZE            ImageRatioSize = {0}, BufferSize = {0};
	USIZE            ImgSize = {0};
	UINT32           ImageRatioIdx, BitStreamSize;

	ScreenNailSize = CFG_SCREENNAIL_SIZE;
	uiImageSize = UI_GetData(FL_PHOTO_SIZE);
	if (uiImageSize < ScreenNailSize) {
		//BufferSize.w = GetPhotoSizeWidth(ScreenNailSize);
		//BufferSize.h = GetPhotoSizeHeight(ScreenNailSize);
		BufferSize.w = CFG_SCREENNAIL_W;
		BufferSize.h = BufferSize.w * 3 / 4;

		ImageRatioIdx = GetPhotoSizeRatio(UI_GetData(FL_PHOTO_SIZE));
		ImageRatioSize = IMAGERATIO_SIZE[ImageRatioIdx];
		ImgSize = PhotoExe_RatioSizeConvert(&BufferSize, &devRatio, &ImageRatioSize);
		BitStreamSize = PhotoExe_GetScreenNailSize();

		PhotoExe_Cap_SetUIInfo(sensor_id, CAP_SEL_SCREEN_IMG, SEL_SCREEN_IMG_ON);
		//#NT#2016/04/26#Lincy Lin -begin
		//#NT#Support sidebyside capture mode
		if (UI_GetData(FL_CONTINUE_SHOT) == CONTINUE_SHOT_SIDE) {
			ImgSize.w = ImgSize.w << 1;
		}
		//#NT#2016/04/26#Lincy Lin -end
		PhotoExe_Cap_SetUIInfo(sensor_id, CAP_SEL_SCREEN_IMG_H_SIZE, ImgSize.w);
		PhotoExe_Cap_SetUIInfo(sensor_id, CAP_SEL_SCREEN_IMG_V_SIZE, ImgSize.h);
		PhotoExe_Cap_SetUIInfo(sensor_id, CAP_SEL_SCREEN_FMT, IPL_IMG_Y_PACK_UV422);
		PhotoExe_Cap_SetUIInfo(sensor_id, CAP_SEL_SCREENBUFSIZE, BitStreamSize);
		DBG_IND("[cap]ScreenNail w=%d,h=%d, buffSize=%d k\r\n", ImgSize.w, ImgSize.h, BitStreamSize / 1024);

	} else {
		PhotoExe_Cap_SetUIInfo(sensor_id, CAP_SEL_SCREEN_IMG, SEL_SCREEN_IMG_OFF);
		PhotoExe_Cap_SetUIInfo(sensor_id, CAP_SEL_SCREENBUFSIZE, 0);
		DBG_IND("[cap]ScreenNail None\r\n");
	}
}


static void PhotoExe_SetQuickViewSize(UINT32 sensor_id)
{
	USIZE            devRatio = {4, 3};
	USIZE            ImageRatioSize = {0}, BufferSize = {0};
	USIZE            ImgSize = {0};
	UINT32           ImageRatioIdx;
	ISIZE            DevSize = {0};

	DevSize = GxVideo_GetDeviceSize(DOUT1);
#if (_QUICKVIEW_SIZE_ == _QUICKVIEW_SIZE_VGA_)
	{
#if 0
		UINT32 QuickViewSize = SIZE_VGA;

		BufferSize.w = GetPhotoSizeWidth(QuickViewSize);
		BufferSize.h = GetPhotoSizeHeight(QuickViewSize);
#else
		// fix bug: w/h size will be wrong if no VGA/3M resolution!
		BufferSize.w = CFG_SCREENNAIL_W;
		BufferSize.h = CFG_SCREENNAIL_H;
#endif

		if (BufferSize.w > (UINT32)DevSize.w) {
			BufferSize.w = DevSize.w;
			BufferSize.h = BufferSize.w * devRatio.h / devRatio.w;
		}
	}
#else
	{
		BufferSize.w = DevSize.w;
		BufferSize.h = BufferSize.w * devRatio.h / devRatio.w;
	}
#endif

	ImageRatioIdx = GetPhotoSizeRatio(UI_GetData(FL_PHOTO_SIZE));
	ImageRatioSize = IMAGERATIO_SIZE[ImageRatioIdx];
	ImgSize = PhotoExe_RatioSizeConvert(&BufferSize, &devRatio, &ImageRatioSize);


#if (_QUICKVIEW_SIZE_ != _QUICKVIEW_SIZE_VGA_)
	//#NT#2013/07/19#Lincy Lin -begin
	//#NT#Fine tune TV NTSC/PAL quick view display quality
	{

		SIZECONVERT_INFO     CovtInfo = {0};
		USIZE                dev1Ratio;
		USIZE                tmpImgSize = {0};

		dev1Ratio = GxVideo_GetDeviceAspect(DOUT1);

		CovtInfo.uiSrcWidth  = ImgSize.w;
		CovtInfo.uiSrcHeight = ImgSize.h;
		CovtInfo.uiDstWidth  = DevSize.w;
		CovtInfo.uiDstHeight = DevSize.h;
		CovtInfo.uiDstWRatio = dev1Ratio.w;
		CovtInfo.uiDstHRatio = dev1Ratio.h;
		//#NT#2016/03/29#Lincy Lin -begin
		//#NT#Fix the photo quick view display error when image size is 16:9 with rotate panel
		CovtInfo.alignType = SIZECONVERT_ALIGN_FLOOR_16;
		//#NT#2016/03/29#Lincy Lin -end
		DisplaySizeConvert(&CovtInfo);

		DBG_IND("[cap]CovtInfo Srcw=%d,h=%d, Dstw=%d ,h=%d, Ratiow=%d ,h=%d, OutW=%d,h=%d\r\n", CovtInfo.uiSrcWidth, CovtInfo.uiSrcHeight, CovtInfo.uiDstWidth, CovtInfo.uiDstHeight, CovtInfo.uiDstWRatio, CovtInfo.uiDstHRatio, CovtInfo.uiOutWidth, CovtInfo.uiOutHeight);
		DBG_IND("[cap]QuickView w=%d,h=%d, DevSize w=%d ,h=%d\r\n", ImgSize.w, ImgSize.h, DevSize.w, DevSize.h);
		//#NT#2016/03/29#Lincy Lin -begin
		//#NT#Fix the photo quick view display error when image size is 16:9 with rotate panel
		if (ImgSize.h != CovtInfo.uiOutHeight) {
			tmpImgSize.h = CovtInfo.uiOutHeight;
			// image size should not exceed video size
			if (tmpImgSize.h > ImgSize.h) {
				tmpImgSize.h = ImgSize.h;
			}
			tmpImgSize.w = tmpImgSize.h * ImgSize.w / ImgSize.h;
			tmpImgSize.w = ALIGN_CEIL_16(tmpImgSize.w);
			ImgSize = tmpImgSize;
		}
		//#NT#2016/03/29#Lincy Lin -end
	}
#endif
	//#NT#2013/07/19#Lincy Lin -end

	//#NT#2016/04/26#Lincy Lin -begin
	//#NT#Support sidebyside capture mode
	if (UI_GetData(FL_CONTINUE_SHOT) == CONTINUE_SHOT_SIDE) {
		ImgSize.w = ImgSize.w << 1;
	}
	//#NT#2016/04/26#Lincy Lin -end
	PhotoExe_Cap_SetUIInfo(sensor_id, CAP_SEL_QV_IMG_H_SIZE, ImgSize.w);
	PhotoExe_Cap_SetUIInfo(sensor_id, CAP_SEL_QV_IMG_V_SIZE, ImgSize.h);
	PhotoExe_Cap_SetUIInfo(sensor_id, CAP_SEL_QV_FMT, IPL_IMG_Y_PACK_UV422);
	DBG_IND("[cap]QuickView w=%d,h=%d, DevSize w=%d ,h=%d\r\n", ImgSize.w, ImgSize.h, DevSize.w, DevSize.h);
}

//return Primary Size or Header+Primary+Screennail Size
static UINT32 PhotoExe_GetExpectSize_RhoBRCrtl(UINT32 ImgIdx, BOOL bPrimaryOnly)
{
	//#NT#2016/04/26#Lincy Lin -begin
	//#NT#Support sidebyside capture mode
	UINT32 BitstreamSize;

	if (UI_GetData(FL_CONTINUE_SHOT) == CONTINUE_SHOT_SIDE) {
		BitstreamSize = GetPhotoSizeWidth(ImgIdx) * 2 * GetPhotoSizeHeight(ImgIdx);
	} else {
		BitstreamSize = GetPhotoSizeWidth(ImgIdx) * GetPhotoSizeHeight(ImgIdx);
	}
	//#NT#2016/04/26#Lincy Lin -end
	switch (UI_GetData(FL_QUALITY)) {
	case QUALITY_SUPER:
		BitstreamSize = (UINT32)(BitstreamSize * 30 / 100);
		break;

	case QUALITY_FINE:
		BitstreamSize = (UINT32)(BitstreamSize * 25 / 100);
		break;

	case QUALITY_NORMAL:
	default:
		BitstreamSize = (UINT32)(BitstreamSize * 20 / 100);
	}

	if (!bPrimaryOnly) {
		BitstreamSize += CFG_JPG_HEADER_SIZE + PhotoExe_GetScreenNailSize();
	}

	return BitstreamSize;

}

//
UINT32 PhotoExe_GetFreePicNum(void)
{
#if 1
	UINT64              uiFreeSpace;
	UINT32              uiMaxImageSize;
	UINT32              freeImgNum;
	UINT32              reserveSize = 0x80000; //  500KB
	UINT32              CaptureSize;
	UINT32              size;

#if (FILESIZE_ALIGN_FUNC)
	reserveSize += FS_ALIGN_RESERVED_SIZE;
#endif
	uiFreeSpace = FileSys_GetDiskInfo(FST_INFO_FREE_SPACE);

	DBG_IND("[cap]Free Space = %d KB\r\n", uiFreeSpace / 1024);

	size = UI_GetData(FL_PHOTO_SIZE);

	CaptureSize = PhotoExe_GetExpectSize_RhoBRCrtl(size, FALSE);
	DBG_IND("[cap]CaptureSize = %d K \r\n", CaptureSize / 1024);

	if (uiFreeSpace < reserveSize) {
		freeImgNum = 0;
	} else {
		freeImgNum = (uiFreeSpace - reserveSize) / CaptureSize;
	}

	DBG_IND("[cap]Free ImgNum = %d\r\n", freeImgNum);

	uiMaxImageSize = CaptureSize + reserveSize;
	//uiMaxImageSize = CaptureSize+(CaptureSize*15/100); //+15%

	localInfo->uiMaxImageSize = uiMaxImageSize;

	DBG_IND("[cap]uiMaxImageSize = %ld k\r\n", uiMaxImageSize / 1024);

	//#NT#store in info
	localInfo->FreePicNum = freeImgNum;
	if (localInfo->FreePicNum > 0) {
		localInfo->isCardFull = FALSE;
	}
	return freeImgNum;
#else
	return 100;
#endif
}



//#NT#2010/09/03#Niven Cho -begin
//#NT#NEW_DATE_IMPRINT
//void PhotoExe_DateImprint(void)
//#NT#2010/09/03#Niven Cho -end


static BOOL PhotoExe_CheckBD(void)
{
	return FALSE;
}

void PhotoExe_GetDispCord(URECT *dispCord)
{
	UINT32 ImageRatioIdx = 0;
	USIZE  ImageRatioSize = {0};
	URECT  DispCoord;
	ISIZE  dev1size;
	USIZE  dev1Ratio;
	USIZE  finalSize = {0};

	ImageRatioIdx = GetPhotoSizeRatio(UI_GetData(FL_PHOTO_SIZE));
	ImageRatioSize = IMAGERATIO_SIZE[ImageRatioIdx];

	//1.get current device size (current mode)
	dev1size = GxVideo_GetDeviceSize(DOUT1);
	//2.get current device aspect Ratio
	dev1Ratio = GxVideo_GetDeviceAspect(DOUT1);
	finalSize = PhotoExe_RatioSizeConvert((USIZE *)&dev1size, &dev1Ratio, &ImageRatioSize);
	DispCoord.w = finalSize.w;
	DispCoord.h = finalSize.h;
	if (finalSize.w == (UINT32)dev1size.w) {
		DispCoord.x = 0;
		DispCoord.y = (dev1size.h - finalSize.h) >> 1;
	} else {
		DispCoord.y = 0;
		DispCoord.x = (dev1size.w - finalSize.w) >> 1;

	}
	*dispCord = DispCoord;
}


static void PhotoExe_CallBackUpdateInfo(UINT32 callBackEvent)
{
	VControl *pCurrnetWnd;

	Ux_GetFocusedWindow(&pCurrnetWnd);
	localInfo->IsCallBack = TRUE;
	localInfo->CallBackEvent = callBackEvent;
	//NVTEVT_UPDATE_INFO
	Ux_SendEvent(pCurrnetWnd, NVTEVT_UPDATE_INFO, 1, localInfo->CallBackEvent);
	// callback to uctrl
#if(UCTRL_FUNC==ENABLE)
	UCtrlAppPhoto_CallBack(callBackEvent);
#endif
	localInfo->IsCallBack = FALSE;
}


static void PhotoExe_StopContShot(void)
{
	if (localInfo->isStopingContShot == FALSE) {
		localInfo->isStopingContShot = TRUE;
		ImageApp_Photo_CapStop();
		PhotoExe_CallBackUpdateInfo(UIAPPPHOTO_CB_STOP_CONTSHOT);
	}
}

static UINT32 PhotoExe_GetMultiSensor_trigger_dispID(void)
{
#if defined(_BSP_NA51000_)
	if (localInfo->sensorCount == 2)
		return PHOTO_DISP_ID_2;
    else
		return PHOTO_DISP_ID_4;
#else
	return PHOTO_DISP_ID_2;
#endif
}

static UINT32 PhotoExe_GetMultiSensor_trigger_strmID(void)
{
#if defined(_BSP_NA51000_)
	if (localInfo->sensorCount == 2)
		return PHOTO_STRM_ID_2;
    else
		return PHOTO_STRM_ID_4;
#else
	return PHOTO_STRM_ID_2;
#endif
}

#if (SBS_VIEW_FUNC == ENABLE)
static void PhotoExe_2sensor_sbsview(UINT32 iPort)
{
	// PIP view
	ISF_DATA         isf_data1, isf_data2, isf_new_data;
	ISF_UNIT         *pUnit;
	ISF_PORT         *pSrc1, *pSrc2;
	IMG_BUF          dst_img;
	UINT32           trigger_strm_id;
	UINT32           trigger_disp_id;
	UINT32           img_buf_size , buff_addr, dst_w, dst_h, dst_pxlfmt;
	APPDISP_VIEW_DRAW         pip_draw;
	APPDISP_VIEW_DRAW         *pdraw = &pip_draw;

	trigger_disp_id = PhotoExe_GetMultiSensor_trigger_dispID();
	trigger_strm_id = PhotoExe_GetMultiSensor_trigger_strmID();
	pUnit = &ISF_UserProc;

#if NVT_DEPTH_VIEW
	// depth sbs view need to show in depth photo function
	if (iPort == ISF_IN1 + trigger_disp_id)
		return;
#endif

	if (iPort == ISF_IN1 + trigger_disp_id) {
		pSrc1 = ImageUnit_Out(pUnit, ISF_OUT1 + PHOTO_DISP_ID_1);
		pSrc2 = ImageUnit_Out(pUnit, ISF_OUT1 + PHOTO_DISP_ID_2);
	} else if (iPort == ISF_IN1 + trigger_strm_id) {
		pSrc1 = ImageUnit_Out(pUnit, ISF_OUT1 + PHOTO_STRM_ID_1);
		pSrc2 = ImageUnit_Out(pUnit, ISF_OUT1 + PHOTO_STRM_ID_2);
	} else {
		return;
	}

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
		ImageUnit_GetVdoImgSize(pUnit, pSrc1->oPort,&dst_w, &dst_h);
        dst_pxlfmt = pdraw->p_src_img[0]->PxlFmt;
		img_buf_size = GxImg_CalcRequireSize(dst_w, dst_h, dst_pxlfmt, GXIMAGE_LINEOFFSET_ALIGN(16));
		buff_addr = ImageUnit_NewData(img_buf_size, &isf_new_data);
		if (buff_addr == 0) {
			DBG_ERR("Get buffer fail\r\n");
			ImageUnit_ReleaseData(&isf_data1);
			ImageUnit_ReleaseData(&isf_data2);
			return;
		}
		GxImg_InitBuf(&dst_img, dst_w, dst_h, dst_pxlfmt, GXIMAGE_LINEOFFSET_ALIGN(16), buff_addr, img_buf_size);
		memcpy(&isf_new_data.Desc[0], &dst_img, sizeof(IMG_BUF));
		pdraw->p_dst_img = &dst_img;
		AppDispView_OnDraw(pdraw);
		ImageUnit_PushData(pSrc1, (ISF_DATA *)&isf_new_data, 0);
		ImageUnit_ReleaseData(&isf_data1);
		ImageUnit_ReleaseData(&isf_data2);
		ImageUnit_ReleaseData(&isf_new_data);
	}
}
#else
static void PhotoExe_2sensor_pipview(UINT32 iPort)
{
	// PIP view
	ISF_DATA         isf_data1, isf_data2;
	ISF_UNIT         *pUnit;
	ISF_PORT         *pSrc1, *pSrc2;
	UINT32           trigger_strm_id;
	UINT32           trigger_disp_id;
	APPDISP_VIEW_DRAW         pip_draw;
	APPDISP_VIEW_DRAW         *pdraw = &pip_draw;

	trigger_disp_id = PhotoExe_GetMultiSensor_trigger_dispID();
	trigger_strm_id = PhotoExe_GetMultiSensor_trigger_strmID();
	pUnit = &ISF_UserProc;
	if (iPort == ISF_IN1 + trigger_disp_id) {
		pSrc1 = ImageUnit_Out(pUnit, ISF_OUT1 + PHOTO_DISP_ID_1);
		pSrc2 = ImageUnit_Out(pUnit, ISF_OUT1 + PHOTO_DISP_ID_2);
	} else if (iPort == ISF_IN1 + trigger_strm_id) {
		pSrc1 = ImageUnit_Out(pUnit, ISF_OUT1 + PHOTO_STRM_ID_1);
		pSrc2 = ImageUnit_Out(pUnit, ISF_OUT1 + PHOTO_STRM_ID_2);
	} else {
		return;
	}

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
		AppDispView_OnDraw(pdraw);
        ImageUnit_PushData(pSrc1, (ISF_DATA *)&isf_data1, 0);
		ImageUnit_ReleaseData(&isf_data1);
		ImageUnit_ReleaseData(&isf_data2);
	}
}
#endif
#if defined(_BSP_NA51000_)
static void PhotoExe_4sensor_view(UINT32 iPort)
{
	// PIP view
	ISF_DATA         isf_data1, isf_data2, isf_data3, isf_data4, isf_new_data;
	ISF_UNIT         *pUnit;//, *pDstUnit;
	ISF_PORT         *pSrc1, *pSrc2, *pSrc3, *pSrc4;
	IMG_BUF          dst_img;
	UINT32           trigger_strm_id;
	UINT32           trigger_disp_id;
	UINT32           img_buf_size , buff_addr, dst_w, dst_h, dst_pxlfmt;

	APPDISP_VIEW_DRAW         pip_draw;
	APPDISP_VIEW_DRAW         *pdraw = &pip_draw;


	trigger_disp_id = PhotoExe_GetMultiSensor_trigger_dispID();
	trigger_strm_id = PhotoExe_GetMultiSensor_trigger_strmID();
	pUnit = &ISF_UserProc;
	if (iPort == ISF_IN1 + trigger_disp_id) {
		pSrc1 = ImageUnit_Out(pUnit, ISF_OUT1 + PHOTO_DISP_ID_1);
		pSrc2 = ImageUnit_Out(pUnit, ISF_OUT1 + PHOTO_DISP_ID_2);
		pSrc3 = ImageUnit_Out(pUnit, ISF_OUT1 + PHOTO_DISP_ID_3);
		pSrc4 = ImageUnit_Out(pUnit, ISF_OUT1 + PHOTO_DISP_ID_4);
	} else if (iPort == ISF_IN1 + trigger_strm_id) {
		pSrc1 = ImageUnit_Out(pUnit, ISF_OUT1 + PHOTO_STRM_ID_1);
		pSrc2 = ImageUnit_Out(pUnit, ISF_OUT1 + PHOTO_STRM_ID_2);
		pSrc3 = ImageUnit_Out(pUnit, ISF_OUT1 + PHOTO_STRM_ID_3);
		pSrc4 = ImageUnit_Out(pUnit, ISF_OUT1 + PHOTO_STRM_ID_4);

	} else {
		return;
	}
	if (ImageUnit_IsAllowPull(pSrc1) && ImageUnit_IsAllowPull(pSrc2) && ImageUnit_IsAllowPull(pSrc3) && ImageUnit_IsAllowPull(pSrc4) ){
		if (ImageUnit_PullData(pSrc1, (ISF_DATA *)&isf_data1, 0) != ISF_OK) {
			return;
		}
		if (ImageUnit_PullData(pSrc2, (ISF_DATA *)&isf_data2, 0) != ISF_OK) {
			ImageUnit_ReleaseData(&isf_data1);
			return;
		}
		if (ImageUnit_PullData(pSrc3, (ISF_DATA *)&isf_data3, 0) != ISF_OK) {
			ImageUnit_ReleaseData(&isf_data1);
			ImageUnit_ReleaseData(&isf_data2);
			return;
		}
		if (ImageUnit_PullData(pSrc4, (ISF_DATA *)&isf_data4, 0) != ISF_OK) {
			ImageUnit_ReleaseData(&isf_data1);
			ImageUnit_ReleaseData(&isf_data2);
			ImageUnit_ReleaseData(&isf_data3);
			return;
		}
		pdraw->p_src_img[0] = (IMG_BUF *)&isf_data1.Desc[0];
		pdraw->p_src_img[1] = (IMG_BUF *)&isf_data2.Desc[0];
		pdraw->p_src_img[2] = (IMG_BUF *)&isf_data3.Desc[0];
		pdraw->p_src_img[3] = (IMG_BUF *)&isf_data4.Desc[0];
		ImageUnit_GetVdoImgSize(pUnit, pSrc1->oPort,&dst_w, &dst_h);
		dst_pxlfmt = pdraw->p_src_img[0]->PxlFmt;
		img_buf_size = GxImg_CalcRequireSize(dst_w, dst_h, dst_pxlfmt, GXIMAGE_LINEOFFSET_ALIGN(16));
		buff_addr = ImageUnit_NewData(img_buf_size, &isf_new_data);
		if (buff_addr == 0) {
			DBG_ERR("Get buffer fail\r\n");
			ImageUnit_ReleaseData(&isf_data1);
			ImageUnit_ReleaseData(&isf_data2);
			ImageUnit_ReleaseData(&isf_data3);
			ImageUnit_ReleaseData(&isf_data4);
			return;
		}
		isf_new_data.TimeStamp = isf_data1.TimeStamp;
		GxImg_InitBuf(&dst_img, dst_w, dst_h, dst_pxlfmt, GXIMAGE_LINEOFFSET_ALIGN(16), buff_addr, img_buf_size);
		memcpy(&isf_new_data.Desc[0], &dst_img, sizeof(IMG_BUF));
		pdraw->p_dst_img = &dst_img;
		AppDispView_OnDraw(pdraw);
		ImageUnit_PushData(pSrc1, (ISF_DATA *)&isf_new_data, 0);
		ImageUnit_ReleaseData(&isf_data1);
		ImageUnit_ReleaseData(&isf_data2);
		ImageUnit_ReleaseData(&isf_data3);
		ImageUnit_ReleaseData(&isf_data4);
		ImageUnit_ReleaseData(&isf_new_data);
	}
}
#endif

#define HTTP_PATH_ID  2
static void NetHTTP_DataStampCB(UINT32 yAddr, UINT32 cbAddr, UINT32 crAddr, UINT32 lineY)
{
    if(System_GetState(SYS_STATE_CURRSUBMODE) == SYS_SUBMODE_WIFI)
    {
        if(MovieStamp_IsEnable()==FALSE)
            MovieStamp_Enable();
        else
        {
            UINT32 imgHeight = (cbAddr - yAddr) / lineY;
            //mjpeg use gragh
            //videosprite_draw_btn_ext(HTTP_PATH_ID, yAddr, cbAddr, crAddr, lineY, lineY, imgHeight);
            videosprite_draw_ext(HTTP_PATH_ID, yAddr, cbAddr, crAddr, lineY, lineY, imgHeight);

            MovieStamp_CopyData(HTTP_PATH_ID, yAddr, cbAddr, crAddr, lineY, lineY);
        }
    }
}
static void PhotoExe_UserProc_ProcessCB(UINT32 iPort)
{
	if (localInfo->sensorCount == 2) {
		#if (SBS_VIEW_FUNC == ENABLE)
		PhotoExe_2sensor_sbsview(iPort);
		#else
		PhotoExe_2sensor_pipview(iPort);
		#endif
    }
#if defined(_BSP_NA51000_)
	else if (localInfo->sensorCount == 4) {
		PhotoExe_4sensor_view(iPort);
    }
#endif
	else {
        DBG_ERR("Not support\r\n");
	}
}
static USERPROC_TRIG_PROC_VAL PhotoExe_UserProc_PushCB(ISF_PORT *pPort, ISF_DATA *pData)
{
	UINT32 trigger_strm_id;
	UINT32 trigger_disp_id;

	trigger_disp_id = PhotoExe_GetMultiSensor_trigger_dispID();
	trigger_strm_id = PhotoExe_GetMultiSensor_trigger_strmID();

	// PIP view display trigger process
	if (pPort && pPort->iPort == ISF_IN1 + trigger_disp_id) {
		return USERPROC_TRIG_PROC_EN;
	}
	// PIP view HTTP streaming trigger process
	if (pPort && pPort->iPort == ISF_IN1 + trigger_strm_id) {
		return USERPROC_TRIG_PROC_EN;
	}
	return USERPROC_TRIG_PROC_NONE;
}

static INT32 PhotoExe_InitCommonMem(void)
{

	NVTMPP_ER        ret;
	NVTMPP_VB_CONF_S st_conf;
	ISIZE            disp_size={0};

	// get current device size (current mode)
	if (System_GetEnableDispCount()==2){
		disp_size = GxVideo_GetDeviceSize(DOUT2);
	}else{
		disp_size = GxVideo_GetDeviceSize(DOUT1);
	}
	memset(&st_conf, 0, sizeof(NVTMPP_VB_CONF_S));
	st_conf.ddr_mem[0].Addr = OS_GetMempoolAddr(POOL_ID_APP);
	st_conf.ddr_mem[0].Size = OS_GetMempoolSize(POOL_ID_APP);
	st_conf.max_pool_cnt = 64;

#if defined(_BSP_NA51023_)
	st_conf.common_pool[0].blk_size = 1920 * 1080 * 3 / 2 + 1024;
#if (SENSOR_CAPS_COUNT == 1)
	st_conf.common_pool[0].blk_cnt = 2;
#else
	st_conf.common_pool[0].blk_cnt = 4;
#endif
	st_conf.common_pool[0].ddr = NVTMPP_DDR_1;
	st_conf.common_pool[1].blk_size = 868 * 480 * 3 / 2 + 1024;
	st_conf.common_pool[1].blk_cnt = 5;
	st_conf.common_pool[1].ddr = NVTMPP_DDR_1;
	st_conf.common_pool[2].blk_size = disp_size.w * disp_size.h * 3 / 2 + 1024;
	st_conf.common_pool[2].blk_cnt = 5;
	st_conf.common_pool[2].ddr = NVTMPP_DDR_1;
#else
#if (SENSOR_CAPS_COUNT == 1)
	#if (DEWARP_FUNC)
	st_conf.common_pool[0].blk_size = 2880 * 720 * 3 / 2 + 1024;
	st_conf.common_pool[0].blk_cnt = 2;
	st_conf.common_pool[0].ddr = NVTMPP_DDR_1;
	#else
	st_conf.common_pool[0].blk_size = 1920 * 1080 * 3 / 2 + 1024;
	st_conf.common_pool[0].blk_cnt = 2;
	st_conf.common_pool[0].ddr = NVTMPP_DDR_1;
	#endif
	st_conf.common_pool[1].blk_size = disp_size.w * disp_size.h * 3 / 2 + 1024;
	st_conf.common_pool[1].blk_cnt = 4;
	st_conf.common_pool[1].ddr = NVTMPP_DDR_1;
	if (System_GetEnableDispCount()==2 && GxVideo_GetDevice(DOUT2)){
        	ISIZE disp2_size={0};
		disp2_size = GxVideo_GetDeviceSize(DOUT1);
		st_conf.common_pool[2].blk_size = disp2_size.w * disp2_size.h * 3 / 2 + 1024;
		st_conf.common_pool[2].blk_cnt = 4;
		st_conf.common_pool[2].ddr = NVTMPP_DDR_1;
	}
#elif (SENSOR_CAPS_COUNT == 2)
	st_conf.common_pool[0].blk_size = 1920 * 1080 * 3 / 2 + 1024;
	st_conf.common_pool[0].blk_cnt = 2;
	st_conf.common_pool[0].ddr = NVTMPP_DDR_1;
	st_conf.common_pool[1].blk_size = 868 * 480 * 3 / 2 + 1024;
	st_conf.common_pool[1].blk_cnt = 4;
	st_conf.common_pool[1].ddr = NVTMPP_DDR_1;
	st_conf.common_pool[2].blk_size = (868*2) * 480 * 3 / 2 + 1024;
	st_conf.common_pool[2].blk_cnt = 4;
	st_conf.common_pool[2].ddr = NVTMPP_DDR_1;
	st_conf.common_pool[3].blk_size = disp_size.w * disp_size.h * 3 / 2 + 1024;
	st_conf.common_pool[3].blk_cnt = 4;
	st_conf.common_pool[3].ddr = NVTMPP_DDR_1;
	st_conf.common_pool[4].blk_size = disp_size.w/2 * disp_size.h * 3 / 2 + 1024;
	st_conf.common_pool[4].blk_cnt = 4;
	st_conf.common_pool[4].ddr = NVTMPP_DDR_1;
#else
    st_conf.common_pool[0].blk_size = 1920 * 1080 * 3 / 2 + 1024;
	st_conf.common_pool[0].blk_cnt = 2*4;
	st_conf.common_pool[0].ddr = NVTMPP_DDR_1;
	st_conf.common_pool[1].blk_size = 864 * 480 * 3 / 2 + 1024;
	st_conf.common_pool[1].blk_cnt = 3;
	st_conf.common_pool[1].ddr = NVTMPP_DDR_1;
	st_conf.common_pool[2].blk_size = (864/2) * (480/2) * 3 / 2 + 1024;
	st_conf.common_pool[2].blk_cnt = 2*4;
	st_conf.common_pool[2].ddr = NVTMPP_DDR_1;
	st_conf.common_pool[3].blk_size = disp_size.w * disp_size.h * 3 / 2 + 1024;
	st_conf.common_pool[3].blk_cnt = 3;
	st_conf.common_pool[3].ddr = NVTMPP_DDR_1;
	st_conf.common_pool[4].blk_size = (disp_size.w/2) * (disp_size.h/2) * 3 / 2 + 1024;
	st_conf.common_pool[4].blk_cnt = 2*4;
	st_conf.common_pool[4].ddr = NVTMPP_DDR_1;
#endif
#endif

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

static INT32 PhotoExe_ExitCommonMem(void)
{
	NVTMPP_ER        ret;
	ret = nvtmpp_vb_exit();
	if (NVTMPP_ER_OK != ret) {
		DBG_ERR("nvtmpp exit vb err: %d\r\n", ret);
		return -1;
	}
	return 0;
}

static void PhotoExe_InitPhotoFunc(void)
{

	PHOTO_FUNC_INFO *phead = NULL;
	phead = &PhotoFuncInfo_dummy;
	phead->pNext = NULL; //mark end
	alg_Photo_InitPhotoFunc(phead);
#if _FD_FUNC_
	// Register FD callback func
	{
		URECT fdDispCoord = {0};
		//FD_RegisterCB(Photo_FdCB);
		ISIZE dev1size;
		//1.get current device size (current mode)
		dev1size = GxVideo_GetDeviceSize(DOUT1);
		// Set FD display coordinate
		fdDispCoord.w = dev1size.w;
		fdDispCoord.h = dev1size.h;
		//FD_SetDispCoord(&fdDispCoord);
		localInfo->FdDispCoord = fdDispCoord;
	}
#endif
#if 0 //move to PhotoExe_InitIPL
	//open
#if (MOVIE_DIS_COMP_FUNC == ENABLE)
	// force to disable DIS
	PhotoExe_IPL_SetUIInfo(IPL_SEL_DISCOMP_IMM, SEL_DISCOMP_ON);
#endif

#if RSC_FUNC
	if (UI_GetData(FL_RSC) == RSC_ON) {
		PhotoExe_RSC_SetSwitch(SEL_RSC_CHANGEMODE, SEL_RSC_ON);
	} else {
		PhotoExe_RSC_SetSwitch(SEL_RSC_CHANGEMODE, SEL_RSC_OFF);
	}
#endif
#endif
	UIPhotoFunc_Open(phead);

}





static void PhotoExe_InitFileNaming(void)
{
	MEM_RANGE               filedb_Pool = {0};

#if USE_FILEDB
	UI_SetData(FL_IsUseFileDB, 1);
#else
	UI_SetData(FL_IsUseFileDB, 0);
#endif

	filedb_Pool.Addr = dma_getCacheAddr(OS_GetMempoolAddr(POOL_ID_FILEDB));
	filedb_Pool.Size = POOL_SIZE_FILEDB;
	ImageApp_Photo_Config(PHOTO_CFG_FBD_POOL, (UINT32)&filedb_Pool);

	ImageApp_Photo_Config(PHOTO_CFG_ROOT_PATH, (UINT32)PHOTO_ROOT_PATH);

	// Folder Naming
	if (System_GetEnableSensor() & SENSOR_1) {
		gPhoto_Folder_Naming.cap_id = PHOTO_CAP_ID_1;
		gPhoto_Folder_Naming.ipl_id = IPL_PATH(0);
		ImageApp_Photo_Config(PHOTO_CFG_FOLDER_NAME, (UINT32)&gPhoto_Folder_Naming);
	}
	if (System_GetEnableSensor() & SENSOR_2) {
		gPhoto_Folder_Naming.cap_id = PHOTO_CAP_ID_2;
		gPhoto_Folder_Naming.ipl_id = IPL_PATH(1);
		ImageApp_Photo_Config(PHOTO_CFG_FOLDER_NAME, (UINT32)&gPhoto_Folder_Naming);
	}
	if (System_GetEnableSensor() & SENSOR_3) {
		gPhoto_Folder_Naming.cap_id = PHOTO_CAP_ID_3;
		gPhoto_Folder_Naming.ipl_id = IPL_PATH(2);
		ImageApp_Photo_Config(PHOTO_CFG_FOLDER_NAME, (UINT32)&gPhoto_Folder_Naming);
	}
	if (System_GetEnableSensor() & SENSOR_4) {
		gPhoto_Folder_Naming.cap_id = PHOTO_CAP_ID_4;
		gPhoto_Folder_Naming.ipl_id = IPL_PATH(3);
		ImageApp_Photo_Config(PHOTO_CFG_FOLDER_NAME, (UINT32)&gPhoto_Folder_Naming);
	}
}


static UINT32 PhotoExe_InitSensorCount(void)
{
    //
	if (System_GetEnableSensor() == (SENSOR_1)) {
		localInfo->sensorCount = 1;
	} else if (System_GetEnableSensor() == (SENSOR_2)) {
		localInfo->sensorCount = 1;
	} else if (System_GetEnableSensor() == (SENSOR_1 | SENSOR_2)) {
		localInfo->sensorCount = 2;
	}
#if defined(_BSP_NA51000_)
	else if (System_GetEnableSensor() == (SENSOR_1 | SENSOR_2 | SENSOR_3 | SENSOR_4)) {
		localInfo->sensorCount = 4;
	}
#endif

	//localInfo->sensorCount  = 1;
	DBG_IND("sensorCount = %d, FL_DUAL_CAM = %d\r\n", localInfo->sensorCount,UI_GetData(FL_DUAL_CAM));

	// load sensor display option
	if (localInfo->sensorCount < 2) {
		if (System_GetEnableSensor() == (SENSOR_1)) {
			localInfo->DualCam = DUALCAM_FRONT;
		} else {
			localInfo->DualCam = DUALCAM_BEHIND;
		}
	} else {
		//localInfo->DualCam = UI_GetData(FL_DUAL_CAM);
		localInfo->DualCam = DUALCAM_BOTH;
	}
#if (PIP_VIEW_FUNC == ENABLE)
	PipView_SetStyle(UI_GetData(FL_DUAL_CAM));
#endif
    DBG_IND("localInfo->DualCam = %d\r\n", localInfo->DualCam);
    return localInfo->DualCam;
}

static void PhotoExe_InitIPL(USIZE *pImageRatioSize)
{
	UIAPP_PHOTO_SENSOR_INFO *pSensorInfo;
	UINT32 sensor_index = 0;
	UINT32 IPL_FUNC=0;
	UINT32 IPL_FUNC2=0;
	UINT32 FrontFPS=0;

    for (sensor_index=0;sensor_index<localInfo->sensorCount;sensor_index++) {
		///get senser info
		pSensorInfo = UIAppPhoto_get_SensorInfo(sensor_index);
		if (pSensorInfo == NULL) {
			DBG_ERR("get pSensorInfo error\r\n");
			return;
		}
		ImageUnit_Begin(ISF_IPL(sensor_index), 0);
			/////set IPL SIZE
			ImageUnit_SetVdoImgSize(ISF_IN1, pSensorInfo->sSize.w, pSensorInfo->sSize.h); //user perferred IPL input w,h (= main stream size)
			/////set IPL FPS front
			FrontFPS = pSensorInfo->fps;
			ImageUnit_SetVdoFramerate(ISF_IN1, ISF_VDO_FRC(FrontFPS,1)); //user perferred IPL input fps (= main stream fps)
			/////set IPL ratio
			ImageUnit_SetVdoAspectRatio(ISF_IN1, pImageRatioSize->w, pImageRatioSize->h);

			///set IME 3DNR
			if (pSensorInfo->bIME3DNR == TRUE) {
				DBG_DUMP("enable sensor %d IME_3dnr\r\n", sensor_index);
				IPL_FUNC |= IMAGEPIPE_FUNC_SQUARE;
			}
			#if WDR_FUNC
			if (1) /*(UI_GetData(FL_WDR) == WDR_ON || UI_GetData(FL_SHDR) == SHDR_OFF)*/ {
				IPL_FUNC |= IPL_FUNC_WDR;
			}
			ImageUnit_SetParam(ISF_CTRL, IPL_SEL_WDR_IMM, Get_WDRValue(UI_GetData(FL_WDR)));
			#endif
			#if SHDR_FUNC
			if (1) /*(UI_GetData(FL_SHDR) == SHDR_ON && UI_GetData(FL_WDR) == WDR_OFF)*/ {
				IPL_FUNC |= IPL_FUNC_SHDR;
			} else {
				UI_SetData(FL_SHDR, SHDR_OFF);
			}
			ImageUnit_SetParam(ISF_CTRL, IPL_SEL_SHDR, Get_SHDRValue(UI_GetData(FL_SHDR)));
			ImageUnit_SetParam(ISF_CTRL, IPL_SEL_SHDR_FRAME_NUM, DVCAM_SHDR_MAX_FRAME);
			#endif
			#if NVT_DEPTH_FUNC
			IPL_FUNC |= IPL_FUNC_NVTDEPTH;
			#endif
			#if DEWARP_FUNC
			IPL_FUNC |= IPL_FUNC_VR360;
			#endif
			#if DEFOG_FUNC
			IPL_FUNC |= IPL_FUNC_DEFOG;
			ImageUnit_SetParam(ISF_CTRL, IPL_SEL_DEFOG_IMM, Get_DefogValue((UI_GetData(FL_DEFOG))));
			#endif

			#if (MOVIE_DIS_MV_FUNC  == ENABLE)
			IPL_FUNC |= IPL_FUNC_SIE_CH4_ETH;
			#endif

			ImageUnit_SetParam(ISF_CTRL, IMAGEPIPE_PARAM_MAX_FUNC, IPL_FUNC);
			ImageUnit_SetParam(ISF_CTRL, IMAGEPIPE_PARAM_FUNC, IPL_FUNC);
#if RSC_FUNC
#if !defined(_Gyro_None_)
        		if (UI_GetData(FL_RSC) == RSC_ON) {
        			IPL_FUNC2 |=  IMAGEPIPE_FUNC2_RSCBUF;
        			PhotoExe_RSC_SetSwitch(SEL_RSC_CHANGEMODE, SEL_RSC_ON);
        		}else{
        			IPL_FUNC2 &= ~IMAGEPIPE_FUNC2_RSCBUF;
        			PhotoExe_RSC_SetSwitch(SEL_RSC_CHANGEMODE, SEL_RSC_OFF);
        		}
#endif
#endif

#if (MOVIE_DIS_COMP_FUNC == ENABLE)
			IPL_FUNC2 |= IMAGEPIPE_FUNC2_VIEWTRACKING1;
			IPL_FUNC2 |= IMAGEPIPE_FUNC2_VIEWTRACKING2;
			IPL_FUNC2 |= IMAGEPIPE_FUNC2_VIEWTRACKING3;
			IPL_FUNC2 |= IMAGEPIPE_FUNC2_VIEWTRACKING4;
			PhotoExe_IPL_SetUIInfo(IPL_SEL_DISCOMP_IMM, SEL_DISCOMP_ON);
#endif

                	ImageUnit_SetParam(ISF_CTRL, IMAGEPIPE_PARAM_MAX_FUNC2, IPL_FUNC2);
                	ImageUnit_SetParam(ISF_CTRL, IMAGEPIPE_PARAM_FUNC2, IPL_FUNC2);

			/////set IPL mirror and flip
			ImageUnit_SetVdoDirection(ISF_IN1, (UI_GetData(FL_MOVIE_SENSOR_ROTATE) == SEN_ROTATE_ON) ? ISF_VDO_DIR_MIRRORY : ISF_VDO_DIR_NONE);
		ImageUnit_End();

	}
}

void PhotoExe_InitNetHttp(void)
{
	// config daemon parameter
	LVIEWNVT_DAEMON_INFO   DaemonInfo = {0};

	DaemonInfo.portNum = 8192;
	// set http live view server thread priority
	DaemonInfo.threadPriority = 6;
	// live view streaming frame rate
	DaemonInfo.frameRate = 30;
	// socket buffer size
	DaemonInfo.sockbufSize = 102400;
	// set time out 10*0.5 = 5 sec
	DaemonInfo.timeoutCnt  = 10;
	// Set type of service as Maximize Throughput
	DaemonInfo.tos = 0x0100;
	// if want to use https
	DaemonInfo.is_ssl = 1;
	DaemonInfo.is_push_mode = 1;
	ImageUnit_Begin(&ISF_NetHTTP, 0);
	ImageUnit_SetParam(ISF_CTRL, NETHTTP_PARAM_DAEMON, (UINT32)&DaemonInfo);
	ImageUnit_End();
}

extern UINT32 g_DualVideo;

static void PhotoExe_InitDisplayStream(UINT32 dualCam, USIZE *pImageRatioSize)
{
	UINT32 i;
	PHOTO_DISP_INFO     *p_disp = NULL;
	APPDISP_VIEW_INFO    appdisp_info = {0};


	// sensor 1 display
	if (dualCam == DUALCAM_FRONT) {
		p_disp = UIAppPhoto_get_DispConfig(UIAPP_PHOTO_DISP_ID_1);
		p_disp->enable = TRUE;
		//p_disp->width  =  0;
		//p_disp->height =  0;
		p_disp->width_ratio  =  pImageRatioSize->w;
		p_disp->height_ratio =  pImageRatioSize->h;
#if (DISPLAY2_FUNC == ENABLE)
		if (g_DualVideo == DISPLAY_1) {
			p_disp->vid_out      =  PHOTO_VID_OUT_1;
		} else if (g_DualVideo == DISPLAY_2) {
			p_disp->vid_out      =  PHOTO_VID_OUT_2;
		} else if (g_DualVideo == (DISPLAY_1|DISPLAY_2)) {
			//p_disp->vid_out      =  PHOTO_VID_OUT_2; //only display video on HDMI(videoout2)
			p_disp->vid_out      =  PHOTO_VID_OUT_1;
#if (DUALDISP_FUNC == ENABLE)
        		PHOTO_DUAL_DISP photo_dual_disp={0};
        		if (GxVideo_GetDevice(DOUT2)) {
        			ISIZE dout1_size = GxVideo_GetDeviceSize(DOUT1);
        			ISIZE dout2_size = GxVideo_GetDeviceSize(DOUT2);
        			ISIZE dout2_ar = {4, 3};       // disp2 aspect ratio
        			if(GetPhotoSizeRatio(UI_GetData(FL_PHOTO_SIZE))==IMAGERATIO_16_9)
        			{
        				dout2_ar.w=16;
        				dout2_ar.h=9;
        			}
        			photo_dual_disp.enable=TRUE;
        			if (dout1_size.w * dout1_size.h > dout2_size.w * dout2_size.h) {
        				photo_dual_disp.disp_size=dout1_size;
        			} else {
        				photo_dual_disp.disp_size=dout2_size;
        			}
        			photo_dual_disp.disp_ar=dout2_ar;
        		} else {
        			photo_dual_disp.enable=FALSE;
        		}
        		ImageApp_Photo_Config(PHOTO_CFG_DUAL_DISP, (UINT32)&photo_dual_disp);
#endif
		}
#else
		p_disp->vid_out      =  PHOTO_VID_OUT_1;
#endif
		if(p_disp->vid_out == PHOTO_VID_OUT_1)
        		p_disp->rotate_dir   =  SysVideo_GetDirbyID(DOUT1);
		else
        		p_disp->rotate_dir   =  SysVideo_GetDirbyID(DOUT2);
		ImageApp_Photo_Config(PHOTO_CFG_DISP_INFO, (UINT32)p_disp);
	}
	// sensor 2 display
	else if (dualCam == DUALCAM_BEHIND) {
		p_disp = UIAppPhoto_get_DispConfig(UIAPP_PHOTO_DISP_ID_2);
		p_disp->enable = TRUE;
		//p_disp->width  =  0;
		//p_disp->height =  0;
		p_disp->width_ratio  =  pImageRatioSize->w;
		p_disp->height_ratio =  pImageRatioSize->h;
		p_disp->rotate_dir   =  SysVideo_GetDir();
#if (DISPLAY2_FUNC == ENABLE)
		if (g_DualVideo == DISPLAY_1) {
			p_disp->vid_out      =  PHOTO_VID_OUT_1;
		} else if (g_DualVideo == DISPLAY_2) {
			p_disp->vid_out      =  PHOTO_VID_OUT_2;
		} else if (g_DualVideo == (DISPLAY_1|DISPLAY_2)) {
			p_disp->vid_out      =  PHOTO_VID_OUT_2; //only display video on HDMI(videoout2)
		}
#else
		p_disp->vid_out      =  PHOTO_VID_OUT_1;
#endif
		ImageApp_Photo_Config(PHOTO_CFG_DISP_INFO, (UINT32)p_disp);
	}
	// PIP view
	else {
		if (AppDispView_GetInfo(&appdisp_info) != E_OK)
			return;
        for (i=0;i<localInfo->sensorCount;i++) {
			p_disp = UIAppPhoto_get_DispConfig(UIAPP_PHOTO_DISP_ID_1+i);
			if (p_disp== NULL)
				return;
			p_disp->enable          = TRUE;
			#if (SBS_VIEW_FUNC == ENABLE)
            p_disp->multi_view_type = PHOTO_MULTI_VIEW_SBS_LR;
			#else
			p_disp->multi_view_type = PHOTO_MULTI_VIEW_PIP;
			#endif
			p_disp->width_ratio  =  pImageRatioSize->w;
			p_disp->height_ratio =  pImageRatioSize->h;
			p_disp->rotate_dir   =  SysVideo_GetDir();
#if (DISPLAY2_FUNC == ENABLE)
			if (g_DualVideo == DISPLAY_1) {
				p_disp->vid_out      =  PHOTO_VID_OUT_1;
			} else if (g_DualVideo == DISPLAY_2) {
				p_disp->vid_out      =  PHOTO_VID_OUT_2;
			} else if (g_DualVideo == (DISPLAY_1|DISPLAY_2)) {
				p_disp->vid_out      =  PHOTO_VID_OUT_2; //only display video on HDMI(videoout2)
			}
#else
			p_disp->vid_out      =  PHOTO_VID_OUT_1;
#endif
			ImageApp_Photo_Config(PHOTO_CFG_DISP_INFO, (UINT32)p_disp);

			ImageUnit_Begin(&ISF_UserProc, 0);
			ImageUnit_SetParam(ISF_OUT1 + p_disp->disp_id, USERPROC_PARAM_SCALE_WIDTH,  appdisp_info.scale[i].w);
			ImageUnit_SetParam(ISF_OUT1 + p_disp->disp_id, USERPROC_PARAM_SCALE_HEIGHT, appdisp_info.scale[i].h);
			ImageUnit_SetParam(ISF_OUT1 + p_disp->disp_id, USERPROC_PARAM_SYNC_SIZE_REF_ID, PHOTO_DISP_ID_1);
			ImageUnit_End();
        }
		ImageUnit_Begin(&ISF_UserProc, 0);
		ImageUnit_SetParam(ISF_OUT1 + p_disp->disp_id, USERPROC_PARAM_PUSH_CALLBACK_IMM, (UINT32)PhotoExe_UserProc_PushCB);
		ImageUnit_SetParam(ISF_OUT1 + p_disp->disp_id, USERPROC_PARAM_PROCESS_CALLBACK_IMM, (UINT32)PhotoExe_UserProc_ProcessCB);
		ImageUnit_End();
	}
	//
}

static void PhotoExe_InitNetworkStream(UINT32 dualCam, USIZE *pImageRatioSize)
{
	UINT32 i;
	PHOTO_STRM_INFO    *p_strm = NULL;
    APPDISP_VIEW_INFO   appdisp_info ={0};


	// sensor 1 display
	if (dualCam == DUALCAM_FRONT) {
		p_strm = UIAppPhoto_get_StreamConfig(UIAPP_PHOTO_STRM_ID_1);
		ImageApp_Photo_Config(PHOTO_CFG_STRM_INFO, (UINT32)p_strm);
		p_strm->width_ratio  =  pImageRatioSize->w;
		p_strm->height_ratio =  pImageRatioSize->h;
		#if (DEWARP_FUNC == DISABLE)
		p_strm->width = ALIGN_CEIL_16(p_strm->height* pImageRatioSize->w/pImageRatioSize->h);
		#endif
		//p_strm->height = PHOTO_STRM_HEIGHT;
		ImageApp_Photo_Config(PHOTO_CFG_STRM_INFO, (UINT32)p_strm);
		if (p_strm->strm_type== PHOTO_STRM_TYPE_HTTP) {
			PhotoExe_InitNetHttp();
		}
#if 0
		PHOTO_STRM_CBR_INFO tStrmCbrInfo =  {
		1,
		PHOTO_STRM_ID_1,
		{1, 4, 200 * 1024, 30, 15, 26, 1, 40, 26, 1, 40, 0, 1, 4, 2}};

    		ImageApp_Photo_Config(PHOTO_CFG_CBR_INFO, (UINT32)&(tStrmCbrInfo));
#endif
	}
	// sensor 2 display
	else if (dualCam == DUALCAM_BEHIND) {
		p_strm = UIAppPhoto_get_StreamConfig(UIAPP_PHOTO_STRM_ID_2);
		ImageApp_Photo_Config(PHOTO_CFG_STRM_INFO, (UINT32)p_strm);
		p_strm->width_ratio  =  pImageRatioSize->w;
		p_strm->height_ratio =  pImageRatioSize->h;
		p_strm->width = ALIGN_CEIL_16(p_strm->height* pImageRatioSize->w/pImageRatioSize->h);
		//p_strm->height = PHOTO_STRM_HEIGHT;
		ImageApp_Photo_Config(PHOTO_CFG_STRM_INFO, (UINT32)p_strm);
		if (p_strm->strm_type == PHOTO_STRM_TYPE_HTTP) {
			PhotoExe_InitNetHttp();
		}
	}
	// PIP view
	else {
		if (AppDispView_GetInfo(&appdisp_info) != E_OK)
			return;
		for (i=0;i<localInfo->sensorCount;i++) {
			p_strm = UIAppPhoto_get_StreamConfig(UIAPP_PHOTO_STRM_ID_1+i);
			if (p_strm == NULL)
				return;
			#if (SBS_VIEW_FUNC == ENABLE)
            p_strm->multi_view_type = PHOTO_MULTI_VIEW_SBS_LR;
			#else
			p_strm->multi_view_type = PHOTO_MULTI_VIEW_PIP;
			#endif
			p_strm->width_ratio  =  pImageRatioSize->w;
			p_strm->height_ratio =  pImageRatioSize->h;
			p_strm->width = ALIGN_CEIL_16(p_strm->height* pImageRatioSize->w/pImageRatioSize->h);
			#if (SBS_VIEW_FUNC == ENABLE)
            p_strm->width <<= 1;
			#endif
		    //p_strm->height = PHOTO_STRM_HEIGHT;
			ImageApp_Photo_Config(PHOTO_CFG_STRM_INFO, (UINT32)p_strm);
			ImageUnit_Begin(&ISF_UserProc, 0);
			ImageUnit_SetParam(ISF_OUT1 + p_strm->strm_id, USERPROC_PARAM_SCALE_WIDTH,  appdisp_info.scale[i].w);
			ImageUnit_SetParam(ISF_OUT1 + p_strm->strm_id, USERPROC_PARAM_SCALE_HEIGHT, appdisp_info.scale[i].h);
			ImageUnit_SetParam(ISF_OUT1 + p_strm->strm_id, USERPROC_PARAM_SYNC_SIZE_REF_ID, PHOTO_STRM_ID_1);
			ImageUnit_End();
		}
		ImageUnit_Begin(&ISF_UserProc, 0);
		ImageUnit_SetParam(ISF_OUT1 + p_strm->strm_id, USERPROC_PARAM_PUSH_CALLBACK_IMM, (UINT32)PhotoExe_UserProc_PushCB);
		ImageUnit_SetParam(ISF_OUT1 + p_strm->strm_id, USERPROC_PARAM_PROCESS_CALLBACK_IMM, (UINT32)PhotoExe_UserProc_ProcessCB);
		ImageUnit_End();

		if (p_strm->strm_type == PHOTO_STRM_TYPE_HTTP) {
			PhotoExe_InitNetHttp();
		}
	}

	if (UI_GetData(FL_MOVIE_DATEIMPRINT) == MOVIE_DATEIMPRINT_ON)
	{
		UINT32      uiStampAddr;
		UINT32      uiWidth  = p_strm->width;
		UINT32      uiHeight = p_strm->height;
		STAMP_COLOR StampColorBg = {RGB_GET_Y( 16,  16,  16), RGB_GET_U( 16,  16,  16), RGB_GET_V( 16,  16,  16)}; // date stamp background color
		STAMP_COLOR StampColorFr = {RGB_GET_Y( 16,  16,  16), RGB_GET_U( 16,  16,  16), RGB_GET_V( 16,  16,  16)}; // date stamp frame color
		STAMP_COLOR StampColorFg = {RGB_GET_Y(224, 224, 192), RGB_GET_U(224, 224, 192), RGB_GET_V(224, 224, 192)}; // date stamp foreground color

#if (STAMP_PRIVATE_POOL == DISABLE)
		// use POOL_ID_DATEIMPRINT as movie data stamp buffer
		uiStampAddr = OS_GetMempoolAddr(POOL_ID_DATEIMPRINT);

		//#NT#2016/07/22#YongChang Qui -begin
		//#NT#MovieStamp uses unified memory as IPC & MovieStamp buffer for IPCam with AE_LOG off
		MovieStamp_SetBuffer(HTTP_PATH_ID, uiStampAddr + POOL_SIZE_DATEIMPRINT*3/4, POOL_SIZE_DATEIMPRINT/4);
		//#NT#2016/07/22#YongChang Qui -end
#else
		uiStampAddr=MovieStamp_GetBufAddr(HTTP_PATH_ID, POOL_SIZE_DATEIMPRINT/4);
		MovieStamp_SetBuffer(HTTP_PATH_ID, uiStampAddr , POOL_SIZE_DATEIMPRINT/4);
#endif
		MovieStamp_SetColor(HTTP_PATH_ID, &StampColorBg, &StampColorFr, &StampColorFg);


		MovieStamp_Setup(
                    HTTP_PATH_ID,
                    STAMP_ON |
                    STAMP_AUTO |
                    STAMP_DATE_TIME |
                    STAMP_BOTTOM_RIGHT |
                    STAMP_POS_NORMAL |
                    STAMP_BG_TRANSPARENT |
                    STAMP_YY_MM_DD |
                    STAMP_IMG_420UV,
                    uiWidth,
                    uiHeight,
                    NULL);
		// enable movie stamp
		//MovieStamp_Enable();
		ImageApp_Photo_Config(PHOTO_CFG_DRAW_CB, (UINT32)&NetHTTP_DataStampCB);
	}
}

static UINT32 PhotoExe_AutoCal_IME3DNRSize(UINT32 sensor_id, UIAPP_PHOTO_SENSOR_INFO *pSensorInfo, USIZE *pImageRatioSize)
{
	UINT32 	ime3dnr_size;
	//UINT32  max_w = PHOTO_3DNR_MAX_WIDTH, max_h = PHOTO_3DNR_MAX_HEIGHT;

	ImageUnit_Begin(ISF_IPL(sensor_id), 0);
	ImageUnit_SetParam(ISF_CTRL, IMAGEPIPE_PARAM_MODE, IMAGEPIPE_MODE_PREVIEW);
	ImageUnit_SetVdoAspectRatio(ISF_IN1, pImageRatioSize->w, pImageRatioSize->h);
	ImageUnit_SetVdoImgSize(ISF_IN1, pSensorInfo->sSize.w, pSensorInfo->sSize.h);  //user perferred IPL input w,h (= main stream size)
	ImageUnit_SetVdoFramerate(ISF_IN1, ISF_VDO_FRC(pSensorInfo->fps, 1));          //user perferred IPL input fps (= main stream fps)
	ImageUnit_End();
	ime3dnr_size = ImageUnit_GetParam(ISF_IPL(sensor_id), ISF_IN1, IMAGEPIPE_PARAM_ACTUAL_INSIZE_IMM);
	DBG_DUMP("Auto select sensor %d IME3DNR size w = %d, h = %d\r\n",sensor_id,ISF_GET_HI(ime3dnr_size),ISF_GET_LO(ime3dnr_size));
	return ime3dnr_size;
}

static void PhotoExe_InitIME3DNRStream(UINT32 dualCam, USIZE *pImageRatioSize)
{
	UIAPP_PHOTO_SENSOR_INFO *pSensorInfo;
	PHOTO_IME3DNR_INFO      ime3dnr;
	UINT32                  sensor_id,ime3dnr_size;

	for (sensor_id=0;sensor_id<localInfo->sensorCount;sensor_id++) {
		pSensorInfo = UIAppPhoto_get_SensorInfo(sensor_id);
		if (pSensorInfo && pSensorInfo->bIME3DNR) {
	        ime3dnr_size = PhotoExe_AutoCal_IME3DNRSize(sensor_id,pSensorInfo,pImageRatioSize);
			ime3dnr.enable          =  TRUE;
			ime3dnr.ime3dnr_id      =  PHOTO_IME3DNR_ID_1+sensor_id;
			ime3dnr.vid_in          =  PHOTO_VID_IN_1+sensor_id;
			ime3dnr.width           =  ISF_GET_HI(ime3dnr_size);
			ime3dnr.height          =  ISF_GET_LO(ime3dnr_size);
			ime3dnr.width_ratio     =  pImageRatioSize->w;
			ime3dnr.height_ratio    =  pImageRatioSize->h;
			ime3dnr.max_width       =  0;
			ime3dnr.max_height      =  0;
			ImageApp_Photo_Config(PHOTO_CFG_IME3DNR_INFO, (UINT32)&ime3dnr);
		}
    }
}

static void PhotoExe_InitAllImageStreams(UINT32 dualCam, USIZE *pImageRatioSize)
{


    DBG_IND("\r\n");

	ImageUnit_Begin(&ISF_Cap, 0);
	ImageUnit_SetParam(ISF_CTRL, CAP_PARAM_RESV_BUFSIZE, 0x200000);
	ImageUnit_End();

    // Init display image stream
	PhotoExe_InitDisplayStream(dualCam, pImageRatioSize);

	// Init network image stream
    PhotoExe_InitNetworkStream(dualCam, pImageRatioSize);

	// Init IME3DNR image stream
    PhotoExe_InitIME3DNRStream(dualCam, pImageRatioSize);
}


/**
  Initialize application for Photo mode

  Initialize application for Photo mode.

  @param void
  @return void
*/
INT32 PhotoExe_OnOpen(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
	USIZE               ImageRatioSize = {0};
	UINT32              ImageRatioIdx;
	UINT32              dualCam, sensor_id;
	PHOTO_STRM_INFO 	*p_strm = NULL;

	ImageRatioIdx = GetPhotoSizeRatio(UI_GetData(FL_PHOTO_SIZE));
	ImageRatioSize = IMAGERATIO_SIZE[ImageRatioIdx];


	g_photo_ImageRatioSize = ImageRatioSize;


	DBG_IND("\r\n");

	//call default
	Ux_DefaultEvent(pCtrl, NVTEVT_EXE_OPEN, paramNum, paramArray);

	SxCmd_AddTable(uiphoto);

	PhotoExe_InitCommonMem();

	PhotoExe_InitPhotoFunc();

	PhotoExe_InitFileNaming();

    dualCam = PhotoExe_InitSensorCount();

	PhotoExe_InitIPL(&ImageRatioSize);

	PhotoExe_InitAllImageStreams(dualCam,&ImageRatioSize);

	// register Callback function
	Photo_RegCB();

	ImageUnit_Begin(&ISF_VdoEnc, 0);
	for (sensor_id=0;sensor_id<localInfo->sensorCount;sensor_id++) {
	    p_strm = UIAppPhoto_get_StreamConfig(UIAPP_PHOTO_STRM_ID_1+sensor_id);

		if (p_strm) {
			if (p_strm->codec == PHOTO_CODEC_MJPG) {
#if (VIDEO_FUNC_MJPG == ENABLE)
				MP_VDOENC_ENCODER *pObj = MP_MjpgEnc_getVideoObject(UIAPP_PHOTO_STRM_ID_1+sensor_id);
				ImageUnit_SetParam(UIAPP_PHOTO_STRM_ID_1+sensor_id, VDOENC_PARAM_ENCODER_OBJ, (UINT32)pObj);
#else
				ImageUnit_SetParam(UIAPP_PHOTO_STRM_ID_1+sensor_id, VDOENC_PARAM_ENCODER_OBJ, 0);
				DBG_ERR("[PHOTO][%d] vdo codec = %d not support\r\n", sensor_id, p_strm->codec);
#endif
			}
			if (p_strm->codec == PHOTO_CODEC_H264) {
#if (VIDEO_FUNC_H264 == ENABLE)
				MP_VDOENC_ENCODER *pObj = MP_H264Enc_getVideoObject(UIAPP_PHOTO_STRM_ID_1+sensor_id);
				ImageUnit_SetParam(UIAPP_PHOTO_STRM_ID_1+sensor_id, VDOENC_PARAM_ENCODER_OBJ, (UINT32)pObj);
#else
				ImageUnit_SetParam(UIAPP_PHOTO_STRM_ID_1+sensor_id, VDOENC_PARAM_ENCODER_OBJ, 0);
				DBG_ERR("[PHOTO][%d] vdo codec = %d not support\r\n", sensor_id, p_strm->codec);
#endif
			}
			if (p_strm->codec == PHOTO_CODEC_H265) {
#if (VIDEO_FUNC_H265 == ENABLE)
				MP_VDOENC_ENCODER *pObj = MP_H265Enc_getVideoObject(UIAPP_PHOTO_STRM_ID_1+sensor_id);
				ImageUnit_SetParam(UIAPP_PHOTO_STRM_ID_1+sensor_id, VDOENC_PARAM_ENCODER_OBJ, (UINT32)pObj);
#else
				ImageUnit_SetParam(UIAPP_PHOTO_STRM_ID_1+sensor_id, VDOENC_PARAM_ENCODER_OBJ, 0);
				DBG_ERR("[PHOTO][%d] vdo codec = %d not support\r\n", sensor_id, p_strm->codec);
#endif
			}
		}
	}
	ImageUnit_End();

	ImageApp_Photo_Open();
#if HTTP_LIVEVIEW_FUNC && WIFI_AP_FUNC==ENABLE
	if (System_GetState(SYS_STATE_CURRSUBMODE) == SYS_SUBMODE_WIFI) {
		Ux_PostEvent(NVTEVT_WIFI_EXE_MODE_DONE, 1, E_OK);
	} else
#endif
	{
        	if (UI_GetData(FL_MOVIE_DATEIMPRINT) == MOVIE_DATEIMPRINT_ON){
                	MovieStamp_Disable();
        	}
	}
#if UCTRL_FUNC
	UCtrlAppPhoto_Open();
#endif
	DBG_FUNC_END("");

//#NT#2016/03/18#Niven Cho -begin
//#NT#improve the using of gprof
#if defined(_GPROF_TOOL_ON_)
	//take the place of SxCmd Temp Buffer after ImageApp_Open, becasue it has changed in Open()
	SxCmd_RegTempMemFunc(System_GetGprofBuffer);
#endif
//#NT#2016/03/18#Niven Cho -end
    System_WaitFS();
	ImageApp_Photo_FileNaming_Open();
	return NVTEVT_CONSUME;
}




INT32 PhotoExe_OnClose(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
	if (ImageUnit_GetParam(&ISF_Cap, ISF_CTRL, CAP_PARAM_WAITTSK) != 0) {
		DBG_ERR("WAITTSK\r\n");
	}
	UIPhotoFunc_Close();
#if _FD_FUNC_
	FD_Lock(TRUE);
	FD_UnInit();
#endif
	if (DATEIMPRINT_OFF != UI_GetData(FL_DATE_STAMP)) {
    		UiDateImprint_DestroyBuff();
    	}
	if (UI_GetData(FL_MOVIE_DATEIMPRINT) == MOVIE_DATEIMPRINT_ON){
        	MovieStamp_Disable();
	}
	MovieStamp_DestroyBuff();
	ImageApp_Photo_Close();
	PhotoExe_ExitCommonMem();
	//call default
	Ux_DefaultEvent(pCtrl, NVTEVT_EXE_CLOSE, paramNum, paramArray);
	return NVTEVT_CONSUME;
}

INT32 PhotoExe_OnSleep(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    ImageApp_Photo_SetScreenSleep();

	return NVTEVT_CONSUME;
}

INT32 PhotoExe_OnWakeup(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    ImageApp_Photo_SetScreenWakeUp();

	return NVTEVT_CONSUME;
}

INT32 PhotoExe_OnMacro(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
	UINT32 uhSelect = 0;
	DBG_IND("%d \r\n", paramArray[0]);
	if (paramNum > 0) {
		uhSelect = paramArray[0];
	}

	UI_SetData(FL_MACRO, uhSelect);
	PhotoExe_AF_SetUIInfo(AF_UI_F, Get_MacroValue(uhSelect));
	return NVTEVT_CONSUME;
}

INT32 PhotoExe_OnSelftimer(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
	UINT32 uhSelect = 0;
	DBG_IND("%d \r\n", paramArray[0]);
	if (paramNum > 0) {
		uhSelect = paramArray[0];
	}

	UI_SetData(FL_SELFTIMER, uhSelect);
	exam_msg("photo selftimer %d\r\n", uhSelect);
	return NVTEVT_CONSUME;
}

INT32 PhotoExe_OnFlash(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
	UINT32 uhSelect = 0;

	DBG_IND("%d \r\n", paramArray[0]);
	if (paramNum > 0) {
		uhSelect = paramArray[0];
	}

	UI_SetData(FL_FLASH_MODE, uhSelect);
	Photo_SetUserIndex(PHOTO_USR_FLASH, uhSelect);
	if (UI_GetData(FL_IsInitSettingFlag) == FALSE) {
		if (uhSelect != FLASH_OFF) {
			if (!UI_GetData(FL_IsStopCharge)) {
#if (FLASHLIGHT_FUNCTION == ENABLE)
				GxFlash_StartCharge();
#endif
			}
		} else {
#if (FLASHLIGHT_FUNCTION == ENABLE)
			GxFlash_StopCharge();
#endif
		}
	}
	PhotoExe_AE_SetUIInfo(AE_UI_FLASH, Get_FlashValue(uhSelect));
	return NVTEVT_CONSUME;
}

INT32 PhotoExe_OnEV(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
	UINT32 uhSelect = 0;
	DBG_IND("%d \r\n", paramArray[0]);
	if (paramNum > 0) {
		uhSelect = paramArray[0];
	}

	UI_SetData(FL_EV, uhSelect);
	Photo_SetUserIndex(PHOTO_USR_EV, uhSelect);
	PhotoExe_AE_SetUIInfo(AE_UI_EV, Get_EVValue(uhSelect));
	exam_msg("photo ev %d\r\n", uhSelect);
	return NVTEVT_CONSUME;
}

INT32 PhotoExe_OnCaptureSize(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
	UINT32 uhSelect = 0;
	DBG_IND("%d \r\n", paramArray[0]);

	if (paramNum > 0) {
		uhSelect = paramArray[0];
	}
	UI_SetData(FL_PHOTO_SIZE, uhSelect);
	Photo_SetUserIndex(PHOTO_USR_SIZE, uhSelect);
	exam_msg("photo capture size %d\r\n", uhSelect);
	return NVTEVT_CONSUME;
}

INT32 PhotoExe_OnQuality(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
	UINT32 uhSelect = 0;
	DBG_IND("%d \r\n", paramArray[0]);
	if (paramNum > 0) {
		uhSelect = paramArray[0];
	}
	UI_SetData(FL_QUALITY, uhSelect);
	Photo_SetUserIndex(PHOTO_USR_QUALITY, uhSelect);
	exam_msg("photo quality %d\r\n", uhSelect);
	return NVTEVT_CONSUME;
}

INT32 PhotoExe_OnWB(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
	UINT32 uhSelect = 0;
	DBG_IND("%d \r\n", paramArray[0]);

	uhSelect = paramArray[0];
	UI_SetData(FL_WB, uhSelect);
	Photo_SetUserIndex(PHOTO_USR_WB, uhSelect);
	PhotoExe_AWB_SetUIInfo(AWB_UI_WB, Get_WBValue(uhSelect));
	exam_msg("photo wb %d\r\n", uhSelect);
	return NVTEVT_CONSUME;
}


INT32 PhotoExe_OnColor(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
	UINT32 uhSelect = 0;
	DBG_IND("%d \r\n", paramArray[0]);

	if (paramNum > 0) {
		uhSelect = paramArray[0];
	}

	if (UI_GetData(FL_ModeIndex) != DSC_MODE_PHOTO_SCENE) {
		UI_SetData(FL_COLOR_EFFECT, uhSelect);
		Photo_SetUserIndex(PHOTO_USR_COLOR, uhSelect);
		PhotoExe_IQ_SetUIInfo(IQ_UI_IMAGEEFFECT, Get_ColorValue(uhSelect));
	}
	return NVTEVT_CONSUME;
}

INT32 PhotoExe_OnISO(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
	UINT32 uhSelect = 0;
	DBG_IND("%d \r\n", paramArray[0]);

	if (paramNum > 0) {
		uhSelect = paramArray[0];
	}

	UI_SetData(FL_ISO, uhSelect);
	Photo_SetUserIndex(PHOTO_USR_ISO, uhSelect);
	PhotoExe_AE_SetUIInfo(AE_UI_ISO, Get_ISOValue(uhSelect));
	exam_msg("photo iso %d\r\n", uhSelect);
	return NVTEVT_CONSUME;
}

INT32 PhotoExe_OnAFWindow(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
	UINT32 uhSelect = 0;
	DBG_IND("%d \r\n", paramArray[0]);

	if (paramNum > 0) {
		uhSelect = paramArray[0];
	}

	UI_SetData(FL_AFWindowIndex, uhSelect);
	Photo_SetUserIndex(PHOTO_USR_AFWINDOW, uhSelect);
	PhotoExe_AF_SetUIInfo(AF_UI_WINDOW, Get_AFWindowValue(uhSelect));
	return NVTEVT_CONSUME;
}

INT32 PhotoExe_OnAFBeam(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
	UINT32 uhSelect = 0;
	DBG_IND("%d \r\n", paramArray[0]);
	if (paramNum > 0) {
		uhSelect = paramArray[0];
	}

	UI_SetData(FL_AFBeamIndex, uhSelect);
	Photo_SetUserIndex(PHOTO_USR_AFBEAM, uhSelect);
	return NVTEVT_CONSUME;
}

INT32 PhotoExe_OnContAF(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
	UINT32 uhSelect = 0;
	DBG_IND("%d \r\n", paramArray[0]);
	if (paramNum > 0) {
		uhSelect = paramArray[0];
	}

	UI_SetData(FL_ContAFIndex, uhSelect);
	Photo_SetUserIndex(PHOTO_USR_CONTAF, uhSelect);
	if (uhSelect == CONT_AF_ON) {
		localInfo->isCafOn = TRUE;
	} else {
		localInfo->isCafOn = FALSE;
	}
	return NVTEVT_CONSUME;
}

INT32 PhotoExe_OnMetering(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
	UINT32 uhSelect = 0;
	DBG_IND("%d \r\n", paramArray[0]);

	if (paramNum > 0) {
		uhSelect = paramArray[0];
	}

	UI_SetData(FL_METERING, uhSelect);
	Photo_SetUserIndex(PHOTO_USR_METERING, uhSelect);
	PhotoExe_AE_SetUIInfo(AE_UI_METER, Get_MeteringValue(uhSelect));
	return NVTEVT_CONSUME;
}

INT32 PhotoExe_OnCaptureMode(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
	UINT32 uhSelect = 0;
	if (paramNum > 0) {
		uhSelect = paramArray[0];
	}

	UI_SetData(FL_CapModeIndex, uhSelect);
	return NVTEVT_CONSUME;
}

INT32 PhotoExe_OnDatePrint(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
	UINT32 uhSelect = 0;
	DBG_IND("%d \r\n", paramArray[0]);

	if (paramNum > 0) {
		uhSelect = paramArray[0];
	}

	UI_SetData(FL_DATE_STAMP, uhSelect);
	Photo_SetUserIndex(PHOTO_USR_DATEIMPRINT, uhSelect);
	exam_msg("photo datestamp %d\r\n", uhSelect);
	return NVTEVT_CONSUME;
}

INT32 PhotoExe_OnPreview(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
	UINT32 uhSelect = 0;

	DBG_IND("%d \r\n", paramArray[0]);
	if (paramNum > 0) {
		uhSelect = paramArray[0];
	}

	UI_SetData(FL_QUICK_REVIEW, uhSelect);
	Photo_SetUserIndex(PHOTO_USR_QREVIEW, uhSelect);
	exam_msg("photo quick view time %d\r\n", uhSelect);
	return NVTEVT_CONSUME;
}


INT32 PhotoExe_OnDigitalZoom(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
	UINT32 uhSelect = 0;
	if (paramNum > 0) {
		uhSelect = paramArray[0];
	}

	UI_SetData(FL_Dzoom, uhSelect);
	Photo_SetUserIndex(PHOTO_USR_DZOOMSTATUS, uhSelect);
	if (uhSelect == DZOOM_OFF) {
		UI_SetData(FL_DzoomIndex, DZOOM_10X);
	}
	return NVTEVT_CONSUME;
}

INT32 PhotoExe_OnFD(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
	UINT32 uhSelect = 0;
	if (paramNum > 0) {
		uhSelect = paramArray[0];
	}

	DBG_IND("%d \r\n", paramArray[0]);

	UI_SetData(FL_FD, uhSelect);
	//Photo_SetUserIndex(PHOTO_USR_FD,uhSelect);
	if (uhSelect == FD_ON) {
		PhotoExe_IPL_AlgSetFD_AEInfo(SEL_FD_ON);
		localInfo->isSdOn = FALSE;
		localInfo->isFdOn = TRUE;
#if _FD_FUNC_
		FD_Lock(FALSE);
#endif
	} else {
		PhotoExe_IPL_AlgSetFD_AEInfo(SEL_FD_OFF);
		localInfo->isSdOn = FALSE;
		localInfo->isFdOn = FALSE;
		if (!localInfo->isAscnOn) {
#if _FD_FUNC_
			FD_Lock(TRUE);
#endif
		}
	}
	exam_msg("photo fd %d\r\n", uhSelect);
	return NVTEVT_CONSUME;
}


INT32 PhotoExe_OnContShot(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
	UINT32 uhSelect = 0;

	DBG_IND("%d \r\n", paramArray[0]);
	if (paramNum > 0) {
		uhSelect = paramArray[0];
	}

	UI_SetData(FL_CONTINUE_SHOT, uhSelect);
	exam_msg("photo contshot  %d\r\n", uhSelect);
	return NVTEVT_CONSUME;
}


INT32 PhotoExe_OnSceneMode(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
	UINT32 uhSelect = 0;

	DBG_IND("%d \r\n", paramArray[0]);

	if (paramNum) {
		uhSelect = paramArray[0];

		// scene select by menu
		if (paramNum == 1) {
			UI_SetData(FL_SCENE, uhSelect);
            PhotoExe_AE_SetUIInfo(AE_UI_SCENE, Get_SceneModeValue(uhSelect));
		} else
			// scene set by photo init
		{
            PhotoExe_AE_SetUIInfo(AE_UI_SCENE, Get_SceneModeValue(uhSelect));
		}
	}
	exam_msg("photo scene %d\r\n", uhSelect);
	return NVTEVT_CONSUME;
}


static UINT32 _PhotoExe_GetImageAR(UINT32 aspect_ratio_x, UINT32 aspect_ratio_y)
{
	UINT32 ratio = 0;
	float fdiv;
	if (aspect_ratio_y == 0) {
		return 0;
	}

	fdiv = ((float)aspect_ratio_x) / aspect_ratio_y;

	if (fdiv == ((float)9) / 16) {
		ratio = 0x00090010;    //9:16
	} else if (fdiv == ((float)2) / 3) {
		ratio = 0x00020003;    //2:3
	} else if (fdiv == ((float)3) / 4) {
		ratio = 0x00030004;    //3:4
	} else if (fdiv == ((float)1) / 1) {
		ratio = 0x00010001;    //1:1
	} else if (fdiv == ((float)4) / 3) {
		ratio = 0x00040003;    //4:3
	} else if (fdiv == ((float)3) / 2) {
		ratio = 0x00030002;    //3:2
	} else if (fdiv == ((float)16) / 9) {
		ratio = 0x00100009;    //16:9
	} else {
		return 0;
	}

	return ratio;
}

INT32 PhotoExe_OnCaptureStart(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
	UINT32 CaptureParm = ON_CAPTURE_SINGLE;
	UINT32 i, sensor_id, sensor_cnt = 1;

	if (localInfo->isStartCapture) {
		DBG_ERR("Capture start in Capturing\r\n");
		return NVTEVT_CONSUME;
	}
	if (localInfo->IsAFProcess) {
		DBG_ERR("Capture start in AF\r\n");
		return NVTEVT_CONSUME;
	}
	#if 0
	if (UI_GetData(FL_SHDR) == SHDR_ON) {
		DBG_WRN("HDR not support Capture\r\n");
		return NVTEVT_CONSUME;
	}
	#endif
	if (ImageUnit_GetParam(&ISF_Cap, ISF_CTRL, CAP_PARAM_WAITTSK) != 0) {
		DBG_ERR("WAITTSK\r\n");
	}
	if (paramNum == 1) {
		CaptureParm = paramArray[0];
	}
	DBG_IND("[cap] CaptureParm = %d\r\n", CaptureParm);
	DBG_IND("[cap] ContShot En %d\r\n", UI_GetData(FL_CONTINUE_SHOT));
	exam_msg("[cap] ContShot %d\r\n", UI_GetData(FL_CONTINUE_SHOT));
	//DBG_IND("[cap] ContSot index %d\r\n",UI_GetData(FL_ContShotIndex));

	if (UI_GetData(FL_FLASH_MODE) != FLASH_OFF) {
#if (FLASHLIGHT_FUNCTION == ENABLE)
		SxTimer_SetFuncActive(SX_TIMER_DET_RECHARGE_ID, FALSE);
		GxFlash_StopCharge();
#endif

	}
	//Disable USB detection
	SxTimer_SetFuncActive(SX_TIMER_DET_USB_ID, FALSE);
	//Disable USB detection
	SxTimer_SetFuncActive(SX_TIMER_DET_TV_ID, FALSE);
	//Disable Mode detection
	SxTimer_SetFuncActive(SX_TIMER_DET_MODE_ID, FALSE);

	AF_LOCK(TRUE);
#if _FD_FUNC_
	FD_Lock(TRUE);
#endif
	AF_WAIT_IDLE();

	Ux_FlushEventByRange(NVTEVT_EXE_FDEND, NVTEVT_EXE_FDEND);

	//idec_setV1En(IDE_ID_1, FALSE);
	if (localInfo->isSingleViewOpen) {
		//idec_setV1En(IDE_ID_1, FALSE);
		//Photo_SingleView_Close(PHOTO_VIEWTYPE_NORMAL);
		localInfo->isSingleViewOpen = FALSE;
	}
	if (UI_GetData(FL_DATE_STAMP) != DATEIMPRINT_OFF) {
		//Init Date Imprint buff
		Ux_SendEvent(&CustomPhotoObjCtrl, NVTEVT_EXE_INIT_DATE_BUF, 0);
	}
	if (UI_GetData(FL_CONTINUE_SHOT) == CONTINUE_SHOT_SIDE) {
		DBG_IND("[cap]sidebyside\r\n");
	}
	//  dual cam capture

	else if (localInfo->DualCam >= DUALCAM_BOTH) {
		sensor_cnt = localInfo->sensorCount;
	}
	for (i = 0; i < sensor_cnt; i++) {
		if (sensor_cnt >= 2) {
			sensor_id = i;
			PhotoExe_Cap_SetUIInfo(sensor_id, CAP_SEL_DRIVEMODE, SEL_DRIVEMODE_MULSENSOR);
			PhotoExe_Cap_SetUIInfo(sensor_id, CAP_SEL_RAW_BUF_NUM, 2);
			PhotoExe_Cap_SetUIInfo(sensor_id, CAP_SEL_YUV_BUF_NUM, 2);
			PhotoExe_Cap_SetUIInfo(sensor_id, CAP_SEL_JPG_BUF_NUM, 3);
		} else {
			sensor_id = localInfo->DualCam; //get active sensor
			DBG_IND("[cap]single sensor_id=%d \r\n", sensor_id);
		}
		PhotoExe_Cap_SetUIInfo(sensor_id, CAL_SEL_DATASTAMP, Get_DatePrintValue(UI_GetData(FL_DATE_STAMP)));
		PhotoExe_Cap_SetUIInfo(sensor_id, CAP_SEL_ACTFLAG, TRUE);
		PhotoExe_Cap_SetUIInfo(sensor_id, CAP_SEL_QUALITY, Get_QualityValue(UI_GetData(FL_QUALITY)));
		//PhotoExe_Cap_SetUIInfo(sensor_id, CAP_SEL_PHDR_EN, UI_GetData(FL_WDR));
		PhotoExe_Cap_SetUIInfo(sensor_id, CAP_SEL_PHDR_EN, WDR_FUNC);
		PhotoExe_Cap_SetUIInfo(sensor_id, CAP_SEL_SHDR_EN, UI_GetData(FL_SHDR));
		PhotoExe_Cap_SetUIInfo(sensor_id, CAP_SEL_DEFOG_EN, UI_GetData(FL_DEFOG));



#if (_FPGA_EMULATION_ == DISABLE)
		PhotoExe_AE_SetUIInfo(AE_UI_FLASH, Get_FlashValue(UI_GetData(FL_FLASH_MODE)));
#else
		PhotoExe_AE_SetUIInfo(AE_UI_FLASH, AE_FLASH_FORCEOFF);
#endif
		//#NT#2016/04/26#Lincy Lin -begin
		//#NT#Support sidebyside capture mode
		if (UI_GetData(FL_CONTINUE_SHOT) == CONTINUE_SHOT_SIDE) {
			PhotoExe_Cap_SetUIInfo(sensor_id, CAP_SEL_JPG_IMG_H_SIZE, 2 * GetPhotoSizeWidth(UI_GetData(FL_PHOTO_SIZE)));
		} else {
			PhotoExe_Cap_SetUIInfo(sensor_id, CAP_SEL_JPG_IMG_H_SIZE, GetPhotoSizeWidth(UI_GetData(FL_PHOTO_SIZE)));
		}
		//#NT#2016/04/26#Lincy Lin -end
		PhotoExe_Cap_SetUIInfo(sensor_id, CAP_SEL_JPG_IMG_V_SIZE, GetPhotoSizeHeight(UI_GetData(FL_PHOTO_SIZE)));
		if (CaptureParm == ON_CAPTURE_SD_CAPTURE) {
			localInfo->uiTakePicNum++;

		}
		if (UI_GetData(FL_CONTINUE_SHOT) == CONTINUE_SHOT_SIDE) {
			if (System_GetEnableSensor() != (SENSOR_1 | SENSOR_2)) {
				DBG_ERR("SidebySide capture needs dual sensors\r\n");
				PhotoExe_CallBackUpdateInfo(UIAPPPHOTO_CB_CAPSTART);
				PhotoExe_CallBackUpdateInfo(UIAPPPHOTO_CB_CAPTUREEND);
				return NVTEVT_CONSUME;
			}
			if (UI_GetData(FL_PHOTO_SIZE) == PHOTO_SIZE_12M) {
				DBG_ERR("SidebySide not support 12M\r\n");
				PhotoExe_CallBackUpdateInfo(UIAPPPHOTO_CB_CAPSTART);
				PhotoExe_CallBackUpdateInfo(UIAPPPHOTO_CB_CAPTUREEND);
				return NVTEVT_CONSUME;
			}
		}
		switch (UI_GetData(FL_CONTINUE_SHOT)) {
		case CONTINUE_SHOT_BURST:
			//In cont shot (except for timelapse cont shot) case ,must disable blink
			PhotoExe_Cap_SetUIInfo(sensor_id, CAP_SEL_DRIVEMODE, SEL_DRIVEMODE_INFBURST);
			PhotoExe_AE_SetUIInfo(AE_UI_FLASH, AE_FLASH_FORCEOFF);
			PhotoExe_Cap_SetUIInfo(sensor_id, CAP_SEL_RAW_BUF_NUM, 2);
			PhotoExe_Cap_SetUIInfo(sensor_id, CAP_SEL_JPG_BUF_NUM, 3);
			PhotoExe_Cap_SetUIInfo(sensor_id, CAP_SEL_FILEFMT, SEL_FILEFMT_JPG);
			PhotoExe_Cap_SetUIInfo(sensor_id, CAP_SEL_JPGFMT, SEL_JPGFMT_422);
			UI_SetData(FL_IsSingleCapture, FALSE);
			localInfo->isDoingContShot = TRUE;
			localInfo->isStopingContShot = FALSE;
			localInfo->uiTakePicNum = 0;
			// set s2 pressed status to true
			Photo_setS2Status(TRUE);
			break;

		case CONTINUE_SHOT_BURST_3:
			PhotoExe_Cap_SetUIInfo(sensor_id, CAP_SEL_DRIVEMODE, SEL_DRIVEMODE_BURST);
			PhotoExe_AE_SetUIInfo(AE_UI_FLASH, AE_FLASH_FORCEOFF);
			PhotoExe_Cap_SetUIInfo(sensor_id, CAP_SEL_PICNUM, 3);
			PhotoExe_Cap_SetUIInfo(sensor_id, CAP_SEL_JPG_BUF_NUM, 2);
			PhotoExe_Cap_SetUIInfo(sensor_id, CAP_SEL_RAW_BUF_NUM, 2);
			PhotoExe_Cap_SetUIInfo(sensor_id, CAP_SEL_FILEFMT, SEL_FILEFMT_JPG);
			PhotoExe_Cap_SetUIInfo(sensor_id, CAP_SEL_JPGFMT, SEL_JPGFMT_422);
			UI_SetData(FL_IsSingleCapture, FALSE);
			localInfo->isDoingContShot = TRUE;
			localInfo->isStopingContShot = FALSE;
			localInfo->uiTakePicNum = 0;
			break;
		#if defined(_BSP_NA51000_)
		case CONTINUE_SHOT_BURST_30:
			PhotoExe_Cap_SetUIInfo(sensor_id, CAP_SEL_DRIVEMODE, SEL_DRIVEMODE_EBURST);
			PhotoExe_AE_SetUIInfo(AE_UI_FLASH, AE_FLASH_FORCEOFF);
			PhotoExe_Cap_SetUIInfo(sensor_id, CAP_SEL_PICNUM, 30);
			PhotoExe_Cap_SetUIInfo(sensor_id, CAP_SEL_RAW_BUF_NUM, 16);
			PhotoExe_Cap_SetUIInfo(sensor_id, CAP_SEL_YUV_BUF_NUM, 2);
			PhotoExe_Cap_SetUIInfo(sensor_id, CAP_SEL_JPG_BUF_NUM, 5);
			PhotoExe_Cap_SetUIInfo(sensor_id, CAP_SEL_FILEFMT, SEL_FILEFMT_JPG);
			PhotoExe_Cap_SetUIInfo(sensor_id, CAP_SEL_JPGFMT, SEL_JPGFMT_422);
			UI_SetData(FL_IsSingleCapture, FALSE);
			localInfo->isDoingContShot = TRUE;
			localInfo->isStopingContShot = FALSE;
			localInfo->uiTakePicNum = 0;
			break;
		#endif
		case CONTINUE_SHOT_SIDE:
			PhotoExe_Cap_SetUIInfo(sensor_id, CAP_SEL_DRIVEMODE, SEL_DRIVEMODE_DUALSBS);
			PhotoExe_Cap_SetUIInfo(sensor_id, CAP_SEL_PICNUM, 1);
			PhotoExe_Cap_SetUIInfo(sensor_id, CAP_SEL_RAW_BUF_NUM, 1);
			PhotoExe_Cap_SetUIInfo(sensor_id, CAP_SEL_JPG_BUF_NUM, 1);
			PhotoExe_Cap_SetUIInfo(sensor_id, CAP_SEL_FILEFMT, SEL_FILEFMT_JPG);
			PhotoExe_Cap_SetUIInfo(sensor_id, CAP_SEL_JPGFMT, SEL_JPGFMT_422);
			UI_SetData(FL_IsSingleCapture, TRUE);
			localInfo->isDoingContShot = FALSE;
			break;
		default:
			if (sensor_cnt >= 2) {
				PhotoExe_Cap_SetUIInfo(sensor_id, CAP_SEL_DRIVEMODE, SEL_DRIVEMODE_MULSENSOR);
				PhotoExe_Cap_SetUIInfo(sensor_id, CAP_SEL_RAW_BUF_NUM, 2);
				PhotoExe_Cap_SetUIInfo(sensor_id, CAP_SEL_YUV_BUF_NUM, 2);
				PhotoExe_Cap_SetUIInfo(sensor_id, CAP_SEL_JPG_BUF_NUM, 3);
			} else {
				if(UI_GetData(FL_SHDR) ==SHDR_ON){
		        	    PhotoExe_Cap_SetUIInfo(sensor_id, CAP_SEL_DRIVEMODE, SEL_DRIVEMODE_MULFMRHDR);
		        	    PhotoExe_Cap_SetUIInfo(sensor_id, CAP_SEL_RAW_BUF_NUM, 2);
				}else{
		        	    PhotoExe_Cap_SetUIInfo(sensor_id, CAP_SEL_DRIVEMODE, SEL_DRIVEMODE_SINGLE);
		        	    PhotoExe_Cap_SetUIInfo(sensor_id, CAP_SEL_RAW_BUF_NUM, 1);
				}
				PhotoExe_Cap_SetUIInfo(sensor_id, CAP_SEL_YUV_BUF_NUM, 1);
				PhotoExe_Cap_SetUIInfo(sensor_id, CAP_SEL_JPG_BUF_NUM, 1);
			}
			PhotoExe_Cap_SetUIInfo(sensor_id, CAP_SEL_PICNUM, 1);
			PhotoExe_Cap_SetUIInfo(sensor_id, CAP_SEL_FILEFMT, SEL_FILEFMT_JPG);
			PhotoExe_Cap_SetUIInfo(sensor_id, CAP_SEL_JPGFMT, SEL_JPGFMT_422);
			UI_SetData(FL_IsSingleCapture, TRUE);
			localInfo->isDoingContShot = FALSE;
			break;
		}

		// BRC setting
		{
			UINT32 TargetBytes = PhotoExe_GetExpectSize_RhoBRCrtl(UI_GetData(FL_PHOTO_SIZE), TRUE);
			UINT32 UBoundBytes = TargetBytes + (TargetBytes * 15 / 100); //+15%
			UINT32 LBoundBytes = TargetBytes - (TargetBytes * 15 / 100); //-15%
			PhotoExe_Cap_SetUIInfo(sensor_id, CAP_SEL_REENCTYPE, SEL_REENCTYPE_RHO);
			PhotoExe_Cap_SetUIInfo(sensor_id, CAP_SEL_RHO_INITQF, 128);
			PhotoExe_Cap_SetUIInfo(sensor_id, CAP_SEL_RHO_TARGETSIZE, TargetBytes);
			PhotoExe_Cap_SetUIInfo(sensor_id, CAP_SEL_RHO_LBOUNDSIZE, LBoundBytes);
			PhotoExe_Cap_SetUIInfo(sensor_id, CAP_SEL_RHO_HBOUNDSIZE, UBoundBytes);
			PhotoExe_Cap_SetUIInfo(sensor_id, CAP_SEL_RHO_RETRYCNT, 4);
			DBG_IND("[cap]TargetBytes=%d k, H=%d k, L=%d K\r\n", TargetBytes / 1024, UBoundBytes / 1024, LBoundBytes / 1024);
		}

		if (localInfo->uiMaxImageSize) {
			PhotoExe_Cap_SetUIInfo(sensor_id, CAP_SEL_FILEBUFMODE, SEL_FILEBUFMODE_MANUAL);
			PhotoExe_Cap_SetUIInfo(sensor_id, CAP_SEL_FILEBUFSIZE, localInfo->uiMaxImageSize);
			DBG_IND("[cap]FileBufSize=%d k\r\n", localInfo->uiMaxImageSize / 1024);
		} else {
			PhotoExe_Cap_SetUIInfo(sensor_id, CAP_SEL_FILEBUFMODE, SEL_FILEBUFMODE_AUTO);
		}
		PhotoExe_SetScreenNailSize(sensor_id);
		PhotoExe_SetQuickViewSize(sensor_id);

		// Set Capture image ratio
		{
			UINT32 ImageRatioIdx = 0;
			USIZE  ImageRatioSize = {0};

			ImageRatioIdx = GetPhotoSizeRatio(UI_GetData(FL_PHOTO_SIZE));
			ImageRatioSize = IMAGERATIO_SIZE[ImageRatioIdx];
			//PhotoExe_Cap_SetUIInfo(sensor_id,CAP_PARAM_IMGASPECT_W, ImageRatioSize.w);
			//PhotoExe_Cap_SetUIInfo(sensor_id,CAP_PARAM_IMGASPECT_H, ImageRatioSize.h);
			PhotoExe_Cap_SetUIInfo(sensor_id, CAP_SEL_IMG_RATIO, _PhotoExe_GetImageAR(ImageRatioSize.w, ImageRatioSize.h));
		}
            	if(UI_GetData(FL_MOVIE_SENSOR_ROTATE) == SEN_ROTATE_ON)
        		ImgCap_SetUIInfo(sensor_id ,CAP_SEL_FLIP_DIRECTION,SEL_FLIP_DIR_V);
	}

	localInfo->isStartCapture = TRUE;

	/* Start to capture */
	localInfo->IsJPGok = FALSE;
	PhotoExe_CallBackUpdateInfo(UIAPPPHOTO_CB_CAPSTART);

    exam_msg("photo capture start\r\n");

	ImageApp_Photo_CapStart();

	#if 0
	if (ImageApp_Photo_CapStart() != 0) {
		if (ImageUnit_GetParam(&ISF_Cap, ISF_CTRL, CAP_PARAM_WAITTSK) != 0) {
			DBG_ERR("WAITTSK\r\n");
		}
		ImageApp_Photo_CapStart();
	}
	#endif
	return NVTEVT_CONSUME;
}


INT32 PhotoExe_OnCaptureStop(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
	DBG_IND("[cap]\r\n");
	exam_msg("photo capture stop\r\n");
	if (localInfo->isDoingContShot) {
		Photo_setS2Status(FALSE);
		ImageApp_Photo_CapStop();
	}
	return NVTEVT_CONSUME;
}

INT32 PhotoExe_OnCaptureEnd(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
	DBG_IND("[cap]\r\n");
	localInfo->isDoingContShot = FALSE;
	return NVTEVT_CONSUME;
}

INT32 PhotoExe_OnZoom(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{

	UINT32 uiZoomCtrl = UI_ZOOM_CTRL_STOP;
	BOOL   bDZoomEnable = FALSE;
	static BOOL bIsAFLocked = FALSE;
	static UINT32 uiZoomCnt = 0;
	static UINT32 uiLastSection = 1;
	UINT32 TimeOutCnt = 0;
	UINT32 TimeOutMaxCnt = 20;

	DBG_IND("paramArray[0]=%d, paramArray[1]=%d\r\n", paramArray[0], paramArray[1]);
	if (localInfo->IsAFProcess) {
		DBG_ERR("Wait AF Idle\r\n");
		AF_WAIT_IDLE();
	}
	if (paramNum == 2) {
		uiZoomCtrl = paramArray[0];
		bDZoomEnable = paramArray[1];
	} else {
		DBG_ERR("ParamNum %d\r\n", paramNum);
	}
	UI_SetData(FL_ZoomOzmDzmChange, FALSE);
	switch (uiZoomCtrl) {
	case UI_ZOOM_CTRL_STOP:

		if (UI_GetData(FL_ZoomIsDigital) == FALSE) {
			DBG_IND("Photo_OpticalZoom_Stop\r\n");
			if (OZOOM_IDX_GET() == OZOOM_IDX_MIN() &&
				(OZOOM_IDX_GET() != uiLastSection)) {
				uiZoomCnt++;
			}

			uiLastSection = OZOOM_IDX_GET();
			Ux_FlushEventByRange(NVTEVT_CB_ZOOM, NVTEVT_CB_ZOOM);
			UI_SetData(FL_IslensStopping, TRUE);
			if (PhotoExe_LensZoomStop() != E_OK) {
				if (PhotoExe_LensReset() != E_OK) {
					DBG_ERR("lens stop and reset error!!!\r\n");
#if (LENSERROFF_FUNCTION == ENABLE)
					System_PowerOff(SYS_POWEROFF_LENSERROR);
#endif
				} else {
					UI_SetData(FL_ZoomOzmDzmChange, TRUE);
					Ux_PostEvent(NVTEVT_CB_ZOOM, 1, ZOOM_IF_OPTICAL);
				}
			} else {
#if 0
				if (uiZoomCnt == 10) {
					uiZoomCnt = 0;

					if (PhotoExe_LensReset() != E_OK) {
						DBG_ERR("lens reset error!!!\r\n");
#if (LENSERROFF_FUNCTION == ENABLE)
						System_PowerOff(SYS_POWEROFF_LENSERROR);
#endif
					}

				}
#endif
			}
			UI_SetData(FL_IslensStopping, FALSE);

		} else {
			DBG_IND("Photo_DZoom_Stop\r\n");
			PhotoExe_DZoomStop();
		}
		if (!bIsAFLocked) {
			AF_LOCK(FALSE);
		}
		break;

	case UI_ZOOM_CTRL_IN:
		bIsAFLocked = AF_ISLOCK();
		if (!bIsAFLocked) {
			AF_LOCK(TRUE);
		}
		//only CAF or ASC need to wait idle
		if (!bIsAFLocked) {
			AF_WAIT_IDLE();
		}
		// wait image effect
		SwTimer_DelayMs(60);
		if (UI_GetData(FL_ZoomIFIndex) == ZOOM_IF_DIGITAL) {
			DBG_IND("\r\n");
			PhotoExe_DZoomIn();
			UI_SetData(FL_ZoomIsDigital, TRUE);
			if (DZOOM_IDX_GET() == DZOOM_IDX_MIN()) {
				UI_SetData(FL_ZoomOzmDzmChange, TRUE);
			}

		} else if (UI_GetData(FL_ZoomIFIndex) == ZOOM_IF_OPTICAL) {
			TimeOutCnt = 0;
			while (TimeOutCnt != TimeOutMaxCnt) {
				if (PhotoExe_LensZoomIn() == E_OK) {
					break;
				} else {
					TimeOutCnt++;
					SwTimer_DelayMs(20);
					DBG_IND("retry\r\n");
				}
			}
			if (TimeOutCnt == TimeOutMaxCnt) {
				DBG_ERR("time out\r\n");
			}
			UI_SetData(FL_ZoomIsDigital, FALSE);
		} else {
			if (OZOOM_IDX_GET() >= OZOOM_IDX_MAX()) {
				if (bDZoomEnable) {
					DBG_IND("\r\n");
					PhotoExe_DZoomIn();
					UI_SetData(FL_ZoomIsDigital, TRUE);
					UI_SetData(FL_ZoomOzmDzmChange, TRUE);
				}
			} else {
				TimeOutCnt = 0;
				while (TimeOutCnt != TimeOutMaxCnt) {
					if (PhotoExe_LensZoomIn() == E_OK) {
						break;
					} else {
						TimeOutCnt++;
						SwTimer_DelayMs(20);
						DBG_IND("retry \r\n");
					}
				}

				if (TimeOutCnt == TimeOutMaxCnt) {
					DBG_ERR("time out\r\n");
				}
				UI_SetData(FL_ZoomIsDigital, FALSE);
			}
		}
		break;

	case UI_ZOOM_CTRL_OUT:

		bIsAFLocked = AF_ISLOCK();
		if (!bIsAFLocked) {
			AF_LOCK(TRUE);
		}

		if (!bIsAFLocked) {
			AF_WAIT_IDLE();
		}
		SwTimer_DelayMs(60);
		if (UI_GetData(FL_ZoomIFIndex) == ZOOM_IF_DIGITAL) {
			DBG_IND("Photo_DZoom_Out\r\n");
			PhotoExe_DZoomOut();
			UI_SetData(FL_ZoomIsDigital, TRUE);
			if (DZOOM_IDX_GET() == DZOOM_IDX_MIN()) {
				UI_SetData(FL_ZoomOzmDzmChange, TRUE);
				Ux_PostEvent(NVTEVT_CB_ZOOM, 1, ZOOM_IF_DIGITAL);
			}
		} else if (UI_GetData(FL_ZoomIFIndex) == ZOOM_IF_OPTICAL) {
			TimeOutCnt = 0;
			while (TimeOutCnt != TimeOutMaxCnt) {
				if (PhotoExe_LensZoomOut() == E_OK) {
					break;
				} else {
					TimeOutCnt++;
					SwTimer_DelayMs(20);
					DBG_IND("retry \r\n");
				}
			}

			if (TimeOutCnt == TimeOutMaxCnt) {
				DBG_ERR("time out\r\n");
			}
			//#NT#2010/11/19#Photon Lin -end
			UI_SetData(FL_ZoomIsDigital, FALSE);
		} else {
			DBG_IND("\r\n");
			if (DZOOM_IDX_GET() <= DZOOM_IDX_MIN()) {
				DBG_IND("Lens_Zoom_Out\r\n");
				TimeOutCnt = 0;
				while (TimeOutCnt != TimeOutMaxCnt) {
					if (PhotoExe_LensZoomOut() == E_OK) {
						break;
					} else {
						TimeOutCnt++;
						SwTimer_DelayMs(20);
						DBG_MSG("Lens_Zoom_Out() retry \r\n");
					}
				}

				if (TimeOutCnt == TimeOutMaxCnt) {
					DBG_MSG("time out Lens_Zoom_Out()\r\n");
				}
				UI_SetData(FL_ZoomIsDigital, FALSE);
				UI_SetData(FL_ZoomOzmDzmChange, TRUE);
			} else {
				DBG_IND("Photo_DZoom_Out\r\n");
				PhotoExe_DZoomOut();
				UI_SetData(FL_ZoomIsDigital, TRUE);
			}
		}
		break;

	case UI_ZOOM_CTRL_RESET_DZOOM:
		if ((UI_GetData(FL_ZoomIsDigital) == TRUE) && (DZOOM_IDX_GET() > DZOOM_IDX_MIN()) /*&& (UI_GetData(UI_MovieHotKeyRec) != TRUE)*/) {
			DBG_IND("DZoom Reset\r\n");
			UI_SetData(FL_DzoomReset, TRUE);
			PhotoExe_DZoomReset();
			UI_SetData(FL_ZoomIsDigital, FALSE);
		}
		break;
	case UI_ZOOM_CTRL_RESET_OZOOM:
		uiZoomCnt = 0;
		break;
	default:
		DBG_ERR("Unknown zoom control 0x%x\r\n", uiZoomCtrl);
		break;
	}
	return NVTEVT_CONSUME;
}


INT32 PhotoExe_OnStartFunc(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{

	UINT32   func = 0;
	UINT32   waitIdle = FALSE;

	DBG_IND("func=0x%x\r\n", paramArray[0]);
	if (localInfo->isStartCapture) {
		DBG_ERR("in capturing\r\n");
	} else if (paramNum == 2) {
		func = paramArray[0];
		waitIdle = paramArray[1];

#if _FD_FUNC_
		if (func & UIAPP_PHOTO_FD) {
			FD_Lock(FALSE);
		}
#endif
		if (func & UIAPP_PHOTO_AF) {
			AF_LOCK(FALSE);
		}
		if (waitIdle == TRUE) {
		}
	} else {
		DBG_ERR("wrong param 0x%x\r\n", paramNum);
	}
	return NVTEVT_CONSUME;
}

INT32 PhotoExe_OnStopFunc(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{

	//UINT32   lock= FALSE;
	UINT32   func = 0;
	UINT32   waitIdle = FALSE;

	DBG_IND("func=0x%x\r\n", paramArray[0]);
	if (paramNum == 2) {
		func = paramArray[0];
		waitIdle = paramArray[1];

#if _FD_FUNC_
		if (func & UIAPP_PHOTO_FD) {
			FD_Lock(TRUE);
		}
#endif
		if (func & UIAPP_PHOTO_AF) {
			AF_LOCK(TRUE);
		}
		if (waitIdle == TRUE) {
			AF_WAIT_IDLE();
		}

	} else {
		DBG_ERR("wrong param 0x%x\r\n", paramNum);
	}
	return NVTEVT_CONSUME;
}

////////////////////////////////////////////////////////////
INT32 PhotoExe_OnAFProcess(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
#if (LENS_FUNCTION == ENABLE)
	UINT32 isAFBeam;
	//UINT32 isSupportPunchThrough;
	UINT32 currEV, EV_Threshold, LV;

	isAFBeam = paramArray[0];
#if (ASSIST_BEAM_FUNC == DISABLE)
	isAFBeam = FALSE;
#endif
	//isSupportPunchThrough = paramArray[1];
	DBG_FUNC_BEGIN("[af]\r\n");
	//AE,AWB lock will auto control by capture flow or AF flow
	//AE_LOCK(TRUE);
	//AWB_LOCK(TRUE);
	AF_LOCK(TRUE);
#if _FD_FUNC_
	FD_Lock(TRUE);
#endif
	Ux_FlushEventByRange(NVTEVT_ALGMSG_FOCUSEND, NVTEVT_ALGMSG_FOCUSEND);
	Ux_FlushEventByRange(NVTEVT_EXE_FDEND, NVTEVT_EXE_FDEND);

	currEV = CURR_EV();
	LV     =  currEV / 10;
	EV_Threshold = AF_BEAM_EV_THRESHOLD;
	DBG_IND("[af]EV = %d, LV=%d\r\n", currEV, LV);

	if (isAFBeam && (currEV < EV_Threshold)) { //LV 5.6~6.1
		//If AF beam is on ,then turn on the focus LED before AF_Process().
		if (!localInfo->isAFBeam) {
			if (LV > 6) {
				DBG_ERR("[af]AF LV %d\r\n", LV);
				LED_SetFcsLevel(0);
			} else {
				LED_SetFcsLevel(LCSBRT_LVL_06 - LV);
			}
			LED_TurnOnLED(GPIOMAP_LED_FCS);
			localInfo->isAFBeam = TRUE;
		}
		/* AF_Run no wait. */
		AF_Run(AF_ID_1, FALSE);
		localInfo->IsAFProcess = TRUE;
	} else {
		/* AF_Run no wait. */
		AF_Run(AF_ID_1, FALSE);
		localInfo->IsAFProcess = TRUE;
	}

	DBG_FUNC_END("[af]\r\n");
#endif
	return NVTEVT_CONSUME;
}
INT32 PhotoExe_OnAFRelease(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
#if (LENS_FUNCTION == ENABLE)
	DBG_IND("[af]\r\n");
	AF_Release(AF_ID_1, TRUE);

	if (localInfo->isAFBeam) {
		LED_TurnOffLED(GPIOMAP_LED_FCS);   //If AF beam is on ,then turn off the focus LED after AF_Process().
		LED_SetFcsLevel(LCSBRT_LVL_03);
		localInfo->isAFBeam = FALSE;
	}
	DBG_FUNC_END("[af]\r\n");
#endif
	return NVTEVT_CONSUME;
}

INT32 PhotoExe_OnAFWaitEnd(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
#if (LENS_FUNCTION == ENABLE)
	DBG_IND("[af]\r\n");

	if (localInfo->IsAFProcess) {
		AF_WAIT_IDLE();
		localInfo->IsAFProcess = FALSE;
		if (localInfo->isAFBeam) {
			LED_TurnOffLED(GPIOMAP_LED_FCS);   //If AF beam is on ,then turn off the focus LED after AF_Process().
			LED_SetFcsLevel(LCSBRT_LVL_03);
			localInfo->isAFBeam = FALSE;
		}
	}
	DBG_FUNC_END("[af]\r\n");
#endif
	return NVTEVT_CONSUME;
}

INT32 PhotoExe_OnImageRatio(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
	UINT32           ImageRatioIdx = 0;
	USIZE            ImageRatioSize = {0};
	UINT32           ime3dnr_size = 0, sensor_id;
	URECT            DispCoord;
	UIAPP_PHOTO_SENSOR_INFO *pSensorInfo;
	PHOTO_STRM_INFO    *p_strm = NULL;


    if (paramNum > 0) {
		ImageRatioIdx = paramArray[0];
	}
	DBG_IND("ImageRatioIdx =%d\r\n", ImageRatioIdx);
	if (ImageRatioIdx >= IMAGERATIO_MAX_CNT) {
		DBG_ERR("ImageRatioIdx =%d\r\n", ImageRatioIdx);
		return NVTEVT_CONSUME;
	}
	ImageRatioSize = IMAGERATIO_SIZE[ImageRatioIdx];

	if (g_photo_ImageRatioSize.w == ImageRatioSize.w && g_photo_ImageRatioSize.h == ImageRatioSize.h)
		return NVTEVT_CONSUME;
	g_photo_ImageRatioSize = ImageRatioSize;
	// get display coordinate
	PhotoExe_GetDispCord(&DispCoord);

	for (sensor_id=0;sensor_id<localInfo->sensorCount;sensor_id++) {
		pSensorInfo = UIAppPhoto_get_SensorInfo(sensor_id);
		if (pSensorInfo == NULL) {
			DBG_ERR("get pSensorInfo error\r\n");
			return NVTEVT_CONSUME;
		}
	    ImageApp_Photo_SetVdoAspectRatio(PHOTO_IME3DNR_ID_1+sensor_id,ImageRatioSize.w,ImageRatioSize.h);
		if (pSensorInfo->bIME3DNR) {
		    ime3dnr_size = PhotoExe_AutoCal_IME3DNRSize(sensor_id,pSensorInfo,&ImageRatioSize);
			ImageApp_Photo_SetVdoImgSize(PHOTO_IME3DNR_ID_1+sensor_id, ISF_GET_HI(ime3dnr_size), ISF_GET_LO(ime3dnr_size));
			ImageApp_Photo_ResetPath(PHOTO_IME3DNR_ID_1+sensor_id);
		}
		else {
			ImageUnit_Begin(ISF_IPL(sensor_id), 0);
			ImageUnit_SetVdoAspectRatio(ISF_IN1, ImageRatioSize.w, ImageRatioSize.h);
			ImageUnit_End();
		}
	    ImageApp_Photo_SetVdoImgSize(PHOTO_DISP_ID_1+sensor_id, 0, 0);
	    ImageApp_Photo_SetVdoAspectRatio(PHOTO_DISP_ID_1+sensor_id,ImageRatioSize.w,ImageRatioSize.h);
		ImageApp_Photo_ResetPath(PHOTO_DISP_ID_1+sensor_id);

		p_strm = UIAppPhoto_get_StreamConfig(UIAPP_PHOTO_STRM_ID_1+sensor_id);
		p_strm->width = ALIGN_CEIL_16(p_strm->height* ImageRatioSize.w/ImageRatioSize.h);
		#if (SBS_VIEW_FUNC == ENABLE)
		ImageApp_Photo_SetVdoImgSize(PHOTO_STRM_ID_1+sensor_id, (p_strm->width << 1), p_strm->height);
		#else
		ImageApp_Photo_SetVdoImgSize(PHOTO_STRM_ID_1+sensor_id, p_strm->width, p_strm->height);
        #endif
		#if (DEWARP_FUNC)
		ImageApp_Photo_SetVdoAspectRatio(PHOTO_STRM_ID_1+sensor_id,ImageRatioSize.w,ImageRatioSize.h);
		#else
		ImageApp_Photo_SetVdoAspectRatio(PHOTO_STRM_ID_1+sensor_id,4,1);
		#endif
		ImageApp_Photo_ResetPath(PHOTO_STRM_ID_1+sensor_id);
	}
	// Set Fd image ratio
#if _FD_FUNC_
	{
		localInfo->FdDispCoord = DispCoord;
	}
#endif


	return NVTEVT_CONSUME;
}


IMG_CAP_QV_DATA gPhoto_QvData = {0};

INT32 PhotoExe_OnCallback(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
	NVTEVT event;

	event = paramArray[0];

	DBG_IND("[cb] event=0x%x\r\n", event);
	switch (event) {
	case NVTEVT_ALGMSG_FLASH:
		DBG_IND("[cb]NVTEVT_ALGMSG_FLASH\r\n");
		PhotoExe_CallBackUpdateInfo(UIAPPPHOTO_CB_FLASH);
		break;

	case NVTEVT_ALGMSG_QVSTART:

		DBG_IND("[cb]NVTEVT_ALGMSG_QVSTART\r\n");
		//copy current QV data
		memcpy(&gPhoto_QvData, (IMG_CAP_QV_DATA *)(paramArray[1]), sizeof(IMG_CAP_QV_DATA));
		//Charge flash
		if (UI_GetData(FL_FLASH_MODE) != FLASH_OFF) {
			//#NT#2011/04/15#Lincy Lin -begin
			//#NT#Hera14 HW bug , no battery insert can't charge for flash
			if (!UI_GetData(FL_IsStopCharge))
				//#NT#2011/04/15#Lincy Lin -end
			{
#if (FLASHLIGHT_FUNCTION == ENABLE)
				SxTimer_SetFuncActive(SX_TIMER_DET_RECHARGE_ID, TRUE);
				GxFlash_StartCharge();
#endif

			}
		}
		PhotoExe_CallBackUpdateInfo(UIAPPPHOTO_CB_QVSTART);
		break;

	case NVTEVT_ALGMSG_JPGOK:

		DBG_IND("[cb]NVTEVT_ALGMSG_JPGOK\r\n");

		// add picture count 1
		UI_SetData(FL_TakePictCnt, UI_GetData(FL_TakePictCnt) + 1);
		localInfo->IsJPGok = TRUE;

		localInfo->isFolderFull = UIStorageCheck(STORAGE_CHECK_FOLDER_FULL, NULL);
		localInfo->isCardFull = UIStorageCheck(STORAGE_CHECK_FULL, &localInfo->FreePicNum);
		if (localInfo->isDoingContShot) {
			pPhotoExeInfo->uiTakePicNum++;
			if (localInfo->isFolderFull || localInfo->isCardFull) {
				PhotoExe_StopContShot();
			}
		} else {
			localInfo->BDstatus = PhotoExe_CheckBD();
		}
		PhotoExe_CallBackUpdateInfo(UIAPPPHOTO_CB_JPGOK);
		break;

	case NVTEVT_ALGMSG_CAPFSTOK: {
			INT32  FSsts = paramArray[1];
			DBG_IND("[cb]NVTEVT_ALGMSG_CAPFSTOK\r\n");
			localInfo->isFolderFull = UIStorageCheck(STORAGE_CHECK_FOLDER_FULL, NULL);
			localInfo->isCardFull = UIStorageCheck(STORAGE_CHECK_FULL, &localInfo->FreePicNum);

			if (FSsts != FST_STA_OK) {
				System_SetState(SYS_STATE_FS, FS_DISK_ERROR);
			}
			if (localInfo->isDoingContShot == TRUE) {
				if (localInfo->isFolderFull || localInfo->isCardFull) {
					PhotoExe_StopContShot();
				}
			}
			PhotoExe_CallBackUpdateInfo(UIAPPPHOTO_CB_FSTOK);
		}
		break;


	case NVTEVT_ALGMSG_CAPTUREEND:
		DBG_IND("[cb]NVTEVT_ALGMSG_CAPTUREEND \r\n");
		localInfo->isStartCapture = FALSE;
		localInfo->isStopingContShot = FALSE;
		//Disable USB detection
		SxTimer_SetFuncActive(SX_TIMER_DET_USB_ID, TRUE);
		//Disable USB detection
		SxTimer_SetFuncActive(SX_TIMER_DET_TV_ID, TRUE);
		//Disable Mode detection
		SxTimer_SetFuncActive(SX_TIMER_DET_MODE_ID, TRUE);
		//clear fd number
#if _FD_FUNC_
		FD_ClrRsltFaceNum();
#endif

		PhotoExe_CallBackUpdateInfo(UIAPPPHOTO_CB_CAPTUREEND);
		localInfo->isDoingContShot = FALSE;
		DBG_TEST("{OnCaptureEnd}\r\n");
		exam_msg("{OnCaptureEnd}\r\n");
		#if (WIFI_AP_FUNC==ENABLE)
		WifiCmd_Done(WIFIFLAG_CAPTURE_DONE, E_OK);
        #endif
		break;
	case NVTEVT_ALGMSG_SLOWSHUTTER:
		PhotoExe_CallBackUpdateInfo(UIAPPPHOTO_CB_SLOWSHUTTER);
		break;

	case NVTEVT_ALGMSG_PREVIEW_STABLE:
		DBG_IND("[cb]NVTEVT_ALGMSG_PREVIEW_STABLE \r\n");
		PhotoExe_CallBackUpdateInfo(UIAPPPHOTO_CB_PREVIEW_STABLE);
		break;

	default:

		break;
	}
	return NVTEVT_PASS;
}


INT32 PhotoExe_OnFDEnd(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
	DBG_IND("[cb]\r\n");

	if ((!localInfo->isStartCapture) && (UI_GetData(FL_FD) != FD_OFF)) {
		//Flush FD event before draw
		Ux_FlushEventByRange(NVTEVT_EXE_FDEND, NVTEVT_EXE_FDEND);
		PhotoExe_CallBackUpdateInfo(UIAPPPHOTO_CB_FDEND);
	}
	return NVTEVT_CONSUME;

}


INT32 PhotoExe_OnSHDR(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
	UINT32 uhSelect = 0;
	DBG_IND("%d \r\n", paramArray[0]);
	if (paramNum > 0) {
		uhSelect = paramArray[0];
	}

	UI_SetData(FL_SHDR, uhSelect);

#if SHDR_FUNC
	PhotoExe_IPL_SetUIInfo(IPL_SEL_SHDR, Get_SHDRValue(uhSelect));

	if (UI_GetData(FL_SHDR) == SHDR_ON) {
		UI_SetData(FL_WDR, WDR_OFF);
		UI_SetData(FL_RSC, RSC_OFF);
	}
#endif
	return NVTEVT_CONSUME;
}

INT32 PhotoExe_OnWDR(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
	UINT32 uhSelect = 0;
	DBG_IND("%d \r\n", paramArray[0]);
	if (paramNum > 0) {
		uhSelect = paramArray[0];
	}

	UI_SetData(FL_WDR, uhSelect);
#if WDR_FUNC
	if (UI_GetData(FL_WDR) == WDR_ON && UI_GetData(FL_SHDR) == SHDR_ON) {
		Ux_SendEvent(&CustomPhotoObjCtrl, NVTEVT_EXE_SHDR, 1, SHDR_OFF);
	} else {
		PhotoExe_IPL_SetUIInfo(IPL_SEL_WDR_IMM, Get_WDRValue(uhSelect));
	}
#endif
	exam_msg("photo wdr %d\r\n", uhSelect);
	return NVTEVT_CONSUME;
}
INT32 PhotoExe_OnDefog(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
	UINT32 uhSelect = 0;
	DBG_IND("%d \r\n", paramArray[0]);
	if (paramNum > 0) {
		uhSelect = paramArray[0];
	}

	UI_SetData(FL_DEFOG, uhSelect);

#if DEFOG_FUNC
	PhotoExe_IPL_SetUIInfo(IPL_SEL_DEFOG_IMM, Get_DefogValue(uhSelect));
#endif
	exam_msg("photo DEFOG %d\r\n", uhSelect);
	return NVTEVT_CONSUME;
}

INT32 PhotoExe_OnAntishake(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
	UINT32 uhSelect = 0;
	DBG_IND("%d \r\n", paramArray[0]);
	if (paramNum > 0) {
		uhSelect = paramArray[0];
	}

	UI_SetData(FL_ANTISHAKE, uhSelect);
	PhotoExe_AE_SetUIInfo(AE_UI_ANTISHAKE, Get_AntishakeValue(uhSelect));
	return NVTEVT_CONSUME;
}

INT32 PhotoExe_OnEdge(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
	UINT32 uiSelect = 0;
	DBG_IND("%d \r\n", paramArray[0]);
	if (paramNum) {
		uiSelect = paramArray[0];
	}

	UI_SetData(FL_EDGE, uiSelect);

	if (uiSelect == MOVIE_EDGE_ON) {
		PhotoExe_IQ_SetUIInfo(IQ_UI_IPPEDGE, IQ_EDGE_ON);
	} else {
		PhotoExe_IQ_SetUIInfo(IQ_UI_IPPEDGE, IQ_EDGE_OFF);
	}

	return NVTEVT_CONSUME;
}

INT32 PhotoExe_OnNR(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
	UINT32 uiSelect = 0;
	DBG_IND("%d \r\n", paramArray[0]);
	if (paramNum) {
		uiSelect = paramArray[0];
	}

	UI_SetData(FL_NR, uiSelect);

	if (uiSelect == MOVIE_NR_ON) {
		PhotoExe_IQ_SetUIInfo(IQ_UI_IPPNR, IQ_NR_ON);
	} else {
		PhotoExe_IQ_SetUIInfo(IQ_UI_IPPNR, IQ_NR_OFF);
	}

	return NVTEVT_CONSUME;
}


INT32 PhotoExe_OnFocusEnd(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
#if (LENS_FUNCTION == ENABLE)
	DBG_IND("[cb]\r\n");
	localInfo->IsAFProcess = FALSE;

	if (localInfo->isAFBeam) {
		LED_TurnOffLED(GPIOMAP_LED_FCS);   //If AF beam is on ,then turn off the focus LED after AF_Process().
		LED_SetFcsLevel(LCSBRT_LVL_03);
		localInfo->isAFBeam = FALSE;
	}
	PhotoExe_CallBackUpdateInfo(UIAPPPHOTO_CB_FOCUSEND);
#endif
	return NVTEVT_PASS;
}


INT32 PhotoExe_OnInitDateBuf(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
	if (DATEIMPRINT_OFF != UI_GetData(FL_DATE_STAMP)) {
		UiDateImprint_InitBuff();
	}
	return NVTEVT_CONSUME;
}

INT32 PhotoExe_OnGenDateStr(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
	if (DATEIMPRINT_OFF != UI_GetData(FL_DATE_STAMP)) {
		//UiDateImprint_UpdateDate();
	}
	return NVTEVT_CONSUME;
}



INT32 PhotoExe_OnGenDatePic(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
	if (paramNum != 1) {
		//error parameter
		return NVTEVT_CONSUME;
	}
	if (DATEIMPRINT_OFF != UI_GetData(FL_DATE_STAMP)) {
		UiDateImprint_GenData((IMG_CAP_DATASTAMP_INFO *)paramArray[0]);
	}
	return NVTEVT_CONSUME;
}

INT32 PhotoExe_OnRSC(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
	UINT32 uiSelect = 0;

	DBG_IND("%d \r\n", paramArray[0]);
	if (paramNum) {
		uiSelect = paramArray[0];
	}

	UI_SetData(FL_RSC, uiSelect);

	if (uiSelect == RSC_ON) {
#if (!defined(_Gyro_None_) && (RSC_FUNC == ENABLE))
		PhotoExe_RSC_SetSwitch(SEL_RSC_RUNTIME, SEL_RSC_ON);
#endif
		if (UI_GetData(FL_SHDR) == SHDR_ON) {
			Ux_SendEvent(&CustomPhotoObjCtrl, NVTEVT_EXE_SHDR, 1, SHDR_OFF);
		}
	} else {
#if (!defined(_Gyro_None_) && (RSC_FUNC == ENABLE))
		PhotoExe_RSC_SetSwitch(SEL_RSC_RUNTIME, SEL_RSC_OFF);
#endif
	}
	exam_msg("photo rsc %d\r\n", uiSelect);
	return NVTEVT_CONSUME;
}

INT32 PhotoExe_OnGdc(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
	UINT32 uhSelect = 0;
	if (paramNum > 0) {
		uhSelect = paramArray[0];
	}

	DBG_IND("%d \r\n", uhSelect);
	if (uhSelect == TRUE) {
#if (!defined(_Gyro_None_) && (RSC_FUNC == ENABLE))
		if (UI_GetData(FL_RSC) == RSC_ON) {
			PhotoExe_RSC_SetSwitch(SEL_RSC_RUNTIME, SEL_RSC_OFF);
			DBG_WRN("GDC is enable, so RSC is forced off\r\n");
		}
#endif
		PhotoExe_IQ_SetUIInfo(IQ_UI_GDCCOMP, IQ_GDCCOMP_ON);
	} else {
		PhotoExe_IQ_SetUIInfo(IQ_UI_GDCCOMP, IQ_GDCCOMP_OFF);
	}
	return NVTEVT_CONSUME;
}

INT32 PhotoExe_OnSharpness(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
	UINT32 uhSelect = 0;
	if (paramNum > 0) {
		uhSelect = paramArray[0];
	}

	DBG_IND("%d \r\n", uhSelect);
	UI_SetData(FL_SHARPNESS, uhSelect);
	PhotoExe_IQ_SetUIInfo(IQ_UI_SHARPNESS, Get_SharpnessValue(uhSelect));
	Photo_SetUserIndex(PHOTO_USR_SHARPNESS, uhSelect);
	exam_msg("photo sharpness %d\r\n", uhSelect);
	return NVTEVT_CONSUME;
}


INT32 PhotoExe_OnSaturation(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
	UINT32 uhSelect = 0;
	if (paramNum > 0) {
		uhSelect = paramArray[0];
	}

	DBG_IND("%d \r\n", uhSelect);
	UI_SetData(FL_SATURATION, uhSelect);
	PhotoExe_IQ_SetUIInfo(IQ_UI_SATURATION, Get_SaturationValue(uhSelect));
	return NVTEVT_CONSUME;
}


INT32 PhotoExe_OnPlayShutterSound(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
	GxSound_Stop();
	UISound_Play(DEMOSOUND_SOUND_SHUTTER_TONE);
	return NVTEVT_CONSUME;
}

#if 0
INT32 PhotoExe_OnCaptureID(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
	UINT32 uhSelect = 0;
	if (paramNum > 0) {
		uhSelect = paramArray[0];
	}

	DBG_IND("%d \r\n", uhSelect);

	if (uhSelect >= localInfo->sensorCount) {
		DBG_ERR("uhSelect %d > sensorCount\r\n");
		return NVTEVT_CONSUME;
	}
	localInfo->CaptureID = uhSelect;
	return NVTEVT_CONSUME;
}
#endif

INT32 PhotoExe_OnVideoChange(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
#if (PIP_VIEW_FUNC == ENABLE)
	PipView_SetStyle(UI_GetData(FL_DUAL_CAM));
#endif
	//DBGD(Perf_GetCurrent());
	if (System_GetState(SYS_STATE_CURRMODE) == PRIMARY_MODE_PHOTO) {
		Ux_SendEvent(0, NVTEVT_EXE_IMAGE_RATIO, 1, GetPhotoSizeRatio(UI_GetData(FL_PHOTO_SIZE)));
	}
	//DBGD(Perf_GetCurrent());
	return NVTEVT_PASS; //PASS this event to UIWnd!
}

INT32 PhotoExe_OnDualcam(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
#if (SENSOR_CAPS_COUNT == 2)
	UINT32 uhSelect;
	if (paramNum > 0) {
		uhSelect = paramArray[0];
	} else {
		uhSelect = 0;
	}

	DBG_IND("%d \r\n", uhSelect);


	if (localInfo->sensorCount >= 2) {
		localInfo->DualCam = uhSelect;
	}
	UI_SetData(FL_DUAL_CAM, uhSelect);

	// also change the capture id
	//PhotoExe_OnCaptureID(pCtrl, paramNum, paramArray);
	//if (localInfo->DualCam != uhSelect)
#if (PIP_VIEW_FASTSWITCH==ENABLE)
	{
		Ux_SendEvent(0, NVTEVT_SENSOR_DISPLAY, 1, (SENSOR_1 | SENSOR_2)); //for Always trigger PIP View
		//#NT#2015/11/25#Niven Cho#[87393] -begin
		//Here be invoked at startup without in any mode
		if (System_GetState(SYS_STATE_CURRMODE) == PRIMARY_MODE_PHOTO) {
			Ux_SendEvent(0, NVTEVT_EXE_IMAGE_RATIO, 1, GetPhotoSizeRatio(UI_GetData(FL_PHOTO_SIZE)));
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

#endif
	return NVTEVT_CONSUME;
}




EVENT_ENTRY CustomPhotoObjCmdMap[] = {
	{NVTEVT_EXE_OPEN,               PhotoExe_OnOpen                 },
	{NVTEVT_EXE_CLOSE,              PhotoExe_OnClose                },
	{NVTEVT_EXE_SLEEP,              PhotoExe_OnSleep                },
	{NVTEVT_EXE_WAKEUP,             PhotoExe_OnWakeup               },
	{NVTEVT_EXE_MACRO,              PhotoExe_OnMacro                },
	{NVTEVT_EXE_SELFTIMER,          PhotoExe_OnSelftimer            },
	{NVTEVT_EXE_FLASH,              PhotoExe_OnFlash                },
	{NVTEVT_EXE_EV,                 PhotoExe_OnEV                   },
	{NVTEVT_EXE_CAPTURE_SIZE,       PhotoExe_OnCaptureSize          },
	{NVTEVT_EXE_QUALITY,            PhotoExe_OnQuality              },
	{NVTEVT_EXE_WB,                 PhotoExe_OnWB                   },
	{NVTEVT_EXE_COLOR,              PhotoExe_OnColor                },
	{NVTEVT_EXE_ISO,                PhotoExe_OnISO                  },
	{NVTEVT_EXE_AFWINDOW,           PhotoExe_OnAFWindow             },
	{NVTEVT_EXE_AFBEAM,             PhotoExe_OnAFBeam               },
	{NVTEVT_EXE_CONTAF,             PhotoExe_OnContAF               },
	{NVTEVT_EXE_METERING,           PhotoExe_OnMetering             },
	{NVTEVT_EXE_CAPTURE_MODE,       PhotoExe_OnCaptureMode          },
	{NVTEVT_EXE_DATE_PRINT,         PhotoExe_OnDatePrint            },
	{NVTEVT_EXE_PREVIEW,            PhotoExe_OnPreview              },
	{NVTEVT_EXE_DIGITAL_ZOOM,       PhotoExe_OnDigitalZoom          },
	{NVTEVT_EXE_FD,                 PhotoExe_OnFD                   },
	{NVTEVT_EXE_CONTSHOT,           PhotoExe_OnContShot             },
	{NVTEVT_EXE_SCENEMODE,          PhotoExe_OnSceneMode            },
	{NVTEVT_EXE_CAPTURE_START,      PhotoExe_OnCaptureStart         },
	{NVTEVT_EXE_CAPTURE_STOP,       PhotoExe_OnCaptureStop          },
	{NVTEVT_EXE_CAPTURE_END,        PhotoExe_OnCaptureEnd           },
	{NVTEVT_EXE_ZOOM,               PhotoExe_OnZoom                 },
	{NVTEVT_EXE_RSC,                PhotoExe_OnRSC                  },
	{NVTEVT_EXE_GDC,                PhotoExe_OnGdc                  },
	{NVTEVT_EXE_START_FUNC,         PhotoExe_OnStartFunc            },
	{NVTEVT_EXE_STOP_FUNC,          PhotoExe_OnStopFunc             },
	{NVTEVT_EXE_AF_PROCESS,         PhotoExe_OnAFProcess            },
	{NVTEVT_EXE_AF_RELEASE,         PhotoExe_OnAFRelease            },
	{NVTEVT_EXE_AF_WAITEND,         PhotoExe_OnAFWaitEnd            },
	{NVTEVT_EXE_IMAGE_RATIO,        PhotoExe_OnImageRatio           },
	{NVTEVT_EXE_INIT_DATE_BUF,      PhotoExe_OnInitDateBuf          },
	{NVTEVT_EXE_GEN_DATE_STR,       PhotoExe_OnGenDateStr           },
	{NVTEVT_EXE_GEN_DATE_PIC,       PhotoExe_OnGenDatePic           },
	{NVTEVT_EXE_SHARPNESS,          PhotoExe_OnSharpness            },
	{NVTEVT_EXE_SATURATION,         PhotoExe_OnSaturation           },
	{NVTEVT_EXE_PLAY_SHUTTER_SOUND, PhotoExe_OnPlayShutterSound     },
	{NVTEVT_VIDEO_CHANGE,           PhotoExe_OnVideoChange          },
	{NVTEVT_EXE_DUALCAM,            PhotoExe_OnDualcam              },
	{NVTEVT_EXE_FDEND,              PhotoExe_OnFDEnd                },
	{NVTEVT_EXE_SHDR,               PhotoExe_OnSHDR                  },
	{NVTEVT_EXE_WDR,                PhotoExe_OnWDR                  },
	{NVTEVT_EXE_ANTISHAKING,        PhotoExe_OnAntishake            },
	{NVTEVT_EXE_EDGE,               PhotoExe_OnEdge                 },
	{NVTEVT_EXE_NR,                 PhotoExe_OnNR                   },
	{NVTEVT_CALLBACK,               PhotoExe_OnCallback             },
	{NVTEVT_ALGMSG_FOCUSEND,        PhotoExe_OnFocusEnd             },
	{NVTEVT_EXE_DEFOG,        PhotoExe_OnDefog},
	{NVTEVT_NULL,                   0},
};

CREATE_APP(CustomPhotoObj, APP_SETUP)


BOOL FlowPhoto_CheckReOpenItem(void)
{
	BOOL bReOpen = FALSE;

#if SHDR_FUNC
	if (UI_GetData(FL_SHDR_MENU) != UI_GetData(FL_SHDR)) {
		if (UI_GetData(FL_SHDR_MENU) == SHDR_ON) {
			UI_SetData(FL_WDR, WDR_OFF);
			UI_SetData(FL_WDR_MENU, WDR_OFF);
			UI_SetData(FL_RSC, RSC_OFF);
			UI_SetData(FL_RSC_MENU, RSC_OFF);
		}
		UI_SetData(FL_SHDR, UI_GetData(FL_SHDR_MENU));
		bReOpen = TRUE;
	}
#endif

#if WDR_FUNC
	if (UI_GetData(FL_WDR_MENU) != UI_GetData(FL_WDR)) {
		if (UI_GetData(FL_WDR_MENU) == WDR_ON) {
			UI_SetData(FL_SHDR, SHDR_OFF);
			UI_SetData(FL_SHDR_MENU, SHDR_OFF);
		}
		UI_SetData(FL_WDR, UI_GetData(FL_WDR_MENU));
		bReOpen = TRUE;
	}
#endif

	if (UI_GetData(FL_RSC_MENU) != UI_GetData(FL_RSC)) {
		if (UI_GetData(FL_RSC_MENU) == RSC_ON) {
			UI_SetData(FL_SHDR, SHDR_OFF);
			UI_SetData(FL_SHDR_MENU, SHDR_OFF);
		}
		UI_SetData(FL_RSC, UI_GetData(FL_RSC_MENU));
		bReOpen = TRUE;
	}
#if DEFOG_FUNC
	if (UI_GetData(FL_DEFOG_MENU) != UI_GetData(FL_DEFOG)) {
		UI_SetData(FL_DEFOG, UI_GetData(FL_DEFOG_MENU));
		bReOpen = TRUE;
	}
#endif

	return bReOpen;
}
