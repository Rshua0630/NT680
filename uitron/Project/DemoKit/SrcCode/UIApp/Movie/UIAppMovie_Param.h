#ifndef _UIAPPMOVIE_Param_H
#define _UIAPPMOVIE_Param_H

#include "ipl_ctrl_flow.h"
#include "Type.h"
#include "GxImage.h"

typedef enum {
	UIAPPMOVIE_SENSOR1 = 0,
	UIAPPMOVIE_SENSOR2,
	UIAPPMOVIE_SENSOR3,
	UIAPPMOVIE_SENSOR4,
	UIAPPMOVIE_SENSOR5,
	UIAPPMOVIE_SENSOR6,
	UIAPPMOVIE_SENSOR7,
	UIAPPMOVIE_SENSOR8,
	UIAPPMOVIE_SENSOR_MAX,
	ENUM_DUMMY4WORD(UIAPPMOVIE_SENSOR)

} UIAPPMOVIE_SENSOR;

typedef enum {
	UIAPPMOVIE_PATH_ID_0 = 0,
	UIAPPMOVIE_PATH_ID_1 = 1,
	UIAPPMOVIE_PATH_ID_2 = 2,
	UIAPPMOVIE_PATH_ID_3 = 3,
	UIAPPMOVIE_PATH_ID_4 = 4,
	UIAPPMOVIE_PATH_ID_5 = 5,
	UIAPPMOVIE_PATH_ID_6 = 6,
	UIAPPMOVIE_PATH_ID_7 = 7,
	UIAPPMOVIE_PATH_ID_8 = 8,
	UIAPPMOVIE_PATH_ID_9 = 9,
	UIAPPMOVIE_PATH_ID_10 = 10,
	UIAPPMOVIE_PATH_ID_11 = 11,
	UIAPPMOVIE_PATH_ID_12 = 12,
	UIAPPMOVIE_PATH_ID_13 = 13,
	UIAPPMOVIE_PATH_ID_14 = 14,
	UIAPPMOVIE_PATH_ID_15 = 15,
	UIAPPMOVIE_PATH_MAX_ID = 16,
	ENUM_DUMMY4WORD(UIAPPMOVIE_PATH_ID)
} UIAPPMOVIE_PATH_ID;


typedef struct {
	UIAPPMOVIE_SENSOR       sensor;
	UINT32              fps;
	ISIZE               sRatio;
	FLIP_TYPE           eFlipType;
	BOOL                bIME3DNR;
	GX_IMAGE_PIXEL_FMT  eVideoFormat[5];
	BOOL                bIME1Compress;

} UIAPPMOVIE_SENSOR_INFO;


typedef struct {

	UIAPPMOVIE_PATH_ID      path;
	UINT32                  fps;
	ISIZE                   resolution;
	UINT32                  bitrate;
	UINT32                  codec; //value: MEDIAVIDENC_MJPG, MEDIAVIDENC_H264, MEDIAVIDENC_H265
	INT32                   i3DNRLevel; //encoder 3DNR

} UIAPPMOVIE_ENCODE_CONFIG;

typedef struct {

	UINT32 codec;
	UINT32 channel;
	UINT32 sample_rate;
	UINT32 volume;


} UIAPPMOVIE_AUDIO_INFO;


extern void UIAppMovie_set_SensorInfo(UIAPPMOVIE_SENSOR_INFO *SensorInfo);

extern UIAPPMOVIE_SENSOR_INFO *UIAppMovie_get_SensorInfo(void);

extern void UIAppMovie_set_EncodeConfig(UIAPPMOVIE_ENCODE_CONFIG *EncodeConfig);

extern UIAPPMOVIE_ENCODE_CONFIG *UIAppMovie_get_EncodeConfig(void);

extern void *ImageApp_get_streamInfo(void);

extern void ImageApp_set_StreamInfo(void *streamInfo);

extern void UIAppMovie_set_DisplayPath(INT32 path);

extern INT32 UIAppMovie_get_DisplayPath(void);

extern INT32 UIAppMovie_get_ALGPath(void);

extern void UIAppMovie_set_ALGPath(INT32 path);

extern UIAPPMOVIE_AUDIO_INFO *UIAppMovie_get_AudioInfo(void);

extern void UIAppMovie_set_AudioInfo(UIAPPMOVIE_AUDIO_INFO *pAudioInfo);

extern UIAPPMOVIE_AUDIO_INFO *UIAppMovie_get_AudioOutInfo(void);

extern void UIAppMovie_set_AudioOutInfo(UIAPPMOVIE_AUDIO_INFO *pAudioOutInfo);
#endif

