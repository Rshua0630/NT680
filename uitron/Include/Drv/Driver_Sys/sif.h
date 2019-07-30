/**
    Header file for SIF module.

    This file is the header file that define the API and data type
    for SIF module.

    @file       sif.h
    @ingroup    mIDrvIO_SIF
    @note       Nothing.

    Copyright   Novatek Microelectronics Corp. 2014.  All rights reserved.
*/

#ifndef _SIF_H
#define _SIF_H

#include "Type.h"

/**
    @addtogroup mIDrvIO_SIF
*/
//@{

/**
    SIF channel ID

    SIF channel ID for related APIs.
*/
typedef enum {
	SIF_CH0,                ///< SIF channel 0
	SIF_CH1,                ///< SIF channel 1
	SIF_CH2,                ///< SIF channel 2
	SIF_CH3,                ///< SIF channel 3
	SIF_CH4,                ///< SIF channel 4
	SIF_CH5,                ///< SIF channel 5
	SIF_CH6,                ///< SIF channel 6
	SIF_CH7,                ///< SIF channel 7

	SIF_TOTAL_CHANNEL,      ///< Total SIF channel number
	ENUM_DUMMY4WORD(SIF_CH)
} SIF_CH;

/**
    SIF bus mode ID

    SIF bus mode ID for SIF_CONFIG_ID_BUSMODE.
*/
typedef enum {
	SIF_MODE_00,          ///< SIF bus mode 0
	SIF_MODE_01,          ///< SIF bus mode 1
	SIF_MODE_02,          ///< SIF bus mode 2
	SIF_MODE_03,          ///< SIF bus mode 3
	SIF_MODE_04,          ///< SIF bus mode 4
	SIF_MODE_05,          ///< SIF bus mode 5
	SIF_MODE_06,          ///< SIF bus mode 6
	SIF_MODE_07,          ///< SIF bus mode 7
	SIF_MODE_08,          ///< SIF bus mode 8
	SIF_MODE_09,          ///< SIF bus mode 9
	SIF_MODE_10,          ///< SIF bus mode 10
	SIF_MODE_11,          ///< SIF bus mode 11
	SIF_MODE_12,          ///< SIF bus mode 12
	SIF_MODE_13,          ///< SIF bus mode 13
	SIF_MODE_14,          ///< SIF bus mode 14
	SIF_MODE_15,          ///< SIF bus mode 15

	ENUM_DUMMY4WORD(SIF_MODE)
} SIF_MODE;

/**
    SIF shift out direction ID

    SIF shift out direction ID for SIF_CONFIG_ID_DIR.
*/
typedef enum {
	SIF_DIR_LSB,            ///< LSb shifted out first
	SIF_DIR_MSB,            ///< MSb shifted out first

	ENUM_DUMMY4WORD(SIF_DIR)
} SIF_DIR;

/**
    SIF continuous mode ON/OFF

    SIF continuous mode ON/OFF setting for SIF_CONFIG_ID_CONTMODE.
    Only valid when SIF is in DMA mode.

*/
typedef enum {
	SIF_CONTMODE_OFF,       ///< Continuous mode is OFF
	SIF_CONTMODE_ON,        ///< Continuous mode is ON

	ENUM_DUMMY4WORD(SIF_CONTMODE)
} SIF_CONTMODE;

/**
    SIF interrupt destination ID

    SIF interrupt destination ID for SIF_CONFIG_ID_DESTINATION.
*/
typedef enum {
	SIF_DEST_CPU1,            ///< interrupt to CPU1
	SIF_DEST_CPU2,            ///< interrupt to CPU2

	ENUM_DUMMY4WORD(SIF_DEST)
} SIF_DEST;

/**
    SIE VD source

    SIE VD source  for SIF_CONFIG_ID_BURST_SRC.
*/
typedef enum {
	SIF_VD_SIE1,            ///< VD source from SIE channel 1
	SIF_VD_SIE2,            ///< VD source from SIE channel 2
	SIF_VD_SIE3,            ///< VD source from SIE channel 3
	SIF_VD_SIE4,            ///< VD source from SIE channel 4
	SIF_VD_SIE5,            ///< VD source from SIE channel 5
	SIF_VD_SIE6,            ///< VD source from SIE channel 6
	SIF_VD_SIE7,            ///< VD source from SIE channel 7
	SIF_VD_SIE8,            ///< VD source from SIE channel 8

	ENUM_DUMMY4WORD(SIF_VD_SOURCE)
} SIF_VD_SOURCE;

