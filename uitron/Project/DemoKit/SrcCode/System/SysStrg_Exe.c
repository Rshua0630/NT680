/*
    System Storage Callback

    System Callback for Storage Module.

    @file       SysStrg_Exe.c
    @ingroup    mIPRJSYS

    @note       這個檔案負責一件事

                1.接收Storage Event的介面
                  可能的event有:

                     STRG_INSERT --- 代表有CARD插入
                       執行unmount之前需要做的事情
                       呼叫File unmound (NAND),
                       執行mount之前需要做的事情
                       呼叫File mount (CARD)

                     STRG_REMOVE --- 代表有CARD拔出
                       執行unmount之前需要做的事情
                       呼叫File unmount (CARD)
                       執行mount之前需要做的事情
                       呼叫File_mount (NAND)

                     STRG_ATTACH --- 代表File mount完畢
                       執行mount之後需要做的事情
                       這邊會傳入mount的結果status

                     STRG_DETACH --- 代表File unmount完畢
                       執行unmount之後需要做的事情
                       這邊會傳入unmount的結果status


    Copyright   Novatek Microelectronics Corp. 2010.  All rights reserved.
*/

////////////////////////////////////////////////////////////////////////////////
#include "SysCommon.h"
#include "AppCommon.h"
////////////////////////////////////////////////////////////////////////////////
#include "UIFrameworkExt.h"
#include "AppLib.h"
#include "dma.h"
#include "DxType.h"
#include "Dx.h"
#include "DxApi.h"
#include "DxWiFi.h"

#include "DxStorage.h"
#include "PStore.h"
#if defined(_CPU2_LINUX_) && defined(_EMBMEM_EMMC_)
#include "Fastboot_PStore.h"
#endif
#include "DCF.h"
#include "GxFlash.h"
#include "ExifVendor.h"
#include "GxDSP.h"
#include "bin_info.h"
#include "FileSysTsk.h"
#include "CoreInfo.h"
#include "emb_partition_info.h"
#include "DrvExt.h"
#include "lz.h"
#include "StrgDef.h"
#include "HwClock.h"
#include "UIAppMovie.h"
#include "NvtIpcAPI.h"
#include "PstoreIpcAPI.h"
#if(USB_MODE==ENABLE)
#include "MsdcNvtCb.h"
#endif
#include "DrvExt.h"
#include "dram_partition_info.h"
#include "modelext_parser.h"
#include "nvtpack.h"
#include "cpu2util_api.h"
#include "modelext_info.h"


#if (UPDFW_MODE == ENABLE)
#include "UIModeUpdFw.h"
#endif

#if (LOGFILE_FUNC==ENABLE)
#include "LogFile.h"
#endif
#if (USERLOG_FUNC == ENABLE)
#include "userlog.h"
#endif
#include "wdt.h"

//#NT#2018/01/31#Ben Wang -begin
//#NT#EMMC_AS_PSTORE
#include "UICommon.h"
//#NT#2018/01/31#Ben Wang -end

#if defined(_EMBMEM_SPI_NOR_)
#define MAX_BLK_PER_SEC         128
#if !defined(PSTORE_MAX_SECTION_NUM)
#define PSTORE_MAX_SECTION_NUM  4
#endif
#else
#define MAX_BLK_PER_SEC         512
#if !defined(PSTORE_MAX_SECTION_NUM)
#define PSTORE_MAX_SECTION_NUM  4
#endif
#endif
#define LDC_HEADER_SIZE         16
#define FW_UPD_FW_TMP_MEM_SIZE  0xA00000

#define LOADER_UPD_FW_PATH "A:\\"_BIN_NAME_".BIN"
#define FW_DEL_INDIACTION_PATH "A:\\NVTDELFW"

//#NT#2018/04/02#Niven Cho -begin
//#NT#PARTIAL_COMPRESS, we use last 10MB of APP as working buffer
#define FW_PARTIAL_COMPRESS_WORK_BUF_SIZE 0xA00000
//#NT#2018/04/02#Niven Cho -end

//global debug level: PRJ_DBG_LVL
#include "PrjCfg.h"
//local debug level: THIS_DBGLVL
#define THIS_DBGLVL         2 // 0=FATAL, 1=ERR, 2=WRN, 3=UNIT, 4=FUNC, 5=IND, 6=MSG, 7=VALUE, 8=USER
///////////////////////////////////////////////////////////////////////////////
#define __MODULE__          SysStrgExe
#define __DBGLVL__          ((THIS_DBGLVL>=PRJ_DBG_LVL)?THIS_DBGLVL:PRJ_DBG_LVL)
#define __DBGFLT__          "*" //*=All, [mark]=CustomClass
#include "DebugModule.h"
///////////////////////////////////////////////////////////////////////////////
//Check definition options
#if defined(_CPU2_ECOS_)
#if (LOGFILE_FUNC == ENABLE && ECOS_DBG_MSG_FORCE_UART1_DIRECT == ENABLE)
#error "LOGFILE_FUNC must turn off ECOS_DBG_MSG_FORCE_UART1_DIRECT"
#endif
#endif

//#NT#2016/12/20#Niven Cho#[114852] -begin
//#NT#FIX Linux update F.W failed. (because of filesys' buffer get smaller on linux)
#define UITRON_FS_WORKING_BUF_SIZE (ALIGN_CEIL_32(0xAC000)+ALIGN_CEIL_32(0x80020))
//#NT#2016/12/20#Niven Cho#[114852] -end

static FST_FS_TYPE m_GxStrgType = FST_FS_TYPE_UITRON;
///////////////////////////////////////////////////////////////////////////////
//
//  EMBMEM
//
///////////////////////////////////////////////////////////////////////////////
//Caculate FAT Start Addr

#if defined(_CPU2_ECOS_)
static BOOL xSysStrg_eCosRun(void);
#endif
#if defined(_CPU2_LINUX_)
static BOOL xSysStrg_LinuxRun(void);
#endif
#if (defined(_DSP1_FREERTOS_) || defined(_DSP2_FREERTOS_))
static BOOL xSysStrg_DspRun(DSP_CORE_ID CoreID);
#endif

UINT32 System_OnStrgInit_EMBMEM_GetGxStrgType(void)
{
	return m_GxStrgType;
}

