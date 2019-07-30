/**
    File Database module.

    The FileDB module support the file sorting & searching for some specific file attributes.
    This is just a library not including any task.

    @file   FileDB.h
    @ingroup    mILibFileDB

    Copyright   Novatek Microelectronics Corp. 2011.  All rights reserved.

*/

#ifndef _FILESDB_H_
#define _FILESDB_H_

#include "Type.h"
#include "fs_file_op.h"

/**
    @addtogroup mImILibFileDB
*/
//@{


/**
    File database handle.
*/
typedef INT32 FILEDB_HANDLE;


/**
    Just support DCF file only, can reduce the work buffer size
*/
#define _SUPPORT_DCF_ONLY_                    1


//#if (_SUPPORT_DCF_ONLY_)
//#define FILEDB_PATH_MAX_LENG                  30                            ///< DCF full file path legnth, ex: "A:\DCIM\100NVTIM\IMAG0001.JPG" & '\0'
//#else
//#define FILEDB_PATH_MAX_LENG                  KFS_FILEPATH_MAX_LENG         ///< the max full path length of file
//#endif

/**
     @name FileDB error return
*/
//@{
#define FILEDB_CREATE_ERROR                  -1                 ///< Create database error
#define FILEDB_SEARCH_ERR                    -1                 ///< search file error
//@}


/**
     FileDB reserved size for cast to internal data structure
*/
#define FILEDB_RESERVE_DWORD                 (1)

/**
     @name FileDB support File Type
*/
//@{
#define FILEDB_SUPPORT_FILE_TYPE_NUM          11                ///< Total FileDB support file type number
#define FILEDB_FMT_JPG                        0x0001            ///< bit 0 -> JPG file type, file extension is *.JPG
#define FILEDB_FMT_MP3                        0x0002            ///< bit 1 -> MP3 file type, file extension is *.MP3
#define FILEDB_FMT_AVI                        0x0004            ///< bit 2 -> AVI file type, file extension is *.AVI
#define FILEDB_FMT_ASF                        0x0008            ///< bit 3 -> ASF file type, file extension is *.ASF
#define FILEDB_FMT_WAV                        0x0010            ///< bit 4 -> WAV file type, file extension is *.WAV
#define FILEDB_FMT_MOV                        0x0020            ///< bit 5 -> MOV file type, file extension is *.MOV
#define FILEDB_FMT_BMP                        0x0040            ///< bit 6 -> BMP file type, file extension is *.BMP
#define FILEDB_FMT_MP4                        0x0080            ///< bit 7 -> MP4 file type, file extension is *.MP4
#define FILEDB_FMT_MPG                        0x0100            ///< bit 8 -> MPG file type, file extension is *.MPG
#define FILEDB_FMT_RAW                        0x0200            ///< bit 9 -> RAW file type, file extension is *.RAW
#define FILEDB_FMT_TS                         0x0400            ///< bit 10 -> TS file type, file extension is *.TS
#define FILEDB_FMT_ANY                        0x07FF            ///< any file format, the ORed of all supported file type
//@}

/**
    @name Get Date macro
    used for creDate or lastWriteDate in file attribute.
*/
//@{
#define FILEDB_GET_DAY_FROM_DATE(x)     (x & 0x1F)              ///<  get day from date
#define FILEDB_GET_MONTH_FROM_DATE(x)   ((x >> 5) & 0x0F)       ///<  get month from date
#define FILEDB_GET_YEAR_FROM_DATE(x)    ((x >> 9) & 0x7F)+1980  ///<  get year from date
//@}

/**
    @name Get Time macro
    used for creTime or lastWriteTime in file attribute.
*/
//@{
#define FILEDB_GET_SEC_FROM_TIME(x)     (x & 0x001F)<<1         ///<  seconds(2 seconds / unit)
#define FILEDB_GET_MIN_FROM_TIME(x)     (x & 0x07E0)>>5         ///<  Minutes
#define FILEDB_GET_HOUR_FROM_TIME(x)    (x & 0xF800)>>11        ///<  hours
//@}

