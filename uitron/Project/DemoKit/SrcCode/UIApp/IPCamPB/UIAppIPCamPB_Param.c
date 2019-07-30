#include <stdio.h>
#include "Debug.h"
#include "PrjCfg.h"
#include "UIAppIPCamPB_Param.h"
#include "WavStudioTsk.h"
#include "ImageUnit_StreamReceiver.h"
#include "ImageUnit_AudDec.h"
///////////////////////////////////////////////////////////////////////////////
#define __MODULE__          UIAppIPCamParam
#define __DBGLVL__          2 // 0=FATAL, 1=ERR, 2=WRN, 3=UNIT, 4=FUNC, 5=IND, 6=MSG, 7=VALUE, 8=USER
#define __DBGFLT__          "*" //*=All, [mark]=CustomClass
#include "DebugModule.h"
///////////////////////////////////////////////////////////////////////////////


static UIAPPIPCAMPB_AUDIO_INFO    g_AudioOutInfo = { AUDDEC_DECODER_PCM, AUDIO_CH_RIGHT, AUDIO_SR_48000, 100 };

static WAVSTUD_INFO_SET g_AudioOutMaxInfo =
	{1,WAVSTUD_DATA_MODE_PUSH,{AUDIO_SR_48000,AUDIO_CH_STEREO,16,2},10,1};

static STMRCV_MAX_MEM_INFO g_StreamReceiverMaxInfo = {
	//uiAudChannels, uiAudioSR       uiAudBits uiAudBufUnitTime
	2,               AUDIO_SR_48000, 16,       1,               0
};

static NMI_AUDDEC_MAX_MEM_INFO  g_AudioDecMaxInfo = {
	//uiAudCodec,        uiAudChannels,             uiAudioSR
	AUDDEC_DECODER_ULAW, 2, 48000, 0, 0
};

UIAPPIPCAMPB_AUDIO_INFO *UIAppIPCamPB_get_AudioOutInfo(void)
{

	return &g_AudioOutInfo;

}

void UIAppIPCamPB_set_AudioOutInfo(UIAPPIPCAMPB_AUDIO_INFO *pAudioOutInfo)
{
	DBG_FUNC_BEGIN("\r\n");

	if (pAudioOutInfo != NULL) {
		memcpy(&g_AudioOutInfo, pAudioOutInfo, sizeof(UIAPPIPCAMPB_AUDIO_INFO));
	} else {
		DBG_ERR("pAudioOutInfo is null\r\n");
	}


	DBG_FUNC_END("\r\n");
}


WAVSTUD_INFO_SET * UIAppIPCamPB_get_AudioOutMaxInfo(void){

	return &g_AudioOutMaxInfo;
}

STMRCV_MAX_MEM_INFO * UIAppIPCamPB_get_StreamReceiverMaxInfo(void){

	return &g_StreamReceiverMaxInfo;
}

NMI_AUDDEC_MAX_MEM_INFO * UIAppIPCamPB_get_AudioDecMaxInfo(void)
{
	return &g_AudioDecMaxInfo;
}