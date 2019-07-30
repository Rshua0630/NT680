#ifndef _NVT_LIBC_STDINT_H
#define _NVT_LIBC_STDINT_H

// Exact-width integer
typedef signed char         int8_t;
typedef signed short        int16_t;
typedef signed int          int32_t;
typedef signed long long    int64_t;

typedef unsigned char       uint8_t;
typedef unsigned short      uint16_t;
typedef unsigned int        uint32_t;
typedef unsigned long long  uint64_t;

// Minimum-width integer types
typedef int8_t              int_least8_t;
typedef int16_t             int_least16_t;
typedef int32_t             int_least32_t;
typedef int64_t             int_least64_t;

typedef uint8_t             uint_least8_t;
typedef uint16_t            uint_least16_t;
typedef uint32_t            uint_least32_t;
typedef uint64_t            uint_least64_t;

// Fastest minimum-width integer types
typedef signed int          int_fast8_t;
typedef signed int          int_fast16_t;
typedef signed int          int_fast32_t;
typedef signed long long    int_fast64_t;

typedef unsigned int        uint_fast8_t;
typedef unsigned int        uint_fast16_t;
typedef unsigned int        uint_fast32_t;
typedef unsigned long long  uint_fast64_t;

// Integer types capable of holding object pointers
typedef signed int          intptr_t;
typedef unsigned int        uintptr_t;

#endif
