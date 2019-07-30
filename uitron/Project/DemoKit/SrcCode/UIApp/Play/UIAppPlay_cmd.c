//global debug level: PRJ_DBG_LVL
#include "PrjCfg.h"

#include "SysCommon.h"
#include "SysCfg.h"
#include "AppLib.h"
#include "UIAppCommon.h"
#include "UIModePlayback.h"
//#NT#2017/05/08#Adam Su -begin
//#NT#Movie Play Demo Test
#include "ImageApp_MoviePlay.h"
#include "ImageApp_Play.h"
#include "PlaybackTsk.h"
#include "SysKer.h" // for OS_GetMempoolAddr
#include "SMediaPlayAPI.h" // for SMEDIAPLAY_SPEED_NORMAL & SMEDIAPLAY_DIR_FORWARD
#include "UIAppPlay.h"
#include "UIFlowPlayIcons.h"
#include "GxVideoFile.h"

//local debug level: THIS_DBGLVL
#define THIS_DBGLVL         2 // 0=FATAL, 1=ERR, 2=WRN, 3=UNIT, 4=FUNC, 5=IND, 6=MSG, 7=VALUE, 8=USER
///////////////////////////////////////////////////////////////////////////////
#define __MODULE__          UiAppPlayCmd
#define __DBGLVL__          ((THIS_DBGLVL>=PRJ_DBG_LVL)?THIS_DBGLVL:PRJ_DBG_LVL)
#define __DBGFLT__          "*" //*=All, [mark]=CustomClass
#include "DebugModule.h"
///////////////////////////////////////////////////////////////////////////////


#define                   FULL_FILE_PATH_LEN  64
#define                   _Struct_Without_UnitStream_  DISABLE//ENABLE // Tmp solution for Demo
static FST_FILE           g_pFileHdl          = NULL;
#if 0//(_Struct_Without_UnitStream_ == ENABLE)
static SMEDIAPLAY_USEROBJ gMovie_Play_Info = {0};
#else
static MOVIEPLAY_FILEPLAY_INFO gMovie_Play_Info = {0};
#endif
//#NT#2017/05/08#Adam Su -end

MOVIEPLAY_DISP_INFO  gUIAppPlayDispConfig[4] = {
    //enable     disp_id         w     h      ratio     rotate dir
	{TRUE, MOVIEPLAY_DISP_ID_1, 960,  240  ,  4 ,  3  ,  0},
	{TRUE, MOVIEPLAY_DISP_ID_2, 960,  240  ,  4 ,  3  ,  0},
	{TRUE, MOVIEPLAY_DISP_ID_3, 960,  240  ,  4 ,  3  ,  0},
	{TRUE, MOVIEPLAY_DISP_ID_4, 960,  240  ,  4 ,  3  ,  0},
};

static void UIAppPlay_SetDispInfo(void)
{
	MOVIEPLAY_DISP_INFO	*p_disp_info = NULL;
	USIZE				device_ratio;

	device_ratio = GxVideo_GetDeviceAspect(0);

	p_disp_info = &gUIAppPlayDispConfig[0];
	p_disp_info->enable = TRUE;
	p_disp_info->width  =  0;
	p_disp_info->height	=  0;
	p_disp_info->width_ratio  =  device_ratio.w;
	p_disp_info->height_ratio =  device_ratio.h;
	p_disp_info->rotate_dir   =  SysVideo_GetDir();
	ImageApp_MoviePlay_Config(MOVIEPLAY_CONFIG_DISP_INFO, (UINT32)p_disp_info);
}

