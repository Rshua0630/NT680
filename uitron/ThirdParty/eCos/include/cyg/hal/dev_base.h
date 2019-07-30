#ifndef CYGONCE_DEV_BASE_H
#define CYGONCE_DEV_BASE_H

#include <pkgconf/hal_arm_nvt.h>

#define PERIPH_BASE                0xF1500000

#define ARM_TIMER_BASEADDR         (PERIPH_BASE + 0x0600)
#define GIC_CPU_BASEADDR           (PERIPH_BASE + 0x2000)
#define GIC_DIST_BASEADDR          (PERIPH_BASE + 0x1000)

#endif
