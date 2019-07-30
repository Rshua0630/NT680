/**
    Copyright   Novatek Microelectronics Corp. 2005.  All rights reserved.

    @file       SoundData.c
    @ingroup    mIPRJAPKey

    @brief      Keypad tone, Startup... sound data
                This file contain the PCM (ADPCM) data of keypad tone, Startup...

    @note       Nothing.

    @date       2005/01/23
*/

/** \addtogroup mIPRJAPKey */
//@{

#ifndef _SOUNDDATA_H
#define _SOUNDDATA_H

#include "Type.h"
#include "PrjCfg.h"


typedef enum {
	DEMOSOUND_SOUND_NONE = 0,
	DEMOSOUND_SOUND_KEY_TONE,
	DEMOSOUND_SOUND_SHUTTER_TONE,
	DEMOSOUND_SOUND_POWERON_TONE,
#if (_ADAS_FUNC_ == ENABLE)
	DEMOSOUND_SOUND_LDWS_TONE,
	DEMOSOUND_SOUND_FCS_TONE,
	DEMOSOUND_SOUND_SNG_TONE,  //STOP and Go
#endif  // #if (_ADAS_FUNC_ == ENABLE)
//#NT#2016/07/20#Brain Yen -begin
//#NT#DDD alarm sound
#if (_DDD_FUNC_ == ENABLE)
	DEMOSOUND_SOUND_DDDWARNING1_TONE,
	DEMOSOUND_SOUND_DDDWARNING2_TONE,
	DEMOSOUND_SOUND_DDDWARNING3_TONE,
#endif  //#if (_DDD_FUNC_ == ENABLE)
//#NT#2016/07/20#Brain Yen -end
	DEMOSOUND_SOUND_MAX_CNT
}
DEMOSOUND_DATA_LIST;


extern void UISound_RegTable(void);
extern void UISound_EnableKey(BOOL bEn);
extern void UISound_Play(UINT32 index);

#endif

//@}
