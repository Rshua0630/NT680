#ifndef CYGONCE_ISO_WCHAR_H
#define CYGONCE_ISO_WCHAR_H

// Usage:         #include <wchar.h>
//

#include <pkgconf/isoinfra.h>          /* Configuration header */

/* INCLUDES */

typedef unsigned short  wchar_t;

// Need size_t definition from compiler
#define __need_size_t
#include <stddef.h>

#ifdef CYGBLD_ISO_WCSLEN_HEADER
# include CYGBLD_ISO_WCSLEN_HEADER
#endif

#endif