/**
    @file       WAVLib.h
    @ingroup    mIAVWAV

    @brief      Header file for WAV file reader/writer.

                This file is the header file that define the API and data type
                for WAV file reader/writer.

    @note       Nothing.

    Copyright   Novatek Microelectronics Corp. 2005.  All rights reserved.
*/

/**
    @addtogroup mIAVWAV
*/
//@{

#ifndef _WAVLIB_H
#define _WAVLIB_H

#include "Audio.h"


/**
    WAV Codec Type
*/
typedef enum {
	AUDIOCODEC_PCM,                     ///< PCM
	AUDIOCODEC_IMA_ADPCM,               ///< IMA ADPCM
	AUDIOCODEC_G726,                    ///< G.726
	AUDIOCODEC_MP3                      ///< MP3
} AUDIO_CODEC;


/**
    WAV Header Information
*/
typedef struct _WAV_HDR_INFO {
	UINT32          uiDataSize;         ///< Data chunk size
	UINT32          uiSampleCount;      ///< Total sample count
	UINT32          uiBitsPerSample;    ///< Bits per sample
	AUDIO_CODEC     AudioCodec;         ///< What kind of codec
	AUDIO_SR        AudioSamplingRate;  ///< Sampling rate
	AUDIO_CH        AudioChannel;       ///< Audio channel
	UINT32          uiChannelCnt;
	UINT32          uiCurrFilePos;
	UINT32          usrDataAddr;
	UINT32          usrDataSize;
} WAV_HDR_INFO, *PWAV_HDR_INFO;


//=================================================
typedef enum _WAV_WRITE_FILE_IDX {
	WAV_WRITE_FILE_IDX_0,
	WAV_WRITE_FILE_IDX_1,
	WAV_WRITE_FILE_IDX_MAX
} WAV_WRITE_FILE_IDX;

//#NT#1. Support 8 bits per sample PCM format
//#NT#2. Support user data in header
//#NT#3. Optimize code to read / write data
#define WAV_DIR_FILE_AUTO               0       ///< Auto create file name based on DCF specification

/**
    @name   WAV Library return status

    Definittion for WAV Library return status
*/
//@{
#define WAV_STATUS_OK                   0       ///< Operation OK
#define WAV_STATUS_INVALID_FILE         1       ///< Invalid file (Not a RIFF type WAV format)
#define WAV_STATUS_INVALID_PARAM        2       ///< Invalid parameter
#define WAV_STATUS_WRITE_ERR            3       ///< Write file error
#define WAV_STATUS_READ_ERR             4       ///< Read file error
//@}

/**
    @name   WAV file bits per sample

    Definition for WAV file bits per sample
*/
//@{
#define WAV_BITS_PER_SAM_PCM_8          0x0008  ///< 8 bits PCM
#define WAV_BITS_PER_SAM_PCM_16         0x0010  ///< 16 bits PCM
#define WAV_BITS_PER_SAM_ADPCM          0x0004  ///< 4 bits ADPCM
//@}


/**
    Read or Write a Wav file.

*/
typedef enum _WAV_ACTION_TYPE {
	WAV_ACTION_TYPE_READ,
	WAV_ACTION_TYPE_WRITE,
	WAV_ACTION_TYPE_WRITE2
} WAV_ACTION_TYPE;


