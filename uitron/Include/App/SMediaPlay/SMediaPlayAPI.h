/**
    Header file of Super Media Player

    Exported header file of super media player.

    @file       SMediaPlayAPI.h
    @ingroup    mIAPPMEDIAPLAY
    @version    V1.00.000
    @date       2014/02/25

    Copyright   Novatek Microelectronics Corp. 2014.  All rights reserved.
*/

#ifndef _SMEDIAPLAYAPI_H
#define _SMEDIAPLAYAPI_H

#include <stdio.h>
#include <string.h>
#include "Type.h"
#include "kernel.h"
#include "SysKer.h"
#include "MediaReadLib.h"

/**
    @addtogroup mIAPPMEDIAPLAY
*/
//@{

/**
    @name media playing EVENT ID

    Callback ID of media player.
*/
//@{
#define SMEDIAPLAY_EVENT_FINISH            1        ///< Finish
#define SMEDIAPLAY_EVENT_ONE_SECOND        2        ///< One second arrives
#define SMEDIAPLAY_EVENT_ONE_VIDEOFRAME    3        ///< Decode one video-frame
#define SMEDIAPLAY_EVENT_START             4        ///< Start mjpgMov playback
#define SMEDIAPLAY_EVENT_STOP              5        ///< media stop
#define SMEDIAPLAY_EVENT_VID_ERROR         6        ///< decode video frame error
#define SMEDIAPLAY_EVENT_ONE_DISPLAYFRAME  7        ///< One display frame
#define SMEDIAPLAY_EVENT_MEDIAINFO_READY   8        ///< media information ready
#define SMEDIAPLAY_EVENT_CLOSE             9        ///< media close
#define SMEDIAPLAY_EVENT_PAUSE             10       ///< media pause
#define SMEDIAPLAY_EVENT_PLAYING           11       ///< media playing
#define SMEDIAPLAY_EVENT_CURR_VIDFRAME     12       ///< current video frame stream data ready
#define SMEDIAPLAY_EVENT_CURR_AUDFRAME     13       ///< current audio frame stream data ready
#define SMEDIAPLAY_EVENT_FILE_READ_ERROR   14       ///< file read error
//@}

/**
    @name type of playing direction

    Type of playing direction
*/
//@{
#define SMEDIAPLAY_DIR_FORWARD              1       ///< Forward Direction
#define SMEDIAPLAY_DIR_BACKWARD             2       ///< Backward Direction
//@}
/**
    Speed type definition

    SMedia play speed definition

    @note for SMediaPlay_SpeedChange()
*/
typedef enum {
    SMEDIAPLAY_SPEED_NORMAL  = 0,                 ///< normal speed
    SMEDIAPLAY_SPEED_2X      = 2,                 ///< 2x   speed, trigger by timer (Play 2*I-frame number in second)
    SMEDIAPLAY_SPEED_4X      = 4,                 ///< 4x   speed, trigger by timer (Play 4*I-frame number in second)
    SMEDIAPLAY_SPEED_8X      = 8,                 ///< 8x   speed, trigger by timer (Play 8*I-frame number in second)
    SMEDIAPLAY_SPEED_16X     = 16,                ///< 16x  speed, trigger by timer (Play 8*I-frame number & skip 1 I-frames in second)
    SMEDIAPLAY_SPEED_32X     = 32,                ///< 32x  speed, trigger by timer (Play 8*I-frame number & skip 3 I-frames in second)
    SMEDIAPLAY_SPEED_64X     = 64,                ///< 64x  speed, trigger by timer (Play 8*I-frame number & skip 7 I-frames in second)
   	SMEDIAPLAY_SPEED_1_2X    = 5000,              ///< 1/2x speed, trigger by timer (Play 1/2*I-frame number in second)
    SMEDIAPLAY_SPEED_1_4X    = 2500,              ///< 1/4x speed, trigger by timer (Play 1/4*I-frame number in second)
    SMEDIAPLAY_SPEED_1_8X    = 1250,              ///< 1/8x speed, trigger by timer (Play 1/8*I-frame number in second)
    SMEDIAPLAY_SPEED_1_16X   = 625,               ///< 1/16x speed, trigger by timer (Play 1/16*I-frame number in second)
	ENUM_DUMMY4WORD(SMEDIAPLAY_SPEEDUP_TYPE)
} SMEDIAPLAY_SPEEDUP_TYPE;

