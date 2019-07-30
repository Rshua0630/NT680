/*
    Copyright   Novatek Microelectronics Corp. 2005~2017.  All rights reserved.

    @file       ImageUnit_VdoDec.h
    @ingroup    mVdoDec

    @note       Nothing.

    @date       2017/07/11
*/

#ifndef IMAGEUNIT_VDODEC_H
#define IMAGEUNIT_VDODEC_H

#include "ImageStream.h"
#include "NMediaPlayAPI.h"
#include "NMediaPlayVdoDec.h"

typedef enum {
	VDODEC_PARAM_START					= 0x8000A00,                     ///< a non-zero arbitrary value
	VDODEC_PARAM_CODEC                  = VDODEC_PARAM_START,
	VDODEC_PARAM_DECDESC,
	VDODEC_PARAM_WIDTH,
	VDODEC_PARAM_HEIGHT,
	VDODEC_PARAM_GOP,
	VDODEC_PARAM_MAX_MEM_INFO,
	VDODEC_PARAM_PREDECNUM,
	VDODEC_PARAM_PORT_OUTPUT_FMT,
	VDODEC_PARAM_EVENT_CB,
	VDODEC_PARAM_REFRESH_VDO,
	VDODEC_PARAM_MAX,
	ENUM_DUMMY4WORD(VDODEC_PARAM)
} VDODEC_PARAM;

typedef enum {
	VDODEC_DECODER_NONE					= 0,
	VDODEC_DECODER_MJPG					= NMEDIAPLAY_DEC_MJPG,
	VDODEC_DECODER_H264					= NMEDIAPLAY_DEC_H264,
	VDODEC_DECODER_H265					= NMEDIAPLAY_DEC_H265,
	VDODEC_DECODER_YUV					= NMEDIAPLAY_DEC_YUV,
	ENUM_DUMMY4WORD(VDODEC_DECODER)
} VDODEC_DECODER;

typedef enum {
	VDODEC_OUTPUT_NONE                  = 0,
	VDODEC_OUTPUT_UNCOMPRESSION,
	VDODEC_OUTPUT_COMPRESSION,
	ENUM_DUMMY4WORD(VDODEC_OUTPUT)
} VDODEC_OUTPUT;

typedef enum {
	ISF_VDODEC_EVENT_ONE_DISPLAYFRAME   = 0,
	ISF_VDODEC_EVENT_CUR_VDOBS,              ///< current used video bs addr & size
	ISF_VDODEC_EVENT_MAX,
	ENUM_DUMMY4WORD(ISF_VDODEC_CB_EVENT)
} ISF_VDODEC_CB_EVENT;

typedef void (IsfVdoDecEventCb)(CHAR *Name, UINT32 event_id, UINT32 value);

extern ISF_UNIT ISF_VdoDec;

extern void ISF_VdoDec_InstallID(void) _SECTION(".kercfg_text");

#endif //IMAGEUNIT_VDODEC_H

