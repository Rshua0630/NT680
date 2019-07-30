#ifndef _UIINFO_H_
#define _UIINFO_H_

#include "GxLib.h"
#include "PrjCfg.h"

#include "WiFiIpcAPI.h"

/**
    @brief UISysInfo is the data structure
    for part 1 qulickly load from Nand

    Store in NAND Reserved Area [MAX: 512 bytes]
*/
typedef struct _UISysInfo {
	UINT32 uhInfoSize;
	UINT32 ubWallpaperStartupIndex;
	UINT32 ubBeepVolumnIndex;
	UINT32 ubBeepPowerIndex;
	UINT32 ulmodeIndex ; //Record Mode Index in SysInfo
}
UISysInfo;

// Maximum system flag number
#define MAX_SYS_FLAG_NUM            256

//PStore
#define VER_STR_MAX     32

/**
    @brief UIMenuStoreInfo is the data structure
    for keep data in pstore;permanent storage
*/
typedef struct _UIMenuUIMenuStoreInfo {
	//Common
	CHAR        strMakerString[VER_STR_MAX];
	CHAR        strModelString[VER_STR_MAX];
	CHAR        strSoftwareVer[VER_STR_MAX];
	CHAR        strMakeNote[VER_STR_MAX];
	CHAR        strImageDescription[VER_STR_MAX];
	UINT32      uhInfoSize;
	UINT32      UIParameter[MAX_SYS_FLAG_NUM];
	UINT32 ulmodeIndex ; //Record Last Mode Index
	UINT32 cbYear;
	UINT32 cbMonth;
	UINT32 cbDay;
	UINT32 cbFileID;
	UINT32 ubLanguageIndex;
	//photo menu
	UINT32 ubAFWindowIndex;
	UINT32 ubAFBeamIndex;
	UINT32 ubContAF;
	UINT32 ubAEmode;
	UINT32 ubQviewIndex;
	UINT32 ubDzoom;
	UINT32 ubBlinkDet;
	UINT32 ubSlowShutter;
	UINT32 ubContshotIndex;
	UINT32 ubTimelapseIndex;
	UINT32 ubDatePrint;
	UINT32 ubPanModeIndex;
	UINT32 ubFlashIndex;
	UINT32 ubEVIndex;
	UINT32 ubSizeIndex;
	UINT32 ubQualityIndex;
	UINT32 ubWBIndex;
	UINT32 ubColorIndex;
	UINT32 ubISOIndex;
	UINT32 ubFDIndex ;
	UINT32 ubSharpnessIndex;
	UINT32 ubPhotoDispMode;
	//wifi
	CHAR   strSSID[NVT_WSC_MAX_SSID_LEN];
	CHAR   strPASSPHRASE[NVT_MAX_WEP_KEY_LEN];

	CHAR   strSSID_hotspot_1[NVT_WSC_MAX_SSID_LEN];
	CHAR   strPASSPHRASE_hotspot_1[NVT_MAX_WEP_KEY_LEN];
} UIMenuStoreInfo;


