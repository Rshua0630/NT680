#ifndef _NVTISP_H
#define _NVTISP_H
/*
	Copyright (c) 2015~	 Novatek Microelectronics Corporation

	@file nvtisp.h

	@version

	@date
*/
#include <stdio.h>
#include "nvt_type.h"
#ifdef __cplusplus
extern "C" {
#endif

#define VIDEO_INPUT_NUM_MAX		1
#define AE_EXP_ROW_NUM			8
#define AE_EXP_COLUMN_NUM		8
#define AE_LUMA_ROW_NUM			32
#define AE_LUMA_COLUMN_NUM		32
#define AE_LUMA_HIST_NUM		64
#define LSC_TABLE_NODE_NUM_MAX	16
#define GAMMA_NODE_NUM			129
typedef struct {
	UINT32  x;      ///< X coordinate of the top-left point of the rectangle
	UINT32  y;      ///< Y coordinate of the top-left point of the rectangle
	UINT32  w;      ///< Rectangle width
	UINT32  h;      ///< Rectangle height
} URECT, *PURECT;

typedef enum {
        ISP_CTRL_MODE_OFF,
	ISP_CTRL_MODE_AUTO,					///< Auto control by ISP
	ISP_CTRL_MODE_MANUAL,				///< Manual control
	ISP_CTRL_MODE_MAX,
	ENUM_DUMMY4WORD(ISPCtrlMode)
} ISPCtrlMode;

typedef enum {
	ISP_COLOR_TYPE_NORMAL,				///< Color image
	ISP_COLOR_TYPE_GRAY,				///< Gray image
	ISP_COLOR_TYPE_MAX,
	ENUM_DUMMY4WORD(ISPColorType)
} ISPColorType;
#if 0
typedef enum {
	ISP_ROTATE_TYPE_NONE,				///< Keep original image
	ISP_ROTATE_TYPE_90,					///< Rotate 90 degrees
	ISP_ROTATE_TYPE_180,				///< Rotate 180 degrees
	ISP_ROTATE_TYPE_270,				///< Rotate 270 degrees
	ISP_ROTATE_TYPE_MAX,
	ENUM_DUMMY4WORD(ISPRotateType)
} ISPRotateType;
#endif
typedef enum {
	ISP_IRCUT_TYPE_NIGHT,				///< Set IR cut filter mode to night, image color switch to gray mode.
	ISP_IRCUT_TYPE_DAY,					///< Set IR cut filter mode to day, image color switch to color mode.
	ISP_IRCUT_TYPE_AUTO,				///< IR cut filter will be controlled by ISP.
	ISP_IRCUT_TYPE_MAX,
	ENUM_DUMMY4WORD(ISPIRCutType)
} ISPIRCutType;

typedef struct {
	UINT32 uiBrightness;				///< Brightness strength. Range: 0~100, default value is 50
	UINT32 uiContrast;					///< Contrast strength. Range: 0~100, default value is 50
	UINT32 uiSaturation;				///< Saturation strength. Range: 0~100, default value is 50
	UINT32 uiHue;						///< Hue strength. Range: 0~360, default value is 0
	UINT32 uiSharpness;					///< Sharpness strength. Range: 0~100, default value is 50
	ISPColorType eColor;				///< Image color mode. Default value is #ISP_COLOR_TYPE_NORMAL.
	BOOL bMirror;						///< Enable mirror. Default value is NVT_FALSE.
	BOOL bFlip;							///< Enable flip. Default value is NVT_FALSE.
	//ISPRotateType eRotate;				///< Image rotation mode (This parameter is not supported.)
	UINT32 uiAspectRatio;				///< Image aspect ratio, set image source to 4:3 or 16:9. Value 4 means 4:3, and value 6 means 16:9. Default value is 6.
	ISPIRCutType eIRCutMode;			///< IR cut control mode. Default value is ISP_IRCUT_TYPE_AUTO.
	BOOL bDewarp;						///< Dewarp Mode control
} ISPImageSetting;

/* Auto Exposure */
typedef enum {
	ISP_ANTIFLICKER_50HZ,				///< 50Hz anti flicker
	ISP_ANTIFLICKER_60HZ,				///< 60Hz anti flicker
	ISP_ANTIFLICKER_AUTO,				///< Auto detect anti flicker
	ISP_ANTIFLICKER_NONE,				///< Disable anti flicker
	ENUM_DUMMY4WORD(ISPAntiFlicker)
} ISPAntiFlicker;

typedef enum {
	ISP_AE_STRATEGY_HIGHLIGHT,			///< Auto exposure focus on bright area
	ISP_AE_STRATEGY_LOWLIGHT,			///< Auto exposure focus on dark area
	ISP_AE_STRATEGY_MAX,
	ENUM_DUMMY4WORD(ISPAEStrategy)
} ISPAEStrategy;

typedef enum {
	ISP_AE_IRIS_TYPE_FIXED,				///< Fixed iris
	ISP_AE_IRIS_TYPE_DC,				///< DC-iris
	ISP_AE_IRIS_TYPE_P,					///< P-iris
	ISP_AE_IRIS_TYPE_MAX,
	ENUM_DUMMY4WORD(ISPAEIrisType)
} ISPAEIrisType;

typedef enum {
	ISP_AE_IRIS_MODE_AUTO,				///< Auto control iris size by ISP
	ISP_AE_IRIS_MODE_CLOSE,				///< Close the iris
	ISP_AE_IRIS_MODE_OPEN,				///< Open the iris to maximum size
	ISP_AE_IRIS_MODE_MANUAL,			///< Set iris size manually, only for P-iris
	ISP_AE_IRIS_MODE_MAX,
	ENUM_DUMMY4WORD(ISPAEIrisMode)
} ISPAEIrisMode;

typedef struct {
	ISPAEIrisType eIrisType;			///< Iris type (This parameter is not supported.)
	ISPAEIrisMode eIrisMode;			///< Iris control mode (This parameter is not supported.)
	UINT32 uiDCIrisSpeed;				///< DC-iris drive speed (This parameter is not supported.)
	UINT32 uiPIrisManualSize;			///< P-iris manual size (This parameter is not supported.)
	UINT32 uiPIrisAutoMaxSize;			///< Maximum size of P-iris auto control (This parameter is not supported.)
	UINT32 uiPIrisAutoMinSize;			///< Minimum size of P-iris auto contro (This parameter is not supported.)
} ISPAEIrisSetting;

typedef struct {
	ISPCtrlMode eAEMode;				///< Exposure control mode. Default value is #ISP_CTRL_MODE_AUTO.
	UINT32 uiShutterSpeedMax;			///< Maximum shutter speed(Unit is microsecond). Range: uiShutterSpeedMin~200000, default value is 33333
	UINT32 uiShutterSpeedMin;			///< Minimum shutter speed(Unit is microsecond). Range: 10~uiShutterSpeedMax, default value is 100
	UINT32 uiGainMax;					///< Maximum gain value(ISO). Range: uiGainMin~51200
	UINT32 uiGainMin;					///< Minimum gain value(ISO). Range: 50~uiGainMax
	UINT32 uiConvergenceSpeed;			///< The adjustment speed of auto exposure. Range: 0~6, default value is 4
	UINT32 uiConvergenceStep;			///< The adjustment step of auto exposure. This parameter is not supported.
	UINT32 uiManualShutterSpeed;		///< Shutter speed for manual control(Unit is microsecond). Range: 10~200000
	UINT32 uiManualGain;				///< Gain for manual control(ISO). Range: 10~51200
	ISPAntiFlicker eAntiFlicker;		///< Anti flicker mode. Default value is ISP_ANTIFLICKER_60HZ.
	ISPAEIrisSetting sIrisSetting;		///< Iris control setting. This parameter is not supported.
	UINT32 uiEV;						///< Exposure value, the target luminance of auto exposure. Range: 0~24, default value is 12
	UINT32 uiAETolerance;				///< The tolerance between target and real luminance. If the difference between target and real luminance, AE will adjust again. Range: 3~100, default value is 3
	UINT32 uiAEDelay;					///< The delay of AE adjustment trigger(Unit is frame). Range: 0~150, default value is 0
	UINT8 uiExpAreaWeight[AE_EXP_ROW_NUM*AE_EXP_COLUMN_NUM];	///< Exposure area weight. Range: 0~255
	ISPAEStrategy eAEStrategy;			///< Auto exposure strategy. Default value is ISP_AE_STRATEGY_HIGHLIGHT
} ISPAESetting;

typedef struct {
	UINT32 uiShutter;					///< Current shutter speed(Unit is microsecond)
	UINT32 uiGain;						///< Current gain(ISO)
	UINT16 uiLumaValue[AE_LUMA_ROW_NUM*AE_LUMA_COLUMN_NUM];	///< Current luma value(0~1023). AE_LUMA_ROW_NUM is 32. AE_LUMA_COLUMN_NUM is 32
	UINT32 uiLumaAverage;				///< Current average luma
	UINT16 uiLumaHistogram[AE_LUMA_HIST_NUM];	///< Current luma histogram. AE_LUMA_HIST_NUM is 64
} ISPAEStatus;

/* White Balance */
typedef struct {
	ISPCtrlMode eWBMode;				///< White balance control mode. Default value is ISP_CTRL_MODE_AUTO.
	UINT32 uiConvergenceSkipFrame;			///< The adjustment skip frame of auto white balance. Range: 1~10, default value is 4
	UINT32 uiConvergenceSpeed;			///< The adjustment speed of auto white balance. Range: 1~10, default value is 10
	UINT32 uiRratio;                    ///< User R ratio. Range: 1~400
	UINT32 uiBratio;                    ///< User B ratio. Range: 1~400
	UINT32 uiManualRGain;				///< Manual red gain value. Range: 1~2047
	UINT32 uiManualGGain;				///< Manual green gain value. This parameter is not supported.
	UINT32 uiManualBGain;				///< Manual blue gain value. Range: 1~2047
} ISPWBSetting;

typedef struct {
	UINT32 uiRGain;						///< Current R gain
	UINT32 uiGGain;						///< Current G gain
	UINT32 uiBGain;						///< Current B gain
    UINT32 uiRratio;                    ///< User R ratio
	UINT32 uiBratio;                    ///< User B ratio
	UINT32 uiColorTemperature;			///< Current color temperature
} ISPWBStatus;

/* WDR */
typedef struct {
	ISPCtrlMode eWDRMode;				///< WDR control mode
	UINT32 uiWDRStrength;				///< WDR strength for manual control. Range: 0~10
} ISPWDRSetting;

/* HDR */
typedef struct {
	BOOL bHDREnable;				///< Set HDR enable or disable
        UINT32 uiFrameNum;                               ///< Frame Number 
} ISPHDRSetting;

/* Noise Reduction */
typedef struct {
	UINT32 uiNR2DStrength;				///< Noise reduction strength in space domain. Range: 1~100, 0 means disable
	UINT32 uiNR3DStrength;				///< 0:disable ISP 3DNR  1: enable ISP 3dnr
} ISPNRSetting;

/* Digital Image Stabilization */
typedef struct {
	BOOL bDISEnable;					///< Enable DIS. Default value is NVT_FALSE
} ISPDISSetting;

/* Lens Distortion Correction */
typedef struct {
	BOOL bLDCEnable;					///< Enable LDC. Default value is NVT_FALSE.
	UINT32 uiRatio;						///< Correct ratio. Range: 1~100
} ISPLDCSetting;

/* Lens Shading Correction */
typedef struct {
	BOOL bLSCEnable;					///< Enable LSC. Default value is NVT_FALSE.
	UINT32 uiRatio;						///< Correct ratio. Range: 1~100
	UINT32 uiCenterXOffset;				///< Center offset of X axis. This parameter is not supported.
	UINT32 uiCenterYOffset;				///< Center offset of Y axis. This parameter is not supported.
} ISPLSCSetting;

/* DeFog */
typedef enum _SEL_DEFOG {
	ISP_DEFOG_OFF = 0,  ///<-defog disable
	ISP_DEFOG_ON,       ///<-defoe enable
	ISP_DEFOG_MAX_CNT,
	ENUM_DUMMY4WORD(ISP_DEFOG_MODE)
} ISP_DEFOG_MODE;

typedef struct {
	ISP_DEFOG_MODE eDeFogMode;			///< defog control mode.
	UINT32 uiDeFogStrength;			        ///< defog strength for manual control. Range: 0~255
} ISPDeFogSetting;

/* Gamma */
typedef enum {
	ISP_GAMMA_CURVE_MODE_LINEAR,		///< Linear gamma curve
	ISP_GAMMA_CURVE_MODE_DEFAULT,		///< Factory default gamma curve
	ISP_GAMMA_CURVE_MODE_USER,			///< User define gamma curve
	ISP_GAMMA_CURVE_MODE_16,			///< Gamma curve 1.6
	ISP_GAMMA_CURVE_MODE_18,			///< Gamma curve 1.8
	ISP_GAMMA_CURVE_MODE_20,			///< Gamma curve 2.0
	ISP_GAMMA_CURVE_MODE_22,			///< Gamma curve 2.2
	ISP_GAMMA_CURVE_MODE_24,			///< Gamma curve 2.4
	ISP_GAMMA_CURVE_MODE_MAX,
	ENUM_DUMMY4WORD(ISPGammaCurveMode)
} ISPGammaCurveMode;

/*  Crop */
typedef struct{

	UINT32 	uiSensor;            ///< SensorID,  value:0~8
	UINT32 	uiIME;               ///< IMEout ID, support 0,1,2,3,4
	BOOL	bEnable;             ///< Enable/Disable crop function 0: Disable, 1: Enable
	URECT	sCropInfo;           ///< crop information reference Sensor input size
	UINT32 	uiSensorWidth;       ///< Sensor input width (read only)
	UINT32  uiSensorHeight;      ///< Sensor input Height (read only)
	UINT32  uiScaleWidth;        ///< scaling crop width for final resolution
	UINT32	uiScaleHeight;       ///< scaling crop height for final resolution
}ISPImageCropSetting;


typedef struct {
	ISPGammaCurveMode eGammaCurveMode;			///< Gamma curve mode
	UINT16 uiUserGammaTable[GAMMA_NODE_NUM];	///< Gamma table in user mode
} ISPGammaSetting;

/* Color Correction Matrix */
typedef struct {
	ISPCtrlMode eCCMMode;				///< CCM control mode, ISP_CTRL_MODE_MANUAL will use uiHighCCM / uiMiddleCCM / uiLowCCM
	UINT32 uiHighColorTemp;				///< High color temperature (This parameter is not supported)
	UINT16 uiHighCCM[9];				///< CCM in high color temperature
	UINT32 uiMiddleColorTemp;			///< Middle color temperature (This parameter is not supported.)
	UINT16 uiMiddleCCM[9];				///< CCM in middle color temperature
	UINT32 uiLowColorTemp;				///< Low color temperature (This parameter is not supported.)
	UINT16 uiLowCCM[9];					///< CCM in low color temperature
} ISPCCMSetting;

typedef struct {
	BOOL bEnable;						///< Enable or disable PIP function
}ISPPIPSetting;

typedef struct
{
		UINT32 uiWidth;
		UINT32 uiHeight;
		UINT32 uiBits;
		UINT32 uiStartpix;
		UINT32 uiB2strp;
		UINT32 uiLineoffset[2];
		UINT32 uiOverlap;
		UINT32 uiRawsize;
		UINT32 uiAddr;
} ISPTuningRawInfo;


/* SBS */
typedef enum{
	ISP_SBS_OFF = 0,	 				///<-normal case, side by side off
	ISP_SBS_LR,			 				///<-Left-right side by side
	ISP_SBS_UD,			 				///<-up-down side by side
	ISP_SBS_MAX_CNT,
} ISPSBSMode;


typedef struct{
	ISPSBSMode eSBSMode;				///< Set SBS mode
}ISPSBSSetting;



typedef enum _ISP_IME_SCALER{
	ISP_IME_BICUBIC     = 0,    ///< bicubic interpolation
	ISP_IME_BILINEAR    = 1,    ///< bilinear interpolation
	ISP_IME_NEAREST     = 2,    ///< nearest interpolation
	ISP_IME_INTEGRATION = 3,    ///< integration interpolation,support only scale down
	ISP_IME_AUTO        = 4,    ///< automatical calculation
	ISP_IME_SCALER_MAX,
	ENUM_DUMMY4WORD(ISP_IME_SCALER)
} ISP_IME_SCALER;

typedef struct{
	UINT32	                   uiRatioThreshold; ///< ime scale ratio threshold [31:16]/[15:0](src:bit[31:16], div:bit[15:0])
	ISP_IME_SCALER		   eMathodLarge; /// using this method if above the threshold
	ISP_IME_SCALER		   eMathodSmall; /// using this method if below the threshold 
}ISPIMEScaleSetting;


/*SBS*/
/**
	Get current SBS setting from ISP.

	@param[in] uiViChannelID		Index of face tracking frame.
	@param[out] psSBSSetting		Pointer to the data structure of SBS setting

	@return NvtErrorCode (Refer to nvt_type.h)
*/
extern NVTRET NvtISP_GetSBSSetting(UINT8 uiViChannelID, ISPSBSSetting *psSBSSetting);

/**
	Set new SBS setting to ISP.

	@param[in] uiViChannelID		Index of face tracking frame.
	@param[in] psSBSSetting			Pointer to the data structure of SBS setting

	@return NvtErrorCode (Refer to nvt_type.h)
*/
extern NVTRET NvtISP_SetSBSSetting(UINT8 uiViChannelID, ISPSBSSetting *psSBSSetting);


/* PIP */
/**
	Get current PIP setting from ISP.

	@param[in] uiViChannelID		Index of face tracking frame.
	@param[out] psPIPSetting		Pointer to the data structure of PIP setting

	@return NvtErrorCode (Refer to nvt_type.h)
*/
extern NVTRET NvtISP_GetPIPSetting(UINT8 uiViChannelID, ISPPIPSetting *psPIPSetting);

/**
	Set new PIP setting to ISP.

	@param[in] uiViChannelID		Index of face tracking frame.
	@param[in] psPIPSetting			Pointer to the data structure of PIP setting

	@return NvtErrorCode (Refer to nvt_type.h)
*/
extern NVTRET NvtISP_SetPIPSetting(UINT8 uiViChannelID, ISPPIPSetting *psPIPSetting);

/* Image Setting */

/**
	Get current image setting from ISP.

	@param[in] uiViChannelID		Index of face tracking frame.
	@param[out] psImageSetting		Pointer to the data structure of image setting

	@return NvtErrorCode (Refer to nvt_type.h)
*/
extern NVTRET NvtISP_GetImageSetting(UINT8 uiViChannelID, ISPImageSetting *psImageSetting);

/**
	Set new image setting to ISP.

	@param[in] uiViChannelID		Index of face tracking frame.
	@param[in] psImageSetting		Pointer to the data structure of image setting

	@return NvtErrorCode (Refer to nvt_type.h)
*/
extern NVTRET NvtISP_SetImageSetting(UINT8 uiViChannelID, ISPImageSetting *psImageSetting);

/* Auto Exposure */
/**
	Get current auto exposure setting from ISP

	@param[in] uiViChannelID		Video input channel ID
	@param[out] psAESetting			Pointer to the data structure of auto exposure setting

	@return NvtErrorCode (Refer to nvt_type.h)
*/
extern NVTRET NvtISP_GetAESetting(UINT8 uiViChannelID, ISPAESetting *psAESetting);

/**
	Set new auto exposure setting to ISP.

	@param[in] uiViChannelID		Video input channel ID
	@param[in] psAESetting			Pointer to the data structure of auto exposure setting

	@return NvtErrorCode (Refer to nvt_type.h)
*/
extern NVTRET NvtISP_SetAESetting(UINT8 uiViChannelID, ISPAESetting *psAESetting);

/**
	Get current auto exposure statistics information.

	@param[in] uiViChannelID		Video input channel ID
	@param[out] psAEStatus			Pointer to the data structure of auto exposure statistics
	@param[in] uiRowNum				Row number of block for AE statistics. Range: 4~32
	@param[in] uiColumnNum			Column number of block for AE statistics. Range: 4~32

	@return NvtErrorCode (Refer to nvt_type.h)
*/
extern NVTRET NvtISP_GetAEStatus(UINT8 uiViChannelID, ISPAEStatus *psAEStatus, UINT8 uiRowNum, UINT8 uiColumnNum);

/* White Balance */
/**
	Get current white balance setting from ISP.

	@param[in] uiViChannelID		Video input channel ID
	@param[out] psWBSetting			Pointer to the data structure of white balance setting

	@return NvtErrorCode (Refer to nvt_type.h)
*/
extern NVTRET NvtISP_GetWBSetting(UINT8 uiViChannelID, ISPWBSetting *psWBSetting);

/**
	Set new white balance setting to ISP.

	@param[in] uiViChannelID		Video input channel ID
	@param[in] psWBSetting			Pointer to the data structure of white balance setting

	@return NvtErrorCode (Refer to nvt_type.h)
*/
extern NVTRET NvtISP_SetWBSetting(UINT8 uiViChannelID, ISPWBSetting *psWBSetting);

/**
	Get current white balance statistics information.

	@param[in] uiViChannelID		Video input channel ID
	@param[out] psWBStatus			Pointer to the data structure of white balance statistics

	@return NvtErrorCode (Refer to nvt_type.h)
*/
extern NVTRET NvtISP_GetWBStatus(UINT8 uiViChannelID, ISPWBStatus *psWBStatus);

/* WDR */
/**
	Get current WDR setting from ISP.

	@param[in] uiViChannelID		Video input channel ID
	@param[out] psWDRSetting		Pointer to the data structure of WDR setting

	@return NvtErrorCode (Refer to nvt_type.h)
*/
extern NVTRET NvtISP_GetWDRSetting(UINT8 uiViChannelID, ISPWDRSetting *psWDRSetting);

/**
	Set new WDR setting to ISP.

	@param[in] uiViChannelID		Video input channel ID
	@param[in] psWDRSetting			Pointer to the data structure of WDR setting

	@return NvtErrorCode (Refer to nvt_type.h)
*/
extern NVTRET NvtISP_SetWDRSetting(UINT8 uiViChannelID, ISPWDRSetting *psWDRSetting);

/* HDR */
/**
	Get current HDR setting from ISP.

	@param[in] uiViChannelID		Video input channel ID
	@param[out] psHDRSetting		Pointer to the data structure of HDR setting

	@return NvtErrorCode (Refer to nvt_type.h)
*/
extern NVTRET NvtISP_GetHDRSetting(UINT8 uiViChannelID, ISPHDRSetting *psHDRSetting);

/**
	Set new HDR setting to ISP.

	@param[in] uiViChannelID		Video input channel ID
	@param[in] psHDRSetting			Pointer to the data structure of HDR setting

	@return NvtErrorCode (Refer to nvt_type.h)
*/
extern NVTRET NvtISP_SetHDRSetting(UINT8 uiViChannelID, ISPHDRSetting *psHDRSetting);

/* Noise Reduction */
/**
	Get current NR setting from ISP.

	@param[in] uiViChannelID		Video input channel ID
	@param[out] psNRSetting			Pointer to the data structure of NR setting

	@return NvtErrorCode (Refer to nvt_type.h)
*/
extern NVTRET NvtISP_GetNRSetting(UINT8 uiViChannelID, ISPNRSetting *psNRSetting);

/**
	Set new NR setting to ISP.

	@param[in] uiViChannelID		Video input channel ID
	@param[in] psNRSetting			Pointer to the data structure of NR setting

	@return NvtErrorCode (Refer to nvt_type.h)
*/
extern NVTRET NvtISP_SetNRSetting(UINT8 uiViChannelID, ISPNRSetting *psNRSetting);

/* Digital Image Stabilization */
/**
	Get current DIS setting from ISP.

	@param[in] uiViChannelID		Video input channel ID
	@param[out] psDISSetting		Pointer to the data structure of DIS setting

	@return NvtErrorCode (Refer to nvt_type.h)
*/
extern NVTRET NvtISP_GetDISSetting(UINT8 uiViChannelID, ISPDISSetting *psDISSetting);

/**
	Set new DIS setting to ISP.

	@param[in] uiViChannelID		Video input channel ID
	@param[in] psDISSetting			Pointer to the data structure of DIS setting

	@return NvtErrorCode (Refer to nvt_type.h)
*/
extern NVTRET NvtISP_SetDISSetting(UINT8 uiViChannelID, ISPDISSetting *psDISSetting);

/* Lens Distortion Correction */
/**
	Get current LDC setting from ISP.

	@param[in] uiViChannelID		Video input channel ID
	@param[out] psLDCSetting		Pointer to the data structure of LDC setting

	@return NvtErrorCode (Refer to nvt_type.h)
*/
extern NVTRET NvtISP_GetLDCSetting(UINT8 uiViChannelID, ISPLDCSetting *psLDCSetting);

/**
	Set new LDC setting to ISP.

	@param[in] uiViChannelID		Video input channel ID
	@param[in] psLDCSetting			Pointer to the data structure of LDC setting

	@return NvtErrorCode (Refer to nvt_type.h)
*/
extern NVTRET NvtISP_SetLDCSetting(UINT8 uiViChannelID, ISPLDCSetting *psLDCSetting);

/* Lens Shading Correction */
/**
	Get current LSC setting from ISP.

	@param[in] uiViChannelID		Video input channel ID
	@param[out] psLSCSetting		Pointer to the data structure of LSC setting

	@return NvtErrorCode (Refer to nvt_type.h)
*/
extern NVTRET NvtISP_GetLSCSetting(UINT8 uiViChannelID, ISPLSCSetting *psLSCSetting);

/**
	Set new LSC setting to ISP.

	@param[in] uiViChannelID		Video input channel ID
	@param[in] psLSCSetting			Pointer to the data structure of LSC setting

	@return NvtErrorCode (Refer to nvt_type.h)
*/
extern NVTRET NvtISP_SetLSCSetting(UINT8 uiViChannelID, ISPLSCSetting *psLSCSetting);

/*  DeFog */
/**
	Get current Defog setting from ISP.

	@param[in] uiViChannelID		Video input channel ID
	@param[out] psDeFogSetting	Pointer to the data structure of Defog setting

	@return NvtErrorCode (Refer to nvt_type.h)
*/
extern NVTRET NvtISP_GetDeFogSetting(UINT8 uiViChannelID, ISPDeFogSetting *psDeFogSetting);

/**
	Set new Defog setting to ISP.

	@param[in] uiViChannelID		Video input channel ID
	@param[in] psDeFogSetting		Pointer to the data structure of defog setting

	@return NvtErrorCode (Refer to nvt_type.h)
*/
extern NVTRET NvtISP_SetDeFogSetting(UINT8 uiViChannelID, ISPDeFogSetting *psDeFogSetting);

/* Gamma */
/**    
        Not support Now!!!!!!!!!!!!!!!!!!!!!!!!!
	Get current gamma setting from ISP.

	@param[in] uiViChannelID		Video input channel ID
	@param[out] psGammaSetting		Pointer to the data structure of gamma setting

	@return NvtErrorCode (Refer to nvt_type.h)
*/
//extern NVTRET NvtISP_GetGammaSetting(UINT8 uiViChannelID, ISPGammaSetting *psGammaSetting);

/**
        Not support Now!!!!!!!!!!!!!!!!!!!!!!!
	Set new gamma setting to ISP.

	@param[in] uiViChannelID		Video input channel ID
	@param[in] psGammaSetting		Pointer to the data structure of gamma setting

	@return NvtErrorCode (Refer to nvt_type.h)
*/
//extern NVTRET NvtISP_SetGammaSetting(UINT8 uiViChannelID, ISPGammaSetting *psGammaSetting);

/* Color Correction Matrix */
/**
        Not support Now!!!!!!!!!!!!!!!!!!!!!!!
	Get current CCM setting from ISP.

	@param[in] uiViChannelID		Video input channel ID
	@param[out] psCCMSetting		Pointer to the data structure of CCM setting

	@return NvtErrorCode (Refer to nvt_type.h)
*/
//extern NVTRET NvtISP_GetCCMSetting(UINT8 uiViChannelID, ISPCCMSetting *psCCMSetting);

/**
        Not support Now!!!!!!!!!!!!!!!!!!!!!!!
	Set new CCM setting to ISP.

	@param[in] uiViChannelID		Video input channel ID
	@param[in] psCCMSetting			Pointer to the data structure of CCM setting

	@return NvtErrorCode (Refer to nvt_type.h)
*/
//extern NVTRET NvtISP_SetCCMSetting(UINT8 uiViChannelID, ISPCCMSetting *psCCMSetting);

/*	get RAW data */
extern NVTRET NvtISP_GetRawInfo(UINT8 uiViChannelID, ISPTuningRawInfo *psRwaInfo);

/* Crop Setting*/
extern NVTRET NvtISP_SetCropSetting(UINT8 uiViChannelID, ISPImageCropSetting *psCropSetting);
extern NVTRET NvtISP_GetCropSetting(UINT8 uiViChannelID, UINT32 uiIMEIndex, ISPImageCropSetting *psCropSetting);


/**
        Set ImeScale Type setting to ISP.

        @param[in] uiViChannelID                Video input channel ID
        @param[in] psIMEScaleSetting            Pointer to the data structure of IMEScaleConfig setting

        @return NvtErrorCode (Refer to nvt_type.h)
*/
extern NVTRET NvtISP_SetIMEScaleSetting(UINT8 uiViChannelID, ISPIMEScaleSetting *psIMEScaleSetting);

/**
        Get ImeScale Type setting from ISP.

        @param[in] uiViChannelID                Video input channel ID
        @param[out] psIMEScaleSetting           Pointer to the data structure of IMEScaleConfig setting

        @return NvtErrorCode (Refer to nvt_type.h)
*/
extern NVTRET NvtISP_GetIMEScaleSetting(UINT8 uiViChannelID, ISPIMEScaleSetting *psIMEScaleSetting);


#ifdef __cplusplus
}
#endif
/* ----------------------------------------------------------------- */
#endif /* _NVTISP_H	 */
