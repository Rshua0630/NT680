#ifndef __NAND_FLASH_LIST_
#define __NAND_FLASH_LIST_

#ifdef __NVT_NAND_ONBOOT__
#include "nvt_nand_656onboot.h"
#endif

#ifdef __NVT_NAND_UBOOT__
#include "linux/types.h"
#include "nvt_nand_nfc_uboot.h"
#endif

#ifdef __NVT_NAND_KERNEL__
#include "linux/types.h"
#endif

typedef struct NAND_FLASH_TIMING {
    unsigned int    tCH;  /* Enable signal hold time */
    unsigned int    tCS;  /* Enable signal setup time */
    unsigned int    tWH;  /* ND_nWE high duration */
    unsigned int    tWP;  /* ND_nWE pulse time */
    unsigned int    tRH;  /* ND_nRE high duration */
    unsigned int    tRP;  /* ND_nRE pulse width */
    unsigned int    tR;   /* ND_nWE high to ND_nRE low for read */
    unsigned int    tWHR; /* ND_nWE high to ND_nRE low for status read */
    unsigned int    tAR;  /* ND_ALE low to ND_nRE low delay */
}NAND_FLASH_TIMING, *PNAND_FLASH_TIMING;

typedef struct NAND_FLASH_CMD_SET {
    uint16_t    read1;
    uint16_t    read2;
    uint16_t    program;
    uint16_t    read_status;
    uint16_t    read_id;
    uint16_t    erase;
    uint16_t    reset;
    uint16_t    lock;
    uint16_t    unlock;
    uint16_t    lock_status;
}NAND_FLASH_CMD_SET, *PNAND_FLASH_CMD_SET;


typedef struct NAND_FLASH_PARAMETER {
    NAND_FLASH_TIMING *timing; /* NAND Flash timing */
    NAND_FLASH_CMD_SET *cmdset;
	void __iomem* pControllerBaseAddr;
    uint32_t page_per_block;/* Pages per block (PG_PER_BLK) */
    uint32_t page_size; /* Page size in bytes (PAGE_SZ) */
    uint32_t oob_size;  /* oob size in a page */
    uint32_t subpage_size;  /* Subpage size in bytes */
    uint32_t subpageoob_size;  /* oob size in a sub page (PAGE_SZ) */
    uint32_t flash_width;   /* Width of Flash memory */
    uint32_t nfc_width; /* Width of Flash controller  */
    uint32_t num_blocks;    /* Number of physical blocks in Flash */
    uint32_t chip_id;
    uint32_t uiNumOfRow;
    uint32_t uiNumOfCol;
    uint32_t uiSimpleReadTiming;
    uint32_t uiSimpleProgramTiming;
    uint32_t uiSimpleFineTuneTiming;
    uint32_t uiInterruptID;
	unsigned char ucTypicalECCType;
	unsigned char ucECCCorrectableBit;
	
}NAND_FLASH_PARAMETER, *PNAND_FLASH_PARAMETER;

typedef struct NAND_FLASH_SUPPORT_LIST {

    /* the data flash bus is shared between the Static Memory
     * Controller and the Data Flash Controller,  the arbiter
     * controls the ownership of the bus
     */
    PNAND_FLASH_PARAMETER    flash;
    unsigned char            num_flash;
}NAND_FLASH_SUPPORT_LIST, *PNAND_FLASH_SUPPORT_LIST;

#endif