#if 1
typedef enum {
	// << system setting flags region (should be stored in PStore and read back while system initial) >>
	FL_NULL,

	// Photo
	//FL_PHOTO_CAPTURE,           // Photo capture
	FL_PHOTO_SIZE,              // Photo size
	FL_QUALITY,                 // Photo quality
	FL_COLOR_EFFECT,            // Photo color
	FL_SCENE,                   // Scene mode
	FL_EV,                      // Exposure value
	FL_ISO,                     // ISO value
	FL_WB,                      // White balance mode
	FL_METERING,                // Exposure metering
	FL_SHARPNESS,               // Image sharpness
	FL_SATURATION,              // Image saturation
	//FL_FACE,                    // Face detection
	//FL_SMILE,                   // Smile detection
	FL_FD,                      // FD/SD detection
	FL_ANTISHAKE,               // Anti-shaking on/off
	FL_FLASH,                   // no use
	FL_RSC,
	FL_HDR,                     // no use
	FL_WDR,
	//FL_AE,
	FL_DUAL_CAM,                //real dual cam status
	FL_DUAL_CAM_MENU,           //dual cam display menu index
	FL_QUICK_REVIEW,            // Quick review time
	FL_DATE_STAMP,              // Date-time stamp OFF/DATE/DATE TIME
	FL_SELFTIMER,               // Self-timer time
	FL_FLASH_MODE,              // Flash light mode
	FL_CONTINUE_SHOT,           // Continue shot on/off
	//FL_QUICKVIEW,
	FL_SHDR,
	// Movie
	//#NT#2016/07/11#Charlie Chang -begin
	//#NT# movie rec status
	FL_MOVIE_REC,
	//#NT#2016/07/11#Charlie Chang -end
	FL_MOVIE_SIZE,              // Movie size flag in exe
	FL_MOVIE_SIZE_MENU,          // Movie menu size index
	//FL_MOVIE_EV,                // Movie EV
	FL_MOVIE_HDR,               // Movie HDR
	FL_MOVIE_HDR_MENU,
	FL_MOVIE_WDR,               // Movie WDR
	FL_MOVIE_WDR_MENU,
	//FL_MOVIE_FRAMERATE,         // Movie frame rate  15fps, 30fps
	FL_MOVIE_QUALITY,           // Movie quality
	FL_MOVIE_COLOR,             // Movie color
	//FL_MOVIE_SELFTIMER,         // Movie mode self-timer time
	FL_MOVIE_CYCLIC_REC,        // Movie cyclic recording
	FL_MOVIE_MOTION_DET,        // Movie motion detect
	FL_MOVIE_AUDIO,             // Movie audio enable
	FL_MOVIE_DATEIMPRINT,       // Movie dateimprint
	FL_MOVIE_TIMELAPSE_REC,     // Movie Timelapse record
	FL_MOVIE_IR_CUT,            // Movie IR Cut
	FL_MOVIE_SENSOR_ROTATE,     // Movie Sensor Roate on/off
	//FL_MOVIE_PREV_SENSOR_ROTATE,     // Previous Movie Sensor Roate status
	FL_MOVIE_PTZ,               // Movie PTZ
	FL_MOVIE_URGENT_PROTECT_AUTO,     // Movie auto urgent protect
	FL_MOVIE_URGENT_PROTECT_MANUAL,   // Movie manual urgent protect
	FL_MOVIE_PIM,               // Movie PIM
	FL_MOVIE_FCW,               // Movie FCW
	FL_MOVIE_LDWS,              // Movie LDWS
	FL_MOVIE_DDD,               // Movie DDD
	FL_MOVIE_ADAS_CAL,          // Movie ADAS CAL
	//#NT#2016/06/14#Charlie Chang -begin
	//#NT# support contrast, audio in,audio in sample rate , flip
	FL_MOVIE_CONTRAST,
	FL_MOVIE_AUDIOIN,
	FL_MOVIE_AUDIOIN_SR,
	FL_MOVIE_FLIP_MIRROR,
	FL_MOVIE_QUALITY_SET,
	//#NT#2016/06/14#Charlie Chang -end
	// Playback
	//FL_DELETE_PHOTO,            // Delete Still
	//FL_DELETE_MOVIE,            // Delete Video
	FL_PROTECT,                 // File protect operation (pseudo flag)

	// Menu Common page
	FL_COMMON_MODE,
	FL_COMMON_MENU,
	FL_COMMON_LOCAL,
	FL_COMMON_CLOUD,
	FL_COMMON_SETUP,
	FL_COMMON_EXT_SETUP,

	// Mode page
	//FL_MODE_MOVIE,
	//FL_MODE_PHOTO,
	//FL_MODE_PLAYBACK,

	// System
	FL_AUTO_POWER_OFF,          // Auto power off time
	FL_BEEP,                    // Beep sound on/off
	FL_LANGUAGE,                // Language
	FL_FREQUENCY,               // Frequency (50Hz/60Hz)
	FL_TV_MODE,                 // TV mode (NTSC/PAL)
	FL_HDMI_MODE,               // HDMI mode
	FL_FORMAT,                  // Format storage
	FL_DEFAULT,                 // Reset Default
	FL_DATE_FORMAT,             // Date format (DD/MM/YY, MM/DD/YY, YY/MM/DD)
	FL_TIME_FORMAT,             // Time format (AM/PM)
	FL_HOUR_FORMAT,             // Hour format (12 Hours/24 Hours)
	FL_DATE_TIME_SETUP,
	FL_OPENING_LOGO,            // Opening logo display (on/off)
	FL_SETUP,                   // Setup menu
	FL_GSENSOR,                 // G-sensor
	FL_EDGE,                    // edge
	FL_NR,                      // NR
	FL_IPP_ISO,                 // IPP ISO
	FL_WARP,
	FL_CPU2_DEFAULT,            // Reset cup2 Default

	FL_DCF_DIR_ID,              // DCF directory ID
	FL_DCF_FILE_ID,             // DCF file ID
	FL_LCD_DISPLAY,             // LCD display status (OSD normal, full, off, LCD power down)
	FL_MACRO,                   // Macro mode on/off
	FL_LCD_BRIGHTNESS,          // LCD brightness level
	FL_USB_MODE,                // USB mode (MSDC, PCC, PictBridge, ...)

	//Common
	FL_ModeIndex,               // mode index
	FL_PreMode,                 // pre mode index
	FL_NextMode,                // next mode index
	FL_ModeTransParam,
	FL_ModeTransDir,

	// Photo
	FL_SceneModeIndex,          //scene mode index
	FL_IsInitSettingFlag,
	FL_AFWindowIndex,
	FL_ContAFIndex,
	FL_AFBeamIndex,
	FL_CapModeIndex,
	FL_Dzoom,
	FL_DzoomReset,             //for PhotoExe_DZoomReset flag
	FL_ZoomIFIndex,
	FL_ZoomIsDigital,
	FL_ZoomOzmDzmChange,
	FL_DzoomIndex,             //digital zoom flag
	FL_ContShotIndex,
	FL_IsSingleCapture,

	//Movie
	FL_MovieDisIndex,
	FL_MovieMCTFIndex,
	FL_MovieRSCIndex,
	FL_MovieRSCIndex_MENU,
	FL_MovieGdcIndex,
	FL_MovieSmearIndex,
	FL_MovieAudioPlayIndex,
	FL_MovieAudioRecIndex,
	FL_MovieAudioRec,

	//Setup
	FL_LCD_OFF,
	FL_BrightAdjIndex,
	FL_DateFormatIndex,
	FL_TimeFormatIndex,
	FL_DualDisp,
	FL_BeepVolumnIndex,             // beep volumn adjust
	FL_AudioPlayIndex,            // audio play volume

	//Info Status
	FL_LensOpen,
	FL_TakePictCnt,
	FL_IsDCIMReadOnly,
	FL_ResetFlag,
	FL_DeleteAllFlag,
	FL_PhotoFrameWriteSts,
	FL_SSWAVWriteSts,
	FL_IsCIPARunning,
	FL_IsEmptyBatt,
	FL_IslensStopping,
	FL_IsStopCharge,
	FL_IsCopyToCarding,
	FL_IsUseFileDB,
	// Wifi
	FL_NetWorkMode,
	FL_WIFI,                    // Wi-Fi on/off  for menu flag
	FL_WIFI_MOVIE_MAXRECTIME,   // Maximum record time of Wi-Fi App setting
	FL_WIFI_PHOTO_FREEPICNUM,   // Free picture number
	FL_WIFI_LINK,
	FL_WIFI_MOVIE_FMT,
	FL_WIFI_MOVIE_APP_PREVIEW_SIZE,  // Preview video resolution for Wi-Fi APP.
	FL_WIFI_MOVIE_APP_PREVIEW_SIZE2,  // Preview video resolution for Wi-Fi APP.
	FL_WIFI_MOVIE_APP_PREVIEW_SIZE3,  // Preview video resolution for Wi-Fi APP.
	FL_WIFI_AUTO_RECORDING,          // CarDV mode or Sport DV mode.
	//#NT#2016/07/12#KCHong#[0104994] -begin
	//#NT#Add FL_MOVIE_LDWS_MENU & FL_MOVIE_FCW_MENU. Due to compatible issue, the two items are at the tail of the structure.
	FL_MOVIE_FCW_MENU,          // Movie FCW menu
	FL_MOVIE_LDWS_MENU,         // Movie LDWS menu
	//#NT#2016/07/12#KCHong#[0104994] -end
	//#NT#2016/07/26#KCHong#[0105955] -begin
	//#NT#If ADAS is ON and timelapse changes to OFF, reopen movie mode
	FL_MOVIE_TIMELAPSE_REC_MENU,
	//#NT#2016/07/26#KCHong#[0105955] -end
	//#NT#2016/08/19#Lincy Lin#[0106935] -begin
	//#NT# Support change WDR, SHDR, RSC setting will change mode after exit menu
	FL_WDR_MENU,
	FL_SHDR_MENU,
	FL_RSC_MENU,
	//#NT#2016/08/19#Lincy Lin -end

    	FL_DEFOG,
    	FL_DEFOG_MENU,
	FL_IndexInfoMAX
} UI_IndexInfo;

#endif
typedef enum {
	CLEAR_RESULT,
	EXE_OK,
	COPY_NUM_FULL
} EXE_RESULT;


//UI_ResetAllStatus,
typedef enum {
	RESET_ALL_OFF = 0,
	RESET_ALL_ON,
	RESET_ALL_SETTING_MAX
} RESET_ALL_SETTING;

