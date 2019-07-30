/*
    External AIC3254 audio codec driver

    This file is the driver for TI AIC3254 extended audio codec.

    @file       AIC3254.c
    @ingroup    mISYSAud
    @note       Nothing.

    Copyright   Novatek Microelectronics Corp. 2013.  All rights reserved.
*/

/** \addtogroup mISYSAud */
//@{

#include "AIC3254.h"
#include "pll.h"
#include "sif.h"
#include "i2c.h"
#include "gpio.h"
#include "top.h"
#include "Utility.h"
#include "dai.h"
#include "string.h"
#include "KerCPU.h"
#include "Debug.h"

static AUDIO_SR             audioSR         = AUDIO_SR_8000;
static BOOL                 bChkPlySR       = FALSE;            // for feature AUDIO_FEATURE_CHECK_PLAY_SAMPLING_RATE
static AUDIO_SR             playCheckSR     = AUDIO_SR_32000;   // for parameter AUDIO_PARAMETER_CHECKED_PLAY_SAMPLING_RATE

static I2C_SESSION          gI2cSession;


static PAUDIO_DEVICE_OBJ    pAudioDeviceObj = NULL;
static AUDIO_OUTPUT         AudOutput       = AUDIO_OUTPUT_SPK;
static AUDIO_VOL            AudVolume       = AUDIO_VOL_6;  // playback volume
static AUDIO_GAIN           AudGain         = AUDIO_GAIN_6; // record gain
static BOOL                 bAudALC         = FALSE;

static AUDIO_CODEC_FUNC     AudCodecFP    = {
	AUDIO_CODEC_TYPE_EXTERNAL,

	// Set Device Object
	audcodec_setDeviceObject,
	// Init
	audcodec_init,
	// Set Record Source
	NULL,
	// Set Output
	audcodec_setOutput,
	// Set Sampling rate
	audcodec_setSamplingRate,
	// Set TxChannel
	audcodec_setChannel,
	// Set RxChannel
	audcodec_setChannel,
	// Set Volume (Playback)
	audcodec_setVolume,
	// Set Gain (Record)
	audcodec_setGain,
	// Set GainDB (Record)
	NULL,
	// Set Effect
	NULL,
	// Set Feature
	audcodec_setFeature,
	// Stop Record
	audcodec_stop,
	// Stop Playback
	audcodec_stop,
	// Record Preset
	NULL,
	// Record
	audcodec_record,
	// Playback
	audcodec_playback,
	// Set I2S Format
	audcodec_setFormat,
	// Set I2S clock ratio
	audcodec_setClockRatio,
	// Send command
	audcodec_sendCommand,
	// Read data
	audcodec_readData,
	// Set Parameter
	audcodec_setParameter,
	// Check Sampling rate
	audcodec_chkSamplingRate,
	// Open
	NULL,
	// Close
	NULL,
};

/**
    Get audio codec function pointer for external audio codec

    This function will return audio code function pointer for external AIC3254
    audio codec.

    @param[out]  pAudCodecFunc      Audio codec function pointer

    @return void
*/
void audExtAIC3254_getFP(PAUDIO_CODEC_FUNC pAudCodecFunc)
{
	memcpy((void *)pAudCodecFunc, (const void *)&AudCodecFP, sizeof(AUDIO_CODEC_FUNC));
}

/*
    Set the audio device object pointer of audio module

    This function will set the audio device object pointer of audio module.

    @param[in] pAudDevObj     device object of audio module

    @return void
*/
static void audcodec_setDeviceObject(PAUDIO_DEVICE_OBJ pAudDevObj)
{
	pAudioDeviceObj = pAudDevObj;
}

