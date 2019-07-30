/**

    Core Communicator module driver

    Core Communicator module driver.

    @file       cc.c
    Copyright   Novatek Microelectronics Corp. 2014.  All rights reserved.

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License version 2 as
    published by the Free Software Foundation.
*/

//#include "interrupt.h"
#include "RCWMacro.h"
#include "nvt-ipc.h"
#include "cc.h"
#include "cc_int.h"
#include "cc_reg.h"

#define loc_cpu()   do{}while(0)
#define unl_cpu()   do{}while(0)

#define CC_SEND_TIMEOUT_CNT   1000000

// CC
#define IOADDR_CC_REG_BASE          (0xC0090000)
// Set register base address
REGDEF_SETIOBASE(IOADDR_CC_REG_BASE);

extern u32 nt96660_is_need_upd_ubi(void);

CC_ER cc_hwLockResource(CC_CORE_ID uiCoreID, CC_RES_ID uiResID)
{
    RCW_DEF(CC_CPU1REQ_REG);
    RCW_DEF(CC_CPU2REQ_REG);
    RCW_DEF(CC_DSPREQ_REG);

    if(uiCoreID == CC_CORE_MIPS1)
    {
        loc_cpu();
        RCW_OF(CC_CPU1REQ_REG).Request = (1 << uiResID);
        RCW_ST(CC_CPU1REQ_REG);
        unl_cpu();

    }
    else if(uiCoreID == CC_CORE_MIPS2)
    {
        loc_cpu();
        RCW_OF(CC_CPU2REQ_REG).Request = (1 << uiResID);
        RCW_ST(CC_CPU2REQ_REG);
        unl_cpu();

    }
    else if(uiCoreID == CC_CORE_DSP)
    {
        loc_cpu();
        RCW_OF(CC_DSPREQ_REG).Request = (1 << uiResID);
        RCW_ST(CC_DSPREQ_REG);
        unl_cpu();
    }
    else
    {
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


    if(uiCoreID == CC_CORE_MIPS1)
    {
        loc_cpu();
        RCW_OF(CC_CPU1REL_REG).Release = (1 << uiResID);
        RCW_ST(CC_CPU1REL_REG);
        unl_cpu();
    }
    else if(uiCoreID == CC_CORE_MIPS2)
    {
        loc_cpu();
        RCW_OF(CC_CPU2REL_REG).Release = (1 << uiResID);
        RCW_ST(CC_CPU2REL_REG);
        unl_cpu();
    }
    else if(uiCoreID == CC_CORE_DSP)
    {
        loc_cpu();
        RCW_OF(CC_DSPREL_REG).Release = (1 << uiResID);
        RCW_ST(CC_DSPREL_REG);
        unl_cpu();
    }
    return CC_ER_OK;
}


CC_I32 cc_getHwResGrant(CC_CORE_ID uiCoreID)
{
    INT32 result;
    RCW_DEF(CC_CPU1GRANT_REG);
    RCW_DEF(CC_CPU2GRANT_REG);
    RCW_DEF(CC_DSPGRANT_REG);

    if(uiCoreID == CC_CORE_MIPS1)
    {
        result = RCW_LD(CC_CPU1GRANT_REG);
    }
    else if(uiCoreID == CC_CORE_MIPS2)
    {
        result = RCW_LD(CC_CPU2GRANT_REG);
    }
    else if(uiCoreID == CC_CORE_DSP)
    {
        result = RCW_LD(CC_DSPGRANT_REG);
    }
    else
    {
        DBG_ERR("Unknow coreID\r\n");
        result = E_SYS;
    }
    return result;
}

void loc_multiCores(void)
{
	NVTIPC_SEM_CTRL *pSemCtrl = (NVTIPC_SEM_CTRL*)NvtIpc_GetSharedMemory();
	UINT32 ptn = (1 << 0);

	if(nt96660_is_need_upd_ubi())
		return;

	while (1)
	{
		cc_hwLockResource(CC_CORE_MIPS2,CC_RESOURCE_IPC);	
		if (pSemCtrl->SemBits & ptn) {
			pSemCtrl->SemBits &= ~ptn;
			// make sure the bits is writen to dram
			while ((pSemCtrl->SemBits & ptn))
			{
				;
			}
			cc_hwUnlockResource(CC_CORE_MIPS2,CC_RESOURCE_IPC);
			break;
		} else {
			cc_hwUnlockResource(CC_CORE_MIPS2,CC_RESOURCE_IPC);
		}
	}
}

void unl_multiCores(void)
{
	NVTIPC_SEM_CTRL *pSemCtrl = (NVTIPC_SEM_CTRL*)NvtIpc_GetSharedMemory();
	UINT32 ptn = (1 << 0);

	if(nt96660_is_need_upd_ubi())
		return;

	cc_hwLockResource(CC_CORE_MIPS2,CC_RESOURCE_IPC);
	pSemCtrl->SemBits |= ptn;
	// make sure the bits is writen to dram
	while (!(pSemCtrl->SemBits & ptn))
	{
		;
	}

	cc_hwUnlockResource(CC_CORE_MIPS2,CC_RESOURCE_IPC);
}
