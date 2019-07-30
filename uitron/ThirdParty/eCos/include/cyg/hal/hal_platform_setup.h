#ifndef CYGONCE_HAL_PLATFORM_SETUP_H
#define CYGONCE_HAL_PLATFORM_SETUP_H

#include <cyg/hal/var_io.h>
#include <cyg/hal/plf_io.h>
#include <cyg/hal/hal_mmu.h>

#include <cyg/hal/arch_defs.h>
#include <cyg/hal/config.h>
#include <cyg/hal/l2_defs.h>
#include <cyg/hal/scu_defs.h>

#define S_MPIDR_U               30
#define M_MPIDR_U               (0x1 << S_MPIDR_U)
#define S_MPIDR_CLUSTER_ID      8
#define M_MPIDR_CLUSTER_ID      (0xf << S_MPIDR_CLUSTER_ID)
#define S_MPIDR_CPU_ID          0
#define M_MPIDR_CPU_ID          (0x3 << S_MPIDR_CPU_ID)

# use CC_CPU2_CPU1_CMDBUF_REG1 to store cpu2 starting address
#define L1_CACHEABLE                0x00015de6
#define L1_NONCACHEABLE             0x00011de2
#define L1_DEVICE                   0x00000de6

#define CONFIG_ENABLE_MMU       1
#define CONFIG_ENABLE_ICACHE    1
#define CONFIG_ENABLE_DCACHE    1


