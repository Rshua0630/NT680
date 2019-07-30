/*
    Header file for DMA module register

    This file is the header file that define register for DMA module.

    @file       dma_reg.h
    @ingroup    mIDrvDMA_DMA
    @note       Nothing.

    Copyright   Novatek Microelectronics Corp. 2016.  All rights reserved.
*/

#ifndef _DMAREG_H
#define _DMAREG_H

//#include "DrvCommon.h"
#include <asm/arch/IOAddress.h>


#define DMA_REG_ADDR(ofs)       (IOADDR_DRAM_REG_BASE+(ofs))
#define DMA2_REG_ADDR(ofs)      (IOADDR_DRAM2_REG_BASE+(ofs))
#define DMA_GETREG(ofs)         INW(DMA_REG_ADDR(ofs))
#define DMA_SETREG(ofs,value)   OUTW(DMA_REG_ADDR(ofs), (value))
#define DMA2_GETREG(ofs)        INW(DMA2_REG_ADDR(ofs))
#define DMA2_SETREG(ofs,value)  OUTW(DMA2_REG_ADDR(ofs), (value))

#define DMA_PRI_BIT_MASK        (0x03)

//
// 0x00 ~ 0xFFC: DDR TOP region
//
#define DMA_CONFIG_REG_OFS      0x00
REGDEF_BEGIN(DMA_CONFIG_REG)        /* declare register cache type "DMA_CONFIG_REG" begin */
    REGDEF_BIT(SDRAM_CAPACITY, 3)
    REGDEF_BIT(SDRAM_COUNT, 1)
    REGDEF_BIT(CL, 4)               /* CAS latency */
    REGDEF_BIT(DDR_TYPE, 1)
    REGDEF_BIT(reserved2,3)
    REGDEF_BIT(WCL, 4)
    REGDEF_BIT(ADDR_EXTEND, 1)      /* 2T command */
    REGDEF_BIT(reserved4,15)
REGDEF_END(DMA_CONFIG_REG)          /* declare register cache type "DMA_CONFIG_REG" end */

#define DMA_CONTROL_REG_OFS     0x04
REGDEF_BEGIN(DMA_CONTROL_REG)       /* declare register cache type "DMA_CONTROL_REG" begin */
    REGDEF_BIT(INIT_EN, 1)
    REGDEF_BIT(PRECHARGE_ALL_CMD, 1)
    REGDEF_BIT(MRS_EMRS_CMD, 1)
    REGDEF_BIT(AUTO_REFRESH_CMD, 1)
    REGDEF_BIT(SELF_REFRESH_CMD, 1)
    REGDEF_BIT(AUTO_REFRESH_CTRL, 1)
    REGDEF_BIT(reserved, 1)
    REGDEF_BIT(SELF_REFRESH_STS, 1)
    REGDEF_BIT(INIT_NOP_RDY, 1)
    REGDEF_BIT(DDR_DLL_LOCK_RDY, 1)
    REGDEF_BIT(PHY_DLL_RDY, 1)
    REGDEF_BIT(reserved2, 1)
    REGDEF_BIT(MODE_REG, 19)
    REGDEF_BIT(reserved3, 1)
REGDEF_END(DMA_CONTROL_REG)         /* declare register cache type "DMA_CONTROL_REG" end */

#define DMA_TIMING0_REG_OFS     0x08
REGDEF_BEGIN(DMA_TIMING0_REG)       /* declare register cache type "DMA_TIMING0_REG" begin */
    REGDEF_BIT(T_RCD, 4)
    REGDEF_BIT(reserve1, 4)
    REGDEF_BIT(T_RRD, 4)
    REGDEF_BIT(T_RP, 5)
//    REGDEF_BIT(T_RTP, 3)
    REGDEF_BIT(reserve2, 3)
    REGDEF_BIT(T_WR, 5)
//    REGDEF_BIT(T_WTR, 3)
    REGDEF_BIT(reserve3, 3)
    REGDEF_BIT(DDR_TXP_EXT, 3)
    REGDEF_BIT(reserve4, 1)
REGDEF_END(DMA_TIMING0_REG)         /* declare register cache type "DMA_TIMING0_REG" end */

#define DMA_TIMING1_REG_OFS     0x0C
REGDEF_BEGIN(DMA_TIMING1_REG)       /* declare register cache type "DMA_TIMING1_REG" begin */
    REGDEF_BIT(T_RFC, 8)
    REGDEF_BIT(reserve1, 8)
    REGDEF_BIT(T_REFI, 13)
    REGDEF_BIT(reserve2, 3)
REGDEF_END(DMA_TIMING1_REG)         /* declare register cache type "DMA_TIMING1_REG" end */

#define DMA_TIMING2_REG_OFS     0x10
REGDEF_BEGIN(DMA_TIMING2_REG)       /* declare register cache type "DMA_TIMING2_REG" begin */
    REGDEF_BIT(T_RC, 6)
    REGDEF_BIT(reserve1, 2)
    REGDEF_BIT(T_FAW, 6)
    REGDEF_BIT(reserve2, 2)
    REGDEF_BIT(T_RAS, 6)
    REGDEF_BIT(reserve3, 2)
    REGDEF_BIT(T_DLLK, 2)
    REGDEF_BIT(reserve4, 6)
REGDEF_END(DMA_TIMING2_REG)         /* declare register cache type "DMA_TIMING2_REG" end */

#define DMA_TIMING3_REG_OFS     0x14
REGDEF_BEGIN(DMA_TIMING3_REG)       /* declare register cache type "DMA_TIMING3_REG" begin */
    REGDEF_BIT(T_RTP, 4)
    REGDEF_BIT(reserve1, 4)
    REGDEF_BIT(T_WTR, 4)
    REGDEF_BIT(reserve2, 20)
REGDEF_END(DMA_TIMING3_REG)         /* declare register cache type "DMA_TIMING3_REG" end */


// 0x18 ~ 0x1C: Reserved

#define DMA_CONTROL1_REG_OFS 0x20
REGDEF_BEGIN(DMA_CONTROL1_REG)      /* declare register cache type "DMA_CONTROL1_REG" begin */
    REGDEF_BIT(ZQ_CALI_CMD, 1)
    REGDEF_BIT(ZQ_CALI_MODE, 1)
    REGDEF_BIT(reserve1, 2)
    REGDEF_BIT(MRESET, 1)
    REGDEF_BIT(WRL_ACT, 1)
    REGDEF_BIT(WRL_DONE, 1)
    REGDEF_BIT(reserve2, 25)
REGDEF_END(DMA_CONTROL1_REG)        /* declare register cache type "DMA_CONTROL1_REG" end */

#define DMA_ODT_CTRL_REG_OFS 0x24
REGDEF_BEGIN(DMA_ODT_CTRL_REG)      /* declare register cache type "DMA_ODT_CTRL_REG" begin */
    REGDEF_BIT(DRAM_ODT_SEL, 2)
    REGDEF_BIT(reserve1, 2)
    REGDEF_BIT(PHY_CMD_ODT_SEL, 1)
    REGDEF_BIT(reserve2, 3)
    REGDEF_BIT(PHY_DATA_ODT_SEL, 2)
    REGDEF_BIT(reserve3, 2)
    REGDEF_BIT(PHY_ODT_SWITCH_DELAY, 2)
    REGDEF_BIT(reserve4, 18)
REGDEF_END(DMA_ODT_CTRL_REG)        /* declare register cache type "DMA_ODT_CTRL_REG" end */

// 0x28 ~ 0x2C: Reserved

#define DMA_DDRIO_TEST0_REG_OFS     0x30
REGDEF_BEGIN(DMA_DDRIO_TEST0_REG)   /* declare register cache type "DMA_DDRIO_TEST_REG" begin */
    REGDEF_BIT(DDR_CMD_DC_TEST, 1)
    REGDEF_BIT(DDR_DATA_DC_TEST, 1)
    REGDEF_BIT(DDR_DC_CKE_OUTPUT_VALUE, 1)
    REGDEF_BIT(DDR_DC_RST_OUTPUT_VALUE, 1)
    REGDEF_BIT(DDR_DOUT, 1)
    REGDEF_BIT(DDR_PHY_TEST_EN, 1)
    REGDEF_BIT(DDR_DC_CS0_OUTPUT_VALUE, 1)
    REGDEF_BIT(DDR_DC_CS1_OUTPUT_VALUE, 1)
    REGDEF_BIT(DDR_PHY_LOAD_DUTY, 1)
    REGDEF_BIT(reserve, 3)
    REGDEF_BIT(DDR_PHY_DUTY_PULSE_WIDTH, 4)
    REGDEF_BIT(reserve2, 16)
REGDEF_END(DMA_DDRIO_TEST0_REG)     /* declare register cache type "DMA_DDRIO_TEST_REG" end */

// 0x34 ~ 0x3C: Reserved

#define DMA_TIMING4_REG_OFS             0x40
REGDEF_BEGIN(DMA_TIMING4_REG)       /* declare register cache type "DMA_TIMING4_REG" begin */
    REGDEF_BIT(reserve1, 20)
    REGDEF_BIT(W2R_EXT, 3)
    REGDEF_BIT(reserved2, 1)
    REGDEF_BIT(R2W_EXT, 3)
    REGDEF_BIT(reserve3, 1)
    REGDEF_BIT(DDR_R2R_EXT_1T_2_2T, 1)
    REGDEF_BIT(reserve4, 3)
REGDEF_END(DMA_TIMING4_REG)     /* declare register cache type "DMA_TIMING4_REG" end */

// 0x44 ~ 0x4C: Reserved

#define DMA_ENGINEER_RESERVE_REG_OFS    0x50
REGDEF_BEGIN(DMA_ENGINEER_RESERVE_REG)  /* declare register cache type "DMA_ENGINEER_RESERVE_REG" begin */
    REGDEF_BIT(PRECHARGE_PD, 1)
    REGDEF_BIT(reserve1, 2)
    REGDEF_BIT(DQS_IN_CYCLE, 1)
    REGDEF_BIT(reserve2, 4)
    REGDEF_BIT(DQS_INEN_DELAY, 2)
    REGDEF_BIT(reserve3, 6)
    REGDEF_BIT(BANK_SWAP_EN, 1)
    REGDEF_BIT(reserve4, 1)
    REGDEF_BIT(PU_CMD, 1)
    REGDEF_BIT(PU_DATA1, 1)
    REGDEF_BIT(reserve5, 8)
    REGDEF_BIT(DEBUG_SEL, 4)
REGDEF_END(DMA_ENGINEER_RESERVE_REG)    /* declare register cache type "DMA_ENGINEER_RESERVE_REG" end */

#define DMA_BANDWIDTH_REG_OFS   0x54
REGDEF_BEGIN(DMA_BANDWIDTH_REG)         /* declare register cache type "DMA_BANDWIDTH_REG" begin */
    REGDEF_BIT(ACT_CYCLE, 26)
    REGDEF_BIT(reserve, 6)
