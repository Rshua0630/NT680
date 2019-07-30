#include "DxCfg.h"
#include "IOCfg.h"

#include "Debug.h"
#include "Audio.h"
#include "eac.h"

#include "DxSound.h"
#include "mIType.h"
#include "ErrorNo.h"
#include "Audio.h"
#include "hdmitx.h"

#include "DxCommon.h"
#include "DxApi.h"

#include "Type.h"
#include "DxType.h"
#include "Dx.h"
#include "stdio.h" //for sscanf

///////////////////////////////////////////////////////////////////////////////
#define __MODULE__          DxDrv
#define __DBGLVL__          2 // 0=FATAL, 1=ERR, 2=WRN, 3=UNIT, 4=FUNC, 5=IND, 6=MSG, 7=VALUE, 8=USER
#define __DBGFLT__          "*" //*=All, [mark]=CustomClass
#include "DebugModule.h"
///////////////////////////////////////////////////////////////////////////////

#define DRVSOUND_OUTDEV_CONFIG_CNT          5
#define DRVSOUND_MAX_VOL_LVL                100

typedef struct {
	DRVSOUND_VOUT       vidOutType;
	AUDIO_CH            audChannel;
	BOOL                monoExpand;
} DRVSOUND_OUTDEV_CONFIG;

typedef enum {
	DRVSOUND_SR_8000   = 8000,     ///< 8 KHz
	DRVSOUND_SR_11025  = 11025,    ///< 11.025 KHz
	DRVSOUND_SR_12000  = 12000,    ///< 12 KHz
	DRVSOUND_SR_16000  = 16000,    ///< 16 KHz
	DRVSOUND_SR_22050  = 22050,    ///< 22.05 KHz
	DRVSOUND_SR_24000  = 24000,    ///< 24 KHz
	DRVSOUND_SR_32000  = 32000,    ///< 32 KHz
	DRVSOUND_SR_44100  = 44100,    ///< 44.1 KHz
	DRVSOUND_SR_48000  = 48000,    ///< 48 KHz
	ENUM_DUMMY4WORD(DRVSOUND_SR)
} DRVSOUND_SR;

#if 1
DRVSOUND_OUTDEV_CONFIG gDxSndAudOutDevConfigTbl[DRVSOUND_OUTDEV_CONFIG_CNT] = {
	{DRVSOUND_VOUT_LCD, AUDIO_CH_LEFT, FALSE},
	{DRVSOUND_VOUT_TV, AUDIO_CH_RIGHT, FALSE},
	{DRVSOUND_VOUT_HDMI, AUDIO_CH_LEFT, FALSE},
	{DRVSOUND_VOUT_LCDTV, AUDIO_CH_STEREO, FALSE},
	{DRVSOUND_VOUT_LCDHDMI, AUDIO_CH_LEFT, TRUE},
};
#else
DRVSOUND_OUTDEV_CONFIG gDxSndAudOutDevConfigTbl[DRVSOUND_OUTDEV_CONFIG_CNT] = {
	{DRVSOUND_VOUT_LCD, AUDIO_CH_LEFT, TRUE},
	{DRVSOUND_VOUT_TV, AUDIO_CH_LEFT, TRUE},
	{DRVSOUND_VOUT_HDMI, AUDIO_CH_LEFT, FALSE},
	{DRVSOUND_VOUT_LCDTV, AUDIO_CH_LEFT, TRUE},
	{DRVSOUND_VOUT_LCDHDMI, AUDIO_CH_LEFT, TRUE},
};
#endif

DRVSOUND_OUTDEV_CONFIG *gpCurDrvAudOutDev = &gDxSndAudOutDevConfigTbl[0];
UINT32 gDrvSndTotalVolLvl = 0xFFFFFFFF;
PAUDIO_CODEC_FUNC gDrvSndCodecFunc = 0;
AUDIO_CODECSEL gDrvSndCodecSel = AUDIO_CODECSEL_DEFAULT;
AUDIO_VOL_LEVEL gDrvSndTotalAudVolLvl = AUDIO_VOL_LEVEL64;////AUDIO_VOL_LEVEL8
UINT32 gCurDrvSndVol = 50;
AUDIO_GAIN gCurDrvSndGain = AUDIO_GAIN_4;


//========== Debug Messsage =======
#ifdef NDEBUG  //release version
#define _DRVSOUND_DBG_                       0
#else
#define _DRVSOUND_DBG_                       3
#endif

#if (_DRVSOUND_DBG_ > 2)
#define DbgMsg_DrvSoundIO(msg)         DBG_IND msg
#define DbgMsg_DrvSound(msg)           DBG_IND msg
#define DbgCode_DrvSound(x)            x

_INLINE void DrvSound_DmpOutDevConfig(void)
{
	DBG_IND("\r\n");
	DBG_IND("audCodec= %d \r\n",  gpCurDrvAudOutDev->vidOutType);
	DBG_IND("audChannel= %d \r\n", gpCurDrvAudOutDev->audChannel);
	DBG_IND("monoExpand= %d \r\n", gpCurDrvAudOutDev->monoExpand);
}

#else

#define DbgMsg_DrvSoundIO(msg)
#define DbgMsg_DrvSound(msg)
#define DbgCode_DrvSound(x)

_INLINE void DrvSound_DmpOutDevConfig(void) {}

#endif
ER DrvSound_SetCodec(PAUDIO_CODEC_FUNC codecFunc, AUDIO_CODECSEL audCodecSel);
ER DrvSound_SetTotalVolLvl(AUDIO_VOL_LEVEL volLvl);


