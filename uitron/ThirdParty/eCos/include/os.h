/***************************************************************************
	(C)Copyright 2013, NOVATEK MICROELECTRONICS CORP. STB all rights reserved.
 
	Product	: STB Firmware
	Date	 : 2013/10/22
    Purpose	 :
	Sources	 : os.h
	Customer : Novatek STB
***************************************************************************/
#ifndef _OS_H_
#define _OS_H_

#include <cyg/infra/cyg_type.h>
#include <cyg/hal/drv_api.h>
//#include <stdint.h>

#include <linux/list.h>

#include "def_type.h"
#include "def_err.h"
//#include "drv_config.h"

/**
 * The error code of operation
 */
enum os_error_e
{
	OS_ERR_SYS     = ERROR_BASE_OS+0x00,    /*!< The error code range of system */
		OS_ERR_SYS_EINVAL,                      /*!< Invalid argument */

	OS_ERR_TASK    = ERROR_BASE_OS+0x10,    /*!< The error code range of task */
		OS_ERR_TASK_EPERM,			            /*!< Not permitted */
		OS_ERR_TASK_ENOENT,			            /*!< No such entity */
		OS_ERR_TASK_ESRCH,			            /*!< No such process */
		OS_ERR_TASK_EAGAIN,                     /*!< Try again later */
		OS_ERR_TASK_ENOMEM,                     /*!< Out of memory */
		OS_ERR_TASK_ENOBUFS,                    /*!< No buffer space available */
		OS_ERR_TASK_EBUSY,                      /*!< Resource busy */
		OS_ERR_TASK_EINVAL,                     /*!< Invalid argument */
		OS_ERR_TASK_ERANGE,                     /*!< Math result cannot be represented */
		OS_ERR_TASK_EDEADLOCK,                  /*!< Resource deadlock would occur */
		OS_ERR_TASK_EOPNOTSUPP,                 /*!< Operation not supported */
		OS_ERR_TASK_ETIMEDOUT,                  /*!< Operation timed out */
        OS_ERR_TASK_EINTR,			            /*!< Operation interrupted */

	OS_ERR_ALARM   = ERROR_BASE_OS+0x20,    /*!< The error code range of alarm */
		OS_ERR_ALARM_EINVAL,                    /*!< Invalid argument */
	    OS_ERR_ALARM_ENOENT,                    /*!< No such entity */
	    OS_ERR_ALARM_EOPNOTSUPP,                /*!< Operation not supported */

	OS_ERR_TIME    = ERROR_BASE_OS+0x30,    /*!< The error code range of time */

	OS_ERR_INTR    = ERROR_BASE_OS+0x40,    /*!< The error code range of interrupt */
		OS_ERR_INTR_EINVAL,                     /*!< Invalid argument */
        OS_ERR_INTR_ENOENT,                     /*!< No such entity */
        OS_ERR_INTR_ERANGE,                     /*!< Math result cannot be represented */

	OS_ERR_IPC     = ERROR_BASE_OS+0x50,    /*!< The error code range of inter-process communication operation */
    OS_ERR_SEMA,                            /*!< The error code range of semaphore */
		OS_ERR_SEMA_EINVAL,                     /*!< Invalid argument */
        OS_ERR_SEMA_ENOENT,                     /*!< No such entity */
		OS_ERR_SEMA_ENOBUFS,                    /*!< No buffer space available */
        OS_ERR_SEMA_EBUSY,                      /*!< Resource busy */
		OS_ERR_SEMA_EDEADLOCK,                  /*!< Resource deadlock would occur */
		OS_ERR_SEMA_ETIMEDOUT,                  /*!< Operation timed out */
        OS_ERR_SEMA_EINTR,			            /*!< Operation interrupted */
    OS_ERR_MUTEX,                           /*!< The error code range of mutex */
		OS_ERR_MUTEX_EPERM,			            /*!< Not permitted */
		OS_ERR_MUTEX_EINVAL,                    /*!< Invalid argument */
		OS_ERR_MUTEX_ENOENT,                    /*!< No such entity */
		OS_ERR_MUTEX_ENOBUFS,                   /*!< No buffer space available */
		OS_ERR_MUTEX_EBUSY,                     /*!< Resource busy */
		OS_ERR_MUTEX_EDEADLOCK,                 /*!< Resource deadlock would occur */
		OS_ERR_MUTEX_ETIMEDOUT,                 /*!< Operation timed out */
		OS_ERR_MUTEX_EINTR,			            /*!< Operation interrupted */
    OS_ERR_EVENT,                           /*!< The error code range of event */
		OS_ERR_EVENT_EINVAL,                    /*!< Invalid argument */
		OS_ERR_EVENT_ENOENT,                    /*!< No such entity */
		OS_ERR_EVENT_ENOBUFS,                   /*!< No buffer space available */
		OS_ERR_EVENT_EBUSY,                     /*!< Resource busy */
		OS_ERR_EVENT_EDEADLOCK,                 /*!< Resource deadlock would occur */
	    OS_ERR_EVENT_EOPNOTSUPP,                /*!< Operation not supported */
		OS_ERR_EVENT_ETIMEDOUT,                 /*!< Operation timed out */
		OS_ERR_EVENT_EINTR,			            /*!< Operation interrupted */
    OS_ERR_MBOX,                            /*!< The error code range of mailbox */
		OS_ERR_MBOX_EINVAL,                     /*!< Invalid argument */
		OS_ERR_MBOX_ENOENT,                     /*!< No such entity */
		OS_ERR_MBOX_ENOBUFS,                    /*!< No buffer space available */
		OS_ERR_MBOX_EBUSY,                      /*!< Resource busy */
		OS_ERR_MBOX_EDEADLOCK,                  /*!< Resource deadlock would occur */
		OS_ERR_MBOX_EOPNOTSUPP,                 /*!< Operation not supported */
		OS_ERR_MBOX_ETIMEDOUT,                  /*!< Operation timed out */
		OS_ERR_MBOX_EINTR,			            /*!< Operation interrupted */
    OS_ERR_MQUEUE,                          /*!< The error code range of message queue */
		OS_ERR_MQUEUE_EINVAL,                   /*!< Invalid argument */
		OS_ERR_MQUEUE_ENOENT,                   /*!< No such entity */
		OS_ERR_MQUEUE_ENOBUFS,                  /*!< No buffer space available */
		OS_ERR_MQUEUE_EBUSY,                    /*!< Resource busy */
		OS_ERR_MQUEUE_EDEADLOCK,                /*!< Resource deadlock would occur */
		OS_ERR_MQUEUE_EOPNOTSUPP,               /*!< Operation not supported */
		OS_ERR_MQUEUE_ETIMEDOUT,                /*!< Operation timed out */
		OS_ERR_MQUEUE_EINTR,			        /*!< Operation interrupted */

	OS_ERR_MEMPOOL = ERROR_BASE_OS+0x80,    /*!< The error code range of mempool */
		OS_ERR_MEMPOOL_EINVAL,                  /*!< Invalid argument */
		OS_ERR_MEMPOOL_ENOENT,                  /*!< No such entity */
		OS_ERR_MEMPOOLK_ENOBUFS,                /*!< No buffer space available */
		OS_ERR_MEMPOOL_EDEADLOCK,               /*!< Resource deadlock would occur */
		OS_ERR_MEMPOOL_EOPNOTSUPP,              /*!< Operation not supported */
};

/**
 * \brief
 * Enumerate to indicate the debug message level
 */
typedef enum {
    EN_OS_MEM_BLK_LIST_BASIC = 0,	/* basic memory information */
    EN_OS_MEM_BLK_LIST_DETAIL,		/* detail memory information */
    EN_OS_MEM_BLK_LIST_RESET,       /* reset the memory information */
}EN_OS_MEM_BLK_LIST_MODE;

// ----------------------------------------------------------------------------
// System Management Function

typedef void (*os_exception_handler)(
	u32   data,                   // user supplied data
    u32   exception_number,       // exception being raised
    u32   *exception_info         // any exception specific info
);

struct os_cpuloadinfo_t
{
	u32 average_point1s;
	u32 average_1s;
	u32 average_10s;
};

ErrorCode OS_SysGetVersion(u32 *pu32Version);
/*!\fn ErrorCode OS_SysGetVersion(u32 *pu32Version)
 * \brief
 *
 * \param    pu32Version
 *
 * \return   System Operation Error Code
 *
 * \code
 * \endcode
 */

ErrorCode OS_SysLockCpu(u32 u32CpuId);
/*!\fn ErrorCode OS_SysLockCpu(u32 u32CpuId)
 * \brief
 *
 * \param    u32CpuId
 *
 * \return   System Operation Error Code
 *
 * \code
 * \endcode
 */

ErrorCode OS_SysUnLockCpu(u32 u32CpuId);
/*!\fn ErrorCode OS_SysUnLockCpu(u32 u32CpuId)
 * \brief
 *
 * \param    u32CpuId
 *
 * \return   System Operation Error Code
 *
 * \code
 * \endcode
 */

ErrorCode OS_SysResetCpu(u32 u32CpuId);
/*!\fn ErrorCode OS_SysResetCpu(u32 u32CpuId)
 * \brief
 *
 * \param    u32CpuId
 *
 * \return   System Operation Error Code
 *
 * \code
 * \endcode
 */

u32 OS_SysGetCpuID(void);
/*!\fn u32 OS_SysGetCpuID(void)
 * \brief
 *
 * \return   System Operation Error Code
 *
 * \code
 * \endcode
 */

ErrorCode OS_SysGetCpuLoad(u32 u32CpuId, struct os_cpuloadinfo_t *pstCpuLoadInfo);
/*!\fn ErrorCode OS_SysGetCpuLoad(u32 u32CpuId, struct os_cpuloadinfo_t *pstCpuLoadInfo)
 * \brief
 * Measure CPU load.
 *
 * The function allows the CPU load to be estimated. The load measurements for the last
 * 100ms, 1s and 10s are returned in struct os_cpuloadinfo_t respectively.
 *
 * \param    u32CpuId                  (Input) CPU ID(not used currently)
 * \param    pstCpuLoadInfo            (Output) CPU laod information
 *
 * \return   System Operation Error Code
 *
 * \retval ERROR_NO_ERROR              Operation success
 *
 * \code
 * \endcode
 */

ErrorCode OS_SysGetCpuInfo(void);
/*!\fn ErrorCode OS_SysGetCpuInfo(void)
 * \brief
 *
 * \return   System Operation Error Code
 *
 * \code
 * \endcode
 */

ErrorCode OS_SysSetHeapSize(u32 u32NewHeapSize);
/*!\fn ErrorCode OS_SysSetHeapSize(u32 u32NewHeapSize)
 * \brief
 *
 * \param    u32NewHeapSize
 *
 * \return   System Operation Error Code
 *
 * \code
 * \endcode
 */

s32 OS_SysGetHeapSize(u32 *pu32HeapStart, u32 *pu32HeapEnd);
/*!\fn s32 OS_SysGetHeapSize(u32 *pu32HeapStart, u32 *pu32HeapEnd)
 * \brief
 *
 * \param    pu32HeapStart
 * \param    pu32HeapEnd
 *
 * \return   System Operation Error Code
 *
 * \code
 * \endcode
 */

ErrorCode OS_SysExceptionRegister(u32 os_exception_number, os_exception_handler handler, u32 u32Data);
/*!\fn ErrorCode OS_SysExceptionRegister(u32 os_exception_number, os_exception_handler handler, u32 u32Data)
 * \brief
 *
 * \return   System Operation Error Code
 *
 * \code
 * \endcode
 */

ErrorCode OS_SysExceptionUnRegister(u32 os_exception_number);
/*!\fn ErrorCode OS_SysExceptionUnRegister(u32 os_exception_number)
 * \brief
 *
 * \return   System Operation Error Code
 *
 * \code
 * \endcode
 */

// ----------------------------------------------------------------------------
// Task Management Function

u32 OS_SysGetCacheAddr(void);
/*!\fn u32 OS_SysGetCacheAddr(void)
 * \brief
 *
 * \return   Cache Base address
 *
 * \code
 * \endcode
 */

u32 OS_SysGetNonCacheAddr(void);
/*!\fn u32 OS_SysGetNonCacheAddr(void)
 * \brief
 *
 * \return   Non-Cache Base address
 *
 * \code
 * \endcode
 */

u32 OS_SysGetDRAMAddrMask(void);
/*!\fn u32 OS_SysGetDRAMAddrMask(void)
 * \brief
 *
 * \return   DRAM mask address
 *
 * \code
 * \endcode
 */

#define OS_TASK_PRIORITY_HIGHEST    1  /*!< The highest thread priority */
#define OS_TASK_PRIORITY_LOWEST    31  /*!< The lowest thread priority */

typedef cyg_handle_t os_thread_handle;
typedef struct cyg_thread os_thread_obj;
typedef cyg_thread_info os_thread_info;

typedef void(*os_thread_entry)(u32 u32Data);
typedef void(*os_thread_cleanup)(u32 u32Data);

