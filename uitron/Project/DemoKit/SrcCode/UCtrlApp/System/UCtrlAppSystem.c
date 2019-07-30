#include "AppControl.h"
#include "SysCommon.h"
#include "UIFramework.h"
#include "PrjCfg.h"
#include "uToken.h"
#include "UIInfo.h"
#include "UIControl.h"
#include "UIControlWnd.h"
#include "UCtrlAppSystem.h"
#include "NvtUctrlAPI.h"
#include "NvtIpcAPI.h"
#include "UCtrlMain.h"
#include "UISetup.h"
#include "nvtmpp.h"
#if (FWS_FUNC == ENABLE)
#include "FwSrvApi.h"
#endif
#include "HfsNvtAPI.h"
#include "UIAppNetwork.h"
#include "UIAppMovie.h"
#include "ImageUnit_NetRTSP.h"
#include "ImageApp_IPCam.h"
#include "ProjectInfo.h"
#include "modelext_info.h"
#include "bin_info.h"
#include "DrvExt.h"
#if(WIFI_FUNC==ENABLE)
//#NT#2016/06/15#Charlie Chang -begin
//#NT# support wifi cmd
#include "WifiCmdParser.h"
#include "WifiAppCmd.h"
//#NT#2016/06/15#Charlie Chang -end
#endif
#if (MSDCVENDOR_NVT == ENABLE)
#include "MsdcNvtCb.h"
#endif
#define THIS_DBGLVL         2 // 0=FATAL, 1=ERR, 2=WRN, 3=UNIT, 4=FUNC, 5=IND, 6=MSG, 7=VALUE, 8=USER
///////////////////////////////////////////////////////////////////////////////
#define __MODULE__          UCtrlAppSys
#define __DBGLVL__          ((THIS_DBGLVL>=PRJ_DBG_LVL)?THIS_DBGLVL:PRJ_DBG_LVL)
#define __DBGFLT__          "*" //*=All, [mark]=CustomClass
#include "DebugModule.h"


#define SXCMD_TEST          0

#define UCTRLAPPSYS_FLG_NET_INITED  FLGPTN_BIT(0)   // notified from linux said net init is ok


#define WIFIRET_LAST_DATA '1'
#define WIFIRET_CONTINUE_DATA '2'
#define WIFI_CMD_ROOT        '/'
#define WIFI_CMD_CUSTOM_TAG  '?'

typedef struct _UCtrlSysParmType {
	UINT32 isSet;
} UCtrlSysParmType;

typedef struct _UCtrlSysParmFWType {
	UINT32 isSet;
	UINT32 fwSize;
} UCtrlSysParmFWType;

typedef struct _UCtrlPortsetParmType {
	UINT32 isSet;
	UINT32 http_port;
	UINT32 https_port;
	UINT32 rtsp_port;
} UCtrlPortsetParmType;

typedef struct _UCtrlAppDisableType {
	UINT32 isSet;
	BOOL http_server;
	BOOL rtsp_server;
} UCtrlAppDisableType;

typedef struct _UCtrlMsdcNvtType {
	UINT32 isSet;
	BOOL bEn;
} UCtrlMsdcNvtType;

typedef struct _UCtrlIspSudoType {
	UINT32 isSet;
	BOOL bEn;
} UCtrlIspSudoType;

typedef struct _UCtrlSysInfoType {
	BOOL                  isErr;
	UCtrlSysParmType      fwupdate;
	UCtrlSysParmType      shutdown;
	UCtrlSysParmType      reboot;
	UCtrlSysParmType      builddate;
	UCtrlSysParmType      core2fwaddr;
	UCtrlSysParmFWType    core2fwready;
	UCtrlSysParmType      core2fwupflag;
	UCtrlPortsetParmType  portset;
	UCtrlAppDisableType   app_disable;
	UCtrlMsdcNvtType      msdcnvt;
	UCtrlIspSudoType      ispsudo;
	//#NT#2018/03/14#Niven Cho -begin
    //#NT#FORMAT_ROOTFS
    UCtrlSysParmType      rootfs_broken;
    //#NT#2018/03/14#Niven Cho -end
	int                   paramcnt;
} UCtrlSysInfoType;

