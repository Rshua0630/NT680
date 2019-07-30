/*
    Register definition header file for PWM module.

    This file is the header file that define the address offset and bit
    definition of registers of PWM module.

    @file       pwm_reg.h
    @ingroup    mIDrvIO_PWM
    @note       Nothing.

    Copyright   Novatek Microelectronics Corp. 2005.  All rights reserved.
*/

#ifndef _PWM_REG_H
#define _PWM_REG_H
#include <asm/arch/rcw_macro.h>
#include <asm/arch/IOAddress.h>
#include "nvt_type.h"
#include <asm/arch/pwm.h>

#define PWM_PWM0_CTRL_OFS	0x00  /* PWM0 Control Register */
#define PWM_PWM0_PRD_OFS	0x04  /* PWM0 Period Register */
#define PWM_PWM0_MS_EXT_OFS	0x04  /* PWM0 Micro step extend Register */

#define PWM_PWM1_CTRL_OFS	0x08  /* PWM1 Control Register */
#define PWM_PWM1_PRD_OFS	0x0C  /* PWM1 Period Register */
#define PWM_PWM1_MS_EXT_OFS	0x0C  /* PWM1 Micro step extend Register */

#define PWM_PWM2_CTRL_OFS	0x10  /* PWM2 Control Register */
#define PWM_PWM2_PRD_OFS	0x14  /* PWM2 Period Register */
#define PWM_PWM2_MS_EXT_OFS	0x14  /* PWM2 Micro step extend Register */

#define PWM_PWM3_CTRL_OFS	0x18  /* PWM3 Control Register */
#define PWM_PWM3_PRD_OFS	0x1C  /* PWM3 Period Register */
#define PWM_PWM3_MS_EXT_OFS	0x1C  /* PWM3 Micro step extend Register */

#define PWM_PWM4_CTRL_OFS	0x20  /* PWM4 Control Register */
#define PWM_PWM4_PRD_OFS	0x24  /* PWM4 Period Register */
#define PWM_PWM4_MS_EXT_OFS	0x24  /* PWM4 Micro step extend Register */

#define PWM_PWM5_CTRL_OFS	0x28  /* PWM5 Control Register */
#define PWM_PWM5_PRD_OFS	0x2C  /* PWM5 Period Register */
#define PWM_PWM5_MS_EXT_OFS	0x2C  /* PWM5 Micro step extend Register */

#define PWM_PWM6_CTRL_OFS	0x30  /* PWM6 Control Register */
#define PWM_PWM6_PRD_OFS	0x34  /* PWM6 Period Register */
#define PWM_PWM6_MS_EXT_OFS	0x34  /* PWM6 Micro step extend Register */

#define PWM_PWM7_CTRL_OFS	0x38  /* PWM7 Control Register */
#define PWM_PWM7_PRD_OFS	0x3C  /* PWM7 Period Register */
#define PWM_PWM7_MS_EXT_OFS	0x3C  /* PWM7 Micro step extend Register */

#define PWM_PWM8_CTRL_OFS	0x40  /* PWM8 Control Register */
#define PWM_PWM8_PRD_OFS	0x44  /* PWM8 Period Register */
#define PWM_PWM8_MS_EXT_OFS	0x44  /* PWM8 Micro step extend Register */

#define PWM_PWM9_CTRL_OFS	0x48  /* PWM9 Control Register */
#define PWM_PWM9_PRD_OFS	0x4C  /* PWM9 Period Register */
#define PWM_PWM9_MS_EXT_OFS	0x4C  /* PWM9 Micro step extend Register */

#define PWM_PWM10_CTRL_OFS	0x50  /* PWM10 Control Register */
#define PWM_PWM10_PRD_OFS	0x54  /* PWM10 Period Register */
#define PWM_PWM10_MS_EXT_OFS	0x54  /* PWM10 Micro step extend Register */

#define PWM_PWM11_CTRL_OFS	0x58  /* PWM11 Control Register */
#define PWM_PWM11_PRD_OFS	0x5C  /* PWM11 Period Register */
#define PWM_PWM11_MS_EXT_OFS	0x5C  /* PWM11 Micro step extend Register */