/**
 * Structure of thread control block
 */
struct os_thread_t
{
	struct list_head list;             /*!< Thread link list */

	os_thread_handle thread_handle;    /*!< Thread handle */
	os_thread_obj thread;              /*!< Thread object */

	u32 stack_size;                    /*!< Size of the stack */
	u8 *stack_ptr;                     /*!< Pointer to the stack */
	bool stack_free;                   /*!< Appoint a free-memory flag using when deleting task */
	u32 priority;                      /*!< Thread priority */
	u32 time_slice;                    /*!< A mount of timer ticks executing this task */
	u32 arg;                           /*!< Argument to thread function */
	u32 reserved;                      /*!< No used */

	s8 *name;                          /*!< Name of thread */

	os_thread_entry entry_fn;          /*!< The entry point for a thread */
	os_thread_cleanup cleanup_fn;      /*!< Thread specific cleanup callback */
};

ErrorCode OS_TaskCreate(struct os_thread_t *pstThread, const s8 *ps8ThreadName);
/*!\fn ErrorCode OS_TaskCreate(struct os_thread_t *pstThread, const s8 *ps8ThreadName)
 * \brief
 * Create a task
 *
 * A newly created thread is always in suspended state and
 * will not start running until it has been resumed via
 * a call to OS_ResumeTask.
 *
 * Also, if threads are created during system initialization
 * then they will not start running until the OS scheduler
 * has been started.
 *
 * \param    pstThread                 (Input/Output) Thread Control Block
 * \param    ps8ThreadName             (Input) Name of thread
 *
 * \return   Thread Operation Error Code
 *
 * \retval ERROR_NO_ERROR              Operation success
 * \retval OS_ERR_TASK_ENOBUFS         No buffer space available
 * \retval OS_ERR_TASK_ERANGE          Math result cannot be represented
 * \retval OS_ERR_TASK_EINVAL          Invalid argument
 *
 * \code
 *	Example Code:
 *
 *	// body of thread
 *	extern void my_thread_entry(u32 data);
 *	// callback function to free some resources when thread will be destroyed
 *	extern void my_thread_cleanup(u32 data);
 *	ErrorCode result;
 *	struct os_thread_t my_thread;
 *	u8 stack[4096];
 *
 *	my_thread.stack_size = 4096;
 *	my_thread.stack_ptr = stack;
 *	my_thread.priority = 10;
 *	my_thread.entry_fn = my_thread_entry;
 *	my_thread.cleanup = my_thread_cleanup;
 *
 *	result = OS_TaskCreate(&my_thread, "MyTask");
 * \endcode
 */

ErrorCode OS_TaskDestroy(os_thread_handle u32ThreadHandle);
/*!\fn ErrorCode OS_TaskDestroy(os_thread_handle u32ThreadHandle)
 * \brief
 * Exit and destroy task
 *
 * Task deletion should be used on a thread after it has exited
 * and is no longer required.
 *
 * \param    u32ThreadHandle           (Input) Handle of thread
 *
 * \return   Thread Operation Error Code
 *
 * \retval ERROR_NO_ERROR              Operation success
 * \retval OS_ERR_TASK_ENOENT          No such entity
 * \retval OS_ERR_TASK_EINVAL          Invalid argument
 * \retval OS_ERR_TASK_EOPNOTSUPP      Operation not supported
 *
 * \code
 *	Example Code:
 *
 *	extern struct os_thread_t my_thread;
 *	ErrorCode result;
 *
 *	result = OS_TaskDestroy(my_thread.thread_handle);
 * \endcode
 */

ErrorCode OS_TaskTerminate(os_thread_handle u32ThreadHandle);
/*!\fn ErrorCode OS_TaskTerminate(os_thread_handle u32ThreadHandle)
 * \brief
 * Terminate task(exit for itself, or kill other thread)
 *
 * The exit case allows a thread to terminate itself, thus ensuring that
 * it will not be run again by the scheduler.
 *
 * The kill case cannot be used by a thread to kill itself.
 * One thread may use OS_TerminateTask on another.
 *
 * \param    u32ThreadHandle           (Input) Handle of thread
 *
 * \return   Thread Operation Error Code
 *
 * \retval ERROR_NO_ERROR              Operation success
 * \retval OS_ERR_TASK_ENOENT          No such entity
 * \retval OS_ERR_TASK_EINVAL          Invalid argument
 *
 * \code
 *	Example Code:
 *
 *	extern struct os_thread_t my_thread;
 *	ErrorCode result;
 *
 *	result = OS_TaskTerminate(my_thread.thread_handle);
 * \endcode
 */

ErrorCode OS_TaskReset(os_thread_handle u32ThreadHandle);
/*!\fn ErrorCode OS_TaskReset(os_thread_handle u32ThreadHandle)
 * \brief
 * Restart the terminated thread
 *
 * The thread TCB data structure can passed to OS_ResetTask
 * remains in use, and the handle returned by OS_ResetTask
 * remains valid.
 *
 * \param    u32ThreadHandle           (Input) Handle of thread
 *
 * \return   Thread Operation Error Code
 *
 * \retval ERROR_NO_ERROR              Operation success
 * \retval OS_ERR_TASK_ENOENT          No such entity
 * \retval OS_ERR_TASK_EINVAL          Invalid argument
 *
 * \code
 *	Example Code:
 *
 *	extern struct os_thread_t my_thread;
 *	ErrorCode result;
 *
 *	// a terminated thread will be not putting into scheduler's ready queue
 *	result = OS_TaskTerminate(my_thread.thread_handle);
 *
 *	// restart the terminated thread and reschedule it
 *	// using OS_TaskReset(...) when it is called by other thread
 *	result = OS_TaskReset(my_thread.thread_handle);
 * \endcode
 */

ErrorCode OS_TaskRelease(os_thread_handle u32ThreadHandle);
/*!\fn ErrorCode OS_TaskRelease(os_thread_handle u32ThreadHandle)
 * \brief
 * Release waiting condition of Task
 *
 * When a thread is blocked on a synchronization primitive such as a
 * semaphore or a mutex, or when it is waiting for an alarm to trigger,
 * it can be forcibly woken up using OS_ReleaseTask.
 *
 * \param    u32ThreadHandle           (Input) Handle of thread
 *
 * \return   Thread Operation Error Code
 *
 * \retval ERROR_NO_ERROR              Operation success
 * \retval OS_ERR_TASK_EPERM           Not permitted
 * \retval OS_ERR_TASK_ENOENT          No such entity
 * \retval OS_ERR_TASK_EINVAL          Invalid argument
 *
 * \code
 *	Example Code:
 *
 *	<A thread>
 *	extern struct os_semaphore_t my_sem;
 *	ErrorCode result;
 *
 *	// waiting some resource, i.e. semaphore
 *	result = OS_WaitSemaphore(my_sem.sem_handle, OS_IPC_ALWAYS_WAIT);
 *
 *	...
 *
 *	<B thread>
 *	extern struct os_thread_t A_thread;
 *	ErrorCode result;
 *
 *	// A thread can be forcibly woken up using OS_ReleaseTask(...)
 *	result = OS_TaskRelease(A_thread.thread_handle);
 * \endcode
 */

ErrorCode OS_TaskSuspend(os_thread_handle u32ThreadHandle);
/*!\fn ErrorCode OS_TaskSuspend(os_thread_handle u32ThreadHandle)
 * \brief
 * Suspend task
 *
 * suspends the execution of the task specified by task handle by putting it
 * into SUSPEND state.
 *
 * Associated with each thread is a suspend counter. When a thread is
 * first created this counter is initialized to 1. OS_SuspendTask
 * can be used to increment the suspend counter, and OS_ResumeTask
 * decrements it. The scheduler will never run a thread with a non-zero
 * suspend counter. Therefore a newly created thread will not run until
 * it has been resumed.
 *
 * \param    u32ThreadHandle           (Input) Handle of thread
 *
 * \return   Thread Operation Error Code
 *
 * \retval ERROR_NO_ERROR              Operation success
 * \retval OS_ERR_TASK_ENOENT          No such entity
 * \retval OS_ERR_TASK_EINVAL          Invalid argument
 * \retval OS_ERR_TASK_EDEADLOCK       Resource deadlock would occur
 *
 * \code
 *	Example Code:
 *
 *	extern struct os_thread_t my_thread;
 *	ErrorCode result;
 *
 *	result = OS_TaskSuspend(my_thread.thread_handle);
 * \endcode
 */

ErrorCode OS_TaskResume(os_thread_handle u32ThreadHandle);
/*!\fn ErrorCode OS_TaskResume(os_thread_handle u32ThreadHandle)
 * \brief
 * Resume suspended task
 *
 * Thread resumed and hence becomes runnable again.
 *
 * \param    u32ThreadHandle           (Input) Handle of thread
 *
 * \return   Thread Operation Error Code
 *
 * \retval ERROR_NO_ERROR              Operation success
 * \retval OS_ERR_TASK_ENOENT          No such entity
 * \retval OS_ERR_TASK_EINVAL          Invalid argument
 *
 * \code
 *	Example Code:
 *
 *	<A thread>
 *	extern struct os_thread_t A_thread;
 *	ErrorCode result;
 *
 *	result = OS_TaskSuspend(A_thread.thread_handle);
 *
 *	...
 *
 *	<B thread>
 *	extern struct os_thread_t A_thread;
 *	ErrorCode result;
 *
 *	// a suspended A_thread can be resumed using OS_TaskResume(...)
 *	result = OS_TaskResume(A_thread.thread_handle);
 * \endcode
 */

ErrorCode OS_TaskYield(void);
/*!\fn ErrorCode OS_TaskYield(void)
 * \brief
 * Sleep task with a slice of time
 *
 * OS_YieldTask allows a thread to relinquish control of the processor
 * to some other runnable thread which has the same priority or higher.
 *
 * \return   Thread Operation Error Code
 *
 * \retval ERROR_NO_ERROR              Operation success
 * \retval OS_ERR_TASK_ENOENT          No such entity
 * \retval OS_ERR_TASK_EDEADLOCK       Resource deadlock would occur
 *
 * \code
 *	Example Code:
 *
 *	ErrorCode result;
 *
 *	// delay a time-slice
 *	result = OS_TaskYield();
 * \endcode
 */

ErrorCode OS_TaskSleep(u32 u32TickTime);
/*!\fn ErrorCode OS_TaskSleep(u32 u32TickTime)
 * \brief
 * Sleep task with timeout
 *
 * OS_SleepTask allows a thread to suspend until the specified number
 * of clock ticks have occurred.
 *
 * It can be forcibly woken up using OS_WakeupTask.
 *
 * \param    u32TickTime               (Input) Specified a piece of sleep time
 *
 * \return   Thread Operation Error Code
 *
 * \retval ERROR_NO_ERROR              Operation success
 * \retval OS_ERR_TASK_ENOENT          No such entity
 * \retval OS_ERR_TASK_EINVAL          Invalid argument
 * \retval OS_ERR_TASK_EDEADLOCK       Resource deadlock would occur
 *
 * \code
 *	Example Code:
 *
 *	#define DELAY_TIME_1000_MS		1000
 *
 *	ErrorCode result;
 *
 *	result = OS_TaskSleep(DELAY_TIME_1000_MS);
 * \endcode
 */

ErrorCode OS_TaskWakeup(os_thread_handle u32ThreadHandle);
/*!\fn ErrorCode OS_TaskWakeup(os_thread_handle u32ThreadHandle)
 * \brief
 * Wakeup task
 *
 * \param    u32ThreadHandle           (Input) Handle of thread
 *
 * \return   Thread Operation Error Code
 *
 * \retval ERROR_NO_ERROR              Operation success
 * \retval OS_ERR_TASK_EPERM           Not permitted
 * \retval OS_ERR_TASK_ENOENT          No such entity
 * \retval OS_ERR_TASK_EINVAL          Invalid argument
 *
 * \code
 *	Example Code:
 *
 *	extern struct os_thread_t my_thread;
 *	ErrorCode result;
 *
 *	result = OS_TaskWakeup(my_thread.thread_handle);
 * \endcode
 */

ErrorCode OS_TaskSetPriority(os_thread_handle u32ThreadHandle, s8 s8Priority);
/*!\fn ErrorCode OS_TaskSetPriority(os_thread_handle u32ThreadHandle, s8 s8Priority)
 * \brief
 * Change task priority
 *
 * OS_SetTaskPriority should also only be called from thread context.
 *
 * \param    u32ThreadHandle           (Input) Handle of thread
 * \param    s8Priority                (Input) Priority of thread
 *
 * \return   Thread Operation Error Code
 *
 * \retval ERROR_NO_ERROR              Operation success
 * \retval OS_ERR_TASK_ENOENT          No such entity
 * \retval OS_ERR_TASK_ERANGE          Math result cannot be represented
 * \retval OS_ERR_TASK_EINVAL          Invalid argument
 *
 * \code
 *	Example Code:
 *
 *	extern struct os_thread_t my_thread;
 *	ErrorCode result;
 *	u8 priority;
 *
 *	priority = 10;
 *	result = OS_TaskSetPriority(my_thread.thread_handle, priority);
 * \endcode
 */

