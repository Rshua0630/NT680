
#ifndef UI_SYSTEMOBJ_H
#define UI_SYSTEMOBJ_H

#include "NVTEvent.h"
#include "UIAppCommon.h"
#include "NVTUserCommand.h"
#if(IPCAM_FUNC !=ENABLE)
#include "UIInfo.h"
#endif
// Change DSC mode type
#define DSCMODE_CHGTO_NEXT              0
#define DSCMODE_CHGTO_PREV              1
#define DSCMODE_CHGTO_CURR              2

typedef enum {
	NVTEVT_SETUP_EVT_START      = APPUSER_SETUP_BASE, ///< Min value = 0x14080000
	/* INSERT NEW EVENT HRER */
	//System
	NVTEVT_EXE_FWUPDATE         = 0x14080000, ///< system update FW
	NVTEVT_EXE_SYSRESET         = 0x14080001, ///< system reset
	NVTEVT_EXE_SYSRESET_NO_WIN  = 0x14080002, ///< system reset
	NVTEVT_EXE_DATEFORMAT       = 0x14080003, ///< system date-time
	NVTEVT_EXE_TIMEFORMAT       = 0x14080004, ///< system date-time
	NVTEVT_EXE_DATE             = 0x14080005, ///< system date-time
	NVTEVT_EXE_TIME             = 0x14080006, ///< system date-time
	NVTEVT_EXE_DISPLAY          = 0x14080007, ///< system UI display style
	NVTEVT_EXE_LANGUAGE         = 0x14080008, ///< system UI string table
	NVTEVT_EXE_POWEROFF         = 0x14080009, ///< system auto power off
	/* INSERT NEW EVENT HRER */
	NVTEVT_EXE_CHANGEDSCMODE    = 0x14080010, ///< mode
	NVTEVT_PREVIEWMODE      = 0x14080011, ///< mode
	NVTEVT_FORCETO_LIVEVIEW_MODE = 0x14080012, ///< mode
	NVTEVT_FORCETO_PLAYBACK_MODE = 0x14080013, ///< mode
	/* INSERT NEW EVENT HRER */
	//Storage
	NVTEVT_EXE_FORMAT           = 0x14080020, ///< device storage
	NVTEVT_EXE_SETUPCOPY2CARD   = 0x14080021, ///< device storage
	NVTEVT_EXE_NUMRESET        = 0x14080022, ///< device storage DCF reset
	NVTEVT_FILEID_RESET        = 0x14080023, ///< device storage FDB reset
	/* INSERT NEW EVENT HRER */
	//Sensor
	NVTEVT_EXE_SENSOR_ROTATE    = 0x14080030, ///< device sensor
	/* INSERT NEW EVENT HRER */
	//Audio
	NVTEVT_EXE_BEEPVOLUME       = 0x14080040, ///< audio
	NVTEVT_EXE_BEEPKEY          = 0x14080041, ///< audio
	/* INSERT NEW EVENT HRER */
	//Display
	NVTEVT_EXE_DUALDISP         = 0x14080050, ///< device display dual
	NVTEVT_EXE_LCDFORMAT        = 0x14080052, ///< device display LCD format
	NVTEVT_EXE_TVFORMAT         = 0x14080052, ///< device display TV format
	NVTEVT_EXE_HDMIFORMAT       = 0x14080053, ///< device display HDMI format
	NVTEVT_EXE_LCDOFF           = 0x14080054, ///< device display LCD brightness auto off
	NVTEVT_EXE_BRIGHTADJ        = 0x14080055, ///< device display LCD brightness auto level
	NVTEVT_EXE_BRIGHTLVL        = 0x14080056, ///< device display LCD brightness manual level
	NVTEVT_EXE_LCD_ROTATE       = 0x14080057, ///< devicedisplay LCD rotate
	/* INSERT NEW EVENT HRER */
	//USB
	NVTEVT_EXE_USBMODE          = 0x14080060, ///< usb mode
	/* INSERT NEW EVENT HRER */
	//AE
	NVTEVT_EXE_FREQ             = 0x14080070, ///< AE
	/* INSERT NEW EVENT HRER */
	//Lens
	NVTEVT_EXE_PB_RETRACT_LENS     = 0x14080080, ///< lens
	/* INSERT NEW EVENT HRER */
	NVTEVT_SETUP_EVT_END        = APPUSER_SETUP_BASE + 0x1000 - 1, ///< Max value = 0x14080fff
	//Wifi
	NVTEVT_EXE_WIFI             = 0x14080090, ///< Wifi
#if !defined(_NVT_SDIO_WIFI_NONE_) || !defined(_NVT_USB_WIFI_NONE_)
	NVTEVT_EXE_WIFI_MODE        = 0x14080091, ///< Wifi
	NVTEVT_EXE_WIFI_START       = 0x14080092, ///< Wifi
	NVTEVT_EXE_WIFI_STOP        = 0x14080093, ///< Wifi
	NVTEVT_EXE_WIFI_SET_SSID    = 0x14080094, ///< Wifi
	NVTEVT_EXE_WIFI_SET_PASSPHRASE  = 0x14080095, ///< Wifi
	NVTEVT_EXE_WIFI_SET_AUTHTYPE = 0x14080096, ///< Wifi
	NVTEVT_EXE_WIFI_DHCP_START  = 0x14080097, ///< Wifi
	NVTEVT_EXE_WIFI_REINIT      = 0x14080098, ///< Wifi
	NVTEVT_EXE_WIFI_BACK2DEV    = 0x14080099, ///< Wifi
	NVTEVT_EXE_WIFI_SET_PIP_STYLE          = 0x14080100, ///< Wifi
	NVTEVT_EXE_WIFI_SEND_SSID_PASSPHRASE   = 0x14080101, ///< Wifi
    NVTEVT_EXE_WIFI_REQ_QR_DECODE          = 0x14080102, ///< Wifi
#endif
	/* INSERT NEW EVENT HRER */
}
CUSTOM_SYS_EVENT;

extern VControl UISetupObjCtrl;

extern EVENT_ENTRY UISystemObjCtrlCmdMap[];

extern BOOL SetupExe_IsNoFile(void);

#endif //UI_SYSTEMOBJ_H
