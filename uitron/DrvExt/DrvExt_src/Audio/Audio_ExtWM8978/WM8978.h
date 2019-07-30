/*
    WM8978 audio codec internal header
    WM8978 audio codec internal header file

    @file       WM8978.h
    @ingroup    mISYSAud
    @note       Nothing.

    Copyright   Novatek Microelectronics Corp. 2012.  All rights reserved.
*/
/** \addtogroup mISYSAud */
//@{

#ifndef _WM8978_H
#define _WM8978_H

#include "Type.h"
#include "Audio.h"
#include "AudioCodec.h"
#include "ErrorNo.h"

// 0011 0100
// --------^
// |       |
// |       Write
// WM8974 fixed I2C device address
#define WM8978_I2C_WRITE             0x34

typedef struct {
	UINT32      uiAddr;
	UINT32      uiData;
} WM8978_REG, *PWM8978_REG;

typedef struct {
	WM8978_REG      swreset;        // 0x00     Software Reset
	WM8978_REG      pwm1;           // 0x01     Power Management 1
	WM8978_REG      pwm2;           // 0x02     Power Management 2
	WM8978_REG      pwm3;           // 0x03     Power Management 3
	WM8978_REG      audinterface;   // 0x04     Audio Interface
	WM8978_REG      compctrl;       // 0x05     Companding Control
	WM8978_REG      cgctrl;         // 0x06     Clock Gen Control
	WM8978_REG      addctrl;        // 0x07     Additional Control
	WM8978_REG      gpio;           // 0x08     GPIO Stuff
	WM8978_REG      jackdet;        // 0x09     Jack Detect Control
	WM8978_REG      dacctrl;        // 0x0A     DAC Control
	WM8978_REG      ldacdvol;       // 0x0B     Left DAC Digital Volume
	WM8978_REG      rdacdvol;       // 0x0C     Right DAC Digital Volume
	WM8978_REG      jackdetctrl;    // 0x0D     Jack Detect Control
//    WM8978_REG      dacdvol;        // 0x0B     DAC Digital Volume
	WM8978_REG      adcctrl;        // 0x0E     ADC Control
	WM8978_REG      adcdvol;        // 0x0F     ADC Digital Volume
	WM8978_REG      rightadcvol;    // 0x10     right ADC digital volume
	WM8978_REG      reserved11;     // 0x11
	WM8978_REG      eq1;            // 0x12     EQ1 - Low Shelf
	WM8978_REG      eq2;            // 0x13     EQ2 - Peak 1
	WM8978_REG      eq3;            // 0x14     EQ3 - Peak 2
	WM8978_REG      eq4;            // 0x15     EQ4 - Peak 3
	WM8978_REG      eq5;            // 0x16     EQ5 - High Shelf
	WM8978_REG      reserved17;     // 0x17
	WM8978_REG      daclim1;        // 0x18     DAC Limiter 1
	WM8978_REG      daclim2;        // 0x19     DAC Limiter 2
	WM8978_REG      reserved1A;     // 0x1A
	WM8978_REG      notchflt1;      // 0x1B     Notch Filter 1
	WM8978_REG      notchflt2;      // 0x1C     Notch Filter 2
	WM8978_REG      notchflt3;      // 0x1D     Notch Filter 3
	WM8978_REG      notchflt4;      // 0x1E     Notch Filter 4
	WM8978_REG      reserved1F;     // 0x1F
	WM8978_REG      alcctrl1;       // 0x20     ALC Control 1
	WM8978_REG      alcctrl2;       // 0x21     ALC Control 2
	WM8978_REG      alcctrl3;       // 0x22     ALC Control 3
	WM8978_REG      noisegate;      // 0x23     Noise Gate
	WM8978_REG      plln;           // 0x24     PLL N
	WM8978_REG      pllk1;          // 0x25     PLL K 1
	WM8978_REG      pllk2;          // 0x26     PLL K 2
	WM8978_REG      pllk3;          // 0x27     PLL K 3
	WM8978_REG      reserved28;     // 0x28
	WM8978_REG      threedctrl;     // 0x29     3D Control
	WM8978_REG      reserved2A;     // 0x2A
	WM8978_REG      beepctrl;       // 0x2B     Beep Control
//    WM8978_REG      attenctrl;      // 0x28     Attenuation Control
	WM8978_REG      inputctrl;      // 0x2C     Input Control
	WM8978_REG      gainctrl;       // 0x2D     INP PGA Gain Control
	WM8978_REG      rightgainctrl;  // 0x2E     right INP PGA gain control
	WM8978_REG      adcboostctrl;   // 0x2F     ADC Boost Control
	WM8978_REG      radcboostctrl;  // 0x30     right ADC boost control
	WM8978_REG      outputctrl;     // 0x31     Output Control
	WM8978_REG      lmixerctrl;     // 0x32     Left Mixer Control
	WM8978_REG      rmixerctrl;     // 0x33     Right Mixer Control
	WM8978_REG      lout1vol;       // 0x34     LOUT1(HP) Volume Control
	WM8978_REG      rout1vol;       // 0x35     ROUT1(HP) Volume Control
//    WM8978_REG      spkmixerctrl;   // 0x32     Speaker Mixer Control
	WM8978_REG      lout2vol;       // 0x36     LOUT2(SPK) Volume Control
	WM8978_REG      rout2vol;       // 0x37     ROUT2(SPK) Volume Control
//    WM8978_REG      spkvolumectrl;  // 0x36     Speaker Volume Control
	WM8978_REG      out3mixer;      // 0x38     OUT3 Mixer Control
	WM8978_REG      out4mixer;      // 0x39     OUT4 (MONO) Mixer Control
//    WM8978_REG      monomixerctrl;  // 0x38     MONO Mixer Control
} WM8978_REG_SET;

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
//static AUDIO_SR audcodec_getSamplingRate(void);
static BOOL audcodec_chkSamplingRate(void);

extern ER   wm8978_writeCtrlReg(UINT32 uiOffset, UINT32 uiValue);
extern ER   wm8978_readCtrlReg(UINT32 uiOffset, UINT32 *puiValue);

#endif

//@}
