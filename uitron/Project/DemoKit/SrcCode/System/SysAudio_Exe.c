/*
    System Audio Callback

    System Callback for Audio Module.

    @file       SysLens_Audio.c
    @ingroup    mIPRJSYS

    @note

    Copyright   Novatek Microelectronics Corp. 2010.  All rights reserved.
*/

////////////////////////////////////////////////////////////////////////////////
#include "SysCommon.h"
#include "AppCommon.h"
////////////////////////////////////////////////////////////////////////////////
#include "UIFrameworkExt.h"
#include "AppLib.h"
#include "GxSound.h"
#include "SMediaPlayAPI.h"
#include "Dx.h"
#include "DxType.h"
#include "DxApi.h"
#include "DxSound.h"
#include "Audio.h"
#if(UI_FUNC==ENABLE)
#include "SoundData.h"
#endif

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

void System_SetAudioOutput(void);
void Sound_CB(UINT32 event, UINT32 param1, UINT32 param2);
DX_HANDLE gDevAudObj = 0;

#if 0
static BOOL gDevAudMode = TRUE; //TURE = full-duplex; FALSE = half-duplex
static BOOL gDevAudInput = FALSE; //send from network
static BOOL gDevAudPlay = FALSE; //play to device
#else
#if (LCD1_FUNC == ENABLE)
extern DX_HANDLE gDevLCDObj;
#endif
#if (TV_FUNC == ENABLE)
extern DX_HANDLE gDevTVObj;
#endif
#if (HDMI_FUNC == ENABLE)
extern DX_HANDLE gDevHDMIObj;
#endif
#endif

static AUDIO_SETTING gProjCommDxSndSet = {0};
static AUDIO_DEVICE_OBJ gProjCommDxSndDev = {0};

int SX_TIMER_DET_AIN_ID = -1;
void Audio_DetAudInsert(void);
#if (LINE_INSERT_FUNCTION == ENABLE)
SX_TIMER_ITEM(Audio_DetAudInsert, Audio_DetAudInsert, 25, FALSE)
static UINT32 audInsertMask = AOUT_DEFAULT_ATTACH_DEVICE; //default: only speaker
static BOOL gDevAudOut = FALSE; //FALSE:SPK, TRUE:LINE
#endif

void System_OnAudioInit(void)
{
	gDevAudObj = Dx_GetObject(DX_CLASS_AUDIO_EXT);
	TM_BOOT_BEGIN("audio", "init");
	//PHASE-1 : Init & Open Drv or DrvExt
	{
		if (gDevAudObj) {
			Dx_Init(gDevAudObj, (void *)&gProjCommDxSndSet, 0, DXSOUND_VER);
			//fix for CID 44938 - begin
			if (Dx_Open(gDevAudObj) != DX_OK) {
				DBG_ERR("DxAudio open fail!\r\n");
				return;
			}
			//fix for CID 44938 - end
			Dx_Control(gDevAudObj, DXSOUND_CAP_SET_DEVINITDATA, (UINT32)&gProjCommDxSndDev, (UINT32)&gProjCommDxSndSet);
			Dx_Control(gDevAudObj, DXSOUND_CAP_SET_CODEC, (UINT32)NULL, AUDIO_CODECSEL_DEFAULT);
			GxSound_Open(gDevAudObj, Sound_CB);
		}
#if(UI_FUNC==ENABLE)
		UISound_RegTable();
#endif
#if (LINE_INSERT_FUNCTION == ENABLE)
		SX_TIMER_DET_AIN_ID = SxTimer_AddItem(&Timer_Audio_DetAudInsert);
		SxTimer_SetFuncActive(SX_TIMER_DET_AIN_ID, TRUE);
#endif
	}
	//PHASE-2 : Init & Open Lib or LibExt
	{
		//config audio Output
		System_SetAudioOutput();
	}
	//    aud_setTotalVolLevel(AUDIO_VOL_LEVEL8);
	TM_BOOT_END("audio", "init");
}

void System_OnAudioExit(void)
{
	//PHASE-2 : Close Lib or LibExt
	{

	}
	//PHASE-1 : Close Drv or DrvExt
	{
		if (gDevAudObj) {
			GxSound_Close();
		}
	}
}

///////////////////////////////////////////////////////////////////////
void Audio_DetAudInsert(void)
{
#if (LINE_INSERT_FUNCTION == ENABLE)
	//DBG_MSG("DetAudio\r\n");
	DX_HANDLE pAudDev1 = Dx_GetObject(DX_CLASS_AUDIO_EXT);
	GxSound_DetInsert((UINT32)pAudDev1, 0);
#endif
	//DBG_MSG("\r\n");
}

///////////////////////////////////////////////////////////////////////

