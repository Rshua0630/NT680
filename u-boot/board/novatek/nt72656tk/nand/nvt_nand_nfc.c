#ifdef __NVT_NAND_UBOOT__
#include <linux/compat.h>
#include <common.h>
#include "nvt_nand_nfc_uboot.h"
#include "nvt_nand_nfc.h"
#include "nvt_nand_nt72656_regs.h"
#endif

#ifdef __NVT_NAND_ONBOOT__
#include <stdio.h>
#include <string.h>
#include "nvt_nand_656onboot.h"
#include "nvt_nand_nfc.h"
#include "nvt_nand_nt72656_regs.h"
#include "board.h"
#endif

#ifdef __NVT_NAND_KERNEL__
#include "nvt_nand_nfc_kernel.h"
#include "nvt_nand_nfc.h"
#include "nvt_nand_nt72656_regs.h"
#if NFC_NAND_INTERRUPT
#include <linux/sched.h>
#include <linux/irq.h>
#include <linux/interrupt.h>
#include <linux/semaphore.h>
#endif
#endif

#define NFC_USE_HW_RESET               0 
#define NFC_NAND_VERIFICATION_USED     0

unsigned int  S_REG_NFC_CFG0 ;
unsigned int  S_REG_NFC_CFG1 ;
unsigned char dwCE = 0;

//internal function
int NVT_nand_nfc_ResetFlash(PNAND_FLASH_PARAMETER pFlashInfo);
void NVT_nand_nfc_DrivingStrength(void);
unsigned int NVT_nand_nfc_GetECCBitsAndPos_RS(PNAND_FLASH_PARAMETER pFlashInfo, PECC_INFO pucErrorBitArray);
unsigned int NVT_nand_nfc_GetECCBitsAndPos_BCH54(PNAND_FLASH_PARAMETER pFlashInfo, PECC_INFO pucErrorBitArray);
unsigned int NVT_nand_nfc_GetECCBitsAndPos(PNAND_FLASH_PARAMETER pFlashInfo, PECC_INFO pucErrorBitArray);
void NVT_nand_nfc_ShowParameter(PNAND_FLASH_PARAMETER pFlashInfo);
int NVT_nand_nfc_initRelatedHW_NT72656(PNAND_FLASH_PARAMETER pFlashInfo);
void NVT_nand_nfc_DumpRegister(PNAND_FLASH_PARAMETER pFlashInfo);
#if NFC_NAND_INTERRUPT
unsigned int NVT_nand_nfc_WaitForCmdComplete(PNAND_FLASH_PARAMETER pFlashInfo, unsigned int uiWaitForSignalType);
struct semaphore NVT_NFC_IRQ;
#endif

int NVT_nand_nfc_initRelatedHW_NT72656(PNAND_FLASH_PARAMETER pFlashInfo)
{
	void __iomem* StbcBase;
	void __iomem* NfcSecureLockCtrl;
	void __iomem* NfcMauW64Master0;
	void __iomem* NfcMauR64Master0;
	void __iomem* NfcMauArbiter;

	//STBC Keypass
	StbcBase = ioremap_nocache(NVT_REG_STBC_BASE, 0x1000);

	WREG_ADDR((StbcBase + NVT_REG_PHY_KEY_CTRL), NVT_STBC_CTRL_KEY1);
	WREG_ADDR((StbcBase + NVT_REG_PHY_KEY_CTRL), NVT_STBC_CTRL_KEY2);
	WREG_ADDR((StbcBase + NVT_REG_PHY_KEY_ENABLE), NVT_STBC_ENABLE);

	//Reduce pad driving strenght
	WREG_ADDR((StbcBase + NVT_REG_STBC_GPF_G1), (RREG_ADDR(StbcBase + NVT_REG_STBC_GPF_G1) | NVT_REG_STBC_GPF_NAND_D0 | NVT_REG_STBC_GPF_NAND_D1));
	WREG_ADDR((StbcBase + NVT_REG_STBC_GPF_G2), (RREG_ADDR(StbcBase + NVT_REG_STBC_GPF_G2) | NVT_REG_STBC_GPF1_NAND_D2 | NVT_REG_STBC_GPF1_NAND_D3 | NVT_REG_STBC_GPF1_NAND_D4 | (NVT_REG_STBC_GPF1_NAND_D5)));
	WREG_ADDR((StbcBase + NVT_REG_STBC_GPF_G3), (RREG_ADDR(StbcBase + NVT_REG_STBC_GPF_G3) | NVT_REG_STBC_GPF2_NAND_D6 | NVT_REG_STBC_GPF2_NAND_D7));

	WREG_ADDR((StbcBase + NVT_REG_PHY_NFC_ARBITOR_CTRL), ((RREG_ADDR(StbcBase + NVT_REG_PHY_NFC_ARBITOR_CTRL) & (~((unsigned int)NVT_NFC_STBC_ENABLE))) | NVT_NFC_MAINCHIP_ENABLE));
	iounmap(StbcBase);

	//Enable AHB access right
	NfcSecureLockCtrl = ioremap_nocache(NVT_REG_PHY_SECURE_LOCK_CTRL, 4);
	WREG_ADDR(NfcSecureLockCtrl, (RREG_ADDR(NfcSecureLockCtrl) & (~((unsigned int) NVT_EMMC_SECURE_LOCK_ENABLE))));
	iounmap(NfcSecureLockCtrl);

	NfcMauW64Master0 = ioremap_nocache(NVT_REG_MAU_W64BIT_MASTER0, 4);
	WREG_ADDR(NfcMauW64Master0, NVT_MAU_W64BIT_MASTER0);
	iounmap(NfcMauW64Master0);

	NfcMauR64Master0 = ioremap_nocache(NVT_REG_MAU_R64BIT_MASTER0, 4);
	WREG_ADDR(NfcMauR64Master0, NVT_MAU_R64BIT_MASTER0);
	iounmap(NfcMauR64Master0);

	NfcMauArbiter = ioremap_nocache(NVT_REG_MAU_ARBITER, 4);
	WREG_ADDR(NfcMauArbiter, NVT_MAU_ARBITER);
	iounmap(NfcMauArbiter);

	return NAND_ERROR_OK;
}

int NVT_nand_nfc_Init(PNAND_FLASH_PARAMETER pFlashInfo)
{
    NFC_DBG(KERN_ERR "%s+\r\n", __func__);

	if(pFlashInfo->pControllerBaseAddr == NULL)
		pFlashInfo->pControllerBaseAddr = (unsigned int*)REG_NFC_BASE;

	if(NVT_nand_nfc_initRelatedHW_NT72656(pFlashInfo) != NAND_ERROR_OK)
		return 0;
		
    S_REG_NFC_CFG0 = 0x44443020;

    if(pFlashInfo->uiNumOfRow == 3)    // Row address cycle count
        S_REG_NFC_CFG0 |= 0x2;        // 3 Row address cycle count
    else
        S_REG_NFC_CFG0 |= 0;          // 2 Row address cycle count

    if(pFlashInfo->uiNumOfCol == 2)    // Column address cycle count
        S_REG_NFC_CFG0 |= 0x1;        // 2 Row address cycle count
    else
        S_REG_NFC_CFG0 |= 0;          // 1 Row address cycle count


    S_REG_NFC_CFG1 = NFC_CFG1_READY_TO_BUSY_TIMEOUT(-1) | NFC_CFG1_LITTLE_ENDIAN_XTRA |
        NFC_CFG1_LITTLE_ENDIAN | NFC_CFG1_BUSY_TO_READY_TIMEOUT(-1);

    // software reset host
    WREG(pFlashInfo, REG_NFC_SW_RESET, 7);
    while(RREG(pFlashInfo, REG_NFC_SW_RESET) != 0);

    NVT_nand_nfc_ResetFlash(pFlashInfo);

    // clear interrupt status
    WREG(pFlashInfo, REG_NFC_INT_ENABLE, 0);
    WREG(pFlashInfo, REG_NFC_INT_STAT, NFC_INT_ALL);

    NFC_DBG(KERN_ERR "%s-\r\n", __func__);
    return 1;
}

