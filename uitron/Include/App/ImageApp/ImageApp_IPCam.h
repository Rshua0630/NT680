/*
    Copyright   Novatek Microelectronics Corp. 2017.  All rights reserved.

    @file       ImageApp_IPCam.h
    @ingroup    mIImageApp

    @note       Nothing.

    @date       2017/03/31
*/

#ifndef IMAGEAPP_IPCAM_H
#define IMAGEAPP_IPCAM_H

#include "GxLib.h"
#include "ImageStream.h"

///< related units
#include "ImageUnit_VdoIn.h"
#include "ImageUnit_ImagePipe.h"
#include "ImageUnit_ImgTrans.h"
#include "ImageUnit_UserProc.h"
#include "ImageUnit_VdoEnc.h"
#include "ImageUnit_VdoOut.h"
#include "ImageUnit_AudIn.h"
#include "ImageUnit_AudEnc.h"
#include "ImageUnit_AudDec.h"
#include "ImageUnit_AudOut.h"
#include "ImageUnit_Mux.h"
#include "ImageUnit_Demux.h"
#include "ImageUnit_Dummy.h"
#include "ImageUnit_StreamReceiver.h"
#include "ImageUnit_StreamSender.h"
#include "ImageUnit_NetRTSP.h"
#include "ImageUnit_UsbUVAC.h"


/**

Connection: default

    VdoIn -> ImagePipe(N) -> ImgTrans -> UserProc -> VdoEnc ->\                  /-> StreamSender
                                                               -> Mux -> Demux ->
    AudIn -----------------------------------------> AudEnc ->/                  \-> NetRTSP


Connection: DISP PATH

    VdoIn -> ImagePipe(N) -> ImgTrans -> UserProc -> VdoOut


Connection: ALG PATH

    VdoIn -> ImagePipe(N) -> ImgTrans -> UserProc -> Dummy


Connection: AUD PATH

    StreamReceiver -> AudOut

*/



#define IPCAM_IPL_PATH_MAX 5

typedef  enum {
	IPCAM_CFG_START = 0x00006000,
	//IPCAM_CFG_IPL_MAXBUF ,
	IPCAM_CFG_PLUG_RTSP= IPCAM_CFG_START,
	//IPCAM_CFG_CROP,
	//IPCAM_CFG_HDMI_OUT,
	IPCAM_CFG_INIT_FLAG,
	IPCAM_CFG_DEWARP_FLAG,
	IPCAM_DISP_PATH,
	IPCAM_ALG_PATH,
	IPCAM_YUV_PATH,
	IPCAM_UVAC_PATH,
	IPCAM_RAW_PATH,
	IPCAM_SENSOR_NUMBER,
	IPCAM_CFG_DIS,
	IPCAM_AUDIO_IN,
	IPCAM_AUDIO_OUT,
	IPCAM_CFG_MAX_CMD,
	ENUM_DUMMY4WORD(IPCAM_CFG_ID)
} IPCAM_CFG_ID;

typedef enum {
	IPCAM_PATH_ID_0 = 0,
	IPCAM_PATH_ID_1 = 1,
	IPCAM_PATH_ID_2 = 2,
	IPCAM_PATH_ID_3 = 3,
	IPCAM_PATH_ID_4 = 4,
	IPCAM_PATH_ID_5 = 5,
	IPCAM_PATH_ID_6 = 6,
	IPCAM_PATH_ID_7 = 7,
#if defined(_BSP_NA51000_)
	IPCAM_PATH_ID_8 = 8,
	IPCAM_PATH_ID_9 = 9,
	IPCAM_PATH_ID_10 = 10,
	IPCAM_PATH_ID_11 = 11,
	IPCAM_PATH_ID_12 = 12,
	IPCAM_PATH_ID_13 = 13,
	IPCAM_PATH_ID_14 = 14,
	IPCAM_PATH_ID_15 = 15,
#endif
	IPCAM_PATH_MAX_ID,
	ENUM_DUMMY4WORD(IPCAM_PATH_ID)
} IPCAM_PATH_ID;


typedef enum {

	IPCAM_STATE_DISABLE = 0,
	IPCAM_STATE_ENABLE = 1,
	IPCAM_STATE_CHGPARM = 2,
	IPCAM_STATE_MAX_ID = 3,
	ENUM_DUMMY4WORD(IPCAM_STATE)
} IPCAM_STATE;


typedef enum {
	IPCAM_SENSOR1 = 0,
	IPCAM_SENSOR2,
#if defined(_BSP_NA51000_)
	IPCAM_SENSOR3,
	IPCAM_SENSOR4,
	IPCAM_SENSOR5,
	IPCAM_SENSOR6,
	IPCAM_SENSOR7,
	IPCAM_SENSOR8,
#endif
	IPCAM_SENSOR_MAX,
	ENUM_DUMMY4WORD(IPCAM_SENSOR)

} IPCAM_SENSOR;

