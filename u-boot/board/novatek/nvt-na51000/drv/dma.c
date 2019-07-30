/*
    DMA module driver

    This file is the driver of DMA module.

    @file       DMA.c
    @ingroup    mIDrvDMA_DMA
    @note       Nothing.

    Copyright   Novatek Microelectronics Corp. 2014.  All rights reserved.
*/

//#include "nvtDrvProtected.h"
#include <common.h>
#include <asm/arch/rcw_macro.h>
#include <asm/arch/nvt_ipc_common.h>
#include <asm/arch/nvt_common.h>
#include "dma_reg.h"
//#include "cache.h"
//#include "Utility.h"
//#include "ist.h"
#include "dma.h"
#include "pll.h"
#include "dma_int.h"

#define HEAVY_LOAD_CTRL_OFS(ch)         (DMA_CHANNEL0_HEAVY_LOAD_CTRL_OFS + ((ch) * 0x10))
#define HEAVY_LOAD_ADDR_OFS(ch)         (DMA_CHANNEL0_HEAVY_LOAD_START_ADDR_OFS + ((ch) * 0x10))
#define HEAVY_LOAD_SIZE_OFS(ch)         (DMA_CHANNEL0_HEAVY_LOAD_DMA_SIZE_OFS + ((ch) * 0x10))
#define HEAVY_LOAD_WAIT_CYCLE_OFS(ch)   (DMA_CHANNEL0_HEAVY_LOAD_WAIT_CYCLE_OFS + ((ch) * 0x10))

#define PROTECT_START_ADDR_OFS(ch)      (DMA_PROTECT_STARTADDR0_REG_OFS+(ch)*8)
#define PROTECT_END_ADDR_OFS(ch)        (DMA_PROTECT_STOPADDR0_REG_OFS+(ch)*8)
#define PROTECT_CH_MSK0_OFS(ch)         (DMA_PROTECT_RANGE0_MSK0_REG_OFS+(ch)*32)
#define PROTECT_CH_MSK1_OFS(ch)         (DMA_PROTECT_RANGE0_MSK1_REG_OFS+(ch)*32)
#define PROTECT_CH_MSK2_OFS(ch)         (DMA_PROTECT_RANGE0_MSK2_REG_OFS+(ch)*32)
#define PROTECT_CH_MSK3_OFS(ch)         (DMA_PROTECT_RANGE0_MSK3_REG_OFS+(ch)*32)
#define PROTECT_CH_MSK4_OFS(ch)         (DMA_PROTECT_RANGE0_MSK4_REG_OFS+(ch)*32)
#define PROTECT_CH_MSK5_OFS(ch)         (DMA_PROTECT_RANGE0_MSK5_REG_OFS+(ch)*32)
#define PROTECT_CH_MSK6_OFS(ch)         (DMA_PROTECT_RANGE0_MSK6_REG_OFS+(ch)*32)
#define PROTECT_CH_MSK7_OFS(ch)         (DMA_PROTECT_RANGE0_MSK7_REG_OFS+(ch)*32)

#define REQUEST_FLAG_OFS(ch)            (DMA_REQ_FLG0_REG_OFS+(ch)*4)
#define REQUEST_ACK_OFS(ch)             (DMA_ACK_FLG0_REG_OFS+(ch)*4)

#define DMA_PRIOFITY_OFS(ch)            (DMA_PRI_CONTROL0_REG_OFS+(ch)*4)

#define DMA_MONITOR_CONFIG_OFS(ch)      (DMA_RECORD_CONFIG0_REG_OFS+(ch)*2)
#if 0
extern void dma_isr(void);
extern void dma2_isr(void);

//UINT32 uiDmaCtrlBaseAddr         = IOADDR_DRAM_REG_BASE;    // Refered by Startup_MIPS.s

//#if (DRV_SUPPORT_IST == ENABLE)

//#else
static DRV_CB pDmaWPCBFunc[DMA_WPSET_TOTAL] = {NULL,NULL,NULL};
//#endif

//static UINT32 gDmaProtectChSts_LW[DMA_WPSET_TOTAL] ={0,0,0}, gDmaProtectChSts_HW[DMA_WPSET_TOTAL]={0,0,0};
static DMA_WP_STS_TYPE gDmaProtectChSts[DMA_WPSET_TOTAL] =
{
    {0,0,0,0},
    {0,0,0,0},
    {0,0,0,0},
};

static DMA_WRITEPROT_ATTR gDmaProtectAttr[DMA_WPSET_TOTAL]; // DMA_ID_1, DMA_ID_2 share the same protect setting
const static CHAR * DmaWPEngineName[] = {
    "CPU",
    "CPU2",
    "DSP_0",
    "DSP_1",
    "DSP2_0",
    "DSP2_1",
    "USB30",
    "Ethernet",
    "IDE_a0",
    "IDE_b0",
    "IDE_a1",
    "IDE_b1",
    "IDE_4",
    "IDE_5",
    "IDE2_a0",
    // Ctrl 0

    "IDE2_b0",
    "IDE2_6",
    "IDE2_7",
    "TS Mux",
    "Crypto",
    "LARB: SIF/I2C/UART",
    "NAND",
    "SDIO",
    "SDIO2",
    "SDIO3",
    "SPI",
    "DAI",
    "USB",
    "HWCopy_0",
    "HWCopy_1",
    "Rotate_0",
    //Ctrl 1

    "Rotate_1",
    "TSDemux_0",
    "TSDemux_1",
    "ISE2_0",
    "ISE2_1",
    "DIS_0",
    "DIS_1",
    "FDE_0",
    "FDE_1",
    "FDE_2",
    "ISE_0",
    "ISE_1",
    "SIE_0",
    "SIE_1",
    "SIE_2",
    "SIE_3",
    // Ctrl 2

    "SIE_4",
    "SIE_5",
    "SIE_6",
    "SIE_7",
    "SIE_8",
    "SIE2_0",
    "SIE2_1",
    "SIE2_2",
    "SIE2_3",
    "SIE2_4",
    "SIE2_5",
    "SIE2_6",
    "SIE3_0",
    "SIE3_1",
    "SIE3_2",
    "SIE3_3",
    // Ctrl 3

    "SIE3_4",
    "SIE3_5",
    "SIE3_6",
    "SIE4_0",
    "SIE4_1",
    "SIE4_2",
    "SIE4_3",
    "SIE4_4",
    "SIE4_5",
    "SIE4_6",
    "SIE5_0",
    "SIE5_1",
    "SIE5_2",
    "SIE6_0",
    "SIE6_1",
    "SIE6_2",
    // Ctrl 4

    "SIE7_0",
    "SIE7_1",
    "SIE7_2",
    "SIE8_0",
    "SIE8_1",
    "SIE8_2",
    "GRA_0",
    "GRA_1",
    "GRA_2",
    "GRA_3",
    "GRA_4",
    "GRA2_0",
    "GRA2_1",
    "GRA2_2",
    "Affine_0",
    "Affine_1",
    // Ctrl 5

    "JPG0",            ///< JPG IMG
    "JPG1",            ///< JPG BS
    "JPG2",            ///< JPG Encode mode DC output
    "DCE_0",
    "DCE_1",
    "DCE_2",
    "DCE_3",
    "DCE_4",
    "DRE_0",
    "DRE_1",
    "DRE_2",
    "DRE_3",
    "DRE_4",
    "DRE_5",
    "DRE_6",
    "DRE_7",
    // Ctrl 6

    "IFE_0",
    "IFE_1",
    "RHE_0",
    "RHE_1",
    "RHE_2",
    "RHE_3",
    "RHE_4",
    "RHE_5",
    "RHE_6",
    "RHE_7",
    "H264_0",
    "H264_1",
    "H264_2",
    "H264_3",
    "H264_4",
    "H264_5",
    // Ctrl 7

    "H264_6",
    "H264_7",
    "H264_8",
    "COE_0",
    "COE_1",
    "IPE_0",
    "IPE_1",
    "IPE_2",
    "IPE_3",
    "IME_0",
    "IME_1",
    "IME_2",
    "IME_3",
    "IME_4",
    "IME_5",
    "IME_6",
    // Ctrl 8

    "IME_7",
    "IME_8",
    "IME_9",
    "IME_A",
    "IME_B",
    "IME_C",
    "IME_D",
    "IME_E",
    "IME_F",
    "IME_10h",
    "IME_11h",
    "IME_12h",
    "IME_13h",
    "IME_14h",
    "IME_15h",
    "IME_16h",
    // Ctrl 9

    "IME_17h",
    "IFE2_0",
    "IFE2_1",
    "SVM_0",
    "SVM_1",
    "SDE_0",
    "SDE_1",
    "SDE_2",
    "SDE_3",
    "IVE_0",
    "IVE_1",
    "CNN_0",
    "CNN_1",

};

static DMA_MONITOR_STRUCT vDmaMonitorObj[DMA_ID_COUNT][DMA_MONITOR_CH_COUNT];
static UINT32 vDmaMonitorPeriod[DMA_ID_COUNT] = {100, 100}; // default period is 100ms
static DMA_GENERIC_CB vDmaMonitorCB[DMA_ID_COUNT] = {NULL, NULL};

static BOOL bOpened = FALSE;



/**
    @addtogroup mIDrvDMA_DMA
*/
//@{

/*
    Get DMA controller register.

    @param[in] id           graphic controller ID
    @param[in] offset       register offset in DMA controller (word alignment)

    @return register value
*/
static REGVALUE dma_getReg(DMA_ID id, UINT32 offset)
{
    if (id == DMA_ID_1)
    {
        return DMA_GETREG(offset);
    }
    else
    {
        return DMA2_GETREG(offset);
    }
}

/*
    Set DMA controller register.

    @param[in] id           graphic controller ID
    @param[in] offset       register offset in DMA controller (word alignment)
    @param[in] value        register value

    @return void
*/
static void dma_setReg(DMA_ID id, UINT32 offset, REGVALUE value)
{
    if (id == DMA_ID_1)
    {
        DMA_SETREG(offset, value);
    }
    else
    {
        DMA2_SETREG(offset, value);
    }
}

/*
    Write protect function default channel display

    @param[in] WpSet        Write protect function set
    @param[in] uiChannel    Write protect function detect channel

    @return
        void
*/
static void DmaDefaultWriteProtect_CB(DMA_WRITEPROT_SET WpSet, UINT32 uiChannel)
{
    DBG_DUMP("Set[%d] addr(0x%08x)~(0x%08x)\r\n", WpSet,
                                gDmaProtectAttr[WpSet].uiStartingAddr,
                                gDmaProtectAttr[WpSet].uiStartingAddr + gDmaProtectAttr[WpSet].uiSize);
    DBG_DUMP("Overwrite by %s(%d)\r\n",
                                DmaWPEngineName[uiChannel-DMA_WPCH_FIRST_CHANNEL],
                                uiChannel);
    dma_clrWPStatus(WpSet, (DMA_WRITEPROT_CH)uiChannel);
}


/*
    Default API to show write protect channel detected

    Call dma_getWPStatus and send status into dma_showWPChannel, after this
    function called, write protect status will be cleared.

    @param[in] WpSet  Write protect function set
    @param[in] WpSts  Write protect function status

    @return void
*/
void dma_showWPChannel(DMA_WRITEPROT_SET WpSet, DMA_WP_STS_TYPE WpSts)
{
    UINT32 index;
    UINT32 uiChkWpSts;

    uiChkWpSts = WpSts.uiChannelGroup[DMA_CH_GROUP0];
    for (index=0; index<DMA_WPCH_COUNT; )
    {
        UINT32 i;

        if (uiChkWpSts)
        {
            i = __builtin_ctz(uiChkWpSts);
            DBG_DUMP("Mem Overwrite by %s\r\n", DmaWPEngineName[index+i-DMA_WPCH_FIRST_CHANNEL]);
            dma_clrWPStatus(WpSet, (DMA_WRITEPROT_CH)(index+i));
            uiChkWpSts &= ~(1<<i);
        }

        if (uiChkWpSts == 0)
        {
            index += 32;
            if (index < DMA_WPCH_COUNT)
            {
                uiChkWpSts = WpSts.uiChannelGroup[index/32];
            }
            else
            {
                uiChkWpSts = 0;
            }
        }
    }
}


