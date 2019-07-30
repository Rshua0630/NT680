#include "SysKer.h"
#include "UIDateImprint.h"
#include "UIDateImprintID.h"

UINT32 UI_DATEIMPRINT_FLG_ID = 0;

void UiDateImprint_InstallID(void)
{
	OS_CONFIG_FLAG(UI_DATEIMPRINT_FLG_ID);
}
