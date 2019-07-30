/**

    This header file of NvtIPC APIs.

    This header file of NvtIPC APIs for user space application using.
    Applications can use the APIs to communicate with CORE1 and CORE3.

    @file       nvtipc.h
    Copyright   Novatek Microelectronics Corp. 2014.  All rights reserved.

*/

#ifndef _NVTIPC_H
#define _NVTIPC_H
#include <stdio.h>
#ifdef __cplusplus
extern "C" {
#endif
/**
    @name Core define value
*/
//@{
#define __CORE1  1           ///< Core MIPS1
#define __CORE2  2           ///< Core MIPS2
#define __CORE3  3           ///< DSP

#define __CORE   __CORE2
//#define __CORE   __CORE2
//#define __CORE   __CORE3
//@}


#define VOS_IPC   1

//------------------------------------------------------------------------------
// Typedefs
//------------------------------------------------------------------------------
typedef signed int          NVTIPC_KEY;     ///< Signed 16 bits data type
typedef unsigned long       NVTIPC_U32;     ///< Unsigned 32 bits data type
typedef signed long         NVTIPC_I32;     ///< Signed 32 bits data type
typedef unsigned long long  NVTIPC_U64;     ///< Unsigned 64 bits data type


#ifndef ENUM_DUMMY4WORD
/**
    Macro to generate dummy element for enum type to expand enum size to word (4 bytes)
*/
//@{
#define ENUM_DUMMY4WORD(name)   E_##name = 0x10000000
//@}
#endif

/**
     @name NvtIpc message queue number & size
*/
//@{
#if __CORE == __CORE3
#define NVTIPC_MSG_QUEUE_NUM             4  ///<  queue number
#define NVTIPC_MSG_ELEMENT_NUM           32 ///<  queue element number
#define NVTIPC_MSG_ELEMENT_SIZE          24 ///<  queue element size
#else
#define NVTIPC_MSG_QUEUE_NUM             16 ///<  queue number
#define NVTIPC_MSG_ELEMENT_NUM           16 ///<  queue element number
#define NVTIPC_MSG_ELEMENT_SIZE          24 ///<  queue element size
#endif
//@}

/**
   Core ID.

   The core ID to send message to.
*/
typedef enum _NVTIPC_SENDTO{
    NVTIPC_SENDTO_CORE1 = 1,                 ///< the core MIPS1
    NVTIPC_SENDTO_CORE2 = 2,                 ///< the core MIPS2
    NVTIPC_SENDTO_CORE3 = 3,                 ///< DSP
    NVTIPC_SENDTO_CORE4 = 4,                 ///< DSP2
    NVTIPC_SENDTO_MAX = NVTIPC_SENDTO_CORE4,
    ENUM_DUMMY4WORD(NVTIPC_SENDTO)
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


/**
   Error Code.

   Any error code occured will display on uart or return.
*/
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
    ENUM_DUMMY4WORD(NVTIPC_ER)
}NVTIPC_ER;

/**
     @name the key & queue id of system queue
*/
//@{
#define NVTIPC_SYS_KEY             (0x0000FFFF)   ///<  the key of system queue
#define NVTIPC_SYS_QUEUE_ID        (0)            ///<  the id of system queue
//@}

/**
   System command ID.

*/
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
    ENUM_DUMMY4WORD(NVTIPC_SYS_CMD)
}NVTIPC_SYS_CMD;


/**
   NVTIPC signal.

*/
typedef enum _NVTIPC_SIG{
    NVTIPC_SIG_SUSPEND = 1,         ///< suspend signal
    NVTIPC_SIG_RESUME,              ///< resume  signal
    ENUM_DUMMY4WORD(NVTIPC_SIG)
}NVTIPC_SIG;


/**
   System command message info.

*/
typedef struct _NVTIPC_SYS_MSG{
    NVTIPC_SYS_CMD   sysCmdID;           ///< system command ID.
    NVTIPC_U32       DataAddr;           ///< system command Data Address.
    NVTIPC_U32       DataSize;           ///< system command Data Size.
    NVTIPC_SENDER    SenderCoreID;       ///< sender CoreID
    NVTIPC_I32       ret;                ///< system command return result.
}NVTIPC_SYS_MSG;

