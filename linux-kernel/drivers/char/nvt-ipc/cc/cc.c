/**

    Core Communicator module driver

    Core Communicator module driver.

    @file       cc.c
    Copyright   Novatek Microelectronics Corp. 2014.  All rights reserved.

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License version 2 as
    published by the Free Software Foundation.
*/

#include <mach/rcw_macro.h>
#include <mach/cc.h>
#include <mach/nvt_type.h>
#include <mach/irqs.h>
#include "cc_int.h"
#include "cc_reg.h"
//#include <mach/nvt-int.h>
#include <linux/interrupt.h>
#include <linux/signal.h>
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
#include <asm/page.h>
#include <asm/cacheflush.h>

static DEFINE_SPINLOCK(my_lock);
static unsigned long myflags;
#define loc_cpu()   spin_lock_irqsave(&my_lock, myflags)
#define unl_cpu()   spin_unlock_irqrestore(&my_lock, myflags)
//#define loc_cpu() //spin_lock()
//#define unl_cpu() //spin_ulock()

#define CC_SEND_TIMEOUT_CNT   1000000
#define CC_GRANT_TIMEOUT_CNT  1000000
// CC

static  UINT32                cc_irq;

// DUAL OS
//#define NVT_INT_CC            NVT_IRQ_CC2

// SMP
//#define NVT_INT_CC            NVT_IRQ_CC

// Set register base address
//REGDEF_SETIOBASE(IOADDR_CC_REG_BASE);

static UINT32 _REGIOBASE = 0;

static CC_HANDLER       CPU1Handler = 0;
static CC_HANDLER       CPU3Handler = 0;
static CC_HANDLER       CPU4Handler = 0;
static UINT32           cpu1CmdSerialNum;
static UINT32           cpu3CmdSerialNum;
static UINT32           cpu4CmdSerialNum;
//static SEM_HANDLE       ccSemID;      // semaphore id



static irqreturn_t cc_isr(int irq, void *dev_id)
{
	UINT32 ccIntSts;
	RCW_DEF(CC_CPU2STS_REG);
	RCW_DEF(CC_CPU2INTEN_REG);

	DBG_IND("linux cc_isr\r\n");

	RCW_LD(CC_CPU2STS_REG);
	RCW_LD(CC_CPU2INTEN_REG);

	// Filter bits which INTEN are disabled
	RCW_VAL(CC_CPU2STS_REG) &= RCW_VAL(CC_CPU2INTEN_REG);
	ccIntSts = RCW_VAL(CC_CPU2STS_REG);
	// clear interrupt status
	RCW_ST(CC_CPU2STS_REG);

	DBG_IND("ccIntSts=0x%x\r\n", ccIntSts);

	// Event from CPU1
	if (ccIntSts & CC_INT_EVENT_FROM_CPU1) {
		// callback handler
		if (CPU1Handler) {
			CPU1Handler(CC_EVT_FROM_CPU1);
		}
	}
	// Event from CPU2
	if (ccIntSts & CC_INT_EVENT_FROM_CPU2) {
		DBG_ERR("Not support CPU2 -> CPU2 event\r\n");
	}
	// Event from DSP
	if (ccIntSts & CC_INT_EVENT_FROM_DSP) {
		// callback handler
		if (CPU3Handler) {
			CPU3Handler(CC_EVT_FROM_DSP);
		}
	}
	// Event from DSP2
	if (ccIntSts & CC_INT_EVENT_FROM_DSP2) {
		// callback handler
		if (CPU4Handler) {
			CPU4Handler(CC_EVT_FROM_DSP2);
		}
	}
	// allow the interrupt to occur again
	return IRQ_HANDLED;
}

static void cc_creatInt(void)
{
	// create interrupt
	int iret = request_irq(cc_irq, cc_isr, 0, "CC_IRQ", NULL);
	if (iret) {
		printk(KERN_ALERT "Request IRQ[%d] failed!\r\n", cc_irq);
	}

}

static void cc_delInt(void)
{
	free_irq(cc_irq, NULL);
}

static void cc_enInt(void)
{
	RCW_DEF(CC_CPU2INTEN_REG);

	RCW_LD(CC_CPU2INTEN_REG);
	RCW_OF(CC_CPU2INTEN_REG).EventFromCPU1_IntEn = 1;
	RCW_OF(CC_CPU2INTEN_REG).EventFromDSP_IntEn = 1;
	RCW_OF(CC_CPU2INTEN_REG).EventFromDSP2_IntEn = 1;
	RCW_ST(CC_CPU2INTEN_REG);
}