#ifdef __NVT_NAND_ONBOOT__
int NVT_nand_nfc_InitFuncByBootstrap(PNAND_FLASH_PARAMETER pFlashInfo)
{
	unsigned int ulBootStrap = *(unsigned int*)_BOOT_STRAP_1;

	if(pFlashInfo->pControllerBaseAddr == NULL)
		pFlashInfo->pControllerBaseAddr = (unsigned int*)REG_NFC_BASE;
	
	if(NVT_nand_nfc_initRelatedHW_NT72656(pFlashInfo) != NAND_ERROR_OK)
		return 0;

	// Get Bootstrap value to determine what type of NAND Flash using in this system
	if(NAND_BT_GET_COLUMN_ADDR_CYCLE(ulBootStrap) == NAND_COLUMN_ADDR_1_CYCLE)
	{
		pFlashInfo->page_size = 512;
    	pFlashInfo->page_per_block = 32;
		pFlashInfo->uiNumOfCol = 1;
	}
	else
	{
		pFlashInfo->uiNumOfCol = 2;
		switch(NAND_BT_GET_PAGE_SZIE(ulBootStrap))
		{
			case 0: // 2k
//				REG_NFC_SYSCTRL |= REG_NFC_SYSCTRL | NFC_SYS_CTRL_PAGE_2048;
				WREG(pFlashInfo, REG_NFC_SYSCTRL, RREG(pFlashInfo, REG_NFC_SYSCTRL) | REG_NFC_SYSCTRL | NFC_SYS_CTRL_PAGE_2048);
				pFlashInfo->page_size = 0x800;
				break;
			case 2: // 4k
	//			  gulNAND_Page_Size = 0x2000;
//				REG_NFC_SYSCTRL |= REG_NFC_SYSCTRL | NFC_SYS_CTRL_PAGE_4096;
				WREG(pFlashInfo, REG_NFC_SYSCTRL, RREG(pFlashInfo, REG_NFC_SYSCTRL) | REG_NFC_SYSCTRL | NFC_SYS_CTRL_PAGE_4096);
				pFlashInfo->page_size = 0x1000;
				break;
			case 1: // 8k
	//			  gulNAND_Page_Size = 0x1000;
//				REG_NFC_SYSCTRL |= REG_NFC_SYSCTRL | NFC_SYS_CTRL_PAGE_8192;
				WREG(pFlashInfo, REG_NFC_SYSCTRL, RREG(pFlashInfo, REG_NFC_SYSCTRL) | REG_NFC_SYSCTRL | NFC_SYS_CTRL_PAGE_8192);
				pFlashInfo->page_size = 0x2000;
				break;
		}
		switch(NAND_BT_GET_BLOCK_SZIE(ulBootStrap))
		{
			case 0: // 128k
	//			  gulNAND_Block_Size = 0x20000;
//				REG_NFC_SYSCTRL |= REG_NFC_SYSCTRL | NFC_SYS_CTRL_BLK_128K;
				WREG(pFlashInfo, REG_NFC_SYSCTRL, RREG(pFlashInfo, REG_NFC_SYSCTRL) | NFC_SYS_CTRL_BLK_128K);	
				pFlashInfo->page_per_block = 128 * 1024 / pFlashInfo->page_size;
				break;
			case 4: // 256k
	//			  gulNAND_Block_Size = 0x80000;
//				REG_NFC_SYSCTRL |= REG_NFC_SYSCTRL | NFC_SYS_CTRL_BLK_256K;
				WREG(pFlashInfo, REG_NFC_SYSCTRL, RREG(pFlashInfo, REG_NFC_SYSCTRL) | NFC_SYS_CTRL_BLK_256K);	
				pFlashInfo->page_per_block = 128 * 1024 / pFlashInfo->page_size;
				break;
			case 2: // 512k
	//			  gulNAND_Block_Size = 0x40000;
//				REG_NFC_SYSCTRL |= REG_NFC_SYSCTRL | NFC_SYS_CTRL_BLK_512K;
				WREG(pFlashInfo, REG_NFC_SYSCTRL, RREG(pFlashInfo, REG_NFC_SYSCTRL) | NFC_SYS_CTRL_BLK_512K);	
				pFlashInfo->page_per_block = 128 * 1024 / pFlashInfo->page_size;
				break;
			case 6: // 1MB
	//			  gulNAND_Block_Size = 0x100000;
//				REG_NFC_SYSCTRL |= REG_NFC_SYSCTRL | NFC_SYS_CTRL_BLK_1M;
				WREG(pFlashInfo, REG_NFC_SYSCTRL, RREG(pFlashInfo, REG_NFC_SYSCTRL) | NFC_SYS_CTRL_BLK_1M);	
				pFlashInfo->page_per_block = 128 * 1024 / pFlashInfo->page_size;
				break;
			case 1: // 2MB
	//			  gulNAND_Block_Size = 0x100000;
//				REG_NFC_SYSCTRL |= REG_NFC_SYSCTRL | NFC_SYS_CTRL_BLK_1M;
				WREG(pFlashInfo, REG_NFC_SYSCTRL, RREG(pFlashInfo, REG_NFC_SYSCTRL) | NFC_SYS_CTRL_BLK_1M);	
				pFlashInfo->page_per_block = 128 * 1024 / pFlashInfo->page_size;
				break;
		}
	}

	switch(NAND_BT_GET_BT_OPTION2(ulBootStrap))
	{
		case 0:
		case 1:
			pFlashInfo->uiNumOfRow = 2;
			break;

		case 3:
		case 4:
			pFlashInfo->uiNumOfRow = 3;
			break;

		default: //corrupt boot strap setting and just return.
			return NAND_ERROR_PARAMETER;
			break;
	}

	pFlashInfo->nfc_width = 8;

	if(NAND_BT_GET_ECC_MODE(ulBootStrap) == NAND_ECC_MODE_BCH)
	{
		//BCH ECC encorder
		pFlashInfo->ucTypicalECCType= NAND_ECC_TYPE_BCH54;
		pFlashInfo->ucECCCorrectableBit = 24;
		pFlashInfo->subpage_size = 1024;
		pFlashInfo->subpageoob_size = 54;
	}
	else
	{
		//RS ECC encorder
		pFlashInfo->ucTypicalECCType= NAND_ECC_TYPE_REEDSOLOMON;
		pFlashInfo->ucECCCorrectableBit = 4;
		pFlashInfo->subpage_size = 512;
		pFlashInfo->subpageoob_size = 16;
	}
	pFlashInfo->oob_size = (pFlashInfo->page_size / pFlashInfo->subpage_size) * pFlashInfo->subpageoob_size;

	pFlashInfo->uiSimpleProgramTiming = 0x22222220;
	pFlashInfo->uiSimpleReadTiming = 0x72222220;
	pFlashInfo->uiSimpleFineTuneTiming = 0;
	pFlashInfo->num_blocks = 1024;

    if(pFlashInfo->uiNumOfRow == 3)    // Row address cycle count
        S_REG_NFC_CFG0 |= 0x2;        // 3 Row address cycle count
    else
        S_REG_NFC_CFG0 |= 0;          // 2 Row address cycle count

    if(pFlashInfo->uiNumOfCol == 2)    // Column address cycle count
        S_REG_NFC_CFG0 |= 0x1;        // 2 Row address cycle count
    else
        S_REG_NFC_CFG0 |= 0;          // 1 Row address cycle count

	return 1;
}
#endif

int NVT_nand_nfc_ResetFlash(PNAND_FLASH_PARAMETER pFlashInfo)
{
	unsigned char ucCE = dwCE;

    NFC_DBG(KERN_ERR "%s+\r\n", __func__);
	if(pFlashInfo == NULL)
	{
		printk("%s:pFlashInfo is NULL", __func__);
	}

	//REG_NFC_CFG0 = (S_REG_NFC_CFG0 & ~0xFFFFFFF0) | pFlashInfo->uiSimpleReadTiming;
	WREG(pFlashInfo, REG_NFC_CFG0, ((S_REG_NFC_CFG0 & ~0xFFFFFFF0) | pFlashInfo->uiSimpleReadTiming));

	//REG_NFC_Fine_Tune = pFlashInfo->uiSimpleFineTuneTiming;
	WREG(pFlashInfo, REG_NFC_Fine_Tune, pFlashInfo->uiSimpleFineTuneTiming);

	//REG_NFC_CFG1 = S_REG_NFC_CFG1 & ~NFC_CFG1_RS_ECC_ENABLE;
	WREG(pFlashInfo, REG_NFC_CFG1, (S_REG_NFC_CFG1 & ~NFC_CFG1_RS_ECC_ENABLE));

    //REG_NFC_INT_STAT = 0xFFFFFFFF;
    WREG(pFlashInfo, REG_NFC_INT_STAT, NFC_INT_ALL);

    WREG(pFlashInfo, REG_NFC_CMD, (NFC_CMD_CE_IDX(ucCE) | NFC_CMD_END_WAIT_BUSY_TO_RDY | NFC_CMD_CYCLE_ONE | NFC_CMD_CODE0(0xff)));
    while(!(RREG(pFlashInfo, REG_NFC_INT_STAT) & (NFC_INT_ERR | NFC_INT_CMD_COMPLETE)));  // wait for command complete


    NFC_DBG(KERN_ERR "%s-\r\n", __func__);
    return (RREG(pFlashInfo, REG_NFC_INT_STAT) & NFC_INT_ERR) ? -1 : 0;
}



void NVT_nand_nfc_WaitFlashReady(PNAND_FLASH_PARAMETER pFlashInfo)
{
    //Wait Ready/Busy Pin
    while(((RREG(pFlashInfo, REG_NFC_STAT)) & NFC_STAT_RB))  // wait for command complete
    {
        printk("\nWarning : NVT_nfc_wait_flash_ready : wait for command complete...\n");
    }
}

unsigned int NVT_nand_nfc_GetMaxEccBits(void)
{
        unsigned char ucErrCnt = 0;
//        ucErrCnt  = REG_NFC_XTRA_DATA12_ERR_NUM(REG_NFC_XTRA_DATA12);


//            printk(KERN_ERR "\n\n\n\nWarning : NVT_nfc_get_ecc_bits : %d\n\n\n\n", ucErrCnt);

        return ucErrCnt;
}

