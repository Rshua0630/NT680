#include "WifiAppCmdMapping.h"
#include "UIFlow.h"

#define MOVE_ZISE_SUPPORT       DISABLE

WIFIAPPINDEXMAP photoSizeMap[] = {
	{PHOTO_SIZE_12M, "12M"  },
	{PHOTO_SIZE_10M, "10M"  },
	{PHOTO_SIZE_8M, "8M"   },
	{PHOTO_SIZE_5M, "5M"   },
	{PHOTO_SIZE_3M, "3M"   },
	{PHOTO_SIZE_VGA, "VGA"  },
	{INDEX_END, 0}
};
#if MOVE_ZISE_SUPPORT
WIFIAPPINDEXMAP movieSizeMap[] = {
	{MOVIE_SIZE_FRONT_2880x2160P24, "2880x2160P24"},           //UHD P24
	{MOVIE_SIZE_FRONT_2560x1440P30, "2560x1440P30"},           //QHD P30
	{MOVIE_SIZE_FRONT_2304x1296P30, "2304x1296P30"},           //3MHD P30
	{MOVIE_SIZE_FRONT_1920x1080P96, "1920x1080P96"},           //FHD
	{MOVIE_SIZE_FRONT_1920x1080P60, "1920x1080P60"},           //FHD
	{MOVIE_SIZE_FRONT_1920x1080P30, "1920x1080P30"},           //FHD
	{MOVIE_SIZE_FRONT_1280x720P120, "1280x720P120"},           //HD
	{MOVIE_SIZE_FRONT_1280x720P60, "1280x720P60"},             //HD
	{MOVIE_SIZE_FRONT_1280x720P30, "1280x720P30"},             //HD
	{MOVIE_SIZE_FRONT_848x480P30, "848x480P30"},               //WVGA
	{MOVIE_SIZE_FRONT_640x480P240, "640x480P240"},             //VGA
	{MOVIE_SIZE_FRONT_640x480P30, "640x480P30"},               //VGA
	{MOVIE_SIZE_FRONT_320x240P30, "320x240P30"},               //QVGA
	{MOVIE_SIZE_DUAL_1920x1080P30_1920x1080P30, "1920x1080P30_1920x1080P30"}, //FHD FHD
	{INDEX_END, 0}
};
#endif

WIFIAPPINDEXMAP cyclicRecMap[] = {
	{MOVIE_CYCLICREC_OFF,  "OFF"  },
	{MOVIE_CYCLICREC_1MIN, "1MIN"  },
	{MOVIE_CYCLICREC_3MIN, "3MIN"  },
	{MOVIE_CYCLICREC_5MIN, "5MIN"  },
	{MOVIE_CYCLICREC_10MIN, "10MIN" },
	{INDEX_END, 0}
};

WIFIAPPINDEXMAP WDRMap[] = {
	{MOVIE_WDR_OFF, "OFF"  },
	{MOVIE_WDR_ON, "ON"  },
	{INDEX_END, 0}
};

WIFIAPPINDEXMAP EVMap[] = {
	{EV_P20, "+2.0"},
	{EV_P16, "+1.6"},
	{EV_P13, "+1.3"},
	{EV_P10, "+1.0"},
	{EV_P06, "+0.6"},
	{EV_P03, "+0.3"},
	{EV_00, "0.0" },
	{EV_N03, "-0.3"},
	{EV_N06, "-0.6"},
	{EV_N10, "-1.0"},
	{EV_N13, "-1.3"},
	{EV_N16, "-1.6"},
	{EV_N20, "-2.0"},
	{INDEX_END, 0}
};

WIFIAPPINDEXMAP motionDetMap[] = {
	{MOVIE_MOTIONDET_OFF, "OFF"  },
	{MOVIE_MOTIONDET_ON, "ON"  },
	{INDEX_END, 0}
};

WIFIAPPINDEXMAP audioMap[] = {
	{MOVIE_AUDIO_OFF, "OFF"  },
	{MOVIE_AUDIO_ON, "ON"  },
	{INDEX_END, 0}
};

WIFIAPPINDEXMAP dateImprintMap[] = {
	{MOVIE_DATEIMPRINT_OFF, "OFF"  },
	{MOVIE_DATEIMPRINT_ON, "ON"  },
	{INDEX_END, 0}
};

WIFIAPPINDEXMAP gSensorMap[] = {
	{GSENSOR_OFF, "OFF"  },
	{GSENSOR_LOW, "LOW"  },
	{GSENSOR_MED, "MED"  },
	{GSENSOR_HIGH, "HIGH"  },
	{INDEX_END, 0}
};

WIFIAPPINDEXMAP wifiAutoRecMap[] = {
	{WIFI_AUTO_RECORDING_OFF, "OFF" },
	{WIFI_AUTO_RECORDING_ON, "ON"  },
	{INDEX_END, 0}
};

WIFIAPPINDEXMAP powerOffMap[] = {
	{POWER_ON,    "ON"},
	{POWER_1MIN,  "1MIN"},
	{POWER_2MIN,  "2MIN"},
	{POWER_3MIN,  "3MIN"},
	{POWER_5MIN,  "5MIN"},
	{POWER_10MIN, "10MIN"},
	{INDEX_END, 0}
};

WIFIAPPINDEXMAP langMap[] = {
	{LANG_EN,  "EN"},
	{LANG_FR,  "FR"},
	{LANG_ES,  "ES"},
	{LANG_PO,  "PO"},
	{LANG_DE,  "DE"},
	{LANG_IT,  "IT"},
	{LANG_SC,  "SC"},
	{LANG_TC,  "TC"},
	{LANG_RU,  "RU"},
	{LANG_JP,  "JP"},
	{INDEX_END, 0}
};

WIFIAPPINDEXMAP TVMap[] = {
	{TV_MODE_NTSC, "NTSC"},
	{TV_MODE_PAL,  "PAL"},
	{INDEX_END, 0}
};

WIFIAPPMAP wifiAppFlagMap[] = {
	{FL_PHOTO_SIZE,         "PHOTO_SIZE",         photoSizeMap},
#if MOVE_ZISE_SUPPORT
	{FL_MOVIE_SIZE,         "MOVIE_SIZE",         movieSizeMap},
#endif
	{FL_MOVIE_CYCLIC_REC,   "MOVIE_CYCLIC_REC",   cyclicRecMap},
	{FL_MOVIE_WDR,          "MOVIE_WDR",          WDRMap},
	{FL_EV,                 "EV",                 EVMap},
	{FL_MOVIE_MOTION_DET,   "MOVIE_MOTION_DET",   motionDetMap},
	{FL_MOVIE_AUDIO,        "MOVIE_AUDIO",        audioMap},
	{FL_MOVIE_DATEIMPRINT,  "MOVIE_DATEIMPRINT",  dateImprintMap},
	{FL_GSENSOR,            "GSENSOR",            gSensorMap},
	{FL_WIFI_AUTO_RECORDING, "WIFI_AUTO_RECORDING", wifiAutoRecMap},
	{FL_AUTO_POWER_OFF,     "AUTO_POWER_OFF",     powerOffMap},
	{FL_LANGUAGE,           "LANGUAGE",           langMap},
	{FL_TV_MODE,            "TV_MODE",            TVMap},
	{FL_NULL, 0},
};

WIFIAPPMAP *WifiCmd_GetMapTbl(void)
{
	return wifiAppFlagMap;
}
