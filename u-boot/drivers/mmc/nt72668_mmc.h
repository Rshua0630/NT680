/*
 *  driver/mmc/nt72668_mmc.h
 *
 *  Author:	Alvin lin
 *  Created:	April 24, 2013
 *  Copyright:	Novatek Inc.
 *
 */

#ifndef __NT72668_MMC_H__
#define __NT72668_MMC_H__

#if 0
#include <asm/arch-nt72668/nt72668_stbc_regs.h>
#include <asm/arch-nt72668/nt72668_nfc_regs.h>
#include <asm/arch-nt72668/nt72668_mmc_regs.h>
#endif
#include <asm/arch-nt72656/nt72668_stbc_regs.h>
#include <asm/arch-nt72656/nt72668_nfc_regs.h>
#include <asm/arch-nt72656/mmc.h>

enum EMMC_DMA_BEAT_MODE { EMMC_DMA_BEAT_16_8_4=0x3, EMMC_DMA_BEAT_8_4=0x2, EMMC_DMA_BEAT_4=0x1, EMMC_DMA_BEAT_1=0x0 };
enum EMMC_BUS_WIDTH { EMMC_BW_1BIT=1, EMMC_BW_4BIT=4, EMMC_BW_8BIT=8, EMMC_BW_4BIT_DDR=0x14, EMMC_BW_8BIT_DDR=0x18 };
enum EMMC_DATA_LATCH { EMMC_NULL_LATACH, EMMC_SINGLE_LATCH, EMMC_DUAL_LATCH };
enum EMMC_SPEED_MODE { EMMC_NULL_SPEED_MODE, EMMC_LEGACY_SPEED, EMMC_HIGH_SPEED, EMMC_HS200_SPEED };
int nt72668_mmc_init(void);
#define CLK_26M	(26000000)
#define CLK_52M (52000000)
#define CLK_200M (200000000)
#define CLK_100M (100000000)

#endif
