#include "DxCfg.h"
#include "IOCfg.h"
#include "DxPower.h"
#include "Utility.h"
#include "DxFlash.h"
#include "DxCommon.h"
#include "DxApi.h"
#include "Dx.h"
#include "rtc.h"
#include "wdt.h"
#include "HwPower.h"
#include "HwClock.h"
#include "clock.h"

#define THIS_DBGLVL         2 // 0=FATAL, 1=ERR, 2=WRN, 3=UNIT, 4=FUNC, 5=IND, 6=MSG, 7=VALUE, 8=USER
///////////////////////////////////////////////////////////////////////////////
#define __MODULE__          DxPwr
#define __DBGLVL__          THIS_DBGLVL
#define __DBGFLT__          "*" //*=All, [mark]=CustomClass
#include "DebugModule.h"


#define TEMPDET_FUNCTION DISABLE
#define TEMPDET_TEST     DISABLE
#define ADC_TEST         ENABLE
#define DUMMY_LOAD                   0
#define BATT_SLIDE_WINDOW_COUNT      8

#define VOLDET_BATTERY_420V         782
#define VOLDET_BATTERY_415V         772
#define VOLDET_BATTERY_410V         763
#define VOLDET_BATTERY_405V         754
#define VOLDET_BATTERY_400V         744
#define VOLDET_BATTERY_390V         726
#define VOLDET_BATTERY_380V         707
#define VOLDET_BATTERY_378V         703
#define VOLDET_BATTERY_377V         701
#define VOLDET_BATTERY_376V         699
#define VOLDET_BATTERY_375V         698
#define VOLDET_BATTERY_374V         696
#define VOLDET_BATTERY_373V         694
#define VOLDET_BATTERY_372V         692
#define VOLDET_BATTERY_371V         690
#define VOLDET_BATTERY_370V         688
#define VOLDET_BATTERY_369V         687
#define VOLDET_BATTERY_367V         683
#define VOLDET_BATTERY_366V         681
#define VOLDET_BATTERY_360V         670
#define VOLDET_BATTERY_355V         660
#define VOLDET_BATTERY_350V         651
#define VOLDET_BATTERY_340V         633
#define VOLDET_BATTERY_004V         7
#define CHARGE_ADC_OFFSET           25
#define LENS_ADC_OFFSET             23
#define LENS_ADC_OFFSET2            12



DummyLoadType dummyLoadData[11];

//***********************************************
//*
//*    Battery Rule depend on Model
//*
//***********************************************

//#define VOLDET_BATTERY_ADC_TH       0

#if TEMPDET_TEST
INT32 temperature_value = 0;
#endif


static INT32  gTempValue = 25;
#if ADC_TEST
UINT32 gAdcValue = 3000;
#endif

//------------------ Battery Status Level -------------------//
#define  BATT_LEVEL_COUNT  4

static UINT32  LiBattAdcLevelValue[BATT_LEVEL_COUNT] = {
	VOLDET_BATTERY_355V,
	VOLDET_BATTERY_371V,
	VOLDET_BATTERY_376V,
	VOLDET_BATTERY_380V,
};



#define  DUMMY_LOAD_OFFSETV          VOLDET_BATTERY_004V
#define  LENS_MOVE_MAX_COUNT         10

static UINT32 *pBattAdcLevelValue = &LiBattAdcLevelValue[0];;
static UINT32  uiBattADCSlideWin[BATT_SLIDE_WINDOW_COUNT] = {0};
static UINT8   uiBattSlideIdx = 0;
static UINT8   uiCurSlideWinCnt = 0;
static INT32   iBattAdcCalOffset = 0;
#if USB_CHARGE_FUNCTION
static UINT32  u32BattChargeCurrent = BATTERY_CHARGE_CURRENT_LOW;
#endif
#if DUMMY_LOAD
static void    DrvPower_dummy_process(UINT32 *V1, UINT32 *V2, BOOL bIris);
#endif
static INT32   DrvPower_GetTempCompentation(INT32 tempValue);

static UINT32  bDummyLoadPwrOff = FALSE;