/**
   System performance measure command message info.

*/
typedef struct _NVTIPC_SYS_PERF_MSG{
    NVTIPC_SYS_CMD   sysCmdID;           ///< system command ID.
    NVTIPC_U32       perfCoreID1;        ///< performance measure core id 1.
    NVTIPC_U32       perfCoreID2;        ///< performance measure core id 2.
    NVTIPC_SENDER    SenderCoreID;       ///< sender CoreID
    NVTIPC_I32       ret;                ///< system command return result.
}NVTIPC_SYS_PERF_MSG;

/**
   Get long counter command msg info.

*/
typedef struct _NVTIPC_SYS_LONG_COUNTER_MSG {
	NVTIPC_U32       sec;				 ///< long counter sec.
	NVTIPC_U32       usec;				 ///< long counter usec.
} NVTIPC_SYS_LONG_COUNTER_MSG;

/**
    The callback function of system call command.
*/
typedef void (*SYSCALL_CMD_FP)(char* cmd);

/**
    The callback function of uart command.
*/
typedef int  (*UART_CMD_FP)(char *cmd);

/**
    The system call command table.
*/
typedef struct _SYSCALL_CMD_TBL {
    char             *cmd;          ///< the command string
    SYSCALL_CMD_FP    func;         ///< the command callback function
} SYSCALL_CMD_TBL;

/**
    The NvtIpc init parameter.
*/
typedef struct _NVTIPC_OPEN_OBJ {
    NVTIPC_U32        workBuffAddr;    ///< the work buffer address
    NVTIPC_U32        workBuffSize;    ///< the work buffer size
    SYSCALL_CMD_TBL   *SysCmdTbl;      ///< the system call command table.
    UART_CMD_FP       uartCmdFp;       ///< the uart command callback function.
} NVTIPC_OPEN_OBJ;


#if __CORE == __CORE2

typedef enum {
    NVTIPC_MEM_TYPE_CACHE            =   0, ///< cache memory
    NVTIPC_MEM_TYPE_NONCACHE         =   1, ///< non-cache memory
    NVTIPC_MEM_TYPE_IO               =   2, ///< io memory
    NVTIPC_MEM_TYPE_MAX              ,      ///<
    ENUM_DUMMY4WORD(NVTIPC_MEM_TYPE)
}NVTIPC_MEM_TYPE;


#define NVTIPC_MEM_ADDR_PRE_DEFINED      (0x80000000)                               ///<  the starting address of dram1
#define NVTIPC_MEM_ADDR_DRAM1_ALL_FLAG   NVTIPC_MEM_ADDR_PRE_DEFINED | 0x00000001   ///<
#define NVTIPC_MEM_ADDR_DRAM2_ALL_FLAG   NVTIPC_MEM_ADDR_PRE_DEFINED | 0x00000002   ///<
#define NVTIPC_MEM_MMIO_BASE_ADDR        (0xF0000000)
#define NVTIPC_MEM_INVALID_ADDR          (0xFFFFFFFF)


#endif


#if __CORE == __CORE1
/**
     Install NvtIpc flag and semaphore id.

*/
extern void       NvtIpc_InstallID(void) _SECTION(".kercfg_text");

/**
     Wait until CPU2 power on ready.

*/
extern void       NvtIpc_WaitCPU2PowerOnReady(void);

/**
     Wait until DSP power on ready.

*/
extern void       NvtIpc_WaitDSPPowerOnReady(void);
#endif

/**
     Calculate the needed work buffer size of NvtIpc.

*/
extern NVTIPC_U32    NvtIpc_CalBuffSize(void);

/**
    Open NvtIpc task.

    This is the NvtIpc task initial function.

    @param openObj: the init parameters.

    @return
        - @b NVTIPC_ER_OK: initial success.
        - @b NVTIPC_ER_ID_NOT_INSTALLED: the NvtIpc flag and semaphore id is not installed.
        - @b NVTIPC_ER_PARM: the initial parameter has some error.

    Example:
    @code
    {
        _ALIGNED(4) static UINT8 WorkBuf[8192];
        static SYSCALL_CMD_TBL ipccmd_tbl[]={
        {"lviewd",(SYSCALL_CMD_FP)xExamNvtIpc_testsys},
        {"hfs",(SYSCALL_CMD_FP)xExamNvtIpc_testsys},
        {"",NULL}   // this item should be the last item
        };
        NVTIPC_OPEN_OBJ   openObj = {0};

        openObj.workBuffAddr = (UINT32)&WorkBuf;
        openObj.workBuffSize = sizeof(WorkBuf);
        openObj.SysCmdTbl = ipccmd_tbl;
        openObj.uartCmdFp = (UART_CMD_FP)SxCmd_DoCommand;
        NvtIpc_Open(&openObj);
    }
    @endcode

*/
extern NVTIPC_ER     NvtIpc_Open(NVTIPC_OPEN_OBJ   *openObj);