//-----------------------------------------------------------------------------
// Battery Status
//-----------------------------------------------------------------------------
typedef enum {
	BATTERY_FULL = 0,
	BATTERY_MED,
	BATTERY_LOW,
	BATTERY_EMPTY,
	BATTERY_EXHAUSTED,
	BATTERY_CHARGE,
	BATTERY_STATUS_TOTAL_NUM
} BATTERY_STATUS;


typedef enum {
	BEEP_ATTR_VOLUME = 0,
	BEEP_ATTR_SHUTTER,
	BEEP_ATTR_KEY,
	BEEP_ATTR_SELFTIMER,
	BEEP_ATTR_POWER,
	BEEP_ATTR_SETTING_MAX
} BEEP_ATTR_SETTING;


//UI_BeepVolumnIndex,
typedef enum {
	BEEP_VOL_MUTE = 0,
	BEEP_VOL_1,
	BEEP_VOL_2,
	BEEP_VOL_3,
	BEEP_VOL_SETTING_MAX
} BEEP_VOL_SETTING;

//UI_BeepShutterIndex,
//UI_BeepKeyIndex,
//UI_BeepSelftimerIndex,
//UI_BeepPowerIndex,
typedef enum {
	BEEP_TONE_1 = 0,
	BEEP_TONE_2,
	BEEP_TONE_3,
	BEEP_TONE_4,
	BEEP_TONE_SETTING_MAX
} BEEP_TONE_SETTING;

typedef enum {
	SLIDESHOW_ATTR_EFFECT = 0,
	SLIDESHOW_ATTR_INTERVAL,
	SLIDESHOW_ATTR_REPEAT,
	SLIDESHOW_ATTR_SETTING_MAX
} SLIDESHOW_ATTR_SETTING;


//UI_LCDOffIndex
typedef enum {
	LCDOFF_ON = 0,
	LCDOFF_1MIN,
	LCDOFF_3MIN,
	LCDOFF_5MIN,
	LCDOFF_10MIN,
	LCDOFF_SETTING_MAX
} LCDOFF_SETTING;

//UI_WallpaperMenuIndex,
typedef enum {
	WALLPAPER_MENU_USER = 0,
	WALLPAPER_MENU_NATURE,
	WALLPAPER_MENU_SPARKLING,
	WALLPAPER_MENU_SETTING_MAX
} WALLPAPER_MENU_SETTING;

//#NT#2011/02/18#Steven feng -begin
//UI_SlideshowMusicIndex
typedef enum {
	SLIDESHOW_TYPE1 = 0,
	SLIDESHOW_TYPE2,
	SLIDESHOW_TYPE3,
	SLIDESHOW_TYPE4,
	SLIDESHOW_TYPE_SETTING_MAX
} SLIEDSHOW_TYPE_MENU_SETTING;
//#NT#2011/02/18#Steven feng -end

//UI_WallpaperStartupIndex,
typedef enum {
	WALLPAPER_STARTUP_USER = 0,
	WALLPAPER_STARTUP_GE,
	WALLPAPER_STARTUP_SETTING_MAX
} WALLPAPER_STARTUP_SETTING;

//UI_WallpaperBackIndex,
typedef enum {
	WALLPAPER_BACK_YES = 0,
	WALLPAPER_BACK_NO,
	WALLPAPER_BACK_SETTING_MAX
} WALLPAPER_BACK_SETTING;

//UI_PlayEffectIndex,
typedef enum {
	PLAYEFFECT_NORAML = 0,
	PLAYEFFECT_VIVID,
	PLAYEFFECT_SETTING_MAX
} PLAYEFFECT_SETTING;

//UI_BrightAdjIndex,
typedef enum {
	LCDBRT_ADJ_AUTO = 0,
	LCDBRT_ADJ_MANUAL,
	LCDBRT_ADJ_SETTING_MAX
} LCDBRT_ADJ_SETTING;

//UI_DateFormatIndex,
typedef enum {
	DATE_YMD = 0,
	DATE_DMY,
	DATE_MDY,
	DATE_SETTING_MAX
} DATE_SETTING;

//UI_TimeFormatIndex,
typedef enum {
	TIME_24HR = 0,
	TIME_12HR,
	TIME_SETTING_MAX
} TIME_SETTING;

//FL_TV_MODE,
typedef enum {
	VIDEOOUT_NTSC = 0,
	VIDEOOUT_PAL,
	VIDEOOUT_SETTING_MAX
} VIDEOOUT_SETTING;

//FL_HDMI_MODE,
typedef enum {
	VIDEOOUT2_4K2KP30,
	VIDEOOUT2_4K2KP25,
	VIDEOOUT2_4K2KP24,
	VIDEOOUT2_UHDP30,
	VIDEOOUT2_UHDP25,
	VIDEOOUT2_UHDP24,
	VIDEOOUT2_FHDI60,
	VIDEOOUT2_FHDP30,
	VIDEOOUT2_HD,
	VIDEOOUT2_AUTO,
	VIDEOOUT2_SETTING_MAX
} VIDEOOUT2_SETTING;

//#NT#2010/11/29#Ben Wang -begin
//#NT#add i key playback mode
//UI_PlayIKeyModeIndex,
typedef enum {
	PLAY_BY_NAME = 0,
	PLAY_BY_DATE,
	PLAY_BY_XX_MAX
} PLAY_BY_SETTING;
//#NT#2010/11/29#Ben Wang -end


//#NT#2012/8/10#Philex - begin
// add menu flag enumeration
//-----------------------------------------------------------------------------
// Photo Size
//-----------------------------------------------------------------------------
enum _PHOTO_SIZE {
	PHOTO_SIZE_12M,
	PHOTO_SIZE_10M,
	PHOTO_SIZE_8M,
	PHOTO_SIZE_5M,
	PHOTO_SIZE_3M,
	PHOTO_SIZE_VGA,
	PHOTO_SIZE_ID_MAX,
	PHOTO_SIZE_2MHD,
	PHOTO_SIZE_1M,
	PHOTO_SIZE_15M,
	PHOTO_SIZE_14M,
	PHOTO_SIZE_7M,
	PHOTO_SIZE_7MHD,
	PHOTO_SIZE_4M,
	PHOTO_SIZE_2M,
};

//-----------------------------------------------------------------------------
// Photo Quality
//-----------------------------------------------------------------------------
enum _QUALITY {
	QUALITY_SUPER,
	QUALITY_FINE,
	QUALITY_NORMAL,
	QUALITY_ID_MAX
};

//-----------------------------------------------------------------------------
// Photo Color
//-----------------------------------------------------------------------------
enum _PHOTO_COLOR {
	COLOR_EFFECT_STANDARD,
	COLOR_EFFECT_MONOCHROME,
	COLOR_EFFECT_SEPIA,
	COLOR_EFFECT_ID_MAX,
	COLOR_EFFECT_VIVID
};