ER DrvSound_SetOutDevConfig(UINT32 outDevIdx, PAUDTS_OBJ pAudPlayObj)
{
	ER retV = E_OK;
	DBG_IND(":idx=%d\r\n", outDevIdx);

	if (0 == pAudPlayObj) {
		DBG_ERR("No AudObj\r\n");
		return E_PAR;
	}
	if (outDevIdx >= DRVSOUND_OUTDEV_CONFIG_CNT) {
		DBG_ERR(":idx=%d\r\n", outDevIdx);
		return E_PAR;
	}
	gpCurDrvAudOutDev = &gDxSndAudOutDevConfigTbl[outDevIdx];
	DrvSound_DmpOutDevConfig();
	if (FALSE == pAudPlayObj->isOpened()) {
		DBG_ERR("idx=%d,pAudPlayObj Not opened,Open now!\r\n", __func__, outDevIdx);
		pAudPlayObj->open();
	}

	//[Config audio Codec & Output]
	//Speaker is connected to IC Speaker Output
	//TV Audio is connected to IC HP_R Output
	if (gpCurDrvAudOutDev->vidOutType == DRVSOUND_VOUT_LCD) {
		DBG_IND("video: device=LCD\r\n");
		aud_switchCodec(AUDIO_CODECSEL_DEFAULT);
		DBG_IND("audio: codec=DEFAULT device=LCD\r\n");
		aud_setOutput(AUDIO_OUTPUT_SPK); //or AUDIO_OUTPUT_MONO
		aud_setFeature(AUDIO_FEATURE_LINE_PWR_ALWAYSON, FALSE);
		aud_setFeature(AUDIO_FEATURE_INTERFACE_ALWAYS_ACTIVE, FALSE);
		aud_setFeature(AUDIO_FEATURE_DISCONNECT_HDMI, TRUE);
		aud_setFeature(AUDIO_FEATURE_PLAY2ALLCODEC, FALSE);
	} else if (gpCurDrvAudOutDev->vidOutType == DRVSOUND_VOUT_TV) {
		DBG_IND("video: device=TV\r\n");
		DBG_IND("audio: codec=DEFAULT device=LINE\r\n");
		aud_switchCodec(AUDIO_CODECSEL_DEFAULT);
		aud_setOutput(AUDIO_OUTPUT_LINE); //or AUDIO_OUTPUT_HP
		aud_setFeature(AUDIO_FEATURE_LINE_PWR_ALWAYSON, TRUE);
		aud_setFeature(AUDIO_FEATURE_INTERFACE_ALWAYS_ACTIVE, FALSE);
		aud_setFeature(AUDIO_FEATURE_DISCONNECT_HDMI, TRUE);
		aud_setFeature(AUDIO_FEATURE_PLAY2ALLCODEC, FALSE);
	} else if (gpCurDrvAudOutDev->vidOutType == DRVSOUND_VOUT_HDMI) {
		DBG_IND("video: device=HDMI\r\n");
		DBG_IND("audio => codec=HDMI device=NONE\r\n");
		aud_switchCodec(AUDIO_CODECSEL_HDMI);
		aud_setOutput(AUDIO_OUTPUT_NONE);
		aud_setFeature(AUDIO_FEATURE_LINE_PWR_ALWAYSON, FALSE);
		aud_setFeature(AUDIO_FEATURE_INTERFACE_ALWAYS_ACTIVE, TRUE);
		aud_setFeature(AUDIO_FEATURE_DISCONNECT_HDMI,  FALSE);
		aud_setFeature(AUDIO_FEATURE_PLAY2ALLCODEC, FALSE);
		// 後續 aud_setSamplingRate() 設定的 sampling rate 須等同 HDMI display object 端設定的 sampling rate
		// 不然需要用 aud_setResampleInfo() 啟動 audio resample
	} else if (gpCurDrvAudOutDev->vidOutType == DRVSOUND_VOUT_LCDTV) {
		DBG_IND("video: device=LCD & TV\r\n");
		DBG_IND("audio: codec=DEFAULT device=SPK\r\n");
		aud_switchCodec(AUDIO_CODECSEL_DEFAULT);
		aud_setOutput(AUDIO_OUTPUT_LINE);
		aud_setFeature(AUDIO_FEATURE_LINE_PWR_ALWAYSON, TRUE);
		aud_setFeature(AUDIO_FEATURE_INTERFACE_ALWAYS_ACTIVE, FALSE);
		aud_setFeature(AUDIO_FEATURE_DISCONNECT_HDMI, TRUE);
		aud_setFeature(AUDIO_FEATURE_PLAY2ALLCODEC, FALSE);
	} else if (gpCurDrvAudOutDev->vidOutType == DRVSOUND_VOUT_LCDHDMI) {
		DBG_IND("video: device=LCD & HDMI\r\n");
		DBG_IND("audio: codec=DEFAULT device=SPK\r\n");
		aud_switchCodec(AUDIO_CODECSEL_DEFAULT);
		aud_setOutput(AUDIO_OUTPUT_SPK);
		aud_setFeature(AUDIO_FEATURE_LINE_PWR_ALWAYSON, FALSE);
		aud_setFeature(AUDIO_FEATURE_INTERFACE_ALWAYS_ACTIVE, TRUE);
		aud_setFeature(AUDIO_FEATURE_DISCONNECT_HDMI, FALSE);
		aud_setFeature(AUDIO_FEATURE_PLAY2ALLCODEC, TRUE);
		// 後續 aud_setSamplingRate() 設定的 sampling rate 須等同 HDMI display object 端設定的 sampling rate
		// 不然需要用 aud_setResampleInfo() 啟動 audio resample
	}

	//[Config audio Channel]
	//TODO ==> GxSound:
	//  project can assign witch channel is for MONO sound play
	//  project can assign if expend MONO sound play as STEREO
	if (AUDIO_CH_STEREO == gpCurDrvAudOutDev->audChannel) {
		pAudPlayObj->setChannel(AUDIO_CH_STEREO);
		if (AUDTS_CH_RX == pAudPlayObj->AudTSCH) {
			pAudPlayObj->setFeature(AUDTS_FEATURE_RECORD_PCM_EXPAND, FALSE);
		} else {
			pAudPlayObj->setFeature(AUDTS_FEATURE_PLAYBACK_PCM_EXPAND, FALSE);
		}
	} else {
		pAudPlayObj->setChannel(gpCurDrvAudOutDev->audChannel);
		if (AUDTS_CH_RX == pAudPlayObj->AudTSCH) {
			pAudPlayObj->setFeature(AUDTS_FEATURE_RECORD_PCM_EXPAND, gpCurDrvAudOutDev->monoExpand);
		} else {
			pAudPlayObj->setFeature(AUDTS_FEATURE_PLAYBACK_PCM_EXPAND, gpCurDrvAudOutDev->monoExpand);
		}
	}

	DBG_IND(":idx=%d\r\n", outDevIdx);
	return retV;
}
UINT32 DrvSound_GetVol(void)
{
	return gCurDrvSndVol;
}
ER DrvSound_SetCodec(PAUDIO_CODEC_FUNC codecFunc, AUDIO_CODECSEL audCodecSel)
{
	ER retV = E_OK;

	//if (0 == aud_getLockStatus())
	{
		retV = aud_open();
		if (E_OK != retV) {
			DBG_ERR("aud_open fail =%d\r\n", retV);
			return retV;
		}
	}
	aud_setExtCodec(codecFunc);
	aud_switchCodec(audCodecSel);
	gDrvSndCodecFunc = codecFunc;
	gDrvSndCodecSel = audCodecSel;
	aud_close();
	DBG_IND("CodecFunc=0x%x,codecSel=%d\r\n", gDrvSndCodecFunc, gDrvSndCodecSel);
	return retV;
}
ER DrvSound_SetTotalVolLvl(AUDIO_VOL_LEVEL volLvl)
{
	ER retV = E_OK;

	//if (0 == aud_getLockStatus())
	{
		retV = aud_open();
		if (E_OK != retV) {
			DBG_ERR("aud_open fail =%d\r\n", retV);
			return retV;
		}
	}
	gDrvSndTotalVolLvl = volLvl;
	aud_setTotalVolLevel(volLvl);
	aud_close();
	DBG_IND("Cur=%d,total=%d\r\n", gCurDrvSndVol, gDrvSndTotalVolLvl);
	return retV;
}
// vol: 0 ~ 100
ER DrvSound_SetVol(UINT32 vol)
{
	ER retV = E_OK;
	AUDIO_VOL_LEVEL maxVolLvl = 0;
	UINT32 tmpVol = 0;
	UINT32 openAud = 0;

	DbgMsg_DrvSoundIO(("+%s:vol=%d\r\n", __func__, vol));

	//if (0 == aud_getLockStatus())
	{
		retV = aud_open();
		if (E_OK == retV) {
			openAud = TRUE;
		} else {
			return retV;
		}
	}
	gCurDrvSndVol = vol;
	if (vol == 0) {
		aud_setVolume(AUDIO_VOL_MUTE);
	} else {
		maxVolLvl = aud_getTotalVolLevel();
		tmpVol = (vol * maxVolLvl) / 100;
		tmpVol = (tmpVol < maxVolLvl) ? (tmpVol + 1) : tmpVol;
		DbgMsg_DrvSoundIO(("%s:Cal=%d,max=%d,set=%d\r\n", __func__, gCurDrvSndVol, maxVolLvl, tmpVol));
		if (maxVolLvl == AUDIO_VOL_LEVEL8) {
			if (tmpVol >= AUDIO_VOL_LEVEL8) {
				tmpVol = AUDIO_VOL_7;
			}
		} else {
			if (tmpVol >= AUDIO_VOL_LEVEL64) {
				tmpVol = AUDIO_VOL_63;
			}
		}
		aud_setVolume(tmpVol);
	}
	if (openAud == TRUE) {
		aud_close();
	}
	DbgMsg_DrvSoundIO(("-%s:finalVol=%d,max=%d,set=%d,opened=%d\r\n", __func__, gCurDrvSndVol, maxVolLvl, tmpVol, openAud));
	return retV;
}

