/*
    Copyright   Novatek Microelectronics Corp. 2017.  All rights reserved.

    @file       ImageApp_MoviePlay.h
    @ingroup    mIImageApp

    @note       Nothing.

    @date       2017/05/08
*/

#ifndef IMAGEAPP_MOVIEPLAY_H
#define IMAGEAPP_MOVIEPLAY_H


#include "SxCmd.h"
#include "SMediaPlayAPI.h"
#include "ImageStream.h"
#include "nvtmpp.h"
#include "ImageApp_MovieCommon.h"
#include "ImageUnit_MediaPlay.h"
#include "ImageUnit_VdoOut.h"
#include "ImageUnit_AudOut.h"
#include "ImageUnit_UserProc.h"
#include "ImageUnit_ImgTrans.h"
#include "ImageUnit_Dummy.h" // for test
#include "ImageUnit_FileIn.h"
#include "ImageUnit_BsDemux.h"
#include "ImageUnit_VdoDec.h"
#include "ImageUnit_AudDec.h"

/**
    @addtogroup mIImageApp
*/
//@{

typedef enum {
	_CFG_FILEPLAY_ID_1 = 0,
	_CFG_FILEPLAY_ID_MAX,
	_CFG_STRMPLAY_ID_1,
	_CFG_STRMPLAY_ID_MAX,
	ENUM_DUMMY4WORD(MOVIEPLAY_CFG_PLAY_ID)
} MOVIEPLAY_CFG_PLAY_ID;

typedef enum {
	MOVIEPLAY_DISP_ID_MIN = 0,
	MOVIEPLAY_DISP_ID_1 = MOVIEPLAY_DISP_ID_MIN,
	MOVIEPLAY_DISP_ID_2,
	MOVIEPLAY_DISP_ID_3,
	MOVIEPLAY_DISP_ID_4,
	MOVIEPLAY_DISP_ID_MAX,
	ENUM_DUMMY4WORD(MOVIEPLAY_DISP_ID)
} MOVIEPLAY_DISP_ID;

typedef enum {
	MOVIEPLAY_VID_OUT_1 = 0,
	MOVIEPLAY_VID_OUT_2,
	MOVIEPLAY_VID_OUT_MAX,
	ENUM_DUMMY4WORD(MOVIEPLAY_VID_OUT)
} MOVIEPLAY_VID_OUT;

// The following part is moved to ImageApp_MovieCommon.h
#if 0
typedef enum {
	_CFG_CODEC_MJPG = 1,
	_CFG_CODEC_H264,
	_CFG_CODEC_H265,
	ENUM_DUMMY4WORD(MOVIEPLAY_CFG_CODEC)
} MOVIEPLAY_CFG_CODEC;

typedef enum {
	_CFG_AUD_CODEC_NONE = 0,
	_CFG_AUD_CODEC_PCM,
	_CFG_AUD_CODEC_AAC,
	ENUM_DUMMY4WORD(MOVIEPLAY_CFG_AUD_CODEC)
} MOVIEPLAY_CFG_AUD_CODEC;

typedef enum {
	_CFG_FILE_FORMAT_MP4 = 0,
	_CFG_FILE_FORMAT_TS,
	_CFG_FILE_FORMAT_MOV,
	ENUM_DUMMY4WORD(CAMMOVIE_CFG_FILE_FORMAT)
} CAMMOVIE_CFG_FILE_FORMAT;
#endif

typedef enum {
	_CFG_PLAY_TYPE_NORMAL = 0,          ///< Normal play
	_CFG_PLAY_TYPE_FASTFWR,             ///< Fast-forward
	_CFG_PLAY_TYPE_FASTRWD,             ///< Fast-rewind
	_CFG_PLAY_TYPE_STEP,                ///< Step play
	_CFG_PLAY_TYPE_STRM_FILE,           ///< Stream file
	_CFG_PLAY_TYPE_STRM_LIVE,           ///< Stream live view
	ENUM_DUMMY4WORD(MOVIEPLAY_CFG_PLAY_MODE)
} MOVIEPLAY_CFG_PLAY_MODE;

typedef struct {
	MOVIEPLAY_CFG_PLAY_ID           fileplay_id;      ///< play ID
	FST_FILE                        file_handle;      ///< file handler
	UINT32                          curr_speed;       ///< current play speed
	UINT32                          curr_direct;      ///< current play direction
	UINT32                          blk_time;         ///< play time per one read file block
	UINT32                          blk_plnum;        ///< preload block number
	UINT32                          blk_ttnum;        ///< total block number
	void (*event_cb)(UINT32 event_id);                ///< user play callback event
    SMEDIAPLAY_MAX_MEM_INFO         *pmax_mem_info;   ///<[in] Max memory information
} MOVIEPLAY_FILEPLAY_INFO;