/*
    Initialize API for audio module

    This function is the initialize API for audio module
    It will set the following parameters if required:
        (1) Sampling Rate
        (2) Channel
        (3) Record Source
        (4) I2S Clock Ratio
        (5) I2S Format
        (6) Master/Slave Mode
        (7) Output Path
        (8) Other Codec related settings (Power on/down, Control interface...)

    @param[in] pAudio       Audio setting

    @return void
*/
static void audcodec_init(PAUDIO_SETTING pAudio)
{
	// Init SIF module or GPIO
	switch (pAudioDeviceObj->uiI2SCtrl) {
	case AUDIO_I2SCTRL_I2C:
		if (i2c_open(&gI2cSession) != E_OK) {
			debug_msg("%s: open i2c fail\r\n", __func__);
			return;
		}
		i2c_setConfig(gI2cSession, I2C_CONFIG_ID_MODE,        I2C_MODE_MASTER);
		i2c_setConfig(gI2cSession, I2C_CONFIG_ID_BUSCLOCK,    I2C_BUS_CLOCK_100KHZ);
		i2c_setConfig(gI2cSession, I2C_CONFIG_ID_PINMUX,      I2C_PINMUX_1ST);
		break;
	default:
		debug_msg("%s: unsupported control IF\r\n", __func__, pAudioDeviceObj->uiI2SCtrl);
		break;
	}

	// Reset external codec
	gpio_setDir(AIC3254_RESET_GPIO, GPIO_DIR_OUTPUT);
	gpio_clearPin(AIC3254_RESET_GPIO);
	Delay_DelayMs(100);
	gpio_setPin(AIC3254_RESET_GPIO);

	// Enable external amplifier
	gpio_setDir(TPA2010_SHUTDOWN_GPIO, GPIO_DIR_OUTPUT);
	gpio_clearPin(TPA2010_SHUTDOWN_GPIO);
	gpio_setPin(TPA2010_SHUTDOWN_GPIO);

	//
	// Record init
	//
	// init to page 0
	audcodec_sendCommand(0x00, 0x00);

	// SW reset
	audcodec_sendCommand(0x01, 0x01);

	// re-init to page 0???
//    audcodec_sendCommand(0x00, 0x00);

	// NADC = 1
	audcodec_sendCommand(0x12, 0x81);

	// MADC = 2
	audcodec_sendCommand(0x13, 0x82);

	// DAC OSR = 128
	audcodec_sendCommand(0x14, 0x80);

	// ADC PRB_R1
	audcodec_sendCommand(0x3D, 0x01);

	// select page 1
	audcodec_sendCommand(0x00, 0x01);

	// power up internal AVdd LDO
	audcodec_sendCommand(0x02, 0xA9);
	audcodec_sendCommand(0x01, 0x08);

	// enable master analog power control
	// DVDD/AVDD LDO = 1.77V, AVDD LDO power up
	audcodec_sendCommand(0x02, 0xA1);

	// input common mode = 0.9V
	audcodec_sendCommand(0x0A, 0x00);

	// Select ADC PTM_R4
	audcodec_sendCommand(0x3D, 0x00);

	// Set MicPGA startup delay = 3.1ms
	audcodec_sendCommand(0x47, 0x32);

	// Set REF charging time = 40ms
	audcodec_sendCommand(0x7B, 0x01);

	// Set MICBIAS = 2.5V (CM=0.9V), and from LDOIN
	audcodec_sendCommand(0x33, 0x68);

	// Route IN3L to LEFT_P with 20K input param
	audcodec_sendCommand(0x34, 0x08);

	// Route IN3_R Mode to LEFT_N with impedance to 20K
	audcodec_sendCommand(0x36, 0x08);

	// Route IN1R to RIGHT_P with impedance to 20K
	audcodec_sendCommand(0x37, 0x80);

	// Route IN1L to RIGHT_N with impedance to 20K
	audcodec_sendCommand(0x39, 0x20);

	//
	// Play init
	//
	// Select page 0
	audcodec_sendCommand(0x00, 0x00);

	// NDAC = 1
	audcodec_sendCommand(0x0B, 0x81);

	// MDAC = 2
	audcodec_sendCommand(0x0C, 0x82);

	// DAC OSR = 128
	audcodec_sendCommand(0x0D, 0x00);
	audcodec_sendCommand(0x0E, 0x80);

	// DAC PRB_P8
//    audcodec_sendCommand(0x3C, 0x08);

	// Select page 1
	audcodec_sendCommand(0x00, 0x01);

	// power up internal AVdd LDO
//    audcodec_sendCommand(0x01, 0x08);

	// enable master analog power control
	// DVDD/AVDD LDO = 1.77V, AVDD LDO power up
//    audcodec_sendCommand(0x02, 0xA1);

	// Set REF charging time = 40ms
//    audcodec_sendCommand(0x7B, 0x01);

//    audcodec_sendCommand(0x1B, 0x00);

	// HP soft steping for pop performance
	audcodec_sendCommand(0x14, 0x25);

	// input common mode = 0.9V
//    audcodec_sendCommand(0x0A, 0x00);

	// Route Left DAC to HPL (HPL is NC)
	audcodec_sendCommand(0x0C, 0x08);

	// Route Right DAC to HPL
	audcodec_sendCommand(0x0D, 0x08);

	// Route Left DAC to LOL
	audcodec_sendCommand(0x0E, 0x08);

	// Route Right DAC to LOR
	audcodec_sendCommand(0x0F, 0x08);

	// Set DAC PTM mode to PTM_P3/4
	audcodec_sendCommand(0x03, 0x00);
	audcodec_sendCommand(0x04, 0x00);

	// HPL gain = 0dB
	audcodec_sendCommand(0x10, 0x1D);
//    audcodec_sendCommand(0x10, 0x00);

	// HPR gain = 0dB
	audcodec_sendCommand(0x11, 0x1D);
//    audcodec_sendCommand(0x11, 0x00);

	// LOL gain = 5dB
	audcodec_sendCommand(0x12, 0x05);

	// LOR gain = 5dB
	audcodec_sendCommand(0x13, 0x05);

	// Power up HPL/HPR drivers
//    audcodec_sendCommand(0x09, 0x30);
	audcodec_sendCommand(0x09, 0x0C);

	// Select page 0
	audcodec_sendCommand(0x00, 0x00);

	// DAC gain = 0dB
	audcodec_sendCommand(0x41, 0x00);
	audcodec_sendCommand(0x42, 0x00);

	// Left DAI to left/right DAC
//    audcodec_sendCommand(0x3F, 0xD6);
	audcodec_sendCommand(0x3F, 0x94);

	// Unmute DAC
	audcodec_sendCommand(0x40, 0x80);
}

