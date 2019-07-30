/**

    The header file of NvtIPC message queues operations,...etc

    @file       NvtIpcMsgQue.h
    Copyright   Novatek Microelectronics Corp. 2014.  All rights reserved.

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License version 2 as
    published by the Free Software Foundation.
*/
#ifndef NVTIPC_MSGQUE_H
#define NVTIPC_MSGQUE_H
#include <mach/nvt_type.h>
#include <mach/nvt-ipc.h>



typedef struct _NVTIPC_MSGQUE {
	UINT32            uiInitKey;
	UINT32            uiSharedKey;
	UINT32            msqID;      // msg queue id
	//SEM_HANDLE        semID;      // semaphore id
	UINT32            uiInIdx;
	UINT32            uiOutIdx;
	NVTIPC_MSG        element[NVTIPC_MSG_ELEMENT_NUM];
	//NVTIPC_SEND_MSG*  SendMsg;
#if __IPC_PROF_MEASURE__
	UINT32            uiProfStartTime;
	UINT32            uiProfMsgCount;
	UINT32            uiProfMaxMsgCount;
#endif
} NVTIPC_MSGQUE;

typedef struct _NVTIPC_MSGBUF {
	UINT32            PingPonIdx;
	NVTIPC_SEND_MSG  *SendMsg[2];
	SEM_HANDLE        semID;      // semaphore id
} NVTIPC_MSGBUF;





typedef struct _NVTIPC_MSQINFO {
	char              *SysCmdBuf;
	char              *SysCmdDmaAddr;
	NVTIPC_MSGQUE     MsgQueTbl[NVTIPC_MSG_QUEUE_NUM];
	char              MsgQueTokenStr[NVTIPC_MSG_QUEUE_NUM][NVTIPC_MSG_QUEUE_TOKEN_STR_MAXLEN + 1];
	NVTIPC_SHM_CTRL   *ShmCtrl;
	SEM_HANDLE        CoresSemID[NVTIPC_SEM_NUM];      // semaphore id
	SEM_HANDLE        LocalSemID[NVTIPC_SEM_NUM];      // semaphore id
	NVTIPC_MSGBUF     MsgBufTbl[NVTIPC_SENDTO_MAX];
#if __VOS_IPC__
	char              *SysCmdBuf_c1;
	NVTIPC_MSGQUE     MsgQueTbl_c1[NVTIPC_MSG_QUEUE_NUM];
	char              MsgQueTokenStr_c1[NVTIPC_MSG_QUEUE_NUM][NVTIPC_MSG_QUEUE_TOKEN_STR_MAXLEN + 1];
	NVTIPC_MSGBUF     MsgBufTbl_c1[NVTIPC_SENDTO_MAX];
#endif
} NVTIPC_MSQINFO;


typedef struct _NVTIPC_CPU2_READY_MSG {
	NVTIPC_SYS_CMD   sysCmdID;           ///< system command ID.
} NVTIPC_CPU2_READY_MSG;

typedef struct _NVTIPC_SEM_INIT_MSG {
	NVTIPC_SEM_CMD   CmdID;              ///< command ID.
	UINT32           semCtrl;            ///< semaphore control pointer
} NVTIPC_SEM_INIT_MSG;



extern INT32          NvtIpcMsgQue_Init(void);
extern void           NvtIpcMsgQue_Exit(void);
extern NVTIPC_SEND_MSG *NvtIpcMsgQue_LockSndMsgBufEx(NVTIPC_U32 coreId, NVTIPC_SENDTO sendTo);
extern void             NvtIpcMsgQue_UnlockSndMsgBufEx(NVTIPC_U32 coreId, NVTIPC_SENDTO sendTo);
extern char          *NvtIpcMsgQue_GetSysCmdBuf(void);
extern NVTIPC_SHM_CTRL *NvtIpcMsgQue_GetShmCtrl(void);
extern void           NvtIpcMsgQue_SemCtrlLockCores(UINT32 semid);
extern void           NvtIpcMsgQue_SemCtrlUnlockCores(UINT32 semid);
extern void           NvtIpcMsgQue_SemCtrlLockLocal(UINT32 semid);
extern void           NvtIpcMsgQue_SemCtrlUnlockLocal(UINT32 semid);
extern NVTIPC_MSGQUE *NvtIpcMsgQue_GetQuebyIDEx(NVTIPC_U32 coreId, UINT32 msqid);
extern INT32          NvtIpcMsgQue_GetIDbyKeyEx(NVTIPC_U32 coreId, INT32 key);
extern INT32          NvtIpcMsgQue_GetEx(NVTIPC_U32 coreId, INT32 key);
extern INT32          NvtIpcMsgQue_RelEx(NVTIPC_U32 coreId, UINT32 msqid);
extern void           NvtIpcMsgQue_DeQueue(NVTIPC_MSGQUE *pMsgQue, NVTIPC_MSG *p_msg);
extern UINT32         NvtIpcMsgQue_isEmpty(NVTIPC_MSGQUE *pMsgQue);
extern UINT32         NvtIpcMsgQue_isValid(NVTIPC_MSGQUE *pMsgQue, UINT32 sharedKey);
extern INT32          NvtIPC_MsgPostEx(NVTIPC_U32 coreId, UINT32 msqid, void  *pMsg, UINT32 msgsz);
extern UINT16         NvtIpcMsgQue_FtokEx(NVTIPC_U32 coreId, const char *pName);
extern char          *NvtIpcMsgQue_GetTokenByKey(NVTIPC_U32 coreId, UINT32 key);
extern void           NvtIpcMsgQue_DumpDebugInfo(void);
extern UINT32         NvtIpcMsgQue_getPhyAddr(UINT32 virtual_addr);
extern void           NvtIpcMsgQue_chkAllReleased(void);

#define NvtIpcMsgQue_Ftok(p1)       NvtIpcMsgQue_FtokEx(__CORE2, p1)
#define NvtIpcMsgQue_Get(p1)        NvtIpcMsgQue_GetEx(__CORE2, p1)
#define NvtIpcMsgQue_Rel(p1)        NvtIpcMsgQue_RelEx(__CORE2, p1)
#define NvtIpcMsgQue_GetQuebyID(p1) NvtIpcMsgQue_GetQuebyIDEx(__CORE2, p1)
#define NvtIpcMsgQue_GetIDbyKey(p1) NvtIpcMsgQue_GetIDbyKeyEx(__CORE2, p1)
#define NvtIPC_MsgPost(p1,p2,p3)    NvtIPC_MsgPostEx(__CORE2, p1,p2,p3)
#define NvtIpcMsgQue_LockSndMsgBuf(p1) NvtIpcMsgQue_LockSndMsgBufEx(__CORE2, p1)
#define NvtIpcMsgQue_UnlockSndMsgBuf(p1) NvtIpcMsgQue_UnlockSndMsgBufEx(__CORE2, p1)
#endif
