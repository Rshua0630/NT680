
#include <config.h>
#include <asm/addrspace.h>
#include <asm/regdef.h>
#include <asm/mipsregs.h>

    .text
    .set noreorder
    .set mips32

    .globl  lowlevel_init
lowlevel_init:

    jr  ra
    nop