/**
    Maximum memory information
*/
typedef struct {
    UINT32                      FileFormat;         ///<[in] File format (e.g. MP4, TS, ..., etc.)
    UINT32                      VidFR;              ///<[in] Video frame rate
    UINT32                      AudFR;              ///<[in] Audio frame rate
    UINT32                      TotalSecs;          ///<[in] Total seconds
    UINT32                      CodecType;          ///<[in] Codec type (e.g. H.264, H.265, ..., etc.)
	UINT32                      ImageWidth;         ///<[in] Image width
	UINT32                      ImageHeight;        ///<[in] Image height
    UINT32                      NeedBufsize;        ///<[out] Needed buffer size
} SMEDIAPLAY_MAX_MEM_INFO, *PSMEDIAPLAY_MAX_MEM_INFO;

typedef struct {
	UINT32 Addr;    ///< Memory buffer starting address
	UINT32 Size;    ///< Memory buffer size
} SMEDIAPLAY_MEM_RANGE, *PSMEDIAPLAY_MEM_RANGE;

/**
    Struture of SMedia Player User Object

    Input structure for SMedia play open.

    @note for SMediaPlay_Open()
*/
typedef struct _SMEDIAPLAY_USEROBJ {
	UINT32      uiMemAddr;                          ///<[in] Data buffer starting address
	UINT32      uiMemSize;                          ///<[in] Data buffer size
	void (*CallBackFunc)(UINT32 event_id);          ///<[in] Callback function
	FST_FILE    hActMediafilehdl;                   ///<[in] Opened Active Media File Handler
	UINT32      uiCurrSpeed;                        ///<[in] Current speed to play
	UINT32      uiCurrDirect;                       ///<[in] Current direction to play
	SMEDIAPLAY_MAX_MEM_INFO uiMaxMemInfo;           ///<[in] Max memory information
} SMEDIAPLAY_USEROBJ, *PSMEDIAPLAY_USEROBJ;

/**
    Struture of Video/Audio Frame position and size

    Struture of Video/Audio Frame position and size

*/
typedef struct _SMEDIAPLAY_FRAMEPOS {
	UINT32 Addr;
	UINT32 Size;
} SMEDIAPLAY_FARMEPOS;


/**
     Install SMediaPlay task, flag and semaphore id.

     Install SMedia play task, flag and semaphore id.

     @return void
*/
extern void     SMediaPlay_InstallID(void) _SECTION(".kercfg_text");

extern void     SMediaPlayTsk(void);
extern void     SMediaVideoDecTsk(void);
extern void     SMediaAudioDecTsk(void);
extern void     SMediaPresentTsk(void);
extern void     SMediaTSDemuxTsk(void);


/**
    Open SMedia player task.

    Open SMedia player task. Prepare to play media files.

    @param[in] pSMediaPlayObj    the SMedia player object.

    @return
     - @b E_OK:     open successfully.
     - @b E_SYS:    media player task is already open.
     - @b E_PAR:    invalid input parameters.
*/
extern ER SMediaPlay_Open(PSMEDIAPLAY_USEROBJ pSMediaPlayObj);


/**
    Close SMedia player.

    Stop playing and close SMedia player.

    @return
     - @b E_OK:     close successfully.
     - @b E_SYS:    media player task is closed.
*/
extern ER SMediaPlay_Close(void);

/**
    Start to Play SMedia player.

    Start to play SMedia player.

    @param[in] speedlevel   SMEDIAPLAY_SPEEDUP_TYPE
    @param[in] direction    the playing direction.

    @return
     - @b E_OK:     play successfully.
     - @b E_SYS:    function is failed.
*/
extern ER SMediaPlay_StartPlay(SMEDIAPLAY_SPEEDUP_TYPE speedlevel, UINT32 direction);

/**
    Stop to Play SMedia player.

    Stop to play SMedia player.

    @return
     - @b E_OK:     stop successfully.
     - @b E_SYS:    function is failed.
*/
extern ER SMediaPlay_StopPlay(void);

/*
    Pause SMedia player.

    Pause SMedia player.

    @return
     - @b E_OK:     pause successfully.
     - @b E_SYS:    function is failed.
*/
extern ER SMediaPlay_PausePlay(void);

