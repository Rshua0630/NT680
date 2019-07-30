#include "Type.h"
#include "ImageApp_Photo.h"
#include "UIPhotoInfo.h"
#include "UIMode.h"
#include "UIFlow.h"
#include "iq_ui_info.h"
#include "ae_ui_info.h"
#include "af_ui_info.h"
#include "awb_ui_info.h"


#define THIS_DBGLVL         2 // 0=FATAL, 1=ERR, 2=WRN, 3=UNIT, 4=FUNC, 5=IND, 6=MSG, 7=VALUE, 8=USER
///////////////////////////////////////////////////////////////////////////////
#define __MODULE__          UIPhotoMapping
#define __DBGLVL__          ((THIS_DBGLVL>=PRJ_DBG_LVL)?THIS_DBGLVL:PRJ_DBG_LVL)
#define __DBGFLT__          "*" //*=All, [mark]=CustomClass

#include "DebugModule.h"

typedef struct {
	UINT32  uiWidth;
	UINT32  uiHeight;
	UINT32  uiImageRatio;
	const CHAR *String;
} PHOTO_SIZE_PARAM;

static PHOTO_SIZE_PARAM g_PhotoCapSizeTable[PHOTO_SIZE_ID_MAX + 2] = {
	{4032,  3024,   IMAGERATIO_4_3,     "12M"}, //12M
	{3648,  2736,   IMAGERATIO_4_3,     "10M"}, //10M
	{3264,  2448,   IMAGERATIO_4_3,     "8M"}, // 8M
	{2992,  1696,   IMAGERATIO_16_9,    "5M"}, // 5M
	{2048,  1536,   IMAGERATIO_4_3,     "3M"}, // 3M
	{640,   480,    IMAGERATIO_4_3,     "VGA"},   // VGA
	///////////////////////////////////////////////////////////////////////////////////
	// +2 mode (only for internel test!!!)
	{1280,  960,    IMAGERATIO_4_3,     "1.3M"},  // 1.3M
	{1920,  1080,   IMAGERATIO_16_9,    "2MHD"}, // 2MHD
};

const CHAR *Get_SizeString(UINT32 uiIndex)
{
	if (uiIndex < PHOTO_SIZE_ID_MAX) {
		return g_PhotoCapSizeTable[uiIndex].String;
	} else {
		return NULL;
	}
}

UINT32 GetPhotoSizeWidth(UINT32 ubIndex)
{
	return g_PhotoCapSizeTable[ubIndex].uiWidth;
}

UINT32 GetPhotoSizeHeight(UINT32 ubIndex)
{
	return g_PhotoCapSizeTable[ubIndex].uiHeight;
}

UINT32 GetPhotoSizeRatio(UINT32 ubIndex)
{
	return g_PhotoCapSizeTable[ubIndex].uiImageRatio;
}

/*************************************************
    Dzoom string handling
**************************************************/
// DZoom string
#define MAX_DZOOM_STR_LEN               16
static CHAR g_cDZoomRatioStr[MAX_DZOOM_STR_LEN] = {0};

CHAR *Get_DZoomRatioString(void)
{
	UINT32 m, n;
	UINT32 DzoomIndx;

	DzoomIndx = DZOOM_IDX_GET();
	m = (DzoomIndx + 10) / 10;
	n = DzoomIndx % 10;
	snprintf(g_cDZoomRatioStr, sizeof(g_cDZoomRatioStr), "x%ld.%ld", m, n);
	return g_cDZoomRatioStr;
}

///////////////////////////////////////////////////////////////////
UINT32 Get_MacroIndexFromValue(UINT32 value)
{
	switch (value) {
	case AF_F_MACRO:
		return MACRO_ON;
	case AF_F_AUTO:
		return MACRO_OFF;
	default:
		DBG_ERR("value=%d\r\n", value);
		return MACRO_OFF;
	}
}