ER DrvSound_SetSR(UINT32 audSR, PAUDTS_OBJ pAudPlayObj)
{
	ER retV = E_OK;
	DbgMsg_DrvSoundIO(("+%s:sr=%d, audObj=0x%x\r\n", __func__, audSR, pAudPlayObj));

	if (0 == pAudPlayObj) {
		DBG_ERR("No AudObj\r\n");
		return E_PAR;
	}
	if (DRVSOUND_SR_8000 >= audSR) {
		pAudPlayObj->setSamplingRate(AUDIO_SR_8000);
	} else if (DRVSOUND_SR_11025 >= audSR) {
		pAudPlayObj->setSamplingRate(AUDIO_SR_11025);
	} else if (DRVSOUND_SR_12000 >= audSR) {
		pAudPlayObj->setSamplingRate(AUDIO_SR_12000);
	} else if (DRVSOUND_SR_16000 >= audSR) {
		pAudPlayObj->setSamplingRate(AUDIO_SR_16000);
	} else if (DRVSOUND_SR_22050 >= audSR) {
		pAudPlayObj->setSamplingRate(AUDIO_SR_22050);
	} else if (DRVSOUND_SR_24000 >= audSR) {
		pAudPlayObj->setSamplingRate(AUDIO_SR_24000);
	} else if (DRVSOUND_SR_32000 >= audSR) {
		pAudPlayObj->setSamplingRate(AUDIO_SR_32000);
	} else if (DRVSOUND_SR_44100 >= audSR) {
		pAudPlayObj->setSamplingRate(AUDIO_SR_44100);
	} else {
		pAudPlayObj->setSamplingRate(AUDIO_SR_48000);
	}
	DbgMsg_DrvSoundIO(("-%s:sr=%d\r\n", __func__, audSR));
	return retV;
}
ER DrvSound_SetCh(PDXSOUND_AUD_CH pDxSndAudCh, PAUDTS_OBJ pAudObj)
{
	AUDIO_CH audCh = pDxSndAudCh->audch;
	UINT32 monoExpand = pDxSndAudCh->monoExpand;
	ER retV = E_OK;
	DbgMsg_DrvSoundIO(("+%s:ch=%d,isMono=%d\r\n", __func__, audCh, monoExpand));

	if (0 == pAudObj) {
		DBG_ERR("pAudPlayObj NULL\r\n");
		return E_PAR;
	} else if (FALSE == pAudObj->isOpened()) {
		DBG_ERR("pAudPlayObj NOT Opened\r\n");
		return E_OACV;
	}
	//aud_setChannel(audCh);
	pAudObj->setChannel(audCh);
	if (AUDIO_CH_STEREO == audCh) {
		if (AUDTS_CH_RX == pAudObj->AudTSCH) {
			pAudObj->setFeature(AUDTS_FEATURE_RECORD_PCM_EXPAND, FALSE);
		} else {
			pAudObj->setFeature(AUDTS_FEATURE_PLAYBACK_PCM_EXPAND, FALSE);
		}

	} else {
		if (AUDTS_CH_RX == pAudObj->AudTSCH) {
			pAudObj->setFeature(AUDTS_FEATURE_RECORD_PCM_EXPAND, monoExpand);
		} else {
			pAudObj->setFeature(AUDTS_FEATURE_PLAYBACK_PCM_EXPAND, monoExpand);
		}

	}
	DbgMsg_DrvSoundIO(("-%s:ch=%d,isMono=%d\r\n", __func__, audCh, monoExpand));
	return retV;
}

