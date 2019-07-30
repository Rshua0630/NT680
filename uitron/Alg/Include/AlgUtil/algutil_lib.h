/**
    AlgUtil library API.

    Definitions of ALG utility functions.

    @file       algutil_lib.h
    @ingroup    AlgUtil

    Copyright   Novatek Microelectronics Corp. 2018.  All rights reserved.
*/
#ifndef _ALGUTIL_LIB_H
#define _ALGUTIL_LIB_H

#include "Type.h"

/**
    Translate DRAM address to physical address.

    Translate DRAM address to physical address.

    @param[in] addr     DRAM address

    @return physical DRAM address
*/
UINT32 algutil_get_phy_addr(UINT32 addr);

/**
    Translate DRAM address to non-cacheable address.

    Translate DRAM address to non-cacheable address.

    @param[in] addr     DRAM address

    @return non-cacheable DRAM address
*/
UINT32 algutil_get_noncache_addr(UINT32 addr);

/**
    Flush read (Device to CPU) cache with checking address is cacheable or not.

    In DMA operation, if buffer is cacheable, we have to flush read buffer before
    DMA operation to make sure CPU will read correct data.

    @param[in] addr     DRAM address
    @param[in] size     DRAM size
*/
VOID algutil_flush_read_cache(UINT32 addr, UINT32 size);

/**
    Flush write (CPU to Device) cache with checking address is cacheable or not.

    In DMA operation, if buffer is cacheable, we have to flush write buffer before
    DMA operation to make sure DMA will send correct data.

    @param[in] addr     DRAM address
    @param[in] size     DRAM size
*/
VOID algutil_flush_write_cache(UINT32 addr, UINT32 size);

#endif  //_ALGUTIL_LIB_H