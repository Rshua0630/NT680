/*
	Header file for RTC controller register

	This file is the header file that define register for RTC module

	@file       rtc_reg.h
	Copyright   Novatek Microelectronics Corp. 2012.  All rights reserved.
*/

#ifndef _RTC_REG_H
#define _RTC_REG_H

#include "rcw_macro.h"

/*
	@addtogroup mIHALRTC
*/

/*RTC Timer Register*/
#define RTC_TIMER_REG_OFS 0x00
union RTC_TIMER_REG {
	uint32_t reg;
	struct {
	unsigned int sec:6;
	unsigned int min:6;
	unsigned int hour:5;
	unsigned int reserved0:15;
	} bit;
};

/*RTC Day & Key Register*/
#define RTC_DAYKEY_REG_OFS 0x04
union RTC_DAYKEY_REG {
	uint32_t reg;
	struct {
	unsigned int day:16;
	unsigned int key:4;
	unsigned int reserved0:12;
	} bit;
};

/*RTC Alarm Register*/
#define RTC_ALARM_REG_OFS 0x08
union RTC_ALARM_REG {
	uint32_t reg;
	struct {
	unsigned int sec:6;
	unsigned int min:6;
	unsigned int hour:5;
	unsigned int reserved0:15;
	} bit;
};

/*RTC Status Register*/
#define RTC_STATUS_REG_OFS 0x0C
union RTC_STATUS_REG {
	uint32_t reg;
	struct {
	unsigned int alarm_sts:1;
	unsigned int srst_sts:1;
	unsigned int ready:1;
	unsigned int reserved0:5;
	unsigned int cset_sts:1;
	unsigned int reserved1:23;
	} bit;
};

/*RTC Control Register*/
#define RTC_CTRL_REG_OFS 0x10
union RTC_CTRL_REG {
	uint32_t reg;
	struct {
	unsigned int alarm_inten:1;
	unsigned int srst:1;
	unsigned int cset:1;
	unsigned int time_sel:1;
	unsigned int day_sel:1;
	unsigned int key_sel:1;
	unsigned int pwralarmtime_sel:1;
	unsigned int pwralarmday_sel:1;
	unsigned int cset_inten:1;
	unsigned int reserved0:23;
	} bit;
};
/*Reserved (0x14)*/

// RTC Power Button Control Register
#define RTC_PWBC_REG_OFS 0x18
union RTC_PWBC_REG {
	uint32_t reg;
	struct {
	unsigned int reset_sdt_timer:1;
	unsigned int pwr_off:1;
	unsigned int reserved0:1;
	unsigned int pwralarm_en:1;
	unsigned int pwralarm_dis:1;
	unsigned int reserved1:19;
	unsigned int pwr_sw1_inten:1;
	unsigned int pwr_sw2_inten:1;
	unsigned int reserved2:6;
	} bit;
};

/*RTC Power Alarm Timer Register*/
#define RTC_PWRALM_REG_OFS 0x20
union RTC_PWRALM_REG {
	uint32_t reg;
	struct {
	unsigned int sec:6;
	unsigned int min:6;
	unsigned int hour:5;
	unsigned int reserved0:3;
	unsigned int day:5;
	unsigned int reserved1:7;
	} bit;
};

/*RTC OSC Analog Register*/
#define RTC_OSCAN_REG_OFS 0x24
union RTC_OSCAN_REG {
	uint32_t reg;
	struct {
	unsigned int osc_analogcfg:8;
	unsigned int reserved0:24;
	} bit;
};
#endif
