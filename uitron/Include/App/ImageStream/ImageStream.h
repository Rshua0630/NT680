/**
    Image Stream Framework.

    A framework to integrate image data flow of all related application tasks.

    @file       ImageStream.h
    @ingroup    mIImgStm
    @note       Nothing

    Copyright   Novatek Microelectronics Corp. 2014.  All rights reserved.
*/

#ifndef _IMAGESTREAM_H_
#define _IMAGESTREAM_H_

#include "Type.h"
#include "GxImage.h"

#if defined(_BSP_NA51000_)
#define ISF_MAX_STREAM		16
#elif defined(_BSP_NA51023_)
#define ISF_MAX_STREAM		8
#else
#define ISF_MAX_STREAM		0
#endif

#define ISF_LATENCY_DEBUG		DISABLE //DISABLE

/*

    Concepts:

    1. stream is a data "directed acyclic graph" defined by many port-connected units.
    2. unit could have many input ports, and many output ports.
    3. unit could be connect togather if any source unit's output port is binding to destination unit's input port.
    4. each port is assigned to connected with PUSH, PULL, or NOTIFY-PULL connect-type.
    5. after open the stream, data will passing between units through their connected ports with behavior of its connect-type.


    Usage:

    1. Connect port to build a data "directed acyclic graph" by these APIs:

        ImageUnit_Begin(pUnit, flag);
            ImageUnit_SetOutput(oPort, pDest, state);
        ImageUnit_End();

    2. Change state of port by these APIs:

        ImageUnit_Begin(pUnit, flag);
            ImageUnit_SetOutput(oPort, pDest, state);
        ImageUnit_End();

    3. Config or change parameters of units by these APIs:

        ImageUnit_Begin(pUnit, flag);
            ImageUnit_SetVdoImgSize(nPort, img_w, img_h);
            ImageUnit_SetVdoPreWindow(nPort, win_x, win_y, win_w, win_h);
            ImageUnit_SetVdoPostWindow(nPort, win_x, win_y, win_w, win_h);
            ImageUnit_SetVdoAspect(nPort, img_ratio_x, img_ratio_y);
            ImageUnit_SetVdoDirect(nPort, img_dir);
            ImageUnit_SetVdoBufferCount(nPort, len);
            ImageUnit_SetAudSample(nPort, aud_bit_per_sample, aud_channel_count, aud_sample_per_second);
            ImageUnit_SetParam(nPort, param, value);
        ImageUnit_End(void);
        ImageUnit_GetParam(pUnit, nPort, param);


    4. Start to control graph by these APIs:

        ImageStream_Open(pStream);
        ImageStream_UpdateAll(pStream);
        ImageStream_Close(pStream);
        ImageStream_IsOpen(pStream);


    5. Get port of units by these APIs:

        ImageUnit_In(pUnit, iPort);
        ImageUnit_Out(pUnit, oPort);


    6. Get data from port and send data to port of units by these APIs:

        ImageUnit_IsAllowPush(pDestPort);
        ImageUnit_Push(pDestPort, pImgBuf, 0);
        ImageUnit_IsAllowPull(pSrcPort);
        ImageUnit_Pull(pSrcPort, pImgBuf, 0);
        ImageUnit_IsAllowNotify(pSrcPort);
        ImageUnit_Notify(pSrcPort, 0);
        addr = ImageUnit_New(size, pData);
        ImageUnit_Release(pData);

*/

/**
    @addtogroup mIImgStm
*/
//@{


extern void ImageStream_InstallID(void) _SECTION(".kercfg_text");


/**
     @name Signature value
*/
//@{
#define ISF_SIGN_STREAM     MAKEFOURCC('I','S','F','S')
#define ISF_SIGN_UNIT       MAKEFOURCC('I','S','F','U')
#define ISF_SIGN_PORT       MAKEFOURCC('I','S','F','P')
#define ISF_SIGN_DATA       MAKEFOURCC('I','S','F','D')
//@}

/**
     @name Result value
*/
//@{
typedef enum {
	ISF_OK                          = 0, ///< ok
	//for auto dirty after SetParam()
	ISF_APPLY_RUNTIME              = 1UL, ///< ok, update by this SYNC/UPDATE
	ISF_APPLY_DOWNSTREAM_RUNTIME   = 2UL, ///< ok, update by downstream SYNC/UPDATE
	ISF_APPLY_DOWNSTREAM_READYTIME = 3UL, ///< ok, update by downstream STOP/START
	ISF_APPLY_DOWNSTREAM_OFFTIME   = 4UL, ///< ok, update by downstream STOP/CLOSE/OPEN/START
	ISF_APPLY_MAX                  = 5UL,
	//for general operation
	ISF_ERR_FAILED              = -1UL, ///< general failure
	ISF_ERR_IGNORE              = -2UL, ///< ignored or bypassed
	ISF_ERR_NOT_SUPPORT         = -3UL, ///< not support
	ISF_ERR_ALREADY_OPEN        = -4UL, ///< already open, or not close yet
	ISF_ERR_NOT_OPEN_YET        = -5UL, ///< already close, or not open yet
	ISF_ERR_INACTIVE_STATE      = -6UL, ///< inactive state
	ISF_ERR_WAIT_TIMEOUT        = -7UL, ///< wait timeout
	//for general value
	ISF_ERR_VER_NOTMATCH        = -10UL, ///< version not match
	ISF_ERR_NULL_OBJECT         = -11UL, ///< object pointer is NULL
	ISF_ERR_NULL_FUNCCB         = -12UL, ///< the object has null function pointer
	ISF_ERR_NULL_POINTER        = -13UL, ///< parameter pointer is NULL
	ISF_ERR_INVALID_VALUE       = -14UL, ///< parameter value is invalid
	ISF_ERR_INVALID_SIGN        = -15UL, ///< invalid signature
	ISF_ERR_INVALID_NAME        = -16UL, ///< invalid name
	ISF_ERR_INVALID_STATE       = -17UL, ///< invalid state
	ISF_ERR_INVALID_PARAM       = -18UL, ///< invalid parameter
	ISF_ERR_INVALID_DATA        = -19UL, ///< invalid data format or content
	//for stream and unit
	ISF_ERR_NAME_ISNOTMATCH     = -20UL, ///< cannot find stream or unit by match name!
	ISF_ERR_COUNT_OVERFLOW      = -21UL, ///< total count of connected unit is overflow!
	//for port
	ISF_ERR_INVALID_PORT_ID     = -30UL, ///< invalid input port id
	ISF_ERR_CONNECT_NOTMATCH    = -31UL, ///< build connect step1: match connect type is failed!
	ISF_ERR_CONNECT_BINDDEST    = -32UL, ///< build connect step2: bind to dest unit is failed!
	ISF_ERR_CONNECT_BINDSRC     = -33UL, ///< build connect step3: bind to src unit is failed!
	ISF_ERR_CONNECT_CHANGED     = -34UL, ///< connect is changed
	ISF_ERR_CONNECT_NOTBUILD    = -35UL, ///< connect is not build
	ISF_ERR_ALREADY_CONNECT     = -36UL, ///< already connect
	ISF_ERR_NOT_CONNECT_YET     = -37UL, ///< not connect yet
	ISF_ERR_INACTIVE_PORT       = -38UL, ///< inactive port
	//for data
	ISF_ERR_BUFFER_CREATE       = -40UL, ///< buffer create pool fail
	ISF_ERR_BUFFER_DESTROY      = -41UL, ///< buffer destroy pool fail
	ISF_ERR_BUFFER_GET          = -42UL, ///< buffer get new block fail (reference_count = 1)
	ISF_ERR_BUFFER_ADD          = -43UL, ///< buffer add block fail (reference_count ++)
	ISF_ERR_BUFFER_RELEASE      = -44UL, ///< buffer release block fail (reference_count --)
	ISF_ERR_QUEUE_EMPTY         = -45UL, ///< the queue is empty
	ISF_ERR_QUEUE_FULL          = -46UL, ///< the queue is full
	ISF_ERR_PROCESS_FAIL        = -47UL, ///< data process fail
	ISF_ERR_PUSH_FAIL           = -48UL, ///< data push fail
	ISF_ERR_PULL_FAIL           = -49UL, ///< data pull fail


	ISF_ERR_MAX                 = -256UL, ///<
	ENUM_DUMMY4WORD(ISF_RV)
}
ISF_RV;
//@}

/**
     @name VALUE for keep last value
*/
#define ISF_VALUE_KEEP              0xffffffff ///< keep last value

