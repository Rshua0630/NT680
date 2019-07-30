////////////////////////////////////////////////////////////////////////////////
#include "SysCommon.h"
//#include "AppCommon.h"
////////////////////////////////////////////////////////////////////////////////
#include "UIAppCommon.h"

#include "NVTUserCommand.h"
#include "GxDisplay.h"
#include "AppLib.h"


#include "UIMode.h"
#include "PrjCfg.h"

#include "SysMain.h"
#include "AppControl.h"

#include "ImageApp_IPCamPB.h"
#include "nvtmpp.h"
#include "UIAppIPCamPB_Param.h"


#define THIS_DBGLVL        2 // 0=FATAL, 1=ERR, 2=WRN, 3=UNIT, 4=FUNC, 5=IND, 6=MSG, 7=VALUE, 8=USER
///////////////////////////////////////////////////////////////////////////////
#define __MODULE__          UiAppIPCamPB
#define __DBGLVL__          ((THIS_DBGLVL>=PRJ_DBG_LVL)?THIS_DBGLVL:PRJ_DBG_LVL)
#define __DBGFLT__          "*" //*=All, [mark]=CustomClass
//#define __DBGFLT__          "[cap]"
//#define __DBGFLT__          "[cb]"
//#define __DBGFLT__          "[set]"
//#define __DBGFLT__          "[zoom]"
//#define __DBGFLT__          "[af]"
#include "DebugModule.h"



#if 1
static INT32 IPCamPBExe_InitCommonMem(void)
{


	NVTMPP_VB_CONF_S st_conf;
	NVTMPP_ER        ret;
	memset(&st_conf, 0, sizeof(NVTMPP_VB_CONF_S));
	st_conf.ddr_mem[0].Addr = OS_GetMempoolAddr(POOL_ID_APP);
	st_conf.ddr_mem[0].Size = OS_GetMempoolSize(POOL_ID_APP);

	st_conf.max_pool_cnt = 64;

	st_conf.common_pool[0].blk_size = 1024;
	st_conf.common_pool[0].blk_cnt = 1;
	st_conf.common_pool[0].ddr = NVTMPP_DDR_1;

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

	SxCmd_DoCommand("nvtmpp status"); //show init buffer status

	return 0;

}
#endif
extern SX_CMD_ENTRY ipcampb[];

static void IPCamPBExe_AudioSet(void){


	UIAPPIPCAMPB_AUDIO_INFO *pAudioOutInfo = UIAppIPCamPB_get_AudioOutInfo();
	WAVSTUD_INFO_SET *wsis_max = UIAppIPCamPB_get_AudioOutMaxInfo();
	STMRCV_MAX_MEM_INFO* pStrmRcvMaxInfo  = UIAppIPCamPB_get_StreamReceiverMaxInfo();
	NMI_AUDDEC_MAX_MEM_INFO *pAudioDecMaxBuf = UIAppIPCamPB_get_AudioDecMaxInfo();
	AUDOUT_MAX_MEM_INFO AudOutMaxInfo  = {wsis_max, 0};


	ImageUnit_Begin(&ISF_StreamReceiver, 0);
	ImageUnit_SetParam(ISF_IN1, STMRCV_PARAM_CODEC, pAudioOutInfo->codec);
	ImageUnit_SetParam(ISF_IN1, STMRCV_PARAM_CHS, pAudioOutInfo->channel); //MONO always left channel for EVB
	ImageUnit_SetParam(ISF_IN1, STMRCV_PARAM_SAMPLERATE, pAudioOutInfo->SampleRate);
	ImageUnit_SetParam(ISF_IN1, STMRCV_PARAM_MAX_MEM_INFO, (UINT32)pStrmRcvMaxInfo);
	ImageUnit_End();

	ImageUnit_Begin(&ISF_AudDec, 0);
	ImageUnit_SetParam(ISF_IN1, AUDDEC_PARAM_MAX_MEM_INFO, (UINT32)pAudioDecMaxBuf);
	DBG_DUMP("charlie sample:%d ch:%d codec:%d\r\n",pAudioOutInfo->SampleRate,pAudioOutInfo->channel,pAudioOutInfo->codec);
	ImageUnit_SetParam(ISF_IN1, AUDDEC_PARAM_SAMPLERATE, pAudioOutInfo->SampleRate);
	ImageUnit_SetParam(ISF_IN1, AUDDEC_PARAM_CHANNELS, pAudioOutInfo->channel);
	ImageUnit_SetParam(ISF_IN1, AUDDEC_PARAM_CODEC, pAudioOutInfo->codec);
	ImageUnit_End();

	ImageUnit_Begin(&ISF_AudOut, 0);
	ImageUnit_SetParam(ISF_CTRL, AUDOUT_PARAM_VOL_IMM, pAudioOutInfo->volume);
	ImageUnit_SetParam(ISF_OUT1, AUDOUT_PARAM_CHANNEL, pAudioOutInfo->channel);
	ImageUnit_SetParam(ISF_OUT1, AUDOUT_PARAM_MAX_MEM_INFO, (UINT32)&AudOutMaxInfo);
	ImageUnit_End();
}


INT32 IPCamPBExe_OnOpen(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
	DBG_FUNC_BEGIN("\r\n");

	DBG_DUMP("IPcamPlayBack mode open\r\n");

	SxCmd_AddTable(ipcampb);

	IPCamPBExe_InitCommonMem();

	IPCamPBExe_AudioSet();

	Ux_DefaultEvent(pCtrl, NVTEVT_EXE_OPEN, paramNum, paramArray);

	ImageApp_IPCamPB_Open();


	//ImageApp_IPCamPB_AllStatusSet(ISF_PORT_STATE_RUN);
	DBG_FUNC_END("\r\n");
	return NVTEVT_CONSUME;
}
static INT32 IPCamPBExe_ExitCommonMem(void)
{
	NVTMPP_ER        ret;

	SxCmd_DoCommand("nvtmpp status"); //show exit buffer status

	ret = nvtmpp_vb_exit();
	if (NVTMPP_ER_OK != ret) {
		DBG_ERR("nvtmpp init vb err: %d\r\n", ret);
		return -1;
	}
	return 0;
}

INT32 IPCamPBExe_OnClose(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
	DBG_FUNC_BEGIN("\r\n");
	DBG_DUMP("IPcamPlayBack mode close\r\n");

	Ux_DefaultEvent(pCtrl, NVTEVT_EXE_CLOSE, paramNum, paramArray);
	ImageApp_IPCamPB_Close();

	if (IPCamPBExe_ExitCommonMem() < 0) {
		return NVTEVT_CONSUME;
	}
	DBG_FUNC_END("\r\n");
	return NVTEVT_CONSUME;
}

EVENT_ENTRY CustomIPCamPBObjCmdMap[] = {
	{NVTEVT_EXE_OPEN,                 IPCamPBExe_OnOpen                     },
	{NVTEVT_EXE_CLOSE,                IPCamPBExe_OnClose                    },
	{NVTEVT_NULL,                     0},  //End of Command Map
};

CREATE_APP(CustomIPCamPBObj, APP_SETUP)