//-----------------------------------------------------------------------------
// Scene Mode
//-----------------------------------------------------------------------------
enum _SCENE_MODE {
	SCENE_AUTO = 0,
	SCENE_PORTRAIT,
	SCENE_LANDSCAPE,
	SCENE_ID_MAX,
	SCENE_NIGHTLANDSCAPE,
};

//-----------------------------------------------------------------------------
// Exposure Value
//-----------------------------------------------------------------------------
enum _EXPOSURE {
	EV_P20,
	EV_P16,
	EV_P13,
	EV_P10,
	EV_P06,
	EV_P03,
	EV_00,
	EV_N03,
	EV_N06,
	EV_N10,
	EV_N13,
	EV_N16,
	EV_N20,
	EV_SETTING_MAX,

	EV_P30,
	EV_P15,
	EV_N15,
	EV_N30
};

//-----------------------------------------------------------------------------
// ISO Value
//-----------------------------------------------------------------------------
enum _ISO {
	ISO_AUTO,
	ISO_100,
	ISO_200,
	ISO_400,
	ISO_800,
	ISO_1600,
	ISO_ID_MAX,
	ISO_3200
} ;

//-----------------------------------------------------------------------------
// White Balance Mode
//-----------------------------------------------------------------------------
enum _WB_MODE {
	WB_AUTO,
	WB_DAYLIGHT,
	WB_CLOUDY,
	WB_TUNGSTEN,
	WB_FLUORESCENT,
	WB_MANUAL,
	WB_ID_MAX,
	// The following are for compiling issue
	WB_FLUORESCENT1,
	WB_FLUORESCENT2,
	WB_INCAND,
};

//-----------------------------------------------------------------------------
// Exposure Metering Method
//-----------------------------------------------------------------------------
enum _METERING {
	METERING_AIAE,
	METERING_CENTER,
	METERING_SPOT,
	METERING_ID_MAX,
};

//-----------------------------------------------------------------------------
// Sharpness Level
//-----------------------------------------------------------------------------
enum _SHARPNESS {
	SHARPNESS_SHARP,
	SHARPNESS_NORMAL,
	SHARPNESS_SOFT,
	SHARPNESS_ID_MAX
};

//-----------------------------------------------------------------------------
// Saturation Level
//-----------------------------------------------------------------------------
enum _SATURATION {
	SATURATION_HIGH,
	SATURATION_NORMAL,
	SATURATION_LOW,
	SATURATION_ID_MAX
};

//-----------------------------------------------------------------------------
// Face Detection State
//-----------------------------------------------------------------------------
enum _FACE_DETECTION {
	FD_OFF,
	FD_ON,
	FD_ID_MAX,
	// The following are for compiling issue
	FD_SMILE,
};

//-----------------------------------------------------------------------------
// AntiShaking State
//-----------------------------------------------------------------------------
enum _ANTISHAKING {
	ANTISHAKE_OFF,
	ANTISHAKE_ON,
	ANTISHAKE_ID_MAX
};

//-----------------------------------------------------------------------------
// Quick Review Time
//-----------------------------------------------------------------------------
enum _QUICK_REVIEW {
	QUICK_REVIEW_0SEC,
	QUICK_REVIEW_2SEC,
	QUICK_REVIEW_5SEC,
	QUICK_REVIEW_ID_MAX
};

//-----------------------------------------------------------------------------
// Date Imprint Mode
//-----------------------------------------------------------------------------
enum _DATE_STAMP {
	DATE_STAMP_OFF,
	DATE_STAMP_DATE,
	DATE_STAMP_DATE_TIME,
	DATE_STAMP_ID_MAX
};

//-----------------------------------------------------------------------------
// Photo Selftimer Time
//-----------------------------------------------------------------------------
enum _SELFTIMER {
	SELFTIMER_OFF,
	SELFTIMER_2SEC,
	SELFTIMER_5SEC,
	SELFTIMER_10SEC,
	SELFTIMER_ID_MAX
};

//-----------------------------------------------------------------------------
// Flash Light Mode
//-----------------------------------------------------------------------------
enum _FLASH_MODE {
	FLASH_AUTO,
	FLASH_ON,
	FLASH_RED_EYE,
	FLASH_OFF,
	FLASH_ID_MAX,
	// The following are for compiling issue
	FLASH_SLOW_FLASH_EYE,
	FLASH_SLOW_FLASH
};

//-----------------------------------------------------------------------------
// Vivilink
//-----------------------------------------------------------------------------
// Photo mode Vivilink items
enum _PHOTO_VIVILINK {
	PHOTO_VIVILINK_FACEBOOK,
	PHOTO_VIVILINK_TWITGOO,
	PHOTO_VIVILINK_TWITPIC,
	PHOTO_VIVILINK_PHOTOBUCKET,
	PHOTO_VIVILINK_FLICKR,
	PHOTO_VIVILINK_MYSPACE,
	PHOTO_VIVILINK_PICASA_ALBUMS,
	PHOTO_VIVILINK_NONE,
	PHOTO_VIVILINK_ID_MAX
};

// Movie mode Vivilink items
enum _MOVIE_VIVILINK {
	MOVIE_VIVILINK_YOUTUBE,
	MOVIE_VIVILINK_PHOTOBUCKET,
	MOVIE_VIVILINK_FLICKR,
	MOVIE_VIVILINK_NONE,
	MOVIE_VIVILINK_ID_MAX
};

// Playback mode Vivilink items for still
enum _PLAY_STILL_VIVILINK {
	PLAY_STILL_VIVILINK_FACEBOOK,
	PLAY_STILL_VIVILINK_TWITGOO,
	PLAY_STILL_VIVILINK_TWITPIC,
	PLAY_STILL_VIVILINK_PHOTOBUCKET,
	PLAY_STILL_VIVILINK_FLICKR,
	PLAY_STILL_VIVILINK_MYSPACE,
	PLAY_STILL_VIVILINK_PICASA_ALBUMS,
	PLAY_STILL_VIVILINK_NONE,
	PLAY_STILL_VIVILINK_ID_MAX
};

// Playback mode Vivilink items for video
enum _PLAY_VIDEO_VIVILINK {
	PLAY_VIDEO_VIVILINK_YOUTUBE,
	PLAY_VIDEO_VIVILINK_PHOTOBUCKET,
	PLAY_VIDEO_VIVILINK_FLICKR,
	PLAY_VIDEO_VIVILINK_NONE,
	PLAY_VIDEO_VIVILINK_ID_MAX
};

// real Vivilink value
enum _VIVILINK {
	VIVILINK_NONE,
	VIVILINK_FACEBOOK,
	VIVILINK_TWITGOO,
	VIVILINK_TWITPIC,
	VIVILINK_PHOTOBUCKET,
	VIVILINK_FLICKR,
	VIVILINK_MYSPACE,
	VIVILINK_PICASA_ALBUMS,
	VIVILINK_YOUTUBE,
	VIVILINK_ID_MAX
};

