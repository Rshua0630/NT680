/**
    System kernel.

    APIs for system startup and initail

    @file       SysKer.h
    @ingroup    mIOS

    Copyright   Novatek Microelectronics Corp. 2012.  All rights reserved.
*/

#ifndef _SYSKER_H_
#define _SYSKER_H_

#include "Type.h"
#include "kernel.h"
#include "Debug.h"
#include "SysKerStackSize.h"

/**
    @addtogroup mIOS
*/
//@{

/**
     @name OS Config & Startup
     @note the APIs used for implementing OS_Main()
*/
//@{

/**
     @name Memory layout
     @note Memory is divided into 4 regions:                             \n
                                                                         \n
 ------------------------------------ (addr = DRAM address top)          \n
 | CODE - system code and user code | (size = .text + .bss + .common)    \n
 |                                  |                                    \n
 ------------------------------------ (addr = _image_general_zi_zi_limit)\n
 | RESV - system stack and heap     | (size = _SYSTEM_RESERVED_SIZE_)    \n
 |                                  |                                    \n
 ------------------------------------                                    \n
 | STACK - user stack               | (size = sum(task[i].stacksize))    \n
 |                                  |                                    \n
 ------------------------------------                                    \n
 | HEAP - user heap                 | (size = (total-CODE-RESV-STACK))   \n
 |                                  |                                    \n
 ------------------------------------ (addr = DRAM address bottom)       \n
                                                                         \n
  1: CODE region                                                         \n
     * its size is static compiled                                       \n
     * its range is growing from address top to bottom                   \n
     * it is include                                                     \n
     - code section[i] for partial-load                                  \n
     - code section[i] for partial-load                                  \n
         :                                                               \n
     - code section[i] for partial-load                                  \n
     - ZI section                                                        \n
                                                                         \n
  2: RESV region                                                         \n
     * its size is static compiled and fixed                             \n
     * its range is growing from address top to bottom                   \n
     * it is include                                                     \n
     - stack of kernel + heap of kernel                                  \n
                                                                         \n
  3: STACK region                                                        \n
     * its size is static compiled                                       \n
     * its range is growing from address top to bottom                   \n
     * it is include                                                     \n
     - stack of user task[i]                                             \n
     - stack of user task[i]                                             \n
         :                                                               \n
     - stack of user task[i]                                             \n
                                                                         \n
  4: HEAP region                                                         \n
     * its size is static compiled, = (total - CODE - RESV - STACK)      \n
     * its range is all remaining space                                  \n
     * it is include                                                     \n
     - memory of user mempool[i]                                         \n
     - memory of user mempool[i]                                         \n
         :                                                               \n
     - memory of user mempool[i]                                         \n
                                                                         \n
*/
//@{
#define MEM_TOTAL       0x80000000
#define MEM_CODE        0x80000001 ///< CODE region
#define MEM_RESV        0x80000004 ///< RESV region
#define MEM_STACK       0x80000002 ///< STACK region
#define MEM_HEAP        0x80000003 ///< HEAP region
#define MEM_ARB2_HEAP   0x80000005 ///< Arbit2 heap region
#define MEM_STACK_TSK(id)    (0x20000000|(id)) ///< STACK of user task[id]
#define MEM_HEAP_FPL(id)     (0x30000000|(id)) ///< HEAP of user mempool[id]
//@}


/**
   OS init parameter for dram range.

*/
typedef struct {
	UINT32           ddr1_addr;          ///< ddr1 starting address.
	UINT32           ddr1_size;          ///< ddr1 size.
	UINT32           uitron_addr;        ///< uitron starting address.
	UINT32           uitron_size;        ///< uitron size.
	UINT32           ddr2_addr;          ///< ddr2 starting address.
	UINT32           ddr2_size;          ///< ddr2 size.
} OS_INIT_S;