s8 OS_TaskGetPriority(os_thread_handle u32ThreadHandle);
/*!\fn s8 OS_TaskGetPriority(os_thread_handle u32ThreadHandle)
 * \brief
 * Get task priority
 *
 * OS_GetTaskPriority returns the real current priority.
 *
 * \param    u32ThreadHandle           (Input) Handle of thread
 *
 * \return   Current thread priority or invalid value(-1)
 *
 * \code
 *	Example Code:
 *
 *	extern struct os_thread_t my_thread;
 *	u8 priority;
 *
 *	priority = OS_TaskGetPriority(my_thread.thread_handle);
 * \endcode
 */

os_thread_handle OS_TaskGetCurrentHandle(void);
/*!\fn os_thread_handle OS_TaskGetCurrentHandle(void)
 * \brief
 * Get task handle itself
 *
 * OS_GetCurrentTaskHandle returns a handle corresponding to the current thread.
 *
 * \return   Current thread handle
 *
 * \code
 *	Example Code:
 *
 *	os_thread_handle my_thread_h;
 *
 *	my_thread_h = OS_TaskGetCurrentHandle();
 *  \endcode
 */

s32 OS_TaskGetID(os_thread_handle u32ThreadHandle);
/*!\fn s32 OS_TaskGetID(os_thread_handle u32ThreadHandle)
 * \brief
 * Get task identifier
 *
 * OS_GetTaskID returns the unique thread ID for the thread
 * identified by thread.
 *
 * \param    u32ThreadHandle           (Input) Handle of thread
 *
 * \return   Thread id number or invalid value(-1)
 *
 * \code
 *	Example Code:
 *
 *	extern struct os_thread_t my_thread;
 *	s32 thread_id;
 *
 *	thread_id = OS_TaskGetID(my_thread.thread_handle);
 * \endcode
 */

ErrorCode OS_TaskGetInfo(os_thread_handle u32ThreadHandle, os_thread_info *pstThreadInfo);
/*!\fn ErrorCode OS_TaskGetInfo(os_thread_handle u32ThreadHandle, os_thread_info *pstThreadInfo)
 * \brief
 * Reference task status
 *
 * OS_GetTaskInfo fills in the os_thread_info structure with
 * information about the thread described by the thread and id arguments.
 * The information returned includes the thread's handle and id, its state
 * and name, priorities and stack parameters.
 *
 * \param    u32ThreadHandle           (Input) Handle of thread
 * \param    pstThreadInfo             (Output) Information of thread
 *
 * \return   Thread Operation Error Code
 *
 * \retval ERROR_NO_ERROR              Operation success
 * \retval OS_ERR_TASK_ENOENT          No such entity
 * \retval OS_ERR_TASK_EINVAL          Invalid argument
 *
 * \code
 *	Example Code:
 *
 *	extern struct os_thread_t my_thread;
 *	ErrorCode result;
 *	os_thread_info thread_info;
 *
 *	// struct os_thread_info
 *	// {
 *	//	os_thread_handle      handle;
 *	//	u32      id;
 *	//
 *	//	#define RUNNING    0          // Thread is runnable or running
 *	//	#define SLEEPING   1          // Thread is waiting for something to happen
 *	//	#define	COUNTSLEEP 2          // Sleep in counted manner
 *	//	#define	SUSPENDED  4          // Suspend count is non-zero
 *	//	#define	CREATING   8          // Thread is being created
 *	//	#define	EXITED     16         // Thread has exited
 *  //
 *	// 	u32      state;
 *  //  u32      sleep;
 *  //  u32      wake;
 *  //
 *	// 	char     *name;
 *	// 	u32      set_pri;
 *	// 	u32      cur_pri;
 *	// 	u32      stack_base;
 *	// 	u32      stack_size;
 *	//	u32      stack_used;
 *	// };
 *
 *	result = OS_TaskGetInfo(my_thread.thread_handle, &thread_info);
 * \endcode
 */

ErrorCode OS_TaskWaitForExit(os_thread_handle u32ThreadHandle);
/*!\fn ErrorCode OS_TaskWaitForExit(os_thread_handle u32ThreadHandle)
 * \brief
 * Wait for a task to finish and release all resources associated with that thread.
 *
 * OS_TaskWaitForExit can legally be called many times for a particular thread handle.
 * Waiting for exit from multiple parallel threads blocks as expected.
 *
 * Calling this function after the thread has exited will return immediately. Eclipse
 * will not call this function after the call to OS_TaskDestroy() for OS_TaskWaitForExit.
 */

void OS_SchedulerLock(void);
/*!\fn ErrorCode OS_SchedulerLock(void)
 * \brief
 * Disable task dispatch
 *
 * OS_LockScheduler can be called to claim the lock. On return
 * it is guaranteed that the current thread will not be preempted,
 * and that no other code is manipulating any kernel data structures.
 *
 * While the scheduler is locked the current thread cannot be preempted,
 * so any higher priority threads will not be able to run. Also no DSRs
 * can run, so device drivers may not be able to service I/O requests.
 */

void OS_SchedulerUnLock(void);
/*!\fn ErrorCode OS_SchedulerUnLock(void)
 * \brief
 * Enable task dispatch
 *
 * OS_UnLockScheduler can be used to release the lock, which may cause
 * the current thread to be preempted.
 */

// ----------------------------------------------------------------------------
// Track Information Management Function

void OS_TrackTaskStatus(void);
/*!\fn void OS_TrackTaskStatus(void)
 * \brief
 * Dump task status
 *
 */

void OS_TrackCoreDump(void);
/*!\fn void OS_TrackCoreDump(void)
 * \brief
 * Dump MIPS32 Coprocessor(CP0) register
 *
 */

// ----------------------------------------------------------------------------
// Alarm/Timer Management Function
/*!\file
 * Alarm
 *
 * Kernel alarms are used together with counters and allow for
 * action to be taken when a certain number of events have occurred.
 * If the counter is associated with a clock then the alarm action
 * happens when the appropriate number of clock ticks have occurred,
 * in other words after a certain period of time.
 *
 */

typedef cyg_handle_t os_alarm_handle;
typedef struct cyg_alarm os_alarm_obj;

typedef void (*alarm_entry)(os_alarm_handle u32AlarmHandle, u32 u32Data);

typedef enum os_alarm_type_e
{
	EN_OS_ALARM_ONESHOT,               /*!< Only trigger once */
	EN_OS_ALARM_REPEAT                 /*!< Repeated */
} EN_OS_ALARM_TYPE;

/**
 * Structure of alarm control block
 */
struct os_alarm_t
{
	struct list_head list;             /*!< Alarm link list */

	os_alarm_handle alarm_handle;      /*!< Alarm handle */
	os_alarm_obj alarm;                /*!< Alarm object */

	EN_OS_ALARM_TYPE alarm_type;       /*!< The type of alarm trigger */

	u32 trigger;                       /*!< Next trigger time */
	u32 interval;                      /*!< Trigger period of the specified events */
	u32 arg;                           /*!< Argument to alarm handle function */

	s8 *name;                          /*!< Name of alarm */

	alarm_entry entry_fn;              /*!< The entry point for a alarm */
};

ErrorCode OS_AlarmCreate(struct os_alarm_t *pstAlarm, const s8 *ps8AlarmName);
/*!\fn ErrorCode OS_AlarmCreate(struct os_alarm_t *pstAlarm, const s8 *ps8AlarmName)
 * \brief
 * Create an alarm
 *
 * This creates a new alarm. Alarms are periodic events, generally
 * tied to the system counter.
 *
 * The period is defined when OS_AlarmActive is called.
 *
 * \param    pstAlarm                  (Input/Output) Pointer to an alarm control block
 * \param    ps8AlarmName              (Input) Name of alarm
 *
 * \return   Alarm Operation Error Code
 *
 * \retval ERROR_NO_ERROR              Operation success
 * \retval OS_ERR_ALARM_EINVAL         Invalid argument
 * \retval OS_ERR_ALARM_EOPNOTSUPP     Operation not supported
 *
 * \code
 *	Example Code:
 *
 *	<ONESHOT>:
 *
 *	#define DELAY_TIME_1000_MS		1000
 *
 *	extern void my_alarm_entry(os_alarm_handle alarm_h, u32 data);
 *	ErrorCode result;
 *	struct os_alarm_t my_alarm;
 *
 *	my_alarm.alarm_type = EN_OS_ALARM_ONESHOT;
 *	my_alarm.trigger = DELAY_TIME_1000_MS; // trigger after 1000 timer tick.
 *	my_alarm.interval = 0;                 // only trigger once
 *	v.entry_fn = my_alarm_entry;
 *
 *	result = OS_AlarmCreate(&my_alarm, "MyAlarm");
 *
 *	// active alarm now or later
 *	// OS_AlarmActive(my_alarm.alarm_handle);
 *
 *	<REPEAT>:
 *
 *	#define DELAY_TIME_1000_MS		1000
 *	#define DELAY_TIME_500_MS		500
 *
 *	extern void my_alarm_entry(os_alarm_handle alarm_h, u32 data);
 *	ErrorCode result;
 *	struct os_alarm_t my_alarm;
 *	u32 init_data = 0xdeadbeef;
 *
 *	my_alarm.alarm_type = EN_OS_ALARM_REPEAT;
 *	my_alarm.trigger = DELAY_TIME_1000_MS; // trigger after 1000 timer tick.
 *	my_alarm.interval = DELAY_TIME_500_MS; // repeat event every 500 timer tick.
 *	my_alarm.entry_fn = my_alarm_entry;
 *	my_alarm.arg = init_data;
 *
 *	result = OS_AlarmCreate(&my_alarm, "MyAlarm");
 *
 *	// active alarm now or later
 *	// OS_AlarmActive(my_alarm.alarm_handle);
 * \endcode
 */

ErrorCode OS_AlarmDestroy(os_alarm_handle u32AlarmHandle);
/*!\fn ErrorCode OS_AlarmDestroy(os_alarm_handle u32AlarmHandle)
 * \brief
 * Delete alarm
 *
 * If an alarm is no longer required then this function deletes an alarm
 * from the system and invalidates the handle to the alarm.
 *
 * The alarm cannot be used once it is deleted.
 *
 * \param    u32AlarmHandle            (Input) Handle of alarm
 *
 * \return   Alarm Operation Error Code
 *
 * \retval ERROR_NO_ERROR              Operation success
 * \retval OS_ERR_ALARM_EINVAL         Invalid argument
 * \retval OS_ERR_ALARM_ENOENT         No such entity
 *
 * \code
 *	Example Code:
 *
 *	extern struct os_alarm_t my_alarm;
 *	ErrorCode result;
 *
 *	result = OS_AlarmDestroy(my_alarm.alarm_handle);
 * \endcode
 */

ErrorCode OS_AlarmActive(os_alarm_handle u32AlarmHandle);
/*!\fn ErrorCode OS_AlarmActive(os_alarm_handle u32AlarmHandle)
 * \brief
 * Active alarm
 *
 * Once an alarm has been created a further call to OS_AlarmActive
 * is needed to activate it.
 *
 * \param    u32AlarmHandle            (Input) Handle of alarm
 *
 * \return   Alarm Operation Error Code
 *
 * \retval ERROR_NO_ERROR              Operation success
 * \retval OS_ERR_ALARM_EINVAL         Invalid argument
 * \retval OS_ERR_ALARM_ENOENT         No such entity
 *
 * \code
 *	Example Code:
 *
 *	#define DELAY_TIME_1000_MS		1000
 *	#define DELAY_TIME_500_MS		500
 *
 *	extern void my_alarm_entry(os_alarm_handle alarm_h, u32 data);
 *	ErrorCode result;
 *	struct os_alarm_t my_alarm;
 *	u32 init_data = 0xdeadbeef;
 *
 *	my_alarm.alarm_type = EN_OS_ALARM_REPEAT;
 *	my_alarm.trigger = DELAY_TIME_1000_MS; // trigger it after current_time + 1 sec.
 *	my_alarm.interval = DELAY_TIME_500_MS; // repeat event each 0.5 sec.
 *	my_alarm.entry_fn = my_alarm_entry;
 *	my_alarm.arg = init_data;
 *
 *	result = OS_AlarmCreate(&my_alarm, "MyAlarm");
 *	if (ERROR_NO_ERROR == result)
 *	{
 *		result = OS_AlarmActive(my_alarm.alarm_handle);
 *	}
 * \endcode
 */

