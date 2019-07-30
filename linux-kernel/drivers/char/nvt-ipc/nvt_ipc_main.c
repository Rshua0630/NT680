/**

    This file handles the main flow nvt_ipc driver operations,...etc

    @file       nvt_ipc_main.c
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
#include <linux/dma-mapping.h>
#include <linux/poll.h>
#include <linux/wait.h>
#include <linux/suspend.h>
#ifdef CONFIG_OF
/* For open firmware. */
#include <linux/of_address.h>
#include <linux/of_device.h>
#include <linux/of_platform.h>
#endif
#include <asm/page.h>
#include <asm/io.h>
#include <asm/cacheflush.h>
#include <mach/nvt-ipc.h>
#include <mach/nvt_type.h>
#include <mach/ioaddress.h>
#include <mach/hardware.h>
#include <mach/cc.h>
#include "nvt_ipc_ioctl.h"
#include "nvt_ipc_debug.h"
#include "nvt_ipc_task.h"
#include "nvt_ipc_sig.h"
#include "NvtIpcInt.h"
#include "NvtIpcMsgQue.h"



#define DRV_VERSION               "1.00.000"

#define DEVICE_NAME               "nvt_ipc"
#define COPY_FROM_USER_ERR_STR    "copy_from_user"
#define COPY_TO_USER_ERR_STR      "copy_to_user"

//#define UITRON_BASE_ADDR             0x02000000
//#define UITRON_SIZE                  0x1e000000
//#define DRAM1_BASE_ADDR              0x00000000
//#define DRAM2_BASE_ADDR              0x40000000


#define NONCACHE_FLAG                0x80000000
//#define MMIO_BASE                    IOADDR_GLOBAL_BASE



typedef void (*CC_GETCMD)(PCC_CMD pCMD);
typedef void (*CC_ACKCMD)(void);
typedef void (*CC_SETERR)(INT32 err, UINT32 CMD);

struct nvt_ipc_device {
	UINT32  ReqQueueBits;
#if __VOS_IPC__
	UINT32  ReqQueueBits_c1;
#endif
	struct nvt_ipc_ctrl *m_ctrl;
};

struct nvt_ipc_ctrl *m_ctrl = NULL;
static DEFINE_MUTEX(nvt_mutex);

// ensure the data flush from AXI bus to dram
void nvt_ipc_flush_shm_data(void)
{
	//DSB to clear barriar on AXI bus and add dummy read operation
	__asm__ __volatile__("dsb\n\t");
	//(*((volatile UINT32*)(nvt_ipc_get_noncache_addr(m_ctrl->phy_base))));
	(*((volatile UINT32 *)m_ctrl->shm_non_cache_base));
}


static void xNvtIpc_SetCore1Error(INT32 err, UINT32 CMD)
{
	NVTIPC_SHM_CTRL    *pShmCtrl = NvtIpcMsgQue_GetShmCtrl();
	pShmCtrl->Cpu1Cpu2Err = err;
	pShmCtrl->Cpu1Cpu2Key = CMD;
	nvt_ipc_flush_shm_data();
	NvtIpc_Task_SetErrCode(err, CMD);
}

static void xNvtIpc_SetCore3Error(INT32 err, UINT32 CMD)
{
	NVTIPC_SHM_CTRL    *pShmCtrl = NvtIpcMsgQue_GetShmCtrl();
	pShmCtrl->Cpu3Cpu2Err = err;
	pShmCtrl->Cpu3Cpu2Key = CMD;
	nvt_ipc_flush_shm_data();
	NvtIpc_Task_SetErrCode(err, CMD);
}

static void xNvtIpc_SetCore4Error(INT32 err, UINT32 CMD)
{
	NVTIPC_SHM_CTRL    *pShmCtrl = NvtIpcMsgQue_GetShmCtrl();
	pShmCtrl->Cpu4Cpu2Err = err;
	pShmCtrl->Cpu4Cpu2Key = CMD;
	nvt_ipc_flush_shm_data();
	NvtIpc_Task_SetErrCode(err, CMD);
}

unsigned int nvt_ipc_get_dram1_size(void)
{
	return m_ctrl->ddr1_size;
}

unsigned int nvt_ipc_get_dram2_size(void)
{
	return m_ctrl->ddr2_size;
}





unsigned int nvt_ipc_get_noncache_addr(unsigned int phy_addr)
{
	unsigned int noncache_addr;
	if (phy_addr < m_ctrl->shm_phy_base || phy_addr > m_ctrl->shm_phy_base + m_ctrl->shm_size) {
		DBG_ERR("phy_addr 0x%x < uItron base 0x%x\r\n", phy_addr, m_ctrl->shm_phy_base);
		return 0;
	}
	noncache_addr = (unsigned int)(phy_addr - m_ctrl->shm_phy_base + m_ctrl->shm_non_cache_base);
	//DBG_DUMP("nvt_ipc_get_noncache_addr \r\n");
	//DBGH(phy_addr);
	//DBGH(noncache_addr);
	return noncache_addr;
}
#if __VOS_IPC__
EXPORT_SYMBOL(nvt_ipc_get_noncache_addr);
#endif

