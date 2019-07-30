/**
    Exif sample code
    Just sample code to show how to use exif lib.
    @file       ExifVendor.c
    @ingroup    Predefined_group_name
    @note       Nothing (or anything need to be mentioned).

    Copyright   Novatek Microelectronics Corp. 2011.  All rights reserved.
*/
#include <string.h>
#include <stdio.h>
#include "math.h"
#include "SysKer.h"
#include "PrjCfg.h"
#include "ExifVendor.h"
#include "ExifDef.h"
#include "Utility.h"
#include "UIInfo.h"
#if(IPCAM_FUNC !=ENABLE)

#include "UIPhotoMapping.h"
#include "UIPhotoInfo.h"
#else
#include "UIAppIPCam_Param.h"
#endif

#include "time.h"
#include "HwClock.h"
#include "ae_api.h"

#if (LENS_FUNCTION == ENABLE)
#include "Lens.h"
#endif
#include "UIAppPhoto.h"
#include "UIMode.h"

#define THIS_DBGLVL         2 // 0=FATAL, 1=ERR, 2=WRN, 3=UNIT, 4=FUNC, 5=IND, 6=MSG, 7=VALUE, 8=USER
///////////////////////////////////////////////////////////////////////////////
#define __MODULE__          ExifVendor
#define __DBGLVL__          ((THIS_DBGLVL>=PRJ_DBG_LVL)?THIS_DBGLVL:PRJ_DBG_LVL)
#define __DBGFLT__          "*" //*=All, [mark]=CustomClass

#include "DebugModule.h"

#define MAKERNOTEID 0x36353000


static UINT32 m_uiPhotoQuality = 0;
static BOOL   m_bCertified = FALSE;
static const INT32 EV_NUMERATOR[(EV_SETTING_MAX)] = {
	20,
	17,
	13,//
	10,//
	7,//
	3,//
	0,//
	-3,//
	-7,//
	-10,//
	-13,//
	-17,//
	-20//
};

typedef  struct {
	UINT32 MakerNoteCheckID;
	UINT16 TestData1;
	UINT16 TestData2;
	//Please reserve two word space to let capture flow save the screennail info.
	UINT32 ScreennailOffset;
	UINT32 ScreennailSize;
} SAMPLE_CODE_MAKER_NOTE, *PSAMPLE_CODE_MAKER_NOTE;

ER ExifCB(EXIF_EVENT event, PMEM_RANGE pBuffer, UINT32 uiRetParamNum, UINT32 *pParamArray)
{
	//UINT32 i;
	DBG_IND("event=%d, Buff Addr=0x%x, Size=0x%x, uiRetParamNum=%d\r\n", event, pBuffer->Addr, pBuffer->Size, uiRetParamNum);
	if (CREATE_MAKERNOTE == event) {
		SAMPLE_CODE_MAKER_NOTE MakerNoteSample = {0};
		MakerNoteSample.MakerNoteCheckID = MAKERNOTEID;
#if (UI_FUNC == ENABLE)
		MakerNoteSample.TestData1 = (UINT16)UI_GetData(FL_QUALITY);
#else
		MakerNoteSample.TestData1 = (UINT16)QUALITY_FINE;
#endif
		MakerNoteSample.TestData2 = 0xA5A5;
		memcpy((void *)pBuffer->Addr, (void *) &MakerNoteSample,  sizeof(MakerNoteSample));
		pBuffer->Size = sizeof(MakerNoteSample);
	} else if (PARSE_MAKERNOTE == event) {
		//user could parse his own makenote data here
		PSAMPLE_CODE_MAKER_NOTE pMakerNoteSample;
		_ALIGNED(4) UINT8 AlignedBuf[sizeof(SAMPLE_CODE_MAKER_NOTE)];
		//pBuffer->Addr may NOT be word-alignd!
		if (pBuffer->Addr & 0x3) {
			memcpy((void *)AlignedBuf, (void *) pBuffer->Addr,  sizeof(SAMPLE_CODE_MAKER_NOTE));
			pMakerNoteSample = (PSAMPLE_CODE_MAKER_NOTE)AlignedBuf;
		} else {
			pMakerNoteSample = (PSAMPLE_CODE_MAKER_NOTE)pBuffer->Addr;
		}
		if (pMakerNoteSample->MakerNoteCheckID == MAKERNOTEID) {
			DBG_MSG("MakerNote TestData1=0x%X, TestData2=0x%X\r\n", pMakerNoteSample->TestData1, pMakerNoteSample->TestData2);
			m_bCertified = TRUE;
			m_uiPhotoQuality = pMakerNoteSample->TestData1;
			//If user wants to let playback decode screennal pic, the screennail info should be returned.
			//pParamArray[0] is the offset of Screennail, which is from the start of JPEG file
			//pParamArray[1] is the size of Screennail
			if (2 == uiRetParamNum) {
				pParamArray[0] = pMakerNoteSample->ScreennailOffset;
				pParamArray[1] = pMakerNoteSample->ScreennailSize;
				return E_OK;
			}
		} else {
			m_bCertified = FALSE;
			m_uiPhotoQuality = MAKERNOTEID;//It means that do NOT show the quality icon.
		}
	} else if (UPDATE_MAKERNOTE == event) {
		//User could record screennail info here. pParamArray[0] is the ScreennailOffset and pParamArray[1] is the ScreennailSize.
		if (2 == uiRetParamNum) {
			SAMPLE_CODE_MAKER_NOTE MakerNoteSample = {0};
			UINT32 Temp1, Temp2;
			//to check if it's our makernote
			Temp1 = Get32BitsData(pBuffer->Addr, TRUE);
			if (MAKERNOTEID == Temp1) {
				DBG_IND("ScreennailOffset=0x%x, Size=0x%x\r\n", pParamArray[0], pParamArray[1]);
				Temp1 = (UINT32)&MakerNoteSample.ScreennailOffset - (UINT32)&MakerNoteSample;
				Temp2 = (UINT32)&MakerNoteSample.ScreennailSize - (UINT32)&MakerNoteSample;
				Set32BitsData(pBuffer->Addr + Temp1, pParamArray[0], TRUE);
				Set32BitsData(pBuffer->Addr + Temp2, pParamArray[1], TRUE);
				return E_OK;
			}
		}
	}
	return E_SYS;
}




