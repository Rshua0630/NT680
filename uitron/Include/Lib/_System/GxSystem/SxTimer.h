/**
    System software timer service.

    A service with a time loop can plug-in many callbacks for a time trigger.

    @file       SxTimer.h
    @ingroup    mGXSYSTEM

    Copyright   Novatek Microelectronics Corp. 2013.  All rights reserved.
*/

#ifndef _SXTIMER_H
#define _SXTIMER_H

#include "GxCommon.h"
#include "SxCmd.h"

////////////////////////////////////////////////////////////////////////////////

typedef void (*SX_TIMER)(void); ///< timer callback prototype

/**
     Timer entry

     A entry for SxTimer create a time service.
     @note user uses SX_TIMER_ITEM insead of setting structure member himself.
*/
typedef struct {
	INT32   id;             ///< [OUTPUT] ID value
	CHAR    *pName;         ///< [INPUT], for debug
	void    *pNext;         ///< Reversed, internal used.
	SX_TIMER pfFunc;        ///< [INPUT], function pointer
	UINT32  uiPeriodCnt;    ///< [INPUT], to evaluate duty cycle time that = uiPeriodCnt*20ms. The duty cycle time is to service the fpDetFunction.
	BOOL    bEnable;        ///< [INPUT], TRUE for active, FALSE for inactive
} SX_TIMER_ENTRY;

/**
     Macro for Creating entry

     create a entry for SxTimer.

     @param[in] name    timer name
     @param[in] func    callback function
     @param[in] period  to evaluate duty cycle time that = period*20ms. The duty cycle time is to service the func.
     @param[in] active  indicate to active time trigger when SxTimer_AddItem is called.
*/
#define SX_TIMER_ITEM(name,func,period,active)  _SECTION(".kercfg_data") SX_TIMER_ENTRY Timer_##name = {0,#name,0,(func),(period),(active)};

#define SX_TIMER_GET_ENTRY_NAME(name) Timer_##name ///< translate item name to real declaration entry name for SxTimer_AddItem.
/**
     Get timer status ID.

     Get timere status or information.
*/
typedef enum {
	SXTIMER_TIMER_BASE,      ///< get time base in (ms).
	SXTIMER_CURR_CNT,        ///< get current time count
	SXTIMER_MAX              ///< maximum set id
} SXTIMER_DATA_SET;

/**
     Open SxTimer

     Start to SxTimer internal timer task.

     @return
        - @b E_OK: success.
        - @b Others: failed to run.

     @note This API is already called at NvtSystem.a
*/
extern ER SxTimer_Open(void);

/**
     Close SxTimer

     To terminate SxTimer internal timer task.

     @return
        - @b E_OK: success.
        - @b Others: failed to run.

     @note This API is already called at NvtSystem.a
*/
extern ER SxTimer_Close(void);

/**
     Suspend SxTimer

     SxTimer internal timer task goes into the supspend state.

     @note All of entrys will be suspend.
*/
extern void SxTimer_Suspend(void);

/**
     Resume SxTimer

     To resume SxTimer internal timer suspended task.

     @note All of entrys will be suspend.
*/
extern void SxTimer_Resume(void);

/**
     Add SxTimer entry

     After user create a entry by SX_TIMER_ITEM, use the API to register into SxTimer.

     @param[in] pEntry  SxTimer's entry that must place at global
     @return
        - @b >=0, assigned entry id
        - @b <0, failed to add item
*/
extern INT32 SxTimer_AddItem(SX_TIMER_ENTRY *pEntry);

/**
     Set active

     Set detecting function active.
     If a function is set inactive, the function will not be serviced in SxTimer.

     @param[in] uiFuncID function ID returned value by SxTimer_AddItem()
     @param[in] bActive TRUE for active, FALSE for inactive

*/
extern void SxTimer_SetFuncActive(UINT32 uiFuncID, BOOL bActive);

/**
     Get active-value

     Get detected function active-value.

     @param[in] FuncID function ID registered by SxTimer_Open
     @return TRUE for active, FALSE for inactive

*/
extern BOOL SxTimer_GetFuncActive(UINT32 FuncID);


/**
     Get SxTimer internal data

     Get SxTimer internal data by using SXTIMER_DATA_SET. Currently, user can
     get time base and current count.

     @param[in] attribute item selection
     @return SxTimer internal data

*/
extern UINT32 SxTimer_GetData(SXTIMER_DATA_SET attribute);


/**
     SxCmd Entry

     Return a SxCmd entry for install SxCmd to type uart command.
*/
extern SX_CMD_ENTRY *SxTimer_GetCmdEntry(void);

#endif //_SXTIMER_H

