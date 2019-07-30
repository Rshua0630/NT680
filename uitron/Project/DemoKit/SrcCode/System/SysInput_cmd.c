//global debug level: PRJ_DBG_LVL
#include "PrjCfg.h"

#include "SysCommon.h"
#include "AppLib.h"

#include "UIFramework.h"
#include "NVTUserCommand.h"
#include "NvtUser.h"

#if (UI_FUNC == ENABLE)
#include "UIAppMovie.h"  // Isiah, implement YUV merge mode of recording func.
#include "UIAppMovie_Param.h"
#include "UIMovieMapping.h"
#if(WIFI_AP_FUNC==ENABLE)
#include "UIFrameworkExt.h"
#include "UIMenuWndWiFiWait.h"
#endif
#if(WIFI_FUNC==ENABLE)
#include "UIModeWifi.h"
#endif

#endif

//local debug level: THIS_DBGLVL
#define THIS_DBGLVL         2 // 0=FATAL, 1=ERR, 2=WRN, 3=UNIT, 4=FUNC, 5=IND, 6=MSG, 7=VALUE, 8=USER
///////////////////////////////////////////////////////////////////////////////
#define __MODULE__          SysInCmd
#define __DBGLVL__          ((THIS_DBGLVL>=PRJ_DBG_LVL)?THIS_DBGLVL:PRJ_DBG_LVL)
#define __DBGFLT__          "*" //*=All, [mark]=CustomClass
#include "DebugModule.h"
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
// key input
///////////////////////////////////////////////////////////////////////////////

//#Fix warning message
//UINT32  uiKeyTmpMsk;

static BOOL Cmd_key_power(CHAR *strCmd)
{
	//if(uiKeyTmpMsk & FLGkey_XXX)
	{
		DBG_DUMP("Power Key\r\n");
		//Ux_PostEvent(NVTEVT_KEY_POWER, 1, NVTEVT_KEY_PRESS);
		Ux_PostEvent(NVTEVT_SYSTEM_SHUTDOWN, 1, 0);
		return TRUE;
	}
}
static BOOL Cmd_key_delete(CHAR *strCmd)
{
	//if(uiKeyTmpMsk & FLGkey_DEL)
	{
		DBG_DUMP("Del Key\r\n");
		Ux_PostEvent(NVTEVT_KEY_DEL, 1, NVTEVT_KEY_PRESS);
		return TRUE;
	}
}
BOOL Cmd_key_menu(CHAR *strCmd)
{
	//if(uiKeyTmpMsk & FLGkey_MENU)
	{
		DBG_DUMP("Menu Key\r\n");
		Ux_PostEvent(NVTEVT_KEY_MENU, 1, NVTEVT_KEY_PRESS);
		return TRUE;
	}
}
static BOOL Cmd_key_mode(CHAR *strCmd)
{
	//if(uiKeyTmpMsk & FLGkey_MODE)
	{
		DBG_DUMP("Mode Key\r\n");
		Ux_PostEvent(NVTEVT_KEY_MODE, 1, NVTEVT_KEY_PRESS);
		return TRUE;
	}
}
static BOOL Cmd_key_playback(CHAR *strCmd)
{
	//if(uiKeyTmpMsk & FLGkey_PLAYBACK)
	{
		DBG_DUMP("Playback Key\r\n");
		Ux_PostEvent(NVTEVT_KEY_PLAYBACK, 1, NVTEVT_KEY_PRESS);
		return TRUE;
	}
}

