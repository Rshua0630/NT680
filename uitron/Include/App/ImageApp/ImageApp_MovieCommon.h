/*
    Copyright   Novatek Microelectronics Corp. 2018.  All rights reserved.

    @file       ImageApp_MovieCommon.h
    @ingroup    mIImageApp

    @note       This header file contains common parts of ImageApp_Movie_xxx.h

    @date       2018/05/28
*/

#ifndef IMAGEAPP_MOVIECOMMON_H
#define IMAGEAPP_MOVIECOMMON_H

#include "media_def.h"
#include "VideoEncode.h"

/**
    @addtogroup mIImageApp
*/
//@{

typedef enum {
	_CFG_REC_ID_1 = 0,
	_CFG_REC_ID_2,
	_CFG_REC_ID_3,
	_CFG_REC_ID_4,
	_CFG_REC_ID_MAX,
	_CFG_STRM_ID_1,
	_CFG_STRM_ID_2,
	_CFG_STRM_ID_3,
	_CFG_STRM_ID_4,
	_CFG_STRM_ID_MAX,
	_CFG_UVAC_ID_1,
	_CFG_UVAC_ID_2,
	_CFG_UVAC_ID_MAX,
	_CFG_CLONE_ID_1,
	_CFG_CLONE_ID_2,
	_CFG_CLONE_ID_3,
	_CFG_CLONE_ID_4,
	_CFG_CLONE_ID_MAX,
	_CFG_DISP_ID_1,
	_CFG_DISP_ID_2,
	_CFG_DISP_ID_MAX,
	ENUM_DUMMY4WORD(MOVIE_CFG_REC_ID)
} MOVIE_CFG_REC_ID;

typedef enum {
	_CFG_VID_IN_1 = 0,
	_CFG_VID_IN_2,
	_CFG_VID_IN_3,
	_CFG_VID_IN_4,
	ENUM_DUMMY4WORD(MOVIE_CFG_VID_IN)
} MOVIE_CFG_VID_IN;

typedef ISIZE        MOVIE_CFG_IMG_SIZE;

typedef UINT32       MOVIE_CFG_FRAME_RATE;

typedef UINT32       MOVIE_CFG_TARGET_RATE;

typedef ISIZE        MOVIE_CFG_IMG_RATIO;

typedef ISIZE        MOVIE_CFG_RAWENC_SIZE;

typedef UINT32       MOVIE_CFG_IMAGEPIPE_FUNC;

typedef UINT32       MOVIE_CFG_IMAGEPIPE_FUNC2;

typedef BOOL         MOVIE_CFG_DISP_ENABLE;

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
	_CFG_REC_TYPE_AV        = MEDIAREC_AUD_VID_BOTH,
	_CFG_REC_TYPE_VID       = MEDIAREC_VID_ONLY,
	_CFG_REC_TYPE_TIMELAPSE = MEDIAREC_TIMELAPSE,
	_CFG_REC_TYPE_GOLFSHOT  = MEDIAREC_GOLFSHOT,
	ENUM_DUMMY4WORD(MOVIE_CFG_REC_MODE)
} MOVIE_CFG_REC_MODE;

typedef enum {
	_CFG_FILE_FORMAT_MP4 = MEDIA_FILEFORMAT_MP4,
	_CFG_FILE_FORMAT_TS  = MEDIA_FILEFORMAT_TS,
	_CFG_FILE_FORMAT_MOV = MEDIA_FILEFORMAT_MOV,
	ENUM_DUMMY4WORD(MOVIE_CFG_FILE_FORMAT)
} MOVIE_CFG_FILE_FORMAT;

typedef enum {
	_CFG_DAR_DEFAULT,                      // default video display aspect ratio (the same as encoded image)
	_CFG_DAR_16_9,                         // 16:9 video display aspect ratio
	ENUM_DUMMY4WORD(MOVIE_CFG_DAR)
} MOVIE_CFG_DAR;

typedef enum {
	_CFG_PROFILE_BASELINE = MP_VDOENC_PROFILE_BASELINE,
	_CFG_PROFILE_MAIN     = MP_VDOENC_PROFILE_MAIN,
	_CFG_PROFILE_HIGH     = MP_VDOENC_PROFILE_HIGH,
	ENUM_DUMMY4WORD(MOVIE_CFG_PROFILE)
} MOVIE_CFG_PROFILE;

