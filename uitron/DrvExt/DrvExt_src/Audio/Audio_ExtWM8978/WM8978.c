/*
    External WM8978 audio codec driver

    This file is the driver for WM8978 extended audio codec.

    @file       WM8978.c
    @ingroup    mISYSAud
    @note       Nothing.

    Copyright   Novatek Microelectronics Corp. 2012.  All rights reserved.
*/

/** \addtogroup mISYSAud */
//@{

#include "WM8978.h"
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

#define WM8978_CLKSEL       (0)             // Clock source of WM8978 board
// 0: use MCLK from NT96650
// 1: use codec internal PLL of WM8978

#define AUD_I2C_CLK_GPIO        (pAudioDeviceObj->uiGPIOClk)
#define AUD_I2C_DAT_GPIO        (pAudioDeviceObj->uiGPIOData)

static AUDIO_SR             audioSR         = AUDIO_SR_8000;
static BOOL                 bChkPlySR       = FALSE;            // for feature AUDIO_FEATURE_CHECK_PLAY_SAMPLING_RATE
static AUDIO_SR             playCheckSR     = AUDIO_SR_32000;   // for parameter AUDIO_PARAMETER_CHECKED_PLAY_SAMPLING_RATE


static WM8978_REG_SET       WM8978CtrlReg   = {
	{0x00, 0x000}, {0x01, 0x000}, {0x02, 0x000}, {0x03, 0x000},
	{0x04, 0x050}, {0x05, 0x000}, {0x06, 0x140}, {0x07, 0x000},
	{0x08, 0x000}, {0x09, 0x000}, {0x0A, 0x000}, {0x0B, 0x0FF},
	{0x0C, 0x0FF}, {0x0D, 0x000}, {0x0E, 0x100}, {0x0F, 0x0FF},
	{0x10, 0x000}, {0x11, 0x000}, {0x12, 0x12C}, {0x13, 0x02C},
	{0x14, 0x02C}, {0x15, 0x02C}, {0x16, 0x02C}, {0x17, 0x000},
	{0x18, 0x032}, {0x19, 0x000}, {0x1A, 0x000}, {0x1B, 0x000},
	{0x1C, 0x000}, {0x1D, 0x000}, {0x1E, 0x000}, {0x1F, 0x000},
	{0x20, 0x038}, {0x21, 0x00B}, {0x22, 0x032}, {0x23, 0x000},
	{0x24, 0x008}, {0x25, 0x00C}, {0x26, 0x093}, {0x27, 0x0E9},
	{0x28, 0x000}, {0x29, 0x000}, {0x2A, 0x000}, {0x2B, 0x000},
	{0x2C, 0x033}, {0x2D, 0x010}, {0x2E, 0x010}, {0x2F, 0x100},
	{0x30, 0x000}, {0x31, 0x002}, {0x32, 0x001}, {0x33, 0x001},
	{0x34, 0x039}, {0x35, 0x039}, {0x36, 0x039}, {0x37, 0x039},
	{0x38, 0x001}, {0x39, 0x001}
};
static PAUDIO_DEVICE_OBJ    pAudioDeviceObj = NULL;
static AUDIO_RECSRC         WM8978RecSrc    = AUDIO_RECSRC_MIC;
static AUDIO_OUTPUT         WM8978Output    = AUDIO_OUTPUT_SPK;
static AUDIO_VOL            WM8978Volume    = AUDIO_VOL_6;
static AUDIO_GAIN           WM8978Gain      = AUDIO_GAIN_6;
static BOOL                 bWM8978ALC      = FALSE;

static AUDIO_I2SCLKR        WM8978CLKRatio  = AUDIO_I2SCLKR_256FS_32BIT;

static AUDIO_CODEC_FUNC     WM8978CodecFP    = {
	AUDIO_CODEC_TYPE_EXTERNAL,

	// Set Device Object
	audcodec_setDeviceObject,
	// Init
	audcodec_init,
	// Set Record Source
	audcodec_setRecordSource,
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
	audcodec_setEffect,
	// Set Feature
	audcodec_setFeature,
	// StopRecord
	audcodec_stopRecord,
	// StopPlayback
	audcodec_stopPlayback,
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
	// Get Sampling rate
//    audcodec_getSamplingRate,
	// Check Sampling rate
	audcodec_chkSamplingRate,
	// Open
	NULL,
	// Close
	NULL,
};

/**
    Get audio codec function pointer for embedded audio codec

    This function will return audio code function pointer for external WM8978
    audio codec.

    @param[out]  pAudCodecFunc      Audio codec function pointer

    @return void
*/
void audExtCodec_getFP(PAUDIO_CODEC_FUNC pAudCodecFunc)
{
	memcpy((void *)pAudCodecFunc, (const void *)&WM8978CodecFP, sizeof(AUDIO_CODEC_FUNC));
}

