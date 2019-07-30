#ifndef _NVT_STREAMQUEUE_HH
#define _NVT_STREAMQUEUE_HH
#include "NetCommon.h"
#include "NvtMedia.hh"

#define NVT_RTP_HDR_SIZE_STD 12

#define NVT_RTP_HDR_SIZE_MJPG_SUB_MAIN 8   //mainJPEGHeader
#define NVT_RTP_HDR_SIZE_MJPG_SUB_QUANT_HDR 4  //quantizationHeaderSize
#define NVT_RTP_HDR_SIZE_MJPG_SUB_QTBL 128  //QTbl
#define NVT_RTP_HDR_SIZE_MJPG (NVT_RTP_HDR_SIZE_STD+NVT_RTP_HDR_SIZE_MJPG_SUB_MAIN+NVT_RTP_HDR_SIZE_MJPG_SUB_QUANT_HDR+NVT_RTP_HDR_SIZE_MJPG_SUB_QTBL)

#define NVT_RTP_HDR_SIZE_AAC_SUB_MAIN 4
#define NVT_RTP_HDR_SIZE_AAC (NVT_RTP_HDR_SIZE_STD + NVT_RTP_HDR_SIZE_AAC_SUB_MAIN)

class NvtStreamQueue
{
public:
	typedef void *MEM_HANDLE;
	typedef struct _BUF_INFO {
		u_int8_t *p_buf;
		u_int32_t size;
		NVT_STRM_INFO strm_info; //backup from ISTRM_CB
		struct timeval pts; // uitron's long timer to get gettimeofday
	} BUF_INFO;

protected:
	typedef enum _MEM_STATE {
		MEM_STATE_EMPTY,
		MEM_STATE_LOCKED,
		MEM_STATE_PUSHED,
		MEM_STATE_PULLED
	} MEM_STATE;
	typedef struct _MEM {
		// buffer control
		MEM_STATE state;
		u_int8_t *p_buf;
		u_int32_t total_size;
		u_int32_t valid_size;
		NVT_STRM_INFO strm_info; //backup from ISTRM_CB
		void *p_next; //pointer to next MEM*
	} MEM;

	SEM_HANDLE m_sem;
	SEM_HANDLE m_sem_api;
	MEM m_mem[2]; //double buffer
	MEM *m_plock; //current locked frame
	MEM *m_ppull; //current pulled frame
	int m_quit; //detect quit case
	int m_mem_align_size; //buffer trunk alignment size
	struct timeval m_pts_ofs; // time offset, gettimeofday - uitron's long timer of first frame


public:
	int open(NVT_CODEC_TYPE codec_type);
	int close();
	int reset();
	MEM_HANDLE lock(); //return handle
	int push(MEM_HANDLE handle, NVT_STRM_INFO *p_strm);
	MEM_HANDLE pull();
	int unlock(MEM_HANDLE handle);
	int get_buf(MEM_HANDLE handle, BUF_INFO *p_info);
	NvtStreamQueue();
	virtual ~NvtStreamQueue();
};
#endif
