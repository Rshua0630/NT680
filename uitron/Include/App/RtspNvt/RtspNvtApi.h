#ifndef _RTSPNVTAPI_H
#define _RTSPNVTAPI_H

#include "Type.h"
#include "ImageStream.h"

#define RTSPNVT_API_VERSION 0x15081415U ///< API version

#define RTSPNVT_REC_WORK_BUF_SIZE 0xC8000 //800K for Max I-Frame Size
#define RTSPNVT_IPC_BUF_SIZE 0x800

#if defined(ISF_LATENCY_DEBUG) && (ISF_LATENCY_DEBUG)
#define RTSPNVT_FIFO_BUF_SIZE 0x19B00
#else
#define RTSPNVT_FIFO_BUF_SIZE 0x15B00
#endif
/************************************************************************/
/* TOS                                                                  */
/************************************************************************/
#define RTSPNVT_IPTOS_LOWDELAY          0x10
#define RTSPNVT_IPTOS_THROUGHPUT        0x08
#define RTSPNVT_IPTOS_RELIABILITY       0x04
#define RTSPNVT_IPTOS_LOWCOST           0x02

/**
   Error Code.

   Any error code occured will display on uart or return.
*/
typedef enum _RTSPNVT_ER {
	RTSPNVT_ER_OK                =   0, ///< no error
	RTSPNVT_ER_MEDIA_PALY        =  -1, ///< there is error occured to use media play
	RTSPNVT_ER_TYPE              =  -2, ///< user's assigned OPEN::Type is not support
	RTSPNVT_ER_ID                =  -3, ///< install id shall be called first.
	RTSPNVT_ER_LIVE555           =  -4, ///< there's something wrong in live555
	RTSPNVT_ER_PARAM             =  -5, ///< error parameter.
	RTSPNVT_ER_SYS               =  -6, ///< internal error.
	ENUM_DUMMY4WORD(RTSPNVT_ER)
} RTSPNVT_ER;

/**
   Warning Code.

   Any warning code occured will display on uart or return.
*/
typedef enum _RTSPNVT_WR {
	RTSPNVT_WR_OK                =   0, ///< no warning
	RTSPNVT_WR_ALREADY_OPEN      =  -1, ///< xRtspNvt_OnOpen called twice
	RTSPNVT_WR_NOT_OPEN          =  -2, ///< xRtspNvt_OnOpen not to call before.
	ENUM_DUMMY4WORD(RTSPNVT_WR)
} RTSPNVT_WR;

/**
   RTSP Type.

   RTSP uses 2 modules being a source input. One is MediaPlay playing the mov files.
   The other is LviewNvt sending live-view data.
*/
typedef enum _RTSPNVT_TYPE {
	RTSPNVT_TYPE_PLAYBACK        =   0, ///< playback via MediaPlay
	RTSPNVT_TYPE_MEDIA_REC,             ///< live-view or recording via MediaRec
	RTSPNVT_TYPE_MEDIA_REC_WITH_AUD,    ///< live-view or recording via MediaRec with audio
	ENUM_DUMMY4WORD(RTSPNVT_TYPE)
} RTSPNVT_TYPE;

/**
    AUDIO Sampling Rate
*/
typedef enum {
	RTSPNVT_AUDIO_SR_8000   = 8000,     ///< 8 KHz
	RTSPNVT_AUDIO_SR_11025  = 11025,    ///< 11.025 KHz
	RTSPNVT_AUDIO_SR_12000  = 12000,    ///< 12 KHz
	RTSPNVT_AUDIO_SR_16000  = 16000,    ///< 16 KHz
	RTSPNVT_AUDIO_SR_22050  = 22050,    ///< 22.05 KHz
	RTSPNVT_AUDIO_SR_24000  = 24000,    ///< 24 KHz
	RTSPNVT_AUDIO_SR_32000  = 32000,    ///< 32 KHz
	RTSPNVT_AUDIO_SR_44100  = 44100,    ///< 44.1 KHz
	RTSPNVT_AUDIO_SR_48000  = 48000,    ///< 48 KHz

	ENUM_DUMMY4WORD(RTSPNVT_AUDIO_SR)
} RTSPNVT_AUDIO_SR;

