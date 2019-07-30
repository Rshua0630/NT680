#ifndef _SYS_MODE
#define _SYS_MODE

#include "Type.h"
#include "NvtUser.h"


extern UINT32 _SECTION(".kercfg_data") USERINIT_FLG_ID;
extern UINT32 _SECTION(".kercfg_data") USEREXIT_FLG_ID;
extern UINT32 _SECTION(".kercfg_data") USERPROC2_ID;
extern UINT32 _SECTION(".kercfg_data") USERPROC3_ID;
extern UINT32 _SECTION(".kercfg_data") USERPROC4_ID;
extern UINT32 _SECTION(".kercfg_data") USERPROC5_ID;
extern UINT32 _SECTION(".kercfg_data") USERPROC6_ID;
extern UINT32 _SECTION(".kercfg_data") USERPROC7_ID;
extern UINT32 _SECTION(".kercfg_data") USERPROC8_ID;

extern void UserMainProc_InstallID(void) _SECTION(".kercfg_text");

extern void SystemInit(void);
extern void SystemInit2(void);
extern void SystemInit3(void);
extern void SystemInit4(void);
extern void SystemInit5(void);
extern void SystemInit6(void);
extern void SystemInit7(void);
extern void SystemInit8(void);

extern void SystemExit(void);
extern void SystemExit2(void);
extern void SystemExit3(void);
extern void SystemExit4(void);
extern void SystemExit5(void);
extern void SystemExit6(void);
extern void SystemExit7(void);
extern void SystemExit8(void);




#if 1
//--------------------------------------------------------------------------------------
//  Flow: Power On/Off
//--------------------------------------------------------------------------------------
//
//FLG_ID_INIT (flag for control init tasks)
//
#define FLGINIT_MASKALL         0xFFFFFFFF
#define FLGINIT_BEGIN           0x10000000
#define FLGINIT_BEGIN2          0x20000000
#define FLGINIT_BEGIN3          0x40000000
#define FLGINIT_BEGIN4          0x80000000
#define FLGINIT_BEGIN5          0x01000000
#define FLGINIT_BEGIN6          0x02000000
#define FLGINIT_BEGIN7          0x04000000
#define FLGINIT_BEGIN8          0x08000000
#define FLGINIT_END             0x00100000
#define FLGINIT_END2            0x00200000
#define FLGINIT_END3            0x00400000
#define FLGINIT_END4            0x00800000
#define FLGINIT_END5            0x00010000
#define FLGINIT_END6            0x00020000
#define FLGINIT_END7            0x00040000
#define FLGINIT_END8            0x00080000
#define FLGINIT_STRGATH         0x00000200
#define FLGINIT_MODE            0x00000100
#define FLGINIT_MOUNTPS         0x00000010
#define FLGINIT_LOADPS          0x00000020
#define FLGINIT_MOUNTFS         0x00000040
#define FLGINIT_LOADFS          0x00000080
#define FLGINIT_MOUNTFWS        0x00000001
#define FLGINIT_LOADCODE1       0x00000002
#define FLGINIT_LOADCODE2       0x00000004
#define FLGINIT_LOADCODE3       0x00000008
#define INIT_SETFLAG(flag) \
	{set_flg(USERINIT_FLG_ID, (flag));}
#define INIT_CLRFLAG(flag) \
	{clr_flg(USERINIT_FLG_ID, (flag));}
#define INIT_WAITFLAG(flag) \
	{FLGPTN uiInitFlag;  wai_flg(&uiInitFlag, USERINIT_FLG_ID, (flag), TWF_ORW);}
#define INIT_CHKFLAG(flag) kchk_flg(USERINIT_FLG_ID, (flag))


