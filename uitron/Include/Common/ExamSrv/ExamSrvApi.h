/**
    Exam System

    @file       ExamSrvApi.h
    @ingroup    mEXAMSRV

    Copyright   Novatek Microelectronics Corp. 2012.  All rights reserved.
*/

#ifndef _EXAMSRVAPI_H
#define _EXAMSRVAPI_H

/**
    @addtogroup mEXAMSRV
*/
//@{

#define EXAMSRV_API_VERSION 0x11012008U ///< API version

/**
   Declare Module Items Macro
*/
//@{
#define EXAM_ITEM_BEGIN(tbl) static EXAMSRV_ITEM (tbl)[]={
#define EXAM_ITEM(cmd,func,desc)  {(cmd),(func),(desc)},
#define EXAM_ITEM_END() {NULL,NULL,NULL}};
//@}

/**
   Declare Script Macro

   Internal Command \n
   @@module [module acronym name]    :switch module \n
   @@delay [ms]                      :Delay_DelayMs \n
*/
//@{
#define EXAM_SCRIPT_BEGIN(tbl) static EXAMSRV_SCRIPT (tbl)[]={
#define EXAM_SCRIPT(cmd)  {cmd,{0}},
#define EXAM_SCRIPT_END() {"@end",{0}}};
//@}

/**
   Tool Marco used in exam item
*/
//@{
#define EXAM_RESULT(x) ExamSrv_ItemResult(x)
#define EXAM_MSG(...) ExamSrv_ItemMsg(__VA_ARGS__)
#define EXAM_VERIFY(x) {if((x)==TRUE){EXAM_RESULT(EXAMSRV_RESULT_PASS);}else{EXAM_RESULT(EXAMSRV_RESULT_FAIL);}}
//@}

/**
   Error Code.

   Any error code occured will display on uart or return.
*/
typedef enum _EXAMSRV_ER {
	EXAMSRV_ER_OK                =   0, ///< no error
	EXAMSRV_ER_NOT_INIT          =  -1, ///< ExamSrv_Init() is not called or not success
	EXAMSRV_ER_SYS               =  -2, ///< unknown system error
	EXAMSRV_ER_LOCK              =  -3, ///< failed to lock API
	EXAMSRV_ER_UNLOCK            =  -4, ///< failed to unlock API
	EXAMSRV_ER_PARAM             =  -5, ///< wrong Parameters
	EXAMSRV_ER_API_VERSION       =  -6, ///< wrong API version
	EXAMSRV_ER_STA_TASK          =  -7, ///< failed to call sta_tsk
	EXAMSRV_ER_WAIT_IDLE         =  -8, ///< failed to call wai_flg for idle
	EXAMSRV_ER_SET_IDLE          =  -9, ///< failed to call set_flg to idle
	EXAMSRV_ER_NOT_IN_IDLE       = -10, ///< if Prop.bEnterOnlyIdle is true, but system is not idle
	EXAMSRV_ER_STATE             = -11, ///< service state machine is not correctly.
	EXAMSRV_ER_INVALID_CALL      = -12, ///< invald to call this API
	EXAMSRV_ER_OUT_DATA_VOLATILE = -13, ///< the parameter EXAMSRV_CMD.out maybe volatile. (suggest to bExitCmdFinish=TRUE)
	EXAMSRV_ER_CMD_MAXDATA       = -14, ///< internal error. all cmd data structure have to declare in EXAMSRV_CMD_MAXDATA
	EXAMSRV_ER_INVALID_CMD_IDX   = -15, ///< invalid command index
	EXAMSRV_ER_CMD_MAP_NULL      = -16, ///< internal error. cmd function map to a null pointer
	EXAMSRV_ER_CMD_NOT_MATCH     = -17, ///< internal error. g_ExamSrvCallTbl is not match to EXAMSRV_CMD_IDX
	EXAMSRV_ER_CMD_IN_DATA       = -18, ///< wrong EXAMSRV_CMD.In Data
	EXAMSRV_ER_CMD_OUT_DATA      = -19, ///< wrong EXAMSRV_CMD.Out Data
	EXAMSRV_ER_MODULE_REVERSED   = -20, ///< EXAMSRV_MODULE::uiReversed have to be zero (or this module has been registered)
	EXAMSRV_ER_MODULE_CMD_INVALID = -21, ///< EXAMSRV_MODULE::Cmd, the '@' keyword is not accept.
	EXAMSRV_ER_SCRIPT_NOITEM     = -22, ///< EXAMSRV_SCRIPT is without any item
	EXAMSRV_ER_SINGLE_LONGNAME   = -23, ///< EXAMSRV_SINGLE Cmd is too long
	EXAMSRV_ER_SCRIPT_PARSE      = -24, ///< parse script wrong
	ENUM_DUMMY4WORD(EXAMSRV_ER)
} EXAMSRV_ER;

