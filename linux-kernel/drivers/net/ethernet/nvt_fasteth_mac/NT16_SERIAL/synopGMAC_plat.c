/**\file
 *  This file defines the wrapper for the platform/OS related functions
 *  The function definitions needs to be modified according to the platform
 *  and the Operating system used.
 *  This file should be handled with greatest care while porting the driver
 *  to a different platform running different operating system other than
 *  Linux 2.6.xx.
 * \internal
 * ----------------------------REVISION HISTORY-----------------------------
 * Synopsys			01/Aug/2007			Created
 */

#include "synopGMAC_plat.h"
#include <linux/delay.h>

/**
  * This is a wrapper function for Memory allocation routine. In linux Kernel
  * it it kmalloc function
  * @param[in] bytes in bytes to allocate
  */

void *plat_alloc_memory(unsigned long bytes)
{
	return kmalloc((size_t)bytes, GFP_KERNEL);
}

/*  jay hsu : memory alloc on system platform */
void *sys_plat_alloc_memory(unsigned long size)
{
	return kmalloc(size, GFP_KERNEL);
}

/*  jay hsu : memory free for system platform */
void sys_plat_free_memory(unsigned long *addr)
{
	/*  jay hsu :  need to add free function */
	kfree(addr);
}

/**
  * This is a wrapper function for Memory free routine. In linux Kernel
  * it it kfree function
  * @param[in] buffer pointer to be freed
  */
void plat_free_memory(void *buffer)
{
	kfree(buffer);
}

/**
  * This is a wrapper function for platform dependent delay
  * Take care while passing the argument to this function
  * @param[in] buffer pointer to be freed
  */
void plat_delay(u32 delay)
{
	while (delay--)
		;
}

