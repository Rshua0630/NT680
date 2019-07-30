#include "SysKer.h"
#include "UCtrlAppPhotoID.h"
#include "UCtrlAppPhoto.h"

UINT32 UCTRL_PHOTO_FLG_ID = 0;

void UCtrlAppPhoto_InstallID(void)
{
	OS_CONFIG_FLAG(UCTRL_PHOTO_FLG_ID);
}