/**
     OS initial

     Calling it at OS_Main() to initialize overall dram range.

     @param[in] init_parm   init parameter for dram range.
     @return
         - @b E_OK:   success
         - @b E_SYS:  addr or size wrong.
*/
extern ER OS_Init(OS_INIT_S *init_parm);
/**
     OS begin code region

     Notify system you will configure the code section and ZI section

     @return
         - @b E_OK:   success
*/
extern ER OS_BeginCode(void);
/**
     OS end code region

     Notify system you finish configuring the code section and ZI section

     @return
         - @b E_OK:   success
*/
extern ER OS_EndCode(void);
/**
     OS begin stack region

     Notify system you will configure the stack

     @return
         - @b E_OK:   success
*/
extern ER OS_BeginStack(void);
/**
     OS begin stack region

     Notify system you finish configuring the stack

     @return
         - @b E_OK:   success
*/
extern ER OS_EndStack(void);
/**
     OS begin heap region

     Notify system you will configure the heap that is memory pool

     @return
         - @b E_OK:   success
*/
extern ER OS_BeginHeap(void);
/**
     OS end heap region

     Notify system you finish configuring the heap that is memory pool

     @return
         - @b E_OK:   success
*/
extern ER OS_EndHeap(void);

/**
     Config kernel (first function before kernel start)

     Step 1 of boot flow: User need to do "Kernel resources configure" and "Basic H/W initial"

*/
extern void OS_Main(void);
/**
     Start kernel

     Step 2 of boot flow: Kernel resources initial and scheduler startup. (internal used only)

*/
extern void _OS_StartUp(void);

/**
     Start the first task

     Step 3 of boot flow: Trigger first task to run NvtMain(). (internal used only)

*/
extern void _OS_StartFirstTask(void);


//@}


/**
     Sytem main function

     System entry (first function after kernel start)

     @param[in] argc       must be zero
     @param[in] argv       must be NULL

*/
extern int NvtMain(int argc, char *argv[]);



/**
     @name CODE layout config
*/
//@{
/**
     Configure code section

     Pointer to Section-Table of Code Region (optional, not include ZI section)

     @param[in] pSection       a memory buffer that must be (MEM_RANGE*)(MEM_DRAM_ADDR+CODE_SECTION_OFFSET);

     @return
         - @b E_OK:   success
         - @b E_SYS:  pSection is NULL
*/
extern ER OS_ConfigCodeSection(MEM_RANGE *pSection);
/**
     Configure code section

     Pointer to Section of ZI

     @param[in] pSection       a memory buffer that must be (MEM_RANGE*)(MEM_DRAM_ADDR+ZI_SECTION_OFFSET);

     @return
         - @b E_OK:   success
         - @b E_SYS:  pSection is NULL
*/
extern ER OS_ConfigZISection(MEM_RANGE *pSection);
#define OS_CONFIG_CODESECTION(addr) OS_ConfigCodeSection(addr); ///< refer to @ref OS_ConfigCodeSection
#define OS_CONFIG_ZISECTION(addr)  OS_ConfigZISection(addr); ///< refer to @ref OS_ConfigZISection
//@}


/**
     @name Memory layout info
*/
//@{
/**
     Get section count

     Input a region by using MEM_??, return the total section counts on this memory region. Usually we use it to
     get total code sections for partial load via OS_GetMemSectionCount(MEM_CODE);

     @param[in] region       memory layout definition MEM_????

     @return section counts
*/
extern UINT32 OS_GetMemSectionCount(int region);
/**
     Get region starting address

     Input a region by using MEM_??, return starting address of this region .

     @param[in] region       memory layout definition MEM_????

     @return starting address
*/
extern UINT32 OS_GetMemAddr(int region);
/**
     Get memory region size

     Input a region by using MEM_??, return size bytes of this region .

     @param[in] region       memory layout definition MEM_????

     @return size bytes
*/
extern UINT32 OS_GetMemSize(int region);
/**
     Get memory region used size

     Input a region by using MEM_??, return used size bytes of this region .

     @param[in] region       memory layout definition MEM_????

     @return size bytes
*/
extern UINT32 OS_GetMemUsedAddr(int region);
/**
     Get memory region remain size

     Input a region by using MEM_??, return remain size bytes of this region .

     @param[in] region       memory layout definition MEM_????

     @return size bytes
*/
extern INT32 OS_GetMemFreeSize(int region);
//@}