//#define EXIF_IMAGE_DESCROPTOIN          "DIGITAL CAMERA "
#define EXIF_MAKER                      "MAKER NAME "
//#define EXIF_MODEL_NAME                 "GoldenSample650"
#define EXIF_SOFTWARE                   "Verx.xx"

//char gExifModel[] ={'N','T','K','9','6','6','3','0',' ',' ',' ',' ',' ',' ',' ',0};

void ExifVendor_Write0thIFD(EXIF_HANDLE_ID HandleID)
{
	UINT32 i;
	CHAR   ImageDescription[] = EXIF_IMAGE_DESCROPTOIN;
	CHAR   Maker[] = EXIF_MAKER;
	CHAR   Model[] = EXIF_MODEL_NAME;
	UINT16  Orientation = 1;
	UINT32  XResolution[2] = {72, 1};
	UINT32  YResolution[2] = {72, 1};
	UINT16  ResolutionUnit = 2;
	CHAR   SoftWare[] = EXIF_SOFTWARE;
	CHAR   DateTime[] = "2012:01:01 01:15:01";
	UINT16  YCbCrPositioning = 2;

	EXIF_SETTAG  ZerothIFDTag[] = {
		{EXIF_IFD_0TH, TAG_ID_IMAGE_DESCRIPTION,   TAG_TYPE_ASCII,  sizeof(ImageDescription), (UINT32)ImageDescription, sizeof(ImageDescription)},
		{EXIF_IFD_0TH, TAG_ID_MAKE,               TAG_TYPE_ASCII,      sizeof(Maker), (UINT32)Maker,       sizeof(Maker)},
		{EXIF_IFD_0TH, TAG_ID_MODEL,              TAG_TYPE_ASCII,      sizeof(Model), (UINT32)Model,       sizeof(Model)},
		{EXIF_IFD_0TH, TAG_ID_ORIENTATION,        TAG_TYPE_SHORT,      1, (UINT32) &Orientation,  sizeof(Orientation)},
		{EXIF_IFD_0TH, TAG_ID_X_RESOLUTION,        TAG_TYPE_RATIONAL,   1, (UINT32)XResolution,       sizeof(XResolution)},
		{EXIF_IFD_0TH, TAG_ID_Y_RESOLUTION,        TAG_TYPE_RATIONAL,   1, (UINT32)YResolution,       sizeof(YResolution)},
		{EXIF_IFD_0TH, TAG_ID_RESOLUTION_UNIT,     TAG_TYPE_SHORT,      1, (UINT32) &ResolutionUnit,        sizeof(ResolutionUnit)},
		{EXIF_IFD_0TH, TAG_ID_SOFTWARE,            TAG_TYPE_ASCII,      sizeof(SoftWare), (UINT32)SoftWare, sizeof(SoftWare)},
		{EXIF_IFD_0TH, TAG_ID_DATE_TIME,           TAG_TYPE_ASCII,      sizeof(DateTime), (UINT32)DateTime,    sizeof(DateTime)},
		{EXIF_IFD_0TH, TAG_ID_Y_CB_CR_POSITIONING,   TAG_TYPE_SHORT,      1, (UINT32) &YCbCrPositioning,    sizeof(YCbCrPositioning)},
	};

	// update datetime
	{
		struct tm DayTime = HwClock_GetTime(TIME_ID_CURRENT);
		//sprintf((char *)DateTime,"%04ld:%02ld:%02ld %02ld:%02ld:%02ld",DayTime.tm_year,DayTime.tm_mon,DayTime.tm_mday,DayTime.tm_hour,DayTime.tm_min,DayTime.tm_sec);
		snprintf((char *)DateTime, sizeof(DateTime), "%04ld:%02ld:%02ld %02ld:%02ld:%02ld", DayTime.tm_year, DayTime.tm_mon, DayTime.tm_mday, DayTime.tm_hour, DayTime.tm_min, DayTime.tm_sec);
		DBG_MSG("DateTime=%s\r\n", DateTime);
	}

	// write each exif data
	for (i = 0; i < sizeof(ZerothIFDTag) / sizeof(EXIF_SETTAG); i++) {
		EXIF_SetTag(HandleID, &ZerothIFDTag[i]);
	}

}
void ExifVendor_WriteExifIFD(EXIF_HANDLE_ID HandleID)
{
	UINT32   i;
	UINT32   uiExposureTime[2] = {1, 32};
	UINT32   uiFNumber[2]      = {28, 10}; // F2.8
	UINT32   uiISOSpeed = 100;
	UINT16   uiExpProgram = ExpPrgNormal;
	CHAR    ExifVersion[] = {'0', '2', '2', '0'};
	CHAR    DateTimeOri[] = "2012:01:01 01:15:01";
	CHAR    DateTimeDigi[] = "2012:01:01 01:15:01";
	CHAR    ComponentCfg[] = {1, 2, 3, 0};
	UINT32   uiShutterSpeed[2] = {50, 10};
	UINT32   uiAperture[2]     = {28, 10}; // F2.8
	UINT32   uiBrightness[2] = {100, 100};// 100
	UINT32   ExpoBias[2] = {0, 10};// 100
	UINT32   MaxAperture[2] = {30, 10};// 30/10 = 3.0
	UINT16   uiMeteringMode = 0;
	UINT16   uiLightSource = 0;
	UINT16   uiStrobeFlash = 0;
	UINT32   FocalLen[2] = {30, 10};// 30/10 = 3.0
	CHAR     UserComment[20] = {0};
	CHAR     FlashPixelVersion[] = {'0', '1', '0', '0'};
	UINT16   uiColorSpace = 1;
	UINT16   uiPixelXDimension = 160;
	UINT16   uiPixelYDimension = 120;
	UINT16   uiSensingMethod = 2;
	UINT32   uiFileSource = 3;  // 3 means DSC
	UINT32   uiSceneType = 1;  // 1 means DSC recored this image
	UINT16   uiCustomRendered = 0;
	UINT16   uiExposureMode = 0;
	UINT16   uiWhiteBalance = 0;
	UINT32   uiDzoomRatio[2] = {100, 100};
	UINT16   uiSceneCaptureType = 0;
	UINT16   uiSharpness = 0;

	//sprintf(UserComment, "EXIF_HDL_ID_%d", HandleID+1);
	snprintf(UserComment, sizeof(UserComment), "EXIF_HDL_ID_%d", HandleID + 1);

	EXIF_SETTAG  ExifIFDTag[] = {
		{EXIF_IFD_EXIF, TAG_ID_EXPOSURE_TIME,       TAG_TYPE_RATIONAL,   1, (UINT32) &uiExposureTime[0],     sizeof(uiExposureTime)},
		{EXIF_IFD_EXIF, TAG_ID_F_NUMBER,            TAG_TYPE_RATIONAL,   1, (UINT32) &uiFNumber[0],          sizeof(uiFNumber)},
		{EXIF_IFD_EXIF, TAG_ID_EXPOSURE_PROGRAM,    TAG_TYPE_SHORT,      1, (UINT32) &uiExpProgram,          sizeof(uiExpProgram)},
		{EXIF_IFD_EXIF, TAG_ID_ISO_SPEED_RATING,    TAG_TYPE_SHORT,      1, (UINT32) &uiISOSpeed,             sizeof(uiISOSpeed)},
		{EXIF_IFD_EXIF, TAG_ID_EXIF_VERSION,        TAG_TYPE_UNDEFINED,  sizeof(ExifVersion), (UINT32) &ExifVersion[0],             sizeof(ExifVersion)},
		{EXIF_IFD_EXIF, TAG_ID_DATE_TIME_ORIGINAL,   TAG_TYPE_ASCII,      sizeof(DateTimeOri), (UINT32) &DateTimeOri[0],    sizeof(DateTimeOri)},
		{EXIF_IFD_EXIF, TAG_ID_DATA_TIME_DIGITIZED,  TAG_TYPE_ASCII,      sizeof(DateTimeDigi), (UINT32) &DateTimeDigi[0],    sizeof(DateTimeDigi)},
		{EXIF_IFD_EXIF, TAG_ID_COMPONENT_CONFIGURATION,      TAG_TYPE_UNDEFINED,  sizeof(ComponentCfg), (UINT32) &ComponentCfg[0],                   sizeof(ComponentCfg)},
		{EXIF_IFD_EXIF, TAG_ID_SHUTTER_SPEED_VALUE,       TAG_TYPE_SRATIONAL,  1, (UINT32) &uiShutterSpeed[0], sizeof(uiShutterSpeed)},
		{EXIF_IFD_EXIF, TAG_ID_APERTURE_VALUE,           TAG_TYPE_RATIONAL,   1, (UINT32) &uiAperture[0],    sizeof(uiAperture)},
		{EXIF_IFD_EXIF, TAG_ID_BRIGHTNESS,         TAG_TYPE_SRATIONAL,  1, (UINT32) &uiBrightness[0], sizeof(uiBrightness)},
		{EXIF_IFD_EXIF, TAG_ID_EXPOSURE_BIAS_VALUE,       TAG_TYPE_SRATIONAL,  1, (UINT32) &ExpoBias[0],    sizeof(ExpoBias)},
		{EXIF_IFD_EXIF, TAG_ID_MAX_APERTURE_RATIO_VALUE,        TAG_TYPE_RATIONAL,   1, (UINT32) &MaxAperture[0], sizeof(MaxAperture)},
		{EXIF_IFD_EXIF, TAG_ID_METERING_MODE,       TAG_TYPE_SHORT,      1, (UINT32) &uiMeteringMode,       sizeof(uiMeteringMode)},
		{EXIF_IFD_EXIF, TAG_ID_LIGHT_SOURCE,        TAG_TYPE_SHORT,      1, (UINT32) &uiLightSource,                sizeof(uiLightSource)},
		{EXIF_IFD_EXIF, TAG_ID_FLASH,        TAG_TYPE_SHORT,      1, (UINT32) &uiStrobeFlash,                sizeof(uiStrobeFlash)},
		{EXIF_IFD_EXIF, TAG_ID_FOCAL_LENGTH,        TAG_TYPE_RATIONAL,   1, (UINT32) &FocalLen[0],    sizeof(FocalLen)},
		{EXIF_IFD_EXIF, TAG_ID_USER_COMMENT,        TAG_TYPE_UNDEFINED,  sizeof(UserComment), (UINT32) &UserComment[0],    sizeof(UserComment)},
		{EXIF_IFD_EXIF, TAG_ID_FLASH_PIX_VERSION,    TAG_TYPE_UNDEFINED,  sizeof(FlashPixelVersion), (UINT32) &FlashPixelVersion[0],                   sizeof(FlashPixelVersion)},
		{EXIF_IFD_EXIF, TAG_ID_COLOR_SPACE,         TAG_TYPE_SHORT,      1, (UINT32) &uiColorSpace,               sizeof(uiColorSpace)},
		{EXIF_IFD_EXIF, TAG_ID_PIXEL_X_DIMENSION,    TAG_TYPE_SHORT,      1, (UINT32) &uiPixelXDimension,                          sizeof(uiPixelXDimension)},
		{EXIF_IFD_EXIF, TAG_ID_PIXEL_Y_DIMENSION,    TAG_TYPE_SHORT,      1, (UINT32) &uiPixelYDimension,                          sizeof(uiPixelYDimension)},
		{EXIF_IFD_EXIF, TAG_ID_SENSING_METHOD,      TAG_TYPE_SHORT,      1, (UINT32) &uiSensingMethod,               sizeof(uiSensingMethod)},
		{EXIF_IFD_EXIF, TAG_ID_FILE_SOURCE,         TAG_TYPE_UNDEFINED,  1, (UINT32) &uiFileSource,                            sizeof(uiFileSource)},
		{EXIF_IFD_EXIF, TAG_ID_SCENE_TYPE,          TAG_TYPE_UNDEFINED,  1, (UINT32) &uiSceneType,                             sizeof(uiSceneType)},
		{EXIF_IFD_EXIF, TAG_ID_CUSTOM_RENDERD,      TAG_TYPE_SHORT,      1, (UINT32) &uiCustomRendered,                sizeof(uiCustomRendered)},
		{EXIF_IFD_EXIF, TAG_ID_EXPOSURE_MODE,       TAG_TYPE_SHORT,      1, (UINT32) &uiExposureMode,                  sizeof(uiExposureMode)},
		{EXIF_IFD_EXIF, TAG_ID_WHITE_BALANCE,       TAG_TYPE_SHORT,      1, (UINT32) &uiWhiteBalance,                  sizeof(uiWhiteBalance)},
		{EXIF_IFD_EXIF, TAG_ID_DZOOM_RATIO,         TAG_TYPE_RATIONAL,   1, (UINT32) &uiDzoomRatio[0],                 sizeof(uiDzoomRatio)},
		{EXIF_IFD_EXIF, TAG_ID_SCENE_CAPTURE_TYPE,  TAG_TYPE_SHORT,      1, (UINT32) &uiSceneCaptureType,              sizeof(uiSceneCaptureType)},
		{EXIF_IFD_EXIF, TAG_ID_SHARPNESS,           TAG_TYPE_SHORT,      1, (UINT32) &uiSharpness,                     sizeof(uiSharpness)}
	};

	/************** datetime **************/
	{
		struct tm DayTime = HwClock_GetTime(TIME_ID_CURRENT);
		//sprintf((char *)DateTimeOri,"%04ld:%02ld:%02ld %02ld:%02ld:%02ld",DayTime.tm_year,DayTime.tm_mon,DayTime.tm_mday,DayTime.tm_hour,DayTime.tm_min,DayTime.tm_sec);
		snprintf((char *)DateTimeOri, sizeof(DateTimeOri), "%04ld:%02ld:%02ld %02ld:%02ld:%02ld", DayTime.tm_year, DayTime.tm_mon, DayTime.tm_mday, DayTime.tm_hour, DayTime.tm_min, DayTime.tm_sec);
		//sprintf((char *)DateTimeDigi,"%04ld:%02ld:%02ld %02ld:%02ld:%02ld",DayTime.tm_year,DayTime.tm_mon,DayTime.tm_mday,DayTime.tm_hour,DayTime.tm_min,DayTime.tm_sec);
		snprintf((char *)DateTimeDigi, sizeof(DateTimeDigi), "%04ld:%02ld:%02ld %02ld:%02ld:%02ld", DayTime.tm_year, DayTime.tm_mon, DayTime.tm_mday, DayTime.tm_hour, DayTime.tm_min, DayTime.tm_sec);
		DBG_MSG("DateTimeOri=%s\r\n", DateTimeOri);
		DBG_MSG("DateTimeDigi=%s\r\n", DateTimeDigi);
	}
	/************** photo w, h **************/
	#if(IPCAM_FUNC != ENABLE)
		uiPixelXDimension = GetPhotoSizeWidth(UI_GetData(FL_PHOTO_SIZE));
	#else
		UIAPPIPCAM_ENCODE_CONFIG *pEncodeConfig = UIAppIPCam_get_EncodeConfig();
		uiPixelXDimension = pEncodeConfig[0].resolution.w;
	#endif
	//#NT#2016/08/30#Lincy Lin -begin
	//#NT# Update correct image width in exif about sidebyside capture
	if (UI_GetData(FL_CONTINUE_SHOT) == CONTINUE_SHOT_SIDE) {
		uiPixelXDimension <<= 1;
	}
	//#NT#2016/08/30#Lincy Lin -end
	#if(IPCAM_FUNC != ENABLE)
		uiPixelYDimension = GetPhotoSizeHeight(UI_GetData(FL_PHOTO_SIZE));
	#else
		uiPixelYDimension = pEncodeConfig[0].resolution.h;
	#endif
	DBG_MSG("uiPixelXDimension=%d, uiPixelYDimension=%d\r\n", uiPixelXDimension, uiPixelYDimension);

	/************** AE info **************/
	{
		UINT32 expo = 0;
		float  fexpo;

#if !defined(_IPL1_IPL_FAKE_)
		AE_GetExifInfo(0, &uiISOSpeed, &expo, &uiAperture[0], &MaxAperture[0]);
#else
		uiISOSpeed = 100;
		expo = 1000000;
		uiAperture[0] = 18;
		MaxAperture[0] = 18;
#endif

		uiFNumber[0] =  uiAperture[0];
		uiAperture[0] = 20 * log2((float)uiAperture[0] / 10);
		MaxAperture[0] = 20 * log2((float)MaxAperture[0] / 10);
		uiShutterSpeed[0] = -log2((float)expo / 1000000) * 10;
		fexpo = (float)expo / 1000000;
		if (fexpo > 1) {
			// the value is integer
			uiExposureTime[0] = fexpo * 10;
			uiExposureTime[1] = 10;
		} else {
			uiExposureTime[0] = 1;
			uiExposureTime[1] = 1 / fexpo;
		}
		DBG_MSG("uiFNumber =%d/%d uiAperture=%d/%d, MaxAperture=%d/%d \r\n", uiFNumber[0], uiFNumber[1], uiAperture[0], uiAperture[1], MaxAperture[0], MaxAperture[1]);
		DBG_MSG("uiShutterSpeed=%d/%d\r\n", uiShutterSpeed[0], uiShutterSpeed[1]);
		DBG_MSG("time = %d us ,uiExposureTime = %d/%d \r\n", expo, uiExposureTime[0], uiExposureTime[1]);
	}
	/************** ExposureMode **************/
	switch (UI_GetData(FL_ModeIndex)) {
	case DSC_MODE_PHOTO_AUTO:
		uiExposureMode = AutoExposure;
		break;

	case DSC_MODE_PHOTO_MANUAL:
	default:
		uiExposureMode = ManualExposure;
		break;
	}
	DBG_MSG("uiExposureMode = %d \r\n", uiExposureMode);
	/************** ExposureProgram **************/
	if (UI_GetData(FL_ModeIndex) == DSC_MODE_PHOTO_SCENE) {
		switch (UI_GetData(FL_SCENE)) {
		case SCENE_PORTRAIT:
		case SCENE_LANDSCAPE:
		case SCENE_NIGHTLANDSCAPE:
			uiExpProgram = ExpPrgCreative;
			break;

		default:
			uiExpProgram = ExpPrgNormal;
			break;
		}
	} else {
		switch (UI_GetData(FL_ModeIndex)) {
		case DSC_MODE_PHOTO_PORTRAIT:
			uiExpProgram = ExpPrgCreative;
			break;

		default:
			uiExpProgram = ExpPrgNormal;
			break;
		}
	}
	DBG_MSG("uiExpProgram = %d \r\n", uiExpProgram);
	/************** MeteringMode **************/
	switch (UI_GetData(FL_METERING)) {
	case METERING_CENTER:
		uiMeteringMode = MeterCenterWeightedAvg;
		break;
	case METERING_SPOT:
		uiMeteringMode = MeterSpot;
		break;
	case METERING_AIAE:
		uiMeteringMode = MeterAverage;
		break;
	default:
		uiMeteringMode = MeterUnknown;
		break;
	}
	DBG_MSG("uiMeteringMode = %d \r\n", uiMeteringMode);
	/************** EV **************/
	{
		ExpoBias[0] = EV_NUMERATOR[UI_GetData(FL_EV)];
		DBG_MSG("ExpoBias = %d/%d \r\n", ExpoBias[0], ExpoBias[1]);
	}
	/************** light source **************/
	switch (UI_GetData(FL_WB)) {
	case WB_AUTO:
		uiLightSource =  LightUnknown;
		break;
	case WB_DAYLIGHT:
		uiLightSource =  LightFineWeather;
		break;
	case WB_CLOUDY :
		uiLightSource =  LightCloudyWeather;
		break;
	case WB_FLUORESCENT1:
		uiLightSource = LightDayWhiteFluorescent;
		break;
	case WB_FLUORESCENT2:
		uiLightSource = LightDaylightFluorescent;
		break;
	case WB_INCAND:
		uiLightSource = LightTungsten;
		break;
	default:
		uiLightSource = LightUnknown;
		break;
	}
	DBG_MSG("uiLightSource = %d \r\n", uiLightSource);
	/************** WB **************/
#if 0
	if (UI_GetData(FL_WB) == WB_AUTO) {
		uiWhiteBalance = AutoWhite;
	} else {
		uiWhiteBalance = ManualWhite;
	}
#else
	uiWhiteBalance = UI_GetData(FL_WB);
#endif
	DBG_MSG("uiWhiteBalance = %d \r\n", uiWhiteBalance);
	/************** Dzoom **************/
#if(PHOTO_MODE==ENABLE)
	uiDzoomRatio[0] = PhotoExe_GetDZoomRatio();
#endif
	if (uiDzoomRatio[0] == 100) {
		uiDzoomRatio[0] = 0; // 0 means Dzoom is not enable
	}
	DBG_MSG("uiDzoomRatio = %d/%d \r\n", uiDzoomRatio[0], uiDzoomRatio[1]);

	/************** Flashlight **************/
	uiStrobeFlash = 0;
	switch (UI_GetData(FL_FLASH_MODE)) {
	case FLASH_OFF:
		uiStrobeFlash = StrobeFlashForceOFF;
		break;
	case FLASH_ON:
		uiStrobeFlash = StrobeFlashForceON | StrobeFired;
		break;
	case FLASH_AUTO:
		uiStrobeFlash = StrobeFlashAuto;
#if 0
		if (CapInfo->Flash) {
			uiStrobeFlash += StrobeFired;
		}
#endif
		break;
	case FLASH_RED_EYE:
		uiStrobeFlash = StrobeFlashAutoRedEye;
#if 0
		if (CapInfo->Flash) {
			uiStrobeFlash += StrobeFired;
		}
#endif
		break;
	case FLASH_SLOW_FLASH_EYE:
		uiStrobeFlash = StrobeFlashForceONRedEye;
		break;
	case FLASH_SLOW_FLASH:
		uiStrobeFlash = StrobeFlashNightStrobe;
		break;
	default:
		uiStrobeFlash = StrobeFlashAuto;
		break;
	}


	DBG_MSG("uiStrobeFlash = 0x%x \r\n", uiStrobeFlash);

	/************** FocalLen **************/
#if (LENS_FUNCTION == ENABLE)
	FocalLen[0] = Lens_FocusGetFocalLength(OZOOM_IDX_GET());
	DBG_MSG("FocalLen = %d/%d \r\n", FocalLen[0], FocalLen[1]);
#endif

	/************** Sharpness **************/
	switch (UI_GetData(FL_SHARPNESS)) {
	case SHARPNESS_SHARP:
		uiSharpness = HARD_SHARPNESS;
		break;
	case SHARPNESS_SOFT:
		uiSharpness = SOFT_SHARPNESS;
		break;
	case SHARPNESS_NORMAL:
	default:
		uiSharpness = NORMAL_SHARPNESS;
		break;
	}
	// write each exif data
	for (i = 0; i < sizeof(ExifIFDTag) / sizeof(EXIF_SETTAG); i++) {
		EXIF_SetTag(HandleID, &ExifIFDTag[i]);
	}

}
void ExifVendor_Write0thIntIFD(EXIF_HANDLE_ID HandleID)
{
	UINT32 i;

	CHAR  InterOper[] = "R98";
	CHAR  Exif98Ver[] = {'0', '1', '0', '0'};
	EXIF_SETTAG  ZerothIntIFDTag[] = {
		{EXIF_IFD_INTEROPERABILITY, TAG_ID_INTER_OPERABILITY_INDEX,  TAG_TYPE_ASCII,      sizeof(InterOper), (UINT32)InterOper, sizeof(InterOper)}, //R98
		{EXIF_IFD_INTEROPERABILITY, TAG_ID_EXIF_98_VERSION,          TAG_TYPE_UNDEFINED,  sizeof(Exif98Ver), (UINT32)Exif98Ver, sizeof(Exif98Ver)}  //0100
	};
	// write each exif data
	for (i = 0; i < sizeof(ZerothIntIFDTag) / sizeof(EXIF_SETTAG); i++) {
		EXIF_SetTag(HandleID, &ZerothIntIFDTag[i]);
	}

}

