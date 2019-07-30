/*
 *  arch/arm/mach-vexpress/include/mach/uncompress.h
 *
 *  Copyright (C) 2003 ARM Limited
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */

/* copy from arch\arm\boot\compressed\misc.c */
#include <linux/serial_reg.h>
#include <mach/hardware.h>

#ifdef CONFIG_DEBUG_NA51000_UART0
static volatile unsigned long * const UART = (unsigned long *)NVT_UART1_BASE_PHYS;
#else
static volatile unsigned long * const UART = (unsigned long *)NVT_UART2_BASE_PHYS;
#endif
static inline void putc(int c)
{
	while (!(UART[UART_LSR] & UART_LSR_THRE))
		barrier();

	UART[UART_TX] = c;
}
static inline void flush(void) {}
static inline void arch_decomp_setup(void) {}