/**
     @name Dumping info
*/
//@{
/**
     Dump range

     Dump each region memory range.
*/
extern void OS_DumpMemRange(void);
/**
     Dump status

     Dump each region memory status. This is the same with 'ker dumpmem'
*/
extern void OS_DumpMemStatus(void);
//@}

//------------------------------------------------------------------------------

/**
    Task Configuration Infomation
*/
typedef struct _SYSTEM_TSK_INFO_ {
	ID          TaskID;                         ///< Task ID
	T_CTSK      Task;                           ///< Task Configuration settings.
	CHAR       *TaskName;                       ///< Task name.(for Debuging)

} SYSTEM_TSK_TABLE, *PSYSTEM_TSK_TABLE;

//===========================================================================
// Modified uITRON RTOS data structure
//===========================================================================
/**
    Flag Configuration Infomation
*/
typedef struct _SYSTEM_EVF_INFO_ {
	ID          evfid;                          ///< Event Flag ID
	CHAR       *EventName;                      ///< Event name.(for Debuging)
} SYSTEM_EVF_TABLE, *PSYSTEM_EVF_TABLE;

/**
    Semphore Configuration Infomation
*/
typedef struct _SYSTEM_SEM_INFO_ {
	ID          SemphoreID;                     ///< Semaphore ID
	T_CSEM      Semphore;                       ///< Semphore information.
	CHAR       *SemphoreName;                   ///< Semphore name.(for Debuging)
} SYSTEM_SEM_TABLE, *PSYSTEM_SEM_TABLE;

/**
    Maibox Configuration Infomation
*/
typedef struct _SYSTEM_MBX_INFO_ {
	ID          MailboxID;                      ///< Mail Box ID
	CHAR       *MailboxName;                    ///< Maibox name.(for Debuging)
} SYSTEM_MBX_TABLE, *PSYSTEM_MBX_TABLE;

/**
    Memory-pool Configuration Infomation
*/
typedef struct _SYSTEM_MEMPOOL_INFO_ {
	ID          MemPoolID;                      ///< Fixed memory pool ID
	T_CMPF      MemPool;                        ///< Memory pool information
	CHAR       *MempoolName;                    ///< Memory pool name.(for Debuging)
} SYSTEM_MEMPOOL_TABLE, *PSYSTEM_MEMPOOL_TABLE;


/*
// Task
struct tcb* gtcbadr[MAX_TASK];  // Array of pointers to tcb
struct tcb  gtcbarr[MAX_TASK];  // Array of tcb
const int   gtcbnum = MAX_TASK; // Total tcb number

// Semaphore
struct sem* gsemadr[MAX_SEMAPHORE];  // Array of pointers to sem
struct sem  gsemarr[MAX_SEMAPHORE];  // Array of sem
const int   gsemnum = MAX_SEMAPHORE; // Total sem number

// Flag
struct flg* gflgadr[MAX_FLAG];  // Array of pointers to flg
struct flg  gflgarr[MAX_FLAG];  // Array of flg
const int   gflgnum = MAX_FLAG; // Total flg number

// Free buf queue (for mailbox)
struct buf  gbufarr[MAX_BUF]; // Array of buf
const int   gbufnum = MAX_BUF; // Total buf number

// Mailbox
struct mbx* gmbxadr[MAX_MAILBOX];  // Array of pointers to mbx
struct mbx  gmbxarr[MAX_MAILBOX];  // Array of mbx
const int   gmbxnum = MAX_MAILBOX; // Total mbx number

// Fixed memory pool
struct fpl  gfplarr[MAX_MEMPOOL];  // Array of fpl
struct fpl* gfpladr[MAX_MEMPOOL];  // Array of pointers to fpl
const int   gfplnum = MAX_MEMPOOL; // Total fpl number
VP          gfremem = (VP)0;             // Total fpl memory start
VP          gendmem = (VP)0;             // Total fpl memory end

// Name String
_ALIGNED(4) CHAR* tasknametable[MAX_TASK];
_ALIGNED(4) CHAR* flagnametable[MAX_FLAG];
_ALIGNED(4) CHAR* semaphorenametable[MAX_SEMAPHORE];
_ALIGNED(4) CHAR* mailboxnametable[MAX_MAILBOX];
_ALIGNED(4) CHAR* mempoolnametable[MAX_MEMPOOL];
*/

