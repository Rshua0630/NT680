/*
    Copyright   Novatek Microelectronics Corp. 2005~2014.  All rights reserved.

    @file       ImageUnit_Play.h
    @ingroup    mPlay

    @note       Nothing.

    @date       2014/10/01
*/

#ifndef IMAGEUNIT_PLAY_H
#define IMAGEUNIT_PLAY_H

/*
Support these input port(s):

Support these output port(s): OUT1,OUT2

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
	PLAY_PARAM_START = 0x00005000,
	PLAY_PARAM_PANELSZ = PLAY_PARAM_START,
	PLAY_PARAM_DISP_SIZE,
	PLAY_PARAM_MAX_RAW_SIZE,
	PLAY_PARAM_MAX_FILE_SIZE,
	PLAY_PARAM_DUAL_DISP,
	PLAY_PARAM_DEC_VID,
	PLAY_PARAM_PLAY_FMT,
	PLAY_PARAM_SUSPEND_CLOSE_IMM,
	PLAY_PARAM_POOL_ADDRESS,
	PLAY_PARAM_DEC_SAR,
};

extern ISF_UNIT ISF_Play;

#endif //IMAGEUNIT_PLAY_H