void System_OnStrgInit_EMBMEM(void)
{
	TM_BOOT_BEGIN("nand", "init_embmem");
#if !defined(_EMBMEM_NONE_)
	static BOOL bStrg_init_EMBMEM = FALSE;
	UINT32 i = 0;
	MODELEXT_HEADER *header = NULL;
	EMB_PARTITION *pEmb = (EMB_PARTITION *)Dx_GetModelExtCfg(MODELEXT_TYPE_EMB_PARTITION, &header);

	if (bStrg_init_EMBMEM) {
		return;
	}

	if (!pEmb || !header) {
		DBG_ERR("pEmb\r\n");
		return;
	}

	for (i = 0; i < EMB_PARTITION_INFO_COUNT; i++) {
		BOOL bNeedInit = FALSE;
		DXSTRG_INIT UserEmbMemInit = {0};
		UserEmbMemInit.buf.Addr = OS_GetMempoolAddr(POOL_ID_STORAGE_NAND);
		UserEmbMemInit.buf.Size = OS_GetMempoolSize(POOL_ID_STORAGE_NAND);
		UserEmbMemInit.prt.uiDxClassType = DX_CLASS_STORAGE_EXT;
		UserEmbMemInit.prt.uiPhyAddr = pEmb->PartitionOffset;
		UserEmbMemInit.prt.uiPhySize = pEmb->PartitionSize;
		UserEmbMemInit.prt.uiResvSize = pEmb->ReversedSize;

		switch (pEmb->EmbType) {
		case EMBTYPE_LOADER:
			bNeedInit = TRUE;
			UserEmbMemInit.prt.uiDxClassType |= USER_DX_TYPE_EMBMEM_LOADER;
			DBG_IND("EMBTYPE_LOADER\r\n");
			break;

		case EMBTYPE_UITRON:
			bNeedInit = TRUE;
			UserEmbMemInit.prt.uiDxClassType |= USER_DX_TYPE_EMBMEM_UITRON;
			DBG_IND("EMBTYPE_UITRON\r\n");
			break;

		case EMBTYPE_UBOOT:
			bNeedInit = TRUE;
			UserEmbMemInit.prt.uiDxClassType |= USER_DX_TYPE_EMBMEM_UBOOT;
			DBG_IND("EMBTYPE_UBOOT\r\n");
			break;

		case EMBTYPE_LINUX:
			bNeedInit = TRUE;
			UserEmbMemInit.prt.uiDxClassType |= USER_DX_TYPE_EMBMEM_LINUX;
			DBG_IND("EMBTYPE_LINUX\r\n");
			break;

		case EMBTYPE_DSP:
			bNeedInit = TRUE;
#if defined(_BSP_NA51000_)
			switch (pEmb->OrderIdx) {
			case 0:
				UserEmbMemInit.prt.uiDxClassType |= USER_DX_TYPE_EMBMEM_DSP;
				DBG_IND("EMBTYPE_DSP\r\n");
				break;
			case 1:
				UserEmbMemInit.prt.uiDxClassType |= USER_DX_TYPE_EMBMEM_DSP2;
				DBG_IND("EMBTYPE_DSP2\r\n");
				break;
			default:
				DBG_ERR("unknown dsp index: %d\r\n",pEmb->OrderIdx);
				break;
			}
#endif
#if defined(_BSP_NA51023_)
			UserEmbMemInit.prt.uiDxClassType |= USER_DX_TYPE_EMBMEM_DSP;
			DBG_IND("EMBTYPE_DSP\r\n");
#endif
			break;

#if !defined(_EMBMEM_SPI_NOR_)
		case EMBTYPE_FAT:
		case EMBTYPE_EXFAT:
			bNeedInit = TRUE;
			UserEmbMemInit.prt.uiDxClassType |= USER_DX_TYPE_EMBMEM_FAT;
			DBG_IND("EMBTYPE_FAT\r\n");
			break;
#endif

		case EMBTYPE_PSTORE:
			bNeedInit = TRUE;
			UserEmbMemInit.prt.uiDxClassType |= USER_DX_TYPE_EMBMEM_PSTORE;
			DBG_IND("EMBTYPE_PSTORE\r\n");
			break;
		}

		if (bNeedInit) {
			DX_HANDLE DxNandDev = Dx_GetObject(UserEmbMemInit.prt.uiDxClassType);
			Dx_Init(DxNandDev, &UserEmbMemInit, 0, STORAGE_VER);
		}
		pEmb++;
	}

#if defined(_EMBMEM_SPI_NOR_)
	{
		//if stoarge is SPI, use ram disk as internal FAT
		DXSTRG_INIT UserEmbMemInit = {0};
		UserEmbMemInit.prt.uiDxClassType = DX_CLASS_STORAGE_EXT | USER_DX_TYPE_EMBMEM_FAT;
		UserEmbMemInit.buf.Addr = OS_GetMempoolAddr(POOL_ID_STORAGE_RAMDISK);
		UserEmbMemInit.buf.Size = OS_GetMempoolSize(POOL_ID_STORAGE_RAMDISK);
		DX_HANDLE DxNandDev = Dx_GetObject(UserEmbMemInit.prt.uiDxClassType);
		Dx_Init(DxNandDev, &UserEmbMemInit, 0, STORAGE_VER);
	}
#endif

	bStrg_init_EMBMEM = TRUE;
#endif
	TM_BOOT_END("nand", "init_embmem");
}

void System_OnStrgExit_EMBMEM(void)
{
	//PHASE-1 : Close Drv or DrvExt
}

///////////////////////////////////////////////////////////////////////////////
//
//  EXMEM
//
///////////////////////////////////////////////////////////////////////////////
//Drv or DrvExt
DXSTRG_INIT UserSdioInit;
#if(COPYCARD2CARD_FUNCTION == ENABLE)
DXSTRG_INIT UserSdio2Init;
#endif
DX_HANDLE DxCardDev1 = 0;
typedef enum _BOOT_CARD_STATE {
	BOOT_CARD_STATE_UNKNOWN,
	BOOT_CARD_STATE_INSERTED,
	BOOT_CARD_STATE_REMOVED,
	ENUM_DUMMY4WORD(BOOT_CARD_STATE)
} BOOT_CARD_STATE;
static BOOT_CARD_STATE m_BootState_Drive[16] = {BOOT_CARD_STATE_UNKNOWN}; //DriveA, DriveB
static UINT32 m_FsDxTypeMap[2] = {FS_DX_TYPE_DRIVE_A, FS_DX_TYPE_DRIVE_B};

void System_OnStrgInit_EXMEM(void)
{
	static BOOL bStrg_init_EXMEM = FALSE;
	if (bStrg_init_EXMEM) {
		return;
	}
	TM_BOOT_BEGIN("sdio", "init");

#if (FS_DX_TYPE_DRIVE_A >= DX_TYPE_CARD1 && FS_DX_TYPE_DRIVE_A <= DX_TYPE_CARD3)
	DX_HANDLE DxCardDev1 = Dx_GetObject(DX_CLASS_STORAGE_EXT | FS_DX_TYPE_DRIVE_A);
	UserSdioInit.buf.Addr = OS_GetMempoolAddr(POOL_ID_STORAGE_SDIO);
	UserSdioInit.buf.Size = OS_GetMempoolSize(POOL_ID_STORAGE_SDIO);
	Dx_Init(DxCardDev1, &UserSdioInit, 0, STORAGE_VER);
#endif

#if (FS_MULTI_STRG_FUNC && FS_DX_TYPE_DRIVE_B >= DX_TYPE_CARD1 && FS_DX_TYPE_DRIVE_B <= DX_TYPE_CARD3)
	DX_HANDLE DxCardDev2 = Dx_GetObject(DX_CLASS_STORAGE_EXT | FS_DX_TYPE_DRIVE_B);
	UserSdio2Init.buf.Addr = OS_GetMempoolAddr(POOL_ID_STORAGE_SDIO2);
	UserSdio2Init.buf.Size = OS_GetMempoolSize(POOL_ID_STORAGE_SDIO2);
	Dx_Init(DxCardDev2, &UserSdio2Init, 0, STORAGE_VER);
#endif

	bStrg_init_EXMEM = TRUE;
	TM_BOOT_END("sdio", "init");
}

void System_OnStrgExit_EXMEM(void)
{
	//PHASE-1 : Close Drv or DrvExt

}

///////////////////////////////////////////////////////////////////////////////
//
//  FILESYS
//
///////////////////////////////////////////////////////////////////////////////
//Lib or LibExt
extern void Strg_CB(UINT32 event, UINT32 param1, UINT32 param2);
void Card_DetInsert(void);
void Card_DetBusy(void);
#if (SDINSERT_FUNCTION == ENABLE)
SX_TIMER_ITEM(Card_DetInsert, Card_DetInsert, 2, FALSE)
#endif
SX_TIMER_ITEM(System_DetBusy, Card_DetBusy, 25, FALSE)
int SX_TIMER_DET_STRG_ID = -1;
int SX_TIMER_DET_SYSTEM_BUSY_ID = -1;

