#include "PrjCfg.h"

#include "SysCommon.h"
#include "SysCfg.h"
#include "AppLib.h"
#include "UIAppCommon.h"
#include "UIAppIPCamPB.h"
#include "UIAppIPCamPB_Param.h"
#include "ImageApp_IPCamPB.h"
//local debug level: THIS_DBGLVL
#define THIS_DBGLVL         2 // 0=FATAL, 1=ERR, 2=WRN, 3=UNIT, 4=FUNC, 5=IND, 6=MSG, 7=VALUE, 8=USER
///////////////////////////////////////////////////////////////////////////////
#define __MODULE__          UiAppIPCamPBCmd
#define __DBGLVL__          ((THIS_DBGLVL>=PRJ_DBG_LVL)?THIS_DBGLVL:PRJ_DBG_LVL)
#define __DBGFLT__          "*" //*=All, [mark]=CustomClass
#include "DebugModule.h"
///////////////////////////////////////////////////////////////////////////////


static BOOL IPCamPB_cmd_status_set(CHAR *strCmd){


	UINT32 status = 0;
	sscanf_s(strCmd, "%d",&status);
	DBG_DUMP("charlie1 :%d\r\n",status);
/*
	ISF_PORT_STATE_OFF      = 0x00, ///< off
	ISF_PORT_STATE_READY    = 0x01, ///< ready
	ISF_PORT_STATE_RUN      = 0x02, ///< run
	ISF_PORT_STATE_WAIT     = 0x03, ///< wait
	ISF_PORT_STATE_SLEEP    = 0x04, ///< sleep
*/

	ImageApp_IPCamPB_AllStatusSet(status);

	return TRUE;
}



SXCMD_BEGIN(ipcampb, "ipcam playback command")
SXCMD_ITEM("status %s", IPCamPB_cmd_status_set, "change status")

SXCMD_END()