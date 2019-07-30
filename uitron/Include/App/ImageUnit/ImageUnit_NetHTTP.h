/*
    Copyright   Novatek Microelectronics Corp. 2017~2026.  All rights reserved.

    @file       ImageUnit_NetHTTP.h
    @ingroup    mImagePipe

    @note       Nothing.

    @date       2017/6/01
*/

#ifndef IMAGEUNIT_NETHTTP_H
#define IMAGEUNIT_NETHTTP_H


#include "Type.h"
#include "ImageStream.h"

enum {
	NETHTTP_PARAM_START = 0x00003000,
	NETHTTP_PARAM_ENCODER = NETHTTP_PARAM_START,     ///< Set video encoder parameter
	NETHTTP_PARAM_DAEMON,
	NETHTTP_PARAM_OPEN_CALLBACK_IMM,                 ///<

};

typedef struct {
	MEM_RANGE               bitstream_mem_range;
} NETHTTP_OPEN_PARM;


typedef BOOL (*NETHTTP_OPEN_CB)(NETHTTP_OPEN_PARM *open_parm);

extern ISF_UNIT ISF_NetHTTP;

#endif //IMAGEUNIT_NETHTTP_H