void ExifVendor_SetData(EXIFVENDOR_DATA_SET attribute, UINT32 value)
{
	switch (attribute) {
	case EXIFVENDOR_QUALITY:
		m_uiPhotoQuality = (UINT32)value;
		break;
	default:
		DBG_ERR("no this attribute");
		break;
	}
}

UINT32 ExifVendor_GetData(EXIFVENDOR_DATA_SET attribute)
{
	switch (attribute) {
	case EXIFVENDOR_CERTIFICATION:
		return m_bCertified;
	case EXIFVENDOR_QUALITY:
		return m_uiPhotoQuality;
	default:
		DBG_ERR("no this attribute");
		return 0;
	}
}


/**
    Get byte number of Rat_num/Rat_denum
*/
static int GetRatByteNum(INT32 Rat)
{
	INT32 RatByteNum ;

	if (Rat > 0) {
		RatByteNum = 1;
	} else {
		return 0;
	}

	while ((Rat = Rat / 10) != 0) {
		RatByteNum++;
	}

	return RatByteNum;
}

static INT32 Power(INT32 base, INT32 time)
{
	INT32 result = 1;

	while (time) {
		result = result * base;
		time --;
	}
	return result;
}
/*
    This function will convert Rat_num/Rat_denum to string ( /-)XX.X (at most 6 Chars)
    if Rat_denum=0 or Rat_num=0, string will be 0
*/
void RationalToStr(INT32 Rat_num, INT32 Rat_denum, UINT8 *pstr)
{
	int    integer, tempchar, tempdigit, remainder;

	if ((Rat_denum != 0) && (Rat_num != 0)) {
		if (Rat_num < 0) {
			*pstr = '-';
			pstr++;
			Rat_num = -Rat_num;
		}

		integer = (int)(Rat_num / Rat_denum);

		remainder = Rat_num - (Rat_denum * integer);

		remainder = remainder * 10;   // + (Rat_denum>>1);

		tempchar = remainder / Rat_denum;

		tempdigit = integer / 10;

		if (tempdigit) {
			*pstr++ = 0x30 + tempdigit ;
		}

		*pstr++ =  0x30 + (integer % 10);
		*pstr++ = '.';

		*pstr++ =  0x30 + tempchar;

		*pstr = 0;        //null string
	} else {
		*pstr = 0x30;
		*(pstr + 1) = 0;
	}
}