/**
     The file sorting type.

     @note If FILEDB_SORT_BY_SN is chosen, SortSN_Delim, SortSN_DelimCount, and SortSN_CharNumOfSN of FILEDB_INIT_OBJ should also be set.
*/
typedef enum _FILEDB_SORT_TYPE {
	FILEDB_SORT_BY_NONE = 0x00,         ///< no sort, the sequence depends on the file sequence in disk
	FILEDB_SORT_BY_CREDATE,             ///< sort by the file create time
	FILEDB_SORT_BY_MODDATE,             ///< sort by the file modify time
	FILEDB_SORT_BY_NAME,                ///< sort by the file name
	FILEDB_SORT_BY_STROKENUM,           ///< sort by the file name stroke number
	FILEDB_SORT_BY_FILEPATH,            ///< sort by the full file path
	FILEDB_SORT_BY_SIZE,                ///< sort by the file size
	FILEDB_SORT_BY_FILETYPE,            ///< sort by the file type
	FILEDB_SORT_BY_TYPE_CRETIME_SIZE,   ///< sort by the sequence (1) File type, (2) File Cretate time, (3) file size
	FILEDB_SORT_BY_SN,                  ///< sort by the serial number. SortSN_Delim, SortSN_DelimCount, and SortSN_CharNumOfSN should be set properly.
} FILEDB_SORT_TYPE;


/**
    FileDB callback funcion for abort FileDB_Create command.
*/
typedef BOOL (*CHKABORT_CB)(void);



/**
     The file information of each file stored in database.
*/
typedef struct _FILEDB_FILE_ATTR {
	UINT16               creTime;                              ///< File created time. bit0~4: seconds/2, bit5~10: minutes, bit11~15: hours.
	UINT16               creDate;                              ///< File created date. bit0~4: day(1-31), bit5~8: month(1-12), bit9~15: year(0-127) add 1980 to convert
	UINT16               lastWriteTime;                        ///< The latest time of write file.
	UINT16               lastWriteDate;                        ///< The latest date of write file.
	UINT64               fileSize64;                             ///< File size in bytes.
	UINT16               fileType;                             ///< File type, the value could be FILEDB_FMT_JPG, FILEDB_FMT_MP3 or FILEDB_FMT_AVI ...
	UINT8                attrib;                               ///< File attribute.
	UINT8                userData;                             ///< The user data that can keep in each file entry
	CHAR                *filename;                             ///< File Name
	CHAR                *filePath;                             ///< File full path
	UINT32               u32Reserved[FILEDB_RESERVE_DWORD];    ///< Reserved data
} FILEDB_FILE_ATTR, *PFILEDB_FILE_ATTR;

/**
     The file database init parameter.
*/
typedef struct _FILEDB_INIT_OBJ {
	CHAR                *rootPath;                             ///<  Root path
	CHAR                *defaultfolder;                        ///<  default folder
	CHAR                *filterfolder;                         ///<  filter folder, ex:"DCIM|CARDV", different folders are separated by '|'.
	BOOL                 bIsRecursive;                         ///<  If recursive search this path
	BOOL                 bIsCyclic;                            ///<  If cyclic search file when file index moving
	BOOL                 bIsMoveToLastFile;                    ///<  If want to move file index to last file when init
	BOOL                 bIsSupportLongName;                   ///<  If support long file name
	BOOL                 bIsDCFFileOnly;                       ///<  If search the file just keep the DCF file only and filter out other files.
	BOOL                 bIsFilterOutSameDCFNumFolder;         ///<  remove duplicate folder id when DCF scan, the vlaue is TRUE or FALSE
	CHAR                 OurDCFDirName[5];                     ///<  DCF dir free chars, ex: NVTIM
	CHAR                 OurDCFFileName[4];                    ///<  DCF file free chars, ex: IMAG
	UINT8                bIsFilterOutSameDCFNumFile;           ///<  remove duplicate file id when DCF scan, the vlaue is TRUE or FALSE
	BOOL                 bIsChkHasFile;                        ///<  If just want to check if there any matched file inside file system
	UINT32               u32MaxFilePathLen;                    ///<  The maximum file path length user want to support,
	UINT32               u32MaxFileNum;                        ///<  The maximum file number that user want to store in database.
	UINT32               fileFilter;                           ///<  The file type filter, it should be ORed of supported file type, ex: FILEDB_FMT_JPG|FILEDB_FMT_AVI.
	UINT32               u32MemAddr;                           ///<  The work buffer address for storing files info.
	UINT32               u32MemSize;                           ///<  The work buffer size
	CHKABORT_CB          fpChkAbort;                           ///<  Callback for abort the FileDB create operation, if the callback function return TRUE means user want to abort.
	BOOL                 bIsSkipDirForNonRecursive;            ///<  If skip folder for non-recursive case
	BOOL                 bIsSkipHidden;                        ///<  Skip hidden files and folders
	CHAR                *SortSN_Delim;                         ///<  The delimiter string, e.g. underline "_", "AA"
	INT32                SortSN_DelimCount;                    ///<  The delimiter count to find the serial number
	INT32                SortSN_CharNumOfSN;                   ///<  The character number of the serial number
} FILEDB_INIT_OBJ, *PFILEDB_INIT_OBJ;

