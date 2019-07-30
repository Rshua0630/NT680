#ifndef __ARM_CHACHE_H__
#define __ARM_CHACHE_H__

#define LEVEL_1 (1 - 1)
#define LEVEL_2 (2 - 1)

#define CLIDR() \
	({ \
	 unsigned long val; \
	 __asm__ __volatile__( \
		 "mrc p15, 1, %0, c0, c0, 1\n\t" \
		 : "=r" (val)); \
	 val; \
	 })

#define CCSIDR() \
	({ \
	 unsigned long val; \
	 __asm__ __volatile__( \
		 "mrc p15, 1, %0, c0, c0, 0\n\t" \
		 : "=r" (val)); \
	 val; \
	 })

#define sel_CSSELR(InD) \
	__asm__ __volatile__(	\
			"mcr p15, 2, %0, c0, c0, 0\n\t" \
			: \
			: "r"(InD));

#define _ICACHE_INV_ALL() _ICIALLU()
#define _ICIALLU() \
	__asm__ __volatile__("mcr p15, 0, r0, c7, c5, 0\n\t");

#define _ICACHE_INV_MVAU(addr) _ICIMVAU(addr)
#define _ICIMVAU(addr) \
	__asm__ __volatile__(	\
			"mcr p15, 0, %0, c7, c5, 1\n\t" \
			: \
			: "r"(addr));

#define _DCACHE_INV_MVAC(addr) _DCIMVAC(addr)
#define _DCIMVAC(addr) \
	__asm__ __volatile__(	\
			"mcr p15, 0, %0, c7, c6, 1\n\t" \
			: \
			: "r"(addr));

#define _DCACHE_WBACK_MVAC(addr) _DCCMVAC(addr)
#define _DCCMVAC(addr) \
	__asm__ __volatile__(	\
			"mcr p15, 0, %0, c7, c10, 1\n\t" \
			: \
			: "r"(addr));

#define _DCACHE_WBACK_INV_MVAC(addr) _DCCIMVAC(addr)
#define _DCCIMVAC(addr) \
	__asm__ __volatile__(	\
			"mcr p15, 0, %0, c7, c14, 1\n\t" \
			: \
			: "r"(addr));

#define _DCCISW(way_set) \
	__asm__ __volatile__(	\
			"mcr p15, 0, %0, c7 ,c14, 2\n\t"  \
			:	\
			: "r"(way_set));

#define _DCISW(way_set) \
	__asm__ __volatile__(	\
			"mcr p15, 0, %0, c7 ,c6, 2\n\t"  \
			:	\
			: "r"(way_set));


#endif /* __ARM_CHACHE_H__ */