/**
    Configure Task,Flag,Semaphore & MemPool maximum counts
    @code
    {
        DEFINE_OS_RESOURCE
        (
            USER_MAX_TASK_COUNT+MIN_TASK_COUNT,
            USER_MAX_SEMAPHORE_COUNT,
            USER_MAX_FLAG_COUNT+MIN_FLAG_COUNT,
            USER_BUF_NUM,
            USER_MAX_MAILBOX_COUNT+MIN_MAILBOX_COUNT,
            USER_MAX_MEMPOOL_COUNT+MIN_MEMPOOL_COUNT,
        );
    }
    @endcode
*/
#ifdef __ECOS
#define DEFINE_OS_RESOURCE(max_task, max_sem, max_flg, max_buf, max_mbx, max_fpl, arb2_str_cnt)  \
	UINT32 os_data_begin; \
	VP          gfremem = (VP)0; \
	VP          gendmem = (VP)0; \
	VP          gfremem_arbit2 = (VP)0; \
	VP          gendmem_arbit2 = (VP)0; \
	const int   gtcbnum = max_task; \
	const int   gsemnum = max_sem; \
	const int   gflgnum = max_flg; \
	const int   gbufnum = max_buf; \
	const int   gmbxnum = max_mbx; \
	const int   gfplnum = max_fpl; \
	INT         gSemaMaxCnt[max_sem]; \
	const int   gfpl_arb2_start_num = arb2_str_cnt;\
	_ALIGNED(4) CHAR* tasknametable[max_task]; \
	_ALIGNED(4) CHAR* semaphorenametable[max_sem]; \
	_ALIGNED(4) CHAR* flagnametable[max_flg]; \
	_ALIGNED(4) CHAR* mailboxnametable[max_mbx]; \
	_ALIGNED(4) CHAR* mempoolnametable[max_fpl]; \
	UINT32 os_data_end
#else
#define DEFINE_OS_RESOURCE(max_task, max_sem, max_flg, max_buf, max_mbx, max_fpl, arb2_str_cnt)  \
	UINT32 os_data_begin; \
	VP          gfremem = (VP)0; \
	VP          gendmem = (VP)0; \
	VP          gfremem_arbit2 = (VP)0; \
	VP          gendmem_arbit2 = (VP)0; \
	struct tcb* gtcbadr[max_task]; \
	struct tcb  gtcbarr[max_task]; \
	const int   gtcbnum = max_task; \
	struct sem* gsemadr[max_sem];\
	struct sem  gsemarr[max_sem];\
	const int   gsemnum = max_sem; \
	struct flg* gflgadr[max_flg]; \
	struct flg  gflgarr[max_flg]; \
	const int   gflgnum = max_flg; \
	struct buf  gbufarr[max_buf]; \
	const int   gbufnum = max_buf; \
	struct mbx* gmbxadr[max_mbx]; \
	struct mbx  gmbxarr[max_mbx]; \
	const int   gmbxnum = max_mbx; \
	struct fpl  gfplarr[max_fpl]; \
	struct fpl* gfpladr[max_fpl]; \
	const int   gfplnum = max_fpl; \
	const int   gfpl_arb2_start_num = arb2_str_cnt;\
	_ALIGNED(4) CHAR* tasknametable[max_task]; \
	_ALIGNED(4) CHAR* semaphorenametable[max_sem]; \
	_ALIGNED(4) CHAR* flagnametable[max_flg]; \
	_ALIGNED(4) CHAR* mailboxnametable[max_mbx]; \
	_ALIGNED(4) CHAR* mempoolnametable[max_fpl]; \
	UINT32 os_data_end
#endif

//debug symbol of Kernel (internal data)
#ifdef __ECOS
extern INT   gSemaMaxCnt[];
#endif
extern CHAR *tasknametable[];
extern CHAR *flagnametable[];
extern CHAR *semaphorenametable[];
extern CHAR *mailboxnametable[];
extern CHAR *mempoolnametable[];

