////////////////////////////////////////////////////////////////////////////////
#include "SysCommon.h"
////////////////////////////////////////////////////////////////////////////////
#include "UIFramework.h"
#include "UIBackgroundObj.h"
#include "Utility.h"
#include "DpofVendor.h"

#include "FileSysTsk.h"
#include "DCF.h"
#include "UIInfo.h"
#include "StrgDef.h"
#include "UIPlayComm.h"

//#NT#2012/10/30#Calvin Chang#Generate Audio NR pattern by noises of zoom, focus and iris -begin
#include "UIAppMovie.h"
#include "Lens.h"
//#NT#2012/10/30#Calvin Chang -end
#include "UIAppNetwork.h"
#include "DxType.h"
#include "Dx.h"
#include "PlaybackTsk.h"
//#include "DialogWarn.h"
#include "FileDB.h"
#include "dma.h"
#include "UISetup.h"
//#NT#2016/05/30#Lincy Lin -begin
//#NT#Support logfile function
#if (LOGFILE_FUNC==ENABLE)
#include "LogFile.h"
#endif
#if(UPDFW_MODE==ENABLE)
#include "UIModeUpdFw.h"
#endif
#if(UPDFW_MODE==ENABLE)
#include "UIModeUpdFw.h"
#include "SysMain.h"
#endif
#include "UIFlowMovieFuncs.h"

//#NT#2016/05/30#Lincy Lin -end
///////////////////////////////////////////////////////////////////////////////
#define __MODULE__          UIBKGObj
#define __DBGLVL__          6 // 0=FATAL, 1=ERR, 2=WRN, 3=UNIT, 4=FUNC, 5=IND, 6=MSG, 7=VALUE, 8=USER
#define __DBGFLT__          "*" //*=All, [mark]=CustomClass
#include "DebugModule.h"
///////////////////////////////////////////////////////////////////////////////
#define PB_COPY2CARD_NEW_DIR            0x00
#define PB_COPY2CARD_APPEND             0x01
#define PB_COPY2CARD_NO_RENAME          0x02
#define PB_COPY2CARD_NEW_DIR_ORG_FILE   0x03

static UINT32 Background_ProtectAll(BOOL bLock); //for any mode protect/unproteck all
static UINT32 Background_DeleteAll(void); //for any mode delete all
static UINT32 BackgroundDelAll(void);
static UINT32 BackgroundCopy2Card(void);
#if(COPYCARD2CARD_FUNCTION == ENABLE)
static UINT32 BackgroundCopyCard1ToCard2(void);
static UINT32 BackgroundCopyCard2ToCard1(void);
#endif
static UINT32 BackgroundFormat(void);
static UINT32 BackgroundFormatCard(void);
static UINT32 BackgroundFormatNand(void);
static UINT32 BackgroundNumReset(void);
#if (USE_DPOF==ENABLE)
static UINT32 BackgroundSetDPOF(void);
#endif
static UINT32 BackgroundSetProtect(void);
static UINT32 BackgroundDummyWait(void);
static UINT32 BackgroundFWUpdate(void);
#if (PHOTO_MODE==ENABLE)
static UINT32 BackgroundDZoomIn(void);
static UINT32 BackgroundDZoomOut(void);
#endif
static UINT32 BackgroundPIMProcess(void);
//#NT#2012/10/30#Calvin Chang#Generate Audio NR pattern by noises of zoom, focus and iris -begin
static UINT32 BackgroundAudiNRLensAction(void);
//#NT#2012/10/30#Calvin Chang -end
static UINT32 BackgroundWiFiOn(void);
#if(WIFI_AP_FUNC==ENABLE)
static UINT32 BackgroundWiFiClearACL(void);
#endif
static UINT32 BackgroundPSInit(void);
static UINT32 BackgroundStopRecProcess(void);

static UINT32 g_uiDpofOPMode = PLAYDPOF_SETONE;
static UINT32 g_uiDpofPrtNum = 0;
static UINT32 g_uiDpofDateOn = FALSE;
static UINT32 g_uiProtectType = SETUP_PROTECT_ALL;
static UINT32 g_uiWaitTime = 1;
static UINT32 *g_pDzoomStop = NULL;

UINT32 gUIStopBckGndDummyWait = FALSE;

static FILEDB_INIT_OBJ gBKFDBInitObj = {
	"A:\\",  //rootPath
	NULL,  //defaultfolder
	NULL,  //filterfolder
	TRUE,  //bIsRecursive
	TRUE,  //bIsCyclic
	TRUE,  //bIsMoveToLastFile
	TRUE, //bIsSupportLongName
	FALSE, //bIsDCFFileOnly
	TRUE,  //bIsFilterOutSameDCFNumFolder
	{'N', 'V', 'T', 'I', 'M'}, //OurDCFDirName[5]
	{'I', 'M', 'A', 'G'}, //OurDCFFileName[4]
	FALSE,  //bIsFilterOutSameDCFNumFile
	FALSE, //bIsChkHasFile
	60,    //u32MaxFilePathLen
	10000,  //u32MaxFileNum
	(FILEDB_FMT_JPG | FILEDB_FMT_AVI | FILEDB_FMT_MOV | FILEDB_FMT_MP4 | FILEDB_FMT_TS), //fileFilter
	0,     //u32MemAddr
	0,     //u32MemSize
	NULL   //fpChkAbort

};
BKG_JOB_ENTRY gBackgroundExtFuncTable[] = {
	{NVTEVT_BKW_DELALL,             BackgroundDelAll},
	{NVTEVT_BKW_COPY2CARD,          BackgroundCopy2Card},
#if(COPYCARD2CARD_FUNCTION == ENABLE)
	{NVTEVT_BKW_COPYCARD1ToCARD2,   BackgroundCopyCard1ToCard2},
	{NVTEVT_BKW_COPYCARD2ToCARD1,   BackgroundCopyCard2ToCard1},
#endif
	{NVTEVT_BKW_FORMAT,             BackgroundFormat},
	{NVTEVT_BKW_FORMAT_CARD,        BackgroundFormatCard},
	{NVTEVT_BKW_FORMAT_NAND,        BackgroundFormatNand},
	{NVTEVT_BKW_NUM_RESET,          BackgroundNumReset},
#if (USB_MODE==ENABLE)
#if (USE_DPOF==ENABLE)
	{NVTEVT_BKW_SETDPOF,            BackgroundSetDPOF},
#endif
#endif
	{NVTEVT_BKW_SETPROTECT,         BackgroundSetProtect},
	{NVTEVT_BKW_DUMMY_WAIT,         BackgroundDummyWait},
	{NVTEVT_BKW_FW_UPDATE,          BackgroundFWUpdate},
#if (PHOTO_MODE==ENABLE)
	{NVTEVT_BKW_DZOOM_IN,           BackgroundDZoomIn},
	{NVTEVT_BKW_DZOOM_OUT,          BackgroundDZoomOut},
#endif
	{NVTEVT_BKW_PIM_PROCESS,        BackgroundPIMProcess},
	//#NT#2012/10/30#Calvin Chang#Generate Audio NR pattern by noises of zoom, focus and iris -begin
	{NVTEVT_BKW_ANR_LENS_ACTION,    BackgroundAudiNRLensAction},
	//#NT#2012/10/30#Calvin Chang -end
	{NVTEVT_BKW_WIFI_ON,            BackgroundWiFiOn},
#if(WIFI_AP_FUNC==ENABLE)
	{NVTEVT_BKW_WIFI_CLEARACL,      BackgroundWiFiClearACL},
#endif
	{NVTEVT_BKW_PS_INIT,            BackgroundPSInit},
	{NVTEVT_BKW_STOPREC_PROCESS,    BackgroundStopRecProcess},

	{0,                             0},
};