/**
    PCM Wave Header

    @note Size is 44 bytes
*/
typedef struct {
	UINT32      uiHeaderID;         ///< Wave Header
	///< @note Should be "RIFF" (0x46464952)
	UINT32      uiHeaderSize;       ///< Header Size
	///< @note = FileSize - 8 or DataChunkSize + 36 + UserDataSize
	UINT32      uiRIFFType;         ///< RIFF Type
	///< @note Should be "WAVE" (0x45564157)
	UINT32      uiFormatID;         ///< Format Chunk ID
	///< @note Should be "fmt " (0x20746D66)
	UINT32      uiFormatSize;       ///< Format Chunk Size
	///< @note Should be 0x00000010
	UINT16      uiCompressionCode;  ///< Compression Code
	///< @note Should be 0x0001 for PCM format WAV file
	UINT16      uiNumOfChannels;    ///< Number Of Channels
	///< - @b 0x0001: Mono
	///< - @b 0x0002: Stereo
	UINT32      uiSamplingRate;     ///< Sampling rate
	UINT32      uiAvgBytePerSec;    ///< Average Byte Per Second
	///< @note = Sampling Rate * Block Align
	UINT16      uiBlockAlign;       ///< Block Align
	///< @note = Significant Bits Per Sample * Number Of Channles / 8
	UINT16      uiSigBitsPerSample; ///< Significant Bits Per Sample
	///< @note Should be 0x0010 or 0x0008, PCM is 8/16 bits per sample
	UINT32      uiDataID;           ///< Data Chunk ID
	///< @note Should be "data" (0x61746164)
	UINT32      uiDataSize;         ///< Data Chunk Size
	///<
	///< Data byte count
} WAV_PCMHEADER, *PWAV_PCMHEADER;

/**
    ADPCM Wave Header

    @note Size is 60 bytes
*/
typedef struct {
	UINT32      uiHeaderID;         ///< Wave Header
	///< @note Should be "RIFF" (0x46464952)
	UINT32      uiHeaderSize;       ///< Header Size
	///< @note = FileSize - 8 or DataChunkSize + 52 + UserDataSize
	UINT32      uiRIFFType;         ///< RIFF Type
	///< @note Should be "WAVE" (0x45564157)
	UINT32      uiFormatID;         ///< Format Chunk ID
	///< @note Should be "fmt " (0x20746D66)
	UINT32      uiFormatSize;       ///< Format Chunk Size
	///< @note Should be 0x00000014
	UINT16      uiCompressionCode;  ///< Compression Code
	///< @note Should be 0x0011 for IMA ADPCM format WAV file
	UINT16      uiNumOfChannels;    ///< Number Of Channels
	///< - @b 0x0001: Mono
	///< - @b 0x0002: Stereo
	UINT32      uiSamplingRate;     ///< Sampling rate
	UINT32      uiAvgBytePerSec;    ///< Average Byte Per Second
	///< @note = Sampling Rate * Block Align / Samples per block
	UINT16      uiBlockAlign;       ///< Block Align
	///< @note = 256 * uiNumOfChannels * max(1, uiSamplingRate/11.025KHz)
	UINT16      uiSigBitsPerSample; ///< Significant Bits Per Sample
	///< @note Should be 0x0004, IMA ADPCM is 4bits per sample
	UINT16      uiExtraFormatBytes; ///< Extra Format bytes
	///< @note Should be 0x0002
	UINT16      uiExtraFormat;      ///< Extra Format Data
	///< Indicates Samples per block
	UINT32      uiFactID;           ///< Fact Chunk ID
	///< @note Should be "fact" (0x74636166)
	UINT32      uiFactSize;         ///< Fact Chunk Size
	///< @note Should be 0x00000004
	UINT32      uiSampleCount;      ///< Samples count of this file
	UINT32      uiDataID;           ///< Data Chunk ID
	///< @note Should be "data" (0x61746164)
	UINT32      uiDataSize;         ///< Data Chunk Size
	///<
	///< Data byte count
} WAV_ADPCMHEADER, *PWAV_ADPCMHEADER;


/**
Only support mono/LC/RC, 11025/12000/22050/48000, 16Bit, PCM for ASF audio record and play
*/

/**
    WAV File Information
*/
typedef struct {
	UINT32          uiDirID;            ///< Directory ID (DCF specification)
	UINT32          uiFileID;           ///< File ID (DCF specification)
	UINT32          uiDataSize;         ///< Data chunk size
	UINT32          uiSampleCount;      ///< Total sample count
	UINT32          uiBitsPerSample;    ///< Bits per sample
	AUDIO_CODEC     AudioCodec;         ///< What kind of codec
	AUDIO_SR        AudioSamplingRate;  ///< Sampling rate
	AUDIO_CH        AudioChannel;       ///< Audio channel
	UINT32          uiChannelCnt;
} WAV_FILEINFO, *PWAV_FILEINFO;

