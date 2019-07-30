/**
    WavStudio module.

    @file       WavStudioTsk.h
    @ingroup    mILIBWAVSTUDIO

    @brief      Header file of WavStudio task.

                APIs/structure/macro declaration of WavStudio task.

    @note       Nothing.

    Copyright   Novatek Microelectronics Corp. 2005.  All rights reserved.

*/

/**
    @addtogroup mILIBWAVSTUDIO
*/
//@{

#ifndef _WAVSTUDIOTSK_H
#define _WAVSTUDIOTSK_H

#include "SysKer.h"
#include "DxApi.h"
#include "DxSound.h"


extern void WavStudio_InstallID(void) _SECTION(".kercfg_text");
extern BOOL WavStudio_ChkModInstalled(void);


/**
    @name Wave studio channel
*/
//@{
#define WAVSTUD_PLAY_AUD_CH               AUDTS_CH_TX1    ///< Play channel
#define WAVSTUD_RECORD_AUD_CH             AUDTS_CH_RX     ///< Record channel
#define WAVSTUD_LOOPBACK_AUD_CH           AUDTS_CH_TXLB   ///< Playback loopback channel
//@}

/**
    @name Wave studio task status
*/
//@{
#define WAVSTUD_STS_UNKNONWN              0x00000000      ///<
#define WAVSTUD_STS_CLOSED                0x00000001      ///< Task is not opened
#define WAVSTUD_STS_STOPPED               0x00000002      ///< Stopped (No recording or playing)
#define WAVSTUD_STS_GOING                 0x00000004      ///< Stopped (No recording or playing)
#define WAVSTUD_STS_PAUSED                0x00000008      ///< Stopped (No recording or playing)
#define WAVSTUD_STS_SAVING                0x00000010      ///< Saving remaining data in RAM to card
//@}


/**
    Stop count if non-stop playing / recording
*/
#define WAVSTUD_NON_STOP_COUNT            0xFFFFFFFFFFFFFFFFULL


/**
    Wave studio event IDs
*/
typedef enum _WAVSTUD_CB_EVT_ {
	WAVSTUD_CB_EVT_OK = 0,              ///< OK
	WAVSTUD_CB_EVT_FAIL,                ///< Fail
	WAVSTUD_CB_EVT_TCHIT,               ///< Time code hit
	WAVSTUD_CB_EVT_STARTED,             ///< WavStudio started
	WAVSTUD_CB_EVT_STOPPED,             ///< WavStudio stopped
	WAVSTUD_CB_EVT_PAUSED,              ///< WavStudio paused
	WAVSTUD_CB_EVT_RESUMED,             ///< WavStudio resumed
	WAVSTUD_CB_EVT_BUF_FULL,            ///< Buffer full
	WAVSTUD_CB_EVT_BUF_EMPTY,           ///< Buffer empty
	WAVSTUD_CB_EVT_READ_FAIL,           ///< Read data fail
	WAVSTUD_CB_EVT_WRITE_FAIL,          ///< Write data fail
	WAVSTUD_CB_EVT_READ_LAST,           ///< Read last data
	WAVSTUD_CB_EVT_BUF_DONE,            ///< Buffer done
	WAVSTUD_CB_EVT_PRELOAD_DONE,        ///< Preload done
	WAVSTUD_CB_EVT_MAX
} WAVSTUD_CB_EVT;

/**
    Wave studio configurations
*/
typedef enum _WAVSTUD_CFG {
	WAVSTUD_CFG_MODE,                   ///< WavStudio Mode
	WAVSTUD_CFG_MEM,                    ///< Get need memory size
	WAVSTUD_CFG_VOL,                    ///< Volume
	WAVSTUD_CFG_AUD_SR,                 ///< Sample rate
	WAVSTUD_CFG_AUD_CH,                 ///< Channel
	WAVSTUD_CFG_AUD_BIT_PER_SEC,        ///< Bit per second
	WAVSTUD_CFG_PLAY_OUT_DEV,           ///< Output device index
	WAVSTUD_CFG_GET_STATUS,             ///< WavStudio status
	WAVSTUD_CFG_HDR_ADDR,               ///< Wav header buffer address
	WAVSTUD_CFG_BUFF_UNIT_TIME,         ///< Buffer unit time
	WAVSTUD_CFG_ALLOC_MEM,              ///< Get allocated memory size
	WAVSTUD_CFG_QUEUE_ENTRY_NUM,        ///< Number of play queue entry
	WAVSTUD_CFG_BUFF_UNIT_SIZE,         ///< Size of one unit buffer
	///  = sample rate * byte per sample * channel number * buffer unit time
	///  e.g. 8K SR, 16 bps, stereo, 200ms per buffer = 8000 * 2 * 2 * 0.2
	WAVSTUD_CFG_RECV_CB,                ///< Callback function to notify the source of play data. Only for passive play mode
	WAVSTUD_CFG_AUD_EVT_CB,             ///< Callback function to notify audio driver event.
	WAVSTUD_CFG_PROC_UNIT_SIZE,
	WAVSTUD_CFG_AEC_OBJ,
	WAVSTUD_CFG_UNLOCK_ADDR,
	WAVSTUD_CFG_PLAY_LB_CHANNEL,
	WAVSTUD_CFG_AUD_CODEC,
	WAVSTUD_CFG_MAX
} WAVSTUD_CFG;