UINT32 Background_CopyTo(BOOL isCopyToCard, UINT32 copMode)
{
	//#NT#2016/06/27#Lincy Lin -begin
	//#NT#Reduce IPCAM used memory pool size
	UINT32 uiTempBufAddr, uiTempBufSize;
	INT32  uiError_Code = FST_STA_OK;
	//HNVT_STRG pSrcObj, pDstObj, pOriObj;
	DX_HANDLE pSrcDXH = 0, pDstDXH = 0, pOriDXH = 0;
	COPYTO_BYNAME_INFO CopyInfo;
#if USE_FILEDB
	FILEDB_HANDLE fileDbHandle;
	UINT32        fileDbbufSize = 0x200000, fileDbbufAddr, FileNumber;
#endif

	uiTempBufAddr = SxCmd_GetTempMem(POOL_SIZE_APP);
	uiTempBufSize = POOL_SIZE_APP;

	// reserved 2MB for FileDB using
#if USE_FILEDB
	uiTempBufSize -= fileDbbufSize;
	fileDbbufAddr = uiTempBufAddr + uiTempBufSize;
#endif

	//debug_msg("uiTempBufAddr = 0x%x, uiTempBufSize = 0x%x\r\n",uiTempBufAddr,uiTempBufSize);
	//debug_msg("fileDbbufAddr = 0x%x, fileDbbufSize = 0x%x\r\n",fileDbbufAddr,fileDbbufSize);

	UI_SetData(FL_IsCopyToCarding, TRUE);

	if (isCopyToCard) {
		FileSys_GetStrgObj(&pDstDXH);
		pOriDXH = pDstDXH;
		// --> TODO:remove begin
#if (defined(_EMBMEM_NAND_) || defined(_EMBMEM_SPI_NAND_))
		{
			pSrcDXH = Dx_GetObject(DX_CLASS_STORAGE_EXT | USER_DX_TYPE_EMBMEM_FAT);
		}
#else
		//pSrcObj = (HNVT_STRG)sdio2_getStorageObject(STRG_OBJ_FAT1);
		//sdio2_setDetectCardExistHdl(LibFs_CheckCardInserted);
		//sdio2_setDetectCardProtectHdl(SLibFs_CheckCardWP);
#endif
		// --> TODO:remove end
	} else {
		//FileSys_GetStrgObj(&pOriObj);
		FileSys_GetStrgObj(&pOriDXH);
		//pSrcObj = pOriObj;
		pSrcDXH = pOriDXH;
		// --> TODO:remove begin
#if (defined(_EMBMEM_NAND_) || defined(_EMBMEM_SPI_NAND_))
		{
			pDstDXH = Dx_GetObject(DX_CLASS_STORAGE_EXT | USER_DX_TYPE_EMBMEM_FAT);
		}
#else
		//pDstObj = (HNVT_STRG)sdio2_getStorageObject(STRG_OBJ_FAT1);
		//sdio2_setDetectCardExistHdl(LibFs_CheckCardInserted);
		//sdio2_setDetectCardProtectHdl(SLibFs_CheckCardWP);
#endif
	}
	memset(&CopyInfo, 0, sizeof(CopyInfo));
	CopyInfo.pSrcStrg = pSrcDXH;
	CopyInfo.pDstStrg = pDstDXH;
	CopyInfo.pBuf = (char *)uiTempBufAddr;
	CopyInfo.uiBufSize = uiTempBufSize;

#if (USE_FILEDB == ENABLE)
	{
		if (pOriDXH != pSrcDXH) {
			if (FileSys_ChangeDisk(pSrcDXH) != FSS_OK) {
				DBG_ERR("ChangeDisk fail\r\n");
			}
		}
		//  create fileDB for copy
		{
			FILEDB_INIT_OBJ   FilDBInitObj = {0};
			CHAR              rootPath[20] = "A:\\";

			FilDBInitObj.rootPath = rootPath;
			FilDBInitObj.bIsRecursive = TRUE;
			FilDBInitObj.bIsCyclic = TRUE;
			FilDBInitObj.bIsMoveToLastFile = TRUE;
			FilDBInitObj.bIsSupportLongName = TRUE;
			FilDBInitObj.bIsDCFFileOnly = FALSE;
			FilDBInitObj.bIsChkHasFile = FALSE;
			FilDBInitObj.u32MaxFilePathLen = 60;
			FilDBInitObj.u32MaxFileNum = 20000;
			FilDBInitObj.fileFilter = FILEDB_FMT_JPG | FILEDB_FMT_AVI | FILEDB_FMT_MOV | FILEDB_FMT_MP4 | FILEDB_FMT_TS;
			FilDBInitObj.u32MemAddr = dma_getCacheAddr(fileDbbufAddr);
			FilDBInitObj.u32MemSize = fileDbbufSize;
			fileDbHandle = FileDB_Create(&FilDBInitObj);
		}
		// copy file one by one
		{
			UINT32                   i = 0, CurrIndex;
			PFILEDB_FILE_ATTR        pFileAttr = 0;

			CurrIndex = FileDB_GetCurrFileIndex(fileDbHandle);
			FileNumber = FileDB_GetTotalFileNum(fileDbHandle);
			for (i = 0; i < FileNumber; i++) {
				pFileAttr = FileDB_SearhFile(fileDbHandle, i);
				if (!pFileAttr) {
					DBG_ERR("no pFileAttr\r\n");
					break;
				}
				CopyInfo.pSrcPath = pFileAttr->filePath;
				CopyInfo.pDstPath = pFileAttr->filePath;
				debug_msg("Copy file %s\r\n", pFileAttr->filePath);
				uiError_Code = FileSys_CopyToByName(&CopyInfo);
				if (uiError_Code != FST_STA_OK) {
					DBG_ERR("err %d\r\n", uiError_Code);
					break;
				}
			}
			if (FileNumber) {
				FileDB_SearhFile(fileDbHandle, CurrIndex);
			}
		}
		FileDB_Release(fileDbHandle);
	}
#endif
	//#NT#2016/06/27#Lincy Lin -end
	if (pOriDXH != pSrcDXH) {
		if (FileSys_ChangeDisk(pOriDXH) != FSS_OK) {
			DBG_ERR("ChangeDisk fail\r\n");
		}
	}

#if (USE_FILEDB == ENABLE)
	{
		UINT32  curIndex;

		fileDbHandle = 0;
		FileDB_Refresh(fileDbHandle);
		FileNumber = FileDB_GetTotalFileNum(fileDbHandle);
		curIndex = FileNumber - 1;
		if (FileNumber) {
			FileDB_SearhFile(fileDbHandle, curIndex);
		}
#if (PLAY_MODE==ENABLE)
		if (System_GetState(SYS_STATE_CURRMODE) == PRIMARY_MODE_PLAYBACK) {
			PB_OpenSpecFileBySeq(curIndex + 1, TRUE);
			PB_WaitCommandFinish(PB_WAIT_INFINITE);
		}
#endif
	}
#endif
#if (USE_DCF == ENABLE)
	{
		DCF_Refresh();
		DCF_SetCurIndex(DCF_GetDBInfo(DCF_INFO_TOL_FILE_COUNT));

#if (PLAY_MODE==ENABLE)
		if (System_GetState(SYS_STATE_CURRMODE) == PRIMARY_MODE_PLAYBACK) {
			PB_OpenSpecFileBySeq(DCF_GetDBInfo(DCF_INFO_CUR_INDEX), TRUE);
			PB_WaitCommandFinish(PB_WAIT_INFINITE);
		}
#endif
	}
#endif

	UI_SetData(FL_IsCopyToCarding, FALSE);
    SxCmd_RelTempMem(uiTempBufAddr);
	return uiError_Code;
}


