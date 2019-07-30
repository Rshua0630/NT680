#include "PlaybackTsk.h"
#include "UIAppPlay.h"
#include "UIBackgroundObj.h"
#include "NVTUserCommand.h"
#include "SysCfg.h" //for memory POOL_ID
#include "UIInfo.h"
#include "UIAppCommon.h"
#include "FileDB.h"
#include "GxDisplay.h"
#include "UIAppMovie.h"
#include "PrjCfg.h"
#include "BG_Images.h"
//#include "DPOF.h"
#include "DpofVendor.h"
#include "PBXFileList_DCF.h"
#include "PBXFileList_FileDB.h"
//#NT#2013/05/08#Calvin Chang#Register decode video file callback function -begin
#include "MovieUdtaVendor.h"      // for movie udta (user data) parsing
//#NT#2013/05/08#Calvin Chang -end
#include "SysMain.h"
#if(WIFI_FUNC==ENABLE)
#include "UIModeWifi.h"
#endif
#if(WIFI_AP_FUNC==ENABLE)
#include "RtspNvtApi.h"
#include "WifiAppCmd.h"
#include "WifiAppXML.h"
#include "UIAppWiFiCmd.h"
#endif
//#NT#2016/07/21#Charlie Chang -begin
//#NT# using for get temp data
//static MEM_RANGE g_stream_app_mem;
//#NT#2016/07/21#Charlie Chang -end
#include "ImageUnit_VdoOut.h"
#include "ImageUnit_Play.h"
#include "ImageUnit_ImgTrans.h"
#include "SMediaPlayAPI.h"
#include "ImageApp_Play.h"
#include "dma.h"
#include "nvtmpp.h"
#include "AppDisp_PBView.h"
#include "SysCommon.h"

#define THIS_DBGLVL         2 // 0=FATAL, 1=ERR, 2=WRN, 3=UNIT, 4=FUNC, 5=IND, 6=MSG, 7=VALUE, 8=USER
///////////////////////////////////////////////////////////////////////////////
#define __MODULE__          UIAppPlay
#define __DBGLVL__          ((THIS_DBGLVL>=PRJ_DBG_LVL)?THIS_DBGLVL:PRJ_DBG_LVL)
#define __DBGFLT__          "*" //*=All, [mark]=CustomClass
#include "DebugModule.h"
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////

extern UINT32 System_GetEnableDisp(void);
extern UINT32 System_GetEnableSensor(void);

//#NT#2010/09/10#Ben Wang -begin
//#NT#Refine code for sort by date only
//#NT#2010/08/09#Lincy Lin -begin
//#NT#Add index view for playback
#define FILEDB_SUPPORT_FILENUM   60000
//#NT#2010/08/09#Lincy Lin -end
#define PB_MAX_BUFFER_WIDTH     3840//1920
#define PB_MAX_BUFFER_HEIGHT    2160//1080
#define PB_FILE_FMT             PBFMT_JPG | PBFMT_WAV | PBFMT_AVI | PBFMT_MOVMJPG | PBFMT_MP4 | PBFMT_TS
#define RTSP_PLAY_FUNC          DISABLE


#define MAX_REC_TIME  60//sec
#if 1//(SDHOTPLUG_FUNCTION == ENABLE)
#include "UIStorageCheck.h"
static BOOL m_bIsPbOpened = FALSE;
#endif

extern UINT32 g_DualVideo;

PLAY_DISP_INFO  UIAppPlayDispConfig[2] = {
    //enable  disp_id       w     h      ratio     rotate dir
	{TRUE, PLAY_DISP_ID_1, 960,  240  ,  4 ,  3  ,  0,          0, DOUT1},
	{TRUE, PLAY_DISP_ID_2, 960,  240  ,  4 ,  3  ,  0,          0, DOUT2}
};


