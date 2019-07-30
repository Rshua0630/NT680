/**
    uITRON 2.0 standard data type.

    @file       mIType.h
    @ingroup    mIKernelCore
    @note       This include file only contain uITRON related data type definition.

    Copyright   Novatek Microelectronics Corp. 2005.  All rights reserved.
*/

#ifndef _MITYPE_H
#define _MITYPE_H

// Check if the ARM CC is latest version
#if defined(__arm)
#if (__ARMCC_VERSION != 120848)
#error ARM CC version is not 1.2 build 848
#endif
#endif

// Check if the GNUC is latest version
#if defined(__GNUC__)
#define GCC_VERSION (__GNUC__ * 10000 + __GNUC_MINOR__ * 100 + __GNUC_PATCHLEVEL__)
#if (GCC_VERSION != 40901)
#if(GCC_VERSION != 40900)
//#error GNUC version is not 4.9.1 or 4.9.0
#endif
#endif
#endif

/**
    @addtogroup mIKernelCore
*/
//@{

// eCos & uITRON common part
#define FLGPTN_TYPE_32      0
#define FLGPTN_TYPE_64      1
#define FLGPTN_TYPE         FLGPTN_TYPE_32

#define ARBIT1_MEM          0x00    ///< Memory pool from arbit 1
#define ARBIT2_MEM          0x01    ///< Memory pool fron arbit 2

#if (FLGPTN_TYPE == FLGPTN_TYPE_32)
// 32 bits flag patterns
typedef unsigned int        FLGPTN;                     ///< Flag patterns
typedef unsigned int        *PFLGPTN;                   ///< Flag patterns (Pointer)
#define FLGPTN_BIT(n)       ((FLGPTN)(1 << (n)))        ///< Bit of flag pattern
#define FLGPTN_BIT_ALL      ((FLGPTN)0xFFFFFFFF)
#elif (FLGPTN_TYPE == FLGPTN_TYPE_64)
// 64 bits flag patterns
typedef unsigned long long  FLGPTN;                     ///< Flag patterns
typedef unsigned long long  *PFLGPTN;                   ///< Flag patterns (Pointer)
#define FLGPTN_BIT(n)       ((FLGPTN)(1ull << (n)))     ///< Bit of flag pattern
#define FLGPTN_BIT_ALL      ((FLGPTN)0xFFFFFFFFFFFFFFFFull)
#else
#error Incorrect flag pattern type
#endif

// NULL
#ifndef NULL
#define NULL        ((void*)0)
#endif

#ifdef __ECOS
// eCos system

#include <cyg/infra/cyg_type.h>
#include <cyg/compat/uitron/uit_type.h>

#else
// uITRON system

/**
    @name Standard data type for uITRON 3.0.
*/
//@{
typedef signed char         B;              ///< Signed 8-bit integer
typedef signed short        H;              ///< Signed 16-bit integer
typedef signed long         W;              ///< Signed 32-bit integer
typedef unsigned char       UB;             ///< Unsigned 8-bit integer
typedef unsigned short      UH;             ///< Unsigned 16-bit integer
typedef unsigned long       UW;             ///< Unsigned 32-bit integer
typedef long                VW;             ///< Variable data type (32 bits)
typedef short               VH;             ///< Variable data type (16 bits)
typedef char                VB;             ///< Variable data type (8 bits)


typedef void                *VP;            ///< Pointer to unspecified data type
typedef void (*FP)(void);                   ///< Program start address

typedef W                   BOOL;           ///< Boolean
typedef signed int          INT;            ///< Signed integer (32 bits)
typedef unsigned int        UINT;           ///< Unsigned integer (32 bits)

typedef W                   ID;             ///< Object ID number
typedef W                   ER;             ///< Error code
typedef INT                 PRI;            ///< Task priority
typedef UINT                ATR;            ///< Object attribute
typedef W                   TMO;            ///< Time-out
//@}

// For BOOL type
enum {
	FALSE   = 0,
	TRUE    = 1,
};

/**
    Information to create task

    Information to create task.
*/
typedef struct t_ctsk {
	VP      exinf;          ///< Extended information (Not referenced)
	ATR     tskatr;         ///< Task attributes (Not referenced)
	FP      task;           ///< Task start address
	PRI     itskpri;        ///< Initial task priority
	INT     stksz;          ///< Task stack size
	// Implementation-dependent information -begin
	VP      istack;         ///< Pointer to task stack
	// Implementation-dependent information -end
} T_CTSK;

/**
    Information to create semaphore

    Information to create semaphore.
*/
typedef struct t_csem {
	VP      exinf;          ///< Extended information (Not referenced)
	ATR     sematr;         ///< Semaphore attribute (Not referenced)
	INT     isemcnt;        ///< Semaphore counter, but only support up to 255 now
	INT     maxsem;         ///< Semaphore Max counter value, but only support up to 255 now
} T_CSEM;

/**
    Information to create flag

    Information to create flag.
*/
typedef struct t_cflg {
	VP      exinf;          ///< Extended information (Not referenced)
	ATR     flgatr;         ///< Flag attribute (Not referenced)
	FLGPTN  iflgptn;        ///< Initial flag pattern
} T_CFLG;

/**
    Information to create fixed memory pool

    Information to create fixed memory pool.
*/
typedef struct t_cmpf {
	VP      exinf;          ///< Extended information (Not referenced)
	ATR     mpfatr;         ///< Fixed memory pool attribute (Not referenced)
	INT     mpfcnt;         ///< Fixed memory pool block count
	INT     blfsz;          ///< Fixed memory pool block size
} T_CMPF;

/**
    Information to create mail box

    Information to create mail box.
*/
typedef struct t_cmbx {
	VP      exinf;          ///< Extended information (Not referenced)
	ATR     mbxatr;         ///< mail box attribute (Not referenced)
} T_CMBX;

/**
    Message type

    Message type.
*/
typedef UB         T_MSG;

#endif

// eCos & uITRON common part
/**
    Kernel reserved type for message entry

    Kernel reserved type for message entry. Must be the first element
    of user defined message structure.
*/
typedef struct t_msg_krsvd {
	PRI     msgpri; ///< Message priority. The smaller the value, the higher the priority.
} T_MSG_KRSVD;

//@}

#endif
