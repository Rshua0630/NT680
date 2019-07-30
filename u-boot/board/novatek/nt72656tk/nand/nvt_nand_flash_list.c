#include "nvt_nand_flash_list.h"
#include "nvt_nand_ecc_layout.h"

static struct NAND_FLASH_TIMING samsungK9F2G08X0A_timing = {
        .tCH    = 10,
        .tCS    = 0,
        .tWH    = 20,
        .tWP    = 40,
        .tRH    = 30,
        .tRP    = 40,
        .tR     = 11123,
        .tWHR   = 110,
        .tAR    = 10,
};

static struct NAND_FLASH_TIMING stNAND02GW3B2D_timing = {
        .tCH    = 10,
        .tCS    = 0,
        .tWH    = 20,
        .tWP    = 40,
        .tRH    = 30,
        .tRP    = 40,
        .tR     = 11123,
        .tWHR   = 110,
        .tAR    = 10,
};

static struct NAND_FLASH_TIMING micronMT29F2G08_timing = {
        .tCH    = 10,
        .tCS    = 0,
        .tWH    = 20,
        .tWP    = 40,
        .tRH    = 30,
        .tRP    = 40,
        .tR     = 11123,
        .tWHR   = 110,
        .tAR    = 10,
};


static struct NAND_FLASH_TIMING spansionS34ML02G1_timing = {
        .tCH    = 10,
        .tCS    = 0,
        .tWH    = 20,
        .tWP    = 40,
        .tRH    = 30,
        .tRP    = 40,
        .tR     = 11123,
        .tWHR   = 110,
        .tAR    = 10,
};

static struct NAND_FLASH_CMD_SET largepage_cmdset = {
        .read1          = 0x3000,
        .read2          = 0x0050,
        .program        = 0x1080,
        .read_status    = 0x0070,
        .read_id        = 0x0090,
        .erase          = 0xD060,
        .reset          = 0x00FF,
        .lock           = 0x002A,
        .unlock         = 0x2423,
        .lock_status    = 0x007A,
};



NAND_FLASH_PARAMETER mach_support_flash_types[] = {
                                                    {&samsungK9F2G08X0A_timing, &largepage_cmdset, (void*)0, 64, 2048, 64, 512, 16, 8, 8, 2048, 0x9510daec, 3, 2, 0x82103020, 0x10103020, 0xC0C0, 95, NAND_ECC_RS_512_16B, 4},  /* K9F2G08U0A */
													{&samsungK9F2G08X0A_timing, &largepage_cmdset, (void*)0, 64, 2048, 64, 512, 16, 8, 8, 2048, 0xf1ec    , 2, 2, 0x82103020, 0x10103020, 0xC0C0, 95, NAND_ECC_RS_512_16B, 4},  /* K9F1G08U0A */
													{&stNAND02GW3B2D_timing   , &largepage_cmdset, (void*)0, 64, 2048, 64, 512, 16, 8, 8, 2048, 0xda20	, 3, 2, 0x82223020, 0x22223020, 0, 95, NAND_ECC_RS_512_16B, 4},  /* ST NAND02GW3B2D */
													{&micronMT29F2G08_timing  , &largepage_cmdset, (void*)0, 64, 2048, 64, 512, 16, 8, 8, 2048, 0xda2c	, 3, 2, 0x82223020, 0x22223020, 0, 95, NAND_ECC_RS_512_16B, 4},  /* MICRON MT29F2G08 */
													{&spansionS34ML02G1_timing, &largepage_cmdset, (void*)0, 64, 2048, 64, 512, 16, 8, 8, 2048, 0xda01	, 3, 2, 0x82223020, 0x22223020, 0, 95, NAND_ECC_RS_512_16B, 4},  /* SPANSION 34ML02G1 */
													{&spansionS34ML02G1_timing, &largepage_cmdset, (void*)0, 64, 2048, 64, 512, 16, 8, 8, 4096, 0xdc01	, 3, 2, 0x82223020, 0x22223020, 0, 95, NAND_ECC_RS_512_16B, 4}  /* SPANSION 34ML04G1 */
                                                  };


NAND_FLASH_SUPPORT_LIST NandFlashSupportList = {
        .flash      = mach_support_flash_types,
        .num_flash  = 6,
};