typedef struct {
	BOOL                 enable;
	MOVIEPLAY_DISP_ID    disp_id;
	UINT32               width;
	UINT32               height;
	UINT32               width_ratio;
	UINT32               height_ratio;
	UINT32               rotate_dir;
	MOVIEPLAY_VID_OUT    vid_out;		///< VdoOut path number
	UINT32				 dev_id;
} MOVIEPLAY_DISP_INFO;

enum {
	MOVIEPLAY_CONFIG_DISP_INFO  = 0xA000A000,
	MOVIEPLAY_CONFIG_FILEPLAY_INFO,
	MOVIEPLAY_CONFIG_STRMPLAY_INFO,
};

enum MOVIEPLAY_VID_SPEEDUP {
	MOVIEPLAY_MOV_1x = 1,
	MOVIEPLAY_MOV_2x = 2,
	MOVIEPLAY_MOV_4x = 4,
	MOVIEPLAY_MOV_8x = 8,
};

// For setting parameters
enum {
	MOVIEPLAY_PARAM_START   = 0x0000AA000,
	MOVIEPLAY_PARAM_FILEHDL = MOVIEPLAY_PARAM_START,                         ///< file handle
	MOVIEPLAY_PARAM_CURSPD,                                                  ///< current speed
	MOVIEPLAY_PARAM_CURDIR,                                                  ///< current direction
	MOVIEPLAY_PARAM_EVENTCB,                                                 ///< callback event
	MOVIEPLAY_PARAM_MAX_MEM_INFO,
};

/**
    Memory Allocation Information
*/
typedef enum _MOVIEPLAY_MEM_INDEX {
	MOVIEPLAY_MEM_VDOENTTBL = 0,    ///< video frame entry table
	MOVIEPLAY_MEM_AUDENTTBL,                          ///< audio frame entry table
	MOVIEPLAY_MEM_CNT,
	ENUM_DUMMY4WORD(_MOVIEPLAY_MEM_INDEX)
} MOVIEPLAY_MEM_INDEX;

/**
	Callback Event for Play_MovieCB()
*/
typedef enum {
	MOVIEPLAY_EVENT_MEDIAINFO_READY,                  ///< media info ready
	MOVIEPLAY_EVENT_ONE_DISPLAYFRAME,                 ///< one display frame
	// [ToDo]
	MOVIEPLAY_EVENT_START,                            ///< play start
	MOVIEPLAY_EVENT_STOP,                             ///< play stop
	MOVIEPLAY_EVENT_FINISH,                           ///< play finish
	MOVIEPLAY_EVENT_CURR_VIDFRAME,                    ///< current video frame stream data ready
	MOVIEPLAY_EVENT_CURR_AUDFRAME,                    ///< current audio frame stream data ready
	ENUM_DUMMY4WORD(MOVIEPLAY_EVENT)
} MOVIEPLAY_EVENT;

/**
	MoviePlay Parameter for NMediaPlay framework
*/
typedef enum {
	MOVIEPLAY_NM_PARAM_BEGIN = 0x00000000,
	/* common */
	MOVIEPLAY_NM_PARAM_VDO_CODEC = MOVIEPLAY_NM_PARAM_BEGIN, ///< video codec type
	MOVIEPLAY_NM_PARAM_BLK_TTNUM,                         ///< total block number
	MOVIEPLAY_NM_PARAM_GOP,                               ///< GOP (Group of Picture)
	MOVIEPLAY_NM_PARAM_AUD_SR,                            ///< audio sample rate
	/* FileIn */
	MOVIEPLAY_NM_PARAM_FILEHDL,                           ///< file handle
	MOVIEPLAY_NM_PARAM_FILESIZE,                          ///< total file size
	MOVIEPLAY_NM_PARAM_FILEDUR,                           ///< total file duration (sec)
	MOVIEPLAY_NM_PARAM_BLK_TIME,                          ///< play time per one read file block
	MOVIEPLAY_NM_PARAM_BLK_PLNUM,                         ///< preload block number
	MOVIEPLAY_NM_PARAM_CUR_VDOBS,                         ///< current used video bs addr & size
	/* BsDemux */
	MOVIEPLAY_NM_PARAM_VDO_ENABLE,                        ///< video enable (default: 0)
	MOVIEPLAY_NM_PARAM_VDO_FR,                            ///< video frame rate
	MOVIEPLAY_NM_PARAM_VDO_TTFRAME,                       ///< total video frames
	MOVIEPLAY_NM_PARAM_AUD_ENABLE,                        ///< audio enable (default: 0)
	MOVIEPLAY_NM_PARAM_AUD_TTFRAME,                       ///< total audio frames
	MOVIEPLAY_NM_PARAM_CONTAINER,                         ///< media container
	MOVIEPLAY_NM_PARAM_FILE_FMT,                          ///< file format (mp4/mov or ts)
	MOVIEPLAY_NM_PARAM_DISP_IDX,                          ///< current display video index
	/* VdoDec */
	MOVIEPLAY_NM_PARAM_DECDESC,                           ///< decode description
	MOVIEPLAY_NM_PARAM_WIDTH,                             ///< width
	MOVIEPLAY_NM_PARAM_HEIGHT,                            ///< height
	/* AudDec */
	MOVIEPLAY_NM_PARAM_AUD_CODEC,                         ///< audio codec type
	MOVIEPLAY_NM_PARAM_AUD_CHS,                           ///< audio channels
	/* GetParam */
	MOVIEPLAY_NM_PARAM_MEDIA_INFO,                        ///< get first media info
	/* Parameter maximum */
	MOVIEPLAY_NM_PARAM_MAX,
	ENUM_DUMMY4WORD(MOVIEPLAY_NM_PARAM)
} MOVIEPLAY_NM_PARAM;