static void cc_disInt(void)
{
	RCW_DEF(CC_CPU2INTEN_REG);

	RCW_LD(CC_CPU2INTEN_REG);
	RCW_OF(CC_CPU2INTEN_REG).EventFromCPU1_IntEn = 0;
	RCW_OF(CC_CPU2INTEN_REG).EventFromDSP_IntEn = 0;
	RCW_OF(CC_CPU2INTEN_REG).EventFromDSP2_IntEn = 0;
	RCW_ST(CC_CPU2INTEN_REG);
}

void cc_register_core1_handler(CC_HANDLER Hdl)
{
	CPU1Handler = Hdl;
}

void cc_register_core3_handler(CC_HANDLER Hdl)
{
	CPU3Handler = Hdl;
}

void cc_register_core4_handler(CC_HANDLER Hdl)
{
	CPU4Handler = Hdl;
}


/**
    set up cc reg base addr and irq number
*/

void cc_config(CC_DEVICE  *cc_device)
{
	_REGIOBASE = (UINT32)cc_device->reg_base;
	cc_irq     = cc_device->irq;

	DBG_IND("cc reg_base = 0x%x\r\n", cc_device->reg_base);
	DBG_IND("cc irq = %d\r\n", cc_device->irq);

}

CC_ER cc_open(void)
{
	RCW_DEF(CC_CPU2STS_REG);

	DBG_IND("cc_open");

	RCW_LD(CC_CPU2STS_REG);
	RCW_OF(CC_CPU2STS_REG).AckFromCPU1 = 0;
	RCW_OF(CC_CPU2STS_REG).AckFromCPU2 = 0;
	RCW_OF(CC_CPU2STS_REG).AckFromDSP  = 0;
	RCW_OF(CC_CPU2STS_REG).AckFromDSP2 = 0;
	RCW_ST(CC_CPU2STS_REG);

	//SEM_CREATE(ccSemID,1);
	// Create cc interrupt
	cc_creatInt();
	// Enable cc interrupt
	cc_enInt();

	// reset value
	cpu1CmdSerialNum = 0;
	cpu3CmdSerialNum = 0;
	cpu4CmdSerialNum = 0;

	return CC_ER_OK;
}

/**
    Close Core Communicator

    Close Core Communicator

    @param[in] xx       xxxxxx
    @return yy
        - @b E_SYS      Something wrong
        - @b E_OK       Everything is oK
*/
CC_ER cc_close(void)
{
	DBG_IND("cc_close");
	// Disable cc interrupt
	cc_disInt();
	// Release cc interrupt
	cc_delInt();

	//SEM_DESTROY(ccSemID);
	return CC_ER_OK;
}


void cc_suspend(void)
{
	disable_irq(cc_irq);
}

void cc_resume(void)
{
	RCW_DEF(CC_CPU2STS_REG);

	DBG_DUMP("cc_resume\r\n");

	RCW_LD(CC_CPU2STS_REG);
	RCW_OF(CC_CPU2STS_REG).AckFromCPU1 = 0;
	RCW_OF(CC_CPU2STS_REG).AckFromCPU2 = 0;
	RCW_OF(CC_CPU2STS_REG).AckFromDSP  = 0;
	RCW_OF(CC_CPU2STS_REG).AckFromDSP2 = 0;
	RCW_ST(CC_CPU2STS_REG);

	// Enable cc interrupt
	cc_enInt();
	enable_irq(cc_irq);
}