int NVT_nand_nfc_ReadId(PNAND_FLASH_PARAMETER pFlashInfo, unsigned char* pucBuffer, unsigned int* id, unsigned int* extid)
{
//    const struct NVT_nand_flash *f = info->flash_info;
//    const struct NVT_nand_cmdset *cmdset = f->cmdset;
    unsigned int  device_id=0x00;
    unsigned int  device_extid=0x00;
	NAND_ER erReturn = NAND_ERROR_OK;  
	unsigned char id_buff[8];
	unsigned char ucCE = dwCE;

    NFC_DBG(KERN_ERR "%s+\r\n", __func__);

	if((pucBuffer == NULL) && (id == NULL) && (extid == NULL))
	{
		printk("Buffer error (%s)\r\n", __func__);		
		return NAND_ERROR_PARAMETER;
	}
	
    WREG(pFlashInfo, REG_NFC_SW_RESET, 0x07);
    while(RREG(pFlashInfo, REG_NFC_SW_RESET) != 0);

	if(pFlashInfo == NULL)
	{
		WREG(pFlashInfo, REG_NFC_CFG0, ((S_REG_NFC_CFG0 & ~0xFFFFFFF0) | 0x44442220));
		WREG(pFlashInfo, REG_NFC_Fine_Tune, 0);
	}
	else
	{
		WREG(pFlashInfo, REG_NFC_CFG0, ((S_REG_NFC_CFG0 & ~0xFFFFFFF0) | pFlashInfo->uiSimpleReadTiming));
		WREG(pFlashInfo, REG_NFC_Fine_Tune, pFlashInfo->uiSimpleFineTuneTiming);
	}
	
//	REG_NFC_CFG1 = S_REG_NFC_CFG1 & ~NFC_CFG1_RS_ECC_ENABLE;
	WREG(pFlashInfo, REG_NFC_CFG1, (S_REG_NFC_CFG1 & ~NFC_CFG1_RS_ECC_ENABLE));

//	REG_NFC_SYSCTRL1 &= ~NFC_SYS_CTRL1_DELAY_LATCH_MSK;
	WREG(pFlashInfo, REG_NFC_SYSCTRL1, (RREG(pFlashInfo, REG_NFC_SYSCTRL1) & ~NFC_SYS_CTRL1_DELAY_LATCH_MSK));

//    REG_NFC_SYSCTRL1 |= NFC_SYS_CTRL1_DELAY_LATCH_1T;
	WREG(pFlashInfo, REG_NFC_SYSCTRL1, (RREG(pFlashInfo, REG_NFC_SYSCTRL1) | NFC_SYS_CTRL1_DELAY_LATCH_1T));

    WREG(pFlashInfo, REG_NFC_TRAN_MODE, (NFC_TRAN_MODE_CE_IDX(ucCE) | NFC_TRAN_MODE_BLK_SIZE(8))); // transfer 8 bytes (ID)
    WREG(pFlashInfo, REG_NFC_INT_STAT, NFC_INT_ALL);  // clear interrupt status
    WREG(pFlashInfo, REG_NFC_CMD, (NFC_CMD_CE_IDX(ucCE) | NFC_CMD_WP_NEG | NFC_CMD_DATA_PRESENT | NFC_CMD_ADDR_CYCLE_DUMMY | NFC_CMD_CYCLE_ONE | NFC_CMD_CODE0(0x90)));
    while(!(RREG(pFlashInfo, REG_NFC_INT_STAT) & (NFC_INT_ERR | NFC_INT_CMD_COMPLETE)));  // wait for command complete
    while(!(RREG(pFlashInfo, REG_NFC_INT_STAT) & (NFC_INT_ERR | NFC_INT_DATA_TRAN_COMPLETE)));    // wait for transfer complete

    if ((RREG(pFlashInfo, REG_NFC_INT_STAT) & NFC_INT_ERR) == 0)
    {
        device_id     = RREG(pFlashInfo, REG_NFC_DATA_PORT);
        device_extid  = RREG(pFlashInfo, REG_NFC_DATA_PORT);
	    NFC_DBG("DEBUG in %s device_id 0x%x\n", __func__, device_id);

        id_buff[0]   = device_id & 0xff;
        id_buff[1]   = (device_id >> 8) & 0xff;
        id_buff[2]   = (device_id >> 16) & 0xff;
        id_buff[3]   = (device_id >> 24) & 0xff;
        id_buff[4]   = device_extid & 0xff;
        id_buff[5]   = (device_extid >> 8) & 0xff;
        id_buff[6]   = (device_extid >> 16) & 0xff;
        id_buff[7]   = (device_extid >> 24) & 0xff;
	    NFC_DBG("DEBUG in %s ID:0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x\n", __func__, id_buff[0], id_buff[1], id_buff[2], id_buff[3], id_buff[4], id_buff[5], id_buff[6], id_buff[7]);

        if(pucBuffer != NULL)
			memcpy(pucBuffer, id_buff, 8);
        
		if(id != NULL)
			*id = device_id;

		if(extid != NULL)
			*extid = device_extid;
        
		NFC_DBG(KERN_ERR "%s-\r\n", __func__);
        erReturn = NAND_ERROR_OK;
    }
	else
		erReturn = NAND_ERROR_SYSTEM;

    NFC_DBG(KERN_ERR "%s-\r\n", __func__);
    return erReturn;
}

int NVT_nand_nfc_ReadStatus(PNAND_FLASH_PARAMETER pFlashInfo, unsigned char* pucBuffer, unsigned int* status)
{
    uint8_t  st_buff[2];
	unsigned char ucCE = dwCE;
	
    NFC_DBG("DEBUG in %s\n", __func__);

	WREG(pFlashInfo, REG_NFC_CFG0, ((S_REG_NFC_CFG0 & ~0xFFFFFFF0) | pFlashInfo->uiSimpleReadTiming));
	WREG(pFlashInfo, REG_NFC_Fine_Tune, pFlashInfo->uiSimpleFineTuneTiming);

	WREG(pFlashInfo, REG_NFC_CFG1, (S_REG_NFC_CFG1 & ~NFC_CFG1_RS_ECC_ENABLE));

    WREG(pFlashInfo, REG_NFC_TRAN_MODE, (NFC_TRAN_MODE_CE_IDX(ucCE) | NFC_TRAN_MODE_BLK_SIZE(1))); // transfer 1 byte (status)
    WREG(pFlashInfo, REG_NFC_INT_STAT, (NFC_INT_CMD_COMPLETE | NFC_INT_DATA_TRAN_COMPLETE));   // clear command complete & transfer complete status
    WREG(pFlashInfo, REG_NFC_CMD, (NFC_CMD_CE_IDX(ucCE) | NFC_CMD_WP_DISABLE | NFC_CMD_WP_KEEP | NFC_CMD_DATA_PRESENT | NFC_CMD_CYCLE_ONE | NFC_CMD_CODE0(0x70)));
    while(!(RREG(pFlashInfo, REG_NFC_INT_STAT) & (NFC_INT_ERR | NFC_INT_CMD_COMPLETE)));  // wait for command complete
    while(!(RREG(pFlashInfo, REG_NFC_INT_STAT) & (NFC_INT_ERR | NFC_INT_DATA_TRAN_COMPLETE)));    // wait for transfer complete

    st_buff[0] = (RREG(pFlashInfo, REG_NFC_DATA_PORT)) & 0xFF;
    NFC_DBG("DEBUG in %s status 0x%02x\n", __func__, st_buff[0]);

    if(status)
        *status = st_buff[0];
    if(pucBuffer)
        memcpy(pucBuffer, st_buff, 1);

    return NAND_ERROR_OK;
}