/* NMediaPlay new param */
typedef struct {
	UINT64                          file_size;        ///< total file size
	FST_FILE                        file_handle;      ///< file handle
	UINT32                          file_dur;         ///< total file duration (sec)
	UINT32                          width;            ///< video width
	UINT32                          height;           ///< video height
	UINT32                          tk_wid;           ///< video track width for sample aspect ratio
	UINT32                          tk_hei;           ///< video track height for sample aspect ratio
	UINT32                          vdo_fr;           ///< video frame rate
	UINT32                          vdo_codec;        ///< video codec type
	UINT32                          vdo_ttfrm;        ///< total video frames
	UINT32                          desc_addr;        ///< video decode description addr (for H.264 and H.265)
	UINT32                          desc_size;        ///< video decode description size (for H.264 and H.265)
	UINT32                          aud_fr;           ///< audio frame rate
	UINT32                          aud_sr;           ///< audio sameple rate
	UINT32                          aud_chs;          ///< audio channels
	UINT32                          aud_codec;        ///< audio codec type
	UINT32                          aud_ttfrm;        ///< total audio frames
	UINT32                          gop;              ///< gop number
	BOOL                            vdo_en;           ///< video enable (default: 0)
	BOOL                            aud_en;           ///< audio enable (default: 0)
} MOVIEPLAY_PLAY_OBJ;

/**
    Set parameter for Movie Play.

    Set parameter for Movie Play.

    @param[in] id    MOVIEPLAY_CFG_PLAY_ID
    @param[in] param MOVIEPLAY_PARAM_ID.
    @param[in] value.

    @return ISF_RV
*/
extern ISF_RV ImageApp_MoviePlay_SetParam(UINT32 param, UINT32 value);

/**
    Get parameter for Movie Play.

    Get parameter for Movie Play.

    @param[in] param MOVIEPLAY_PARAM_ID.

    @return UINT32
*/
extern UINT32 ImageApp_MoviePlay_GetParam(UINT32 param);

/*
    Update Image Stream.

    Update Image Stream.

    @param[in] void

    @return ISF_RV
*/
extern ISF_RV ImageApp_MoviePlay_UpdateAll(void);

/**
    Config setting for Movie Play.

    Config setting for Movie Play.

    @param[in] void

    @return ISF_RV
*/
extern ISF_RV ImageApp_MoviePlay_Config(UINT32 config_id, UINT32 value);

/**
    Open Movie Play.

    Open Movie Play.

    @param[in] void

    @return ISF_RV
*/
extern ISF_RV ImageApp_MoviePlay_Open(void);

/**
    Close Movie Play.

    Close Movie Play.

    @param[in] void

    @return ISF_RV
*/
extern ISF_RV ImageApp_MoviePlay_Close(void);

/**
    Start Movie Play.

    Start Movie Play.

    @param[in] void

    @return ISF_RV
*/
extern ISF_RV ImageApp_MoviePlay_Start(void);

/**
    Pause Movie Play.

    Pause Movie Play.

    @param[in] void

    @return ISF_RV
*/
extern ISF_RV ImageApp_MoviePlay_FilePlay_Pause(void);

/**
    Resume Movie Play.

    Resume Movie Play.

    @param[in] void

    @return ISF_RV
*/
extern ISF_RV ImageApp_MoviePlay_FilePlay_Resume(void);

/**
    Update Speed and Direction.

    @param[in] speed    current play speed
    @param[in] direct   current play direction

    @return ISF_RV
*/
extern ISF_RV ImageApp_MoviePlay_FilePlay_UpdateSpeedDirect(UINT32 speed, UINT32 direct);

/**
    Trigger Step Play.

    Trigger Step Play.

    @param[in] direct

    @return ISF_RV
*/
ISF_RV ImageApp_MoviePlay_FilePlay_StepByStep(UINT32 direct);

//@}
#endif//IMAGEAPP_MOVIEPLAY_H