REGDEF_END(DMA_BANDWIDTH_REG)           /* declare register cache type "DMA_BANDWIDTH_REG" end */

#define DMA_BANDWIDTH_SIZE_REG_OFS  0x58
REGDEF_BEGIN(DMA_BANDWIDTH_SIZE_REG)    /* declare register cache type "DMA_BANDWIDTH_SIZE_REG" begin */
    REGDEF_BIT(TOTAL_ACC_SIZE, 28)
    REGDEF_BIT(reserve, 3)
    REGDEF_BIT(TOTAL_ACC_OVF, 1)
REGDEF_END(DMA_BANDWIDTH_SIZE_REG)      /* declare register cache type "DMA_BANDWIDTH_SIZE_REG" end */

#define DMA_BANDWIDTH_REQ_REG_OFS   0x5C
REGDEF_BEGIN(DMA_BANDWIDTH_REQ_REG)     /* declare register cache type "DMA_BANDWIDTH_REQ_REG" begin */
    REGDEF_BIT(TOTAL_ACC_REQ, 28)
    REGDEF_BIT(reserve, 3)
    REGDEF_BIT(TOTAL_REQ_OVF, 1)
REGDEF_END(DMA_BANDWIDTH_REQ_REG)       /* declare register cache type "DMA_BANDWIDTH_REQ_REG" end */

#define DMA_PROTECT_CTRL_REG_OFS   0x60
REGDEF_BEGIN(DMA_PROTECT_CTRL_REG)    /* declare register cache type "DMA_PROTECT_CTRL_REG" begin */
    REGDEF_BIT(DMA_WPWD0_EN, 1)
    REGDEF_BIT(DMA_WPWD1_EN, 1)
    REGDEF_BIT(DMA_WPWD2_EN, 1)
    REGDEF_BIT(            , 13)
    REGDEF_BIT(DMA_WPWD0_SEL, 1)
    REGDEF_BIT(DMA_WPWD1_SEL, 1)
    REGDEF_BIT(DMA_WPWD2_SEL, 1)
    REGDEF_BIT(             , 13)
REGDEF_END(DMA_PROTECT_CTRL_REG)      /* declare register cache type "DMA_PROTECT_CTRL_REG" end */

#define DMA_PROTECT_INTCTRL_REG_OFS   0x64
REGDEF_BEGIN(DMA_PROTECT_INTCTRL_REG)    /* declare register cache type "DMA_PROTECT_INTCTRL_REG_OFS" begin */
    REGDEF_BIT(DMA_WPWD0_INTEN, 1)
    REGDEF_BIT(DMA_WPWD1_INTEN, 1)
    REGDEF_BIT(DMA_WPWD2_INTEN, 1)
    REGDEF_BIT(               , 5)
    REGDEF_BIT(AUTO_REFRESH_TIMEOUT_INTEN, 1)
    REGDEF_BIT(DMA_OUTRANGE_INTEN, 1)
    REGDEF_BIT(DMA_UPDATE_USAGE_INTEN, 1)
    REGDEF_BIT(               , 21)
REGDEF_END(DMA_PROTECT_INTCTRL_REG)      /* declare register cache type "DMA_PROTECT_INTCTRL_REG_OFS" end */

#define DMA_PROTECT_INTSTS_REG_OFS   0x68
REGDEF_BEGIN(DMA_PROTECT_INTSTS_REG)    /* declare register cache type "DMA_PROTECT_INTSTS_REG_OFS" begin */
    REGDEF_BIT(DMA_WPWD0_STS, 1)
    REGDEF_BIT(DMA_WPWD1_STS, 1)
    REGDEF_BIT(DMA_WPWD2_STS, 1)
    REGDEF_BIT(               , 5)
    REGDEF_BIT(AUTO_REFRESH_TIMEOUT_STS, 1)
    REGDEF_BIT(DMA_OUTRANGE_STS, 1)
    REGDEF_BIT(DMA_UPDATE_USAGE_STS, 1)
    REGDEF_BIT(             , 21)
REGDEF_END(DMA_PROTECT_INTSTS_REG)      /* declare register cache type "DMA_PROTECT_INTSTS_REG_OFS" end */

#define DMA_PROTECT_CHSTS_REG_OFS   0x6C
REGDEF_BEGIN(DMA_PROTECT_CHSTS_REG)    /* declare register cache type "DMA_PROTECT_CHSTS_REG" begin */
    REGDEF_BIT(CHSTS0    , 8)
    REGDEF_BIT(CHSTS1    , 8)
    REGDEF_BIT(CHSTS2    , 8)
    REGDEF_BIT(          , 8)
REGDEF_END(DMA_PROTECT_CHSTS_REG)      /* declare register cache type "DMA_PROTECT_CHSTS_REG" end */

#define DMA_PROTECT_STARTADDR0_REG_OFS  0x70
REGDEF_BEGIN(DMA_PROTECT_STARTADDR0_REG)    /* declare register cache type "DMA_PROTECT_STAADR0_REG" begin */
    REGDEF_BIT(STA_ADDR    , 30)
    REGDEF_BIT(            , 2)
REGDEF_END(DMA_PROTECT_STARTADDR0_REG)      /* declare register cache type "DMA_PROTECT_STAADR0_REG" end */

#define DMA_PROTECT_STOPADDR0_REG_OFS  0x74
REGDEF_BEGIN(DMA_PROTECT_STOPADDR0_REG)    /* declare register cache type "DMA_PROTECT_STAADR0_REG" begin */
    REGDEF_BIT(STP_ADDR    , 30)
    REGDEF_BIT(            , 2)
REGDEF_END(DMA_PROTECT_STOPADDR0_REG)      /* declare register cache type "DMA_PROTECT_STAADR0_REG" end */


#define DMA_PROTECT_STARTADDR1_REG_OFS  0x78
REGDEF_BEGIN(DMA_PROTECT_STARTADDR1_REG)    /* declare register cache type "DMA_PROTECT_STAADR0_REG" begin */
    REGDEF_BIT(STA_ADDR    , 30)
    REGDEF_BIT(            , 2)
REGDEF_END(DMA_PROTECT_STARTADDR1_REG)      /* declare register cache type "DMA_PROTECT_STAADR0_REG" end */

#define DMA_PROTECT_STOPADDR1_REG_OFS  0x7C
REGDEF_BEGIN(DMA_PROTECT_STOPADDR1_REG)    /* declare register cache type "DMA_PROTECT_STAADR0_REG" begin */
    REGDEF_BIT(STP_ADDR    , 30)
    REGDEF_BIT(            , 2)
REGDEF_END(DMA_PROTECT_STOPADDR1_REG)      /* declare register cache type "DMA_PROTECT_STAADR0_REG" end */

#define DMA_PROTECT_STARTADDR2_REG_OFS  0x80
REGDEF_BEGIN(DMA_PROTECT_STARTADDR2_REG)    /* declare register cache type "DMA_PROTECT_STAADR0_REG" begin */
    REGDEF_BIT(STA_ADDR             ,30)
    REGDEF_BIT(                     , 2)
REGDEF_END(DMA_PROTECT_STARTADDR2_REG)      /* declare register cache type "DMA_PROTECT_STAADR0_REG" end */

#define DMA_PROTECT_STOPADDR2_REG_OFS   0x84
REGDEF_BEGIN(DMA_PROTECT_STOPADDR2_REG)     /* declare register cache type "DMA_PROTECT_STAADR0_REG" begin */
    REGDEF_BIT(STP_ADDR             ,30)
    REGDEF_BIT(                     , 2)
REGDEF_END(DMA_PROTECT_STOPADDR2_REG)       /* declare register cache type "DMA_PROTECT_STAADR0_REG" end */

// 0x88 ~ 0x8C: Reserved

#define DMA_PROTECT_RANGE0_MSK0_REG_OFS 0x90
REGDEF_BEGIN(DMA_PROTECT_RANGE0_MSK0_REG)   /* declare register cache type "DMA_PROTECT_RANGE0_MSK0_REG" begin */
//    REGDEF_BIT(MIPS1_EN,1)
//    REGDEF_BIT(MIPS2_EN,1)
//    REGDEF_BIT(DSP1_EN,1)
//    REGDEF_BIT(DSP2_EN,1)
    REGDEF_BIT(                     , 32)
REGDEF_END(DMA_PROTECT_RANGE0_MSK0_REG)     /* declare register cache type "DMA_PROTECT_RANGE0_MSK0_REG" end */

#define DMA_PROTECT_RANGE0_MSK1_REG_OFS 0x94
REGDEF_BEGIN(DMA_PROTECT_RANGE0_MSK1_REG)   /* declare register cache type "DMA_PROTECT_RANGE0_MSK1_REG" begin */
    REGDEF_BIT(                     , 32)
REGDEF_END(DMA_PROTECT_RANGE0_MSK1_REG)     /* declare register cache type "DMA_PROTECT_RANGE0_MSK1_REG" end */

#define DMA_PROTECT_RANGE0_MSK2_REG_OFS 0x98
REGDEF_BEGIN(DMA_PROTECT_RANGE0_MSK2_REG)   /* declare register cache type "DMA_PROTECT_RANGE0_MSK2_REG" begin */
    REGDEF_BIT(                     , 32)
REGDEF_END(DMA_PROTECT_RANGE0_MSK2_REG)     /* declare register cache type "DMA_PROTECT_RANGE0_MSK2_REG" end */

#define DMA_PROTECT_RANGE0_MSK3_REG_OFS 0x9C
REGDEF_BEGIN(DMA_PROTECT_RANGE0_MSK3_REG)   /* declare register cache type "DMA_PROTECT_RANGE0_MSK3_REG" begin */
    REGDEF_BIT(                     , 32)
REGDEF_END(DMA_PROTECT_RANGE0_MSK3_REG)     /* declare register cache type "DMA_PROTECT_RANGE0_MSK3_REG" end */

#define DMA_PROTECT_RANGE0_MSK4_REG_OFS 0xA0
REGDEF_BEGIN(DMA_PROTECT_RANGE0_MSK4_REG)   /* declare register cache type "DMA_PROTECT_RANGE0_MSK4_REG" begin */
    REGDEF_BIT(                     , 32)
REGDEF_END(DMA_PROTECT_RANGE0_MSK4_REG)     /* declare register cache type "DMA_PROTECT_RANGE0_MSK4_REG" end */

#define DMA_PROTECT_RANGE0_MSK5_REG_OFS 0xA4
REGDEF_BEGIN(DMA_PROTECT_RANGE0_MSK5_REG)   /* declare register cache type "DMA_PROTECT_RANGE0_MSK5_REG" begin */
    REGDEF_BIT(                     , 32)
