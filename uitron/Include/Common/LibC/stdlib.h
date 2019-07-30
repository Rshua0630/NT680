#ifndef _NVT_LIBC_STDLIB_H
#define _NVT_LIBC_STDLIB_H

#include <stddef.h>

#define EXIT_SUCCESS    0
#define EXIT_FAILURE    1
#define RAND_MAX        2147483647    /* maximum (signed) int value */

extern int      atexit(void (* function)(void));
extern int      exit(int status);
extern void     abort(void);

extern void     srand(unsigned int seed);
extern int      rand(void);

extern int      atoi(const char *nptr);
extern long     atol(const char *nptr);

extern void    *bsearch(const void *key, const void *base, size_t num, size_t size, int (*compar)(const void *, const void *));
extern void     qsort(void *base, size_t num, size_t size, int (*compar)(const void *, const void *));

extern int      abs(int j);

#endif
