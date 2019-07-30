/**
    Header file of jpegdec library

    Exported header file of jpegdec library.

    @file       dal_jpegdec.h
    @ingroup    mIAVCODEC
    @note       Nothing.

    Copyright   Novatek Microelectronics Corp. 2009.  All rights reserved.
*/
#ifndef _DAL_JPEGDEC_H
#define _DAL_JPEGDEC_H

#include <stdio.h>
#include <string.h>
#include "Type.h"
#include "kernel.h"
#include "dal_vdodec.h"


/**
	API
*/
ER dal_jpegdec_init(DAL_VDODEC_ID id, DAL_VDODEC_INIT *pinit);
ER dal_jpegdec_getinfo(DAL_VDODEC_ID id, DAL_VDODEC_GET_ITEM item, UINT32 *pvalue);
ER dal_jpegdec_setinfo(DAL_VDODEC_ID id, DAL_VDODEC_SET_ITEM item, UINT32 value);
ER dal_jpegdec_decodeone(DAL_VDODEC_ID id, DAL_VDODEC_PARAM *pparam);
ER dal_jpegdec_close(DAL_VDODEC_ID id);

#endif
