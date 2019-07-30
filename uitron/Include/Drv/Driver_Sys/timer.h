/**
    Public header file for timer module.

    This file is the header file that define the API and data type for timer
    module.

    @file       timer.h
    @ingroup    miDrvTimer_Timer
    @note       Nothing.

    Copyright   Novatek Microelectronics Corp. 2011.  All rights reserved.
*/

#ifndef _TIMER_H
#define _TIMER_H

#include "Type.h"
#include "Driver.h"

/**
    @addtogroup miDrvTimer_Timer
*/
//@{

/**
    Timer ID.

    Timer ID.
*/
typedef enum {
	TIMER_0,                        ///< Timer ID for Timer 0
	TIMER_1,                        ///< Timer ID for Timer 1
	TIMER_2,                        ///< Timer ID for Timer 2
	TIMER_3,                        ///< Timer ID for Timer 3
	TIMER_4,                        ///< Timer ID for Timer 4
	TIMER_5,                        ///< Timer ID for Timer 5
	TIMER_6,                        ///< Timer ID for Timer 6
	TIMER_7,                        ///< Timer ID for Timer 7
	TIMER_8,                        ///< Timer ID for Timer 8
	TIMER_9,                        ///< Timer ID for Timer 9
	TIMER_10,                       ///< Timer ID for Timer 10
	TIMER_11,                       ///< Timer ID for Timer 11
	TIMER_12,                       ///< Timer ID for Timer 12
	TIMER_13,                       ///< Timer ID for Timer 13
	TIMER_14,                       ///< Timer ID for Timer 14
	TIMER_15,                       ///< Timer ID for Timer 15
	TIMER_16,                       ///< Timer ID for Timer 16
	TIMER_17,                       ///< Timer ID for Timer 17
	TIMER_18,                       ///< Timer ID for Timer 18
	TIMER_19,                       ///< Timer ID for Timer 19
	TIMER_NUM,                      ///< Timer number

	ENUM_DUMMY4WORD(TIMER_ID)
} TIMER_ID, *PTIMER_ID;

#define TIMER_INVALID   0xFFFFFFFF  ///< Invalid Timer ID

/**
    Timer mode.

    Timer mode.
*/
typedef enum {
	TIMER_MODE_CLKSRC_DIV0  = 0x00000000,   ///< Timer clock source is from divider 0, the clock is fixed at 1 MHz.
	TIMER_MODE_CLKSRC_DIV1  = 0x00000001,   ///< Timer clock source is from divider 1, the clock can be configured.
	TIMER_MODE_ONE_SHOT     = 0x00000010,   ///< Timer operation mode is "one shot"
	TIMER_MODE_FREE_RUN     = 0x00000020,   ///< Timer operation mode is "free run"
	TIMER_MODE_ENABLE_INT   = 0x00000040,   ///< Enable timeout interrupt

	ENUM_DUMMY4WORD(TIMER_MODE)
} TIMER_MODE, *PTIMER_MODE;

/**
    Timer state.

    Timer state.
*/
typedef enum {
	TIMER_STATE_PAUSE,              ///< Stop timer
	TIMER_STATE_PLAY,               ///< Start timer

	ENUM_DUMMY4WORD(TIMER_STATE)
} TIMER_STATE, *PTIMER_STATE;

/**
    Timer Configuration ID

    Timer Configuration ID for timer_setConfig() and timer_getConfig()

*/
typedef enum {
	TIMER_CONFIG_ID_DIV1_CLK = 1,   ///< Divider1 clock. Default is 1 MHz (1 clock is 1us)
	///< @note  Valid value: 11,718 ~ 3,000,000 , unit: Hz.
	///<        Real clock might lower than your setting.
	///<        Please call timer_getConfig(TIMER_CONFIG_ID_DIV1_CLK) to confirm.
	///<        Formula: Clock = 3 MHz / (Divider + 1)

	ENUM_DUMMY4WORD(TIMER_CONFIG_ID)
} TIMER_CONFIG_ID;

// Timer Driver API
extern ER       timer_open(PTIMER_ID pTimerID, DRV_CB EventHandler);
extern ER       timer_close(TIMER_ID TimerID);
extern ER       timer_openAutoClose(PTIMER_ID pTimerID, DRV_CB EventHandler);
extern ER       timer_cfg(TIMER_ID TimerID, UINT32 uiIntval, TIMER_MODE TimerMode, TIMER_STATE TimerState);
extern ER       timer_pausePlay(TIMER_ID TimerID, TIMER_STATE TimerState);
extern ER       timer_waitTimeup(TIMER_ID TimerID);
extern ER       timer_checkTimeup(TIMER_ID TimerID, BOOL *pbEn);
extern ER       timer_reload(TIMER_ID TimerID, UINT32 interval);

// Timer Utility API
extern void     timer_setConfig(TIMER_CONFIG_ID ConfigID, UINT32 uiConfig);
extern UINT32   timer_getConfig(TIMER_CONFIG_ID ConfigID);
extern TIMER_ID timer_getSysTimerID(void);
extern UINT32   timer_getCurrentCount(TIMER_ID TimerID);
extern UINT32   timer_getTargetCount(TIMER_ID TimerID);
extern UINT32   timer_getCurrHWTimerLockNums(void);

//@}

#endif