// Vivilink setting operation
enum _VIVILINK_SETTING {
	VIVILINK_SET_ONE,
	VIVILINK_SET_ALL,
	VIVILINK_SET_ID_MAX
};

//-----------------------------------------------------------------------------
// Continue shot State
//-----------------------------------------------------------------------------
enum _CONTINUE_SHOT_SETTING {
	CONTINUE_SHOT_OFF,
	CONTINUE_SHOT_BURST_3,
	CONTINUE_SHOT_BURST_30,
	CONTINUE_SHOT_BURST,
	CONTINUE_SHOT_SETTING_MAX,
	CONTINUE_SHOT_SIDE,  //side by side
};

//-----------------------------------------------------------------------------
// RSC
//-----------------------------------------------------------------------------
enum _RSC_SETTING {
	RSC_OFF,
	RSC_ON,
	RSC_SETTING_MAX
};

//-----------------------------------------------------------------------------
// WDR
//-----------------------------------------------------------------------------
enum _WDR_SETTING {
	WDR_OFF,
	WDR_ON,
	WDR_SETTING_MAX
};

//-----------------------------------------------------------------------------
// Sensor HDR
//-----------------------------------------------------------------------------
enum _SHDR_SETTING {
	SHDR_OFF,
	SHDR_ON,
	SHDR_SETTING_MAX
};

//-----------------------------------------------------------------------------
// DEFOG
//-----------------------------------------------------------------------------
enum _DEFOG_SETTING {
	DEFOG_OFF,
	DEFOG_ON,
	DEFOG_SETTING_MAX
};

//-----------------------------------------------------------------------------
// Dual Cam
//-----------------------------------------------------------------------------
enum _DUALCAM_SETTING {
	DUALCAM_FRONT,
	DUALCAM_BEHIND,
	DUALCAM_BOTH,    //FRONT is major
	DUALCAM_BOTH2,   //BEHIND is major
//#NT#2016/06/17#Brain Yen -begin
//#NT#add for Pipview LR
	DUALCAM_LR_16_9,   //left and right is 16:9 size, half screen respectively
	DUALCAM_LR_FULL,   //left and right is full half screen and crop respectively
	DUALCAM_LR_FRONT_FULL,   //front is full screen and crop
	DUALCAM_LR_BEHIND_FULL, //behind is full screen and crop
//#NT#2016/06/17#Brain Yen -end
	DUALCAM_SETTING_MAX
};

//-----------------------------------------------------------------------------
// Movie Size
//-----------------------------------------------------------------------------
//MOVIE_SIZE_TAG
enum _MOVIE_SIZE {
	MOVIE_SIZE_FRONT_2880x2160P24, //UHD P24
	MOVIE_SIZE_FRONT_2560x1440P30, //QHD P30
	MOVIE_SIZE_FRONT_2304x1296P30, //3MHD P30
	MOVIE_SIZE_FRONT_1920x1080P96, //FHD
	MOVIE_SIZE_FRONT_1920x1080P60, //FHD
	MOVIE_SIZE_FRONT_1920x1080P30, //FHD
	MOVIE_SIZE_FRONT_1280x720P120, //HD
	MOVIE_SIZE_FRONT_1280x720P60, //HD
	MOVIE_SIZE_FRONT_1280x720P30,  //HD
	MOVIE_SIZE_FRONT_848x480P30,   //WVGA
	MOVIE_SIZE_FRONT_640x480P240,   //VGA
	MOVIE_SIZE_FRONT_640x480P30,   //VGA
	MOVIE_SIZE_FRONT_320x240P30,   //QVGA
	MOVIE_SIZE_DUAL_1920x1080P30_1920x1080P30, //FHD FHD
	MOVIE_SIZE_CLONE_1920x1080P30_1920x1080P30, //FHD FHD
	MOVIE_SIZE_CLONE_1920x1080P30_1280x720P30,  //FHD HD
	MOVIE_SIZE_CLONE_2560x1440P30_848x480P30, //QHD WVGA
	MOVIE_SIZE_CLONE_2304x1296P30_848x480P30, //3MHD WVGA
	MOVIE_SIZE_CLONE_1920x1080P60_848x480P30, //FHD WVGA
	MOVIE_SIZE_CLONE_1920x1080P60_640x360P30, //FHD VGA
	MOVIE_SIZE_CLONE_1920x1080P30_848x480P30, //FHD WVGA
	MOVIE_SIZE_CLONE_2048x2048P30_480x480P30,
	MOVIE_SIZE_CLONE_1280x720P30_1280x720P30, //HD HD
	MOVIE_SIZE_CLONE_848x480P30_848x480P30,   //WVGA WVGA
	MOVIE_SIZE_CLONE_640x480P30_640x480P30,   //VGA VGA
	MOVIE_SIZE_CLONE_320x240P30_320x240P30,   //QVGA QVGA
	MOVIE_SIZE_ID_MAX,
};

//-----------------------------------------------------------------------------
// Movie Quality
//-----------------------------------------------------------------------------
enum _MOVIE_QUALITY {
	MOVIE_QUALITY_FINE,
	MOVIE_QUALITY_STANDARD,
	MOVIE_QUALITY_ECONOMY,
	MOVIE_QUALITY_ID_MAX
};

//-----------------------------------------------------------------------------
// Movie Color
//-----------------------------------------------------------------------------
enum _MOVIE_COLOR {
	MOVIE_COLOR_NORMAL,
	MOVIE_COLOR_MONO,
	MOVIE_COLOR_SEPIA,
	MOVIE_COLOR_ID_MAX
};

//-----------------------------------------------------------------------------
// Movie Selftimer
//-----------------------------------------------------------------------------
enum _MOVIE_SELFTIMER {
	MOVIE_SELFTIMER_OFF,
	MOVIE_SELFTIMER_ON_2S,
	MOVIE_SELFTIMER_ON_10S,
	MOVIE_SELFTIMER_ON_10_2S,
	MOVIE_SELFTIMER_ID_MAX
};

//-----------------------------------------------------------------------------
// Movie cyclic record
//-----------------------------------------------------------------------------
enum _MOVIE_CYCLICREC {
	MOVIE_CYCLICREC_OFF,
	MOVIE_CYCLICREC_1MIN,
	MOVIE_CYCLICREC_3MIN,
	MOVIE_CYCLICREC_5MIN,
	MOVIE_CYCLICREC_10MIN,
	MOVIE_CYCLICREC_ID_MAX
};


//-----------------------------------------------------------------------------
// Movie HDR State
//-----------------------------------------------------------------------------
enum {
	MOVIE_HDR_OFF = 0,
	MOVIE_HDR_ON,
	MOVIE_HDR_ID_MAX
};

