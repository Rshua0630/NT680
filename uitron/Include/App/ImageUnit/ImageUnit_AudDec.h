/*
    Copyright   Novatek Microelectronics Corp. 2005~2017.  All rights reserved.

    @file       ImageUnit_AudDec.h
    @ingroup    mAudDec

    @note       Nothing.

    @date       2017/07/11
*/

#ifndef IMAGEUNIT_AUDDEC_H
#define IMAGEUNIT_AUDDEC_H

#include "ImageStream.h"
#include "NMediaPlayAPI.h"
#include "NMediaPlayAudDec.h"

typedef enum {
	AUDDEC_PARAM_START					= 0x8000A00,                     ///< a non-zero arbitrary value
	AUDDEC_PARAM_CODEC				= AUDDEC_PARAM_START,
	AUDDEC_PARAM_SAMPLERATE,
	AUDDEC_PARAM_CHANNELS,
	AUDDEC_PARAM_MAX_MEM_INFO,
	AUDDEC_PARAM_PREDECNUM,
	AUDDEC_PARAM_FILEPLAY_MODE,
	AUDDEC_PARAM_MAX,
	ENUM_DUMMY4WORD(AUDDEC_PARAM)
} AUDDEC_PARAM;

typedef enum {
	AUDDEC_DECODER_NONE					= 0,
	AUDDEC_DECODER_PCM					= NMEDIAPLAY_DEC_PCM,  // value = 1
	AUDDEC_DECODER_AAC                  = NMEDIAPLAY_DEC_AAC,  // value = 2
	AUDDEC_DECODER_PPCM					= NMEDIAPLAY_DEC_PPCM, // value = 3
	AUDDEC_DECODER_ULAW					= NMEDIAPLAY_DEC_ULAW, // value = 4
	AUDDEC_DECODER_ALAW					= NMEDIAPLAY_DEC_ALAW, // value = 5
	ENUM_DUMMY4WORD(AUDDEC_DECODER)
} AUDDEC_DECODER;

typedef enum {
	AUDDEC_OUTPUT_NONE                  = 0,
	AUDDEC_OUTPUT_UNEXTRACT,
	AUDDEC_OUTPUT_EXTRACT,
	ENUM_DUMMY4WORD(AUDDEC_OUTPUT)
} AUDDEC_OUTPUT;

extern ISF_UNIT ISF_AudDec;

extern void ISF_AudDec_InstallID(void) _SECTION(".kercfg_text");

#endif //IMAGEUNIT_AUDDEC_H