CC_ER cc_send_core1_cmd(PCC_CMD pCMD)
{
	UINT32  timeoutCnt = 0;
	RCW_DEF(CC_CPU2STS_REG);
	RCW_DEF(CC_CPU2_CPU1_CMDBUF_REG1);
	RCW_DEF(CC_CPU2_CPU1_CMDBUF_REG2);
	RCW_DEF(CC_CPU2_CPU1_CMDBUF_REG3);
	RCW_DEF(CC_CPU2ACT_REG);

	// wait for status bit ready
#if 1
	do {
		RCW_LD(CC_CPU2STS_REG);
		if (timeoutCnt++ == CC_SEND_TIMEOUT_CNT) {
			DBG_ERR("cc_send_core1_cmd timeout\r\n");
			//SEM_SIGNAL(ccSemID);
			return CC_ER_TMOUT;
		}
	} while (!RCW_OF(CC_CPU2STS_REG).AckFromCPU1);
#endif


	// Enter critical section
	loc_cpu();

	// clear status bit
	RCW_VAL(CC_CPU2STS_REG) = 0;
	RCW_OF(CC_CPU2STS_REG).AckFromCPU1 = 1;
	RCW_ST(CC_CPU2STS_REG);

	DBG_IND("Opcode=0x%x,data=0x%x,size=0x%x\r\n", (int)pCMD->uiCMDOperation, (int)pCMD->pData, (int)pCMD->uiDataSize);

	// set register data
	RCW_OF(CC_CPU2_CPU1_CMDBUF_REG1).CoreID = CC_CORE_CA53_CORE1;
	RCW_OF(CC_CPU2_CPU1_CMDBUF_REG1).SerialID = cpu1CmdSerialNum++;
	RCW_OF(CC_CPU2_CPU1_CMDBUF_REG1).OPCode = pCMD->uiCMDOperation;
	RCW_ST(CC_CPU2_CPU1_CMDBUF_REG1);

	RCW_VAL(CC_CPU2_CPU1_CMDBUF_REG2) = (CC_U32)pCMD->pData;
	RCW_ST(CC_CPU2_CPU1_CMDBUF_REG2);

	RCW_VAL(CC_CPU2_CPU1_CMDBUF_REG3) = (CC_U32)pCMD->uiDataSize;
	RCW_ST(CC_CPU2_CPU1_CMDBUF_REG3);


	// trigger command
	RCW_VAL(CC_CPU2ACT_REG) = 0;
	RCW_OF(CC_CPU2ACT_REG).TriggerCPU1 = 1;
	RCW_ST(CC_CPU2ACT_REG);

	// Leave critical section
	unl_cpu();

	DBG_IND("wait ack begin\r\n");
	// wait ack
#if 0
	do {
		RCW_LD(CC_CPU2STS_REG);
		if (timeoutCnt++ == CC_SEND_TIMEOUT_CNT) {
			DBG_ERR("cc_send_core1_cmd timeout\r\n");
			//SEM_SIGNAL(ccSemID);
			return CC_ER_TMOUT;
		}
	} while (!RCW_OF(CC_CPU2STS_REG).AckFromCPU1);
#endif

	DBG_IND("wait ack end\r\n");
	return CC_ER_OK;
}

CC_ER cc_send_core3_cmd(PCC_CMD pCMD)
{
	UINT32  timeoutCnt = 0;
	RCW_DEF(CC_CPU2STS_REG);
	RCW_DEF(CC_CPU2_DSP_CMDBUF_REG1);
	RCW_DEF(CC_CPU2_DSP_CMDBUF_REG2);
	RCW_DEF(CC_CPU2_DSP_CMDBUF_REG3);
	RCW_DEF(CC_CPU2ACT_REG);

	// wait for status bit ready
#if 1
	do {
		RCW_LD(CC_CPU2STS_REG);
		if (timeoutCnt++ == CC_SEND_TIMEOUT_CNT) {
			DBG_ERR("cc_send_core3_cmd timeout\r\n");
			//SEM_SIGNAL(ccSemID);
			return CC_ER_TMOUT;
		}
	} while (!RCW_OF(CC_CPU2STS_REG).AckFromDSP);
#endif

	// Enter critical section
	loc_cpu();

	// clear status bit
	RCW_VAL(CC_CPU2STS_REG) = 0;
	RCW_OF(CC_CPU2STS_REG).AckFromDSP = 1;
	RCW_ST(CC_CPU2STS_REG);

	DBG_IND("Opcode=0x%x,data=0x%x,size=0x%x\r\n", (int)pCMD->uiCMDOperation, (int)pCMD->pData, (int)pCMD->uiDataSize);

	// set register data
	RCW_OF(CC_CPU2_DSP_CMDBUF_REG1).CoreID = CC_CORE_DSP;
	RCW_OF(CC_CPU2_DSP_CMDBUF_REG1).SerialID = cpu3CmdSerialNum++;
	RCW_OF(CC_CPU2_DSP_CMDBUF_REG1).OPCode = pCMD->uiCMDOperation;
	RCW_ST(CC_CPU2_DSP_CMDBUF_REG1);

	RCW_VAL(CC_CPU2_DSP_CMDBUF_REG2) = (CC_U32)pCMD->pData;
	RCW_ST(CC_CPU2_DSP_CMDBUF_REG2);

	RCW_VAL(CC_CPU2_DSP_CMDBUF_REG3) = (CC_U32)pCMD->uiDataSize;
	RCW_ST(CC_CPU2_DSP_CMDBUF_REG3);


	// trigger command
	RCW_VAL(CC_CPU2ACT_REG) = 0;
	RCW_OF(CC_CPU2ACT_REG).TriggerDSP = 1;
	RCW_ST(CC_CPU2ACT_REG);

	// Leave critical section
	unl_cpu();

	DBG_IND("wait ack begin\r\n");
	// wait ack
#if 0
	do {
		RCW_LD(CC_CPU2STS_REG);
		if (timeoutCnt++ == CC_SEND_TIMEOUT_CNT) {
			DBG_ERR("cc_send_core3_cmd timeout\r\n");
			//SEM_SIGNAL(ccSemID);
			return CC_ER_TMOUT;
		}
	} while (!RCW_OF(CC_CPU2STS_REG).AckFromDSP);
#endif

	DBG_IND("wait ack end\r\n");
	return CC_ER_OK;
}

