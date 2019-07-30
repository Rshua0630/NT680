#ifndef _UIIPCAMPB_H_
#define _UIIPCAMPB_H_

#include "AppLib.h"
#include "UIAppCommon.h"
#include "NVTEvent.h"
#include "UVAC.h"
extern VControl CustomIPCamPBObjCtrl;


typedef enum {
	NVTEVT_IPCAMPB_EVT_START      = APPUSER_IPCAMPB_BASE, ///< Min value = 0x14001000
	//Flow and basic parameter
	NVTEVT_EXE_IPCAMPB_OPEN         = NVTEVT_IPCAMPB_EVT_START,
	NVTEVT_EXE_IPCAMPB_CLOSE,

} CUSTOM_IPCAMPB_EVENT;




#endif //_UIIPCAMPB_H_
