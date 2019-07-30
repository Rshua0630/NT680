/*
 * (C) Copyright 2003
 * Wolfgang Denk Engineering, <wd@denx.de>
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#if defined(CONFIG_64BIT)
#define PTR_COUNT_SHIFT	3
#else
#define PTR_COUNT_SHIFT	2
#endif
/*
OUTPUT_FORMAT("elf32-tradlittlemips", "elf32-tradbigmips",
              "elf32-tradlittlemips")
OUTPUT_ARCH(mips:isa32)	      
*/
OUTPUT_FORMAT("elf32-tradlittlemips", "elf32-tradlittlemips", "elf32-tradlittlemips")	      
OUTPUT_ARCH(mips:isa32r2)
ENTRY(_start_nvt)
SECTIONS
{

	.START_UP 0x8F400000:
	{
	  CONFIG_SYS_TEXT_BASE = ABSOLUTE(.);
	  arch/mips/cpu/mips32/start.o (.text.startup*)
	}

	.CODE_INFO 0x8F4002F0:
	{
	  KEEP(board/novatek/evb660/built-in.o(.data.coreinfo*));
	}

	.BIN_INFO 0x8F400400:
	{
	  KEEP(board/novatek/evb660/built-in.o(.data.bininfo*));
	}	
	. = ALIGN(4);
	.text : {
		*(.text*)
	}

	. = ALIGN(4);
	.rodata : {
		*(SORT_BY_ALIGNMENT(SORT_BY_NAME(.rodata*)))
	}

	. = ALIGN(4);
	.data : {
		*(.data*)
	}

	. = .;
	_gp = ALIGN(16) + 0x7ff0;

	.got : {
		*(.got)
	}

	num_got_entries = SIZEOF(.got) >> PTR_COUNT_SHIFT;

	. = ALIGN(4);
	.sdata : {
		*(.sdata*)
	}

	. = ALIGN(4);
	.u_boot_list : {
		KEEP(*(SORT(.u_boot_list*)));
	}

	. = ALIGN(4);
	__image_copy_end = .;
	__init_end = .;

	.rel.dyn : {
		__rel_dyn_start = .;
		*(.rel.dyn*)
		__rel_dyn_end = .;
	}
	. = ALIGN(4);
	.BIN_END :
	{
	  KEEP(board/novatek/evb660/built-in.o(.BinEnd));
	}
	_end = .;
	uboot_bin_size = _end - CONFIG_SYS_TEXT_BASE;

	.bss __rel_dyn_start (OVERLAY) : {
		__bss_start = .;
		*(.sbss.*)
		*(.bss.*)
		*(COMMON)
		. = ALIGN(4);
		__bss_end = .;
	}

	.dynsym _end : {
		*(.dynsym)
	}

	.dynbss : {
		*(.dynbss)
	}

	.dynstr : {
		*(.dynstr)
	}

	.dynamic : {
		*(.dynamic)
	}

	.plt : {
		*(.plt)
	}

	.interp : {
		*(.interp)
	}

	.gnu : {
		*(.gnu*)
	}

	.MIPS.stubs : {
		*(.MIPS.stubs)
	}

	.hash : {
		*(.hash)
	}
	uboot_end = .;
}