/**
    Convert 16 bits PCM data to 8 bits PCM data.

    Convert 16 bits PCM data to 8 bits PCM data.

    @param[in] uiSrcAddr    Source buffer address (16 bits PCM data, must be word alignment)
    @param[in] uiDstAddr    Destination buffer address (8 bits PCM data, must be word alignment)
    @param[in] uiSize       Size of source buffer (Must be word alignment)
    @return Size of destination buffer
*/
extern UINT32       WAV_Convert16to8(UINT32 uiSrcAddr, UINT32 uiDstAddr, UINT32 uiSize);

/**
    Convert 8 bits PCM data to 16 bits PCM data.

    Convert 8 bits PCM data to 16 bits PCM data.

    @param[in] uiSrcAddr    Source buffer address (8 bits PCM data, must be word alignment)
    @param[in] uiDstAddr    Destination buffer address (16 bits PCM data, must be word alignment)
    @param[in] uiSize       Size of source buffer (Must be word alignment)
    @return Size of destination buffer
*/
extern UINT32       WAV_Convert8to16(UINT32 uiSrcAddr, UINT32 uiDstAddr, UINT32 uiSize);

/**
    Set buffer for writing / reading WAV file header.

    Set buffer for writing / reading WAV file header.
    The buffer address can't be zero and buffer size must >= 44 Bytes.

    @note This API must be called before you read / write WAV file.

    @param[in] uiDataAddr   Buffer address
    @param[in] uiDataSize   Buffer size
    @return
        - @b WAV_STATUS_INVALID_PARAM: Buffer address = 0 or Buffer size < 44 Bytes
        - @b WAV_STATUS_OK: Set buffer for WAV file header OK.
*/
extern UINT32       WAV_SetHeaderBuffer(UINT32 uiBufferAddr, UINT32 uiBufferSize, WAV_ACTION_TYPE actType);

/**
    Set WAV file user data.

    Set WAV file user data address and size. When write WAV file header,
    these user data will write to file too. Set data address or size to
    zero will disable user data. The user data will be inserted after
    Format Chunk (PCM) or Fact Chunk (ADPCM) and before Data Chunk.

    @note User data is disabled by default and data size must be word alignment.

    @param[in] uiDataAddr   User data address
    @param[in] uiDataSize   User data size, must be word alignment
    @return
        - @b WAV_STATUS_INVALID_PARAM: User data size is not word alignment
        - @b WAV_STATUS_OK: Set WAV file user data OK.
*/
extern UINT32       WAVWriter_SetUserData(UINT32 uiDataAddr, UINT32 uiDataSize, WAV_WRITE_FILE_IDX fileIdx);

/**
    Write WAV file header

    Write WAV file header.
    If uiAddr is 0, means write to single file, otherwise write header to
    uiAddr.

    @note uiAddr will affect WAVWriter_WriteData() behavior.

    @param[in] pWAVFileInfo: WAV file information
        - @b uiDirID:       [in] Dir ID of WAV file you want to write
        - @b uiFileID       [in] File ID of WAV file you want to write
        - @b AudioCodec:    [in] What kind of codec
            - @b AUDIOCODEC_PCM
            - @b AUDIOCODEC_IMA_ADPCM
        - @b AudioSamplingRate: [in] Sampling rate
        - @b AudioChannel:  [in] Audio channel
            - @b AUDIO_CH_LEFT
            - @b AUDIO_CH_RIGHT
            - @b AUDIO_CH_STEREO
    @param[in] uiAddr       Write header to file or address
        - @b 0: Write to file
        - @b Otherwise: Write header to uiAddr
    @return Write header status.
        - @b WAV_STATUS_INVALID_PARAM: Invalid WAV file information
        - @b WAV_STATUS_OK: Write OK
*/
//extern UINT32       WAVWriter_WriteHeader(PWAV_FILEINFO pWAVFileInfo, UINT32 uiAddress, WAV_WRITE_FILE_IDX fileIdx);