static INT32 PlayExe_InitCommonMem(void)
{

	NVTMPP_ER        ret;
	NVTMPP_VB_CONF_S st_conf;

	memset(&st_conf, 0, sizeof(NVTMPP_VB_CONF_S));
	st_conf.ddr_mem[0].Addr = OS_GetMempoolAddr(POOL_ID_APP);
	st_conf.ddr_mem[0].Size = OS_GetMempoolSize(POOL_ID_APP);

	st_conf.max_pool_cnt = 64;

	st_conf.common_pool[0].blk_size = 3840 * 2160 * 3 / 2 + 1024;
	st_conf.common_pool[0].blk_cnt = 4;
	st_conf.common_pool[0].ddr = NVTMPP_DDR_1;

	if (g_DualVideo == (DISPLAY_1|DISPLAY_2)) {
		st_conf.common_pool[1].blk_size = 1920 * 1080 * 3 / 2 + 1024;
		st_conf.common_pool[1].blk_cnt = 4;
		st_conf.common_pool[1].ddr = NVTMPP_DDR_1;
#if (VDO_USE_ROTATE_BUFFER)
		st_conf.common_pool[1].blk_cnt += 2;
#endif
	} else {
#if (VDO_USE_ROTATE_BUFFER)
		st_conf.common_pool[1].blk_size = 1920 * 1080 * 3 / 2 + 1024;
		st_conf.common_pool[1].blk_cnt = 2;
		st_conf.common_pool[1].ddr = NVTMPP_DDR_1;
#endif
	}
	/*st_conf.common_pool[1].blk_size = 640 * 480 * 3 / 2 + 1024;
	st_conf.common_pool[1].blk_cnt = 4;
	st_conf.common_pool[1].ddr = NVTMPP_DDR_1;
	st_conf.common_pool[1].blk_size = 640 * 480 * 3 / 2 + 1024;
	st_conf.common_pool[1].blk_cnt = 4;
	st_conf.common_pool[1].ddr = NVTMPP_DDR_1;*/

	ret = nvtmpp_vb_set_conf(&st_conf);
	if (NVTMPP_ER_OK != ret) {
		DBG_ERR("nvtmpp set vb err: %d\r\n", ret);
		return -1;
	}
	ret = nvtmpp_vb_init();
	if (NVTMPP_ER_OK != ret) {
		DBG_ERR("nvtmpp init vb err: %d\r\n", ret);
		return -1;
	}
	return 0;

}

INT32 PlayExe_ExitCommonMem(void)
{
	NVTMPP_ER        ret;
	ret = nvtmpp_vb_exit();
	if (NVTMPP_ER_OK != ret) {
		DBG_ERR("nvtmpp init vb err: %d\r\n", ret);
		return -1;
	}
	return 0;
}

static void PlayExe_SetDispInfo(PLAY_DISP_ID id, BOOL enable, PUSIZE pratio)
{
	PLAY_DISP_INFO*  p_disp = NULL;

	p_disp = &UIAppPlayDispConfig[id];
	p_disp->enable = enable;

	if (enable) {
		p_disp->vid_out      = PLAY_VID_OUT_1+id;
		p_disp->width        = 0;
		p_disp->height       = 0;
		p_disp->width_ratio  = pratio->w;
		p_disp->height_ratio = pratio->h;
		p_disp->rotate_dir   = SysVideo_GetDirbyID(p_disp->dev_id);
	}
	ImageApp_Play_Config(PLAY_CONFIG_DISP_INFO, (UINT32)p_disp);
}