REGDEF_END(DMA_PROTECT_RANGE0_MSK5_REG)     /* declare register cache type "DMA_PROTECT_RANGE0_MSK5_REG" end */

#define DMA_PROTECT_RANGE0_MSK6_REG_OFS 0xA8
REGDEF_BEGIN(DMA_PROTECT_RANGE0_MSK6_REG)   /* declare register cache type "DMA_PROTECT_RANGE0_MSK6_REG" begin */
    REGDEF_BIT(                     , 32)
REGDEF_END(DMA_PROTECT_RANGE0_MSK6_REG)     /* declare register cache type "DMA_PROTECT_RANGE0_MSK6_REG" end */

#define DMA_PROTECT_RANGE0_MSK7_REG_OFS 0xAC
REGDEF_BEGIN(DMA_PROTECT_RANGE0_MSK7_REG)   /* declare register cache type "DMA_PROTECT_RANGE0_MSK7_REG" begin */
    REGDEF_BIT(                     , 32)
REGDEF_END(DMA_PROTECT_RANGE0_MSK7_REG)     /* declare register cache type "DMA_PROTECT_RANGE0_MSK7_REG" end */

#define DMA_PROTECT_RANGE1_MSK0_REG_OFS 0xB0
REGDEF_BEGIN(DMA_PROTECT_RANGE1_MSK0_REG)   /* declare register cache type "DMA_PROTECT_RANGE1_MSK0_REG" begin */
    REGDEF_BIT(                     , 32)
REGDEF_END(DMA_PROTECT_RANGE1_MSK0_REG)     /* declare register cache type "DMA_PROTECT_RANGE1_MSK0_REG" end */

#define DMA_PROTECT_RANGE1_MSK1_REG_OFS 0xB4
REGDEF_BEGIN(DMA_PROTECT_RANGE1_MSK1_REG)   /* declare register cache type "DMA_PROTECT_RANGE1_MSK1_REG" begin */
    REGDEF_BIT(                     , 32)
REGDEF_END(DMA_PROTECT_RANGE1_MSK1_REG)     /* declare register cache type "DMA_PROTECT_RANGE1_MSK1_REG" end */

#define DMA_PROTECT_RANGE1_MSK2_REG_OFS 0xB8
REGDEF_BEGIN(DMA_PROTECT_RANGE1_MSK2_REG)   /* declare register cache type "DMA_PROTECT_RANGE1_MSK2_REG" begin */
    REGDEF_BIT(                     , 32)
REGDEF_END(DMA_PROTECT_RANGE1_MSK2_REG)     /* declare register cache type "DMA_PROTECT_RANGE1_MSK2_REG" end */

#define DMA_PROTECT_RANGE1_MSK3_REG_OFS 0xBC
REGDEF_BEGIN(DMA_PROTECT_RANGE1_MSK3_REG)   /* declare register cache type "DMA_PROTECT_RANGE1_MSK3_REG" begin */
    REGDEF_BIT(                     , 32)
REGDEF_END(DMA_PROTECT_RANGE1_MSK3_REG)     /* declare register cache type "DMA_PROTECT_RANGE1_MSK3_REG" end */

#define DMA_PROTECT_RANGE1_MSK4_REG_OFS 0xC0
REGDEF_BEGIN(DMA_PROTECT_RANGE1_MSK4_REG)   /* declare register cache type "DMA_PROTECT_RANGE1_MSK4_REG" begin */
    REGDEF_BIT(                     , 32)
REGDEF_END(DMA_PROTECT_RANGE1_MSK4_REG)     /* declare register cache type "DMA_PROTECT_RANGE1_MSK4_REG" end */

#define DMA_PROTECT_RANGE1_MSK5_REG_OFS 0xC4
REGDEF_BEGIN(DMA_PROTECT_RANGE1_MSK5_REG)   /* declare register cache type "DMA_PROTECT_RANGE1_MSK5_REG" begin */
    REGDEF_BIT(                     , 32)
REGDEF_END(DMA_PROTECT_RANGE1_MSK5_REG)     /* declare register cache type "DMA_PROTECT_RANGE1_MSK5_REG" end */

#define DMA_PROTECT_RANGE1_MSK6_REG_OFS 0xC8
REGDEF_BEGIN(DMA_PROTECT_RANGE1_MSK6_REG)   /* declare register cache type "DMA_PROTECT_RANGE1_MSK6_REG" begin */
    REGDEF_BIT(                     , 32)
REGDEF_END(DMA_PROTECT_RANGE1_MSK6_REG)     /* declare register cache type "DMA_PROTECT_RANGE1_MSK6_REG" end */

#define DMA_PROTECT_RANGE1_MSK7_REG_OFS 0xCC
REGDEF_BEGIN(DMA_PROTECT_RANGE1_MSK7_REG)   /* declare register cache type "DMA_PROTECT_RANGE1_MSK7_REG" begin */
    REGDEF_BIT(                     , 32)
REGDEF_END(DMA_PROTECT_RANGE1_MSK7_REG)     /* declare register cache type "DMA_PROTECT_RANGE1_MSK7_REG" end */

#define DMA_PROTECT_RANGE2_MSK0_REG_OFS 0xD0
REGDEF_BEGIN(DMA_PROTECT_RANGE2_MSK0_REG)   /* declare register cache type "DMA_PROTECT_RANGE2_MSK0_REG" begin */
    REGDEF_BIT(                     , 32)
REGDEF_END(DMA_PROTECT_RANGE2_MSK0_REG)     /* declare register cache type "DMA_PROTECT_RANGE2_MSK0_REG" end */

#define DMA_PROTECT_RANGE2_MSK1_REG_OFS 0xD4
REGDEF_BEGIN(DMA_PROTECT_RANGE2_MSK1_REG)   /* declare register cache type "DMA_PROTECT_RANGE2_MSK1_REG" begin */
    REGDEF_BIT(                     , 32)
REGDEF_END(DMA_PROTECT_RANGE2_MSK1_REG)     /* declare register cache type "DMA_PROTECT_RANGE2_MSK1_REG" end */

#define DMA_PROTECT_RANGE2_MSK2_REG_OFS 0xD8
REGDEF_BEGIN(DMA_PROTECT_RANGE2_MSK2_REG)   /* declare register cache type "DMA_PROTECT_RANGE2_MSK2_REG" begin */
    REGDEF_BIT(                     , 32)
REGDEF_END(DMA_PROTECT_RANGE2_MSK2_REG)     /* declare register cache type "DMA_PROTECT_RANGE2_MSK2_REG" end */

#define DMA_PROTECT_RANGE2_MSK3_REG_OFS 0xDC
REGDEF_BEGIN(DMA_PROTECT_RANGE2_MSK3_REG)   /* declare register cache type "DMA_PROTECT_RANGE2_MSK3_REG" begin */
    REGDEF_BIT(                     , 32)
REGDEF_END(DMA_PROTECT_RANGE2_MSK3_REG)     /* declare register cache type "DMA_PROTECT_RANGE2_MSK3_REG" end */

#define DMA_PROTECT_RANGE2_MSK4_REG_OFS 0xE0
REGDEF_BEGIN(DMA_PROTECT_RANGE2_MSK4_REG)   /* declare register cache type "DMA_PROTECT_RANGE2_MSK4_REG" begin */
    REGDEF_BIT(                     , 32)
REGDEF_END(DMA_PROTECT_RANGE2_MSK4_REG)     /* declare register cache type "DMA_PROTECT_RANGE2_MSK4_REG" end */

#define DMA_PROTECT_RANGE2_MSK5_REG_OFS 0xE4
REGDEF_BEGIN(DMA_PROTECT_RANGE2_MSK5_REG)   /* declare register cache type "DMA_PROTECT_RANGE2_MSK5_REG" begin */
    REGDEF_BIT(                     , 32)
REGDEF_END(DMA_PROTECT_RANGE2_MSK5_REG)     /* declare register cache type "DMA_PROTECT_RANGE2_MSK5_REG" end */

#define DMA_PROTECT_RANGE2_MSK6_REG_OFS 0xE8
REGDEF_BEGIN(DMA_PROTECT_RANGE2_MSK6_REG)   /* declare register cache type "DMA_PROTECT_RANGE2_MSK6_REG" begin */
    REGDEF_BIT(                     , 32)
REGDEF_END(DMA_PROTECT_RANGE2_MSK6_REG)     /* declare register cache type "DMA_PROTECT_RANGE2_MSK6_REG" end */

#define DMA_PROTECT_RANGE2_MSK7_REG_OFS 0xEC
REGDEF_BEGIN(DMA_PROTECT_RANGE2_MSK7_REG)   /* declare register cache type "DMA_PROTECT_RANGE2_MSK7_REG" begin */
    REGDEF_BIT(                     , 32)
REGDEF_END(DMA_PROTECT_RANGE2_MSK7_REG)     /* declare register cache type "DMA_PROTECT_RANGE2_MSK7_REG" end */

#define DMA_PROTECT_DETECT_ADDR0_REG_OFS    0xF0
REGDEF_BEGIN(DMA_PROTECT_DETECT_ADDR0_REG)  /* declare register cache type "DMA_PROTECT_DETECT_ADDR0_REG" begin */
    REGDEF_BIT(addr                 , 32)
REGDEF_END(DMA_PROTECT_DETECT_ADDR0_REG)    /* declare register cache type "DMA_PROTECT_DETECT_ADDR0_REG" end */

#define DMA_PROTECT_DETECT_ADDR1_REG_OFS    0xF4
REGDEF_BEGIN(DMA_PROTECT_DETECT_ADDR1_REG)  /* declare register cache type "DMA_PROTECT_DETECT_ADDR1_REG" begin */
    REGDEF_BIT(addr                 , 32)
REGDEF_END(DMA_PROTECT_DETECT_ADDR1_REG)    /* declare register cache type "DMA_PROTECT_DETECT_ADDR1_REG" end */

#define DMA_PROTECT_DETECT_ADDR2_REG_OFS    0xF8
REGDEF_BEGIN(DMA_PROTECT_DETECT_ADDR2_REG)  /* declare register cache type "DMA_PROTECT_DETECT_ADDR2_REG" begin */
    REGDEF_BIT(addr                 , 32)
REGDEF_END(DMA_PROTECT_DETECT_ADDR2_REG)    /* declare register cache type "DMA_PROTECT_DETECT_ADDR2_REG" end */

#define DMA_OUT_RANGE_CHANNEL_REG_OFS   0x100
REGDEF_BEGIN(DMA_OUT_RANGE_CHANNEL_REG)     /* declare register cache type "DMA_OUT_RANGE_CHANNEL_REG" begin */
    REGDEF_BIT(DRAM_OUTRANGE_CH     , 8)
    REGDEF_BIT(                     , 24)