UINT32 Get_MacroValue(UINT32 uhIndex)
{
	switch (uhIndex) {
	case MACRO_ON:
		return AF_F_MACRO;
	case MACRO_OFF:
		return AF_F_AUTO;
	default:
		DBG_ERR("uhIndex=%d\r\n", uhIndex);
		return AF_F_AUTO;
	}

}

UINT32 Get_FlashValue(UINT32 uiIndex)
{
	switch (uiIndex) {
	case FLASH_AUTO:
		return AE_FLASH_AUTO;
	case FLASH_RED_EYE:
		return AE_FLASH_AUTOREDEYE;
	case FLASH_OFF:
		return AE_FLASH_FORCEOFF;
	case FLASH_ON:
		return AE_FLASH_FORCEON;
	default:
		DBG_ERR("uiIndex=%d\r\n", uiIndex);
		return 0;
	}
}

UINT32 Get_FlashIndexFromValue(UINT32 value)
{
	switch (value) {
	case AE_FLASH_FORCEON:
		return FLASH_ON;
	case AE_FLASH_AUTO:
		return FLASH_AUTO;
	case AE_FLASH_AUTOREDEYE:
		return FLASH_RED_EYE;
	case  AE_FLASH_FORCEOFF:
		return FLASH_OFF;
	default:
		DBG_ERR("value=%d\r\n", value);
		return FLASH_OFF;
	}
}

UINT32 Get_EVValue(UINT32 uhIndex)
{
	switch (uhIndex) {
	case EV_N20:
		return AE_EV_N2;
	case EV_N16:
		return AE_EV_N5_3;
	case EV_N13:
		return AE_EV_N4_3;
	case EV_N10:
		return AE_EV_N1;
	case EV_N06:
		return AE_EV_N2_3;
	case EV_N03:
		return AE_EV_N1_3;
	case EV_00:
		return AE_EV_00;
	case EV_P03:
		return AE_EV_P1_3;
	case EV_P06:
		return AE_EV_P2_3;
	case EV_P10:
		return AE_EV_P1;
	case EV_P13:
		return AE_EV_P4_3;
	case EV_P16:
		return AE_EV_P5_3;
	case EV_P20:
		return AE_EV_P2;
	default:
		DBG_ERR("uhIndex=%d\r\n", uhIndex);
		return AE_EV_00;
	}
}

UINT32 Get_EVIndexFromValue(UINT32 value)
{
	switch (value) {
	case AE_EV_N2:
		return EV_N20;
	case AE_EV_N5_3:
		return EV_N16;
	case AE_EV_N4_3:
		return EV_N13;
	case AE_EV_N1:
		return EV_N10;
	case AE_EV_N2_3:
		return EV_N06;
	case AE_EV_N1_3:
		return EV_N03;
	case AE_EV_00:
		return EV_00;
	case AE_EV_P1_3:
		return EV_P03;
	case AE_EV_P2_3:
		return EV_P06;
	case AE_EV_P1:
		return EV_P10;
	case AE_EV_P4_3:
		return EV_P13;
	case AE_EV_P5_3:
		return EV_P16;
	case AE_EV_P2:
		return EV_P20;
	default:
		DBG_ERR("value=%d\r\n", value);
		return EV_00;

	}
}

UINT32 Get_QualityValue(UINT32 uhIndex)
{
	switch (uhIndex) {
	case QUALITY_SUPER:
		return 95;
	case QUALITY_FINE:
		return 87;
	case QUALITY_NORMAL:
		return 80;
	default:
		DBG_ERR("uhIndex=%d\r\n", uhIndex);
		return 80;
	}

}


UINT32 Get_WBValue(UINT32 uhIndex)
{
	switch (uhIndex) {
	case WB_AUTO:
		return AWB_WB_AUTO;
	case WB_DAYLIGHT:
		return AWB_WB_DAYLIGHT;
	case WB_CLOUDY:
		return AWB_WB_CLOUDY;
	case WB_TUNGSTEN:
		return AWB_WB_TUNGSTEN;
	case WB_FLUORESCENT:
		return AWB_WB_CUSTOMER1;
	default:
		DBG_ERR("uhIndex=%d\r\n", uhIndex);
		return AWB_WB_AUTO;
	}
}

