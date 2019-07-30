#include "SysKer.h"
#include "UserCmd.h"
#include "UserCmdID.h"
#include "UserCmdInt.h"

#define PRI_USERCMD             11 // 10
#define STKSIZE_USERCMD         8192

UINT32 FLG_ID_USERCMD = 0;
UINT32 USERCMD_TSK_ID = 0;

void UserCmd_InstallID(void)
{
	OS_CONFIG_FLAG(FLG_ID_USERCMD);
	OS_CONFIG_TASK(USERCMD_TSK_ID, PRI_USERCMD, STKSIZE_USERCMD, UserCmdTsk);
}
