/**
 * Novatek lens driver.
 * 
 * Reference lens driver implementation.
 * 
 * @file DxCamera_Lens.c
 * @note Nothing.
 *
 * Copyright (C) 2017 Novatek Microelectronics Corp. All rights reserved.
 *
 */

#include "DxCfg.h"
#include "IOCfg.h"
#include "Debug.h"
#include "DxLens.h"
#include "Utility.h"


#define __MODULE__          DXLENS
#define __DBGLVL__          2      // 0=FATAL, 1=ERR, 2=WRN, 3=UNIT, 4=FUNC, 5=IND, 6=MSG, 7=VALUE, 8=USER
#define __DBGFLT__          "*"    //*=All, [mark]=CustomClass
#include "DebugModule.h"


static LENSCTRL_OBJ LensCtrl;
static LENS_DEVICE_OBJ LensDevice;
static UINT32 g_uiGPIOCfg[LENS_IO_MAX];


void DrvLens_PowerOn(void)
{
    LensCtrl.pLens = DrvLens_GetLens();
    LensCtrl.pLensDev = DrvLens_GetDevice();
}

void DrvLens_PowerOff(void)
{
    ;
}

PLENS_DEVICE_OBJ DrvLens_GetDevice(void)
{
    LensDevice.pGPIOArray = &g_uiGPIOCfg[0];

#if 0
    LensDevice.pGPIOArray[LENS_IO_IN_0]       =  GPIO_LENS_IN1A;
    LensDevice.pGPIOArray[LENS_IO_IN_1]       =  GPIO_LENS_IN1B;
    LensDevice.pGPIOArray[LENS_IO_IN_2]       =  GPIO_LENS_IN2A;
    LensDevice.pGPIOArray[LENS_IO_IN_3]       =  GPIO_LENS_IN2B;
    LensDevice.pGPIOArray[LENS_IO_IN_4]       =  GPIO_LENS_IN3A;
    LensDevice.pGPIOArray[LENS_IO_IN_5]       =  GPIO_LENS_IN3B;
    LensDevice.pGPIOArray[LENS_IO_IN_6]       =  GPIO_LENS_IN4A;
    LensDevice.pGPIOArray[LENS_IO_IN_7]       =  GPIO_LENS_IN4B;
    LensDevice.pGPIOArray[LENS_IO_IN_8]       =  GPIO_LENS_IN5A;
    LensDevice.pGPIOArray[LENS_IO_IN_9]       =  GPIO_LENS_IN5B;
    LensDevice.pGPIOArray[LENS_IO_IN_10]      =  GPIO_LENS_IN6A;
    LensDevice.pGPIOArray[LENS_IO_IN_11]      =  GPIO_LENS_IN6B;
    LensDevice.pGPIOArray[LENS_IO_ZOOM_PI]    =  GPIO_LENS_ZOOM_PI;
    LensDevice.pGPIOArray[LENS_IO_ZOOM_PR]    =  GPIO_LENS_ZOOM_PR;
    LensDevice.pGPIOArray[LENS_IO_FOCUS_PI]   =  GPIO_LENS_FOCUS_PI;
    LensDevice.pGPIOArray[LENS_IO_ZOOM_PIINT] =  GPIO_LENS_ZOOM_PI_ITR;
#endif

    return &LensDevice;
}

PLENSCTRL_OBJ DrvLens_GetLensCtrl(void)
{
    if (LensCtrl.pLens && LensCtrl.pLens->pMotor && LensCtrl.pLensDev) {
        return &LensCtrl;
    } else {
        DBG_ERR("LensCtrl obj not initialized!\r\n");
        return NULL;
    }
}

void DrvLens_RegLensCB(void *plens_task_cb, void *plens_api_cb)
{
    LensCtrl.TaskCB = (FPLENS_CALLBACK)plens_task_cb;
    LensCtrl.APICB  = (FPLENS_CALLBACK)plens_api_cb;
}


