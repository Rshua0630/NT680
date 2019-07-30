/*
    Copyright   Novatek Microelectronics Corp. 2005~2014.  All rights reserved.

    @file       ImageUnit_VdoIn.h
    @ingroup    mVdoIn

    @note       Nothing.

    @date       2014/10/01
*/

#ifndef IMAGEUNIT_VDOIN_H
#define IMAGEUNIT_VDOIN_H

/*
For ImageUnit_VdoIn

Support these input port(s): IN1

Support these output port(s): OUT1,OUT2,OUT3,OUT4,OUT5,OUT6,OUT7,OUT8,
*/


#include "Type.h"
#include "ImageStream.h"
#include "ipl_alg_infor.h" //for imagepipe parameters
#include "ipl_utility.h" //for imagepipe parameters
#include "ipl_cb_msg.h" //for imagepipe parameters

#if defined(_BSP_NA51000_)
#define VDOIN_MAX_NUM	8
#endif
#if defined(_BSP_NA51023_)
#define VDOIN_MAX_NUM	2
#endif


enum {
	VDOIN_PARAM_START = 0x80001000,
	VDOIN_PARAM_SENSORCOUNT = VDOIN_PARAM_START,
	VDOIN_PARAM_SENSORATTACH,
	VDOIN_PARAM_SENSORDETACH,
	VDOIN_PARAM_SENSORDISPLAY,
	VDOIN_PARAM_SENSORMASK,  // mask
};

#if _TODO
#define IPL_MODE_NOWAIT        0x80000000
#define IPL_MODE_DIRECT2H264   0x40000000   ///< Enable DIRECT2H264
#endif

// Sensor mask
#define SENSOR_1                        0x00000001
#if (VDOIN_MAX_NUM >= 2)
#define SENSOR_2                        0x00000002
#endif
#if (VDOIN_MAX_NUM >= 3)
#define SENSOR_3                        0x00000004
#endif
#if (VDOIN_MAX_NUM >= 4)
#define SENSOR_4                        0x00000008
#endif
#if (VDOIN_MAX_NUM >= 5)
#define SENSOR_5                        0x00000010
#endif
#if (VDOIN_MAX_NUM >= 6)
#define SENSOR_6                        0x00000020
#endif
#if (VDOIN_MAX_NUM >= 7)
#define SENSOR_7                        0x00000040
#endif
#if (VDOIN_MAX_NUM >= 8)
#define SENSOR_8                        0x00000080
#endif
#define SENSOR_ALL                      0xffffffff

extern ISF_UNIT ISF_VdoIn1;
#if (VDOIN_MAX_NUM >= 2)
extern ISF_UNIT ISF_VdoIn2;
#endif
#if (VDOIN_MAX_NUM >= 3)
extern ISF_UNIT ISF_VdoIn3;
#endif
#if (VDOIN_MAX_NUM >= 4)
extern ISF_UNIT ISF_VdoIn4;
#endif
#if (VDOIN_MAX_NUM >= 5)
extern ISF_UNIT ISF_VdoIn5;
#endif
#if (VDOIN_MAX_NUM >= 6)
extern ISF_UNIT ISF_VdoIn6;
#endif
#if (VDOIN_MAX_NUM >= 7)
extern ISF_UNIT ISF_VdoIn7;
#endif
#if (VDOIN_MAX_NUM >= 8)
extern ISF_UNIT ISF_VdoIn8;
#endif

extern ISF_UNIT *uiIsfVInMapping[VDOIN_MAX_NUM];
#define ISF_VIN(id)  uiIsfVInMapping[id]

#endif //IMAGEUNIT_VDOIN_H

