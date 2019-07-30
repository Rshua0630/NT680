/**
    Header file of audio codec uLaw library

    Exported header file of audio encoding codec library.

    @file       AudioCodec_uLaw.h
    @ingroup    mIAVCODEC
    @note       Nothing.
    @version    V1.00.000
    @date       2015/08/17

    Copyright   Novatek Microelectronics Corp. 2015.  All rights reserved.
*/
#ifndef _AUDIOCODECULAW_H
#define _AUDIOCODECULAW_H

#include "Type.h"
#include "AudioEncode.h"
#include "AudioDecode.h"

/*
    Public funtions to get audio codec object
*/
//@{
extern PMP_AUDENC_ENCODER MP_uLawEnc_getEncodeObject(void);
extern PAUDIO_DECODER MP_uLawDec_getDecodeObject(void);
//@}

#endif

//@}