REGDEF_END(DMA_OUT_RANGE_CHANNEL_REG)       /* declare register cache type "DMA_OUT_RANGE_CHANNEL_REG" end */

#define DMA_OUT_RANGE_ADDR_REG_OFS      0x104
REGDEF_BEGIN(DMA_OUT_RANGE_ADDR_REG)        /* declare register cache type "DMA_OUT_RANGE_ADDR_REG" begin */
    REGDEF_BIT(DRAM_OUTRANGE_ADDR   , 32)
REGDEF_END(DMA_OUT_RANGE_ADDR_REG)          /* declare register cache type "DMA_OUT_RANGE_ADDR_REG" end */

// 0x1000 ~ 0x7FFC: Reserve for DDR PHY (by IP team)

//
// 0x8000 ~ 0xFFFC: DDR Arbiter region
//
#define DMA_SDRAM_CTRL_REG0_OFS  0x8000
REGDEF_BEGIN(DMA_SDRAM_CTRL_REG0)  /* declare register cache type "DMA_SDRAM_CTRL_REG0" begin */
    REGDEF_BIT(DMA_CH_EN_SWITCH , 1)
    REGDEF_BIT(DMA_CH_DIS_DONE  , 1)
    REGDEF_BIT(                 , 2)
    REGDEF_BIT(AXI_CH0_REQ_NUM  , 2)
    REGDEF_BIT(                 , 2)
    REGDEF_BIT(AXI_CH1_REQ_NUM  , 2)
    REGDEF_BIT(                 , 2)
    REGDEF_BIT(AXI_CH2_REQ_NUM  , 2)
    REGDEF_BIT(                 , 2)
    REGDEF_BIT(AXI_CH3_REQ_NUM  , 2)
    REGDEF_BIT(                 , 2)
    REGDEF_BIT(AXI_CH4_REQ_NUM  , 2)
    REGDEF_BIT(                 , 2)
    REGDEF_BIT(AXI_CH5_REQ_NUM  , 2)
    REGDEF_BIT(                 , 2)
    REGDEF_BIT(AXI_CH6_REQ_NUM  , 2)
    REGDEF_BIT(                 , 2)
REGDEF_END(DMA_SDRAM_CTRL_REG0)    /* declare register cache type "DMA_SDRAM_CTRL_REG0" end */

#define DMA_SDRAM_CTRL_REG1_OFS  0x8004
REGDEF_BEGIN(DMA_SDRAM_CTRL_REG1)  /* declare register cache type "DMA_SDRAM_CTRL_REG1" begin */
    REGDEF_BIT(AXI_CH7_REQ_NUM  , 2)
    REGDEF_BIT(                 , 30)
REGDEF_END(DMA_SDRAM_CTRL_REG1)    /* declare register cache type "DMA_SDRAM_CTRL_REG1" end */

// 0x8004 ~ 0x800C: Reserved

#define ARBIT_OFFSET            0x8000
#define DMA_CHANNEL_ENABLE0_REG_OFS  (0x10 + ARBIT_OFFSET)
REGDEF_BEGIN(DMA_CHANNEL_ENABLE0_REG)  /* declare register cache type "DMA_CHANNEL_ENABLE0_REG" begin */
    REGDEF_BIT(                 , 32)
REGDEF_END(DMA_CHANNEL_ENABLE0_REG)    /* declare register cache type "DMA_CHANNEL_ENABLE0_REG" end */

#define DMA_CHANNEL_ENABLE1_REG_OFS    (0x14 + ARBIT_OFFSET)
REGDEF_BEGIN(DMA_CHANNEL_ENABLE1_REG)  /* declare register cache type "DMA_CHANNEL_ENABLE1_REG" begin */
    REGDEF_BIT(                 , 32)
REGDEF_END(DMA_CHANNEL_ENABLE1_REG)    /* declare register cache type "DMA_CHANNEL_ENABLE1_REG" end */

#define DMA_CHANNEL_ENABLE2_REG_OFS    (0x18 + ARBIT_OFFSET)
REGDEF_BEGIN(DMA_CHANNEL_ENABLE2_REG)  /* declare register cache type "DMA_CHANNEL_ENABLE2_REG" begin */
    REGDEF_BIT(                 , 32)
REGDEF_END(DMA_CHANNEL_ENABLE2_REG)    /* declare register cache type "DMA_CHANNEL_ENABLE2_REG" end */

#define DMA_CHANNEL_ENABLE3_REG_OFS    (0x1C + ARBIT_OFFSET)
REGDEF_BEGIN(DMA_CHANNEL_ENABLE3_REG)  /* declare register cache type "DMA_CHANNEL_ENABLE3_REG" begin */
    REGDEF_BIT(reserved             ,32)
REGDEF_END(DMA_CHANNEL_ENABLE3_REG)    /* declare register cache type "DMA_CHANNEL_ENABLE3_REG" end */

#define DMA_CHANNEL_ENABLE4_REG_OFS    (0x20 + ARBIT_OFFSET)
REGDEF_BEGIN(DMA_CHANNEL_ENABLE4_REG)  /* declare register cache type "DMA_CHANNEL_ENABLE4_REG" begin */
    REGDEF_BIT(reserved             ,32)
REGDEF_END(DMA_CHANNEL_ENABLE4_REG)    /* declare register cache type "DMA_CHANNEL_ENABLE4_REG" end */

#define DMA_CHANNEL_ENABLE5_REG_OFS    (0x24 + ARBIT_OFFSET)
REGDEF_BEGIN(DMA_CHANNEL_ENABLE5_REG)  /* declare register cache type "DMA_CHANNEL_ENABLE5_REG" begin */
    REGDEF_BIT(reserved             ,32)
REGDEF_END(DMA_CHANNEL_ENABLE5_REG)    /* declare register cache type "DMA_CHANNEL_ENABLE5_REG" end */

#define DMA_CHANNEL_ENABLE6_REG_OFS    (0x28 + ARBIT_OFFSET)
REGDEF_BEGIN(DMA_CHANNEL_ENABLE6_REG)  /* declare register cache type "DMA_CHANNEL_ENABLE6_REG" begin */
    REGDEF_BIT(reserved             ,32)
REGDEF_END(DMA_CHANNEL_ENABLE6_REG)    /* declare register cache type "DMA_CHANNEL_ENABLE6_REG" end */

#define DMA_CHANNEL_ENABLE7_REG_OFS    (0x2C + ARBIT_OFFSET)
REGDEF_BEGIN(DMA_CHANNEL_ENABLE7_REG)  /* declare register cache type "DMA_CHANNEL_ENABLE7_REG" begin */
    REGDEF_BIT(reserved             ,32)
REGDEF_END(DMA_CHANNEL_ENABLE7_REG)    /* declare register cache type "DMA_CHANNEL_ENABLE7_REG" end */

#define DMA_REQ_FLG0_REG_OFS        (0x30 + ARBIT_OFFSET)
#define DMA_REQ_FLG1_REG_OFS        (0x34 + ARBIT_OFFSET)
#define DMA_REQ_FLG2_REG_OFS        (0x38 + ARBIT_OFFSET)
#define DMA_REQ_FLG3_REG_OFS        (0x3C + ARBIT_OFFSET)
#define DMA_REQ_FLG4_REG_OFS        (0x40 + ARBIT_OFFSET)
#define DMA_REQ_FLG5_REG_OFS        (0x44 + ARBIT_OFFSET)
#define DMA_REQ_FLG6_REG_OFS        (0x48 + ARBIT_OFFSET)
#define DMA_REQ_FLG7_REG_OFS        (0x4C + ARBIT_OFFSET)

#define DMA_ACK_FLG0_REG_OFS        (0xB0 + ARBIT_OFFSET)
#define DMA_ACK_FLG1_REG_OFS        (0xB4 + ARBIT_OFFSET)
#define DMA_ACK_FLG2_REG_OFS        (0xB8 + ARBIT_OFFSET)
#define DMA_ACK_FLG3_REG_OFS        (0xBC + ARBIT_OFFSET)
#define DMA_ACK_FLG4_REG_OFS        (0xC0 + ARBIT_OFFSET)
#define DMA_ACK_FLG5_REG_OFS        (0xC4 + ARBIT_OFFSET)
#define DMA_ACK_FLG6_REG_OFS        (0xC8 + ARBIT_OFFSET)
#define DMA_ACK_FLG7_REG_OFS        (0xCC + ARBIT_OFFSET)

#define DMA_CHANNEL0_HEAVY_LOAD_CTRL_OFS    (0x60 + ARBIT_OFFSET)
REGDEF_BEGIN(DMA_CHANNEL0_HEAVY_LOAD_CTRL_REG)  /* declare register cache type "DMA_CHANNEL0_HEAVY_LOAD_CTRL_REG" begin */
    REGDEF_BIT(CH0_TEST_START   , 1)
    REGDEF_BIT(RESERVED0        , 1)
    REGDEF_BIT(CH0_SKIP_COMPARE , 1)
    REGDEF_BIT(RESERVED1        , 1)
    REGDEF_BIT(CH0_TEST_METHOD  , 2)
    REGDEF_BIT(RESERVED2        , 2)
    REGDEF_BIT(CH0_BURST_LEN    , 7)
    REGDEF_BIT(RESERVED3        , 1)
    REGDEF_BIT(CH0_TEST_TIMES   ,16)
REGDEF_END(DMA_CHANNEL0_HEAVY_LOAD_CTRL_REG)    /* declare register cache type "DMA_CHANNEL0_HEAVY_LOAD_CTRL_REG" end */

#define DMA_CHANNEL0_HEAVY_LOAD_START_ADDR_OFS      (0x64 + ARBIT_OFFSET)
#define DMA_CHANNEL0_HEAVY_LOAD_DMA_SIZE_OFS        (0x68 + ARBIT_OFFSET)

#define DMA_CHANNEL0_HEAVY_LOAD_WAIT_CYCLE_OFS      (0x6C + ARBIT_OFFSET)
REGDEF_BEGIN(DMA_CHANNEL0_HEAVY_LOAD_WAIT_CYCLE)    /* declare register cache type "DMA_CHANNEL0_HEAVY_LOAD_WAIT_CYCLE" begin */
    REGDEF_BIT(INTERVAL_REQ, 10)
    REGDEF_BIT(RESERVED0        , 6)
    REGDEF_BIT(CHECKSUM, 16)
REGDEF_END(DMA_CHANNEL0_HEAVY_LOAD_WAIT_CYCLE)      /* declare register cache type "DMA_CHANNEL0_HEAVY_LOAD_WAIT_CYCLE" end */



