///////////////////////////////////////////////////////////////////
/**
    UserLog module.

    The module supports to store some user spceific log messages to a stoarge(ex: SD card).
    Userlog support open multiple handles at the same time, the maximum limit can refer USERLOG_MAX_HANDLE_NUM.
    And user can specify differnt settings (struct USERLOG_FILE_OPEN) for different handle.
    When a string was putted to a userlog file, the string is cached in temp buffer \n
    and will not flushed to storage immediately until autoFlushTimeSec timeout.
    The filename is created automtically by datetime.
    When the file data reaches maxFileSize will be cutted and a new file will be created.
    The total file number on the userlog path will not exceeds maxFileNum.
    Whe total file number on the userlog path reaches maxFileNum, the oldest file will be deleted.

    @file       userlog.h
    @note       Nothing.

    Copyright   Novatek Microelectronics Corp. 2016.  All rights reserved.

*/
#ifndef _USERLOG_H
#define _USERLOG_H

#define USERLOG_ROOT_DIR_MAX_LEN        32        ///<  UserLog root directory maximum length
#define USERLOG_MAX_FILENUM             20        ///<  UserLog maximum file number
#define USERLOG_DEFAULT_MAX_FILENUM     10        ///<  UserLog default maximum file number
#define USERLOG_MAX_FILESIZE            0xA00000  ///<  UserLog maximum file size of each file
#define USERLOG_MIN_FILESIZE            0x004000  ///<  UserLog minimun file size of each file
#define USERLOG_DEFAULT_MAX_FILESIZE    0x100000  ///<  UserLog default maximum file size of each file
#define USERLOG_MIN_WORKBUFF_SIZE       4096      ///<  UserLog minimun working buffer size
#define USERLOG_DEFAULT_FLUSH_TIME      5         ///<  UserLog default auto flush time

#define USERLOG_MAX_HANDLE_NUM          2         ///<  UserLog maximum file handle number


/**
    UserLog Handle.
*/
typedef UINT32  *USERLOG_HANDLE;


/**
   Warning Code.

   Any warning code occured will display on uart or return.
*/
typedef enum {
	USERLOG_WR_OK                =   0, ///< no warning
	USERLOG_WR_ALREADY_OPEN      =  -1, ///< userlog_open called twice
	USERLOG_WR_NOT_OPEN          =  -2, ///< userlog_open not to call before.
	ENUM_DUMMY4WORD(USERLOG_WR)
} USERLOG_WR;




/**
    Init parameters for UserLog open a new file handle.

    This structure is used in userlog_fopen() to specify the needed information for normal working.
*/
typedef struct {
	UINT32                  workBufAddr;                               ///<  the working buffer address
	UINT32                  workBufSize;                               ///<  the working buffer size, should not smaller than USERLOG_MIN_WORKBUFF_SIZE
	CHAR                    rootDir[USERLOG_ROOT_DIR_MAX_LEN + 1];     ///<  the root directory path of log files
	UINT32                  maxFileNum;                                ///<  the maximum log files number that can keep in storage, can not exceeds USERLOG_MAX_FILENUM.
	///<  0 means use default, the default value is USERLOG_DEFAULT_MAX_FILENUM.
	UINT32                  maxFileSize;                               ///<  the maximum filesize of each log file, can not exceeds USERLOG_MAX_FILESIZE.
	///<  0 means use default, the default value is USERLOG_DEFAULT_MAX_FILESIZE.
	UINT32                  autoFlushTimeSec;                          ///<  the time for auto flush buffer data to storage, the unit is sec. 0 means use default "USERLOG_DEFAULT_FLUSH_TIME".
} USERLOG_FILE_OPEN;


/**
     Install userlog flag and semaphore id.

*/
extern void       userlog_InstallID(void) _SECTION(".kercfg_text");

/**
    Open userlog main task and init some internal status of userlog module.

    This is the userlog task initial function.

    @return
        - @b E_OK: initial success.
        - @b E_SYS: the initial has some error.

*/
extern ER userlog_open(void);

