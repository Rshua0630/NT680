/**
    IPC header file
    This file will handle core communications.
    @file       nvt-ipc.h
    @ingroup
    @note
    Copyright   Novatek Microelectronics Corp. 2014.  All rights reserved.

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License version 2 as
    published by the Free Software Foundation.
*/

#ifndef _NT96660_NVT_IPC_H
#define _NT96660_NVT_IPC_H

#include <linux/types.h>
#include <mach/rcw_macro.h>


//------------------------------------------------------------------------------
// Typedefs
//------------------------------------------------------------------------------
typedef signed int          NVTIPC_KEY;     ///< Signed 16 bits data type
typedef unsigned long       NVTIPC_U32;     ///< Unsigned 32 bits data type
typedef signed long         NVTIPC_I32;     ///< Signed 32 bits data type

typedef enum _NVTIPC_SENDTO{
    NVTIPC_SENDTO_CORE1 = 1,
    NVTIPC_SENDTO_CORE2 = 2,
    NVTIPC_SENDTO_CORE3 = 3,
    NVTIPC_SENDTO_CORE4 = 4,
    NVTIPC_SENDTO_MAX = NVTIPC_SENDTO_CORE4,
    E_NVTIPC_SENDTO = 0x10000000
}NVTIPC_SENDTO;

/**
   Sender Core ID.

   The Sendor Core ID.
*/
typedef enum _NVTIPC_SENDER{
    NVTIPC_SENDER_CORE1 = 1,                 ///< the CA53 core1
    NVTIPC_SENDER_CORE2 = 2,                 ///< the CA53 core2
    NVTIPC_SENDER_CORE3 = 3,                 ///< DSP 1
    NVTIPC_SENDER_CORE4 = 4,                 ///< DSP 2
    NVTIPC_SENDER_MAX = NVTIPC_SENDER_CORE4,
    E_NVTIPC_SENDER = 0x10000000
}NVTIPC_SENDER;

typedef enum _NVTIPC_ER{
    NVTIPC_ER_OK                      =   0, ///< no error
    NVTIPC_ER_MBX_ID                  =  -1, ///< there is error on mailbox ID
    NVTIPC_ER_MSGQUE_ID               =  -2, ///< there is error on message queue ID
    NVTIPC_ER_MSGQUE_FULL             =  -3, ///< the message queue is full
    NVTIPC_ER_SND_MSG                 =  -4, ///< has some error when send msg
    NVTIPC_ER_RCV_MSG                 =  -5, ///< has some error when receive msg
    NVTIPC_ER_PARM                    =  -6, ///< has some error of input parameter
    NVTIPC_ER_NO_MORE_QUEUE           =  -7, ///< no more message queue to get
    NVTIPC_ER_ID_NOT_INSTALLED        =  -8, ///< the NvtIPC used flag, semaphore ID is not installed
    NVTIPC_ER_KEY_DUPLICATE           =  -9, ///< the NVTIPC_KEY is duplicate with other message queue
    NVTIPC_ER_RCVSIZE_OVER_LIMIT      =  -10, ///< the receive message size over limit
    NVTIPC_ER_SNDSIZE_OVER_LIMIT      =  -11, ///< the send message size over limit
    NVTIPC_ER_MSGQUE_RELEASED         =  -12, ///< the message queue is released
    NVTIPC_ER_NOT_OPENED              =  -13, ///< the ipc is not opened
    NVTIPC_ER_NO_MORE_SHM             =  -14, ///< no more share memory to get
    E_NVTIPC_ER = 0x10000000
}NVTIPC_ER;

