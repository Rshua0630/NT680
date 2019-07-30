#ifndef CYGONCE_HAL_VAR_IO_H
#define CYGONCE_HAL_VAR_IO_H

#include <cyg/hal/dev_base.h>
#include <cyg/hal/gic.h>
#include <cyg/hal/arm_timer.h>

//#define __bitwise__ __attribute__((bitwise))
//#define __force	__attribute__((force))
#define __bitwise__
#define __force


#define __bitwise __bitwise__

#define __u32 unsigned int
#define __le32 unsigned int

#define __le32_to_cpu(x) ((__force __u32)(__le32)(x))
#define __cpu_to_le32(x) ((__force __le32)(__u32)(x)

#define le32_to_cpu __le32_to_cpu
#define cpu_to_le32 __cpu_to_le32

#define readl_relaxed(c)    ({ u32 __r = le32_to_cpu((__force __le32) \
								__raw_readl(c)); __r; })
#define writel_relaxed(v,c)	__raw_writel((__force u32) cpu_to_le32(v),c)

#define dsb(x) __asm__ __volatile__ ("mcr p15, 0, %0, c7, c10, 4" \
                        : : "r" (0) : "memory")

#define rmb()           dsb()
#define wmb()           dsb()
#define __iormb()		rmb()
#define __iowmb()		wmb()

#define readl(c)		({ u32 __v = readl_relaxed(c); __iormb(); __v; })
#define writel(v,c)		({ __iowmb(); writel_relaxed(v,c); })

#endif // CYGONCE_HAL_VAR_IO_H
