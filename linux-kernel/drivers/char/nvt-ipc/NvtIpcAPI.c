/**

    This file handles the main exported APIs of nvt_ipc driver.

    @file       NvtIpcAPI.c
    Copyright   Novatek Microelectronics Corp. 2014.  All rights reserved.

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License version 2 as
    published by the Free Software Foundation.
*/
#include <linux/kernel.h>
#include <linux/vmalloc.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/mm.h>
#include <linux/device.h>
#include <linux/slab.h>
#include <linux/bootmem.h>
#include <linux/sched.h>
#include <linux/interrupt.h>
#include <linux/syscalls.h>
#include <linux/signal.h>
#include <linux/kthread.h>
#include <linux/hrtimer.h>
#include <linux/spinlock.h>
#include <linux/platform_device.h>
#include <linux/delay.h>
#include <asm/page.h>
#include <asm/cacheflush.h>
#include "mach/cc.h"
#include <mach/nvt-ipc.h>
#include <mach/hardware.h>
#include "nvt_ipc_debug.h"
#include "NvtIpcInt.h"
#include "NvtIpcMsgQue.h"


static  DECLARE_COMPLETION(sys_cmd_completion);
//static  DECLARE_COMPLETION(uart_completion);

#if __COREIPC__
static  DEFINE_SPINLOCK(my_lock);
//#define loc_cpu()           spin_lock(&my_lock)
//#define unl_cpu()           spin_unlock(&my_lock)
static unsigned long        myflags;
#define loc_cpu()           spin_lock_irqsave(&my_lock, myflags)
#define unl_cpu()           spin_unlock_irqrestore(&my_lock, myflags)
#define loc_multiCores()    {loc_cpu();cc_hwlock_resource(CC_CORE_CA53_CORE2,CC_RESOURCE_IPC);}
#define unl_multiCores()    {cc_hwunlock_resource(CC_CORE_CA53_CORE2,CC_RESOURCE_IPC);unl_cpu();}
#endif

#if __COREIPC__
static unsigned int drv_getSysTick(void)
{
	volatile unsigned int *p1 = (volatile unsigned int *)(TIMER0_COUNTER_REG);
	return *p1;
}
#endif


static void NvtIpc_WaitRebootAck(void)
{
	UINT32 volatile value;
	UINT32 timeoutCnt = 0;
	while (1) {
		value = INW(CC_REBOOT_ACK_ADDR);
		if (value == NVTIPC_SYSCMD_REBOOT_ACK) {
			break;
		}
		if (timeoutCnt++ == IPC_WAIT_TIMEOUT_CNT) {
			DBG_ERR("timeout\r\n");
		}
	}
}

static void NvtIpc_WaitPoweroffAck(void)
{
	UINT32 volatile value;
	UINT32 timeoutCnt = 0;
	while (1) {
		value = INW(CC_POWEROFF_ACK_ADDR);
		if (value == NVTIPC_SYSCMD_POWEROFF_ACK) {
			break;
		}
		if (timeoutCnt++ == IPC_WAIT_TIMEOUT_CNT) {
			DBG_ERR("timeout\r\n");
		}
	}
}

static enum hrtimer_restart  NvtIpc_SysCall_TimerCb(struct hrtimer *timer)
{
	complete(&sys_cmd_completion);
	DBG_WRN("NVTIPC_SYSCALL Timeout\r\n");
	return HRTIMER_NORESTART;
}

void NvtIpc_WaitSysCallAck(void)
{
	struct  hrtimer htimer;
	ktime_t kt_periode;

	kt_periode = ktime_set(SYSCALL_MAX_WAIT_TIME, 0); //seconds,nanoseconds
	hrtimer_init(&htimer, CLOCK_REALTIME, HRTIMER_MODE_REL);
	htimer.function = NvtIpc_SysCall_TimerCb;
	hrtimer_start(&htimer, kt_periode, HRTIMER_MODE_REL);
	wait_for_completion(&sys_cmd_completion);
	hrtimer_cancel(&htimer);
}

static enum hrtimer_restart  NvtIpc_Uart_TimerCb(struct hrtimer *timer)
{
	complete(&sys_cmd_completion);
	DBG_WRN("NVTIPC_UART Timeout\r\n");
	return HRTIMER_NORESTART;
}

