//This source code is generated by UI Designer Studio.

////////////////////////////////////////////////////////////////////////////////
#include "SysCommon.h"
#include "AppCommon.h"
#include "UICommon.h"
////////////////////////////////////////////////////////////////////////////////
#include "UIPlayComm.h"
//#include "FlowCommon.h"
#include "UIStorageCheck.h"

#include "FileSysTsk.h"
#include "ImageUnit_VdoOut.h"
#include "GxDisplay.h"
#include "Dx.h"
#include "DxType.h"
#include "PlaybackTsk.h"
#include "FileDB.h"
#include "PBXFileList_DCF.h"
#include "PBXFileList_FileDB.h"
#include "DCF.h"
#include "UIAppCommon.h"
#include "NameRule_Custom.h"

///////////////////////////////////////////////////////////////////////////////
#define __MODULE__          UIPlayComm
#define __DBGLVL__          2 // 0=FATAL, 1=ERR, 2=WRN, 3=UNIT, 4=FUNC, 5=IND, 6=MSG, 7=VALUE, 8=USER
#define __DBGFLT__          "*" //*=All, [mark]=CustomClass
#include "DebugModule.h"

#if (HDMI_FUNC == ENABLE)
extern DX_HANDLE gDevHDMIObj;
#endif

#define AUTO_HIDE_TIME  2000//ms
URECT g_ThumbRectVideoComm[MAX_THUMB_RECT_NUM] = {0};
//#NT#2012/11/27#Scottie -begin
//#NT#Support Dual Display for PB
URECT g_ThumbRectVideoComm2[MAX_THUMB_RECT_NUM] = {0};
//#NT#2012/11/27#Scottie -end
extern COORDTS_HANDLE g_hTsOsdToVdo;

static UINT32 g_uiAutoHideID = NULL_TIMER;
//#NT#2011/01/31#Ben Wang -begin
//#NT#Add screen control to sync display with OSD
static UINT32 g_uiStartX = 0;
static UINT32 g_uiStartY = 0;
static UINT32 g_uiWidth = TOOL_LAYOUT_W;
static UINT32 g_uiHeight = TOOL_LAYOUT_H;
//#NT#2011/01/31#Ben Wang -end
//#NT#2011/03/30#Ben Wang -begin
//#NT#Add common key sound setting
static UINT32 g_uiPressKeySound = FLGKEY_SOUND_MASK_DEFAULT | FLGKEY_ZOOMIN | FLGKEY_ZOOMOUT;
static UINT32 g_uiContKeySound = FLGKEY_SOUND_MASK_DEFAULT;
//#NT#2011/03/30#Ben Wang -end
void CloseAutoHideTimer()
{
	//#NT#2011/03/14#Ben Wang -begin
	//#NT#Use CONTINUE timer instead.
	//g_uiAutoHideID = NULL_TIMER;
	StopAutoHideTimer();
	//#NT#2011/03/14#Ben Wang -end
}
void StopAutoHideTimer()
{
	if (g_uiAutoHideID != NULL_TIMER) {
		GxTimer_StopTimer(&g_uiAutoHideID);
	}
}
void ResetAutoHideTimer()
{
	StopAutoHideTimer();
	//#NT#2011/03/14#Ben Wang -begin
	//#NT#Use CONTINUE timer instead.
	//g_uiAutoHideID = GxTimer_StartTimer(AUTO_HIDE_TIME, NVTEVT_SELFTIMER, ONE_SHOT);
	g_uiAutoHideID = GxTimer_StartTimer(AUTO_HIDE_TIME, NVTEVT_SELFTIMER, CONTINUE);
	//#NT#2011/03/14#Ben Wang -end
}

//#NT#2015/08/25#KCHong -begin
BOOL IsAutoHideTimer()
{
	return (g_uiAutoHideID != NULL_TIMER) ? TRUE : FALSE;
}
//#NT#2015/08/25#KCHong -end

