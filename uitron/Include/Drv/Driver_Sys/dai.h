/**
    Digital Audio Interface(DAI) Controller global header

    This file is the header file that define the API and data type
    for DAI module

    @file       dai.h
    @ingroup    mIDrvAud_DAI
    @note       Nothing.

    Copyright   Novatek Microelectronics Corp. 2017.  All rights reserved.
*/



#ifndef _DAI_H
#define _DAI_H

#include "Type.h"
#include "Driver.h"

/**
    @addtogroup mIDrvAud_DAI
*/
//@{



/**
    Digital Audio interface(DAI) general configuration Selection.

    This is used in dai_setConfig() to specify which of the function is selected to assign new configuration.
*/
typedef enum {
	DAI_CONFIG_ID_EXTCODEC_EN,          ///< Enable/Disable External Codec. If external codec is disabled, the internal codec is enabled.
	DAI_CONFIG_ID_ISRCB,                ///< Assign ISR Callback event handler. Please use "DRV_CB" as callback prototype.
										///< This is valid only if dai driver is opened.
	DAI_CONFIG_ID_HDMI_TXEN,            ///< Set DAI Transmit to HDMI Enable/Disable. If disabled, the HDMI audio would be forced MUTE.
	DAI_CONFIG_ID_AVSYNC_EN,            ///< Set Record AV Sync Function Enable.
	DAI_CONFIG_ID_AVSYNC_SRC,           ///< Set Record AV Sync Source from SIE or SIE2.

	DAI_CONFIG_ID_CLKSRC,               ///< Select Codec system clock source. Please use "DAI_CODESCK" as input parameter.
										///< Only DAI_CODESCK_INT is valid for NT96680.
	DAI_CONFIG_ID_SET_INTEN,            ///< Set DAI controller interrupt enable. Please use "DAI_INTERRUPT" as bitwise input parameter.
	DAI_CONFIG_ID_CLR_INTEN,            ///< Clear DAI controller interrupt enable. Please use "DAI_INTERRUPT" as bitwise input parameter.
	DAI_CONFIG_ID_CLR_INTSTS,           ///< Clear DAI controller interrupt status. Please use "DAI_INTERRUPT" as bitwise input parameter.


	ENUM_DUMMY4WORD(DAI_CONFIG_ID)
} DAI_CONFIG_ID;

/**
    DAI Interrupt

    @note Used for dai_setConfig(DAI_CONFIG_ID_SET_INTEN / DAI_CONFIG_ID_CLR_INTEN).
*/
typedef enum {
	DAI_TX1DMADONE_INT      = 0X00000001,///< DMA buffer done for Playback Channel 1 (TX1)
	DAI_TX2DMADONE_INT      = 0X00000002,///< DMA buffer done for Playback Channel 2 (TX2)
	DAI_RX1DMADONE_INT      = 0X00000004,///< DMA buffer done for   Record Channel 1 (RX1)
	DAI_RX2DMADONE_INT      = 0X00000008,///< DMA buffer done for   Record Channel 2 (RX2)

	DAI_TX1DMALOAD_INT      = 0x00000010,///< DMA start address loaded for Playback Channel 1 (TX1)
	DAI_TX2DMALOAD_INT      = 0x00000020,///< DMA start address loaded for Playback Channel 1 (TX1)
	DAI_RXDMALOAD_INT       = 0x00000040,///< DMA start address loaded for Record Channel (RX-1&2)

	DAI_TX1STOP_INT         = 0x00000100,///< DAI Playback Channel 1 stopped. (TX1)
	DAI_TX2STOP_INT         = 0x00000200,///< DAI Playback Channel 2 stopped. (TX2)
	DAI_RXSTOP_INT          = 0x00000400,///< DAI Record Channel stopped. (RX-1&2)

	DAI_TX1BWERR_INT        = 0x00001000,///< DAI Playback Channel 1 BandWidth Not enough.(TX1)
	DAI_TX2BWERR_INT        = 0x00002000,///< DAI Playback Channel 2 BandWidth Not enough.(TX2)
	DAI_RX1BWERR_INT        = 0x00004000,///< DAI Record Channel 1 BandWidth Not enough.(RX1)
	DAI_RX2BWERR_INT        = 0x00008000,///< DAI Record Channel 2 BandWidth Not enough.(RX2)

	DAI_TX1TCHIT_INT        = 0x00010000,///< Time Code Hit for Playback Channel 1.(TX1)
	DAI_RXTCHIT_INT         = 0x00040000,///< Time Code Hit for Record Channe.(RX-1&2)

	DAI_RXTCLATCH_INT       = 0x00100000,///< Time Code Latch for Record Channe.(RX-1&2)

	DAI_TXLBDMADONE_INT     = 0X01000000,///< DMA buffer done for Playback Loopback Channel(TXLB)
	DAI_TXLBDMALOAD_INT     = 0x02000000,///< DMA start address loaded for Playback Loopback Channel(TXLB)
	DAI_TXLBSTOP_INT        = 0x04000000,///< DAI Playback Loopback Channel stopped. (TXLB)
	DAI_TXLBBWERR_INT       = 0x08000000,///< DAI Playback Loopback Channel BandWidth Not enough.(TXLB)

	DAI_INTERRUPT_ALL       = 0x0F15F77F,///< DAI All interrupt
	DAI_INTERRUPT_TXLB_ALL  = 0x0F000000,///< DAI TXLB All interrupt
	ENUM_DUMMY4WORD(DAI_INTEN)
} DAI_INTERRUPT;