//#NT#2013/03/06#Lily Kao -end

/**
  Detect Audio is plugging in or unplugged

  Detect Audio is plugging in or unplugged.
  Return TRUE if Audio is plugging in, FALSE if Audio is unplugged.

  @param void
  @return BOOL: TRUE -> Audio is plugging in, FALSE -> Audio is unplugged
*/
/*
BOOL GPIOMap_DetAudio(void)
{
    return FALSE;
}
*/

/**
  Detect which kind of Audio Output Device is plugging in or unplugged
  No Sample code for detecting now
  @param void
  @return AUDIO_OUTPUT
        AUDIO_OUTPUT_SPK,           ///< Speaker
        AUDIO_OUTPUT_HP,            ///< Headphone
        AUDIO_OUTPUT_LINE,          ///< Line Out
        AUDIO_OUTPUT_MONO           ///< Mono Out
*/
AUDIO_OUTPUT GPIOMap_DetAudioOutDevice(void)
{
#if (AIN_DET_CTRL == AIN_DET_DUMMY)
	return AUDIO_OUTPUT_SPK;
#endif
#if ((AIN_DET_CTRL == AIN_DET_BY_VIO) && (USE_VIO == ENABLE))
	return (vio_getpin(VIO_AIN_DET) != 0) ? AUDIO_OUTPUT_LINE : AUDIO_OUTPUT_SPK;
#endif
#if (AIN_DET_CTRL == AIN_DET_BY_GPIO)
	return (gpio_getPin(GPIO_AIN_PLUG) == 0) ? AUDIO_OUTPUT_SPK : AUDIO_OUTPUT_LINE;
#endif
}
//XXXXXXXXXXXXXXX 2009/06/12 Lily Kao -begin
/*
void GPIOMap_DetSetAudioOutDevice(void)
{
    AUDIO_OUTPUT audiOut = GPIOMap_DetAudioOutDevice();

    GxSoud_SetAudioOutDevice(audiOut);
    return;
}
*/
//XXXXXXXXXXXXXXX 2009/06/12 Lily Kao -end

/**
  Turn on External Amplifier

  If bEn == TRUE, Turn on External Amplifier
  If bEn == FALSE, Turn off External Amplifier(mute).

  @param BOOL bEn: TRUE -> turn on, FALSE -> turn off
  @return void
*/
#if 0
void GPIOMap_TurnOnExtrAmp(BOOL bEn)
{
	DBG_IND("++GPIOMap_TurnOnExtrAmp:%x\r\n", bEn);
	if (bEn == TRUE) {
		//gpio_setPin(xxx);
	} else {
		//gpio_clearPin(xxx);
	}
	DBG_IND("--GPIOMap_TurnOnExtrAmp:%x\r\n", bEn);
}
#endif