/**
     Install FileDB flag and semaphore id.

*/
extern void FileDB_InstallID(void) _SECTION(".kercfg_text");

/**
    Create a file database.

    @param pfileDbInitObj: the init parameters of create file database.

    @return FILEDB_HANDLE, if the return value is FILEDB_CREATE_ERROR that means no more FILEDB handle can be created now.

    Example:
    @code
    {
        static FILEDB_HANDLE gExamFileDBHdl = FILEDB_CREATE_ERROR;
        FILEDB_INIT_OBJ   FilDBInitObj={0};
        CHAR              rootPath[20]="A:\\";
        CHAR              defaultfolder[20]="A:\\CarDV\\";
        CHAR              filterfolder[50]="DCIM|LOOPING";
        UINT32            isDefaultFolder = FALSE;
        UINT32            isFilterFolder = FALSE;
        UINT32            isRecursive = TRUE;
        UINT32            isDCF = FALSE;
        UINT32            isSupportLong = TRUE;
        UINT32            MaxFilePathLen = 40;
        UINT32            MaxFileNum = 2000;
        UINT32            FileFilter = FILEDB_FMT_ANY;
        UINT32            MemSize = TEMP_BUF_SIZE;

        // set the work buffer to cache memory
        FilDBInitObj.rootPath = rootPath;
        FilDBInitObj.defaultfolder = NULL;
        FilDBInitObj.filterfolder = NULL;
        FilDBInitObj.bIsRecursive = isRecursive;
        FilDBInitObj.bIsCyclic = TRUE;
        FilDBInitObj.bIsMoveToLastFile = TRUE;
        FilDBInitObj.bIsSupportLongName = isSupportLong;
        FilDBInitObj.bIsDCFFileOnly = isDCF;
        FilDBInitObj.bIsChkHasFile = FALSE;
        FilDBInitObj.u32MaxFilePathLen = MaxFilePathLen;
        FilDBInitObj.u32MaxFileNum = MaxFileNum;
        FilDBInitObj.fileFilter = FileFilter;
        FilDBInitObj.u32MemAddr = dma_getCacheAddr(MemAddr);
        FilDBInitObj.u32MemSize = MemSize;
        FilDBInitObj.bIsSkipDirForNonRecursive = FALSE;
        FilDBInitObj.bIsSkipHidden = FALSE;
        FilDBInitObj.SortSN_Delim = "_";
        FilDBInitObj.SortSN_DelimCount = 2;
        FilDBInitObj.SortSN_CharNumOfSN = 4;
        // create FileDB
        gExamFileDBHdl = FileDB_Create(&FilDBInitObj);
        // do some file search
        ¡K
    }
    @endcode

*/
extern FILEDB_HANDLE FileDB_Create(PFILEDB_INIT_OBJ pfileDbInitObj);

/**
    Release a file database.

    @param[in] fileDbHandle the fileDB handle that get from FileDB_Create(), the possible value should be >=0.

    Example:
    @code
    {
        // release FileDB because not to use it anymore
        FileDB_Release(gExamFileDBHdl);
        gExamFileDBHdl = FILEDB_CREATE_ERROR;
    }
    @endcode
*/
extern void FileDB_Release(FILEDB_HANDLE fileDbHandle);

