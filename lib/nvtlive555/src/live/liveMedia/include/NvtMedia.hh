#ifndef _NVT_MEDIA_HH
#define _NVT_MEDIA_HH
#include "NetCommon.h"

//for eCos650, heap size in LDS_DEMO1_evb_ECOS.lds need increasing to 4MB
//for using Linux / eCos as FAKE INTERFACE, the live555 cannot quit!!
#if defined(WIN32) || defined(__UBUNTU)
#define CFG_FAKE_INTERFACE  1
#define CFG_TEST_AUTH 0
#else
#define CFG_FAKE_INTERFACE  0
#define CFG_TEST_AUTH 0
#endif

#define CFG_MAIN_TYPE 1 //0: Client, 1:Server, 2:Both (Only for Win32)
#if (defined(WIN32) && CFG_MAIN_TYPE==2 && CFG_FAKE_INTERFACE==0)
#error "CFG_FAKE_INTERFACE must on for CFG_MAIN_TYPE==2"
#endif

#define NALU_TYPE_IDR 0x65 //H264
#define NAL_UNIT_CODED_SLICE_IDR_W_RADL 19 //H265
#define NAL_UNIT_CODED_SLICE_IDR_N_LP   20 //H265

#if defined(__ECOS)
#include <cyg/kernel/kapi.h>
#define CYG_THREAD_EXITED 16
#define THREAD_DECLARE(name,arglist) void  name (cyg_addrword_t arglist)
#define THREAD_HANDLE cyg_handle_t
#define THREAD_OBJ cyg_thread
#define THREAD_CREATE(name,thread_handle,fp,p_data,p_stack,size_stack,p_thread_obj)  cyg_thread_create(9,fp,(cyg_addrword_t)p_data,name,(void*)p_stack,size_stack,&thread_handle,p_thread_obj)
#define THREAD_RESUME(thread_handle) cyg_thread_resume(thread_handle)
#define THREAD_DESTROY(thread_handle) cyg_thread_suspend(thread_handle);while(!cyg_thread_delete(thread_handle)){cyg_thread_delay(1);}
#define THREAD_RETURN return
#define THREAD_EXIT() cyg_thread_exit()
#define THREAD_JOIN(thread_handle) {cyg_thread_info info={0};cyg_thread_get_info(thread_handle, cyg_thread_get_id(thread_handle), &info); \
		while(info.state != CYG_THREAD_EXITED){ cyg_thread_delay(1); cyg_thread_get_info(thread_handle, cyg_thread_get_id(thread_handle), &info);}}
#define SEM_HANDLE cyg_sem_t
#define SEM_CREATE(handle,init_cnt) cyg_semaphore_init(&handle,init_cnt)
#define SEM_SIGNAL(handle) cyg_semaphore_post(&handle)
#define SEM_WAIT(handle) cyg_semaphore_wait(&handle);
#define SEM_DESTROY(handle) cyg_semaphore_destroy(&handle)
#elif defined(WIN32)
#define THREAD_DECLARE(name,arglist) DWORD WINAPI name (LPVOID arglist)
#define THREAD_HANDLE HANDLE
#define THREAD_OBJ DWORD
#define THREAD_CREATE(name,thread_handle,fp,p_data,p_stack,size_stack,p_thread_obj)  thread_handle=CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)fp,(LPVOID)p_data,0,p_thread_obj)
#define THREAD_RESUME(thread_handle)
#define THREAD_DESTROY(thread_handle) TerminateThread(thread_handle,0);
#define THREAD_RETURN return 0
#define THREAD_EXIT()  ExitThread(0)
#define THREAD_JOIN(thread_handle) WaitForSingleObject(thread_handle,INFINITE);
#define SEM_HANDLE HANDLE
#define SEM_CREATE(handle,init_cnt) handle=CreateSemaphore(NULL, init_cnt, init_cnt,NULL)
#define SEM_SIGNAL(handle) ReleaseSemaphore(handle,1,NULL)
#define SEM_WAIT(handle) WaitForSingleObject(handle,INFINITE);
#define SEM_DESTROY(handle) CloseHandle(handle)
#elif defined(__LINUX) || defined(__UBUNTU)
#include <pthread.h>
#include <semaphore.h>
#define THREAD_DECLARE(name,arglist) void* name (void* arglist)
#define THREAD_HANDLE pthread_t
#define THREAD_OBJ int
#define THREAD_CREATE(name,thread_handle,fp,p_data,p_stack,size_stack,p_thread_obj)  pthread_create(&thread_handle,NULL,fp,p_data)
#define THREAD_RESUME(thread_handle)
#define THREAD_DESTROY(thread_handle) pthread_cancel(thread_handle);
#define THREAD_RETURN return NULL
#define THREAD_EXIT()  pthread_exit(0)
#define THREAD_JOIN(thread_handle) pthread_join(thread_handle, NULL);
#define SEM_HANDLE sem_t
#define SEM_CREATE(handle,init_cnt) sem_init(&handle, 0, init_cnt)
#define SEM_SIGNAL(handle) sem_post(&handle)
#define SEM_WAIT(handle) sem_wait(&handle)
#define SEM_DESTROY(handle) sem_destroy(&handle)
#endif

