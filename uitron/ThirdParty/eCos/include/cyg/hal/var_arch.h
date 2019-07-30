#ifndef CYGONCE_HAL_VAR_ARCH_H
#define CYGONCE_HAL_VAR_ARCH_H
#include <pkgconf/hal.h>
#include <cyg/hal/hal_io.h>

//#define CYGARC_UNCACHED_ADDR_MASK   (0x40000000)
#ifndef CYGARC_CACHED_ADDRESS
#define CYGARC_CACHED_ADDRESS(x)    (((((CYG_ADDRESS)(x))>=0x60000000UL)?((CYG_ADDRESS)(x)-0x60000000UL):(CYG_ADDRESS)(x))
#endif
#ifndef CYGARC_UNCACHED_ADDRESS
#define CYGARC_UNCACHED_ADDRESS(x)      ((((CYG_ADDRESS)(x))<0x60000000UL)?((CYG_ADDRESS)(x)+0x60000000UL):(CYG_ADDRESS)(x))
#endif

#ifndef CYGARC_PHYSICAL_ADDRESS
#define CYGARC_PHYSICAL_ADDRESS(x)      ((((CYG_ADDRESS)(x))>=0x60000000UL)?((CYG_ADDRESS)(x)-0x60000000UL):(CYG_ADDRESS)(x))
#endif

#ifndef HAL_MEMORY_BARRIER
#define HAL_MEMORY_BARRIER()                                            \
CYG_MACRO_START                                                         \
    asm volatile (                                                      \
        "dsb" /* drain the write buffer */  \
        );                                                              \
CYG_MACRO_END
#endif


//--------------------------------------------------------------------------
// Idle thread code.
// This macro is called in the idle thread loop, and gives the HAL the
// chance to insert code. Typical idle thread behaviour might be to halt the
// processor. These implementations halt the system core clock.

#ifndef HAL_IDLE_THREAD_ACTION

#endif

#endif // CYGONCE_HAL_VAR_ARCH_H
