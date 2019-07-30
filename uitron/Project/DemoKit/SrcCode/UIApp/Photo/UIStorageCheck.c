/**
    Copyright   Novatek Microelectronics Corp. 2009.  All rights reserved.

    @file       UIStorageCheck.c
    @ingroup    Unknown

    @brief


    @note       Nothing.

    @version
    @author
    @date

*/


/** \addtogroup Unknown*/
//@{
////////////////////////////////////////////////////////////////////////////////
#include "SysCommon.h"
//#include "AppCommon.h"
////////////////////////////////////////////////////////////////////////////////

#include "UIAppPhoto.h"
#include "UIStorageCheck.h"
//#include "FlowPanCap.h"

#include "FileSysTsk.h"
#include "PlaybackTsk.h"
#include "DCF.h"

#define THIS_DBGLVL         2 // 0=FATAL, 1=ERR, 2=WRN, 3=UNIT, 4=FUNC, 5=IND, 6=MSG, 7=VALUE, 8=USER
///////////////////////////////////////////////////////////////////////////////
#define __MODULE__          UIStorageCheck
#define __DBGLVL__          ((THIS_DBGLVL>=PRJ_DBG_LVL)?THIS_DBGLVL:PRJ_DBG_LVL)
#define __DBGFLT__          "*" //*=All, [mark]=CustomClass
#include "DebugModule.h"

void UIStorageCheck_AllSize(UINT64 *uiTotal, UINT64 *uiRemain)
{
	(*uiTotal) = FileSys_GetDiskInfo(FST_INFO_DISK_SIZE);
	(*uiRemain) = FileSys_GetDiskInfo(FST_INFO_FREE_SPACE);
	(*uiTotal) /= (1024 * 1024);
	(*uiRemain) /= (1024 * 1024);
}

static BOOL bIsFileReachMax = FALSE;

void UIStorageCheck_ResetFileReachMax(void)
{
	bIsFileReachMax = FALSE;
}
//#NT#2009/12/30#Ben Wang -begin
//#NT#Add check for folder attribute
static BOOL check_CurrDir_writable(void)
{
	UINT32 useFileDB = 0;
	useFileDB = UI_GetData(FL_IsUseFileDB);

	if (useFileDB) {
		return TRUE;
	} else {
#if (USE_DCF == ENABLE)
		UINT32 CurrDirID;
		SDCFDIRINFO dirinfo = {0};

		CurrDirID = DCF_GetDBInfo(DCF_INFO_CUR_DIR_ID);
		if (CurrDirID) {
			if (DCF_GetDirInfo(CurrDirID, &dirinfo)) {
				if (M_IsReadOnly(dirinfo.ucAttrib)) {
					DBG_IND("DCF folder %d is READONLY.\r\n", CurrDirID);
					return FALSE;
				} else {
					DBG_IND("DCF folder %d is existing and writable.\r\n", CurrDirID);
					return TRUE;
				}
			} else {
				return FALSE;
			}
		} else {
			return FALSE;
		}
#else
    return TRUE;
#endif
	}
}


BOOL UIStorageCheck_CurrDirWritable(void)
{
	return TRUE;
}

BOOL UIStorageCheck_DCIMWritable(void)
{
	UINT32 useFileDB = 0;
	useFileDB = UI_GetData(FL_IsUseFileDB);
	if (useFileDB) {
		return TRUE;
	} else {
		char   DCIM_DIR[9] = "A:\\DCIM";
		INT32 uiStatus = 0;
		UINT8  ucAttrib = 0;
		BOOL   ret = FALSE;


		uiStatus = FileSys_GetAttrib(DCIM_DIR, &ucAttrib);
		if (uiStatus == FST_STA_OK) {
			if (ucAttrib & FS_ATTRIB_DIRECTORY) {
				if (System_GetState(SYS_STATE_CARD)  == CARD_REMOVED) {
					DBG_IND("SD Card is removed, DCIM is located on NAND.\r\n");
				}

				if (ucAttrib & FS_ATTRIB_READ) {
					DBG_ERR("A:\\DCIM Directory is READONLY.\r\n");
				} else {
					DBG_IND("A:\\DCIM Directory is existing and writable.\r\n");
					ret = TRUE;
				}
			} else {
				DBG_ERR("A:\\DCIM is not existing.\r\n");
			}
		}

		{
			ret = TRUE;
			DBG_IND("check_DCIM_writable: uiStatus=%d\r\n", uiStatus);
		}
		return ret;
	}
}


BOOL UIStorageCheck_Error(void)
{
	UINT32 useFileDB = 0;
	useFileDB = UI_GetData(FL_IsUseFileDB);
	if (useFileDB) {
		return FALSE;
	} else {
		char   DCIM_DIR[9] = "A:\\DCIM";
		INT32 uiStatus = 0;
		UINT8  ucAttrib = 0;
		BOOL   ret = FALSE;

		uiStatus = FileSys_GetAttrib(DCIM_DIR, &ucAttrib);
		if ((uiStatus == FST_STA_OK) || (uiStatus == FST_STA_BUSY)) {

		} else {
			DBG_IND("UIStorageCheck_Error: uiStatus=%d\r\n", uiStatus);
			ret = TRUE;
		}
		return ret;
	}
}