/**
    DAI system clock source

    @note Used for dai_setConfig(DAI_CONFIG_ID_CLKSRC).
*/
typedef enum {
	DAI_CODESCK_INT,                    ///< internal clock
	DAI_CODESCK_EXT,                    ///< external clock

	ENUM_DUMMY4WORD(DAI_CODESCK)
} DAI_CODESCK;



/**
    DAI Playback Channel Selection ID

    @note Used for dai_setTxConfig() to select specified playback channel configuration.
*/
typedef enum {
	DAI_TXCH_TX1,                       ///< DAI Playback Channel 1 (TX1) Select ID
	DAI_TXCH_TX2,                       ///< DAI Playback Channel 2 (TX2) Select ID

	ENUM_DUMMY4WORD(DAI_TXCH)
} DAI_TXCH;

/**
    DAI playback configuration select

    @note Used for dai_setTxConfig(DAI_TXCH) and dai_getTxConfig(DAI_TXCH).
*/
typedef enum {
	DAI_TXCFG_ID_CHANNEL,               ///< Configure playback channel as mono-left/right or stereo on the audio interface.
										///< Please use "DAI_CH" as input parameter. DAI_CH_DUAL_MONO is not valid setting for playback.
	DAI_TXCFG_ID_TOTAL_CH,              ///< Configure Total Channel Number on interface. If embedded audio codec is selected, this field must be set to DAI_TOTCH_2CH.
										///< Please use "DAI_TOTCH" as input parameter.
	DAI_TXCFG_ID_PCMLEN,                ///< Configure playback source PCM bit length. Please use  "DAI_DRAMPCMLEN" as input parameter.
	DAI_TXCFG_ID_DRAMCH,                ///< Configure the playback source bitstream on the DRAM is stereo or mono audio data.
										///< Please use "DAI_DRAMSOUNDFMT" as input parameter.
	DAI_TXCFG_ID_TIMECODE_TRIG,         ///< Configure Playback Time Code Trigger Value. This is valid only for DAI_TXCH_TX1.
										///< This field is valid for the 32bits value.
	DAI_TXCFG_ID_TIMECODE_OFS,          ///< Configure Playback Time Code Offset Value. This is valid only for DAI_TXCH_TX1.
										///< This field is valid for the 32bits value.
	DAI_TXCFG_ID_TIMECODE_VAL,          ///< Get Playback TimeCode value. This is valid only for dai_getTxConfig(DAI_TXCH_TX1,DAI_TXCFG_ID_TIMECODE_VAL).

	ENUM_DUMMY4WORD(DAI_TXCFG_ID)
} DAI_TXCFG_ID;

/**
    DAI playback loopback configuration select

    @note Used for dai_setTxlbConfig() and dai_getTxlbConfig().
*/
typedef enum {
	DAI_TXLBCFG_ID_CHANNEL = 0x1000,    ///< Configure playback loopback channel as mono-left/right or stereo on the audio interface.
										///< Please use "DAI_CH" as input parameter. DAI_CH_DUAL_MONO is not valid setting for playback loopback.
	DAI_TXLBCFG_ID_TOTAL_CH,            ///< Configure Total Channel Number on interface. If embedded audio codec is selected, this field must be set to DAI_TOTCH_2CH.
										///< Please use "DAI_TOTCH" as input parameter.
	DAI_TXLBCFG_ID_PCMLEN,              ///< Configure playback loopback source PCM bit length. Please use  "DAI_DRAMPCMLEN" as input parameter.
	DAI_TXLBCFG_ID_DRAMCH,              ///< Configure the playback loopback source bitstream on the DRAM is stereo or mono audio data.
										///< Please use "DAI_DRAMSOUNDFMT" as input parameter.
	DAI_TXLBCFG_ID_RXSYNC,              ///< ENABLE/DISABLE the TXLB start operation sync to RX start.

	ENUM_DUMMY4WORD(DAI_TXLBCFG_ID)
} DAI_TXLBCFG_ID;

