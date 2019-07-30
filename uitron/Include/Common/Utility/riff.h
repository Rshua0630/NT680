/**
    RIFF chunk

    @file       riff.h
    @ingroup    mIUtilRiff
    @note       Nothing.

    Copyright   Novatek Microelectronics Corp. 2017.  All rights reserved.
*/

#ifndef _RIFF_H
#define _RIFF_H

#include "Type.h"

/**
    @addtogroup mIUtilRiff
*/
//@{

/**
     riff chunk. (addr must align to 1 word = 4 bytes)
*/
typedef struct _RIFF_CHUNK {
	UINT32              fourcc;                     ///< MAKEFOURCC('?','?','?','?')
	UINT32              size;                      ///< chunk size = 8 + body
} RIFF_CHUNK, *PRIFF_CHUNK; ///< size = 2 words = 8 bytes

//@}

#endif //_RIFF_H

