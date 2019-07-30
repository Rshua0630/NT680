/**
    uITRON mailbox related header file.

    @file       Mailbox.h
    @ingroup    mIKernelMail
    @note       Nothing.

    Copyright   Novatek Microelectronics Corp. 2005.  All rights reserved.
*/

#ifndef _MAILBOX_H
#define _MAILBOX_H

#ifdef __ECOS
// eCos system

#include <cyg/compat/uitron/uit_func.h>

extern UINT     kchk_msg(ID mbxid);

#else
// uITRON system

// Buffer
struct buf {
	struct buf             *b_forw;     /*  0 Forward ptr in queue             */
	VP                      b_madr;     /*  4 Message address                  */
	PRI                     b_prty;     /*  8 Message priority                 */
};

// Mailbox
struct mbx {
	struct tcb             *m_forw;     /*  0 First waiting TCB                */
	struct tcb             *m_back;     /*  4 Last waiting TCB                 */
	volatile UB             m_stat;     /*  8 Mailbox status                   */
	UB                      m_type;     /*  9 Structure type                   */
	UH                      m_rsvd;     /* 10 Reserved (for alignment)         */
	struct buf             *m_mque;     /* 12 Message queue                    */
	VP                      m_exinf;    /* 16 Extended information             */
};

/**
    @addtogroup mIKernelMail
*/
//@{

extern ER       cre_mbx(ID mbxid, T_CMBX *pk_cmbx);
extern ER       snd_msg(ID mbxid, T_MSG *p_msg);
extern ER       rcv_msg(T_MSG **ppk_msg, ID mbxid);
extern UINT     kchk_msg(ID mbxid);

//@}

#endif

#endif
