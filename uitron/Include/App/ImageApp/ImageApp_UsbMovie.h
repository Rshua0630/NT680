/*
    Copyright   Novatek Microelectronics Corp. 2014.  All rights reserved.

    @file       ImageApp_UsbMovie.h
    @ingroup    mIImageApp

    @note       Nothing.

    @date       2014/11/28
*/

#ifndef USBMOVIE_H
#define USBMOVIE_H

#include "GxLib.h"
#include "AudFilterAPI.h"
#include "ImageStream.h"
#include "ImageUnit_ImagePipe.h"
#include "UVAC.h"
#include "ImageUnit_Cap.h"

/**
    @addtogroup mIImageApp
*/
//@{

/**
     @name Tagret bitrate for MJPEG. Unit is Byte/Sec.

     @note for NVT_USBMOVIE_CFG_TBR_MJPG
*/
//@{
#define NVT_USBMOVIE_TBR_MJPG_LOW       0x200000  ///< 2M
#define NVT_USBMOVIE_TBR_MJPG_HIGH      0x1000000 ///< 16M
#define NVT_USBMOVIE_TBR_MJPG_DEFAULT   0x800000  ///< 8M
//@}

/**
     @name Tagret bitrate for H264. Unit is Byte/Sec.

     @note for NVT_USBMOVIE_CFG_TBR_H264
*/
//@{
#define NVT_USBMOVIE_TBR_H264_LOW       0x100000  ///< 1M
#define NVT_USBMOVIE_TBR_H264_HIGH      0x800000  ///< 8M
#define NVT_USBMOVIE_TBR_H264_DEFAULT   0x200000  ///< 2M
//@}

/**
     @name Image ratio.

     @note for ImageUnit_CfgAspect()
*/
//@{
#define NVT_USBMOVIE_CROPRATIO_W_16     16
#define NVT_USBMOVIE_CROPRATIO_W_4      4
#define NVT_USBMOVIE_CROPRATIO_H_9      9
#define NVT_USBMOVIE_CROPRATIO_H_3      3
//@}


/**
     @name Usb movie mode.

     @note for NVT_USBMOVIE_CFG_MODE
*/
//@{
#define NVT_USBMOVIE_MODE_CAM             0    ///< like a PC cam
//#define NVT_USBMOVIE_MODE_MULTIMEDIA      1    ///< for multimedia system
//@}

/**
     @name Usb media format.

     Uvc payload format.

     @note for NVT_USBMOVIE_CFG_MEDIA_FMT
*/
//@{
#define NVT_USBMOVIE_MEDIA_MJPEG            0    ///< MJPEG
#define NVT_USBMOVIE_MEDIA_H264             1    ///< H264
//@}


typedef ISIZE     MOVIE_CFG_IMG_SIZE;
typedef UINT32    MOVIE_CFG_FRAME_RATE;
typedef UINT32    MOVIE_CFG_TARGET_RATE;
typedef UINT32    MOVIE_CFG_AUD_RATE;
typedef UINT32    MOVIE_CFG_AUD_CH;
typedef ISIZE     MOVIE_CFG_IMG_RATIO;
typedef ISIZE     MOVIE_CFG_RAWENC_SIZE;

typedef UINT32    MOVIE_CFG_GOP_NUM;
typedef BOOL      MOVIE_CFG_RTSP_STRM_OUT;

typedef BOOL      MOVIE_CFG_FILE_OPTION;
typedef UINT32    MOVIE_CFG_FILE_SECOND;
typedef UINT32    MOVIE_CFG_FILE_ENDTYPE;

typedef CHAR     *MOVIE_CFG_ROOT_PATH;
typedef MEM_RANGE MOVIE_CFG_MEM_RANGE;
typedef CHAR     *MOVIE_CFG_FOLDER_NAME;

/**
    Configuration identifier

    @note for ImageApp_UsbMovie_Config(), ImageApp_UsbMovie_GetConfig()
*/
typedef enum {
	USBMOVIE_CFG_CHANNEL,               ///< UVC channel number
	USBMOVIE_CFG_TBR_MJPG,              ///< tagret bitrate for MJPEG. Unit is Byte/Sec.
	USBMOVIE_CFG_TBR_H264,              ///< tagret bitrate for H264. Unit is Byte/Sec.
	USBMOVIE_CFG_SET_MOVIE_SIZE_CB,     ///< the callback is invoked while UVC is started by USB host
	USBMOVIE_CFG_MODE,                  ///< for PC cam mode or multimedia mode
	USBMOVIE_CFG_MEDIA_FMT,             ///< payload data is MJPEG or H264, for multimedia mode only
	USBMOVIE_CFG_VDOIN_INFO,
	USBMOVIE_CFG_DISP_INFO,
	USBMOVIE_CFG_DUAL_DISP,
	USBMOVIE_CFG_MAX,
} USBMOVIE_CFG;