/**
    Search file by file index.

    Use file index to get one file's info. The current index of this file database will also be moved to i32FileIndex.

    @param[in] fileDbHandle the fileDB handle that get from FileDB_Create(), the possible value should be >=0.
    @param[in] i32FileIndex the file index.

    @return the file info pointer.

    Example:
    @code
    {
        FILEDB_FILE_ATTR *pFileAttr;

        pFileAttr = FileDB_SearhFile(gExamFileDBHdl, uiSequ);
        if (pFileAttr == NULL)
        {
            DBG_ERR("GetPathBySeq fails..!!!\r\n");
            return E_SYS;
        }
        DBG_DUMP(" getpath %s \r\n", pFileAttr->filePath);
    }
    @endcode
*/
extern PFILEDB_FILE_ATTR FileDB_SearhFile(FILEDB_HANDLE fileDbHandle, INT32 i32FileIndex);

/**
    Search file by file index.

    Use file index to get one file's info. The current index of this file database will not be changed.
    (This is the major difference between FileDB_SearhFile2 & FileDB_SearhFile)

    @param[in] fileDbHandle the fileDB handle that get from FileDB_Create(), the possible value should be >=0.
    @param[in] i32FileIndex the file index.

    @return the file info pointer.
*/
extern PFILEDB_FILE_ATTR FileDB_SearhFile2(FILEDB_HANDLE fileDbHandle, INT32 i32FileIndex);

/**
    Get total files number of a file database.

    @param[in] fileDbHandle the fileDB handle that get from FileDB_Create(), the possible value should be >=0.

    @return The total files number.

    Example:
    @code
    {
        // get total files number
        FileCount = FileDB_GetTotalFileNum(gExamFileDBHdl);
    }
    @endcode

*/
extern UINT32 FileDB_GetTotalFileNum(FILEDB_HANDLE fileDbHandle);

/**
    Get the file info of current index.

    @param[in] fileDbHandle the fileDB handle that get from FileDB_Create(), the possible value should be >=0.

    @return the file info pointer.

    Example:
    @code
    {
        FILEDB_FILE_ATTR *pFileAttr;

        pFileAttr = FileDB_CurrFile(gExamFileDBHdl);
        DBG_DUMP(" getpath %s \r\n", pFileAttr->filePath);

    }
    @endcode
*/
extern PFILEDB_FILE_ATTR FileDB_CurrFile(FILEDB_HANDLE fileDbHandle);

/**
    Get the file info of next index.

    This API will move the current index to next index and return the file info.
    @note If the bIsCyclic is set TRUE in FileDB_Create(), when the current index reaches the last file index,
    If user calls FileDB_NextFile(), then the index will be moved to first file index.

    @param[in] fileDbHandle the fileDB handle that get from FileDB_Create(), the possible value should be >=0.

    @return the file info pointer.

    Example:
    @code
    {
        FILEDB_FILE_ATTR *pFileAttr;

        pFileAttr = FileDB_NextFile(gExamFileDBHdl);
        DBG_DUMP(" getpath %s \r\n", pFileAttr->filePath);
    }
    @endcode
*/
extern PFILEDB_FILE_ATTR FileDB_NextFile(FILEDB_HANDLE fileDbHandle);

/**
    Get the file info of previous index.

    This API will move the current index to previous index and return the file info.
    @note If the bIsCyclic is set TRUE in FileDB_Create(), when the current index reaches the first file index,
    If user calls FileDB_PrevFile(), then the index will be moved to last file index.

    @param[in] fileDbHandle the fileDB handle that get from FileDB_Create(), the possible value should be >=0.

    @return the file info pointer.

    Example:
    @code
    {
        FILEDB_FILE_ATTR *pFileAttr;

        pFileAttr = FileDB_PrevFile(gExamFileDBHdl);
        DBG_DUMP(" getpath %s \r\n", pFileAttr->filePath);

    }
    @endcode
*/
extern PFILEDB_FILE_ATTR FileDB_PrevFile(FILEDB_HANDLE fileDbHandle);

