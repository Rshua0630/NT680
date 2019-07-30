/**

    This file contains the nvt_ipc kernel thread init/exit and thread main fuction.

    @file       nvt_ipc_task.c
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
#include "nvt_ipc_debug.h"
#include <mach/nvt_type.h>
#include "NvtIpcInt.h"
#include "NvtIpcMsgQue.h"



static DEFINE_SEMAPHORE(ipc_sem);

// kernel thread
#if UNIT_TEST_IPC
static struct task_struct *ipc_test_tsk;
#endif
static struct task_struct *ipc_err_tsk;
static struct task_struct *ipc_sem_tsk;
static INT32  ipc_errCode = 0;
static UINT32 ipc_errCMD = 0;


#if UNIT_TEST_IPC

#if 0
static int ipcTestThread(void *arg)
{
	NVTIPC_SYS_MSG  ipcSysMsg = {0};
	char syscall_str[128];

	msleep(5000);
	ipcSysMsg.sysCmdID = NVTIPC_SYSCMD_SYSCALL_REQ;
	strcpy(syscall_str, "reboot");
	ipcSysMsg.DataAddr = (NVTIPC_U32)&syscall_str[0];
	ipcSysMsg.DataSize = strlen(syscall_str) + 1;
	nvt_ipc_msg_snd(NVTIPC_SYS_QUEUE_ID, NVTIPC_SENDTO_CORE1, &ipcSysMsg, sizeof(ipcSysMsg));
	return 0;
}
/*
static int ipcTestThread(void *arg)
{
    NVTIPC_I32 msqid;
    UINT32 test[NVTIPC_MSG_ELEMENT_SIZE/4];
    int    rcvSize, sndSize, i;
    msqid = nvt_ipc_msg_get(nvt_ipc_ftok("ipc_unittest"));
    if (msqid < 0)
    {
        DBG_ERR("get ipc_unittest queue fail.\n");
        return 0;
    }
    i = 0;
    while (1)
    {
        rcvSize = nvt_ipc_msg_rcv(msqid,&test,sizeof(test));
        DBG_DUMP("ut i=%d\r\n",i);
        i++;
        if (rcvSize < 0)
        {
            DBG_ERR("rcvSize %d\r\n",rcvSize);
            break;
        }
        sndSize = nvt_ipc_msg_snd(msqid, NVTIPC_SENDTO_CORE1, &test, rcvSize);
    }
    nvt_ipc_msg_rel(msqid);
    return 0;
}
*/
#else
static int ipcTestThread(void *arg)
{
	volatile unsigned int  *pCounter = (volatile unsigned int *)ipc_getNonCacheAddr(0x00000680);
	int count, i;
	int value = 20;
	for (;;) {
		count = 1000;
		while (count) {
			nvt_ipc_sem_lock(0);
			(*pCounter) = value;
			for (i = 0; i < 10000; i++) {
				if ((*pCounter) != value) {
					while (1) {
						DBG_ERR("value(%d) != %d\r\n", (*pCounter), value);
					}
				}
			}
			nvt_ipc_sem_unlock(0);
			if (count % 100 == 0) {
				DBG_DUMP("sem_race, count=%d, gCounter=%d\r\n", count, (*pCounter));
			}
			count--;
		}
		DBG_DUMP("ipcTestThread, gCounter2=%d\r\n", (*pCounter));
		msleep(1000);
	}
	return 0;
}
#endif
static int NvtIpc_UnitTest_Init(void)
{
	int ret;
	// start a kernel thread for handing ipc sys cmd
	ipc_test_tsk = kthread_create(ipcTestThread, (void *)NULL, "ipcTestThread");
	if (IS_ERR(ipc_test_tsk)) {
		ret = PTR_ERR(ipc_test_tsk);
		ipc_test_tsk = NULL;
		DBG_ERR("creating ipc_test_tsk %d fail.\n", (int)ipc_test_tsk);
		return -1;
	}
	wake_up_process(ipc_test_tsk);
	return 0;

}
static void NvtIpc_UnitTest_Exit(void)
{
	if (ipc_test_tsk) {
		kthread_stop(ipc_test_tsk);
		ipc_test_tsk = NULL;
	}
}
#endif


static int ipcErrThread(void *arg)
{
	while (1) {
		if (wait_event_interruptible(m_ctrl->nvt_errWaitq, (ipc_errCode != 0))) {
			DBG_ERR("force exit\r\n");
			break;
		}
		if (ipc_errCode == NVTIPC_ER_MSGQUE_ID) {
			DBG_ERR("Core %d NvtIPC msg key 0x%x, can't find receiver\r\n", __CORE, (int)ipc_errCMD);
		} else if (ipc_errCode == NVTIPC_ER_MSGQUE_FULL) {
			DBG_ERR("Core %d NvtIPC msg key 0x%x, Queue full\r\n", __CORE, (int)ipc_errCMD);
		} else {
			DBG_ERR("Core %d NvtIPC msg key 0x%x, err %d\r\n", __CORE, (int)ipc_errCMD, (int)ipc_errCode);
		}
		ipc_errCode = 0;
		ipc_errCMD = 0;
	}
	return 0;
}