#define DMA_CHANNEL1_HEAVY_LOAD_CTRL_OFS    (0x70 + ARBIT_OFFSET)
REGDEF_BEGIN(DMA_CHANNEL1_HEAVY_LOAD_CTRL_REG)  /* declare register cache type "DMA_CHANNEL0_HEAVY_LOAD_CTRL_REG" begin */
    REGDEF_BIT(CH1_TEST_START   , 1)
    REGDEF_BIT(RESERVED0        , 1)
    REGDEF_BIT(CH1_SKIP_COMPARE , 1)
    REGDEF_BIT(RESERVED1        , 1)
    REGDEF_BIT(CH1_TEST_METHOD  , 2)
    REGDEF_BIT(RESERVED2        , 2)
    REGDEF_BIT(CH1_BURST_LEN    , 7)
    REGDEF_BIT(RESERVED3        , 1)
    REGDEF_BIT(CH1_TEST_TIMES   ,16)
REGDEF_END(DMA_CHANNEL1_HEAVY_LOAD_CTRL_REG)    /* declare register cache type "DMA_CHANNEL0_HEAVY_LOAD_CTRL_REG" end */


#define DMA_CHANNEL1_HEAVY_LOAD_START_ADDR_OFS      (0x74 + ARBIT_OFFSET)
#define DMA_CHANNEL1_HEAVY_LOAD_DMA_SIZE_OFS        (0x78 + ARBIT_OFFSET)

#define DMA_CHANNEL1_HEAVY_LOAD_WAIT_CYCLE_OFS      (0x7C + ARBIT_OFFSET)
REGDEF_BEGIN(DMA_CHANNEL1_HEAVY_LOAD_WAIT_CYCLE)    /* declare register cache type "DMA_CHANNEL1_HEAVY_LOAD_WAIT_CYCLE" begin */
    REGDEF_BIT(INTERVAL_REQ, 10)
    REGDEF_BIT(RESERVED0        , 6)
    REGDEF_BIT(CHECKSUM, 16)
REGDEF_END(DMA_CHANNEL1_HEAVY_LOAD_WAIT_CYCLE)      /* declare register cache type "DMA_CHANNEL1_HEAVY_LOAD_WAIT_CYCLE" end */

#define DMA_CHANNEL2_HEAVY_LOAD_CTRL_OFS            (0x80 + ARBIT_OFFSET)
REGDEF_BEGIN(DMA_CHANNEL2_HEAVY_LOAD_CTRL_REG)  /* declare register cache type "DMA_CHANNEL0_HEAVY_LOAD_CTRL_REG" begin */
    REGDEF_BIT(CH2_TEST_START   , 1)
    REGDEF_BIT(RESERVED0        , 1)
    REGDEF_BIT(CH2_SKIP_COMPARE , 1)
    REGDEF_BIT(RESERVED1        , 1)
    REGDEF_BIT(CH2_TEST_METHOD  , 2)
    REGDEF_BIT(RESERVED2        , 2)
    REGDEF_BIT(CH2_BURST_LEN    , 7)
    REGDEF_BIT(RESERVED3        , 1)
    REGDEF_BIT(CH2_TEST_TIMES   ,16)
REGDEF_END(DMA_CHANNEL2_HEAVY_LOAD_CTRL_REG)    /* declare register cache type "DMA_CHANNEL0_HEAVY_LOAD_CTRL_REG" end */

#define DMA_CHANNEL2_HEAVY_LOAD_START_ADDR_OFS      (0x84 + ARBIT_OFFSET)
#define DMA_CHANNEL2_HEAVY_LOAD_DMA_SIZE_OFS        (0x88 + ARBIT_OFFSET)

#define DMA_CHANNEL2_HEAVY_LOAD_WAIT_CYCLE_OFS      (0x8C + ARBIT_OFFSET)
REGDEF_BEGIN(DMA_CHANNEL2_HEAVY_LOAD_WAIT_CYCLE)    /* declare register cache type "DMA_CHANNEL2_HEAVY_LOAD_WAIT_CYCLE" begin */
    REGDEF_BIT(INTERVAL_REQ, 10)
    REGDEF_BIT(RESERVED0        , 6)
    REGDEF_BIT(CHECKSUM, 16)
REGDEF_END(DMA_CHANNEL2_HEAVY_LOAD_WAIT_CYCLE)      /* declare register cache type "DMA_CHANNEL2_HEAVY_LOAD_WAIT_CYCLE" end */

#define DMA_HEAVY_LOAD_TEST_PATTERN0_OFS            (0x90 + ARBIT_OFFSET)
#define DMA_HEAVY_LOAD_TEST_PATTERN1_OFS            (0x94 + ARBIT_OFFSET)
#define DMA_HEAVY_LOAD_TEST_PATTERN2_OFS            (0x98 + ARBIT_OFFSET)
#define DMA_HEAVY_LOAD_TEST_PATTERN3_OFS            (0x9C + ARBIT_OFFSET)

#define DMA_HEAVY_LOAD_STS_OFS    (0xA0 + ARBIT_OFFSET)
REGDEF_BEGIN(DMA_HEAVY_LOAD_STS_REG)  /* declare register cache type "DMA_HEAVY_LOAD_STS_REG" begin */
    REGDEF_BIT(HEAVY_LOAD_COMPLETE_STS  , 1)
    REGDEF_BIT(HEAVY_LOAD_ERROR_STS     , 1)
    REGDEF_BIT(Reserved                 ,30)
REGDEF_END(DMA_HEAVY_LOAD_STS_REG)    /* declare register cache type "DMA_HEAVY_LOAD_STS_REG" end */

#define DMA_HEAVY_LOAD_ERROR_DATA_OFS               (0xA4 + ARBIT_OFFSET)
#define DMA_HEAVY_LOAD_CORRECT_DATA_OFS             (0xA8 + ARBIT_OFFSET)
#define DMA_HEAVY_LOAD_CURRENT_ADDR_OFS             (0xAC + ARBIT_OFFSET)

#define DMA_PRI_CONTROL0_REG_OFS    (0x100 + ARBIT_OFFSET)
REGDEF_BEGIN(DMA_PRI_CONTROL0_REG)  /* declare register cache type "DMA_PRI_CONTROL0_REG" begin */
    REGDEF_BIT(                 , 32)
REGDEF_END(DMA_PRI_CONTROL0_REG)    /* declare register cache type "DMA_PRI_CONTROL0_REG" end */

#define DMA_PRI_CONTROL1_REG_OFS    (0x104 + ARBIT_OFFSET)
REGDEF_BEGIN(DMA_PRI_CONTROL1_REG)  /* declare register cache type "DMA_PRI_CONTROL1_REG" begin */
    REGDEF_BIT(                 , 32)
REGDEF_END(DMA_PRI_CONTROL1_REG)    /* declare register cache type "DMA_PRI_CONTROL1_REG" end */

#define DMA_PRI_CONTROL2_REG_OFS    (0x108 + ARBIT_OFFSET)
REGDEF_BEGIN(DMA_PRI_CONTROL2_REG)  /* declare register cache type "DMA_PRI_CONTROL2_REG" begin */
    REGDEF_BIT(                 , 32)
REGDEF_END(DMA_PRI_CONTROL2_REG)    /* declare register cache type "DMA_PRI_CONTROL2_REG" end */

#define DMA_PRI_CONTROL3_REG_OFS    (0x10C + ARBIT_OFFSET)
REGDEF_BEGIN(DMA_PRI_CONTROL3_REG)  /* declare register cache type "DMA_PRI_CONTROL3_REG" begin */
    REGDEF_BIT(                 , 32)
REGDEF_END(DMA_PRI_CONTROL3_REG)    /* declare register cache type "DMA_PRI_CONTROL3_REG" end */

#define DMA_PRI_CONTROL4_REG_OFS    (0x110 + ARBIT_OFFSET)
REGDEF_BEGIN(DMA_PRI_CONTROL4_REG)  /* declare register cache type "DMA_PRI_CONTROL4_REG" begin */
    REGDEF_BIT(                 , 32)
REGDEF_END(DMA_PRI_CONTROL4_REG)    /* declare register cache type "DMA_PRI_CONTROL4_REG" end */

#define DMA_PRI_CONTROL5_REG_OFS    (0x114 + ARBIT_OFFSET)
REGDEF_BEGIN(DMA_PRI_CONTROL5_REG)  /* declare register cache type "DMA_PRI_CONTROL5_REG" begin */
    REGDEF_BIT(                 , 32)
REGDEF_END(DMA_PRI_CONTROL5_REG)    /* declare register cache type "DMA_PRI_CONTROL5_REG" end */

#define DMA_PRI_CONTROL6_REG_OFS    (0x118 + ARBIT_OFFSET)
REGDEF_BEGIN(DMA_PRI_CONTROL6_REG)  /* declare register cache type "DMA_PRI_CONTROL6_REG" begin */
    REGDEF_BIT(reserve1         ,32)
REGDEF_END(DMA_PRI_CONTROL6_REG)    /* declare register cache type "DMA_PRI_CONTROL6_REG" end */

#define DMA_PRI_CONTROL7_REG_OFS    (0x11C + ARBIT_OFFSET)
REGDEF_BEGIN(DMA_PRI_CONTROL7_REG)  /* declare register cache type "DMA_PRI_CONTROL7_REG" begin */
    REGDEF_BIT(reserve1         ,32)
REGDEF_END(DMA_PRI_CONTROL7_REG)    /* declare register cache type "DMA_PRI_CONTROL7_REG" end */

#define DMA_PRI_CONTROL8_REG_OFS    (0x120 + ARBIT_OFFSET)
REGDEF_BEGIN(DMA_PRI_CONTROL8_REG)  /* declare register cache type "DMA_PRI_CONTROL8_REG" begin */
    REGDEF_BIT(                 , 32)
REGDEF_END(DMA_PRI_CONTROL8_REG)    /* declare register cache type "DMA_PRI_CONTROL8_REG" end */

#define DMA_PRI_CONTROL9_REG_OFS    (0x124 + ARBIT_OFFSET)
REGDEF_BEGIN(DMA_PRI_CONTROL9_REG)  /* declare register cache type "DMA_PRI_CONTROL9_REG" begin */
    REGDEF_BIT(                 , 32)
REGDEF_END(DMA_PRI_CONTROL9_REG)    /* declare register cache type "DMA_PRI_CONTROL9_REG" end */

#define DMA_PRI_CONTROL10_REG_OFS   (0x128 + ARBIT_OFFSET)
REGDEF_BEGIN(DMA_PRI_CONTROL10_REG) /* declare register cache type "DMA_PRI_CONTROL10_REG" begin */
    REGDEF_BIT(                 , 32)
REGDEF_END(DMA_PRI_CONTROL10_REG)   /* declare register cache type "DMA_PRI_CONTROL10_REG" end */

#define DMA_PRI_CONTROL11_REG_OFS   (0x12C + ARBIT_OFFSET)
REGDEF_BEGIN(DMA_PRI_CONTROL11_REG) /* declare register cache type "DMA_PRI_CONTROL11_REG" begin */
    REGDEF_BIT(                 , 32)
