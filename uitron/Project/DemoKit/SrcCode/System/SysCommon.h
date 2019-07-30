/*
    Copyright   Novatek Microelectronics Corp. 2005.  All rights reserved.

    @file       SysCommon.h
    @ingroup    mIPRJAPUIFlow

    @brief      UIDef Functions
                This file is the user interface ( for LIB callback function).

    @note       Nothing.

    @date       2005/04/01
*/

#ifndef _SYSCOMMON_H
#define _SYSCOMMON_H

//Kernel
#include "Type.h"
#include "SysKer.h"
#include "Debug.h"
//Lib
#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <Memory.h>
//#include "PStore.h"
#include "GxLib.h"
#include "FileSysTsk.h" //to replace GxFile
#include "UMSDC.h"
//NvtSystem
#include "UIFramework.h"

#include "SysSensor.h"
//Config
#include "SysCfg.h"
#include "PrjCfg.h"

//C (Events)
#include "NVTToolCommand.h"
#include "NVTUserCommand.h"


/////////////////////////////////////////

#include "SysMain.h"

extern int SX_TIMER_DET_KEY_ID;
extern int SX_TIMER_DET_USB_ID;
extern int SX_TIMER_DET_PWR_ID;
extern int SX_TIMER_DET_MODE_ID;
extern int SX_TIMER_DET_TIMER_ID;
extern int SX_TIMER_DET_LED_ID;
extern int SX_TIMER_DET_STRG_ID;
extern int SX_TIMER_DET_TV_ID;
extern int SX_TIMER_DET_LCDREG_ID;
extern int SX_TIMER_DET_BATT_ID;
extern int SX_TIMER_DET_SLEEP_ID;
extern int SX_TIMER_DET_AUTOPOWEROFF_ID;
extern int SX_TIMER_DET_AUDIODEV_ID;
extern int SX_TIMER_DET_FLASH_ID;
extern int SX_TIMER_AUTO_INPUT_ID;
extern int SX_TIMER_BURNIN_AUTORUN_ID;
extern int SX_TIMER_DET_RECHARGE_ID;
extern int SX_TIMER_DET_SYSTEM_BUSY_ID;
extern int SX_TIMER_DET_CLOSE_LENS_ID;
extern int SX_TIMER_DET_EV_ID;
extern int SX_TIMER_DET_SHOWADC;
extern int SX_TIMER_DET_MEM;
extern int SX_TIMER_NET_CONNECT_ID;

#define AUTO_INPUT_TIMER_CNT    2

//audio
extern BOOL System_GetAudioOut(void);
extern BOOL System_GetAudioMode(void);
extern BOOL System_GetAudioInput(void);
extern UINT32 System_GetAudioVol(void);
extern INT32 System_PutAudioData(UINT32 did, UINT32 addr, UINT32 *ptrsize);
//#NT#2016/09/08#HM Tseng -begin
//#NT#Support audio channel
extern UINT32 System_GetAudioChannel(void);
//#NT#2016/09/08#HM Tseng -end



//custom CB
extern void USB_CB(UINT32 event, UINT32 param1, UINT32 param2);
extern void USB_PlugInSetFunc(void);
extern void PST_CB(UINT32 event, UINT32 param1, UINT32 param2);
extern void Timer_CB(UINT32 event, UINT32 param1, UINT32 param2);
extern void Sound_CB(UINT32 event, UINT32 param1, UINT32 param2);
extern void LED_CB(UINT32 event, UINT32 param1, UINT32 param2);
extern void Lens_CB(UINT32 event, UINT32 param1, UINT32 param2);
extern void IPL_CB(UINT32 event, UINT32 param1, UINT32 param2);

extern void Load_IPLInfo(void);
extern void Load_ResInfo(void);
extern void Load_DispLogo(void);
extern void Load_LangString(void);

extern BOOL gIsUSBChargePreCheck;

extern int PRIMARY_MODE_MAIN;       ///< Main
extern int PRIMARY_MODE_PLAYBACK;   ///< Playback
extern int PRIMARY_MODE_PHOTO;      ///< Photo preview
extern int PRIMARY_MODE_MOVIE;      ///< Movie preview
extern int PRIMARY_MODE_USBMSDC;    ///< USB MSDC
extern int PRIMARY_MODE_USBSIDC;    ///< USB SIDC/PTP
extern int PRIMARY_MODE_USBPCC;     ///< USB PC camera
extern int PRIMARY_MODE_USBYOUTUBE; ///< USB YouTube
extern int PRIMARY_MODE_USBCHARGE;  ///< USB CHARGE
extern int PRIMARY_MODE_USBMENU;    ///< USB MENU
extern int PRIMARY_MODE_SLEEP;      ///< Sleep
extern int PRIMARY_MODE_IPCAM;      ///< IPCam


//custom Job
void System_DetBusy(void);
void System_DetCloseLens(void) ;
void System_ResetDetCloseLenCount(void);
void System_DetBackLight(void);
void System_DetShowADC(void);

//custom key sound setting
#define FLGKEY_SOUND_MASK_DEFAULT     (FLGKEY_KEY_MASK &~(FLGKEY_SHUTTER2 |FLGKEY_SHUTTER1 | FLGKEY_ZOOMIN | FLGKEY_ZOOMOUT | FLGKEY_MOVIE))

//utility function
#define SYS_POWEROFF_BATT_EMPTY            1
#define SYS_POWEROFF_LENS_ERROR            2

