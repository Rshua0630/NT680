#ifndef _CACHE_H_
#define _CACHE_H_

#include <asm/addrspace.h>

#define cache_op(op,addr)									\
    __asm__ __volatile__(									\
		"   .set    push                                \n" \
		"   .set    noreorder                           \n" \
		"   .set    mips3\n\t                           \n" \
		"   cache   %0, %1                              \n" \
		"   .set    pop                                 \n" \
		:                                                   \
		: "i" (op), "R" (*(unsigned char *)(addr)))

#define __sync()								\
	__asm__ __volatile__(						\
		".set   push\n\t"						\
		".set   noreorder\n\t"					\
		".set   mips2\n\t"						\
		"sync\n\t"								\
		".set   pop"							\
		: /* no output */						\
		: /* no input */						\
		: "memory")

#define __fast_iob()							\
	__asm__ __volatile__(						\
		".set   push\n\t"						\
		".set   noreorder\n\t"					\
		"lw $0,%0\n\t"							\
		"nop\n\t"								\
		".set   pop"							\
		: /* no output */						\
		: "m" (*(int *)CKSEG1)					\
		: "memory")

#define fast_iob()								\
	do {										\
		__sync();								\
		__fast_iob();							\
	} while (0)

#define iob()         fast_iob()
#define wbflush_ahb() fast_iob()

#define cache32_unroll32(base,op)							\
    __asm__ __volatile__(									\
		"   .set push                                   \n" \
		"   .set noreorder                              \n" \
		"   .set mips3                                  \n" \
		"   cache %1, 0x000(%0); cache %1, 0x020(%0)    \n" \
		"   cache %1, 0x040(%0); cache %1, 0x060(%0)    \n" \
		"   cache %1, 0x080(%0); cache %1, 0x0a0(%0)    \n" \
		"   cache %1, 0x0c0(%0); cache %1, 0x0e0(%0)    \n" \
		"   cache %1, 0x100(%0); cache %1, 0x120(%0)    \n" \
		"   cache %1, 0x140(%0); cache %1, 0x160(%0)    \n" \
		"   cache %1, 0x180(%0); cache %1, 0x1a0(%0)    \n" \
		"   cache %1, 0x1c0(%0); cache %1, 0x1e0(%0)    \n" \
		"   cache %1, 0x200(%0); cache %1, 0x220(%0)    \n" \
		"   cache %1, 0x240(%0); cache %1, 0x260(%0)    \n" \
		"   cache %1, 0x280(%0); cache %1, 0x2a0(%0)    \n" \
		"   cache %1, 0x2c0(%0); cache %1, 0x2e0(%0)    \n" \
		"   cache %1, 0x300(%0); cache %1, 0x320(%0)    \n" \
		"   cache %1, 0x340(%0); cache %1, 0x360(%0)    \n" \
		"   cache %1, 0x380(%0); cache %1, 0x3a0(%0)    \n" \
		"   cache %1, 0x3c0(%0); cache %1, 0x3e0(%0)    \n" \
		"   .set pop                                    \n" \
		:                                                   \
		: "r" (base),                                       \
		  "i" (op));

#define icache_line_size    32
#define dcache_line_size    32

#define flush_cache(start_addr, size) wb_inv_dcache_range(start_addr, size)
/**
 * @brief Invalidate All Entries of I$
 *
 * Invalidate all entries of instruciton cache by
 * CAHCE operation ICIndexInv (I$ Index Invalidate)
 *
 * @param
 *
 * @return
 */
void inv_icache_all(void);
void inv_icache_range(void);

/**
 * @brief Invalidate and Writeback All Entries of D$
 *
 * Invalidate and writeback all entries of data cache by
 * CAHCE operation DCIndexWBInv (D$ Index Writeback Invalidate)
 *
 * @param
 *
 * @return
 */
void wb_inv_dcache_all(void);
void wb_inv_dcache_range(unsigned long, unsigned long);
void inv_dcache_range(unsigned long, unsigned long);
void wb_dcache_range(unsigned long, unsigned long);

/**
 * @brief Invalidate and Writeback All Entries of S$
 *
 * Invalidate and writeback all entries of secondary cache by
 * CAHCE operation SCIndexWBInv (S$ Index Writeback Invalidate)
 *
 * @param
 *
 * @return 0 if secondary cache exist or else -1.
 */
long wb_inv_scache_all(void);

/**
 * @brief Hit Writeback Invalidate Specified Entries of D$
 *
 * Hit writeback and invalidate specified entries of data cache by
 * CAHCE operation DCHitWBInv (D$ Hit Writeback Invalidate)
 *
 * @param addr Base address
 * @param len	 Length
 *
 * @return
 */
void wb_inv_scache_range(unsigned long, unsigned long);
void inv_scache_range(unsigned long, unsigned long);
void wb_scache_range(unsigned long, unsigned long);

#endif /* _CACHE_H_ */
