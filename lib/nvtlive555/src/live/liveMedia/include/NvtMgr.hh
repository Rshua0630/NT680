#ifndef _NVT_MEDIA_DATA_HH
#define _NVT_MEDIA_DATA_HH

#include "NvtMedia.hh"
#include "NetCommon.h"
#include "MediaSink.hh"
#include "nvtlive555.h"

#ifdef _DEBUG
#define CFG_SERVER_TIMEOUT_SEC 10 //100000
#else
#define CFG_SERVER_TIMEOUT_SEC 30
#endif

#define CFG_RTSPD_FAKE_FPS 30
#define CFG_MSG_ARGV_MAX_COUNT 4

//Common error codes start at -64
#define NVT_COMMON_ER_EAGAIN (-64)

class NvtMgr
{
public:
	typedef enum _JOB_STATE {
		JOB_STATE_EMPTY,
		JOB_STATE_TODO,
		JOB_STATE_CANCELED,
	}JOB_STATE;

	typedef struct _TASK {
		TaskFunc* proc;
		void* client_data;
		unsigned int session_id; // for debug
		void* p_framer; // for debug
		unsigned int seq_num; // for debug
	}TASK;

	typedef struct _MSG_CMD {
		int     argc;
		char    argv[CFG_MSG_ARGV_MAX_COUNT][16];
	}MSG_CMD;

	typedef struct _MSG_BUF {
		long    mtype;
		MSG_CMD cmd;
	} MSG_BUF;

private:
	typedef struct _JOB {
		JOB_STATE state;
		TASK task;
		void* next_job;
	}JOB;

	ISTRM_CB m_StrmCb;
	NVTLIVE555_CFG m_UserCfg;
	NVTLIVE555_CB  m_UserCb;

	// Task control
	JOB *m_pJob;
	JOB *m_pJobHead;
	JOB *m_pJobTail;
	SEM_HANDLE m_sem_job;

	// internal to interrupt select
	int m_SocketPush;
	int m_SocketPullServer;
	int m_SocketPullClient;
	
	// to avoid calling MediaPlay seek function twice.
	u_int8_t m_bSeekDirty; 

	// to linux command system
	int m_msqid;
	THREAD_HANDLE m_hCmd;
	THREAD_OBJ m_hCmdObj;
	u_int8_t *m_pCmdStack;
	Boolean m_bCmdLoop;
	char *m_pWatchVariable;

	// global lock for nvtlive555's api to do critical something
	SEM_HANDLE m_sem_global;

	// for debug
	Boolean m_bShowEvent;
	Boolean m_bShowLatencyV; //show video latency
	Boolean m_bShowLatencyA; //show audio latency
	struct timeval m_PtsOfs; //time offset, gettimeofday - uitron's long timer
	Boolean m_bSupportAudio; //support connection with audio stream

	void xCalibratePtsOfs(struct timeval *pPtsOfs); //for m_PtsOfs

public: //APIs
	int Set_StrmCb(ISTRM_CB *pStrmCb);
	int Set_UserCb(NVTLIVE555_CB *pUserCb);
	ISTRM_CB *Get_StrmCb();
	NVTLIVE555_CB *Get_UserCb();
	int IsSupportRtcp();
	int IsSupportMeta();
	u_int32_t Get_Port();
	u_int32_t Get_SendBufSize();
	u_int32_t Get_TypeOfService();
	u_int32_t Get_TCPTimeOutSec();
	u_int32_t Get_MaxConnections();
	int Trans_UrlToInfo(NVTLIVE555_URL_INFO *pInfo, const char *url);
	int64_t Get_NextPtsTimeDiff(struct timeval *p_timePTS, u_int32_t frame_time_us);
	void Calc_TimeOffset(struct timeval *p_time, u_int32_t offset_time_us);
	void Set_SeekDirty();
	void SendEvent(LIVE555_SERVER_EVENT evt);
	int GlobalLock();
	int GlobalUnlock();

	u_int32_t Authenticate(const char *cHTTPHeader, const u_int32_t uiHTTPHeaderLen, char *cRetBuf, u_int32_t uiRetBufLen);
	u_int32_t RequireKeyFrame(u_int32_t m_uiMediaSrcID);
	u_int32_t GetNextMetaDataFrm(char *pTxt, u_int32_t uMaxTxtSize, int b_init);

	//Job Control
	int PushJob(TASK *pTask);
	int PullJob(TASK *pTask);
	int CancelJob(TASK *pTask);
	int SetupJobSocket();
	int FlushJobSocket();

	//Debug Command
	int RunCmdThread();
	int SetQuitVariable(char* pWatchVariable);
	static void DispatchCmd(void* firstArg);
	Boolean IsShowLatencyV() { return m_bShowLatencyV; } //show video latency
	Boolean IsShowLatencyA() { return m_bShowLatencyA; } //show audio latency
	void GetPtsOfs(struct timeval *pts) { *pts = m_PtsOfs; } // gettimeofday subtract it to get uitron_time
	Boolean IsSupportAudio() { return m_bSupportAudio; } //show audio latency

public:
	NvtMgr();
	virtual ~NvtMgr();
	static THREAD_DECLARE(ThreadCmd, pNvtMgr);
};

extern "C" int NvtMgr_CreateMgr();
extern "C" int NvtMgr_DestroyMgr();
extern "C" NvtMgr *NvtMgr_GetHandle();
#endif
