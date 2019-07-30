/**
    To setup sparse memory section size
    @file      memory.h
    @ingroup
    @note
    Copyright   Novatek Microelectronics Corp. 2017.  All rights reserved.

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License version 2 as
    published by the Free Software Foundation.
*/
#ifndef __ASM_ARCH_MEMORY_H
#define __ASM_ARCH_MEMORY_H

/*
 * Maximum section size is 8MB, maximum section is 512.
 */
#define MAX_PHYSMEM_BITS	32
#define SECTION_SIZE_BITS	23

#endif