unsigned int nvt_ipc_get_phy_addr(unsigned int noncache_addr)
{
	unsigned int phy_addr;
	if (noncache_addr < (unsigned int)m_ctrl->shm_non_cache_base || noncache_addr > (unsigned int)m_ctrl->shm_non_cache_base + m_ctrl->shm_size) {
		DBG_ERR("noncache_addr 0x%x out-of-bound 0x%x~0x%x\r\n", noncache_addr, (unsigned int)m_ctrl->shm_non_cache_base, (unsigned int)m_ctrl->shm_non_cache_base + m_ctrl->shm_size);
		return 0;
	}
	phy_addr = (unsigned int)(noncache_addr - (unsigned int)m_ctrl->shm_non_cache_base + m_ctrl->shm_phy_base);
	//DBG_DUMP("nvt_ipc_get_phy_addr \r\n");
	//DBGH(noncache_addr);
	//DBGH(phy_addr);
	return phy_addr;
}
#if __VOS_IPC__
EXPORT_SYMBOL(nvt_ipc_get_phy_addr);
#endif



static void xNvtIpc_CoreHandleFunc(CC_GETCMD getCmdFunc, CC_ACKCMD ackCmdFunc, CC_SETERR setErrFunc)
{
#if __VOS_IPC__
	UINT32               coreId;
#endif
	INT32                msqid;
	NVTIPC_ER            ret = NVTIPC_ER_OK;
	CC_CMD               ccCmd;
	wait_queue_head_t    *pWaitq;

	getCmdFunc(&ccCmd);
	msqid = NvtIpcMsgQue_GetIDbyKey((INT32)ccCmd.uiCMDOperation);
	if (unlikely(msqid < 0)) {
		ret = NVTIPC_ER_MSGQUE_ID;
		goto CoreHandleErr;
	}
	ret = NvtIPC_MsgPost(msqid, (void *)ipc_getNonCacheAddr((UINT32)ccCmd.pData), (UINT32)ccCmd.uiDataSize);
	if (unlikely(ret != NVTIPC_ER_OK)) {
		goto CoreHandleErr;
	}
	ackCmdFunc();
#if __VOS_IPC__
	coreId = (ccCmd.uiCMDID & 0x0F);
	//DBG_DUMP("coreId = %d\r\n",coreId);
	if (coreId == __CORE1) {
		pWaitq = &m_ctrl->nvt_readWaitq_c1[msqid];
	} else {
		pWaitq = &m_ctrl->nvt_readWaitq[msqid];
	}

#else
	pWaitq = &m_ctrl->nvt_readWaitq[msqid];
#endif
	wake_up_interruptible(pWaitq);
	return;

CoreHandleErr:
	// should set error before ack
	setErrFunc(ret, ccCmd.uiCMDOperation);
	// ack to other cpu
	ackCmdFunc();
	wake_up_interruptible(&m_ctrl->nvt_errWaitq);
}

static void xNvtIpc_Core1Handler(CC_U32 uiEvent)
{
	xNvtIpc_CoreHandleFunc(cc_get_core1_cmd, cc_ack_core1_cmd, xNvtIpc_SetCore1Error);
}

static void xNvtIpc_Core3Handler(CC_U32 uiEvent)
{
	xNvtIpc_CoreHandleFunc(cc_get_core3_cmd, cc_ack_core3_cmd, xNvtIpc_SetCore3Error);
}

static void xNvtIpc_Core4Handler(CC_U32 uiEvent)
{
	xNvtIpc_CoreHandleFunc(cc_get_core4_cmd, cc_ack_core4_cmd, xNvtIpc_SetCore4Error);
}



#ifndef CONFIG_SMP
static void nvt_ipc_SendPowerOnReady(void)
{
	// Send Power on ready to CPU1
	NVTIPC_CPU2_READY_MSG   ipcMsg;
	NVTIPC_SHM_CTRL    *pShmCtrl = NvtIpcMsgQue_GetShmCtrl();

	DBG_DUMP("wait cpu1 power on \r\n");
	while (pShmCtrl->Cpu1Sts != NVTIPC_CPUSTS_POWERON_READY) {
		;
	}
	pShmCtrl->Cpu2Sts = NVTIPC_CPUSTS_POWERON_READY;
	ipcMsg.sysCmdID = NVTIPC_SYSCMD_CPU2_POWERON_READY;
	DBG_DUMP("nvt_ipc_SendPowerOnReady \r\n");
	nvt_ipc_msg_snd(NVTIPC_SYS_QUEUE_ID, NVTIPC_SENDTO_CORE1, &ipcMsg, sizeof(ipcMsg));
}
#endif

#if 0
static void nvt_ipc_SendResumeReady(void)
{
	// Send Resume ready to CPU1
	NVTIPC_CPU2_READY_MSG   ipcMsg;
	ipcMsg.sysCmdID = NVTIPC_SYSCMD_CPU2_RESUME_READY;
	nvt_ipc_msg_snd(NVTIPC_SYS_QUEUE_ID, NVTIPC_SENDTO_CORE1, &ipcMsg, sizeof(ipcMsg));
}
#endif