/*
    Set Output Path API for audio module

    This function is the Set Output Path API for audio module

    @param[in] Output       Output Path

    @return void
*/
static void audcodec_setOutput(AUDIO_OUTPUT Output)
{
	switch (Output) {
	default:
		debug_msg("%s: unsupported play output, force to SPK: 0x%lx\r\n", __func__, Output);
	// fall through
	case AUDIO_OUTPUT_SPK:
		AudOutput = AUDIO_OUTPUT_SPK;
		break;
	case AUDIO_OUTPUT_HP:
		// Change output immediately if DAC is enabled (Playback now)
		AudOutput = AUDIO_OUTPUT_HP;
		break;
	}
}

/*
    Set Sampling Rate API for audio module

    This function is the Set Sampling Rate API for audio module

    @param[in] SamplingRate     Sampling Rate

    @return void
*/
static void audcodec_setSamplingRate(AUDIO_SR SamplingRate)
{
	audioSR = SamplingRate;
#if 0
	pll_setClockRate(PLL_CLKSEL_ADO_CLKSEL, PLL_CLKSEL_ADO_CLKSEL_CLKMUX);

	switch (SamplingRate) {
	// 8 KHz
	case AUDIO_SR_8000:
		pll_setClockFreq(ADOCLK_FREQ, 2048000);
		break;

	// 11.025 KHz
	case AUDIO_SR_11025:
		pll_setClockFreq(ADOCLK_FREQ, 2822400);
		break;

	// 12 KHz
	case AUDIO_SR_12000:
		pll_setClockFreq(ADOCLK_FREQ, 3072000);
		break;

	// 16 KHz
	case AUDIO_SR_16000:
		pll_setClockFreq(ADOCLK_FREQ, 4096000);
		break;

	// 22.05 KHz
	case AUDIO_SR_22050:
		pll_setClockFreq(ADOCLK_FREQ, 5644800);
		break;

	// 24 KHz
	case AUDIO_SR_24000:
		pll_setClockFreq(ADOCLK_FREQ, 6144000);
		break;

	// 32 KHz
	case AUDIO_SR_32000:
		pll_setClockFreq(ADOCLK_FREQ, 8192000);
		break;

	// 44.1 KHz
	case AUDIO_SR_44100:
		pll_setClockFreq(ADOCLK_FREQ, 11289600);
		break;

	// 48 KHz
	default:
		pll_setClockFreq(ADOCLK_FREQ, 12288000);
		break;
	}
#endif
}

