/**
    System Power/Reset Control

    Control system power and reset

    @file       GxSystem.h
    @ingroup    mGXSYSTEM

    Copyright   Novatek Microelectronics Corp. 2011.  All rights reserved.
*/

#ifndef _GXSYSTEM_H
#define _GXSYSTEM_H

#include "SysKer.h" //for OSINIT_PARAM
#include "GxCommon.h"
#include "SxCmd.h"
#include "SxTimer.h"
#include "DxSys.h"

/**
    @addtogroup mGXSYSTEM
*/
//@{

/**
     Power on source identifier

     Use GxSystem_GetPowerOnSource() to get which source to trigger the power on.
*/
typedef enum _GX_PWRON_SRC {
	GX_PWRON_SRC_PWR_SW,       ///< power on from power switch
	GX_PWRON_SRC_PWR_SW2,      ///< power on from power switch 2
	GX_PWRON_SRC_PWR_VBUS,     ///< power on from power switch 3 (Usually for USB VBUS, depend on project)
	GX_PWRON_SRC_PWR_VBAT,      ///< power on from power switch 4 (Usually for VBAT, depend on project)
	GX_PWRON_SRC_PWR_ALM,      ///< Power on from PWR ALARM
	ENUM_DUMMY4WORD(GX_PWRON_SRC)
} GX_PWRON_SRC;

/**
     Install GxSystem task,flag and semaphore id
*/
extern void GxSystem_InstallID(void) _SECTION(".kercfg_text");


/**
     Get power on source

     Get Power on by which source

     @return refer to @ref GX_PWRON_SRC
*/
extern GX_PWRON_SRC GxSystem_GetPowerOnSource(void);

/**
     Wait for GxSystem_PowerOn finish.

     Wait flag singal until GxSystem_PowerOn is call.
*/
extern void GxSystem_WaitForPowerOn(void);

/**
     Wait for GxSystem_PowerOff finish.

     Wait flag singal until GxSystem_PowerOff is call.
*/
extern void GxSystem_WaitForPowerOff(void);

/**
     Power-on process

     Do power-on process to pass GxSystem_WaitForPowerOn()
*/
extern void GxSystem_PowerOn(void);

/**
     Power-off process

     Do power-off process to pass GxSystem_WaitForPowerOff(), then do shutdown, or HW reset, or SW reset.
*/
extern void GxSystem_PowerOff(void);

/**
     Enable HW Reset by AlarmTime

     After power-off, wake up system after a time period.

     @param[in] param unused currently.
     @param[in] hour hours
     @param[in] min minutes
     @param[in] sec seconds
*/
extern void GxSystem_EnableHWResetByAlarmTime(UINT32 param, INT32 hour, INT32 min, INT32 sec);

/**
     Enable HW Reset ASAP

     After power-off, wake up system ASAP.

     @param[in] param unused currently.
*/
extern void GxSystem_EnableHWReset(UINT32 param);

/**
     Cancel the HW Reset

     Cancel the time to wake up system.
*/
extern void GxSystem_CancelHWReset(void);

/**
     Enable SW Reset

     Do a software reset(watch dog reset) after power-off done.

     @param[in] param a tag is saved into DRAM before reset
*/
extern void GxSystem_EnableSWReset(UINT32 param);

/**
     Cancel the SW Reset

     Disable the software reset.
*/
extern void GxSystem_CancelSWReset(void);

/**
     Get the SW Reset param

     Get the tag that is saved into DRAM before reset
*/
extern UINT32 GxSystem_GetSWResetParam(void);

/**
     Shutdown immediately

     Skip normal power-off flow, shutdown directly. (NOTE: non-safe)
*/
extern void GxSystem_ShutdownNOW(void);

/**
     SW Reset immediately

     Skip normal power-off flow, software reset directly. (NOTE: non-safe)
*/
extern void GxSystem_SWResetNOW(void);

/**
     HW Reset immediately

     Skip normal power-off flow, hardware reset directly. (NOTE: non-safe)
*/
extern void GxSystem_HWResetNOW(void);


/**
     Register CB running on system idle

     Register CB running on system idle.
*/
extern void GxSystem_RegIdleCB(void (*OnIdle)(void));

//@}
#endif //_GXSYSTEM_H

