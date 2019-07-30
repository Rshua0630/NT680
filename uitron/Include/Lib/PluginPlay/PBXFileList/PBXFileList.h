/**
    Playback FileList plugin module.

    All function for Playback fileList handle, This module can be a plug-in to Application Playback.

    @file       PBXFileList.h
    @ingroup    mILibPbFileList

    Copyright   Novatek Microelectronics Corp. 2013.  All rights reserved.
*/

#ifndef _PBXFILELIST_H
#define _PBXFILELIST_H


/**
    @addtogroup mILibPbFileList
*/
//@{

#define PBX_FLIST_NAME_MAX_LENG                  60

/**

    The fileList support file type.
*/
//@{
#define PBX_FLIST_FILE_TYPE_JPG                 (0x0001)  ///< bit 0 -> JPG file type, file extension is *.JPG
#define PBX_FLIST_FILE_TYPE_AVI                 (0x0002)  ///< bit 1 -> AVI file type, file extension is *.AVI
#define PBX_FLIST_FILE_TYPE_WAV                 (0x0004)  ///< bit 2 -> WAV file type, file extension is *.WAV
#define PBX_FLIST_FILE_TYPE_RAW                 (0x0008)  ///< bit 3 -> RAW file type, file extension is *.RAW
#define PBX_FLIST_FILE_TYPE_TIF                 (0x0010)  ///< bit 4 -> TIF file type, file extension is *.TIF
#define PBX_FLIST_FILE_TYPE_MPO                 (0x0020)  ///< bit 5 -> MPO file type, file extension is *.MPO
#define PBX_FLIST_FILE_TYPE_MOV                 (0x0040)  ///< bit 6 -> MOV file type, file extension is *.MOV
#define PBX_FLIST_FILE_TYPE_MP4                 (0x0080)  ///< bit 7 -> MP4 file type, file extension is *.MP4
#define PBX_FLIST_FILE_TYPE_MPG                 (0x0100)  ///< bit 8 -> MPG file type, file extension is *.MPG
#define PBX_FLIST_FILE_TYPE_ASF                 (0x0200)  ///< bit 9 -> ASF file type, file extension is *.ASF
#define PBX_FLIST_FILE_TYPE_TS                  (0x0400)  ///< bit 10 -> TS file type, file extension is *.TS
#define PBX_FLIST_FILE_TYPE_ANYFORMAT           (0x07FF)  ///< any file format means JPG|AVI|WAV|RAW|TIF|MPO|MOV|MP4|MPG|ASF
//@}


/**
     The fileList sorting type.
*/
typedef enum _PBX_FLIST_SORT_TYPE {
	PBX_FLIST_SORT_BY_NONE = 0x00,               ///< no sort, the sequence depends on the file sequence in disk
	PBX_FLIST_SORT_BY_CREDATE,                   ///< sort by the file create time
	PBX_FLIST_SORT_BY_MODDATE,                   ///< sort by the file modify time
	PBX_FLIST_SORT_BY_NAME,                      ///< sort by the file name
	PBX_FLIST_SORT_BY_FILEPATH,                  ///< sort by the full file path
} PBX_FLIST_SORT_TYPE;


/**
     The fileList config ID.

     @note for the parameter ConfigID of Config() function.
*/
typedef enum _PBX_FLIST_CONFIG_ID {
	PBX_FLIST_CONFIG_MEM                        = 0x01,    ///< only for FileDB, the heap memory need to used
	PBX_FLIST_CONFIG_MAX_FILENUM                = 0x02,    ///< only for FileDB, the max files number
	PBX_FLIST_CONFIG_MAX_FILEPATH_LEN           = 0x03,    ///< only for FileDB, the max file path length
	PBX_FLIST_CONFIG_VALID_FILETYPE             = 0x04,    ///< set valid file type, ex:PBX_FLIST_FILE_TYPE_JPG|PBX_FLIST_FILE_TYPE_AVI
	PBX_FLIST_CONFIG_DEP_FILETYPE               = 0x05,    ///< only for DCF, set DCF dependence file format
	PBX_FLIST_CONFIG_DCF_ONLY                   = 0x06,    ///< only for FileDB, if search the file just keep the DCF file only and filter out other files.
	PBX_FLIST_CONFIG_SORT_TYPE                  = 0x07,    ///< only for FileDB, sorting the Files by sortType.
	PBX_FLIST_CONFIG_ROOT_PATH                  = 0x08,    ///< only for FileDB, set the root path
	PBX_FLIST_CONFIG_DEFAULT_FOLDER             = 0x09,    ///< only for FileDB, set the default folder
	PBX_FLIST_CONFIG_FILTER_FOLDER              = 0x0A,    ///< only for FileDB, set the filter folder
	PBX_FLIST_CONFIG_SUPPORT_LONGNAME           = 0x0B,    ///< only for FileDB, set if support long file name
	PBX_FLIST_CONFIG_SORT_LARGEFIRST            = 0x0C,    ///< only for FileDB, sorting sequence is large first or not
	PBX_FLIST_CONFIG_SKIPDIR_FOR_NONRECURSIVE   = 0x0D,    ///< only for FileDB, set if skip folders for non-recursive scan
	PBX_FLIST_CONFIG_SKIPHIDDEN                 = 0x0E,    ///< only for FileDB, set if skip hidden files and folders
	ENUM_DUMMY4WORD(PBX_FLIST_CONFIG_ID)
} PBX_FLIST_CONFIG_ID;

