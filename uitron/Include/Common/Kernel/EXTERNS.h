/*
    Extern variables.

    @file       EXTERNS.h
    @ingroup    mIKernelCore
    @note       Nothing.

    Copyright   Novatek Microelectronics Corp. 2005.  All rights reserved.
*/

#ifndef _EXTERNS_H
#define _EXTERNS_H

#include "syscall.h"
#include "QUE.h"

#ifdef __ECOS
// eCos system

//-----------------------------------------------------------------------------
// External variables
//-----------------------------------------------------------------------------
// Task
extern const int            gtcbnum;
// Semaphore
extern const int            gsemnum;
// Flag
extern const int            gflgnum;
// Mailbox
extern const int            gmbxnum;
extern const int            gbufnum;
// Fixed memory pool
extern VP                   gfremem;
extern VP                   gendmem;
extern const int            gfplnum;
extern VP                   gfremem_arbit2;
extern VP                   gendmem_arbit2;

#else
// uITRON system

//-----------------------------------------------------------------------------
// External variables
//-----------------------------------------------------------------------------
// Scheduling flags
extern volatile union sch   gschflg;

// Time
extern struct dlq           gtsvque;

// Task
extern struct dcq           grdyque;
extern struct tcb          *gruntcb;
extern struct tcb          *gtcbadr[];
extern struct tcb           gtcbarr[];
extern const int            gtcbnum;

// Semaphore
extern struct sem           gsemarr[];
extern struct sem          *gsemadr[];
extern const int            gsemnum;

// Flag
extern struct flg          *gflgadr[];
extern struct flg           gflgarr[];
extern const int            gflgnum;

// Mailbox
extern struct buf          *gbuffre;
extern struct buf           gbufarr[];
extern struct mbx           gmbxarr[];
extern struct mbx          *gmbxadr[];
extern const int            gmbxnum;
extern const int            gbufnum;

// Fixed memory pool
extern struct fpl           gfplarr[];
extern struct fpl          *gfpladr[];
extern VP                   gfremem;
extern VP                   gendmem;
extern VP                   gfremem_arbit2;
extern VP                   gendmem_arbit2;
extern const int            gfplnum;
extern const int            gfpl_arb2_start_num;
#endif

#endif
