/*
    Copyright   Novatek Microelectronics Corp. 2005~2014.  All rights reserved.

    @file       ImageUnit_Demux.h
    @ingroup    mDemux

    @note       Nothing.

    @date       2014/10/01
*/

#ifndef IMAGEUNIT_DEMUX_H
#define IMAGEUNIT_DEMUX_H

/*
Support these port config API(s):

============================================================
port
------------------------------------------------------------
============================================================
*/

#include "Type.h"
#include "ImageStream.h"

enum {
	DEMUX_PARAM_START = 0x00008000,

	///< support these ISF_IN parameters
	DEMUX_PARAM_CONNECTTYPE = DEMUX_PARAM_START,//assign connect-type of INPUT port, default is ISF_CONNECT_PUSH
	DEMUX_PARAM_SYNC_FROM_OPORT,                //auto set "size. fmt, aspect" of INPUT port from "given" OUTPUT port
};

extern ISF_UNIT ISF_Demux;

#endif //IMAGEUNIT_DEMUX_H