static UCtrlSysInfoType   sysInfo;
static UCtrlSysInfoType  *pSysInfo = &sysInfo;

extern UINT32 _SECTION(".kercfg_data") UCTRLAPPSYS_FLG_ID;
UINT32 UCTRLAPPSYS_FLG_ID = 0;

static INT32 param_portset(INT32 argc, char *argv[])
{
	if (argc != 3) {
		DBG_ERR("need [rtsp port] [http port] [https port]\r\n");
		return -1;
	}
	pSysInfo->portset.rtsp_port = atoi(argv[0]);
	pSysInfo->portset.https_port = atoi(argv[1]);
	pSysInfo->portset.http_port = atoi(argv[2]);
	pSysInfo->portset.isSet = 1;
	pSysInfo->paramcnt++;
	return 0;
}

static INT32 param_rtsp_portget(INT32 argc, char *argv[])
{
#if(FORCE_UNPLUG_RTSP != ENABLE)
	char retStr[64] = {0};

	UINT32 rtsp_port = ImageUnit_GetParam(&ISF_NetRTSP, ISF_CTRL, NETRTSP_PARAM_PORT);

	DBG_DUMP("rtsp port=%d\r\n", rtsp_port);
	snprintf(retStr, sizeof(retStr), "%d", rtsp_port);
	NvtUctrl_SetRetString(retStr);
#else
	DBG_ERR("not support !!\r\n");
#endif
	return 0;
}

static INT32 param_fwupdate(INT32 argc, char *argv[])
{
	pSysInfo->fwupdate.isSet = 1;
	pSysInfo->paramcnt++;

	return 0;
}

static INT32 param_shutdown(INT32 argc, char *argv[])
{
	pSysInfo->shutdown.isSet = 1;
	pSysInfo->paramcnt++;
	return 0;
}

static INT32 param_reboot(INT32 argc, char *argv[])
{
	pSysInfo->reboot.isSet = 1;
	pSysInfo->paramcnt++;
	return 0;
}

static INT32 param_notify(INT32 argc, char *argv[])
{
	if (strcmp("net_init_ok", argv[0]) == 0) {
		set_flg(UCTRLAPPSYS_FLG_ID, UCTRLAPPSYS_FLG_NET_INITED);
	}
	return 0;
}

/* Build date query */
static INT32 param_builddate(INT32 argc, char *argv[])
{
	pSysInfo->builddate.isSet = 1;
	pSysInfo->paramcnt++;
	return 0;
}

/* Core2 query firmware buffer address */
static INT32 param_core2fwaddr(INT32 argc, char *argv[])
{
	pSysInfo->core2fwaddr.isSet = 1;
	pSysInfo->paramcnt++;
	return 0;
}

/* Core2 notify firmware is ready in buffer */
static INT32 param_core2fwready(INT32 argc, char *argv[])
{
	if (argc != 1) {
		DBG_ERR("argc!=1 (%d)\r\n", argc);
		return -1;
	}

	pSysInfo->core2fwready.isSet = 1;
	pSysInfo->core2fwready.fwSize = atoi(argv[0]);
	pSysInfo->paramcnt++;
	return 0;
}

/* Core2 query firmware upgrade flag, check if firmware upgraded */
static INT32 param_core2fwupflag(INT32 argc, char *argv[])
{
	pSysInfo->core2fwupflag.isSet = 1;
	pSysInfo->paramcnt++;
	return 0;
}
//#NT#2016/05/18#David Tsai -begin
//#NT# Support http and rtsp service disable in booting time
static INT32 param_app_disable(INT32 argc, char *argv[])
{
	if (argc != 2) {
		DBG_ERR("argc!=2 (%d)\r\n", argc);
		return -1;
	}

	pSysInfo->app_disable.isSet = 1;
	pSysInfo->app_disable.http_server = atoi(argv[0]);
	pSysInfo->app_disable.rtsp_server = atoi(argv[1]);

	return 0;
}
//#NT#2016/05/18#David Tsai -end

