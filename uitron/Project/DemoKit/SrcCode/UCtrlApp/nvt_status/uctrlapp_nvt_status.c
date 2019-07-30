#include "Debug.h"
#include "SxCmd.h"
#include "uToken.h"
#include "NvtUctrlAPI.h"
#include "PrjCfg.h"
#include "uctrlapp_nvt_status.h"
#include "nvtmpp.h"
#include <stdio.h>
#include "NvtIpcAPI.h"
#include "vdodisp_api.h"
#include "ipl_debug_infor.h"
#include "photo_task.h"
#include "sensor.h"
#if (CALIBRATION_FUNC == ENABLE)
#include "ipl_calibration.h"
#endif
#define THIS_DBGLVL         2 // 0=FATAL, 1=ERR, 2=WRN, 3=UNIT, 4=FUNC, 5=IND, 6=MSG, 7=VALUE, 8=USER
///////////////////////////////////////////////////////////////////////////////
#define __MODULE__          nvt_status
#define __DBGLVL__          ((THIS_DBGLVL>=PRJ_DBG_LVL)?THIS_DBGLVL:PRJ_DBG_LVL)
#define __DBGFLT__          "*" //*=All, [mark]=CustomClass
#include "DebugModule.h"

#define SXCMD_TEST          1

typedef void (*DUMP_FP)(void (*dump)(char *fmt, ...));

typedef struct _NVT_STATE_MODULE {
	char *name;
	DUMP_FP fp;
} NVT_STATE_MODULE;

//static void dummy0(void (*dump)(char *fmt, ...));
//static void dummy1(void (*dump)(char *fmt, ...));
static NVT_STATE_MODULE state_module[] = {
	//{"dummy0", dummy0},
	//{"dummy1", dummy1},
	{"vdodisp", vdodisp_dump_info},
	{"nvtmpp",  nvtmpp_dump_status},
	{"nvtipc",  NvtIPC_DumpDebugInfo},
	{"nvtipl", ipl_dump_status},
	{"nvtphototsk", Photo_DumpInfo},
	{"nvtsensor", sensor_dump_info},
#if (CALIBRATION_FUNC == ENABLE)
	{"nvtcal", cal_dump_all},
#endif
};

typedef struct _PARAM_GET_CNT {
	BOOL is_set;
} PARAM_GET_CNT;

typedef struct _PARAM_GET_NAME {
	BOOL is_set;
	UINT32 index;
} PARAM_GET_NAME;

typedef struct _PARAM_GET_BUF {
	BOOL is_set;
	UINT32 index;
} PARAM_GET_BUF;

typedef struct _PARAM_GET_DUMP {
	BOOL is_set;
	int  index; // index for state_module
} PARAM_GET_DUMP;

typedef struct _PARAM_CTRL {
	PARAM_GET_CNT get_cnt;
	PARAM_GET_NAME get_name;
	PARAM_GET_BUF get_buf;
	PARAM_GET_DUMP get_dump;
} PARAM_CTRL;

typedef struct _NVT_STATUS_CTRL {
	UINT32 work_addr;
	UINT32 work_size;
	UINT32 curr_addr;
	PARAM_CTRL param;
} NVT_STATUS_CTRL;

static NVT_STATUS_CTRL nvt_status_ctrl = {0};
static NVT_STATUS_CTRL *p_ctrl = &nvt_status_ctrl;

static void dump(char *fmtstr, ...)
{
	UINT32 work_end = p_ctrl->work_addr + p_ctrl->work_size;
	int remain_size = 0;
	va_list marker;
	//coverity[returned_null]
	//coverity[var_assigned]
	va_start(marker, fmtstr);

	if (p_ctrl->curr_addr < work_end) {
		remain_size = work_end - p_ctrl->curr_addr;
	}
	//coverity[dereference]
	vsnprintf((char *)p_ctrl->curr_addr, remain_size, fmtstr, marker);
	va_end(marker);

	p_ctrl->curr_addr += strlen((char *)p_ctrl->curr_addr);
}

#if 0
static void dummy0(void (*dump)(char *fmt, ...))
{
	int i;
	for (i = 0; i < 10; i++) {
		dump("dummy0[%d]\r\n", i);
	}
}
static void dummy1(void (*dump)(char *fmt, ...))
{
	int i;
	for (i = 0; i < 10; i++) {
		dump("dummy1[%d]\r\n", i);
	}
}
#endif

static INT32 param_get_cnt(INT32 argc, char *argv[])
{
	p_ctrl->param.get_cnt.is_set = 1;
	return 0;
}

static INT32 param_get_name(INT32 argc, char *argv[])
{
	int index = atoi(argv[0]);
	int cnt = (int)(sizeof(state_module) / sizeof(state_module[0]));
	if (index >= cnt) {
		DBG_ERR("index larger than count %d > %d \r\n", index, cnt);
		return -1;
	}
	p_ctrl->param.get_name.is_set = 1;
	p_ctrl->param.get_name.index = index;
	return 0;
}

static INT32 param_get_buf(INT32 argc, char *argv[])
{
	p_ctrl->param.get_buf.is_set = 1;
	return 0;
}