/*
  DRAM control ISR

  DRAM control Interrupt Service Routine

  @return void
*/
void dma_isr(void)
{
    T_DMA_PROTECT_INTSTS_REG ProtectIntStsReg;
    T_DMA_PROTECT_CHSTS_REG ProtectChStsReg;
    T_DMA_OUT_RANGE_CHANNEL_REG outRangeChReg;
    T_DMA_OUT_RANGE_ADDR_REG outRangeAddrReg;
#if (DRV_SUPPORT_IST == ENABLE)
    UINT32  uiEvent;
#endif

    outRangeChReg.Reg = DMA_GETREG(DMA_OUT_RANGE_CHANNEL_REG_OFS);
    outRangeAddrReg.Reg = DMA_GETREG(DMA_OUT_RANGE_ADDR_REG_OFS);

    ProtectIntStsReg.Reg = DMA_GETREG(DMA_PROTECT_INTSTS_REG_OFS);
    DMA_SETREG(DMA_PROTECT_INTSTS_REG_OFS, ProtectIntStsReg.Reg);

    ProtectIntStsReg.Reg &= DMA_GETREG(DMA_PROTECT_INTCTRL_REG_OFS);
    ProtectChStsReg.Reg = DMA_GETREG(DMA_PROTECT_CHSTS_REG_OFS);

    if(ProtectIntStsReg.Reg == 0)
        return;

#if (DRV_SUPPORT_IST == ENABLE)
    uiEvent = 0;
#endif

    if (ProtectIntStsReg.Bit.AUTO_REFRESH_TIMEOUT_STS)
    {
        DBG_FATAL("Auto Refresh Timeout\r\n");
    }
    if (ProtectIntStsReg.Bit.DMA_OUTRANGE_STS)
    {
//        T_DMA_OUT_RANGE_CHANNEL_REG outRangeChReg;
//        T_DMA_OUT_RANGE_ADDR_REG outRangeAddrReg;

//        outRangeChReg.Reg = DMA_GETREG(DMA_OUT_RANGE_CHANNEL_REG_OFS);
//        outRangeAddrReg.Reg = DMA_GETREG(DMA_OUT_RANGE_ADDR_REG_OFS);

        if ((outRangeChReg.Bit.DRAM_OUTRANGE_CH == 0) ||
            (outRangeChReg.Bit.DRAM_OUTRANGE_CH >= DMA_WPCH_COUNT))
        {
            DBG_ERR("Out range addr 0x%lx\r\n", outRangeAddrReg.Bit.DRAM_OUTRANGE_ADDR);
        }
        else
        {
            DBG_ERR("Out range channel %s, addr 0x%lx\r\n", DmaWPEngineName[outRangeChReg.Bit.DRAM_OUTRANGE_CH-DMA_WPCH_FIRST_CHANNEL],
                                                    outRangeAddrReg.Bit.DRAM_OUTRANGE_ADDR);
        }
//        DBG_ERR("Ch coding 0x%lx\r\n", outRangeChReg.Bit.DRAM_OUTRANGE_CH);
//        DBG_ERR("int sts 0x%lx\r\n", ProtectIntStsReg.Reg);
    }
    if (ProtectIntStsReg.Bit.DMA_UPDATE_USAGE_STS)
    {
        DMA_MONITOR_CH ch;
        T_DMA_RECORD0_SIZE_REG sizeReg;
        T_DMA_RECORD0_COUNT_REG countReg;

        for (ch=DMA_MONITOR_CH0; ch<DMA_MONITOR_ALL; ch++)
        {
            sizeReg.Reg = DMA_GETREG(DMA_RECORD0_SIZE_REG_OFS + ch*8);
            countReg.Reg = DMA_GETREG(DMA_RECORD0_COUNT_REG_OFS + ch*8);

            if (sizeReg.Bit.MONITOR_SIZE_OVF)
                vDmaMonitorObj[DMA_ID_1][ch].uiSizeLog = 0xFFFFFFF;
            else
                vDmaMonitorObj[DMA_ID_1][ch].uiSizeLog = sizeReg.Bit.MONITOR_ACC_SIZE;

            if (countReg.Bit.MONITOR_REQ_OVF)
                vDmaMonitorObj[DMA_ID_1][ch].uiReqLog = 0xFFFFFFF;
            else
                vDmaMonitorObj[DMA_ID_1][ch].uiReqLog = countReg.Bit.MONITOR_ACC_REQ;
        }

        sizeReg.Reg = DMA_GETREG(DMA_BANDWIDTH_SIZE_REG_OFS);
        countReg.Reg = DMA_GETREG(DMA_BANDWIDTH_REQ_REG_OFS);

        if (sizeReg.Bit.MONITOR_SIZE_OVF)
            vDmaMonitorObj[DMA_ID_1][DMA_MONITOR_ALL].uiSizeLog = 0xFFFFFFF;
        else
            vDmaMonitorObj[DMA_ID_1][DMA_MONITOR_ALL].uiSizeLog = sizeReg.Bit.MONITOR_ACC_SIZE;

        if (countReg.Bit.MONITOR_REQ_OVF)
            vDmaMonitorObj[DMA_ID_1][DMA_MONITOR_ALL].uiReqLog = 0xFFFFFFF;
        else
            vDmaMonitorObj[DMA_ID_1][DMA_MONITOR_ALL].uiReqLog = countReg.Bit.MONITOR_ACC_REQ;

        if (vDmaMonitorCB[DMA_ID_1] != NULL)
        {
#if (DRV_SUPPORT_IST == ENABLE)
            uiEvent |= DMA_IST_EVENT_MONITOR;
#else
            vDmaMonitorCB[DMA_ID_1](0);
#endif
        }
//        dma_updateMonitorQueue(DMA_MONITOR_CH0);
    }
    if (ProtectIntStsReg.Bit.DMA_WPWD0_STS)
    {
        UINT32 uiWordOffset = ProtectChStsReg.Bit.CHSTS0 / 32;
        UINT32 uiBitOffset = ProtectChStsReg.Bit.CHSTS0 % 32;

        gDmaProtectChSts[DMA_WPSET_0].uiChannelGroup[DMA_CH_GROUP0+uiWordOffset] |= 0x1 << uiBitOffset;
    }
    if (ProtectIntStsReg.Bit.DMA_WPWD1_STS)
    {
        UINT32 uiWordOffset = ProtectChStsReg.Bit.CHSTS1 / 32;
        UINT32 uiBitOffset = ProtectChStsReg.Bit.CHSTS1 % 32;

        gDmaProtectChSts[DMA_WPSET_1].uiChannelGroup[DMA_CH_GROUP0+uiWordOffset] |= 0x1 << uiBitOffset;
    }
    if (ProtectIntStsReg.Bit.DMA_WPWD2_STS)
    {
        UINT32 uiWordOffset = ProtectChStsReg.Bit.CHSTS2 / 32;
        UINT32 uiBitOffset = ProtectChStsReg.Bit.CHSTS2 % 32;

        gDmaProtectChSts[DMA_WPSET_2].uiChannelGroup[DMA_CH_GROUP0+uiWordOffset] |= 0x1 << uiBitOffset;
    }

    //CALL BACK FUNCTION for set 0
    if (gDmaProtectChSts[DMA_WPSET_0].uiChannelGroup[DMA_CH_GROUP0] ||
        gDmaProtectChSts[DMA_WPSET_0].uiChannelGroup[DMA_CH_GROUP1] ||
        gDmaProtectChSts[DMA_WPSET_0].uiChannelGroup[DMA_CH_GROUP2] ||
        gDmaProtectChSts[DMA_WPSET_0].uiChannelGroup[DMA_CH_GROUP3] ||
        gDmaProtectChSts[DMA_WPSET_0].uiChannelGroup[DMA_CH_GROUP4] ||
        gDmaProtectChSts[DMA_WPSET_0].uiChannelGroup[DMA_CH_GROUP5])
    {
#if (DRV_SUPPORT_IST == ENABLE)
        if (pDmaWPCBFunc[DMA_WPSET_0] != NULL)
        {
            uiEvent |= DMA_IST_EVENT_WPSET0;
        }
#else
        if(pDmaWPCBFunc[DMA_WPSET_0] != NULL)
        {
            pDmaWPCBFunc[DMA_WPSET_0](0);
        }
#endif
        else
        {
            DmaDefaultWriteProtect_CB(DMA_WPSET_0, ProtectChStsReg.Bit.CHSTS0);
        }
    }

    //CALL BACK FUNCTION for set 1
    if (gDmaProtectChSts[DMA_WPSET_1].uiChannelGroup[DMA_CH_GROUP0] ||
        gDmaProtectChSts[DMA_WPSET_1].uiChannelGroup[DMA_CH_GROUP1] ||
        gDmaProtectChSts[DMA_WPSET_1].uiChannelGroup[DMA_CH_GROUP2] ||
        gDmaProtectChSts[DMA_WPSET_1].uiChannelGroup[DMA_CH_GROUP3] ||
        gDmaProtectChSts[DMA_WPSET_1].uiChannelGroup[DMA_CH_GROUP4] ||
        gDmaProtectChSts[DMA_WPSET_1].uiChannelGroup[DMA_CH_GROUP5])
    {
#if (DRV_SUPPORT_IST == ENABLE)
        if (pDmaWPCBFunc[DMA_WPSET_1] != NULL)
        {
            uiEvent |= DMA_IST_EVENT_WPSET1;
        }
#else
        if(pDmaWPCBFunc[DMA_WPSET_1] != NULL)
        {
            pDmaWPCBFunc[DMA_WPSET_1](0);
        }
#endif
        else
        {
            DmaDefaultWriteProtect_CB(DMA_WPSET_1, ProtectChStsReg.Bit.CHSTS1);
        }
    }
    //CALL BACK FUNCTION for set 2
    if (gDmaProtectChSts[DMA_WPSET_2].uiChannelGroup[DMA_CH_GROUP0] ||
        gDmaProtectChSts[DMA_WPSET_2].uiChannelGroup[DMA_CH_GROUP1] ||
        gDmaProtectChSts[DMA_WPSET_2].uiChannelGroup[DMA_CH_GROUP2] ||
        gDmaProtectChSts[DMA_WPSET_2].uiChannelGroup[DMA_CH_GROUP3] ||
        gDmaProtectChSts[DMA_WPSET_2].uiChannelGroup[DMA_CH_GROUP4] ||
        gDmaProtectChSts[DMA_WPSET_2].uiChannelGroup[DMA_CH_GROUP5])
    {
#if (DRV_SUPPORT_IST == ENABLE)
        if (pDmaWPCBFunc[DMA_WPSET_2] != NULL)
        {
            uiEvent |= DMA_IST_EVENT_WPSET2;
        }
#else
        if(pDmaWPCBFunc[DMA_WPSET_2] != NULL)
        {
            pDmaWPCBFunc[DMA_WPSET_2](0);
        }
#endif
        else
        {
            DmaDefaultWriteProtect_CB(DMA_WPSET_2, ProtectChStsReg.Bit.CHSTS2);
        }
    }

#if (DRV_SUPPORT_IST == ENABLE)
    if (uiEvent != 0)
    {
        drv_setIstEvent(DRV_IST_MODULE_DMA, uiEvent);
    }
#endif
}

/*
  DRAM2 control ISR

  DRAM control Interrupt Service Routine

  @return void
*/
void dma2_isr(void)
{
    T_DMA_PROTECT_INTSTS_REG ProtectIntStsReg;
    T_DMA_PROTECT_CHSTS_REG ProtectChStsReg;
    T_DMA_OUT_RANGE_CHANNEL_REG outRangeChReg;
    T_DMA_OUT_RANGE_ADDR_REG outRangeAddrReg;
#if (DRV_SUPPORT_IST == ENABLE)
    UINT32  uiEvent;
#endif

    outRangeChReg.Reg = DMA2_GETREG(DMA_OUT_RANGE_CHANNEL_REG_OFS);
    outRangeAddrReg.Reg = DMA2_GETREG(DMA_OUT_RANGE_ADDR_REG_OFS);

    ProtectIntStsReg.Reg = DMA2_GETREG(DMA_PROTECT_INTSTS_REG_OFS);
    DMA2_SETREG(DMA_PROTECT_INTSTS_REG_OFS, ProtectIntStsReg.Reg);

    ProtectIntStsReg.Reg &= DMA2_GETREG(DMA_PROTECT_INTCTRL_REG_OFS);
    ProtectChStsReg.Reg = DMA2_GETREG(DMA_PROTECT_CHSTS_REG_OFS);

    if(ProtectIntStsReg.Reg == 0)
        return;

#if (DRV_SUPPORT_IST == ENABLE)
    uiEvent = 0;
#endif

    if (ProtectIntStsReg.Bit.AUTO_REFRESH_TIMEOUT_STS)
    {
        DBG_FATAL("Auto Refresh Timeout\r\n");
    }
    if (ProtectIntStsReg.Bit.DMA_OUTRANGE_STS)
    {
//        T_DMA_OUT_RANGE_CHANNEL_REG outRangeChReg;
//        T_DMA_OUT_RANGE_ADDR_REG outRangeAddrReg;

//        outRangeChReg.Reg = DMA2_GETREG(DMA_OUT_RANGE_CHANNEL_REG_OFS);
//        outRangeAddrReg.Reg = DMA2_GETREG(DMA_OUT_RANGE_ADDR_REG_OFS);

        if ((outRangeChReg.Bit.DRAM_OUTRANGE_CH == 0) ||
            (outRangeChReg.Bit.DRAM_OUTRANGE_CH >= DMA_WPCH_COUNT))
        {
            DBG_ERR("Out range channel\r\n");
        }
        else
        {
            DBG_ERR("Out range channel %s, addr 0x%lx\r\n", DmaWPEngineName[outRangeChReg.Bit.DRAM_OUTRANGE_CH-DMA_WPCH_FIRST_CHANNEL],
                                                    outRangeAddrReg.Bit.DRAM_OUTRANGE_ADDR);
        }
        DBG_ERR("Ch coding 0x%lx\r\n", outRangeChReg.Bit.DRAM_OUTRANGE_CH);
        DBG_ERR("int sts 0x%lx\r\n", ProtectIntStsReg.Reg);
    }
    if (ProtectIntStsReg.Bit.DMA_UPDATE_USAGE_STS)
    {
        DMA_MONITOR_CH ch;
        T_DMA_RECORD0_SIZE_REG sizeReg;
        T_DMA_RECORD0_COUNT_REG countReg;

        for (ch=DMA_MONITOR_CH0; ch<DMA_MONITOR_ALL; ch++)
        {
            sizeReg.Reg = DMA2_GETREG(DMA_RECORD0_SIZE_REG_OFS + ch*8);
            countReg.Reg = DMA2_GETREG(DMA_RECORD0_COUNT_REG_OFS + ch*8);

            if (sizeReg.Bit.MONITOR_SIZE_OVF)
                vDmaMonitorObj[DMA_ID_2][ch].uiSizeLog = 0xFFFFFFF;
            else
                vDmaMonitorObj[DMA_ID_2][ch].uiSizeLog = sizeReg.Bit.MONITOR_ACC_SIZE;

            if (countReg.Bit.MONITOR_REQ_OVF)
                vDmaMonitorObj[DMA_ID_2][ch].uiReqLog = 0xFFFFFFF;
            else
                vDmaMonitorObj[DMA_ID_2][ch].uiReqLog = countReg.Bit.MONITOR_ACC_REQ;
        }

        sizeReg.Reg = DMA2_GETREG(DMA_BANDWIDTH_SIZE_REG_OFS);
        countReg.Reg = DMA2_GETREG(DMA_BANDWIDTH_REQ_REG_OFS);

        if (sizeReg.Bit.MONITOR_SIZE_OVF)
            vDmaMonitorObj[DMA_ID_2][DMA_MONITOR_ALL].uiSizeLog = 0xFFFFFFF;
        else
            vDmaMonitorObj[DMA_ID_2][DMA_MONITOR_ALL].uiSizeLog = sizeReg.Bit.MONITOR_ACC_SIZE;

        if (countReg.Bit.MONITOR_REQ_OVF)
            vDmaMonitorObj[DMA_ID_2][DMA_MONITOR_ALL].uiReqLog = 0xFFFFFFF;
        else
            vDmaMonitorObj[DMA_ID_2][DMA_MONITOR_ALL].uiReqLog = countReg.Bit.MONITOR_ACC_REQ;

        if (vDmaMonitorCB[DMA_ID_2] != NULL)
        {
#if (DRV_SUPPORT_IST == ENABLE)
            uiEvent |= DMA_IST_EVENT_MONITOR;
#else
            vDmaMonitorCB[DMA_ID_2](0);
#endif
        }
//        dma_updateMonitorQueue(DMA_MONITOR_CH0);
    }
    if (ProtectIntStsReg.Bit.DMA_WPWD0_STS)
    {
        UINT32 uiWordOffset = ProtectChStsReg.Bit.CHSTS0 / 32;
        UINT32 uiBitOffset = ProtectChStsReg.Bit.CHSTS0 % 32;

        gDmaProtectChSts[DMA_WPSET_0].uiChannelGroup[DMA_CH_GROUP0+uiWordOffset] |= 0x1 << uiBitOffset;
    }
    if (ProtectIntStsReg.Bit.DMA_WPWD1_STS)
    {
        UINT32 uiWordOffset = ProtectChStsReg.Bit.CHSTS1 / 32;
        UINT32 uiBitOffset = ProtectChStsReg.Bit.CHSTS1 % 32;

        gDmaProtectChSts[DMA_WPSET_1].uiChannelGroup[DMA_CH_GROUP0+uiWordOffset] |= 0x1 << uiBitOffset;
    }
    if (ProtectIntStsReg.Bit.DMA_WPWD2_STS)
    {
        UINT32 uiWordOffset = ProtectChStsReg.Bit.CHSTS2 / 32;
        UINT32 uiBitOffset = ProtectChStsReg.Bit.CHSTS2 % 32;

        gDmaProtectChSts[DMA_WPSET_2].uiChannelGroup[DMA_CH_GROUP0+uiWordOffset] |= 0x1 << uiBitOffset;
    }

    //CALL BACK FUNCTION for set 0
    if (gDmaProtectChSts[DMA_WPSET_0].uiChannelGroup[DMA_CH_GROUP0] ||
        gDmaProtectChSts[DMA_WPSET_0].uiChannelGroup[DMA_CH_GROUP1] ||
        gDmaProtectChSts[DMA_WPSET_0].uiChannelGroup[DMA_CH_GROUP2] ||
        gDmaProtectChSts[DMA_WPSET_0].uiChannelGroup[DMA_CH_GROUP3] ||
        gDmaProtectChSts[DMA_WPSET_0].uiChannelGroup[DMA_CH_GROUP4] ||
        gDmaProtectChSts[DMA_WPSET_0].uiChannelGroup[DMA_CH_GROUP5])
    {
#if (DRV_SUPPORT_IST == ENABLE)
        if (pDmaWPCBFunc[DMA_WPSET_0] != NULL)
        {
            uiEvent |= DMA_IST_EVENT_WPSET0;
        }
#else
        if(pDmaWPCBFunc[DMA_WPSET_0] != NULL)
        {
            pDmaWPCBFunc[DMA_WPSET_0](0);
        }
#endif
        else
        {
            DmaDefaultWriteProtect_CB(DMA_WPSET_0, ProtectChStsReg.Bit.CHSTS0);
        }
    }

    //CALL BACK FUNCTION for set 1
    if (gDmaProtectChSts[DMA_WPSET_1].uiChannelGroup[DMA_CH_GROUP0] ||
        gDmaProtectChSts[DMA_WPSET_1].uiChannelGroup[DMA_CH_GROUP1] ||
        gDmaProtectChSts[DMA_WPSET_1].uiChannelGroup[DMA_CH_GROUP2] ||
        gDmaProtectChSts[DMA_WPSET_1].uiChannelGroup[DMA_CH_GROUP3] ||
        gDmaProtectChSts[DMA_WPSET_1].uiChannelGroup[DMA_CH_GROUP4] ||
        gDmaProtectChSts[DMA_WPSET_1].uiChannelGroup[DMA_CH_GROUP5])
    {
#if (DRV_SUPPORT_IST == ENABLE)
        if (pDmaWPCBFunc[DMA_WPSET_1] != NULL)
        {
            uiEvent |= DMA_IST_EVENT_WPSET1;
        }
#else
        if(pDmaWPCBFunc[DMA_WPSET_1] != NULL)
        {
            pDmaWPCBFunc[DMA_WPSET_1](0);
        }
#endif
        else
        {
            DmaDefaultWriteProtect_CB(DMA_WPSET_1, ProtectChStsReg.Bit.CHSTS1);
        }
    }
    //CALL BACK FUNCTION for set 2
    if (gDmaProtectChSts[DMA_WPSET_2].uiChannelGroup[DMA_CH_GROUP0] ||
        gDmaProtectChSts[DMA_WPSET_2].uiChannelGroup[DMA_CH_GROUP1] ||
        gDmaProtectChSts[DMA_WPSET_2].uiChannelGroup[DMA_CH_GROUP2] ||
        gDmaProtectChSts[DMA_WPSET_2].uiChannelGroup[DMA_CH_GROUP3] ||
        gDmaProtectChSts[DMA_WPSET_2].uiChannelGroup[DMA_CH_GROUP4] ||
        gDmaProtectChSts[DMA_WPSET_2].uiChannelGroup[DMA_CH_GROUP5])
    {
#if (DRV_SUPPORT_IST == ENABLE)
        if (pDmaWPCBFunc[DMA_WPSET_2] != NULL)
        {
            uiEvent |= DMA_IST_EVENT_WPSET2;
        }
#else
        if(pDmaWPCBFunc[DMA_WPSET_2] != NULL)
        {
            pDmaWPCBFunc[DMA_WPSET_2](0);
        }
#endif
        else
        {
            DmaDefaultWriteProtect_CB(DMA_WPSET_2, ProtectChStsReg.Bit.CHSTS2);
        }
    }

#if (DRV_SUPPORT_IST == ENABLE)
    if (uiEvent != 0)
    {
        drv_setIstEvent(DRV_IST_MODULE_DMA2, uiEvent);
    }
#endif
}

