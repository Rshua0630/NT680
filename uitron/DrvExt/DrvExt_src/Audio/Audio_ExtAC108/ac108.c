/*
    Emaltion External audio codec driver

    This file is the driver for Emaltion extended audio codec.

    @file       AudExtEMU.c
    @ingroup    mISYSAud
    @note       Nothing.

    Copyright   Novatek Microelectronics Corp. 2016.  All rights reserved.
*/

/** \addtogroup mISYSAud */
//@{

#include "ac108.h"
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


static PI2C_OBJ 	pI2cObjAc108;
static I2C_SESSION 	I2cSessAc108 = I2C_TOTAL_SESSION;

static BOOL			Ac108GainBalance = FALSE;
static UINT32 		DigitalGain = 0xA0;



static AUDIO_CODEC_FUNC     EXTEMUCodecFP    = {
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
	(void *)AUDIO_CODEC_FUNC_USE_DEFAULT,
	// Set Gain (Record)
	audcodec_setGain,
	// Set GainDB (Record)
	audcodec_setGainDB,
	// Set Effect
	audcodec_setEffect,
	// Set Feature
	audcodec_setFeature,
	// StopRecord
	audcodec_stopRecord,
	// StopPlayback
	(void *)AUDIO_CODEC_FUNC_USE_DEFAULT,
	// Record Preset
	NULL,
	// Record
	audcodec_record,
	// Playback
	(void *)AUDIO_CODEC_FUNC_USE_DEFAULT,
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
	(void *)AUDIO_CODEC_FUNC_USE_DEFAULT,
	// Close
	(void *)AUDIO_CODEC_FUNC_USE_DEFAULT,
};


#if 1
static void audext_ac108_write(UINT32 Addr, UINT32 Value)
{
	I2C_DATA    I2cData;
	I2C_BYTE    I2cByte[4];

	I2cData.VersionInfo     = DRV_VER_96680;
	I2cData.pByte           = I2cByte;
	I2cData.ByteCount       = I2C_BYTE_CNT_3;

	I2cByte[0].uiValue      = (0x3B<<1);
	I2cByte[0].Param        = I2C_BYTE_PARAM_START;
	I2cByte[1].uiValue      = Addr & 0xFF;
	I2cByte[1].Param        = I2C_BYTE_PARAM_NONE;
	I2cByte[2].uiValue      = Value & 0xFF;
	I2cByte[2].Param        = I2C_BYTE_PARAM_STOP;

	pI2cObjAc108->lock(I2cSessAc108);

	if (pI2cObjAc108->transmit(&I2cData) != I2C_STS_OK) {
		pI2cObjAc108->unlock(I2cSessAc108);
		debug_msg("i2c transmit err 1!\r\n");
		return;
	}

	pI2cObjAc108->unlock(I2cSessAc108);

}

static UINT32 audext_ac108_read(UINT32 Addr)
{
	I2C_DATA    I2cData;
	I2C_BYTE    I2cByte[4];

	I2cData.VersionInfo     = DRV_VER_96680;
	I2cData.pByte           = I2cByte;
	I2cData.ByteCount       = I2C_BYTE_CNT_2;

	I2cByte[0].uiValue      = (0x3B<<1);
	I2cByte[0].Param        = I2C_BYTE_PARAM_START;
	I2cByte[1].uiValue      = Addr & 0xFF;
	I2cByte[1].Param        = I2C_BYTE_PARAM_NONE;

	pI2cObjAc108->lock(I2cSessAc108);

	if (pI2cObjAc108->transmit(&I2cData) != I2C_STS_OK) {
		pI2cObjAc108->unlock(I2cSessAc108);
		debug_msg("i2c transmit err 1!\r\n");
		return 0;
	}

	I2cData.ByteCount       = I2C_BYTE_CNT_1;

	I2cByte[0].uiValue      = (0x3B<<1) + 1;
	I2cByte[0].Param        = I2C_BYTE_PARAM_START;
	if (pI2cObjAc108->transmit(&I2cData) != I2C_STS_OK) {
		pI2cObjAc108->unlock(I2cSessAc108);
		debug_msg("i2c transmit err 2!\r\n");
		return 0;
	}

	I2cData.ByteCount       = I2C_BYTE_CNT_1;
	I2cByte[0].Param    	= I2C_BYTE_PARAM_NACK | I2C_BYTE_PARAM_STOP;
	pI2cObjAc108->receive(&I2cData);

	pI2cObjAc108->unlock(I2cSessAc108);

	return I2cByte[0].uiValue;
}

