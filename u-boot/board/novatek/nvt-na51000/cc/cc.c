/**

    Core Communicator module driver

    Core Communicator module driver.

    @file       cc.c
    Copyright   Novatek Microelectronics Corp. 2016.  All rights reserved.

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License version 2 as
    published by the Free Software Foundation.
*/

#include <asm/arch/rcw_macro.h>
#include <asm/arch/nvt_ipc_common.h>
#include <asm/arch/IOAddress.h>
#include "cc.h"
#include "cc_int.h"
#include "cc_reg.h"


#define CC_SEND_TIMEOUT_CNT		1000000

// Set register base address
REGDEF_SETIOBASE(IOADDR_CC_REG_BASE);

CC_ER cc_sendCore1CMD(PCC_CMD pCMD)
{
    unsigned int  timeoutCnt = 0;
    RCW_DEF(CC_CPU2STS_REG);
    RCW_DEF(CC_CPU2_CPU1_CMDBUF_REG1);
    RCW_DEF(CC_CPU2_CPU1_CMDBUF_REG2);
    RCW_DEF(CC_CPU2_CPU1_CMDBUF_REG3);
    RCW_DEF(CC_CPU2ACT_REG);

    // wait for status bit ready
    #if 0
    do
    {
        RCW_LD(CC_CPU2STS_REG);
        if (timeoutCnt++ == CC_SEND_TIMEOUT_CNT)
        {
            DBG_ERR("cc_sendCore1CMD timeout\r\n");
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

    DBG_IND("Opcode=0x%x,data=0x%x,size=0x%x\r\n",(int)pCMD->uiCMDOperation,(int)pCMD->pData,(int)pCMD->uiDataSize);

    // set register data
    RCW_OF(CC_CPU2_CPU1_CMDBUF_REG1).SerialID = 1;
    RCW_OF(CC_CPU2_CPU1_CMDBUF_REG1).OPCode = pCMD->uiCMDOperation;
    RCW_ST(CC_CPU2_CPU1_CMDBUF_REG1);

    RCW_VAL(CC_CPU2_CPU1_CMDBUF_REG2) =(CC_U32)pCMD->pData;
    RCW_ST(CC_CPU2_CPU1_CMDBUF_REG2);

    RCW_VAL(CC_CPU2_CPU1_CMDBUF_REG3) =(CC_U32)pCMD->uiDataSize;
    RCW_ST(CC_CPU2_CPU1_CMDBUF_REG3);


    // trigger command
    RCW_VAL(CC_CPU2ACT_REG) = 0;
    RCW_OF(CC_CPU2ACT_REG).TriggerCPU1 = 1;
    RCW_ST(CC_CPU2ACT_REG);

    // Leave critical section
    unl_cpu();

    DBG_IND("wait ack begin\r\n");
    // wait ack
    #if 1
    do
    {
        RCW_LD(CC_CPU2STS_REG);
        if (timeoutCnt++ == CC_SEND_TIMEOUT_CNT)
        {
            DBG_ERR("cc_sendCore1CMD timeout\r\n");
            //SEM_SIGNAL(ccSemID);
            return CC_ER_TMOUT;
        }
    } while (!RCW_OF(CC_CPU2STS_REG).AckFromCPU1);
    #endif

    DBG_IND("wait ack end\r\n");
    return CC_ER_OK;
}

CC_ER cc_hwLockResource(CC_CORE_ID uiCoreID, CC_RES_ID uiResID)
{
	RCW_DEF(CC_CPU1REQ_REG);
	RCW_DEF(CC_CPU2REQ_REG);
	RCW_DEF(CC_DSPREQ_REG);

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
	} else {
		DBG_ERR("Unknow coreID\r\n");
		return CC_ER_PARM;
	}
	// Polling until relative grant bit assert to 1
	while(!(cc_getHwResGrant(uiCoreID) & (1 << uiResID)));
	return CC_ER_OK;
}


CC_ER cc_hwUnlockResource(CC_CORE_ID uiCoreID, CC_RES_ID uiResID)
{
	RCW_DEF(CC_CPU1REL_REG);
	RCW_DEF(CC_CPU2REL_REG);
	RCW_DEF(CC_DSPREL_REG);

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
	}
	return CC_ER_OK;
}

CC_I32 cc_getHwResGrant(CC_CORE_ID uiCoreID)
{
	int result;
	RCW_DEF(CC_CPU1GRANT_REG);
	RCW_DEF(CC_CPU2GRANT_REG);
	RCW_DEF(CC_DSPGRANT_REG);

	if (uiCoreID == CC_CORE_CA53_CORE1) {
		result = RCW_LD(CC_CPU1GRANT_REG);
	} else if (uiCoreID == CC_CORE_CA53_CORE2) {
		result = RCW_LD(CC_CPU2GRANT_REG);
	} else if (uiCoreID == CC_CORE_DSP) {
		result = RCW_LD(CC_DSPGRANT_REG);
	} else {
		DBG_ERR("Unknow coreID\r\n");
		result = E_SYS;
	}
	return result;
}