static int nvt_ioctl(struct file *file, unsigned int cmd, unsigned long arg)
{
	int __user *argp = (int __user *)arg;

	DBG_IND("cmd = 0x%x\r\n", cmd);
	switch (cmd) {

	case NVTIOC_IPC_MSGQUE_GET: {
			int                     msqid;
			NVTIPC_IOC_MSGQUE_GET_S msg = {0};
			struct nvt_ipc_device  *pDev = file->private_data;

			if (unlikely(copy_from_user(&msg, argp, sizeof(msg)))) {
				DBG_ERR("%s\r\n", COPY_FROM_USER_ERR_STR);
				return -EFAULT;
			}
			msqid = NvtIpcMsgQue_Get(msg.key);
			if (likely(msqid >= 0)) {
				mutex_lock(&nvt_mutex);
				pDev->ReqQueueBits |= (1 << msqid);
				mutex_unlock(&nvt_mutex);
			}
			msg.rtn = msqid;
			if (unlikely(copy_to_user(argp, &msg, sizeof(msg)))) {
				DBG_ERR("%s\r\n", COPY_TO_USER_ERR_STR);
				return -EFAULT;
			}
		}
		break;
	case NVTIOC_IPC_MSGQUE_REL: {
			NVTIPC_IOC_MSGQUE_REL_S msg = {0};
			struct nvt_ipc_device *pDev = file->private_data;

			if (unlikely(copy_from_user(&msg, argp, sizeof(msg)))) {
				DBG_ERR("%s\r\n", COPY_FROM_USER_ERR_STR);
				return -EFAULT;
			}
			msg.rtn = NvtIpcMsgQue_Rel(msg.msqid);
			if (likely(msg.rtn >= 0)) {
				mutex_lock(&nvt_mutex);
				pDev->ReqQueueBits &= ~(1 << msg.msqid);
				mutex_unlock(&nvt_mutex);
				// wake up the waiting process
				wake_up_interruptible(&m_ctrl->nvt_readWaitq[msg.msqid]);
			}
			if (unlikely(copy_to_user(argp, &msg, sizeof(msg)))) {
				DBG_ERR("%s\r\n", COPY_TO_USER_ERR_STR);
				return -EFAULT;
			}
		}
		break;
	case NVTIOC_IPC_CONSOLE_CMD: {
			NVTIPC_IOC_MEM_BUF buf = {0};
			if (unlikely(copy_from_user(&buf, argp, sizeof(buf)))) {
				DBG_ERR("%s\r\n", COPY_FROM_USER_ERR_STR);
				return -EFAULT;
			}
			//nvt_tty_run_cmd(buf.addr,buf.size-1);
			//nvt_tty_run_cmd("\r",1);
		}
		break;
	case NVTIOC_IPC_SYS_CMD: {
			unsigned int cmdID;
			if (unlikely(copy_from_user(&cmdID, argp, sizeof(cmdID)))) {
				DBG_ERR("%s\r\n", COPY_FROM_USER_ERR_STR);
				return -EFAULT;
			}
			switch (cmdID) {
			case NVTIPC_SYSCMD_CPU2_POWERON_READY:

				break;
			case NVTIPC_SYSCMD_SYSCALL_ACK:
				NvtIpc_SysCmdAck(cmdID);
				break;
			case NVTIPC_SYSCMD_UART_ACK:
				NvtIpc_SysCmdAck(cmdID);
				break;
			case NVTIPC_SYSCMD_IPC_DUMPINFO:
				NvtIpcMsgQue_DumpDebugInfo();
				break;
			case NVTIPC_SYSCMD_SUSPEND_REQ:
				nvt_ipc_sig_send(NVTIPC_SIG_SUSPEND);
				if (nvt_ipc_sig_wait_all_ack(NVTIPC_SIG_SUSPEND, NVTIPC_SIG_SUSPEND_TIMEOUT_SEC) == -ERESTARTSYS)
					return -ERESTARTSYS;
				break;
			case NVTIPC_SYSCMD_GET_LONG_COUNTER_ACK:
				NvtIpc_SysCmdAck(cmdID);
				break;
			default:
				CHKPNT;
				DBG_ERR("Unknown syscmd %d\r\n", cmdID);
				return -EFAULT;
			}

		}
		break;
	case NVTIOC_IPC_FTOK: {
			NVTIPC_IOC_FTOK_S msg = {{0}, 0};

			if (unlikely(copy_from_user(&msg, argp, sizeof(msg)))) {
				DBG_ERR("%s\r\n", COPY_FROM_USER_ERR_STR);
				return -EFAULT;
			}
			msg.rtn = nvt_ipc_ftok(msg.path);
			if (unlikely(copy_to_user(argp, &msg, sizeof(msg)))) {
				DBG_ERR("%s\r\n", COPY_TO_USER_ERR_STR);
				return -EFAULT;
			}
		}
		break;
	case NVTIOC_IPC_MSG_SND: {
			NVTIPC_IOC_MSG_SND_S msg = {0};
			if (unlikely(copy_from_user(&msg, argp, sizeof(msg)))) {
				DBG_ERR("%s\r\n", COPY_FROM_USER_ERR_STR);
				return -EFAULT;
			}
			msg.rtn = nvt_ipc_msg_sndEx(__CORE2, msg.msqid, msg.sendTo, msg.pMsg, msg.msgsz, 1);
			if (unlikely(copy_to_user(argp, &msg, sizeof(msg)))) {
				DBG_ERR("%s\r\n", COPY_TO_USER_ERR_STR);
				return -EFAULT;
			}
		}
		break;
	case NVTIOC_IPC_MSG_RCV: {
			NVTIPC_IOC_MSG_RCV_S msg = {0};
			if (unlikely(copy_from_user(&msg, argp, sizeof(msg)))) {
				DBG_ERR("%s\r\n", COPY_FROM_USER_ERR_STR);
				return -EFAULT;
			}
			msg.rtn = nvt_ipc_msg_rcv(msg.msqid, msg.pMsg, msg.msgsz);
			if (msg.rtn == -ERESTARTSYS) {
				return -ERESTARTSYS;
			}
			if (unlikely(copy_to_user(argp, &msg, sizeof(msg)))) {
				DBG_ERR("%s\r\n", COPY_TO_USER_ERR_STR);
				return -EFAULT;
			}
		}
		break;
	case NVTIOC_IPC_GET_DRAM1_RANGE: {
			NVTIPC_IOC_MEM_BUF msg;

			msg.addr = (void *)m_ctrl->ddr1_addr;
			msg.size = nvt_ipc_get_dram1_size();
			if (unlikely(copy_to_user(argp, &msg, sizeof(msg)))) {
				DBG_ERR("%s\r\n", COPY_TO_USER_ERR_STR);
				return -EFAULT;
			}
		}
		break;
	case NVTIOC_IPC_GET_DRAM2_RANGE: {
			NVTIPC_IOC_MEM_BUF msg;

			msg.addr = (void *)m_ctrl->ddr2_addr;
			msg.size = nvt_ipc_get_dram2_size();
			if (unlikely(copy_to_user(argp, &msg, sizeof(msg)))) {
				DBG_ERR("%s\r\n", COPY_TO_USER_ERR_STR);
				return -EFAULT;
			}
		}
		break;
	case NVTIOC_IPC_FLUSH_CACHE: {
			NVTIPC_IOC_FLUSH_CACHE_S msg = {0};
			if (unlikely(copy_from_user(&msg, argp, sizeof(msg)))) {
				DBG_ERR("%s\r\n", COPY_FROM_USER_ERR_STR);
				return -EFAULT;
			}
			dmac_flush_range((void *)msg.addr, (void *)(msg.addr + msg.size));
			//dmac_unmap_area((void*)msg.addr, msg.size, DMA_FROM_DEVICE);

		}
		break;
#if __VOS_IPC__
	case NVTIOC_IPC_FTOK_C1: {
			NVTIPC_IOC_FTOK_S msg = {{0}, 0};

			if (unlikely(copy_from_user(&msg, argp, sizeof(msg)))) {
				DBG_ERR("%s\r\n", COPY_FROM_USER_ERR_STR);
				return -EFAULT;
			}
			msg.rtn = NvtIpcMsgQue_FtokEx(__CORE1, msg.path);
			if (unlikely(copy_to_user(argp, &msg, sizeof(msg)))) {
				DBG_ERR("%s\r\n", COPY_TO_USER_ERR_STR);
				return -EFAULT;
			}
		}
		break;
	case NVTIOC_IPC_MSGQUE_GET_C1: {
			int                        msqid;
			NVTIPC_IOC_MSGQUE_GET_S msg = {0};
			struct nvt_ipc_device  *pDev = file->private_data;

			if (unlikely(copy_from_user(&msg, argp, sizeof(msg)))) {
				DBG_ERR("%s\r\n", COPY_FROM_USER_ERR_STR);
				return -EFAULT;
			}
			msqid = NvtIpcMsgQue_GetEx(__CORE1, msg.key);
			if (likely(msqid >= 0)) {
				mutex_lock(&nvt_mutex);
				pDev->ReqQueueBits_c1 |= (1 << msqid);
				mutex_unlock(&nvt_mutex);
			}
			msg.rtn = msqid;
			if (unlikely(copy_to_user(argp, &msg, sizeof(msg)))) {
				DBG_ERR("%s\r\n", COPY_TO_USER_ERR_STR);
				return -EFAULT;
			}
		}
		break;
	case NVTIOC_IPC_MSGQUE_REL_C1: {
			NVTIPC_IOC_MSGQUE_REL_S msg = {0};
			struct nvt_ipc_device *pDev = file->private_data;

			if (unlikely(copy_from_user(&msg, argp, sizeof(msg)))) {
				DBG_ERR("%s\r\n", COPY_FROM_USER_ERR_STR);
				return -EFAULT;
			}
			msg.rtn = NvtIpcMsgQue_RelEx(__CORE1, msg.msqid);
			if (likely(msg.rtn >= 0)) {
				mutex_lock(&nvt_mutex);
				pDev->ReqQueueBits_c1 &= ~(1 << msg.msqid);
				mutex_unlock(&nvt_mutex);
				// wake up the waiting process
				wake_up_interruptible(&m_ctrl->nvt_readWaitq_c1[msg.msqid]);
			}
			if (unlikely(copy_to_user(argp, &msg, sizeof(msg)))) {
				DBG_ERR("%s\r\n", COPY_TO_USER_ERR_STR);
				return -EFAULT;
			}
		}
		break;
	case NVTIOC_IPC_MSG_SND_C1: {
			NVTIPC_IOC_MSG_SND_S msg = {0};
			if (unlikely(copy_from_user(&msg, argp, sizeof(msg)))) {
				DBG_ERR("%s\r\n", COPY_FROM_USER_ERR_STR);
				return -EFAULT;
			}
			msg.rtn = nvt_ipc_msg_sndEx(__CORE1, msg.msqid, msg.sendTo, msg.pMsg, msg.msgsz, 1);
			if (unlikely(copy_to_user(argp, &msg, sizeof(msg)))) {
				DBG_ERR("%s\r\n", COPY_TO_USER_ERR_STR);
				return -EFAULT;
			}
		}
		break;
	case NVTIOC_IPC_MSG_RCV_C1: {
			NVTIPC_IOC_MSG_RCV_S msg = {0};
			if (unlikely(copy_from_user(&msg, argp, sizeof(msg)))) {
				DBG_ERR("%s\r\n", COPY_FROM_USER_ERR_STR);
				return -EFAULT;
			}
			msg.rtn = nvt_ipc_msg_rcvEx(__CORE1, msg.msqid, msg.pMsg, msg.msgsz);
			if (msg.rtn == -ERESTARTSYS) {
				return -ERESTARTSYS;
			}
			if (unlikely(copy_to_user(argp, &msg, sizeof(msg)))) {
				DBG_ERR("%s\r\n", COPY_TO_USER_ERR_STR);
				return -EFAULT;
			}
		}
		break;
#endif
#if 1
	case NVTIOC_IPC_SEM_LOCK: {
			UINT32 semid;
			//struct nvt_ipc_device* pDev = file->private_data;

			if (unlikely(copy_from_user(&semid, argp, sizeof(semid)))) {
				DBG_ERR("%s\r\n", COPY_FROM_USER_ERR_STR);
				return -EFAULT;
			}
#if 0
			if (semid < NVTIPC_SEM_NUM) {
				mutex_lock(&nvt_mutex);
				pDev->ReqSemBits |= (1 << semid);
				mutex_unlock(&nvt_mutex);
				nvt_ipc_sem_lock(semid);
			}
#else
			nvt_ipc_sem_lock(semid);
#endif
		}
		break;
	case NVTIOC_IPC_SEM_UNLOCK: {
			UINT32 semid;
			//struct nvt_ipc_device* pDev = file->private_data;

			if (unlikely(copy_from_user(&semid, argp, sizeof(semid)))) {
				DBG_ERR("%s\r\n", COPY_FROM_USER_ERR_STR);
				return -EFAULT;
			}
#if 0
			if (semid < NVTIPC_SEM_NUM) {
				nvt_ipc_sem_unlock(semid);
				mutex_lock(&nvt_mutex);
				pDev->ReqSemBits &= ~(1 << semid);
				mutex_unlock(&nvt_mutex);
			}
#else
			nvt_ipc_sem_unlock(semid);
#endif

		}
		break;
#endif
		case NVTIOC_IPC_SIG_WAIT: {
			NVTIOC_IPC_SIG_WAIT_S msg = {0};

			if (unlikely(copy_from_user(&msg, argp, sizeof(msg)))) {
				DBG_ERR("%s\r\n", COPY_FROM_USER_ERR_STR);
				return -EFAULT;
			}
			msg.rtn = nvt_ipc_sig_wait();
			if (msg.rtn == -ERESTARTSYS) {
				return -ERESTARTSYS;
			}
			// keep wait ack info


			if (unlikely(copy_to_user(argp, &msg, sizeof(msg)))) {
				DBG_ERR("%s\r\n", COPY_TO_USER_ERR_STR);
				return -EFAULT;
			}
		}
		break;

		case NVTIOC_IPC_SIG_ACK: {
			NVTIOC_IPC_SIG_ACK_S msg = {0};

			if (unlikely(copy_from_user(&msg, argp, sizeof(msg)))) {
				DBG_ERR("%s\r\n", COPY_FROM_USER_ERR_STR);
				return -EFAULT;
			}
			msg.rtn = nvt_ipc_sig_ack(msg.sig);
			if (unlikely(copy_to_user(argp, &msg, sizeof(msg)))) {
				DBG_ERR("%s\r\n", COPY_TO_USER_ERR_STR);
				return -EFAULT;
			}
		}
		break;


	default:
		DBG_ERR("nvt-ipc Unknown cmd 0x%x\r\n", cmd);
		return -EINVAL;
	}
	return 0;
}




