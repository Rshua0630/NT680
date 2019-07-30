#ifndef _CAN_H
#define _CAN_H

#include "Type.h"
#include "Driver.h"

typedef enum {
	CAN_OP_MODE_NORMAL = 0,
	CAN_OP_MODE_SLEEP  = 1,
	CAN_OP_MODE_LOOP   = 2,
	CAN_OP_MODE_LISTEN = 3,
	CAN_OP_MODE_CONFIG = 4,
	CAN_OP_MODE_BITS   = 7,

	ENUM_DUMMY4WORD(CAN_OP_MODE)
} CAN_OP_MODE, *PCAN_OP_MODE;


/**
    CANBUS module general configuration Selection

    This is used in canbus_setConfig()/canbus_getConfig() to specify which of the function is selected to assign new configuration.
*/
typedef enum {
	CANBUS_CFG_ID_BUSOFF,         ///< (Read only)
	CANBUS_CFG_ID_TACTIVE,        ///< (Read only)
	CANBUS_CFG_ID_RACTIVE,        ///< (Read only)

	CANBUS_CFG_ID_LBMI,           ///< Internal Loopback
	CANBUS_CFG_ID_LBME,           ///< External Loopback

	CANBUS_CFG_ID_STBY,           ///< Transceiver Standby Mode. Default is Disable.
	CANBUS_CFG_ID_LOM,            ///< Listen Only Mode. Default is Disable.
	CANBUS_CFG_ID_TBSEL,          ///< Transmit Buffer Select. Default is PTB (high-priority buffer).(Read only)

	CANBUS_CFG_ID_TSSTAT,         ///< Transmission Secondary Status bits
	CANBUS_CFG_ID_TSMODE,         ///< Transmit buffer Secondary operation. 0 for FIFO / 1 for priority decision mode.
	CANBUS_CFG_ID_TSNEXT,         ///< Transmit buffer Secondary NEXT

	CANBUS_CFG_ID_RSTAT,          ///<
	CANBUS_CFG_ID_RREL,           ///< Receive buffer Release
	CANBUS_CFG_ID_ROV,            ///< Receive buffer Overflow
	CANBUS_CFG_ID_ROM,            ///< Receive buffer Overflow Mode

	CANBUS_CFG_ID_EWL,            ///< Programmable Error Warning Limit = (EWL+1)*8.
	CANBUS_CFG_ID_AFWL,           ///< Receive buffer Almost Full Warning Limit.

	CANBUS_CFG_ID_SSPOFF,         ///< Secondary Sample Point Offset
	CANBUS_CFG_ID_TDCEN,          ///< Receive buffer Almost Full Warning Limit.

	CANBUS_CFG_ID_ALC,            ///< Arbitration Lost Capture.(Read only)
	CANBUS_CFG_ID_KOER,           ///< Kind of Error.(Read only)

	CANBUS_CFG_ID_RECNT,          ///< Receive Error Count.(Read only)
	CANBUS_CFG_ID_TECNT,          ///< Transmit Error Count.(Read only)

	CANBUS_CFG_ID_SELMASK,        ///< Select acceptance MASK. 0 for code / 1 for mask.
	CANBUS_CFG_ID_ACFADR,         ///< Acceptance filter address

	CANBUS_CFG_ID_AE,             ///< Acceptance filter enable
	CANBUS_CFG_ID_ACF,            ///< Acceptance filter control bits
	CANBUS_CFG_ID_AIDE,           ///< Acceptance mask IDE bit check enable
	CANBUS_CFG_ID_AIDEE,          ///< Acceptance mask IDE bit value

	CANBUS_CFG_ID_TOTAL,
	ENUM_DUMMY4WORD(CANBUS_CFG_ID)
} CANBUS_CFG_ID;