#define PWM_PWM12_CTRL_OFS	0x60  /* PWM12 Control Register */
#define PWM_PWM12_PRD_OFS	0x64  /* PWM12 Period Register */
#define PWM_PWM12_MS_EXT_OFS	0x64  /* PWM12 Micro step extend Register */

#define PWM_PWM13_CTRL_OFS	0x68  /* PWM13 Control Register */
#define PWM_PWM13_PRD_OFS	0x6C  /* PWM13 Period Register */
#define PWM_PWM13_MS_EXT_OFS	0x6C  /* PWM13 Micro step extend Register */

#define PWM_PWM14_CTRL_OFS	0x70  /* PWM14 Control Register */
#define PWM_PWM14_PRD_OFS	0x74  /* PWM14 Period Register */
#define PWM_PWM14_MS_EXT_OFS	0x74  /* PWM14 Micro step extend Register */

#define PWM_PWM15_CTRL_OFS	0x78  /* PWM15 Control Register */
#define PWM_PWM15_PRD_OFS	0x7C  /* PWM15 Period Register */
#define PWM_PWM15_MS_EXT_OFS	0x7C  /* PWM15 Micro step extend Register */

#define PWM_PWM16_CTRL_OFS	0xA0  /* PWM16 Control Register */
#define PWM_PWM16_PRD_OFS	0xA4  /* PWM16 Period Register */

#define PWM_PWM17_CTRL_OFS	0xA8  /* PWM17 Control Register */
#define PWM_PWM17_PRD_OFS	0xAC  /* PWM17 Period Register */

#define PWM_PWM18_CTRL_OFS	0xB0  /* PWM18 Control Register */
#define PWM_PWM18_PRD_OFS	0xB4  /* PWM18 Period Register */

#define PWM_PWM19_CTRL_OFS	0xB8  /* PWM19 Control Register */
#define PWM_PWM19_PRD_OFS	0xBC  /* PWM19 Period Register */


#define PWM_CTRL_REG_BUF_OFS	0x00 		/* PWM Control Register Buf */
union PWM_CTRL_REG_BUF {
	u32 reg;
	struct {
	unsigned int pwm_on:16;			/* PWM on cycle */
	unsigned int pwm_type:1;		/* 0: PWM ,1: Mirco step mode */
	unsigned int pwm_ms_dir:1;		/* Micro step moving direction 0: Counterclockwise 1: Clockwise */
	unsigned int reserved0:2;		/* Reserved bit 18~19 */
	unsigned int pwm_ms_threshold_en:1;	/* Mirco step degree threshold enable */
	unsigned int reserved1:3;		/* Reserved bit 21~23 */
	unsigned int pwm_ms_threshold:6;	/* Mirco step degree threshold value */
	unsigned int reserved2:2;		/* Reserved bit 30~31 */
	} bit;	
};

#define PWM_PERIOD_REG_BUF_OFS    0x04		/* PWM Period Register Buf */
union PWM_PERIOD_REG_BUF {
	u32 reg;
	struct {
	unsigned int pwm_r:8;			/* PWM rising time, the number of base clock cycles. */
	unsigned int pwm_f:8;			/* PWM falling time, the number of base clock cycles. */
	unsigned int pwm_prd:8;			/* PWM basis period, the number of clock cycles.  */
	unsigned int reserved0:4;		/* Reserved bit 24~27 */
	unsigned int pwm_inv:1;			/* Only used in PWM mode.0: Not invert; 1: PWM0 invert. */
	unsigned int reserved1:3;		/* Reserved bit 29~31 */
	} bit;
};

#define PWM_MS_EXT_REG_BUF_OFS    0x04		/* PWM micro step extern control register */
union PWM_MS_EXT_REG_BUF {
	u32 reg;
	struct {
	unsigned int pwm_ms_start_grads:9;	/* Micro step starting graduation */
	unsigned int reserved0:3;		/* Reserved bit 9~11 */
	unsigned int pwm_ms_step_per_unit:3;	/* Micro step step per unit */
	unsigned int reserved1:1;		/* Reserved bit 15 */
	unsigned int pwm_ms_grads_per_step:2;	/* Micro step graduations per step */
	unsigned int reserved2:14;		/* Reserved bit 17~31 */
	} bit;
};

