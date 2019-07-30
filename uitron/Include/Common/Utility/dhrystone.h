/**
    Dhrystone utility

    Dhrystone utility.

    @file       Dhrystone.h
    @ingroup    mIUtilDhrystone
    @note       Nothing.

    Copyright   Novatek Microelectronics Corp. 2012.  All rights reserved.
*/

#ifndef _DHRY_H
#define _DHRY_H

#include "Type.h"

/**
    @addtogroup mIUtilDhrystone
*/
//@{

extern BOOL dhry_SetGlobMemory(UINT32 DhryGlobAddress);
extern void dhrymain(int count);
/*****/

//@}

#endif  //_DHRY_H
