/**
    DbgUt, APIs declare.

    @file       DbgUtApi.h
    @ingroup    mDBGUT

    Copyright   Novatek Microelectronics Corp. 2012.  All rights reserved.
*/

#ifndef _DBGUTAPI_H
#define _DBGUTAPI_H
#include "dma.h"

/**
    @addtogroup mDBGUT
*/
//@{

#define DBGUT_API_VERSION 0x13040911U   ///< API version

/**
     Calculation of engine group and bit. Used in uiExcludeMask.

     Example:
     @code
     {
        //Mask Grph Channel
        Cfg.User.Base.uiExcludeMask[DMA_WPCH_GRA_3/DBGUT_WP_BITS_PER_GROUP]
            = 1 << (DMA_WPCH_GRA_3%DBGUT_WP_BITS_PER_GROUP);
     }
     @endcode
*/
#define DBGUT_WP_BITS_PER_GROUP 32
#define DBGUT_DMA_CH_GROUP_CNT  6 ///< Total DMA group counts

/**
   Error Code.

   Any error code occured will display on uart or return.
*/
typedef enum _DBGUT_ER {
	DBGUT_ER_OK                     =   0, ///< no error
	DBGUT_ER_NOT_INIT               =  -1, ///< DbgUt_Init() is not called or not success
	DBGUT_ER_SYS                    =  -2, ///< unknown system error
	DBGUT_ER_LOCK                   =  -3, ///< failed to lock API
	DBGUT_ER_UNLOCK                 =  -4, ///< failed to unlock API
	DBGUT_ER_PARAM                  =  -5, ///< wrong Parameters
	DBGUT_ER_API_VERSION            =  -6, ///< wrong API version
	DBGUT_ER_STA_TASK               =  -7, ///< failed to call sta_tsk
	DBGUT_ER_WAIT_IDLE              =  -8, ///< failed to call wai_flg for idle
	DBGUT_ER_SET_IDLE               =  -9, ///< failed to call set_flg to idle
	DBGUT_ER_NOT_IN_IDLE            = -10, ///< if Prop.bEnterOnlyIdle is true, but system is not idle
	DBGUT_ER_STATE                  = -11, ///< service state machine is not correctly.
	DBGUT_ER_INVALID_CALL           = -12, ///< invald to call this API
	DBGUT_ER_OUT_DATA_VOLATILE      = -13, ///< the parameter DBGUT_CMD.out maybe volatile. (suggest to bExitCmdFinish=TRUE)
	DBGUT_ER_CMD_MAXDATA            = -14, ///< internal error. all cmd data structure have to declare in DBGUT_CMD_MAXDATA
	DBGUT_ER_INVALID_CMD_IDX        = -15, ///< invalid command index
	DBGUT_ER_CMD_MAP_NULL           = -16, ///< internal error. cmd function map to a null pointer
	DBGUT_ER_CMD_NOT_MATCH          = -17, ///< internal error. g_DbgUtCallTbl is not match to DBGUT_CMD_IDX
	DBGUT_ER_CMD_IN_DATA            = -18, ///< wrong DBGUT_CMD.In Data
	DBGUT_ER_CMD_OUT_DATA           = -19, ///< wrong DBGUT_CMD.Out Data
	DBGUT_ER_TIMER_ALREADY_LOCK     = -20, ///< internal timer already locked
	DBGUT_ER_TIMER_ALREADY_UNLOCK   = -21, ///< internal timer already unlocked
	DBGUT_ER_TIMER_LOCK_FAIL        = -22, ///< internal timer lock failed
	DBGUT_ER_TIMER_LOCK_EXCEPTION   = -23, ///< internal timer lock exception
	DBGUT_ER_TIMER_UNLOCK_EXCEPTION = -24, ///< internal timer unlock exception
	DBGUT_ER_TIMER_CFG_EXCEPTION    = -25, ///< internal timer config exception
	DBGUT_ER_TIMER_CFG_INVALID_TID  = -26, ///< internal timer config invalid timer id
	DBGUT_ER_TIMER_CFG_FAIL         = -27, ///< internal timer config fail
	ENUM_DUMMY4WORD(DBGUT_ER)
} DBGUT_ER;

