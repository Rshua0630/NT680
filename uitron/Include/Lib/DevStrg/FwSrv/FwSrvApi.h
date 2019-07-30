/**
    Firmware Read/Write Service

    @file       FwSrvApi.h
    @ingroup    mFWSRV

    Copyright   Novatek Microelectronics Corp. 2012.  All rights reserved.
*/

#ifndef _FWSRVAPI_H
#define _FWSRVAPI_H

#include "PartLoad.h"

/**
    @addtogroup mFWSRV
*/
//@{

#define FWSRV_API_VERSION 0x20160722   ///< API version
#define FWSRV_PL_BURST_END_TAG  0xFFFFFFFFU ///< indicate end number for FWSRV_PL_LOAD_BURST_IN::puiIdxSequence

/**
   Error Code.

   Any error code occured will display on uart or return.
*/
typedef enum _FWSRV_ER {
	FWSRV_ER_OK                =   0, ///< no error
	FWSRV_ER_NOT_INIT          =  -1, ///< FwSrv_Init() is not called or not success
	FWSRV_ER_SYS               =  -2, ///< unknown system error
	FWSRV_ER_LOCK              =  -3, ///< failed to lock API
	FWSRV_ER_UNLOCK            =  -4, ///< failed to unlock API
	FWSRV_ER_PARAM             =  -5, ///< wrong Parameters
	FWSRV_ER_API_VERSION       =  -6, ///< wrong API version
	FWSRV_ER_STA_TASK          =  -7, ///< failed to call sta_tsk
	FWSRV_ER_WAIT_IDLE         =  -8, ///< failed to call wai_flg for idle
	FWSRV_ER_SET_IDLE          =  -9, ///< failed to call set_flg to idle
	FWSRV_ER_NOT_IN_IDLE       = -10, ///< if Prop.bEnterOnlyIdle is true, but system is not idle
	FWSRV_ER_STATE             = -11, ///< service state machine is not correctly.
	FWSRV_ER_INVALID_CALL      = -12, ///< invald to call this API
	FWSRV_ER_OUT_DATA_VOLATILE = -13, ///< the parameter FWSRV_CMD.out maybe volatile. (suggest to bExitCmdFinish=TRUE)
	FWSRV_ER_CMD_MAXDATA       = -14, ///< internal error. all cmd data structure have to declare in FWSRV_CMD_MAXDATA
	FWSRV_ER_INVALID_CMD_IDX   = -15, ///< invalid command index
	FWSRV_ER_CMD_MAP_NULL      = -16, ///< internal error. cmd function map to a null pointer
	FWSRV_ER_CMD_NOT_MATCH     = -17, ///< internal error. g_FwSrvCallTbl is not match to FWSRV_CMD_IDX
	FWSRV_ER_CMD_IN_DATA       = -18, ///< wrong FWSRV_CMD.In Data
	FWSRV_ER_CMD_OUT_DATA      = -19, ///< wrong FWSRV_CMD.Out Data
	FWSRV_ER_PL_VERSION        = -20, ///< partial load lib invalid
	FWSRV_ER_PL_INIT           = -21, ///< init partial load fail
	FWSRV_ER_PL_LOAD           = -22, ///< partial load fail
	FWSRV_ER_SIZE_NOT_BLK_ALIGN = -23, ///< input size have to block alignment
	FWSRV_ER_SIZE_NOT_ENOUGH   = -24, ///< input buffer size is too small
	FWSRV_ER_SIZE_OUT_OF_RANGE = -25, ///< input data size is out of range
	FWSRV_ER_AREA_OUT_OF_RANGE = -26, ///< input data area is out of range
	FWSRV_ER_ADDR_NOT_WORD_ALIGN = -27, ///< input address have to word alignment
	FWSRV_ER_DEPACK_WRONG_CONTEXT = -28, ///< wrong context of input file at depack command
	FWSRV_ER_DX                = -29, ///< access Dx object is failed.
	FWSRV_ER_ECOS_CONTEXT      = -30, ///< invalid ecos context
	FWSRV_ER_ECOS_BUF_TOO_SMALL = -31, ///< buffer is too small to read eCos firmware
	FWSRV_ER_LINUX             = -32, ///< read linux fail
	FWSRV_ER_DSP               = -33, ///< invalid dsp context
	FWSRV_ER_CONTEXT           = -34, ///< invalid buffer context
	FWSRV_ER_INVALID_UPDATED_DATA = -35, ///< after fw updated, fwsrv reload and check fail
	FWSRV_ER_WRITE_BLOCK = -36,          ///< write block fail during fw updating
	ENUM_DUMMY4WORD(FWSRV_ER)
} FWSRV_ER;