#define PWM_CTRL_INTEN_OFS  0xE0		/* PWM interrupt Control Register, control PWMx_DONE_INTEN */
union PWM_CTRL_INTEN {
	u32 reg;
	struct {
	unsigned int pwm0_done_inten:1;
	unsigned int pwm1_done_inten:1;
	unsigned int pwm2_done_inten:1;
	unsigned int pwm3_done_inten:1;
	unsigned int pwm4_done_inten:1;
	unsigned int pwm5_done_inten:1;
	unsigned int pwm6_done_inten:1;
	unsigned int pwm7_done_inten:1;
	unsigned int pwm8_done_inten:1;
	unsigned int pwm9_done_inten:1;
	unsigned int pwm10_done_inten:1;
	unsigned int pwm11_done_inten:1;
	unsigned int pwm12_done_inten:1;
	unsigned int pwm13_done_inten:1;
	unsigned int pwm14_done_inten:1;
	unsigned int pwm15_done_inten:1;
	unsigned int pwm16_done_inten:1;
	unsigned int pwm17_done_inten:1;
	unsigned int pwm18_done_inten:1;
	unsigned int pwm19_done_inten:1;
	unsigned int reserved0:12;
	} bit;
};


#define PWM_MS_CTRL_INTEN_OFS  0xE4		/* Micro step interrupt Control Register, control MSx_DONE_INTEN */
union PWM_MS_CTRL_INTEN {
	u32 reg;
	struct {
	unsigned int ms0_done_inten:1;
	unsigned int ms1_done_inten:1;
	unsigned int ms2_done_inten:1;
	unsigned int ms3_done_inten:1;
	unsigned int ms4_done_inten:1;
	unsigned int ms5_done_inten:1;
	unsigned int ms6_done_inten:1;
	unsigned int ms7_done_inten:1;
	unsigned int ms8_done_inten:1;
	unsigned int ms9_done_inten:1;
	unsigned int ms10_done_inten:1;
	unsigned int ms11_done_inten:1;
	unsigned int ms12_done_inten:1;
	unsigned int ms13_done_inten:1;
	unsigned int ms14_done_inten:1;
	unsigned int ms15_done_inten:1;
	unsigned int reserved0:16;
	} bit;
};

#define PWM_CCNT_CTRL_INTEN_OFS  0xE8		/* CCNT  interrupt Control Register */
union PWM_CCNT_CTRL_INTEN {
	u32 reg;
	struct {
	unsigned int ccnt0_edge_inten:1;
	unsigned int ccnt0_trig_inten:1;
	unsigned int ccnt0_tout_inten:1;
	unsigned int reserved0:1;
	unsigned int ccnt1_edge_inten:1;
	unsigned int ccnt1_trig_inten:1;
	unsigned int ccnt1_tout_inten:1;
	unsigned int reserved1:1;
	unsigned int ccnt2_edge_inten:1;
	unsigned int ccnt2_trig_inten:1;
	unsigned int ccnt2_tout_inten:1;
	unsigned int reserved2:1;
	unsigned int ccnt3_edge_inten:1;
	unsigned int ccnt3_trig_inten:1;
	unsigned int ccnt3_tout_inten:1;
	unsigned int reserved3:17;
	} bit;
};

#define PWM_CLKDIV_LOAD_INTEN_OFS  0xEC		/* CLKDIV load done interrupt Control Register */
union PWM_CLKDIV_LOAD_INTEN {
	u32 reg;
	struct {
	unsigned int clkdiv_00_03_load_inten:1;
	unsigned int clkdiv_04_07_load_inten:1;
	unsigned int clkdiv_08_11_load_inten:1;
	unsigned int reserved0:13;
	unsigned int clkdiv_00_03_tgt_cnt_inten:1;
	unsigned int clkdiv_04_07_tgt_cnt_inten:1;
	unsigned int clkdiv_08_11_tgt_cnt_inten:1;
	unsigned int reserved1:13;
	} bit;
};