//////////////////////////////////////////////////////////////
/**
     @name Resource ID for @ref OS_DumpKernelResID and @ref OS_GetKernelResMaxCnt
*/
//@{
#define KR_RDQ 0x00000000   ///< ready queue
#define KR_TSK 0x00000001   ///< task
#define KR_SEM 0x00000002   ///< semaphore
#define KR_FLG 0x00000003   ///< flag
#define KR_BUF 0x00000004   ///< mail box remain buffer
#define KR_MBX 0x00000005   ///< mail box queued parameters
#define KR_FPL 0x00000006   ///< memory pool
//@}

#define DYNAMIC_ID              0x00    ///< dynamic ID (non-fixed ID)

/**
    For Debuging (Internal using, don't reference it) \n
    Statck checking tag. If you modify this value, please re-build Debug module
*/
#define STACK_CHECK_TAG         0xDEADBEEF


//Install Resource, use macro instead of these function. So no export Doxygen
extern UINT32 OS_AddTask(const PSYSTEM_TSK_TABLE pTaskInfo);
extern UINT32 OS_AddEventflag(const PSYSTEM_EVF_TABLE pEventflagInfo);
extern UINT32 OS_AddSemphore(const PSYSTEM_SEM_TABLE pSemphoreInfo);
extern UINT32 OS_AddMailbox(const PSYSTEM_MBX_TABLE pMailboxInfo);
extern UINT32 OS_AddMempool(const PSYSTEM_MEMPOOL_TABLE pMempoolInfo);

/**
     @name Creating OS resources macro
     @note Use these macro to create your own install function (Task,Flag,Semaphore,MailBox,Memory Pool)
*/
//@{
/**
     Configure a task

     Install a task resource.

     @param[in] id       an UINT32 id without initialized
     @param[in] priority task priority
     @param[in] stksize  task stack size
     @param[in] func     task function point
     @code
     {
        OS_CONFIG_TASK(IPL_PPROC_TaskID, PRI_IPL_PPROC, STKSIZE_IPL_PPROC, IPL_PProc_Tsk);
     }
     @endcode
*/
#define OS_CONFIG_TASK(id, priority, stksize, func)    \
	{\
		_ALIGNED(4) SYSTEM_TSK_TABLE cTaskInfo; \
		cTaskInfo.TaskID = DYNAMIC_ID; \
		cTaskInfo.Task.exinf = NULL; \
		cTaskInfo.Task.tskatr = 0; \
		cTaskInfo.Task.task = func; \
		cTaskInfo.Task.itskpri = priority; \
		cTaskInfo.Task.stksz = stksize; \
		cTaskInfo.Task.istack = 0; \
		cTaskInfo.TaskName = #func; \
		id = OS_AddTask(&cTaskInfo);  \
	}
/**
     Configure a flag

     Install a flag resource.

     @param[in] id       an UINT32 id without initialized
     @code
     {
        OS_CONFIG_FLAG(FLG_ID_RECMEDIA);
     }
     @endcode
*/
#define OS_CONFIG_FLAG(id)    \
	{\
		_ALIGNED(4) SYSTEM_EVF_TABLE cFlagInfo; \
		cFlagInfo.evfid = DYNAMIC_ID; \
		cFlagInfo.EventName = #id; \
		id = OS_AddEventflag(&cFlagInfo);  \
	}
/**
     Configure a semaphore

     Install a semaphore resource.

     @param[in] id          An UINT32 id without initialized
     @param[in] Attribute   It needs to be set to 0.
     @param[in] Counter     Semaphore initial remain counter
     @param[in] MaxCounter  Semaphore maximum counter

     @note Normally, Counter equals MaxCounter

     @code
     {
        OS_CONFIG_SEMPHORE(SEMID_GXTIMER, 0, 1, 1);
     }
     @endcode
*/
//DONT use the way of structure init that will cause .LANCHOR1 section not in part1
#define OS_CONFIG_SEMPHORE(id, Attribute,  Counter,    MaxCounter)    \
	{\
		_ALIGNED(4) SYSTEM_SEM_TABLE cSemInfo; \
		cSemInfo.SemphoreID = DYNAMIC_ID; \
		cSemInfo.Semphore.exinf = NULL; \
		cSemInfo.Semphore.sematr = Attribute; \
		cSemInfo.Semphore.isemcnt = Counter; \
		cSemInfo.Semphore.maxsem = MaxCounter; \
		cSemInfo.SemphoreName = #id; \
		id = OS_AddSemphore(&cSemInfo);  \
	}