#if (DRV_SUPPORT_IST == ENABLE)
/*
    DRAM control IST

    DRAM control Interrupt Service Task
    This function is registered to ist in dma_open()

    @param[in] uiEvent      input event bit mask
                            - @b DMA_IST_EVENT_WPSET0: DMA_WPSET_0
                            - @b DMA_IST_EVENT_WPSET1: DMA_WPSET_1
                            - @b DMA_IST_EVENT_WPSET2: DMA_WPSET_2
                            - @b DMA_IST_EVENT_MONITOR: DMA monitor event

    @return void
*/
static void dma_ist(UINT32 uiEvent)
{
    UINT32 i;

    // Check if DMA monitor event is pending
    if (uiEvent & DMA_IST_EVENT_MONITOR)
    {
        uiEvent &= ~DMA_IST_EVENT_MONITOR;

        if (vDmaMonitorCB[DMA_ID_1])
            vDmaMonitorCB[DMA_ID_1](0);
    }

    // Handle DMA write detect events
    if (uiEvent != 0)
    {
        // Only 4 bits, don't apply __builtin_ctz()
        i = 0;
        while (uiEvent != 0)
        {
            if (uiEvent & 0x01)
            {
                if (pDmaWPCBFunc[i])
                    pDmaWPCBFunc[i](0);
            }

            i++;
            uiEvent >>= 1;
        }
    }
}

/*
    DRAM2 control IST

    DRAM control Interrupt Service Task
    This function is registered to ist in dma_open()

    @param[in] uiEvent      input event bit mask
                            - @b DMA_IST_EVENT_WPSET0: DMA_WPSET_0
                            - @b DMA_IST_EVENT_WPSET1: DMA_WPSET_1
                            - @b DMA_IST_EVENT_WPSET2: DMA_WPSET_2
                            - @b DMA_IST_EVENT_MONITOR: DMA monitor event

    @return void
*/
static void dma2_ist(UINT32 uiEvent)
{
    UINT32 i;

    // Check if DMA monitor event is pending
    if (uiEvent & DMA_IST_EVENT_MONITOR)
    {
        uiEvent &= ~DMA_IST_EVENT_MONITOR;

        if (vDmaMonitorCB[DMA_ID_2])
            vDmaMonitorCB[DMA_ID_2](0);
    }

    // Handle DMA write detect events
    if (uiEvent != 0)
    {
        // Only 4 bits, don't apply __builtin_ctz()
        i = 0;
        while (uiEvent != 0)
        {
            if (uiEvent & 0x01)
            {
                if (pDmaWPCBFunc[i])
                    pDmaWPCBFunc[i](0);
            }

            i++;
            uiEvent >>= 1;
        }
    }
}
#endif

/*
    Check if DMA controller is active

    Check if DMA controller is active

    @param[in] id           DMA controller ID

    @return BOOL
        - @b TRUE:  active
        - @b FALSE: inactive
*/
static BOOL dma_chkDramActive(DMA_ID id)
{
    T_DMA_CONTROL_REG   dmaCtrlReg;

    dmaCtrlReg.Reg = dma_getReg(id, DMA_CONTROL_REG_OFS);
    // assume auto refresh is identical to DMA controller EN
    if (dmaCtrlReg.Bit.AUTO_REFRESH_CTRL == 0)
        return FALSE;

    return TRUE;
}


/*
    Check the  specified DMA channel whether write data to protecting address or not

    Check the  specified DMA channel whether write data to protecting address or not

    @param[in] WpSet        Write protect function set
    @param[in] chkChannel   To be check DMA channel

    @return BOOL
        - @b TRUE:  Checked DMA chanel has written data to protecting area.
        - @b FALSE: Checked DMA chanel isn't writting data to protecting area yet.
*/
BOOL dma_chkDmaWR2ProtectAddr(DMA_WRITEPROT_SET WpSet, DMA_WRITEPROT_CH chkChannel)
{
    if(chkChannel == DMA_WPCH_ALL)
    {
        return (BOOL)((gDmaProtectChSts[WpSet].uiChannelGroup[DMA_CH_GROUP0] |
                        gDmaProtectChSts[WpSet].uiChannelGroup[DMA_CH_GROUP1] |
                        gDmaProtectChSts[WpSet].uiChannelGroup[DMA_CH_GROUP2] |
                        gDmaProtectChSts[WpSet].uiChannelGroup[DMA_CH_GROUP3] |
                        gDmaProtectChSts[WpSet].uiChannelGroup[DMA_CH_GROUP4] |
                        gDmaProtectChSts[WpSet].uiChannelGroup[DMA_CH_GROUP5]) != 0);
    }
    else if (chkChannel < DMA_WPCH_COUNT)
    {
        UINT32 uiWordOffset = chkChannel / 32;
        UINT32 uiBitOffset = chkChannel % 32;

        return (BOOL)((gDmaProtectChSts[WpSet].uiChannelGroup[DMA_CH_GROUP0+uiWordOffset] & (0x1<<uiBitOffset)) != 0);
    }
    else
    {
        DBG_ERR("unknow dma write protect channel[%2d]\r\n", chkChannel);
        return FALSE;
    }

}

/*
    Clear the  status of specified DMA channel.

    Clear the  status of specified DMA channel.

    @param[in] WpSet        Write protect function set
    @param[in] chkChannel   To be clear status of the specified DMA channel.

    @return void
*/
void dma_clrWPStatus(DMA_WRITEPROT_SET WpSet, DMA_WRITEPROT_CH chkChannel)
{
    if(chkChannel == DMA_WPCH_ALL)
    {
        memset(gDmaProtectChSts[WpSet].uiChannelGroup, 0, sizeof(DMA_WP_STS_TYPE));
    }
    else if (chkChannel < DMA_WPCH_COUNT)
    {
        UINT32 uiWordOffset = chkChannel / 32;
        UINT32 uiBitOffset = chkChannel % 32;

        gDmaProtectChSts[WpSet].uiChannelGroup[DMA_CH_GROUP0+uiWordOffset] &= ~(0x1<<uiBitOffset);
    }
    else
    {
        DBG_ERR("unknow dma write protect channel[%2d]\r\n", chkChannel);
    }

}


/*
    get the  write protect status.

    get the  status which DMA channel has been write data to protecting address.

    @param[in] WpSet  Write protect function set
    @param[out] pWpSts          High word (bit32~63)status of write protect. Each bit indicate one DMA channel.(Refer to DMA_WRITEPROT_CH enum)
                                Bit32~ Bit34 is mapping to DMA channel DMA_WPCH_IFE0~DMA_WPCH_GRPH_OUTPUT.
                                low word status of write protect.user Each bit indicate one DMA channel.(Refer to DMA_WRITEPROT_CH enum)
                                (Bit2~ Bit31) is mapping to DMA channel DMA_WPCH_SIE0~DMA_WPCH_DAI1.
                                Combination to UINT64
    @return void
*/
void dma_getWPStatus(DMA_WRITEPROT_SET WpSet, DMA_WP_STS_TYPE *pWpSts)
{
    memcpy(pWpSts->uiChannelGroup, gDmaProtectChSts[WpSet].uiChannelGroup, sizeof(DMA_WP_STS_TYPE));
}


/*
    Config DMA write protection function.

    This function is used to config DMA write protection function.
    The using right of write protect function must have been gotten before calling this function,

    @param[in] WpSet        Write protect function set
    @param[in] PprotectAttr Configuration for write protect function
    @param[in] pDrvcb       callback handler for write protect function

    @return void
*/
void dma_configWPFunc(DMA_WRITEPROT_SET WpSet, PDMA_WRITEPROT_ATTR PprotectAttr, DRV_CB pDrvcb)
{
    memcpy(&gDmaProtectAttr[WpSet].chEnMask, &PprotectAttr->chEnMask, sizeof(DMA_CH_MSK));
    gDmaProtectAttr[WpSet].uiProtectlel = PprotectAttr->uiProtectlel;

//#if (DRV_SUPPORT_IST == ENABLE)
//    pfIstCB_DMA[WpSet] = pDrvcb;
//#else
    pDmaWPCBFunc[WpSet] = pDrvcb;
//#endif

    // NT96680 DMA only support DDR3
    {
        //for DDR3 the starting address and size must be 4 word alignment
        if((PprotectAttr->uiStartingAddr & 0x0000000f) != 0x00000000)
        {
            DBG_WRN("DMA_WP: starting address isn't 4 words alignment!\r\n");
        }
        if((PprotectAttr->uiSize & 0x0000000f) != 0x00000000)
        {
            DBG_WRN("DMA_WP: protecting size isn't 4 words alignment!\r\n");
        }
        gDmaProtectAttr[WpSet].uiStartingAddr = PprotectAttr->uiStartingAddr & 0xFFFFFFF0;
        gDmaProtectAttr[WpSet].uiSize = PprotectAttr->uiSize - 1;
    }
}

/*
    Enable DMA write protect function.

    Enable DMA write protect function.

    @param[in] WpSet  Write protect function set

    @return void
*/
void dma_enableWPFunc(DMA_WRITEPROT_SET WpSet)
{
    T_DMA_PROTECT_STARTADDR0_REG uiStartingAddr0 = {0};
    T_DMA_PROTECT_STOPADDR0_REG uiStopAddr0 = {0};
    T_DMA_PROTECT_INTCTRL_REG dma1WpIntCtrl;
    T_DMA_PROTECT_INTCTRL_REG dma2WpIntCtrl;
    T_DMA_PROTECT_INTSTS_REG intStsReg = {0};
    T_DMA_PROTECT_CTRL_REG dma1WpCtrl;
    T_DMA_PROTECT_CTRL_REG dma2WpCtrl;
//    UINT32  uiWpStsReg;
    UINT32 vChMask[DMA_CH_GROUP_CNT];
    UINT32 uiStartPhyAddr, uiEndPhyAddr, uiDma2EndPhy;

    uiStartPhyAddr = dma_getPhyAddr(gDmaProtectAttr[WpSet].uiStartingAddr);
    uiEndPhyAddr = dma_getPhyAddr(gDmaProtectAttr[WpSet].uiStartingAddr) + gDmaProtectAttr[WpSet].uiSize;
    uiDma2EndPhy = dma_getPhyAddr(dma_getDramBaseAddr(DMA_ID_2)) + dma_getDramCapacity(DMA_ID_2);

    dma1WpCtrl.Reg = DMA_GETREG(DMA_PROTECT_CTRL_REG_OFS);
    dma2WpCtrl.Reg = DMA2_GETREG(DMA_PROTECT_CTRL_REG_OFS);
    dma1WpIntCtrl.Reg = DMA_GETREG(DMA_PROTECT_INTCTRL_REG_OFS);
    dma2WpIntCtrl.Reg = DMA2_GETREG(DMA_PROTECT_INTCTRL_REG_OFS);

    {
        //set starting address and stop address
        // 1. check if protect range is in DMA controller 1
        if (dma_getPhyAddr(gDmaProtectAttr[WpSet].uiStartingAddr) < dma_getDramCapacity(DMA_ID_1))
        {
            if (uiEndPhyAddr >= dma_getDramCapacity(DMA_ID_1))
            {
                uiStopAddr0.Bit.STP_ADDR = dma_getDramCapacity(DMA_ID_1) - 1;
            }
            else
            {
                uiStopAddr0.Bit.STP_ADDR = uiEndPhyAddr;
            }

            uiStartingAddr0.Bit.STA_ADDR = uiStartPhyAddr;

            DMA_SETREG(PROTECT_START_ADDR_OFS(WpSet), uiStartingAddr0.Reg);
            DMA_SETREG(PROTECT_END_ADDR_OFS(WpSet), uiStopAddr0.Reg);

            dma1WpCtrl.Reg |= 1<<WpSet;
        }
        else
        {
            dma1WpCtrl.Reg &= ~(1<<WpSet);
        }
        // 2. check if protect range is in DMA controller 2
        if (uiEndPhyAddr > dma_getPhyAddr(dma_getDramBaseAddr(DMA_ID_2)))
        {
            if (uiStartPhyAddr < dma_getPhyAddr(dma_getDramBaseAddr(DMA_ID_2)))
            {
                uiStartingAddr0.Bit.STA_ADDR = dma_getPhyAddr(dma_getDramBaseAddr(DMA_ID_2));
            }
            else
            {
                uiStartingAddr0.Bit.STA_ADDR = uiStartPhyAddr;
            }

            if (uiEndPhyAddr >= uiDma2EndPhy)
            {
                uiStopAddr0.Bit.STP_ADDR = uiDma2EndPhy - 1;
            }
            else
            {
                uiStopAddr0.Bit.STP_ADDR = uiEndPhyAddr;
            }

            DMA2_SETREG(PROTECT_START_ADDR_OFS(WpSet), uiStartingAddr0.Reg);
            DMA2_SETREG(PROTECT_END_ADDR_OFS(WpSet), uiStopAddr0.Reg);

            dma2WpCtrl.Reg |= 1<<WpSet;
        }
        else
        {
            dma2WpCtrl.Reg &= ~(1<<WpSet);
        }


#if _FPGA_EMULATION_
        DBG_IND("%s: sizeof vChMask %d, sizeof DMA_CH_MSK %d\r\n", __func__, sizeof(vChMask), sizeof(DMA_CH_MSK));
#endif
        memcpy(vChMask, &gDmaProtectAttr[WpSet].chEnMask, sizeof(DMA_CH_MSK));

        DMA_SETREG(PROTECT_CH_MSK0_OFS(WpSet), vChMask[DMA_CH_GROUP0]);
        DMA_SETREG(PROTECT_CH_MSK1_OFS(WpSet), vChMask[DMA_CH_GROUP1]);
        DMA_SETREG(PROTECT_CH_MSK2_OFS(WpSet), vChMask[DMA_CH_GROUP2]);
        DMA_SETREG(PROTECT_CH_MSK3_OFS(WpSet), vChMask[DMA_CH_GROUP3]);
        DMA_SETREG(PROTECT_CH_MSK4_OFS(WpSet), vChMask[DMA_CH_GROUP4]);
        DMA_SETREG(PROTECT_CH_MSK5_OFS(WpSet), vChMask[DMA_CH_GROUP5]);
        DMA2_SETREG(PROTECT_CH_MSK0_OFS(WpSet), vChMask[DMA_CH_GROUP0]);
        DMA2_SETREG(PROTECT_CH_MSK1_OFS(WpSet), vChMask[DMA_CH_GROUP1]);
        DMA2_SETREG(PROTECT_CH_MSK2_OFS(WpSet), vChMask[DMA_CH_GROUP2]);
        DMA2_SETREG(PROTECT_CH_MSK3_OFS(WpSet), vChMask[DMA_CH_GROUP3]);
        DMA2_SETREG(PROTECT_CH_MSK4_OFS(WpSet), vChMask[DMA_CH_GROUP4]);
        DMA2_SETREG(PROTECT_CH_MSK5_OFS(WpSet), vChMask[DMA_CH_GROUP5]);

        dma1WpIntCtrl.Reg |= 1<<WpSet;
        dma2WpIntCtrl.Reg |= 1<<WpSet;
        if(gDmaProtectAttr[WpSet].uiProtectlel == DMA_WPLEL_DETECT)
        {
            dma1WpCtrl.Reg |= 1<<(16+WpSet);
            dma2WpCtrl.Reg |= 1<<(16+WpSet);
        }
        else
        {
            dma1WpCtrl.Reg &= ~(1<<(16+WpSet));
            dma2WpCtrl.Reg &= ~(1<<(16+WpSet));
        }
    }

    memset(gDmaProtectChSts[WpSet].uiChannelGroup, 0, sizeof(DMA_WP_STS_TYPE));

    intStsReg.Bit.DMA_WPWD0_STS = 1;
    intStsReg.Bit.DMA_WPWD1_STS = 1;
    intStsReg.Bit.DMA_WPWD2_STS = 1;

    loc_cpu();

    //Clear interrupt status
    DMA_SETREG(DMA_PROTECT_INTSTS_REG_OFS, intStsReg.Reg);
    DMA2_SETREG(DMA_PROTECT_INTSTS_REG_OFS, intStsReg.Reg);

//    drv_enableInt(DRV_INT_DMA);   // move do dma_open()

    DMA_SETREG(DMA_PROTECT_CTRL_REG_OFS, dma1WpCtrl.Reg);
    DMA2_SETREG(DMA_PROTECT_CTRL_REG_OFS, dma2WpCtrl.Reg);
    DMA_SETREG(DMA_PROTECT_INTCTRL_REG_OFS, dma1WpIntCtrl.Reg);
    DMA2_SETREG(DMA_PROTECT_INTCTRL_REG_OFS, dma2WpIntCtrl.Reg);

    unl_cpu();
}