/**
    Warning Code.

    The warning codes is only displayed via uart.
*/
typedef enum _FWSRV_WR {
	FWSRV_WR_OK                =   0, ///< no warning
	FWSRV_WR_INIT_TWICE        =  -1, ///< FwSrv_Init() is called twice
	FWSRV_WR_ALREADY_CLOSED    =  -2, ///< service is already closed.
	FWSRV_WR_SUSPEND_TWICE     =  -3, ///< FwSrv_Suspend maybe is called twice
	FWSRV_WR_NOT_IN_SUSPEND    =  -4, ///< call FwSrv_Resume, but service isn't in suspend state before.
	FWSRV_WR_CMD_SKIP          =  -5, ///< existing a command is skipped
	ENUM_DUMMY4WORD(FWSRV_WR)
} FWSRV_WR;

/**
    State Machine.

    Indicate the task state.
*/
typedef enum _FWSRV_STATE {
	FWSRV_STATE_UNKNOWN    = 0,    ///< unknown. only existing service is not opened.
	FWSRV_STATE_OPEN_BEGIN,        ///< start to open a task
	FWSRV_STATE_OPEN_END,          ///< finished to do open flow
	FWSRV_STATE_CLOSE_BEGIN,       ///< start to close
	FWSRV_STATE_CLOSE_END,         ///< task is terminated.
	FWSRV_STATE_SUSPEND_BEGIN,     ///< start to enter suspend state
	FWSRV_STATE_SUSPEND_END,       ///< task is in suspend state
	FWSRV_STATE_RESUME_BEGIN,      ///< start to resume service
	FWSRV_STATE_RESUME_END,        ///< finished to resume, task start to recevice command
	FWSRV_STATE_CMD_BEGIN,         ///< start to do a command
	FWSRV_STATE_CMD_END,           ///< command finished
	FWSRV_STATE_CMD_CB_BEGIN,      ///< start to call a callback when command finished
	FWSRV_STATE_CMD_CB_END,        ///< exit to callback when command finished
	FWSRV_STATE_STATUS_CB_BEGIN,   ///< start to call a callback when status change
	FWSRV_STATE_STATUS_CB_END,     ///< exit to callback when status change
	FWSRV_STATE_ER_CB_BEGIN,       ///< start to call a callback when error occured
	FWSRV_STATE_ER_CB_END,         ///< exit to callback when error occured
	FWSRV_STATE_MAX_NUM,           ///< total state numbers
	ENUM_DUMMY4WORD(FWSRV_STATE)
} FWSRV_STATE;

/**
    Command Index.

    The command of FwSrv service support. Set this index to the FWSRV_CMD
    and call the FwSrv_Cmd to do command when service is opened.
*/
/**
    Command Index.

    The command of FwSrv service support. Set this index to the FWSRV_CMD
    and call the FwSrv_Cmd to do command when service is opened.
*/
typedef enum _FWSRV_CMD_IDX {
	FWSRV_CMD_IDX_UNKNOWN  = 0,             ///< unknown command. maybe forget to set FWSRV_CMD.Idx1
	FWSRV_CMD_IDX_PL_LOAD_BURST,            ///< Input: FWSRV_PL_LOAD_BURST_IN (NOT SUPPORT VOS)
	FWSRV_CMD_IDX_UITRON_BLK_WRITE,         ///< Input: FWSRV_UITRON_BLK_DESC ,for advance write, write block directly.
	FWSRV_CMD_IDX_UITRON_BLK_READ,          ///< Input: FWSRV_UITRON_BLK_DESC ,for advance read, read block directly.
	FWSRV_CMD_IDX_LOAD_PARTITION,           ///< Input: FWSRV_LOAD_PARTITION
	FWSRV_CMD_IDX_VERIFY_FW,                ///< Input: FWSRV_VERIFY_FW
	FWSRV_CMD_IDX_DUMP_UITRON,              ///< Input: FWSRV_BUF_DESC, Output: FWSRV_BUF_DESC, dump uitron to dram without compression, buffer size required: original uitron size*2
	FWSRV_CMD_IDX_MAX_NUM,                  ///< total command numbers
	ENUM_DUMMY4WORD(FWSRV_CMD_IDX)
} FWSRV_CMD_IDX;

