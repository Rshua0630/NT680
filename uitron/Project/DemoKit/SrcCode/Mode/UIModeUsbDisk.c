////////////////////////////////////////////////////////////////////////////////
#include "SysCommon.h"
//#include "AppCommon.h"
////////////////////////////////////////////////////////////////////////////////
#include "AppLib.h"
#include "UIMode.h"
#include "UIModeUsbDisk.h"
#include "UIAppUsbDisk.h"
//UIControl
#include "UIFrameworkExt.h"

#include "DeviceCtrl.h"
#if !defined(_UI_STYLE_USB_DISK_)
#include "UIFlow.h"
#endif

int PRIMARY_MODE_USBMSDC = -1;    ///< USB MSDC

void ModeUsbDisk_Open(void);
void ModeUsbDisk_Close(void);

void ModeUsbDisk_Open(void)
{
	INT32 retV;

	Input_ResetMask();
	USB_PlugInSetFunc();
	Ux_SetActiveApp(&CustomMSDCObjCtrl);
	retV = Ux_SendEvent(0, NVTEVT_EXE_OPEN, 0);
	if (retV != E_TMOUT) {
#if (!defined(_UI_STYLE_IPCAM_) && !defined(_UI_STYLE_USB_DISK_))
		Ux_OpenWindow((VControl *)(&UIFlowWndUSBCtrl), 1, UIFlowWndUSB_MSDC_MODE);
#endif
	} else {
		debug_msg("MSDCOpen TO:%d\r\n", retV);
	}
}
void ModeUsbDisk_Close(void)
{
	#if !defined(_UI_STYLE_USB_DISK_)
	Ux_CloseWindow((VControl *)(&UIFlowWndUSBCtrl), 0);
	#endif
	Ux_SendEvent(0, NVTEVT_EXE_CLOSE, 0);
}

SYS_MODE gModeUsbDisk = {
	"USBDISK",
	ModeUsbDisk_Open,
	ModeUsbDisk_Close,
	NULL,
	NULL,
	NULL,
	NULL
};
