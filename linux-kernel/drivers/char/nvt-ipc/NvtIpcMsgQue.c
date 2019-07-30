/**

    This file will handle the NvtIPC message queues operations,...etc

    @file       NvtIpcMsgQue.c
    Copyright   Novatek Microelectronics Corp. 2014.  All rights reserved.

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License version 2 as
    published by the Free Software Foundation.
*/
#include <asm/page.h>
#include <linux/mm.h>
#include <linux/signal.h>
#include <linux/syscalls.h>
#include <linux/spinlock.h>
#include <linux/time.h>
#include <linux/slab.h>
#include <linux/dma-mapping.h>
#include <mach/nvt_type.h>
#include <mach/nvt-ipc.h>
#include "nvt_ipc_debug.h"
#include "NvtIpcInt.h"
#include "NvtIpcMsgQue.h"

#define DBG_COLOR_RED "^R"
#define DBG_COLOR_BLUE "^B"
#define DBG_COLOR_YELLOW "^Y"



static NVTIPC_MSQINFO *g_pMsqInfo = NULL;
static DEFINE_SPINLOCK(nvtIpc_lock);

static UINT32 NvtIpc_GetSharedMemory(void)
{
	UINT32 Addr = NVTIPC_SHARED_MEM_ADDR;
	return Addr;
}

static UINT32 NvtIpc_CalcNeedbufSize(void)
{
	UINT32 bufsize;

	bufsize = NVTIPC_SYS_CMD_MAX_LEN + (NVTIPC_SENDTO_MAX * 2 * sizeof(NVTIPC_SEND_MSG));
#if __VOS_IPC__
	bufsize *= 2;
#endif
	return bufsize;

}

INT32 NvtIpcMsgQue_Init(void)
{
	UINT32        i, tmpBuf;
	NVTIPC_MSGQUE *pMsgQue;

	g_pMsqInfo = (NVTIPC_MSQINFO *)kmalloc(sizeof(NVTIPC_MSQINFO), GFP_KERNEL);
	if (!g_pMsqInfo) {
		DBG_ERR("alloc NvtIpcMsgQue fail\r\n");
		return -1;
	}
#if __IPC_SEND_BUF_CACHEABLE__
	g_pMsqInfo->SysCmdBuf = kmalloc(NvtIpc_CalcNeedbufSize(), GFP_KERNEL);
#else
	g_pMsqInfo->SysCmdBuf = dma_alloc_coherent(NULL, NvtIpc_CalcNeedbufSize(),
							(dma_addr_t *)&g_pMsqInfo->SysCmdDmaAddr, 0);
#endif
	DBG_IND("g_pMsqInfo->SysCmdBuf = 0x%x, SysCmdDmaAddr= 0x%x\r\n", (UINT32)g_pMsqInfo->SysCmdBuf, (UINT32)g_pMsqInfo->SysCmdDmaAddr);
	if (!g_pMsqInfo->SysCmdBuf) {
		DBG_ERR("alloc SysCmdBuf failed\r\n");
		kfree(g_pMsqInfo);
		return -1;
	}
	tmpBuf = (UINT32)g_pMsqInfo->SysCmdBuf + NVTIPC_SYS_CMD_MAX_LEN;
#if __VOS_IPC__
	g_pMsqInfo->SysCmdBuf_c1 = (char *)tmpBuf;
	tmpBuf += NVTIPC_SYS_CMD_MAX_LEN;
#endif
	for (i = 0; i < NVTIPC_MSG_QUEUE_NUM; i++) {
		pMsgQue = &g_pMsqInfo->MsgQueTbl[i];
		pMsgQue->uiInitKey = 0;
		// reset token string
		g_pMsqInfo->MsgQueTokenStr[i][0] = 0;
#if __VOS_IPC__
		pMsgQue = &g_pMsqInfo->MsgQueTbl_c1[i];
		pMsgQue->uiInitKey = 0;
		g_pMsqInfo->MsgQueTokenStr_c1[i][0] = 0;
#endif
	}
	g_pMsqInfo->ShmCtrl = (NVTIPC_SHM_CTRL *)ioremap_nocache(NvtIpc_GetSharedMemory(), sizeof(NVTIPC_SHM_CTRL));
	DBG_IND("g_pMsqInfo->SemCtrl = 0x%x\r\n", (UINT32)g_pMsqInfo->ShmCtrl);
	if (g_pMsqInfo->ShmCtrl->InterfaceVer != NVTIPC_INTERFACE_VER) {
		DBG_ERR("nvtipc version mismatch = 0x%x, 0x%x\r\n", g_pMsqInfo->ShmCtrl->InterfaceVer, NVTIPC_INTERFACE_VER);
		while (1)
			;
	}
	for (i = 0; i < NVTIPC_SEM_NUM; i++) {
		// create semaphore
		SEM_CREATE(g_pMsqInfo->CoresSemID[i], 0);
		SEM_CREATE(g_pMsqInfo->LocalSemID[i], 1);
	}
	for (i = 0; i < NVTIPC_SENDTO_MAX; i++) {
		SEM_CREATE(g_pMsqInfo->MsgBufTbl[i].semID, 1);
		g_pMsqInfo->MsgBufTbl[i].PingPonIdx = 0;
		g_pMsqInfo->MsgBufTbl[i].SendMsg[0] = (NVTIPC_SEND_MSG *)tmpBuf;
		tmpBuf += sizeof(NVTIPC_SEND_MSG);
		g_pMsqInfo->MsgBufTbl[i].SendMsg[1] = (NVTIPC_SEND_MSG *)tmpBuf;
		tmpBuf += sizeof(NVTIPC_SEND_MSG);
#if __VOS_IPC__
		SEM_CREATE(g_pMsqInfo->MsgBufTbl_c1[i].semID, 1);
		g_pMsqInfo->MsgBufTbl_c1[i].PingPonIdx = 0;
		g_pMsqInfo->MsgBufTbl_c1[i].SendMsg[0] = (NVTIPC_SEND_MSG *)tmpBuf;
		tmpBuf += sizeof(NVTIPC_SEND_MSG);
		g_pMsqInfo->MsgBufTbl_c1[i].SendMsg[1] = (NVTIPC_SEND_MSG *)tmpBuf;
		tmpBuf += sizeof(NVTIPC_SEND_MSG);
#endif
	}
	if (tmpBuf - (UINT32)g_pMsqInfo->SysCmdBuf != NvtIpc_CalcNeedbufSize()) {
		DBG_ERR("alloc %d, need %d\r\n", (tmpBuf - (UINT32)g_pMsqInfo->SysCmdBuf), NvtIpc_CalcNeedbufSize());
		return -1;
	}
	return 0;
}

