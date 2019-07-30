/*
    Copyright   Novatek Microelectronics Corp. 2005~2014.  All rights reserved.

    @file       ImageUnit_CamFile.h
    @ingroup    mImagePipe

    @note       Nothing.

    @date       2014/10/01
*/

#ifndef IMAGEUNIT_CAMFILE_H
#define IMAGEUNIT_CAMFILE_H

/*
Support these port config API(s):

============================================================
port
------------------------------------------------------------
ImageUnit_CfgImgSize()
ImageUnit_CfgImgAspect()
ImageUnit_CfgImgWindow()
ImageUnit_CfgImgDirect()
============================================================
*/

#include "Type.h"
#include "ImageStream.h"
#include "NameRule_Custom.h"

typedef void (CustomFileNamingCB)(UINT32 uiPathId, char *pFileName);

enum {
	CAMFILE_PARAM_START = 0x00003000,
	CAMFILE_PARAM_USE_FILEDB,
	CAMFILE_PARAM_CONNECT_DELAY,
	CAMFILE_PARAM_MOVIE_FILEDB_CRATE,
	CAMFILE_PARAM_MOVIE_NAMERULE_EMPTY,
	CAMFILE_PARAM_CUSTOM_NAMING,
	CAMFILE_PARAM_FILESIZE_ALIGN,
	CAMFILE_PARAM_FILEDB_MAXNUM,
	CAMFILE_PARAM_FILEDB_REOPEN,
	CAMFILE_PARAM_FILEDB_CLOSE,
};

// file naming method (manual, semi-auto, auto)
#define FILE_NAMING_MANUAL          NMC_FILEPATH_METHOD_1
#define FILE_NAMING_SEMIAUTO        NMC_FILEPATH_METHOD_2
#define FILE_NAMING_AUTO            NMC_FILEPATH_METHOD_3
#define FILE_NAMING_METHOD_MAX      FILE_NAMING_AUTO

// file naming type of semi-auto naming method
#define FILE_NAMING_SEMIAUTO_TYPE1  NAMERULECUS_TIMETYPE_1
#define FILE_NAMING_SEMIAUTO_TYPE2  NAMERULECUS_TIMETYPE_2
#define FILE_NAMING_SEMIAUTO_TYPE3  NAMERULECUS_TIMETYPE_3
#define FILE_NAMING_SEMIAUTO_MAX    FILE_NAMING_SEMIAUTO_TYPE3

// file naming date format (YMD, MDY, DMY)
#define FILE_NAMING_DATEFMT_YMD     NAMERULECUS_YMDTYPE_YMD
#define FILE_NAMING_DATEFMT_MDY     NAMERULECUS_YMDTYPE_MDY
#define FILE_NAMING_DATEFMT_DMY     NAMERULECUS_YMDTYPE_DMY
#define FILE_NAMING_DATEFMT_MAX     FILE_NAMING_DATEFMT_DMY

extern ISF_UNIT ISF_CamFile;

extern void ImageApp_CamFile_SetFolder(CHAR *pStr);
extern CHAR *ImageApp_CamFile_GetFolder(void);
extern CHAR *ImageApp_CamFile_GetLastWriteFilePath(void);
extern void *ImageApp_CamFile_GetNamingHandler(void);

/**
    Register root folder name of Custom Naming Rule.

    Register root folder name of Custom Naming Rule.

    @param[in] pStr folder name, default: "A:\\CarDV\\"

    @return void
*/
extern void ImageApp_CamFile_SetRootFolder(CHAR *pStr);

/**
    Register Moive folder name of Custom Naming Rule.

    Register Movie folder name of Custom Naming Rule.

    @param[in] uiPathId movie path ID
    @param[in] pStr folder name, default: "MOVIE"

    @return void
*/
extern void ImageApp_CamFile_SetMovieFolder(UINT32 uiPathId, CHAR *pStr);

/**
    Register Read-only folder name of Custom Naming Rule.

    Register Read-only folder name of Custom Naming Rule.

    @param[in] uiPathId movie path ID
    @param[in] pStr folder name, default: "MOVIE\\RO"

    @return void
*/
extern void ImageApp_CamFile_SetROFolder(UINT32 uiPathId, CHAR *pStr);

/**
    Register Emergency Recording folder name of Custom Naming Rule.

    Register Emergency Recording folder name of Custom Naming Rule.

    @param[in] uiPathId movie path ID
    @param[in] pStr folder name, default: "MOVIE\\EMR"

    @return void
*/
extern void ImageApp_CamFile_SetEMRFolder(UINT32 uiPathId, CHAR *pStr);