/**
    AUDIO Channel
*/
typedef enum {
	RTSPNVT_AUDIO_CH_LEFT,              ///< Left
	RTSPNVT_AUDIO_CH_RIGHT,             ///< Right
	RTSPNVT_AUDIO_CH_STEREO,            ///< Stereo
	RTSPNVT_AUDIO_CH_MONO,              ///< Mono two channel

	ENUM_DUMMY4WORD(RTSPNVT_AUDIO_CH)
} RTSPNVT_AUDIO_CH;

/**
    Video frame rate.
*/
typedef enum _RTSPNVT_FRAMERATE {
	RTSPNVT_FRAMERATE_30 = 30,             ///< Video frame rate 30
	RTSPNVT_FRAMERATE_24 = 24,             ///< Video frame rate 24
	RTSPNVT_FRAMERATE_15 = 15,             ///< Video frame rate 15
	RTSPNVT_FRAMERATE_12 = 12,             ///< Video frame rate 12
	ENUM_DUMMY4WORD(RTSPNVT_FRAMERATE)
} RTSPNVT_FRAMERATE;

/**
    Server Event

    Most of events are mapping to RTSP command that refered to RTSP spec.
*/
typedef enum _RTSPNVT_EVENT {
	RTSPNVT_EVENT_OPTIONS,      ///< recevie RTSP OPTION command
	RTSPNVT_EVENT_DESCRIBE,     ///< recevie RTSP DESCRIBE command
	RTSPNVT_EVENT_SETUP,        ///< recevie RTSP SETUP command
	RTSPNVT_EVENT_TEARDOWN,     ///< recevie RTSP TERADOWN command
	RTSPNVT_EVENT_PLAY,         ///< recevie RTSP PLAY command
	RTSPNVT_EVENT_PAUSE,        ///< recevie RTSP PAUSE command
	RTSPNVT_EVENT_FRAME_START_I,///< start to send client an I-Frame
	RTSPNVT_EVENT_FRAME_START_P,///< start to send client an P-Frame
	RTSPNVT_EVENT_FRAME_END,    ///< finished to send current frame
	ENUM_DUMMY4WORD(RTSPNVT_EVENT)
} RTSPNVT_EVENT;

typedef void (*RTSPNVT_CB_EVENT)(RTSPNVT_EVENT event);

/**
    Parameters, after RtspNvt_Open, user can change these parameters.
*/
typedef enum _RTSPNVT_MEDIA_SOURCE {
	RTSPNVT_MEDIA_SOURCE_PATH2_FIRST,   ///< use Path2 to be the primary stream
	RTSPNVT_MEDIA_SOURCE_PATH1_FIRST,   ///< use Path1 to be the primary stream
	RTSPNVT_MEDIA_SOURCE_BY_URL,        ///< invalid value the stream depends on URL
} RTSPNVT_MEDIA_SOURCE;

typedef enum _RTSPNVT_AVSUPPORT {
	RTSPNVT_AVSUPPORT_VIDEO_ONLY,       ///< video only
	RTSPNVT_AVSUPPORT_VIDEO_WITH_AUDIO, ///< video + audio
} RTSPNVT_AVSUPPORT;

typedef enum _RTSPNVT_PARAM {
	RTSPNVT_PARAM_MEDIA_SOURCE = 0, //Val: RTSPNVT_MEDIA_SOURCE
	RTSPNVT_PARAM_AVSUPPORT,        //Val: RTSPNVT_AVSUPPORT
	RTSPNVT_PARAM_VIDEO_PAUSE,      //Val: Video Stream ID
	RTSPNVT_PARAM_VIDEO_RESUME,     //Val: Video Stream ID
	RTSPNVT_PARAM_AUDIO_PAUSE,      //Val: Audio Stream ID
	RTSPNVT_PARAM_AUDIO_RESUME,     //Val: Audio Stream ID
	ENUM_DUMMY4WORD(RTSPNVT_PARAM)
} RTSPNVT_PARAM;

typedef enum _RTSPNVT_CODEC_VIDEO {
	RTSPNVT_CODEC_VIDEO_UNKNOWN = 0,    ///< Invalid value
	RTSPNVT_CODEC_VIDEO_NONE,           ///< No video
	RTSPNVT_CODEC_VIDEO_H264,           ///< H264
	RTSPNVT_CODEC_VIDEO_MJPG,           ///< MJPG
	RTSPNVT_CODEC_VIDEO_H265,           ///< H265
	RTSPNVT_CODEC_VIDEO_MAX_COUNT,      ///< Maximum count
} RTSPNVT_CODEC_VIDEO;

