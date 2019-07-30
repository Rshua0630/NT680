#include "SysCommon.h"
#include "PrjCfg.h"
#include "SysCfg.h"
#include "GxSystem.h"
#include "SysMain.h"
#include "Debug.h"

#include "AppCommon.h"

#include "UIModeMain.h"
#if(IPCAM_MODE==ENABLE)
#include "UIModeIPCam.h"
#endif
#if(IPCAMPB_MODE==ENABLE)
#include "UIModeIPCamPB.h"
#endif
#if(MOVIE_MODE==ENABLE)
#include "UIModeMovie.h"
#endif
#if(PHOTO_MODE==ENABLE)
#include "UIModePhoto.h"
#endif
#if(PLAY_MODE==ENABLE)
#include "UIModePlayback.h"
#endif
#if(UPDFW_MODE==ENABLE)
#include "UIModeUpdFw.h"
#endif

//global debug level: PRJ_DBG_LVL
#include "PrjCfg.h"
//local debug level: THIS_DBGLVL
#define THIS_DBGLVL         2 // 0=FATAL, 1=ERR, 2=WRN, 3=UNIT, 4=FUNC, 5=IND, 6=MSG, 7=VALUE, 8=USER
///////////////////////////////////////////////////////////////////////////////
#define __MODULE__          SysModeCmd
#define __DBGLVL__          ((THIS_DBGLVL>=PRJ_DBG_LVL)?THIS_DBGLVL:PRJ_DBG_LVL)
#define __DBGFLT__          "*" //*=All, [mark]=CustomClass
#include "DebugModule.h"


///////////////////////////////////////////////////////////////////////////////
// mode cmd
///////////////////////////////////////////////////////////////////////////////

//#NT#2016/09/20#Bob Huang -begin
//#NT#Support HDMI Display with 3DNR Out
#if (_3DNROUT_FUNC == ENABLE)
extern BOOL gb3DNROut;
#endif
//#NT#2016/09/20#Bob Huang -end

static BOOL Cmd_mode_main(CHAR *strCmd)
{
	DBG_DUMP("Main Mode\r\n");
	{
#if(UI_FUNC==ENABLE)
		int iCurrMode = System_GetState(SYS_STATE_CURRMODE);
#endif
		int iNextMode = PRIMARY_MODE_MAIN;
#if(UI_FUNC==ENABLE)
		UI_SetData(FL_PreMode, iCurrMode);
		UI_SetData(FL_NextMode, iNextMode);
#endif
		Ux_PostEvent(NVTEVT_SYSTEM_MODE, 1, iNextMode);
	}
	return TRUE;
}
static BOOL Cmd_mode_photo(CHAR *strCmd)
{
#if(PHOTO_MODE==ENABLE)
	DBG_DUMP("Photo Mode\r\n");
	{
		int iCurrMode = System_GetState(SYS_STATE_CURRMODE);
		int iNextMode = PRIMARY_MODE_PHOTO;
		UI_SetData(FL_PreMode, iCurrMode);
		UI_SetData(FL_NextMode, iNextMode);
		Ux_PostEvent(NVTEVT_SYSTEM_MODE, 1, iNextMode);
	}
#endif
	return TRUE;
}
static BOOL Cmd_mode_movie(CHAR *strCmd)
{
#if(MOVIE_MODE==ENABLE)
	DBG_DUMP("Movie Mode\r\n");
	{
		//#NT#2016/09/20#Bob Huang -begin
		//#NT#Support HDMI Display with 3DNR Out
#if (_3DNROUT_FUNC == ENABLE)
		gb3DNROut = FALSE;
#endif
		//#NT#2016/09/20#Bob Huang -end
		int iCurrMode = System_GetState(SYS_STATE_CURRMODE);
		int iNextMode = PRIMARY_MODE_MOVIE;
		UI_SetData(FL_PreMode, iCurrMode);
		UI_SetData(FL_NextMode, iNextMode);
		Ux_PostEvent(NVTEVT_SYSTEM_MODE, 1, iNextMode);
	}
#endif
	return TRUE;
}

