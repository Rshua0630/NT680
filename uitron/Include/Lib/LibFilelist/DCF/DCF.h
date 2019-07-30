/**
    DCF module.

    The DCF module includes all DCF object handling.
    This is just a library not including any task.

    @file       DCF.h
    @ingroup    mILibDCF

    Copyright   Novatek Microelectronics Corp. 2011.  All rights reserved.
*/

#ifndef _DCF_H_
#define _DCF_H_
#include "Type.h"

/**
    @addtogroup mILibDCF
*/
//@{

/**
     @name This is temporarily define for DCF modify test , need to remove later
*/
//@{
#define USE_DCF_LIB                  1
//@}

/**
    DCF handle.
*/
typedef INT32 DCF_HANDLE;

/**
     @name DCF error return
*/
//@{
#define DCF_OPEN_ERROR                  -1        ///< Open a DCF database error

/**
     @name DCF standard Max & Min ID
*/
//@{
#define MIN_DCF_DIR_NUM             (100)   ///< Minimum DCF DIR ID
#define MAX_DCF_DIR_NUM             (999)   ///< Maximum DCF DIR ID
#define MIN_DCF_FILE_NUM            (1)     ///< Minimum DCF File ID
#define MAX_DCF_FILE_NUM            (9999)  ///< Maximum DCF File ID
//@}


/**
     @name DCF Error ID.
     @note When DCF max object is 999-9999, the DCF is out of range.\n
     Then the DCF next id will be set to this error value.\n
     DCF_GetNextID will return this error value.
*/
//@{
#define ERR_DCF_DIR_NUM              (0)    ///< Error DCF DIR ID
#define ERR_DCF_FILE_NUM             (0)    ///< Error DCF File ID
//@}

/**
     @name DCF File Type
*/
//@{
#define DCF_FILE_TYPE_NUM                  10       ///< Total DCF support file type number
#define DCF_FILE_TYPE_NOFILE              (0x0000)  ///< no file
#define DCF_FILE_TYPE_JPG                 (0x0001)  ///< bit 0 -> JPG file type, file extension is *.JPG
#define DCF_FILE_TYPE_AVI                 (0x0002)  ///< bit 1 -> AVI file type, file extension is *.AVI
#define DCF_FILE_TYPE_WAV                 (0x0004)  ///< bit 2 -> WAV file type, file extension is *.WAV
#define DCF_FILE_TYPE_RAW                 (0x0008)  ///< bit 3 -> RAW file type, file extension is *.RAW
#define DCF_FILE_TYPE_TIF                 (0x0010)  ///< bit 4 -> TIF file type, file extension is *.TIF
#define DCF_FILE_TYPE_MPO                 (0x0020)  ///< bit 5 -> MPO file type, file extension is *.MPO
#define DCF_FILE_TYPE_MOV                 (0x0040)  ///< bit 6 -> MOV file type, file extension is *.MOV
#define DCF_FILE_TYPE_MP4                 (0x0080)  ///< bit 7 -> MP4 file type, file extension is *.MP4
#define DCF_FILE_TYPE_MPG                 (0x0100)  ///< bit 8 -> MPG file type, file extension is *.MPG
#define DCF_FILE_TYPE_ASF                 (0x0200)  ///< bit 9 -> ASF file type, file extension is *.ASF
#define DCF_FILE_TYPE_ANYFORMAT           (0x03FF)  ///< any file format means JPG|AVI|WAV|RAW|TIF|MPO|MOV|MP4|MPG|ASF
#define DCF_FMT_VALID_BIT                 (0x8000)  ///< bit 15  (MST) indicate valid DCF obj
#define DCF_FILE_TYPE_BITS                (16)      ///< This value should larger than DCF_FILE_TYPE_NUM, the value could be 8, 16, 32
#define DCF_FILE_CACHE_BITS               (0xFFFF)  ///< every file have 16 states:
//@}

/**
     @name DCF name length
*/
//@{
#define DCF_DIR_NAME_LEN                  5     ///<  DCF dir free chars length, ex: NVTIM
#define DCF_FILE_NAME_LEN                 4     ///<  DCF file free chars length, ex: IMAG
#define DCF_FULL_FILE_PATH_LEN            30    ///<  DCF full file path legnth, ex: "A:\DCIM\100NVTIM\IMAG0001.JPG" & '\0'
//@}