static long nvt_unlocked_ioctl(struct file *file, unsigned int cmd, unsigned long arg)
{
	int ret;

	ret = nvt_ioctl(file, cmd, arg);

	return ret;
}


static int nvt_ipc_open(struct inode *inode, struct file *file)
{
	struct nvt_ipc_device *pDev;

	pDev = kzalloc(sizeof(struct nvt_ipc_device), GFP_KERNEL);
	if (!pDev) {
		return -ENOMEM;
	}
	pDev->m_ctrl = m_ctrl;
	file->private_data = pDev;
	return 0;
}

static int nvt_ipc_release(struct inode *inode, struct file *file)
{
	struct nvt_ipc_device *pDev = file->private_data;
	char  *token;
	int    i;
	NVTIPC_MSGQUE  *pMsgQue;


	mutex_lock(&nvt_mutex);
	for (i = 0; i < NVTIPC_MSG_QUEUE_NUM ; i++) {
		if (pDev->ReqQueueBits & (1 << i)) {
			pMsgQue = NvtIpcMsgQue_GetQuebyIDEx(__CORE2, i);
			token = NvtIpcMsgQue_GetTokenByKey(__CORE2, pMsgQue->uiSharedKey);
			if (token) {
				DBG_DUMP("Rel Core2 NvtIpcMsgQue %d, token=%s\r\n", i, token);
			} else {
				DBG_DUMP("Rel Core2 NvtIpcMsgQue %d, \r\n", i);
			}
			NvtIpcMsgQue_RelEx(__CORE2, i);
		}
#if __VOS_IPC__
		if (pDev->ReqQueueBits_c1 & (1 << i)) {
			pMsgQue = NvtIpcMsgQue_GetQuebyIDEx(__CORE1, i);
			token = NvtIpcMsgQue_GetTokenByKey(__CORE1, pMsgQue->uiSharedKey);
			if (token) {
				DBG_DUMP("Rel Core1 NvtIpcMsgQue %d, token=%s\r\n", i, token);
			} else {
				DBG_DUMP("Rel Core1 NvtIpcMsgQue %d, \r\n", i);
			}
			NvtIpcMsgQue_RelEx(__CORE1, i);
		}
#endif

	}
	nvt_ipc_sig_rm_pid_from_wait_list(current->pid);
#if 0
	for (i = 0; i < NVTIPC_SEM_NUM ; i++) {
		if (pDev->ReqSemBits & (1 << i)) {
			nvt_ipc_sem_unlock(i);
			DBG_DUMP("Rel NvtIpcSem %d\r\n", i);
		}
	}
#endif
	kfree(pDev);
	mutex_unlock(&nvt_mutex);
	return 0;
}
static int nvt_ipc_mmap(struct file *filep, struct vm_area_struct *vma)
{
	if (vma->vm_end < vma->vm_start) {
		DBG_ERR("vm_start 0x%x > vm_end 0x%x", (unsigned int)vma->vm_start, (unsigned int)vma->vm_end);
		return -EINVAL;
	}
	DBG_IND("vm_start 0x%x , vm_end 0x%x, vm_pgoff 0x%x\r\n", (unsigned int)vma->vm_start, (unsigned int)vma->vm_end, (unsigned int)vma->vm_pgoff);

	vma->vm_flags |= VM_IO | VM_DONTEXPAND | VM_DONTDUMP;




	if ((vma->vm_pgoff << PAGE_SHIFT) & NONCACHE_FLAG) {
		vma->vm_page_prot = pgprot_noncached(vma->vm_page_prot);
		if (((vma->vm_pgoff << PAGE_SHIFT) & m_ctrl->mmio_phy_base) != m_ctrl->mmio_phy_base) {
			vma->vm_pgoff -= (NONCACHE_FLAG >> PAGE_SHIFT);
		}
		DBG_IND("map noncache, vm_pgoff 0x%x\r\n", (unsigned int)vma->vm_pgoff);
	} else {
		//vma->vm_page_prot = __pgprot((pgprot_val(vma->vm_page_prot)&~_CACHE_MASK) | _CACHE_CACHABLE_NONCOHERENT);
		DBG_IND("map cache\r\n");
	}
	return remap_pfn_range(vma,
						   vma->vm_start,
						   vma->vm_pgoff,
						   vma->vm_end - vma->vm_start,
						   vma->vm_page_prot);
}

