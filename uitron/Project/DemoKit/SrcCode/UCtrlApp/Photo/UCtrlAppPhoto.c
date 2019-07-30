#include "SysCommon.h"
#include "UIFramework.h"
#include "PrjCfg.h"
#include "uToken.h"
#include "UIInfo.h"
#include "UIAppPhoto.h"
#include "UIStorageCheck.h"
#include "UIControl.h"
#include "UIControlWnd.h"
#include "UCtrlAppPhoto.h"
#include "UCtrlAppPhotoID.h"
#include "NvtUctrlAPI.h"
#include "UCtrlMain.h"
#include "UIPhotoMapping.h"


#define THIS_DBGLVL         2 // 0=FATAL, 1=ERR, 2=WRN, 3=UNIT, 4=FUNC, 5=IND, 6=MSG, 7=VALUE, 8=USER
///////////////////////////////////////////////////////////////////////////////
#define __MODULE__          UCtrlAppPhoto
#define __DBGLVL__          ((THIS_DBGLVL>=PRJ_DBG_LVL)?THIS_DBGLVL:PRJ_DBG_LVL)
#define __DBGFLT__          "*" //*=All, [mark]=CustomClass
//#define __DBGFLT__          "[cap]"
#include "DebugModule.h"


#define SXCMD_TEST          0

#define ON                  1
#define OFF                 0

/*
    {NVTEVT_EXE_MACRO,              PhotoExe_OnMacro                },
    {NVTEVT_EXE_SELFTIMER,          PhotoExe_OnSelftimer            },
    {NVTEVT_EXE_EV,                 PhotoExe_OnEV                   },
    {NVTEVT_EXE_CAPTURE_SIZE,       PhotoExe_OnCaptureSize          },
    {NVTEVT_EXE_QUALITY,            PhotoExe_OnQuality              },
    {NVTEVT_EXE_WB,                 PhotoExe_OnWB                   },
    {NVTEVT_EXE_COLOR,              PhotoExe_OnColor                },
    {NVTEVT_EXE_ISO,                PhotoExe_OnISO                  },
    {NVTEVT_EXE_METERING,           PhotoExe_OnMetering             },
    {NVTEVT_EXE_DATE_PRINT,         PhotoExe_OnDatePrint            },
    {NVTEVT_EXE_PREVIEW,            PhotoExe_OnPreview              },
    {NVTEVT_EXE_FD,                 PhotoExe_OnFD                   },
    {NVTEVT_EXE_CONTSHOT,           PhotoExe_OnContShot             },
    {NVTEVT_EXE_SCENEMODE,          PhotoExe_OnSceneMode            },
    {NVTEVT_EXE_CAPTURE_START,      PhotoExe_OnCaptureStart         },
    {NVTEVT_EXE_CAPTURE_STOP,       PhotoExe_OnCaptureStop          },
    {NVTEVT_EXE_RSC,                PhotoExe_OnRSC                  },
    {NVTEVT_EXE_SHARPNESS,          PhotoExe_OnSharpness            },
    {NVTEVT_EXE_DUALCAM,            PhotoExe_OnDualcam              },
    {NVTEVT_EXE_SHDR,               PhotoExe_OnSHDR                 },
    {NVTEVT_EXE_WDR,                PhotoExe_OnWDR                  },
    {NVTEVT_EXE_ANTISHAKING,        PhotoExe_OnAntishake            },

*/
typedef struct _UCtrlPhotoParmType {
	UINT32 isSet;
	UINT32 index;
} UCtrlPhotoParmType;


typedef struct _UCtrlPhotoInfoType {
	BOOL                isErr;
	UCtrlPhotoParmType  macro;
	UCtrlPhotoParmType  selftimer;
	UCtrlPhotoParmType  ev;
	UCtrlPhotoParmType  size;
	UCtrlPhotoParmType  quality;
	UCtrlPhotoParmType  wb;
	UCtrlPhotoParmType  color;
	UCtrlPhotoParmType  iso;
	UCtrlPhotoParmType  ae;
	UCtrlPhotoParmType  datestamp;
	UCtrlPhotoParmType  qview;
	UCtrlPhotoParmType  fd;
	UCtrlPhotoParmType  contshot;
	UCtrlPhotoParmType  scene;
	UCtrlPhotoParmType  cap;
	UCtrlPhotoParmType  wait;
	UCtrlPhotoParmType  rsc;
	UCtrlPhotoParmType  sharp;
	UCtrlPhotoParmType  dualcam;
	UCtrlPhotoParmType  hdr;
	UCtrlPhotoParmType  wdr;
	UCtrlPhotoParmType  antishake;
	UCtrlPhotoParmType  getcap;
	UCtrlPhotoParmType  getfreepicnum;
} UCtrlPhotoInfoType;