void NvtIpcMsgQue_Exit(void)
{
	UINT32 i;
	NVTIPC_MSGQUE *pMsgQue;

	for (i = 0; i < NVTIPC_MSG_QUEUE_NUM; i++) {
		pMsgQue = &g_pMsqInfo->MsgQueTbl[i];
		pMsgQue->uiInitKey = 0;
#if __VOS_IPC__
		pMsgQue = &g_pMsqInfo->MsgQueTbl_c1[i];
		pMsgQue->uiInitKey = 0;
#endif
		// delete semaphore
		//SEM_DESTROY(pMsgQue->semID);
	}
	for (i = 0; i < NVTIPC_SEM_NUM; i++) {
		// delete semaphore
		SEM_DESTROY(g_pMsqInfo->CoresSemID[i]);
		SEM_DESTROY(g_pMsqInfo->LocalSemID[i]);
	}
	for (i = 0; i < NVTIPC_SENDTO_MAX; i++) {
		SEM_DESTROY(g_pMsqInfo->MsgBufTbl[i].semID)
#if __VOS_IPC__
		SEM_DESTROY(g_pMsqInfo->MsgBufTbl_c1[i].semID)
#endif
	}
	iounmap(g_pMsqInfo->ShmCtrl);
#if __IPC_SEND_BUF_CACHEABLE__
	kfree(g_pMsqInfo->SysCmdBuf);
#else
	dma_free_coherent(NULL, NVTIPC_SYS_CMD_MAX_LEN + (NVTIPC_SENDTO_MAX * 2 * sizeof(NVTIPC_SEND_MSG)), g_pMsqInfo->SysCmdBuf, (dma_addr_t)g_pMsqInfo->SysCmdDmaAddr);
#endif
	kfree(g_pMsqInfo);
}

static UINT16 NvtIpcMsgQue_NameHash(const char *pName)
{
	UINT16 Hash = 0;
	const char *pChar = pName;
	while ((*pChar) != 0) {
		Hash = ((Hash & 1) ? 0x8000 : 0) + (Hash >> 1) + (*pChar);
		pChar++;
	}
	DBG_IND("pName=%s, Hash=0x%x\r\n", pName, Hash);
	return Hash;
}