CC_ER cc_send_core4_cmd(PCC_CMD pCMD)
{
	UINT32  timeoutCnt = 0;
	RCW_DEF(CC_CPU2STS_REG);
	RCW_DEF(CC_CPU2_DSP2_CMDBUF_REG1);
	RCW_DEF(CC_CPU2_DSP2_CMDBUF_REG2);
	RCW_DEF(CC_CPU2_DSP2_CMDBUF_REG3);
	RCW_DEF(CC_CPU2ACT_REG);

	// wait for status bit ready
#if 1
	do {
		RCW_LD(CC_CPU2STS_REG);
		if (timeoutCnt++ == CC_SEND_TIMEOUT_CNT) {
			DBG_ERR("cc_send_core4_cmd timeout\r\n");
			//SEM_SIGNAL(ccSemID);
			return CC_ER_TMOUT;
		}
	} while (!RCW_OF(CC_CPU2STS_REG).AckFromDSP2);
#endif

	// Enter critical section
	loc_cpu();

	// clear status bit
	RCW_VAL(CC_CPU2STS_REG) = 0;
	RCW_OF(CC_CPU2STS_REG).AckFromDSP2 = 1;
	RCW_ST(CC_CPU2STS_REG);

	DBG_IND("Opcode=0x%x,data=0x%x,size=0x%x\r\n", (int)pCMD->uiCMDOperation, (int)pCMD->pData, (int)pCMD->uiDataSize);

	// set register data
	RCW_OF(CC_CPU2_DSP2_CMDBUF_REG1).CoreID = CC_CORE_DSP2;
	RCW_OF(CC_CPU2_DSP2_CMDBUF_REG1).SerialID = cpu4CmdSerialNum++;
	RCW_OF(CC_CPU2_DSP2_CMDBUF_REG1).OPCode = pCMD->uiCMDOperation;
	RCW_ST(CC_CPU2_DSP2_CMDBUF_REG1);

	RCW_VAL(CC_CPU2_DSP2_CMDBUF_REG2) = (CC_U32)pCMD->pData;
	RCW_ST(CC_CPU2_DSP2_CMDBUF_REG2);

	RCW_VAL(CC_CPU2_DSP2_CMDBUF_REG3) = (CC_U32)pCMD->uiDataSize;
	RCW_ST(CC_CPU2_DSP2_CMDBUF_REG3);


	// trigger command
	RCW_VAL(CC_CPU2ACT_REG) = 0;
	RCW_OF(CC_CPU2ACT_REG).TriggerDSP2 = 1;
	RCW_ST(CC_CPU2ACT_REG);

	// Leave critical section
	unl_cpu();

	// wait ack
#if 0
	DBG_IND("wait ack begin\r\n");
	do {
		RCW_LD(CC_CPU2STS_REG);
		if (timeoutCnt++ == CC_SEND_TIMEOUT_CNT) {
			DBG_ERR("cc_send_core3_cmd timeout\r\n");
			//SEM_SIGNAL(ccSemID);
			return CC_ER_TMOUT;
		}
	} while (!RCW_OF(CC_CPU2STS_REG).AckFromDSP2);
	DBG_IND("wait ack end\r\n");
#endif

	return CC_ER_OK;
}