enum {
	MOVIE_CONFIG_RECORD_INFO  = 0xF000F000,
	MOVIE_CONFIG_STREAM_INFO,
	MOVIE_CONFIG_AUDIO_INFO,
	MOVIE_CONFIG_DISP_INFO,
	MOVIE_CONFIG_UVAC_INFO,
	MOVIE_CONFIG_ALG_INFO,
};

typedef enum {
	MOVIE_USER_CB_EVENT_START                 = 0x0000E300,
	// EVENT
	MOVIE_USER_CB_EVENT_VIDEO_READY           = MOVIE_USER_CB_EVENT_START,
	MOVIE_USER_CB_EVENT_STAMP_CB,                                            // value: (MP_VDOENC_YUV_SRC*)
	MOVIE_USER_CB_EVENT_REC_ONE_SECOND,
	MOVIE_USER_CB_EVENT_REC_STOP,
	MOVIE_USER_CB_EVENT_CARD_FULL,
	MOVIE_USER_CB_EVENT_REC_FILE_COMPLETED,
	MOVIE_USER_CB_EVENT_EMR_FILE_COMPLETED,
	MOVIE_USER_CB_EVENT_CARSH_FILE_COMPLETED,
	MOVIE_USER_CB_EVENT_SNAPSHOT_OK,
	MOVIE_USER_CB_EVENT_TIMELAPSE_ONESHOT,
	MOVIE_USER_CB_EVENT_FILENAMING_MOV_CB,
	MOVIE_USER_CB_EVENT_FILENAMING_EMR_CB,
	MOVIE_USER_CB_EVENT_FILENAMING_SNAPSHOT_CB,
	MOVIE_USER_CB_EVENT_KICKTHUMB,
	MOVIE_USER_CB_EVENT_CUTFILE,
	MOVIE_USER_CB_EVENT_CLOSE_RESULT,
	MOVIE_USER_CB_EVENT_OVERTIME,
	// ERROR
	MOVIE_USER_CB_ERROR_CARD_SLOW             = 0x0000E400,
	MOVIE_USER_CB_ERROR_CARD_WR_ERR,
	MOVIE_USER_CB_ERROR_SEAMLESS_REC_FULL,
	MOVIE_USER_CB_ERROR_SNAPSHOT_ERR,

	ENUM_DUMMY4WORD(MOVIE_CB_EVENT)
} MOVIE_USER_CB_EVENT;

typedef void (MovieUserEventCb)(UINT32 id, MOVIE_USER_CB_EVENT event_id, UINT32 value);

typedef struct {
	INT32  aq_enable;
	UINT32 aq_str;
	INT32  sraq_init_aslog2;
	INT32  max_sraq;
	INT32  min_sraq;
	INT32  sraq_const_aslog2;
} MOVIE_CFG_AQ_INFO;

typedef struct {
	UINT32 uiEnable;
	UINT32 uiStaticTime;
	UINT32 uiFrameRate;
	UINT32 uiByteRate;
	UINT32 uiGOP;
	UINT32 uiInitIQp;
	UINT32 uiMinIQp;
	UINT32 uiMaxIQp;
	UINT32 uiInitPQp;
	UINT32 uiMinPQp;
	UINT32 uiMaxPQp;
	INT32  iIPWeight;
	UINT32 uiRowRcEnalbe;
	UINT32 uiRowRcQpRange;
	UINT32 uiRowRcQpStep;
} MOVIE_CFG_CBR_INFO;

typedef struct {
	MEM_RANGE mem_range;
	CHAR *p_root_path;
	CHAR drive; // 'A' ~ 'Z'
} MOVIE_CFG_FDB_INFO;

