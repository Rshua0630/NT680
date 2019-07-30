/**
    Header file for MPEG2 Transport Stream Muxing module

    This file is the header file for MPEG2 Transport Stream Muxing module driver
    The user can reference this section for the detail description of the each driver API usage
    and also the parameter descriptions and its limitations.
    The overall combinational usage flow is introduced in the application note document,
    and the user must reference to the application note for the driver usage flow.

    @file       tsmux.h
    @ingroup    mIDrvMisc_TSMux
    @note       Nothing

    Copyright   Novatek Microelectronics Corp. 2014.  All rights reserved.
*/

/**
    @addtogroup mIDrvMisc_TSMux
*/
//@{

#ifndef _TSMUX_H
#define _TSMUX_H

#include "Type.h"





/**
    TSMUX module general configuration Selection

    This is used in tsmux_setConfig()/tsmux_getConfig() to specify which of the function is selected to assign new configuration.
*/
typedef enum {
	TSMUX_CFG_ID_PAYLOADSIZE,       ///< Configure the TS packet payload size. Currently support only 184 or 176 Bytes only.
	///< The input value must use 184 or 176. Others no support.
	TSMUX_CFG_ID_SRC_ADDR,          ///< Muxing input buffer address. This input address can be byte aligned.
	TSMUX_CFG_ID_DST_ADDR,          ///< Muxing input buffer address. This output address can be 8-bytes aligned.
	TSMUX_CFG_ID_SRC_LENGTH,        ///< Muxing input bit stream length in bytes. Valid value from 0x1 to 0x3ffffff.

	TSMUX_CFG_ID_DST_BUFSIZE,       ///< Set the destination buffer size. If the destination buffer is not enough for source input,
	///< it would output warning messages as notifications.
	TSMUX_CFG_ID_MUXING_LEN,        ///< Get Last Mux output size.

	TSMUX_CFG_ID_SYNC_BYTE,         ///< TS Stream header's Field: Sync Byte.
	TSMUX_CFG_ID_CONTINUITY_CNT,    ///< TS Stream header's Field: continuity counter initial value. Valid value 0x0 ~ 0xF.
	TSMUX_CFG_ID_PID,               ///< TS Stream header's Field: 13-bits Packet ID. Valid value 0x0 ~ 0x1FFF.
	TSMUX_CFG_ID_TEI,               ///< TS Stream header's Field: Transport Error Indicator. Valid value 0 or 1.
	TSMUX_CFG_ID_TP,                ///< TS Stream header's Field: Transport Priority. Valid value 0 or 1.
	TSMUX_CFG_ID_SCRAMBLECTRL,      ///< TS Stream header's Field: Scrambling Control. Valid value 0~3. The value 0 means Not Scrambled.

	TSMUX_CFG_ID_STUFF_VAL,         ///< Set Stuff values. Default value is 0x00.
	TSMUX_CFG_ID_ADAPT_FLAGS,       ///< Set 8bits adaptation field flags.

	TSMUX_CFG_ID_ACCMUXING_LEN,     ///< Get Accumulated Mux output size after last flush.

	TSMUX_CFG_ID_TOTAL,
	ENUM_DUMMY4WORD(TSMUX_CFG_ID)
} TSMUX_CFG_ID;




//
//  Extern APIs
//
extern ER       tsmux_open(void);
extern ER       tsmux_close(void);
extern BOOL     tsmux_isOpened(void);
extern ER       tsmux_start(BOOL bWait, BOOL bStartIndicator, BOOL bFlush);
extern ER       tsmux_waitDone(void);
extern ER       tsmux_setConfig(TSMUX_CFG_ID CfgID, UINT32 uiCfgValue);
extern UINT32   tsmux_getConfig(TSMUX_CFG_ID CfgID);

//@}
#endif
