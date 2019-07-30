/*
    Copyright (c) 2014  Novatek Microelectronics Corporation

    @file FsIpc.h

    @version

    @date
*/

#ifndef _FSIPCAPI_H
#define _FSIPCAPI_H

#include "Type.h"
/**
     FsIpc object structure.

     Defines the information for opening FsIpc
*/
typedef struct {
	UINT32  uiMemPoolAddr;  ///< shared memory address
	UINT32  uiMemPoolSize;  ///< shared memory size
} FSIPC_OPEN_OBJ;

/**
    Install FsIpc flag and semaphore id.

    @return void.
*/
extern void FsIpc_InstallID(void) _SECTION(".kercfg_text");

/**
    Open FsIpc task.

    This is FsIpc initialization function.

    @return
        - @b E_PS_OK if success, others fail
*/
extern ER   FsIpc_Open(FSIPC_OPEN_OBJ *pObj);

/**
    Close FsIpc task.

    @return
        - @b E_PS_OK if success, others fail
*/
extern ER   FsIpc_Close(void);

/**
    Mount eCos fs.

    @return
        - @b E_PS_OK if success, others fail
*/
extern ER   FsIpc_Mount(CHAR *pRootPath);

/**
    Umount eCos fs.

    @return
        - @b E_PS_OK if success, others fail
*/
extern ER   FsIpc_Umount(void);

/**
    Convert the unix time stamp to the dos format

    @return void.
*/
void FsIpc_Date_Unix2Dos(UINT32 UnixTimeStamp, UINT16 *OutDosTime, UINT16 *OutDosDate);

#endif /* _FSIPCAPI_H  */