/**
    DAI record configuration select

    @note Used for dai_setRxConfig() and dai_getRxConfig().
*/
typedef enum {
	DAI_RXCFG_ID_CHANNEL = 0x2000,      ///< Configure record channel as mono-left/right, dual-mono or stereo on the audio interface.
										///< Please use "DAI_CH" as input parameter.
	DAI_RXCFG_ID_TOTAL_CH,              ///< Configure Total Channel Number on interface. If embedded audio codec is selected, this field must be set to DAI_TOTCH_2CH.
										///< Please use "DAI_TOTCH" as input parameter.
	DAI_RXCFG_ID_PCMLEN,                ///< Configure recorded destination PCM bit length on DRAM. Please use  "DAI_DRAMPCMLEN" as input parameter.
	DAI_RXCFG_ID_DRAMCH,                ///< Configure the recording destination bitstream on the DRAM is stereo or mono audio data.
										///< Please use "DAI_DRAMSOUNDFMT" as input parameter.
	DAI_RXCFG_ID_TIMECODE_TRIG,         ///< Configure Record Channel Time Code Trigger Value. This field is valid for the 32bits value.
	DAI_RXCFG_ID_TIMECODE_OFS,          ///< Configure Record Time Code Offset Value. This field is valid for the 32bits value.
	DAI_RXCFG_ID_TIMECODE_VAL,          ///< Get Record TimeCode value. This is valid only for dai_getTxConfig(DAI_RXCFG_ID_TIMECODE_VAL).


	ENUM_DUMMY4WORD(DAI_RXCFG_ID)
} DAI_RXCFG_ID;

/**
    DAI Playback(TX)/Record(RX) channel

    @note Used for dai_setTxConfig(DAI_TXCH, DAI_TXCFG_ID_CHANNEL) & dai_setRxConfig(DAI_RXCFG_ID_CHANNEL).
*/
typedef enum {
	DAI_CH_DUAL_MONO,                   ///< Dual Mono Channels. This is valid only for Record.
	DAI_CH_STEREO,                      ///< Stereo Chennels.
	DAI_CH_MONO_LEFT,                   ///< Mono Left Channel
	DAI_CH_MONO_RIGHT,                  ///< Mono Right Channel

	ENUM_DUMMY4WORD(DAI_CH)
} DAI_CH;

/**
    Total Audio Channels transceive on interface.

    If embedded audio codec is selected, only DAI_TOTCH_2CH is valid. 4/6/8-CH are valid for I2S only.

    @note Used for dai_setTxConfig(DAI_TXCH, DAI_TXCFG_ID_TOTAL_CH) & dai_setRxConfig(DAI_RXCFG_ID_TOTAL_CH).
*/
typedef enum {
	DAI_TOTCH_2CH,                      ///< Total 2 Audio Channels on interface.
	DAI_TOTCH_4CH,                      ///< Total 4 Audio Channels on interface. Valid only for I2S.
	DAI_TOTCH_6CH,                      ///< Total 6 Audio Channels on interface. Valid only for I2S.
	DAI_TOTCH_8CH,                      ///< Total 8 Audio Channels on interface. Valid only for I2S.

	ENUM_DUMMY4WORD(DAI_TOTCH)
} DAI_TOTCH;

/**
    DAI DRAM format

    @note Used for dai_setTxConfig(DAI_TXCH, DAI_TXCFG_ID_DRAMCH) & dai_setRxConfig(DAI_TXCFG_ID_DRAMCH).
*/
typedef enum {
	DAI_DRAMPCM_STEREO,                 ///< Stereo
	DAI_DRAMPCM_MONO,                   ///< Mono

	ENUM_DUMMY4WORD(DAI_DRAMSOUNDFMT)
} DAI_DRAMSOUNDFMT;