#if 1//( _AUDIOCODEC_EXT_ == _AUDIOCODEC_EXT_NONE_)//no external codec, use embeded codec
void DrvSound_SetAudioDevInitData(AUDIO_DEVICE_OBJ  *pAudioDevice, AUDIO_SETTING *pAudioSetting)
{
	DBG_IND("++DrvSound_SetAudioDevInitData:Dev=0x%x, Set=0x%x\r\n", pAudioDevice, pAudioSetting);
	if (pAudioDevice) {
		//pAudioDevice->pEventHandler       = NULL;

		pAudioDevice->uiGPIOColdReset     = 0;

		//pAudioDevice->uiI2SCtrl           = AUDIO_I2SCTRL_NONE;
		pAudioDevice->uiI2SCtrl           = AUDIO_I2SCTRL_GPIO_I2C;

		pAudioDevice->uiChannel           = 0;

		//pAudioDevice->uiGPIOData          = 0;
		pAudioDevice->uiGPIOData          = P_GPIO_0;

		//pAudioDevice->uiGPIOClk           = 0;
		pAudioDevice->uiGPIOClk           = P_GPIO_2;

#if 1 //FPGA_HDMI
		pAudioDevice->uiI2SCtrl           = AUDIO_I2SCTRL_NONE;
		pAudioDevice->uiChannel           = 0;
		pAudioDevice->uiGPIOData          = 0;
		pAudioDevice->uiGPIOClk           = 0;
#endif

		pAudioDevice->uiGPIOCS            = 0;
		pAudioDevice->uiADCZero           = 0;
	}
	if (pAudioSetting) {
		pAudioSetting->Clock.bClkExt      = FALSE;                        // Must be
		pAudioSetting->Clock.Clk          = AUDIO_CLK_12288;              // Clock = Sampling rate * 256
		pAudioSetting->I2S.bMaster        = TRUE;                         // Must be
		pAudioSetting->I2S.I2SFmt         = AUDIO_I2SFMT_STANDARD;        // Must be
		pAudioSetting->I2S.ClkRatio       = AUDIO_I2SCLKR_256FS_32BIT;    // Must be
		pAudioSetting->Fmt                = AUDIO_FMT_I2S;                // Must be
		pAudioSetting->SamplingRate       = AUDIO_SR_48000;               // Sampling rate = Clock / 256
		pAudioSetting->Channel            = AUDIO_CH_STEREO;;//AUDIO_CH_RIGHT;               // Audio channel
		pAudioSetting->RecSrc             = AUDIO_RECSRC_MIC;             // Must be
		pAudioSetting->Output             = GPIOMap_DetAudioOutDevice(); //AUDIO_OUTPUT_SPK;
		pAudioSetting->bEmbedded          = TRUE;                         // Must be, if you want to use the audio I/O pin
	}
	DBG_IND("--DrvSound_DefaultAUdioSettingData\r\n");
}
#else//#elif( _AUDIOCODEC_EXT_ == _AUDIOCODEC_EXT_WM8978_)//init for WM8978
void DrvSound_SetAudioDevInitData(AUDIO_DEVICE_OBJ  *pAudioDevice, AUDIO_SETTING *pAudioSetting)
{
	AUDIO_CODEC_FUNC AudCodecFunc;
	DBG_IND("++DrvSound_SetAudioDevInitData:Dev=0x%x, Set=0x%x\r\n", pAudioDevice, pAudioSetting);
	if (pAudioDevice) {
		//pAudioDevice->pEventHandler       = NULL;
		pAudioDevice->uiGPIOColdReset     = 0;
		pAudioDevice->uiI2SCtrl           = AUDIO_I2SCTRL_GPIO_I2C;
		pAudioDevice->uiChannel           = 0;
		pAudioDevice->uiGPIOData          = P_GPIO_17;//P_GPIO_28;
		pAudioDevice->uiGPIOClk           = P_GPIO_18;//P_GPIO_29;
		pAudioDevice->uiGPIOCS            = 0;
		pAudioDevice->uiADCZero           = 0;
	}
	if (pAudioSetting) {
		pAudioSetting->Clock.bClkExt      = FALSE;                        // Must be
		pAudioSetting->I2S.bMaster        = TRUE;                         // Must be
		pAudioSetting->I2S.I2SFmt         = AUDIO_I2SFMT_STANDARD;        // Must be
		pAudioSetting->I2S.ClkRatio       = AUDIO_I2SCLKR_256FS_32BIT;    // Must be
		pAudioSetting->Fmt                = AUDIO_FMT_I2S;                // Must be
		pAudioSetting->Clock.Clk          = AUDIO_CLK_12288;              // Clock = Sampling rate * 256
		pAudioSetting->SamplingRate       = AUDIO_SR_48000;               // Sampling rate = Clock / 256
		pAudioSetting->Channel            = AUDIO_CH_RIGHT;               // Audio channel
		pAudioSetting->RecSrc             = AUDIO_RECSRC_MIC;             // Must be
		pAudioSetting->Output             = AUDIO_OUTPUT_SPK;             // Output source
		pAudioSetting->bEmbedded          = TRUE;                         // Must be. (Don't care in audio lib)
	}
	audExtCodec_getFP(&AudCodecFunc);
	aud_setExtCodec(&AudCodecFunc);
	DBG_IND("--DrvSound_DefaultAUdioSettingData\r\n");
}
//#else
void DrvSound_SetAudioDevInitData(AUDIO_DEVICE_OBJ  *pAudioDevice, AUDIO_SETTING *pAudioSetting)
{
	DBG_FATAL("AUDIO SETTING ERROR!\r\n");
}
#endif