typedef enum {

	IPCAM_IME1,
	IPCAM_IME2,
	IPCAM_IME3,
	IPCAM_IME4,
	IPCAM_IME5,
	IPCAM_IME_MAX,
	ENUM_DUMMY4WORD(IPCAM_IME)

} IPCAM_IME;

typedef enum {

	IPCAM_STREAM_MAIN,
	IPCAM_STREAM_EXT,
	IPCAM_STREAM_MAX,
	ENUM_DUMMY4WORD(IPCAM_STREAM_TYPE)

} IPCAM_STREAM_TYPE;

typedef enum {

	IPCAM_TRIGGER_SRC,
	IPCAM_TRIGGER_DEST,
	IPCAM_TRIGGER_MAX,
	ENUM_DUMMY4WORD(IPCAM_TRIGGER_TYPE)

} IPCAM_TRIGGER_TYPE;


typedef enum {

	IPCAM_AV_VIDEO,
	IPCAM_AV_AUDIO,
	IPCAM_AV_MAX,
	ENUM_DUMMY4WORD(IPCAM_AV_TYPE)
} IPCAM_AV_TYPE;


typedef struct {

	IPCAM_PATH_ID       path;
	IPCAM_STATE         enable;
	IPCAM_SENSOR        sensor;
	IPCAM_IME           ime;
	IPCAM_STREAM_TYPE   type;
	//BOOL				bUserPullData;
	IPCAM_TRIGGER_TYPE  trig;
	UINT32              maxbuf;
	IPCAM_AV_TYPE		avtype;

} IPCAM_STREAM_INFO;


#if 0
typedef struct {

	IPCAM_PATH_ID       path;
	UINT32              fps;
	ISIZE               resolution;
	UINT32              bitrate;
	UINT32              codec; //value: MEDIAVIDENC_MJPG, MEDIAVIDENC_H264, MEDIAVIDENC_H265
	INT32               i3DNRLevel; //encoder 3DNR

} IPCAM_STREAM_CONFIG;
#endif

typedef struct {

	UINT32 codec;
	UINT32 channel;
	UINT32 sample_rate;

} IPCAM_AUDIO_INFO;


//@}


/**
    Config settings for movie recording.

    Config settings for movie recording, such as IPCAM_CFG_POOL or others.

    @param[in] config_id IPCAM_CFG_POOL or others.
    @param[in] value configuration value.

    @return void
*/
extern void ImageApp_IPCam_Config(UINT32 config_id, UINT32 value);

/**
    Get config settings for movie recording.

    Get config settings for movie recording, such as IPCAM_CFG_IMGPATH or others.

    @param[in] config_id IPCAM_CFG_IMGPATH or others.

    @return configuration value
*/

extern INT32 ImageApp_IPCam_GetConfig(UINT32 config_id);

/**
    Open movie recorder.

    Open movie recorder.

    @param[in] void

    @return void
*/
extern void ImageApp_IPCam_Open(void);

/**
    Close movie recorder.

    Close movie recorder.

    @return void
*/
extern void ImageApp_IPCam_all_path_Close(void);

extern void ImageApp_IPCam_CfgImgSize(IPCAM_PATH_ID Path, UINT32 img_w, UINT32 img_h);

extern void ImageApp_IPCam_CfgImgRatio(IPCAM_PATH_ID Path, UINT32 ratio_w, UINT32 ratio_h);

extern void ImageApp_IPCam_reset_path(IPCAM_PATH_ID Path);


extern void ImageApp_IPCam_Get_Sensor_Mapping(IPCAM_SENSOR *Sensorlist);

extern void ImageApp_IPCam_Set_Sensor_Mapping(IPCAM_SENSOR *Sensorlist);

extern IPCAM_STREAM_INFO *ImageApp_IPCam_get_streamInfo(void);

extern void ImageApp_IPCam_set_StreamInfo(IPCAM_STREAM_INFO *streamInfo);

extern void ImageApp_IPCam_update_all_path(void);

extern INT32 ImageApp_IPCam_Snapshot(UINT32 PathID ,UINT32 *SnapshotAddr,ISIZE ScaleInfo, BOOL bCompress, UINT32 Sensor);

extern void ImageApp_IPCam_audio_set_status(ISF_PORT_STATE status);

extern void ImageApp_IPCam_VideoOut_set_status(UINT32 path, ISF_PORT_STATE status);

extern void ImageApp_IPCam_ImagePipe_set_status(UINT32 path, ISF_PORT_STATE status);

extern void ImageApp_IPCam_BeginStream(UINT32 path);

extern void ImageApp_IPCam_EndStream(void);
//@}
#endif//IMAGEAPP_IPCAM_H
