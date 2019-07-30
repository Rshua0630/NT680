////////////////////////////////////////////////////////////////////////////////
#include "SysCommon.h"
////////////////////////////////////////////////////////////////////////////////
#include "AppLib.h"
#include "UIMode.h"
#include "UIModeSleep.h"
#include "SysCommon.h"
//UIControl
#include "UIFrameworkExt.h"
#include "UIInfo.h"

#include "UISetup.h"

#include "Dx.h"
#include "DxApi.h"
#include "DxStorage.h"
#if (FWS_FUNC == ENABLE)
#include "FwSrvApi.h"
#endif
#if (UCTRL_FUNC)
#include "NvtUctrlAPI.h"
#include "SysMain.h"
#endif
#include "rtc.h"
#include "clock.h"
#include "uart.h"
#if defined(_CPU2_LINUX_)
#include "cpu2util_api.h"
#endif
#include "DrvExt.h"
#include "MsdcNvtCb.h"
#include "MemCheck.h"
#include "bin_info.h"
#define THIS_DBGLVL         2 // 0=FATAL, 1=ERR, 2=WRN, 3=UNIT, 4=FUNC, 5=IND, 6=MSG, 7=VALUE, 8=USER
///////////////////////////////////////////////////////////////////////////////
#define __MODULE__          UIModeSleep
#define __DBGLVL__          ((THIS_DBGLVL>=PRJ_DBG_LVL)?THIS_DBGLVL:PRJ_DBG_LVL)
#define __DBGFLT__          "*" //*=All, [mark]=CustomClass
#include "DebugModule.h"
///////////////////////////////////////////////////////////////////////////////

#define PRI_MODE_SLEEP 11
#define STKSIZE_MODE_SLEEP 2048
#define FLGMODESLEEP_LINUX_SUSPEND  FLGPTN_BIT(0)

int PRIMARY_MODE_SLEEP = -1;      ///< Sleep


static DX_HANDLE hDxVdo1;
#if (DISPLAY2_FUNC == ENABLE)
static DX_HANDLE hDxVdo2;
#endif
#if (MSDCVENDOR_NVT == ENABLE)
static GXUSB_HDL_ID m_ConnectedID = GXUSB_HDL_ID_INVALID;
#endif

//Test of FWxxxxT.bin test flow => T_BIN = 1
#define T_BIN	0
#define TEMP_RELOAD_BUF	1

#if T_BIN
static CHAR    FWFileName[] = "A:\\FW96680T.bin";
#include "FileSysTsk.h"
#define RTC_ROBUST 1

#if RTC_ROBUST
static void emu_rtcIncreaseTime(RTC_TIME *pTime, UINT32 uiSecond)
{
    UINT32 uiMinute, uiHour;
    UINT32 uiTimeH, uiTimeM, uiTimeS;

    uiHour      = uiSecond / 3600;
    uiSecond   -= uiHour * 3600;
    uiMinute    = uiSecond / 60;
    uiSecond   -= uiMinute * 60;

    // second, minute, hour bit number isn't enough for calculating
    // (might overflow). Need 32-bits value for calculating.
    uiTimeS     = pTime->s.second;
    uiTimeM     = pTime->s.minute;
    uiTimeH     = pTime->s.hour;

    uiTimeS    += uiSecond;
    while (uiTimeS > 59)
    {
        uiTimeS -= 60;
        uiTimeM++;
    }

    uiTimeM    += uiMinute;
    while (uiTimeM > 59)
    {
        uiTimeM -= 60;
       uiTimeH++;
    }

    uiTimeH    += uiHour;
    while (uiTimeH > 23)
    {
        uiTimeH -= 24;
    }

    pTime->s.second = uiTimeS;
    pTime->s.minute = uiTimeM;
    pTime->s.hour   = uiTimeH;
}
#endif
#endif

#ifdef  __S3_POWER_DOWN
UINT32 MODE_SLEEP_TSK_ID = 0;
UINT32 FLG_ID_MODE_SLEEP = 0;
static void ModeSleepTsk(void);

void ModeSleep_InstallID(void)
{
	OS_CONFIG_TASK(MODE_SLEEP_TSK_ID, PRI_MODE_SLEEP, STKSIZE_MODE_SLEEP, ModeSleepTsk);
	OS_CONFIG_FLAG(FLG_ID_MODE_SLEEP);
}