typedef enum _RTSPNVT_CODEC_AUDIO {
	RTSPNVT_CODEC_AUDIO_UNKNOWN = 0,    ///< Invalid value
	RTSPNVT_CODEC_AUDIO_NONE,           ///< No audio
	RTSPNVT_CODEC_AUDIO_PCM,            ///< PCM (refer to media's ppcm)
	RTSPNVT_CODEC_AUDIO_AAC,            ///< AAC
	RTSPNVT_CODEC_AUDIO_G711_ULAW,      ///< uLaw
	RTSPNVT_CODEC_AUDIO_G711_ALAW,      ///< aLaw
	RTSPNVT_CODEC_AUDIO_MAX_COUNT,      ///< Maximum count
} RTSPNVT_CODEC_AUDIO;

typedef enum _RTSPNVT_REQ_IDX {
	RTSPNVT_REQ_IDX_VIDEO_INFO, ///< IN: UINT32(MediaSrcID), OUT: RTSPNVT_VIDEO_INFO
	RTSPNVT_REQ_IDX_AUDIO_INFO, ///< IN: UINT32(MediaSrcID), OUT: RTSPNVT_AUDIO_INFO
	RTSPNVT_REQ_IDX_KEY_FRAME,  ///< IN: UINT32(MediaSrcID), OUT: NULL
	RTSPNVT_REQ_IDX_WORK_BUF,   ///< IN: UINT32(MediaSrcID), OUT: MEM_RANGE
	RTSPNVT_REQ_IDX_WORK_BUF_RELEASE, ///< IN: MEM_RANGE,    OUT: NULL
	RTSPNVT_REQ_IDX_STRM_BUF_RELEASE, ///< IN: RTSPNVT_FRAME,OUT: NULL
	RTSPNVT_REQ_IDX_PB_OPEN,    ///< IN: char (file path),   OUT: NULL
	RTSPNVT_REQ_IDX_PB_CLOSE,   ///< IN: NULL,               OUT: NULL
	RTSPNVT_REQ_IDX_PB_SEEK,    ///< IN: UINT32(Frame Idx),   OUT: UINT32 (Aligned Frame Idx)
} RTSPNVT_REQ_IDX;

typedef struct _RTSPNVT_VIDEO_INFO {
	UINT32 uiMediaSrcID;
	UINT32 uiCodec; //LIVE555_CODEC_VIDEO
	UINT32 uiFps;
	UINT32 uiTotoalFrame;
	UINT8  pVPS[64];
	UINT32 uiSizeVPS;
	UINT8  pSPS[64];
	UINT32 uiSizeSPS;
	UINT8  pPPS[16];
	UINT32 uiSizePPS;
} RTSPNVT_VIDEO_INFO;

typedef struct _RTSPNVT_AUDIO_INFO {
	UINT32 uiMediaSrcID;                ///< Media channel ID
	RTSPNVT_CODEC_AUDIO uiCodec;        ///< Aideo code
	UINT32 uiSamplePerSecond;           ///< Sample per second (eg. 16000, 32000,..)
	UINT32 uiBitPerSample;              ///< Bit per sample (eg. 16)
	UINT32 uiChannelCnt;                ///< channel counts (eg. 2)
	UINT32 uiTotalTimeInMs;             ///< Total time (unit: ms), (Set it to 0 for live-view)
} RTSPNVT_AUDIO_INFO;

typedef struct _RTSPNVT_BITRATE_REQ {
	UINT32 uiMediaSrcID; ///< Media channel ID
	UINT32 uiStrmID;     ///< Connection ID (Not Used for User)
	UINT32 BitrateLevel; //< (0 ~ 9). Use (Original_Bitrate * (10-BitrateLevel)) / 10;
} RTSPNVT_BITRATE_REQ;

typedef struct _RTSPNVT_FRAME_ {
	UINT32 uiMediaSrcID;                ///< Media channel ID
	UINT32 bIsVideo;                    ///< indicate this frame is video or audio
	UINT32 uiAddr;                      ///< frame buffer address
	UINT32 uiSize;                      ///< frame buffer size
	UINT32 uiSvcTemporalId;             ///< H264's SVC id.
	UINT32 hData;						///< reversed for ImageUnit_NetRtsp
	UINT64 TimeStamp; 					///< time-stamp
#if defined(ISF_LATENCY_DEBUG) && (ISF_LATENCY_DEBUG)
	ISF_DATA *pIsf;                     ///< isf data for debug
#endif
} RTSPNVT_FRAME;

