/*
    Driver Register Cache Word (RCW) operation header file.

    Driver Register Cache Word (RCW) operation header file.
    The macros defined in this header file is used to simply the register
    bit field read and write.  By adapting this way of register access,
    it shall provide more error-proof and more readable way to write
    drivers.  It also does register access in load/store way which allow
    one to accurately control the number of counts the register read and
    write is proceeded from code writing.

    @file       RCWMacro.h
    @ingroup    mIDriver
    @note       Nothing.

    Copyright   Novatek Microelectronics Corp. 2010.  All rights reserved.
*/

#ifndef _RCWMACRO_H
#define _RCWMACRO_H

#include "IOAddress.h"

//type.h
#define DISABLE 0
#define ENABLE  1
#define ENUM_DUMMY4WORD(name)   E_##name = 0x10000000

typedef u32 UINT32;
typedef u32 UBITFIELD;  ///< Unsigned bit field

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

#define  dma_getPhyAddr(addr) ((addr) & 0x1FFFFFFF)

/**
    @addtogroup mIDriver
*/
//@{

//
// Register word value type
//
// The size in bits of this type should match the RCW type
#define REGVALUE                UINT32

//
// Register access log switch
//
// DISABLE: Disable log (Direct access)
// ENABLE: Enable
#define LOG_REGISTER            DISABLE

#if (LOG_REGISTER == ENABLE)
// log I/O version
    extern void                 outw(REGVALUE *addr, REGVALUE value);
    extern REGVALUE             inw(REGVALUE *addr);
    #define OUTW(addr,value)    outw((REGVALUE *)(addr), (REGVALUE)(value))
    #define INW(addr)           inw((REGVALUE *)(addr))
#else
// Direct I/O version
    #define OUTW(addr,value)    (*(REGVALUE volatile *)(addr) = (REGVALUE)(value))
    #define INW(addr)           (*(REGVALUE volatile *)(addr))
#endif

//
// Macros for Register Cache Word (RCW) type definition
//
// Each RCW type should be exactly the same size with REGVALUE type
// For example, to declare a Register Cache Word type:
//
//     #define rcwname_OFS   0x00   /* the name of RCW corresponding register address offset
//                                     should be in specific format with "_OFS" appended */
//     REGDEF_BEGIN(rcwname)
//         REGDEF_BIT(field1, 8)    /* declare field1 as 8 bits width */
//         REGDEF_BIT(field2, 8)    /* declare field1 as 8 bits width */
//         REGDEF_BIT(, 16)         /* pad reserved (not-used) bits to fill RCW type same as REGVALUE size */
//     REGDEF_END(rcwname)
//
// Register Cache Word type defintion header
#define REGDEF_BEGIN(name)      \
typedef union                   \
{                               \
    REGVALUE    Reg;            \
    struct                      \
    {

// Register Cache Word bit defintion
#define REGDEF_BIT(field, bits) \
    UBITFIELD   field : bits;

// Register Cache Word type defintion trailer
#define REGDEF_END(name)        \
    } Bit;                      \
} T_##name;                     \
STATIC_ASSERT(sizeof(T_##name) == sizeof(REGVALUE));

// Macro to define register offset
#define REGDEF_OFFSET(name, ofs)        static const UINT32 name##_OFS=(ofs);

//
// Macros for prerequisite stuff initialization for Register Cache Word
// operations
//
// Macro to set register base address for current module
// One must call this macro somewhere to set register I/O base address for
// current module, either globally or locally before any RCW operations is
// invoked
// What this macro set is only effective in current C file
#define REGDEF_SETIOBASE(base)          static const UINT32 _REGIOBASE=(base)

// Macro to set register repeat group offset address in byte
// This macro is optional and only used where register with repeat group exist,
// only one repeat group is allowed in one file
// If registers with repeat group exist, then one only have to define the RCW type
// for the first group, respective group of registers can be accessed through
// RCW_LD2, RCW_ST2, RCW_LD2OF macros
// What this macro set is only effective in current C file
#define REGDEF_SETGRPOFS(grpofs)        static const UINT32 _REGGRPOFS=(grpofs)

//
// Macros for Register Cache Word (RCW) operations
//
// These RCW operations are most likely to be used inside C functions
// It is not recommended to use RCW_DEF(RCW) in global scope which
// declare RCW as global variable
// RCW_LD/RCW_ST/RCW_LDOF pair is for normal single register access while
// RCW_LD2/RCW_ST2/RCW_LD2OF pair is for repeat group register access
// Beware that the RCW_LD/RCW_ST/RCW_LDOF and RCW_LD2/RCW_ST2/RCW_LD2OF
// must not be mixed used. e.g. use RCW_LD then RCW_ST2 or use RCW_LD2 then RCW_ST
//
// Register Cache Word declaration
#define RCW_DEF(RCW)            T##_##RCW t##RCW
// Register Cache Word read from I/O
#define RCW_LD(RCW)             t##RCW.Reg=INW(_REGIOBASE+RCW##_OFS)
// Register Cache Word repeat from I/O for group read
#define RCW_LD2(RCW,grpidx)     t##RCW.Reg=INW(_REGIOBASE+RCW##_OFS+(grpidx)*_REGGRPOFS)
// Register Cache Word bit reference
#define RCW_OF(RCW)             t##RCW.Bit
// Register Cache Word reference
#define RCW_VAL(RCW)            t##RCW.Reg
// Register Cache Word write to I/O
#define RCW_ST(RCW)             OUTW(_REGIOBASE+RCW##_OFS,t##RCW.Reg)
// Register Cache Word write to I/O for repeat group
#define RCW_ST2(RCW,grpidx)     OUTW(_REGIOBASE+RCW##_OFS+(grpidx)_REGGRPOFS,t##RCW.Reg)
// Register Cache Word read and reference to register cache word bit
// !!!need to check for compiler if support for this syntax
#define RCW_LDOF(RCW)           ((T##_##RCW)(RCW_LD(RCW))).Bit
// Register Cache Word read and reference to register cache word bit for repeat group
// !!!need to check for compiler if support for this syntax
#define RCW_LD2OF(RCW,grpidx)   ((T##_##RCW)(RCW_LD2(RCW,grpidx))).Bit

//@}

#endif
