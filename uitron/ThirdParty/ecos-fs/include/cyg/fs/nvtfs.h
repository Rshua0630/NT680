#ifndef CYGONCE_CYG_FS_NVT_H
#define CYGONCE_CYG_FS_NVT_H
#include <sys/stat.h>
//=============================================================================
//
//      nvtfs.h
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
//              #include <cyg/fs/nvtfs.h>
//
//
//####DESCRIPTIONEND####
//
//=============================================================================

// -------------------------------------------------------------------------
// FAT filesystem dir entry attributes
typedef char*   TYPE_FileSys_GetVerInfo(void);
typedef char*   TYPE_FileSys_GetBuildDate(void);

typedef int     TYPE_FileSys_OpenFile( const char *path, int oflag, bool bIsAbsPath);//open (with path)
typedef int     TYPE_FileSys_CloseFile( int fd );//close
typedef int     TYPE_FileSys_ReadFile( int fd, void *buf, ssize_t len );//read
typedef int     TYPE_FileSys_WriteFile( int fd, const void *buf, ssize_t len );//write
typedef int     TYPE_FileSys_SeekFile( int fd, off64_t pos, int whence );//lseek
typedef int     TYPE_FileSys_StatFile( int fd, struct stat *pStat );//fstat
typedef int     TYPE_FileSys_FlushFile( int fd );//fsync
typedef int     TYPE_FileSys_DeleteFile( const char *path, bool bIsAbsPath);//unlink (with path)
typedef int     TYPE_FileSys_RenameFile( const char *pNewName, const char *path, bool bIsAbsPath);//rename (with path)

typedef int     TYPE_FileSys_MakeDir( const char *path, bool bIsAbsPath );//mkdir (with path)
typedef int     TYPE_FileSys_DeleteDir( const char *path, bool bIsAbsPath );//rmdir (with path)

typedef int     TYPE_FileSys_Stat( const char *path, struct stat *pStat, bool bIsAbsPath);//stat (with path)
typedef int     TYPE_FileSys_GetAttrib( const char *path, cyg_uint32 *pAttrib, bool bIsAbsPath);//cyg_fs_get_attrib (with path)
typedef int     TYPE_FileSys_SetAttrib( const char *path, cyg_uint32 uiAttrib, bool bIsAbsPath);//cyg_fs_set_attrib (with path)

typedef char*   TYPE_fs_getcwd( void );//getcwd
typedef int     TYPE_fs_chdir( const char *path, bool bIsAbsPath );//chdir (with path)

typedef int     TYPE_fs_SearchFileOpen( const char *path, bool bIsAbsPath);//opendir (with path)
typedef int     TYPE_fs_SearchFile( int fd, char *pName, ssize_t len);//readdir
typedef int     TYPE_fs_SearchFileRewind( int fd );//rewinddir
typedef int     TYPE_fs_SearchFileClose( int fd );//closedir

//File System callback structure
typedef struct _FileSys_CBS
{
    TYPE_FileSys_GetVerInfo     *pf_FileSys_GetVerInfo;
    TYPE_FileSys_GetBuildDate   *pf_FileSys_GetBuildDate;
    TYPE_FileSys_OpenFile       *pf_FileSys_OpenFile;
    TYPE_FileSys_CloseFile      *pf_FileSys_CloseFile;
    TYPE_FileSys_ReadFile       *pf_FileSys_ReadFile;
    TYPE_FileSys_WriteFile      *pf_FileSys_WriteFile;
    TYPE_FileSys_SeekFile       *pf_FileSys_SeekFile;
    TYPE_FileSys_StatFile       *pf_FileSys_StatFile;
    TYPE_FileSys_FlushFile      *pf_FileSys_FlushFile;
    TYPE_FileSys_DeleteFile     *pf_FileSys_DeleteFile;
    TYPE_FileSys_RenameFile     *pf_FileSys_RenameFile;
    TYPE_FileSys_MakeDir        *pf_FileSys_MakeDir;
    TYPE_FileSys_DeleteDir      *pf_FileSys_DeleteDir;
    TYPE_FileSys_Stat           *pf_FileSys_Stat;
    TYPE_FileSys_GetAttrib      *pf_FileSys_GetAttrib;
    TYPE_FileSys_SetAttrib      *pf_FileSys_SetAttrib;
    TYPE_fs_getcwd              *pf_fs_getcwd;
    TYPE_fs_chdir               *pf_fs_chdir;
    TYPE_fs_SearchFileOpen      *pf_fs_SearchFileOpen;
    TYPE_fs_SearchFile          *pf_fs_SearchFile;
    TYPE_fs_SearchFileRewind    *pf_fs_SearchFileRewind;
    TYPE_fs_SearchFileClose     *pf_fs_SearchFileClose;
}FileSys_CBS;

/**
    Register Novatek File System callback functions to eCos file system "nvtfs"

    @return void
*/
__externC void FileSysECOS_RegisterCB( FileSys_CBS* pCBS);

#endif // CYGONCE_CYG_FS_NVT_H
// End of nvtfs.h