typedef UINT32(*RTSPNVT_CB_REQ)(RTSPNVT_REQ_IDX Idx, void *pInput, void *pOutput);

/**
    Open Data.

    The uiWorkSize of requirement is accroding to RTSPNVT_TYPE.
    RTSPNVT_TYPE_PLAYBACK: require RTSPNVT_REC_WORK_BUF_SIZE+RTSPNVT_IPC_BUF_SIZE+MediaPlay Needed Memory
    RTSPNVT_TYPE_MEDIA_REC: require RTSPNVT_REC_WORK_BUF_SIZE+RTSPNVT_IPC_BUF_SIZE
*/
typedef struct _RTSPNVT_OPEN {
	UINT32              uiApiVer;   ///< just assign to RTSPNVT_API_VERSION
	RTSPNVT_TYPE        Type;       ///< Type for playback or live-view
	UINT32              uiPortNum;  ///< what port RTSP used, 554 is used normally.
	UINT32              uiSendBufSize; ///< for set of socket's sending buffer, set 0 to be default value 50k
	UINT32              uiWorkAddr; ///< working address
	UINT32              uiWorkSize; ///< working size, describ as below
	///< 650: RTSPNVT_TYPE_MEDIA_REC: 0
	///< 650: RTSPNVT_TYPE_PLAYBACK: whole POOL_ID_APP
	///< 660: RTSPNVT_TYPE_MEDIA_REC: RTSPNVT_REC_WORK_BUF_SIZE + RTSPNVT_IPC_BUF_SIZE
	///< 660: RTSPNVT_TYPE_PLAYBACK: whole POOL_ID_APP
	///< 680: RTSPNVT_TYPE_MEDIA_REC: RTSPNVT_FIFO_BUF_SIZE + RTSPNVT_IPC_BUF_SIZE
	///< 680: RTSPNVT_TYPE_PLAYBACK: whole POOL_ID_APP

	RTSPNVT_CB_EVENT    fpEvent;    ///< (Optional) it can be NULL or send the RTSPNVT_EVENT to the callback you assign
	UINT32              uiTypeOfService; ///< (Optional) //RTSPNVT_IPTOS_LOWDELAY, RTSPNVT_IPTOS_THROUGHPUT, RTSPNVT_IPTOS_RELIABILITY, RTSPNVT_IPTOS_LOWCOST
	RTSPNVT_CB_REQ      fpReq; ///< (Optional) if it set to NULL, rtsp directs to use MeidaRec API
	BOOL                bSupportDynaWorkMem; ///< Enable RTSPNVT_REQ_IDX_WORK_BUF, RTSPNVT_REQ_IDX_WORK_BUF_RELEASE, for NA51000, NA51023
} RTSPNVT_OPEN, *PRTSPNVT_OPEN;

extern void RtspNvt_InstallID(void) _SECTION(".kercfg_text");

extern RTSPNVT_ER RtspNvt_Open(RTSPNVT_OPEN *pOpen);
extern RTSPNVT_ER RtspNvt_Close(void);
extern RTSPNVT_ER RtspNvt_SetParam(RTSPNVT_PARAM Param, UINT32 uiVal);
extern RTSPNVT_ER RtspNvt_GetWorkBuf(MEM_RANGE *mem);
/**
    Notify RtspNvt MediaRec Event

    Only for condition Type=RTSPNVT_TYPE_MEDIA_REC. While the event 'MOVREC_EVENT' is coming, call it.
    If user employees NvtSystem (650), call Movie_RegRTSPSetBSCB(RtspNvt_RTSPSetBS); after calling RtspNvt_Open().
    And call Movie_RegRTSPSetBSCB(NULL); before calling RtspNvt_Close();

    @param[in] p1  video/audio
    @param[in] p2  media source id
    @param[in] p3  not used currently. set it to 0 is ok.

    @return
        - @b E_OK: suspend success.
*/
extern UINT32 RtspNvt_RTSPSetBS(UINT32 p1, UINT32 p2, UINT32 p3);

/**
    Notify RtspNvt MediaRec Event (Version 2, for Image Unit)
    @param[in] pFrame  frame description.
    @return
        - @b E_OK: suspend success.
*/
extern UINT32 RtspNvt_PushBS(RTSPNVT_FRAME *pFrame);
#endif