ErrorCode OS_AlarmReset(os_alarm_handle u32AlarmHandle, u32 u32Trigger, u32 u32Interval);
/*!\fn ErrorCode OS_AlarmReset(os_alarm_handle u32AlarmHandle, u32 u32Trigger, u32 u32Interval)
 * \brief
 * Re-initialize alarm
 *
 * OS_AlarmReset can be used to reset the periodic interval of an existing alarm.
 *
 * \param    u32AlarmHandle            (Input) Handle of alarm
 * \param    u32Trigger                (Input) trigger time first
 * \param    u32Interval               (Input) event period
 *
 * \return   Alarm Operation Error Code
 *
 * \retval ERROR_NO_ERROR              Operation success
 * \retval OS_ERR_ALARM_EINVAL         Invalid argument
 * \retval OS_ERR_ALARM_ENOENT         No such entity
 *
 * \code
 *	Example Code:
 *
 *	#define DELAY_TIME_1000_MS		1000
 *	#define DELAY_TIME_500_MS		500
 *	#define DELAY_TIME_200_MS		200
 *	#define DELAY_TIME_100_MS		100
 *
 *	extern void my_alarm_entry(os_alarm_handle alarm_h, u32 data);
 *	ErrorCode result;
 *	struct os_alarm_t my_alarm;
 *	u32 init_data = 0xdeadbeef;
 *	u32 trigger, interval;
 *
 *	my_alarm.alarm_type = EN_OS_ALARM_REPEAT;
 *	my_alarm.trigger = DELAY_TIME_1000_MS; // trigger it after current_time + 1 sec.
 *	my_alarm.interval = DELAY_TIME_500_MS; // repeat event each 0.5 sec.
 *	my_alarm.entry_fn = my_alarm_entry;
 *	my_alarm.arg = init_data;
 *
 *	result = OS_AlarmCreate(&my_alarm, "MyAlarm");
 *	if (ERROR_NO_ERROR == result)
 *	{
 *		result = OS_AlarmActive(my_alarm.alarm_handle);
 *	}
 *
 *	...
 *
 *	// change frequency of the alarm event, repeat event each 0.1 second
 *	trigger = DELAY_TIME_200_MS;
 *	interval = DELAY_TIME_100_MS;
 *
 *	result = OS_AlarmReset(my_alarm.alarm_handle, trigger, interval);
 * \endcode
 */

ErrorCode OS_AlarmEnable(os_alarm_handle u32AlarmHandle);
/*!\fn ErrorCode OS_AlarmEnable(os_alarm_handle u32AlarmHandle)
 * \brief
 * Enable alarm
 *
 * This re-enables an alarm that has previously been disabled by
 * a call to OS_AlarmDisable. This is most often used with a periodic alarm.
 *
 * A periodic alarm that has been disabled and later re-enabled will fire
 * at the same intervals it did previously.
 * For example, a periodic alarm that fired every 10 seconds at time
 * T0, T10, T20, T30... etc that was disabled for 15 seconds at time T31 and
 * then re-enabled would then start firing again at T50, T60, T70 etc.
 *
 *
 * \param    u32AlarmHandle            (Input) Handle of alarm
 *
 * \return   Alarm Operation Error Code
 *
 * \retval ERROR_NO_ERROR              Operation success
 * \retval OS_ERR_ALARM_EINVAL         Invalid argument
 * \retval OS_ERR_ALARM_ENOENT         No such entity
 *
 * \code
 *	Example Code:
 *
 *	extern struct os_alarm_t my_alarm;
 *	ErrorCode result;
 *
 *	result = OS_AlarmEnable(my_alarm.alarm_handle);
 * \endcode
 */

ErrorCode OS_AlarmDisable(os_alarm_handle u32AlarmHandle);
/*!\fn ErrorCode OS_AlarmDisable(os_alarm_handle u32AlarmHandle)
 * \brief
 * Disable alarm
 *
 * This disables an alarm. This is most often used with a periodic alarm.
 *
 * \param    u32AlarmHandle            (Input) Handle of alarm
 *
 * \return   Alarm Operation Error Code
 *
 * \retval ERROR_NO_ERROR              Operation success
 * \retval OS_ERR_ALARM_EINVAL         Invalid argument
 * \retval OS_ERR_ALARM_ENOENT         No such entity
 *
 * \code
 *	Example Code:
 *
 *	extern struct os_alarm_t my_alarm;
 *	ErrorCode result;
 *
 *	result = OS_AlarmDisable(my_alarm.alarm_handle);
 * \endcode
 */

/*!\file
 * Timer/Counter
 *
 * Kernel counters can be used to keep track of how many times
 * a particular event has occurred. Usually this event is an
 * external signal of some sort. The most common use of counters
 * is in the implementation of clocks, but they can be useful with
 * other event sources as well. Application code can attach alarms
 * to counters, causing a function to be called when some number of
 * events have occurred.
 *
 */

typedef cyg_handle_t os_timer_handle;

u64 OS_TimerGetTime(void);
/*!\fn u64 OS_TimerGetTime(void)
 * \brief
 * Getting RTC current counter value
 *
 * The RTC current counter value can be obtained using OS_TimerGetTime.
 *
 * \return   Current counter value
 *
 * \code
 * \endcode
 */

void OS_TimerSetTime(u32 u32NewClock);
/*!\fn void OS_TimerSetTime(u32 u32NewClock)
 * \brief
 * Setting RTC clock value(unit: second)
 *
 * The current clock value associated with RTC can be using
 * modified with OS_TimerSetTime.
 *
 * \param    u32NewClock               (Input) New RTC counter value(parsing from DVB TDT/TOT packets)
 *
 * \return   None
 *
 * \code
 * \endcode
 */

// ----------------------------------------------------------------------------
// Interrupt Management Function
/*!\file
 * Interrupt
 *
 * The kernel provides an interface for installing interrupt handlers and
 * controlling when interrupts occur.
 *
 */

#define OS_ISR_MIN                 1   /*!< Min ISR numbers */
#define OS_ISR_MAX                 63  /*!< Max ISR numbers */

typedef cyg_handle_t os_intr_handle;
typedef struct cyg_interrupt os_intr_obj;

typedef u32 (*os_isr_entry)(u32 u32Vector, u32 u32Data);
typedef void (*os_dsr_entry)(u32 u32Vector, u32 u32Count, u32 u32Data);

enum os_isr_results_e
{
    OS_ISR_HANDLED  = 1,               /*!< Interrupt was handled */
    OS_ISR_CALL_DSR = 2                /*!< Schedule DSR */
};

typedef enum os_intr_status_e
{
	EN_OS_INTR_NONE=0x00,              /*!< No Interrupt handle */
	EN_OS_INTR_CREATE=0x01,            /*!< Interrupt created */
	EN_OS_INTR_ATTACH=0x02,            /*!< Interrupt attached */
	EN_OS_INTR_DETACH=0x04,            /*!< Interrupt detached */
	EN_OS_INTR_MASK=0x08,              /*!< Interrupt masked */
	EN_OS_INTR_UNMASK=0x10             /*!< Interrupt unmasked */
} EN_OS_INTR_STATUS;

/**
 * Structure of interrupt control block
 */
struct os_intr_t
{
	struct list_head list;             /*!< Interrupt link list */

	os_intr_handle intr_handle;        /*!< Interrupt handle */
	os_intr_obj intr;                  /*!< Interrupt object */

	u32 vector;                        /*!< Vector number */
	u32 level;                         /*!< Priority level */
	u32 arg;                           /*!< Argument to isr/dsr function */
	EN_OS_INTR_STATUS status;          /*!< Status of interrupt handle */

	s8 *name;                          /*!< Name of interrupt */

	os_isr_entry isr_fn;               /*!< ISR entry function */
	os_dsr_entry dsr_fn;               /*!< DSR entry function */
};

ErrorCode OS_InterruptCreate(struct os_intr_t *pstIntr, const s8 *ps8IntrName);
/*!\fn ErrorCode OS_InterruptCreate(struct os_intr_t *pstIntr, const s8 *ps8IntrName)
 * \brief
 * Define interrupt handler
 *
 * The function provides an interface for installing interrupt handlers
 * and controlling when interrupts occur.
 *
 * \param    pstIntr                   (Input/Output) Interrupt Control Block
 * \param    ps8IntrName               (Input) Name of interrupt
 *
 * \return   Interrupt Operation Error Code
 *
 * \retval ERROR_NO_ERROR              Operation success
 * \retval OS_ERR_INTR_EINVAL          Invalid argument
 * \retval OS_ERR_INTR_ERANGE          Math result cannot be represented
 *
 * \code
 *	Example Code:
 *
 *	#define INTERRUPT_VECTOR_SOFTWARE	 1
 *
 *	u32 my_isr_entry(u32 vector, u32 data)
 *	{
 *		bool dsr_required = 0;
 *
 *		//  ... some code
 *
 *		return dsr_required ? OS_ISR_CALL_DSR : OS_ISR_HANDLED;
 *	}
 *	void my_dsr_entry(u32 vector, u32 count, u32 data)
 *	{
 *		...
 *	}
 *
 *	extern u32 my_isr_entry(u32 vector, u32 data);
 *	extern void my_dsr_entry(u32 vector, u32 count, u32 data);
 *	struct os_intr_t my_interrupt;
 *	ErrorCode result;
 *
 *	my_interrupt.vector = INTERRUPT_VECTOR_SOFTWARE;
 *	my_interrupt.level = 0;
 *	my_interrupt.arg = 0xdeadbeef;
 *	my_interrupt.isr_fn = my_isr_entry;
 *	my_interrupt.dsr_fn = my_dsr_entry;
 *
 *	result = OS_InterruptCreate(&my_interrupt, "MyInterrupt");
 *
 *	// attach interrupt now or later
 *	// result = OS_InterruptAttach(my_interrupt.intr_handle);
 * \endcode
 */

ErrorCode OS_InterruptDelete(os_intr_handle u32IntrHandle);
/*!\fn ErrorCode OS_InterruptDelete(os_intr_handle u32IntrHandle)
 * \brief
 * Delete interrupt handler
 *
 * OS_DeleteInterrupt can be used if an interrupt handler is
 * no longer required.
 *
 * \param    u32IntrHandle             (Input) Interrupt handle
 *
 * \return   Interrupt Operation Error Code
 *
 * \retval ERROR_NO_ERROR              Operation success
 * \retval OS_ERR_INTR_EINVAL          Invalid argument
 * \retval OS_ERR_INTR_ENOENT          No such entity
 *
 * \code
 *	Example Code:
 *
 *	extern struct os_intr_t my_interrupt;
 *	ErrorCode result;
 *
 *	result = OS_InterruptDelete(my_interrupt.intr_handle);
 * \endcode
 */

ErrorCode OS_InterruptAttach(os_intr_handle u32IntrHandle);
/*!\fn ErrorCode OS_InterruptAttach(os_intr_handle u32IntrHandle)
 * \brief
 * Attach interrupt
 *
 * Attaching whichever handle is currently appropriate.
 *
 * This makes it possible to have several different interrupt handlers
 * for a given vector, attaching whichever one is currently appropriate.
 *
 * \param    u32IntrHandle             (Input) Interrupt handle
 *
 * \return   Interrupt Operation Error Code
 *
 * \retval ERROR_NO_ERROR              Operation success
 * \retval OS_ERR_INTR_EINVAL          Invalid argument
 * \retval OS_ERR_INTR_ENOENT          No such entity
 *
 * \code
 *	Example Code:
 *
 *	#define INTERRUPT_VECTOR_SOFTWARE	 1
 *
 *	extern u32 my_isr_entry(u32 vector, u32 data);
 *	extern void my_dsr_entry(u32 vector, u32 count, u32 data);
 *	struct os_intr_t my_interrupt;
 *	ErrorCode result;
 *
 *	my_interrupt.vector = INTERRUPT_VECTOR_SOFTWARE;
 *	my_interrupt.level = 0;
 *	my_interrupt.arg = 0xdeadbeef;
 *	my_interrupt.isr_fn = my_isr_entry;
 *	my_interrupt.dsr_fn = my_dsr_entry;
 *
 *	result = OS_InterruptCreate(&my_interrupt, "MyInterrupt");
 *	result = OS_InterruptAttach(my_interrupt.intr_handle);
 * \endcode
 */

ErrorCode OS_InterruptDetach(os_intr_handle u32IntrHandle);
/*!\fn ErrorCode OS_InterruptDetach(os_intr_handle u32IntrHandle)
 * \brief
 * Detach interrupt
 *
 * Replacing an interrupt handler requires a call to OS_DetachInterrupt,
 * followed by another call to OS_AttachInterrupt for the replacement handler.
 *
 * \param    u32IntrHandle             (Input) Interrupt handle
 *
 * \return   Interrupt Operation Error Code
 *
 * \retval ERROR_NO_ERROR              Operation success
 * \retval OS_ERR_INTR_EINVAL          Invalid argument
 * \retval OS_ERR_INTR_ENOENT          No such entity
 *
 * \code
 *	Example Code:
 *
 *	extern struct os_intr_t my_interrupt;
 *	ErrorCode result;
 *
 *	result = OS_InterruptDetach(my_interrupt.intr_handle);
 * \endcode
 */

