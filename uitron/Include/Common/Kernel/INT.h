/**
    uITRON interrupt related header file.

    @file       INT.h
    @ingroup    mIKernelIntr
    @note       Nothing.

    Copyright   Novatek Microelectronics Corp. 2005.  All rights reserved.
*/


#ifndef _INT_H
#define _INT_H

#ifdef __ECOS
// eCos system

#include <cyg/compat/uitron/uit_func.h>

#else
// uITRON system

union   sch {
	struct {
		unsigned char   psf;
		unsigned char   isf;
		unsigned char   tmf;
		unsigned char   acf;
	} byt;
	struct {
		unsigned short  pif;
		unsigned short  taf;
	} srt;
	unsigned long       wrd;
};

#define gpsflag     gschflg.byt.psf     // Scheduler is processing
#define gtmflag     gschflg.byt.tmf     // Timer is being requested
//#if (_ARCH_ == _ARCH_ARM_)
#define girqflag    gschflg.byt.acf     // IRQ int
//#elif (_ARCH_ == _ARCH_MIPS_)
//#define gexlflag    gschflg.byt.acf     // exception level flag 0:in ISR 1: otherwise
//#endif
#define gfiqflag    gschflg.byt.isf     // INT

/**
    @addtogroup mIKernelIntr
*/
//@{

extern ER       def_int(UINT dintno, FP hdl);
extern ER       dis_int(UINT intno);
extern ER       ena_int(UINT intno);

//@}

#endif

#endif