static UCtrlPhotoInfoType   photoInfo;
static UCtrlPhotoInfoType  *pPhotoInfo = &photoInfo;


// flag define
#define FLG_UCTRL_PHOTO_CAPSTART           0x00000001
#define FLG_UCTRL_PHOTO_CAPEND             0x00000002



static INT32 param_macro(INT32 argc, char *argv[])
{
	UINT32 value;
	if (argc != 1) {
		DBG_ERR("argc!=1 (%d)\r\n", argc);
		return -1;
	}

	DBG_IND("get: %d\r\n", atoi(argv[0]));
	value = atoi(argv[0]);
	if (value >= MACRO_ID_MAX) {
		DBG_ERR("value =%d\r\n", value);
		return -1;
	}
	pPhotoInfo->macro.index = value;
	pPhotoInfo->macro.isSet = 1;
	return 0;
}

static INT32 param_selftimer(INT32 argc, char *argv[])
{
	UINT32 value;
	if (argc != 1) {
		DBG_ERR("argc!=1 (%d)\r\n", argc);
		return -1;
	}
	DBG_IND("get: %d\r\n", atoi(argv[0]));
	value = atoi(argv[0]);
	if (value >= SELFTIMER_ID_MAX) {
		DBG_ERR("value =%d\r\n", value);
		return -1;
	}
	pPhotoInfo->selftimer.index = value;
	pPhotoInfo->selftimer.isSet = 1;
	return 0;
}

static INT32 param_ev(INT32 argc, char *argv[])
{
	UINT32 value;
	if (argc != 1) {
		DBG_ERR("argc!=1 (%d)\r\n", argc);
		return -1;
	}
	DBG_IND("get: %d\r\n", atoi(argv[0]));
	value = atoi(argv[0]);
	if (value >= EV_SETTING_MAX) {
		DBG_ERR("value =%d\r\n", value);
		return -1;
	}
	pPhotoInfo->ev.index = value;
	pPhotoInfo->ev.isSet = 1;
	return 0;
}

#if 0
static INT32 uphoto_getPhotoSizeIndex(UINT32 width, UINT32 height)
{
	UINT32 i;
	for (i = 0; i < PHOTO_SIZE_ID_MAX; i++) {
		if (width == GetPhotoSizeWidth(i) && height == GetPhotoSizeHeight(i)) {
			DBG_IND("index =%d\r\n", i);
			return i;
		}
	}

	return -1;
}
static INT32 param_size(INT32 argc, char *argv[])
{
	INT32 value;
	if (argc != 2) {
		DBG_ERR("argc!=2 (%d)\r\n", argc);
		return -1;
	}
	DBG_IND("get: %s %s\r\n", argv[0], argv[1]);
	value = uphoto_getPhotoSizeIndex(atoi(argv[0]), atoi(argv[1]));
	if (value < 0) {
		DBG_ERR("Not support w=%s, h=%s\r\n", argv[0], argv[1]);
		return -1;
	}
	pPhotoInfo->size.index = value;
	pPhotoInfo->size.isSet = 1;
	return 0;
}
#else
static INT32 param_size(INT32 argc, char *argv[])
{
	UINT32 value;
	if (argc != 1) {
		DBG_ERR("argc!=1 (%d)\r\n", argc);
		return -1;
	}
	DBG_IND("get: %d\r\n", atoi(argv[0]));
	value = atoi(argv[0]);
	if (value >= PHOTO_SIZE_ID_MAX) {
		DBG_ERR("value =%d\r\n", value);
		return -1;
	}
	pPhotoInfo->size.index = value;
	pPhotoInfo->size.isSet = 1;
	return 0;
}
#endif
static INT32 param_quality(INT32 argc, char *argv[])
{
	UINT32 value;
	if (argc != 1) {
		DBG_ERR("argc!=1 (%d)\r\n", argc);
		return -1;
	}
	DBG_IND("get: %d\r\n", atoi(argv[0]));
	value = atoi(argv[0]);
	if (value >= QUALITY_ID_MAX) {
		DBG_ERR("value =%d\r\n", value);
		return -1;
	}
	pPhotoInfo->quality.index = value;
	pPhotoInfo->quality.isSet = 1;
	return 0;
}

