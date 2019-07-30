/**
    Random utility

    Random utility.

    @file       Random.h
    @ingroup    mIUtilRandom
    @note       Nothing.

    Copyright   Novatek Microelectronics Corp. 2012.  All rights reserved.
*/

#ifndef _RANDOM_H
#define _RANDOM_H

#include "Type.h"

/**
    @addtogroup mIUtilRandom
*/
//@{

extern void     randNum(UINT32 seed);
extern UINT32   randomUINT32(void);
extern void     randomPatGen(UINT32 dstBuffer, UINT32 len);

//@}

#endif  //_RANDOM_H