/**
     Port state
*/
typedef enum {
	ISF_PORT_STATE_OFF      = 0x00, ///< off
	ISF_PORT_STATE_READY    = 0x01, ///< ready
	ISF_PORT_STATE_RUN      = 0x02, ///< run
	ISF_PORT_STATE_WAIT     = 0x03, ///< wait
	ISF_PORT_STATE_SLEEP    = 0x04, ///< sleep
	ISF_PORT_STATE_ALL      = 0xff, ///< any state for compare
	ENUM_DUMMY4WORD(ISF_PORT_STATE)
}
ISF_PORT_STATE;

/**
     Port command
*/
typedef enum {
	////////////////////////////////////////////////////////////////////////////////////////////////
	// begin cmd (forward)
	ISF_PORT_CMD_CONNECT            = 0x00000001, ///< connect -> off
	ISF_PORT_CMD_OPEN               = 0x00000002, ///< off -> ready
	ISF_PORT_CMD_START              = 0x00000004, ///< ready -> run
	ISF_PORT_CMD_RESUME             = 0x00000008, ///< wait -> run
	ISF_PORT_CMD_WAKEUP             = 0x00000010, ///< sleep -> run
	ISF_PORT_CMD_S1                 = 0x00000020, ///<
	ISF_PORT_CMD_S0                 = 0x00000040, ///<
	////////////////////////////////////////////////////////////////////////////////////////////////
	// sync cmd (forward)
	ISF_PORT_CMD_RUNTIME_UPDATE     = 0x00000100, ///< run -> run (update change)
	// sync cmd (backward)
	ISF_PORT_CMD_OFFTIME_SYNC       = 0x00000200, ///< off -> off (off-time property is dirty)
	ISF_PORT_CMD_READYTIME_SYNC     = 0x00000400, ///< ready -> ready (ready-time property is dirty)
	ISF_PORT_CMD_RUNTIME_SYNC       = 0x00000800, ///< run -> run (run-time property is dirty)
	////////////////////////////////////////////////////////////////////////////////////////////////
	// end cmd (backward)
	ISF_PORT_CMD_DISCONNECT         = 0x00001000, ///< off -> disconnect
	ISF_PORT_CMD_CLOSE              = 0x00002000, ///< stop -> off
	ISF_PORT_CMD_STOP               = 0x00004000, ///< run -> stop
	ISF_PORT_CMD_SLEEP              = 0x00008000, ///< run -> sleep
	ISF_PORT_CMD_PAUSE              = 0x00010000, ///< run -> wait
	ISF_PORT_MASK_STATE             = 0x0001ffff, ///< state mask
	////////////////////////////////////////////////////////////////////////////////////////////////
	ISF_PORT_CMD_OUTPUT             = 0x01000000, ///< output is dirty
	ISF_PORT_CMD_STATE              = 0x02000000, ///< state is dirty
	ISF_PORT_CMD_INPUT_INFO         = 0x10000000, ///< property is dirty (input port info)
	ISF_PORT_CMD_OUTPUT_INFO        = 0x20000000, ///< property is dirty (output port info)
	ISF_PORT_CMD_INPUT_PARAM        = 0x40000000, ///< property is dirty (input port param)
	ISF_PORT_CMD_OUTPUT_PARAM       = 0x80000000, ///< property is dirty (output port param)
	ISF_UNIT_CMD_CTRL_PARAM         = 0x04000000, ///< property is dirty (ctrl param)
	ENUM_DUMMY4WORD(ISF_PORT_CMD)
}
ISF_PORT_CMD;


/**
     Stream output port ID.
*/
typedef enum {
	ISF_OUT_BASE = 0,
	ISF_OUT1 = ISF_OUT_BASE, ///< output 1
	ISF_OUT2, ///< output 2
	ISF_OUT3, ///< output 3
	ISF_OUT4, ///< output 4
	ISF_OUT5, ///< output 5
	ISF_OUT6, ///< output 6
	ISF_OUT7, ///< output 7
	ISF_OUT8, ///< output 8
	ISF_OUT9, ///< output 9
	ISF_OUT10, ///< output 10
	ISF_OUT11, ///< output 11
	ISF_OUT12, ///< output 12
	ISF_OUT13, ///< output 13
	ISF_OUT14, ///< output 14
	ISF_OUT15, ///< output 15
	ISF_OUT16, ///< output 16
	ISF_OUT17, ///< output 17
	ISF_OUT18, ///< output 18
	ISF_OUT19, ///< output 19
	ISF_OUT20, ///< output 20
	ISF_OUT21, ///< output 21
	ISF_OUT22, ///< output 22
	ISF_OUT23, ///< output 23
	ISF_OUT24, ///< output 24
	ISF_OUT25, ///< output 25
	ISF_OUT26, ///< output 26
	ISF_OUT27, ///< output 27
	ISF_OUT28, ///< output 28
	ISF_OUT29, ///< output 29
	ISF_OUT30, ///< output 30
	ISF_OUT31, ///< output 31
	ISF_OUT32, ///< output 32
	ISF_OUT_MAX,
	ENUM_DUMMY4WORD(ISF_OUT)
}
ISF_OUT;

#define ISF_OUTC ISF_OUT6

/**
     Stream input port ID.
*/
typedef enum {
	ISF_IN_BASE = 128,
	ISF_IN1 = ISF_IN_BASE, ///< input 1
	ISF_IN2, ///< input 2
	ISF_IN3, ///< input 3
	ISF_IN4, ///< input 4
	ISF_IN5, ///< input 5
	ISF_IN6, ///< input 6
	ISF_IN7, ///< input 7
	ISF_IN8, ///< input 8
	ISF_IN9, ///< input 9
	ISF_IN10, ///< input 10
	ISF_IN11, ///< input 11
	ISF_IN12, ///< input 12
	ISF_IN13, ///< input 13
	ISF_IN14, ///< input 14
	ISF_IN15, ///< input 15
	ISF_IN16, ///< input 16
	ISF_IN17, ///< input 17
	ISF_IN18, ///< input 18
	ISF_IN19, ///< input 19
	ISF_IN20, ///< input 20
	ISF_IN21, ///< input 21
	ISF_IN22, ///< input 22
	ISF_IN23, ///< input 23
	ISF_IN24, ///< input 24
	ISF_IN25, ///< input 25
	ISF_IN26, ///< input 26
	ISF_IN27, ///< input 27
	ISF_IN28, ///< input 28
	ISF_IN29, ///< input 29
	ISF_IN30, ///< input 30
	ISF_IN31, ///< input 31
	ISF_IN32, ///< input 32
	ISF_IN_MAX,
	ENUM_DUMMY4WORD(ISF_IN)
}
ISF_IN;

#define ISF_CTRL    0xffffffff ///< unit control

#define ISF_IS_OPORT(pUnit, nPort)     ((((UINT32)(nPort)) < ISF_IN_BASE) && (((UINT32)(nPort)) < (pUnit)->nOut))

#define ISF_IS_IPORT(pUnit, nPort)     ((((UINT32)(nPort)) >= ISF_IN_BASE) && (((UINT32)(nPort))-ISF_IN_BASE < (pUnit)->nIn))

/**
     Stream connect type of port.
*/
typedef enum {
	ISF_CONNECT_NONE = 0,  ///< dummy connect type
	ISF_CONNECT_PUSH = 0x01,  ///< push connect type
	ISF_CONNECT_PULL = 0x02,  ///< pull connect type
	ISF_CONNECT_NOTIFY = 0x04,  ///< notify connect type, support ISF_CONNECT_NOTIFY|ISF_CONNECT_PUSH
	ISF_CONNECT_SYNC = 0x08,  ///< sync connect type, support ISF_CONNECT_PUSH|ISF_CONNECT_SYNC
	ISF_CONNECT_ALL = ISF_CONNECT_PUSH | ISF_CONNECT_PULL | ISF_CONNECT_NOTIFY | ISF_CONNECT_SYNC, ///< all connect type
	ENUM_DUMMY4WORD(ISF_CONNECT_TYPE)
}
ISF_CONNECT_TYPE;

#define ISF_VIDEO_RAW_HEADER_SIZE    1024   ///< size of VRAW internal header in BYTES, include ISF_DATA + ISF_VIDEO_RAW_BUF + (internal info)

/**
    Event message.
*/
typedef struct _ISF_EVENT_MSG {
	UINT32 Event; ///< default 0
	UINT32 Param1; ///< default 0
	UINT32 Param2; ///< default 0
	UINT32 Param3; ///< default 0
} ISF_EVENT_MSG; ///< size = 4 words

#define RECORD_DATA_TS			DISABLE //ENABLE

#define ISF_DATA_DESC_SIZE  22 ///< size of data desc in WORDS
#if (RECORD_DATA_TS == ENABLE)
#define ISF_DATA_SIZE       46 ///< size of data in WORDS
#elif ISF_LATENCY_DEBUG
#define ISF_DATA_SIZE       50 ///< size of data in WORDS
#else
#define ISF_DATA_SIZE       36 ///< size of data in WORDS
#endif