void ResetPlayKeySound(void)
{
	Input_SetKeySoundMask(KEY_PRESS, FLGKEY_SOUND_MASK_DEFAULT | FLGKEY_ZOOMIN | FLGKEY_ZOOMOUT);
	Input_SetKeySoundMask(KEY_CONTINUE, FLGKEY_SOUND_MASK_DEFAULT);
}
//#NT#2011/03/30#Ben Wang -begin
//#NT#Add common key sound setting
void UIWndBackupKeySound(void)
{
	g_uiPressKeySound = Input_GetKeySoundMask(KEY_PRESS);
	g_uiContKeySound = Input_GetKeySoundMask(KEY_CONTINUE);
}
void UIWndRestoreKeySound(void)
{
	Input_SetKeySoundMask(KEY_PRESS, g_uiPressKeySound);
	Input_SetKeySoundMask(KEY_CONTINUE, g_uiContKeySound);
}
void UIWndSetActiveKeySound(UINT32 uiPressKeySound, UINT32 uiContKeySound)
{
	Input_SetKeySoundMask(KEY_PRESS, uiPressKeySound);
	Input_SetKeySoundMask(KEY_CONTINUE, uiContKeySound);
}
//#NT#2011/03/30#Ben Wang -end
void ChkKeySoundComm(void)
{
	UINT32 uiDefaultSound = Input_GetKeySoundMask(KEY_PRESS);
	UINT32 uiFileNum = 0;
	PB_GetParam(PBPRMID_TOTAL_FILE_COUNT, &uiFileNum);
	if (uiFileNum <= 1) {
		uiDefaultSound &= ~(FLGKEY_LEFT | FLGKEY_RIGHT);
	} else {
		uiDefaultSound |= (FLGKEY_LEFT | FLGKEY_RIGHT);
	}

	Input_SetKeySoundMask(KEY_PRESS, uiDefaultSound);
	Input_SetKeySoundMask(KEY_CONTINUE, uiDefaultSound);
}
static BOOL    gbNoFile = TRUE;
static void xUIPlay_Filesys_DelCB(FIND_DATA *pFindData, BOOL *bDelete, UINT32 Param1, UINT32 Param2)
{
	INT32    fileNum;
	UINT32   fileType;
	UINT32   filterType = (DCF_GetDBInfo(DCF_INFO_VALID_FILE_FMT) | DCF_GetDBInfo(DCF_INFO_DEP_FILE_FMT));
	UINT8     attrib;

	attrib    = pFindData->attrib;

	DBG_IND("FileName = %s\r\n", pFindData->FATMainName);

	fileNum = DCF_IsValidFile(pFindData->FATMainName, pFindData->FATExtName, &fileType);

	if (!M_IsReadOnly(attrib) && fileNum && (filterType & fileType)) {
		*bDelete = TRUE;
	} else {
		*bDelete = FALSE;
		gbNoFile = FALSE;
	}
}
static void xUIPlay_Filesys_LockCB(FIND_DATA *pFindData, BOOL *bApply, UINT32 Param1, UINT32 Param2)
{
	INT32    fileNum;
	UINT32   fileType;
	UINT32   filterType = (DCF_GetDBInfo(DCF_INFO_VALID_FILE_FMT) | DCF_GetDBInfo(DCF_INFO_DEP_FILE_FMT));

	DBG_IND("FileName = %s\r\n", pFindData->FATMainName);

	fileNum = DCF_IsValidFile(pFindData->FATMainName, pFindData->FATExtName, &fileType);
	if (fileNum && (filterType & fileType)) {
		*bApply = TRUE;
	} else {
		*bApply = FALSE;
	}
}
static void xUIPlay_FList_DCF_DelAll(void)
{
	SDCFDIRINFO dirinfo;
	char path[DCF_FULL_FILE_PATH_LEN];
	BOOL ret;
	UINT32 i, uiMaxDirNum;

	uiMaxDirNum = DCF_GetDBInfo(DCF_INFO_MAX_DIR_ID);
	for (i = 100; i <= uiMaxDirNum; i++) {
		// check if folder has file
		ret = DCF_GetDirInfo(i, &dirinfo);
		if (ret /*&& dirinfo.uiNumOfDcfObj*/) {
			// delete all in one folder
			DCF_GetDirPath(i, path);
			gbNoFile = TRUE;
			FileSys_DelDirFiles(path, xUIPlay_Filesys_DelCB);
			// delete empty folder
			if (gbNoFile) {
				if (FileSys_DeleteDir(path) != FST_STA_OK) {
					DBG_ERR("Failed to Delete %s.\r\n", path);
				}
			}
		}
	}
	DCF_Refresh();
}
static ER xUIPlay_FList_FDB_ProtectFile(UINT32  cmdID)
{
	FILEDB_HANDLE     FileDBHdl = 0;
	PFILEDB_FILE_ATTR FileAttr = NULL;
	INT32             ret;
	UINT32            FileNum, i;
	BOOL              bLock;

	Perf_Mark();
	if (cmdID == PLAY_PROTECT_ONE || cmdID == PLAY_UNPROTECT_ONE) {
		if (cmdID == PLAY_PROTECT_ONE) {
			bLock = TRUE;
		} else {
			bLock = FALSE;
		}
		FileAttr = FileDB_CurrFile(FileDBHdl);
		//#NT#2016/05/25#Ben Wang -begin
		//#NT#Fix coverity issue.
		//i =  FileDB_GetCurrFileIndex(FileDBHdl);
		//#NT#2016/05/25#Ben Wang -end
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
	} else if (cmdID == PLAY_PROTECT_ALL || cmdID == PLAY_UNPROTECT_ALL) {
		if (cmdID == PLAY_PROTECT_ALL) {
			bLock = TRUE;
		} else {
			bLock = FALSE;
		}
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
	}
	DBG_IND("ProtectTime = %04d ms \r\n", Perf_GetDuration() / 1000);
	return E_OK;
FDB_Protect_Err:
	DBG_ERR("cmdID=%d\r\n", cmdID);
	return E_OK;
}
static void xUIPlay_FList_DCF_LockAll(BOOL bLock)
{
	SDCFDIRINFO dirinfo = {0};
	char path[DCF_FULL_FILE_PATH_LEN] = {0};
	UINT32 i, uiMaxDirNum;

	uiMaxDirNum = DCF_GetDBInfo(DCF_INFO_MAX_DIR_ID);
	for (i = 100; i <= uiMaxDirNum; i++) {
		// check if folder has file
		if (DCF_GetDirInfo(i, &dirinfo)) {
			if (dirinfo.uiNumOfDcfObj) {
				DCF_GetDirPath(i, path);
				FileSys_LockDirFiles(path, bLock, xUIPlay_Filesys_LockCB);
			}
		}
	}
}