void System_SetAudioOutput(void)
{
#if (LINE_INSERT_FUNCTION == ENABLE)
#if 0//(AUDIO_PLAY_FUNCTION == ENABLE)
	BOOL cPlay = FALSE;
#endif
	//=========================================================================
	// Change Audio Output according to LINE cable inserted or not
	//=========================================================================
	DBG_IND("\r\n");
#if 0//(AUDIO_PLAY_FUNCTION == ENABLE)
	cPlay = gDevAudPlay; //get current play status
	if (cPlay) {
		System_AudioOutStop();    //stop output
	}
#endif
	if (audInsertMask == SPK_MASK) { //only speaker
		// set audio output device entry for LCD/SPK case
		DBG_DUMP("Audio Out = SPK\r\n");
		gDevAudOut = FALSE;
		GxSound_SetOutDevConfigIdx(0);
#if 0//(AUDIO_PLAY_FUNCTION == ENABLE)
		WavStudio_SetConfig(WAVSTUD_CFG_PLAY_OUT_DEV, 0, 0);
#endif
#if (PLAY_MODE==ENABLE)
		SMediaPlay_SetOutDevConfigIdx(0);
#endif
	} else if (audInsertMask & LOUT_MASK) { //with lineout
		// set audio output device entry for TV/LINE case
		DBG_DUMP("Audio Out = LINE\r\n");
		gDevAudOut = TRUE;
		GxSound_SetOutDevConfigIdx(1);
#if 0//(AUDIO_PLAY_FUNCTION == ENABLE)
		WavStudio_SetConfig(WAVSTUD_CFG_PLAY_OUT_DEV, 1, 0);
#endif
#if (PLAY_MODE==ENABLE)
		SMediaPlay_SetOutDevConfigIdx(1);
#endif
	}
#if 0//(AUDIO_PLAY_FUNCTION == ENABLE)
	if (cPlay) {
		System_AudioOutStart();    //start output
	}
#endif
#else //others
	//=========================================================================
	// Change Audio Output according to TV/HDMI cable inserted or not
	//=========================================================================
	DX_HANDLE VideoDevObj1;
	DX_HANDLE VideoDevObj2;
	VideoDevObj1 = (DX_HANDLE)GxVideo_GetDevice(DOUT1);
#if (DISPLAY2_FUNC == ENABLE)
	VideoDevObj2 = (DX_HANDLE)GxVideo_GetDevice(DOUT2);
#else
	VideoDevObj2 = (DX_HANDLE)0;
#endif
#if (LCD1_FUNC == ENABLE)
	if ((VideoDevObj1 == gDevLCDObj) && (VideoDevObj2 == 0)) { //case 1
		// set audio output device entry for LCD case
		GxSound_SetOutDevConfigIdx(0);
#if 0//(AUDIO_PLAY_FUNCTION == ENABLE)
		WavStudio_SetConfig(WAVSTUD_CFG_PLAY_OUT_DEV, 0, 0);
#endif
#if (PLAY_MODE==ENABLE)
		SMediaPlay_SetOutDevConfigIdx(0);
#endif
	}
#endif
#if (TV_FUNC == ENABLE)
	if ((VideoDevObj1 == gDevTVObj) && (VideoDevObj2 == 0)) { //case 2
		// set audio output device entry for TV case
		GxSound_SetOutDevConfigIdx(1);
#if 0//(AUDIO_PLAY_FUNCTION == ENABLE)
		WavStudio_SetConfig(WAVSTUD_CFG_PLAY_OUT_DEV, 1, 0);
#endif
#if (PLAY_MODE==ENABLE)
		SMediaPlay_SetOutDevConfigIdx(1);
#endif
	}
#endif
#if (HDMI_FUNC == ENABLE)
	if ((VideoDevObj1 == gDevHDMIObj) && (VideoDevObj2 == 0)) { //case 3
		// set audio output device entry for HDMI case
		GxSound_SetOutDevConfigIdx(2);
#if 0//(AUDIO_PLAY_FUNCTION == ENABLE)
		WavStudio_SetConfig(WAVSTUD_CFG_PLAY_OUT_DEV, 2, 0);
#endif
#if (PLAY_MODE==ENABLE)
		SMediaPlay_SetOutDevConfigIdx(2);
#endif
	}
#endif
#if (DISPLAY2_FUNC == ENABLE)
#if ((LCD1_FUNC == ENABLE) && (TV_FUNC == ENABLE) && (DUALDISP_FUNC == ENABLE))
	if ((VideoDevObj1 == gDevLCDObj) && (VideoDevObj2 == gDevTVObj)) { //case 4
		// set audio output device entry for LCD & TV case
		GxSound_SetOutDevConfigIdx(3);
#if 0//(AUDIO_PLAY_FUNCTION == ENABLE)
		WavStudio_SetConfig(WAVSTUD_CFG_PLAY_OUT_DEV, 3, 0);
#endif
#if (PLAY_MODE==ENABLE)
		SMediaPlay_SetOutDevConfigIdx(3);
#endif
	}
#endif
#if ((LCD1_FUNC == ENABLE) && (HDMI_FUNC == ENABLE) && (DUALDISP_FUNC == ENABLE))
	if ((VideoDevObj1 == gDevLCDObj) && (VideoDevObj2 == gDevHDMIObj)) { //case 5
		// set audio output device entry for LCD & HDMI case
		GxSound_SetOutDevConfigIdx(4);
#if 0//(AUDIO_PLAY_FUNCTION == ENABLE)
		WavStudio_SetConfig(WAVSTUD_CFG_PLAY_OUT_DEV, 4, 0);
#endif
#if (PLAY_MODE==ENABLE)
		SMediaPlay_SetOutDevConfigIdx(4);
#endif
	}
#endif
#endif
#endif
}

