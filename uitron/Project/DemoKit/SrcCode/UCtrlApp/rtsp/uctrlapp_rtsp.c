#include "stdio.h"
#include "Debug.h"
#include "SxCmd.h"
#include "uToken.h"
#include "ImageStream.h"
#include "NvtUctrlAPI.h"
#include "uctrlapp_rtsp.h"
#include "PrjCfg.h"
#include "ImageUnit_VdoEnc.h"

#define THIS_DBGLVL         2 // 0=FATAL, 1=ERR, 2=WRN, 3=UNIT, 4=FUNC, 5=IND, 6=MSG, 7=VALUE, 8=USER
///////////////////////////////////////////////////////////////////////////////
#define __MODULE__          uctrlapp_rtsp
#define __DBGLVL__          ((THIS_DBGLVL>=PRJ_DBG_LVL)?THIS_DBGLVL:PRJ_DBG_LVL)
#define __DBGFLT__          "*" //*=All, [mark]=CustomClass
#include "DebugModule.h"


typedef struct _PARAM_RESET_IFRAME {
	BOOL is_set;
	UINT32  pathId;
} PARAM_RESET_IFRAME;

typedef struct _PARAM_CTRL {
	PARAM_RESET_IFRAME reset_iframe;
} PARAM_CTRL;

typedef struct _NVT_STATUS_CTRL {
	PARAM_CTRL param;
} RTSP_CTRL;

static RTSP_CTRL m_rtsp_ctrl = {0};
static RTSP_CTRL *p_ctrl = &m_rtsp_ctrl;

static INT32 param_reset_iframe(INT32 argc, char *argv[])
{
#if(IPCAM_MODE == ENABLE)
	if (argc != 1) {
		DBG_ERR("-reset_iframe [channel index]\r\n");
		return -1;
	}
	p_ctrl->param.reset_iframe.pathId = atoi(argv[0]);
	p_ctrl->param.reset_iframe.is_set = 1;
#else
	//Only IPCam can reset iframe because of no saving files
#endif
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
	char ret_str[32]={0};
	if (p_ctrl->param.reset_iframe.is_set) {
		ImageUnit_Begin(&ISF_VdoEnc, 0);
		ISF_RV er = ImageUnit_SetParam(p_ctrl->param.reset_iframe.pathId, VDOENC_PARAM_RESET_IFRAME, 1);
		ImageUnit_End();
		snprintf(ret_str, sizeof(ret_str)-1, "%d", er);
		NvtUctrl_SetRetString(ret_str);
	}

	return 0;
}

static INT32 param_unknown(char *name)
{
	DBG_ERR("unknown param: %s\r\n", name);
	return 0;
}

static UTOKEN_PARAM tbl_utoken_param[] = {
	{"reset_iframe", param_reset_iframe},
	{NULL, NULL}, //last tag, it must be
};

static UTOKEN_MODULE utoken_module = {
	"urtsp",
	tbl_utoken_param,
	param_begin,
	param_end,
	param_unknown,
	NULL
};

UTOKEN_MODULE *uctrlapp_rtsp_get_utoken_module(void)
{
	return &utoken_module;
}
