/**
    Software timer module.

    Software timer module. Provide inaccurate timer for low priority
    or tolerance allowed purpose.

    @file       SwTimer.h
    @ingroup    mIUtilSwTimer
    @note       Nothing.

    Copyright   Novatek Microelectronics Corp. 2012.  All rights reserved.
*/

#ifndef _SWTIMER_H
#define _SWTIMER_H

#include "Type.h"

/**
    @addtogroup mIUtilSwTimer
*/
//@{

/**
    Software timer ID.

    Software timer ID.
*/
typedef enum {
	SWTIMER_00,                         ///< Software timer ID 00
	SWTIMER_01,                         ///< Software timer ID 01
	SWTIMER_02,                         ///< Software timer ID 02
	SWTIMER_03,                         ///< Software timer ID 03
	SWTIMER_04,                         ///< Software timer ID 04
	SWTIMER_05,                         ///< Software timer ID 05
	SWTIMER_06,                         ///< Software timer ID 06
	SWTIMER_07,                         ///< Software timer ID 07
	SWTIMER_08,                         ///< Software timer ID 08
	SWTIMER_09,                         ///< Software timer ID 09
	SWTIMER_10,                         ///< Software timer ID 10
	SWTIMER_11,                         ///< Software timer ID 11
	SWTIMER_12,                         ///< Software timer ID 12
	SWTIMER_13,                         ///< Software timer ID 13
	SWTIMER_14,                         ///< Software timer ID 14
	SWTIMER_15,                         ///< Software timer ID 15
	SWTIMER_16,                         ///< Software timer ID 16
	SWTIMER_17,                         ///< Software timer ID 17
	SWTIMER_18,                         ///< Software timer ID 18
	SWTIMER_19,                         ///< Software timer ID 19
	SWTIMER_20,                         ///< Software timer ID 20
	SWTIMER_21,                         ///< Software timer ID 21
	SWTIMER_22,                         ///< Software timer ID 22
	SWTIMER_23,                         ///< Software timer ID 23
	SWTIMER_24,                         ///< Software timer ID 24
	SWTIMER_25,                         ///< Software timer ID 25
	SWTIMER_26,                         ///< Software timer ID 26
	SWTIMER_27,                         ///< Software timer ID 27
	SWTIMER_28,                         ///< Software timer ID 28
	SWTIMER_29,                         ///< Software timer ID 29
	SWTIMER_30,                         ///< Software timer ID 30
	SWTIMER_31,                         ///< Software timer ID 31
	SWTIMER_32,                         ///< Software timer ID 32
	SWTIMER_33,                         ///< Software timer ID 33
	SWTIMER_34,                         ///< Software timer ID 34
	SWTIMER_35,                         ///< Software timer ID 35
	SWTIMER_36,                         ///< Software timer ID 36
	SWTIMER_37,                         ///< Software timer ID 37
	SWTIMER_38,                         ///< Software timer ID 38
	SWTIMER_39,                         ///< Software timer ID 39
	SWTIMER_40,                         ///< Software timer ID 40
	SWTIMER_41,                         ///< Software timer ID 41
	SWTIMER_42,                         ///< Software timer ID 42
	SWTIMER_43,                         ///< Software timer ID 43
	SWTIMER_44,                         ///< Software timer ID 44
	SWTIMER_45,                         ///< Software timer ID 45
	SWTIMER_46,                         ///< Software timer ID 46
	SWTIMER_47,                         ///< Software timer ID 47
	SWTIMER_48,                         ///< Software timer ID 48
	SWTIMER_49,                         ///< Software timer ID 49
	SWTIMER_50,                         ///< Software timer ID 50
	SWTIMER_51,                         ///< Software timer ID 51
	SWTIMER_52,                         ///< Software timer ID 52
	SWTIMER_53,                         ///< Software timer ID 53
	SWTIMER_54,                         ///< Software timer ID 54
	SWTIMER_55,                         ///< Software timer ID 55
	SWTIMER_56,                         ///< Software timer ID 56
	SWTIMER_57,                         ///< Software timer ID 57
	SWTIMER_58,                         ///< Software timer ID 58
	SWTIMER_59,                         ///< Software timer ID 59
	SWTIMER_60,                         ///< Software timer ID 60
	SWTIMER_61,                         ///< Software timer ID 61
	SWTIMER_62,                         ///< Software timer ID 62
	SWTIMER_63,                         ///< Software timer ID 63
	SWTIMER_NUM,                        ///< Total software timer number

	ENUM_DUMMY4WORD(SWTIMER_ID)
} SWTIMER_ID, *PSWTIMER_ID;

#define SWTIMER_INVALID         0x80000000  ///< Invalid Timer ID

#define SWTIMER_MIN_INTERVAL    1           ///< Minimum interval
#define SWTIMER_MAX_INTERVAL    2147483     ///< Maximum interval
#define SWTIMER_MIN_RESOLUTION  1           ///< Minimum resolution
#define SWTIMER_MAX_RESOLUTION  20          ///< Maximum resolution

/**
    Software timer mode

    Software timer mode.
*/
typedef enum {
	SWTIMER_MODE_ONE_SHOT   = 0x00000000,   ///< Mode is "one-shot", you have to start timer again when expired.
	SWTIMER_MODE_FREE_RUN   = 0x00000001,   ///< Mode is "free-run", auto re-start timer when expired.

	ENUM_DUMMY4WORD(SWTIMER_MODE)
} SWTIMER_MODE, *PSWTIMER_MODE;

/**
    Software timer configuration ID

    Software timer configuration ID for SwTimer_SetConfig() and SwTimer_GetConfig()
*/
typedef enum {
	SWTIMER_CONFIG_ID_RESOLUTION,   ///< Software timer resolution, unit: ms (millisecond)
	///< @note  Valid value: SWTIMER_MIN_RESOLUTION (1) ~ SWTIMER_MAX_RESOLUTION (20), default: 10.
	///< Lower value higher system burden (CPU loading, interrupt, context switch...).
	///< Higher value less accuracy.

	ENUM_DUMMY4WORD(SWTIMER_CONFIG_ID)
} SWTIMER_CONFIG_ID;

/**
    Software timer callback function

    @param[in] uiEvent  specific timer timeout ID

*/
typedef void (*SWTIMER_CB)(UINT32 uiEvent);

// Software timer API
extern void     SwTimer_InstallID(void) _SECTION(".kercfg_text");

extern void     SwTimer_Init(void);
extern ER       SwTimer_Open(PSWTIMER_ID pSwTimerID, SWTIMER_CB EventHandler);
extern ER       SwTimer_Close(SWTIMER_ID SwTimerID);
extern ER       SwTimer_Cfg(SWTIMER_ID SwTimerID, INT32 iInterval, SWTIMER_MODE SwTimerMode);
extern ER       SwTimer_Start(SWTIMER_ID SwTimerID);
extern ER       SwTimer_Stop(SWTIMER_ID SwTimerID);
extern ER       SwTimer_WaitTimeup(SWTIMER_ID SwTimerID);

extern void     SwTimer_SetConfig(SWTIMER_CONFIG_ID ConfigID, UINT32 uiConfig);
extern UINT32   SwTimer_GetConfig(SWTIMER_CONFIG_ID ConfigID);

extern void     SwTimer_DelayMs(UINT32 uiMS);

//@}

#endif
