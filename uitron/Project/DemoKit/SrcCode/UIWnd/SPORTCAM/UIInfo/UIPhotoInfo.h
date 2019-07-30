#ifndef UI_PHOTOINFO_H
#define UI_PHOTOINFO_H

#include "PrjCfg.h"
#include "UIInfo.h"
// user should check enum depend on different project


//-----------------------------------------------------------------------------
// Image Effect mode
//-----------------------------------------------------------------------------
typedef enum {
	IMAGEEFFECT_SKETCH = 0,
	IMAGEEFFECT_FISHEYE,
	IMAGEEFFECT_COLORPENCIL,
	IMAGEEFFECT_WDR,
	IMAGEEFFECT_ANTISHAKE,
	IMAGEEFFECT_HDR,
	IMAGEEFFECT_SETTING_MAX
} IMAGEEFFECT_SETTING;

//-----------------------------------------------------------------------------
// Image Color
//-----------------------------------------------------------------------------
typedef enum {
	COLOR_STANDARD = 0,
	COLOR_VIVID,
	COLOR_BW,
	COLOR_SEPIA,
	COLOR_SETTING_MAX
} COLOR_SETTING;


//-----------------------------------------------------------------------------
// Auto Focus
//-----------------------------------------------------------------------------
typedef enum {
	AFWINDOW_SINGLE = 0,
	AFWINDOW_MULTI,
	AFWINDOW_SETTING_MAX
} AFWINDOW_SETTING;

//-----------------------------------------------------------------------------
// Cont. AF
//-----------------------------------------------------------------------------
typedef enum {
	CONT_AF_ON = 0,
	CONT_AF_OFF,
	CONT_AF_SETTING_MAX
} CONT_AF_SETTING;

//-----------------------------------------------------------------------------
// AF Assist Beam
//-----------------------------------------------------------------------------
typedef enum {
	AFBEAM_OFF = 0,
	AFBEAM_ON,
	AFBEAM_SETTING_MAX
} AFBEAN_SETTING;

//-----------------------------------------------------------------------------
// Digital Zoom
//-----------------------------------------------------------------------------
typedef enum {
	DZOOM_OFF = 0,
	DZOOM_ON,
	DZOOM_SETTING_MAX
} DZOOM_SETTING;

//-----------------------------------------------------------------------------
// Date Imprint State
//-----------------------------------------------------------------------------
typedef enum {
	DATEIMPRINT_OFF = 0,
	DATEIMPRINT_DATE,
	DATEIMPRINT_DATE_TIME,
	DATEIMPRINT_SETTING_MAX
} DATEIMPRINT_SETTING;


typedef enum {
	DZOOM_10X = 0,   // 0
	DZOOM_12X,
	DZOOM_14X,
	DZOOM_16X,
	DZOOM_18X,
	DZOOM_20X,      // 5
	DZOOM_22X,
	DZOOM_24X,
	DZOOM_26X,
	DZOOM_28X,
	DZOOM_30X,      // 10
	DZOOM_32X,
	DZOOM_34X,
	DZOOM_36X,
	DZOOM_38X,
	DZOOM_40X,      // 15
	DZOOM_45X,
	DZOOM_50X,
	DZOOM_55X,
	DZOOM_60X,
	DZOOM_X_SETTING_MAX  // 20
} DZOOM_X_SETTING;


#if 0

// Cont Shot State
//-----------------------------------------------------------------------------
typedef enum {
	CONT_SHOT_STATE_OFF = 0x00,
	CONT_SHOT_STATE_ON,
	CONT_SHOT_STATE_SETTING_MAX
} CONT_SHOT_STATE_SETTIMG;

//-----------------------------------------------------------------------------
// Cont Shot Type
//-----------------------------------------------------------------------------

typedef enum {
	CONT_SHOT_OFF = 0,
	CONT_SHOT_INFINITE,
	CONT_SHOT_10,
	CONT_SHOT_3,
	CONT_SHOT_TIMELAPSE,
	CONT_SHOT_SETTING_MAX
} CONT_SHOT_SETTING;
#endif
typedef enum {
	ZOOM_IF_OPTICAL = 0,
	ZOOM_IF_DIGITAL,
	ZOOM_IF_BOTH
} ZOOM_IF_INDEX;
//-----------------------------------------------------------------------------
// Saturation Level
//-----------------------------------------------------------------------------
typedef enum {
	SATURATION_P1 = 0,
	SATURATION_NORNAL,
	SATURATION_N1,
	SATURATION_SETTING_MAX
} SATURATION_SETTING;

//-----------------------------------------------------------------------------
// Contrast Level
//-----------------------------------------------------------------------------
typedef enum {
	CONTRAST_P4 = 0,
	CONTRAST_P3,
	CONTRAST_P2,
	CONTRAST_P1,
	CONTRAST_NORMAL,
	CONTRAST_N1,
	CONTRAST_N2,
	CONTRAST_N3,
	CONTRAST_N4,
	CONTRAST_SETTING_MAX
} CONTRAST_SETTING;

//-----------------------------------------------------------------------------
// Image Ratio
//-----------------------------------------------------------------------------

typedef enum {
	IMAGERATIO_9_16 = 0,
	IMAGERATIO_2_3,
	IMAGERATIO_3_4,
	IMAGERATIO_1_1,
	IMAGERATIO_4_3,
	IMAGERATIO_3_2,
	IMAGERATIO_16_9,
	IMAGERATIO_MAX_CNT,
	ENUM_DUMMY4WORD(IMAGERATIO_SETTING)
} IMAGERATIO_SETTING;





#endif //UI_PHOTOINFO_H

