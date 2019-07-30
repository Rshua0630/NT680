/**
    NVT sleep function
    To handle CPU context save/restore during suspend/resume stage
    @file sleep.S
    @ingroup
    @note
    Copyright Novatek Microelectronics Corp. 2017. All rights reserved.

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License version 2 as
    published by the Free Software Foundation.
*/


#include <linux/linkage.h>
#include <asm/assembler.h>
#include <mach/hardware.h>

#if	defined(CONFIG_DEBUG_NA51000_UART0)
#define	NVT_PM_UART_BASE_VIRT	NVT_UART1_BASE_VIRT
#define NVT_PM_UART_BASE_PHYS	NVT_UART1_BASE_PHYS
#elif	defined(CONFIG_DEBUG_NA51000_UART2)
#define	NVT_PM_UART_BASE_VIRT	NVT_UART3_BASE_VIRT
#define NVT_PM_UART_BASE_PHYS	NVT_UART3_BASE_PHYS
#elif	defined(CONFIG_DEBUG_NA51000_UART3)
#define	NVT_PM_UART_BASE_VIRT	NVT_UART4_BASE_VIRT
#define NVT_PM_UART_BASE_PHYS	NVT_UART4_BASE_PHYS
#else
#define	NVT_PM_UART_BASE_VIRT	NVT_UART2_BASE_VIRT
#define NVT_PM_UART_BASE_PHYS	NVT_UART2_BASE_PHYS
#endif

	.text
	.align 5

ENTRY(na51000_print_char_S)
	ldr	r4, =NVT_PM_UART_BASE_VIRT
	ldr	r5, =0x53
	str	r5, [r4]
	mov	pc, lr
ENDPROC(na51000_print_char_S)

ENTRY(na51000_print_char_R)
	ldr	r4, =NVT_PM_UART_BASE_PHYS
	ldr	r5, =0x52
	str	r5, [r4]
	mov	pc, lr
ENDPROC(na51000_print_char_R)

/* To enable smp cache coherency bit */
ENTRY(na51000_cpuectlr_smpen)
	/* Disable I cache */
	mrc	p15, 0, r4, c1, c0, 0
	bic	r4, r4, #0x1000
	mcr     p15, 0, r4, c1, c0, 0
	nop

	/* Disable D cache */
	mrc	p15, 0, r4, c1, c0, 0
	bic	r4, r4, #0x4
	mcr     p15, 0, r4, c1, c0, 0
	nop


	/* invalidate icache */
	mcr	p15, 0, r4, c7, c5, 0
	mcr	p15, 0, r4, c7, c5, 6
	mcr	p15, 0, r4, c7, c10, 4		@ cp15dsb
	mcr	p15, 0, r4, c7, c5, 4		@ cp15isb

	/* invalidate dcache */
	mov	r8, lr
	bl	v7_invalidate_l1
	mov	lr, r8

	/* Enable SMP coherency bit */
	mrrc	p15, 1, r4, r5, c15
	orr	r4, r4, #0x40
	mcrr	p15, 1, r4, r5, c15
	isb

	mrrc	p15, 1, r4, r5, c15
	tst	r4, #0x40
	bne	1f
	ldr	r4, =NVT_PM_UART_BASE_PHYS
	ldr	r5, =0x44			@ Print D if not smp en=1
	str	r5, [r4]
	mov	pc, lr
1:
	ldr	r4, =NVT_PM_UART_BASE_PHYS
	ldr	r5, =0x43			@ Print C if smp en=1
	str	r5, [r4]
	mov	pc, lr
ENDPROC(na51000_cpuectlr_smpen)

ENTRY(na51000_cpu_suspend)
	@ Call ipc to shutdown core2
	ldr	r4, =NVT_CC_BASE_VIRT
	add	r5, r4, #0x118
	ldr	r6, =0xffff0000
	str	r6, [r5]
	add	r5, r4, #0x11c
	ldr	r6, =9
	str	r6, [r5]
	add	r5, r4, #0x24
	ldr	r6, =0x10000
	str	r6, [r5]
	add	r5, r4, #0x20
	ldr	r6, =1
	str	r6, [r5]

	@ waiting for power off
	dsb
	isb
	bl	na51000_print_char_S		@ Print char S
	wfi
ENDPROC(na51000_cpu_suspend)

/*
 * ============================
 * == Core2 resume entry point ==
 * ============================
 *
 * void na51000_cpu_resume(void)
 *
 * Loader will jump to this function while waking up from CPU
 * OFF state. Physical address of the function is
 * stored in the RTC register in CPU OFF mode.
 */
ENTRY(na51000_cpu_resume)
	bl	na51000_print_char_R		@ Print char R
	bl	na51000_cpuectlr_smpen
	b	cpu_resume			@ Jump to generic resume
ENDPROC(na51000_cpu_resume)
