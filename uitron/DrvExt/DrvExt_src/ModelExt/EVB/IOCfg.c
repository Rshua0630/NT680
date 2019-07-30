/**
    Copyright   Novatek Microelectronics Corp. 2005.  All rights reserved.

    @file       IOCfg.c
    @ingroup    mIPRJAPCommonIO

    @brief      IO config module
                This file is the IO config module

    @note       Nothing.

    @date       2005/12/24
*/

/** \addtogroup mIPRJAPCommonIO */
//@{

#include "Type.h"
#include "DrvExt.h"
//#include <stdio.h>
//#include <stdlib.h>
//#include <string.h>
#include "Debug.h"
#include "IOCfg.h"
#include "Utility.h"
#include "pll.h"
#include "IOInit.h"
#include "IntDirCfg.h"
#include "gyro_drv_ext.h"

//#include "timer.h"

///////////////////////////////////////////////////////////////////////////////
#define __MODULE__          DxDrv
#define __DBGLVL__          2 // 0=FATAL, 1=ERR, 2=WRN, 3=UNIT, 4=FUNC, 5=IND, 6=MSG, 7=VALUE, 8=USER
#define __DBGFLT__          "*" //*=All, [mark]=CustomClass
#include "DebugModule.h"
///////////////////////////////////////////////////////////////////////////////

#if (USE_VIO == ENABLE)
UINT32 Virtual_IO[VIO_MAX_ID] = {0};
UINT32 vio_getpin(UINT32 id)
{
	if (id >= VIO_MAX_ID) {
		return 0;
	}
	return Virtual_IO[id];
}
void vio_setpin(UINT32 id, UINT32 v)
{
	if (id >= VIO_MAX_ID) {
		return;
	}
	Virtual_IO[id] = v;
}
#endif

#define GPIO_SET_NONE           0xffffff
#define GPIO_SET_OUTPUT_LOW     0x0
#define GPIO_SET_OUTPUT_HI      0x1

/**
  Do GPIO initialization

  Initialize input/output pins, and pin status

  @param void
  @return void
*/
void IO_InitGPIO(void)
{
	UINT32 iSValue;

	MODELEXT_HEADER *header;
	GPIO_INIT_OBJ *uiGPIOMapInitTab;

	uiGPIOMapInitTab = (GPIO_INIT_OBJ *)Dx_GetModelExtCfg(MODELEXT_TYPE_GPIO_INFO, &header);
	if (!uiGPIOMapInitTab || !header) {
		DBG_FATAL("Modelext: iocfg is null\n");
		return;
	}

	UINT32 totalGpioInitCount = header->number;

	DBG_IND("GPIO START\r\n");
	//--------------------------------------------------------------------
	// Open GPIO driver
	//--------------------------------------------------------------------
	gpio_open();
	for (iSValue = 0 ; iSValue < totalGpioInitCount ; iSValue++) {
		if (uiGPIOMapInitTab[iSValue].GpioDir == GPIO_DIR_INPUT) {
			gpio_setDir(uiGPIOMapInitTab[iSValue].GpioPin, GPIO_DIR_INPUT);
			pad_setPullUpDown(uiGPIOMapInitTab[iSValue].PadPin, uiGPIOMapInitTab[iSValue].PadDir);
		} else {
			gpio_setDir(uiGPIOMapInitTab[iSValue].GpioPin, GPIO_DIR_OUTPUT);
			if (uiGPIOMapInitTab[iSValue].PadDir == GPIO_SET_OUTPUT_HI) {
				gpio_setPin(uiGPIOMapInitTab[iSValue].GpioPin);
			} else {
				gpio_clearPin(uiGPIOMapInitTab[iSValue].GpioPin);
			}
		}
	}

	//--------------------------------------------------------------------
	// Use Non-Used PWM to be Delay Timer
	//--------------------------------------------------------------------
#if defined(PWMID_TIMER)
	Delay_setPwmChannels(PWMID_TIMER);
#endif

	DBG_IND("GPIO END\r\n");

}