static BOOL Cmd_mode_ipcam(CHAR *strCmd)
{
#if(IPCAM_MODE==ENABLE)
	DBG_DUMP("IPcam Mode\r\n");

	//#NT#2016/09/20#Bob Huang -begin
	//#NT#Support HDMI Display with 3DNR Out
#if (_3DNROUT_FUNC == ENABLE)
	gb3DNROut = FALSE;
#endif
	//#NT#2016/09/20#Bob Huang -end
#if (UI_FUNC == ENABLE)
	int iCurrMode = System_GetState(SYS_STATE_CURRMODE);
#endif
	int iNextMode = PRIMARY_MODE_IPCAM;
#if (UI_FUNC == ENABLE)
	UI_SetData(FL_PreMode, iCurrMode);
	UI_SetData(FL_NextMode, iNextMode);
#endif
	Ux_PostEvent(NVTEVT_SYSTEM_MODE, 1, iNextMode);

#endif
	return TRUE;
}

static BOOL Cmd_mode_ipcampb(CHAR *strCmd)
{
#if(IPCAMPB_MODE==ENABLE)
	DBG_DUMP("IPcam Mode\r\n");


#if (UI_FUNC == ENABLE)
	int iCurrMode = System_GetState(SYS_STATE_CURRMODE);
#endif
	int iNextMode = PRIMARY_MODE_IPCAMPB;
#if (UI_FUNC == ENABLE)
	UI_SetData(FL_PreMode, iCurrMode);
	UI_SetData(FL_NextMode, iNextMode);
#endif
	Ux_PostEvent(NVTEVT_SYSTEM_MODE, 1, iNextMode);

#endif
	return TRUE;
}

static BOOL Cmd_mode_playback(CHAR *strCmd)
{
#if(PLAY_MODE==ENABLE)
	DBG_DUMP("Playback Mode\r\n");
	{
		int iCurrMode = System_GetState(SYS_STATE_CURRMODE);
		int iNextMode = PRIMARY_MODE_PLAYBACK;
		UI_SetData(FL_PreMode, iCurrMode);
		UI_SetData(FL_NextMode, iNextMode);
		Ux_PostEvent(NVTEVT_SYSTEM_MODE, 1, iNextMode);
	}
#endif
	return TRUE;
}
static BOOL Cmd_mode_msdc(CHAR *strCmd)
{
#if(USB_MODE==ENABLE)
	DBG_DUMP("USB MSDC Mode\r\n");
	{
		int iCurrMode = System_GetState(SYS_STATE_CURRMODE);
		int iNextMode = PRIMARY_MODE_USBMSDC;
		UI_SetData(FL_PreMode, iCurrMode);
		UI_SetData(FL_NextMode, iNextMode);
		Ux_PostEvent(NVTEVT_SYSTEM_MODE, 1, iNextMode);
	}
#endif
	return TRUE;
}
static BOOL Cmd_mode_sidc(CHAR *strCmd)
{
#if(USB_MODE==ENABLE)
	DBG_DUMP("USB SIDC Mode\r\n");
	//Ux_PostEvent(NVTEVT_SYSTEM_MODE, 1, PRIMARY_MODE_USBSIDC);
#endif
	return TRUE;
}
static BOOL Cmd_mode_pcc(CHAR *strCmd)
{
#if(USB_MODE==ENABLE)
	DBG_DUMP("USB PCC Mode\r\n");
	{
		int iCurrMode = System_GetState(SYS_STATE_CURRMODE);
		int iNextMode = PRIMARY_MODE_USBPCC;
		UI_SetData(FL_PreMode, iCurrMode);
		UI_SetData(FL_NextMode, iNextMode);
		Ux_PostEvent(NVTEVT_SYSTEM_MODE, 1, iNextMode);
	}
#endif
	return TRUE;
}
static BOOL Cmd_mode_sleep(CHAR *strCmd)
{
	DBG_DUMP("Sleep Mode\r\n");
	{
#if (UI_FUNC == ENABLE)
		int iCurrMode = System_GetState(SYS_STATE_CURRMODE);
#endif
		int iNextMode = PRIMARY_MODE_SLEEP;
#if (UI_FUNC == ENABLE)
		UI_SetData(FL_PreMode, iCurrMode);
		UI_SetData(FL_NextMode, iNextMode);
#endif
		Ux_PostEvent(NVTEVT_SYSTEM_MODE, 1, iNextMode);
	}
	return TRUE;
}
static BOOL Cmd_mode_updfw(CHAR *strCmd)
{
#if(UPDFW_MODE==ENABLE)
	DBG_DUMP("Update F.W Mode\r\n");
	Ux_PostEvent(NVTEVT_SYSTEM_MODE, 1, PRIMARY_MODE_UPDFW);
#else
	DBG_ERR("Not support update firmware mode.\r\n");
#endif
	return TRUE;
}

