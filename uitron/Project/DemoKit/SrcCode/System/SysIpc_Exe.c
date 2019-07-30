////////////////////////////////////////////////////////////////////////////////
#include "SysCommon.h"
#include "AppCommon.h"
#include "NvtIpcAPI.h"
#include "SxCmd.h"
////////////////////////////////////////////////////////////////////////////////

//global debug level: PRJ_DBG_LVL
#include "PrjCfg.h"
#include "SysCfg.h"
//local debug level: THIS_DBGLVL
#define THIS_DBGLVL         2 // 0=FATAL, 1=ERR, 2=WRN, 3=UNIT, 4=FUNC, 5=IND, 6=MSG, 7=VALUE, 8=USER
///////////////////////////////////////////////////////////////////////////////
#define __MODULE__          SysIPCExe
#define __DBGLVL__          ((THIS_DBGLVL>=PRJ_DBG_LVL)?THIS_DBGLVL:PRJ_DBG_LVL)
#define __DBGFLT__          "*" //*=All, [mark]=CustomClass
#include "DebugModule.h"
///////////////////////////////////////////////////////////////////////////////

#if defined(_CPU2_ECOS_)
static BOOL CPU2_Cmd(CHAR *pStr)
{
	NVTIPC_SYS_MSG sysMsg;
	sysMsg.sysCmdID = NVTIPC_SYSCMD_UART_REQ;
	sysMsg.DataAddr = (UINT32)pStr;
	sysMsg.DataSize = strlen(pStr) + 1;

	if (NvtIPC_MsgSnd(NVTIPC_SYS_QUEUE_ID, NVTIPC_SENDTO_CORE2, &sysMsg, sizeof(sysMsg)) < 0) {
		DBG_ERR("NvtIPC_MsgSnd\r\n");
	}
	return TRUE;
}
#endif

#if defined(_CPU2_LINUX_)
static BOOL CPU2_Cmd(CHAR *pStr)
{
	static char cmd[256] = {0};
	NVTIPC_SYS_MSG sysMsg;

	strncpy(cmd, pStr, sizeof(cmd)-1);
	strncat(cmd, " &", sizeof(cmd)-1);
	sysMsg.sysCmdID = NVTIPC_SYSCMD_SYSCALL_REQ;
	sysMsg.DataAddr = (UINT32)cmd;
	sysMsg.DataSize = strlen(cmd) + 1;
	if (NvtIPC_MsgSnd(NVTIPC_SYS_QUEUE_ID, NVTIPC_SENDTO_CORE2, &sysMsg, sizeof(sysMsg)) < 0) {
		DBG_ERR("NvtIPC_MsgSnd\r\n");
	}
	return TRUE;
}
#endif

#if defined(_DSP1_FREERTOS_)
BOOL DSP1_Cmd(CHAR *pStr)
{
	NVTIPC_SYS_MSG sysMsg;

	sysMsg.sysCmdID = NVTIPC_SYSCMD_UART_REQ;
	sysMsg.DataAddr = (UINT32)pStr;
	sysMsg.DataSize = strlen(pStr) + 1;
	if (NvtIPC_MsgSnd(NVTIPC_SYS_QUEUE_ID, NVTIPC_SENDTO_CORE3, &sysMsg, sizeof(sysMsg)) < 0) {
		DBG_ERR("NvtIPC_MsgSnd\r\n");
	}
	return TRUE;
}
#endif
#if defined(_DSP2_FREERTOS_)
BOOL DSP2_Cmd(CHAR *pStr)
{
	NVTIPC_SYS_MSG sysMsg;

	sysMsg.sysCmdID = NVTIPC_SYSCMD_UART_REQ;
	sysMsg.DataAddr = (UINT32)pStr;
	sysMsg.DataSize = strlen(pStr) + 1;
	if (NvtIPC_MsgSnd(NVTIPC_SYS_QUEUE_ID, NVTIPC_SENDTO_CORE4, &sysMsg, sizeof(sysMsg)) < 0) {
		DBG_ERR("NvtIPC_MsgSnd\r\n");
	}
	return TRUE;
}
#endif

#ifdef __ECOS
#include <cyg/infra/maincmd.h>
static BOOL eCos_Cmd(CHAR *pStr)
{
	if (pStr[0] == '?') {
		listmaincmd();
	} else {
		runmaincmd(pStr);
	}
	return TRUE;
}
#endif

static void system_poweroff(void)
{
	DBG_IND("system_poweroff\r\n");
	Ux_PostEvent(NVTEVT_SYSTEM_SHUTDOWN, 1, 0);
}

static void system_reboot(void)
{
	DBG_IND("system_reboot\r\n");
#if defined(_CLOCK_RTC_)
	GxSystem_EnableHWReset(0);
#elif defined(_CLOCK_DRTC_)
	GxSystem_EnableSWReset(0); //it may cause damage on peripheral, cause their power are not be reset here.
	DBG_WRN("Using SW Reset, it might be not safe for peripheral.\r\n");
#else
	DBG_FATAL("unknown CLOCK setting?\r\n");
#endif
	Ux_PostEvent(NVTEVT_SYSTEM_SHUTDOWN, 1, 0);
}


void System_OnIPC(void)
{
	NVTIPC_OPEN_OBJ   openObj = {0};
	TM_BOOT_BEGIN("ipc", "init");

	openObj.workBuffAddr = OS_GetMempoolAddr(POOL_ID_NVTIPC);
	openObj.workBuffSize = OS_GetMempoolSize(POOL_ID_NVTIPC);
	openObj.SysCmdTbl = NULL;
	openObj.uartCmdFp = (UART_CMD_FP)SxCmd_DoCommand;
	openObj.poweroffFp = system_poweroff;
	openObj.rebootFp = system_reboot;
#if defined(_CPU2_LINUX_)
	openObj.bIsSupportCoresSem = TRUE;
#endif
	NvtIPC_Open(&openObj);

#if defined(_CPU2_ECOS_) || defined(_CPU2_LINUX_)
	SxCmd_RegCmdMode(SX_MODE_CPU2, "CPU2", '#', CPU2_Cmd);
#endif
#if defined(_DSP1_FREERTOS_)
	SxCmd_RegCmdMode(SX_MODE_DSP1, "DSP1", '@', DSP1_Cmd);
#endif
#if defined(_DSP2_FREERTOS_)
	SxCmd_RegCmdMode(SX_MODE_DSP2, "DSP2", '&', DSP2_Cmd);
#endif
#ifdef __ECOS
	SxCmd_RegCmdMode(SX_MODE_EXAM, "ECOS1", '$', eCos_Cmd);
#endif
	TM_BOOT_END("ipc", "init");
}
