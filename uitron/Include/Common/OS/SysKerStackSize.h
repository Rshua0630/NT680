/**
    System kernel.

    APIs for system startup and initail

    @file       SysKer.h
    @ingroup    mIOS

    Copyright   Novatek Microelectronics Corp. 2012.  All rights reserved.
*/

#ifndef _SYSKER_STACKSIZE_H_
#define _SYSKER_STACKSIZE_H_

//System reversed, do not export to doxygen
//RESV layout config
#if (_ARCH_ == _ARCH_ARM_)
//ARM
#define StackSize_SVC           0x1000      /* 4KB */
#define StackSize_ABT           0x1000      /* 4KB */
#define StackSize_UDF           0x1000      /* 4KB */
#define StackSize_FIQ           0x1000      /* 4KB */
#define StackSize_IRQ           0x1000      /* 4KB */
#define StackSize_USR           0x1000      /* 4KB */
#define HeapSize_CRuntime       0x2000      /* 8KB */

// Order
// StackSize_IRQ
// StackSize_FIQ
// StackSize_ABT
// StackSize_UDF
// StackSize_USR
// StackSize_SVC

#define StackEnd_IRQ            StackSize_IRQ
#define StackEnd_FIQ            StackSize_FIQ + StackEnd_IRQ;
#define StackEnd_ABT            StackSize_ABT + StackEnd_FIQ;
#define StackEnd_UDF            StackSize_UDF + StackEnd_ABT
#define StackEnd_USR            StackSize_USR + StackEnd_UDF
#define StackEnd_SVC            StackSize_SVC + StackEnd_USR

#define _SYSTEM_RESERVED_SIZE_  (HeapSize_CRuntime + StackSize_SVC + StackSize_ABT + StackSize_UDF + StackSize_FIQ + StackSize_IRQ + StackSize_USR)

#elif (_ARCH_ == _ARCH_MIPS_)

//MIPS
#define StackSize_KNL           0x1000      /* 4KB */
#define StackSize_INT           0x1000      /* 4KB */

#define _SYSTEM_RESERVED_SIZE_  (StackSize_KNL + StackSize_INT)

#endif


#endif //_SYSKER_H_