/**
    Open a userlog file handle.

    Open a userlog file handle. The maximum supported file handle number is defined by USERLOG_MAX_HANDLE_NUM.
    User can't create file handle number exceeds USERLOG_MAX_HANDLE_NUM at the same time.

    @return
        - @b E_OK: initial success.
        - @b E_SYS: the initial has some error.

    Example:
    @code
    {
        USERLOG_FILE_OPEN   fOpenParm={0};
        UINT32              workBufSize = 8192;
        UINT32              autoflushtime = 5;
        UINT32              maxFileNum = 10;
        UINT32              maxFileSize = 0x300000;
        UINT32              bIsRoot = FALSE;
        CHAR                rootDir[USERLOG_ROOT_DIR_MAX_LEN+1]="A:\\Novatek\\USERLOG\\";
        USERLOG_HANDLE      userlog_hdl;

        fOpenParm.workBufAddr= SxCmd_GetTempMem(0x10000);
        fOpenParm.workBufSize = workBufSize;
        fOpenParm.autoFlushTimeSec = autoflushtime;
        fOpenParm.maxFileNum = maxFileNum;
        fOpenParm.maxFileSize = maxFileSize;
        strncpy(fOpenParm.rootDir, rootDir, USERLOG_ROOT_DIR_MAX_LEN);
        // get a userlog file handle
        userlog_hdl = userlog_fopen(&fOpenParm);
        if (userlog_hdl == NULL)
        {
            DBG_ERR("userlog fopen has error\r\n");
        }
        // put string to this userlog handle
        userlog_fputs(userlog_hdl,"string1\r\n");
        userlog_fputs(userlog_hdl,"string2\r\n");

        // close userlog handle
        userlog_fclose(userlog_hdl);
    }
    @endcode
*/
extern USERLOG_HANDLE userlog_fopen(USERLOG_FILE_OPEN *pfOpen);


/**
    Put a string to one userlog file.

    Put a string to one userlog file, the string is cached in temp buffer until
    the autoFlushTimeSec timeout, the data will be flushed to storage.
    The filename is created automtically by datetime.
    When the file data exceeds maxFileSize will be cutted and create a new file .
    The total file number on the userlog path will not exceeds maxFileNum.
    Whe total file number on the userlog path reaches maxFileNum, the oldest file will be deleted.

    @param[in] ulogHdl        The userlog file handle.
    @param[in] str            The string to put to file.

    @return
        - @b E_OK: initial success.
        - @b E_SYS: the initial has some error.

*/
extern ER userlog_fputs(USERLOG_HANDLE  ulogHdl, const char *str);

/**
    Write data to one userlog file.

    Write data to one userlog file, the data is cached in temp buffer until
    the autoFlushTimeSec timeout, the data will be flushed to storage.
    The filename is created automtically by datetime.
    When the file data exceeds maxFileSize will be cutted and create a new file .
    The total file number on the userlog path will not exceeds maxFileNum.
    Whe total file number on the userlog path reaches maxFileNum, the oldest file will be deleted.

    @param[in] ulogHdl        The userlog file handle.
    @param[in] buffer         The data to write to file.
    @param[in] wsize          The data size to write to file.

    @return
        - @b E_OK: initial success.
        - @b E_SYS: the initial has some error.

*/
extern ER userlog_fwrite(USERLOG_HANDLE  ulogHdl, void *buffer, UINT32 wsize);


/**
    Close a userlog file handle.

    Close a userlog file handle, when close a file handle the buffer cached in temp buffer will be flushed to storage immediately.

    @param[in] ulogHdl        The userlog file handle.

    @return
        - @b E_OK: Close success.

*/
extern ER userlog_fclose(USERLOG_HANDLE  ulogHdl);


/**
    Close userlog main task and closed all opened userlog file handle.

    @return
        - @b E_OK: Close success.

*/
extern ER userlog_close(void);

/**
    Dump some internal variables and status of userlog module for debug using.

    @return None.
*/
extern void userlog_dump_debuginfo(void);

#endif //_USERLOG_H