void DrvSound_SetAudioFeature(void)
{
	DBG_IND("++\r\n");

#if 1
	aud_setDefaultSettings(AUDIO_DEFSET_20DB);
#else
	//
	// Note: These default audio recoding setting is good for reference board.
	//       For audio recording quaility fine-tuning request, please help to connect Novatek Engineer.
	// Enable ALC/Noise Gate function
	aud_setFeature(AUDIO_FEATURE_ALC,                                                  TRUE);//HDMI test
	aud_setFeature(AUDIO_FEATURE_NG_WITH_MICBOOST,                                    FALSE);
	aud_setParameter(AUDIO_PARAMETER_NOISETHD_WITH_BOOST,         EAC_NG_THRESHOLD_N58P5_DB);
	aud_setParameter(AUDIO_PARAMETER_ALC_MAXGAIN,                  EAC_ALC_MAXGAIN_P21P0_DB);
	aud_setFeature(AUDIO_FEATURE_MICBOOST,                                             TRUE);
	aud_setParameter(AUDIO_PARAMETER_NOISEGAIN,                                         0x1);
	aud_setParameter(AUDIO_PARAMETER_NOISEGATE_TIME_RESOLUTION, EAC_ALC_TRESO_BASIS_45000US);
	aud_setParameter(AUDIO_PARAMETER_ALC_TIME_RESOLUTION,        EAC_ALC_TRESO_BASIS_5000US);
	aud_setParameter(AUDIO_PARAMETER_ALC_ATTACK_TIME,                                   0x4);
	aud_setParameter(AUDIO_PARAMETER_ALC_DECAY_TIME,                                    0x5);
	aud_setParameter(AUDIO_PARAMETER_ALCNG_ATTACK_TIME,                                 0x6);
	aud_setParameter(AUDIO_PARAMETER_ALCNG_DECAY_TIME,                                  0x3);
#endif
	aud_setTotalVolLevel(gDrvSndTotalAudVolLvl);
	aud_setVolume(gCurDrvSndVol);
	aud_setGain(gCurDrvSndGain);

	DBG_IND("--\r\n");
}

//#NT#2013/06/14#Calvin Chang#Get HDMI sample rate -begin
UINT32 DrvSound_GetHDMIAudSR(void)
{
	UINT32        audsr;
	HDMI_AUDIOFMT hdmisr;

	hdmisr = hdmitx_getConfig(HDMI_CONFIG_ID_AUDIO);

	switch (hdmisr) {
	case HDMI_AUDIO32KHZ:
		audsr = 32000;
		break;
	case HDMI_AUDIO44_1KHZ:
		audsr = 44100;
		break;
	case HDMI_AUDIO48KHZ:
		audsr = 48000;
		break;
	default:
		DBG_ERR("hdmi sample is %d\r\n", hdmisr);
		audsr = 0;
		break;
	}

	return  audsr;

}
//#NT#2013/06/14#Calvin Chang -end

// vol: 0 ~ 100
ER DrvSound_SetAudGain(UINT32 vol)
{
	//AUDIO_DEVICE_OBJ AudioDevObj;
	BOOL             bAudioALCEn = TRUE;
	UINT32           uAudGain    = AUDIO_GAIN_MUTE;
	UINT32           ustep       = 0;
	UINT32           upercgain   = 0;

	// Map the percentage of volume to Audio Gain
	switch (vol) {
	case 0: { //0% (Mute)
			bAudioALCEn = FALSE;
			uAudGain    = AUDIO_GAIN_MUTE;
		}
		break;

	case 100: { //100% (Maximum)
			uAudGain = AUDIO_GAIN_7;
		}
		break;

	default: {
			ustep = (100 / (UINT32)AUDIO_GAIN_7); // AUDIO_GAIN_7 is maximum value

			upercgain = ustep;

			while (upercgain < vol) {
				upercgain += ustep;
				uAudGain++;
			}
		}
		break;
	}

	if (aud_getLockStatus()) {
		aud_setFeature(AUDIO_FEATURE_ALC, bAudioALCEn);
		aud_setGain(uAudGain);
	} else {
		if (E_OK == aud_open()) {
			aud_setFeature(AUDIO_FEATURE_ALC, bAudioALCEn);
			aud_setGain(uAudGain);
			aud_close();
		} else {
			DBG_ERR("aud_open fail %d\r\n");
			return E_PAR;
		}
	}

	return E_OK;

}

