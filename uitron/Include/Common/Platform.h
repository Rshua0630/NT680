#ifndef _PLATFORM_H
#define _PLATFORM_H

// Toolchain definition
#if defined (__arm)
#define _ALIGNED(x) __align(x)
#define _PACKED_BEGIN __packed
#define _PACKED_END
#define _INLINE static __inline
#define _ASM_NOP __asm{nop;}
#define _SECTION(sec)
#define _CALLBACK()
#define _CALLBACK_SECTION(sec)
#define _STOPTRACE
#elif defined (__GNUC__)
#define _ALIGNED(x) __attribute__((aligned(x)))
#define _PACKED_BEGIN
#define _PACKED_END __attribute__ ((packed))
#define _INLINE static inline
#define _ASM_NOP __asm__("nop");
#define _SECTION(sec)          __attribute__ ((section (sec)))
#define _CALLBACK()            __attribute__ ((weak))
#define _CALLBACK_SECTION(sec) __attribute__ ((weak, section(sec)))
#define _STOPTRACE __attribute__((__no_instrument_function__))
#endif

// Architecture definition
// These should be defined in MakeCommon.txt
#ifndef _ARCH_ARM_
#error _ARCH_ARM_ is not defined in MakeCommon.txt
#define _ARCH_ARM_
#endif

#ifndef _ARCH_MIPS_
#error _ARCH_MIPS_ is not defined in MakeCommon.txt
#define _ARCH_MIPS_
#endif

#ifndef _ARCH_
#error _ARCH_ is not defined in MakeCommon.txt
#define _ARCH_
#endif

//OS identifier
#if defined(__ECOS)
#define _CPU1_ECOS_
#else
#define _CPU1_UITRON_
#endif

//BSP identifier
#define _BSP_NA51000_
#define _BSP_NAME_		"NA51000"

//todo identifier
#define _TODO 			0

#endif
