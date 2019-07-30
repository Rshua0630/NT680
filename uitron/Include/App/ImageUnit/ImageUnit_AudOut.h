/*
    Copyright   Novatek Microelectronics Corp. 2005~2014.  All rights reserved.

    @file       ImageUnit_AudOut.h
    @ingroup    mAudOut

    @note       Nothing.

    @date       2014/10/01
*/

#ifndef IMAGEUNIT_AUDOUT_H
#define IMAGEUNIT_AUDOUT_H

/*
Support these port config API(s):

============================================================
port                        IN1,IN2
------------------------------------------------------------
============================================================
*/

#include "Type.h"
#include "ImageStream.h"
#include "WavStudioTsk.h"

enum {
	AUDOUT_PARAM_START = 0x00018000,
	AUDOUT_PARAM_VOL_IMM,
	AUDOUT_PARAM_OUT_DEV,
	AUDOUT_PARAM_CHANNEL,
	AUDOUT_PARAM_BUF_UNIT_TIME,
	AUDOUT_PARAM_BUF_COUNT,
	AUDOUT_PARAM_MAX_MEM_INFO,
	AUDOUT_PARAM_EN_EVT_CB,
};

typedef enum _AUDOUT_EVT {
	AUDOUT_EVT_TCHIT = 0,           ///< Time code hit
	AUDOUT_EVT_STARTED,             ///< Started
	AUDOUT_EVT_STOPPED,             ///< Stopped
	AUDOUT_EVT_PAUSED,              ///< Paused
	AUDOUT_EVT_RESUMED,             ///< Resumed
	AUDOUT_EVT_BUF_FULL,            ///< Buffer full
	AUDOUT_EVT_BUF_EMPTY,           ///< Buffer empty
	AUDOUT_EVT_BUF_DONE,            ///< Buffer done
	AUDOUT_EVT_PRELOAD_DONE,        ///< Preload done
	AUDOUT_EVT_MAX
} AUDOUT_EVT;

typedef struct {
	PWAVSTUD_INFO_SET pAudInfoSet;
	BOOL              bRelease;
} AUDOUT_MAX_MEM_INFO, *PAUDOUT_MAX_MEM_INFO;

extern ISF_UNIT ISF_AudOut;

#endif //IMAGEUNIT_AUDOUT_H