#define MAX_LINUX_POOL 32

typedef struct
{
	char name[32];
	BOOL enable;
	NVTMPP_VB_POOL sPool;
}LINUX_POOL_INFO;
static LINUX_POOL_INFO sLinuxPool[MAX_LINUX_POOL]= {0};

static INT32 param_bufget(INT32 argc, char *argv[]){

	if (argc != 2) {
		DBG_ERR("argc!=2 (%d)\r\n", argc);
		return -1;
	}
	UINT32 uiBufSize = atoi(argv[1]);
	char Name[32] ={0};
	if(strlen(argv[0]) < sizeof(Name)){
		memcpy(Name, argv[0], strlen(argv[0]));
	}
	else{
		memcpy(Name, argv[0], sizeof(Name));
	}


	int i=0;
	NVTMPP_VB_BLK  blk;
	UINT32 buf_addr=0;
	UINT32 uiPhyAddr = 0;
	char RetStr[32];
	///check same pool
	for(i=0 ; i<MAX_LINUX_POOL ; i++){

		if(sLinuxPool[i].enable == FALSE){
			continue;
		}
		if(strcmp(sLinuxPool[i].name,Name) == 0 ){
			DBG_WRN("Pool Name is already enable\r\n");
			return -1;
		}
	}

	for(i=0;i<MAX_LINUX_POOL; i++ ){

		if(sLinuxPool[i].enable == TRUE){
			continue;
		}
		memcpy(sLinuxPool[i].name, Name, sizeof(Name));
		DBG_DUMP("charlie i:%d name:%s size:%d\r\n",i,sLinuxPool[i].name,uiBufSize);
		sLinuxPool[i].sPool = nvtmpp_vb_create_pool(sLinuxPool[i].name, uiBufSize, 1, NVTMPP_DDR_1);
		if(NVTMPP_VB_INVALID_POOL == sLinuxPool[i].sPool){
			DBG_WRN("create pool fail\r\n");
			return -1;
		}
		blk = nvtmpp_vb_get_block(0, sLinuxPool[i].sPool, uiBufSize, NVTMPP_DDR_1);
		if(blk == NVTMPP_VB_INVALID_BLK){
			DBG_WRN("create blk fail\r\n");
			if(nvtmpp_vb_destroy_pool(sLinuxPool[i].sPool) < 0){
				DBG_ERR("destroy pool fail!!\r\n");
			}
			return -1;

		}
		sLinuxPool[i].enable = TRUE;
		buf_addr = nvtmpp_vb_block2addr(blk);
		uiPhyAddr = NvtIPC_GetPhyAddr(buf_addr);
		sprintf(RetStr,"%d %d",uiPhyAddr,uiBufSize);
		NvtUctrl_SetRetString(RetStr);
		return 0;
	}

	DBG_WRN("All Pools are already Enable, can not create the new pool\r\n");
	return -1;
}

static INT32 param_bufrel(INT32 argc, char *argv[]){

	if (argc != 1) {
		DBG_ERR("argc!=1 (%d)\r\n", argc);
		return -1;
	}
	char Name[32]={0};


	if(strlen(argv[0]) < sizeof(Name)){
		memcpy(Name, argv[0], strlen(argv[0]));
	}
	else{
		memcpy(Name, argv[0], sizeof(Name));
	}


	int i=0;
	for(i=0; i < MAX_LINUX_POOL; i++){

		if(sLinuxPool[i].enable == FALSE){
			continue;
		}
		if(strcmp(Name,sLinuxPool[i].name) == 0){
			if(nvtmpp_vb_destroy_pool(sLinuxPool[i].sPool) < 0){
				DBG_ERR("destroy pool fail!!\r\n");
				return -1;
			}
			sLinuxPool[i].enable = FALSE;
			return 0;
		}

	}
	DBG_WRN("can not find the pool name %s!!\r\n",Name);
	return -1;

}