#if 0
static unsigned int nvt_ipc_poll(struct file *file, poll_table *wait)
{
	int mask = 0;
	struct nvt_ipc_device *pdev = (struct nvt_ipc_device *)file->private_data;

	poll_wait(file, &pdev->m_ctrl->nvt_pm_poll_wait_queue_read, wait);
	if (pdev->m_ctrl->nvt_pm_notifier_flag) {
		mask |= POLLIN | POLLRDNORM;
		pdev->m_ctrl->nvt_pm_notifier_flag = 0;
		//pdev->m_ctrl->nvt_pm_notifier_read_flag = 1;
	}

	return mask;
}

static ssize_t nvt_ipc_read(struct file *file, char __user *buf,
							size_t count, loff_t *ppos)
{
	int ret = 0;
	struct nvt_ipc_device *pdev = (struct nvt_ipc_device *)file->private_data;

	ret = wait_event_interruptible(pdev->m_ctrl->nvt_pm_poll_wait_queue_read,
								   pdev->m_ctrl->nvt_pm_notifier_read_flag == 0);

	mutex_lock(&nvt_mutex);
	if (pdev->m_ctrl->nvt_pm_notifier_read_flag == 0) {
		pr_info("NVTIPC normal read: %s %d\n", __func__, __LINE__);
	} else {
		pr_info("NVTIPC PM read: %s %d flag=%d \n", __func__, __LINE__, pdev->m_ctrl->nvt_pm_notifier_read_flag);
		ret = copy_to_user(buf, &pdev->m_ctrl->nvt_pm_notifier_read_flag, sizeof(pdev->m_ctrl->nvt_pm_notifier_read_flag));
		pdev->m_ctrl->nvt_pm_notifier_read_flag = 0;
		mutex_unlock(&nvt_mutex);
		if (ret < 0) {
			return -EFAULT;
		} else {
			return sizeof(pdev->m_ctrl->nvt_pm_notifier_read_flag);
		}
	}
	mutex_unlock(&nvt_mutex);

	return 0;
}
#endif

