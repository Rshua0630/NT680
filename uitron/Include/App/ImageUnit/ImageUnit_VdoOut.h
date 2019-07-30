/*
    Copyright   Novatek Microelectronics Corp. 2005~2014.  All rights reserved.

    @file       ImageUnit_VdoOut.h
    @ingroup    mVdoOut

    @note       Nothing.

    @date       2014/10/01
*/

#ifndef IMAGEUNIT_VDOOUT_H
#define IMAGEUNIT_VDOOUT_H

/*
Support these port config API(s):

============================================================
port                        IN1,IN2
------------------------------------------------------------
ImageUnit_CfgImgSize()      [Y]
ImageUnit_CfgImgAspect()    [Y]
ImageUnit_CfgImgWindow()    [Y](*)
ImageUnit_CfgImgDirect()
============================================================
(*) set IDE output cast window

TODO: if ImagePipe1.out is connected to VdoOut.in
- if assign ImagePipe1's IME crop window, it will also affect VdoOut's IDE cast window?

*/

#include "Type.h"
#include "ImageStream.h"

enum {
	VDOOUT_PARAM_START = 0x00003000,
	VDOOUT_PARAM_DEVICE, ///< set object
	VDOOUT_PARAM_FORMAT,
	VDOOUT_PARAM_EVENTCB, ///< event callback
	VDOOUT_PARAM_SYNCSRC, ///< iPort: sync to output of this source unit
	VDOOUT_PARAM_FUNC, ///< iPort: function
	//VDOOUT_PARAM_DISPCOUNT, ///< total insert count
	//VDOOUT_PARAM_DISPATTACH, ///< insert
	//VDOOUT_PARAM_DISPDETACH, ///< remove
	//VDOOUT_PARAM_DISPMASK, ///< insert mask
	//VDOOUT_PARAM_VIEWSRC, ///< userdraw
	//VDOOUT_PARAM_DISPDIRECT, ///< userdraw mode
};

#define VDOOUT_EVENT_PUSH           0x01    //param1 = device id, param2 = (ISF_DATA *)
#define VDOOUT_EVENT_VSYNC          0x02    //param1 = device id
#define VDOOUT_EVENT_VSYNC_FIRST    0x03    //param1 = device id
#define VDOOUT_EVENT_RELEASE        0x04    //param1 = device id, param2 = (ISF_DATA *)
#define VDOOUT_EVENT_NEW            0x05    //param1 = device id, param2 = size

#define VDOOUT_FUNC_NOWAIT			0x00000001 //do not wait for flip finish

extern ISF_UNIT ISF_VdoOut1;
extern ISF_UNIT ISF_VdoOut2;

#endif //IMAGEUNIT_VDOOUT_H