REGDEF_END(DMA_PRI_CONTROL11_REG)   /* declare register cache type "DMA_PRI_CONTROL11_REG" end */

#define DMA_PRI_CONTROL12_REG_OFS   (0x130 + ARBIT_OFFSET)
REGDEF_BEGIN(DMA_PRI_CONTROL12_REG) /* declare register cache type "DMA_PRI_CONTROL12_REG" begin */
    REGDEF_BIT(                 , 32)
REGDEF_END(DMA_PRI_CONTROL12_REG)   /* declare register cache type "DMA_PRI_CONTROL12_REG" end */

#define DMA_PRI_CONTROL13_REG_OFS   (0x134 + ARBIT_OFFSET)
REGDEF_BEGIN(DMA_PRI_CONTROL13_REG) /* declare register cache type "DMA_PRI_CONTROL13_REG" begin */
    REGDEF_BIT(                 , 32)
REGDEF_END(DMA_PRI_CONTROL13_REG)   /* declare register cache type "DMA_PRI_CONTROL13_REG" end */

#define DMA_PRI_CONTROL14_REG_OFS   (0x138 + ARBIT_OFFSET)
REGDEF_BEGIN(DMA_PRI_CONTROL14_REG) /* declare register cache type "DMA_PRI_CONTROL14_REG" begin */
    REGDEF_BIT(reserve1         ,32)
REGDEF_END(DMA_PRI_CONTROL14_REG)   /* declare register cache type "DMA_PRI_CONTROL14_REG" end */

#define DMA_PRI_CONTROL15_REG_OFS   (0x13C + ARBIT_OFFSET)
REGDEF_BEGIN(DMA_PRI_CONTROL15_REG) /* declare register cache type "DMA_PRI_CONTROL15_REG" begin */
    REGDEF_BIT(reserve1         ,32)
REGDEF_END(DMA_PRI_CONTROL15_REG)   /* declare register cache type "DMA_PRI_CONTROL15_REG" end */


#define DMA_BOUNDING_CTRL_REG_OFS   (0x200 + ARBIT_OFFSET)
REGDEF_BEGIN(DMA_BOUNDING_CTRL_REG) /* declare register cache type "DMA_BOUNDING_CTRL_REG" begin */
    REGDEF_BIT(DATA_BOUND_TEST_EN,  1)
    REGDEF_BIT(ADDR_BOUND_TEST_EN,  1)
    REGDEF_BIT(reserve1          ,  30)
REGDEF_END(DMA_BOUNDING_CTRL_REG)   /* declare register cache type "DMA_BOUNDING_CTRL_REG" end */

#define DMA_BOUNDING_CFG_REG_OFS    (0x204 + ARBIT_OFFSET)
REGDEF_BEGIN(DMA_BOUNDING_CFG_REG)  /* declare register cache type "DMA_BOUNDING_CFG_REG" begin */
    REGDEF_BIT(ADDR_BOUND_TEST_SEL, 1)
    REGDEF_BIT(reserve1          ,  31)
REGDEF_END(DMA_BOUNDING_CFG_REG)    /* declare register cache type "DMA_BOUNDING_CFG_REG" end */

#define DMA_BOUNDING_STS_REG_OFS    (0x208 + ARBIT_OFFSET)
REGDEF_BEGIN(DMA_BOUNDING_STS_REG)  /* declare register cache type "DMA_BOUNDING_STS_REG" begin */
    REGDEF_BIT(DATA_BOUND_COMPLETE_STS,     1)
    REGDEF_BIT(ADDR_BOUND_COMPLETE_STS,     1)
    REGDEF_BIT(reserve1,                    14)
    REGDEF_BIT(DATA_BOUND_COMPLETE_RESULT,  1)
    REGDEF_BIT(ADDR_BOUND_COMPLETE_RESULT,  1)
    REGDEF_BIT(reserve2,                    14)
REGDEF_END(DMA_BOUNDING_STS_REG)    /* declare register cache type "DMA_BOUNDING_STS_REG" end */

#define DMA_BOUNDING_REPORT_REG_OFS (0x20C + ARBIT_OFFSET)
REGDEF_BEGIN(DMA_BOUNDING_REPORT_REG)   /* declare register cache type "DMA_BOUNDING_REPORT_REG" begin */
    REGDEF_BIT(DATA_BOUND_FAIL_PAD,     4)
    REGDEF_BIT(reserve1,                4)
    REGDEF_BIT(ADDR_BOUND_FAIL_PAD,     4)
    REGDEF_BIT(reserve2,                20)
REGDEF_END(DMA_BOUNDING_REPORT_REG)     /* declare register cache type "DMA_BOUNDING_REPORT_REG" end */

#define MONITOR_OFFSET              0x9000

#define DMA_RECORD_PERIOD_REG_OFS       (0x00 + MONITOR_OFFSET)
REGDEF_BEGIN(DMA_RECORD_PERIOD_REG)     /* declare register cache type "DMA_RECORD_PERIOD_REG" begin */
    REGDEF_BIT(MONITOR_PERIOD,          26)
    REGDEF_BIT(reserved ,               6)
REGDEF_END(DMA_RECORD_PERIOD_REG)       /* declare register cache type "DMA_RECORD_PERIOD_REG" end */

#define DMA_RECORD_CONFIG0_REG_OFS      (0x10 + MONITOR_OFFSET)
REGDEF_BEGIN(DMA_RECORD_CONFIG0_REG)    /* declare register cache type "DMA_RECORD_CONFIG0_REG" begin */
    REGDEF_BIT(MONITOR0_DMA_CH,         8)
    REGDEF_BIT(MONITOR0_DMA_DIR,        2)
    REGDEF_BIT(reserved1 ,              6)
    REGDEF_BIT(MONITOR1_DMA_CH,         8)
    REGDEF_BIT(MONITOR1_DMA_DIR,        2)
    REGDEF_BIT(reserved2 ,              6)
REGDEF_END(DMA_RECORD_CONFIG0_REG)      /* declare register cache type "DMA_RECORD_CONFIG0_REG" end */

#define DMA_RECORD_CONFIG1_REG_OFS      (0x14 + MONITOR_OFFSET)
REGDEF_BEGIN(DMA_RECORD_CONFIG1_REG)    /* declare register cache type "DMA_RECORD_CONFIG1_REG" begin */
    REGDEF_BIT(MONITOR2_DMA_CH,         8)
    REGDEF_BIT(MONITOR2_DMA_DIR,        2)
    REGDEF_BIT(reserved1 ,              6)
    REGDEF_BIT(MONITOR3_DMA_CH,         8)
    REGDEF_BIT(MONITOR3_DMA_DIR,        2)
    REGDEF_BIT(reserved2 ,              6)
REGDEF_END(DMA_RECORD_CONFIG1_REG)      /* declare register cache type "DMA_RECORD_CONFIG1_REG" end */

#define DMA_RECORD_CONFIG2_REG_OFS      (0x18 + MONITOR_OFFSET)
REGDEF_BEGIN(DMA_RECORD_CONFIG2_REG)    /* declare register cache type "DMA_RECORD_CONFIG2_REG" begin */
    REGDEF_BIT(MONITOR4_DMA_CH,         8)
    REGDEF_BIT(MONITOR4_DMA_DIR,        2)
    REGDEF_BIT(reserved1 ,              6)
    REGDEF_BIT(MONITOR5_DMA_CH,         8)
    REGDEF_BIT(MONITOR5_DMA_DIR,        2)
    REGDEF_BIT(reserved2 ,              6)
REGDEF_END(DMA_RECORD_CONFIG2_REG)      /* declare register cache type "DMA_RECORD_CONFIG2_REG" end */

#define DMA_RECORD_CONFIG3_REG_OFS      (0x1C + MONITOR_OFFSET)
REGDEF_BEGIN(DMA_RECORD_CONFIG3_REG)    /* declare register cache type "DMA_RECORD_CONFIG3_REG" begin */
    REGDEF_BIT(MONITOR6_DMA_CH,         8)
    REGDEF_BIT(MONITOR6_DMA_DIR,        2)
    REGDEF_BIT(reserved1 ,              6)
    REGDEF_BIT(MONITOR7_DMA_CH,         8)
    REGDEF_BIT(MONITOR7_DMA_DIR,        2)
    REGDEF_BIT(reserved2 ,              6)
REGDEF_END(DMA_RECORD_CONFIG3_REG)      /* declare register cache type "DMA_RECORD_CONFIG3_REG" end */

#define DMA_RECORD0_SIZE_REG_OFS        (0x30 + MONITOR_OFFSET)
REGDEF_BEGIN(DMA_RECORD0_SIZE_REG)      /* declare register cache type "DMA_RECORD0_SIZE_REG" begin */
    REGDEF_BIT(MONITOR_ACC_SIZE,        28)
    REGDEF_BIT(reserved1 ,              3)
    REGDEF_BIT(MONITOR_SIZE_OVF,        1)
REGDEF_END(DMA_RECORD0_SIZE_REG)        /* declare register cache type "DMA_RECORD0_SIZE_REG" end */

#define DMA_RECORD0_COUNT_REG_OFS       (0x34 + MONITOR_OFFSET)
REGDEF_BEGIN(DMA_RECORD0_COUNT_REG)     /* declare register cache type "DMA_RECORD0_COUNT_REG" begin */
    REGDEF_BIT(MONITOR_ACC_REQ,         28)
    REGDEF_BIT(reserved1 ,              3)
    REGDEF_BIT(MONITOR_REQ_OVF,         1)
REGDEF_END(DMA_RECORD0_COUNT_REG)       /* declare register cache type "DMA_RECORD0_COUNT_REG" end */

#define DMA_RECORD1_SIZE_REG_OFS        (0x38 + MONITOR_OFFSET)
REGDEF_BEGIN(DMA_RECORD1_SIZE_REG)      /* declare register cache type "DMA_RECORD1_SIZE_REG" begin */
    REGDEF_BIT(MONITOR_ACC_SIZE,        28)
    REGDEF_BIT(reserved1 ,              3)
    REGDEF_BIT(MONITOR_SIZE_OVF,        1)
REGDEF_END(DMA_RECORD1_SIZE_REG)        /* declare register cache type "DMA_RECORD1_SIZE_REG" end */

#define DMA_RECORD1_COUNT_REG_OFS       (0x3C + MONITOR_OFFSET)
REGDEF_BEGIN(DMA_RECORD1_COUNT_REG)     /* declare register cache type "DMA_RECORD1_COUNT_REG" begin */
    REGDEF_BIT(MONITOR_ACC_REQ,         28)
    REGDEF_BIT(reserved1 ,              3)
    REGDEF_BIT(MONITOR_REQ_OVF,         1)
REGDEF_END(DMA_RECORD1_COUNT_REG)       /* declare register cache type "DMA_RECORD1_COUNT_REG" end */