/*
    Disable specific set of DMA write protect function.

    Disable specific set of DMA write protect function.

    @param[in] WpSet  Write protect function set

    @return void
*/
void dma_disableWPFunc(DMA_WRITEPROT_SET WpSet)
{
    T_DMA_PROTECT_INTCTRL_REG dma1WpIntCtrl;
    T_DMA_PROTECT_INTCTRL_REG dma2WpIntCtrl;
    T_DMA_PROTECT_CTRL_REG dma1WpCtrl;
    T_DMA_PROTECT_CTRL_REG dma2WpCtrl;

    dma1WpCtrl.Reg = DMA_GETREG(DMA_PROTECT_CTRL_REG_OFS);
    dma1WpIntCtrl.Reg = DMA_GETREG(DMA_PROTECT_INTCTRL_REG_OFS);
    dma2WpCtrl.Reg = DMA2_GETREG(DMA_PROTECT_CTRL_REG_OFS);
    dma2WpIntCtrl.Reg = DMA2_GETREG(DMA_PROTECT_INTCTRL_REG_OFS);

    dma1WpIntCtrl.Reg &= ~(1<<WpSet);
    dma1WpCtrl.Reg &= ~(1<<WpSet);
    dma2WpIntCtrl.Reg &= ~(1<<WpSet);
    dma2WpCtrl.Reg &= ~(1<<WpSet);

//    drv_disableInt(DRV_INT_DMA);

    loc_cpu();

    DMA_SETREG(DMA_PROTECT_CTRL_REG_OFS, dma1WpCtrl.Reg);
    DMA2_SETREG(DMA_PROTECT_CTRL_REG_OFS, dma2WpCtrl.Reg);
    DMA_SETREG(DMA_PROTECT_INTCTRL_REG_OFS, dma1WpIntCtrl.Reg);
    DMA2_SETREG(DMA_PROTECT_INTCTRL_REG_OFS, dma2WpIntCtrl.Reg);

    unl_cpu();
}


/*
    Get write protect channel No.

    Get write protect channel number

    @param[in] WpSts    Point of dma write protect channel number array

    @return WP channel number

*/
UINT32  dma_getWPStsChannel(DMA_WP_STS_TYPE WpSts)
{
    UINT32 index;
    UINT32 uiChkWpSts;
    UINT32 uiResult;
    BOOL bOtherAssert = FALSE;

    uiResult = 0;
    uiChkWpSts = WpSts.uiChannelGroup[DMA_CH_GROUP0];
    // Find 1st non-zero LSB bit position
    for (index=0; index<DMA_WPCH_COUNT; )
    {
        UINT32 i;

        if (uiChkWpSts)
        {
            i = __builtin_ctz(uiChkWpSts);
            uiChkWpSts &= ~(1<<i);
            uiResult = index + i;
            break;
        }

        if (uiChkWpSts == 0)
        {
            index += 32;
            if (index < DMA_WPCH_COUNT)
            {
                uiChkWpSts = WpSts.uiChannelGroup[index/32];
            }
            else
            {
                uiChkWpSts = 0;
            }
        }
    }

    // Find if other dma protect/detect asserted
    while (index < DMA_WPCH_COUNT)
    {
        if (uiChkWpSts)
        {
            bOtherAssert = TRUE;
            break;
        }
        else
        {
            index += 32;
            if (index < DMA_WPCH_COUNT)
            {
                uiChkWpSts = WpSts.uiChannelGroup[index/32];
            }
            else
            {
                uiChkWpSts = 0;
            }
        }
    }

    if (bOtherAssert)
    {
        DBG_WRN("Another dma channel assert wp status\r\n");
    }

    return uiResult;
}

/**
    Get Dram type

    Get Dram type

    @return DDR_TYPE
        - @b DDR_TYPE_DDR3: DDR3 type dram
*/
DMA_DDR_TYPE dma_getDramType(void)
{
    return DDR_TYPE_DDR3;
}

/*
    Enable / Disable precharge power down

    @param[in] flag  enable precharge power down
        - @b TRUE:  enable precharge power down
        - @b FALSE: disable precharge power down
    @return void
*/
void dma_setPrechargePowerDown(DMA_ID id, BOOL flag)
{
    T_DMA_ENGINEER_RESERVE_REG engReg;

    engReg.Reg = dma_getReg(id, DMA_ENGINEER_RESERVE_REG_OFS);
    if (flag)
    {
        engReg.Bit.PRECHARGE_PD = 1;
    }
    else
    {
        engReg.Bit.PRECHARGE_PD = 0;
    }

    dma_setReg(id, DMA_ENGINEER_RESERVE_REG_OFS, engReg.Reg);
}


/*

    Flush read (Device to CPU) cache without checking buffer is cacheable or not


    In DMA operation, if buffer is cacheable, we have to flush read buffer before
    DMA operation to make sure CPU will read correct data.



    @param[in] uiStartAddr  Buffer starting address
    @param[in] uiLength     Buffer length
    @return Use clean and invalidate data cache all or not (cpu_cleanInvalidateDCacheAll)
        - @b TRUE:   Use cpu_cleanInvalidateDCacheAll
        - @b FALSE:  Not use cpu_cleanInvalidateDCacheAll
*/
BOOL dma_flushReadCacheWithoutCheck(UINT32 uiStartAddr, UINT32 uiLength)
{
    BOOL   bIsCleanInvalidateDCacheAll;
    const UINT32 uiCacheLineSizeMask = (1<<CACHE_SET_NUMBER_SHIFT) - 1;
    UINT32 uiEndAddr = (uiStartAddr + uiLength);

    // Start address isn't cache line alignment
    // In read operation, we only need to invalidate the buffer.
    // Since invalidate is based on cache line (64 bytes), if the starting address
    // isn't cache line alignment, invalidate the line will invalidate
    // non-buffer address. If these address is in cache and dirty,
    // invalidate operation will cause data inconsistent problem.
    // So we have to clean the data cache to write dirty data back to DRAM.
    if ((uiStartAddr & uiCacheLineSizeMask) != 0)
    {
        cpu_cleanInvalidateDCache(uiStartAddr);
    }


    // End address isn't cache line alignment
    if (((uiStartAddr + uiLength) & uiCacheLineSizeMask) != 0)
    {
        cpu_cleanInvalidateDCache(uiStartAddr + uiLength);
    }

    // Invalidate data cache
    // cover the case where address = 0xXXXXXXX0
    // So, need - 1
    uiStartAddr += uiCacheLineSizeMask;
    uiStartAddr &= ~uiCacheLineSizeMask;

    // cahcle line size alignment
    if (uiEndAddr & uiCacheLineSizeMask)
    {
        uiEndAddr &= ~uiCacheLineSizeMask;
    }


    if((uiEndAddr - uiStartAddr) > CACHE_FLUSHALL_BOUNDARY)
    {
        cpu_cleanInvalidateDCacheAll();
        bIsCleanInvalidateDCacheAll = TRUE;
    }
    else
    {
        //Use aligned end address
        cpu_invalidateDCacheBlock(uiStartAddr, uiEndAddr);
        bIsCleanInvalidateDCacheAll = FALSE;
    }
    return bIsCleanInvalidateDCacheAll;
}


/*
    Flush write (CPU to Device) cache without checking buffer is cacheable or not

    In DMA operation, if buffer is cacheable, we have to flush write buffer before
    DMA operation to make sure DMA will send correct data.

    @note   Depend on performance measurement
        clean data cache 016K = 00015 us
        clean data cache 032K = 00020 us
        clean data cache 064K = 00031 us
        clean data cache 128K = 00056 us
        clean data cache 256K = 00102 us
        clean data cache 512K = 00198 us

        fatch 16K data cache  = 00024 us

        So, once if (uiEndAddr - uiStartAddr) > 32K
        calling cpu_cleanInvalidateDCacheAll

    @param[in] uiStartAddr  Buffer starting address
    @param[in] uiLength     Buffer length
    @return Use clean and invalidate data cache all or not (cpu_cleanInvalidateDCacheAll)
        - @b TRUE:   Use cpu_cleanInvalidateDCacheAll
        - @b FALSE:  Not use cpu_cleanInvalidateDCacheAll
*/
BOOL dma_flushWriteCacheWithoutCheck(UINT32 uiStartAddr, UINT32 uiLength)
{
    BOOL   bIsCleanInvalidateDCacheAll;
    // line size
    const UINT32 uiCacheLineSizeMask = (1<<CACHE_SET_NUMBER_SHIFT) - 1;
    UINT32 uiEndAddr = (uiStartAddr + uiLength);

    uiStartAddr &= ~uiCacheLineSizeMask;
    // cahcle line size alignment
    if (uiEndAddr & uiCacheLineSizeMask)
    {
        uiEndAddr &= ~uiCacheLineSizeMask;
        uiEndAddr += (1<<CACHE_SET_NUMBER_SHIFT);
    }

    if((uiEndAddr - uiStartAddr) > CACHE_FLUSHALL_BOUNDARY)
    {
        cpu_cleanInvalidateDCacheAll();
        bIsCleanInvalidateDCacheAll = TRUE;
    }
    else
    {
        cpu_cleanDCacheBlock(uiStartAddr, uiEndAddr);
        bIsCleanInvalidateDCacheAll = FALSE;
    }
    return bIsCleanInvalidateDCacheAll;
}

/*
    Flush read (Device to CPU) cache where lineoffset not equal to width
    Flush read (Device to CPU) cache where lineoffset not equal to width

    @param[in] uiStartAddr  Buffer starting address
    @param[in] uiLength     Buffer length
    @return Use clean and invalidate data cache all or not (cpu_cleanInvalidateDCacheAll)
        - @b TRUE:   Use cpu_cleanInvalidateDCacheAll
        - @b FALSE:  Not use cpu_cleanInvalidateDCacheAll

*/
BOOL dma_flushReadCacheWidthNEQLineOffsetWithoutCheck(UINT32 uiStartAddr, UINT32 uiLength)
{
    BOOL   bIsCleanInvalidateDCacheAll;
    // line size
    const UINT32 uiCacheLineSizeMask = (1<<CACHE_SET_NUMBER_SHIFT) - 1;
    UINT32 uiEndAddr = (uiStartAddr + uiLength);

    uiStartAddr &= ~uiCacheLineSizeMask;
    // cahcle line size alignment
    if (uiEndAddr & uiCacheLineSizeMask)
    {
        uiEndAddr &= ~uiCacheLineSizeMask;
        uiEndAddr += (1<<CACHE_SET_NUMBER_SHIFT);
    }

    if((uiEndAddr - uiStartAddr) > CACHE_FLUSHALL_BOUNDARY)
    {
        cpu_cleanInvalidateDCacheAll();
        bIsCleanInvalidateDCacheAll = TRUE;
    }
    else
    {
        cpu_cleanInvalidateDCacheBlock(uiStartAddr, uiEndAddr);
        bIsCleanInvalidateDCacheAll = FALSE;
    }
    return bIsCleanInvalidateDCacheAll;
}


/**
    Return DRAM starting address

    Get DRAM starting address for usage (always return non cache area)

    @return DRAM starting address(0x60000000)
*/
UINT32 dma_getDramBaseAddr(DMA_ID id)
{
    if (id == DMA_ID_1)
        return dma_getNonCacheAddr(0x0);
    else
        return dma_getNonCacheAddr(0x40000000);
}

#endif
/*
    Enable specified DMA channel.

    Enable specified DMA channel.

    @param[in] Channel  DMA channel
    @return
        - @b E_OK: enable chanel success
        - @b Else: enable channel fail
*/
ER dma_enableChannel(DMA_CH Channel)
{
    union T_DMA_SDRAM_CTRL_REG0   uiSdramReg0;
    UINT32 uiChannelMask;
    UINT32 uiChannelOffset;
    REGVALUE dmaChannelReg;


    if ((Channel != DMA_CH_ALL) &&
        (Channel >= DMA_CH_COUNT || Channel <= DMA_CH_RSV))
        return E_PAR;

    if(Channel == DMA_CH_ALL)
    {
        uiSdramReg0.Reg = DMA_GETREG(DMA_SDRAM_CTRL_REG0_OFS);
        uiSdramReg0.Bit.DMA_CH_EN_SWITCH = 1;
        DMA_SETREG(DMA_SDRAM_CTRL_REG0_OFS, uiSdramReg0.Reg);

        uiSdramReg0.Reg = DMA2_GETREG(DMA_SDRAM_CTRL_REG0_OFS);
        uiSdramReg0.Bit.DMA_CH_EN_SWITCH = 1;
        DMA2_SETREG(DMA_SDRAM_CTRL_REG0_OFS, uiSdramReg0.Reg);
    }
    if (Channel < 32)
    {
        uiChannelMask = 1 << Channel;

        uiChannelOffset = DMA_CHANNEL_ENABLE0_REG_OFS;
    }
    else if (Channel < 64)
    {
        uiChannelMask = 1 << (Channel-32);

        uiChannelOffset = DMA_CHANNEL_ENABLE1_REG_OFS;
    }
    else if (Channel < 96)
    {
        uiChannelMask = 1 << (Channel-64);

        uiChannelOffset = DMA_CHANNEL_ENABLE2_REG_OFS;
    }
    else if (Channel < 128)
    {
        uiChannelMask = 1 << (Channel-96);

        uiChannelOffset = DMA_CHANNEL_ENABLE3_REG_OFS;
    }
    else if (Channel < 160)
    {
        uiChannelMask = 1 << (Channel-128);

        uiChannelOffset = DMA_CHANNEL_ENABLE4_REG_OFS;
    }
    else if (Channel < 192)
    {
        uiChannelMask = 1 << (Channel-160);

        uiChannelOffset = DMA_CHANNEL_ENABLE5_REG_OFS;
    }
    else if (Channel < 224)
    {
        uiChannelMask = 1 << (Channel-192);

        uiChannelOffset = DMA_CHANNEL_ENABLE6_REG_OFS;
    }
    else //if (Channel < 256)
    {
        uiChannelMask = 1 << (Channel-224);

        uiChannelOffset = DMA_CHANNEL_ENABLE7_REG_OFS;
    }


    loc_cpu();  // enter critical section

    dmaChannelReg = DMA_GETREG(uiChannelOffset);

    dmaChannelReg |= uiChannelMask;

    DMA_SETREG(uiChannelOffset, dmaChannelReg);

    dmaChannelReg = DMA2_GETREG(uiChannelOffset);

    dmaChannelReg |= uiChannelMask;

    DMA2_SETREG(uiChannelOffset, dmaChannelReg);

    unl_cpu();  // exit critical section
    return E_OK;
}
#if 0

