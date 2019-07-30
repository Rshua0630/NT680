#ifndef _NVT_LIBC_STRING_H
#define _NVT_LIBC_STRING_H

#include <stdarg.h>
#include <stddef.h>

#define PLD_SPEEDUP  1

extern char    *strncpy(char *dest, const char *src, size_t count);
extern char    *strncat(char *dest, const char *src, size_t count);
extern char    *strstr(const char *s1, const char *s2);
extern char    *strrchr(const char *s, int c);
extern char    *strcpy(char *dest, const char *src);
extern char    *strcat(char *dest, const char *src);
extern char    *strchr(const char *s, int c);
extern char    *strtok(char *s, const char *delim);
extern size_t   strlen(const char *s);
extern int      strncmp(const char *cs, const char *ct, size_t count);
extern int      strcmp(const char *cs, const char *ct);

extern void    *memset(void *s, int c, size_t count);
extern void    *memcpy(void *dest, const void *src, size_t count);
#if (PLD_SPEEDUP == 1)
extern void    *neon_memcpy(void *dest, const void *src, size_t count);
extern void    *gen_memcpy(void *dest, const void *src, size_t count);
extern void    *gen_memset(void *s, int c, size_t count);
extern int      gen_memcmp(const void *cs, const void *ct, size_t count);
#else
extern void    *neon_memcpy(void *dest, const void *src, size_t count);
extern void    *pld_memcpy(void *dest, const void *src, size_t count);
extern void    *pld_memset(void *s, int c, size_t count);
extern int      pld_memcmp(const void *cs, const void *ct, size_t count);

#endif
extern void    *memmove(void *dest, const void *src, size_t count);
extern void    *memchr(const void *ptr, int value, size_t count);
extern int      memcmp(const void *cs, const void *ct, size_t count);

#endif
