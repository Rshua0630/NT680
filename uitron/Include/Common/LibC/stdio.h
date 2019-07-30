#ifndef _NVT_LIBC_STDIO_H
#define _NVT_LIBC_STDIO_H

#include <stdarg.h>
#include <stddef.h>

extern int  sprintf(char *buf, const char *fmt, ...);
extern int  vsprintf(char *buf, const char *fmt, va_list args);

extern int  snprintf(char *buf, size_t size, const char *fmt, ...);
extern int  vsnprintf(char *buf, size_t size, const char *fmt, va_list args);

extern int  sscanf(const char *buf, const char *fmt, ...);
extern int  vsscanf(const char *buf, const char *fmt, va_list args);

extern int  vsscanf_s(const char *inp, char const *fmt0, va_list ap);
extern int  sscanf_s(const char *ibuf, const char *fmt, ...);


#endif