/**
    DAI PCM length

    @note Used for @note Used for dai_setTxConfig(DAI_TXCH, DAI_TXCFG_ID_PCMLEN) & dai_setRxConfig(DAI_RXCFG_ID_PCMLEN).
*/
typedef enum {
	DAI_DRAMPCM_8,                      ///< 8bits
	DAI_DRAMPCM_16,                     ///< 16bits
	DAI_DRAMPCM_32,                     ///< 32bits. Valid only for I2S Only.

	ENUM_DUMMY4WORD(DAI_DRAMPCMLEN)
} DAI_DRAMPCMLEN;



/**
    DAI I2S interface general configuration Selection

    This is used in dai_setI2sConfig() to specify which of the function is selected to assign new configuration.
*/
typedef enum {
	DAI_I2SCONFIG_ID_CLKRATIO,          ///< Select I2S clock ratio setting.
										///< Please use "DAI_I2SCLKR" as input parameter. Available values are below:
										///< \n DAI_I2SCLKR_256FS_32BIT:  SystemClk = 256 FrameSync, FrameSync =  32 BitClk
										///< \n DAI_I2SCLKR_256FS_64BIT:  SystemClk = 256 FrameSync, FrameSync =  64 BitClk
										///< \n DAI_I2SCLKR_256FS_128BIT: SystemClk = 256 FrameSync, FrameSync = 128 BitClk
										///< \n DAI_I2SCLKR_256FS_256BIT: SystemClk = 256 FrameSync, FrameSync = 256 BitClk
	DAI_I2SCONFIG_ID_FORMAT,            ///< Configure I2S Format. Please use "DAI_I2SFMT" as input parameter.
										///< Only DAI_I2SFMT_STANDARD is valid for NT96680.
	DAI_I2SCONFIG_ID_OPMODE,            ///< Configure I2S Operating mode as Master or Slave. Please use "DAI_OPMODE" as input parameter.
	DAI_I2SCONFIG_ID_CHANNEL_LEN,       ///< Configure I2S interface how many bit-clock cycles per audio channel.
										///< Please use "DAI_I2SCHLEN" as input parameter.
	DAI_I2SCONFIG_ID_HDMICH_SEL,        ///< During I2S TDM 4/6/8 CH playback, this field select which two channels to HDMI audio.
										///< Please use "DAI_I2SHDMI_SEL" as input parameters.
	DAI_I2SCONFIG_ID_SLAVEMATCH,        ///< Get I2S Slave Clock Ratio Setting is matched with the settings or not.

	ENUM_DUMMY4WORD(DAI_I2SCONFIG_ID)
} DAI_I2SCONFIG_ID;

/**
    HDMI Audio Channel Select when I2S TDM 4/6/8 CH.

    This is used in dai_setI2sConfig(DAI_I2SCONFIG_ID_HDMICH_SEL)
*/
typedef enum {
	DAI_I2SHDMI_SEL_CH01,       ///< During I2S TDM 4/6/8CH playback, channel 0/1 are transmited to HDMI audio.
	DAI_I2SHDMI_SEL_CH23,       ///< During I2S TDM 4/6/8CH playback, channel 2/3 are transmited to HDMI audio.
	DAI_I2SHDMI_SEL_CH45,       ///< During I2S TDM   6/8CH playback, channel 4/5 are transmited to HDMI audio.
	DAI_I2SHDMI_SEL_CH67,       ///< During I2S TDM     8CH playback, channel 6/7 are transmited to HDMI audio.

	ENUM_DUMMY4WORD(DAI_I2SHDMI_SEL)
} DAI_I2SHDMI_SEL;