//#define DCF_CHARS_SET_CREATE   0              /*  - Create new directory.          */
//#define DCF_CHARS_SET_ALLOWED  1              /*  - Sets the allowed chars.        */


/**
     DCF dir info.
*/
typedef struct _SDCFDIRINFO {
	CHAR        szDirFreeChars[6];         ///< directory free chars including \0, ex: NVTIM
	UINT32      uiNumOfDcfObj;             ///< count of DCF objects in the directory
	UINT8       ucAttrib;                  ///< directory attributes ( RO, HIDDEN )
} SDCFDIRINFO, *PSDCFDIRINFO;

/**
     DCF parameter ID is used in funcion DCF_SetParm.
*/
typedef enum _DCF_PRMID {
	DCF_PRMID_REMOVE_DUPLICATE_FOLDER = 0, ///< remove duplicate folder id when DCF scan, the vlaue is TRUE or FALSE
	DCF_PRMID_REMOVE_DUPLICATE_FILE,       ///< remove duplicate file id when DCF scan, the vlaue is TRUE or FALSE
	DCF_PRMID_INIT_INDEX_TO_LAST,          ///< DCF init scan & index to last DCF object, the vlaue is TRUE or FALSE
	DCF_PRMID_SET_VALID_FILE_FMT,          ///< set DCF valid file format, ex: DCF_FILE_TYPE_JPG|DCF_FILE_TYPE_AVI
	DCF_PRMID_SET_DEP_FILE_FMT,            ///< set DCF dependence file format, ex: DCF_FILE_TYPE_JPG|DCF_FILE_TYPE_MPO
	DCF_PRMID_REMOVE_HIDDEN_ITEM,          ///< remove hidden items, only valid when remove duplicate parameters is TRUE, the value is TRUE or FALSE
	ENUM_DUMMY4WORD(DCF_PRMID)
} DCF_PRMID;

/**
     DCF INFO ID.
*/
typedef enum _DCF_INFOID {
	DCF_INFO_TOL_FILE_COUNT = 0,           ///< total DCF file count
	DCF_INFO_TOL_DIR_COUNT,                ///< how many DCF folder in DCIM
	DCF_INFO_CUR_INDEX,                    ///< current index of dcf file , range is from 1 to DCF_INFO_TOL_FILE_COUNT
	DCF_INFO_CUR_DIR_ID,                   ///< current access directory number.
	DCF_INFO_CUR_FILE_ID,                  ///< current access file number
	DCF_INFO_MAX_DIR_ID,                   ///< maximun directory ID of this system
	DCF_INFO_MAX_FILE_ID,                  ///< maximun file ID of current access directory, it may bigger then DirFileNum
	DCF_INFO_CUR_FILE_TYPE,                ///< current access file format
	DCF_INFO_MAX_SYS_FILE_ID,              ///< Max file ID of Max Dir
	DCF_INFO_IS_9999,                      ///< the existence of 999xxxxx\yyyy9999.zzz
	DCF_INFO_DIR_FILE_SEQ,                 ///< the file sequence of current dir
	DCF_INFO_DIR_FILE_NUM,                 ///< how many DCF file in current dir
	DCF_INFO_VALID_FILE_FMT,               ///< valid file format, ex: DCF_FILE_TYPE_JPG|DCF_FILE_TYPE_AVI|DCF_FILE_TYPE_MOV
	DCF_INFO_DEP_FILE_FMT,                 ///< dependence file format, ex:DCF_FILE_TYPE_JPG|DCF_FILE_TYPE_WAV|DCF_FILE_TYPE_MPO
	DCF_INFO_DIR_FREE_CHARS,               ///< DCF dir free chars, ex: NVTIM
	DCF_INFO_FILE_FREE_CHARS,              ///< DCF file free chars, ex: IMAG
	ENUM_DUMMY4WORD(DCF_INFOID)
} DCF_INFOID;


