/**

    Some basic common type defines.

    @file       Type.h
    Copyright   Novatek Microelectronics Corp. 2014.  All rights reserved.

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License version 2 as
    published by the Free Software Foundation.
*/

#ifndef ___TYPE_H
#define ___TYPE_H

typedef void                VOID;
typedef char                CHAR;       ///< Character type (8 bits)
///< @note MIPS compilers historically made "char" variables default to unsigned.

typedef unsigned long long  UINT64;     ///< Unsigned 64 bits data type
typedef signed long long    INT64;      ///< Signed 64 bits data type
typedef unsigned int        UINT32;     ///< Unsigned 32 bits data type
typedef signed int          INT32;      ///< Signed 32 bits data type
typedef unsigned short      UINT16;     ///< Unsigned 16 bits data type
typedef signed short        INT16;      ///< Signed 16 bits data type
typedef unsigned char       UINT8;      ///< Unsigned 8 bits data type
typedef signed char         INT8;       ///< Signed 8 bits data type
typedef float               FLOAT;      ///< Floating point integer

//typedef unsigned int        UBITFIELD;  ///< Unsigned bit field
//typedef signed int          BITFIELD;   ///< Signed bit field
typedef unsigned int        BOOL;       ///< Boolean
typedef unsigned int        ER;         ///< Boolean

/*
// For BOOL type
enum
{
    FALSE   = 0,
    TRUE    = 1,
};
*/

/**
    Macro to generate dummy element for enum type to expand enum size to word (4 bytes)
*/
//@{
#define ENUM_DUMMY4WORD(name)   E_##name = 0x10000000
//@}

//#define E_OK                0           ///< Normal completion
//#define E_PAR               (-33)       ///< Parameter error

#define WAY_SELECT_CNT                  4
#define WAY_SELECT_SHIFT                12          // 4 way ==> 16384 / 4 = 4096 = 2 ^ "12"
#define CACHE_SET_NUMBER                128
#define CACHE_SET_NUMBER_SHIFT          5           //32bytes per set

#define DBG_DUMP(...) printk(__VA_ARGS__)
#define DBG_WRN(...) printk(__VA_ARGS__)
#define DBG_ERR(...) printk(__VA_ARGS__)

#endif