/**
    DAI I2S clock ratio

    @note Used for dai_setI2sConfig(DAI_I2SCONFIG_ID_CLKRATIO).
*/
typedef enum {
	DAI_I2SCLKR_256FS_32BIT,            ///< SystemClk = 256 * FrameSync, FrameSync = 32 BitClk
	DAI_I2SCLKR_256FS_64BIT,            ///< SystemClk = 256 * FrameSync, FrameSync = 64 BitClk
	DAI_I2SCLKR_256FS_128BIT,           ///< SystemClk = 256 * FrameSync, FrameSync = 128 BitClk
	DAI_I2SCLKR_256FS_256BIT,           ///< SystemClk = 256 * FrameSync, FrameSync = 256 BitClk

	DAI_I2SCLKR_384FS_32BIT,            ///< SystemClk = 384 * FrameSync, FrameSync = 32 BitClk
	DAI_I2SCLKR_384FS_48BIT,            ///< SystemClk = 384 * FrameSync, FrameSync = 48 BitClk
	DAI_I2SCLKR_384FS_96BIT,            ///< SystemClk = 384 * FrameSync, FrameSync = 96 BitClk

	DAI_I2SCLKR_512FS_32BIT,            ///< SystemClk = 512 * FrameSync, FrameSync = 32 BitClk
	DAI_I2SCLKR_512FS_64BIT,            ///< SystemClk = 512 * FrameSync, FrameSync = 64 BitClk
	DAI_I2SCLKR_512FS_128BIT,           ///< SystemClk = 512 * FrameSync, FrameSync = 128 BitClk

	DAI_I2SCLKR_768FS_32BIT,            ///< SystemClk = 768 * FrameSync, FrameSync = 32 BitClk
	DAI_I2SCLKR_768FS_48BIT,            ///< SystemClk = 768 * FrameSync, FrameSync = 48 BitClk
	DAI_I2SCLKR_768FS_64BIT,            ///< SystemClk = 768 * FrameSync, FrameSync = 64 BitClk
	DAI_I2SCLKR_768FS_192BIT,           ///< SystemClk = 768 * FrameSync, FrameSync = 192 BitClk

	DAI_I2SCLKR_1024FS_32BIT,           ///< SystemClk = 1024 * FrameSync, FrameSync = 32 BitClk
	DAI_I2SCLKR_1024FS_64BIT,           ///< SystemClk = 1024 * FrameSync, FrameSync = 64 BitClk
	DAI_I2SCLKR_1024FS_128BIT,          ///< SystemClk = 1024 * FrameSync, FrameSync = 128 BitClk
	DAI_I2SCLKR_1024FS_256BIT,          ///< SystemClk = 1024 * FrameSync, FrameSync = 256 BitClk

	ENUM_DUMMY4WORD(DAI_I2SCLKR)
} DAI_I2SCLKR;

/**
    DAI I2S format

    @note Use for dai_setI2sConfig(DAI_I2SCONFIG_ID_FORMAT)
*/
typedef enum {
	DAI_I2SFMT_STANDARD,                ///< I2S Standard
	DAI_I2SFMT_DSP,                     ///< I2S DSP mode
	DAI_I2SFMT_LIKE_MSB,                ///< I2S Like, MSB justified
	DAI_I2SFMT_LIKE_LSB,                ///< I2S Like, LSB justified

	ENUM_DUMMY4WORD(DAI_I2SFMT)
} DAI_I2SFMT;

/**
    DAI I2S OP mode

    @note Used for dai_setI2sConfig(DAI_I2SCONFIG_ID_OPMODE)
*/
typedef enum {
	DAI_OP_SLAVE,                       ///< Slave mode
	DAI_OP_MASTER,                      ///< Master mode

	ENUM_DUMMY4WORD(DAI_OPMODE)
} DAI_OPMODE;

/**
    DAI I2S Channel Length

    @note Use for dai_setI2sConfig(DAI_I2SCONFIG_ID_CHANNEL_LEN)
*/
typedef enum {
	DAI_I2SCHLEN_16BITS,                ///< I2S interface is 16 bit-clocks per audio channel
	DAI_I2SCHLEN_32BITS,                ///< I2S interface is 32 bit-clocks per audio channel

	ENUM_DUMMY4WORD(DAI_I2SCHLEN)
} DAI_I2SCHLEN;

/**
    DAI AVSYNC Source Select

    @note Use for dai_setConfig(DAI_CONFIG_ID_AVSYNC_SRC)
*/
typedef enum {
	DAI_AVSYNCSRC_SIE,                  ///< AVSYNC Source from SIE.
	DAI_AVSYNCSRC_SIE2,                 ///< AVSYNC Source from SIE2.
	DAI_AVSYNCSRC_SIE3,                 ///< AVSYNC Source from SIE3.
	DAI_AVSYNCSRC_SIE4,                 ///< AVSYNC Source from SIE4.
	DAI_AVSYNCSRC_SIE5,                 ///< AVSYNC Source from SIE5.
	DAI_AVSYNCSRC_SIE6,                 ///< AVSYNC Source from SIE6.
	DAI_AVSYNCSRC_SIE7,                 ///< AVSYNC Source from SIE7.
	DAI_AVSYNCSRC_SIE8,                 ///< AVSYNC Source from SIE8.

	ENUM_DUMMY4WORD(DAI_AVSYNCSRC)
} DAI_AVSYNCSRC;






