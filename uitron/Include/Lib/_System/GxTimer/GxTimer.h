/**
    Software timer for UI flow.

    @file       GxTimer.h
    @ingroup    mIGxTimer

    Copyright   Novatek Microelectronics Corp. 2009.  All rights reserved.
*/

/**
    @addtogroup mIGxTimer
*/
//@{

#ifndef _GXTIMER_H
#define _GXTIMER_H

#include "GxCommon.h"

/**
     Indicate the Timer ID is inactive.
*/
#define NULL_TIMER                      0xFFFFFFFF

/**
     The operation type of Timer.
*/
typedef enum _TIMER_TYPE {
	ONE_SHOT = 0x00,  ///< One-shot mode, the timer just run once.
	CONTINUE          ///< Continuing mode, the timer will keep running untill GxTimer_StopTimer() is invoked.
} TIMER_TYPE;

/**
     Install GxTimer semaphore id
*/
extern void GxTimer_InstallID(void) _SECTION(".kercfg_text");

/**
     Register GxTimer callback function.

     The callback function will be invoked when any timer matches its "interval".
     @param[in] fpTimerCB Callback function.
*/
extern void    GxTimer_RegCB(GX_CALLBACK_PTR fpTimerCB);

/**
     Set timer base.

     The default timer base is 20 ms.
     @param[in] TimerBaseMs Timer base, unit is ms.
*/
extern void    GxTimer_SetTimerBase(UINT32 TimerBaseMs);

/**
  Timer count.

  Check if the active timer count matches the interval which is set by GxTimer_StartTimer().
  @note This API should be registered to SxTimer.
*/
extern void    GxTimer_CountTimer(void);

/**
  Start a timer.
  @note It will always allocate a free timer to the TimerNameID even if the previous one is still running.\n
        For that reason, the timer need to stop before starting for the behavior of "reset".
  @param[in] interval Timer trigger interval(ms).
  @param[in] TimerNameID This event will be posted as arguement when timer count matches the interval.
  @param[in] type The operation type of Timer: #_TIMER_TYPE.
  @return The timer ID allocated by GxTimer, the maximnu number of timer is 5.
*/
extern UINT32  GxTimer_StartTimer(UINT32 interval, UINT32 TimerNameID, TIMER_TYPE type);

/**
  Stop a running timer.

  @param[in] timerID The ID from the return value of GxTimer_StartTimer().
*/
extern void    GxTimer_StopTimer(UINT32 *timerID);

/**
  Dump the status of all timers.
*/
extern void GxTimer_Dump(void);

/**
    Install SxCmd.

    Install GxTimer command to SxCmd.
*/
extern void GxTimer_InstallCmd(void);

//@}
#endif
