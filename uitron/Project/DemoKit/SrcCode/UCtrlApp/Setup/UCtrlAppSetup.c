#include "SysCommon.h"
#include "UIFramework.h"
#include "PrjCfg.h"
#include "uToken.h"
#include "UIInfo.h"
#include "UISetup.h"
#include "UIControl.h"
#include "UIControlWnd.h"
#include "UCtrlAppSetup.h"
#include "NvtUctrlAPI.h"
#include "UCtrlMain.h"
#include "HwClock.h"


#define THIS_DBGLVL         2 // 0=FATAL, 1=ERR, 2=WRN, 3=UNIT, 4=FUNC, 5=IND, 6=MSG, 7=VALUE, 8=USER
///////////////////////////////////////////////////////////////////////////////
#define __MODULE__          UCtrlAppSetup
#define __DBGLVL__          ((THIS_DBGLVL>=PRJ_DBG_LVL)?THIS_DBGLVL:PRJ_DBG_LVL)
#define __DBGFLT__          "*" //*=All, [mark]=CustomClass
#include "DebugModule.h"


#define SXCMD_TEST          0

/*
    {NVTEVT_EXE_LCDOFF,              SetupExe_OnLCDOff            },
    {NVTEVT_EXE_POWEROFF,            SetupExe_OnPowerOff          },
    {NVTEVT_EXE_BEEPVOLUME,          SetupExe_OnBeepVolume        },
    {NVTEVT_EXE_TVFORMAT,            SetupExe_OnTVFormat          },
    {NVTEVT_EXE_HDMIFORMAT,          SetupExe_OnHDMIFormat        },
    {NVTEVT_EXE_BEEPKEY,             SetupExe_OnBeepKey           },
    {NVTEVT_EXE_LANGUAGE,            SetupExe_OnLanguage          },
    {NVTEVT_EXE_FREQ,                SetupExe_OnFreq              },
*/

typedef struct _UCtrlSetupParmType {
	UINT32 isSet;
	UINT32 index;
} UCtrlSetupParmType;

typedef struct _UCtrlSetupDateParmType {
	UINT32 isSet;
	UINT32 uiYear;
	UINT32 uiMonth;
	UINT32 uiDay;
} UCtrlSetupDateParmType;

typedef struct _UCtrlSetupTimeParmType {
	UINT32 isSet;
	UINT32 uiHour;
	UINT32 uiMin;
	UINT32 uiSec;
} UCtrlSetupTimeParmType;

typedef struct _UCtrlSetupTimeOffsetParmType {
	UINT32 isSet;
	INT32  iHour;
	INT32  iMin;
} UCtrlSetupTimeOffsetParmType;

typedef struct _UCtrlSetupInfoType {
	BOOL                    isErr;
	UCtrlSetupParmType      LCDoff;
	UCtrlSetupParmType      poweroff;
	UCtrlSetupParmType      beepvolume;
	UCtrlSetupParmType      TVformat;
	UCtrlSetupParmType      HDMIformat;
	UCtrlSetupParmType      beepkey;
	UCtrlSetupParmType      language;
	UCtrlSetupParmType      freq;
	UCtrlSetupDateParmType  date;
	UCtrlSetupTimeParmType  time;
	UCtrlSetupTimeOffsetParmType timeoffset;
} UCtrlSetupInfoType;

static UCtrlSetupInfoType   setupInfo;
static UCtrlSetupInfoType  *pSetupInfo = &setupInfo;

static INT32 param_LCDoff(INT32 argc, char *argv[])
{
	UINT32 value;
	if (argc != 1) {
		DBG_ERR("argc!=1 (%d)\r\n", argc);
		return -1;
	}
	DBG_IND("get: %d\r\n", atoi(argv[0]));
	value = atoi(argv[0]);
	if (value >= LCDOFF_SETTING_MAX) {
		DBG_ERR("value =%d\r\n", value);
		return -1;
	}
	pSetupInfo->LCDoff.index = value;
	pSetupInfo->LCDoff.isSet = 1;
	return 0;
}
static INT32 param_poweroff(INT32 argc, char *argv[])
{
	UINT32 value;
	if (argc != 1) {
		DBG_ERR("argc!=1 (%d)\r\n", argc);
		return -1;
	}
	DBG_IND("get: %d\r\n", atoi(argv[0]));
	value = atoi(argv[0]);
	if (value >= POWEROFF_SETTING_MAX) {
		DBG_ERR("value =%d\r\n", value);
		return -1;
	}
	pSetupInfo->poweroff.index = value;
	pSetupInfo->poweroff.isSet = 1;
	return 0;
}

