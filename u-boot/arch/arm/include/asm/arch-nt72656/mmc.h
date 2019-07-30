/*
 * Copyright (C) 2014 Novatek Microelectronics Corp. All rights reserved.
 * Author: SP-KSW <SP_KSW_MailGrp@novatek.com.tw>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation's version 2 of
 * the License.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 * MA 02111-1307 USA
 */
 /*
 *  use source from
 *  arch/arm/include/asm/arch-nt72668/nt72668_mmc.h
 *
 *  Author:	Alvin lin
 *  Created:	April 24, 2013
 *  Copyright:	Novatek Inc.
 *
 */

#ifndef __NT72656_MMC_HOST_H
#define __NT72656_MMC_HOST_H

//#include <asm/arch/hardware.h>
#include <asm/types.h>

#define REG_FCR_BASE                            (0xFC048200)
#define REG_SDC_BASE                            (0xFC048300)
//#define REG_NFC_BASE                            (0xFC048000)
#define EMMC_SECURE_LOCK_CTRL                   *(volatile unsigned int *)(0xFD650000)
#define EMMC_CLK_SRC_CTRL                       *(volatile unsigned int *)(0xFC040C18)

#define EMMC_SECURE_LOCK_ENABLE                 (1<<4)
#define EMMC_FAST_CLK_SRC_ENABLE                (1<<16)
// -------------------------------------------------------------------------------------------------
#define REG_FCR_FUNC_CTRL                       (*(volatile u32*)(REG_FCR_BASE+0x00))
#define REG_FCR_CPBLT                           (*(volatile u32*)(REG_FCR_BASE+0x04))
#define REG_FCR_SD_2nd_FUNC_CTRL                (*(volatile u32*)(REG_FCR_BASE+0x08))
#define REG_FCR_FUNC_CTRL_1                     (*(volatile u32*)(REG_FCR_BASE+0x0c))
#define REG_FCR_HS200_CTRL			(*(volatile u32*)(REG_FCR_BASE+0x14))

// =================================================================================================
#define FCR_HS200_CTRL_ENABLE				(1<<0)
#define FCR_HS200_CTRL_SW_OVERSAMPLE_ENABLE		(1<<1)
#define FCR_HS200_CTRL_SW_OVERSAMPLE_CMD_MASK		(3<<2)
#define FCR_HS200_CTRL_HW_TRACK_EACH_BLK		(1<<6)
//#define FCR_HS200_CRTL_FASTEST_CLK			(1<<7)
#define FCR_HS200_CTRL_FASTEST_CLK			(1<<7)
#define FCR_HS200_CTRL_DISABLE_CMD_CONFLICT		(1<<8)
#define FCR_HS200_OUTPUT_SELECT_MASK			(3<<26)
#define FCR_HS200_OUTPUT_SELECT_PHASE(phase)		((phase&3)<<26)
/* TODO: all hs200 new setting */