/**
    General data and its descriptor.
*/
typedef struct _ISF_DATA {
	UINT32 Sign; ///< signature, equal to ISF_SIGN_DATA
	UINT32 hData;  ///< handle of real data, it will be "custom data handle", or "nvtmpp blk_id"
	ISF_RV(*pLockCB)(UINT64 module, UINT32 hData);  ///< CB to lock "custom data handle", or NULL if hData is "nvtmpp blk_id"
	ISF_RV(*pUnlockCB)(UINT64 module, UINT32 hData);  ///< CB to unlock "custom data handle", or NULL if hData is "nvtmpp blk_id"
	ISF_EVENT_MSG Event; ///< default 0
	UINT64 Serial; ///< serial id
	UINT64 TimeStamp; ///< time-stamp ImagePipe process start
#if (RECORD_DATA_TS == ENABLE)
	UINT64 TimeStamp_g; ///< time-stamp ImagePipe new
	UINT64 TimeStamp_p; ///< time-stamp ImagePipe process end (push)
	UINT64 TimeStamp_e; ///< time-stamp VdoEnc process start
	UINT64 TimeStamp_s; ///< time-stamp VdoEnc process end (sync)
	UINT64 TimeStamp_n; ///< time-stamp ImagePipe notify
#elif ISF_LATENCY_DEBUG
	UINT64 vd_timestamp;
	UINT64 dramend_timestamp;
	UINT64 ipl_timestamp_start;
	UINT64 ipl_timestamp_end;
	UINT64 enc_timestamp_start;
	UINT64 enc_timestamp_end;
	UINT64 rtsp_timestamp_start;
#endif
	UINT32 pSrcPort; ///< data source
	UINT32 bDirectReady; ///< ready of direct mode
	UINT32 Desc[ISF_DATA_DESC_SIZE]; ///< data descriptor, the first word must be FOURCC
} ISF_DATA; ///< size = 36 words
STATIC_ASSERT(sizeof(ISF_DATA) / sizeof(UINT32) == ISF_DATA_SIZE);

/**
     Video raw data descriptor. (see IMG_BUF of GxImage)
*/
typedef IMG_BUF ISF_VIDEO_RAW_BUF; ///< size = 20 words
#if 0
typedef struct _IMG_BUF {
	UINT32              flag;                      ///< MAKEFOURCC('V','R','A','W') or MAKEFOURCC('N','V','X','1') or MAKEFOURCC('N','V','X','2')
	UINT32              Width;                     ///< image width
	UINT32              Height;                    ///< image height
	GX_IMAGE_PIXEL_FMT  PxlFmt;                    ///< pixel format
	UINT32              PxlAddr[MAX_PLANE_NUM];    ///< pixel address
	UINT32              LineOffset[MAX_PLANE_NUM]; ///< pixel lineoffset
	IPOINT              ScaleRatio;                ///< Scale ratio for virtual/Real coordinate translation
	IRECT               VirtCoord;                 ///< Virtual coordinate
	IRECT               RealCoord;                 ///< Real    coordinate
} IMG_BUF, *PIMG_BUF;
#endif
STATIC_ASSERT(sizeof(ISF_VIDEO_RAW_BUF) / sizeof(UINT32) == ISF_DATA_DESC_SIZE);

/**
     Audio raw data descriptor.
*/
typedef struct _ISF_AUDIO_RAW_BUF {
	UINT32              flag;                      ///< MAKEFOURCC('A','R','A','W')
	UINT32              ChannelCnt;                ///< ChannelCnt
	UINT32              Resv1;                     ///< Resv
	UINT32              Resv2;                     ///< Resv
	UINT32              DataAddr;                  ///< data address
	UINT32              DataSize;                  ///< data size = SampePerSecond * ChannelCnt * BitsPerSample / 8
	UINT32              SampePerSecond;            ///< SampePerSecond
	UINT32              BitsPerSample;             ///< BitsPerSample
	UINT32              Resv[14];                  ///<
} ISF_AUDIO_RAW_BUF, *PISF_AUDIO_RAW_BUF; ///< size = 20 words
STATIC_ASSERT(sizeof(ISF_AUDIO_RAW_BUF) / sizeof(UINT32) == ISF_DATA_DESC_SIZE);


/**
     Stream audio bitstream data descriptor.
*/
typedef struct _ISF_AUDIO_STREAM_BUF {
	UINT32              flag;                      ///< MAKEFOURCC('A','S','T','M')
	UINT32              ChannelCnt;                ///< ChannelCnt
	UINT32              Resv1;                     ///< Resv
	UINT32              CodecType;                 ///< codec type
	UINT32              DataAddr;                  ///< data address
	UINT32              DataSize;                  ///< data size = SampePerSecond * ChannelCnt * BitsPerSample / 8
	UINT32              SampePerSecond;            ///< SampePerSecond
	UINT32              BitsPerSample;             ///< BitsPerSample
	UINT32              Resv[14];                  ///<
} ISF_AUDIO_STREAM_BUF, *PISF_AUDIO_STREAM_BUF; ///< size = 20 words
STATIC_ASSERT(sizeof(ISF_AUDIO_STREAM_BUF) / sizeof(UINT32) == ISF_DATA_DESC_SIZE);

/**
     Stream video bitstream data descriptor.
*/
typedef struct _ISF_VIDEO_STREAM_BUF {
	UINT32              flag;                      ///< MAKEFOURCC('V','S','T','M')
	UINT32              Width;                     ///< image width
	UINT32              Height;                    ///< image height
	UINT32              CodecType;                 ///< codec type
	UINT32              DataAddr;                  ///< data address
	UINT32              DataSize;                  ///< data size
	UINT32              FramePerSecond;            ///< FramePerSecond
	UINT32              BytePerSecond;             ///< BytePerSecond
	UINT32              Resv[14];                  ///<
} ISF_VIDEO_STREAM_BUF, *PISF_VIDEO_STREAM_BUF; ///< size = 20 words
STATIC_ASSERT(sizeof(ISF_VIDEO_STREAM_BUF) / sizeof(UINT32) == ISF_DATA_DESC_SIZE);

//NOTE: for VideoCodec == H264,
//      Resv[0]=SPS addr, Resv[1]=SPS size, Resv[2]=PPS addr, Resv[3]=PPS size


/**
     metadata descriptor.
*/
typedef struct _ISF_METADATA_BUF {
	UINT32              flag;                      ///< MAKEFOURCC()
	UINT32              addr;                      ///< pointer to a metadata "riff chunk"
} ISF_METADATA_BUF, *PISF_METADATA_BUF; ///< size = 2 words
STATIC_ASSERT(sizeof(ISF_METADATA_BUF) / sizeof(UINT32) == 2);

/**
     fileout descriptor.
*/
typedef struct _ISF_FILEOUT_BUF {
	UINT32              flag;                      ///< MAKEFOURCC('F','O','U','T')
	UINT32              event;                     ///< defined by the input unit
	UINT32              fileop;                    ///< bitwise: open/close/conti_write or seek_write/flush/none(event only)/discard
	UINT32              addr;                      ///< write data address
	UINT64              size;                      ///< write data size
	UINT64              pos;                       ///< only valid if seek_write
	UINT32              type;                      ///< file type, MP4, TS, JPG, THM
	CHAR                *p_fpath;                  ///< Specify file path (if NULL, it will callback project to get)
	UINT32              fpath_size;                 ///< Length of file path
	INT32               (*fp_pushed)(void *p_data); ///< callback for previous module release buffer
	ISF_RV              ret_push;                  ///< fileop result
	UINT32              resv[9];                  ///< reserve to meet ImageStream DESC size
} ISF_FILEOUT_BUF, *PISF_FILEOUT_BUF;
STATIC_ASSERT(sizeof(ISF_FILEOUT_BUF) / sizeof(UINT32) == ISF_DATA_DESC_SIZE);

struct _ISF_BASE_UNIT;
struct _ISF_UNIT;
struct _ISF_STREAM;

/**
    Stream state.
*/
typedef struct _ISF_STATE_ {
	struct _ISF_STREAM *pStream; ///< config for stream
	UINT32 ListID;
	ISF_PORT_STATE StateCfg; ///< new port state
	ISF_PORT_STATE State; ///< current port state
	UINT32 Dirty; ///< dirty cmd
}
ISF_STATE;

#define ISF_PORT_INFO_SIZE  24  ///< size of port-info in WORDS