//USB charge function
#define USB_PWRON_NONE             0
#define USB_PWRON_ADPIN_BATIN      1
#define USB_PWRON_ADPIN_BATOUT     2
#define USB_PWRON_ADPOUT_BATIN     3
#define USB_PWRON_ADPOUT_BATOUT    4

#define USB_SRC_NONE            0
#define USB_SRC_USB_ADAPTER     1
#define USB_SRC_USB_PC          2
#define USB_SRC_USB_CHARGING_PC 3
#define USB_SRC_UNKNOWN         0xffffffff

#define POWER_SRC_BATTERY       0
#define POWER_SRC_USB_ADAPTER   1
#define POWER_SRC_USB_PC        2
#define POWER_SRC_UNKNOWN       0xffffffff

//USB src detect
//#NT#2018/01/12#Ben Wang -begin
//#NT#Support dual USB device detection
extern void USB_UpdateSource(GXUSB_HDL_ID HandleID);
//#NT#2018/01/12#Ben Wang -end
extern UINT32 USB_GetSource(void);

//USB MsdcNvt Mode
extern void USB_SetForceMsdcNvt(BOOL bEn);
extern BOOL USB_GetIsMsdcNvt(void);
extern MSDC_OBJ *USB_GetMsdcObj(GXUSB_HDL_ID id);

//CARD insert detect
extern void Storage_UpdateSource(void);
extern void Storage_PowerOn_Start(void);
extern void Storage_PowerOn_End(void);


#define UPDNAND_STS_FW_OK               0   // Update FW to NAND OK
#define UPDNAND_STS_FW_INVALID_STG      1   // Invalid source storage
#define UPDNAND_STS_FW_READ_ERR         2   // FW doesn't exist or read error
#define UPDNAND_STS_FW_READ_CHK_ERR     3   // Read FW checksum failed, might be error
#define UPDNAND_STS_FW_WRITE_ERR        4   // Write FW to NAND error
#define UPDNAND_STS_FW_READ2_ERR        5   // Read FW from NAND failed (for write checking)
#define UPDNAND_STS_FW_WRITE_CHK_ERR    6   // Write FW checksum failed
#define UPDNAND_STS_INVLID_MODE         7   // Must be main mode

extern UINT32   System_OnStrg_UploadFW(UINT32 DevID);
//#NT#2018/03/14#Niven Cho -begin
//#NT#FORMAT_ROOTFS
extern UINT32 System_OnStrg_Format_RootFs(void);
//#NT#2018/03/14#Niven Cho -end



//USB=>Battery charge flow
extern void Power_StartUSBCharge(void);
extern void Power_StopUSBCharge(void);

//SysInput
extern void Input_ResetMask(void);


//

//custom flow
//void Display_ChangeDevice(UINT32 dev_id);

//#NT#2010/10/21#Lincy Lin -end
extern void AutoInput(void);
//#NT#2013/01/29#Lincy Lin -begin
//#NT#
typedef enum _SPLASH_ID {
	SPLASH_POWERON = 0,                 ///< power on logo
	SPLASH_POWEROFF,                    ///< power off logo
	ENUM_DUMMY4WORD(SPLASH_ID)
} SPLASH_ID;
extern void Display_ShowSplash(SPLASH_ID splash_id);
extern void Display_ShowPreview(void);
//#NT#2013/01/29#Lincy Lin -end

//#NT#2011/02/14#Steven feng -begin
typedef struct _SSWAV_Info_ {
	CHAR  uiPS_SectionName[5];
	char *ui_pFilePathName;
} SSWAV_Info;
//#NT#2011/02/14#Steven feng -end


typedef struct _Language_Info_ {
	UINT32  uiPS_SectionName;
	char *ui_pFilePathName;
} Language_Info;

#if(_LANG_STORE_ == _PSTORE_)
extern Language_Info g_LanguageInfo[LANGUAGE_SETTING_MAX];
extern UINT32 g_pLanguageSize_[LANGUAGE_SETTING_MAX];
#endif
extern BOOL g_bIsInitSystemFinish;


void IPL_PowerOn_Load(void);
void IPL_PowerOn_OpenSensor(void);
void IPL_PowerOn_Apply(void);
void IPL_PowerOn_ChangeMode(void);
void IPL_PowerOff_CloseSensor(void);

//#NT#2011/02/10#Lily Kao -begin
extern void Sound_SetRepeat(UINT32 isTrue);
//#NT#2011/02/10#Lily Kao -end

//#NT#2011/02/18#Lily Kao -begin
extern UINT32 Sound_GetRepeat(void);
//#NT#2011/02/18#Lily Kao -end

extern void File_InitDCFID(void);
extern void File_ResetDCFID(void);
extern void File_LoadDCFID(void);
extern void File_SaveDCFID(void);
extern void File_IncPictCnt(void);

extern UINT32 Command_GetTempBuffer(UINT32 uiSize);

extern UINT32 System_GetConnectDisplay(void);
extern UINT32 System_GetEnableDisp(void);
extern UINT32 System_GetEnableDispCount(void);

extern UINT32 System_GetConnectSensor(void);
extern UINT32 System_GetEnableSensor(void);
extern UINT32 System_GetEnableSensorCount(void);


extern void SysVideo_OnVideoInsert(void);
extern void SysVideo_OnVideoRemove(void);
extern UINT32 SysVideo_GetDir(void);
extern UINT32 SysVideo_GetDirbyID(UINT32 DevID);
extern void   VdoOut_SetSkipAllocTemp(UINT32 is_alloc);

#endif//_SYSCOMMON_H