/**
     DCF seek index command.
*/
typedef enum _DCF_SEEK_CMD {
	DCF_SEEK_SET = 0,                      ///< set index to offset
	DCF_SEEK_CUR,                          ///< jump offset from current index
	DCF_SEEK_END,                          ///< jump offset from last index (DCF_INFO_TOL_FILE_COUNT)
	DCF_SEEK_MAX_ID,                       ///< total seek command numbers
	ENUM_DUMMY4WORD(DCF_SEEK_CMD)
} DCF_SEEK_CMD;

/**
     DCF callback message.

*/
typedef enum {
	DCF_CBMSG_LAST_FILE_FIND = 0,          ///< DCF scan find the last file
	DCF_CBMSG_SCAN_COMPLETE,               ///< DCF scan complete
	DCF_CBMSG_MAX_CNT,                     ///< total callback message count
	ENUM_DUMMY4WORD(DCF_CBMSG)
} DCF_CBMSG;

/**
    DCF callback funcion proto type
*/
typedef void (*DCF_CBMSG_FP)(DCF_CBMSG uiMsgID, void *Data);

/**
     The DCF init parameter.
*/
typedef struct _DCF_OPEN_PARM {
	CHAR                 Drive;                                  ///<  The Drive Name of File System
	UINT32               WorkbuffAddr;                           ///<  The work buffer address for storing DCF objects info.
	UINT32               WorkbuffSize;                           ///<  The work buffer size
} DCF_OPEN_PARM, *PDCF_OPEN_PARM;



/**
     Install DCF flag and semaphore id.

*/
extern void DCF_InstallID(void) _SECTION(".kercfg_text");

//DCF DB control API

/**
     Config DCF int parameters.

     @note The paraemters should be config before DCF scan.

     @param[in] parmID:  parameter ID
     @param[in] value:  the setting value
     @return
         - @b TRUE:    set ok
         - @b FALSE:   set fail (parameter error)
*/
extern BOOL   DCF_SetParm(DCF_PRMID parmID, UINT32 value);



extern DCF_HANDLE DCF_Open(PDCF_OPEN_PARM pParm);

extern void DCF_Close(DCF_HANDLE DcfHandle);
/**
     Scan DCF objects.

     Scan DCF objects from filesystem and store info to DCF database.

     @note This API is running in the caller task, it may take a long time.\n
           Ths scan time depeonds on the file number stored in storage.
*/
extern void   DCF_ScanObjEx(DCF_HANDLE DcfHandle);

/**
     Refresh DCF database.

     Refresh DCF database by re-scan filesystem, the API is similiar to DCF_ScanObj.
     The difference is that this API will keep the original DCF index when re-scan.

     @note This API is running in the caller task, it may take a long time.\n
           Ths refresh time depeonds on the file number stored in storage.
*/
extern void   DCF_RefreshEx(DCF_HANDLE DcfHandle);
/**
     Add a file to DCF database.

     @note This API is just to translate the input file path to a DCF object, and store info to DCF database.\n
           It will not check if the file is really existed in the storage or not.\n
           If the file is not really stored in storage, then after re-scan DCF database\n
           by calling DCF_ScanObj() or DCF_Refresh(), this DCF object will disppear in DCF database.

     @param[in] filePath: The DCF object file fullpath.
     @return
         - @b TRUE:    Add file ok.
         - @b FALSE:   Add file fail (May be input file path error).
*/
extern BOOL   DCF_AddDBfileEx(DCF_HANDLE DcfHandle, CHAR *filePath);

/**
     Delete a folder from DCF database.

     @param[in] filePath: The DCF object folder fullpath.
     @return
         - @b TRUE:    Delete folder ok.
         - @b FALSE:   Delete folder fail (May be input file path error, or folder not exist in DCF Database).
*/
extern BOOL   DCF_DelDBDirEx(DCF_HANDLE DcfHandle, CHAR *filePath);

/**
     Delete a file from DCF database.

     @param[in] filePath: The DCF object file fullpath.
     @return
         - @b TRUE:    Delete file ok.
         - @b FALSE:   Delete file fail (May be input file path error, or file not exist in DCF Database).
*/
extern BOOL   DCF_DelDBfileEx(DCF_HANDLE DcfHandle, CHAR *filePath);


