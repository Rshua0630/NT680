/*
    AIC3254 audio codec internal header
    AIC3254 audio codec internal header file

    @file       AIC3254.h
    @ingroup    mISYSAud
    @note       Nothing.

    Copyright   Novatek Microelectronics Corp. 2013.  All rights reserved.
*/
/** \addtogroup mISYSAud */
//@{

#ifndef _AIC3254_H
#define _AIC3254_H

#include "Type.h"
#include "Audio.h"
#include "AudioCodec.h"
#include "ErrorNo.h"

// 0011 0100
// --------^
// |       |
// |       Write
// AIC3254 fixed I2C device address
#define AIC3254_I2C_SADDR           (0x30)

//Temp solution for fpga 2014/03/03
#define AIC3254_RESET_GPIO          (P_GPIO_46)//(P_GPIO_53)
#define TPA2010_SHUTDOWN_GPIO       (P_GPIO_31)


static void audcodec_setDeviceObject(PAUDIO_DEVICE_OBJ pAudDevObj);
static void audcodec_init(PAUDIO_SETTING pAudio);
static void audcodec_setOutput(AUDIO_OUTPUT Output);
static void audcodec_setSamplingRate(AUDIO_SR SamplingRate);
static void audcodec_setChannel(AUDIO_CH Channel);
static void audcodec_setVolume(AUDIO_VOL Vol);
static void audcodec_setGain(AUDIO_GAIN Gain);
static BOOL audcodec_setFeature(AUDIO_FEATURE Feature, BOOL bEnable);
static void audcodec_stop(void);
static void audcodec_record(void);
static void audcodec_playback(void);
static void audcodec_setFormat(AUDIO_I2SFMT I2SFmt);
static void audcodec_setClockRatio(AUDIO_I2SCLKR I2SCLKRatio);
static BOOL audcodec_sendCommand(UINT32 uiReg, UINT32 uiData);
static BOOL audcodec_readData(UINT32 uiReg, UINT32 *puiData);
static BOOL audcodec_setParameter(AUDIO_PARAMETER Parameter, UINT32 uiSetting);
//static AUDIO_SR audcodec_getSamplingRate(void);
static BOOL audcodec_chkSamplingRate(void);

extern ER   wm8978_writeCtrlReg(UINT32 uiOffset, UINT32 uiValue);
extern ER   wm8978_readCtrlReg(UINT32 uiOffset, UINT32 *puiValue);

#endif

//@}