/**
    Warning Code.

    The warning codes is only displayed via uart.
*/
typedef enum _EXAMSRV_WR {
	EXAMSRV_WR_OK                =   0, ///< no warning
	EXAMSRV_WR_INIT_TWICE        =  -1, ///< ExamSrv_Init() is called twice
	EXAMSRV_WR_ALREADY_CLOSED    =  -2, ///< service is already closed.
	EXAMSRV_WR_SUSPEND_TWICE     =  -3, ///< ExamSrv_Suspend maybe is called twice
	EXAMSRV_WR_NOT_IN_SUSPEND    =  -4, ///< call ExamSrv_Resume, but service isn't in suspend state before.
	EXAMSRV_WR_CMD_SKIP          =  -5, ///< existing a command is skipped
	ENUM_DUMMY4WORD(EXAMSRV_WR)
} EXAMSRV_WR;

/**
    State Machine.

    Indicate the task state.
*/
typedef enum _EXAMSRV_STATE {
	EXAMSRV_STATE_UNKNOWN    = 0,    ///< unknown. only existing service is not opened.
	EXAMSRV_STATE_OPEN_BEGIN,        ///< start to open a task
	EXAMSRV_STATE_OPEN_END,          ///< finished to do open flow
	EXAMSRV_STATE_CLOSE_BEGIN,       ///< start to close
	EXAMSRV_STATE_CLOSE_END,         ///< task is terminated.
	EXAMSRV_STATE_SUSPEND_BEGIN,     ///< start to enter suspend state
	EXAMSRV_STATE_SUSPEND_END,       ///< task is in suspend state
	EXAMSRV_STATE_RESUME_BEGIN,      ///< start to resume service
	EXAMSRV_STATE_RESUME_END,        ///< finished to resume, task start to recevice command
	EXAMSRV_STATE_CMD_BEGIN,         ///< start to do a command
	EXAMSRV_STATE_CMD_END,           ///< command finished
	EXAMSRV_STATE_CMD_CB_BEGIN,      ///< start to call a callback when command finished
	EXAMSRV_STATE_CMD_CB_END,        ///< exit to callback when command finished
	EXAMSRV_STATE_STATUS_CB_BEGIN,   ///< start to call a callback when status change
	EXAMSRV_STATE_STATUS_CB_END,     ///< exit to callback when status change
	EXAMSRV_STATE_ER_CB_BEGIN,       ///< start to call a callback when error occured
	EXAMSRV_STATE_ER_CB_END,         ///< exit to callback when error occured
	EXAMSRV_STATE_MAX_NUM,           ///< total state numbers
	ENUM_DUMMY4WORD(EXAMSRV_STATE)
} EXAMSRV_STATE;

