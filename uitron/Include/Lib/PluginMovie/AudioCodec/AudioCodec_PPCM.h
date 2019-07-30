/**
    Header file of audio codec PPCM library

    Exported header file of audio encoding codec library.

    @file       AudioCodecPPCM.h
    @ingroup    mIAVCODEC
    @note       Nothing.
    @version    V1.01.001
    @date       2014/09/24

    Copyright   Novatek Microelectronics Corp. 2014.  All rights reserved.
*/
#ifndef _AUDIOCODECPPCM_H
#define _AUDIOCODECPPCM_H

#include "Type.h"
#include "AudioEncode.h"
#include "AudioDecode.h"

/*
    Public funtions to get audio codec object
*/
//@{
extern PMP_AUDENC_ENCODER MP_PPCMEnc_getEncodeObject(void);
extern PMP_AUDENC_ENCODER MP_PPCMEnc_getNoAlignEncodeObject(void);
extern PAUDIO_DECODER MP_PPCMDec_getAudioDecodeObject(void);


//@}
#endif

//@}


