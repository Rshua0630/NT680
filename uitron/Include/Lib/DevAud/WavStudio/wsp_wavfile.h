/**
    WavFile module.

    @file       wsp_wavfile.h
    @ingroup    mILIBWAVFILE

    @brief      Header file of WavFile.

                APIs/structure/macro declaration of WavFile.

    @note       Nothing.

    Copyright   Novatek Microelectronics Corp. 2015.  All rights reserved.

*/

/**
    @addtogroup mILIBWAVFILE
*/
//@{

#include "Type.h"
#include "FileSysTsk.h"
#include "Audio.h"
#include "WAVLib.h"


#define WAVFILE_HEADER_BUF_SIZE         0x00000200  ///< 512 Bytes

/**
    Wave file event IDs
*/
typedef enum _WAVFILE_CB_EVT_ {
	WAVFILE_CB_EVT_OK = 0,                  ///< OK
	WAVFILE_CB_EVT_FAIL,                    ///< Fail
	WAVFILE_CB_EVT_DISK_FULL,               ///< Disk full
	WAVFILE_CB_EVT_READ_FAIL,               ///< Read file fail
	WAVFILE_CB_EVT_WRITE_FAIL,              ///< Write file fail
	WAVFILE_CB_EVT_INVALID_FILE,            ///< Invalid file
	WAVFILE_CB_EVT_PARSE_HEADER_FAIL,       ///< Not valid wav file
	WAVFILE_CB_EVT_READ_LAST,               ///< File read last
	WAVFILE_CB_EVT_MAX
} WAVFILE_CB_EVT;

/**
    Wave file configurations
*/
typedef enum _WAVFILE_CFG {
	WAVFILE_CFG_GET_DURATION,               ///< Remain time of the disk for recording
	WAVFILE_CFG_WAV_PLAY_TIME,              ///< Play time of the file (second)
	WAVFILE_CFG_MAX
} WAVFILE_CFG;

/**
    Wave studio file information
*/
typedef struct _WAVFILE_FILENAME {
	unsigned char   *pFileName;             ///< point to a character array of long file name, 128 byte maximum
	UINT32          fileNameLen;            ///< byte size of the string
} WAVFILE_FILENAME, *PWAVFILE_FILENAME;

/**
    Wave file audio info
*/
typedef struct _WAVFILE_INFO {
	AUDIO_SR            audSR;              ///< Sample rate
	AUDIO_CH            audCh;              ///< Channel
	UINT32              bitPerSamp;         ///< bit per sample
	AUDIO_CODEC         codec;              ///< Codec (e.g. PCM)
} WAVFILE_AUD_INFO, *PWAVFILE_AUD_INFO;

/**
    Bit per sample
*/
typedef enum _WAVFILE_BITS_PER_SAM {
	WAVFILE_BITS_PER_SAM_8  = 0x08,         ///< 8 bits PCM
	WAVFILE_BITS_PER_SAM_16 = 0x10,         ///< 16 bits PCM
} WAVFILE_BITS_PER_SAM;

/**
    Set Wav file play configuration.

    Set Wav file play configuration.

    @param[in]  cfg             Configuration type.
    @param[in]  p1              parameter 1. (According to the confiuration type)
    @param[in]  p2              parameter 2. (According to the confiuration type)
    @return
        - @b E_OK: No error.
        - @b E_PAR: Parameter error.
*/
UINT32 WavFilePlay_SetConfig(UINT32 cfg, UINT32 p1, UINT32 p2);

/**
    Get Wav file play configuration.

    Get Wav file play configuration.

    @param[in]  cfg             Configuration type.
    @param[in]  p1              parameter 1. (According to the confiuration type)
    @param[in]  p2              parameter 2. (According to the confiuration type)
    @return     Configuration value.
*/
UINT32 WavFilePlay_GetConfig(UINT32 cfg, UINT32 p1, UINT32 p2);

/**
    Set file name.

    Set file name.

    @param[in]  fileName         Wav file name.
    @return
        - @b TRUE: Stop recording / playing.
        - @b FALSE: WAV studio task doesn't recording or playing.
*/
BOOL WavFile_LoadOpen(PWAVFILE_FILENAME fileName);

/**
    Close wav file.

    Close wav file.

    @return
        - @b TRUE: No error.
        - @b FALSE: Close file fail.
*/
BOOL WavFile_LoadClose(void);

/**
    Parse wav file header.

    Parse wav file header.

    @param[in]  info            Audio info configurations.
    @param[in]  stopCount       Stop sample count.
    @param[in]  hdrMem          Buffer info of wav header.
    @return
        - @b TRUE: No error.
        - @b FALSE: File error.
*/
BOOL WavFile_LoadHeader(PWAVFILE_AUD_INFO info, UINT64 *stopCount, MEM_RANGE hdrMem);

/**
    Read sound data to buffer.

    Read sound data to buffer.

    @param[in]  pAudBufQue      Buffer to be filled.
    @param[in]  id              Act id.
    @return
        - @b TRUE: No error.
        - @b FALSE: File error.
*/
BOOL WavFile_LoadCB(PAUDIO_BUF_QUEUE pAudBufQue, UINT32 id, UINT64 timestamp);

/**
    Set Wav file record configuration.

    Set Wav file record configuration.

    @param[in]  cfg             Configuration type.
    @param[in]  p1              parameter 1. (According to the confiuration type)
    @param[in]  p2              parameter 2. (According to the confiuration type)
    @return
        - @b E_OK: No error.
        - @b E_PAR: Parameter error.
*/
UINT32 WavFileRecord_SetConfig(UINT32 cfg, UINT32 p1, UINT32 p2);

/**
    Get Wav file record configuration.

    Get Wav file record configuration.

    @param[in]  cfg             Configuration type.
    @param[in]  p1              parameter 1. (According to the confiuration type)
    @param[in]  p2              parameter 2. (According to the confiuration type)
    @return     Configuration value.
*/
UINT32 WavFileRecord_GetConfig(UINT32 cfg, UINT32 p1, UINT32 p2);


/**
    Set file name and user data.

    Set file name and user data.

    @param[in]  fileName         Wav file name.
    @param[in]  userData         User data.
    @return
        - @b TRUE: No error.
        - @b FALSE: Open too many times.
*/
BOOL WavFile_SaveOpen(PWAVFILE_FILENAME fileName, MEM_RANGE userData);

/**
    Update wav file header and close wav file.

    Update wav file header and close wav file.

    @return
        - @b TRUE: No error.
        - @b FALSE: Write file fail.
*/
BOOL WavFile_SaveClose(void);

/**
    Create wav file and write header.

    Create wav file and write header.

    @param[in]  info            Audio info configurations.
    @param[in]  stopCount       Stop sample count.
    @param[in]  hdrMem          Buffer info of wav header.
    @return
        - @b TRUE: No error.
        - @b FALSE: File error.
*/
BOOL WavFile_SaveHeader(PWAVFILE_AUD_INFO info, UINT64 *stopCount, MEM_RANGE hdrMem);

/**
    Get sound data from buffer.

    Get sound data from buffer.

    @param[in]  pAudBufQue      Buffer to be read.
    @param[in]  id              Act id.
    @return
        - @b TRUE: No error.
        - @b FALSE: File error.
*/
BOOL WavFile_SaveCB(PAUDIO_BUF_QUEUE pAudBufQue, UINT32 id, UINT64 timestamp);

//@}