static void NvtIpc_WaitUartAck(void)
{
	struct  hrtimer htimer;
	ktime_t kt_periode;

	kt_periode = ktime_set(UART_MAX_WAIT_TIME, 0); //seconds,nanoseconds
	hrtimer_init(&htimer, CLOCK_REALTIME, HRTIMER_MODE_REL);
	htimer.function = NvtIpc_Uart_TimerCb;
	hrtimer_start(&htimer, kt_periode, HRTIMER_MODE_REL);
	wait_for_completion(&sys_cmd_completion);
	hrtimer_cancel(&htimer);
}

static enum hrtimer_restart  nvt_ipc_longcounter_timercb(struct hrtimer *timer)
{
	complete(&sys_cmd_completion);
	DBG_WRN("NVTIPC_GET_LONG_COUNTER Timeout\r\n");
	return HRTIMER_NORESTART;
}


static void nvt_ipc_wait_longcounter_ack(void)
{
	struct  hrtimer htimer;
	ktime_t kt_periode;

	kt_periode = ktime_set(LONG_COUNTER_MAX_WAIT_TIME, 0); //seconds,nanoseconds
	hrtimer_init(&htimer, CLOCK_REALTIME, HRTIMER_MODE_REL);
	htimer.function = nvt_ipc_longcounter_timercb;
	hrtimer_start(&htimer, kt_periode, HRTIMER_MODE_REL);
	wait_for_completion(&sys_cmd_completion);
	hrtimer_cancel(&htimer);
}

#if __SEMCMD_WAIT_ACK__
static void NvtIpc_WaitSemAck(void)
{
	if (SEM_WAIT(ipc_sem)) {
		DBG_ERR("ERESTARTSYS");
	}
}
#endif

void NvtIpc_SysCmdAck(NVTIPC_SYS_CMD   sysCmd)
{
	switch (sysCmd) {
	case NVTIPC_SYSCMD_SYSCALL_ACK:
	case NVTIPC_SYSCMD_UART_ACK:
	case NVTIPC_SYSCMD_GET_LONG_COUNTER_ACK:
		complete(&sys_cmd_completion);
		break;
	default:
		DBG_ERR("Unknown syscmd %d\r\n", sysCmd);
	}
}




NVTIPC_KEY nvt_ipc_ftok(const char *path)
{
	return NvtIpcMsgQue_Ftok(path);
}

// get message queue
NVTIPC_I32 nvt_ipc_msg_get(NVTIPC_KEY key)
{
	return NvtIpcMsgQue_Get(key);
}

// release message queue
NVTIPC_ER nvt_ipc_msg_rel(NVTIPC_U32 msqid)
{
	return NvtIpcMsgQue_Rel(msqid);
}