// =================================================================================================
// REG_FCR_FUNC_CTRL
#define FCR_FUNC_CTRL_SW_MS_CD                  (1 << 27)
#define FCR_FUNC_CTRL_SW_SD_CD                  (1 << 26)
#define FCR_FUNC_CTRL_SW_SD_WP                  (1 << 25)
#define FCR_FUNC_CTRL_SW_CDWP_ENABLE            (1 << 24)
#define FCR_FUNC_CTRL_LITTLE_ENDIAN             (1 << 23)
#define FCR_FUNC_CTRL_SD_FLEXIBLE_CLK           (1 << 22)
#define FCR_FUNC_CTRL_AHB_MAX_BURST_SIZE(n)     (((n) & 3) << 20)
#define FCR_FUNC_CTRL_AHB_MAX_BURST_SIZE_MASK   FCR_FUNC_CTRL_AHB_MAX_BURST_SIZE(-1)
#define FCR_FUNC_CTRL_AHB_MAX_BURST_SIZE_512    FCR_FUNC_CTRL_AHB_MAX_BURST_SIZE(3)
#define FCR_FUNC_CTRL_AHB_MAX_BURST_SIZE_256    FCR_FUNC_CTRL_AHB_MAX_BURST_SIZE(2)
#define FCR_FUNC_CTRL_AHB_MAX_BURST_SIZE_128    FCR_FUNC_CTRL_AHB_MAX_BURST_SIZE(1)
#define FCR_FUNC_CTRL_AHB_MAX_BURST_SIZE_64     FCR_FUNC_CTRL_AHB_MAX_BURST_SIZE(0)
#define FCR_FUNC_CTRL_INT_OPT_STAT              (1 << 19)
#define FCR_FUNC_CTRL_INT_OPT_DMA               (1 << 18)
#define FCR_FUNC_CTRL_SD_PULLUP_RESISTOR        (1 << 17)
#define FCR_FUNC_CTRL_MMC_8BIT                  (1 << 16)
#define FCR_FUNC_CTRL_CD_DEBOUNCE_TIME(n)       (((n) & 0xf) << 12)
#define FCR_FUNC_CTRL_CD_DEBOUNCE_TIME_MASK     FCR_FUNC_CTRL_CD_DEBOUNCE_TIME(-1)
#define FCR_FUNC_CTRL_PW_UP_TIME(n)             (((n) & 0xf) << 8)
#define FCR_FUNC_CTRL_PW_UP_TIME_MASK           FCR_FUNC_CTRL_PW_UP_TIME(-1)
#define FCR_FUNC_CTRL_SD_SIG_PULLUP_TIME(n)     (((n) & 0xf) << 4)
#define FCR_FUNC_CTRL_SD_SIG_PULLUP_TIME_MASK   FCR_FUNC_CTRL_SD_SIG_PULLUP_TIME(-1)
#define FCR_FUNC_CTRL_MS_SIG_DELAY(n)           (((n) & 3) << 2)
#define FCR_FUNC_CTRL_MS_SIG_DELAY_MASK         FCR_FUNC_CTRL_MS_SIG_DELAY(-1)
#define FCR_FUNC_CTRL_SD_SIG_DELAY(n)           ((n) & 3)
#define FCR_FUNC_CTRL_SD_SIG_DELAY_MASK         FCR_FUNC_CTRL_SD_SIG_DELAY(-1)
#define FCR_FUNC_CTRL_READ_CLK_DELAY(n)		(((n) & 3) << 28)
#define FCR_FUNC_CTRL_READ_CLK_DELAY_MASK	FCR_FUNC_CTRL_READ_CLK_DELAY(-1)

#define SDC_MAX_CLK_DIV				(1023)
//#define SDC_MAX_CLK_DIV				(255)
// -------------------------------------------------------------------------------------------------
// REG_FCR_CPBLT

#define FCR_DDRMdEn                             (1 << 19)

#define FCR_CPBLT_SD_CLK_BYPASS			(1 << 20)
#define FCR_CPBLT_DUAL_DATA_RATE_ENABLE		(1 << 19)
#define FCR_CPBLT_VOL_18V                       (1 << 18)
#define FCR_CPBLT_VOL_30V                       (1 << 17)
#define FCR_CPBLT_VOL_33V                       (1 << 16)
#define FCR_CPBLT_SD_BASE_CLK_FREQ(n)           (((n) & 0x3f) << 8)
#define FCR_CPBLT_SD_BASE_CLK_FREQ_MASK         FCR_CPBLT_SD_BASE_CLK_FREQ(-1)
#define FCR_CPBLT_SD_MAX_CURR_CPBLT(n)          ((n) & 0xff)
#define FCR_CPBLT_SD_MAX_CURR_CPBLT_MASK        FCR_CPBLT_SD_MAX_CURR_CPBLT(-1)

// =================================================================================================