//
// Export APIs in digital Audio interface
//

/**
    Open digital audio controller driver.

    Open digital auiod controller.

    @param[in] pIsrHdl      Callback function registered for interrupt notification.

    @return void
*/
extern void     dai_open(DRV_CB pIsrHdl);

/**
    Close digital audio controller driver.

    Close digital auiod controller.

    @return void
*/
extern void     dai_close(void);

/**
    Set Digital Audio interface(DAI) General Configurations.

    Set Digital Audio interface(DAI) general configuration.
    Use DAI_CONFIG_ID as configuration selection and uiCfgValue is the configuration parameter.

    @param[in] ConfigID     Configuration selection. Please refer to DAI_CONFIG_ID for details.
    @param[in] uiCfgValue   configuration parameter. Please refer to DAI_CONFIG_ID for details.

    @return void
*/
extern void     dai_setConfig(DAI_CONFIG_ID ConfigID, UINT32 uiCfgValue);

/**
    Get Digital Audio interface(DAI) General Configurations.

    Get Digital Audio interface(DAI) general configuration.
    Use DAI_CONFIG_ID as configuration selection and uiCfgValue is the configuration parameter.

    @param[in] ConfigID     Configuration selection. Please refer to DAI_CONFIG_ID for details.

    @return void
*/
extern UINT32   dai_getConfig(DAI_CONFIG_ID ConfigID);

/**
    Set Digital Audio I2S interface configurations

    Use DAI_I2SCONFIG_ID as configuration selection and uiCfgValue is the configuration parameter.

    @param[in] ConfigID     Configuration selection. Please refer to DAI_I2SCONFIG_ID for details.
    @param[in] uiCfgValue   configuration parameter. Please refer to DAI_I2SCONFIG_ID for details.

    @return void
*/
extern void     dai_setI2sConfig(DAI_I2SCONFIG_ID ConfigID, UINT32 uiCfgValue);

/**
    Get Digital Audio I2S interface configurations

    Use DAI_I2SCONFIG_ID as configuration selection and uiCfgValue is the configuration parameter.

    @param[in] ConfigID     Configuration selection. Please refer to DAI_I2SCONFIG_ID for details.

    @return void
*/
extern UINT32   dai_getI2sConfig(DAI_I2SCONFIG_ID ConfigID);

/**
    Set DAI Playback Chaneels Configurations.

    Set DAI Playback Chaneels(TX1 and TX2) Configurations.

    @param[in]  Chan        playback Channel selection
    @param[in]  ConfigID    playback config ID selection
    @param[in]  uiCfgValue  configuration value

    @return void
*/
extern void     dai_setTxConfig(DAI_TXCH Chan, DAI_TXCFG_ID ConfigID, UINT32 uiCfgValue);

/**
    Get DAI Playback Chaneels Configurations.

    Get DAI Playback Chaneels(TX1 and TX2) Configurations.

    @param[in]  Chan        playback Channel selection
    @param[in]  ConfigID    playback config ID selection

    @return configuration value
*/
extern UINT32   dai_getTxConfig(DAI_TXCH Chan, DAI_TXCFG_ID ConfigID);

/**
    Set DAI Playback Loopback Chaneel Configurations.

    Set DAI Playback Loopback Chaneels(TXLB) Configurations.

    @param[in]  Chan        playback Loopback Channel selection
    @param[in]  ConfigID    playback Loopback config ID selection
    @param[in]  uiCfgValue  configuration value

    @return void
*/
extern void     dai_setTxlbConfig(DAI_TXLBCFG_ID ConfigID, UINT32 uiCfgValue);

/**
    Set DAI Playback Loopback Chaneel Configurations.

    Set DAI Playback Loopback Chaneels(TXLB) Configurations.

    @param[in]  Chan        playback Loopback Channel selection
    @param[in]  ConfigID    playback Loopback config ID selection
    @param[in]  uiCfgValue  configuration value

    @return void
*/
extern UINT32   dai_getTxlbConfig(DAI_TXLBCFG_ID ConfigID);

/**
    Set DAI Record Channels Configurations.

    Set DAI Record Channels (RX1 & RX2) Configurations.

    @param[in]  ConfigID    record config ID selection
    @param[in]  uiCfgValue  configuration value

    @return void
*/
extern void     dai_setRxConfig(DAI_RXCFG_ID ConfigID, UINT32 uiCfgValue);