/*
  Send command to Wolfson WM8978 codec

  This function send command to Wolfson WM8978 codec

  @param[in] pReg       WM8978 codec register value

  @return void
*/
static void WM8978_sendCommand(PWM8978_REG pReg)
{
	UINT32  uiData, i;

	uiData = ((pReg->uiAddr & 0x7F) << 9) | (pReg->uiData & 0x1FF);

	switch (pAudioDeviceObj->uiI2SCtrl) {
	case AUDIO_I2SCTRL_SIF:
	default:
//        uiData = uiData << 16;
//        sif_sendMSB(pAudioDeviceObj->uiChannel, uiData);
		break;

	case AUDIO_I2SCTRL_I2C:
		/*
		// Lock I2C
		i2c_lock();

		// Send device address and indicate write command
		uiI2CData = WM8974_I2C_WRITE;
		i2c_masterTransmit(uiI2CData, TRUE, FALSE);

		// Send first byte
		uiI2CData = uiData >> 8;
		i2c_masterTransmit(uiI2CData, FALSE, FALSE);

		// Send last byte
		uiI2CData = uiData;
		i2c_masterTransmit(uiI2CData, FALSE, TRUE);

		// Unlock I2C
		i2c_unlock();
		*/
		break;

	case AUDIO_I2SCTRL_GPIO_SIF:
		uiData = uiData << 16;
		gpio_clearPin(pAudioDeviceObj->uiGPIOCS);
		Delay_DelayUsPolling(1);
		for (i = 16; i > 0; i--) {
			gpio_clearPin(pAudioDeviceObj->uiGPIOClk);

			if (uiData & 0x80000000) {
				gpio_setPin(pAudioDeviceObj->uiGPIOData);
			} else {
				gpio_clearPin(pAudioDeviceObj->uiGPIOData);
			}

			Delay_DelayUsPolling(1);
			gpio_setPin(pAudioDeviceObj->uiGPIOClk);

			Delay_DelayUsPolling(1);
			uiData <<= 1;
		}
		gpio_setPin(pAudioDeviceObj->uiGPIOCS);

		Delay_DelayUsPolling(1);
		break;

	case AUDIO_I2SCTRL_GPIO_I2C:
		wm8978_writeCtrlReg((uiData >> 8) & 0xFF, uiData & 0xFF);
		break;
	}
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
	case AUDIO_I2SCTRL_SIF:
	default:
#if 0
		//#NT#2007/05/07#Chris Hsu -begin
		//#Conform to datasheet
		// Max clock = 1000/160 = 6.25 MHz
		// Set SIF clock
		switch (pll_getClockRate(PCLKSEL)) {
		case PCLKSEL_CLK60:
		default:
			// uiClkDiv = (60 / (6.25 * 2)) - 1 = 3.8 -> 4
			// Sclk = 60 / (2 * ( 4 + 1)) = 6 MHz
			SIFCtrl.uiClkDiv = 4;
			break;

		case PCLKSEL_CLK48:
		// uiClkDiv = (48 / (6.25 * 2)) - 1 = 2.84 -> 3
		// Sclk = 48 / (2 * ( 3 + 1)) = 6 MHz
		case PCLKSEL_CLK40:
			// uiClkDiv = (40 / (6.25 * 2)) - 1 = 2.2 -> 3
			// Sclk = 40 / (2 * ( 3 + 1)) = 5 MHz
			SIFCtrl.uiClkDiv = 3;
			break;

		case PCLKSEL_CLK24:
			// uiClkDiv = (24 / (6.25 * 2)) - 1 = 0.92 -> 1
			// Sclk = 24 / (2 * ( 1 + 1)) = 6 MHz
			SIFCtrl.uiClkDiv = 1;
			break;
		}

		SIFCtrl.uiMode          = SIF_MODE_0000;
		SIFCtrl.bSyncEn         = FALSE;
		SIFCtrl.uiSyncSrc       = SIF_SYNCSRC_LCD;  // Don't care
		SIFCtrl.uiSenDuration   = 0;                // Don't care
		SIFCtrl.uiLength        = 16;
		SIFCtrl.bMSB            = TRUE;
		SIFCtrl.uiSenH          = 0;                // Don't care
		//#NT#2007/05/07#Chris Hsu -end

		sif_setControl(pAudioDeviceObj->uiChannel, &SIFCtrl);
#endif
		break;

	case AUDIO_I2SCTRL_I2C:
#if 0
		// Select pinmux of I2C
		pinmux_select_i2c(PINMUX_I2C);

		// Set I2C clock and TSR/GSR
		uiI2CGSR = 0x06;
		switch (pll_getClockRate(PCLKSEL)) {
		case PCLKSEL_CLK60:
		default:
			uiI2CClkDiv = 0x91;
			uiI2CTSR    = 0x19;
			break;

		case PCLKSEL_CLK48:
			uiI2CClkDiv = 0x73;
			uiI2CTSR    = 0x14;
			break;

		case PCLKSEL_CLK40:
			uiI2CClkDiv = 0x5F;
			uiI2CTSR    = 0x11;
			break;

		case PCLKSEL_CLK24:
			uiI2CClkDiv = 0x37;
			uiI2CTSR    = 0x0A;
			break;
		}
		i2c_initMaster(uiI2CClkDiv, uiI2CTSR, uiI2CGSR);

		i2c_enableInt(I2C_INT_DT_EN);
#endif
		break;

	case AUDIO_I2SCTRL_GPIO_SIF:
		gpio_setDir(pAudioDeviceObj->uiGPIOData, GPIO_DIR_OUTPUT);
		gpio_setDir(pAudioDeviceObj->uiGPIOClk, GPIO_DIR_OUTPUT);
		gpio_setDir(pAudioDeviceObj->uiGPIOCS, GPIO_DIR_OUTPUT);

		gpio_setPin(pAudioDeviceObj->uiGPIOData);
		gpio_setPin(pAudioDeviceObj->uiGPIOClk);
		gpio_setPin(pAudioDeviceObj->uiGPIOCS);
		break;

	case AUDIO_I2SCTRL_GPIO_I2C:
		gpio_setDir(pAudioDeviceObj->uiGPIOClk, GPIO_DIR_INPUT);
		gpio_setDir(pAudioDeviceObj->uiGPIOData, GPIO_DIR_INPUT);
		break;
	}

	// Set VROI to 1, Enable SPK BOOST
	WM8978CtrlReg.outputctrl.uiData |= 0x005;
	WM8978_sendCommand(&WM8978CtrlReg.outputctrl);

	// Enable BIAS, BUFIO, VMIDSEL, BUFDCOPEN
	WM8978CtrlReg.pwm1.uiData |= 0x10D;
	WM8978_sendCommand(&WM8978CtrlReg.pwm1);

#if (WM8978_CLKSEL == 0)
	// Internal operation use MCLK clock
	WM8978CtrlReg.cgctrl.uiData &= ~0x100;
#else
	// Internal operation use PLL output clock
	WM8978CtrlReg.cgctrl.uiData |= 0x100;
#endif
	WM8978_sendCommand(&WM8978CtrlReg.cgctrl);

#if (WM8978_CLKSEL == 0)
	// Disable PLL
	WM8978CtrlReg.pwm1.uiData &= ~0x020;
#else
	// Enable PLL
	WM8978CtrlReg.pwm1.uiData |= 0x020;
#endif
	WM8978_sendCommand(&WM8978CtrlReg.pwm1);

#if 1
	// Set Data Length (16Bits)
	WM8978CtrlReg.audinterface.uiData &= ~0x060;
	WM8978_sendCommand(&WM8978CtrlReg.audinterface);