// -------------------------------------------------------------------------------------------------
#define REG_SDC_DMA_ADDR                        (*(volatile u32*)(REG_SDC_BASE+0x00))
#define REG_SDC_BLK_SIZE                        (*(volatile  u16*)(REG_SDC_BASE+0x04))
#define REG_SDC_BLK_COUNT                       (*(volatile  u16*)(REG_SDC_BASE+0x06))
#define REG_SDC_ARG                             (*(volatile u32*)(REG_SDC_BASE+0x08))
#define REG_SDC_TRAN_MODE                       (*(volatile  u16*)(REG_SDC_BASE+0x0c))
#define REG_SDC_CMD                             (*(volatile  u16*)(REG_SDC_BASE+0x0e))
#define REG_SDC_RESP(n)                         (*(volatile u32*)(REG_SDC_BASE+0x10+4*((n)&3)))
#define REG_SDC_RESP0                           REG_SDC_RESP(0)
#define REG_SDC_RESP1                           REG_SDC_RESP(1)
#define REG_SDC_RESP2                           REG_SDC_RESP(2)
#define REG_SDC_RESP3                           REG_SDC_RESP(3)
#define REG_SDC_DATA_PORT                       (*(volatile u32*)(REG_SDC_BASE+0x20))
#define REG_SDC_STAT                            (*(volatile u32*)(REG_SDC_BASE+0x24))
#define REG_SDC_HOST_CTRL                       (*(volatile  u8*)(REG_SDC_BASE+0x28))
#define REG_SDC_PW_CTRL                         (*(volatile  u8*)(REG_SDC_BASE+0x29))
#define REG_SDC_BLK_GAP_CTRL                    (*(volatile  u8*)(REG_SDC_BASE+0x2a))
#define REG_SDC_WAKEUP_CTRL                     (*(volatile  u8*)(REG_SDC_BASE+0x2b))
#define REG_SDC_CLK_CTRL                        (*(volatile  u16*)(REG_SDC_BASE+0x2c))
#define REG_SDC_TIMEOUT_CTRL                    (*(volatile  u8*)(REG_SDC_BASE+0x2e))
#define REG_SDC_SW_RESET                        (*(volatile  u8*)(REG_SDC_BASE+0x2f))
#define REG_SDC_INT_STAT                        (*(volatile u32*)(REG_SDC_BASE+0x30))

#define REG_SDC_ERR_INT_STAT                    (*(volatile u16*)(REG_SDC_BASE+0x32))
#define ADMA_Error_INT	(1<<9)
#define Data_Timeout_Error_INT	(1<<4)


#define REG_SDC_INT_STAT_ENABLE                 (*(volatile u32*)(REG_SDC_BASE+0x34))
#define REG_SDC_INT_ENABLE                      (*(volatile u32*)(REG_SDC_BASE+0x38))

//08 RW Card Interrupt Signal Enable
//07 RW Card Removal Signal Enable
//06 RW Card Insertion Signal Enable
//05 RW Buffer Read Ready Signal Enable
//04 RW Buffer Write Ready Signal Enable
//03 RW DMA Interrupt Signal Enable
//02 RW Block Gap Event Signal Enable
//01 RW Transfer Complete Signal Enable
//00 RW Command Complete Signal Enable
#define eMMC_INT_Card_Interrupt_Signal_Enable       (1<<8)
#define eMMC_INT_Card_Removal_Signal_Enable         (1<<7)
#define eMMC_INT_Card_Insertion_Signal_Enable       (1<<6)
#define eMMC_INT_Buffer_Read_Ready_Signal_Enable    (1<<5)
#define eMMC_INT_Buffer_Write_Ready_Signal_Enable   (1<<4)
#define eMMC_INT_DMA_Interrupt_Signal_Enable        (1<<3)
#define eMMC_INT_Block_Gap_Event_Signal_Enable      (1<<2)
#define eMMC_INT_Transfer_Complete_Signal_Enable    (1<<1)
#define eMMC_INT_Command_Complete_Signal_Enable     (1<<0)

//#define REG_SDC_ERR_INT_ENABLE                  (*(volatile u16*)(REG_SDC_BASE+0x3a))

//15-12 RW Vendor Specific Error Signal Enable
//11-10 Rsvd Reserved
//09 RW ADMA Error Signal Enable
//08 RW Auto CMD12 Error Signal Enable
//07 RW Current Limit Error Signal Enable
//06 RW Data End Bit Error Signal Enable
//05 RW Data CRC Error Signal Enable
//04 RW Data Timeout Error Signal Enable
//03 RW Command Index Error Signal Enable
//02 RW Command End Bit Error Signal Enable
//01 RW Command CRC Error Signal Enable
//00 RW Command Timeout Error Signal Enable

//Original
//#define eMMC_ERR_INT_ADMA_Error_Signal_Enable       (1<<9)   // 688 got SDMA only
//Ben, open the following #define for 568, Nov/15/2011.
#define eMMC_ERR_INT_ADMA_Error_Signal_Enable       (1<<9) 