int NVT_nand_nfc_ReadPage(PNAND_FLASH_PARAMETER pFlashInfo, unsigned int buffer_phys, int iPageAddr, unsigned int* puiECCbitError)
{
//    NAND_ER erRetrun = NAND_ERROR_OK;
//    unsigned short usNumSubPage = 0;
//    unsigned short usSubPageIndex = 0;
//    unsigned short usDataIndex = 0;
//    unsigned char  ucCorrectedErr = 0;

	unsigned long ulStatusReg = 0;
	unsigned long ulStatusReg1 = 0;
//	unsigned long ulStatusReg2 = 0;
	unsigned char ucCE = dwCE;
//	unsigned char ucMaxErrorCount = 0;

NFC_DBG("DEBUG in %s\r\n", __func__);
NFC_DBG("ReadPage 0x%x\r\n Buff:0x%x", iPageAddr, buffer_phys);

#if NFC_USE_HW_RESET
	int	  i,j;
	REG_NFC_RESET = REG_NFC_RESET |(( 0x01 )<<25 ) ;
	for(j=0 ; j < 1000 ; j++);
	REG_NFC_RESET =   REG_NFC_RESET & (~(( 0x01 )<<25 )) ;

	REG_NFC_CFG0 =	( S_REG_NFC_CFG0_Read) ; //& 0x00FEFF0F) | 0x32000020; // 0x11100133 ;
	REG_NFC_CFG1 = S_REG_NFC_CFG1 ;
	REG_NFC_SYSCTRL = S_REG_NFC_SYSCTRL;
	REG_NFC_SYSCTRL1 = S_REG_NFC_SYSCTRL1 ; //| 0x800 ;
	REG_NFC_SYSCTRL2 = S_REG_NFC_SYSCTRL2 ;
	REG_NFC_Fine_Tune = S_REG_NFC_Fine_Tune ; //| 0xc0 ; //S_REG_NFC_Fine_Tune;
#else
	WREG(pFlashInfo, REG_NFC_SW_RESET, 7);
	while(RREG(pFlashInfo, REG_NFC_SW_RESET) != 0);
#endif

	//reset ecc decode
//	REG_NFC_CFG1 |= (0x01<<22);
	WREG(pFlashInfo, REG_NFC_CFG1, (RREG(pFlashInfo, REG_NFC_CFG1) | (0x01<<22)));
//	REG_NFC_CFG1 &= ~(0x01<<22);
	WREG(pFlashInfo, REG_NFC_CFG1, (RREG(pFlashInfo, REG_NFC_CFG1) & ~(0x01<<22)));

//	  REG_NFC_SYSCTRL2 = REG_NFC_SYSCTRL2 | (1<<6) ; // Set Check Blank function
	WREG(pFlashInfo, REG_NFC_SYSCTRL2, ((1<<6) | (pFlashInfo->subpageoob_size - 2) | 0x800 | 0x400)) ; // Set Check Blank function

	//enable Free run mechanism
	WREG(pFlashInfo, REG_NFC_SYSCTRL1, (NFC_SYS_CTRL1_FREE_RUN | NFC_SYS_CTRL1_NEW_VERSION));
	//set 1: for new free-run read: RS and BCH one page 1 time
//	REG_NFC_SYSCTRL1 |= NFC_SYS_CTRL1_BCH16_ENABLE_NEW_ARCH;
	WREG(pFlashInfo, REG_NFC_SYSCTRL1, (RREG(pFlashInfo, REG_NFC_SYSCTRL1) | NFC_SYS_CTRL1_BCH16_ENABLE_NEW_ARCH));
	//set 1: disable "Remove extra 2 dummy bytes  of extra"
//	REG_NFC_SYSCTRL1 |= NFC_SYS_CTRL1_DELAY_LATCH_1T;
	WREG(pFlashInfo, REG_NFC_SYSCTRL1, (RREG(pFlashInfo, REG_NFC_SYSCTRL1) | NFC_SYS_CTRL1_DELAY_LATCH_1T));
    //***	REG_NFC_SYSCTRL1 |= NFC_SYS_CTRL1_DELAY_LATCH_2T;

	WREG(pFlashInfo, REG_NFC_CFG0, ((S_REG_NFC_CFG0 & ~0xFFFFFFF0) | pFlashInfo->uiSimpleReadTiming));
	WREG(pFlashInfo, REG_NFC_Fine_Tune, pFlashInfo->uiSimpleFineTuneTiming);

	WREG(pFlashInfo, REG_NFC_CFG0, (RREG(pFlashInfo, REG_NFC_CFG0) & ~NFC_CFG0_DATAWIDTH_16));
    WREG(pFlashInfo, REG_NFC_SYSCTRL, (NFC_SYS_CTRL_EXTRA_SIZE(pFlashInfo->subpageoob_size) | NFC_SYS_CTRL_PAGE_2048 | NFC_SYS_CTRL_BLK_128K | NFC_SYS_CTRL_8_BIT_NAND));

	// Set Row Address
	WREG(pFlashInfo, REG_NFC_ROW_ADDR, iPageAddr);
	// Set Col Address to subpage0 (0)
	WREG(pFlashInfo, REG_NFC_COL_ADDR, 0);

	// NFC_TRAN_MODE_XTRA_DATA_COUNT_16 : This config will not use when BCH is ready
	WREG(pFlashInfo, REG_NFC_TRAN_MODE, (NFC_TRAN_MODE_CE_IDX(ucCE) | NFC_TRAN_MODE_RAND_ACC_CMD_CYCLE_TWO |
		NFC_TRAN_MODE_ECC_ENABLE | NFC_TRAN_MODE_ECC_RESET |
		NFC_TRAN_MODE_XTRA_DATA_COUNT_16 | NFC_TRAN_MODE_TRANSFER_BYTE_COUNT(pFlashInfo->page_size) |
		NFC_TRAN_MODE_START_WAIT_RDY | NFC_TRAN_MODE_DATA_SEL_DMA));

	// Set random access command
	WREG(pFlashInfo, REG_NFC_RAND_ACC_CMD, (NFC_RAND_ACC_CMD_CODE1(0xe0) | NFC_RAND_ACC_CMD_CODE0(0x05) | NFC_RAND_ACC_CMD_COL_ADDR(pFlashInfo->page_size)));

	// Set DMA Destination
	WREG(pFlashInfo, REG_NFC_DMA_ADDR, buffer_phys);

	// Set XTRA Buffer
	WREG(pFlashInfo, REG_NFC_XTRA_ADDR, buffer_phys + pFlashInfo->page_size);

//	REG_NFC_AHB_BURST_SIZE &= ~NFC_AHB_MASTER_MSK;
//	REG_NFC_AHB_BURST_SIZE |= NFC_AHB_MASTER_16_8_4BEAT;
	WREG(pFlashInfo, REG_NFC_AHB_BURST_SIZE, ((RREG(pFlashInfo, REG_NFC_AHB_BURST_SIZE) & ~NFC_AHB_MASTER_MSK) | NFC_AHB_MASTER_16_8_4BEAT));

	if(pFlashInfo->ucTypicalECCType == NAND_ECC_TYPE_REEDSOLOMON)
	{
		NFC_DBG("R RS\r\n");	
		WREG(pFlashInfo, REG_NFC_CFG1, (RREG(pFlashInfo, REG_NFC_CFG1) | NFC_CFG1_RS_ECC_ENABLE));	 // Macoto 0615
		WREG(pFlashInfo, REG_NFC_SYSCTRL2, (RREG(pFlashInfo, REG_NFC_SYSCTRL2) & (~(1<<7))));	// enable ecc correction
		WREG(pFlashInfo, REG_NFC_SYSCTRL, (RREG(pFlashInfo, REG_NFC_SYSCTRL) | NFC_SYS_CTRL_ECC_RS | NFC_SYS_CTRL_SUBPAGE_512));
		WREG(pFlashInfo, REG_NFC_SYSCTRL1, (RREG(pFlashInfo, REG_NFC_SYSCTRL1) | NFC_SYS_CTRL1_REMOVE_EXTRA_2DUMMY_BYTE_DIS));

	}
	else if(pFlashInfo->ucTypicalECCType == NAND_ECC_TYPE_BCH54)
	{
		NFC_DBG("R BCH54\r\n");
		WREG(pFlashInfo, REG_NFC_CFG1, (RREG(pFlashInfo, REG_NFC_CFG1) & ~(NFC_CFG1_RS_ECC_ENABLE)));
		WREG(pFlashInfo, REG_NFC_CFG1, (RREG(pFlashInfo, REG_NFC_CFG1) | NFC_CFG1_BCH_ECC_ENABLE | NFC_CFG1_BCH_ENABLE_PL));

		WREG(pFlashInfo, REG_NFC_SYSCTRL2, (RREG(pFlashInfo, REG_NFC_SYSCTRL2) & (~(1<<7))));	// Macoto enable correct
 
		//printf("REG_NFC_SYSCTRL2:%d\r\n", REG_NFC_SYSCTRL2);
		WREG(pFlashInfo, REG_NFC_SYSCTRL, (RREG(pFlashInfo, REG_NFC_SYSCTRL) | NFC_SYS_CTRL_ECC_BCH | NFC_SYS_CTRL_SUBPAGE_1024));
	}
	else if(pFlashInfo->ucTypicalECCType == NAND_ECC_TYPE_NONE_54)
	{
		NFC_DBG("R NONE-BCH54\r\n");
		WREG(pFlashInfo, REG_NFC_CFG1, (RREG(pFlashInfo, REG_NFC_CFG1) & ~(NFC_CFG1_RS_ECC_ENABLE)));	// Macoto 0615
		WREG(pFlashInfo, REG_NFC_CFG1, (RREG(pFlashInfo, REG_NFC_CFG1) | NFC_CFG1_BCH_ECC_ENABLE | NFC_CFG1_BCH_ENABLE_PL));

		WREG(pFlashInfo, REG_NFC_SYSCTRL, (RREG(pFlashInfo, REG_NFC_SYSCTRL) | NFC_SYS_CTRL_ECC_BCH | NFC_SYS_CTRL_SUBPAGE_1024));

		WREG(pFlashInfo, REG_NFC_SYSCTRL2, (RREG(pFlashInfo, REG_NFC_SYSCTRL2) | (1<<7)));		// Macoto Disable correct
		WREG(pFlashInfo, REG_NFC_SYSCTRL2, (RREG(pFlashInfo, REG_NFC_SYSCTRL2) | 0x80)); //RS off
		WREG(pFlashInfo, REG_NFC_SYSCTRL2, (RREG(pFlashInfo, REG_NFC_SYSCTRL2) & ~0x800)); //It should be setted to 0 when BCH is selected.

	}
	else if(pFlashInfo->ucTypicalECCType == NAND_ECC_TYPE_NONE_16)
	{
		NFC_DBG("R NONE-RS16\r\n");
		WREG(pFlashInfo, REG_NFC_SYSCTRL2, (RREG(pFlashInfo, REG_NFC_SYSCTRL2) | (1<<7))) ;		// Macoto Disable correct
	}

	WREG(pFlashInfo, REG_NFC_INT_STAT, NFC_INT_ALL);
#if NFC_NAND_INTERRUPT
	WREG(pFlashInfo, REG_NFC_INT_ENABLE, NFC_INT_ERR | NFC_INT_MEM_TRAN_COMPLETE  | NFC_INT_ENABLE);	
#endif

	// Set DMA Control
	WREG(pFlashInfo, REG_NFC_DMA_CTRL, (NFC_DMA_CTRL_READ | pFlashInfo->page_size));  // Free Run mode does not need to -1

	// Set command
	WREG(pFlashInfo, REG_NFC_CMD, (NFC_CMD_CE_IDX(ucCE) | NFC_CMD_DATA_PRESENT /*| NFC_CMD_WP_NEG */ | NFC_CMD_WP_KEEP | NFC_CMD_FREERUN |
	NFC_CMD_ADDR_CYCLE_COL_ROW | NFC_CMD_END_WAIT_BUSY_TO_RDY |
	NFC_CMD_CYCLE_TWO | NFC_CMD_CODE1(0x30) | NFC_CMD_CODE0(0x00)));

#if NFC_NAND_INTERRUPT
	if(NVT_nand_nfc_WaitForCmdComplete(pFlashInfo, NFC_INT_ERR | NFC_INT_MEM_TRAN_COMPLETE) != NAND_ERROR_OK)
	{
		printk("[NFC]Read page operation timeout\r\n");
		return NAND_ERROR_TIMEOUT;
	}
#else
	// wait for system memory transfer complete
	while( !(RREG(pFlashInfo, REG_NFC_INT_STAT) & (NFC_INT_ERR_RW | NFC_INT_MEM_TRAN_COMPLETE) ) );
	while( !(RREG(pFlashInfo, REG_NFC_INT_STAT) & (NFC_INT_ERR_RW | NFC_INT_FREE_RUN_COMPLETE) ) );
#endif	
	
	ulStatusReg = RREG(pFlashInfo, REG_NFC_INT_STAT);

	WREG(pFlashInfo, REG_NFC_CFG1, (RREG(pFlashInfo, REG_NFC_CFG1) & ~NFC_CFG1_RS_ECC_ENABLE));
	WREG(pFlashInfo, REG_NFC_CFG1, (RREG(pFlashInfo, REG_NFC_CFG1) & ~NFC_CFG1_BCH_ECC_ENABLE));

	WREG(pFlashInfo, REG_NFC_INT_STAT, NFC_INT_ALL);

#if NFC_NAND_VERIFICATION_USED	
	// Check blank page
	if (((ulStatusReg >> 22) & 0x01) == 0x01)
	{
//		  printf("Blank page found:B:%d P:%d\n\r", dwPageAddr / NandInfo.PagesPerBlock, dwPageAddr % NandInfo.PagesPerBlock);
		*pbBlankPage = TRUE;
		if((((NFC_ERR_CNT0 >> 20) & 0x3) == 0) && (((NFC_ERR_CNT1 >> 20) & 0x3) == 0))
		{
			printk("Blank page check is failed:%x  %x	%x", ulStatusReg, NFC_ERR_CNT0, NFC_ERR_CNT1);
			while(1);
		}
	}
	else
		*pbBlankPage = FALSE;

	if(puiECCbitError)
		*puiECCbitError = NVT_nand_nfc_GetECCBitsAndPos(pucErrorBitArray);

	//print ECN_Page
	if ((*pucMaxErrorCount > 0x00 ) && ((pFlashInfo->ucTypicalECCType != NAND_ECC_TYPE_NONE_54) && ((pFlashInfo->ucTypicalECCType != NAND_ECC_TYPE_NONE_16))))
	{
		printk("\r\n B:%4d P:%3d   ", dwPageAddr / NandInfo.PagesPerBlock, dwPageAddr % NandInfo.PagesPerBlock);
		printk("Byte error:%d", *pucMaxErrorCount);

		printk("\nECC0:0x%8x	ECC1:0x%8x\r\n", NFC_ERR_CNT0, NFC_ERR_CNT1);
	}
#else
		if(puiECCbitError)
			*puiECCbitError = NVT_nand_nfc_GetECCBitsAndPos(pFlashInfo, NULL);
#endif
		
	if (ulStatusReg & NFC_INT_FR_UNCORECTABLE)
	{
		printk("Uncorrectable ECC B:%d P:%d\r\n", iPageAddr / pFlashInfo->page_per_block, iPageAddr % pFlashInfo->page_per_block);
		printk("ulStatusReg:0x%lx\r\n", ulStatusReg);
		NVT_nand_nfc_DumpRegister(pFlashInfo);
		return NAND_ERROR_UNCORRECTABLE_ECC;
	}
		
		if ((ulStatusReg & NFC_INT_ERR) != 0)
		{
			printk("REG_NFC_INT_STAT Error !!\n");
			printk("Status Reg:%ld", ulStatusReg);
			printk("Status Reg1:%ld\r\n", ulStatusReg1);
			return NAND_ERROR_SYSTEM;
		}

	return NAND_ERROR_OK;
}
int NVT_nand_nfc_EraseBlock(PNAND_FLASH_PARAMETER pFlashInfo, int iPageAddr)
{
	unsigned char ucCE = dwCE;
    NFC_DBG("DEBUG in %s page_addr 0x%x\n", __func__, iPageAddr);

	if((iPageAddr % pFlashInfo->page_per_block) != 0)
	{
		printk("[NFC]A worng page address(0x%x) found during a erase operation.", iPageAddr);
		return NAND_ERROR_SYSTEM;
	}

	WREG(pFlashInfo, REG_NFC_INT_STAT, NFC_INT_ALL);
#if NFC_NAND_INTERRUPT
	WREG(pFlashInfo, REG_NFC_INT_ENABLE, NFC_INT_ERR | NFC_INT_CMD_COMPLETE | NFC_INT_ENABLE);	
#endif

	WREG(pFlashInfo, REG_NFC_CFG0, ((S_REG_NFC_CFG0 & ~0xFFFFFFF0) | pFlashInfo->uiSimpleProgramTiming));
	WREG(pFlashInfo, REG_NFC_Fine_Tune, pFlashInfo->uiSimpleFineTuneTiming);

	WREG(pFlashInfo, REG_NFC_CFG1, (S_REG_NFC_CFG1 & ~NFC_CFG1_RS_ECC_ENABLE));

    WREG(pFlashInfo, REG_NFC_ROW_ADDR, iPageAddr);
    WREG(pFlashInfo, REG_NFC_COL_ADDR, 0);

    WREG(pFlashInfo, REG_NFC_CMD, (NFC_CMD_WP_DISABLE | NFC_CMD_WP_KEEP | NFC_CMD_CE_IDX(ucCE) | NFC_CMD_ADDR_CYCLE_ROW | NFC_CMD_END_WAIT_BUSY_TO_RDY |
        NFC_CMD_CYCLE_TWO | NFC_CMD_CODE1(0xd0) | NFC_CMD_CODE0(0x60)));

#if NFC_NAND_INTERRUPT
	if(NVT_nand_nfc_WaitForCmdComplete(pFlashInfo, NFC_INT_ERR | NFC_INT_CMD_COMPLETE) != NAND_ERROR_OK)
	{
		printk("[NFC]Erase operation timeout\r\n");
		return NAND_ERROR_TIMEOUT;
	}
#else
	while(!(RREG(pFlashInfo, REG_NFC_INT_STAT) & (NFC_INT_ERR | NFC_INT_CMD_COMPLETE)));	// wait for command complete
#endif	

	NVT_nand_nfc_WaitFlashReady(pFlashInfo);

	if (RREG(pFlashInfo, REG_NFC_INT_STAT) & NFC_INT_ERR)
	{
		printk("[NFC]Erase failed:0x%x Block:%d\r\n", RREG(pFlashInfo, REG_NFC_INT_STAT), iPageAddr / pFlashInfo->page_per_block);
		return NAND_ERROR_SYSTEM;
	}

#if NFC_NAND_VERIFICATION_USED
    // check status
    unsigned char ucStatus; 
    NVT_nand_nfc_ReadStatus(pFlashInfo, NULL, &ucStatus);
    if(ucStatus & 0x40)
    {   // ready
    	//printk("0x40 REG_NFC_INT_STAT = %lx\n",REG_NFC_INT_STAT);
        if(ucStatus & 1)
        {   // fail
            printk("erase %08lx fail, mark defective\n", iPageAddr / pFlashInfo->page_per_block);
            return NAND_ERROR_FLASH;
        }
        break;
    }
#endif

    return NAND_ERROR_OK;
}

