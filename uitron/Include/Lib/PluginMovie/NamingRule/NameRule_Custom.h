/**
    Header file of media naming rule library

    Exported header file of media naming rule library.

    @file       NameRule_Custom.h
    @ingroup    mIMEDIAWRITE
    @note       1st version.
    @version    V1.00.001
    @date       2015/06/01

    Copyright   Novatek Microelectronics Corp. 2005.  All rights reserved.
*/
#ifndef _NAMERULECUSTOM_H
#define _NAMERULECUSTOM_H

#include "MediaReadLib.h"
#include "MediaWriteLib.h"
#include "NameRule_FileDB.h"
/**
    @addtogroup mIMEDIAWRITE
*/
//@{
typedef enum {
	NRCUS_ID_1,                ///< namerule_ID_1 = 0
	NRCUS_ID_2,                ///< namerule_ID_2 = 1
	NRCUS_ID_3,                ///< namerule_ID_3 = 2
	NRCUS_ID_4,                ///< namerule_ID_4 = 3
	NRCUS_ID_5,                ///< namerule_ID_5 = 4
	NRCUS_ID_6,                ///< namerule_ID_6 = 5
	NRCUS_ID_7,                ///< namerule_ID_7 = 6
	NRCUS_ID_8,                ///< namerule_ID_8 = 7
	NRCUS_ID_MAX,              ///< namerule_ID maximum
	ENUM_DUMMY4WORD(NRCUS_ID)
} NRCUS_ID;

/*
    Public funtions to use fileformat maker
*/
//@{
#define NAMEHDL_CUSTOM_CHECKID  0x63757374 ///< 'cust' as checkID

typedef enum {
	NMC_FILEPATH_METHOD_1  = 1,//give full filepath
	NMC_FILEPATH_METHOD_2,  //using NH_Custom_BuildFullPath
	NMC_FILEPATH_METHOD_3,  //using default 20150615_120000_xxxA.mp4
	NMC_FILEPATH_METHOD_MAX,//using default 20150615_120000_xxxA.mp4
	ENUM_DUMMY4WORD(NMC_FILEPATH_METHO)
} NMC_FILEPATH_METHO;


#define NMC_ROOT_MAX_LEN    20
#define NMC_OTHERS_MAX_LEN  20
#define NMC_TOTALFILEPATH_MAX_LEN 80

#define NAMERULECUS_YMDTYPE_YMD   0 //ymd
#define NAMERULECUS_YMDTYPE_MDY   1 //mdy
#define NAMERULECUS_YMDTYPE_DMY   2 //dmy


#define NAMERULECUS_TIMETYPE_1   1 //yyyymmdd_hhmmss
#define NAMERULECUS_TIMETYPE_2   2 //yyyy_mmdd_hhmmss
#define NAMERULECUS_TIMETYPE_3   3 //yy-mm-dd_hhmmss
#define NAMERULECUS_TIMETYPE_GIVE  0x80 //using given name

#define NAMERULECUS_FILETYPE_MOV MEDIA_FILEFORMAT_MOV //mov
#define NAMERULECUS_FILETYPE_AVI MEDIA_FILEFORMAT_AVI //avi
#define NAMERULECUS_FILETYPE_MP4 MEDIA_FILEFORMAT_MP4 //mp4
#define NAMERULECUS_FILETYPE_JPG 0x80 //NAMERULE_FMT_JPG
#define NAMERULECUS_FILETYPE_RAW 0x81 //NAMERULE_FMT_RAW

#define NMC_SECONDTYPE_RO       1
#define NMC_SECONDTYPE_MOVIE    2
#define NMC_SECONDTYPE_PHOTO    3
#define NMC_SECONDTYPE_EMR      4
//#define NMC_SECONDTYPE_REAR     5
#define NMC_SECONDTYPE_EV1      6
#define NMC_SECONDTYPE_EV2      7
#define NMC_SECONDTYPE_EV3      8

#define NMC_FILESAVE_PART1   1//SMEDIAREC_FILESAVE_PART1
#define NMC_FILESAVE_PART2   2//SMEDIAREC_FILESAVE_PART2

typedef struct {
	UINT32 filetype;  //file type, NAMERULECUS_FILETYPE_MOV or others
	UINT32 ymdtype;   //ymd type,  NAMERULECUS_TIMETYPE_1 or others
	UINT32 timetype;  //time type, NAMERULECUS_TIMETYPE_1 or others
	UINT32 en_count;  //enable count number or not
	UINT32 en_endchar;//enable endchar or not
	char   *pEndChar;
	char   *pGiveName;

} NMC_NAMEINFO;


