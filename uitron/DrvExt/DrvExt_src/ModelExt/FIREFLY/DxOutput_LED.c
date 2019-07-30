/**
    Copyright   Novatek Microelectronics Corp. 2005.  All rights reserved.

    @file       DetLED.c
    @ingroup    mIPRJAPKeyIO

    @brief      Control LED on/off
                Control LED on/off/toggle

    @note       Nothing.

    @date       2005/12/15
*/

/** \addtogroup mIPRJAPKeyIO */
//@{

#include "DxCfg.h"
#include "IOCfg.h"

#include "DxOutput.h"
#include "DxCommon.h"
#include "DxApi.h"

///////////////////////////////////////////////////////////////////////////////
#define __MODULE__          DxDrv
#define __DBGLVL__          2 // 0=FATAL, 1=ERR, 2=WRN, 3=UNIT, 4=FUNC, 5=IND, 6=MSG, 7=VALUE, 8=USER
#define __DBGFLT__          "*" //*=All, [mark]=CustomClass
#include "DebugModule.h"
///////////////////////////////////////////////////////////////////////////////

//#NT#set FCS LED brightness level
UINT32   FcsLedLvl = LCSBRT_LVL_03;
UINT32   FcsLedPrdVal[LCSBRT_LVL_SETTING_MAX] = {
	14,
	15,
	16,
	17,
	18,
	19,
	20
};

////////////////////////////////////////////////////////////////////////////////
// GPIO related

/**
  Turn on LED

  Turn on LED.

  @param UINT32 uiLED: Which LED, could be the following
    GPIOMAP_LED_GREEN
    GPIOMAP_LED_RED
    GPIOMAP_LED_FCS //FOCUS
  @return void
*/
void LED_TurnOnLED(UINT32 uiLED)
{

	DBG_IND("LED_TurnOnLED %d\r\n", uiLED);
	switch (uiLED) {
	case GPIOMAP_LED_GREEN:
#if (LED_GREEN_CTRL == LED_GREEN_BY_GPIO)
		gpio_setPin(GPIO_GREEN_LED);
#endif
		break;

	case GPIOMAP_LED_RED:
#if (LED_RED_CTRL == LED_RED_BY_GPIO)
		gpio_setPin(GPIO_RED_LED);
#endif
		break;

	case GPIOMAP_LED_FCS:
#if (LED_FOCUS_CTRL == LED_FOCUS_BY_GPIO)
		gpio_setPin(GPIO_FOCUS_LED);
#endif
#if (LED_FOCUS_CTRL == LED_FOCUS_BY_PWM)
		{
			//set FOCUS LED brightness level
			PWM_CFG PWMInfo;
			if (pwm_open(PWMID_FOCUS_LED) != E_OK) {
				pwm_stop(PWMID_FOCUS_LED);
				pwm_close(PWMID_FOCUS_LED, TRUE);
				pwm_open(PWMID_FOCUS_LED);
			}
			PWMInfo.uiDiv = 220;
			PWMInfo.uiPrd = 22;
			PWMInfo.uiFall = FcsLedPrdVal[FcsLedLvl];
			PWMInfo.uiOnCycle = 0;
			PWMInfo.uiInv = 0;
			PWMInfo.uiRise = 0;
			pwm_set(PWMID_FOCUS_LED, &PWMInfo);
			pwm_reload(PWMID_FOCUS_LED);
		}
#endif

	default:
		break;
	}
}

/**
  Turn off LED

  Turn off LED.

  @param UINT32 uiLED: Which LED, could be the following
    GPIOMAP_LED_GREEN
    GPIOMAP_LED_RED
    GPIOMAP_LED_FCS
  @return void
*/
void LED_TurnOffLED(UINT32 uiLED)
{
	DBG_IND("LED_TurnOffLED %d\r\n", uiLED);
	switch (uiLED) {
	case GPIOMAP_LED_GREEN:
#if (LED_GREEN_CTRL == LED_GREEN_BY_GPIO)
		gpio_clearPin(GPIO_GREEN_LED);
#endif
		break;

	case GPIOMAP_LED_RED:
#if (LED_RED_CTRL == LED_RED_BY_GPIO)
		gpio_clearPin(GPIO_RED_LED);
#endif
		break;

	case GPIOMAP_LED_FCS:
#if (LED_FOCUS_CTRL == LED_FOCUS_BY_GPIO)
		gpio_clearPin(GPIO_FOCUS_LED);
#endif
#if (LED_FOCUS_CTRL == LED_FOCUS_BY_PWM)
		{
			//set FOCUS LED brightness level
			PWM_CFG PWMInfo;
			PWMInfo.uiDiv = 220;
			PWMInfo.uiPrd = 0;
			PWMInfo.uiFall = 0;
			PWMInfo.uiOnCycle = 0;
			PWMInfo.uiInv = 0;
			PWMInfo.uiRise = 0;
			pwm_set(PWMID_FOCUS_LED, &PWMInfo);
			pwm_reload(PWMID_FOCUS_LED);
		}
#endif
	default:
		break;
	}
}

