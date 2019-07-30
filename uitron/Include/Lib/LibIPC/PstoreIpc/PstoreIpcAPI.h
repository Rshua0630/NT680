/////////////////////////////////////////////////////////////////
/*
    Copyright (c) 2014~  Novatek Microelectronics Corporation

    @file PstoreIpcAPI.h

    @version

    @date

*//////////////////////////////////////////////////////////////////
#ifndef _PSTOREIPCAPI_H
#define _PSTOREIPCAPI_H

/**
   Warning Code.

   Any warning code occured will display on uart or return.
*/
typedef enum _PSTOREIPC_WR {
	PSTOREIPC_WR_OK                =   0, ///< no warning
	PSTOREIPC_WR_ALREADY_OPEN      =  -1, ///< PstoreIpc_Open called twice
	PSTOREIPC_WR_NOT_OPEN          =  -2, ///< PstoreIpc_Open not to call before.
	ENUM_DUMMY4WORD(PSTOREIPC_WR)
} PSTOREIPC_WR;



/**
    Init parameters for PstoreIpc module.

    This structure is used in PstoreIpc_Open() to specify the HFSNvt task needed information for normal working
    and user customization callback.
*/
typedef struct {
	UINT32                  sharedMemAddr;                           ///<  shared memory address
	UINT32                  sharedMemSize;                           ///<  shared memory size
} PSTOREIPC_OPEN;
/*-----------------------------------
    functions declare
-----------------------------------*/

/**
     Install PstoreIpc flag and semaphore id.

*/
extern void       PstoreIpc_InstallID(void) _SECTION(".kercfg_text");

/**
    Open PstoreIpc task.

    This is the PstoreIpc task initial function.

    @param[in] pOpen:  Init info for PstoreIpc.

    @return
        - @b E_OK: initial success.
        - @b E_PAR: the initial parameter has some error.

    Example:
    @code

    @endcode

*/
extern ER     PstoreIpc_Open(PSTOREIPC_OPEN *pOpen);

/**
    Close PstoreIpc task.

    @return
        - @b E_OK: close success.

*/
extern ER     PstoreIpc_Close(void);


#endif  // _PSTOREIPCAPI_H

