/*
    System Sound Callback

    System Callback for Sound Module.

    @file       SysSoundCB.c
    @ingroup    mIPRJSYS

    @note       �o���ɮ׭t�d�@���

                1.����Sound callback������
                  Sound_CB()
                  �i�઺event��:
                     SYSTEM_CB_CONFIG
                     SOUND_CB_START
                       �|�I�sGxFlash_SuspendCharge();
                     SOUND_CB_STOP
                       �|�I�sGxFlash_ResumeCharge();
                     SOUND_CB_OUTDEVCHG
                       �|�I�sGxSound_SetOutput()


    Copyright   Novatek Microelectronics Corp. 2010.  All rights reserved.
*/

////////////////////////////////////////////////////////////////////////////////
#include "SysCommon.h"
////////////////////////////////////////////////////////////////////////////////
#if 0//_SOUND_CB_CHARGE_
#include "GxFlash.h"
#endif
#include "GxSound.h"
#include "Debug.h"
#include "GxSound.h"
#include "AppLib.h"

//global debug level: PRJ_DBG_LVL
#include "PrjCfg.h"
//local debug level: THIS_DBGLVL
#define THIS_DBGLVL         2 // 0=FATAL, 1=ERR, 2=WRN, 3=UNIT, 4=FUNC, 5=IND, 6=MSG, 7=VALUE, 8=USER
///////////////////////////////////////////////////////////////////////////////
#define __MODULE__          SysAudioExe
#define __DBGLVL__          ((THIS_DBGLVL>=PRJ_DBG_LVL)?THIS_DBGLVL:PRJ_DBG_LVL)
#define __DBGFLT__          "*" //*=All, [mark]=CustomClass
#include "DebugModule.h"
///////////////////////////////////////////////////////////////////////////////


int SX_TIMER_DET_AUDIODEV_ID = -1;
extern BOOL g_bIsInitSystemFinish;

UINT32 guiSndRepeat = FALSE;

void Sound_SetRepeat(UINT32 isTrue)
{
	guiSndRepeat = isTrue;
}

UINT32 Sound_GetRepeat(void)
{
	return guiSndRepeat;
}

void Sound_CB(UINT32 event, UINT32 param1, UINT32 param2)
{
	DBG_IND("event=%d,p1=%08x,p2=%08x\r\n", event, param1, param2);
	switch (event) {
	case SYSTEM_CB_CONFIG:
		//1.�]�winit��
		//2.�]�wCB��,
		//3.���USxJob�A�� ---------> System Job
		//4.���USxTimer�A�� ---------> Detect Job
		//5.���USxCmd�A�� ---------> Cmd Function
		//System_AddSxCmd(Sound_OnCommand); //GxSound
		break;
	case SOUND_CB_START:
		break;
	case SOUND_CB_STOP:
#if SLIDESHOW_MUSIC_OPTION
		//#NT#2011/02/10#Lily Kao -begin
		if (guiSndRepeat) {
			//#NT#2011/02/18#Lily Kao -begin
			FlowPlaySlideShow_PlaySpecificBGMusic(UI_SLIDEMUSIC_MENU_SETTING);
			//#NT#2011/02/18#Lily Kao -end
		}
		//#NT#2011/02/10#Lily Kao -end
#endif
		break;
	case SOUND_CB_PLUG:
		//This CB will be trigger when LINE plug
		DBG_IND("SOUND_CB_PLUG\r\n");
		{
			Ux_PostEvent(NVTEVT_AUDIO_INSERT, 1, param1);
		}
		break;
	case SOUND_CB_UNPLUG:
		//This CB will be trigger when LINE unplug
		DBG_IND("SOUND_CB_UNPLUG\r\n");
		{
			Ux_PostEvent(NVTEVT_AUDIO_REMOVE, 1, param1);
		}
		break;
	default:
		DBG_ERR("Not Support SOUND_CB event=%d\r\n", event);
		break;
	}
	DBG_IND("end\r\n");
}