#define PWM_CTRL_INTSTS_OFS  0xF0		/* PWM interrupt status Register, PWMx_DONE_INTSTS */
union PWM_CTRL_INTSTS {
	u32 reg;
	struct {
	unsigned int pwm0_done_intsts:1;
	unsigned int pwm1_done_intsts:1;
	unsigned int pwm2_done_intsts:1;
	unsigned int pwm3_done_intsts:1;
	unsigned int pwm4_done_intsts:1;
	unsigned int pwm5_done_intsts:1;
	unsigned int pwm6_done_intsts:1;
	unsigned int pwm7_done_intsts:1;
	unsigned int pwm8_done_intsts:1;
	unsigned int pwm9_done_intsts:1;
	unsigned int pwm10_done_intsts:1;
	unsigned int pwm11_done_intsts:1;
	unsigned int pwm12_done_intsts:1;
	unsigned int pwm13_done_intsts:1;
	unsigned int pwm14_done_intsts:1;
	unsigned int pwm15_done_intsts:1;
	unsigned int pwm16_done_intsts:1;
	unsigned int pwm17_done_intsts:1;
	unsigned int pwm18_done_intsts:1;
	unsigned int pwm19_done_intsts:1;
	unsigned int reserved0:12;
	} bit;
};



#define PWM_MS_CTRL_INTSTS_OFS  0xF4		/* Micro step interrupt status Register, control MSx_DONE_INTSTS */
union PWM_MS_CTRL_INTSTS {
	u32 reg;
	struct {
	unsigned int ms0_done_intsts:1;
	unsigned int ms1_done_intsts:1;
	unsigned int ms2_done_intsts:1;
	unsigned int ms3_done_intsts:1;
	unsigned int ms4_done_intsts:1;
	unsigned int ms5_done_intsts:1;
	unsigned int ms6_done_intsts:1;
	unsigned int ms7_done_intsts:1;
	unsigned int ms8_done_intsts:1;
	unsigned int ms9_done_intsts:1;
	unsigned int ms10_done_intsts:1;
	unsigned int ms11_done_intsts:1;
	unsigned int ms12_done_intsts:1;
	unsigned int ms13_done_intsts:1;
	unsigned int ms14_done_intsts:1;
	unsigned int ms15_done_intsts:1;
	unsigned int reserved0:16;
	} bit;
};

#define PWM_CCNT_CTRL_INTSTS_OFS  0xF8		/* CCNT  interrupt status Register */
union PWM_CCNT_CTRL_INTSTS {
	u32 reg;
	struct {
	unsigned int ccnt0_edge_intsts:1;
	unsigned int ccnt0_trig_intsts:1;
	unsigned int ccnt0_tout_intsts:1;
	unsigned int reserved0:1;
	unsigned int ccnt1_edge_intsts:1;
	unsigned int ccnt1_trig_intsts:1;
	unsigned int ccnt1_tout_intsts:1;
	unsigned int reserved1:1;
	unsigned int ccnt2_edge_intsts:1;
	unsigned int ccnt2_trig_intsts:1;
	unsigned int ccnt2_tout_intsts:1;
	unsigned int reserved2:1;
	unsigned int ccnt3_edge_intsts:1;
	unsigned int ccnt3_trig_intsts:1;
	unsigned int ccnt3_tout_intsts:1;
	unsigned int reserved3:17;
	} bit;
};


#define PWM_CLKDIV_LOAD_STS_OFS  0xFC		/* Clock load done status */
union PWM_CLKDIV_LOAD_STS {
	u32 reg;
	struct {
	unsigned int pwm_00_03_clkdiv_load_done_intsts:1;
	unsigned int pwm_04_07_clkdiv_load_done_intsts:1;
	unsigned int pwm_08_11_clkdiv_load_done_intsts:1;
	unsigned int reserved0:13;
	unsigned int pwm_00_03_tgt_cnt_done_intsts:1;
	unsigned int pwm_04_07_tgt_cnt_done_intsts:1;
	unsigned int pwm_08_11_tgt_cnt_done_intsts:1;
	unsigned int reserved1:13;
	} bit;
};



#define PWM_ENABLE_OFS  0x100			/* PWM enable  register */
union PWM_ENABLE {
	u32 reg;
	struct {
	unsigned int pwm0_en:1;
	unsigned int pwm1_en:1;
	unsigned int pwm2_en:1;
	unsigned int pwm3_en:1;
	unsigned int pwm4_en:1;
	unsigned int pwm5_en:1;
	unsigned int pwm6_en:1;
	unsigned int pwm7_en:1;
	unsigned int pwm8_en:1;
	unsigned int pwm9_en:1;
	unsigned int pwm10_en:1;
	unsigned int pwm11_en:1;
	unsigned int pwm12_en:1;
	unsigned int pwm13_en:1;
	unsigned int pwm14_en:1;
	unsigned int pwm15_en:1;
	unsigned int pwm16_en:1;
	unsigned int pwm17_en:1;
	unsigned int pwm18_en:1;
	unsigned int pwm19_en:1;
	unsigned int reserved0:12;
	} bit;
};