UINT32 BackgroundDelAll(void)
{
	INT32 ret;
#if (PLAY_MODE==ENABLE)
	if (System_GetState(SYS_STATE_CURRMODE) == PRIMARY_MODE_PLAYBACK) {
		UIPlay_Delete(PB_DELETE_ALL);
		ret = TRUE;//dummy return value
	} else
#endif
	{
		ret = Background_DeleteAll();
	}
	return (UINT32)ret;
}

//for any mode delete all,
//UI flow need to update orgianl FileDB data,check FL_DeleteAllFlag
UINT32 Background_DeleteAll(void)
{
#if (USE_FILEDB == ENABLE)
	FILEDB_HANDLE      FileDBHdl = 0;
	PFILEDB_FILE_ATTR  FileAttr = NULL;
	INT32              ret = E_OK;
	INT32              FileNum, i;
	PFILEDB_INIT_OBJ   pFDBInitObj = &gBKFDBInitObj;

	UI_SetData(FL_DeleteAllFlag, TRUE);

	pFDBInitObj->u32MemAddr = SxCmd_GetTempMem(POOL_SIZE_FILEDB);

	if (!pFDBInitObj->u32MemAddr) {
		return FALSE;
	}

	pFDBInitObj->u32MemSize = POOL_SIZE_FILEDB;
	FileDBHdl = FileDB_Create(pFDBInitObj);

	FileNum = FileDB_GetTotalFileNum(FileDBHdl);
	for (i = FileNum - 1; i >= 0; i--) {
		FileAttr = FileDB_SearhFile(FileDBHdl, i);
		if (FileAttr) {
			if (M_IsReadOnly(FileAttr->attrib)) {
				continue;
			}
			ret = FileSys_DeleteFile(FileAttr->filePath);
			DBG_IND("i = %04d path=%s\r\n", i, FileAttr->filePath);
			if (ret != FST_STA_OK) {
				goto FDB_Delete_Err;
			} else {
				FileDB_DeleteFile(FileDBHdl, i);
			}
		} else {
			goto FDB_Delete_Err;
		}
	}
	FileDB_Release(FileDBHdl);
    SxCmd_RelTempMem(pFDBInitObj->u32MemAddr);
	return (UINT32)ret;

FDB_Delete_Err:
	DBG_ERR("FDB_Delete err\r\n");
	FileDB_Release(FileDBHdl);
    SxCmd_RelTempMem(pFDBInitObj->u32MemAddr);
	return E_SYS;
#else
	DBG_ERR("not support!\r\n");
	return E_SYS;
#endif
}
UINT32 BackgroundCopy2Card(void)
{
	return Background_CopyTo(TRUE, PB_COPY2CARD_NEW_DIR);
}

UINT32 BackgroundCopy2Nand(void)
{
	return Background_CopyTo(FALSE, PB_COPY2CARD_NEW_DIR);
}