void System_OnStrgInit_FS(void)
{
	TM_BOOT_BEGIN("sdio", "init_fs");
	{
		MEM_RANGE Pool;
		Pool.Addr = dma_getCacheAddr(OS_GetMempoolAddr(POOL_ID_FS_BUFFER));
//#NT#2018/02/26#Ben Wang -begin
//#NT#LINUX-EMMC-MULTI-STRG
#if (FS_MULTI_STRG_FUNC == ENABLE)
#if defined(_CPU2_LINUX_) && defined(_EMBMEM_EMMC_)
		MEM_RANGE Pool2, Pool3, Pool4;
		Pool.Size = POOL_SIZE_FS_BUFFER;
		GxStrg_SetConfigEx(0, FILE_CFG_BUF, (UINT32)&Pool);
		Pool2.Addr = Pool.Addr + POOL_SIZE_FS_BUFFER;
		Pool2.Size = POOL_SIZE_FS_BUFFER;
		GxStrg_SetConfigEx(2, FILE_CFG_BUF, (UINT32)&Pool2);
		Pool3.Addr = Pool2.Addr + POOL_SIZE_FS_BUFFER;
		Pool3.Size = POOL_SIZE_FS_BUFFER;
		GxStrg_SetConfigEx(3, FILE_CFG_BUF, (UINT32)&Pool3);
		Pool4.Addr = Pool3.Addr + POOL_SIZE_FS_BUFFER;
		Pool4.Size = POOL_SIZE_FS_BUFFER;
		GxStrg_SetConfigEx(PST_DEV_ID, FILE_CFG_BUF, (UINT32)&Pool4);
#else
		MEM_RANGE Pool2;
		Pool.Size = POOL_SIZE_FS_BUFFER;
		GxStrg_SetConfigEx(0, FILE_CFG_BUF, (UINT32)&Pool);
		Pool2.Addr = Pool.Addr + POOL_SIZE_FS_BUFFER;
		Pool2.Size = POOL_SIZE_FS_BUFFER;
		GxStrg_SetConfigEx(1, FILE_CFG_BUF, (UINT32)&Pool2);
#endif
#else
//#NT#2018/02/26#Ben Wang -end
		Pool.Size = OS_GetMempoolSize(POOL_ID_FS_BUFFER);
		GxStrg_SetConfigEx(0, FILE_CFG_BUF, (UINT32)&Pool);
#endif
	}

	//#NT#2017/06/02#Nestor Yang -begin
	//#NT# Do not link uITRON if not use
	//GxStrg_SetConfigEx(0, FILE_CFG_FS_TYPE, m_GxStrgType);
#if defined(_CPU2_LINUX_)
	GxStrg_SetConfigEx(0, FILE_CFG_FS_TYPE, FileSys_GetOPS_Linux()); //for FILE_CFG_FS_TYPE, DevID is don't care
#else
	GxStrg_SetConfigEx(0, FILE_CFG_FS_TYPE, FileSys_GetOPS_uITRON());
#endif
	//#NT#2017/06/02#Nestor Yang -end

#if (LOGFILE_FUNC==ENABLE)
	GxStrg_SetConfigEx(0, FILE_CFG_MAX_OPEN_FILE, 6);
#endif
#if (USERLOG_FUNC == ENABLE)
	GxStrg_SetConfigEx(0, FILE_CFG_MAX_OPEN_FILE, 6);
#endif
#if (CURL_FUNC == ENABLE)
	GxStrg_SetConfigEx(0, FILE_CFG_MAX_OPEN_FILE, 8);
#endif
#if (MOVIE_MAPPING_MULTIREC == ENABLE)
	GxStrg_SetConfigEx(0, FILE_CFG_MAX_OPEN_FILE, 8);
#endif

	// Enable 32MB alignment recording.
	GxStrg_SetConfigEx(0, FILE_CFG_ALIGNED_SIZE, 32 * 1024 * 1024);

	GxStrg_RegCB(Strg_CB);         //Register CB function of GxStorage (NANR or CARD)
	{
		//1.設定init值
		//FileSys:
		//2.設定CB值,
		//3.註冊SxJob服務 ---------> System Job
		//4.註冊SxTimer服務 ---------> Detect Job
#if (SDINSERT_FUNCTION == ENABLE)
		if (m_GxStrgType == FST_FS_TYPE_UITRON) {
			SX_TIMER_DET_STRG_ID = SxTimer_AddItem(&Timer_Card_DetInsert);
		}
#endif
		//SX_TIMER_DET_SYSTEM_BUSY_ID = SxTimer_AddItem(&Timer_System_DetBusy);
		//5.註冊SxCmd服務 ---------> Cmd Function
		//System_AddSxCmd(Storage_OnCommand); //GxStorage

		//start scan
		SxTimer_SetFuncActive(SX_TIMER_DET_STRG_ID, TRUE);
		SxTimer_SetFuncActive(SX_TIMER_DET_SYSTEM_BUSY_ID, TRUE);
	}
	TM_BOOT_END("sdio", "init_fs");

	if (m_GxStrgType == FST_FS_TYPE_UITRON) {
#if (FS_MULTI_STRG_FUNC)
		UINT32 uiDxState = 0;
		DX_HANDLE pStrgDev = Dx_GetObject(DX_CLASS_STORAGE_EXT | FS_DX_TYPE_DRIVE_B);
		if (Dx_GetState((DX_HANDLE)pStrgDev, STORAGE_STATE_INSERT, &uiDxState) != DX_OK || uiDxState == FALSE) {
			Ux_PostEvent(NVTEVT_STRG_REMOVE, 1, 1);
		} else {
			Ux_PostEvent(NVTEVT_STRG_INSERT, 1, 1);
		}
#endif
	}

	{
		MEM_RANGE WorkBuf;
		WorkBuf.Addr = OS_GetMempoolAddr(POOL_ID_EXIF);
		WorkBuf.Size = POOL_SIZE_EXIF;
		EXIF_Init(EXIF_HDL_ID_1, &WorkBuf, ExifCB);
#if !defined(_SENSOR2_CMOS_OFF_)
		WorkBuf.Addr += POOL_SIZE_EXIF;
		EXIF_Init(EXIF_HDL_ID_2, &WorkBuf, ExifCB);
#endif
#if !defined(_SENSOR3_CMOS_OFF_)
		WorkBuf.Addr += POOL_SIZE_EXIF;
		EXIF_Init(EXIF_HDL_ID_3, &WorkBuf, ExifCB);
#endif
#if !defined(_SENSOR4_CMOS_OFF_)
		WorkBuf.Addr += POOL_SIZE_EXIF;
		EXIF_Init(EXIF_HDL_ID_4, &WorkBuf, ExifCB);
#endif
	}
#if (USE_DCF == ENABLE)
	{
		// init DCF

		CHAR pFolderName[9] = {0};
		CHAR pFileName[5] = {0};
		// init DCF FolderID/FileID with RTC data
		struct tm tm_cur = HwClock_GetTime(TIME_ID_CURRENT);
		snprintf(pFolderName, sizeof(pFolderName), "%1d%02d%02d", tm_cur.tm_year % 0x0A, tm_cur.tm_mon, tm_cur.tm_mday);
		snprintf(pFileName, sizeof(pFileName), "%02d%02d", tm_cur.tm_hour, tm_cur.tm_min);
		//DCF dir-name
		DCF_SetDirFreeChars(pFolderName);
		//DCF file-name
		DCF_SetFileFreeChars(DCF_FILE_TYPE_ANYFORMAT, pFileName);

		//DCF format
		DCF_SetParm(DCF_PRMID_SET_VALID_FILE_FMT, DCF_SUPPORT_FORMAT);
		DCF_SetParm(DCF_PRMID_SET_DEP_FILE_FMT, DCF_FILE_TYPE_JPG | DCF_FILE_TYPE_WAV | DCF_FILE_TYPE_MPO);
		//TODO: [DCF] How to add an new format & its ext?
	}
#endif
}
void System_OnStrgInit_FS2(void)
{
	// update card status again
	if (GxStrg_GetDeviceCtrl(0, CARD_READONLY)) {
		System_SetState(SYS_STATE_CARD, CARD_LOCKED);
	} else if (GxStrg_GetDeviceCtrl(0, CARD_INSERT)) {
		System_SetState(SYS_STATE_CARD, CARD_INSERTED);
	} else {
		System_SetState(SYS_STATE_CARD, CARD_REMOVED);
	}
}
void System_OnStrgInit_FS3(void)
{

}

void System_OnStrgExit_FS(void)
{
	//PHASE-2 : Close Lib or LibExt
#if (LOGFILE_FUNC==ENABLE)
	LogFile_Close();
#endif
	// unmount file system
	GxStrg_CloseDevice(0);
#if (FS_MULTI_STRG_FUNC)
	GxStrg_CloseDevice(1);
#endif
#if defined(_CPU2_LINUX_)
	cpu2util_terminate();
#endif

}

