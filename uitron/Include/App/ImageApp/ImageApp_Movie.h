/*
    Copyright   Novatek Microelectronics Corp. 2017.  All rights reserved.

    @file       ImageApp_Movie.h
    @ingroup    mIImageApp

    @note       This header file is updated by ImageApp_Movie 1.00.025

    @date       2017/03/15
*/

#ifndef IMAGEAPP_MOVIE_H
#define IMAGEAPP_MOVIE_H


#include "ImageStream.h"
#include "ImageApp_MovieCommon.h"

///< related units
#include "ImageUnit_VdoIn.h"
#include "ImageUnit_ImagePipe.h"
#include "ImageUnit_VdoEnc.h"
#include "ImageUnit_VdoOut.h"
#include "ImageUnit_AudIn.h"
#include "ImageUnit_AudEnc.h"
#include "ImageUnit_FileOut.h"
#include "ImageUnit_BsMux.h"
#include "ImageUnit_Mux.h"
#include "ImageUnit_Demux.h"
#include "ImageUnit_StreamSender.h"
#include "ImageUnit_NetRTSP.h"
#include "ImageUnit_UserProc.h"
#include "ImageUnit_ImgTrans.h"
#include "ImageUnit_Dummy.h"
#include "ImageUnit_UsbUVAC.h"


/**
    @addtogroup mIImageApp
*/
//@{

typedef UINT32       MOVIE_CFG_AUD_RATE;
typedef UINT32       MOVIE_CFG_AUD_CH;

typedef UINT32       MOVIE_CFG_GOP_NUM;
typedef BOOL         MOVIE_CFG_RTSP_STRM_OUT;

typedef CHAR         *MOVIE_CFG_ROOT_PATH;
typedef MEM_RANGE    MOVIE_CFG_MEM_RANGE;
typedef CHAR         *MOVIE_CFG_FOLDER_NAME;
typedef BOOL         MOVIE_CFG_FILE_NAMING;

typedef IPL_SEL_ITEM MOVIE_CFG_IPL_SEL_ITEM;
typedef UINT32       MOVIE_CFG_IPL_SEL_VALUE;

typedef UINT32       MOVIE_CFG_DISP_ROTATE;

typedef UINT32       MOVIE_CFG_DISP_PIP_VIEW;

// The following part is moved to ImageApp_MovieCommon.h
#if 0
typedef enum {
	_CFG_CODEC_MJPG = 1,
	_CFG_CODEC_H264,
	_CFG_CODEC_H265,
	ENUM_DUMMY4WORD(MOVIE_CFG_CODEC)
} MOVIE_CFG_CODEC;

typedef enum {
	_CFG_UVC_CODEC_MJPG = _CFG_CODEC_MJPG,
	_CFG_UVC_CODEC_H264 = _CFG_CODEC_H264,
	ENUM_DUMMY4WORD(MOVIE_CFG_UVC_CODEC)
} MOVIE_CFG_UVC_CODEC;

typedef enum {
	_CFG_AUD_CODEC_NONE = 0,
	_CFG_AUD_CODEC_PCM,
	_CFG_AUD_CODEC_AAC,
	ENUM_DUMMY4WORD(MOVIE_CFG_AUD_CODEC)
} MOVIE_CFG_AUD_CODEC;

typedef enum {
	_CFG_REC_TYPE_AV = 0,
	_CFG_REC_TYPE_VID,
	_CFG_REC_TYPE_TIMELAPSE,
	_CFG_REC_TYPE_GOLFSHOT,
	_CFG_REC_TYPE_PRE_RECORD,
	_CFG_REC_TYPE_PRE_RECORD_VID,
	ENUM_DUMMY4WORD(MOVIE_CFG_REC_MODE)
} MOVIE_CFG_REC_MODE;

typedef enum {
	_CFG_FILE_FORMAT_MP4 = 0,
	_CFG_FILE_FORMAT_TS,
	_CFG_FILE_FORMAT_MOV,
	ENUM_DUMMY4WORD(MOVIE_CFG_FILE_FORMAT)
} MOVIE_CFG_FILE_FORMAT;

typedef enum {
	_CFG_DAR_DEFAULT,                      // default video display aspect ratio (the same as encoded image)
	_CFG_DAR_16_9,                         // 16:9 video display aspect ratio
	ENUM_DUMMY4WORD(MOVIE_CFG_DAR)
} MOVIE_CFG_DAR;