/*
    Set Channel API for audio module

    This function is the Set Channel API for audio module

    @param[in] Channel      Channel Mode
    @return void
*/
static void audcodec_setChannel(AUDIO_CH Channel)
{
	switch (Channel) {
	case AUDIO_CH_LEFT:
	default:
		break;

	case AUDIO_CH_RIGHT:
		break;
	}
}

/*
    Set PCM Volume API for audio module

    This function is the Set PCM Volume API for audio module

    @param[in] Vol      Volume

    @return void
*/
static void audcodec_setVolume(AUDIO_VOL Vol)
{
//    UINT32 uiPlayTargetVol;
//    const UINT32 uiPlayMaxVol = 0x30;

	if ((UINT32)Vol > (UINT32)aud_getTotalVolLevel()) {
		//coverity[mixed_enums]
		Vol = aud_getTotalVolLevel();
	}

	AudVolume = Vol;
#if 0
	// init to page 0
	audcodec_sendCommand(0x00, 0x00);
	if (Vol == AUDIO_VOL_MUTE) {
		// DAC mute
		audcodec_sendCommand(0x40, 0x0C);
	} else {
		uiPlayTargetVol = uiPlayMaxVol * (Vol - 1) / (aud_getTotalVolLevel() - 1);

		// Left DAC volume
		audcodec_sendCommand(0x41, uiPlayTargetVol);

		// Right DAC volume
		audcodec_sendCommand(0x42, uiPlayTargetVol);

		DBG_ERR("%s: vol 0x%lx, target 0x%lx\r\n", __func__, Vol, uiPlayTargetVol);

		// DAC unmute
		audcodec_sendCommand(0x40, 0x00);
	}
#endif
}

/*
    Set Record Gain API for audio module

    This function is the Set Record Gain API for audio module

    @param[in] Gain     Gain

    @return void
*/
static void audcodec_setGain(AUDIO_GAIN Gain)
{
	UINT32 uiRecrdTargetVol;
	const UINT32 uiRecordMinVol = 0x28;
	const UINT32 uiRecordMaxVol = 0x68;

	if (Gain > AUDIO_GAIN_7) {
		Gain = AUDIO_GAIN_7;
	}

	AudGain = Gain;

	uiRecrdTargetVol = (Gain - 1) * (uiRecordMaxVol - uiRecordMinVol) / (AUDIO_GAIN_7 - 1);
	uiRecrdTargetVol += uiRecordMinVol;

	// init to page 0
	audcodec_sendCommand(0x00, 0x00);

	// Left ADC volume
	audcodec_sendCommand(0x53, uiRecrdTargetVol);

	// Right ADC volume
	audcodec_sendCommand(0x54, uiRecrdTargetVol);

	debug_msg("%s: gain 0x%lx, target 0x%lx\r\n", __func__, Gain, uiRecrdTargetVol);
}

/*
    Set feature API for audio module

    This function is the Set Sound Effect API for audio module

    @param[in] Feature      Audio feature
    @param[in] bEnable      Enable/Disable the feature

    @return
        - @b FALSE: input feature not support
        - @b TRUE: input feature handle ok
*/
static BOOL audcodec_setFeature(AUDIO_FEATURE Feature, BOOL bEnable)
{
	switch (Feature) {
	case AUDIO_FEATURE_ALC:
		bAudALC = bEnable;
		break;
	case AUDIO_FEATURE_CHECK_PLAY_SAMPLING_RATE:
		bChkPlySR = bEnable;
		break;

	default:
		return FALSE;
	}

	return TRUE;
}

/*
    Stop API for audio module

    This function is the Stop API for audio module

    @return void
*/
static void audcodec_stop(void)
{
	// Playback
	//if (aud_isPlaybackMode() == TRUE)
	{

	}
	// Record
	//else
	{

	}
}

