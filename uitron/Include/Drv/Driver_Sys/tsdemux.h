/**
    Header file for MPEG2 Transport Stream De=Muxing module

    This file is the header file for MPEG2 Transport Stream De-Muxing module driver
    The user can reference this section for the detail description of the each driver API usage
    and also the parameter descriptions and its limitations.
    The overall combinational usage flow is introduced in the application note document,
    and the user must reference to the application note for the driver usage flow.

    @file       tsdemux.h
    @ingroup    mIDrvMisc_TSDEMux
    @note       Nothing

    Copyright   Novatek Microelectronics Corp. 2016.  All rights reserved.
*/

/**
    @addtogroup mIDrvMisc_TSDEMux
*/
//@{

#ifndef _TSDEMUX_H
#define _TSDEMUX_H

#include "Type.h"





/**
    TSDEMUX module general configuration Selection

    This is used in tsdemux_setConfig()/tsdemux_getConfig() to specify which of the function is selected to assign new configuration.
*/
typedef enum {
	TSDEMUX_CFG_ID_FREQ,                ///< TSDEMUX operating frequency (unit: MHz). Context can be:
	///< - @b 160: 160 MHz
	///< - @b 240: 240 MHz

	TSDEMUX_CFG_ID_SYNC_BYTE,           ///< Set SYNC Byte checking value. Default is 0x47.
	TSDEMUX_CFG_ID_ADAPTATION_FLAG,     ///< Set Adaptation Flag checking value. Default is 0x00.

	TSDEMUX_CFG_ID_PID0_ENABLE,         ///< Enable/Disable of the filtering for TS packet's payload with PID0.
	TSDEMUX_CFG_ID_PID1_ENABLE,         ///< Enable/Disable of the filtering for TS packet's payload with PID1.
	TSDEMUX_CFG_ID_PID2_ENABLE,         ///< Enable/Disable of the filtering for TS packet's payload with PID2.

	TSDEMUX_CFG_ID_PID0_VALUE,          ///< 13bits PID-0 value.
	TSDEMUX_CFG_ID_PID1_VALUE,          ///< 13bits PID-1 value.
	TSDEMUX_CFG_ID_PID2_VALUE,          ///< 13bits PID-2 value.

	TSDEMUX_CFG_ID_CONTINUITY0_MODE,    ///<
	TSDEMUX_CFG_ID_CONTINUITY1_MODE,    ///<
	TSDEMUX_CFG_ID_CONTINUITY2_MODE,    ///<

	TSDEMUX_CFG_ID_CONTINUITY0_VALUE,   ///<
	TSDEMUX_CFG_ID_CONTINUITY1_VALUE,   ///<
	TSDEMUX_CFG_ID_CONTINUITY2_VALUE,   ///<

	TSDEMUX_CFG_ID_CONTINUITY0_LAST,    ///< Get the last continuity counter value after previous operation done.
	TSDEMUX_CFG_ID_CONTINUITY1_LAST,    ///< Get the last continuity counter value after previous operation done.
	TSDEMUX_CFG_ID_CONTINUITY2_LAST,    ///< Get the last continuity counter value after previous operation done.

	TSDEMUX_CFG_ID_IN_ADDR,             ///< Set Input Start Address
	TSDEMUX_CFG_ID_IN_SIZE,             ///< Set Input Size

	TSDEMUX_CFG_ID_OUT0_ADDR,           ///< Set Out0 Start Address
	TSDEMUX_CFG_ID_OUT1_ADDR,           ///< Set Out1 Start Address
	TSDEMUX_CFG_ID_OUT2_ADDR,           ///< Set Out2 Start Address

	TSDEMUX_CFG_ID_TOTAL,
	ENUM_DUMMY4WORD(TSDEMUX_CFG_ID)
} TSDEMUX_CFG_ID;


typedef enum {
	TSDEMUX_STATUS_OK               = 0x00000000,
	TSDEMUX_STATUS_NO_OPEN          = 0x00000010,

	TSDEMUX_STATUS_CONTINUITY_ERR0  = 0x00000100,
	TSDEMUX_STATUS_TEI0             = 0x00000200,
	TSDEMUX_STATUS_SYNC_ERR0        = 0x00000400,
	TSDEMUX_STATUS_ADAPFLG_ERR0     = 0x00000800,

	TSDEMUX_STATUS_CONTINUITY_ERR1  = 0x00010000,
	TSDEMUX_STATUS_TEI1             = 0x00020000,
	TSDEMUX_STATUS_SYNC_ERR1        = 0x00040000,
	TSDEMUX_STATUS_ADAPFLG_ERR1     = 0x00080000,

	TSDEMUX_STATUS_CONTINUITY_ERR2  = 0x01000000,
	TSDEMUX_STATUS_TEI2             = 0x02000000,
	TSDEMUX_STATUS_SYNC_ERR2        = 0x04000000,
	TSDEMUX_STATUS_ADAPFLG_ERR2     = 0x08000000,

	TSDEMUX_STATUS_SIZE_ERR         = 0x10000000,

	TSDEMUX_STATUS_ALL              = 0x0F0F0F00,
	ENUM_DUMMY4WORD(TSDEMUX_STATUS)
} TSDEMUX_STATUS;


/**
    Tsdemux request structure

    @note For tsdemux_start()
*/
typedef struct {
	BOOL   bWait;
	UINT32 IN_ADDR;
	UINT32 IN_SIZE;
	UINT32 OUT0_START_ADDR;
	UINT32 OUT1_START_ADDR;
	UINT32 OUT2_START_ADDR;
} TSDEMUX_REQUEST, *PTSDEMUX_REQUEST;


//
//  Extern APIs
//
extern ER               tsdemux_open(void);
extern ER               tsdemux_close(void);
extern BOOL             tsdemux_isOpened(void);
extern TSDEMUX_STATUS   tsdemux_start(BOOL bWait, BOOL bFlush);
extern TSDEMUX_STATUS   tsdemux_waitDone(void);
extern TSDEMUX_STATUS   tsdemux_getOutSize(UINT32 *pOutSize0, UINT32 *pOutSize1, UINT32 *pOutSize2);
extern ER               tsdemux_setConfig(TSDEMUX_CFG_ID CfgID, UINT32 uiCfgValue);
extern UINT32           tsdemux_getConfig(TSDEMUX_CFG_ID CfgID);

//@}
#endif