#if(COPYCARD2CARD_FUNCTION == ENABLE)
//#NT#2016/10/18#Niven Cho -begin
//#NT#mount sd-2 as "B:\\"
typedef enum {
	WARN_REMOVE_USB = 0,
	WARN_CONNECT_FAIL,
	WARN_UNKNOWN_FILE,
	WARN_NO_PICTURE,
	WARN_LENS_ERROR,
	WARN_CARD_ERROR,
	WARN_BUILT_IN_MEMORY_ERROR,
	WARN_CARD_UNFORMAT,
	WARN_WRITE_PROTECT,
	WARN_CARD_FULL,
	WARN_MEM_FULL,
	WARN_FILE_PROTECT,
	WARN_DEL_PROTECT,
	WARN_BATT_EMPTY,
	WARN_PAN_ERROR,
	WARN_COPY_CARD_FULL,
	WARN_COPY_CARD_PROTECT,
	WARN_COPY_NO_PICTURE,
	WRN_FW_UPDATE_COMPLETE,
	WARN_RECORD_SLOW_MEDIA,
	WRN_FW_UPDATING,
	WRN_FW_CHECK,
	WRN_NOT_DETECT_REDEYE,
	WRN_EXCEED_MAX_FOLDER,
	WRN_TOO_MANY_PIC,
	WARN_TYPE_MAX
} WARN_TYPE;
//#NT#2016/10/18#Niven Cho -end
UINT32 Background_CopyCardToCard(BOOL IsPrimaryCard, UINT32 copMode)
{
	UINT32 uiTempBufAddr, uiTempBufSize, FileNumber;
	INT32  uiError_Code = FST_STA_OK;
	//HNVT_STRG pSrcObj, pDstObj, pOriObj;
	DX_HANDLE pSrcDXH, pDstDXH, pOriDXH;
	COPYTO_BYNAME_INFO CopyInfo;
#if USE_FILEDB
	FILEDB_HANDLE fileDbHandle, fileDbbufSize = 0x200000, fileDbbufAddr;
#endif
#if (USE_DCF == ENABLE)
	UINT32 uiThisFileFormat;
	UINT32 DirID, FileID;
	UINT32 NewDirID, NewFileID, i;
	CHAR   srcPath[DCF_FULL_FILE_PATH_LEN], dstPath[DCF_FULL_FILE_PATH_LEN];
#endif


	uiTempBufAddr = SxCmd_GetTempMem(POOL_SIZE_APP);
	uiTempBufSize = POOL_SIZE_APP;

	// reserved 2MB for FileDB using
#if USE_FILEDB
	uiTempBufSize -= fileDbbufSize;
	fileDbbufAddr = uiTempBufAddr + uiTempBufSize;
#endif

	UI_SetData(FL_IsCopyToCarding, TRUE);

	if (IsPrimaryCard) {
		FileSys_GetStrgObj(&pDstDXH);
		pOriDXH = pDstDXH;
		{
			pSrcDXH = Dx_GetObject(DX_CLASS_STORAGE_EXT | DX_TYPE_CARD2);
		}
	} else {
		FileSys_GetStrgObj(&pOriDXH);
		pSrcDXH = pOriDXH;
		{
			pDstDXH = Dx_GetObject(DX_CLASS_STORAGE_EXT | DX_TYPE_CARD2);
		}
	}

	memset(&CopyInfo, 0, sizeof(CopyInfo));
	CopyInfo.pSrcStrg = pSrcDXH;
	CopyInfo.pDstStrg = pDstDXH;
	CopyInfo.pBuf = (char *)uiTempBufAddr;
	CopyInfo.uiBufSize = uiTempBufSize;

#if (USE_FILEDB == ENABLE)
	{
		if (pOriDXH != pSrcDXH) {
			if (FileSys_ChangeDisk(pSrcDXH) != FSS_OK) {
				DBG_ERR("ChangeDisk fail\r\n");
			}
		}
		//  create fileDB for copy
		{
			FILEDB_INIT_OBJ   FilDBInitObj = {0};
			CHAR              rootPath[20] = "A:\\";

			FilDBInitObj.rootPath = rootPath;
			FilDBInitObj.bIsRecursive = TRUE;
			FilDBInitObj.bIsCyclic = TRUE;
			FilDBInitObj.bIsMoveToLastFile = TRUE;
			FilDBInitObj.bIsSupportLongName = TRUE;
			FilDBInitObj.bIsDCFFileOnly = FALSE;
			FilDBInitObj.bIsChkHasFile = FALSE;
			FilDBInitObj.u32MaxFilePathLen = 60;
			FilDBInitObj.u32MaxFileNum = 20000;
			FilDBInitObj.fileFilter = FILEDB_FMT_JPG | FILEDB_FMT_AVI | FILEDB_FMT_MOV | FILEDB_FMT_MP4 | FILEDB_FMT_TS;
			FilDBInitObj.u32MemAddr = dma_getCacheAddr(fileDbbufAddr);
			FilDBInitObj.u32MemSize = fileDbbufSize;
			fileDbHandle = FileDB_Create(&FilDBInitObj);
		}
		// copy file one by one
		{
			UINT32                   i = 0, CurrIndex;
			PFILEDB_FILE_ATTR        pFileAttr;

			CurrIndex = FileDB_GetCurrFileIndex(fileDbHandle);
			FileNumber = FileDB_GetTotalFileNum(fileDbHandle);
			for (i = 0; i < FileNumber; i++) {
				pFileAttr = FileDB_SearhFile(fileDbHandle, i);
				CopyInfo.pSrcPath = pFileAttr->filePath;
				CopyInfo.pDstPath = pFileAttr->filePath;
				debug_msg("Copy file %s\r\n", pFileAttr->filePath);
				uiError_Code = FileSys_CopyToByName(&CopyInfo);

				if (uiError_Code != FST_STA_OK) {
					if (uiError_Code == FST_STA_NOFREE_SPACE) {
						uiError_Code = WARN_CARD_FULL;
					}
					break;
				}
			}
			if (FileNumber) {
				FileDB_SearhFile(fileDbHandle, CurrIndex);
			}
		}
		FileDB_Release(fileDbHandle);
	}
#endif
#if (USE_DCF == ENABLE)
	{
		//if (pOriObj==pSrcObj)
		if (pOriDXH == pSrcDXH) {
			// get copy file number
			FileNumber = DCF_GetDBInfo(DCF_INFO_TOL_FILE_COUNT);

			// get dst max ID
			//FileSys_ChangeDisk(pDstObj);
			if (FileSys_ChangeDisk(pDstDXH) != FSS_OK) {
				DBG_ERR("ChangeDisk fail\r\n");
			}
			DCF_ScanObj();
			DCF_GetNextID(&NewDirID, &NewFileID);
			//FileSys_ChangeDisk(pOriObj);
			if (FileSys_ChangeDisk(pOriDXH) != FSS_OK) {
				DBG_ERR("ChangeDisk fail\r\n");
			}
			DCF_ScanObj();
		}
		// pOriObj == pDstObj
		else {
			// get dst max ID
#if 0
			DCF_GetNextID(&NewDirID, &NewFileID);
#else // Rescan DCF if not scanned yet.
			if (!DCF_GetNextID(&NewDirID, &NewFileID)) {
				DCF_ScanObj();
				DCF_GetNextID(&NewDirID, &NewFileID);
			}
#endif
			// get copy file number
			//FileSys_ChangeDisk(pSrcObj);
			if (FileSys_ChangeDisk(pSrcDXH) != FSS_OK) {
				DBG_ERR("ChangeDisk fail\r\n");
			}
			DCF_ScanObj();

			FileNumber = DCF_GetDBInfo(DCF_INFO_TOL_FILE_COUNT);
		}
		if (NewDirID == ERR_DCF_DIR_NUM && NewFileID == ERR_DCF_FILE_NUM) {
			uiError_Code = WRN_EXCEED_MAX_FOLDER;
			goto L_CopyToCard_Exit;
		}
		switch (copMode) {
		case PB_COPY2CARD_NEW_DIR:
			if (NewFileID != MIN_DCF_FILE_NUM) {
				NewDirID++;
				NewFileID = MIN_DCF_FILE_NUM;
				if (NewDirID > MAX_DCF_DIR_NUM) {
					uiError_Code = WRN_EXCEED_MAX_FOLDER;
					goto L_CopyToCard_Exit;
				}
			}
		// continue run append code should not add break here
		case PB_COPY2CARD_APPEND:
			for (i = 1; i <= FileNumber; i++) {
				// get src path
				DCF_SetCurIndex(i);
				DCF_GetObjInfo(i, &DirID, &FileID, &uiThisFileFormat);
				DCF_GetObjPath(i, uiThisFileFormat, srcPath);
				CopyInfo.pSrcPath = srcPath;

				// get dst path
				DCF_MakeObjPath(NewDirID, NewFileID, uiThisFileFormat, dstPath);
				CopyInfo.pDstPath = dstPath;
				uiError_Code = FileSys_CopyToByName(&CopyInfo);
				if (uiError_Code != FST_STA_OK) {
					if (uiError_Code == FST_STA_NOFREE_SPACE) {
						uiError_Code = WARN_CARD_FULL;
					}
					break;
				}
				NewFileID++;
				if (NewFileID > MAX_DCF_FILE_NUM) {
					NewFileID = 1;
					NewDirID++;
					if (NewDirID > MAX_DCF_DIR_NUM) {
						uiError_Code = WRN_EXCEED_MAX_FOLDER;
						goto L_CopyToCard_Exit;
					}
				}
			}
			break;

		case PB_COPY2CARD_NEW_DIR_ORG_FILE: {
				UINT32 tmpDirID  = 0;

				// Fix bug 0050970: Show no picture warning message if no file in Nand when doing copy to card function.
				if (!FileNumber) {
					uiError_Code = WARN_COPY_NO_PICTURE ;
					goto L_CopyToCard_Exit;
				}

				for (i = 1; i <= FileNumber; i++) {
					// get src path
					DCF_SetCurIndex(i);
					DCF_GetObjInfo(i, &DirID, &FileID, &uiThisFileFormat);
					DCF_GetObjPath(i, uiThisFileFormat, srcPath);
					CopyInfo.pSrcPath = srcPath;

					// check if need to new dir
					if (tmpDirID != DirID) {
						tmpDirID = DirID;
						NewDirID++;
						if (NewDirID > MAX_DCF_DIR_NUM) {
							uiError_Code = WRN_EXCEED_MAX_FOLDER;
							goto L_CopyToCard_Exit;
						}
					}

					// get dst path
					DCF_MakeObjPath(NewDirID, FileID, uiThisFileFormat, dstPath);
					CopyInfo.pDstPath = dstPath;
					uiError_Code = FileSys_CopyToByName(&CopyInfo);
					if (uiError_Code != FST_STA_OK) {
						if (uiError_Code == FST_STA_NOFREE_SPACE) {
							uiError_Code = WARN_CARD_FULL;
						}

						break;
					}
				}
			}
			break;
		case PB_COPY2CARD_NO_RENAME:

		default:
			debug_msg("Not Support Copy command %d\r\n", copMode);
			break;
		}
	}
L_CopyToCard_Exit:
#endif

	if (pOriDXH != pSrcDXH) {
		if (FileSys_ChangeDisk(pOriDXH) != FSS_OK) {
			DBG_ERR("ChangeDisk fail\r\n");
		}
	}

#if (USE_FILEDB == ENABLE)
	{
		UINT32  curIndex;

		fileDbHandle = 0;
		FileDB_Refresh(fileDbHandle);
		FileNumber = FileDB_GetTotalFileNum(fileDbHandle);

		curIndex = FileNumber - 1;
		if (FileNumber) {
			FileDB_SearhFile(fileDbHandle, curIndex);
		}
#if (PLAY_MODE==ENABLE)
		if (System_GetState(SYS_STATE_CURRMODE) == PRIMARY_MODE_PLAYBACK) {
			PB_OpenSpecFileBySeq(curIndex + 1, TRUE);
			PB_WaitCommandFinish(PB_WAIT_INFINITE);
		}
#endif
	}
#endif
#if (USE_DCF == ENABLE)
	{
		DCF_Refresh();
		DCF_SetCurIndex(DCF_GetDBInfo(DCF_INFO_TOL_FILE_COUNT));
#if (PLAY_MODE==ENABLE)
		if (System_GetState(SYS_STATE_CURRMODE) == PRIMARY_MODE_PLAYBACK) {
			PB_OpenSpecFileBySeq(DCF_GetDBInfo(DCF_INFO_CUR_INDEX), TRUE);
			PB_WaitCommandFinish(PB_WAIT_INFINITE);
		}
#endif
	}
#endif

	UI_SetData(FL_IsCopyToCarding, FALSE);
	SxCmd_RelTempMem(uiTempBufAddr);
	return uiError_Code;
}