UINT16 NvtIpcMsgQue_FtokEx(NVTIPC_U32 coreId, const char *path)
{
	UINT32 i;
	unsigned long flags;
	char *queuetoken;
	spin_lock_irqsave(&nvtIpc_lock, flags);
	for (i = 0; i < NVTIPC_MSG_QUEUE_NUM; i++) {
#if __VOS_IPC__
		if (coreId == __CORE2) {
			queuetoken =  g_pMsqInfo->MsgQueTokenStr[i];
		} else {
			queuetoken =  g_pMsqInfo->MsgQueTokenStr_c1[i];
		}
#else
		queuetoken =  g_pMsqInfo->MsgQueTokenStr[i];
#endif
		if (queuetoken[0] == 0) {
			strncpy(queuetoken, path, NVTIPC_MSG_QUEUE_TOKEN_STR_MAXLEN);
			queuetoken[NVTIPC_MSG_QUEUE_TOKEN_STR_MAXLEN] = 0;
			DBG_IND("MsgQueTokenStr[%d]=%s\r\n", i, queuetoken[i]);
			break;
		}
	}
	spin_unlock_irqrestore(&nvtIpc_lock, flags);
	return NvtIpcMsgQue_NameHash(path);
}



char *NvtIpcMsgQue_GetTokenByKey(NVTIPC_U32 coreId, UINT32 key)
{
	UINT32 i;
	for (i = 0; i < NVTIPC_MSG_QUEUE_NUM; i++) {
#if __VOS_IPC__
		if ((coreId == __CORE2) && NvtIpcMsgQue_NameHash(g_pMsqInfo->MsgQueTokenStr[i]) == key) {
			return g_pMsqInfo->MsgQueTokenStr[i];
		} else if ((coreId == __CORE1) && NvtIpcMsgQue_NameHash(g_pMsqInfo->MsgQueTokenStr_c1[i]) == key) {
			return g_pMsqInfo->MsgQueTokenStr_c1[i];
		}
#else
		if (NvtIpcMsgQue_NameHash(g_pMsqInfo->MsgQueTokenStr[i]) == key) {
			return g_pMsqInfo->MsgQueTokenStr[i];
		}
#endif
	}
	return NULL;
}

static void NvtIpcMsgQue_RelToken(NVTIPC_U32 coreId, UINT32 key)
{
	char *token;
	token = NvtIpcMsgQue_GetTokenByKey(coreId, key);
	if (token) {
		DBG_IND("RelToken %s\r\n", token);
		token[0] = 0;
	}
}



NVTIPC_SEND_MSG *NvtIpcMsgQue_LockSndMsgBufEx(NVTIPC_U32 coreId, NVTIPC_SENDTO sendTo)
{
	NVTIPC_MSGBUF    *pMsgBuf;

#if __VOS_IPC__
	if (coreId == __CORE1) {
		pMsgBuf = &g_pMsqInfo->MsgBufTbl_c1[sendTo - 1];
	} else {
		pMsgBuf = &g_pMsqInfo->MsgBufTbl[sendTo - 1];
	}
#else
	pMsgBuf = &g_pMsqInfo->MsgBufTbl[sendTo - 1];
#endif
	DBG_IND("pMsgBuf =0x%x\r\n", (UINT32)pMsgBuf);
	SEM_WAIT(pMsgBuf->semID);
	if (pMsgBuf->PingPonIdx == 0) {
		pMsgBuf->PingPonIdx = 1;
		return pMsgBuf->SendMsg[0];
	} else {
		pMsgBuf->PingPonIdx = 0;
		return pMsgBuf->SendMsg[1];
	}
}

void NvtIpcMsgQue_UnlockSndMsgBufEx(NVTIPC_U32 coreId, NVTIPC_SENDTO sendTo)
{
	NVTIPC_MSGBUF    *pMsgBuf;

#if __VOS_IPC__
	if (coreId == __CORE1) {
		pMsgBuf = &g_pMsqInfo->MsgBufTbl_c1[sendTo - 1];
	} else {
		pMsgBuf = &g_pMsqInfo->MsgBufTbl[sendTo - 1];
	}
#else
	pMsgBuf = &g_pMsqInfo->MsgBufTbl[sendTo - 1];
#endif
	SEM_SIGNAL(pMsgBuf->semID);
}

char *NvtIpcMsgQue_GetSysCmdBuf(void)
{
	if (unlikely(!g_pMsqInfo)) {
		DBG_ERR("%d\r\n", NVTIPC_ER_NOT_OPENED);
		return NULL;
	}
	return g_pMsqInfo->SysCmdBuf;
}


NVTIPC_SHM_CTRL *NvtIpcMsgQue_GetShmCtrl(void)
{
	if (unlikely(!g_pMsqInfo)) {
		DBG_ERR("%d\r\n", NVTIPC_ER_NOT_OPENED);
		return NULL;
	}
	return g_pMsqInfo->ShmCtrl;
}


void NvtIpcMsgQue_SemCtrlLockCores(UINT32 semid)
{
	if (unlikely(!g_pMsqInfo)) {
		DBG_ERR("%d\r\n", NVTIPC_ER_NOT_OPENED);
		return;
	}
	if (SEM_WAIT(g_pMsqInfo->CoresSemID[semid])) {
		return;
	}
}

