#include "MovieStampID.h"
#include "SysKer.h"
#include "MovieStampInt.h"
#include "MovieStampAPI.h"
#include "VideoSpriteIpc.h"

#define PRI_MOVIESTAMP          10
#define STKSIZE_MOVIESTAMP      4096

UINT32 MOVIESTAMPTSK_ID = 0;
UINT32 FLG_ID_MOVIESTAMP = 0;
UINT32 FTYPEIPC_SEM_ID = 0;

void MovieStamp_InstallID(void)
{
	OS_CONFIG_TASK(MOVIESTAMPTSK_ID, PRI_MOVIESTAMP, STKSIZE_MOVIESTAMP, MovieStampTsk);
	OS_CONFIG_FLAG(FLG_ID_MOVIESTAMP);
	OS_CONFIG_SEMPHORE(FTYPEIPC_SEM_ID, 0, 1, 1);
}