/**
  Check whether LED is on or not

  Check whether LED is on or not.
  Return TRUE if LED is on, return FALSE if LED is off.

  @param UINT32 uiLED: Which LED, could be the following
    GPIOMAP_LED_GREEN
    GPIOMAP_LED_RED
    GPIOMAP_LED_FCS
  @return BOOL
*/
BOOL LED_IsLEDOn(UINT32 uiLED)
{
	BOOL bvalue = FALSE;
	DBG_IND("LED_IsLEDOn %d\r\n", uiLED);
	switch (uiLED) {
	case GPIOMAP_LED_GREEN:
#if (LED_GREEN_CTRL == LED_GREEN_BY_GPIO)
		bvalue = gpio_getPin(GPIO_GREEN_LED);
#endif
		break;
	case GPIOMAP_LED_RED:
#if (LED_RED_CTRL == LED_RED_BY_GPIO)
		bvalue = gpio_getPin(GPIO_RED_LED);
#endif
		break;
	case GPIOMAP_LED_FCS:
#if (LED_FOCUS_CTRL == LED_FOCUS_BY_GPIO)
		bvalue = gpio_getPin(GPIO_FOCUS_LED);
#endif
		break;
	default:
		bvalue = FALSE;
		break;
	}
	return bvalue;
}

//set FCS LED brightness level
void LED_SetFcsLevel(UINT32 uiLevel)
{
#if (LED_FOCUS_CTRL == LED_FOCUS_BY_PWM)
	FcsLedLvl = uiLevel;
#endif
}

///////////////////////////////////////////////////////////////////////////////

//public func
UINT32 DrvOutGetcaps(UINT32 CapID, UINT32 Param1); // Get Capability Flag (Base on interface version)
UINT32 DrvOutSetcfgs(UINT32 CfgID, UINT32 Param1); // Set Config Setting
UINT32 DrvOutInit(void *pInitParam); // Set Init Parameters
UINT32 DrvOutOpen(void); // Common Constructor
UINT32 DrvOutClose(void); // Common Destructor
UINT32 DrvOutState(UINT32 StateID, UINT32 Value); // General Properties
UINT32 DrvOutControl(UINT32 CtrlID, UINT32 Param1, UINT32 Param2);  // General Methods
UINT32 DrvOutCommand(CHAR *pcCmdStr); //General Command Console

//dx object
DX_OBJECT gDevLED = {
	DXFLAG_SIGN,
	DX_CLASS_STATUS_EXT,
	OUTPUT_VER,
	"Output_LED",
	0, 0, 0, 0,
	DrvOutGetcaps,
	DrvOutSetcfgs,
	DrvOutInit,
	DrvOutOpen,
	DrvOutClose,
	DrvOutState,
	DrvOutControl,
	DrvOutCommand,
	0,
};

UINT32 hDevOut = (UINT32) &gDevLED;

#define LED_BLINK_FAST         (25)   ///< if toggle timer interval set 20ms for 1 Count ,25*20ms = 500ms
#define LED_BLINK_SLOW         (50)   ///< if toggle timer interval set 20ms for 1 Count ,50*20ms = 1000ms

#if (LED_GREEN_CTRL == LED_GREEN_BY_GPIO)
UINT32 uState_LED_GREEN = 0;
UINT32 uState_LED_GREEN_on = 0;
UINT32 uState_LED_GREEN_count = 0;
#endif
#if (LED_RED_CTRL == LED_RED_BY_GPIO)
UINT32 uState_LED_RED = 0;
UINT32 uState_LED_RED_on = 0;
UINT32 uState_LED_RED_count = 0;
#endif
#if (LED_FOCUS_CTRL == LED_FOCUS_BY_GPIO)
UINT32 uState_LED_FCS = 0;
UINT32 uState_LED_FCS_on = 0;
UINT32 uState_LED_FCS_count = 0;
#endif