typedef enum {
	_CFG_PROFILE_BASELINE,
	_CFG_PROFILE_MAIN,
	_CFG_PROFILE_HIGH,
	ENUM_DUMMY4WORD(MOVIE_CFG_PROFILE)
} MOVIE_CFG_PROFILE;
#endif

typedef enum {
	MOVIE_ALG_IPL1_3 = 0,
	MOVIE_ALG_IPL1_4,
	MOVIE_ALG_IPL2_3,
	MOVIE_ALG_IPL2_4,
	MOVIE_ALG_IPL_MAX,
	ENUM_DUMMY4WORD(MOVIE_ALG_IPL_PORT)
} MOVIE_ALG_IPL_PORT;

typedef struct {
	MOVIE_CFG_DISP_ROTATE    rotate_dir;
	MOVIE_CFG_DISP_PIP_VIEW  pip_view;
} MOVIE_CFG_DISP_INFO;

typedef struct {
	MOVIE_CFG_REC_ID        uvac_id;
	MOVIE_CFG_VID_IN        vid_in;
	MOVIE_CFG_IMG_SIZE      size;
	MOVIE_CFG_FRAME_RATE    frame_rate;
	MOVIE_CFG_TARGET_RATE   max_bit_rate;
	MOVIE_CFG_UVC_CODEC     codec;
	MOVIE_CFG_AUD_CODEC     aud_codec;
	MOVIE_CFG_AQ_INFO       aq_info;
	MOVIE_CFG_CBR_INFO      cbr_info;
	BOOL                    ipl_set_enable;
	MOVIE_CFG_IMG_RATIO      ratio;
} MOVIE_UVAC_INFO;

typedef struct {
	MOVIE_CFG_AUD_RATE      aud_rate;
	MOVIE_CFG_AUD_CH        aud_ch;
} MOVIE_AUDIO_INFO;

typedef struct {
	MOVIE_CFG_REC_ID        rec_id;
	MOVIE_CFG_FOLDER_NAME   movie;
	MOVIE_CFG_FOLDER_NAME   emr;
	MOVIE_CFG_FOLDER_NAME   snapshot;
	MOVIE_CFG_FILE_NAMING   file_naming;
} MOVIE_RECODE_FOLDER_NAMING;

typedef struct {
	MOVIE_CFG_IPL_SEL_ITEM   item;
	MOVIE_CFG_IPL_SEL_VALUE  value;
} MOVIE_RECODE_IPL_INFO;

#define MOVIE_ALG_META_PASS  0x00000010

enum {
	MOVIE_PARAM_START = 0x0000EC000,
	MOVIE_PARAM_IMG_SIZE = MOVIE_PARAM_START,
	MOVIE_PARAM_IMG_RATIO,
	MOVIE_PARAM_FRAMERATE,
	MOVIE_PARAM_CODEC,
	MOVIE_PARAM_TARGETRATE,
	MOVIE_PARAM_INITQP,
	MOVIE_PARAM_MINQP,
	MOVIE_PARAM_MAXQP,
	MOVIE_PARAM_DAR,
	MOVIE_PARAM_AUD_SAMPLERATE,
	MOVIE_PARAM_AUD_CHS,
	MOVIE_PARAM_AUD_CODEC,
	MOVIE_PARAM_AUD_ENCODER_OBJ,
	MOVIE_PARAM_AUD_OUTPUT_FMT,
	MOVIE_PARAM_FILE_FORMAT,
	MOVIE_PARAM_FILE_SEAMLESSSEC,
	MOVIE_PARAM_FILE_ROLLBACKSEC,
	MOVIE_PARAM_FILE_EMRON,
	MOVIE_PARAM_FILE_STRGID,
	MOVIE_PARAM_FILE_KEEPSEC,
	MOVIE_PARAM_FILE_ENDTYPE,
	MOVIE_PARAM_MAX_FILE,
	MOVIE_PARAM_IMGCAP_MAXSIZE,
	MOVIE_PARAM_IMGCAP_SIZE,
	MOVIE_PARAM_USER_CB,
	MOVIE_PARAM_IPL_INFO,
	MOVIE_PARAM_AQ_INFO,
	MOVIE_PARAM_CBR_INFO,
	MOVIE_PARAM_YUV_COMPRESS,
	MOVIE_PARAM_3DNR_CB,
	MOVIE_PARAM_IMM_CB,
	MOVIE_PARAM_REC_FORMAT,
	MOVIE_PARAM_FILE_PLAYFRAMERATE,
	MOVIE_PARAM_TIMELAPSE_TIME,
	MOVIE_PARAM_GPS_DATA,
	MOVIE_PARAM_SKIP_FRAME,
	MOVIE_PARAM_PIPVIEW_PUSH_CB,
	MOVIE_PARAM_PIPVIEW_PROCESS_CB,
	MOVIE_PARAM_FILE_BUFRESSEC,
	MOVIE_PARAM_FILE_EMRSEC,
	MOVIE_PARAM_FILE_TRIGEMR,
	MOVIE_PARAM_FILE_OVERLAP,
	MOVIE_PARAM_PRECALC_BUFFER,
	MOVIE_PARAM_PROFILE,
	MOVIE_PARAM_DUAL_DISP_EN,
	MOVIE_PARAM_DUAL_DISP_IPL_SIZE,
	MOVIE_PARAM_DUAL_DISP_AR,
	MOVIE_PARAM_VDO_ENCODER_OBJ,
	MOVIE_PARAM_MULTI_TEMPORARY_LAYER,
	MOVIE_PARAM_TV_RANGE,
	MOVIE_PARAM_IMGCAP_JPG_BUFNUM,
	MOVIE_PARAM_D2D_MODE,
};