#define PWM_DISABLE_OFS  0x104		/* PWM disable  register */
union PWM_DISABLE {
	u32 reg;
	struct {
	unsigned int pwm0_dis:1;
	unsigned int pwm1_dis:1;
	unsigned int pwm2_dis:1;
	unsigned int pwm3_dis:1;
	unsigned int pwm4_dis:1;
	unsigned int pwm5_dis:1;
	unsigned int pwm6_dis:1;
	unsigned int pwm7_dis:1;
	unsigned int pwm8_dis:1;
	unsigned int pwm9_dis:1;
	unsigned int pwm10_dis:1;
	unsigned int pwm11_dis:1;
	unsigned int pwm12_dis:1;
	unsigned int pwm13_dis:1;
	unsigned int pwm14_dis:1;
	unsigned int pwm15_dis:1;
	unsigned int pwm16_dis:1;
	unsigned int pwm17_dis:1;
	unsigned int pwm18_dis:1;
	unsigned int pwm19_dis:1;
	unsigned int reserved0:12;
	} bit;
};

#define PWM_LOAD_OFS  0x108		/* PWM load  register */
union PWM_LOAD {
	u32 reg;
	struct {
	unsigned int pwm0_load:1;
	unsigned int pwm1_load:1;
	unsigned int pwm2_load:1;
	unsigned int pwm3_load:1;
	unsigned int pwm4_load:1;
	unsigned int pwm5_load:1;
	unsigned int pwm6_load:1;
	unsigned int pwm7_load:1;
	unsigned int pwm8_load:1;
	unsigned int pwm9_load:1;
	unsigned int pwm10_load:1;
	unsigned int pwm11_load:1;
	unsigned int pwm12_load:1;
	unsigned int pwm13_load:1;
	unsigned int pwm14_load:1;
	unsigned int pwm15_load:1;
	unsigned int pwm16_load:1;
	unsigned int pwm17_load:1;
	unsigned int pwm18_load:1;
	unsigned int pwm19_load:1;
	unsigned int reserved0:12;
	} bit;
};

#define PWM_CLKDIV_LOAD_OFS  0x10C		/* PWM clock div load  register */
union PWM_CLKDIV_LOAD {
	u32 reg;
	struct {
	unsigned int pwm00_03_clkdiv_load:1;
	unsigned int pwm04_07_clkdiv_load:1;
	unsigned int pwm08_11_clkdiv_load:1;
	unsigned int reserved0:29;
	} bit;
};

#define PWM_MS_START_OFS  0x110			/* PWM micro step start register */
union PWM_MS_START {
	u32 reg;
	struct {
	unsigned int ms0_start:1;
	unsigned int ms1_start:1;
	unsigned int ms2_start:1;
	unsigned int ms3_start:1;
	unsigned int ms4_start:1;
	unsigned int ms5_start:1;
	unsigned int ms6_start:1;
	unsigned int ms7_start:1;
	unsigned int ms8_start:1;
	unsigned int ms9_start:1;
	unsigned int ms10_start:1;
	unsigned int ms11_start:1;
	unsigned int ms12_start:1;
	unsigned int ms13_start:1;
	unsigned int ms14_start:1;
	unsigned int ms15_start:1;
	unsigned int reserved0:16;
	} bit;
};


#define PWM_MS_STOP_OFS  0x114		/* PWM micro step stop register */
union PWM_MS_STOP {
	u32 reg;
	struct {
	unsigned int ms0_stop:1;
	unsigned int ms1_stop:1;
	unsigned int ms2_stop:1;
	unsigned int ms3_stop:1;
	unsigned int ms4_stop:1;
	unsigned int ms5_stop:1;
	unsigned int ms6_stop:1;
	unsigned int ms7_stop:1;
	unsigned int ms8_stop:1;
	unsigned int ms9_stop:1;
	unsigned int ms10_stop:1;
	unsigned int ms11_stop:1;
	unsigned int ms12_stop:1;
	unsigned int ms13_stop:1;
	unsigned int ms14_stop:1;
	unsigned int ms15_stop:1;
	unsigned int reserved0:16;
	} bit;
};