//-----------------------------------------------------------------------------
// Movie WDR State
//-----------------------------------------------------------------------------
enum {
	MOVIE_WDR_OFF = 0,
	MOVIE_WDR_ON,
	MOVIE_WDR_ID_MAX
};

//-----------------------------------------------------------------------------
// Movie Motion Detect State
//-----------------------------------------------------------------------------
enum {
	MOVIE_MOTIONDET_OFF = 0,
	MOVIE_MOTIONDET_ON,
	MOVIE_MOTIONDET_ID_MAX
};

//-----------------------------------------------------------------------------
// Movie Audio enable State
//-----------------------------------------------------------------------------
enum {
	MOVIE_AUDIO_OFF = 0,
	MOVIE_AUDIO_ON,
	MOVIE_AUDIO_ID_MAX
};

//-----------------------------------------------------------------------------
// Movie Audio volume State
//-----------------------------------------------------------------------------
enum {
	MOVIE_VOL_MIN = 0,
	MOVIE_VOL_MAX = 100
};

//-----------------------------------------------------------------------------
// Movie Audio record State
//-----------------------------------------------------------------------------
enum {
	MOVIE_AUD_REC_OFF = 0,
	MOVIE_AUD_REC_ON,
	MOVIE_AUD_REC_ID_MAX
};

//-----------------------------------------------------------------------------
// Movie DateImprint State
//-----------------------------------------------------------------------------
enum {
	MOVIE_DATEIMPRINT_OFF = 0,
	MOVIE_DATEIMPRINT_ON,
	MOVIE_DATEIMPRINT_ID_MAX
};

//-----------------------------------------------------------------------------
// GSENSOR
//-----------------------------------------------------------------------------
enum _GSENSOR {
	GSENSOR_OFF = 0,
	GSENSOR_LOW,
	GSENSOR_MED,
	GSENSOR_HIGH,
	GSENSOR_ID_MAX,
	GSENSOR_ON
};

//-----------------------------------------------------------------------------
// Movie time lapse record
//-----------------------------------------------------------------------------
enum _MOVIE_TIMELAPSEREC {
	MOVIE_TIMELAPSEREC_OFF,
	MOVIE_TIMELAPSEREC_1SEC,
	MOVIE_TIMELAPSEREC_5SEC,
	MOVIE_TIMELAPSEREC_10SEC,
	MOVIE_TIMELAPSEREC_30SEC,
	MOVIE_TIMELAPSEREC_1MIN,
	MOVIE_TIMELAPSEREC_5MIN,
	MOVIE_TIMELAPSEREC_10MIN,
	MOVIE_TIMELAPSEREC_30MIN,
	MOVIE_TIMELAPSEREC_1HOUR,
	MOVIE_TIMELAPSEREC_2HOUR,
	MOVIE_TIMELAPSEREC_3HOUR,
	MOVIE_TIMELAPSEREC_1DAY,
	MOVIE_TIMELAPSEREC_ID_MAX
};

//-----------------------------------------------------------------------------
// Movie IR Cut option
//-----------------------------------------------------------------------------
enum {
	MOVIE_IR_CUT_OFF = 0,// night mode
	MOVIE_IR_CUT_ON,     // day mode
	MOVIE_IR_CUT_AUTO,
	MOVIE_IR_CUT_ID_MAX,
};

//-----------------------------------------------------------------------------
// Movie PTZ option
//-----------------------------------------------------------------------------
enum {
	MOVIE_PTZ_OFF = 0,
	MOVIE_PTZ_ON,
	MOVIE_PTZ_ID_MAX,
};

//-----------------------------------------------------------------------------
// Movie auto urgent protect option
//-----------------------------------------------------------------------------
enum {
	MOVIE_URGENT_PROTECT_AUTO_OFF = 0,
	MOVIE_URGENT_PROTECT_AUTO_ON,
	MOVIE_URGENT_PROTECT_AUTO_ID_MAX,
};

//-----------------------------------------------------------------------------
// Movie manual urgent protect option
//-----------------------------------------------------------------------------
enum {
	MOVIE_URGENT_PROTECT_MANUAL_OFF = 0,
	MOVIE_URGENT_PROTECT_MANUAL_ON,
	MOVIE_URGENT_PROTECT_MANUAL_ID_MAX,
};

//-----------------------------------------------------------------------------
// Movie PIM option
//-----------------------------------------------------------------------------
enum {
	MOVIE_PIM_OFF = 0,
	MOVIE_PIM_ON,
	MOVIE_PIM_ID_MAX,
};

//-----------------------------------------------------------------------------
// Movie FCW option
//-----------------------------------------------------------------------------
enum {
	MOVIE_FCW_OFF = 0,
	MOVIE_FCW_ON,
	MOVIE_FCW_ID_MAX,
};

//-----------------------------------------------------------------------------
// Movie LDWS option
//-----------------------------------------------------------------------------
enum {
	MOVIE_LDWS_OFF = 0,
	MOVIE_LDWS_ON,
	MOVIE_LDWS_ID_MAX,
};

//-----------------------------------------------------------------------------
// Movie DDD option
//-----------------------------------------------------------------------------
enum {
	MOVIE_DDD_OFF = 0,
	MOVIE_DDD_ON,
	MOVIE_DDD_ID_MAX,
};

//-----------------------------------------------------------------------------
// Movie ADAS CAL option
//-----------------------------------------------------------------------------
enum {
	MOVIE_ADAS_CAL_OFF = 0,
	MOVIE_ADAS_CAL_ON,
	MOVIE_ADAS_CAL_ID_MAX,
};

//-----------------------------------------------------------------------------
// Movie Dual Record option
//-----------------------------------------------------------------------------
enum {
	MOVIE_DUAL_RECORD_OFF = 0,
	MOVIE_DUAL_RECORD_ON,
	MOVIE_DUAL_RECORD_ID_MAX,
};

//-----------------------------------------------------------------------------
// Date Format
//-----------------------------------------------------------------------------
enum _DATE_FORMAT {
	DATE_FORMAT_YMD,
	DATE_FORMAT_MDY,
	DATE_FORMAT_DMY,
	DATE_FORMAT_ID_MAX
};

//-----------------------------------------------------------------------------
// File Protect
//-----------------------------------------------------------------------------
enum _FILE_PROTECT {
	PROTECT_ONE = 0,
	UNPROTECT_ONE,
	PROTECT_ALL,
	UNPROTECT_ALL,
	FILE_PROTECT_ID_MAX
};

//-----------------------------------------------------------------------------
// Slide Show Time
//-----------------------------------------------------------------------------
enum _SLIDE_SHOW {
	SLIDE_SHOW_2SEC,
	SLIDE_SHOW_5SEC,
	SLIDE_SHOW_8SEC,
	SLIDE_SHOW_ID_MAX
};