/**
    Warning Code.

    The warning codes is only displayed via uart.
*/
typedef enum _DBGUT_WR {
	DBGUT_WR_OK                         =    0, ///< no warning
	DBGUT_WR_INIT_TWICE                 =   -1, ///< DbgUt_Init() is called twice
	DBGUT_WR_ALREADY_CLOSED             =   -2, ///< service is already closed.
	DBGUT_WR_SUSPEND_TWICE              =   -3, ///< DbgUt_Suspend maybe is called twice
	DBGUT_WR_NOT_IN_SUSPEND             =   -4, ///< call DbgUt_Resume, but service isn't in suspend state before.
	DBGUT_WR_CMD_SKIP                   =   -5, ///< existing a command is skipped
	DBGUT_WR_DMA_USAGE_START_TWICE      =   -6, ///< DBGUT_CMD_DMA_USAGE_START already start
	DBGUT_WR_DMA_USAGE_ALREADY_STOPED   =   -7, ///< DBGUT_CMD_DMA_USAGE_STOP already stop
	DBGUT_WR_DMA2_USAGE_START_TWICE     =   -6, ///< DBGUT_CMD_DMA_USAGE_START already start
	DBGUT_WR_DMA2_USAGE_ALREADY_STOPED  =   -7, ///< DBGUT_CMD_DMA_USAGE_STOP already stop
	DBGUT_WR_CPU_USAGE_START_TWICE      =   -6, ///< DBGUT_CMD_CPU_USAGE_START already start
	DBGUT_WR_CPU_USAGE_ALREADY_STOPED   =   -7, ///< DBGUT_CMD_CPU_USAGE_STOP already stop
	ENUM_DUMMY4WORD(DBGUT_WR)
} DBGUT_WR;

/**
    Dma Command Index.

    The command of DbgUt service support. Set this index to the DBGUT_CMD_DMA
    and call the DbgUt_CmdDma to do command when service is opened.
*/
typedef enum _DBGUT_CMD_DMA {
	DBGUT_CMD_DMA_UNKNOWN  = 0,     ///< unknown command.
	DBGUT_CMD_DMA_USAGE_CFG,        ///< In:DBGUT_DMA_USAGE_CFG Out:NULL,
	DBGUT_CMD_DMA_USAGE_START,      ///< In:NULL, Out:NULL, start to count usage
	DBGUT_CMD_DMA_USAGE_GET,        ///< In:NULL, Out:UINT32
	DBGUT_CMD_DMA_USAGE_STOP,       ///< In:NULL, Out:NULL, stop to count usage
	DBGUT_CMD_DMA2_USAGE_CFG,       ///< In:DBGUT_DMA_USAGE_CFG Out:NULL,
	DBGUT_CMD_DMA2_USAGE_START,     ///< In:NULL, Out:NULL, start to count usage
	DBGUT_CMD_DMA2_USAGE_GET,       ///< In:NULL, Out:UINT32
	DBGUT_CMD_DMA2_USAGE_STOP,      ///< In:NULL, Out:NULL, stop to count usage
	DBGUT_CMD_DMA_PROTECT_CFG,      ///< In:DBGUT_DMA_PROTECT_CFG, Out:NULL
	DBGUT_CMD_DMA_PROTECT_START,
	DBGUT_CMD_DMA_PROTECT_STOP,
	DBGUT_CMD_DMA_PROTECT_DUMP,     ///< In:NULL, Out:NULL, dump protected address info
	ENUM_DUMMY4WORD(DBGUT_CMD_DMA)
} DBGUT_CMD_DMA;

/**
    CPU Command Index.

    The command of DbgUt service support. Set this index to the DBGUT_CMD_CPU
    and call the DbgUt_CmdCpu to do command when service is opened.
*/
typedef enum _DBGUT_CMD_CPU {
	DBGUT_CMD_CPU_UNKNOWN  = 0,     ///< unknown command.
	DBGUT_CMD_CPU_USAGE_CFG,        ///< In:DBGUT_CPU_USAGE_CFG Out:NULL
	DBGUT_CMD_CPU_USAGE_START,      ///< In:NULL, Out:NULL, start to count usage
	DBGUT_CMD_CPU_USAGE_GET,        ///< In:NULL, Out:UINT32
	DBGUT_CMD_CPU_USAGE_STOP,       ///< In:NULL, Out:NULL, stop to count usage
	ENUM_DUMMY4WORD(DBGUT_CMD_CPU)
} DBGUT_CMD_CPU;