static INT32 param_beepvolume(INT32 argc, char *argv[])
{
	UINT32 value;
	if (argc != 1) {
		DBG_ERR("argc!=1 (%d)\r\n", argc);
		return -1;
	}
	DBG_IND("get: %d\r\n", atoi(argv[0]));
	value = atoi(argv[0]);
	if (value >= BEEP_VOL_SETTING_MAX) {
		DBG_ERR("value =%d\r\n", value);
		return -1;
	}
	pSetupInfo->beepvolume.index = value;
	pSetupInfo->beepvolume.isSet = 1;
	return 0;
}

static INT32 param_TVformat(INT32 argc, char *argv[])
{
	UINT32 value;
	if (argc != 1) {
		DBG_ERR("argc!=1 (%d)\r\n", argc);
		return -1;
	}
	DBG_IND("get: %d\r\n", atoi(argv[0]));
	value = atoi(argv[0]);
	if (value >= TV_MODE_ID_MAX) {
		DBG_ERR("value =%d\r\n", value);
		return -1;
	}
	pSetupInfo->TVformat.index = value;
	pSetupInfo->TVformat.isSet = 1;
	return 0;
}

static INT32 param_HDMIformat(INT32 argc, char *argv[])
{
	UINT32 value;
	if (argc != 1) {
		DBG_ERR("argc!=1 (%d)\r\n", argc);
		return -1;
	}
	DBG_IND("get: %d\r\n", atoi(argv[0]));
	value = atoi(argv[0]);
	if (value >= VIDEOOUT2_SETTING_MAX) {
		DBG_ERR("value =%d\r\n", value);
		return -1;
	}
	pSetupInfo->HDMIformat.index = value;
	pSetupInfo->HDMIformat.isSet = 1;
	return 0;
}

static INT32 param_beepkey(INT32 argc, char *argv[])
{
	UINT32 value;
	if (argc != 1) {
		DBG_ERR("argc!=1 (%d)\r\n", argc);
		return -1;
	}

	DBG_IND("get: %d\r\n", atoi(argv[0]));
	value = atoi(argv[0]);
	if (value > ON) {
		DBG_ERR("value =%d\r\n", value);
		return -1;
	}
	pSetupInfo->beepkey.index = value;
	pSetupInfo->beepkey.isSet = 1;
	return 0;
}

static INT32 param_language(INT32 argc, char *argv[])
{
	UINT32 value;
	if (argc != 1) {
		DBG_ERR("argc!=1 (%d)\r\n", argc);
		return -1;
	}
	DBG_IND("get: %d\r\n", atoi(argv[0]));
	value = atoi(argv[0]);
	if (value >= LANG_ID_MAX) {
		DBG_ERR("value =%d\r\n", value);
		return -1;
	}
	pSetupInfo->language.index = value;
	pSetupInfo->language.isSet = 1;
	return 0;
}

static INT32 param_freq(INT32 argc, char *argv[])
{
	UINT32 value;
	if (argc != 1) {
		DBG_ERR("argc!=1 (%d)\r\n", argc);
		return -1;
	}
	DBG_IND("get: %d\r\n", atoi(argv[0]));
	value = atoi(argv[0]);
	if (value >= FREQUENCY_ID_MAX) {
		DBG_ERR("value =%d\r\n", value);
		return -1;
	}
	pSetupInfo->freq.index = value;
	pSetupInfo->freq.isSet = 1;
	return 0;
}

static INT32 param_date(INT32 argc, char *argv[])
{
	UINT32 uiYear, uiMonth, uiDay;

	if (argc != 1) {
		DBG_ERR("argc!=1 (%d)\r\n", argc);
		return -1;
	}
	DBG_IND("get: %s\r\n", argv[0]);
	if (3 == sscanf_s(argv[0], "%d-%d-%d", &uiYear, &uiMonth, &uiDay)) {
		pSetupInfo->date.uiYear = uiYear;
		pSetupInfo->date.uiMonth = uiMonth;
		pSetupInfo->date.uiDay = uiDay;
		pSetupInfo->date.isSet = 1;
	} else {
		DBG_ERR("param err\r\n");
		return -1;
	}
	return 0;
}

static INT32 param_time(INT32 argc, char *argv[])
{
	UINT32 uiHour, uiMin, uiSec;

	if (argc != 1) {
		DBG_ERR("argc!=1 (%d)\r\n", argc);
		return -1;
	}
	DBG_IND("get: %s\r\n", argv[0]);
	if (3 == sscanf_s(argv[0], "%d:%d:%d", &uiHour, &uiMin, &uiSec)) {
		pSetupInfo->time.uiHour = uiHour;
		pSetupInfo->time.uiMin = uiMin;
		pSetupInfo->time.uiSec = uiSec;
		pSetupInfo->time.isSet = 1;

	} else {
		DBG_ERR("param err\r\n");
		return -1;
	}
	return 0;
}