ErrorCode OS_InterruptAck(u32 u32Vector);
/*!\fn ErrorCode OS_InterruptAck(u32 u32Vector)
 * \brief
 * Acknowledge interrupt
 *
 * Notify controller that interrupt has been handled and
 * need to clear the status flag.
 *
 * \param    u32Vector                 (Input) Interrupt vector number
 *
 * \return   Interrupt Operation Error Code
 *
 * \retval ERROR_NO_ERROR              Operation success
 * \retval OS_ERR_INTR_ERANGE          Math result cannot be represented
 *
 * \code
 *	Example Code:
 *
 *	u32 my_isr_entry(u32 vector, u32 data)
 *	{
 *		bool dsr_required = 0;
 *
 *		//  ... some code
 *
 *		// ISR has been handled, clear interrupt status flag
 *		OS_InterruptAck(vector);
 *
 *		return dsr_required ? OS_ISR_CALL_DSR : OS_ISR_HANDLED;
 *	}
 * \endcode
 */

ErrorCode OS_InterruptMask(u32 u32Vector);
/*!\fn ErrorCode OS_InterruptMask(u32 u32Vector)
 * \brief
 * Mask interrupt
 *
 * Ignore interrupt for which handler is installed.
 *
 * \param    u32Vector                 (Input) Interrupt vector number
 *
 * \return   Interrupt Operation Error Code
 *
 * \retval ERROR_NO_ERROR              Operation success
 * \retval OS_ERR_INTR_ERANGE          Math result cannot be represented
 *
 * \code
 * \endcode
 */

ErrorCode OS_InterruptUnMask(u32 u32Vector);
/*!\fn ErrorCode OS_InterruptUnMask(u32 u32Vector)
 * \brief
 * Unmask interrupt
 *
 * Attend interrupt for which handler is installed.
 *
 * \param    u32Vector                 (Input) Interrupt vector number
 *
 * \return   Interrupt Operation Error Code
 *
 * \retval ERROR_NO_ERROR              Operation success
 * \retval OS_ERR_INTR_ERANGE          Math result cannot be represented
 *
 * \code
 * \endcode
 */

ErrorCode OS_InterruptGetInfo(u32 u32Vector, struct os_intr_t *pstIntr);
/*!\fn ErrorCode OS_InterruptGetInfo(u32 u32Vector, struct os_intr_t *pstIntr)
 * \brief
 * Reference interrupt status
 *
 * \param    u32Vector                 (Input) Interrupt vector number
 * \param    pstIntr                   (Output) The control handle of interrupt
 *
 * \return   Interrupt Operation Error Code
 *
 * \retval ERROR_NO_ERROR              Operation success
 * \retval OS_ERR_INTR_ERANGE          Math result cannot be represented
 * \retval OS_ERR_INTR_ENOENT          No such entity
 *
 * \code
 * \endcode
 */

void OS_InterruptDisable(void);
/*!\fn ErrorCode OS_InterruptDisable(void)
 * \brief
 * Disable interrupt
 *
 * It is possible to disable all interrupts globally,
 * using OS_DisableInterrupt.
 */

void OS_InterruptEnable(void);
/*!\fn ErrorCode OS_InterruptEnable(void)
 * \brief
 * Enable interrupt
 *
 * It is possible to enable all interrupts globally,
 * using OS_EnableInterrupt.
 */

// ----------------------------------------------------------------------------
// Inter-Process Communication Management Function

#define OS_IPC_NO_WAIT      0          /*!< Return immediately */
#define OS_IPC_ALWAYS_WAIT  0xFFFFFFFF /*!< Block until it has successfully decremented the counter */

/*!\file
 * Semaphore
 *
 * Associated with each semaphore is an integer counter that keeps
 * track of the number of events that have not yet been processed.
 * If this counter is zero, an attempt by a consumer thread to wait
 * on the semaphore will block until some other thread or a DSR posts
 * a new event to the semaphore. If the counter is greater than zero
 * then an attempt to wait on the semaphore will consume one event,
 * in other words decrement the counter, and return immediately.
 * Posting to a semaphore will wake up the first thread that is
 * currently waiting, which will then resume inside the semaphore
 * wait operation and decrement the counter again.
 *
 */

typedef cyg_handle_t os_sem_handle;
typedef struct cyg_sem_t os_sem_obj;

struct os_semaphore_t
{
	struct list_head list;             /*!< Semaphore link list */

	os_sem_handle sem_handle;          /*!< Semaphore handle */
	os_sem_obj sema;                   /*!< Semaphore object */
	s32 init_count;                    /*!< The semaphore count */

	s8 *name;                          /*!< Name of semaphore */
};

struct os_seminfo_t
{
	os_thread_handle owner_thread;
	os_thread_handle wait_thread;      /*!< The header of waiting threads queue */

	s32 init_count;                    /*!< The semaphore count */
	s32 curr_count;                    /*!< The semaphore used-count */
	s8 *name;                          /*!< Name of semaphore */
};

ErrorCode OS_SemaphoreCreate(struct os_semaphore_t *pstSema, const s8 *ps8SemaName);
/*!\fn ErrorCode OS_SemaphoreCreate(struct os_semaphore_t *pstSema, const s8 *ps8SemaName)
 * \brief
 * Create a semaphore
 *
 * OS_SemaphoreCreate is used to initialize a semaphore, and
 * the semaphore initial counter value can be any number, zero, or positive,
 * but typically a value of zero is used to indicate that no events have
 * occurred yet.
 *
 * \param    pstSema                   (Input/Output) Semaphore Control Block
 * \param    ps8SemaName               (Input) Name of semaohore
 *
 * \return   Semaphore Operation Error Code
 *
 * \retval ERROR_NO_ERROR              Operation success
 * \retval OS_ERR_SEMA_EINVAL          Invalid argument
 *
 * \code
 * \endcode
 */

ErrorCode OS_SemaphoreDestroy(os_sem_handle u32SemaHandle);
/*!\fn ErrorCode OS_SemaphoreDestroy(os_sem_handle u32SemaHandle)
 * \brief
 * Delete a semaphore
 *
 * \param    u32SemaHandle             (Input) Semaphore handle
 *
 * \return   Semaphore Operation Error Code
 *
 * \retval ERROR_NO_ERROR              Operation success
 * \retval OS_ERR_SEMA_EINVAL          Invalid argument
 * \retval OS_ERR_SEMA_ENOENT          No such entity
 * \retval OS_ERR_SEMA_EDEADLOCK       Resource deadlock would occur
 *
 * \code
 * \endcode
 */

ErrorCode OS_SemaphoreSignal(os_sem_handle u32SemaHandle);
/*!\fn ErrorCode OS_SemaphoreSignal(os_sem_handle u32SemaHandle)
 * \brief
 * Post a semaphore
 *
 * This increments the counter and wakes up the first thread waiting on
 * the semaphore.
 *
 * \param    u32SemaHandle             (Input) Semaphore handle
 *
 * \return   Semaphore Operation Error Code
 *
 * \retval ERROR_NO_ERROR              Operation success
 * \retval OS_ERR_SEMA_EINVAL          Invalid argument
 * \retval OS_ERR_SEMA_ENOENT          No such entity
 *
 * \code
 * \endcode
 */

ErrorCode OS_SemaphoreWait(os_sem_handle u32SemaHandle, u32 u32WaitTime);
/*!\fn ErrorCode OS_SemaphoreWait(os_sem_handle u32SemaHandle, u32 u32WaitTime)
 * \brief
 * Waiting a semaphore resource release
 *
 * OS_SemaphoreWait is used by a consumer thread to wait for an event.
 * If the current counter is greater than 0, in other words if the event
 * has already occurred in the past, then the counter will be decremented
 * and the call will return immediately.
 *
 * \param    u32SemaHandle             (Input) Semaphore handle
 * \param    u32WaitTime               (Input) Time limit on the operation
 *
 * \return   Semaphore Operation Error Code
 *
 * \retval ERROR_NO_ERROR              Operation success
 * \retval OS_ERR_SEMA_EINVAL          Invalid argument
 * \retval OS_ERR_SEMA_ENOENT          No such entity
 * \retval OS_ERR_SEMA_EBUSY           Resource busy
 * \retval OS_ERR_SEMA_ETIMEDOUT       Operation timed out
 * \retval OS_ERR_SEMA_EINTR           Operation interrupted
 *
 * \code
 * \endcode
 */

ErrorCode OS_SemaphoreRotate(os_sem_handle u32SemaHandle);
/*!\fn ErrorCode OS_SemaphoreRotate(os_sem_handle u32SemaHandle)
 * \brief
 */

ErrorCode OS_SemaphoreGetInfo(os_sem_handle u32SemaHandle, struct os_seminfo_t *pstSemaInfo);
/*!\fn ErrorCode OS_SemaphoreGetInfo(os_sem_handle u32SemaHandle, struct os_seminfo_t *pstSemaInfo)
 * \brief
 * Reference semaphore status
 *
 * \param    u32SemaHandle             (Input) Semaphore handle
 * \param    pstSemaInfo               (Output) Semaphore Information
 *
 * \return   Semaphore Operation Error Code
 *
 * \retval ERROR_NO_ERROR              Operation success
 * \retval OS_ERR_SEMA_EINVAL          Invalid argument
 * \retval OS_ERR_SEMA_ENOENT          No such entity
 * \retval OS_ERR_SEMA_ENOBUFS         No buffer space available
 *
 * \code
 * \endcode
 */

/*!\file
 * Mutex
 *
 * The purpose of mutexes is to let threads share resources safely.
 * If two or more threads attempt to manipulate a data structure with
 * no locking between them then the system may run for quite some time
 * without apparent problems, but sooner or later the data structure
 * will become inconsistent and the application will start behaving
 * strangely and is quite likely to crash. The same can apply even when
 * manipulating a single variable or some other resource.
 *
 */

typedef cyg_handle_t os_mutex_handle;
typedef struct cyg_mutex_t os_mutex_obj;
typedef cyg_threadqueue os_mutex_threadqueue;

struct os_mutex_t
{
	struct list_head list;             /*!< Mutex link list */

	os_mutex_handle mutex_handle;      /*!< Mutex handle */
	os_mutex_obj mutex;                /*!< Mutex object */

	s8 *name;                          /*!< Name of mutex */
};

struct os_mutexinfo_t
{
	os_thread_obj        *owner;       /*!< Current locking thread */
	os_mutex_threadqueue wait_list;    /*!< Queue of waiting threads */
	bool				 locked;       /*!< True if locked */
};

ErrorCode OS_MutexCreate(struct os_mutex_t *pstMutex, const s8 *ps8MutexName);
/*!\fn ErrorCode OS_MutexCreate(struct os_mutex_t *pstMutex, const s8 *ps8MutexName)
 * \brief
 * A mutex must be initialized before it can be used, by calling OS_MutexCreate.
 *
 * \param    pstMutex                  (Input/Output) Mutex Control Block
 * \param    ps8MutexName              (Input) Name of mutex
 *
 * \return   Mutex Operation Error Code
 *
 * \retval ERROR_NO_ERROR              Operation success
 * \retval OS_ERR_MUTEX_EINVAL         Invalid argument
 *
 * \code
 * \endcode
 */

ErrorCode OS_MutexDestroy(os_mutex_handle u32MutexHandle);
/*!\fn ErrorCode OS_MutexDestroy(os_mutex_handle u32MutexHandle)
 * \brief
 * Delete mutex
 *
 * If a mutex is no longer required and there are no threads waiting on it
 * then OS_MutexDestroy can be used.
 *
 * \param    u32MutexHandle            (Input) Handle of mutex
 *
 * \return   Mutex Operation Error Code
 *
 * \retval ERROR_NO_ERROR              Operation success
 * \retval OS_ERR_MUTEX_EINVAL         Invalid argument
 * \retval OS_ERR_MUTEX_ENOENT         No such entity
 * \retval OS_ERR_MUTEX_EDEADLOCK      Resource deadlock would occur
 *
 * \code
 * \endcode
 */

ErrorCode OS_MutexLock(os_mutex_handle u32MutexHandle, u32 u32WaitTime);
/*!\fn ErrorCode OS_MutexLock(os_mutex_handle u32MutexHandle, u32 u32WaitTime)
 * \brief
 * Get the mutex and blocking another thread desire to get the mutex
 *
 * \param    u32MutexHandle            (Input) Handle of mutex
 * \param    u32WaitTime               (Input) Time limit on the operation
 *
 * \return   Mutex Operation Error Code
 *
 * \retval ERROR_NO_ERROR              Operation success
 * \retval OS_ERR_MUTEX_EINVAL         Invalid argument
 * \retval OS_ERR_MUTEX_ENOENT         No such entity
 * \retval OS_ERR_MUTEX_EBUSY          Resource busy
 * \retval OS_ERR_MUTEX_ETIMEDOUT      Operation timed out
 * \retval OS_ERR_MUTEX_EINTR          Operation interrupted
 *
 * \code
 * \endcode
 */

