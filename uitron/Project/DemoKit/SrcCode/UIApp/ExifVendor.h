#ifndef _EXIF_VENDOR_H
#define _EXIF_VENDOR_H
#include "Exif.h"
#include "Type.h"

//----- Exposure Program ---------------------------
#define ExpPrgNoDef            0
#define ExpPrgManual           1
#define ExpPrgNormal           2
#define ExpPrgAperture         3    // Aperture Priority
#define ExpPrgShutter          4    // Shutter Priority
#define ExpPrgCreative         5
#define ExpPrgAction           6
#define ExpPrgPortrait         7
#define ExpPrgLandscape        8
//----- Metering Mode -------------------------------
#define MeterUnknown           0
#define MeterAverage           1
#define MeterCenterWeightedAvg 2
#define MeterSpot              3
#define MeterMultiSpot         4
#define MeterPattern           5
#define MeterPartial           6
//----- Light Source -------------------------------
#define LightUnknown              0
#define LightDaylight             1
#define LightFluorescent          2
#define LightTungsten             3
#define LightFlash                4
#define LightFineWeather          9
#define LightCloudyWeather        10
#define LightDaylightFluorescent  12
#define LightDayWhiteFluorescent  13
#define LightCoolWhiteFluorescent 14
#define LightWhiteFluorescent     15
#define LightStandardA            17
#define LightStandardB            18
#define LightStandardC            19
//----- Strobe Flash -------------------------------
#define StrobeNoFired               0
#define StrobeFired                 1
#define StrobeNoDetectFunction      (0 << 1) // bit[2:1] = 0
#define StrobeReturnNotDetected     (2 << 1) // bit[2:1] = 2
#define StrobeReturnDetected        (3 << 1) // bit[2:1] = 3
#define StrobeModeUnknown           (0 << 3) // bit[3:4] = 0
#define StrobeForceOn               (1 << 3) // bit[3:4] = 1
#define StrobeForceOff              (2 << 3) // bit[3:4] = 2
#define StrobeModeAuto              (3 << 3) // bit[3:4] = 3
#define StrobeFunctionPresent       (0 << 5) // bit[5] = 0
#define StrobeNoFunction            (1 << 5) // bit[5] = 1
#define StrobeRedEyeOff             (0 << 6) // bit[6] = 0
#define StrobeRedEyeOn              (1 << 6) // bit[6] = 1
#define StrobeFlashAuto             ((1<<4)+(1<<3))
#define StrobeFlashForceOFF         ((1<<4))
#define StrobeFlashForceON          ((1<<3))
#define StrobeFlashAutoRedEye       ((1<<6)+(1<<4)+(1<<3))
#define StrobeFlashForceONRedEye    ((1<<6)+(1<<3))
#define StrobeFlashNightStrobe      ((1<<6)+(1<<4)+(1<<3))
//----- Color Space --------------------------------
#define ColorSpaceSRGB         1
//----- Sensing Mode -------------------------------
#define SensingNoDef           1
#define SensingOneChip         2    // OneChipColorArea sensor
#define SensingTwoChip         3
#define SensingThreeChip       4
#define SensingColorSeqArea    5
#define SensingTriLinear       7
#define SensingColorSeqLinear  8
//----- CustomRendered ----------------------------
#define NormalRendered         0
#define CustomRendered         1
//-----Exposure Mode-------------------------------
#define AutoExposure           0
#define ManualExposure         1
#define AutoBracket            2
//-----White Balance ------------------------------
#define AutoWhite              0
#define ManualWhite            1
#define Daylight               2
#define Cloudy                 3
#define Tungsten               4
#define Flurescent             5

//-----SceneCapture Type --------------------------
#define StandardScene          0
#define LandScapeScene         1
#define PortraitScene          2
#define NightScene             3

//-----Gain Control--------------------------------
#define NONE_GAIN              0
#define LOW_GAIN_UP            1
#define HIGH_GAIN_UP           2
#define LOW_GAIN_DOWN          3
#define HIGH_GAIN_DOWN         4

//-----Contrast------------------------------------
#define NORMAL_CONTRAST        0
#define SOFT_CONTRAST          1
#define HARD_CONTRAST          2

//-----Saturation----------------------------------
#define NORMAL_SATURATION      0
#define LOW_SATURATION         1
#define HIGH_SATURATION        2

//-----Sharpness-----------------------------------
#define NORMAL_SHARPNESS       0
#define SOFT_SHARPNESS         1
#define HARD_SHARPNESS         2

//----- Compression -------------------------------
#define UnCompressed           1
#define JPEGCompression        6

typedef enum {
	EXIFVENDOR_CERTIFICATION,
	EXIFVENDOR_QUALITY,
	ENUM_DUMMY4WORD(EXIFVENDOR_DATA_SET)
} EXIFVENDOR_DATA_SET;


extern ER ExifCB(EXIF_EVENT event, PMEM_RANGE pBuffer, UINT32 uiRetParamNum, UINT32 *pParamArray);
extern void ExifVendor_Write0thIFD(EXIF_HANDLE_ID HandleID);
extern void ExifVendor_WriteExifIFD(EXIF_HANDLE_ID HandleID);
extern void ExifVendor_Write0thIntIFD(EXIF_HANDLE_ID HandleID);

extern void ExifVendor_SetData(EXIFVENDOR_DATA_SET attribute, UINT32 value);
extern UINT32 ExifVendor_GetData(EXIFVENDOR_DATA_SET attribute);

extern void ExposureBiasToStr(INT32 Rat_num, INT32 Rat_denum, UINT8 *pstr);
extern void RationalToStr(INT32 Rat_num, INT32 Rat_denum, UINT8 *pstr);
extern void ExposureTimeToStr(INT32 Rat_num, INT32 Rat_denum, UINT8 *pstr);
#endif//_EXIF_VENDOR_H