static INT32 param_timeoffset(INT32 argc, char *argv[])
{
	INT32 iHour, iMin;

	if (argc != 1) {
		DBG_ERR("argc!=1 (%d)\r\n", argc);
		return -1;
	}
	DBG_IND("get: %s\r\n", argv[0]);

	if (2 == sscanf_s(argv[0], "%d:%d", &iHour, &iMin)) {
		pSetupInfo->timeoffset.iHour = iHour;
		if (iHour < 0) {
			pSetupInfo->timeoffset.iMin = -iMin;
		} else {
			pSetupInfo->timeoffset.iMin = iMin;
		}

		pSetupInfo->timeoffset.isSet = 1;
	} else {
		DBG_ERR("param err\r\n");
		return -1;
	}

	return 0;
}

static INT32 param_begin(char *str, INT32 len)
{
	DBG_IND("get: %s\r\n", str);
	memset(pSetupInfo, 0, sizeof(UCtrlSetupInfoType));
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

	if (pSetupInfo->LCDoff.isSet) {
		UctrlMain_SendEvent(NVTEVT_EXE_LCDOFF,             1,  pSetupInfo->LCDoff.index);
	}
	if (pSetupInfo->poweroff.isSet) {
		UctrlMain_SendEvent(NVTEVT_EXE_POWEROFF,           1,  pSetupInfo->poweroff.index);
	}
	if (pSetupInfo->beepvolume.isSet) {
		UctrlMain_SendEvent(NVTEVT_EXE_BEEPVOLUME,         1,  pSetupInfo->beepvolume.index);
	}
	if (pSetupInfo->TVformat.isSet) {
		UctrlMain_SendEvent(NVTEVT_EXE_TVFORMAT,           1,  pSetupInfo->TVformat.index);
	}
	if (pSetupInfo->HDMIformat.isSet) {
		UctrlMain_SendEvent(NVTEVT_EXE_HDMIFORMAT,         1,  pSetupInfo->HDMIformat.index);
	}
	if (pSetupInfo->beepkey.isSet) {
		UctrlMain_SendEvent(NVTEVT_EXE_BEEPKEY,            1,  pSetupInfo->beepkey.index);
	}
	if (pSetupInfo->language.isSet) {
		UctrlMain_SendEvent(NVTEVT_EXE_LANGUAGE,           1,  pSetupInfo->language.index);
	}
	if (pSetupInfo->freq.isSet) {
		UctrlMain_SendEvent(NVTEVT_EXE_FREQ,               1,  pSetupInfo->freq.index);
	}
	if (pSetupInfo->date.isSet) {
		UctrlMain_SendEvent(NVTEVT_EXE_DATE,               3,  pSetupInfo->date.uiYear, pSetupInfo->date.uiMonth, pSetupInfo->date.uiDay);
	}
	if (pSetupInfo->time.isSet) {
		UctrlMain_SendEvent(NVTEVT_EXE_TIME,               3,  pSetupInfo->time.uiHour, pSetupInfo->time.uiMin, pSetupInfo->time.uiSec);
	}
	if (pSetupInfo->timeoffset.isSet) {
        HwClock_SetParam(HWCLOCK_PARAM_ID_READ_TIME_OFFSET, pSetupInfo->timeoffset.iHour, pSetupInfo->timeoffset.iMin);
	}
	return 0;
}

static INT32 param_unknown(char *name)
{
	DBG_ERR("unknown param: %s\r\n", name);
	return 0;
}

UTOKEN_PARAM tbl_setup_param[] = {
	{"LCDoff", param_LCDoff},
	{"poweroff", param_poweroff},
	{"beepvolume", param_beepvolume},
	{"TVformat", param_TVformat},
	{"HDMIformat", param_HDMIformat},
	{"beepkey", param_beepkey},
	{"language", param_language},
	{"freq", param_freq},
	{"date", param_date},
	{"time", param_time},
	{"timeoffset", param_timeoffset},
	{NULL, NULL}, //last tag, it must be
};

UTOKEN_MODULE module_setup = {
	"usetup",
	tbl_setup_param,
	param_begin,
	param_end,
	param_unknown,
	NULL
};

#if SXCMD_TEST

static BOOL Cmd_usetup_set(CHAR *strCmd)
{

	uToken_Parse(strCmd, &module_setup);
	return TRUE;
}

SXCMD_BEGIN(usetup, "usetup")
SXCMD_ITEM("set %", Cmd_usetup_set, "")
SXCMD_END()

#endif

void UCtrlAppSetup_InstallCmd(void)
{
#if SXCMD_TEST
	SxCmd_AddTable(usetup);
#endif
}