static void audext_ac108_update(UINT32 Addr, UINT32 Mask, UINT32 Value)
{
	UINT32 temp;

	temp = audext_ac108_read(Addr);
	temp &= (~Mask);
	audext_ac108_write(Addr, temp|(Value&Mask));
}
#endif

void audExtAC108_getFP(PAUDIO_CODEC_FUNC pAudCodecFunc)
{
	memcpy((void *)pAudCodecFunc, (const void *)&EXTEMUCodecFP, sizeof(AUDIO_CODEC_FUNC));
}
static void audcodec_setDeviceObject(PAUDIO_DEVICE_OBJ pAudDevObj)
{

	pI2cObjAc108 = i2c_getDrvObject(pAudDevObj->uiGPIOData);
	if(pI2cObjAc108 == NULL) {
		debug_msg("i2c_getDrvObject failed\r\n");
	}

	if(pI2cObjAc108->open(&I2cSessAc108) != E_OK) {
		debug_msg("i2c open failed\r\n");
	}

	pI2cObjAc108->setConfig(I2cSessAc108, I2C_CONFIG_ID_PINMUX,       pAudDevObj->uiGPIOClk);
	pI2cObjAc108->setConfig(I2cSessAc108, I2C_CONFIG_ID_BUSCLOCK,     I2C_BUS_CLOCK_100KHZ);
	pI2cObjAc108->setConfig(I2cSessAc108, I2C_CONFIG_ID_HANDLE_NACK,  TRUE);
}