/**
     DX_TYPE_EMBMEM? map to partition name

*/
/**
     DX_TYPE_EMBMEM? map to partition name

*/
typedef struct _FWSRV_DXTYPE_MAP {
	UINT32 Loader;          ///< USER_DX_TYPE_EMBMEM_LOADER
	UINT32 ModelExt;        ///< USER_DX_TYPE_EMBMEM_MODELEXT
	UINT32 uITRON;          ///< USER_DX_TYPE_EMBMEM_UITRON
	UINT32 eCos;            ///< USER_DX_TYPE_EMBMEM_ECOS
	UINT32 uBoot;           ///< USER_DX_TYPE_EMBMEM_UBOOT
	UINT32 Linux;           ///< USER_DX_TYPE_EMBMEM_LINUX
	UINT32 RootFs;          ///< USER_DX_TYPE_EMBMEM_ROOTFS
	UINT32 DSP;             ///< USER_DX_TYPE_EMBMEM_DSP
	UINT32 DSP2;            ///< USER_DX_TYPE_EMBMEM_DSP2
} FWSRV_DXTYPE_MAP, *PFWSRV_DXTYPE_MAP;


/**
    State callback. If there's any state changed, the service will callback out.
*/
typedef void (*FWSRV_FP_STATE_CB)(FWSRV_STATE State);

/**
    Error callback. If there's any error occured, the service will callback out.
*/
typedef void (*FWSRV_FP_ERROR_CB)(FWSRV_ER er);

/**
    Initial Data.

    For system inital via FwSrv_Init().
*/
typedef struct _FWSRV_INIT {
	UINT32              uiApiVer;   ///< just assign to FWSRV_API_VERSION
	ID                  TaskID;     ///< given a task id form project layer creation (can set 0 to use system default)
	ID                  SemID;      ///< given a semaphore id form project layer creation (can set 0 to use system default)
	ID                  FlagID;     ///< given a flag id form project layer creation (can set 0 to use system default)
	FWSRV_DXTYPE_MAP    DxMap;      ///< map of DX_TYPE_EMBMEM? to partition name
	PARTLOAD_INIT       PlInit;     ///< partial load init data. if you don't use partial load, uiWorkingSize can set it to 0
	FWSRV_FP_STATE_CB   fpStateCb;  ///< (Option) system will call the callback when state change
	FWSRV_FP_ERROR_CB   fpErrorCb;  ///< (Option) if there is error occured, callback will be performed for debug
} FWSRV_INIT, *PFWSRV_INIT;

/**
    Finish information.

    When FWSRV_PROP.fpFinishCb assign a callback, the data will send to this
    callback after command finish.
*/
typedef struct _FWSRV_FINISH {
	FWSRV_CMD_IDX   Idx;        ///< Finished command index
	FWSRV_ER        er;         ///< command finish result.
	VOID            *pUserData; ///< If FWSRV_PROP.pUserData is assigned a userdata pointer. service will take it to FWSRV_PROP.fpFinishCb
} FWSRV_FINISH, *PFWSRV_FINISH;

/**
    Command finish callback. When any command is finished, the service will callback out.
*/
typedef void (*FWSRV_CB_FINISH)(const FWSRV_FINISH *pInfo);

/**
    Command Property.

    Indicate how to run the current command.
*/
typedef struct _FWSRV_PROP {
	BOOL                bEnterOnlyIdle; ///< do job only system idle. if system is busy, the FWSRV_ER_NOT_IN_IDLE returned.
	BOOL                bExitCmdFinish; ///< exit API when job finished.
	FWSRV_CB_FINISH     fpFinishCb;     ///< (Option) system will call the callback when job is finished
	VOID                *pUserData;     ///< (Option) when CbFinish is called, assigned userdata can get from CbFinish parameter.
} FWSRV_PROP, *PFWSRV_PROP;

/**
    Command Input/Output Data.

    Including data pointer and data size depend on FWSRV_CMD_IDX.
*/
typedef struct _FWSRV_DATA {
	VOID   *pData;      ///< input/output data pointer
	UINT32  uiNumByte;  ///< input/output data size
} FWSRV_DATA, *PFWSRV_DATA;

/**
    Command Description.

    The necessary information for running a command via FwSrv_Cmd().
*/
typedef struct _FWSRV_CMD {
	FWSRV_CMD_IDX  Idx;    ///< index
	FWSRV_PROP     Prop;   ///< property
	FWSRV_DATA     In;     ///< input data description (depend on Idx)
	FWSRV_DATA     Out;    ///< output data description (depend on Idx)
} FWSRV_CMD, *PFWSRV_CMD;

/**
     Partial load finish callback

     For the command FWSRV_CMD_IDX_PL_LOAD_BURST , the module will callback when
     any part load finish.
*/
typedef void (*FWSRV_CB_PL_LOAD_FINISH)(const UINT32 Idx);