static ER xUIPlay_FList_DCF_ProtectFile(UINT32  cmdID)
{
	char     filePath[DCF_FULL_FILE_PATH_LEN];
	UINT32   index, i, uiThisFileFormat, tmpFileType;
	BOOL     bLock;

	Perf_Mark();
	DBG_IND("cmdID=%d\r\n", cmdID);
	if ((cmdID == PLAY_PROTECT_ONE) || (cmdID == PLAY_UNPROTECT_ONE)) {
		if (cmdID == PLAY_PROTECT_ONE) {
			bLock = TRUE;    // lock
		} else {
			bLock = FALSE;    // unlock
		}
		index = DCF_GetCurIndex();
		uiThisFileFormat = DCF_GetDBInfo(DCF_INFO_CUR_FILE_TYPE);
		for (i = 0; i <= DCF_FILE_TYPE_NUM; i++) {
			tmpFileType = 1;
			tmpFileType <<= i;
			if (uiThisFileFormat & tmpFileType) {
				DCF_GetObjPath(index, tmpFileType, filePath);
				FileSys_SetAttrib(filePath, FST_ATTRIB_READONLY, bLock);
			}
		}
	} else if ((cmdID == PLAY_PROTECT_ALL) || (cmdID == PLAY_UNPROTECT_ALL)) {
		if (cmdID == PLAY_PROTECT_ALL) {
			bLock  = TRUE;    // lock
		} else {
			bLock  = FALSE;    // unlock
		}
		xUIPlay_FList_DCF_LockAll(bLock);
	}
	DBG_IND("ProtectTime = %04d ms \r\n", Perf_GetDuration() / 1000);
	return E_OK;
}
static ER xUIPlay_FList_FDB_DelFile(UINT32  cmdID)
{
	FILEDB_HANDLE     FileDBHdl = 0;
	PFILEDB_FILE_ATTR FileAttr = NULL;
	INT32             ret;
	INT32             FileNum, i;
	//#NT#2016/10/03#Hideo Lin -begin
	//#NT#To delete both of main and clone movies
#if (SMALL_CLONE_MOVIE == ENABLE)
	char    filePath[NMC_TOTALFILEPATH_MAX_LEN];
	char    fileName[NMC_TOTALFILEPATH_MAX_LEN - NMC_ROOT_MAX_LEN - NMC_OTHERS_MAX_LEN];
	char   *pMainName;
	char   *pExtName;
	char   *pFolder;
#endif
	//#NT#2016/10/03#Hideo Lin -end

	Perf_Mark();
	DBG_IND("cmdID=%d\r\n", cmdID);
	if (cmdID == PB_DELETE_ONE) {
		FileAttr = FileDB_CurrFile(FileDBHdl);
		i =  FileDB_GetCurrFileIndex(FileDBHdl);
		if (FileAttr) {
			ret = FileSys_DeleteFile(FileAttr->filePath);
			if (ret == FST_STA_OK) {
				//#NT#2016/10/03#Hideo Lin -begin
				//#NT#To delete both of main and clone movies
#if (SMALL_CLONE_MOVIE == ENABLE)
				// file attribute will be changed after FileDB_DeleteFile,
				// so we have to get the clone movie path before that,
				// and don't need to do FileDB delete since clone movies had been filtered

				snprintf(fileName, NMC_TOTALFILEPATH_MAX_LEN - NMC_ROOT_MAX_LEN - NMC_OTHERS_MAX_LEN, "%s", FileAttr->filename);
				pMainName = strtok(fileName, ".");
				pExtName  = strrchr(FileAttr->filename, '.');

				if (strstr(FileAttr->filePath, MOVIE_RO_FOLDER) == 0) { // normal folder
					pFolder = MOVIE_SHARE_FOLDER;
				} else { // RO folder
					pFolder = MOVIE_SHARE_RO_FOLDER;
				}

				snprintf(filePath, NMC_TOTALFILEPATH_MAX_LEN, "%s%s\\%s_S%s",
						 DV_ROOT_PATH, pFolder, pMainName, pExtName);

				ret = FileSys_DeleteFile(filePath);
#endif
				//#NT#2016/10/03#Hideo Lin -end

				FileDB_DeleteFile(FileDBHdl, i);
			} else {
				goto FDB_Delete_Err;
			}
		} else {
			goto FDB_Delete_Err;
		}
	} else if (cmdID == PB_DELETE_ALL) {
		BOOL  isCurrFileReadOnly = FALSE;
		CHAR  path[PBX_FLIST_NAME_MAX_LENG + 1];
		UINT32 Index;

		FileAttr = FileDB_CurrFile(FileDBHdl);
		if (FileAttr && M_IsReadOnly(FileAttr->attrib)) {
			isCurrFileReadOnly = TRUE;
			strncpy(path, FileAttr->filePath, PBX_FLIST_NAME_MAX_LENG);
		}
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
					//#NT#2016/10/03#Hideo Lin -begin
					//#NT#To delete both of main and clone movies
#if (SMALL_CLONE_MOVIE == ENABLE)
					// file attribute will be changed after FileDB_DeleteFile,
					// so we have to get the clone movie path before that,
					// and don't need to do FileDB delete since clone movies had been filtered

					snprintf(fileName, NMC_TOTALFILEPATH_MAX_LEN - NMC_ROOT_MAX_LEN - NMC_OTHERS_MAX_LEN, "%s", FileAttr->filename);
					pMainName = strtok(fileName, ".");
					pExtName  = strrchr(FileAttr->filename, '.');

					if (strstr(FileAttr->filePath, MOVIE_RO_FOLDER) == 0) { // normal folder
						pFolder = MOVIE_SHARE_FOLDER;
					} else { // RO folder
						pFolder = MOVIE_SHARE_RO_FOLDER;
					}

					snprintf(filePath, NMC_TOTALFILEPATH_MAX_LEN, "%s%s\\%s_S%s",
							 DV_ROOT_PATH, pFolder, pMainName, pExtName);

					ret = FileSys_DeleteFile(filePath);
#endif
					//#NT#2016/10/03#Hideo Lin -end

					FileDB_DeleteFile(FileDBHdl, i);
				}
			} else {
				goto FDB_Delete_Err;
			}
		}
		if (isCurrFileReadOnly) {
			Index = FileDB_GetIndexByPath(FileDBHdl, path);
			FileDB_SearhFile(FileDBHdl, Index);
		}
	}
	DBG_IND("DelTime = %04d ms \r\n", Perf_GetDuration() / 1000);
	return E_OK;
