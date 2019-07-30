/**
    Header file of jpegenc library

    Exported header file of jpegenc library.

    @file       dal_jpegenc.h
    @ingroup    mIAVCODEC
    @note       Nothing.

    Copyright   Novatek Microelectronics Corp. 2009.  All rights reserved.
*/
#ifndef _DAL_JPEGENC_H
#define _DAL_JPEGENC_H

#include <stdio.h>
#include <string.h>
#include "Type.h"
#include "kernel.h"
#include "dal_vdoenc.h"


/**
	API
*/
ER dal_jpegenc_init(DAL_VDOENC_ID id, DAL_VDOENC_INIT *pinit);
ER dal_jpegenc_getinfo(DAL_VDOENC_ID id, DAL_VDOENC_GET_ITEM item, UINT32 *pvalue);
ER dal_jpegenc_setinfo(DAL_VDOENC_ID id, DAL_VDOENC_SET_ITEM item, UINT32 value);
ER dal_jpegenc_encodeone(DAL_VDOENC_ID id, DAL_VDOENC_PARAM *pparam);
ER dal_jpegenc_close(DAL_VDOENC_ID id);

#endif
