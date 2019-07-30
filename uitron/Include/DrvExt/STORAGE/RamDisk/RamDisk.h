/**
    Header file of RAM disk drvier.

    @file       RamDisk.h
    @ingroup    mICardRAM
    @note       Nothing.

    Copyright   Novatek Microelectronics Corp. 2004.  All rights reserved.
*/

#ifndef _RAMDISK_H
#define _RAMDISK_H

#include "StrgDef.h"

/**
    @addtogroup mICardRAM
*/
//@{


/**
    Get Ram Disk storage obect

    @return Pointer of storage object
*/
PSTORAGE_OBJ ramdsk_getStorageObject(void);

/**
    Get Ram Disk pstore storage obect

    @return Pointer of storage object
*/

PSTORAGE_OBJ ramdsk_ps_getStorageObject(void);

//@}

#endif // _SDRAMCard_H