/**
    Command Index.

    The command of ExamSrv service support. Set this index to the EXAMSRV_CMD
    and call the ExamSrv_Cmd to do command when service is opened.
*/
typedef enum _EXAMSRV_CMD_IDX {
	EXAMSRV_CMD_IDX_UNKNOWN  = 0,    ///< unknown command. maybe forget to set EXAMSRV_CMD.Idx1
	EXAMSRV_CMD_IDX_REG_MODULE,      ///< add exam moudle (In:EXAMSRV_MODULE*, Out:NULL)
	EXAMSRV_CMD_IDX_RUN_SCRIPT,      ///< add exam script module (In:EXAMSRV_SCRIPT*, Out:NULL)
	EXAMSRV_CMD_IDX_DISP_HELP,       ///< display help (In:char*, Out:NULL), if char* is NULL, display all module name
	EXAMSRV_CMD_IDX_USERTSK_CFG,     ///< config user task (In:EXAMSRV_USERTSK_CFG, Out:NULL)
	EXAMSRV_CMD_IDX_USERTSK_START,   ///< start user task (In:EXAMSRV_USER_ID, Out NULL)
	EXAMSRV_CMD_IDX_USERTSK_TERM,    ///< terminal user task (In:EXAMSRV_USER_ID, Out NULL)
	EXAMSRV_CMD_IDX_GET_USER_FLAG,   ///< get user flag id (In:EXAMSRV_USER_ID, Out ID)
	EXAMSRV_CMD_IDX_GET_USER_SEM,    ///< get user semaphore id (In:EXAMSRV_USER_ID, Out ID)
	EXAMSRV_CMD_IDX_MAX_NUM,         ///< total command numbers
	ENUM_DUMMY4WORD(EXAMSRV_CMD_IDX)
} EXAMSRV_CMD_IDX;

/**
    Result Type

    Test module report the result via ExamSrv_ItemResult
*/
typedef enum _EXAMSRV_RESULT {
	EXAMSRV_RESULT_PASS = 0,    ///< indicate this item is pass
	EXAMSRV_RESULT_FAIL,        ///< indicate this item is fail
	EXAMSRV_RESULT_SKIP,        ///< indicate this item is skip
	EXAMSRV_RESULT_MAX_NUM,
	ENUM_DUMMY4WORD(EXAMSRV_RESULT)
} EXAMSRV_RESULT;

/**
    User ID for Task/Semaphore/Flag

    ExamSrv provide internal id of Task/Semaphore/Flag. Used for EXAMSRV_CMD_IDX_GET_USER_???
*/
typedef enum _EXAMSRV_USER_ID {
	EXAMSRV_USER_ID_0 = 0,      ///< user id 0
	EXAMSRV_USER_ID_1,          ///< user id 1
	EXAMSRV_USER_ID_2,          ///< user id 2
	EXAMSRV_USER_ID_3,          ///< user id 3
	EXAMSRV_USER_ID_4,          ///< user id 4
	EXAMSRV_USER_ID_MAX_NUM,    ///< total user id count
	ENUM_DUMMY4WORD(EXAMSRV_USER_ID)
} EXAMSRV_USER_ID;

typedef void (*EXAMSRV_FP_STATE_CB)(EXAMSRV_STATE State); ///< State callback. If there's any state changed, the service will callback out.
typedef void (*EXAMSRV_FP_ERROR_CB)(EXAMSRV_ER er); ///< Error callback. If there's any error occured, the service will callback out.
/**
    Initial Data.

    For system inital via ExamSrv_Init().
*/
typedef struct _EXAMSRV_INIT {
	UINT32              uiApiVer;   ///< just assign to EXAMSRV_API_VERSION
	ID                  TaskID;     ///< given a task id form project layer creation (can set 0 to use system default)
	ID                  SemID;      ///< given a semaphore id form project layer creation (can set 0 to use system default)
	ID                  FlagID;     ///< given a flag id form project layer creation (can set 0 to use system default)
	ID                  UserTaskID[EXAMSRV_USER_ID_MAX_NUM]; ///< for user plug-in loading task (can set 0 to use system default)
	ID                  UserSemID[EXAMSRV_USER_ID_MAX_NUM]; ///< for user plug-in loading task (can set 0 to use system default)
	ID                  UserFlagID[EXAMSRV_USER_ID_MAX_NUM]; ///< for user plug-in loading task (can set 0 to use system default)
	EXAMSRV_FP_STATE_CB fpStateCb;  ///< (Optional) system will call the callback when state change
	EXAMSRV_FP_ERROR_CB fpErrorCb;  ///< (Optional) if there is error occured, callback will be performed for debug
} EXAMSRV_INIT, *PEXAMSRV_INIT;