/**
    Close NvtIpc task.

    @return
        - @b NVTIPC_ER_OK: close success.

*/
extern NVTIPC_ER     NvtIpc_Close(void);


/**
    Get a key by path.

    @param path: the path for generate key.

    @return The generated hash key.

    Example:
    @code
    {
        NVTIPC_KEY key;

        key = NvtIPC_Ftok("lviewd");
    }
    @endcode


*/
extern NVTIPC_KEY NvtIPC_Ftok(const char *path);

/**
    Get a new message queue by key

    @param key: the key generated by NvtIPC_Ftok().

    @return The message queue ID, return < 0 when some error happened.

    Example:
    @code
    {
        NVTIPC_KEY key;
        NVTIPC_I32 msqid;

        key = NvtIPC_Ftok("lviewd");
        msqid = NvtIPC_MsgGet(key);
    }
    @endcode

*/
extern NVTIPC_I32 NvtIPC_MsgGet(NVTIPC_KEY key);

/**
    Release a message queue.

    @param msqid: the message queue ID.

    @return
        - @b NVTIPC_ER_OK: Release a message queue success.
        - @b NVTIPC_ER_PARM: the input message queue id is incorrect.

    Example:
    @code
    {
        NvtIPC_MsgRel(msqid);
    }
    @endcode

*/
extern NVTIPC_ER NvtIPC_MsgRel(NVTIPC_U32 msqid);

/**
    Send a message to other core.

    @param msqid: the message queue ID.
    @param sendTo: the core to send to.
    @param pMsg: the message data address.
    @param msgsz: the message size.

    @return the message size sent, return < 0 when some error happened.

    Example:
    @code
    {
        NVTIPC_KEY key;
        NVTIPC_I32 msqid;
        UINT32 test[NVTIPC_MSG_ELEMENT_SIZE/4]={0x1000,0x2000,0x3000,0x4000,0x5000,0x6000};
        UINT32 msgsz = NVTIPC_MSG_ELEMENT_SIZE;
        int    sndSize;

        key = NvtIPC_Ftok("lviewd");
        msqid = NvtIPC_MsgGet(key);
        sndSize = NvtIPC_MsgSnd(msqid,NVTIPC_SENDTO_CORE2,&test,msgsz);
    }
    @endcode

*/
extern NVTIPC_I32 NvtIPC_MsgSnd(NVTIPC_U32 msqid, NVTIPC_SENDTO sendTo, void*  pMsg, NVTIPC_U32 msgsz);

/**
    Receive a message from message queue.

    @param msqid: the message queue ID.
    @param pMsg: the buffer to receive a message.
    @param msgsz: the buffer size.

    @return the message size received, return < 0 when some error happened.

    Example:
    @code
    {
        NVTIPC_KEY key;
        NVTIPC_I32 msqid;
        UINT32 msg[NVTIPC_MSG_ELEMENT_SIZE/4];
        int    sndSize;
        BOOL   bContinue = TRUE;

        key = NvtIPC_Ftok("lviewd");
        msqid = NvtIPC_MsgGet(key);

        while(bContinue)
        {
            if ((ret = NvtIPC_MsgRcv(msqid, &msg, sizeof(msg))) < 0)
            {
                DBG_ERR("msgrcv %d\r\n",ret);
                bContinue = FALSE;
                break;
            }
            // do something when receive a message
            // ...
        }
    }
    @endcode

*/
extern NVTIPC_I32 NvtIPC_MsgRcv(NVTIPC_U32 msqid, void*  pMsg, NVTIPC_U32 msgsz);

/**
    Dump debug info.

    It will dump the debug information of NvtIPC of all cores.

*/
extern void       NvtIPC_DumpDebugInfo(void);

/**
    Lock a semaphore in multi cores.

    @param semid: the semaphore ID.

    Example:
    @code
    {
        NvtIPC_SemLock(semid);
    }
    @endcode

*/
extern NVTIPC_I32       NvtIPC_SemLock(NVTIPC_U32 semid);

/**
    Unlock a semaphore in multi cores.

    @param semid: the semaphore ID.

    Example:
    @code
    {
        NvtIPC_SemUnlock(semid);
    }
    @endcode

*/
extern NVTIPC_I32       NvtIPC_SemUnlock(NVTIPC_U32 semid);