/**
     The fileList set info ID.

     @note for the parameter InfoID of SetInfo() function.
*/

typedef enum _PBX_FLIST_SETINFO_ID {
	PBX_FLIST_SETINFO_CURINDEX         = 0x21,
	ENUM_DUMMY4WORD(PBX_FLIST_SETINFO_ID)
} PBX_FLIST_SETINFO_ID;


/**
     The fileList get info ID.

     @note for the parameter InfoID of GetInfo() function.
*/

typedef enum _PBX_FLIST_GETINFO_ID {
	PBX_FLIST_GETINFO_FILESEQ          = 0x41,    ///< File sequence ID
	PBX_FLIST_GETINFO_FILENUMS         = 0x42,    ///< Total file number
	PBX_FLIST_GETINFO_READONLY         = 0x43,    ///< Is this file Read only
	PBX_FLIST_GETINFO_FILESIZE64       = 0x44,    ///< File size (UINT64)
	PBX_FLIST_GETINFO_FILETYPE         = 0x45,    ///< File type
	PBX_FLIST_GETINFO_FILEPATH         = 0x46,    ///< File path (CHAR *)
	PBX_FLIST_GETINFO_ISWITHMEMO       = 0x47,    ///< Is this file with Vioce memo
	PBX_FLIST_GETINFO_FILEID           = 0x48,    ///< only for DCF, the current file ID
	PBX_FLIST_GETINFO_DIRID            = 0x49,    ///< only for DCF, the current folder ID
	PBX_FLIST_GETINFO_VALID_FILETYPE   = 0x4A,    ///< only for DCF, the valid file type of DCF
	PBX_FLIST_GETINFO_FILENUMS_INDIR   = 0x4B,    ///< only for DCF, the file number count of current folder
	PBX_FLIST_GETINFO_FILESEQ_INDIR    = 0x4C,    ///< only for DCF, the file sequence in current folder

	PBX_FLIST_GETINFO_MODDATE          = 0x4D,    ///< only for FileDB, the file modify date

	PBX_FLIST_GETINFO_DB_HANDLE        = 0x61,    ///< DB handle
	ENUM_DUMMY4WORD(PBX_FLIST_GETINFO_ID)
} PBX_FLIST_GETINFO_ID;

/**
    seek index command.

    @note for the parameter seekCmd of SeekIndex() function.
*/
typedef enum _PBX_FLIST_SEEK_CMD {
	PBX_FLIST_SEEK_SET = 0,                      ///< set index to offset
	PBX_FLIST_SEEK_CUR,                          ///< jump offset from current index
	PBX_FLIST_SEEK_END,                          ///< jump offset from last index
	PBX_FLIST_SEEK_MAX_ID,                       ///< total seek command numbers
	ENUM_DUMMY4WORD(PBX_FLIST_SEEK_CMD)
} PBX_FLIST_SEEK_CMD;


/**
    Type defintion struture of playback file list object.

    Type defintion struture of playback file list object functions.
*/
typedef struct {
	ER(*Config)(UINT32 ConfigID, UINT32 param1, UINT32 param2);      ///< Config some setting for file list
	ER(*Init)(void);                                                 ///< initialize file list
	ER(*UnInit)(void);                                               ///< Un-initialize file list
	ER(*GetInfo)(UINT32  InfoID, VOID *pparam1, VOID *pparam2);      ///< Get some info of file list
	ER(*SetInfo)(UINT32  InfoID, UINT32 param1, UINT32 param2);      ///< Set some info of file list
	ER(*SeekIndex)(INT32 offset, PBX_FLIST_SEEK_CMD seekCmd);        ///< Seek the current index of file list
	ER(*MakeFilePath)(UINT32  fileType, CHAR *path);                 ///< Make a file path for create a new file
	ER(*AddFile)(CHAR *path);                                        ///< Add a file to the file list
	ER(*Refresh)(void);                                              ///< Refresh the file list data base
} PBX_FLIST_OBJ, *PPBX_FLIST_OBJ;


//@}

#endif //_PBXFILELIST_H