/**
    Finish information.

    When EXAMSRV_PROP.fpFinishCb assign a callback, the data will send to this
    callback after command finish.
*/
typedef struct _EXAMSRV_FINISH {
	EXAMSRV_CMD_IDX  Idx;        ///< Finished command index
	EXAMSRV_ER       er;         ///< command finish result.
	VOID           *pUserData;  ///< If EXAMSRV_PROP.pUserData is assigned a userdata pointer. service will take it to EXAMSRV_PROP.fpFinishCb
} EXAMSRV_FINISH, *PEXAMSRV_FINISH;

typedef void (*EXAMSRV_CB_FINISH)(const EXAMSRV_FINISH *pInfo); ///< Command finish callback. When any command is finished, the service will callback out.
/**
    Command Property.

    Indicate how to run the current command.
*/
typedef struct _EXAMSRV_PROP {
	BOOL                bEnterOnlyIdle; ///< do job only system idle. if system is busy, the EXAMSRV_ER_NOT_IN_IDLE returned.
	BOOL                bExitCmdFinish; ///< exit API when job finished.
	EXAMSRV_CB_FINISH    fpFinishCb;    ///< (Optional) system will call the callback when job is finished
	VOID               *pUserData;      ///< (Optional) when CbFinish is called, assigned userdata can get from CbFinish parameter.
} EXAMSRV_PROP, *PEXAMSRV_PROP;

/**
    Command Input/Output Data.

    Including data pointer and data size depend on EXAMSRV_CMD_IDX.
*/
typedef struct _EXAMSRV_DATA {
	VOID   *pData;      ///< input/output data pointer
	UINT32  uiNumByte;  ///< input/output data size
} EXAMSRV_DATA, *PEXAMSRV_DATA;

/**
    Command Description.

    The necessary information for running a command via ExamSrv_Cmd().
*/
typedef struct _EXAMSRV_CMD {
	EXAMSRV_CMD_IDX  Idx;    ///< index
	EXAMSRV_PROP     Prop;   ///< property
	EXAMSRV_DATA     In;     ///< input data description (depend on Idx)
	EXAMSRV_DATA     Out;    ///< output data description (depend on Idx)
} EXAMSRV_CMD, *PEXAMSRV_CMD;


/**
    Item Table

    Register items table.
*/
typedef struct _EXAMSRV_REG_TBL {
	const CHAR *MoudleName;     ///< module name
	const CHAR *Description;    ///< module description
	void       *pReversed;      ///< reversed for linking next module
} EXAMSRV_REG_TBL, *PEXAMSRV_REG_TBL;

/**
    Script Table

    Accroding to registered module, user can make script to do test
*/
typedef struct _EXAMSRV_SCRIPT {
	CHAR *Cmd;                          ///< command name
	UINT32 Cnt[EXAMSRV_RESULT_MAX_NUM]; ///< result count
} EXAMSRV_SCRIPT, *PEXAMSRV_SCRIPT;

/**
    Module Item Function Point

    Each test item is of command-like function point.
*/
typedef BOOL (*EXAMSRV_FP)(CHAR *strCmd);

/**
    Module Item
*/
typedef struct _EXAMSRV_ITEM {
	CHAR *Cmd;          ///< item Name
	EXAMSRV_FP fpCmd;   ///< pointer of command implementation
	CHAR *Desc;         ///< item description
} EXAMSRV_ITEM, *PEXAMSRV_ITEM;