/**
    OS Command Index.

    The command of DbgUt service support. Set this index to the DBGUT_CMD_OS
    and call the DbgUt_CmdOs to do command when service is opened.
*/
typedef enum _DBGUT_CMD_OS {
	DBGUT_CMD_OS_UNKNOWN  = 0,    ///< unknown command.
	DBGUT_CMD_OS_DUMP_KER,        ///< dump ker
	ENUM_DUMMY4WORD(DBGUT_CMD_OS)
} DBGUT_CMD_OS;


/**
    Initial Data.

    For system inital via DbgUt_Init().
*/
typedef struct _DBGUT_INIT {
	UINT32              uiApiVer;   ///< just assign to DBGUT_API_VERSION
	ID                  TaskID;     ///< given a task id form project layer creation (can set 0 to use system default)
	ID                  SemID;      ///< given a semaphore id form project layer creation (can set 0 to use system default)
	ID                  FlagID;     ///< given a flag id form project layer creation (can set 0 to use system default)
} DBGUT_INIT, *PDBGUT_INIT;

/**
    Command Input/Output Data.

    Including data pointer and data size depend on DBGUT_CMD_IDX.
*/
typedef struct _DBGUT_DATA {
	VOID   *pData;      ///< input/output data pointer
	UINT32  uiNumByte;  ///< input/output data size
} DBGUT_DATA, *PDBGUT_DATA;

/**
    Command Description.

    The necessary information for running a command via DbgUt_Cmd().
*/
typedef struct _DBGUT_CMD_DATA {
	DBGUT_DATA     In;     ///< input data description (depend on Idx)
	DBGUT_DATA     Out;    ///< output data description (depend on Idx)
} DBGUT_CMD_DATA, *PDBGUT_CMD_DATA;

typedef struct _DBGUT_USAGE_QUEUE {
	UINT8      *pUsageQueue;    ///< can be NULL for disable. or input an gloal array for saving each usage when time interval occured.
	UINT32      uiByteQueue;    ///< sizeof usage queue
	UINT32     *pCurrIdxQueue;  ///< user provide an UINT32 variable for get index when usage module saving
	UINT32     *pTotalSamples;  ///< user provide an UINT32 variable for get total samples that have been count
} DBGUT_USAGE_QUEUE, *PDBGUT_USAGE_QUEUE;

typedef struct _DBGUT_CPU_USAGE_CFG {
	UINT32      uiItervalMs; ///< time Interval in Ms
	BOOL        bAutoDisp;   ///< auto display usage on uart
	DBGUT_USAGE_QUEUE Queue; ///< (Optional) an gloal structure for saving each usage when time interval occured.
} DBGUT_CPU_USAGE_CFG, *PDBGUT_CPU_USAGE_CFG;

typedef struct _DBGUT_DMA_USAGE_CFG {
	UINT32      uiItervalMs; ///< Time Interval in Ms
	BOOL        bAutoDisp;   ///< auto display usage on uart
	DBGUT_USAGE_QUEUE Queue; ///< (Optional) an gloal structure for saving each usage when time interval occured.
} DBGUT_DMA_USAGE_CFG, *PDBGUT_DMA_USAGE_CFG;

typedef struct _DBGUT_DMA_PROTECT_UNIT {
	BOOL        bEn;            ///< indicate enable
	BOOL        bDetectOnly;    ///< select detect only or including protection.
	UINT32      uiExcludeMask[DBGUT_DMA_CH_GROUP_CNT]; ///< set any bit to '1' to exclude you dont wanna see engine information
} DBGUT_DMA_PROTECT_UNIT, *PDBGUT_DMA_PROTECT_UNIT;

typedef struct _DBGUT_DMA_PROTECT_USER {
	DBGUT_DMA_PROTECT_UNIT  Base;   ///< indicate enable, detect only and exclude mask.
	UINT32                  uiAddr; ///< user defined detection of address
	UINT32                  uiSize; ///< user defined detection of size
	BOOL                    bIncludeCpu; ///< indicate including CPU r/w detection or pretection.
	BOOL                    bIncludeCpu2; ///< indicate including CPU2 r/w detection or pretection.
	BOOL                    bIncludeDsp; ///< indicate including DSP r/w detection or pretection.
	BOOL                    bIncludeDsp2; ///< indicate including DSP2 r/w detection or pretection.
} DBGUT_DMA_PROTECT_USER, *PDBGUT_DMA_PROTECT_USER;

