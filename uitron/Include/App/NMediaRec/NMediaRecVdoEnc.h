#ifndef _NMEDIARECVDOENC_H
#define _NMEDIARECVDOENC_H

#include "NvtMediaInterface.h"

typedef enum {
	NMI_VDOENC_PARAM_ALLOC_SIZE,
	NMI_VDOENC_PARAM_ENCODER_OBJ,
	NMI_VDOENC_PARAM_MEM_RANGE,					//codec + enc buf range
	NMI_VDOENC_PARAM_ENCBUF_ADDR,				//enc buf addr
	NMI_VDOENC_PARAM_ENCBUF_SIZE,				//enc buf size
	NMI_VDOENC_PARAM_MAX_MEM_INFO,
	NMI_VDOENC_PARAM_ALLOC_SNAPSHOT_BUF,
	NMI_VDOENC_PARAM_SNAPSHOT,
	NMI_VDOENC_REG_CB,
	NMI_VDOENC_PARAM_RECFORMAT,                 //MEDIAREC_GOLFSHOT, MEDIAREC_TIMELAPSE
	NMI_VDOENC_PARAM_FILETYPE,
	NMI_VDOENC_PARAM_WIDTH,
	NMI_VDOENC_PARAM_HEIGHT,
	NMI_VDOENC_PARAM_FRAMERATE,
	NMI_VDOENC_PARAM_ENCBUF_MS,				    //enc buf length (unit: ms)
	NMI_VDOENC_PARAM_ENCBUF_RESERVED_MS,        //enc buf reserved length (unit: ms)
	NMI_VDOENC_PARAM_CODEC,
	NMI_VDOENC_PARAM_PROFILE,
	NMI_VDOENC_PARAM_GOPTYPE,
	NMI_VDOENC_PARAM_INITQP,
	NMI_VDOENC_PARAM_MINQP,
	NMI_VDOENC_PARAM_MAXQP,
	NMI_VDOENC_PARAM_MIN_I_RATIO,				//the ratio of I frame min size to bitrate
	NMI_VDOENC_PARAM_MIN_P_RATIO,				//the ratio of P frame min size to bitrate
	NMI_VDOENC_PARAM_MAX_FRAME_QUEUE,
	NMI_VDOENC_PARAM_CBRINFO,
	NMI_VDOENC_PARAM_EVBRINFO,
	NMI_VDOENC_PARAM_VBRINFO,
	NMI_VDOENC_PARAM_FIXQPINFO,
	NMI_VDOENC_PARAM_AQINFO,
	NMI_VDOENC_PARAM_3DNR_CB,
	NMI_VDOENC_PARAM_TARGETRATE,
	NMI_VDOENC_PARAM_DAR,
	NMI_VDOENC_PARAM_ROI,
	NMI_VDOENC_PARAM_START_SMART_ROI,
	NMI_VDOENC_PARAM_WAIT_SMART_ROI,
	NMI_VDOENC_PARAM_SMART_ROI,
	NMI_VDOENC_PARAM_DIS,
	NMI_VDOENC_PARAM_TV_RANGE,
	NMI_VDOENC_PARAM_START_TIMER_BY_MANUAL,
	NMI_VDOENC_PARAM_START_TIMER,
	NMI_VDOENC_PARAM_STOP_TIMER,
	NMI_VDOENC_PARAM_DROP_FRAME,
	NMI_VDOENC_PARAM_SKIP_FRAME,
	NMI_VDOENC_PARAM_SVC,
	NMI_VDOENC_PARAM_LTR,
	NMI_VDOENC_PARAM_SEI,
	NMI_VDOENC_PARAM_MULTI_TEMPORARY_LAYER,		//smart265 multi temporary layer
	NMI_VDOENC_PARAM_USRQP,
	NMI_VDOENC_PARAM_ROTATE,                    //rotate enc(0:disable, 1:counter clockwise, 2:clockwise)
	NMI_VDOENC_PARAM_TIMELAPSE_TIME,
	NMI_VDOENC_PARAM_IMM_CB,                    //register callback function for the events which should be processed immediately
	NMI_VDOENC_PARAM_GOPNUM,
	NMI_VDOENC_PARAM_FRAME_INTERVAL,
	NMI_VDOENC_PARAM_UNLOCK_BS_ADDR,
	NMI_VDOENC_PARAM_TRIGGER_MODE,				//trigger mode (timer, direct, or notify)
	NMI_VDOENC_PARAM_RESET_IFRAME,				//reset hw to start encoding i frame
	NMI_VDOENC_PARAM_RESET_SEC,					//reset sec info (for displaying recoding time)
	NMI_VDOENC_PARAM_TIMERRATE_IMM,
	NMI_VDOENC_PARAM_TIMERRATE2_IMM,
} NMI_VDOENC_PARAM;

typedef enum {
	NMI_VDOENC_ACTION_START,
	NMI_VDOENC_ACTION_STOP,
	NMI_VDOENC_ACTION_MAX,
} NMI_VDOENC_ACTION;

