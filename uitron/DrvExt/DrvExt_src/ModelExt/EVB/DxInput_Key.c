/**
    Copyright   Novatek Microelectronics Corp. 2009.  All rights reserved.

    @file       DetKey.c
    @ingroup    mIPRJAPKeyIO

    @brief      Scan key, modedial
                Scan key, modedial

    @note       Nothing.

    @date       2017/05/02
*/

/** \addtogroup mIPRJAPKeyIO */
//@{

#include "DxCfg.h"
#include "IOCfg.h"

#include "DxInput.h"
#include "Debug.h"
#include "KeyDef.h"
#include "rtc.h"
#include "HwPower.h"
#include "Delay.h"

///////////////////////////////////////////////////////////////////////////////
#define __MODULE__          DxDrv
#define __DBGLVL__          2 // 0=FATAL, 1=ERR, 2=WRN, 3=UNIT, 4=FUNC, 5=IND, 6=MSG, 7=VALUE, 8=USER
#define __DBGFLT__          "*" //*=All, [mark]=CustomClass
#include "DebugModule.h"
///////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// ADC related
////////////////////////////////////////////////////////////////////////////////
#if (ADC_KEY == ENABLE)
#define VOLDET_KEY_ADC_LVL1          (251)
#define VOLDET_KEY_ADC_LVL2          (155)
#define VOLDET_KEY_ADC_LVL3          (51)
#define VOLDET_KEY_ADC_TH            (460)
#define VOLDET_KEY_ADC_LVL4          (358)

#define VOLDET_KEY_LVL_UNKNOWN           0xFFFFFFFF
#define VOLDET_KEY_LVL_0                 0
#define VOLDET_KEY_LVL_1                 1
#define VOLDET_KEY_LVL_2                 2
#define VOLDET_KEY_LVL_3                 3
#define VOLDET_KEY_LVL_4                 4
#define VOLDET_KEY_LVL_5                 5
#endif

#if (ADC_KEY == ENABLE)
static UINT32 VolDet_GetKey1ADC(void)
{
#if (VOLDET_ADC_CONT_MODE == DISABLE)
	UINT32 uiADCValue;

	uiADCValue = adc_readData(ADC_CH_VOLDET_KEY1);
	// One-Shot Mode, trigger one-shot
	adc_triggerOneShot(ADC_CH_VOLDET_KEY1);

	return uiADCValue;
#else
	return adc_readData(ADC_CH_VOLDET_KEY1);
#endif
}
/**
  Get ADC key voltage level

  Get  ADC key  2 voltage level.

  @param void
  @return UINT32 key level, refer to VoltageDet.h -> VOLDET_MS_LVL_XXXX
*/
static UINT32 VolDet_GetKey1Level(void)
{
	static UINT32   uiRetKey1Lvl;
	UINT32          uiKey1ADC, uiCurKey2Lvl;

	uiKey1ADC = VolDet_GetKey1ADC();
	DBG_IND("uiKey1ADC %d \r\n", uiKey1ADC);
	if (uiKey1ADC < VOLDET_KEY_ADC_TH) {
		if (uiKey1ADC < VOLDET_KEY_ADC_LVL3) {
			uiCurKey2Lvl = VOLDET_KEY_LVL_4;
		} else if (uiKey1ADC < VOLDET_KEY_ADC_LVL2) {
			uiCurKey2Lvl = VOLDET_KEY_LVL_3;
		} else if (uiKey1ADC < VOLDET_KEY_ADC_LVL1) {
			uiCurKey2Lvl = VOLDET_KEY_LVL_2;
		} else if (uiKey1ADC < VOLDET_KEY_ADC_LVL4) {
			uiCurKey2Lvl = VOLDET_KEY_LVL_1;
		} else {
			uiCurKey2Lvl = VOLDET_KEY_LVL_0;
		}
	} else {
		uiCurKey2Lvl = VOLDET_KEY_LVL_UNKNOWN;
	}

	uiRetKey1Lvl = uiCurKey2Lvl;

	return uiRetKey1Lvl;
}

/**
  Detect Mode Switch state.

  Detect Mode Switch state.

  @param void
  @return UINT32 Mode Switch state (DSC Mode)
*/
#endif