#if (PWR_FUNC == ENABLE)
static BOOL FileSys_DetBusy(void)
{
	return (BOOL)((INT32)FileSys_GetParam(FST_PARM_TASK_STS, 0) == FST_STA_BUSY);
}
#endif

void Card_DetInsert(void)
{
	GxStrg_Det(0, Dx_GetObject(DX_CLASS_STORAGE_EXT | FS_DX_TYPE_DRIVE_A));
#if (0)//FS_MULTI_STRG_FUNC)
	GxStrg_Det(1, Dx_GetObject(DX_CLASS_STORAGE_EXT | FS_DX_TYPE_DRIVE_B));
#endif
}

void Card_DetBusy(void)
{
#if (PWR_FUNC == ENABLE)
	static BOOL bBusyLED = FALSE;

	if (GxPower_GetControl(GXPWR_CTRL_SLEEP_LEVEL) <= 1 && (!GxPower_GetControl(GXPWR_CTRL_BATTERY_CHARGE_EN))) {
		if (FileSys_DetBusy()) {
			if (bBusyLED == FALSE) {
				DBG_IND("System - busy\r\n");
				bBusyLED = TRUE;
#if (OUTPUT_FUNC == ENABLE)
				GxLED_SetCtrl(KEYSCAN_LED_GREEN, SETLED_SPEED, GXLED_1SEC_LED_TOGGLE_CNT / 5);
				GxLED_SetCtrl(KEYSCAN_LED_GREEN, TURNON_LED, FALSE);
				GxLED_SetCtrl(KEYSCAN_LED_GREEN, SET_TOGGLE_LED, TRUE);
#endif
			}
		} else {
			if (bBusyLED == TRUE) {
				DBG_IND("System - not busy\r\n");
				bBusyLED = FALSE;
#if (OUTPUT_FUNC == ENABLE)
				GxLED_SetCtrl(KEYSCAN_LED_GREEN, SET_TOGGLE_LED, FALSE);
				GxLED_SetCtrl(KEYSCAN_LED_GREEN, TURNON_LED, FALSE);
#endif
			}
		}
	}
#endif
}


INT32 System_OnStrgInsert(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
	UINT32 stg_id = paramArray[0];

	if (m_BootState_Drive[stg_id] != BOOT_CARD_STATE_UNKNOWN) {
		if (stg_id == 0) {
#if(IPCAM_FUNC==DISABLE && SDHOTPLUG_FUNCTION == DISABLE)
			System_PowerOff(SYS_POWEROFF_NORMAL);
#endif
		}
	} else {
		TM_BOOT_BEGIN("sdio", "mount_fs");
		m_BootState_Drive[stg_id] = BOOT_CARD_STATE_INSERTED;
	}

	// linux partition as PStore
	if (stg_id == PST_DEV_ID) {
#if defined(_CPU2_LINUX_)
		if (GxStrg_OpenDevice(stg_id, NULL) != TRUE) {
			DBG_ERR("Storage mount pstore fail\r\n");
		}
		return NVTEVT_CONSUME;
#else
		DBG_FATAL("stg_id cannot be %d.\r\n", PST_DEV_ID);
#endif
	}

	DX_HANDLE pStrgDev = Dx_GetObject(DX_CLASS_STORAGE_EXT | m_FsDxTypeMap[stg_id]);
	if (GxStrg_OpenDevice(stg_id, pStrgDev) != TRUE) {
		char *pDxName = "unknown";
		Dx_GetInfo(pStrgDev, DX_INFO_NAME, &pDxName);
		DBG_ERR("Storage mount %s fail\r\n", pDxName);
		return NVTEVT_CONSUME;
	}

#if (USE_DCF == ENABLE)
	{
		DCF_OPEN_PARM           dcfParm = {0};
		// Open DCF
		dcfParm.Drive = (stg_id == 0) ? 'A' : 'B';
		dcfParm.WorkbuffAddr = dma_getCacheAddr(OS_GetMempoolAddr(POOL_ID_DCF_BUFFER));
		dcfParm.WorkbuffSize = POOL_SIZE_DCF_BUFFER;
		DCF_Open(&dcfParm);
		DCF_ScanObj();
	}
#endif

	if (GxStrg_GetDeviceCtrl(stg_id, CARD_READONLY)) {
		System_SetState(SYS_STATE_CARD, CARD_LOCKED);
		DBG_IND("Card Locked\r\n");
	} else {
		System_SetState(SYS_STATE_CARD, CARD_INSERTED);
		DBG_IND("Card inserted\r\n");
	}
	return NVTEVT_CONSUME;
}


INT32 System_OnStrgRemove(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
	UINT32 stg_id = paramArray[0];
	if (m_BootState_Drive[stg_id] != BOOT_CARD_STATE_UNKNOWN) {
		if (stg_id == 0) {
#if (LOGFILE_FUNC==ENABLE)
			LogFile_Suspend();
			LogFile_Close();
#endif
#if (USERLOG_FUNC == ENABLE)
			{
				userlog_close();
			}
#endif
#if (USE_DCF == ENABLE)
			//Fix the error "DCF_GetInfoByHandle() Dcf Handle 0 Data may be overwritted" when card plug out/in
			DCF_Close(0);
#endif
			System_SetState(SYS_STATE_CARD, CARD_REMOVED);

			GxStrg_CloseDevice(stg_id);
#if(IPCAM_FUNC==DISABLE && SDHOTPLUG_FUNCTION == DISABLE)
			System_PowerOff(SYS_POWEROFF_NORMAL);
#endif
		}
	} else {
		TM_BOOT_BEGIN("sdio", "mount_fs");
		m_BootState_Drive[stg_id] = BOOT_CARD_STATE_REMOVED;
		#if (FS_SWITCH_STRG_FUNC == ENABLE)
		if (stg_id==0) {
			DX_HANDLE pStrgDev = Dx_GetObject(DX_CLASS_STORAGE_EXT|FS_DX_TYPE_DRIVE_B);
			if (GxStrg_OpenDevice(stg_id, pStrgDev)!= TRUE) {
				char* pDxName="unknown";
				Dx_GetInfo(pStrgDev, DX_INFO_NAME,&pDxName);
				DBG_ERR("Storage mount %s fail\r\n",pDxName);
				return NVTEVT_CONSUME;
			}
			System_SetState(SYS_STATE_CARD, CARD_INSERTED);
			return NVTEVT_CONSUME;
		}
		#else
		// boot without card, send attach to continue UI flow.
		// because of UserWaitEvent(NVTEVT_STRG_ATTACH, &paramNum, paramArray);
		Ux_PostEvent(NVTEVT_STRG_ATTACH, 2, stg_id, 0xFF);
		#endif
	}

	DX_HANDLE pStrgDev = Dx_GetObject(DX_CLASS_STORAGE_EXT | m_FsDxTypeMap[stg_id]);
	if (GxStrg_CloseDevice(stg_id) != TRUE) {
		char *pDxName = "unknown";
		Dx_GetInfo(pStrgDev, DX_INFO_NAME, &pDxName);
		DBG_ERR("Storage mount %s fail\r\n", pDxName);
		return NVTEVT_CONSUME;
	}

	return NVTEVT_CONSUME;
}