void NvtIpcMsgQue_SemCtrlUnlockCores(UINT32 semid)
{
	if (unlikely(!g_pMsqInfo)) {
		DBG_ERR("%d\r\n", NVTIPC_ER_NOT_OPENED);
		return;
	}
	SEM_SIGNAL(g_pMsqInfo->CoresSemID[semid]);
}

void NvtIpcMsgQue_SemCtrlLockLocal(UINT32 semid)
{
	if (unlikely(!g_pMsqInfo)) {
		DBG_ERR("%d\r\n", NVTIPC_ER_NOT_OPENED);
		return;
	}
	if (SEM_WAIT(g_pMsqInfo->LocalSemID[semid])) {
		return;
	}
}

void NvtIpcMsgQue_SemCtrlUnlockLocal(UINT32 semid)
{
	if (unlikely(!g_pMsqInfo)) {
		DBG_ERR("%d\r\n", NVTIPC_ER_NOT_OPENED);
		return;
	}
	SEM_SIGNAL(g_pMsqInfo->LocalSemID[semid]);
}

INT32 NvtIpcMsgQue_GetIDbyKeyEx(NVTIPC_U32 coreId, INT32 key)
{
	UINT32 i;
	NVTIPC_MSGQUE *pMsgQue = 0;
	for (i = 0; i < NVTIPC_MSG_QUEUE_NUM; i++) {
#if __VOS_IPC__
		if (coreId == __CORE1) {
			pMsgQue = &g_pMsqInfo->MsgQueTbl_c1[i];
		} else {
			pMsgQue = &g_pMsqInfo->MsgQueTbl[i];
		}
#else
		pMsgQue = &g_pMsqInfo->MsgQueTbl[i];
#endif
		if (pMsgQue->uiInitKey == CFG_NVTIPC_INIT_KEY && (UINT32)key == pMsgQue->uiSharedKey) {
			return pMsgQue->msqID;
		}
	}
	return -1;
}

NVTIPC_MSGQUE *NvtIpcMsgQue_GetQuebyIDEx(NVTIPC_U32 coreId, UINT32 msqid)
{
	NVTIPC_MSGQUE *pMsgQue;

	if (unlikely(msqid >= NVTIPC_MSG_QUEUE_NUM)) {
		DBG_ERR("msqid %d is invalid\r\n", msqid);
		return 0;
	}
#if __VOS_IPC__
	if (coreId == __CORE1) {
		pMsgQue = &g_pMsqInfo->MsgQueTbl_c1[msqid];
	} else {
		pMsgQue = &g_pMsqInfo->MsgQueTbl[msqid];
	}
#else
	pMsgQue = &g_pMsqInfo->MsgQueTbl[msqid];
#endif
	if (unlikely(pMsgQue->uiInitKey != CFG_NVTIPC_INIT_KEY)) {
		DBG_ERR("msqid %d is not initial\r\n", msqid);
		return 0;
	}
	return pMsgQue;
}



