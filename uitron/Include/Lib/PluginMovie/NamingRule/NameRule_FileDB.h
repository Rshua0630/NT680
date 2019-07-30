/**
    Header file of media naming rule (_FileDB) library

    Exported header file of media naming rule library.

    @file       NameRule_FileDB.h
    @ingroup    mIMEDIAWRITE
    @note       1st version.
    @version    V1.00.001
    @date       2013/02/26

    Copyright   Novatek Microelectronics Corp. 2005.  All rights reserved.
*/
#ifndef _NAMERULEFILEDB_H
#define _NAMERULEFILEDB_H

/**
    @addtogroup mIMEDIAWRITE
*/
//@{

/*
    Public funtions to use fileformat maker
*/
//@{

#define CARDV_PATHLEN  80

#define NAMERULE_FMT_JPG    0x80    ///< .JPG file, for NH_FileDBUti_MakeObjPath()
#define NAMERULE_FMT_RAW    0x81    ///< .RAW file

//Naming handler
typedef struct {
	UINT32 uiYear;
	UINT32 uiMonth;
	UINT32 uiDate;
	UINT32 uiHour;
	UINT32 uiMin;
	UINT32 uiSec;
	UINT32 uiNumber;
	UINT32 SecValid;
	char   ucChar;

} NM_NAMEINFO;

/**
    Get FileDB naming rule.

    Get FileDB naming rule.

    @param[in] void

    @return
     PMEDIANAMINGRULE DCFFull naming rule
*/
extern PMEDIANAMINGRULE NameRule_getFileDB(void);

/**
    Make file name.

    Make file name.

    @note filename will be: A:\CarDV\2013_0312_1010_xxxx.MOV (xxxx: non sense number)

    @param[in] ptimeinfo    pointer to time information
    @param[in] fileType     filetype: MEDIAREC_MOV or MEDIAREC_AVI
    @param[out] filePath    full filepath
    @param[in] frontDir     directory path, ex. A:\\CarDV\\
    @return
        BOOL if OK, return TRUE
*/
extern BOOL NH_FileDBUti_MakeObjPath(NM_NAMEINFO *ptimeinfo, UINT32 fileType, CHAR *filePath, CHAR *frontDir);


/**
    Open FileDB naming rule debug msg.

    Open FileDB naming rule debug msg.

    @param[in] on on/off

    @return void
*/
extern void NH_FileDB_OpenMsg(UINT8 on);

/**
    Set fileDB root Foldername. Default: CARDV

    @param[in] pChar    pointer to foldername
    @return void
*/
extern void NH_FileDB_SetRootFolder(char *pChar);

/**
    Set fileDB normal Foldername. Default: MOVIE

    @param[in] pChar    pointer to foldername
    @return void
*/
extern void NH_FileDB_SetNormalFolder(char *pChar);

/**
    Set fileDB read-only Foldername. Default: RO (A:\CARDV\RO)

    @param[in] pChar    pointer to foldername
    @return void
*/
extern void NH_FileDB_SetReadOnlyFolder(char *pChar);

/**
    Set fileDB jpg Foldername. Default: PHOTO

    @param[in] pChar    pointer to foldername
    @return void
*/
extern void NH_FileDB_SetJPGFolder(char *pChar);

/**
    Set fileDB EMR Foldername. Default: EMR

    @param[in] pChar    pointer to foldername
    @return void
*/
extern void NH_FileDB_SetEMRFolder(char *pChar);

/**
    Set fileDB init num. Default: 0 (next = init+1)

    @param[in] uiFileID    init num (=last)
    @return void
*/
extern void NH_FileDB_SetFileID(UINT32 uiFileID);

/**
    Set movie filetype to DELETE. Default: FILEDB_FMT_MOV|FILEDB_FMT_MP4|FILEDB_FMT_AVI

    @param[in] type    FILEDB_FMT_MOV or others
    @return void
*/
extern void NH_FileDB_SetMOVIEfiletype(UINT32 type);

/**
    Set new files will be put to last in fileDB

    @param[in] value 1:on 0:off
    @return void
*/
extern void NH_FileDB_Set_AlwaysAdd2Last(UINT32 value);

/**
    Set fileDB emr Foldername. Default: EMR

    @param[in] pChar    pointer to foldername
    @return void
*/
extern void NH_FileDB_SetEMRFolder(char *pChar);

//@}

#endif//_NAMERULEFILEDB_H

//@}