/*
    Disable specified DMA channel.

    Disable specified DMA channel.

    @param[in] Channel  DMA channel
    @return
        - @b E_OK: disable chanel success
        - @b Else: disable channel fail
*/
ER dma_disableChannel(DMA_CH Channel)
{
    T_DMA_SDRAM_CTRL_REG0   uiSdramReg0;
    UINT32 uiChannelMask = 0x0;
    UINT32 uiChannelOffset = DMA_CHANNEL_ENABLE0_REG_OFS;
    REGVALUE dmaChannelReg;


    if ((Channel != DMA_CH_ALL) && (Channel >= DMA_CH_COUNT))
        return E_PAR;

    if(Channel == DMA_CH_ALL)
    {
        uiSdramReg0.Reg = DMA_GETREG(DMA_SDRAM_CTRL_REG0_OFS);
        uiSdramReg0.Bit.DMA_CH_EN_SWITCH = 0;
        DMA_SETREG(DMA_SDRAM_CTRL_REG0_OFS, uiSdramReg0.Reg);

        uiSdramReg0.Reg = DMA2_GETREG(DMA_SDRAM_CTRL_REG0_OFS);
        uiSdramReg0.Bit.DMA_CH_EN_SWITCH = 0;
        DMA2_SETREG(DMA_SDRAM_CTRL_REG0_OFS, uiSdramReg0.Reg);
    }
    else
    {
        if (Channel < 32)
        {
            uiChannelMask = 1 << Channel;

            uiChannelOffset = DMA_CHANNEL_ENABLE0_REG_OFS;
        }
        else if (Channel < 64)
        {
            uiChannelMask = 1 << (Channel-32);

            uiChannelOffset = DMA_CHANNEL_ENABLE1_REG_OFS;
        }
        else if (Channel < 96)
        {
            uiChannelMask = 1 << (Channel-64);

            uiChannelOffset = DMA_CHANNEL_ENABLE2_REG_OFS;
        }
        else if (Channel < 128)
        {
            uiChannelMask = 1 << (Channel-96);

            uiChannelOffset = DMA_CHANNEL_ENABLE3_REG_OFS;
        }
        else if (Channel < 160)
        {
            uiChannelMask = 1 << (Channel-128);

            uiChannelOffset = DMA_CHANNEL_ENABLE4_REG_OFS;
        }
        else if (Channel < 192)
        {
            uiChannelMask = 1 << (Channel-160);

            uiChannelOffset = DMA_CHANNEL_ENABLE5_REG_OFS;
        }
        else if (Channel < 192)
        {
            uiChannelMask = 1 << (Channel-160);

            uiChannelOffset = DMA_CHANNEL_ENABLE5_REG_OFS;
        }
        else if (Channel < 224)
        {
            uiChannelMask = 1 << (Channel-192);

            uiChannelOffset = DMA_CHANNEL_ENABLE6_REG_OFS;
        }
        else //if (Channel < 256)
        {
            uiChannelMask = 1 << (Channel-224);

            uiChannelOffset = DMA_CHANNEL_ENABLE7_REG_OFS;
        }


        loc_cpu();  // enter critical section

        dmaChannelReg = DMA_GETREG(uiChannelOffset);

        dmaChannelReg &= ~uiChannelMask;

        DMA_SETREG(uiChannelOffset, dmaChannelReg);

        dmaChannelReg = DMA2_GETREG(uiChannelOffset);

        dmaChannelReg &= ~uiChannelMask;

        DMA2_SETREG(uiChannelOffset, dmaChannelReg);

        unl_cpu();  // exit critical section
    }

    if(Channel == DMA_CH_ALL)
    {
        uiSdramReg0.Reg = DMA_GETREG(DMA_SDRAM_CTRL_REG0_OFS);
        while(!uiSdramReg0.Bit.DMA_CH_DIS_DONE)
        {
            uiSdramReg0.Reg = DMA_GETREG(DMA_SDRAM_CTRL_REG0_OFS);
        }
    }

    // DMA controller 2 is not the one that always existed
    // Check if it is active
    if (dma_chkDramActive(DMA_ID_2))
    {
        if(Channel == DMA_CH_ALL)
        {
            uiSdramReg0.Reg = DMA2_GETREG(DMA_SDRAM_CTRL_REG0_OFS);
            while(!uiSdramReg0.Bit.DMA_CH_DIS_DONE)
            {
                uiSdramReg0.Reg = DMA2_GETREG(DMA_SDRAM_CTRL_REG0_OFS);
            }
        }
    }

    return E_OK;
}
#endif
/*
    Set AXI request count.

    set AXI request count for each processor cycle.

    @param[in] channel  AXI channel
    @param[in] count    request count for each cycle
                        - @b 1: 1 request
                        - @b 2: 2 requests
                        - @b 3: 3 requests
                        - @b 4: 4 requests
                        - @b others: invalid

    @return
        - @b E_OK: success
*/
ER dma_setAXIRequestCount(DMA_AXI_CH channel, UINT32 count)
{
    union T_DMA_SDRAM_CTRL_REG0 arb1CtrlReg0, arb2CtrlReg0;
    union T_DMA_SDRAM_CTRL_REG1 arb1CtrlReg1, arb2CtrlReg1;

    if (count < 1)
    {
        printf("Min request count is 1, but input %d\r\n", (u32)count);
        count = 1;
    }
    else if (count > 4)
    {
        printf("Max request count is 4, but input %d\r\n", (u32)count);
        count = 4;
    }
    count--;

    loc_cpu();

    arb1CtrlReg0.Reg = DMA_GETREG(DMA_SDRAM_CTRL_REG0_OFS);
    arb2CtrlReg0.Reg = DMA2_GETREG(DMA_SDRAM_CTRL_REG0_OFS);
    arb1CtrlReg1.Reg = DMA_GETREG(DMA_SDRAM_CTRL_REG1_OFS);
    arb2CtrlReg1.Reg = DMA2_GETREG(DMA_SDRAM_CTRL_REG1_OFS);

    switch (channel)
    {
    case DMA_AXI_CH0:
        arb1CtrlReg0.Bit.AXI_CH0_REQ_NUM = count;
        arb2CtrlReg0.Bit.AXI_CH0_REQ_NUM = count;
        break;
    case DMA_AXI_CH1:
        arb1CtrlReg0.Bit.AXI_CH1_REQ_NUM = count;
        arb2CtrlReg0.Bit.AXI_CH1_REQ_NUM = count;
        break;
    case DMA_AXI_CH2:
        arb1CtrlReg0.Bit.AXI_CH2_REQ_NUM = count;
        arb2CtrlReg0.Bit.AXI_CH2_REQ_NUM = count;
        break;
    case DMA_AXI_CH3:
        arb1CtrlReg0.Bit.AXI_CH3_REQ_NUM = count;
        arb2CtrlReg0.Bit.AXI_CH3_REQ_NUM = count;
        break;
    case DMA_AXI_CH4:
        arb1CtrlReg0.Bit.AXI_CH4_REQ_NUM = count;
        arb2CtrlReg0.Bit.AXI_CH4_REQ_NUM = count;
        break;
    case DMA_AXI_CH5:
        arb1CtrlReg0.Bit.AXI_CH5_REQ_NUM = count;
        arb2CtrlReg0.Bit.AXI_CH5_REQ_NUM = count;
        break;
    case DMA_AXI_CH6:
        arb1CtrlReg0.Bit.AXI_CH6_REQ_NUM = count;   // USB3.0 only connect to ARB1
        break;
    case DMA_AXI_CH7:
        arb1CtrlReg1.Bit.AXI_CH7_REQ_NUM = count;   // ethernet only connect to ARB1
        break;
    default:
        printf("Invalid AXI ch %d\r\n", (u32)channel);
        break;
    }

    DMA_SETREG(DMA_SDRAM_CTRL_REG0_OFS, arb1CtrlReg0.Reg);
    DMA2_SETREG(DMA_SDRAM_CTRL_REG0_OFS, arb2CtrlReg0.Reg);
    DMA_SETREG(DMA_SDRAM_CTRL_REG1_OFS, arb1CtrlReg1.Reg);
    DMA2_SETREG(DMA_SDRAM_CTRL_REG1_OFS, arb2CtrlReg1.Reg);

    unl_cpu();

    return E_OK;
}
#if 0

/*
    Get dram capacity

    Get dram capacity of DMA controller configuration

    @note in DMA controller 0xB000_0000 bit[2..0] where
        => Dram capacity = 1 << (20 + 4 + reg[2..0])
        => 0   128Mb   ==> 16MB    ==> 2 ^ 4 => 1 << 24
        => 1   256Mb   ==> 32MB    ==> 2 ^ 5 => 1 << 25
        => 2   512Mb   ==> 64MB    ==> 2 ^ 6 => 1 << 26
        => 3   1Gb     ==>128MB    ==> 2 ^ 7 => 1 << 27
        => 4   2Gb     ==>256MB    ==> 2 ^ 8 => 1 << 28
        => 5   4Gb     ==>512MB    ==> 2 ^ 9 => 1 << 29

    @return DRAM capacity (unit: byte)

    Example:
    @code
    {
       UINT32 uiDramCapacity = dma_getDramCapacity(DMA_ID_1);
       UINT32 uiDramEndAddress = dma_getDramCapacity + dma_getDramBaseAddr(DMA_ID_1);
    }
    @endcode
*/
UINT32 dma_getDramCapacity(DMA_ID id)
{
    T_DMA_CONFIG_REG    dmaCfgReg;
    UINT32 uiDramCapacity = 0;

    if (dma_chkDramActive(id) == FALSE)
        return 0;

    dmaCfgReg.Reg = dma_getReg(id, DMA_CONFIG_REG_OFS);
    uiDramCapacity = 1 << (24 + dmaCfgReg.Bit.SDRAM_CAPACITY);
    uiDramCapacity <<= dmaCfgReg.Bit.SDRAM_COUNT;

    return uiDramCapacity;
}


/*
    Get DRAM utilization

    Get DRAM bandwidth utilization

    @param[in] id   DMA controller ID

    @return DRAM utilization %

    Example:
    @code
    {
       debug_msg("utilization = %d %\r\n", dma_getBWUtilization());
    }
    @endcode
*/
UINT32 dma_getBWUtilization(DMA_ID id)
{
    T_DMA_BANDWIDTH_REG bandwidthReg;
    T_DMA_RECORD_PERIOD_REG periodReg;

    bandwidthReg.Reg = dma_getReg(id, DMA_BANDWIDTH_REG_OFS);
    periodReg.Reg = dma_getReg(id, DMA_RECORD_PERIOD_REG_OFS);

    if (periodReg.Bit.MONITOR_PERIOD == 0)
    {
        DBG_ERR("DMA%d monitor period is 0\r\n", id);
        return 0;
    }

    return (UINT32)bandwidthReg.Bit.ACT_CYCLE * 100 / periodReg.Bit.MONITOR_PERIOD;
}

/*
    Get Module DRAM utilization

    Get Module DRAM bandwidth utilization

    @param[in]  module for queury

    @return DRAM utilization X / 255 %

    Example:
    @code
    {
       debug_msg("utilization = %d %\r\n", dma_getModuleUtilization());
    }
    @endcode
*/
UINT32 dma_getModuleUtilization(DMA_UTILIZATION_MODULE module)
{
#if 0
    // HH comment: these regsiter are REMOVED, used monitor register instead
    switch (module)
    {
    case DMA_UTILIZATION_MODULE_MIPS1:
        {
            T_DMA_MIPS1_BANDWIDTH_REG bandwidthReg;

            bandwidthReg.Reg = DMA_GETREG(DMA_MIPS1_BANDWIDTH_REG_OFS);
            return ((bandwidthReg.Bit.REQ_NUM)>>8);
        }
    case DMA_UTILIZATION_MODULE_MIPS2:
        {
            T_DMA_MIPS2_BANDWIDTH_REG bandwidthReg;

            bandwidthReg.Reg = DMA_GETREG(DMA_MIPS2_BANDWIDTH_REG_OFS);
            return ((bandwidthReg.Bit.REQ_NUM)>>8);
        }
    case DMA_UTILIZATION_MODULE_DSP1:
        {
            T_DMA_DSP1_BANDWIDTH_REG bandwidthReg;

            bandwidthReg.Reg = DMA_GETREG(DMA_DSP1_BANDWIDTH_REG_OFS);
            return ((bandwidthReg.Bit.REQ_NUM)>>8);
        }
    case DMA_UTILIZATION_MODULE_DSP2:
        {
            T_DMA_DSP1_BANDWIDTH_REG bandwidthReg;

            bandwidthReg.Reg = DMA_GETREG(DMA_DSP2_BANDWIDTH_REG_OFS);
            return ((bandwidthReg.Bit.REQ_NUM)>>8);
        }
    case DMA_UTILIZATION_MODULE_ALL:
    default:
        return dma_getBWUtilization();
    }
#else
    return 0;
#endif
}

/*
    Get DMA monitor information

    Get DMA monitor information (size, request counts)

    @param[in] id       DMA controller ID
    @param[in] ch       select monitor channel
    @param[in] type     select which info need to get
                        - @b DMA_MONITOR_TYPE_SIZE: get transfer size (unit: word) of this channel
                        - @b DMA_MONITOR_TYPE_REQ: get DMA request count

    @return selected information
*/
UINT32 dma_getMonitorInfo(DMA_ID id, DMA_MONITOR_CH ch, DMA_MONITOR_TYPE type)
{
    if (id >= DMA_ID_COUNT) return 0;
    if (ch >= DMA_MONITOR_ALL) return 0;

    if (type == DMA_MONITOR_TYPE_SIZE)
        return vDmaMonitorObj[id][ch].uiSizeLog;
    else
        return vDmaMonitorObj[id][ch].uiReqLog;
}

/*
    Config DMA monitor

    Config DMA monitor

    @param[in] id       DMA controller ID
    @param[in] chMon    select monitor channel
    @param[in] chDma    select DMA channel to be monitored by chMon
    @param[in] direction    select which direction to be monitored
                        - @b DMA_DIRECTION_READ: DRAM -> Module
                        - @b DMA_DIRECTION_WRITE: Module <- DRAM
                        - @b DMA_DIRECTION_BOTH: both direction

    @return
        - @b E_OK: success
        - @b Else: input parameter not valid
*/
ER dma_configMonitor(DMA_ID id, DMA_MONITOR_CH chMon, DMA_CH chDma, DMA_DIRECTION direction)
{
    UINT32 regAddr;
    T_DMA_RECORD_CONFIG0_REG configReg;

    if (id >= DMA_ID_COUNT)
    {
        DBG_ERR("DMA id %d invalid\r\n", id);
        return E_PAR;
    }
    if (chMon >= DMA_MONITOR_ALL)
    {
        DBG_ERR("Monitor ch %d invalid\r\n", chMon);
        return E_PAR;
    }
    if (chDma >= DMA_CH_COUNT)
    {
        DBG_ERR("DMA ch %d invalid\r\n", chDma);
        return E_PAR;
    }

    regAddr = DMA_RECORD_CONFIG0_REG_OFS + (chMon/2) * 4;

    // Enter critical section
    loc_cpu();

    configReg.Reg = dma_getReg(id, regAddr);
    if (chMon & 0x01)
    {
        configReg.Bit.MONITOR1_DMA_CH = chDma;
        configReg.Bit.MONITOR1_DMA_DIR = direction;
    }
    else
    {
        configReg.Bit.MONITOR0_DMA_CH = chDma;
        configReg.Bit.MONITOR0_DMA_DIR = direction;
    }

    dma_setReg(id, regAddr, configReg.Reg);

    // Leave critical section
    unl_cpu();

    return E_OK;
}


/*
    Set heavy loading test pattern

    Set heavy loading test pattern, total 4 sets

    @param[in] id           DMA controller ID
    @param[in] uiPattern0   Pattern0
    @param[in] uiPattern1   Pattern1
    @param[in] uiPattern2   Pattern2
    @param[in] uiPattern3   Pattern3
*/
void dma_setHeavyLoadPattern(DMA_ID id, UINT32 uiPattern0, UINT32 uiPattern1, UINT32 uiPattern2, UINT32 uiPattern3)
{
    dma_setReg(id, DMA_HEAVY_LOAD_TEST_PATTERN0_OFS, uiPattern0);
    dma_setReg(id, DMA_HEAVY_LOAD_TEST_PATTERN1_OFS, uiPattern1);
    dma_setReg(id, DMA_HEAVY_LOAD_TEST_PATTERN2_OFS, uiPattern2);
    dma_setReg(id, DMA_HEAVY_LOAD_TEST_PATTERN3_OFS, uiPattern3);
}