INT32 NvtIpcMsgQue_GetEx(NVTIPC_U32 coreId, INT32 key)
{
	UINT32         i;
	NVTIPC_MSGQUE *pMsgQue = NULL;
	unsigned long  flags;

	DBG_IND("key =0x%x\r\n", key);
	// key is only UINT16
	if (unlikely(key > 0xFFFF)) {
		DBG_ERR("key 0x%x is invalid\r\n", key);
		return NVTIPC_ER_PARM;
	}
	spin_lock_irqsave(&nvtIpc_lock, flags);
	for (i = 0; i < NVTIPC_MSG_QUEUE_NUM; i++) {
#if __VOS_IPC__
		if (coreId == __CORE1) {
			pMsgQue = &g_pMsqInfo->MsgQueTbl_c1[i];
		} else {
			pMsgQue = &g_pMsqInfo->MsgQueTbl[i];
		}
#else
		pMsgQue = &g_pMsqInfo->MsgQueTbl[i];
#endif
		if (pMsgQue->uiInitKey == CFG_NVTIPC_INIT_KEY && (UINT32)key == pMsgQue->uiSharedKey) {
			spin_unlock_irqrestore(&nvtIpc_lock, flags);
			DBG_ERR("key 0x%x is duplicate\r\n", key);
			return NVTIPC_ER_PARM;
		}
	}
	for (i = 0; i < NVTIPC_MSG_QUEUE_NUM; i++) {
#if __VOS_IPC__
		if (coreId == __CORE1) {
			pMsgQue = &g_pMsqInfo->MsgQueTbl_c1[i];
		} else {
			pMsgQue = &g_pMsqInfo->MsgQueTbl[i];
		}
#else
		pMsgQue = &g_pMsqInfo->MsgQueTbl[i];
#endif
		if (pMsgQue->uiInitKey != CFG_NVTIPC_INIT_KEY) {
			break;
		}
	}
	if (unlikely(i >= NVTIPC_MSG_QUEUE_NUM)) {
		spin_unlock_irqrestore(&nvtIpc_lock, flags);
		DBG_ERR("all %d queues are used\r\n", NVTIPC_MSG_QUEUE_NUM);
		return NVTIPC_ER_NO_MORE_QUEUE;
	}
	// ---- set initial value ----

	pMsgQue->uiInitKey = CFG_NVTIPC_INIT_KEY;
	// msg queue id
	pMsgQue->msqID = i;
	pMsgQue->uiInIdx = 0;
	pMsgQue->uiOutIdx = 0;
	pMsgQue->uiSharedKey = key;
#if __IPC_PROF_MEASURE__
	pMsgQue->uiProfStartTime = 0;
	pMsgQue->uiProfMaxMsgCount = 0;
	pMsgQue->uiProfMsgCount = 0;
#endif
	spin_unlock_irqrestore(&nvtIpc_lock, flags);
	DBG_IND("msqID %d, uiSharedKey %d\r\n", pMsgQue->msqID, pMsgQue->uiSharedKey);
	return pMsgQue->msqID;
}
INT32 NvtIpcMsgQue_RelEx(NVTIPC_U32 coreId, UINT32 msqid)
{
	NVTIPC_MSGQUE *pMsgQue;
	unsigned long flags;

	spin_lock_irqsave(&nvtIpc_lock, flags);
	pMsgQue = NvtIpcMsgQue_GetQuebyIDEx(coreId, msqid);
	if (unlikely(!pMsgQue)) {
		spin_unlock_irqrestore(&nvtIpc_lock, flags);
		DBG_ERR("msqid %d is invalid\r\n", msqid);
		return NVTIPC_ER_PARM;
	}
	NvtIpcMsgQue_RelToken(coreId, pMsgQue->uiSharedKey);
	pMsgQue->uiInitKey = 0;
	spin_unlock_irqrestore(&nvtIpc_lock, flags);
	return NVTIPC_ER_OK;
}

static void NvtIpcMsgQue_ProfAddMsgCount(NVTIPC_MSGQUE *pMsgQue)
{
	UINT32   CurrTime;
	struct   timeval time;

	do_gettimeofday(&time);
	CurrTime = (UINT32)(time.tv_sec);

	DBG_IND("CurrTime 0x%x \r\n", CurrTime);
	// reset start time
	if (CurrTime >= pMsgQue->uiProfStartTime + 8) {
		pMsgQue->uiProfStartTime = CurrTime & 0xFFFFFFF8;
		if (pMsgQue->uiProfMsgCount > pMsgQue->uiProfMaxMsgCount) {
			pMsgQue->uiProfMaxMsgCount = pMsgQue->uiProfMsgCount;
		}
		pMsgQue->uiProfMsgCount = 0;
		DBG_IND("uiProfStartTime 0x%x , uiProfMaxMsgCount = %d\r\n", pMsgQue->uiProfStartTime, pMsgQue->uiProfMaxMsgCount);
	} else {
		pMsgQue->uiProfMsgCount++;
		DBG_IND("uiProfMsgCount = %d \r\n", pMsgQue->uiProfMsgCount);
	}
}

// This API will be used in ISR
static INT32 NvtIpcMsgQue_EnQueue(NVTIPC_MSGQUE *pMsgQue, NVTIPC_MSG *p_msg)
{
	UINT32     InNextIdx;
	NVTIPC_MSG *pTmpmsg;

	DBG_IND("uiInIdx =%d\r\n", pMsgQue->uiInIdx);
	InNextIdx = pMsgQue->uiInIdx + 1;
	if (unlikely(InNextIdx >= NVTIPC_MSG_ELEMENT_NUM)) {
		InNextIdx -= NVTIPC_MSG_ELEMENT_NUM;
	}
	if (unlikely(InNextIdx == pMsgQue->uiOutIdx)) {
		return NVTIPC_ER_MSGQUE_FULL;
	}
	pTmpmsg = &pMsgQue->element[pMsgQue->uiInIdx];
	*pTmpmsg = *p_msg;
	pMsgQue->uiInIdx = InNextIdx;
	return NVTIPC_ER_OK;

}

