/**
    CPU module driver.

    This file is the driver of CPU module.

    @file       CPU.h
    @ingroup    mIDrvSys_Core
    @note       Nothing.

    Copyright   Novatek Microelectronics Corp. 2011.  All rights reserved.
*/

#ifndef _CPU_H
#define _CPU_H

#include "Type.h"

/**
     cpu information

     cpu core information during exception
     @note for cpu_expCause()
*/
typedef enum {
	CPU_EXP_CAUSE = 0x0,                        ///< Exception cause
	CPU_EXP_PC,                                 ///< Exception program counter
	CPU_SRSCTL,                                 ///< Shadow register number
	CPU_ISR_FLAG,                               ///< Program is in xxx_isr(module hook on driver ISR)
	CPU_EXP_INFO_NUM,
	ENUM_DUMMY4WORD(CPU_EXP_INFO)
} CPU_EXP_INFO;

#define cpu_setTimerCount(m)    cpu_setTimerCountReg(m)

//typedef void (*CPU_EXPCallbackFunc)(UINT32 *);
typedef void (*CPU_CBEXP)(UINT32 *, UINT32 *);

//extern ER     cpu_regExpCallbackFunc(CPU_CBEXP cpuExpCB);
extern ER   kdef_ecb(CPU_CBEXP cpuExpCB);
extern ER   cpu_begin_neon(void);
extern ER   cpu_end_neon(void);

#endif