/*
    Enable heavy loading operation

    Start heavy loading operation in specific channel

    @param[in] id           DMA controller ID
    @param[in] uiCh         3 channel are available for configure
    @param[in] hvyParam     configuration information

    @return ER
        - @b E_OK: Enable heavy load success
        - @b E_PAR:Param error
    Example:
    @code
    {
        dma_setHeavyLoadPattern(DMA_ID_1, Pat0, Pat1, Pat2, Pat3); // if necessary(or driver will use random number)

        dma_enableHeavyLoad(DMA_ID_1, chX, param);
        dma_enableHeavyLoad(DMA_ID_1, chY, param);
        dma_enableHeavyLoad(DMA_ID_1, chZ, param);  // Enable maximun 3 channel at the same time

        if(dma_waitHeavyLoadDonePolling(DMA_ID_1, call_back) == TRUE)
        {
            SUCCESS();
        }
        else
        {
            FAIL();
        }
    }
    @endcode

*/
ER dma_enableHeavyLoad(DMA_ID id, DMA_HEAVY_LOAD_CH uiCh, PDMA_HEAVY_LOAD_PARAM hvyParam)
{
    T_DMA_CHANNEL0_HEAVY_LOAD_CTRL_REG  uHvyLoadReg;
    UINT32 uiTempValue;

    uHvyLoadReg.Reg = dma_getReg(id, HEAVY_LOAD_CTRL_OFS(uiCh));



    if(uHvyLoadReg.Bit.CH0_TEST_START == 1)
    {
        DBG_WRN("DMA_HVY(ch%d): already enabled register 0x%08x\r\n", uiCh, dma_getReg(id, HEAVY_LOAD_CTRL_OFS(uiCh)));
        //return E_SYS;
        uHvyLoadReg.Bit.CH0_TEST_START = 0;
        dma_setReg(id, HEAVY_LOAD_CTRL_OFS(uiCh), uHvyLoadReg.Reg);
    }

    uHvyLoadReg.Bit.CH0_TEST_METHOD = hvyParam->uiTestMethod;
    uHvyLoadReg.Bit.CH0_BURST_LEN = hvyParam->uiBurstLen;

    if(hvyParam->uiTestTimes == 0)
    {
        DBG_WRN("Heavy load test time = 0 is not available, auto-adjust it to 1\r\n");
        hvyParam->uiTestTimes++;
    }
    uHvyLoadReg.Bit.CH0_TEST_TIMES = hvyParam->uiTestTimes;

    if((hvyParam->uiStartAddr & 0x00000003) !=0x00000000)
    {
        DBG_WRN("DMA_HVY: starting address isn't words alignment!\r\n");
    }
    if((hvyParam->uiDmaSize & 0x00000003) != 0x00000000)
    {
        DBG_WRN("DMA_HVY: size isn't words alignment!\r\n");
    }

    if(hvyParam->uiDmaSize < 4)
    {
        DBG_ERR("DMA_HVY: size less than 1 word\r\n");
        return E_PAR;
    }
    dma_setReg(id, HEAVY_LOAD_ADDR_OFS(uiCh), dma_getPhyAddr((hvyParam->uiStartAddr & 0xFFFFFFFC)));
    dma_setReg(id, HEAVY_LOAD_SIZE_OFS(uiCh), (hvyParam->uiDmaSize & 0xFFFFFFFC));
    dma_setReg(id, HEAVY_LOAD_CTRL_OFS(uiCh), uHvyLoadReg.Reg);

    //DMA_SETREG(HEAVY_LOAD_WAIT_CYCLE_OFS(uiCh), 0x3);

    if(dma_getReg(id, DMA_HEAVY_LOAD_TEST_PATTERN0_OFS) == 0x00000000)
    {
        uiTempValue = randomUINT32();

        if(uiTempValue == 0)
            uiTempValue = 0x69696969;
        dma_setReg(id, DMA_HEAVY_LOAD_TEST_PATTERN0_OFS, uiTempValue);
        DBG_ERR("DMA_HVY: Pattern0 not configed, assign random number 0x%08x\r\n", uiTempValue);
    }
    if(dma_getReg(id, DMA_HEAVY_LOAD_TEST_PATTERN1_OFS) == 0x00000000)
    {
        uiTempValue = randomUINT32();

        if(uiTempValue == 0)
            uiTempValue = 0x5a5a5a5a;
        dma_setReg(id, DMA_HEAVY_LOAD_TEST_PATTERN1_OFS, uiTempValue);
        DBG_ERR("DMA_HVY: Pattern1 not configed, assign random number 0x%08x\r\n", uiTempValue);
    }
    if(dma_getReg(id, DMA_HEAVY_LOAD_TEST_PATTERN2_OFS) == 0x00000000)
    {
        uiTempValue = randomUINT32();
        if(uiTempValue == 0)
            uiTempValue = 0x55aaaa55;
        dma_setReg(id, DMA_HEAVY_LOAD_TEST_PATTERN2_OFS, uiTempValue);
        DBG_ERR("DMA_HVY: Pattern2 not configed, assign random number 0x%08x\r\n", uiTempValue);
    }
    if(dma_getReg(id, DMA_HEAVY_LOAD_TEST_PATTERN3_OFS) == 0x00000000)
    {
        uiTempValue = randomUINT32();

        if(uiTempValue == 0)
            uiTempValue = 0xaa5555aa;
        dma_setReg(id, DMA_HEAVY_LOAD_TEST_PATTERN3_OFS, uiTempValue);
        DBG_ERR("DMA_HVY: Pattern3 not configed, assign random number 0x%08x\r\n", uiTempValue);
    }

    uHvyLoadReg.Bit.CH0_TEST_START = 1;
    dma_setReg(id, HEAVY_LOAD_CTRL_OFS(uiCh), uHvyLoadReg.Reg);
    DBG_IND("channel %d\r\n", uiCh);

    return E_OK;
}


/*
    Polling status of heavy load

    Polling status bit of heavy load controller, done or error occurred

    @note : If more than 1 channel, done status will assert until all channel operation done
            and error status will occurred once if any channel encounter error

    @param[in] id               DMA controller ID
    @param[in] pCallBackHdl     Heavy Loading callback function during polling heavy load done,
                                Upper layer can display utilization there, if NULL, driver will
                                display utilization automatically
    @return BOOL
        - @b TRUE: Test success
        - @b TRUE: Test error occurred
*/
BOOL dma_waitHeavyLoadDonePolling(DMA_ID id, DMA_HVYLoadCallbackFunc pCallBackHdl)
{
//    UINT32 uiResult = 0;
    UINT32 cnt;
    T_DMA_HEAVY_LOAD_STS_REG stsReg = {0};

    stsReg.Reg = dma_getReg(id, DMA_HEAVY_LOAD_STS_OFS);

    cnt = 0;
    while(stsReg.Reg == 0)
    {
        cnt++;
        stsReg.Reg = dma_getReg(id, DMA_HEAVY_LOAD_STS_OFS);

        if(pCallBackHdl)
        {
            pCallBackHdl();
        }
        else
        {
            if(cnt % 100 == 0)
            {
                DBG_DUMP("DMA%d utilization => [%02d%]\r\n", id, dma_getBWUtilization(id));
//                DBG_DUMP("DMA%d utilization => [%02d%]\r\n", id, ((dma_getBWUtilization(id)*100)/255));
            }
        }
        Delay_DelayMs(10);
    }

    if (stsReg.Bit.HEAVY_LOAD_COMPLETE_STS)
    {
        DBG_IND("heavy loading test success\r\n");
        dma_setReg(id, DMA_HEAVY_LOAD_STS_OFS, stsReg.Reg);
        return TRUE;
    }
    else if (stsReg.Bit.HEAVY_LOAD_ERROR_STS)
    {
        DBG_ERR("heavy loading test fail @[0x%08x] V[0x%08x] vs X[0x%08x]\r\n", DMA_GETREG(DMA_HEAVY_LOAD_CURRENT_ADDR_OFS), DMA_GETREG(DMA_HEAVY_LOAD_CORRECT_DATA_OFS), DMA_GETREG(DMA_HEAVY_LOAD_ERROR_DATA_OFS));
        dma_setReg(id, DMA_HEAVY_LOAD_STS_OFS, stsReg.Reg);
        return FALSE;
    }
    else
    {
        DBG_ERR("Heavy loading test unknow result sts = [0x%08x] \r\n", stsReg.Reg);
        dma_setReg(id, DMA_HEAVY_LOAD_STS_OFS, stsReg.Reg);
        return FALSE;
    }
}

/*
    Get heavy load checksum

    Enable heavy load to calculate 16 bit checksum and get result

    @param[in] id           DMA controller ID
    @param[in] uiCh         3 channel are available for configure
    @param[in] uiBurstLen   0 ~ 127
    @param[in] uiAddr       DRAM buffer address (should word align)
    @param[in] uiLength     buffer length (should word align)

    @return checksum
*/
UINT32 dma_getHeavyLoadChkSum(DMA_ID id, DMA_HEAVY_LOAD_CH uiCh, UINT32 uiBurstLen, UINT32 uiAddr, UINT32 uiLength)
{
    T_DMA_CHANNEL0_HEAVY_LOAD_WAIT_CYCLE waitCycleReg;
    T_DMA_CHANNEL0_HEAVY_LOAD_CTRL_REG  uHvyLoadReg;
    DMA_HEAVY_LOAD_PARAM hvyParam = {0};

    if (uiAddr & 0x03)
    {
        DBG_ERR("input addr not word align: 0x%lx\r\n", uiAddr);
        return 0;
    }
    if (uiLength & 0x03)
    {
        DBG_ERR("input length not word align: 0x%lx\r\n", uiLength);
        return 0;
    }

    // Configure to skip error when compare not match
    uHvyLoadReg.Reg = dma_getReg(id, HEAVY_LOAD_CTRL_OFS(uiCh));
    uHvyLoadReg.Bit.CH0_SKIP_COMPARE = 1;
    dma_setReg(id, HEAVY_LOAD_CTRL_OFS(uiCh), uHvyLoadReg.Reg);

    hvyParam.uiBurstLen = uiBurstLen;
    hvyParam.uiDmaSize = uiLength;
    hvyParam.uiTestMethod = DMA_HEAVY_LOAD_READ_ONLY;
    hvyParam.uiStartAddr = dma_getPhyAddr(uiAddr);
    hvyParam.uiTestTimes = 1;

    // flush cache
    dma_flushWriteCache(uiAddr, uiLength);

    // Enable heavy load to calculate checksum
    if (dma_enableHeavyLoad(id, uiCh, &hvyParam) != E_OK)
    {
        return 0;
    }
    // Wait heavy load complete
    if (dma_waitHeavyLoadDonePolling(id, NULL) == FALSE)
    {
        DBG_ERR("heavy load result fail\r\n");
    }

    // Read register that contains checksum result
    waitCycleReg.Reg = dma_getReg(id, HEAVY_LOAD_WAIT_CYCLE_OFS(uiCh));

    // Restore to detect error when compare not match
    uHvyLoadReg.Reg = dma_getReg(id, HEAVY_LOAD_CTRL_OFS(uiCh));
    uHvyLoadReg.Bit.CH0_SKIP_COMPARE = 0;
    dma_setReg(id, HEAVY_LOAD_CTRL_OFS(uiCh), uHvyLoadReg.Reg);

    return waitCycleReg.Bit.CHECKSUM;
}

/*
    Get specific DMA channel name

    Get specific DMA channel name provided by dma driver layer

    @param[in] uiCh     Specific DMA channel number

    @return string of DMA channel name
*/
const CHAR * dma_getChannelName(DMA_CH uiCh)
{
    if(uiCh >= DMA_CH_COUNT)
    {
        DBG_ERR("Unknow dma channel = [%02d] \r\n", uiCh);
        return NULL;
    }
    else
    {
        return DmaWPEngineName[uiCh-DMA_CH_FIRST_CHANNEL];
    }
}


/*
    Clear DMA channel request and ack flag

    Clear DMA channel request and ack flag

    @return void
*/
void dma_clearRequestChannel(void)
{
    //debug_msg("0x%08x\r\n",DMA_GETREG(DMA_REQ_FLG0_REG_OFS));

    DMA_SETREG(DMA_REQ_FLG0_REG_OFS, DMA_GETREG(DMA_REQ_FLG0_REG_OFS));
    DMA_SETREG(DMA_REQ_FLG1_REG_OFS, DMA_GETREG(DMA_REQ_FLG1_REG_OFS));
    DMA_SETREG(DMA_REQ_FLG2_REG_OFS, DMA_GETREG(DMA_REQ_FLG2_REG_OFS));
    DMA_SETREG(DMA_REQ_FLG3_REG_OFS, DMA_GETREG(DMA_REQ_FLG3_REG_OFS));

    //debug_msg("0x%08x\r\n",DMA_GETREG(DMA_REQ_FLG0_REG_OFS));

    DMA_SETREG(DMA_ACK_FLG0_REG_OFS, DMA_GETREG(DMA_ACK_FLG0_REG_OFS));
    DMA_SETREG(DMA_ACK_FLG1_REG_OFS, DMA_GETREG(DMA_ACK_FLG1_REG_OFS));
    DMA_SETREG(DMA_ACK_FLG2_REG_OFS, DMA_GETREG(DMA_ACK_FLG2_REG_OFS));
    DMA_SETREG(DMA_ACK_FLG3_REG_OFS, DMA_GETREG(DMA_ACK_FLG3_REG_OFS));

    DMA2_SETREG(DMA_REQ_FLG0_REG_OFS, DMA2_GETREG(DMA_REQ_FLG0_REG_OFS));
    DMA2_SETREG(DMA_REQ_FLG1_REG_OFS, DMA2_GETREG(DMA_REQ_FLG1_REG_OFS));
    DMA2_SETREG(DMA_REQ_FLG2_REG_OFS, DMA2_GETREG(DMA_REQ_FLG2_REG_OFS));
    DMA2_SETREG(DMA_REQ_FLG3_REG_OFS, DMA2_GETREG(DMA_REQ_FLG3_REG_OFS));

    DMA2_SETREG(DMA_ACK_FLG0_REG_OFS, DMA2_GETREG(DMA_ACK_FLG0_REG_OFS));
    DMA2_SETREG(DMA_ACK_FLG1_REG_OFS, DMA2_GETREG(DMA_ACK_FLG1_REG_OFS));
    DMA2_SETREG(DMA_ACK_FLG2_REG_OFS, DMA2_GETREG(DMA_ACK_FLG2_REG_OFS));
    DMA2_SETREG(DMA_ACK_FLG3_REG_OFS, DMA2_GETREG(DMA_ACK_FLG3_REG_OFS));
}