#define eMMC_ERR_INT_Auto_CMD12_Error_Signal_Enable       (1<<8) 
#define eMMC_ERR_INT_Current_Limit_Error_Signal_Enable    (1<<7)
#define eMMC_ERR_INT_Data_End_Bit_Error_Signal_Enable     (1<<6) 
#define eMMC_ERR_INT_Data_CRC_Error_Signal_Enable         (1<<5)
#define eMMC_ERR_INT_Data_Timeout_Error_Signal_Enable     (1<<4) 
#define eMMC_ERR_INT_Command_Index_Error_Signal_Enable    (1<<3)
#define eMMC_ERR_INT_Command_End_Bit_Error_Signal_Enable  (1<<2) 
#define eMMC_ERR_INT_Command_CRC_Error_Signal_Enable      (1<<1)
#define eMMC_ERR_INT_Command_Timeout_Error_Signal_Enable  (1<<0) 

//Original
//#define eMMC_ALL_ERR_INT                                  0x1ff   // ERR INT bits 0~8
//Ben, Nov/15/2011.
#define eMMC_ALL_ERR_INT                                  0x3ff   // ERR INT bits 0~9
   

#define REG_SDC_AUTO_CMD12_ERR_STAT             (*(volatile  u16*)(REG_SDC_BASE+0x3c))
#define REG_SDC_CPBLT(n)                        (*(volatile u32*)(REG_SDC_BASE+0x40+4*(n)))
#define REG_SDC_CPBLT0                          REG_SDC_CPBLT(0)
#define REG_SDC_CPBLT1                          REG_SDC_CPBLT(1)
#define REG_SDC_MAX_CURR_CPBLT(n)               (*(volatile u32*)(REG_SDC_BASE+0x48+4*(n)))
#define REG_SDC_MAX_CURR_CPBLT0                 REG_SDC_MAX_CURR_CPBLT(0)
#define REG_SDC_MAX_CURR_CPBLT1                 REG_SDC_MAX_CURR_CPBLT(1)
#define REG_SDC_SLOT_INT_STAT                   (*(volatile  u16*)(REG_SDC_BASE+0xfc))
#define REG_SDC_HOST_VER                        (*(volatile  u16*)(REG_SDC_BASE+0xfe))

#define   SDHCI_SPEC_100	0
#define   SDHCI_SPEC_200	1

#define REG_SDC_ADMA_Error_Status               (*(volatile  u8*)(REG_SDC_BASE+0x54))
#define REG_SDC_ADMA_SYSTEM_ADDR                (*(volatile  u32*)(REG_SDC_BASE+0x58))
#define REG_SDC_ADMA_SYSTEM_ADDR_MASK		(0x1FFFFFFF)

//Ben+, Nov/15/2011.
#define ADMA_Length_Mismatch_Error	(1<<2)

#define REG_Force_Event_Register_for_Error_Interrupt_Status    (*(volatile  u16*)(REG_SDC_BASE+0x52))
#define REG_Force_Event_Register_for_Auto_CMD12_Error_Status    (*(volatile  u16*)(REG_SDC_BASE+0x50))

#define Auto_CMD12_Error_Bit_Auto_CMD12_Not_Executed                   (1<<0)
#define Auto_CMD12_Error_Bit_Auto_CMD12_Timeout_Error                  (1<<1)
#define Auto_CMD12_Error_Bit_Auto_CMD12_CRC_Error                      (1<<2)
#define Auto_CMD12_Error_Bit_Auto_CMD12_End_Bit_Error                  (1<<3)
#define Auto_CMD12_Error_Bit_Auto_CMD12_Index_Error                    (1<<4)
#define Auto_CMD12_Error_Bit_Command_Not_Issued_By_Auto_CMD12_Error    (1<<7)

//Ben, for ADMA test item 20, Block Gap Stop/Continue test, Dec/22/2011.
#define Stop_At_Block_Gap_Request                                      (1<<0)
#define Continue_Request                                               (1<<1)


