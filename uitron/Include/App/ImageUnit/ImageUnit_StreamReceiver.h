/*
    Copyright   Novatek Microelectronics Corp. 2005~2014.  All rights reserved.

    @file       ImageUnit_StreamReceiver.h
    @ingroup    mImagePipe

    @note       Nothing.

    @date       2014/10/01
*/

#ifndef IMAGEUNIT_STREAMRECEIVER_H
#define IMAGEUNIT_STREAMRECEIVER_H

/*
For ImageUnit_StreamReceiver

Support these input port(s): none

Support these output port(s): OUT1
*/


#include "Type.h"
#include "ImageStream.h"
#include "StreamReceiverAPI.h"

enum {
	STMRCV_PARAM_START = 0x80008000,                     ///< a non-zero arbitrary value
	STMRCV_PARAM_CODEC,
	STMRCV_PARAM_CHS,
	STMRCV_PARAM_SAMPLERATE,
	STMRCV_PARAM_MAX_MEM_INFO,
	STMRCV_PARAM_AUD_BYPASS,
	STMRCV_PARAM_VID_INFO_CB,
};

typedef struct {
	UINT32 uiAudChannels;    //in: channels, 1 or 2
	UINT32 uiAudioSR;        //in: audio sample rate
	UINT32 uiAudBits;        //in: bits per sample, 8 or 16
	UINT32 uiAudBufUnitTime; //in: buffer number (unit: milliseconds)
	BOOL   bRelease;
} STMRCV_MAX_MEM_INFO, *PSTMRCV_MAX_MEM_INFO;

extern ISF_UNIT ISF_StreamReceiver;

extern void ISF_StreamReceiver_InstallID(void) _SECTION(".kercfg_text");

#endif //IMAGEUNIT_STREAMRECEIVER_H

