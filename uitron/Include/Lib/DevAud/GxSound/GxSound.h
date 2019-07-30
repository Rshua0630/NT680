/**
    GxSound module.

    For playing key sound.

    @file       GxSound.h
    @ingroup    mILIBGXSOUND
    @note       Nothing (or anything need to be mentioned).

    Copyright   Novatek Microelectronics Corp. 2012.  All rights reserved.
*/
#ifndef _GXSOUND_H
#define _GXSOUND_H

#include "DxType.h"
#include "GxCommon.h"

#ifdef __cplusplus
extern "C" {
#endif



/**
    @addtogroup mILIBGXSOUND
*/
//@{


/**
     @name Operations on a sound
*/
//@{
#define SOUND_CTRL_STOP             0       //stop playing the current sound immediatly
#define SOUND_CTRL_PLAY             1       //play a sound
#define SOUND_CTRL_WAITSTOP         2       //stop playing the current sound and wait it finished
//@}


/**
     @name Maximum repeat-times for playing a sound
*/
//@{
#define SOUND_PLAY_MAX_TIMES        10
//@}


/**
     Attributes of the sound.

*/
typedef struct {
	const UINT8 *puiData;   ///< points to the memory that a pcm sound data located
	UINT32      uiSize;     ///< how many bytes of the sound data
	UINT32      sampleRate; ///< audio sample rate, ex: 44.1k, 32k, 48k,¡K.
	UINT32      isMono;     ///< mono or stereo
	UINT32      soundId;    ///< an unique Id for the sound, not its index in the sound table
} SOUND_DATA, *PSOUND_DATA;

/**
     To-Do operations on a sound.

*/
typedef enum {
	SOUND_PLAY_START = 0,       ///< start to play a sound
	SOUND_PLAY_STOP,            ///< stop playing a sound
	SOUND_PLAY_PAUSE,           ///< pause playing a sound
	SOUND_PLAY_RESUME,          ///< resume playing a sound
	SOUND_PLAY_MAX
} SOUND_PLAY_EVENT;

/**
     A callback event signals what is going.

     @note: Internal operations on a sound.
*/
typedef enum {
	SOUND_CB_START = 0,
	SOUND_CB_STOP,
	SOUND_CB_OUTDEVCHG, //to be remove
	SOUND_CB_PLUG,
	SOUND_CB_UNPLUG,
	SOUND_CB_MAX
} SOUND_CB_EVENT_TYPE;

/**
     PlaySound callback event function prototype.

*/
typedef void (*FPSOUNDCB)(UINT32 event, UINT32 param1, UINT32 param2);

/**
     Plays a sound, wav-pcm format, which is indicated by "pSndData".

     It is usually used to play a sound which is not in the registered sound table.

     @param[in] sndEvt     Start,stop,pause,or resume play a sound.
                           Please reference the type "SOUND_PLAY_EVENT".
     @param[in] pSndData   Point to the memory that the sound is located.
                           Please reference the structure,"SOUND_DATA".
     @param[in] isPCM      sound data is PCM-Only or Wav.

     @return
         - @b E_OK:    Ok
         - @b E_PAR:   Fail
*/
extern ER GxSound_ActOnSndNotInTbl(SOUND_PLAY_EVENT sndEvt, SOUND_DATA *pSndData, UINT32 isPCM);

/**
     Registers all key sounds in the format of "SOUND_DATA".

     @param[in] uiCount     How many elements in the sound table, "pSoundTable".
     @param[in] pSoundTable Points to an array in the structure of "SOUND_DATA".
                            Please reference the structure,"SOUND_DATA".

     @return
         - @b E_OK:    Ok
         - @b E_PAR:   Fail
*/
extern ER   GxSound_SetSoundTable(UINT32 uiCount, SOUND_DATA *pSoundTable);

/**
     Skip or keep playing a sound.
     @note It is not mute or close the audio driver.

     @param[in] bEn         TRUE=Play, FALSE=Not play
     @return    None.
*/
extern void GxSound_EnableSound(BOOL bEn);

/**
     Whether the sound-playing function is skipped or kept.

     @return
         - @b TRUE:    Keep
         - @b FALSE:   Skip
*/
extern BOOL GxSound_IsSoundEnabled(void);

/**
  Check whether is play or stop

  @param void
  @return BOOL: TRUE -> play, FALSE -> stop
*/
extern BOOL GxSound_IsPlaying(void);

/**
     Get current-set volume.

     @return
         - @b AUDIO_VOL   Volume level. Please reference the structure in the file, Audio.h.
*/
//extern AUDIO_VOL GxSound_GetVolume(void);

/**
     Get current-set volume.

     @return
         - @b UINT32   Volume level. 0 ~ 100
*/
extern UINT32 GxSound_GetVolume(void);


/**
     Set volume.
     Set volume for the next playing sound.

     @param[in] vol         Volume level. Please reference the structure in the file, Audio.h..

     @return    None
*/
//extern void GxSound_SetVolume(AUDIO_VOL vol);

/**
     Set the audio volume

     @param[in] vol   0 ~ 100.


     @return    None
*/
void GxSound_SetVolume(UINT32 vol);



/**
     Open the key-sound playing module.

     @return
         - @b E_OK:    Ok
         - @b E_SYS:   Fail
*/
extern ER GxSound_Open(DX_HANDLE dxSndHdl, FPSOUNDCB fpSoundCB);

/**
     Close the key-sound playing module.

     @return
         - @b E_OK:    Ok
         - @b E_SYS:   Fail
*/
extern ER GxSound_Close(void);

/**
     Play the sound with the "index" in registered sound table by the function "GxSound_SetSoundTable".

     @param[in] index       index in the registered sound table.
     @param[in] times       repeat times

     @return    None
*/
extern void GxSound_Play_Repeat(int index, UINT32 times);

/**
     Play the sound once with the "index" in registered sound table by the function "GxSound_SetSoundTable".

     @param[in] index       index in the registered sound table.

     @return    None
*/
extern void GxSound_Play(int index);

/**
     Stop playing the current sound immediately.

     @return    None
*/
extern void GxSound_Stop(void);

/**
     Stop playing the current sound but wait it finished.

     @return    None
*/
extern void GxSound_WaitStop(void);

/**
     Detect current sound output device insert or not.

     @return    None
*/
extern void GxSound_DetInsert(UINT32 DevID, UINT32 context);

/**
     Set the configuration index of play-sound audio device

     @param[in] outDevConfigIdx   index of audio configuration device.


     @return    None
*/
void GxSound_SetOutDevConfigIdx(UINT32 outDevConfigIdx);


/**
     Install PlaySound task, flag and semaphore id

     This is a default setting API. Users can create their own task id, semphore id,
     flag id to adjust own stack size and task priority as the following:

     @code
     {
        #define PRI_PLAYSOUND            4
        #define STKSIZE_PLAYSOUND        1024

        UINT32 PLAYSOUNDTSK_ID = 0;
        UINT32 FLG_ID_SOUND = 0;

        void GxSound_InstallID(void)
        {
            OS_CONFIG_TASK(PLAYSOUNDTSK_ID,   PRI_PLAYSOUND,     STKSIZE_PLAYSOUND,      PlaySoundTsk);
            OS_CONFIG_FLAG(FLG_ID_SOUND);
        }

     }
     @endcode
*/
extern void GxSound_InstallID(void) _SECTION(".kercfg_text");

extern BOOL GxSound_ChkModInstalled(void);


//@}


#ifdef __cplusplus
} //extern "C"
#endif

#endif //_GXSOUND_H

