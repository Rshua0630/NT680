#include "SysKer.h"
#include "GxSystem.h"
#include "SysCfg.h"
#include "stdlib.h"
#include "string.h"
#include "FileSysTsk.h"
#include "DrvExt.h"
#include "SysMain.h"
#include "DbgUtApi.h"
#include "DxSys.h"
#include "NvtUser.h"
#include "NvtBack.h"
#include "rtc.h"
#include "clock.h"
//#NT#2016/07/21#KCHong -begin
//#NT#Add GSensor I2C bus power on reset
#include "gpio.h"
//#NT#2016/07/21#KCHong -end

///////////////////////////////////////////////////////////////////////////////
#define __MODULE__          main
#define __DBGLVL__          2 // 0=FATAL, 1=ERR, 2=WRN, 3=UNIT, 4=FUNC, 5=IND, 6=MSG, 7=VALUE, 8=USER
#define __DBGFLT__          "*" //*=All, [mark]=CustomClass
#include "DebugModule.h"
///////////////////////////////////////////////////////////////////////////////

#if (_INSTRUMENT_FUNCTION_ == 1)
#include <stdio.h>
#include "Debug.h"
#include "Platform.h"

//Reference: http://blog.linux.org.tw/~jserv/archives/001870.html

extern UINT32 user_SecLoad[10];


void _STOPTRACE __cyg_profile_func_enter(void *this_func, void *call_site)
{
	debug_msg(">>> func = %08x, call = %08x ", this_func, call_site);

	//check partial-load
	{
		UINT32 addr = (UINT32)this_func;
		int i;
		for (i = 0; i < 10; i++) {
			//check inside section id
			if ((addr >= OS_GetMemAddr(i)) && (addr < OS_GetMemAddr(i) + OS_GetMemSize(i))) {
				if (user_SecLoad[i] == 1) {
					debug_msg("secID=%d\r\n", user_SecLoad[i]);
				} else {
					debug_msg("secID=%d ^RNOT LOADED YET!\r\n", user_SecLoad[i]);
				}
				continue;
			}
		}
	}

	debug_msg("\r\n");
}
void _STOPTRACE __cyg_profile_func_exit(void *this_func, void *call_site)
{
	//debug_msg("<<< func = %08x, call = %08x\r\n", this_func, call_site);
}
#endif

extern void User_IdleCB(void);
extern UINT32 User_GetTempBuffer(UINT32 uiSize);
extern UINT32 User_RelTempBuffer(UINT32 addr);
extern BOOL User_CommandFilter(CHAR *pcCmdStr, int len);
extern void System_BindDeviceModule(void);

/**
    main

    Main routine of system. This will be the first C-routine
*/
//#NT#2016/03/03#Ben Wang -begin
//#NT#Add debug message for script test.
BOOL g_bDbgTest = FALSE;
//#NT#2016/03/03#Ben Wang -end

FILEOP_TAB fileOpTab = {
	FileSys_OpenFile,
	FileSys_CloseFile,
	FileSys_WriteFile,
	(FST_OPEN_WRITE | FST_CREATE_ALWAYS)
};

void System_BindSysModule(void);

//#NT#2016/07/21#KCHong -begin
//#NT#Add GSensor I2C bus power on reset
#if (GSENSOR_FUNCTION == ENABLE)
void  Nvti2c_DevPowerOnReset(UINT32 GpioI2cCLK, UINT32 GpioI2cDAT)
{
	UINT32  UnitTimeUs = 6;// H/L unit time use 6us
	UINT32  i;

	gpio_setDir(GpioI2cCLK, GPIO_DIR_INPUT);
	gpio_setDir(GpioI2cDAT, GPIO_DIR_INPUT);

	if (gpio_getPin(GpioI2cCLK)) {
		if (gpio_getPin(GpioI2cDAT)) {
			// Both the CLK&DAT are high.
			// We issue START & STOP to reset ext device
			gpio_setDir(GpioI2cDAT, GPIO_DIR_OUTPUT);
			gpio_clearPin(GpioI2cDAT);
			Delay_DelayUs(UnitTimeUs << 1);
			gpio_setDir(GpioI2cDAT, GPIO_DIR_INPUT);
		} else {
			// CLK is High But Data is Low.
			// We Toggle clock pin and wait device release DATA pin.
			// And then issue START & STOP to reset ext device
			for (i = 0; i < 8; i++) {
				gpio_setDir(GpioI2cCLK, GPIO_DIR_OUTPUT);
				gpio_clearPin(GpioI2cCLK);
				Delay_DelayUs(UnitTimeUs);
				gpio_setDir(GpioI2cCLK, GPIO_DIR_INPUT);
				Delay_DelayUs(UnitTimeUs);

				if (gpio_getPin(GpioI2cDAT)) {
					break;
				}
			}

			if (gpio_getPin(GpioI2cDAT)) {
				gpio_setDir(GpioI2cDAT, GPIO_DIR_OUTPUT);
				gpio_clearPin(GpioI2cDAT);
				Delay_DelayUs(UnitTimeUs << 1);
				gpio_setDir(GpioI2cDAT, GPIO_DIR_INPUT);
			} else {
				debug_err(("i2c Dev no relase DAT pin\r\n"));
			}

		}
	} else {
		debug_err(("i2c clk pin is keeping low!\r\n"));
	}

	gpio_setDir(GpioI2cCLK, GPIO_DIR_INPUT);
	gpio_setDir(GpioI2cDAT, GPIO_DIR_INPUT);
}
#endif
//#NT#2016/07/21#KCHong -end