int NVT_nand_nfc_WritePage(PNAND_FLASH_PARAMETER pFlashInfo, unsigned int buffer_phys, int iPageAddr)
{
    NAND_ER erRetrun = NAND_ERROR_OK;
#if NFC_NAND_VERIFICATION_USED
	unsigned long ulStatusReg = 0;
#endif

	unsigned char ucCE = dwCE;
    NFC_DBG("DEBUG in %s PageAddr:%d\n", __func__, iPageAddr);

#if NFC_USE_HW_RESET
	REG_NFC_RESET =   REG_NFC_RESET |(( 0x01 )<<25 ) ;
	for(j=0 ; j < 1000 ; j++);
	REG_NFC_RESET =   REG_NFC_RESET & (~(( 0x01 )<<25 )) ;

	REG_NFC_CFG0 =	S_REG_NFC_CFG0_Program; //
	REG_NFC_CFG1 = S_REG_NFC_CFG1 ;
	REG_NFC_SYSCTRL = S_REG_NFC_SYSCTRL;
	REG_NFC_SYSCTRL1 = S_REG_NFC_SYSCTRL1 ; //| 0x800 ;
	REG_NFC_SYSCTRL2 = S_REG_NFC_SYSCTRL2 ;
	REG_NFC_Fine_Tune = S_REG_NFC_Fine_Tune;// | 0xc000 ; //S_REG_NFC_Fine_Tune;
#else
	WREG(pFlashInfo, REG_NFC_SW_RESET, 7);
	while(RREG(pFlashInfo, REG_NFC_SW_RESET) != 0);
#endif

	// Set DMA Destination
	WREG(pFlashInfo, REG_NFC_DMA_ADDR, buffer_phys);

	// Set XTRA Buffer
	WREG(pFlashInfo, REG_NFC_XTRA_ADDR, (buffer_phys + pFlashInfo->page_size));

	//reset ecc decode
	WREG(pFlashInfo, REG_NFC_CFG1, (S_REG_NFC_CFG1 | NFC_CFG1_ECC_ENCODE_RESET));
	WREG(pFlashInfo, REG_NFC_CFG1, (RREG(pFlashInfo, REG_NFC_CFG1) & ~NFC_CFG1_ECC_ENCODE_RESET));

	WREG(pFlashInfo, REG_NFC_CFG0, ((S_REG_NFC_CFG0 & ~0xFFFFFFF0) | pFlashInfo->uiSimpleProgramTiming));
	WREG(pFlashInfo, REG_NFC_Fine_Tune, pFlashInfo->uiSimpleFineTuneTiming);

	// Set Row and Column Address
	WREG(pFlashInfo, REG_NFC_ROW_ADDR, iPageAddr);
	WREG(pFlashInfo, REG_NFC_COL_ADDR, 0);

	WREG(pFlashInfo, REG_NFC_AHB_BURST_SIZE, (RREG(pFlashInfo, REG_NFC_AHB_BURST_SIZE) & ~NFC_AHB_MASTER_MSK));
	WREG(pFlashInfo, REG_NFC_AHB_BURST_SIZE, (RREG(pFlashInfo, REG_NFC_AHB_BURST_SIZE) | NFC_AHB_MASTER_16_8_4BEAT));

	WREG(pFlashInfo, REG_NFC_CFG0, (RREG(pFlashInfo, REG_NFC_CFG0) & ~NFC_CFG0_DATAWIDTH_16));
	WREG(pFlashInfo, REG_NFC_SYSCTRL, (NFC_SYS_CTRL_EXTRA_SIZE(pFlashInfo->subpageoob_size) | NFC_SYS_CTRL_PAGE_2048 | NFC_SYS_CTRL_BLK_128K | NFC_SYS_CTRL_8_BIT_NAND));

	if(pFlashInfo->ucTypicalECCType  == NAND_ECC_TYPE_REEDSOLOMON)
	{
		NFC_DBG("RS W\n");
	
		WREG(pFlashInfo, REG_NFC_SYSCTRL1, (NFC_SYS_CTRL1_FREE_RUN | NFC_SYS_CTRL1_NEW_VERSION | NFC_SYS_CTRL1_FREE_RUN_WRITE));
		//set 1: for new free-run read: RS and BCH one page 1 time
		WREG(pFlashInfo, REG_NFC_SYSCTRL1, (RREG(pFlashInfo, REG_NFC_SYSCTRL1) | NFC_SYS_CTRL1_BCH16_ENABLE_NEW_ARCH));
		//set 1: disable "Remove extra 2 dummy bytes  of extra"
		WREG(pFlashInfo, REG_NFC_SYSCTRL1, (RREG(pFlashInfo, REG_NFC_SYSCTRL1) | NFC_SYS_CTRL1_REMOVE_EXTRA_2DUMMY_BYTE_DIS));
		WREG(pFlashInfo, REG_NFC_SYSCTRL1, (RREG(pFlashInfo, REG_NFC_SYSCTRL1) | NFC_SYS_CTRL1_DELAY_LATCH_1T));

		WREG(pFlashInfo, REG_NFC_TRAN_MODE, (NFC_TRAN_MODE_ECC_ENABLE | NFC_TRAN_MODE_ECC_RESET |
							NFC_TRAN_MODE_KEEP_CE | NFC_TRAN_MODE_CE_IDX(ucCE) |
							NFC_TRAN_MODE_RAND_ACC_CMD_CYCLE_ONE | NFC_TRAN_MODE_XTRA_DATA_COUNT_16 |
							NFC_TRAN_MODE_BLK_SIZE(pFlashInfo->page_size) | NFC_TRAN_MODE_DATA_SEL_DMA |
							NFC_TRAN_MODE_WRITE));

		WREG(pFlashInfo, REG_NFC_CFG1, (RREG(pFlashInfo, REG_NFC_CFG1) | NFC_CFG1_RS_ECC_ENABLE));	 // Macoto 0615
		WREG(pFlashInfo, REG_NFC_SYSCTRL2, 0);

	}
	else if(pFlashInfo->ucTypicalECCType ==NAND_ECC_TYPE_BCH54)
	{
		NFC_DBG("BCH W\n");
		WREG(pFlashInfo, REG_NFC_SYSCTRL1, (NFC_SYS_CTRL1_FREE_RUN | NFC_SYS_CTRL1_FREE_RUN_WRITE));
		//set 1: for new free-run read: RS and BCH one page 1 time
		WREG(pFlashInfo, REG_NFC_SYSCTRL1, (RREG(pFlashInfo, REG_NFC_SYSCTRL1) | NFC_SYS_CTRL1_BCH16_ENABLE_NEW_ARCH));
		//set 1: disable "Remove extra 2 dummy bytes  of extra"
  //	  REG_NFC_SYSCTRL1 |= NFC_SYS_CTRL1_REMOVE_EXTRA_2DUMMY_BYTE_DIS;
		WREG(pFlashInfo, REG_NFC_SYSCTRL1, (RREG(pFlashInfo, REG_NFC_SYSCTRL1) | NFC_SYS_CTRL1_DELAY_LATCH_1T));

		WREG(pFlashInfo, REG_NFC_TRAN_MODE, (NFC_TRAN_MODE_ECC_ENABLE | NFC_TRAN_MODE_ECC_RESET |
							NFC_TRAN_MODE_KEEP_CE | NFC_TRAN_MODE_CE_IDX(ucCE) |
							NFC_TRAN_MODE_RAND_ACC_CMD_CYCLE_ONE | NFC_TRAN_MODE_XTRA_DATA_COUNT_16 |
							NFC_TRAN_MODE_BLK_SIZE(pFlashInfo->page_size) | NFC_TRAN_MODE_DATA_SEL_DMA |
							NFC_TRAN_MODE_WRITE | NFC_TRAN_MODE_ECC_ENABLE | NFC_TRAN_MODE_ECC_RESET));

		WREG(pFlashInfo, REG_NFC_CFG1, (RREG(pFlashInfo, REG_NFC_CFG1) & ~NFC_CFG1_RS_ECC_ENABLE));	  // Macoto 0615
		WREG(pFlashInfo, REG_NFC_CFG1, (RREG(pFlashInfo, REG_NFC_CFG1) | NFC_CFG1_BCH_ECC_ENABLE | NFC_CFG1_BCH_ENABLE_PL));
		WREG(pFlashInfo, REG_NFC_CFG1, (RREG(pFlashInfo, REG_NFC_CFG1) | (NFC_CFG1_LITTLE_ENDIAN_XTRA | NFC_CFG1_LITTLE_ENDIAN)));    // Macoto 0615
		WREG(pFlashInfo, REG_NFC_SYSCTRL2, 0);
		WREG(pFlashInfo, REG_NFC_SYSCTRL, (RREG(pFlashInfo, REG_NFC_SYSCTRL) | NFC_SYS_CTRL_ECC_BCH | NFC_SYS_CTRL_SUBPAGE_1024));
	}
	else if(pFlashInfo->ucTypicalECCType == NAND_ECC_TYPE_NONE_54)
	{
		NFC_DBG("NBCH W\n");
	
		WREG(pFlashInfo, REG_NFC_SYSCTRL1, (NFC_SYS_CTRL1_FREE_RUN | NFC_SYS_CTRL1_FREE_RUN_WRITE));
		//set 1: for new free-run read: RS and BCH one page 1 time
		WREG(pFlashInfo, REG_NFC_SYSCTRL1, (RREG(pFlashInfo, REG_NFC_SYSCTRL1) | NFC_SYS_CTRL1_BCH16_ENABLE_NEW_ARCH));
		//set 1: disable "Remove extra 2 dummy bytes  of extra"
		WREG(pFlashInfo, REG_NFC_SYSCTRL1, (RREG(pFlashInfo, REG_NFC_SYSCTRL1) | NFC_SYS_CTRL1_DELAY_LATCH_1T));

		WREG(pFlashInfo, REG_NFC_CFG1, (RREG(pFlashInfo, REG_NFC_CFG1) & ~NFC_CFG1_RS_ECC_ENABLE));	  // Macoto 0615
//		  REG_NFC_TRAN_MODE = REG_NFC_TRAN_MODE | NFC_TRAN_MODE_ECC_RESET;

		WREG(pFlashInfo, REG_NFC_TRAN_MODE, (NFC_TRAN_MODE_KEEP_CE | NFC_TRAN_MODE_CE_IDX(ucCE) |
							NFC_TRAN_MODE_RAND_ACC_CMD_CYCLE_ONE | NFC_TRAN_MODE_XTRA_DATA_COUNT_16 |
							NFC_TRAN_MODE_BLK_SIZE(pFlashInfo->page_size) | NFC_TRAN_MODE_DATA_SEL_DMA |
							NFC_TRAN_MODE_WRITE | NFC_TRAN_MODE_ECC_ENABLE	| NFC_TRAN_MODE_ECC_RESET));

		WREG(pFlashInfo, REG_NFC_CFG1, (RREG(pFlashInfo, REG_NFC_CFG1) & ~NFC_CFG1_RS_ECC_ENABLE));	  // Macoto 0615
		WREG(pFlashInfo, REG_NFC_CFG1, (RREG(pFlashInfo, REG_NFC_CFG1) | NFC_CFG1_LITTLE_ENDIAN_XTRA | NFC_CFG1_LITTLE_ENDIAN));    // Macoto 0615
		WREG(pFlashInfo, REG_NFC_CFG1, (RREG(pFlashInfo, REG_NFC_CFG1) | NFC_CFG1_BCH_ECC_ENABLE | NFC_CFG1_BCH_ENABLE_PL));

		WREG(pFlashInfo, REG_NFC_SYSCTRL2, (RREG(pFlashInfo, REG_NFC_SYSCTRL2) | 0x80)); //RS off
		WREG(pFlashInfo, REG_NFC_SYSCTRL2, (RREG(pFlashInfo, REG_NFC_SYSCTRL2) & ~0x800)); //It should be setted to 0 when BCH is selected.
		WREG(pFlashInfo, REG_NFC_SYSCTRL, (RREG(pFlashInfo, REG_NFC_SYSCTRL) | NFC_SYS_CTRL_ECC_BCH | NFC_SYS_CTRL_SUBPAGE_1024));

//		  NFCHIP_Set_16extra(pulExtraBuffer);
	}
	else if(pFlashInfo->ucTypicalECCType == NAND_ECC_TYPE_NONE_16)
	{
		NFC_DBG("NRS W\n");
		WREG(pFlashInfo, REG_NFC_SYSCTRL1, (NFC_SYS_CTRL1_FREE_RUN | NFC_SYS_CTRL1_NEW_VERSION | NFC_SYS_CTRL1_FREE_RUN_WRITE));
		//set 1: for new free-run read: RS and BCH one page 1 time
		WREG(pFlashInfo, REG_NFC_SYSCTRL1, (RREG(pFlashInfo, REG_NFC_SYSCTRL1) | NFC_SYS_CTRL1_BCH16_ENABLE_NEW_ARCH));
		//set 1: disable "Remove extra 2 dummy bytes  of extra"
		WREG(pFlashInfo, REG_NFC_SYSCTRL1, (RREG(pFlashInfo, REG_NFC_SYSCTRL1) | NFC_SYS_CTRL1_REMOVE_EXTRA_2DUMMY_BYTE_DIS));
		WREG(pFlashInfo, REG_NFC_SYSCTRL1, (RREG(pFlashInfo, REG_NFC_SYSCTRL1) | NFC_SYS_CTRL1_DELAY_LATCH_1T));

	
		WREG(pFlashInfo, REG_NFC_CFG1, (RREG(pFlashInfo, REG_NFC_CFG1) & ~NFC_CFG1_RS_ECC_ENABLE));	  // Macoto 0615
//		  REG_NFC_TRAN_MODE = REG_NFC_TRAN_MODE | NFC_TRAN_MODE_ECC_RESET;

		WREG(pFlashInfo, REG_NFC_TRAN_MODE, (NFC_TRAN_MODE_KEEP_CE | NFC_TRAN_MODE_CE_IDX(ucCE) |
							NFC_TRAN_MODE_RAND_ACC_CMD_CYCLE_ONE | NFC_TRAN_MODE_XTRA_DATA_COUNT_16 |
							NFC_TRAN_MODE_BLK_SIZE(pFlashInfo->page_size) | NFC_TRAN_MODE_DATA_SEL_DMA |
							NFC_TRAN_MODE_WRITE));

		WREG(pFlashInfo, REG_NFC_CFG1, (RREG(pFlashInfo, REG_NFC_CFG1) & ~NFC_CFG1_RS_ECC_ENABLE));	  // Macoto 0615
		WREG(pFlashInfo, REG_NFC_CFG1, (RREG(pFlashInfo, REG_NFC_CFG1) | NFC_CFG1_LITTLE_ENDIAN_XTRA | NFC_CFG1_LITTLE_ENDIAN));    // Macoto 0615

		WREG(pFlashInfo, REG_NFC_SYSCTRL2, (RREG(pFlashInfo, REG_NFC_SYSCTRL2) | 0x800)); //RS off
//		  NFCHIP_Set_16extra(pulExtraBuffer);
	}
	else
	{
		printk("Unknown ECC type1\r\n");
		return NAND_ERROR_SYSTEM;
	}

	WREG(pFlashInfo, REG_NFC_INT_STAT, NFC_INT_ALL);
#if NFC_NAND_INTERRUPT
	WREG(pFlashInfo, REG_NFC_INT_ENABLE, NFC_INT_ERR | NFC_INT_FREE_RUN_COMPLETE | NFC_INT_ENABLE); 
#endif

	// Set random access command
	WREG(pFlashInfo, REG_NFC_RAND_ACC_CMD, (NFC_RAND_ACC_CMD_CODE0(0x85) | NFC_RAND_ACC_CMD_COL_ADDR(pFlashInfo->page_size)));

  //  PrintString("write 2K\r\n");
  //  NFC_DumpReg();
	
	// Set command, Queer.....
	WREG(pFlashInfo, REG_NFC_CMD, (NFC_CMD_CE_IDX(ucCE) | NFC_CMD_WP_NEG | NFC_CMD_WP_KEEP | //NFC_CMD_FREERUN |
		NFC_CMD_DATA_PRESENT | NFC_CMD_ADDR_CYCLE_COL_OTHER |
		NFC_CMD_CYCLE_ONE | NFC_CMD_CODE0(0x80)));
	
	//REG_NFC_DMA_CTRL = NandInfo.BytesPerPage;   // Modify for Extra data lose when DMA busy  0906
	WREG(pFlashInfo, REG_NFC_DMA_CTRL, pFlashInfo->page_size);	// Modify for Extra data lose when DMA busy  0906
	
	// wait for transfer complete
#if NFC_NAND_INTERRUPT
	erRetrun = (NVT_nand_nfc_WaitForCmdComplete(pFlashInfo, NFC_INT_ERR | NFC_INT_FREE_RUN_COMPLETE));
#else
	while(!(RREG(pFlashInfo, REG_NFC_INT_STAT) & (NFC_INT_ERR | NFC_INT_DATA_TRAN_COMPLETE)))
	while(!(RREG(pFlashInfo, REG_NFC_INT_STAT) & (NFC_INT_ERR |NFC_INT_FREE_RUN_COMPLETE)))
#endif

	NVT_nand_nfc_WaitFlashReady(pFlashInfo);

	if(erRetrun != NAND_ERROR_OK)
	{
		printk("[NFC]Write page operation timeout\r\n");
		return NAND_ERROR_TIMEOUT;
	}
	
#if NFC_NAND_VERIFICATION_USED
	ulStatusReg = RREG(pFlashInfo, REG_NFC_INT_STAT);
#endif

	WREG(pFlashInfo, REG_NFC_INT_STAT, NFC_INT_ERR | NFC_INT_FREE_RUN_COMPLETE);
	
	WREG(pFlashInfo, REG_NFC_CFG1, (RREG(pFlashInfo, REG_NFC_CFG1) & ~NFC_CFG1_RS_ECC_ENABLE));	  // Macoto 0615
	WREG(pFlashInfo, REG_NFC_CFG1, (RREG(pFlashInfo, REG_NFC_CFG1) & ~NFC_CFG1_BCH_ECC_ENABLE));  //Macoto 0614
	

#if NFC_NAND_VERIFICATION_USED
		if((ulStatusReg & NFC_INT_ERR) != 0)
		{
			printk("NFC_WritePage_FreeRun Fail !!\n");
			printk("\r\nStatus Reg:%ld", ulStatusReg);
			return NAND_ERROR_SYSTEM;
		}
	
		NVT_nand_nfc_ReadStatus(pFlashInfo, NULL, &ulStatusReg);
		if(ulStatusReg & 0x40)
		{	// ready
			if(ulStatusReg & 1)
			{	// fail
				//NFC_Reset(bChipNumber);
				printk("Status Fail !!\n");
				printk("\r\nPageAddr:0x%x Status Reg:%ld", iPageAddr, ulStatusReg);
				return NAND_ERROR_SYSTEM;
			} // end of if(bStatus & 1)
		} // end of if(bStatus & 0x40)
#endif

	NFC_DBG("DEBUG in %s-\n", __func__);

	return NAND_ERROR_OK;

}

