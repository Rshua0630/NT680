/*
    Emulated main function processing.

    Stephen H.L.Wang
 */

#ifndef _MAINFUNC_H
#define _MAINFUNC_H

#if 0
//=======

#ifndef CYG_LABEL_NAME

#define CYG_LABEL_NAME(_name_) _name_

#endif

#ifndef CYG_LABEL_DEFN

#define CYG_LABEL_DEFN(_label) _label

#endif

# if !defined(CYGBLD_ATTRIB_SECTION)
#  define CYGBLD_ATTRIB_SECTION(__sect__) __attribute__((section (__sect__)))
# endif


# if !defined(CYGBLD_ATTRIB_ALIGN)
#  define CYGBLD_ATTRIB_ALIGN(__align__) __attribute__((aligned(__align__)))int mainfunc(char *name, mainfunc_t pmainfunc, char *commandlineArg)

# endif

#ifndef CYGARC_ALIGNMENT
# define CYGARC_ALIGNMENT 32
#endif
// And corresponding power of two alignment
#ifndef CYGARC_P2ALIGNMENT
# define CYGARC_P2ALIGNMENT 5
#endif
#if (CYGARC_ALIGNMENT) != (1 << CYGARC_P2ALIGNMENT)
# error "Inconsistent CYGARC_ALIGNMENT and CYGARC_P2ALIGNMENT values"
#endif



#define __string(_x) #_x
#define __xstring(_x) __string(_x)

#ifdef __i386__

//    ".def  " __xstring(CYG_LABEL_DEFN(_label)) "; .scl 2; .type 32; .endef\n"                

#ifndef CYG_MAIN_TABLE_BEGIN
#define CYG_MAIN_TABLE_BEGIN( _label, _name )                                 \
__asm__(".section .main.table." __xstring(_name) ".begin,\"aw\"\n"       \
    ".globl " __xstring(CYG_LABEL_DEFN(_label)) "\n"                         \
    ".p2align " __xstring(CYGARC_P2ALIGNMENT) "\n"                           \
__xstring(CYG_LABEL_DEFN(_label)) ":\n"                                      \
       )
#endif

#ifndef CYG_MAIN_TABLE_END
#define CYG_MAIN_TABLE_END( _label, _name )                                   \
__asm__(".section .main.table." __xstring(_name) ".finish,\"aw\"\n"      \
    ".globl " __xstring(CYG_LABEL_DEFN(_label)) "\n"                         \
    ".p2align " __xstring(CYGARC_P2ALIGNMENT) "\n"                           \
__xstring(CYG_LABEL_DEFN(_label)) ":\n"                                      \
       )
#endif

#else

#ifndef CYG_MAIN_TABLE_BEGIN
#define CYG_MAIN_TABLE_BEGIN( _label, _name )                                 \
__asm__(".section \".main.table." __xstring(_name) ".begin\",\"aw\"\n"       \
    ".globl " __xstring(CYG_LABEL_DEFN(_label)) "\n"                         \
    ".type    " __xstring(CYG_LABEL_DEFN(_label)) ",object\n"                \
    ".p2align " __xstring(CYGARC_P2ALIGNMENT) "\n"                           \
__xstring(CYG_LABEL_DEFN(_label)) ":\n"                                      \
    ".previous\n"                                                            \
       )
#endif

#ifndef CYG_MAIN_TABLE_END
#define CYG_MAIN_TABLE_END( _label, _name )                                   \
__asm__(".section \".main.table." __xstring(_name) ".finish\",\"aw\"\n"      \
    ".globl " __xstring(CYG_LABEL_DEFN(_label)) "\n"                         \
    ".type    " __xstring(CYG_LABEL_DEFN(_label)) ",object\n"                \
    ".p2align " __xstring(CYGARC_P2ALIGNMENT) "\n"                           \
__xstring(CYG_LABEL_DEFN(_label)) ":\n"                                      \
    ".previous\n"                                                            \
       )
#endif


#endif //#ifdef __i386__


#ifndef CYG_MAIN_TABLE_ENTRY
#define CYG_MAIN_TABLE_ENTRY( _name ) \
        CYGBLD_ATTRIB_SECTION(".main.table." __xstring(_name) ".data")
#endif


// This macro must be applied to any types whose objects are to be placed in
// tables
#ifndef CYG_MAIN_TABLE_TYPE
#define CYG_MAIN_TABLE_TYPE CYGBLD_ATTRIB_ALIGN( CYGARC_ALIGNMENT )
#endif

#else
#include <cyg/hal/hal_tables.h>

#define CYG_MAIN_TABLE_BEGIN	CYG_HAL_TABLE_BEGIN
#define CYG_MAIN_TABLE_END	CYG_HAL_TABLE_END
#define CYG_MAIN_TABLE_ENTRY	CYG_HAL_TABLE_ENTRY
#define CYG_MAIN_TABLE_TYPE  	CYG_HAL_TABLE_TYPE
#endif

//---

typedef int (*mainfunc_t)(int argc, char **argv);

typedef struct mainfunc_entry {
    mainfunc_t pmain;
    char name[16];
} CYG_MAIN_TABLE_TYPE mainfunc_entry_t;

extern mainfunc_entry_t __MAINFUNCTAB__[], __MAINFUNCTAB_END__;

extern int mainfunc(char *name, mainfunc_t pmainfunc, char *commandlineArg);

#define MAINFUNC_ENTRY(_l,_argc,_argv)  \
externC int _l##_main(int _argc, char **_argv);			\
mainfunc_entry_t _l CYG_MAIN_TABLE_ENTRY(mainfunc) = {           \
   _l##_main,                                                   \
   #_l								\
};								\
externC int _l##_main(int _argc, char **_argv)			

#endif	//#ifndef _MAINFUNC_H

