/**
    NVT utilities for command customization

    @file       gxdsp.c
    @ingroup
    @note
    Copyright   Novatek Microelectronics Corp. 2016.  All rights reserved.

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License version 2 as
    published by the Free Software Foundation.
*/
#include <common.h>
#include <fs.h>
#include <asm/io.h>
#include <nvt_type.h>
#include "gxdsp.h"
#include <asm/arch/rcw_macro.h>
#include "drv/dma.h"
#include "dsp/dsp.h"
#include <asm/arch/IOAddress.h>
#include "nvtpack.h"
#include <modelext/modelext_parser.h>
#include <modelext/dram_partition_info.h>

#define CFG_GXDSP_INIT_KEY    MAKEFOURCC('_','D','S','P') ///< a key value '_','D','S','P' for indicating system initial.
#define GXDSP_CORE1_KEY    MAKEFOURCC('D','S','P','1')
#define GXDSP_CORE2_KEY    MAKEFOURCC('D','S','P','2')

#define PhyToCacheAddr(addr)          (addr)
#define dma_getPhyAddr(addr) 	((((uint32_t)(addr))>=0x60000000UL)?((uint32_t)(addr)-0x60000000UL):(uint32_t)(addr))

//#define REGVALUE                UINT32
//#define OUTW(addr,value)    (*(REGVALUE volatile *)(addr) = (REGVALUE)(value))
//#define INW(addr)           (*(REGVALUE volatile *)(addr))

#define CC_CONFIG_ID_SET_DSP2_DSP_BASE1_ADDRESS_OFS     0x184
#define CC_CONFIG_ID_SET_DSP2_DSP_BASE2_ADDRESS_OFS     0x188

//DSP HW max size
#define DSP_HW_DATAINT_MAX_SIZE    0x80000 //512KB
#define DSP_HW_PROGINT_MAX_SIZE    0x8000  //32KB

//max reserved size in DRAM
#define BLOADER_MAX_SIZE    0x1000  //4KB

#define BLOADER_TARGET_OFFSET (BLOADER_MAX_SIZE)//4KB

#define MAX_DSP_BIN_NUM 5

typedef struct _DSP_BIN_HEADER{
    UINT32  BinSize;
    UINT32  Reserve1;
    UINT32  Reserve2;
    UINT32  StartAddr;
}DSP_BIN_HEADER, *PDSP_BIN_HEADER;

typedef struct _GXDSP_UNIT{
    UINT32 uiAddr;
    UINT32 uiBinSize;  //bin file size
    UINT32 uiCodeSize; //code size
    UINT32 uiPoolSize; //memory pool size
}GXDSP_UNIT,*PGXDSP_UNIT;

typedef struct _GXDSP_CTRL{
    UINT32      uiInitKey;
    GXDSP_OPEN  Open;
    DSP_CORE_ID CoreID;
    UINT32      BLoaderAddr;
    UINT32      ProgIntAddr;
    UINT32      DataIntAddr;
}GXDSP_CTRL,*PGXDSP_CTRL;

static GXDSP_CTRL m_GxDSPCtrl = {0};

static UINT32 MemCheck_CalcCheckSum16Bit(UINT32 uiAddr,UINT32 uiLen)
{
	UINT32 i,uiSum = 0;
	UINT16 *puiValue = (UINT16 *)uiAddr;

	for (i=0; i<(uiLen >> 1); i++)
	{
	uiSum += (*(puiValue + i) + i);
	}

	uiSum &= 0x0000FFFF;

	return uiSum;
}

GXDSP_CTRL* xGxDSP_GetCtrl(void)
{
    return &m_GxDSPCtrl;
}

