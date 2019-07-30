/**
    Audio Echo Cancellation library header file

    This file is the global header of Audio Echo Cancellation library.

    @file       Aec.h
    @ingroup    mIAudEC
    @note       Nothing.

    Copyright   Novatek Microelectronics Corp. 2015.  All rights reserved.
*/

#ifndef   _AEC_H
#define   _AEC_H

#include "Type.h"
#include "Audio.h"

/**
    @addtogroup mIAudEC
*/
//@{


/**
    Audio Echo Cancellation(AEC) Configuration Select ID

    This is used in Aec_SetConfig() as the select ID to assign new configurations
    for Audio Echo Cancellation(AEC) library.
*/
typedef enum
{
    AEC_CONFIG_ID_SAMPLERATE,       ///< Set SampleRate in Hertz. Please use AUDIO_SR as input
    AEC_CONFIG_ID_RECORD_CH_NO,     ///< Set Record Channel Number. 1 is MONO. 2 is Stereo.
    AEC_CONFIG_ID_PLAYBACK_CH_NO,   ///< Set Playback Channel Number. 1 is MONO. 2 is Stereo.

    AEC_CONFIG_ID_NOISE_CANEL_EN,   ///< Set Noise Suppression Enable/Disable. Default is ENABLE.
    AEC_CONFIG_ID_NOISE_CANCEL_LVL, ///< Defualt is -20dB. Suggest value range -3 ~ -40. Unit in dB.
    AEC_CONFIG_ID_ECHO_CANCEL_LVL,  ///< Defualt is -50dB. Suggest value range -30 ~ -60. Unit in dB.

    AEC_CONFIG_ID_NONLINEAR_PROCESS_EN,///< Set Non Linear Process Enable/Disable. Default is DISABLE.
    AEC_CONFIG_ID_AR_MODULE_EN,     ///< Set Amplitude Rate Module Enable/Disable. Default is ENABLE. DISABLE this module if farend signal is saturated.


    AEC_CONFIG_ID_RECORD_ALIGN,     ///< Add 0x00 to the record buffer to align the playback and record buffer.
                                    ///< Unit in samples. For SampleRate larger than 19.2KHz, the max value is 512 samples.
                                    ///< Otherwise(8KHz), the max value is 256 samples. Default is 0.
                                    ///< This is used during "playback start first" so we need to add delay to record buffer.
    AEC_CONFIG_ID_PLAYBACK_ALIGN,   ///< Add 0x00 to the playback buffer to align the playback and record buffer.
                                    ///< Unit in samples. For SampleRate larger than 19.2KHz, the max value is 512 samples.
                                    ///< Otherwise(8KHz), the max value is 256 samples. Default is 0.
                                    ///< This is used during "record start first" so we need to add delay to playback buffer.
    AEC_CONFIG_ID_LEAK_ESTIMTAE_EN, ///< Enable/Disable the AEC leak estimate.
    AEC_CONFIG_ID_LEAK_ESTIMTAE_VAL,///< Initial Condition of the leak estimate. Please use AEC_QCONSTANT16( 0.25 ~ 0.99) as input parameter.

	AEC_CONFIG_ID_FILTER_LEN,		///< Set AEC internal Filter Length. Set to 0 is using default value. Default is 0.
	AEC_CONFIG_ID_FRAME_SIZE,		///< Set AEC internal Frame Size. Set to 0 is using default value. Default is 0.

    AEC_CONFIG_ID_NOTCH_RADIUS,     ///< Set the Notch filter Radius. AEC_QCONSTANT16(0.992,15) is the default value. Adjustible range 0 ~ 1.
                                    ///< Please use AEC_QCONST16( 0.000 ~ 1.000,15) as input parameter.

    AEC_CONFIG_ID_PRELOAD_EN,       ///< Set Preload filter coefficient Enable/Disable. Default is DISABLE.
    AEC_CONFIG_ID_FOREADDR,         ///< Set Buffer Address for Preload FORE filter coefficient
    AEC_CONFIG_ID_BACKADDR,         ///< Set Buffer Address for Preload BACK filter coefficient
    AEC_CONFIG_ID_FORESIZE,         ///< Set Buffer Size for Preload FORE filter coefficient, return FALSE if not enough.
    AEC_CONFIG_ID_BACKSIZE,         ///< Set Buffer Size for Preload BACK filter coefficient, return FALSE if not enough.

	AEC_CONFIG_ID_BUF_ADDR,			///< AEC Lib internal buffer address.
	AEC_CONFIG_ID_BUF_SIZE,			///< AEC Lib buffer size provided by upper layer. Must be larger than Aec_GetRequireBufSize().

    AEC_CONFIG_ID_RESERVED_0,       // Default is 1. DO NOT MODIFY THIS.
    AEC_CONFIG_ID_RESERVED_1,       // Default is 1. DO NOT MODIFY THIS.
    AEC_CONFIG_ID_RESERVED_2,       // Default is 0. DO NOT MODIFY THIS.
    ENUM_DUMMY4WORD(AEC_CONFIG_ID)
}AEC_CONFIG_ID;


