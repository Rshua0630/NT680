////////////////////////////////////////////////////////////////////////////////
#include "SysCommon.h"
//#include "AppCommon.h"
////////////////////////////////////////////////////////////////////////////////
#include "AppLib.h"
#include "UIMode.h"
#include "UIModeUsbCharge.h"
#include "UIAppUsbCharge.h"
//UIControl
#include "UIFrameworkExt.h"
#define UI_SHOW_CHARGER_PERIOD     5000  //ms


int PRIMARY_MODE_USBCHARGE = -1;    ///< USB CHARGE

void ModeUsbCharge_Open(void);
void ModeUsbCharge_Close(void);

void ModeUsbCharge_Open(void)
{
	Input_ResetMask();
	USB_PlugInSetFunc();
	Ux_SetActiveApp(&CustomUSBChargeObjCtrl);
	Ux_SendEvent(0, NVTEVT_EXE_OPEN, 0);
}
void ModeUsbCharge_Close(void)
{
	Ux_SendEvent(0, NVTEVT_EXE_CLOSE, 0);
}

SYS_MODE gModeUsbCharge = {
	"USBCHARGE",
	ModeUsbCharge_Open,
	ModeUsbCharge_Close,
	NULL,
	NULL,
	NULL,
	NULL
};