BOOL xGxDSP_AssignInfoWithChkValid(void)
{
    BOOL bAllBinValid = TRUE;
    GXDSP_CTRL *pCtrl = xGxDSP_GetCtrl();
    DSP_FW_HEADER *pDSPFWInfo;
    DSP_BIN_INFO *pDspBinInfo;
    UINT32 DspPoolEnd = pCtrl->Open.uiAddr + pCtrl->Open.uiSize;
    UINT32 i;

    pDSPFWInfo = (DSP_FW_HEADER *)(DspPoolEnd - sizeof(DSP_FW_HEADER));

    if(pDSPFWInfo->uiFourCC == GXDSP_CORE1_KEY)
    {
        pCtrl->CoreID = DSP_CORE_ID_1;
    }
    else if(pDSPFWInfo->uiFourCC == GXDSP_CORE2_KEY)
    {
        pCtrl->CoreID = DSP_CORE_ID_2;
    }
    else
    {
        printf("No DSP_FW_HEADER at the end of DSP pool\r\n");
        return FALSE;
    }

    printf("DSP Memory Pool: 0x%08X, size: 0x%08X\r\n",(u32)pCtrl->Open.uiAddr,(u32)pCtrl->Open.uiSize);
    printf("_ProgExt.bin @0x%08X, StartAddr=0x%08X size=0x%08X sum=0x%X\r\n",(u32)PhyToCacheAddr(pDSPFWInfo->ProgExt.TargetAddr),(u32)pDSPFWInfo->ProgExt.TargetAddr,(u32)pDSPFWInfo->ProgExt.Size,(u32)MemCheck_CalcCheckSum16Bit(pDSPFWInfo->ProgExt.TargetAddr,pDSPFWInfo->ProgExt.Size));
    printf("_DataExt.bin @0x%08X, StartAddr=0x%08X size=0x%08X sum=0x%X\r\n",(u32)PhyToCacheAddr(pDSPFWInfo->DataExt.TargetAddr),(u32)pDSPFWInfo->DataExt.TargetAddr,(u32)pDSPFWInfo->DataExt.Size,(u32)MemCheck_CalcCheckSum16Bit(pDSPFWInfo->DataExt.TargetAddr,pDSPFWInfo->DataExt.Size));
    printf("_DataInt.bin @0x%08X, StartAddr=0x%08X size=0x%08X sum=0x%X\r\n",(u32)PhyToCacheAddr(pDSPFWInfo->DataInt.TargetAddr),0,(u32)pDSPFWInfo->DataInt.Size,(u32)MemCheck_CalcCheckSum16Bit(pDSPFWInfo->DataInt.TargetAddr,pDSPFWInfo->DataInt.Size));
    printf("_ProgInt.bin @0x%08X, StartAddr=0x%08X size=0x%08X sum=0x%X\r\n",(u32)PhyToCacheAddr(pDSPFWInfo->ProgInt.TargetAddr),0,(u32)pDSPFWInfo->ProgInt.Size,(u32)MemCheck_CalcCheckSum16Bit(pDSPFWInfo->ProgInt.TargetAddr,pDSPFWInfo->ProgInt.Size));
    printf("_BLoader.bin @0x%08X, StartAddr=0x%08X size=0x%08X sum=0x%X\r\n",(u32)PhyToCacheAddr(pDSPFWInfo->BLoader.TargetAddr),(u32)pDSPFWInfo->BLoader.TargetAddr,(u32)pDSPFWInfo->BLoader.Size,(u32)MemCheck_CalcCheckSum16Bit(pDSPFWInfo->BLoader.TargetAddr,pDSPFWInfo->BLoader.Size));

    //verify the checksum of every bin file
    //check sum
    pDspBinInfo = (DSP_BIN_INFO *)&(pDSPFWInfo->BLoader.Offset);
    for(i=0; i<MAX_DSP_BIN_NUM; i++)
    {
        if((((pDspBinInfo+i)->CheckSum+MemCheck_CalcCheckSum16Bit(PhyToCacheAddr((pDspBinInfo+i)->TargetAddr),(pDspBinInfo+i)->Size))& 0x0000FFFF)!=0)
        {
            printf("bin %d check sum fail.\r\n",(u32)i);
            bAllBinValid = FALSE;
            break;
        }
    }

    //asign info
    if(bAllBinValid)
    {
        pCtrl->BLoaderAddr = pDSPFWInfo->BLoader.TargetAddr;
        pCtrl->ProgIntAddr = pDSPFWInfo->ProgInt.TargetAddr;
        pCtrl->DataIntAddr = pDSPFWInfo->DataInt.TargetAddr;
    }

    return bAllBinValid;
}

