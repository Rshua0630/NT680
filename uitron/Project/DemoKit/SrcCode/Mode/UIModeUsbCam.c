////////////////////////////////////////////////////////////////////////////////
#include "SysCommon.h"
//#include "AppCommon.h"
////////////////////////////////////////////////////////////////////////////////
#include "AppLib.h"
#include "UIMode.h"
#include "UIModeUsbCam.h"
#include "UIAppUsbCam.h"
//UIControl
#include "UIFrameworkExt.h"

#include "DeviceCtrl.h"
#include "UIFlow.h"

int PRIMARY_MODE_USBPCC = -1;     ///< USB PC camera

void ModeUsbCam_Open(void);
void ModeUsbCam_Close(void);

void ModeUsbCam_Open(void)
{
#if (USB_MODE == ENABLE)
	Input_ResetMask();
	USB_PlugInSetFunc();

#if (UVAC_VID_DISPLAY == ENABLE)
	UI_Show(UI_SHOW_WINDOW, FALSE);
	UI_FlushDisplayShow_Disable();
#endif

	Ux_SetActiveApp(&CustomUSBPCCObjCtrl);
	Ux_SendEvent(0, NVTEVT_EXE_OPEN, 0);
#if !defined(_UI_STYLE_IPCAM_)
	Ux_OpenWindow((VControl *)(&UIFlowWndUSBCtrl), 1, UIFlowWndUSB_PCC_MODE);
#endif
#endif
}
void ModeUsbCam_Close(void)
{
	Ux_SendEvent(0, NVTEVT_EXE_CLOSE, 0);

#if (UVAC_VID_DISPLAY == ENABLE)
	UI_Show(UI_SHOW_WINDOW, TRUE);
#endif

#if !defined(_UI_STYLE_IPCAM_)
	Ux_CloseWindow((VControl *)(&UIFlowWndUSBCtrl), 0);
#endif
}

SYS_MODE gModeUsbCam = {
	"USBCAM",
	ModeUsbCam_Open,
	ModeUsbCam_Close,
	NULL,
	NULL,
	NULL,
	NULL
};