/**
    Wave studio act types
*/
typedef enum _WAVSTUD_ACT {
	WAVSTUD_ACT_PLAY,                   ///< Play
	WAVSTUD_ACT_REC,                    ///< Record 1
	WAVSTUD_ACT_REC2,                   ///< Record 2
	WAVSTUD_ACT_LB,
	WAVSTUD_ACT_MAX,
} WAVSTUD_ACT;

/**
    Wave studio memory
*/
typedef struct _WAVSTUD_MEM {
	UINT32 uiAddr;                      ///< Start Memory Address
	UINT32 uiSize;                      ///< Start Memory Size
} WAVSTUD_MEM, *PWAVSTUD_MEM;

/**
    Wave studio event callback function prototype
*/
typedef void (*WAVSTUD_CB)(WAVSTUD_ACT act, UINT32 p1, UINT32 p2);

/**
    Wave studio memory allocation
*/
typedef struct {
	WAVSTUD_MEM     mem;
	WAVSTUD_CB      pWavStudCB;     ///< Wave studio record/playback callback
} WAVSTUD_APPOBJ, *PWAVSTUD_APPOBJ;

/**
    Bit per sample
*/
typedef enum _WAVSTUD_BITS_PER_SAM {
	WAVSTUD_BITS_PER_SAM_8  = 0x08,     ///< 8 bits PCM
	WAVSTUD_BITS_PER_SAM_16 = 0x10,     ///< 16 bits PCM
} WAVSTUD_BITS_PER_SAM;

/**
    Codec Type
*/
typedef enum {
	WAVSTUDCODEC_PCM,                   ///< PCM
	WAVSTUDCODEC_MAX
} WAVSTUD_CODEC;

/**
    Wave studio audio info
*/
typedef struct _WAVSTUD_AUD_INFO {
	AUDIO_SR            audSR;          ///< Sample Rate
	AUDIO_CH            audCh;          ///< Channel
	UINT32              bitPerSamp;     ///< bit Per Sample
	UINT32				ch_num;			///< Channel number
} WAVSTUD_AUD_INFO, *PWAVSTUD_AUD_INFO;


/**
    Time of one buffer
*/
typedef enum _WAVSTUD_UNIT_TIME {
	WAVSTUD_UNIT_TIME_0,
	WAVSTUD_UNIT_TIME_100MS,
	WAVSTUD_UNIT_TIME_200MS,
	WAVSTUD_UNIT_TIME_300MS,
	WAVSTUD_UNIT_TIME_400MS,
	WAVSTUD_UNIT_TIME_500MS,
	WAVSTUD_UNIT_TIME_600MS,
	WAVSTUD_UNIT_TIME_700MS,
	WAVSTUD_UNIT_TIME_800MS,
	WAVSTUD_UNIT_TIME_900MS,
	WAVSTUD_UNIT_TIME_1000MS,
} WAVSTUD_UNIT_TIME;

typedef enum {
	WAVSTUD_DATA_MODE_PUSH,
	WAVSTUD_DATA_MODE_PULL,
	WAVSTUD_DATA_MODE_PUSH_FILE,
	ENUM_DUMMY4WORD(WAVSTUD_DATA_MODE)
} WAVSTUD_DATA_MODE;

/**
    Wave studio audio codec
*/
typedef enum _WAVSTUD_AUD_CODEC {
	WAVSTUD_AUD_CODEC_EMBEDDED,
	WAVSTUD_AUD_CODEC_EXTERNAL,
} WAVSTUD_AUD_CODEC;

/**
    Audio info set
*/
typedef struct _WAVSTUD_INFO_SET {
	UINT32 objCount;                  ///<
	WAVSTUD_DATA_MODE dataMode;

	WAVSTUD_AUD_INFO audInfo;           ///< Audio info of record
	UINT32 bufCount;                 ///< Record buffer count (Should >= 5)
	WAVSTUD_UNIT_TIME unitTime;      ///< Record unit time
} WAVSTUD_INFO_SET, *PWAVSTUD_INFO_SET;