typedef struct {
	UINT32 pathid;
	CHAR *pRO;
	CHAR *pMovie;
	CHAR *pPhoto;
	CHAR *pEMR;
	CHAR *pEvent1;
	CHAR *pEvent2;
	CHAR *pEvent3;
} NMC_FOLDER_INFO;

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

} NMC_TIMEINFO;


typedef struct {
	UINT32 pathid;
	UINT32 secondType;//NMC_SECONDTYPE_MOVIE or others
	NMC_TIMEINFO *ptimeinfo;
	NMC_NAMEINFO *pnameinfo;

} NMC_FULLPATH_INFO;

typedef struct {
	UINT32 pathid;
	UINT32 ymdtype;    //NAMERULECUS_YMDTYPE_YMD or others
	UINT32 timetype;   //NAMERULECUS_TIMETYPE_1 or others
	UINT32 en_count;
	UINT32 en_endChar;
	CHAR   *pEndChar;

} NMC_METHOD2_INFO;
/**
    Get Emtpy naming rule.

    Get Emtpy naming rule.

    @param[in] void

    @return
     PMEDIANAMINGRULE Emtpy naming rule
*/
extern PMEDIANAMINGRULE NameRule_getCustom(void);
extern PMEDIANAMINGRULE NameRule_getCustom_byid(UINT32 g_nr_id);

/**
    Open CUSTOM naming rule debug msg.

    Open CUSTOM naming rule debug msg.

    @param[in] on on/off

    @return void
*/
extern void NH_Custom_OpenMsg(UINT8 on);
extern BOOL NH_CustomUti_MakeObjPath(NMC_TIMEINFO *ptimeinfo, NMC_NAMEINFO *pInfo, CHAR *filePath, CHAR *frontDir);
extern void NH_Custom_SetWholeDirPath(UINT32 pathid, CHAR *pPath, UINT32 sectype);
extern void NH_Custom_SetFolderPath(NMC_FOLDER_INFO *pInfo);
extern void NH_Custom_BuildFullPath(NMC_FULLPATH_INFO *pFullInfo, CHAR *pOutputName);
extern void NH_CustomUti_MakeDefaultPath(UINT32 pathid, UINT32 sectype, UINT32 filetype, CHAR *pOutputName);
extern void NH_Custom_BuildFullPath_GiveFileName(UINT32 pathid, UINT32 sectype, CHAR *pFileName, CHAR *pOutputName);
extern ER   NH_CustomUti_ChangeFrontPath(CHAR *pPath, UINT32 pathid);
extern ER   NH_CustomUti_ChangeFrontPath2nd(UINT32 oldtype, UINT32 newtype, CHAR *pPath, UINT32 pathid);
extern void NH_Custom_SetFileHandleID(UINT32 value);
extern void NH_Custom_SetFileID(UINT32 uiFileID);
extern void NH_Custom_MakeDirPath(CHAR *pFirst, CHAR *pSecond, CHAR *pPath, UINT32 pathlen);
extern void NH_Custom_SetRootPath(CHAR *pPath);
extern void NH_Custom_SetDefaultFiletype(UINT32 filetype);//MEDIA_FILEFORMAT_MOV or others
extern void NH_Custom_SetFileHandleIDByPath(UINT32 value, UINT32 pathid);

/**
    Set method to make filepath

    @param[in] NMC_FILEPATH_METHOD_3 or others (default: NMC_FILEPATH_METHOD_3)
    @return void
*/
extern void NH_Custom_SetFilepathMethod(UINT32 type);

/**
    Set method 2 parameters.

    @param[in] pInfo information to make full filepath by method2
    @return void
*/
extern void NH_Custom_SetMethod2Param(NMC_METHOD2_INFO *pInfo);

/**
    Make photo full filepath.

    @param[in] pathid 0 or 1
    @param[in] filetype NAMERULECUS_FILETYPE_JPG
    @param[in] pOutputName output full filepath
    @return void
*/
extern void NH_CustomUti_MakePhotoPath(UINT32 pathid, UINT32 filetype, CHAR *pOutputName);

/**
    Make full filepath.

    @param[in] pathid 0 or 1
    @param[in] sectype NMC_SECONDTYPE_PHOTO or others
    @param[in] filetype NAMERULECUS_FILETYPE_JPG or others
    @param[in] pFileName input filename
    @param[in] pOutputName output full filepath
    @return void
*/
extern void NH_Custom_BuildFullPath_GiveFileNameWithFiletype(UINT32 pathid, UINT32 sectype, UINT32 filetype, CHAR *pFileName, CHAR *pOutputName);

extern void NH_Custom_SetRootPathByPath(UINT32 pathid, CHAR *pPath);

//@}

#endif//_NAMERULECUSTOM_H

//@}