#if NFC_NAND_INTERRUPT	
unsigned int NVT_nand_nfc_WaitForCmdComplete(PNAND_FLASH_PARAMETER pFlashInfo, unsigned int uiWaitForSignalType)
{
	while(!(RREG(pFlashInfo, REG_NFC_INT_STAT) & (uiWaitForSignalType)))	
	{
		if(down_timeout(&NVT_NFC_IRQ, HZ/8) == -ETIME)		
		{				
			if(!(RREG(pFlashInfo, REG_NFC_INT_STAT) & (uiWaitForSignalType)))
			{
				printk("NFC timeout happened\r\n"); 
				return NAND_ERROR_TIMEOUT;		
			}
		}
	}	
	return 0;
}

irqreturn_t NVT_nand_nfc_ISR(int irq, void *devid)
{
	PNAND_FLASH_PARAMETER pFlashInfo = (PNAND_FLASH_PARAMETER)devid;
	WREG(pFlashInfo, REG_NFC_INT_ENABLE, ~(RREG(pFlashInfo, REG_NFC_INT_STAT)));	
	up(&NVT_NFC_IRQ);	
    return IRQ_HANDLED;
}

unsigned int NVT_nand_nfc_InitInterrupt(PNAND_FLASH_PARAMETER pFlashInfo)
{
	NFC_DBG("%s+\r\n", __func__);
	if(request_irq(pFlashInfo->uiInterruptID, NVT_nand_nfc_ISR, IRQF_DISABLED, "NFC_INT", pFlashInfo) < 0)
	{
		printk("failed to request NFC IRQ:%d\n", pFlashInfo->uiInterruptID);
		return NAND_ERROR_SYSTEM;
	}

	sema_init(&NVT_NFC_IRQ,0);	
	NFC_DBG("%s-\r\n", __func__);
	return NAND_ERROR_OK;
}
#endif

