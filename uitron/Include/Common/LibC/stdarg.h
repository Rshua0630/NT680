#ifndef _NVT_LIBC_STDARG_H
#define _NVT_LIBC_STDARG_H

typedef char *va_list;

#if defined(__arm)

/*
 * Storage alignment properties
 */
#define  _AUPBND                (sizeof (int) - 1)
#define  _ADNBND                (sizeof (int) - 1)

/*
 * Variable argument list macro definitions
 */
#define _bnd(X, bnd)            (((sizeof (X)) + (bnd)) & (~(bnd)))
#define va_arg(ap, T)           (*(T *)(((ap) += (_bnd (T, _AUPBND))) - (_bnd (T,_ADNBND))))
#define va_end(ap)              (void) 0
#define va_start(ap, A)         (void) ((ap) = (((char *) &(A)) + (_bnd (A,_AUPBND))))

#endif


#if defined(__GNUC__)

//NOTE: [MIPS] Every parameter smaller than 32 bits is promoted to 32 bits.
//NOTE: [MIPS] 64-bit parameters are 8-byte aligned.
//Refernece: http://www.mips.com/media/files/MD00565-2B-MIPS32-QRC-01.01.pdf
//MIPS32 Instruction Set Quick Reference
//see "DEFAULT C CALLING CONVENTION"

/* Generic rounding macros. _size_ must be power of 2 (or result is garbage) */
#define __TRUNC(_addr_, _size_) ((_addr_) & ~((_size_) - 1))
#define __ROUND(_addr_, _size_) __TRUNC((_addr_) + (_size_) - 1, (_size_))

#define va_start(_ap_, _v_) \
	(_ap_ = (char*) __builtin_next_arg (_v_))
#define va_arg(_ap_, _type_) \
	((_ap_ = (char *) ((__alignof__ (_type_) > 4 \
						? __ROUND((int)_ap_,8) : __ROUND((int)_ap_,4)) \
					   + __ROUND(sizeof(_type_),4))), \
	 *(_type_ *) (void *) (_ap_ - __ROUND(sizeof(_type_),4)))
#define va_end(list)

#endif

#endif
