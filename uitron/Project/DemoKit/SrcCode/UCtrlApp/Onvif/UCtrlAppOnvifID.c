#include "SysKer.h"
#include "UCtrlAppOnvifID.h"
#include "UCtrlAppOnvif.h"

UINT32 UCTRL_ONVIF_FLG_ID = 0;

void UCtrlAppOnvif_InstallID(void)
{
	OS_CONFIG_FLAG(UCTRL_ONVIF_FLG_ID);
}
