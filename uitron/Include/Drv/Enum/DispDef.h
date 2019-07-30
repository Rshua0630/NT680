
#ifndef _DISPDEF_H_
#define _DISPDEF_H_

#include "Type.h"

//Display device
#define PRIMARY_DISPTYPE_LCD    0
#define PRIMARY_DISPTYPE_TV     1   //Maybe NTSC, PAL or HDMI

#define DISP_LASTMODE    0xffffffff

//Display mode
typedef enum {
	DISP_LCDMODE_RGB_SERIAL           =    0,     ///< LCD MODE is RGB Serial or UPS051
	DISP_LCDMODE_RGB_PARALL           =    1,     ///< LCD MODE is RGB Parallel (888)
	DISP_LCDMODE_YUV640               =    2,     ///< LCD MODE is YUV640
	DISP_LCDMODE_YUV720               =    3,     ///< LCD MODE is YUV720
	DISP_LCDMODE_RGBD360              =    4,     ///< LCD MODE is RGB Dummy 360
	DISP_LCDMODE_RGBD320              =    5,     ///< LCD MODE is RGB Dummy 320
	DISP_LCDMODE_RGB_THROUGH          =    6,     ///< LCD MODE is RGB through mode
	DISP_LCDMODE_CCIR601              =    7,     ///< LCD MODE is CCIR601
	DISP_LCDMODE_CCIR656              =    8,     ///< LCD MODE is CCIR656
	DISP_LCDMODE_RGB_PARALL666        =    9,     ///< LCD MODE is RGB Parallel 666
	DISP_LCDMODE_RGB_PARALL565        =   10,     ///< LCD MODE is RGB Parallel 565
	DISP_LCDMODE_RGB_PARALL_DELTA     =   11,     ///< LCD MODE is RGB Parallel Delta
	//#NT#2016/05/27#Jeah Yen -begin
	//#NT#Support MIPI and MI mode
	DISP_LCDMODE_MIPI                 =   12,     ///< LCD MODE is MIPI

	DISP_LCDMODE_MI_OFFSET            = 32,       ///< Memory LCD MODE offset
	DISP_LCDMODE_MI_FMT0              = 32 + 0,   ///< LCD MODE is Memory(Parallel Interface) 8bits
	DISP_LCDMODE_MI_FMT1              = 32 + 1,   ///< LCD MODE is Memory(Parallel Interface) 8bits
	DISP_LCDMODE_MI_FMT2              = 32 + 2,   ///< LCD MODE is Memory(Parallel Interface) 8bits
	DISP_LCDMODE_MI_FMT3              = 32 + 3,   ///< LCD MODE is Memory(Parallel Interface) 8bits
	DISP_LCDMODE_MI_FMT4              = 32 + 4,   ///< LCD MODE is Memory(Parallel Interface) 8bits
	DISP_LCDMODE_MI_FMT5              = 32 + 5,   ///< LCD MODE is Memory(Parallel Interface) 8bits
	DISP_LCDMODE_MI_FMT6              = 32 + 6,   ///< LCD MODE is Memory(Parallel Interface) 8bits
	DISP_LCDMODE_MI_FMT7              = 32 + 7,   ///< LCD MODE is Memory(Parallel Interface) 9bits
	DISP_LCDMODE_MI_FMT8              = 32 + 8,   ///< LCD MODE is Memory(Parallel Interface) 16bits
	DISP_LCDMODE_MI_FMT9              = 32 + 9,   ///< LCD MODE is Memory(Parallel Interface) 16bits
	DISP_LCDMODE_MI_FMT10             = 32 + 10,  ///< LCD MODE is Memory(Parallel Interface) 18bits
	DISP_LCDMODE_MI_FMT11             = 32 + 11,  ///< LCD MODE is Memory(Parallel Interface)  8bits
	DISP_LCDMODE_MI_FMT12             = 32 + 12,  ///< LCD MODE is Memory(Parallel Interface) 16bits
	DISP_LCDMODE_MI_FMT13             = 32 + 13,  ///< LCD MODE is Memory(Parallel Interface) 16bits
	DISP_LCDMODE_MI_FMT14             = 32 + 14,  ///< LCD MODE is Memory(Parallel Interface) 16bits
	DISP_LCDMODE_MI_SERIAL_BI         = 32 + 20,  ///< LCD MODE is Serial Interface bi-direction
	DISP_LCDMODE_MI_SERIAL_SEP        = 32 + 21,  ///< LCD MODE is Serial Interface separation

	DISP_LCDMODE_NO_CHANGE            = 0xFF,     ///< LCD MODE use current setting
	//#NT#2016/05/27#Jeah Yen -end

	ENUM_DUMMY4WORD(DISP_LCDMODE)
} DISP_LCDMODE;

