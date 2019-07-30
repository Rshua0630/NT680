/*
    Copyright   Novatek Microelectronics Corp. 2005~2014.  All rights reserved.

    @file       ImageUnit_StreamSender.h
    @ingroup    mImagePipe

    @note       Nothing.

    @date       2014/10/01
*/

#ifndef IMAGEUNIT_STREAMSENDER_H
#define IMAGEUNIT_STREAMSENDER_H

/*
Support these input port(s): IN1, IN2, IN3, ..., IN14, IN15

Support these output port(s):

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

#define STREAMSENDER_CHAN_NUM       16

#define STRMSND_ISF_DATASET_MAX     60

#define STRMSND_ACTIVE_PORT_MAX     8

enum {
	STMSND_PARAM_START = 0x80008000,                     ///< a non-zero arbitrary value
	STMSND_PARAM_SLOTNUM_AUDIO_IMM = STMSND_PARAM_START, ///< The slot number of audio stream data
	STMSND_PARAM_SLOTNUM_VIDEO_IMM,                      ///< The slot number of video stream data
	STMSND_PARAM_SET_AVGSIZE_GOPNUM,                     ///< Set the GOP number of average size
	STMSND_PARAM_GET_AVGSIZE_CH00,                       ///< Get average size of channel 00
	STMSND_PARAM_GET_AVGSIZE_CH01,                       ///< Get average size of channel 01
	STMSND_PARAM_GET_AVGSIZE_CH02,                       ///< Get average size of channel 02
	STMSND_PARAM_SET_COMM_POOL_ENABLE,                   ///< Enable common pool to using in linux
	STMSND_PARAM_SET_AUTO_REL_ISF,                       ///< Auto release isf data (Default is ON). Turn off to support lock mechanism.
};

extern ISF_UNIT ISF_StreamSender;

extern void ISF_StreamSender_InstallID(void) _SECTION(".kercfg_text");

#endif //IMAGEUNIT_STREAMSENDER_H

