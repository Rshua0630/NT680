#ifndef _NVT_STREAM_FRAMER_HH
#define _NVT_STREAM_FRAMER_HH

#include "FramedFilter.hh"
#include "NvtMgr.hh"
#include "NvtStreamQueue.hh"

class NvtStreamFramer: public FramedFilter
{
public:
	typedef struct _INIT_DATA {
		UsageEnvironment *env;
		char const *FileName;
		void *pParentSms;
		u_int32_t uiMediaSrcID;
		u_int32_t SessionID;
	} INIT_DATA;

	typedef enum {
		PACKET_STATE_NEW = 0,
		PACKET_STATE_NEW_OR_VPS = 0, //(H265)
		PACKET_STATE_NEW_OR_SPS = 0, //(H264)
		PACKET_STATE_SPS,
		PACKET_STATE_PPS,
		PACKET_STATE_CONT,
	} PACKET_STATE;

	typedef struct _RTP_PACKET {
		unsigned char *pBuf;
		u_int32_t  bLastPacket;//last packet in frame
		u_int8_t **ppReplace; //for video frame only
		Boolean   *pSkipThisSend; //due to MediaRec lag, we skip current send.
		u_int32_t *piSocketIdx;   //NVT_MODIFIED, Multi-UDP Source
		int       *pByteSent;     //NVT_MODIFIED, WIFI-TEAM
		PACKET_STATE NextState; //Next State
	} RTP_PACKET;

public:
	virtual void doGetNextFrame() = 0;
	// called by getNextFrame()

protected:
	NvtStreamFramer(INIT_DATA *pInit);
	virtual ~NvtStreamFramer();

protected:
	// thread for receiving nvt stream sender
	Boolean m_bRecvLoop;
	Boolean m_bExecuted; //avoid seek re-entry
	THREAD_HANDLE m_hRecv; //thread to wait nvt-stream sender
	THREAD_OBJ m_hRecvObj;
	u_int8_t *m_pStack;
	char m_SessionName[16];

	// NvtStreamQueue mechanism	
	u_int32_t m_hStrm; //handle to ISTRM_CB
	NvtStreamQueue m_StreamQueue;
	NvtStreamQueue::MEM_HANDLE m_hQueueMem; //locked or pulled handle
	NvtStreamQueue::BUF_INFO m_QueueBuf; //m_hQueueMem to get real buffer offset and size

	void *m_pParentSms; //cast to NvtServerMediaSubsession	
	RTP_PACKET m_RtpPacket;
	NVT_MEDIA_TYPE  m_MediaType;
	u_int32_t m_uMaxOutputPacketSize;

	// frame control
	u_int32_t m_SessionID;
	u_int32_t m_uiMediaSrcID;
	u_int8_t *m_pCurrFrmData;
	u_int32_t m_uCurrFrmSize;
	u_int32_t m_uCurrFrmOffset;
	IGET_STRM m_fpGetStrm;
	IRELEASE_STRM m_fpReleaseStrm;
	//Flag Data
	Boolean m_bForceStop;

	//for Debug
	int m_ThreadState;

	//pause, resume
	Boolean m_bPauseState;

	void xGetNextFrm(void);
	void xStopNextFrm(void); //stop doGetNextFrame
	void xDropCurrFrm(void);

public:
	void SetParentSms(void *pParentSms);
	void SetForceStop(Boolean bForceStop);
	Boolean IsContinueToNext(); //for push mode, indicate to next schedule at packed sent
	Boolean IsPollingMode();
	Boolean GetIsForceStop();
	NvtStreamQueue *GetStreamQueue();
	Boolean Execute(); //start to provide stream
	static THREAD_DECLARE(ThreadRecv, pStreamFramer);

	double Seek(u_int32_t uiMediaSrcID, double fTarget);
	void Pause();
	void Resume();
	void SetMaxOutputPacketSize(u_int32_t uSize);
	RTP_PACKET *GetRtpPacket();
	virtual void AfterSendPacket();

	//for debug
	void DumpInfo();
	u_int32_t GetSessionID() { return m_SessionID; }
	NVT_MEDIA_TYPE GetMediaType() { return m_MediaType; }
};

class NvtStreamFramerMJPG: public NvtStreamFramer
{
public:
	static NvtStreamFramerMJPG *createNew(INIT_DATA *pInit);

