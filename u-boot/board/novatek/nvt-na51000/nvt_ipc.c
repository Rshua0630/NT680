#include <common.h>
#include <asm/arch/rcw_macro.h>
#include <asm/arch/nvt_ipc_common.h>
#include <asm/arch/IOAddress.h>
#include "cc/cc.h"
#include <asm/arch/nvt_common.h>
#include <asm/addrspace.h>


#define DBG_ERR(fmtstr, args...) printf("ERR: " fmtstr, ##args)

#define DBG_WRN(fmtstr, args...) printf("WRN: " fmtstr, ##args)

#if 1
#define DBG_IND(fmtstr, args...)
#else
#define DBG_IND(fmtstr, args...) printf("%s(): " fmtstr, \
    __func__, ##args)
#endif

#define DBG_DUMP(fmtstr, args...) printf(fmtstr, ##args)

#define loc_multiCores()    {cc_hwLockResource(CC_CORE_CA53_CORE2,CC_RESOURCE_IPC);}
#define unl_multiCores()    {cc_hwUnlockResource(CC_CORE_CA53_CORE2,CC_RESOURCE_IPC);}

#define TIMER0_COUNTER_REG  (IOADDR_TIMER_REG_BASE+0x108)


static unsigned int drv_getSysTick(void)
{
    volatile unsigned int* p1 = (volatile unsigned int*)TIMER0_COUNTER_REG;
    return *p1;

}

static void cc_invalid_cache(unsigned long addr, unsigned long size)
{
    return invalidate_dcache_range(addr,addr+size);
}

static void cc_flush_cache(unsigned long addr, unsigned long size)
{
    return flush_dcache_range(addr,addr+size);
}


//invalidate_dcache_range(dwBuffer, dwBuffer+64);
//flush_dcache_range((u32)dwBuffer, (u32)dwBuffer+NAND_Page_Size);

// ensure the data flush from AXI bus to dram
#if 0
static void cc_flush_data(void)
{
    /*DSB to clear barriar on AXI bus and add dummy read operation
      DSB can make sure data from CA53 to AXI BUS.
      Dummy Read can make sure AXI BUS data to Dram.
    */
	__asm__ __volatile__("dsb\n\t");
	(*((volatile unsigned int*)(NVTIPC_SHARED_MEM_ADDR)));
}
#endif

static void cc_SndSemUnlockCmd(unsigned long semid)
{
    CC_CMD          ccCmd;
	NVTIPC_SHM_CTRL *pSemCtrl = (NVTIPC_SHM_CTRL*)(NVTIPC_SHARED_MEM_ADDR);
    NVTIPC_SEM_MSG   *pMsg = (NVTIPC_SEM_MSG   *)&pSemCtrl->ubootCmd;

    DBG_IND("cc_SndSemUnlockCmd 1\r\n");
    pMsg->CmdID = NVTIPC_SEM_CMD_UNLOCK;
    pMsg->semid = semid;

    ccCmd.uiCMDOperation = 0xe8d2;  // key=0xe8d2, token=ipcsem
    ccCmd.pData = (CC_U32*)&pMsg->CmdID;
    ccCmd.uiDataSize = sizeof(NVTIPC_SEM_MSG);
	cc_flush_cache((unsigned long)pSemCtrl,NVTIPC_SHARED_MEM_SIZE);

    DBG_IND("cc_SndSemUnlockCmd 2\r\n");
    cc_sendCore1CMD(&ccCmd);
    DBG_IND("cc_SndSemUnlockCmd 3\r\n");
}

int nvt_ipc_sem_lock(unsigned long semid)
{
	//static UINT32 g_loc_count = 0;
	NVTIPC_SHM_CTRL *pSemCtrl = (NVTIPC_SHM_CTRL*)(NVTIPC_SHARED_MEM_ADDR);
	UINT32 ptn = (1 << semid);

    DBG_IND("semid=%d\r\n",semid);
	if(semid >= NVTIPC_SEM_NUM)
    {
        DBG_ERR("semid = %d\r\n",(int)semid);
        return -1;
    }

	//g_loc_count = 0;

	while (1)
	{
		loc_multiCores();
		cc_invalid_cache((unsigned long)pSemCtrl,NVTIPC_SHARED_MEM_SIZE);
		//g_loc_count++;
		//if (g_loc_count % 100 ==0)
			//printf("lock %d\r\n",(int)g_loc_count);
		if (pSemCtrl->SemBits & ptn)
		{
			pSemCtrl->SemBits &= ~ptn;
			// make sure the bits is writen to dram
			cc_flush_cache((unsigned long)pSemCtrl,NVTIPC_SHARED_MEM_SIZE);
			unl_multiCores();
			break;
		}
		else
		{
			//printf("lock %d\r\n",(int)g_loc_count);
			unl_multiCores();
		}
	}
	return 0;
}

int nvt_ipc_sem_unlock(unsigned long semid)
{
    NVTIPC_SHM_CTRL *pSemCtrl = (NVTIPC_SHM_CTRL*)(NVTIPC_SHARED_MEM_ADDR);
	UINT32 ptn = (1 << semid);

    DBG_IND("semid=%d\r\n",semid);
    if(semid >= NVTIPC_SEM_NUM)
    {
        DBG_ERR("semid = %d\r\n",(int)semid);
        return -1;
    }
	loc_multiCores();
	pSemCtrl->SemBits |= ptn;
    pSemCtrl->Cpu2SemUnLockTime = drv_getSysTick();
    if(pSemCtrl->Cpu1SemWaitCnt[semid])
    {
        //pSemCtrl->Cpu1SemWaitCnt[semid]--;
        DBG_IND("Cpu1SemWaitCnt = 0x%x \r\n",pSemCtrl->Cpu1SemWaitCnt[semid]);
		cc_flush_cache((unsigned long)pSemCtrl,NVTIPC_SHARED_MEM_SIZE);
        unl_multiCores();
        // need to send to cpu1 to wake up waiting thread
        cc_SndSemUnlockCmd(semid);
    }
    else
    {
        DBG_IND("No cpu wait\r\n");
		cc_flush_cache((unsigned long)pSemCtrl,NVTIPC_SHARED_MEM_SIZE);
        unl_multiCores();
    }
	return 0;
}
