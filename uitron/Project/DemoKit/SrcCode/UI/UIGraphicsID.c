#include "SysKer.h"
#include "UIGraphics.h"

UINT32 UICTRL_GFX_SEM_ID = 0;

void UI_InstallID(void)
{
	OS_CONFIG_SEMPHORE(UICTRL_GFX_SEM_ID, 0, 1, 1);
}


