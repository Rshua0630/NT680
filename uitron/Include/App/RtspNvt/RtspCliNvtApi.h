#ifndef _RTSPCLINVTAPI_H
#define _RTSPCLINVTAPI_H

#include "Type.h"

#define RTSPCLINVT_API_VERSION 0x15081415U ///< API version

#define RTSPCLINVT_REC_WORK_BUF_SIZE 0x180000 ///< for Max I-Frame Size (1MB), and Audio (0.5MB)
#define RTSPCLINVT_IPC_BUF_SIZE 2048

/**
   Error Code.

   Any error code occured will display on uart or return.
*/
typedef enum _RTSPCLINVT_ER {
	RTSPCLINVT_ER_OK                =   0, ///< no error
	RTSPCLINVT_ER_PARAM             =  -1, ///< error parameter.
	RTSPCLINVT_ER_NOT_SUPPORT       =  -2, ///< function not support
	RTSPCLINVT_ER_ID                =  -3, ///< install id shall be called first.
	RTSPCLINVT_ER_LIVE555CLI        =  -4, ///< live555-cli failed.
	ENUM_DUMMY4WORD(RTSPCLINVT_ER)
} RTSPCLINVT_ER;

/**
   Warning Code.

   Any warning code occured will display on uart or return.
*/
typedef enum _RTSPCLINVT_WR {
	RTSPCLINVT_WR_OK                =   0, ///< no warning
	RTSPCLINVT_WR_ALREADY_OPEN      =  -1, ///< xRtspNvt_OnOpen called twice
	RTSPCLINVT_WR_NOT_OPEN          =  -2, ///< xRtspNvt_OnOpen not to call before.
	ENUM_DUMMY4WORD(RTSPCLINVT_WR)
} RTSPCLINVT_WR;

typedef enum _RTSPCLINVT_CODEC_VIDEO {
	RTSPCLINVT_CODEC_VIDEO_UNKNOWN = 0,
	RTSPCLINVT_CODEC_VIDEO_NONE,
	RTSPCLINVT_CODEC_VIDEO_H264,
	RTSPCLINVT_CODEC_VIDEO_MJPG,
	RTSPCLINVT_CODEC_VIDEO_H265,
	RTSPCLINVT_CODEC_VIDEO_MAX_COUNT,
} RTSPCLINVT_CODEC_VIDEO;

typedef enum _RTSPCLINVT_CODEC_AUDIO {
	RTSPCLINVT_CODEC_AUDIO_UNKNOWN = 0,
	RTSPCLINVT_CODEC_AUDIO_NONE,
	RTSPCLINVT_CODEC_AUDIO_PCM,
	RTSPCLINVT_CODEC_AUDIO_AAC,
	RTSPCLINVT_CODEC_AUDIO_G711_ULAW,
	RTSPCLINVT_CODEC_AUDIO_G711_ALAW,
	RTSPCLINVT_CODEC_AUDIO_MAX_COUNT,
} RTSPCLINVT_CODEC_AUDIO;

typedef struct _RTSPCLINVT_VIDEO_INFO {
	UINT32 uiMediaSrcID; //Always be '0'
	UINT32 uiCodec; //RTSPCLINVT_CODEC_VIDEO
	UINT32 uiFps; //Not Used
	UINT32 uiTotoalFrame; //Not Used
	UINT8  pVPS[64];
	UINT32 uiSizeVPS;
	UINT8  pSPS[64];
	UINT32 uiSizeSPS;
	UINT8  pPPS[16];
	UINT32 uiSizePPS;
} RTSPCLINVT_VIDEO_INFO;

typedef struct _RTSPCLINVT_AUDIO_INFO {
	UINT32 uiMediaSrcID; //reversed
	UINT32 uiCodec; //RTSPCLINVT_CODEC_AUDIO
	UINT32 uiSamplePerSecond;
	UINT32 uiBitPerSample; //Not Used
	UINT32 uiChannelCnt;
	UINT32 uiTotalTimeInMs;
} RTSPCLINVT_AUDIO_INFO;

typedef struct _RTSPCLINVT_STREAM_INFO {
	RTSPCLINVT_VIDEO_INFO vid;
	RTSPCLINVT_AUDIO_INFO aud;
} RTSPCLINVT_STREAM_INFO;

///< TODO: TIMESTAMP
typedef struct _RTSPCLINVT_FRM {
	UINT32 uiFrmCmd; //reversed
	UINT32 uiPtsSec;
	UINT32 uiPtsUsec;
	UINT32 uiAddr;
	UINT32 uiSize;
} RTSPCLINVT_FRM;

/**
    Client Event
*/
typedef enum _RTSPCLINVT_EVENT {
	RTSPCLINVT_EVENT_OPEN,            ///< get sdp response (RTSPCLINVT_STREAM_INFO*)
	RTSPCLINVT_EVENT_CLOSE,           ///< disconnected (NULL)
	RTSPCLINVT_EVENT_VIDEO,           ///< recevie a video frame (RTSPCLINVT_FRM)
	RTSPCLINVT_EVENT_AUDIO,           ///< recevie a audio frame (RTSPCLINVT_FRM)
	ENUM_DUMMY4WORD(RTSPCLINVT_EVENT)
} RTSPCLINVT_EVENT;


/**
*/
typedef void (*RTSPCLINVT_CB_EVENT)(RTSPCLINVT_EVENT event, void *pData);

/**
    Open Data.
*/
typedef struct _RTSPCLINVT_OPEN {
	UINT32 uiApiVer;   ///< just assign to RTSPCLINVT_API_VERSION
	CHAR  *szURL;
	UINT32 uiWorkAddr;
	UINT32 uiWorkSize;
	BOOL   bDisableVideo;
	BOOL   bDisableAudio;
	RTSPCLINVT_CB_EVENT fpEvent;
} RTSPCLINVT_OPEN, *PRTSPCLINVT_OPEN;

extern void RtspCliNvt_InstallID(void) _SECTION(".kercfg_text");

extern RTSPCLINVT_ER RtspCliNvt_Open(RTSPCLINVT_OPEN *pOpen);
extern RTSPCLINVT_ER RtspCliNvt_Close(void);

#endif