typedef struct _DBGUT_DMA_PROTECT_CFG {
	DBGUT_DMA_PROTECT_UNIT Fw;      ///< firmware area of memory protection
	DBGUT_DMA_PROTECT_UNIT Heap;    ///< firmware area of stack protection
	DBGUT_DMA_PROTECT_USER User;    ///< user defined of memory protection
} DBGUT_DMA_PROTECT_CFG, *PDBGUT_DMA_PROTECT_CFG;

/**
     Install DbgUt task,flag and semaphore id

     This is default install API. User can create own task id, semphore id,
     flag id to adjust own stack size and task priority as following:

     @code
     {
        #define PRI_DBGUT             4
        #define STKSIZE_DBGUT         2048

        UINT32 DBGUT_TSK_ID = 0;
        UINT32 DBGUT_FLG_ID = 0;
        UINT32 DBGUT_SEM_ID = 0;

        void Install_UserDbgUtID(void)
        {
            OS_CONFIG_TASK(DBGUT_TSK_ID,PRI_DBGUT,STKSIZE_DBGUT,DbgUtTsk);
            OS_CONFIG_FLAG(DBGUT_FLG_ID);
            OS_CONFIG_SEMPHORE(DBGUT_SEM_ID,0,1,1);
        }
     }
     @endcode
*/
extern void DbgUt_InstallID(void) _SECTION(".kercfg_text");

/**
    DbgUt main task.

    Task body. Just set it into user_systasktabl in SysCfg.c. The stack size
    suggest to 4096KB. If there are many callbacks have plug-in, the stack
    size maybe need to more.
*/
extern VOID DbgUtTsk(VOID);

/**
    Initialize DbgUt service.

    For using the DbgUt service, the task id, flag id, semphore id have to
    prepare by project layer. Assign these ids to the DBGUT_INIT strcuture
    when system starup (just set once).

    @param[in] pInit necessary inital data.
    @return
        - @b DBGUT_ER_OK: inital success.
        - @b DBGUT_ER_API_VERSION: DbgUtApi.h and AppDbgUt.a not match.
*/
extern DBGUT_ER DbgUt_Init(const DBGUT_INIT *pInit);


/**
    Set a command action to DbgUt service.

    Configure DBGUT_CMD and send it to the service to do some action via
    DbgUt_Cmd.

    @param[in] Cmd command index description.
    @param[in] pData command description.
    @return
        - @b DBGUT_ER_OK: send command success.
        - @b DBGUT_ER_CMD_NOT_MATCH: internal error. Please contact to module
             maintenance staff.
        - @b DBGUT_ER_CMD_IN_DATA: the DBGUT_CMD.In maybe assing wrong object
             / structure
        - @b DBGUT_ER_CMD_IN_DATA: the DBGUT_CMD.Out maybe assing wrong object
             / structure
        - @b DBGUT_ER_STATE: may DbgUt_Open does not be opened yet.
        - @b DBGUT_ER_NOT_IN_IDLE: If DBGUT_CMD.DBGUT_PROP.bEnterOnlyIdle set
             to TRUE and service is not in idle. Notice you this command is
             skipped.
        - @b DBGUT_ER_OUT_DATA_VOLATILE: If the command include the output data
             and set DBGUT_CMD.DBGUT_PROP.bExitCmdFinish to FALSE, the finish
             callback have to plug-in a callback function to DBGUT_CMD
             .DBGUT_PROP.fpFinishCb and DBGUT_CMD.Out have to point to a
             global data structure.
        - @b DBGUT_ER_CMD_MAXDATA: internal error. Please contact to module
             maintenance staff.
*/
//@{
extern DBGUT_ER DbgUt_CmdDma(const DBGUT_CMD_DMA Cmd, const DBGUT_CMD_DATA *pData); ///< Do related DMA command
extern DBGUT_ER DbgUt_CmdDma2(const DBGUT_CMD_DMA Cmd, const DBGUT_CMD_DATA *pData); ///< Do related DMA command
extern DBGUT_ER DbgUt_CmdCpu(const DBGUT_CMD_CPU Cmd, const DBGUT_CMD_DATA *pData); ///< Do related CPU command
//@}

/**
    Install SxCmd

    Install uart command to SxCmd
*/
extern void DbgUt_InstallCmd(void);

extern void cpu_enableWatch(int i, UINT32 addr, UINT32 size);
extern void cpu_disableWatch(int i);


//@}
#endif