void NvtIpcMsgQue_DeQueue(NVTIPC_MSGQUE *pMsgQue, NVTIPC_MSG *p_msg)
{
	unsigned long flags;

	DBG_IND("uiOutIdx =%d\r\n", pMsgQue->uiOutIdx);
	spin_lock_irqsave(&nvtIpc_lock, flags);
	*p_msg = pMsgQue->element[pMsgQue->uiOutIdx];
	pMsgQue->uiOutIdx++;
	if (unlikely(pMsgQue->uiOutIdx >= NVTIPC_MSG_ELEMENT_NUM)) {
		pMsgQue->uiOutIdx -= NVTIPC_MSG_ELEMENT_NUM;
	}
	NvtIpcMsgQue_ProfAddMsgCount(pMsgQue);
	spin_unlock_irqrestore(&nvtIpc_lock, flags);
}

UINT32 NvtIpcMsgQue_isEmpty(NVTIPC_MSGQUE *pMsgQue)
{
	if (pMsgQue->uiInIdx == pMsgQue->uiOutIdx) {
		return 1;
	} else {
		return 0;
	}
}

static UINT32 NvtIpcMsgQue_getElementCount(NVTIPC_MSGQUE *pMsgQue)
{
	UINT32 tempInIdx;
	if (pMsgQue->uiInIdx == pMsgQue->uiOutIdx) {
		return 0;
	}
	tempInIdx = pMsgQue->uiInIdx;
	if (tempInIdx < pMsgQue->uiOutIdx) {
		tempInIdx += NVTIPC_MSG_ELEMENT_NUM;
	}
	return tempInIdx - pMsgQue->uiOutIdx;
}

UINT32 NvtIpcMsgQue_isValid(NVTIPC_MSGQUE *pMsgQue, UINT32 sharedKey)
{
	if (likely((pMsgQue->uiInitKey == CFG_NVTIPC_INIT_KEY) && (sharedKey == pMsgQue->uiSharedKey))) {
		return 1;
	} else {
		return 0;
	}
}




// This API will be used in ISR
INT32 NvtIPC_MsgPostEx(NVTIPC_U32 coreId, UINT32 msqid, void  *pMsg, UINT32 msgsz)
{
	NVTIPC_MSG     sndMsg;
	NVTIPC_MSGQUE  *pMsgQue;
	ER             ret;
#if __VOS_IPC__
	unsigned long  flags;
#endif

	DBG_IND("msqid = %d, pMsg=0x%x, msgsz=0x%x\r\n", msqid, (UINT32)pMsg, msgsz);
	if (unlikely(!pMsg)) {
		//DBG_ERR("pMsg is NULL\r\n");
		return NVTIPC_ER_PARM;
	}
	if (unlikely(msgsz > NVTIPC_MSG_ELEMENT_SIZE)) {
		//DBG_ERR("msgsz =%d > limit %d\r\n",msgsz,NVTIPC_MSG_ELEMENT_SIZE);
		return NVTIPC_ER_SNDSIZE_OVER_LIMIT;
	}
#if __VOS_IPC__
	spin_lock_irqsave(&nvtIpc_lock, flags);
#endif
	pMsgQue = NvtIpcMsgQue_GetQuebyIDEx(coreId, msqid);
	if (unlikely(!pMsgQue)) {
#if __VOS_IPC__
		spin_unlock_irqrestore(&nvtIpc_lock, flags);
#endif
		//DBG_ERR("pMsgQue\r\n");
		return NVTIPC_ER_MSGQUE_ID;
	}
	memcpy(&sndMsg.data, pMsg, msgsz);
	sndMsg.size = msgsz;
	ret = NvtIpcMsgQue_EnQueue(pMsgQue, &sndMsg);
	if (unlikely(ret != NVTIPC_ER_OK)) {
#if __VOS_IPC__
		spin_unlock_irqrestore(&nvtIpc_lock, flags);
#endif
		//DBG_ERR("Queue full, msqid=%d\r\n",msqid);
		return NVTIPC_ER_MSGQUE_FULL;
	}
#if __VOS_IPC__
	spin_unlock_irqrestore(&nvtIpc_lock, flags);
#endif
	return NVTIPC_ER_OK;
}