UINT32 BackgroundCopyCard1ToCard2(void)
{
	return Background_CopyCardToCard(FALSE, PB_COPY2CARD_NEW_DIR);
}

UINT32 BackgroundCopyCard2ToCard1(void)
{
	return Background_CopyCardToCard(TRUE, PB_COPY2CARD_NEW_DIR);
}
#endif


UINT32 BackgroundFormat(void)
{
	int ret;

	DX_HANDLE   pStrgDXH = 0;
	FileSys_GetStrgObj(&pStrgDXH);
	//#NT#2016/05/30#Lincy Lin -begin
	//#NT#Support logfile function
#if (LOGFILE_FUNC==ENABLE)
	LogFile_Suspend();
#endif
	//#NT#2016/05/30#Lincy Lin -end
	ret = FileSys_FormatDisk(pStrgDXH, FALSE);
	DBG_DUMP("FormatDisk ret=%d\r\n", ret);
	if (ret == FST_STA_OK) {
		// reset file ID (for FileDB)
#if USE_FILEDB
		if (UI_GetData(FL_IsUseFileDB)) {
			Ux_SendEvent(&UISetupObjCtrl, NVTEVT_FILEID_RESET, 0);
		}
#endif
#if (USE_DCF == ENABLE)
		//reset next id 100 , 1
		DCF_SetNextID(MIN_DCF_DIR_NUM, MIN_DCF_FILE_NUM);
		UI_SetData(FL_DCF_DIR_ID, MIN_DCF_DIR_NUM);
		UI_SetData(FL_DCF_FILE_ID, MIN_DCF_FILE_NUM);

		UI_SetData(FL_IsDCIMReadOnly, FALSE);
#endif
#if (PLAY_MODE==ENABLE)
		if (System_GetState(SYS_STATE_CURRMODE) == PRIMARY_MODE_PLAYBACK) {
			UIPlay_PlaySingle(PB_SINGLE_CURR);
		}
#endif
		//#NT#2016/05/30#Lincy Lin -begin
		//#NT#Support logfile function
#if (LOGFILE_FUNC==ENABLE)
		LogFile_ReOpen();
#endif
		//#NT#2016/05/30#Lincy Lin -end
	}

	return (UINT32)ret;
}

