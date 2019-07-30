/*
    User include file for kernel assembly.

    @file       regdef.h
    @ingroup    mIKernelCore
    @note       Nothing.

    Copyright   Novatek Microelectronics Corp. 2017.  All rights reserved.
*/

#ifndef ALIGN
#define ALIGN .align        4
#endif

#ifndef ENTRY
#define ENTRY(name) \
	.globl name; \
	ALIGN; \
	name:
#endif

#ifndef WEAK
#define WEAK(name) \
	.weak name; \
	name:
#endif

#ifndef END
#define END(name) \
	.size name, .-name
#endif

#define ENDPROC(name) \
	.type name, %function; \
	END(name)