FDB_Delete_Err:
	DBG_ERR("cmdID=%d\r\n", cmdID);
	return E_SYS;

}
static ER xUIPlay_FList_DCF_DelFile(UINT32  cmdID)
{
	DBG_IND("cmdID=%d\r\n", cmdID);
	if (cmdID == PB_DELETE_ONE) {
		//#NT#2016/05/24#Ben Wang -begin
		//#NT#Fix the coverity issue.
		UINT32 index, uiThisFileFormat, i, tmpFileType;
		//#NT#2016/05/24#Ben Wang -end
		CHAR   filePath[DCF_FULL_FILE_PATH_LEN] = {0};

		index = DCF_GetCurIndex();
		uiThisFileFormat = DCF_GetDBInfo(DCF_INFO_CUR_FILE_TYPE);
		for (i = 0; i <= DCF_FILE_TYPE_NUM; i++) {
			tmpFileType = 0x0001 << i;
			if (uiThisFileFormat & tmpFileType) {
				if (DCF_GetObjPath(index, uiThisFileFormat, filePath)) {
					if (FST_STA_OK == FileSys_DeleteFile(filePath)) {
						DCF_DelDBfile(filePath);
					}
				}
			}
		}
	} else if (cmdID == PB_DELETE_ALL) {
		char        filePath[DCF_FULL_FILE_PATH_LEN];
		BOOL        ret;
		UINT8       attrib = 0;
		UINT32      DirID = 0, FileID = 0, Index = 0, uiThisFileFormat = 0;
		BOOL        isCurrFileReadOnly = FALSE;

		Index = DCF_GetDBInfo(DCF_INFO_CUR_INDEX);
		DCF_GetObjInfo(Index, &DirID, &FileID, &uiThisFileFormat);
		DCF_GetObjPath(Index, uiThisFileFormat, filePath);

		ret = FileSys_GetAttrib(filePath, &attrib);
		if ((ret == E_OK) && M_IsReadOnly(attrib) == TRUE) {
			isCurrFileReadOnly = TRUE;
		}

		// ---------  delete all files  -----------------------
		xUIPlay_FList_DCF_DelAll();
		if (isCurrFileReadOnly) {
			Index = DCF_GetIndexByID(DirID, FileID);
			DCF_SetCurIndex(Index);
		}
		DCF_Refresh();
	}
	return E_OK;
}
void UIPlay_Protect(PLAY_PROTECT_CMD Command)
{
	UINT32      useFileDB    = 0;

	useFileDB = UI_GetData(FL_IsUseFileDB);

	if (useFileDB) {
		xUIPlay_FList_FDB_ProtectFile(Command);
	} else {
		xUIPlay_FList_DCF_ProtectFile(Command);
	}
}
void UIPlay_Delete(UINT32 Command)
{
	UINT32 index = 0;
	UINT32 useFileDB = 0;


	useFileDB = UI_GetData(FL_IsUseFileDB);
	if (useFileDB) {
		xUIPlay_FList_FDB_DelFile(Command);
	} else {
		xUIPlay_FList_DCF_DelFile(Command);
	}

	PB_GetParam(PBPRMID_CURR_FILESEQ, &index);

	PB_OpenSpecFileBySeq(index, TRUE);

	PB_WaitCommandFinish(PB_WAIT_INFINITE);
}
INT32 UIPlay_PlaySingle(UINT32 Command)
{
	PLAY_SINGLE_OBJ FlowPlaySingleObj;
	FlowPlaySingleObj.PlayCommand = Command;
	if (0 == (Command & PB_SINGLE_THUMB)) {
		FlowPlaySingleObj.PlayCommand |= PB_SINGLE_PRIMARY;
	}
	FlowPlaySingleObj.JumpOffset  = 1;
#if (_SCREENNAIL_SIZE_ == _SCREENNAIL_SIZE_VGA_)
#if (HDMI_FUNC == ENABLE)
	if (IsHDMIPlugIn()) {
		FlowPlaySingleObj.PlayCommand |= PB_SINGLE_NO_HIDDEN;
	}
#endif
#endif
	PB_PlaySingleMode(&FlowPlaySingleObj);
	return PB_WaitCommandFinish(PB_WAIT_INFINITE);
}