typedef struct {
	MOVIE_CFG_REC_ID             rec_id;
	MOVIE_CFG_VID_IN             vid_in;
	MOVIE_CFG_IMG_SIZE           size;
	MOVIE_CFG_FRAME_RATE         frame_rate;
	MOVIE_CFG_TARGET_RATE        target_bitrate;
	MOVIE_CFG_CODEC              codec;
	MOVIE_CFG_AUD_CODEC          aud_codec;
	MOVIE_CFG_REC_MODE           rec_mode;
	MOVIE_CFG_FILE_FORMAT        file_format;
	MOVIE_CFG_IMG_RATIO          ratio;
	MOVIE_CFG_RAWENC_SIZE        raw_enc_size;
	MOVIE_CFG_IMAGEPIPE_FUNC     imagepipe_func;
	MOVIE_CFG_DISP_ENABLE        disp_enable;
	BOOL                         ipl_set_enable;
	MOVIE_CFG_DAR                dar;
	MOVIE_CFG_AQ_INFO            aq_info;
	MOVIE_CFG_CBR_INFO           cbr_info;
	BOOL                         sensor_rotate;
	MOVIE_CFG_IMAGEPIPE_FUNC2    imagepipe_func2;
} MOVIE_RECODE_INFO;

typedef UINT32       MOVIE_CFG_FILE_SECOND;

typedef enum {
	_CFG_EMR_OFF = 0,               /// EMR off
	_CFG_EMR_MODE1,                 /// Main remains + EMR (N/A)
	_CFG_EMR_MODE2,					/// Main paused + EMR (N/A)
	_CFG_EMR_MODE3,                 /// EMR only (N/A)
	_CFG_EMR_MODE4,                 /// set crash
	ENUM_DUMMY4WORD(MOVIE_CFG_FILE_OPTION)
} MOVIE_CFG_FILE_OPTION;

typedef UINT32       MOVIE_CFG_FILE_ENDTYPE;

typedef struct {
	MOVIE_CFG_REC_ID        rec_id;
	MOVIE_CFG_FILE_SECOND   seamless_sec;  // Seamless second
	MOVIE_CFG_FILE_OPTION   emr_on;        // pre-record emergency on
	MOVIE_CFG_FILE_SECOND   emr_sec;       // pre-record second for emergency
	MOVIE_CFG_FILE_SECOND   keep_sec;      // post-record second for emergency
	MOVIE_CFG_FILE_SECOND   overlap_sec;   // overlap second
	MOVIE_CFG_FILE_ENDTYPE  end_type;      // such as MOVREC_ENDTYPE_CUTOVERLAP
} MOVIE_RECODE_FILE_OPTION;

typedef UINT32    MOVIE_CFG_GOP_NUM;

typedef BOOL      MOVIE_CFG_RTSP_STRM_OUT;

typedef struct {
	MOVIE_CFG_REC_ID        strm_id;
	MOVIE_CFG_VID_IN        vid_in;
	MOVIE_CFG_IMG_SIZE      size;
	MOVIE_CFG_FRAME_RATE    frame_rate;
	MOVIE_CFG_TARGET_RATE   max_bit_rate;
	MOVIE_CFG_CODEC         codec;
	MOVIE_CFG_GOP_NUM       gop_num;
	MOVIE_CFG_AUD_CODEC     aud_codec;
	MOVIE_CFG_RTSP_STRM_OUT rstp_strm_out;
	MOVIE_CFG_AQ_INFO       aq_info;
	MOVIE_CFG_CBR_INFO      cbr_info;
	BOOL                    ipl_set_enable;
	MOVIE_CFG_IMG_RATIO     ratio;
	BOOL                    userproc_pull;
} MOVIE_STRM_INFO;

typedef struct {
	UINT32                   ImgFmt;
	USIZE                    ImgSize;
	URECT                    Window;
	UINT32                   PreviewEn;
} MOVIE_ALG_IPL_INFO;

typedef struct {
	MOVIE_ALG_IPL_INFO      path13;
	MOVIE_ALG_IPL_INFO      path14;
	MOVIE_CFG_REC_ID        rec_id;
} MOVIE_ALG_INFO;

typedef enum {
	_CFG_ALG_PATH3 = 0,
	_CFG_ALG_PATH4,
	_CFG_ALG_PATH_MAX,
	ENUM_DUMMY4WORD(MOVIE_CFG_ALG_PATH)
} MOVIE_CFG_ALG_PATH;

//@}
#endif//IMAGEAPP_MOVIECOMMON_H