BOOL GxDSP_ChkFwHeader(DSP_CORE_ID DspCore, DSP_FW_HEADER* pHeader)
{
    MODELEXT_HEADER *header;
	DRAM_PARTITION *p_dram_partition = (DRAM_PARTITION *)modelext_get_cfg((unsigned char*)CONFIG_MODELEXT_SDRAM_BASE, MODELEXT_TYPE_DRAM_PARTITION, &header);
	UINT32 DspPoolAddr, DspPoolSize;
	if(!p_dram_partition || !header)
	{
	    printf("no dram partition info!\r\n");
		return FALSE;
	}
	if(DSP_CORE_ID_1 == DspCore)
	{
	    DspPoolAddr = p_dram_partition->dsp1_addr;
	    DspPoolSize = p_dram_partition->dsp1_size;
	    if( pHeader->uiFourCC != GXDSP_CORE1_KEY)
        {
            printf("Invalid DSP FW(FourCC=0x%X).\r\n" ,(u32)pHeader->uiFourCC);
            return FALSE;
        }
	}
	else
	{
	    DspPoolAddr = p_dram_partition->dsp2_addr;
	    DspPoolSize = p_dram_partition->dsp2_size;
	    if( pHeader->uiFourCC != GXDSP_CORE2_KEY)
        {
            printf("Invalid DSP FW(FourCC=0x%X).\r\n" ,(u32)pHeader->uiFourCC);
            return FALSE;
        }
	}
    UINT32 DspPoolEnd = DspPoolAddr + DspPoolSize;

    //check if FW Header overlap BLoader
    if(((UINT32)pHeader >= pHeader->BLoader.TargetAddr) && ((UINT32)pHeader < pHeader->BLoader.TargetAddr+pHeader->BLoader.Size))
    {
        printf("FW Header overlap BLoader @0x%X.\r\n" ,(u32)pHeader);
        return FALSE;
    }

    //check DSP FW Header
    if(pHeader->uiSize != sizeof(DSP_FW_HEADER))
    {
        printf("Invalid DSP FW header @0x%X(size=%d).\r\n" ,(u32)pHeader ,(u32)pHeader->uiSize);
        return FALSE;
    }
//ben to do
#if 0
	//check DSP BLoader
    if( pHeader->BLoader.TargetAddr != dma_getPhyAddr(DspPoolEnd - BLOADER_TARGET_OFFSET) || pHeader->BLoader.Size > (BLOADER_MAX_SIZE-sizeof(DSP_FW_HEADER)))
    {
        printf("DSP loader TargetAddr=0x%X,Size=%d  NOT match DRAM partition(0x%X ~0x%X)\r\n"
                                           ,pHeader->BLoader.TargetAddr
                                           ,pHeader->BLoader.Size
                                           ,DspPoolAddr
                                           ,DspPoolAddr+DspPoolSize);
    }
#else
    //check DSP BLoader
    if( pHeader->BLoader.TargetAddr != dma_getPhyAddr(DspPoolEnd - BLOADER_TARGET_OFFSET) || pHeader->BLoader.Size > (BLOADER_MAX_SIZE-sizeof(DSP_FW_HEADER)))
    {
        printf("Invalid DSP boot loader. (TargetAddr=0x%X,Size=%d)\r\n"
                                           ,(u32)pHeader->BLoader.TargetAddr
                                           ,(u32)pHeader->BLoader.Size);
//ben to do        return FALSE;
    }
     //check DSP ProgInt
    if( pHeader->ProgInt.TargetAddr + pHeader->ProgInt.Size > pHeader->BLoader.TargetAddr)
    {
        printf("Invalid DSP ProgInt. (TargetAddr=0x%X,Size=%d)\r\n"
                                           ,(u32)pHeader->ProgInt.TargetAddr
                                           ,(u32)pHeader->ProgInt.Size);
//ben to do        return FALSE;
    }
    //check DSP DataInt
    if( pHeader->DataInt.TargetAddr + pHeader->DataInt.Size > pHeader->BLoader.TargetAddr)
    {
        printf("Invalid DSP DataInt. (TargetAddr=0x%X,Size=%d)\r\n"
                                           ,(u32)pHeader->DataInt.TargetAddr
                                           ,(u32)pHeader->DataInt.Size);
//ben to do        return FALSE;
    }

    if( pHeader->ProgExt.TargetAddr < dma_getPhyAddr(DspPoolAddr) || pHeader->DataExt.TargetAddr < dma_getPhyAddr(DspPoolAddr) ||
        pHeader->ProgExt.TargetAddr + pHeader->ProgExt.Size > dma_getPhyAddr(DspPoolEnd) ||
        pHeader->DataExt.TargetAddr+pHeader->DataExt.Size > dma_getPhyAddr(DspPoolEnd))
    {
        printf("Invalid DSP Ext. (ProgExt TargetAddr=0x%X Size=%d, DataExt TargetAddr=0x%X Size=%d, DspPool=0x%X %d\r\n"
                                           ,(u32)pHeader->ProgExt.TargetAddr
                                           ,(u32)pHeader->ProgExt.Size
                                           ,(u32)pHeader->DataExt.TargetAddr
                                           ,(u32)pHeader->DataExt.Size
                                           ,(u32)DspPoolAddr
                                           ,(u32)DspPoolSize);
//ben to do        return FALSE;
    }
#endif
    return TRUE;

}

