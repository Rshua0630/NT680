#ifndef _UIAPPWIFICMD_H_
#define _UIAPPWIFICMD_H_

#include "NVTEvent.h"
#include "UIAppCommon.h"
#include "NVTUserCommand.h"
#include "UIInfo.h"
#include "UIInfo.h"
#include "UIInfo.h"
#include "UIAppWiFiCmdMovie.h"
#include "UIAppWiFiCmdPhoto.h"


typedef enum {

	// wifi event
	NVTEVT_WIFI_EXE_START      = APPUSER_WIFICMD_BASE, ///< Min value = 0x14080000


	NVTEVT_WIFI_EXE_PHOTO_SIZE  = 0x14020000,         ///< Set Photo size by Wi-Fi App
	NVTEVT_WIFI_EXE_PHOTO_CAPTURE   = 0x14020001,      ///< Execute photo capture shot by Wi-Fi App
	NVTEVT_WIFI_EXE_MOVIE_REC_SIZE  = 0x14020002,     ///< Set Movie recording size by Wi-Fi App
	NVTEVT_WIFI_EXE_MOVIE_REC   = 0x14020003,          ///< Movie recording by Wi-Fi App
	NVTEVT_WIFI_EXE_CYCLIC_REC  = 0x14020004,
	NVTEVT_WIFI_EXE_MOVIE_WDR   = 0x14020005,
	NVTEVT_WIFI_EXE_MOVIE_EV    = 0x14020006,
	NVTEVT_WIFI_EXE_MOTION_DET  = 0x14020007,
	NVTEVT_WIFI_EXE_MOVIE_AUDIO = 0x14020008,
	NVTEVT_WIFI_EXE_MOVIE_GSENSOR_SENS  = 0x14020009,  ///< Set G-sensor sensitivity
	NVTEVT_WIFI_EXE_DATEIMPRINT = 0x1402000a,
	NVTEVT_WIFI_EXE_MOVIE_LIVEVIEW_SIZE = 0x1402000b, ///< Set Movie preview size by Wi-Fi App
	NVTEVT_WIFI_EXE_SET_AUTO_RECORDING  = 0x1402000c, ///< CarDV mode or Sport DV mode.
	NVTEVT_WIFI_EXE_MOVIE_REC_BITRATE   = 0x1402000d,  ///< Set Movie recording bit rate by Wi-Fi App
	NVTEVT_WIFI_EXE_MOVIE_LIVEVIEW_BITRATE  = 0x1402000e,  ///< Set Movie preview bit rate by Wi-Fi App
	NVTEVT_WIFI_EXE_MOVIE_LIVEVIEW_START    = 0x1402000f,    ///< Set Movie preview bit rate by Wi-Fi App
	NVTEVT_WIFI_EXE_MOVIE_TRIGGER_RAWENC    = 0x14020010,       ///< Trigger RAW encode in movie recording mode.
	NVTEVT_WIFI_EXE_QUERY       = 0x14020011,              ///< Request to get whole of preferences by Wi-Fi App
	NVTEVT_WIFI_EXE_MODE        = 0x14020012,               ///< Change mode by Wi-Fi App
	NVTEVT_WIFI_EXE_SET_DATE    = 0x14020013,           ///< Set the Date by Wi-Fi App
	NVTEVT_WIFI_EXE_SET_TIME    = 0x14020014,           ///< Set the Time by Wi-Fi App
	NVTEVT_WIFI_EXE_SET_SSID    = 0x14020015,
	NVTEVT_WIFI_EXE_SET_PASSPHRASE  = 0x14020016,
	NVTEVT_WIFI_EXE_DATEFORMAT  = 0x14020017,
	NVTEVT_WIFI_EXE_TIMEFORMAT  = 0x14020018,
	NVTEVT_WIFI_EXE_POWEROFF    = 0x14020019,
	NVTEVT_WIFI_EXE_LANGUAGE    = 0x1402001a,
	NVTEVT_WIFI_EXE_TVFORMAT    = 0x1402001b,
	NVTEVT_WIFI_EXE_FORMAT      = 0x1402001c,
	NVTEVT_WIFI_EXE_SYSRESET    = 0x1402001d,
	NVTEVT_WIFI_EXE_VERSION     = 0x1402001e,
	NVTEVT_WIFI_EXE_FWUPDATE    = 0x1402001f,
	NVTEVT_WIFI_EXE_RECONNECT   = 0x14020020,
	NVTEVT_WIFI_EXE_SAVEMENU    = 0x14020021,
	NVTEVT_WIFI_EXE_REMOVE_USER = 0x14020022,
	NVTEVT_WIFI_EXE_SEND_SSID_PASSPHRASE  = 0x14020026,
	NVTEVT_WIFI_EXE_SET_WIFI_CONNECT_MODE = 0x14020027,
//#NT#2016/03/23#Isiah Chang -begin
//#NT#add new Wi-Fi UI flow.
	NVTEVT_WIFI_EXE_PIP_STYLE             = 0x14020028,
	NVTEVT_WIFI_EXE_MODE_DONE             = 0x14020029,
	NVTEVT_WIFI_EXE_APP_STARTUP           = 0x1402002a,
	NVTEVT_WIFI_EXE_APP_SESSION_CLOSE     = 0x1402002b,
	NVTEVT_WIFI_EXE_HEARTBEAT             = 0x1402002c,
//#NT#2016/03/23#Isiah Chang -end

//#NT#2016/06/03#Charlie Chang -begin
//#NT# support contrast ,two-way audio, flip_mirror, quality set
	NVTEVT_WIFI_EXE_MOVIE_CONTRAST              = 0x1402002d,
	NVTEVT_WIFI_EXE_MOVIE_AUDIOIN               = 0x1402002e,
	NVTEVT_WIFI_EXE_MOVIE_AUDIOIN_SR            = 0x1402002f,
	NVTEVT_WIFI_EXE_MOVIE_FLIP_MIRROR           = 0x14020030,
	NVTEVT_WIFI_EXE_MOVIE_QUALITY_SET           = 0x14020031,
//#NT#2016/06/03#Charlie Chang -end
//#NT#2016/12/02#Isiah Chang -begin
//#NT#Add WiFiCmd Bitrate control.
	NVTEVT_WIFI_EXE_MOVIE_BRC_ADJUST            = 0x14020033,
//#NT#2016/12/02#Isiah Chang -end
	/* INSERT NEW EVENT HRER */
}
CUSTOM_WIFI_EVENT;

extern VControl UIWifiCmdObjCtrl;
#endif
