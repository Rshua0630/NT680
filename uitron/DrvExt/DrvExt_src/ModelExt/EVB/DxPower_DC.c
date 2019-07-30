#include "DxCfg.h"
#include "IOCfg.h"
#include "DxPower.h"
#include "Utility.h"
#include "DxCommon.h"
#include "DxApi.h"
#include "Dx.h"
#include "wdt.h"
#include "HwPower.h"
#include "HwClock.h"

#define THIS_DBGLVL         2 // 0=FATAL, 1=ERR, 2=WRN, 3=UNIT, 4=FUNC, 5=IND, 6=MSG, 7=VALUE, 8=USER
///////////////////////////////////////////////////////////////////////////////
#define __MODULE__          DxPwr
#define __DBGLVL__          THIS_DBGLVL
#define __DBGFLT__          "*" //*=All, [mark]=CustomClass
#include "DebugModule.h"


static UINT32 DrvPowerGetcaps(UINT32 CapID, UINT32 Param1);
static UINT32 DrvPowercfgs(UINT32 CfgID, UINT32 Param1);
static UINT32 DrvPowerInit(void *pInitParam);
static UINT32 DrvPowerOpen(void);
static UINT32 DrvPowerClose(void);
static UINT32 DrvPowerState(UINT32 StateID, UINT32 Value);
static UINT32 DrvPowerControl(UINT32 CtrlID, UINT32 Param1, UINT32 Param2);
static UINT32 DrvPowerCommand(CHAR *pcCmdStr);
static UINT32 DrvPower_GetControl(DRVPWR_CTRL DrvPwrCtrl);
static void   DrvPower_SetControl(DRVPWR_CTRL DrvPwrCtrl, UINT32 value);

DX_OBJECT gDevPowerDC = {
	DXFLAG_SIGN,
	DX_CLASS_POWER_EXT,
	POWER_VER,
	"PowerDC",
	0, 0, 0, 0,
	DrvPowerGetcaps,
	DrvPowercfgs,
	DrvPowerInit,
	DrvPowerOpen,
	DrvPowerClose,
	DrvPowerState,
	DrvPowerControl,
	DrvPowerCommand,
	0,
};

#define USE_HWTIMER     DISABLE //ENABLE

//-----------------------------------------------------------------------------
// Soft Reset (Watch Dog)
//-----------------------------------------------------------------------------

//#Fine tune the watch dog timer value
#define WDT_TIMER_INTERVAL         160
#define WDT_TIMEOUT                350     //350 ms

static BOOL gLoopQuit = FALSE;
extern void _DrvPowerDC_Quit(void);
void _DrvPowerDC_Quit(void)
{
	gLoopQuit = TRUE;
}

static void DrvPower_PowerOff(void)
{
	// Loop forever
	while (!gLoopQuit) { //fix for CID 43219
		;
	}
}


#if (USE_HWTIMER == ENABLE)
static TIMER_ID     g_WatchDog_TimerID = 0xffffffffU;
#else
static SWTIMER_ID   g_WatchDog_TimerID = 0xffffffffU;
#endif
static UINT32       g_WatchDog_Timeout = 0;
static BOOL         g_WatchDog_Enable = 0;

static void _WatchDog_Isr(UINT32 uiEvent)
{
	// kick
	wdt_trigger();
}

static void DrvPower_SoftReset_ClearCounter(void)
{
	// kick
	wdt_trigger();
}

static void DrvPower_SoftReset_SetCounter(UINT32 uiTimeOut)
{
	if (g_WatchDog_TimerID != 0xffffffffU) {
		return;    //already enable
	}
	g_WatchDog_Timeout = uiTimeOut;
}

