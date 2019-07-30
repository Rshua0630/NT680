#include <linux/jiffies.h>
#include <linux/random.h>
#include <linux/semaphore.h>
#include <linux/slab.h>
#include <linux/time.h>

#include "include/nt72_nand_config.h"
#include "include/nt72_nand_nfc.h"

inline void *nt72_nand_kapi_malloc(size_t size, gfp_t flags)
{
	return kmalloc(size, flags);
}

void nt72_nand_kapi_get_timestamp(u32 *s, u32 *us)
{
	struct timeval t;

	do_gettimeofday(&t);
	*s = (u32)t.tv_sec;
	*us = (u32)t.tv_usec;
}

u32 nt72_nand_kapi_rand32(void)
{
	u32 result;

	get_random_bytes(&result, sizeof(result));

	return result;
}

void nt72_nand_kapi_cpu_relax(void)
{
	cpu_relax();
}

unsigned long nt72_nand_kapi_timeout_init(void)
{
	return jiffies + REG_POLLING_TIMEOUT;
}

int nt72_nand_kapi_timeout_check(unsigned long stop_time)
{
	if (unlikely(time_after(jiffies, stop_time)))
		return 1;

	return 0;
}

void nt72_nand_kapi_lock_nfc(struct nt72_nand_info *info)
{
	down(&info->sem_nfc);
}

void nt72_nand_kapi_unlock_nfc(struct nt72_nand_info *info)
{
	up(&info->sem_nfc);
}
