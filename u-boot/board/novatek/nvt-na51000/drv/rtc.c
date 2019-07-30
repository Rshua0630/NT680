/*
	rtc init driver.

	This file is the driver of rtc init.

	@file		rtc.c
	@ingroup
	@note		Nothing.

	Copyright   Novatek Microelectronics Corp. 2017.  All rights reserved.
*/

#include <asm/arch/rcw_macro.h>
#include <asm/arch/IOAddress.h>
#include <common.h>

#ifdef CONFIG_NVT_DRTC
#define CG_GETREG(ofs)	INW(IOADDR_CG_REG_BASE+(ofs))
#define CG_SETREG(ofs, value)	OUTW(IOADDR_CG_REG_BASE +(ofs),(value))
#define RTC_GETREG(ofs)	INW(IOADDR_DRTC_REG_BASE+(ofs))
#define RTC_SETREG(ofs, value)	OUTW(IOADDR_DRTC_REG_BASE +(ofs),(value))
#define REG_RTC_DAY_KEY 0x4
#define REG_RTC_CTRL 0x10
#define TIME_EN 0x10
#define DEFAULT_YEAR 0x8EAC
#define BASE_YEAR 1900
#define BASE_MONTH 1
#define BASE_DAY 1
#define DRTC_CG_OFS 0x90
#define DRTC_CG_RST_OFS 0xB0
#define DRTC_CG_EN 0x400000
#define DRTC_CG_RST 0x400000
#else
#define RTC_GETREG(ofs)	INW(IOADDR_RTC_REG_BASE+(ofs))
#define RTC_SETREG(ofs, value)	OUTW(IOADDR_RTC_REG_BASE +(ofs),(value))

/*Check value*/
#define DEFAULT_YEAR 0x8EAC
#define RTC_INT_KEY 0x20000
#define RTC_INT_KEY_MASK 0x70000
#define RTC_INT_KEY_POR 0x10000
#define RTC_INT_OSC_ANALOG_CFG 0x9
#define BASE_YEAR 1900
#define BASE_MONTH 1
#define BASE_DAY 1

/*Register*/
#define REG_RTC_DAY_KEY 0x4
#define REG_RTC_STS 0x0C
#define REG_RTC_CTRL 0x10
#define REG_RTC_OSC_ANAG 0x24

/*Status register bits*/
#define RTC_STS_SRST  (0x02)

/*Register bits*/
#define RTC_SWRST 0x2
#define DAY_SEL 0x10
#define KEY_PWRALMDAY_SEL 0xA0
#define CSET_CTRL 0x4
#endif

static const u32 rtc_day_of_year[2] = {365, 366};

static const u8 rtc_day_of_month[2][12] = {
	/* Not leap year */
	{31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31},
	/* Leap year */
	{31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31}
};


static BOOL rtc_is_leap_year(u32 year)
{
	return (BOOL)((((year % 4) == 0) && (((year % 100) != 0) || ((year % 400) == 0))));
}

static u32 rtc_convertdate(u32 year, int month, int day)
{
	u32 i = 0;
	u32 ret_day = 0;

	/* Check parameter */
	if ((month == 0) || (month > 12) || (day == 0) || \
		(day > rtc_day_of_month[rtc_is_leap_year(year)][month - 1])) {
		printf("Invalid date, set to default\n");
		return DEFAULT_YEAR;
	}

	for (i = BASE_YEAR; i < year; i++)
		ret_day += rtc_day_of_year[rtc_is_leap_year(i)];

	// Month
	for (i = BASE_MONTH; i < month; i++)
		ret_day += rtc_day_of_month[rtc_is_leap_year(year)][i - 1];

	ret_day += day - BASE_DAY;

	return ret_day;
}

#ifndef CONFIG_NVT_DRTC
void nvt_rtc_init(void)
{
	u32 daykey = RTC_GETREG(REG_RTC_DAY_KEY);
	u32 key = daykey & 0xF0000;
	u32 ctrl = 0, year = 0, status = 0;
	u32 configured_year = rtc_convertdate(RTC_YEAR, RTC_MONTH, RTC_DAY);

	if ((key & RTC_INT_KEY_POR) || ((key & RTC_INT_KEY_MASK) != RTC_INT_KEY)) {
		printf("Power lost, First bootup\n");

		RTC_SETREG(REG_RTC_CTRL, RTC_SWRST);
		do {
			ctrl = RTC_GETREG(REG_RTC_CTRL);
		} while (ctrl & RTC_SWRST);
		do {
			status = RTC_GETREG(REG_RTC_STS);
		} while (status & RTC_STS_SRST);

		RTC_SETREG(REG_RTC_CTRL, KEY_PWRALMDAY_SEL);
		__asm__ __volatile__("dsb\n\t");
		RTC_SETREG(REG_RTC_DAY_KEY, RTC_INT_KEY);
		RTC_SETREG(REG_RTC_OSC_ANAG, RTC_INT_OSC_ANALOG_CFG);
		RTC_SETREG(REG_RTC_CTRL, KEY_PWRALMDAY_SEL | CSET_CTRL);
		do {
			ctrl = RTC_GETREG(REG_RTC_CTRL);
		} while (ctrl & CSET_CTRL);

		daykey = RTC_GETREG(REG_RTC_DAY_KEY);
		year = daykey & 0xFFFF;

		if (year < configured_year) {
			year = configured_year;
			RTC_SETREG(REG_RTC_CTRL, DAY_SEL);
			RTC_SETREG(REG_RTC_DAY_KEY, (daykey | year));
			RTC_SETREG(REG_RTC_CTRL, DAY_SEL | CSET_CTRL);
		}
	}
}
#else
void nvt_rtc_init(void)
{
	u32 configured_year = rtc_convertdate(RTC_YEAR, RTC_MONTH, RTC_DAY);
	u32 ctrl = RTC_GETREG(REG_RTC_CTRL);
	u32 cg_drtc = CG_GETREG(DRTC_CG_OFS);
	u32 cg_drtc_rst = CG_GETREG(DRTC_CG_RST_OFS);

	CG_SETREG(DRTC_CG_RST_OFS, DRTC_CG_RST | cg_drtc_rst);
	CG_SETREG(DRTC_CG_OFS, DRTC_CG_EN | cg_drtc);
	RTC_SETREG(REG_RTC_CTRL, TIME_EN | ctrl);
	RTC_SETREG(REG_RTC_DAY_KEY, configured_year);
}
#endif