typedef enum _NVTIPC_SYS_CMD{
    NVTIPC_SYSCMD_POWEROFF_REQ = 0,         ///< cpu power off request
    NVTIPC_SYSCMD_SYSCALL_REQ,              ///< system call request
    NVTIPC_SYSCMD_SYSCALL_ACK,              ///< system call ack
    NVTIPC_SYSCMD_UART_REQ,                 ///< uart command request
    NVTIPC_SYSCMD_UART_ACK,                 ///< uart command ack
    NVTIPC_SYSCMD_CPU2_POWERON_READY,       ///< CPU2 power on ready
    NVTIPC_SYSCMD_DSP_POWERON_READY,        ///< DSP power on ready
    NVTIPC_SYSCMD_IPC_DUMPINFO,             ///< dump debug info
    NVTIPC_SYSCMD_SUSPEND_REQ,              ///< cpu suspend request
    NVTIPC_SYSCMD_SUSPEND_ACK,              ///< cpu suspend ack
    NVTIPC_SYSCMD_CPU2_RESUME_READY,        ///< CPU2 resume ready
    NVTIPC_SYSCMD_POWEROFF_ACK,             ///< cpu power off ack
    NVTIPC_SYSCMD_REBOOT_REQ,               ///< system reboot request
    NVTIPC_SYSCMD_REBOOT_ACK,               ///< system reboot ack
    NVTIPC_SYSCMD_APP_POWEROFF_REQ,         ///< linux -> uItron ,application power off request
    NVTIPC_SYSCMD_APP_POWEROFF_ACK,         ///< uItron -> linux ,application power off ack
    NVTIPC_SYSCMD_DSP2_POWERON_READY,       ///< DSP2 power on ready
    NVTIPC_SYSCMD_CHK_AVAILABLE_REQ,        ///< check available request
    NVTIPC_SYSCMD_CHK_AVAILABLE_ACK,        ///< check available ack
    NVTIPC_SYSCMD_PERF_REQ,                 ///< ipc command performance request
	NVTIPC_SYSCMD_PERF_ACK,                 ///< ipc command performance ack
    NVTIPC_SYSCMD_GET_LONG_COUNTER_REQ,		///< get cpu1 long counter time request
	NVTIPC_SYSCMD_GET_LONG_COUNTER_ACK,		///< get cpu1 long counter time ack
    E_NVTIPC_SYS_CMD = 0x10000000
}NVTIPC_SYS_CMD;


typedef struct _NVTIPC_SYS_MSG{
    NVTIPC_SYS_CMD   sysCmdID;           ///< system command ID.
    NVTIPC_U32       DataAddr;           ///< system command Data Address.
    NVTIPC_U32       DataSize;           ///< system command Data Size.
    NVTIPC_SENDER    SenderCoreID;       ///< sender CoreID
    NVTIPC_I32       ret;                ///< system command return result.
}NVTIPC_SYS_MSG;


#define NVTIPC_SEM_ID_MAX	8

typedef enum
{
	NVTIPC_SEM_ID_0 = 0x0,

	// Insert member before this line
	// Don't change the order of following three members
	NVTIPC_SEM_ID_NUM,

} NVTIPC_SEM_ID;

typedef enum _NVTIPC_PM_NOTIFIER{
    NVTIPC_PM_NOTIFIER_RESUME = 1,
    NVTIPC_PM_NOTIFIER_MAX,
}NVTIPC_PM_NOTIFIER;

STATIC_ASSERT(NVTIPC_SEM_ID_NUM <= NVTIPC_SEM_ID_MAX);

#define NVTIPC_SEMID_NAND NVTIPC_SEM_ID_0 	//< Nand semaphore id between
						//< multi-cpu

#if defined(CONFIG_NVT_IPC)

/**
    Get a key by path.

    @param path: the path for generate key.

    @return The generated hash key.

*/
extern NVTIPC_KEY nvt_ipc_ftok(const char *path);
/**
    Get a new message queue by key

    @param key: the key generated by NvtIPC_Ftok().

    @return The message queue ID, return < 0 when some error happened.

*/
extern NVTIPC_I32 nvt_ipc_msg_get(NVTIPC_KEY key);
/**
    Release a message queue.

    @param msqid: the message queue ID.

    @return
        - @b NVTIPC_ER_OK: Release a message queue success.
        - @b NVTIPC_ER_PARM: the input message queue id is incorrect.

*/
extern NVTIPC_ER nvt_ipc_msg_rel(NVTIPC_U32 msqid);

