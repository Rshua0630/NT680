/**
    System power control module.

    This module handles the power control, including battery level detection, auto power off, auto sleep.
    It also control the battery charge flow.

    @file       GxPower.h
    @ingroup    mILibPowerCtrl

    Copyright   Novatek Microelectronics Corp. 2011.  All rights reserved.
*/
#ifndef _GXPOWER_H
#define _GXPOWER_H

#include "GxCommon.h"

/**
    @addtogroup mILibPowerCtrl
*/
//@{

////////////////////////////////////////////////////////////////////////////////
/**
     Gx Power control ID
*/
typedef enum _GXPWR_CTRL {
	GXPWR_CTRL_AUTOPOWEROFF_TIME = 0,              ///<  auto power off system time value set, get
	GXPWR_CTRL_AUTOPOWEROFF_EN,                    ///<  system auto power off enable, disable
	GXPWR_CTRL_BATTERY_DETECT_EN,                  ///<  battery detect function enable, disable
	GXPWR_CTRL_SLEEP_EN,                           ///<  sleep function enbale, disable
	GXPWR_CTRL_SLEEP_RESET,                        ///<  reset sleep counter and control
	GXPWR_CTRL_AUTOPOWEROFF_RESET,                 ///<  reset system auto power off counter
	GXPWR_CTRL_SLEEP_LEVEL,                        ///<  system sleep level 0~3 (RO)
	GXPWR_CTRL_IS_DUMMUYLOAD_POWEROFF,             ///<  if dummy load check battery fail need to power off (RO)
	GXPWR_CTRL_BATTERY_LEVEL,                      ///<  current battery level get (RO)
	GXPWR_CTRL_SLEEP_TIME_L,                       ///<  low level sleep mode time value set, get (RW)
	GXPWR_CTRL_SLEEP_TIME,                         ///<  normal level sleep mode time value set, get  (RW)
	GXPWR_CTRL_SLEEP_TIME_D,                       ///<  deep level sleep mode time value set, get (RW)
	GXPWR_CTRL_IS_BATT_INSERT,                     ///<  battery insert status (RO)
	GXPWR_CTRL_IS_DEAD_BATT,                       ///<  if battery power status is dead battery (RO)
	GXPWR_CTRL_BATTERY_CHARGE_EN,                  ///<  battery charge enable, disable
	GXPWR_CTRL_BATTERY_CHARGE_CURRENT,             ///<  battery charge current set ,get(RW)
	GXPWR_CTRL_BATTERY_CHARGE_OK,                  ///<  battery charge ok status get (RO)
	GXPWR_CTRL_IGNORE_BATT_CHECK,                  ///<  power on force ignore battery check
	ENUM_DUMMY4WORD(GXPWR_CTRL)
} GXPWR_CTRL;

/**
     @name Battery charge current value
*/
//@{
#define BATT_CHARGE_CURRENT_LOW         0          ///<  charge current low
#define BATT_CHARGE_CURRENT_MEDIUM      1          ///<  charge current medimum
#define BATT_CHARGE_CURRENT_HIGH        2          ///<  charge current high
//@}


/**
     @name Power Callback event
*/
//@{
#define POWER_CB_BATT_EMPTY             0          ///<  battery is emtpy
#define POWER_CB_BATT_CHG               1          ///<  battery level changed
#define POWER_CB_SLEEP_ENTER_L          2          ///<  enter low level sleep mode
#define POWER_CB_SLEEP_ENTER            3          ///<  enter normal level sleep mode
#define POWER_CB_SLEEP_ENTER_D          4          ///<  enter deep level sleep mode
#define POWER_CB_SLEEP_RESET            5          ///<  reset auto sleep count down timer
#define POWER_CB_POWEROFF               6          ///<  auto power off time is up
#define POWER_CB_BATT_OVERHEAT          7          ///<  battery temperature is overheat
#define POWER_CB_CHARGE_OK              8          ///<  battery charge is ok (charge full)
#define POWER_CB_CHARGE_SUSPEND         9          ///<  temperature is too high suspend charge
#define POWER_CB_CHARGE_RESUME          10         ///<  resume charge after temperature becomes normal
//@}


/**
    Detect battery voltage level.

    Detect battery voltage level and store it in uiBatteryLvl (static variable)
    If the battery voltage is VOLDET_BATTERY_LVL_EMPTY, then callback event POWER_CB_BATT_EMPTY to power off the system.
*/
extern void     GxPower_DetBattery(void);

/**
    Detect auto power off

    Detect auto power off.
    If the auto power off counter reach auto power off setting, then callback event POWER_CB_POWEROFF to do power off.
*/
extern void     GxPower_DetAutoPoweroff(void);

/**
    Get GxPower setting value.

    @param[in] PowerCtrl:  setting ID
    @return the setting value.

*/
extern UINT32   GxPower_GetControl(GXPWR_CTRL PowerCtrl);

/**
    Set GxPower setting value.

    @param[in] PowerCtrl:  setting ID
    @param[in] value:  the setting value

*/
extern void     GxPower_SetControl(GXPWR_CTRL PowerCtrl, UINT32 value);

/**
    Do the dummy loading.

    This API is just mapping to DrvPower_DummyLoad(). User need to implement the function DrvPower_DummyLoad() to do the dummy load.\n

    @note User must call this API before calling GxPower_PowerON() when system power on.

    @return  TRUE:  battery status is ok can power on.
             FALSE: battery status is not ok can't power on.
*/
extern UINT32   GxPower_DummyLoad(void);

/**
     GxPower power on init.

     @note User must register callback funcion then can receive the Driver Power Callback event DRVPWR_CB_XXX.

     @param[in] fpDxPowerCB: the callback function pointer.

*/
extern void      GxPower_PowerON(GX_CALLBACK_PTR DxPower_CB);

/**
     Register callback function.

     @note User must register callback funcion then can receive the Power Callback event POWER_CB_XXX.

     @param[in] fpPowerCB: the callback function pointer.
*/
extern void     GxPower_RegCB(GX_CALLBACK_PTR fpPowerCB);

/**
     Detect if enter sleep mode.

     If the sleep mode counter reachs setting, then callback event to enter sleep mode.\n
     The sleep mode have 3 levels, each level has separate time setting and separate callback event.

*/
extern void     GxPower_DetSleep(void);

/**
     Detect if the charge status is ok.

     Detect battery charge status and determine to continue charge or stop charging.\n
     It will also detect the battery temperature to check if the battery is overheat and need to suspend charge.

*/
extern void     GxPower_DetCharge(void);

/**
    Install SxCmd.

    Install uart command to SxCmd.
*/
extern void     GxPower_InstallCmd(void);
//@}
#endif //_GXPOWER_H
