#ifndef CYGONCE_CYG_FS_PSTORE_H
#define CYGONCE_CYG_FS_PSTORE_H
#include <sys/stat.h>
//=============================================================================
//
//      pstorefs.h
//
//      Novatek FS attributes and stat information
//
//=============================================================================
//####ECOSGPLCOPYRIGHTBEGIN####
// -------------------------------------------
// This file is part of eCos, the Embedded Configurable Operating System.
// Copyright (C) 2004 Red Hat, Inc.
// Copyright (C) 2005 eCosCentric Limited
//
// eCos is free software; you can redistribute it and/or modify it under
// the terms of the GNU General Public License as published by the Free
// Software Foundation; either version 2 or (at your option) any later version.
//
// eCos is distributed in the hope that it will be useful, but WITHOUT ANY
// WARRANTY; without even the implied warranty of MERCHANTABILITY or
// FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
// for more details.
//
// You should have received a copy of the GNU General Public License along
// with eCos; if not, write to the Free Software Foundation, Inc.,
// 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA.
//
// As a special exception, if other files instantiate templates or use macros
// or inline functions from this file, or you compile this file and link it
// with other works to produce a work based on this file, this file does not
// by itself cause the resulting work to be covered by the GNU General Public
// License. However the source code for this file must still be made available
// in accordance with section (3) of the GNU General Public License.
//
// This exception does not invalidate any other reasons why a work based on
// this file might be covered by the GNU General Public License.
//
// Alternative licenses for eCos may be arranged by contacting Red Hat, Inc.
// at http://sources.redhat.com/ecos/ecos-license/
// -------------------------------------------
//####ECOSGPLCOPYRIGHTEND####
//=============================================================================
//#####DESCRIPTIONBEGIN####
//
// Author(s):     David Brennam <eCos@brennanhome.com>
// Contributors:
// Date:          2004-10-22
// Purpose:
// Description:   This header contains attributes and stat like mode
//                information for FAT filesystems.
//
// Usage:
//              #include <cyg/fs/pstorefs.h>
//
//
//####DESCRIPTIONEND####
//
//=============================================================================

// -------------------------------------------------------------------------
// FAT filesystem dir entry attributes
typedef char*   TYPE_PStore_GetVerInfo(void);
typedef char*   TYPE_PStore_GetBuildDate(void);

typedef int     TYPE_PStore_OpenSection( const char *pSecName, int oflag, int* pSecSize);//open
typedef int     TYPE_PStore_CloseSection( int fd );//close
typedef int     TYPE_PStore_ReadSection( int fd, void *buf, ssize_t len, int offset);//read
typedef int     TYPE_PStore_WriteSection( int fd, const void *buf, ssize_t len, int offset);//write
typedef int     TYPE_PStore_StatSection( int fd, struct stat *pStat );//fstat
typedef int     TYPE_PStore_DeleteSection( const char *pSecName);//unlink

typedef int     TYPE_PStore_Stat( const char *pSecName, struct stat *pStat);//stat

typedef bool    TYPE_PStore_SearchSectionOpen(void);//opendir
typedef bool    TYPE_PStore_SearchSection( char *pName, ssize_t len);//readdir
typedef bool    TYPE_PStore_SearchSectionRewind(void);//rewinddir
typedef int     TYPE_PStore_SearchSectionClose(void);//closedir

//File System callback structure
typedef struct _PStore_CBS
{
    TYPE_PStore_GetVerInfo          *pf_PStore_GetVerInfo;
    TYPE_PStore_GetBuildDate        *pf_PStore_GetBuildDate;
    TYPE_PStore_OpenSection         *pf_PStore_OpenSection;
    TYPE_PStore_CloseSection        *pf_PStore_CloseSection;
    TYPE_PStore_ReadSection         *pf_PStore_ReadSection;
    TYPE_PStore_WriteSection        *pf_PStore_WriteSection;
    TYPE_PStore_StatSection         *pf_PStore_StatSection;
    TYPE_PStore_DeleteSection       *pf_PStore_DeleteSection;
    TYPE_PStore_Stat                *pf_PStore_Stat;
    TYPE_PStore_SearchSectionOpen   *pf_PStore_SearchSectionOpen;
    TYPE_PStore_SearchSection       *pf_PStore_SearchSection;
    TYPE_PStore_SearchSectionRewind *pf_PStore_SearchSectionRewind;
    TYPE_PStore_SearchSectionClose  *pf_PStore_SearchSectionClose;
}PStore_CBS;

/**
    Register Novatek PStore callback functions to eCos file system "pstorefs"

    @return void
*/
__externC void PStoreECOS_RegisterCB( PStore_CBS* pCBS);

#endif // CYGONCE_CYG_FS_PSTORE_H
// End of pstorefs.h