void NvtIpcMsgQue_DumpDebugInfo(void)
{
	UINT32 i;
	NVTIPC_MSGQUE *pMsgQue;
	char          *token;
#if __VOS_IPC__
	NVTIPC_SHM_CTRL *pShmCtrl = NvtIpcMsgQue_GetShmCtrl();
#endif

	if (unlikely(!g_pMsqInfo)) {
		DBG_ERR("%d\r\n", NVTIPC_ER_NOT_OPENED);
		return;
	}
#if __VOS_IPC__
	DBG_DUMP("[CORE1]----------------IPC info begin----------------\r\n");
	if (pShmCtrl) {
		UINT32 i;
		DBG_DUMP("[CORE1]----\t InterfaceVer =0x%x\r\n", pShmCtrl->InterfaceVer);
		DBG_DUMP("[CORE1]----\t SemBits =0x%x, cpu1AckBits=0x%x, cpu2AckBits=0x%x\r\n", pShmCtrl->SemBits, pShmCtrl->Cpu1AckBits, pShmCtrl->Cpu2AckBits);
		for (i = 0; i < NVTIPC_SEM_NUM; i++) {
			DBG_DUMP("[CORE1]----\t Cpu1SemWaitCnt[%d]=%d, Cpu2SemWaitCnt[%d]=%d\r\n", i, pShmCtrl->Cpu1SemWaitCnt[i], i, pShmCtrl->Cpu2SemWaitCnt[i]);
		}
		DBG_DUMP("[CORE1]----\t Cpu1SemLockTime=%d, Cpu1SemUnLockTime=%d\r\n", pShmCtrl->Cpu1SemLockTime, pShmCtrl->Cpu1SemUnLockTime);
		DBG_DUMP("[CORE1]----\t Cpu2SemLockTime=%d, Cpu2SemUnLockTime=%d\r\n", pShmCtrl->Cpu2SemLockTime, pShmCtrl->Cpu2SemUnLockTime);
		DBG_DUMP("[CORE1]----\t Cpu1Cpu2Err=%d, Cpu1Cpu2Key=0x%x\r\n", pShmCtrl->Cpu1Cpu2Err, pShmCtrl->Cpu1Cpu2Key);
		DBG_DUMP("[CORE1]----\t Cpu1Cpu3Err=%d, Cpu1Cpu3Key=0x%x\r\n", pShmCtrl->Cpu1Cpu3Err, pShmCtrl->Cpu1Cpu3Key);
		DBG_DUMP("[CORE1]----\t Cpu1Cpu4Err=%d, Cpu1Cpu4Key=0x%x\r\n", pShmCtrl->Cpu1Cpu4Err, pShmCtrl->Cpu1Cpu4Key);
		DBG_DUMP("[CORE1]----\t Cpu2Cpu1Err=%d, Cpu2Cpu1Key=0x%x\r\n", pShmCtrl->Cpu2Cpu1Err, pShmCtrl->Cpu2Cpu1Key);
		DBG_DUMP("[CORE1]----\t Cpu2Cpu3Err=%d, Cpu2Cpu3Key=0x%x\r\n", pShmCtrl->Cpu2Cpu3Err, pShmCtrl->Cpu2Cpu3Key);
		DBG_DUMP("[CORE1]----\t Cpu2Cpu4Err=%d, Cpu2Cpu4Key=0x%x\r\n", pShmCtrl->Cpu2Cpu4Err, pShmCtrl->Cpu2Cpu4Key);
		DBG_DUMP("[CORE1]----\t Cpu3Cpu1Err=%d, Cpu3Cpu1Key=0x%x\r\n", pShmCtrl->Cpu3Cpu1Err, pShmCtrl->Cpu3Cpu1Key);
		DBG_DUMP("[CORE1]----\t Cpu3Cpu2Err=%d, Cpu3Cpu2Key=0x%x\r\n", pShmCtrl->Cpu3Cpu2Err, pShmCtrl->Cpu3Cpu2Key);
		DBG_DUMP("[CORE1]----\t Cpu3Cpu4Err=%d, Cpu3Cpu4Key=0x%x\r\n", pShmCtrl->Cpu3Cpu4Err, pShmCtrl->Cpu3Cpu4Key);
		DBG_DUMP("[CORE1]----\t Cpu4Cpu1Err=%d, Cpu4Cpu1Key=0x%x\r\n", pShmCtrl->Cpu4Cpu1Err, pShmCtrl->Cpu4Cpu1Key);
		DBG_DUMP("[CORE1]----\t Cpu4Cpu2Err=%d, Cpu4Cpu2Key=0x%x\r\n", pShmCtrl->Cpu4Cpu2Err, pShmCtrl->Cpu4Cpu2Key);
		DBG_DUMP("[CORE1]----\t Cpu4Cpu3Err=%d, Cpu4Cpu3Key=0x%x\r\n", pShmCtrl->Cpu4Cpu3Err, pShmCtrl->Cpu4Cpu3Key);

	}
	DBG_DUMP("[CORE1]----------------IPC info begin----------------\r\n");
	DBG_DUMP("[CORE1]----\tIPC Core%d\r\n", __CORE1);
	DBG_DUMP("[CORE1]----\tIPC MsqID =%d, key=0x%x\r\n", NVTIPC_SYS_QUEUE_ID, NVTIPC_SYS_KEY);
	for (i = 1; i < NVTIPC_MSG_QUEUE_NUM; i++) {
		pMsgQue = &g_pMsqInfo->MsgQueTbl_c1[i];;
		if (pMsgQue->uiInitKey == CFG_NVTIPC_INIT_KEY) {
			token = NvtIpcMsgQue_GetTokenByKey(__CORE1, pMsgQue->uiSharedKey);
			if (token) {
				DBG_DUMP("[CORE1]----\tIPC MsqID =%d, key=0x%x, token=%s ,ProfMaxMsgCount=%d(/8s), elementCount=%d\r\n", pMsgQue->msqID, pMsgQue->uiSharedKey, token, pMsgQue->uiProfMaxMsgCount, NvtIpcMsgQue_getElementCount(pMsgQue));
			} else {
				DBG_DUMP("[CORE1]----\tIPC MsqID =%d, key=0x%x, ProfMaxMsgCount=%d(/8s), elementCount=%d\r\n", pMsgQue->msqID, pMsgQue->uiSharedKey, pMsgQue->uiProfMaxMsgCount, NvtIpcMsgQue_getElementCount(pMsgQue));
			}
		}
	}
	DBG_DUMP("[CORE1]----------------IPC info end----------------\r\n");
#endif
	//DBG_DUMP("[CORE%d]----------------IPC info begin----------------\r\n",__CORE);
	//DBG_DUMP("[CORE%d]----\tIPC Core%d\r\n",__CORE,__CORE);
	//DBG_DUMP("[CORE%d]----\tIPC MsqID =%d, key=0x%x\r\n",__CORE,NVTIPC_SYS_QUEUE_ID, NVTIPC_SYS_KEY);
	DBG_DUMP("\r\n-----NvtIPC CORE%d MsgQue INFO-------------------------------------------------------------\r\n", __CORE);
	for (i = 0; i < NVTIPC_MSG_QUEUE_NUM; i++) {
		pMsgQue = &g_pMsqInfo->MsgQueTbl[i];
		if (pMsgQue->uiInitKey == CFG_NVTIPC_INIT_KEY) {
			token = NvtIpcMsgQue_GetTokenByKey(__CORE, pMsgQue->uiSharedKey);
			if (token) {
				DBG_DUMP("[CORE%d] MsqID =%d, key=0x%x, token=%s ,ProfMaxMsgCount=%d(/8s), elementCount=%d\r\n", __CORE, pMsgQue->msqID, pMsgQue->uiSharedKey, token, pMsgQue->uiProfMaxMsgCount, NvtIpcMsgQue_getElementCount(pMsgQue));
			} else {
				DBG_DUMP("[CORE%d] MsqID =%d, key=0x%x, ProfMaxMsgCount=%d(/8s), elementCount=%d\r\n", __CORE, pMsgQue->msqID, pMsgQue->uiSharedKey, pMsgQue->uiProfMaxMsgCount, NvtIpcMsgQue_getElementCount(pMsgQue));
			}
		}
	}
}