void ExposureBiasToStr(INT32 Rat_num, INT32 Rat_denum, UINT8 *pstr)
{
	int    integer, tempchar, tempdigit, remainder, remainder2;

	if ((Rat_denum != 0) && (Rat_num != 0)) {
		if (Rat_num < 0) {
			*pstr = '-';
			Rat_num = -Rat_num;
		} else {
			*pstr = '+';
		}
		pstr++;

		integer = (int)(Rat_num / Rat_denum);

		remainder = Rat_num - (Rat_denum * integer);

		remainder = remainder * 10;

		tempchar = remainder / Rat_denum;

		tempdigit = integer / 10;

		if (tempdigit) { // > 10
			*pstr++ = 0x30 + tempdigit ;
			*pstr++ = 0x30 + (integer % 10);
			*pstr++ = '.';
			*pstr++ =  0x30 + tempchar;
			*pstr = '\0';

		} else {
			remainder2 = remainder - (Rat_denum * tempchar);

			if (remainder2 == 0 || (tempchar != 3 && tempchar != 6)) {
				*pstr++ =  0x30 + integer;
				*pstr++ = '.';
				*pstr++ =  0x30 + tempchar;
				*pstr = '\0';
			} else { // tempchar == 3 or 6
				*pstr++ =  0x30 + integer * 3 + ((tempchar + 3) * 3 / 10);
				*pstr++ = '/';
				*pstr++ =  '3';
				*pstr = '\0';
			}
		}
	} else {
		*pstr++ = '+';
		*pstr++ = '0';
		*pstr++ = '.';
		*pstr++ = '0';
		*pstr = '\0';
	}
}