#define CC_CPU2_CPU1_CMDBUF_REG1    0xF0090118
#define IOADDR_DRAM_REG_BASE        0xF0000000
#define IOADDR_DRAM2_REG_BASE       0xF0100000
#define SIZE_128MB                  0x8000000
#define DRAM1_MAX_SIZE              0x40000000
#define IOADDR_HVYLOAD_REG_BASE     0xF0008000

	.macro _setup1

    mrc     p15, 0, r0, c0, c0, 5   @ Read MPIDR
    ands    r0, r0, #M_MPIDR_CPU_ID
    CMP     r0, #0
    BEQ     init_core_0

    ldr     r0, =0xF0290000
    ldr     r1, =0x25
    str     r1, [r0]
    # main() will return firmware address at r0
    LDR     r1, =CC_CPU2_CPU1_CMDBUF_REG1
    LDR     r1, [r1]
    LDR     r0, [r1, #0x0]
    MOV     pc, r0
    nop

init_core_0:

	/* Set VBAR */
	ldr		r0, =__exception_handlers
	mcr		p15, 0, r0, c12, c0, 0

	/* Enable imprecise data aborts ,disable IRQ/FIQ and set SVC mode */
	cpsie	a
	cpsid	if, #K_CPSR_M_SVC

    /* Switch on the VFP and NEON hardware */
    mrc     p15, 0, r0, c1, c0, 2      @ Read Coprocessor Access Control Register (CPACR)
    orr     r0, r0, #(0xF << 20)       @ Enable access to CP 10 & 11(full access trm 4-185
    mcr     p15, 0, r0, c1, c0, 2      @ Write Coprocessor Access Control Register (CPACR)

    mov     r0, #0x40000000            @ bit[30] is enable bit
    vmsr    fpexc, r0                  @ Write FPEXC register, EN bit set


	/* Disable I/D cache, branch prefiction and MMU */
	mrc		p15, 0, r0, c1, c0, 0    @ Read CP15 System Control register
	bic		r0, r0, #M_SCTLR_I       @ Clear I bit 12 to disable I Cache
	bic		r0, r0, #M_SCTLR_Z       @ Clear Z bit 11 to disable Branch prediction
	bic		r0, r0, #M_SCTLR_C       @ Clear C bit  2 to disable D Cache
	bic     r0, r0, #M_SCTLR_A       @ Clear A bit  1 to disable strict alignment fault checking
	bic     r0, r0, #M_SCTLR_M       @ Clear M bit  0 to disable MMU
	mcr     p15, 0, r0, c1, c0, 0    @ Write CP15 System Control register


	/* Setup stack early for C function call, ARM arch init code will setup
	 * the same stack later */
	ldr     sp,.__startup_stack

	/* Clear BSS sections */
#if 0
clear_bss:
	ldr     r0, =__bss_start
	ldr     r1, =__bss_end
	mov     r2, #0
clbss_l:
	str     r2, [r0]
	add     r0, r0, #4
	cmp     r0, r1
	bne     clbss_l
#else
    # ==========================================================================
    # Initialize ZI and RW
    # ==========================================================================
    //BL      Init_Data
    ldr     r0,=IOADDR_HVYLOAD_REG_BASE
    ldr     r1,=_image_general_zi_zi_base
    str     r1, [r0, #0x64]
    mov     r2, #0x330
    str     r2, [r0, #0x60]
    ldr     r1,=_ecos_bss_size
    str     r1, [r0, #0x68]
    mov     r1, #0x0
    str     r1, [r0, #0x90]
    str     r1, [r0, #0x94]
    str     r1, [r0, #0x98]
    str     r1, [r0, #0x9C]
    orr     r2, r2, #0x1
    str     r2, [r0, #0x60]
#endif

    # Disable hardware management of data coherency with other cores in the cluster
	mrrc 	p15, 1, r0, r1, c15
	bic		r0, r0, #0x40
	mcrr	p15, 1, r0, r1, c15
	# Disable hardware management of data coherency with other cores in the cluster

invTlb:
    # ==========================================================================
    # Invalidate L1 Caches.
    # Invalidate Instruction cache.
    # ==========================================================================
    MOV     r1, #0
    MCR     p15, 0, r1, c7, c5, 0
    MCR     p15, 0, r1, c7, c5, 6
#    dsb
#    isb

    # ======================================================================== #
    # Invalidate L1 Data cache .                                               #
    # To make the code general purpose, calculate the                          #
    # cache size first and loop through each set + way.                        #
    # ======================================================================== #
    MRC     p15, 1, r0, c0, c0, 0           @ Read Cache Size ID. ccsidr
    LDR     r3, =0x1ff
    AND     r0, r3, r0, LSR #13             @ r0 = no. of sets - 1. @ L1 cache

    MOV     r1, #0                          @ r1 = way counter way_loop.
way_loop:
    MOV     r3, #0                          @ r3 = set counter set_loop.
set_loop:
    MOV     r2, r1, LSL #30
    ORR     r2, r3, LSL #6                  @ r2 = set/way cache operation format.
    MCR     p15, 0, r2, c7, c6, 2           @ Invalidate the line described by r2.
                                            @ DCISW Invalidate data cache line by set/way

    ADD     r3, r3, #1                      @ Increment set counter.
    CMP     r0, r3                          @ Last set reached yet?
    BGT     set_loop                        @ If not, iterate set_loop,
    ADD     r1, r1, #1                      @ else, next.
    CMP     r1, #4                          @ Last way reached yet?
    BNE     way_loop                        @ if not, iterate way_loop.

    # ======================================================================== #
    # Invalidate L2 Data cache .                                               #
    # To make the code general purpose, calculate the                          #
    # cache size first and loop through each set + way.                        #
    # ======================================================================== #
    MOV     r0, #2
    MCR     p15, 2, r0, c0, c0, 0           @ Set CSSELR cache select register
                                            @ Select to level2 cache


    MRC     p15, 1, r0, c0, c0, 0           @ Read Cache Size ID. ccsidr
                                            @ Cache Size ID Register
    LDR     r3, =0x1ff
    AND     r0, r3, r0, LSR #13             @ r0 = no. of sets - 1 @ L2 cache.
                                            @ here is 511

    MOV     r1, #0                          @ r1 = way counter way_loop_L2.
way_loop_L2:
    MOV     r3, #0                          @ r3 = set counter set_loop_L2.
set_loop_L2:
    MOV     r2, r1, LSL #28                 @ ws = (way << (32 - 4)) + level_type
    ADD     r2, r2, #2                      @ level_type 2 => L2 cache
    ORR     r2, r3, LSL #6                  @ r2 = set/way cache operation format.
                                            @ ws += _DC_LS => _DC_LS = r3 << 6 +=64
    MCR     p15, 0, r2, c7, c6, 2           @ Invalidate the line described by r2.
                                            @ DCISW Invalidate data cache line by set/way

    ADD     r3, r3, #1                      @ Increment set counter.
    CMP     r0, r3                          @ Last set reached yet?
    BGT     set_loop_L2                     @ If not, iterate set_loop_L2,
    ADD     r1, r1, #1                      @ else, next.
    CMP     r1, #4                          @ Last way reached yet?
    BNE     way_loop_L2                     @ if not, iterate way_loop_L2.


    # =========================================================================#
    # Invalidate TLB                                                           #
    # =========================================================================#
    MOV     r1, #0
    MCR     p15, 0, r1, c8, c7, 0           @ Invalidate entire unified TLB
    MCR     p15, 0, r1, c8, c6, 0           @ Invalidate entire data TLB
    MCR     p15, 0, r1, c8, c5, 0           @ Invalidate entire instruction TLB

    # =========================================================================#
    # Enable D-side Prefetch(HACTLR)trm4-194                                   #
    # =========================================================================#
    MRC p15, 0, r1, c1, c0, 1               @ Read Auxiliary Control Register.
    ORR r1, r1, #(0x1 <<2)                  @ Enable D-side prefetch.
    MCR p15, 0, r1, c1, c0, 1               @ Write Auxiliary Control Register.

    # =========================================================================#
    @ DSB causes completion of all cache maintenance operations appearing in   #
    @ program order before the DSB instruction.                                #
    @ An ISB instruction causes the effect of all branch predictor maintenance #
    @ operations before the ISB instruction to be visible to all instructions  #
    @ after the ISB instruction.                                               #
    @ Initialize PageTable.                                                    #
    # =========================================================================#


    ldr     r0,=IOADDR_HVYLOAD_REG_BASE
wait_hvy_load:
    ldr     r1, [r0, #0x60]
    AND     r2, r1, #0x1
    CMP     r2, #0
    BNE     wait_hvy_load



enable_mmu:
#if (CYGSEM_HAL_ENABLE_MMU_ON_STARTUP)
#	bl		hal_mmu_init
    # ==========================================================================
    # PAGE TABLE generation
    # ==========================================================================
    # ==========================================================================
    # Create cacheable dram area 0x00000000 ~ _PLATFORM_MEM_SZ_
    # SUB number = _PLATFORM_MEM_SZ_ / 1MB -1
    # ==========================================================================
    ldr     r0,=IOADDR_DRAM_REG_BASE
    ldr     r1,=SIZE_128MB
    ldr     r0, [r0]
    ubfx    r2, r0, #3, #1                  @ R2 = SDRAM_COUNT
    subs    r0, r0, #3
    ubfx    r0, r0, #0, #3                  @ R0 = SDRAM_CAPACITY
    lsl     r1, r1, r0                      @ Now r1 stores size of single DDR chip
    lsl     r1, r1, r2                      @ Now r1 stores summed size of all DDR chip
    ldr     r9,=_ttb                        @ r9 = TLB entry
#    ldr     r1,=_PLATFORM_MEM_SZ_           @ loop counter
    lsr     r1, r1, #20                     @ NT96687 = 512MB r1 = 512 words
    subs    r1, r1, #1
    mov     r4, r1                          @ backup r1 (loop counter) to r4
    ldr     r2,=(0x00000000 | L1_CACHEABLE)

init_ttb_dram_c1:
    add     r3, r2, r1, LSL#20              @ R3 now contains full level1 descriptor to write
    str     r3, [r9, r1, LSL#2]             @ Str table entry at TTB base + 0x800 + loopcount*4
    subs    r1, r1, #1                      @ Decrement loop counter
    bpl     init_ttb_dram_c1

    # ==========================================================================
    # Create cacheable dram area 0x40000000 ~ ARB2 size
    # SUB number = ARB2 size / 1MB - 1
    # ==========================================================================
    ldr     r2,=IOADDR_DRAM2_REG_BASE
    ldr     r1, [r2, #4]
    ubfx    r6, r1, #5, #1                  @ R6 = AUTO_REFRESH_CTRL
    cmp     r6, #0
    beq     init_ttb_dram2_c1_done          @ If AUTO_REFRESH_CTRL is 0, ARB2 is disabled, then skip it
    ldr     r1,=SIZE_128MB
    ldr     r2, [r2]
    subs    r2, r2, #3
    ubfx    r2, r2, #0, #3                  @ R0 = SDRAM_CAPACITY
    lsl     r1, r1, r2                      @ Now r1 stores size of single DDR chip (ARB2 only has 1 DDR)
#   ldr     r0,=_ttb
    orr     r0, r9, #0x1000
    lsr     r1, r1, #20
    subs    r1, r1, #1
    mov     r5, r1                          @ backup r1 (loop counter) to r5
    ldr     r2,=(0x40000000 | L1_CACHEABLE)

init_ttb_dram2_c1:
    add     r3, r2, r1, LSL#20              @ R3 now contains full level1 descriptor to write
    str     r3, [r0, r1, LSL#2]             @ Str table entry at TTB base + 0x800 + loopcount*4
    subs    r1, r1, #1                      @ Decrement loop counter
    bpl     init_ttb_dram2_c1
init_ttb_dram2_c1_done:

    # ==========================================================================
    # ie: Create non cacheable dram area 0x6000_0000 to 0xA000_0000
    # mapping to 0x0000_0000 ~ _PLATFORM_MEM_SZ_
    # page table starting addr = _ttb + addr (0x1800)
    # addr = (0x60000000 / 0x100000) * 4 = 0x1800
    # ==========================================================================
#   ldr     r0,=_ttb
    orr     r0, r9, #0x1800

    mov     r1, r4                          @ restore loop counter calculated in previous loop
    ldr     r2,=(0x00000000 | L1_NONCACHEABLE)
init_ttb_dram_c2:
    add     r3, r2, r1, LSL#20              @ R3 now contains full level1 descriptor to write
    str     r3, [r0, r1, LSL#2]             @ Str table entry at TTB base + 0x800 + loopcount*4
    subs    r1, r1, #1                      @ Decrement loop counter
    bpl     init_ttb_dram_c2

    # ==========================================================================
    # ie: Create non cacheable dram area 0xA000_0000 to 0xC000_0000 (ARB2)
    # mapping to 0x4000_0000 ~ ARB2 size
    # page table starting addr = _ttb + addr (0x2800)
    # ==========================================================================
    # R6 = AUTO_REFRESH_CTRL (stored in previous code)
    cmp     r6, #0
    beq     init_ttb_dram2_c2_done          @ If AUTO_REFRESH_CTRL is 0, ARB2 is disabled, then skip it
#   ldr     r0,=_ttb
    orr     r0, r9, #0x2800

    mov     r1, r5                          @ restore loop counter calculated in previous loop
    ldr     r2,=(0x40000000 | L1_NONCACHEABLE)
init_ttb_dram2_c2:
    add     r3, r2, r1, LSL#20              @ R3 now contains full level1 descriptor to write
    str     r3, [r0, r1, LSL#2]             @ Str table entry at TTB base + 0x800 + loopcount*4
    subs    r1, r1, #1                      @ Decrement loop counter
    bpl     init_ttb_dram2_c2
init_ttb_dram2_c2_done:


    # ==========================================================================
    # PAGE TABLE generation
    #
    # Build a flat translation table for the whole address space.
    # Create non cache Device from 0xE0000000 to 0xE0100000 total 1 1MB section
    # mapping to 0x8000_0000(ROM area)
    # page table starting addr = _ttb + addr (0x3C00)
    # addr = (0xF0000000 / 0x100000) * 4 = 0x3C00
    # ==========================================================================
#   ldr     r0,=_ttb
    orr     r0, r9, #0x3800
    ldr     r1,=0x0                        @ loop counter
    ldr     r2,=(0x80000000 | L1_NONCACHEABLE)

init_ttb_rom:
    add     r3, r2, r1, LSL#20              @ R3 now contains full level1 descriptor to write
    str     r3, [r0, r1, LSL#2]             @ Str table entry at TTB base + loopcount*4
    subs    r1, r1, #1                      @ Decrement loop counter
    bpl     init_ttb_rom

    # ==========================================================================
    # PAGE TABLE generation
    #
    # Build a flat translation table for the whole address space.
    # ie: Create Device from 0xF0000000 to FFFF0000 total 255 1MB section
    # page table starting addr = _ttb + addr (0x3C00)
    # addr = (0xF0000000 / 0x100000) * 4 = 0x3C00
    # ==========================================================================
#   ldr     r0,=_ttb
    orr     r0, r9, #0x3C00
    ldr     r1,=0xFE                        @ loop counter
    ldr     r2,=(0xF0000000 | L1_DEVICE)

init_ttb_so:
    add     r3, r2, r1, LSL#20              @ R3 now contains full level1 descriptor to write
    str     r3, [r0, r1, LSL#2]             @ Str table entry at TTB base + loopcount*4
    subs    r1, r1, #1                      @ Decrement loop counter
    bpl     init_ttb_so

    # ==========================================================================
    # Write the address of our page table base to TTBR0
    # Initialize MMU.
    # ==========================================================================
    mov     r0,#0x0
    mcr     p15, 0, r0, c2, c0, 2           @ TTBCR

#   ldr     r0,=_ttb
    mov     r1, #0x08                       @ RGN=b01 bit[4:3]  (outer cacheable write-back cached, write allocate)
                                            @ S=0      (translation table walk to non-shared memory)
    orr     r1,r1,#0x40                     @ IRGN=b01 (inner cacheability for the translation table walk is Write-back Write-allocate)

    orr     r0,r9,r1
    mcr     p15, 0, r0, c2, c0, 0

    # ==========================================================================
    # Set all domains as manager
    # ==========================================================================
    ldr     r0, =0xFFFFFFFF
    mcr     p15, 0, r0, c3, c0, 0


#endif
	mrc		p15, 0, r0, c1, c0, 0	@ Read CP15 System Control register
#	bic		r0, r0, #M_SCTLR_I		@ Clear I bit 12 to disable I Cache
#	bic		r0, r0, #M_SCTLR_C		@ Clear C bit  2 to disable D Cache
#	bic		r0, r0, #M_SCTLR_A		@ Clear A bit  1 to disable strict alignment fault checking
#if (CYGSEM_HAL_ENABLE_MMU_ON_STARTUP)
	orr		r0, r0, #M_SCTLR_M		@ Set M bit 0 to enable MMU before scatter loading
#endif
	mcr		p15, 0, r0, c1, c0, 0	@ Write CP15 System Control register
	isb
	dsb

    # Enable hardware management of data coherency with other cores in the cluster
	mrrc 	p15, 1, r0, r1, c15
	orr		r0, r0, #0x40
	mcrr	p15, 1, r0, r1, c15
	isb
	# Enable hardware management of data coherency with other cores in the cluster

enable_L1:
	# I-cache is controlled by bit 12
	# D-cache is controlled by bit 2

	mrc		p15, 0, r0, c1, c0, 0	@ Read CP15 register 1
#if (CYGSEM_HAL_ENABLE_ICACHE_ON_STARTUP)
	orr		r0, r0, #M_SCTLR_I		@ Enable I Cache
#endif
#if (CYGSEM_HAL_ENABLE_DCACHE_ON_STARTUP)
	orr		r0, r0, #M_SCTLR_C		@ Enable D Cache
#endif
	mcr		p15, 0, r0, c1, c0, 0	@ Write CP15 register 1
	dsb
	isb

	.endm

#define PLATFORM_SETUP1 _setup1

#endif // CYGONCE_HAL_PLATFORM_SETUP_H