UINT32 DrvOutGetcaps(UINT32 CapID, UINT32 Param1) // Get Capability Flag (Base on interface version)
{
	UINT32 v = 0;
	switch (CapID & 0x0000ffff) {
	case OUTPUT_CAPS_BASE:
		v = 0;
		break;
	case OUTPUT_CAPS_TIMER:
		//for GPIOMAP_LED_GREEN:
#if (LED_GREEN_CTRL == LED_GREEN_BY_GPIO)
		if (uState_LED_GREEN & 0x80000000) { //if dirty
			uState_LED_GREEN &= ~0x80000000; //clear dirty
			uState_LED_GREEN_count = 0; //reset count
		}
		uState_LED_GREEN_count++; //increase count
		if (((uState_LED_GREEN == 2) && (uState_LED_GREEN_count == LED_BLINK_SLOW))   //SLOW
			|| ((uState_LED_GREEN == 3) && (uState_LED_GREEN_count == LED_BLINK_FAST))) { //FAST
			uState_LED_GREEN_count = 0; //reset count
			if (uState_LED_GREEN_on == TRUE) { //do blink
				gpio_clearPin(GPIO_GREEN_LED);
				uState_LED_GREEN_on = FALSE;
			} else {
				gpio_setPin(GPIO_GREEN_LED);
				uState_LED_GREEN_on = TRUE;
			}
		}
#endif
		//for GPIOMAP_LED_RED:
#if (LED_RED_CTRL == LED_RED_BY_GPIO)
		if (uState_LED_RED & 0x80000000) { //if dirty
			uState_LED_RED &= ~0x80000000; //clear dirty
			uState_LED_RED_count = 0; //reset count
		}
		uState_LED_RED_count++; //increase count
		if (((uState_LED_RED == 2) && (uState_LED_RED_count == LED_BLINK_SLOW))   //SLOW
			|| ((uState_LED_RED == 3) && (uState_LED_RED_count == LED_BLINK_FAST))) { //FAST
			uState_LED_RED_count = 0; //reset count
			if (uState_LED_RED_on == TRUE) { //do blink
				gpio_clearPin(GPIO_RED_LED);
				uState_LED_RED_on = FALSE;
			} else {
				gpio_setPin(GPIO_RED_LED);
				uState_LED_RED_on = TRUE;
			}
		}
#endif
		//for GPIOMAP_LED_FCS:
#if (LED_FOCUS_CTRL == LED_FOCUS_BY_GPIO)
		if (uState_LED_FCS & 0x80000000) { //if dirty
			uState_LED_FCS &= ~0x80000000; //clear dirty
			uState_LED_FCS_count = 0; //reset count
		}
		uState_LED_FCS_count++; //increase count
		if (((uState_LED_FCS == 2) && (uState_LED_FCS_count == LED_BLINK_SLOW))   //SLOW
			|| ((uState_LED_FCS == 3) && (uState_LED_FCS_count == LED_BLINK_FAST))) { //FAST
			uState_LED_FCS_count = 0; //reset count
			if (uState_LED_FCS_on == TRUE) { //do blink
				gpio_clearPin(GPIO_FOCUS_LED);
				uState_LED_FCS_on = FALSE;
			} else {
				gpio_setPin(GPIO_FOCUS_LED);
				uState_LED_FCS_on = TRUE;
			}
		}
#endif
		break;
	default:
		break;
	}
	return v;
}

UINT32 DrvOutSetcfgs(UINT32 CfgID, UINT32 Param1) // Set Config Setting
{
	switch (CfgID) {
	case OUTPUT_CFG_MODE:
		break;
	default:
		break;
	}
	return DX_OK;
}

UINT32 DrvOutInit(void *pInitParam) // Set Init Parameters
{
	DBG_FUNC_BEGIN("\r\n");
	return DX_OK;
}

UINT32 DrvOutOpen(void) // Common Constructor
{
	DBG_FUNC_BEGIN("\r\n");
	return DX_OK;
}

UINT32 DrvOutClose(void) // Common Destructor
{
	DBG_FUNC_BEGIN("\r\n");

	return DX_OK;
}

