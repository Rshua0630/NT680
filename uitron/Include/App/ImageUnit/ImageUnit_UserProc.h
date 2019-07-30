/*
    Copyright   Novatek Microelectronics Corp. 2017~2026.  All rights reserved.

    @file       ImageUnit_UserProc.h
    @note       Nothing.

    @date       2017/03/31
*/

#ifndef IMAGEUNIT_USERPROC_H
#define IMAGEUNIT_USERPROC_H

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

typedef enum {
	USERPROC_TRIG_PROC_NONE = 0,
	USERPROC_TRIG_PROC_EN   = 1,
	ENUM_DUMMY4WORD(USERPROC_TRIG_PROC_VAL)
} USERPROC_TRIG_PROC_VAL;

typedef USERPROC_TRIG_PROC_VAL (*USERPROC_PUSH_CB)(ISF_PORT *pPort, ISF_DATA *pData);
typedef void (*USERPROC_PROCESS_CB)(UINT32 iPort);

enum {
	USERPROC_PARAM_START = 0x00000000,
	USERPROC_PARAM_BYPASS_IMM = USERPROC_PARAM_START,
	USERPROC_PARAM_ALLOW_PULL_IMM,
	USERPROC_PARAM_PUSH_CALLBACK_IMM,
	USERPROC_PARAM_PROCESS_CALLBACK_IMM,
	USERPROC_PARAM_SCALE_WIDTH,
	USERPROC_PARAM_SCALE_HEIGHT,
	USERPROC_PARAM_SYNC_SIZE_REF_ID,

};



extern ISF_UNIT ISF_UserProc;

extern void     ISF_UserProc_InstallID(void) _SECTION(".kercfg_text");

#endif //IMAGEUNIT_USERPROC_H