static BOOL Cmd_key_movie(CHAR *strCmd)
{
	//if(uiKeyTmpMsk & FLGkey_PLAYBACK)
	{
		DBG_DUMP("Movie Key\r\n");
		Ux_PostEvent(NVTEVT_KEY_MOVIE, 1, NVTEVT_KEY_PRESS);
		return TRUE;
	}
}
static BOOL Cmd_key_up(CHAR *strCmd)
{
	//if(uiKeyTmpMsk & FLGkey_UP)
	{
		DBG_DUMP("Up Key\r\n");
		Ux_PostEvent(NVTEVT_KEY_UP, 1, NVTEVT_KEY_PRESS);
		return TRUE;
	}
}
static BOOL Cmd_key_down(CHAR *strCmd)
{
	//if(uiKeyTmpMsk & FLGkey_DOWN)
	{
		DBG_DUMP("Down Key\r\n");
		Ux_PostEvent(NVTEVT_KEY_DOWN, 1, NVTEVT_KEY_PRESS);
		return TRUE;
	}
}
static BOOL Cmd_key_right(CHAR *strCmd)
{
	//if(uiKeyTmpMsk & FLGkey_RIGHT)
	{
		DBG_DUMP("Right Key\r\n");
		Ux_PostEvent(NVTEVT_KEY_RIGHT, 1, NVTEVT_KEY_PRESS);
		return TRUE;
	}
}
static BOOL Cmd_key_left(CHAR *strCmd)
{
	//if(uiKeyTmpMsk & FLGkey_LEFT)
	{
		DBG_DUMP("Left Key\r\n");
		Ux_PostEvent(NVTEVT_KEY_LEFT, 1, NVTEVT_KEY_PRESS);
		return TRUE;
	}
}
static BOOL Cmd_key_enter(CHAR *strCmd)
{
	//if(uiKeyTmpMsk & FLGkey_ENTER)
	{
		DBG_DUMP("Enter Key\r\n");
		Ux_PostEvent(NVTEVT_KEY_ENTER, 1, NVTEVT_KEY_PRESS);
		return TRUE;
	}
}
static BOOL Cmd_key_zoomin(CHAR *strCmd)
{
	//if(uiKeyTmpMsk & FLGkey_ZOOMIN)
	{
		DBG_DUMP("Zoom In Key\r\n");
		Ux_PostEvent(NVTEVT_KEY_ZOOMIN, 1, NVTEVT_KEY_PRESS);
		return TRUE;
	}
}
static BOOL Cmd_key_zoomout(CHAR *strCmd)
{
	//if(uiKeyTmpMsk & FLGkey_ZOOMOUT)
	{
		DBG_DUMP("Zoom Out Key\r\n");
		Ux_PostEvent(NVTEVT_KEY_ZOOMOUT, 1, NVTEVT_KEY_PRESS);
		return TRUE;
	}
}

static BOOL Cmd_key_zoomin_release(CHAR *strCmd)
{
	//if(uiKeyTmpMsk & FLGkey_ZOOMIN)
	{
		DBG_DUMP("Zoom In Key release\r\n");
		Ux_PostEvent(NVTEVT_KEY_ZOOMIN, 1, NVTEVT_KEY_RELEASE);
		return TRUE;
	}
}

static BOOL Cmd_key_zoomout_release(CHAR *strCmd)
{
	//if(uiKeyTmpMsk & FLGkey_ZOOMOUT)
	{
		DBG_DUMP("Zoom Out Key release\r\n");
		Ux_PostEvent(NVTEVT_KEY_ZOOMOUT, 1, NVTEVT_KEY_RELEASE);
		return TRUE;
	}
}

static BOOL Cmd_key_zoomrelease(CHAR *strCmd)
{
	{
		DBG_DUMP("zoom release Key\r\n");
		Ux_PostEvent(NVTEVT_KEY_ZOOMRELEASE, 1, NVTEVT_KEY_ZOOMRELEASE);
		return TRUE;
	}
}

static BOOL Cmd_key_shutter1(CHAR *strCmd)
{
	//if(uiKeyTmpMsk & FLGkey_SHUTTER1)
	{
		DBG_DUMP("Shutter1 Key\r\n");
		Ux_PostEvent(NVTEVT_KEY_SHUTTER1, 1, NVTEVT_KEY_PRESS);
		return TRUE;
	}
}
#if (WIFI_AP_FUNC==ENABLE)
extern void FlowWiFiMovie_Stop2Idle(void);
#endif
extern void FlowPhoto_DoCapture(void);
static BOOL Cmd_key_shutter2(CHAR *strCmd)
{
#if(WIFI_AP_FUNC==ENABLE)
	if (System_GetState(SYS_STATE_CURRSUBMODE) == SYS_SUBMODE_WIFI) {
#if(IPCAM_FUNC!=ENABLE)
		//FlowWiFiMovie_Stop2Idle();
#endif
	}
#endif
	//if(uiKeyTmpMsk & FLGkey_SHUTTER2)
	{
		DBG_DUMP("Shutter2 Key\r\n");
		Ux_PostEvent(NVTEVT_KEY_SHUTTER2, 1, NVTEVT_KEY_PRESS);
		return TRUE;
	}
}

