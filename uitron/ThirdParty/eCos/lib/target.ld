STARTUP(vectors.o)
ENTRY(reset_vector)
INPUT(extras.o)
GROUP( libtarget.a libgcc.a libsupc++.a )
MEMORY
{
    sram0 : ORIGIN = 0xFC130000, LENGTH = 64K
    sdram : ORIGIN = 0x00000000, LENGTH = 124 * 1024 * 1024
}
SECTIONS
{
    .debug_aranges 0 : { *(.debug_aranges) } .debug_pubnames 0 : { *(.debug_pubnames) } .debug_info 0 : { *(.debug_info) } .debug_abbrev 0 : { *(.debug_abbrev) } .debug_line 0 : { *(.debug_line) } .debug_frame 0 : { *(.debug_frame) } .debug_str 0 : { *(.debug_str) } .debug_loc 0 : { *(.debug_loc) } .debug_macinfo 0 : { *(.debug_macinfo) } .note.arm.ident 0 : { KEEP (*(.note.arm.ident)) } /DISCARD/ 0 : { *(.fini_array*) }
    .rom_vectors 0x00008000 : { __rom_vectors_vma = ABSOLUTE(.); . = .; KEEP (*(.vectors)) } > sdram __rom_vectors_lma = LOADADDR(.rom_vectors);
    .rel.text : { *(.rel.text) *(.rel.text.*) *(.rel.gnu.linkonce.t*) } > sdram .rela.text : { *(.rela.text) *(.rela.text.*) *(.rela.gnu.linkonce.t*) } > sdram .rel.data : { *(.rel.data) *(.rel.data.*) *(.rel.gnu.linkonce.d*) } > sdram .rela.data : { *(.rela.data) *(.rela.data.*) *(.rela.gnu.linkonce.d*) } > sdram .rel.rodata : { *(.rel.rodata) *(.rel.rodata.*) *(.rel.gnu.linkonce.r*) } > sdram .rela.rodata : { *(.rela.rodata) *(.rela.rodata.*) *(.rela.gnu.linkonce.r*) } > sdram .rel.got : { *(.rel.got) } > sdram .rela.got : { *(.rela.got) } > sdram .rel.ctors : { *(.rel.ctors) } > sdram .rela.ctors : { *(.rela.ctors) } > sdram .rel.dtors : { *(.rel.dtors) } > sdram .rela.dtors : { *(.rela.dtors) } > sdram .rel.init : { *(.rel.init) } > sdram .rela.init : { *(.rela.init) } > sdram .rel.fini : { *(.rel.fini) } > sdram .rela.fini : { *(.rela.fini) } > sdram .rel.bss : { *(.rel.bss) } > sdram .rela.bss : { *(.rela.bss) } > sdram .rel.plt : { *(.rel.plt) } > sdram .rela.plt : { *(.rela.plt) } > sdram .rel.dyn : { *(.rel.dyn) } > sdram
    .ARM.extab ALIGN (0x4) : { PROVIDE (__stext = ABSOLUTE(.)); _stext = ABSOLUTE(.) ; . = .; } > sdram /DISCARD/ 0 : { *(.ARM.extab* .gnu.linkonce.armextab.*) } . = ALIGN(8); . = ALIGN(8); __exidx_start = ABSOLUTE(.); .ARM.exidx ALIGN(8) : AT ((LOADADDR (.ARM.extab) + SIZEOF (.ARM.extab) + (8) - 1) & ~ ((8) - 1)) { . = .; } > sdram __exidx_end = ABSOLUTE(.); /DISCARD/ 0 : { *(.ARM.exidx* .gnu.linkonce.armexidx.*) } .text ALIGN(8) : AT ((LOADADDR (.ARM.exidx) + SIZEOF (.ARM.exidx) + (8) - 1) & ~ ((8) - 1)) { *(.text*) *(.gnu.warning) *(.gnu.linkonce.t.*) *(.init) *(.glue_7) *(.glue_7t) __CTOR_LIST__ = ABSOLUTE (.); KEEP (*(SORT (.ctors*))) __CTOR_END__ = ABSOLUTE (.); __DTOR_LIST__ = ABSOLUTE (.); KEEP (*(SORT (.dtors*))) __DTOR_END__ = ABSOLUTE (.); } > sdram _etext = .; PROVIDE (__etext = .);
    .fini ALIGN (0x4) : { . = .; *(.fini) } > sdram
    .rodata ALIGN (0x4) : { . = .; *(.rodata*) *(.gnu.linkonce.r.*) } > sdram
    .rodata1 ALIGN (0x4) : { . = .; *(.rodata1) } > sdram
    .got ALIGN (0x4) : { . = .; *(.got.plt) *(.got) _GOT1_START_ = ABSOLUTE (.); *(.got1) _GOT1_END_ = ABSOLUTE (.); _GOT2_START_ = ABSOLUTE (.); *(.got2) _GOT2_END_ = ABSOLUTE (.); } > sdram
    .fixup ALIGN (0x4) : { . = .; *(.fixup) } > sdram
    .gcc_except_table ALIGN (0x4) : { . = .; } > sdram /DISCARD/ 0 : { *(.gcc_except_table*) }
    .data ALIGN (0x4) : AT ((LOADADDR (.gcc_except_table) + SIZEOF (.gcc_except_table) + (4) - 1) & ~ ((4) - 1)) { __ram_data_start = ABSOLUTE (.); *(.data*) *(.data1) *(.gnu.linkonce.d.*) . = ALIGN (4); KEEP(*( SORT (.ecos.table.*))) ; . = ALIGN (4); __init_array_start__ = ABSOLUTE (.); KEEP (*(SORT (.init_array.*))) KEEP (*(SORT (.init_array))) __init_array_end__ = ABSOLUTE (.); *(.dynamic) *(.sdata*) *(.gnu.linkonce.s.*) . = ALIGN (4); *(.2ram.*) } > sdram __rom_data_start = LOADADDR (.data); __ram_data_end = .; PROVIDE (__ram_data_end = .); _edata = .; PROVIDE (edata = .); PROVIDE (__rom_data_end = LOADADDR (.data) + SIZEOF(.data));
    .mmu_tables ALIGN (0x4000) (NOLOAD): { __mmu_tables_start = ABSOLUTE(.); . = .; *(.mmu_tables); __mmu_tables_end = ABSOLUTE(.); } > sdram
    .bss ALIGN (0x4) : { __bss_start = ABSOLUTE (.); *(.scommon) *(.dynsbss) *(.sbss*) *(.gnu.linkonce.sb.*) *(.dynbss) *(.bss*) *(.gnu.linkonce.b.*) *(COMMON) __bss_end = ABSOLUTE (.); } > sdram
    __heap1 = ALIGN (0x8);
    . = ALIGN(4); _end = .; PROVIDE (end = .);
}