#else
	// Set Data Length (32Bits)
	WM8978CtrlReg.audinterface.uiData |= 0x060;
	WM8978_sendCommand(&WM8978CtrlReg.audinterface);
#endif

	// Set Sampling Rate
	audcodec_setSamplingRate(pAudio->SamplingRate);


	// Set I2S Clock Ratio
	audcodec_setClockRatio(pAudio->I2S.ClkRatio);

	// Set Channel
//    audcodec_setChannel(pAudio->Channel);

	// Always acts as SLAVE!!!
	WM8978CtrlReg.cgctrl.uiData &= ~0x001;
	WM8978_sendCommand(&WM8978CtrlReg.cgctrl);
	// Always acts as MASTER!!!
//    WM8978CtrlReg.cgctrl.uiData |= 0x001;
//    WM8978_sendCommand(&WM8978CtrlReg.cgctrl);

	// Set I2S format
	audcodec_setFormat(pAudio->I2S.I2SFmt);

	// Enable PGA Gain, so we can set Record Gain
	// Set Microphone Bias voltage control to 0.75*AVDD
	WM8978CtrlReg.inputctrl.uiData |= 0x33;
	WM8978_sendCommand(&WM8978CtrlReg.inputctrl);

	// Set left/right Mixer source to left/right DAC
	WM8978CtrlReg.lmixerctrl.uiData |= 0x01;
	WM8978_sendCommand(&WM8978CtrlReg.lmixerctrl);
	WM8978CtrlReg.rmixerctrl.uiData |= 0x01;
	WM8978_sendCommand(&WM8978CtrlReg.rmixerctrl);

	// Set LOUT1/ROUT1, LOUT2/ROUT2 Volume to maximum
	WM8978CtrlReg.lout1vol.uiData = 0x1BF;
	WM8978CtrlReg.rout1vol.uiData = 0x1BF;
	WM8978CtrlReg.lout2vol.uiData = 0x1BF;
	WM8978CtrlReg.rout2vol.uiData = 0x1BF;
	WM8978_sendCommand(&WM8978CtrlReg.lout1vol);
	WM8978_sendCommand(&WM8978CtrlReg.rout1vol);
	WM8978_sendCommand(&WM8978CtrlReg.lout2vol);
	WM8978_sendCommand(&WM8978CtrlReg.rout2vol);

	// Enable PGABOOST
	WM8978CtrlReg.adcboostctrl.uiData = 0x100;
	WM8978_sendCommand(&WM8978CtrlReg.adcboostctrl);

	// Select output path
	audcodec_setOutput(pAudio->Output);

	// Select record path
	audcodec_setOutput(pAudio->Output);


#if (WM8978_CLKSEL == 0)
	if (WM8978CLKRatio == AUDIO_I2SCLKR_256FS_32BIT) {
		dai_setI2sConfig(DAI_I2SCONFIG_ID_CLKRATIO, DAI_I2SCLKR_256FS_32BIT);
	} else {
		dai_setI2sConfig(DAI_I2SCONFIG_ID_CLKRATIO, DAI_I2SCLKR_256FS_64BIT);
	}
#endif

}

/*
    Set Record Source API for audio module

    This function is the Set Record Source API for audio module

    @param[in] RecSrc   Record Source

    @return void
*/
static void audcodec_setRecordSource(AUDIO_RECSRC RecSrc)
{
	switch (RecSrc) {
	case AUDIO_RECSRC_MIC:
		WM8978RecSrc = AUDIO_RECSRC_MIC;

		// Enable PGA BOOST, Disable L2/AUXL2 BOOST
		WM8978CtrlReg.adcboostctrl.uiData = 0x100;
		WM8978_sendCommand(&WM8978CtrlReg.adcboostctrl);
		WM8978CtrlReg.radcboostctrl.uiData = 0x100;
		WM8978_sendCommand(&WM8978CtrlReg.radcboostctrl);

		// Unmute MIC
		WM8978CtrlReg.rightgainctrl.uiData &= ~0x40;
		WM8978CtrlReg.rightgainctrl.uiData |= 0x100;
		WM8978_sendCommand(&WM8978CtrlReg.rightgainctrl);

		WM8978CtrlReg.gainctrl.uiData &= ~0x40;
		WM8978CtrlReg.gainctrl.uiData |= 0x100;
		WM8978_sendCommand(&WM8978CtrlReg.gainctrl);
		break;
	case AUDIO_RECSRC_AUX:
		WM8978RecSrc = AUDIO_RECSRC_AUX;

		// Disable PGA/L2 BOOST, Enable AUXL2 BOOST
		WM8978CtrlReg.adcboostctrl.uiData = 0x007;
		WM8978_sendCommand(&WM8978CtrlReg.adcboostctrl);
		WM8978CtrlReg.radcboostctrl.uiData = 0x007;
		WM8978_sendCommand(&WM8978CtrlReg.radcboostctrl);

		// Mute MIC
		WM8978CtrlReg.gainctrl.uiData |= 0x40;
		WM8978CtrlReg.rightgainctrl.uiData |= 0x40;
		WM8978_sendCommand(&WM8978CtrlReg.gainctrl);
		WM8978_sendCommand(&WM8978CtrlReg.rightgainctrl);
		break;
	default:
		debug_msg("%s: unsupported record source: 0x%lx\r\n", __func__, RecSrc);
		break;
	}
}

