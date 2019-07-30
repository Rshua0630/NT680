/**
    Playback File plugin module.

    All function for Playback file handle, This module can be a plug-in to Application Playback.

    @file       PBXFile.h
    @ingroup    mILibPbFilePlugIn

    Copyright   Novatek Microelectronics Corp. 2011.  All rights reserved.
*/


#ifndef _PBXFILE_H
#define _PBXFILE_H
#include "Type.h"


/**
    @addtogroup mILibPbFilePlugIn
*/

/**
     File Info ID

*/
typedef enum _PBX_FILEINFO {
	PBX_FILEINFO_FILEID = 0x01,     ///< File ID
	PBX_FILEINFO_FILESIZE,          ///< File size
	PBX_FILEINFO_FILEFORMAT,        ///< File format
	PBX_FILEINFO_FILESEQ,           ///< File sequence ID
	PBX_FILEINFO_FILENUMS,          ///< Total file number
	PBX_FILEINFO_DIRNUMS,           ///< Total folder number
	PBX_FILEINFO_MAXDIRID,          ///< Maximun directory ID
	PBX_FILEINFO_DIRID,             ///< Directory ID
	PBX_FILEINFO_FILENUMS_INDIR,    ///< Total file number in current directory
	PBX_FILEINFO_FILESEQ_INDIR,     ///< File sequence ID in current directory
	PBX_FILEINFO_MAXFILEID_INDIR,   ///< Maximun file ID in current directory
	PBX_FILEINFO_ISFILEREADONLY,    ///< Is this file Read only
	PBX_FILEINFO_ISWITHMENO,        ///< Is this file with Vioce memo
	ENUM_DUMMY4WORD(PBX_FILEINFO)
} PBX_FILEINFO;

/**
     File operation command.

*/
typedef enum _PBX_FILE_CMD {
	PBX_FILE_CMD_READ = 0x00,       ///< Read a file
	PBX_FILE_CMD_WRITE,             ///< Write a file
	PBX_FILE_CMD_UPDATE,            ///< Update a file
	ENUM_DUMMY4WORD(PBX_FILE_CMD)
} PBX_FILE_CMD;

/**
     File Access parameter. (Support file size > 4GB)

*/
typedef struct _PBXFile_Access_Info64 {
	PBX_FILE_CMD  fileCmd;          ///< File operation command
	UINT8        *fileName;         ///< File name, should be full path, e.g. "A:\DCIM\100NVTIM\IMAG0001.JPG"
	UINT8        *pBuf;             ///< File R/W buffer
	UINT32        bufSize;          ///< File R/W buffer size
	UINT64        filePos;          ///< File R/W position
} PBXFile_Access_Info64, *PPBXFile_Access_Info64;

/**
     Wait File System init finish.

     This function is just check if the file system status is init finish, it will not wait infinite.

     @return ths status of init finish
         - @b TRUE:   FileSys init finish
         - @b FALSE:  FileSys not init finish yet
*/
extern BOOL   PBXFile_WaitInit(void);


/**
     Get file info.

     @param[in]  WhichInfo  the file info ID
     @param[in]  parm1 is not used now, just reserved

     @return the info of specified info id

     Example:
     @code
     {
        // Current DCF File ID
        FileID=PBXFile_GetInfo(PBX_FILEINFO_FILEID,0);
        // Current File size
        FileSize=PBXFile_GetInfo(PBX_FILEINFO_FILESIZE,0);
        // Current File format
        FileFormat=PBXFile_GetInfo(PBX_FILEINFO_FILEFORMAT,0);
        // Current File sequence ID
        FileSequence=PBXFile_GetInfo(PBX_FILEINFO_FILESEQ,0);
        // Total DCF file number
        FileNums=PBXFile_GetInfo(PBX_FILEINFO_FILENUMS,0);
        // Total DCF folder number
        DirNums=PBXFile_GetInfo(PBX_FILEINFO_DIRNUMS,0);
        // Maximun DCF directory ID
        MaxDirID=PBXFile_GetInfo(PBX_FILEINFO_MAXDIRID,0);
        // Current DCF Directory ID
        DirID=PBXFile_GetInfo(PBX_FILEINFO_DIRID,0);
        // Total file number in current DCF directory
        FilenumsInDir=PBXFile_GetInfo(PBX_FILEINFO_FILENUMS_INDIR,0);
        // File sequence ID in current DCF directory
        FileseqInDir=PBXFile_GetInfo(PBX_FILEINFO_FILESEQ_INDIR,0);
        // Maximun DCF file ID in current directory
        MaxFileIDInDir=PBXFile_GetInfo(PBX_FILEINFO_MAXFILEID_INDIR,0);
        // If current file is Read only
        IsReadOnly=PBXFile_GetInfo(PBX_FILEINFO_ISFILEREADONLY,0);
        // If current file with Vioce memo
        IsWithMemo=PBXFile_GetInfo(PBX_FILEINFO_ISWITHMENO,0);
     }
     @endcode

*/
extern UINT32 PBXFile_GetInfo(PBX_FILEINFO  WhichInfo, UINT32 parm1);

