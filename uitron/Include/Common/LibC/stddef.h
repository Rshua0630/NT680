#ifndef _NVT_LIBC_STDDEF_H
#define _NVT_LIBC_STDDEF_H

typedef long            ptrdiff_t;
typedef unsigned int    size_t;
typedef unsigned short  wchar_t;

#undef NULL
#define NULL (void *)0

#undef offsetof
#define offsetof(type, field) ((size_t)&(((type *)0)->field))

#endif