static BOOL Cmd_key_shutter1rel(CHAR *strCmd)
{
	{
		DBG_DUMP("Shutter1 release Key\r\n");
		Ux_PostEvent(NVTEVT_KEY_SHUTTER1, 1, NVTEVT_KEY_RELEASE);
		return TRUE;
	}
}

static BOOL Cmd_key_shutter2rel(CHAR *strCmd)
{
	{
		DBG_DUMP("Shutter2 release Key\r\n");
		Ux_PostEvent(NVTEVT_KEY_SHUTTER2, 1, NVTEVT_KEY_RELEASE);
		return TRUE;
	}
}
#if(WIFI_AP_FUNC==ENABLE)
#include "UIMenuWndWiFiWait.h"
static BOOL Cmd_key_i(CHAR *strCmd)
{
	DBG_DUMP("I Key\r\n");

#if(IPCAM_FUNC!=ENABLE)
//#NT#2016/03/23#Isiah Chang -begin
//#NT#add new Wi-Fi UI flow.
#if(WIFI_UI_FLOW_VER == WIFI_UI_VER_2_0)
	if (UI_GetData(FL_WIFI_LINK) == WIFI_LINK_NG) {
		BKG_PostEvent(NVTEVT_BKW_WIFI_ON);
	}
#else
	Ux_OpenWindow(&UIMenuWndWiFiWaitCtrl, 0);
#endif
//#NT#2016/03/23#Isiah Chang -end
#endif
	BKG_PostEvent(NVTEVT_BKW_WIFI_ON);
	return TRUE;

}
#endif
static BOOL Cmd_key_next(CHAR *strCmd)
{
	DBG_DUMP("next Key\r\n");
	Ux_PostEvent(NVTEVT_KEY_NEXT, 1, NVTEVT_KEY_PRESS);
	return TRUE;
}
//#NT#2016/03/16#Niven Cho -begin
//#NT#add key previous
static BOOL Cmd_key_prev(CHAR *strCmd)
{
	DBG_DUMP("prev Key\r\n");
	Ux_PostEvent(NVTEVT_KEY_PREV, 1, NVTEVT_KEY_PRESS);
	return TRUE;
}
//#NT#2016/03/16#Niven Cho -end
static BOOL Cmd_key_select(CHAR *strCmd)
{
	DBG_DUMP("select Key\r\n");
	Ux_PostEvent(NVTEVT_KEY_SELECT, 1, NVTEVT_KEY_PRESS);
	return TRUE;
}
static BOOL Cmd_key_cut(CHAR *strCmd)
{
#if(MOVIE_MODE==ENABLE)
	UINT32 ircut = 0;

	sscanf_s(strCmd, "%d ", &ircut);
	DBG_DUMP("ircut %d\r\n", ircut);
	Ux_PostEvent(NVTEVT_EXE_MOVIE_IR_CUT,         1,  ircut);
#endif
	return TRUE;
}

#if 0//(CURL_FUNC == ENABLE)
static void CurlCmd_dumpmem(UINT32 Addr, UINT32 size)
{
	UINT32 i, j;
	DBG_DUMP("\r\n Addr=0x%x, size =0x%x\r\n", Addr, size);


	for (j = Addr; j < (Addr + size); j += 0x10) {
		DBG_DUMP("^R 0x%8x:", j);
		for (i = 0; i < 16; i++) {
			DBG_DUMP("0x%2x,", *(char *)(j + i));
		}
		DBG_DUMP("\r\n");
	}
	DBG_DUMP("Data %s\r\n", (char *)Addr);

}


static void curl_rw_cb(UINT32 MessageID, UINT32 Status, UINT32 isContine, UINT32 Param3)
{
	UINT32 *bContue = (UINT32 *)isContine;

	if (bContue) {
		// set contine false to stop curl r/w
		//*bContue = FALSE;
	}
}
#endif