/**
     Set the DCF Dir free characters.

     This function enables the host to set the DCF free DIR characters.
     Ths free characters wiill be used in file names created when calling DCF_MakeObjPath().

     @param[in] baChars   The DCF Dir free chars, the length is 5, ex: "NVTIM"

     Example:
     @code
     {
        DCF_SetDirFreeChars("NVTIM");
     }
     @endcode
*/
extern void   DCF_SetDirFreeChars(CHAR baChars[]);

/**
     Set the DCF File free characters by fileType.

     This function enables the host to set the DCF free file characters by different file type.
     The free chars will be used in file names created when calling DCF_MakeObjPath().

     @param[in] fileType: The File type. The file type can be one file type ex: DCF_FILE_TYPE_JPG.\n
                           Or file type combination, ex:DCF_FILE_TYPE_JPG|DCF_FILE_TYPE_AVI or DCF_FILE_TYPE_ANYFORMAT ...
     @param[in] baChars[]:  The DCF file free chars, the length is 4, ex: "IMAG".

     Example:
     @code
     {
        DCF_SetFileFreeChars(DCF_FILE_TYPE_ANYFORMAT, "IMAG" );
     }
     @endcode
*/
extern void   DCF_SetFileFreeChars(UINT32 fileType, CHAR baChars[]);

/**
     Make a DCF directory path for create a new DCF folder.

     @param[in] DirID:    The DCF Dir ID.
     @param[out] Path:    The DCF folder path that maked.

     Example:
     @code
     {
         DCF_MakeDirPath(DirID,path);
         FileSys_MakeDir(path);
     }
     @endcode
*/
extern BOOL   DCF_MakeDirPathEx(DCF_HANDLE DcfHandle, UINT32  DirID, CHAR *Path);
/**
     Make a DCF object file path for create a new file.

     This function is to make a Make a DCF object file path by input DirID, FileID & fileType.
     It will also reference the free chars set by DCF_SetDirFreeChars() and DCF_SetFileFreeChars() to make a file path.

     @note Because the DCF DirID, FileID should not be duplicate in normal case.\n
           So normally case, user must call DCF_GetNextID() firstly to get the next ID.\n
           And then call this funcion to make a DCF object path for create a file.


     @param[in] DirID:    The DCF Dir ID.
     @param[in] FileID:    The DCF file ID.
     @param[in] fileType:  The DCF File type. The file type can be just one file type ex: DCF_FILE_TYPE_JPG or DCF_FILE_TYPE_AVI.\n
                                  Can not be the file type combination, ex:DCF_FILE_TYPE_JPG|DCF_FILE_TYPE_AVI.
     @param[out] filePath: The DCF file path that maked.

     Example:
     @code
     {
         DCF_GetNextID(&DirID,&FileID);
         DCF_MakeObjPath(DirID,FileID,DCF_FILE_TYPE_RAW,path);
         FileSys_OpenFile(path,FST_CREATE_ALWAYS|FST_OPEN_WRITE);
     }
     @endcode
*/
extern BOOL   DCF_MakeObjPathEx(DCF_HANDLE DcfHandle, UINT32  DirID, UINT32 FileID, UINT32 fileType, CHAR *filePath);

/**
     Check if input name a valid DCF Dir name.

     @param[in] pDirName: The DCF Dir Name, 8 chars, ex:"100NVTIM"
     @return directory ID,  return 0 for invalid DCF folder name
*/
extern INT32  DCF_IsValidDir(char *pDirName);

/**
     Check if input name a valid DCF file name.

     @param[in]  pFileName: 11 chars , include the externsion name but not include the dot. ex:"IMAG0001JPG"
     @param[out] pType: return the file type, JPG, WAV, or AVI ...
     @return file ID,  return 0 for invalid DCF file name
*/
extern INT32  DCF_IsValidFile(CHAR *pMainName, CHAR *pExtName, UINT32 *pType);


//DCF DB info

/**
     Get DCF database info.

     Get DCF database info, total dir count, file count, current index ...

     @param[in] infoID: The DCF info ID.
     @return the result info.
*/
extern UINT32 DCF_GetDBInfoEx(DCF_HANDLE DcfHandle, DCF_INFOID infoID);