INT32 System_OnStrgAttach(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
	MODELEXT_HEADER *head = NULL;
	BININFO *pBinInfo = (BININFO *)Dx_GetModelExtCfg(MODELEXT_TYPE_BIN_INFO, &head);
	UINT32 result = paramArray[1];

	if (m_GxStrgType == FST_FS_TYPE_LINUX) {
		//Do Nothing
	} else { //m_GxStrgType == FST_FS_TYPE_UITRON
		//if spi use RamDisk as inner Memory,need to format RamDisk
#if defined(_EMBMEM_SPI_NOR_)
		DX_HANDLE pStrgDevCur = (DX_HANDLE)GxStrg_GetDevice(0);
		DX_HANDLE pStrgDevRAM = (DX_HANDLE)Dx_GetObject(DX_CLASS_STORAGE_EXT | USER_DX_TYPE_EMBMEM_FAT);

		if ((pStrgDevCur == pStrgDevRAM) && (result != FST_STA_OK)) {
			result = FileSys_FormatDisk(pStrgDevCur, TRUE);
		}
#endif
    CHKPNT;
#if ( !defined(_EMBMEM_SPI_NOR_) && (FS_MULTI_STRG_FUNC == ENABLE))
		UINT32 stg_id = paramArray[0];

		if (stg_id != 0) { // stg_id=1 is interal storage FAT
			return NVTEVT_CONSUME;
		}
#endif
	}

	switch (result) {
	case FST_STA_OK:
#if (USE_DCF == ENABLE)
		if (!UI_GetData(FL_IsCopyToCarding)) {
			DCF_ScanObj();
		}
#endif
        System_SetState(SYS_STATE_FS, FS_INIT_OK);
		if (pBinInfo && pBinInfo->ld.LdCtrl2 & LDCF_UPDATE_FW) {
			FST_FILE hFile = FileSys_OpenFile(FW_DEL_INDIACTION_PATH, FST_OPEN_READ | FST_OPEN_EXISTING);
			if (hFile != NULL) {
				DBG_DUMP("Detected %s, delete %s \r\n", FW_DEL_INDIACTION_PATH, LOADER_UPD_FW_PATH);
				FileSys_CloseFile(hFile);
				// Delete FW bin from A:
				if (FileSys_DeleteFile(LOADER_UPD_FW_PATH) != FST_STA_OK) {
					DBG_ERR("delete "_BIN_NAME_".BIN failed .\r\n");
				}
				if (FileSys_DeleteFile(FW_DEL_INDIACTION_PATH) != FST_STA_OK) {
					DBG_ERR("delete %s failed .\r\n", FW_DEL_INDIACTION_PATH);
				}
			}
		}
#if (LOGFILE_FUNC==ENABLE)
		{
			LOGFILE_OPEN    logOpenParm = {0};
			UINT32          maxFileNum = 32;
			UINT32          maxFileSize = 0x100000; // 1MB
			CHAR            rootDir[LOGFILE_ROOT_DIR_MAX_LEN + 1] = "A:\\Novatek\\LOG\\";
			CHAR            sysErrRootDir[LOGFILE_ROOT_DIR_MAX_LEN + 1] = "A:\\Novatek\\SYS\\";

			logOpenParm.maxFileNum = maxFileNum;
			logOpenParm.maxFileSize = maxFileSize;
			// linux not support alloc file , so set false
			#if defined(_CPU2_LINUX_)
			logOpenParm.isPreAllocAllFiles = FALSE;
			#else
			logOpenParm.isPreAllocAllFiles = TRUE;
			#endif
			logOpenParm.isSaveLastTimeSysErrLog  = wdt_getResetNum()>0 ? TRUE : FALSE;
			logOpenParm.lastTimeSysErrLogBuffAddr = OS_GetMempoolAddr(POOL_ID_LOGFILE);
			logOpenParm.lastTimeSysErrLogBuffSize = POOL_SIZE_LOGFILE;
			strncpy(logOpenParm.rootDir, rootDir, LOGFILE_ROOT_DIR_MAX_LEN);
			strncpy(logOpenParm.sysErrRootDir, sysErrRootDir, LOGFILE_ROOT_DIR_MAX_LEN);
			LogFile_Open(&logOpenParm);
		}
#endif
#if (USERLOG_FUNC == ENABLE)
		{
			userlog_open();
		}
#endif
		break;
	case FST_STA_DISK_UNFORMAT:
		System_SetState(SYS_STATE_FS, FS_UNFORMATTED);
		break;
	case FST_STA_DISK_UNKNOWN_FORMAT:
		System_SetState(SYS_STATE_FS, FS_UNKNOWN_FORMAT);
		break;
	case FST_STA_CARD_ERR:
		System_SetState(SYS_STATE_FS, FS_DISK_ERROR);
		break;
	default:
		System_SetState(SYS_STATE_FS, FS_DISK_ERROR);
		break;
	}
#if (POWERON_FAST_BOOT == ENABLE)
	INIT_SETFLAG(FLGINIT_STRGATH);
#endif
	Ux_PostEvent(NVTEVT_STORAGE_INIT, 0, 0);
#if (PWR_FUNC == ENABLE)
	GxPower_SetControl(GXPWR_CTRL_SLEEP_RESET, 0);
#endif
	return NVTEVT_CONSUME;
}

INT32 System_OnStrgDetach(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
	if (m_GxStrgType == FST_FS_TYPE_LINUX) {
		UINT32 stg_id = paramArray[0];

		if (stg_id != 0) { //not sd-1
			return NVTEVT_CONSUME;
		}
	}

	switch (paramArray[1]) {
	case FST_STA_OK:
		DBG_IND("FS: unmount OK\r\n");
		break;

	default:
		DBG_ERR("^RFS: unmount FAIL\r\n");
		break;
	}
	return NVTEVT_CONSUME;
}

BOOL gChkCardPwrOn = FALSE;
BOOL gChkCardChange = FALSE;

void Storage_PowerOn_Start(void)
{
	gChkCardPwrOn = GxStrg_GetDeviceCtrl(0, CARD_INSERT);
	DBG_IND("^BStg Power On = %d\r\n", gChkCardPwrOn);
}
void Storage_UpdateSource(void)
{
	DBG_IND("^Y-------------CARD det\r\n");
	if (gChkCardPwrOn) {
		if (FALSE == GxStrg_GetDeviceCtrl(0, CARD_INSERT)) { //CARD已拔除
			gChkCardChange = TRUE;
		}
	} else {
		if (TRUE == GxStrg_GetDeviceCtrl(0, CARD_INSERT)) { //CARD已插入
			gChkCardChange = TRUE;
		}
	}
}
void Storage_PowerOn_End(void)
{
	Storage_UpdateSource();
	gChkCardPwrOn = GxStrg_GetDeviceCtrl(0, CARD_INSERT);

	if (TRUE == gChkCardChange) { //CARD曾經插拔
		System_PowerOff(SYS_POWEROFF_NORMAL); //關機
		return;
	}
}


///////////////////////////////////////////////////////////////////////////////
//
//  PSTORE
//
///////////////////////////////////////////////////////////////////////////////
void System_PS_Format(void)
{
	PSFMT gFmtStruct = {MAX_BLK_PER_SEC, PSTORE_MAX_SECTION_NUM};
	PStore_Format(&gFmtStruct);
}
void System_OnFlashInit_PS(void)
{
	//PHASE-2 : Init & Open Lib or LibExt
	TM_BOOT_BEGIN("nand", "init_ps");
#if (PST_FUNC == ENABLE)
	// Open PStore
	PSFMT gFmtStruct = {MAX_BLK_PER_SEC, PSTORE_MAX_SECTION_NUM};
	PSTORE_INIT_PARAM gPStoreParam;
	UINT32 result = 0;
	UINT8 *pBuf;
	DX_HANDLE pStrgDev = 0;
	pStrgDev = Dx_GetObject(DX_CLASS_STORAGE_EXT | USER_DX_TYPE_EMBMEM_PSTORE);
#if defined(_CPU2_LINUX_) && defined(_EMBMEM_EMMC_)
	PStore_Init(PS_TYPE_FILESYS, PST_FS_DRIVE[0]);
	UINT32 paramNum;
	UINT32 paramArray[MAX_MESSAGE_PARAM_NUM];
	do {
		UserWaitEvent(NVTEVT_STRG_ATTACH, &paramNum, paramArray);
	} while(paramArray[0] != PST_DEV_ID); //PStore will mount first before dev[0],dev[1]
#else
#if (defined(_BSP_NA51023_) && _TODO)
	PStore_Init(PS_TYPE_EMBEDED, 0);
#endif
#endif
	pBuf = (UINT8 *)OS_GetMempoolAddr(POOL_ID_PS_BUFFER);

        PStore_SetPrjVersion(PST_VER);

	gPStoreParam.pBuf = pBuf;
	gPStoreParam.uiBufSize = POOL_SIZE_PS_BUFFER;
	DBG_DUMP("PStore uiBufSize=%d\r\n", gPStoreParam.uiBufSize);
	result = PStore_Open(pStrgDev, &gPStoreParam);

	if (result != E_PS_OK) {
		DBG_ERR("PStore Open fail %d format \r\n", result);
		PStore_Format(&gFmtStruct);
	}

#if defined(_CPU2_LINUX_)
	{
		PSTOREIPC_OPEN   PsopenObj = {0};
		// open pstore ipc
		PstoreIpc_Open(&PsopenObj);
	}
#endif
#endif
#if (POWERON_FAST_BOOT == ENABLE)
	INIT_SETFLAG(FLGINIT_MOUNTPS);
#endif
	TM_BOOT_END("nand", "init_ps");
}
void System_OnStrgInit_PS(void)
{
#if (PST_FUNC == ENABLE)
#if defined(_CPU2_LINUX_) && defined(_EMBMEM_EMMC_)
        TM_BOOT_BEGIN("pstore", "fast");
        if(fastboot_pstore_init(0x80000 - 8192)){
                DBG_ERR("fail to init fastboot pstore\r\n");
                return;
        }
        TM_BOOT_END("pstore", "fast");

	BKG_PostEvent(NVTEVT_BKW_PS_INIT);
#else
	System_OnFlashInit_PS();
#endif
#endif
}
void System_OnStrgExit_PS(void)
{
#if (PST_FUNC == ENABLE)
#if defined(_CPU2_LINUX_)
	PstoreIpc_Close();
#endif
	PStore_Close();
#endif
}