unsigned int NVT_nand_nfc_GetECCBitsAndPos_RS(PNAND_FLASH_PARAMETER pFlashInfo, PECC_INFO pucErrorBitArray)
{
    unsigned char ucErrorCnt[8] = {0, 0, 0, 0, 0, 0, 0, 0};
    bool          bUncorrectable[8] = {0, 0, 0, 0, 0, 0, 0, 0};
    unsigned char i;
    unsigned char ucMaxECN = 0;
    unsigned char subpage_cnt = pFlashInfo->page_size / pFlashInfo->subpage_size;

    for ( i = 0; i< subpage_cnt ; i++)
    {
        if((i % 2 ) == 0)
        {

            ucErrorCnt[i] = (RREG(pFlashInfo, NFC_ERR_CNT0) >> (5 * (i / 2))) & 0x07;
            if(((RREG(pFlashInfo, NFC_ERR_CNT0) >> ((5 * (i / 2)) + 3)) & 0x03) == 0x2)
            {
                bUncorrectable[i] = true;
            }
            else
                bUncorrectable[i] = false;
        }
        else
        {
            ucErrorCnt[i] = (RREG(pFlashInfo, NFC_ERR_CNT1) >> (5 * (i / 2))) & 0x07;
            if(((RREG(pFlashInfo, NFC_ERR_CNT1) >> ((5 * (i / 2)) + 3)) & 0x03) == 0x2)
            {
                bUncorrectable[i] = true;
            }
            else
                bUncorrectable[i] = false;
        }
    }

    for ( i = 0 ; i < subpage_cnt ; i++ )
    {
        if(ucMaxECN < ucErrorCnt[i])
            ucMaxECN = ucErrorCnt[i];
    }

    if(pucErrorBitArray != NULL)
    {
        for ( i = 0; i< subpage_cnt ; i++)
        {
            pucErrorBitArray[i].usSymbolCount = ucErrorCnt[i];
            pucErrorBitArray[i].bUncorrectable = bUncorrectable[i];
        }
    }
    return ucMaxECN;
}