BOOL IsFileError(void)
{
	UINT32 uiStatus;
	uiStatus = PB_WaitCommandFinish(PB_WAIT_INFINITE);
	if (uiStatus & (PB_STA_ERR_FILE | PB_STA_ERR_DECODE)) {
		return TRUE;
	} else {
		return FALSE;
	}
}
#if (HDMI_FUNC == ENABLE)
BOOL IsHDMIPlugIn(void)
{
#if defined(_BSP_NA51000_)
	DX_HANDLE DevObj1, DevObj2;
	DevObj1 = (DX_HANDLE)GxVideo_GetDevice(DOUT1);
	DevObj2 = (DX_HANDLE)GxVideo_GetDevice(DOUT2);
	if (DevObj1 == gDevHDMIObj || DevObj2 == gDevHDMIObj) {
		return TRUE;
	} else {
		return FALSE;
	}
#endif
#if defined(_BSP_NA51023_)
	DX_HANDLE DevObj1;
	DevObj1 = (DX_HANDLE)GxVideo_GetDevice(DOUT1);
	if (DevObj1 == gDevHDMIObj) {
		return TRUE;
	} else {
		return FALSE;
	}
#endif
}
#endif
BOOL ExceedMaxPlayByDateFileNum(void)
{
	UINT32 FileNum = 0;
	PB_GetParam(PBPRMID_TOTAL_FILE_COUNT, &FileNum);
	if (FileNum > MAX_PLAY_BY_DATE_FILE_NUM) {
		return TRUE;
	} else {
		return FALSE;
	}
}

extern UINT32 g_DualVideo;

//#NT#2010/11/23#Ben Wang -begin
//#NT#Refine code for browse window
void BrowseThumbUpdateLayout(URECT *pOSDRect, UINT32 uiThumbNum)
{
	USIZE bufsize, bufsize2;
	COORDTS_TARGET Target = {0};
	COORDTS_TRANS  Trans = {0};
	UINT32 tmp = 0;

	if (g_DualVideo & DISPLAY_1) {

		ImageUnit_GetDisp_BufSize(&ISF_VdoOut1, ISF_IN1, &bufsize);
		if (SysVideo_GetDirbyID(DOUT1) == ISF_VDO_DIR_ROTATE_270 || SysVideo_GetDirbyID(DOUT1) == ISF_VDO_DIR_ROTATE_90) {
			tmp       = bufsize.h;
			bufsize.h = bufsize.w;
			bufsize.w = tmp;
		}
		DBG_MSG("PB img1 %dx%d\r\n", bufsize.w, bufsize.h);

		Target.hHandle = g_hTsOsdToVdo;
		Target.SizeTarget.w = bufsize.w;
		Target.SizeTarget.h = bufsize.h;
		Target.pRcRegion = NULL;
		CoordTs_SetTarget(&Target);

		Trans.hHandle = g_hTsOsdToVdo;
		Trans.bInverse = FALSE;
		Trans.Method = COORDTS_METHOD_FULL_FIT;
		Trans.pRcSrc = pOSDRect;
		Trans.pRcDst = g_ThumbRectVideoComm;
		Trans.uiRectNum = uiThumbNum;
		CoordTs_TransRect(&Trans);

	}

//#NT#2012/11/27#Scottie -begin
//#NT#Support Dual Display for PB
	if (g_DualVideo & DISPLAY_2) {
		ImageUnit_GetDisp_BufSize(&ISF_VdoOut2, ISF_IN1, &bufsize2);
		DBG_MSG("PB img2 %dx%d\r\n", bufsize2.w, bufsize2.h);

		Target.hHandle = g_hTsOsdToVdo;
		Target.SizeTarget.w = bufsize2.w;
		Target.SizeTarget.h = bufsize2.h;
		Target.pRcRegion = NULL;
		CoordTs_SetTarget(&Target);

		Trans.hHandle = g_hTsOsdToVdo;
		Trans.bInverse = FALSE;
		Trans.Method = COORDTS_METHOD_FULL_FIT;
		Trans.pRcSrc = pOSDRect;
		Trans.pRcDst = g_ThumbRectVideoComm2;
		Trans.uiRectNum = uiThumbNum;
		CoordTs_TransRect(&Trans);
	}
//#NT#2012/11/27#Scottie -end
}

