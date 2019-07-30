#ifndef __ARM_REG_H__
#define __ARM_REG_H__

/* Cache dirty register () */
#define read_CDSR() \
	({ \
	 unsigned long val; \
	 __asm__ __volatile__(\
		 "mrc p15, 0, %0, c7, c10, 6\n\t" \
		 : "=r"(val) \
		 );\
	 val;\
	 })
#define change_property() \
	({ \
	 __asm__ __volatile__( \
		 ".arch_extension sec\n\t" \
		 "smc #123\n\t" \
		 : \
		 : \
		 ); \
	 })
#define read_DBGDTRTX() \
	({ \
	 unsigned int val;	\
	  __asm__ ("mrc p14, 0, %0, c0, c5, 0\n\t" \
		  : "=r" (val) \
		  ); \
	  val;\
	})
#define write_DBGDTRTX(val) \
	({ \
	 __asm__ ("mcr p14, 0, %0, c0, c5, 0\n\t" \
		 : \
		 : "r" (val) \
		 ); \
	})
#define read_DBGDSCR_EXT() \
	({ \
	 unsigned int val; \
	  __asm__ ("mrc p14, 0, %0, c0, c2, 2\n\t" \
		  : "=r" (val) \
		  ); \
	  val;\
	})
#define read_PMCR()	\
	({ \
	 unsigned int val; \
	  __asm__ ("mrc p15, 0, %0, c9, c12, 0\n\t" \
		  : "=r" (val) \
		  ); \
	  val;\
	})
#define write_PMCR(val) \
	({ \
	 __asm__ ("mcr p15, 0, %0, c9, c12, 0\n\t" \
		 : \
		 : "r" (val) \
		 ); \
	})
#define read_PMCNTENSET()	\
	({ \
	 unsigned int val; \
	  __asm__ ("mrc p15, 0, %0, c9, c12, 1\n\t" \
		  : "=r" (val) \
		  ); \
	  val;\
	})
#define write_PMCNTENSET(val) \
	({ \
	 __asm__ ("mcr p15, 0, %0, c9, c12, 1\n\t" \
		 : \
		 : "r" (val) \
		 ); \
	})
#define read_PMCCNTR()	\
	({ \
	 unsigned int val; \
	  __asm__ ("mrc p15, 0, %0, c9, c13, 0\n\t" \
		  : "=r" (val) \
		  ); \
	  val;\
	})
#define write_PMCCNTR(val) \
	({ \
	 __asm__ ("mcr p15, 0, %0, c9, c13, 0\n\t" \
		 : \
		 : "r" (val) \
		 ); \
	})
#define read_PMINTENSET()	\
	({ \
	 unsigned int val; \
	  __asm__ ("mrc p15, 0, %0, c9, c14, 1\n\t" \
		  : "=r" (val) \
		  ); \
	  val;\
	})
#define write_PMINTENSET(val) \
	({ \
	 __asm__ ("mcr p15, 0, %0, c9, c14, 1\n\t" \
		 : \
		 : "r" (val) \
		 ); \
	})
#define read_PMOVSR()	\
	({ \
	 unsigned int val; \
	  __asm__ ("mrc p15, 0, %0, c9, c12, 3\n\t" \
		  : "=r" (val) \
		  ); \
	  val;\
	})
#define write_PMOVSR(val) \
	({ \
	 __asm__ ("mcr p15, 0, %0, c9, c12, 3\n\t" \
		 : \
		 : "r" (val) \
		 ); \
	})
#endif /* __ARM_REG_H__ */