static void DrvPower_SoftReset_Enable(BOOL bEnable)
{
	if (bEnable) {
		if (g_WatchDog_Enable) {
			return;
		}

#if (USE_HWTIMER == ENABLE)
		if (timer_open(&g_WatchDog_TimerID, _WatchDog_Isr) != E_OK)
#else
		if (SwTimer_Open(&g_WatchDog_TimerID, _WatchDog_Isr) != E_OK)
#endif
		{
			DBG_ERR("WDT timer open failed!\r\n");
			g_WatchDog_TimerID = 0xffffffffU;
			return;
		}

		//to avoid wdt already in use
		wdt_close();
		DBG_DUMP("+(watch-dog)\r\n");
		Delay_DelayUs(1000); //wait message output complet
		// Open
		wdt_open();
		// Config WDT to [uiTimeOut] ms, reset system
		wdt_setConfig(WDT_CONFIG_ID_MODE, WDT_MODE_RESET);
		wdt_setConfig(WDT_CONFIG_ID_TIMEOUT, g_WatchDog_Timeout);
		// Enable WDT
		wdt_enable();
		// Assign ISR
#if (USE_HWTIMER == ENABLE)
		timer_cfg(g_WatchDog_TimerID, 1000 * WDT_TIMER_INTERVAL, TIMER_MODE_FREE_RUN | TIMER_MODE_ENABLE_INT, TIMER_STATE_PLAY);
#else
		SwTimer_Cfg(g_WatchDog_TimerID, WDT_TIMER_INTERVAL, SWTIMER_MODE_FREE_RUN);
		SwTimer_Start(g_WatchDog_TimerID);
#endif

		g_WatchDog_Enable = 1;
	} else {
		if (!g_WatchDog_Enable) {
			return;
		}

		if (g_WatchDog_TimerID != 0xffffffffU) {
#if (USE_HWTIMER == ENABLE)
			timer_close(g_WatchDog_TimerID);
#else
			SwTimer_Close(g_WatchDog_TimerID);
#endif
			g_WatchDog_TimerID = 0;
		}
		DBG_DUMP("-(watch-dog)\r\n");
		// Disable WDT
		wdt_disable();
		wdt_close();

		g_WatchDog_Enable = 0;
	}
}

static void DrvPower_SoftReset(void)
{
	DrvPower_SoftReset_Enable(FALSE);
	DBG_DUMP("(sw-reset)\r\n");
#if (_FPGA_EMULATION_ != ENABLE)
	Delay_DelayUs(1000); //wait message output complete
#endif

	//setting wdt start
	// Open
	wdt_open();
	// Config WDT to 350 ms, reset system
	wdt_setConfig(WDT_CONFIG_ID_MODE, WDT_MODE_RESET);
	//#NT#2017/11/22#HM Tseng -begin
	//#NT#Only for normal reboot
	wdt_setConfig(WDT_CONFIG_ID_EXT_RESET, WDT_EXT_RESET);
	//#NT#2017/11/22#HM Tseng -end
	wdt_setConfig(WDT_CONFIG_ID_TIMEOUT, WDT_TIMEOUT);
	// Enable WDT
	wdt_enable();

	//wait to system reset
#if (_FPGA_EMULATION_ != ENABLE)
	abort();
#else
	// Loop forever
	while (1) {
		_ASM_NOP
	}
#endif
}

static UINT32 DrvPower_GetPowerKey(UINT32 pwrID)
{
	switch (pwrID) {
	case POWER_ID_SWRT:
		return 0; //keep 1 if soft reset
	default:
		DBG_ERR("unknown pwr id (%d)\r\n", pwrID);
		break;
	}
	return 0;
}


static void DrvPower_SetPowerKey(UINT32 pwrID, UINT32 value)
{
	switch (pwrID) {
	case POWER_ID_SWRT:
		if (value == 0) { //disable s/w reset timer
			DrvPower_SoftReset_Enable(FALSE);
		}
		if (value == 1) { //enable s/w reset timer
			DrvPower_SoftReset_Enable(TRUE);
		}
		if (value == 0xf0) { //keep power on - clear s/w reset timer
			DrvPower_SoftReset_ClearCounter();
		}
		if (value == 0xff) { //s/w reset NOW
			DrvPower_SoftReset();
		}
		break;
	default:
		DBG_ERR("unknown pwr id (%d)\r\n", pwrID);
		break;
	}
}


static void   DrvPower_SetControl(DRVPWR_CTRL DrvPwrCtrl, UINT32 value)
{
	DBG_IND("DrvPwrCtrl(%d), value(%d)\r\n", DrvPwrCtrl, value);
	switch (DrvPwrCtrl) {
	case DRVPWR_CTRL_BATTERY_CALIBRATION_EN:
		break;

	case DRVPWR_CTRL_BATTERY_ADC_CAL_OFFSET:
		break;

#if USB_CHARGE_FUNCTION
	case DRVPWR_CTRL_BATTERY_CHARGE_EN:
		break;
	case DRVPWR_CTRL_BATTERY_CHARGE_CURRENT:
		break;
	case DRVPWR_CTRL_BATTERY_CHARGE_ISET:
		break;
	case DRVPWR_CTRL_BATTERY_CHARGE_VSET:
		break;
	case DRVPWR_CTRL_ENABLE_CHARGEIC:
		break;
#endif
	case DRVPWR_CTRL_SWRT_COUNTER:
		DrvPower_SoftReset_SetCounter(value);
		break;
	case DRVPWR_CTRL_SWRT:
		DrvPower_SetPowerKey(POWER_ID_SWRT, value);
		break;
	case DRVPWR_CTRL_PSW1:
		DBG_WRN("DrvPwrCtrl(%d) not supported\r\n", DrvPwrCtrl);
		if (value == 0xff) { //power off
			DrvPower_PowerOff();
		}
		break;
	case DRVPWR_CTRL_HWRT:
		DBG_WRN("DrvPwrCtrl(%d) not supported\r\n", DrvPwrCtrl);
		break;
	default:
		DBG_ERR("DrvPwrCtrl(%d)\r\n", DrvPwrCtrl);
		break;
	}
}

