#include "SysKer.h"
#include "WifiCmdParser.h"
#include "WifiCmdParserInt.h"

UINT32 FLG_ID_WIFICMD = 0;
UINT32 WIFICMD_SEM_ID = 0;
UINT32 WIFISTR_SEM_ID = 0;

void WifiCmd_InstallID(void)
{
	OS_CONFIG_FLAG(FLG_ID_WIFICMD);
	OS_CONFIG_SEMPHORE(WIFICMD_SEM_ID, 0, 1, 1);
	OS_CONFIG_SEMPHORE(WIFISTR_SEM_ID, 0, 1, 1);
}