static void PlayExe_SetDispParam(void)
{
	//PLAY_DISP_INFO*  p_disp = NULL;
	USIZE DeviceRatio;

#if (DISPLAY2_FUNC == ENABLE)
	if (g_DualVideo == DISPLAY_1) {
		DBG_IND("Device 1\r\n");

		DeviceRatio = GxVideo_GetDeviceAspect(DOUT1);
		PlayExe_SetDispInfo(PLAY_DISP_ID_1, TRUE, &DeviceRatio);
		PlayExe_SetDispInfo(PLAY_DISP_ID_2, FALSE, &DeviceRatio);

		ImageApp_Play_Config(PLAY_CONFIG_DUAL_DISP, FALSE);

	} else if (g_DualVideo == DISPLAY_2) {
		DBG_IND("Device 2\r\n");

		DeviceRatio = GxVideo_GetDeviceAspect(DOUT2);
		PlayExe_SetDispInfo(PLAY_DISP_ID_1, FALSE, &DeviceRatio);
		PlayExe_SetDispInfo(PLAY_DISP_ID_2, TRUE, &DeviceRatio);

		ImageApp_Play_Config(PLAY_CONFIG_DUAL_DISP, TRUE);

	} else if (g_DualVideo == (DISPLAY_1|DISPLAY_2)) {
		DBG_IND("Device 1+2\r\n");

		DeviceRatio = GxVideo_GetDeviceAspect(DOUT1);
		PlayExe_SetDispInfo(PLAY_DISP_ID_1, TRUE, &DeviceRatio);

		DeviceRatio = GxVideo_GetDeviceAspect(DOUT2);
		PlayExe_SetDispInfo(PLAY_DISP_ID_2, TRUE, &DeviceRatio);

		ImageApp_Play_Config(PLAY_CONFIG_DUAL_DISP, TRUE);

	}
#else
	PLAY_DISP_INFO*  p_disp = NULL;

	DeviceRatio = GxVideo_GetDeviceAspect(DOUT1);

	p_disp = &UIAppPlayDispConfig[0];
	p_disp->enable = TRUE;
	p_disp->width  =  0;
	p_disp->height =  0;
	p_disp->width_ratio  =  DeviceRatio.w;
	p_disp->height_ratio =  DeviceRatio.h;
	p_disp->rotate_dir   =  SysVideo_GetDirbyID(p_disp->dev_id);
	p_disp->vid_out      =  PHOTO_VID_OUT_1;
	ImageApp_Play_Config(PLAY_CONFIG_DISP_INFO, (UINT32)p_disp);
#endif
}

static void PlayExe_SetPBParam(void)
{
	USIZE device_size  = {0};
	USIZE device2_size = {0};
	ISIZE tmp_size 	   = {0};

	if (g_DualVideo & DISPLAY_1) {
		tmp_size = GxVideo_GetDeviceSize(DOUT1);
		device_size.h = (UINT32)tmp_size.h;
		device_size.w = (UINT32)tmp_size.w;
		DBG_IND("Device 1 w=%d, h=%d\r\n", tmp_size.w, tmp_size.h);
	}
	if (g_DualVideo & DISPLAY_2) {
		tmp_size = GxVideo_GetDeviceSize(DOUT2);
		device2_size.h = (UINT32)tmp_size.h;
		device2_size.w = (UINT32)tmp_size.w;
		DBG_IND("Device 2 w=%d, h=%d\r\n", device2_size.w, device2_size.h);
	}

	if ((device2_size.h * device2_size.w) > (device_size.h*device_size.w)) {
		device_size.h = device2_size.h;
		device_size.w = device2_size.w;
	}

	ImageUnit_Begin(&ISF_Play, 0);
	ImageUnit_SetParam(ISF_CTRL, PLAY_PARAM_PANELSZ, (UINT32)&device_size);
	ImageUnit_SetParam(ISF_CTRL, PLAY_PARAM_PLAY_FMT, PB_FILE_FMT);

	ImageUnit_End();
}

//#NT#2013/05/08#Calvin Chang#Register decode video file callback function -begin
BOOL PBDecVideoCB(UINT32 uiUserDataAddr, UINT32 uiUserDataSize)
{
	return MovieUdta_ParseVendorUserData(uiUserDataAddr, uiUserDataSize);
}
//#NT#2013/05/08#Calvin Chang -end