/**
    Map a shared memory range that can shared with uitron or map a IO memory that can access HW register.

    @param memtype: the memory type that want to map.
    @param addr: memory address that want to map.
    @param size: memory size that want to map.

    @return
        - @b NULL : map has some error.
        - @b otherwise: the mapped address, the output addr will be page aligned.
*/
extern void*       NvtIPC_mmap(NVTIPC_MEM_TYPE memtype, unsigned int addr , unsigned int size);



/**
    Un-map a shared memory range.

    @param addr: memory address that want to unmap.
    @param size: memory size that want to unmap.

    @return
        - @b -1: unmap has some error.
        - @b  0: Success.
*/
extern NVTIPC_I32       NvtIPC_munmap(void* addr ,unsigned int size);

/**
    Change a physical address to non-cached address

    @param addr: physical address.

    @return
        - @b NVTIPC_MEM_INVALID_ADDR: this physical address is not in memory map list.
        - @b otherwise: non-cached address.
*/
extern unsigned int     NvtIPC_GetNonCacheAddr(unsigned int addr);

/**
    Change a physical address to cached address

    @param addr: physical address.

    @return
        - @b NVTIPC_MEM_INVALID_ADDR: this physical address is not in memory map list.
        - @b otherwise: cached address.
*/

extern unsigned int     NvtIPC_GetCacheAddr(unsigned int addr);

/**
    Change a non-cached address to physical address address

    @param addr: non-cached address.

    @return
        - @b NVTIPC_MEM_INVALID_ADDR: this non-cached address is not in memory map list.
        - @b otherwise: physical address.
*/
extern unsigned int     NvtIPC_GetPhyAddr(unsigned int addr);

/**
    Get IO memory mapped address.

    @param addr: IO memory address.

    @return
        - @b NVTIPC_MEM_INVALID_ADDR: this IO memory address is not in memory map list.
        - @b otherwise: io mapped address.
*/

extern unsigned int     NvtIPC_GetMmioAddr(unsigned int addr);

/**
    Flush the cache memory data.

    @param addr: the cache memory address.
    @param size: the memory size want to flush.

    @return
        - @b <0 : this memory address is not in memory map list.
        - @b =0: operation success.
*/
extern int NvtIPC_FlushCache(unsigned int addr, unsigned int size);

/**
    Wait suspend/resume signal.

    @param none
    @return
        - @b >=0 : some signal received, the signal define value can reference NVTIPC_SIG.
        - @b < 0 : some system error occurred.

    Example:
    @code
    {
		int ipc_sig_ret;
		while (1) {
			ipc_sig_ret = NvtIPC_SigWait();
			switch (ipc_sig_ret) {
			case NVTIPC_SIG_SUSPEND:
				DBG_DUMP("Got suspend signal\r\n");
				// suspend action here
				//
				break;
			case NVTIPC_SIG_RESUME:
				// resume action here
                //
				break;
			default:
				DBG_WRN("not handled ipc signal: %d\r\n", ipc_sig_ret);
				break;
			}
			if (ipc_sig_ret >= 0)
				NvtIPC_SigAck(ipc_sig_ret); // always ack for nvtipc
		}
    }
    @endcode
*/
extern int NvtIPC_SigWait(void);

/**
    Ack for received signal.

    @param none
    @return
        - @b >= 0 : Ack successed.
        - @b <  0 : some system error occurred.
*/
extern int NvtIPC_SigAck(NVTIPC_SIG sig);

#if VOS_IPC
extern NVTIPC_KEY NvtIPC_Ftok_C1(const char *path);
extern NVTIPC_I32 NvtIPC_MsgGet_C1(NVTIPC_KEY key);
extern NVTIPC_ER  NvtIPC_MsgRel_C1(NVTIPC_U32 msqid);
extern NVTIPC_I32 NvtIPC_MsgSnd_C1(NVTIPC_U32 msqid, NVTIPC_SENDTO sendTo, void*  pMsg, NVTIPC_U32 msgsz);
extern NVTIPC_I32 NvtIPC_MsgRcv_C1(NVTIPC_U32 msqid, void*  pMsg, NVTIPC_U32 msgsz);
#endif

#ifdef __cplusplus
}
#endif
/* ----------------------------------------------------------------- */
#endif /* _NVTIPC_H  */
