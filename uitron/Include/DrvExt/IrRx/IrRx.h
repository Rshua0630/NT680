/**
    Copyright   Novatek Microelectronics Corp. 2007. All rights reserved.

    @file       IrRx.h
    @ingroup    mISYSIrRx

    @brief      Header file of IR remote control receiver module.
                This file is the header file of IR remote control receiver module.

    @note       Nothing.

    @version    V1.00.000
    @author     JustinLee
    @date       2007/01/22
*/

/** \addtogroup mISYSIrRx */
//@{

#ifndef _IR_RX_H_
#define _IR_RX_H_

#include "Type.h"




// IR receiver data queue size
#define IRRX_DATA_BUF_NUM               10           // Circular queue, so max buffer available is (IRRX_DATA_BUF_NUM-1) !

// IR receiver Repeat Code identifier
#define IRRX_REPEAT_CODE_IDT            0xFFFFFFFF  // Self-defined Repeat Code identifier used in IR receiver data queue

// IR receiver error codes
#define IRRX_ERR_OK                     0
#define IRRX_ERR_UNSYNC                 1
#define IRRX_ERR_INVALID_BIT            2
#define IRRX_ERR_INVALID_BURST          3
#define IRRX_ERR_TOO_MANY_BIT           4




// IR receiver data queue structure
typedef struct _IRRX_DATA_QUEUE {
	UINT32                  uiRxData;   // Received data (full content)
	UINT32                  uiRxTime;   // Data received time ticks after IR receiver timer started
	struct _IRRX_DATA_QUEUE *pNext;     // Next queue element
} IRRX_DATA_QUEUE, *PIRRX_DATA_QUEUE;

// Data info structure for IR remote control receiver module
typedef struct _IRRX_DATA_INFO {
	UINT32      uiTimeNeg;              // Timer Count value at negative (falling) edge trigger point (protocol view)
	UINT32      uiTimePos;              // Timer Count value at positive (rising) edge trigger point (protocol view)

	UINT32      uiWidthHigh;            // Width of the previous high pulse (protocol view), in us unit
	UINT32      uiWidthLow;             // Width of the previous low pulse (protocol view), in us unit

	UINT32      uiRxDataTime;           // Timer Count value of the last received data
	UINT32      uiRxDataBits;           // Received data bits

	BOOL        bIsBurst;               // Recognized an AGC burst (protocol view)
	BOOL        bLeaderCode;            // Detected the first Leader Code
	BOOL        bRepeatCode;            // Detected the Repeat Code
	BOOL        bStopBit;               // Detected the Stop Bit

	UINT32      uiBitNo;                // Number of received bits
	UINT32      uiRepeatCnt;            // Number of detected Repeat Code

	UINT32      uiError;                // Error code

} IRRX_DATA_INFO, *PIRRX_DATA_INFO;

// Device obj structure for IR remote control receiver module
typedef struct _IRRX_DEVICE_OBJ {
	UINT32      uiGPIOPin;              // GPIO pin number for IR remote control receiver
	UINT32      uiGPIOPad;              // PAD pin number for IR remote control receiver
	BOOL        bEmbedded;              // Embedded (internal) IR receiver or external IR receiver
} IRRX_DEVICE_OBJ, *PIRRX_DEVICE_OBJ;




// APIs
extern ER irrx_open(PIRRX_DEVICE_OBJ pIrRxDevObj);
extern ER irrx_close(void);
extern ID irrx_getLockStatus(void);

extern void irrx_initDataQueue(void);
extern BOOL irrx_isDataQueueFull(void);
extern BOOL irrx_enqueueData(PIRRX_DATA_QUEUE pPutDataQ);
extern BOOL irrx_dequeueData(PIRRX_DATA_QUEUE pGetDataQ);

extern INT  irrx_mapGPIOInterrupt(UINT32 uiGPIOPin);
extern void irrx_checkDataExp(void);

extern UINT32 irrx_getCurrTimerCnt(void);
extern UINT32 irrx_getDataContent(void);

extern BOOL irrx_isDataReady(void);
extern BOOL irrx_isDataRepeat(void);





#endif

//@}