/**
    AEC object
*/
typedef struct _WAVSTUD_AUD_AEC_OBJ {
	ER(*Open)(void);
	ER(*Close)(void);
	ER(*Start)(INT32, INT32, INT32);
	ER(*Stop)(void);
	ER(*Apply)(UINT32, UINT32, UINT32, UINT32);
	void(*Enable)(BOOL);
	ER(*SetBuf)(UINT32, UINT32);
} WAVSTUD_AUD_AEC_OBJ, *PWAVSTUD_AUD_AEC_OBJ;

/**
    Open WAV studio task.

    Start WAV studio task.

    @param[in] pWavObj      Wav Studio application object
    @param[in] dxSndHdl     Dx sound object handle
    @param[in] pWavInfoSet  Audio info, act count, and buffer count of play and record
    @return Open status
        - @b E_SYS: Task is already opened
        - @b E_NOMEM: Memory size is not enough
        - @b E_OK: No error
*/
extern ER WavStudio_Open(WAVSTUD_ACT actType, PWAVSTUD_APPOBJ pWavObj, DX_HANDLE dxSndHdl, PWAVSTUD_INFO_SET pWavInfoSet);

/**
    Close WAV studio task.

    Close WAV studio task.

    @return Close status
        - @b E_SYS: Task is already closed
        - @b E_OK: No error
*/
extern ER WavStudio_Close(WAVSTUD_ACT actType);


/**
    Start to record or play.

    Start to record or play.

    @param[in]  actType         Act type.
    @param[in]  info            Audio info configurations.
    @param[in]  stopcount       Stop sample count.
    @param[in]  DataCB          Data callback function pointer.\n
                                When actType is play, the callback function should fill sound data to the buffer.\n
                                When actType is record, the callback function should get sound data from the buffer.
    @return
        - @b TRUE: Start to record
        - @b FALSE: WAV studio task is not in stopped status
*/
extern BOOL WavStudio_Start(WAVSTUD_ACT actType, PWAVSTUD_AUD_INFO info, UINT64 stopcount, BOOL (*DataCB)(PAUDIO_BUF_QUEUE, UINT32, UINT64));

/**
    Stop recording / playing.

    Stop recording / playing.

    @param[in]  actType         Act type.
    @return
        - @b TRUE: Stop recording / playing.
        - @b FALSE: WAV studio task doesn't recording or playing.
*/
extern BOOL WavStudio_Stop(WAVSTUD_ACT actType);

/**
    Pause recording / playing.

    Pause recording / playing.

    @param[in]  actType         Act type.
    @return
        -@ TRUE: Pause recording / playing.
        -@ FALSE: Not recording / playing.
*/
extern BOOL WavStudio_Pause(WAVSTUD_ACT actType);

/**
    Resume recording / playing.

    Resume recording / playing from pause status.

    @param[in]  actType         Act type.
    @return
        - @b TRUE: Resume from paused status.
        - @b FALSE: Not in paused status.
*/
extern BOOL WavStudio_Resume(WAVSTUD_ACT actType);

/**
    Set WavStudio configuration.

    Set WavStudio configuration.

    @param[in]  cfg             Configuration type.
    @param[in]  p1              parameter 1. (According to the confiuration type)
    @param[in]  p2              parameter 2. (According to the confiuration type)
    @return
        - @b E_OK: No error.
        - @b E_PAR: Parameter error.
*/
extern UINT32 WavStudio_SetConfig(WAVSTUD_CFG cfg, UINT32 p1, UINT32 p2);

/**
    Get WavStudio configuration.

    Get WavStudio configuration.

    @param[in]  cfg             Configuration type.
    @param[in]  p1              parameter 1. (According to the confiuration type)
    @param[in]  p2              parameter 2. (According to the confiuration type)
    @return     Configuration value.
*/
extern UINT32 WavStudio_GetConfig(WAVSTUD_CFG cfg, UINT32 p1, UINT32 p2);

/**
    Wait for WavStudio start.

    Wait for WavStudio start.

    @param[in]  actType         Act type.
*/
extern void WavStudio_WaitStart(WAVSTUD_ACT actType);

/**
    Wait for WavStudio stop.

    Wait for WavStudio stop.

    @param[in]  actType         Act type.
*/
extern void WavStudio_WaitStop(WAVSTUD_ACT actType);

/**
    Push buffer to queue.

    Push buffer to play queue. Only valid in passive play mode.

    @param[in]      uiAddr      Buffer address.
    @param[in,out]  pSize       Pointer to the buffer size.
                                Return the actual size that is pushed into play queue.
    @return
        - @b TRUE: Buffer is successfully pushed into queue.
        - @b FALSE: Buffer is not completely pushed into queue. Refer to pSize for the actual pushed size.
*/
extern BOOL WavStudio_PushBufToQueue(UINT32 uiAddr, UINT32 *pSize);

#endif

//@}
