///////////////////////////////////////////////////////////////////
/**
    LogFile module.

    The module supports to store the uart log messages to a stoarge(ex: SD card).

    @file       LogFile.h
    @note       Nothing.

    Copyright   Novatek Microelectronics Corp. 2016.  All rights reserved.

*/
#ifndef _LOGFILE_H
#define _LOGFILE_H

#define LOGFILE_ROOT_DIR_MAX_LEN        32         ///<  LogFile root directory maximum length
#define LOGFILE_MAX_FILENUM             32         ///<  LogFile maximum file number
#define LOGFILE_DEFAULT_MAX_FILENUM     32         ///<  LogFile default maximum file number
#define LOGFILE_MAX_FILESIZE            0x200000   ///<  LogFile maximum file size of each file
#define LOGFILE_MIN_FILESIZE            0x010000   ///<  LogFile minimun file size of each file
#define LOGFILE_DEFAULT_MAX_FILESIZE    0x100000   ///<  LogFile default maximum file size of each file
#if defined(_BSP_NA51000_)
#define LOGFILE_BUFFER_SIZE				70*1024    ///<  LogFile bufferd maximum bytes
#else
#define LOGFILE_BUFFER_SIZE				60*1024    ///<  LogFile bufferd maximum bytes
#endif

/**
   Warning Code.

   Any warning code occured will display on uart or return.
*/
typedef enum {
	LOGFILE_WR_OK                =   0, ///< no warning
	LOGFILE_WR_ALREADY_OPEN      =  -1, ///< LogFile_Open called twice
	LOGFILE_WR_NOT_OPEN          =  -2, ///< LogFile_Open not to call before.
	ENUM_DUMMY4WORD(LOGFILE_WR)
} LOGFILE_WR;

/**
   Console output type.

   Any warning code occured will display on uart or return.
*/
#define LOGFILE_CON_NONE                0x00      ///<  console intput/output none.
#define LOGFILE_CON_UART                0x01      ///<  console output to uart
#define LOGFILE_CON_MEM                 0x02      ///<  console output keep in memory, only store system error log
#define LOGFILE_CON_STORE               0x04      ///<  console output to storage


/**

The TimeType the begin with each line log

*/
typedef enum {
	LOGFILE_TIME_TYPE_NONE   	 =   0, 			///< No Time
	LOGFILE_TIME_TYPE_DATETIME   =   1, 			///< RTC DateTime
	LOGFILE_TIME_TYPE_COUNTER    =   2, 			///< HW clock long counter, the time start from power on.
	ENUM_DUMMY4WORD(LOGFILE_TIME_TYPE)
} LOGFILE_TIME_TYPE;

/**
    Init parameters for LogFile module.

    This structure is used in LogFile_Open() to specify the LogFile module needed information for normal working.
*/
typedef struct {
	UINT32					ConType;									///<  The Console type value, ex: LOGFILE_CON_UART|LOGFILE_CON_STORE
	LOGFILE_TIME_TYPE		TimeType;									///<  The TimeType the begin with each line log
	UINT32					LogBuffAddr;								///<  the last time system error log buffer address
	UINT32					LogBuffSize;								///<  the last time system error log buffer size
} LOGFILE_CFG;


/**
    Init parameters for LogFile module.

    This structure is used in LogFile_Open() to specify the LogFile module needed information for normal working.
*/
typedef struct {
	CHAR                    rootDir[LOGFILE_ROOT_DIR_MAX_LEN + 1];      ///<  the root directory path of log files
	CHAR                    sysErrRootDir[LOGFILE_ROOT_DIR_MAX_LEN + 1];///<  the root directory path of system error log files
	UINT32                  maxFileNum;                                 ///<  the maximum log files number that can keep in storage, can not exceeds LOGFILE_MAX_FILENUM.
																		///<  0 means use default, the default value is LOGFILE_DEFAULT_MAX_FILENUM.
	UINT32                  maxFileSize;                                ///<  the maximum filesize of each log file, can not exceeds LOGFILE_MAX_FILESIZE.
																		///<  0 means use default, the default value is LOGFILE_DEFAULT_MAX_FILESIZE.
	BOOL                    isPreAllocAllFiles;                         ///<  if pre-alloc all log files when log file folder is emtpy.
	BOOL                    isSaveLastTimeSysErrLog;                    ///<  if save last time system error log to file.
	UINT32                  lastTimeSysErrLogBuffAddr;					///<  the last time system error log buffer address
	UINT32                  lastTimeSysErrLogBuffSize;					///<  the last time system error log buffer size
} LOGFILE_OPEN;