/**
    Specific function enable
*/
typedef enum _NVT_USBMOVIE_SFUNC_EN {
	NVT_USBMOVIE_SFUNC_NONE             = 0x00000000,    ///< disable specific function related IPL path.
	NVT_USBMOVIE_SFUNC_IPL1_PATH3       = 0x00010000,    ///< Link IPL1 path3 to ImageUnit_ALG ISF_IN1
	NVT_USBMOVIE_SFUNC_IPL1_PATH4       = 0x00020000,    ///< Link IPL1 path4 to ImageUnit_ALG ISF_IN2
	NVT_USBMOVIE_SFUNC_IPL2_PATH3       = 0x00040000,    ///< Link IPL2 path3 to ImageUnit_ALG ISF_IN3
	NVT_USBMOVIE_SFUNC_IPL2_PATH4       = 0x00080000,    ///< Link IPL2 path4 to ImageUnit_ALG ISF_IN4
	NVT_USBMOVIE_SFUNC_IPL1_PATH3_PVEN  = 0x00100000,    ///< IPL1 path3 output enable when preview
	NVT_USBMOVIE_SFUNC_IPL1_PATH4_PVEN  = 0x00200000,    ///< IPL1 path4 output enable when preview
	NVT_USBMOVIE_SFUNC_IPL2_PATH3_PVEN  = 0x00400000,    ///< IPL2 path3 output enable when preview
	NVT_USBMOVIE_SFUNC_IPL2_PATH4_PVEN  = 0x00800000,    ///< IPL2 path4 output enable when preview
	NVT_USBMOVIE_SFUNC_IPL_MASK         = 0x00ff0000,    ///< Mask of IPL1/2 path3/4 setting
	ENUM_DUMMY4WORD(NVT_USBMOVIE_SFUNC_EN)
} NVT_USBMOVIE_SFUNC_EN;

/**
    Video resolution.

    @note for the parameter of IMAGAPP_UVAC_SET_VIDRESO_CB
*/
typedef struct _NVT_USBMOVIE_VID_RESO {
	UINT32 width;
	UINT32 height;
	UINT32 fps;
	UVAC_VIDEO_FORMAT codec;
} NVT_USBMOVIE_VID_RESO, *PNVT_USBMOVIE_VID_RESO;

typedef enum {
	UVAC_STRM_ID_MIN = 0,
	UVAC_STRM_ID_1 = UVAC_STRM_ID_MIN,
	UVAC_STRM_ID_2,
	UVAC_STRM_ID_MAX,
	ENUM_DUMMY4WORD(UVAC_STRM_ID)
} UVAC_STRM_ID;

typedef enum {
	UVAC_DISP_ID_MIN = UVAC_STRM_ID_MAX,
	UVAC_DISP_ID_1 = UVAC_DISP_ID_MIN,
	UVAC_DISP_ID_2,
	UVAC_DISP_ID_MAX,
	ENUM_DUMMY4WORD(UVAC_DISP_ID)
} UVAC_DISP_ID;

typedef enum {
	UVAC_VID_IN_1 = 0,
	UVAC_VID_IN_2,
	UVAC_VID_IN_MAX,
	ENUM_DUMMY4WORD(UVAC_VID_IN)
} UVAC_VID_IN;

typedef enum {
	UVAC_VID_OUT_1 = 0,
	UVAC_VID_OUT_2,
	UVAC_VID_OUT_MAX,
	ENUM_DUMMY4WORD(UVAC_VID_OUT)
} UVAC_VID_OUT;

typedef enum {
	UVAC_VID_CODEC_MJPG = 1,
	UVAC_VID_CODEC_H264,
	ENUM_DUMMY4WORD(UVAC_VID_CODEC)
} UVAC_VID_CODEC;

typedef enum {
	UVAC_AUD_CODEC_PCM = 0,
	UVAC_AUD_CODEC_AAC,
	ENUM_DUMMY4WORD(UVAC_AUD_CODEC)
} UVAC_AUD_CODEC;

typedef struct {
	BOOL enable;
	ISIZE disp_size;
	USIZE disp_ar;
} UVAC_DUAL_DISP;