ErrorCode OS_MutexUnLock(os_mutex_handle u32MutexHandle);
/*!\fn ErrorCode OS_MutexUnLock(os_mutex_handle u32MutexHandle)
 * \brief
 * Release the mutex
 *
 * The current owner of a mutex should call OS_MutexUnLock when
 * a lock is no longer required. This operation must be performed
 * by the owner, not by another thread.
 *
 * \param    u32MutexHandle            (Input) Handle of mutex
 *
 * \return   Mutex Operation Error Code
 *
 * \retval ERROR_NO_ERROR              Operation success
 * \retval OS_ERR_MUTEX_EINVAL         Invalid argument
 * \retval OS_ERR_MUTEX_ENOENT         No such entity
 *
 * \code
 * \endcode
 */

ErrorCode OS_MutexRelease(os_mutex_handle u32MutexHandle);
/*!\fn ErrorCode OS_MutexRelease(os_mutex_handle u32MutexHandle)
 * \brief
 * Forcing the all locked threads wake up
 *
 * OS_MutexRelease can be used to wake up all threads that are
 * currently blocked inside a call to OS_MutexLock for a specific mutex.
 *
 * \param    u32MutexHandle            (Input) Handle of mutex
 *
 * \return   Mutex Operation Error Code
 *
 * \retval ERROR_NO_ERROR              Operation success
 * \retval OS_ERR_MUTEX_EINVAL         Invalid argument
 * \retval OS_ERR_MUTEX_ENOENT         No such entity
 *
 * \code
 * \endcode
 */

ErrorCode OS_MutexGetInfo(os_mutex_handle u32MutexHandle, struct os_mutexinfo_t *pstMutexInfo);
/*!\fn ErrorCode OS_MutexGetInfo(os_mutex_handle u32MutexHandle, struct os_mutexinfo_t *pstMutexInfo)
 * \brief
 * Reference mutex status
 *
 * \param    u32MutexHandle            (Input) Handle of mutex
 * \param    pstMutexInfo              (Output) Information of mutex
 *
 * \return   Mutex Operation Error Code
 *
 * \retval ERROR_NO_ERROR              Operation success
 * \retval OS_ERR_MUTEX_EINVAL         Invalid argument
 * \retval OS_ERR_MUTEX_ENOENT         No such entity
 * \retval OS_ERR_MUTEX_ENOBUFS        No buffer space available
 *
 * \code
 * \endcode
 */

/*!\file
 * Event Flag
 *
 * Event flags allow a consumer thread to wait for one of several different
 * types of event to occur. Alternatively it is possible to wait for some
 * combination of events. The implementation is relatively straightforward.
 * Each event flag contains a 32-bit integer. Application code associates
 * these bits with specific events, so for example bit 0 could indicate that
 * an I/O operation has completed and data is available, while bit 1 could
 * indicate that the user has pressed a start button. A producer thread or
 * a DSR can cause one or more of the bits to be set, and a consumer thread
 * currently waiting for these bits will be woken up.
 *
 * Unlike semaphores no attempt is made to keep track of event counts.
 * It does not matter whether a given event occurs once or multiple times
 * before being consumed, the corresponding bit in the event flag will change
 * only once.
 *
 */

typedef cyg_handle_t os_event_handle;
typedef struct cyg_flag_t os_event_obj;
typedef cyg_threadqueue os_event_threadqueue;

enum os_event_mode_e
{
	OS_EVENT_MODE_AND=0,            /*!< All the specified event bits are set, and event flag is not cleared */
	OS_EVENT_MODE_OR=2,             /*!< At least one of the specified event bits is set, and event flag is not cleared */
	OS_EVENT_MODE_AND_CLR=1,        /*!< All the specified event bits are set, and the entire event flag is cleared */
	OS_EVENT_MODE_OR_CLR=3,         /*!< At least one of the specified event bits is set, and the entire flag is cleared */
};

struct os_event_t
{
	struct list_head list;             /*!< Event link list */

	os_event_handle event_handle;      /*!< Event flag handle */
	os_event_obj event;                /*!< Event flag object */

    u32 flag;                          /*!< The flag value */

    s8 *name;                          /*!< Name of event flag */
};

struct os_eventinfo_t
{
    u32    flag;                       /* The flag value */
    os_event_threadqueue wait_list;    /* Queue of waiting threads */
};

ErrorCode OS_EventFlagCreate(struct os_event_t *pstEvent, const s8 *ps8EventName);
/*!\fn ErrorCode OS_EventFlagCreate(struct os_event_t *pstEvent, const s8 *ps8EventName)
 * \brief
 * Create event flag
 *
 * \param    pstEvent                  (Input/Output) Event Control Block
 * \param    ps8EventName              (Input) Name of event flag
 *
 * \return   EventFlag Operation Error Code
 *
 * \retval ERROR_NO_ERROR              Operation success
 * \retval OS_ERR_EVENT_EINVAL         Invalid argument
 *
 * \code
 * \endcode
 */

ErrorCode OS_EventFlagDestroy(os_event_handle u32EventHandle);
/*!\fn ErrorCode OS_EventFlagDestroy(os_event_handle u32EventHandle)
 * \brief
 * Delete event flag
 *
 * \param    u32EventHandle            (Input) Handle of event flag
 *
 * \return   EventFlag Operation Error Code
 *
 * \retval ERROR_NO_ERROR              Operation success
 * \retval OS_ERR_EVENT_EINVAL         Invalid argument
 * \retval OS_ERR_EVENT_ENOENT         No such entity
 * \retval OS_ERR_EVENT_EDEADLOCK      Resource deadlock would occur
 *
 * \code
 * \endcode
 */

ErrorCode OS_EventFlagWait(os_event_handle u32EventHandle, u32 u32Pattern, u32 *pu32Flag, u8 u8Mode, u32 u32WaitTime);
/*!\fn ErrorCode OS_EventFlagWait(os_event_handle u32EventHandle, u32 u32Pattern, u32 *pu32Flag, u8 u8Mode, u32 u32WaitTime)
 * \brief
 * Waiting the required condition is satisfied
 *
 * \param    u32EventHandle            (Input) Handle of event flag
 * \param    u32Pattern                (Input) Combination of flag bits
 * \param    pu32Flag                  (Output) Event flag
 * \param    u8Mode                    (Input) Option mode
 * \param    u32WaitTime               (Input) Time limit on the operation
 *
 * \return   EventFlag Operation Error Code
 *
 * \retval ERROR_NO_ERROR              Operation success
 * \retval OS_ERR_EVENT_EINVAL         Invalid argument
 * \retval OS_ERR_EVENT_ENOENT         No such entity
 * \retval OS_ERR_EVENT_ENOBUFS        No buffer space available
 * \retval OS_ERR_EVENT_EOPNOTSUPP     Operation not supported
 * \retval OS_ERR_EVENT_EBUSY          Resource busy
 * \retval OS_ERR_EVENT_ETIMEDOUT      Operation timed out
 * \retval OS_ERR_EVENT_EINTR          Operation interrupted
 *
 * \code
 * \endcode
 */

ErrorCode OS_EventFlagSet(os_event_handle u32EventHandle, u32 u32Flag);
/*!\fn ErrorCode OS_EventFlagSet(os_event_handle u32EventHandle, u32 u32Flag)
 * \brief
 * Specified bits are or'd into the current event flag value
 *
 * \param    u32EventHandle            (Input) Handle of event flag
 * \param    u32Flag                   (Input) Event flag
 *
 * \return   EventFlag Operation Error Code
 *
 * \retval ERROR_NO_ERROR              Operation success
 * \retval OS_ERR_EVENT_EINVAL         Invalid argument
 * \retval OS_ERR_EVENT_ENOENT         No such entity
 *
 * \code
 * \endcode
 */

ErrorCode OS_EventFlagClear(os_event_handle u32EventHandle, u32 u32Flag);
/*!\fn ErrorCode OS_EventFlagClear(os_event_handle u32EventHandle, u32 u32Flag)
 * \brief
 * Clear one or more bits in the event flag
 *
 * \param    u32EventHandle            (Input) Handle of event flag
 * \param    u32Flag                   (Input) Event flag
 *
 * \return   EventFlag Operation Error Code
 *
 * \retval ERROR_NO_ERROR              Operation success
 * \retval OS_ERR_EVENT_EINVAL         Invalid argument
 * \retval OS_ERR_EVENT_ENOENT         No such entity
 *
 * \code
 * \endcode
 */

ErrorCode OS_EventFlagGetInfo(os_event_handle u32EventHandle, struct os_eventinfo_t *pstEventInfo);
/*!\fn ErrorCode OS_EventFlagGetInfo(os_event_handle u32EventHandle, struct os_eventinfo_t *pstEventInfo)
 * \brief
 * Reference event flag status
 *
 * \param    u32EventHandle            (Input) Handle of event flag
 * \param    pstEventInfo              (Output) Information of event flag
 *
 * \return   EventFlag Operation Error Code
 *
 * \retval ERROR_NO_ERROR              Operation success
 * \retval OS_ERR_EVENT_EINVAL         Invalid argument
 * \retval OS_ERR_EVENT_ENOENT         No such entity
 * \retval OS_ERR_EVENT_ENOBUFS        No buffer space available
 *
 * \code
 * \endcode
 */

/*!\file
 * Mail Box
 *
 * Mail boxes are a synchronization primitive. Like semaphores they can be
 * used by a consumer thread to wait until a certain event has occurred, but
 * the producer also has the ability to transmit some data along with each
 * event. This data, the message, is normally a pointer to some data
 * structure. It is stored in the mail box itself, so the producer thread that
 * generates the event and provides the data usually does not have to block
 * until some consumer thread is ready to receive the event.
 *
 */

typedef cyg_handle_t os_mbox_handle;
typedef struct cyg_mbox os_mbox_obj;
typedef cyg_threadqueue os_mbox_threadqueue;

struct os_mailbox_t
{
	struct list_head list;             /*!< MailBox link list */

	os_mbox_handle mbox_handle;        /*!< Mail box handle */
	os_mbox_obj mbox;                  /*!< Mail box object */

	s8 *name;                          /*!< Name of mail box */
};

struct os_mailboxinfo_t
{
	u32 used_slot;                     /*!< Count of used slots */
	os_mbox_threadqueue wait_list;     /*!< Queue of waiting threads */
};

ErrorCode OS_MailBoxCreate(struct os_mailbox_t *pstMailBox, const s8 *ps8MailBoxName);
/*!\fn ErrorCode OS_MailBoxCreate(struct os_mailbox_t *pstMailBox, const s8 *ps8MailBoxName)
 * \brief
 * Create a mail box
 *
 * Before a mail box can be used it must be created with a call to cyg_mbox_create.
 *
 * \param    pstMailBox                (Input/Output) MailBox Control Block
 * \param    ps8MailBoxName            (Input) Name of mail box
 *
 * \return   MailBox Operation Error Code
 *
 * \retval ERROR_NO_ERROR              Operation success
 * \retval OS_ERR_MBOX_EINVAL          Invalid argument
 *
 * \code
 * \endcode
 */

ErrorCode OS_MailBoxDestroy(os_mbox_handle u32MailBoxHandle);
/*!\fn ErrorCode OS_MailBoxDestroy(os_mbox_handle u32MailBoxHandle)
 * \brief
 * Delete mail box
 *
 * \param    u32MailBoxHandle          (Input) Handle of mail box
 *
 * \return   MailBox Operation Error Code
 *
 * \retval ERROR_NO_ERROR              Operation success
 * \retval OS_ERR_MBOX_EINVAL          Invalid argument
 * \retval OS_ERR_MBOX_ENOENT          No such entity
 *
 * \code
 * \endcode
 */

ErrorCode OS_MailBoxSend(os_mbox_handle u32MailBoxHandle, void *pMail, u32 u32WaitTime);
/*!\fn ErrorCode OS_MailBoxSend(os_mbox_handle u32MailBoxHandle, void *pMail, u32 u32WaitTime)
 * \brief
 * Place a new message in the mail box
 *
 * \param    u32MailBoxHandle          (Input) Handle of mail box
 * \param    pMail                     (Input) Message
 * \param    u32WaitTime               (Input) Time limit on the operation
 *
 * \return   MailBox Operation Error Code
 *
 * \retval ERROR_NO_ERROR              Operation success
 * \retval OS_ERR_MBOX_EINVAL          Invalid argument
 * \retval OS_ERR_MBOX_ENOENT          No such entity
 * \retval OS_ERR_MBOX_EBUSY           Resource busy
 * \retval OS_ERR_MBOX_ETIMEDOUT       Operation timed out
 * \retval OS_ERR_MBOX_EINTR		   Operation interrupted
 *
 * \code
 * \endcode
 */