/**
    Stream port.

    NOTE: if get any push or pull pDataBuf pointer, user must verify data format by Desc[0] FOURCC, then cast it to related struct pointer:

        if(pData->Desc[0] == MAKEFOURCC('V','R','A','W')) => video raw         => cast to ISF_VIDOO_RAW_BUF* (IMG_BUF*)
        if(pData->Desc[0] == MAKEFOURCC('N','V','X','1')) => video compressed  => cast to ISF_VIDOO_RAW_BUF* (IMG_BUF*)
        if(pData->Desc[0] == MAKEFOURCC('N','V','X','2')) => video compressed  => cast to ISF_VIDOO_RAW_BUF* (IMG_BUF*)
        if(pData->Desc[0] == MAKEFOURCC('A','R','A','W')) => audio raw         => cast to ISF_AUDIO_RAW_BUF*
        if(pData->Desc[0] == MAKEFOURCC('V','S','T','M')) => video stream      => cast to ISF_VIDEO_STREAM_BUF*
        if(pData->Desc[0] == MAKEFOURCC('A','S','T','M')) => audio stream      => cast to ISF_AUDIO_STREAM_BUF*
*/
typedef struct _ISF_PORT_ {
	UINT32 Sign; // ///< signature, equal to ISF_SIGN_PORT
	ISF_CONNECT_TYPE ConnectType; ///< connect type
	UINT32 iPort; ///< input-port ID of pDestUnit
	struct _ISF_UNIT *pDestUnit; ///< pointer to dest unit (cannot be NULL)
	UINT32 oPort; ///< output-port ID of pSrcUnit
	struct _ISF_UNIT *pSrcUnit; ///< pointer to src unit (NULL if disconnect)
	UINT32 Info[ISF_PORT_INFO_SIZE]; ///< vdo/aud info of this port
	UINT32 FlagCfg; ///< ctrl flag (config), default 0
	UINT32 Flag; ///< ctrl flag, default 0
	UINT32 Attr; ///< attr flag, default 0, see ISF_PORT_ATTR
	// push interface for transfer image buffer
	ISF_RV (*PushImgBufToDest)(struct _ISF_PORT_* pPort, ISF_DATA *pData, INT32 nWaitMs); ///< CB to push data to dest unit (for PUSH connect type)(provided by port-caps of pDestUnit, called by pSrcUnit)
	// pull interface for transfer image buffer
	ISF_RV (*SendNotifyToDest)(struct _ISF_PORT_* pPort, ISF_DATA *pSyncData, INT32 nWaitMs); ///< CB to send notify to dest unit (for PULL connect type)
	ISF_RV (*PullImgBufFromSrc)(struct _ISF_PORT_* pPort, ISF_DATA *pData, struct _ISF_UNIT *pDestUnit, INT32 nWaitMs);  ///< CB to pull data from src unit (for PULL connect type)(provided by port-caps of pSrcUnit, called by pDestUnit)
}
ISF_PORT;

/**
     @name attr of port
*/
#define ISF_PORT_ATTR_READYSRCBUF   0x00000001      ///< if enable this flag => never auto lock data when Push(),Pull()
#define ISF_PORT_ATTR_KEEPSRCBUF    0x00000002      ///< if enable this flag => never auto unlock data when Push(),Pull()
#define ISF_PORT_ATTR_DDR2          0x00000004      ///< if enable this flag => common blk and private pool are required from DDR2

/**
     @name flag of port
*/
#define ISF_PORT_NULL               ((ISF_PORT*)0) ///< disconnect output of port
#define ISF_PORT_EOS                ((ISF_PORT*)0xA5A5A5A5) ///< connect output of port as "end of stream"
#define ISF_PORT_KEEP               ((ISF_PORT*)0xffffffff) ///< keep output of port

/**
     @name param for "dirty of port"
*/
#define ISF_PARAM_PORT_DIRTY		0x8000000f

/**
     @name dirty of port
*/
#define ISF_INFO_CLASS_VDO			0x00010000
#define ISF_INFO_CLASS_AUD			0x00020000
#define ISF_INFO_CLASS_VBS			0x00040000
#define ISF_INFO_CLASS_ABS			0x00080000
#define ISF_INFO_DIRTY_MASK		0x0000ffff
#define ISF_INFO_DIRTY_BUFCOUNT	0x00000001
#define ISF_INFO_DIRTY_IMGSIZE		0x00000002
#define ISF_INFO_DIRTY_IMGFMT		0x00000004
#define ISF_INFO_DIRTY_FRAMERATE	0x00000008
#define ISF_INFO_DIRTY_DIRECT		0x00000010
#define ISF_INFO_DIRTY_ASPECT		0x00000020
#define ISF_INFO_DIRTY_PREWIN		0x00000040
#define ISF_INFO_DIRTY_POSTWIN		0x00000080
#define ISF_INFO_DIRTY_AUDBITS		0x00000002
#define ISF_INFO_DIRTY_AUDCH		0x00000004
#define ISF_INFO_DIRTY_SAMPLERATE	0x00000008

#define ISF_GET_WHI(v)			((UINT32)((v) >> 32))
#define ISF_GET_WLO(v)			((UINT32)(v))

/**
     @name get high half
*/
#define ISF_GET_HI(v)				((((UINT32)(v)) >> 16) & 0x0000ffff)

/**
     @name get low half
*/
#define ISF_GET_LO(v)				(((UINT32)(v)) & 0x0000ffff)

/**
     @name image size of video
*/
#define ISF_VDO_SIZE(w,h)			(((((UINT32)(w)) & 0x0000ffff)<<16) | (((UINT32)(h)) & 0x0000ffff))
#define ISF_VDO_SIZE_W_DEFAULT		0 ///< default
#define ISF_VDO_SIZE_H_DEFAULT		0 ///< default

/**
     @name image format of video
*/
typedef GX_IMAGE_PIXEL_FMT ISF_VDO_FMT; ///< image format, refer to GX_IMAGE_PIXEL_FMT
#define ISF_VDO_FMT_YUV422		    GX_IMAGE_PIXEL_FMT_YUV422_PACKED
#define ISF_VDO_FMT_YUV420		    GX_IMAGE_PIXEL_FMT_YUV420_PACKED
#define ISF_VDO_FMT_Y8		    GX_IMAGE_PIXEL_FMT_Y_ONLY
#define ISF_VDO_FMT_DEFAULT		ISF_VDO_FMT_YUV420

/**
     @name frame-rate of video
     @note INT16 dst_fr = ISF_GET_HI(pImgInfo->FramePerSecond);
           INT16 src_fr = ISF_GET_LO(pImgInfo->FramePerSecond);
           if((dst_fr==1) && (src_fr==-1))                    ==> frame is triggered by user, fps = [unknown]
           if((dst_fr> 1) && (src_fr>=1) && (dst_fr> src_fr)) ==> frame is triggered by dest port, fps = dst_fr/src_fr
           if((dst_fr>=1) && (src_fr>=1) && (dst_fr==src_fr)) ==> frame is triggered by src port, fps = upstream_fps*1/1
           if((dst_fr>=1) && (src_fr> 1) && (dst_fr< src_fr)) ==> frame is triggered by src port, fps = upstream_fps*dst_fr/src_fr
           others                                             ==> invalid
*/
#define ISF_VDO_FRC(dst_fr,src_fr)	(((((UINT32)(dst_fr)) & 0x0000ffff)<<16) | (((UINT32)(src_fr)) & 0x0000ffff))  ///< output dst frame per src frame
#define ISF_VDO_FRC_DIRECT  		ISF_VDO_FRC( 0, 0) ///< output each frame
#define ISF_VDO_FRC_ALL			ISF_VDO_FRC( 1, 1) ///< output each frame
#define ISF_VDO_FRC_NOTIFY			ISF_VDO_FRC( 1,-1) ///< output 1 frame if user notify
#define ISF_VDO_FRC_DEFAULT		ISF_VDO_FRC_DIRECT ///< default

/**
     @name aspect-ratio of video
*/
#define ISF_VDO_ASP_RATIO(x,y)     (((((UINT32)(x)) & 0x0000ffff)<<16) | (((UINT32)(y)) & 0x0000ffff)) ///< composited ratio in WORD
#define ISF_VDO_ASP_X_DEFAULT		0 ///< default
#define ISF_VDO_ASP_Y_DEFAULT		0 ///< default

/**
     @name direction of video
*/
#define ISF_VDO_DIR_NONE           0x00000000 ///< off (default value)
#define ISF_VDO_DIR_MIRRORX        0x10000000 ///< mirror in x direction
#define ISF_VDO_DIR_MIRRORY        0x20000000 ///< mirror in y direction
#define ISF_VDO_DIR_MIRRORXY       0x30000000 ///< mirror in x and y direction
#define ISF_VDO_DIR_ROTATE_MASK    0x01ffffff ///< rotate mask
#define ISF_VDO_DIR_ROTATE_0       0x00000000 ///< rotate 0 degrees clockwise
#define ISF_VDO_DIR_ROTATE_90      0x005A0000 ///< rotate 90 degrees clockwise
#define ISF_VDO_DIR_ROTATE_180     0x00B40000 ///< rotate 180 degrees clockwise
#define ISF_VDO_DIR_ROTATE_270     0x010E0000 ///< rotate 270 degrees clockwise
#define ISF_VDO_DIR_ROTATE_360     0x01680000 ///< rotate 360 degrees clockwise
#define ISF_VDO_DIR_ROTATE(n)      ((UINT32)((n)*0x10000)) ///< rotate (n) degrees clockwise
#define ISF_VDO_DIR_DEFAULT		ISF_VDO_DIR_NONE ///< default