// =================================================================================================
// REG_SDC_BLK_SIZE
#define SDC_BLK_SIZE_DMA_BUFF_BND(n)            ((u16)((n) & 7) << 12)
#define SDC_BLK_SIZE_DMA_BUFF_BND_MASK          SDC_BLK_SIZE_DMA_BUFF_BND(-1)
#define SDC_BLK_SIZE_DMA_BUFF_BND_512K          SDC_BLK_SIZE_DMA_BUFF_BND(7)
#define SDC_BLK_SIZE_DMA_BUFF_BND_256K          SDC_BLK_SIZE_DMA_BUFF_BND(6)
#define SDC_BLK_SIZE_DMA_BUFF_BND_128K          SDC_BLK_SIZE_DMA_BUFF_BND(5)
#define SDC_BLK_SIZE_DMA_BUFF_BND_64K           SDC_BLK_SIZE_DMA_BUFF_BND(4)
#define SDC_BLK_SIZE_DMA_BUFF_BND_32K           SDC_BLK_SIZE_DMA_BUFF_BND(3)
#define SDC_BLK_SIZE_DMA_BUFF_BND_16K           SDC_BLK_SIZE_DMA_BUFF_BND(2)
#define SDC_BLK_SIZE_DMA_BUFF_BND_8K            SDC_BLK_SIZE_DMA_BUFF_BND(1)
#define SDC_BLK_SIZE_DMA_BUFF_BND_4K            SDC_BLK_SIZE_DMA_BUFF_BND(0)
#define SDC_BLK_SIZE_TRAN_BLK_SIZE(n)           ((u16)((n) & 0xfff))

// -------------------------------------------------------------------------------------------------
// REG_SDC_TRAN_MODE
#define SDC_TRAN_MODE_MULT_BLK                  (1 << 5)
#define SDC_TRAN_MODE_READ                      (1 << 4)
#define SDC_TRAN_MODE_AUTO_CMD12                (1 << 2)
#define SDC_TRAN_MODE_BLK_COUNT                 (1 << 1)
#define SDC_TRAN_MODE_DMA                       (1 << 0)

// -------------------------------------------------------------------------------------------------
// REG_SDC_CMD
#define SDC_CMD_IDX(n)                          ((((u16)(n)) & 0x3f) << 8)
#define SDC_CMD_TYPE_ABORT                      (3 << 6)
#define SDC_CMD_TYPE_RESUME                     (2 << 6)
#define SDC_CMD_TYPE_SUSPEND                    (1 << 6)
#define SDC_CMD_TYPE_NORMAL                     (0 << 6)
#define SDC_CMD_DATA_PRESENT                    (1 << 5)
#define SDC_CMD_IDX_CHK                         (1 << 4)
#define SDC_CMD_CRC_CHK                         (1 << 3)
#define SDC_CMD_RESP_TYPE_LEN_0                 (0 << 0)
#define SDC_CMD_RESP_TYPE_LEN_136               (1 << 0)
#define SDC_CMD_RESP_TYPE_LEN_48                (2 << 0)
#define SDC_CMD_RESP_TYPE_LEN_48_BUSY_CHK       (3 << 0)

// -------------------------------------------------------------------------------------------------
// REG_SDC_STAT
#define SDC_STAT_CMD_SIG                        (1 << 24)
#define SDC_STAT_DAT_SIG(n)                     (1 << (20 + (n)))
#define SDC_STAT_DAT_SIG0                       SDC_STAT_DAT_SIG(0)
#define SDC_STAT_DAT_SIG1                       SDC_STAT_DAT_SIG(1)
#define SDC_STAT_DAT_SIG2                       SDC_STAT_DAT_SIG(2)
#define SDC_STAT_DAT_SIG3                       SDC_STAT_DAT_SIG(3)
#define SDC_STAT_WP_PIN                         (1 << 19)
#define SDC_STAT_CD_PIN                         (1 << 18)
#define SDC_STAT_CARD_STAT_STABLE               (1 << 17)
#define SDC_STAT_CARD_INS                       (1 << 16)
#define SDC_STAT_BUFF_READ_ENABLE               (1 << 11)
#define SDC_STAT_BUFF_WRITE_ENABLE              (1 << 10)
#define SDC_STAT_READ_TRAN_ACTIVE               (1 << 9)
#define SDC_STAT_WRITE_TRAN_ACTIVE              (1 << 8)
#define SDC_STAT_DAT_LINE_ACTIVE                (1 << 2)
#define SDC_STAT_CMD_INHIBIT_DAT                (1 << 1)
#define SDC_STAT_CMD_INHIBIT_CMD                (1 << 0)

// -------------------------------------------------------------------------------------------------
// REG_SDC_HOST_CTRL
#define SDC_HOST_CTRL_8BIT			(1 << 5)
#define SDC_HOST_CTRL_HIGH_SPEED                (1 << 2)
#define SDC_HOST_CTRL_4BIT                      (1 << 1)
#define SDC_HOST_CTRL_LED_ON                    (1 << 0)