//
//FLG_ID_EXIT (flag for control init tasks)
//
#define FLGEXIT_MASKALL         0xFFFFFFFF
#define FLGEXIT_BEGIN           0x10000000
#define FLGEXIT_BEGIN2          0x20000000
#define FLGEXIT_BEGIN3          0x40000000
#define FLGEXIT_BEGIN4          0x80000000
#define FLGEXIT_BEGIN5          0x01000000
#define FLGEXIT_BEGIN6          0x02000000
#define FLGEXIT_BEGIN7          0x04000000
#define FLGEXIT_BEGIN8          0x08000000
#define FLGEXIT_END             0x00100000
#define FLGEXIT_END2            0x00200000
#define FLGEXIT_END3            0x00400000
#define FLGEXIT_END4            0x00800000
#define FLGEXIT_END5            0x00010000
#define FLGEXIT_END6            0x00020000
#define FLGEXIT_END7            0x00040000
#define FLGEXIT_END8            0x00080000
#define EXIT_SETFLAG(flag) \
	{set_flg(USEREXIT_FLG_ID, (flag));}
#define EXIT_CLRFLAG(flag) \
	{clr_flg(USEREXIT_FLG_ID, (flag));}
#define EXIT_WAITFLAG(flag) \
	{FLGPTN uiInitFlag;  wai_flg(&uiInitFlag, USEREXIT_FLG_ID, (flag), TWF_ORW);}

#define SYSTEM_POWERON_NORMAL       0x00
#define SYSTEM_POWERON_LOWPOWER     0x01
#define SYSTEM_POWERON_SAFE         0x02
#define SYSTEM_POWERON_CHARGE       0x03

#define SYSTEM_POWEROFF_NORMAL      0x00
#define SYSTEM_POWEROFF_LOWPOWER    0x01
#define SYSTEM_POWEROFF_SAFE        0x02
#define SYSTEM_POWEROFF_CHARGE      0x03

#define SYSTEM_PARTIAL_LOAD_DISABLE  0X00
#define SYSTEM_PARTIAL_LOAD_ENABLE   0X01

#endif

//////////////////////////////////////////////////////

typedef struct _SYS_MODE {
	char *strName;
	void (*pfMode_OnOpen)(void);
	void (*pfMode_OnClose)(void);
	void (*pfMode_OnSleepEnter)(void);
	void (*pfMode_OnSleepLeave)(void);
	void (*pfMode_OnPauseDMA)(void);
	void (*pfMode_OnResumeDMA)(void);
}
SYS_MODE;

typedef struct _SYS_SUBMODE {
	char *strName;
	void (*pfSubMode_OnOpen)(void);
	void (*pfSubMode_OnClose)(void);
}
SYS_SUBMODE;


INT32 System_AddMode(SYS_MODE *pMode);
INT32 System_AddSubMode(SYS_SUBMODE *pMode);
SYS_SUBMODE *System_GetSubModeCtrl(void);
void System_ChangeSubMode(INT32 mode_id);

//////////////////////////////////////////////////////

#define SYS_STATE_POWERON       0   //default = SYS_POWERON_NORMAL
#define SYS_STATE_POWEROFF      1   //default = SYS_POWEROFF_NORMAL
#define SYS_STATE_CURRMODE      2
#define SYS_STATE_PREVMODE      3
#define SYS_STATE_NEXTMODE      4
#define SYS_STATE_ACTIVE        5
#define SYS_STATE_SLEEPLEVEL    6
#define SYS_STATE_CURRSUBMODE   7
#define SYS_STATE_PREVSUBMODE   8
#define SYS_STATE_NEXTSUBMODE   9
#define SYS_STATE_FS            10 // FileSys Status
#define SYS_STATE_CARD          11 // Card Status
#define SYS_STATE_MAX           12

//SYS_STATE_MODE
#define SYS_MODE_UNKNOWN          ((INT32)-1)

//SYS_STATE_POWERON
#define SYS_POWERON_NORMAL      0x00
#define SYS_POWERON_LOWPOWER    0x01
#define SYS_POWERON_SAFE        0x02
#define SYS_POWERON_CHARGE      0x03

//SYS_STATE_POWEROFF
#define SYS_POWEROFF_NORMAL     0x00
#define SYS_POWEROFF_LOWPOWER   0x01
#define SYS_POWEROFF_SAFE       0x02
#define SYS_POWEROFF_CHARGE     0x03
#define SYS_POWEROFF_BATTEMPTY  0x04
#define SYS_POWEROFF_LENSERROR  0x05