NVTIPC_I32 nvt_ipc_msg_sndEx(NVTIPC_U32 coreId, NVTIPC_U32 msqid, NVTIPC_SENDTO sendTo, void  *pMsg, NVTIPC_U32 msgsz, NVTIPC_U32 ioctl)
{
	NVTIPC_MSGQUE      *pMsgQue;
	NVTIPC_SEND_MSG    *pSendMsg;
	CC_CMD              ccCmd;
	int                 ret;
	NVTIPC_SHM_CTRL    *pShmCtrl = NvtIpcMsgQue_GetShmCtrl();

	DBG_IND("msqid = %d, sendTo=%d, pMsg=0x%x, msgsz=0x%x\r\n", (int)msqid, (int)sendTo, (int)pMsg, (int)msgsz);
	pMsgQue = NvtIpcMsgQue_GetQuebyIDEx(coreId, msqid);
	if (unlikely(!pMsgQue)) {
		DBG_ERR("pMsgQue is NULL\r\n");
		return NVTIPC_ER_PARM;
	}
	if (unlikely(pMsg == NULL)) {
		DBG_ERR("lock sem fail\r\n");
		return NVTIPC_ER_PARM;
	}
	if (unlikely(sendTo > NVTIPC_SENDTO_MAX)) {
		DBG_ERR("sendTo =%d \r\n", sendTo);
		return NVTIPC_ER_PARM;
	}
	if (unlikely(!pMsg)) {
		DBG_ERR("pMsg is NULL\r\n");
		return NVTIPC_ER_PARM;
	}
	if (unlikely(msgsz > NVTIPC_MSG_ELEMENT_SIZE)) {
		DBG_ERR("msgsz =%d > limit %d\r\n", (int)msgsz, NVTIPC_MSG_ELEMENT_SIZE);
		return NVTIPC_ER_SNDSIZE_OVER_LIMIT;
	}
	if (unlikely(!msgsz)) {
		DBG_ERR("msgsz is 0\r\n");
		return NVTIPC_ER_PARM;
	}
	pSendMsg = NvtIpcMsgQue_LockSndMsgBufEx(coreId, sendTo);
	pSendMsg->sendTo = sendTo;
	memcpy(&pSendMsg->msgData, pMsg, msgsz);
	if (NVTIPC_SYS_QUEUE_ID == msqid) {
		NVTIPC_SYS_MSG  *pIpcSysMsg = (NVTIPC_SYS_MSG *)&pSendMsg->msgData;
		char            *pCmd = NvtIpcMsgQue_GetSysCmdBuf();


		if (NVTIPC_SYSCMD_SYSCALL_REQ == pIpcSysMsg->sysCmdID
			|| NVTIPC_SYSCMD_UART_REQ == pIpcSysMsg->sysCmdID) {
			if (pIpcSysMsg->DataSize > NVTIPC_SYS_CMD_MAX_LEN) {
				DBG_ERR("pIpcSysMsg->DataSize Over limit %d\r\n", NVTIPC_SYS_CMD_MAX_LEN);
				NvtIpcMsgQue_UnlockSndMsgBufEx(coreId, sendTo);
				return NVTIPC_ER_PARM;
			}
			if (ioctl) {
				copy_from_user(pCmd, (char *)pIpcSysMsg->DataAddr, pIpcSysMsg->DataSize);
			} else {
				memcpy(pCmd, (char *)pIpcSysMsg->DataAddr, pIpcSysMsg->DataSize);
			}
#if __IPC_SEND_BUF_CACHEABLE__
			if (ipc_isCacheAddr((UINT32)pCmd)) {
				ipc_storeCache((void *)pCmd, pIpcSysMsg->DataSize);
			}
#endif
		}
		pIpcSysMsg->DataAddr = NvtIpcMsgQue_getPhyAddr((UINT32)pCmd);
	}
#if __IPC_SEND_BUF_CACHEABLE__
	if (ipc_isCacheAddr((UINT32)&pSendMsg->msgData)) {
		ipc_storeCache((void *)&pSendMsg->msgData, msgsz);
	}
#endif
	// send data to other CPU
	if (likely(pSendMsg->sendTo == NVTIPC_SENDTO_CORE1)) {
#if __VOS_IPC__
		{
			INT32 msqid_c1;
			msqid_c1 = NvtIpcMsgQue_GetIDbyKeyEx(__CORE1, (INT32)pMsgQue->uiSharedKey);
			if (msqid_c1 < 0) {
				ret = NVTIPC_ER_MSGQUE_ID;
				goto sndErr;
			}
			ret = NvtIPC_MsgPostEx(__CORE1, msqid_c1, pSendMsg->msgData, msgsz);
			if (ret < 0) {
				goto sndErr;
			}
			wake_up_interruptible(&nvt_readWaitq_c1[msqid_c1]);
		}
#else
		ccCmd.uiCMDOperation = pMsgQue->uiSharedKey;
		ccCmd.pData = (CC_U32 *)NvtIpcMsgQue_getPhyAddr((UINT32)&pSendMsg->msgData);
		ccCmd.uiDataSize = msgsz;
		if (pShmCtrl->Cpu2Cpu1Err != 0) {
			DBG_ERR("Core %d sendTo = %d, key 0x%x, err %d\r\n", __CORE, sendTo, pShmCtrl->Cpu2Cpu1Key, pShmCtrl->Cpu2Cpu1Err);
		}
		pShmCtrl->Cpu2Cpu1Err = 0;
		nvt_ipc_flush_shm_data();
		ret = cc_send_core1_cmd(&ccCmd);
#endif
	}
#if __VOS_IPC__
	else if (pSendMsg->sendTo == NVTIPC_SENDTO_CORE2) {
		INT32 msqid_c2;
		msqid_c2 = NvtIpcMsgQue_GetIDbyKeyEx(__CORE2, (INT32)pMsgQue->uiSharedKey);
		if (msqid_c2 < 0) {
			ret = NVTIPC_ER_MSGQUE_ID;
			goto sndErr;
		}
		ret = NvtIPC_MsgPostEx(__CORE2, msqid_c2, pSendMsg->msgData, msgsz);
		if (ret < 0) {
			goto sndErr;
		}
		wake_up_interruptible(&nvt_readWaitq[msqid_c2]);
	}
#endif
	else if (pSendMsg->sendTo == NVTIPC_SENDTO_CORE3) {
		ccCmd.uiCMDOperation = pMsgQue->uiSharedKey;
		ccCmd.pData = (CC_U32 *)NvtIpcMsgQue_getPhyAddr((UINT32)&pSendMsg->msgData);
		ccCmd.uiDataSize = msgsz;
		if (pShmCtrl->Cpu2Cpu3Err != 0) {
			DBG_ERR("Core %ud sendTo = %d, key 0x%x, err %ud\r\n", __CORE, sendTo, pShmCtrl->Cpu2Cpu3Key, pShmCtrl->Cpu2Cpu3Err);
		}
		pShmCtrl->Cpu2Cpu3Err = 0;
		nvt_ipc_flush_shm_data();
		ret = cc_send_core3_cmd(&ccCmd);
	} else {
		ccCmd.uiCMDOperation = pMsgQue->uiSharedKey;
		ccCmd.pData = (CC_U32 *)NvtIpcMsgQue_getPhyAddr((UINT32)&pSendMsg->msgData);
		ccCmd.uiDataSize = msgsz;
		if (pShmCtrl->Cpu2Cpu4Err != 0) {
			DBG_ERR("Core %ud sendTo = %d, key 0x%x, err %ud\r\n", __CORE, sendTo, pShmCtrl->Cpu2Cpu4Key, pShmCtrl->Cpu2Cpu4Err);
		}
		pShmCtrl->Cpu2Cpu4Err = 0;
		nvt_ipc_flush_shm_data();
		ret = cc_send_core4_cmd(&ccCmd);
	}
	NvtIpcMsgQue_UnlockSndMsgBufEx(coreId, sendTo);

	// has some error
	if (unlikely(ret < 0)) {
		return NVTIPC_ER_SND_MSG;
	}
#if __SYSCMD_WAIT_FINISH__
	if (NVTIPC_SYS_QUEUE_ID == msqid) {
		NVTIPC_SYS_MSG  *pIpcSysMsg = (NVTIPC_SYS_MSG *)&pSendMsg->msgData;
		if (NVTIPC_SYSCMD_REBOOT_REQ == pIpcSysMsg->sysCmdID) {
			// wait reboot ack
			NvtIpc_WaitRebootAck();
		} else if (NVTIPC_SYSCMD_POWEROFF_REQ == pIpcSysMsg->sysCmdID) {
			// wait poweroff ack
			NvtIpc_WaitPoweroffAck();
		} else if (NVTIPC_SYSCMD_UART_REQ == pIpcSysMsg->sysCmdID) {
			NvtIpc_WaitUartAck();
		} else if (NVTIPC_SYSCMD_GET_LONG_COUNTER_REQ == pIpcSysMsg->sysCmdID) {
			NVTIPC_SYS_MSG  *pRetMsg = (NVTIPC_SYS_MSG *)pMsg;
			char            *pCmd = NvtIpcMsgQue_GetSysCmdBuf();


			nvt_ipc_wait_longcounter_ack();
			pIpcSysMsg->DataAddr = (UINT32)pCmd;
			if (ioctl) {
				copy_to_user((char *)pRetMsg->DataAddr, (char *)pIpcSysMsg->DataAddr, pRetMsg->DataSize);
			} else {
				memcpy((char *)pRetMsg->DataAddr, (char *)pIpcSysMsg->DataAddr, pRetMsg->DataSize);
			}
		}
	}
#endif
	return msgsz;
#if __VOS_IPC__
sndErr:
	if (ret == NVTIPC_ER_MSGQUE_ID) {
		DBG_ERR("Core %d sendTo = %d, key 0x%x, can't find receiver\r\n", (int)coreId, sendTo, (int)pMsgQue->uiSharedKey);
	} else {
		DBG_ERR("Core %d sendTo = %d, key 0x%x, err = %d\r\n", (int)coreId, sendTo, (int)pMsgQue->uiSharedKey, ret);
	}
	NvtIpcMsgQue_UnlockSndMsgBufEx(coreId, sendTo);
	return ret;
#endif
}