static INT32 param_get_dump(INT32 argc, char *argv[])
{
	if (argc != 1) {
		DBG_ERR("-dump [module index]\r\n");
		return -1;
	}
	if (atoi(argv[0]) >= (int)(sizeof(state_module) / sizeof(state_module[0]))) {
		DBG_ERR("index: %d\r\n", atoi(argv[0]));
		return -2;
	}

	p_ctrl->param.get_dump.is_set = 1;
	p_ctrl->param.get_dump.index = atoi(argv[0]);
	return 0;
}

static INT32 param_begin(char *str, INT32 len)
{
	DBG_IND("get: %s\r\n", str);
	memset(&p_ctrl->param, 0, sizeof(PARAM_CTRL));
	return 0;
}

static INT32 param_end(UTOKEN_END_INFO *p_info)
{
	char retStr[128];
	if (p_info->err_code < 0) {
		snprintf(retStr, sizeof(retStr), "ERR: parm %s", p_info->failed_name);
		NvtUctrl_SetRetString(retStr);
		DBG_ERR("%s", retStr);
		return p_info->err_code;
	}

	if (p_ctrl->param.get_cnt.is_set) {
		int cnt = (int)(sizeof(state_module) / sizeof(state_module[0]));
		snprintf(retStr, sizeof(retStr), "%d", cnt);
		NvtUctrl_SetRetString(retStr);
	}

	if (p_ctrl->param.get_name.is_set) {
		snprintf(retStr, sizeof(retStr), "%s", state_module[p_ctrl->param.get_name.index]);
		NvtUctrl_SetRetString(retStr);
	}

	if (p_ctrl->param.get_buf.is_set) {
		snprintf(retStr, sizeof(retStr), "%08X %08X", NvtIPC_GetPhyAddr(p_ctrl->work_addr), p_ctrl->work_size);
		NvtUctrl_SetRetString(retStr);
	}

	if (p_ctrl->param.get_dump.is_set) {
		p_ctrl->curr_addr = p_ctrl->work_addr;
		state_module[p_ctrl->param.get_dump.index].fp(dump);
		snprintf(retStr, sizeof(retStr), "%08X %08X", NvtIPC_GetPhyAddr(p_ctrl->work_addr), p_ctrl->work_size);
		NvtUctrl_SetRetString(retStr);
		//DON'T use DBG_DUMP or debug_msg, because it cuaue some long string was cut.
		debug_putstring((char *)p_ctrl->work_addr);
	}

	return 0;
}

static INT32 param_unknown(char *name)
{
	DBG_ERR("unknown param: %s\r\n", name);
	return 0;
}

static UTOKEN_PARAM tbl_utoken_param[] = {
	{"get_cnt", param_get_cnt},
	{"get_name", param_get_name},
	{"get_buf", param_get_buf},
	{"get_dump", param_get_dump},
	{NULL, NULL}, //last tag, it must be
};

static UTOKEN_MODULE utoken_module = {
	"unvt_status",
	tbl_utoken_param,
	param_begin,
	param_end,
	param_unknown,
	NULL
};

#if SXCMD_TEST

static BOOL sxcmd_uctrl(CHAR *strCmd)
{
	uToken_Parse(strCmd, &utoken_module);
	return TRUE;
}

SXCMD_BEGIN(nvt_status, "nvt_status")
SXCMD_ITEM("uctrl %", sxcmd_uctrl, "")
SXCMD_END()

#endif

BOOL uctrlapp_nvt_status_init(UINT32 work_addr, UINT32 work_size)
{
	// need 4K page align for linux
	p_ctrl->work_addr = ALIGN_CEIL(work_addr, 0x1000);
	p_ctrl->work_size = ALIGN_FLOOR(work_size - (p_ctrl->work_addr - work_addr), 0x1000);

	DBG_IND("align: addr 0x%08X -> 0x%08X\r\n", work_addr, p_ctrl->work_addr);
	DBG_IND("align: size 0x%08X -> 0x%08X\r\n", work_size, p_ctrl->work_size);
	uctrlapp_nvt_status_installcmd();
	return TRUE;
}

UTOKEN_MODULE *uctrlapp_nvt_status_get_utoken_module(void)
{
	return &utoken_module;
}

//#NT#2018/05/15#YongChang Qui -begin
//#NT#Add ramoops to print linux panic message after watchdog reboot
static INT32 ramoops_begin(char *str, INT32 len)
{
	return 0;
}

static INT32 ramoops_end(UTOKEN_END_INFO *p_info)
{
	return 0;
}

static INT32 ramoops_print(INT32 argc, char *argv[])
{
	static char msg[4096];
	NvtUctrl_GetConfigData(msg, 4096);
	DBG_ERR(":%s\r\n", msg);

	return 0;
}

UTOKEN_PARAM ramoops_param[] = {
	{"print", ramoops_print},
};

static UTOKEN_MODULE ramoops_module = {
	"ramoops",
	ramoops_param,
	ramoops_begin,
	ramoops_end,
	param_unknown,
	NULL
};

UTOKEN_MODULE *ramoops_get_utoken_module(void)
{
	return &ramoops_module;
}
//#NT#2018/05/15#YongChang Qui -end

void uctrlapp_nvt_status_installcmd(void)
{
#if SXCMD_TEST
	SxCmd_AddTable(nvt_status);
#endif
}
