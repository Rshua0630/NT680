/*
    Copyright   Novatek Microelectronics Corp. 2005~2014.  All rights reserved.

    @file       ImageUnit_BsMux.h
    @ingroup    mBsMux

    @note       Nothing.

    @date       2017/10/01
*/

#ifndef IMAGEUNIT_BSMUX_H
#define IMAGEUNIT_BSMUX_H

#include "ImageStream.h"
#include "NvtMediaInterface.h"
#include "NMediaRecBsMuxer.h"

enum {
	BSMUX_PARAM_START    = 0x0001F000,
	BSMUX_PARAM_EVENT_CB = BSMUX_PARAM_START,
	BSMUX_PARAM_GPS_DATA,
	BSMUX_PARAM_FILE_TRIGEMR,
};

typedef void (IsfBsMuxEventCb)(CHAR *Name, UINT32 event_id, UINT32 value);

extern ISF_UNIT ISF_BsMux;

#endif //IMAGEUNIT_BSMUX_H