NVTIPC_I32 nvt_ipc_msg_snd(NVTIPC_U32 msqid, NVTIPC_SENDTO sendTo, void  *pMsg, NVTIPC_U32 msgsz)
{
	return nvt_ipc_msg_sndEx(__CORE2, msqid, sendTo, pMsg, msgsz, 0);
}

NVTIPC_I32 nvt_ipc_msg_rcvEx(NVTIPC_U32 coreId, NVTIPC_U32 msqid, void  *pMsg, NVTIPC_U32 msgsz)
{
	NVTIPC_MSGQUE *pMsgQue;
	NVTIPC_MSG     MsgData;
	UINT32         sharedKey;

	if (unlikely(!pMsg)) {
		DBG_ERR("pMsg is NULL\r\n");
		return NVTIPC_ER_PARM;
	}
	pMsgQue = NvtIpcMsgQue_GetQuebyIDEx(coreId, msqid);
	if (unlikely(!pMsgQue)) {
		return NVTIPC_ER_MSGQUE_ID;
	}
	while (NvtIpcMsgQue_isEmpty(pMsgQue)) {
		wait_queue_head_t    *pWaitq;

		sharedKey = pMsgQue->uiSharedKey;
#if !__COREIPC__|| __VOS_UNIT_TEST__
		if (coreId == __CORE1) {
			pWaitq = &m_ctrl->nvt_readWaitq_c1[msqid];
		} else {
			pWaitq = &m_ctrl->nvt_readWaitq[msqid];
		}

#else
		pWaitq = &m_ctrl->nvt_readWaitq[msqid];
#endif
		// the message queue is empty , need to wait
		// exit wait condition has two case , message queue has data, or message queue is released.
		if (unlikely(wait_event_interruptible(*pWaitq, ((!NvtIpcMsgQue_isEmpty(pMsgQue)) || (!NvtIpcMsgQue_isValid(pMsgQue, sharedKey)))))) {
			// wake up by signal
			if (NvtIpcMsgQue_isEmpty(pMsgQue)) {
				//DBG_WRN("Queue is empty , sharedKey=0x%x\r\n",sharedKey);
				return -ERESTARTSYS;
			} else if (!NvtIpcMsgQue_isValid(pMsgQue, sharedKey)) {
				DBG_WRN("Queue is released , sharedKey=0x%x\r\n", sharedKey);
				return NVTIPC_ER_MSGQUE_RELEASED;
			}
		}
		// the message queue is released
		if (unlikely(!NvtIpcMsgQue_isValid(pMsgQue, sharedKey))) {
			DBG_WRN("Queue is released , sharedKey=0x%x\r\n", sharedKey);
			return NVTIPC_ER_MSGQUE_RELEASED;
		}
	}
	// ok , data is there, return something
	NvtIpcMsgQue_DeQueue(pMsgQue, &MsgData);
	if (unlikely(msgsz < MsgData.size)) {
		DBG_ERR("msgsz = %d < dataSize %d\r\n", (int)msgsz, MsgData.size);
		return NVTIPC_ER_RCV_MSG;
	}
	memcpy(pMsg, (void *)&MsgData.data, MsgData.size);
	return MsgData.size;
}