UINT32 DrvOutState(UINT32 StateID, UINT32 Value) // General Properties
{
	DBG_FUNC_BEGIN("\r\n");

	if (StateID & DXGET) {
		UINT32 rvalue =  0;
		StateID &= ~DXGET;
		switch (StateID) {
		case GPIOMAP_LED_GREEN:
#if (LED_GREEN_CTRL == LED_GREEN_BY_GPIO)
			rvalue = (uState_LED_GREEN & ~0x80000000);
#endif
			break;
		case GPIOMAP_LED_RED:
#if (LED_RED_CTRL == LED_RED_BY_GPIO)
			rvalue = (uState_LED_RED & ~0x80000000);
#endif
			break;
		case GPIOMAP_LED_FCS:
#if (LED_FOCUS_CTRL == LED_FOCUS_BY_GPIO)
			rvalue = (uState_LED_FCS & ~0x80000000);
#endif
			break;
		default:
			break;
		}
		return rvalue;
	} else if (StateID & DXSET) {
		StateID &= ~DXSET;
		DBG_IND("StateID %d %d\r\n", StateID, Value);
		switch (StateID) {
		case GPIOMAP_LED_GREEN:
#if (LED_GREEN_CTRL == LED_GREEN_BY_GPIO)
			uState_LED_GREEN = 0x80000000 | Value; //set dirty
			if (Value == 0) {
				gpio_clearPin(GPIO_GREEN_LED);
				uState_LED_GREEN_on = FALSE;
			} else {
				gpio_setPin(GPIO_GREEN_LED);
				uState_LED_GREEN_on = FALSE;
			}
#endif
			break;

		case GPIOMAP_LED_RED:
#if (LED_RED_CTRL == LED_RED_BY_GPIO)
			uState_LED_RED = 0x80000000 | Value; //set dirty
			if (Value == 0) {
				gpio_clearPin(GPIO_RED_LED);
				uState_LED_RED_on = FALSE;
			} else {
				gpio_setPin(GPIO_RED_LED);
				uState_LED_RED_on = FALSE;
			}
#endif
			break;

		case GPIOMAP_LED_FCS:
#if (LED_FOCUS_CTRL == LED_FOCUS_BY_GPIO)
			uState_LED_FCS = 0x80000000 | Value; //set dirty
			if (Value == 0) {
				gpio_clearPin(GPIO_FOCUS_LED);
				uState_LED_FCS_on = FALSE;
			} else {
				gpio_setPin(GPIO_FOCUS_LED);
				uState_LED_FCS_on = TRUE;
			}
#endif
#if (LED_FOCUS_CTRL == LED_FOCUS_BY_PWM)
			if (Value == 0) {
				//set FOCUS LED brightness level
				PWM_CFG PWMInfo;
				PWMInfo.uiDiv = 220;
				PWMInfo.uiPrd = 0;
				PWMInfo.uiFall = 0;
				PWMInfo.uiOnCycle = 0;
				PWMInfo.uiInv = 0;
				PWMInfo.uiRise = 0;
				pwm_set(PWMID_FOCUS_LED, &PWMInfo);
				pwm_reload(PWMID_FOCUS_LED);
			} else {
				//set FOCUS LED brightness level
				PWM_CFG PWMInfo;
				if (pwm_open(PWMID_FOCUS_LED) != E_OK) {
					pwm_stop(PWMID_FOCUS_LED);
					pwm_close(PWMID_FOCUS_LED, TRUE);
					pwm_open(PWMID_FOCUS_LED);
				}
				PWMInfo.uiDiv = 220;
				PWMInfo.uiPrd = 22;
				PWMInfo.uiFall = FcsLedPrdVal[Value];
				PWMInfo.uiOnCycle = 0;
				PWMInfo.uiInv = 0;
				PWMInfo.uiRise = 0;
				pwm_set(PWMID_FOCUS_LED, &PWMInfo);
				pwm_reload(PWMID_FOCUS_LED);
			}
#endif
			break;
		default:
			break;
		}
	}
	return DX_OK;
}

UINT32 DrvOutControl(UINT32 CtrlID, UINT32 Param1, UINT32 Param2)  // General Methods
{
	DBG_FUNC_BEGIN("\r\n");

	switch (CtrlID) {
	case OUTPUT_CTRL_MODE:
		break;

	default:
		break;
	}
	return DX_OK;
}

UINT32 DrvOutCommand(CHAR *pcCmdStr) //General Command Console
{
	switch (*pcCmdStr) {
	case 'v':
		if (!strncmp(pcCmdStr, "ver", 3)) {
			DBG_DUMP("0x%X\r\n", OUTPUT_VER);
			return TRUE;
		}
		break;
	}
	return FALSE;
}


//@}