// -------------------------------------------------------------------------------------------------
// REG_SDC_PW_CTRL
#define SDC_PW_CTRL_BUS_VOL(n)                  ((((u16)(n)) & 7) << 1)
#define SDC_PW_CTRL_BUS_VOL_33V                 SDC_PW_CTRL_BUS_VOL(7)
#define SDC_PW_CTRL_BUS_VOL_30V                 SDC_PW_CTRL_BUS_VOL(6)
#define SDC_PW_CTRL_BUS_VOL_18V                 SDC_PW_CTRL_BUS_VOL(5)
#define SDC_PW_CTRL_BUS_PW_ON                   (1 << 0)

// -------------------------------------------------------------------------------------------------
// REG_SDC_BLK_GAP_CTRL
#define SDC_BLK_GAP_CTRL_INT_AT_BLK_GAP         (1 << 3)
#define SDC_BLK_GAP_CTRL_READ_WAIT_CTRL         (1 << 2)
#define SDC_BLK_GAP_CTRL_CONT_REQ               (1 << 1)
#define SDC_BLK_GAP_CTRL_STOP_AT_BLK_GAP_REQ    (1 << 0)

// -------------------------------------------------------------------------------------------------
// REG_SDC_WAKEUP_CTRL
#define SDC_WAKEUP_CTRL_CARD_REM                (1 << 2)
#define SDC_WAKEUP_CTRL_CARD_INS                (1 << 1)
#define SDC_WAKEUP_CTRL_CARD_INT                (1 << 0)

// -------------------------------------------------------------------------------------------------
// REG_SDC_CLK_CTRL
#define SDC_CLK_CTRL_SDCLK_FREQ_SEL(n)          ((((u16)(n)) & 0xff) << 8)
#define SDC_CLK_CTRL_SDCLK_FREQ_SEL_EX(n)       ((((n) & 0xff) << 8) | ((n&0x300)>>2))
#define SDC_CLK_CTRL_SDCLK_ENABLE               (1 << 2)
#define SDC_CLK_CTRL_INCLK_STABLE               (1 << 1)
#define SDC_CLK_CTRL_INCLK_ENABLE               (1 << 0)

// -------------------------------------------------------------------------------------------------
// REG_SDC_TIMEOUT_CTRL
#define SDC_TIMEOUT_CTRL_DATA_TIMEOUT(n)        ((n) & 0xf)

// -------------------------------------------------------------------------------------------------
// REG_SDC_SW_RESET
#define SDC_SW_RESET_DAT_LINE                   (1 << 2)
#define SDC_SW_RESET_CMD_LINE                   (1 << 1)
#define SDC_SW_RESET_ALL                        (1 << 0)

// -------------------------------------------------------------------------------------------------
// REG_SDC_INT_STAT, REG_SDC_INT_STAT_ENABLE, REG_SDC_INT_ENABLE

//Original
//Ben, for previous Big Endian setting, Apr/19/2011.
/*
#define SDC_INT_ERR_INT                         (1 << 31)
#define SDC_INT_CARD_INT                        (1 << 24)
#define SDC_INT_CARD_REM                        (1 << 23)
#define SDC_INT_CARD_INS                        (1 << 22)
#define SDC_INT_BUFF_READ_RDY                   (1 << 21)
#define SDC_INT_BUFF_WRITE_RDY                  (1 << 20)
#define SDC_INT_DMA_INT                         (1 << 19)
#define SDC_INT_BLK_GAP_EVENT                   (1 << 18)
#define SDC_INT_TRAN_COMPLETE                   (1 << 17)
#define SDC_INT_CMD_COMPLETE                    (1 << 16)
#define SDC_INT_ERR_VENDOR_SPEC_STAT(n)         ((1 << ((n) + 12)) & 0xf000)
#define SDC_INT_ERR_AUTO_CMD12                  (1 << 8)
#define SDC_INT_ERR_CURR_LIMIT                  (1 << 7)
#define SDC_INT_ERR_DATA_END_BIT                (1 << 6)
#define SDC_INT_ERR_DATA_CRC                    (1 << 5)
#define SDC_INT_ERR_DATA_TIMEOUT                (1 << 4)
#define SDC_INT_ERR_CMD_IDX                     (1 << 3)
#define SDC_INT_ERR_CMD_END_BIT                 (1 << 2)
#define SDC_INT_ERR_CMD_CRC                     (1 << 1)
#define SDC_INT_ERR_CMD_TIMEOUT                 (1 << 0)
*/
//Ben, borrow from 78320 for 558/688 MIPS using Little Endian, Apr/18/2011. 
// Normal Interrupt Status Register (Offset 030h)
#define SDC_INT_ERR_INT                         (1 << 15)  // D09~D14 is Reserved
#define SDC_INT_CARD_INT                        (1 << 8)
#define SDC_INT_CARD_REM                        (1 << 7)
#define SDC_INT_CARD_INS                        (1 << 6)
#define SDC_INT_BUFF_READ_RDY                   (1 << 5)
#define SDC_INT_BUFF_WRITE_RDY                  (1 << 4)
#define SDC_INT_DMA_INT                         (1 << 3)
#define SDC_INT_BLK_GAP_EVENT                   (1 << 2)
#define SDC_INT_TRAN_COMPLETE                   (1 << 1)
#define SDC_INT_CMD_COMPLETE                    (1 << 0)