UINT32 Get_WBIndexFromValue(UINT32 value)
{
	switch (value) {
	case AWB_WB_AUTO:
		return WB_AUTO;
	case AWB_WB_DAYLIGHT:
		return WB_DAYLIGHT;
	case AWB_WB_CLOUDY:
		return WB_CLOUDY;
	case AWB_WB_CUSTOMER1:
		return WB_FLUORESCENT;
	default:
		DBG_ERR("value=%d\r\n", value);
		return WB_AUTO;
	}
}

UINT32 Get_ColorValue(UINT32 uhIndex)
{
	switch (uhIndex) {
	case COLOR_EFFECT_STANDARD:
		return IQ_IMAGEEFFECT_OFF;
	case COLOR_EFFECT_MONOCHROME:
		return IQ_IMAGEEFFECT_BW;
	case COLOR_EFFECT_SEPIA:
		return IQ_IMAGEEFFECT_SEPIA;
	default:
		DBG_ERR("uhIndex=%d\r\n", uhIndex);
		return IQ_IMAGEEFFECT_OFF;
	}
}

UINT32 Get_ISOValue(UINT32 uhIndex)
{
	switch (uhIndex) {
	case ISO_AUTO:
		return AE_ISO_AUTO;
	case ISO_100:
		return AE_ISO_100;
	case ISO_200:
		return AE_ISO_200;
	case ISO_400:
		return AE_ISO_400;
	case ISO_800:
		return AE_ISO_800;
	case ISO_1600:
		return AE_ISO_1600;

	default:
		DBG_ERR("uhIndex=%d\r\n", uhIndex);
		return AE_ISO_AUTO;
	}
}

UINT32 Get_SceneModeValue(UINT32 uhIndex)
{
	switch (uhIndex) {
	case SCENE_LANDSCAPE:
		return AE_SCENE_LANDSCAPE;

	case SCENE_PORTRAIT:
		return AE_SCENE_POTRAIT;

	case SCENE_AUTO:
		return AE_SCENE_AUTO;

	default:
		DBG_ERR("=%d\r\n", uhIndex);
		return AE_SCENE_AUTO;
	}
}
UINT32 Get_SceneModeIndexFromValue(UINT32 uhIndex)
{
	switch (uhIndex) {
	case AE_SCENE_LANDSCAPE:
		return SCENE_LANDSCAPE;
	case AE_SCENE_POTRAIT:
		return SCENE_PORTRAIT;
	case AE_SCENE_AUTO:
		return SCENE_AUTO;
	default:
		DBG_ERR("=%d\r\n", uhIndex);
		return SCENE_AUTO;
	}
}

UINT32 Get_MeteringValue(UINT32 uhIndex)
{
	switch (uhIndex) {
	case METERING_SPOT:
		return AE_METER_SPOT;
	case METERING_CENTER:
		return AE_METER_CENTERWEIGHTED;
	case METERING_AIAE:
	default:
		return AE_METER_USERDEFINE;
	}
}

UINT32 Get_AFWindowIndexFromValue(UINT32 value)
{
	switch (value) {
	case AF_WINDOW_CENTER:
		return AFWINDOW_SINGLE;
	case AF_WINDOW_AUTO:
		return AFWINDOW_MULTI;
	default:
		DBG_ERR("value=%d\r\n", value);
		return AFWINDOW_SINGLE;
	}
}
UINT32 Get_AFWindowValue(UINT32 uhIndex)
{
	switch (uhIndex) {
	case AFWINDOW_SINGLE:
		return AF_WINDOW_CENTER;
	case AFWINDOW_MULTI:
		return AF_WINDOW_AUTO;
	default:
		DBG_ERR("uhIndex=%d\r\n", uhIndex);
		return AF_WINDOW_CENTER;
	}
}