/**
    Get DAI Record Channels Configurations.

    Get DAI Record Channels (RX1 & RX2) Configurations.

    @param[in]  ConfigID    record config ID selection

    @return configuration value
*/
extern UINT32   dai_getRxConfig(DAI_RXCFG_ID ConfigID);

/**
    Enable/Disable DAI playback DMA Channel

    Enable/Disable DAI playback DMA Channel

    @param[in]  Chan        playback Channel selection
    @param[in]  bEn         TRUE is Enable. FALSE is Disable.

    @return void
*/
extern void     dai_enableTxDMA(DAI_TXCH Chan, BOOL bEn);

/**
    Enable/Disable DAI record DMA Channel

    Enable/Disable DAI record DMA Channel

    @param[in]  bEn         TRUE is Enable. FALSE is Disable.

    @return void
*/
extern void     dai_enableRxDMA(BOOL bEn);

/**
    Enable/Disable DAI playback loopback DMA Channel

    Enable/Disable DAI playback loopback DMA Channel

    @param[in]  bEn         TRUE is Enable. FALSE is Disable.

    @return void
*/
extern void     dai_enableTxlbDMA(BOOL bEn);

/**
    Start/Stop DAI Playback

    Start/Stop DAI Specified Channel Playback

    @param[in]  Chan        playback Channel selection
    @param[in]  bEn         TRUE is Start. FALSE is Stop.

    @return void
*/
extern void     dai_enableTx(DAI_TXCH Chan, BOOL bEn);

/**
    Start/Stop DAI Record

    Start/Stop DAI Specified Channel Record

    @param[in]  bEn         TRUE is Start. FALSE is Stop.

    @return void
*/
extern void     dai_enableRx(BOOL bEn);

/**
    Start/Stop DAI playback loopback

    Start/Stop DAI Specified Channel Record

    @param[in]  bEn         TRUE is Start. FALSE is Stop.

    @return void
*/
extern void     dai_enableTxlb(BOOL bEn);

/**
    Set DAI module enable/disable

    Set DAI module enable/disable

    @param[in] bEn      DAI module enable/disable
     - @b TRUE:     module enable
     - @b FALSE:    module disable

    @return void
*/
extern void     dai_enableDAI(BOOL bEn);

/**
    Check whether DAI is enabled or not

    If DAI is enabled this function will return TRUE.

    @return
        - @b TRUE:  DAI is enabled
        - @b FALSE: DAI is disabled
*/
extern BOOL     dai_isDAIEnable(void);

/**
    Check if DAI Playback Channel is enabled

    Check if DAI Playback Channel is enabled

    @param[in]  Chan        playback Channel selection

    @return
        - @b TRUE:  Specified Playback channel is enabled
        - @b FALSE: Specified Playback channel is disabled
*/
extern BOOL     dai_isTxEnable(DAI_TXCH Chan);

/**
    Check if DAI record Channel is enabled

    Check if DAI record Channel is enabled

    @return
        - @b TRUE:  Record is enabled
        - @b FALSE: Record is disabled
*/
extern BOOL     dai_isRxEnable(void);

/**
    Check if DAI playback loopback Channel is enabled

    Check if DAI playback loopback Channel is enabled

    @return
        - @b TRUE:  playback loopback is enabled
        - @b FALSE: playback loopback is disabled
*/
extern BOOL     dai_isTxlbEnable(void);

/**
    Check whether DAI is under tx/rx

    If DAI is under tx/rx, this function will return TRUE.

    @return
        - @b TRUE:  DAI is under tx/rx.
        - @b FALSE: DAI is not under tx/rx.

*/
extern BOOL     dai_isTXRXEnable(void);

/**
    Check if playback DMA Channel is Enabled

    Check if playback DMA Channel is Enabled

    @param[in]  Chan        playback Channel selection

    @return
        - @b TRUE:  Specified Playback DMA channel is enabled
        - @b FALSE: Specified Playback DMA channel is disabled
*/
extern BOOL     dai_isTxDMAEnable(DAI_TXCH Chan);

/**
    Check if Record DMA Channel is Enabled

    Check if Record DMA Channel is Enabled

    @return
        - @b TRUE:  Specified Record DMA channel is enabled
        - @b FALSE: Specified Record DMA channel is disabled
*/
extern BOOL     dai_isRxDMAEnable(void);