/**
  Initialize application for Playback mode

  Initialize application for Playback mode.

  @param void
  @return void
*/
INT32 PlayExe_OnOpen(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
#if (USE_DPOF==ENABLE)
	//MEM_RANGE dpofPool;
#endif
	UINT32 useFileDB = 0;
	//PLAY_OBJ PlayObj;

#if 1//(SDHOTPLUG_FUNCTION == ENABLE)
	if (TRUE == m_bIsPbOpened || UIStorageCheck(STORAGE_CHECK_ERROR, NULL) == TRUE) {
		return NVTEVT_CONSUME;
	}
	m_bIsPbOpened = TRUE;
#endif
	//call default
	Ux_DefaultEvent(pCtrl, NVTEVT_EXE_OPEN, paramNum, paramArray);

	if (PlayExe_InitCommonMem() < 0) {
		return NVTEVT_CONSUME;
	}

	PlayExe_SetDispParam();
	PlayExe_SetPBParam();

#if (USE_DPOF==ENABLE)
	//dpofPool = AppMem_Alloc("DPOF", DPOF_BUFFER_SIZE);
#endif

	//Pool = AppMem_Alloc("ImageStream", APPMEM_ALL);

#if USE_FILEDB
	UI_SetData(FL_IsUseFileDB, 1);
#else
	UI_SetData(FL_IsUseFileDB, 0);
#endif
	useFileDB = UI_GetData(FL_IsUseFileDB);
	if (useFileDB) {
		MEM_RANGE FDBMem;
		CHAR      *rootPath = "A:\\";
		//CHAR*      defaultfolder="A:\\CarDV\\";

		PPBX_FLIST_OBJ  pFlist = PBXFList_FDB_getObject();
		FDBMem.Addr = dma_getCacheAddr(OS_GetMempoolAddr(POOL_ID_FILEDB));
		FDBMem.Size = POOL_SIZE_FILEDB;
		pFlist->Config(PBX_FLIST_CONFIG_MEM, FDBMem.Addr, FDBMem.Size);
		pFlist->Config(PBX_FLIST_CONFIG_MAX_FILENUM, 10000, 0);
		pFlist->Config(PBX_FLIST_CONFIG_MAX_FILEPATH_LEN, 60, 0);
		pFlist->Config(PBX_FLIST_CONFIG_VALID_FILETYPE, PBX_FLIST_FILE_TYPE_JPG | PBX_FLIST_FILE_TYPE_AVI | PBX_FLIST_FILE_TYPE_MOV | PBX_FLIST_FILE_TYPE_MP4 | PBX_FLIST_FILE_TYPE_TS, 0);
		pFlist->Config(PBX_FLIST_CONFIG_DCF_ONLY, FALSE, 0);
		//pFlist->Config(PBX_FLIST_CONFIG_SORT_TYPE,PBX_FLIST_SORT_BY_FILEPATH,0);
		pFlist->Config(PBX_FLIST_CONFIG_SORT_TYPE, PBX_FLIST_SORT_BY_MODDATE, 0);
		pFlist->Config(PBX_FLIST_CONFIG_ROOT_PATH, (UINT32)rootPath, 0);
		pFlist->Config(PBX_FLIST_CONFIG_SUPPORT_LONGNAME, 1, 0);
		//pFlist->Config(PBX_FLIST_CONFIG_DEFAULT_FOLDER,(UINT32)defaultfolder,0);
		//#NT#2016/08/12#Hideo Lin -begin
		//#NT#For small size clone movie (filter clone movie for playback)
#if (SMALL_CLONE_MOVIE == ENABLE)
		// Note: the folder name should match which defined in FlowMovie_FileDBCreate()
		pFlist->Config(PBX_FLIST_CONFIG_ROOT_PATH, (UINT32)DV_ROOT_PATH, 0);
		pFlist->Config(PBX_FLIST_CONFIG_FILTER_FOLDER, (UINT32)PLAY_FOLDER_FILTER, 0);
#endif
		//#NT#2016/08/12#Hideo Lin -end
		PB_SetParam(PBPRMID_FILELIST_OBJ, (UINT32)pFlist);
	} else {
		PB_SetParam(PBPRMID_FILELIST_OBJ, (UINT32)PBXFList_DCF_getObject());
	}
	PB_SetParam(PBPRMID_DEC_VIDEO_CALLBACK, (UINT32)PBDecVideoCB);
	PB_SetParam(PBPRMID_ONDRAW_CALLBACK, (UINT32)PBView_OnDrawCB);

    System_WaitFS();
	//open
	ImageApp_Play_Open();
	UIAppPlay_InstallCmd();

	//#NT#2016/07/20#Charlie Chang -begin
	//#NT# for PlayExe_GetTmpBuf function to get buf
	//g_stream_app_mem.Addr = Pool.Addr;
	//g_stream_app_mem.Size = Pool.Size;
	//#NT#2016/07/20#Charlie Chang -end
#if(WIFI_AP_FUNC==ENABLE)
	if (System_GetState(SYS_STATE_CURRSUBMODE) == SYS_SUBMODE_WIFI) {
#if (RTSP_PLAY_FUNC==ENABLE)
		/*RTSPNVT_OPEN Open = {0};
		Open.uiApiVer = RTSPNVT_API_VERSION;
		Open.Type = RTSPNVT_TYPE_PLAYBACK;
		Open.uiPortNum = 554;
		//#NT#2016/07/13#Charlie Chang -begin
		//#NT# re-allocate rtsp addr and size
		Open.uiWorkAddr = Pool.Addr;
		Open.uiWorkSize = Pool.Size;
		//#NT#2016/07/13#Charlie Chang -end
		RtspNvt_Open(&Open);
		//#NT#2016/07/20#Charlie Chang -begin
		//#NT# for PlayExe_GetTmpBuf function to get buf
		RtspNvt_GetWorkBuf(&g_stream_app_mem);
		//#NT#2016/07/20#Charlie Chang -end
		*/

#endif
		//WifiCmd_Done(WIFIFLAG_MODE_DONE,E_OK);
		Ux_PostEvent(NVTEVT_WIFI_EXE_MODE_DONE, 1, E_OK);
	}
#endif

//#NT#2016/03/18#Niven Cho -begin
//#NT#improve the using of gprof
#if defined(_GPROF_TOOL_ON_)
	//take the place of SxCmd Temp Buffer after ImageApp_Open, becasue it has changed in Open()
	SxCmd_RegTempMemFunc(System_GetGprofBuffer);
#else
	//#NT#2016/07/20#Charlie Chang -begin
	//#NT# using appmem to get tmp buf
	//SxCmd_RegTempMemFunc(PlayExe_GetTmpBuf);
	//#NT#2016/07/20#Charlie Chang -end
#endif
//#NT#2016/03/18#Niven Cho -end
	return NVTEVT_CONSUME;
}