//-----------------------------------------------------------------------------
// Auto Power Off Time
//-----------------------------------------------------------------------------
//FL_AUTO_POWER_OFF
typedef enum {
	POWER_ON = 0,
	POWER_1MIN,
	POWER_2MIN,
	POWER_3MIN,
	POWER_5MIN,
	POWER_10MIN,
	POWEROFF_SETTING_MAX
} POWEROFF_SETTING;

//-----------------------------------------------------------------------------
// Beep Sound
//-----------------------------------------------------------------------------
enum _BEEP {
	BEEP_OFF,
	BEEP_ON,
	BEEP_ID_MAX
};

//-----------------------------------------------------------------------------
// Language Setting
//-----------------------------------------------------------------------------
enum _LANGUAGE {
	LANG_EN,
	LANG_FR,
	LANG_ES,
	LANG_PO,
	LANG_DE,
	LANG_IT,
	LANG_SC,
	LANG_TC,
	LANG_RU,
	LANG_JP,
	LANG_ID_MAX,

	LANG_DU,
	LANG_TU,
	LANG_PB,
	LANG_YI,
	LANG_PL,
	LANG_DE_JAY
};

//-----------------------------------------------------------------------------
// Power Frequency
//-----------------------------------------------------------------------------
enum _FREQUENCY {
	FREQUENCY_50HZ,
	FREQUENCY_60HZ,
	FREQUENCY_ID_MAX
};

//-----------------------------------------------------------------------------
// TV Mode
//-----------------------------------------------------------------------------
enum _TV_MODE {
	TV_MODE_NTSC,
	TV_MODE_PAL,
	TV_MODE_ID_MAX
};

//-----------------------------------------------------------------------------
// Sensor rotate Mode
//-----------------------------------------------------------------------------
enum _SENSOR_ROTATE_MODE {
	SEN_ROTATE_OFF,
	SEN_ROTATE_ON,
	SEN_ROTATE_MAX
};

//-----------------------------------------------------------------------------
// Opening Logo Display
//-----------------------------------------------------------------------------
enum _OPENING_LOGO {
	OPENING_LOGO_OFF,
	OPENING_LOGO_ON,
	OPENING_LOGO_ID_MAX
};

//-----------------------------------------------------------------------------
// LCD Display Status
//-----------------------------------------------------------------------------
enum _LCD_DISPLAY {
	DISPOUT_NORMAL,
	DISPOUT_ALL,
	DISPOUT_OFF,
	DISPOUT_ID_MAX,
};

//-----------------------------------------------------------------------------
// LCD Brightness Adjust Level
//-----------------------------------------------------------------------------
enum _LCD_BRIGHTNESS_LEVEL {
	LCDBRT_LVL_01 = 1,
	LCDBRT_LVL_02,
	LCDBRT_LVL_03,
	LCDBRT_LVL_04,
	LCDBRT_LVL_05,
	LCDBRT_LVL_06,
	LCDBRT_LVL_07,
	LCDBRT_LVL_08,
	LCDBRT_LVL_09,
	LCDBRT_LVL_ID_MAX
};

//-----------------------------------------------------------------------------
// Macro State
//-----------------------------------------------------------------------------
enum _MACRO {
	MACRO_OFF,
	MACRO_ON,
	MACRO_ID_MAX
};

//-----------------------------------------------------------------------------
// Audio volume State
//-----------------------------------------------------------------------------
enum {
	AUDIO_VOL_MIN = 0,
	AUDIO_VOL_MAX = 100
};

//-----------------------------------------------------------------------------
// USB Mode
//-----------------------------------------------------------------------------
enum _USB_MODE {
	USB_MODE_MSDC,
	USB_MODE_PCC,
	USB_MODE_PTP,
	USB_MODE_ID_MAX,
	USB_MODE_CHARGE // for future use
};

//-----------------------------------------------------------------------------
// Wi-Fi on/off
//-----------------------------------------------------------------------------
enum _WIFI {
	WIFI_OFF,
	WIFI_ON,
	WIFI_ID_MAX
};

//-----------------------------------------------------------------------------
// Cloud on/off
//-----------------------------------------------------------------------------
enum _CLOUD {
	CLOUD_OFF,
	CLOUD_ON,
	CLOUD_ID_MAX
};

//-----------------------------------------------------------------------------
// HDMI/TV plug
//-----------------------------------------------------------------------------
typedef enum {
	PLUG_OFF,
	PLUG_TV,
	PLUG_HDMI,
	PLUG_MAX,
	ENUM_DUMMY4WORD(PLUG_DEV)
} PLUG_DEV;

//FL_DualDisp,
typedef enum {
	DUALDISP_OFF = 0,
	DUALDISP_ON,
	DUALDISP_SETTING_MAX
} DUALDISP_SETTING;

//-----------------------------------------------------------------------------
// Wi-Fi link OK/NG
//-----------------------------------------------------------------------------
enum _WIFI_LINK {
	WIFI_LINK_NG = 0,
	WIFI_LINK_OK,
	WIFI_LINK_LINKING,
	WIFI_LINK_ID_MAX
};

enum _WIFI_RTSP {
	WIFI_RTSP_LIVEVIEW,
	WIFI_RTSP_REC,
	WIFI_RTSP_IDLE,
	WIFI_RTSP_MAX
};

extern CHAR previewEVStr[EV_SETTING_MAX][5];

enum _WIFI_MOVIE_APP_PREVIEW_SIZE {
	WIFI_MOVIE_APP_PREVIEW_SIZE_5M_4_3,    // 2560 x 1920
	WIFI_MOVIE_APP_PREVIEW_SIZE_4M_16_9,   // 2560 x 1440
	WIFI_MOVIE_APP_PREVIEW_SIZE_3M_4_3,    // 2048 x 1536
	WIFI_MOVIE_APP_PREVIEW_SIZE_1080P,     // 1920 x 1080
	WIFI_MOVIE_APP_PREVIEW_SIZE_960P,      // 1280 x 960
	WIFI_MOVIE_APP_PREVIEW_SIZE_720P,      // 1280 x 720
	WIFI_MOVIE_APP_PREVIEW_SIZE_WVGA,      // 854 x 480
	WIFI_MOVIE_APP_PREVIEW_SIZE_PAL,       // 720 x 576
	WIFI_MOVIE_APP_PREVIEW_SIZE_704x576,   // 704 x 576
	WIFI_MOVIE_APP_PREVIEW_SIZE_NTSC,      // 720 x 480
	WIFI_MOVIE_APP_PREVIEW_SIZE_704x480,   // 704 x 480
	WIFI_MOVIE_APP_PREVIEW_SIZE_VGA_4_3,   // 640 x 480
	WIFI_MOVIE_APP_PREVIEW_SIZE_VGA_16_9,  // 640 x 360
	WIFI_MOVIE_APP_PREVIEW_SIZE_352x288,   // 352 x 288
	WIFI_MOVIE_APP_PREVIEW_SIZE_QVGA,      // 320 x 240
	WIFI_MOVIE_APP_PREVIEW_SIZE_ID_MAX,
};

