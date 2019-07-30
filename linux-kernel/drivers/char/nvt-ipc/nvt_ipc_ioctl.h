/**

    The header file of NvtIPC ioctl operations.

    @file       nvt_ipc_ioctl.h
    Copyright   Novatek Microelectronics Corp. 2014.  All rights reserved.

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License version 2 as
    published by the Free Software Foundation.
*/
#ifndef __NVT_IPC_IOCTL_H
#define __NVT_IPC_IOCTL_H

#include <asm/ioctl.h>

#define NVTIPC_IOC_MAGIC 'C'

#define NVTIPC_MSG_QUEUE_TOKEN_STR_MAXLEN    32  //  bytes
#define NVTIPC_MSG_ELEMENT_SIZE              24 ///<  queue element size

typedef struct {
	void        *addr;
	unsigned int size;
} NVTIPC_IOC_MEM_BUF, *PNVTIOC_IPC_MEM_BUF;


typedef struct {
	NVTIPC_KEY      key;                          ///< [in]
	NVTIPC_I32      rtn;                          ///< [out]
} NVTIPC_IOC_MSGQUE_GET_S, *PNVTIPC_IOC_MSGQUE_GET_S;

typedef struct {
	NVTIPC_U32      msqid;                        ///< [in]
	NVTIPC_I32      rtn;                          ///< [out]
} NVTIPC_IOC_MSGQUE_REL_S, *PNVTIPC_IOC_MSGQUE_REL_S;

typedef struct {
	char            path[NVTIPC_MSG_QUEUE_TOKEN_STR_MAXLEN + 1]; ///< [in]
	NVTIPC_KEY      rtn;                                       ///< [out]
} NVTIPC_IOC_FTOK_S, *PNVTIPC_IOC_FTOK_S;


typedef struct {
	NVTIPC_U32      msqid;                         ///< [in]
	NVTIPC_SENDTO   sendTo;                        ///< [in]
	char            pMsg[NVTIPC_MSG_ELEMENT_SIZE]; ///< [in]
	NVTIPC_U32      msgsz;                         ///< [in]
	NVTIPC_I32      rtn;                           ///< [out]
} NVTIPC_IOC_MSG_SND_S, *PNVTIPC_IOC_MSG_SND_S;

typedef struct {
	NVTIPC_U32      msqid;                        ///< [in]
	char            pMsg[NVTIPC_MSG_ELEMENT_SIZE];///< [out]
	NVTIPC_U32      msgsz;                        ///< [in]
	NVTIPC_I32      rtn;                          ///< [out]
} NVTIPC_IOC_MSG_RCV_S, *PNVTIPC_IOC_MSG_RCV_S;

typedef struct {
	unsigned int addr;
	unsigned int size;
} NVTIPC_IOC_FLUSH_CACHE_S, *PNVTIPC_IOC_FLUSH_CACHE_S;


typedef struct {
	NVTIPC_I32      rtn;                          ///< [out]
} NVTIOC_IPC_SIG_WAIT_S, *PNVTIOC_IPC_SIG_WAIT_S;


typedef struct {
	NVTIPC_I32      sig;                          ///< [in]
	NVTIPC_I32      rtn;                          ///< [out]
} NVTIOC_IPC_SIG_ACK_S, *PNVTIOC_IPC_SIG_ACK_S;



#define NVTIOC_IPC_MSGQUE_GET     _IOWR(NVTIPC_IOC_MAGIC, 0, NVTIPC_IOC_MSGQUE_GET_S)
#define NVTIOC_IPC_MSGQUE_REL     _IOWR(NVTIPC_IOC_MAGIC, 1, NVTIPC_IOC_MSGQUE_REL_S)
#define NVTIOC_IPC_CONSOLE_CMD    _IOW(NVTIPC_IOC_MAGIC,  2, NVTIPC_IOC_MEM_BUF)
#define NVTIOC_IPC_SYS_CMD        _IOW(NVTIPC_IOC_MAGIC,  3, unsigned int)
#define NVTIOC_IPC_FTOK           _IOWR(NVTIPC_IOC_MAGIC, 4, NVTIPC_IOC_FTOK_S)
#define NVTIOC_IPC_SEM_LOCK       _IOW(NVTIPC_IOC_MAGIC,  5, unsigned int)
#define NVTIOC_IPC_SEM_UNLOCK     _IOW(NVTIPC_IOC_MAGIC,  6, unsigned int)
#define NVTIOC_IPC_MSG_SND        _IOWR(NVTIPC_IOC_MAGIC, 7, NVTIPC_IOC_MSG_SND_S)
#define NVTIOC_IPC_MSG_RCV        _IOWR(NVTIPC_IOC_MAGIC, 8, NVTIPC_IOC_MSG_RCV_S)
#define NVTIOC_IPC_MSGQUE_GET_C1  _IOWR(NVTIPC_IOC_MAGIC, 9, NVTIPC_IOC_MSGQUE_GET_S)
#define NVTIOC_IPC_MSGQUE_REL_C1  _IOWR(NVTIPC_IOC_MAGIC, 10, NVTIPC_IOC_MSGQUE_REL_S)
#define NVTIOC_IPC_MSG_SND_C1     _IOWR(NVTIPC_IOC_MAGIC, 11, NVTIPC_IOC_MSG_SND_S)
#define NVTIOC_IPC_MSG_RCV_C1      _IOWR(NVTIPC_IOC_MAGIC,12, NVTIPC_IOC_MSG_RCV_S)
#define NVTIOC_IPC_GET_DRAM1_RANGE _IOR(NVTIPC_IOC_MAGIC, 13, NVTIPC_IOC_MEM_BUF)
#define NVTIOC_IPC_GET_DRAM2_RANGE _IOR(NVTIPC_IOC_MAGIC, 14, NVTIPC_IOC_MEM_BUF)
#define NVTIOC_IPC_FTOK_C1         _IOWR(NVTIPC_IOC_MAGIC,15, NVTIPC_IOC_FTOK_S)
#define NVTIOC_IPC_FLUSH_CACHE     _IOW(NVTIPC_IOC_MAGIC, 16, NVTIPC_IOC_FLUSH_CACHE_S)
#define NVTIOC_IPC_SIG_WAIT        _IOW(NVTIPC_IOC_MAGIC, 17, NVTIOC_IPC_SIG_WAIT_S)
#define NVTIOC_IPC_SIG_ACK         _IOW(NVTIPC_IOC_MAGIC, 18, NVTIOC_IPC_SIG_ACK_S)




#endif