INT32 PlayExe_OnVideoChange(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
	// change to current UI mode again
	Ux_SendEvent(0, NVTEVT_SYSTEM_MODE, 1, System_GetState(SYS_STATE_CURRMODE));
	return NVTEVT_CONSUME;

	//return NVTEVT_PASS; //PASS this event to UIWnd!
}

INT32 PlayExe_OnClose(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
#if 1//(SDHOTPLUG_FUNCTION == ENABLE)
	if (FALSE == m_bIsPbOpened) {
		return NVTEVT_CONSUME;
	}
	m_bIsPbOpened = FALSE;
#endif
#if (WIFI_AP_FUNC==ENABLE)
	if (System_GetState(SYS_STATE_CURRSUBMODE) == SYS_SUBMODE_WIFI) {
#if (RTSP_PLAY_FUNC==ENABLE)
		RtspNvt_Close();
#endif
	}
#endif
#if (USE_DPOF==ENABLE)
	DpofVendor_Close();
#endif

	ImageApp_Play_Close();

#if (VDO_USE_COPY_BUFFER == ENABLE)
 	VdoOut_SetSkipAllocTemp(FALSE);
#endif

	if (PlayExe_ExitCommonMem() < 0) {
		return NVTEVT_CONSUME;
	}
	//call default
	Ux_DefaultEvent(pCtrl, NVTEVT_EXE_CLOSE, paramNum, paramArray);

	return NVTEVT_CONSUME;
}

INT32 PlayExe_OnSleep(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
	ImageApp_Play_SetScreenSleep();

	return NVTEVT_CONSUME;
}

INT32 PlayExe_OnWakeup(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
	ImageApp_Play_SetScreenWakeUp();

	return NVTEVT_CONSUME;
}

////////////////////////////////////////////////////////////

EVENT_ENTRY CustomPlayObjCmdMap[] = {
	{NVTEVT_EXE_OPEN,               PlayExe_OnOpen            },
	{NVTEVT_EXE_CLOSE,              PlayExe_OnClose           },
	//#NT#2012/07/31#Hideo Lin -end
	{NVTEVT_VIDEO_CHANGE,           PlayExe_OnVideoChange     },
	{NVTEVT_EXE_SLEEP,              PlayExe_OnSleep           },
	{NVTEVT_EXE_WAKEUP,             PlayExe_OnWakeup          },
	{NVTEVT_NULL,                   0},  //End of Command Map
};

CREATE_APP(CustomPlayObj, APP_SETUP)