NVTIPC_I32 nvt_ipc_msg_rcv(NVTIPC_U32 msqid, void  *pMsg, NVTIPC_U32 msgsz)
{
	return nvt_ipc_msg_rcvEx(__CORE2, msqid, pMsg, msgsz);
}

void nvt_ipc_sem_test_mem(NVTIPC_U32 value)
{
	NVTIPC_SHM_CTRL *pShmCtrl = NvtIpcMsgQue_GetShmCtrl();
	pShmCtrl->reserved[0] = value;
}


NVTIPC_I32 nvt_ipc_sem_lock(unsigned long semid)
{
	NVTIPC_SHM_CTRL *pShmCtrl = NvtIpcMsgQue_GetShmCtrl();
#if __COREIPC__
	UINT32 ptn = (1 << semid);
	//UINT32 timeoutCnt;
#endif

	//DBG_DUMP("LB\r\n");
	//DBG_IND("B Lock semid = %d\r\n",(int)semid);
	if (unlikely(!pShmCtrl)) {
		DBG_ERR("%d\r\n", NVTIPC_ER_NOT_OPENED);
		return NVTIPC_ER_PARM;
	}
	if (unlikely(semid >= NVTIPC_SEM_NUM)) {
		DBG_ERR("semid = %d\r\n", (int)semid);
		return NVTIPC_ER_PARM;
	}
#if !__COREIPC__
	NvtIpcMsgQue_SemCtrlLockLocal(semid);
	return NVTIPC_ER_OK;
#else
	NvtIpcMsgQue_SemCtrlLockLocal(semid);
	loc_multiCores();
	if (pShmCtrl->SemBits & ptn) {
		//DBG_IND("semid %d is free\r\n",(int)semid);
		pShmCtrl->SemBits &= ~ptn;
		// make sure the bits is writen to dram
		nvt_ipc_flush_shm_data();
		//DBG_DUMP("pShmCtrl->SemBits= 0x%x, timeoutCnt=%d\r\n",pShmCtrl->SemBits,timeoutCnt);
		unl_multiCores();
	} else {
		//DBG_IND("semid %d is locked\r\n",(int)semid);
		pShmCtrl->Cpu2SemWaitCnt[semid]++;
		nvt_ipc_flush_shm_data();
		//DBG_IND("Cpu2SemWaitCnt = 0x%x \r\n",pShmCtrl->Cpu2SemWaitCnt[semid]);
		unl_multiCores();
		while (1) {
			NvtIpcMsgQue_SemCtrlLockCores(semid);
			loc_multiCores();
			if (pShmCtrl->SemBits & ptn) {
				pShmCtrl->SemBits &= ~ptn;
				pShmCtrl->Cpu2SemWaitCnt[semid]--;
				nvt_ipc_flush_shm_data();
				unl_multiCores();
				break;
			}
			// the semaphore is get by other cpu
			else {
				//DBG_DUMP("lock by cpu1\r\n");
				//pShmCtrl->Cpu2SemWaitCnt[semid]++;
				nvt_ipc_flush_shm_data();
				unl_multiCores();
			}
		}
	}
	pShmCtrl->Cpu2SemLockTime = drv_getSysTick();
	return NVTIPC_ER_OK;
#endif
}


