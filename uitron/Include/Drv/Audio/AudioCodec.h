/**
    Header file for Audio Codec module

    This file is the header file that define the API and data type
    for Audio Codec module.

    @file       AudioCodec.h
    @ingroup    mISYSAud
    @note       Nothing.

    Copyright   Novatek Microelectronics Corp. 2016.  All rights reserved.
*/

#ifndef _AUDIOCODEC_H
#define _AUDIOCODEC_H

#include "Audio.h"

/**
    @addtogroup mISYSAud
*/
//@{

#define AUDIO_CODEC_FUNC_USE_DEFAULT    (0xE0000000)

extern void     audExtCodec_getFP(PAUDIO_CODEC_FUNC pAudCodecFunc);
extern void     audExtAIC3254_getFP(PAUDIO_CODEC_FUNC pAudCodecFunc);
extern void     audExtCodecEMU_getFP(PAUDIO_CODEC_FUNC pAudCodecFunc);
extern void     audExtAC108_getFP(PAUDIO_CODEC_FUNC pAudCodecFunc);

//@}

#endif