static INT32 param_wb(INT32 argc, char *argv[])
{
	UINT32 value;
	if (argc != 1) {
		DBG_ERR("argc!=1 (%d)\r\n", argc);
		return -1;
	}
	DBG_IND("get: %d\r\n", atoi(argv[0]));
	value = atoi(argv[0]);
	if (value >= WB_ID_MAX) {
		DBG_ERR("value =%d\r\n", value);
		return -1;
	}
	pPhotoInfo->wb.index = value;
	pPhotoInfo->wb.isSet = 1;
	return 0;
}

static INT32 param_color(INT32 argc, char *argv[])
{
	UINT32 value;
	if (argc != 1) {
		DBG_ERR("argc!=1 (%d)\r\n", argc);
		return -1;
	}
	DBG_IND("get: %d\r\n", atoi(argv[0]));
	value = atoi(argv[0]);
	if (value >= COLOR_EFFECT_ID_MAX) {
		DBG_ERR("value =%d\r\n", value);
		return -1;
	}
	pPhotoInfo->color.index = value;
	pPhotoInfo->color.isSet = 1;
	return 0;
}

static INT32 param_iso(INT32 argc, char *argv[])
{
	UINT32 value;
	if (argc != 1) {
		DBG_ERR("argc!=1 (%d)\r\n", argc);
		return -1;
	}
	DBG_IND("get: %d\r\n", atoi(argv[0]));
	value = atoi(argv[0]);
	if (value >= ISO_ID_MAX) {
		DBG_ERR("value =%d\r\n", value);
		return -1;
	}
	pPhotoInfo->iso.index = value;
	pPhotoInfo->iso.isSet = 1;
	return 0;
}

static INT32 param_ae(INT32 argc, char *argv[])
{
	UINT32 value;
	if (argc != 1) {
		DBG_ERR("argc!=1 (%d)\r\n", argc);
		return -1;
	}
	DBG_IND("get: %d\r\n", atoi(argv[0]));
	value = atoi(argv[0]);
	if (value >= METERING_ID_MAX) {
		DBG_ERR("value =%d\r\n", value);
		return -1;
	}
	pPhotoInfo->ae.index = value;
	pPhotoInfo->ae.isSet = 1;
	return 0;
}

static INT32 param_datestamp(INT32 argc, char *argv[])
{
	UINT32 value;
	if (argc != 1) {
		DBG_ERR("argc!=1 (%d)\r\n", argc);
		return -1;
	}
	DBG_IND("get: %d\r\n", atoi(argv[0]));
	value = atoi(argv[0]);
	if (value >= DATE_STAMP_ID_MAX) {
		DBG_ERR("value =%d\r\n", value);
		return -1;
	}
	pPhotoInfo->datestamp.index = value;
	pPhotoInfo->datestamp.isSet = 1;
	return 0;
}

static INT32 param_qview(INT32 argc, char *argv[])
{
	UINT32 value;
	if (argc != 1) {
		DBG_ERR("argc!=1 (%d)\r\n", argc);
		return -1;
	}
	DBG_IND("get: %d\r\n", atoi(argv[0]));
	value = atoi(argv[0]);
	if (value >= QUICK_REVIEW_ID_MAX) {
		DBG_ERR("value =%d\r\n", value);
		return -1;
	}
	pPhotoInfo->qview.index = value;
	pPhotoInfo->qview.isSet = 1;
	return 0;
}

static INT32 param_fd(INT32 argc, char *argv[])
{
	UINT32 value;
	if (argc != 1) {
		DBG_ERR("argc!=1 (%d)\r\n", argc);
		return -1;
	}

	DBG_IND("get: %d\r\n", atoi(argv[0]));
	value = atoi(argv[0]);
	if (value >= FD_ID_MAX) {
		DBG_ERR("value =%d\r\n", value);
		return -1;
	}
	pPhotoInfo->fd.index = value;
	pPhotoInfo->fd.isSet = 1;
	return 0;
}

static INT32 param_contshot(INT32 argc, char *argv[])
{
	UINT32 value;
	if (argc != 1) {
		DBG_ERR("argc!=1 (%d)\r\n", argc);
		return -1;
	}
	DBG_IND("get: %d\r\n", atoi(argv[0]));
	value = atoi(argv[0]);
	if (value >= CONTINUE_SHOT_SETTING_MAX) {
		DBG_ERR("value =%d\r\n", value);
		return -1;
	}
	pPhotoInfo->contshot.index = value;
	pPhotoInfo->contshot.isSet = 1;
	return 0;
}