/**
    Module Description
*/
typedef struct _EXAMSRV_MODULE {
	CHAR *Name;             ///< module full name
	CHAR *Acronym;          ///< acronym of module
	EXAMSRV_ITEM *pItems;   ///< item list
	UINT32 uiReversed;      ///< reversed for system using
} EXAMSRV_MODULE, *PEXAMSRV_MODULE;

/**
    User task config
*/
typedef struct _EXAMSRV_USERTSK_CFG {
	EXAMSRV_USER_ID    PlugId;  ///< ExamSrv provided id
	FP                 fpTask;  ///< point of task implementation
} EXAMSRV_USERTSK_CFG, *PEXAMSRV_USERTSK_CFG;

/**
    Module List type
*/
typedef EXAMSRV_MODULE *(*EXAMSRV_ENTRY_FP)(void);

/**
     Install ExamSrv task,flag and semaphore id

     This is default install API. User can create own task id, semphore id,
     flag id to adjust own stack size and task priority as following:

     @code
     {

        #define PRI_EXAMSRV             20
        #define STKSIZE_EXAMSRV         4096

        UINT32 EXAMSRV_TSK_ID = 0;
        UINT32 EXAMSRV_FLG_ID = 0;
        UINT32 EXAMSRV_SEM_ID = 0;
        UINT32 EXAMSRV0_TSK_ID = 0;
        UINT32 EXAMSRV1_TSK_ID = 0;
        UINT32 EXAMSRV2_TSK_ID = 0;
        UINT32 EXAMSRV3_TSK_ID = 0;
        UINT32 EXAMSRV4_TSK_ID = 0;
        UINT32 EXAMSRV0_FLG_ID = 0;
        UINT32 EXAMSRV1_FLG_ID = 0;
        UINT32 EXAMSRV2_FLG_ID = 0;
        UINT32 EXAMSRV3_FLG_ID = 0;
        UINT32 EXAMSRV4_FLG_ID = 0;
        UINT32 EXAMSRV0_SEM_ID = 0;
        UINT32 EXAMSRV1_SEM_ID = 0;
        UINT32 EXAMSRV2_SEM_ID = 0;
        UINT32 EXAMSRV3_SEM_ID = 0;
        UINT32 EXAMSRV4_SEM_ID = 0;

        void ExamSrvID_Install(void)
        {
            OS_CONFIG_TASK(EXAMSRV_TSK_ID,PRI_EXAMSRV,STKSIZE_EXAMSRV,ExamSrvTsk);
            OS_CONFIG_FLAG(EXAMSRV_FLG_ID);
            OS_CONFIG_SEMPHORE(EXAMSRV_SEM_ID,0,1,1);
            //User Plug-in
            OS_CONFIG_TASK(EXAMSRV0_TSK_ID,PRI_EXAMSRV-10,STKSIZE_EXAMSRV,ExamSrvUserTsk);
            OS_CONFIG_TASK(EXAMSRV1_TSK_ID,PRI_EXAMSRV-5,STKSIZE_EXAMSRV,ExamSrvUserTsk);
            OS_CONFIG_TASK(EXAMSRV2_TSK_ID,PRI_EXAMSRV,STKSIZE_EXAMSRV,ExamSrvUserTsk);
            OS_CONFIG_TASK(EXAMSRV3_TSK_ID,PRI_EXAMSRV+5,STKSIZE_EXAMSRV,ExamSrvUserTsk);
            OS_CONFIG_TASK(EXAMSRV4_TSK_ID,PRI_EXAMSRV+10,STKSIZE_EXAMSRV,ExamSrvUserTsk);
            OS_CONFIG_FLAG(EXAMSRV0_FLG_ID);
            OS_CONFIG_FLAG(EXAMSRV1_FLG_ID);
            OS_CONFIG_FLAG(EXAMSRV2_FLG_ID);
            OS_CONFIG_FLAG(EXAMSRV3_FLG_ID);
            OS_CONFIG_FLAG(EXAMSRV4_FLG_ID);
            OS_CONFIG_SEMPHORE(EXAMSRV0_SEM_ID,0,1,1);
            OS_CONFIG_SEMPHORE(EXAMSRV1_SEM_ID,0,1,1);
            OS_CONFIG_SEMPHORE(EXAMSRV2_SEM_ID,0,1,1);
            OS_CONFIG_SEMPHORE(EXAMSRV3_SEM_ID,0,1,1);
            OS_CONFIG_SEMPHORE(EXAMSRV4_SEM_ID,0,1,1);
        }
    }
    @endcode

*/
extern void ExamSrv_InstallID(void) _SECTION(".kercfg_text");