/**
    Config settings for DV Cam.

    Config settings for DV Cam.

    @param[in] config_id.
    @param[in] value configuration value.

    @return ISF_RV
*/
extern ISF_RV ImageApp_Movie_Config(UINT32 config_id, UINT32 value);

/**
    Get config settings DV Cam.

    Get config settings for movie recording, such as CAM_CFG_IMGPATH or others.

    @param[in] config_id or others.

    @return configuration value
*/

extern UINT32 ImageApp_Movie_GetConfig(UINT32 config_id);

/**
    Set the file option

    Set the file option.

    @param[in] config_id.
    @param[in] value option value.

    @return ISF_RV
*/
extern ISF_RV ImageApp_Movie_File_Option(MOVIE_RECODE_FILE_OPTION *prec_option);

/**
    Set parameter for DV Cam.

    Set parameter for DV Cam.

    @param[in] id    MOVIE_CFG_REC_ID
    @param[in] param MOVIE_PARAM_ID.
    @param[in] value.

    @return ISF_RV
*/
extern ISF_RV ImageApp_Movie_SetParam(MOVIE_CFG_REC_ID cfg_id, UINT32 param, UINT32 value);

/**
    Open DV Cam.

    Open DV Cam.

    @param[in] void

    @return ISF_RV
*/
extern ISF_RV ImageApp_Movie_Open(void);

/**
    Close DV Cam.

    Close DV Cam.

    @return ISF_RV
*/
extern ISF_RV ImageApp_Movie_Close(void);

/**
    start DV Cam recording.

    start DV Cam recording.

    @param[in] MOVIE_CFG_REC_ID id

    @return ISF_RV
*/
extern ISF_RV ImageApp_Movie_Record_Start(MOVIE_CFG_REC_ID id);

/**
    stop DV Cam recording.

    stop DV Cam recording.

    @param[in] MOVIE_CFG_REC_ID id

    @return ISF_RV
*/
extern ISF_RV ImageApp_Movie_Record_Stop(MOVIE_CFG_REC_ID id);

/**
    start DV Cam streaming.

    start DV Cam streaming.

    @param[in] MOVIE_CFG_REC_ID id

    @return ISF_RV
*/
extern ISF_RV ImageApp_Movie_Streaming_Start(MOVIE_CFG_REC_ID id);

/**
    stop DV Cam streaming.

    stop DV Cam streaming.

    @param[in] MOVIE_CFG_REC_ID id

    @return ISF_RV
*/
extern ISF_RV ImageApp_Movie_Streaming_Stop(MOVIE_CFG_REC_ID id);

/**
    trigger a snapshot

    trigger a snapshot.

    @param[in] MOVIE_CFG_REC_ID id

    @return ISF_RV
*/
extern ISF_RV ImageApp_Movie_Trigger_Snapshot(MOVIE_CFG_REC_ID id);

/**
    set crash the assigned rec id to backup files

    set crash the assigned rec id to backup files

    @param[in] MOVIE_CFG_REC_ID id

    @return ISF_RV
*/
extern ISF_RV ImageApp_Movie_SetCrash(MOVIE_CFG_REC_ID rec_id, UINT32 is_set);