//static DX_CALLBACK_PTR   g_fpDxPowerCB = NULL;
/**
  Get battery voltage ADC value

  Get battery voltage ADC value

  @param void
  @return UINT32 ADC value
*/


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
#if USB_CHARGE_FUNCTION
static INT32  DrvPower_BattTempGet(void);
#endif
static void DrvPower_PowerOnInit(void);
DX_OBJECT gDevPowerBATT = {
	DXFLAG_SIGN,
	DX_CLASS_POWER_EXT,
	POWER_VER,
	"Power",
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




static UINT32 DrvPower_GetBatteryADC(void)
{

	UINT32 uiADCValue;
	INT32  TempCompentationADC;

#if ADC_TEST
	uiADCValue = gAdcValue;
#else
	uiADCValue = (adc_readData(ADC_CH_VOLDET_BATTERY));
#endif
	{
		TempCompentationADC = DrvPower_GetTempCompentation(gTempValue);
		DBG_MSG("Temp= %d\r\n", gTempValue);
#if 0 //charlie need wait DrvFlash_IsCharge finish
		if (DrvFlash_IsCharge()) {
			//DBG_MSG("ADC = %d,",uiADCValue);
			uiADCValue += (CHARGE_ADC_OFFSET + TempCompentationADC);
			DBG_MSG("Charge ADC+= %d\r\n", CHARGE_ADC_OFFSET + TempCompentationADC);
		} else {
			if (gDevPower.pfCallback) {

				gDevPower.pfCallback(DRVPWR_CB_IS_LENS_MOVING, 0, 0);
				DBG_MSG("ADC = %d,", uiADCValue);
				uiADCValue += (LENS_ADC_OFFSET + TempCompentationADC);
				DBG_MSG("lens2 ADC+= %d\r\n", LENS_ADC_OFFSET + TempCompentationADC);



			}
		}
#else
		if (gDevPowerBATT.pfCallback) {

			gDevPowerBATT.pfCallback(DRVPWR_CB_IS_LENS_MOVING, 0, 0);
			DBG_MSG("ADC = %d,", uiADCValue);
			uiADCValue += (LENS_ADC_OFFSET + TempCompentationADC);
			DBG_MSG("lens2 ADC+= %d\r\n", LENS_ADC_OFFSET + TempCompentationADC);



		}
#endif
		uiADCValue += (iBattAdcCalOffset / 2);
		return uiADCValue;
	}

}
//#NT#2009/09/02#Lincy Lin -begin
//#Add function for check battery overheat
/**
  Get battery

  Get battery Temperature ADC value

  @param void
  @return UINT32 ADC value
*/
static BOOL DrvPower_IsBattOverHeat(void)
{
	return FALSE;
}
//#NT#2009/09/02#Lincy Lin -end




/**
  Get battery voltage level

  Get battery voltage level.
  If battery voltage level is DRVPWR_BATTERY_LVL_EMPTY, it means
  that you have to power off the system.

  @param void
  @return UINT32 Battery Level, refer to VoltageDet.h -> VOLDET_BATTERY_LVL_XXXX
*/

static UINT32 DrvPower_GetBatteryLevel(void)
{
	static UINT32   uiPreBatteryLvl    = DRVPWR_BATTERY_LVL_UNKNOWN;
	//static UINT32   uiPreBatteryADC    = 0;
	static UINT32   uiRetBatteryLvl;
	static UINT32   uiEmptycount = 0;
	static UINT32   uiCount = 0;
	UINT32          uiCurBatteryADC, uiCurBatteryLvl, i;
	BOOL            isMatch = FALSE;

	uiCurBatteryLvl = 0;
	if (uiPreBatteryLvl == DRVPWR_BATTERY_LVL_UNKNOWN) {
		uiCurBatteryADC = DrvPower_GetBatteryADC();
		//uiPreBatteryADC = DrvPower_GetBatteryADC() - 1;
		for (i = 0; i < BATT_SLIDE_WINDOW_COUNT; i++) {
			uiBattADCSlideWin[i] = uiCurBatteryADC;
			DBG_MSG("AVG=%d\r\n", uiCurBatteryADC);
		}
	} else {

		uiCurSlideWinCnt = BATT_SLIDE_WINDOW_COUNT;
		uiBattADCSlideWin[uiBattSlideIdx++] = DrvPower_GetBatteryADC();
		if (uiBattSlideIdx >= uiCurSlideWinCnt) {
			uiBattSlideIdx = 0;
		}
		uiCurBatteryADC = 0;
		for (i = 0; i < uiCurSlideWinCnt; i++) {
			uiCurBatteryADC += uiBattADCSlideWin[i];
			DBG_MSG("A[%d]=%d,", i, uiBattADCSlideWin[i]);
		}
		uiCurBatteryADC /= uiCurSlideWinCnt;
		DBG_MSG("AVG=%d", uiCurBatteryADC);
		DBG_MSG(", V=%d", uiCurBatteryADC * 42 / 9100);
		DBG_MSG(".%02d\r\n", (uiCurBatteryADC * 42 / 91) % 100);
	}

	//DBG_IND("%d,%d,%d\r\n",VOLDET_BATTERY_ADC_LVL0,VOLDET_BATTERY_ADC_LVL1,VOLDET_BATTERY_ADC_LVL2);
	for (i = BATT_LEVEL_COUNT; i > 0; i--) {
		if (uiCurBatteryADC > pBattAdcLevelValue[i - 1]) {
			uiCurBatteryLvl = i;
			isMatch = TRUE;
			break;
		}
	}
	if (isMatch != TRUE) {
		uiCurBatteryLvl = 0;
	}

	// Debounce
	if ((uiCurBatteryLvl == uiPreBatteryLvl) ||
		(uiPreBatteryLvl == DRVPWR_BATTERY_LVL_UNKNOWN)) {
		uiRetBatteryLvl = uiCurBatteryLvl;
	}
	uiPreBatteryLvl = uiCurBatteryLvl;
	//uiPreBatteryADC = uiCurBatteryADC;

	if (uiCount % 2 == 0) {
		uiRetBatteryLvl = uiPreBatteryLvl;
	}
	uiCount++;
	//
	if (uiEmptycount || uiRetBatteryLvl == DRVPWR_BATTERY_LVL_0) {
		uiEmptycount++;
		if (uiEmptycount == 4) {
			return DRVPWR_BATTERY_LVL_EMPTY;
		}
	}

	return uiRetBatteryLvl;
}


static void DrvPower_PowerOnInit(void)
{
	pBattAdcLevelValue = &LiBattAdcLevelValue[0];
#if USB_CHARGE_FUNCTION
	gTempValue = DrvPower_BattTempGet();
#endif
}

UINT32 DrvPower_DummyLoad(void)
{
#if DUMMY_LOAD
	UINT32 Ave_V, deltaV, V1, V2;

	DrvPower_dummy_process(&V1, &V2, FALSE);
	deltaV = V2 - V1;
	Ave_V = V1;
	dummyLoadData[0].deltaV = deltaV;
	dummyLoadData[0].Ave_V = Ave_V;


	if (dummyLoadData[0].Ave_V < (LiBattAdcLevelValue[0] + DUMMY_LOAD_OFFSETV)) {
		bDummyLoadPwrOff = TRUE;

	} else {
		bDummyLoadPwrOff = FALSE;
	}
	DBG_IND("Ave_V=%d ; deltaV=%d , bDummyLoadPwrOff=%d\r\n", Ave_V, deltaV, bDummyLoadPwrOff);
	return (!bDummyLoadPwrOff);
#else
	return TRUE;
#endif
}
#if DUMMY_LOAD
static void DrvPower_dummy_process(UINT32 *V1, UINT32 *V2, BOOL bIris)
{
	UINT32 i, Va;
	UINT32 Vtotal, DetectLoop = 25;
	float  Vtmp1, Vtmp2, Vtmp3;

	if (gDevPower.pfCallback) {
		gpio_clearPin(GPIO_LENS_MD_RST); // Enable motor driver.GPIO_LENS_MD_RST
		Delay_DelayMs(5);
		gpio_setPin(GPIO_LENS_MD_RST); // Enable motor driver.GPIO_LENS_MD_RST
		gDevPower.pfCallback(DRVPWR_CB_DUMMY_LOAD_START, 0, 0);
	}
	Delay_DelayMs(40);

	Vtotal = 0;
	for (i = 0; i < DetectLoop ; i++) { //sample 25 times.
		Va = DrvPower_GetBatteryADC();  //sample average value.(In loading)
		Delay_DelayUs(200);
		Vtotal += Va;
	}
	*V1 = Vtotal / DetectLoop;
	if (gDevPower.pfCallback) {
		gDevPower.pfCallback(DRVPWR_CB_DUMMY_LOAD_END, 0, 0);
	}
	Delay_DelayMs(5);
	Vtotal = 0;
	for (i = 0; i < DetectLoop ; i++) { //sample 25 times.
		Va = DrvPower_GetBatteryADC();  //sample average value.(after loading)
		Delay_DelayUs(200);
		Vtotal += Va;
	}

	*V2 = Vtotal / DetectLoop;


	Vtmp1 = (float)(*V1) * 3.3 * 1.39 / 4096;
	Vtmp2 = (float)(*V2) * 3.3 * 1.39 / 4096;


	Vtmp3 = Vtmp2 - Vtmp1;

	DBG_IND("delta=(%1.3f, %d) Ave_V:%1.3f V2:%1.3f, %d, %d, Cal = %d \r\n", Vtmp3, (*V2) - (*V1), Vtmp1, Vtmp2, *V1, *V2, iBattAdcCalOffset);

}
#endif

static void DrvPower_BattADC_Calibration(BOOL enable)
{
}

#if USB_CHARGE_FUNCTION
static void DrvPower_EnableChargeIC(BOOL bCharge)
{

}

static void DrvPower_ChargeBattEn(BOOL bCharge)
{

}


static void DrvPower_ChargeCurrentSet(UINT32 Current)
{
	u32BattChargeCurrent = Current;
}

static UINT32 DrvPower_ChargeCurrentGet(void)
{
	return u32BattChargeCurrent;
}

static void DrvPower_ChargeISet(BOOL isHigh)
{

}

static BOOL DrvPower_ChargeIGet(void)
{
	return 0;
}

static void DrvPower_ChargeVSet(BOOL isHigh)
{

}

static BOOL DrvPower_ChargeVGet(void)
{
	return 0;
}


static BOOL DrvPower_IsUnderCharge(void)
{
	return 0;
}

static BOOL DrvPower_IsUSBChargeOK(void)
{
	return 0;
}

static BOOL DrvPower_IsBattIn(void)
{
	return TRUE;

}

static BOOL DrvPower_IsDeadBatt(void)
{
	return FALSE;
}

static BOOL DrvPower_IsNeedRecharge(void)
{
	return FALSE;
}

static INT32 DrvPower_BattTempGet(void)
{
	return 25;
}
#endif

static INT32 DrvPower_GetTempCompentation(INT32 tempValue)
{
	return 0;
}

static UINT32 DrvPower_GetPowerOnSrc(void)
{
	UINT32 ret = 0;

	ret = rtc_getPWROnSource();

	return ret;
}

#define USE_HWTIMER     DISABLE //ENABLE

static void __add_1s(struct tm *ct)
{
	if (ct->tm_sec == 59) {
		if (ct->tm_min == 59) {
			ct->tm_hour += 1;
			ct->tm_min = 0;
			ct->tm_sec = 0;
		} else {
			ct->tm_min += 1;
			ct->tm_sec = 0;
		}
	} else {
		ct->tm_sec += 1;
	}
}


static void DrvPower_PowerOff_ClearCounter(void)
{
	rtc_resetShutdownTimer();
}

static BOOL gLoopQuit = FALSE;
extern void _DrvPowerBatt_Quit(void);
void _DrvPowerBatt_Quit(void)
{
	gLoopQuit = TRUE;
}

static void DrvPower_PowerOff(void)
{
	DBG_DUMP("(pwr-off)\r\n");

#if (_FPGA_EMULATION_ != ENABLE)
	Delay_DelayUs(800);
#ifdef  __S3_POWER_DOWN
	clk_powerdown(CLK_PDN_MODE_POWEROFF);
#else
	rtc_poweroffPWR(); //this func will turn-off system power
#endif

	// Loop forever
	while (!gLoopQuit) { //fix for CID 43219
		;
	}
#else
	// Loop forever
	while (1) {
		;
	}
#endif
}

//-----------------------------------------------------------------------------
// Power Reset (Power Alarm)
//-----------------------------------------------------------------------------
static void DrvPower_PowerReset_Enable(BOOL bEnable)
{
	if (bEnable != 0) {
		rtc_enablePWRAlarm();
		DBG_DUMP("+(pwr-alarm)\r\n");
	} else {
		DBG_DUMP("-(pwr-alarm)\r\n");
		rtc_disablePWRAlarm();
	}
}

static void DrvPower_PowerReset_ClearCounter(void)
{
	DBG_ERR("Not Support!");
}

static void DrvPower_PowerReset(void)
{
	struct tm CurrDT;
	DrvPower_PowerReset_Enable(FALSE);

	CurrDT = HwClock_GetTime(TIME_ID_CURRENT);
	__add_1s(&CurrDT);
	__add_1s(&CurrDT);
	__add_1s(&CurrDT);
	HwClock_SetTime(TIME_ID_HWRT, CurrDT, 0);

	DrvPower_PowerReset_Enable(TRUE);

	DrvPower_PowerOff();
}

//-----------------------------------------------------------------------------
// Soft Reset (Watch Dog)
//-----------------------------------------------------------------------------

//#Fine tune the watch dog timer value
#define WDT_TIMER_INTERVAL         160
#define WDT_TIMEOUT                350     //350 ms

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
	case POWER_ID_PSW1:
		return rtc_getPWRStatus();
		break;
	case POWER_ID_PSW2:
		return rtc_getPWR2Status();
		break;
	case POWER_ID_PSW3:
		return rtc_getPWR3Status();
		break;
	case POWER_ID_PSW4:
		return rtc_getPWR4Status();
		break;
	case POWER_ID_HWRT:
		return rtc_isPWRAlarmEnabled();
		break;
	case POWER_ID_SWRT:
		return 0; //keep 1 if soft reset
		break;
	default:
		DBG_ERR("unknown pwr id (%d)\r\n", pwrID);
		break;
	}
	return 0;
}


