/*
    Copyright   Novatek Microelectronics Corp. 2005~2014.  All rights reserved.

    @file       ImageUnit_NetRTSP.h
    @ingroup    mImagePipe

    @note       Nothing.

    @date       2014/10/01
*/

#ifndef IMAGEUNIT_NETRTSP_H
#define IMAGEUNIT_NETRTSP_H

/*
Support these input port(s): IN1,IN2,IN3

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
#include "RtspNvtApi.h"

enum {
	NETRTSP_PARAM_START = 0x00003000,
	NETRTSP_PARAM_MAX_CLIENT = NETRTSP_PARAM_START,
	NETRTSP_PARAM_PORT,
	NETRTSP_PARAM_AUDIO,
	NETRTSP_PARAM_MEDIA_SOURCE,
	NETRTSP_PARAM_AVSUPPORT,
	NETRTSP_PARAM_IFRAME_REQ_CB,
	NETRTSP_PARAM_TOS,
};

enum{

	NETRTSP_PORT_PARAM_FPS,
};

typedef void (*RTSP_IFRAME_REQ_CB_FP)(UINT32 videoPathId);

extern ISF_UNIT ISF_NetRTSP;

#endif //IMAGEUNIT_NETRTSP_H