/**
    Get current file index.

    @param[in] fileDbHandle the fileDB handle that get from FileDB_Create(), the possible value should be >=0.

    @return the file index, the possible value should be from 0 to TotalFileNum-1.

    Example:
    @code
    {
        // get current file index
        FileIndex = FileDB_GetCurrFileIndex(gExamFileDBHdl);
    }
    @endcode

*/
extern UINT32 FileDB_GetCurrFileIndex(FILEDB_HANDLE fileDbHandle);

/**
    Sorting the FileDB by sortType.

    @param[in] fileDbHandle the fileDB handle that get from FileDB_Create(), the possible value should be >=0.
    @param[in] sortType The sort type.
    @param[in] bIsSortLargeFirst if set to TRUE, then the sort result will be largest first, else is smallest first

    Example:
    @code
    {
        // sort file by create time
        FileDB_SortBy(gExamFileDBHdl,FILEDB_SORT_BY_CREDATE,FALSE);
    }
    @endcode

*/
extern void   FileDB_SortBy(FILEDB_HANDLE fileDbHandle, FILEDB_SORT_TYPE sortType, BOOL bIsSortLargeFirst);

/**

    Get files number of specific file type.

    @param[in] fileDbHandle the fileDB handle that get from FileDB_Create(), the possible value should be >=0.
    @param[in] fileType the file type, it can be the combine value of FILEDB_FMT_JPG, FILEDB_FMT_MP3, FILEDB_FMT_AVI..
    @return The total files number of fileType

    Example:
    @code
    {
        // get jpg file count
        JpgFileCount = FileDB_GetFilesNumByFileType(gExamFileDBHdl, FILEDB_FMT_JPG);
    }
    @endcode
*/
extern UINT32 FileDB_GetFilesNumByFileType(FILEDB_HANDLE fileDbHandle, UINT32 fileType);

/**
    Refresh the File DataBase.

    @note: Because the FileDB is just a database. so when some files are deleted, addeded or modified.\n
    User need to call this API to refresh the database.

    @param[in] fileDbHandle refer to FileDB.h, the possible value should be >=0 and <FILEDB_NUM
    @return E_OK: The refresh command is success, else means have some error.

    Example:
    @code
    {
        CHAR      path[50]="A:\\CarDV";

        // delete all files in one folder
        FileSys_DelDirFiles(path,NULL);
        // Refresh the FileDB
        FileDB_Refresh();

    }
    @endcode
*/
extern INT32 FileDB_Refresh(FILEDB_HANDLE fileDbHandle);

/**
    Add a file to DataBase by sorting sequence.

    @param[in] fileDbHandle the fileDB handle that get from FileDB_Create(), the possible value should be >=0.
    @param[in] filePath the file full path.
    @return
         - @b TRUE:    Add file ok.
         - @b FALSE:   Add file fail.

    Example:
    @code
    {
        FST_FILE fp;

        fp = FileSys_OpenFile(Path, FST_CREATE_ALWAYS|FST_OPEN_WRITE);
        rt = FileSys_WriteFile(fp, (UINT8*)Addr, &Length, 0, NULL);
        FileSys_CloseFile(fp);
        if (rt == FST_STA_OK)
        {
            // Add the file to FileDB
            FileDB_AddFile(gExamFileDBHdl, Path);
        }
    }
    @endcode
*/

extern BOOL FileDB_AddFile(FILEDB_HANDLE fileDbHandle, CHAR *filePath);


/**
    Add a file to the last item in DataBase.

    @param[in] fileDbHandle the fileDB handle that get from FileDB_Create(), the possible value should be >=0.
    @param[in] filePath the file full path.
    @return
         - @b TRUE:    Add file ok.
         - @b FALSE:   Add file fail.

    Example:
    @code
    {
        FST_FILE fp;

        fp = FileSys_OpenFile(Path, FST_CREATE_ALWAYS|FST_OPEN_WRITE);
        rt = FileSys_WriteFile(fp, (UINT8*)Addr, &Length, 0, NULL);
        FileSys_CloseFile(fp);
        if (rt == FST_STA_OK)
        {
            // Add the file to FileDB
            FileDB_AddFile(gExamFileDBHdl, Path);
        }
    }
    @endcode
*/

extern BOOL FileDB_AddFileToLast(FILEDB_HANDLE fileDbHandle, CHAR *filePath);