/**
     Get file count for one file type.

     @param[in] fileType: The DCF File type. The file type can be just one file type ex: DCF_FILE_TYPE_JPG or DCF_FILE_TYPE_AVI.\n
                                 Can not be the file type combination, ex:'DCF_FILE_TYPE_JPG|DCF_FILE_TYPE_AVI'.
     @return the file count.
*/
extern UINT32 DCF_GetOneTypeFileCountEx(DCF_HANDLE DcfHandle, UINT32 fileType);

//DCF dir info

/**
     Get specific DCF Dir Info.

     @param[in] DirID: The DCF Dir ID.
     @param[out] psDirInfo: The DCF Dir Info.
     @return
         - @b TRUE:    Get Dir Info OK.
         - @b FALSE:   Can not get Dir Info, the Dir not exist in DataBase.
*/
extern BOOL   DCF_GetDirInfoEx(DCF_HANDLE DcfHandle, UINT32 DirID, PSDCFDIRINFO psDirInfo);

/**
     Get specific DCF Dir path.

     @param[in] DirID: The DCF Dir ID.
     @param[out] Path: The DCF Dir full path, ex: "A:\DCIM\100NVTIM\"
     @return
         - @b TRUE:    Get Dir path OK.
         - @b FALSE:   Can not get Dir path, the Dir not exist in DataBase.
*/
extern BOOL   DCF_GetDirPathEx(DCF_HANDLE DcfHandle, UINT32 DirID, CHAR *Path);

//DCF object info
/**
     Get DCF object info by index.

     Get DCF object DirID, FileID, fileType by index.

     @param[in] index: The DCF obejct index.
     @param[out] DirID: The DCF object DirID.
     @param[out] FileID: The DCF object FileID.
     @param[out] fileType: The DCF object fileType.
     @return
         - @b TRUE:    Get object info OK.
         - @b FALSE:   Get object info fail, input index is incorrect.
*/
extern BOOL   DCF_GetObjInfoEx(DCF_HANDLE DcfHandle, UINT32  index, UINT32 *DirID, UINT32 *FileID, UINT32 *fileType);

/**
     Get DCF object file path.

     Get DCF object file full path by DCF index.

     @param[in] index: The DCF obejct index.
     @param[in] fileType: The DCF object fileType.The file type can be just one file type ex: DCF_FILE_TYPE_JPG or DCF_FILE_TYPE_AVI\n
                                  Can not be the file type combination, ex:'DCF_FILE_TYPE_JPG|DCF_FILE_TYPE_AVI'.
     @param[out] filePath: The DCF object file full Path, ex: "A:\DCIM\100NVTIM\IMAG0001.JPG".
     @return
         - @b TRUE:    Get object path OK.
         - @b FALSE:   Get object path fail, input index out of range.
*/
extern BOOL   DCF_GetObjPathEx(DCF_HANDLE DcfHandle, UINT32  index, UINT32 fileType, CHAR *filePath);

/**
     Get DCF object file path by Dir ID, File ID.

     @param[in] DirID: The DCF object DirID.
     @param[in] FileID: The DCF object FileID.
     @param[in] fileType: The DCF object fileType.The file type can be just one file type ex: DCF_FILE_TYPE_JPG or DCF_FILE_TYPE_AVI\n
                                  Can not be the file type combination, ex:'DCF_FILE_TYPE_JPG|DCF_FILE_TYPE_AVI'.
     @param[out] filePath: The DCF object file full Path, ex: "A:\DCIM\100NVTIM\IMAG0001.JPG".
     @return
         - @b TRUE:    Get object path OK.
         - @b FALSE:   Get object path fail, input index out of range.
*/
extern BOOL   DCF_GetObjPathByIDEx(DCF_HANDLE DcfHandle, UINT32 DirID, UINT32 FileID, UINT32 fileType, CHAR *filePath);

/**
     Get DCF last index object info.

     This function can speed up playback mode power on first time to get last DCF object info.\n
     It will wait DCF scan find last object, but not wait DCF scan whole complete.\n
     So it can gain some time when DCF have a lot of directory and files.

     @note this function can't be called before DCF Scan.

     @param[out] DirID: The last DCF object DirID.
     @param[out] FileID: The last DCF object FileID.
     @param[out] fileType: The last DCF object fileType.
     @return
         - @b TRUE:    Get last object info OK.
         - @b FALSE:   Get last object info fail.
*/
extern BOOL   DCF_GetLastObjInfoEx(DCF_HANDLE DcfHandle, UINT32 *DirID, UINT32 *FileID, UINT32 *fileType);

