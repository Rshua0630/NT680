/**
    Audio Noise Reduction library global header file

    This file is the global header file of Audio Noise Reduction library.
    This library is mainly designed for the sampling rate 32/44.1/48 KHz.
    Noise reduce quality for other sampling rates are not guaranteed.

    @file       AudNR.h
    @ingroup    mIAudNoiseReduce
    @note       Nothing.

    Copyright   Novatek Microelectronics Corp. 2012.  All rights reserved.
*/

#ifndef _AUDNR_H_
#define _AUDNR_H_

#include "Type.h"


/**
    @addtogroup mIAudNoiseReduce
*/
//@{

/**
    Audio Noise Reduction Information

    This structure is used in AudNR_Open() to return the library implementation information, such as output delay latency,
    and also the input buffer size must be multiples of the Block Size per channel.
*/
typedef struct {

	UINT32  uiOutputDelay;      ///< Current Released Lib version Output Delay latency. Unit in samples.
	UINT32  uiInputBlkSize;     ///< AudNR_ApplyNoiseReduce() buffer size must be multiples of the uiInputBlkSize samples per channel.
	///< Current version is 1024 samples per channel.

} AUDNR_INFO, *PAUDNR_INFO;

/**
    Audio Noise Reduction Sampling Rate

    This is the supporting sampling rate list for the noise reduction library and is used in AudNR_SetConfig(AUDNR_CONFIG_ID_SAMPLE_RATE)
    to assign the audio bit stream sampling rate.
*/
typedef enum {
	AUDNR_SR_32000  = 32000,    ///< Audio Bit Stream sampling rate is 32 KHz.
	AUDNR_SR_44100  = 44100,    ///< Audio Bit Stream sampling rate is 44.1 KHz.
	AUDNR_SR_48000  = 48000,    ///< Audio Bit Stream sampling rate is 48 KHz.

	ENUM_DUMMY4WORD(AUDNR_SR)
} AUDNR_SR;

/**
    Audio Noise Reduction Channel Number

    This is used in AudNR_SetConfig(AUDNR_CONFIG_ID_CHANNEL_NO) to specify the audio channel number.
*/
typedef enum {
	AUDNR_CH_MONO   = 1,    ///< MONO   Channel.
	AUDNR_CH_STEREO = 2,    ///< STEREO Channel.

	ENUM_DUMMY4WORD(AUDNR_CH)
} AUDNR_CH;


/**
    Target Machine Noise Specification

    This structure is used to input the machine noise specification information to the library.
    And these input information should be the calibration results from the noise sound.
    This is used in AudNR_SetConfig(AUDNR_CONFIG_ID_NOISE_SPEC) to assign the pointer addrss as the input paramter uiCfgValue.
*/
typedef struct {
	INT32   *piFreqBandNo;  ///< Dividing Frequency Band Number.
	INT32   *piFreqBand;    ///< Dividing Frequency Band Section Information.
	INT64   *piNoisePower;  ///< Dividing Frequency Band Noise Power Information.
	INT32   *piNoiseSpec;   ///< 1024 entries of the Machine Noise Specification.

} AUDNR_NOISE_SPEC, *PAUDNR_NOISE_SPEC;


/**
    Audio Noise Reduce Configuration ID

    This is used in AudNR_SetConfig() to assign the specified ID confuguration value.
*/
typedef enum {
	AUDNR_CONFIG_ID_SAMPLE_RATE,                ///< Assign the audio stream sampling rate.
	AUDNR_CONFIG_ID_CHANNEL_NO,                 ///< Assign the audio stream channel number.

	AUDNR_CONFIG_ID_BACKGROUND_ENABLE,          ///< Set the background noise cancellation Enable/Disable.
	AUDNR_CONFIG_ID_BACKGROUND_DECREASE_LVL,    ///< Assign the background noise cancellation strength.
	///< This value is in the format of Q15.16.
	AUDNR_CONFIG_ID_BACKGROUND_LOWER_BOUND,     ///< Assign the background noise cancellation lower bound.
	///< This value is in the format of Q15.16, and maximum value is 2<<16.

	AUDNR_CONFIG_ID_NOISE_ENABLE,               ///< Set the machine noise cancellation Enable/Disable.
	AUDNR_CONFIG_ID_NOISE_DECREASE_LVL,         ///< Assign the machine noise cancellation strength.
	///< This value is in the format of Q15.16.
	AUDNR_CONFIG_ID_NOISE_MIX_BACKGROUND,       ///< Assign the machine noise cancellation mix background noise level..
	///< This value is in the format of Q15.16, and maximum value is 1<<16.
	AUDNR_CONFIG_ID_NOISE_STOP_FREQUENCY,       ///< Machine Noise cancellation stop frequency. Unit in Hertz.
	///< This value should be larger than (sampling-rate/2). Default is 15000 Hz.
	AUDNR_CONFIG_ID_NOISE_REPLACE_FREQUENCY,    ///< The input stream frequency larger than this value would be replaced by the background noise.
	///< This value should be larger than (sampling-rate/2). Unit in Hertz. Default is 15000 Hz.
	AUDNR_CONFIG_ID_NOISE_SPEC,                 ///< This is the pointer address of the input NoiseSpec structure PAUDNR_NOISE_SPEC.

	ENUM_DUMMY4WORD(AUDNR_CONFIG_ID)
} AUDNR_CONFIG_ID;


/**
    Audio Noise Reduce Input/Output Information

    This is used in AudNR_ApplyNoiseReduce() to specify the input buffer information
    and also the machine noise reduce and background noise reduce enable/disable.
*/
typedef struct {
	UINT32  uiBufSize;                  ///< Input Buffer Size in byte count. This value must be multiples of the 1024 samples per channel.

	UINT32  uiInputAddress;             ///< Input Buffer Address.
	UINT32  uiOutputAddress;            ///< Output Buffer Address.

} AUDNR_IO_CFG, *PAUDNR_IO_CFG;



/**
    Audio Noise Reduction Coefficient Remap

    These definitions are used to remap the floating value coefficients into designed fixed point format.
*/
//@{
#define AUDNR_COEF(x)     ((INT32)((FLOAT)((FLOAT)(x))*65536.0))///< Remap floating point input coefficients into fixed point value.
//@}



//
//  Exporting APIs
//
extern BOOL AudNR_Open(PAUDNR_INFO pAudNRInfo);
extern BOOL AudNR_IsOpened(void);
extern BOOL AudNR_Close(void);
extern BOOL AudNR_Init(void);
extern BOOL AudNR_SetConfig(AUDNR_CONFIG_ID ConfigID, UINT32 uiCfgValue);
extern BOOL AudNR_ApplyNoiseReduce(PAUDNR_IO_CFG InOutCfg);

//@}

#endif
