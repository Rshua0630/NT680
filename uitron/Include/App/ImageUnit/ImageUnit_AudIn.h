/*
    Copyright   Novatek Microelectronics Corp. 2005~2014.  All rights reserved.

    @file       ImageUnit_AudIn.h
    @ingroup    mAudIn

    @note       Nothing.

    @date       2014/10/01
*/

#ifndef IMAGEUNIT_AUDIN_H
#define IMAGEUNIT_AUDIN_H

/*
For ImageUnit_AudIn

Support these input port(s): none

Support these output port(s): OUT1
*/


#include "Type.h"
#include "ImageStream.h"
#include "WavStudioTsk.h"

enum {
	AUDIN_PARAM_START = 0x00019000,
	AUDIN_PARAM_VOL_IMM,
	AUDIN_PARAM_AEC_OBJ,
	AUDIN_PARAM_CHANNEL,
	AUDIN_PARAM_BUF_UNIT_TIME,
	AUDIN_PARAM_BUF_COUNT,
	AUDIN_PARAM_AEC_EN,
	AUDIN_PARAM_ANR_OBJ,
	AUDIN_PARAM_ANR_EN,
	AUDIN_PARAM_AEC_PLAY_CHANNEL,
	AUDIN_PARAM_MAX_MEM_INFO,
	AUDIN_PARAM_AEC_BUFSIZE,
	AUDIN_PARAM_ANR_BUFSIZE,
	AUDIN_PARAM_AUD_CODEC,
	AUDIN_PARAM_AUDFILT_OBJ,
	AUDIN_PARAM_AUDFILT_EN
};

typedef enum {
	AUDIN_AUD_CODEC_EMBEDDED,
	AUDIN_AUD_CODEC_EXTERNAL,
	ENUM_DUMMY4WORD(AUDIN_AUD_CODEC)
} AUDIN_AUD_CODEC;

typedef struct {
	ER(*Open)(INT32, INT32);
	ER(*Close)(void);
	ER(*Apply)(UINT32, UINT32);
	void(*Enable)(BOOL);
	ER(*SetBuf)(UINT32, UINT32);
} AUDIN_ANR_OBJ, *PAUDIN_ANR_OBJ;

/**
    AudFilter object
*/
typedef struct _AUDIN_AUDFILT_OBJ {
	BOOL(*Open)(UINT32 , BOOL);
	BOOL(*Close)(void);
	BOOL(*Apply)(UINT32, UINT32, UINT32);
	void(*Design)(void);
} AUDIN_AUDFILT_OBJ, *PAUDIN_AUDFILT_OBJ;

typedef struct {
	PWAVSTUD_INFO_SET pAudInfoSet;
	BOOL              bRelease;
} AUDIN_MAX_MEM_INFO, *PAUDIN_MAX_MEM_INFO;

extern ISF_UNIT ISF_AudIn;

#endif //IMAGEUNIT_AUDIN_H