/**
    Send a message to other core.

    @param msqid: the message queue ID.
    @param sendTo: the core to send to.
    @param pMsg: the message data address.
    @param msgsz: the message size.

    @return the message size sent, return < 0 when some error happened.

*/
extern NVTIPC_I32 nvt_ipc_msg_snd(NVTIPC_U32 msqid, NVTIPC_SENDTO sendTo, void*  pMsg, NVTIPC_U32 msgsz);

/**
    Receive a message from message queue.

    @param msqid: the message queue ID.
    @param pMsg: the buffer to receive a message.
    @param msgsz: the buffer size.

    @return the message size received, return < 0 when some error happened.

*/
extern NVTIPC_I32 nvt_ipc_msg_rcv(NVTIPC_U32 msqid, void*  pMsg, NVTIPC_U32 msgsz);

/**
    Lock a semaphore in multi cores.

    @param semid: the semaphore ID.

    @return >=0 for success, < 0 when some error happened, the error code can reference NVTIPC_ER.

    Example:
    @code
    {
        nvt_ipc_sem_lock(NVTIPC_SEMID_NAND);
    }
    @endcode

*/
extern NVTIPC_I32 	nvt_ipc_sem_lock(unsigned long semid);

/**
    Unlock a semaphore in multi cores.

    @param semid: the semaphore ID.

    @return >=0 for success, < 0 when some error happened, the error code can reference NVTIPC_ER.

    Example:
    @code
    {
        nvt_ipc_sem_unlock(NVTIPC_SEMID_NAND);
    }
    @endcode

*/
extern NVTIPC_I32 	nvt_ipc_sem_unlock(unsigned long semid);

/**
    Change a physical address to non-cached address

    @param addr: physical address.

    @return
        - @b 0: this physical address is not in region of uitron Memory.
        - @b otherwise: non-cached address.
*/
extern unsigned int nvt_ipc_get_noncache_addr(unsigned int phy_addr);

/**
    Change a non-cached address to physical address

    @param addr: non-cached address.

    @return
        - @b 0: this non-cached address is not in region of uitron Memory.
        - @b otherwise: physical address.
*/
extern unsigned int nvt_ipc_get_phy_addr(unsigned int noncache_addr);

/**
    Get a IPC share memory.

    @param key: the key generated by NvtIPC_Ftok().

    @return The share memory address, return < 0 when some error happened.

    Example:
    @code
    {
	    NVTIPC_KEY key;
	    NVTIPC_I32 shm_addr;
    	key = nvt_ipc_ftok("lviewd");
        shm_addr = nvt_ipc_shm_get(key,size);
	    if (shm_addr < 0)
	    {
			DBG_ERR("shm_addr = %d\r\n",shm_addr);
	    }
    }
    @endcode

*/
extern NVTIPC_U32   nvt_ipc_shm_get(NVTIPC_KEY key, NVTIPC_U32 size);

/**
    Release a IPC share memory.

    @param shm_addr: the IPC share memory address.

    @return return 0 when success, return < 0 when some error happened.

    Example:
    @code
    {
	    nvt_ipc_shm_rel(shm_addr);
    }
    @endcode

*/
extern NVTIPC_ER    nvt_ipc_shm_rel(NVTIPC_U32 shm_addr);

/**
    Flush IPC share memory data to dram.

    @param shm_addr: the IPC share memory address.
    @param shm_size: the IPC share memory size.

    Example:
    @code
    {
	    nvt_ipc_shm_flush(shm_addr,shm_size);
    }
    @endcode

*/
extern void         nvt_ipc_shm_flush(NVTIPC_U32 shm_addr, NVTIPC_U32 shm_size);

#else
static inline NVTIPC_I32 nvt_ipc_sem_lock(unsigned long semid)
{
    return 0;
}
static inline NVTIPC_I32 nvt_ipc_sem_unlock(unsigned long semid)
{
    return 0;
}
#endif

#endif /* _NT96660_NVT_IPC_H */