//#NT#2017/08/14#Charlie Chang -begin
//#NT# support hot plug mem to linux
static INT32 param_dynamic_mem(INT32 argc, char *argv[]){


	if (argc != 1) {
		DBG_ERR("argc!=1 (%d)\r\n", argc);
		return -1;
	}
	UINT32 ret=0;
	ret = nvtmpp_vb_init();
	if (NVTMPP_ER_OK != ret) {
		DBG_ERR("nvtmpp init vb err: %d\r\n", ret);
		return -1;
	}

	UINT32 uiAlignSize = atoi(argv[0]);
	char RetStr[32];
	DBG_DUMP("hotplug alignSize=%d\r\n",uiAlignSize);
	INT32  iFreeSize = nvtmpp_vb_get_max_free_size(NVTMPP_DDR_1) - HOT_PLUG_REMAIN_SIZE;
	NVTMPP_VB_POOL vb_pool;
	NVTMPP_VB_BLK  blk;
	UINT32 buf_addr=0;
	UINT32 uiPhyAddr = 0;
//	UINT32 uiFinalAddr = 0;

	DBG_DUMP("free:%d align:%d\r\n",iFreeSize,uiAlignSize);
	if(iFreeSize <=0){
		DBG_WRN("not enough mem!!!\r\n");
		return-1;
	}

	vb_pool = nvtmpp_vb_create_pool("LinuxPool", iFreeSize, 1, NVTMPP_DDR_1);
	if (NVTMPP_VB_INVALID_POOL == vb_pool) {
		DBG_ERR("create private pool err\r\n");
		return -1;
	}
	blk = nvtmpp_vb_get_block(0, vb_pool, iFreeSize, NVTMPP_DDR_1);
	if (NVTMPP_VB_INVALID_BLK == blk) {
		if(nvtmpp_vb_destroy_pool(vb_pool) < 0){
			DBG_ERR("destroy pool fail!!\r\n");
		}
		DBG_ERR("get vb block err\r\n");
		return -1;
	}
	buf_addr = nvtmpp_vb_block2addr(blk);
	uiPhyAddr = NvtIPC_GetPhyAddr(buf_addr);
	sprintf(RetStr,"%d %d",uiPhyAddr,iFreeSize);
	#if 0
	////check alignment
		if((uiPhyAddr%uiAlignSize) != 0){

			uiFinalAddr = ((uiPhyAddr/uiAlignSize)+1)*uiAlignSize;
			iFreeSize = iFreeSize - (uiFinalAddr - uiPhyAddr);

			if(iFreeSize <=0){
				DBG_WRN("aling(%x)  original_addr=%x final_addr=%x size=%d, buf size not enough\r\n",
					uiPhyAddr,uiFinalAddr,iFreeSize);
				nvtmpp_vb_destroy_pool(vb_pool);
				sprintf(RetStr,"0 0");

			}
			else{
				iDevideValue = iFreeSize/uiAlignSize;
				if(iDevideValue == 0){
					DBG_WRN("aling(%x) original_addr=%x final_addr=%x size=%d, buf size not enough!\r\n",
					uiPhyAddr,uiFinalAddr,iFreeSize);
					nvtmpp_vb_destroy_pool(vb_pool);
					sprintf(RetStr,"0 0");
				}
				else{
					sprintf(RetStr,"%d %d",uiFinalAddr,iDevideValue*uiAlignSize);
				}
			}
		}
		else{
			sprintf(RetStr,"%d %d",NvtIPC_GetPhyAddr(buf_addr),uiAlignSize*iDevideValue);
		}


	}
	#endif
	DBG_DUMP("(addr,size):%s\r\n",RetStr);
	NvtUctrl_SetRetString(RetStr);
    return 0;
}
//#NT#2017/08/14#Charlie Chang -end