static BOOL Cmd_key_curl(CHAR *strCmd)
{
#if 0 //(CURL_FUNC == ENABLE)

#include "NetFs.h"

	static char tmpbuff[0x400000];
	UINT32 size, pos;
	char   ip[60] = "192.168.0.3";
	char   rootdir[10] = "A:\\";
	char   path[60] = "A:\\test.bin";

	//sscanf(strCmd,"%d %d %d %d %d %d %d %d",&g_lviewframeRate,&g_lviewTargetRate,&g_lviewQueueFrame,&g_lviewPort,&g_lviewTaskPri, &g_hwmemcpy, &g_maxJpgSize, &g_reqDelay);
	sscanf(strCmd, "%d %s", &size, path);

	FST_FILE  filehdl;


	NetFs_SetParam(NETFS_PARAM_REMOTE_IP, (UINT32)ip);
	NetFs_SetParam(NETFS_PARAM_ROOT_DIR, (UINT32)rootdir);
	NetFs_SetParam(NETFS_PARAM_USE_SSL, FALSE);

	// test1
	pos = 0;
	filehdl = NetFs_OpenFile(path, FST_OPEN_READ);
	NetFs_SeekFile(filehdl, 0, FST_SEEK_SET);
	size = 0x200000;
	NetFs_ReadFile(filehdl, (UINT8 *)tmpbuff, &size, 0, NULL);
	pos += size;
	NetFs_SeekFile(filehdl, pos, FST_SEEK_SET);
	size = 0x200000;
	NetFs_ReadFile(filehdl, (UINT8 *)tmpbuff + pos, &size, 0, curl_rw_cb);
	pos += size;
	NetFs_CloseFile(filehdl);

	size = 0x200000;
	filehdl = FileSys_OpenFile("A:\\CURL\\test1.bin", FST_OPEN_WRITE | FST_CREATE_ALWAYS);
	FileSys_WriteFile(filehdl, (UINT8 *)tmpbuff, &size, 0, NULL);
	FileSys_CloseFile(filehdl);

	pos = 0;
	filehdl = NetFs_OpenFile("A:\\CURL\\testuplo.bin", FST_OPEN_WRITE | FST_CREATE_ALWAYS);

	NetFs_SetUploadSrcFilePath(filehdl, "A:\\test.bin");
	size = 0x100000;
	NetFs_WriteFile(filehdl, (UINT8 *)tmpbuff + pos, &size, 0, curl_rw_cb);
	pos += size;
	size = 0x000080;
	NetFs_WriteFile(filehdl, (UINT8 *)tmpbuff + pos, &size, 0, NULL);
	pos += size;
	size = 0x000080;
	NetFs_WriteFile(filehdl, (UINT8 *)tmpbuff + pos, &size, 0, NULL);
	pos += size;
	size = 0x0fff00;
	NetFs_WriteFile(filehdl, (UINT8 *)tmpbuff + pos, &size, 0, NULL);
	pos += size;
	NetFs_CloseFile(filehdl);


	/*

	// test2
	pos = 0;
	filehdl = NetFs_OpenFile(path,FST_OPEN_READ);
	NetFs_SeekFile(filehdl,0,FST_SEEK_SET);
	size = 0x10000;
	NetFs_ReadFile(filehdl, (UINT8 *)tmpbuff, &size, 0, NULL);
	pos += size;
	NetFs_SeekFile(filehdl,pos,FST_SEEK_SET);
	size = 0x10000;
	NetFs_ReadFile(filehdl, (UINT8 *)tmpbuff+pos, &size, 0, NULL);
	pos += size;
	NetFs_CloseFile(filehdl);

	size = 0x20000;
	filehdl = FileSys_OpenFile("A:\\CURL\\test2.bin",FST_OPEN_WRITE|FST_CREATE_ALWAYS);
	FileSys_WriteFile(filehdl, (UINT8 *)tmpbuff, &size, 0, NULL);
	FileSys_CloseFile(filehdl);

	// test3
	pos = 0;
	filehdl = NetFs_OpenFile(path,FST_OPEN_READ);
	NetFs_SeekFile(filehdl,0,FST_SEEK_SET);
	size = 0x20000;
	NetFs_ReadFile(filehdl, (UINT8 *)tmpbuff, &size, 0, NULL);
	pos += size;
	NetFs_SeekFile(filehdl,pos,FST_SEEK_SET);
	size = 0x20000;
	NetFs_ReadFile(filehdl, (UINT8 *)tmpbuff+pos, &size, 0, NULL);
	pos += size;
	NetFs_CloseFile(filehdl);

	size = 0x40000;
	filehdl = FileSys_OpenFile("A:\\CURL\\test3.bin",FST_OPEN_WRITE|FST_CREATE_ALWAYS);
	FileSys_WriteFile(filehdl, (UINT8 *)tmpbuff, &size, 0, NULL);
	FileSys_CloseFile(filehdl);

	// test4
	pos = 0;
	filehdl = NetFs_OpenFile(path,FST_OPEN_READ);
	size = 0x50000;
	NetFs_ReadFile(filehdl, (UINT8 *)tmpbuff, &size, 0, NULL);
	pos += size;
	size = 0x50000;
	NetFs_ReadFile(filehdl, (UINT8 *)tmpbuff+pos, &size, 0, NULL);
	pos += size;
	NetFs_CloseFile(filehdl);

	size = 0xA0000;
	filehdl = FileSys_OpenFile("A:\\CURL\\test4.bin",FST_OPEN_WRITE|FST_CREATE_ALWAYS);
	FileSys_WriteFile(filehdl, (UINT8 *)tmpbuff, &size, 0, NULL);
	FileSys_CloseFile(filehdl);
	*/
#endif
	return TRUE;
}