#define SYS_SUBMODE_NORMAL      0

//SYS_STATE_FS
typedef enum {
	CARD_REMOVED = 0,
	CARD_INSERTED,
	CARD_LOCKED
} CARD_STATUS;

//SYS_STATE_CARD
typedef enum {
	FS_DISK_ERROR = 0,
	FS_UNKNOWN_FORMAT,
	FS_UNFORMATTED,
	FS_NOT_INIT,
	FS_INIT_OK,
	FS_NUM_FULL
} FS_STATUS;

extern void System_PowerOn(UINT32 pwrOnType);
extern void System_PowerOff(UINT32 pwrOffType);
extern void System_ChangeSleep(UINT32 sleep_lv);
extern void System_ChangeMode(UINT32 mode_id);

extern void System_ModeOpen(UINT32 mode_id);
extern void System_ModeClose(void);
extern void System_ModeSleepEnter(void);
extern void System_ModeSleepLeave(void);

extern INT32 System_GetState(INT32 state);
extern void System_SetState(INT32 state, INT32 value);

extern void System_ChangeCPUSpeed(int speed_id);
extern void System_ChangeDMASpeed(int speed_id);

//#NT#2016/03/18#Niven Cho -begin
//#NT#improve the using of gprof
extern UINT32 System_GetGprofBuffer(UINT32 uiSize);
//#NT#2016/03/18#Niven Cho -end

extern void UserMainProc(void);
extern void UserErrCb(UXUSER_CB_TYPE evt, UINT32 p1, UINT32 p2, UINT32 p3);
extern void UserCalConsumeFps(UINT32 bEn,UINT32 unit);



//////////////////////////////////////////////////////

extern INT32 System_GetBootFirstMode(void);
extern void System_OnUsbPreInit(void);
extern void System_OnPowerPreInit(void);

extern void System_OnStrg_DownloadFW(UINT32 *SecOrderTable, void (*LoadCallback)(const UINT32 Idx));

extern void System_CPU2_Start(void);
extern void System_CPU2_Stop(void);
extern void System_CPU2_WaitReady(void);

extern void System_DSP_Start(void);
extern void System_DSP_WaitReady(void);

extern void System_OnTimerInit(void);
extern void System_OnIPC(void);
extern void System_OnUctrl(void);
extern void System_OnStrgInit_EMBMEM(void);
extern UINT32 System_OnStrgInit_EMBMEM_GetGxStrgType(void);
extern void System_OnStrgInit_EXMEM(void);
extern void System_OnStrgInit_FWS(void);
extern void System_OnStrgInit_FS(void);
extern void System_OnStrgInit_FS2(void);
extern void System_OnStrgInit_FS3(void);
extern void System_OnStrgInit_PS(void);
extern void System_OnFlashInit_PS(void);
extern void System_OnVideoInit(void);
extern void System_OnVideoInit2(void);
extern void System_OnVideoInit3(void);
extern void System_OnAudioInit(void);
extern void System_OnUsbInit(void);
extern void System_OnLensInit(void);
extern void System_OnSensorInit(void);
extern void System_OnSensorInit2(void);
extern void System_OnSensorInit3(void);
extern void System_OnPowerInit(void);
extern void System_OnInputInit(void);
extern void System_OnOutputInit(void);
extern void System_OnBTInit(void);

extern void System_OnTimerExit(void);
extern void System_OnStrgExit_EMBMEM(void);
extern void System_OnStrgExit_EXMEM(void);
extern void System_OnStrgExit_FWS(void);
extern void System_OnStrgExit_FS(void);
extern void System_OnStrgExit_PS(void);
extern void System_OnVideoExit(void);
extern void System_OnAudioExit(void);
extern void System_OnUsbExit(void);
extern void System_OnLensExit(void);
extern void System_OnSensorExit(void);
extern void System_OnPowerExit(void);
extern void System_OnInputExit(void);
extern void System_OnOutputExit(void);
extern void System_OnBTExit(void);
extern void System_PS_Format(void);
extern void System_WaitFS(void);

#endif
