/*
 * Novatek NT72668 suspend/resume code.
 *
 */

#include <linux/linkage.h>
#include "../include/mach/debug.h"
#include "../include/mach/motherboard.h"

	.macro	senduart,wd,xx
	str	\wd, [\xx, #NT72668_UARTx_RBR]
	.endm

	.macro	busyuart,wd,xx
1002:	ldr	\wd, [\xx, #NT72668_UARTx_LSR]
	ands	\wd, \wd, #(NT72668_UART_LSR_TXEMPTY | \
			NT72668_UART_LSR_THREMPTY)
	beq	1002b
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
stop:
	b stop
	/*
	 * mapping SRAM to CPU side
	 */
	ldr     x1, =NT72668_IMVQ_REMAP_BASE
	ldr     w2,[x1]
	bic     w2,w2,#(1 << 0)
	bic     w2,w2,#(1 << 1)
	str     w2,[x1]
	/*
	 * copy execution code to SRAM
	 */
	adr     x1,power_off_start
	ldr	x0, =NT72668_IMVQ_SRAM_BASE
	mov     x4,x0
	adr     x2,power_off_end
copy_1:
	ldr     w3,[x1],#0x4
	str     w3,[x0],#0x4
	cmp     x1,x2
	bne     copy_1
	br	x4
/*
 * set DRAM enter self-refresh mode and request micom to power off
 */
power_off_start:
	adr     x1,power_off_tbl_addr
	adr     x5,power_off_tbl_addr_end
	adr     x2,power_off_tbl_value
out_val:
	ldr     w3,[x2],#0x4
	ldr     w4,[x1],#0x4
	str     w3,[x4]
	cmp     x1,x5
	bne     out_val

	adr	x1,NT72668_MAU0_STATUS_tbl
	ldr	w1,[x1]
mau0_busy:
	ldr     w2,[x1]
	and     w2,w2,#0x1
	mov     w3,#0x1
	cmp     w2,w3
	bne     mau0_busy

#ifndef CONFIG_ARCH_HAVE_NO_MAU1
	adr	x1,NT72668_MAU1_STATUS_tbl
	ldr	w1,[x1]
mau1_busy:
	ldr     w2,[x1]
	and     w2,w2,#0x1
	cmp     w2,w3
	bne     mau1_busy
#endif

	/* output successful message */
	adr	x3,NT72668_UART1_BASE_tbl
	ldr	w3,[x3]
	mov	x0,#0x40
	str	x0,[x3]
	str	x0,[x3]
	mov	x0,#13
	str	x0,[x3]
	mov	x0,#10
	str	x0,[x3]

#if defined(CONFIG_NVT_SUSPEND_MICOM_UART)
	mov	w0,#0x0
	mov	w6,#0x3
	adr     x2,UART_power_off_tbl_end
	adr     x3,NT72668_STBCUART_BASE_tbl
	ldr     w3,[x3]
retry_loop:
	adr     x1,UART_power_off_tbl_start
out_val2:
	ldr     w4,[x1],#0x4
	busyuart w5, x3
	senduart w4, x3
	cmp	w1,w2
	bne	out_val2
	add	w0,w0,#0x1
	cmp     w0,w6
	bne     retry_loop
#endif
#if defined(CONFIG_NVT_SUSPEND_MICOM_IPC)
	adr     x1,ipc_tbl_addr_start
	adr     x2,ipc_tbl_value_start
#if defined(CONFIG_NVT_SUSPEND_IPC_AUTO_POWERON)
	ldr     w3,[x2],#0x4
	ldr     w4,[x1],#0x4
	str     w3,[x4]

	ldr     w3,[x2],#0x4
	ldr     w4,[x1],#0x4
	movz	w5,#0xffff , LSL #16
cont_1:
	ldr     w0,[x4]
	ands    w0,w0,w5
	cmp     w0,w3
	bne     cont_1
#endif
#if defined(CONFIG_NVT_SUSPEND_IPC_SLEEP)
	ldr     w3,[x2],#0x4
	ldr     w4,[x1],#0x4
	str     w3,[x4]
#endif
#endif
	isb sy
	dsb sy
loop:
	wfi
	b loop

power_off_tbl_addr:
	/* set DDR enter self-refresh mode */
	.word 0xfd010000
	.word 0xfd0e0000
	.word 0xfd01014c
	.word 0xfd0e014c
	.word 0xfd010000
	.word 0xfd0e0000
	/* request micom to power off */
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
NT72668_MAU0_STATUS_tbl:
	.word NT72668_MAU0_STATUS
NT72668_MAU1_STATUS_tbl:
	.word NT72668_MAU1_STATUS
NT72668_UART1_BASE_tbl:
	.word NT72668_UART1_BASE
NT72668_STBCUART_BASE_tbl:
	.word NT72668_STBCUART_BASE
ENDPROC(nt72668_wait_for_die)

power_off_end:
	.word 0xff