/*
    Set Output Path API for audio module

    This function is the Set Output Path API for audio module

    @param[in] Output       Output Path

    @return void
*/
static void audcodec_setOutput(AUDIO_OUTPUT Output)
{
#if _EMULATION_
	debug_msg("Fix WM8978 Output to HP in Emulation!\r\n");
	Output = AUDIO_OUTPUT_HP;
#endif

	switch (Output) {
	case AUDIO_OUTPUT_SPK:
	//coverity[dead_error_begin]
	default:
		// Change output immediately if DAC is enabled (Playback now)
		if (WM8978CtrlReg.pwm3.uiData & 0x03) {
			WM8978CtrlReg.pwm2.uiData &= 0x180;       // debug
			WM8978CtrlReg.pwm3.uiData |= ~0x60;       // debug
			WM8978_sendCommand(&WM8978CtrlReg.pwm2);
			WM8978_sendCommand(&WM8978CtrlReg.pwm3);
		}
		WM8978Output = AUDIO_OUTPUT_SPK;
		break;
	case AUDIO_OUTPUT_HP:
		// Change output immediately if DAC is enabled (Playback now)
		if (WM8978CtrlReg.pwm3.uiData & 0x03) {
			WM8978CtrlReg.pwm2.uiData |= 0x180;       // debug
			WM8978CtrlReg.pwm3.uiData &= ~0x60;       // debug
			WM8978CtrlReg.pwm3.uiData |= 0x0C;        // Enable L/R, Mixer
			WM8978_sendCommand(&WM8978CtrlReg.pwm2);
			WM8978_sendCommand(&WM8978CtrlReg.pwm3);
		}
		WM8978Output = AUDIO_OUTPUT_HP;
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
	WM8978CtrlReg.addctrl.uiData &= ~0x00E;
	WM8978CtrlReg.cgctrl.uiData &= ~0x0E0;

	audioSR = SamplingRate;
#if 0
	if (codecClkSel == AUDIO_CLK_SEL_CLKMUX) {
		pll_setClockRate(PLL_CLKSEL_ADO_CLKSEL, PLL_CLKSEL_ADO_CLKSEL_CLKMUX);
	} else {
		pll_setClockRate(PLL_CLKSEL_ADO_CLKSEL, PLL_CLKSEL_ADO_CLKSEL_CLKDIV);
		pll_setPLLEn(PLL_ID_7, TRUE);
	}
#endif
	switch (SamplingRate) {
	// 8 KHz
	case AUDIO_SR_8000:
		WM8978CtrlReg.addctrl.uiData   |= 0x00A;

#if (WM8978_CLKSEL == 0)
//        pll_setClockFreq(ADOCLK_FREQ, 2048000);
#else
		WM8978CtrlReg.cgctrl.uiData    |= 0x0E0;

		WM8978CtrlReg.plln.uiData       = 0x008;
		WM8978CtrlReg.pllk1.uiData      = 0x000;
		WM8978CtrlReg.pllk2.uiData      = 0x000;
		WM8978CtrlReg.pllk3.uiData      = 0x000;
#endif
		break;

	// 11.025 KHz
	case AUDIO_SR_11025:
		WM8978CtrlReg.addctrl.uiData   |= 0x008;
#if (WM8978_CLKSEL == 0)
//        pll_setClockFreq(ADOCLK_FREQ, 2822400);
#else
		WM8978CtrlReg.cgctrl.uiData    |= 0x0C0;

		WM8978CtrlReg.plln.uiData       = 0x007;
		WM8978CtrlReg.pllk1.uiData      = 0x016;
		WM8978CtrlReg.pllk2.uiData      = 0x0CC;
		WM8978CtrlReg.pllk3.uiData      = 0x199;
#endif
		break;

	// 12 KHz
	case AUDIO_SR_12000:
		WM8978CtrlReg.addctrl.uiData   |= 0x008;
#if (WM8978_CLKSEL == 0)
//        pll_setClockFreq(ADOCLK_FREQ, 3072000);
#else
		WM8978CtrlReg.cgctrl.uiData    |= 0x0C0;

		WM8978CtrlReg.plln.uiData       = 0x008;
		WM8978CtrlReg.pllk1.uiData      = 0x000;
		WM8978CtrlReg.pllk2.uiData      = 0x000;
		WM8978CtrlReg.pllk3.uiData      = 0x000;
#endif
		break;

	// 16 KHz
	case AUDIO_SR_16000:
		WM8978CtrlReg.addctrl.uiData   |= 0x006;
#if (WM8978_CLKSEL == 0)
//        pll_setClockFreq(ADOCLK_FREQ, 4096000);
#else
		WM8978CtrlReg.cgctrl.uiData    |= 0x0A0;

		WM8978CtrlReg.plln.uiData       = 0x008;
		WM8978CtrlReg.pllk1.uiData      = 0x000;
		WM8978CtrlReg.pllk2.uiData      = 0x000;
		WM8978CtrlReg.pllk3.uiData      = 0x000;
#endif
		break;

	// 22.05 KHz
	case AUDIO_SR_22050:
		WM8978CtrlReg.addctrl.uiData   |= 0x004;
#if (WM8978_CLKSEL == 0)
//        pll_setClockFreq(ADOCLK_FREQ, 5644800);
#else
		WM8978CtrlReg.cgctrl.uiData    |= 0x080;

		WM8978CtrlReg.plln.uiData       = 0x007;
		WM8978CtrlReg.pllk1.uiData      = 0x016;
		WM8978CtrlReg.pllk2.uiData      = 0x0CC;
		WM8978CtrlReg.pllk3.uiData      = 0x199;
#endif
		break;

	// 24 KHz
	case AUDIO_SR_24000:
		WM8978CtrlReg.addctrl.uiData   |= 0x004;
#if (WM8978_CLKSEL == 0)
//        pll_setClockFreq(ADOCLK_FREQ, 6144000);
#else
		WM8978CtrlReg.cgctrl.uiData    |= 0x080;

		WM8978CtrlReg.plln.uiData       = 0x008;
		WM8978CtrlReg.pllk1.uiData      = 0x000;
		WM8978CtrlReg.pllk2.uiData      = 0x000;
		WM8978CtrlReg.pllk3.uiData      = 0x000;
#endif
		break;

	// 32 KHz
	case AUDIO_SR_32000:
		WM8978CtrlReg.addctrl.uiData   |= 0x002;
#if (WM8978_CLKSEL == 0)
//        pll_setClockFreq(ADOCLK_FREQ, 8192000);
#else
		WM8978CtrlReg.cgctrl.uiData    |= 0x060;

		WM8978CtrlReg.plln.uiData       = 0x008;
		WM8978CtrlReg.pllk1.uiData      = 0x000;
		WM8978CtrlReg.pllk2.uiData      = 0x000;
		WM8978CtrlReg.pllk3.uiData      = 0x000;
#endif
		break;

	// 44.1 KHz
	case AUDIO_SR_44100:
#if (WM8978_CLKSEL == 0)
//        pll_setClockFreq(ADOCLK_FREQ, 11289600);
#else
		WM8978CtrlReg.cgctrl.uiData    |= 0x040;

		WM8978CtrlReg.plln.uiData       = 0x007;
		WM8978CtrlReg.pllk1.uiData      = 0x016;
		WM8978CtrlReg.pllk2.uiData      = 0x0CC;
		WM8978CtrlReg.pllk3.uiData      = 0x199;
#endif
		break;

	// 48 KHz
	default:
#if (WM8978_CLKSEL == 0)
//        pll_setClockFreq(ADOCLK_FREQ, 12288000);
#else
		WM8978CtrlReg.cgctrl.uiData    |= 0x040;

		WM8978CtrlReg.plln.uiData       = 0x008;
		WM8978CtrlReg.pllk1.uiData      = 0x000;
		WM8978CtrlReg.pllk2.uiData      = 0x000;
		WM8978CtrlReg.pllk3.uiData      = 0x000;
#endif
		break;
	}

	WM8978_sendCommand(&WM8978CtrlReg.addctrl);
	WM8978_sendCommand(&WM8978CtrlReg.cgctrl);
#if (WM8978_CLKSEL == 1)
	WM8978_sendCommand(&WM8978CtrlReg.plln);
	WM8978_sendCommand(&WM8978CtrlReg.pllk1);
	WM8978_sendCommand(&WM8978CtrlReg.pllk2);
	WM8978_sendCommand(&WM8978CtrlReg.pllk3);
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
		WM8978CtrlReg.audinterface.uiData |= 0x003;
		break;

	case AUDIO_CH_RIGHT:
		WM8978CtrlReg.audinterface.uiData &= ~0x003;
		WM8978CtrlReg.audinterface.uiData |= 0x001;
		break;

	default:
	case AUDIO_CH_STEREO:
		WM8978CtrlReg.audinterface.uiData &= ~0x003;
		break;
	}
	WM8978_sendCommand(&WM8978CtrlReg.audinterface);
}