ER DrvSound_SetSR_Resample(PDXSOUND_AUD_RESAMPLE_INFO pAudResampleInfo, PAUDTS_OBJ pAudObj)
{
	ER                   retV = E_OK;
	AUDIO_RESAMPLE_INFO  ResampleInfo = {0};
	BOOL                 bResampleEnable = FALSE;
	AUDIO_SR             uiTargetAudRate;
	HDMI_AUDIOFMT        uiHDMIAudRate;
	UINT32 audSR = pAudResampleInfo->audSR;
	UINT32 uiWorkBufAddr = pAudResampleInfo->uiWorkBufAddr;
	UINT32 uiWorkBufSize = pAudResampleInfo->uiWorkBufSize;

	DbgMsg_DrvSoundIO(("+%s:sr=%d\r\n", __func__, audSR));
	//DbgCode_DrvSound(if (aud_getLockStatus() == NO_TASK_LOCKED) DBG_ERR("%s: AudDrv not open\r\n",__func__);)

	// Set Audio
	if (DRVSOUND_SR_8000 >= audSR) {
		pAudObj->setSamplingRate(AUDIO_SR_8000);
	} else if (DRVSOUND_SR_11025 >= audSR) {
		pAudObj->setSamplingRate(AUDIO_SR_11025);
	} else if (DRVSOUND_SR_12000 >= audSR) {
		pAudObj->setSamplingRate(AUDIO_SR_12000);
	} else if (DRVSOUND_SR_16000 >= audSR) {
		pAudObj->setSamplingRate(AUDIO_SR_16000);
	} else if (DRVSOUND_SR_22050 >= audSR) {
		pAudObj->setSamplingRate(AUDIO_SR_22050);
	} else if (DRVSOUND_SR_24000 >= audSR) {
		pAudObj->setSamplingRate(AUDIO_SR_24000);
	} else if (DRVSOUND_SR_32000 >= audSR) {
		pAudObj->setSamplingRate(AUDIO_SR_32000);
	} else if (DRVSOUND_SR_44100 >= audSR) {
		pAudObj->setSamplingRate(AUDIO_SR_44100);
	} else {
		pAudObj->setSamplingRate(AUDIO_SR_48000);
	}
	DbgMsg_DrvSoundIO(("-%s:sr=%d\r\n", __func__, audSR));

	// Set HDMI & Resample
	switch (audSR) {
	case DRVSOUND_SR_8000: {
			bResampleEnable = TRUE;
			uiTargetAudRate = AUDIO_SR_32000;
			uiHDMIAudRate   = HDMI_AUDIO32KHZ;
			break;
		}

	case DRVSOUND_SR_11025: {
			bResampleEnable = TRUE;
			uiTargetAudRate = AUDIO_SR_44100;
			uiHDMIAudRate   = HDMI_AUDIO44_1KHZ;
			break;
		}

	case DRVSOUND_SR_12000: {
			bResampleEnable = TRUE;
			uiTargetAudRate = AUDIO_SR_32000;
			uiHDMIAudRate   = HDMI_AUDIO32KHZ;
			break;
		}

	case DRVSOUND_SR_16000: {
			bResampleEnable = TRUE;
			uiTargetAudRate = AUDIO_SR_32000;
			uiHDMIAudRate   = HDMI_AUDIO32KHZ;
			break;
		}

	case DRVSOUND_SR_22050: {
			bResampleEnable = TRUE;
			uiTargetAudRate = AUDIO_SR_44100;
			uiHDMIAudRate   = HDMI_AUDIO44_1KHZ;
			break;
		}

	case DRVSOUND_SR_24000: {
			bResampleEnable = TRUE;
			uiTargetAudRate = AUDIO_SR_48000;
			uiHDMIAudRate   = HDMI_AUDIO48KHZ;
			break;
		}

	case DRVSOUND_SR_32000: {
			bResampleEnable = FALSE;
			uiTargetAudRate = AUDIO_SR_32000;
			uiHDMIAudRate   = HDMI_AUDIO32KHZ;
			break;
		}

	case DRVSOUND_SR_44100: {
			bResampleEnable = FALSE;
			uiTargetAudRate = AUDIO_SR_44100;
			uiHDMIAudRate   = HDMI_AUDIO44_1KHZ;
			break;
		}

	default:
	case DRVSOUND_SR_48000: {
			bResampleEnable = FALSE;
			uiTargetAudRate = AUDIO_SR_48000;
			uiHDMIAudRate   = HDMI_AUDIO48KHZ;
			break;
		}
	}

	if (uiWorkBufSize < (uiTargetAudRate * 4 * 4)) {
		DBG_ERR("%s Buffer size is not enough!\r\n", __func__);
		return E_SYS;
	}

	// Set HDMI audio rate
	//if(hdmitx_open())
	{
		hdmitx_setConfig(HDMI_CONFIG_ID_AUDIO, uiHDMIAudRate);
	}

	// Set Resample
	ResampleInfo.bResample     = bResampleEnable;
	ResampleInfo.AudioTargetSR = uiTargetAudRate;
	ResampleInfo.uiDstBufAddr  = uiWorkBufAddr;
	ResampleInfo.uiDstBufSize  = uiWorkBufSize;

	//aud_setResampleInfo(&ResampleInfo);
	if (FALSE == pAudObj->setResampleInfo(&ResampleInfo)) {
		DBG_IND("aud resample false\r\n");
	}

	return retV;
}

//========================================================
UINT32 DrvSound_Init(void *pInitParam);
UINT32 DrvSound_Open(void);
UINT32 DrvSound_Close(void);;
UINT32 DrvSound_Getcaps(UINT32 CapID, UINT32 Param1);
UINT32 DrvSound_Setcfgs(UINT32 CfgID, UINT32 Param1);
UINT32 DrvSound_Command(CHAR *pcCmdStr);
UINT32 DrvSound_Control(UINT32 CtrlID, UINT32 Param1, UINT32 Param2);
UINT32 DrvSound_State(UINT32 StateID, UINT32 Value);

DX_OBJECT gDevSnd = {
	DXFLAG_SIGN,
	DX_CLASS_AUDIO_EXT,
	DXSOUND_VER,
	"DrvExt Sound",
	0, 0, 0, 0,
	DrvSound_Getcaps,
	DrvSound_Setcfgs,
	DrvSound_Init,
	DrvSound_Open,
	DrvSound_Close,
	DrvSound_State,
	DrvSound_Control,
	DrvSound_Command,
	0,
};

