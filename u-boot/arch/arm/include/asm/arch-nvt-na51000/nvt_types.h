/**
    NVT common header file
    Define nvt types header
    @file       nvt_types.h
    @ingroup
    @note
    Copyright   Novatek Microelectronics Corp. 2016.  All rights reserved.

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License version 2 as
    published by the Free Software Foundation.
*/

#ifndef __NVT_TYPES_H__
#define __NVT_TYPES_H__

#define ALIGN_FLOOR(value, base)  ((value) & ~((base)-1))
#define ALIGN_CEIL(value, base)   ALIGN_FLOOR((value) + ((base)-1), base)
#define ALIGN_CEIL_4(a)           ALIGN_CEIL(a, 4)
#endif
