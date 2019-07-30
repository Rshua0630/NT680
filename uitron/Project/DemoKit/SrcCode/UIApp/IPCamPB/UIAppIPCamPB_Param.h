#ifndef _UIAPPIPCAMPB_Param_H
#define _UIAPPIPCAMPB_Param_H
#include "WavStudioTsk.h"
#include "ImageUnit_StreamReceiver.h"
#include "NMediaPlayAudDec.h"
typedef struct {

	UINT32 codec;
	UINT32 channel;
	UINT32 SampleRate;
	UINT32 volume;


} UIAPPIPCAMPB_AUDIO_INFO;

extern UIAPPIPCAMPB_AUDIO_INFO *UIAppIPCamPB_get_AudioOutInfo(void);

extern void UIAppIPCamPB_set_AudioOutInfo(UIAPPIPCAMPB_AUDIO_INFO *pAudioOutInfo);

extern WAVSTUD_INFO_SET * UIAppIPCamPB_get_AudioOutMaxInfo(void);

extern STMRCV_MAX_MEM_INFO * UIAppIPCamPB_get_StreamReceiverMaxInfo(void);

extern NMI_AUDDEC_MAX_MEM_INFO * UIAppIPCamPB_get_AudioDecMaxInfo(void);

#endif