#define PWM_TGT_CNT_ENABLE_OFS  0x118		/* PWM target count enable register */
union PWM_TGT_CNT_ENABLE {
	u32 reg;
	struct {
	unsigned int pwm00_03_target_cnt_en:1;
	unsigned int pwm04_07_target_cnt_en:1;
	unsigned int pwm08_11_target_cnt_en:1;
	unsigned int reserved0:29;
	} bit;
};

#define PWM_TGT_CNT_DISABLE_OFS  0x11C		/* PWM target count disable register */
union PWM_TGT_CNT_DISABLE {
	u32 reg;
	struct {
	unsigned int pwm00_03_target_cnt_dis:1;
	unsigned int pwm04_07_target_cnt_dis:1;
	unsigned int pwm08_11_target_cnt_dis:1;
	unsigned int reserved0:29;
	} bit;
};


#define PWM_CCNT0_CONFIG_OFS             0x120
#define PWM_CCNT0_START_COUNT_VAL_OFS    0x124
#define PWM_CCNT0_TRIGGER_VAL_OFS        0x128
#define PWM_CCNT0_CONTROL_OFS            0x12C
#define PWM_CCNT0_CURRENT_VAL_OFS        0x130
#define PWM_CCNT0_EDGE_INTERVAL_OFS      0x134

#define PWM_CCNT1_CONFIG_OFS             0x140
#define PWM_CCNT1_START_COUNT_VAL_OFS    0x144
#define PWM_CCNT1_TRIGGER_VAL_OFS        0x148
#define PWM_CCNT1_CONTROL_OFS            0x14C
#define PWM_CCNT1_CURRENT_VAL_OFS        0x150
#define PWM_CCNT1_EDGE_INTERVAL_OFS      0x154

#define PWM_CCNT2_CONFIG_OFS             0x160
#define PWM_CCNT2_START_COUNT_VAL_OFS    0x164
#define PWM_CCNT2_TRIGGER_VAL_OFS        0x168
#define PWM_CCNT2_CONTROL_OFS            0x16C
#define PWM_CCNT2_CURRENT_VAL_OFS        0x170
#define PWM_CCNT2_EDGE_INTERVAL_OFS      0x174

#define PWM_CCNT3_CONFIG_OFS             0x180
#define PWM_CCNT3_START_COUNT_VAL_OFS    0x184
#define PWM_CCNT3_TRIGGER_VAL_OFS        0x188
#define PWM_CCNT3_CONTROL_OFS            0x18C
#define PWM_CCNT3_CURRENT_VAL_OFS        0x190
#define PWM_CCNT3_EDGE_INTERVAL_OFS      0x194

#define PWM_CCNT_CONFIG_OFS  0x120		/* Cycle Count Calculator */
union PWM_CCNT_CONFIG {
	u32 reg;
	struct {
	unsigned int ccnt_filter:8;		/* Cycle Count glitch filter register */
	unsigned int reserved0:8;		/* Reserved bit 8~15 */
	unsigned int ccnt_decrease:1;		/* Cycle Count increase or decrease */
	unsigned int ccnt_inv:1;		/* Cycle Count invert source signal */
	unsigned int ccnt_mode:1;		/* Cycle Count mode selection */
	unsigned int reserved1:1;		/* Reserved bit 19 */
	unsigned int ccnt_signal_source:1;	/* Cycle Count signal source selection */
	unsigned int reserved2:11;		/* Reserved bit 21~31 */
	} bit;
};

#define PWM_CCNT_START_COUNT_VAL_OFS  0x124	/* Cycle Count starting count value register */
union PWM_CCNT_START_COUNT_VAL {
	u32 reg;
	struct {
	unsigned int ccnt_sta_val:16;		/* Cycle Count starting count value */
	unsigned int reserved0:16;		/* Reserved bit 16~31 */
	} bit;
};

