#ifndef _NVT_MEDIA_CLI_DATA_HH
#define _NVT_MEDIA_CLI_DATA_HH
#include "NvtMedia.hh"
#include "liveMedia.hh"
#include "../mediaServer/include/Live555CliCommon.h"

#ifdef _DEBUG
#define CFG_CLIENT_TIMEOUT_SEC 100000
#else
#define CFG_CLIENT_TIMEOUT_SEC 10
#endif

#define CFG_CHECK_SERVER_VENDOR 1

class NvtMediaCliData
{
public:
	typedef struct _IPC {
		int   use_ipc;
		void *pBufAddr;
		u_int32_t BufSize;
		int ipc_msgid;
	} IPC;

private:
	IPC  m_IPC;
	SEM_HANDLE hSemIPC;
	LIVE555CLI_CONFIG m_Config;
	u_int8_t *m_pBufVideo;
	u_int32_t m_uiBufSizeVideo;
	u_int8_t *m_pBufAudio;
	u_int32_t m_uiBufSizeAudio;
	LIVE555CLI_STREAM_INFO m_StrmInfo;
	void *m_pRTSPClient;

	void xRun_ICmd(LIVE555CLI_ICMD *pCmd);
	u_int32_t xCopyICmdToIPC(LIVE555CLI_ICMD *pCmd);
	u_int32_t xCopyIPCToICmd(LIVE555CLI_ICMD *pCmd);

public: //APIs
	u_int32_t Set_IPC(u_int32_t addr, u_int32_t size);
	u_int32_t Set_Config(LIVE555CLI_CONFIG *pConfig);
	u_int32_t Set_VideoInfo(LIVE555CLI_VIDEO_INFO *pInfo);
	u_int32_t Set_AudioInfo(LIVE555CLI_AUDIO_INFO *pInfo);
	u_int32_t SetNextVideoFrm(LIVE555CLI_FRM *pFrm);
	u_int32_t SetNextAudioFrm(LIVE555CLI_FRM *pFrm);
	void      SetRTSPClientPointer(void *pRTSPClient);
	u_int8_t *Get_BufVideo(u_int32_t *p_size);
	u_int8_t *Get_BufAudio(u_int32_t *p_size);
	void     *GetRTSPClientPointer(void);
	LIVE555CLI_CONFIG *Get_Config();
	LIVE555CLI_CODEC_VIDEO Trans_VideoCodec(const char *streamId);
	LIVE555CLI_CODEC_AUDIO Trans_AudioCodec(const char *streamId);

	NVT_MEDIA_TYPE Get_MediaType(char const *fMediumName);

	u_int8_t *Trans_Phy_To_NonCache(u_int32_t addr);
	u_int8_t *Trans_Phy_To_Cache(u_int32_t addr);
	u_int32_t Trans_To_Phy(void *addr);

	u_int32_t OpenStream();
	u_int32_t CloseStream();

	char  *Get_Url(void);

public:
	NvtMediaCliData();
	virtual ~NvtMediaCliData();
};

#endif