BOOL GxDSP_Open(GXDSP_OPEN* pOpen)
{
    GXDSP_CTRL *pCtrl = xGxDSP_GetCtrl();
	UINT32 uiBootloaderResetAddress;
	DSP_ID dsp_id;
return TRUE;
    if(pCtrl->uiInitKey != CFG_GXDSP_INIT_KEY)
    {
        printf("GxDSP_Init must be called first.\r\n");
        return FALSE;
    }

    pCtrl->Open = *pOpen;

    if(!xGxDSP_AssignInfoWithChkValid())
    {
        return FALSE;
    }

    if(pCtrl->CoreID == DSP_CORE_ID_1)
        dsp_id = DSP_ID_1;
    else
        dsp_id = DSP_ID_2;

	uiBootloaderResetAddress = pCtrl->BLoaderAddr;
    //Enable DSP CG's setting
    dsp_open(dsp_id);
    //Set DSP reset mode to Boot mode
    dsp_setConfig(dsp_id, DSP_CONFIG_ID_BOOTMODE, DSP_BOOTMODE_BOOT_SEQ);
    //Set DSP boot address
    uiBootloaderResetAddress &=  ~(0x60000000);
    dsp_setConfig(dsp_id, DSP_CONFIG_ID_BOOTVECTOR, uiBootloaderResetAddress);

	//set internal program bin address
	//cc_setConfig(CC_CONFIG_ID_SET_DSP2_DSP_BASE1_ADDRESS, pCtrl->ProgIntAddr);
	OUTW(IOADDR_CC_REG_BASE + CC_CONFIG_ID_SET_DSP2_DSP_BASE1_ADDRESS_OFS, dma_getPhyAddr(pCtrl->ProgIntAddr));
	//set internal data bin address
	//cc_setConfig(CC_CONFIG_ID_SET_DSP2_DSP_BASE2_ADDRESS, pCtrl->DataIntAddr);
	OUTW(IOADDR_CC_REG_BASE + CC_CONFIG_ID_SET_DSP2_DSP_BASE2_ADDRESS_OFS, dma_getPhyAddr(pCtrl->DataIntAddr));

	//enable outstanding
    // dram controller enable outstanding
	dma_setAXIRequestCount(DMA_AXI_CH2, 4);
	dma_setAXIRequestCount(DMA_AXI_CH3, 4);
	dma_setAXIRequestCount(DMA_AXI_CH4, 4);
	dma_setAXIRequestCount(DMA_AXI_CH5, 4);
	dsp_setConfig(dsp_id, DSP_CONFIG_ID_EPP_POSTD, 1);
	dsp_setConfig(dsp_id, DSP_CONFIG_ID_EDP_POSTD, 1);

    //Set DSP debug mode
    dsp_setConfig(dsp_id, DSP_CONFIG_ID_DEBUGPORTSEL, 0x1);
    dsp_setConfig(dsp_id, DSP_CONFIG_ID_DEBUGPORTEN, TRUE);
    //Set DSP reset release
    dsp_setConfig(dsp_id, DSP_CONFIG_ID_DERESET, TRUE);
    return TRUE;
}