#define TVCLASS(c)          ((UINT32)(c)) & 0xff000000

typedef enum {
	DISP_TVMODE_NTSC                = 0,          ///< TV MODE is NTSC (Video format is 640x448 i60 or p30)
	DISP_TVMODE_PAL                 = 1,          ///< TV MODE is PAL (Video format is 640x544 i50 or p25)
	
	//#NT#2017/08/09#Jeah Yen -begin
	//#NT#Support new TV mode
	DISP_TVMODE_NTSC_SD             = 2,          ///< TV MODE is SD NTSC (Video format is 720x480 i60 or p30)
	DISP_TVMODE_PAL_SD              = 3,          ///< TV MODE is SD PAL (Video format is 720x526 i50 or p25)
	//#NT#2017/08/09#Jeah Yen -end

	ENUM_DUMMY4WORD(DISP_TVMODE)
} DISP_TVMODE;

typedef enum {
	DISP_HDMIMODE_640X480P60         = 1,    ///< HDMI Video format is 640x480 & Progressive 60fps
	DISP_HDMIMODE_720X480P60         = 2,    ///< HDMI Video format is 720x480 & Progressive 60fps & 4:3
	DISP_HDMIMODE_720X480P60_16X9    = 3,    ///< HDMI Video format is 720x480 & Progressive 60fps & 16:9
	DISP_HDMIMODE_1280X720P60        = 4,    ///< HDMI Video format is 1280x720 & Progressive 60fps
	DISP_HDMIMODE_1920X1080I60       = 5,    ///< HDMI Video format is 1920x1080 & Interlaced 60fps
	DISP_HDMIMODE_720X480I60         = 6,    ///< HDMI Video format is 720x480 & Interlaced 60fps
	DISP_HDMIMODE_720X480I60_16X9    = 7,    ///< HDMI Video format is 720x480 & Interlaced 60fps & 16:9
	DISP_HDMIMODE_720X240P60         = 8,    ///< HDMI Video format is 720x240 & Progressive 60fps
	DISP_HDMIMODE_720X240P60_16X9    = 9,    ///< HDMI Video format is 720x240 & Progressive 60fps & 16:9
	DISP_HDMIMODE_1440X480I60        = 10,   ///< HDMI Video format is 1440x480 & Interlaced 60fps
	DISP_HDMIMODE_1440X480I60_16X9   = 11,   ///< HDMI Video format is 1440x480 & Interlaced 60fps & 16:9
	DISP_HDMIMODE_1440X240P60        = 12,   ///< HDMI Video format is 1440x240 & Progressive 60fps
	DISP_HDMIMODE_1440X240P60_16X9   = 13,   ///< HDMI Video format is 1440x240 & Progressive 60fps & 16:9
	DISP_HDMIMODE_1440X480P60        = 14,   ///< HDMI Video format is 1440x480 & Progressive 60fps
	DISP_HDMIMODE_1440X480P60_16X9   = 15,   ///< HDMI Video format is 1440x480 & Progressive 60fps & 16:9
	DISP_HDMIMODE_1920X1080P60       = 16,   ///< HDMI Video format is 1920x1080 & Progressive 60fps
	DISP_HDMIMODE_720X576P50         = 17,   ///< HDMI Video format is 720x576 & Progressive 50fps
	DISP_HDMIMODE_720X576P50_16X9    = 18,   ///< HDMI Video format is 720x576 & Progressive 50fps & 16:9
	DISP_HDMIMODE_1280X720P50        = 19,   ///< HDMI Video format is 1280x720 & Progressive 50fps
	DISP_HDMIMODE_1920X1080I50       = 20,   ///< HDMI Video format is 1920x1080 & Interlaced 50fps
	DISP_HDMIMODE_720X576I50         = 21,   ///< HDMI Video format is 720x576 & Interlaced 50fps
	DISP_HDMIMODE_720X576I50_16X9    = 22,   ///< HDMI Video format is 720x576 & Interlaced 50fps & 16:9
	DISP_HDMIMODE_720X288P50         = 23,   ///< HDMI Video format is 720x288 & Progressive 50fps
	DISP_HDMIMODE_720X288P50_16X9    = 24,   ///< HDMI Video format is 720x288 & Progressive 50fps & 16:9
	DISP_HDMIMODE_1440X576I50        = 25,   ///< HDMI Video format is 1440x576 & Interlaced 50fps
	DISP_HDMIMODE_1440X576I50_16X9   = 26,   ///< HDMI Video format is 1440x576 & Interlaced 50fps & 16:9
	DISP_HDMIMODE_1440X288P50        = 27,   ///< HDMI Video format is 1440x288 & Progressive 50fps
	DISP_HDMIMODE_1440X288P50_16X9   = 28,   ///< HDMI Video format is 1440x288 & Progressive 50fps & 16:9
	DISP_HDMIMODE_1440X576P50        = 29,   ///< HDMI Video format is 1440x576 & Progressive 50fps
	DISP_HDMIMODE_1440X576P50_16X9   = 30,   ///< HDMI Video format is 1440x576 & Progressive 50fps & 16:9
	DISP_HDMIMODE_1920X1080P50       = 31,   ///< HDMI Video format is 1920x1080 & Progressive 50fps
	DISP_HDMIMODE_1920X1080P24       = 32,   ///< HDMI Video format is 1920x1080 & Progressive 24fps
	DISP_HDMIMODE_1920X1080P25       = 33,   ///< HDMI Video format is 1920x1080 & Progressive 25fps
	DISP_HDMIMODE_1920X1080P30       = 34,   ///< HDMI Video format is 1920x1080 & Progressive 30fps
	DISP_HDMIMODE_1920X1080I50_VT1250 = 39,  ///< HDMI Video format is 1920x1080 & Interlaced  50fps & V-total is 1250 lines
	DISP_HDMIMODE_1920X1080I100      = 40,   ///< HDMI Video format is 1920x1080 & Interlaced  100fps
	DISP_HDMIMODE_1280X720P100       = 41,   ///< HDMI Video format is 1280X720  & Progressive 100fps
	DISP_HDMIMODE_720X576P100        = 42,   ///< HDMI Video format is 720X576   & Progressive 100fps
	DISP_HDMIMODE_720X576P100_16X9   = 43,   ///< HDMI Video format is 720X576   & Progressive 100fps & 16:9
	DISP_HDMIMODE_720X576I100        = 44,   ///< HDMI Video format is 720X576  & Interlaced 100fps
	DISP_HDMIMODE_720X576I100_16X9   = 45,   ///< HDMI Video format is 720X576  & Interlaced 100fps & 16:9
	DISP_HDMIMODE_1920X1080I120      = 46,   ///< HDMI Video format is 1920X1080 & Interlaced 120fps
	DISP_HDMIMODE_1280X720P120       = 47,   ///< HDMI Video format is 1280X720  & Progressive 120fps
	DISP_HDMIMODE_720X480P120        = 48,   ///< HDMI Video format is 720X480   & Progressive 120fps
	DISP_HDMIMODE_720X480P120_16X9   = 49,   ///< HDMI Video format is 720X480   & Progressive 120fps & 16:9
	DISP_HDMIMODE_720X480I120        = 50,   ///< HDMI Video format is 720X480  & Interlaced 120fps
	DISP_HDMIMODE_720X480I120_16X9   = 51,   ///< HDMI Video format is 720X480  & Interlaced 120fps & 16:9
	DISP_HDMIMODE_720X576P200        = 52,   ///< HDMI Video format is 720X576  & Progressive 200fps
	DISP_HDMIMODE_720X576P200_16X9   = 53,   ///< HDMI Video format is 720X576  & Progressive 200fps & 16:9
	DISP_HDMIMODE_720X576I200        = 54,   ///< HDMI Video format is 720X576  & Interlaced 200fps
	DISP_HDMIMODE_720X576I200_16X9   = 55,   ///< HDMI Video format is 720X576  & Interlaced 200fps & 16:9
	DISP_HDMIMODE_720X480P240        = 56,   ///< HDMI Video format is 720X480  & Progressive 240fps
	DISP_HDMIMODE_720X480P240_16X9   = 57,   ///< HDMI Video format is 720X480  & Progressive 240fps & 16:9
	DISP_HDMIMODE_720X480I240        = 58,   ///< HDMI Video format is 720X480  & Interlaced 240fps
	DISP_HDMIMODE_720X480I240_16X9   = 59,   ///< HDMI Video format is 720X480  & Interlaced 240fps & 16:9

	//#NT#2017/01/24#Jeah Yen -begin
	//#NT#Support new HDMI mode
	DISP_HDMIMODE_3840X2160P24       = 93,   ///< HDMI Video format is 3840x2160 & Progressive 24fps
	DISP_HDMIMODE_3840X2160P25       = 94,   ///< HDMI Video format is 3840x2160 & Progressive 25fps
	DISP_HDMIMODE_3840X2160P30       = 95,   ///< HDMI Video format is 3840x2160 & Progressive 30fps
	DISP_HDMIMODE_4096X2160P24       = 98,   ///< HDMI Video format is 4096x2160 & Progressive 24fps
	DISP_HDMIMODE_4096X2160P25       = 99,   ///< HDMI Video format is 4096x2160 & Progressive 25fps
	DISP_HDMIMODE_4096X2160P30       = 100,  ///< HDMI Video format is 4096x2160 & Progressive 30fps

	//#NT#2017/12/04#Jeah Yen -begin
	//#NT#Support HDMI 1.4b mode
	DISP_HDMIMODE_3840X2160P30_14B   = 95+128,///< HDMI 1.4B Extended foramt HDMI_VIC=1. Video format is 3840x2160 & Progressive 30fps
	DISP_HDMIMODE_3840X2160P25_14B   = 94+128,///< HDMI 1.4B Extended foramt HDMI_VIC=2. Video format is 3840x2160 & Progressive 30fps
	DISP_HDMIMODE_3840X2160P24_14B   = 93+128,///< HDMI 1.4B Extended foramt HDMI_VIC=3. Video format is 3840x2160 & Progressive 24fps
	DISP_HDMIMODE_4096X2160P24_14B   = 98+128,///< HDMI 1.4B Extended foramt HDMI_VIC=4. Video format is 4096x2160 & Progressive 24fps

	DISP_HDMIMODE_MAX                = 255,
	//#NT#2017/12/04#Jeah Yen -end
	//#NT#2017/01/24#Jeah Yen -end

	ENUM_DUMMY4WORD(DISP_HDMIMODE)
} DISP_HDMIMODE;


typedef enum {
	AUD_HDMIMODE_32KHZ               = 0,    ///< HDMI input Audio is 32KHz
	AUD_HDMIMODE_44_1KHZ             = 1,    ///< HDMI input Audio is 44.1KHz
	AUD_HDMIMODE_48KHZ               = 2,    ///< HDMI input Audio is 48KHz

	ENUM_DUMMY4WORD(AUD_HDMIMODE)
} AUD_HDMIMODE;

#endif //_DISPDEF_H_