/**
    CANBUS module bit timing configuration Selection

    This is used in canbus_settiming()/canbus_gettiming() to specify which of the function is selected to assign new configuration.
*/
typedef enum {
	///< Transmit Secondary Buffer mode selection.
	CANBUS_TIME_ID_S_SEG1,        ///< Bit Timing Segment 1 (slow speed)
	CANBUS_TIME_ID_S_SEG2,        ///< Bit Timing Segment 2 (slow speed)
	CANBUS_TIME_ID_S_SJW,         ///< Synchronization Jump Width (slow speed)
	CANBUS_TIME_ID_S_PRESC,       ///< Prescaler (slow speed)

	CANBUS_TIME_ID_F_SEG1,        ///< Bit Timing Segment 1 (fast speed)
	CANBUS_TIME_ID_F_SEG2,        ///< Bit Timing Segment 2 (fast speed)
	CANBUS_TIME_ID_F_SJW,         ///< Synchronization Jump Width (fast speed)
	CANBUS_TIME_ID_F_PRESC,       ///< Prescaler (fast speed)

	CANBUS_TIME_ID_TOTAL,
	ENUM_DUMMY4WORD(CANBUS_TIME_ID)
} CANBUS_TIME_ID;


/**
    CANBUS Status

    CANBUS status for transmit and receive.
*/
typedef enum {
	CANBUS_STS_OK,                     ///< (Base status) Status OK
	CANBUS_STS_BUS_NOT_AVAILABLE,      ///< (Base status) Bus is not avaliable
	CANBUS_STS_INVALID_PARAM,          ///< (Base status) Invalid parameter

	CANBUS_STS_STOP = 0x80000000,      ///< (Extra status) Detect STOP at slave mode
	///< You might get (I2C_STS_NACK | I2C_STS_STOP) or (I2C_STS_NACK | I2C_STS_STOP)
	ENUM_DUMMY4WORD(CANBUS_STS)
} CANBUS_STS, *PCANBUS_STS;


/**
    CANBUS transmit/receive byte counter

    CANBUS transmit/receive byte counter.
*/
typedef enum {
	CANBUS_BYTE_CNT_1 = 1,
	CANBUS_BYTE_CNT_2,
	CANBUS_BYTE_CNT_3,
	CANBUS_BYTE_CNT_4,
	CANBUS_BYTE_CNT_5,
	CANBUS_BYTE_CNT_6,
	CANBUS_BYTE_CNT_7,
	CANBUS_BYTE_CNT_8,

	ENUM_DUMMY4WORD(CANBUS_BYTE_CNT)
} CANBUS_BYTE_CNT, *PCANBUS_BYTE_CNT;


/**
    CANBUS transmit / receive parameter

    CANBUS transmit / receive parameter.
*/
typedef struct {
	DRV_VER_INFO       VersionInfo;   ///< Driver version
	UINT32             SID;
	UINT32             EID;
	CANBUS_BYTE_CNT    DLC;           ///< How many bytes will be transmitted / received
	UINT32             RTR;
	UINT32             IDE;
	UINT32             TSMODE;
} CANBUS_CFG, *PCANBUS_CFG;

//
// CAN public APIs
//
extern ER canbus_open(void);
extern ER canbus_close(void);
extern BOOL canbus_isOpened(void);
extern ER canbus_reset(void);
extern ER canbus_resetrelease(void);
extern ER canbus_setConfig(CANBUS_CFG_ID CfgID, UINT32 uiCfgValue);
extern UINT32 canbus_getConfig(CANBUS_CFG_ID CfgID);
extern ER canbus_setTiming(CANBUS_TIME_ID CfgID, UINT32 uiCfgValue);
extern void CANreadCFG(UINT32 addr);
extern CANBUS_STS canbus_tpe(PCANBUS_CFG pCFG, UINT8 *pData);
extern CANBUS_STS canbus_tsone(PCANBUS_CFG pCFG, UINT8 *pData);
extern CANBUS_STS canbus_receive(PCANBUS_CFG pCFG, UINT8 *pData);
extern CANBUS_STS canbus_rx(UINT8 *pData);
extern CANBUS_STS canbus_getSTBData(UINT8 *pTBUF0_D1, UINT8 *pTBUF0_D2, UINT8 *pTBUF0_D3, UINT8 *pTBUF0_D4,
									UINT8 *pTBUF1_D5, UINT8 *pTBUF1_D6, UINT8 *pTBUF1_D7, UINT8 *pTBUF1_D8);
extern CANBUS_STS canbus_setSTB(PCANBUS_CFG pCFG, UINT8 *pData);

#endif
