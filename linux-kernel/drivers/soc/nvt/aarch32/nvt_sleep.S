/*
 * Novatek NT72668 suspend/resume code.
 *
 */

#include <linux/linkage.h>
#include "mach/debug.h"
#include "mach/motherboard.h"

	.macro	senduart,rd,rx
	str	\rd, [\rx, #NT72668_UARTx_RBR]
	.endm

	.macro	busyuart,rd,rx
1002:	ldr	\rd, [\rx, #NT72668_UARTx_LSR]
	and	\rd, \rd, #(NT72668_UART_LSR_TXEMPTY | \
			NT72668_UART_LSR_THREMPTY)
	teq	\rd, #(NT72668_UART_LSR_TXEMPTY | NT72668_UART_LSR_THREMPTY)
	bne	1002b
	.endm

/*
 *
 * void nt72668_wait_for_die(void)
 *
 * This function performs the CPU power down sequence.
 */
.globl nt72668_wait_for_die
ENTRY(nt72668_wait_for_die)
	.balign 32, 0xe1a00000

	/*
	 * mapping SRAM to CPU side
	 */
	movw    r1,#NT72668_IMVQ_REMAP_BASE_LOW
	movt    r1,#NT72668_IMVQ_REMAP_BASE_HIGH
	ldr     r2,[r1]
	bic     r2,r2,#(1 << 0)
	bic     r2,r2,#(1 << 1)
	str     r2,[r1]
	/*
	 * copy execution code to SRAM
	 */
	adr     r1,power_off_start
	movw    r0,#NT72668_IMVQ_SRAM_BASE_LOW
	movt    r0,#NT72668_IMVQ_SRAM_BASE_HIGH
	mov     r4,r0
	adr     r2,power_off_end
copy_1:
	ldr     r3,[r1],#0x4
	str     r3,[r0],#0x4
	cmp     r1,r2
	bne     copy_1
	mov     pc,r4
/*
 * set DRAM enter self-refresh mode and request micom to power off
 */
power_off_start:
	adr     r1,power_off_tbl_addr
	adr     r5,power_off_tbl_addr_end
	adr     r2,power_off_tbl_value
out_val:
	ldr     r3,[r2],#0x4
	ldr     r4,[r1],#0x4
	str     r3,[r4]
	cmp     r1,r5
	bne     out_val

	movw	r1,#NT72668_MAU0_STATUS_LOW
	movt	r1,#NT72668_MAU0_STATUS_HIGH
mau0_busy:
	ldr     r2,[r1]
	and     r2,r2,#0x1
	mov     r3,#0x1
	cmp     r2,r3
	bne     mau0_busy

#ifndef CONFIG_ARCH_HAVE_NO_MAU1
	movw	r1,#NT72668_MAU1_STATUS_LOW
	movt	r1,#NT72668_MAU1_STATUS_HIGH
mau1_busy:
	ldr     r2,[r1]
	and     r2,r2,#0x1
	cmp     r2,r3
	bne     mau1_busy
#endif

	@ output successful message
	movw	r3,#NT72668_UART1_BASE_LOW
	movt	r3,#NT72668_UART1_BASE_HIGH
	mov	r0,#0x40
	str	r0,[r3]
	str	r0,[r3]
	mov	r0,#13
	str	r0,[r3]
	mov	r0,#10
	str	r0,[r3]

#if defined(CONFIG_NVT_SUSPEND_MICOM_UART)
	mov	r0,#0x0
	mov	r6,#0x3
	adr     r2,UART_power_off_tbl_end
	movw    r3,#NT72668_STBCUART_BASE_LOW
	movt    r3,#NT72668_STBCUART_BASE_HIGH
retry_loop:
	adr     r1,UART_power_off_tbl_start
out_val2:
	ldr     r4,[r1],#0x4
	busyuart r5, r3
	senduart r4, r3
	cmp	r1,r2
	bne	out_val2
	add	r0,r0,#0x1
	cmp     r0,r6
	bne     retry_loop
#endif
#if defined(CONFIG_NVT_SUSPEND_MICOM_IPC)
	adr     r1,ipc_tbl_addr_start
	adr     r2,ipc_tbl_value_start
#if defined(CONFIG_NVT_SUSPEND_IPC_AUTO_POWERON)
	ldr     r3,[r2],#0x4
	ldr     r4,[r1],#0x4
	str     r3,[r4]

	ldr     r3,[r2],#0x4
	ldr     r4,[r1],#0x4
	movw    r5,#0
	movt    r5,#0xffff
cont_1:
	ldr     r0,[r4]
	ands    r0,r0,r5
	cmp     r0,r3
	bne     cont_1
#endif
#if defined(CONFIG_NVT_SUSPEND_IPC_SLEEP)
	ldr     r3,[r2],#0x4
	ldr     r4,[r1],#0x4
	str     r3,[r4]
#endif
#endif
	isb
	dsb
loop:
	wfi
	b loop

power_off_tbl_addr:
	@ set DDR enter self-refresh mode
	.word 0xfd010000
	.word 0xfd0e0000
	.word 0xfd01014c
	.word 0xfd0e014c
	.word 0xfd010000
	.word 0xfd0e0000
	@ request micom to power off
#if defined(CONFIG_NVT_SUSPEND_MICOM_IPC)
	.word 0xfc040204
	.word 0xfc040204
	.word 0xfc040208
power_off_tbl_addr_end:
ipc_tbl_addr_start:
#if defined(CONFIG_NVT_SUSPEND_IPC_AUTO_POWERON)
	.word 0xfc040000
	.word 0xfc040004
#endif
#if defined(CONFIG_NVT_SUSPEND_IPC_SLEEP)
	.word 0xfc040000
#endif
#else
power_off_tbl_addr_end:
#endif
power_off_tbl_value:
	.word 0x00000005
	.word 0x00000005
	.word 0x003c00c0
	.word 0x003c00c0
	.word 0x8
	.word 0x8
#if defined(CONFIG_NVT_SUSPEND_MICOM_IPC)
	.word NT72668_IPC_KEY_PASS_0
	.word NT72668_IPC_KEY_PASS_1
	.word NT72668_IPC_KEY_PASS_2
ipc_tbl_value_start:
#if defined(CONFIG_NVT_SUSPEND_IPC_AUTO_POWERON)
	.word NT72668_IPC_AUTO_POWERON_REQ
	.word NT72668_IPC_AUTO_POWERON_RES
#endif
#if defined(CONFIG_NVT_SUSPEND_IPC_SLEEP)
	.word NT72668_IPC_SLEEP_REQ
#endif
#endif
#if defined(CONFIG_NVT_SUSPEND_MICOM_UART)
UART_power_off_tbl_start:
	.word 0xff
	.word 0xff
	.word MICOM_REQ_SLEEP
	.word 0x0
	.word 0x0
	.word 0x0
	.word 0x0
	.word 0x0
	.word MICOM_REQ_SLEEP
UART_power_off_tbl_end:
#endif
power_off_end:
ENDPROC(nt72668_wait_for_die)

ENTRY(nt72668_cpu_resume)
	@ Novatek resume entry point

	b	cpu_resume	@ jump to generic resume
ENDPROC(nt72668_cpu_resume)