static void audcodec_init(PAUDIO_SETTING pAudio)
{
	UINT32 temp,vec;
	UINT32 m1,m2,n,k1,k2;


	if((pI2cObjAc108 == NULL)||(I2cSessAc108 == I2C_TOTAL_SESSION)) {
		debug_msg("I2C Channel Init Failed\r\n");
	}

	aud_setI2SChBits(AUDIO_I2SCH_BITS_32);


	//reset
	//audext_ac108_write(CHIP_RST, CHIP_RST_VAL);
	//Delay_DelayMs(3);


	#if 1
	// SYSCLK_SRC_PLL
	audext_ac108_update(SYSCLK_CTRL, 0x1 << SYSCLK_SRC, SYSCLK_SRC_PLL << SYSCLK_SRC);
	#else
	// SYSCLK_SRC_MCLK
	audext_ac108_update(SYSCLK_CTRL, 0x1 << SYSCLK_SRC, SYSCLK_SRC_MCLK << SYSCLK_SRC);
	#endif


	/*
		ac108_set_fmt
	*/

	#if 1
	// slave mode
	// 0x30:chip is slave mode, BCLK & LRCK input,enable SDO1_EN and
	//  SDO2_EN, Transmitter Block Enable, Globe Enable
	audext_ac108_update(I2S_CTRL, 0x03 << LRCK_IOEN | 0x03 << SDO1_EN | 0x1 << TXEN | 0x1 << GEN,
						  0x00 << LRCK_IOEN | 0x03 << SDO1_EN | 0x1 << TXEN | 0x1 << GEN);
	#else
	// master mode
	audext_ac108_update(I2S_CTRL, 0x03 << LRCK_IOEN | 0x03 << SDO1_EN | 0x1 << TXEN | 0x1 << GEN,
					  0x00 << LRCK_IOEN | 0x03 << SDO1_EN | 0x1 << TXEN | 0x1 << GEN);
	/* multi_chips: only one chip set as Master, and the others also need to set as Slave */
	audext_ac108_update(I2S_CTRL, 0x3 << LRCK_IOEN, 0x3 << LRCK_IOEN);
	#endif


	/* ac108_configure_power */
	//0x06:Enable Analog LDO
	audext_ac108_update(PWR_CTRL6, 0x01 << LDO33ANA_ENABLE, 0x01 << LDO33ANA_ENABLE);
	// 0x07:
	// Control VREF output and micbias voltage ?
	// REF faststart disable, enable Enable VREF (needed for Analog
	// LDO and MICBIAS)
	audext_ac108_update(PWR_CTRL7, 0x1f << VREF_SEL | 0x01 << VREF_FASTSTART_ENABLE | 0x01 << VREF_ENABLE,
					   0x13 << VREF_SEL | 0x00 << VREF_FASTSTART_ENABLE | 0x01 << VREF_ENABLE);
	// 0x09:
	// Disable fast-start circuit on VREFP
	// VREFP_RESCTRL=00=1 MOhm
	// IGEN_TRIM=100=+25%
	// Enable VREFP (needed by all audio input channels)
	audext_ac108_update(PWR_CTRL9, 0x01 << VREFP_FASTSTART_ENABLE | 0x03 << VREFP_RESCTRL | 0x07 << IGEN_TRIM | 0x01 << VREFP_ENABLE,
					   0x00 << VREFP_FASTSTART_ENABLE | 0x00 << VREFP_RESCTRL | 0x04 << IGEN_TRIM | 0x01 << VREFP_ENABLE);


	//0x31: 0: normal mode, negative edge drive and positive edge sample
	//1: invert mode, positive edge drive and negative edge sample
	audext_ac108_update(I2S_BCLK_CTRL,  0x01 << BCLK_POLARITY, BCLK_NORMAL_DRIVE_N_SAMPLE_P << BCLK_POLARITY);
	// 0x32: same as 0x31
	audext_ac108_update(I2S_LRCK_CTRL1, 0x01 << LRCK_POLARITY, LRCK_LEFT_HIGH_RIGHT_LOW << LRCK_POLARITY);

	// 0x34:Encoding Mode Selection,Mode
	// Selection,data is offset by 1 BCLKs to LRCK
	// normal mode for the last half cycle of BCLK in the slot ?
	// turn to hi-z state (TDM) when not transferring slot ?
	audext_ac108_update(I2S_FMT_CTRL1,	0x01 << ENCD_SEL | 0x03 << MODE_SEL | 0x01 << TX2_OFFSET |
						0x01 << TX1_OFFSET | 0x01 << TX_SLOT_HIZ | 0x01 << TX_STATE,
								  0 << ENCD_SEL 	|
								  LEFT_JUSTIFIED_FORMAT << MODE_SEL 		|
								  1 << TX2_OFFSET 			|
								  1 << TX1_OFFSET 			|
								  0x00 << TX_SLOT_HIZ 				|
								  0x01 << TX_STATE);

	// 0x60:
	// MSB / LSB First Select: This driver only support MSB First Select .
	// OUT2_MUTE,OUT1_MUTE shoule be set in widget.
	// LRCK = 1 BCLK width
	// Linear PCM
	//  TODO:pcm mode, bit[0:1] and bit[2] is special
	audext_ac108_update(I2S_FMT_CTRL3,	0x01 << TX_MLS | 0x03 << SEXT  | 0x01 << LRCK_WIDTH | 0x03 << TX_PDM,
						0x00 << TX_MLS | 0x03 << SEXT  | 0x00 << LRCK_WIDTH | 0x00 << TX_PDM);

	audext_ac108_write(HPF_EN, 0x00);

	m1 = 4;
	m2 = 0;
	n  = 128;
	k1 = 24;
	k2 = 0;

	/*
		ac108_hw_params
	*/

	// TDM mode or normal mode
	audext_ac108_write(I2S_LRCK_CTRL2, 128 - 1);
	audext_ac108_update(I2S_LRCK_CTRL1, 0x03 << 0, 0x00);

	/**
	 * 0x35:
	 * TX Encoding mode will add  4bits to mark channel number
	 * TODO: need a chat to explain this
	 */
	audext_ac108_update(I2S_FMT_CTRL2, 0x07 << SAMPLE_RESOLUTION | 0x07 << SLOT_WIDTH_SEL,
						7 << SAMPLE_RESOLUTION
 						  | 7 << SLOT_WIDTH_SEL);

	/**
	 * 0x60:
	 * ADC Sample Rate synchronised with I2S1 clock zone
	 */
	audext_ac108_update(ADC_SPRC, 0x0f << ADC_FS_I2S1, 8 << ADC_FS_I2S1);
	audext_ac108_write(HPF_EN, 0x0F);

	/*ac108_config_pll(ac10x, ac108_sample_rate[rate].real_val, 0); */
	/* 0x11,0x12,0x13,0x14: Config PLL DIV param M1/M2/N/K1/K2 */
	audext_ac108_update(PLL_CTRL5, 0x1f << PLL_POSTDIV1 | 0x01 << PLL_POSTDIV2,
					   k1 << PLL_POSTDIV1 | k2 << PLL_POSTDIV2);
	audext_ac108_update(PLL_CTRL4, 0xff << PLL_LOOPDIV_LSB, (unsigned char)n << PLL_LOOPDIV_LSB);
	audext_ac108_update(PLL_CTRL3, 0x03 << PLL_LOOPDIV_MSB, (n >> 8) << PLL_LOOPDIV_MSB);
	audext_ac108_update(PLL_CTRL2, 0x1f << PLL_PREDIV1 | 0x01 << PLL_PREDIV2,
					    m1 << PLL_PREDIV1 | m2 << PLL_PREDIV2);

	/*0x18: PLL clk lock enable*/
	audext_ac108_update(PLL_LOCK_CTRL, 0x1 << PLL_LOCK_EN, 0x1 << PLL_LOCK_EN);

	/*0x10: PLL Common voltage Enable, PLL Enable,PLL loop divider factor detection enable*/
	audext_ac108_update(PLL_CTRL1, 0x01 << PLL_EN | 0x01 << PLL_COM_EN | 0x01 << PLL_NDET,
					   0x01 << PLL_EN | 0x01 << PLL_COM_EN | 0x01 << PLL_NDET);

	/**
	 * 0x20: enable pll, pll source from mclk/bclk, sysclk source from pll, enable sysclk
	 */
	audext_ac108_update(SYSCLK_CTRL, 0x01 << PLLCLK_EN | 0x03  << PLLCLK_SRC | 0x01 << SYSCLK_SRC | 0x01 << SYSCLK_EN,
					     0x01 << PLLCLK_EN |0<< PLLCLK_SRC | 0x01 << SYSCLK_SRC | 0x01 << SYSCLK_EN);

	audext_ac108_update(I2S_BCLK_CTRL, 0x0F << BCLKDIV, 2 << BCLKDIV);

	/*
		ac108_multi_chips_slots
	*/
	vec = 0xFUL;
	//audext_ac108_write(I2S_TX1_CTRL1, 8 - 1);
	audext_ac108_write(I2S_TX1_CTRL1, 4 - 1);
	audext_ac108_write(I2S_TX1_CTRL2, (vec >> 0) & 0xFF);
	audext_ac108_write(I2S_TX1_CTRL3, (vec >> 8) & 0xFF);

#if 1
	vec = (0x2 << 0 | 0x3 << 2 | 0x0 << 4 | 0x1 << 6);
#else
	//2CH TEST
	vec = (0x2 << 0 | 0x0 << 2 | 0x3 << 4 | 0x1 << 6);
#endif
	audext_ac108_write(I2S_TX1_CHMP_CTRL1, (vec >>  0) & 0xFF);
	audext_ac108_write(I2S_TX1_CHMP_CTRL2, (vec >>  8) & 0xFF);
	audext_ac108_write(I2S_TX1_CHMP_CTRL3, (vec >> 16) & 0xFF);
	audext_ac108_write(I2S_TX1_CHMP_CTRL4, (vec >> 24) & 0xFF);

	/* Digital Gain Default value */
	audext_ac108_write(ADC1_DVOL_CTRL, 0xA7);//CH3
	audext_ac108_write(ADC2_DVOL_CTRL, 0xA7);//CH2
	audext_ac108_write(ADC3_DVOL_CTRL, 0xA7);//CH1
	audext_ac108_write(ADC4_DVOL_CTRL, 0x9C);//CH0
	//audext_ac108_write(ADC1_DVOL_CTRL, 0x98);//CH3
	//audext_ac108_write(ADC2_DVOL_CTRL, 0x98);//CH2
	//audext_ac108_write(ADC3_DVOL_CTRL, 0x98);//CH1
	//audext_ac108_write(ADC4_DVOL_CTRL, 0x98);//CH0


	/* PGA Gain Default value */
	audext_ac108_write(ANA_PGA1_CTRL, 0x00<<1);
	audext_ac108_write(ANA_PGA2_CTRL, 0x00<<1);
	audext_ac108_write(ANA_PGA3_CTRL, 0x00<<1);
	audext_ac108_write(ANA_PGA4_CTRL, 0x00<<1);

	// MIC-BIAS ON
	audext_ac108_update(ANA_ADC1_CTRL1, 0x07 << ADC1_MICBIAS_EN,  0x07 << ADC1_MICBIAS_EN);
	audext_ac108_update(ANA_ADC2_CTRL1, 0x07 << ADC2_MICBIAS_EN,  0x07 << ADC2_MICBIAS_EN);
	audext_ac108_update(ANA_ADC3_CTRL1, 0x07 << ADC3_MICBIAS_EN,  0x07 << ADC3_MICBIAS_EN);
	audext_ac108_update(ANA_ADC4_CTRL1, 0x07 << ADC4_MICBIAS_EN,  0x07 << ADC4_MICBIAS_EN);

	audext_ac108_write(ADC_DIG_EN, 		0x1F);
	audext_ac108_write(ANA_ADC4_CTRL7, 	0x0F);
	audext_ac108_write(ANA_ADC4_CTRL6, 	0x20);

	/*
		ac108_trigger
	*/
	temp = audext_ac108_read(I2S_CTRL);
	if ((temp & (0x02 << LRCK_IOEN)) && (temp & (0x01 << LRCK_IOEN)) == 0) {
		/* disable global clock */
		audext_ac108_update(I2S_CTRL, 0x1 << TXEN | 0x1 << GEN, 0x1 << TXEN | 0x0 << GEN);
	}
	/*0x21: Module clock enable<I2S, ADC digital, MIC offset Calibration, ADC analog>*/
	audext_ac108_write(MOD_CLK_EN, 1 << _I2S | 1 << ADC_DIGITAL | 1 << MIC_OFFSET_CALIBRATION | 1 << ADC_ANALOG);
	/*0x22: Module reset de-asserted<I2S, ADC digital, MIC offset Calibration, ADC analog>*/
	audext_ac108_write(MOD_RST_CTRL, 1 << _I2S | 1 << ADC_DIGITAL | 1 << MIC_OFFSET_CALIBRATION | 1 << ADC_ANALOG);

	// AC108 as Master!!
	audext_ac108_write(I2S_CTRL, 		audext_ac108_read(I2S_CTRL)|0xC0);

	//audext_ac108_write(0x20, 0x89);
	//audext_ac108_write(0x38, 0x03);
	//audext_ac108_write(0x60, 0x03);//*****not set!

	#if 0
	{
		UINT32 i;
		debug_msg("\r\n      0x0  0x1  0x2  0x3  0x4  0x5  0x6  0x7  0x8  0x9  0xA  0xB  0xC  0xD  0xE  0xF");
		for(i=0;i<256;i++) {
			if(!(i&0xF))
				debug_msg("\r\n0x%02X ",i);
			debug_msg("0x%02X ",audext_ac108_read(i));
		}
		debug_msg("\r\n");
	}

	#endif

}

