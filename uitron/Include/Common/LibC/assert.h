#ifndef _NVT_LIBC_ASSERT_H
#define _NVT_LIBC_ASSERT_H

#ifdef NDEBUG
#define assert(__e) ((void)0)
#else
#define assert(__e) ((__e) ? (void)0 : __assert_func (__FILE__, __LINE__, __FUNCTION__, #__e))
#endif /* !NDEBUG */

void __assert_func(const char *, int, const char *, const char *) __attribute__((__noreturn__));

#endif