/*
    Show illigal DMA channel request

    Show illigal DMA channel request

    @return void
*/
void dma_showRequestChannel(void)
{
    UINT32 index;
    UINT32 uiChkWpSts;

    //
    // Scan DMA request flags
    // (DMA1)
    //
    uiChkWpSts = DMA_GETREG(REQUEST_FLAG_OFS(0));
    // Normal case, we don't check CPU's request and ack (Not necessary)
    // Skip 1st (CPU) channel
    index           = (DMA_WPCH_FIRST_CHANNEL + 1);
    uiChkWpSts     &= ~(1<<DMA_WPCH_FIRST_CHANNEL);
    for (index=DMA_WPCH_FIRST_CHANNEL+1; index<DMA_WPCH_COUNT; )
    {
        UINT32 i;

        if (uiChkWpSts)
        {
            i = __builtin_ctz(uiChkWpSts);
            DBG_ERR("Illigal dma channel request by %s\r\n", DmaWPEngineName[index+i-DMA_WPCH_FIRST_CHANNEL]);
            uiChkWpSts &= ~(1<<i);
        }

        if (uiChkWpSts == 0)
        {
            index += 32;
            if (index < DMA_WPCH_COUNT)
            {
                uiChkWpSts = DMA_GETREG(REQUEST_FLAG_OFS(index/32));
            }
            else
            {
                uiChkWpSts = 0;
            }
        }
    }

    //
    // Scan DMA request acks
    // (DMA1)
    //
    uiChkWpSts = DMA_GETREG(REQUEST_ACK_OFS(0));
    // Normal case, we don't check CPU's request and ack (Not necessary)
    // Skip 1st (CPU) channel
    index           = (DMA_WPCH_FIRST_CHANNEL + 1);
    uiChkWpSts     &= ~(1<<DMA_WPCH_FIRST_CHANNEL);
    for (index=DMA_WPCH_FIRST_CHANNEL+1; index<DMA_WPCH_COUNT; )
    {
        UINT32 i;

        if (uiChkWpSts)
        {
            i = __builtin_ctz(uiChkWpSts);
            DBG_WRN("dma channel ack by %s ( might disable channel after request )\r\n", DmaWPEngineName[index+i-DMA_WPCH_FIRST_CHANNEL]);
            uiChkWpSts &= ~(1<<i);
        }

        if (uiChkWpSts == 0)
        {
            index += 32;
            if (index < DMA_WPCH_COUNT)
            {
                uiChkWpSts = DMA_GETREG(REQUEST_ACK_OFS(index/32));
            }
            else
            {
                uiChkWpSts = 0;
            }
        }
    }

    //
    // Scan DMA request flags
    // (DMA2)
    //
    uiChkWpSts = DMA2_GETREG(REQUEST_FLAG_OFS(0));
    // Normal case, we don't check CPU's request and ack (Not necessary)
    // Skip 1st (CPU) channel
    index           = (DMA_WPCH_FIRST_CHANNEL + 1);
    uiChkWpSts     &= ~(1<<DMA_WPCH_FIRST_CHANNEL);
    for (index=DMA_WPCH_FIRST_CHANNEL+1; index<DMA_WPCH_COUNT; )
    {
        UINT32 i;

        if (uiChkWpSts)
        {
            i = __builtin_ctz(uiChkWpSts);
            DBG_ERR("Illigal dma2 channel request by %s\r\n", DmaWPEngineName[index+i-DMA_WPCH_FIRST_CHANNEL]);
            uiChkWpSts &= ~(1<<i);
        }

        if (uiChkWpSts == 0)
        {
            index += 32;
            if (index < DMA_WPCH_COUNT)
            {
                uiChkWpSts = DMA2_GETREG(REQUEST_FLAG_OFS(index/32));
            }
            else
            {
                uiChkWpSts = 0;
            }
        }
    }

    //
    // Scan DMA request acks
    // (DMA2)
    //
    uiChkWpSts = DMA2_GETREG(REQUEST_ACK_OFS(0));
    // Normal case, we don't check CPU's request and ack (Not necessary)
    // Skip 1st (CPU) channel
    index           = (DMA_WPCH_FIRST_CHANNEL + 1);
    uiChkWpSts     &= ~(1<<DMA_WPCH_FIRST_CHANNEL);
    for (index=DMA_WPCH_FIRST_CHANNEL+1; index<DMA_WPCH_COUNT; )
    {
        UINT32 i;

        if (uiChkWpSts)
        {
            i = __builtin_ctz(uiChkWpSts);
            DBG_WRN("dma2 channel ack by %s ( might disable channel after request )\r\n", DmaWPEngineName[index+i-DMA_WPCH_FIRST_CHANNEL]);
            uiChkWpSts &= ~(1<<i);
        }

        if (uiChkWpSts == 0)
        {
            index += 32;
            if (index < DMA_WPCH_COUNT)
            {
                uiChkWpSts = DMA2_GETREG(REQUEST_ACK_OFS(index/32));
            }
            else
            {
                uiChkWpSts = 0;
            }
        }
    }

    dma_clearRequestChannel();
}


/*
    Dump DDR phy information.

    Dump DDR phy information.

    @return void
*/
void dma_dumpPhyInfo(void)
{
    T_DMA_PHY_00_REG dmaPhy00Reg;
    T_DMA_PHY_07_REG dmaPhy07Reg;
    T_DMA_PHY_09_REG dmaPhy09Reg;
    T_DMA_PHY_38_REG dmaPhy38Reg;
    T_DMA_PHY_39_REG dmaPhy39Reg;
    T_DMA_PHY_40_REG dmaPhy40Reg;
    T_DMA_PHY_41_REG dmaPhy41Reg;
    T_DMA_PHY_42_REG dmaPhy42Reg;
    T_DMA_PHY_43_REG dmaPhy43Reg;
    T_DMA_PHY_44_REG dmaPhy44Reg;
    T_DMA_PHY_45_REG dmaPhy45Reg;
    T_DMA_PHY_46_REG dmaPhy46Reg;
    T_DMA_PHY_47_REG dmaPhy47Reg;
    T_DMA_PHY_53_REG dmaPhy53Reg;
    T_DMA_PHY_54_REG dmaPhy54Reg;
    T_DMA_PHY_57_REG dmaPhy57Reg;
    T_DMA_PHY_58_REG dmaPhy58Reg;
    T_DMA_PHY_85_REG dmaPhy85Reg;
    T_DMA_PHY_86_REG dmaPhy86Reg;
    T_DMA_PHY_AE_REG dmaPhyAEReg;
    T_DMA_PHY_AF_REG dmaPhyAFReg;
    T_DMA_PHY_B0_REG dmaPhyB0Reg;
    T_DMA_PHY_B1_REG dmaPhyB1Reg;
    T_DMA_PHY_B2_REG dmaPhyB2Reg;
    T_DMA_PHY_B3_REG dmaPhyB3Reg;
    T_DMA_PHY_B8_REG dmaPhyB8Reg;

    dmaPhy00Reg.Reg = DMA_GETREG(DMA_PHY_00_REG_OFS);
    dmaPhy07Reg.Reg = DMA_GETREG(DMA_PHY_07_REG_OFS);
    dmaPhy09Reg.Reg = DMA_GETREG(DMA_PHY_09_REG_OFS);
    dmaPhy38Reg.Reg = DMA_GETREG(DMA_PHY_38_REG_OFS);
    dmaPhy39Reg.Reg = DMA_GETREG(DMA_PHY_39_REG_OFS);
    dmaPhy40Reg.Reg = DMA_GETREG(DMA_PHY_40_REG_OFS);
    dmaPhy41Reg.Reg = DMA_GETREG(DMA_PHY_41_REG_OFS);
    dmaPhy42Reg.Reg = DMA_GETREG(DMA_PHY_42_REG_OFS);
    dmaPhy43Reg.Reg = DMA_GETREG(DMA_PHY_43_REG_OFS);
    dmaPhy44Reg.Reg = DMA_GETREG(DMA_PHY_44_REG_OFS);
    dmaPhy45Reg.Reg = DMA_GETREG(DMA_PHY_45_REG_OFS);
    dmaPhy46Reg.Reg = DMA_GETREG(DMA_PHY_46_REG_OFS);
    dmaPhy47Reg.Reg = DMA_GETREG(DMA_PHY_47_REG_OFS);
    dmaPhy53Reg.Reg = DMA_GETREG(DMA_PHY_53_REG_OFS);
    dmaPhy54Reg.Reg = DMA_GETREG(DMA_PHY_54_REG_OFS);
    dmaPhy57Reg.Reg = DMA_GETREG(DMA_PHY_57_REG_OFS);
    dmaPhy58Reg.Reg = DMA_GETREG(DMA_PHY_58_REG_OFS);
    dmaPhy85Reg.Reg = DMA_GETREG(DMA_PHY_85_REG_OFS);
    dmaPhy86Reg.Reg = DMA_GETREG(DMA_PHY_86_REG_OFS);
    dmaPhyAEReg.Reg = DMA_GETREG(DMA_PHY_AE_REG_OFS);
    dmaPhyAFReg.Reg = DMA_GETREG(DMA_PHY_AF_REG_OFS);
    dmaPhyB0Reg.Reg = DMA_GETREG(DMA_PHY_B0_REG_OFS);
    dmaPhyB1Reg.Reg = DMA_GETREG(DMA_PHY_B1_REG_OFS);
    dmaPhyB2Reg.Reg = DMA_GETREG(DMA_PHY_B2_REG_OFS);
    dmaPhyB3Reg.Reg = DMA_GETREG(DMA_PHY_B3_REG_OFS);
    dmaPhyB8Reg.Reg = DMA_GETREG(DMA_PHY_B8_REG_OFS);

    DBG_DUMP("DRAM_read_DQS0_phase = 0x%02x\r\n", dmaPhyB8Reg.Bit.REG_DQS0_90);
    DBG_DUMP("DRAM_read_DQS1_phase = 0x%02x\r\n", dmaPhyB8Reg.Bit.REG_DQS1_90);
    DBG_DUMP("DRAM_write_DQH_phase = 0x%02x\r\n", dmaPhyB2Reg.Bit.REG_WR_DQ0);
    DBG_DUMP("DRAM_write_DQL_phase = 0x%02x\r\n", dmaPhyAEReg.Bit.REG_WR_DQ1);
    DBG_DUMP("DRAM_write_DQS1_phase = 0x%02x\r\n", dmaPhyB3Reg.Bit.REG_WR_DQS0);
    DBG_DUMP("DRAM_write_DQS0_phase = 0x%02x\r\n", dmaPhyAFReg.Bit.REG_WR_DQS1);
    DBG_DUMP("DRAM_cmd_phase = 0x%02x\r\n", dmaPhyB0Reg.Bit.REG_CMD);
    DBG_DUMP("DRAM_clk_phase = 0x%02x\r\n", dmaPhyB1Reg.Bit.REG_CLK);

    DBG_DUMP("DRAM_DQ_low_byte_pull_up_driving = 0x%02x\r\n", dmaPhy42Reg.Bit.PLEG_0_LB);
    DBG_DUMP("DRAM_DQ_low_byte_pull_down_driving = 0x%02x\r\n", dmaPhy43Reg.Bit.NLEG_0_LB);
    DBG_DUMP("DRAM_DQ_high_byte_pull_up_driving = 0x%02x\r\n", dmaPhy44Reg.Bit.PLEG_0_HB);
    DBG_DUMP("DRAM_DQ_high_byte_pull_down_driving = 0x%02x\r\n", dmaPhy45Reg.Bit.NLEG_0_HB);
    DBG_DUMP("DRAM_DQS_pull_up_driving = 0x%02x\r\n", dmaPhy46Reg.Bit.PLEG_0_DQS);
    DBG_DUMP("DRAM_DQS_pull_down_driving = 0x%02x\r\n", dmaPhy47Reg.Bit.NLEG_0_DQS);

    DBG_DUMP("DRAM_CMDADDR_pull_up_driving = 0x%02x\r\n", dmaPhy38Reg.Bit.PLEG_CA);
    DBG_DUMP("DRAM_CMDADDR_pull_down_driving = 0x%02x\r\n", dmaPhy39Reg.Bit.NLEG_CA);
    DBG_DUMP("DRAM_CS#_pull_up_driving = 0x%02x\r\n", dmaPhy57Reg.Bit.PLEG_CA_MCS);
    DBG_DUMP("DRAM_CS#_pull_down_driving = 0x%02x\r\n", dmaPhy54Reg.Bit.NLEG_CA_MCS);
    DBG_DUMP("DRAM_CLK_pull_up_driving = 0x%02x\r\n", dmaPhy40Reg.Bit.PLEG_MCLK);
    DBG_DUMP("DRAM_CLK_pull_down_driving = 0x%02x\r\n", dmaPhy41Reg.Bit.NLEG_MCLK);
    DBG_DUMP("DRAM_RST_pull_up_driving = 0x%02x\r\n", dmaPhy58Reg.Bit.PLEG_CA_MRST);
    DBG_DUMP("DRAM_RST_pull_down_driving = 0x%02x\r\n", dmaPhy53Reg.Bit.NLEG_CA_MRST);

    DBG_DUMP("DRAM_CLK_Duty = 0x%02x\r\n", dmaPhy00Reg.Reg);
    DBG_DUMP("DRAM_DQS_Duty = 0x%02x\r\n", dmaPhy07Reg.Reg);
    if (dmaPhy09Reg.Bit.MCLK_POL)
    {
        DBG_DUMP("DRAM_CLK_polarity = Inverse\r\n");
    }
    else
    {
        DBG_DUMP("DRAM_CLK_polarity = Non Inverse\r\n");
    }

    if (dmaPhy85Reg.Bit.ODT_EN_CK)
    {
        DBG_DUMP("DRAM_CLK_ODT_EN = Enable\r\n");
    }
    else
    {
        DBG_DUMP("DRAM_CLK_ODT_EN = Disable\r\n");
    }
    switch (dmaPhy85Reg.Bit.ODT_MODE_CK)
    {
    case 0:
        DBG_DUMP("DRAM_CLK_ODT_VALUE = 600 Ohm\r\n");
        break;
    case 1:
        DBG_DUMP("DRAM_CLK_ODT_VALUE = 300 Ohm\r\n");
        break;
    case 2:
        DBG_DUMP("DRAM_CLK_ODT_VALUE = 200 Ohm\r\n");
        break;
    case 3:
        DBG_DUMP("DRAM_CLK_ODT_VALUE = 120 Ohm\r\n");
        break;
    case 4:
        DBG_DUMP("DRAM_CLK_ODT_VALUE = 86 Ohm\r\n");
        break;
    case 5:
        DBG_DUMP("DRAM_CLK_ODT_VALUE = 67 Ohm\r\n");
        break;
    case 6:
        DBG_DUMP("DRAM_CLK_ODT_VALUE = 55 Ohm\r\n");
        break;
    case 7:
    default:
        DBG_DUMP("DRAM_CLK_ODT_VALUE = 50 Ohm\r\n");
        break;
    }

    if (dmaPhy85Reg.Bit.ODT_EN_CMD)
    {
        DBG_DUMP("DRAM_CA_ODT_EN = Enable\r\n");
    }
    else
    {
        DBG_DUMP("DRAM_CA_ODT_EN = Disable\r\n");
    }
    switch (dmaPhy85Reg.Bit.ODT_MODE_CMD)
    {
    case 0:
        DBG_DUMP("DRAM_CA_ODT_VALUE = 600 Ohm\r\n");
        break;
    case 1:
        DBG_DUMP("DRAM_CA_ODT_VALUE = 300 Ohm\r\n");
        break;
    case 2:
        DBG_DUMP("DRAM_CA_ODT_VALUE = 200 Ohm\r\n");
        break;
    case 3:
        DBG_DUMP("DRAM_CA_ODT_VALUE = 120 Ohm\r\n");
        break;
    case 4:
        DBG_DUMP("DRAM_CA_ODT_VALUE = 86 Ohm\r\n");
        break;
    case 5:
        DBG_DUMP("DRAM_CA_ODT_VALUE = 67 Ohm\r\n");
        break;
    case 6:
        DBG_DUMP("DRAM_CA_ODT_VALUE = 55 Ohm\r\n");
        break;
    case 7:
    default:
        DBG_DUMP("DRAM_CA_ODT_VALUE = 50 Ohm\r\n");
        break;
    }

    if (dmaPhy86Reg.Bit.ODT_EN_DQS0)
    {
        DBG_DUMP("DRAM_DQS_ODT_EN = Enable\r\n");
    }
    else
    {
        DBG_DUMP("DRAM_DQS_ODT_EN = Disable\r\n");
    }
    switch (dmaPhy86Reg.Bit.ODT_MODE_DQS0)
    {
    case 0:
        DBG_DUMP("DRAM_DQS_ODT_VALUE = 600 Ohm\r\n");
        break;
    case 1:
        DBG_DUMP("DRAM_DQS_ODT_VALUE = 300 Ohm\r\n");
        break;
    case 2:
        DBG_DUMP("DRAM_DQS_ODT_VALUE = 200 Ohm\r\n");
        break;
    case 3:
        DBG_DUMP("DRAM_DQS_ODT_VALUE = 120 Ohm\r\n");
        break;
    case 4:
        DBG_DUMP("DRAM_DQS_ODT_VALUE = 86 Ohm\r\n");
        break;
    case 5:
        DBG_DUMP("DRAM_DQS_ODT_VALUE = 67 Ohm\r\n");
        break;
    case 6:
        DBG_DUMP("DRAM_DQS_ODT_VALUE = 55 Ohm\r\n");
        break;
    case 7:
    default:
        DBG_DUMP("DRAM_DQS_ODT_VALUE = 50 Ohm\r\n");
        break;
    }

    if (dmaPhy86Reg.Bit.ODT_EN_D0)
    {
        DBG_DUMP("DRAM_DQ_ODT_EN = Enable\r\n");
    }
    else
    {
        DBG_DUMP("DRAM_DQ_ODT_EN = Disable\r\n");
    }
    switch (dmaPhy86Reg.Bit.ODT_MODE_D0)
    {
    case 0:
        DBG_DUMP("DRAM_DQ_ODT_VALUE = 600 Ohm\r\n");
        break;
    case 1:
        DBG_DUMP("DRAM_DQ_ODT_VALUE = 300 Ohm\r\n");
        break;
    case 2:
        DBG_DUMP("DRAM_DQ_ODT_VALUE = 200 Ohm\r\n");
        break;
    case 3:
        DBG_DUMP("DRAM_DQ_ODT_VALUE = 120 Ohm\r\n");
        break;
    case 4:
        DBG_DUMP("DRAM_DQ_ODT_VALUE = 86 Ohm\r\n");
        break;
    case 5:
        DBG_DUMP("DRAM_DQ_ODT_VALUE = 67 Ohm\r\n");
        break;
    case 6:
        DBG_DUMP("DRAM_DQ_ODT_VALUE = 55 Ohm\r\n");
        break;
    case 7:
    default:
        DBG_DUMP("DRAM_DQ_ODT_VALUE = 50 Ohm\r\n");
        break;
    }

}