///////////////////////////////////////////////////////////////////////////////
//
//  FWSTORE
//
///////////////////////////////////////////////////////////////////////////////
#include "DxStorage.h"
#include "FileSysTsk.h"
#if (FWS_FUNC == ENABLE)
#include "FwSrvApi.h"
#endif
#include "MemCheck.h"
#include "GxStrg.h"

#if (FWS_FUNC == ENABLE)
#if !defined(_EMBMEM_NONE_)
static UINT32 gPL_BufAddr = 0;
static UINT32 gPL_BufSize = 0;
static FWSRV_INIT gPL_Init = {0};
static FWSRV_CMD gPL_Cmd = {0};
static FWSRV_PL_LOAD_BURST_IN gPL_In = {0};
#endif
#endif

void System_OnStrgInit_FWS(void)
{
	TM_BOOT_BEGIN("nand", "init_fws");
#if (FWS_FUNC == ENABLE)
#if !defined(_EMBMEM_NONE_)
	ER er;
	UINT32 m_LdLoadedOffest;
	DX_HANDLE pStrgDev = 0;
	MODELEXT_HEADER *header = NULL;
	BININFO *pBinInfo = (BININFO *)Dx_GetModelExtCfg(MODELEXT_TYPE_BIN_INFO, &header);
	if (!pBinInfo || !header) {
		DBG_ERR("bininfo is null\r\n");
		return;
	}

	DBG_MSG("Init!\r\n");
	DBG_IND("^M LD_RSV_SIZE=%d\r\n", pBinInfo->ld.LdResvSize);
	DBG_IND("^M FW_MAX_SIZE=%08X\r\n", pBinInfo->ld.FWResvSize);
	// here we removed LDCF_UPDATE_FW_DONE check, becasue it was handled on u-boot

	pStrgDev = Dx_GetObject(DX_CLASS_STORAGE_EXT | USER_DX_TYPE_EMBMEM_UITRON);

#if defined(_FW_TYPE_PARTIAL_COMPRESS_)
	//#NT#2018/04/02#Niven Cho -begin
	//#NT#PARTIAL_COMPRESS, we use last 10MB of APP as working buffer
	gPL_Init.PlInit.DataType = PARTLOAD_DATA_TYPE_COMPRESS_GZ;
	gPL_BufAddr = OS_GetMempoolAddr(POOL_ID_APP) + OS_GetMempoolSize(POOL_ID_APP) - FW_PARTIAL_COMPRESS_WORK_BUF_SIZE;
	gPL_BufSize = FW_PARTIAL_COMPRESS_WORK_BUF_SIZE ;
	//#NT#2018/04/02#Niven Cho -end
#else
	gPL_Init.PlInit.DataType = PARTLOAD_DATA_TYPE_UNCOMPRESS;
	gPL_BufAddr = OS_GetMempoolAddr(POOL_ID_FWS_BUFFER);
	gPL_BufSize = POOL_SIZE_FWS_BUFFER;
#endif

	m_LdLoadedOffest = pBinInfo->ld.LdLoadSize; //Get Ld loaded size

	gPL_Init.uiApiVer = FWSRV_API_VERSION;
	gPL_Init.TaskID = 0;
	gPL_Init.SemID = 0;
	gPL_Init.FlagID = 0;
	gPL_Init.DxMap.ModelExt= USER_DX_TYPE_EMBMEM_MODELEXT;
	gPL_Init.DxMap.uITRON = USER_DX_TYPE_EMBMEM_UITRON;
	gPL_Init.DxMap.uBoot = USER_DX_TYPE_EMBMEM_UBOOT;
	gPL_Init.DxMap.Linux = USER_DX_TYPE_EMBMEM_LINUX;
	gPL_Init.DxMap.RootFs = USER_DX_TYPE_EMBMEM_UITRON;
	gPL_Init.DxMap.DSP = USER_DX_TYPE_EMBMEM_DSP;
#if defined(_BSP_NA51000_)
	gPL_Init.DxMap.DSP2 = USER_DX_TYPE_EMBMEM_DSP2;
#endif
	gPL_Init.PlInit.uiApiVer = PARTLOAD_API_VERSION;
	gPL_Init.PlInit.hStrg = pStrgDev;
	gPL_Init.PlInit.uiAddrBegin = OS_GetMemAddr(MEM_CODE) + m_LdLoadedOffest; //Must be block boundary align
	gPL_Init.PlInit.uiWorkingAddr = gPL_BufAddr;
	gPL_Init.PlInit.uiWorkingSize = gPL_BufSize;
	er = FwSrv_Init(&gPL_Init);
	if (er != FWSRV_ER_OK) {
		DBG_ERR("Init failed!\r\n");
	}
	er = FwSrv_Open();
	if (er != FWSRV_ER_OK) {
		DBG_ERR("Open failed!\r\n");
	}

#endif
#endif

#if (POWERON_FAST_BOOT == ENABLE)
	INIT_SETFLAG(FLGINIT_MOUNTFWS);
#endif
	TM_BOOT_END("nand", "init_fws");
}

void System_OnStrgExit_FWS(void)
{
#if (FWS_FUNC == ENABLE)
	ER er;
	er = FwSrv_Close();
	if (er != FWSRV_ER_OK) {
		DBG_ERR("Close failed!\r\n");
	}
#endif
}

void System_CPU2_Start(void)
{
#if (ECOS_POWER_OFF_FLOW == ENABLE)
	DBG_ERR("ECOS_POWER_OFF_FLOW not support.\r\n");
#else
	static BOOL bStartCPU2 = FALSE;
#endif
	if (!bStartCPU2) {
#if defined(_CPU2_LINUX_)
		//if not support, this function returns directly.
		xSysStrg_LinuxRun();
#endif
#if defined(_CPU2_ECOS_)
		//if not support, this function returns directly.
		xSysStrg_eCosRun();
#endif
		bStartCPU2 = TRUE;
	}
}

void System_CPU2_Stop(void)
{
#if defined(_CPU2_LINUX_)
	cpu2util_terminate();
#endif
}

void System_CPU2_WaitReady(void)
{
#if defined(_CPU2_ECOS_) || defined(_CPU2_LINUX_)
	NvtIPC_WaitCPU2PowerOnReady();
#endif
}

void System_DSP_Start(void)
{
	static BOOL bStartDSP = FALSE;
	if (!bStartDSP) {
#if defined(_DSP1_FREERTOS_)
		xSysStrg_DspRun(DSP_CORE_ID_1);
#endif
#if defined(_DSP2_FREERTOS_)
		xSysStrg_DspRun(DSP_CORE_ID_2);
#endif
		bStartDSP = TRUE;
	}
}