void cc_get_core1_cmd(PCC_CMD pCMD)
{
	RCW_DEF(CC_CPU1_CPU2_CMDBUF_REG1);
	RCW_DEF(CC_CPU1_CPU2_CMDBUF_REG2);
	RCW_DEF(CC_CPU1_CPU2_CMDBUF_REG3);

	DBG_IND("cc_get_core1_cmd\r\n");

	RCW_LD(CC_CPU1_CPU2_CMDBUF_REG1);
	pCMD->uiCMDID = RCW_OF(CC_CPU1_CPU2_CMDBUF_REG1).SerialID;
	pCMD->uiCMDOperation = RCW_OF(CC_CPU1_CPU2_CMDBUF_REG1).OPCode;
	pCMD->pData = (CC_U32 *)(RCW_LD(CC_CPU1_CPU2_CMDBUF_REG2));
	pCMD->uiDataSize = RCW_LD(CC_CPU1_CPU2_CMDBUF_REG3);

	//DBG_DUMP("ID=%d\r\n",pCMD->uiCMDID);

}

void cc_get_core3_cmd(PCC_CMD pCMD)
{
	RCW_DEF(CC_DSP_CPU2_CMDBUF_REG1);
	RCW_DEF(CC_DSP_CPU2_CMDBUF_REG2);
	RCW_DEF(CC_DSP_CPU2_CMDBUF_REG3);

	DBG_IND("cc_get_core3_cmd\r\n");

	RCW_LD(CC_DSP_CPU2_CMDBUF_REG1);

	pCMD->uiCMDID = RCW_OF(CC_DSP_CPU2_CMDBUF_REG1).SerialID;
	pCMD->uiCMDOperation = RCW_OF(CC_DSP_CPU2_CMDBUF_REG1).OPCode;
	pCMD->pData = (CC_U32 *)(RCW_LD(CC_DSP_CPU2_CMDBUF_REG2));
	pCMD->uiDataSize = RCW_LD(CC_DSP_CPU2_CMDBUF_REG3);
}

void cc_get_core4_cmd(PCC_CMD pCMD)
{
	RCW_DEF(CC_DSP2_CPU2_CMDBUF_REG1);
	RCW_DEF(CC_DSP2_CPU2_CMDBUF_REG2);
	RCW_DEF(CC_DSP2_CPU2_CMDBUF_REG3);

	DBG_IND("cc_get_core4_cmd\r\n");

	RCW_LD(CC_DSP2_CPU2_CMDBUF_REG1);

	pCMD->uiCMDID = RCW_OF(CC_DSP2_CPU2_CMDBUF_REG1).SerialID;
	pCMD->uiCMDOperation = RCW_OF(CC_DSP2_CPU2_CMDBUF_REG1).OPCode;
	pCMD->pData = (CC_U32 *)(RCW_LD(CC_DSP2_CPU2_CMDBUF_REG2));
	pCMD->uiDataSize = RCW_LD(CC_DSP2_CPU2_CMDBUF_REG3);
}


void cc_ack_core1_cmd(void)
{
	RCW_DEF(CC_CPU2ACT_REG);

	DBG_IND("cc_ack_core1_cmd\r\n");
	// Enter critical section
	loc_cpu();

	RCW_VAL(CC_CPU2ACT_REG) = 0;
	RCW_OF(CC_CPU2ACT_REG).AckCPU1 = 1;
	RCW_ST(CC_CPU2ACT_REG);

	// Leave critical section
	unl_cpu();
}

void cc_ack_core3_cmd(void)
{
	RCW_DEF(CC_CPU2ACT_REG);

	DBG_IND("cc_ack_core3_cmd\r\n");

	// Enter critical section
	loc_cpu();

	RCW_VAL(CC_CPU2ACT_REG) = 0;
	RCW_OF(CC_CPU2ACT_REG).AckDSP = 1;
	RCW_ST(CC_CPU2ACT_REG);

	// Leave critical section
	unl_cpu();
}

void cc_ack_core4_cmd(void)
{
	RCW_DEF(CC_CPU2ACT_REG);

	DBG_IND("cc_ack_core4_cmd\r\n");

	// Enter critical section
	loc_cpu();

	RCW_VAL(CC_CPU2ACT_REG) = 0;
	RCW_OF(CC_CPU2ACT_REG).AckDSP2 = 1;
	RCW_ST(CC_CPU2ACT_REG);

	// Leave critical section
	unl_cpu();
}


