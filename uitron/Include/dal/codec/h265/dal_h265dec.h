/**
    Header file of h265dec library

    Exported header file of h265dec library.

    @file       dal_h265dec.h
    @ingroup    mIAVCODEC
    @note       Nothing.

    Copyright   Novatek Microelectronics Corp. 2009.  All rights reserved.
*/
#ifndef _DAL_H265DEC_H
#define _DAL_H265DEC_H

#include <stdio.h>
#include <string.h>
#include "Type.h"
#include "kernel.h"

#define DAL_H265DEC_DRIVER_BUF_USEDSIZE		0x6E0000 // 6.85 Mbytes for 1536x1536 Maximum size
#define DAL_H265DEC_GOP_IPB_1ST15			3 // according to H265DEC_GOP_IPB_1ST15
#define DAL_H265DEC_GOP_15					15 // according to H265DEC_GOP_15
#define DAL_H265DEC_PRE_DECODE_FREAME_NUM	1 // for the Pre-decode Frame Number (2017/08/02 Adam Su)
#define DAL_H265DEC_GOP_IPONLY				1 // decode GOP type: I, P only

/**
	API
*/
ER dal_h265dec_init(DAL_VDODEC_ID id, DAL_VDODEC_INIT *pinit);
ER dal_h265dec_getinfo(DAL_VDODEC_ID id, DAL_VDODEC_GET_ITEM item, UINT32 *pvalue);
ER dal_h265dec_setinfo(DAL_VDODEC_ID id, DAL_VDODEC_SET_ITEM item, UINT32 value);
ER dal_h265dec_decodeone(DAL_VDODEC_ID id, DAL_VDODEC_PARAM *pparam);
ER dal_h265dec_close(DAL_VDODEC_ID id);

#endif