/**
     @name fileout operation definitions (bitwise)
*/
#define ISF_FILEOUT_FOP_NONE       0x00000000 ///< Do nothing. Used for event only
#define ISF_FILEOUT_FOP_CREATE     0x00000001 ///< Create a new file (if old one exists, it will be truncated)
#define ISF_FILEOUT_FOP_CLOSE      0x00000002 ///< Close the file
#define ISF_FILEOUT_FOP_CONT_WRITE 0x00000004 ///< Write continously (from current position)
#define ISF_FILEOUT_FOP_SEEK_WRITE 0x00000008 ///< Write after seeking
#define ISF_FILEOUT_FOP_FLUSH      0x00000010 ///< Flush the data right away after writing
#define ISF_FILEOUT_FOP_DISCARD    0x00000100 ///< Discard operations which not processed yet
#define ISF_FILEOUT_FOP_SNAPSHOT   (ISF_FILEOUT_FOP_CREATE | ISF_FILEOUT_FOP_CONT_WRITE | ISF_FILEOUT_FOP_CLOSE) ///< Create a single file in one operation

/**
     Port Info: Image or Video.
*/
typedef struct _ISF_IMG_INFO_ {
	UINT32 ImgFmt; ///< image format, set by ImageUnit_SetVdoImgFormat()
	UINT32 Dirty; ///< dirty
	USIZE MaxImgSize; ///< maximum image size, assign by dest unit
	USIZE ImgSize; ///< image size, set by ImageUnit_SetVdoImgSize()
	USIZE ImgAspect; ///< image aspect size, set by ImageUnit_SetVdoAspectRatio()
	UINT32 Resv1[4]; ///<
	URECT PostWindow; ///< cast window range, set by ImageUnit_SetVdoPostWindow()
	URECT PreWindow; ///< crop window range, set by ImageUnit_SetVdoPreWindow()
	UINT32 Direct; ///< direction, set by ImageUnit_SetVdoDirection()
	UINT32 Resv0; ///<
	UINT32 FramePerSecond; ///< frame per second, set by ImageUnit_SetVdoFramerate()
	UINT32 BufferCount; ///< frame buffer count (fixed)
}
ISF_IMG_INFO;
STATIC_ASSERT(sizeof(ISF_IMG_INFO) / sizeof(UINT32) == ISF_PORT_INFO_SIZE);


/**
     Buffer format of audio.
*/
typedef enum _ISF_AUD_FMT_ {
	ISF_AUD_FMT_PCM  = 0x10,    ///< pcm format
	ENUM_DUMMY4WORD(ISF_AUD_FMT)
} ISF_AUD_FMT;

/**
     Port Info: Audio.
*/
typedef struct _ISF_AUD_INFO_ {
	UINT32 AudFmt; ///< audio format, refer to ISF_AUD_FMT
	UINT32 Dirty; ///< dirty
	UINT32 MaxBitPerSample; ///< maximum, assign by dest unit before ImageStream_SetMode(ISF_OMD_XXXXXINIT)
	UINT32 MaxChannelCount; ///< maximum, assign by dest unit before ImageStream_SetMode(ISF_OMD_XXXXXINIT)
	UINT32 MaxSamplePerSecond; ///< maximum, assign by dest unit before ImageStream_SetMode(ISF_OMD_XXXXXINIT)
	UINT32 Resv0; ///<
	UINT32 BitPerSample; ///< audio sample bits
	UINT32 ChannelCount; ///< audio channel count
	UINT32 SamplePerSecond; ///< audio sample rate, for PULL connect type, this is TargetSampleRate
	UINT32 Resv1[4]; ///<
	UINT32 Resv2[4]; ///<
	UINT32 Resv3; ///<
	UINT32 FramePerSecond; ///< frame per second
	UINT32 BufferCount; ///< frame buffer count (fixed)
}
ISF_AUD_INFO;
STATIC_ASSERT(sizeof(ISF_IMG_INFO) / sizeof(UINT32) == ISF_PORT_INFO_SIZE);


/**
     Caps of Stream port.
*/
typedef struct _ISF_PORT_CAPS {
	ISF_CONNECT_TYPE ConnectTypeCaps; ///< caps of connect type
	// push interface for transfer image buffer
	ISF_RV (*PushImgBufToDest)(struct _ISF_PORT_* pPort, ISF_DATA *pData, INT32 nWaitMs); ///< CB to push data to dest unit (for PUSH connect type)
	// pull interface for transfer image buffer
	ISF_RV (*SendNotifyToDest)(struct _ISF_PORT_* pPort, ISF_DATA *pSyncData, INT32 nWaitMs); ///< CB to send notify to dest unit (for PULL connect type)
	ISF_RV (*PullImgBufFromSrc)(struct _ISF_PORT_* pPort, ISF_DATA *pData, struct _ISF_UNIT *pDestUnit, INT32 nWaitMs);  ///< CB to pull data from src unit (for PULL connect type)
}
ISF_PORT_CAPS;

/**
    Stream i/o path (from input port to output port of a unit).
*/
typedef struct _ISF_PORT_PATH_ {
	struct _ISF_UNIT *pUnit; ///< pointer to this unit (default should not NULL)
	UINT32 iPort; ///< input port
	UINT32 oPort; ///< output port
	struct _ISF_STREAM *pStream; ///< pointer to working stream (default should be NULL)
	UINT32 Flag; ///< select
	UINT32 Resv; ///< reserved
}
ISF_PORT_PATH;

/**
     Stream Base Unit.
*/
typedef struct _ISF_BASE_UNIT {
	UINT32 (*Create)(struct _ISF_UNIT *pThisUnit, ISF_PORT *pSrcPort, CHAR* name, UINT32 blk_size, UINT32 blk_cnt);  ///< CB to create pool
	ISF_RV (*Destory)(struct _ISF_UNIT *pThisUnit, ISF_PORT *pSrcPort, UINT32 pool);  ///< CB to destory pool
	UINT32 (*NewI)(struct _ISF_UNIT *pThisUnit, ISF_PORT *pSrcPort, CHAR* name, UINT32 size, ISF_DATA *pData);  ///< CB to create pool and new data
	ISF_RV (*ReleaseI)(struct _ISF_UNIT *pThisUnit, ISF_DATA *pData, ISF_RV Result);  ///< CB to free data and destory pool
	UINT32 (*New)(struct _ISF_UNIT *pThisUnit, ISF_PORT *pSrcPort, UINT32 pool, UINT32 attr, UINT32 size, ISF_DATA *pData);  ///< CB to new data and reference =1
	ISF_RV (*Add)(struct _ISF_UNIT *pThisUnit, ISF_PORT *pSrcPort, ISF_DATA *pData);  ///< CB to reference++
	ISF_RV (*Release)(struct _ISF_UNIT *pThisUnit, ISF_PORT *pSrcPort, ISF_DATA *pData, ISF_RV Result);  ///< CB to reference--, and free data if reference ==0
	ISF_RV (*Push)(struct _ISF_UNIT *pThisUnit, ISF_PORT *pDestPort, ISF_DATA *pData, INT32 nWaitMs);  ///< CB to push data
	ISF_RV (*Pull)(struct _ISF_UNIT *pThisUnit, ISF_PORT *pSrcPort, ISF_DATA *pData, INT32 nWaitMs);  ///< CB to pull data
	ISF_RV (*Notify)(struct _ISF_UNIT *pThisUnit, ISF_PORT *pSrcPort, ISF_DATA *pData, INT32 nWaitMs, ISF_RV Result, ISF_PORT *pDestPort);  ///< CB to notify & sync data
}
ISF_BASE_UNIT;

