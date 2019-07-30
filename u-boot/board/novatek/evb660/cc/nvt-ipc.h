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

#include "RCWMacro.h"


//------------------------------------------------------------------------------
// Typedefs
//------------------------------------------------------------------------------
typedef signed int          NVTIPC_KEY;     ///< Signed 16 bits data type
typedef unsigned long       NVTIPC_U32;     ///< Unsigned 32 bits data type
typedef signed long         NVTIPC_I32;     ///< Signed 32 bits data type

typedef enum _NVTIPC_SENDTO{
    NVTIPC_SENDTO_CORE1 = 1,
    NVTIPC_SENDTO_CORE2,
    NVTIPC_SENDTO_CORE3,
    NVTIPC_SENDTO_MAX,
    E_NVTIPC_SENDTO = 0x10000000
}NVTIPC_SENDTO;

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
    E_NVTIPC_SYS_CMD = 0x10000000
}NVTIPC_SYS_CMD;


typedef struct _NVTIPC_SYS_MSG{
    NVTIPC_SYS_CMD   sysCmdID;
    NVTIPC_U32       DataAddr;
    NVTIPC_U32       DataSize;
}NVTIPC_SYS_MSG;

typedef enum
{
    CC_CORE_MIPS1 = 0x0,    ///< 1st Core MIPS1
    CC_CORE_MIPS2,          ///< 2nd Core MIPS2
    CC_CORE_DSP,            ///< 3rd Core DSP

    CC_CORE_NUM,

}CC_CORE_ID;

typedef enum _CC_ER{
    CC_ER_OK                      =  0x00000000, ///< no error
    CC_ER_PARM                    =  -1,         ///< parameter error
    CC_ER_TMOUT                   =  -2,         ///< cc timeout
}CC_ER;

#define CC_RES_ID_MAX      32

typedef enum
{
    CC_RES_ID_0 = 0x0,
    CC_RES_ID_1,
    CC_RES_ID_2,
    CC_RES_ID_3,
	  CC_RES_ID_4,

    // Insert member before this line
    // Don't change the order of following three members
    CC_RES_ID_NUM,

}CC_RES_ID;

STATIC_ASSERT(CC_RES_ID_NUM <= CC_RES_ID_MAX);

#define CC_RESOURCE_IPC     CC_RES_ID_0     //< IPC resource between multi-cpu
#define CC_RESOURCE_RTC     CC_RES_ID_1     //< RTC resource between multi-cpu
#define CC_RESOURCE_CKG     CC_RES_ID_2     //< CKG resource between multi-cpu
#define CC_RESOURCE_TOP     CC_RES_ID_3     //< TOP resource between multi-cpu
#define CC_RESOURCE_GPIO    CC_RES_ID_4     //< TOP resource between multi-cpu

#define NVTIPC_SEM_ID_MAX	8

#define CC_CONFIG_CPU2_NVTIPC_SHARED_MEM		0xC0090100
#define NVTIPC_SEM_NUM							8

// the variables in this structure in shared in CPU1 & CPU2 ,
// so need to use volatile for each variable
typedef struct _NVTIPC_SEM_CTRL{
    volatile UINT32          SemBits;
    volatile UINT32          Cpu1AckBits;
    volatile UINT32          Cpu2AckBits;
    volatile UINT32          Cpu1SemWaitCnt[NVTIPC_SEM_NUM];
    volatile UINT32          Cpu2SemWaitCnt[NVTIPC_SEM_NUM];
    volatile UINT32          Cpu1SemLockTime;
    volatile UINT32          Cpu1SemUnLockTime;
    volatile UINT32          Cpu2SemLockTime;
    volatile UINT32          Cpu2SemUnLockTime;
    volatile INT32           Cpu1Cpu2Err;
    volatile INT32           Cpu1Cpu3Err;
    volatile INT32           Cpu2Cpu1Err;
    volatile INT32           Cpu2Cpu3Err;
    volatile INT32           Cpu3Cpu1Err;
    volatile INT32           Cpu3Cpu2Err;
    volatile UINT32          reserved[3];
}NVTIPC_SEM_CTRL;

#define ipc_getNonCacheAddr(addr) (((addr) & 0x1FFFFFFF)|0xA0000000)

static inline UINT32 NvtIpc_GetSharedMemory(void)
{
	UINT32 Addr = ipc_getNonCacheAddr(INW(CC_CONFIG_CPU2_NVTIPC_SHARED_MEM));
	return Addr;
}

typedef enum
{
	NVTIPC_SEM_ID_0 = 0x0,

	// Insert member before this line
	// Don't change the order of following three members
	NVTIPC_SEM_ID_NUM,

} NVTIPC_SEM_ID;


STATIC_ASSERT(NVTIPC_SEM_ID_NUM <= NVTIPC_SEM_ID_MAX);

#define NVTIPC_SEMID_NAND NVTIPC_SEM_ID_0 	//< Nand semaphore id between
						//< multi-cpu

#if defined(CONFIG_NVT_IPC)

/**
    Get a key by path.

    @param path: the path for generate key.

    @return The generated hash key.

*/
extern NVTIPC_KEY nvt_ipc_Ftok(const char *path);
/**
    Get a new message queue by key

    @param key: the key generated by NvtIPC_Ftok().

    @return The message queue ID, return < 0 when some error happened.

*/
extern NVTIPC_I32 nvt_ipc_MsgGet(NVTIPC_KEY key);
/**
    Release a message queue.

    @param msqid: the message queue ID.

    @return
        - @b NVTIPC_ER_OK: Release a message queue success.
        - @b NVTIPC_ER_PARM: the input message queue id is incorrect.

*/
extern NVTIPC_ER nvt_ipc_MsgRel(NVTIPC_U32 msqid);

/**
    Send a message to other core.

    @param msqid: the message queue ID.
    @param sendTo: the core to send to.
    @param pMsg: the message data address.
    @param msgsz: the message size.

    @return the message size sent, return < 0 when some error happened.

*/
extern NVTIPC_I32 nvt_ipc_MsgSnd(NVTIPC_U32 msqid, NVTIPC_SENDTO sendTo, void*  pMsg, NVTIPC_U32 msgsz);

/**
    Receive a message from message queue.

    @param msqid: the message queue ID.
    @param pMsg: the buffer to receive a message.
    @param msgsz: the buffer size.

    @return the message size received, return < 0 when some error happened.

*/
extern NVTIPC_I32 nvt_ipc_MsgRcv(NVTIPC_U32 msqid, void*  pMsg, NVTIPC_U32 msgsz);

extern int 	nvt_ipc_SemLock(unsigned long semid);
extern int 	nvt_ipc_SemUnLock(unsigned long semid);
extern CC_ER  	cc_hwLockResource(CC_CORE_ID uiCoreID, CC_RES_ID uiResource);
extern CC_ER  	cc_hwUnlockResource(CC_CORE_ID uiCoreID, CC_RES_ID uiResource);
extern void loc_multiCores(void);
extern void unl_multiCores(void);

#else
static inline int nvt_ipc_SemLock(unsigned long semid)
{
    return 0;
}
static inline int nvt_ipc_SemUnLock(unsigned long semid)
{
    return 0;
}
#endif

#endif /* _NT96660_NVT_IPC_H */
