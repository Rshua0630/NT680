/**
    Fastboot PStore driver export variables and function prototypes.

    Fastboot PStore is novatek original simple file system.
    For speeding up pstore read operations during boot process.

    @file       Fastboot_PStore.h
    @ingroup    mIFastbootPStore

    Copyright   Novatek Microelectronics Corp. 2018.  All rights reserved.
*/


#ifndef _FASTBOOTPSTORE_H
#define _FASTBOOTPSTORE_H

#include "Type.h"
#include "DxType.h"
/**
     @addtogroup mIFastbootPStore
*/

/**
    FastbootPStore header structure.
*/
typedef struct _FASTBOOTPSTORE
{
    UINT32 uiDataSize;              ///<section size (without header)
    UINT32 uiDataAddr;              ///<section addr
    UINT32 uiCheckSum1;              ///<make sure this header is intact. value = (uiDataSize | uiDataAddr)
    UINT32 uiCheckSum2;              ///<make sure section buffer is intact
} FASTBOOTPSTORE, *PFASTBOOTPSTORE;

/**
    system wide initialization

    @return
        - @b 0 if success
        - @b otherwise if fail
*/
extern int fastboot_pstore_init(UINT32 phyAddr);

/**
    read fastboot pstore's section data(fastboot only has single section)

    @return
        - @b none 0 if success
        - @b 0 if fail
*/
extern int fastboot_pstore_read(void* buf, int len);

/**
    system wide uninitialization
	once uninitialized, FastbootPStore can't be used anymore, even after calling fastboot_pstore_init() again

    @return
        - @b 0 if success
        - @b otherwise if fail
*/
extern int fastboot_pstore_uninit(void);

#endif // _FASTBOOTPSTORE_H

//@}