UINT32      g_lviewframeRate = 30, g_lviewTargetRate = (800 * 1024), g_lviewQueueFrame = 2, g_lviewPort = 8192, g_lviewTaskPri = 6, g_hwmemcpy = 1, g_maxJpgSize = 204800, g_reqDelay = 0;

static BOOL Cmd_mode_lview(CHAR *strCmd)
{
	sscanf_s(strCmd, "%d %d %d %d %d %d %d %d", &g_lviewframeRate, &g_lviewTargetRate, &g_lviewQueueFrame, &g_lviewPort, &g_lviewTaskPri, &g_hwmemcpy, &g_maxJpgSize, &g_reqDelay);
	return TRUE;
}
//#NT#2016/05/31#Ben Wang -begin
//#NT#Add UVC multimedia function.
#if(UVC_MULTIMEDIA_FUNC == ENABLE)
static BOOL Cmd_mode_uvc_multimedia(CHAR *strCmd)
{
//#NT#2018/01/12#Ben Wang -begin
//#NT#Support dual USB device detection
	USB_UpdateSource(GXUSB_HDL_ID_1);
//#NT#2018/01/12#Ben Wang -end
	if ((USB_GetSource() == USB_SRC_USB_PC) || (USB_GetSource() == USB_SRC_USB_CHARGING_PC)) {
		int iCurrMode = System_GetState(SYS_STATE_CURRMODE);
		int iNextMode = PRIMARY_MODE_MOVIE;
		DBG_DUMP("UVC Multimedia Mode\r\n");
		UI_SetData(FL_PreMode, iCurrMode);
		UI_SetData(FL_NextMode, iNextMode);
		Ux_PostEvent(NVTEVT_SYSTEM_MODE, 2, iNextMode, SYS_SUBMODE_UVC);
	} else {
		DBG_ERR("Not connecting to a USB HOST!\r\n");
	}
	return TRUE;
}
#endif
//#NT#2016/05/31#Ben Wang -end

//#NT#2016/09/20#Bob Huang -begin
//#NT#Support HDMI Display with 3DNR Out
#if (_3DNROUT_FUNC == ENABLE)
static BOOL Cmd_mode_3dnr(CHAR *strCmd)
{
	DBG_DUMP("3DNR Mode\r\n");
	gb3DNROut = TRUE;
	int iCurrMode = System_GetState(SYS_STATE_CURRMODE);
	int iNextMode = PRIMARY_MODE_MOVIE;
	UI_SetData(FL_PreMode, iCurrMode);
	UI_SetData(FL_NextMode, iNextMode);
	Ux_PostEvent(NVTEVT_SYSTEM_MODE, 1, iNextMode);
	return TRUE;
}
#endif
//#NT#2016/09/20#Bob Huang -end

SXCMD_BEGIN(mode, "mode switch")
SXCMD_ITEM("main", Cmd_mode_main, "main mode")
SXCMD_ITEM("photo", Cmd_mode_photo, "photo mode")
SXCMD_ITEM("movie", Cmd_mode_movie, "movie mode")
SXCMD_ITEM("playback", Cmd_mode_playback, "playback mode")
SXCMD_ITEM("msdc", Cmd_mode_msdc, "msdc mode")
SXCMD_ITEM("sidc", Cmd_mode_sidc, "sidc mode")
SXCMD_ITEM("pcc", Cmd_mode_pcc, "pcc mode")
SXCMD_ITEM("sleep", Cmd_mode_sleep, "sleep mode")
SXCMD_ITEM("lview %", Cmd_mode_lview, "live view mode setting")
SXCMD_ITEM("ipcam", Cmd_mode_ipcam, "ipcam mode")
SXCMD_ITEM("ipcampb", Cmd_mode_ipcampb, "ipcam PlayBack mode")
SXCMD_ITEM("updfw", Cmd_mode_updfw, "update firmware mode")
//#NT#2016/05/31#Ben Wang -begin
//#NT#Add UVC multimedia function.
#if(UVC_MULTIMEDIA_FUNC == ENABLE)
SXCMD_ITEM("uvc", Cmd_mode_uvc_multimedia, "uvc multimedia mode")
#endif
//#NT#2016/05/31#Ben Wang -end
//#NT#2016/09/20#Bob Huang -begin
//#NT#Support HDMI Display with 3DNR Out
#if (_3DNROUT_FUNC == ENABLE)
SXCMD_ITEM("3dnr", Cmd_mode_3dnr, "3dnr mode")
#endif
//#NT#2016/09/20#Bob Huang -end
SXCMD_END()
