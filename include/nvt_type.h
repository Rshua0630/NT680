#ifndef _NVTTYPE_H
#define _NVTTYPE_H
typedef void                    VOID;
typedef int                     BOOLEAN;
typedef unsigned long long      UINT64;
typedef long long               INT64;
typedef unsigned long           UINT32;
typedef long                    INT32;
typedef unsigned short          UINT16;
typedef short                   INT16;
typedef unsigned char           UINT8;
typedef char                    INT8;
typedef unsigned int            UINT;
typedef int                     INT;
typedef float                   FLOAT;
typedef double                  DOUBLE;
typedef char                    CHAR;
typedef unsigned int            BOOL;
typedef unsigned int            ER;             ///< Error code
typedef INT32                   NVTRET;

#ifndef DISABLE
#define DISABLE             0           ///< Feature is disabled
#endif

#ifndef ENABLE
#define ENABLE              1           ///< Feature is enabled
#endif

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

/**
    Macro to generate dummy element for enum type to expand enum size to word (4 bytes)
*/
//@{
#define ENUM_DUMMY4WORD(name)   E_##name = 0x10000000
//@}

#define NVT_FALSE   0
#define NVT_TRUE    1

/* Error Code Define */
typedef enum {
    ER_FUNC_STOP_FAIL=-10005,
    ER_FUNC_START_FAIL=-10004,
    ER_FUNC_NOT_STOP=-10003,
    ER_FUNC_NOT_START=-10002,
    ER_INVALID_CHANNEL=-10001,
    ER_STORAGE_WRITE_FAIL=-203,
    ER_STORAGE_READ_FAIL=-202,
    ER_STORAGE_OPEN_FAIL=-201,
    ER_IPC_EXCHANGE_FAIL=-111,
    ER_END_BY_INTERRUPT=-110,
    ER_RESOURCE_OUT_OF_RANGE=-109,
    ER_RESOURCE_NOT_EXIST=-108,
    ER_RESOURCE_NOT_INIT=-107,
    ER_RESOURCE_RELEASE_FAIL=-106,
    ER_ITEM_NOT_SUPPORT=-105,
    ER_GET_DATA_FAIL=-104,
    ER_TIME_OUT=-103,
    ER_MEM_ALLOC_FAIL=-102,
    ER_INVALID_INPUT_DATA=-101,
    ER_UNCLASSIFIED_ERROR=-1,
    ER_SUCCESS=0,
} NvtErrorCode;

/**
    @name Align macros

    Align floor, align round, and align ceil

    @note base must be 2^n, where n = 1, 2, 3, ...
*/
//@{
#define ALIGN_FLOOR(value, base)  ((value) & ~((base)-1))                   ///< Align Floor
#define ALIGN_ROUND(value, base)  ALIGN_FLOOR((value) + ((base)/2), base)   ///< Align Round
#define ALIGN_CEIL(value, base)   ALIGN_FLOOR((value) + ((base)-1), base)   ///< Align Ceil

#endif