/**
     Configure a mail box id

     Install a mail box resource.

     @param[in] id       an UINT32 id without initialized
     @code
     {
        OS_CONFIG_FLAG(FLG_ID_RECMEDIA);
     }
     @endcode
*/
#define OS_CONFIG_MAILBOX(id)    \
	{\
		_ALIGNED(4) SYSTEM_MBX_TABLE cMbxInfo; \
		cMbxInfo.MailboxID = DYNAMIC_ID; \
		cMbxInfo.MailboxName = #id; \
		id = OS_AddMailbox(&cMbxInfo);  \
	}

/**
     Configure a memory pool

     Install a memory pool resource.

     @param[in] id       an UINT32 id without initialized
     @param[in] PoolSize memory pool size
     @param[in] PoolCounter pool counter. Total memory size is PoolSize*PoolCounter
     @code
     {
        OS_CONFIG_MEMPOOL(POOL_ID_GFX_STRING,    POOL_SIZE_GFX_STRING,      POOL_CNT_GFX_STRING);
     }
     @endcode
*/
#define OS_CONFIG_MEMPOOL(id, PoolSize, PoolCounter)    \
	{\
		_ALIGNED(4) SYSTEM_MEMPOOL_TABLE cMempoolInfo; \
		cMempoolInfo.MemPoolID = DYNAMIC_ID; \
		cMempoolInfo.MemPool.exinf = 0; \
		cMempoolInfo.MemPool.mpfatr = ARBIT1_MEM; \
		cMempoolInfo.MemPool.mpfcnt = PoolCounter; \
		cMempoolInfo.MemPool.blfsz = PoolSize; \
		cMempoolInfo.MempoolName = #id; \
		if(OS_IsStartUp())cMempoolInfo.MemPoolID = id; \
		id = OS_AddMempool(&cMempoolInfo);  \
	}
//@}

/**
     Configure a memory pool from arbit2

     Install a memory pool resource from arbit2.

     @param[in] id       an UINT32 id without initialized
     @param[in] PoolSize memory pool size
     @param[in] PoolCounter pool counter. Total memory size is PoolSize*PoolCounter
     @code
     {
        OS_CONFIG_MEMPOOL(POOL_ID_GFX_STRING,    POOL_SIZE_GFX_STRING,      POOL_CNT_GFX_STRING);
     }
     @endcode
*/
#define OS_CONFIG_MEMPOOL_ARB2(id, PoolSize, PoolCounter)    \
	{\
		_ALIGNED(4) SYSTEM_MEMPOOL_TABLE cMempoolInfo; \
		cMempoolInfo.MemPoolID = DYNAMIC_ID; \
		cMempoolInfo.MemPool.exinf = 0; \
		cMempoolInfo.MemPool.mpfatr = ARBIT2_MEM; \
		cMempoolInfo.MemPool.mpfcnt = PoolCounter; \
		cMempoolInfo.MemPool.blfsz = PoolSize; \
		cMempoolInfo.MempoolName = #id; \
		if(OS_IsStartUp())cMempoolInfo.MemPoolID = id; \
		id = OS_AddMempool(&cMempoolInfo);  \
	}
//@}



extern BOOL OS_IsStartUp(void);


/**
     General information for getting kernel resource begin ID

     All kernel resource ID range is limited to [begin_ID..end_ID]. User can use this
     API to get begin ID.

     @param[in] res_type    Resource ID.
     @return count numbers
*/
extern UINT32 OS_GetKernelResBeginID(UINT32 res_type);

/**
     General information for getting kernel resource end ID

     All kernel resource ID range is limited to [begin_ID..end_ID]. User can use this
     API to get end ID.

     @param[in] res_type    Resource ID.
     @return count numbers
*/
extern UINT32 OS_GetKernelResEndID(UINT32 res_type);

/**
     General information for getting kernel resource maximum counter

     All kernel maximun resource are defined DEFINE_OS_RESOURCE. User can use this
     API to get those definitions.

     @param[in] res_type    Resource ID.
     @return count numbers
*/
extern UINT32 OS_GetKernelResMaxCnt(UINT32 res_type);