UINT32 BackgroundFormatCard(void)
{
	int ret;

	//DX_HANDLE pStrgDev = Dx_GetObject(DX_CLASS_STORAGE_EXT|DX_TYPE_CARD1);
	//UINT32 hStrgObj = Dx_Getcaps(pStrgDev, STORAGE_CAPS_HANDLE, 0);
	//ret = FileSys_FormatDisk((HNVT_STRG)hStrgObj, FALSE);
	DX_HANDLE pStrgDXH = Dx_GetObject(DX_CLASS_STORAGE_EXT | DX_TYPE_CARD1);
	//#NT#2016/05/30#Lincy Lin -begin
	//#NT#Support logfile function
#if (LOGFILE_FUNC==ENABLE)
	LogFile_Suspend();
#endif
	//#NT#2016/05/30#Lincy Lin -end
	ret = FileSys_FormatDisk(pStrgDXH, FALSE);
	DBG_DUMP("FormatDisk ret=%d\r\n", ret);
    exam_msg("FormatDisk ret=%d\r\n", ret);
	if (ret == FST_STA_OK) {
		// reset file ID (for FileDB)
#if USE_FILEDB
		if (UI_GetData(FL_IsUseFileDB)) {
			Ux_SendEvent(&UISetupObjCtrl, NVTEVT_FILEID_RESET, 0);
		}
#endif
#if (USE_DCF == ENABLE)
		//reset next id 100 , 1
		DCF_SetNextID(MIN_DCF_DIR_NUM, MIN_DCF_FILE_NUM);
		UI_SetData(FL_DCF_DIR_ID, MIN_DCF_DIR_NUM);
		UI_SetData(FL_DCF_FILE_ID, MIN_DCF_FILE_NUM);
		UI_SetData(FL_IsDCIMReadOnly, FALSE);
#endif
#if (PLAY_MODE==ENABLE)
		if (System_GetState(SYS_STATE_CURRMODE) == PRIMARY_MODE_PLAYBACK) {
			UIPlay_PlaySingle(PB_SINGLE_CURR);
		}
#endif
		//#NT#2016/05/30#Lincy Lin -begin
		//#NT#Support logfile function
#if (LOGFILE_FUNC==ENABLE)
		LogFile_ReOpen();
#endif
		//#NT#2016/05/30#Lincy Lin -end
	}
	return (UINT32)ret;
}
UINT32 BackgroundFormatNand(void)
{
	int ret;

	//DX_HANDLE pStrgDev = Dx_GetObject(DX_CLASS_STORAGE_EXT|DX_TYPE_EMBMEM1);
	//UINT32 hStrgObj = Dx_Getcaps(pStrgDev, STORAGE_CAPS_HANDLE, 0);
	//ret = FileSys_FormatDisk((HNVT_STRG)hStrgObj, FALSE);
	DX_HANDLE pStrgDXH = Dx_GetObject(DX_CLASS_STORAGE_EXT | USER_DX_TYPE_EMBMEM_FAT);
	//#NT#2016/05/30#Lincy Lin -begin
	//#NT#Support logfile function
#if (LOGFILE_FUNC==ENABLE)
	LogFile_Suspend();
#endif
	//#NT#2016/05/30#Lincy Lin -end
	ret = FileSys_FormatDisk(pStrgDXH, FALSE);
	DBG_DUMP("FormatDisk ret=%d\r\n", ret);

	if (ret == FST_STA_OK) {
		// reset file ID (for FileDB)
#if USE_FILEDB
		if (UI_GetData(FL_IsUseFileDB)) {
			Ux_SendEvent(&UISetupObjCtrl, NVTEVT_FILEID_RESET, 0);
		}
#endif
#if (USE_DCF == ENABLE)
		//reset next id 100 , 1
		DCF_SetNextID(MIN_DCF_DIR_NUM, MIN_DCF_FILE_NUM);
		UI_SetData(FL_DCF_DIR_ID, MIN_DCF_DIR_NUM);
		UI_SetData(FL_DCF_FILE_ID, MIN_DCF_FILE_NUM);
		UI_SetData(FL_IsDCIMReadOnly, FALSE);
#endif
#if (PLAY_MODE==ENABLE)
		if (System_GetState(SYS_STATE_CURRMODE) == PRIMARY_MODE_PLAYBACK) {
			UIPlay_PlaySingle(PB_SINGLE_CURR);
		}
#endif
		//#NT#2016/05/30#Lincy Lin -begin
		//#NT#Support logfile function
#if (LOGFILE_FUNC==ENABLE)
		LogFile_ReOpen();
#endif
		//#NT#2016/05/30#Lincy Lin -end
	}
	return (UINT32)ret;
}