/**
    Delete a file from DataBase.

    @note: This API is just remove the file entry from the database, it will not really delete the file from disk.

    @param[in] fileDbHandle the fileDB handle that get from FileDB_Create(), the possible value should be >=0.
    @param[in] u32FileIndex the file index.
    @return
         - @b TRUE:    Delete file ok.
         - @b FALSE:   Delete file fail.

    Example:
    @code
    {
        CHAR      path[50]="A:\\CarDV\\20130206.1824_0001.mov";
        UINT32    index;

        // Delete a file from file system.
        FileSys_DeleteFile(path);

        index = FileDB_GetIndexByPath(gExamFileDBHdl,path);
        if (index!=FILEDB_SEARCH_ERR)
        {
            FileDB_DeleteFile(gExamFileDBHdl,index);
        }

    }
    @endcode
*/
extern BOOL  FileDB_DeleteFile(FILEDB_HANDLE fileDbHandle, UINT32 u32FileIndex);

/**
    Get the file index by specific file path.

    @param[in] fileDbHandle refer to FileDB.h, the possible value should be >=0 and <FILEDB_NUM
    @param[in] filePath the file full path.
    @return value >=0 means get the index success, if return value FILEDB_SEARCH_ERR: get file index fail.

    Example:
    @code
    {
        CHAR      path[50]="A:\\CarDV\\20130206.1824_0001.mov";
        UINT32    index;

        index = FileDB_GetIndexByPath(gExamFileDBHdl,path);
        if (index!=FILEDB_SEARCH_ERR)
        {
            pFileAttr = FileDB_SearhFile(gExamFileDBHdl, index);
            DBG_DUMP(" file attrib = 0x%x \r\n", pFileAttr->attrib);
        }
    }
    @endcode
*/
extern INT32 FileDB_GetIndexByPath(FILEDB_HANDLE fileDbHandle, char *filePath);

/**
    Get required buffer size by supported max file number.

    @param[in] fileNum The maximum file number that user want to store in file database.
    @param[in] u32MaxFilePathLen The maximum file path length want to support
    @return the needed buffer size.
*/
extern UINT32 FileDB_CalcBuffSize(UINT32 fileNum, UINT32 u32MaxFilePathLen);

/**
     Dump File Database related information.

     @param[in] fileDbHandle refer to FileDB.h, the possible value should be >=0 and <FILEDB_NUM
*/
extern void   FileDB_DumpInfo(FILEDB_HANDLE fileDbHandle);

/**
    Set a file size.

    @param[in] fileDbHandle the fileDB handle that get from FileDB_Create(), the possible value should be >=0.
    @param[in] i32FileIndex the file index.
    @param[in] fileSize64 the file size (UINT64)
    @return
         - @b TRUE:    Set ok.
         - @b FALSE:   Set fail.
*/
extern BOOL FileDB_SetFileSize(FILEDB_HANDLE fileDbHandle, INT32 i32FileIndex, UINT64 fileSize64);

/**
    Set a file read-only attribute.

    @param[in] fileDbHandle the fileDB handle that get from FileDB_Create(), the possible value should be >=0.
    @param[in] i32FileIndex the file index.
    @param[in] bReadOnly the file read-only
    @return
         - @b TRUE:    Set ok.
         - @b FALSE:   Set fail.
*/
extern BOOL FileDB_SetFileRO(FILEDB_HANDLE fileDbHandle, INT32 i32FileIndex, BOOL bReadOnly);

/**
    Get a file serial number by the specific file name.

    @param[in] fileDbHandle refer to FileDB.h, the possible value should be >=0 and <FILEDB_NUM
    @param[in] fileName the file name.
    @return value >=0 means a valid serial number, if return value < 0 means failure.

    Example:
    @code
    {
        CHAR      szFileName[50]="20130206_1824_0001.mov";
        INT32     SN;

        SN = FileDB_GetSNByName(gExamFileDBHdl, szFileName);
        if (SN < 0)
            DBG_ERR("Get SN failed\r\n");
        else
            DBG_IND("SN = %d\r\n", SN);
    }
    @endcode
*/
extern INT32 FileDB_GetSNByName(FILEDB_HANDLE fileDbHandle, char *fileName);

//@}
#endif  //_FILESDB_H_