int NvtMain(int argc, char *argv[])
{
	// should init interrupt direction firstly
	Dx_InitIO();

	debug_msg("Hello, World!\r\n");
	TM_BOOT_BEGIN("flow", "service");

	//Init DbgUt to measure usage of CPU/DMA
	{
		DBGUT_INIT DbgUtInit = {0};
		DbgUtInit.uiApiVer = DBGUT_API_VERSION;
		DbgUt_Init(&DbgUtInit);
	}

	//debug_msg("service task - begin!\r\n");
	// Start SxCmd
	SxCmd_RegCmdMode(SX_MODE_CPU1, "CPU1", '>', SxCmd_DoCommand);
	SxCmd_Open();
	//register user command filter
	SxCmd_RegCmdFilterCB(User_CommandFilter);
	//register Project level Memory Provide API for Command Parser Interface
	SxCmd_RegTempMemFunc(User_GetTempBuffer);
    SxCmd_RegRelTempMemFunc(User_RelTempBuffer);
	//register Project level On Idle API for GxSystem
	GxSystem_RegIdleCB(User_IdleCB);
	System_BindSysModule();
#if (POWERON_TRACE == ENABLE)
	//////////////////////////////////////////
	// Wait for some on call GxSystem_PowerOn()
	GxSystem_WaitForPowerOn();
#endif
	debug_reg_fileFunc(&fileOpTab);  //register debug dump opertaion table
	TM_BOOT_END("flow", "service");
	//////////////////////////////////////// DO NOT CHANGE THESE CODE - BEGIN //////////////
#if (GSENSOR_FUNCTION == ENABLE)
	//#NT#2016/07/21#KCHong -begin
	//#NT#Add GSensor I2C bus power on reset
	Nvti2c_DevPowerOnReset(P_GPIO_22, P_GPIO_21);
#endif
	//#NT#2016/07/21#KCHong -end
	//Dx_InitIO();
	//////////////////////////////////////// DO NOT CHANGE THESE CODE - END ////////////////
	// Start system service, hook devices, user task
	///////////////////////////////////////////
	//call user to config Mode

	///////////////////////////////////////////
	//call all devices to config Lib (also config SxJob/SxTimer/SxCmd)
	System_BindDeviceModule(); //install command

	TM_BOOT_BEGIN("flow", "user");
	///////////////////////////////////////////
	// Start SxJob/SxTimer
	SxTimer_Open(); //begin system monitor service

	///////////////////////////////////////////
	// Start User Event System
	{
		UXUSER_OBJ uiUserObj = {0};
		BKG_OBJ uiBackObj = {0};
		//Init UI framework
		uiUserObj.pfMainProc = (USER_MAIN_FP)UserMainProc;
		uiUserObj.pfNvtUserCB = UserErrCb;
		//debug_msg("user task - begin!\r\n");
		Ux_Open(&uiUserObj);
		//Init UI background
		uiBackObj.pDefaultFuncTbl = 0; //gBackgroundExtFuncTable
		BKG_Open(&uiBackObj);
	}
	// Start Dsc System
	//System_PowerOn(SYS_POWERON_NORMAL);
	TM_BOOT_END("flow", "user");

	//////////////////////////////////////////
	// Wait for some on call GxSystem_PowerOff()
	GxSystem_WaitForPowerOff();
	// End user task, device,  system service
	///////////////////////////////////////////
	// End User Event System
	{
		Ux_Close();
		BKG_Close();
	}
	//debug_msg("user task - end!\r\n");
	///////////////////////////////////////////

	// End SxJob/SxTimer
	SxTimer_Close(); //end system monitor service

	//////////////////////////////////////// DO NOT CHANGE THESE CODE - BEGIN //////////////
	Dx_UninitIO();
	//////////////////////////////////////// DO NOT CHANGE THESE CODE - END ////////////////

	// End SxCmd
	//SxCmd_DoCommand("ker dump");
	SxCmd_Close();
	//debug_msg("service task - end!\r\n");

	debug_msg("Byebye, World!\r\n");

	return 1;
}


//===========================================================================
//Config Dummy Interrupt handler

#include "interrupt.h"
static void DummyInt_Detect(void)
{
	INT_ID  intID;

	intID = int_getDummyId();
	if (intID != INT_ID_MAX) {
		DBG_DUMP("Dummy INT_ID_%.2ld\r\n", intID);
	}
}

//===========================================================================
//Config idle handler

extern void sys_profile_process(void);

void User_IdleCB(void)
{
	//do dummy interrupt detect
	DummyInt_Detect();
#ifndef __ECOS
	//do profile process
	sys_profile_process();
#endif
}