#define DMA_RECORD2_SIZE_REG_OFS        (0x40 + MONITOR_OFFSET)
REGDEF_BEGIN(DMA_RECORD2_SIZE_REG)      /* declare register cache type "DMA_RECORD2_SIZE_REG" begin */
    REGDEF_BIT(MONITOR_ACC_SIZE,        28)
    REGDEF_BIT(reserved1 ,              3)
    REGDEF_BIT(MONITOR_SIZE_OVF,        1)
REGDEF_END(DMA_RECORD2_SIZE_REG)        /* declare register cache type "DMA_RECORD2_SIZE_REG" end */

#define DMA_RECORD2_COUNT_REG_OFS       (0x44 + MONITOR_OFFSET)
REGDEF_BEGIN(DMA_RECORD2_COUNT_REG)     /* declare register cache type "DMA_RECORD2_COUNT_REG" begin */
    REGDEF_BIT(MONITOR_ACC_REQ,         28)
    REGDEF_BIT(reserved1 ,              3)
    REGDEF_BIT(MONITOR_REQ_OVF,         1)
REGDEF_END(DMA_RECORD2_COUNT_REG)       /* declare register cache type "DMA_RECORD2_COUNT_REG" end */

#define DMA_RECORD3_SIZE_REG_OFS        (0x48 + MONITOR_OFFSET)
REGDEF_BEGIN(DMA_RECORD3_SIZE_REG)      /* declare register cache type "DMA_RECORD3_SIZE_REG" begin */
    REGDEF_BIT(MONITOR_ACC_SIZE,        28)
    REGDEF_BIT(reserved1 ,              3)
    REGDEF_BIT(MONITOR_SIZE_OVF,        1)
REGDEF_END(DMA_RECORD3_SIZE_REG)        /* declare register cache type "DMA_RECORD3_SIZE_REG" end */

#define DMA_RECORD3_COUNT_REG_OFS       (0x4C + MONITOR_OFFSET)
REGDEF_BEGIN(DMA_RECORD3_COUNT_REG)     /* declare register cache type "DMA_RECORD3_COUNT_REG" begin */
    REGDEF_BIT(MONITOR_ACC_REQ,         28)
    REGDEF_BIT(reserved1 ,              3)
    REGDEF_BIT(MONITOR_REQ_OVF,         1)
REGDEF_END(DMA_RECORD3_COUNT_REG)       /* declare register cache type "DMA_RECORD3_COUNT_REG" end */

#define DMA_RECORD4_SIZE_REG_OFS        (0x50 + MONITOR_OFFSET)
REGDEF_BEGIN(DMA_RECORD4_SIZE_REG)      /* declare register cache type "DMA_RECORD4_SIZE_REG" begin */
    REGDEF_BIT(MONITOR_ACC_SIZE,        28)
    REGDEF_BIT(reserved1 ,              3)
    REGDEF_BIT(MONITOR_SIZE_OVF,        1)
REGDEF_END(DMA_RECORD4_SIZE_REG)        /* declare register cache type "DMA_RECORD4_SIZE_REG" end */

#define DMA_RECORD4_COUNT_REG_OFS       (0x54 + MONITOR_OFFSET)
REGDEF_BEGIN(DMA_RECORD4_COUNT_REG)     /* declare register cache type "DMA_RECORD4_COUNT_REG" begin */
    REGDEF_BIT(MONITOR_ACC_REQ,         28)
    REGDEF_BIT(reserved1 ,              3)
    REGDEF_BIT(MONITOR_REQ_OVF,         1)
REGDEF_END(DMA_RECORD4_COUNT_REG)       /* declare register cache type "DMA_RECORD4_COUNT_REG" end */

#define DMA_RECORD5_SIZE_REG_OFS        (0x58 + MONITOR_OFFSET)
REGDEF_BEGIN(DMA_RECORD5_SIZE_REG)      /* declare register cache type "DMA_RECORD5_SIZE_REG" begin */
    REGDEF_BIT(MONITOR_ACC_SIZE,        28)
    REGDEF_BIT(reserved1 ,              3)
    REGDEF_BIT(MONITOR_SIZE_OVF,        1)
REGDEF_END(DMA_RECORD5_SIZE_REG)        /* declare register cache type "DMA_RECORD5_SIZE_REG" end */

#define DMA_RECORD5_COUNT_REG_OFS       (0x5C + MONITOR_OFFSET)
REGDEF_BEGIN(DMA_RECORD5_COUNT_REG)     /* declare register cache type "DMA_RECORD5_COUNT_REG" begin */
    REGDEF_BIT(MONITOR_ACC_REQ,         28)
    REGDEF_BIT(reserved1 ,              3)
    REGDEF_BIT(MONITOR_REQ_OVF,         1)
REGDEF_END(DMA_RECORD5_COUNT_REG)       /* declare register cache type "DMA_RECORD5_COUNT_REG" end */

#define DMA_RECORD6_SIZE_REG_OFS        (0x60 + MONITOR_OFFSET)
REGDEF_BEGIN(DMA_RECORD6_SIZE_REG)      /* declare register cache type "DMA_RECORD6_SIZE_REG" begin */
    REGDEF_BIT(MONITOR_ACC_SIZE,        28)
    REGDEF_BIT(reserved1 ,              3)
    REGDEF_BIT(MONITOR_SIZE_OVF,        1)
REGDEF_END(DMA_RECORD6_SIZE_REG)        /* declare register cache type "DMA_RECORD6_SIZE_REG" end */

#define DMA_RECORD6_COUNT_REG_OFS       (0x64 + MONITOR_OFFSET)
REGDEF_BEGIN(DMA_RECORD6_COUNT_REG)     /* declare register cache type "DMA_RECORD6_COUNT_REG" begin */
    REGDEF_BIT(MONITOR_ACC_REQ,         28)
    REGDEF_BIT(reserved1 ,              3)
    REGDEF_BIT(MONITOR_REQ_OVF,         1)
REGDEF_END(DMA_RECORD6_COUNT_REG)       /* declare register cache type "DMA_RECORD6_COUNT_REG" end */

#define DMA_RECORD7_SIZE_REG_OFS        (0x68 + MONITOR_OFFSET)
REGDEF_BEGIN(DMA_RECORD7_SIZE_REG)      /* declare register cache type "DMA_RECORD7_SIZE_REG" begin */
    REGDEF_BIT(MONITOR_ACC_SIZE,        28)
    REGDEF_BIT(reserved1 ,              3)
    REGDEF_BIT(MONITOR_SIZE_OVF,        1)
REGDEF_END(DMA_RECORD7_SIZE_REG)        /* declare register cache type "DMA_RECORD7_SIZE_REG" end */

#define DMA_RECORD7_COUNT_REG_OFS       (0x6C + MONITOR_OFFSET)
REGDEF_BEGIN(DMA_RECORD7_COUNT_REG)     /* declare register cache type "DMA_RECORD7_COUNT_REG" begin */
    REGDEF_BIT(MONITOR_ACC_REQ,         28)
    REGDEF_BIT(reserved1 ,              3)
    REGDEF_BIT(MONITOR_REQ_OVF,         1)
REGDEF_END(DMA_RECORD7_COUNT_REG)       /* declare register cache type "DMA_RECORD7_COUNT_REG" end */

#if 0
#define DMA_MIPS1_BANDWIDTH_REG_OFS     (0xB0 + MONITOR_OFFSET)
REGDEF_BEGIN(DMA_MIPS1_BANDWIDTH_REG)   /* declare register cache type "DMA_MIPS1_BANDWIDTH_REG" begin */
    REGDEF_BIT(REQ_NUM  , 16)
    REGDEF_BIT(reserved , 16)
REGDEF_END(DMA_MIPS1_BANDWIDTH_REG)     /* declare register cache type "DMA_MIPS1_BANDWIDTH_REG" end */

#define DMA_MIPS2_BANDWIDTH_REG_OFS     (0xB4 + MONITOR_OFFSET)
REGDEF_BEGIN(DMA_MIPS2_BANDWIDTH_REG)   /* declare register cache type "DMA_MIPS2_BANDWIDTH_REG" begin */
    REGDEF_BIT(REQ_NUM  , 16)
    REGDEF_BIT(reserved , 16)
REGDEF_END(DMA_MIPS2_BANDWIDTH_REG)     /* declare register cache type "DMA_MIPS2_BANDWIDTH_REG" end */

#define DMA_DSP1_BANDWIDTH_REG_OFS      (0xB8 + MONITOR_OFFSET)
REGDEF_BEGIN(DMA_DSP1_BANDWIDTH_REG)    /* declare register cache type "DMA_DSP1_BANDWIDTH_REG" begin */
    REGDEF_BIT(REQ_NUM  , 16)
    REGDEF_BIT(reserved , 16)
REGDEF_END(DMA_DSP1_BANDWIDTH_REG)      /* declare register cache type "DMA_DSP1_BANDWIDTH_REG" end */

#define DMA_DSP2_BANDWIDTH_REG_OFS      (0xBC + MONITOR_OFFSET)
REGDEF_BEGIN(DMA_DSP2_BANDWIDTH_REG)    /* declare register cache type "DMA_DSP2_BANDWIDTH_REG" begin */
    REGDEF_BIT(REQ_NUM  , 16)
    REGDEF_BIT(reserved , 16)
REGDEF_END(DMA_DSP2_BANDWIDTH_REG)      /* declare register cache type "DMA_DSP2_BANDWIDTH_REG" end */
#endif

//
// 0x1000~0x7FFC: DDR PHY region (provided by IP team)
//
#define DMA_PHY_CONTROL_REG_OFS    0x1000

#define DMA_PHY_00_REG_OFS      (0x0*4 + DMA_PHY_CONTROL_REG_OFS)
REGDEF_BEGIN(DMA_PHY_00_REG)
    REGDEF_BIT(ADJ_SEL_CK               , 4)
    REGDEF_BIT(ADJ_TYPE_CK              , 1)
    REGDEF_BIT(ADJ_EN_CK                , 1)
    REGDEF_BIT(                         , 26)
REGDEF_END(DMA_PHY_00_REG)

#define DMA_PHY_07_REG_OFS      (0x7*4 + DMA_PHY_CONTROL_REG_OFS)
REGDEF_BEGIN(DMA_PHY_07_REG)
    REGDEF_BIT(ADJ_SEL_DQS              , 4)
    REGDEF_BIT(ADJ_TYPE_DQS             , 1)
    REGDEF_BIT(ADJ_EN_DQS               , 1)
    REGDEF_BIT(                         , 26)
REGDEF_END(DMA_PHY_07_REG)