/**
    ExamSrv main task.

    Task body. Just set it into user_systasktabl in SysCfg.c. The stack size
    suggest to 4096KB. If there are many callbacks have plug-in, the stack
    size maybe need to more.
*/
extern VOID ExamSrvTsk(VOID);

/**
    ExamSrv loading task.
*/
extern VOID ExamSrvUserTsk(VOID);

/**
    Initialize ExamSrv service.

    For using the ExamSrv service, the task id, flag id, semphore id have to
    prepare by project layer. Assign these ids to the EXAMSRV_INIT strcuture
    when system starup (just set once).

    @param[in] pInit necessary inital data.
    @return
        - @b EXAMSRV_ER_OK: inital success.
        - @b EXAMSRV_ER_API_VERSION: ExamSrvApi.h and AppExamSrv.a not match.
*/
extern EXAMSRV_ER ExamSrv_Init(const EXAMSRV_INIT *pInit);

/**
    Start to open the ExamSrv service.

    After ExamSrv service is initialized, call ExamSrv_Open to start the
    service.

    @return
        - @b EXAMSRV_ER_OK: open success.
        - @b EXAMSRV_ER_STATE: may ExamSrv_Init does not be called yet.
        - @b EXAMSRV_ER_STA_TASK: failed to start task(sta_tsk failed).
*/
extern EXAMSRV_ER ExamSrv_Open(VOID);

/**
    Stop to open the ExamSrv service.

    When ExamSrv service is running, call ExamSrv_Close to terminate the task
    service.

    @note: Don't call ExamSrv_Close in any ExamSrv service callback. In
    callback, the ExamSrv_AsyncClose is allowed to call to stop recevice receive
    data but task is not terminated. The ExamSrv_Close need to be called outside.

    @return
        - @b EXAMSRV_ER_OK: close success.
        - @b EXAMSRV_ER_INVALID_CALL: call ExamSrv_Close in callback.
        - @b EXAMSRV_ER_STA_TASK: failed to start task(sta_tsk failed).
*/
extern EXAMSRV_ER ExamSrv_Close(VOID);

/**
    Suspend the service task.

    When ExamSrv service is running, call ExamSrv_Suspend to stop recevice
    command. Any command will be skiped by service task. The function exit after
    task finish his suspend.

    @return
        - @b EXAMSRV_ER_OK: suspend success.
        - @b EXAMSRV_ER_STATE: may ExamSrv_Open does not be opened yet.
*/
extern EXAMSRV_ER ExamSrv_Suspend(VOID);

/**
    Resume the service task.

    When ExamSrv service is suspended, call ExamSrv_Resume to start to
    recevice command. The function exit after task finish his resume.

    @return
        - @b EXAMSRV_ER_OK: suspend success.
        - @b EXAMSRV_ER_STATE: may ExamSrv_Open does not be opened yet.
*/
extern EXAMSRV_ER ExamSrv_Resume(VOID);