static const struct file_operations nvt_fops = {
	.owner              = THIS_MODULE,
	.unlocked_ioctl     = nvt_unlocked_ioctl,
	.open               = nvt_ipc_open,
	.release            = nvt_ipc_release,
	.mmap               = nvt_ipc_mmap,
	#if 0
	.poll               = nvt_ipc_poll,
	.read               = nvt_ipc_read,
	#endif
};

static struct class *nvt_class = NULL;
static struct device *nvt_device = NULL;

#ifdef CONFIG_OF

static const struct of_device_id nvt_ipc_of_match[];

static int nvt_ipc_parse_dt(struct platform_device *pdev)
{
	const struct of_device_id *of_id;
	struct device_node *node = pdev->dev.of_node;
	struct resource *res;
	CC_DEVICE  cc_device;

	DBG_IND("nvt_ipc_parse_dt \r\n");

	if (unlikely(!node)) {
		return -EINVAL;
	}
	of_id = of_match_node(nvt_ipc_of_match, node);
	if (unlikely(!of_id)) {
		return -EINVAL;
	}
	res = platform_get_resource(pdev, IORESOURCE_MEM, 0);

	DBG_IND("IORESOURCE_MEM start=0x%x, end = 0x%x \r\n", res->start, res->end);

#if 0
	cc_device.reg_base = devm_ioremap_resource(&pdev->dev, res);
	if (IS_ERR(cc_device.reg_base)) {
		return PTR_ERR(cc_device.reg_base);
	}
#else
	cc_device.reg_base = NVT_CC_BASE_VIRT;
#endif
	DBG_IND("cc_device.reg_base=0x%x\r\n", cc_device.reg_base);
	res = platform_get_resource(pdev, IORESOURCE_IRQ, 0);
	if (unlikely(res == NULL)) {
		dev_err(&pdev->dev, "Failed to get IRQ resource\n");
		return -ENXIO;
	}
	DBG_IND("IORESOURCE_IRQ start=%d\r\n", res->start);
	cc_device.irq = res->start;
	cc_config(&cc_device);
	m_ctrl->cc_reg_base = (unsigned int)cc_device.reg_base;

#if 0
	if (unlikely(of_property_read_u32(node, "mmio_base", &m_ctrl->mmio_phy_base))) {
		return -EINVAL;
	}
#else
	m_ctrl->mmio_phy_base = NVT_PERIPHERAL_PHYS_BASE;
#endif
	DBG_IND("mmio_base=0x%x\r\n", m_ctrl->mmio_phy_base);
	return 0;
}
#endif /* CONFIG_OF */