/**
     General information for getting kernel resource name

     All kernel maximun resource name are assigned by user. User can use this
     API to get name of given ID.

     @param[in] res_type    Resource ID.
     @return count numbers
*/
extern char *OS_GetKernelResName(UINT32 res_type, ID ResID);

/**
     Get task ID

     Use this to get current task id where you call.

     @return task ID
*/
extern ID OS_GetTaskID(void);

/**
     Get task ID

     Use this to get current task name where you call.

     @return task name string
*/
extern char *OS_GetTaskName(void);

/**
     Get pool address

     User uses OS_CONFIG_MEMPOOL to assign a memory pool by a UINT32 variable. After system
     startup, user can use this API to get this starting address by ID.

     @param[in] PoolID    a ID that before using OS_CONFIG_MEMPOOL to assign
     @return starting address

     @code
     OS_GetMempoolAddr(POOL_ID_STORAGE_NAND);
     @endcode
*/
#define OS_GetMempoolAddr(PoolID) OS_GetMemAddr(MEM_HEAP_FPL(PoolID))

/**
     Get pool size

     User uses OS_CONFIG_MEMPOOL to assign a memory pool size. After system
     startup, user can use this API to get this size by ID.

     @param[in] PoolID    a ID that before using OS_CONFIG_MEMPOOL to assign
     @return memory size

     @code
     OS_GetMempoolSize(POOL_ID_STORAGE_NAND);
     @endcode
*/
#define OS_GetMempoolSize(PoolID) OS_GetMemSize(MEM_HEAP_FPL(PoolID))

/**
     Dump all OS resource usage

     Dump information of total counts and used counts of task, flag, semaphore,
     mailbox buffer, mailbox, memory pool. The results show on UART.
*/
extern void OS_DumpKernelResCnt(void);

/**
     Dump a member of resource information

     This dumps a information for a member of one type resource information in detai.

     @return
     - @b TRUE:   success
     - @b FALSE:  wrong res_type or ResID
*/
extern BOOL OS_DumpKernelResID(UINT32 res_type, ID ResID);

/**
     Dump all resource information

     Refer to @ref OS_DumpKernelResID. This dump can dump all existing OS resource
     information in detail.
*/
extern void OS_DumpKernelResStatus(void);

/**
     Dump stack backtrace information

     This function can dump stack backtrace from given registers and stack range information.

     @param[in] info        Register context
                                info[29] = $sp
                                info[30] = $fp
                                info[31] = $ra
                                info[36] = $pc
     @param[in] stack_base  Stack base
     @param[in] stack_size  Stack size
     @param[in] level       detail level,
                                0: show caller address
                                1: also show stack frame range
                                2: also show stack frame content data
*/
extern void OS_DumpStackBacktrace(UINT32 *info, UINT32 stack_base, INT32 stack_size, UINT32 level);

/**
     Dump stack content information

     This function can dump stack content data from given stack range information.

     @param[in] stack_ptr   Stack pointer
     @param[in] stack_base  Stack base
     @param[in] stack_size  Stack size
     @param[in] level       detail level,
                                0: show stack content data: (stack pointer ~ stack bottom)
                                1: show stack content data: (stack top ~ stack bottom)
                            NOTE:
                                stack top = stack base
                                stack bottom = stack base + stack size - sizeof(UINT32)
*/
extern void OS_DumpStackContent(UINT32 stack_ptr, UINT32 stack_base, INT32 stack_size, UINT32 level);


/**
     Dump stack backtrace information of current task

     This function can dump stack backtrace of current task.
*/
extern void OS_DumpCurrentBacktrace(void);

//////////////////////////////////////////////////////////////
//debug MACRO
#define __TASKID__  OS_GetTaskID() ///< refer to @ref OS_GetTaskID
#define __TASK__  OS_GetTaskName() ///< refer to @ref OS_GetTaskName
#define __CALL__  __builtin_return_address(0) ///< get this function return address

/**
    Install SxCmd

    Install uart command to SxCmd
*/
extern void OS_InstallCmd(void);

//@}
#endif //_SYSKER_H_