/**
     Get a file time.

     Get a file time from the file entry, file time include create datetime & modified datetime.

     @param[in]  szFileName  the file name, should the file full path, ex: "A:\DCIM\100NVTIM\IMAG0001.JPG"
     @param[out]  creDateTime the file create date & time, ex: creDateTime[6] = {2012,5,5, 10, 9, 11};  -> year/month/day/hour/minute/second
     @param[out]  modDateTime the file modified date & time, ex: modDateTime[6] = {2013,6,6, 13,12,14}; -> year/month/day/hour/minute/second

     @return ths status of get file time
         - @b E_OK:   Get file time success
         - @b E_SYS:  Get file time error.

     Example:
     @code
     {
        UINT32   uiCreDateTime1[6] = {0};
        UINT32   uiModDateTime1[6] = {0};
        char     tmpfilename[50];

        sprintf(tmpfilename,"A:\\timetest.txt");
        PBXFile_GetTime(tmpfilename, uiCreDateTime1,  uiModDateTime1);
     }
     @endcode
*/
extern ER     PBXFile_GetTime(char *szFileName, UINT32  creDateTime[6], UINT32  modDateTime[6]);

/**
     Set a file time.

     Set a file time to the file entry, the file time include create datetime & modified datetime.

     @param[in]  szFileName  the file name, should the file full path, ex: "A:\DCIM\100NVTIM\IMAG0001.JPG"
     @param[in]  creDateTime the file create date & time, ex: creDateTime[6] = {2012,5,5, 10, 9, 11}; -> year/month/day/hour/minute/second
     @param[in]  modDateTime the file modified date & time, ex:modDateTime[6] = {2013,6,6, 13,12,14}; -> year/month/day/hour/minute/second

     @return ths status of set file time
         - @b E_OK:   Set file time success
         - @b E_SYS:  Set file time error.


     Example:
     @code
     {
        UINT32   uiCreDateTime1[6] = {2012,5,5, 10,9, 11};
        UINT32   uiModDateTime1[6] = {2013,6,6, 13,12,14};
        char     tmpfilename[50];

        sprintf(tmpfilename,"A:\\timetest.txt");
        PBXFile_SetTime(tmpfilename, uiCreDateTime1,  uiModDateTime1);
     }
     @endcode
*/
extern ER     PBXFile_SetTime(char *szFileName, UINT32  creDateTime[6], UINT32  modDateTime[6]);

/**
     Access a file (Support file size > 4GB).

     Read/Write a file data with specific position.

     @param[in]  pfileAccess descrption
     @note
     -# This API will open the file first, then seek to the file position to do the fileCmd. Finally it will close the file.
     -# This API is almost the same as PBXFile_Access, but it supports file size > 4GB.

     @return ths result of access a file
         - @b E_PAR:  Input parameter error
         - @b E_OK:   Access file success

     Example:
     @code
     {
        // write a file
        {
            CHAR filePath[20]="A:\\TEST.JPG";

            fileAccess.fileCmd = PBX_FILE_CMD_WRITE;
            fileAccess.fileName = (UINT8*)filePath;
            fileAccess.pBuf = (UINT8*)writeBuf;
            fileAccess.bufSize = fileSize;
            fileAccess.filePos = 0;
            uiErrCheck = PBXFile_Access(&fileAccess);
        }
        // read a file
        {
            CHAR filePath[20]="A:\\TEST.JPG";

            fileAccess.fileCmd = PBX_FILE_CMD_READ;
            fileAccess.fileName = (UINT8*)filePath;
            fileAccess.pBuf = (UINT8*)((UINT32)readBuf+pos);
            fileAccess.bufSize = bufSize;
            fileAccess.filePos = pos;
            PBXFile_Access(&fileAccess);
        }
        // update a file
        {
            UINT32 uiFilePos,uiFileSize=2;
            PBXFile_Access_Info  fileAccess = {0};
            CHAR filePath[20]="A:\\TEST.JPG";

            uiFilePos = (UINT32)pucFileBuf - guiPlayFileBuf;
            fileAccess.fileCmd = PBX_FILE_CMD_UPDATE;
            fileAccess.fileName = (UINT8*)filePath;
            fileAccess.pBuf = (UINT8*)pucFileBuf;
            fileAccess.bufSize = uiFileSize;
            fileAccess.filePos = uiFilePos;
            PBXFile_Access(&fileAccess);
        }
     }
     @endcode

*/
extern ER     PBXFile_Access64(PPBXFile_Access_Info64 pFileAccess64);

/**
     Get the current storage free space of filesystem.

     @return the free space of FileSys storage.

     Example:
     @code
     {
        UINT64 ui64StorageRemainSize;
        ui64StorageRemainSize = PBXFile_GetFreeSpace();
     }
     @endcode

*/
extern UINT64 PBXFile_GetFreeSpace(void);

#endif