UINT32 Get_FDImageRatioValue(UINT32 value)
{
	switch (value) {
	case IMAGERATIO_4_3:
		return FD_IMAGERATIO_4_3;
	case IMAGERATIO_3_2:
		return FD_IMAGERATIO_3_2;
	case IMAGERATIO_16_9:
		return FD_IMAGERATIO_16_9;
	default:
		DBG_ERR("value=%d\r\n", value);
		return FD_IMAGERATIO_4_3;
	}
}

UINT32 Get_DatePrintValue(UINT32 value)
{
	switch (value) {
	case DATE_STAMP_OFF:
		return SEL_DATASTAMP_OFF;
	case DATE_STAMP_DATE:
	case DATE_STAMP_DATE_TIME:
		return SEL_DATASTAMP_ON;
	default:
		DBG_ERR("value=%d\r\n", value);
		return SEL_DATASTAMP_OFF;
	}
}

UINT32 Get_SharpnessValue(UINT32 value)
{
	switch (value) {
	case SHARPNESS_SHARP:
		return IQ_SHARPNESS_P1;
	case SHARPNESS_NORMAL:
		return IQ_SHARPNESS_NORMAL;
	case SHARPNESS_SOFT:
		return IQ_SHARPNESS_N1;

	default:
		DBG_ERR("value=%d\r\n", value);
		return IQ_SHARPNESS_NORMAL;
	}
}

UINT32 Get_SaturationValue(UINT32 value)
{
	switch (value) {
	case SATURATION_HIGH:
		return IQ_SATURATION_P1;
	case SATURATION_NORMAL:
		return IQ_SATURATION_NORMAL;
	case SATURATION_LOW:
		return IQ_SATURATION_N1;
	default:
		DBG_ERR("value=%d\r\n", value);
		return IQ_SHARPNESS_NORMAL;
	}
}

UINT32 Get_ImageEffectValue(UINT32 value)
{

	switch (value) {
	case IMAGEEFFECT_SKETCH:
		return IQ_IMAGEEFFECT_SKETCH;

	case IMAGEEFFECT_FISHEYE:
		return IQ_IMAGEEFFECT_FISHEYE;

	case IMAGEEFFECT_COLORPENCIL:
		return IQ_IMAGEEFFECT_COLORPENCIL;

	case IMAGEEFFECT_ANTISHAKE:
		return IQ_IMAGEEFFECT_OFF;

	case IMAGEEFFECT_HDR:
		return IQ_IMAGEEFFECT_OFF;

	default:
		DBG_ERR("value=%d\r\n", value);
		return IQ_IMAGEEFFECT_OFF;
	}
}

UINT32 Get_SHDRValue(UINT32 uhIndex)
{
	switch (uhIndex) {
	case SHDR_ON:
		return SEL_SHDR_ON;
	case SHDR_OFF:
		return SEL_SHDR_OFF;
	default:
		DBG_ERR("uhIndex=%d\r\n", uhIndex);
		return SEL_SHDR_ON;
	}
}


UINT32 Get_WDRValue(UINT32 uhIndex)
{
	switch (uhIndex) {
	case WDR_ON:
		return SEL_WDR_AUTO;
	case WDR_OFF:
		return SEL_WDR_OFF;
	default:
		DBG_ERR("uhIndex=%d\r\n", uhIndex);
		return SEL_WDR_AUTO;
	}
}


UINT32 Get_AntishakeValue(UINT32 uhIndex)
{
	switch (uhIndex) {
	case ANTISHAKE_ON:
		return AE_ANTISHAKE_LV1;
	case ANTISHAKE_OFF:
		return AE_ANTISHAKE_LV0;
	default:
		DBG_ERR("uhIndex=%d\r\n", uhIndex);
		return AE_ANTISHAKE_LV0;
	}
}
UINT32 Get_DefogValue(UINT32 uhIndex)
{
	switch (uhIndex) {
	case DEFOG_ON:
		return SEL_DEFOG_ON;
	case DEFOG_OFF:
		return SEL_DEFOG_OFF;
	default:
		DBG_ERR("uhIndex=%d\r\n", uhIndex);
		return SEL_DEFOG_ON;
	}
}