/**
     Get DCF last index object path.

     This function can speed up playback mode power on first time to get last DCF object info.\n
     It will wait DCF scan find last object, but not wait DCF scan whole complete.\n
     So it can gain some time when DCF have a lot of directory and files.

     @note this function can't be called before DCF Scan.

     @param[in] fileType: The DCF last object fileType.The file type can be just one file type ex: DCF_FILE_TYPE_JPG or DCF_FILE_TYPE_AVI\n
                                  Can not be the file type combination, ex:'DCF_FILE_TYPE_JPG|DCF_FILE_TYPE_AVI'.
     @param[out] filePath: The DCF last object file full Path, ex: "A:\DCIM\100NVTIM\IMAG0001.JPG".
     @return
         - @b TRUE:    Get last object info OK.
         - @b FALSE:   Get last object info fail.
*/

extern BOOL   DCF_GetLastObjPathEx(DCF_HANDLE DcfHandle, UINT32 fileType, CHAR *filePath);

//DCF index control

/**
     Get DCF current index.

     @return current index.
*/
//#define       DCF_GetCurIndex()  DCF_GetDBInfo(DCF_INFO_CUR_INDEX)

/**
     Get DCF index by ID.

     @param[in] DirID: The DCF obejct DirID.
     @param[in] FileID: The DCF obejct FileID.
     @return DCF index, return 0 means the file not exist.

*/
extern UINT32 DCF_GetIndexByIDEx(DCF_HANDLE DcfHandle, UINT32 DirID, UINT32 FileID);

/**
     Move DCF current index.

     @param[in] index: The DCF obejct index.
     @return
         - @b TRUE:    Set index OK.
         - @b FALSE:   Set index fail, input index out of range.
*/
extern BOOL   DCF_SetCurIndexEx(DCF_HANDLE DcfHandle, UINT32 index);

/**
     Move DCF current index by seek cmd.

     @param[in] offset: The index jump offset.
     @param[in] seekCmd: The seek command.
     @return  result index,  return 0 means seek index fail.
*/
extern UINT32 DCF_SeekIndexEx(DCF_HANDLE DcfHandle, INT32 offset, DCF_SEEK_CMD seekCmd);

/**
     Move DCF current index by DCF ID.

     This funcion is the combination of DCF_GetIndexByID() & DCF_SetCurIndex().

     @param[in] DirID: The DCF obejct DirID.
     @param[in] FileID: The DCF obejct FileID.
     @return  result index,  return 0 means seek index fail.
*/
extern UINT32 DCF_SeekIndexByIDEx(DCF_HANDLE DcfHandle, UINT32 DirID, UINT32 FileID);


//DCF next ID control

/**
     Get DCF next write DirID , FileID.

     @note The next write DirID, FileID will be initialized after DCF scan.\n
           The ID will be auto calculated by DCF system, it may change after DCF_AddDBfile(), DCF_DelDBfile(), or DCF_Refresh().

     @param[out] DirID: The DCF next write DirID.
     @param[out] FileID: The DCF next write FileID.
     @return
         - @b TRUE:    Get next id OK.
         - @b FALSE:   Get next id fail, the DCF max object 999-9999 is exist.
*/
extern BOOL   DCF_GetNextIDEx(DCF_HANDLE DcfHandle, UINT32 *DirID, UINT32 *FileID);

/**
     Set DCF next write DirID , FileID.

     Although DCF next write DirID, FileID is auto calculated by DCF system.\n
     User still can change the ID by this API.

     @note If user called this API to set next write ID, user need to handle the next ID by yourself from now on.\n
           It means user need to re-calculated the next write ID, when add or delete DCF database file.

     @param[in] DirID: The DCF next write DirID.
     @param[in] FileID: The DCF next write FileID.
     @return
         - @b TRUE:    Set next id OK.
         - @b FALSE:   Set next id fail, the ID is incorrect.
*/
extern BOOL   DCF_SetNextIDEx(DCF_HANDLE DcfHandle, UINT32 DirID, UINT32 FileID);

