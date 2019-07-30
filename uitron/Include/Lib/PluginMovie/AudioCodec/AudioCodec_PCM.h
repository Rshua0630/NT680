/**
    Header file of audio encoding codec PCM library

    Exported header file of audio encoding codec library.

    @file       AudioCodecPCM.h
    @ingroup    mIAVCODEC
    @note       Nothing.
    @version    V1.01.003
    @date       2010/05/05

    Copyright   Novatek Microelectronics Corp. 2005.  All rights reserved.
*/
#ifndef _AUDIOCODECPCM_H
#define _AUDIOCODECPCM_H

#include "Type.h"
#include "AudioEncode.h"
#include "AudioDecode.h"

/*
    Public funtions to get audio encoding object
*/
//@{
extern PMP_AUDENC_ENCODER MP_PCMEnc_getEncodeObject(void);
extern PAUDIO_DECODER MP_PCMDec_getAudioDecodeObject(void);

//@}
#endif

//@}