/*
    Wait Player Ready

    Wait player ready

    @return
     - @b E_OK:     player is ready.
     - @b E_SYS:    media player task is not open or not media files.
*/
extern ER SMediaPlay_WaitReady(void);

/**
    Restart to first frame.

    To restart to first frame. This function is to restart and play UI flow.

    @return
     - @b E_OK:     stop successfully.
     - @b E_SYS:    function is failed.
*/
extern ER SMediaPlay_Restart1stFrmPlay(void);

/**
    Play Video Step By Step.

    Play video step by step.

    @param[in] direction    the playing direction.

    @return
     - @b E_OK:     step by step successfully.
     - @b E_SYS:    function is failed.
*/
extern ER SMediaPlay_VideoOneStepPlay(UINT32 direction);

/**
    Jump to video frame to specific frame.

    Jump to video frame to specific frame.

    @param [in]  frameIndex: Frame of named index number to playback.

    @return jump I-frame index number
*/
extern UINT32 SMediaPlay_JumpVideoToSpecFramePlay(UINT32 frameIndex);

/**
    Get now playing seconds of the media file.

    Get now playing seconds of the media file.

    @return now playing seconds of the media file
*/
extern UINT32 SMediaPlay_GetCurrPlaySecs(void);

/**
    Get total video frames of the media file.

    Get total video frames of the media file after header is parsed.

    @return total video frames of the media file.
*/
extern UINT32 SMediaPlay_GetTotalVideoFrame(void);

/**
    Get now playing video frame of the media file.

    Get now playing video frame of the media file.

    @return now playing video frame of the media file
*/
extern UINT32 SMediaPlay_GetCurrVideoFrame(void);

/**
    Get video frames rate.

    Get video frames rate of the media file after header is parsed.

    @return total video frames rate.
*/
extern UINT32 SMediaPlay_GetVideoFrameRate(void);

/**
    Get the Current Video Frame Data Address and Size

    Get the current video frame data address and size

    param  [in] VFrmInfo: return video frame position and size (SMEDIAPLAY_FARMEPOS)

    @return void
*/
extern void   SMediaPlay_GetVideoFrameData(SMEDIAPLAY_FARMEPOS *VFrmPos);

/**
    Get the Current Audio Frame Data Address and Size

    Get the current audio frame data address and size

    param  [in] AFrmInfo: return audio frame data addresss and size (SMEDIAPLAY_FARMEPOS)

    @return void
*/
extern void   SMediaPlay_GetAudioFrameData(SMEDIAPLAY_FARMEPOS *AFrmPos);

/**
    Enable Get Video/Audio Frame Stream Data event

    Enable Get Video/Audio Frame Stream Data event (SMEDIAPLAY_EVENT_CURR_VIDFRAME/SMEDIAPLAY_EVENT_CURR_AUDFRAME)

    @param  [in] enable: If ture, callback the event SMEDIAPLAY_EVENT_CURR_VIDFRAME/SMEDIAPLAY_EVENT_CURR_AUDFRAME

    @return void
*/
extern void   SMediaPlay_SetVAFrameStrmDataEvent(BOOL enable);

/*
    Get Now Display Frame YUV Address

    Get Now Display Frame YUV Address

    @param  [in] py: Y plane address pointer
    @param  [in] pu: U plane address pointer
    @param  [in] pv: V plane address pointer

    @return void
*/
extern void SMediaPlay_GetNowDisplayFrame(UINT32 *py, UINT32 *pu, UINT32 *pv);

/**
    Get Media Information.

    Get Video/Audio Media Information.

    @param  void

    @return MEDIA_FIRST_INFO pointer
*/
extern MEDIA_FIRST_INFO *SMediaPlay_GetMediaInfo(void);

/**
    Get Display Scale Buffer

    Get Display Scale Buffer

    @param  [in] Addr: address pointer
    @param  [in] Size: sizw pointer

    @return void
*/
extern void SMediaPlay_GetDispScaleBuffer(UINT32 *Addr, UINT32 *Size);

/**
    Get Display Ratio Scale Buffer

    Get Display Ratio Scale Buffer

    @param  [in] Addr: address pointer
    @param  [in] Size: sizw pointer

    @return void
*/
extern void SMediaPlay_GetDispRatioScaleBuffer(UINT32 *Addr, UINT32 *Size);

