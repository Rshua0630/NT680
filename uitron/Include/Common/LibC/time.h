#ifndef _NVT_LIBC_TIME_H
#define _NVT_LIBC_TIME_H

#include <stddef.h>

// 1 ms= mini second = 10^-3 second
// 1 us= micro second = 10^-6 second
// 1 ns= nano second = 10^-9 second

// Each tick is equal to 1/CLOCKS_PER_SEC second
#define CLOCKS_PER_SEC  1000000L    //1 us
#define CLK_TCK         CLOCKS_PER_SEC

#define _CLK_T_         long
typedef _CLK_T_         clock_t;

#define _TIME_T_        long
typedef _TIME_T_        time_t;

struct tm {
	int   tm_sec;
	int   tm_min;
	int   tm_hour;
	int   tm_mday;
	int   tm_mon;
	int   tm_year;
	int   tm_wday;
	int   tm_yday;
	int   tm_isdst;
};

extern clock_t      clock(void);
extern time_t       time(time_t *timer);
extern struct tm   *localtime(const time_t *timer);
extern time_t       mktime(struct tm *timeptr);

extern void         time_set(time_t ct);

#endif
