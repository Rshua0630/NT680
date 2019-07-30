////////////////////////////////////////////////////////////////////////////////
#include "SysCommon.h"

////////////////////////////////////////////////////////////////////////////////
#include "AppLib.h"
#include "UIMode.h"
#include "UIModeIPCamPB.h"
#include "UIAppIPCamPB.h"

int PRIMARY_MODE_IPCAMPB = -1;      ///< IPCamPB



void ModeIPCamPB_Open(void);
void ModeIPCamPB_Close(void);

void ModeIPCamPB_Open(void)
{
	Ux_SetActiveApp(&CustomIPCamPBObjCtrl);
	Ux_SendEvent(0, NVTEVT_EXE_OPEN, 0);


}
void ModeIPCamPB_Close(void)
{

	Ux_SendEvent(0, NVTEVT_EXE_CLOSE, 0);

}

SYS_MODE gModeIPCamPB = {
	"IPCAMPB",
	ModeIPCamPB_Open,
	ModeIPCamPB_Close,
	NULL,
	NULL,
	NULL,
	NULL
};