static void audcodec_setRecordSource(AUDIO_RECSRC RecSrc)
{}
static void audcodec_setOutput(AUDIO_OUTPUT Output)
{}
static void audcodec_setSamplingRate(AUDIO_SR SamplingRate)
{

	if(SamplingRate == AUDIO_SR_48000) {
		audext_ac108_update(I2S_BCLK_CTRL, 0x0F << BCLKDIV, 2 << BCLKDIV);
	} else if (SamplingRate == AUDIO_SR_16000) {
		audext_ac108_update(I2S_BCLK_CTRL, 0x0F << BCLKDIV, 4 << BCLKDIV);
	} else if (SamplingRate == AUDIO_SR_8000) {
		audext_ac108_update(I2S_BCLK_CTRL, 0x0F << BCLKDIV, 6 << BCLKDIV);
	} else if (SamplingRate == AUDIO_SR_24000) {
		audext_ac108_update(I2S_BCLK_CTRL, 0x0F << BCLKDIV, 3 << BCLKDIV);
	}

}
static void audcodec_setChannel(AUDIO_CH Channel)
{}

static void audcodec_setGain(AUDIO_GAIN Gain)
{
	UINT32 value;

	//debug_msg("audcodec_setGain %d\r\n",Gain);

	if(Gain == AUDIO_GAIN_MUTE) {
		value = 0;

		audext_ac108_write(ADC1_DVOL_CTRL, 0x00);
		audext_ac108_write(ADC2_DVOL_CTRL, 0x00);
		audext_ac108_write(ADC3_DVOL_CTRL, 0x00);
		audext_ac108_write(ADC4_DVOL_CTRL, 0x00);

		audext_ac108_write(ANA_PGA1_CTRL, value<<1);
		audext_ac108_write(ANA_PGA2_CTRL, value<<1);
		audext_ac108_write(ANA_PGA3_CTRL, value<<1);
		audext_ac108_write(ANA_PGA4_CTRL, value<<1);

	} else {

		value = ((Gain - AUDIO_GAIN_0)<<2)+3;

		audext_ac108_write(ADC1_DVOL_CTRL, DigitalGain);
		audext_ac108_write(ADC2_DVOL_CTRL, DigitalGain);
		audext_ac108_write(ADC3_DVOL_CTRL, DigitalGain);
		audext_ac108_write(ADC4_DVOL_CTRL, DigitalGain);

		/* PGA Gain Max 0x1F */
		audext_ac108_write(ANA_PGA1_CTRL, value<<1); // Our Ch3

		audext_ac108_write(ANA_PGA3_CTRL, value<<1); // Our CH1

		if(Ac108GainBalance) {
			//making volume balance
			if(value>6)
				value-=6;
			else
				value=0;
		}
		audext_ac108_write(ANA_PGA4_CTRL, value<<1); // Our CH0
		audext_ac108_write(ANA_PGA2_CTRL, value<<1); // Our CH2
	}

}
static void audcodec_setGainDB(FLOAT db)
{
	UINT32 value;

	debug_msg("audcodec_setGainDB %f\r\n",db);
	if(db > 31)
		db = 31;
	else if (db < 0)
		db = 0;
	value = (UINT32)db;

	/* PGA Gain Max 0x1F */
	audext_ac108_write(ANA_PGA1_CTRL, value<<1); // Our Ch3
	audext_ac108_write(ANA_PGA3_CTRL, value<<1); // Our CH1

	if(Ac108GainBalance) {
		//making volume balance
		if(value>6)
			value-=6;
		else
			value=0;
	}
	audext_ac108_write(ANA_PGA4_CTRL, value<<1); // Our CH0
	audext_ac108_write(ANA_PGA2_CTRL, value<<1); // Our CH2

}