NVTIPC_I32 nvt_ipc_sem_unlock(unsigned long semid)
{
	NVTIPC_SHM_CTRL *pShmCtrl = NvtIpcMsgQue_GetShmCtrl();
#if __COREIPC__
	UINT32 ptn = (1 << semid);
#endif

	//DBG_IND("B Unlock semid = %d\r\n",(int)semid);
	if (unlikely(!pShmCtrl)) {
		DBG_ERR("%d\r\n", NVTIPC_ER_NOT_OPENED);
		return NVTIPC_ER_PARM;
	}
	if (unlikely(semid >= NVTIPC_SEM_NUM)) {
		DBG_ERR("semid = %d\r\n", (int)semid);
		return NVTIPC_ER_PARM;
	}
#if !__COREIPC__
	NvtIpcMsgQue_SemCtrlUnlockLocal(semid);
	return NVTIPC_ER_OK;
#else
	loc_multiCores();
	pShmCtrl->SemBits |= ptn;
	pShmCtrl->Cpu2SemUnLockTime = drv_getSysTick();
	if (pShmCtrl->Cpu2SemWaitCnt[semid]) {
		//pShmCtrl->Cpu2SemWaitCnt[semid]--;
		nvt_ipc_flush_shm_data();
		//DBG_IND("Cpu2SemWaitCnt = 0x%x \r\n",pShmCtrl->Cpu2SemWaitCnt[semid]);
		unl_multiCores();
		NvtIpcMsgQue_SemCtrlUnlockCores(semid);
	} else if (pShmCtrl->Cpu1SemWaitCnt[semid]) {
		//pShmCtrl->Cpu1SemWaitCnt[semid]--;
		nvt_ipc_flush_shm_data();
		//DBG_IND("Cpu1SemWaitCnt = 0x%x \r\n",pShmCtrl->Cpu1SemWaitCnt[semid]);
		unl_multiCores();
		// need to send to cpu1 to wake up waiting thread
		{
			NVTIPC_SEM_MSG   ipcMsg;
			ipcMsg.CmdID = NVTIPC_SEM_CMD_UNLOCK;
			ipcMsg.semid = semid;
			nvt_ipc_msg_snd(NVTIPC_SEM_QUEUE_ID, NVTIPC_SENDTO_CORE1, &ipcMsg, sizeof(ipcMsg));
		}
#if __SEMCMD_WAIT_ACK__
		NvtIpc_WaitSemAck();
#endif
	} else {
		nvt_ipc_flush_shm_data();
		//DBG_IND("No cpu wait\r\n");
		unl_multiCores();
	}
#endif
	NvtIpcMsgQue_SemCtrlUnlockLocal(semid);
	//DBG_IND("E Unlock semid = %d\r\n",(int)semid);
	return NVTIPC_ER_OK;
}