////////////////////////////////////////////////////////////////////////////////
// GPIO related

//static BOOL g_bIsShutter2Pressed = FALSE;

/**
  Delay between toggle GPIO pin of input/output

  Delay between toggle GPIO pin of input/output

  @param void
  @return void
*/
static void DrvKey_DetKeyDelay(void)
{
	gpio_readData(0);
	gpio_readData(0);
	gpio_readData(0);
	gpio_readData(0);
}

void DrvKey_Init(void)
{
}
/**
  Detect normal key is pressed or not.

  Detect normal key is pressed or not.
  Return key pressed status (refer to KeyDef.h)

  @param void
  @return UINT32
*/
UINT32 DrvKey_DetNormalKey(void)
{
	UINT32 uiKeyCode = 0;

#if (ADC_KEY == ENABLE)
	UINT32 uiKey1Lvl = VolDet_GetKey1Level();
	switch (uiKey1Lvl) {
	case VOLDET_KEY_LVL_UNKNOWN:
	default:
		break;
	case VOLDET_KEY_LVL_0:
		uiKeyCode |= FLGKEY_RIGHT;
		break;
	case VOLDET_KEY_LVL_1:
		uiKeyCode |= FLGKEY_DOWN;
		break;
	case VOLDET_KEY_LVL_2:
		uiKeyCode |= FLGKEY_SHUTTER2;
		break;
	case VOLDET_KEY_LVL_3:
		uiKeyCode |= FLGKEY_UP;
		break;
	case VOLDET_KEY_LVL_4:
		uiKeyCode |= FLGKEY_ENTER;
		break;
	}
#endif


#if (GPIO_KEY == ENABLE)
	if (gpio_getPin(GPIO_KEY_LEFT)) {
		uiKeyCode |= FLGKEY_LEFT;
	}
	if (gpio_getPin(GPIO_KEY_SHUTTER1)) {
		uiKeyCode |= FLGKEY_SHUTTER1;
	}
	if (gpio_getPin(GPIO_KEY_SHUTTER2)) {
		uiKeyCode |= FLGKEY_SHUTTER2;
	}
	if (gpio_getPin(GPIO_KEY_ZOOMIN)) {
		uiKeyCode |= FLGKEY_ZOOMIN;
	}
	if (gpio_getPin(GPIO_KEY_ZOOMOUT)) {
		uiKeyCode |= FLGKEY_ZOOMOUT;
	}
	if (gpio_getPin(GPIO_KEY_ENTER)) {
		uiKeyCode |= FLGKEY_ENTER;
	}
#endif
	/*
	    //detect if power-on by press playback key
	    if (!HwPower_GetPowerKey(POWER_ID_PSW2))
	    {
	        uiKeyCode |= FLGKEY_PLAYBACK;
	    }
	*/
	DBG_IND("KEY=%08x\r\n", uiKeyCode);

	DrvKey_DetKeyDelay();
	return uiKeyCode;
}
/**
  Detect power key is pressed or not.

  Detect power key is pressed or not.
  Return key pressed status (refer to KeyDef.h)

  @param void
  @return UINT32
*/
UINT32 DrvKey_DetPowerKey(void)
{
	UINT32 uiKeyCode = 0;

	if (HwPower_GetPowerKey(POWER_ID_PSW1)) {
		uiKeyCode = FLGKEY_KEY_POWER;
		// Reset shutdown timer
		HwPower_SetPowerKey(POWER_ID_PSW1, 0xf0);
	}
	return uiKeyCode;
}
UINT32 DrvKey_DetStatusKey(DX_STATUS_KEY_GROUP KeyGroup)
{
	UINT32 uiReturn = STATUS_KEY_LVL_UNKNOWN;
	switch (KeyGroup) {
	case DX_STATUS_KEY_GROUP1:
		break;

	case DX_STATUS_KEY_GROUP2:
		break;

	case DX_STATUS_KEY_GROUP3:
		break;

	case DX_STATUS_KEY_GROUP4:
		break;

	case DX_STATUS_KEY_GROUP5:
		break;

	default:
		DBG_ERR("[StatusKey]no this attribute");
		break;
	}
	return uiReturn;
}
