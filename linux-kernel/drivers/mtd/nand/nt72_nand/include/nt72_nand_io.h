#ifndef _NT72_NAND_IO_H_
#define _NT72_NAND_IO_H_

#include "nt72_nand_config.h"

#ifdef USE_IO_REMAP
/* struct resource: for newer kernel */
#include <linux/io.h>
/* struct resource: for older kernel */
#include <linux/ioport.h>
#endif

#ifndef USE_IO_REMAP
#define nt72_nand_nfc_ioremap_all(...)
#else
int nt72_nand_nfc_ioremap_all(struct resource *resource);
#endif

u32 clkgen_read32(const u32 addr);

u32 nfc_read32(const u32 addr);
void nfc_write32(const u32 addr, u32 val);

u32 stbc_read32(const u32 addr);
void stbc_write32(const u32 addr, u32 val);

#endif /* _NT72_NAND_IO_H_ */