static INT32 param_scene(INT32 argc, char *argv[])
{
	UINT32 value;
	if (argc != 1) {
		DBG_ERR("argc!=1 (%d)\r\n", argc);
		return -1;
	}
	DBG_IND("get: %d\r\n", atoi(argv[0]));
	value = atoi(argv[0]);
	if (value >= SCENE_ID_MAX) {
		DBG_ERR("value =%d\r\n", value);
		return -1;
	}
	pPhotoInfo->scene.index = value;
	pPhotoInfo->scene.isSet = 1;
	return 0;
}

static INT32 param_cap(INT32 argc, char *argv[])
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
	pPhotoInfo->cap.index = value;
	pPhotoInfo->cap.isSet = 1;
	return 0;
}

static INT32 param_wait(INT32 argc, char *argv[])
{
	pPhotoInfo->wait.isSet = 1;
	return 0;
}

static INT32 param_rsc(INT32 argc, char *argv[])
{
	UINT32 value;
	if (argc != 1) {
		DBG_ERR("argc!=1 (%d)\r\n", argc);
		return -1;
	}
	DBG_IND("get: %d\r\n", atoi(argv[0]));
	value = atoi(argv[0]);
	if (value >= RSC_SETTING_MAX) {
		DBG_ERR("value =%d\r\n", value);
		return -1;
	}
	pPhotoInfo->rsc.index = value;
	pPhotoInfo->rsc.isSet = 1;
	return 0;
}

static INT32 param_sharp(INT32 argc, char *argv[])
{
	UINT32 value;
	if (argc != 1) {
		DBG_ERR("argc!=1 (%d)\r\n", argc);
		return -1;
	}
	DBG_IND("get: %d\r\n", atoi(argv[0]));
	value = atoi(argv[0]);
	if (value >= SHARPNESS_ID_MAX) {
		DBG_ERR("value =%d\r\n", value);
		return -1;
	}
	pPhotoInfo->sharp.index = value;
	pPhotoInfo->sharp.isSet = 1;
	return 0;
}

static INT32 param_dualcam(INT32 argc, char *argv[])
{
	UINT32 value;
	if (argc != 1) {
		DBG_ERR("argc!=1 (%d)\r\n", argc);
		return -1;
	}
	DBG_IND("get: %d\r\n", atoi(argv[0]));
	value = atoi(argv[0]);
	if (value >= DUALCAM_SETTING_MAX) {
		DBG_ERR("value =%d\r\n", value);
		return -1;
	}
	pPhotoInfo->dualcam.index = value;
	pPhotoInfo->dualcam.isSet = 1;
	return 0;
}

static INT32 param_hdr(INT32 argc, char *argv[])
{
	UINT32 value;
	if (argc != 1) {
		DBG_ERR("argc!=1 (%d)\r\n", argc);
		return -1;
	}
	DBG_IND("get: %d\r\n", atoi(argv[0]));
	value = atoi(argv[0]);
	if (value >= SHDR_SETTING_MAX) {
		DBG_ERR("value =%d\r\n", value);
		return -1;
	}
	pPhotoInfo->hdr.index = value;
	pPhotoInfo->hdr.isSet = 1;
	return 0;
}

static INT32 param_wdr(INT32 argc, char *argv[])
{
	UINT32 value;
	if (argc != 1) {
		DBG_ERR("argc!=1 (%d)\r\n", argc);
		return -1;
	}
	DBG_IND("get: %d\r\n", atoi(argv[0]));
	value = atoi(argv[0]);
	if (value >= WDR_SETTING_MAX) {
		DBG_ERR("value =%d\r\n", value);
		return -1;
	}
	pPhotoInfo->wdr.index = value;
	pPhotoInfo->wdr.isSet = 1;
	return 0;
}

static INT32 param_antishake(INT32 argc, char *argv[])
{
	UINT32 value;
	if (argc != 1) {
		DBG_ERR("argc!=1 (%d)\r\n", argc);
		return -1;
	}
	DBG_IND("get: %d\r\n", atoi(argv[0]));
	value = atoi(argv[0]);
	if (value >= ANTISHAKE_ID_MAX) {
		DBG_ERR("value =%d\r\n", value);
		return -1;
	}
	pPhotoInfo->antishake.index = value;
	pPhotoInfo->antishake.isSet = 1;
	return 0;
}


