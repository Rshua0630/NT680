/**
    Flash charge control Module.

    This module handles the charge control of flash. It supports APIs that can start, stop ,suspend, resume charge ...
    It also supports APIs that can check the flash charge status.

    @file       GxFlash.h
    @ingroup    mILibFlashCharge

    Copyright   Novatek Microelectronics Corp. 2011.  All rights reserved.
*/
#ifndef _GXFLASH_H
#define _GXFLASH_H

#include "Type.h"
#include "GxCommon.h"

/**
    @addtogroup mILibFlashCharge
*/
//@{

/**
     Gx Flash control ID
*/
typedef enum _FLASH_CONTROL {
	FLASH_CHARGE_TIMEOUT_SEC = 0,     ///< flash charge timeout second
	FLASH_CHARGE_CURRENT,             ///< flash charge current low or high
	FLASH_RECHARGE_SEC,               ///< flash re-charge time second
	FLASH_SUSPEND_CALLER_CNT,         ///< flash charge suspend caller count (RO)
	FLASH_CONTROL_MAX,
	ENUM_DUMMY4WORD(FLASH_CONTROL)
} FLASH_CONTROL;

/**
     Install GxFlash semaphore id.
*/
extern void GxFlash_InstallID(void) _SECTION(".kercfg_text");


/**
   Detect flash voltage.

   Detect flash voltage and determine to continue charge flash or stop charging.
   If the charge time is over the charge timeout value(default is 20s), the charge will also be stopped even the flash voltage is still not ready.

*/
extern void    GxFlash_DetFlash(void);

/**
    Start flash charge.

    This API will start charge if the charge is not in the charge suspend status.
*/
extern void    GxFlash_StartCharge(void);

/**
    Stop flash charge.

    @note This API will stop flash charge & reset the charge suspend counter.
*/
extern void    GxFlash_StopCharge(void);        //  stop charge

/**
    Suspend flash charging.

    This API will suspend the flash charge, and add the suspend counter 1.\n
    If user calls this API twice, then the suspend counter is 2, user need to call GxFlash_ResumeCharge() twice to resume charge.\n
    Suspend/Resume flash charge is because of HW limitation, user need to avoid the inrush current to get better power consumption.\n
    So when other device such as lens is active, user need to suspend the flash charge to avoid inrush current.

*/
extern void    GxFlash_SuspendCharge(void);

/**
    Resume flash charging.

    This API will resume the flash charge if the flash is in suspend status & suspend counter is <=1.\n
    If the suspend counter is > 1, this API will just minus the suspend counter by 1.
*/
extern void    GxFlash_ResumeCharge(void);

/**
    Check if the flash charging is in suspend state now.

    @return  TRUE:  Flash charge is suspended.
             FALSE: Flash charge is't suspended.
*/
extern BOOL    GxFlash_IsChargeSuspended(void);

/**
    Check if the flash is charging now.

    @return  TRUE:  Flash is charging.
             FALSE: Flash is not charging.
*/
extern BOOL    GxFlash_IsCharging(void);
/**
    Check if the flash is ready(charge full).

    @return  TRUE:  Flash is ready.
             FALSE: Flash is not ready.
*/
extern BOOL    GxFlash_IsFlashReady(void);

/**
    Set GxFlash setting value.

    @param[in] flashControl:  setting ID
    @param[in] value:  the setting value

*/
extern void    GxFlash_SetData(FLASH_CONTROL flashControl, UINT32 value);

/**
    Get GxFlash setting value.

    @param[in] flashControl:  setting ID
    @return the setting value.

*/
extern UINT32  GxFlash_GetData(FLASH_CONTROL flashControl);

/**
    Trigger the flash with specific time.

    @param[in] Us: The flash trigger time.(Unit is micro second.)
*/
extern void    GxFlash_TriggerFlash(UINT32 Us);

/**
   Detect if need to re-charge flash.

   After we charge full the flash, the flash voltage may become lower as time passed.\n
   This API will detect if the flash voltage is too low then will re-charge the flash.\n
   This default cycle time to detect if need to re-charge flash is 50s.

*/
extern void    GxFlash_DetReCharge(void);

//@}
#endif //_GXFLASH_H