UINT32 BackgroundNumReset(void)
{
#if (USE_DCF == ENABLE)
	UINT32 fid = 0, did = 0;

	//#Get DCF Folder/FIle ID
	DCF_GetNextID((UINT32 *)&did, (UINT32 *)&fid);
	debug_err(("SetupExe_OnNumReset(), DirNum = %d, FileNum = %d\r\n", did, fid));

	if (did < MAX_DCF_DIR_NUM) {
		//reset
		DCF_SetNextID(did + 1, 1);

		//#Get DCF Folder/FIle ID
		DCF_GetNextID((UINT32 *)&did, (UINT32 *)&fid);

		SwTimer_DelayMs(0x2000); //for ui show wait window for 2 second
		debug_err(("SetupExe_OnNumReset(), DirNum = %d, FileNum = %d\r\n", did, fid));

		return TRUE;
	} else {
		debug_err(("did over %d\r\n", MAX_DCF_DIR_NUM));
		return FALSE;
	}
#else
	return TRUE;
#endif
}
#if (USE_DPOF == ENABLE)
UINT32 BackgroundSetDPOF(void)
{
	INT32 ret;

	if (g_uiDpofOPMode == PLAYDPOF_SETONE)
		//Ux_SendEvent(&UIPlayObjCtrl,NVTEVT_SET_DPOFONE,2, g_uiDpofPrtNum, g_uiDpofDateOn);
	{
		DpofVendor_SetDPOFQty(DPOF_OP_SET_CURR, g_uiDpofPrtNum, g_uiDpofDateOn);
	} else {
		//#NT#2010/05/31#[0010985]Lily Kao -begin
		//#NT#DPOF-All from the current or the first picture
		//Ux_SendEvent(&UIPlayObjCtrl,NVTEVT_SET_DPOFALL_FROM_FIRST,2, g_uiDpofPrtNum, g_uiDpofDateOn);
		//#NT#2010/05/31#[0010985]Lily Kao -end
		DpofVendor_SetDPOFQty(DPOF_OP_SET_ALL, g_uiDpofPrtNum, g_uiDpofDateOn);
	}
	ret = TRUE;//dummy return value
	return (UINT32)ret;
}
#endif
UINT32 BackgroundSetProtect(void)
{
#if (PLAY_MODE==ENABLE)
	if (System_GetState(SYS_STATE_CURRMODE) == PRIMARY_MODE_PLAYBACK) {
		INT32 ret;
		UIPlay_Protect(g_uiProtectType);
		ret = TRUE;//dummy return value
		return (UINT32)ret;
	} else
#endif
	{
		if (g_uiProtectType == SETUP_PROTECT_ALL) {
			return Background_ProtectAll(TRUE);
		} else {
			return Background_ProtectAll(FALSE);
		}
	}
}

UINT32 Background_ProtectAll(BOOL bLock)
{
#if (USE_FILEDB == ENABLE)
	FILEDB_HANDLE      FileDBHdl = 0;
	PFILEDB_FILE_ATTR  FileAttr = NULL;
	INT32              ret;
	INT32              FileNum, i;
	PFILEDB_INIT_OBJ   pFDBInitObj = &gBKFDBInitObj;

	pFDBInitObj->u32MemAddr = SxCmd_GetTempMem(POOL_SIZE_FILEDB);

	if (!pFDBInitObj->u32MemAddr) {
		return FALSE;
	}

	pFDBInitObj->u32MemSize = POOL_SIZE_FILEDB;
	FileDBHdl = FileDB_Create(pFDBInitObj);
	//FileDB_DumpInfo(FileDBHdl);
	FileNum = FileDB_GetTotalFileNum(FileDBHdl);

	for (i = 0; i < FileNum; i++) {
		FileAttr = FileDB_SearhFile(FileDBHdl, i);
		if (FileAttr) {
			ret = FileSys_SetAttrib(FileAttr->filePath, FST_ATTRIB_READONLY, bLock);
			if (ret == FST_STA_OK) {
				if (bLock) {
					FileAttr->attrib |= FS_ATTRIB_READ;
				} else {
					FileAttr->attrib &= (~FS_ATTRIB_READ);
				}
			} else {
				goto FDB_Protect_Err;
			}
		} else {
			goto FDB_Protect_Err;
		}
	}
	FileDB_Release(FileDBHdl);
    SxCmd_RelTempMem(pFDBInitObj->u32MemAddr);

	return E_OK;

FDB_Protect_Err:
	DBG_ERR("bLock=%d err\r\n", bLock);
	FileDB_Release(FileDBHdl);
    SxCmd_RelTempMem(pFDBInitObj->u32MemAddr);

	return FALSE;
#else
	DBG_ERR("not support!\r\n");
	return FALSE;
#endif
}

UINT32 BackgroundDummyWait_Stop(void)
{
	//debug_msg("BackgroundDummyWait_Stop:%d,%d\r\n", gUIStopBckGndDummyWait, g_uiWaitTime);
	if (g_uiWaitTime) {
		gUIStopBckGndDummyWait = TRUE;
	}
	return g_uiWaitTime;
}
//#NT#2010/06/23#Lily Kao -begin
UINT32 BackgroundDummyWait(void)
{
	UINT32 i = g_uiWaitTime;

	gUIStopBckGndDummyWait = FALSE;
	while (i) {
		//debug_msg("BackgroundDummyWait:%d,%d,%d\r\n", gUIStopBckGndDummyWait, g_uiWaitTime,i);
		Delay_DelayMs(1);
		i--;
		if (gUIStopBckGndDummyWait == TRUE) {
			//debug_msg("BackgroundDummyWait break:%d,%d\r\n", gUIStopBckGndDummyWait, g_uiWaitTime);
			gUIStopBckGndDummyWait = FALSE;
			g_uiWaitTime = 0;
			break;
		}
	}
	return 1;
}

extern UINT32   System_OnStrg_UploadFW(UINT32 uiStorage);