UINT32 NvtIpcMsgQue_getPhyAddr(UINT32 virtual_addr)
{
	return virtual_addr - (g_pMsqInfo->SysCmdBuf - g_pMsqInfo->SysCmdDmaAddr);
}

void NvtIpcMsgQue_chkAllReleased(void)
{
	char  *token;
	int    i;
	NVTIPC_MSGQUE  *pMsgQue;

	pMsgQue = NvtIpcMsgQue_GetQuebyID(NVTIPC_SYS_QUEUE_ID);
	if (!NvtIpcMsgQue_isEmpty(pMsgQue))
		DBG_WRN("NvtIpcMsgQue %d, is not emtpy\r\n", NVTIPC_SYS_QUEUE_ID);
	pMsgQue = NvtIpcMsgQue_GetQuebyID(NVTIPC_SEM_QUEUE_ID);
	if (!NvtIpcMsgQue_isEmpty(pMsgQue))
		DBG_WRN("NvtIpcMsgQue %d, is not emtpy\r\n", NVTIPC_SEM_QUEUE_ID);
    // ignore system queue 0 and semaphore queue 1
	for (i = 2; i < NVTIPC_MSG_QUEUE_NUM ; i++) {
		pMsgQue = &g_pMsqInfo->MsgQueTbl[i];
		if (pMsgQue->uiInitKey == 0)
			continue;
		token = NvtIpcMsgQue_GetTokenByKey(__CORE2, pMsgQue->uiSharedKey);
		if (token) {
			DBG_WRN("NvtIpcMsgQue %d, token=%s is not released\r\n", i, token);
		} else {
			DBG_WRN("NvtIpcMsgQue %d is not released \r\n", i);
		}
	}
}