/**
     Lock & get DCF next write DirID , FileID.

     Lock & get DCF next write DirID , fileID until unlock it.\n
     This API is used to avoid different application get the same next write ID.\n
     If one application called this API firstly, and another application also called this API, \n
     The second applicaion will be blocked until the first applicaion unlock it.

     @param[out] DirID: The DCF next write DirID.
     @param[out] FileID: The DCF next write FileID.
     @return
         - @b TRUE:    Get next id OK.
         - @b FALSE:   Get next id fail, the DCF max object 999-9999 is exist.
*/
extern BOOL   DCF_LockNextIDEx(DCF_HANDLE DcfHandle, UINT32 *DirID, UINT32 *FileID);

/**
     Unlock DCF next write DirID , FileID.

*/
extern void   DCF_UnlockNextIDEx(DCF_HANDLE DcfHandle);

/**
     Dump DCF Database related information.

*/
extern void   DCF_DumpInfoEx(DCF_HANDLE DcfHandle);

/**
     Register callback function.

     @param[in] CB: the callback function pointer.
*/
extern void   DCF_RegisterCBEx(DCF_HANDLE DcfHandle, DCF_CBMSG_FP CB);


//  backward compatible
#define DCF_ScanObj() DCF_ScanObjEx(0)
#define DCF_Refresh() DCF_RefreshEx(0)
#define DCF_AddDBfile(parm1) DCF_AddDBfileEx(0, parm1)
#define DCF_DelDBDir(parm1) DCF_DelDBDirEx(0, parm1)
#define DCF_DelDBfile(parm1) DCF_DelDBfileEx(0, parm1)
#define DCF_MakeDirPath(parm1,parm2) DCF_MakeDirPathEx(0, parm1,parm2)
#define DCF_MakeObjPath(parm1,parm2,parm3,parm4) DCF_MakeObjPathEx(0, parm1,parm2,parm3,parm4)
#define DCF_GetDBInfo(parm1) DCF_GetDBInfoEx(0, parm1)
#define DCF_GetOneTypeFileCount(parm1) DCF_GetOneTypeFileCountEx(0, parm1)
#define DCF_GetDirInfo(parm1,parm2) DCF_GetDirInfoEx(0, parm1,parm2)
#define DCF_GetDirPath(parm1,parm2) DCF_GetDirPathEx(0, parm1,parm2)
#define DCF_GetObjInfo(parm1,parm2,parm3,parm4) DCF_GetObjInfoEx(0, parm1,parm2,parm3,parm4)
#define DCF_GetObjPath(parm1,parm2,parm3) DCF_GetObjPathEx(0, parm1,parm2,parm3)
#define DCF_GetObjPathByID(parm1,parm2,parm3,parm4) DCF_GetObjPathByIDEx(0, parm1,parm2,parm3,parm4)
#define DCF_GetLastObjInfo(parm1,parm2,parm3) DCF_GetLastObjInfoEx(0, parm1,parm2,parm3)
#define DCF_GetLastObjPath(parm1,parm2) DCF_GetLastObjPathEx(0, parm1,parm2)
#define DCF_GetIndexByID(parm1,parm2) DCF_GetIndexByIDEx(0, parm1,parm2)
#define DCF_SetCurIndex(parm1) DCF_SetCurIndexEx(0, parm1)
#define DCF_SeekIndex(parm1,parm2) DCF_SeekIndexEx(0, parm1,parm2)
#define DCF_SeekIndexByID(parm1,parm2) DCF_SeekIndexByIDEx(0, parm1,parm2)
#define DCF_GetNextID(parm1,parm2) DCF_GetNextIDEx(0, parm1,parm2)
#define DCF_SetNextID(parm1,parm2) DCF_SetNextIDEx(0, parm1,parm2)
#define DCF_LockNextID(parm1,parm2) DCF_LockNextIDEx(0, parm1,parm2)
#define DCF_UnlockNextID() DCF_UnlockNextIDEx(0)
#define DCF_DumpInfo() DCF_DumpInfoEx(0)
#define DCF_RegisterCB(parm1) DCF_RegisterCBEx(0, parm1)

#define DCF_GetCurIndex()  DCF_GetDBInfo(DCF_INFO_CUR_INDEX)
//@}
#endif  //_DCF_H_