//#NT#2016/08/05#Niven Cho -begin
//#NT#MSDC-NET
static INT32 param_msdcnvt(INT32 argc, char *argv[])
{
	if (argc != 1) {
		DBG_ERR("argc!=1 (%d)\r\n", argc);
		return -1;
	}

	pSysInfo->msdcnvt.isSet = pSysInfo->ispsudo.isSet = 1;
	pSysInfo->msdcnvt.bEn = ((atoi(argv[0]) & 0x01) != 0) ? TRUE : FALSE;
	pSysInfo->ispsudo.bEn = ((atoi(argv[0]) & 0x02) != 0) ? TRUE : FALSE;

	return 0;
}
//#NT#2016/08/05#Niven Cho -end

//#NT#2018/03/14#Niven Cho -begin
//#NT#FORMAT_ROOTFS
static INT32 param_rootfs_broken(INT32 argc, char* argv[])
{
    pSysInfo->rootfs_broken.isSet = 1;
    return 0;
}
//#NT#2018/03/14#Niven Cho -end

static INT32 param_begin(char *str, INT32 len)
{
	DBG_IND("get: %s\r\n", str);
	memset(pSysInfo, 0, sizeof(UCtrlSysInfoType));
	return 0;
}
static void system_poweroff(void)
{
	DBG_IND("system_poweroff\r\n");
	Ux_PostEvent(NVTEVT_SYSTEM_SHUTDOWN, 1, 0);
}
static void system_reboot(void)
{
	DBG_IND("system_reboot\r\n");
#if defined(_CLOCK_RTC_)
	GxSystem_EnableHWReset(0);
#elif defined(_CLOCK_DRTC_)
	GxSystem_EnableSWReset(0); //it may cause damage on peripheral, cause their power are not be reset here.
	DBG_WRN("Using SW Reset, it might be not safe for peripheral.\r\n");
#else
	DBG_FATAL("unknown CLOCK setting?\r\n");
#endif
	Ux_PostEvent(NVTEVT_SYSTEM_SHUTDOWN, 1, 0);
}
static INT32 param_end(UTOKEN_END_INFO *p_info)
{
	char retStr[64];
#if ONVIF_FUNC
	static BOOL onvifStartFlag = FALSE;
#endif

	if (p_info->err_code < 0) {
		snprintf(retStr, sizeof(retStr) - 1, "ERR: parm %s", p_info->failed_name);
		NvtUctrl_SetRetString(retStr);
		DBG_ERR("%s\r\n", retStr);
		return p_info->err_code;
	}

	DBG_IND("param cnt %d\r\n", pSysInfo->paramcnt);

	if (pSysInfo->paramcnt > 1) {
		snprintf(retStr, sizeof(retStr) - 1, "ERR: parm cnt %d > 1", pSysInfo->paramcnt);
		NvtUctrl_SetRetString(retStr);
		DBG_ERR("%s\r\n", retStr);
		return -1;
	}

	if (pSysInfo->fwupdate.isSet) {
		UctrlMain_SendEvent(NVTEVT_EXE_FWUPDATE, 0);
	}
	if (pSysInfo->shutdown.isSet) {
		system_poweroff();

	}
	if (pSysInfo->reboot.isSet) {
		system_reboot();
	}

	/* Reture build date string to core 2 */
	if (pSysInfo->builddate.isSet) {
		char retStr[64];

		snprintf(retStr, sizeof(retStr), "%s", Prj_GetCheckinDate());
		DBG_IND("reStr= %s\r\n", retStr);
		NvtUctrl_SetRetString(retStr);
	}

	/* Return firmware buffer address to core2 */
	if (pSysInfo->core2fwaddr.isSet) {
		char retStr[64];

		if (System_GetState(SYS_STATE_CURRMODE) != PRIMARY_MODE_MAIN) {
			DBG_ERR("Not in Main Mode.\r\n");
			snprintf(retStr, sizeof(retStr), "0 0");
		} else {
			//because FwSrv need 4 word alignment source bin address
			UINT32 uiAddr = ALIGN_CEIL_16(OS_GetMempoolAddr(POOL_ID_APP));
			UINT32 uiSize = OS_GetMempoolSize(POOL_ID_APP) - (uiAddr - OS_GetMempoolAddr(POOL_ID_APP));
			snprintf(retStr, sizeof(retStr), "%lu %lu", NvtIPC_GetPhyAddr(uiAddr), uiSize);
		}

		DBG_IND("reStr= %s\r\n", retStr);
		NvtUctrl_SetRetString(retStr);
	}

	/* Core2 upload firmware ready notify */
	if (pSysInfo->core2fwready.isSet) {
#if (FWS_FUNC == ENABLE)
#if _TODO
		FWSRV_CMD Cmd = {0};
		/* Firmware upload ready, start upgrade */
		DBG_DUMP("receive firmware ready notify, fwSize is %lu!\r\n", pSysInfo->core2fwready.fwSize);
		System_CPU2_Stop();
		FWSRV_BIN_UPDATE_ALL_IN_ONE FwInfo = {0};
		FwInfo.uiSrcBufAddr = ALIGN_CEIL_16(OS_GetMempoolAddr(POOL_ID_APP));
		FwInfo.uiSrcBufSize = pSysInfo->core2fwready.fwSize;
		FwInfo.uiWorkBufAddr = FwInfo.uiSrcBufAddr + ALIGN_CEIL_16(FwInfo.uiSrcBufSize); //because FwSrv need 4 word alignment source bin address
		FwInfo.uiWorkBufSize = OS_GetMempoolSize(POOL_ID_APP) - (FwInfo.uiSrcBufAddr - OS_GetMempoolAddr(POOL_ID_APP)) - ALIGN_CEIL_16(FwInfo.uiSrcBufSize); //for FwSrv reload to check sum
		FwInfo.fpItemCb = NULL;
		memset(&Cmd, 0, sizeof(Cmd));
		Cmd.Idx = FWSRV_CMD_IDX_BIN_UPDATE_ALL_IN_ONE;
		Cmd.In.pData = &FwInfo;
		Cmd.In.uiNumByte = sizeof(FwInfo);
		Cmd.Prop.bExitCmdFinish = TRUE;
		FWSRV_ER fws_er = FwSrv_Cmd(&Cmd);
		if (fws_er != FWSRV_ER_OK) {
			DBG_ERR("FW bin write failed %d\r\n", fws_er);
			system_reboot();
			return fws_er;
		}
		system_reboot();
#endif
#endif
	}

	if (pSysInfo->core2fwupflag.isSet) {
		char retStr[64];
#if(BOOT_RESET_MENU_INFO == ENABLE)
		if (SysGetFlag(FL_CPU2_DEFAULT)) {
			SysSetFlag(FL_CPU2_DEFAULT, 0);
			Save_MenuInfo();
			snprintf(retStr, sizeof(retStr), "1");
		} else {
			snprintf(retStr, sizeof(retStr), "0");
		}

		DBG_IND("reStr= %s\r\n", retStr);
#else
		MODELEXT_HEADER *head = NULL;
		BININFO *pBinInfo = (BININFO *)Dx_GetModelExtCfg(MODELEXT_TYPE_BIN_INFO, &head);
		UINT32 uiFWUpdate = (pBinInfo->ld.LdCtrl2 & LDCF_UPDATE_FW);
		if (uiFWUpdate == 0) {
			snprintf(retStr, sizeof(retStr), "0");
		} else {
			snprintf(retStr, sizeof(retStr), "1");
		}
#endif
		NvtUctrl_SetRetString(retStr);
	}
	if (pSysInfo->portset.isSet) {

		DBG_DUMP("set rtsp port=%d http_port=%d https_port=%d\r\n",
				 pSysInfo->portset.rtsp_port,
				 pSysInfo->portset.http_port,
				 pSysInfo->portset.https_port);
		////set hfs server
		UINet_HFSUnInit();
		UINet_HFSPortSet(pSysInfo->portset.http_port,
						 pSysInfo->portset.https_port);
		UINet_HFSInit();

		/////set rtsp server
#if(FORCE_UNPLUG_RTSP != ENABLE)
		UINT32 rtsp_port = ImageUnit_GetParam(&ISF_NetRTSP, ISF_CTRL, NETRTSP_PARAM_PORT);
		if (rtsp_port != (UINT32)pSysInfo->portset.rtsp_port) {
			ImageUnit_Begin(&ISF_NetRTSP, 0);
			ImageUnit_SetParam(ISF_CTRL, NETRTSP_PARAM_PORT, pSysInfo->portset.rtsp_port);
			ImageUnit_End();
			ImageApp_IPCam_update_all_path();
		}
		snprintf(retStr, sizeof(retStr), "OK");
		NvtUctrl_SetRetString(retStr);
#endif

		/////starup onvif
#if ONVIF_FUNC
		if (onvifStartFlag == FALSE) {
			UINet_OnvifInit();
			onvifStartFlag = TRUE;
		}
#endif
	}

	if (pSysInfo->app_disable.isSet) {
		if (pSysInfo->app_disable.http_server != 0) {
			DBG_DUMP("\r\n!!!!!Disable hfs!!!!!\r\n");
			UINet_DisableHFS(FALSE);
		}
		if (pSysInfo->app_disable.rtsp_server != 0) {
			DBG_DUMP("\r\n!!!!!Disable rtsp!!!!!\r\n");
			ImageApp_IPCam_Config(IPCAM_CFG_PLUG_RTSP, 0);
		}
	}

#if (MSDCVENDOR_NVT == ENABLE)
	if (pSysInfo->msdcnvt.isSet) {
		MsdcNvtCb_Net(pSysInfo->msdcnvt.bEn);
	}
	if (pSysInfo->ispsudo.isSet && System_GetState(SYS_STATE_CURRMODE) != PRIMARY_MODE_MAIN) {
		IspSudoCb_Net(pSysInfo->ispsudo.bEn);
	}
#endif

	//#NT#2018/03/14#Niven Cho -begin
    //#NT#FORMAT_ROOTFS
    if (pSysInfo->rootfs_broken.isSet)
    {
		DBG_ERR("root-fs are broken, please rescue it.\r\n");
    }
	//#NT#2018/03/14#Niven Cho -end

	return 0;
}

