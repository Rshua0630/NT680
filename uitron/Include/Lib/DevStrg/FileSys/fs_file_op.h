////////////////////////////////////////////////////////////////
/**
    Header for File System Library

    This file maintain the C language file operation interface and search functions.

    @file       fs_file_op.h
    @ingroup    mILibFileAccess
    @note       For DMA access, every file access and size must fall into word boundary.

    Copyright   Novatek Microelectronics Corp. 2012.  All rights reserved.
*/

#ifndef _FS_FILE_OP_H
#define _FS_FILE_OP_H
#include "Type.h"
#include "FileSysTsk.h"

/**
    @addtogroup mILibFileAccess
*/
//@{
//--------------------------------------
// Constant definition
//--------------------------------------

/**
    @name FS seek command
*/
//@{
#define KFS_SEEK_SET                 FST_SEEK_SET                        ///<  start of stream (see fseek)
#define KFS_SEEK_CUR                 FST_SEEK_CUR                        ///<  current position in stream (see fseek)
#define KFS_SEEK_END                 FST_SEEK_END                        ///<  end of stream (see fseek)
//@}

/**
     @name FS API return status
*/
//@{
#define FSS_OFFSET                  (-256)
#define FSS_OK                      (0)                       ///< file system status ok
#define FSS_FAIL                    ((-1)+ FSS_OFFSET)        ///< status fail
#define FSS_BUSY                    ((-2)+ FSS_OFFSET)        ///< status busy
#define FSS_UNFORMAT                ((-3)+ FSS_OFFSET)        ///< storage is unformat. if realy want to use please format first
#define FSS_EXCESS_SIZE             ((-4)+ FSS_OFFSET)        ///< access area excess storage's size
#define FSS_CANT_ACCESS             ((-5)+ FSS_OFFSET)        ///< try to access reserved area
#define FSS_ERROR_PARAM             ((-6)+ FSS_OFFSET)        ///< parameter error
#define FSS_UNKNOW_FORMAT           ((-7)+ FSS_OFFSET)        ///< unknow format
#define FSS_DUPLEX_NAME             ((-8)+ FSS_OFFSET)        ///< find the same name in folder
#define FSS_NOMORE_SPACE            ((-9)+ FSS_OFFSET)        ///< nomore space
#define FSS_OPEN_TOO_MANY           ((-10)+ FSS_OFFSET)       ///< it open too many files
#define FSS_IS_FOLDER               ((-11)+ FSS_OFFSET)       ///< we want to open a folder
#define FSS_ILLEGAL_FILENAME        ((-12)+ FSS_OFFSET)       ///< filenaem is invalid, usually due to non 8.3 filename or file path deep
#define FSS_OPERATION_DENY          ((-13)+ FSS_OFFSET)       ///< don't allow this file operation.
#define FSS_FORMAT_FAT32            ((-14)+ FSS_OFFSET)       ///< FAT 32 format, not support so far
#define FSS_NO_MORE_FILES           ((-16)+ FSS_OFFSET)       ///< use in search file, it means no more file in folder/
#define FSS_CARD_ERROR              ((-17)+ FSS_OFFSET)       ///< card error
#define FSS_STRSIZE_MISMATCH        ((-18)+ FSS_OFFSET)       ///< Storage Size mismatch, check Nand total sectors match PBR Setting
#define FSS_FILE_NOTEXIST           ((-19)+ FSS_OFFSET)       ///< file is not exist
#define FSS_DATA_DAMAGE             ((-20)+ FSS_OFFSET)       ///< data damage
//@}


/**
     @name Directory entry attribute bit
*/
//@{
#define FS_ATTRIB_READ              FST_ATTRIB_READONLY             ///< bit 0, indicates the file is read only.
#define FS_ATTRIB_HIDDEN            FST_ATTRIB_HIDDEN               ///< bit 1, indicates the file should not be shown in normal directory listings.
#define FS_ATTRIB_SYSTEM            FST_ATTRIB_SYSTEM               ///< bit 2, indicates the file is an operating system file.
#define FS_ATTRIB_VOLUME            FST_ATTRIB_VOLUME               ///< bit 3, indicates the entry is the label for the volume.
#define FS_ATTRIB_DIRECTORY         FST_ATTRIB_DIRECTORY            ///< bit 4, indicates it is a directory
#define FS_ATTRIB_ACHIEVE           FST_ATTRIB_ARCHIVE              ///< bit 5, indicates the file has been created or modified since the attribute was clear
//@}

/**
     @name FS attribute macro
*/
//@{
#define M_IsReadOnly(x)             ((x) & FS_ATTRIB_READ)          ///< if the file is read only.
#define M_IsHidden(x)               ((x) & FS_ATTRIB_HIDDEN)        ///< if the file is a hidden file.
#define M_IsSystem(x)               ((x) & FS_ATTRIB_SYSTEM)        ///< if the file is an operating system file.
#define M_IsVolumeLabel(x)          ((x) & FS_ATTRIB_VOLUME)        ///< if the entry is the label for the volume.
#define M_IsDirectory(x)            ((x) & FS_ATTRIB_DIRECTORY)     ///< if it is a directory.
#define M_IsArchive(x)              ((x) & FS_ATTRIB_ACHIEVE)       ///< if the file has been created or modified.
#define M_IsFile(x)                 (!((x) & FS_ATTRIB_DIRECTORY))  ///< if it is a file.
//@}

/**
    @name FS get date macro
    parse the date field to year, month, day
*/
//@{
#define FS_GET_DAY_FROM_DATE(x)     (x & 0x1F)                  ///<  get day from date
#define FS_GET_MONTH_FROM_DATE(x)   ((x >> 5) & 0x0F)           ///<  get month from date
#define FS_GET_YEAR_FROM_DATE(x)    (((x >> 9) & 0x7F)+1980)    ///<  get year from date
//@}

/**
    @name FS get time macro
    parse the time field to hour, minute, second
*/
//@{
#define FS_GET_SEC_FROM_TIME(x)     ((x & 0x001F)<<1)           ///<  get seconds from time
#define FS_GET_MIN_FROM_TIME(x)     ((x & 0x07E0)>>5)           ///<  get minutes from time
#define FS_GET_HOUR_FROM_TIME(x)    ((x & 0xF800)>>11)          ///<  get hours from time
//@}

/**
    The storage object pointer.
*/
typedef UINT32 HNVT_STRG;


//@}
#endif //_FS_FILE_OP_H

