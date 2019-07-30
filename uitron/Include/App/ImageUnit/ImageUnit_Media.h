/*
    Copyright   Novatek Microelectronics Corp. 2005~2014.  All rights reserved.

    @file       ImageUnit_Media.h
    @ingroup    mImagePipe

    @note       Nothing.

    @date       2014/10/01
*/

#ifndef IMAGEUNIT_MEDIA_H
#define IMAGEUNIT_MEDIA_H

/*
Support these input port(s): IN1,IN2,IN3,IN4

Support these output port(s): OUT1,OUT2,OUT3,OUT4

Support these port config API(s):

============================================================
port
------------------------------------------------------------
ImageUnit_CfgImgSize()
ImageUnit_CfgImgAspect()
ImageUnit_CfgImgWindow()
ImageUnit_CfgImgDirect()
============================================================
*/

#include "Type.h"
#include "ImageStream.h"


enum {
	MEDIA_PARAM_START = 0x00008000,
	MEDIA_PARAM_USE_FILEDB = MEDIA_PARAM_START,
	MEDIA_PARAM_FILEDB_MEM,
	MEDIA_PARAM_DRAW_CB,                 // register callback function for data drawing (such as date stamp) as movie recording.
	MEDIA_PARAM_AUDIOFILTER_CB,          // register callback function for audio filtering as movie recording.
	MEDIA_PARAM_IMM_CB,                  // register callback function for the events which should be processed immediately
	MEDIA_PARAM_RAWENC_CB,               // register callback function for raw encode
	MEDIA_PARAM_INPUTPATH,
	MEDIA_PARAM_AUDIOVOL,
	MEDIA_PARAM_GET_STATE,
	MEDIA_PARAM_SNAP1,                   // enable snapshot of path 1
	MEDIA_PARAM_SNAP2,                   // enable snapshot of path 2
	MEDIA_PARAM_REC_CB,
	MEDIA_PARAM_USE_BUF_MAX_FHD,         // IPL buffer use maximum 1920x1088
	MEDIA_PARAM_DRAW_DIRECT_CB,          // register callback function for data drawing (such as date stamp) on Record Direct Path.
	MEDIA_PARAM_MOVIEPRV_CHGMODE,        // indicate to change IPL preview mode while set ISF_OMD_MOVIEPRV mode on opening CamMovie mode (default: TRUE).
	MEDIA_PARAM_USE_BUF_MAX_SIZE,        // indicate to use Maximum resolution for IPL bufffer (default:TRUE).
	MEDIA_PARAM_DUAL_SENSOR_SBS,         // indicate to enable side by side on dual sensor application
	MEDIA_PARAM_VASTRM_MODE_EN,          // indicate to enable VA Stream mode
	MEDIA_PARAM_VASTRM_UPDATE_CB,        // regiter callback function for VA stream update parameters.
	MEDIA_PARAM_RAWENC_NOFILE,           // indicate to Raw Encode function under no card case.
	MEDIA_PARAM_MAXSIZE_P1,              // indicate to P1 maximum size for crop function.
	MEDIA_PARAM_MAXSIZE_P2,              // indicate to P2 maximum size for crop function.
	MEDIA_PARAM_MAXSIZE_P3,              // indicate to P3 maximum size for crop function.
	MEDIA_PARAM_CROP_P1,                 // indicate to P1 crop window size for crop function.
	MEDIA_PARAM_CROP_P2,                 // indicate to P2 crop window size for crop function.
	MEDIA_PARAM_CROP_P3,                 // indicate to P3 crop window size for crop function.
	MEDIA_PARAM_RAWENC_P1,               // indicate to P1 Raw Encode size.
	MEDIA_PARAM_RAWENC_P2,               // indicate to P2 Raw Encode size.
	MEDIA_PARAM_RAWENC_P3,               // indicate to P3 Raw Encode size.
	MEDIA_PARAM_MEDIA_4_0_MODE_EN,       // indicate to enable media 4.0 mode
	MEDIA_PARAM_IPLCHGCB_DELAY,          // indicate to set delay time (ms) during IPL change mode CB
	MEDIA_PARAM_DISABLE_PATH2_WRITE,     // disable path2 write card.
	MEDIA_PARAM_DISABLE_PATH1_WRITE,     // disable path1 write card.
	MEDIA_PARAM_VASTRM_WOA_MODE_EN,      // indicate to enable VA Stream mode (without Audio)
	MEDIA_PARAM_ASTRM_PCM_OUTPUT,        // indicate to output PCM audio stream
	MEDIA_PARAM_KEEP_IPL_MODE,           // indicate to keep IPL mode while Media IPL Change CB (default:FALSE)
};

typedef struct {
	UINT32  iPort;
	UINT32  oPort;
	UINT32  bIsConnected;
	UINT32  bStartRec;
	UINT32  bPushReady;
} ISF_MEDIA_RECPATH_INFO;

typedef struct {
	UINT32  bEnCrop;
	UINT32  uiMaxWidth;
	UINT32  uiMaxHeight;
	UINT32  uiCropW;
	UINT32  uiCropH;
	UINT32  uiCropX;
	UINT32  uiCropY;
} ISF_MEDIA_CROP_INFO;


extern ISF_UNIT ISF_Media;

typedef void (*MEDIA_VASTRM_UPDATE_CB)(UINT32 path);

#endif //IMAGEUNIT_MEDIA_H
