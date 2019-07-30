/**
    Header file of ADPCM Encode/Decode Library

    This file is the Header file of IMA ADPCM Encode/Decode Library.

    @file       ADPCM.h
    @ingroup    mIAVADPCM
    @note       Nothing.

    Copyright   Novatek Microelectronics Corp. 2012.  All rights reserved.
*/

#ifndef _ADPCM_H
#define _ADPCM_H

#include "Type.h"

/**
    @addtogroup mIAVADPCM
*/
//@{

/**
    @name   Full IMA ADPCM packet sample count

    Definition for Full IMA ADPCM packet sample count
*/
//@{
#define ADPCM_PACKET_SAMPLES_8K         0x01F9      ///< One packet sample count of 8K sampling rate
#define ADPCM_PACKET_SAMPLES_11K        0x01F9      ///< One packet sample count of 11K sampling rate
#define ADPCM_PACKET_SAMPLES_22K        0x03F9      ///< One packet sample count of 22K sampling rate
#define ADPCM_PACKET_SAMPLES_44K        0x07F9      ///< One packet sample count of 44K sampling rate
//@}

/**
    @name   Full IMA ADPCM packet PCM data size

    Definition for Full IMA ADPCM packet PCM data size
*/
//@{
#define ADPCM_PACKET_PCMSIZE_8K_MONO    (ADPCM_PACKET_SAMPLES_8K << 1)      ///< PCM data size of one ADPCM packet for 8K sampling rate mono
#define ADPCM_PACKET_PCMSIZE_8K_STEREO  (ADPCM_PACKET_SAMPLES_8K << 2)      ///< PCM data size of one ADPCM packet for 8K sampling rate stereo
#define ADPCM_PACKET_PCMSIZE_11K_MONO   (ADPCM_PACKET_SAMPLES_11K << 1)     ///< PCM data size of one ADPCM packet for 11K sampling rate mono
#define ADPCM_PACKET_PCMSIZE_11K_STEREO (ADPCM_PACKET_SAMPLES_11K << 2)     ///< PCM data size of one ADPCM packet for 11K sampling rate stereo
#define ADPCM_PACKET_PCMSIZE_22K_MONO   (ADPCM_PACKET_SAMPLES_22K << 1)     ///< PCM data size of one ADPCM packet for 22K sampling rate mono
#define ADPCM_PACKET_PCMSIZE_22K_STEREO (ADPCM_PACKET_SAMPLES_22K << 2)     ///< PCM data size of one ADPCM packet for 22K sampling rate stereo
#define ADPCM_PACKET_PCMSIZE_44K_MONO   (ADPCM_PACKET_SAMPLES_44K << 1)     ///< PCM data size of one ADPCM packet for 44K sampling rate mono
#define ADPCM_PACKET_PCMSIZE_44K_STEREO (ADPCM_PACKET_SAMPLES_44K << 2)     ///< PCM data size of one ADPCM packet for 44K sampling rate stereo
//@}

/**
    @name   Full IMA ADPCM packet align size

    Definition for Full IMA ADPCM packet align size
*/
//@{
#define ADPCM_PACKET_ALIGN_8K_MONO      0x0100          ///< Packet align size for 8K sampling rate mono
#define ADPCM_PACKET_ALIGN_8K_STEREO    0x0200          ///< Packet align size for 8K sampling rate stereo
#define ADPCM_PACKET_ALIGN_11K_MONO     0x0100          ///< Packet align size for 11K sampling rate mono
#define ADPCM_PACKET_ALIGN_11K_STEREO   0x0200          ///< Packet align size for 11K sampling rate stereo
#define ADPCM_PACKET_ALIGN_22K_MONO     0x0200          ///< Packet align size for 22K sampling rate mono
#define ADPCM_PACKET_ALIGN_22K_STEREO   0x0400          ///< Packet align size for 22K sampling rate stereo
#define ADPCM_PACKET_ALIGN_44K_MONO     0x0400          ///< Packet align size for 44K sampling rate mono
#define ADPCM_PACKET_ALIGN_44K_STEREO   0x0800          ///< Packet align size for 44K sampling rate stereo
//@}

/**
    @name   IMA ADPCM average bytes per second

    Definition for IMA ADPCM average bytes per second
*/
//@{
#define ADPCM_AVG_BPS_8K_MONO           0x00000FD7      ///< 8K sampling rate mono average bytes per second
#define ADPCM_AVG_BPS_8K_STEREO         0x00001FAE      ///< 8K sampling rate stereo average bytes per second
#define ADPCM_AVG_BPS_11K_MONO          0x000015D4      ///< 11K sampling rate mono average bytes per second
#define ADPCM_AVG_BPS_11K_STEREO        0x00002BA9      ///< 11K sampling rate stereo average bytes per second
#define ADPCM_AVG_BPS_22K_MONO          0x00002B5C      ///< 22K sampling rate mono average bytes per second
#define ADPCM_AVG_BPS_22K_STEREO        0x000056B9      ///< 22K sampling rate stereo average bytes per second
#define ADPCM_AVG_BPS_44K_MONO          0x0000566D      ///< 44K sampling rate mono average bytes per second
#define ADPCM_AVG_BPS_44K_STEREO        0x0000ACDB      ///< 44K sampling rate stereo average bytes per second
//@}

/**
    ADPCM State
*/
typedef struct {
	INT16   LValPrev;   ///< Previous left channel value
	INT8    LIndex;     ///< Left channel index
	INT16   RValPrev;   ///< Previous right channel value
	INT8    RIndex;     ///< Right channel index
} ADPCM_STATE, *PADPCM_STATE;

// Public APIs
extern UINT32   Adpcm_EncodeMono(INT16 *pInData, INT8 *pOutData, UINT32 uiSampleCount, PADPCM_STATE pState);
extern UINT32   Adpcm_EncodeStereo(INT16 *pInData, INT8 *pOutData, UINT32 uiSampleCount, PADPCM_STATE pState);

extern UINT32   Adpcm_DecodeMono(INT8 *pInData, INT16 *pOutData, UINT32 uiSampleCount, PADPCM_STATE pState);
extern UINT32   Adpcm_DecodeStereo(INT8 *pInData, INT16 *pOutData, UINT32 uiSampleCount, PADPCM_STATE pState);

extern UINT32   Adpcm_EncodePacketMono(INT16 *pInData, INT8 *pOutData, UINT32 uiSampleCount, PADPCM_STATE pState);
extern UINT32   Adpcm_EncodePacketStereo(INT16 *pInData, INT8 *pOutData, UINT32 uiSampleCount, PADPCM_STATE pState);

extern UINT32   Adpcm_DecodePacketMono(INT8 *pInData, INT16 *pOutData, UINT32 uiSampleCount);
extern UINT32   Adpcm_DecodePacketStereo(INT8 *pInData, INT16 *pOutData, UINT32 uiSampleCount);

//@}

// Backward compatible, don't reference any more.
#define adpcm_encodeMono            Adpcm_EncodeMono
#define adpcm_encodeStereo          Adpcm_EncodeStereo
#define adpcm_decodeMono            Adpcm_DecodeMono
#define adpcm_decodeStereo          Adpcm_DecodeStereo
#define adpcm_encodePacketMono      Adpcm_EncodePacketMono
#define adpcm_encodePacketStereo    Adpcm_EncodePacketStereo
#define adpcm_decodePacketMono      Adpcm_DecodePacketMono
#define adpcm_decodePacketStereo    Adpcm_DecodePacketStereo

#endif
