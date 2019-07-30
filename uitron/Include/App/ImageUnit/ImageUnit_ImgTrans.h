/*
    Copyright   Novatek Microelectronics Corp. 2017~2026.  All rights reserved.

    @file       ImageUnit_ImgTrans.h
    @note       Nothing.

    @date       2017/03/31
*/

#ifndef IMAGEUNIT_IMGTRANS_H
#define IMAGEUNIT_IMGTRANS_H

/*
Support these input port(s): IN1,IN2 ~ IN16

Support these output port(s): OUT1,OUT2 ~ OUT16

Support these port config API(s):

============================================================
port
------------------------------------------------------------
ImageUnit_CfgImgSize()      [Y]
ImageUnit_CfgImgAspect()    [Y]
ImageUnit_CfgImgWindow()
ImageUnit_CfgImgDirect()
============================================================
*/


#include "Type.h"
#include "ImageStream.h"

enum {
	IMGTRANS_PARAM_START = 0x00000000,
	IMGTRANS_PARAM_RESET_IMM = IMGTRANS_PARAM_START,
	IMGTRANS_PARAM_BYPASS_IMM,
	IMGTRANS_PARAM_CONNECT_IMM,
	IMGTRANS_PARAM_DISCONNECT_IMM,
	IMGTRANS_PARAM_MAX_IMG_WIDTH_IMM,
	IMGTRANS_PARAM_MAX_IMG_HEIGHT_IMM,
	IMGTRANS_PARAM_FPS_IMM,
};

extern ISF_UNIT ISF_ImgTrans;

extern void       ISF_ImgTrans_InstallID(void) _SECTION(".kercfg_text");


#endif //IMAGEUNIT_IMGTRANS_H

