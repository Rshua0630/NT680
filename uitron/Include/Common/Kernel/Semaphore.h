/**
    uITRON semaphore related header file.

    @file       Semaphore.h
    @ingroup    mIKernelSema
    @note       Nothing.

    Copyright   Novatek Microelectronics Corp. 2005.  All rights reserved.
*/

#ifndef _SEM_H
#define _SEM_H

#ifdef __ECOS
// eCos system

#include <cyg/compat/uitron/uit_func.h>

#else
// uITRON system

struct  sem {
	struct tcb             *s_forw;     /*  0 Ptr to first waiting TCB         */
	struct tcb             *s_back;     /*  4 Ptr to last waiting TCB          */
	volatile UB             s_stat;     /* 12 Semaphore status                 */
	UB                      s_type;     /* 13 Structure type                   */
	UB                      s_cntr;     /* 14 Semaphore counter                */
	UB                      s_cmax;     /* 15 Semaphore max value              */
	struct tcb             *s_owner;    /* 16 Semaphore's owner                */
};

/**
    @addtogroup mIKernelSema
*/
//@{

extern ER       cre_sem(ID semid, T_CSEM *pk_csem);
extern ER       sig_sem(ID semid);
extern ER       wai_sem(ID semid);

//@}

#endif

#endif