static INT32 param_getcap(INT32 argc, char *argv[])
{
	DBG_IND("getcap\r\n");
	pPhotoInfo->getcap.isSet = 1;
	return 0;
}

static INT32 param_getfreepicnum(INT32 argc, char *argv[])
{
	DBG_IND("getfreepicnum\r\n");
	pPhotoInfo->getfreepicnum.isSet = 1;
	return 0;
}


static INT32 param_begin(char *str, INT32 len)
{
	DBG_IND("get: %s\r\n", str);
	memset(pPhotoInfo, 0, sizeof(UCtrlPhotoInfoType));
	return 0;
}

static INT32 param_end(UTOKEN_END_INFO *p_info)
{
	char retStr[128];
	int  ret;
	int iCurrMode = System_GetState(SYS_STATE_CURRMODE);

	if (p_info->err_code < 0) {
		snprintf(retStr, sizeof(retStr), "ERR: parm %s", p_info->failed_name);
		NvtUctrl_SetRetString(retStr);
		DBG_ERR("%s\r\n", retStr);
		return p_info->err_code;
	}
	if (iCurrMode != PRIMARY_MODE_PHOTO) {
		snprintf(retStr, sizeof(retStr), "ERR: CurrMode is not Photo");
		NvtUctrl_SetRetString(retStr);
		DBG_ERR("%s\r\n", retStr);
		return -1;
	}
	if (pPhotoInfo->getcap.isSet && pPhotoInfo->getfreepicnum.isSet) {
		snprintf(retStr, sizeof(retStr), "ERR: Can't get multiple data");
		NvtUctrl_SetRetString(retStr);
		DBG_ERR("%s\r\n", retStr);
		return -1;
	}
	DBG_IND("isStartCapture: %d\r\n", kchk_flg(UCTRL_PHOTO_FLG_ID, FLG_UCTRL_PHOTO_CAPSTART));
	DBG_IND("cap: isSet=%d, index=%d \r\n", pPhotoInfo->cap.isSet, pPhotoInfo->cap.index);
	if (kchk_flg(UCTRL_PHOTO_FLG_ID, FLG_UCTRL_PHOTO_CAPSTART) && !(pPhotoInfo->cap.isSet && (pPhotoInfo->cap.index == OFF))) {
		snprintf(retStr, sizeof(retStr), "ERR: is under Capturing");
		NvtUctrl_SetRetString(retStr);
		DBG_ERR("%s\r\n", retStr);
		return -1;
	}
	if (pPhotoInfo->macro.isSet) {
		UctrlMain_SendEvent(NVTEVT_EXE_MACRO,             1,  pPhotoInfo->macro.index);
	}
	if (pPhotoInfo->selftimer.isSet) {
		UctrlMain_SendEvent(NVTEVT_EXE_SELFTIMER,         1,  pPhotoInfo->selftimer.index);
	}
	if (pPhotoInfo->ev.isSet) {
		UctrlMain_SendEvent(NVTEVT_EXE_EV,                1,  pPhotoInfo->ev.index);
	}
	if (pPhotoInfo->size.isSet) {
		UctrlMain_SendEvent(NVTEVT_EXE_CAPTURE_SIZE,      1,  pPhotoInfo->size.index);
	}
	if (pPhotoInfo->quality.isSet) {
		UctrlMain_SendEvent(NVTEVT_EXE_QUALITY,           1,  pPhotoInfo->quality.index);
	}
	if (pPhotoInfo->wb.isSet) {
		UctrlMain_SendEvent(NVTEVT_EXE_WB,                1,  pPhotoInfo->wb.index);
	}
	if (pPhotoInfo->color.isSet) {
		UctrlMain_SendEvent(NVTEVT_EXE_COLOR,             1,  pPhotoInfo->color.index);
	}
	if (pPhotoInfo->iso.isSet) {
		UctrlMain_SendEvent(NVTEVT_EXE_ISO,               1,  pPhotoInfo->iso.index);
	}
	if (pPhotoInfo->ae.isSet) {
		UctrlMain_SendEvent(NVTEVT_EXE_METERING,          1,  pPhotoInfo->ae.index);
	}
	if (pPhotoInfo->datestamp.isSet) {
		UctrlMain_SendEvent(NVTEVT_EXE_DATE_PRINT,        1,  pPhotoInfo->datestamp.index);
	}
	if (pPhotoInfo->qview.isSet) {
		UctrlMain_SendEvent(NVTEVT_EXE_PREVIEW,           1,  pPhotoInfo->qview.index);
	}
	if (pPhotoInfo->fd.isSet) {
		UctrlMain_SendEvent(NVTEVT_EXE_FD,                1,  pPhotoInfo->fd.index);
	}
	if (pPhotoInfo->contshot.isSet) {
		UctrlMain_SendEvent(NVTEVT_EXE_CONTSHOT,          1,  pPhotoInfo->contshot.index);
	}
	if (pPhotoInfo->scene.isSet) {
		UctrlMain_SendEvent(NVTEVT_EXE_SCENEMODE,         1,  pPhotoInfo->scene.index);
	}
	if (pPhotoInfo->rsc.isSet) {
		UctrlMain_SendEvent(NVTEVT_EXE_RSC,               1,  pPhotoInfo->rsc.index);
	}
	if (pPhotoInfo->sharp.isSet) {
		UctrlMain_SendEvent(NVTEVT_EXE_SHARPNESS,         1,  pPhotoInfo->sharp.index);
	}
	if (pPhotoInfo->dualcam.isSet) {
		UctrlMain_SendEvent(NVTEVT_EXE_DUALCAM,           1,  pPhotoInfo->dualcam.index);
	}
	if (pPhotoInfo->hdr.isSet) {
		UctrlMain_SendEvent(NVTEVT_EXE_SHDR,              1,  pPhotoInfo->hdr.index);
	}
	if (pPhotoInfo->wdr.isSet) {
		UctrlMain_SendEvent(NVTEVT_EXE_WDR,               1,  pPhotoInfo->wdr.index);
	}
	if (pPhotoInfo->antishake.isSet) {
		UctrlMain_SendEvent(NVTEVT_EXE_ANTISHAKING,       1,  pPhotoInfo->antishake.index);
	}

	if (pPhotoInfo->cap.isSet && pPhotoInfo->cap.index == ON && pPhotoInfo->wait.isSet && UI_GetData(FL_CONTINUE_SHOT) == CONTINUE_SHOT_BURST) {
		snprintf(retStr, sizeof(retStr), "ERR: Can't wait contshot burst capture end");
		NvtUctrl_SetRetString(retStr);
		DBG_ERR("%s\r\n", retStr);
		return -1;
	}
	// update ui info
	if (!pPhotoInfo->cap.isSet) {
		if (iCurrMode == PRIMARY_MODE_PHOTO) {
			UctrlMain_SendEvent(NVTEVT_UPDATE_INFO,       1,  UIAPPPHOTO_CB_UCTRL_UPDATE_UI);
		}
	}
	// capture
	else {
		if (pPhotoInfo->cap.index == ON) {
			UctrlMain_SendEvent(NVTEVT_EXE_CAPTURE_START,     0,  0);
		} else {
			UctrlMain_SendEvent(NVTEVT_EXE_CAPTURE_STOP,      0,  0);
		}
	}
	// wait capture complete
	if (pPhotoInfo->wait.isSet) {
		FLGPTN     uiFlag;
		DBG_IND("wait\r\n");
		wai_flg(&uiFlag, UCTRL_PHOTO_FLG_ID, FLG_UCTRL_PHOTO_CAPEND, TWF_ORW);
		DBG_IND("wait end\r\n");
	}
	if (pPhotoInfo->getcap.isSet) {
		UINT32            FreePicNum = 0;
		char             *pFilePath = 0;
		char             *pFileName = 0;

		if (UIStorageCheck(STORAGE_CHECK_FOLDER_FULL, NULL)) {
			snprintf(retStr, sizeof(retStr), "ERR: folder full");
			ret = -1;
		} else if (UIStorageCheck(STORAGE_CHECK_FULL, &FreePicNum)) {
			snprintf(retStr, sizeof(retStr), "ERR: storage full");
			ret = -1;
		} else if (System_GetState(SYS_STATE_FS) == FS_DISK_ERROR) {
			snprintf(retStr, sizeof(retStr), "ERR: write file fail");
			ret = -1;
		} else {
			pFilePath = NULL;//ImageApp_CamFile_GetLastWriteFilePath();
			if (pFilePath == NULL) {
				DBG_ERR("pFilePath is NULL\r\n");
				snprintf(retStr, sizeof(ret), "ERR: write file fail");
				NvtUctrl_SetRetString(retStr);
				return -1;
			}
			DBG_IND("pFilePath = %s\r\n", pFilePath);
			if (strlen(pFilePath)) {
				pFileName = strchr(pFilePath, '\\');
				DBG_IND("pFileName = %s\r\n", pFileName);
				snprintf(retStr, sizeof(retStr), "%s %d", pFileName, FreePicNum);
				ret = 0;
			} else {
				snprintf(retStr, sizeof(retStr), "ERR: write file fail");
				ret = -1;
			}
		}
		NvtUctrl_SetRetString(retStr);
		return ret;

	}
	if (pPhotoInfo->getfreepicnum.isSet) {
		UINT32            FreePicNum = 0;
		UIStorageCheck(STORAGE_CHECK_FULL, &FreePicNum);
		snprintf(retStr, sizeof(retStr), "%d", FreePicNum);
		NvtUctrl_SetRetString(retStr);
		return 0;
	}
	return 0;
}