///////////////////////////////////////////////////////////////////////
//Device flow event

INT32 System_OnAudioInsert(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
	DBG_DUMP("^Y===================================== audio plug [%s]\r\n", "LINE");
#if (LINE_INSERT_FUNCTION == ENABLE)
	audInsertMask |= 0x02; //line insert
	System_SetAudioOutput();
#endif
	return NVTEVT_CONSUME;
}

INT32 System_OnAudioRemove(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
	DBG_DUMP("^Y===================================== audio unplug [%s]\r\n", "LINE");
#if (LINE_INSERT_FUNCTION == ENABLE)
	audInsertMask &= ~0x02; //line remove
	System_SetAudioOutput();
#endif
	return NVTEVT_CONSUME;
}


INT32 System_OnAudioAttach(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
	//  AUDIO: if(boot)->play boot sound
	//  AUDIO: if(boot)->start audio srv
	return NVTEVT_CONSUME;
}

INT32 System_OnAudioDetach(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
	return NVTEVT_CONSUME;
}

INT32 System_OnAudioMode(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
	if (paramNum < 1) {
		return NVTEVT_CONSUME;
	}

#if 0//(AUDIO_PLAY_FUNCTION == ENABLE)
	if (gDevAudObj) {
		gDevAudMode = paramArray[0];
	}
#endif

	return NVTEVT_CONSUME;
}

INT32 System_OnAudioInput(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
	if (paramNum < 1) {
		return NVTEVT_CONSUME;
	}

#if 0//(AUDIO_PLAY_FUNCTION == ENABLE)
	if (gDevAudObj) {
		if ((paramArray[0] == 1) && (!gDevAudInput)) { //start
			DBG_MSG("^GAudOut Start - begin)\r\n");
			gDevAudInput = TRUE;
			if (!gDevAudMode) { //half-duplex
				DBG_MSG("audio record - MUTE\r\n");
				Ux_SendEvent(0,   NVTEVT_EXE_MOVIE_AUDIO,         1,  RECMOVIE_AUD_OFF);
			}
			DBG_MSG("^GAudOut Start - end\r\n");
		} else if ((paramArray[0] == 0) && (gDevAudInput)) { //stop
			DBG_MSG("^GAudOut Stop - begin\r\n");
			gDevAudInput = FALSE;
			if (!gDevAudMode) { //half-duplex
				DBG_MSG("audio record - UNMUTE\r\n");
				Ux_SendEvent(0,   NVTEVT_EXE_MOVIE_AUDIO,         1,  RECMOVIE_AUD_ON);
			}
			DBG_MSG("^GAudOut Stop - end\r\n");
		}
	}
#endif

	return NVTEVT_CONSUME;
}

INT32 System_OnAudioVol(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
	if (paramNum < 1) {
		return NVTEVT_CONSUME;
	}

	return NVTEVT_CONSUME;
}

BOOL System_GetAudioOut(void)
{
#if (LINE_INSERT_FUNCTION == ENABLE)
	return gDevAudOut;
#else
	return 0;
#endif
}

BOOL System_GetAudioMode(void)
{
#if 0//(AUDIO_PLAY_FUNCTION == ENABLE)
	return gDevAudMode;
#else
	return 0;
#endif
}

BOOL System_GetAudioInput(void)
{
#if 0//(AUDIO_PLAY_FUNCTION == ENABLE)
	return gDevAudInput;
#else
	return 0;
#endif
}

UINT32 System_GetAudioVol(void)
{
	return 0;
}

INT32 System_PutAudioData(UINT32 did, UINT32 addr, UINT32 *ptrsize)
{
	return 0;
}

//#NT#2016/09/08#HM Tseng -begin
//#NT#Support audio channel
UINT32 System_GetAudioChannel(void)
{
	return 0;
}
//#NT#2016/09/08#HM Tseng -end

