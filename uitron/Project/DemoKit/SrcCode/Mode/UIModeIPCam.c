////////////////////////////////////////////////////////////////////////////////
#include "SysCommon.h"

////////////////////////////////////////////////////////////////////////////////
#include "AppLib.h"
#include "UIMode.h"
#include "UIModeIPCam.h"
#include "UIAppIPCam.h"

int PRIMARY_MODE_IPCAM = -1;      ///< IPCam



void ModeIPCam_Open(void);
void ModeIPCam_Close(void);

void ModeIPCam_Open(void)
{
	Ux_SetActiveApp(&CustomIPCamObjCtrl);
	Ux_SendEvent(0, NVTEVT_EXE_OPEN, 0);


}
void ModeIPCam_Close(void)
{

	Ux_SendEvent(0, NVTEVT_EXE_CLOSE, 0);

}

SYS_MODE gModeIPCam = {
	"IPCAM",
	ModeIPCam_Open,
	ModeIPCam_Close,
	NULL,
	NULL,
	NULL,
	NULL
};