static int nvt_ipc_probe(struct platform_device *pdev)
{
	int ret = 0, i;
	NVTIPC_SHM_CTRL    *pShmCtrl;

	DBG_IND("nvt_ipc_probe \r\n");

	m_ctrl = kzalloc(sizeof(struct nvt_ipc_ctrl), GFP_KERNEL);
	if (!m_ctrl) {
		return -ENOMEM;
	}

	platform_set_drvdata(pdev, m_ctrl);

	if (unlikely(nvt_ipc_parse_dt(pdev) < 0)) {
		DBG_ERR("parse_dt %d\r\n", ret);
		goto initErr1;
	}
	m_ctrl->major = register_chrdev(0, DEVICE_NAME, &nvt_fops);
	if (m_ctrl->major < 0) {
		DBG_ERR("register a device fail.\r\n");
		ret = -1;
		goto initErr1;
	}
	DBG_IND("major=%d\n", m_ctrl->major);
	nvt_class = class_create(THIS_MODULE, DEVICE_NAME);
	if (IS_ERR(nvt_class)) {
		DBG_ERR("creating class fail.\r\n");
		ret = -1;
		goto initErr2;
	}
	nvt_device = device_create(nvt_class, NULL, MKDEV(m_ctrl->major, 0), NULL, DEVICE_NAME);
	if (IS_ERR(nvt_device)) {
		DBG_ERR("creating device fail.\r\n");
		ret = -1;
		goto initErr3;
	}
	// init wait queue
	for (i = 0; i < NVTIPC_MSG_QUEUE_NUM; i++) {
		init_waitqueue_head(&m_ctrl->nvt_readWaitq[i]);
#if __VOS_IPC__
		init_waitqueue_head(&m_ctrl->nvt_readWaitq_c1[i]);
#endif
	}
	init_waitqueue_head(&m_ctrl->nvt_errWaitq);

	// nvt pm notifier init.
	#if 0
	init_waitqueue_head(&m_ctrl->nvt_pm_poll_wait_queue_read);
	m_ctrl->nvt_pm_notifier_flag = 0;
	#endif

    // init ipc signal contrl
	nvt_ipc_sig_init(&m_ctrl->sig_ctrl);

	// cc driver init
	cc_open();
	cc_register_core1_handler(xNvtIpc_Core1Handler);
	cc_register_core3_handler(xNvtIpc_Core3Handler);
	cc_register_core4_handler(xNvtIpc_Core4Handler);
	// ipc message queue init
	if (NvtIpcMsgQue_Init() < 0) {
		ret = -1;
		goto initErr4;
	}
	// get ipc sys queue
	NvtIpcMsgQue_Get(NVTIPC_SYS_KEY);
	// get ipc sem queue
	NvtIpcMsgQue_Get(NvtIpcMsgQue_Ftok(IPC_SEM_QUEUE_TOKEN_STR));
#if __VOS_IPC__
	// get ipc sys queue
	NvtIpcMsgQue_GetEx(__CORE1, NVTIPC_SYS_KEY);
	// get ipc sem queue
	NvtIpcMsgQue_GetEx(__CORE1, NvtIpcMsgQue_FtokEx(__CORE1, IPC_SEM_QUEUE_TOKEN_STR));
#endif
	// init task
	if (NvtIpc_Task_Init() < 0) {
		ret = -1;
		goto initErr5;
	}
	// set dram info
	pShmCtrl = NvtIpcMsgQue_GetShmCtrl();
	m_ctrl->ddr1_addr = pShmCtrl->ddr1_addr;
	m_ctrl->ddr1_size = pShmCtrl->ddr1_size;
	m_ctrl->uitron_addr = pShmCtrl->uitron_addr;
	m_ctrl->uitron_size = pShmCtrl->uitron_size;
	m_ctrl->ddr2_addr = pShmCtrl->ddr2_addr;
	m_ctrl->ddr2_size = pShmCtrl->ddr2_size;
	DBG_DUMP("Dram1 size 0x%x\r\n", nvt_ipc_get_dram1_size());
	DBG_DUMP("Dram2 size 0x%x\r\n", nvt_ipc_get_dram2_size());
#if 0
	m_ctrl->non_cache_base = ioremap_nocache(m_ctrl->uitron_addr, m_ctrl->uitron_size);
	if (!m_ctrl->non_cache_base) {
		DBG_ERR("ioremap failed\n");
		ret = -1;
		goto initErr5;
	}
#else
	m_ctrl->shm_phy_base = NVTIPC_SHARED_MEM_ADDR;
	m_ctrl->shm_non_cache_base = (unsigned int)pShmCtrl;
	m_ctrl->shm_size = NVTIPC_SHARED_MEM_ADDR;
#endif
#ifndef CONFIG_SMP
	nvt_ipc_SendPowerOnReady();
#endif
	return 0;
initErr5:
	NvtIpc_Task_Exit();
initErr4:
	NvtIpcMsgQue_Exit();
	cc_close();
	device_del(nvt_device);
initErr3:
	class_destroy(nvt_class);
initErr2:
	unregister_chrdev(m_ctrl->major, DEVICE_NAME);
initErr1:
	kfree(m_ctrl);
	return ret;
}