/**
     Stream Unit.
*/
typedef struct _ISF_UNIT {
	UINT32 Sign; ///< signature, equal to ISF_SIGN_UINT
	CHAR *Name; ///< name string
	UINT64 Module; ///< nvtmpp module name
	UINT32 ListID; ///< id of stream working list
	UINT32 nIn; ///< input-port count
	UINT32 nOut; ///< output-port count
	UINT32 nPath; ///< input-output path count
	ISF_PORT **In; ///< input-port pointer array (active)
	ISF_PORT **Out; ///< output-port pointer array (active)
	ISF_PORT **OutCfg; ///< output-port pointer array (config)
	ISF_PORT_CAPS **InCaps; ///< input-port-caps pointer array
	ISF_PORT_CAPS **OutCaps; ///< output-port-caps pointer array
	ISF_STATE **OutState; ///< output-port pointer array (active)
	ISF_PORT_PATH *Path; ///< input-output path array
	UINT32 FlagCfg; ///< flag (config)
	UINT32 Flag; ///< flag
	UINT32 Lock; ///< lock state
	UINT32 Dirty; ///< dirty
	UINT32 Attr; ///< attr of CTRL port
	void *RefData; ///< reference data pointer
	ISF_BASE_UNIT *pBase; ///< base interface
	// interface for connect/disconnect
	ISF_RV(*BindInput)(struct _ISF_UNIT *pThisUnit, UINT32 iPort, struct _ISF_UNIT *pSrcUnit, UINT32 oPort);  ///< CB to check input port
	ISF_RV(*BindOutput)(struct _ISF_UNIT *pThisUnit, UINT32 oPort, struct _ISF_UNIT *pDestUnit, UINT32 iPort);  ///< CB to check output port
	// interface after connect
	ISF_RV(*SetParam)(struct _ISF_UNIT *pThisUnit, UINT32 param, UINT32 value);  ///< CB to set parameter of ISF_CTRL port
	UINT32(*GetParam)(struct _ISF_UNIT *pThisUnit, UINT32 param);  ///< CB to get parameter of ISF_CTRL port
	ISF_RV(*SetPortParam)(struct _ISF_UNIT *pThisUnit, UINT32 nPort, UINT32 param, UINT32 value);  ///< CB to set parameter of ISF_INxxx or ISF_OUTxxx port
	UINT32(*GetPortParam)(struct _ISF_UNIT *pThisUnit, UINT32 nPort, UINT32 param);  ///< CB to get parameter of ISF_INxxx or ISF_OUTxxx port
	ISF_RV(*UpdatePort)(struct _ISF_UNIT *pThisUnit, UINT32 oPort, ISF_PORT_CMD cmd);  ///< CB to update a ISF_OUTxxx port
	BOOL (*Debug)(CHAR *strCmd);
}
ISF_UNIT;

/**
     Stream.
*/
#define ISF_ROOTLIST_SIZE			16		///< max count of root ports
#define ISF_CONNECTLIST_SIZE		256		///< max count of connected ports

typedef struct _ISF_CONNECT {
	ISF_UNIT *pUnit;
	UINT32 oPort;
}
ISF_CONNECT;

typedef struct _ISF_STREAM {
	UINT32 Sign; ///< signature, equal to ISF_SIGN_STREAM
	//MEM_RANGE* pMem;
	CHAR *Name;
	UINT32 Lock;
	UINT32 Flag;
	UINT32 FlagCfg;
	UINT32 Dirty;
	UINT32 nRootCount;
	UINT32 nLeafCount;
	UINT32 nConnectCount;
	UINT32 QueueHead;
	UINT32 QueueTail;
	ISF_PORT *RootList[ISF_ROOTLIST_SIZE];
	ISF_CONNECT ConnectList[ISF_CONNECTLIST_SIZE];
}
ISF_STREAM;


///////////////////////////////////////////////////////////////////////////////
/**
     Get input port of a unit.

     Get input port of a unit.

     @note This function is returning pointer to port immediately.

     @param[in] pUnit           pointer to unit.
     @param[in] iPort           input port ID. (see ISF_IN)
     @return                    pointer to port.
*/
extern ISF_PORT *ImageUnit_In(ISF_UNIT *pUnit, UINT32 iPort);

/**
     Get output port of a unit.

     Get output port of a unit.

     @note This function is returning pointer to port immediately.

     @param[in] pUnit           pointer to unit.
     @param[in] oPort           output port ID. (see ISF_OUT)
     @return                    pointer to port.
*/
extern ISF_PORT *ImageUnit_Out(ISF_UNIT *pUnit, UINT32 oPort);

/**
     Begin to get/set a unit.

     Begin to get/set a unit.

     @note This function is effective immediately.
           But all operation between ImageUnit_Begin and ImageUnit_End will effective after user call ImageStream_UpdateAll().

     @param[in] pUnit           pointer to unit.
     @param[in] flag            flag of unit.
*/
extern void ImageUnit_Begin(ISF_UNIT *pUnit, UINT32 flag);

/**
     End to get/set a unit.

     End to get/set a unit.

     @note This function is effective immediately.
           But all operation between ImageUnit_Begin and ImageUnit_End will effective after user call ImageStream_UpdateAll().
*/
extern void ImageUnit_End(void);

/**
     Set input/output port connect-type.

     Set input/output port connect-type before connect.

     @note This function is effective immediately.
           iPort always work!, but oPort work if already SetOutput()!

     @param[in] nPort           input/output port ID. (see ISF_IN and ISF_OUT)
     @param[in] pDest           pointer to destination port.
     @param[in] ConnectType     connect type. (see ISF_CONNECT_TYPE)

*/
extern void ImageUnit_SetConnectType(UINT32 nPort, UINT32 ConnectType);
extern void ImageUnit_GetConnectType(ISF_UNIT *pUnit, UINT32 nPort, UINT32* ConnectType);

/**
     Set output port connect destination, and output port state.

     Set output port connect destination, and output port state.

     @note This parameter is effective after user call ImageStream_UpdateAll().

     @param[in] oPort           output port ID. (see ISF_OUT)
     @param[in] pDest           pointer to destination port.
     @param[in] State           state of this port.
*/
extern void ImageUnit_SetOutput(UINT32 nPort, ISF_PORT *pDest, ISF_PORT_STATE State);
extern void ImageUnit_GetOutput(ISF_UNIT *pUnit, UINT32 nPort, ISF_PORT **pDest, ISF_PORT_STATE* pState);

/**
     Set video image size of an input/output port..

     Set video image size of an input/output port.

     @note This parameter is effective after user call ImageStream_UpdateAll().
           The image size is always effective on input port and output port.

     @param[in] nPort           input port ID or output port ID. (see ISF_IN and ISF_OUT)
     @param[in] img_w           image size width.
     @param[in] img_h           image size height.
*/
extern void ImageUnit_SetVdoImgSize(UINT32 nPort, UINT32 img_w, UINT32 img_h);
extern void ImageUnit_GetVdoImgSize(ISF_UNIT *pUnit, UINT32 nPort, UINT32* img_w, UINT32* img_h);

/**
     Set video image format of an input/output port

     Set video image format of an input/output port.

     @note This parameter is effective after user call ImageStream_UpdateAll().
           The image size is always effective on input port and output port.

     @param[in] nPort           input port ID or output port ID. (see ISF_IN and ISF_OUT)
     @param[in] format          image format.
*/
extern void ImageUnit_SetVdoImgFormat(UINT32 nPort, UINT32 format);
extern void ImageUnit_GetVdoImgFormat(ISF_UNIT *pUnit, UINT32 nPort, UINT32* format);

/**
     Set video pre-window of an input port.

     Set video pre-window of an input port.

     @note This parameter is effective after user call ImageStream_UpdateAll().
           The pre-window is always effective on input port of destination unit.

     @param[in] nPort           input port ID or output port ID. (see ISF_IN and ISF_OUT)
     @param[in] win_x           window x-offset.
     @param[in] win_y           window y-offset.
     @param[in] win_w           window width.
     @param[in] win_h           window height.
*/
extern void ImageUnit_SetVdoPreWindow(UINT32 nPort, UINT32 win_x, UINT32 win_y, UINT32 win_w, UINT32 win_h);
extern void ImageUnit_GetVdoPreWindow(ISF_UNIT *pUnit, UINT32 nPort, UINT32* win_x, UINT32* win_y, UINT32* win_w, UINT32* win_h);

/**
     Set video post-window of an output port.

     Set video post-window of an output port.

     @note This parameter is effective after user call ImageStream_UpdateAll().
           The post-window is always effective on input port of destination unit.

     @param[in] nPort           input port ID or output port ID. (see ISF_IN and ISF_OUT)
     @param[in] win_x           window x-offset.
     @param[in] win_y           window y-offset.
     @param[in] win_w           window width.
     @param[in] win_h           window height.
*/
extern void ImageUnit_SetVdoPostWindow(UINT32 nPort, UINT32 win_x, UINT32 win_y, UINT32 win_w, UINT32 win_h);
extern void ImageUnit_GetVdoPostWindow(ISF_UNIT *pUnit, UINT32 nPort, UINT32* win_x, UINT32* win_y, UINT32* win_w, UINT32* win_h);