#if defined(WIN32)
#define CHKPNT
#define DBGD(x)
#define DBGH(x)
#define DBGTAG(x)
#else
//@{
#define CHKPNT    printf("\033[37mCHK: %d, %s\033[0m\r\n",__LINE__,__func__) ///< Show a color sting of line count and function name in your insert codes
#define DBGD(x)   printf("\033[0;35m%s=%d\033[0m\r\n",#x,x)                  ///< Show a color sting of variable name and variable deciaml value
#define DBGH(x)   printf("\033[0;35m%s=0x%08X\033[0m\r\n",#x,x)              ///< Show a color sting of variable name and variable hexadecimal value
#define DBGTAG(x) printf("%s,T:%02d %s\r\n",__func__,__TASKID__,(x)?"-end":"-begin");///< Show function caller task with begin 'DBGTAG(0)' and end 'DBGTAG(1)'
//@}
#endif

#if defined(_WIN32)
#define SLEEP_MS(x) Sleep(x)
#define SLEEP_SEC(x) Sleep((x)*1000)
#else
#define SLEEP_MS(x) usleep((x)*1000)
#define SLEEP_SEC(x) sleep(x)
#endif

typedef enum _NVT_MEDIA_TYPE {
	NVT_MEDIA_TYPE_VIDEO,
	NVT_MEDIA_TYPE_AUDIO,
	NVT_MEDIA_TYPE_META,
	NVT_MEDIA_TYPE_COUNT,
} NVT_MEDIA_TYPE;

typedef enum _NVT_CODEC_TYPE {
	NVT_CODEC_TYPE_UNKNOWN,
	NVT_CODEC_TYPE_MJPG,
	NVT_CODEC_TYPE_H264,
	NVT_CODEC_TYPE_H265,
	NVT_CODEC_TYPE_PCM,
	NVT_CODEC_TYPE_AAC,
	NVT_CODEC_TYPE_G711_ALAW,
	NVT_CODEC_TYPE_G711_ULAW,
	NVT_CODEC_TYPE_META,
	NVT_CODEC_TYPE_COUNT,
} NVT_CODEC_TYPE;

typedef enum _NVT_DBG_CMD {
	NVT_DBG_CMD_DUMP,
	NVT_DBG_CMD_FPS,
} NVT_DBG_CMD;

typedef struct _NVT_DBG_LATENCY {
	struct timeval begin_wait;
	struct timeval wait_done;
	struct timeval push_done;
	struct timeval pull_done;
	struct timeval unlock_done; //aka. sent finish
} NVT_DBG_LATENCY;

typedef struct _NVT_STRM_INFO {
	unsigned long long seq_num;
	unsigned int addr;
	unsigned int size;
	struct timeval tm;
	//extra info
	unsigned int svc_idx; //only h264 only
	//debug latency
	NVT_MEDIA_TYPE type;
	NVT_DBG_LATENCY latency;
} NVT_STRM_INFO;

typedef struct _NVT_VIDEO_INFO {
	NVT_CODEC_TYPE codec_type;
	unsigned char vps[64];
	int vps_size;
	unsigned char sps[64];
	int sps_size;
	unsigned char pps[64];
	int pps_size;
} NVT_VIDEO_INFO;

typedef struct _NVT_AUDIO_INFO {
	NVT_CODEC_TYPE codec_type;
	int sample_per_second;
	int bit_per_sample;
	int channel_cnt;
} NVT_AUDIO_INFO;

typedef int(*IOPEN_STRM)(int channel, void *p_strm_mgr);
typedef int(*ICLOSE_STRM)(int h_strm);
typedef int(*IGET_VIDEO_INFO)(int h_strm, int channel, int timeout_ms, NVT_VIDEO_INFO *p_info);
typedef int(*IGET_AUDIO_INFO)(int h_strm, int channel, int timeout_ms, NVT_AUDIO_INFO *p_info);
typedef int(*IGET_STRM)(int h_strm, int channel, int timeout_ms, NVT_STRM_INFO *p_strm);
typedef int(*IRELEASE_STRM)(int h_strm, int channel);
typedef int(*IDBG_CMD)(NVT_DBG_CMD cmd);

typedef struct _ISTRM_CB {
	IOPEN_STRM OpenStrm;
	ICLOSE_STRM CloseStrm;
	IGET_STRM GetVideoStrm;
	IRELEASE_STRM ReleaseVideoStrm;
	IGET_VIDEO_INFO GetVideoInfo;
	IGET_STRM GetAudioStrm;
	IRELEASE_STRM ReleaseAudioStrm;
	IGET_AUDIO_INFO GetAudioInfo;
	IDBG_CMD DbgCmd;
} ISTRM_CB;

#ifdef _DEBUG
#include <stdlib.h>
#include <crtdbg.h>
#ifndef DBG_NEW
#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
#define new DBG_NEW
#endif
#endif  // _DEBUG


#endif