typedef struct {
	UINT32                  reserved;									///<  the reserved word for memory pool init will reset first word to zero.
	UINT32                  tag;										///<  the verify tag for this data
	UINT32                  size;                                		///<  the data size
	CHAR                   *data;                                		///<  the data
} LOGFILE_DATA_HEAD;


/**
    Register and set LogFile console.

    @param[in] ConType:  The Console type value.

    Example:
    @code
    {
        LOGFILE_CFG   cfg = {0};

		cfg.ConType = LOGFILE_CON_UART|LOGFILE_CON_STORE;
		cfg.TimeType = LOGFILE_TIME_TYPE_COUNTER;
		LogFile_Config(&cfg);
    }
    @endcode

*/
extern ER 		  LogFile_Config(LOGFILE_CFG *pCfg);

/**
     Install LogFile flag and semaphore id.

*/
extern void       LogFile_InstallID(void) _SECTION(".kercfg_text");

/**
    Open LogFile task.

    This is the LogFile task initial function.
    Before calling this API, need to call LogFile_SetConsole firstly.

    @param[in] pOpen:  Init info for LogFile.

    @return
        - @b E_OK: initial success.
        - @b E_PAR: the initial parameter has some error.

    Example:
    @code
    {
        LOGFILE_OPEN    logOpenParm={0};
        UINT32          maxFileNum = 10;
        UINT32          maxFileSize = 0x40000; // 256KB
        CHAR            rootDir[LOGFILE_ROOT_DIR_MAX_LEN+1]="A:\\Novatek\\LOG\\";

        logOpenParm.maxFileNum = maxFileNum;
        logOpenParm.maxFileSize = maxFileSize;
        strncpy(logOpenParm.rootDir,rootDir,LOGFILE_ROOT_DIR_MAX_LEN);
        LogFile_Open(&logOpenParm);
    }
    @endcode

*/
extern ER         LogFile_Open(LOGFILE_OPEN *pOpen);


/**
    Close LogFile task.

    @return
        - @b E_OK: Close success.

*/
extern ER         LogFile_Close(void);

/**
    Re-Open LogFile task.

    Keep the original settings and re-open LogFile task.
    Before calling this API, need to call LogFile_Open firstly.
    When user format the SD card need to call LogFile_ReOpen() to notify logfile re-open.

    @param[in] pOpen:  Init info for LogFile.

    @return
        - @b E_OK: initial success.
        - @b E_PAR: the initial parameter has some error.

*/
extern ER         LogFile_ReOpen(void);
/**
    Suspend LogFile write file operation.

    @return
        - @b E_OK: Suspend success.

*/
extern ER         LogFile_Suspend(void);

/**
    Resume LogFile write file operation.

    @return
        - @b E_OK: Resume success.

*/
extern ER         LogFile_Resume(void);

/**
    Dump some internal variables and status of LogFile module for debug using.

    @return None.
*/
extern void       LogFile_DumpDebugInfo(void);

/**
    Dump all the buffered log to memory.

    Dump all the buffered log to memory.
    The data output header format can reference the structure LOGFILE_DATA_HEAD.

    @param[in] Addr:  The memory address want to dump.
    @param[in] Size:  The memory size.

    @return
        - @b E_OK:  Dump success.
        - @b E_PAR: The input parameter has some error.
        - @b E_OBJ: LogFile function is not opened.
*/
extern ER         LogFile_DumpToMem(UINT32 Addr, UINT32 Size);

/**
    Dump all the buffered log to file.

    @param[in] filepath:  The filepath want to dump.

    @return
        - @b E_OK:  Dump success.
        - @b E_PAR: The input parameter has some error.
        - @b E_OBJ: LogFile function is not opened.
        - @b E_SYS: Write file has some error.
*/
extern ER         LogFile_DumpToFile(CHAR* filepath);

#endif //_LOGFILE_H