ErrorCode OS_MailBoxReceive(os_mbox_handle u32MailBoxHandle, void *pMail, u32 u32WaitTime);
/*!\fn ErrorCode OS_MailBoxReceive(os_mbox_handle u32MailBoxHandle, void *pMail, u32 u32WaitTime)
 * \brief
 * Waiting on a mail box
 *
 * \param    u32MailBoxHandle          (Input) Handle of mail box
 * \param    pMail                     (Output) Message
 * \param    u32WaitTime               (Input) Time limit on the operation
 *
 * \return   MailBox Operation Error Code
 *
 * \retval ERROR_NO_ERROR              Operation success
 * \retval OS_ERR_MBOX_EINVAL          Invalid argument
 * \retval OS_ERR_MBOX_ENOENT          No such entity
 * \retval OS_ERR_MBOX_EBUSY           Resource busy
 * \retval OS_ERR_MBOX_ETIMEDOUT       Operation timed out
 * \retval OS_ERR_MBOX_EINTR		   Operation interrupted
 *
 * \code
 * \endcode
 */

ErrorCode OS_MailBoxGetInfo(os_mbox_handle u32MailBoxHandle, struct os_mailboxinfo_t *pstMailBoxInfo);
/*!\fn ErrorCode OS_MailBoxGetInfo(os_mbox_handle u32MailBoxHandle, struct os_mailboxinfo_t *pstMailBoxInfo)
 * \brief
 * Reference mail box status
 *
 * \param    u32MailBoxHandle          (Input) Handle of mail box
 * \param    pstMailBoxInfo            (Output) Information of mail box
 *
 * \return   MailBox Operation Error Code
 *
 * \retval ERROR_NO_ERROR              Operation success
 * \retval OS_ERR_MBOX_EINVAL          Invalid argument
 * \retval OS_ERR_MBOX_ENOENT          No such entity
 * \retval OS_ERR_MBOX_ENOBUFS         No buffer space available
 *
 * \code
 * \endcode
 */

/*!\file
 * Message Buffer
 *
 * Message buffers provide both synchronization and communication by passing
 * messages. Unlike mailboxes, message buffers allow copying and transfer of
 * variable length messages. This makes it possible to pass messages even when
 * the sending and receiving end have no shared memory in common.
 *
 */

#define OS_MQ_MAX_PRIO		32

typedef cyg_handle_t os_mqueue_handle;

struct os_mqueue_t
{
	struct list_head list;             /*!< Message queue link list */

	os_mqueue_handle mqueue_handle;    /*!< Message queue handle */

	u32 mq_maxmsg;                     /*!< Max. number of messages */
	u32 mq_msgsize;                    /*!< Max. message buffer size (in bytes) */

	s8 *name;                          /*!< Name of message queue */
};

struct os_mqueueinfo_t
{
	u32 mq_flags;                      /*!< mqueue attribute flags */
	u32 mq_maxmsg;                     /*!< max number of messages */
	u32 mq_msgsize;                    /*!< max message size */
	u32 mq_curmsgs;                    /*!< Number of messages currently queued */
};

ErrorCode OS_MsgQueueCreate(struct os_mqueue_t *pstMsgQueue, const s8 *ps8MsgQueueName);
/*!\fn ErrorCode OS_MsgQueueCreate(struct os_mqueue_t *pstMsgQueue, const s8 *ps8MsgQueueName)
 * \brief
 * Create message queue
 *
 * Message buffers are objects used for managing the transfer of variable length
 * messages. The main difference from a mailbox (mbx) is that variable length
 * message contents are copied during sending and receiving in the message buffer.
 *
 * \param    pstMsgQueue               (Input/Output) Message queue control block
 * \param    ps8MsgQueueName           (Input) Name of message buffer
 *
 * \return   MessageQueue Operation Error Code
 *
 * \retval ERROR_NO_ERROR              Operation success
 *
 * \code
 * \endcode
 */

ErrorCode OS_MsgQueueDestroy(cyg_handle_t u32MsgQueueHandle);
/*!\fn ErrorCode OS_MsgQueueDestroy(cyg_handle_t u32MsgQueueHandle)
 * \brief
 * Delete message queue
 *
 * This function releases memory area used for the control block of
 * the associated message buffer and the buffer area used for storing messages.
 *
 * \param    u32MsgQueueHandle         (Input) Handle of message buffer
 *
 * \return   MessageQueue Operation Error Code
 *
 * \retval ERROR_NO_ERROR              Operation success
 *
 * \code
 * \endcode
 */

ErrorCode OS_MsgQueueSend(cyg_handle_t u32MsgQueueHandle, void *pMessage, u32 u32Len, u32 u32Priority, u32 u32WaitTime);
/*!\fn ErrorCode OS_MsgQueueSend(cyg_handle_t u32MsgQueueHandle, void *pMessage, u32 u32Len, u32 u32Priority, u32 u32WaitTime)
 * \brief
 * Send message to message queue with timeout
 *
 * OS_MsgQueueSend() copies the message stored at the address given by pMessage to
 * the message buffer specified by u32MsgQueueHandle.
 *
 * OS_MsgQueueSend() shall behave as if the message is inserted into the message buffer
 * at the position indicated by the u32Priority argument.
 *
 * A message with a larger numeric value of u32Priority shall be inserted before
 * messages with lower values of u32Priority.
 *
 * \param    u32MsgQueueHandle         (Input) Handle of message buffer
 * \param    pMessage                  (Input) Message
 * \param    u32Len                    (Input) Message length
 * \param    u32Priority               (Input) Message priority
 * \param    u32WaitTime               (Input) Time limit on the operation
 *
 * \return   MessageQueue Operation Error Code
 *
 * \retval ERROR_NO_ERROR              Operation success
 *
 * \code
 * \endcode
 */

ErrorCode OS_MsgQueueReceive(cyg_handle_t u32MsgQueueHandle, void *pMessage, u32 u32WaitTime);
/*!\fn ErrorCode OS_MsgQueueReceive(cyg_handle_t u32MsgQueueHandle, void *pMessage, u32 u32WaitTime)
 * \brief
 * Receive message from message queue with timeout
 *
 * OS_MsgQueueReceive receives the message from the message buffer
 * specified by u32MsgQueueHandle, and stores it at the memory location
 * given by pMessage.
 *
 * \param    u32MsgQueueHandle         (Input) Handle of message buffer
 * \param    pMessage                  (Output) Message
 * \param    u32WaitTime               (Input) Time limit on the operation
 *
 * \return   MessageQueue Operation Error Code
 *
 * \retval ERROR_NO_ERROR              Operation success
 *
 * \code
 * \endcode
 */

ErrorCode OS_MsgQueueFlush(cyg_handle_t u32MsgQueueHandle);
/*!\fn ErrorCode OS_MsgQueueFlush(cyg_handle_t u32MsgQueueHandle)
 *  \brief
 *  Clear all messages in queue
 *
 * \param    u32MsgQueueHandle         (Input) Handle of message buffer
 *
 * \return   MessageQueue Operation Error Code
 *
 * \retval ERROR_NO_ERROR              Operation success
 *
 * \code
 * \endcode
 */

ErrorCode OS_MsgQueueGetInfo(cyg_handle_t u32MsgQueueHandle, struct os_mqueueinfo_t *pstMQInfo);
/*!\fn ErrorCode OS_MsgQueueGetInfo(cyg_handle_t u32MsgQueueHandle, struct os_mqueueinfo_t *pstMQInfo)
 * \brief
 * Get message queue status
 *
 * \param    u32MsgQueueHandle         (Input) Handle of message buffer
 * \param    pstMQInfo                 (Output) Information of message queue
 *
 * \return   MessageQueue Operation Error Code
 *
 * \retval ERROR_NO_ERROR              Operation success
 *
 * \code
 * \endcode
 */

/**
 * \brief
 * Enumerate to indicate the List debug message mode
 */
typedef enum {
    EN_OS_LIST_DBG_MSG_MODE_NONE = 0,	/* no debug message                     */
    EN_OS_LIST_DBG_MSG_MODE_NAME,		/* dump List itsm's name                */
    EN_OS_LIST_DBG_MSG_MODE_FULL,		/* dump List itsm's name and address    */
    EN_OS_LIST_DBG_MSG_MODE_NUM,
}EN_OS_LIST_DBG_MSG_MODE;

void OS_IPC_CheckSemaphoreList (EN_OS_LIST_DBG_MSG_MODE enMode, const char* ps8File, const char* ps8Function, int s32Line);
/*!\fn void OS_IPC_CheckSemaphoreList (EN_OS_LIST_DBG_MSG_MODE enMode, const char* ps8File, const char* ps8Function, int s32Line)
 * \brief
 * Check Semaphore List
 *
 * \param    enMode                 (Input) Debug Message Mode
 * \param    ps8File                (Input) __FILE__
 * \param    ps8Function            (Input) __FUNCTION__
 * \param    s32Line                (Input) __LINE__
 */

void OS_IPC_CheckMutexList (EN_OS_LIST_DBG_MSG_MODE enMode, const char* ps8File, const char* ps8Function, int s32Line);
/*!\fn void OS_IPC_CheckMutexList (EN_OS_LIST_DBG_MSG_MODE enMode, const char* ps8File, const char* ps8Function, int s32Line)
 * \brief
 * Check IPC Mutex List
 *
 * \param    enMode                 (Input) Debug Message Mode
 * \param    ps8File                (Input) __FILE__
 * \param    ps8Function            (Input) __FUNCTION__
 * \param    s32Line                (Input) __LINE__
 */

void OS_IPC_CheckEventList (EN_OS_LIST_DBG_MSG_MODE enMode, const char* ps8File, const char* ps8Function, int s32Line);
/*!\fn void OS_IPC_CheckEventList (EN_OS_LIST_DBG_MSG_MODE enMode, const char* ps8File, const char* ps8Function, int s32Line)
 * \brief
 * Check IPC Event List
 *
 * \param    enMode                 (Input) Debug Message Mode
 * \param    ps8File                (Input) __FILE__
 * \param    ps8Function            (Input) __FUNCTION__
 * \param    s32Line                (Input) __LINE__
 */

void OS_IPC_CheckMailBoxList (EN_OS_LIST_DBG_MSG_MODE enMode, const char* ps8File, const char* ps8Function, int s32Line);
/*!\fn void OS_IPC_CheckMailBoxList (EN_OS_LIST_DBG_MSG_MODE enMode, const char* ps8File, const char* ps8Function, int s32Line)
 * \brief
 * Check IPC MailBox List
 *
 * \param    enMode                 (Input) Debug Message Mode
 * \param    ps8File                (Input) __FILE__
 * \param    ps8Function            (Input) __FUNCTION__
 * \param    s32Line                (Input) __LINE__
 */

void OS_IPC_CheckMsgQueueList (EN_OS_LIST_DBG_MSG_MODE enMode, const char* ps8File, const char* ps8Function, int s32Line);
/*!\fn void OS_IPC_CheckMsgQueueList (EN_OS_LIST_DBG_MSG_MODE enMode, const char* ps8File, const char* ps8Function, int s32Line)
 * \brief
 * Check IPC Message Queue List
 *
 * \param    enMode                 (Input) Debug Message Mode
 * \param    ps8File                (Input) __FILE__
 * \param    ps8Function            (Input) __FUNCTION__
 * \param    s32Line                (Input) __LINE__
 */

void OS_IPC_CheckAllList (EN_OS_LIST_DBG_MSG_MODE enMode, const char* ps8File, const char* ps8Function, int s32Line);
/*!\fn void OS_IPC_CheckAllList (EN_OS_LIST_DBG_MSG_MODE enMode, const char* ps8File, const char* ps8Function, int s32Line)
 * \brief
 * Check All IPC List
 *
 * \param    enMode                 (Input) Debug Message Mode
 * \param    ps8File                (Input) __FILE__
 * \param    ps8Function            (Input) __FUNCTION__
 * \param    s32Line                (Input) __LINE__
 */

void OS_TaskCheckList (EN_OS_LIST_DBG_MSG_MODE enMode, const char* ps8File, const char* ps8Function, int s32Line);
/*!\fn void OS_TaskCheckList (EN_OS_LIST_DBG_MSG_MODE enMode, const char* ps8File, const char* ps8Function, int s32Line)
 * \brief
 * Check Task List
 *
 * \param    enMode                 (Input) Debug Message Mode
 * \param    ps8File                (Input) __FILE__
 * \param    ps8Function            (Input) __FUNCTION__
 * \param    s32Line                (Input) __LINE__
 */