/**
     Set video aspect ratio of an input/output port.

     Set video aspect ratio of an input/output port.

     @note This parameter is effective after user call ImageStream_UpdateAll().
           The aspect ratio is always effective on input port and output port.

     @param[in] nPort           input port ID or output port ID. (see ISF_IN and ISF_OUT)
     @param[in] img_ratio_x     image aspect ratio in x direction.
     @param[in] img_ratio_y     image aspect ratio in y direction.
*/
extern void ImageUnit_SetVdoAspectRatio(UINT32 nPort, UINT32 img_ratio_x, UINT32 img_ratio_y);
extern void ImageUnit_GetVdoAspectRatio(ISF_UNIT *pUnit, UINT32 nPort, UINT32* img_ratio_x, UINT32* img_ratio_y);

/**
     Set video direction of an input/output port.

     Set video direction of an input/output port.

     @note This parameter is effective after user call ImageStream_UpdateAll().
           The direction is always effective on input port and output port.

     @param[in] nPort           input port ID or output port ID. (see ISF_IN and ISF_OUT)
     @param[in] img_dir         image direction flag.
*/
extern void ImageUnit_SetVdoDirection(UINT32 nPort, UINT32 img_dir);
extern void ImageUnit_GetVdoDirection(ISF_UNIT *pUnit, UINT32 nPort, UINT32* img_dir);

/**
     Set video framerate of an input/output port.

     Set video framerate of an input/output port.

     @note This parameter is effective after user call ImageStream_UpdateAll().
           The direction is always effective on input port and output port.

     @param[in] nPort           input port ID or output port ID. (see ISF_IN and ISF_OUT)
     @param[in] framerate       framerate or ISF_VDO_FRC(), ISF_VDO_FRC_DIRECT, ISF_VDO_NOTIFY.
*/
extern void ImageUnit_SetVdoFramerate(UINT32 nPort, UINT32 framerate);
extern void ImageUnit_GetVdoFramerate(ISF_UNIT *pUnit, UINT32 nPort, UINT32* framerate);

/**
     Set video buffer-count of an input/output port.

     Set video buffer-count of an input/output port.

     @note This parameter is effective after user call ImageStream_UpdateAll(), and need set before port READY.
           The direction is always effective on input port and output port.

     @param[in] nPort           input port ID or output port ID. (see ISF_IN and ISF_OUT)
     @param[in] buf_count       count of buffer.
*/
extern void ImageUnit_SetVdoBufferCount(UINT32 nPort, UINT32 buf_count);
extern void ImageUnit_GetVdoBufferCount(ISF_UNIT *pUnit, UINT32 nPort, UINT32* buf_count);

/**
     Set audio sampling of an input/output port.

     Set audio sampling of an input/output port.

     @note This parameter is effective after user call ImageStream_UpdateAll().
           The direction is always effective on input port and output port.

     @param[in] nPort           input port ID or output port ID.
     @param[in] aud_bit_per_sample          bit per sample.
     @param[in] aud_channel_count           channel count.
     @param[in] aud_sample_per_second       sample per second.
     @return                    result. (see ISF_RV)
*/
extern void ImageUnit_SetAudSample(UINT32 nPort, UINT32 bit_per_sample, UINT32 channel_count, UINT32 sample_per_second);
extern void ImageUnit_GetAudSample(ISF_UNIT *pUnit, UINT32 nPort, UINT32* bit_per_sample, UINT32* channel_count, UINT32* sample_per_second);

extern void ImageUnit_SetAttr(UINT32 nPort, UINT32 attr);
extern void ImageUnit_GetAttr(ISF_UNIT *pUnit, UINT32 nPort, UINT32* attr);

/**
     Set general parameter of an input/output port.

     Set general parameter of an input/output port.

     @note None.
     @note If this parameter is a IMM parameter, it will effective immediately.
           Otherwise, this parameter is effective after user call ImageStream_UpdateAll().
           If nPort is input port ID, the parameter will affect only affect on this input port.
           If nPort is output port ID, the parameter will affect only affect on this output port.
           If nPort is control port ID, the parameter will affect all input and output ports of this unit.

     @param[in] nPort           input port ID or output port ID or control ID.
     @param[in] param           parameter id.
     @param[in] value           parameter value.
     @return                    result. (see ISF_RV)
*/
extern ISF_RV ImageUnit_SetParam(UINT32 nPort, UINT32 param, UINT32 value);

/**
     Get general parameter of an input/output port.

     Get general parameter of an input/output port.

     @note This function is returning current parameter immediately.

     @param[in] pUnit           pointer to unit.
     @param[in] param           parameter id.
     @return                    parameter value.
*/
extern UINT32 ImageUnit_GetParam(ISF_UNIT *pUnit, UINT32 nPort, UINT32 param);

/**
     New a data and add reference to it.

     New a data and add reference to it.

     @note This function is effective immediately.
           The data is aquired from public common pools of nvtmpp.
           If success, the public data will add 1 to reference count of "User".

     @param[in] size            public data size (bytes).
     @param[in] pData           pointer to ISF_DATA.
     @return                    address of public data.
*/
extern UINT32 ImageUnit_NewData(UINT32 size, ISF_DATA *pData);

/**
     Add reference to a data.

     Add reference to a data.

     @note This function is effective immediately.
           If success, the public data will add 1 to reference count of current owner.

     @param[in] pData           pointer to ISF_DATA.
     @return                    result. (see ISF_RV)
*/
extern ISF_RV ImageUnit_AddData(ISF_DATA *pData);

/**
     Remove reference to a data.

     Remove reference to a data.

     @note This function is effective immediately.
           If success, the public data will subtract 1 from reference count of current owner.
           If success, and finally total reference count of this public data is equal to 0, this buffer will be freed to public common pool of nvtmpp.

     @param[in] pData           pointer to ISF_DATA.
     @return                    result. (see ISF_RV)
*/
extern ISF_RV ImageUnit_ReleaseData(ISF_DATA *pData);

/**
     Check if allow push data to destination of this port.

     Check if allow push data to destination of this port.

     @note This function is returning current status immediately.

     @param[in] pDestPort       pointer to destination port.
     @return                    TRUE/FALSE.
*/
extern BOOL ImageUnit_IsAllowPush(ISF_PORT *pDestPort);

/**
     Push a data to destination of this port.

     Push a data to destination of this port.

     @note This function is effective immediately.
           If success, the public data will add 1 to reference count of destination unit, and subtract 1 from reference count of "User".

     @param[in] pDestPort       pointer to destination port.
     @param[in] pData           pointer to ISF_DATA.
     @param[in] nWaitMs         nWaitMs < 0, NON-BLOCK
                                nWaitMs = 0, BLOCK
                                nWaitMs > 0, BLOCK until n(ms) is timeout
     @return                    result. (see ISF_RV)
*/
extern ISF_RV ImageUnit_PushData(ISF_PORT *pDestPort, ISF_DATA *pData, INT32 nWaitMs);

/**
     Check if allow pull data from source of this port.

     Check if allow pull data from source of this port.

     @note This function is returning current status immediately.

     @param[in] pDestPort       pointer to source port.
     @return                    TRUE/FALSE.
*/
extern BOOL ImageUnit_IsAllowPull(ISF_PORT *pSrcPort);

/**
     Pull a data from source of this port.

     Pull a data from source of this port.

     @note This function is effective immediately.
           If success, the public data will add 1 to reference count of destination unit, and subtract 1 from reference count of "User".

     @param[in] pSrcPort        pointer to source port.
     @param[in] pData           pointer to data.
     @param[in] nWaitMs         nWaitMs < 0, NON-BLOCK
                                nWaitMs = 0, BLOCK
                                nWaitMs > 0, BLOCK until n(ms) is timeout
     @return                    result. (see ISF_RV)
*/
extern ISF_RV ImageUnit_PullData(ISF_PORT *pSrcPort, ISF_DATA *pData, INT32 nWaitMs);

/**
     Check if allow notify to source of this port.

     Check if allow notify to source of this port.

     @note This function is returning current status immediately.

     @param[in] pDestPort       pointer to source port.
     @return                    TRUE/FALSE.
*/
extern BOOL ImageUnit_IsAllowNotify(ISF_PORT *pSrcPort);

/**
     Notify to source of this port.

     Notify to source of this port.

     @note None.
     @note This function is effective immediately.

     @param[in] pSrcPort        pointer to source port.
     @return                    result. (see ISF_RV)
*/
extern ISF_RV ImageUnit_Notify(ISF_PORT *pSrcPort, ISF_DATA* pSyncData);

extern ISF_RV ImageUnit_SendEvent(ISF_PORT *pDestPort, UINT32 event, UINT32 param1, UINT32 param2, UINT32 param3);


///////////////////////////////////////////////////////////////////////////////
/**
     Check a stream is created or not.

     Check a stream is created or not.

     @note This function is returning current status immediately

     @param[in] pStream         pointer to a stream.
     @return                    result. (see ISF_RV)
*/
extern ISF_RV ImageStream_IsOpen(ISF_STREAM *pStream);