void System_DSP_WaitReady(void)
{
#if defined(_DSP1_FREERTOS_)
	NvtIPC_WaitDSPPowerOnReady();
#endif
#if defined(_DSP2_FREERTOS_)
	NvtIPC_WaitDSP2PowerOnReady();
#endif

}

void System_OnStrg_DownloadFW(UINT32 *SecOrderTable, void (*LoadCallback)(const UINT32 Idx))
{
	TM_BOOT_BEGIN("nand", "load2");
	DBG_MSG("Init!\r\n");

	//DSP must start before CPU2, because there is chance of cpu2 using POOL_APP which has used by compressed DSP.
	System_DSP_Start();

#if defined(_CPU2_LINUX_) || (defined(_CPU2_ECOS_) && (POWERON_FAST_CPU2_BOOT == ENABLE))
	System_CPU2_Start();
#endif

#if (FWS_FUNC == ENABLE)
#if !defined(_EMBMEM_NONE_)
	BOOL bEnablePartload = FALSE;
	int nSect = 0;
	MODELEXT_HEADER *header = NULL;
	BININFO *pBinInfo = (BININFO *)Dx_GetModelExtCfg(MODELEXT_TYPE_BIN_INFO, &header);
	if (!pBinInfo || !header) {
		DBG_ERR("bininfo is null\r\n");
		return;
	}

	BOOL bRunFromCard = (pBinInfo->ld.LdCtrl2 & LDCF_BOOT_CARD) ? TRUE : FALSE;
	DBG_IND("^G[LOAD-FW]\r\n");
	nSect = OS_GetMemSectionCount(MEM_CODE);  //How to get last section ZI
	DBG_IND("^GTotal Sections = %d\r\n", nSect);
	if (LoadCallback) {
		LoadCallback(CODE_SECTION_01);
	}
	DBG_IND("^M P1_LOAD_SIZE=%08X, TIME=%d\r\n", pBinInfo->ld.LdLoadSize, pBinInfo->ld.LdLoadTime);

	DBG_IND("^MPL_check_Ld:\r\n");
	DBG_IND("^M PL_EN=%08X\r\n", pBinInfo->ld.LdCtrl & LDCF_PARTLOAD_EN);
	DBG_IND("^M LZ_EN=%08X\r\n", pBinInfo->head.BinCtrl & HDCF_LZCOMPRESS_EN);
	bEnablePartload = ((pBinInfo->ld.LdCtrl & LDCF_PARTLOAD_EN) != 0);

	if (!bEnablePartload || bRunFromCard) {
		return;
	}

	DBG_IND("^MPL_validate-p1-range:");
	//ValidatePartOne
	{
		// Make Sure: Partloader start (PART-2 start) <= Loader loaded end
		int i;
		UINT32 FwSize = 0;
		UINT32 uiDestAddress = OS_GetMemAddr(MEM_CODE) + pBinInfo->ld.LdLoadSize;
		if ((uiDestAddress != 0) && (uiDestAddress < OS_GetMemAddr(1))) {
			DBG_FATAL("PART_TWO start (%08x) > PART_ONE loaded end (%08x)!\r\n",
					  OS_GetMemAddr(1), uiDestAddress);
		}

		// Make Sure: Partloader end (PART-last end) <= MEM_CODE end
		for (i = 0; i < nSect; i++) {
			DBG_IND("Sect%d: Addr:%08X, Size:%08X\r\n", i, OS_GetMemAddr(i), OS_GetMemSize(i));
			FwSize += OS_GetMemSize(i);
		}
		if (OS_GetMemAddr(0) + FwSize > OS_GetMemAddr(MEM_CODE) + OS_GetMemSize(MEM_CODE)) {
			DBG_FATAL("PART_LAST end (%08x) > MEM_CODE end (%08x)!\r\n",
					  OS_GetMemAddr(0) + FwSize, OS_GetMemAddr(MEM_CODE) + OS_GetMemSize(MEM_CODE));
		}
	}
	DBG_IND("^M ok\r\n");

	DBG_IND("^MPL_begin\r\n");
	{
		ER er;
		gPL_In.puiIdxSequence = SecOrderTable;
		gPL_In.fpLoadedCb = LoadCallback;
		gPL_Cmd.Idx = FWSRV_CMD_IDX_PL_LOAD_BURST; //continue load
		gPL_Cmd.In.pData = &gPL_In;
		gPL_Cmd.In.uiNumByte = sizeof(gPL_In);
		gPL_Cmd.Prop.bExitCmdFinish = TRUE;
		er = FwSrv_Cmd(&gPL_Cmd);
		if (er != FWSRV_ER_OK) {
			DBG_ERR("Process failed!\r\n");
		}
	}
	DBG_IND("^MPL_end\r\n");
#endif
#endif
	TM_BOOT_END("nand", "load2");
}

#if (IPCAM_FUNC != ENABLE)
UINT32 System_OnStrg_UploadFW(UINT32 DevID)
{
#if (FWS_FUNC == ENABLE)
#if !defined(_CPU2_LINUX_)
	DBG_ERR("is only for _CPU2_LINUX_.\r\n");
	return UPDNAND_STS_FW_INVALID_STG;
#else
	if (System_GetState(SYS_STATE_CURRMODE) != PRIMARY_MODE_MAIN) {
#if (UPDFW_MODE == ENABLE)
		if (System_GetState(SYS_STATE_CURRMODE) != PRIMARY_MODE_UPDFW) {
			DBG_ERR("must be in update mode.\r\n");
			return UPDNAND_STS_INVLID_MODE;
		}
#else
		DBG_ERR("must be in main mode.\r\n");
		return UPDNAND_STS_INVLID_MODE;
#endif
	}

	INT32 fst_er;
	FWSRV_CMD Cmd = {0};
	FST_FILE hFile;
	UINT32 uiWorkAddr, uiWorkSize;
	UINT32 uiFwAddr, uiFwSize;

	// get file size first to calc working buffer
	uiFwSize = (UINT32)FileSys_GetFileLen(FW_UPDATE_NAME);
	uiFwAddr = OS_GetMempoolAddr(POOL_ID_APP);

	// read file
	hFile = FileSys_OpenFile(FW_UPDATE_NAME, FST_OPEN_READ);
	if (hFile != 0) {
		fst_er = FileSys_ReadFile(hFile, (UINT8 *)uiFwAddr, &uiFwSize, 0, NULL);
		FileSys_CloseFile(hFile);
		if (fst_er != FST_STA_OK) {
			DBG_ERR("FW bin read fail\r\n");
			return UPDNAND_STS_FW_READ2_ERR;
		}
	} else {
		DBG_ERR("cannot find %s\r\n", FW_UPDATE_NAME);
		return UPDNAND_STS_FW_READ2_ERR;
	}

	uiWorkAddr = uiFwAddr + ALIGN_CEIL_32(uiFwSize);
	uiWorkSize = 0xA00000; // 10MB for temp memory

	// verify firmware
	FWSRV_VERIFY_FW Verify = {0};
	Verify.Buf.uiAddr = uiFwAddr;
	Verify.Buf.uiSize = uiFwSize;
	Cmd.Idx = FWSRV_CMD_IDX_VERIFY_FW;
	Cmd.In.pData = &Verify;
	Cmd.In.uiNumByte = sizeof(Verify);
	Cmd.Prop.bExitCmdFinish = TRUE;
	if (FwSrv_Cmd(&Cmd) != FWSRV_ER_OK) {
		DBG_ERR("check sum failed.\r\n");
		return UPDNAND_STS_FW_READ_CHK_ERR;
	}

	// CPU2 must stop before load uboot firmware
	System_CPU2_Stop();

	// load uboot partition
	FWSRV_LOAD_PARTITION Load = {0};
	Load.EmbType = EMBTYPE_UBOOT;
	Load.OrderIdx = 0;
	Load.WorkBuf.uiAddr = uiWorkAddr;
	Load.WorkBuf.uiSize = uiWorkSize;

	Cmd.Idx = FWSRV_CMD_IDX_LOAD_PARTITION;
	Cmd.In.pData = &Load;
	Cmd.In.uiNumByte = sizeof(Load);
	Cmd.Prop.bExitCmdFinish = TRUE;
	if (FwSrv_Cmd(&Cmd) != FWSRV_ER_OK) {
		DBG_ERR("load uboot failed.\r\n");
		return UPDNAND_STS_FW_INVALID_STG;
	}

	// start update
	HEADINFO *p_hdr = (HEADINFO *)(_BOARD_UBOOT_ADDR_ + BIN_INFO_OFFSET_UBOOT);
	CPU2UTIL_UPDFW upd_fw = {0};
	upd_fw.uboot_addr = p_hdr->CodeEntry;
	upd_fw.uboot_size = p_hdr->BinLength;
	upd_fw.fw_addr = uiFwAddr;
	upd_fw.fw_size = uiFwSize;
	upd_fw.ota_name = FW_UPDATE_NAME+3; // +3 to remove "A:\"

	if (cpu2util_updfw(&upd_fw) != 0) {
		DBG_ERR("update failed - 1.\r\n");
		return UPDNAND_STS_FW_WRITE_CHK_ERR;
	}

	if (cpu2util_wait_updfw_done(5) != 0) {
		DBG_ERR("update failed - 2.\r\n");
		return UPDNAND_STS_FW_WRITE_CHK_ERR;
	}

	cpu2util_terminate();

	return UPDNAND_STS_FW_OK;
#endif
#else
	DBG_ERR("not support FwSrv!\r\n");
	return UPDNAND_STS_FW_INVALID_STG;
#endif
}
#endif