//-----------------------------------------------------------------------------
// Movie Rec Size for the front path
//-----------------------------------------------------------------------------
enum _FINALCAM_MOVIE_REC_SIZE {
	FINALCAM_MOVIE_REC_SIZE_1080P,   // 1920 x 1080
	FINALCAM_MOVIE_REC_SIZE_720P,    // 1280 x 720
	FINALCAM_MOVIE_REC_SIZE_WVGA,    // 854 x 480
	FINALCAM_MOVIE_REC_SIZE_VGA,     // 640 x 480
	FINALCAM_MOVIE_REC_SIZE_ID_MAX,
	FINALCAM_MOVIE_REC_SIZE_1440P,   // 1440 x 1080 with DAR.
	FINALCAM_MOVIE_REC_SIZE_QVGA,    // 320 x 240
	FINALCAM_MOVIE_REC_SIZE_1080P_FULLRES    // 1920 x 1080 with full resolution sensor input (for compiling issue)
};

enum _WIFI_AUTO_RECORDING {
	WIFI_AUTO_RECORDING_OFF,
	WIFI_AUTO_RECORDING_ON,
	WIFI_AUTO_RECORDING_ID_MAX
};

//#NT#2014/10/02#Calvin Chang#Support IP Camera Flow -begin
enum _STRM_TYPE_ {
	STRM_TYPE_STREAMING,
	STRM_TYPE_FULLTIME_RECORD,
	STRM_TYPE_FULLTIME_RECORD_VIDONLY,  // reserve audio resource for device speaker play
	STRM_TYPE_STREAMING_VIDONLY,
	STRM_TYPE_ID_MAX,
};
//#NT#2014/10/02#Calvin Chang -end
//#NT#2014/10/21#Philex Lin - begin
//-----------------------------------------------------------------------------
// Flag enumerations for TUTK IO Commands
//-----------------------------------------------------------------------------
enum _TUTK_RECORD_TYPE {
	TUTK_RECORDTYPE_OFF,
	TUTK_RECORDTYPE_FULLTIME,
	TUTK_RECORDTYPE_ALARM,
//    TUTK_RECORDTYPE_MANUAL,
	TUTK_RECORDTYPE_MAX,
};
enum _TUTK_MOTIONDETECT_SENSITIVITY {
	MOTIONDETECT_SENSITIVITY_OFF,
	MOTIONDETECT_SENSITIVITY_LOW,
	MOTIONDETECT_SENSITIVITY_MIDDLE,
	MOTIONDETECT_SENSITIVITY_HIGHER,
	MOTIONDETECT_SENSITIVITY_HIGHEST,
	MOTIONDETECT_SENSITIVITY_MAX,
};

enum _TUTK_ENVIRONMENT_MODE {
	ENVIRONMENT_MODE_INDOOR_50HZ,
	ENVIRONMENT_MODE_INDOOR_60HZ,
	ENVIRONMENT_MODE_OUTDOOR,
	ENVIRONMENT_MODE_NIGHT,
	ENVIRONMENT_MODE_MAX,
};

enum _TUTK_VIDEOMODE {
	VIDEOMODE_NORMAL,
	VIDEOMODE_FLIP,
	VIDEOMODE_MIRROR,
	VIDEOMODE_FLIP_MIRROR,
	VIDEOMODE_MAX,
};

enum _TUTK_AVSERVER {
	AVSERVER_OFF,
	AVSERVER_ON,
	AVSERVER_MAX,
};
//#NT#2014/10/21#Philex Lin - end


//-----------------------------------------------------------------------------
// Get/Set Photo Mode User Defined Items
//-----------------------------------------------------------------------------
typedef enum {
	PHOTO_USR_FLASH         = 0,
	PHOTO_USR_EV,
	PHOTO_USR_AFWINDOW,
	PHOTO_USR_AFBEAM,
	PHOTO_USR_METERING,
	PHOTO_USR_TIMELAPSE,
	PHOTO_USR_CONTSHOT,
	PHOTO_USR_CONTAF,
	PHOTO_USR_QREVIEW,
	PHOTO_USR_DZOOMSTATUS,
	PHOTO_USR_SLOWSHUTTER,
	PHOTO_USR_DATEIMPRINT,
	PHOTO_USR_BLINK,
	PHOTO_USR_PANDET,
	PHOTO_USR_COLOR,
	PHOTO_USR_SIZE,
	PHOTO_USR_QUALITY,
	PHOTO_USR_ISO,
	PHOTO_USR_WB,
	PHOTO_USR_SATURATION,
	PHOTO_USR_SHARPNESS,
	PHOTO_USR_CONTRAST,
	PHOTO_USR_FD,
	PHOTO_USR_DISPMODE
} PHOTO_USR_ITEM_T;

extern void     Load_SysInfo(void);
extern void     Save_SysInfo(void);
extern void     Init_SysInfo(void);

//UI Setting Info
extern void     Load_MenuInfo(void); //Load from PST
extern void     Save_MenuInfo(void); //Save to PST
extern void     Init_MenuInfo(void);
extern void     Reset_MenuInfo(void); //reset
extern UIMenuStoreInfo *UI_GetMenuInfo(void);

//#NT#2012/10/17#Philex Lin-begin
// add this API to check ACIn and naming is backward for NT96220
extern BOOL KeyScan_IsACIn(void);
// add Auto Poweroff/USB detect/Enable Key sound scan items
extern void KeyScan_EnableMisc(BOOL bEn);
// add get battery level function
extern UINT32   GetBatteryLevel(void);
//#NT#2012/10/17#Philex Lin-end

extern void     SysSetFlag(UINT32 uiFlag, UINT32 uiValue);
extern UINT32   SysGetFlag(UINT32 uiFlag);

extern void     SysCheckFlag(void);
extern void     SysSetFixedFlag(void);
extern void     SysResetFlag(void);
extern void     SysExeMenuSettingFuncs(void);

extern UINT32   UI_GetData(UINT32 attribute);
extern void     UI_SetData(UINT32 attribute, UINT32 value);
extern UINT32   Movie_GetDualRecord(UINT32 uiMovieSizeIndex);
extern void     Photo_SetUserIndex(PHOTO_USR_ITEM_T uiItem, UINT32 uhIndex);
extern UINT32   Photo_GetUserIndex(PHOTO_USR_ITEM_T uiItem);

extern PLUG_DEV KeyScan_GetPlugDev(void);


#define UI_SetData(a,b)            SysSetFlag(a,b)
#define UI_GetData(a)              SysGetFlag(a)

extern void FlowDCF_UpdateName(void);

extern void UI_SetInitVolume(UINT32 volumeIndex);
extern void UI_SaveRTCDate(void);
extern void UI_SetFileDBFileID(void);
#endif //_UIINFO_H_