/**
     Burst Description

     For the command FWSRV_CMD_IDX_PL_LOAD_BURST. Partial load will load activity.
*/
typedef struct _FWSRV_PL_LOAD_BURST_IN {
	UINT32 *puiIdxSequence; ///< An UINT32 array to save load sequence (have to be the global array address) have to use FWSRV_PL_BURST_END_TAG at last
	FWSRV_CB_PL_LOAD_FINISH fpLoadedCb; ///< load a part finish callback.
} FWSRV_PL_LOAD_BURST_IN, *PFWSRV_PL_LOAD_BURST_IN;

/**
     Buffer Description

     A buffer with address and size information.
*/
typedef struct _FWSRV_BUF_DESC {
	UINT32 uiAddr;  ///< address
	UINT32 uiSize;  ///< size
} FWSRV_BUF_DESC, *PFWSRV_BUF_DESC;

/**
     Block Description.

     Given a logical block index with buffer description.
*/
typedef struct _FWSRV_UITRON_BLK_DESC {
	UINT32     uiIdx;   ///< logical block index
	FWSRV_BUF_DESC Buf; ///< buffer description
} FWSRV_UITRON_BLK_DESC, *PFWSRV_UITRON_BLK_DESC;

/**
     The item information.

     The item in the all-in-one package shows partition id, file/section name, and
     the address and size of the context.
*/
typedef struct _FWSRV_BIN_ITEM {
	UINT32 uiPartitionID;   ///< partition id
	CHAR  *pName;           ///< section / file name
	UINT32 uiAddr;          ///< address of context
	UINT32 uiSize;          ///< size of context
} FWSRV_BIN_ITEM, *PFWSRV_BIN_ITEM;

typedef struct _FWSRV_LOAD_PARTITION {
	UINT32 EmbType;         ///< support only EMBTYPE_UBOOT
	UINT32 OrderIdx;        ///< 0
	FWSRV_BUF_DESC WorkBuf; ///< if uboot is compressed, FwSrv need temp buffer to decode it.
} FWSRV_LOAD_PARTITION, *PFWSRV_LOAD_PARTITION;

typedef struct _FWSRV_VERIFY_FW {
	FWSRV_BUF_DESC Buf;
} FWSRV_VERIFY_FW, *PFWSRV_VERIFY_FW;

/**
     FWSRV_CMD_IDX_BIN_UPDATE_ALL_IN_ONE Notify Check Sum is OK

     For the command FWSRV_CMD_IDX_BIN_UPDATE_ALL_IN_ONE , if internal previous checksum is vaild, we notify it.
*/
typedef void (*FWSRV_CB_CHECK_SUM_OK)(void);

/**
     Install FwSrv task,flag and semaphore id

     This is default install API. User can create own task id, semphore id,
     flag id to adjust own stack size and task priority as following:

     @code
     {
        #define PRI_FWSRV             10
        #define STKSIZE_FWSRV         2048

        UINT32 FWSRV_TSK_ID = 0;
        UINT32 FWSRV_FLG_ID = 0;
        UINT32 FWSRV_SEM_ID = 0;

        void Install_UserFwSrvID(void)
        {
            OS_CONFIG_TASK(FWSRV_TSK_ID,PRI_FWSRV,STKSIZE_FWSRV,FwSrvTsk);
            OS_CONFIG_FLAG(FWSRV_FLG_ID);
            OS_CONFIG_SEMPHORE(FWSRV_SEM_ID,0,1,1);
        }
     }
     @endcode
*/
extern void FwSrv_InstallID(void) _SECTION(".kercfg_text");

/**
    FwSrv main task.

    Task body. Just set it into user_systasktabl in SysCfg.c. The stack size
    suggest to 4096KB. If there are many callbacks have plug-in, the stack
    size maybe need to more.
*/
extern VOID FwSrvTsk(VOID);

/**
    Initialize FwSrv service.

    For using the FwSrv service, the task id, flag id, semphore id have to
    prepare by project layer. Assign these ids to the FWSRV_INIT strcuture
    when system starup (just set once).

    @param[in] pInit necessary inital data.
    @return
        - @b FWSRV_ER_OK: inital success.
        - @b FWSRV_ER_API_VERSION: FwSrvApi.h and AppFwSrv.a not match.
*/
extern FWSRV_ER FwSrv_Init(const FWSRV_INIT *pInit);

/**
    Start to open the FwSrv service.

    After FwSrv service is initialized, call FwSrv_Open to start the
    service.

    @return
        - @b FWSRV_ER_OK: open success.
        - @b FWSRV_ER_STATE: may FwSrv_Init does not be called yet.
        - @b FWSRV_ER_STA_TASK: failed to start task(sta_tsk failed).
*/
extern FWSRV_ER FwSrv_Open(VOID);