CC_ER cc_hwlock_resource(CC_CORE_ID uiCoreID, CC_RES_ID uiResID)
{
	RCW_DEF(CC_CPU1REQ_REG);
	RCW_DEF(CC_CPU2REQ_REG);
	RCW_DEF(CC_DSPREQ_REG);
	RCW_DEF(CC_DSP2REQ_REG);
	UINT32 timeoutCnt = 0;

	if (uiCoreID == CC_CORE_CA53_CORE1) {
		loc_cpu();
		RCW_OF(CC_CPU1REQ_REG).Request = (1 << uiResID);
		RCW_ST(CC_CPU1REQ_REG);
		unl_cpu();

	} else if (uiCoreID == CC_CORE_CA53_CORE2) {
		loc_cpu();
		RCW_OF(CC_CPU2REQ_REG).Request = (1 << uiResID);
		RCW_ST(CC_CPU2REQ_REG);
		unl_cpu();

	} else if (uiCoreID == CC_CORE_DSP) {
		loc_cpu();
		RCW_OF(CC_DSPREQ_REG).Request = (1 << uiResID);
		RCW_ST(CC_DSPREQ_REG);
		unl_cpu();
	} else if (uiCoreID == CC_CORE_DSP2) {
		loc_cpu();
		RCW_OF(CC_DSP2REQ_REG).Request = (1 << uiResID);
		RCW_ST(CC_DSP2REQ_REG);
		unl_cpu();
	} else {
		DBG_ERR("Unknow coreID\r\n");
		return CC_ER_PARM;
	}
	// Polling until relative grant bit assert to 1
	while (!(cc_get_hw_res_grant(uiCoreID) & (1 << uiResID))) {
		if (timeoutCnt++ == CC_GRANT_TIMEOUT_CNT) {
			DBG_ERR("timeout, uiResID=%d\r\n", uiResID);
			//return E_TMOUT;
		}
	}
	return CC_ER_OK;
}
EXPORT_SYMBOL(cc_hwlock_resource);



CC_ER cc_hwunlock_resource(CC_CORE_ID uiCoreID, CC_RES_ID uiResID)
{
	RCW_DEF(CC_CPU1REL_REG);
	RCW_DEF(CC_CPU2REL_REG);
	RCW_DEF(CC_DSPREL_REG);
	RCW_DEF(CC_DSP2REL_REG);


	if (uiCoreID == CC_CORE_CA53_CORE1) {
		loc_cpu();
		RCW_OF(CC_CPU1REL_REG).Release = (1 << uiResID);
		RCW_ST(CC_CPU1REL_REG);
		unl_cpu();
	} else if (uiCoreID == CC_CORE_CA53_CORE2) {
		loc_cpu();
		RCW_OF(CC_CPU2REL_REG).Release = (1 << uiResID);
		RCW_ST(CC_CPU2REL_REG);
		unl_cpu();
	} else if (uiCoreID == CC_CORE_DSP) {
		loc_cpu();
		RCW_OF(CC_DSPREL_REG).Release = (1 << uiResID);
		RCW_ST(CC_DSPREL_REG);
		unl_cpu();
	} else if (uiCoreID == CC_CORE_DSP2) {
		loc_cpu();
		RCW_OF(CC_DSP2REL_REG).Release = (1 << uiResID);
		RCW_ST(CC_DSP2REL_REG);
		unl_cpu();
	} else {
		DBG_ERR("Unknow coreID\r\n");
		return E_SYS;
	}
	return CC_ER_OK;
}
EXPORT_SYMBOL(cc_hwunlock_resource);


CC_I32 cc_get_hw_res_grant(CC_CORE_ID uiCoreID)
{
	INT32 result;
	RCW_DEF(CC_CPU1GRANT_REG);
	RCW_DEF(CC_CPU2GRANT_REG);
	RCW_DEF(CC_DSPGRANT_REG);
	RCW_DEF(CC_DSP2GRANT_REG);

	if (uiCoreID == CC_CORE_CA53_CORE1) {
		result = RCW_LD(CC_CPU1GRANT_REG);
	} else if (uiCoreID == CC_CORE_CA53_CORE2) {
		result = RCW_LD(CC_CPU2GRANT_REG);
	} else if (uiCoreID == CC_CORE_DSP) {
		result = RCW_LD(CC_DSPGRANT_REG);
	} else if (uiCoreID == CC_CORE_DSP2) {
		result = RCW_LD(CC_DSP2GRANT_REG);
	} else {
		DBG_ERR("Unknow coreID\r\n");
		result = E_SYS;
	}
	return result;
}
EXPORT_SYMBOL(cc_get_hw_res_grant);


