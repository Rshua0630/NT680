/**
    Type declaration

    Type declaration.

    @file       Type.h
    @ingroup    mICOM
    @note       Nothing.

    Copyright   Novatek Microelectronics Corp. 2005.  All rights reserved.
*/

#ifndef _TYPE_H
#define _TYPE_H

#include "StdCMac.h"
#include "Platform.h"       //include syntax cross different platform: ARM, or MIPS
#include "mIType.h"         //include kernel usage type

#define __ARM_CORTEX__ 0
/**
    @addtogroup mICOM
*/
//@{
//------------------------------------------------------------------------------
// Typedefs
//------------------------------------------------------------------------------
#ifdef __ECOS
#define VOID void
#else
typedef void VOID;
#endif
typedef char                CHAR;       ///< Character type (8 bits)
///< @note MIPS compilers historically made "char" variables default to unsigned.

typedef unsigned long long  UINT64;     ///< Unsigned 64 bits data type
typedef signed long long    INT64;      ///< Signed 64 bits data type
typedef unsigned long       UINT32;     ///< Unsigned 32 bits data type
typedef signed long         INT32;      ///< Signed 32 bits data type
typedef unsigned short      UINT16;     ///< Unsigned 16 bits data type
typedef signed short        INT16;      ///< Signed 16 bits data type
typedef unsigned char       UINT8;      ///< Unsigned 8 bits data type
typedef signed char         INT8;       ///< Signed 8 bits data type
typedef float               FLOAT;      ///< Floating point integer

typedef unsigned int        UBITFIELD;  ///< Unsigned bit field
typedef signed int          BITFIELD;   ///< Signed bit field


//------------------------------------------------------------------------------
// Macros
//------------------------------------------------------------------------------
// NULL
#ifndef NULL
#define NULL                ((void*)0)
#endif

#define NO_TASK_LOCKED      0           ///< The resource is not locked by any task
#define TASK_LOCKED         1           ///< The resource is locked by some task

#define DISABLE             0           ///< Feature is disabled
#define ENABLE              1           ///< Feature is enabled

#define OFF                 0           ///< Feature is OFF
#define ON                  1           ///< Feature is ON

/**
    Macro to generate dummy element for enum type to expand enum size to word (4 bytes)
*/
//@{
#define ENUM_DUMMY4WORD(name)   E_##name = 0x10000000
//@}

/**
    @name Align macros

    Align floor, align round, and align ceil

    @note base must be 2^n, where n = 1, 2, 3, ...
*/
//@{
#define ALIGN_FLOOR(value, base)  ((value) & ~((base)-1))                   ///< Align Floor
#define ALIGN_ROUND(value, base)  ALIGN_FLOOR((value) + ((base)/2), base)   ///< Align Round
#define ALIGN_CEIL(value, base)   ALIGN_FLOOR((value) + ((base)-1), base)   ///< Align Ceil
//@}

/**
    @name Align (round off)

    Round Off to 32, 16, 8 and 4

    @note Example: ALIGN_ROUND_32(32) = 32, ALIGN_ROUND_32(47) = 32, ALIGN_ROUND_32(48) = 64
*/
//@{
#define ALIGN_ROUND_64(a)       ALIGN_ROUND(a, 64)  ///< Round Off to 64
#define ALIGN_ROUND_32(a)       ALIGN_ROUND(a, 32)  ///< Round Off to 32
#define ALIGN_ROUND_16(a)       ALIGN_ROUND(a, 16)  ///< Round Off to 16
#define ALIGN_ROUND_8(a)        ALIGN_ROUND(a, 8)   ///< Round Off to 8
#define ALIGN_ROUND_4(a)        ALIGN_ROUND(a, 4)   ///< Round Off to 4
//@}

/**
    @name Align (round up)

    Round Up to 32, 16, 8 and 4.

    @note Example: ALIGN_CEIL_32(32) = 32, ALIGN_CEIL_32(33) = 64, ALIGN_CEIL_32(63) = 64
*/
//@{
#define ALIGN_CEIL_64(a)        ALIGN_CEIL(a, 64)   ///< Round Up to 64
#define ALIGN_CEIL_32(a)        ALIGN_CEIL(a, 32)   ///< Round Up to 32
#define ALIGN_CEIL_16(a)        ALIGN_CEIL(a, 16)   ///< Round Up to 16
#define ALIGN_CEIL_8(a)         ALIGN_CEIL(a, 8)    ///< Round Up to 8
#define ALIGN_CEIL_4(a)         ALIGN_CEIL(a, 4)    ///< Round Up to 4
//@}