/**
    Check if Playback Loopback DMA Channel is Enabled

    Check if Playback Loopback DMA Channel is Enabled

    @return
        - @b TRUE:  Specified Playback Loopback DMA channel is enabled
        - @b FALSE: Specified Playback Loopback DMA channel is disabled
*/
extern BOOL     dai_isTxlbDMAEnable(void);

/**
    Check whether Tx or Rx DMA is enabled or not

    If Tx or Rx DMA is enabled this function will return TRUE.

    @return
        - @b TRUE:  DMA is enabled
        - @b FALSE: DMA is disabled
*/
extern BOOL     dai_isDMAEnable(void);

/**
    Set Playback DMA parameter

    Set Playback DMA starting address, buffer size.

    @param[in] uiChan       Playback DMA Channel Selection. Valid value is 0 or 1 for NT96680.
    @param[in] uiStadr      DMA start address. (unit: byte, should be word-alignment)
    @param[in] uiBufSz      DMA buffer size (unit: 16 words aligned)

    @return void
*/
extern void     dai_setTxDMAPara(UINT32 uiChan, UINT32 uiStadr, UINT32 uiBufSz);

/**
    Set Record DMA parameter

    Set Record DMA starting address, buffer size.

    @param[in] uiChan       Record DMA Channel Selection. Valid value is 0 or 1 for NT96680.
    @param[in] uiStadr      DMA start address. (unit: byte, should be word-alignment)
    @param[in] uiBufSz      DMA buffer size (unit: 16 words aligned)

    @return void
*/
extern void     dai_setRxDMAPara(UINT32 uiChan, UINT32 uiStadr, UINT32 uiBufSz);

/**
    Set Playback Loopback DMA parameter

    Set Playback Loopback DMA starting address, buffer size.

    @param[in] uiStadr      DMA start address. (unit: byte, should be word-alignment)
    @param[in] uiBufSz      DMA buffer size (unit: 16 words aligned)

    @return void
*/
extern void     dai_setTxlbDMAPara(UINT32 uiStadr, UINT32 uiBufSz);

/**
    Get Playback DMA parameter

    Get Playback DMA starting address, buffer size.

    @param[in] uiChan       Playback DMA Channel Selection. Valid value is 0 or 1 for NT96680.
    @param[out] uiStadr     DMA start address. (unit: byte, should be word-alignment)
    @param[out] uiBufSz     DMA buffer size (unit: word)

    @return void
*/
extern void     dai_getTxDMAPara(UINT32 uiChan, UINT32 *puiStadr, UINT32 *puiBufSz);

/**
    Get Record DMA parameter

    Get Record DMA starting address, buffer size.

    @param[in] uiChan       Record DMA Channel Selection. Valid value is 0 or 1 for NT96680.
    @param[out] uiStadr      DMA start address. (unit: byte, should be word-alignment)
    @param[out] uiBufSz      DMA buffer size (unit: word)

    @return void
*/
extern void     dai_getRxDMAPara(UINT32 uiChan, UINT32 *puiStadr, UINT32 *puiBufSz);

/**
    Get Playback Loopback DMA parameter

    Get Playback Loopback DMA starting address, buffer size.

    @param[in] uiStadr      DMA start address. (unit: byte, should be word-alignment)
    @param[in] uiBufSz      DMA buffer size (unit: word)

    @return void
*/
extern void     dai_getTxlbDMAPara(UINT32 *puiStadr, UINT32 *puiBufSz);

/**
    Get Playback DMA currrent address

    Get Playback DMA currrent address

    @param[in] uiChan   Playback DMA Channel Selection. Valid value is 0 or 1 for NT96680.

    @return Playback DMA current address
*/
extern UINT32   dai_getTxDMACurrAdr(UINT32 uiChan);

/**
    Get Record DMA currrent address

    Get Record DMA currrent address

    @param[in] uiChan   Record DMA Channel Selection. Valid value is 0 or 1 for NT96680.

    @return Record DMA current address
*/
extern UINT32   dai_getRxDMACurrAdr(UINT32 uiChan);

/**
    Get Playback Loopback DMA currrent address

    Get Playback Loopback DMA currrent address

    @return Playback Loopback DMA current address
*/
extern UINT32   dai_getTxlbDMACurrAdr(void);

/**
    Clear DAI flag

    Clear DAI flag

    @param[in] IntFlag  interrupt flags to clear.

    @return void
*/
extern void     dai_clrFlg(DAI_INTERRUPT IntFlag);

//@}
#endif