//@}

// Non-public APIs

/*
    initialize DMA monitor (sw) resources

    @return void
*/
static void dma_initMonitorResource(void)
{
    memset(vDmaMonitorObj, 0, sizeof(vDmaMonitorObj));
}

/*
    Open DMA driver

    Open DMA driver

    @return
        - @b E_OK   : success
        - @b E_SYS  : error
*/
ER dma_open(void)
{
    T_DMA_PROTECT_INTCTRL_REG uiWpIntCtrl = {0};
    T_DMA_PROTECT_INTSTS_REG ProtectIntStsReg;
    T_DMA_RECORD_PERIOD_REG periodReg = {0};
    UINT64 uiDmaFreq;

    // Arbiter clock can be PLL3 or PLL14
    uiDmaFreq = pll_getPLLFreq(PLL_ID_3);

    // assume monitor period is 100ms
    // MONITOR_PERIOD = 100ms / (1/F) = 0.1s * F = F / 10
    periodReg.Bit.MONITOR_PERIOD = uiDmaFreq / (1000/vDmaMonitorPeriod[DMA_ID_1]);

    ProtectIntStsReg.Reg = DMA_GETREG(DMA_PROTECT_INTSTS_REG_OFS);
    loc_cpu();

    DMA_SETREG(DMA_PROTECT_INTSTS_REG_OFS, ProtectIntStsReg.Reg);   // clear interrupt status

    if (bOpened == TRUE)
    {
        unl_cpu();
        DBG_ERR("Driver already opened\r\n");
        return E_SYS;
    }

#if (DRV_SUPPORT_IST == ENABLE)
    pfIstCB_DMA = dma_ist;
    pfIstCB_DMA2 = dma2_ist;
#endif

    bOpened = TRUE;
    // temp disable auto refresh timeout INTEN, observe a while
//    uiWpIntCtrl.Bit.AUTO_REFRESH_TIMEOUT_INTEN = 1;
    uiWpIntCtrl.Bit.DMA_OUTRANGE_INTEN = 1;
    uiWpIntCtrl.Bit.DMA_UPDATE_USAGE_INTEN = 1;
    dma_setReg(DMA_ID_1, DMA_PROTECT_INTCTRL_REG_OFS, uiWpIntCtrl.Reg);
    dma_setReg(DMA_ID_2, DMA_PROTECT_INTCTRL_REG_OFS, uiWpIntCtrl.Reg);
    dma_setReg(DMA_ID_1, DMA_RECORD_PERIOD_REG_OFS, periodReg.Reg);
    dma_setReg(DMA_ID_2, DMA_RECORD_PERIOD_REG_OFS, periodReg.Reg);
    unl_cpu();

    // When DMA utilization is busy, CPU DAP (ICE) may  be timeouted.
    // Increase CPU DAP DMA priority to prevent this phenomenon.
    dma_setChannelPriority(DMA_CH_CPU2, DMA_PRIORITY_HIGH);

    drv_enableInt(DRV_INT_DMA);
    if (dma_chkDramActive(DMA_ID_2))
        drv_enableInt(DRV_INT_DMA2);

    dma_initMonitorResource();

    return E_OK;
}

/*
    Close DMA driver

    Close DMA driver

    @return
        - @b E_OK   : success
        - @b E_SYS  : error
*/
ER dma_close(void)
{
    T_DMA_PROTECT_INTCTRL_REG uiWpIntCtrl = {0};

    loc_cpu();
    if (bOpened == FALSE)
    {
        unl_cpu();
        DBG_ERR("Driver already closed\r\n");
        return E_SYS;
    }

    bOpened = FALSE;
    dma_setReg(DMA_ID_1, DMA_PROTECT_INTCTRL_REG_OFS, uiWpIntCtrl.Reg);
    dma_setReg(DMA_ID_2, DMA_PROTECT_INTCTRL_REG_OFS, uiWpIntCtrl.Reg);
    unl_cpu();

    return E_OK;
}

/*
    Set dma configuration

    @param[in] id               controller identifier
    @param[in] configID         configuration identifier
    @param[in] configContext    configuration context for configID

    @return
        - @b E_OK: set configuration success
        - @b E_NOSPT: configID is not supported
*/
ER dma_setConfig(DMA_ID id, DMA_CONFIG_ID configID, UINT32 configContext)
{
    if (id >= DMA_ID_COUNT)
    {
        DBG_ERR("input id %d too large\r\n", id);
        return E_NOSPT;
    }

    switch ((UINT32)configID)
    {
    case DMA_CONFIG_ID_MONITOR_PERIOD:
#if 0
        if (configContext > DMA_MAX_MONITOR_TIME)
        {
            DBG_WRN("Max period is %d ms, but input %d ms, shrink it to %d ms\r\n",
                DMA_MAX_MONITOR_TIME, configContext, DMA_MAX_MONITOR_TIME);
            configContext = DMA_MAX_MONITOR_TIME;
        }
#endif

        vDmaMonitorPeriod[id] = configContext;
        {
            T_DMA_RECORD_PERIOD_REG periodReg = {0};
            UINT64 uiDmaFreq;

            // ARB clock can be PLL3 or PLL14
            uiDmaFreq = pll_getPLLFreq(PLL_ID_3);
            DBG_ERR("arb freq %d\r\n", uiDmaFreq);

            // assume monitor period is 100ms
            // MONITOR_PERIOD = 100ms / (1/F) = 0.1s * F = F / 10
            periodReg.Bit.MONITOR_PERIOD = uiDmaFreq / (1000/vDmaMonitorPeriod[id]);

            dma_setReg(id, DMA_RECORD_PERIOD_REG_OFS, periodReg.Reg);
        }
        break;

    default:
        return E_NOSPT;
    }

    return E_OK;
}

/*
    Set DMA call back routine

    This function provides a facility for upper layer to install callback routine.

    @param[in] id           controller identifier
    @param[in] callBackID   callback routine identifier
    @param[in] pCallBack    callback routine to be installed

    @return
        - @b E_OK: install callback success
        - @b E_NOSPT: callBackID is not valid
*/
ER dma_setCallBack(DMA_ID id, DMA_CALLBACK callBackID, DMA_GENERIC_CB pCallBack)
{
    if (id >= DMA_ID_COUNT)
    {
        DBG_ERR("input id %d too large\r\n", id);
        return E_NOSPT;
    }

    switch (callBackID)
    {
    case DMA_CALLBACK_MONITOR_UPDATE:
    {
        loc_cpu();

        vDmaMonitorCB[id] = pCallBack;

        unl_cpu();
    }
    break;
    default:
    {
        return E_NOSPT;
    }
    }

    return E_OK;
}

/*
    Set priority of DMA channel.

    Set DMA priority of specific channel.


    @param[in] id           DMA controller ID
    @param[in] Channel      DMA channel
    @param[in] Priority     DMA priority
        - @b DMA_PRIORITY_LOW       : Low priority (Default)
        - @b DMA_PRIORITY_MIDDLE    : Middle priority
        - @b DMA_PRIORITY_HIGH      : High priority
        - @b DMA_PRIORITY_SUPER_HIGH: Spuer High priority

    @return Set priority status
        - @b E_OK   : Set priority success
        - @b E_PAR  : Channel or priority is not correct
*/
ER dma_setCtrlPriority(DMA_ID id, DMA_CH Channel, DMA_PRIORITY Priority)
{
    UINT32   uiPriMask;
    UINT32   uiPriorityOffset;
    UINT32   uiPriorityBitOffset;
    REGVALUE uiPriorityReg;

    if(((Channel <= DMA_CH_RSV) || (Channel >= DMA_CH_COUNT)) ||
       (Priority > DMA_PRIORITY_SUPER_HIGH))
    {
        DBG_ERR("Invalid channel %d, priority 0x%lx\r\n", Channel, Priority);
        return E_PAR;
    }

    // Only SIE/IDE channel allowed to be super high
    // gcc case range: https://gcc.gnu.org/onlinedocs/gcc-4.5.4/gcc/Case-Ranges.html
    switch (Channel)
    {
    case DMA_CH_SIE_0 ... DMA_CH_SIE8_2:
        break;
    case DMA_CH_IDE_a0 ... DMA_CH_IDE2_7:
        break;
    default:
        if (Priority == DMA_PRIORITY_SUPER_HIGH)
        {
            Priority = DMA_PRIORITY_HIGH;
            DBG_WRN("Set to super high but not SIE channel, force to high\r\n");
        }
        break;
    }

    uiPriorityOffset = DMA_PRIOFITY_OFS(Channel / 16);
    uiPriorityBitOffset = (Channel%16) * 2;

    uiPriMask = DMA_PRI_BIT_MASK << uiPriorityBitOffset;

    loc_cpu();  // enter critical section

    uiPriorityReg = dma_getReg(id, uiPriorityOffset);
    uiPriorityReg &= ~uiPriMask;
    uiPriorityReg |= (Priority << uiPriorityBitOffset);
    dma_setReg(id, uiPriorityOffset, uiPriorityReg);

    unl_cpu();  // exit critical section

    return E_OK;
}

/*
    Get priority of DMA channel of specific DMA controller.

    Get DMA priority of specific channel.

    @param[in] id           DMA controller ID
    @param[in] Channel      DMA channel
    @param[out] pPriority   Returned DMA priority, could be one of the following
        - @b DMA_PRIORITY_LOW       : Low priority (Default)
        - @b DMA_PRIORITY_MIDDLE    : Middle priority
        - @b DMA_PRIORITY_HIGH      : High priority
        - @b DMA_PRIORITY_SUPER_HIGH: Spuer High priority

    @return Get priority status
        - @b E_OK   : Get priority success
        - @b E_PAR  : Channel is not correct or pPriority is NULL
*/
ER dma_getCtrlPriority(DMA_ID id, DMA_CH Channel, PDMA_PRIORITY pPriority)
{
    UINT32      uiPriMask;
    UINT32      uiPriorityOffset;
    UINT32      uiPriorityBitOffset;
    REGVALUE    uiPriorityReg;

    if ((Channel >= DMA_CH_COUNT) || (Channel <= DMA_CH_RSV) || (pPriority == NULL))
    {
        return E_PAR;
    }

    uiPriorityOffset = DMA_PRIOFITY_OFS(Channel / 16);
    uiPriorityBitOffset = (Channel%16) * 2;

    uiPriorityReg = dma_getReg(id, uiPriorityOffset);
    uiPriMask = DMA_PRI_BIT_MASK << uiPriorityBitOffset;

    *pPriority = ((uiPriorityReg & uiPriMask) >> uiPriorityBitOffset);

    return E_OK;
}


/*
    Set priority of DMA channel.

    Set DMA priority of specific channel on DMA_ID_1 and DMA_ID_2.


    @param[in] Channel      DMA channel
    @param[in] Priority     DMA priority
        - @b DMA_PRIORITY_LOW       : Low priority (Default)
        - @b DMA_PRIORITY_MIDDLE    : Middle priority
        - @b DMA_PRIORITY_HIGH      : High priority
        - @b DMA_PRIORITY_SUPER_HIGH: Spuer High priority
    @return Set priority status
        - @b E_OK   : Set priority success
        - @b E_PAR  : Channel or priority is not correct
*/
ER dma_setChannelPriority(DMA_CH Channel, DMA_PRIORITY Priority)
{
    UINT32   uiPriMask;
    UINT32   uiPriorityOffset;
    UINT32   uiPriorityBitOffset;
    REGVALUE uiPriorityReg;

    if(((Channel <= DMA_CH_RSV) || (Channel >= DMA_CH_COUNT)) ||
       (Priority > DMA_PRIORITY_SUPER_HIGH))
    {
        DBG_ERR("Invalid channel %d, priority 0x%lx\r\n", Channel, Priority);
        return E_PAR;
    }

    // Only SIE/IDE channel allowed to be super high
    // gcc case range: https://gcc.gnu.org/onlinedocs/gcc-4.5.4/gcc/Case-Ranges.html
    switch (Channel)
    {
    case DMA_CH_SIE_0 ... DMA_CH_SIE8_2:
        break;
    case DMA_CH_IDE_a0 ... DMA_CH_IDE2_7:
        break;
    default:
        if (Priority == DMA_PRIORITY_SUPER_HIGH)
        {
            Priority = DMA_PRIORITY_HIGH;
            DBG_WRN("Set to super high but not SIE channel, force to high\r\n");
        }
        break;
    }

    uiPriorityOffset = DMA_PRIOFITY_OFS(Channel / 16);
    uiPriorityBitOffset = (Channel%16) * 2;

    uiPriMask = DMA_PRI_BIT_MASK << uiPriorityBitOffset;

    loc_cpu();  // enter critical section

    uiPriorityReg = dma_getReg(DMA_ID_1, uiPriorityOffset);
    uiPriorityReg &= ~uiPriMask;
    uiPriorityReg |= (Priority << uiPriorityBitOffset);
    dma_setReg(DMA_ID_1, uiPriorityOffset, uiPriorityReg);

    uiPriorityReg = dma_getReg(DMA_ID_2, uiPriorityOffset);
    uiPriorityReg &= ~uiPriMask;
    uiPriorityReg |= (Priority << uiPriorityBitOffset);
    dma_setReg(DMA_ID_2, uiPriorityOffset, uiPriorityReg);

    unl_cpu();  // exit critical section

    return E_OK;
}

/*
    Get priority of DMA channel.

    Get DMA priority of specific channel.
    This API will also check consistency between DMA_ID_1 and DMA_ID_2


    @param[in] Channel      DMA channel
    @param[out] pPriority   Returned DMA priority, could be one of the following
        - @b DMA_PRIORITY_LOW       : Low priority (Default)
        - @b DMA_PRIORITY_MIDDLE    : Middle priority
        - @b DMA_PRIORITY_HIGH      : High priority
        - @b DMA_PRIORITY_SUPER_HIGH: Spuer High priority

    @return Get priority status
        - @b E_OK   : Get priority success
        - @b E_PAR  : Channel is not correct or pPriority is NULL
        - @b E_OBJ  : DMA priority in DMA_ID_1 and DMA_ID_2 is not the same, plz use dma_setChPriority() instead
*/
ER dma_getChannelPriority(DMA_CH Channel, PDMA_PRIORITY pPriority)
{
    UINT32      uiPriMask;
    UINT32      uiPriorityOffset;
    UINT32      uiPriorityBitOffset;
    UINT32      uiDma1Pri, uiDma2Pri;
    REGVALUE    uiPriorityReg;

    if ((Channel >= DMA_CH_COUNT) || (Channel <= DMA_CH_RSV) || (pPriority == NULL))
    {
        return E_PAR;
    }

    uiPriorityOffset = DMA_PRIOFITY_OFS(Channel / 16);
    uiPriorityBitOffset = (Channel%16) * 2;

    uiPriorityReg = dma_getReg(DMA_ID_1, uiPriorityOffset);
    uiPriMask = DMA_PRI_BIT_MASK << uiPriorityBitOffset;

    uiDma1Pri = ((uiPriorityReg & uiPriMask) >> uiPriorityBitOffset);

    uiPriorityReg = dma_getReg(DMA_ID_2, uiPriorityOffset);
    uiPriMask = DMA_PRI_BIT_MASK << uiPriorityBitOffset;

    uiDma2Pri = ((uiPriorityReg & uiPriMask) >> uiPriorityBitOffset);

    if (uiDma1Pri != uiDma2Pri) return E_OBJ;

    *pPriority = uiDma1Pri;

    return E_OK;
}
#endif