/**
    Write WAV data.

    Write WAV data.

    @param uiBufAddr    The address of WAV data (PCM or ADPCM) that you want to write
    @param uiDataSize   WAV data size that you want to save
    @return Write data status
        - @b WAV_STATUS_WRITE_ERR: Write error
        - @b WAV_STATUS_OK: Write OK
*/
//extern UINT32       WAVWriter_WriteData(UINT32 uiBufAddr, UINT32 uiDataSize, WAV_WRITE_FILE_IDX fileIdx);

/**
    Update WAV file header.

    Update WAV file header and close file.
    If uiAddr is not 0, the total data size will return via pWAVFileInfo->uiDataSize.

    @param[in,out] pWAVFileInfo     WAV file information.
        - @b uiSampleCount: [in] For ADPCM, Total sample count
        - @b AudioCodec:    [in] What kind of codec
            - @b AUDIOCODEC_PCM
            - @b AUDIOCODEC_IMA_ADPCM
        - @b uiDataSize:    [out] Total data size if uiAddr is not 0
    @param[in] uiAddr
        - @b 0: Write to file
        - @b Otherwise: Update header to uiAddr
    @return Update header status.
        - @b WAV_STATUS_WRITE_ERR: Write error
        - @b WAV_STATUS_INVALID_PARAM: Not a valid WAV file information
        - @b WAV_STATUS_OK: Write OK
*/
//extern UINT32       WAVWriter_UpdateHeader(PWAV_FILEINFO pWAVFileInfo, UINT32 uiAddress, WAV_WRITE_FILE_IDX fileIdx);

/**
    Read WAV file header

    Read WAV file header and parse information.
    The WAV file information will be return to caller.

    @note uiAddr will affect WAVReader_ReadData() behavior.

    @param[in,out] pWAVFileInfo     WAV file information
        - @b uiDirID:       [in] Dir ID of WAV file you want to read
        - @b uiFileID:      [in] File ID of WAV file you want to read
        - @b uiDataSize:    [out] Total data (PCM or ADPCM) size of WAV file
        - @b uiSampleCount: [out] Total audio sample count
        - @b AudioCodec:    [out] What kind of codec
            - @b AUDIOCODEC_PCM
            - @b AUDIOCODEC_IMA_ADPCM
        - @b AudioSamplingRate: [out] Sampling rate
        - @b AudioChannel:  [out] Audio channel
            - @b AUDIO_CH_LEFT
            - @b AUDIO_CH_STEREO
        - @b uiBitsPerSample:   [out] bits per sample
    @param[in] uiAddr   Where to read the data
        - @b 0:         Read data from file
        - @b Otherwise: Read data from uiAddr
    @return
        - @b WAV_STATUS_READ_ERR:       Read error
        - @b WAV_STATUS_INVALID_FILE:   Not a valid WAV file
        - @b WAV_STATUS_OK:             Read OK
*/
extern UINT32       WAVReader_ReadHeader(PWAV_FILEINFO pWAVFileInfo, UINT32 uiAddress);

/**
    Read WAV data

    Read WAV data (PCM or ADPCM). You have to wait for FileSystem command
    finished before processing these data.

    @param[in] uiBufAddr    Buffer address that audio data will be occupied
    @param[in] uiDataSize   Audio data size that you want to read
    @return Read data status
        - @b WAV_STATUS_READ_ERR: Read error
        - @b WAV_STATUS_OK: Read OK
*/
extern UINT32       WAVReader_ReadData(UINT32 uiBufAddr, UINT32 uiDataSize, UINT32 isSync);

extern UINT32 WAV_FillPCMHdr(UINT32 hdrAddr, UINT32 hdrLen, PWAV_HDR_INFO pWavPCMHdrInfo, WAV_WRITE_FILE_IDX idx);

extern UINT32 WAV_ParseHeader(UINT32 addr, UINT32 len, PWAV_HDR_INFO pWavHdrInfo);

#endif

//@}
