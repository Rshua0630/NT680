#ifndef _NVT_RTP_SINK_HH
#define _NVT_RTP_SINK_HH

#include "MultiFramedRTPSink.hh"
#include "NvtStreamFramer.hh"
#include "NvtMgr.hh"

class NvtRTPSink: public MultiFramedRTPSink
{
public:
	typedef struct _INIT_DATA {
		UsageEnvironment *env;
		Groupsock *RTPgs;
		unsigned char rtpPayloadType;
		unsigned rtpTimestampFrequency;
		char const *rtpPayloadFormatName;
		unsigned numChannels; //default: 1
		u_int32_t uiMediaSrcID;
		void *pParentSms;
	} INIT_DATA;

protected:
	NvtRTPSink(INIT_DATA *pInit);
	virtual ~NvtRTPSink();

protected: // redefined virtual functions:
	virtual Boolean sourceIsCompatibleWithUs(MediaSource &source);

	virtual void doSpecialFrameHandling(unsigned fragmentationOffset,
										unsigned char *frameStart,
										unsigned numBytesInFrame,
										struct timeval framePresentationTime,
										unsigned numRemainingBytes);

	virtual Boolean frameCanAppearAfterPacketStart(unsigned char const *frameStart,
			unsigned numBytesInFrame) const;

	virtual void NotifyFinishSendPacket();

protected:
	NvtStreamFramer::RTP_PACKET *m_pRtpPacket;
	NvtStreamFramer *m_pFramer;
	void *m_pParentSms;
	u_int32_t m_uiMediaSrcID;
	NVT_MEDIA_TYPE m_MediaType;
	u_int32_t m_RtpHdrSize;

public:
	void SetForceStop();
	void ResetBuffer();
	unsigned int GetDstAddress(); //wifiteam
	NvtStreamFramer* GetFramer();

public:
	static void OnSendErrorFunc(void *clientData);
};

class NvtRTPSinkVideo: public NvtRTPSink
{
protected:
	NvtRTPSinkVideo(INIT_DATA *pInit);
	virtual ~NvtRTPSinkVideo();

public:
	char const *sdpMediaType() const
	{
		return "video";
	}
};

class NvtRTPSinkAudio: public NvtRTPSink
{
protected:
	NvtRTPSinkAudio(INIT_DATA *pInit);
	virtual ~NvtRTPSinkAudio();

public:
	char const *sdpMediaType() const
	{
		return "audio";
	}
};

class NvtRTPSinkMJPG: public NvtRTPSinkVideo
{
public:
	static NvtRTPSinkMJPG *createNew(INIT_DATA *pInit);

protected: // redefined virtual functions:
	virtual unsigned specialHeaderSize() const;

	NvtRTPSinkMJPG(INIT_DATA *pInit);
	virtual ~NvtRTPSinkMJPG();

private: // redefined virtual functions:
	virtual void doSpecialFrameHandling(unsigned fragmentationOffset,
										unsigned char *frameStart,
										unsigned numBytesInFrame,
										struct timeval framePresentationTime,
										unsigned numRemainingBytes);

private:
	unsigned int m_FrameOffset;
	unsigned m_LastnumBytesInFrame;
	u_int8_t  m_QTblTmp[NVT_RTP_HDR_SIZE_MJPG];
};

class NvtRTPSinkH264: public NvtRTPSinkVideo
{
public:
	static NvtRTPSinkH264 *createNew(INIT_DATA *pInit);

protected: // redefined virtual functions:
	virtual char const *auxSDPLine();

	NvtRTPSinkH264(INIT_DATA *pInit);
	virtual ~NvtRTPSinkH264();

private:
	char *m_FmtpSDPLine;
};

class NvtRTPSinkH265 : public NvtRTPSinkVideo
{
public:
	static NvtRTPSinkH265 *createNew(INIT_DATA *pInit);

protected: // redefined virtual functions:
	virtual char const *auxSDPLine();

	NvtRTPSinkH265(INIT_DATA *pInit);
	virtual ~NvtRTPSinkH265();

private:
	unsigned removeH264or5EmulationBytes(u_int8_t *to, unsigned toMaxSize,
										 u_int8_t const *from, unsigned fromSize);

private:
	char *m_FmtpSDPLine;
};

class NvtRTPSinkPCM: public NvtRTPSinkAudio
{
public:
	static NvtRTPSinkPCM *createNew(INIT_DATA *pInit);

protected: // redefined virtual functions:
	NvtRTPSinkPCM(INIT_DATA *pInit);
	virtual ~NvtRTPSinkPCM();
};

class NvtRTPSinkAAC: public NvtRTPSinkAudio
{
public:
	static NvtRTPSinkAAC *createNew(INIT_DATA *pInit);

protected: // redefined virtual functions:
	virtual unsigned specialHeaderSize() const;
	virtual char const *auxSDPLine();
	NvtRTPSinkAAC(INIT_DATA *pInit);
	virtual ~NvtRTPSinkAAC();

private:
	virtual void doSpecialFrameHandling(unsigned fragmentationOffset,
										unsigned char *frameStart,
										unsigned numBytesInFrame,
										struct timeval framePresentationTime,
										unsigned numRemainingBytes);

private:
	char *m_FmtpSDPLine;
};

class NvtRTPSinkG711: public NvtRTPSinkAudio
{
public:
	static NvtRTPSinkG711 *createNew(INIT_DATA *pInit);

protected: // redefined virtual functions:
	NvtRTPSinkG711(INIT_DATA *pInit);
	virtual ~NvtRTPSinkG711();
};

class NvtRTPSinkMeta: public NvtRTPSink
{
public:
	static NvtRTPSinkMeta *createNew(INIT_DATA *pInit);

protected:

	NvtRTPSinkMeta(INIT_DATA *pInit);
	virtual ~NvtRTPSinkMeta();

public:
	char const *sdpMediaType() const
	{
		return "application";
	}
};


#endif
