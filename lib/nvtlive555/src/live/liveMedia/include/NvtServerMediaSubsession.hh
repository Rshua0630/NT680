#ifndef _NVT_SERVER_MEDIA_SUBSESSION_HH
#define _NVT_SERVER_MEDIA_SUBSESSION_HH

#include "FileServerMediaSubsession.hh"
#include "NvtStreamFramer.hh"
#include "NvtRTPSink.hh"
#include "NvtMgr.hh"

class NvtServerMediaSubsession: public FileServerMediaSubsession
{
public:
	typedef struct _INIT_DATA {
		UsageEnvironment *env;
		char const *FileName;
		u_int32_t uiMediaSrcID;
		NVT_MEDIA_TYPE MediaType;
		NVT_VIDEO_INFO VideoInfo;
		NVT_AUDIO_INFO AudioInfo;
		NVTLIVE555_URL_INFO UrlInfo;
	} INIT_DATA;

public:
	static NvtServerMediaSubsession *createNew(INIT_DATA *pInit);

protected:
	NvtServerMediaSubsession(INIT_DATA *pInit);

	virtual ~NvtServerMediaSubsession();

protected: // redefined virtual functions
	virtual void seekStreamSource(FramedSource *inputSource, double &seekNPT, double streamDuration, u_int64_t &numBytes);
	virtual FramedSource *createNewStreamSource(unsigned clientSessionId, unsigned &estBitrate);
	virtual RTPSink *createNewRTPSink(Groupsock *rtpGroupsock, unsigned char rtpPayloadTypeIfDynamic, FramedSource *inputSource);
	virtual float duration() const;

private:
	char  m_FileName[256];
	NvtStreamFramer *m_pFarmer;
	NvtRTPSink *m_pRTPSink;
	NvtServerMediaSubsession *m_pPartnerSms;
	NVT_MEDIA_TYPE m_MediaType;
	u_int32_t m_uiMediaSrcID;
	NVT_VIDEO_INFO m_VideoInfo;
	NVT_AUDIO_INFO m_AudioInfo;
	NVTLIVE555_URL_INFO m_UrlInfo;

public:
	NvtStreamFramer *GetFramer();
	NvtStreamFramer *GetPartnerFramer();
	NvtRTPSink      *GetRTPSink();
	NVT_MEDIA_TYPE GetMediaType();
	NVT_VIDEO_INFO *GetVideoInfo();
	NVT_AUDIO_INFO *GetAudioInfo();
	NVTLIVE555_URL_INFO *GetUrlInfo();
	int RefreshCodecInfo(u_int32_t hStrm);
	void NotifyFramerDeleted();
	void NotifySinkDeleted();
	u_int32_t GetMediaSrcID();
	void SetPartnerSms(NvtServerMediaSubsession *pPartnerSms);
};

#endif
