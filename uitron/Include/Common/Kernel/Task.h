/**
    uITRON task related header file.

    @file       Task.h
    @ingroup    mIKernelTask
    @note       Nothing.

    Copyright   Novatek Microelectronics Corp. 2005.  All rights reserved.
*/

#ifndef _TASK_H
#define _TASK_H

#ifdef __ECOS
// eCos system

#include <cyg/compat/uitron/uit_func.h>
extern void     kent_tsk(void);

#else
// uITRON system

struct  tcb {                           /* Numbers indiacate the starting byte */
	/* for each member.                    */
	volatile struct tcb    *p_forw;     /*  0 Forw ptr in wait queue           */
	volatile struct tcb    *p_back;     /*  4 Back ptr in wait queue           */
	volatile UB             p_stat;     /*  8 Current state                    */
	UB                      p_rsvd0;    /*  9 Reserved 0                       */
	UB                      p_rsvd1;    /* 10 Reserved 1                       */
	UB                      p_rsvd2;    /* 11 Reserved 2                       */
	ID                      p_iden;     /* 12 TCB ID (number)                  */
	VP                      p_list;     /* 16 Queue ptr process is in          */
	struct tcb             *t_forw;     /* 20 Forw ptr in time queue           */
	struct tcb             *t_back;     /* 24 Back ptr in time queue           */
	TMO                     t_tval;     /* 28 Timeout value                    */
	UB                      p_rqst;     /* 32 Request status bits              */
	volatile UB             p_psch;     /* 33 Optional parameter to scheduler  */
	UB                      p_wupc;     /* 34 Task wakeup counter              */
	UB                      p_susc;     /* 35 Task suspension counter          */
	PRI                     p_prty;     /* 36 Executing priority               */
	ER                      p_rets;     /* 40 Returned status                  */
	VP                      p_retp;     /* 44 Returned parameter               */
	VP                      p_efad;     /* 48 Event flags waited addr          */
	VP                      p_par1;     /* 52 Operation parameter 1            */
	VP                      p_par2;     /* 56 Operation parameter 2            */
	VP                      p_ussp;     /* 60 Task stack ptr                   */
	VP                      p_ient;     /* 64 Task initial entry ptr           */
	VP                      p_istk;     /* 68 Task initial stack ptr           */
	INT                     p_isiz;     /* 72 Task initial stack size          */
	VP                      p_exinf;    /* 76 Extended information             */
	FLGPTN                  p_wflg;     /* 80 Wait flags                       */
	PRI                     p_pini;     /* 84 Initial priority                 */
	PRI                     p_porg;     /* 88 Original priority                */
	PRI                     p_pnew;     /* 92 New requested priority           */
};

// Bits in p_psch field
#define P_SCH_STA   0x01                // Initial dispatch; never run after sta_tsk
#define P_SCH_EXT   0x02                // Exiting a task
#define P_SCH_INT   0x04                // Task has been preempted via interuupt

#define __FIELD_OFFSET(type, field)     (  (unsigned long)&( ((type *)0)->field )  )

/**
    @addtogroup mIKernelTask
*/
//@{

extern ER       cre_tsk(ID tskid, T_CTSK *pk_ctsk);
extern ER       sta_tsk(ID tskid, INT stacd);
extern ER       sus_tsk(ID tskid);
extern ER       rsm_tsk(ID tskid);
extern void     ext_tsk(void);
extern ER       ter_tsk(ID tskid);
extern ER       get_tid(ID *p_tskid);
extern void     kent_tsk(void);

//@}

#endif

#endif