static int nvt_ipc_remove(struct platform_device *pdev)
{
	DBG_IND("nvt_ipc_remove \r\n");

	//iounmap(m_ctrl->non_cache_base);
	NvtIpc_Task_Exit();
	NvtIpcMsgQue_Exit();
	cc_close();
	if (nvt_device) {
		device_del(nvt_device);
	}
	if (nvt_class) {
		class_destroy(nvt_class);
	}
	unregister_chrdev(m_ctrl->major, DEVICE_NAME);
	kfree(m_ctrl);
	return 0;
}

static int nvt_ipc_suspend(struct device *dev)
{
	//DBG_IND("nvt_ipc_suspend state = %d\r\n", (int)state.event);
	NvtIpcMsgQue_chkAllReleased();
	// disable cc irq
	cc_suspend();
	return 0;
}

static int nvt_ipc_resume(struct device *dev)
{
	//struct nvt_ipc_ctrl *p_ipc_ctrl = dev_get_drvdata(dev);

	DBG_IND("nvt_ipc_resume\r\n");
	// enable cc irq
	cc_resume();
#ifndef CONFIG_SMP
	nvt_ipc_SendPowerOnReady();
#endif
    #if 0
	p_ipc_ctrl->nvt_pm_notifier_read_flag = NVTIPC_PM_NOTIFIER_RESUME;
	p_ipc_ctrl->nvt_pm_notifier_flag = 1;
	#endif
	nvt_ipc_sig_send(NVTIPC_SIG_RESUME);
	return 0;
}

static const struct dev_pm_ops nvt_ipc_pm_ops = {
	SET_SYSTEM_SLEEP_PM_OPS(nvt_ipc_suspend, nvt_ipc_resume)
};

#ifdef CONFIG_OF
/* Match table for device tree binding */
static const struct of_device_id nvt_ipc_of_match[] = {
	{ .compatible = "na51000-nvt-ipc",},
	{},
};
MODULE_DEVICE_TABLE(of, nvt_ipc_of_match);
#else
#define nvt_ipc_of_match NULL
#endif

static struct platform_driver nvt_ipc_driver = {
	.probe   =  nvt_ipc_probe,
	.remove  =  nvt_ipc_remove,
	.driver  = {
		.name =  DEVICE_NAME,
		.owner = THIS_MODULE,
		.of_match_table = nvt_ipc_of_match,
		.pm = &nvt_ipc_pm_ops,
	},
};


int __init nvt_ipc_init(void)
{
	DBG_DUMP("nvt_ipc_init \r\n");
	return platform_driver_register(&nvt_ipc_driver);
}



void __exit nvt_ipc_exit(void)
{
	platform_driver_unregister(&nvt_ipc_driver);
}


core_initcall(nvt_ipc_init);
module_exit(nvt_ipc_exit);
MODULE_AUTHOR("Novatek Microelectronics Corp.");
MODULE_LICENSE("GPL v2");
MODULE_DESCRIPTION("nvt-ipc driver for NA51000 SOC");
MODULE_VERSION(DRV_VERSION);