static void audcodec_setEffect(AUDIO_EFFECT Effect)
{}
static BOOL audcodec_setFeature(AUDIO_FEATURE Feature, BOOL bEnable)
{
	if(Feature == AUDIO_FEATURE_MICBOOST) {
		Ac108GainBalance = bEnable;
		debug_msg("set Pga Gain balance %s\r\n",Ac108GainBalance?"ENABLE":"DISABLE");
	}

	return TRUE;
}
static void audcodec_stopRecord(void)
{}
static void audcodec_record(void)
{}
static void audcodec_setFormat(AUDIO_I2SFMT I2SFmt)
{}
static void audcodec_setClockRatio(AUDIO_I2SCLKR I2SCLKRatio)
{}
static BOOL audcodec_sendCommand(UINT32 uiReg, UINT32 uiData)
{
	audext_ac108_write(uiReg, uiData);
	return TRUE;
}
static BOOL audcodec_readData(UINT32 uiReg, UINT32 *puiData)
{
	*puiData = audext_ac108_read(uiReg);
	return TRUE;
}
static BOOL audcodec_setParameter(AUDIO_PARAMETER Parameter, UINT32 uiSetting)
{
	if(Parameter == AUDIO_PARAMETER_RECORD_DIGITAL_GAIN) {
		DigitalGain = uiSetting;
		audext_ac108_write(ADC1_DVOL_CTRL, DigitalGain);
		audext_ac108_write(ADC2_DVOL_CTRL, DigitalGain);
		audext_ac108_write(ADC3_DVOL_CTRL, DigitalGain);
		audext_ac108_write(ADC4_DVOL_CTRL, DigitalGain);
		debug_msg("SetDGAIN=0x%02X\r\n",DigitalGain);
	}


	return TRUE;
}
static BOOL audcodec_chkSamplingRate(void)
{
	return TRUE;
}



//@}