/**
    This function will convert Rat_num/Rat_denum to string XX/XX (at most 6 Chars)
    if Rat_denum=0 or Rat_num=0, string will be 0
*/
void ExposureTimeToStr(INT32 Rat_num, INT32 Rat_denum, UINT8 *pstr)
{
	INT32    integer, tempdigit;
	INT32    Rat_num_byteNum, Rat_denum_ByteNum;
	INT32    i;

	if ((Rat_denum != 0) && (Rat_num != 0)) {
		if (Rat_num > Rat_denum) {
			Rat_num = Rat_num / Rat_denum;
			Rat_num_byteNum = GetRatByteNum(Rat_num);
			for (i = 0; i < Rat_num_byteNum; i++) {
				integer = Rat_num / Power(10, Rat_num_byteNum - 1 - i);
				Rat_num = Rat_num % Power(10, Rat_num_byteNum - 1 - i);
				*pstr++ = 0x30 + integer ;
			}
			*pstr++ = '/';
			*pstr++ = '1';
		} else {
			if (Rat_num != 0x01) {
				Rat_denum /= Rat_num;
				Rat_num = 0x01;
			}

			Rat_num_byteNum = GetRatByteNum(Rat_num);
			for (i = 0; i < Rat_num_byteNum; i++) {
				integer = Rat_num / Power(10, Rat_num_byteNum - 1 - i);
				Rat_num = Rat_num % Power(10, Rat_num_byteNum - 1 - i);
				*pstr++ = 0x30 + integer ;
			}

			*pstr++ = '/';

			Rat_denum_ByteNum = GetRatByteNum(Rat_denum);
			for (i = 0; i < Rat_denum_ByteNum; i++) {
				tempdigit = Rat_denum / Power(10, Rat_denum_ByteNum - 1 - i);
				Rat_denum = Rat_denum % Power(10, Rat_denum_ByteNum - 1 - i);
				*pstr++ =  0x30 + tempdigit ;
			}
		}

		*pstr = 0;        //null string
	} else {
		*pstr = 0x30;
		*(pstr + 1) = 0;
	}
}