static UINT32  DrvPower_GetControl(DRVPWR_CTRL DrvPwrCtrl)
{
	UINT32 rvalue = 0;
	switch (DrvPwrCtrl) {
	case DRVPWR_CTRL_BATTERY_LEVEL:
		break;
	case DRVPWR_CTRL_BATTERY_ADC_VALUE:
		break;
	case DRVPWR_CTRL_BATTERY_ADC_CAL_OFFSET:
		break;
	case DRVPWR_CTRL_IS_DUMMUYLOAD_POWEROFF:
		break;
	case DRVPWR_CTRL_IS_BATT_OVERHEAT:
		break;
#if USB_CHARGE_FUNCTION
	case DRVPWR_CTRL_IS_BATT_INSERT:
		break;
	case DRVPWR_CTRL_IS_DEAD_BATT:
		break;
	case DRVPWR_CTRL_IS_NEED_RECHARGE:
		break;
	case DRVPWR_CTRL_BATTERY_CHARGE_EN:
		break;
	case DRVPWR_CTRL_BATTERY_CHARGE_OK:
		break;
	case DRVPWR_CTRL_BATTERY_CHARGE_CURRENT:
		break;
	case DRVPWR_CTRL_BATTERY_CHARGE_ISET:
		break;
	case DRVPWR_CTRL_BATTERY_CHARGE_VSET:
		break;
	case DRVPWR_CTRL_BATTERY_TEMPERATURE:
		break;
#else
	case DRVPWR_CTRL_BATTERY_CHARGE_EN:
		rvalue = FALSE;
		break;
#endif
	case DRVPWR_CTRL_SWRT:
		rvalue = DrvPower_GetPowerKey(POWER_ID_SWRT);
		break;
	case DRVPWR_CTRL_POWERON_SOURCE:
	case DRVPWR_CTRL_POWER_LOST:
	case DRVPWR_CTRL_PSW1:
	case DRVPWR_CTRL_PSW2:
	case DRVPWR_CTRL_PSW3:
	case DRVPWR_CTRL_PSW4:
	case DRVPWR_CTRL_HWRT:
		DBG_WRN("DrvPwrCtrl(%d) not supported\r\n", DrvPwrCtrl);
		break;
	default:
		DBG_ERR("DrvPwrCtrl(%d)\r\n", DrvPwrCtrl);
		break;
	}
	return rvalue;
}
/*
void DrvPower_RegCB(DX_CALLBACK_PTR fpDxPowerCB)
{

    g_fpDxPowerCB = fpDxPowerCB;
}
*/
///////////DX object
static UINT32 DrvPowerGetcaps(UINT32 CapID, UINT32 Param1)
{
	UINT32 v = 0;
	switch (CapID & 0x0000ffff) {
	case POWER_CAPS_BASE:
		v = POWER_BF_DRTC;
		break;
	default:
		break;
	}
	return v;
}

static UINT32 DrvPowercfgs(UINT32 CfgID, UINT32 Param1)
{

	return DX_OK;
}
static UINT32 DrvPowerInit(void *pInitParam)
{
	DBG_IND("Init\r\n");
	return DX_OK;
}

static UINT32 DrvPowerOpen(void)
{
	DBG_IND("open\r\n");
	return DX_OK;
}

static UINT32 DrvPowerClose(void)
{
	DBG_IND("close\r\n");
	return DX_OK;
}

static UINT32 DrvPowerState(UINT32 StateID, UINT32 Value)
{
	if (StateID & DXGET) {
		StateID &= ~DXGET;
		return DrvPower_GetControl(StateID);
	} else if (StateID & DXSET) {
		StateID &= ~DXSET;
		DrvPower_SetControl(StateID, Value);
	}
	return DX_OK;
}
static UINT32 DrvPowerControl(UINT32 CtrlID, UINT32 Param1, UINT32 Param2)
{

	return DX_OK;
}

static UINT32 DrvPowerCommand(CHAR *pcCmdStr)  //General Command Console
{

	switch (*pcCmdStr) {
	case 'd':
		/*if (!strncmp(pcCmdStr, "Nand dump", 9))
		{
		    return TRUE;
		}*/
		break;
	}
	return FALSE;
}
/*
static void DrvPowerCallback(UINT32 EventID, UINT32 Param1, UINT32 Param2){

    if(g_fpDxPowerCB){
        g_fpDxPowerCB(EventID,Param1,Param2);
    }
}
*/