static UINT32 BackgroundFWUpdate(void)
{
#if (IPCAM_FUNC != ENABLE)
	UINT32 result = 0;
	INT curMode = System_GetState(SYS_STATE_CURRMODE) ;

	System_ChangeSubMode(SYS_SUBMODE_UPDFW);
	Ux_SendEvent(0, NVTEVT_SYSTEM_MODE, 1, PRIMARY_MODE_UPDFW);
	result = System_OnStrg_UploadFW(0);

	if (result != 0) {
		Ux_SendEvent(0, NVTEVT_SYSTEM_MODE, 1, curMode);
		System_ChangeSubMode(SYS_SUBMODE_NORMAL);

	}
	return result;
#else
	debug_msg("_TODO -> %s:%d, task(%x,%s) caller(%pF)\r\n", __FUNCTION__, __LINE__,
			  OS_GetTaskID(), OS_GetTaskName(), __builtin_return_address(0));

	return 0;
#endif
}

#if (PHOTO_MODE==ENABLE)
extern void PhotoExe_DZoomInBK(UINT32 *pIsDzoomStop);
extern void PhotoExe_DZoomOutBK(UINT32 *pIsDzoomStop);

static UINT32 BackgroundDZoomIn(void)
{
	PhotoExe_DZoomInBK(g_pDzoomStop);
	return TRUE;
}

static UINT32 BackgroundDZoomOut(void)
{
	PhotoExe_DZoomOutBK(g_pDzoomStop);
	return TRUE;
}
#endif

//#NT#2012/10/23#Hideo Lin -begin
//#NT#For picture in movie processing
static UINT32 BackgroundPIMProcess(void)
{
	Ux_SendEvent(&CustomMovieObjCtrl, NVTEVT_EXE_MOVIE_REC_STOP, 0);
	return NVTRET_OK;
}
//#NT#2012/10/23#Hideo Lin -end

//#NT#2012/10/30#Calvin Chang#Generate Audio NR pattern by noises of zoom, focus and iris -begin
static UINT32 BackgroundAudiNRLensAction(void)
{
#if (LENS_FUNCTION == ENABLE)
	UINT32 i, uiLoop;

	Lens_ZoomSetSpeed(ZOOM_SPEED_LOW);

	Delay_DelayMs(3000); // Wait 2 seconds

	// 1. Zoom In/out + Focus in 3 seconds
	debug_err(("Zoom In/out + Focus in 3 seconds!\r\n"));
	uiLoop = 1;
	for (i = 0; i < uiLoop; i++) {
		Lens_ZoomIn();
		Lens_FocusDoAction(FOCUS_PREEXC | FOCUS_RUN | FOCUS_POSTEXC, 50); // position = 45 ~ 600;
		Lens_FocusDoAction(FOCUS_PREEXC | FOCUS_RUN | FOCUS_POSTEXC, 600); // position = 45 ~ 600;
		Delay_DelayMs(150);
		Lens_ZoomStop();
		Delay_DelayMs(25);
		Lens_ZoomOut();
		Lens_FocusDoAction(FOCUS_PREEXC | FOCUS_RUN | FOCUS_POSTEXC, 50); // position = 45 ~ 600;
		Lens_FocusDoAction(FOCUS_PREEXC | FOCUS_RUN | FOCUS_POSTEXC, 600); // position = 45 ~ 600;
		Delay_DelayMs(150);
		Lens_ZoomStop();
		Delay_DelayMs(25);
	}

	Delay_DelayMs(6000); // Wait 6 seconds

	//2. Focus Forward N steps in 3 seconds
	debug_err(("Focus Forward N steps in 3 seconds!\r\n"));
	uiLoop = 6;
	for (i = 0; i < uiLoop; i++) {
		Lens_FocusDoAction(FOCUS_PREEXC | FOCUS_RUN | FOCUS_POSTEXC, 50); // position = 45 ~ 600;
		Lens_FocusDoAction(FOCUS_PREEXC | FOCUS_RUN | FOCUS_POSTEXC, 450); // position = 45 ~ 600;

	}

	Delay_DelayMs(7000); // Wait 7 seconds

	//3. Iris switch in 3 seconds
	debug_err(("Iris switch in 3 seconds!\r\n"));
	uiLoop = 15;
	for (i = 0; i < uiLoop; i++) {
		Lens_ApertureSetPosition(IRIS_POS_SMALL);
		Delay_DelayMs(50);
		Lens_ApertureSetPosition(IRIS_POS_BIG);
		Delay_DelayMs(50);
	}

	Delay_DelayMs(4000);

	Lens_ZoomSetSpeed(ZOOM_SPEED_HIGH);

	// Record stop!
	Ux_SendEvent(&CustomMovieObjCtrl, NVTEVT_EXE_MOVIE_REC_STOP, 0);

	debug_err(("Record Stop!\r\n"));
#endif

	return NVTRET_OK;
}
//#NT#2012/10/30#Calvin Chang -end
static UINT32 BackgroundWiFiOn(void)
{
	Ux_SendEvent(0, NVTEVT_EXE_WIFI_START, 0);
	return NVTRET_OK;
}
#if(WIFI_AP_FUNC==ENABLE)

static UINT32 BackgroundWiFiClearACL(void)
{
	Delay_DelayMs(ACL_TIME);
	UINet_ClearACLTable();
	return NVTRET_OK;
}
#endif
static UINT32 BackgroundPSInit(void)
{
	System_OnFlashInit_PS();
        return NVTRET_OK;
}
static UINT32 BackgroundStopRecProcess(void)
{
	//debug_err(("StopRec\r\n\n\n\n\n"));
	FlowMovie_StopRec();
        return NVTRET_OK;
}

void AppBKW_SetData(BKW_DATA_SET attribute, UINT32 value)
{
	switch (attribute) {
	case BKW_DPOF_MODE:
		g_uiDpofOPMode = value;
		break;
	case BKW_DPOF_NUM:
		g_uiDpofPrtNum = value;
		break;
	case BKW_DPOF_DATE:
		g_uiDpofDateOn = value;
		break;
	case BKW_PROTECT_TYPE:
		g_uiProtectType = value;
		break;
	case BKW_WAIT_TIME:
		g_uiWaitTime = value;
		break;
	case BKW_DZOOM_STOP:
		g_pDzoomStop = (UINT32 *)value;
		break;
	default:
		debug_err(("[AppBKW_SetData]no this attribute"));
		break;
	}

}


