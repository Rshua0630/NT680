/*
    Copyright   Novatek Microelectronics Corp. 2005.  All rights reserved.

    @file       UICommon.h
    @ingroup    mIPRJAPUIFlow

    @brief      UI Common Interfaces
                This file is the user interface ( for LIB callback function).

    @note       Nothing.

    @date       2005/04/01
*/

#ifndef _UICOMMON_H
#define _UICOMMON_H

//V (UI)
#include "PrjCfg.h"
#include "UIFrameworkExt.h"
#if(UI_FUNC==ENABLE)
#include "UIDisplayUtil.h"
#include "UIGraphics.h"
#include "UIDisplay.h"
#endif
//#include "UIAnimation.h"
#include "GxRect.h"
#include "CoordTs.h"

#include "KeyDef.h"
#if (INPUT_FUNC == ENABLE)
#include "GxInput.h"
#include "SysInput_API.h"
#endif

//C (INPUT EVENTS)
///////////////////////////////////////////////////////////////////////////////
//SysInput
extern void Input_SetAnyKeyUnlock(BOOL en);
#if (INPUT_FUNC == ENABLE)
extern void Input_SetKeyMask(KEY_STATUS uiMode, UINT32 uiMask);
extern UINT32 Input_GetKeyMask(KEY_STATUS uiMode);
extern void Input_SetKeySoundMask(KEY_STATUS uiMode, UINT32 uiMask);
extern UINT32 Input_GetKeySoundMask(KEY_STATUS uiMode);
#endif
extern void Input_SetTouchMask(UINT32 uiMask);
extern void Input_SetTouchSoundMask(UINT32 uiMask);
extern UINT32 Input_GetTouchSoundMask(void);
extern UINT32 Input_GetCurrStatus(UINT32 status);
extern UINT32 Input_Key2Mode(UINT32 keys);

//--------------------------------------------------------------------------------------
//  Control (unlock/lock detect flow and device control)
//--------------------------------------------------------------------------------------
extern void UI_LockAutoSleep(void);
extern void UI_UnlockAutoSleep(void);
extern void UI_LockAutoPWROff(void);
extern void UI_UnlockAutoPWROff(void);
extern void UI_LockStatusEvent(void);
extern void UI_LockEvent(void);
extern void UI_UnlockEvent(void);
extern UINT32 UI_IsForceLockAutoSleep(void);
extern UINT32 UI_IsForceLockAutoPWROff(void);
extern UINT32 UI_IsForceLockStatus(void);
extern UINT32 UI_IsForceLock(void);

extern void UserWaitEvent(NVTEVT wait_evt, UINT32 *wait_paramNum, UINT32 *wait_paramArray);


#endif //_UICOMMON_H_

