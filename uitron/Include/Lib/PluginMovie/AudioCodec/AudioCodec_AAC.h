/**
    Header file of audio codec AAC library

    Exported header file of audio encoding codec library.

    @file       AudioCodecAAC.h
    @ingroup    mIAVCODEC
    @note       Nothing.
    @version    V1.01.003
    @date       2010/05/05

    Copyright   Novatek Microelectronics Corp. 2005.  All rights reserved.
*/
#ifndef _AUDIOCODECAAC_H
#define _AUDIOCODECAAC_H

#include "Type.h"
#include "AudioEncode.h"
#include "AudioDecode.h"

/*
    Public funtions to get audio codec object
*/
//@{
extern PMP_AUDENC_ENCODER MP_AACEnc_getEncodeObject(void);
extern PMP_AUDENC_ENCODER MP_AACEnc_getNoAlignEncodeObject(void);//2012/02/23 Meg
extern PAUDIO_DECODER MP_AACDec_getAudioDecodeObject(void);
extern ER MP_AACDec_parseADTS(UINT32 bsAddr, UINT32 *pHeader_length, UINT32 *pStream_length);


//@}
#endif

//@}


