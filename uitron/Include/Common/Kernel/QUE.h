/**
    uITRON queue related header file.

    @file       QUE.h
    @ingroup    mIKernelCore
    @note       Nothing.

    Copyright   Novatek Microelectronics Corp. 2005.  All rights reserved.
*/

#ifndef _QUE_H
#define _QUE_H

#ifdef __ECOS
// eCos system

#else
// uITRON system

/**
    @addtogroup mIKernelCore
*/
//@{

/**
    Double Linked Queue
*/
struct  dlq {
	void                   *q_forw;     ///< First TCB in queue
	void                   *q_back;     ///< Last TCB in queue
};

/**
    Double Linked Queue With Concurrent Access
*/
struct  dcq {
	void *volatile          q_forw;     ///< First item in queue
	void *volatile          q_back;     ///< Last item in queue
	volatile unsigned char  q_stat;     ///< Queue status
	unsigned char           q_type;     ///< Queue type
	unsigned short          q_rsvd;     ///< Reserved (for alignment)
};


/**
    Double Linked Queue With Concurrent Access, Param and Lock
*/
struct  que {
	void                   *q_forw;     ///< First item in queue
	void                   *q_back;     ///< Last item in queue
	volatile unsigned char  q_stat;     ///< Queue status
	unsigned char           q_type;     ///< Queue type
	unsigned short          q_rsvd;     ///< Reserved (for alignment)
	void                   *q_parm;     ///< Queue parameter
	void                   *q_ownr;     ///< Queue owner
};

//@}

#endif

#endif
