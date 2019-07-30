/**
    Test DRAM.

    @file       TestDRAM.h
    @ingroup    mIPRJAPCfg
    @note       Nothing.

    Copyright   Novatek Microelectronics Corp. 2005.  All rights reserved.
*/

#ifndef _TESTDRAM_H
#define _TESTDRAM_H

#include "Type.h"

/**
    @addtogroup mIPRJAPCfg
*/
//@{

extern BOOL     TestDRAM_WriteReadWord(UINT32 uiStartAddr, UINT32 uiSize);
extern BOOL     TestDRAM_WriteReadHalfWord(UINT32 uiStartAddr, UINT32 uiSize);
extern BOOL     TestDRAM_WriteReadByte(UINT32 uiStartAddr, UINT32 uiSize);

extern BOOL     TestDram_WriteSpecificPtnByte(UINT32 uiStartAddr, UINT32 uiSize);
extern BOOL     TestDram_WriteSpecificPtnHWord(UINT32 uiStartAddr, UINT32 uiSize);

//@}

#endif
