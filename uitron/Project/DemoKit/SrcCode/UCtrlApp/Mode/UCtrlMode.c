#include "SysCommon.h"
#include "UIFramework.h"
#include "PrjCfg.h"
#include "UIInfo.h"
#include "AppControl.h"
#if(PHOTO_MODE==ENABLE)
#include "UIAppPhoto.h"
#endif
#include "UIControl.h"
#include "UIControlWnd.h"
#include "uToken.h"
#include "NvtUctrlAPI.h"
#include "UCtrlMain.h"
#if(IPCAMPB_MODE == ENABLE)
#include "UIModeIPCamPB.h"
#endif

#define THIS_DBGLVL         2 // 0=FATAL, 1=ERR, 2=WRN, 3=UNIT, 4=FUNC, 5=IND, 6=MSG, 7=VALUE, 8=USER
///////////////////////////////////////////////////////////////////////////////
#define __MODULE__          UCtrlMode
#define __DBGLVL__          ((THIS_DBGLVL>=PRJ_DBG_LVL)?THIS_DBGLVL:PRJ_DBG_LVL)
#define __DBGFLT__          "*" //*=All, [mark]=CustomClass
//#define __DBGFLT__          "[cap]"
#include "DebugModule.h"


#define SXCMD_TEST          1


static INT32 param_c(INT32 argc, char *argv[])
{
	char *str;
	if (argc != 1) {
		DBG_ERR("argc!=1 (%d)\r\n", argc);
		return -1;
	}
	DBG_IND("get: %s\r\n", argv[0]);
	str = argv[0];
#if (PHOTO_MODE==ENABLE)
	if (0 == strcmp(str, "photo")) {
		int iCurrMode = System_GetState(SYS_STATE_CURRMODE);
		int iNextMode = PRIMARY_MODE_PHOTO;

		DBG_DUMP("Photo Mode\r\n");
		UI_SetData(FL_PreMode, iCurrMode);
		UI_SetData(FL_NextMode, iNextMode);

#if 1
		UctrlMain_SendEvent(NVTEVT_SYSTEM_MODE, 1, iNextMode);
#else
		UctrlMain_TriggerEvent();
		Ux_PostEvent(UCTRL_EVENT_MASK | NVTEVT_SYSTEM_MODE, 1, iNextMode);
		UctrlMain_WaitEventFinish();
#endif

	} else
#endif


#if(IPCAM_MODE == ENABLE)
   if(0 == strcmp(str,"ipcam")){

			DBG_DUMP("IPCam Mode\r\n");
			UctrlMain_SendEvent(NVTEVT_SYSTEM_MODE, 1, PRIMARY_MODE_IPCAM);
			return 0;
   }
#endif
#if(IPCAMPB_MODE == ENABLE)
   if(0 == strcmp(str,"ipcampb")){

			DBG_DUMP("IPCam PlayNack Mode\r\n");
			UctrlMain_SendEvent(NVTEVT_SYSTEM_MODE, 1, PRIMARY_MODE_IPCAMPB);
			return 0;
   }
#endif


#if (MOVIE_MODE==ENABLE)
		if (0 == strcmp(str, "movie")) {
			int iCurrMode = System_GetState(SYS_STATE_CURRMODE);
			int iNextMode = PRIMARY_MODE_MOVIE;

			DBG_DUMP("Movie Mode\r\n");
			UI_SetData(FL_PreMode, iCurrMode);
			UI_SetData(FL_NextMode, iNextMode);
#if 1
			UctrlMain_SendEvent(NVTEVT_SYSTEM_MODE, 1, iNextMode);
#else
			UctrlMain_TriggerEvent();
			Ux_PostEvent(UCTRL_EVENT_MASK | NVTEVT_SYSTEM_MODE, 1, iNextMode);
			UctrlMain_WaitEventFinish();
#endif

		} else
#endif

#if (PLAY_MODE==ENABLE)
			if (0 == strcmp(str, "playback")) {
				int iCurrMode = System_GetState(SYS_STATE_CURRMODE);
				int iNextMode = PRIMARY_MODE_PLAYBACK;

				DBG_DUMP("Playback Mode\r\n");
				UI_SetData(FL_PreMode, iCurrMode);
				UI_SetData(FL_NextMode, iNextMode);
#if 1
				UctrlMain_SendEvent(NVTEVT_SYSTEM_MODE, 1, iNextMode);
#else
				UctrlMain_TriggerEvent();
				Ux_PostEvent(UCTRL_EVENT_MASK | NVTEVT_SYSTEM_MODE, 1, iNextMode);
				UctrlMain_WaitEventFinish();
#endif

			} else
#endif
#if 0
#if (IPCAM_UVC_FUNC==ENABLE)
#if defined(_USB1_HOST_)
				//for compiling check
				USB1_TYPE should be DEVICE!
#endif
				if (0 == strcmp(str, "uvc")) {
					int iCurrMode = System_GetState(SYS_STATE_CURRMODE);
					int iNextMode = PRIMARY_MODE_MOVIE;

					DBG_DUMP("uvc Mode\r\n");
					UI_SetData(FL_PreMode, iCurrMode);
					UI_SetData(FL_NextMode, iNextMode);
					UctrlMain_SendEvent(NVTEVT_SYSTEM_MODE, 2, iNextMode, SYS_SUBMODE_UVC);
				} else
#endif
#endif
					if (0 == strcmp(str, "main")) {
#if 0
						int iCurrMode = System_GetState(SYS_STATE_CURRMODE);
						int iNextMode = PRIMARY_MODE_MAIN;
#endif
						DBG_DUMP("Main Mode\r\n");
#if 0
						UI_SetData(FL_PreMode, iCurrMode);
						UI_SetData(FL_NextMode, iNextMode);
#endif
#if 1
						UctrlMain_SendEvent(NVTEVT_SYSTEM_MODE, 1, PRIMARY_MODE_MAIN);
#else
						UctrlMain_TriggerEvent();
						Ux_PostEvent(UCTRL_EVENT_MASK | NVTEVT_SYSTEM_MODE, 1, iNextMode);
						UctrlMain_WaitEventFinish();
#endif
					} else {
						return -1;
					}
	return 0;
}


static INT32 param_begin(char *str, INT32 len)
{
	DBG_IND("get: %s\r\n", str);
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
	return 0;
}

static INT32 param_unknown(char *name)
{
	DBG_ERR("unknown param: %s\r\n", name);
	return 0;
}

UTOKEN_PARAM tbl_mode_param[] = {
	{"c", param_c},
	{NULL, NULL}, //last tag, it must be
};

UTOKEN_MODULE module_mode = {
	"umode",
	tbl_mode_param,
	param_begin,
	param_end,
	param_unknown,
	NULL
};

#if SXCMD_TEST

static BOOL Cmd_umode_set(CHAR *strCmd)
{
	uToken_Parse(strCmd, &module_mode);
	return TRUE;
}

SXCMD_BEGIN(umode, "umode")
SXCMD_ITEM("set %", Cmd_umode_set, "")
SXCMD_END()

void UCtrlMode_InstallCmd(void)
{
	SxCmd_AddTable(umode);
}
#endif