typedef enum {
	NMI_VDOENC_EVENT_INFO_READY,				//notify video header ready
	NMI_VDOENC_EVENT_BS_CB,						//notify enc bs ready
	NMI_VDOENC_EVENT_FRAME_INTERVAL,			//notify every frame_rate*seamless_sec
	NMI_VDOENC_EVENT_3DNR_ENC_SETTING,			//get current 3dnr level from ap
	NMI_VDOENC_EVENT_STAMP_CB,					//notify to draw stamp
	NMI_VDOENC_EVENT_SEC_CB,					//notify to update displaying sec
	NMI_VDOENC_EVENT_NOTIFY_IPL,				//notify ipl to trigger
	NMI_VDOENC_EVENT_MAX,
	ENUM_DUMMY4WORD(NMI_VDOENC_EVENT)
} NMI_VDOENC_EVENT;

typedef enum {
	NMI_VDOENC_TRIGGER_TIMER,					//for DVCam by internal timer
	NMI_VDOENC_TRIGGER_DIRECT,					//for IPCam direct input trigger
	NMI_VDOENC_TRIGGER_NOTIFY,					//for IPCam with two buf
	NMI_VDOENC_TRIGGER_MAX,
	ENUM_DUMMY4WORD(NMI_VDOENC_TRIGGER)
} NMI_VDOENC_TRIGGER;

typedef struct {
	UINT32 Addr;    							//memory buffer starting address
	UINT32 Size;    							//memory buffer size
} NMI_VDOENC_MEM_RANGE, *PNMI_VDOENC_MEM_RANGE;

typedef struct {
	UINT32 PathID;          					//path id
	UINT32 BufID;           					//memory block id
	UINT32 IsKey;           					//video is key frame or not
	UINT32 IsIDR2Cut;       					//video is idr to cut here
	UINT32 FrameType;       					//video frame type (0:P, 1:B, 2:I, 3:IDR, 4:KP)
	UINT32 Addr;            					//1st output bit-stream address
	UINT32 Size;            					//1st output bit-stream size
	UINT32 Addr2;            					//2nd output bit-stream address
	UINT32 Size2;            					//2nd output bit-stream address
	UINT32 RawYAddr;        					//raw y address
	UINT32 SVCSize;         					//svc header size
	UINT32 TemporalId;      					//svc temporal layer id (0, 1, 2)
	UINT32 Occupied;        					//raw yuv frame need to be encoded again
	UINT64 TimeStamp;       					//timestamp
	UINT32 FrameCount;      					//frame count
	UINT32 SrcOutYAddr;     					//src out y addr
	UINT32 SrcOutUVAddr;						//src out uv addr
	UINT32 SrcOutWidth;     					//src out width
	UINT32 SrcOutHeight;    					//src out height
	UINT32 SrcOutYLoft;     					//src out y line offset
	UINT32 SrcOutUVLoft;    					//src out uv line offset
#if ISF_LATENCY_DEBUG
	//Check latency og stream
	UINT64 enc_timestamp_start;
	UINT64 enc_timestamp_end;
#endif
} NMI_VDOENC_BS_INFO, *PNMI_VDOENC_BS_INFO;

typedef enum {
	NMR_VDOENC_UNKNOWN,
	NMR_VDOENC_MJPG,
	NMR_VDOENC_H264,
	NMR_VDOENC_H265,
	ENUM_DUMMY4WORD(NMR_VDOENC_CODEC)
} NMR_VDOENC_CODEC;

typedef enum {
	NMR_VDOENC_P_SLICE = 0,
	NMR_VDOENC_B_SLICE = 1,
	NMR_VDOENC_I_SLICE = 2,
	NMR_VDOENC_IDR_SLICE = 3,
	NMR_VDOENC_KP_SLICE = 4
} NMR_VDOENC_SLICE_TYPE;

typedef struct {
	UINT32 				uiCodec;
	UINT32				uiWidth;
	UINT32				uiHeight;
	UINT32				uiTargetByterate;
	UINT32				uiEncBufMs;
	UINT32				uiRecFormat;
	UINT32				uiSVCLayer;
	UINT32				uiLTRInterval;
	UINT32				uiRotate;
	BOOL				bAllocSnapshotBuf;
	BOOL				bRelease;				//release max buf
	UINT32				uiSnapShotSize;			//out: snapshot size
	UINT32				uiCodecsize;			//out: codec size
	UINT32				uiEncsize;				//out: enc buf size
} NMI_VDOENC_MAX_MEM_INFO, *PNMI_VDOENC_MAX_MEM_INFO;


typedef UINT32 (NMI_VDOENC_CB)(UINT32 event, UINT32 param);
typedef void (NMI_VDOENC_IMM_CB)(UINT32 type, UINT32 p1, UINT32 p2, UINT32 p3);

extern void             NMR_VdoEnc_AddUnit(void);


#endif //_NMEDIARECVDOENC_H