/**
     Create a stream.

     Create a stream.

     @note This function is effective immediately.
           In default, all root ports of a stream are NULL.

     @param[in] pStream         pointer to a stream.
     @return                    result. (see ISF_RV)
*/
extern ISF_RV ImageStream_Open(ISF_STREAM *pStream);

/**
     Destroy a stream.

     Destroy a stream.

     @note This function is effective immediately.
           It will auto change state to OFF state and disconnect all ports of this stream.

     @param[in] pStream         pointer to a stream.

     @return                    result. (see ISF_RV)
*/
extern ISF_RV ImageStream_Close(ISF_STREAM *pStream);

/**
     Begin to modify graph of a stream.

     Begin to modify graph of a stream.

     @note This function is effective immediately.

     @param[in] pStream         pointer to a stream.
*/
extern void ImageStream_Begin(ISF_STREAM *pStream, UINT32 flag);

/**
     End to modify graph of a stream.

     End to modify graph of a stream.

     @note This function is effective immediately.

     @param[in] pStream         pointer to a stream.
*/
extern void ImageStream_End(void);

/**
     Set root of graph of a stream.

     Set root of graph of a stream.

     @note This function is effective immediately.

     @param[in] iPort           root port ID.
     @param[in] pSrc            pointer to input port of root unit.
*/
extern void ImageStream_SetRoot(UINT32 iPort, ISF_PORT *pSrc);
extern ISF_PORT *ImageStream_GetRoot(ISF_STREAM *pStream, UINT32 id);

/**
     Set state to ports of downstream of a stream.

     Set state to ports of downstream of a stream.

     @note This function is effective immediately.
*/
extern void ImageStream_SetOutputDownstream(ISF_PORT *pOutput, UINT32 new_state);

/**
     Sync info by state to ports of downstream of a stream.

     Sync info by state to ports of downstream of a stream.

     @note This function is effective immediately.
*/
extern void ImageStream_SyncOutputDownstream(ISF_PORT *pOutput, UINT32 sync_state);

/**
     Query self output port.

     Query input port of unit is connected to which output port of itself.

     @note This function is effective immediately.
*/
extern UINT32 ImageUnit_QueryOutputPort(ISF_UNIT *pUnit, UINT32 iPort);

/**
     Query self input port.

     Query output port of unit is connected to which input port of itself.

     @note This function is effective immediately.
*/
extern UINT32 ImageUnit_QueryInputPort(ISF_UNIT *pUnit, UINT32 oPort);

/**
     Query connected input port.

     Query output port of src_unit is connected to which input port of dest_unit.

     @note This function is effective immediately.
*/
extern UINT32 ImageUnit_QueryDownstreamInputPort(ISF_UNIT *pSrcUnit, UINT32 oPort, ISF_UNIT *pDestUnit);

/**
     Query connected output port.

     Query input port of dest_unit is connected to which output port of src_unit.

     @note This function is effective immediately.
*/
extern UINT32 ImageUnit_QueryUpstreamOutputPort(ISF_UNIT *pSrcUnit, UINT32 iPort, ISF_UNIT *pDestUnit);

extern UINT32 ImageUnit_QueryUpstreamFramerate(ISF_UNIT *pSrcUnit, UINT32 iPort, ISF_UNIT *pDestUnit);


/**
     Update last operation to all ports of a stream.

     Update last operation to all ports of a stream.

     @note This function is effective immediately.
           This function will update connecting change from ImageUnit_SetOutput.
           This function will update state change from ImageUnit_SetOutput.
           This function will update parameter change from ImageUnit_SetVdoXXXX, ImageUnit_SetAudSample and ImageUnit_SetParam.

     @param[in] pStream         pointer to a stream.
     @return                    result. (see ISF_RV)
*/
extern ISF_RV ImageStream_UpdateAll(ISF_STREAM *pStream);

extern ISF_STREAM ISF_Stream[ISF_MAX_STREAM];

//@}

//framework level debug msg API
extern void ISF_DEBUG(UINT32 Class, ISF_UNIT *pThisUnit, UINT32 nPort, const char *fmtstr, ...);

//Class mask of ISF_DEBUG() and "isf debug" cmd
#define ISF_DEBUG_STATE_COMMAND	0x100 //dispatch connect/disconnect, open/close, start/stop, sleep/wakeup
#define ISF_DEBUG_STATE_CTYPE		0x200 //set connect type
#define ISF_DEBUG_STATE_OUTPUT		0x400 //set output, state
#define ISF_DEBUG_STATE_AUTOAPPLY	0x800 //do auto stop/start, do auto close/open
#define ISF_DEBUG_PARAM_CTRL		0x010 //set control parameters
#define ISF_DEBUG_PARAM_GENERAL	0x020 //set port general parameters
#define ISF_DEBUG_PARAM_VDOFMT		0x040 //set port video imgsize, fmt, prewin, postwin, aspect, direct
#define ISF_DEBUG_PARAM_VDOFR		0x080 //set port video framerate
#define ISF_DEBUG_PARAM_VDOCODEC	0x001 //set port video codec and related
#define ISF_DEBUG_PARAM_AUDFMT		0x002 //set port audio bps, channel
#define ISF_DEBUG_PARAM_AUDSR		0x004 //set port audio samplerate
#define ISF_DEBUG_PARAM_AUDCODEC	0x008 //set port audio codec and related

//unit level debug msg API
extern void ISF_TRACE(UINT32 Class, ISF_UNIT *pThisUnit, UINT32 nPort, const char *fmtstr, ...);

//Class mask of ISF_TRACE() and "isf trace" cmd
#define ISF_TRACE_STATE_COMMAND	0x100 //apply connect/disconnect, open/close, start/stop, sleep/wakeup
#define ISF_TRACE_PARAM_CTRL		0x010 //apply control parameters
#define ISF_TRACE_PARAM_GENERAL	0x020 //apply port general parameters
#define ISF_TRACE_PARAM_VDOFMT		0x040 //apply port video imgsize, fmt, prewin, postwin, aspect, direct
#define ISF_TRACE_PARAM_VDOFR		0x080 //apply port video framerate
#define ISF_TRACE_PARAM_VDOCODEC	0x001 //apply port video codec and related
#define ISF_TRACE_PARAM_AUDFMT		0x002 //apply port audio bps, channel
#define ISF_TRACE_PARAM_AUDSR		0x004 //apply port audio samplerate
#define ISF_TRACE_PARAM_AUDCODEC	0x008 //apply port audio codec and related

//Class mask of "isf probe" cmd
#define _ISF_PROBE_NEW_OK			0x100 //NEW OK (0)
#define _ISF_PROBE_ADD_OK			0x200 //ADD OK (0)
#define _ISF_PROBE_PUSH_OK			0x010 //PUSH OK (0)
#define _ISF_PROBE_RELEASE_OK		0x001 //RELEASE OK (0)
#define _ISF_PROBE_NEW_FAIL		0x400 //ISF_ERR_BUFFER_CREATE (-40)
#define _ISF_PROBE_ADD_FAIL		0x800 //ISF_ERR_BUFFER_ADD (-41)
#define _ISF_PROBE_INACTIVE_STATE	0x020 //ISF_ERR_INACTIVE_STATE (-6)
#define _ISF_PROBE_INVALID_DATA	0x002 //ISF_ERR_INVALID_DATA (-19)
#define _ISF_PROBE_QUEUE_FULL		0x004 //ISF_ERR_QUEUE_FULL (-46)
#define _ISF_PROBE_PROCESS_FAIL	0x008 //ISF_ERR_PROCESS_FAIL (-47)


#define DBG_FPS(pThisUnit, port_str, i, action_str) \
	{ \
		static UINT64 fps_t1, fps_t2; \
		static INT32 fps_diff,fps_count=0; \
	    if(fps_count == 0) \
	        fps_t1 = HwClock_GetLongCounter(); \
	    fps_count++; \
	    fps_t2 =  HwClock_GetLongCounter(); \
	    fps_diff = ISF_GET_WHI(fps_t2)-ISF_GET_WHI(fps_t1); \
	    if(fps_diff>0) \
	    { \
	        float fps = (((float)(fps_count-1))*1000000)/(((float)fps_diff)*1000000 + ISF_GET_WLO(fps_t2) - ISF_GET_WLO(fps_t1)); \
			DBG_DUMP("^C\"%s\"."port_str"[%d] "action_str"! -- %f Frame/sec\r\n", (pThisUnit)->Name,(i), fps);  \
		    fps_count = 0; \
	    } \
	}

//cmd
extern void ImageStream_InstallCmd(void);

#endif //_IMAGESTREAM_H_
