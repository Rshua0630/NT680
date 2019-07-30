/**
    Clock tick
    Configure the clock tick rate
    @file       arch/arm/mach-nvt-na51000/include/mach/timex.h
    @ingroup
    @note
    Copyright   Novatek Microelectronics Corp. 2016.  All rights reserved.

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License version 2 as
    published by the Free Software Foundation.
*/

#ifdef CONFIG_NT96680_FPGA_EMULATION_S1
#define CLOCK_TICK_RATE		(6750000)
#else
#define CLOCK_TICK_RATE		(24000000)
#endif