#define SDC_INT_ERR_VENDOR_SPEC_STAT(n)         ((1 << ((n) + 28)) & 0xf000000)
#define SDC_INT_ERR_AUTO_CMD12                  (1 << 24)
#define SDC_INT_ERR_CURR_LIMIT                  (1 << 23)
#define SDC_INT_ERR_DATA_END_BIT                (1 << 22)
#define SDC_INT_ERR_DATA_CRC                    (1 << 21)
#define SDC_INT_ERR_DATA_TIMEOUT                (1 << 20)
#define SDC_INT_ERR_CMD_IDX                     (1 << 19)
#define SDC_INT_ERR_CMD_END_BIT                 (1 << 18)
#define SDC_INT_ERR_CMD_CRC                     (1 << 17)
#define SDC_INT_ERR_CMD_TIMEOUT                 (1 << 16)

// -------------------------------------------------------------------------------------------------
// REG_SDC_AUTO_CMD12_ERR_STAT
#define SDC_AUTO_CMD12_ERR_STAT_CMD_NOT_ISSUED_BY_AUTO_CMD12_ERR (1 << 7)
#define SDC_AUTO_CMD12_ERR_STAT_IDX_ERR         (1 << 4)
#define SDC_AUTO_CMD12_ERR_STAT_END_BIT_ERR     (1 << 3)
#define SDC_AUTO_CMD12_ERR_STAT_CRC_ERR         (1 << 2)
#define SDC_AUTO_CMD12_ERR_STAT_TIMEOUT_ERR     (1 << 1)
#define SDC_AUTO_CMD12_ERR_STAT_NOT_EXECUTED    (1 << 0)

// -------------------------------------------------------------------------------------------------
// REG_SDC_CPBLT
#define SDC_CPBLT_VOL_18V                       (1 << 26)
#define SDC_CPBLT_VOL_30V                       (1 << 25)
#define SDC_CPBLT_VOL_33V                       (1 << 24)
#define SDC_CPBLT_SUSPEND_RESUME                (1 << 23)
#define SDC_CPBLT_DMA                           (1 << 22)
#define SDC_CPBLT_HIGH_SPEED                    (1 << 21)
#define SDC_CPBLT_MAX_BLK_LEN_MASK              (3 << 16)
#define SDC_CPBLT_MAX_BLK_LEN_2048              (2 << 16)
#define SDC_CPBLT_MAX_BLK_LEN_1024              (1 << 16)
#define SDC_CPBLT_MAX_BLK_LEN_512               (0 << 16)
#define SDC_CPBLT_BASE_CLK_FREQ_MASK            (0x3f << 8)
#define SDC_CPBLT_TIMEOUT_CLK_UNIT              (1 << 7)
#define SDC_CPBLT_TIMEOUT_CLK_FREQ_MASK         (0x3f << 0)

// -------------------------------------------------------------------------------------------------
// REG_SDC_MAX_CURR_CPBLT
#define SDC_MAX_CURR_CPBLT_18V_MASK             (0xff << 16)
#define SDC_MAX_CURR_CPBLT_30V_MASK             (0xff << 8)
#define SDC_MAX_CURR_CPBLT_33V_MASK             (0xff << 0)