/**
    Set the record root path

    Set the folder name.

    @param[in] root path

    @return ISF_RV
*/
void ImageApp_Movie_Root_Path(MOVIE_CFG_ROOT_PATH prootpath, UINT32 pathid);


/**
    Set buffer for FileDB

    Set buffer for FileDB

    @param[in] MOVIE_CFG_MEM_RANGE

    @return void
*/
void ImageApp_Movie_FDB_Pool(MOVIE_CFG_MEM_RANGE *pPool, UINT32 pathID);


/**
    Set root path for FileDB

    Set root path for FileDB

    @return void
*/

void ImageApp_Movie_FDB_Path(MOVIE_CFG_ROOT_PATH prootpath, UINT32 pathID);


/**
    Set the folder name

    Set the folder name.

    @param[in] config_id.
    @param[in] value .

    @return ISF_RV
*/
ISF_RV ImageApp_Movie_Folder_Naming(MOVIE_RECODE_FOLDER_NAMING *pfolder_naming);

/*
    Register User Callback function.

    Register User Callback function.

    @param[in] fncb MovieUserEventCb* fuction pointer.

    @return configuration value
*/

ISF_RV ImageApp_Movie_RegUserCB(MovieUserEventCb *fncb);

/**
    start DV Cam UVAC.

    start DV Cam UVAC.

    @param[in] MOVIE_CFG_REC_ID id

    @return ISF_RV
*/

extern ISF_RV ImageApp_Movie_Uvac_Start(MOVIE_CFG_REC_ID id);

/**
    start DV Cam UVAC.

    start DV Cam UVAC.

    @param[in] MOVIE_CFG_REC_ID id

    @return ISF_RV
*/

extern ISF_RV ImageApp_Movie_Uvac_Stop(MOVIE_CFG_REC_ID id);

/*
    Set FileDB related options

    Set FileDB related options

    @param[in] fdb structure.

    @return ISF_RV
*/
extern ISF_RV ImageApp_Movie_FileDB_Config(MOVIE_CFG_FDB_INFO *p_cfg, UINT32 pathID);

/*
    Set FileDB create and initialize

    Set FileDB create and initialize

    @param[in] drive name ('A'~'Z')

    @return ISF_RV
*/
extern ISF_RV ImageApp_Movie_FileDB_Create(CHAR drive);

/*
    Release FileDB resource

    Release FileDB resource

    @param[in] drive name ('A'~'Z')

    @return void
*/
extern void ImageApp_Movie_FileDB_Release(CHAR drive);

/**
    return sensor ID by video path.

    return sensor ID by video path.

    @param[in] UINT32 path ID

    @return MOVIE_CFG_VID_IN
*/
extern MOVIE_CFG_VID_IN ImageApp_Movie_GetVidIn_ByPath(UINT32 pathID);

/**
    return sensor ID by video path.

    @param[in] UINT32 path ID

    @return MOVIE_CFG_VID_IN
*/
extern MOVIE_CFG_VID_IN ImageApp_Movie_GetVidIn_ByPath_Strm(UINT32 pathID);

/**
    return Image Unit port.

    return Image Unit port.

    @param[in] MOVIE_CFG_REC_ID id

    @return ISF_PORT
*/
extern ISF_PORT *ImageApp_Movie_Get_Pip_ImgSrc(MOVIE_CFG_REC_ID id);

/**
    return stream status

    return stream status

    @param[in] UINT32 MOVIE_CFG_REC_ID id

    @return status of stream (TRUE for running)
*/
extern BOOL ImageApp_Movie_Is_Stream_Running(MOVIE_CFG_REC_ID id);

/**
    return rest free seconds

    return rest free seconds

    @param[in] none

    @return rest free seconds
*/
extern UINT32 ImageApp_Movie_GetFreeRec(void);

/**
    return alg data port of ImageUnit_UserProc

    return alg data port of ImageUnit_UserProc

    @param[in] ipl port defined by MOVIE_ALG_IPL_PORT

    @return alg data port of ImageUnit_UserProc
*/
extern ISF_OUT ImageApp_Movie_GetAlgOutPort(MOVIE_ALG_IPL_PORT Port);

/**
    return video encoder data port

    return video encoder data port

    @param[in] UINT32 MOVIE_CFG_REC_ID id

    @return video encoder data port of ImageUnit_VdoEnc
*/
extern ISF_OUT ImageApp_Movie_GetVdoEncPort(MOVIE_CFG_REC_ID id);

//@}
#endif//IMAGEAPP_MOVIE_H
