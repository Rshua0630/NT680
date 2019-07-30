#include "nvt_nand_uboot.h"

unsigned char scan_ff_pattern[] = { 0xff };

NVT_NAND_INFO NvtNandInfo;

struct nand_bbt_descr delta_bbt_descr = {
    .options = 0,
    .offs = 0,
    .len = 1,
    .pattern = scan_ff_pattern
};

void NVT_nand_invalidata_buf(PNVT_NAND_INFO info)
{
	uint32_t addr = (uint32_t)info->data_buff;

	invalidate_dcache_range(addr, addr + info->data_buff_size);
}

void NVT_nand_flush_data_buf(PNVT_NAND_INFO info)
{
	uint32_t addr = (uint32_t)info->data_buff;

	flush_dcache_range(addr, addr + info->data_buff_size);
}

int NVT_nand_init_buff(PNVT_NAND_INFO nand_info)
{
	uint8_t *buf;
	const int size = NAND_MAX_PAGESIZE + NAND_MAX_OOBSIZE;

	nand_info->data_buff_size = roundup(size, ARCH_DMA_MINALIGN);

	/* DMA buffers */
	buf = (unsigned char*)memalign(ARCH_DMA_MINALIGN, nand_info->data_buff_size);
	if (!buf) {
		printf("NVT_NAND: Error allocating DMA buffers\n");
		return -ENOMEM;
	}

	memset(buf, 0, nand_info->data_buff_size);

	nand_info->data_buff = buf;
	nand_info->data_buff_phys = (unsigned int)buf;
	nand_info->buf_start = 0;

	return 0;
}

int board_nand_init(struct nand_chip *nand)
{
    int iFlashIndex;
    uint32_t uiFlashID, uiExtFlashID;
	extern NAND_FLASH_SUPPORT_LIST NandFlashSupportList;
	
		//    uint32_t uiBlockIndex;

//	gd_t *gd; /* global data */
//	asm volatile ("move %0, $26\n" : "=r" (gd));

    debug("%s\n",__func__);

    NvtNandInfo.flash_info = NULL;

    for(iFlashIndex = 0 ; iFlashIndex < NandFlashSupportList.num_flash ; iFlashIndex++)
    {
        NVT_nand_nfc_Init(&NandFlashSupportList.flash[iFlashIndex]);
        NVT_nand_nfc_ReadId(&NandFlashSupportList.flash[iFlashIndex], NULL, &uiFlashID, &uiExtFlashID);

        printk("ID0:0x%x   ID2:0x%x\r\n", uiFlashID, uiExtFlashID);
        printk("Index:0x%x   TableID2:0x%x timing:0x%x\r\n", iFlashIndex, NandFlashSupportList.flash[iFlashIndex].chip_id, NandFlashSupportList.flash[iFlashIndex].uiSimpleReadTiming);
		
//        gd->bd->bi_flashid     = uiFlashID;
//        gd->bd->bi_ext_flashid = uiExtFlashID;

        if((NandFlashSupportList.flash[iFlashIndex].chip_id == uiFlashID)  ||
		   (NandFlashSupportList.flash[iFlashIndex].chip_id == (uiFlashID & 0xFFFF)))
        {
            NvtNandInfo.flash_info = &(NandFlashSupportList.flash[iFlashIndex]);
            debug("\nFlash device found (FlasID:0x%x)\n", NvtNandInfo.flash_info->chip_id);
            break;
        }
    }

    if(NvtNandInfo.flash_info == NULL)
    {
        printf("Detect Flash failed");
        return -1;
    }

    nand->options       = 0;
    nand->waitfunc      = NVT_nand_wait_func;
    nand->select_chip   = NVT_nand_select_chip;
    nand->dev_ready     = NVT_nand_dev_ready;
    nand->cmdfunc       = NVT_nand_cmdfunc;
    nand->read_byte     = NVT_nand_read_byte;
    nand->read_word     = NVT_nand_read_word;
    nand->verify_buf    = NVT_nand_verify_buf;
    nand->write_buf     = NVT_nand_write_buf;
    nand->read_buf      = NVT_nand_read_buf;

    nand->ecc.mode = NAND_ECC_HW;
    nand->ecc.read_page     = NVT_nand_read_page_hwecc;
    nand->ecc.write_page    = NVT_nand_write_page_hwecc;
    nand->ecc.hwctl     = NVT_nand_ecc_hwctl;
    nand->ecc.calculate = NVT_nand_ecc_calculate;
    nand->ecc.correct   = NVT_nand_ecc_correct;
//    nand->ecc.size      = pNandInfo->page_size;
    nand->ecc.size      = NvtNandInfo.flash_info->subpage_size;
    nand->ecc.strength  = NvtNandInfo.flash_info->ucECCCorrectableBit;

	switch(NvtNandInfo.flash_info->ucTypicalECCType)
	{
		case NAND_ECC_RS_512_16B:
			if(NvtNandInfo.flash_info->page_size == 2048)
			{
			    nand->ecc.layout = &hw_2KB_page_ecclayout_RS;
    			nand->ecc.bytes = hw_2KB_page_ecclayout_RS.eccbytes;
			}
			if(NvtNandInfo.flash_info->page_size == 512)
			{
			    nand->ecc.layout = &hw_512B_page_ecclayout_RS;
    			nand->ecc.bytes = hw_512B_page_ecclayout_RS.eccbytes;
			}			
			break;
			
		case NAND_ECC_BCH24_1024_54B_HW:
			if(NvtNandInfo.flash_info->page_size == 2048)
			{
				nand->ecc.layout = &hw_4KB_page_ecclayout_BCH24_1024_54;
				nand->ecc.bytes = hw_4KB_page_ecclayout_BCH24_1024_54.eccbytes;
			}
			break;

		default:
			printk(KERN_ERR "unknown ECC type\r\n");
			return -1;
			break;
	}
	
//    nand->ecc.read_page = nt78320_nand_ReadPage;
//    nand->ecc.read_page_raw = nt78320_nand_ReadPage;
    nand->badblock_pattern = &delta_bbt_descr;

	nand->priv = &NvtNandInfo;
	NVT_nand_init_buff(&NvtNandInfo);

#if 0
    printf("Erase all block");
    for(uiBlockIndex = 0 ; uiBlockIndex < pNandInfo->num_blocks ; uiBlockIndex++)
    {
        uint32_t uiStatus;
        nt78320_nand_EraseBlock(NULL, uiBlockIndex * pNandInfo->page_per_block);
        nt78320_nand_ReadStatus(NULL, &uiStatus);
        printf("Erase:%d status:0x%x", uiBlockIndex, uiStatus);
    }
#endif


    return 0;
}