/*
    Set PCM Volume API for audio module

    This function is the Set PCM Volume API for audio module

    @param[in] Vol      Volume

    @return void
*/
static void audcodec_setVolume(AUDIO_VOL Vol)
{
	UINT32 uiData;

	if (Vol > AUDIO_VOL_7) {
		Vol = AUDIO_VOL_7;
	}

	WM8978CtrlReg.ldacdvol.uiData &= ~0xFF;
	WM8978CtrlReg.rdacdvol.uiData &= ~0xFF;

	if (Vol == AUDIO_VOL_MUTE) {
		uiData = 0x100;
	} else {
		uiData = ((Vol - 1) * 16) + 0x8F;
	}

	WM8978CtrlReg.ldacdvol.uiData = uiData | 0x100;
	WM8978CtrlReg.rdacdvol.uiData = uiData | 0x100;

	WM8978_sendCommand(&WM8978CtrlReg.ldacdvol);
	WM8978_sendCommand(&WM8978CtrlReg.rdacdvol);

	WM8978Volume = Vol;
}

/*
    Set Record Gain API for audio module

    This function is the Set Record Gain API for audio module

    @param[in] Gain     Gain

    @return void
*/
static void audcodec_setGain(AUDIO_GAIN Gain)
{
	UINT32 uiData;

	if (Gain > AUDIO_GAIN_7) {
		Gain = AUDIO_GAIN_7;
	}

	WM8978Gain = Gain;

	if (WM8978RecSrc == AUDIO_RECSRC_MIC) {
		if (Gain == AUDIO_GAIN_MUTE) {
			WM8978CtrlReg.gainctrl.uiData |= 0x40;
			WM8978CtrlReg.rightgainctrl.uiData |= 0x40;
		} else {
			WM8978CtrlReg.gainctrl.uiData &= ~0x7F;
			WM8978CtrlReg.rightgainctrl.uiData &= ~0x7F;
			uiData = 0x3F - ((AUDIO_GAIN_7 - Gain) << 2);
			WM8978CtrlReg.gainctrl.uiData |= uiData | 0x100;
			WM8978CtrlReg.rightgainctrl.uiData |= uiData | 0x100;
		}
		WM8978_sendCommand(&WM8978CtrlReg.gainctrl);
		WM8978_sendCommand(&WM8978CtrlReg.rightgainctrl);
	}
}

