/*
    Copyright   Novatek Microelectronics Corp. 2017.  All rights reserved.

    @file       ImageApp_Photo.h
    @ingroup    mIImageApp

    @note       Nothing.

    @date       2017/06/01
*/

#ifndef IA_PHOTO_H
#define IA_PHOTO_H

#include "GxLib.h"

#include "ImageStream.h"
#include "ImageUnit_ImagePipe.h"
#include "ImageUnit_VdoOut.h"
#include "ImageUnit_CamFile.h"
#include "ImageUnit_Cap.h"
#include "NVTEvent.h"
#include "VideoEncode.h"

/**
    @addtogroup mIImageApp
*/
//@{

/**
     @name Configuration ID for Config().
*/
//@{
#define PHOTO_CFG_DISP_INFO     1           ///<  photo display info
#define PHOTO_CFG_STRM_INFO     2           ///<  photo streaming info
#define PHOTO_CFG_IME3DNR_INFO  3           ///<  photo IME3DNR info
#define PHOTO_CFG_STRM_TYPE     4           ///<  photo streaming type, HTTP or RTSP
#define PHOTO_CFG_FBD_POOL      5           ///<  filedb pool memory range
#define PHOTO_CFG_ROOT_PATH     6           ///<  photo write file root path
#define PHOTO_CFG_FOLDER_NAME   7           ///<  photo write file folder name
#define PHOTO_CFG_DUAL_DISP   8           ///<  photo dual disp
#define PHOTO_CFG_DRAW_CB       9           ///<  photo draw callback
#define PHOTO_CFG_CBR_INFO          10
//@}

typedef void(PHOTO_DRAW_CB)(UINT32  yAddr, UINT32 cbAddr, UINT32 crAddr, UINT32 lineOfsY);                 ///< Callback for draw image.



typedef enum {
	PHOTO_DISP_ID_MIN = 0,
	PHOTO_DISP_ID_1 = PHOTO_DISP_ID_MIN,
	PHOTO_DISP_ID_2,
#if defined(_BSP_NA51000_)
	PHOTO_DISP_ID_3,
	PHOTO_DISP_ID_4,
#endif
	PHOTO_DISP_ID_MAX,
	ENUM_DUMMY4WORD(PHOTO_DISP_ID)
} PHOTO_DISP_ID;

typedef enum {
	PHOTO_STRM_ID_MIN = PHOTO_DISP_ID_MAX,
	PHOTO_STRM_ID_1 = PHOTO_STRM_ID_MIN,
	PHOTO_STRM_ID_2,
#if defined(_BSP_NA51000_)
	PHOTO_STRM_ID_3,
	PHOTO_STRM_ID_4,
#endif
	PHOTO_STRM_ID_MAX,
	ENUM_DUMMY4WORD(PHOTO_STRM_ID)
} PHOTO_STRM_ID;

typedef enum {
	PHOTO_IME3DNR_ID_MIN = PHOTO_STRM_ID_MAX,
	PHOTO_IME3DNR_ID_1 = PHOTO_IME3DNR_ID_MIN,
	PHOTO_IME3DNR_ID_2,
#if defined(_BSP_NA51000_)
	PHOTO_IME3DNR_ID_3,
	PHOTO_IME3DNR_ID_4,
#endif
	PHOTO_IME3DNR_ID_MAX,
	ENUM_DUMMY4WORD(PHOTO_IME3DNR_ID)
} PHOTO_IME3DNR_ID;

typedef enum {
	PHOTO_VID_IN_1 = 0,
	PHOTO_VID_IN_2,
#if defined(_BSP_NA51000_)
	PHOTO_VID_IN_3,
	PHOTO_VID_IN_4,
#endif
	PHOTO_VID_IN_MAX,
	ENUM_DUMMY4WORD(PHOTO_VID_IN)
} PHOTO_VID_IN;

typedef enum {
	PHOTO_VID_OUT_1 = 0,
	PHOTO_VID_OUT_2,
	PHOTO_VID_OUT_MAX,
	ENUM_DUMMY4WORD(PHOTO_VID_OUT)
} PHOTO_VID_OUT;


typedef enum {
	PHOTO_CAP_ID_1 = 0,
	PHOTO_CAP_ID_2,
#if defined(_BSP_NA51000_)
	PHOTO_CAP_ID_3,
	PHOTO_CAP_ID_4,
#endif
	PHOTO_CAP_ID_MAX,
	ENUM_DUMMY4WORD(PHOTO_CAP_ID)
} PHOTO_CAP_ID;

typedef enum {
	PHOTO_CODEC_MJPG = 1,
	PHOTO_CODEC_H264,
	PHOTO_CODEC_H265,
	ENUM_DUMMY4WORD(PHOTO_CODEC)
} PHOTO_CODEC;