void ModeSleepTsk(void)
{
	kent_tsk();
#if T_BIN
	FST_FILE    filehdl;
	int 		Status;
	FWSRV_BUF_DESC In = {0}; //temp buffer
	In.uiAddr = OS_GetMempoolAddr(POOL_ID_APP);
	In.uiSize = OS_GetMempoolSize(POOL_ID_APP);
//		DBG_DUMP("^Raddr = 0x%08x / size = 0x%08x\r\n", In.uiAddr, In.uiSize);
//		HEADINFO *p_hdr = (HEADINFO *)(In.uiAddr + BIN_INFO_OFFSET_UITRON);
	FileSys_WaitFinish();
	filehdl = FileSys_OpenFile((CHAR *)FWFileName, FST_OPEN_READ);
	Status = FileSys_ReadFile(filehdl, (UINT8 *)In.uiAddr, (UINT32 *)&In.uiSize, 0, NULL);
	FileSys_CloseFile(filehdl);
	if (Status == 0 ) {
		DBG_DUMP("^GRead FW addr 0x%08x / size  0x%08x\r\n", In.uiAddr,In.uiSize);
	} else {
		DBG_DUMP("^RRead loader binary fail\r\n");
		return;
	}
//		DBG_DUMP("^G Bin location = 0x%08x\r\n", p_hdr->CodeEntry);
	clk_set_reload_fw_addr(In.uiAddr, In.uiSize);
#else
	FWSRV_CMD Cmd = {0};
	FWSRV_BUF_DESC In = {0}; //temp buffer
	FWSRV_BUF_DESC Out = {0}; //uncompressed uitron buffer
	In.uiAddr = OS_GetMempoolAddr(POOL_ID_APP);
	In.uiSize = OS_GetMempoolSize(POOL_ID_APP);
	Cmd.Idx = FWSRV_CMD_IDX_DUMP_UITRON;
	Cmd.In.pData = &In;
	Cmd.In.uiNumByte = sizeof(In);
	Cmd.Out.pData = &Out;
	Cmd.Out.uiNumByte = sizeof(Out);
	Cmd.Prop.bExitCmdFinish = TRUE;
	if (FwSrv_Cmd(&Cmd) != FWSRV_ER_OK) {
		DBG_ERR("failed to enter sleep mode.\r\n");
		return;
	}
	clk_set_reload_fw_addr(Out.uiAddr, Out.uiSize);
	//Steven Temprary use
#if TEMP_RELOAD_BUF
	MEM_RANGE reg_rld_buffer = {0};
	reg_rld_buffer.Addr = (In.uiAddr +  Out.uiSize + 3) & 0xFFFFFFFC;
	reg_rld_buffer.Size = In.uiSize - (reg_rld_buffer.Addr - In.uiAddr);
//DBG_DUMP("^GModeSleep_Open regAddr[0x%08x] regSize[0x%08x]\r\n", reg_rld_buffer.uiAddr, reg_rld_buffer.uiSize);
	clk_set_reload_apb_register(reg_rld_buffer.Addr, &reg_rld_buffer.Size);
	// saving apb buf info
	MODELEXT_HEADER *header = NULL;
	BININFO *pBinInfo = (BININFO *)Dx_GetModelExtCfg(MODELEXT_TYPE_BIN_INFO, &header);
	if (!pBinInfo || !header) {
		DBG_FATAL("bininfo is null\n");
		return;
	}
	pBinInfo->comm.Resv[COMM_RESV_IDX_S3_APB_ADDR] = reg_rld_buffer.Addr;
	pBinInfo->comm.Resv[COMM_RESV_IDX_S3_APB_LEN] = reg_rld_buffer.Size;
#endif
#endif
	set_flg(FLG_ID_MODE_SLEEP, FLGMODESLEEP_LINUX_SUSPEND);

	ext_tsk();
}
#endif