static INT32 param_unknown(char *name)
{
	DBG_ERR("unknown param: %s\r\n", name);
	return 0;
}

UTOKEN_PARAM tbl_photo_param[] = {
	{"macro", param_macro},
	{"selftimer", param_selftimer},
	{"ev", param_ev},
	{"size", param_size},
	{"quality", param_quality},
	{"wb", param_wb},
	{"color", param_color},
	{"iso", param_iso},
	{"ae", param_ae},
	{"datestamp", param_datestamp},
	{"qview", param_qview},
	{"fd", param_fd},
	{"contshot", param_contshot},
	{"scene", param_scene},
	{"cap", param_cap},
	{"wait", param_wait},
	{"rsc", param_rsc},
	{"sharp", param_sharp},
	{"dualcam", param_dualcam},
	{"hdr", param_hdr},
	{"wdr", param_wdr},
	{"antishake", param_antishake},
	{"getcap", param_getcap},
	{"getfreepicnum", param_getfreepicnum},
	{NULL, NULL}, //last tag, it must be
};

UTOKEN_MODULE module_photo = {
	"uphoto",
	tbl_photo_param,
	param_begin,
	param_end,
	param_unknown,
	NULL
};

void UCtrlAppPhoto_Open(void)
{
	clr_flg(UCTRL_PHOTO_FLG_ID, 0xFFFFFFFF);
}