//#NT#2018/03/14#Niven Cho -begin
//#NT#FORMAT_ROOTFS
UINT32 System_OnStrg_Format_RootFs(void)
{
#if (FWS_FUNC == ENABLE)
#if !defined(_CPU2_LINUX_)
	DBG_ERR("is only for _CPU2_LINUX_.\r\n");
	return UPDNAND_STS_FW_INVALID_STG;
#else
	if (System_GetState(SYS_STATE_CURRMODE) != PRIMARY_MODE_MAIN) {
#if (UPDFW_MODE == ENABLE)
		if (System_GetState(SYS_STATE_CURRMODE) != PRIMARY_MODE_UPDFW) {
			DBG_ERR("must be in main mode.\r\n");
			return UPDNAND_STS_INVLID_MODE;
		}
#else
		DBG_ERR("must be in main mode.\r\n");
		return UPDNAND_STS_INVLID_MODE;
#endif
	}

	FWSRV_CMD Cmd = {0};

	// CPU2 must stop before load uboot firmware
	System_CPU2_Stop();

	// load uboot partition
	FWSRV_LOAD_PARTITION Load = {0};
	Load.EmbType = EMBTYPE_UBOOT;
	Load.OrderIdx = 0;
	Load.WorkBuf.uiAddr = OS_GetMempoolAddr(POOL_ID_APP);
	Load.WorkBuf.uiSize = OS_GetMempoolSize(POOL_ID_APP);

	Cmd.Idx = FWSRV_CMD_IDX_LOAD_PARTITION;
	Cmd.In.pData = &Load;
	Cmd.In.uiNumByte = sizeof(Load);
	Cmd.Prop.bExitCmdFinish = TRUE;
	if (FwSrv_Cmd(&Cmd) != FWSRV_ER_OK) {
		DBG_ERR("load uboot failed.\r\n");
		return UPDNAND_STS_FW_INVALID_STG;
	}

	// start update
	HEADINFO *p_hdr = (HEADINFO *)(_BOARD_UBOOT_ADDR_ + BIN_INFO_OFFSET_UBOOT);
	CPU2UTIL_FORMAT_ROOTFS format_rootfs = {0};
	format_rootfs.uboot_addr = p_hdr->CodeEntry;
	format_rootfs.uboot_size = p_hdr->BinLength;
	format_rootfs.timeout_min = 2;

	if (cpu2util_format_rootfs(&format_rootfs) != 0) {
		DBG_ERR("failed format rootfs - 1.\r\n");
		return UPDNAND_STS_FW_WRITE_CHK_ERR;
	}

	return UPDNAND_STS_FW_OK;
#endif
#else
	DBG_ERR("not support FwSrv!\r\n");
	return UPDNAND_STS_FW_INVALID_STG;
#endif
}
//#NT#2018/03/14#Niven Cho -end

#if defined(_CPU2_LINUX_)
static BOOL xSysStrg_LinuxRun(void)
{
#ifdef  __S3_POWER_DOWN
    MODELEXT_HEADER *header;
	BININFO *pBinInfo = (BININFO *)Dx_GetModelExtCfg(MODELEXT_TYPE_BIN_INFO, &header);
#endif

	m_GxStrgType = FST_FS_TYPE_LINUX;
#ifdef  __S3_POWER_DOWN
	if(pBinInfo->ld.LdCtrl2 & LDCF_S3_BOOT) {
		cpu2util_resume();
	}
#endif
	return TRUE;
}
#endif

#if (defined(_DSP1_FREERTOS_) || defined(_DSP2_FREERTOS_))
static BOOL xSysStrg_DspRun(DSP_CORE_ID CoreID)
{
//#NT#2018/02/07#Ben Wang -begin
//#NT#EMMC can't be accessed by uITRON and Linux at the same time.
	MODELEXT_HEADER *header = NULL;
#if defined(_CPU2_LINUX_) && defined(_EMBMEM_EMMC_)
	//in this case, DSP FW should be loaded by uboot
	BININFO *pBinInfo = (BININFO *)Dx_GetModelExtCfg(MODELEXT_TYPE_BIN_INFO, &header);
	volatile UINT8 *pDspLoadDone;
	UINT32 WaitCnt = 0;
	if (!pBinInfo || !header) {
		DBG_FATAL("bininfo is null\n");
		return FALSE;
	}
	//refer to COMMINFO
	pDspLoadDone = (volatile UINT8 *)&pBinInfo->comm.Resv[0] + 2;
	//BYTE[2]: DSP_DONE:  0:Not yet, 1: Done 2:NG
	while (*pDspLoadDone == 0) {
		Delay_DelayMs(10);
		WaitCnt ++;
		if(WaitCnt > 600) {
			DBG_ERR("uboot load DSP timeout!\r\n");
			return FALSE;
		}
	}
	if(*pDspLoadDone == 2) {
		DBG_ERR("uboot load DSP failed!\r\n");
		return FALSE;
	}

#else
	UINT32 DxTypeMap;
	MEM_RANGE WorkBuf;
	DBG_DUMP("^G[LOAD DSP%d]\r\n",CoreID+1);
	WorkBuf.Addr= OS_GetMempoolAddr(POOL_ID_APP);
    WorkBuf.Size = POOL_SIZE_APP;
	if (CoreID == DSP_CORE_ID_1) {
		DxTypeMap = USER_DX_TYPE_EMBMEM_DSP;
	} else {
		DxTypeMap = USER_DX_TYPE_EMBMEM_DSP2;
	}
	if(FALSE == GxDSP_LoadFW(CoreID, &WorkBuf, DxTypeMap)) {
		return FALSE;
	}
#endif
	DBG_DUMP("^G[RUN DSP%d]\r\n",CoreID+1);

	GXDSP_OPEN Open = {0};
	DRAM_PARTITION *p_dram_partition = (DRAM_PARTITION *)Dx_GetModelExtCfg(MODELEXT_TYPE_DRAM_PARTITION, &header);
	if (CoreID == DSP_CORE_ID_1) {
		Open.uiAddr = p_dram_partition->dsp1_addr; //memory pool
		Open.uiSize = p_dram_partition->dsp1_size;
	} else {

		Open.uiAddr = p_dram_partition->dsp2_addr; //memory pool
		Open.uiSize = p_dram_partition->dsp2_size;
	}
	if (GxDSP_Open(&Open)) {
		return TRUE;
	}
//#NT#2018/02/07#Ben Wang -end
	return FALSE;
}
#endif