	NvtStreamFramerMJPG(INIT_DATA *pInit);
	virtual ~NvtStreamFramerMJPG();

protected:
	// redefined virtual functions:
	virtual void doGetNextFrame();
	virtual void AfterSendPacket();

public:
	u_int8_t type();
	u_int8_t qFactor();
	u_int8_t width();
	u_int8_t height();
	u_int8_t const *quantizationTables(u_int8_t &precision, u_int16_t &length);
	u_int32_t GetSpecialHdrSize();

private:
	u_int32_t     m_SpecialHdrSize;
	u_int8_t      m_width;
	u_int8_t      m_height;
	u_int8_t      m_qTable[128];
};

class NvtStreamFramerH264: public NvtStreamFramer
{
public:
	static NvtStreamFramerH264 *createNew(INIT_DATA *pInit);

	NvtStreamFramerH264(INIT_DATA *pInit);
	virtual ~NvtStreamFramerH264();

protected:
	// redefined virtual functions:
	virtual void doGetNextFrame();
	virtual void AfterSendPacket();

private:
	u_int8_t *xAdjustDataOfs(u_int8_t *pData, u_int32_t *p_size);

public:
	typedef enum {
		SVC_METHOD_SKIP_LEVEL_0, //send all frames
		SVC_METHOD_SKIP_LEVEL_1, //skip L2
		SVC_METHOD_SKIP_LEVEL_2, //skip L1, L2
		SVC_METHOD_SKIP_LEVEL_COUNTS, //total level
	} SVC_METHOD_SKIP_LEVEL;

private:
	u_int32_t m_uBufferOffset; // h264,265 needing remove 0x00, 0x00, 0x00, 0x01
	u_int8_t  m_uLastFU_Indicator;
	u_int8_t  m_uLastFU_Header;
	Boolean   m_bResetToKeyFrm;
	Boolean   m_bSvcSupport;
	Boolean   m_bSkipTillNextSvc0;
	SVC_METHOD_SKIP_LEVEL m_SvcSkipLevel;
	SVC_METHOD_SKIP_LEVEL m_SvcSkipLevelByUser;

	//NVT_MODIFIED, WIFI-TEAM
	u_int32_t m_skip_svc_frm[3];
	u_int32_t m_skip_svc_byte[3];
	u_int32_t m_svc_socket_map[3];
};

class NvtStreamFramerH265 : public NvtStreamFramer
{
public:
	static NvtStreamFramerH265 *createNew(INIT_DATA *pInit);

	NvtStreamFramerH265(INIT_DATA *pInit);
	virtual ~NvtStreamFramerH265();

protected:
	// redefined virtual functions:
	virtual void doGetNextFrame();
	virtual void AfterSendPacket();

private:
	u_int8_t *xAdjustDataOfs(u_int8_t *pData, u_int32_t *p_size);
	bool xIsKeyFrm();

private:
	u_int32_t m_uBufferOffset; // h264,265 needing remove 0x00, 0x00, 0x00, 0x01
	u_int8_t  m_uLast_Payload_Header[2];
	u_int8_t  m_uLastFU_Header;
	Boolean   m_bResetToKeyFrm;
};

class NvtStreamFramerPCM: public NvtStreamFramer
{
public:
	static NvtStreamFramerPCM *createNew(INIT_DATA *pInit);

	NvtStreamFramerPCM(INIT_DATA *pInit);
	virtual ~NvtStreamFramerPCM();

protected:
	// redefined virtual functions:
	virtual void doGetNextFrame();

private:
	void xSwapCopy(u_int8_t *pDst, u_int8_t *pSrc, u_int32_t uiSize);
};

class NvtStreamFramerAAC: public NvtStreamFramer
{
public:
	static NvtStreamFramerAAC *createNew(INIT_DATA *pInit);

	NvtStreamFramerAAC(INIT_DATA *pInit);
	virtual ~NvtStreamFramerAAC();

protected:
	// redefined virtual functions:
	virtual void doGetNextFrame();
};

class NvtStreamFramerG711: public NvtStreamFramer
{
public:
	static NvtStreamFramerG711 *createNew(INIT_DATA *pInit);

	NvtStreamFramerG711(INIT_DATA *pInit);
	virtual ~NvtStreamFramerG711();

protected:
	// redefined virtual functions:
	virtual void doGetNextFrame();
};

class NvtStreamFramerMeta: public NvtStreamFramer
{
public:
	static NvtStreamFramerMeta *createNew(INIT_DATA *pInit);

	NvtStreamFramerMeta(INIT_DATA *pInit);
	virtual ~NvtStreamFramerMeta();

protected:
	// redefined virtual functions:
	virtual void doGetNextFrame();

private:
	char         *m_pMetaText; // temp buffer for event service to send report
	u_int32_t     m_uMetaTextSize;
	int           m_bFirstFrm;
};

#endif
