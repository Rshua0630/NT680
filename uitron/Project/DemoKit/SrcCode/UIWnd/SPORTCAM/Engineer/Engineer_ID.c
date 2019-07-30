
#include "Engineer_ID.h"
#include "SysKer.h"
#include "EngineerMode.h"

UINT32 FLG_ID_ENGINEER = 0;

void Engineer_InstallID(void)
{
        OS_CONFIG_FLAG(FLG_ID_ENGINEER);
}
