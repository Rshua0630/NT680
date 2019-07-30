/**

    The internal header file of nvt_ipc driver.

    @file       NvtIpcInt.h
    Copyright   Novatek Microelectronics Corp. 2014.  All rights reserved.

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License version 2 as
    published by the Free Software Foundation.
*/
#ifndef _NVTIPC_INT_H
#define _NVTIPC_INT_H
#include <mach/nvt_type.h>
#include <mach/nvt-ipc.h>
#include <linux/io.h>
#include "cc/cc_reg.h"
#include "nvt_ipc_sig.h"
#include "nvt_ipc_common.h"

//#define NVTIPC_INTERFACE_VER 0x16071419

#define __CORE1  1
#define __CORE2  2
#define __CORE3  3
#define __CORE   __CORE2

#define __COREIPC__                   1

#ifdef CONFIG_NVT_VOS
#define __VOS_UNIT_TEST__             1
#define __VOS_IPC__                   1
#else
#define __VOS_UNIT_TEST__             0
#define __VOS_IPC__                   0
#endif

#define __SYSCMD_WAIT_FINISH__        1

#define __SEMCMD_WAIT_ACK__           0

#define __IPC_PROF_MEASURE__          1

#define __IPC_SEND_BUF_CACHEABLE__    0

#define UNIT_TEST_IPC                 0


#define SYSCALL_MAX_WAIT_TIME            30  // seconds
#define UART_MAX_WAIT_TIME               60  // seconds
#define LONG_COUNTER_MAX_WAIT_TIME       10  // seconds
#define IPC_WAIT_TIMEOUT_CNT             100000000   // cpu cycle -> 10sec
#define IPC_LOCK_SEMBITS_TIMEOUT_CNT     100         // cpu cycle



#define NVTIPC_MSG_QUEUE_NUM             16
#define NVTIPC_MSG_ELEMENT_NUM           16
#define NVTIPC_MSG_ELEMENT_SIZE          24 //  bytes



#define NVTIPC_SYS_CMD_MAX_LEN               256 //  bytes
#define NVTIPC_MSG_QUEUE_TOKEN_STR_MAXLEN    32  //  bytes



#define NVTIPC_SYS_KEY             (0x0000FFFF)
#define IPC_SEM_QUEUE_TOKEN_STR    "ipcsem"
#define NVTIPC_SYS_QUEUE_ID        (0)
#define NVTIPC_SEM_QUEUE_ID        (1)


#define MAKEFOURCC(ch0, ch1, ch2, ch3) ((UINT32)(UINT8)(ch0) | ((UINT32)(UINT8)(ch1) << 8) | ((UINT32)(UINT8)(ch2) << 16) | ((UINT32)(UINT8)(ch3) << 24 ))

#define CFG_NVTIPC_INIT_KEY         MAKEFOURCC('N','I','P','C')

#define SEM_HANDLE                  struct semaphore
#define SEM_CREATE(handle,init_cnt) sema_init(&handle,init_cnt)
#define SEM_SIGNAL(handle)          up(&handle)
#define SEM_WAIT(handle)            down_interruptible(&handle)//down(&handle)
#define SEM_DESTROY(handle)



struct nvt_ipc_ctrl {
	wait_queue_head_t nvt_readWaitq[NVTIPC_MSG_QUEUE_NUM];
#if __VOS_IPC__
	wait_queue_head_t nvt_readWaitq_c1[NVTIPC_MSG_QUEUE_NUM];
#endif
	wait_queue_head_t nvt_errWaitq;
    #if 0
	wait_queue_head_t nvt_pm_poll_wait_queue_read;
	int			      nvt_pm_notifier_flag;
	int               nvt_pm_notifier_read_flag;
	#endif
	int               major;
	unsigned int      shm_phy_base;       ///< share memory physical address
	unsigned int      shm_non_cache_base; ///< share memory non-cache address
	unsigned int      shm_size;           ///< share memory size
	unsigned int      ddr1_addr;          ///< ddr1 starting address.
	unsigned int      ddr1_size;          ///< ddr1 size.
	unsigned int      uitron_addr;        ///< uitron starting address.
	unsigned int      uitron_size;        ///< uitron size.
	unsigned int      ddr2_addr;          ///< ddr2 starting address.
	unsigned int      ddr2_size;          ///< ddr2 size.
	unsigned int      mmio_phy_base;      ///< mmio base address.
	unsigned int      cc_reg_base;        ///< HW CC register base address.
	NVTIPC_SIG_CTRL   sig_ctrl;
};

extern struct nvt_ipc_ctrl *m_ctrl;


#define ipc_getPhyAddr(addr)      nvt_ipc_get_phy_addr(addr)
#define ipc_getNonCacheAddr(addr) nvt_ipc_get_noncache_addr(addr)
#define ipc_storeCache(pBuf,len)  {void *ptr;for (ptr = pBuf;ptr < (pBuf + len+PAGE_SIZE); ptr += PAGE_SIZE){flush_dcache_page(pfn_to_page(PFN_DOWN(virt_to_phys(ptr))));}}


#define CC_REBOOT_ACK_ADDR            (m_ctrl->cc_reg_base+0x11C)
#define CC_POWEROFF_ACK_ADDR          (m_ctrl->cc_reg_base+0x11C)
#define TIMER0_COUNTER_REG            (NVT_PERIPHERAL_VIRT_BASE+0x00040108)



extern void       NvtIpc_SysCmdAck(NVTIPC_SYS_CMD   sysCmd);
extern NVTIPC_I32 nvt_ipc_msg_rcvEx(NVTIPC_U32 coreId, NVTIPC_U32 msqid, void  *pMsg, NVTIPC_U32 msgsz);
extern NVTIPC_I32 nvt_ipc_msg_sndEx(NVTIPC_U32 coreId, NVTIPC_U32 msqid, NVTIPC_SENDTO sendTo, void  *pMsg, NVTIPC_U32 msgsz, NVTIPC_U32 ioctl);
extern void       nvt_ipc_flush_shm_data(void);



#endif //_NVTIPC_INT_H