/**
    Set a command action to ExamSrv service.

    Configure EXAMSRV_CMD and send it to the service to do some action via
    ExamSrv_Cmd.

    @param[in] pCmd command description.
    @return
        - @b EXAMSRV_ER_OK: send command success.
        - @b EXAMSRV_ER_CMD_NOT_MATCH: internal error. Please contact to module
             maintenance staff.
        - @b EXAMSRV_ER_CMD_IN_DATA: the EXAMSRV_CMD.In maybe assing wrong object
             / structure
        - @b EXAMSRV_ER_CMD_IN_DATA: the EXAMSRV_CMD.Out maybe assing wrong object
             / structure
        - @b EXAMSRV_ER_STATE: may ExamSrv_Open does not be opened yet.
        - @b EXAMSRV_ER_NOT_IN_IDLE: If EXAMSRV_CMD.EXAMSRV_PROP.bEnterOnlyIdle set
             to TRUE and service is not in idle. Notice you this command is
             skipped.
        - @b EXAMSRV_ER_OUT_DATA_VOLATILE: If the command include the output data
             and set EXAMSRV_CMD.EXAMSRV_PROP.bExitCmdFinish to FALSE, the finish
             callback have to plug-in a callback function to EXAMSRV_CMD
             .EXAMSRV_PROP.fpFinishCb and EXAMSRV_CMD.Out have to point to a
             global data structure.
        - @b EXAMSRV_ER_CMD_MAXDATA: internal error. Please contact to module
             maintenance staff.
*/
extern EXAMSRV_ER ExamSrv_Cmd(const EXAMSRV_CMD *pCmd);

/**
    Suspend task service, but exit this function without finish suspend.

    This function is for advance. If you don't want to wait ExamSrv_Suspend()
    finish too long and you want to pipe some flow, you can do the flow that
    ExamSrv_AsyncSuspend() -> Do Something -> ExamSrv_Suspend();

    @note: ExamSrv_Suspend() is also called necessary to confirm the suspend is
           finished.

    @return
        - @b EXAMSRV_ER_OK: suspend success.
        - @b EXAMSRV_ER_STATE: may ExamSrv_Open does not be opened yet.
*/
extern EXAMSRV_ER ExamSrv_AsyncSuspend(VOID);

/**
    Stop task service, but exit this function without finish close.

    This function is for advance. If you don't want to wait ExamSrv_Close()
    finish too long and you want to pipe some flow, you can do the flow that
    ExamSrv_AsyncClose() -> Do Something -> ExamSrv_Close();

    @note: ExamSrv_AsyncClose() is also called necessary to confirm the suspend is
           finished.

    @return
        - @b EXAMSRV_ER_OK: suspend success.
        - @b EXAMSRV_ER_STATE: may ExamSrv_Open does not be opened yet.
*/
extern EXAMSRV_ER ExamSrv_AsyncClose(VOID);

/**
    Dump ExamSrv application library information.

    The information includes version, build date, last error code, last warning
    code, last command, last state and check internal data.

    @return
        - @b EXAMSRV_ER_OK: suspend success.
*/
extern EXAMSRV_ER ExamSrv_DumpInfo(VOID);

/**
    Print message

    Only for exam task(test item) used.
*/
extern void ExamSrv_ItemMsg(char *fmtstr, ...);

/**
    Report the result

    Only for exam task(test item) used.
*/
extern void ExamSrv_ItemResult(EXAMSRV_RESULT type);

/**
    Wait key or wait string

    Only for exam task(test item) used.

    @return
        - @b uart input string.
        - @b NULL: the caller is not in ExamSrv task.
*/
extern char *ExamSrv_ItemWaitKey(void);

/**
    Check running item is waiting for key

    @return
        - @b BOOL.
*/
extern BOOL ExamSrvDirect_IsWaitKey(void);

/**
    Send key string to running item.

    The input str must be global data, and have to avoid racing between command
    task and exam task use the same global array. Do not use this api in different
    task.

    @return
        - @b EXAMSRV_ER_OK: suspend success.
        - @b EXAMSRV_ER_STATE: Script item is not in wait state
*/
extern EXAMSRV_ER ExamSrvDirect_SendKey(char *str);

//@}
#endif

