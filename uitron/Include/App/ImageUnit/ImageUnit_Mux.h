/*
    Copyright   Novatek Microelectronics Corp. 2005~2014.  All rights reserved.

    @file       ImageUnit_Mux.h
    @ingroup    mMux

    @note       Nothing.

    @date       2014/10/01
*/

#ifndef IMAGEUNIT_MUX_H
#define IMAGEUNIT_MUX_H

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
	MUX_PARAM_START = 0x00008000,

	///< support these ISF_IN parameters
	MUX_PARAM_CONNECTTYPE = MUX_PARAM_START,//assign connect-type of INPUT port, default is ISF_CONNECT_PUSH
	MUX_PARAM_SYNC_FROM_OPORT,                //auto set "size. fmt, aspect" of INPUT port from "given" OUTPUT port
};

extern ISF_UNIT ISF_Mux;

#endif //IMAGEUNIT_MUX_H