/**
    Stop to open the FwSrv service.

    When FwSrv service is running, call FwSrv_Close to terminate the task
    service.

    @note: Don't call FwSrv_Close in any FwSrv service callback. In
    callback, the FwSrv_AsyncClose is allowed to call to stop recevice receive
    data but task is not terminated. The FwSrv_Close need to be called outside.

    @return
        - @b FWSRV_ER_OK: close success.
        - @b FWSRV_ER_INVALID_CALL: call FwSrv_Close in callback.
        - @b FWSRV_ER_STA_TASK: failed to start task(sta_tsk failed).
*/
extern FWSRV_ER FwSrv_Close(VOID);

/**
    Suspend the service task.

    When FwSrv service is running, call FwSrv_Suspend to stop recevice
    command. Any command will be skiped by service task. The function exit after
    task finish his suspend.

    @return
        - @b FWSRV_ER_OK: suspend success.
        - @b FWSRV_ER_STATE: may FwSrv_Open does not be opened yet.
*/
extern FWSRV_ER FwSrv_Suspend(VOID);

/**
    Resume the service task.

    When FwSrv service is suspended, call FwSrv_Resume to start to
    recevice command. The function exit after task finish his resume.

    @return
        - @b FWSRV_ER_OK: suspend success.
        - @b FWSRV_ER_STATE: may FwSrv_Open does not be opened yet.
*/
extern FWSRV_ER FwSrv_Resume(VOID);

/**
    Set a command action to FwSrv service.

    Configure FWSRV_CMD and send it to the service to do some action via
    FwSrv_Cmd.

    @param[in] pCmd command description.
    @return
        - @b FWSRV_ER_OK: send command success.
        - @b FWSRV_ER_CMD_NOT_MATCH: internal error. Please contact to module
             maintenance staff.
        - @b FWSRV_ER_CMD_IN_DATA: the FWSRV_CMD.In maybe assing wrong object
             / structure
        - @b FWSRV_ER_CMD_IN_DATA: the FWSRV_CMD.Out maybe assing wrong object
             / structure
        - @b FWSRV_ER_STATE: may FwSrv_Open does not be opened yet.
        - @b FWSRV_ER_NOT_IN_IDLE: If FWSRV_CMD.FWSRV_PROP.bEnterOnlyIdle set
             to TRUE and service is not in idle. Notice you this command is
             skipped.
        - @b FWSRV_ER_OUT_DATA_VOLATILE: If the command include the output data
             and set FWSRV_CMD.FWSRV_PROP.bExitCmdFinish to FALSE, the finish
             callback have to plug-in a callback function to FWSRV_CMD
             .FWSRV_PROP.fpFinishCb and FWSRV_CMD.Out have to point to a
             global data structure.
        - @b FWSRV_ER_CMD_MAXDATA: internal error. Please contact to module
             maintenance staff.
*/
extern FWSRV_ER FwSrv_Cmd(const FWSRV_CMD *pCmd);

/**
    Suspend task service, but exit this function without finish suspend.

    This function is for advance. If you don't want to wait FwSrv_Suspend()
    finish too long and you want to pipe some flow, you can do the flow that
    FwSrv_AsyncSuspend() -> Do Something -> FwSrv_Suspend();

    @note: FwSrv_Suspend() is also called necessary to confirm the suspend is
           finished.

    @return
        - @b FWSRV_ER_OK: suspend success.
        - @b FWSRV_ER_STATE: may FwSrv_Open does not be opened yet.
*/
extern FWSRV_ER FwSrv_AsyncSuspend(VOID);

/**
    Stop task service, but exit this function without finish close.

    This function is for advance. If you don't want to wait FwSrv_Close()
    finish too long and you want to pipe some flow, you can do the flow that
    FwSrv_AsyncClose() -> Do Something -> FwSrv_Close();

    @note: FwSrv_AsyncClose() is also called necessary to confirm the suspend is
           finished.

    @return
        - @b FWSRV_ER_OK: suspend success.
        - @b FWSRV_ER_STATE: may FwSrv_Open does not be opened yet.
*/
extern FWSRV_ER FwSrv_AsyncClose(VOID);

/**
    Dump FwSrv application library information.

    The information includes version, build date, last error code, last warning
    code, last command, last state and check internal data.

    @return
        - @b FWSRV_ER_OK: suspend success.
*/
extern FWSRV_ER FwSrv_DumpInfo(VOID);

//@}
#endif

