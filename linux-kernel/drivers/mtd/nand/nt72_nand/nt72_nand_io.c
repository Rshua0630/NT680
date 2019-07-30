#include "include/nt72_nand_io.h"
#include "include/nt72_nand_regs.h"
#include "include/nt72_nand_utils.h"

#ifdef USE_IO_REMAP
static void __iomem *nfc_base;
static void __iomem *stbc_base;
static void __iomem *clkgen_base;
#endif

#ifdef USE_IO_REMAP
int nt72_nand_nfc_ioremap_all(struct resource *resource)
{
	int result = 0;

	nfc_base = ioremap_nocache(resource->start, resource_size(resource));
	stbc_base = ioremap_nocache(REG_STBC_BASE, 0x1000);
	clkgen_base = ioremap_nocache(REG_CLK_GEN_BASE, 0x1000);

	if (!nfc_base) {
		nfc_err("ioremap NFC failed\n");
		result = -1;
	}
	if (!stbc_base) {
		nfc_err("ioremap STBC failed\n");
		result = -1;
	}
	if (!clkgen_base) {
		nfc_err("ioremap CLKGEN failed\n");
		result = -1;
	}

	return result;
}
#endif

inline u32 clkgen_read32(const u32 addr)
{
	u32 val;

#ifdef USE_IO_REMAP
	val = ioread32(clkgen_base + ((addr) - (REG_CLK_GEN_BASE)));
#else
	/* val = (u32)(*(volatile unsigned long *)addr); */
#endif

	nt72_nand_bus_sniffer_add("CLKR", addr, val);

	return val;
}

inline u32 nfc_read32(const u32 addr)
{
	u32 val;

#ifdef USE_IO_REMAP
	val = ioread32(nfc_base + ((addr) - (REG_NFC_BASE)));
#else
	/* val = (u32)(*(volatile unsigned long *)addr); */
#endif

	nt72_nand_bus_sniffer_add("NFCR", addr, val);

	return val;
}

inline void nfc_write32(const u32 addr, u32 val)
{
#ifdef USE_IO_REMAP
	iowrite32(val, nfc_base + ((addr) - (REG_NFC_BASE)));
#else
	/* *(volatile unsigned long *)addr = val; */
#endif

	nt72_nand_bus_sniffer_add("NFCW", addr, val);
}

inline u32 stbc_read32(const u32 addr)
{
	u32 val;

#ifdef USE_IO_REMAP
	val = ioread32(stbc_base + ((addr) - (REG_STBC_BASE)));
#else
	/* val = (u32)(*(volatile unsigned long *)addr); */
#endif

	nt72_nand_bus_sniffer_add("STBCR", addr, val);

	return val;
}

inline void stbc_write32(const u32 addr, u32 val)
{
#ifdef USE_IO_REMAP
	iowrite32(val, stbc_base + ((addr) - (REG_STBC_BASE)));
#else
	/* *(volatile unsigned long *)addr = val; */
#endif

	nt72_nand_bus_sniffer_add("STBCW", addr, val);
}