static void ModeSleep_Open(void)
{
#if defined(_CLOCK_RTC_) //sleep mode only support to rtc
#ifdef  __S3_POWER_DOWN
	CLK_PDN_MODE pdn_mode = CLK_PDN_MODE_SYSOFF_DRAMON;
#else
	CLK_PDN_MODE pdn_mode = CLK_PDN_MODE_SLEEP3;
#endif
	Ux_SetActiveApp(&UISetupObjCtrl);
	Ux_SendEvent(0, NVTEVT_EXE_OPEN, 0);

#if (defined(_CPU2_ECOS_) && ECOS_POWER_OFF_FLOW == DISABLE)
	DBG_ERR("ECOS_POWER_OFF_FLOW must set ENABLE\r\n");
#endif

	SxTimer_SetFuncActive(SX_TIMER_DET_TIMER_ID, FALSE);
	SxTimer_SetFuncActive(SX_TIMER_DET_TV_ID, FALSE);
	SxTimer_SetFuncActive(SX_TIMER_DET_STRG_ID, FALSE);
#if(USB_MODE==ENABLE)
	SxTimer_SetFuncActive(SX_TIMER_DET_USB_ID, FALSE);
#endif
	SxTimer_SetFuncActive(SX_TIMER_DET_SLEEP_ID, FALSE);
	SxTimer_SetFuncActive(SX_TIMER_DET_AUTOPOWEROFF_ID, FALSE);
	SxTimer_SetFuncActive(SX_TIMER_DET_KEY_ID, FALSE);
	SxTimer_SetFuncActive(SX_TIMER_DET_PWR_ID, FALSE);
#if (FWS_FUNC == ENABLE)
	if (!clk_pre_powerdown(pdn_mode)) {
		DBG_ERR("Failed to pre_powerdown\r\n");
		return;
	}
#ifdef  __S3_POWER_DOWN
	if (pdn_mode == CLK_PDN_MODE_SYSOFF_DRAMON) {
		sta_tsk(MODE_SLEEP_TSK_ID, 0);

		//FLGPTN uiFlag;
		//wai_flg(&uiFlag, FLG_ID_MODE_SLEEP, FLGMODESLEEP_LINUX_SUSPEND, TWF_ORW | TWF_CLR);
	}
#endif
#else
#ifdef  __S3_POWER_DOWN
	if (pdn_mode == CLK_PDN_MODE_SYSOFF_DRAMON) {
		DBG_ERR("CLK_PDN_MODE_SYSOFF_DRAMON needs FWS_FUNC=ENABLE\r\n");
		return;
	}
#endif //__S3_POWER_DOWN
#endif
	//STORAGE
	System_OnStrgExit_PS();
#ifdef  __S3_POWER_DOWN
	//no need to close filesys, it ok after resume for CLK_PDN_MODE_SLEEP3 but CLK_PDN_MODE_SYSOFF_DRAMON needs
	if (pdn_mode == CLK_PDN_MODE_SYSOFF_DRAMON) {
		GxStrg_CloseDevice(0);
	}
#endif
	//LCD
	GxVideo_SetDeviceCtrl(DOUT1, DISPLAY_DEVCTRL_BACKLIGHT, FALSE);
	hDxVdo1 = (DX_HANDLE)GxVideo_GetDevice(DOUT1);
	if (hDxVdo1) {
		GxVideo_CloseDevice(DOUT1);
	}

#if (DISPLAY2_FUNC == ENABLE)
	hDxVdo2 = (DX_HANDLE)GxVideo_GetDevice(DOUT2);
	if (hDxVdo2) {
		GxVideo_CloseDevice(DOUT2);
	}
#endif

#if defined(_CPU2_LINUX_)
	cpu2util_suspend();
#endif
	//leave adc
	//Dx_UninitIO();

#if (MSDCVENDOR_NVT == ENABLE)
	if ((m_ConnectedID = MsdcNvtCb_GetConnectedID()) != GXUSB_HDL_ID_INVALID) {
		MsdcNvtCb_Close();
	}
#endif

#if 1 //set it as 0 for debug
	rtc_setConfig(RTC_CONFIG_ID_WAKEUP_ENABLE, RTC_WAKEUP_SRC_PWR_SW|RTC_WAKEUP_SRC_ALARM);
	//enter to sleep
#if T_BIN
#if RTC_ROBUST
	{
		struct tm Curr_DateTime;
		struct tm* pAlarmTime = 0;
		time_t c_dt = -1;
		Curr_DateTime= HwClock_GetTime(TIME_ID_CURRENT);
	    DBG_DUMP("Curr Time %ld/%ld/%ld ",Curr_DateTime.tm_year,Curr_DateTime.tm_mon,Curr_DateTime.tm_mday);
	    DBG_DUMP("%ld:%ld:%ld\r\n",Curr_DateTime.tm_hour,Curr_DateTime.tm_min,Curr_DateTime.tm_sec);

		Curr_DateTime.tm_year -= 1900;
		Curr_DateTime.tm_mon -= 1;
		c_dt = mktime(&Curr_DateTime);
		c_dt += 25;

		pAlarmTime = localtime(&c_dt);
		if(pAlarmTime)
		{
			pAlarmTime->tm_year+=1900;
			pAlarmTime->tm_mon+=1;
			Curr_DateTime = *pAlarmTime;
		}
		DBG_DUMP("Alarm Time %ld/%ld/%ld ",Curr_DateTime.tm_year,Curr_DateTime.tm_mon,Curr_DateTime.tm_mday);
	    DBG_DUMP("%ld:%ld:%ld\r\n",Curr_DateTime.tm_hour,Curr_DateTime.tm_min,Curr_DateTime.tm_sec);
		HwClock_SetTime(TIME_ID_HWRT, Curr_DateTime, 0);
		HwPower_SetPowerKey(POWER_ID_HWRT, TRUE);
        rtc_waitCSETDone();
	}
#endif
#endif

#ifdef  __S3_POWER_DOWN
	if (pdn_mode == CLK_PDN_MODE_SYSOFF_DRAMON) {
		FLGPTN uiFlag;
		wai_flg(&uiFlag, FLG_ID_MODE_SLEEP, FLGMODESLEEP_LINUX_SUSPEND, TWF_ORW | TWF_CLR);
	}
#endif

#if (FWS_FUNC == ENABLE)
	FwSrv_Close(); //must closing it after FLGMODESLEEP_LINUX_SUSPEND, bacause ModeSleepTsk use it
#endif
#if(UCTRL_FUNC == ENABLE)
	//make sure clear share memory bUitronRdy bit,for linux resume check bUitronRdy correctly
	NvtUctrl_Close();
#endif

	clk_powerdown(pdn_mode);
	//leave sleeping
	//enter adc
	//Dx_InitIO();

	UINT32 iCurrMode = System_GetState(SYS_STATE_CURRMODE);
	UINT32 iNextMode = System_GetState(SYS_STATE_PREVMODE);
	UI_SetData(FL_PreMode, iCurrMode);
	UI_SetData(FL_NextMode, iNextMode);
	Ux_PostEvent(NVTEVT_SYSTEM_MODE, 1, iNextMode);
#endif
#else
	DBG_ERR("sleep mode only support to CLOCK = CLOCK_RTC.\r\n");
#endif
}