UINT32 DrvSound_Init(void *pInitParam)
{
	ER retV = aud_open();
	DBG_IND("+-:0x%x\r\n", pInitParam);

	DrvSound_SetAudioDevInitData(NULL, (AUDIO_SETTING *)pInitParam);
	aud_init((AUDIO_SETTING *)pInitParam);
	if (retV != E_OK) {
		DBG_ERR("aud_open fail = %d\r\n", retV);
		return retV;
	}
	DrvSound_SetAudioFeature();
	aud_close();
	return DX_OK;
}

UINT32 DrvSound_Open(void)
{
	DBG_IND("+-\r\n");
	return DX_OK;
}

UINT32 DrvSound_Close(void)
{
	DBG_IND("+-\r\n");
	return DX_OK;
}
UINT32 DrvSound_Getcaps(UINT32 CapID, UINT32 Param1)
{
	UINT32 retV = 0;
	DBG_IND(":CapID=%d,Param1=%d+\r\n", CapID, Param1);
	switch (CapID) {
	case DXSOUND_CAP_VOLUME:
		retV = DrvSound_GetVol();
		break;
	case DXSOUND_CAP_GET_HDMI_SAMPLERATE:
		retV = DrvSound_GetHDMIAudSR();
		break;
	case DXSOUND_CAP_GET_AUDOUTDEV:
		retV = (UINT32)(GPIOMap_DetAudioOutDevice() == AUDIO_OUTPUT_LINE);
		break;
	case DXSOUND_CAP_SAMPLERATE:
	case DXSOUND_CAP_CHANNEL:
	case DXSOUND_CAP_GAIN:
	default:
		break;
	}
	DBG_IND(":CapID=%d,Param1=%d,v=%d-\r\n", CapID, Param1, retV);
	return retV;
}
UINT32 DrvSound_Setcfgs(UINT32 CfgID, UINT32 Param1)
{
	DBG_IND("+-\r\n");
	return DX_OK;
}
UINT32 DrvSound_Command(CHAR *pcCmdStr)
{
	DBG_IND("+-\r\n");
	switch (*pcCmdStr) {
	case 'd':
		/*if (!strncmp(pcCmdStr, "disp tv dump", 12))
		{
		    ...
		    return TRUE;
		}*/
		break;
#if (USE_VIO == ENABLE)
	case 'v':
		if (!strncmp(pcCmdStr, "vdet", 4)) {
			char ts[10];
			UINT32 v1;
			sscanf_s(pcCmdStr, "%s %lu", ts, sizeof(ts), &v1); //fix for CID 44013
			DBG_IND("v1 = %d\r\n", v1);
			vio_setpin(VIO_AIN_DET, v1);
			return TRUE;
		}
		break;
#endif
	}
	return FALSE;
}

UINT32 DrvSound_Control(UINT32 CtrlID, UINT32 Param1, UINT32 Param2)
{
	UINT32 retV = DX_OK;
	DBG_IND(":CtrlID=%d,P1=0x%x,P2=0x%x+\r\n", CtrlID, Param1, Param2);
	switch (CtrlID) {
	case DXSOUND_CAP_SET_DEVINITDATA:
		DrvSound_SetAudioDevInitData((AUDIO_DEVICE_OBJ *)Param1, (AUDIO_SETTING *)Param2);
		//if (0 == aud_getLockStatus())
		{
			retV = aud_open();
			if (E_OK != retV) {
				DBG_ERR("aud_open fail =%d\r\n", retV);
				return retV;
			}
		}
		//aud_init((AUDIO_SETTING *)Param2);
		aud_setDeviceObject((AUDIO_DEVICE_OBJ *)Param1);
		aud_close();
		break;
	case DXSOUND_CAP_SET_CODEC:
		DrvSound_SetCodec((PAUDIO_CODEC_FUNC)Param1, (AUDIO_CODECSEL)Param2);
		break;
	case DXSOUND_CAP_SET_TOTAL_VOLUME_LEVEL:
		DrvSound_SetTotalVolLvl((AUDIO_VOL_LEVEL)Param1);
		break;
	case DXSOUND_CAP_VOLUME:
		retV = (UINT32)DrvSound_SetVol(Param1);
		break;
	case DXSOUND_CAP_SAMPLERATE:
		retV = (UINT32)DrvSound_SetSR(Param1, (PAUDTS_OBJ)Param2);
		break;
	case DXSOUND_CAP_CHANNEL:
		retV = (UINT32)DrvSound_SetCh((PDXSOUND_AUD_CH)Param1, (PAUDTS_OBJ)Param2);
		break;
	case DXSOUND_CAP_GAIN:
		retV = (UINT32)DrvSound_SetAudGain(Param1);
		break;
	case DXSOUND_CAP_RESAMPLE:
		DrvSound_SetSR_Resample((PDXSOUND_AUD_RESAMPLE_INFO)Param1, (PAUDTS_OBJ)Param2);
		break;
	case DXSOUND_CAP_SET_OUTDEV:
		DrvSound_SetOutDevConfig(Param1, (PAUDTS_OBJ)Param2);
		break;
	default:
		break;
	}
	DBG_IND(":CtrlID=%d,P1=%d,P2=%d-\r\n", CtrlID, Param1, Param2);
	return retV;
}

UINT32 DrvSound_State(UINT32 StateID, UINT32 Value) // General Properties
{
	DBG_IND("+-\r\n");
	return DX_OK;
}

//=============

//XXXXXXXXXXXXXXX 2009/06/16 Lily Kao -end