typedef enum
{
    AEC_BUFINFO_ID_INTERNAL,
    AEC_BUFINFO_ID_FORESIZE,
    AEC_BUFINFO_ID_BACKSIZE,
    ENUM_DUMMY4WORD(AEC_BUFINFO_ID)
}AEC_BUFINFO_ID;



/**
    Structure for Audio Echo Cancellation(AEC) input BitStream(BS) information

    Structure for audio Echo Cancellation input BitStream information, such as BS length and BS buffer address.
    The uiBsLength must be multiple of 1024 samples. This is used in Aec_Run().
*/
typedef struct
{
    UINT32      uiBsAddrPlayIn;     ///< Audio Playback buffer address for AEC input.
    UINT32      uiBsAddrRecordIn;   ///< Audio Record buffer address for AEC input.
    UINT32      uiBsAddrOut;        ///< AEC output BitStream buffer Address.

    UINT32      uiBsLength;         ///< Audio Stream Length in Samples count.
                                    ///< This value must be multiples of 1024.

}AEC_BITSTREAM,*PAEC_BITSTREAM;

typedef struct
{
    UINT32      uiInternalBufSize;  ///< AEC internal needed buffer size.
    UINT32      uiForeBufSize;      ///< AEC Preload Foreground Coefficient buffer size.
    UINT32      uiBackBufSize;     ///< AEC Preload Background Coefficient buffer size.

}AEC_BufferSizeInfo,*PAEC_BufferSizeInfo;


#define AEC_QCONSTANT16(x) ((INT32)(.5+(x)*(((INT32)1)<<(15))))


//
//  Exporting APIs
//

extern ER       Aec_Open(void);
extern BOOL     Aec_IsOpened(void);
extern ER       Aec_Close(void);
extern void     Aec_SetConfig(AEC_CONFIG_ID AecSel, INT32 iAecConfig);
extern BOOL     Aec_Init(void);
extern BOOL     Aec_Run(PAEC_BITSTREAM pAecIO);
extern UINT32   Aec_GetConfig(AEC_CONFIG_ID AecSel);
extern INT32 	Aec_GetRequireBufSize(AEC_BUFINFO_ID BufID);






/*
    AudNS Related Definitions
*/


/**
    Audio Noise Suppression(AudNS) Configuration Select ID

    This is used in Aec_SetConfig() as the select ID to assign new configurations
    for Audio Echo Cancellation(AEC) library.
*/
typedef enum
{
    AUDNS_CONFIG_ID_SAMPLERATE,       ///< Set SampleRate in Hertz. Please use AUDIO_SR as input
    AUDNS_CONFIG_ID_CHANNEL_NO,       ///< Set Record Channel Number. 1 is MONO. 2 is Stereo.

    AUDNS_CONFIG_ID_NOISE_CANCEL_LVL, ///< Defualt is -20dB. Suggest value range -3 ~ -40. Unit in dB.

	AUDNS_CONFIG_ID_BUF_ADDR,		  ///< AudNS Lib internal buffer address.
	AUDNS_CONFIG_ID_BUF_SIZE,		  ///< AudNS Lib buffer size provided by upper layer. Must be larger than AudNS_GetRequireBufSize().

    AUDNS_CONFIG_ID_RESERVED_1,       // Default is 1. DO NOT MODIFY THIS.
    AUDNS_CONFIG_ID_RESERVED_2,       // Default is 0. DO NOT MODIFY THIS.
    ENUM_DUMMY4WORD(AUDNS_CONFIG_ID)
}AUDNS_CONFIG_ID;

/**
    Structure for Audio Echo Cancellation(AEC) input BitStream(BS) information

    Structure for audio Echo Cancellation input BitStream information, such as BS length and BS buffer address.
    The uiBsLength must be multiple of 1024 samples. This is used in Aec_Run().
*/
typedef struct
{
    UINT32      uiBsAddrIn;         ///< Audio Recorded buffer address for AudNS input.
    UINT32      uiBsAddrOut;        ///< AudNS output BitStream buffer Address.

    UINT32      uiBsLength;         ///< Audio Stream Length in Samples count.
                                    ///< This value must be multiples of 1024.

}AUDNS_BITSTREAM,*PAUDNS_BITSTREAM;


//
//  Exporting APIs
//

extern ER       AudNS_Open(void);
extern BOOL     AudNS_IsOpened(void);
extern ER       AudNS_Close(void);
extern void     AudNS_SetConfig(AUDNS_CONFIG_ID AudNSSel, INT32 iAudNSConfig);
extern BOOL     AudNS_Init(void);
extern BOOL     AudNS_Run(PAUDNS_BITSTREAM pAudNSIO);
extern INT32 	AudNS_GetRequireBufSize(void);
//@}
#endif
