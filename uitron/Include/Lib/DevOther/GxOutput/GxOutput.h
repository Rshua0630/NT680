/**
    GxOutput module

    LED toogle mechanism. User should implement the API listed in DxOutput.h.

    @file       GxOutput.h
    @ingroup    mIGxOutput

    Copyright   Novatek Microelectronics Corp. 2012.  All rights reserved.
*/
#ifndef _GXOUTPUT_H
#define _GXOUTPUT_H

#include "GxCommon.h"


/**
    @addtogroup mIGxOutput
*/
//@{

#define GXLED_1SEC_LED_TOGGLE_CNT (50)   ///< if toggle timer interval set 20ms for 1 Count ,50*20ms = 1000ms

/**
    @name LED identification
*/
//@{
#define KEYSCAN_LED_GREEN               0                ///<Green LED
#define KEYSCAN_LED_RED                 1                ///<Red LED
#define KEYSCAN_LED_FCS                 2                ///<FCS LED
#define KEYSCAN_LED_3                   3                ///<LED 3
#define KEYSCAN_LED_4                   4                ///<LED 4
#define KEYSCAN_LED_5                   5                ///<LED 5
#define KEYSCAN_LED_6                   6                ///<LED 6
#define KEYSCAN_LED_7                   7                ///<LED 7
#define KEYSCAN_LED_8                   8                ///<LED 8
#define KEYSCAN_LED_9                   9                ///<LED 9
#define KEYSCAN_LED_10                  10               ///<LED 10
#define KEYSCAN_LED_11                  11               ///<LED 11
//@}


/**
    @name LED toggle speed
*/
//@{
#define KEYSCAN_LED_TSPEED_SLOW         0               ///<Slow speed,speed is the same as timer frequence
#define KEYSCAN_LED_TSPEED_NORMAL       1               ///<Normal speed,twice as fast than KEYSCAN_LED_TSPEED_SLOW
#define KEYSCAN_LED_TSPEED_FAST         2               ///<Fast speed,Six times faster than KEYSCAN_LED_TSPEED_SLOW
//@}

/**
    LED control attribute
*/
typedef enum {
	TURNON_LED = 0,                 ///<LED on or off control
	SET_TOGGLE_LED,                 ///<Enable/Disable toggle control
	SETLED_SPEED                    ///<LED speed control
} LED_CONTROL;

/**
  Check whether LED is on or off

  Return TRUE if LED is on, otherwise return FALSE.

  @param[in] uiLED Which LED, LED id
  @return LED status
*/
extern BOOL     GxLED_IsLEDOn(UINT32 uiLED);

/**
  Toggle LED,this API is for timer callback.
  It should be regular be executed.The timer interval would
  effect LED toggle speed.

  @return void
*/
extern void GxLED_ToggleLED(void);

/**
  Set LED control.

  @param[in] uiLED Which LED, LED id
  @param[in] state Which state, could be the LED_CONTROL
  @param[in] value Which value
  @return void
*/
extern BOOL GxLED_SetCtrl(UINT32 uiLED, UINT32 state, UINT32 value);

/**
     Open device.

     Open device.
     @note Nothing

     @param[in] DevID           Device id (ex: DOUT1 or DOUT2).
     @param[in] NewDevObj       device object.
     @param[in] mode            device mode.
*/
extern void GxOutput_OpenDevice(UINT32 DevID, UINT32 NewDevObj, UINT32 mode);

/**
     Close device.

     Close device.
     @note Nothing

     @param[in] DevID           Device id (ex: DOUT1 or DOUT2).
*/
extern void GxOutput_CloseDevice(UINT32 DevID);

/**
     Set control to device.

     Set control to device.
     @note Nothing

     @param[in] DevID           Device id (ex: DOUT1 or DOUT2).
     @param[in] data            Control state
     @param[in] value           Control value
*/
extern void GxOutput_SetDeviceCtrl(UINT32 DevID, UINT32 data, UINT32 value);

/**
     Get control to device.

     Get control to device.
     @note Nothing

     @param[in] DevID           Device id (ex: DOUT1 or DOUT2).
     @param[in] data            Control state
     @return                    Control value
*/
extern UINT32 GxOutput_GetDeviceCtrl(UINT32 DevID, UINT32 data);

extern void GxOutput_OnTimer(UINT32 DevID);

//@}
#endif //_GXOUTPUT_H