/*
    Record API for audio module

    This function is the Record API for audio module

    @return void
*/
static void audcodec_record(void)
{
#if 0
	// init to page 0
	audcodec_sendCommand(0x00, 0x00);

	// SW reset
	audcodec_sendCommand(0x01, 0x01);

	// re-init to page 0???
	audcodec_sendCommand(0x00, 0x00);

	// NADC = 1
	audcodec_sendCommand(0x12, 0x81);

	// MADC = 2
	audcodec_sendCommand(0x13, 0x82);

	// OSR = 128
	audcodec_sendCommand(0x14, 0x80);

	// ADC PRB_R1
	audcodec_sendCommand(0x3D, 0x01);

	// select page 1
	audcodec_sendCommand(0x00, 0x01);

	// power up internal AVdd LDO
	audcodec_sendCommand(0x01, 0x08);

	// enable master analog power control
	// DVDD/AVDD LDO = 1.77V, AVDD LDO power up
	audcodec_sendCommand(0x02, 0xA1);

	// input common mode = 0.9V
	audcodec_sendCommand(0x0A, 0x00);

	// Select ADC PTM_R4
	audcodec_sendCommand(0x3D, 0x00);

	// Set MicPGA startup delay = 3.1ms
	audcodec_sendCommand(0x47, 0x32);

	// Set REF charging time = 40ms
	audcodec_sendCommand(0x7B, 0x01);

	// Set MICBIAS = 2.5V (CM=0.9V), and from LDOIN
	audcodec_sendCommand(0x33, 0x68);

	// Route IN3L to LEFT_P with 20K input param
	audcodec_sendCommand(0x34, 0x08);

	// Route IN3_R Mode to LEFT_N with impedance to 20K
	audcodec_sendCommand(0x36, 0x08);

	// Route IN1R to RIGHT_P with impedance to 20K
	audcodec_sendCommand(0x37, 0x80);

	// Route IN1L to RIGHT_N with impedance to 20K
	audcodec_sendCommand(0x39, 0x20);
#endif
	// Unmute Left MICPGA
	audcodec_sendCommand(0x3B, 0x0C);

	// Unmute Right MICPGA
	audcodec_sendCommand(0x3C, 0x0C);

	// Select page 0
	audcodec_sendCommand(0x00, 0x00);

	// Power up left/right ADC
	audcodec_sendCommand(0x51, 0xC0);

	// Unmute Unmute Left/right ADC
	audcodec_sendCommand(0x52, 0x00);
}

/*
    Playback API for audio module

    This function is the Playback API for audio module

    @return void
*/
static void audcodec_playback(void)
{

}

/*
    Set I2S Format API for audio module

    This function is the Set I2S Format API for audio module

    @param[in] I2SFmt   I2S Format

    @return void
*/
static void audcodec_setFormat(AUDIO_I2SFMT I2SFmt)
{
	switch (I2SFmt) {
	default:
		debug_msg("%s: unsupported I2S fmt, force to standard I2S: 0x%lx\r\n", __func__, I2SFmt);
	case AUDIO_I2SFMT_STANDARD:
		break;
	}
}

/*
    Set I2S Clock Ratio API for audio module

    This function is the Set I2S Clock Ratio API for audio module

    @param[in] I2SCLKRatio      I2S Clock Ratio

    @return void
*/
static void audcodec_setClockRatio(AUDIO_I2SCLKR I2SCLKRatio)
{
	// Only support AUDIO_I2SCLKR_256FS_32BIT
	if (I2SCLKRatio != AUDIO_I2SCLKR_256FS_32BIT) {
		debug_msg("%s: clkRatio 0x%lx not support\r\n", __func__, I2SCLKRatio);
	}
}

