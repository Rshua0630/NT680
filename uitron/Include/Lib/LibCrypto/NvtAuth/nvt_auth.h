/**
    Nvt authentication utility

    Random utility.

    @file       nvt_auth.h
    @ingroup    mIUtilAuth
    @note       Nothing.

    Copyright   Novatek Microelectronics Corp. 2015.  All rights reserved.
*/

#ifndef _NVT_AUTH_H
#define _NVT_AUTH_H

#include "Type.h"

/**
    @addtogroup mIUtilAuth
*/
//@{

extern UINT32 Nvt_AuthGen(UINT8 *key, UINT8 *profile, UINT32 profileLen, UINT8 *auth);

//@}

#endif  //_NVT_AUTH_H
