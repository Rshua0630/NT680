#ifndef __ASSEMBLER__
#include <cyg/infra/cyg_type.h>
#include <pkgconf/hal_arm_nvt.h>
#include <stddef.h>
#endif

#define CYGMEM_REGION_sdram         (0x00000000)
#define CYGMEM_REGION_sdram_SIZE    (CYGNUM_HAL_DRAM_SIZE*1024*1024)

#ifndef __ASSEMBLER__
extern char CYG_LABEL_NAME(__rom_vectors_vma) [];
extern char CYG_LABEL_NAME(_etext) [];
extern char CYG_LABEL_NAME(__ram_data_start) [];
extern char CYG_LABEL_NAME(__ram_data_end) [];
extern char CYG_LABEL_NAME(__mmu_tables_start) [];
extern char CYG_LABEL_NAME(__mmu_tables_end) [];
extern char CYG_LABEL_NAME(__bss_start) [];
extern char CYG_LABEL_NAME(__bss_end) [];
extern char CYG_LABEL_NAME(__heap1) [];
#endif
#define CYGMEM_SECTION_heap1 (CYG_LABEL_NAME (__heap1))
#define CYGMEM_SECTION_heap1_SIZE (CYGMEM_REGION_sdram + CYGMEM_REGION_sdram_SIZE - (size_t) CYG_LABEL_NAME (__heap1))