#define PWM_CCNT_TRIGGER_VAL_OFS  0x128		/* Cycle Count trigger value register */
union PWM_CCNT_TRIGGER_VAL {
	u32 reg;
	struct {
	unsigned int ccnt_trig_val:16;		/* Cycle Count trigger value register */
	unsigned int reserved0:16;		/* Reserved bit 16~31 */
	} bit;
};

#define PWM_CCNT_CONTROL_OFS  0x12C		/* Cycle Count controller reigster */
union PWM_CCNT_CONTROL {
	u32 reg;
	struct {
	unsigned int ccnt_time_en:1;		/* Cycle Count timeout enable bit */
	unsigned int reserved0:15;		/* Reserved bit 1~15 */
	unsigned int ccnt_time_cnt:16;		/* Cycle timeout count register  */
	} bit;
};

#define PWM_CCNT_CURRENT_VAL_OFS  0x130		/* Cycle Count current value reigster */
union PWM_CCNT_CURRENT_VAL {
	u32 reg;
	struct {
	unsigned int ccnt_current_val:16;	/* Cycle count current value */
	unsigned int reserved0:16;		/* Reserved bit 1~15 */
	} bit;
};

#define PWM_CCNT_EDGE_INTERVAL_OFS  0x134	/* Cycle Count edge interval register */
union PWM_CCNT_EDGE_INTERVAL {
	u32 reg;
	struct {
	unsigned int ccnt_current_val:16;	/* Cycle count edge interval value */
	unsigned int reserved0:16;		/* Reserved bit 1~15 */
	} bit;
};


#define PWM_CCNT_ENABLE_OFS  0x200		/* Cycle Count enable  register */
union PWM_CCNT_ENABLE {
	u32 reg;
	struct {
	unsigned int ccnt0_en:1;
	unsigned int ccnt1_en:1;
	unsigned int ccnt2_en:1;
	unsigned int ccnt3_en:1;
	unsigned int reserved0:28;
	} bit;
};

#define PWM_CCNT_LOAD_OFS  0x204		/* Cycle Count load  register */
union PWM_CCNT_LOAD {
	u32 reg;
	struct {
	unsigned int ccnt0_load:1;
	unsigned int ccnt1_load:1;
	unsigned int ccnt2_load:1;
	unsigned int ccnt3_load:1;
	unsigned int reserved0:28;
	} bit;
};

#define PWM_TGT_CNT_REG0_OFS  0x210		/* PWM target count register 0*/
union PWM_TGT_CNT_REG0 {
	u32 reg;
	struct {
	unsigned int pwm00_03_target_cnt:16;
	unsigned int pwm04_07_target_cnt:16;
	} bit;
};

#define PWM_TGT_CNT_REG1_OFS  0x214		/* PWM target count register 1*/
union PWM_TGT_CNT_REG1 {
	u32 reg;
	struct {
	unsigned int pwm08_11_target_cnt:16;
	unsigned int reserved0:16;
	} bit;
};

#define PWM0_EXPEND_PERIOD_REG_BUF_OFS  0x230
#define PWM1_EXPEND_PERIOD_REG_BUF_OFS  0x234
#define PWM2_EXPEND_PERIOD_REG_BUF_OFS  0x238
#define PWM3_EXPEND_PERIOD_REG_BUF_OFS  0x23C
#define PWM4_EXPEND_PERIOD_REG_BUF_OFS  0x240
#define PWM5_EXPEND_PERIOD_REG_BUF_OFS  0x244
#define PWM6_EXPEND_PERIOD_REG_BUF_OFS  0x248
#define PWM7_EXPEND_PERIOD_REG_BUF_OFS  0x24C
union PWM_EXPEBD_PERIOD_REG_BUF {
	u32 reg;
	struct {
	unsigned int pwm_r:8;			/* PWM rising time, the number of base clock cycles. */
	unsigned int pwm_f:8;			/* PWM falling time, the number of base clock cycles. */
	unsigned int pwm_prd:8;			/* PWM basis period, the number of clock cycles.  */
	unsigned int reserved0:8;
	} bit;
};


#define PWM_DEBUG_PORT_OFS  0x3FC
union PWM_DEBUG_PORT {
	u32 reg;
	struct {
	unsigned int debug_sel:4;
	unsigned int reserved0:28;
	} bit;
};


#endif


