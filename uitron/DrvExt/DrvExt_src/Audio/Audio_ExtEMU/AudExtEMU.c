/*
    Emaltion External audio codec driver

    This file is the driver for Emaltion extended audio codec.

    @file       AudExtEMU.c
    @ingroup    mISYSAud
    @note       Nothing.

    Copyright   Novatek Microelectronics Corp. 2016.  All rights reserved.
*/

/** \addtogroup mISYSAud */
//@{

#include "AudExtEMU.h"
#include "pll.h"
#include "sif.h"
#include "i2c.h"
#include "gpio.h"
#include "top.h"
#include "Utility.h"
#include "dai.h"
#include "string.h"
#include "KerCPU.h"
#include "Debug.h"


static AUDIO_CODEC_FUNC     EXTEMUCodecFP    = {
	AUDIO_CODEC_TYPE_EXTERNAL,

	// Set Device Object
	audcodec_setDeviceObject,
	// Init
	audcodec_init,
	// Set Record Source
	audcodec_setRecordSource,
	// Set Output
	audcodec_setOutput,
	// Set Sampling rate
	audcodec_setSamplingRate,
	// Set TxChannel
	audcodec_setChannel,
	// Set RxChannel
	audcodec_setChannel,
	// Set Volume (Playback)
	audcodec_setVolume,
	// Set Gain (Record)
	audcodec_setGain,
	// Set GainDB (Record)
	NULL,
	// Set Effect
	audcodec_setEffect,
	// Set Feature
	audcodec_setFeature,
	// StopRecord
	audcodec_stopRecord,
	// StopPlayback
	audcodec_stopPlayback,
	// Record Preset
	NULL,
	// Record
	audcodec_record,
	// Playback
	audcodec_playback,
	// Set I2S Format
	audcodec_setFormat,
	// Set I2S clock ratio
	audcodec_setClockRatio,
	// Send command
	audcodec_sendCommand,
	// Read data
	audcodec_readData,
	// Set Parameter
	audcodec_setParameter,
	// Check Sampling rate
	audcodec_chkSamplingRate,
	// Open
	NULL,
	// Close
	NULL,
};

void audExtCodecEMU_getFP(PAUDIO_CODEC_FUNC pAudCodecFunc)
{
	memcpy((void *)pAudCodecFunc, (const void *)&EXTEMUCodecFP, sizeof(AUDIO_CODEC_FUNC));
}
static void audcodec_setDeviceObject(PAUDIO_DEVICE_OBJ pAudDevObj)
{}
static void audcodec_init(PAUDIO_SETTING pAudio)
{}
static void audcodec_setRecordSource(AUDIO_RECSRC RecSrc)
{}
static void audcodec_setOutput(AUDIO_OUTPUT Output)
{}
static void audcodec_setSamplingRate(AUDIO_SR SamplingRate)
{}
static void audcodec_setChannel(AUDIO_CH Channel)
{}
static void audcodec_setVolume(AUDIO_VOL Vol)
{}
static void audcodec_setGain(AUDIO_GAIN Gain)
{}
static void audcodec_setEffect(AUDIO_EFFECT Effect)
{}
static BOOL audcodec_setFeature(AUDIO_FEATURE Feature, BOOL bEnable)
{
	return TRUE;
}
static void audcodec_stopPlayback(void)
{}
static void audcodec_stopRecord(void)
{}
static void audcodec_record(void)
{}
static void audcodec_playback(void)
{}
static void audcodec_setFormat(AUDIO_I2SFMT I2SFmt)
{}
static void audcodec_setClockRatio(AUDIO_I2SCLKR I2SCLKRatio)
{}
static BOOL audcodec_sendCommand(UINT32 uiReg, UINT32 uiData)
{
	return TRUE;
}
static BOOL audcodec_readData(UINT32 uiReg, UINT32 *puiData)
{
	return TRUE;
}
static BOOL audcodec_setParameter(AUDIO_PARAMETER Parameter, UINT32 uiSetting)
{
	return TRUE;
}
static BOOL audcodec_chkSamplingRate(void)
{
	return TRUE;
}



//@}
