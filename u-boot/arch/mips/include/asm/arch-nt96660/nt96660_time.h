#ifndef __NT96660_TIME_H__
#define __NT96660_TIME_H__
#include <common.h>

unsigned int nt96660_time_start;

static inline unsigned int getTM0Time(void)
{
	unsigned int val = 0;

	val = *((volatile unsigned int *)(0xc0040108));
	return val;
}

static inline unsigned int getTM0InitTime(void)
{
	return nt96660_time_start;
}

static inline void TM0Init(void)
{
	nt96660_time_start = 0;

	nt96660_time_start = *((volatile unsigned int *)(0xc0040108));
}
#endif