// -------------------------------------------------------------------------------------------------
// REG_SDC_SLOT_INT_STAT
#define SDC_SLOT_INT_STAT_SLOT(n)               ((1 << (n)) & 0xff)

// -------------------------------------------------------------------------------------------------
// REG_SDC_HOST_VER
#define SDC_HOST_VER_VEND_VER_MASK              (0xff << 8)
#define SDC_HOST_VER_SPEC_VER_MASK              (0xff << 0)

#define REG_NFC_SYS_CTRL                        (*(volatile unsigned long *)(REG_NFC_BASE+0x5c))
#define NFC_EMMC_SEL                                (1<<7)

#define EMMC_CARD_CID_DWORD_LENGTH		4
#define EMMC_CARD_CSD_DWORD_LENGTH		4
#define EMMC_CARD_EXT_CSD_DWORD_LENGTH		128

/* cmd6 is a little complex, please ref the spec to know what is what hear... */
#define EMMC_CMD6ARG(Access, Index, Value, CmdSet) ((((Access)&3)<<24)|(((Index)&0xff)<<16)|(((Value)&0xff)<<8)|(((CmdSet)&7)<<0))
#define EMMC_CMD6_ACCESS_WRITE		0x3
#define EMMC_CMD6_ACCESS_CLEAR		0x2
#define EMMC_CMD6_ACCESS_SET		0x1
#define EMMC_CMD6_ACCESS_CMD		0x0

/* eMMC response check */
/* for cmd1 response check */
#define EMMC_CMD1_SUPPORT_DUAL_VOLTAGE		(1<<7)
#define EMMC_CMD1_ADDR_MODE_FILTER		(0x3<<29)
#define EMMC_CMD1_ADDR_MODE_SECTOR		(0x2<<29)
#define EMMC_CMD1_CARD_IDLE_AFTER_PWR		(1<<31)

/* cmd1 arg, high capacity, larger than 2GB */
#define EMMC_CMD1_INTENDED_HIGH_CAPACITY	0x40000000
/* cmd1 arg, 3.3V */
#define EMMC_CMD1_INTENDED_HIGH_VOLTAGE		0x00FF8000
/* cmd1 arg, 1.8V */
#define EMMC_CMD1_INTENDED_LOW_VOLTAGE		0x00000080

/* response of cmd1, other values are not compliant with spec 4.51 */
#define EMMC_CMD1_RESP_HIGH_VOLTAGE		0x80FF8000
#define EMMC_CMD1_RESP_DUAL_VOLTAGE		0x80FF8080

/* 400KHz for init handshaking */
#define EMMC_INIT_BUS_CLK			400000

#define EMMC_ADMA_DESCRIPTOR_SIZE		8

/* our limitation */
#define EMMC_DMA_ALIGNMENT			4

#define EMMC_ADMA_ACT_NOP			(0<<4)
#define EMMC_ADMA_ACT_RSV			(1<<4)
#define EMMC_ADMA_ACT_TRAN			(2<<4)
#define EMMC_ADMA_ACT_LINK			(3<<4)

#define EMMC_ADMA_DESC_ATTR_VALID		(1<<0)
#define EMMC_ADMA_DESC_ATTR_END			(1<<1)
#define EMMC_ADMA_DESC_ATTR_INT			(1<<2)

#define EMMC_NORMAL_SPEED_MAX			26000000
#define EMMC_HIGH_SPEED_MAX			52000000
#define EMMC_HIGH_SPEED200_MAX			200000000

#define EMMC_DEVICE_EXT_CSD_BYTE_LEN		512
#define EMMC_DEVICE_HS200_FINE_TUNE_8BIT_LEN	128
#define EMMC_DEVICE_HS200_FINE_TUNE_4BIT_LEN	64

/*#define SDCLK_SOURCE_HIGH                       (208000000)*/
#define SDCLK_SOURCE_HIGH                       (800000000)
#define SDCLK_SOURCE_LOW                        (12000000)
#define SDCLK_MAX                               (SDCLK_SOURCE_HIGH/4)
#define EMMC_SECTOR_SIZE                	512
#define EMMC_DEV_ANY_STATUS_ERR                         (0xfdf90008)

#endif