void UCtrlAppPhoto_CallBack(UINT32 callBackEvent)
{
	static BOOL isCapEnd = FALSE;
	static BOOL isWriteFileFinish = FALSE;

	switch (callBackEvent) {
	case UIAPPPHOTO_CB_CAPSTART:
		clr_flg(UCTRL_PHOTO_FLG_ID, FLG_UCTRL_PHOTO_CAPEND);
		set_flg(UCTRL_PHOTO_FLG_ID, FLG_UCTRL_PHOTO_CAPSTART);
		isCapEnd = FALSE;
		isWriteFileFinish = FALSE;
		DBG_IND("Cap start\r\n");
		break;
	case UIAPPPHOTO_CB_CAPTUREEND:
		isCapEnd = TRUE;
		DBG_IND("Cap end\r\n");
		if (isCapEnd && isWriteFileFinish) {
			clr_flg(UCTRL_PHOTO_FLG_ID, FLG_UCTRL_PHOTO_CAPSTART);
			set_flg(UCTRL_PHOTO_FLG_ID, FLG_UCTRL_PHOTO_CAPEND);
		}
		break;
	case UIAPPPHOTO_CB_FSTOK:
		isWriteFileFinish = TRUE;
		DBG_IND("FST OK\r\n");
		if (isCapEnd && isWriteFileFinish) {
			clr_flg(UCTRL_PHOTO_FLG_ID, FLG_UCTRL_PHOTO_CAPSTART);
			set_flg(UCTRL_PHOTO_FLG_ID, FLG_UCTRL_PHOTO_CAPEND);
		}
		break;

	default:
		break;
	}
}


#if SXCMD_TEST

static BOOL Cmd_uphoto_set(CHAR *strCmd)
{
	uToken_Parse(strCmd, &module_photo);
	return TRUE;
}

SXCMD_BEGIN(uphoto, "uphoto")
SXCMD_ITEM("set %", Cmd_uphoto_set, "")
SXCMD_END()
#endif