//#NT#2018/03/16#Niven Cho -begin
//#NT#FORMAT_ROOTFS
static BOOL Cmd_key_format_rootfs(CHAR* strCmd)
{
	System_OnStrg_Format_RootFs();
	return TRUE;
}
//#NT#2018/03/16#Niven Cho -end

SXCMD_BEGIN(key, "key command")
SXCMD_ITEM("power", Cmd_key_power, "power key")
SXCMD_ITEM("delete", Cmd_key_delete, "delete key")
SXCMD_ITEM("menu", Cmd_key_menu, "menu key")
SXCMD_ITEM("mode", Cmd_key_mode, "mode key")
SXCMD_ITEM("playback", Cmd_key_playback, "playback key")
SXCMD_ITEM("movie", Cmd_key_movie, "movie key")
SXCMD_ITEM("up", Cmd_key_up, "up key")
SXCMD_ITEM("down", Cmd_key_down, "down key")
SXCMD_ITEM("right", Cmd_key_right, "right key")
SXCMD_ITEM("left", Cmd_key_left, "left key")
SXCMD_ITEM("enter", Cmd_key_enter, "enter key")
SXCMD_ITEM("zoomin", Cmd_key_zoomin, "zoomin key pressed")
SXCMD_ITEM("zoomout", Cmd_key_zoomout, "zoomout key pressed")
SXCMD_ITEM("zoominrel", Cmd_key_zoomin_release, "zoomin key released")
SXCMD_ITEM("zoomoutrel", Cmd_key_zoomout_release, "zoomout key released")
SXCMD_ITEM("zoomrel", Cmd_key_zoomrelease, "zoom key released")
SXCMD_ITEM("shutter1", Cmd_key_shutter1, "shutter1 key pressed")
SXCMD_ITEM("shutter2", Cmd_key_shutter2, "suutter2 key pressed")
SXCMD_ITEM("shutter1rel", Cmd_key_shutter1rel, "shutter1 key released")
SXCMD_ITEM("shutter2rel", Cmd_key_shutter2rel, "shutter2 key released")
#if(WIFI_AP_FUNC==ENABLE)
SXCMD_ITEM("i", Cmd_key_i, "i key pressed")
#endif
SXCMD_ITEM("next", Cmd_key_next, "next key")
SXCMD_ITEM("select", Cmd_key_select, "select key")
SXCMD_ITEM("cut %", Cmd_key_cut, "cut key")
SXCMD_ITEM("curl %", Cmd_key_curl, "curl key")
//#NT#2016/03/16#Niven Cho -begin
//#NT#add key previous
SXCMD_ITEM("prev", Cmd_key_prev, "prev key")
//#NT#2016/03/16#Niven Cho -end
//#NT#2018/03/16#Niven Cho -begin
//#NT#FORMAT_ROOTFS
SXCMD_ITEM("format_rootfs", Cmd_key_format_rootfs, "format rootfs")
//#NT#2018/03/16#Niven Cho -end
SXCMD_END()