//#NT#2016/10/18#Ben Wang -begin
//#NT#Refine uctrl wifi cmd
static INT32 param_wificmdinit(INT32 argc, char *argv[])
{
#if _TODO
	DBG_IND("\r\n");
	WifiAppCmd_init();
	WifiCmd_ReceiveCmd(1);
#endif
	return 0;
}
static INT32 param_wificmd(INT32 argc, char *argv[])
{
#if _TODO
	char RetStr[16];
	char   *pch;
	char buf[2048] = {0};
	//char *pCmdStr;

	int buf_size;

	char mimeType[64] = {0};
	sprintf(mimeType, "text/xml");
	int ret = 0;
	static int index = 0;

	if (argc < 1 || argv[0] == NULL) {
		DBG_ERR("argc error  argc=%d\r\n", argc);
		return 0;
	}
	DBG_IND("cmd=%s\r\n", argv[0]);

	pch = strchr((char *)argv[0], WIFI_CMD_CUSTOM_TAG);
	if (NULL == pch || *argv[0] != WIFI_CMD_ROOT) {
		DBG_WRN("Not a valid cmd (%s)\r\n", argv[0]);
		return 0;
	}
	//the argument "path" of WifiCmd_GetData is the string prior to '?'
	*pch = 0;
	buf_size = sizeof(buf);
//	memset(buf,0,sizeof(buf));
	buf_size--; //reserve the first for continue flag
	ret = WifiCmd_GetData(argv[0], pch + 1, (UINT32)buf + 1, (UINT32 *)&buf_size, mimeType, index);
	buf_size++;//add first byte
	if (ret == 0) { ///not continue data
		index = 0;
		memset(buf, WIFIRET_LAST_DATA, 1); ////set continue flag: 1 means last data
	} else if (ret == 1) { /// continue data
		index ++;
		memset(buf, WIFIRET_CONTINUE_DATA, 1); ////set continue flag:2 means continue data
	} else {
		index = 0;
		buf_size = 0;
		DBG_ERR("WifiCmd_GetDataerror ret=%d\r\n", ret);
	}
	DBG_IND("buf_size=%d\r\n", buf_size);
	NvtUctrl_SetConfigData((void *)&buf, buf_size);
	snprintf(RetStr, sizeof(RetStr), "%d", buf_size);
	NvtUctrl_SetRetString(RetStr);
	if (THIS_DBGLVL > 1) {
		*(buf + sizeof(buf) - 1)  = 0;
		DBG_DUMP("XML Return:\r\n%s\r\n", buf);
	}


#endif
	return 0;
}
//#NT#2016/10/18#Ben Wang -end