/**
    SIF Configuration ID

    SIF Configuration ID for sif_setConfig() and sif_getConfig()

*/
typedef enum {
	SIF_CONFIG_ID_BUSMODE,          ///< Bus mode. Please refer to Application Note for more detailed information.
	///< @note  Valid value: SIF_MODE_00 ~ SIF_MODE_15

	SIF_CONFIG_ID_BUSCLOCK,         ///< Bus clock. Must be configured before SIF_CONFIG_ID_SEND, SIF_CONFIG_ID_SENS,
	///< SIF_CONFIG_ID_SENH and SIF_CONFIG_ID_DELAY.
	///< @note  Valid value: 187,500 ~ 48,000,000 , unit: Hz.
	///<        Real bus clock might lower than your setting.
	///<        Please call sif_getConfig(SIF_CONFIG_ID_BUSCLOCK) to confirm.
	///<        Formula: Bus Clock = 96MHz / (2 * (CLKDIV + 1))

	SIF_CONFIG_ID_SEND,             ///< SEN duration for SIF_MODE_08 ~ SIF_MODE_15. Must be configured after SIF_CONFIG_ID_BUSCLOCK.
	///< Please refer to Application Note for more detailed information.
	///< @note  Valid value: (1 ~ 16) * half bus clock, unit: ns. Default: (16 * half bus clock) ns.
	///<        Real SEN duration might greater than your setting.
	///<        Please call sif_getConfig(SIF_CONFIG_ID_SEND) to confirm.

	SIF_CONFIG_ID_SENS,             ///< SEN setup time for SIF_MODE_00 ~ SIF_MODE_07. Must be configured after SIF_CONFIG_ID_BUSCLOCK.
	///< Please refer to Application Note for more detailed information.
	///< @note  Valid value: (0 ~ 15) * half bus clock, unit: ns. Default: 0 ns.
	///<        Real SEN setup time might greater than your setting.
	///<        Please call sif_getConfig(SIF_CONFIG_ID_SENS) to confirm.

	SIF_CONFIG_ID_SENH,             ///< SEN hold time for all modes. Must be configured after SIF_CONFIG_ID_BUSCLOCK.
	///< Please refer to Application Note for more detailed information.
	///< @note  Valid value: (0 ~ 7) * half bus clcok, unit: ns. Default: 0 ns.
	///<        Real SEN hold time might greater than your setting.
	///<        Please call sif_getConfig(SIF_CONFIG_ID_SENH) to confirm.

	SIF_CONFIG_ID_LENGTH,           ///< How many bits will be transfered per transmission.
	///< @note  Valid value: 1 ~ 255

	SIF_CONFIG_ID_DIR,              ///< Shift out direction
	///< - @b SIF_DIR_LSB   : LSb is shifted out first (Defalut value)
	///< - @b SIF_DIR_MSB   : MSb is shifted out first

	SIF_CONFIG_ID_CONTMODE,         ///< Continuous mode ON/OFF
	///< - @b SIF_CONTMODE_OFF  : Continuous mode is OFF (Default value)
	///< - @b SIF_CONTMODE_ON   : Continuous mode is ON

	SIF_CONFIG_ID_CONTADDRBITS,     ///< Continuous mode address bits number
	///< @note  Valid value: 1 ~ 255

	SIF_CONFIG_ID_DELAY,            ///< Delay between successive transmissions.
	///< Must be configured after SIF_CONFIG_ID_BUSMODE, SIF_CONFIG_ID_BUSCLOCK,
	///< SIF_CONFIG_ID_SENH and SIF_CONFIG_ID_SEND.
	///< @note  Valid value: 63 ~ 85,062 , unit: ns. Default: 63 ns.
	///<        Real delay might greater than your setting.
	///<        Please call sif_getConfig(SIF_CONFIG_ID_DELAY) to confirm.

	SIF_CONFIG_ID_AUTOPINMUX,       ///< Auto switch pinmux to SIF and GPIO when sif_open() and sif_close().
	///< - @b FALSE : Don't switch pinmux (Default value)
	///< - @b TRUE  : Switch to SIF when sif_open(), switch to GPIO when sif_close()

	SIF_CONFIG_ID_BURST_DELAY,      ///< Delay time after SIE VD signal in each burst transmission.
	///< - @note Valid value: 0 ~ 100000000 (100 ms), unit: ns. Default 0 ns.
	///< - The mininum unit is 10 ns

	SIF_CONFIG_ID_BURST_INTVAL1,    ///< VD intervals between 1st and 2nd burst transmissions.
	///< - @note Valid value: 0x0~0x7. Default 0x0.

	SIF_CONFIG_ID_BURST_INTVAL2,    ///< VD intervals between 2nd and 3rd burst transmissions
	///< - @note Valid value: 0x0~0x7. Default 0x0

	SIF_CONFIG_ID_BURST_SRC,        ///< Input Soure of SIE VD channel 1~8
	///< - @note Valid value: 0x0~0x7. Default 0x0

	SIF_CONFIG_ID_DESTINATION,      ///< SIF Interrupt Destination
	///< - @note Valid value: 0x0~0x1. Default 0x0

	ENUM_DUMMY4WORD(SIF_CONFIG_ID)
} SIF_CONFIG_ID;

extern ER       sif_open(SIF_CH Ch);
extern ER       sif_close(SIF_CH Ch);
extern BOOL     sif_isOpened(SIF_CH Ch);

extern void     sif_setConfig(SIF_CH Ch, SIF_CONFIG_ID ConfigID, UINT32 uiConfig);
extern UINT32   sif_getConfig(SIF_CH Ch, SIF_CONFIG_ID ConfigID);

extern void     sif_send(SIF_CH Ch, UINT32 uiData0, UINT32 uiData1, UINT32 uiData2);
extern void     sif_waitTransferEnd(SIF_CH Ch);

extern void     sif_sendDMA(SIF_CH Ch, UINT32 uiAddr, UINT32 uiDMALength);
extern void     sif_waitDMATransferEnd(SIF_CH Ch);

extern void     sif_send_burst_DMA(SIF_CH Ch, UINT32 uiAddr, UINT8 uiBurstNumber, UINT32 uiDMALength1, UINT32 uiDMALength2, UINT32 uiDMALength3);
//@}

#endif