/**
    @name Align (round down)

    Round Down to 32, 16, 8 and 4.

    @note Example: ALIGN_FLOOR_32(32) = 32, ALIGN_FLOOR_32(33) = 32, ALIGN_FLOOR_32(63) = 32
*/
//@{
#define ALIGN_FLOOR_64(a)       ALIGN_FLOOR(a, 64)  ///< Round down to 64
#define ALIGN_FLOOR_32(a)       ALIGN_FLOOR(a, 32)  ///< Round down to 32
#define ALIGN_FLOOR_16(a)       ALIGN_FLOOR(a, 16)  ///< Round down to 16
#define ALIGN_FLOOR_8(a)        ALIGN_FLOOR(a, 8)   ///< Round down to 8
#define ALIGN_FLOOR_4(a)        ALIGN_FLOOR(a, 4)   ///< Round down to 4
//@}

/**
    Convert four characters into a four-character code.
*/
//@{
#define MAKEFOURCC(ch0, ch1, ch2, ch3) ((UINT32)(UINT8)(ch0) | ((UINT32)(UINT8)(ch1) << 8) | ((UINT32)(UINT8)(ch2) << 16) | ((UINT32)(UINT8)(ch3) << 24 ))
//@}

/**
    @name Assert macros
*/
//@{
#define ASSERT_CONCAT_(a, b)    a##b
#define ASSERT_CONCAT(a, b)     ASSERT_CONCAT_(a, b)

#if defined(__COUNTER__)

#define STATIC_ASSERT(expr) \
	enum { ASSERT_CONCAT(FAILED_STATIC_ASSERT_, __COUNTER__) = 1/(expr) }

#else

// This might cause compile error when writing STATIC_ASSERT at the same line
// in two (or more) files and one file include another one.
#define STATIC_ASSERT(expr) \
	enum { ASSERT_CONCAT(FAILED_STATIC_ASSERT_, __LINE__) = 1/(expr) }

#endif
//@}


//------------------------------------------------------------------------------
// Data Structures
//------------------------------------------------------------------------------
/**
    Structure for point

    Structure for point.
*/
typedef struct {
	UINT32  x;      ///< X coordinate of the point
	UINT32  y;      ///< Y coordinate of the point
} UPOINT, *PUPOINT;

/**
    Structure for rectangle size

    Structure for rectangle size.
*/
typedef struct {
	UINT32  w;      ///< Rectangle width
	UINT32  h;      ///< Rectangle height
} USIZE, *PUSIZE;

/**
    Structure for rectangle

    Structure for rectangle.
*/
typedef struct {
	UINT32  x;      ///< X coordinate of the top-left point of the rectangle
	UINT32  y;      ///< Y coordinate of the top-left point of the rectangle
	UINT32  w;      ///< Rectangle width
	UINT32  h;      ///< Rectangle height
} URECT, *PURECT;

/**
    Structure for point

    Structure for point.
*/
typedef struct {
	INT32  x;       ///< X coordinate of the point
	INT32  y;       ///< Y coordinate of the point
} IPOINT, *PIPOINT;

/**
    Structure for rectangle size

    Structure for rectangle size.
*/
typedef struct {
	INT32  w;       ///< Rectangle width
	INT32  h;       ///< Rectangle height
} ISIZE, *PISIZE;

/**
    Structure for rectangle

    Structure for rectangle.
*/
typedef struct {
	INT32  x;       ///< X coordinate of the top-left point of the rectangle
	INT32  y;       ///< Y coordinate of the top-left point of the rectangle
	INT32  w;       ///< Rectangle width
	INT32  h;       ///< Rectangle height
} IRECT, *PIRECT;

/**
    Structure for memory range

    Structure for memory range.
*/
typedef struct _MEM_RANGE {
	UINT32 Addr;    ///< Memory buffer starting address
	UINT32 Size;    ///< Memory buffer size
} MEM_RANGE, *PMEM_RANGE;
//@}

#endif
