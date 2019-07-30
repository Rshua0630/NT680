#ifndef _APPCTRL_H_
#define _APPCTRL_H_

#include "Type.h"
#include "Debug.h"
#include "VControl.h"

//////////////////////////////////////////////////

// APP event class

typedef enum {
	NVTEVT_FLOW_EVT_START       = SYS_FLOW_EVENT_BASE, ///< Min value = 0x10010000
	NVTEVT_SYSTEM_BOOT          = 0x10010000, ///< Open first mode here
	NVTEVT_SYSTEM_SHUTDOWN      = 0x10010001, ///< Close last mode here
	NVTEVT_SYSTEM_MODE          = 0x10010002, ///< Change mode
	NVTEVT_SYSTEM_SLEEP         = 0x10010003, ///< Change sleep level of mode
	/* INSERT NEW EVENT HRER */
	NVTEVT_FLOW_EVT_END         = SYS_FLOW_EVENT_BASE + 0x1000 - 1, ///< Max value = 0x10010fff

	NVTEVT_MODE_EVT_START       = SYS_MODE_EVENT_BASE, ///< Min value = 0x10020000
	NVTEVT_EXE_OPEN             = 0x10020000,
	NVTEVT_EXE_CLOSE            = 0x10020001,
	NVTEVT_EXE_SLEEP            = 0x10020003,
	NVTEVT_EXE_WAKEUP           = 0x10020004,
	/* INSERT NEW EVENT HRER */
	NVTEVT_MODE_EVT_END         = SYS_MODE_EVENT_BASE + 0x1000 - 1, ///< Max value = 0x10020fff

	NVTEVT_STRG_EVT_START       = DEV_STRG_EVENT_BASE, ///< Min value = 0x11010000
	NVTEVT_STRG_INSERT          = 0x11010000, //Param1=storage id
	NVTEVT_STRG_REMOVE          = 0x11010001, //Param1=storage id
	NVTEVT_STRG_DETACH          = 0x11010002, //Param1=storage id, Param2=status
	NVTEVT_STRG_ATTACH          = 0x11010003, //Param1=storage id, Param2=status
	NVTEVT_STRG_CHANGE          = 0x11010004, //Param1=storage id, if insert / remove occurs, also indicate change flag
	/* INSERT NEW EVENT HRER */
	NVTEVT_STRG_EVT_END         = DEV_STRG_EVENT_BASE + 0x1000 - 1, ///< Max value = 0x11010fff

	NVTEVT_VOUT_EVT_START       = DEV_VOUT_EVENT_BASE, ///< Min value = 0x11020000
	NVTEVT_VIDEO_INSERT         = 0x11020000, //Param1=insert/remove, Param2=video-type: TV, HDMI
	NVTEVT_VIDEO_REMOVE         = 0x11020001, //Param1=insert/remove, Param2=video-type: TV, HDMI
	NVTEVT_VIDEO_DETACH         = 0x11020002, //Param1=status, Param2=video-type: LCD, TV, HDMI
	NVTEVT_VIDEO_ATTACH         = 0x11020003, //Param1=status, Param2=video-type: LCD, TV, HDMI
	NVTEVT_VIDEO_MODE           = 0x11020004, //Param1=status, Param2=video-type: LCD, TV, HDMI
	NVTEVT_VIDEO_DIR            = 0x11020005, //Param1=status, Param2=video-type: LCD, TV, HDMI
	NVTEVT_VIDEO_INIT           = 0x11020010,
	NVTEVT_VIDEO_CHANGE         = 0x11020011,
	/* INSERT NEW EVENT HRER */
	NVTEVT_VOUT_EVT_END         = DEV_VOUT_EVENT_BASE + 0x1000 - 1, ///< Max value = 0x11020fff

	NVTEVT_AOUT_EVT_START       = DEV_AOUT_EVENT_BASE, ///< Min value = 0x11030000
	NVTEVT_AUDIO_INSERT         = 0x11030000, //Param1=insert/remove, Param2=audio-type: LINEOUT, HEAR-PHONE, HDMI
	NVTEVT_AUDIO_REMOVE         = 0x11030001, //Param1=insert/remove, Param2=audio-type: LINEOUT, HEAR-PHONE, HDMI
	NVTEVT_AUDIO_DETACH         = 0x11030002, //Param1=status, Param2=audio-type: SPEAKER, LINEOUT, HEAR-PHONE, HDMI
	NVTEVT_AUDIO_ATTACH         = 0x11030003, //Param1=status, Param2=audio-type: SPEAKER, LINEOUT, HEAR-PHONE, HDMI
	NVTEVT_AUDIO_INPUT          = 0x11030004, //Param1=start/stop, Param2=pointer to WAVSTUD_AUD_INFO
	NVTEVT_AUDIO_PLAY_VOL       = 0x11030005, //Param1=vol, Param2=N/A
	NVTEVT_AUDIO_MODE           = 0x11030006, //Param1=mode, Param2=N/A
	/* INSERT NEW EVENT HRER */
	NVTEVT_AOUT_EVT_END         = DEV_AOUT_EVENT_BASE + 0x1000 - 1, ///< Max value = 0x11030fff

	NVTEVT_VIN__EVT_START       = DEV_VIN_EVENT_BASE, ///< Min value = 0x11040000
	NVTEVT_SENSOR_INSERT        = 0x11040000, //Param1=insert/remove, Param2=sensor-id
	NVTEVT_SENSOR_REMOVE        = 0x11040001, //Param1=insert/remove, Param2=sensor-id
	NVTEVT_SENSOR_DETACH        = 0x11040002,
	NVTEVT_SENSOR_ATTACH        = 0x11040003,
	NVTEVT_SENSOR_DISPLAY       = 0x11040004,
	/* INSERT NEW EVENT HRER */
	NVTEVT_VIN__EVT_END         = DEV_VIN_EVENT_BASE + 0x1000 - 1, ///< Max value = 0x11040fff

	NVTEVT_USB_EVT_START        = DEV_USB_EVENT_BASE, ///< Min value = 0x11050000
	NVTEVT_USB_INSERT           = 0x11050000, //Param1=USB connection type
	NVTEVT_USB_REMOVE           = 0x11050001, //Param1=USB connection type
	NVTEVT_USB_CHARGE_CURRENT   = 0x11050002, //Param1=available charging current
	/* INSERT NEW EVENT HRER */
	NVTEVT_USB_EVT_END          = DEV_USB_EVENT_BASE + 0x1000 - 1, ///< Max value = 0x11050fff

	NVTEVT_POWER_EVT_START      = DEV_POWER_EVENT_BASE, ///< Min value = 0x11080000
	NVTEVT_PWR_BATTEMPTY        = 0x11080000,
	NVTEVT_PWR_BATTCHANGE       = 0x11080001,
	NVTEVT_PWR_CHARGE_OK        = 0x11080002,
	NVTEVT_PWR_CHARGE_SUSPEND   = 0x11080003,
	NVTEVT_PWR_CHARGE_RESUME    = 0x11080004,
	/* INSERT NEW EVENT HRER */
	NVTEVT_POWER_EVT_END        = DEV_POWER_EVENT_BASE + 0x1000 - 1, ///< Max value = 0x11080fff

	NVTEVT_LENS_EVT_START       = DEV_LENS_EVENT_BASE, ///< Min value = 0x11090000
	NVTEVT_LENS_DETACH          = 0x11090000,
	NVTEVT_LENS_ATTACH          = 0x11090001,
	/* INSERT NEW EVENT HRER */
	NVTEVT_LENS_EVT_END         = DEV_LENS_EVENT_BASE + 0x1000 - 1, ///< Max value = 0x11090fff
}
NVT_SYS_EVENT;

//////////////////////////////////////////////////

extern INT32 Ux_AppDispatchMessage(NVTEVT evt, UINT32 paramNum, UINT32 *paramArray);

#endif //_APPCTRL_H_
