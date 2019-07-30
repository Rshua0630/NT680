/**
    Header file of delay function

    This is the public header of delay function that provide delay in millisecond or microsecond.

    @file       Delay.h
    @ingroup    mIUtilDelay
    @note       Nothing.

    Copyright   Novatek Microelectronics Corp. 2012.  All rights reserved.
*/

#ifndef _DELAY_H
#define _DELAY_H

#include "Type.h"

/**
    @addtogroup mIUtilDelay
*/
//@{

extern void     Delay_DelayMs(UINT32 uiMS);
extern void     Delay_DelayUs(UINT32 uiUS);
extern void     Delay_DelayUsPolling(UINT32 uiUS);
extern void     Delay_SetUsThreshold(UINT32 uiUS);

//@}

#endif