#define DMA_PHY_09_REG_OFS      (0x9*4 + DMA_PHY_CONTROL_REG_OFS)
REGDEF_BEGIN(DMA_PHY_09_REG)
    REGDEF_BIT(                         , 5)
    REGDEF_BIT(MCLK_POL                 , 1)
    REGDEF_BIT(D0_WEN_EN                , 1)
    REGDEF_BIT(D1_WEN_EN                , 1)
    REGDEF_BIT(                         , 24)
REGDEF_END(DMA_PHY_09_REG)

#define DMA_PHY_0A_REG_OFS      (0xA*4 + DMA_PHY_CONTROL_REG_OFS)
REGDEF_BEGIN(DMA_PHY_0A_REG)
    REGDEF_BIT(REN0_SEL                 , 4)
    REGDEF_BIT(REN1_SEL                 , 4)
    REGDEF_BIT(                         , 24)
REGDEF_END(DMA_PHY_0A_REG)

#define DMA_PHY_0B_REG_OFS      (0xB*4 + DMA_PHY_CONTROL_REG_OFS)
REGDEF_BEGIN(DMA_PHY_0B_REG)
    REGDEF_BIT(REN2_SEL                 , 4)
    REGDEF_BIT(REN3_SEL                 , 4)
    REGDEF_BIT(                         , 24)
REGDEF_END(DMA_PHY_0B_REG)

#define DMA_PHY_38_REG_OFS    (0x38*4 + DMA_PHY_CONTROL_REG_OFS)
REGDEF_BEGIN(DMA_PHY_38_REG)
    REGDEF_BIT(PLEG_CA                  , 5)
    REGDEF_BIT(Reserved                 ,27)
REGDEF_END(DMA_PHY_38_REG)

#define DMA_PHY_39_REG_OFS    (0x39*4 + DMA_PHY_CONTROL_REG_OFS)
REGDEF_BEGIN(DMA_PHY_39_REG)
    REGDEF_BIT(NLEG_CA                  , 5)
    REGDEF_BIT(Reserved                 ,27)
REGDEF_END(DMA_PHY_39_REG)

#define DMA_PHY_40_REG_OFS    (0x40*4 + DMA_PHY_CONTROL_REG_OFS)
REGDEF_BEGIN(DMA_PHY_40_REG)
    REGDEF_BIT(PLEG_MCLK                , 5)
    REGDEF_BIT(Reserved                 ,27)
REGDEF_END(DMA_PHY_40_REG)

#define DMA_PHY_41_REG_OFS    (0x41*4 + DMA_PHY_CONTROL_REG_OFS)
REGDEF_BEGIN(DMA_PHY_41_REG)
    REGDEF_BIT(NLEG_MCLK                , 5)
    REGDEF_BIT(Reserved                 ,27)
REGDEF_END(DMA_PHY_41_REG)

#define DMA_PHY_42_REG_OFS    (0x42*4 + DMA_PHY_CONTROL_REG_OFS)
REGDEF_BEGIN(DMA_PHY_42_REG)
    REGDEF_BIT(PLEG_0_LB                , 5)
    REGDEF_BIT(Reserved                 ,27)
REGDEF_END(DMA_PHY_42_REG)

#define DMA_PHY_43_REG_OFS    (0x43*4 + DMA_PHY_CONTROL_REG_OFS)
REGDEF_BEGIN(DMA_PHY_43_REG)
    REGDEF_BIT(NLEG_0_LB                , 5)
    REGDEF_BIT(Reserved                 ,27)
REGDEF_END(DMA_PHY_43_REG)

#define DMA_PHY_44_REG_OFS    (0x44*4 + DMA_PHY_CONTROL_REG_OFS)
REGDEF_BEGIN(DMA_PHY_44_REG)
    REGDEF_BIT(PLEG_0_HB                , 5)
    REGDEF_BIT(Reserved                 ,27)
REGDEF_END(DMA_PHY_44_REG)

#define DMA_PHY_45_REG_OFS    (0x45*4 + DMA_PHY_CONTROL_REG_OFS)
REGDEF_BEGIN(DMA_PHY_45_REG)
    REGDEF_BIT(NLEG_0_HB                , 5)
    REGDEF_BIT(Reserved                 ,27)
REGDEF_END(DMA_PHY_45_REG)

#define DMA_PHY_46_REG_OFS    (0x46*4 + DMA_PHY_CONTROL_REG_OFS)
REGDEF_BEGIN(DMA_PHY_46_REG)
    REGDEF_BIT(PLEG_0_DQS               , 5)
    REGDEF_BIT(Reserved                 ,27)
REGDEF_END(DMA_PHY_46_REG)

#define DMA_PHY_47_REG_OFS    (0x47*4 + DMA_PHY_CONTROL_REG_OFS)
REGDEF_BEGIN(DMA_PHY_47_REG)
    REGDEF_BIT(NLEG_0_DQS               , 5)
    REGDEF_BIT(Reserved                 ,27)
REGDEF_END(DMA_PHY_47_REG)

#define DMA_PHY_53_REG_OFS    (0x53*4 + DMA_PHY_CONTROL_REG_OFS)
REGDEF_BEGIN(DMA_PHY_53_REG)
    REGDEF_BIT(NLEG_CA_MRST             , 5)
    REGDEF_BIT(ODTEN_CA_MCS             , 1)
    REGDEF_BIT(ODTEN_CA_MRST            , 1)
    REGDEF_BIT(Reserved                 ,24)
REGDEF_END(DMA_PHY_53_REG)

#define DMA_PHY_54_REG_OFS    (0x54*4 + DMA_PHY_CONTROL_REG_OFS)
REGDEF_BEGIN(DMA_PHY_54_REG)
    REGDEF_BIT(NLEG_CA_MCS              , 5)
    REGDEF_BIT(Reserved                 ,27)
REGDEF_END(DMA_PHY_54_REG)

#define DMA_PHY_57_REG_OFS    (0x57*4 + DMA_PHY_CONTROL_REG_OFS)
REGDEF_BEGIN(DMA_PHY_57_REG)
    REGDEF_BIT(PLEG_CA_MCS              , 5)
    REGDEF_BIT(Reserved                 ,27)
REGDEF_END(DMA_PHY_57_REG)

#define DMA_PHY_58_REG_OFS    (0x58*4 + DMA_PHY_CONTROL_REG_OFS)
REGDEF_BEGIN(DMA_PHY_58_REG)
    REGDEF_BIT(PLEG_CA_MRST             , 5)
    REGDEF_BIT(Reserved                 ,27)
REGDEF_END(DMA_PHY_58_REG)

#define DMA_PHY_85_REG_OFS    (0x85*4 + DMA_PHY_CONTROL_REG_OFS)
REGDEF_BEGIN(DMA_PHY_85_REG)
    REGDEF_BIT(ODT_MODE_CK              , 3)
    REGDEF_BIT(ODT_EN_CK                , 1)
    REGDEF_BIT(ODT_MODE_CMD             , 3)
    REGDEF_BIT(ODT_EN_CMD               , 1)
    REGDEF_BIT(Reserved                 ,24)
REGDEF_END(DMA_PHY_85_REG)

#define DMA_PHY_86_REG_OFS    (0x86*4 + DMA_PHY_CONTROL_REG_OFS)
REGDEF_BEGIN(DMA_PHY_86_REG)
    REGDEF_BIT(ODT_MODE_DQS0            , 3)
    REGDEF_BIT(ODT_EN_DQS0              , 1)
    REGDEF_BIT(ODT_MODE_D0              , 3)
    REGDEF_BIT(ODT_EN_D0                , 1)
    REGDEF_BIT(Reserved                 ,24)
REGDEF_END(DMA_PHY_86_REG)

#define DMA_PHY_AE_REG_OFS    (0xAE*4 + DMA_PHY_CONTROL_REG_OFS)
REGDEF_BEGIN(DMA_PHY_AE_REG)
    REGDEF_BIT(REG_WR_DQ1               , 5)
    REGDEF_BIT(Reserved                 ,27)
REGDEF_END(DMA_PHY_AE_REG)

#define DMA_PHY_AF_REG_OFS    (0xAF*4 + DMA_PHY_CONTROL_REG_OFS)
REGDEF_BEGIN(DMA_PHY_AF_REG)
    REGDEF_BIT(REG_WR_DQS1              , 5)
    REGDEF_BIT(Reserved                 ,27)
REGDEF_END(DMA_PHY_AF_REG)

#define DMA_PHY_B0_REG_OFS    (0xB0*4 + DMA_PHY_CONTROL_REG_OFS)
REGDEF_BEGIN(DMA_PHY_B0_REG)
    REGDEF_BIT(REG_CMD                  , 5)
    REGDEF_BIT(                         , 1)
    REGDEF_BIT(DATA0_REG_EN12           , 1)
    REGDEF_BIT(REG_RESET                , 1)
    REGDEF_BIT(Reserved                 ,24)
REGDEF_END(DMA_PHY_B0_REG)

#define DMA_PHY_B1_REG_OFS    (0xB1*4 + DMA_PHY_CONTROL_REG_OFS)
REGDEF_BEGIN(DMA_PHY_B1_REG)
    REGDEF_BIT(REG_CLK                  , 5)
    REGDEF_BIT(REG_VREF_2               , 2)
    REGDEF_BIT(REG_FD12                 , 1)
    REGDEF_BIT(Reserved                 ,24)
REGDEF_END(DMA_PHY_B1_REG)

#define DMA_PHY_B2_REG_OFS    (0xB2*4 + DMA_PHY_CONTROL_REG_OFS)
REGDEF_BEGIN(DMA_PHY_B2_REG)
    REGDEF_BIT(REG_WR_DQ0               , 5)
    REGDEF_BIT(                         , 2)
    REGDEF_BIT(REG_CHMEN                , 1)
    REGDEF_BIT(Reserved                 ,24)
REGDEF_END(DMA_PHY_B2_REG)

#define DMA_PHY_B3_REG_OFS    (0xB3*4 + DMA_PHY_CONTROL_REG_OFS)
REGDEF_BEGIN(DMA_PHY_B3_REG)
    REGDEF_BIT(REG_WR_DQS0              , 5)
    REGDEF_BIT(                         , 1)
    REGDEF_BIT(DATA1_REG_EN_COMPVIL     , 1)
    REGDEF_BIT(DATA0_REG_EN_COMPVIL     , 1)
    REGDEF_BIT(Reserved                 ,24)
REGDEF_END(DMA_PHY_B3_REG)

#define DMA_PHY_B8_REG_OFS    (0xB8*4 + DMA_PHY_CONTROL_REG_OFS)
REGDEF_BEGIN(DMA_PHY_B8_REG)
    REGDEF_BIT(REG_DQS1_90              , 4)
    REGDEF_BIT(REG_DQS0_90              , 4)
    REGDEF_BIT(Reserved                 ,24)
REGDEF_END(DMA_PHY_B8_REG)

#endif // _DMAREG_H
