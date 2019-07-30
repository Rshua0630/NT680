#ifndef _NVT_LIBC_MALLOC_H
#define _NVT_LIBC_MALLOC_H

#include <stddef.h>

#ifdef _WIN32
#pragma warning ( disable : 4761 )
#endif

#ifdef __cplusplus
extern "C" {
#endif

typedef void (*_heap_fp)(void);

extern void     _heap_init(void *pBuf, size_t uiSize, _heap_fp fpLock, _heap_fp fpUnlock);
extern void     _heap_exit(void);
extern void     _heap_dump(void);
extern size_t   _heap_getfree(void);

extern void    *malloc(size_t uiSize);
extern void    *realloc(void *pBuf, size_t uiSize);
extern void    *calloc(unsigned int uiCount, size_t uiSize);
extern void     free(void *pBuf);

#ifdef __cplusplus
}
#endif

#endif
