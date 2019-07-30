#include "AppControl.h"
#include "SysCommon.h"
#include "UIFramework.h"
#include "PrjCfg.h"
#include "uToken.h"
#include "UIInfo.h"
#include "UCtrlAppOnvif.h"
#include "UCtrlAppOnvifID.h"
#include "NvtUctrlAPI.h"
#include "NvtIpcAPI.h"
#include "UCtrlMain.h"
#include "UISetup.h"

#define THIS_DBGLVL         2 // 0=FATAL, 1=ERR, 2=WRN, 3=UNIT, 4=FUNC, 5=IND, 6=MSG, 7=VALUE, 8=USER
///////////////////////////////////////////////////////////////////////////////
#define __MODULE__          UCtrlAppOnvif
#define __DBGLVL__          ((THIS_DBGLVL>=PRJ_DBG_LVL)?THIS_DBGLVL:PRJ_DBG_LVL)
#define __DBGFLT__          "*" //*=All, [mark]=CustomClass
#include "DebugModule.h"


#define SXCMD_TEST          0


// flag define
#define FLG_UCTRL_ONVIF_START           FLGPTN_BIT(0)
#define FLG_UCTRL_ONVIF_EXIT            FLGPTN_BIT(1)


typedef struct _UCtrlOnvifParmType {
	UINT32 isSet;
} UCtrlOnvifParmType;


typedef struct _UCtrlOnvifInfoType {
	BOOL               isErr;
	UCtrlOnvifParmType  start;
	UCtrlOnvifParmType  exit;
	int               paramcnt;
} UCtrlOnvifInfoType;

static UCtrlOnvifInfoType   onvifInfo;
static UCtrlOnvifInfoType  *pOnvifInfo = &onvifInfo;

static INT32 param_start(INT32 argc, char *argv[])
{
	pOnvifInfo->start.isSet = 1;
	pOnvifInfo->paramcnt++;
	return 0;
}

static INT32 param_exit(INT32 argc, char *argv[])
{
	pOnvifInfo->exit.isSet = 1;
	pOnvifInfo->paramcnt++;
	return 0;
}


static INT32 param_begin(char *str, INT32 len)
{
	DBG_IND("get: %s\r\n", str);
	memset(pOnvifInfo, 0, sizeof(UCtrlOnvifInfoType));
	return 0;
}

static INT32 param_end(UTOKEN_END_INFO *p_info)
{
	char retStr[64];

	if (p_info->err_code < 0) {
		snprintf(retStr, sizeof(retStr), "ERR: parm %s", p_info->failed_name);
		NvtUctrl_SetRetString(retStr);
		DBG_ERR("%s\r\n", retStr);
		return p_info->err_code;
	}
	if (pOnvifInfo->start.isSet) {
		set_flg(UCTRL_ONVIF_FLG_ID, FLG_UCTRL_ONVIF_START);
	}
	if (pOnvifInfo->exit.isSet) {
		set_flg(UCTRL_ONVIF_FLG_ID, FLG_UCTRL_ONVIF_EXIT);
	}
	return 0;
}

static INT32 param_unknown(char *name)
{
	DBG_ERR("unknown param: %s\r\n", name);
	return 0;
}

UTOKEN_PARAM tbl_onvif_param[] = {
	{"start", param_start},
	{"exit", param_exit},
	{NULL, NULL}, //last tag, it must be
};

UTOKEN_MODULE module_onvif = {
	"uonvif",
	tbl_onvif_param,
	param_begin,
	param_end,
	param_unknown,
	NULL
};


void UCtrlAppOnvif_Open(void)
{
#define        CMD_LEN 128
	CHAR           szCmd[CMD_LEN + 1];
	NVTIPC_SYS_MSG SysMsg;
	NVTIPC_I32 Ret_NvtIpc;


	clr_flg(UCTRL_ONVIF_FLG_ID, 0xFFFFFFFF);

	//sprintf(szCmd,"DeviceDiscovery -d");
	snprintf(szCmd, CMD_LEN, "DeviceDiscovery -d");
#if 0
	if (SysGetFlag(FL_CPU2_DEFAULT)) {
		SysSetFlag(FL_CPU2_DEFAULT, 0);
		strncat(szCmd, " -r", CMD_LEN); // set -r to reset onvif default value
	}
#endif
	strncat(szCmd, " &", CMD_LEN);
	SysMsg.sysCmdID = NVTIPC_SYSCMD_SYSCALL_REQ;
	SysMsg.DataAddr = (NVTIPC_U32)szCmd;
	SysMsg.DataSize = (NVTIPC_U32)strlen(szCmd) + 1;

	Ret_NvtIpc = NvtIPC_MsgSnd(NVTIPC_SYS_QUEUE_ID, NVTIPC_SENDTO_CORE2, &SysMsg, sizeof(SysMsg));
	if (Ret_NvtIpc < 0) {
		DBG_ERR("NvtIPC_MsgSnd(SYSCALL_REQ) = %d\r\n", Ret_NvtIpc);
	}
#if SXCMD_TEST
	UCtrlAppOnvif_InstallCmd();
#endif
}

void UCtrlAppOnvif_Close(void)
{
	FLGPTN     uiFlag;

	if (NvtUctrl_Notify("exit") == E_OK) {
		if (kchk_flg(UCTRL_ONVIF_FLG_ID, FLG_UCTRL_ONVIF_START)) {
			wai_flg(&uiFlag, UCTRL_ONVIF_FLG_ID, FLG_UCTRL_ONVIF_EXIT, TWF_ORW);
		}
	}
}

#if SXCMD_TEST

static BOOL Cmd_uonvif_set(CHAR *strCmd)
{
	uToken_Parse(strCmd, &module_onvif);
	return TRUE;
}

static BOOL Cmd_uonvif_event(CHAR *strCmd)
{
	CHAR  EventName[32] = {0};

	sscanf(strCmd, "%s", EventName + snprintf(EventName, sizeof(EventName), "event "));
	//DBG_DUMP("EventName=%s\r\n",EventName);
	NvtUctrl_Notify(EventName);
	return TRUE;
}

SXCMD_BEGIN(uonvif, "uonvif")
SXCMD_ITEM("set %", Cmd_uonvif_set, "")
SXCMD_ITEM("event %", Cmd_uonvif_event, "")
SXCMD_END()
#endif


void UCtrlAppOnvif_InstallCmd(void)
{
#if SXCMD_TEST
	SxCmd_AddTable(uonvif);
#endif
}


