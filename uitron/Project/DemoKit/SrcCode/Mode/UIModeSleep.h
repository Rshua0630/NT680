/*
    Copyright   Novatek Microelectronics Corp. 2005.  All rights reserved.

    @file       UIMode.h
    @ingroup    mIPRJAPUIFlow

    @brief      UI Mode Functions
                This file is the user interface ( for interchange flow control).

    @note       Nothing.

    @date       2005/04/01
*/

#ifndef _UIMODESLEEP_H
#define _UIMODESLEEP_H

#include "SysMain.h"
extern SYS_MODE gModeSleep;
extern int PRIMARY_MODE_SLEEP;   ///< Sleep

extern UINT32 _SECTION(".kercfg_data") MODE_SLEEP_TSK_ID;
extern UINT32 _SECTION(".kercfg_data") FLG_ID_MODE_SLEEP;
extern void ModeSleep_InstallID(void) _SECTION(".kercfg_text");


#endif //_UIMODESLEEP_H