typedef enum {
	PHOTO_STRM_TYPE_HTTP = 0,
	PHOTO_STRM_TYPE_RTSP,
	ENUM_DUMMY4WORD(PHOTO_STRM_TYPE)
} PHOTO_STRM_TYPE;


typedef enum {
	PHOTO_MULTI_VIEW_SINGLE = 0,         ///<  single sensor view
	PHOTO_MULTI_VIEW_PIP,                ///<  pip view
	PHOTO_MULTI_VIEW_SBS_LR,             ///<  sbs Left, Right view
	PHOTO_MULTI_VIEW_SBS_UD,             ///<  sbs Up , Down view
	ENUM_DUMMY4WORD(PHOTO_MULTI_VIEW)
} PHOTO_MULTI_VIEW;



typedef struct {
	BOOL                 enable;
	PHOTO_MULTI_VIEW     multi_view_type;      ///<  enable PIP view
	PHOTO_DISP_ID        disp_id;
	PHOTO_VID_IN         vid_in;
	UINT32               width;
	UINT32               height;
	UINT32               width_ratio;
	UINT32               height_ratio;
	UINT32               rotate_dir;
	UINT32               is_merge_3dnr_path;
	PHOTO_VID_OUT        vid_out;
} PHOTO_DISP_INFO;


typedef struct {
	BOOL                 enable;
	PHOTO_MULTI_VIEW     multi_view_type;      ///<  enable PIP view
	PHOTO_STRM_ID        strm_id;
	PHOTO_VID_IN         vid_in;
	UINT32               width;
	UINT32               height;
	UINT32               width_ratio;
	UINT32               height_ratio;
	UINT32               frame_rate;
	UINT32               max_width;
	UINT32               max_height;
	UINT32               max_bitrate;
	UINT32               target_bitrate;
	PHOTO_CODEC          codec;
	PHOTO_STRM_TYPE      strm_type;
	UINT32               is_merge_3dnr_path;
} PHOTO_STRM_INFO;

typedef struct {
    BOOL                 enable;
	PHOTO_IME3DNR_ID     ime3dnr_id;
	PHOTO_VID_IN         vid_in;
	UINT32               width;
	UINT32               height;
	UINT32               width_ratio;
	UINT32               height_ratio;
	UINT32               max_width;
	UINT32               max_height;
	UINT32               is_merge_3dnr_path;
} PHOTO_IME3DNR_INFO;


typedef struct {
	PHOTO_CAP_ID         cap_id;
	UINT32               ipl_id;
	CHAR                *folder_path;
} PHOTO_CAP_FOLDER_NAMING;

typedef struct {
	BOOL enable;
	ISIZE disp_size;
	ISIZE disp_ar;
} PHOTO_DUAL_DISP;

typedef struct {
	BOOL                 enable;
	PHOTO_STRM_ID        strm_id;
	MP_VDOENC_CBR_INFO   cbr_info;
} PHOTO_STRM_CBR_INFO;

/**
    Config settings for Photo mode.

    Config settings for Photo mode.

    @param[in] config_id PHOTO_CFG_DISP_INFO or others.
    @param[in] value configuration value.

    @return void

    Example:
    @code
    {
        PHOTO_DISP_INFO  dscam_disp;

        dscam_disp.enable          =  TRUE;
        dscam_disp.enable_pip      =  FALSE;
        dscam_disp.disp_id         =  PHOTO_DISP_ID_1;
        dscam_disp.vid_in          =  PHOTO_VID_IN_1;
        dscam_disp.width           =  640;
        dscam_disp.height          =  480;
        dscam_disp.width_ratio     =  4;
        dscam_disp.height_ratio    =  3;
        ImageApp_Photo_Config(PHOTO_CFG_DISP_INFO, (UINT32)&dscam_disp);
    }
    @endcode
*/
extern void ImageApp_Photo_Config(UINT32 config_id, UINT32 value);


extern UINT32 ImageApp_Photo_GetConfig(UINT32 config_id);


/**
    Open photo mode.

    Open photo mode.

    @param[in] void

    @return void
*/
extern void ImageApp_Photo_Open(void);

/**
    Close photo mode.

    Close photo mode.

    @param[in] void

    @return void
*/

extern INT32 ImageApp_Photo_FileNaming_Open(void);


extern void ImageApp_Photo_SetVdoImgSize(UINT32 Path, UINT32 w, UINT32 h);

extern void ImageApp_Photo_SetVdoAspectRatio(UINT32 Path, UINT32 w, UINT32 h);

extern void ImageApp_Photo_ResetPath(UINT32 Path);


extern void ImageApp_Photo_Close(void);

extern UINT32 ImageApp_Photo_CapStart(void);

extern void ImageApp_Photo_CapStop(void);

extern void ImageApp_Photo_CapWaitFinish(void);

extern UINT32 ImageApp_Photo_SetScreenSleep(void);

extern UINT32 ImageApp_Photo_SetScreenWakeUp(void);

//@}
#endif//IA_PHOTO_H

