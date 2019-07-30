/*
    Copyright   Novatek Microelectronics Corp. 2005~2014.  All rights reserved.

    @file       ImageUnit_ImagePProc.h
    @ingroup    mImagePProc

    @note       Nothing.

    @date       2014/10/01
*/

#ifndef IMAGEUNIT_IMAGEPPROC_H
#define IMAGEUNIT_IMAGEPPROC_H

/*
For ImageUnit_ImagePProc

Support these input port(s): none

Support these output port(s): OUT1
*/


#include "Type.h"
#include "ImageStream.h"


enum {
	//all
	IMAGEPPROC_PARAM_START = 0x80000000,
	IMAGEPPROC_PARAM_RESET_ALL = 0x80000001,

	//oport
	IMAGEPPROC_PARAM_SET_OPORT_PROC_FP = 0x80001000,
};


typedef struct _ISF_IMAGE_PPORC_BUF {
	const IMG_BUF *pData;
	struct _ISF_IMAGE_PPORC_BUF *pNext;
} ISF_IMAGE_PPORC_BUF;

/**
    oport process function pointer
*/
typedef IMG_BUF(*IMAGEPPROC_OPROT_PPROC_FP)(ISF_IMAGE_PPORC_BUF *);

extern ISF_UNIT ISF_ImagePProc;

extern void ISF_ImagePProc_InstallID(void) _SECTION(".kercfg_text");

#endif //IMAGEUNIT_IMAGEPPROC_H

