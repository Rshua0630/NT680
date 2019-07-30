////////////////////////////////////////////////////////////////////////////////
#include "SysCommon.h"
//#include "AppCommon.h"
////////////////////////////////////////////////////////////////////////////////
#include "AppLib.h"
#include "UIMode.h"
//UIControl
#include "UIFrameworkExt.h"
#include "DeviceCtrl.h"
#include "UIFlow.h"

int PRIMARY_MODE_USBMENU = -1;

void ModeUsbMenu_Open(void);
void ModeUsbMenu_Close(void);

void ModeUsbMenu_Open(void)
{
	Input_ResetMask();
#if !defined(_UI_STYLE_IPCAM_)
	Ux_OpenWindow(&UIMenuWndUSBCtrl, 0);
#endif
}
void ModeUsbMenu_Close(void)
{
#if !defined(_UI_STYLE_IPCAM_)
	Ux_CloseWindow(&UIMenuWndUSBCtrl, 0);
#endif
}

SYS_MODE gModeUsbMenu = {
	"USBMenu",
	ModeUsbMenu_Open,
	ModeUsbMenu_Close,
	NULL,
	NULL,
	NULL,
	NULL
};
