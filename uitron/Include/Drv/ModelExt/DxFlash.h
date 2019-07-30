/**
    Flash charge IO control.

    This module is the driver of GxFlash system. It handles the IO control of flash charge and flash trigger.

    @file       DxFlash.h
    @ingroup    mIDrvFlashCharge

    Copyright   Novatek Microelectronics Corp. 2012.  All rights reserved.
*/
#ifndef _DXFLASH_H_
#define _DXFLASH_H_

#include "Type.h"

/**
    @addtogroup mIDrvFlashCharge
*/
//@{

///////////////////////////////////////////////////////////////////////////////
// Flash
///////////////////////////////////////////////////////////////////////////////
/**
    Enable/Disable flash charge.

    @param[in] bCharge:  enable or disable charge
    @param[in] bHighCurrent:  if set charge current High

*/
extern void DrvFlash_ChargeFlash(BOOL bCharge, BOOL bHighCurrent);

/**
    Check if the flash is charging now.

    @return  TRUE:  Flash is charging.
             FALSE: Flash is not charging.
*/
extern BOOL DrvFlash_IsCharge(void);

/**
    Check if the flash is ready(charge full).

    @return  TRUE:  Flash is ready.
             FALSE: Flash is not ready.
*/
extern BOOL DrvFlash_IsFlashRdy(void);

/**
    Start/Stop the flash trigger.

    @param[in] bEn: Start or Stop trigger.
*/
extern void DrvFlash_TriggerFlash(BOOL bEn);

//@}
#endif //_DXFLASH_H_