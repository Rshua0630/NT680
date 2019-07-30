/**
    Header file of h265enc library

    Exported header file of h265enc library.

    @file       dal_h265enc.h
    @ingroup    mIAVCODEC
    @note       Nothing.

    Copyright   Novatek Microelectronics Corp. 2009.  All rights reserved.
*/
#ifndef _DAL_H265ENC_H
#define _DAL_H265ENC_H

#include <stdio.h>
#include <string.h>
#include "Type.h"
#include "kernel.h"


/**
	API
*/
ER dal_h265enc_init(DAL_VDOENC_ID id, DAL_VDOENC_INIT *pinit);
ER dal_h265enc_getinfo(DAL_VDOENC_ID id, DAL_VDOENC_GET_ITEM item, UINT32 *pvalue);
ER dal_h265enc_setinfo(DAL_VDOENC_ID id, DAL_VDOENC_SET_ITEM item, UINT32 value);
ER dal_h265enc_encodeone(DAL_VDOENC_ID id, DAL_VDOENC_PARAM *pparam);
ER dal_h265enc_close(DAL_VDOENC_ID id);

#endif
