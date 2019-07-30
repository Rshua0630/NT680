/**
    Header file for RTC module.

    This file is the header file that define the API and data type
    for RTC module.

    @file       rtc.h
    @ingroup    miDrvTimer_RTC
    @note       Nothing.

    Copyright   Novatek Microelectronics Corp. 2012.  All rights reserved.
*/

#ifndef _DRTC_H
#define _DRTC_H

#include "Type.h"
#include "Driver.h"

/**
    @addtogroup miDrvTimer_RTC
*/
//@{

/**
    RTC Date
*/
typedef union {
	// Format conformed to RTC register
	_PACKED_BEGIN struct {
		UBITFIELD   day: 5;     ///< Day
		UBITFIELD   month: 4;   ///< Month
		UBITFIELD   year: 13;   ///< Year
	} _PACKED_END s;
	UINT32      value;          ///< Date value all together Y:M:D
} DRTC_DATE, *PDRTC_DATE;

/**
    RTC Time
*/
typedef union {
	// Format conformed to RTC register
	_PACKED_BEGIN struct {
		UBITFIELD   second: 6;  ///< Second
		UBITFIELD   rsv0: 2;
		UBITFIELD   minute: 6;  ///< Minute
		UBITFIELD   rsv1: 2;
		UBITFIELD   hour: 5;    ///< Hour
	} _PACKED_END s;
	UINT32      value;          ///< Time value all together H:M:S
} DRTC_TIME, *PDRTC_TIME;

/**
    DRTC Alarm Period
*/
typedef enum {
	DRTC_ALARM_PERIOD_MINUTE = 0x1,       ///< alarm happens every minute
	DRTC_ALARM_PERIOD_HOUR   = 0x2,       ///< alarm happens every hour
	DRTC_ALARM_PERIOD_DAY    = 0x4,       ///< alarm happens every day
	DRTC_ALARM_PERIOD_ONCE   = 0x8,       ///< alarm happens only once
} DRTC_ALARM_PERIOD, *PDRTC_ALARM_PERIOD;

/**
    DRTC Alarm Switch
*/
typedef enum {
	DRTC_ALARM_DISABLE,      ///< disable alarm
	DRTC_ALARM_ENABLE,       ///< enable alarm
} DRTC_ALARM_SWITCH, *PDRTC_ALARM_SWITCH;

/**
    DRTC Configuration ID

    DRTC Configuration ID for drtc_set_config() and rtc_get_config().

*/
typedef enum {
	DRTC_CONFIG_ID_ALARM_PERIOD,    ///< Configuration for alarm period.
	///< Could be read and write of the following value:
	///< - @b DRTC_ALARM_PERIOD_MINUTE : alarm happens every minute
	///< - @b DRTC_ALARM_PERIOD_HOUR   : alarm happens every hour
	///< - @b DRTC_ALARM_PERIOD_DAY    : alarm happens every day
	///< - @b DRTC_ALARM_PERIOD_YEAR   : alarm happens every year
	DRTC_CONFIG_ID_INIT_DIV,        ///< Clock divsion of 12MHz source for initial pulse
	///< Could be read and write of the following value:
	///< - @i 0~0xFF 8 bits MSB over 24 bits division
	DRTC_CONFIG_ID_ALARM_EN,        ///< Alarm enable
	///< - @b DRTC_ALARM_ENABLE  : enable alarm
	///< - @b DRTC_ALARM_DISABLE : disable alarm
	ENUM_DUMMY4WORD(DRTC_CONFIG_ID)
} DRTC_CONFIG_ID, *PDRTC_CONFIG_ID;

// Public APIs

// General
extern ER               drtc_open(void);
extern void             drtc_reset(void);

extern void             drtc_set_config(DRTC_CONFIG_ID config_id, UINT32 uiconfig);
extern UINT32           drtc_get_config(DRTC_CONFIG_ID config_id);

// Date, time
extern void             drtc_set_basedate(UINT32 uiyear, UINT32 uimonth, UINT32 uiday);
extern DRTC_DATE        drtc_get_basedate(void);

extern void             drtc_set_maxdate(UINT32 uiyear, UINT32 uimonth, UINT32 uiday);
extern DRTC_DATE        drtc_get_maxdate(void);

extern void             drtc_set_read_timeoffset(INT32 ihour, INT32 iminute);

extern ER               drtc_set_date(UINT32 uiyear, UINT32 uimonth, UINT32 uiday);
extern DRTC_DATE        drtc_get_date(void);

extern ER               drtc_set_time(UINT32 uihour, UINT32 uiminute, UINT32 uisecond);
extern DRTC_TIME        drtc_get_time(void);

extern UINT32           drtc_convert_date2days(DRTC_DATE DRTC_DATE);
extern DRTC_DATE        drtc_convert_days2date(UINT32 uidays);
extern BOOL             drtc_isleapyear(UINT32 uiyear);

// Alarm
extern ER               drtc_set_alarm(DRTC_DATE drtcdate, DRTC_TIME drtctime, DRV_CB eventhandler);
extern void             drtc_get_alarm(PDRTC_DATE pdrtcdate, PDRTC_TIME pdrtctime);

//@}

#endif
