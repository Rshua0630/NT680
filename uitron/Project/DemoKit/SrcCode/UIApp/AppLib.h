#ifndef _APPLIB_H_
#define _APPLIB_H_

#include "ErrorNo.h"
#include "Debug.h"
#include "Utility.h"

/////////////////////////////////////////
// AppLib

#include "UIFramework.h"
#include "UIControlEvent.h"
#include "AppControl.h"
#include "PrjCfg.h"

typedef enum {
	APP_BASE = APP_TYPE_MIN,
	APP_SETUP,
	APP_PLAY,
	APP_PHOTO,
	APP_MOVIEPLAY,
	APP_MOVIEREC,
	APP_PRINT,
	APP_PCC,
	APP_MSDC,
	APP_USBCHARGE,
#if(WIFI_AP_FUNC==ENABLE)
	APP_WIFICMD,
#endif
	APP_TYPE_MAX
}
APP_TYPE;


#endif //_APPLIB_H_
