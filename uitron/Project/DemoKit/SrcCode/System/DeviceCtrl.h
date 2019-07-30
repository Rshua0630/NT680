/**
    Copyright   Novatek Microelectronics Corp. 2005.  All rights reserved.

    @file       DeviceCtrl.h
    @ingroup    mIPRJAPCfg

    @brief      Header file of DeviceCtrl.c
                Header file of DeviceCtrl.c

    @note       Nothing.

    @date       2005/12/09
*/

/** \addtogroup mIPRJAPCfg */
//@{

#ifndef _DEVICECTRL_H
#define _DEVICECTRL_H

#include "Type.h"

extern BOOL     DevCtrl_Lens(BOOL bIsOpen);
extern void     DevCtrl_LensDriverInit(void);
extern BOOL     DevCtrl_Lens_PowerOff_IsNeedRetract(void);
extern void     DevCtrl_Lens_PowerOff_Retract(void);
extern void     DevCtrl_Lens_Playback_Retract(void);

#endif

//@}
