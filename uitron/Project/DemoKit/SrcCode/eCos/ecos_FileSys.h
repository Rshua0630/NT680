#ifndef _ECOS_FILESYS_H
#define _ECOS_FILESYS_H

#include <sys/types.h>
#include "Type.h"

void cyg_FileSys_RegisterCB(void);
void date_unix2dos(UINT32 unixtimestamp, UINT16 *outdostime, UINT16 *outdosdate);
void listdir( char *name, int statp);
#endif