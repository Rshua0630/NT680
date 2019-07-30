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
    LensDevice.pGPIOArray[LENS_IO_RESET]       =  GPIO_LENS_RESET;
    LensDevice.pGPIOArray[LENS_IO_IN_0]        =  GPIO_LENS_IR_CTRL0;
    LensDevice.pGPIOArray[LENS_IO_IN_1]        =  GPIO_LENS_IR_CTRL1;
    LensDevice.pGPIOArray[LENS_IO_ZOOM_PIINT]  =  GPIO_LENS_ZOOM_INTR;
    LensDevice.pGPIOArray[LENS_IO_FOCUS_PIINT] =  GPIO_LENS_FOCUS_INTR;
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


