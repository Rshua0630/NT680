/*
 *  arch/arm/include/asm/mach-nvt72668/nvt72668_stbc.h
 *
 *  Author:	Z Y Chai
 *  Created:	Oct 10, 2013
 *  Copyright:	Novatek Inc.
 *
 */

#ifndef __NVT72668_STBC_H__
#define __NVT72668_STBC_H__

#include <linux/rtc.h>
#include "nvt_stbc_regs.h"

void nvt_soc_stbc_unlock_reg(void);
void nvt_soc_stbc_enable_wdog(unsigned short sec);
void nvt_soc_stbc_kick_wdog(void);
int nvt_soc_stbc_get_time(struct rtc_time *tm);
int nvt_soc_stbc_set_time(struct rtc_time *tm);
void nvt_soc_stbc_enable_wdog(unsigned short sec);
void nvt_soc_stbc_kick_wdog(void);

int nvt_soc_stbc_cmd(int cmd);
u8 nvt_stbc_read_reg(unsigned short reg);
int nvt_stbc_write_reg(u8 reg_val, unsigned short reg);
int nvt_stbc_stay_in_sram(void);
void nvt_stbc_reset(void);
void nvt_stbc_exit_idle(void);

struct nvt_stbc_hwplat {
	void __iomem *stbc_vbase;
	void __iomem *stbc_download;
	void __iomem *stbc_upload;
	void __iomem *stbc_int_stat;
	void __iomem *stbc_pattern;
	void __iomem *stbc_keypass;
	void __iomem *stbc_rtc_base_reg;
	void __iomem *stbc_rtc_sec_reg;
	void __iomem *stbc_rtc_day_reg;
	void __iomem *stbc_rtc_ctl_reg;
	void __iomem *stbc_rtc_switch_reg;
	void __iomem *stbc_bootstrap_reg;
};
#endif