void BrowseThumbNaviKey(PBROWSE_NAVI_INFO pBrowseNavi)
{
	UINT32  CurrFileIndex, uiParamVal;
	UINT32  CurrFileSeqID, FileNumsInDir = 0;
	UINT32  FileNumsInFinalPage, FileNumsInFinalLine;
	UINT32  *pThumbSeqID;
	UINT32  CurThumbNums, CurrPageID, FinalPageID;
	UINT32  NumsPerPage, CurrThumbRow, FinalThumbRow, NextFileIndex;
	UINT32  BrowserCommand = 0, JumpOffset = 0;
	PLAY_BROWSER_OBJ     FlowPBBrowserObj = {0};


	PB_GetParam(PBPRMID_THUMB_CURR_IDX, &CurrFileIndex);
	PB_GetParam(PBPRMID_THUMB_SEQ_ARRAY, &uiParamVal);
	pThumbSeqID = (UINT32 *)uiParamVal;
	pThumbSeqID += (CurrFileIndex - 1);
	CurrFileSeqID = *pThumbSeqID;
	NumsPerPage = pBrowseNavi->HorNums * pBrowseNavi->VerNums;

	PB_GetParam(PBPRMID_TOTAL_FILE_COUNT, &FileNumsInDir);

	if (FileNumsInDir == 0) {
		return;
	}

	if ((FileNumsInDir % NumsPerPage) == 0) {
		FileNumsInFinalPage = NumsPerPage;
	} else {
		FileNumsInFinalPage = FileNumsInDir % NumsPerPage;
	}

	if ((FileNumsInFinalPage % pBrowseNavi->HorNums) == 0) {
		FileNumsInFinalLine = pBrowseNavi->HorNums;
	} else {
		FileNumsInFinalLine = FileNumsInFinalPage % pBrowseNavi->HorNums;
	}

	CurrPageID  = ((CurrFileSeqID - 1) / NumsPerPage);
	FinalPageID = ((FileNumsInDir - 1) / NumsPerPage);

	PB_GetParam(PBPRMID_THUMB_CURR_NUM, &CurThumbNums);

	CurrThumbRow  = (CurrFileIndex - 1) / pBrowseNavi->HorNums;
	FinalThumbRow = (CurThumbNums - 1) / pBrowseNavi->HorNums;

	DBG_MSG("key=0x%x, CurrFileIndex=%d, CurrFileSeqID=%d\r\n", pBrowseNavi->NaviKey, CurrFileIndex, CurrFileSeqID);
	DBG_MSG("NumsPerPage=%d, FileNumsInDir=%d\r\n", NumsPerPage, FileNumsInDir);
	DBG_MSG("FileNumsInFinalPage=%d, FileNumsInFinalLine=%d\r\n", FileNumsInFinalPage, FileNumsInFinalLine);
	DBG_MSG("CurrPageID=%d, FinalPageID=%d\r\n", CurrPageID, FinalPageID);
	DBG_MSG("CurThumbNums=%d, CurrThumbRow=%d, FinalThumbRow=%d\r\n", CurThumbNums, CurrThumbRow, FinalThumbRow);

	// page-direction (LEFT)
	if (pBrowseNavi->NaviKey == NVTEVT_KEY_LEFT) {
		BrowserCommand = PB_BROWSER_PREV;
		JumpOffset     = 1;
	}
	// page-direction (RIGHT)
	else if (pBrowseNavi->NaviKey == NVTEVT_KEY_RIGHT) {
		BrowserCommand = PB_BROWSER_NEXT;
		JumpOffset     = 1;
	}
	// page-direction (UP)
	else if (pBrowseNavi->NaviKey == NVTEVT_KEY_UP) {
		BrowserCommand = PB_BROWSER_PREV;

		if (CurrPageID > 0) {
			// means now is not 1st-page, decode the same file-index-image
			JumpOffset = pBrowseNavi->HorNums;
		} else { // if(CurrPageID == 0)
			// means now is 1st-page
			if (CurrThumbRow > 0) {
				// means CurrThumbRow=1,2, do not change page, decode the same file-index-image
				JumpOffset = pBrowseNavi->HorNums;
			} else if (FileNumsInDir <= NumsPerPage) {
				// means only one page & CurrThumbRow = 0,

				if (FinalThumbRow == 0) {
					// means only one row, do nothing
					JumpOffset = 0;
				} else if (FileNumsInFinalLine >= CurrFileIndex) {
					// must decode the same file-index-image
					JumpOffset = FileNumsInFinalLine;
				} else {
					// must decode the same file-index-image
					JumpOffset = pBrowseNavi->HorNums + FileNumsInFinalLine;
				}
			} else {
				// means CurrThumbRow=0, go to final page
				if (FileNumsInFinalPage < pBrowseNavi->HorNums) {
					// means final-page file nums < 3 (only one line),
					if (FileNumsInFinalLine >= CurrFileIndex) {
						// must decode the same file-index-image
						JumpOffset = FileNumsInFinalLine;
					} else {
						// must decode final file-index-image
						JumpOffset = CurrFileIndex;
					}
				} else {
					// means final-page file nums >= 3
					if (FileNumsInFinalLine >= CurrFileIndex) {
						// must decode the same file-index-image
						JumpOffset = FileNumsInFinalLine;
					} else {
						// must decode the same file-index-image
						JumpOffset = pBrowseNavi->HorNums + FileNumsInFinalLine;
					}
				}
			}
		}// else if(CurrPageID == 0)
	}
	// page-direction (DOWN)
	//else if(pBrowseNavi->NaviKey & NVTEVT_KEY_DOWN)
	else if (pBrowseNavi->NaviKey == NVTEVT_KEY_DOWN) { //#NT#2015/08/25#KCHong#Fixed
		BrowserCommand = PB_BROWSER_NEXT;

		if ((CurrPageID + 1) < FinalPageID) {
			// means now is not final-page & (final-1)-page, decode the same file-index-image
			JumpOffset = pBrowseNavi->HorNums;
		} else if ((CurrPageID + 1) == FinalPageID) {
			// means now is (final-1)-page, go to final-page
			if (CurrThumbRow != FinalThumbRow) {
				// means CurrThumbRow != MaxRow, do not change page, decode the same file-index-image
				JumpOffset = pBrowseNavi->HorNums;
			} else if (FileNumsInFinalPage < pBrowseNavi->HorNums) {
				// means final-page file nums < 3 (only one line),
				if ((CurrFileIndex % pBrowseNavi->HorNums) == 0) {
					NextFileIndex = pBrowseNavi->HorNums;
				} else {
					NextFileIndex = CurrFileIndex % pBrowseNavi->HorNums;
				}

				if (FileNumsInFinalLine >= NextFileIndex) {
					// must decode the same file-index-image
					JumpOffset = pBrowseNavi->HorNums;
				} else {
					// must decode final file-index-image
					JumpOffset = pBrowseNavi->HorNums - (NextFileIndex - FileNumsInFinalLine);
				}
			} else {
				// must decode the same file-index-image
				JumpOffset = pBrowseNavi->HorNums;
			}
		} else { // if(CurrPageID == FinalPageID)
			// means now is final-page
			if ((CurrFileIndex + pBrowseNavi->HorNums) <= CurThumbNums) {
				// means do not change page, decode the same file-index-image
				JumpOffset = pBrowseNavi->HorNums;
			} else if (FileNumsInDir <= NumsPerPage) {
				// means only one page & CurrThumbRow=end,
				if (FinalThumbRow == 0) {
					// means only one row, do nothing
					JumpOffset = 0;
				} else if (CurrThumbRow != FinalThumbRow) {
					JumpOffset = (pBrowseNavi->HorNums + FileNumsInFinalLine);
				} else {
					JumpOffset = (FileNumsInFinalLine);
				}
			} else {
				// means now is final-page, go to 1st-page
				// must decode the same file-index-image
				if (CurrThumbRow != FinalThumbRow) {
					JumpOffset = (pBrowseNavi->HorNums + FileNumsInFinalLine);
				} else {
					JumpOffset = (FileNumsInFinalLine);
				}
			}
		}// End of if(CurrPageID == FinalPageID)
	}
	//#NT#2015/08/25#KCHong -begin
	// page-direction (Previous page)
	else if (pBrowseNavi->NaviKey == NVTEVT_KEY_PREV) {
		BrowserCommand = PB_BROWSER_PREV;

		if (CurrPageID > 0) {
			// means now is not 1st-page, decode the same file-index-image
			JumpOffset = NumsPerPage;
		} else { // if(CurrPageID == 0)
			// means now is 1st-page
			if (FileNumsInDir <= NumsPerPage) {
				// means only one page & CurrThumbRow = 0,
				// means only one row, do nothing
				JumpOffset = 0;
			} else {
				// means CurrThumbRow=0, go to final page
				if (FileNumsInFinalPage < CurrFileIndex) {
					// means final-page file nums < CurrFileIndex, jump to last file
					JumpOffset = CurrFileIndex;
				} else {
					// means final-page file nums >= CurrFilIndex, jump to corresponding file
					JumpOffset = FileNumsInFinalPage;
				}

			}
		}// else if(CurrPageID == 0)
	}
	// page-direction (next page)
	else if (pBrowseNavi->NaviKey == NVTEVT_KEY_NEXT) {
		BrowserCommand = PB_BROWSER_NEXT;

		if ((CurrPageID + 1) < FinalPageID) {
			// means now is not final-page & (final-1)-page, decode the same file-index-image
			JumpOffset = NumsPerPage;
		} else if ((CurrPageID + 1) == FinalPageID) {
			// means now is (final-1)-page, go to final-page
			if (FileNumsInFinalPage < CurrFileIndex) {
				// means final-page file nums < CurrFileIndex, jump to last file
				JumpOffset = NumsPerPage - CurrFileIndex + FileNumsInFinalPage;
			} else {
				// means final-page file nums >= CurrFilIndex, jump to corresponding file
				JumpOffset = NumsPerPage;
			}
		} else { // if(CurrPageID == FinalPageID)
			// means now is final-page
			JumpOffset = FileNumsInFinalPage;
		}// End of if(CurrPageID == FinalPageID)
	}
	// page-direction (jump by index)
	else if (pBrowseNavi->NaviKey < NumsPerPage) {
		pBrowseNavi->NaviKey ++;        // Sync the index base since UI use 0~8 but here use 1~9
		if (pBrowseNavi->NaviKey >= CurrFileIndex) {
			BrowserCommand = PB_BROWSER_NEXT;
			JumpOffset = pBrowseNavi->NaviKey - CurrFileIndex;
		} else {
			BrowserCommand = PB_BROWSER_PREV;
			JumpOffset = CurrFileIndex - pBrowseNavi->NaviKey;
		}
	}
	//#NT#2015/08/25#KCHong -end
	else {
		DBG_ERR("Only support navigation key\r\n");
	}
	DBG_MSG("BrowserCommand=0x%x, JumpOffset=%d\r\n", BrowserCommand, JumpOffset);

	FlowPBBrowserObj.BrowserCommand = BrowserCommand;
	FlowPBBrowserObj.JumpOffset     = JumpOffset;
	FlowPBBrowserObj.HorNums        = pBrowseNavi->HorNums;
	FlowPBBrowserObj.VerNums        = pBrowseNavi->VerNums;
	if (FileNumsInDir <= NumsPerPage) { // means only one page
		FlowPBBrowserObj.bReDecodeImages = FALSE;
	} else {
		FlowPBBrowserObj.bReDecodeImages = TRUE;
	}
	//#NT#2010/11/05#Ben Wang -begin
	//#NT#Add the feature of "show selected thumb on background"
	//FlowPBBrowserObj.bDrawCurrImgOnBG = g_bDrawCurrImgOnBG;
	//#NT#2010/11/05#Ben Wang -end
	if (PB_PlayBrowserMode(&FlowPBBrowserObj) != PB_STA_DONE) {
		PB_WaitCommandFinish(PB_WAIT_INFINITE);
	}
}
//#NT#2010/11/23#Ben Wang -end
//#NT#2011/01/31#Ben Wang -begin
//#NT#Add screen control to sync display with OSD
void SetVdoWinSizeForPB(UINT32 uiStartX, UINT32 uiStartY, UINT32 uiWidth, UINT32 uiHeight)
{
	g_uiStartX = uiStartX;
	g_uiStartY = uiStartY;
	g_uiWidth = uiWidth;
	g_uiHeight = uiHeight;
	UI_Show(UI_SHOW_PLAYBACK, TRUE);
}