NVTIPC_U32   nvt_ipc_shm_get(NVTIPC_KEY key, NVTIPC_U32 size)
{
	UINT32              i;
	NVTIPC_I32          ret = NVTIPC_ER_OK;
	NVTIPC_SHM_CTRL    *pShmCtrl = NvtIpcMsgQue_GetShmCtrl();

	if (size > NVTIPC_SHM_SIZE) {
		DBG_ERR("size %d > limit %d\r\n", (int)size, NVTIPC_SHM_SIZE);
		return 0;
	}
	loc_multiCores();
	// find exist
	for (i = 0; i < NVTIPC_MSG_QUEUE_NUM; i++) {
		if (pShmCtrl->shmbuf[i].key == key) {
			pShmCtrl->shmbuf[i].ref_count++;
			ret = (NVTIPC_I32)&pShmCtrl->shmbuf[i].buf;
			goto find_mem;
		}
	}
	// find free
	for (i = 0; i < NVTIPC_MSG_QUEUE_NUM; i++) {
		if (pShmCtrl->shmbuf[i].key == 0) {
			pShmCtrl->shmbuf[i].key = key;
			pShmCtrl->shmbuf[i].ref_count = 1;
			ret = (NVTIPC_I32)&pShmCtrl->shmbuf[i].buf;
			goto find_mem;
		}
	}
	nvt_ipc_flush_shm_data();
	unl_multiCores();
	return 0;
find_mem:
	unl_multiCores();
	return ret;
}

NVTIPC_ER   nvt_ipc_shm_rel(NVTIPC_U32 shm_addr)
{
	NVTIPC_SHM_CTRL    *pShmCtrl = NvtIpcMsgQue_GetShmCtrl();
	NVTIPC_SHM_BUF     *pShmbuf;

	if (shm_addr < (NVTIPC_U32)&pShmCtrl->shmbuf[0] || shm_addr > (NVTIPC_U32)&pShmCtrl->shmbuf[0] + sizeof(pShmCtrl->shmbuf)) {
		DBG_ERR("Invalid shm_addr 0x%x\r\n", (int)shm_addr);
		return NVTIPC_ER_PARM;
	}
	loc_multiCores();
	pShmbuf = (NVTIPC_SHM_BUF *)(shm_addr - NVTIPC_SHM_HEADER_SIZE);
	if (pShmbuf->ref_count) {
		pShmbuf->ref_count--;
	}

	if (pShmbuf->ref_count == 0) {
		memset(pShmbuf, 0x00, sizeof(NVTIPC_SHM_BUF));
	}
	nvt_ipc_flush_shm_data();
	unl_multiCores();
	return NVTIPC_ER_OK;
}

void   nvt_ipc_shm_flush(NVTIPC_U32 shm_addr, NVTIPC_U32 shm_size)
{
	nvt_ipc_flush_shm_data();
}


EXPORT_SYMBOL(nvt_ipc_ftok);
EXPORT_SYMBOL(nvt_ipc_msg_get);
EXPORT_SYMBOL(nvt_ipc_msg_rel);
EXPORT_SYMBOL(nvt_ipc_msg_snd);
EXPORT_SYMBOL(nvt_ipc_msg_rcv);
EXPORT_SYMBOL(nvt_ipc_sem_lock);
EXPORT_SYMBOL(nvt_ipc_sem_unlock);
EXPORT_SYMBOL(nvt_ipc_shm_get);
EXPORT_SYMBOL(nvt_ipc_shm_rel);
EXPORT_SYMBOL(nvt_ipc_shm_flush);