/*
    Set Sound Effect API for audio module

    This function is the Set Sound Effect API for audio module

    @param[in] Effect       Sound Effect
    @return void
*/
static void audcodec_setEffect(AUDIO_EFFECT Effect)
{
	switch (Effect) {
	case AUDIO_EFFECT_NONE:
	default:
		WM8978CtrlReg.eq1.uiData        = 0x12C;
		WM8978CtrlReg.eq2.uiData        = 0x02C;
		WM8978CtrlReg.eq3.uiData        = 0x02C;
		WM8978CtrlReg.eq4.uiData        = 0x02C;
		WM8978CtrlReg.eq5.uiData        = 0x02C;
		WM8978CtrlReg.threedctrl.uiData = 0x000;
		break;
	case AUDIO_EFFECT_3D_HALF:
		WM8978CtrlReg.eq1.uiData        = 0x12C;
		WM8978CtrlReg.eq2.uiData        = 0x02C;
		WM8978CtrlReg.eq3.uiData        = 0x02C;
		WM8978CtrlReg.eq4.uiData        = 0x02C;
		WM8978CtrlReg.eq5.uiData        = 0x02C;
		WM8978CtrlReg.threedctrl.uiData = 0x008;
		break;
	case AUDIO_EFFECT_3D_FULL:
		WM8978CtrlReg.eq1.uiData        = 0x12C;
		WM8978CtrlReg.eq2.uiData        = 0x02C;
		WM8978CtrlReg.eq3.uiData        = 0x02C;
		WM8978CtrlReg.eq4.uiData        = 0x02C;
		WM8978CtrlReg.eq5.uiData        = 0x02C;
		WM8978CtrlReg.threedctrl.uiData = 0x00F;
		break;

	case AUDIO_EFFECT_ROCK:
		WM8978CtrlReg.eq1.uiData        = 0x147;
		WM8978CtrlReg.eq2.uiData        = 0x02E;
		WM8978CtrlReg.eq3.uiData        = 0x02C;
		WM8978CtrlReg.eq4.uiData        = 0x06B;
		WM8978CtrlReg.eq5.uiData        = 0x049;
		WM8978CtrlReg.threedctrl.uiData = 0x003;
		break;

	case AUDIO_EFFECT_POP:
		WM8978CtrlReg.eq1.uiData        = 0x110;
		WM8978CtrlReg.eq2.uiData        = 0x107;
		WM8978CtrlReg.eq3.uiData        = 0x149;
		WM8978CtrlReg.eq4.uiData        = 0x02C;
		WM8978CtrlReg.eq5.uiData        = 0x029;
		WM8978CtrlReg.threedctrl.uiData = 0x005;
		break;

	case AUDIO_EFFECT_JAZZ:
		WM8978CtrlReg.eq1.uiData        = 0x166;
		WM8978CtrlReg.eq2.uiData        = 0x12A;
		WM8978CtrlReg.eq3.uiData        = 0x02C;
		WM8978CtrlReg.eq4.uiData        = 0x16A;
		WM8978CtrlReg.eq5.uiData        = 0x049;
		WM8978CtrlReg.threedctrl.uiData = 0x002;
		break;

	case AUDIO_EFFECT_CLASSICAL:
		WM8978CtrlReg.eq1.uiData        = 0x12C;
		WM8978CtrlReg.eq2.uiData        = 0x02C;
		WM8978CtrlReg.eq3.uiData        = 0x14A;
		WM8978CtrlReg.eq4.uiData        = 0x169;
		WM8978CtrlReg.eq5.uiData        = 0x02C;
		WM8978CtrlReg.threedctrl.uiData = 0x002;
		break;
		/*
		    case AUDIO_EFFECT_DNACE:
		        WM8978CtrlReg.eq1.uiData    = 0x144;
		        WM8978CtrlReg.eq2.uiData    = 0x108;
		        WM8978CtrlReg.eq3.uiData    = 0x02C;
		        WM8978CtrlReg.eq4.uiData    = 0x069;
		        WM8978CtrlReg.eq5.uiData    = 0x046;
		        break;

		    case AUDIO_EFFECT_HEAVY:
		        WM8978CtrlReg.eq1.uiData    = 0x146;
		        WM8978CtrlReg.eq2.uiData    = 0x02C;
		        WM8978CtrlReg.eq3.uiData    = 0x14F;
		        WM8978CtrlReg.eq4.uiData    = 0x16A;
		        WM8978CtrlReg.eq5.uiData    = 0x065;
		        break;

		    case AUDIO_EFFECT_DISCO:
		        WM8978CtrlReg.eq1.uiData    = 0x125;
		        WM8978CtrlReg.eq2.uiData    = 0x12A;
		        WM8978CtrlReg.eq3.uiData    = 0x02C;
		        WM8978CtrlReg.eq4.uiData    = 0x16A;
		        WM8978CtrlReg.eq5.uiData    = 0x068;
		        break;

		    case AUDIO_EFFECT_SOFT:
		        WM8978CtrlReg.eq1.uiData    = 0x148;
		        WM8978CtrlReg.eq2.uiData    = 0x12B;
		        WM8978CtrlReg.eq3.uiData    = 0x02C;
		        WM8978CtrlReg.eq4.uiData    = 0x02C;
		        WM8978CtrlReg.eq5.uiData    = 0x030;
		        break;

		    case AUDIO_EFFECT_LIVE:
		        WM8978CtrlReg.eq1.uiData    = 0x149;
		        WM8978CtrlReg.eq2.uiData    = 0x02C;
		        WM8978CtrlReg.eq3.uiData    = 0x02C;
		        WM8978CtrlReg.eq4.uiData    = 0x069;
		        WM8978CtrlReg.eq5.uiData    = 0x02C;
		        break;

		    case AUDIO_EFFECT_HALL:
		        WM8978CtrlReg.eq1.uiData    = 0x146;
		        WM8978CtrlReg.eq2.uiData    = 0x12A;
		        WM8978CtrlReg.eq3.uiData    = 0x02C;
		        WM8978CtrlReg.eq4.uiData    = 0x14A;
		        WM8978CtrlReg.eq5.uiData    = 0x047;
		        break;
		*/
	}

	WM8978_sendCommand(&WM8978CtrlReg.eq1);
	WM8978_sendCommand(&WM8978CtrlReg.eq2);
	WM8978_sendCommand(&WM8978CtrlReg.eq3);
	WM8978_sendCommand(&WM8978CtrlReg.eq4);
	WM8978_sendCommand(&WM8978CtrlReg.eq5);
	WM8978_sendCommand(&WM8978CtrlReg.threedctrl);
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
		bWM8978ALC = bEnable;

		if (bWM8978ALC == TRUE) {
			// Enable ALC
			WM8978CtrlReg.alcctrl1.uiData |= 0x180;
			WM8978_sendCommand(&WM8978CtrlReg.alcctrl1);

			// Enable noise gate
			WM8978CtrlReg.noisegate.uiData |= 0x008;
			WM8978_sendCommand(&WM8978CtrlReg.noisegate);
		} else {
			// Disable ALC
			WM8978CtrlReg.alcctrl1.uiData &= ~0x180;
			WM8978_sendCommand(&WM8978CtrlReg.alcctrl1);

			// Disable noise gate
			WM8978CtrlReg.noisegate.uiData &= ~0x008;
			WM8978_sendCommand(&WM8978CtrlReg.noisegate);
		}

		// Set gain
		audcodec_setGain(WM8978Gain);
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
static void audcodec_stopPlayback(void)
{
	// Playback
	// Mute DAC
	WM8978CtrlReg.dacctrl.uiData |= 0x040;
	WM8978_sendCommand(&WM8978CtrlReg.dacctrl);

	switch (WM8978Output) {
	case AUDIO_OUTPUT_SPK:
	default:
		WM8978CtrlReg.pwm3.uiData &= ~0x060;
		WM8978_sendCommand(&WM8978CtrlReg.pwm3);
		break;

	case AUDIO_OUTPUT_HP:
		WM8978CtrlReg.pwm2.uiData &= ~0x180;
		WM8978_sendCommand(&WM8978CtrlReg.pwm2);
		break;
	}
	// Disable DAC, speaker, speaker mixer
	WM8978CtrlReg.pwm3.uiData &= ~0x065;
	WM8978_sendCommand(&WM8978CtrlReg.pwm3);
}

static void audcodec_stopRecord(void)
{
	{
		// Record
		// Disable MICB
		WM8978CtrlReg.pwm1.uiData &= ~0x010;
		WM8978_sendCommand(&WM8978CtrlReg.pwm1);

		// Disable ADC, Input PGA / BOOST
		WM8978CtrlReg.pwm2.uiData &= ~0x00F;
		//WM8978CtrlReg.pwm2.uiData &= ~0x015;
		WM8978_sendCommand(&WM8978CtrlReg.pwm2);
	}
}

/*
    Record API for audio module

    This function is the Record API for audio module

    @return void
*/
static void audcodec_record(void)
{
	// Enable MICB
	WM8978CtrlReg.pwm1.uiData |= 0x010;
	WM8978_sendCommand(&WM8978CtrlReg.pwm1);

	// Enable ADC, Input PGA
	WM8978CtrlReg.pwm2.uiData |= 0x03F;
	WM8978_sendCommand(&WM8978CtrlReg.pwm2);

	// Delay to avoid pop noise
	Delay_DelayMs(100);
}

/*
    Playback API for audio module

    This function is the Playback API for audio module

    @return void
*/
static void audcodec_playback(void)
{
	// Enable L/R DAC, Mixer
	WM8978CtrlReg.pwm3.uiData |= 0x00F;

	switch (WM8978Output) {
	case AUDIO_OUTPUT_SPK:
	default:
		WM8978CtrlReg.pwm2.uiData &= ~0x180;
		WM8978CtrlReg.pwm3.uiData |= 0x060;
		break;

	case AUDIO_OUTPUT_HP:
		WM8978CtrlReg.pwm2.uiData |= 0x180;
		WM8978CtrlReg.pwm3.uiData &= ~0x060;
		break;
	}

	WM8978_sendCommand(&WM8978CtrlReg.pwm2);
	WM8978_sendCommand(&WM8978CtrlReg.pwm3);

	// Must delay to avoid pop noise
	Delay_DelayMs(1);

	// Unmute DAC
	WM8978CtrlReg.dacctrl.uiData &= ~0x040;
	WM8978_sendCommand(&WM8978CtrlReg.dacctrl);

}

/*
    Set I2S Format API for audio module

    This function is the Set I2S Format API for audio module

    @param[in] I2SFmt: I2S Format

    @return void
*/
static void audcodec_setFormat(AUDIO_I2SFMT I2SFmt)
{
	WM8978CtrlReg.audinterface.uiData &= ~0x018;
	switch (I2SFmt) {
	case AUDIO_I2SFMT_STANDARD:
	default:
		WM8978CtrlReg.audinterface.uiData |= 0x010;
		break;

	case AUDIO_I2SFMT_LIKE_LSB:
		WM8978CtrlReg.audinterface.uiData |= 0x000;
		break;

	case AUDIO_I2SFMT_LIKE_MSB:
		WM8978CtrlReg.audinterface.uiData |= 0x008;
		break;
	}
	WM8978_sendCommand(&WM8978CtrlReg.audinterface);
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
	// BCLK = MCLK/8
	if (I2SCLKRatio != AUDIO_I2SCLKR_256FS_32BIT) {
		debug_msg("%s: clkRatio 0x%lx not support\r\n", __func__, I2SCLKRatio);
	}
	WM8978CtrlReg.cgctrl.uiData &= ~0x01C;
	WM8978CtrlReg.cgctrl.uiData |= 0x00C;
	WM8978_sendCommand(&WM8978CtrlReg.cgctrl);

	WM8978CLKRatio = I2SCLKRatio;
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
	UINT32      i;
	PWM8978_REG pWM8974Reg;

	pWM8974Reg = (PWM8978_REG)&WM8978CtrlReg;
	for (i = 0; i < sizeof(WM8978_REG_SET) / sizeof(WM8978_REG); i++) {
		if (pWM8974Reg->uiAddr == uiReg) {
			pWM8974Reg->uiData = uiData;
			WM8978_sendCommand(pWM8974Reg);
			return TRUE;
		} else {
			pWM8974Reg++;
		}
	}
	return FALSE;
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
	UINT32      i;
	PWM8978_REG pWM8974Reg;

	pWM8974Reg = (PWM8978_REG)&WM8978CtrlReg;
	for (i = 0; i < sizeof(WM8978_REG_SET) / sizeof(WM8978_REG); i++) {
		if (pWM8974Reg->uiAddr == uiReg) {
			*puiData = pWM8974Reg->uiData;
			return TRUE;
		} else {
			pWM8974Reg++;
		}
	}

	*puiData = 0;
	return FALSE;
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
	switch (Parameter) {
	default:
		return FALSE;
	}

	return TRUE;
}

#if 0
/*
    Get Sampling Rate API for audio module

    This function return Sampling Rate for audio module

    @return void
*/
static AUDIO_SR audcodec_getSamplingRate(void)
{
	return audioSR;
}
#endif

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

static void wm8978_i2cStart(void)
{
	gpio_setDir(AUD_I2C_DAT_GPIO, GPIO_DIR_INPUT);
	gpio_setDir(AUD_I2C_CLK_GPIO, GPIO_DIR_INPUT);

	// Start Condition
	Delay_DelayUsPolling(9);    // tSU:STA/tBUF >= 4.7us

	gpio_setDir(AUD_I2C_DAT_GPIO, GPIO_DIR_OUTPUT);
	gpio_clearPin(AUD_I2C_DAT_GPIO);

	Delay_DelayUsPolling(8);    // tHD:STA >= 4us

	gpio_setDir(AUD_I2C_CLK_GPIO, GPIO_DIR_OUTPUT);
	gpio_clearPin(AUD_I2C_CLK_GPIO);
}

static void wm8978_i2cStop(void)
{
	// Ensure data is sink to LOW
	gpio_setDir(AUD_I2C_DAT_GPIO, GPIO_DIR_OUTPUT);
	gpio_clearPin(AUD_I2C_DAT_GPIO);
	Delay_DelayUsPolling(1);

	// Stop Condition
	gpio_setDir(AUD_I2C_CLK_GPIO, GPIO_DIR_INPUT);

	Delay_DelayUsPolling(8);    // tSU:STO >= 4us

	gpio_setDir(AUD_I2C_DAT_GPIO, GPIO_DIR_INPUT);
}

static ER wm8978_i2cOutBit(UINT32 uiBit)
{
	// Prepare data
	if (uiBit & 0x01) {
		gpio_setDir(AUD_I2C_DAT_GPIO, GPIO_DIR_INPUT);
	} else {
		gpio_setDir(AUD_I2C_DAT_GPIO, GPIO_DIR_OUTPUT);
		gpio_clearPin(AUD_I2C_DAT_GPIO);
	}

	// toggle CLK
	Delay_DelayUsPolling(9);    // tLOW >= 4.7us, tSU.DAT >=250ns

	gpio_setDir(AUD_I2C_CLK_GPIO, GPIO_DIR_INPUT);      // CLK Release

	Delay_DelayUsPolling(8);    // tHIGH >= 4 us

	gpio_setDir(AUD_I2C_CLK_GPIO, GPIO_DIR_OUTPUT);     // CLK LOW
	gpio_clearPin(AUD_I2C_CLK_GPIO);

	gpio_setDir(AUD_I2C_DAT_GPIO, GPIO_DIR_INPUT);

	return E_OK;
}

static UINT32 wm8978_i2cInBit(void)
{
	UINT32 uiValue;

	// toggle CLK
	Delay_DelayUsPolling(9);    // tLOW >= 4.7us, tSU.DAT >=250ns

	uiValue = gpio_getPin(AUD_I2C_DAT_GPIO);

	gpio_setDir(AUD_I2C_CLK_GPIO, GPIO_DIR_INPUT);      // CLK Release

	Delay_DelayUsPolling(8);    // tHIGH >= 4 us

	gpio_setDir(AUD_I2C_CLK_GPIO, GPIO_DIR_OUTPUT);     // CLK LOW
	gpio_clearPin(AUD_I2C_CLK_GPIO);

	if (uiValue) {
		uiValue = 1;
	}

	return uiValue;
}

static ER wm8978_i2cGetAck(void)
{
	UINT32 uiValue;

	uiValue = wm8978_i2cInBit();

	if (uiValue == 0) {
		return E_OK;
	} else {
		return E_NOEXS;
	}
}

static ER wm8978_i2cWriteByte(UINT32 uiValue)
{
	UINT32 i;
	ER erReturn;

	loc_cpu();

	for (i = 0; i < 8; i++) {
		wm8978_i2cOutBit(uiValue >> (7 - i));
	}

	erReturn = wm8978_i2cGetAck();

	unl_cpu();

	return erReturn;
}

static UINT32 wm8978_i2cReadByte(BOOL bAck)
{
	UINT32 i;
	UINT32 uiValue = 0;

	loc_cpu();

	for (i = 0; i < 8; i++) {
		uiValue |= wm8978_i2cInBit() << (7 - i);
	}

	if (bAck) {
		wm8978_i2cOutBit(0);
	} else {
		wm8978_i2cOutBit(1);
	}

	unl_cpu();

	return uiValue;
}

/*
    Write CODEC register

    @param[in] uiOffset     register address
    @param[in] uiValue      register value

    @return
        - @b E_OK: success
*/
ER wm8978_writeCtrlReg(UINT32 uiOffset, UINT32 uiValue)
{
	UINT32 retry = 3;
	ER erReturn;

	while (retry--) {
		// FPGA board
		gpio_setDir(AUD_I2C_CLK_GPIO, GPIO_DIR_INPUT);
		gpio_setDir(AUD_I2C_DAT_GPIO, GPIO_DIR_INPUT);

		wm8978_i2cStart();

		erReturn = wm8978_i2cWriteByte(WM8978_I2C_WRITE);
		if (erReturn != E_OK) {
			debug_msg("%s: send address fail\r\n", __func__);
			wm8978_i2cStop();
			Delay_DelayMs(1);
			continue;
		}
		erReturn = wm8978_i2cWriteByte(uiOffset);
		if (erReturn != E_OK) {
			debug_msg("%s: send offset 0x%lx fail\r\n", __func__, uiOffset);
			wm8978_i2cStop();
			Delay_DelayMs(1);
			continue;
		}

		if (wm8978_i2cWriteByte(uiValue) != E_OK) {
			debug_msg("wm8978_i2cWriteByte err\r\n");
		}

		wm8978_i2cStop();

		Delay_DelayMs(1);
		break;
	}

	return E_OK;
}

/*
    Read CODEC register

    @param[in] uiOffset     register address
    @param[out] puiValue    register value

    @return
        - @b E_OK: success
*/
ER wm8978_readCtrlReg(UINT32 uiOffset, UINT32 *puiValue)
{
	ER erReturn;

	// FPGA board
	gpio_setDir(AUD_I2C_CLK_GPIO, GPIO_DIR_INPUT);
	gpio_setDir(AUD_I2C_DAT_GPIO, GPIO_DIR_INPUT);

	wm8978_i2cStart();

	erReturn = wm8978_i2cWriteByte(WM8978_I2C_WRITE);
	if (erReturn != E_OK) {
		debug_msg("%s: send address fail\r\n", __func__);
		return erReturn;
	}
	erReturn = wm8978_i2cWriteByte(uiOffset);
	if (erReturn != E_OK) {
		debug_msg("%s: send offset 0x%lx fail\r\n", __func__, uiOffset);
		return erReturn;
	}
	wm8978_i2cStart(); // repeat-Start

	erReturn = wm8978_i2cWriteByte(WM8978_I2C_WRITE | 0x01);
	if (erReturn != E_OK) {
		debug_msg("%s: send read address fail\r\n", __func__);
		return erReturn;
	}

	*puiValue = wm8978_i2cReadByte(FALSE);

	wm8978_i2cStop();

	return E_OK;
}

//@}
