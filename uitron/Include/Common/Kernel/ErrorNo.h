/**
    uITRON Error Code values. The error number of the return status.

    @file       ErrorNo.h
    @ingroup    mIKernelCore
    @note       Nothing.

    Copyright   Novatek Microelectronics Corp. 2005.  All rights reserved.
*/

#ifndef _ERRORNO_H
#define _ERRORNO_H

#ifdef __ECOS
// eCos system

#include <cyg/infra/cyg_type.h>
#include <cyg/compat/uitron/uit_type.h>

#else
// uITRON system

/**
    @addtogroup mIKernelCore
*/
//@{

/**
    @name   uITRON error code value

    Definition for uITRON error code value
*/
//@{
#define E_OK                0           ///< Normal completion
#define E_SYS               (-5)        ///< System error
#define E_NOMEM             (-10)       ///< Insufficient memory
#define E_NOSPT             (-17)       ///< Feature not supported
#define E_INOSPT            (-18)       ///< Feature not supported by ITRON/FILE specification
#define E_RSFN              (-20)       ///< Reserved function code number
#define E_RSATR             (-24)       ///< Reserved attribute
#define E_PAR               (-33)       ///< Parameter error
#define E_ID                (-35)       ///< Invalid ID number
#define E_NOEXS             (-52)       ///< Object does not yet exist
#define E_OBJ               (-63)       ///< Invalid object state
#define E_MACV              (-65)       ///< Memory access disabled or memory access violation
#define E_OACV              (-66)       ///< Object access violation
#define E_CTX               (-69)       ///< Context error
#define E_QOVR              (-73)       ///< Queuing or nesting overflow
#define E_DLT               (-81)       ///< Object being waited for was deleted
#define E_TMOUT             (-85)       ///< Polling failure or timeout exceeded
#define E_RLWAI             (-86)       ///< WAIT state was forcibly released
//@}

//@}

#endif

#endif