typedef enum {
	UVAC_PATH_ID_0 = 0,
	UVAC_PATH_ID_1 = 1,
	UVAC_PATH_ID_2 = 2,
	UVAC_PATH_ID_3 = 3,
	UVAC_PATH_ID_4 = 4,
	UVAC_PATH_ID_5 = 5,
	UVAC_PATH_ID_6 = 6,
	UVAC_PATH_ID_7 = 7,
	UVAC_PATH_ID_8 = 8,
	UVAC_PATH_ID_9 = 9,
	UVAC_PATH_ID_10 = 10,
	UVAC_PATH_ID_11 = 11,
	UVAC_PATH_ID_12 = 12,
	UVAC_PATH_ID_13 = 13,
	UVAC_PATH_ID_14 = 14,
	UVAC_PATH_ID_15 = 15,
	UVAC_PATH_MAX_ID = 16,
	ENUM_DUMMY4WORD(UVAC_PATH_ID)
} UVAC_PATH_ID;

typedef enum {
	UVAC_SENSOR1 = 0,
	UVAC_SENSOR2,
	UVAC_SENSOR3,
	UVAC_SENSOR4,
	UVAC_SENSOR5,
	UVAC_SENSOR6,
	UVAC_SENSOR7,
	UVAC_SENSOR8,
	UVAC_SENSOR_MAX,
	ENUM_DUMMY4WORD(UVAC_SENSOR)

} UVAC_SENSOR;

typedef struct {
	UVAC_VID_IN         vid_in;
	ISF_OUT             imagepipe_out;
} UVAC_VDO_IN_INFO;

typedef struct {
	BOOL                 enable;
	UVAC_DISP_ID         disp_id;
	UVAC_VID_IN          vid_in;
	UINT32               width;
	UINT32               height;
	UINT32               width_ratio;
	UINT32               height_ratio;
	UINT32               rotate_dir;
	UVAC_VID_OUT         vid_out;
} UVAC_DISP_INFO;

/**
    @name Callback prototype for NVT_USBMOVIE_CFG_SET_MOVIE_SIZE_CB

    @param[in] pVidReso1 pointer to video resolution of UVC channel 1 , refering to #_NVT_USBMOVIE_VID_RESO
    @param[in] pVidReso2 pointer to video resolution of UVC channel 2 , refering to #_NVT_USBMOVIE_VID_RESO

    @return void
*/
//@{
typedef void (*IMAGAPP_UVAC_SET_VIDRESO_CB)(PNVT_USBMOVIE_VID_RESO pVidReso1, PNVT_USBMOVIE_VID_RESO pVidReso2);
//@}

/**
    Open USB movie mode.

    Open USB movie mode.

    @param[in] void

    @return void
*/
extern void ImageApp_UsbMovie_Open(void);

/**
    Close USB movie mode.

    Close USB movie mode.

    @param[in] void

    @return void
*/
extern void ImageApp_UsbMovie_Close(void);

/**
    Start recording.

    In NVT_USBMOVIE_MODE_CAM, start recording media stream to USB host.
    In NVT_USBMOVIE_MODE_MULTIMEDIA, start recording file.

    @return void
*/
extern void ImageApp_UsbMovie_RecStart(void);

/**
    Stop recording or start live view.

    In NVT_USBMOVIE_MODE_CAM, stop recording and transfering stream to USB host.
    In NVT_USBMOVIE_MODE_MULTIMEDIA, stop recording file in NVT_USBMOVIE_MODE_MULTIMEDIA.

    @return void
*/
extern void ImageApp_UsbMovie_RecStop(void);

/**
    Stop live view.

    Only valid in NVT_USBMOVIE_MODE_MULTIMEDIA, stop transfering stream to USB host.

    @return void
*/
extern void ImageApp_UsbMovie_LviewStop(void);

/**
    Start snapshot. (take a picture in movie by extracting recording frame)

    Start snapshot.

    @return void
*/
extern void ImageApp_UsbMovie_Snapshot(void);

/**
    Config settings for USB movie mode.

    Config settings for USB movie mode.

    @param[in] config_id refer to NVT_USBMOVIE_CFG
    @param[in] value configuration value.

    @return void
*/
extern void ImageApp_UsbMovie_Config(UINT32 config_id, UINT32 value);

/**
    Get config settings for USB movie mode.

    Get config settings for USB movie mode.

    @param[in] config_id refer to NVT_USBMOVIE_CFG

    @return configuration value
*/
extern UINT32 ImageApp_UsbMovie_GetConfig(UINT32 config_id);

extern ISF_RV ImageApp_UVAC_SetParam(UINT32 param, UINT32 value);

void ImageApp_UsbMovie_SetVdoImgSize(UINT32 path_index, UINT32 w, UINT32 h);
void ImageApp_UsbMovie_SetVdoAspectRatio(UINT32 path_index, UINT32 w, UINT32 h);
void ImageApp_UsbMovie_ResetPath(UINT32 path_index);

//@}
#endif//USBMOVIE_H

