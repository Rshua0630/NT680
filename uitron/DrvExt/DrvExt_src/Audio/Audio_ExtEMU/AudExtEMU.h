/*
    EMU audio codec internal header
    EMU audio codec internal header file

    @file       AudExtEMU.h
    @ingroup    mISYSAud
    @note       Nothing.

    Copyright   Novatek Microelectronics Corp. 2016.  All rights reserved.
*/
/** \addtogroup mISYSAud */
//@{

#ifndef _AUDEXTEMU_H
#define _AUDEXTEMU_H

#include "Type.h"
#include "Audio.h"
#include "AudioCodec.h"
#include "ErrorNo.h"


static void audcodec_setDeviceObject(PAUDIO_DEVICE_OBJ pAudDevObj);
static void audcodec_init(PAUDIO_SETTING pAudio);
static void audcodec_setRecordSource(AUDIO_RECSRC RecSrc);
static void audcodec_setOutput(AUDIO_OUTPUT Output);
static void audcodec_setSamplingRate(AUDIO_SR SamplingRate);
static void audcodec_setChannel(AUDIO_CH Channel);
static void audcodec_setVolume(AUDIO_VOL Vol);
static void audcodec_setGain(AUDIO_GAIN Gain);
static void audcodec_setEffect(AUDIO_EFFECT Effect);
static BOOL audcodec_setFeature(AUDIO_FEATURE Feature, BOOL bEnable);
static void audcodec_stopRecord(void);
static void audcodec_stopPlayback(void);
static void audcodec_record(void);
static void audcodec_playback(void);
static void audcodec_setFormat(AUDIO_I2SFMT I2SFmt);
static void audcodec_setClockRatio(AUDIO_I2SCLKR I2SCLKRatio);
static BOOL audcodec_sendCommand(UINT32 uiReg, UINT32 uiData);
static BOOL audcodec_readData(UINT32 uiReg, UINT32 *puiData);
static BOOL audcodec_setParameter(AUDIO_PARAMETER Parameter, UINT32 uiSetting);
static BOOL audcodec_chkSamplingRate(void);


#endif

//@}
