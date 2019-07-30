/* Memory sub-system initialization code */

#include <config.h>
#include <asm/regdef.h>
#include <asm/mipsregs.h>

#include <asm/arch/nt96660_regs.h>

	.text
	.set noreorder

	.globl	lowlevel_init
lowlevel_init:
	/*
************************************************************************
* mapping 0xc0000000 via TLB
************************************************************************
*/
    mtc0      $0, $2 # Clr EntryLo0
    mtc0        $0, $3 # Clr EntryLo1
    mtc0       $0, $5 # Clr PageMask
    mtc0       $0, $6 # Clr Wired

    lui     $15,0x1fff
    ori    $15,$15,0xe000 # $15=0x1fffe000
    mtc0       $15,$5,0 # Write to PageMask[28:13] set page size as 256MB

    # write EntryLo0($2) ,PFN=0xC0000,C=3, D=1,V=1,G=1
    lui     $15,0x0300
    ori    $15,$15,0x0017 # $15=0x0300001f, cache=3 (0x001f), uncache=2 (0x0017)
#    ori         $15,$15,0x003f # $15=0x0300001f, cache=3 (0x001f), uncache=2 (0x0017), uncache accelerated=7 (0x003f)
    mtc0       $15,$2,0

    # write EntryLo1($3) ,PFN=0xD0000,C=3, D=1,V=1,G=1
    lui     $15,0x0340
    ori    $15,$15,0x0017 # $15=0x0140001f, cache=3 (0x001f), uncache=2 (0x0017)
    mtc0       $15,$3,0

    # write EntryHi ,VPN=0, VPN2X=0, ASID=0;
    lui     $15, 0xC000
    mtc0       $15, $10, 0

    # set index register = 0
    mtc0 $0, $0, 0
    # write to TLB
    ssnop
    ssnop
    tlbwi # Update TLB register
    nop
/*
************************************************************************
* mapping 0xc0000000 via TLB
************************************************************************
*/
	jr	ra
	nop