//#NT#2017/05/08#Adam Su -begin
//#NT#Movie Play Demo Test
static BOOL Cmd_key_MoviePlay_Open(CHAR *strCmd)
{
	char     pFilePath[FULL_FILE_PATH_LEN];
	UINT32   uiMPDirID, uiMPFileID;
	UINT32	 pool_address = 0;

	if (g_pFileHdl) {
		FileSys_CloseFile(g_pFileHdl);
		g_pFileHdl = NULL;
	}

	// Get file path
	sscanf_s(strCmd, "%d %d", &uiMPDirID, &uiMPFileID); // {100, 1, 128(DCF_FILE_TYPE_MP4)},
	sprintf(pFilePath, "A:\\DCIM\\%dNVTIM\\IMAG%04d.MP4", uiMPDirID, uiMPFileID);
	DBG_DUMP("Play File Path = %s\r\n", pFilePath);

	// Open Test Media File
	g_pFileHdl = FileSys_OpenFile(pFilePath, FST_OPEN_READ);
	if (!g_pFileHdl) {
		DBG_ERR("Can't open Video file!\r\n");
	}

#if 0//(_Struct_Without_UnitStream_ == ENABLE)
	UINT32   ret;

	gCamMovie_Play_Info.hActMediafilehdl = g_pFileHdl;
	gCamMovie_Play_Info.CallBackFunc     = NULL;
	gCamMovie_Play_Info.uiMemAddr        = (UINT32)OS_GetMempoolAddr(POOL_ID_APP);
	gCamMovie_Play_Info.uiMemSize        = (UINT32)OS_GetMempoolSize(POOL_ID_APP);
	gCamMovie_Play_Info.uiCurrSpeed      = SMEDIAPLAY_SPEED_NORMAL;
	gCamMovie_Play_Info.uiCurrDirect     = SMEDIAPLAY_DIR_FORWARD;

	ImageApp_CamMoviePlay_Config(CAMMOVIEPLAY_CONFIG_FILEPLAY_INFO, (UINT32) &gCamMovie_Play_Info);

	ret = ImageApp_CamMoviePlay_Open();

	if (ret != ISF_OK) {
		DBG_ERR("CamMoviePlay Open Error!!!\r\n");
	}
#else
	pool_address = ImageUnit_GetParam(&ISF_Play, ISF_CTRL, PLAY_PARAM_POOL_ADDRESS);
	if (pool_address == 0) {
		FileSys_CloseFile(g_pFileHdl);
		g_pFileHdl = NULL;
		DBG_ERR("Movie play memory pool is NULL\r\n");
		return FALSE;
	}

	gMovie_Play_Info.fileplay_id = _CFG_FILEPLAY_ID_1;
	//gMovie_Play_Info.mem_addr    = pool_address;
	//gMovie_Play_Info.mem_size    = (UINT32)OS_GetMempoolSize(POOL_ID_APP) - (pool_address - (UINT32)OS_GetMempoolAddr(POOL_ID_APP));
	gMovie_Play_Info.file_handle = g_pFileHdl;
	gMovie_Play_Info.curr_speed  = SMEDIAPLAY_SPEED_NORMAL;
	gMovie_Play_Info.curr_direct = SMEDIAPLAY_DIR_FORWARD;
	gMovie_Play_Info.event_cb    = Play_MovieCB;

    // Config max mem info
    GXVIDEO_INFO VidInfo = {0};
    SMEDIAPLAY_MAX_MEM_INFO SMPMaxMemInfo = {0};

    PB_GetParam(PBPRMID_INFO_VDO, (UINT32*) &VidInfo);
    SMPMaxMemInfo.FileFormat = MEDIA_FILEFORMAT_MOV;
    SMPMaxMemInfo.VidFR = VidInfo.uiVidRate;
    SMPMaxMemInfo.AudFR = VidInfo.AudInfo.uiSR / 1024;
    SMPMaxMemInfo.TotalSecs = VidInfo.uiToltalSecs;
    SMPMaxMemInfo.CodecType = VidInfo.uiVidType;
    SMPMaxMemInfo.ImageWidth = VidInfo.uiVidWidth;
    SMPMaxMemInfo.ImageHeight = VidInfo.uiVidHeight;
    gMovie_Play_Info.pmax_mem_info = &SMPMaxMemInfo;

	ImageApp_MoviePlay_Config(MOVIEPLAY_CONFIG_FILEPLAY_INFO, (UINT32) &gMovie_Play_Info);

	// Config rotation info
	UIAppPlay_SetDispInfo();

	ImageApp_Play_Close();

	ImageApp_MoviePlay_Open();

	FlowPB_UpdateIcons(0);

#endif

	return TRUE;
}
static BOOL Cmd_key_MoviePlay_Close(CHAR *strCmd)
{
	if (g_pFileHdl) {
		FileSys_CloseFile(g_pFileHdl);
		g_pFileHdl = NULL;
	}

	ImageApp_MoviePlay_Close();

	return TRUE;
}
//#NT#2017/05/08#Adam Su -end


SXCMD_BEGIN(apppb, "playback command")
//#NT#2017/05/08#Adam Su -begin
//#NT#Movie Play Demo Test
SXCMD_ITEM("mpo %", Cmd_key_MoviePlay_Open,  "MoviePlay Open")
SXCMD_ITEM("mpc",   Cmd_key_MoviePlay_Close, "MoviePlay Close")
//#NT#2017/05/08#Adam Su -end
SXCMD_END()

void UIAppPlay_InstallCmd(void)
{
	SxCmd_AddTable(apppb);
}
