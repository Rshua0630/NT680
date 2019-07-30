#include "DxCfg.h"
#include "IOCfg.h"
#include "Debug.h"
#include "DxWiFi.h"
#include "SwTimer.h"

///////////////////////////////////////////////////////////////////////////////
#define __MODULE__          DxWiFi
#define __DBGLVL__          2 // 0=FATAL, 1=ERR, 2=WRN, 3=UNIT, 4=FUNC, 5=IND, 6=MSG, 7=VALUE, 8=USER
#define __DBGFLT__          "*" //*=All, [mark]=CustomClass
#include "DebugModule.h"
///////////////////////////////////////////////////////////////////////////////
void DrvWiFi_PowerOn(void)
{
    DBG_FUNC_BEGIN("[API]\r\n");
    gpio_clearPin(GPIO_WIFI_PWR);
    SwTimer_DelayMs(20);
    DBG_FUNC_END("[API]\r\n");
}

void DrvWiFi_PowerOff(void)
{
    DBG_FUNC_BEGIN("[API]\r\n");
    gpio_setPin(GPIO_WIFI_PWR);
    SwTimer_DelayMs(20);
    DBG_FUNC_END("[API]\r\n");
}