/**
    Get This Video Frame Offset.

    Get This Video Frame Offset.

    @param  [in] offset   buffer start address of BStream read from file
    @param  [in] framenum Frame of named index number to playback.

    @return void
*/
extern void SMediaPlay_GetThisVideoFrameOffset(UINT64 *offset, UINT32 framenum);

/**
    Get create/modification time in MOV file

    Get create/modification time in MOV file

    param  [in] *pctime: create time pointer
    param  [in] *pmtime: modification time pointer

    @return void
*/
extern void SMediaPlay_GetCreateModifyTime(UINT32 *pctime, UINT32 *pmtime);

/*
    Play Video Frame Data.

    Play video frame data by one shot.

    @param[in] uiBSAddr      Bit-Stream address
    @param[in] uiBSSize      Bit-Stream size
    @param[in] uiThisFrmIdx  This Frame Index
    @param[in] bThisIsEOF    This End of Frame flag

    @return
     - @b E_OK:     step by step successfully.
     - @b E_SYS:    function is failed.
*/
extern ER SMediaPlay_OneShotVideoPlay(UINT32 uiBSAddr, UINT32 uiBSSize, UINT32 uiThisFrmIdx, UINT32 bThisIsEOF);

/**
    Set Audio Volume.

    Set audio volume level.

    @param[in] vol              the audio volume.

    @return void
*/
extern void SMediaPlay_SetAudVolume(UINT32 vol);

/**
     Set the configuration index of SMedia Play Application

     @param[in] outDevConfigIdx   index of audio configuration device.


     @return    None
*/
extern void SMediaPlay_SetOutDevConfigIdx(UINT32 outDevConfigIdx);

/**
    Video File Recovery Function API

    File recovery API for power-cut porection.

    @param[in] pFilePath   video file full path name
    @param[in] memAddr     memory address
    @param[in] size        memory size
    @return
     - @b E_OK:     Recover the video file successfully.
     - @b E_SYS:    Recovery function is failed.
*/

extern ER SMediaPlay_FileRecovery(char *pFilePath, UINT32 memAddr, UINT32 size);

/**
     Dump Video status for Cmd_nsys_dsysdma() in SysCmd_CB.c

     @param[in] pData   pointer of memory to write result.


     @return    How many line of this data status.
*/

extern UINT32 SMediaPlay_DumpDataStatus(UINT32 *pData);

/**
    Initialize Stream Decoder parameters.

    Initialize Stream Decoder parameters.

    @param[in] workbuf        pointer of memory for h264 internal use.
    @param[in] workbufsize    size of memory for h264 internal use.
    @param[in] rawWidth       video width.
    @param[in] rawHeight      video height.
    @param[in] DescAddr   	  pointer of memory for description
    @param[in] DescLen        size of memory for description

    @return
     - @b E_OK:     set successfully.
     - @b E_SYS:    function is failed.
*/
extern ER SMediaPlay_StreamDecodeInit(UINT32 workbuf, UINT32 workbufsize, UINT32 rawWidth, UINT32 rawHeight, UINT32 DescAddr, UINT32 DescLen);

/**
    Stream Trigger One Frame.

    Stream Trigger One Frame.

    @param[in] bs_addr    pointer of memory for bitstream.
    @param[in] bs_size    size of memory for bitstream.
    @param[out] y_addr    pointer of memory for output Raw Y.
    @param[out] uv_addr   pointer of memory for output Raw UV.
    @param[in] frameType  frame type of I_FRAME / P_FRAME

    @return
     - @b E_OK:     set successfully.
     - @b E_SYS:    function is failed.
*/
extern ER SMediaPlay_StreamDecodeOneFrame(UINT32 bs_addr, UINT32 bs_size, UINT32 y_addr, UINT32 uv_addr, UINT32 frameType);

/**
    Seek for specific position.

    Seek for specific position.

    @param[in] position    the timestamp in video file

    @return
     - @b E_OK:     Seek successfully.
     - @b E_PAR:    invalid input parameters.
     - @b E_SYS:    function is failed.
*/
extern ER SMediaPlay_Seek(UINT32 sec);

extern ER SMediaPlay_SetDispTrigCB(UINT32 CallbackFunc);

extern ER SMediaPlay_TriggerDisplay(void);

extern ER SMediaPlay_SetMaxMemInfo(SMEDIAPLAY_MAX_MEM_INFO *pMaxMemInfo);

extern void SMediaPlay_InstallCmd(void);


//@}


#endif