static void DrvPower_SetPowerKey(UINT32 pwrID, UINT32 value)
{
	switch (pwrID) {
	case POWER_ID_PSW1:
		if (value == 0xf0) { //keep power on - clear power off timer
			DrvPower_PowerOff_ClearCounter();
		}
		if (value == 0xff) { //power off NOW
			DrvPower_PowerOff();
		}
		break;
	case POWER_ID_HWRT:
		if (value == 0) { //disable h/w reset timer
			DrvPower_PowerReset_Enable(FALSE);
		}
		if (value == 1) { //enable h/w reset timer
			DrvPower_PowerReset_Enable(TRUE);
		}
		if (value == 0xf0) { //keep power on - clear h/w reset timer
			DrvPower_PowerReset_ClearCounter();
		}
		if (value == 0xff) { //h/w reset NOW
			DrvPower_PowerReset();
		}
		break;
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
		DrvPower_BattADC_Calibration((BOOL)value);
		break;

	case DRVPWR_CTRL_BATTERY_ADC_CAL_OFFSET:
		iBattAdcCalOffset = (INT32)value;
		break;

#if USB_CHARGE_FUNCTION
	case DRVPWR_CTRL_BATTERY_CHARGE_EN:
		DrvPower_ChargeBattEn((BOOL)value);
		break;
	case DRVPWR_CTRL_BATTERY_CHARGE_CURRENT:
		DrvPower_ChargeCurrentSet(value);
		break;

	case DRVPWR_CTRL_BATTERY_CHARGE_ISET:
		DrvPower_ChargeISet((BOOL)value);
		break;

	case DRVPWR_CTRL_BATTERY_CHARGE_VSET:
		DrvPower_ChargeVSet((BOOL)value);
		break;

	case DRVPWR_CTRL_ENABLE_CHARGEIC:
		DrvPower_EnableChargeIC((BOOL)value);
		break;

#endif
	case DRVPWR_CTRL_SWRT_COUNTER:
		DrvPower_SoftReset_SetCounter(value);
		break;
	case DRVPWR_CTRL_PSW1:
		DrvPower_SetPowerKey(POWER_ID_PSW1, value);
		break;
	case DRVPWR_CTRL_HWRT:
		DrvPower_SetPowerKey(POWER_ID_HWRT, value);
		break;
	case DRVPWR_CTRL_SWRT:
		DrvPower_SetPowerKey(POWER_ID_SWRT, value);
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
		rvalue = DrvPower_GetBatteryLevel();
		break;
	case DRVPWR_CTRL_BATTERY_ADC_VALUE:
		rvalue = DrvPower_GetBatteryADC();
		break;
	case DRVPWR_CTRL_BATTERY_ADC_CAL_OFFSET:
		rvalue = iBattAdcCalOffset;
		break;
	case DRVPWR_CTRL_IS_DUMMUYLOAD_POWEROFF:
		rvalue = bDummyLoadPwrOff;
		break;
	case DRVPWR_CTRL_IS_BATT_OVERHEAT:
		rvalue = (UINT32)DrvPower_IsBattOverHeat();
		break;
#if USB_CHARGE_FUNCTION
	case DRVPWR_CTRL_IS_BATT_INSERT:
		rvalue = (UINT32)DrvPower_IsBattIn();
		break;
	case DRVPWR_CTRL_IS_DEAD_BATT:
		rvalue = (UINT32)DrvPower_IsDeadBatt();
		break;
	case DRVPWR_CTRL_IS_NEED_RECHARGE:
		rvalue = (UINT32)DrvPower_IsNeedRecharge();
		break;
	case DRVPWR_CTRL_BATTERY_CHARGE_EN:
		rvalue = (UINT32)DrvPower_IsUnderCharge();
		break;
	case DRVPWR_CTRL_BATTERY_CHARGE_OK:
		rvalue = (UINT32)DrvPower_IsUSBChargeOK();
		break;
	case DRVPWR_CTRL_BATTERY_CHARGE_CURRENT:
		rvalue = DrvPower_ChargeCurrentGet();
		break;
	case DRVPWR_CTRL_BATTERY_CHARGE_ISET:
		rvalue = (UINT32)DrvPower_ChargeIGet();
		break;
	case DRVPWR_CTRL_BATTERY_CHARGE_VSET:
		rvalue = (UINT32)DrvPower_ChargeVGet();
		break;
	case DRVPWR_CTRL_BATTERY_TEMPERATURE:
#if USB_CHARGE_FUNCTION
		rvalue = DrvPower_BattTempGet();
#endif
		break;

#else
	case DRVPWR_CTRL_BATTERY_CHARGE_EN:
		rvalue = FALSE;
		break;
#endif
	case DRVPWR_CTRL_POWERON_SOURCE:
		rvalue = DrvPower_GetPowerOnSrc();
		break;
	case DRVPWR_CTRL_POWER_LOST:
		rvalue = rtc_isPowerLost();
		break;
	case DRVPWR_CTRL_PSW1:
		rvalue = DrvPower_GetPowerKey(POWER_ID_PSW1);
		break;
	case DRVPWR_CTRL_PSW2:
		rvalue = DrvPower_GetPowerKey(POWER_ID_PSW2);
		break;
	case DRVPWR_CTRL_PSW3:
		rvalue = DrvPower_GetPowerKey(POWER_ID_PSW3);
		break;
	case DRVPWR_CTRL_PSW4:
		rvalue = DrvPower_GetPowerKey(POWER_ID_PSW4);
		break;
	case DRVPWR_CTRL_HWRT:
		rvalue = DrvPower_GetPowerKey(POWER_ID_HWRT);
		break;
	case DRVPWR_CTRL_SWRT:
		rvalue = DrvPower_GetPowerKey(POWER_ID_SWRT);
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

	return DX_OK;
}

static UINT32 DrvPowercfgs(UINT32 CfgID, UINT32 Param1)
{

	return DX_OK;
}
static UINT32 DrvPowerInit(void *pInitParam)
{
	DBG_IND("Init\r\n");
	DrvPower_PowerOnInit();
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




