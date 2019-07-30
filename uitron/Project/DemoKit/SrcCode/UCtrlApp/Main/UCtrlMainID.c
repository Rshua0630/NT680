#include "SysKer.h"
#include "UCtrlMainID.h"
#include "UCtrlMain.h"

UINT32 UCTRL_MAIN_FLG_ID = 0;
UINT32 UCTRL_MAIN_SEM_ID = 0;

void UCtrlMain_InstallID(void)
{
	OS_CONFIG_FLAG(UCTRL_MAIN_FLG_ID);
	OS_CONFIG_SEMPHORE(UCTRL_MAIN_SEM_ID, 0, 1, 1);
}