/**
  Initialize voltage detection

  Initialize voltage detection for battery and flash

  @param void
  @return void
*/

void IO_InitADC(void)
{
#if 0
	if (adc_open(ADC_CH_VOLDET_BATTERY) != E_OK) {
		DBG_ERR("Can't open ADC channel for battery voltage detection\r\n");
		return;
	}

	//650 Range is 250K Hz ~ 2M Hz
	adc_setConfig(ADC_CONFIG_ID_OCLK_FREQ, 250000); //250K Hz

	//battery voltage detection
	adc_setChConfig(ADC_CH_VOLDET_BATTERY, ADC_CH_CONFIG_ID_SAMPLE_FREQ, 10000); //10K Hz, sample once about 100 us for CONTINUOUS mode
	adc_setChConfig(ADC_CH_VOLDET_BATTERY, ADC_CH_CONFIG_ID_SAMPLE_MODE, (VOLDET_ADC_MODE) ? ADC_CH_SAMPLEMODE_CONTINUOUS : ADC_CH_SAMPLEMODE_ONESHOT);
	adc_setChConfig(ADC_CH_VOLDET_BATTERY, ADC_CH_CONFIG_ID_INTEN, FALSE);
#endif

#if (ADC_KEY == ENABLE)
	adc_open(ADC_CH_VOLDET_KEY1);
	adc_setChConfig(ADC_CH_VOLDET_KEY1, ADC_CH_CONFIG_ID_SAMPLE_FREQ, 10000); //10K Hz, sample once about 100 us for CONTINUOUS mode
	adc_setChConfig(ADC_CH_VOLDET_KEY1, ADC_CH_CONFIG_ID_SAMPLE_MODE, (VOLDET_ADC_MODE) ? ADC_CH_SAMPLEMODE_CONTINUOUS : ADC_CH_SAMPLEMODE_ONESHOT);
	adc_setChConfig(ADC_CH_VOLDET_KEY1, ADC_CH_CONFIG_ID_INTEN, FALSE);
#endif

	// Enable adc control logic
	adc_setEnable(TRUE);

	//???
	Delay_DelayMs(15); //wait ADC stable  //for pwr on speed up
}

static void UninitADC(void)
{
#if (ADC_KEY == ENABLE)
	adc_setEnable(FALSE);
	adc_close(ADC_CH_VOLDET_KEY1);
#endif
}

static void IO_InitGyro(void)
{
    GYRO_CFGINFO  g_GyroCfgInfo= {0};

    g_GyroCfgInfo.AxisSelec[0] = 1; //RSC_GYRO_AXIS_X;
    g_GyroCfgInfo.AxisSelec[1] = 0; //RSC_GYRO_AXIS_Y;
    g_GyroCfgInfo.AxisSelec[2] = 2; //RSC_GYRO_AXIS_Z;
    g_GyroCfgInfo.DirSelec[0] = 0; //RSC_DIR_POS
    g_GyroCfgInfo.DirSelec[1] = 1; //RSC_DIR_POS
    g_GyroCfgInfo.DirSelec[2] = 1; //RSC_DIR_POS
    gyro_cfg(GYRO_SENSOR_ID_1,&g_GyroCfgInfo);
}

void Dx_InitIO(void)  // Config IO for external device
{
	// when cpu2 is linux , the pinmux and interrupt destination is setted by uboot
#if !defined(_CPU2_LINUX_)
	IO_InitIntDir();    //initial interrupt destination
#endif
	IO_InitPinmux();    //initial PINMUX config (there are 2 ways in it: uitron only or with cpu2)
	IO_InitGPIO();      //initial GPIO pin status
	IO_InitADC();       //initial ADC pin status
    IO_InitGyro();      //initial gyro x,y,z direction
}

void Dx_UninitIO(void)  // Config IO for external device
{
	UninitADC();
}



