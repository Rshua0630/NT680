/* sys/sysmacros.h

   Copyright 1998, 2001 Red Hat, Inc.

This file is part of Cygwin.

This software is a copyrighted work licensed under the terms of the
Cygwin license.  Please consult the file "CYGWIN_LICENSE" for
details. */

#ifndef _SYS_SYSMACROS_H
#define _SYS_SYSMACROS_H

#include <cyg/hal/hal_endian.h>

#if (CYG_BYTEORDER == CYG_MSBFIRST)
#define major(dev) ((int)(((dev) >> 16) & 0xffff))
#define minor(dev) ((int)((dev) & 0xffff))
#define mkdev(major, minor) (((major) << 16) | ((minor) & 0xffff))
#elif (CYG_BYTEORDER == CYG_LSBFIRST)
#define major(dev) ((int)(((dev) >> 8) & 0xff))
#define minor(dev) ((int)((dev) & 0xff))
#define mkdev(major, minor) (((major) << 8) | ((minor) & 0xff))
#else
#error Endianess not defined by configuration
#endif

#endif /* _SYS_SYSMACROS_H */