static int NvtIpc_ErrThread_Init(void)
{
	int ret;
	// start a kernel thread for handing ipc sys cmd
	ipc_err_tsk = kthread_create(ipcErrThread, (void *)NULL, "ipcErrThread");
	if (IS_ERR(ipc_err_tsk)) {
		ret = PTR_ERR(ipc_err_tsk);
		ipc_err_tsk = NULL;
		DBG_ERR("creating ipc_err_tsk %d fail.\n", (int)ipc_err_tsk);
		return -1;
	}
	wake_up_process(ipc_err_tsk);
	return 0;
}
static void NvtIpc_ErrThread_Exit(void)
{
	if (ipc_err_tsk) {
		kthread_stop(ipc_err_tsk);
		ipc_err_tsk = NULL;
	}
}

#if 1
static int ipcSemThread(void *arg)
{
	NVTIPC_MSGQUE    *pMsgQue = (NVTIPC_MSGQUE *)arg;
	NVTIPC_MSG        MsgData;
	NVTIPC_SEM_MSG    ipcMsg;
	UINT32            sharedKey;

	//
	SEM_WAIT(ipc_sem);
	while (1) {
		while (NvtIpcMsgQue_isEmpty(pMsgQue)) {
			sharedKey = pMsgQue->uiSharedKey;
			// the message queue is empty , need to wait
			if (wait_event_interruptible(m_ctrl->nvt_readWaitq[NVTIPC_SEM_QUEUE_ID], ((!NvtIpcMsgQue_isEmpty(pMsgQue)) || (!NvtIpcMsgQue_isValid(pMsgQue, sharedKey))))) {
				return -ERESTARTSYS;
			}
			// the message queue is released
			if (unlikely(!NvtIpcMsgQue_isValid(pMsgQue, sharedKey))) {
				DBG_WRN("Queue is released , sharedKey=0x%x\r\n", sharedKey);
				return -EIDRM;
			}
		}
		// ok , data is there, return something
		{
			NvtIpcMsgQue_DeQueue(pMsgQue, &MsgData);
			memcpy(&ipcMsg, &MsgData.data, sizeof(ipcMsg));
			DBG_IND("Rcv IPC SemCmd cmd=%d, semid=%d\r\n", ipcMsg.CmdID, (int)ipcMsg.semid);
			switch (ipcMsg.CmdID) {
#if 0
			case NVTIPC_SEM_CMD_INIT: {
					NVTIPC_SEM_INIT_MSG  *pSemMsg = &ipcMsg;
					NvtIpcMsgQue_SetSemCtrl(pSemMsg->semCtrl);
					break;
				}
#endif
			case NVTIPC_SEM_CMD_UNLOCK: {
#if __SEMCMD_WAIT_ACK__
					NVTIPC_SHM_CTRL *pShmCtrl = NvtIpcMsgQue_GetShmCtrl();
					UINT32 ptn;
#endif
					UINT32 semid = ipcMsg.semid;

#if __SEMCMD_WAIT_ACK__
					ptn = (1 << semid);
					loc_multiCores();
					DBG_IND("semid = %d, Cpu2SemWaitCnt = %d, ptn =0x%x\r\n", (int)semid, (int)pShmCtrl->Cpu2SemWaitCnt[semid], (int)ptn);
					pShmCtrl->Cpu2AckBits |= ptn;
					DBG_IND("NVTIPC_SEM_CMD_UNLOCK \r\n");
					unl_multiCores();
#endif
					NvtIpcMsgQue_SemCtrlUnlockCores(semid);

				}
				break;
			case NVTIPC_SEM_CMD_UNLOCK_ACK:
				SEM_SIGNAL(ipc_sem);
				break;

			default:
				DBG_ERR("Err IPC sem cmd=%d\r\n", ipcMsg.CmdID);
			}
		}
	}
	return 0;
}
static int  NvtIpc_SemThread_Init(void)
{
	NVTIPC_MSGQUE    *pMsgQue;
	int              ret;

	pMsgQue = NvtIpcMsgQue_GetQuebyID(NVTIPC_SEM_QUEUE_ID);
	// start a kernel thread for handing ipc sys cmd
	ipc_sem_tsk = kthread_create(ipcSemThread, (void *)pMsgQue, "ipcSemThread");
	if (IS_ERR(ipc_sem_tsk)) {
		ret = PTR_ERR(ipc_sem_tsk);
		ipc_sem_tsk = NULL;
		DBG_ERR("creating ipc_sem_tsk %d fail.\n", (int)ipc_sem_tsk);
		return -1;
	}
	wake_up_process(ipc_sem_tsk);
	return 0;
}
static void NvtIpc_SemThread_Exit(void)
{
	if (ipc_sem_tsk) {
		kthread_stop(ipc_sem_tsk);
		ipc_sem_tsk = NULL;
	}
}
#endif

int NvtIpc_Task_Init(void)
{
#if UNIT_TEST_IPC
	if (NvtIpc_UnitTest_Init() < 0) {
		return -1;
	}
#endif
	if (NvtIpc_SemThread_Init() < 0) {
		return -1;
	}
	if (NvtIpc_ErrThread_Init() < 0) {
		return -1;
	}
	return 0;
}

void NvtIpc_Task_Exit(void)
{
#if UNIT_TEST_IPC
	NvtIpc_UnitTest_Exit();
#endif
	NvtIpc_SemThread_Exit();
	NvtIpc_ErrThread_Exit();
}


void NvtIpc_Task_SetErrCode(INT32 err, UINT32 CMD)
{
	ipc_errCode = err;
	ipc_errCMD = CMD;
}