/*
    Send command to audio codec

    This function is the Send command to audio codec API for audio module

    @param[in] uiReg        Codec register index
    @param[in] uiData       Register data

    @return
        - @b TRUE: success
        - @b FALSE: fail
*/
static BOOL audcodec_sendCommand(UINT32 uiReg, UINT32 uiData)
{
	switch (pAudioDeviceObj->uiI2SCtrl) {
	case AUDIO_I2SCTRL_I2C: {
			I2C_DATA i2cData;
			I2C_BYTE i2cByte[3];

			i2cData.VersionInfo = DRV_VER_96660;
			i2cData.pByte       = i2cByte;
			i2cData.ByteCount   = 3;
			i2cByte[0].uiValue  = AIC3254_I2C_SADDR;    // slave address of TVL320AIC3254
			i2cByte[0].Param    = I2C_BYTE_PARAM_START;
			i2cByte[1].uiValue  = uiReg;
			i2cByte[1].Param    = I2C_BYTE_PARAM_NONE;
			i2cByte[2].uiValue  = uiData;
			i2cByte[2].Param    = I2C_BYTE_PARAM_STOP;

			// Lock I2C
			i2c_lock(gI2cSession);

			if (i2c_transmit(&i2cData) != I2C_STS_OK) {
				debug_msg("i2c_transmit err\r\n");
			}

			// Unlock I2C
			i2c_unlock(gI2cSession);
		}
		return TRUE;
	default:
		return FALSE;
	}
}

/*
    Read data from audio codec

    This function is the Read data from codec API for audio module

    @param[in] uiReg        Codec register index
    @param[out] puiData     Register data

    @return
        - @b TRUE: success
        - @b FALSE: fail
*/
static BOOL audcodec_readData(UINT32 uiReg, UINT32 *puiData)
{
	switch (pAudioDeviceObj->uiI2SCtrl) {
	case AUDIO_I2SCTRL_I2C: {
			I2C_DATA i2cData;
			I2C_BYTE i2cByte[3];

			i2cData.VersionInfo = DRV_VER_96660;
			i2cData.pByte       = i2cByte;
			i2cData.ByteCount   = 3;
			i2cByte[0].uiValue  = AIC3254_I2C_SADDR;    // slave address of TVL320AIC3254
			i2cByte[0].Param    = I2C_BYTE_PARAM_START;
			i2cByte[1].uiValue  = uiReg;
			i2cByte[1].Param    = I2C_BYTE_PARAM_NONE;
			i2cByte[2].uiValue  = AIC3254_I2C_SADDR | 0x01;    // slave address with READ
			i2cByte[2].Param    = I2C_BYTE_PARAM_START;

			// Lock I2C
			i2c_lock(gI2cSession);

			do {
				if (i2c_transmit(&i2cData) != I2C_STS_OK) {
					break;
				}

				i2cData.ByteCount = 1;
				i2cByte[0].Param = I2C_BYTE_PARAM_STOP | I2C_BYTE_PARAM_NACK;
				if (i2c_receive(&i2cData) != I2C_STS_OK) {
					debug_msg("i2c_receive err\r\n");
				}

			} while (0);

			// Unlock I2C
			i2c_unlock(gI2cSession);

			*puiData = i2cByte[0].uiValue;
		}
		return TRUE;
	default:
		return FALSE;
	}
}

/*
    Set Audio Codec control parameters

    This function set the special parameter to the audio codec.
    The result is codec dependent.
    You should call this function after aud_init().

    @param[in] Parameter    Audio parameter. Available values are below:

    @param[in] uiSetting    parameter setting of Parameter. Below is the case by case descriptions.

    @return
        - @b FALSE: input parameter not support
        - @b TRUE: input parameter handle ok
*/
static BOOL audcodec_setParameter(AUDIO_PARAMETER Parameter, UINT32 uiSetting)
{
	return FALSE;
}

/*
    Check Sampling Rate API for audio module

    This function check sampling rate set by feature AUDIO_PARAMETER_CHECKED_PLAY_SAMPLING_RATE

    @return
        - @b TRUE: sampling rate is valid. OR not require to check
        - @b FALSE: sampling rate is invalid
*/
static BOOL audcodec_chkSamplingRate(void)
{
	if (bChkPlySR) {
		if (playCheckSR != audioSR) {
			debug_msg("%s: installed SR %d, but current SR is %d\r\n",
					  __func__, playCheckSR, audioSR);
			return FALSE;
		}
	}

	return TRUE;
}

//@}