extern UINT32 g_DualVideo;

void UpdateVdoWinForPB(void)
{
	URECT rcSrc, rcDst;
	COORDTS_TRANS  Trans = {0};
	COORDTS_TARGET Target = {0};
	USIZE bufsize, bufsize2;
	UINT32 tmp = 0;

	if (g_DualVideo & DISPLAY_1) {
		ImageUnit_GetDisp_BufSize(&ISF_VdoOut1, ISF_IN1, &bufsize);
		if (SysVideo_GetDirbyID(DOUT1) == ISF_VDO_DIR_ROTATE_270 || SysVideo_GetDirbyID(DOUT1) == ISF_VDO_DIR_ROTATE_90) {
			tmp       = bufsize.h;
			bufsize.h = bufsize.w;
			bufsize.w = tmp;
		}
		DBG_MSG("PB img1 %dx%d\r\n", bufsize.w, bufsize.h);

		Target.hHandle = g_hTsOsdToVdo;
		Target.SizeTarget.w = bufsize.w;
		Target.SizeTarget.h = bufsize.h;
		Target.pRcRegion = NULL;
		CoordTs_SetTarget(&Target);

		rcSrc.x = g_uiStartX;
		rcSrc.y = g_uiStartY;
		rcSrc.w = g_uiWidth;
		rcSrc.h = g_uiHeight;

		Trans.hHandle = g_hTsOsdToVdo;
		Trans.bInverse = FALSE;
		Trans.Method = COORDTS_METHOD_FULL_FIT;
		Trans.pRcSrc = &rcSrc;
		Trans.pRcDst = &rcDst;
		Trans.uiRectNum = sizeof(rcSrc) / sizeof(rcDst);
		CoordTs_TransRect(&Trans);
		PB_ConfigVdoWIN(PBDISP_IDX_PRI, &rcDst);
	}

//#NT#2012/11/22#Scottie -begin
//#NT#Support Dual Display for PB
	if (g_DualVideo & DISPLAY_2) {
		ImageUnit_GetDisp_BufSize(&ISF_VdoOut2, ISF_IN1, &bufsize2);
		DBG_MSG("PB img2 %dx%d\r\n", bufsize2.w, bufsize2.h);

		Target.hHandle = g_hTsOsdToVdo;
		Target.SizeTarget.w = bufsize2.w;
		Target.SizeTarget.h = bufsize2.h;
		Target.pRcRegion = NULL;
		CoordTs_SetTarget(&Target);

		rcSrc.x = g_uiStartX;
		rcSrc.y = g_uiStartY;
		rcSrc.w = g_uiWidth;
		rcSrc.h = g_uiHeight;

		Trans.hHandle = g_hTsOsdToVdo;
		Trans.bInverse = FALSE;
		Trans.Method = COORDTS_METHOD_FULL_FIT;
		Trans.pRcSrc = &rcSrc;
		Trans.pRcDst = &rcDst;
		Trans.uiRectNum = sizeof(rcSrc) / sizeof(rcDst);
		CoordTs_TransRect(&Trans);
		PB_ConfigVdoWIN(PBDISP_IDX_SEC, &rcDst);
	}
//#NT#2012/11/22#Scottie -end

	DBG_MSG("OSD URECT=%d, %d, %d, %d => VIDEO URECT=%d, %d, %d, %d\r\n",
			rcSrc.x, rcSrc.y, rcSrc.w, rcSrc.h,
			rcDst.x, rcDst.y, rcDst.w, rcDst.h);
}
//#NT#2011/01/31#Ben Wang -end

