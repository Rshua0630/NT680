/*************************************************************************
* Name:        lz.h
* Author:      Marcus Geelnard
* Description: LZ77 coder/decoder interface.
* Reentrant:   Yes
*-------------------------------------------------------------------------
* Copyright (c) 2003-2006 Marcus Geelnard
*
* This software is provided 'as-is', without any express or implied
* warranty. In no event will the authors be held liable for any damages
* arising from the use of this software.
*
* Permission is granted to anyone to use this software for any purpose,
* including commercial applications, and to alter it and redistribute it
* freely, subject to the following restrictions:
*
* 1. The origin of this software must not be misrepresented; you must not
*    claim that you wrote the original software. If you use this software
*    in a product, an acknowledgment in the product documentation would
*    be appreciated but is not required.
*
* 2. Altered source versions must be plainly marked as such, and must not
*    be misrepresented as being the original software.
*
* 3. This notice may not be removed or altered from any source
*    distribution.
*
* Marcus Geelnard
* marcus.geelnard at home.se
*************************************************************************/
/**
    Lz Compress/UnCompress

    LZ77 coder/decoder interface.

    @file       lz.h
    @ingroup    mIUtilLzCompress
    @note       Nothing.

    Copyright   Novatek Microelectronics Corp. 2012.  All rights reserved.
*/

#ifndef _lz_h_
#define _lz_h_

#ifdef __cplusplus
extern "C" {
#endif

/**
    @addtogroup mIUtilLzCompress
*/
//@{

#if 0
/**
     Lz Compress

     Compress a block of data using an LZ77 coder.

     @param[in] in      Input (uncompressed) buffer.
     @param[in] out     Output (compressed) buffer. This buffer must be 0.4% larger
                        than the input buffer, plus one byte.
     @param[in] insize  Number of input bytes.
     @return the size of the compressed data.
*/
extern int LZ_Compress(unsigned char *in, unsigned char *out,
					   unsigned int insize);
#endif

/**
     Lz Uncompress

     Uncompress a block of data using an LZ77 decoder.

     @param[in] in      Input (compressed) buffer.
     @param[in] out     Output (uncompressed) buffer. This buffer must be large
                        enough to hold the uncompressed data.
     @param[in] insize  Number of input bytes.
     @return number of size decoded.
*/
extern unsigned int LZ_Uncompress(unsigned char *in, unsigned char *out,
								  unsigned int insize);

//@}

#ifdef __cplusplus
}
#endif

#endif /* _lz_h_ */