static void ModeSleep_Close(void)
{
#if(UCTRL_FUNC == ENABLE)
	System_OnUctrl();
#endif
	Ux_SendEvent(0, NVTEVT_EXE_CLOSE, 0);

#if (MSDCVENDOR_NVT == ENABLE)
	if (m_ConnectedID != GXUSB_HDL_ID_INVALID) {
		extern void USBMakerInit_UMSD(USB_MSDC_INFO* );
		MSDCNVTCB_OPEN Open = {0};
		Open.GxUsbHdlID = m_ConnectedID;
		Open.fpUSBMakerInit = USBMakerInit_UMSD;
		Delay_DelayMs(1000);
		MsdcNvtCb_Open(&Open);
	}
#endif

#if defined(_CPU2_LINUX_)
	cpu2util_resume();
#endif

	//LCD
	if (hDxVdo1) {
		GxVideo_OpenDevice(DOUT1, (UINT32)hDxVdo1, DISP_LASTMODE);
	}

#if (DISPLAY2_FUNC == ENABLE)
	if (hDxVdo2) {
		GxVideo_OpenDevice(DOUT2, (UINT32)hDxVdo2, DISP_LASTMODE);
	}
#endif

	GxVideo_SetDeviceCtrl(DOUT1, DISPLAY_DEVCTRL_BACKLIGHT, TRUE);

	//STORAGE
	if (System_OnStrgInit_EMBMEM_GetGxStrgType() == FST_FS_TYPE_UITRON) {
		DX_HANDLE pStrgDev = Dx_GetObject(DX_CLASS_STORAGE_EXT | DX_TYPE_CARD1);
		if (GxStrg_OpenDevice(0, pStrgDev) != TRUE) {
			char *pDxName = "unknown";
			Dx_GetInfo(pStrgDev, DX_INFO_NAME, &pDxName);
			DBG_ERR("Storage mount %s fail\r\n", pDxName);
		}
	}
	System_OnStrgInit_PS();
#if (FWS_FUNC == ENABLE)
	FwSrv_Open();
#endif

	SxTimer_SetFuncActive(SX_TIMER_DET_TIMER_ID, TRUE);
	SxTimer_SetFuncActive(SX_TIMER_DET_TV_ID, TRUE);
	SxTimer_SetFuncActive(SX_TIMER_DET_STRG_ID, TRUE);
#if(USB_MODE==ENABLE)
	SxTimer_SetFuncActive(SX_TIMER_DET_USB_ID, TRUE);
#endif
	SxTimer_SetFuncActive(SX_TIMER_DET_SLEEP_ID, TRUE);
	SxTimer_SetFuncActive(SX_TIMER_DET_AUTOPOWEROFF_ID, TRUE);
	SxTimer_SetFuncActive(SX_TIMER_DET_KEY_ID, TRUE);
	SxTimer_SetFuncActive(SX_TIMER_DET_PWR_ID, TRUE);
}

SYS_MODE gModeSleep = {
	"SLEEP",
	ModeSleep_Open,
	ModeSleep_Close,
	NULL,
	NULL,
	NULL,
	NULL
};