static INT32 param_unknown(char *name)
{
	DBG_ERR("unknown param: %s\r\n", name);
	return 0;
}

UTOKEN_PARAM tbl_sys_param[] = {
	{"fwupdate", param_fwupdate},
	{"shutdown", param_shutdown},
	{"reboot", param_reboot},
	{"notify", param_notify},
	{"builddate", param_builddate},
	{"core2fwaddr", param_core2fwaddr},
	{"core2fwready", param_core2fwready},
	{"core2fwupflag", param_core2fwupflag},
	{"portset", param_portset},
	{"rtsp_portget", param_rtsp_portget},
	{"app_disable", param_app_disable},
	{"wificmdinit", param_wificmdinit},
	{"wificmd", param_wificmd},
	{"msdcnvt", param_msdcnvt},
	{"hotplugmem",param_dynamic_mem}, //// get dynamic mem for linux
	//#NT#2018/03/14#Niven Cho -begin
    //#NT#FORMAT_ROOTFS
    {"rootfs_broken",param_rootfs_broken},
	{"bufget",param_bufget},
	{"bufrel",param_bufrel},
    //#NT#2018/03/14#Niven Cho -end
	{NULL, NULL}, //last tag, it must be
};

UTOKEN_MODULE module_sys = {
	"usys",
	tbl_sys_param,
	param_begin,
	param_end,
	param_unknown,
	NULL
};

#if SXCMD_TEST

static BOOL Cmd_usys_set(CHAR *strCmd)
{
	uToken_Parse(strCmd, &module_sys);
	return TRUE;
}

SXCMD_BEGIN(usys, "usys")
SXCMD_ITEM("set %", Cmd_usys_set, "")
SXCMD_END()

#endif

void UCtrlAppSys_InstallCmd(void)
{
#if SXCMD_TEST
	SxCmd_AddTable(usys);
#endif
}

void UCtrlAppSys_InstallID(void)
{
	OS_CONFIG_FLAG(UCTRLAPPSYS_FLG_ID);
}

void UCtrlAppSys_WaitNetInitFinish(void)
{
	if (UCTRLAPPSYS_FLG_ID == 0) {
		DBG_ERR("need to UCtrlAppSys_InstallID()\r\n");
		return;
	}

	FLGPTN FlgPtn;
	wai_flg(&FlgPtn, UCTRLAPPSYS_FLG_ID, UCTRLAPPSYS_FLG_NET_INITED, TWF_ORW | TWF_CLR);
}