UINT32 UIStorageCheck_DirFileMax(void)
{
	UINT32 useFileDB = 0;
	useFileDB = UI_GetData(FL_IsUseFileDB);
	if (useFileDB) {
		return 0;
	} else {
		UINT32  ret = 0x00000000;

#if (PLAY_MODE==ENABLE)
#if (USE_DCF == ENABLE)
		UINT32 CurrFileSeq = DCF_GetDBInfo(DCF_INFO_CUR_INDEX);

		if (DCF_GetDBInfo(DCF_INFO_MAX_DIR_ID) == MAX_DCF_DIR_NUM) {
			UINT32 uiFileID;

			ret |= DIRID_MAX;
			DBG_ERR("FolderID reaches 999.\r\n");
			PB_OpenSpecFileBySeq(DCF_GetDBInfo(DCF_INFO_TOL_FILE_COUNT), TRUE);
			PB_GetParam(PBPRMID_NAMEID_FILE, &uiFileID);
			if (uiFileID >= MAX_DCF_FILE_NUM) {
				DBG_ERR("FileID reaches 9999.\r\n");
				ret |= FILEID_MAX;
			}
			PB_OpenSpecFileBySeq(CurrFileSeq, TRUE);
		}
#endif
#endif
		return ret;
	}
}


BOOL UIStorageCheck(UINT32 uiCheckItem, UINT32 *pFreeCount)
{
#if (PHOTO_MODE==ENABLE)
	UINT32 uiCountTmp = 0;
#endif
	UINT32 useFileDB = 0;
	useFileDB = UI_GetData(FL_IsUseFileDB);

	DBG_IND("UI_StorageCheck: Check item = 0x%x\r\n", uiCheckItem);

	switch (uiCheckItem) {
	case STORAGE_CHECK_FOLDER_FULL:
		if (useFileDB) {
			return FALSE;
		} else {
#if (USE_DCF == ENABLE)
			BOOL   bIsFileReachMax = FALSE;
			UINT32     DCF_DirId = 0, DCF_FileId = 0;
			BOOL   ret;

			if (System_GetState(SYS_STATE_FS) == FS_NUM_FULL) {
				DBG_ERR("UI_Validate_Storage: DIR:999 is existing!\r\n");
				return TRUE;
			}
			bIsFileReachMax = FALSE;
			//#NT#2011/03/28#Lincy Lin -begin
			//#NT#fix delete the 9999th file in 999 folder cause capture not save
			ret = DCF_GetNextID(&DCF_DirId, &DCF_FileId);
			if (ret && DCF_DirId == 0 && DCF_FileId == 0) {
				bIsFileReachMax = TRUE;
				DBG_IND("Next dir_id   =%d,   file_id = %d \r\n", DCF_DirId, DCF_FileId);
			} else if (ret && DCF_GetDBInfo(DCF_INFO_IS_9999) == TRUE)
				//#NT#2011/03/28#Lincy Lin -end
			{
				bIsFileReachMax = TRUE;
			}
			return bIsFileReachMax;
#else
            return FALSE;
#endif
		}
		break;
	case STORAGE_CHECK_FULL:
#if (PHOTO_MODE==ENABLE)
		uiCountTmp = PhotoExe_GetFreePicNum();
		//debug_msg("uiCountTmp = %d, pFreeCount=%d\r\n",uiCountTmp,pFreeCount);
		if (pFreeCount) {
			*pFreeCount = uiCountTmp;
		}
		if (uiCountTmp == 0) {
			return TRUE;
		} else {
			return FALSE;
		}
#else
		return FALSE;
#endif

		break;
	case STORAGE_CHECK_LOCKED:
		if (System_GetState(SYS_STATE_CARD)  == CARD_LOCKED) {
			DBG_ERR("UIStorageCheck: %d Card locked\r\n", System_GetState(SYS_STATE_CARD));
			return TRUE;
		} else {
			return FALSE;
		}
		break;
	case STORAGE_CHECK_ERROR:
		FileSys_WaitFinish();
		if (System_GetState(SYS_STATE_FS) == FS_DISK_ERROR
			|| System_GetState(SYS_STATE_FS) == FS_UNKNOWN_FORMAT
			|| System_GetState(SYS_STATE_FS) == FS_UNFORMATTED
			//#NT#2016/12/06#Niven Cho -begin
			//#NT#MULTI_DRIVE
			|| System_GetState(SYS_STATE_CARD)  == CARD_REMOVED)
			//#NT#2016/12/06#Niven Cho -end
			//|| UIStorageCheck_Error() )
		{
			DBG_ERR("UIStorageCheck: %d Card Error\r\n", System_GetState(SYS_STATE_FS));
			return TRUE;
		} else {
			return FALSE;
		}
		break;
	case STORAGE_CHECK_DCIM_READONLY:
		if ((UIStorageCheck_DCIMWritable() == FALSE)
			|| (check_CurrDir_writable() == FALSE)) {
			return TRUE;
		} else {
			return FALSE;
		}
		break;
	default:
		DBG_ERR("UI_StorageCheck: Unknown item = 0x%x\r\n", uiCheckItem);
		return FALSE;

	}
	return FALSE;

}




//@}