//return the maximum error bit of all decode unit
unsigned int NVT_nand_nfc_GetECCBitsAndPos_BCH54(PNAND_FLASH_PARAMETER pFlashInfo, PECC_INFO pucErrorBitArray)
{
    unsigned char ucErrorCnt[8] = {0, 0, 0, 0, 0, 0, 0, 0};
    bool          bUncorrectable[8] = {0, 0, 0, 0, 0, 0, 0, 0};
    unsigned char i;
    unsigned char ucMaxECN = 0;
    unsigned char subpage_cnt = pFlashInfo->page_size / pFlashInfo->subpage_size;

    for ( i = 0; i< subpage_cnt ; i++)
    {
        if((i & 1 ) == 0)
        {

            ucErrorCnt[i] = (RREG(pFlashInfo, NFC_ERR_CNT0) >> (5 * (i / 2))) & 0x1F;
//            printk("SubPage:%d uncorrectable:%d\r\n", i, ((NFC_ERR_CNT1 >> ((2 * (i / 2)) + 24)) & 0x03));
            if(((RREG(pFlashInfo, NFC_ERR_CNT0) >> ((2 * (i / 2)) + 24)) & 0x03) == 0x2)
            {
                bUncorrectable[i] = true;
//                printk("uncorrectable:%d\r\n", bUncorrectable[i]);
            }
            else
                bUncorrectable[i] = false;
        }
        else
        {
            ucErrorCnt[i] = (RREG(pFlashInfo, NFC_ERR_CNT1) >> (5 * (i / 2))) & 0x1F;

//            printk("SubPage:%d uncorrectable:%d\r\n", i, ((NFC_ERR_CNT1 >> ((2 * (i / 2)) + 24)) & 0x03));
            if(((RREG(pFlashInfo, NFC_ERR_CNT1) >> ((2 * (i / 2)) + 24)) & 0x03) == 0x2)
            {
                bUncorrectable[i] = true;
//                printk("uncorrectable:%d\r\n", bUncorrectable[i]);
            }
            else
                bUncorrectable[i] = false;
        }
    }

    for ( i = 0 ; i < subpage_cnt ; i++ )
    {
        if(ucMaxECN < ucErrorCnt[i])
            ucMaxECN = ucErrorCnt[i];
    }

    if(pucErrorBitArray != NULL)
    {
        for ( i = 0; i< subpage_cnt ; i++)
        {
            pucErrorBitArray[i].usSymbolCount = ucErrorCnt[i];
            pucErrorBitArray[i].bUncorrectable = bUncorrectable[i];

//            printk("%d uncorrectable:%d\r\n", i, bUncorrectable[i]);

        }
    }
    return ucMaxECN;
}

unsigned int NVT_nand_nfc_GetECCBitsAndPos(PNAND_FLASH_PARAMETER pFlashInfo, PECC_INFO pucErrorBitArray)
{
    if(pFlashInfo->ucTypicalECCType == NAND_ECC_TYPE_REEDSOLOMON)
    {
        return NVT_nand_nfc_GetECCBitsAndPos_RS(pFlashInfo, pucErrorBitArray);
    }
    else if(pFlashInfo->ucTypicalECCType == NAND_ECC_TYPE_BCH54)
    {
        return NVT_nand_nfc_GetECCBitsAndPos_BCH54(pFlashInfo, pucErrorBitArray);
    }

    return 0xFFFFFFFF;
}

void NVT_nand_nfc_ShowParameter(PNAND_FLASH_PARAMETER pFlashInfo)
{
	printk("chip_id:%x\r\n", pFlashInfo->chip_id);
	printk("Num blocks:%d\r\n", pFlashInfo->num_blocks);
	printk("Pages per block:%d\r\n", pFlashInfo->page_per_block);
	printk("Page Size:%d\r\n", pFlashInfo->page_size);
	printk("Sub Page Size:%d\r\n", pFlashInfo->subpage_size);
	printk("Spare Size per page:%d\r\n", pFlashInfo->oob_size);
	printk("Spare size per sub page Size:%d\r\n", pFlashInfo->subpageoob_size);
	printk("Column addr:%d\r\n", pFlashInfo->uiNumOfCol);
	printk("Row addr:%d\r\n", pFlashInfo->uiNumOfRow);
	printk("ECC capbility:%d\r\n", pFlashInfo->ucECCCorrectableBit);
	printk("ECC type:%d\r\n", pFlashInfo->ucTypicalECCType);
	printk("Fine tune:0x%x\r\n", pFlashInfo->uiSimpleFineTuneTiming);
	printk("Program timing:0x%x\r\n", pFlashInfo->uiSimpleProgramTiming);
	printk("Read timing:0x%x\r\n", pFlashInfo->uiSimpleReadTiming);
}

void NVT_nand_nfc_DumpRegister(PNAND_FLASH_PARAMETER pFlashInfo)
{
	unsigned int uiLoop;
	printk("Dump NAND flash controller register\r\n");

	for(uiLoop = 0 ; uiLoop < 0x140 ; uiLoop += 4)
	{	
		if((uiLoop % 16) == 0)
			printk("\r\n0x%08x     ", uiLoop);
		printk("%08x ", RREG(pFlashInfo, uiLoop));
	}
}