/**
    Register Photo folder name of Custom Naming Rule.

    Register Photo folder name of Custom Naming Rule.

    @param[in] uiPathId movie path ID
    @param[in] pStr folder name, default: "PHOTO"

    @return void
*/
extern void ImageApp_CamFile_SetPhotoFolder(UINT32 uiPathId, CHAR *pStr);

/**
    Set custom file naming method.

    Set custom file naming method.

    @param[in] uiMethod method: FILE_NAMING_MANUAL, FILE_NAMING_SEMIAUTO, FILE_NAMING_AUTO

    @return void
*/
extern void ImageApp_CamFile_SetFileNamingMethod(UINT32 uiMethod);

/**
    Set file naming type of semi-auto custom file naming method.

    Set file naming type of semi-auto custom file naming method.

    @param[in] uiType file naming type: FILE_NAMING_SEMIAUTO_TYPE1, FILE_NAMING_SEMIAUTO_TYPE2, FILE_NAMING_SEMIAUTO_TYPE3

    @return void
*/
extern void ImageApp_CamFile_SetFileNamingType(UINT32 uiType);

/**
    Set file naming YMD type of semi-auto custom file naming method.

    Set file naming YMD type of semi-auto custom file naming method.

    @param[in] uiYmdType YMD type: FILE_NAMING_DATEFMT_YMD, FILE_NAMING_DATEFMT_MDY, FILE_NAMING_DATEFMT_DMY

    @return void
*/
extern void ImageApp_CamFile_SetFileNamingYMD(UINT32 uiYmdType);

/**
    Set file name end character of semi-auto custom file naming method.

    Set file name end character of semi-auto custom file naming method.

    @param[in] uiPathId recording path ID
    @param[in] bEnable  enable/disable end character
    @param[in] pStr     pointer to end character string

    @return void
*/
extern void ImageApp_CamFile_SetFileNameEndChar(UINT32 uiPathId, BOOL bEnable, CHAR *pStr);

/**
    Enable/disable file name serial number of semi-auto custom file naming method.

    Enable/disable file name serial number of semi-auto custom file naming method.

    @param[in] uiPathId recording path ID
    @param[in] bEnable  enable/disable file name serial number

    @return void
*/
extern void ImageApp_CamFile_SetFileNameSerialNum(UINT32 uiPathId, BOOL bEnable);

/**
    Activate the settings of semi-auto custom file naming method.

    Activate the settings of semi-auto custom file naming method.

    @param[in]

    @return void
*/
extern void ImageApp_CamFile_SetSemiAutoFileNaming(void);

/**
    Register movie file naming callback function for manual file naming method.

    Register movie file naming callback function for manual file naming method.

    @param[in] pMovieFileNamingCB function pointer of callback function

    @return void
*/
extern void ImageApp_CamFile_RegMovieFileNamingCB(CustomFileNamingCB *pMovieFileNamingCB);

/**
    Register photo file naming callback function for manual file naming method.

    Register photo file naming callback function for manual file naming method.

    @param[in] pPhotoFileNamingCB function pointer of callback function

    @return void
*/
extern void ImageApp_CamFile_RegPhotoFileNamingCB(CustomFileNamingCB *pPhotoFileNamingCB);

/**
    Register EMR movie file naming callback function for manual file naming method.

    Register EMR movie file naming callback function for manual file naming method.

    @param[in] pMovieFileNamingCB function pointer of callback function

    @return void
*/
extern void ImageApp_CamFile_RegEMRMovFileNamingCB(CustomFileNamingCB *pMovieFileNamingCB);

/**
    To make full path movie file name for manual file naming method.

    To make full path movie file name for manual file naming method.
    Please call it in MOVREC_EVENT_GETNAME_PATH0 event of media record CB
    if you have registered movie file naming callback function.

    @param[in] uiPathId recording path ID

    @return void
*/
extern void ImageApp_CamFile_MakeMovieFileNameCB(UINT32 uiPathId);

/**
    To make full path EMR movie file name for manual file naming method.

    To make full path EMR movie file name for manual file naming method.
    Please call it in MOVREC_EVENT_GETNAME_PATH0 event of media record CB
    if you have registered movie file naming callback function.

    @param[in] uiPathId recording path ID

    @return void
*/
extern void ImageApp_CamFile_MakeEMRMovFileNameCB(UINT32 uiPathId);


/**
    Register root folder name by Path.

    Register root folder name by Path.

    @param[in] uiPathId path ID
    @param[in] pStr folder name, default: "A:\\CarDV\\"

    @return void
*/
extern void ImageApp_CamFile_SetRootFolderByPath(UINT32 uiPathId, CHAR *pStr);


#endif //IMAGEUNIT_CAMFILE_H

