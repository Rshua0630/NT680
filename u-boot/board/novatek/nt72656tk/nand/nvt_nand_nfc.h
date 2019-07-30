#ifndef __NVT_NFC_H_
#define __NVT_NFC_H_
#include "nvt_nand_flash_list.h"

#define NAND_ER                      unsigned int
#define NAND_ERROR_OK                0x00000000
#define NAND_ERROR_UNCORRECTABLE_ECC 0x00000001
#define NAND_ERROR_FLASH             0x00000002
#define NAND_ERROR_SYSTEM            0x00000010
#define NAND_ERROR_PARAMETER         0x00000020
#define NAND_ERROR_TIMEOUT           0x00000040

/* error code and state */
enum {
    ERR_NONE    = 0,
    ERR_DMABUSERR   = -1,
    ERR_SENDCMD = -2,
    ERR_DBERR   = -3,
    ERR_BBERR   = -4,
    ERR_ECC_FAIL    = -5,
    ERR_ECC_UNCLEAN = -6,
};

typedef enum
{
    NAND_ECC_TYPE_HAMMING = 0,
    NAND_ECC_TYPE_REEDSOLOMON,
    NAND_ECC_TYPE_BCH8,
    NAND_ECC_TYPE_BCH27,
    NAND_ECC_TYPE_BCH54,
    NAND_ECC_TYPE_NONE_16,
    NAND_ECC_TYPE_NONE_54
}ECC_TYPE;

typedef struct ECC_INFO
{
    unsigned short usErrorPos[24];
    unsigned short usSymbolCount;
    unsigned char  bUncorrectable;
}ECC_INFO, *PECC_INFO; //Sub page unit

#define NVT_nand_nfc_ReadOOB NVT_nand_nfc_ReadPage


int NVT_nand_nfc_Init(PNAND_FLASH_PARAMETER pFlashInfo);
int NVT_nand_nfc_ReadId(PNAND_FLASH_PARAMETER pFlashInfo, unsigned char* pucBuffer, unsigned int* id, unsigned int* extid);
int NVT_nand_nfc_ReadStatus(PNAND_FLASH_PARAMETER pFlashInfo, unsigned char* pucBuffer, unsigned int* status);
int NVT_nand_nfc_ReadPage(PNAND_FLASH_PARAMETER pFlashInfo, unsigned int buffer_phys, int iPageAddr, unsigned int* puiECCbitError);
int NVT_nand_nfc_EraseBlock(PNAND_FLASH_PARAMETER pFlashInfo, int iPageAddr);
int NVT_nand_nfc_WritePage(PNAND_FLASH_PARAMETER pFlashInfo, unsigned int buffer_phys, int iPageAddr);
void NVT_nand_nfc_WaitFlashReady(PNAND_FLASH_PARAMETER pFlashInfo);

#ifdef __NVT_NAND_ONBOOT__
int NVT_nand_nfc_InitFuncByBootstrap(PNAND_FLASH_PARAMETER pFlashInfo);
#endif

#endif