#define OS_IPC_CHECK_SEMAPHORE_LIST(p)      OS_IPC_CheckSemaphoreList (p, __FILE__, __FUNCTION__, __LINE__)
#define OS_IPC_CHECK_MUTEX_LIST(p)          OS_IPC_CheckMutexList (p, __FILE__, __FUNCTION__, __LINE__)
#define OS_IPC_CHECK_EVENT_LIST(p)          OS_IPC_CheckEventList (p, __FILE__, __FUNCTION__, __LINE__)
#define OS_IPC_CHECK_MAIL_BOX_LIST(p)       OS_IPC_CheckMailBoxList (p, __FILE__, __FUNCTION__, __LINE__)
#define OS_IPC_CHECK_MSG_QUEUE_LIST(p)      OS_IPC_CheckMsgQueueList (p, __FILE__, __FUNCTION__, __LINE__)
#define OS_IPC_CHECK_ALL_LIST(p)            OS_IPC_CheckAllList (p, __FILE__, __FUNCTION__, __LINE__)
#define OS_TASK_CHECK_LIST(p)               OS_TaskCheckList (p, __FILE__, __FUNCTION__, __LINE__)
// ----------------------------------------------------------------------------
// Memory Pool Management Function
/*!\file
 * Memory Pool
 *
 * The memory allocation package provides support for additional
 * memory pools, to complement the system heap. These pools are not
 * created automatically by the system, they have to be created by
 * application code or by other packages.
 *
 * There are exported APIs for two types of pool: fixed and variable.
 *
 * Allocating memory from a fixed memory pool is very fast and, more
 * importantly, deterministic. However the size of each allocation is
 * fixed at the time the pool is created. This is not a problem if the
 * required allocations are all the same size, or nearly so, but otherwise
 * the memory will be used inefficiently. The pool cannot become fragmented.
 *
 * Variable memory pools provide essentially the same functionality as
 * the system heap, so are rarely used. However on some targets not all
 * free memory is assigned automatically to the system heap. For example
 * there may be a small area of fast on-chip memory as well as the slower
 * external memory. The system heap will only use the latter. A variable
 * memory pool can be created for the former, allowing application code
 * to dynamically allocate fast memory where appropriate.
 *
 */

#define OS_MEM_NO_WAIT      0          /*!< Return immediately */
#define OS_MEM_ALWAYS_WAIT  0xFFFFFFFF /*!< Block until it has successfully got the memory */

typedef cyg_handle_t os_mempool_handle;
typedef cyg_mempool_info os_mempool_info;

typedef enum os_mempool_type_e
{
	EN_OS_MEM_BLOCK_FIXED,             /*!< Fixed memory block size */
	EN_OS_MEM_BLOCK_VARIABLE           /*!< Variable memory block size */
} EN_OS_MPOOL_TYPE;

/**
 * Structure of memory pool control block
 */
struct os_mempool_t
{
	struct list_head list;             /*!< Mempool link list */

	os_mempool_handle mempool_handle;  /*!< Mempool handle */
	void *mpool;			           /*!< Mempool object */

	EN_OS_MPOOL_TYPE block_type;       /*!< The type of memory block size */

	void *base;                        /*!< Pointer to the mempool */
	s32 total_size;                    /*!< The total size of mempool */
	s32 block_size;                    /*!< The allocated memory block size */
	s32 minfree;                       /*!< The minimum free size of mempool */

	s8 *name;                          /*!< Name of mempool */
};

ErrorCode OS_MemPoolCreate(struct os_mempool_t *pstMemPool, const s8 *ps8MemPoolName);
/*!\fn ErrorCode OS_MemPoolCreate(struct os_mempool_t *pstMemPool, const s8 *ps8MemPoolName)
 * \brief
 * Create memory pool
 *
 * \param    pstMemPool                (Input/Output) Mempool Control Block
 * \param    ps8MemPoolName            (Input) Name of Mempool
 *
 * \return   Mempool Operation Error Code
 *
 * \retval ERROR_NO_ERROR              Operation success
 * \retval OS_ERR_MEMPOOL_EINVAL       Invalid argument
 * \retval OS_ERR_MEMPOOL_EOPNOTSUPP   Operation not supported
 *
 * \code
 *	Example Code:
 *
 *	<FIXED>:
 *
 *	ErrorCode result;
 *	struct os_mempool_t my_mempool;
 *
 *	my_mempool.block_type = EN_OS_MEM_BLOCK_FIXED;
 *	my_mempool.total_size = 8*1024;
 *	my_mempool.block_size = 128;
 *
 *	result = OS_CreateMemPool(&my_mempool, "MyMempoolFixed");
 *
 *	<VARIABLE>:
 *
 *	ErrorCode result;
 *	struct os_mempool_t my_mempool;
 *
 *	my_mempool.block_type = EN_OS_MEM_BLOCK_VARIABLE;
 *	my_mempool.total_size = 8*1024;
 *
 *	result = OS_MemPoolCreate(&my_mempool, "MyMempoolVar");
 * \endcode
 */

ErrorCode OS_MemPoolDestroy(os_mempool_handle u32MemPoolHandle);
/*!\fn ErrorCode OS_MemPoolDestroy(os_mempool_handle u32MemPoolHandle)
 * \brief
 * Delete memory pool
 *
 * \param    u32MemPoolHandle          (Input) Handle of Mempool
 *
 * \return   Mempool Operation Error Code
 *
 * \retval ERROR_NO_ERROR              Operation success
 * \retval OS_ERR_MEMPOOL_EINVAL       Invalid argument
 * \retval OS_ERR_MEMPOOL_ENOENT       No such entity
 * \retval OS_ERR_MEMPOOL_EDEADLOCK    Resource deadlock would occur
 *
 * \code
 *	Example Code:
 *
 *	extern struct os_mempool_t my_mempool;
 *
 *	ErrorCode result;
 *	os_mempool_handle my_mempool_h;
 *
 *	my_mempool_h = my_mempool.mempool_handle;
 *
 *	result = OS_MemPoolDestroy(my_mempool_h);
 * \endcode
 */

ErrorCode OS_MemPoolGetInfo(os_mempool_handle u32MemPoolHandle, os_mempool_info *pstMemPoolInfo);
/*!\fn ErrorCode OS_MemPoolGetInfo(os_mempool_handle u32MemPoolHandle, os_mempool_info *pstMemPoolInfo)
 * \brief
 * Reference memory pool status
 *
 * \param    u32MemPoolHandle          (Input) Handle of Mempool
 * \param    pstMemPoolInfo            (Output) Information of mempool
 *
 * \return   Mempool Operation Error Code
 *
 * \retval ERROR_NO_ERROR              Operation success
 * \retval OS_ERR_MEMPOOL_EINVAL       Invalid argument
 * \retval OS_ERR_MEMPOOL_ENOENT       No such entity
 *
 * \code
 *	Example Code:
 *
 *	extern struct os_mempool_t my_mempool;
 *	os_mempool_info my_mempool_info;
 *	ErrorCode result;
 *
 *	result = OS_MemPoolGetInfo(my_mempool.mempool_handle, &my_mempool_info);
 * \endcode
 */

void *OS_MemAlloc(os_mempool_handle u32MemPoolHandle, u32 u32Size, u32 u32WaitTime);
/*!\fn void *OS_MemAlloc(os_mempool_handle u32MemPoolHandle, u32 u32Size, u32 u32WaitTime)
 * \brief
 * Get memory block
 *
 * \param    u32MemPoolHandle          (Input) Handle of Mempool
 * \param    u32Size                   (Input) The allocating memory range
 * \param    u32WaitTime               (Input) Check whether any free block or suspended wait
 *
 * \return   The point of memory base address or invalid value(0)
 *
 * \code
 *	Example Code:
 *
 *	extern struct os_mempool_t my_mempool;
 *
 *	ErrorCode result;
 *	os_mempool_handle my_mempool_h;
 *	void *my_buff = NULL;
 *
 *	my_mempool_h = my_mempool.mempool_handle;
 *
 *	my_buff = OS_MemAlloc(my_mempool_h, 1024, OS_MEM_NO_WAIT);
 * \endcode
 */

ErrorCode OS_MemFree(os_mempool_handle u32MemPoolHandle, void *pMem);
/*!\fn ErrorCode OS_MemFree(os_mempool_handle u32MemPoolHandle, void *pMem)
 * \brief
 * Release memory block
 *
 * \param    u32MemPoolHandle          (Input) Handle of Mempool
 * \param    pMem                      (Input) Pointer of free memory
 *
 * \return   Mempool Operation Error Code
 *
 * \retval ERROR_NO_ERROR              Operation success
 * \retval OS_ERR_MEMPOOL_EINVAL       Invalid argument
 * \retval OS_ERR_MEMPOOL_ENOENT       No such entity
 *
 * \code
 *	Example Code:
 *
 *	extern struct os_mempool_t my_mempool;
 *	extern void *my_buff;
 *
 *	ErrorCode result;
 *	os_mempool_handle my_mempool_h;
 *
 *	my_mempool_h = my_mempool.mempool_handle;
 *
 *	result = OS_MemFree(my_mempool_h, my_buff);
 * \endcode
 */
#if 0
void *OS_MemResize(os_mempool_handle u32MemPoolHandle, void *pMem, u32 u32OldSize, u32 u32NewSize);
/*!\fn void *OS_MemResize(os_mempool_handle u32MemPoolHandle, void *pMem, u32 u32OldSize, u32 u32NewSize)
 * \brief
 * Resize existing allocated memory back into memory pool.
 *
 * The data is copied from old to new though.
 *
 * \param    u32MemPoolHandle          (Input) Handle of Mempool
 * \param    pMem                      (Input) Pointer of free memory
 * \param    u32OldSize                (Input) Size of previous memory
 * \param    u32NewSize                (Input) Size of new assignment memory
 *
 * \return   The point of memory base address or invalid value(0)
 *
 * \code
 *	Example Code:
 *
 *	extern struct os_mempool_t my_mempool;
 *
 *	ErrorCode result;
 *	os_mempool_handle my_mempool_h;
 *	void *old_buff = NULL;
 *	void *new_buff = NULL;
 *	int old_size = 1024;
 *	int new_size = 4096;
 *
 *	my_mempool_h = my_mempool.mempool_handle;
 *
 *	old_buff = OS_MemAlloc(my_mempool_h, old_size, OS_MEM_NO_WAIT);
 *	new_buff = OS_MemResize(my_mempool_h, old_buff, old_size, new_size);
 * \endcode
 */
#endif
s32 OS_MemGetTotalFreeSize(os_mempool_handle u32MemPoolHandle);
/*!\fn s32 OS_MemGetTotalFreeSize(os_mempool_handle u32MemPoolHandle)
 * \brief
 * Get total amount of free memory.
 *
 * \param    u32MemPoolHandle          (Input) Handle of memory pool
 *
 * \return   Total free memory size or invalid value(-1)
 *
 * \code
 *	Example Code:
 *
 *	extern struct os_mempool_t my_mempool;
 *
 *	ErrorCode result;
 *	os_mempool_handle my_mempool_h;
 *	s32 totle_free;
 *
 *	my_mempool_h = my_mempool.mempool_handle;
 *
 *	totle_free = OS_MemGetTotalFreeSize(my_mempool_h);
 * \endcode
 */

s32 OS_MemGetMaxFreeSize(os_mempool_handle u32MemPoolHandle);
/*!\fn s32 OS_MemGetMaxFreeSize(os_mempool_handle u32MemPoolHandle)
 * \brief
 * Get largest free memory block size.
 *
 * \param    u32MemPoolHandle          (Input) Handle of memory pool
 *
 * \return   The maximum block size of free memory or invalid value(-1)
 *
 * \code
 *	Example Code:
 *
 *	extern struct os_mempool_t my_mempool;
 *
 *	ErrorCode result;
 *	os_mempool_handle my_mempool_h;
 *	s32 max_free;
 *
 *	my_mempool_h = my_mempool.mempool_handle;
 *
 *	max_free = OS_MemGetMaxFreeSize(my_mempool_h);
 * \endcode
 */
ErrorCode OS_MemPoolIndex2Handle(EN_OS_MEM_BLK_LIST_MODE enMode, u32 u32Index, u32* u32MemPoolHandle );

ErrorCode OS_MemDumpBlockList(os_mempool_handle u32MemPoolHandle, EN_OS_MEM_BLK_LIST_MODE enMode);
/*!\fn ErrorCode OS_MemDumpBlockList(os_mempool_handle u32MemPoolHandle, EN_OS_MEM_BLK_LIST_MODE enMode)
 * \brief
 * Dump memory blocks usage.
 *
 * \param    u32MemPoolHandle			(Input) Handle of memory pool
 * \param    enMode						(Input) information mode of memory pool
 *
 * \return   Mempool Operation Error Code
 *
 * \code
 *	Example Code:
 *
 *	extern struct os_mempool_t my_mempool;
 *
 *	ErrorCode result;
 *	os_mempool_handle my_mempool_h;
 *
 *	my_mempool_h = my_mempool.mempool_handle;
 *
 *	result = OS_MemDumpBlockList(my_mempool_h);
 * \endcode
 */

ErrorCode OS_MemPoolDump(os_mempool_handle u32MemPoolHandle);
/*!\fn ErrorCode OS_MemPoolDump(os_mempool_handle u32MemPoolHandle)
 * \brief
 * Dump memory blocks usage.
 *
 * \param    u32MemPoolHandle			(Input) Handle of memory pool
 *
 * \return   Mempool Operation Error Code
 */

struct os_function_t
{
	u32 stack_size;
	u32 ra_offset;
	u32 pc;
	u32 sp;
	u32 ra;
};

#if (OS_MEM_WATCH == 1)
// ----------------------------------------------------------------------------
// Memory Pool Watch Function
#include "os_memwatch.h"	/* put this at end of file, it is for memory watch function. */
#endif /* #if (OS_MEM_WATCH == 1) */

#endif /* _OS_H_ */
